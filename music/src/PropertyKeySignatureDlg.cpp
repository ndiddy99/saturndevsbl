//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// プロパティ調性記号ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "PropertyKeySignatureDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//******************************************************************************
// 構築と破壊
//******************************************************************************

// コンストラクタ
CPropertyKeySignatureDlg::CPropertyKeySignatureDlg () : CDialog (CPropertyKeySignatureDlg::IDD) {
	m_strTime = _T("");
	m_nSFIndex = 0;
	m_nMiIndex = 0;
}

//******************************************************************************
// オペレーション
//******************************************************************************

// #又はbの数コンボボックス充満
BOOL CPropertyKeySignatureDlg::FillSFIndexCombo () {
	long i = 0;
	CString strSign;
	VERIFY (strSign.LoadString (IDS_FLAT));
	for (i = 7; i > 0; i--) {
		CString strItem;
		strItem.Format (_T("%d%s"), i, strSign);
		((CComboBox*)GetDlgItem (IDC_PROPERTYKEYSIGNATURE_SFINDEX))->AddString (strItem);
	}
	VERIFY (strSign.LoadString (IDS_SHARP));
	for (i = 0; i <= 7; i++) {
		CString strItem;
		strItem.Format (_T("%d%s"), i, strSign);
		((CComboBox*)GetDlgItem (IDC_PROPERTYKEYSIGNATURE_SFINDEX))->AddString (strItem);
	}
	return TRUE;
}

// 長調or短調コンボボックス充満
BOOL CPropertyKeySignatureDlg::FillMiIndexCombo () {
	CString strSign;
	VERIFY (strSign.LoadString (IDS_MAJOR));
	((CComboBox*)GetDlgItem (IDC_PROPERTYKEYSIGNATURE_MIINDEX))->AddString (strSign);
	VERIFY (strSign.LoadString (IDS_MINOR));
	((CComboBox*)GetDlgItem (IDC_PROPERTYKEYSIGNATURE_MIINDEX))->AddString (strSign);
	return TRUE;
}

//******************************************************************************
// オーバーライド
//******************************************************************************

// データエクスチェンジ
void CPropertyKeySignatureDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_PROPERTYKEYSIGNATURE_TIME, m_strTime);
	DDX_CBIndex (pDX, IDC_PROPERTYKEYSIGNATURE_SFINDEX, m_nSFIndex);
	DDX_CBIndex (pDX, IDC_PROPERTYKEYSIGNATURE_MIINDEX, m_nMiIndex);
}

// ダイアログ初期化
BOOL CPropertyKeySignatureDlg::OnInitDialog () {
	FillSFIndexCombo ();
	FillMiIndexCombo ();
	BOOL bRet = CDialog::OnInitDialog ();
	return bRet;
}


//******************************************************************************
// メッセージマップ
//******************************************************************************

BEGIN_MESSAGE_MAP (CPropertyKeySignatureDlg, CDialog)
END_MESSAGE_MAP ()


