//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストトラックモードビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _TRACKLISTTRACKMODEVIEW_H_
#define _TRACKLISTTRACKMODEVIEW_H_

#include "InplaceEdit.h"

class CTrackListTrackModeView : public CSekaijuView {

	DECLARE_DYNCREATE (CTrackListTrackModeView)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CTrackListTrackModeView();          // コンストラクタ
	virtual ~CTrackListTrackModeView(); // デストラクタ


	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
	long m_lCurRow;                     // 現在の行番号(0～)
	long m_lCurColumn;                  // 現在の列番号(0～)
	long m_lCurButtonState;             // 現在の行・列のボタン状態
	long m_lCurButtonInterval;          // ボタンの増減間隔

protected:
	// テキストボックス
	CInplaceEdit m_theTextBox;

	// マウスキャプターさくら
	CPoint m_ptMouseDown;               // マウスが押された座標
	CPoint m_ptMouseMove;               // マウスが動かされた前回の座標


	BOOL   m_bSettingCellString;        // セルの文字列を編集中であるか

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	virtual BOOL GetCellFromPoint (CPoint pt, long* pRow, long* pColumn);
	virtual void* GetCellEvent (long lRow, long lColumn);
	virtual BOOL AutoScrolltoShowCell (long lRow, long lColumn);
	virtual BOOL IsTextEditing ();
	virtual BOOL BeginTextEditing ();
	virtual BOOL EndTextEditingOK ();
	virtual BOOL EndTextEditingCancel ();
	virtual BOOL BeginValueUpDown ();
	virtual BOOL EndValueUpDown ();
	virtual BOOL AddValueOfCurCell (long lDeltaValue);
	virtual BOOL ToggleValueOfCurCell ();
	virtual BOOL DoColorEditing ();
	virtual CRect GetRectFromCell (long lRow, long lColumn);
	virtual BOOL MoveTextBox (long lRow, long lColumn);
	virtual CString GetCellString (long lRow, long lColumn);
	virtual BOOL SetCellString (long nRow, long nColumn, CString strText);
	virtual BOOL DrawCurFrame (CDC* pDC);

	// オーバーライド
protected:
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw (CDC* pDC);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// メッセージマップ
protected:
	afx_msg BOOL OnCreate (LPCREATESTRUCT lpcs);
	afx_msg void OnActivate (UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnKillFocus (CWnd* pNewWnd);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
  	afx_msg void OnKeyUp (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk (UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk (UINT nFlags, CPoint point);
	afx_msg void OnTimer (UINT nIDEvent);
	afx_msg void OnMouseWheel40 (UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP ();
};

#endif

