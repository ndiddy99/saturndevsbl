//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 色選択コンボボックスクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _COLORSELECTCOMBOBOX_H_
#define _COLORSELECTCOMBOBOX_H_

#define COLORSELECTCOMBOBOX_BLACK       (RGB (0, 0, 0))
#define COLORSELECTCOMBOBOX_DARKRED     (RGB (128, 0, 0))
#define COLORSELECTCOMBOBOX_DARKGREEN   (RGB (0, 128, 0))
#define COLORSELECTCOMBOBOX_DARKYELLOW  (RGB (128, 128, 0))
#define COLORSELECTCOMBOBOX_DARKBLUE    (RGB (0, 0, 128))
#define COLORSELECTCOMBOBOX_DARKMAGENTA (RGB (128, 0, 128))
#define COLORSELECTCOMBOBOX_DARKCYAN    (RGB (0, 128, 128))
#define COLORSELECTCOMBOBOX_DARKGRAY    (RGB (128, 128, 128))
#define COLORSELECTCOMBOBOX_GRAY        (RGB (192, 192, 192))
#define COLORSELECTCOMBOBOX_RED         (RGB (255, 0, 0))
#define COLORSELECTCOMBOBOX_GREEN       (RGB (0, 255, 0))
#define COLORSELECTCOMBOBOX_YELLOW      (RGB (255, 255, 0)
#define COLORSELECTCOMBOBOX_BLUE        (RGB (0, 0, 255))
#define COLORSELECTCOMBOBOX_MAGENTA     (RGB (255, 0, 255))
#define COLORSELECTCOMBOBOX_CYAN        (RGB (0, 255, 255))
#define COLORSELECTCOMBOBOX_WHITE       (RGB (255, 255, 255))
#define COLORSELECTCOMBOBOX_OTHER       0xFFFFFFFF

class CColorSelectComboBox : public CComboBox {
	DECLARE_DYNCREATE (CColorSelectComboBox)
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
protected:
	COLORREF m_clrCurColor;

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual BOOL PreCreateWindow (CREATESTRUCT&);
	virtual void MeasureItem (LPMEASUREITEMSTRUCT);
	virtual void DrawItem (LPDRAWITEMSTRUCT);

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	BOOL SetCurColor (COLORREF clr);
	COLORREF GetCurColor () const { return m_clrCurColor; }
	void ClearSampleColor ();
	int AddSampleColor (COLORREF clr);
	COLORREF GetSampleColor (int nIndex) const;
	int SetSampleColor (int nIndex, COLORREF clr);
	
	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnSelChange ();
	DECLARE_MESSAGE_MAP ()
};

#endif
