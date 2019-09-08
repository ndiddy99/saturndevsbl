//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// オプションプロパティシートクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxcmn.h>
#include <afxpriv.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "SekaijuApp.h"
#include "OptionSheet.h"

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
COptionSheet::COptionSheet (CWnd* pParentWnd) :
CPropertySheet (IDS_OPTIONS, pParentWnd) {
	AddPage (&m_theGeneralOptionPage);
	AddPage (&m_theColorOptionPage);
	AddPage (&m_theFontOptionPage);
	AddPage (&m_theTrackListOption1Page);
	AddPage (&m_theTrackListOption2Page);
	AddPage (&m_thePianoRollOptionPage);
	AddPage (&m_theEventListOptionPage);
	AddPage (&m_theMusicalScoreOptionPage);
}

// デストラクタ
COptionSheet::~COptionSheet () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 初期化時
BOOL COptionSheet::OnInitDialog() {
	BOOL bResult = CPropertySheet::OnInitDialog();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	SetActivePage (pSekaijuApp->m_theCurrentPage.m_lOption);
	CTabCtrl* pTabCtrl = GetTabControl ();
	ASSERT (pTabCtrl);
//	pTabCtrl->SetFont (&(pSekaijuApp->m_theDefaultFont));
//	pTabCtrl->SendMessageToDescendants 
//		(WM_SETFONT, (WPARAM)(HFONT)(pSekaijuApp->m_theDefaultFont), MAKELPARAM(TRUE, 0));
	return bResult;
}

// コマンド時
BOOL COptionSheet::OnCommand (WPARAM wParam, LPARAM lParam) {
	// 『OK』又は『キャンセル』ボタンが押された
	if (LOWORD (wParam) == IDOK || LOWORD (wParam) == IDCANCEL) {
		CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
		pSekaijuApp->m_theCurrentPage.m_lOption = GetActiveIndex ();
	}
	return CPropertySheet::OnCommand(wParam, lParam);
}

// DoModal時
int COptionSheet::DoModal () {
	// オーバーライドされたコールバック関数を使用するように指示。
	m_psh.dwFlags |= PSH_USECALLBACK;
	m_psh.pfnCallback = PropSheetProc;
	return CPropertySheet::DoModal ();
}

// 各ページの構築時
void COptionSheet::BuildPropPageArray () {
	CPropertySheet::BuildPropPageArray();
	// リソースで指定したプロパティページのフォントフェイスとフォントサイズは
	// 無視されるので、ここで強制指定する。
	CString strFontName;
	strFontName.LoadString (IDS_DEFAULTFONTNAME);
	LPCPROPSHEETPAGE ppsp = m_psh.ppsp;
	for (int nPage = 0; nPage < m_pages.GetSize (); nPage++) {
		const DLGTEMPLATE* pResource = ppsp->pResource;
		CDialogTemplate dlgTemplate (pResource);
		dlgTemplate.SetFont (strFontName, 9);
		memmove ((void*)pResource, dlgTemplate.m_hTemplate, dlgTemplate.m_dwTemplateSize);
		(BYTE*&)ppsp += ppsp->dwSize;
	}
}

// コールバック関数
int CALLBACK COptionSheet::PropSheetProc (HWND hwndDlg, UINT uMsg, LPARAM lParam) {
	switch (uMsg) {
	case PSCB_PRECREATE:
		{
		// リソースで指定したプロパティシートのフォントフェイスとフォントサイズは
		// 無視されるので、ここで強制指定する。
			CString strFontName;
			strFontName.LoadString (IDS_DEFAULTFONTNAME);
			LPDLGTEMPLATE pResource = (LPDLGTEMPLATE)lParam;
			CDialogTemplate dlgTemplate (pResource);
			dlgTemplate.SetFont (strFontName, 9);
			memmove ((void*)lParam, dlgTemplate.m_hTemplate, dlgTemplate.m_dwTemplateSize);
			break;
		}
	default:
		break;
	}
	return 0;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP (COptionSheet, CPropertySheet)
	ON_BN_CLICKED (ID_APPLY_NOW, OnApplyNow)
END_MESSAGE_MAP ()


// 『適用』ボタンが押された
void COptionSheet::OnApplyNow () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	GetActivePage ()->UpdateData (TRUE);
	pSekaijuApp->ApplyOptionSheet (this);
	m_theGeneralOptionPage.SetModified (FALSE);
	m_theColorOptionPage.SetModified (FALSE);
	m_theFontOptionPage.SetModified (FALSE);
	m_theTrackListOption1Page.SetModified (FALSE);
	m_theTrackListOption2Page.SetModified (FALSE);
	m_thePianoRollOptionPage.SetModified (FALSE);
	m_theEventListOptionPage.SetModified (FALSE);
	m_theMusicalScoreOptionPage.SetModified (FALSE);
}

