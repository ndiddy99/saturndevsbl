//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントの種類リストボックスクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _EVENTKINDLISTBOX_H_
#define _EVENTKINDLISTBOX_H_

class CEventKindListBox : public CCheckListBox {
	DECLARE_DYNCREATE (CEventKindListBox)

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
protected:
	CDocument* m_pDocument;             // ドキュメントへのポインタ
	long m_lMenuID;                     // 右クリックしたときに現れるメニューのID
	long m_lLastRButtonDownIndex;       // 最後に右クリックした項目番号(0～)

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEventKindListBox ();               // コンストラクタ
	CEventKindListBox (CDocument* pDocument, long lMenuID); // コンストラクタ
	virtual ~CEventKindListBox ();      // デストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
public:
	void CEventKindListBox::SetDocument (CDocument* pDocument);
	CSekaijuDoc* GetDocument ();
	long  GetLastRButtonDownIndex ();

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP ()
};

#endif
