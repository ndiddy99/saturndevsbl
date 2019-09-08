//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ビート検出とテンポ自動挿入ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "EditBeatScanDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

// コンストラクタ
CEditBeatScanDlg::CEditBeatScanDlg () : CDialog (CEditBeatScanDlg::IDD) {
	m_lTimeMode = 0;
	m_lTimeResolution = 0;
	m_nBeatTrackIndex = 0;
	m_nBeatIntervalIndex = 0;
	m_nInsertTempo = 0;
}

// オペレーション

// ビートトラックコンボボックスの充満
BOOL CEditBeatScanDlg::FillBeatTrackCombo () {
	CComboBox* pBeatTrackCombo = (CComboBox*)GetDlgItem (IDC_EDITBEATSCAN_BEATTRACK);
	long lCount = m_theTrackNameArray.GetSize ();
	long i;
	CString strTextLine;
	for (i = 0; i < lCount; i++) {
		strTextLine.Format (_T("%d-%s"), i + 1, m_theTrackNameArray.GetAt (i));
		pBeatTrackCombo->AddString (strTextLine);
	}
	return TRUE;
}

// ビート間隔コンボボックスの充満
BOOL CEditBeatScanDlg::FillBeatIntervalCombo () {
	CComboBox* pBeatIntervalCombo = (CComboBox*)GetDlgItem (IDC_EDITBEATSCAN_BEATINTERVAL);
	CString strTextLine;
	CString strFormat;
	if (m_lTimeMode == 0) {
		VERIFY (strFormat.LoadString (IDS_D_4DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 1);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_8DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 2);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_12DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 3);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_16DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 4);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_24DIVNOTE));
		strTextLine.Format (strFormat,  m_lTimeResolution / 6);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_32DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 8);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_48DIVNOTE));
		strTextLine.Format (strFormat, m_lTimeResolution / 12);
		pBeatIntervalCombo->AddString (strTextLine);
	}
	else {
		VERIFY (strFormat.LoadString (IDS_D_1FRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 1);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_2DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 2);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_3DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 3);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_4DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 4);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_6DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 6);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_8DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 8);
		pBeatIntervalCombo->AddString (strTextLine);
		VERIFY (strFormat.LoadString (IDS_D_12DIVFRAME));
		strTextLine.Format (strFormat, m_lTimeResolution / 12);
		pBeatIntervalCombo->AddString (strTextLine);
	}
	return TRUE;
}

// オーバーライド

// ダイアログデータエクスチェンジ
void CEditBeatScanDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_CBIndex (pDX, IDC_EDITBEATSCAN_BEATTRACK, m_nBeatTrackIndex);
	DDX_CBIndex (pDX, IDC_EDITBEATSCAN_BEATINTERVAL, m_nBeatIntervalIndex);
	DDX_Check (pDX, IDC_EDITBEATSCAN_INSERTTEMPO, m_nInsertTempo);
}

// ダイアログ初期化
BOOL CEditBeatScanDlg::OnInitDialog () {
	FillBeatTrackCombo ();
	FillBeatIntervalCombo ();
	BOOL bRet = CDialog::OnInitDialog ();
	return bRet;
}



// メッセージマップ
BEGIN_MESSAGE_MAP (CEditBeatScanDlg, CDialog)
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
// CEditBeatScanDlg メッセージ ハンドラ



