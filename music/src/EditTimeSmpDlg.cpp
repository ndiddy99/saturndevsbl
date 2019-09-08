//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// タイムの変更(SMPTEベース用)ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "EditTimeSmpDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEditTimeSmpDlg::CEditTimeSmpDlg () : CDialog (CEditTimeSmpDlg::IDD) {
	m_nAmount = 0;
	m_nUnit = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

BOOL CEditTimeSmpDlg::SetAmountRange () {
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITTIMESMP_PERCENTUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITTIMESMP_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pPercentUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIMESMP_AMOUNTSP))->SetRange (0, 1000);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIMESMP_AMOUNTSP))->SetPos (CLIP (0, lValue, 1000));
	}
	else {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIMESMP_AMOUNTSP))->SetRange (-8192, 8192);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIMESMP_AMOUNTSP))->SetPos (CLIP (-8192, lValue, 8192));
	}
	return TRUE;
}


//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CEditTimeSmpDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITTIMESMP_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITTIMESMP_TICKUNIT, m_nUnit);
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITTIMESMP_PERCENTUNIT);
	if (pPercentUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, 0, 1000);
	}
	else {
		DDV_MinMaxInt (pDX, m_nAmount, -8192, 8192);
	}
}


BOOL CEditTimeSmpDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}



//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditTimeSmpDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITTIMESMP_TICKUNIT, IDC_EDITTIMESMP_RANDOMTICKUNIT, OnChangeUnit)
END_MESSAGE_MAP ()

// 単位が変更された
void CEditTimeSmpDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


