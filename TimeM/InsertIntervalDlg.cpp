// InsertIntervalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "InsertIntervalDlg.h"
#include "TitleHelper.h"

// CInsertIntervalDlg dialog

IMPLEMENT_DYNAMIC(CInsertIntervalDlg, CDialog)

CInsertIntervalDlg::CInsertIntervalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsertIntervalDlg::IDD, pParent)
	, m_strTitle(_T(""))
	, m_strStartTime(_T("00:00:00,000"))
	, m_strEndTime(_T("00:00:00,000"))
{
	m_pmapListInterval	= NULL;
	m_pDoc				= NULL;
	m_nInterval			= 10000;
	m_nSelTitlePos		= -1;
}

CInsertIntervalDlg::~CInsertIntervalDlg()
{
}

void CInsertIntervalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_START, m_EditStart);
	DDX_Control(pDX, IDC_EDIT_END, m_EditEnd);
	DDX_Text(pDX, IDC_EDIT_SUBTITLE, m_strTitle);
	DDX_Text(pDX, IDC_EDIT_START, m_strStartTime);
	DDX_Text(pDX, IDC_EDIT_END, m_strEndTime);
	DDX_Control(pDX, IDC_LIST_INTERVAL, m_ListInterval);
}


BEGIN_MESSAGE_MAP(CInsertIntervalDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInsertIntervalDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_INTERVAL, &CInsertIntervalDlg::OnLvnItemchangedListInterval)
	ON_EN_CHANGE(IDC_EDIT_END, &CInsertIntervalDlg::OnEnChangeEditEnd)
	ON_EN_CHANGE(IDC_EDIT_START, &CInsertIntervalDlg::OnEnChangeEditStart)
END_MESSAGE_MAP()


// CInsertIntervalDlg message handlers

void CInsertIntervalDlg::OnBnClickedOk()
{
	UpdateData();

	if(m_nSelTitlePos > 0)
		m_pDoc->InsertTitle(m_nSelTitlePos, m_strStartTime, m_strEndTime, m_strTitle);
}

BOOL CInsertIntervalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ListInterval.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// TODO:  Add extra initialization here
	CString strInfo;
	strInfo.LoadString(IDS_POS_COLNAME);
	m_ListInterval.InsertColumn(0, strInfo, 0, 40);
	strInfo.LoadString(IDS_INTERVAL_COLNAME);
	m_ListInterval.InsertColumn(1, strInfo, 0, 120);

	LISTSEL_MAP::iterator iterMap;
	if(NULL != m_pmapListInterval)
	{
		for(iterMap = m_pmapListInterval->begin(); iterMap != m_pmapListInterval->end(); iterMap ++)
		{
			int nItem = m_ListInterval.InsertItem(m_ListInterval.GetItemCount(), _T(""));
			strInfo.Format(_T("%d"), iterMap->first);
			m_ListInterval.SetItemText(nItem, 0, strInfo);

			m_ListInterval.SetItemText(nItem, 1, CTitleHelper::FormatTime(iterMap->second));
			m_ListInterval.SetItemData(nItem, iterMap->first);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInsertIntervalDlg::OnLvnItemchangedListInterval(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(((pNMLV->uNewState&LVIS_FOCUSED) == LVIS_FOCUSED)
		&&((pNMLV->uOldState&LVIS_FOCUSED) == 0))
	{
		m_nSelTitlePos = m_ListInterval.GetItemData(pNMLV->iItem);
		m_pDoc->SetCurrentPos(m_nSelTitlePos);
		PTITLE_UNIT pUnit1 = m_pDoc->m_Action.GetItem(m_nSelTitlePos - 1, FALSE);
		m_strStartTime = CTitleHelper::FormatTime(pUnit1->nEnd + 1);

		PTITLE_UNIT pUnit2 = m_pDoc->m_Action.GetItem(m_nSelTitlePos, FALSE);
		int nEndTime = CTitleHelper::GetTimeValue(m_strStartTime) + m_nInterval;

		if(nEndTime > pUnit2->nStart)
		{
			nEndTime = pUnit2->nStart - 1;
		}

		m_strEndTime = CTitleHelper::FormatTime(nEndTime);
		m_pDoc->UpdateRefWin();
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CInsertIntervalDlg::OnEnChangeEditEnd()
{
	UpdateData();
	m_nInterval = CTitleHelper::GetTimeValue(m_strEndTime) - CTitleHelper::GetTimeValue(m_strStartTime);
}

void CInsertIntervalDlg::OnEnChangeEditStart()
{
	UpdateData();
	m_nInterval = CTitleHelper::GetTimeValue(m_strEndTime) - CTitleHelper::GetTimeValue(m_strStartTime);
}
