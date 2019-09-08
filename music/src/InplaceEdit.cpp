//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// インプレースエディットクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */


#include "winver.h"
#include <afxwin.h>
#include <afxext.h>

#include "InplaceEdit.h"


// アロケーションの監視
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// メッセージマップ
IMPLEMENT_DYNCREATE (CInplaceEdit, CEdit)

BEGIN_MESSAGE_MAP (CInplaceEdit, CEdit)
	ON_WM_KILLFOCUS ()
	ON_WM_KEYDOWN ()
	ON_WM_CHAR ()
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CInplaceEdit::CInplaceEdit () {
}

// デストラクタ
CInplaceEdit::~CInplaceEdit () {
}

//------------------------------------------------------------------------------
// メッセージマップ
//------------------------------------------------------------------------------

// フォーカスが失われたとき
void CInplaceEdit::OnKillFocus (CWnd* pNewWnd) {
	_RPTF1 (_CRT_WARN, "CInplaceEditBox::OnKillFocus (pNewWnd=0x%08x)\n", pNewWnd);
	CWnd* pParentWnd = GetParent ();
	if (pParentWnd != pNewWnd) {
		pParentWnd->SendMessage (WM_KILLFOCUS, (WPARAM)pNewWnd, (LPARAM)0L);
	}
	else {
		//pParentWnd->SetFocus ();
	}
	CEdit::OnKillFocus (pNewWnd); //20080809追加(カレットの消失防止)
}

// キー押し下げ時
void CInplaceEdit::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
	CWnd* pParentWnd = GetParent ();
	switch (nChar) {
	case VK_RETURN:
		//GetParent ()->SendMessage (WM_KEYDOWN, VK_RETURN, (nRepCnt << 16) | nFlags);
		pParentWnd->SetFocus ();
		pParentWnd->PostMessage (WM_KEYDOWN, VK_RETURN, (nRepCnt << 16) | nFlags);
		break;
	case VK_ESCAPE:
		//GetParent ()->SendMessage (WM_KEYDOWN, VK_ESCAPE, (nRepCnt << 16) | nFlags);
		pParentWnd->SetFocus ();
		pParentWnd->PostMessage (WM_KEYDOWN, VK_ESCAPE, (nRepCnt << 16) | nFlags);
		break;
	}
	CEdit::OnKeyDown (nChar, nRepCnt, nFlags);
}

// 文字入力時
void CInplaceEdit::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags) {
	// Enterを押したときにプープー音が鳴るのを防ぐ
	switch (nChar) {
	case VK_RETURN:
		return;
	}
	CEdit::OnChar (nChar, nRepCnt, nFlags);
}
