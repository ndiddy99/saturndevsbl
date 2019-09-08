//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 細分化とトリルダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _EDITBREAKUPANDTRILLDLG_H_
#define _EDITBREAKUPANDTRILLDLG_H_

class CEditBreakupAndTrillDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lTimeMode;                   // タイムモード
	long m_lTimeResolution;             // 分解能[tick/4分音符]又は[サブフレーム/1フレーム]
	int m_nDurationIndex;               // 細分化後の1音の長さインデックス(0=4分音符～)
	int m_nEnableTrill;                 // トリルを有効にするか？
	int m_nKeyShift;                    // トリル音のキーシフト

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEditBreakupAndTrillDlg();          // コンストラクタ
	enum {IDD = IDD_EDITBREAKUPANDTRILL};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL FillDurationCombo ();          // 音長さコンボボックスの充満
	BOOL SetKeyShiftRange ();           // トリル音のキーシフトの有効範囲設定
	BOOL UpdateKeyShift ();             // トリル音のキーシフトエディットボックス更新

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);
	virtual BOOL OnInitDialog ();       // ダイアログの初期化

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
	afx_msg void OnChangeEnableTrill ();
};

#endif
