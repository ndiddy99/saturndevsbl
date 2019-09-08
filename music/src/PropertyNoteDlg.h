//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// プロパティノートダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _PROPERTYNOTEDLG_H_
#define _PROPERTYNOTEDLG_H_

class CPropertyNoteDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	BOOL m_bTrackZeroOrigin;                 // トラック番号を0から数えるか
	BOOL m_bNoteOnNoteOn0;                   // ノートオン+ノートオン(ベロシティ0)か
	CStringArray m_theTrackNameArray;        // トラック名配列
	CDWordArray m_theTrackOutputChannelArray;// 出力チャンネル配列
	CStringArray m_theKeyNameArray;          // キー名配列
	int m_nTrackIndex;                       // トラック
	CString m_strTime;                       // タイム
	int m_nChannel;                          // チャンネル
	int m_nKey;                              // キー
	int m_nOnVelocity;                       // 打鍵ベロシティ
	int m_nOffVelocity;                      // 離鍵ベロシティ
	int m_nDuration;                         // 音長さ

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPropertyNoteDlg();                      // コンストラクタ
	enum {IDD = IDD_PROPERTYNOTE};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL SetChannelRange ();                 // チャンネルの可変範囲設定
	BOOL SetOnVelocityRange ();              // 打鍵ベロシティの可変範囲設定
	BOOL SetOffVelocityRange ();             // 離鍵ベロシティの可変範囲設定
	BOOL SetDurationRange ();                // 音長さの可変範囲設定
	BOOL FillTrackIndexCombo ();             // トラックインデックスコンボボックス充満
	BOOL FillKeyCombo ();                    // キーインデックスコンボボックス充満

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
	afx_msg void CPropertyNoteDlg::OnTrackIndexSelEndOK ();
	DECLARE_MESSAGE_MAP ()
};

#endif