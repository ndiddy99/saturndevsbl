//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// テキストファイルクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _TEXTFILE_H_
#define _TEXTFILE_H_

class CTextFile : public CFile {
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
public:
	CTextFile();                        // コンストラクタ

	//--------------------------------------------------------------------------
	// オペレーション
	//--------------------------------------------------------------------------
protected:
	virtual void InvertByteOrder (LPWSTR lpszInvertText, LPCWSTR lpszText, int nLen);
public:
	virtual void WriteBOM (int nCodePage);
	virtual int  ReadBOM (int* pCodePage);
	virtual void WriteTextA (LPCSTR lpszText, int nCodePage);
	virtual void WriteTextW (LPCWSTR lpszText, int nCodePage);
#ifdef _UNICODE
#define WriteText WriteTextW
#else
#define WriteText WriteTextA
#endif
	virtual UINT ReadTextA (LPSTR lpszText, int nTextLen, int nCodePage);
	virtual UINT ReadTextW (LPWSTR lpszText, int nTextLen, int nCodePage);
	virtual CString ReadTextA (int nCodePage);
	virtual CString ReadTextW (int nCodePage);

#ifdef _UNICODE
#define ReadText ReadTextW
#else
#define ReadText ReadTextA
#endif
};

#endif
