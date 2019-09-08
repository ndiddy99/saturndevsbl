//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// プロパティマーカーダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "resource.h"
#include "PropertyMarkerDlg.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//******************************************************************************
// 構築と破壊
//******************************************************************************

// コンストラクタ
CPropertyMarkerDlg::CPropertyMarkerDlg () : CDialog (CPropertyMarkerDlg::IDD) {
	m_strTime = _T("");
	m_strText = _T("");
}

//******************************************************************************
// オペレーション
//******************************************************************************


//******************************************************************************
// オーバーライド
//******************************************************************************

// データエクスチェンジ
void CPropertyMarkerDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_PROPERTYMARKER_TIME, m_strTime);
	DDX_Text (pDX, IDC_PROPERTYMARKER_TEXT, m_strText);
}

// ダイアログ初期化
BOOL CPropertyMarkerDlg::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	return bRet;
}


//******************************************************************************
// メッセージマップ
//******************************************************************************

BEGIN_MESSAGE_MAP (CPropertyMarkerDlg, CDialog)
END_MESSAGE_MAP ()


