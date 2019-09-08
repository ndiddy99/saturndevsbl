//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 譜面オプションページクラス
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
#include "MusicalScoreOptionPage.h"

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
CMusicalScoreOptionPage::CMusicalScoreOptionPage () : CPropertyPage (CMusicalScoreOptionPage::IDD) {
	m_lDefTrackZoom = 0;
	m_lDefTimeZoom = 0;
	m_bSpeakerModeVisibleTrack = FALSE;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CMusicalScoreOptionPage::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_MUSICALSCOREOPTION_DEFTRACKZOOM, m_lDefTrackZoom);
	DDV_MinMaxInt (pDX, m_lDefTrackZoom, 1, 8);
	DDX_Text (pDX, IDC_MUSICALSCOREOPTION_DEFTIMEZOOM, m_lDefTimeZoom);
	DDV_MinMaxInt (pDX, m_lDefTimeZoom, 1, 16);
	DDX_Check (pDX, IDC_MUSICALSCOREOPTION_ENABLETRACKZOOMKEY, m_bEnableTrackZoomKey);
	DDX_Check (pDX, IDC_MUSICALSCOREOPTION_ENABLETIMEZOOMKEY, m_bEnableTimeZoomKey);
	DDX_Radio (pDX, IDC_MUSICALSCOREOPTION_SPEAKERMODEALLTRACK, m_bSpeakerModeVisibleTrack);

}

// ダイアログの初期化
BOOL CMusicalScoreOptionPage::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	((CSpinButtonCtrl*)GetDlgItem (IDC_MUSICALSCOREOPTION_DEFTRACKZOOMSP))->SetRange (1, 8);
	((CSpinButtonCtrl*)GetDlgItem (IDC_MUSICALSCOREOPTION_DEFTIMEZOOMSP))->SetRange (1, 16);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
//	SendMessageToDescendants 
//		(WM_SETFONT, (WPARAM)(HFONT)(pSekaijuApp->m_theDefaultFont), MAKELPARAM(TRUE, 0));
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CMusicalScoreOptionPage, CPropertyPage)
END_MESSAGE_MAP ()


