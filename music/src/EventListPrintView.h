//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリスト印刷ビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _EVENTLISTPRINTVIEW_H_
#define _EVENTLISTPRINTVIEW_H_

class CEventListPrintView : public CSekaijuView {

public:
	DECLARE_DYNCREATE (CEventListPrintView)

	// CEventListFrameからCEventListPrintView::OnCmdMsgの呼び出しを許可する。
	friend class CEventListFrame;

	// 印刷関係
	CSize m_sizDevPaper;                // 物理紙サイズ[ドット]
	CSize m_sizLogPaper;                // 論理紙サイズ[*1/10mm]
	CSize m_sizLogPrinterDPI;           // プリンタのDPI
	long m_lNumEventPerPage;            // 1ページあたりのイベント数
	long m_lMaxPage;                    // 最大ページ数

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CEventListPrintView();             // コンストラクタ
	virtual ~CEventListPrintView ();   // デストラクタ

	//------------------------------------------------------------------------------
	// オペレーション
	//------------------------------------------------------------------------------

protected:
	CRect GetRectFromCell (long lRow, long lColumn);
	CString GetCellString (long lRow, long lColumn);
	void DrawIndexScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawIndexPropertyView (CDC* pDC, CPrintInfo* pInfo);
	void DrawPropertyScaleView (CDC* pDC, CPrintInfo* pInfo);

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
protected:
	virtual void OnPrint (CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting (CPrintInfo* pInfo);
	virtual void OnBeginPrinting (CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting (CDC* pDC, CPrintInfo* pInfo);

	//--------------------------------------------------------------------------
	// メッセージマップ
	//--------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP ()
};

#endif
