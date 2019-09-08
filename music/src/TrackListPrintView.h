//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// イベントリスト印刷ビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _TRACKLISTPRINTVIEW_H_
#define _TRACKLISTPRINTVIEW_H_

class CTrackListPrintView : public CSekaijuView {

public:
	DECLARE_DYNCREATE (CTrackListPrintView)

	// CTrackListFrameからCTrackListPrintView::OnCmdMsgの呼び出しを許可する。
	friend class CTrackListFrame;

	// 印刷関係
	CSize m_sizDevPaper;                // 物理紙サイズ[ドット]
	CSize m_sizLogPaper;                // 論理紙サイズ[*1/10mm]
	CSize m_sizLogPrinterDPI;           // プリンタのDPI
	long m_lNumTrackPerPage;            // 1ページあたりのトラック数
	long m_lMaxRowPage;                 // 最大ページ数(縦方向)
	long m_lMaxColPage;                 // 最大ページ数(横方向)
	long m_lMaxRollPage;                // 最大ページ数(ピアノロール方向)
	long m_lMaxPage;                    // 最大ページ数(縦方向*(横方向+ピアノロール方向))

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CTrackListPrintView();              // コンストラクタ
	virtual ~CTrackListPrintView ();    // デストラクタ

	//------------------------------------------------------------------------------
	// オペレーション
	//------------------------------------------------------------------------------
protected:
	CString GetCellString (long lRow, long lColumn);
	long TimetoX (long lTime);
	void DrawIndexScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawIndexPropertyView (CDC* pDC, CPrintInfo* pInfo);
	void DrawPropertyScaleView (CDC* pDC, CPrintInfo* pInfo);
	void DrawTrackTimeView (CDC* pDC, CPrintInfo* pInfo);
	void DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor);
	void DrawTimeScaleView (CDC* pDC, CPrintInfo* pInfo);

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
