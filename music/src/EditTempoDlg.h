//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// テンポの変更ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _EDITTEMPODLG_H_
#define _EDITTEMPODLG_H_

class CEditTempoDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strAmount;                   // 値の変更量
	int m_nUnit;                        // 単位(0=絶対指定,1=増減,2=パーセント,3=ランダム増減)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEditTempoDlg ();                   // コンストラクタ
	enum {IDD = IDD_EDITTEMPO};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL SetAmountRange ();             // 値の変更量の有効範囲を設定する
	void CEditTempoDlg::DDV_MinMaxString (CDataExchange* pDX, 
		LPCTSTR lpszValue, double dMin, double dMax);

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV のサポート
	virtual BOOL OnInitDialog ();       // ダイアログの初期化

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnChangeUnit (UINT nID);
	afx_msg void OnChangeTarget (UINT nID);
	DECLARE_MESSAGE_MAP ()
};

#endif
