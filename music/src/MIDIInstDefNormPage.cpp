//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDIインストゥルメント定義(通常)ページクラス
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


IMPLEMENT_DYNCREATE (CMIDIInstDefNormPage, CPropertyPage)

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMIDIInstDefNormPage::CMIDIInstDefNormPage () : 
CPropertyPage (IDD_MIDIINSTDEFNORM) {
}

// デストラクタ
CMIDIInstDefNormPage::~CMIDIInstDefNormPage () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CMIDIInstDefNormPage::DoDataExchange (CDataExchange* pDX) {
	long i;
	for (i = 0; i < MAXMIDIINDEVICENUM; i++) {
		DDX_CBString (pDX, IDC_MIDIINSTDEFNORM_01 + i, m_strMIDIInstDefNormName[i]);
	}
}

// ダイアログの初期化
BOOL CMIDIInstDefNormPage::OnInitDialog () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	long lNum = MIDIIn_GetDeviceNum ();
	long lPort = 0;
	for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
		MIDIInstrumentDefinition* pMIDIInstDef;
		CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_MIDIINSTDEFNORM_01 + lPort);
		int nIndex = 0;
		long i = 0;
		for (i = 0; i < MAXMIDIINSTRUMENTNUM; i++) {
			if (pSekaijuApp->m_pMIDIInstrument[i] != NULL) {
				forEachInstrumentDefinition (pSekaijuApp->m_pMIDIInstrument[i], pMIDIInstDef) {
					TCHAR szName[256];
					memset (szName, 0, sizeof (szName));
					MIDIInstrumentDefinition_GetTitle (pMIDIInstDef, szName, 255);
					pComboBox->AddString (szName);
					if (_tcscmp (szName, pSekaijuApp->m_strMIDIInstDefNormName[lPort]) == 0) {
						pComboBox->SetCurSel (nIndex);
					}
					nIndex++;
				}
			}
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

BEGIN_MESSAGE_MAP (CMIDIInstDefNormPage, CPropertyPage)
	ON_CONTROL_RANGE (CBN_SELCHANGE, IDC_MIDIINSTDEFNORM_01, IDC_MIDIINSTDEFNORM_16, OnSelChange)
END_MESSAGE_MAP ()

void CMIDIInstDefNormPage::OnSelChange (UINT nID) {
	SetModified (TRUE);
}
