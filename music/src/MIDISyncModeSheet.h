//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI同期モードプロパティシートクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _MIDISYNCMODESHEET_H_
#define _MIDISYNCMODESHEET_H_

#include "MIDIInSyncModePage.h"
#include "MIDIOutSyncModePage.h"

class CMIDISyncModeSheet : public CPropertySheet {

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMIDISyncModeSheet (CWnd* pParentWnd);  // コンストラクタ
	virtual ~CMIDISyncModeSheet ();         // デストラクタ
	
	//--------------------------------------------------------------------------
	// 各ページ
	//--------------------------------------------------------------------------
public:
	CMIDIInSyncModePage m_theMIDIInSyncModePage;    // MIDI入力同期モードページ
	CMIDIOutSyncModePage m_theMIDIOutSyncModePage;  // MIDI出力同期モードページ

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog ();
	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam);
	virtual int DoModal ();
protected:
	virtual void BuildPropPageArray ();
	static int CALLBACK PropSheetProc (HWND hwndDlg, UINT uMsg, LPARAM lParam);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnApplyNow ();
	DECLARE_MESSAGE_MAP ()
};

#endif
