//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ファイルプロパティダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include "resource.h"
#include "FilePropertyDlg.h"

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CFilePropertyDlg::CFilePropertyDlg () : CDialog (CFilePropertyDlg::IDD) {
	m_strTitle = _T("");
	m_strSubTitle = _T("");
	m_strCopyright = _T("");
	m_strComment1 = _T("");
	m_strComment2 = _T("");
	m_strComment3 = _T("");
	m_nSMFFormat = -1;
	m_nTimeMode = -1;
	m_nResolution = 120;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------
void CFilePropertyDlg::FillResolutionComboAndStatic () {
	CComboBox* pResolutionCombo = (CComboBox*)GetDlgItem (IDC_FILEPROPERTY_RESOLUTION);
	CStatic* pResolutionStatic = (CStatic*)GetDlgItem (IDC_FILEPROPERTY_RESOLUTIONU);
	CString strOldComboText;
	pResolutionCombo->GetWindowText (strOldComboText);
	pResolutionCombo->ResetContent ();
	long i;
	for (i = 0; i < 5; i++) {
		CButton* pButton = (CButton*)GetDlgItem (IDC_FILEPROPERTY_TPQNBASE + i);
		if (pButton->GetCheck () == TRUE) {
			break;
		}
	}

	// TPQNベース
	if (i == 0) {
		CString strUnit;
		VERIFY (strUnit.LoadString (IDS_TICKS_PER_QUARTER_NOTE));
		pResolutionCombo->AddString (_T("48"));
		pResolutionCombo->AddString (_T("60"));
		pResolutionCombo->AddString (_T("96"));
		pResolutionCombo->AddString (_T("120"));
		pResolutionCombo->AddString (_T("192"));
		pResolutionCombo->AddString (_T("240"));
		pResolutionCombo->AddString (_T("384"));
		pResolutionCombo->AddString (_T("480"));
		pResolutionCombo->AddString (_T("960"));
		pResolutionCombo->SetWindowText (strOldComboText);
		pResolutionStatic->SetWindowText (strUnit);
	}
	// SMPTEベース
	else if (1 <= i && i <= 5) {
		CString strUnit;
		VERIFY (strUnit.LoadString (IDS_SUBFRAMES_PER_FRAME));
		pResolutionCombo->AddString (_T("10"));
		pResolutionCombo->AddString (_T("20"));
		pResolutionCombo->AddString (_T("30"));
		pResolutionCombo->AddString (_T("40"));
		pResolutionCombo->AddString (_T("50"));
		pResolutionCombo->AddString (_T("60"));
		pResolutionCombo->AddString (_T("70"));
		pResolutionCombo->AddString (_T("80"));
		pResolutionCombo->AddString (_T("90"));
		pResolutionCombo->AddString (_T("100"));
		pResolutionCombo->SetWindowText (strOldComboText);
		pResolutionStatic->SetWindowText (strUnit);
	}
}


//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CFilePropertyDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_FILEPROPERTY_TITLE, m_strTitle);
	DDX_Text (pDX, IDC_FILEPROPERTY_SUBTITLE, m_strSubTitle);
	DDX_Text (pDX, IDC_FILEPROPERTY_COPYRIGHT, m_strCopyright);
	DDX_Text (pDX, IDC_FILEPROPERTY_COMMENT1, m_strComment1);
	DDX_Text (pDX, IDC_FILEPROPERTY_COMMENT2, m_strComment2);
	DDX_Text (pDX, IDC_FILEPROPERTY_COMMENT3, m_strComment3);
	DDX_Text (pDX, IDC_FILEPROPERTY_NUMTRACK, m_strNumTrack);
	DDX_Text (pDX, IDC_FILEPROPERTY_NUMEVENT, m_strNumEvent);
	DDX_Text (pDX, IDC_FILEPROPERTY_ENDMILLISEC, m_strEndMillisec);
	DDX_Text (pDX, IDC_FILEPROPERTY_ENDTIME, m_strEndTime);

	DDX_Radio (pDX, IDC_FILEPROPERTY_SMFFORMAT0, m_nSMFFormat);
	DDX_Radio (pDX, IDC_FILEPROPERTY_TPQNBASE, m_nTimeMode);
	DDX_Text (pDX, IDC_FILEPROPERTY_RESOLUTION, m_nResolution);
	CButton* pTimeModeButton = (CButton*)GetDlgItem (IDC_FILEPROPERTY_TPQNBASE);
	if (pTimeModeButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nResolution, 1, 960);
	}
	else {
		DDV_MinMaxInt (pDX, m_nResolution, 1, 255);
	}
}

// ダイアログ初期化
BOOL CFilePropertyDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	GetDlgItem (IDC_FILEPROPERTY_COMMENT2)->EnableWindow (FALSE);
	GetDlgItem (IDC_FILEPROPERTY_COMMENT3)->EnableWindow (FALSE);
	FillResolutionComboAndStatic ();
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CFilePropertyDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_FILEPROPERTY_TPQNBASE, IDC_FILEPROPERTY_SMPTE30BASE, OnChangeTimeMode)
END_MESSAGE_MAP ()

// タイムモードが変更された
void CFilePropertyDlg::OnChangeTimeMode (UINT nID) {
	FillResolutionComboAndStatic ();
}


