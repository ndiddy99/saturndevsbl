//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// インプレースリストボックスクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>

#include "InplaceListBox.h"


// アロケーションの監視
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// メッセージマップ
IMPLEMENT_DYNCREATE (CInplaceListBox, CListBox)

BEGIN_MESSAGE_MAP (CInplaceListBox, CListBox)
	ON_WM_KILLFOCUS ()
	ON_WM_KEYDOWN ()
	ON_WM_LBUTTONDBLCLK ()
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CInplaceListBox::CInplaceListBox () {

}

// デストラクタ
CInplaceListBox::~CInplaceListBox () {
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// フォーカスが失われたとき
void CInplaceListBox::OnKillFocus (CWnd* pNewWnd) {
	_RPTF1 (_CRT_WARN, "CInplaceListBox::OnKillFocus (pNewWnd=0x%08x)\n", pNewWnd);
	CWnd* pParentWnd = GetParent ();
	if (pParentWnd != pNewWnd) {
		pParentWnd->SendMessage (WM_KILLFOCUS, (WPARAM)pNewWnd, (LPARAM)0L);
	}
	else {
		//pParentWnd->SetFocus ();
	}
}

// キー押し下げ時
void CInplaceListBox::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CWnd* pParentWnd = GetParent ();
	switch (nChar) {
	case VK_UP:
	case VK_LEFT:
	case VK_RIGHT:
	case VK_DOWN:
		//TODO:上下左右キーは編集終了か選択できるようにしておく
		//GetParent ()->SendMessage (WM_KEYDOWN, VK_RETURN, (nRepCnt << 16) | nFlags);
		//GetParent ()->PostMessage (WM_KEYDOWN, nChar, (nRepCnt << 16) | nFlags);
		break;
		return;
	case VK_RETURN:
		//GetParent ()->SendMessage (WM_KEYDOWN, VK_RETURN, (nRepCnt << 16) | nFlags);
		pParentWnd->SetFocus ();
		pParentWnd->PostMessage (WM_KEYDOWN, VK_RETURN, (nRepCnt << 16) | nFlags);
		break;
	case VK_ESCAPE:
		//GetParent ()->SendMessage (WM_KEYDOWN, VK_ESCAPE, (nRepCnt << 16) | nFlags);
		pParentWnd->SetFocus ();
		pParentWnd->PostMessage (WM_KEYDOWN, VK_ESCAPE, (nRepCnt << 16) | nFlags);
		break;
	}
	CListBox::OnKeyDown (nChar, nRepCnt, nFlags);
}

// マウス左ボタンダブルクリック時
void CInplaceListBox::OnLButtonDblClk (UINT nFlags, CPoint point) {
	// 選択項目の確定信号(VK_RETURN)を親ウィンドウに送る
	GetParent ()->SendMessage (WM_KEYDOWN, VK_RETURN, (1 << 16) | 0);
	CListBox::OnLButtonDblClk (nFlags, point);
}
