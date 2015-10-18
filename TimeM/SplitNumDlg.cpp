// SplitNumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "SplitNumDlg.h"


// CSplitNumDlg dialog

IMPLEMENT_DYNAMIC(CSplitNumDlg, CDialog)

CSplitNumDlg::CSplitNumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplitNumDlg::IDD, pParent)
	, m_nSplitNum(3)
{

}

CSplitNumDlg::~CSplitNumDlg()
{
}

void CSplitNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPLITNUM, m_nSplitNum);
}


BEGIN_MESSAGE_MAP(CSplitNumDlg, CDialog)
END_MESSAGE_MAP()


// CSplitNumDlg message handlers
