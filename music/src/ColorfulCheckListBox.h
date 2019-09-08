//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// カラフルなチェックリストボックスクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _COLORFULCHECKLISTBOX_H_
#define _COLORFULCHECKLISTBOX_H_

class CColorfulCheckListBox : public CCheckListBox {
	DECLARE_DYNCREATE (CColorfulCheckListBox)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CColorfulCheckListBox ();           // コンストラクタ
	virtual ~CColorfulCheckListBox ();  // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	int  AddItem (LPCTSTR lpszText, COLORREF lForeColor, COLORREF lBackColor);
	void RemoveAllItem ();

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);
	virtual void MeasureItem (LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem (LPDRAWITEMSTRUCT lpDrawItemStruct);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnDestroy ();
	DECLARE_MESSAGE_MAP ()

};

#endif
