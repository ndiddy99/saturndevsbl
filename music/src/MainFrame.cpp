//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MDI親フレームウィンドウクラス
// (C)2002-2018 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxpriv.h>
#include <afxext.h>
#include <afxcmn.h>
#include <afxmt.h>
//#include <afximpl.h>

#define _AfxSetDlgCtrlID(hWnd, nID)     SetWindowLong(hWnd, GWL_ID, nID)
#define _AfxGetDlgCtrlID(hWnd)          ((UINT)::GetDlgCtrlID(hWnd))

#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "mousewheel.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuToolBar.h"
#include "SekaijuStatusBar.h"
#include "MainFrame.h"
#include "ChildFrame.h"
#include "SekaijuDoc.h"
#include "SekaijuPreviewView.h"
#include "SAL.h"

#ifndef TBSTYLE_ALTDRAG
#define TBSTYLE_ALTDRAG 0x0400
#endif
#ifndef TBSTYLE_FLAT
#define TBSTYLE_FLAT    0x0800
#endif
#ifndef TBSTYLE_LIST
#define TBSTYLE_LIST    0x1000
#endif


// デバッグ用
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC (CMainFrame, CMDIFrameWnd)

// メッセージマップ
BEGIN_MESSAGE_MAP (CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE ()
	ON_WM_DESTROY ()
	ON_WM_CLOSE ()
	ON_WM_TIMER ()
	ON_WM_HSCROLL ()
	ON_WM_MOUSEWHEEL40 ()
	ON_WM_SIZE ()
	ON_COMMAND (ID_VIEW_TOOLBAR1, OnViewToolbar1)
	ON_UPDATE_COMMAND_UI (ID_VIEW_TOOLBAR1, OnUpdateViewToolbar1UI)
	ON_COMMAND (ID_VIEW_TOOLBAR2, OnViewToolbar2)
	ON_UPDATE_COMMAND_UI (ID_VIEW_TOOLBAR2, OnUpdateViewToolbar2UI)
	ON_UPDATE_COMMAND_UI (ID_INDICATOR_INPUTVELOCITY, OnUpdateIndicatorInputVelocityUI)
	ON_UPDATE_COMMAND_UI (ID_INDICATOR_OUTPUTVELOCITY, OnUpdateIndicatorOutputVelocityUI)
	ON_UPDATE_COMMAND_UI (ID_INDICATOR_FORMAT, OnUpdateIndicatorFormatUI)
	ON_UPDATE_COMMAND_UI (ID_INDICATOR_NUMTRACKS, OnUpdateIndicatorNumTracksUI)
	ON_UPDATE_COMMAND_UI (ID_INDICATOR_TIMEBASE, OnUpdateIndicatorTimeBaseUI)
	
	ON_MESSAGE (WM_COMMANDWAKEUP, OnCommandWakeUp)
	ON_MESSAGE (WM_COMMANDREADSHM, OnCommandReadShm)
	ON_MESSAGE (WM_COMMANDFILEOPEN, OnCommandFileOpen)
	ON_MESSAGE (WM_COMMANDUPDATEALLVIEWS, OnCommandUpdateAllViews)


	ON_MESSAGE (WM_SALPAUSE, OnSALPause)
	ON_MESSAGE (WM_SALMESSAGE, OnSALMessage)
	ON_MESSAGE (WM_SALGETINT, OnSALGetInt)
	ON_MESSAGE (WM_SALGETSTRING, OnSALGetString)
	ON_MESSAGE (WM_SALGETTIME, OnSALGetTime)
	ON_MESSAGE (WM_SALSENDMIDI, OnSALSendMIDI)
	ON_MESSAGE (WM_SALINSERT, OnSALInsert)
	ON_MESSAGE (WM_SALDELETE, OnSALDelete)
	ON_MESSAGE (WM_SALGETVALUE, OnSALGetValue)
	ON_MESSAGE (WM_SALSETVALUE, OnSALSetValue)
	ON_MESSAGE (WM_SALGETFOREACHEVENTARRAY, OnSALGetForEachEventArray)
	ON_MESSAGE (WM_SALEND, OnSALEnd)

	ON_WM_DRAWITEM ()
END_MESSAGE_MAP ()

// ステータス ライン インジケータ
static UINT indicators[] = {
	ID_SEPARATOR,
	ID_SEPARATOR, // ID_INDICATOR_INPUTVELOCITY
	ID_SEPARATOR, // ID_INDICATOR_OUTPUTVELOCITY
	ID_SEPARATOR, // ID_INDICATOR_FORMAT
	ID_SEPARATOR, // ID_INDICATOR_NUMTRACKS
	ID_SEPARATOR, // ID_INDICATOR_TIMEBASE
	ID_INDICATOR_KANA,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CMainFrame::CMainFrame () {
	m_strWndClass = AfxGetAppName ();
	m_strTempFileName = _T("");
}

// デストラクタ
CMainFrame::~CMainFrame () {
}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// 再生位置スクロールバー(ツールバー2上)の範囲設定
void CMainFrame::SetPositionScrollRange (long lStartTime, long lEndTime, BOOL bRedraw) {
	ASSERT (m_wndPositionScroll.GetSafeHwnd ());
	m_wndPositionScroll.SetScrollRange (lStartTime, lEndTime, bRedraw);
}

// SetWindowTextのテキストが異なる場合のみ更新するヴァージョン
int CMainFrame::SetWindowTextWhenDifferent (CWnd* pWnd, LPCTSTR lpszText) {
	ASSERT (pWnd);
	CString strCurText;
	pWnd->GetWindowText (strCurText);
	if (_tcscmp (strCurText, lpszText) != 0) {
		pWnd->SetWindowText (lpszText);
	}
	return 1;
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

// ウィンドウ作成直前
BOOL CMainFrame::PreCreateWindow (CREATESTRUCT& cs) {
	// 注意：このPreCreateWindowを有効に働かせるためには、
	// CMainFrameをLoadFrameではなくCreate関数で作成すること。

	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();

	// デフォルトのMDIフレーム用WNDCLASSを作成し、登録する。
	BOOL bRet = CMDIFrameWnd::PreCreateWindow (cs);
	// cs.lpszClassが"AfxMDIFrame40(d)"になる.

	// メインウィンドウのクラス名をm_strWndClassにする。
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	// m_strWndClassがまだ登録されていない場合
	if (!::GetClassInfo(hInst, m_strWndClass, &wndcls)) {
		// デフォルトのWNDCLASSを取得
		bRet = ::GetClassInfo(hInst, cs.lpszClass, &wndcls);
		if (bRet) {
			// デフォルトのWNDCLASSを固定クラス名に改造して登録
			wndcls.lpszClassName = m_strWndClass;
			wndcls.hIcon = ::LoadIcon (hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
			if (::RegisterClass(&wndcls)) {
				cs.lpszClass = m_strWndClass;
			}
			else {
				bRet = FALSE;
			}
		}
	}

	// csの各メンバ変数を更新する。
	//cs.hInstance;
	//cs.hMenu;
	//cs.hwndParent;
	// 起動時に前回のウィンドウ位置を復元する場合
	if (pSekaijuApp->m_theGeneralOption.m_bRestoreWindowPlacement) {
		// 最小化
		if (pSekaijuApp->m_theWindowPlacement.m_bIconic ||
			pSekaijuApp->m_nCmdShow == SW_SHOWMINIMIZED) {
			cs.cy = CW_USEDEFAULT;
			cs.cx = CW_USEDEFAULT;
			cs.y = CW_USEDEFAULT;
			cs.x = CW_USEDEFAULT;
			cs.style |= WS_MINIMIZE;
		}
		// 最大化
		else if (pSekaijuApp->m_theWindowPlacement.m_bZoomed ||
			pSekaijuApp->m_nCmdShow == SW_SHOWMAXIMIZED) {
			cs.cy = CW_USEDEFAULT;
			cs.cx = CW_USEDEFAULT;
			cs.y = CW_USEDEFAULT;
			cs.x = CW_USEDEFAULT;
			cs.style |= WS_MAXIMIZE;
		}
		// その他
		else {
			cs.cy = pSekaijuApp->m_theWindowPlacement.m_nHeight;
			cs.cx = pSekaijuApp->m_theWindowPlacement.m_nWidth;
			cs.y = pSekaijuApp->m_theWindowPlacement.m_nY;
			cs.x = pSekaijuApp->m_theWindowPlacement.m_nX;
		}
	}
	// デフォルトのウィンドウ位置を用いる場合
	else {
		cs.cy = CW_USEDEFAULT;
		cs.cx = CW_USEDEFAULT;
		cs.y = CW_USEDEFAULT;
		cs.x = CW_USEDEFAULT;
	}
	//cs.lpszName;
	//cs.lpszClass;
	//cs.dwExStyle;

	// 注意：最小化と最大化は、ShowWindow時のm_nCmdShowの
	// 値によって最終的に決まるので、ShowWindowの直前で
	// m_nCmdShowの値も変更しておかなければならない。

	// return直後のCreateWindowExにcsの各メンバ変数が渡される。

	return bRet;
}

// 現在アクティブな子フレームウィンドウを取得(なければthis)
CFrameWnd* CMainFrame::GetActiveFrame () {
	// check first for MDI client window not created
	if (m_hWndMDIClient == NULL) {
		return this;
	}
	// MDI client has been created, get active MDI child
	HWND hWnd = (HWND)::SendMessage(m_hWndMDIClient, WM_MDIGETACTIVE, 0, NULL);
	if (!::IsWindow (hWnd)) {
		return this;
	}
	CMDIChildWnd* pWnd = (CMDIChildWnd*)CWnd::FromHandle(hWnd);
	if (pWnd == NULL) {
		return this;
	}
	if (!pWnd->IsKindOf (RUNTIME_CLASS (CMDIChildWnd))) {
		return this;
	}

	// check for special pseudo-inactive state
	if (pWnd != NULL && pWnd->m_bPseudoInactive &&
		(pWnd->GetStyle () & WS_VISIBLE) == 0) {
		// Window is hidden, active, but m_bPseudoInactive -- return NULL
		return this;
	}
	return pWnd;
}


//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// ウィンドウ作成時
int CMainFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CRect rcTemp;
	int i;
	
	if (CMDIFrameWnd::OnCreate (lpCreateStruct) == -1) {
		return -1;
	}

	// 画面のdpi取得
	HDC hDC = ::GetDC (this->GetSafeHwnd ());
	long lLogPixelsY = ::GetDeviceCaps (hDC, LOGPIXELSY);
	::ReleaseDC (this->GetSafeHwnd (), hDC);
	// デフォルトフォントの生成
	long lFontHeight = CLIP (6, 12 * lLogPixelsY / 72, 72);
	CString strFontName;
	VERIFY (strFontName.LoadString (IDS_DEFAULTFONTNAME));
	pSekaijuApp->m_theDefaultFont.CreateFont 
		(-lFontHeight, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,  
		strFontName);

	// ツールバー1の作成(MFC9.0,Window7スタイル対応)
	if (!m_wndToolBar1.CreateEx (this, 0,
		WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		CRect (4, 4, 4, 4)) ||
		!m_wndToolBar1.LoadToolBar (IDR_MAINFRAME1)) {
		TRACE0 ("Failed to create toolbar\n");
		return -1;
	}
	m_wndToolBar1.EnableDocking (CBRS_ALIGN_ANY);
	EnableDocking (CBRS_ALIGN_ANY);
	DockControlBar (&m_wndToolBar1);
	m_wndToolBar1.ModifyStyle (0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_CUSTOMERASE);

	// ツールバー2の作成(MFC9.0,Window7スタイル対応)
	if (!m_wndToolBar2.CreateEx (this, 0,
		WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		CRect (4, 4, 4, 4)) ||
		!m_wndToolBar2.LoadToolBar (IDR_MAINFRAME2)) {
		TRACE0 ("Failed to create toolbar\n");
		return -1;
	}
	m_wndToolBar2.EnableDocking (CBRS_ALIGN_ANY);
	EnableDocking (CBRS_ALIGN_ANY);
	DockControlBar (&m_wndToolBar2);
	m_wndToolBar2.ModifyStyle (0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);

	// 時:分:秒:ミリ秒の作成
	m_wndToolBar2.SetButtonInfo (0, IDC_MILLISECEDIT, TBBS_SEPARATOR, 100);
	m_wndToolBar2.GetItemRect (0, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 20;
	if (!m_wndMillisecEdit.CreateEx (
		WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_CENTER | ES_READONLY,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar2.GetSafeHwnd (), (HMENU)IDC_MILLISECEDIT)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndMillisecEdit.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// 小節:拍:ティックの作成
	m_wndToolBar2.SetButtonInfo (2, IDC_TIMEEDIT, TBBS_SEPARATOR, 100);
	m_wndToolBar2.GetItemRect (2, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 20;
	if (!m_wndTimeEdit.CreateEx (
		WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_CENTER | ES_READONLY,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar2.GetSafeHwnd (), (HMENU)IDC_TIMEEDIT)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndTimeEdit.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// 再生位置スクロールバーの作成
	m_wndToolBar2.SetButtonInfo (9, IDC_POSITIONSCROLL, TBBS_SEPARATOR, 120);
	m_wndToolBar2.GetItemRect (9, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 20;
	if (!m_wndPositionScroll.CreateEx (
		0, _T("SCROLLBAR"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar2.GetSafeHwnd (), (HMENU)IDC_POSITIONSCROLL)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}

	// キーシフトの作成
	m_wndToolBar2.SetButtonInfo (19, IDC_KEYSHIFTCOMBO, TBBS_SEPARATOR, 60);
	m_wndToolBar2.GetItemRect (19, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 80;
	if (!m_wndKeyShiftCombo.CreateEx (
		WS_EX_CLIENTEDGE, _T("COMBOBOX"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | CBS_DROPDOWNLIST,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar2.GetSafeHwnd (), (HMENU)IDC_KEYSHIFTCOMBO)) {
		TRACE0 ("Failed to create combo box\n");
		return -1;
	}
	m_wndKeyShiftCombo.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));
	for (i = 12; i >= -12; i--) {
		CString str;
		if (i > 0) {
			str.Format (_T("+%d"), i);
		}
		else {
			str.Format (_T("%d"), i);
		}
		m_wndKeyShiftCombo.AddString (str);
	}
	m_wndKeyShiftCombo.SetCurSel (12 - pSekaijuApp->m_lKeyShift);

	// 拍子･調性の作成
	m_wndToolBar2.SetButtonInfo (21, IDC_MEASUREEDIT, TBBS_SEPARATOR, 60);
	m_wndToolBar2.GetItemRect (21, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 20;
	if (!m_wndMeasureEdit.CreateEx (
		WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_CENTER | ES_READONLY,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar2.GetSafeHwnd (), (HMENU)IDC_MEASUREEDIT)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndMeasureEdit.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// テンポ[BPM]の作成
	m_wndToolBar2.SetButtonInfo (23, IDC_TEMPOEDIT, TBBS_SEPARATOR, 60);
	m_wndToolBar2.GetItemRect (23, &rcTemp);
	rcTemp.top = 2;
	rcTemp.bottom = 20;
	if (!m_wndTempoEdit.CreateEx (
		WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_CENTER | ES_READONLY,
		rcTemp.left, rcTemp.top, rcTemp.Width (), rcTemp.Height (), 
		m_wndToolBar2.GetSafeHwnd (), (HMENU)IDC_TEMPOEDIT)) {
		TRACE0 ("Failed to create edit box\n");
		return -1;
	}
	m_wndTempoEdit.SetFont (CFont::FromHandle ((HFONT)::GetStockObject (DEFAULT_GUI_FONT)));

	// ステータスバーの作成
	if (!m_wndStatusBar.Create (this) ||
		!m_wndStatusBar.SetIndicators (indicators, sizeof(indicators)/sizeof(UINT))) {
		TRACE0 ("Failed to create status bar\n");
		return -1;
	}

	m_wndStatusBar.SetPaneInfo (0, 0, SBPS_STRETCH, 0);
	m_wndStatusBar.SetPaneInfo (1, ID_INDICATOR_FORMAT, 0, 64); 
	m_wndStatusBar.SetPaneInfo (2, ID_INDICATOR_NUMTRACKS, 0, 64); 
	m_wndStatusBar.SetPaneInfo (3, ID_INDICATOR_TIMEBASE, 0, 96); 
	m_wndStatusBar.SetPaneInfo (4, ID_INDICATOR_INPUTVELOCITY, 0, 156);
	m_wndStatusBar.SetPaneInfo (5, ID_INDICATOR_OUTPUTVELOCITY, 0, 156); 

	// ツールバーのテキスト更新用タイマー起動
	SetTimer (0x03, 55, NULL);

	// ステータスバーのゲージ更新用タイマー起動
	SetTimer (0x04, 55, NULL);

	// ファイルのドロップを許可
	DragAcceptFiles (TRUE);

	return TRUE;
}

// ウィンドウ破壊時
void CMainFrame::OnDestroy () {
	// ツールバーのテキスト更新用タイマー終了
	KillTimer (0x03);
	// ステータスバーのゲージ更新用タイマー起動
	KillTimer (0x04);
	// ウィンドウ位置の保持
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CRect rcWindow;
	GetWindowRect (&rcWindow);
	pSekaijuApp->m_theWindowPlacement.m_bIconic = this->IsIconic ();
	pSekaijuApp->m_theWindowPlacement.m_bZoomed = this->IsZoomed ();
	pSekaijuApp->m_theWindowPlacement.m_nX = rcWindow.left;
	pSekaijuApp->m_theWindowPlacement.m_nY = rcWindow.top;
	pSekaijuApp->m_theWindowPlacement.m_nWidth = rcWindow.Width ();
	pSekaijuApp->m_theWindowPlacement.m_nHeight = rcWindow.Height ();
	// デフォルトフォントの破棄
	pSekaijuApp->m_theDefaultFont.DeleteObject ();
	CMDIFrameWnd::OnDestroy ();
}


// メインウィンドウの左上×(閉じる)が押されたとき
void CMainFrame::OnClose() {
	// MFC4.0風(SDI)印刷プレビュー時
	if (m_lpfnCloseProc != NULL && !(*m_lpfnCloseProc)(this)) {
		return;
	}
	// 通常時
	CMDIFrameWnd::OnClose ();
}


// タイマー処理時
void CMainFrame::OnTimer (UINT nIDEvent) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp();
	// ツールバーのテキスト更新用タイマー(0x03)
	if (nIDEvent == 0x03) {
		CFrameWnd* pActiveFrame = this->GetActiveFrame ();
		if (pActiveFrame == NULL || pActiveFrame == this) {
			/* 現在の時：分：秒：ミリ秒表示 */
			SetWindowTextWhenDifferent (&m_wndTimeEdit, _T(""));
			/* 現在の小節：拍：ティック値表示 */
			SetWindowTextWhenDifferent (&m_wndMillisecEdit, _T(""));
			/* スクロールポジションの更新 */
			m_wndPositionScroll.SetScrollPos (0, TRUE);
			/* 現在の拍子及び調性記号の表示 */
			SetWindowTextWhenDifferent (&m_wndMeasureEdit, _T(""));
			/* 現在のテンポ[BPM]の表示 */
			SetWindowTextWhenDifferent (&m_wndTempoEdit, _T(""));
		}
		else {
			CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pActiveFrame->GetActiveDocument ());
			if (pSekaijuDoc) {
				TCHAR szText[1024];
				if (pSekaijuDoc->m_pMIDIData && pSekaijuDoc->m_pMIDIClock) {
					pSekaijuDoc->m_theCriticalSection.Lock ();
					pSekaijuApp->m_theCriticalSection.Lock ();
					long lCurTime, lMeasure, lBeat, lTick, lFrameNumber;
					long lCurMillisec, lHour, lMinute, lSec, lMillisec;
					long lCurTempo;
					long nn, dd, cc, bb;
					long sf, mi;
					lCurTime = MIDIClock_GetTickCount (pSekaijuDoc->m_pMIDIClock);
					lCurMillisec = MIDIClock_GetMillisec (pSekaijuDoc->m_pMIDIClock);
					/* 現在の時：分：秒：ミリ秒表示 */
					lHour = (lCurMillisec / 3600000);
					lMinute = (lCurMillisec / 60000) % 60;
					lSec = (lCurMillisec / 1000) % 60;
					lMillisec = lCurMillisec % 1000;
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%02d:%02d:%02d:%03d"), lHour, lMinute, lSec, lMillisec);
					SetWindowTextWhenDifferent (&m_wndMillisecEdit, szText);
					/* 現在の小節：拍：ティック値表示 */
					MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
					long lTimeMode, lTimeResolution;
					MIDIData_GetTimeBase (pMIDIData, &lTimeMode, &lTimeResolution);
					if (lTimeMode == MIDIDATA_TPQNBASE) {
						MIDIData_BreakTimeEx (pSekaijuDoc->m_pMIDIData, lCurTime, &lMeasure, &lBeat, &lTick,
							&nn, &dd, &cc, &bb);
						memset (szText, 0, sizeof (szText));
						_sntprintf (szText, 1023, _T("%05d:%02d:%03d"), lMeasure + 1, lBeat + 1, lTick);
						SetWindowTextWhenDifferent (&m_wndTimeEdit, szText);
					}
					else if (lTimeMode == MIDIDATA_SMPTE24BASE ||
						lTimeMode == MIDIDATA_SMPTE25BASE ||
						lTimeMode == MIDIDATA_SMPTE29BASE ||
						lTimeMode == MIDIDATA_SMPTE30BASE) {
						lFrameNumber = lCurTime / lTimeResolution;
						lTick = lCurTime % lTimeResolution;
						memset (szText, 0, sizeof (szText));
						_sntprintf (szText, 1023, _T("%08d:%03d"), lFrameNumber, lTick);
						SetWindowTextWhenDifferent (&m_wndTimeEdit, szText);
					}
					/* スクロールポジションの更新 */
					m_wndPositionScroll.SetScrollPos (lCurTime, TRUE);
					/* 現在の拍子記号及び調性記号の表示 */
					MIDIData_FindTimeSignature (pSekaijuDoc->m_pMIDIData, lCurTime, &nn, &dd, &cc, &bb);
					MIDIData_FindKeySignature (pSekaijuDoc->m_pMIDIData, lCurTime, &sf, &mi);
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%d/%d, %d%s"), nn, 1 << dd, abs(sf), 
						(sf > 0 ? _T("#") : (sf < 0 ? _T("b") : _T(" ")))); // 20100613修正
					SetWindowTextWhenDifferent (&m_wndMeasureEdit, szText);
					/* 現在のテンポ[BPM]の表示 */
					MIDIData_FindTempo (pSekaijuDoc->m_pMIDIData, lCurTime, &lCurTempo);
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%1.2lf"), (double)60000000 / (double)lCurTempo);
					SetWindowTextWhenDifferent (&m_wndTempoEdit, szText);
					pSekaijuApp->m_theCriticalSection.Unlock ();
					pSekaijuDoc->m_theCriticalSection.Unlock ();
				}
				else {
					/* 現在の時：分：秒：ミリ秒表示 */
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%05d:%02d:%03d"), 1, 1, 0);
					SetWindowTextWhenDifferent (&m_wndTimeEdit, szText);
					/* 現在の小節：拍：ティック値表示 */
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%02d:%02d:%02d:%03d"), 0, 0, 0, 0);
					SetWindowTextWhenDifferent (&m_wndMillisecEdit, szText);
					/* スクロールポジションの更新 */
					m_wndPositionScroll.SetScrollPos (0, TRUE);
					/* 現在の拍子記号及び調性記号の表示 */
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%d/%d, %d%s"), 4, 4, 0, _T(" "));
					SetWindowTextWhenDifferent (&m_wndMeasureEdit, szText);
					/* 現在のテンポ[BPM]の表示 */
					memset (szText, 0, sizeof (szText));
					_sntprintf (szText, 1023, _T("%1.2lf"), (double)60000000 / (double)600000);
					SetWindowTextWhenDifferent (&m_wndTempoEdit, szText);
				}
			}
			else {
				/* 現在の時：分：秒：ミリ秒表示 */
				SetWindowTextWhenDifferent (&m_wndTimeEdit, _T(""));
				/* 現在の小節：拍：ティック値表示 */
				SetWindowTextWhenDifferent (&m_wndMillisecEdit, _T(""));
				/* スクロールポジションの更新 */
				m_wndPositionScroll.SetScrollPos (0, TRUE);
				/* 現在の拍子記号及び調性記号の表示 */
				SetWindowTextWhenDifferent (&m_wndMeasureEdit, _T(""));
				/* 現在のテンポ[BPM]の表示 */
				SetWindowTextWhenDifferent (&m_wndTempoEdit, _T(""));
			}
		}
	}
	// ステータスバーのゲージ更新用タイマー(0x04)
	else if (nIDEvent == 0x04) {
		CRect rcItem;
		CRect rcLeft;
		CRect rcRight;
		CDC* pDC = NULL;
		long lPort;
		long lPart;
		long j = 0;
		// 入力ベロシティゲージの描画
		unsigned char ucMaxVelocity = 0;
		for (lPort = 0; lPort < MAXMIDIINDEVICENUM; lPort++) {
			MIDIStatus* pMIDIInStatus = pSekaijuApp->m_pMIDIInStatus[lPort];
			if (pMIDIInStatus) {
				for (lPart = 0; lPart < 16; lPart++) {
					MIDIPart* pMIDIPart = MIDIStatus_GetMIDIPart (pMIDIInStatus, lPart);
					unsigned char ucVelocity[128];
					memset (ucVelocity, 0, 128);
					MIDIPart_GetNoteEx (pMIDIPart, ucVelocity, 128);
					for (j = 0; j < 128; j++) {
						if (ucVelocity[j] > ucMaxVelocity) {
							ucMaxVelocity = ucVelocity[j];
						}
					}
				}
			}
		}
		m_wndStatusBar.GetItemRect (4, &rcItem);
		rcLeft.left = rcItem.left + 32, 
		rcLeft.top = rcItem.top + 2;
		rcLeft.right = rcItem.left + 32 + ucMaxVelocity;
		rcLeft.bottom = rcItem.bottom - 2;
		rcRight.left = rcItem.left + 32 + ucMaxVelocity;
		rcRight.top = rcItem.top + 2;
		rcRight.right = rcItem.right - 2;
		rcRight.bottom = rcItem.bottom - 2;
		pDC = m_wndStatusBar.GetDC ();
		pDC->FillSolidRect (rcLeft, RGB (255,0,0));
		pDC->FillSolidRect (rcRight, GetSysColor (COLOR_3DFACE));
		m_wndStatusBar.ReleaseDC (pDC);
		// 出力ベロシティゲージの描画
		ucMaxVelocity = 0;
		for (lPort = 0; lPort < MAXMIDIOUTDEVICENUM; lPort++) {
			MIDIStatus* pMIDIOutStatus = pSekaijuApp->m_pMIDIOutStatus[lPort];
			if (pMIDIOutStatus) {
				for (lPart = 0; lPart < 16; lPart++) {
					MIDIPart* pMIDIPart = MIDIStatus_GetMIDIPart (pMIDIOutStatus, lPart);
					unsigned char ucVelocity[128];
					memset (ucVelocity, 0, 128);
					MIDIPart_GetNoteEx (pMIDIPart, ucVelocity, 128);
					for (j = 0; j < 128; j++) {
						if (ucVelocity[j] > ucMaxVelocity) {
							ucMaxVelocity = ucVelocity[j];
						}
					}
				}
			}
		}
		m_wndStatusBar.GetItemRect (5, &rcItem);
		rcLeft.left = rcItem.left + 32, 
		rcLeft.top = rcItem.top + 2;
		rcLeft.right = rcItem.left + 32 + ucMaxVelocity;
		rcLeft.bottom = rcItem.bottom - 2;
		rcRight.left = rcItem.left + 32 + ucMaxVelocity;
		rcRight.top = rcItem.top + 2;
		rcRight.right = rcItem.right - 2;
		rcRight.bottom = rcItem.bottom - 2;
		pDC = m_wndStatusBar.GetDC ();
		pDC->FillSolidRect (rcLeft, RGB (0,128,0));
		pDC->FillSolidRect (rcRight, GetSysColor (COLOR_3DFACE));
		m_wndStatusBar.ReleaseDC (pDC);
	
	}
}

// ツールバー2上の位置スクロールバーの操作メッセージはここにはこない。
void CMainFrame::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	// ツールバー上のスクロールメッセージは、CSekaijuToolBar::OnHScrollで処理せよ。
}

// マウスホイールが回された時
void CMainFrame::OnMouseWheel40 (UINT nFlags, CPoint point) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	long lDelta = (short)HIWORD (nFlags);
	long lFlags = LOWORD (nFlags);
	if (lFlags & MK_CONTROL) {
		if (lDelta > 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel == 0 ||
			lDelta < 0 && pSekaijuApp->m_theGeneralOption.m_bInvertCtrlMouseWheel != 0) {
			this->PostMessage (WM_COMMAND, ID_CONTROL_PREVMEASURE);
		}
		else {
			this->PostMessage (WM_COMMAND, ID_CONTROL_NEXTMEASURE);
		}
	}
}

// メインウィンドウのサイズ左右拡大時にCDockBarの右端1ピクセルが正しく描画されないバグ回避
// メインウィンドウのサイズ上下拡大時にCDockBarの下端1ピクセルが正しく描画されないバグ回避
void CMainFrame::OnSize (UINT nType, int cx, int cy) {
	const DWORD dwDockBarMap[4] ={
		AFX_IDW_DOCKBAR_TOP, AFX_IDW_DOCKBAR_BOTTOM,
		AFX_IDW_DOCKBAR_LEFT, AFX_IDW_DOCKBAR_RIGHT};
	for (int i = 0; i < 4; i++) {
		CDockBar* pDockBar = (CDockBar*)GetControlBar(dwDockBarMap[i]);
		if (pDockBar != NULL) {
			CRect rcClient;
			pDockBar->GetClientRect (&rcClient);
			CRect rcInvalidate = rcClient;
			rcInvalidate.left = rcClient.right - 1;
			pDockBar->InvalidateRect (rcInvalidate);
			rcInvalidate = rcClient;
			rcInvalidate.top = rcClient.bottom - 1;
			pDockBar->InvalidateRect (rcInvalidate);
		}
	}
	CMDIFrameWnd::OnSize (nType, cx, cy);
}


// 『表示』-『ツールバー1』
void CMainFrame::OnViewToolbar1 () {
	ShowControlBar (&m_wndToolBar1, (m_wndToolBar1.GetStyle () & WS_VISIBLE) == 0, FALSE);
}

// 『表示』-『ツールバー1』
void CMainFrame::OnUpdateViewToolbar1UI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck ((m_wndToolBar1.GetStyle () & WS_VISIBLE) ? 1 : 0);
}

// 『表示』-『ツールバー2』
void CMainFrame::OnViewToolbar2 () {
	ShowControlBar (&m_wndToolBar2, (m_wndToolBar2.GetStyle () & WS_VISIBLE) == 0, FALSE);
}

// 『表示』-『ツールバー2』
void CMainFrame::OnUpdateViewToolbar2UI (CCmdUI* pCmdUI) {
	pCmdUI->SetCheck ((m_wndToolBar2.GetStyle () & WS_VISIBLE) ? 1 : 0);
}

// インジケーター-入力ベロシティ
void CMainFrame::OnUpdateIndicatorInputVelocityUI (CCmdUI* pCmdUI) {
	pCmdUI->SetText (_T("IN"));
}

// インジケーター-出力ベロシティ
void CMainFrame::OnUpdateIndicatorOutputVelocityUI (CCmdUI* pCmdUI) {
	pCmdUI->SetText (_T("OUT"));
}

// インジケーター-フォーマット
void CMainFrame::OnUpdateIndicatorFormatUI (CCmdUI* pCmdUI) {
	CFrameWnd* pFrameWnd = (CFrameWnd*)GetActiveFrame ();
	if (pFrameWnd) {
		CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pFrameWnd->GetActiveDocument ());
		if (pSekaijuDoc) {
			MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
			long lFormat = MIDIData_GetFormat (pMIDIData);
			CString strText;
			strText.Format (_T("Format%d"), lFormat);
			pCmdUI->SetText (strText);
		}
		else {
			pCmdUI->SetText (_T(""));
		}
	}
	else {
		pCmdUI->SetText (_T(""));
	}
}

// インジケーター-トラック数
void CMainFrame::OnUpdateIndicatorNumTracksUI (CCmdUI* pCmdUI) {
	CFrameWnd* pFrameWnd = (CFrameWnd*)GetActiveFrame ();
	if (pFrameWnd) {
		CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pFrameWnd->GetActiveDocument ());
		if (pSekaijuDoc) {
			MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
			long lNumTracks = MIDIData_GetNumTrack (pMIDIData);
			CString strText;
			strText.Format (_T("%dTracks"), lNumTracks);
			pCmdUI->SetText (strText);
		}
		else {
			pCmdUI->SetText (_T(""));
		}
	}
	else {
		pCmdUI->SetText (_T(""));
	}
}

// インジケーター-タイムベース
void CMainFrame::OnUpdateIndicatorTimeBaseUI (CCmdUI* pCmdUI) {
	CFrameWnd* pFrameWnd = (CFrameWnd*)GetActiveFrame ();
	if (pFrameWnd) {
		CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)(pFrameWnd->GetActiveDocument ());
		if (pSekaijuDoc) {
			MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
			long lTimeMode = MIDIData_GetTimeMode (pMIDIData);
			long lTimeResolution = MIDIData_GetTimeResolution (pMIDIData);
			CString strText;
			switch (lTimeMode) {
			case MIDIDATA_TPQNBASE:
				strText.Format (_T("%dTPQN"), lTimeResolution);
				break;
			case MIDIDATA_SMPTE24BASE:
				strText.Format (_T("SMPTE24/%d"), lTimeResolution);
				break;
			case MIDIDATA_SMPTE25BASE:
				strText.Format (_T("SMPTE24/%d"), lTimeResolution);
				break;
			case MIDIDATA_SMPTE29BASE:
				strText.Format (_T("SMPTE29.97/%d"), lTimeResolution);
				break;
			case MIDIDATA_SMPTE30BASE:
				strText.Format (_T("SMPTE30/%d"), lTimeResolution);
				break;
			}
			pCmdUI->SetText (strText);
		}
		else {
			pCmdUI->SetText (_T(""));
		}
	}
	else {
		pCmdUI->SetText (_T(""));
	}
}


// このウィンドウを最前面に出すことを要求
long CMainFrame::OnCommandWakeUp (WPARAM wParam, LPARAM lParam) {
	// メイン・ウィンドウが最小化されていれば元に戻す
	if (this->IsIconic ()) {
		::ShowWindowAsync (this->GetSafeHwnd (), SW_RESTORE);
	}
	// メイン・ウィンドウを最前面に表示する
	SetForegroundWindow ();
	return 1;
}

// 共有メモリが変更されたので読み取ることを要求
long CMainFrame::OnCommandReadShm (WPARAM wParam, LPARAM lParam) {
	CString strMsg;
	HANDLE hShare = CreateFileMapping
		(INVALID_HANDLE_VALUE, NULL, FILE_MAP_READ, 0, 1024, AfxGetAppName ());
	if (hShare == INVALID_HANDLE_VALUE) {
		// 共有メモリ(受信側)オープンエラー
		strMsg.LoadString (IDS_SHAREMEMORY_FOR_RECV_OPEN_ERROR);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return 0;
	}
	TCHAR *pShareMem = (TCHAR*)MapViewOfFile (hShare, FILE_MAP_READ, 0, 0, 1024);
	if (pShareMem == NULL) {
		// 共有メモリ(受信側)マッピングエラー
		::CloseHandle (hShare);
		strMsg.LoadString (IDS_SHAREMEMORY_FOR_RECV_MAPPING_ERROR);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);
		return 0;
	}
	m_strTempFileName = CString (pShareMem);
	::UnmapViewOfFile (pShareMem);
	::CloseHandle (hShare);
	return 1;
}

// ファイルを開くことを要求(開くファイル名はm_strTempFileName)
long CMainFrame::OnCommandFileOpen (WPARAM wParam, LPARAM lParam) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp();
	CDocTemplate* pSekaijuDocTemplate = pSekaijuApp->m_pSekaijuDocTemplate;
	CSekaijuDoc* pSekaijuDoc = 
		(CSekaijuDoc*)(pSekaijuDocTemplate->OpenDocumentFile (m_strTempFileName));
	if (pSekaijuDoc == NULL) {
		//MessageBox (m_strTempFileName, APPNAME, MB_ICONINFORMATION);
		_RPT1 (_CRT_WARN, "新しいドキュメントを開けませんでした。-%s.", m_strTempFileName);
	}
	return 1;
}

// 指定ドキュメントに対しUpdateAllViewsを要求
long CMainFrame::OnCommandUpdateAllViews (WPARAM wParam, LPARAM lParam) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)wParam;
	//TODO:ここでpSekaijuDocが存在する又はまだ生きていることをチェックしないと
	//次の行(EnterCriticalSection)でエラーとなる。
	if (pSekaijuApp->IsDocumentExist (pSekaijuDoc)) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		pSekaijuDoc->UpdateAllViews (NULL, 0xFFFFFFFF);
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		return 1;
	}
	return 0;
}

long CMainFrame::OnSALPause (WPARAM wParam, LPARAM lParam) {
	return 1;
}

long CMainFrame::OnSALMessage (WPARAM wParam, LPARAM lParam) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)wParam;
	CString strMessage ((TCHAR*)lParam);
	if (pSekaijuApp->IsDocumentExist (pSekaijuDoc)) {
		m_wndStatusBar.SetPaneText (0, strMessage, 1);
		return 1;
	}
	return 0;
}

long CMainFrame::OnSALGetInt (WPARAM wParam, LPARAM lParam) {
	return 1;
}

long CMainFrame::OnSALGetString (WPARAM wParam, LPARAM lParam) {
	return 1;
}

long CMainFrame::OnSALGetTime (WPARAM wParam, LPARAM lParam) {
	return 1;
}

long CMainFrame::OnSALSendMIDI (WPARAM wParam, LPARAM lParam) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)wParam;
	SALSendMIDI* pSALSendMIDI = (SALSendMIDI*)lParam;
	MIDIOut* pMIDIOut = NULL;
	MIDIStatus* pMIDIStatus = NULL;
	BYTE byMessage[1024];
	int nLen = 0;
	memset (byMessage, 0, sizeof (byMessage));
	pSekaijuApp->m_theCriticalSection.Lock ();
	if (0 <= pSALSendMIDI->m_nPort && pSALSendMIDI->m_nPort < MAXMIDIOUTDEVICENUM) {
		pMIDIOut = pSekaijuApp->m_pMIDIOut[pSALSendMIDI->m_nPort];
		pMIDIStatus = pSekaijuApp->m_pMIDIOutStatus[pSALSendMIDI->m_nPort];
	}
	// ノートオフ,ノートオン,キーアフタータッチ,コントロールチェンジ,チャンネルアフタータッチ,ピッチベンド
	if (pSALSendMIDI->m_nKind == 0x80 || 
		pSALSendMIDI->m_nKind == 0x90 ||
		pSALSendMIDI->m_nKind == 0xA0 ||
		pSALSendMIDI->m_nKind == 0xB0 ||
		pSALSendMIDI->m_nKind == 0xC0 ||
		pSALSendMIDI->m_nKind == 0xD0 ||
		pSALSendMIDI->m_nKind == 0xE0) {
		byMessage[0] = (pSALSendMIDI->m_nKind & 0xF0) | (pSALSendMIDI->m_nCh & 0x0F);
		byMessage[1] = BYTE (CLIP (0, pSALSendMIDI->m_nValue[0], 127));
		byMessage[2] = BYTE (CLIP (0, pSALSendMIDI->m_nValue[1], 127));
		nLen = (pSALSendMIDI->m_nKind == 0xA0 || pSALSendMIDI->m_nKind == 0xC0) ? 2 : 3;
	}
	// SYSXDATA
	else if (pSALSendMIDI->m_nKind == 0xF0) { 
		int j = 0;
		nLen = CLIP (0, pSALSendMIDI->m_nLen, 1024);
		for (j = 0; j < nLen; j++) {
			if (pSALSendMIDI->m_nValue[j] == 0xF0 || 
				pSALSendMIDI->m_nValue[j] == 0xF7) {
				byMessage[j] = (BYTE)(CLIP (0, pSALSendMIDI->m_nValue[j], 255));
			}
			else {
				byMessage[j] = (BYTE)(CLIP (0, pSALSendMIDI->m_nValue[j], 127));
			}
		}
	}
	// SYSX
	else if (pSALSendMIDI->m_nKind == 0x1F0) { 
		int j = 0;
		nLen = CLIP (0, pSALSendMIDI->m_nLen, 1023) + 1;
		byMessage[0] = 0xF0;
		for (j = 0; j < nLen - 1; j++) {
			if (pSALSendMIDI->m_nValue[j] == 0xF0 || 
				pSALSendMIDI->m_nValue[j] == 0xF7) {
				byMessage[j+1] = (BYTE)(CLIP (0, pSALSendMIDI->m_nValue[j], 255));
			}
			else {
				byMessage[j+1] = (BYTE)(CLIP (0, pSALSendMIDI->m_nValue[j], 127));
			}
		}
	}
	else {
		pSekaijuApp->m_theCriticalSection.Unlock ();
		return 0;
	}
	if (pMIDIOut != NULL) {
		MIDIOut_PutMIDIMessage (pMIDIOut, byMessage, nLen);
	}
	if (pMIDIStatus != NULL) {
		MIDIStatus_PutMIDIMessage (pMIDIStatus, byMessage, nLen);
	}
	pSekaijuApp->m_theCriticalSection.Unlock ();
	return 1;
}

long CMainFrame::OnSALInsert (WPARAM wParam, LPARAM lParam) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)wParam;
	SALInsert* pSALInsert = (SALInsert*)lParam;
	if (pSekaijuApp->IsDocumentExist (pSekaijuDoc)) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		int nRet = 1;
		CHistoryUnit* pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
		MIDIEvent* pCurEvent = (MIDIEvent*)(pSALInsert->m_pCurEvent);
		int nTime = pSALInsert->m_nTime;
		int nCh = pSALInsert->m_nCh;
		int nValue[1024];
		void* pValue = pSALInsert->m_szValue;
		int i = 0;
		for (i = 0; i < 1024; i++) {
			nValue[i] = pSALInsert->m_nValue[i];
		}
		MIDITrack* pMIDITrack = NULL;
		if (pCurEvent != NULL) {
			pMIDITrack = MIDIEvent_GetParent (pCurEvent);
		}
		else {
			long lTrackIndex = pSekaijuDoc->m_nSALCurTrack;
			pMIDITrack = pSekaijuDoc->GetTrack (lTrackIndex);
			if (pMIDITrack == NULL) {
				pSekaijuDoc->m_theCriticalSection.Unlock ();
				return 0;
			}
		}
		MIDIEvent* pMIDIEvent = NULL;
		if (pSALInsert->m_nKind == 0x00) {
			pMIDIEvent = MIDIEvent_CreateSequenceNumber (nTime, nValue[0]);
		}
		else if (pSALInsert->m_nKind == 0x01) {
			pMIDIEvent = MIDIEvent_CreateTextEvent (nTime, (TCHAR*)pValue);
		}
		else if (pSALInsert->m_nKind == 0x02) {
			pMIDIEvent = MIDIEvent_CreateCopyrightNotice (nTime, (TCHAR*)pValue);
		}
		else if (pSALInsert->m_nKind == 0x03) {
			pMIDIEvent = MIDIEvent_CreateTrackName (nTime, (TCHAR*)pValue);
		}
		else if (pSALInsert->m_nKind == 0x04) {
			pMIDIEvent = MIDIEvent_CreateInstrumentName (nTime, (TCHAR*)pValue);
		}
		else if (pSALInsert->m_nKind == 0x05) {
			pMIDIEvent = MIDIEvent_CreateLyric (nTime, (TCHAR*)pValue);
		}
		else if (pSALInsert->m_nKind == 0x06) {
			pMIDIEvent = MIDIEvent_CreateMarker (nTime, (TCHAR*)pValue);
		}
		else if (pSALInsert->m_nKind == 0x07) {
			pMIDIEvent = MIDIEvent_CreateCuePoint (nTime, (TCHAR*)pValue);
		}
		else if (pSALInsert->m_nKind == 0x08) {
			pMIDIEvent = MIDIEvent_CreateProgramName (nTime, (TCHAR*)pValue);
		}
		else if (pSALInsert->m_nKind == 0x09) {
			pMIDIEvent = MIDIEvent_CreateDeviceName (nTime, (TCHAR*)pValue);
		}
		else if (pSALInsert->m_nKind == 0x20) {
			pMIDIEvent = MIDIEvent_CreateChannelPrefix (nTime, nValue[0]);
		}
		else if (pSALInsert->m_nKind == 0x21) {
			pMIDIEvent = MIDIEvent_CreatePortPrefix (nTime, nValue[0]);
		}
		else if (pSALInsert->m_nKind == 0x2F) {
			pMIDIEvent = MIDIEvent_CreateEndofTrack (nTime);
		}
		else if (pSALInsert->m_nKind == 0x51) {
			pMIDIEvent = MIDIEvent_CreateTempo (nTime, (int)((double)6000000000 / (double)nValue[0]));
		}
		else if (pSALInsert->m_nKind == 0x54) {
			pMIDIEvent = MIDIEvent_CreateSMPTEOffset (nTime, nValue[0], nValue[1], nValue[2], nValue[3], nValue[4], nValue[5]);
		}
		else if (pSALInsert->m_nKind == 0x58) {
			pMIDIEvent = MIDIEvent_CreateTimeSignature (nTime, nValue[0], nValue[1], nValue[2], nValue[3]);
		}
		else if (pSALInsert->m_nKind == 0x59) {
			pMIDIEvent = MIDIEvent_CreateKeySignature (nTime, nValue[0], nValue[1]);
		}
		else if (pSALInsert->m_nKind == 0x7F) {
			pMIDIEvent = MIDIEvent_CreateSequencerSpecific (nTime, (char*)pValue, nValue[0]);
		}
		else if  (pSALInsert->m_nKind == 0x80) {
			pMIDIEvent = MIDIEvent_CreateNoteOnNoteOff (nTime, nCh, nValue[0], nValue[1], nValue[2], nValue[3]);
		}
		else if  (pSALInsert->m_nKind == 0x90) {
			pMIDIEvent = MIDIEvent_CreateNote (nTime, nCh, nValue[0], nValue[1], nValue[2]);
		}
		else if (pSALInsert->m_nKind == 0xA0) {
			pMIDIEvent = MIDIEvent_CreateKeyAftertouch (nTime, nCh, nValue[0], nValue[1]);
		}
		else if (pSALInsert->m_nKind == 0xB0) {
			pMIDIEvent = MIDIEvent_CreateControlChange (nTime, nCh, nValue[0], nValue[1]);
		}
		else if (pSALInsert->m_nKind == 0xC0) {
			nValue[0] = CLIP (0, nValue[0], 127);
			if (nValue[1] == -1) {
				pMIDIEvent = MIDIEvent_CreateProgramChange (nTime, nCh, nValue[0]);
			}
			else {
				nValue[1] = CLIP (0, nValue[1], 16383);
				pMIDIEvent = MIDIEvent_CreatePatchChange (nTime, nCh, nValue[1] / 128, nValue[1] % 128, nValue[0]);
			}
		}
		else if (pSALInsert->m_nKind == 0xD0) {
			pMIDIEvent = MIDIEvent_CreateChannelAftertouch (nTime, nCh, nValue[0]);
		}
		else if (pSALInsert->m_nKind == 0xE0) {
			pMIDIEvent = MIDIEvent_CreatePitchBend (nTime, nCh, nValue[0] + 8192);
		}
		else if (pSALInsert->m_nKind == 0xF0) {
			int nLen = CLIP (0, nValue[0], 1024);
			pMIDIEvent = MIDIEvent_CreateSysExEvent (nTime, (BYTE*)pValue, nLen);
		}
		else if (pSALInsert->m_nKind == 0x1F0) {
			int nLen = CLIP (0, nValue[0], 1023);
			memmove ((BYTE*)(((BYTE*)pValue) + 1), (BYTE*)pValue, nLen);
			*((BYTE*)pValue) = 0xF0;
			pMIDIEvent = MIDIEvent_CreateSysExEvent (nTime, (BYTE*)pValue, nLen + 1);
		}
		if (pMIDIEvent != NULL) {
			nRet = MIDITrack_InsertEvent (pMIDITrack, pMIDIEvent);
			if (nRet >= 1) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pMIDIEvent));
			}
			else {
				VERIFY (MIDIEvent_Delete (pMIDIEvent));
			}
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		return nRet;
	}
	return 0;
}

long CMainFrame::OnSALDelete (WPARAM wParam, LPARAM lParam) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)wParam;
	MIDIEvent* pMIDIEvent = (MIDIEvent*)(lParam);
	int nRet = 0;
	if (pSekaijuApp->IsDocumentExist (pSekaijuDoc)) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		CHistoryUnit* pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
		if (pMIDIEvent != NULL) {
			MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
			if (!(MIDIEvent_IsEndofTrack (pMIDIEvent) && pMIDIEvent->m_pNextEvent == NULL)) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pMIDIEvent));
				VERIFY (nRet = MIDITrack_RemoveEvent (pMIDITrack, pMIDIEvent));
			}
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		return nRet;
	}
	return 0;
}

long CMainFrame::OnSALGetValue (WPARAM wParam, LPARAM lParam) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)wParam;
	SALGetValue* pSALGetValue = (SALGetValue*)lParam;
	MIDIEvent* pMIDIEvent = (MIDIEvent*)(pSALGetValue->m_pTarget);
	if (pSekaijuApp->IsDocumentExist (pSekaijuDoc)) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		int nRet = 1;
		if (_tcscmp (pSALGetValue->m_szVarName, _T("TIMEBASE")) == 0) {
			pSALGetValue->m_nValue = MIDIData_GetTimeResolution (pSekaijuDoc->m_pMIDIData);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("(meas")) == 0 ||
			_tcscmp (pSALGetValue->m_szVarName, _T("(beat")) == 0 ||
			_tcscmp (pSALGetValue->m_szVarName, _T("(tick")) == 0) {
			VERIFY (MIDIData_BreakTime (pSekaijuDoc->m_pMIDIData, 
				pSALGetValue->m_nParam1, (long*)&(pSALGetValue->m_nParam2),
				(long*)&(pSALGetValue->m_nParam3), (long*)&(pSALGetValue->m_nParam4)));
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("(makeTime")) == 0) {
			VERIFY (MIDIData_MakeTime (pSekaijuDoc->m_pMIDIData,
				pSALGetValue->m_nParam1, pSALGetValue->m_nParam2,
				pSALGetValue->m_nParam3, (long*)&(pSALGetValue->m_nParam4)));
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Event.Track")) == 0) {
			MIDIEvent* pMIDIEvent = (MIDIEvent*)(pSALGetValue->m_pTarget);
			ASSERT (pMIDIEvent != NULL);
			MIDITrack* pMIDITrack = MIDIEvent_GetParent (pMIDIEvent);
			ASSERT (pMIDITrack != NULL);
			pSALGetValue->m_nValue = pSekaijuDoc->GetTrackIndex (pMIDITrack);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Event.Kind")) == 0) {
			// パッチチェンジ(CC#0+CC#32+PC)は特別扱い。
			if (MIDIEvent_IsPatchChange (pMIDIEvent)) {
				pSALGetValue->m_nValue = 0xC0;
			}
			// 通常のイベント
			else {
				int nTemp = MIDIEvent_GetKind (pMIDIEvent);
				if (0x90 <= nTemp && nTemp <= 0xEF) {
					nTemp &= 0xF0;
				}
				pSALGetValue->m_nValue = nTemp;
			}
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Event.Chan")) == 0 && 
			MIDIEvent_IsMIDIEvent (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetChannel (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Event.Time")) == 0) {
			pSALGetValue->m_nValue = MIDIEvent_GetTime (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("SeqNum.Val")) == 0 &&
			MIDIEvent_IsSequenceNumber (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetNumber (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Event.Text")) == 0 &&
			0x01 <= MIDIEvent_GetKind (pMIDIEvent) &&
			MIDIEvent_GetKind (pMIDIEvent) <= 0x1F) {
			int nLen = MIDIEvent_GetLen (pMIDIEvent);
			nLen = CLIP (0, nLen, 1024);
			memset (pSALGetValue->m_szValue, 0, sizeof (TCHAR)* 1024);
			MIDIEvent_GetText (pMIDIEvent, pSALGetValue->m_szValue, 1023);
			pSALGetValue->m_nLen = _tcslen (pSALGetValue->m_szValue);
			pSALGetValue->m_nValue = _tcslen (pSALGetValue->m_szValue);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("SeqNum.Val")) == 0 &&
			MIDIEvent_IsChannelPrefix (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetValue (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("ChanPrefix.Val")) == 0 &&
			MIDIEvent_IsChannelPrefix (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetValue (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("PortPrefix.Val")) == 0 &&
			MIDIEvent_IsPortPrefix (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetValue (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Tempo.Val")) == 0 &&
			MIDIEvent_IsTempo (pMIDIEvent)) {
			long lTempo = MIDIEvent_GetTempo (pMIDIEvent);
			pSALGetValue->m_nValue = (int)((double)6000000000 / (double)lTempo);
		}
		else if ((_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.Mode")) == 0 ||
			_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.Hour")) == 0 ||
			_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.Min")) == 0 ||
			_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.Sec")) == 0 ||
			_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.Frame")) == 0 ||
			_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.SubFr")) == 0) &&
			MIDIEvent_IsSMPTEOffset (pMIDIEvent)) {
			long lData[6];
			MIDIEvent_GetSMPTEOffset (pMIDIEvent, 
				&lData[0], &lData[1], &lData[2], &lData[3], &lData[4], &lData[5]);
			if (_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.Mode")) == 0) {
				pSALGetValue->m_nValue = lData[0];
			}
			else if (_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.Hour")) == 0) {
				pSALGetValue->m_nValue = lData[1];
			}
			else if (_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.Min")) == 0) {
				pSALGetValue->m_nValue = lData[2];
			}
			else if (_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.Sec")) == 0) {
				pSALGetValue->m_nValue = lData[3];
			}
			else if (_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.Frame")) == 0) {
				pSALGetValue->m_nValue = lData[4];
			}
			else if (_tcscmp (pSALGetValue->m_szVarName, _T("SmpteOffset.SubFr")) == 0) {
				pSALGetValue->m_nValue = lData[5];
			}
		}
		else if ((_tcscmp (pSALGetValue->m_szVarName, _T("TimeSig.Num")) == 0 ||
			_tcscmp (pSALGetValue->m_szVarName, _T("TimeSig.Den")) == 0 ||
			_tcscmp (pSALGetValue->m_szVarName, _T("TimeSig.Clo")) == 0 ||
			_tcscmp (pSALGetValue->m_szVarName, _T("TimeSig.Blo")) == 0) &&
			MIDIEvent_IsTimeSignature (pMIDIEvent)) {
			long lData[4];
			MIDIEvent_GetTimeSignature (pMIDIEvent, 
				&lData[0], &lData[1], &lData[2], &lData[3]);
			if (_tcscmp (pSALGetValue->m_szVarName, _T("TimeSig.Num")) == 0) {
				pSALGetValue->m_nValue = lData[0];
			}
			else if (_tcscmp (pSALGetValue->m_szVarName, _T("TimeSig.Den")) == 0) {
				pSALGetValue->m_nValue = (1 << lData[1]);
			}
			else if (_tcscmp (pSALGetValue->m_szVarName, _T("TimeSig.Clo")) == 0) {
				pSALGetValue->m_nValue = lData[2];
			}
			else if (_tcscmp (pSALGetValue->m_szVarName, _T("TimeSig.Blo")) == 0) {
				pSALGetValue->m_nValue = lData[3];
			}
		}
		else if ((_tcscmp (pSALGetValue->m_szVarName, _T("KeySig.Sf")) == 0 ||
			_tcscmp (pSALGetValue->m_szVarName, _T("KeySig.Mi")) == 0) &&
			MIDIEvent_IsKeySignature (pMIDIEvent)) {
			long lData[2];
			MIDIEvent_GetKeySignature (pMIDIEvent, &lData[0], &lData[1]);
			if (_tcscmp (pSALGetValue->m_szVarName, _T("KeySig.Sf")) == 0) {
				pSALGetValue->m_nValue = lData[0];
			}
			else if (_tcscmp (pSALGetValue->m_szVarName, _T("KeySig.Mi")) == 0) {
				pSALGetValue->m_nValue = lData[1];
			}
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("SeqSpec.Data")) == 0 &&
			MIDIEvent_IsSequencerSpecific (pMIDIEvent)) {
			int nLen = MIDIEvent_GetLen (pMIDIEvent);
			nLen = CLIP (0, nLen, 1024);
			pSALGetValue->m_nLen = nLen;
			MIDIEvent_GetData (pMIDIEvent, (BYTE*)(pSALGetValue->m_szValue), nLen);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Note.Key")) == 0 && 
			MIDIEvent_IsNote (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetKey (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Note.Vel")) == 0 && 
			MIDIEvent_IsNote (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetVelocity (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Note.Dur")) == 0 && 
			MIDIEvent_IsNote (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetDuration (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("KeyAft.Key")) == 0 && 
			MIDIEvent_IsKeyAftertouch (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetKey (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("KeyAft.Val")) == 0 && 
			MIDIEvent_IsKeyAftertouch (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetValue (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Control.Num")) == 0 && 
			MIDIEvent_IsControlChange (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetNumber (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Control.Val")) == 0 && 
			MIDIEvent_IsControlChange (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetValue (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Patch.Num")) == 0) {
			if (MIDIEvent_IsPatchChange (pMIDIEvent)) {
				pSALGetValue->m_nValue = MIDIEvent_GetPatchNum (pMIDIEvent);
			}
			else if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
				pSALGetValue->m_nValue = MIDIEvent_GetNumber (pMIDIEvent);
			}
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Patch.Bank")) == 0) {
			if (MIDIEvent_IsPatchChange (pMIDIEvent)) {
				pSALGetValue->m_nValue = MIDIEvent_GetBank ((MIDIEvent*)(pSALGetValue->m_pTarget));
			}
			else if (MIDIEvent_IsProgramChange (pMIDIEvent)) {
				pSALGetValue->m_nValue = -1;
			}
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("ChanAft.Val")) == 0 && 
			MIDIEvent_IsChannelAftertouch (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetValue (pMIDIEvent);
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Wheel.Val")) == 0 && 
			MIDIEvent_IsPitchBend (pMIDIEvent)) {
			pSALGetValue->m_nValue = MIDIEvent_GetValue (pMIDIEvent) - 8192;
		}
		else if (_tcscmp (pSALGetValue->m_szVarName, _T("Sysx.Data")) == 0 &&
			MIDIEvent_IsSysExEvent (pMIDIEvent)) {
			int nLen = MIDIEvent_GetLen (pMIDIEvent);
			nLen = CLIP (0, nLen, 1024);
			pSALGetValue->m_nLen = nLen;
			MIDIEvent_GetData (pMIDIEvent, (BYTE*)(pSALGetValue->m_szValue), nLen);
		}
		else {
			_RPTF1 (_CRT_WARN, "%s:定義されていない変数の取得を要求されました。", pSALGetValue->m_szVarName); 
			nRet = 0;
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		return nRet;
	}
	return 0;
}

long CMainFrame::OnSALSetValue (WPARAM wParam, LPARAM lParam) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)wParam;
	SALSetValue* pSALSetValue = (SALSetValue*)lParam;
	SAL_AddToForEachEventArray pfnSAL_AddToForEachEventArray = 
		(SAL_AddToForEachEventArray)pSekaijuApp->m_pfnSAL_AddToForEachEventArray;
	if (pSekaijuApp->IsDocumentExist (pSekaijuDoc)) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		int nRet = 1;
		MIDIEvent* pCurSALEvent = (MIDIEvent*)(pSALSetValue->m_pTarget);
		int nValue = pSALSetValue->m_nValue;
		int nLen = pSALSetValue->m_nLen;
		MIDIEvent* pCloneEvent = NULL;
		CHistoryUnit* pCurHistoryUnit = pSekaijuDoc->GetCurHistoryUnit ();
		if (_tcscmp (pSALSetValue->m_szVarName, _T("Event.Track")) == 0) {
			// 移動可能なイベントの場合のみ
			MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
			long lFormat = MIDIData_GetFormat (pMIDIData);
			if (lFormat == 1 && !MIDIEvent_IsMIDIEvent (pCurSALEvent) && nValue == 0 ||
				lFormat == 1 && (MIDIEvent_GetKind (pCurSALEvent) & 0xF0) != 0x50 && nValue != 0 ||
				lFormat == 2) {
				MIDITrack* pSrcTrack = MIDIEvent_GetParent (pCurSALEvent);
				MIDITrack* pDestTrack = pSekaijuDoc->GetTrack (nValue);
				// 転送元と転送先のトラックが存在する場合のみ
				if (pSrcTrack != NULL && pDestTrack != NULL) {
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
					VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
					VERIFY (MIDITrack_RemoveEvent (pSrcTrack, pCloneEvent));
					VERIFY (MIDITrack_InsertEvent (pDestTrack, pCloneEvent));
					VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
					pSALSetValue->m_pTarget = pCloneEvent;
				}
				else {
					nRet = 0;
				}
			}
			else {
				nRet = 0;
			}
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Event.Time")) == 0) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetTime (pCloneEvent, CLIP (0, nValue, 0x7FFFFFFF)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Event.Chan")) == 0 && 
			MIDIEvent_IsMIDIEvent (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetChannel (pCloneEvent, CLIP (0, nValue, 65535)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("SeqNum.Val")) == 0 && 
			MIDIEvent_IsSequenceNumber (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, CLIP (0, nValue, 65535)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Event.Text")) == 0 && 
			0x01 <= MIDIEvent_GetKind (pCurSALEvent) && MIDIEvent_GetKind (pCurSALEvent) < 0x1F) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetText (pCloneEvent, (TCHAR*)(pSALSetValue->m_szValue)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("ChanPrefix.Val")) == 0 && 
			MIDIEvent_IsChannelPrefix (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, CLIP (0, nValue, 15)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("PortPrefix.Val")) == 0 && 
			MIDIEvent_IsPortPrefix (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, CLIP (0, nValue, 255)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Tempo.Val")) == 0 && 
			MIDIEvent_IsTempo (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			int nBPM100 = CLIP (400, nValue, 6000000);
			VERIFY (MIDIEvent_SetTempo (pCloneEvent, (long)((double)(6000000000) / (double)nBPM100)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if ((_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.Mode")) == 0 ||
			_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.Hour")) == 0 ||
			_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.Min")) == 0 ||
			_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.Sec")) == 0 ||
			_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.Frame")) == 0 ||
			_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.SubFr")) == 0) &&
			MIDIEvent_IsSMPTEOffset (pCurSALEvent)) {
			long lData[6];
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_GetSMPTEOffset (pCloneEvent, 
				&lData[0], &lData[1], &lData[2], &lData[3], &lData[4], &lData[5]));
			if (_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.Mode")) == 0) {
				VERIFY (MIDIEvent_SetSMPTEOffset (pCloneEvent, 
					CLIP (0, pSALSetValue->m_nValue, 3), lData[1], lData[2], lData[3], lData[4], lData[5]));
			}
			else if (_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.Hour")) == 0) {
				VERIFY (MIDIEvent_SetSMPTEOffset (pCloneEvent, 
					lData[0], CLIP (0, pSALSetValue->m_nValue, 23), lData[2], lData[3], lData[4], lData[5]));
			}
			else if (_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.Min")) == 0) {
				VERIFY (MIDIEvent_SetSMPTEOffset (pCloneEvent, 
					lData[0], lData[1], CLIP (0, pSALSetValue->m_nValue, 59), lData[3], lData[4], lData[5]));
			}
			else if (_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.Sec")) == 0) {
				VERIFY (MIDIEvent_SetSMPTEOffset (pCloneEvent, 
					lData[0], lData[1], lData[2], CLIP (0, pSALSetValue->m_nValue, 59), lData[4], lData[5]));
			}
			else if (_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.Frame")) == 0) {
				VERIFY (MIDIEvent_SetSMPTEOffset (pCloneEvent, 
					lData[0], lData[1], lData[2], lData[3], CLIP (0, pSALSetValue->m_nValue, 29), lData[5]));
			}
			else if (_tcscmp (pSALSetValue->m_szVarName, _T("SmpteOffset.SubFr")) == 0) {
				VERIFY (MIDIEvent_SetSMPTEOffset (pCloneEvent, 
					lData[0], lData[1], lData[2], lData[3], lData[4], CLIP (0, pSALSetValue->m_nValue, 99)));
			}
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if ((_tcscmp (pSALSetValue->m_szVarName, _T("TimeSig.Num")) == 0 ||
			_tcscmp (pSALSetValue->m_szVarName, _T("TimeSig.Den")) == 0 ||
			_tcscmp (pSALSetValue->m_szVarName, _T("TimeSig.Clo")) == 0 ||
			_tcscmp (pSALSetValue->m_szVarName, _T("TimeSig.Blo")) == 0) &&
			MIDIEvent_IsTimeSignature (pCurSALEvent)) {
			long lData[4];
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_GetTimeSignature (pCloneEvent, 
				&lData[0], &lData[1], &lData[2], &lData[3]));
			if (_tcscmp (pSALSetValue->m_szVarName, _T("TimeSig.Num")) == 0) {
				VERIFY (MIDIEvent_SetTimeSignature (pCloneEvent, 
					CLIP (1, pSALSetValue->m_nValue, 255), lData[1], lData[2], lData[3]));
			}
			else if (_tcscmp (pSALSetValue->m_szVarName, _T("TimeSig.Den")) == 0) {
				int nDD = 0;
				if (pSALSetValue->m_nValue < 1) {
					nDD = 0;
				}
				else if (pSALSetValue->m_nValue < 2) {
					nDD = 1;
				}
				else if (pSALSetValue->m_nValue < 4) {
					nDD = 2;
				}
				else if (pSALSetValue->m_nValue < 16) {
					nDD = 3;
				}
				else if (pSALSetValue->m_nValue < 32) {
					nDD = 4;
				}
				else if (pSALSetValue->m_nValue < 64) {
					nDD = 5;
				}
				else {
					nDD = 6;
				}
				VERIFY (MIDIEvent_SetTimeSignature (pCloneEvent, 
					lData[0], nDD, lData[2], lData[3]));
			}
			else if (_tcscmp (pSALSetValue->m_szVarName, _T("TimeSig.Clo")) == 0) {
				VERIFY (MIDIEvent_SetTimeSignature (pCloneEvent, 
					lData[0], lData[1], CLIP (1, pSALSetValue->m_nValue, 255), lData[3]));
			}
			else if (_tcscmp (pSALSetValue->m_szVarName, _T("TimeSig.Blo")) == 0) {
				VERIFY (MIDIEvent_SetTimeSignature (pCloneEvent, 
					lData[0], lData[1], lData[2], CLIP (1, pSALSetValue->m_nValue, 255)));
			}
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if ((_tcscmp (pSALSetValue->m_szVarName, _T("KeySig.Sf")) == 0 ||
			_tcscmp (pSALSetValue->m_szVarName, _T("KeySig.Mi")) == 0) &&
			MIDIEvent_IsKeySignature (pCurSALEvent)) {
			long lData[2];
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_GetKeySignature (pCloneEvent, &lData[0], &lData[1]));
			if (_tcscmp (pSALSetValue->m_szVarName, _T("KeySig.Sf")) == 0) {
				VERIFY (MIDIEvent_SetKeySignature (pCloneEvent,
					CLIP (-7, pSALSetValue->m_nValue, 7), lData[1]));
			}
			else if (_tcscmp (pSALSetValue->m_szVarName, _T("KeySig.Mi")) == 0) {
				VERIFY (MIDIEvent_SetKeySignature (pCloneEvent,
					lData[0], CLIP (0, pSALSetValue->m_nValue, 1)));
			}
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("SeqSpec.Data")) == 0 && 
			MIDIEvent_IsSequencerSpecific (pCurSALEvent)) {
			long* pData = (long*)(pSALSetValue->m_szValue);
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetData (pCloneEvent, (BYTE*)(pSALSetValue->m_szValue), CLIP (0, nLen, 1024))); 
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Note.Key")) == 0 && 
			MIDIEvent_IsNote (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetKey (pCloneEvent, CLIP (0, nValue, 127)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Note.Vel")) == 0 && 
			MIDIEvent_IsNote (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetVelocity (pCloneEvent, CLIP (1, nValue, 127)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Note.Dur")) == 0 && 
			MIDIEvent_IsNote (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetDuration (pCloneEvent, CLIP (1, nValue, 65535)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("KeyAft.Key")) == 0 && 
			MIDIEvent_IsKeyAftertouch (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetKey (pCloneEvent, CLIP (0, nValue, 127)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("KeyAft.Val")) == 0 && 
			MIDIEvent_IsKeyAftertouch (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, CLIP (0, nValue, 127)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Control.Num")) == 0 && 
			MIDIEvent_IsControlChange (pCurSALEvent) && !MIDIEvent_IsCombined (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetNumber (pCloneEvent, CLIP (0, nValue, 127)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Control.Val")) == 0 && 
			MIDIEvent_IsControlChange (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, CLIP (0, nValue, 127)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Patch.Num")) == 0) {
			if (MIDIEvent_IsPatchChange (pCurSALEvent)) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
				VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
				VERIFY (MIDIEvent_SetPatchNum (pCloneEvent, CLIP (0, nValue, 127)));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pSALSetValue->m_pTarget = pCloneEvent;
			}
			else if (MIDIEvent_IsProgramChange (pCurSALEvent)) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
				VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
				VERIFY (MIDIEvent_SetNumber (pCloneEvent, CLIP (0, nValue, 127)));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pSALSetValue->m_pTarget = pCloneEvent;
			}
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Patch.Bank")) == 0) {
			if (MIDIEvent_IsPatchChange (pCurSALEvent)) {
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
				VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
				VERIFY (MIDIEvent_SetBank (pCloneEvent, CLIP (0, nValue, 16383)));
				VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
				pSALSetValue->m_pTarget = pCloneEvent;
			}
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("ChanAft.Val")) == 0 && 
			MIDIEvent_IsChannelAftertouch (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, CLIP (0, nValue, 127)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Wheel.Val")) == 0 && 
			MIDIEvent_IsPitchBend (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetValue (pCloneEvent, CLIP (0, nValue + 8192, 16383)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else if (_tcscmp (pSALSetValue->m_szVarName, _T("Sysx.Data")) == 0 && 
			MIDIEvent_IsSysExEvent (pCurSALEvent)) {
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_REMOVEEVENT, pCurSALEvent));
			VERIFY (pCloneEvent = pSekaijuDoc->ReplaceMIDIEvent (pCurSALEvent));
			VERIFY (MIDIEvent_SetData (pCloneEvent, (BYTE*)(pSALSetValue->m_szValue), CLIP (0, nLen, 1024)));
			VERIFY (pCurHistoryUnit->AddHistoryRecord (HISTORYRECORD_INSERTEVENT, pCloneEvent));
			pSALSetValue->m_pTarget = pCloneEvent;
		}
		else {
			_RPTF1 (_CRT_WARN, "%s:定義されていない変数の設定を要求されました。", pSALSetValue->m_szVarName); 
			nRet = 0;
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		return nRet;
	}
	return 0;
}

long CMainFrame::OnSALGetForEachEventArray (WPARAM wParam, LPARAM lParam) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)wParam;
	void* pSAL = pSekaijuApp->m_pSAL;
	SAL_AddToForEachEventArray pfnSAL_AddToForEachEventArray = 
		(SAL_AddToForEachEventArray)pSekaijuApp->m_pfnSAL_AddToForEachEventArray;
	void* pForEachEventArray = (void*)lParam;
	if (pSekaijuApp->IsDocumentExist (pSekaijuDoc)) {
		pSekaijuDoc->m_theCriticalSection.Lock ();
		int nRet = 1;
		MIDIData* pMIDIData = pSekaijuDoc->m_pMIDIData;
		MIDITrack* pMIDITrack = NULL;
		MIDIEvent* pMIDIEvent = NULL;
		forEachTrack (pMIDIData, pMIDITrack) {
			forEachEvent (pMIDITrack, pMIDIEvent) {
				if (pSekaijuDoc->IsEventSelected (pMIDIEvent)) {
					if (MIDIEvent_IsCombined (pMIDIEvent) && 
						pMIDIEvent->m_pPrevCombinedEvent == NULL ||
						!MIDIEvent_IsCombined (pMIDIEvent)) {
						pfnSAL_AddToForEachEventArray (pSAL, pMIDIEvent);
					}
				}
			}
		}
		pSekaijuDoc->m_theCriticalSection.Unlock ();
		return nRet;
	}
	return 0;
}




long CMainFrame::OnSALEnd (WPARAM wParam, LPARAM lParam) {
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CSekaijuDoc* pSekaijuDoc = (CSekaijuDoc*)wParam;
	SALSetValue* pSALSetValue = (SALSetValue*)lParam;
	if (pSekaijuApp->IsDocumentExist (pSekaijuDoc)) {
		return 1;

	}
	return 0;
}


void CMainFrame::OnDrawItem (int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {
	return CMDIFrameWnd::OnDrawItem (nIDCtl, lpDrawItemStruct);
}

