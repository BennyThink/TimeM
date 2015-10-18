#include "StdAfx.h"
#include "resource.h"
#include "TitleFile.h"
#include "TitleHelper.h"

CTitleFile::CTitleFile(void)
{
}

CTitleFile::~CTitleFile(void)
{
}

BOOL CTitleFile::IsLineEnd(PBUF_READ pReadData)
{
	LPBYTE pData = pReadData->bufFile + pReadData->dwBufOffset;

	if(pReadData->nUnicodeState == 0)
	{
		if(CSTR_EQUAL == CompareStringA(LOCALE_USER_DEFAULT, 0, (LPCSTR)pData, 2, "\r\n", 2))
			return TRUE;
	}
	else
	{
		if(CSTR_EQUAL == CompareStringW(LOCALE_USER_DEFAULT, 0, (LPCWSTR)pData, 2, L"\r\n", 2))
			return TRUE;
	}

	return FALSE;
}

void CTitleFile::StepOffset(PBUF_READ pReadData)
{
	if(0 == pReadData->nUnicodeState)
		pReadData->dwBufOffset += 2;
	else
		pReadData->dwBufOffset += 4;
}

BOOL CTitleFile::ReadSubStart(PBUF_READ pReadData, PTITLE_UNIT pUnit)
{
	StepInvalidChar(pReadData);
	if(pReadData->dwBufOffset < pReadData->dwBufSize)
	{
		LPBYTE pData = pReadData->bufFile + pReadData->dwBufOffset;
		if(pReadData->nUnicodeState == 0)
		{
			CHAR	szTime[16];
			LPSTR pAData = (LPSTR)pData;
			if((pAData[2] == ':')&&
				(pAData[5] == ':')&&
				(pAData[8] == ',')&&
				(CSTR_EQUAL == CompareStringA(LOCALE_USER_DEFAULT, 0, pAData + 12, 5, " --> ", 5))&&
				(pAData[19] == ':')&&
				(pAData[22] == ':')&&
				(pAData[25] == ','))
			{
				ZeroMemory(szTime, 16*sizeof(CHAR));
				CopyMemory(szTime, pAData, 12*sizeof(CHAR));

				WCHAR szWTime[16];
				int nCntCvt = MultiByteToWideChar(CP_ACP, 0, szTime, 16, szWTime, 16);
				if(nCntCvt == 16)
					pUnit->nStart = CTitleHelper::GetTimeValue(CString(szWTime));

				pReadData->dwBufOffset += 17*sizeof(CHAR);
				return TRUE;
			}
		}
		else
		{
			WCHAR	szTime[16];
			LPWSTR pWData = (LPWSTR)pData;
			if((pWData[2] == L':')&&
				(pWData[5] == L':')&&
				(pWData[8] == L',')&&
				(CSTR_EQUAL == CompareStringW(LOCALE_USER_DEFAULT, 0, pWData + 12, 5, L" --> ", 5))&&
				(pWData[19] == L':')&&
				(pWData[22] == L':')&&
				(pWData[25] == L','))
			{
				ZeroMemory(szTime, 16*sizeof(WCHAR));
				CopyMemory(szTime, pWData, 12*sizeof(WCHAR));
				pUnit->nStart = CTitleHelper::GetTimeValue(CString(szTime));

				pReadData->dwBufOffset += 17*sizeof(WCHAR);
				return TRUE;
			}
		}

	}
	return FALSE;
}

BOOL CTitleFile::ReadSubEnd(PBUF_READ pReadData, PTITLE_UNIT pUnit)
{
	StepInvalidChar(pReadData);
	if(pReadData->dwBufOffset < pReadData->dwBufSize)
	{
		LPBYTE pData = pReadData->bufFile + pReadData->dwBufOffset;
		if(pReadData->nUnicodeState == 0)
		{
			LPSTR pAData = (LPSTR)pData;
			CHAR	szTime[16];
			if((pAData[2] == ':')&&
				(pAData[5] == ':')&&
				(pAData[8] == ','))
			{
				ZeroMemory(szTime, 16*sizeof(CHAR));
				CopyMemory(szTime, pAData, 12*sizeof(CHAR));

				WCHAR szWTime[16];
				int nCntCvt = MultiByteToWideChar(CP_ACP, 0, szTime, 16, szWTime, 16);
				if(nCntCvt == 16)
					pUnit->nEnd = CTitleHelper::GetTimeValue(CString(szWTime));

				pReadData->dwBufOffset += 14*sizeof(CHAR);
				return TRUE;
			}
		}
		else
		{
			LPWSTR pWData = (LPWSTR)pData;
			WCHAR	szTime[16];
			if((pWData[2] == L':')&&
				(pWData[5] == L':')&&
				(pWData[8] == L','))
			{
				ZeroMemory(szTime, 16*sizeof(WCHAR));
				CopyMemory(szTime, pWData, 12*sizeof(WCHAR));

				pUnit->nEnd = CTitleHelper::GetTimeValue(CString(szTime));

				pReadData->dwBufOffset += 14*sizeof(WCHAR);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CTitleFile::ReadSubTitle(PBUF_READ pReadData, PTITLE_UNIT pUnit)
{
	StepSpaceChar(pReadData);
	if(pReadData->dwBufOffset < pReadData->dwBufSize)
	{
		LPBYTE pData = pReadData->bufFile + pReadData->dwBufOffset;
		if(pReadData->nUnicodeState == 0)
		{
			LPSTR pAData = (LPSTR)pData;

			LPCSTR pEnter = StrStrA(pAData, "\r\n");
			if(pEnter == pAData)
			{
				pReadData->dwBufOffset += 2*sizeof(CHAR);
				return TRUE;
			}

			while((pEnter != NULL)
				&&(CSTR_EQUAL != CompareStringA(LOCALE_USER_DEFAULT, 0, pEnter, 4, "\r\n\r\n", 4)))
			{
				pEnter = StrStrA(pEnter + 1*sizeof(CHAR), "\r\n");
			}

			ULONG nLen = 0;
			if(NULL == pEnter)
			{
				nLen = (ULONG)((LPSTR)(pReadData->bufFile + pReadData->dwBufSize) - pAData);
			}
			else
				nLen = (ULONG)(pEnter - pAData);

			int nWSize = MultiByteToWideChar(CP_ACP, 0, pAData, nLen, NULL, 0);
			if(nWSize > 0)
			{
				MultiByteToWideChar(CP_ACP, 0, pAData, nLen, pUnit->content.GetBufferSetLength(nWSize), nWSize);
				pUnit->content.ReleaseBuffer();
			}

			pReadData->dwBufOffset += (nLen + 4)*sizeof(CHAR);
		}
		else
		{
			LPWSTR pWData = (LPWSTR)pData;

			LPCWSTR pEnter = StrStrW(pWData, L"\r\n");
			if(pEnter == pWData)
			{
				pReadData->dwBufOffset += 2*sizeof(WCHAR);
				return TRUE;
			}

			while((pEnter != NULL)
				&&(CSTR_EQUAL != CompareStringW(LOCALE_USER_DEFAULT, 0, pEnter, 4, L"\r\n\r\n", 4)))
			{
				pEnter = StrStrW(pEnter + 1*sizeof(WCHAR), L"\r\n");
			}

			ULONG nLen = 0;
			if(NULL == pEnter)
			{
				nLen = (ULONG)((LPWSTR)(pReadData->bufFile + pReadData->dwBufSize) - pWData);
			}
			else
				nLen = (ULONG)(pEnter - pWData);

			LPWSTR pBuf = pUnit->content.GetBufferSetLength(nLen);
			CopyMemory(pBuf, pWData, nLen*sizeof(WCHAR));
			pUnit->content.ReleaseBuffer();

			pReadData->dwBufOffset += (nLen + 4)*sizeof(WCHAR);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CTitleFile::ReadSubTitleByLine(PBUF_READ pReadData, PTITLE_UNIT pUnit)
{
	LPBYTE pData = pReadData->bufFile + pReadData->dwBufOffset;
	if(pReadData->nUnicodeState == 0)
	{
		LPSTR pAData = (LPSTR)pData;
		LPSTR pEnter = StrStrA(pAData, "\r\n");
		if(pEnter == pAData)
		{
			pReadData->dwBufOffset += 2*sizeof(CHAR);
			return TRUE;
		}
		if(pEnter == NULL)
			return FALSE;

		ULONG nLen = (ULONG)(pEnter - pAData);

		int nWSize = MultiByteToWideChar(CP_ACP, 0, pAData, nLen, NULL, 0);
		if(nWSize > 0)
		{
			MultiByteToWideChar(CP_ACP, 0, pAData, nLen, pUnit->content.GetBufferSetLength(nWSize), nWSize);
			pUnit->content.ReleaseBuffer();
		}
		pReadData->dwBufOffset += (nLen + 2)*sizeof(CHAR);
	}
	else
	{
		LPWSTR pWData = (LPWSTR)pData;
		LPWSTR pEnter = StrStrW(pWData, L"\r\n");
		if(pEnter == pWData)
		{
			pReadData->dwBufOffset += 2*sizeof(WCHAR);
			return TRUE;
		}
		if(pEnter == NULL)
			return FALSE;

		ULONG nLen = (ULONG)(pEnter - pWData);

		LPWSTR pBuf = pUnit->content.GetBufferSetLength(nLen);
		CopyMemory(pBuf, pWData, nLen*sizeof(WCHAR));
		pUnit->content.ReleaseBuffer();
		pReadData->dwBufOffset += (nLen + 2)*sizeof(WCHAR);
	}
	return TRUE;
}

BOOL CTitleFile::ReadSSALine(PBUF_READ pReadData, CString& strLine)
{
	LPBYTE pData = pReadData->bufFile + pReadData->dwBufOffset;
	if(pReadData->nUnicodeState == 0)
	{
		LPSTR pAData = (LPSTR)pData;
		LPSTR pHdr = StrStrIA(pAData, "Dialogue:");
		if(pHdr == NULL)
		{
			return FALSE;
		}
		LPSTR pEnter = StrStrIA(pHdr, "\r\n");
		if(pEnter == NULL)
		{
			pEnter = (LPSTR)(pReadData->bufFile + pReadData->dwBufSize);
		}

		pReadData->dwBufOffset += (pHdr - pAData)*sizeof(CHAR);
		ULONG nLen = (ULONG)(pEnter - pHdr);

		int nWSize = MultiByteToWideChar(CP_ACP, 0, pHdr, nLen, NULL, 0);
		if(nWSize > 0)
		{
			MultiByteToWideChar(CP_ACP, 0, pHdr, nLen, strLine.GetBufferSetLength(nWSize), nWSize);
			strLine.ReleaseBuffer();
		}
		pReadData->dwBufOffset += (nLen + 2)*sizeof(CHAR);
	}
	else
	{
		LPWSTR pWData = (LPWSTR)pData;
		LPWSTR pHdr = StrStrIW(pWData, L"Dialogue:");
		if(pHdr == NULL)
		{
			return FALSE;
		}
		LPWSTR pEnter = StrStrIW(pHdr, L"\r\n");
		if(pEnter == NULL)
		{
			pEnter = (LPWSTR)(pReadData->bufFile + pReadData->dwBufSize);
		}

		pReadData->dwBufOffset += (pHdr - pWData)*sizeof(WCHAR);
		ULONG nLen = (ULONG)(pEnter - pHdr);

		LPWSTR pBuf = strLine.GetBufferSetLength(nLen);
		CopyMemory(pBuf, pHdr, nLen*sizeof(WCHAR));
		strLine.ReleaseBuffer();
		pReadData->dwBufOffset += (nLen + 2)*sizeof(WCHAR);
	}
	return TRUE;
}

PTITLE_UNIT CTitleFile::StepTransUnit(PBUF_READ pReadData, BOOL bByLine)
{
	PTITLE_UNIT pUnit = new TITLE_UNIT;
	InitializeUnit(pUnit);

	while(CTitleFile::IsLineEnd(pReadData))
		CTitleFile::StepOffset(pReadData);
	if(bByLine)
	{
		if(CTitleFile::ReadSubTitleByLine(pReadData, pUnit))
		{
			return pUnit;
		}
	}
	else if(CTitleFile::ReadSubTitle(pReadData, pUnit))
	{
		return pUnit;
	}
	delete pUnit;
	return NULL;
}

void CTitleFile::StepSpaceChar(PBUF_READ pReadData)
{
	LPBYTE pData = pReadData->bufFile + pReadData->dwBufOffset;
	if(pReadData->nUnicodeState == 0)
	{
		LPSTR pAData = (LPSTR)pData;
		while(pAData[0] == ' ')
		{
			pAData ++;
			pReadData->dwBufOffset += sizeof(CHAR);
		}
	}
	else
	{
		LPWSTR pWData = (LPWSTR)pData;
		while(pWData[0] == L' ')
		{
			pWData ++;
			pReadData->dwBufOffset += sizeof(TCHAR);
		}
	}
}

void CTitleFile::StepInvalidChar(PBUF_READ pReadData)
{
	LPBYTE pData = pReadData->bufFile + pReadData->dwBufOffset;
	if(pReadData->nUnicodeState == 0)
	{
		LPSTR pAData = (LPSTR)pData;
		while((pAData[0] == ' ')
			||(pAData[0] == '\r')
			||(pAData[0] == '\n'))
		{
			pAData ++;
			pReadData->dwBufOffset += sizeof(CHAR);
		}
	}
	else
	{
		LPWSTR pWData = (LPWSTR)pData;
		while((pWData[0] == L' ')
			||(pWData[0] == L'\r')
			||(pWData[0] == L'\n'))
		{
			pWData ++;
			pReadData->dwBufOffset += sizeof(TCHAR);
		}
	}
}

void CTitleFile::StepALine(PBUF_READ pReadData)
{
	LPBYTE pData = pReadData->bufFile + pReadData->dwBufOffset;
	if(pReadData->nUnicodeState == 0)
	{
		LPSTR pAData = (LPSTR)pData;
		LPSTR pEnter = StrStrA(pAData, "\r\n");
		if(pEnter == pAData)
		{
			pReadData->dwBufOffset += 2*sizeof(CHAR);
			return;
		}
		if(pEnter == NULL)
		{
			pReadData->dwBufOffset = pReadData->dwBufSize;
			return;
		}

		ULONG nLen = (ULONG)(pEnter - pAData);
		pReadData->dwBufOffset += (nLen + 2)*sizeof(CHAR);
	}
	else
	{
		LPWSTR pWData = (LPWSTR)pData;
		LPWSTR pEnter = StrStrW(pWData, L"\r\n");
		if(pEnter == pWData)
		{
			pReadData->dwBufOffset += 2*sizeof(WCHAR);
			return;
		}
		if(pEnter == NULL)
		{
			pReadData->dwBufOffset = pReadData->dwBufSize;
			return;
		}

		ULONG nLen = (ULONG)(pEnter - pWData);
		pReadData->dwBufOffset += (nLen + 2)*sizeof(WCHAR);
	}
}

PTITLE_UNIT CTitleFile::StepTitleUnit(PBUF_READ pReadData)
{
	while(CTitleFile::IsLineEnd(pReadData))
		CTitleFile::StepOffset(pReadData);
	PTITLE_UNIT pUnit = new TITLE_UNIT;
	InitializeUnit(pUnit);

	CString strLine;
	switch(pReadData->dwFmtFlags)
	{
	case FMTFLAG_SRT:
	case FMTFLAG_UNKNOWN:
		while(pReadData->dwBufOffset < pReadData->dwBufSize)
		{
			if(CTitleFile::ReadSubStart(pReadData, pUnit))
				break;
			StepALine(pReadData);
		}
		if(pReadData->dwBufOffset < pReadData->dwBufSize)
		{
			if(CTitleFile::ReadSubEnd(pReadData, pUnit)&&
				CTitleFile::ReadSubTitle(pReadData, pUnit))
			{
				pUnit->dwFlags	= 0;
				return pUnit;
			}
		}
		break;
	case FMTFLAG_SSA:
	case FMTFLAG_ASS:
		if(CTitleFile::ReadSSALine(pReadData, strLine)&&
			CTitleFile::FillSSAUnit(strLine, pUnit))
		{
			pUnit->dwFlags	= 0;
			return pUnit;
		}
		break;
	}
	delete pUnit;
	return NULL;
}

LPTSTR StepSplit(LPTSTR pStart, LPCTSTR pszTag, CString& strData)
{
	LPTSTR pEnd = NULL;
	pEnd = StrStr(pStart, pszTag);
	if(NULL != pEnd)
	{
		strData = pStart;
		strData.GetBuffer();
		strData.ReleaseBufferSetLength(pEnd - pStart);
	}
	return pEnd;
}

BOOL CTitleFile::FillSSAUnit(CString& strLine, PTITLE_UNIT pUnit)
{
	LPTSTR pStart = NULL;

	CString strData;
	pStart = strLine.GetBuffer();
	pStart = StepSplit(pStart, _T(":"), strData);
	if(NULL == pStart)
		return FALSE;
	// Layer
	pStart ++;
	pStart = StepSplit(pStart, _T(","), strData);
	if(NULL == pStart)
		return FALSE;
	pUnit->strLayer = strData;

	// Start
	pStart ++;
	pStart = StepSplit(pStart, _T(","), strData);
	if(NULL == pStart)
		return FALSE;
	pUnit->nStart = CTitleHelper::GetTimeValue(strData);;

	// End
	pStart ++;
	pStart = StepSplit(pStart, _T(","), strData);
	if(NULL == pStart)
		return FALSE;
	pUnit->nEnd = CTitleHelper::GetTimeValue(strData);;

	// Style
	pStart ++;
	pStart = StepSplit(pStart, _T(","), strData);
	if(NULL == pStart)
		return FALSE;
	pUnit->strStyle = strData;

	// Actor
	pStart ++;
	pStart = StepSplit(pStart, _T(","), strData);
	if(NULL == pStart)
		return FALSE;
	pUnit->strActor = strData;

	// MarginL
	pStart ++;
	pStart = StepSplit(pStart, _T(","), strData);
	if(NULL == pStart)
		return FALSE;
	pUnit->strMarginL = strData;

	// MarginR
	pStart ++;
	pStart = StepSplit(pStart, _T(","), strData);
	if(NULL == pStart)
		return FALSE;
	pUnit->strMarginR = strData;

	// MarginV
	pStart ++;
	pStart = StepSplit(pStart, _T(","), strData);
	if(NULL == pStart)
		return FALSE;
	pUnit->strMarginV = strData;

	// Effect
	pStart ++;
	pStart = StepSplit(pStart, _T(","), strData);
	if(NULL == pStart)
		return FALSE;
	pUnit->strEffect = strData;

	// Text
	pStart ++;
	pUnit->content = pStart;
	pUnit->content.Replace(_T("\\N"), _T("\r\n"));

	strLine.ReleaseBuffer();
	return TRUE;
}

BOOL CTitleFile::PrepareRead(PBUF_READ pReadData, CFile* pFile)
{
	ZeroMemory(pReadData, sizeof(BUF_READ));
	ULONGLONG nFileLen = pFile->GetLength();
	if(nFileLen > FILE_BUFMAX)
	{
		AfxMessageBox(IDS_TOOLARGEFILE);
		return FALSE;
	}
	try
	{
		pReadData->dwBufSize		= (DWORD)nFileLen;
		pReadData->bufFile			= new BYTE[pReadData->dwBufSize];
		pReadData->dwBufOffset		= 0;
		pReadData->nUnicodeState	= 0;
		pReadData->dwFmtFlags		= FMTFLAG_SRT;
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}

void CTitleFile::FillBuffer(PBUF_READ pReadData, CFile* pFile)
{
	ZeroMemory(pReadData->bufFile, pReadData->dwBufSize);
	UINT nRead = pFile->Read(pReadData->bufFile, pReadData->dwBufSize);
	pReadData->nUnicodeState = 0;
	pReadData->dwBufOffset = 0;
}

void CTitleFile::ClearReadBuf(PBUF_READ pReadData)
{
	if(pReadData->bufFile != NULL)
	{
		delete [] pReadData->bufFile;
		pReadData->bufFile = NULL;
	}
	pReadData->dwBufSize	= 0;
}

CString CTitleFile::GetSSAHdr(LPCTSTR lpszData, UINT& nEventOffset, BOOL bAdvanced)
{
	CString strHdr;
	CString strEvents;
	CString strInfo = CTitleFile::GetSSASection(CString(lpszData), _T("[Script Info]"));
	CString strV4Style = CTitleFile::GetSSASection(CString(lpszData), bAdvanced?_T("[V4+ Styles]"):_T("[V4 Styles]"));
	LPTSTR pEvent = StrStrI(lpszData, _T("[Events]"));
	if(pEvent != NULL)
	{
		LPTSTR pEventStart = StrStrI(pEvent, _T("Format:"));
		if(pEventStart != NULL)
		{
			LPTSTR pEventEnd = StrStrI(pEventStart, _T("\r\n"));
			if(pEventEnd != NULL)
			{
				nEventOffset = pEventStart - lpszData;
				CopyMemory(strEvents.GetBufferSetLength(pEventEnd - pEventStart),
					pEventStart, (pEventEnd - pEventStart)*sizeof(TCHAR));
				strEvents.ReleaseBuffer();
			}
		}
	}
	strHdr += _T("[Script Info]\r\n");
	strHdr += strInfo;
	strHdr += L"\r\n";
	strHdr += L"\r\n";
	strHdr += bAdvanced?_T("[V4+ Styles]\r\n"):_T("[V4 Styles]\r\n");
	strHdr += strV4Style;
	strHdr += L"\r\n";
	strHdr += L"\r\n";
	strHdr += _T("[Events]\r\n");
	strHdr += strEvents;
	strHdr += _T("\r\n");
	return strHdr;
}

CStringW CTitleFile::GetSSAHdrA(LPCSTR lpszData, UINT& nEventOffset, BOOL bAdvanced)
{
	CStringW strHdr;
	CStringA strHdrA;
	CStringA strEventsA;
	CStringA strInfoA = CTitleFile::GetSSASection(CStringA(lpszData), "[Script Info]");
	CStringA strV4StyleA = CTitleFile::GetSSASection(CStringA(lpszData), bAdvanced?"[V4+ Styles]":"[V4 Styles]");
	LPSTR pEvent = StrStrIA(lpszData, "[Events]");
	if(pEvent != NULL)
	{
		LPSTR pEventStart = StrStrIA(pEvent, "Format:");
		if(pEventStart != NULL)
		{
			LPSTR pEventEnd = StrStrIA(pEventStart, "\r\n");
			if(pEventEnd != NULL)
			{
				nEventOffset = pEventStart - lpszData;
				memcpy(strEventsA.GetBufferSetLength(pEventEnd - pEventStart),
					pEventStart, (pEventEnd - pEventStart)*sizeof(CHAR));
				strEventsA.ReleaseBuffer();
			}
		}
	}
	strHdrA += "[Script Info]\r\n";
	strHdrA += strInfoA;
	strHdrA += "\r\n";
	strHdrA += "\r\n";
	strHdrA += bAdvanced?"[V4+ Styles]\r\n":"[V4 Styles]\r\n";
	strHdrA += strV4StyleA;
	strHdrA += "\r\n";
	strHdrA += "\r\n";
	strHdrA += "[Events]\r\n";
	strHdrA += strEventsA;
	strHdrA += "\r\n";

	int nASize = MultiByteToWideChar(CP_ACP, 0,
		strHdrA, strHdrA.GetLength(),
		NULL, 0);
	if(nASize > 0)
	{
		MultiByteToWideChar(CP_ACP, 0,
			strHdrA, strHdrA.GetLength(),
			strHdr.GetBufferSetLength(nASize), nASize);
		strHdr.ReleaseBuffer();
	}
	return strHdr;
}

CStringA CTitleFile::GetSSASection(CStringA strSrc, CStringA strTag)
{
	CStringA strRet;
	int nStart = strSrc.Find(strTag);
	if(nStart != -1)
	{
		int nEnd = strSrc.Find("[", nStart + strTag.GetLength());
		if(nEnd == -1)
			nEnd = strSrc.GetLength();
		LPSTR pData = strSrc.GetBuffer();

		int nLen = nEnd - nStart - strTag.GetLength();
		StringCbCopyNA(strRet.GetBufferSetLength(nLen), nLen*sizeof(CHAR), pData + nStart + strTag.GetLength(), nLen*sizeof(CHAR));
	}
	strRet = strRet.TrimLeft("\r\n");
	strRet = strRet.TrimRight("\r\n");
	return strRet;
}

CString CTitleFile::GetSSASection(CString strSrc, CString strTag)
{
	CString strRet;
	int nStart = strSrc.Find(strTag);
	if(nStart != -1)
	{
		int nEnd = strSrc.Find(_T("["), nStart + strTag.GetLength());
		if(nEnd == -1)
			nEnd = strSrc.GetLength();
		LPTSTR pData = strSrc.GetBuffer();

		int nLen = nEnd - nStart - strTag.GetLength();
		StringCbCopyN(strRet.GetBufferSetLength(nLen), nLen*sizeof(TCHAR), pData + nStart + strTag.GetLength(), nLen*sizeof(TCHAR));
	}
	strRet = strRet.TrimLeft(_T("\r\n"));
	strRet = strRet.TrimRight(_T("\r\n"));
	return strRet;
}

/*
#define IS_TEXT_UNICODE_ASCII16               0x0001
#define IS_TEXT_UNICODE_REVERSE_ASCII16       0x0010

#define IS_TEXT_UNICODE_STATISTICS            0x0002
#define IS_TEXT_UNICODE_REVERSE_STATISTICS    0x0020

#define IS_TEXT_UNICODE_CONTROLS              0x0004
#define IS_TEXT_UNICODE_REVERSE_CONTROLS      0x0040

#define IS_TEXT_UNICODE_SIGNATURE             0x0008	// 0xFEFF
#define IS_TEXT_UNICODE_REVERSE_SIGNATURE     0x0080

#define IS_TEXT_UNICODE_ILLEGAL_CHARS         0x0100
#define IS_TEXT_UNICODE_ODD_LENGTH            0x0200
#define IS_TEXT_UNICODE_DBCS_LEADBYTE         0x0400
#define IS_TEXT_UNICODE_NULL_BYTES            0x1000

#define IS_TEXT_UNICODE_UNICODE_MASK          0x000F
#define IS_TEXT_UNICODE_REVERSE_MASK          0x00F0
#define IS_TEXT_UNICODE_NOT_UNICODE_MASK      0x0F00
#define IS_TEXT_UNICODE_NOT_ASCII_MASK        0xF000
*/

UINT CTitleFile::TestUnicodeBuf(LPVOID lpData, int nSize)
{
	int nTestMask = IS_TEXT_UNICODE_UNICODE_MASK|IS_TEXT_UNICODE_REVERSE_MASK;
	BOOL bIsUnicode = IsTextUnicode(lpData, nSize, &nTestMask);

	if((!bIsUnicode)&&(nTestMask == 0))
		return 0;

	UINT nRet = 0;
	if(nTestMask&IS_TEXT_UNICODE_REVERSE_CONTROLS)
		nRet |= READFLAG_UNICODE|READFLAG_REVERSE;
	if(nTestMask&IS_TEXT_UNICODE_REVERSE_SIGNATURE)
		nRet |= READFLAG_UNICODE|READFLAG_REVERSE|READFLAG_SINATURE;
	if(nRet > 0)
		return nRet;

	if(nTestMask&IS_TEXT_UNICODE_CONTROLS)
		nRet |= READFLAG_UNICODE;
	if(nTestMask&IS_TEXT_UNICODE_SIGNATURE)
		nRet |= READFLAG_UNICODE|READFLAG_SINATURE;
	return nRet;
}

void CTitleFile::StepSignature(PBUF_READ pReadData)
{
	if((READFLAG_UNICODE == (pReadData->nUnicodeState&READFLAG_UNICODE))
		&&(READFLAG_SINATURE == (pReadData->nUnicodeState&READFLAG_SINATURE)))
	{
		pReadData->dwBufOffset += 2;
	}
}

void CTitleFile::ProcessReverse(PBUF_READ pReadData)
{
	if((READFLAG_UNICODE == (pReadData->nUnicodeState&READFLAG_UNICODE))
		&&(READFLAG_REVERSE == (pReadData->nUnicodeState&READFLAG_REVERSE)))
	{
		LPBYTE pData = pReadData->bufFile + pReadData->dwBufOffset;
		for(UINT i = 0; i < (pReadData->dwBufSize - 1); i += 2)
		{
			BYTE byData = pData[i];
			pData[i] = pData[i + 1];
			pData[i + 1] = byData;
		}
	}
}

BOOL CTitleFile::ReadTitleFile(CFile* pFile, VT_TITLE& vtTitle, CString& strFmtHdr, BOOL& bUnicode)
{
	BUF_READ rbData;
	BOOL	bRet = FALSE;
	CString strExt = PathFindExtension(pFile->GetFileName());
	try
	{
		if(CTitleFile::PrepareRead(&rbData, pFile))
		{
			CTitleFile::FillBuffer(&rbData, pFile);
			rbData.nUnicodeState = TestUnicodeBuf(rbData.bufFile, rbData.dwBufSize);
			CTitleFile::StepSignature(&rbData);
			CTitleFile::ProcessReverse(&rbData);
			if(strExt.CompareNoCase(_T(".ssa")) == 0)
			{
				rbData.dwFmtFlags = FMTFLAG_SSA;
				UINT nOffset = 0;
				if(rbData.nUnicodeState > 0)
				{
					strFmtHdr = CTitleFile::GetSSAHdr((LPCTSTR)(rbData.bufFile + rbData.dwBufOffset), nOffset);
					rbData.dwBufOffset += nOffset*sizeof(WCHAR);
				}
				else
				{
					strFmtHdr = CTitleFile::GetSSAHdrA((LPCSTR)(rbData.bufFile + rbData.dwBufOffset), nOffset);
					rbData.dwBufOffset += nOffset*sizeof(CHAR);
				}
			}
			else if(strExt.CompareNoCase(_T(".ass")) == 0)
			{
				rbData.dwFmtFlags = FMTFLAG_ASS;
				UINT nOffset = 0;
				if(rbData.nUnicodeState > 0)
				{
					strFmtHdr = CTitleFile::GetSSAHdr((LPCTSTR)(rbData.bufFile + rbData.dwBufOffset), nOffset, TRUE);
					rbData.dwBufOffset += nOffset*sizeof(WCHAR);
				}
				else
				{
					strFmtHdr = CTitleFile::GetSSAHdrA((LPCSTR)(rbData.bufFile + rbData.dwBufOffset), nOffset, TRUE);
					rbData.dwBufOffset += nOffset*sizeof(CHAR);
				}
			}
			else if(strExt.CompareNoCase(_T(".srt")) == 0)
			{
				rbData.dwFmtFlags = FMTFLAG_SRT;
			}
			else
			{
				rbData.dwFmtFlags = FMTFLAG_UNKNOWN;
			}
			PTITLE_UNIT pUnit;
			while(rbData.dwBufOffset < rbData.dwBufSize)
			{
				pUnit = CTitleFile::StepTitleUnit(&rbData);
				if(pUnit != NULL)
					vtTitle.push_back(pUnit);
			}
			bRet = TRUE;
		}
	}
	catch(...)
	{
		OutputDebugString(_T("Catch an error on read file!\r\n"));
	}
	CTitleFile::ClearReadBuf(&rbData);
	bUnicode = (rbData.nUnicodeState > 0);
	return bRet;
}

BOOL CTitleFile::ReadTransFile(CFile* pFile, VT_TITLE& vtTitle, BOOL bSplitByLine, BOOL& bUnicode)
{
	BUF_READ rbData;
	try
	{
		if(CTitleFile::PrepareRead(&rbData, pFile))
		{
			CTitleFile::FillBuffer(&rbData, pFile);
			rbData.nUnicodeState = TestUnicodeBuf(rbData.bufFile, rbData.dwBufSize);
			CTitleFile::StepSignature(&rbData);
			CTitleFile::ProcessReverse(&rbData);
			PTITLE_UNIT pUnit;
			while((pUnit = CTitleFile::StepTransUnit(&rbData, bSplitByLine)) != NULL)
				vtTitle.push_back(pUnit);
		}
	}
	catch(...)
	{
		return FALSE;
	}
	CTitleFile::ClearReadBuf(&rbData);
	bUnicode = (rbData.nUnicodeState > 0);
	return TRUE;
}

BOOL CTitleFile::WriteTitleFile(CFile* pFile, const VT_TITLE& vtTitle, CString& strFmtHdr, CString& strPreCode, CString& strPostCode, DWORD dwFmt, BOOL bUnicode, int nStart, int nEnd)
{
	CString strItem;
	int	nLen;
	PTITLE_UNIT pUnit;
	int nSize = (int)vtTitle.size();

	if(bUnicode)
	{
		WORD wUFlag = 0xFEFF;
		pFile->Write(&wUFlag, 2);
	}
	switch(dwFmt)
	{
	case FMTFLAG_SRT:
		// Nothing to do
		break;
	case FMTFLAG_SSA:
	case FMTFLAG_ASS:
		if(!bUnicode)
		{
			CStringA strItemA;
			int nASize = WideCharToMultiByte(CP_ACP, 0,
				strFmtHdr, strFmtHdr.GetLength(),
				NULL, 0, NULL, NULL);
			if(nASize > 0)
			{
				WideCharToMultiByte(CP_ACP, 0,
					strFmtHdr, strFmtHdr.GetLength(),
					strItemA.GetBufferSetLength(nASize), nASize, NULL, NULL);
				pFile->Write(strItemA.GetBuffer(), strItemA.GetLength()*sizeof(CHAR));
			}
		}
		else
		{
			pFile->Write(strFmtHdr, strFmtHdr.GetLength()*sizeof(WCHAR));
		}
		break;
	}

	if(-1 == nStart)
	{
		nStart = 0;
	}

	if(-1 == nEnd)
	{
		nEnd = nSize;
	}

	int iSrtCnt = 0;
	for(int iUnit = nStart; iUnit < nEnd; iUnit ++)
	{
		pUnit = vtTitle.at(iUnit);
		switch(dwFmt)
		{
		case FMTFLAG_SRT:
			iSrtCnt ++;
			strItem = CTitleHelper::FormatSRTString(iSrtCnt, pUnit);
			break;
		case FMTFLAG_SSA:
			strItem = CTitleHelper::FormatSSAString(pUnit, strPreCode, strPostCode);
			break;
		case FMTFLAG_ASS:
			strItem = CTitleHelper::FormatASSString(pUnit, strPreCode, strPostCode);
			break;
		}

		if(bUnicode)
		{
			nLen = strItem.GetLength();
			pFile->Write(strItem.GetBuffer(), nLen*sizeof(TCHAR));
		}
		else
		{
			CStringA strAItem;
			int nASize = WideCharToMultiByte(CP_ACP, 0, strItem, strItem.GetLength(), NULL, 0, NULL, NULL);
			if(nASize > 0)
			{
				WideCharToMultiByte(CP_ACP, 0, 
					strItem, strItem.GetLength(),
					strAItem.GetBufferSetLength(nASize), nASize,
					NULL, NULL);
				pFile->Write(strAItem.GetBuffer(), nASize*sizeof(CHAR));
			}
		}
	}
	return TRUE;
}

void CTitleFile::DeleteContents(VT_TITLE& vtTitle)
{
	int nSize = (int)vtTitle.size();
	for(int iTitle = nSize - 1; iTitle > -1; iTitle --)
	{
		delete vtTitle[iTitle];
		vtTitle.pop_back();
	}
}

// 950,936...
CStringA CTitleFile::ConvertCodePageStr(UINT nCPFrom, UINT nCPTo, const CStringA& strAnsi)
{
	CStringA strRetAnsi;
	CStringW strWide;
	try
	{
		int nSize = MultiByteToWideChar(nCPFrom, 0, strAnsi, strAnsi.GetLength(), strWide.GetBuffer(), 0);
		strWide.ReleaseBuffer();
		if(nSize > 0)
		{
			MultiByteToWideChar(nCPFrom, 0, strAnsi, strAnsi.GetLength(), strWide.GetBufferSetLength(nSize), nSize);
			strWide.ReleaseBuffer(nSize);
		}

		nSize = WideCharToMultiByte(nCPTo, 0, strWide, strWide.GetLength(), strRetAnsi.GetBuffer(), 0, NULL, NULL);
		strRetAnsi.ReleaseBuffer();
		if(nSize > 0)
		{
			WideCharToMultiByte(nCPTo, 0, strWide, strWide.GetLength(), strRetAnsi.GetBufferSetLength(nSize), nSize, NULL, NULL);
			strRetAnsi.ReleaseBuffer(nSize);
		}
	}
	catch(...)
	{
		// Add error process or log
	}
	return strRetAnsi;
}

// SORT_CHINESE_PRC/SORT_CHINESE_BIG5, LCMAP_TRADITIONAL_CHINESE/LCMAP_SIMPLIFIED_CHINESE
CString CTitleFile::LCMapStrCN(UINT nSortKey, UINT nMapFlags, const CString& strInput)
{
	CString strRetAnsi;
	try
	{
		LCID wLCID = MAKELCID(MAKELANGID
			(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), nSortKey);
		int nSize = LCMapString(wLCID, nMapFlags, strInput, strInput.GetLength(), NULL, 0);
		if(nSize > 0)
		{
			LCMapString(wLCID, nMapFlags, strInput, strInput.GetLength(), strRetAnsi.GetBufferSetLength(nSize), nSize);
			strRetAnsi.ReleaseBufferSetLength(nSize);
		}
	}
	catch(...)
	{
		// Add error process or log
	}
	return strRetAnsi;
}

CStringA CTitleFile::ConvertUnicodeToUTF8(const CStringW& strSource)
{
	CStringA strRetAnsi;
	try
	{
		int nSize = WideCharToMultiByte(CP_UTF8, 0, strSource, strSource.GetLength(), strRetAnsi.GetBuffer(), 0, NULL, NULL);
		strRetAnsi.ReleaseBuffer();
		if(nSize > 0)
		{
			WideCharToMultiByte(CP_UTF8, 0, strSource, strSource.GetLength(), strRetAnsi.GetBufferSetLength(nSize), nSize, NULL, NULL);
			strRetAnsi.ReleaseBuffer(nSize);
		}
	}
	catch(...)
	{
		// Add error process or log
	}
	return strRetAnsi;
}
