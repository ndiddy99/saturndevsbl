//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// プロパティ拍子記号ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "PropertyTimeSignatureDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//******************************************************************************
// 構築と破壊
//******************************************************************************

// コンストラクタ
CPropertyTimeSignatureDlg::CPropertyTimeSignatureDlg () : CDialog (CPropertyTimeSignatureDlg::IDD) {
	m_strTime = _T("");
	m_lNN = 4;
	m_nDDIndex = 2;
	m_lCC = 24;
	m_lBB = 8;
}

//******************************************************************************
// オペレーション
//******************************************************************************

// 分子可変範囲設定
BOOL CPropertyTimeSignatureDlg::SetNNRange () {
	CString strValue;
	GetDlgItem (IDC_PROPERTYTIMESIGNATURE_NN)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYTIMESIGNATURE_NNSP))->SetRange (1, 255);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYTIMESIGNATURE_NNSP))->SetPos (CLIP (1, lValue, 255));
	return TRUE;
}

// 分母コンボボックス充満(1,2,4,8,16,32とする)
BOOL CPropertyTimeSignatureDlg::FillDDIndexCombo () {
	long i = 0;
	for (i = 0; i <= 5; i++) {
		CString strItem;
		strItem.Format (_T("%d"), 1 << i);
		((CComboBox*)GetDlgItem (IDC_PROPERTYTIMESIGNATURE_DDINDEX))->AddString (strItem);
	}
	return TRUE;
}

// 4分音符当たりのクロック数可変範囲設定
BOOL CPropertyTimeSignatureDlg::SetCCRange () {
	CString strValue;
	GetDlgItem (IDC_PROPERTYTIMESIGNATURE_CC)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYTIMESIGNATURE_CCSP))->SetRange (1, 255);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYTIMESIGNATURE_CCSP))->SetPos (CLIP (1, lValue, 255));
	return TRUE;
}

// 4分音符当たりの32分音符の数可変範囲設定
BOOL CPropertyTimeSignatureDlg::SetBBRange () {
	CString strValue;
	GetDlgItem (IDC_PROPERTYTIMESIGNATURE_BB)->GetWindowText (strValue);
	long lValue = _ttol (strValue);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYTIMESIGNATURE_BBSP))->SetRange (1, 255);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PROPERTYTIMESIGNATURE_BBSP))->SetPos (CLIP (1, lValue, 255));
	return TRUE;
}


//******************************************************************************
// オーバーライド
//******************************************************************************

// データエクスチェンジ
void CPropertyTimeSignatureDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_CBIndex (pDX, IDC_PROPERTYTIMESIGNATURE_DDINDEX, m_nDDIndex);
	DDX_Text (pDX, IDC_PROPERTYTIMESIGNATURE_TIME, m_strTime);
	DDX_Text (pDX, IDC_PROPERTYTIMESIGNATURE_NN, m_lNN);
	DDV_MinMaxLong (pDX, m_lNN, 1, 255);
	DDX_Text (pDX, IDC_PROPERTYTIMESIGNATURE_CC, m_lCC);
	DDV_MinMaxLong (pDX, m_lCC, 1, 255);
	DDX_Text (pDX, IDC_PROPERTYTIMESIGNATURE_BB, m_lBB);
	DDV_MinMaxLong (pDX, m_lBB, 1, 255);
}

// ダイアログ初期化
BOOL CPropertyTimeSignatureDlg::OnInitDialog () {
	FillDDIndexCombo ();
	BOOL bRet = CDialog::OnInitDialog ();
	SetNNRange ();
	SetCCRange ();
	SetBBRange ();
	return bRet;
}


//******************************************************************************
// メッセージマップ
//******************************************************************************

BEGIN_MESSAGE_MAP (CPropertyTimeSignatureDlg, CDialog)
END_MESSAGE_MAP ()


