//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 履歴ユニットクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _HISTORYUNIT_H_
#define _HISTORYUNIT_H_

class CHistoryUnit {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strName;                  // この履歴ユニットの名前
	CTime m_theTime;                    // この履歴ユニットが記録された時刻
	CPtrArray m_theHistoryRecordArray;  // この履歴ユニットが持つ履歴記録(複数)へのポインタ配列

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CHistoryUnit ();                    // コンストラクタ
	virtual ~CHistoryUnit ();           // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	virtual long AddHistoryRecord (long lType, void* pObject);

};

#endif
