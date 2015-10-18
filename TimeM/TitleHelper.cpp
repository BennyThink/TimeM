#include "StdAfx.h"
#include "TitleHelper.h"

CTitleHelper::CTitleHelper(void)
{
}

CTitleHelper::~CTitleHelper(void)
{
}

CString	CTitleHelper::FormatComma(int nValue)
{
	CString strOutput;
	CString strLow;
	int nHigh = nValue;
	int nLow = 0;
	while(nHigh > 0)
	{
		nLow = nHigh%1000;
		nHigh = nHigh/1000;
		if(nHigh > 0)
		{
			strLow.Format(_T(",%03d"), nLow);
		}
		else
		{
			strLow.Format(_T("%d"), nLow);
		}
		strOutput = strLow + strOutput;
	}
	return strOutput;
}

CString CTitleHelper::FormatSSATime(int nTime)
{
	CString	strTime;
	int		nHour,
			nMinute,
			nSecond,
			nMillion;

	nHour		= nTime/3600000;
	nMinute		= (nTime - nHour*3600*1000)/60000;
	nSecond		= (nTime - nHour*3600*1000 - nMinute*60*1000)/1000;
	nMillion	= nTime - nHour*3600*1000 - nMinute*60*1000 - nSecond*1000;
	nMillion	= nMillion/10;

	strTime.Format(_T("%01d:%02d:%02d.%02d"), nHour, nMinute, nSecond, nMillion);
	return strTime;
}

CString CTitleHelper::FormatTime(int nTime)
{
	CString	strTime;
	int		nHour,
			nMinute,
			nSecond,
			nMillion;

	nHour		= nTime/3600000;
	nMinute		= (nTime - nHour*3600*1000)/60000;
	nSecond		= (nTime - nHour*3600*1000 - nMinute*60*1000)/1000;
	nMillion	= nTime - nHour*3600*1000 - nMinute*60*1000 - nSecond*1000;

	strTime.Format(_T("%02d:%02d:%02d,%03d"), nHour, nMinute, nSecond, nMillion);
	return strTime;
}

int CTitleHelper::GetTimeValue(const CString& strTime)
{
	int nTime = 0;

	int nLen = strTime.GetLength();
	if(nLen == 12)
	{
		if((strTime.GetAt(2) == _T(':'))&&
			(strTime.GetAt(5) == _T(':'))&&
			(strTime.GetAt(8) == _T(',')))
		{
			CString strLeft = strTime.Left(2);
			int nHour = _tstoi(strLeft);

			CString strRight = strTime.Right(nLen - 3);
			strLeft = strRight.Left(2);
			int nMinute = _tstoi(strLeft);

			strRight = strRight.Right(nLen - 6);
			strLeft = strRight.Left(2);
			int nSecond = _tstoi(strLeft);

			strRight = strRight.Right(nLen - 9);
			int nMillion = _tstoi(strRight);

			nTime = nHour*3600*1000 + nMinute*60*1000 + nSecond*1000 + nMillion;
		}
	}

	if(nLen == 10)
	{
		if((strTime.GetAt(1) == _T(':'))&&
			(strTime.GetAt(4) == _T(':'))&&
			(strTime.GetAt(7) == _T('.')))
		{
			CString strLeft = strTime.Left(1);
			int nHour = _tstoi(strLeft);

			CString strRight = strTime.Right(nLen - 2);
			strLeft = strRight.Left(2);
			int nMinute = _tstoi(strLeft);

			strRight = strRight.Right(nLen - 5);
			strLeft = strRight.Left(2);
			int nSecond = _tstoi(strLeft);

			strRight = strRight.Right(nLen - 8);
			int nMillion = _tstoi(strRight);

			nTime = nHour*3600*1000 + nMinute*60*1000 + nSecond*1000 + nMillion*10;
		}
	}

	return nTime;
}

CString	CTitleHelper::FormatSRTString(int nSeqNo, PTITLE_UNIT pUnit)
{
	CString strItem;
	strItem.Format(_T("%d\r\n%s --> %s\r\n%s\r\n"),
		nSeqNo,
		CTitleHelper::FormatTime(pUnit->nStart),
		CTitleHelper::FormatTime(pUnit->nEnd),
		pUnit->content);
	if(pUnit->content.GetLength() > 0)
		strItem.Append(_T("\r\n"));
	return strItem;
}

CString	CTitleHelper::FormatSSAString(PTITLE_UNIT pUnit, const CString& strPreCode, const CString& strPostCode)
{
	CString strContent = pUnit->content;
	strContent = strContent.TrimLeft(_T("\r\n"));
	strContent = strContent.TrimRight(_T("\r\n"));
	// for special YYeTs, process effect code for english line precode and postcode
	CStringArray astrLines;
	BreakStringEachLine(strContent, astrLines);
	int nCnt = astrLines.GetCount();
	BOOL	bInChs = TRUE;
	strContent.Empty();
	for(int iLine = 0; iLine < nCnt; iLine ++)
	{
		if((CTitleHelper::IsDBCSString(astrLines[iLine]))
			||((nCnt == 2)
			&&(astrLines[0].Compare(astrLines[1]) == 0)
			&&(iLine == 0)))
		{
			if(!bInChs)
				strContent += strPostCode;
			if(!strContent.IsEmpty())
				strContent += _T("\r\n");
			bInChs = TRUE;
			strContent += astrLines[iLine];
		}
		else
		{
			if(!strContent.IsEmpty())
				strContent += _T("\r\n");
			if(bInChs)
				strContent += strPreCode;
			strContent += astrLines[iLine];
			bInChs = FALSE;
		}
	}
	if(!bInChs)
		strContent += strPostCode;
	// nEnd special code

	strContent.Replace(_T("\r\n"), _T("\\N"));
	CString strItem;
	strItem.Format(_T("Dialogue:%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\r\n"),
		pUnit->strLayer,
		CTitleHelper::FormatSSATime(pUnit->nStart),
		CTitleHelper::FormatSSATime(pUnit->nEnd),
		pUnit->strStyle,
		pUnit->strActor,
		pUnit->strMarginL,
		pUnit->strMarginR,
		pUnit->strMarginV,
		pUnit->strEffect,
		strContent);
	return strItem;
}

CString	CTitleHelper::FormatASSString(PTITLE_UNIT pUnit, const CString& strPreCode, const CString& strPostCode)
{
	return CTitleHelper::FormatSSAString(pUnit, strPreCode, strPostCode);
}

//	"0123456789",
//	"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
//	"abcdefghijklmnopqrstuvwxyz", case '_': case '-': 
/*
	switch(ch)
	{
	case ' ': case '!': case '\'': case ')': case ',': case '.': 
	case ':': case ';': case '=': case '>': case '?': case '@':
		case '"': case '#': case '$': case '%': case '&': case '(': case '*': case '+': case '/': case '<': 
	case '[': case '\\': case ']': case '^': case '`': case '{': case '|': case '}': case '~':
	}
*/
BOOL CTitleHelper::IsWordBreak(TCHAR ch)
{
	if(ch == _T(' '))
		return TRUE;
	return FALSE;
}

int CTitleHelper::SelectPriorityChar(LPCTSTR lpszData, int nStartPos, int nPreBreak, int nSufBreak)
{
	if((lpszData == NULL)
		||(nStartPos < 2)
		||(nPreBreak < 1)
		||(nSufBreak < 3))
	{
		ASSERT(FALSE);
		return -1;
	}

	if((nStartPos - nPreBreak) < (nSufBreak - nStartPos))
	{
		switch(lpszData[nPreBreak - 1])
		{
		case _T(';'):	case _T(','):	case _T('.'):	case _T('?'):	case _T('!'):
			return nPreBreak;
		default:
			switch(lpszData[nSufBreak - 1])
			{
			case _T(';'):	case _T(','):	case _T('.'):	case _T('?'):	case _T('!'):
				return nSufBreak;
			default:
				return nPreBreak;
			}
			break;
		}
	}
	else
	{
		switch(lpszData[nSufBreak - 1])
		{
		case _T(';'):	case _T(','):	case _T('.'):	case _T('?'):	case _T('!'):
			return nSufBreak;
		default:
			switch(lpszData[nPreBreak - 1])
			{
			case _T(';'):	case _T(','):	case _T('.'):	case _T('?'):	case _T('!'):
				return nPreBreak;
			default:
				return nSufBreak;
			}
			break;
		}
	}
}

void CTitleHelper::RemoveAssCode(CString& strSentence)
{
	int nLen = strSentence.GetLength();
	LPTSTR lpszData = strSentence.GetBuffer();
	stack<TCHAR> stkCC;
	int iPos = nLen - 1;
	int nFlag = 0;
	while(iPos > -1)
	{
		switch(lpszData[iPos])
		{
		// prefix
		case _T('{'):	if(nFlag == 3)nFlag = 0;	iPos -- ;continue;
		// suffix
		case _T('}'):	if(nFlag == 0)nFlag = 3;	iPos -- ;continue;
		default:
			break;
		}
		if(nFlag > 0)
		{
			iPos -- ;
			continue;
		}
		stkCC.push(lpszData[iPos]);
		iPos -- ;
	}

	int nSize = stkCC.size();
	for(int iCC = 0; iCC < nSize; iCC ++)
	{
		lpszData[iCC] = stkCC.top();
		stkCC.pop();
	}

	strSentence.ReleaseBufferSetLength(nSize);
	strSentence = strSentence.Trim(_T("* "));

	lpszData = strSentence.GetBuffer();
	if((nSize == 2)&&((lpszData[0] == (TCHAR)0xA1)&&(lpszData[1] == (TCHAR)0xF6)))
	{
		lpszData[0] = 0x20;
		lpszData[1] = 0x20;
	}
	strSentence.ReleaseBuffer();

	strSentence = strSentence.Trim(_T("* "));
}

void CTitleHelper::RemoveDupSpace(CString& strSentence)
{
	int nLen = strSentence.GetLength();
	LPTSTR lpszData = strSentence.GetBuffer();
	stack<TCHAR> stkCC;
	int iPos = nLen - 1;
	while(iPos > -1)
	{
		if((lpszData[iPos] == _T(' '))
			&&(stkCC.top() == _T(' ')))
		{
			iPos -- ;
			continue;
		}
		stkCC.push(lpszData[iPos]);
		iPos -- ;
	}

	int nSize = stkCC.size();
	for(int iCC = 0; iCC < nSize; iCC ++)
	{
		lpszData[iCC] = stkCC.top();
		stkCC.pop();
	}
	strSentence.ReleaseBufferSetLength(nSize);
}

void CTitleHelper::CCSentence(CString& strSentence)
{
	int nLen = strSentence.GetLength();
	LPTSTR lpszData = strSentence.GetBuffer();
	stack<TCHAR> stkCC;
	int iPos = nLen - 1;
	int nFlag = 0;
	while(iPos > -1)
	{
		switch(lpszData[iPos])
		{
		// prefix
		case _T('('):	if(nFlag == 1)nFlag = 0;	iPos -- ;continue;
		case _T('['):	if(nFlag == 2)nFlag = 0;	iPos -- ;continue;
		case _T('{'):	if(nFlag == 3)nFlag = 0;	iPos -- ;continue;
		case _T('<'):	if(nFlag == 4)nFlag = 0;	iPos -- ;continue;
		// suffix
		case _T(')'):	if(nFlag == 0)nFlag = 1;	iPos -- ;continue;
		case _T(']'):	if(nFlag == 0)nFlag = 2;	iPos -- ;continue;
		case _T('}'):	if(nFlag == 0)nFlag = 3;	iPos -- ;continue;
		case _T('>'):	if(nFlag == 0)nFlag = 4;	iPos -- ;continue;
		case _T(':'):	nFlag = 100;				break;
		case _T('\r'):
			iPos -- ;
			continue;
		case _T('\n'):
			stkCC.push(_T(' '));
			iPos -- ;
			continue;
		default:
			TCHAR	szTest[4];
			LCID wLCID = MAKELCID(MAKELANGID
					(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
			int nReturn = LCMapString(wLCID, LCMAP_LOWERCASE, lpszData + iPos, 1, szTest, 1);
			if(nReturn < 1)
			{
				lpszData[iPos] = _T(' ');
			}
			break;
		}
		if(nFlag == 100)
			break;
		if(nFlag > 0)
		{
			iPos -- ;
			continue;
		}
		stkCC.push(lpszData[iPos]);
		iPos -- ;
	}

	while((stkCC.size() > 0)
		&&(stkCC.top() == _T(' ')))
		stkCC.pop();

	int nSize = stkCC.size();
	for(int iCC = 0; iCC < nSize; iCC ++)
	{
		lpszData[iCC] = stkCC.top();
		stkCC.pop();
	}
	strSentence.ReleaseBufferSetLength(nSize);
	strSentence = strSentence.Trim(_T("* "));
	if(!CTitleHelper::IsDBCSString(strSentence))
	{
		strSentence.MakeLower();
		CString strChar = strSentence.Left(1);
		strChar.MakeUpper();
		strSentence.GetBuffer()[0] = strChar[0];
		strSentence.ReleaseBuffer();
	}
}

BOOL CTitleHelper::ReplaceNoCase(CString& strSrc, const CString& strTarget, const CString& strResult)
{
	CString strTmp = strSrc;
	CString strTmp2;
	LPTSTR lpszData = strTmp.GetBuffer();
	LPTSTR lpszTag = StrStrI(lpszData, strTarget);
	if(lpszTag != NULL)
	{
		strSrc = strTmp.Left(lpszTag - lpszData);
		strSrc += strResult;
		strTmp2 = strTmp.Right(strTmp.GetLength() - (lpszTag - lpszData) - strTarget.GetLength());
		ReplaceNoCase(strTmp2, strTarget, strResult);
		strTmp.ReleaseBuffer();
		strSrc += strTmp2;
		return TRUE;
	}
	strTmp.ReleaseBuffer();
	return FALSE;
}

BOOL CTitleHelper::ReplaceTitle(PTITLE_UNIT pUnit, const CString& strTarget, const CString& strResult,
							BOOL bCase, BOOL bEng, BOOL bChs)
{
	CString strUpLine;
	CString strDnLine;
	BOOL bModified;

	BreakString(pUnit->content, strUpLine, strDnLine);

	if(bChs)
	{
		if(bCase)
		{
			bModified = strUpLine.Replace(strTarget, strResult);
		}
		else
		{
			bModified = CTitleHelper::ReplaceNoCase(strUpLine, strTarget, strResult);
		}
	}
	if(bEng)
	{
		if(bCase)
		{
			bModified = strDnLine.Replace(strTarget, strResult);
		}
		else
		{
			bModified = CTitleHelper::ReplaceNoCase(strDnLine, strTarget, strResult);
		}
	}
	pUnit->content = strUpLine;
	pUnit->content += _T("\r\n");
	pUnit->content += strDnLine;
	return bModified;
}

BOOL CTitleHelper::BreakTitleUnit(int nPos, PTITLE_UNIT pUnit1, PTITLE_UNIT pUnit2)
{
	int nLen = pUnit1->content.GetLength();
	LPTSTR lpszData = pUnit1->content.GetBuffer();
	int nPreBreak = -1;
	int nSufBreak = -1;
	for(int iStep = 0; iStep < nPos; iStep ++)
	{
		if((nPreBreak == -1)
			&&(CTitleHelper::IsWordBreak(lpszData[nPos - 1 - iStep])))
			nPreBreak = nPos - 1 - iStep;

		if((nSufBreak == -1)
			&&(CTitleHelper::IsWordBreak(lpszData[nPos + iStep])))
			nSufBreak = nPos + iStep;

		if((nPreBreak > -1)&&(nSufBreak > -1))
			break;
	}
	if((nPreBreak < 1)
		||(nSufBreak == (nLen - 1))
		||(nSufBreak == -1))
		return FALSE;

	int nPriorityPos = CTitleHelper::SelectPriorityChar(lpszData, nPos, nPreBreak, nSufBreak);
	if(nPriorityPos > -1)
	{
		LPTSTR lpszData2 = pUnit2->content.GetBufferSetLength( nLen - nPriorityPos - 1);
		CopyMemory(lpszData2, lpszData + nPriorityPos + 1, nLen - nPriorityPos - 1);
		pUnit2->content.ReleaseBufferSetLength(nLen - nPriorityPos - 1);
		pUnit1->content.ReleaseBufferSetLength(nPriorityPos);

		int nTimeLen = pUnit1->nEnd - pUnit1->nStart;
		int nLen1 = pUnit1->content.GetLength();
		int nLen2 = pUnit2->content.GetLength();
		pUnit2->nEnd = pUnit1->nEnd;
		pUnit1->nEnd = pUnit1->nStart + (nTimeLen*nLen1/(nLen1 + nLen2));
		pUnit2->nStart = pUnit1->nEnd + 1;
		pUnit2->dwFlags	= 0;

		return TRUE;
	}
	else
	{
		pUnit1->content.ReleaseBuffer();
		return FALSE;
	}
}

BOOL CTitleHelper::BreakUnitByShare(int nShare, PTITLE_UNIT pUnit1, PTITLE_UNIT pUnit2)
{
	if( nShare < 2)
		return FALSE;
	int nLen = pUnit1->content.GetLength();
	int nPos = nLen / nShare;

	return BreakTitleUnit(nPos, pUnit1, pUnit2);
}

BOOL CTitleHelper::BreakUnitByTimeSpin(int nTimeSpin, PTITLE_UNIT pUnit1, PTITLE_UNIT pUnit2)
{
	if(( pUnit1->nEnd - pUnit1->nStart < nTimeSpin)
		||(nTimeSpin < 0))
		return FALSE;
	int nLen = pUnit1->content.GetLength();
	int nPos = nLen * nTimeSpin/ (pUnit1->nEnd - pUnit1->nStart);

	return BreakTitleUnit(nPos, pUnit1, pUnit2);
}

BOOL CTitleHelper::BreakString(const CString& strSrc, CString& strFirstLine, CString& strOtherLine)
{
	int nLineEnd = strSrc.Find(_T("\r\n"));
	if(nLineEnd == -1)
	{
		strFirstLine = strSrc;
	}
	else
	{
		strFirstLine = strSrc.Left(nLineEnd);
		strOtherLine = strSrc.Right(strSrc.GetLength() - nLineEnd - 2);
	}
	return TRUE;
}

typedef struct tagMapCNChar
{
	WCHAR	chCn;
	WCHAR	chEn;
}CHAR_MAPCN, *LPCHAR_MAPCN;

BOOL CTitleHelper::IsDBCSString(CString& strTag)
{
	CHAR_MAPCN aMap[] =
	{
		{L'£°'	, L'0'}, {L'£±'	, L'1'}, {L'£²'	, L'2'}, {L'£³'	, L'3'}, {L'£´'	, L'4'},
		{L'£µ'	, L'5'}, {L'£¶'	, L'6'}, {L'£·'	, L'7'}, {L'£¸'	, L'8'}, {L'£¹'	, L'9'},
		{L'£¿'	, L'?'}, {L'£¡'	, L'!'}, {L'£¬'	, L','}, {L'¡£'	, L'.'}, {L'£º'	, L':'},
		{L'¡°'	, L'"'}, {L'¡°'	, L'"'}, {L'£»'	, L';'}, {L'£¼'	, L'<'}, {L'£¾'	, L'>'},
		{L'£À'	, L'@'}, {L'££'	, L'#'}, {L'£¤'	, L'$'}, {L'£¥'	, L'%'}, {L'£¦'	, L'&'},
		{L'¡Á'	, L'*'}, {L'£¨'	, L'('}, {L'£©'	, L')'}, {L'¡ª¡ª'	, L'_'}, {L'£«'	, L'+'},
		{L'£ü'	, L'|'}, {L'£Ü'	, L'\\'},{L'£½'	, L'='}, {L'£­'	, L'-'}, {L'£à'	, L'`'},
		{L'¡«'	, L'~'}, {L'£Û'	, L'['}, {L'£Ý'	, L']'}, {L'£û'	, L'{'}, {L'£ý'	, L'}'},
		{L'¦ä'	, L'^'}, 
		{L'\0'	, L'\0'},
	};
	CString strTmp = strTag;
	int iChar = 0;
	while(aMap[iChar].chCn != L'\0')
	{
		strTmp.Replace(aMap[iChar].chCn, aMap[iChar].chEn);
		iChar ++;
	}

	int nASize = WideCharToMultiByte(CP_ACP, 0,
		strTmp, strTmp.GetLength(),
		NULL, 0, NULL, NULL);

/*	int nRet = IS_TEXT_UNICODE_ASCII16;
	if(IsTextUnicode((LPCTSTR)strTag, strTag.GetLength()*sizeof(TCHAR), &nRet))*/
	if(nASize == strTmp.GetLength())
		return FALSE;
	return TRUE;
}

BOOL CTitleHelper::BreakStringEachLine(LPCTSTR lpszSrc, CStringArray& astrLines)
{
	CString strLine;
	CString strRemain = lpszSrc;
	int nBreak = -1;

	nBreak = strRemain.Find(_T("\r\n"));
	while(nBreak != -1)
	{
		strLine = strRemain.Left(nBreak);
		astrLines.Add(strLine);

		strRemain = strRemain.Right(strRemain.GetLength() - nBreak - 2);
		nBreak = strRemain.Find(_T("\r\n"));
	}
	if(!strRemain.IsEmpty())
		astrLines.Add(strRemain);

	return TRUE;
}

int CTitleHelper::GetCountTitle(const CString& strTitle)
{
	CString strTag = strTitle;

	int nCount = 0;
	while(strTag.GetLength() > 0)
	{
		int nEnterPos = strTag.Find(_T("\r\n"));
		if(nEnterPos == -1)
		{
			nCount ++;
			break;
		}
		else
		{
			nCount ++;
			strTag = strTag.Right(strTag.GetLength() - nEnterPos - 2);
		}
	}
	return nCount;
}

int CTitleHelper::CountChsChar(const CString& strTitle)
{
	int nCnt = 0;
	LPCTSTR lpszData = strTitle.GetString();
	int nStrLen = strTitle.GetLength();
	int iChar = 0;
	while(iChar < nStrLen)
	{
		int nASize = WideCharToMultiByte(CP_ACP, 0,
			lpszData + iChar, 1,
			NULL, 0, NULL, NULL);

		if(nASize == 2)
		{
			nCnt ++;
		}
		iChar ++;
	}
	return nCnt;
}

int CTitleHelper::CountTitleLine(const CString& strTitle, CString& strCount)
{
	int nCnt = 0;
	CString strTmp;
	CString strTag = strTitle;
	strCount.Empty();

	while(strTag.GetLength() > 0)
	{
		int nEnterPos = strTag.Find(_T("\r\n"));
		if(nEnterPos == -1)
		{
			strTmp.Format(_T("%d\r\n"), strTag.GetLength());
			strCount += strTmp;
			nCnt ++;
			break;
		}
		else
		{
			strTmp.Format(_T("%d\r\n"), nEnterPos);
			strCount += strTmp;
			nCnt ++;
			strTag = strTag.Right(strTag.GetLength() - nEnterPos - 2);
		}
	}
	return nCnt;
}

PTITLE_UNIT	CTitleHelper::GetTitleItem(int nPos, VT_TITLE& vtTitle)
{
	if((nPos < 0)
		||(nPos >= (int)vtTitle.size()))
		return NULL;
	return vtTitle.at(nPos);
}
