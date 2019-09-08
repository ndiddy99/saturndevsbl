//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 音程の変更ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "EditKeyDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEditKeyDlg::CEditKeyDlg () : CDialog (CEditKeyDlg::IDD) {
	m_nAmount = 0;
	m_nUnit = 0;
	m_nTargetNote = 0;
	m_nTargetKeyAfter = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

BOOL CEditKeyDlg::SetAmountRange () {
	CButton* pHalfUnitButton = (CButton*)GetDlgItem (IDC_EDITKEY_HALFUNIT);
	CButton* pOctaveUnitButton = (CButton*)GetDlgItem (IDC_EDITKEY_OCTAVEUNIT);
	CButton* pRandomHalfUnitButton = (CButton*)GetDlgItem (IDC_EDITKEY_RANDOMHALFUNIT);
	CButton* pRandomOctaveUnitButton = (CButton*)GetDlgItem (IDC_EDITKEY_RANDOMOCTAVEUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITKEY_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pHalfUnitButton->GetCheck () || pRandomHalfUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITKEY_AMOUNTSP))->SetRange (-127, 127);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITKEY_AMOUNTSP))->SetPos (CLIP (-127, lValue, 127));
	}
	else if (pOctaveUnitButton->GetCheck () || pRandomOctaveUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITKEY_AMOUNTSP))->SetRange (-10, 10);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITKEY_AMOUNTSP))->SetPos (CLIP (-10, lValue, 10));
	}
	return TRUE;
}


//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CEditKeyDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITKEY_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITKEY_HALFUNIT, m_nUnit);
	DDX_Check (pDX, IDC_EDITKEY_TARGETNOTE, m_nTargetNote);
	DDX_Check (pDX, IDC_EDITKEY_TARGETKEYAFTER, m_nTargetKeyAfter);
	CButton* pHalfUnitButton = (CButton*)GetDlgItem (IDC_EDITKEY_HALFUNIT);
	CButton* pOctaveUnitButton = (CButton*)GetDlgItem (IDC_EDITKEY_OCTAVEUNIT);
	CButton* pRandomHalfUnitButton = (CButton*)GetDlgItem (IDC_EDITKEY_RANDOMHALFUNIT);
	CButton* pRandomOctaveUnitButton = (CButton*)GetDlgItem (IDC_EDITKEY_RANDOMOCTAVEUNIT);
	if (pHalfUnitButton->GetCheck () || pRandomHalfUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, -127, 127);
	}
	else if (pOctaveUnitButton->GetCheck () || pRandomOctaveUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, -10, 10);
	}
}


BOOL CEditKeyDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}



//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditKeyDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITKEY_HALFUNIT, IDC_EDITKEY_RANDOMOCTAVEUNIT, OnChangeUnit)
END_MESSAGE_MAP ()

// 単位が変更された
void CEditKeyDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


