//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹ファイルダイアログクラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _SEKAIJUFILEDLG_H_
#define _SEKAIJUFILEDLG_H_

class CSekaijuFileDlg : public CFileDialog {

	//DECLARE_DYNAMIC (CSekaijuFileDlg)
	
	//--------------------------------------------------------------------------
	// 構築と破壊
	//--------------------------------------------------------------------------
	// コンストラクタ
public:
	CSekaijuFileDlg (BOOL bOpenFileDialog); 
	CSekaijuFileDlg (BOOL bOpenFileDialog,
		LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags,
		LPCTSTR lpszFilter, CWnd* pParentWnd);

	//--------------------------------------------------------------------------
	// アトリビュート
	//--------------------------------------------------------------------------
public:
	CString m_strDefExt; // デフォルトの拡張子(ファイルタイプを切り替えるたびに更新)

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
public:
	//virtual int DoModal ();
	virtual void OnTypeChange (); // ファイルタイプが変更された
};

#endif
