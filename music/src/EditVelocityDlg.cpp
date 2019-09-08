//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ベロシティの変更ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "EditVelocityDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEditVelocityDlg::CEditVelocityDlg () : CDialog (CEditVelocityDlg::IDD) {
	m_nAmount = 0;
	m_nUnit = 0;
	m_nTargetNoteOn = 0;
	m_nTargetNoteOff = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

BOOL CEditVelocityDlg::SetAmountRange () {
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_RELATIVEUNIT);
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_PERCENTUNIT);
	CButton* pRandomUpDownUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_RANDOMUPDOWNUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITVELOCITY_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pAbsoluteUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetRange (1, 127);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetPos (CLIP (1, lValue, 127));
	}
	else if (pRelativeUnitButton->GetCheck () || pRandomUpDownUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetRange (-127, 127);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetPos (CLIP (-127, lValue, 127));
	}
	else if (pPercentUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetRange (0, 1000);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVELOCITY_AMOUNTSP))->SetPos (CLIP (0, lValue, 1000));
	}
	return TRUE;
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CEditVelocityDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITVELOCITY_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITVELOCITY_ABSOLUTEUNIT, m_nUnit);
	DDX_Check (pDX, IDC_EDITVELOCITY_TARGETNOTEON, m_nTargetNoteOn);
	DDX_Check (pDX, IDC_EDITVELOCITY_TARGETNOTEOFF, m_nTargetNoteOff);
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_RELATIVEUNIT);
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_PERCENTUNIT);
	CButton* pRandomUpDownUnitButton = (CButton*)GetDlgItem (IDC_EDITVELOCITY_RANDOMUPDOWNUNIT);
	if (pAbsoluteUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, 1, 127);
	}
	else if (pRelativeUnitButton->GetCheck () || pRandomUpDownUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, -127, 127);
	}
	else if (pPercentUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, 0, 1000);
	}
}

// ダイアログ初期化時
BOOL CEditVelocityDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditVelocityDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITVELOCITY_ABSOLUTEUNIT, IDC_EDITVELOCITY_RANDOMUPDOWNUNIT, OnChangeUnit)
END_MESSAGE_MAP ()

// 単位が変更された
void CEditVelocityDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


