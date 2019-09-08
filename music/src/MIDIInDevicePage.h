//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI入力デバイスページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _MIDIINDEVICEPAGE_H_
#define _MIDIINDEVICEPAGE_H_

class CMIDIInDevicePage : public CPropertyPage {
	DECLARE_DYNCREATE (CMIDIInDevicePage)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
	CString m_strMIDIInName[MAXMIDIINDEVICENUM];    // MIDIInデバイス名[0～15]

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMIDIInDevicePage ();               // コンストラクタ
	virtual ~CMIDIInDevicePage ();      // デストラクタ

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
