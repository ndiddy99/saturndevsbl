//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDIデバイスプロパティシートクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _MIDIDEVICESHEET_H_
#define _MIDIDEVICESHEET_H_

#include "MIDIInDevicePage.h"
#include "MIDIOutDevicePage.h"
#include "MIDIInstDefNormPage.h"
#include "MIDIInstDefDrumPage.h"
#include "MIDIThruPage.h"

class CMIDIDeviceSheet : public CPropertySheet {
	//--------------------------------------------------------------------------
	// 各プロパティページ
	//--------------------------------------------------------------------------
public:
	CMIDIInDevicePage m_theMIDIInDevicePage;        // MIDI入力デバイスページ
	CMIDIOutDevicePage m_theMIDIOutDevicePage;      // MIDI出力デバイスページ
	CMIDIInstDefNormPage m_theMIDIInstDefNormPage;  // MIDIインストゥルメント(通常)ページ
	CMIDIInstDefDrumPage m_theMIDIInstDefDrumPage;  // MIDIインストゥルメント(ドラム)ページ
	CMIDIThruPage m_theMIDIThruPage;                // MIDIThruページ
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMIDIDeviceSheet (CWnd* pParentWnd);    // コンストラクタ
	virtual ~CMIDIDeviceSheet ();           // デストラクタ

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
