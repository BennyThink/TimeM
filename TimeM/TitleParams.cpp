// TitleParams.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "TitleParams.h"
#include "shlwapi.h"
#include "TitleFile.h"

// CTitleParams

CTitleParams::CTitleParams()
{
	m_dwDefLen		= 1000;
	m_nStartDelay	= 0;
	m_nMinInterval	= 10000;
}

CTitleParams::~CTitleParams()
{
}

CString CTitleParams::GetDefHeader(LPCTSTR pHdrFile)
{
	CString strHeader;

	TCHAR	szModuleFile[MAX_PATH];
	GetModuleFileName(NULL, szModuleFile, MAX_PATH);
	PathRemoveFileSpec(szModuleFile);
	PathCombine(szModuleFile, szModuleFile, pHdrFile);

	CFile fileRead;
	BUF_READ rbData;
	if(fileRead.Open(szModuleFile, CFile::modeRead))
	{
		if(CTitleFile::PrepareRead(&rbData, &fileRead))
		{
			CTitleFile::FillBuffer(&rbData, &fileRead);
			rbData.nUnicodeState = CTitleFile::TestUnicodeBuf(rbData.bufFile, rbData.dwBufSize);
			CTitleFile::StepSignature(&rbData);
			CTitleFile::ProcessReverse(&rbData);

			if(rbData.nUnicodeState > 0)
			{
				LPTSTR pData = strHeader.GetBufferSetLength(rbData.dwBufSize - rbData.dwBufOffset);
				CopyMemory(pData, (rbData.bufFile + rbData.dwBufOffset), rbData.dwBufSize - rbData.dwBufOffset);
				strHeader.ReleaseBuffer();
			}
			else
			{
				int nASize = MultiByteToWideChar(CP_ACP, 0,
					LPSTR(rbData.bufFile + rbData.dwBufOffset), (rbData.dwBufSize - rbData.dwBufOffset)/sizeof(CHAR),
					NULL, 0);
				if(nASize > 0)
				{
					MultiByteToWideChar(CP_ACP, 0,
						LPSTR(rbData.bufFile + rbData.dwBufOffset), (rbData.dwBufSize - rbData.dwBufOffset)/sizeof(CHAR),
						strHeader.GetBufferSetLength(nASize), nASize);
					strHeader.ReleaseBuffer();
				}
			}
		}
		CTitleFile::ClearReadBuf(&rbData);
	}
	return strHeader;
}

CString CTitleParams::GetDefSSAHeader()
{
	return GetDefHeader(FILE_DEFSSA_HEADER);
}

CString CTitleParams::GetDefASSHeader()
{
	return GetDefHeader(FILE_DEFASS_HEADER);
}
