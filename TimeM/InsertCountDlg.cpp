// InsertCountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "InsertCountDlg.h"


// CInsertCountDlg dialog

IMPLEMENT_DYNAMIC(CInsertCountDlg, CDialog)

CInsertCountDlg::CInsertCountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsertCountDlg::IDD, pParent)
	, m_nLineCount(3)
	, m_strTimeInit(_T("00:00:01,000"))
{

}

CInsertCountDlg::~CInsertCountDlg()
{
}

void CInsertCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LINECOUNT, m_nLineCount);
	DDX_Control(pDX, IDC_EDIT_TIME_LEN, m_InitTime);
	DDX_Text(pDX, IDC_EDIT_TIME_LEN, m_strTimeInit);
}


BEGIN_MESSAGE_MAP(CInsertCountDlg, CDialog)
	ON_BN_CLICKED(IDC_CHK_INITTIME, &CInsertCountDlg::OnBnClickedChkInittime)
END_MESSAGE_MAP()


// CInsertCountDlg message handlers

void CInsertCountDlg::OnOK()
{
	m_bEmpty = IsDlgButtonChecked(IDC_RBTN_EMPTYLINE);
	m_bInitTime = IsDlgButtonChecked(IDC_CHK_INITTIME);

	CDialog::OnOK();
}

BOOL CInsertCountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckRadioButton(IDC_RBTN_EMPTYLINE, IDC_RBTN_SEQFILL, IDC_RBTN_EMPTYLINE);
	CheckDlgButton(IDC_CHK_INITTIME, FALSE);
	OnBnClickedChkInittime();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInsertCountDlg::OnBnClickedChkInittime()
{
	m_InitTime.EnableWindow(IsDlgButtonChecked(IDC_CHK_INITTIME));
}
