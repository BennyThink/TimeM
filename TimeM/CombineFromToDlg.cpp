// CombineFromToDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "CombineFromToDlg.h"


// CCombineFromToDlg dialog

IMPLEMENT_DYNAMIC(CCombineFromToDlg, CDialog)

CCombineFromToDlg::CCombineFromToDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCombineFromToDlg::IDD, pParent)
	, m_nLineFrom(0)
	, m_nLineTo(0)
{

}

CCombineFromToDlg::~CCombineFromToDlg()
{
}

void CCombineFromToDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FROM, m_nLineFrom);
	DDX_Text(pDX, IDC_EDIT_TO, m_nLineTo);
}


BEGIN_MESSAGE_MAP(CCombineFromToDlg, CDialog)
END_MESSAGE_MAP()


// CCombineFromToDlg message handlers
