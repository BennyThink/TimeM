#pragma once
#include "TitleUnit.h"

class CTitleFile
{
public:
	CTitleFile(void);
	~CTitleFile(void);
	static BOOL			IsLineEnd(PBUF_READ pReadData);
	static void			StepOffset(PBUF_READ pReadData);
	static BOOL			ReadSubTitleByLine(PBUF_READ pReadData, PTITLE_UNIT pUnit);

	static void			StepSpaceChar(PBUF_READ pReadData);
	static void			StepInvalidChar(PBUF_READ pReadData);
	static void			StepALine(PBUF_READ pReadData);
	static BOOL			ReadSubStart(PBUF_READ pReadData, PTITLE_UNIT pUnit);
	static BOOL			ReadSubEnd(PBUF_READ pReadData, PTITLE_UNIT pUnit);
	static BOOL			ReadSubTitle(PBUF_READ pReadData, PTITLE_UNIT pUnit);

	static BOOL			ReadSSALine(PBUF_READ pReadData, CString& strLine);
	static BOOL			FillSSAUnit(CString& strLine, PTITLE_UNIT pUnit);

	static PTITLE_UNIT	StepTitleUnit(PBUF_READ pReadData);
	static PTITLE_UNIT	StepTransUnit(PBUF_READ pReadData, BOOL bByLine);
	static BOOL			PrepareRead(PBUF_READ pReadData, CFile* pFile);
	static void			FillBuffer(PBUF_READ pReadData, CFile* pFile);
	static void			ClearReadBuf(PBUF_READ pReadData);
	static BOOL			ReadTitleFile(CFile* pFile, VT_TITLE& vtTitle, CString& strFmtHdr, BOOL& bUnicode);
	static BOOL			WriteTitleFile(CFile* pFile, const VT_TITLE& vtTitle, CString& strFmtHdr, CString& strPreCode, CString& strPostCode, DWORD dwFmt, BOOL bUnicode, int nStart = -1, int nEnd = -1);
	static BOOL			ReadTransFile(CFile* pFile, VT_TITLE& vtTitle, BOOL bSplitByLine, BOOL& bUnicode);
	static void			DeleteContents(VT_TITLE& vtTitle);

	static CString		GetSSAHdr(LPCTSTR lpszData, UINT& nEventOffset, BOOL bAdvanced = FALSE);
	static CString		GetSSASection(CString strSrc, CString strTag);

	static CStringW		GetSSAHdrA(LPCSTR lpszData, UINT& nEventOffset, BOOL bAdvanced = FALSE);
	static CStringA		GetSSASection(CStringA strSrc, CStringA strTag);

	static void			StepSignature(PBUF_READ pReadData);
	static void			ProcessReverse(PBUF_READ pReadData);
	static UINT			TestUnicodeBuf(LPVOID lpData, int nSize);	// 0 is false, unicode/reverse/signature
	static CStringA		ConvertCodePageStr(UINT nCPFrom, UINT nCPTo, const CStringA& strAnsi);
	static CString		LCMapStrCN(UINT nSortKey, UINT nMapFlags, const CString& strInput);
	static CStringA		ConvertUnicodeToUTF8(const CStringW& strSource);
};
