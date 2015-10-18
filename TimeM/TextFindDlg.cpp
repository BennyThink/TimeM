// TextFindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "TextFindDlg.h"


// CTextFindDlg dialog

IMPLEMENT_DYNAMIC(CTextFindDlg, CDialog)

CTextFindDlg::CTextFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextFindDlg::IDD, pParent)
	, m_bCaseSense(FALSE)
	, m_strToFind(_T(""))
{

}

CTextFindDlg::~CTextFindDlg()
{
}

void CTextFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_CASESENSE, m_bCaseSense);
	DDX_Text(pDX, IDC_EDIT_TOFIND, m_strToFind);
}


BEGIN_MESSAGE_MAP(CTextFindDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTextFindDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTextFindDlg message handlers

BOOL CTextFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_bDNDirect = TRUE;
	CheckRadioButton(IDC_RBTN_UPDIRECT, IDC_RBTN_DNDIRECT, IDC_RBTN_DNDIRECT);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTextFindDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_bDNDirect = (BST_CHECKED == IsDlgButtonChecked(IDC_RBTN_DNDIRECT));
	OnOK();
}
