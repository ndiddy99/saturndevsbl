//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 値の変更ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _EDITVALUEDLG_H_
#define _EDITVALUEDLG_H_

class CEditValueDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	int m_nAmount;                      // 値の変更量
	int m_nUnit;                        // 単位(0=絶対指定,1=増減,2=パーセント,3=ランダム増減)
	int m_nTargetKeyAfter;              // キーアフタータッチを対象とする(0/1)
	int m_nTargetControlChange;         // コントロールチェンジを対象とする(0/1)
	int m_nTargetChannelAfter;          // チャンネルアフタータッチを対象とする(0/1)
	int m_nTargetPitchBend;             // ピッチベンドを対象とする(0/1)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEditValueDlg ();                   // コンストラクタ
	enum {IDD = IDD_EDITVALUE};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL SetAmountRange ();             // 値の変更量の有効範囲を設定する

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
