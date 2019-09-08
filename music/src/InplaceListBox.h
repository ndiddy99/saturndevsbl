//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// インプレースリストボックスクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _INPLACELISTBOX_H_
#define _INPLACELISTBOX_H_

class CInplaceListBox : public CListBox {

	DECLARE_DYNCREATE (CInplaceListBox);

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CInplaceListBox ();                 // コンストラクタ
	virtual ~CInplaceListBox ();        // デストラクタ

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnKillFocus (CWnd* pNewWnd);
	afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

};


#endif

