#include "stdafx.h"
#include "TimeM.h"
#include "AboutDlg.h"

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strCRCmt(_T(""))
{
	m_strCRCmt.LoadString(IDS_CR_COMMENT);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CRTEXT, m_strCRCmt);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CTimeMApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(212, 208, 200));
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch(pWnd->GetDlgCtrlID())
	{
	case IDC_EDIT_CRTEXT:
		break;
	default:
//		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(212, 208, 200));
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
		break;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
