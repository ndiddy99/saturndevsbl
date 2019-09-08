//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹印刷プレビュービュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _SEKAIJUPREVIEWVIEW_H_
#define _SEKAIJUPREVIEWVIEW_H_

class CSekaijuPreviewView : public CPreviewView {
	DECLARE_DYNCREATE (CSekaijuPreviewView)
	
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CSekaijuPreviewView ();                    // コンストラクタ
	virtual ~CSekaijuPreviewView ();           // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	void SetPreviewState (CPrintPreviewState* pPreviewState);
	CPrintPreviewState* GetPreviewState () {return m_pPreviewState;}
	void SetToolBar (CDialogBar* pToolBar);
	CDialogBar* GetToolBar () {return m_pToolBar;}
	CView* GetPrintView () {return m_pPrintView;}
	void ActivateView (BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView);
	void PreviewClose ();

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnPreviewClose();
//	afx_msg void OnNumPageChange();
//	afx_msg void OnNextPage();
//	afx_msg void OnPrevPage();
	afx_msg void OnPreviewPrint();
//	afx_msg void OnZoomIn();
//	afx_msg void OnZoomOut();
//	afx_msg void OnUpdateNumPageChange(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateNextPage(CCmdUI* pCmdUI);
//	afx_msg void OnUpdatePrevPage(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

#endif
