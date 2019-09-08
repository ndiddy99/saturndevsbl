//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// MDI子フレームウィンドウクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _CHILDFRAME_H_
#define _CHILDFRAME_H_

class CChildFrame : public CMDIChildWnd {
	DECLARE_DYNCREATE (CChildFrame)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CChildFrame ();                     // コンストラクタ
	virtual ~CChildFrame ();            // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	CDocument* GetDocument ();          // ドキュメントへのポインタ取得
	virtual long GetCurTrackIndex ();   // 純粋仮想関数扱い。

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void PostNcDestroy ();

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
	afx_msg void OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
};

#endif

