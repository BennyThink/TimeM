// MinIntervalSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "MinIntervalSettingDlg.h"


// CMinIntervalSettingDlg dialog

IMPLEMENT_DYNAMIC(CMinIntervalSettingDlg, CDialog)

CMinIntervalSettingDlg::CMinIntervalSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMinIntervalSettingDlg::IDD, pParent)
	, m_strMinInterval(_T(""))
{

}

CMinIntervalSettingDlg::~CMinIntervalSettingDlg()
{
}

void CMinIntervalSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TIME_OFFSET, m_EditOffset);
	DDX_Text(pDX, IDC_EDIT_TIME_OFFSET, m_strMinInterval);
}


BEGIN_MESSAGE_MAP(CMinIntervalSettingDlg, CDialog)
END_MESSAGE_MAP()


// CMinIntervalSettingDlg message handlers
