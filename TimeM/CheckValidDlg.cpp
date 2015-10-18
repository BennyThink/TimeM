// CheckValidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "CheckValidDlg.h"
#include "TitleHelper.h"
#include "TimeMDoc.h"


// CCheckValidDlg dialog

IMPLEMENT_DYNAMIC(CCheckValidDlg, CDialog)

CCheckValidDlg::CCheckValidDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckValidDlg::IDD, pParent)
	, m_nChsCntMax(18)
	, m_nTitleLineCnt(2)
	, m_strMaxTime(_T("00:00:05,000"))
	, m_strMinTime(_T("00:00:00,500"))
{

}

CCheckValidDlg::~CCheckValidDlg()
{
}

void CCheckValidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_cbtn1);
	DDX_Control(pDX, IDC_BUTTON2, m_cbtn2);
	DDX_Control(pDX, IDC_BUTTON3, m_cbtn3);
	DDX_Control(pDX, IDC_BUTTON4, m_cbtn4);
	DDX_Control(pDX, IDC_BUTTON5, m_cbtn5);
	DDX_Control(pDX, IDC_BUTTON6, m_cbtn6);
	DDX_Control(pDX, IDC_BUTTON7, m_cbtn7);
	DDX_Control(pDX, IDC_EDIT2, m_TimeMax);
	DDX_Control(pDX, IDC_EDIT3, m_TimeMin);
	DDX_Text(pDX, IDC_EDIT1, m_nChsCntMax);
	DDX_Text(pDX, IDC_EDIT4, m_nTitleLineCnt);
	DDX_Text(pDX, IDC_EDIT2, m_strMaxTime);
	DDX_Text(pDX, IDC_EDIT3, m_strMinTime);
	DDX_Control(pDX, IDC_LIST1, m_ListCheck);
}

BEGIN_MESSAGE_MAP(CCheckValidDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCheckValidDlg::OnBnClickedOk)
	ON_MESSAGE(MSG_QUERYCELLTEXTCOLOR, &CCheckValidDlg::OnQueryCellTextColor)
	ON_MESSAGE(MSG_QUERYCELLBKCOLOR, &CCheckValidDlg::OnQueryCellBkColor)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CCheckValidDlg::OnLvnGetdispinfoList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CCheckValidDlg::OnLvnItemchangedList1)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BTN_SELALL, &CCheckValidDlg::OnBnClickedBtnSelall)
	ON_BN_CLICKED(IDC_BTN_CLEAROVERLAP, &CCheckValidDlg::OnBnClickedBtnClearoverlap)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CCheckValidDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_cbtn1.SetColor(RGB(128, 128, 255));
	m_cbtn2.SetColor(RGB(255, 255, 0));
	m_cbtn3.SetColor(RGB(0,	255, 128));
	m_cbtn4.SetColor(RGB(255, 128, 255));
	m_cbtn5.SetColor(RGB(255, 128, 64));
	m_cbtn6.SetColor(RGB(255, 128, 128));
	m_cbtn7.SetColor(RGB(64, 128, 128));
	CheckDlgButton(IDC_CHECK1, BST_CHECKED);
	CheckDlgButton(IDC_CHECK2, BST_CHECKED);
	CheckDlgButton(IDC_CHECK3, BST_CHECKED);
	CheckDlgButton(IDC_CHECK4, BST_CHECKED);
	CheckDlgButton(IDC_CHECK5, BST_CHECKED);
	CheckDlgButton(IDC_CHECK6, BST_CHECKED);
	CheckDlgButton(IDC_CHECK7, BST_CHECKED);

	m_ListCheck.InsertColumn(0, _T("´íÎóÐÐºÅ"), 0, 60);
	m_ListCheck.InsertColumn(1, _T("´íÎóÀàÐÍ"), 0, 200);

	CRect rcClient;
	GetWindowRect(rcClient);
	m_sizeMin.cx = rcClient.Width();
	m_sizeMin.cy = rcClient.Height();

	m_ListCheck.GetWindowRect(rcClient);
	m_sizeList.cx = rcClient.Width();
	m_sizeList.cy = rcClient.Height();

	return TRUE;
}

// CCheckValidDlg message handlers

void CCheckValidDlg::OnBnClickedOk()
{
	UpdateData();
	m_pDoc->ChkClear();
	if(IsDlgButtonChecked(IDC_CHECK1))
	{
		m_pDoc->ChkExceedSpecChs(m_nChsCntMax);
	}
	if(IsDlgButtonChecked(IDC_CHECK2))
	{
		m_pDoc->ChkStartMoreEnd();
	}
	if(IsDlgButtonChecked(IDC_CHECK3))
	{
		m_pDoc->ChkTimeOverlap();
	}
	if(IsDlgButtonChecked(IDC_CHECK4))
	{
		m_pDoc->ChkTimeEmpty();
	}
	if(IsDlgButtonChecked(IDC_CHECK5))
	{
		m_pDoc->ChkTimeMoreThan(CTitleHelper::GetTimeValue(m_strMaxTime));
	}
	if(IsDlgButtonChecked(IDC_CHECK6))
	{
		m_pDoc->ChkTimeLessThan(CTitleHelper::GetTimeValue(m_strMinTime));
	}
	if(IsDlgButtonChecked(IDC_CHECK7))
	{
		m_pDoc->ChkTitleLineMoreThan(m_nTitleLineCnt);
	}
	MAP_CHKPARAMS::iterator iterChk;
	for(iterChk = m_pDoc->m_mapChkParams.begin(); iterChk != m_pDoc->m_mapChkParams.end(); iterChk ++)
	{
		m_pDoc->m_vtChkParams.push_back(iterChk->second);
	}
	m_ListCheck.SetItemCount(m_pDoc->m_vtChkParams.size());
}

LRESULT CCheckValidDlg::OnQueryCellTextColor(WPARAM wParam, LPARAM lParam)
{
	return GetSysColor(COLOR_WINDOWTEXT);
}

LRESULT CCheckValidDlg::OnQueryCellBkColor(WPARAM wParam, LPARAM lParam)
{
	switch(m_pDoc->m_vtChkParams[wParam].nErrType)
	{
	case 1:
		return m_cbtn1.GetColor();
	case 2:
		return m_cbtn2.GetColor();
	case 3:
		return m_cbtn3.GetColor();
	case 4:
		return m_cbtn4.GetColor();
	case 5:
		return m_cbtn5.GetColor();
	case 6:
		return m_cbtn6.GetColor();
	case 7:
		return m_cbtn7.GetColor();
	}
	return GetSysColor(COLOR_WINDOW);
}

void CCheckValidDlg::OnLvnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here

	if((LVIF_TEXT == (pDispInfo->item.mask&LVIF_TEXT))
		&&(pDispInfo->item.pszText != NULL))
	{
		CString strInfo;
		switch(pDispInfo->item.iSubItem)
		{
		case 0:
			strInfo.Format(_T("%d"), m_pDoc->m_vtChkParams[pDispInfo->item.iItem].nTitleId + 1);
			StringCbCopy(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, strInfo);
			break;
		case 1:
			switch(m_pDoc->m_vtChkParams[pDispInfo->item.iItem].nErrType)
			{
			case 1:
				strInfo.LoadString(IDS_CHKERRINFO1);
				break;
			case 2:
				strInfo.LoadString(IDS_CHKERRINFO2);
				break;
			case 3:
				strInfo.LoadString(IDS_CHKERRINFO3);
				break;
			case 4:
				strInfo.LoadString(IDS_CHKERRINFO4);
				break;
			case 5:
				strInfo.LoadString(IDS_CHKERRINFO5);
				break;
			case 6:
				strInfo.LoadString(IDS_CHKERRINFO6);
				break;
			case 7:
				strInfo.LoadString(IDS_CHKERRINFO7);
				break;
			}
			StringCbCopy(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, strInfo);
			break;
		}
	}
	*pResult = 0;
}

void CCheckValidDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	if(((pNMLV->uNewState&LVIS_FOCUSED) == LVIS_FOCUSED)
		&&((pNMLV->uOldState&LVIS_FOCUSED) == 0))
	{
		m_pDoc->SetCurrentPos(m_pDoc->m_vtChkParams[pNMLV->iItem].nTitleId);
		m_pDoc->UpdateRefWin();
	}
	*pResult = 0;
}

void CCheckValidDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDCtl == IDC_LIST1)
	{
		m_ListCheck.SendChildNotifyLastMsg();
		return;
	}

	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CCheckValidDlg::OnBnClickedBtnSelall()
{
	int nSize = m_pDoc->m_vtChkParams.size();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		m_pDoc->SelectItem(m_pDoc->m_vtChkParams[iItem].nTitleId);
	}
}

void CCheckValidDlg::OnBnClickedBtnClearoverlap()
{
	// TODO: Add your control notification handler code here
	m_pDoc->ClearOverlap();
}

void CCheckValidDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = m_sizeMin.cx;
	lpMMI->ptMinTrackSize.y = m_sizeMin.cy;
}

void CCheckValidDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(IsWindow(m_ListCheck.m_hWnd))
	{
		CRect rcWnd;
		GetWindowRect(rcWnd);

		m_ListCheck.SetWindowPos(&wndTop, 0, 0,
			(m_sizeList.cx + rcWnd.Width() - m_sizeMin.cx),
			(m_sizeList.cy + rcWnd.Height() - m_sizeMin.cy),
			SWP_NOMOVE|SWP_NOZORDER);
	}
}
