//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// トラックリストオプション2ページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxcmn.h>
#include <afxext.h>

#include "resource.h"
#include "TrackListOption2Page.h"

#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CTrackListOption2Page::CTrackListOption2Page () : CPropertyPage (CTrackListOption2Page::IDD) {
	m_lDefCC000Width = 0;
	m_lDefCC032Width = 0;
	m_lDefPCWidth = 0;
	m_lDefCC007Width = 0;
	m_lDefCC010Width = 0;
	m_lDefCC091Width = 0;
	m_lDefCC093Width = 0;
	m_lDefCC094Width = 0;
	m_lDefKeyShiftWidth = 0;
	m_lDefVelShiftWidth = 0;
	m_lDefTimeShiftWidth = 0;
	m_lDefNumEventWidth = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CTrackListOption2Page::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFCC000WIDTH, m_lDefCC000Width);
	DDV_MinMaxInt (pDX, m_lDefCC000Width, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFCC032WIDTH, m_lDefCC032Width);
	DDV_MinMaxInt (pDX, m_lDefCC032Width, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFPCWIDTH, m_lDefPCWidth);
	DDV_MinMaxInt (pDX, m_lDefPCWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFCC007WIDTH, m_lDefCC007Width);
	DDV_MinMaxInt (pDX, m_lDefCC007Width, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFCC010WIDTH, m_lDefCC010Width);
	DDV_MinMaxInt (pDX, m_lDefCC010Width, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFCC091WIDTH, m_lDefCC091Width);
	DDV_MinMaxInt (pDX, m_lDefCC091Width, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFCC093WIDTH, m_lDefCC093Width);
	DDV_MinMaxInt (pDX, m_lDefCC093Width, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFCC094WIDTH, m_lDefCC094Width);
	DDV_MinMaxInt (pDX, m_lDefCC094Width, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFKEYSHIFTWIDTH, m_lDefKeyShiftWidth);
	DDV_MinMaxInt (pDX, m_lDefKeyShiftWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFVELSHIFTWIDTH, m_lDefVelShiftWidth);
	DDV_MinMaxInt (pDX, m_lDefVelShiftWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFTIMESHIFTWIDTH, m_lDefTimeShiftWidth);
	DDV_MinMaxInt (pDX, m_lDefTimeShiftWidth, 1, 32);
	DDX_Text (pDX, IDC_TRACKLISTOPTION2_DEFNUMEVENTWIDTH, m_lDefNumEventWidth);
	DDV_MinMaxInt (pDX, m_lDefNumEventWidth, 1, 32);
	DDX_Check (pDX, IDC_TRACKLISTOPTION2_ENABLEROWZOOMKEY, m_bEnableRowZoomKey);
	DDX_Check (pDX, IDC_TRACKLISTOPTION2_ENABLECOLUMNZOOMKEY, m_bEnableColumnZoomKey);
	DDX_Check (pDX, IDC_TRACKLISTOPTION2_ENABLETIMEZOOMKEY, m_bEnableTimeZoomKey);

}

// ダイアログの初期化
BOOL CTrackListOption2Page::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION2_DEFCC000WIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION2_DEFCC032WIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION2_DEFPCWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION2_DEFCC007WIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION2_DEFCC010WIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION2_DEFCC091WIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION2_DEFCC093WIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION2_DEFCC094WIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION2_DEFKEYSHIFTWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION2_DEFVELSHIFTWIDTHSP))->SetRange (1, 32);
	((CSpinButtonCtrl*)GetDlgItem (IDC_TRACKLISTOPTION2_DEFTIMESHIFTWIDTHSP))->SetRange (1, 32);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
//	SendMessageToDescendants 
//		(WM_SETFONT, (WPARAM)(HFONT)(pSekaijuApp->m_theDefaultFont), MAKELPARAM(TRUE, 0));
	return bRet;
}



//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (CTrackListOption2Page, CPropertyPage)
END_MESSAGE_MAP ()



