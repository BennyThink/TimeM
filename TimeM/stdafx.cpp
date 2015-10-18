
// stdafx.cpp : 只包括标准包含文件的源文件
// TimeM.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

#define	MAX_STRBUF_LENGTH	1024
void _cdecl WriteLogLineCb(LPTSTR lpszLine, va_list pParam);

#ifdef	_DEBUG
#define	LOG_OPENFLAG	OPEN_ALWAYS
void _cdecl WriteLogLine(LPTSTR lpszLine, ...)
{
	va_list pParam;
	va_start(pParam, lpszLine);
	TCHAR	szLogFile[MAX_PATH];
	GetModuleFileName(NULL, szLogFile, MAX_PATH);
	PathRemoveFileSpec(szLogFile);
	PathCombine(szLogFile, szLogFile, _T("AppLog.txt"));

	if(PathFileExists(szLogFile))
		WriteLogLineCb(lpszLine, pParam);
}
#else
#define	LOG_OPENFLAG	OPEN_EXISTING
void _cdecl WriteLogLine(LPTSTR lpszLine, ...)
{
	va_list pParam;
	va_start(pParam, lpszLine);
	TCHAR	szLogFile[MAX_PATH];
	GetModuleFileName(NULL, szLogFile, MAX_PATH);
	PathRemoveFileSpec(szLogFile);
	PathCombine(szLogFile, szLogFile, _T("AppLog.txt"));

	if(PathFileExists(szLogFile))
		WriteLogLineCb(lpszLine, pParam);
}
#endif
void _cdecl WriteLogLineCb(LPTSTR lpszLine, va_list pParam)
{
	TCHAR	szLogName[MAX_PATH];
	StringCbPrintf(szLogName, MAX_PATH, _T("Log%04x-%04x.txt"), GetCurrentProcessId(), GetCurrentThreadId());

	HANDLE	hLogFile = NULL;
	TCHAR	szLogFile[MAX_PATH];
	GetModuleFileName(NULL, szLogFile, MAX_PATH);
	PathRemoveFileSpec(szLogFile);
	PathCombine(szLogFile, szLogFile, szLogName);

//	EnterCriticalSection(&g_CriticalCurrSect);
	hLogFile = CreateFile(szLogFile, 
		GENERIC_WRITE, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hLogFile != INVALID_HANDLE_VALUE)
	{

		SetFilePointer(hLogFile, 0, NULL, FILE_END);

		LPTSTR lpszOutput = new TCHAR[MAX_STRBUF_LENGTH];
		if(lpszOutput != NULL)
		{
			SYSTEMTIME		curTime;

			GetLocalTime(&curTime);
			ZeroMemory(lpszOutput, MAX_STRBUF_LENGTH*sizeof(TCHAR));
			StringCbPrintf(lpszOutput, MAX_STRBUF_LENGTH*sizeof(TCHAR), _T("%04d-%02d-%02d %02d::%02d::%02d %03d "), 
				curTime.wYear,
				curTime.wMonth,
				curTime.wDay,
				curTime.wHour,
				curTime.wMinute,
				curTime.wSecond,
				curTime.wMilliseconds);

			ULONG nLen = lstrlen(lpszOutput);

			StringCbVPrintf(lpszOutput+nLen, MAX_STRBUF_LENGTH - nLen, lpszLine, pParam);
			OutputDebugString(lpszOutput);

			DWORD dwWrited;
			nLen = lstrlen(lpszOutput);
			if(hLogFile != NULL)
			{
				WriteFile(hLogFile, (PVOID)lpszOutput, nLen*sizeof(TCHAR), &dwWrited, NULL);
			}

			delete [] lpszOutput;
		}

		CloseHandle(hLogFile);
	}
//	LeaveCriticalSection(&g_CriticalCurrSect);
}

