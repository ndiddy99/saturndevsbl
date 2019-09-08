//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリストオプションページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _EVENTLISTOPTION_H_
#define _EVENTLISTOPTION_H_

class CEventListOptionPage : public CPropertyPage {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lDefRowZoom;                 // デフォルトの行方向拡大率[倍]
	long m_lDefColumnZoom;              // デフォルトの列方向拡大率[倍]
	long m_lDefTrackWidth;              // デフォルトのトラック幅[pixel]
	long m_lDefMillisecWidth;           // デフォルトの時:分:秒:ミリ秒幅[pixel]
	long m_lDefTimeWidth;               // デフォルトのタイム幅[pixel]
	long m_lDefKindWidth;               // デフォルトのイベントの種類幅[pixel]
	long m_lDefChWidth;                 // デフォルトのチャンネル幅[pixel]
	long m_lDefVal1Width;               // デフォルトの値1幅[pixel]
	long m_lDefVal2Width;               // デフォルトの値2幅[pixel]
	long m_lDefVal3Width;               // デフォルトの値3幅[pixel]
	BOOL m_bInsertEventAfter;           // 同時刻のイベントの直後に挿入する
	BOOL m_bDuplicateEventAfter;        // 現在のイベントの直後に挿入する
	BOOL m_bDeleteEventAfter;           // 削除したイベントの次のイベントにフォーカスを合わせる
	BOOL m_bEnableRowZoomKey;           // 行方向ズームのショートカットキーCtrl+'+''-'を有効にする
	BOOL m_bEnableColumnZoomKey;        // 列方向ズームのショートカットキーCtrl+'+''-'を有効にする

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	// コンストラクタ
	CEventListOptionPage ();
	enum {IDD = IDD_EVENTLISTOPTION};

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
