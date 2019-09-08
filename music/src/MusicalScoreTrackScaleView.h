//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 譜面トラックスケールビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _MUSICALSCORETRACKSCALEVIEW_H_
#define _MUSICALSCORETRACKSCALEVIEW_H_

#include "InplaceEdit.h"

class CMusicalScoreTrackScaleView : public CSekaijuView {
protected:
	DECLARE_DYNCREATE (CMusicalScoreTrackScaleView)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
protected:
	CPoint m_ptMouseDown;               // マウスが押されたときの座標
	CPoint m_ptMouseMove;               // マウスが動かされたとき前回の座標
	UINT   m_nMouseDownFlags;           // マウスが押されたときのフラグ
	UINT   m_nMouseMoveFlags;           // マウスが動かされたとき前回のフラグ
	long   m_lDownTrack;                // マウスが押されたときの指すトラック(0～MAXMIDITRACKNUM-1)
	long   m_lCurTrack;                 // マウスが動かされたときの現在の指すトラック(0～MAXMIDITRACKNUM-1)
	long   m_lOldTrack;                 // マウスが動かされたときの前回の指すトラック(0～MAXMIDITRACKNUM-1)
	long   m_lOldTime;                  // 前回のタイム[tick]
	BOOL   m_bSettingCellString;         // セルの文字列を編集中であるか

	// テキストボックス
	CInplaceEdit  m_theTextBox;

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMusicalScoreTrackScaleView ();           // コンストラクタ
	virtual ~CMusicalScoreTrackScaleView ();  // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
protected:
	void DrawGClef 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawFClef 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawFlat 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawSharp 
		(CDC* pDC, long x, long y, long rx, long ry);
	void DrawTimeAndKeySignature 
		(CDC* pDC, long lTrackIndex, long lTime);
	
	CString GetCellString (long lTrack);
	BOOL SetCellString (long lTrack, CString strText);
	BOOL IsTextEditing ();
	BOOL BeginTextEditing ();
	BOOL EndTextEditingOK ();
	BOOL EndTextEditingCancel ();
	BOOL MoveTextBox (long lTrackIndex);

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
public:
	virtual void OnDraw (CDC* pDC);
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo = NULL);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg BOOL OnCreate (LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy ();
	afx_msg void OnKillFocus (CWnd* pNewWnd);
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

