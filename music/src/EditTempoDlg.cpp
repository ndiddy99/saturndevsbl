//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// テンポの変更ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "EditTempoDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEditTempoDlg::CEditTempoDlg () : CDialog (CEditTempoDlg::IDD) {
	m_strAmount = _T("");
	m_nUnit = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

BOOL CEditTempoDlg::SetAmountRange () {
	return TRUE;
}

void CEditTempoDlg::DDV_MinMaxString (CDataExchange* pDX, 
	LPCTSTR lpszValue, double dMin, double dMax) {
	ASSERT (dMin <= dMax);
	double dValue = _tstof (lpszValue);
	DDV_MinMaxDouble (pDX, dValue, dMin, dMax);
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CEditTempoDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITTEMPO_AMOUNT, m_strAmount);
	DDX_Radio (pDX, IDC_EDITTEMPO_ABSOLUTEUNIT, m_nUnit);
	CButton* pAbsoluteUnitButton = (CButton*)GetDlgItem (IDC_EDITTEMPO_ABSOLUTEUNIT);
	CButton* pRelativeUnitButton = (CButton*)GetDlgItem (IDC_EDITTEMPO_RELATIVEUNIT);
	CButton* pPercentUnitButton = (CButton*)GetDlgItem (IDC_EDITTEMPO_PERCENTUNIT);
	CButton* pRandomUpDownUnitButton = (CButton*)GetDlgItem (IDC_EDITTEMPO_RANDOMUPDOWNUNIT);
	if (pAbsoluteUnitButton->GetCheck ()) {
		DDV_MinMaxString (pDX, m_strAmount, 4.00, 60000.00);
	}
	else if (pRelativeUnitButton->GetCheck () || pRandomUpDownUnitButton->GetCheck ()) {
		DDV_MinMaxString (pDX, m_strAmount, -60000.00, 60000.00);
	}
	else if (pPercentUnitButton->GetCheck ()) {
		DDV_MinMaxString (pDX, m_strAmount, 0.001, 1000.00);
	}
}

// ダイアログ初期化時
BOOL CEditTempoDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	GetDlgItem (IDC_EDITTEMPO_AMOUNTSP)->DestroyWindow ();
	SetAmountRange ();
	return bRet;
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditTempoDlg, CDialog)
	ON_CONTROL_RANGE (BN_CLICKED,  IDC_EDITTEMPO_ABSOLUTEUNIT, IDC_EDITTEMPO_RANDOMUPDOWNUNIT, OnChangeUnit)
END_MESSAGE_MAP ()

// 単位が変更された
void CEditTempoDlg::OnChangeUnit (UINT nID) {
	SetAmountRange ();
}

// 対象が変更された
void CEditTempoDlg::OnChangeTarget (UINT nID) {
	SetAmountRange ();
}
