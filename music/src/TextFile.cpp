//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// テキストファイルクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "winver.h"
#include <locale.h>
#include <afxwin.h>
#include "TextFile.h"

//------------------------------------------------------------------------------
// 構築と破壊
//------------------------------------------------------------------------------

// コンストラクタ
CTextFile::CTextFile () : CFile () {

}

//------------------------------------------------------------------------------
// オペレーション
//------------------------------------------------------------------------------

// バイトオーダーの反転(BE⇔LE)
void CTextFile::InvertByteOrder (LPWSTR lpszInvertText, LPCWSTR lpszText, int nTextLen) {
	BYTE* pDest = (BYTE*)lpszInvertText;
	BYTE* pSrc = (BYTE*)lpszText;
	for (int i = 0; i < nTextLen; i++) {
		for (int j = 0; j < (int)sizeof (wchar_t); j++) {
			*(pDest + sizeof (wchar_t) * i + j) = 
			*(pSrc + sizeof (wchar_t) * (i + 1) - (j + 1)); 
		}
	}
}

// BOMの書き込み
void CTextFile::WriteBOM (int nCodePage) {
	// UTF-16LE
	if (nCodePage == 1200) {
		BYTE byBOM [] = {0xFF, 0xFE};
		CFile::Write (byBOM, 2);
	}
	// UTF-16BE
	else if (nCodePage == 1201) {
		BYTE byBOM [] = {0xFE, 0xFF};
		CFile::Write (byBOM, 2);
	}
	// UTF-8 (currently unsupported)
	else if (nCodePage == CP_UTF8) {
		BYTE byBOM [] = {0xEF, 0xBB, 0xBF};
		CFile::Write (byBOM, 3);
	}
}

// BOMの読み込み
int CTextFile::ReadBOM (int* pCodePage) {
	BYTE byBOM[3];
	memset (byBOM, 0, 3);
	*pCodePage = 0;
	int nLen = 0;
	CFile::Read (byBOM, 3);
	// UTF-16LE
	if (byBOM[0] == 0xFF && byBOM[1] == 0xFE) {
		*pCodePage = 1200;
		nLen = 2;
	}
	// UTF-16BE
	if (byBOM[0] == 0xFE && byBOM[1] == 0xFF) {
		*pCodePage = 1201;
		nLen = 2;
	}
	// UTF-8
	else if (byBOM[0] == 0xEF && byBOM[1] == 0xBB && byBOM[2] == 0xBF) {
		*pCodePage = CP_UTF8;
		nLen = 3;
	}
	CFile::Seek (nLen, CFile::begin);
	return nLen;
}


// テキストの書き出し(ANSI)
void CTextFile::WriteTextA (LPCSTR lpszText, int nCodePage) {
	int nTextLen = strlen (lpszText);
	char szLocale[16];
	memset (szLocale, 0, sizeof (szLocale));
	if (nCodePage == 0) {
		_snprintf (szLocale, 16, ".ACP", nCodePage);
	}
	else {
		_snprintf (szLocale, 16, ".%d", nCodePage);
	}
	// ANSIをUTF-16LEで書き込み
	if (nCodePage == 1200) {
		wchar_t* lpszWideText = (wchar_t*)calloc (sizeof (wchar_t), nTextLen + 1);
		if (lpszWideText == NULL) {
			return;
		}
		setlocale (LC_CTYPE, ".ACP");
		int nRet = mbstowcs (lpszWideText, lpszText, nTextLen);
		int nWideTextLen = wcslen (lpszWideText);
		CFile::Write (lpszWideText, sizeof(wchar_t) * nWideTextLen);
		free (lpszWideText);
	}
	// ANSIをUTF-16BEで書き込み
	else if (nCodePage == 1201) {
		wchar_t* lpszWideText = (wchar_t*)calloc (sizeof (wchar_t), nTextLen + 1);
		wchar_t* lpszInvertText = (wchar_t*)calloc (sizeof (wchar_t), nTextLen + 1);
		if (lpszWideText == NULL || lpszInvertText == NULL) {
			free (lpszWideText);
			free (lpszInvertText);
			return;
		}
		setlocale (LC_CTYPE, ".ACP");
		int nRet = mbstowcs (lpszWideText, lpszText, nTextLen);
		int nWideTextLen = wcslen (lpszWideText);
		InvertByteOrder (lpszInvertText, lpszWideText, nWideTextLen);
		CFile::Write (lpszInvertText, sizeof(wchar_t) * nWideTextLen);
		free (lpszWideText);
		free (lpszInvertText);
	}
	// ANSIをANSIで書き込み
	else {
		CFile::Write (lpszText, nTextLen);
	}
}

// テキストの書き出し(Unicode)
void CTextFile::WriteTextW (LPCWSTR lpszText, int nCodePage) {
	int nTextLen = wcslen (lpszText);
	wchar_t szLocale[16];
	memset (szLocale, 0, sizeof (szLocale));
	if (nCodePage == 0) {
		_snwprintf (szLocale, 16, L".ACP");
	}
	else {
		_snwprintf (szLocale, 16, L".%d", nCodePage);
	}
	// UTF16LEをUTF16LEで書き込み
	if (nCodePage == 1200) {
		CFile::Write (lpszText, sizeof (wchar_t) * nTextLen);
	}
	// UTF16LEをUTF16BEで書き込み
	else if (nCodePage == 1201) {
		wchar_t* lpszInvertText = (wchar_t*)calloc (sizeof (wchar_t), nTextLen + 1);
		if (lpszInvertText == NULL) {
			return;
		}
		InvertByteOrder (lpszInvertText, lpszText, nTextLen);
		CFile::Write (lpszInvertText, sizeof (wchar_t) * nTextLen);
		free (lpszInvertText);
	}
	// UTF16LEをANSIで書き込み
	else {
		char* lpszAnsiText = (char*)calloc (1, 4 * nTextLen + 1);
		if (lpszAnsiText == NULL) {
			return;
		}
		_wsetlocale (LC_CTYPE, szLocale);
		int nRet = wcstombs (lpszAnsiText, lpszText, 4 * nTextLen);
		int nAnsiTextLen = strlen (lpszAnsiText);
		CFile::Write (lpszAnsiText, nAnsiTextLen);
		free (lpszAnsiText);
	}
}

// テキストの読み込み(ANSI)
UINT CTextFile::ReadTextA (LPSTR lpszText, int nTextLen, int nCodePage) {
	char szLocale[16];
	memset (szLocale, 0, sizeof (szLocale));
	if (nCodePage == 0) {
		_snprintf (szLocale, 16, ".ACP", nCodePage);
	}
	else {
		_snprintf (szLocale, 16, ".%d", nCodePage);
	}
	memset (lpszText, 0, nTextLen);
	// ファイルサイズの取得
	__int64 llFileSize = CFile::GetLength ();
	// UTF16LEファイルをANSIで読み込み
	if (nCodePage == 1200) {
		wchar_t* lpszUnicodeText = (wchar_t*)calloc (sizeof (wchar_t), (nTextLen + 1));
		if (lpszUnicodeText == NULL) {
			return 0;
		}
		CFile::Read (lpszUnicodeText, sizeof (wchar_t) * (nTextLen - 1));
		setlocale (LC_CTYPE, szLocale);
		int nRet = wcstombs (lpszText, lpszUnicodeText, nTextLen);
		free (lpszUnicodeText);
		return nRet;
	}
	// UTF16BEファイルをANSIで読み込み
	else if (nCodePage == 1201) {
		wchar_t* lpszUnicodeText = (wchar_t*)calloc (sizeof (wchar_t), (nTextLen + 1));
		if (lpszUnicodeText == NULL) {
			return 0;
		}
		wchar_t* lpszTempText = (wchar_t*)calloc (sizeof (wchar_t), nTextLen + 1);
		if (lpszTempText == NULL) {
			return 0;
		}
		CFile::Read (lpszTempText, sizeof (wchar_t) * (nTextLen - 1));
		InvertByteOrder (lpszUnicodeText, lpszTempText, nTextLen);
		setlocale (LC_CTYPE, szLocale);
		int nRet = wcstombs (lpszText, lpszUnicodeText, nTextLen);
		free (lpszTempText);
		free (lpszUnicodeText);
	}
	// ANSIファイルをANSIで読み込み
	else {
		CFile::Read (lpszText, (nTextLen - 1));
	}
	return strlen (lpszText);
}

// テキストの読み込み(Unicode)
UINT CTextFile::ReadTextW (LPWSTR lpszText, int nTextLen, int nCodePage) {
	wchar_t szLocale[16];
	memset (szLocale, 0, sizeof (szLocale));
	if (nCodePage == 0) {
		_snwprintf (szLocale, 16, L".ACP");
	}
	else {
		_snwprintf (szLocale, 16, L".%d", nCodePage);
	}
	// ファイルサイズの取得
	memset (lpszText, 0, sizeof (wchar_t) * nTextLen);
	// UTF16LEファイルをUTF16LEで読み込み
	if (nCodePage == 1200) {
		CFile::Read (lpszText, sizeof (wchar_t) * (nTextLen - 1));
	}
	// UTF16BEファイルをUTF16LEで読み込み
	else if (nCodePage == 1201) {
		wchar_t* lpszTempText = (wchar_t*)calloc (sizeof (wchar_t), nTextLen + 1);
		if (lpszTempText == NULL) {
			return 0;
		}
		CFile::Read (lpszTempText, sizeof (wchar_t) * (nTextLen - 1));
		InvertByteOrder (lpszText, lpszTempText, nTextLen);
		free (lpszTempText);
	}
	// ANSIファイルをUTF16LEで読み込み
	else {
		char* lpszAnsiText = (char*)calloc (1, nTextLen + 1);
		if (lpszAnsiText == NULL) {
			return 0;
		}
		memset (lpszAnsiText, 0, nTextLen + 1);
		CFile::Read (lpszAnsiText, (nTextLen - 1));
		int nRet = mbstowcs (lpszText, lpszAnsiText, (nTextLen - 1));
		int nAnsiTextLen = strlen (lpszAnsiText);
		free (lpszAnsiText);
	}
	return wcslen (lpszText);
}

// テキストの読み込み(ANSI)
CString CTextFile::ReadTextA (int nCodePage) {
	char szLocale[16];
	memset (szLocale, 0, sizeof (szLocale));
	if (nCodePage == 0) {
		_snprintf (szLocale, 15, ".ACP");
	}
	else {
		_snprintf (szLocale, 15, ".%d", nCodePage);
	}
	// ファイルサイズの取得
	__int64 nnFileSize = CFile::GetLength ();
	int nFileSize = (int)__min (nnFileSize, 1073741823);
	BYTE* pFileImage = (BYTE*)calloc (1, nFileSize + 1);
	if (pFileImage == NULL) {
		return "";
	}
	CFile::Read (pFileImage, nFileSize);
	CFile::Close ();
	CString strRet;

	// UTF16LEファイルをANSIで読み込み
	if (nCodePage == 1200) {
		char* pszAnsiText = (char*)calloc (sizeof (char), (nFileSize + 1));
		if (pszAnsiText == NULL) {
			free (pFileImage);
			return "";
		}
		setlocale (LC_CTYPE, szLocale);
		int nRet = wcstombs (pszAnsiText, (const wchar_t*)pFileImage, nFileSize);
		strRet = pszAnsiText;
		free (pszAnsiText);
	}
	// UTF16BEファイルをANSIで読み込み
	else if (nCodePage == 1201) {
		wchar_t* lpszUnicodeText = (wchar_t*)calloc (1, (nFileSize + 1));
		if (lpszUnicodeText == NULL) {
			free (pFileImage);
			return "";
		}
		char* lpszAnsiText = (char*)calloc (4, nFileSize + 1);
		if (lpszAnsiText == NULL) {
			free (pFileImage);
			return "";
		}
		InvertByteOrder ((LPWSTR)pFileImage, lpszUnicodeText, nFileSize / sizeof (wchar_t));
		setlocale (LC_CTYPE, szLocale);
		int nRet = wcstombs (lpszAnsiText, lpszUnicodeText, 4 * nFileSize);
		strRet = lpszAnsiText;
		free (lpszAnsiText);
		free (lpszUnicodeText);
	}
	// ANSIファイルをANSIで読み込み
	else {
		strRet = (char*)pFileImage;
	}
	free (pFileImage);
	return strRet;
}

// テキストの読み込み(Unicode)
CString CTextFile::ReadTextW (int nCodePage) {
	wchar_t szLocale[16];
	memset (szLocale, 0, sizeof (szLocale));
	if (nCodePage == 0) {
		_snwprintf (szLocale, 16, L".ACP");
	}
	else {
		_snwprintf (szLocale, 16, L".%d", nCodePage);
	}
	// ファイルサイズの取得
	__int64 nnFileSize = CFile::GetLength ();
	int nFileSize = (int)__min (nnFileSize, 1073741823);
	BYTE* pFileImage = (BYTE*)calloc (1, nFileSize + sizeof (wchar_t));
	if (pFileImage == NULL) {
		return L"";
	}
	CFile::Read (pFileImage, nFileSize);
	CFile::Close ();
	CString strRet;
	// UTF16LEファイルをUTF16LEで読み込み
	if (nCodePage == 1200) {
		strRet = (TCHAR*)pFileImage;
	}
	// UTF16BEファイルをUTF16LEで読み込み
	else if (nCodePage == 1201) {
		BYTE* pFileImage2 = (BYTE*)calloc (1, nFileSize + sizeof (wchar_t));
		if (pFileImage2 == NULL) {
			free (pFileImage);
			return L"";
		}
		InvertByteOrder ((LPWSTR)pFileImage, (LPWSTR)pFileImage2, nFileSize / sizeof (wchar_t));
		strRet = (wchar_t*)pFileImage2;
		free (pFileImage2);
	}
	// ANSIファイルをUTF16LEで読み込み
	else {
		wchar_t* pszUnicodeText = (wchar_t*)calloc (sizeof (wchar_t), nFileSize + 1);
		if (pszUnicodeText == NULL) {
			free (pFileImage);
			return L"";
		}
		_wsetlocale (LC_CTYPE, szLocale);
		int nRet = mbstowcs ((LPWSTR)pszUnicodeText, (LPSTR)pFileImage, sizeof (wchar_t) * nFileSize);
		strRet = pszUnicodeText;
		free (pszUnicodeText);
	}
	free (pFileImage);
	return strRet;
}

