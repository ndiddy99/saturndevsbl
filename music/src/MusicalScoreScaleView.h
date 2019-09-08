//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 譜面スケールウィンドウクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _MUSICALSCORESCALEVIEW_H_
#define _MUSICALSCORESCALEVIEW_H_

class CMusicalScoreScaleView : public CSekaijuView {
	DECLARE_DYNCREATE (CMusicalScoreScaleView)

	//-------------------------------------------------------------------------
	// 構築と破壊
	//-------------------------------------------------------------------------	
public:
	CMusicalScoreScaleView ();          // コンストラクタ
	virtual ~CMusicalScoreScaleView (); // デストラクタ
	
	//-------------------------------------------------------------------------	
	// オーバーライド
	//-------------------------------------------------------------------------	
public:
	virtual void OnDraw (CDC* pDC);
	virtual void OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint);

	//-------------------------------------------------------------------------	
	// メッセージマップ
	//-------------------------------------------------------------------------	
protected:
 	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP ()
};


#endif
