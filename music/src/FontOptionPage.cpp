//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// フォントオプションページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */


#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include <afxext.h>

#include "resource.h"
#include "FontOptionPage.h"

#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CFontOptionPage::CFontOptionPage () : CPropertyPage (CFontOptionPage::IDD) {
	long i = 0;
	for (i = 0; i < 4; i++) {
		m_strFontName[i] = _T("");
		m_lFontSize[i] = 0;
	}
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CFontOptionPage::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	long i = 0;
	for (i = 0; i < 4; i++) {
		DDX_CBString (pDX, IDC_FONTOPTION_FONTNAME_00 + 4 * i, m_strFontName[i]);
		DDX_Text (pDX, IDC_FONTOPTION_FONTSIZE_00 + 4 * i, m_lFontSize[i]);
		DDV_MinMaxInt (pDX, m_lFontSize[i], 1, 64);
	}
}

// ダイアログの初期化
BOOL CFontOptionPage::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	// フォント名
	long i = 0;
	for (i = 0; i < 4; i++) {
		((CComboBox*)GetDlgItem (IDC_FONTOPTION_FONTNAME_00 + 4 * i))->AddString (_T("(Default)"));
	}
	CClientDC dc (this);
	::EnumFontFamilies ((HDC)dc, NULL, (FONTENUMPROC) EnumFontFamProc, (LPARAM) this);
	// フォントサイズ
	for (i = 0; i < 4; i++) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_FONTOPTION_FONTSIZESP_00 + 4 * i))->SetRange (1, 64);
	}
	// フォント名の選択
	for (i = 0; i < 4; i++) {
		CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_FONTOPTION_FONTNAME_00 + 4 * i);
		long lCount = pComboBox->GetCount ();
		long lIndex = pComboBox->FindStringExact (0, m_strFontName[i]);
		if (lIndex != CB_ERR) {
			pComboBox->SetCurSel (lIndex);
		}
	}
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
//	SendMessageToDescendants 
//		(WM_SETFONT, (WPARAM)(HFONT)(pSekaijuApp->m_theDefaultFont), MAKELPARAM(TRUE, 0));
	return bRet;
}

int CALLBACK CFontOptionPage::EnumFontFamProc 
(ENUMLOGFONT* lpelf, NEWTEXTMETRIC* lpntm, int nFontType, LPARAM lParam) {
	CFontOptionPage* pFontOptionPage = (CFontOptionPage*)lParam;
	ASSERT (pFontOptionPage);
	long i = 0;
	for (i = 0; i < 4; i++) {
		CComboBox* pFontNameComboBox = 
			(CComboBox*)(pFontOptionPage->GetDlgItem (IDC_FONTOPTION_FONTNAME_00 + 4 * i));
		ASSERT (pFontNameComboBox);
		pFontNameComboBox->AddString (lpelf->elfLogFont.lfFaceName);
	}
	return 1;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CFontOptionPage, CPropertyPage)
END_MESSAGE_MAP ()




