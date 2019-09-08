//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストオプション2ページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _TRACKLISTOPTION2PAGE_H_
#define _TRACKLISTOPTION2PAGE_H_

class CTrackListOption2Page : public CPropertyPage {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lDefCC000Width;              // デフォルトのCC#0の幅[pixel]
	long m_lDefCC032Width;              // デフォルトのCC#32の幅[pixel]
	long m_lDefPCWidth;                 // デフォルトのプログラムナンバーの幅[pixel]
	long m_lDefCC007Width;              // デフォルトのボリュームの幅[pixel]
	long m_lDefCC010Width;              // デフォルトのパンの幅[pixel]
	long m_lDefCC091Width;              // デフォルトのリバーブの幅[pixel]
	long m_lDefCC093Width;              // デフォルトのコーラスの幅[pixel]
	long m_lDefCC094Width;              // デフォルトのディレイの幅[pixel]
	long m_lDefKeyShiftWidth;           // デフォルトのキーシフトの幅[pixel]
	long m_lDefVelShiftWidth;           // デフォルトのベロシティシフトの幅[pixel]
	long m_lDefTimeShiftWidth;          // デフォルトのタイムシフトの幅[pixel]
	long m_lDefNumEventWidth;           // デフォルトのイベント数の幅[pixel]
	// ※実際の列の幅[pixel]は、各列の幅に列方向拡大倍率をかけた値となる。
	BOOL m_bEnableRowZoomKey;           // 行方向ズームのショートカットキーCtrl+'+''-'を有効にする
	BOOL m_bEnableColumnZoomKey;        // 列方向ズームのショートカットキーCtrl+'+''-'を有効にする
	BOOL m_bEnableTimeZoomKey;          // 時間方向ズームのショートカットキーCtrl+'+''-'を有効にする
	
public:
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
	CTrackListOption2Page ();           // コンストラクタ
	enum {IDD = IDD_TRACKLISTOPTION2};

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
