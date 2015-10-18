// TitleFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "TitleFilterDlg.h"


// CTitleFilterDlg dialog

IMPLEMENT_DYNAMIC(CTitleFilterDlg, CDialog)

CTitleFilterDlg::CTitleFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTitleFilterDlg::IDD, pParent)
	, m_strSpecChar(_T(""))
	, m_bEmpty(FALSE)
	, m_bSpecChar(FALSE)
{

}

CTitleFilterDlg::~CTitleFilterDlg()
{
}

void CTitleFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPECCHAR, m_strSpecChar);
	DDX_Check(pDX, IDC_CHK_REMOVEEMPTY, m_bEmpty);
	DDX_Check(pDX, IDC_CHK_REMOVESPECCHAR, m_bSpecChar);
}


BEGIN_MESSAGE_MAP(CTitleFilterDlg, CDialog)
	ON_BN_CLICKED(IDC_CHK_REMOVESPECCHAR, &CTitleFilterDlg::OnBnClickedChkRemovespecchar)
	ON_BN_CLICKED(IDC_CHK_REMOVEEMPTY, &CTitleFilterDlg::OnBnClickedChkRemoveempty)
END_MESSAGE_MAP()


void CTitleFilterDlg::UpdateUIStates()
{
	CWnd* pOK = GetDlgItem(IDOK);
	if(pOK != NULL)
	{
		pOK->EnableWindow(IsDlgButtonChecked(IDC_CHK_REMOVESPECCHAR)|IsDlgButtonChecked(IDC_CHK_REMOVEEMPTY));
	}

	CWnd* pEdit = GetDlgItem(IDC_EDIT_SPECCHAR);
	if(pEdit != NULL)
	{
		pEdit->EnableWindow(IsDlgButtonChecked(IDC_CHK_REMOVESPECCHAR));
	}
}

// CTitleFilterDlg message handlers

void CTitleFilterDlg::OnBnClickedChkRemovespecchar()
{
	// TODO: Add your control notification handler code here
	UpdateUIStates();
}

BOOL CTitleFilterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	UpdateUIStates();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTitleFilterDlg::OnBnClickedChkRemoveempty()
{
	// TODO: Add your control notification handler code here
	UpdateUIStates();
}
