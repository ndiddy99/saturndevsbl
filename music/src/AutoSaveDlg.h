//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 自動保存ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _AUTOSAVEDLG_H_
#define _AUTOSAVEDLG_H_

class CAutoSaveDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strExeFilePath;           // 実行ファイルのパス名
	int m_nOn;                          // 自動保存オン
	int m_lInterval;                    // 自動保存間隔(1-120)[分]
	int m_nDisableWhilePlaying;         // 演奏中は自動保存しない
	int m_nDisableWhileRecording;       // リアルタイム入力中は自動保存しない

public:
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
	// コンストラクタ
	CAutoSaveDlg ();
	enum {IDD = IDD_AUTOSAVE};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
	BOOL SetIntervalRange ();
	BOOL UpdateInterval ();
	BOOL UpdateDisableWhilePlaying ();
	BOOL UpdateDisableWhileRecording ();

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
	afx_msg void OnChangeOn ();
	afx_msg void OnDeleteAllFileNow ();
};

#endif
