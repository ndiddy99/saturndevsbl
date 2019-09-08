//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹ファイルダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _SEKAIJUTOOLBAR_H_
#define _SEKAIJUTOOLBAR_H_


class CSekaijuToolBar : public CToolBar {
	DECLARE_DYNAMIC (CSekaijuToolBar)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CSekaijuToolBar ();                 // コンストラクタ
	virtual ~CSekaijuToolBar ();        // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	void EraseNonClient ();

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void DrawGripper (CDC* pDC, const CRect& rect);
	virtual void DrawBorders (CDC* pDC, CRect& rect);
	virtual void DoPaint (CDC* pDC);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnNcPaint ();
	afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyShiftChange ();
	afx_msg BOOL OnEraseBkgnd (CDC* pDC);
	afx_msg void OnCustomDraw (NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP ()

};

#endif
