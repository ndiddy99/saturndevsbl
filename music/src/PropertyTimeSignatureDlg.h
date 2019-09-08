//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// プロパティ拍子記号ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _PROPERTYTIMESIGNATUREDLG_H_
#define _PROPERTYTIMESIGNATUREDLG_H_

class CPropertyTimeSignatureDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strTime;                       // タイム
	long m_lNN;                              // 分子(1～255)
	int  m_nDDIndex;                         // 分母(0=1,2=2,3=4,4=8,5=16,6=32)
	long m_lCC;                              // 4分音符当たりのクロック数(常に24,1～255)
	long m_lBB;                              // 4分音符当たりの32分音符の数(常に8,1～255)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPropertyTimeSignatureDlg();                      // コンストラクタ
	enum {IDD = IDD_PROPERTYTIMESIGNATURE};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL CPropertyTimeSignatureDlg::SetNNRange ();
	BOOL CPropertyTimeSignatureDlg::FillDDIndexCombo ();
	BOOL CPropertyTimeSignatureDlg::SetCCRange ();
	BOOL CPropertyTimeSignatureDlg::SetBBRange ();

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