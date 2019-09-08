//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 履歴記録クラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include "..\\..\\MIDIData\\MIDIData.h"
#include "HistoryRecord.h"

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CHistoryRecord::CHistoryRecord () {
	m_lType = 0;
	m_pObject = NULL;
	m_pPrevObject = NULL;
	m_pNextObject = NULL;
	m_pFirstChild = NULL;
	m_pLastChild = NULL;
	m_pParent = NULL;
	
}

// デストラクタ
CHistoryRecord::~CHistoryRecord () {
}

