//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// 世界樹ドキュメントマネージャークラス
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _SEKAIJUDOCMANAGER_H_
#define _SEKAIJUDOCMANAGER_H_

class CSekaijuDocManager : public CDocManager {

	DECLARE_DYNAMIC (CSekaijuDocManager)

	//--------------------------------------------------------------------------
	// オーバーライド
	//--------------------------------------------------------------------------
public:
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
			DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);

};

#endif
