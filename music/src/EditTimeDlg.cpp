//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// タイムの変更(TPQNベース用)ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "EditTimeDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEditTimeDlg::CEditTimeDlg () : CDialog (CEditTimeDlg::IDD) {
	m_nAmount = 0;
	m_nUnit = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

BOOL CEditTimeDlg::SetAmountRange () {
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITTIME_PERCENTUNIT);
	CString strValue;
	GetDlgItem (IDC_EDITTIME_AMOUNT)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (pPercentUnitButton->GetCheck ()) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIME_AMOUNTSP))->SetRange (0, 1000);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIME_AMOUNTSP))->SetPos (CLIP (0, lValue, 1000));
	}
	else {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIME_AMOUNTSP))->SetRange (-8192, 8192);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITTIME_AMOUNTSP))->SetPos (CLIP (-8192, lValue, 8192));
	}
	return TRUE;
}


//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CEditTimeDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITTIME_AMOUNT, m_nAmount);
	DDX_Radio (pDX, IDC_EDITTIME_TICKUNIT, m_nUnit);
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITTIME_PERCENTUNIT);
	if (pPercentUnitButton->GetCheck ()) {
		DDV_MinMaxInt (pDX, m_nAmount, 0, 1000);
	}
	else {
		DDV_MinMaxInt (pDX, m_nAmount, -8192, 8192);
	}
}


BOOL CEditTimeDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}



//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditTimeDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITTIME_TICKUNIT, IDC_EDITTIME_RANDOMTICKUNIT, OnChangeUnit)
END_MESSAGE_MAP ()

// 単位が変更された
void CEditTimeDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}


