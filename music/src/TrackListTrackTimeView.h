//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストトラックタイムビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _TRACKLISTTRACKTIMEVIEW_H_
#define _TRACKLISTTRACKTIMEVIEW_H_

class CTrackListTrackTimeView : public CSekaijuView {

	DECLARE_DYNCREATE (CTrackListTrackTimeView)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CTrackListTrackTimeView ();             // コンストラクタ
	virtual ~CTrackListTrackTimeView ();    // デストラクタ

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
protected:
	long m_lCurTime;                    // 現在の描画タイム[tick]
	long m_lOldTime;                    // 前回の描画タイム[tick]
	long m_lOldX;                       // 前回の縦線x座標[pixel]
	long m_lOldY1;                      // 前回の縦線y上座標[pixel]
	long m_lOldY2;                      // 前回の縦線y下座標[pixel]
	BOOL m_bOldDraw;                    // 前回縦線を描画したか？
	CPoint m_ptMouseDown;               // マウスが押されたときの座標
	CPoint m_ptMouseMove;               // マウスが動かされたときの前回の座標
	int    m_nMouseDownFlags;           // マウスが押されたときのフラグ
	int    m_nMouseMoveFlags;           // マウスが動かされたときの前回のフラグ
	long m_lTempMode;                   // 一時的なモード
	long m_lTempTool;                   // 一時的なツール(0～)
	long m_lTempTime;                   // 一時的なタイム(0～)[tick]
	long m_lTempTrackIndex;             // 一時的なトラック番号(0～65535)
	long m_lMinX;
	long m_lMaxX;
	long m_lMinY;
	long m_lMaxY;
	CPtrArray m_theTempSelectedEventArray;
	CPtrArray m_theTempEndofTrackEventArray;

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
protected:
	virtual void EraseOldLine (CDC* pDC);
	virtual void DrawCurLine (CDC* pDC);

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
public:
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw (CDC* pDC);
	virtual void OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy ();
	afx_msg void OnTimer (UINT nIDEvent);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP ()
};



#endif
