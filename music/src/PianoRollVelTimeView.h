//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールベロシティタイムビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _PIANOROLLVELTIMEVIEW_H_
#define _PIANOROLLVELTIMEVIEW_H_

class CPianoRollVelTimeView : public CSekaijuView {
	DECLARE_DYNCREATE(CPianoRollVelTimeView)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPianoRollVelTimeView();            // コンストラクタ
	virtual ~CPianoRollVelTimeView();   // デストラクタ

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
	
	// カレントポジション縦線描画用
protected:
	long m_lCurTime;                    // 現在の描画タイム[tick]
	long m_lOldTime;                    // 前回の描画タイム[tick]
	long m_lOldX;                       // 前回の縦線x座標[pixel]
	long m_lOldY1;                      // 前回の縦線y上座標[pixel]
	long m_lOldY2;                      // 前回の縦線y下座標[pixel]
	BOOL m_bOldDraw;                    // 前回縦線を描画したか？
protected:
	CPoint m_ptMouseDown;               // マウスが押されたときの座標
	CPoint m_ptMouseMove;               // マウスが動かされたときの前回の座標
	UINT m_nMouseDownFlags;             // マウスが押されたときのフラグ
	UINT m_nMouseMoveFlags;             // マウスが動かされたときの前回のフラグ
	long m_lTempTool;                   // 一時的なツール
	long m_lTempTrackIndex;             // 一時的なトラック番号(0～65535)
	long m_lTempSnap;                   // 一時的なスナップ(1～)[tick]
	long m_lTempVelocity;               // 一時的なベロシティ(1～127)
	long m_lTempPortIndex;              // 一時的なポート番号(0～15)
	long m_lTempChannel;                // 一時的なチャンネル(0～15)
	long m_lTempGraphKind;              // 一時的なグラフの種類
	long m_lTempGraphSnap;              // 一時的なグラフのスナップ(1～)[tick]
	long m_lTempTime;                   // 一時的なタイム(0～)[tick]
	long m_lTempTimeNoteOn;             // 一時的なノートオンタイム(0～)[tick]
	MIDIEvent* m_pTempEvent;            // 一時的なイベントへのポインタ
	MIDIEvent* m_pLastEvent;            // 一時的な最後のイベントへのポインタ
	long m_lTempMode;                   // 一時的なモード(ペン、ライン、消しゴム、選択、スクラブ)
	long m_lTempLineMode;               // 一時的なラインモード(直線、始点が水平な2次曲線、終点が水平な2次曲線)
	long m_lLastInsertedValue;
	long m_lLastInsertedTempoBPM;
	long m_lLastInsertedPitchBend;

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
protected:
	virtual void EraseOldLine (CDC* pDC);
	virtual void DrawCurLine (CDC* pDC);

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
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
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#endif


