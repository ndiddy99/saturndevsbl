
#ifndef _SAL_H_
#define _SAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define WM_COMMANDUPDATEALLVIEWS (WM_USER + 30)
#define WM_SALPAUSE (WM_USER + 31)
#define WM_SALMESSAGE (WM_USER + 32)
#define WM_SALGETINT (WM_USER + 33)
#define WM_SALGETSTRING (WM_USER + 34)
#define WM_SALGETTIME (WM_USER + 35)
#define WM_SALSENDMIDI (WM_USER + 36)
#define WM_SALINSERT (WM_USER + 37)
#define WM_SALDELETE (WM_USER + 38)
#define WM_SALGETVALUE (WM_USER + 39)
#define WM_SALSETVALUE (WM_USER + 40)
#define WM_SALGETFOREACHEVENTARRAY (WM_USER + 41)
#define WM_SALEND (WM_USER + 42)

typedef struct tagSALGetValue {
	TCHAR m_szVarName[1024];
	void* m_pTarget;
	int m_nVarType;
	int m_nParam1;
	int m_nParam2;
	int m_nParam3;
	int m_nParam4;
	int m_nValue;
	int m_nLen;
	TCHAR m_szValue[1024];
} SALGetValue;

typedef struct tagSALSetValue {
	TCHAR m_szVarName[1024];
	void* m_pTarget; /* ÉqÉXÉgÉäãLò^Ç…ÇÊÇËê›íËå„ÇÕïœÇÌÇÈÇÃÇ≈forEachEventArrrayÇíºÇ∑Ç±Ç∆ */
	int m_nVarType;
	int m_nParam1;
	int m_nParam2;
	int m_nParam3;
	int m_nParam4;
	int m_nValue;
	int m_nLen;
	TCHAR m_szValue[1024];
} SALSetValue;

typedef struct tagSALInsert {
	void* m_pCurEvent;
	int m_nTime;
	int m_nKind;
	int m_nCh;
	int m_nValue[1024];
	TCHAR m_szValue[1024];
} SALInsert;

typedef struct tagSALSendMIDI {
	int m_nPort;
	int m_nCh;
	int m_nKind;
	int m_nLen;
	int m_nValue[1024];
} SALSendMIDI;


// ä÷êî
typedef void*(_stdcall* SAL_Create)(HWND, const TCHAR*, int);
typedef void (_stdcall* SAL_Delete)(void*);
typedef int  (_stdcall* SAL_SelectFile)(void*, TCHAR*, int);
typedef int  (_stdcall* SAL_LoadFile)(void*, const TCHAR*, int);
typedef int  (_stdcall* SAL_Run)(void*, const void*);
typedef int  (_stdcall* SAL_AddToForEachEventArray)(void*, void*);
typedef int  (_stdcall* SAL_Terminate)(void*, int nReason);



#ifdef __cplusplus
}
#endif

#endif



