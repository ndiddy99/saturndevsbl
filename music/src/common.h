//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 共用ルーチン
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLIP
#define CLIP(A,B,C) ((B)<(A)?(A):((B)>(C)?(C):(B)))
#endif

// 数字として認識できる文字列か調べる(戻り値:認識できた数字の文字数)
long IsNumeric (const TCHAR* pszText);

/* 16進法で使用可能な文字なら1を、そうでなければ0を返す */
long IsHexDigit (TCHAR c);

/* バイナリ配列を16進テキスト文字列(半角スペース区切り)に変換する */
long bin2txt (BYTE* pBin, long lBinLen, TCHAR* pszText, long lTextLen);

/* 16進テキスト文字列(半角スペース区切り)をバイナリ配列に変換する */
long txt2bin (TCHAR* pszText, long lTextLen, BYTE* pBin, long lBinLen);

/* '\t''\r''\n''\\'コード入りの文字列をコード無しの文字列(C言語スタイル)に変換する　*/
/* 戻り値：出来上がった文字列のバイト数 */
long codestr2str (TCHAR* pszCodeStr, long lCodeStrLen, TCHAR* pszStr, long lStrLen);

/* コード無しの文字列(C言語スタイル)を'\t''\r''\n''\\'コード入りの文字列に変換する */
/* 戻り値：出来上がった文字列のバイト数 */
long str2codestr (TCHAR* pszStr, long lStrLen, TCHAR* pszCodeStr, long lCodeStrLen);


#ifdef __cplusplus
}
#endif

#endif
