//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI出力デバイスページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _MIDIOUTDEVICEPAGE_H_
#define _MIDIOUTDEVICEPAGE_H_

class CMIDIOutDevicePage : public CPropertyPage {
	DECLARE_DYNCREATE (CMIDIOutDevicePage)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strMIDIOutName[MAXMIDIOUTDEVICENUM];  // MIDI出力同期モード

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMIDIOutDevicePage ();              // コンストラクタ
	virtual ~CMIDIOutDevicePage ();     // デストラクタ

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);
	virtual BOOL OnInitDialog ();

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnSelChange (UINT nID);
	DECLARE_MESSAGE_MAP ()

};

#endif
