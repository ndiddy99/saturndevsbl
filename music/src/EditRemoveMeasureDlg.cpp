//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 小節の除去ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "EditRemoveMeasureDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEditRemoveMeasureDlg::CEditRemoveMeasureDlg () : CDialog (CEditRemoveMeasureDlg::IDD) {
	m_nPosition = 0;
	m_nNumMeasure = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

BOOL CEditRemoveMeasureDlg::SetAmountRange () {
	CString strValue;
	GetDlgItem (IDC_EDITREMOVEMEASURE_POSITION)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	if (m_bZeroOrigin) {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITREMOVEMEASURE_POSITIONSP))->SetRange (0, 32767);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITREMOVEMEASURE_POSITIONSP))->SetPos (CLIP (0, lValue, 32767));
	}
	else {
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITREMOVEMEASURE_POSITIONSP))->SetRange (1, 32767);
		((CSpinButtonCtrl*)GetDlgItem (IDC_EDITREMOVEMEASURE_POSITIONSP))->SetPos (CLIP (1, lValue, 32767));
	}
	GetDlgItem (IDC_EDITREMOVEMEASURE_NUMMEASURE)->GetWindowText (strValue);
	lValue = _ttol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EDITREMOVEMEASURE_NUMMEASURESP))->SetRange (1, 256);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EDITREMOVEMEASURE_NUMMEASURESP))->SetPos (CLIP (1, lValue, 256));
	return TRUE;
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CEditRemoveMeasureDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITREMOVEMEASURE_POSITION, m_nPosition);
	if (m_bZeroOrigin) {
		DDV_MinMaxInt (pDX, m_nPosition, 0, 32767);
	}
	else {
		DDV_MinMaxInt (pDX, m_nPosition, 1, 32767);
	}
	DDX_Text (pDX, IDC_EDITREMOVEMEASURE_NUMMEASURE, m_nNumMeasure);
	DDV_MinMaxInt (pDX, m_nNumMeasure, 1, 256);
}

// ダイアログ初期化時
BOOL CEditRemoveMeasureDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditRemoveMeasureDlg, CDialog)
END_MESSAGE_MAP ()



