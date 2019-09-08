//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDIThruページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _MIDITHRUPAGE_H_
#define _MIDITHRUPAGE_H_

class CMIDIThruPage : public CPropertyPage {
	DECLARE_DYNCREATE (CMIDIThruPage)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
	int m_nMIDIThru[MAXMIDIINDEVICENUM];    // MIDIThru[0～15]

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMIDIThruPage ();               // コンストラクタ
	virtual ~CMIDIThruPage ();      // デストラクタ

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
	DECLARE_MESSAGE_MAP ()

};

#endif
