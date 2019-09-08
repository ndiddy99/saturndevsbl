//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストオプション1ページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */


#ifndef _TRACKLISTOPTION1PAGE_H_
#define _TRACKLISTOPTION1PAGE_H_

class CTrackListOption1Page : public CPropertyPage {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lDefRowZoom;                 // デフォルトの列方向拡大倍率
	long m_lDefColumnZoom;              // デフォルトの行方向拡大倍率
	long m_lDefTimeZoom;                // デフォルトのタイム方向拡大倍率
	long m_lDefNameWidth;               // デフォルトの名前の幅[pixel]
	long m_lDefColorWidth;              // デフォルトの色の幅[pixel]
	long m_lDefInputOnWidth;            // デフォルトの入力ONの幅[pixel]
	long m_lDefInputPortWidth;          // デフォルトの入力ポートの幅[pixel]
	long m_lDefInputChWidth;            // デフォルトの入力CHの幅[pixel]
	long m_lDefOutputOnWidth;           // デフォルトの出力ONの幅[pixel]
	long m_lDefOutputPortWidth;         // デフォルトの出力ポートの幅[pixel]
	long m_lDefOutputChWidth;           // デフォルトの出力CHの幅[pixel]
	long m_lDefViewModeWidth;           // デフォルトのビューモードの幅[pixel]
	// ※実際の列の幅[pixel]は、各列の幅に列方向拡大倍率をかけた値となる。
	
public:
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
	CTrackListOption1Page ();           // コンストラクタ
	enum {IDD = IDD_TRACKLISTOPTION1};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------

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
	//afx_msg void OnChangeEnableAutoSave ();
	DECLARE_MESSAGE_MAP ()
};

#endif
