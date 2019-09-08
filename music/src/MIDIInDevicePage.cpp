//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDI入力デバイスページクラス
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
#include "MIDIInDevicePage.h"
#include "MIDIOutDevicePage.h"
#include "MIDIInstDefNormPage.h"
#include "MIDIInstDefDrumPage.h"
#include "MIDIDeviceSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE (CMIDIInDevicePage, CPropertyPage)

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMIDIInDevicePage::CMIDIInDevicePage () : 
CPropertyPage (IDD_MIDIINDEVICE) {
}

// デストラクタ
CMIDIInDevicePage::~CMIDIInDevicePage () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------
void CMIDIInDevicePage::DoDataExchange (CDataExchange* pDX) {
	long i;
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		// 20100126:空文字列対応
		// コントロール→メンバ変数
		if (pDX->m_bSaveAndValidate) {
			if (((CComboBox*)GetDlgItem (IDC_MIDIINDEVICE_01 + i))->GetCurSel () == 0) {
				m_strMIDIInName[i] = _T("");
			}
			else {
				DDX_CBString (pDX, IDC_MIDIINDEVICE_01 + i, m_strMIDIInName[i]);
			}
		}
		// メンバ変数→コントロール
		else {
			if (m_strMIDIInName[i] == _T("")) {
				((CComboBox*)GetDlgItem (IDC_MIDIINDEVICE_01 + i))->SetCurSel (0);
			}
			else {
				DDX_CBString (pDX, IDC_MIDIINDEVICE_01 + i, m_strMIDIInName[i]);
			}
		}
	}
}

// ダイアログの初期化
BOOL CMIDIInDevicePage::OnInitDialog () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CString strNone;
	VERIFY (strNone.LoadString (IDS_NONE));
	long lNum = MIDIIn_GetDeviceNum ();
	/* 20081219 MIDIIn_GetDeviceNameが大変遅いのでループ順序変更 */
	long lPort, j;
	for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_MIDIINDEVICE_01 + lPort);
		pComboBox->AddString (strNone);
	}
	for (j = 0; j < lNum; j++) {
		TCHAR szName[256];
		memset (szName, 0, sizeof (szName));
		MIDIIn_GetDeviceName (j, szName, 255);
		for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
			CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_MIDIINDEVICE_01 + lPort);
			pComboBox->AddString (szName);
		}
	}
//	SendMessageToDescendants 
//		(WM_SETFONT, (WPARAM)(HFONT)(pSekaijuApp->m_theDefaultFont), MAKELPARAM(TRUE, 0));
	CDialog::OnInitDialog(); // AddStringは基本関数の呼び出しより前(20090625)
	return TRUE;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CMIDIInDevicePage, CPropertyPage)
	ON_CONTROL_RANGE (CBN_SELCHANGE, IDC_MIDIINDEVICE_01, IDC_MIDIINDEVICE_16, OnSelChange)
END_MESSAGE_MAP ()


void CMIDIInDevicePage::OnSelChange (UINT nID) {
	SetModified (TRUE);
}
