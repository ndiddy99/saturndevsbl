//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントの種類リストボックスクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "resource.h"
#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxcmn.h>
#include <afxmt.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuDoc.h"
#include "EventKindListBox.h"

// TSIZEOFマクロ //20120211追加
#ifndef TSIZEOF
#define TSIZEOF(STR) (sizeof(STR)/sizeof(TCHAR))
#endif
#ifndef TCSLEN
#ifdef UNICODE
#define TCSLEN(STRING) wcslen(STRING)
#else
#define TCSLEN(STRING) strlen(STRING)
#endif
#endif
#ifndef TCSNCPY
#ifdef UNICODE
#define TCSNCPY(STRING1,STRING2,N) wcsncpy(STRING1,STRING2,N)
#else
#define TCSNCPY(STRING1,STRING2,N) strncpy(STRING1,STRING2,N)
#endif
#endif

// メッセージマップ
IMPLEMENT_DYNCREATE (CEventKindListBox, CCheckListBox)

BEGIN_MESSAGE_MAP (CEventKindListBox, CCheckListBox)
	ON_WM_RBUTTONDOWN ()
END_MESSAGE_MAP ()



//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEventKindListBox::CEventKindListBox () {
	m_pDocument = NULL;
	m_lMenuID = 0;
	m_lLastRButtonDownIndex = 0;
	CCheckListBox::CCheckListBox ();
}

// コンストラクタ
CEventKindListBox::CEventKindListBox (CDocument* pDocument, long lMenuID) {
	m_pDocument = pDocument;
	m_lMenuID = lMenuID;
	m_lLastRButtonDownIndex = 0;
	CCheckListBox::CCheckListBox ();
}

// デストラクタ
CEventKindListBox::~CEventKindListBox () {
	CCheckListBox::~CCheckListBox ();
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

CSekaijuDoc* CEventKindListBox::GetDocument () {
	return (CSekaijuDoc*)m_pDocument;
}

long CEventKindListBox::GetLastRButtonDownIndex () {
	return m_lLastRButtonDownIndex;
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// マウス右ボタン押された時  
void CEventKindListBox::OnRButtonDown (UINT nFlags, CPoint point) {
	this->SetFocus ();
	if (m_lMenuID == 0) {
		return;
	}
	BOOL bOutside;
	long lIndex = this->ItemFromPoint (point, bOutside);
	if (bOutside) {
		return;
	}
	if (lIndex < 0 || lIndex >= GetCount ()) {
		return;
	}
	m_lLastRButtonDownIndex = lIndex;
	// ポップアップメニューの表示
	CPoint ptMenu (point);
	ClientToScreen (&ptMenu);
	CMenu theMenu;
	VERIFY (theMenu.LoadMenu (m_lMenuID));
	CMenu* pContextMenu = theMenu.GetSubMenu (0);
	pContextMenu->TrackPopupMenu (TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		ptMenu.x, ptMenu.y, GetParentFrame ());
}


