// TitleOptions.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "TitleOptions.h"

#include "TitleHelper.h"

// CTitleOptionsPage dialog

IMPLEMENT_DYNCREATE(CTitleOptionsPage, CPropertyPage)

CTitleOptionsPage::CTitleOptionsPage()
	: CPropertyPage(CTitleOptionsPage::IDD)
	, m_strTimeDelay(_T("00:00:00,000"))
	, m_strDefTitleLen(_T("00:00:00,000"))
	, m_strToolPath(_T(""))
{

}

CTitleOptionsPage::~CTitleOptionsPage()
{
}

void CTitleOptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TIME_OFFSET, m_EditTimeDelay);
	DDX_Control(pDX, IDC_EDIT_TIME_DEFLEN, m_EditTimeDefLen);
	DDX_Text(pDX, IDC_EDIT_TIME_OFFSET, m_strTimeDelay);
	DDX_Text(pDX, IDC_EDIT_TIME_DEFLEN, m_strDefTitleLen);
	DDX_Text(pDX, IDC_EDIT_ZIPTOOL, m_strToolPath);
}

BEGIN_MESSAGE_MAP(CTitleOptionsPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_BROWZIPTOOL, &CTitleOptionsPage::OnBnClickedBtnBrowziptool)
	ON_EN_KILLFOCUS(IDC_EDIT_TIME_OFFSET, &CTitleOptionsPage::OnEnKillfocusEditTimeOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_TIME_DEFLEN, &CTitleOptionsPage::OnEnKillfocusEditTimeDeflen)
	ON_BN_CLICKED(IDC_RBTN_BACK, &CTitleOptionsPage::OnBnClickedRbtnBack)
	ON_BN_CLICKED(IDC_RBTN_FORWARD, &CTitleOptionsPage::OnBnClickedRbtnForward)
END_MESSAGE_MAP()

void CTitleOptionsPage::SetTimeDelay(int nTimeDelay)
{
	m_nTimeDelay = nTimeDelay;
	m_strTimeDelay = CTitleHelper::FormatTime(m_nTimeDelay>0?m_nTimeDelay:-m_nTimeDelay);
}

void CTitleOptionsPage::SetTitleDefLen(int nDefLen)
{
	m_nTitleDeflen = nDefLen;
	m_strDefTitleLen = CTitleHelper::FormatTime(m_nTitleDeflen);
}

// CTitleOptionsPage message handlers
BOOL CTitleOptionsPage::OnInitDialog()
{
	SetTimeDelay(AfxGetApp()->GetProfileInt(_T("Settings"), _T("StartDelay"), -300));
	SetTitleDefLen(AfxGetApp()->GetProfileInt(_T("Settings"), _T("DefaultLength"), 1000));
	SetZipPath(AfxGetApp()->GetProfileString(_T("Settings"), _T("ZipToolPath")));
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CheckRadioButton(IDC_RBTN_FORWARD, IDC_RBTN_BACK, m_nTimeDelay<0?IDC_RBTN_FORWARD:IDC_RBTN_BACK);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTitleOptionsPage::OnBnClickedBtnBrowziptool()
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[] = _T("压缩工具(*.exe)|*.exe|所有文件(*.*)|*.*||");
	CFileDialog openTools(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFilters);
	if(IDOK == openTools.DoModal())
	{
		m_strToolPath = openTools.GetPathName();
		AfxGetApp()->WriteProfileString( _T("Settings"), _T("ZipToolPath"), GetZipPath());
		UpdateData(FALSE);
	}
}

void CTitleOptionsPage::OnEnKillfocusEditTimeOffset()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nTimeDelay = (IDC_RBTN_BACK == GetCheckedRadioButton(IDC_RBTN_FORWARD, IDC_RBTN_BACK))
		?CTitleHelper::GetTimeValue(m_strTimeDelay):-CTitleHelper::GetTimeValue(m_strTimeDelay);
	AfxGetApp()->WriteProfileInt( _T("Settings"), _T("StartDelay"), GetTimeDelay());

}

void CTitleOptionsPage::OnEnKillfocusEditTimeDeflen()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nTitleDeflen = CTitleHelper::GetTimeValue(m_strDefTitleLen);
	AfxGetApp()->WriteProfileInt( _T("Settings"), _T("DefaultLength"), GetTitleDefLen());
}

void CTitleOptionsPage::OnBnClickedRbtnBack()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nTimeDelay = (IDC_RBTN_BACK == GetCheckedRadioButton(IDC_RBTN_FORWARD, IDC_RBTN_BACK))
		?CTitleHelper::GetTimeValue(m_strTimeDelay):-CTitleHelper::GetTimeValue(m_strTimeDelay);
	AfxGetApp()->WriteProfileInt( _T("Settings"), _T("StartDelay"), GetTimeDelay());

}

void CTitleOptionsPage::OnBnClickedRbtnForward()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nTimeDelay = (IDC_RBTN_BACK == GetCheckedRadioButton(IDC_RBTN_FORWARD, IDC_RBTN_BACK))
		?CTitleHelper::GetTimeValue(m_strTimeDelay):-CTitleHelper::GetTimeValue(m_strTimeDelay);
	AfxGetApp()->WriteProfileInt( _T("Settings"), _T("StartDelay"), GetTimeDelay());

}
