//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 言語ダイアログクラス
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
#include "LanguageDlg.h"

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
CLanguageDlg::CLanguageDlg () : CDialog (CLanguageDlg::IDD) {
	m_strUserInterface = _T("");
	m_strTextEncoding = _T("");
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// 言語コンボボックスの充満
BOOL CLanguageDlg::FillUserInterfaceCombo () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_LANGUAGE_USERINTERFACE);
	if (pComboBox == NULL) {
		return FALSE;
	}
	pComboBox->ResetContent ();
	TCHAR* pszUserInterface[] = {
		_T("Japanese"), 
		_T("English"),
		_T("Chinese"),
	};
	long i = 0;
	for (i = 0; i < sizeof (pszUserInterface) / sizeof (pszUserInterface[0]); i++) {
		pComboBox->AddString (pszUserInterface[i]);
		if (pSekaijuApp->m_theLanguageDlgStatus.m_strUserInterface.Compare (pszUserInterface[i]) == 0) {
			pComboBox->SetCurSel (i);
		}
	}
	return TRUE;
}

BOOL CLanguageDlg::FillTextEncodingCombo () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CComboBox* pComboBox = (CComboBox*)GetDlgItem (IDC_LANGUAGE_TEXTENCODING);
	if (pComboBox == NULL) {
		return FALSE;
	}
	pComboBox->ResetContent ();
	TCHAR* pszTextEncoding[] = {
		_T("0-Windows Control Panel ANSI Code Page"),
		_T("874-Thai"),
		_T("932-Japanese Shift-JIS"),
		_T("936-Chinese Simplified"),
		_T("949-Korean"),
		_T("950-Traditional Chinese"),
		_T("1250-Central European"),
		_T("1251-Cyrillic"),
		_T("1252-Western Latin-1 [recommended]"),
		_T("1253-Greek"),
		_T("1254-Turkish"),
		_T("1255-Hebrew"),
		_T("1256-Arabic"),
		_T("1257-Baltic"),
		_T("1258-Vietnamese"),
//		_T("28592-Central European (ISO)"),
//		_T("20866-Cyrillic (KOI8-R)"),
//		_T("50225-Korean (ISO)"),
		_T("1200-Unicode UTF-16LE"),
		_T("1201-Unicode UTF-16BE")
	};
	long i = 0;
	long lCurCharCode = _ttol (pSekaijuApp->m_theLanguageDlgStatus.m_strTextEncoding);
	for (i = 0; i < sizeof (pszTextEncoding) / sizeof (pszTextEncoding[0]); i++) {
		pComboBox->AddString (pszTextEncoding[i]);
		long lCharCode = _ttol (pszTextEncoding[i]);
		if (lCharCode == lCurCharCode) {
			pComboBox->SetCurSel (i);
		}
	}
	return TRUE;
}



//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CLanguageDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_CBString (pDX, IDC_LANGUAGE_USERINTERFACE, m_strUserInterface);
	DDX_CBString (pDX, IDC_LANGUAGE_TEXTENCODING, m_strTextEncoding);
}

// ダイアログの初期化
BOOL CLanguageDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	FillUserInterfaceCombo ();
	FillTextEncodingCombo ();
	return bRet;
}



//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CLanguageDlg, CDialog)
END_MESSAGE_MAP ()




