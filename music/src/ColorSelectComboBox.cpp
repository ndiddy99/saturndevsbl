//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 色選択コンボボックスクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxdlgs.h>
#include "ColorSelectComboBox.h"

// メッセージマップ
IMPLEMENT_DYNCREATE (CColorSelectComboBox, CComboBox)

// メッセージマップ
BEGIN_MESSAGE_MAP (CColorSelectComboBox, CComboBox)
	ON_CONTROL_REFLECT (CBN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// ウィンドウスタイルの変更
BOOL CColorSelectComboBox::PreCreateWindow (CREATESTRUCT& cs) {
	if (!CComboBox::PreCreateWindow (cs)) {
		return FALSE;
	}
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~(CBS_DROPDOWN | CBS_SIMPLE | CBS_OWNERDRAWVARIABLE | CBS_SORT);
	cs.style |= (CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS);
	return TRUE;
}

// 項目の高さ
void CColorSelectComboBox::MeasureItem (LPMEASUREITEMSTRUCT lpmis) {
	lpmis->itemHeight = 14;
}

// 項目の描画
void CColorSelectComboBox::DrawItem (LPDRAWITEMSTRUCT lpdis) {
	CDC dc;
	dc.Attach (lpdis->hDC);
	CRect rect = lpdis->rcItem;
	int nIndex = lpdis->itemID;

	CBrush* pBrush = new CBrush;
	pBrush->CreateSolidBrush (::GetSysColor ((lpdis->itemState &
		ODS_SELECTED) ? COLOR_HIGHLIGHT : COLOR_WINDOW));
	dc.FillRect (rect, pBrush);
	delete pBrush;

	if (lpdis->itemState & ODS_FOCUS) {
		dc.DrawFocusRect (rect);
	}

	if (nIndex != (UINT)-1) {
		COLORREF clrSample = (COLORREF)GetItemData (nIndex);
		if (clrSample == COLORSELECTCOMBOBOX_OTHER) {
			CFont* pFont = CFont::FromHandle 
				((HFONT)(::GetStockObject (DEFAULT_GUI_FONT)));
			CFont* pOldFont = dc.SelectObject (pFont);
			dc.SetBkMode (TRANSPARENT);
			dc.DrawText (_T("Others..."), rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			dc.SelectObject (pOldFont);
		}
		else {
			CRect rcSample (rect);
			rcSample.DeflateRect (3, 3);
			dc.FillSolidRect (rcSample, clrSample);
		}
	}
	else {
		CRect rcSample (rect);
		rcSample.DeflateRect (3, 3);
		dc.FillSolidRect (rcSample, m_clrCurColor);		
	}
	dc.Detach ();
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// 現在の色を設定する(この関数はいつでも使える。)
BOOL CColorSelectComboBox::SetCurColor (COLORREF clr) {
	if (clr < 0x00000000 || clr > 0x00FFFFFF) {
		return FALSE;
	}
	m_clrCurColor = clr;
	if (m_hWnd != NULL) {
		int nCount = GetCount ();
		int i = 0;
		// 一致するカラーサンプルを選択状態にする
		for (i = 0; i <= nCount; i++) {
			COLORREF clrSampleColor = GetItemData (i);
			if (clrSampleColor == m_clrCurColor) {
				SetCurSel (i);
				break;
			}
		}
		// 一致するカラーサンプルがなかったので選択解除
		if (i >= nCount) {
			SetCurSel (-1);
		}
		Invalidate ();
	}
	return TRUE;
}

// すべてのカラーサンプルをクリアする。
void CColorSelectComboBox::ClearSampleColor () {
	ResetContent ();
}

// カラーサンプルを追加する。
int CColorSelectComboBox::AddSampleColor (COLORREF clr) {
	int nIndex = AddString (_T(""));
	if (nIndex == CB_ERR || nIndex == CB_ERRSPACE) {
		return nIndex;
	}
	return SetSampleColor (nIndex , (DWORD)clr);
}

// カラーサンプルを取得する。
COLORREF CColorSelectComboBox::GetSampleColor (int nIndex) const {
	return (COLORREF)GetItemData (nIndex);
}

// カラーサンプルを設定する。
int CColorSelectComboBox::SetSampleColor (int nIndex, COLORREF clr) {
	int nRet = SetItemData (nIndex, (DWORD)clr);
	int nCurSel = GetCurSel ();
	// 新しいカラーサンプルとカレントカラーの一致チェック
	if (nCurSel == CB_ERR || nCurSel == nIndex) { 
		COLORREF clrSampleColor = GetItemData (nIndex);
		// 新しいカラーサンプルとカレントカラーは一致したので選択
		if (clrSampleColor == m_clrCurColor) {
			SetCurSel (nIndex);
		}
		// 新しいカラーサンプルとカレントカラーは一致しなかったので選択解除
		else {
			SetCurSel (-1);
		}
	}
	return nRet;
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// カレントセルが変更されたとき(メッセージリフレクション)
void CColorSelectComboBox::OnSelChange () {
	int nCurSel = GetCurSel ();
	if (0 <= nCurSel && nCurSel < GetCount ()) {
		COLORREF clrCurColor = (COLORREF)GetItemData (nCurSel);
		if (clrCurColor == COLORSELECTCOMBOBOX_OTHER) {
			CColorDialog theDlg;
			theDlg.m_cc.Flags |= (CC_FULLOPEN | CC_RGBINIT);
			theDlg.m_cc.rgbResult = m_clrCurColor;
			if (theDlg.DoModal () == IDOK) {
				m_clrCurColor = theDlg.GetColor ();
			}
			SetCurSel (-1);
		}
		else {
			m_clrCurColor = clrCurColor;
		}
		Invalidate ();
	}
}
