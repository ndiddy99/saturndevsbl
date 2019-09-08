//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリストインデックススケールビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _EVENTLISTINDEXSCALEVIEW_H_
#define _EVENTLISTINDEXSCALEVIEW_H_

class CEventListIndexScaleView : public CSekaijuView {

public:
	DECLARE_DYNCREATE (CEventListIndexScaleView)
	
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEventListIndexScaleView();             // コンストラクタ
	virtual ~CEventListIndexScaleView ();   // デストラクタ

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
protected:
	CPoint m_ptMouseDown;                   // マウスが押されたときの座標
	CPoint m_ptMouseMove;                   // マウスが動かされたときの前回の座標
	UINT m_nMouseDownFlags;                 // マウスが押されたときのフラグ
	UINT m_nMouseMoveFlags;                 // マウスが動かされたときの前回のフラグ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
protected:
	virtual CRect GetRectFromRow (long lRow);

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void OnDraw (CDC* pDC);
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg BOOL OnCreate (LPCREATESTRUCT lpcs);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk (UINT nFlags, CPoint point);
	afx_msg void OnTimer (UINT nIDEvent);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP ()
};

#endif
