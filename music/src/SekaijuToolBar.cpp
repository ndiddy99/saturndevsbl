//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹ツールバークラス
// (C)2002-2018 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxcmn.h>
#include <afxmt.h>
#include <afxThemeHelper.h>
#include <uxtheme.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "Resource.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuToolBar.h"
#include "SekaijuStatusBar.h"
#include "MainFrame.h"
#include "SekaijuDoc.h"


#define RP_BACKGROUND 6

// デバッグ用
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC (CSekaijuToolBar, CToolBar)

// メッセージマップ
BEGIN_MESSAGE_MAP (CSekaijuToolBar, CToolBar)
	ON_WM_NCPAINT()
	ON_WM_HSCROLL ()
	ON_CBN_SELCHANGE (IDC_KEYSHIFTCOMBO, OnKeyShiftChange)
	ON_WM_DRAWITEM ()
	ON_WM_ERASEBKGND ()
	ON_NOTIFY_REFLECT (NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CSekaijuToolBar::CSekaijuToolBar () {
}

// デストラクタ
CSekaijuToolBar::~CSekaijuToolBar () {
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// http://support.microsoft.com/kb/843490/jaの問題に対応
void CSekaijuToolBar::EraseNonClient () {
	CWindowDC dc (this);
	CRect rectClient;
	GetClientRect (rectClient);
	CRect rectWindow;
	GetWindowRect (rectWindow);
	ScreenToClient (rectWindow);
	rectClient.OffsetRect (-rectWindow.left, -rectWindow.top);
	dc.ExcludeClipRect (rectClient);

	// 下の位置よりもこの位置でWM_ERASEBKGNDを送信したほうがグリッパーが正しく描画される。
	SendMessage (WM_ERASEBKGND, (WPARAM)dc.m_hDC);

	// m_hReBarThemeのない場合(クラシックモードなど)の背景色を設定しておく。
	dc.SetBkColor (::GetSysColor (COLOR_3DFACE));

	// draw borders in non-client area
	rectWindow.OffsetRect (-rectWindow.left, -rectWindow.top);
	DrawBorders (&dc, rectWindow);

	// erase parts not drawn
	dc.IntersectClipRect (rectWindow);
	//SendMessage (WM_ERASEBKGND, (WPARAM)dc.m_hDC);

	// draw gripper in non-client area
	// DrawGripper (&dc, rectWindow);
	DrawNCGripper (&dc, rectWindow);
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// http://support.microsoft.com/kb/843490/jaの問題に対応
void CSekaijuToolBar::DoPaint (CDC* pDC) {
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	// Paint inside the client area.
	CRect rect;
	GetClientRect (rect);
	DrawBorders (pDC, rect);
	DrawGripper (pDC, rect);
}

// http://support.microsoft.com/kb/843490/jaの問題に対応
void CSekaijuToolBar::DrawGripper (CDC* pDC, const CRect& rect) {
	pDC->FillSolidRect (&rect, ::GetSysColor (COLOR_BTNFACE)); //VC++4.0用
	CToolBar::DrawGripper (pDC,rect); //VC++4.0std(MFC4.0)では未対応
}

// 境界線のないツールバーでクライアントエリア外の背景が描画されないのでオーバーライド
void CSekaijuToolBar::DrawBorders (CDC* pDC, CRect& rect) {
	DWORD dwStyle = m_dwStyle;
	if (IsKindOf(RUNTIME_CLASS(CToolBar)) && (dwStyle & TBSTYLE_FLAT) && (dwStyle & CBRS_BORDER_ANY)) {
		CToolBar::DrawBorders (pDC, rect);
		return;
	}
	// 以下、CBRS_BORDER_ANYでない場合でもツールバーの外側にスペースを確保している場合があるので
	// 背景は描画されなければならない。
	CRect rectWindow = rect;
	HRESULT hr = E_FAIL;
	if (m_hReBarTheme) {
		if (CThemeHelper::IsThemeBackgroundPartiallyTransparent
			(m_hReBarTheme, RP_BACKGROUND, 0)) {
			CThemeHelper::DrawThemeParentBackground (m_hWnd, pDC->m_hDC, &rectWindow);
		}
		hr = CThemeHelper::DrawThemeBackground
			(m_hReBarTheme, pDC->m_hDC, RP_BACKGROUND, 0, &rectWindow, NULL);
	}
	if (FAILED(hr)) {
		pDC->FillSolidRect ((LPRECT)rectWindow, pDC->GetBkColor());
	}
	return;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// http://support.microsoft.com/kb/843490/jaの問題に対応
void CSekaijuToolBar::OnNcPaint() {
	//Invalidate (TRUE); // VC++4.0std(MFC4.0)でツールバーが再描画されないのを防止
	EraseNonClient();
}

// 位置スクロールバーを動かしたときの挙動(演奏位置移動)
void CSekaijuToolBar::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if (pMainFrame == NULL) {
		return;
	}
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pMainFrame->GetActiveFrame ()->GetActiveDocument());
	if (pSekaijuDoc == NULL) {
		return;
	}
	if (pScrollBar == &(pMainFrame->m_wndPositionScroll)) {
		SCROLLINFO si;
		pScrollBar->GetScrollInfo (&si, SIF_ALL);
		switch (nSBCode) {
		case SB_LINELEFT:
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_PREVBEAT, 0);
			break;
		case SB_LINERIGHT:
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_NEXTBEAT, 0);
			break;
		case SB_PAGELEFT:
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE, 0);
			break;
		case SB_PAGERIGHT:
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE, 0);
			break;
		case SB_LEFT:
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_TOBEGIN, 0);
			break;
		case SB_RIGHT:
			pMainFrame->SendMessage (WM_COMMAND, ID_CONTROL_TOEND, 0);
			break;
		case SB_THUMBTRACK:
			if (!pSekaijuApp->m_bRecording) {
				pSekaijuDoc->m_theCriticalSection.Lock ();
				pSekaijuApp->SendAllNoteOff ();
				pSekaijuApp->SendAllHold1Off ();
				pSekaijuApp->SendAllSostenutoOff ();
				pSekaijuApp->SendAllHold2Off ();
				pSekaijuApp->SendAllSoundOff ();
				pSekaijuApp->SetPlayPosition (pSekaijuDoc, si.nTrackPos);
				pSekaijuDoc->UpdateAllViews (NULL, SEKAIJUDOC_POSITIONCHANGED);
				pSekaijuDoc->m_theCriticalSection.Unlock ();
			}
			break;
		}
	}
}

// キーシフトの値が変更されたとき
void CSekaijuToolBar::OnKeyShiftChange () {
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CComboBox* pKeyShiftCombo = &(pMainFrame->m_wndKeyShiftCombo);
	pMainFrame->PostMessage (WM_COMMAND, ID_CONTROL_KEYSHIFT12 + pKeyShiftCombo->GetCurSel ());
}

// ツールバーのクライアントエリアの背景描画要求時
// デフォルトのMFC状態ではTBCS_TRANSPARENTの背景が正しく描画されない。
// 具体的には、ツールバーのクライアントエリアの背景はCDockBarの画像、
// クライアントエリア外側とグリッパー背景はCReBarの画像となる。
// ここではクライアントエリアの背景をCRebarの画像に合わせる。
BOOL CSekaijuToolBar::OnEraseBkgnd (CDC* pDC) {
	CWindowDC dc (this);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	HRESULT hr = E_FAIL;
	// m_hReBarThemeのない場合(クラシックモードなど)の背景色を設定しておく。
	pDC->SetBkColor (::GetSysColor (COLOR_3DFACE));
	if (m_hReBarTheme) {
		if (CThemeHelper::IsThemeBackgroundPartiallyTransparent
			(m_hReBarTheme, RP_BACKGROUND, 0)) {
			CThemeHelper::DrawThemeParentBackground (m_hWnd, pDC->m_hDC, &rectWindow);
		}
		hr = CThemeHelper::DrawThemeBackground
			(m_hReBarTheme, pDC->m_hDC, RP_BACKGROUND, 0,&rectWindow, NULL);
	}
	if (FAILED(hr)) {
		pDC->FillSolidRect ((LPRECT)rectWindow, pDC->GetBkColor());
	}
	return TRUE;
}

// ツールバーのカスタムドロー(必要に応じてインプリメントしてください)
void CSekaijuToolBar::OnCustomDraw (NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMCUSTOMDRAW lpNMCustomDraw = (LPNMCUSTOMDRAW) pNMHDR;
	switch (lpNMCustomDraw->dwDrawStage) {
	case CDDS_PREERASE:
		break;
	case CDDS_POSTERASE:
		break;
	case CDDS_PREPAINT:
		break;
	case CDDS_POSTPAINT:
		break;
	case CDDS_ITEMPREPAINT:
		break;
	default:
		break;
	}
	*pResult = 0;
	*pResult |= CDRF_NOTIFYITEMDRAW;
	*pResult |= CDRF_NOTIFYSUBITEMDRAW;
	*pResult |= CDRF_NOTIFYPOSTPAINT;
	*pResult |= CDRF_NOTIFYPOSTERASE;
}
