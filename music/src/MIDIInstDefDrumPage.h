//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDIインストゥルメント定義(ドラム)ページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */


#ifndef _MIDIINSTDEFDRUMPAGE_H_
#define _MIDIINSTDEFDRUMPAGE_H_

class CMIDIInstDefDrumPage : public CPropertyPage {
	DECLARE_DYNCREATE (CMIDIInstDefDrumPage)

	//--------------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------------
public:
	CString m_strMIDIInstDefDrumName[MAXMIDIOUTDEVICENUM];  // MIDIインストゥルメント(ドラム)名

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CMIDIInstDefDrumPage ();            // コンストラクタ
	virtual ~CMIDIInstDefDrumPage ();   // デストラクタ

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
