//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI入力同期モードページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _MIDIINSYNCMODEPAGE_H_
#define _MIDIINSYNCMODEPAGE_H_


class CMIDIInSyncModePage : public CPropertyPage {
	DECLARE_DYNCREATE (CMIDIInSyncModePage)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	int m_nMIDIInSyncMode[MAXMIDIINDEVICENUM]; // MIDI入力同期モード[0～15]

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMIDIInSyncModePage ();             // コンストラクタ
	virtual ~CMIDIInSyncModePage ();    // デストラクタ

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
