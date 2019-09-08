//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI出力同期モードページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */


#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"
#include "MIDIInSyncModePage.h"
#include "MIDIOutSyncModePage.h"
#include "MIDISyncModeSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNCREATE (CMIDIOutSyncModePage, CPropertyPage)

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMIDIOutSyncModePage::CMIDIOutSyncModePage () : 
CPropertyPage (IDD_MIDIOUTSYNCMODE) {
}

// デストラクタ
CMIDIOutSyncModePage::~CMIDIOutSyncModePage () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CMIDIOutSyncModePage::DoDataExchange (CDataExchange* pDX) {
	long i;
	for (i = 0; i < MAXMIDIOUTDEVICENUM; i++) {
		DDX_CBIndex (pDX, IDC_MIDIOUTSYNCMODE_01 + i, m_nMIDIOutSyncMode[i]);
	}
}

// ダイアログの初期化
BOOL CMIDIOutSyncModePage::OnInitDialog () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CString strItem[6];
	strItem[0].LoadString (IDS_NONE);
	strItem[1].LoadString (IDS_SEND_MIDI_TIMING_CLOCK);
	strItem[2].LoadString (IDS_SEND_SMPTE24_MTC);
	strItem[3].LoadString (IDS_SEND_SMPTE25_MTC);
	strItem[4].LoadString (IDS_SEND_SMPTE29P97_MTC);
	strItem[5].LoadString (IDS_SEND_SMPTE30_MTC);
	long lPort = 0;
	for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_MIDIOUTSYNCMODE_01 + lPort);
		pComboBox->AddString (strItem[0]);
		pComboBox->AddString (strItem[1]);
		pComboBox->AddString (strItem[2]);
		pComboBox->AddString (strItem[3]);
		pComboBox->AddString (strItem[4]);
		pComboBox->AddString (strItem[5]);
	}
//	SendMessageToDescendants 
//		(WM_SETFONT, (WPARAM)(HFONT)(pSekaijuApp->m_theDefaultFont), MAKELPARAM(TRUE, 0));
	CDialog::OnInitDialog(); // AddStringは基本関数の呼び出しより前(20090625)
	return TRUE;
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CMIDIOutSyncModePage, CPropertyPage)
	ON_CONTROL_RANGE (CBN_SELCHANGE, IDC_MIDIOUTSYNCMODE_01, IDC_MIDIOUTSYNCMODE_16, OnSelChange)
END_MESSAGE_MAP ()

void CMIDIOutSyncModePage::OnSelChange (UINT nID) {
	SetModified (TRUE);
}
