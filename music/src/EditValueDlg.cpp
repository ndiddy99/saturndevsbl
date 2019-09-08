//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 値の変更ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "EditValueDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEditValueDlg::CEditValueDlg () : CDialog (CEditValueDlg::IDD) {
	m_nAmount = 0;
	m_nUnit = 0;
	m_nTargetKeyAfter = 0;
	m_nTargetControlChange = 0;
	m_nTargetChannelAfter = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

BOOL CEditValueDlg::SetAmountRange () {
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_RELATIVEUNIT);
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_PERCENTUNIT);
	CButton* pRandomUpDownUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_RANDOMUPDOWNUNIT);
	CButton* pTargetPitchBendButton = (CButton*)GetDlgItem (IDC_EDITVALUE_TARGETPITCHBEND);
	CString strValue;
	GetDlgItem (IDC_EDITVALUE_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	// 絶対指定
	if (pAbsoluteUnitButton->GetCheck ()) {
		if (pTargetPitchBendButton->GetCheck ()) {
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetRange (-8192, 8191);
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetPos (CLIP (-8192, lValue, 8191));
		}
		else {
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetRange (0, 127);
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetPos (CLIP (0, lValue, 127));
		}
	}
	// 相対指定・ランダム相対指定
	else if (pRelativeUnitButton->GetCheck () || pRandomUpDownUnitButton->GetCheck ()) {
		if (pTargetPitchBendButton->GetCheck ()) {
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetRange (-16384, 16383);
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetPos (CLIP (-16384, lValue, 16383));
		}
		else {
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetRange (-127, 127);
			((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetPos (CLIP (-127, lValue, 127));
		}
	}
	// パーセント
	else if (pPercentUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetRange (0, 1000);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITVALUE_AMOUNTSP))->SetPos (CLIP (0, lValue, 1000));
	}
	return TRUE;
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CEditValueDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITVALUE_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITVALUE_ABSOLUTEUNIT, m_nUnit);
	DDX_Check (pDX, IDC_EDITVALUE_TARGETKEYAFTER, m_nTargetKeyAfter);
	DDX_Check (pDX, IDC_EDITVALUE_TARGETCONTROLCHANGE, m_nTargetControlChange);
	DDX_Check (pDX, IDC_EDITVALUE_TARGETCHANNELAFTER, m_nTargetChannelAfter);
	DDX_Check (pDX, IDC_EDITVALUE_TARGETPITCHBEND, m_nTargetPitchBend);
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_RELATIVEUNIT);
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_PERCENTUNIT);
	CButton* pRandomUpDownUnitButton = (CButton*)GetDlgItem (IDC_EDITVALUE_RANDOMUPDOWNUNIT);
	CButton* pTargetPitchBendButton = (CButton*)GetDlgItem (IDC_EDITVALUE_TARGETPITCHBEND);
	if (pAbsoluteUnitButton->GetCheck ()) {
		if (pTargetPitchBendButton->GetCheck ()) {
			DDV_MinMaxInt (pDX, m_nAmount, -8192, 8191);
		}
		else {
			DDV_MinMaxInt (pDX, m_nAmount, 0, 127);
		}
	}
	else if (pRelativeUnitButton->GetCheck () || pRandomUpDownUnitButton->GetCheck ()) {
		if (pTargetPitchBendButton->GetCheck ()) {
			DDV_MinMaxInt (pDX, m_nAmount, -16384, 16383);
		}
		else {
			DDV_MinMaxInt (pDX, m_nAmount, -127, 127);
		}
	}
	else if (pPercentUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, 0, 1000);
	}
}

// ダイアログ初期化時
BOOL CEditValueDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditValueDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITVALUE_ABSOLUTEUNIT, IDC_EDITVALUE_RANDOMUPDOWNUNIT, OnChangeUnit)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITVALUE_TARGETKEYAFTER, IDC_EDITVALUE_TARGETPITCHBEND, OnChangeTarget)
END_MESSAGE_MAP ()

// 単位が変更された
void CEditValueDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}

// 対象が変更された
void CEditValueDlg::OnChangeTarget (UINT nID) {
	SetAmountRange ();
}
