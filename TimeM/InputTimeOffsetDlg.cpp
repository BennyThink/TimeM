// InputTimeOffsetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "InputTimeOffsetDlg.h"
#include "TitleHelper.h"


// CInputTimeOffsetDlg dialog

IMPLEMENT_DYNAMIC(CInputTimeOffsetDlg, CDialog)

CInputTimeOffsetDlg::CInputTimeOffsetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputTimeOffsetDlg::IDD, pParent)
	, m_strTimeOffset(_T("00:00:00,000"))
{
	m_nOffset = 0;
}

CInputTimeOffsetDlg::~CInputTimeOffsetDlg()
{
}

void CInputTimeOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TIME_OFFSET, m_TimeOffset);
	DDX_Text(pDX, IDC_EDIT_TIME_OFFSET, m_strTimeOffset);
}


BEGIN_MESSAGE_MAP(CInputTimeOffsetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInputTimeOffsetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInputTimeOffsetDlg message handlers

BOOL CInputTimeOffsetDlg::OnInitDialog()
{
	m_strTimeOffset = CTitleHelper::FormatTime(m_nStart);
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CheckRadioButton(IDC_RBTN_SEL, IDC_RBTN_AFTER, IDC_RBTN_SEL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInputTimeOffsetDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	switch(GetCheckedRadioButton(IDC_RBTN_SEL, IDC_RBTN_AFTER))
	{
	case IDC_RBTN_SEL:
		m_dwFlag = 1;
		break;
	case IDC_RBTN_ALL:
		m_dwFlag = 2;
		break;
	case IDC_RBTN_MARKS:
		m_dwFlag = 3;
		break;
	case IDC_RBTN_AFTER:
		m_dwFlag = 4;
		break;
	}
	UpdateData();
	m_nOffset = CTitleHelper::GetTimeValue(m_strTimeOffset) - m_nStart;
	CDialog::OnOK();
}

void CInputTimeOffsetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
