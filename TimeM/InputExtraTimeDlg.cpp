// InputExtraTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "InputExtraTimeDlg.h"


// CInputExtraTimeDlg dialog

IMPLEMENT_DYNAMIC(CInputExtraTimeDlg, CDialog)

CInputExtraTimeDlg::CInputExtraTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputExtraTimeDlg::IDD, pParent)
	, m_strTimeExtra(_T("00:00:00,000"))
{

}

CInputExtraTimeDlg::~CInputExtraTimeDlg()
{
}

void CInputExtraTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TIME_OFFSET, m_TimeExtra);
	DDX_Text(pDX, IDC_EDIT_TIME_OFFSET, m_strTimeExtra);
}

BEGIN_MESSAGE_MAP(CInputExtraTimeDlg, CDialog)
END_MESSAGE_MAP()

// CInputExtraTimeDlg message handlers
