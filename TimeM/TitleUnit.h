
#pragma once

#define	FLAG_MARKCLEAR	0xFFFFFF00
#define	FLAG_MARKMASK	0x000000FF
#define	FLAG_TITLEMARK1	0x00000001
#define	FLAG_TITLEMARK2	0x00000002
#define	FLAG_TITLEMARK3	0x00000003
#define	FLAG_TITLEMARK4	0x00000004
#define	FLAG_TITLEMARK5	0x00000005
#define	FLAG_TITLEMARK6	0x00000006

#define	FLAG_STATECLEAR	0xFFFF00FF
#define	FLAG_STATEMASK	0x0000FF00
#define	FLAG_TITLELOCK	0x00000100

typedef struct tagTitleUnit
{
	CString strLayer;
	int		nStart;
	int		nEnd;
	CString strStyle;
	CString strActor;
	CString strMarginL;
	CString strMarginR;
	CString strMarginV;
	CString strEffect;
	CString	content;
	DWORD	dwFlags;
}TITLE_UNIT, *PTITLE_UNIT;

inline void InitializeUnit(PTITLE_UNIT pUnit)
{
	pUnit->strLayer = _T(" 0");
	pUnit->nStart = 0;
	pUnit->nEnd = 0;
	pUnit->strStyle = _T("*Default");
	pUnit->strActor = _T("NTP");
	pUnit->strMarginL = _T("0000");
	pUnit->strMarginR = _T("0000");;
	pUnit->strMarginV = _T("0000");;
	pUnit->strEffect.Empty();
	pUnit->content.Empty();
	pUnit->dwFlags = 0;
}

typedef vector<PTITLE_UNIT> VT_TITLE;

#define	ACTTYPE_INSERT	0x01
#define	ACTTYPE_DELETE	0x02
#define	ACTTYPE_MODIFY	0x03

typedef struct tagActionUnit
{
	int			nActionType;	// 1 insert, 2 delete, 3 modify
	int			nActionPos;
	TITLE_UNIT	unitTitle;
}ACTION_UNIT, *PACTION_UNIT;

typedef vector<PACTION_UNIT> VT_ACTION, *PVT_ACTION;
typedef stack<PVT_ACTION> STK_ACTION;

typedef map<int, int>	LISTSEL_MAP;
typedef pair<int, int>	INT_PAIR;

typedef multimap<int, int> SORTINT_MAP;
typedef multimap<string, int> SORTSTR_MAP;
typedef vector<int>	SORT_VT;

#define	READFLAG_UNICODE	0x01
#define	READFLAG_REVERSE	0x02
#define	READFLAG_SINATURE	0x04

#define	FMTFLAG_SRT			0x00
#define	FMTFLAG_SSA			0x01
#define	FMTFLAG_ASS			0x02
#define	FMTFLAG_UNKNOWN		DWORD(-1)

typedef struct	tagReadBuf
{
	LPBYTE		bufFile;
	DWORD		dwBufSize;
	DWORD		dwBufOffset;
	UINT		nUnicodeState;	// 0 is not unicode, and others use flags
								//	READFLAG_UNICODE/READFLAG_REVERSE/READFLAG_SINATURE
	DWORD		dwFmtFlags;		// default 0 is srt, FMTFLAG_SRT/FMTFLAG_SSA/FMTFLAG_ASS
}BUF_READ, *PBUF_READ;

#define	FILE_BUFMAX	(4*1024*1024)
