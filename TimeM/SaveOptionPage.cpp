// SaveOptionPage.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "SaveOptionPage.h"


// CSaveOptionPage dialog

IMPLEMENT_DYNCREATE(CSaveOptionPage, CPropertyPage)

CSaveOptionPage::CSaveOptionPage()
	: CPropertyPage(CSaveOptionPage::IDD)
	, m_nTimeDelay(0)
	, m_nMaxBak(0)
	, m_strBakPath(_T(""))
{

}

CSaveOptionPage::~CSaveOptionPage()
{
}

void CSaveOptionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TIMEDELAY, m_nTimeDelay);
	DDX_Text(pDX, IDC_EDIT_BAKMAX, m_nMaxBak);
	DDX_Text(pDX, IDC_EDIT_BAKPATH, m_strBakPath);
}


BEGIN_MESSAGE_MAP(CSaveOptionPage, CPropertyPage)
	ON_BN_CLICKED(IDC_CHK_AUTOSAVE, &CSaveOptionPage::OnBnClickedChkAutosave)
	ON_BN_CLICKED(IDC_CHK_AUTOBAK, &CSaveOptionPage::OnBnClickedChkAutobak)
	ON_EN_KILLFOCUS(IDC_EDIT_TIMEDELAY, &CSaveOptionPage::OnEnKillfocusEditTimedelay)
	ON_EN_KILLFOCUS(IDC_EDIT_BAKMAX, &CSaveOptionPage::OnEnKillfocusEditBakmax)
	ON_BN_CLICKED(IDC_BTN_BROW, &CSaveOptionPage::OnBnClickedBtnBrow)
	ON_BN_CLICKED(IDC_CHK_SRTASSOCIATE, &CSaveOptionPage::OnBnClickedChkSrtassociate)
	ON_BN_CLICKED(IDC_CHK_ASSASSOCIATE, &CSaveOptionPage::OnBnClickedChkAssassociate)
	ON_BN_CLICKED(IDC_CHK_SSAASSOCIATE, &CSaveOptionPage::OnBnClickedChkSsaassociate)
END_MESSAGE_MAP()

// CSaveOptionPage message handlers

BOOL CSaveOptionPage::OnInitDialog()
{
	TCHAR	szBakPath[MAX_PATH];
	GetModuleFileName(NULL, szBakPath, MAX_PATH);
	PathRemoveFileSpec(szBakPath);
	StringCbCat(szBakPath, MAX_PATH, _T("\\UserData"));

	m_nTimeDelay = AfxGetApp()->GetProfileInt(_T("Settings"), _T("BackupTime"), 10);
	m_nMaxBak = AfxGetApp()->GetProfileInt(_T("Settings"), _T("MaxBackup"), 3);
	m_strBakPath = AfxGetApp()->GetProfileString(_T("Settings"), _T("BackupPath"), szBakPath);

	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CheckDlgButton(IDC_CHK_AUTOSAVE, AfxGetApp()->GetProfileInt(_T("Settings"), _T("IsAutoSave"), TRUE));
	CheckDlgButton(IDC_CHK_AUTOBAK, AfxGetApp()->GetProfileInt(_T("Settings"), _T("IsAutoBackup"), TRUE));
	CheckDlgButton(IDC_CHK_SRTASSOCIATE, HasRegistered(_T(".srt")));
	CheckDlgButton(IDC_CHK_ASSASSOCIATE, HasRegistered(_T(".ass")));
	CheckDlgButton(IDC_CHK_SSAASSOCIATE, HasRegistered(_T(".ssa")));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSaveOptionPage::OnBnClickedChkAutosave()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	AfxGetApp()->WriteProfileInt( _T("Settings"), _T("IsAutoSave"), IsDlgButtonChecked(IDC_CHK_AUTOSAVE));
}

void CSaveOptionPage::OnBnClickedChkAutobak()
{
	// TODO: Add your control notification handler code here
	BOOL bAutoBak = IsDlgButtonChecked(IDC_CHK_AUTOBAK);
	AfxGetApp()->WriteProfileInt( _T("Settings"), _T("IsAutoBackup"), bAutoBak);

	CWnd* pWndBakPath = GetDlgItem(IDC_EDIT_BAKPATH);
	CWnd* pWndBrow = GetDlgItem(IDC_BTN_BROW);
	CWnd* pWndTimeDelay = GetDlgItem(IDC_EDIT_TIMEDELAY);
	CWnd* pWndBakMax = GetDlgItem(IDC_EDIT_BAKMAX);

	pWndBakPath->EnableWindow(bAutoBak);
	pWndBrow->EnableWindow(bAutoBak);
	pWndTimeDelay->EnableWindow(bAutoBak);
	pWndBakMax->EnableWindow(bAutoBak);
}

void CSaveOptionPage::OnEnKillfocusEditTimedelay()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("BackupTime"), m_nTimeDelay);
}

void CSaveOptionPage::OnEnKillfocusEditBakmax()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("MaxBackup"), m_nMaxBak);
}

void CSaveOptionPage::OnBnClickedBtnBrow()
{
	// TODO: Add your control notification handler code here
    BROWSEINFO bi;
    ZeroMemory(&bi,sizeof(BROWSEINFO));
    LPMALLOC pMalloc;
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    TCHAR szSelPath[MAX_PATH];
    if(pidl != NULL)
    {
        SHGetPathFromIDList(pidl, szSelPath);
		m_strBakPath = szSelPath;
        if(SUCCEEDED(SHGetMalloc(&pMalloc)))//pidl指向的对象用完应该释放，之前忽略了
        {
            pMalloc->Free(pidl);
            pMalloc->Release();
        }
    }
	AfxGetApp()->WriteProfileString(_T("Settings"), _T("BackupPath"), m_strBakPath);
	UpdateData(FALSE);
}

BOOL CSaveOptionPage::HasRegistered(LPCTSTR pszType)
{
	POSITION posTmpl = AfxGetApp()->GetFirstDocTemplatePosition();
	if(posTmpl != NULL)
	{
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(posTmpl);
		CString strFileTypeId;
		if (pTemplate->GetDocString(strFileTypeId, CDocTemplate::regFileTypeId))
		{
			CString strTemp;
			LONG lSize = _MAX_PATH * 2;
			LONG lResult = AfxRegQueryValue(HKEY_CLASSES_ROOT, pszType,
				strTemp.GetBuffer(lSize), &lSize);
			strTemp.ReleaseBuffer();
			return (strTemp.Compare(strFileTypeId) == 0);
		}
	}
	return FALSE;
}

void CSaveOptionPage::RegFileType(LPCTSTR pszType)
{
	POSITION posTmpl = AfxGetApp()->GetFirstDocTemplatePosition();
	if(posTmpl != NULL)
	{
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(posTmpl);
		CString strFileTypeId;
		if (pTemplate->GetDocString(strFileTypeId, CDocTemplate::regFileTypeId))
		{
			CString strTemp;
			LONG lSize = _MAX_PATH * 2;
			LONG lResult = AfxRegQueryValue(HKEY_CLASSES_ROOT, pszType,
				strTemp.GetBuffer(lSize), &lSize);
			strTemp.ReleaseBuffer();

			if(strTemp.Compare(strFileTypeId) != 0)
			{
				AfxRegSetValue(
					HKEY_CLASSES_ROOT, pszType, REG_SZ,
					strFileTypeId, lstrlen(strFileTypeId) * sizeof(TCHAR));
				AfxGetApp()->WriteProfileString(_T("Settings"), pszType, strTemp);
			}
		}
	}
}

void CSaveOptionPage::UnRegFileType(LPCTSTR pszType)
{
	POSITION posTmpl = AfxGetApp()->GetFirstDocTemplatePosition();
	if(posTmpl != NULL)
	{
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(posTmpl);
		CString strFileTypeId;
		if (pTemplate->GetDocString(strFileTypeId, CDocTemplate::regFileTypeId))
		{
			CString strTemp = AfxGetApp()->GetProfileString(_T("Settings"), pszType);
			if(strTemp.Compare(strFileTypeId) != 0)
			{
				AfxRegSetValue(
					HKEY_CLASSES_ROOT, pszType, REG_SZ,
					strTemp, lstrlen(strTemp) * sizeof(TCHAR));
			}
		}
	}
}

void CSaveOptionPage::OnBnClickedChkSrtassociate()
{
	if(IsDlgButtonChecked(IDC_CHK_SRTASSOCIATE))
	{
		RegFileType(_T(".srt"));
	}
	else
	{
		if(HasRegistered(_T(".srt")))
			UnRegFileType(_T(".srt"));
	}
}

void CSaveOptionPage::OnBnClickedChkAssassociate()
{
	if(IsDlgButtonChecked(IDC_CHK_ASSASSOCIATE))
	{
		RegFileType(_T(".ass"));
	}
	else
	{
		if(HasRegistered(_T(".ass")))
			UnRegFileType(_T(".ass"));
	}
}

void CSaveOptionPage::OnBnClickedChkSsaassociate()
{
	if(IsDlgButtonChecked(IDC_CHK_SSAASSOCIATE))
	{
		RegFileType(_T(".ssa"));
	}
	else
	{
		if(HasRegistered(_T(".ssa")))
			UnRegFileType(_T(".ssa"));
	}
}
