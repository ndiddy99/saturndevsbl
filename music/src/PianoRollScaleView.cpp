﻿//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールスケールビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxcmn.h>
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
#include "MainFrame.h"
#include "ChildFrame.h"
#include "PianoRollFrame.h"
#include "PianoRollScaleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE (CPianoRollScaleView, CSekaijuView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CPianoRollScaleView, CSekaijuView)
	ON_WM_KEYDOWN ()
	ON_WM_MOUSEWHEEL40 ()
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CPianoRollScaleView::CPianoRollScaleView () {
}

// デストラクタ
CPianoRollScaleView::~CPianoRollScaleView () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 描画
void CPianoRollScaleView::OnDraw (CDC* pDC) {
	CSekaijuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rcClient;
	GetClientRect (&rcClient);
	long lColorBtnFace = ::GetSysColor (COLOR_BTNFACE);
	pDC->FillSolidRect (&rcClient, lColorBtnFace);
}

// ビューの更新
void CPianoRollScaleView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) {
	// クリティカルセクションはロックされているものとする。
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	// 演奏開始した場合、リアルタイム入力開始した場合、位置移動した場合
	if ((lHint & SEKAIJUDOC_PLAYSTARTED) || 
		(lHint & SEKAIJUDOC_RECORDSTARTED) ||
		(lHint & SEKAIJUDOC_POSITIONCHANGED)) {
		if (pSekaijuApp->m_theGeneralOption.m_bEnableAutoPageUpdate) { // 20091224追加
			pPianoRollFrame->m_bAutoPageUpdate = TRUE;
		}
	}
	// MIDIデータが変更された場合
	if (lHint & SEKAIJUDOC_MIDIDATACHANGED) { /* 20091024追加 */
		pPianoRollFrame->UpdateSnapCombo (); /* 20091024追加 */
		pPianoRollFrame->UpdateDurationCombo (); /* 20091024追加 */
	}
	// MIDIトラックが変更された場合
	if (lHint & SEKAIJUDOC_MIDITRACKCHANGED) {
		pPianoRollFrame->UpdateTrackList ();
		pPianoRollFrame->UpdateTrackCombo ();
	}
	// MIDIデータ又はMIDIトラック又はMIDIイベントが変更された場合
	if ((lHint & SEKAIJUDOC_MIDIDATACHANGED) ||
		(lHint & SEKAIJUDOC_MIDITRACKCHANGED) ||
		(lHint & SEKAIJUDOC_MIDIEVENTCHANGED)) {
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		long lEndTime = MIDIData_GetEndTime (pMIDIData);
		pMainFrame->SetPositionScrollRange (0, lEndTime, TRUE);
		pPianoRollFrame->RecalcKeyScrollInfo ();
		pPianoRollFrame->RecalcVelScrollInfo ();
		pPianoRollFrame->RecalcTimeScrollInfo ();
	}
	CSekaijuView::OnUpdate (pSender, lHint, pHint);
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// キーが押された時
void CPianoRollScaleView::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	pPianoRollFrame->PostMessage (WM_KEYDOWN, nChar, (nFlags << 16) | nRepCnt);
	return;
}

// マウスホイールが回された時
void CPianoRollScaleView::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CPianoRollFrame* pPianoRollFrame = (CPianoRollFrame*)GetParent ();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = GetDocument ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			pPianoRollFrame->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			pPianoRollFrame->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
	else {
		long lKeyScrollPos = pPianoRollFrame->GetKeyScrollPos ();
		long lKeyZoom = pPianoRollFrame->GetKeyZoom ();
		lKeyScrollPos -= lKeyZoom * lDelta / WHEELDELTA;
		pPianoRollFrame->SetKeyScrollPos (lKeyScrollPos);
	}
}



