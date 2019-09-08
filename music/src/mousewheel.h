//******************************************************************************
// MIDIシーケンサーソフト『世界樹』
// マウスホイール対応ヘッダーファイル(VisualC++4.0用)
// (C)2002-2017 おーぷんMIDIぷろじぇくと／くず
//******************************************************************************

/* This Source Code Form is subject to the terms of the Mozilla Public */
/* License, v. 2.0. If a copy of the MPL was not distributed with this */
/* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _MOUSEWHEEL_H_
#define _MOUSEWHEEL_H_

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif

#ifndef WHEELDELTA
#define WHEELDELTA 120
#endif

#ifndef ON_WM_MOUSEWHEEL40
#define ON_WM_MOUSEWHEEL40() \
	{ WM_MOUSEWHEEL, 0, 0, 0, AfxSig_vwp, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))OnMouseWheel40 },
#endif

#endif
