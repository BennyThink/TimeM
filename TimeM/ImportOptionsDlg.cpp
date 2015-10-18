// ImportOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "ImportOptionsDlg.h"


// CImportOptionsDlg dialog

IMPLEMENT_DYNAMIC(CImportOptionsDlg, CDialog)

CImportOptionsDlg::CImportOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportOptionsDlg::IDD, pParent)
{
	m_bSingleLine = FALSE;
}

CImportOptionsDlg::~CImportOptionsDlg()
{
}

void CImportOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImportOptionsDlg, CDialog)
END_MESSAGE_MAP()


// CImportOptionsDlg message handlers

BOOL CImportOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CheckRadioButton(IDC_RBTN_SINGLELINE, IDC_RBTN_SPLITNULLLINE, IDC_RBTN_SPLITNULLLINE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CImportOptionsDlg::GetSingleLineOption()
{
	return m_bSingleLine;
}

void CImportOptionsDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_bSingleLine = IsDlgButtonChecked(IDC_RBTN_SINGLELINE);
	CDialog::OnOK();
}
