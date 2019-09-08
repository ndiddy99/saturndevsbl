//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// プロパティ調性記号ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _PROPERTYKEYSIGNATUREDLG_H_
#define _PROPERTYKEYSIGNATUREDLG_H_

class CPropertyKeySignatureDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strTime;                       // タイム
	int  m_nSFIndex;                         // #又はbの数(0=7b...7=0#...14=7#)
	int  m_nMiIndex;                         // 長調or短調(0=major,1=minor)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPropertyKeySignatureDlg();                      // コンストラクタ
	enum {IDD = IDD_PROPERTYKEYSIGNATURE};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL CPropertyKeySignatureDlg::FillSFIndexCombo ();
	BOOL CPropertyKeySignatureDlg::FillMiIndexCombo ();


	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV のサポート
	virtual BOOL OnInitDialog ();            // ダイアログの初期化

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ();
};

#endif