//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// カラフルなコンボボックスクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _COLORFULCOMBOBOX_H_
#define _COLORFULCOMBOBOX_H_

class CColorfulComboBox : public CComboBox {
	DECLARE_DYNCREATE (CColorfulComboBox)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CColorfulComboBox ();
	virtual ~CColorfulComboBox ();

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
