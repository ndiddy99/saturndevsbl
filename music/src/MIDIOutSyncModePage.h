//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI出力同期モードページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _MIDIOUTSYNCMODEPAGE_H_
#define _MIDIOUTSYNCMODEPAGE_H_


class CMIDIOutSyncModePage : public CPropertyPage {
	DECLARE_DYNCREATE (CMIDIOutSyncModePage)

public:
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
	CMIDIOutSyncModePage ();            // コンストラクタ
	virtual ~CMIDIOutSyncModePage ();   // デストラクタ

	//--------------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------------
public:
	int m_nMIDIOutSyncMode[MAXMIDIOUTDEVICENUM];

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
