//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹ビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// このクラスの役割はCSekaijuDoc* GetDocument()を提供することと、
// MFC4.0風のSDIの印刷プレビューを実現するDoPrintPreviewのオーバーライドと
// OnEndPrintPreviewのオーバーライドをインプリメントすることである。
// 実用的なビューはCSekaijuViewから派生させて作ること。
// なお、このクラスはVisible=FALSEのダミービューとしても使える。

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
#include "SekaijuToolBar.h"
#include "SekaijuStatusBar.h"
#include "MainFrame.h"
#include "SekaijuView.h"
#include "SekaijuPreviewView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE (CSekaijuView, CView)

// メッセージマップ
BEGIN_MESSAGE_MAP (CSekaijuView, CView)
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CSekaijuView::CSekaijuView() {
}

// デストラクタ
CSekaijuView::~CSekaijuView () {
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// OnEndPrintPreviewを呼び出す
void CSekaijuView::EndPrintPreview
	(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) {
	OnEndPrintPreview (pDC, pInfo, point, pView);
}

// OnEndPrintingを呼び出す
void CSekaijuView::EndPrinting(CDC* pDC, CPrintInfo* pInfo) {
	OnEndPrinting (pDC, pInfo);	
}

// OnActivateViewを呼び出す
void CSekaijuView::ActivateView (BOOL bActivate, CView* pActivateView, CView* pDeactiveView) {
	OnActivateView (bActivate, pActivateView, pDeactiveView);
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// ウィンドウスタイルの変更
BOOL CSekaijuView::PreCreateWindow (CREATESTRUCT& cs) {
	return CView::PreCreateWindow (cs);
}

// 描画(ダミー)
void CSekaijuView::OnDraw (CDC* pDC) {
	ASSERT_VALID (GetDocument());
}

// GetDocument(非デバッグ バージョンはインラインです。)
#ifdef _DEBUG
CSekaijuDoc* CSekaijuView::GetDocument () {
	ASSERT (m_pDocument->IsKindOf(RUNTIME_CLASS(CSekaijuDoc)));
	return (CSekaijuDoc*)m_pDocument;
}

#endif



// 印刷プレビュー時にメインフレームの閉じるボタンが押されたときに誘導する関数。
// 印刷プレビューはMFC4.0風(SDI方式)とする。
BOOL CALLBACK _AfxSekaijuPreviewCloseProc (CFrameWnd* pFrameWnd);

// CView::OnFilePrintPreview のオーバーライド
// 印刷プレビューはMFC4.0風(SDI方式)とする。
void CSekaijuView::OnFilePrintPreview () {
	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame. Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;
	pState->lpfnCloseProc = _AfxSekaijuPreviewCloseProc;

	if (!DoPrintPreview (AFX_IDD_PREVIEW_TOOLBAR, this,
		RUNTIME_CLASS (CSekaijuPreviewView), pState)) {
		// In derived classes, reverse special window handling
		// here for Preview failure case
		TRACE0 ("Error: DoPrintPreview failed");
		AfxMessageBox (AFX_IDP_COMMAND_FAILURE);
		delete pState;
	}
}


// CView::DoPrintPreviewのオーバーライド
// 印刷プレビューはMFC4.0風(SDI方式)とする。
BOOL CSekaijuView::DoPrintPreview (UINT nIDResource, CView* pPrintView,
	CRuntimeClass* pPreviewViewClass, CPrintPreviewState* pState) {

	ASSERT_VALID_IDR(nIDResource);
	ASSERT_VALID(pPrintView);
	ASSERT(pPreviewViewClass != NULL);
	ASSERT(pPreviewViewClass->IsDerivedFrom (RUNTIME_CLASS (CSekaijuPreviewView)));
	ASSERT(pState != NULL);

	CFrameWnd* pParent = NULL;
	pParent = (CFrameWnd*)AfxGetThread()->m_pMainWnd;
	ASSERT_VALID (pParent);
	ASSERT_KINDOF (CFrameWnd, pParent);

	CCreateContext context;
	context.m_pNewViewClass = pPreviewViewClass;
	context.m_pCurrentFrame = pParent;
	context.m_pCurrentDoc = GetDocument();
	context.m_pLastView = this;

	// Create the preview view object

	CSekaijuPreviewView* pView = (CSekaijuPreviewView*)pPreviewViewClass->CreateObject();
	if (pView == NULL) {
		TRACE0 ("Error: Failed to create preview view.\n");
		return FALSE;
	}
	ASSERT_KINDOF (CSekaijuPreviewView, pView);
	pView->SetPreviewState (pState); // save pointer

	pParent->OnSetPreviewMode (TRUE, pState); // Take over Frame Window

	// MFC4.0 (SDIの印刷プレビュー画面)
	// Create the toolbar from the dialog resource
	CDialogBar* pDialogBar = new CDialogBar;
	pView->SetToolBar (pDialogBar);
	if (!pView->GetToolBar ()->Create (pParent, MAKEINTRESOURCE(nIDResource),
		CBRS_TOP, AFX_IDW_PREVIEW_BAR)) {
		TRACE0 ("Error: Preview could not create toolbar dialog.\n");
		pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
		delete pView->GetToolBar();
		pView->SetToolBar (NULL);
		pView->SetPreviewState (NULL); // do not delete state structure
		delete pView;
		return FALSE;
	}
	pView->GetToolBar()->m_bAutoDelete = TRUE; // automatic cleanup

	// Create the preview view as a child of the App Main Window.  This
	// is a sibling of this view if this is an SDI app.  This is NOT a sibling
	// if this is an MDI app.

	if (!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), pParent, AFX_IDW_PANE_FIRST, &context)) {
		TRACE0("Error: couldn't create preview view for frame.\n");
		pParent->OnSetPreviewMode (FALSE, pState);   // restore Frame Window
		pView->SetPreviewState (NULL); // do not delete state structure
		delete pView;
		return FALSE;
	}

	// Preview window shown now
	pState->pViewActiveOld = (pParent->GetActiveFrame()->GetActiveView());
	CSekaijuView* pActiveView = (CSekaijuView*)(pParent->GetActiveFrame()->GetActiveView());
	if (pActiveView != NULL) {
		pActiveView->ActivateView(FALSE, pActiveView, pActiveView);
	}

	if (!pView->SetPrintView (pPrintView)) {
		pView->PreviewClose ();
		return TRUE; // signal that OnEndPrintPreview was called
	}

	// TODO:次の行はCMDIFrameWnd::OnUpdateFrameTitle内のGetActiveDocumentでバグを起こす。
	// CMainFrameがアクティブなビューを持つのは印刷プレビュー中のみである。
	// 印刷プレビューが終わる際にはCMainFrameのアクティブなビューをNULLに戻すこと。
	pParent->SetActiveView (pView); // set active view - even for MDI

	// update toolbar and redraw everything
	pView->GetToolBar()->SendMessage (WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
	pParent->RecalcLayout(); // position and size everything
	pParent->UpdateWindow();

	return TRUE;
}

// 印刷プレビュー時にメインフレームの閉じるボタンが押されたときに誘導する関数。
// 印刷プレビューはMFC4.0風(SDI方式)とする。
BOOL CALLBACK _AfxSekaijuPreviewCloseProc (CFrameWnd* pFrameWnd) {
	ASSERT_VALID(pFrameWnd);
	CSekaijuPreviewView* pSekaijuPreviewView = 
		(CSekaijuPreviewView*) pFrameWnd->GetDlgItem (AFX_IDW_PANE_FIRST);
	pSekaijuPreviewView->PreviewClose();
	return FALSE;
}


// CView::EndPrintPreviewのオーバーライド
// 印刷プレビューはMFC4.0風(SDI方式)とする。
void CSekaijuView::OnEndPrintPreview (CDC* pDC, CPrintInfo* pInfo, POINT, CPreviewView* pView) {
	ASSERT_VALID(pDC);
	ASSERT_VALID(pView);
	CSekaijuPreviewView* pSekaijuPreviewView = (CSekaijuPreviewView*)pView;

	if (pSekaijuPreviewView->GetPrintView () != NULL) {
		((CSekaijuView*)(pSekaijuPreviewView->GetPrintView ()))->EndPrinting(pDC, pInfo);
	}
	CFrameWnd* pParent = (CFrameWnd*)AfxGetThread()->m_pMainWnd;
	ASSERT_VALID(pParent);
	ASSERT_KINDOF(CFrameWnd, pParent);

	// restore the old main window
	pParent->OnSetPreviewMode(FALSE, pSekaijuPreviewView->GetPreviewState());

	// Force active view back to old one
	// TODO:次の行はCMDIFrameWnd::OnUpdateFrameTitle内のGetActiveDocumentでバグを起こす。
	// CMainFrameがアクティブなビューを持つのは印刷プレビュー中のみである。
	// 印刷プレビューが終わる際にはCMainFrameのアクティブなビューをNULLに戻すこと。
	//pParent->SetActiveView (pSekaijuPreviewView->GetPreviewState()->pViewActiveOld);
	pParent->SetActiveView (NULL);
	if (pParent != GetParentFrame()) {
		OnActivateView (TRUE, this, this); // re-activate view in real frame
	}
	pSekaijuPreviewView->DestroyWindow();  // destroy preview view
			// C++ object will be deleted in PostNcDestroy

	// restore main frame layout and idle message
	pParent->RecalcLayout();
	pParent->SendMessage (WM_SETMESSAGESTRING, (WPARAM)AFX_IDS_IDLEMESSAGE, 0L);
	pParent->UpdateWindow();
}





