//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹ステータスバークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxcmn.h>
#include <afxmt.h>
#include "..\\..\\MIDIIO\\MIDIIO.h"
#include "..\\..\\MIDIData\\MIDIData.h"
#include "..\\..\\MIDIClock\\MIDIClock.h"
#include "..\\..\\MIDIStatus\\MIDIStatus.h"
#include "..\\..\\MIDIInstrument\\MIDIInstrument.h"
#include "Resource.h"
#include "HistoryRecord.h"
#include "HistoryUnit.h"
#include "SekaijuApp.h"
#include "SekaijuToolBar.h"
#include "SekaijuStatusBar.h"
#include "MainFrame.h"
#include "SekaijuDoc.h"




// デバッグ用
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC (CSekaijuStatusBar, CStatusBar)

// メッセージマップ
BEGIN_MESSAGE_MAP (CSekaijuStatusBar, CStatusBar)
	ON_WM_LBUTTONDBLCLK ()	
END_MESSAGE_MAP ()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CSekaijuStatusBar::CSekaijuStatusBar () {
}

// デストラクタ
CSekaijuStatusBar::~CSekaijuStatusBar () {
}

//------------------------------------------------------------------------------
// オーバーライド
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------
void CSekaijuStatusBar::OnLButtonDblClk (UINT nFlags, CPoint point) {
	CDC* pDC = GetDC ();
	CRect rcItem;
	CSekaijuApp* pSekaijuApp = (CSekaijuApp*)AfxGetApp ();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd ();
	long i = 0;
	for (i = 0; i < 6; i++) {
		GetItemRect (i, &rcItem);
		if (rcItem.PtInRect (point)) {
			break;
		}
	}
	switch (i) {
	case 0:
		break;
	case 1: // フォーマット
	case 2: // トラック数
	case 3: // タイムベース
		pMainFrame->PostMessage (WM_COMMAND, ID_FILE_PROPERTY);
		break;
	case 4: // 入力ベロシティゲージ
		pSekaijuApp->m_theCurrentPage.m_lMIDIDevice = 0;
		pMainFrame->PostMessage (WM_COMMAND, ID_SETUP_MIDIDEVICE);
		break;
	case 5: // 出力ベロシティゲージ
		pSekaijuApp->m_theCurrentPage.m_lMIDIDevice = 1;
		pMainFrame->PostMessage (WM_COMMAND, ID_SETUP_MIDIDEVICE);
		break;
	}
}



