//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// オプションプロパティシートクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _OPTIONSHEET_H_
#define _OPTIONSHEET_H_

#include "GeneralOptionPage.h"
#include "ColorOptionPage.h"
#include "FontOptionPage.h"
#include "TrackListOption1Page.h"
#include "TrackListOption2Page.h"
#include "PianoRollOptionPage.h"
#include "EventListOptionPage.h"
#include "MusicalScoreOptionPage.h"

class COptionSheet : public CPropertySheet {
	//--------------------------------------------------------------------------
	// 各ページ
	//--------------------------------------------------------------------------
public:
	CGeneralOptionPage m_theGeneralOptionPage;
	CColorOptionPage m_theColorOptionPage;
	CFontOptionPage m_theFontOptionPage;
	CTrackListOption1Page m_theTrackListOption1Page;
	CTrackListOption2Page m_theTrackListOption2Page;
	CPianoRollOptionPage m_thePianoRollOptionPage;
	CEventListOptionPage m_theEventListOptionPage;
	CMusicalScoreOptionPage m_theMusicalScoreOptionPage;

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	COptionSheet (CWnd* pParentWnd);    // コンストラクタ
	virtual ~COptionSheet ();           // デストラクタ

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
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
