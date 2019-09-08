//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 色オプションページクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _COLOROPTIONPAGE_H_
#define _COLOROPTIONPAGE_H_

#include "ColorSelectComboBox.h"
 
class CColorOptionPage : public CPropertyPage {
	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CColorSelectComboBox m_theForeColorCombo[8];
	CColorSelectComboBox m_theBackColorCombo[2];
	CColorSelectComboBox m_theHorzColorCombo[2];
	CColorSelectComboBox m_theVertColorCombo[2];

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CColorOptionPage ();              // コンストラクタ
	enum {IDD = IDD_COLOROPTION};

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV のサポート
	virtual BOOL OnInitDialog ();

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
};

#endif
