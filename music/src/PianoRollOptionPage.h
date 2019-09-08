//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールオプションページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _PIANOROLLOPTIONPAGE_H_
#define _PIANOROLLOPTIONPAGE_H_

class CPianoRollOptionPage : public CPropertyPage {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lDefKeyZoom;                 // デフォルトのキー方向拡大倍率[倍]
	long m_lDefVelZoom;                 // デフォルトのベロシティ方向拡大倍率[倍]
	long m_lDefTimeZoom;                // デフォルトのタイム方向拡大倍率[倍]
	BOOL m_bEnableKeyZoomKey;           // 鍵盤方向ズームのショートカットキーCtrl+'+''-'を有効にする
	BOOL m_bEnableVelZoomKey;           // ベロシティ方向ズームのショートカットキーCtrl+'+''-'を有効にする
	BOOL m_bEnableTimeZoomKey;          // 時間方向ズームのショートカットキーCtrl+'+''-'を有効にする
	BOOL m_bSpeakerModeVisibleTrack;    // 可視状態のトラックを試聴する
	long m_lGraphLineWidth;             // グラフの幅[pixel]

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPianoRollOptionPage ();            // コンストラクタ
	enum {IDD = IDD_PIANOROLLOPTION};

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
