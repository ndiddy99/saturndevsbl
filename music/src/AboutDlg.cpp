//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ヴァージョン情報ダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include "resource.h"
#include "AboutDlg.h"

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CAboutDlg::CAboutDlg () : CDialog (CAboutDlg::IDD) {

}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CAboutDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CAboutDlg, CDialog)
END_MESSAGE_MAP ()

