//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// プロパティテンポダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _PROPERTYTEMPODLG_H_
#define _PROPERTYTEMPODLG_H_

class CPropertyTempoDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strTime;                       // タイム
	CString m_strTempoBPM;                   // テンポ[BPM]

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPropertyTempoDlg();                      // コンストラクタ
	enum {IDD = IDD_PROPERTYTEMPO};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	void CPropertyTempoDlg::DDV_MinMaxString (CDataExchange* pDX, 
		LPCTSTR lpszValue, double dMin, double dMax);

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