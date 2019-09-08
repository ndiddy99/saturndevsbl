//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI入力同期モードページクラス
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




IMPLEMENT_DYNCREATE (CMIDIInSyncModePage, CPropertyPage)

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMIDIInSyncModePage::CMIDIInSyncModePage () : 
CPropertyPage (IDD_MIDIINSYNCMODE) {
}

// デストラクタ
CMIDIInSyncModePage::~CMIDIInSyncModePage () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------
void CMIDIInSyncModePage::DoDataExchange (CDataExchange* pDX) {
	long i;
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		DDX_CBIndex (pDX, IDC_MIDIINSYNCMODE_01 + i, m_nMIDIInSyncMode[i]);
	}
}

// ダイアログの初期化
BOOL CMIDIInSyncModePage::OnInitDialog () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	long lPort;
	CString strItem[3];
	VERIFY (strItem[0].LoadString (IDS_NONE));
	VERIFY (strItem[1].LoadString (IDS_RECEIVE_MIDI_TIMING_CLOCK));
	VERIFY (strItem[2].LoadString (IDS_RECEIVE_SMPTE_MTC));
	for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_MIDIINSYNCMODE_01 + lPort);
		pComboBox->AddString (strItem[0]);
		pComboBox->AddString (strItem[1]);
		pComboBox->AddString (strItem[2]);
	}
//	SendMessageToDescendants 
//		(WM_SETFONT, (WPARAM)(HFONT)(pSekaijuApp->m_theDefaultFont), MAKELPARAM(TRUE, 0));
	CDialog::OnInitDialog(); // AddStringは基本関数の呼び出しより前(20090625)
	return TRUE;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CMIDIInSyncModePage, CPropertyPage)
	ON_CONTROL_RANGE (CBN_SELCHANGE, IDC_MIDIINSYNCMODE_01, IDC_MIDIINSYNCMODE_16, OnSelChange)
END_MESSAGE_MAP ()

void CMIDIInSyncModePage::OnSelChange (UINT nID) {
	long lPort;
	for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		if ((long)nID != IDC_MIDIINSYNCMODE_01 + lPort) {
			CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_MIDIINSYNCMODE_01 + lPort);
			pComboBox->SetCurSel (0);
		}
	}
	SetModified (TRUE);
}
