//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹ドキュメントテンプレートクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _SEKAIJUDOCTEMPLETE_H_
#define _SEKAIJUDOCTEMPLETE_H_

class CSekaijuDocTemplate : public CMultiDocTemplate {
	DECLARE_DYNAMIC (CSekaijuDocTemplate)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
	// コンストラクタ
public:
	CSekaijuDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
	CFrameWnd* CreateTrackListFrame (CDocument* pDoc);
	CFrameWnd* CreatePianoRollFrame (CDocument* pDoc);
	CFrameWnd* CreateEventListFrame (CDocument* pDoc);
	CFrameWnd* CreateMusicalScoreFrame (CDocument* pDoc);	
	
	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
	virtual void UpdateAllViews (CView* pSender,  LPARAM lHint = 0L, CObject* pHint = NULL);
	virtual CDocument* OpenDocumentFile (LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);

};

#endif
