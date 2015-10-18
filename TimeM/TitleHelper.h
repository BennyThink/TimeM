#pragma once
#include "TitleUnit.h"

class CTitleHelper
{
public:
	CTitleHelper(void);
	~CTitleHelper(void);

	static CString	FormatComma(int nValue);
	static CString	FormatSSATime(int nTime);
	static CString	FormatTime(int nTime);				// million second
	static int		GetTimeValue(const CString& strTime);	// million second

	static CString	FormatSRTString(int nSeqNo, PTITLE_UNIT pUnit);
	static CString	FormatSSAString(PTITLE_UNIT pUnit, const CString& strPreCode, const CString& strPostCode);
	static CString	FormatASSString(PTITLE_UNIT pUnit, const CString& strPreCode, const CString& strPostCode);

	static void		RemoveDupSpace(CString& strSentence);
	static void		CCSentence(CString& strSentence);
	static void		RemoveAssCode(CString& strSentence);
	static BOOL		IsWordBreak(TCHAR ch);
	static BOOL		BreakTitleUnit(int nPos, PTITLE_UNIT pUnit1, PTITLE_UNIT pUnit2);
	static BOOL		BreakUnitByTimeSpin(int nTimeSpin, PTITLE_UNIT pUnit1, PTITLE_UNIT pUnit2);
	static BOOL		BreakUnitByShare(int nShare, PTITLE_UNIT pUnit1, PTITLE_UNIT pUnit2);
	static int		SelectPriorityChar(LPCTSTR lpszData, int nStartPos, int nPreBreak, int nSufBreak);
	static BOOL		ReplaceNoCase(CString& strSrc, const CString& strTarget, const CString& strResult);
	static BOOL		ReplaceTitle(PTITLE_UNIT pUnit, const CString& strTarget, const CString& strResult,	BOOL bCase, BOOL bEng, BOOL bChs);

	static BOOL		IsDBCSString(CString& strTag);
	static BOOL		BreakStringEachLine(LPCTSTR lpszSrc, CStringArray& astrLines);
	static BOOL		BreakString(const CString& strSrc, CString& strFirstLine, CString& strOtherLine);
	static int		CountTitleLine(const CString& strTitle, CString& strCount);
	static int		GetCountTitle(const CString& strTitle);
	static int		CountChsChar(const CString& strTitle);
	static PTITLE_UNIT	GetTitleItem(int nPos, VT_TITLE& vtTitle);
};

