//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MIDIデバイスプロパティシートクラス
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
#include "MIDIInDevicePage.h"
#include "MIDIOutDevicePage.h"
#include "MIDIInstDefNormPage.h"
#include "MIDIInstDefDrumPage.h"
#include "MIDIThruPage.h"
#include "MIDIDeviceSheet.h"

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMIDIDeviceSheet::CMIDIDeviceSheet (CWnd* pParentWnd)
	:CPropertySheet (IDS_MIDIDEVICE_AND_INSTRUMENT, pParentWnd) {
	AddPage (&m_theMIDIInDevicePage);
	AddPage (&m_theMIDIOutDevicePage);
	AddPage (&m_theMIDIInstDefNormPage);
	AddPage (&m_theMIDIInstDefDrumPage);
	AddPage (&m_theMIDIThruPage);
}

// デストラクタ
CMIDIDeviceSheet::~CMIDIDeviceSheet () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// 初期化時
BOOL CMIDIDeviceSheet::OnInitDialog() {
	BOOL bResult = CPropertySheet::OnInitDialog();
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	SetActivePage (pSekaijuApp->m_theCurrentPage.m_lMIDIDevice);
	CTabCtrl* pTabCtrl = GetTabControl ();
	ASSERT (pTabCtrl);
//	pTabCtrl->SetFont (&(pSekaijuApp->m_theDefaultFont));
//	pTabCtrl->SendMessageToDescendants 
//		(WM_SETFONT, (WPARAM)(HFONT)(pSekaijuApp->m_theDefaultFont), MAKELPARAM(TRUE, 0));
	return bResult;
}

// コマンド時
BOOL CMIDIDeviceSheet::OnCommand (WPARAM wParam, LPARAM lParam) {
	// 『OK』又は『キャンセル』ボタンが押された
	if (LOWORD (wParam) == IDOK || LOWORD (wParam) == IDCANCEL) {
		CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
		pSekaijuApp->m_theCurrentPage.m_lMIDIDevice = GetActiveIndex ();
	}
	return CPropertySheet::OnCommand(wParam, lParam);
}

// DoModal時
int CMIDIDeviceSheet::DoModal () {
	// オーバーライドされたコールバック関数を使用するように指示。
	m_psh.dwFlags |= PSH_USECALLBACK;
	m_psh.pfnCallback = PropSheetProc;
	return CPropertySheet::DoModal ();
}

// 各ページの構築時
void CMIDIDeviceSheet::BuildPropPageArray () {
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
int CALLBACK CMIDIDeviceSheet::PropSheetProc (HWND hwndDlg, UINT uMsg, LPARAM lParam) {
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

BEGIN_MESSAGE_MAP (CMIDIDeviceSheet, CPropertySheet)
	ON_BN_CLICKED (ID_APPLY_NOW, OnApplyNow)
END_MESSAGE_MAP ()

// 『適用』ボタンが押された
void CMIDIDeviceSheet::OnApplyNow () {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	GetActivePage ()->UpdateData (TRUE);
	pSekaijuApp->ApplyMIDIDeviceSheet (this);
	m_theMIDIInDevicePage.SetModified (FALSE);
	m_theMIDIOutDevicePage.SetModified (FALSE);
	m_theMIDIInstDefNormPage.SetModified (FALSE);
	m_theMIDIInstDefDrumPage.SetModified (FALSE);
	m_theMIDIThruPage.SetModified (FALSE);
}

