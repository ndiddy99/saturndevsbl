//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロールオプションページクラス
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
#include "PianoRollOptionPage.h"

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
CPianoRollOptionPage::CPianoRollOptionPage () : CPropertyPage (CPianoRollOptionPage::IDD) {
	m_lDefKeyZoom = 0;
	m_lDefVelZoom = 0;
	m_lDefTimeZoom = 0;
	m_bSpeakerModeVisibleTrack = FALSE;
	m_lGraphLineWidth = 0;
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// データエクスチェンジ
void CPianoRollOptionPage::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_PIANOROLLOPTION_DEFKEYZOOM, m_lDefKeyZoom);
	DDV_MinMaxInt (pDX, m_lDefKeyZoom, 4, 16);
	DDX_Text (pDX, IDC_PIANOROLLOPTION_DEFVELZOOM, m_lDefVelZoom);
	DDV_MinMaxInt (pDX, m_lDefVelZoom, 1, 4);
	DDX_Text (pDX, IDC_PIANOROLLOPTION_DEFTIMEZOOM, m_lDefTimeZoom);
	DDV_MinMaxInt (pDX, m_lDefTimeZoom, 1, 16);
	DDX_Check (pDX, IDC_PIANOROLLOPTION_ENABLEKEYZOOMKEY, m_bEnableKeyZoomKey);
	DDX_Check (pDX, IDC_PIANOROLLOPTION_ENABLEVELZOOMKEY, m_bEnableVelZoomKey);
	DDX_Check (pDX, IDC_PIANOROLLOPTION_ENABLETIMEZOOMKEY, m_bEnableTimeZoomKey);
	DDX_Radio (pDX, IDC_PIANOROLLOPTION_SPEAKERMODEALLTRACK, m_bSpeakerModeVisibleTrack);
	DDX_Text (pDX, IDC_PIANOROLLOPTION_GRAPHLINEWIDTH, m_lGraphLineWidth);
	DDV_MinMaxInt (pDX, m_lGraphLineWidth, 1, 4);
}

// ダイアログの初期化
BOOL CPianoRollOptionPage::OnInitDialog () {
	BOOL bRet = CDialog::OnInitDialog ();
	((CSpinButtonCtrl*)GetDlgItem (IDC_PIANOROLLOPTION_DEFKEYZOOMSP))->SetRange (4, 16);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PIANOROLLOPTION_DEFVELZOOMSP))->SetRange (1, 4);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PIANOROLLOPTION_DEFTIMEZOOMSP))->SetRange (1, 16);
	((CSpinButtonCtrl*)GetDlgItem (IDC_PIANOROLLOPTION_GRAPHLINEWIDTHSP))->SetRange (1, 4);
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
//	SendMessageToDescendants 
//		(WM_SETFONT, (WPARAM)(HFONT)(pSekaijuApp->m_theDefaultFont), MAKELPARAM(TRUE, 0));
	return bRet;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP (CPianoRollOptionPage, CPropertyPage)
END_MESSAGE_MAP ()


