// CPSelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "CPSelDlg.h"


// CCPSelDlg dialog

IMPLEMENT_DYNAMIC(CCPSelDlg, CDialog)

CCPSelDlg::CCPSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCPSelDlg::IDD, pParent)
	, m_strCP(_T("ANSI"))
{

}

CCPSelDlg::~CCPSelDlg()
{
}

void CCPSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_CB_CPLIST, m_strCP);
}


BEGIN_MESSAGE_MAP(CCPSelDlg, CDialog)
END_MESSAGE_MAP()


// CCPSelDlg message handlers
