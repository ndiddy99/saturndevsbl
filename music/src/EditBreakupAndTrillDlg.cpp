//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 細分化とトリルダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "EditBreakupAndTrillDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

// コンストラクタ
CEditBreakupAndTrillDlg::CEditBreakupAndTrillDlg () : CDialog (CEditBreakupAndTrillDlg::IDD) {
	m_lTimeMode = 0;
	m_lTimeResolution = 0;
	m_nDurationIndex = 0;
	m_nEnableTrill = 0;
	m_nKeyShift = 0;
}

// オペレーション
BOOL CEditBreakupAndTrillDlg::FillDurationCombo () {
	CComboBox* pDurationCombo = (CComboBox*)GetDlgItem (IDC_EDITBREAKUPANDTRILL_DURATION);
	CString strTextLine;
	CString strFormat;
	if (m_lTimeMode == 0) {
		VERIFY (strFormat.LoadString (IDS_D_4DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 1);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_8DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 2);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_12DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 3);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_16DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 4);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_24DIVNOTE));
		strTextLine.Format (strFormat,  m_lTimeResolution / 6);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_32DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 8);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_48DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 12);
		pDurationCombo->AddString (strTextLine);
	}
	else {
		VERIFY (strFormat.LoadString (IDS_D_1FRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 1);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_2DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 2);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_3DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 3);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_4DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 4);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_6DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 6);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_8DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 8);
		pDurationCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_12DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 12);
		pDurationCombo->AddString (strTextLine);
	}

	return TRUE;
}

BOOL CEditBreakupAndTrillDlg::SetKeyShiftRange () {
	((CSpinButtonCtrl*)GetDlgItem (IDC_EDITBREAKUPANDTRILL_KEYSHIFTSP))->SetRange (-127, 127);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EDITBREAKUPANDTRILL_KEYSHIFTSP))->SetPos (CLIP (-127, m_nKeyShift, 127));
	return TRUE;
}

BOOL CEditBreakupAndTrillDlg::UpdateKeyShift () {
	int nEnableTrill = ((CButton*)GetDlgItem (IDC_EDITBREAKUPANDTRILL_ENABLETRILL))->GetCheck ();
	GetDlgItem (IDC_EDITBREAKUPANDTRILL_KEYSHIFT)->EnableWindow (nEnableTrill);
	GetDlgItem (IDC_EDITBREAKUPANDTRILL_KEYSHIFTSP)->EnableWindow (nEnableTrill);
	return TRUE;
}


// オーバーライド

// データエクスチェンジ
void CEditBreakupAndTrillDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_CBIndex (pDX, IDC_EDITBREAKUPANDTRILL_DURATION, m_nDurationIndex);
	DDX_Check (pDX, IDC_EDITBREAKUPANDTRILL_ENABLETRILL, m_nEnableTrill);
	DDX_Text (pDX, IDC_EDITBREAKUPANDTRILL_KEYSHIFT, m_nKeyShift);
}


BOOL CEditBreakupAndTrillDlg::OnInitDialog () {
	FillDurationCombo ();
	BOOL bRet = CDialog::OnInitDialog ();
	UpdateKeyShift ();
	SetKeyShiftRange ();
	return bRet;
}




// メッセージマップ
BEGIN_MESSAGE_MAP (CEditBreakupAndTrillDlg, CDialog)
	ON_BN_CLICKED (IDC_EDITBREAKUPANDTRILL_ENABLETRILL, OnChangeEnableTrill)
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
// CEditBreakupAndTrillDlg メッセージ ハンドラ

void CEditBreakupAndTrillDlg::OnChangeEnableTrill () {
	UpdateKeyShift ();
}

