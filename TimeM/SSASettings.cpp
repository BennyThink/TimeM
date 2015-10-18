// SSASettings.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "SSASettings.h"
#include "TitleFile.h"


// CSSASettings dialog

IMPLEMENT_DYNAMIC(CSSASettings, CDialog)

CSSASettings::CSSASettings(BOOL bAdvanced, CWnd* pParent /*=NULL*/)
	: CDialog(CSSASettings::IDD, pParent)
	, m_strSSAInfo(_T(""))
	, m_strSSAV4Style(_T(""))
	, m_strPreCode(_T(""))
	, m_strPostCode(_T(""))
	, m_strEvents(_T(""))
	, m_strStyleName(_T("[V4 Styles]"))
	, m_strInfoName(_T("[Script Info]"))
	, m_strEventName(_T("[Events]"))
{
	if(bAdvanced)
		m_strStyleName = _T("[V4+ Styles]");
	else
		m_strStyleName = _T("[V4 Styles]");
}

CSSASettings::~CSSASettings()
{
}

void CSSASettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SINFO, m_strSSAInfo);
	DDX_Text(pDX, IDC_EDIT_SSASTYLES, m_strSSAV4Style);
	DDX_Text(pDX, IDC_CB_EFFECTPREV, m_strPreCode);
	DDX_Text(pDX, IDC_CB_EFFECTPOST, m_strPostCode);
	DDX_Text(pDX, IDC_STATIC_SSASTYLES, m_strStyleName);
	DDX_Text(pDX, IDC_EDIT_EVENTS, m_strEvents);
	DDX_Text(pDX, IDC_STATIC_SINFO, m_strInfoName);
	DDX_Text(pDX, IDC_STATIC_EVENTS, m_strEventName);
	DDX_Control(pDX, IDC_CB_EFFECTPREV, m_cbLinePrevs);
	DDX_Control(pDX, IDC_CB_EFFECTPOST, m_cbLinePosts);
}


BEGIN_MESSAGE_MAP(CSSASettings, CDialog)
END_MESSAGE_MAP()


// CSSASettings message handlers
BOOL CSSASettings::OnInitDialog()
{
	m_strSSAInfo = CTitleFile::GetSSASection(m_strHdr, m_strInfoName);
	m_strSSAV4Style = CTitleFile::GetSSASection(m_strHdr, m_strStyleName);
	m_strEvents = CTitleFile::GetSSASection(m_strHdr, m_strEventName);
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	ReadRecentCodes();
	m_cbLinePrevs.SetCurSel(0);
	m_cbLinePosts.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSSASettings::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();

	m_strHdr.Empty();
	m_strHdr += m_strInfoName;
	m_strHdr += _T("\r\n");
	m_strHdr += m_strSSAInfo;
	m_strHdr += _T("\r\n");
	m_strHdr += _T("\r\n");
	
	m_strHdr += m_strStyleName;
	m_strHdr += _T("\r\n");
	m_strHdr += m_strSSAV4Style;
	m_strHdr += _T("\r\n");
	m_strHdr += _T("\r\n");

	m_strHdr += m_strEventName;
	m_strHdr += _T("\r\n");
	m_strHdr += m_strEvents;
	m_strHdr += _T("\r\n");

	SaveRecentCodes();
	CDialog::OnOK();
}

#define	MAX_CODELEN	1024
void CSSASettings::SaveRecentCodes()
{
	TCHAR	szModule[MAX_PATH];
	GetModuleFileName(NULL, szModule, MAX_PATH);
	PathRemoveFileSpec(szModule);
	PathCombine(szModule, szModule, _T("EffectCode.tpl"));

	CString strCode;
	CString strKey;

	int iCode = 0;
	if(!m_strPreCode.IsEmpty())
	{
		strKey.Format(_T("%d"), iCode);
		WritePrivateProfileString(_T("PrevCode"), strKey, m_strPreCode, szModule);
		iCode ++;
	}

	int nCnt = m_cbLinePrevs.GetCount();
	for(int iItem = 0; iItem < nCnt; iItem ++)
	{
		m_cbLinePrevs.GetLBText(iItem, strCode);
		if(m_strPreCode.CompareNoCase(strCode) != 0)
		{
			strKey.Format(_T("%d"), iCode);
			WritePrivateProfileString(_T("PrevCode"), strKey, strCode, szModule);
			iCode ++;
		}
	}

	iCode = 0;
	if(!m_strPostCode.IsEmpty())
	{
		strKey.Format(_T("%d"), iCode);
		WritePrivateProfileString(_T("PrevCode"), strKey, m_strPostCode, szModule);
		iCode ++;
	}

	nCnt = m_cbLinePosts.GetCount();
	for(int iItem = 0; iItem < nCnt; iItem ++)
	{
		m_cbLinePosts.GetLBText(iItem, strCode);
		if(m_strPostCode.CompareNoCase(strCode) != 0)
		{
			strKey.Format(_T("%d"), iCode);
			WritePrivateProfileString(_T("PrevCode"), strKey, strCode, szModule);
			iCode ++;
		}
	}
}

void CSSASettings::ReadRecentCodes()
{
	TCHAR	szModule[MAX_PATH];
	TCHAR	szLineCodes[MAX_CODELEN];
	CString strKey;
	GetModuleFileName(NULL, szModule, MAX_PATH);
	PathRemoveFileSpec(szModule);
	PathCombine(szModule, szModule, _T("EffectCode.tpl"));
	int iPrevCode = 0;
	while(1)
	{
		strKey.Format(_T("%d"), iPrevCode);
		GetPrivateProfileString(_T("PrevCode"), strKey, NULL, szLineCodes, MAX_CODELEN, szModule);
		if(lstrlen(szLineCodes) == 0)
		{
			break;
		}
		m_cbLinePrevs.AddString(szLineCodes);
		iPrevCode ++;
	}

	iPrevCode = 0;
	while(1)
	{
		strKey.Format(_T("%d"), iPrevCode);
		GetPrivateProfileString(_T("PostCode"), strKey, NULL, szLineCodes, MAX_CODELEN, szModule);
		if(lstrlen(szLineCodes) == 0)
		{
			break;
		}
		m_cbLinePosts.AddString(szLineCodes);
		iPrevCode ++;
	}
}
