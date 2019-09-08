//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 履歴記録クラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _HISTORYRECORD_H_
#define _HISTORYRECORD_H_

#define HISTORYRECORD_INSERTEVENT 0x0001        // MIDIイベント挿入
#define HISTORYRECORD_REMOVEEVENT 0x0002        // MIDIイベント削除
#define HISTORYRECORD_INSERTEVENTALL 0x0003     // MIDIイベント全挿入
#define HISTORYRECORD_REMOVEEVENTALL 0x0004     // MIDIイベント全削除
#define HISTORYRECORD_INSERTTRACK 0x0011        // MIDIトラック挿入
#define HISTORYRECORD_REMOVETRACK 0x0012        // MIDIトラック削除
#define HISTORYRECORD_INSERTDATA  0x0021        // MIDIデータ挿入(未使用)
#define HISTORYRECORD_REMOVEDATA  0x0022        // MIDIデータ削除(未使用)

class CHistoryRecord {

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	long m_lType;           // 記録タイプ
	void* m_pObject;        // オブジェクトへのポインタ
	void* m_pPrevObject;    // オブジェクトが双方向リンクリスト要素の場合の前のオブジェクトへのポインタ
	void* m_pNextObject;    // オブジェクトが双方向リンクリスト要素の場合の次のオブジェクトへのポインタ
	void* m_pFirstChild;    // オブジェクトが子を持つ場合の最初の子オブジェクトへのポインタ
	void* m_pLastChild;     // オブジェクトが子を持つ場合の最後の子オブジェクトへのポインタ
	void* m_pParent;        // オブジェクトが親を持つ場合の親オブジェクトへのポインタ

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CHistoryRecord ();                  // コンストラクタ
	virtual ~CHistoryRecord ();         // デストラクタ
};

#endif

