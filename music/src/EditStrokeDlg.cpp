//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ストロークダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "EditStrokeDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CEditStrokeDlg::CEditStrokeDlg () : CDialog (CEditStrokeDlg::IDD) {
	m_nDeltaTime = 1;
	m_nMode = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

BOOL CEditStrokeDlg::SetAmountRange () {
	CString strValue;
	GetDlgItem (IDC_EDITSTROKE_DELTATIME)->GetWindowText (strValue);
	long lStroke = _ttol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EDITSTROKE_DELTATIMESP))->SetRange (1, 32767);
	((CSpinButtonCtrl*)GetDlgItem (IDC_EDITSTROKE_DELTATIMESP))->SetPos (CLIP (1, lStroke, 32767));
	return TRUE;
}


//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CEditStrokeDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDITSTROKE_DELTATIME, m_nDeltaTime);
	DDX_Radio (pDX, IDC_EDITSTROKE_MODEUP, m_nMode);
	DDV_MinMaxInt (pDX, m_nDeltaTime, 1, 32767);
}

// ダイアログ初期化時
BOOL CEditStrokeDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	SetAmountRange ();
	return bRet;
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CEditStrokeDlg, CDialog)
END_MESSAGE_MAP ()

