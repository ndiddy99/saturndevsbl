//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// インプレースエディットクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _INPLACEEDIT_H_
#define _INPLACEEDIT_H_

class CInplaceEdit : public CEdit {

	DECLARE_DYNCREATE (CInplaceEdit);

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CInplaceEdit ();                    // コンストラクタ
	virtual ~CInplaceEdit ();           // デストラクタ

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnKillFocus (CWnd* pNewWnd);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar (UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()

};

#endif
