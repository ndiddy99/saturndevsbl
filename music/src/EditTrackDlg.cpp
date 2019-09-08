﻿//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックの変更ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include <afxext.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "resource.h"
#include "SekaijuApp.h"
#include "EditTrackDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

// コンストラクタ
CEditTrackDlg::CEditTrackDlg () : CDialog (CEditTrackDlg::IDD) {
	m_nAmount = 1;
	m_nUnit = 0;
	m_nFitChannel = 0;
}

// オペレーション
BOOL CEditTrackDlg::SetAmountRange () {
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITTRACK_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITTRACK_RELATIVEUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITTRACK_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pAbsoluteUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTRACK_AMOUNTSP))->SetRange (1, 256);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTRACK_AMOUNTSP))->SetPos (CLIP (1, lValue, 256));
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTRACK_AMOUNTSP))->SetRange (-255, 2555);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTRACK_AMOUNTSP))->SetPos (CLIP (-255, lValue, 255));
	}
	return TRUE;
}


// オーバーライド

// データエクスチェンジ
void CEditTrackDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITTRACK_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITTRACK_ABSOLUTEUNIT, m_nUnit);
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITTRACK_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITTRACK_RELATIVEUNIT);
	if (pAbsoluteUnitButton->GetCheck ()) {
		CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
		BOOL bTrackZeroOrigin = pSekaijuApp->m_theGeneralOption.m_bTrackZeroOrigin;
		DDV_MinMaxInt (pDX, m_nAmount, 
			(bTrackZeroOrigin ? 0 : 1),
			(bTrackZeroOrigin ? 255 : 256)); // TODO:
	}
	else if (pRelativeUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, -255, 255);
	}
	DDX_Check (pDX, IDC_EDITTRACK_FITCHANNEL, m_nFitChannel);
}


BOOL CEditTrackDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}



// メッセージマップ
BEGIN_MESSAGE_MAP (CEditTrackDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITTRACK_ABSOLUTEUNIT, IDC_EDITTRACK_RELATIVEUNIT, OnChangeUnit)
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
// CEditTrackDlg メッセージ ハンドラ

// 単位が変更された
void CEditTrackDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


