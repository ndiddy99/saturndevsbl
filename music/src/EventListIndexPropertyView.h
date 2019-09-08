//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリストインデックスプロパティビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _EVENTLISTINDEXPROPERTYVIEW_H_
#define _EVENTLISTINDEXPROPERTYVIEW_H_

#include "InplaceEdit.h"
#include "InplaceListBox.h"

class CEventListIndexPropertyView : public CSekaijuView {

	DECLARE_DYNCREATE (CEventListIndexPropertyView)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEventListIndexPropertyView();          // コンストラクタ
	virtual ~CEventListIndexPropertyView(); // デストラクタ


	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lCurRow;                     // 現在の行番号(0～)
	long m_lCurColumn;                  // 現在の列番号(0～)
	long m_lCurButtonState;             // 現在のボタン状態
	long m_lCurButtonInterval;          // 現在のボタンの増減間隔
	CInplaceEdit m_theTextBox;          // インプレーステキストボックス
	CInplaceListBox m_theListBox;       // インプレースリストボックス

protected:
	CPoint m_ptMouseDown;               // マウスが押されたときの座標
	CPoint m_ptMouseMove;               // マウスが動かされたときの前回の座標
	MIDITrack* m_pCloneTrack;           // 一時的な複製されたトラックへのポインタ
	MIDIEvent* m_pCloneEvent;           // 一時的な複製されたイベントへのポインタ
	MIDIEvent* m_pLastEvent;            // 一時的な最後のイベントへのポインタ
	BOOL   m_bSettingCellString;        // セルの文字列を設定中か

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
protected:
	virtual BOOL GetCellFromPoint (CPoint pt, long* pRow, long* pColumn);
	virtual CRect GetRectFromCell (long lRow, long lColumn);

public:
	virtual BOOL AutoScrolltoShowCell (long lRow, long lColumn);
	virtual BOOL IsTextEditing ();
	virtual BOOL IsListSelecting ();
	virtual BOOL BeginTextEditing ();
	virtual BOOL BeginListSelecting ();
	virtual BOOL EndTextEditingOK ();
	virtual BOOL EndListSelectingOK ();
	virtual BOOL EndTextEditingCancel ();
	virtual BOOL EndListSelectingCancel ();
	virtual BOOL BeginValueUpDown ();
	virtual BOOL EndValueUpDown ();
	virtual BOOL AddValueOfCurCell (long lDeltaValue);
	virtual BOOL MoveTextBox (long lRow, long lColumn);
	virtual BOOL MoveListBox (long lRow, long lColumn);
	virtual BOOL UpdateParentToolbarControl ();
	virtual CString GetCellString (long lRow, long lColumn);
	virtual BOOL SetCellString (long lRow, long lColumn, CString strText);
	virtual BOOL ShowPopupMenu (CPoint point);
	virtual int DrawCurFrame (CDC* pDC);

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
public:
	virtual void OnPrepareDC (CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnDraw (CDC* pDC);
	virtual void OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);


	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg BOOL OnCreate (LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy ();
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

