//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// ピアノロール印刷ビュークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _PIANOROLLPRINTVIEW_H_
#define _PIANOROLLPRINTVIEW_H_

class CPianoRollPrintView : public CSekaijuView {
	DECLARE_DYNCREATE (CPianoRollPrintView)

	// CPianoRollFrameからCPianoRollPrintView::OnCmdMsgの呼び出しを許可する。
	friend class CPianoRollFrame;

	// 印刷関係
	CSize m_sizDevPaper;                // 物理紙サイズ[ドット]
	CSize m_sizLogPaper;                // 論理紙サイズ[*1/10mm]
	CSize m_sizLogPrinterDPI;           // プリンタのDPI

	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CPianoRollPrintView ();             // コンストラクタ
	virtual ~CPianoRollPrintView ();    // デストラクタ

	//------------------------------------------------------------------------------
	// オペレーション
	//------------------------------------------------------------------------------

protected:
	long TimetoX (long lTime);
	long VeltoY (long lVel);
	long PitchBendtoY (long lPitchBend);
	long TempoBPMtoY (long lTempoBPM);
	long KeyToY (long lKey);
	void DrawFlagAndText (CDC* pDC, long lTime, LPCTSTR lpszText, long lColor);
	void DrawVelScaleView (CDC* pDC);
	void DrawVelTimeView (CDC* pDC);
	void DrawKeyScaleView (CDC* pDC);
	void DrawKeyTimeView (CDC* pDC);
	void DrawScaleView (CDC* pDC);
	void DrawTimeScaleView (CDC* pDC);

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

