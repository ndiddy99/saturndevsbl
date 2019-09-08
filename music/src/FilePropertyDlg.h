//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ファイルプロパティダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _FILEPROPERTYDLG_H_
#define _FILEPROPERTYDLG_H_

class CFilePropertyDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strTitle;                 // タイトル(=最初のトラックの最初のトラック名)
	CString m_strSubTitle;              // サブタイトル(=最初のトラックの2番目のトラック名)
	CString m_strCopyright;             // 著作権(=最初のトラックの最初の著作権)
	CString m_strComment1;              // コメント1(=最初のトラックの最初のテキスト)
	CString m_strComment2;              // コメント2(=最初のトラックの2番目のテキスト)
	CString m_strComment3;              // コメント3(=最初のトラックの3番目のテキスト)
	CString m_strNumTrack;              // トラック数
	CString m_strNumEvent;              // イベント数
	CString m_strEndMillisec;           // 終了ミリ秒
	CString m_strEndTime;               // 終了タイム
	int m_nSMFFormat;                   // SMFフォーマット0/1/2
	int m_nTimeMode;                    // タイムモード(0=TPQNベース,...)
	int m_nResolution;                  // 分解能[tick/4分音符]又は[サブフレーム/1フレーム]

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CFilePropertyDlg ();                // コンストラクタ
	enum {IDD = IDD_FILEPROPERTY};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
	void FillResolutionComboAndStatic ();

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV のサポート
	virtual BOOL OnInitDialog ();

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnChangeTimeMode (UINT nID);
	DECLARE_MESSAGE_MAP ()
};

#endif
