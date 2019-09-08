//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// プロパティテンポダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "PropertyTempoDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//******************************************************************************
// 構築と破壊
//******************************************************************************

// コンストラクタ
CPropertyTempoDlg::CPropertyTempoDlg () : CDialog (CPropertyTempoDlg::IDD) {
	m_strTime = _T("");
	m_strTempoBPM = _T("120.00");
}

//******************************************************************************
// オペレーション
//******************************************************************************

void CPropertyTempoDlg::DDV_MinMaxString (CDataExchange* pDX, 
	LPCTSTR lpszValue, double dMin, double dMax) {
	ASSERT (dMin <= dMax);
	double dValue = _tstof (lpszValue);
	DDV_MinMaxDouble (pDX, dValue, dMin, dMax);
}

//******************************************************************************
// オーバーライド
//******************************************************************************

// データエクスチェンジ
void CPropertyTempoDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_PROPERTYTEMPO_TIME, m_strTime);
	DDX_Text (pDX, IDC_PROPERTYTEMPO_TEMPOBPM, m_strTempoBPM);
	DDV_MinMaxString (pDX, m_strTempoBPM, 4.00, 60000.00);
}

// ダイアログ初期化
BOOL CPropertyTempoDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	return bRet;
}


//******************************************************************************
// メッセージマップ
//******************************************************************************

BEGIN_MESSAGE_MAP (CPropertyTempoDlg, CDialog)
END_MESSAGE_MAP ()


