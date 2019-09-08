﻿//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリストプロパティスケールビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxmt.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "mousewheel.h"
#include "ColorfulComboBox.h"
#include "ColorfulCheckListBox.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuToolBar.h"
#include "SekaijuStatusBar.h"
#include "ChildFrame.h"
#include "EventListFrame.h"
#include "EventListPropertyScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE (CEventListPropertyScaleView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CEventListPropertyScaleView, CSekaijuView)
	ON_WM_CREATE ()
	ON_WM_KEYDOWN ()
	ON_WM_LBUTTONDOWN ()	
	ON_WM_RBUTTONDOWN ()	
	ON_WM_LBUTTONUP ()	
	ON_WM_RBUTTONUP ()	
	ON_WM_MOUSEMOVE ()
	ON_WM_MOUSEWHEEL40 ()
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEventListPropertyScaleView::CEventListPropertyScaleView () {
}

// デストラクタ
CEventListPropertyScaleView::~CEventListPropertyScaleView () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 原点の移動
void CEventListPropertyScaleView::OnPrepareDC (CDC* pDC, CPrintInfo* pInfo) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	pDC->SetWindowOrg (pEventListFrame->GetColumnScrollPos (), 0);
}

// 描画
void CEventListPropertyScaleView::OnDraw (CDC* pDC) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	pSekaijuDoc->m_theCriticalSection.Lock ();
	MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	long lColorBtnShadow = ::GetSysColor (COLOR_BTNSHADOW);
	long lColorBtnHighlight = ::GetSysColor (COLOR_BTNHIGHLIGHT);
	long lColorBtnText = ::GetSysColor (COLOR_BTNTEXT);
	CRect rcClient;
	GetClientRect (&rcClient);
	pDC->DPtoLP (&rcClient);
	// 背景の塗りつぶし
	pDC->FillSolidRect (&rcClient, lColorBtnFace);
	pDC->SetBkMode (TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject (&(pSekaijuApp->m_theFont[0]));
	CRect theRect (0, 0, 0, 0);
	long lColumnZoom = pEventListFrame->GetColumnZoom ();
	pDC->SetTextColor (lColorBtnText);
	for (long j = 0; j < 8; j++) {
		theRect.top = rcClient.top;
		theRect.bottom = rcClient.bottom;
		theRect.left = theRect.right; 
		theRect.right = theRect.right + pEventListFrame->GetColumnBaseWidth (j) * lColumnZoom;
		pDC->Draw3dRect (&theRect, lColorBtnHighlight, lColorBtnShadow);
		pDC->DrawText (pEventListFrame->GetColumnTitle (j), &theRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	pDC->SelectObject (pOldFont);
	pSekaijuDoc->m_theCriticalSection.Unlock ();
	::Sleep (0);
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ生成時
BOOL CEventListPropertyScaleView::OnCreate (LPCREATESTRUCT lpcs) {
	return CSekaijuView::OnCreate (lpcs);
}

// キー押し下げ時
void CEventListPropertyScaleView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	pEventListFrame->PostMessage (WM_KEYDOWN, nChar, (nFlags << 16) | nRepCnt);
}

// マウス左ボタン押された時  
void CEventListPropertyScaleView::OnLButtonDown (UINT nFlags, CPoint point) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	point += CSize (pEventListFrame->GetColumnScrollPos (), 0);
	// カーソルが境界上にあるか調べる
	long j = 0;
	long lBorderX = 0;
	for (j = 0; j < 8; j++) {
		lBorderX += pEventListFrame->GetColumnBaseWidth (j) * pEventListFrame->GetColumnZoom ();
		if (lBorderX - 2 <= point.x && point.x <= lBorderX + 2) {
			break;
		}
	}
	// カーソルが境界上にあった場合
	if (0 <= j && j < 8) {
		SetCapture ();
		m_lTempColumnIndex = j;
		m_lTempColumnBaseWidth = pEventListFrame->GetColumnBaseWidth (j);
		m_ptMouseDown = m_ptMouseMoveOld = point;
		::SetCursor (pSekaijuApp->m_hCursorSizeWE);
	}
}

// マウス右ボタン押された時  
void CEventListPropertyScaleView::OnRButtonDown (UINT nFlags, CPoint point) {

}

// マウス左ボタン離されたとき
void CEventListPropertyScaleView::OnLButtonUp (UINT nFlags, CPoint point) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	if (GetCapture () == this) {
		ReleaseCapture ();
		::SetCursor (pSekaijuApp->m_hCursorArrow);
		pEventListFrame->RecalcColumnScrollInfo ();
		pEventListFrame->m_pPropertyScaleView->Invalidate ();
		pEventListFrame->m_pIndexPropertyView->Invalidate ();
	}
}

// マウス右ボタン離されたとき
void CEventListPropertyScaleView::OnRButtonUp (UINT nFlags, CPoint point) {

}

// マウスが動かされたとき
void CEventListPropertyScaleView::OnMouseMove (UINT nFlags, CPoint point) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	point += CSize (pEventListFrame->GetColumnScrollPos (), 0);
	// キャプター中
	if (GetCapture () == this) {
		CRect rcClient;
		GetClientRect (&rcClient);
		CSize szMouseDelta = point.x - m_ptMouseDown.x;
		long lNewColumnBaseWidth = 
			(m_lTempColumnBaseWidth * pEventListFrame->GetColumnZoom () + szMouseDelta.cx) / 
			pEventListFrame->GetColumnZoom ();
		lNewColumnBaseWidth = CLIP (1, lNewColumnBaseWidth, 1024);
		if (lNewColumnBaseWidth != pEventListFrame->GetColumnBaseWidth (m_lTempColumnIndex)) {
			CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
			pEventListFrame->SetColumnBaseWidth (m_lTempColumnIndex, lNewColumnBaseWidth);
			pEventListFrame->m_pPropertyScaleView->Invalidate ();
			pEventListFrame->m_pIndexPropertyView->Invalidate ();
		}
		m_ptMouseMoveOld = point;
	}
	// 非キャプター中
	else {
		// カーソルが境界上にあるか調べる
		long j = 0;
		long lBorderX = 0;
		for (j = 0; j < 8; j++) {
			lBorderX += pEventListFrame->GetColumnBaseWidth (j) * pEventListFrame->GetColumnZoom ();
			if (lBorderX - 2 <= point.x && point.x <= lBorderX + 2) {
				break;
			}
		}
		// カーソルが境界上にあった場合
		if (0 <= j && j < 8) {
			::SetCursor (pSekaijuApp->m_hCursorSizeWE);
		}
		// カーソルが境界上にない場合
		else {
			::SetCursor (pSekaijuApp->m_hCursorArrow);
		}
	}

}

// マウスホイールが回された時
void CEventListPropertyScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CEventListFrame* pEventListFrame = (CEventListFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pEventListFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pEventListFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	else {
		long lRowScrollPos = pEventListFrame->GetRowScrollPos ();
		long lRowZoom = pEventListFrame->GetRowZoom ();
		lRowScrollPos -= lRowZoom * lDelta / WHEELDELTA;
		pEventListFrame->SetRowScrollPos (lRowScrollPos);
		pEventListFrame->m_bAutoPageUpdate = FALSE;
	}
}
