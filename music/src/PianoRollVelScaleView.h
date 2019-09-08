//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールベロシティスケールビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _PIANOROLLVELSCALEVIEW_H_
#define _PIANOROLLVELSCALEVIEW_H_

class CPianoRollVelScaleView : public CSekaijuView {

	DECLARE_DYNCREATE (CPianoRollVelScaleView)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
protected:
	CPoint m_ptMouseDown;               // マウスが押されたときの座標
	CPoint m_ptMouseMove;               // マウスが動かされたときの前回の座標
	UINT   m_nMouseDownFlags;           // マウスが押されたときのフラグ
	UINT   m_nMouseMoveFlags;           // マウスが動かされたときの前回のフラグ
	long   m_lDownTempoBPM;             // マウスが押された位置のテンポ[BPM]
	long   m_lCurTempoBPM;              // マウスが動かされた現在の位置のテンポ[BPM]
	long   m_lOldTempoBPM;              // マウスが動かされた前回の位置のテンポ[BPM]
	long   m_lDownVel;                  // マウスが押された位置のベロシティ又は値(0～127)
	long   m_lCurVel;                   // マウスが動かされた現在の位置のベロシティ又は値(0～127)
	long   m_lOldVel;                   // マウスが動かされた前回の位置のベロシティ又は値(0～127)
	long   m_lDownPitchBend;            // マウスが押された位置のピッチベンド
	long   m_lCurPitchBend;             // マウスが動かされた現在の位置のピッチベンド
	long   m_lOldPitchBend;             // マウスが動かされた前回の位置のピッチベンド

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPianoRollVelScaleView ();			// コンストラクタ
	virtual ~CPianoRollVelScaleView (); // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:

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
