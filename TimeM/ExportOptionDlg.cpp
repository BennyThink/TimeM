// ExportOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "ExportOptionDlg.h"
#include "SSASettings.h"


// CExportOptionDlg dialog

IMPLEMENT_DYNAMIC(CExportOptionDlg, CDialog)

CExportOptionDlg::CExportOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportOptionDlg::IDD, pParent)
	, m_strEngName(_T("英文"))
	, m_strChsName(_T("简体"))
	, m_strChtName(_T("繁体"))
	, m_strOutPath(_T(""))
{
	UNIT_FILE unitFile;
	unitFile.wFmtID		= FILE_FORMAT_SRT;
	unitFile.wLangFlag	= LANG_FLAG_ENG;
	unitFile.bUnicode	= FALSE;
	m_ExportParam.vtFiles.push_back(unitFile);

	unitFile.wFmtID		= FILE_FORMAT_SRT;
	unitFile.wLangFlag	= LANG_FLAG_CHS;
	unitFile.bUnicode	= FALSE;
	m_ExportParam.vtFiles.push_back(unitFile);

	unitFile.wFmtID		= FILE_FORMAT_SRT;
	unitFile.wLangFlag	= LANG_FLAG_CHT;
	unitFile.bUnicode	= FALSE;
	m_ExportParam.vtFiles.push_back(unitFile);

	unitFile.wFmtID		= FILE_FORMAT_SRT;
	unitFile.wLangFlag	= LANG_FLAG_CHS|LANG_FLAG_ENG;
	unitFile.bUnicode	= FALSE;
	m_ExportParam.vtFiles.push_back(unitFile);

	unitFile.wFmtID		= FILE_FORMAT_SRT;
	unitFile.wLangFlag	= LANG_FLAG_CHT|LANG_FLAG_ENG;
	unitFile.bUnicode	= FALSE;
	m_ExportParam.vtFiles.push_back(unitFile);

	unitFile.wFmtID		= FILE_FORMAT_ASS;
	unitFile.wLangFlag	= LANG_FLAG_ENG;
	unitFile.bUnicode	= TRUE;
	m_ExportParam.vtFiles.push_back(unitFile);

	unitFile.wFmtID		= FILE_FORMAT_ASS;
	unitFile.wLangFlag	= LANG_FLAG_CHS;
	unitFile.bUnicode	= TRUE;
	m_ExportParam.vtFiles.push_back(unitFile);

	unitFile.wFmtID		= FILE_FORMAT_ASS;
	unitFile.wLangFlag	= LANG_FLAG_CHT;
	unitFile.bUnicode	= TRUE;
	m_ExportParam.vtFiles.push_back(unitFile);

	unitFile.wFmtID		= FILE_FORMAT_ASS;
	unitFile.wLangFlag	= LANG_FLAG_CHS|LANG_FLAG_ENG;
	unitFile.bUnicode	= TRUE;
	m_ExportParam.vtFiles.push_back(unitFile);

	unitFile.wFmtID		= FILE_FORMAT_ASS;
	unitFile.wLangFlag	= LANG_FLAG_CHT|LANG_FLAG_ENG;
	unitFile.bUnicode	= TRUE;
	m_ExportParam.vtFiles.push_back(unitFile);

	m_nFocus = -1;
}

CExportOptionDlg::~CExportOptionDlg()
{
}

void CExportOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILEFMT, m_FileFmt);
	DDX_Text(pDX, IDC_EDIT_ENGNAME, m_strEngName);
	DDX_Text(pDX, IDC_EDIT_CHSNAME, m_strChsName);
	DDX_Text(pDX, IDC_EDIT_CHTNAME, m_strChtName);
	DDX_Text(pDX, IDC_EDIT_OUTPATH, m_strOutPath);
}


BEGIN_MESSAGE_MAP(CExportOptionDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SETASS, &CExportOptionDlg::OnBnClickedBtnSetass)
	ON_BN_CLICKED(IDC_CHK_UNICODE, &CExportOptionDlg::OnBnClickedChkUnicode)
	ON_MESSAGE(MSG_LISTITEM_CHANGED, &CExportOptionDlg::OnListItemChanged)
	ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST_FILEFMT, &CExportOptionDlg::OnLvnEndScrollListFilefmt)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FILEFMT, &CExportOptionDlg::OnNMClickListFilefmt)
	ON_BN_CLICKED(IDC_BTN_BROW, &CExportOptionDlg::OnBnClickedBtnBrow)
END_MESSAGE_MAP()

CString CExportOptionDlg::FmtFileUnit(UNIT_FILE& unit)
{
	CString strOut;
	switch(unit.wFmtID)
	{
	case FILE_FORMAT_SRT:
		strOut = _T("srt");
		break;
	case FILE_FORMAT_SSA:
		strOut = _T("ssa");
		break;
	case FILE_FORMAT_ASS:
		strOut = _T("ass");
		break;
	}
	if(strOut.IsEmpty())
		return strOut;

	if(unit.wLangFlag&LANG_FLAG_CHS)
		strOut += _T("_简体中文");
	if(unit.wLangFlag&LANG_FLAG_CHT)
		strOut += _T("_繁体中文");
	if(unit.wLangFlag&LANG_FLAG_ENG)
		strOut += _T("_英文");
	return strOut;
}

// CExportOptionDlg message handlers

BOOL CExportOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_FileFmt.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES|LVS_EX_INFOTIP);
	m_FileFmt.SetHdrChkImage(IDB_BMP_LISTCHK, 16, 16, RGB(255, 0, 255));

	m_FileFmt.InsertColumn(0, _T("内容格式"), 0, 220);
	m_FileFmt.InsertColumn(1, _T("编码"), 0, 100);
	m_FileFmt.SetColumnImage(0, 0);

	int nSize = (int)m_ExportParam.vtFiles.size();
	for(int iFile = 0; iFile < nSize; iFile ++)
	{
		UNIT_FILE& unit = m_ExportParam.vtFiles.at(iFile);
		int nItem = m_FileFmt.InsertItem(iFile, FmtFileUnit(unit));
		m_FileFmt.SetItemText(nItem, 1, unit.bUnicode?_T("Unicode"):_T("Ansi"));
		m_FileFmt.SetItemData(nItem, iFile);
	}

	UpdateUIStates();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CExportOptionDlg::UpdateUIStates()
{
	CWnd* pWndChk = GetDlgItem(IDC_CHK_UNICODE);
	if(-1 == m_nFocus)
	{
		if(pWndChk)
			pWndChk->ShowWindow(SW_HIDE);
	}
	else
	{
		CRect rcItem;
		m_FileFmt.GetItemRect(m_nFocus, rcItem, LVIR_BOUNDS);
		CRect rcList;
		m_FileFmt.GetWindowRect(rcList);
		ScreenToClient(rcList);

		if(pWndChk)
		{
			CRect rcWnd;
			pWndChk->GetWindowRect(rcWnd);
			ScreenToClient(rcWnd);
			pWndChk->SetWindowPos(&wndTop, rcWnd.left,
				rcList.top + rcItem.top, rcWnd.Width(), rcWnd.Height(),
				SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
			CheckDlgButton(IDC_CHK_UNICODE, m_ExportParam.vtFiles[m_nFocus].bUnicode);
		}
	}
}

LRESULT CExportOptionDlg::OnListItemChanged(WPARAM wParam, LPARAM lParam)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(lParam);
	if(((pNMLV->uNewState&LVIS_FOCUSED) == LVIS_FOCUSED)
		&&((pNMLV->uOldState&LVIS_FOCUSED) == 0))
	{
		m_nFocus = pNMLV->iItem;
	}
	else if((pNMLV->uNewState&LVIS_FOCUSED) == 0)
	{
		m_nFocus = -1;
	}

	UpdateUIStates();
	return 1;
}

void CExportOptionDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();

	int nCnt = m_FileFmt.GetItemCount();
	for(int iItem = nCnt - 1; iItem > -1; iItem --)
	{
		if(!m_FileFmt.GetCheck(iItem))
			m_ExportParam.vtFiles.erase(m_ExportParam.vtFiles.begin() + iItem);
	}

	int nSize = (int)m_ExportParam.vtFiles.size();
	for(int iFile = 0; iFile < nSize; iFile ++)
	{
		UNIT_FILE& unit = m_ExportParam.vtFiles.at(iFile);

		if(unit.wLangFlag&LANG_FLAG_CHS)
		{
			if(!unit.strName.IsEmpty())
				unit.strName += _T("&");
			unit.strName += m_strChsName;
		}
		if(unit.wLangFlag&LANG_FLAG_CHT)
		{
			if(!unit.strName.IsEmpty())
				unit.strName += _T("&");
			unit.strName += m_strChtName;
		}
		if(unit.wLangFlag&LANG_FLAG_ENG)
		{
			if(!unit.strName.IsEmpty())
				unit.strName += _T("&");
			unit.strName += m_strEngName;
		}
	}
	m_ExportParam.bZipOut = IsDlgButtonChecked(IDC_CHK_ZIPPACK);
	CDialog::OnOK();
}

void CExportOptionDlg::OnBnClickedBtnSetass()
{
	CSSASettings dlgSSASettings(TRUE);
	dlgSSASettings.m_strHdr = m_ExportParam.strFmtHeader.IsEmpty()?CTitleParams::GetDefASSHeader():m_ExportParam.strFmtHeader;
	dlgSSASettings.m_strPreCode = m_ExportParam.strPreCode;
	dlgSSASettings.m_strPostCode = m_ExportParam.strPostCode;
	if(IDOK != dlgSSASettings.DoModal())
		return;
	m_ExportParam.strFmtHeader = dlgSSASettings.m_strHdr;
	m_ExportParam.strPreCode = dlgSSASettings.m_strPreCode;
	m_ExportParam.strPostCode = dlgSSASettings.m_strPostCode;
}

void CExportOptionDlg::OnLvnEndScrollListFilefmt(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Internet Explorer 5.5 or greater.
	// The symbol _WIN32_IE must be >= 0x0560.
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateUIStates();
	*pResult = 0;
}

void CExportOptionDlg::OnNMClickListFilefmt(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	UpdateUIStates();
	*pResult = 0;
}

void CExportOptionDlg::OnBnClickedChkUnicode()
{
	m_ExportParam.vtFiles[m_nFocus].bUnicode = IsDlgButtonChecked(IDC_CHK_UNICODE);
	m_FileFmt.SetItemText(m_nFocus, 1, m_ExportParam.vtFiles[m_nFocus].bUnicode?_T("Unicode"):_T("Ansi"));
}

void CExportOptionDlg::OnBnClickedBtnBrow()
{
	UpdateData();
    BROWSEINFO bi;
    ZeroMemory(&bi,sizeof(BROWSEINFO));
    LPMALLOC pMalloc;
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    TCHAR szSelPath[MAX_PATH];
    if(pidl != NULL)
    {
        SHGetPathFromIDList(pidl, szSelPath);
		m_strOutPath = szSelPath;
        if(SUCCEEDED(SHGetMalloc(&pMalloc)))//pidl指向的对象用完应该释放，之前忽略了
        {
            pMalloc->Free(pidl);
            pMalloc->Release();
        }
    }
	UpdateData(FALSE);
}
