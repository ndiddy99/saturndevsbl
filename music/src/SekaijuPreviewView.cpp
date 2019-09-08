//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹印刷プレビュービュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// このクラスの役割はMFC4.0風のSDIの印刷プレビュービューを実装することである。
// 特に印刷プレビューツールバーの各ボタンの外見と動作をオーバーライドする。

#include "winver.h"
#include <afxwin.h>
#include <afxpriv.h>
#include <afxext.h>
#include <afxmt.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuDoc.h"
#include "SekaijuView.h"
#include "SekaijuPreviewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE (CSekaijuPreviewView, CPreviewView)

BEGIN_MESSAGE_MAP (CSekaijuPreviewView, CPreviewView)
	ON_COMMAND (AFX_ID_PREVIEW_CLOSE, OnPreviewClose)
	//ON_COMMAND (AFX_ID_PREVIEW_NUMPAGE, OnNumPageChange)
	//ON_COMMAND (AFX_ID_PREVIEW_NEXT, OnNextPage)
	//ON_COMMAND (AFX_ID_PREVIEW_PREV, OnPrevPage)
	ON_COMMAND (AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
	//ON_COMMAND (AFX_ID_PREVIEW_ZOOMIN, OnZoomIn)
	//ON_COMMAND (AFX_ID_PREVIEW_ZOOMOUT, OnZoomOut)
	//ON_UPDATE_COMMAND_UI (AFX_ID_PREVIEW_NUMPAGE, OnUpdateNumPageChange)
	//ON_UPDATE_COMMAND_UI (AFX_ID_PREVIEW_NEXT, OnUpdateNextPage)
	//ON_UPDATE_COMMAND_UI (AFX_ID_PREVIEW_PREV, OnUpdatePrevPage)
	//ON_UPDATE_COMMAND_UI (AFX_ID_PREVIEW_ZOOMIN, OnUpdateZoomIn)
	//ON_UPDATE_COMMAND_UI (AFX_ID_PREVIEW_ZOOMOUT, OnUpdateZoomOut)
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CSekaijuPreviewView::CSekaijuPreviewView() {
}

// デストラクタ
CSekaijuPreviewView::~CSekaijuPreviewView () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

void CSekaijuPreviewView::SetPreviewState (CPrintPreviewState* pPreviewState) {
	this->m_pPreviewState = pPreviewState;
}

void CSekaijuPreviewView::SetToolBar (CDialogBar* pToolBar) {
	this->m_pToolBar = pToolBar;
}

void CSekaijuPreviewView::ActivateView (BOOL bActivate, CView* pActivateView,
										CView* pDeactiveView) {
	OnActivateView (bActivate, pActivateView, pDeactiveView);
}


void CSekaijuPreviewView::PreviewClose () {
	CSekaijuPreviewView::OnPreviewClose ();
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// 印刷プレビューツールバー-『閉じる』が押されたとき
void CSekaijuPreviewView::OnPreviewClose() {
	m_pToolBar->DestroyWindow();
	m_pToolBar = NULL;
	m_pPreviewInfo->m_nCurPage = m_nCurrentPage;
	((CSekaijuView*)m_pOrigView)->EndPrintPreview
		(m_pPreviewDC, m_pPreviewInfo, CPoint(0, 0), this);
}

// 印刷プレビューツールバー-『印刷』が押されたとき
void CSekaijuPreviewView::OnPreviewPrint() {
	CView* pOrigView = m_pOrigView;
	OnPreviewClose (); // 印刷プレビューを強制的に閉じる
	CWnd* pMainWnd = AfxGetThread ()->m_pMainWnd;
	ASSERT_VALID (pMainWnd);
	pMainWnd->SendMessage (WM_COMMAND, ID_FILE_PRINT);
}
