//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストタイムスケールビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _TRACKLISTTIMESCALEVIEW_H_
#define _TRACKLISTTIMESCALEVIEW_H_

class CTrackListTimeScaleView : public CSekaijuView {

	DECLARE_DYNCREATE (CTrackListTimeScaleView)
	
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CTrackListTimeScaleView ();             // コンストラクタ
	virtual ~CTrackListTimeScaleView ();    // デストラクタ

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
protected:
	CPoint m_ptMouseDown;                // マウスが押されたときの座標
	CPoint m_ptMouseMove;                // マウスが動かされたときの前回の座標
	UINT   m_nMouseDownFlags;            // マウスが押されたときのフラグ
	UINT   m_nMouseMoveFlags;            // マウスが動かされたときの前回のフラグ
	long   m_lDownMeasure;               // マウスが押された位置の小節(0～)
	long   m_lDownBeat;                  // マウスが押された位置の拍(0～)
	long   m_lDownTick;                  // マウスが押された位置のティック(0～)
	long   m_lDownTime;                  // マウスが押された位置のタイム(0～)[tick]
	long   m_lOldMeasure;                // マウスが動かされた前回の位置の小節(0～)
	long   m_lOldBeat;                   // マウスが動かされた前回の位置の拍(0～)
	long   m_lOldTick;                   // マウスが動かされた前回の位置のティック(0～)
	long   m_lOldTime;                   // マウスが動かされた前回の位置のタイム(0～)[tick]
	long   m_lCurMeasure;                // マウスが動かされた現在の位置の小節(0～)
	long   m_lCurBeat;                   // マウスが動かされた現在の位置の拍(0～)
	long   m_lCurTick;                   // マウスが動かされた現在の位置のティック(0～)
	long   m_lCurTime;                   // マウスが動かされた現在の位置のタイム(0～)[tick]

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	virtual void DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor);

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw (CDC* pDC);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg BOOL OnCreate (LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy ();
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
