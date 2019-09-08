//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ビート検出とテンポ自動挿入ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _EDITBEATSCANDLG_H_
#define _EDITBEATSCANDLG_H_

class CEditBeatScanDlg : public CDialog {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lTimeMode;                   // タイムモード
	long m_lTimeResolution;             // 分解能[tick/4分音符]又は[サブフレーム/1フレーム]
	CStringArray m_theTrackNameArray;   // トラック名の配列
	int m_nBeatTrackIndex;              // ビートの記録されたトラック番号(0～65535)
	int m_nBeatIntervalIndex;           // ビートの間隔インデックス(0=4分音符～)
	int m_nInsertTempo;                 // テンポを自動挿入するか？

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEditBeatScanDlg();                 // コンストラクタ
	enum {IDD = IDD_EDITBEATSCAN};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL FillBeatTrackCombo ();
	BOOL FillBeatIntervalCombo ();

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

};

#endif
