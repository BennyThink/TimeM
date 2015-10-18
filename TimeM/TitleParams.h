#pragma once

typedef struct tagCmdUnit
{
	WORD	nType;
	WORD	nCommand;
}COMMAND_UNIT, *PCOMMAND_UNIT;

#define	FILE_DEFSSA_HEADER	_T("SSAHdr.tpl")
#define	FILE_DEFASS_HEADER	_T("ASSHdr.tpl")

// CTitleParams
class CTitleParams
{

public:
	CTitleParams();
	virtual ~CTitleParams();

protected:
	int		m_nMinInterval;
	DWORD	m_dwDefLen;
	int		m_nStartDelay;
	BOOL	m_bAutoSave;
	CString	m_strZipTools;
	CString	m_strDefSSAHeader;
	CString	m_strDefASSHeader;
	int		m_nMaxBak;
	CString m_strBakPath;
public:
	void SetMaxBak(int nMax){m_nMaxBak = nMax;};
	int GetMaxBak(){return m_nMaxBak;};

	void SetBakPath(const CString& strBakPath){m_strBakPath = strBakPath;};
	const CString& GetBakPath(){return m_strBakPath;};

	void SetZipToolsPath(CString strZipPath){m_strZipTools = strZipPath;};
	const CString& GetZipPath(){return m_strZipTools;};

	void SetDefaultLen(DWORD dwLen){m_dwDefLen = dwLen;};
	DWORD GetDefaultLen(){return m_dwDefLen;};

	void SetStartDelay(int nDelay){m_nStartDelay = nDelay;};
	int GetStartDelay(){return m_nStartDelay;};

	BOOL IsAutoSave(){return m_bAutoSave;};
	void SetAutoSave(BOOL bAutoSave = TRUE){m_bAutoSave = bAutoSave;};

	int GetMinInterval(){return m_nMinInterval;}
	void SetMinInterval(int nMinInterval){m_nMinInterval = nMinInterval;}

	static CString GetDefHeader(LPCTSTR pHdrFile);
	static CString GetDefSSAHeader();
	static CString GetDefASSHeader();
};
