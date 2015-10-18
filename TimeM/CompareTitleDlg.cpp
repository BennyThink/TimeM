// CompaireTitleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "CompareTitleDlg.h"

#include "TitleFile.h"
#include "TitleHelper.h"

// CCompareTitleDlg dialog
#define	MSG_SYNCLISTSCROLL	(WM_USER+101)

IMPLEMENT_DYNAMIC(CCompareTitleDlg, CDialog)

CCompareTitleDlg::CCompareTitleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompareTitleDlg::IDD, pParent)
	, m_strStdFile(_T(""))
	, m_strTransFile(_T(""))
{
	m_bLockScroll	= FALSE;
}

CCompareTitleDlg::~CCompareTitleDlg()
{
}

void CCompareTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STDTITLE, m_ListTitleStd);
	DDX_Control(pDX, IDC_LIST_TRANSTITLE, m_ListTitleTrans);
	DDX_Text(pDX, IDC_EDIT_FILESTD, m_strStdFile);
	DDX_Text(pDX, IDC_EDIT_FILETRANS, m_strTransFile);
}


BEGIN_MESSAGE_MAP(CCompareTitleDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSTD, &CCompareTitleDlg::OnBnClickedBtnBrowstd)
	ON_BN_CLICKED(IDC_BTN_BROWTRANS, &CCompareTitleDlg::OnBnClickedBtnBrowtrans)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_STDTITLE, &CCompareTitleDlg::OnLvnGetdispinfoListStdtitle)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TRANSTITLE, &CCompareTitleDlg::OnLvnGetdispinfoListTranstitle)
	ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST_STDTITLE, &CCompareTitleDlg::OnLvnEndScrollListStdtitle)
	ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST_TRANSTITLE, &CCompareTitleDlg::OnLvnEndScrollListTranstitle)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STDTITLE, &CCompareTitleDlg::OnLvnItemchangedListStdtitle)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TRANSTITLE, &CCompareTitleDlg::OnLvnItemchangedListTranstitle)
	ON_BN_CLICKED(IDC_CHK_SYNCSCROLL, &CCompareTitleDlg::OnBnClickedChkSyncscroll)
	ON_MESSAGE(MSG_SYNCLISTSCROLL, &CCompareTitleDlg::OnSyncListScroll)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BTN_SNAPSEL, &CCompareTitleDlg::OnBnClickedBtnSnapsel)
END_MESSAGE_MAP()

BOOL CCompareTitleDlg::DoFilePrompt(CString& strFile)
{
	TCHAR szFilter[] = _T("×ÖÄ»ÎÄ¼þ (*.srt;*.ass;*.ssa)|*.srt;*.ass;*.ssa|All Files (*.*)|*.*||");
	CFileDialog dlgOpen(TRUE, 0, 0, 0, szFilter);
	if(dlgOpen.DoModal() == IDOK)
	{
		strFile = dlgOpen.GetPathName();
		return TRUE;
	}
	return FALSE;
}

BOOL CCompareTitleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strCol;
	strCol.LoadString(IDS_COL_TITLE_START);
	m_ListTitleStd.InsertColumn(0, strCol, 0, 85);
	m_ListTitleTrans.InsertColumn(0, strCol, 0, 85);

	strCol.LoadString(IDS_COL_TITLE_END);
	m_ListTitleStd.InsertColumn(1, strCol, 0, 85);
	m_ListTitleTrans.InsertColumn(1, strCol, 0, 85);

	strCol.LoadString(IDS_COL_TITLE_CONTENT);
	m_ListTitleStd.InsertColumn(2, strCol, 0, 400);
	m_ListTitleTrans.InsertColumn(2, strCol, 0, 400);

	CheckDlgButton(IDC_CHK_SYNCSCROLL, BST_CHECKED);
	m_ListTitleStd.AttachCompare(&m_TitleRel, 0);
	m_ListTitleTrans.AttachCompare(&m_TitleRel, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CCompareTitleDlg message handlers

void CCompareTitleDlg::OnBnClickedBtnBrowstd()
{
	CFile fileOpen;
	BOOL	bUnicode;
	if((DoFilePrompt(m_strStdFile))
		&&(fileOpen.Open(m_strStdFile, CFile::modeRead)))
	{
		AfxGetApp()->BeginWaitCursor();
		CTitleFile::DeleteContents(m_TitleRel.GetLeftVT());
		CString strHdr;
		CTitleFile::ReadTitleFile(&fileOpen, m_TitleRel.GetLeftVT(), strHdr, bUnicode);

		m_TitleRel.BuildRelation(BUILD_TYPE_BYNORMAL);
		m_ListTitleStd.SetItemCount(m_TitleRel.GetRelCount());
		m_ListTitleTrans.SetItemCount(m_TitleRel.GetRelCount());
		AfxGetApp()->EndWaitCursor();
	}
	UpdateData(FALSE);
}

void CCompareTitleDlg::OnBnClickedBtnBrowtrans()
{
	CFile fileOpen;
	BOOL	bUnicode;
	if((DoFilePrompt(m_strTransFile))
		&&(fileOpen.Open(m_strTransFile, CFile::modeRead)))
	{
		AfxGetApp()->BeginWaitCursor();
		CTitleFile::DeleteContents(m_TitleRel.GetRightVT());
		CString strHdr;
		CTitleFile::ReadTitleFile(&fileOpen, m_TitleRel.GetRightVT(), strHdr, bUnicode);

		m_TitleRel.BuildRelation(BUILD_TYPE_BYNORMAL);
		m_ListTitleStd.SetItemCount(m_TitleRel.GetRelCount());
		m_ListTitleTrans.SetItemCount(m_TitleRel.GetRelCount());
		AfxGetApp()->EndWaitCursor();
	}
	UpdateData(FALSE);
}

void CCompareTitleDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	switch(nIDCtl)
	{
	case IDC_LIST_STDTITLE:
		m_ListTitleStd.SendChildNotifyLastMsg();
		return;
	case IDC_LIST_TRANSTITLE:
		m_ListTitleTrans.SendChildNotifyLastMsg();
		return;
	}

	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CCompareTitleDlg::GetDispInfoTitleItem(NMLVDISPINFO *pDispInfo, PTITLE_UNIT pUnit)
{
	if(pDispInfo->item.mask&LVIF_TEXT)
	{
		if(pUnit != NULL)
		{
			switch(pDispInfo->item.iSubItem)
			{
			case 0:
				StringCbCopy(pDispInfo->item.pszText, MAX_PATH,
					CTitleHelper::FormatTime(pUnit->nStart));
				break;
			case 1:
				StringCbCopy(pDispInfo->item.pszText, MAX_PATH,
					CTitleHelper::FormatTime(pUnit->nEnd));
				break;
			case 2:
				StringCbCopy(pDispInfo->item.pszText, MAX_PATH, pUnit->content);
				break;
			}
		}
	}
}

void CCompareTitleDlg::OnLvnGetdispinfoListStdtitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	PAIR_REL relPair = m_TitleRel.GetRelItem(pDispInfo->item.iItem);
	PTITLE_UNIT pUnit = CTitleHelper::GetTitleItem(relPair.nLeftID, m_TitleRel.GetLeftVT());
	GetDispInfoTitleItem(pDispInfo, pUnit);
	*pResult = 0;
}

void CCompareTitleDlg::OnLvnGetdispinfoListTranstitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	PAIR_REL relPair = m_TitleRel.GetRelItem(pDispInfo->item.iItem);
	PTITLE_UNIT pUnit = CTitleHelper::GetTitleItem(relPair.nRightID, m_TitleRel.GetRightVT());
	GetDispInfoTitleItem(pDispInfo, pUnit);
	*pResult = 0;
}

void CCompareTitleDlg::OnLvnEndScrollListStdtitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Internet Explorer 5.5 or greater.
	// The symbol _WIN32_IE must be >= 0x0560.
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL);
	*pResult = 0;
}

void CCompareTitleDlg::OnLvnEndScrollListTranstitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Internet Explorer 5.5 or greater.
	// The symbol _WIN32_IE must be >= 0x0560.
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL, 1);
	*pResult = 0;
}

void CCompareTitleDlg::OnLvnItemchangedListStdtitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL);
	*pResult = 0;
}

void CCompareTitleDlg::OnLvnItemchangedListTranstitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL, 1);
	*pResult = 0;
}

void CCompareTitleDlg::SyncListScroll(const CListCtrl& listSrc, CListCtrl& listTag)
{
	if(IsDlgButtonChecked(IDC_CHK_SYNCSCROLL))
	{
		if(m_bLockScroll)
		{
		}
		else
		{
			m_bLockScroll = TRUE;
			CRect rcItem;
			int nSrcTop = listSrc.GetTopIndex();
			int nTagTop = listTag.GetTopIndex();
			if(nTagTop != nSrcTop)
			{
				listTag.GetItemRect(0, rcItem, LVIR_BOUNDS);
				listTag.Scroll(CSize(0, (nSrcTop - nTagTop)*rcItem.Height()));
			}
			m_bLockScroll = FALSE;
		}
	}
}

void CCompareTitleDlg::OnBnClickedChkSyncscroll()
{
	PostMessage(MSG_SYNCLISTSCROLL);
}

LRESULT CCompareTitleDlg::OnSyncListScroll(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 0)
		SyncListScroll(m_ListTitleStd, m_ListTitleTrans);
	else
		SyncListScroll(m_ListTitleTrans, m_ListTitleStd);
	return 0;
}

BOOL CCompareTitleDlg::IsSelectedMatch()
{
	if(m_ListTitleStd.GetSelectedCount() == 0)
		return FALSE;
	if(m_ListTitleStd.GetSelectedCount() != m_ListTitleTrans.GetSelectedCount())
		return FALSE;
	LISTSEL_MAP mapStdSel, mapTransSel;
	POSITION posSel = m_ListTitleStd.GetFirstSelectedItemPosition();
	int nSel;
	while(posSel != NULL)
	{
		nSel = m_ListTitleStd.GetNextSelectedItem(posSel);
		mapStdSel.insert(INT_PAIR(nSel, nSel));
	}

	posSel = m_ListTitleTrans.GetFirstSelectedItemPosition();
	while(posSel != NULL)
	{
		nSel = m_ListTitleTrans.GetNextSelectedItem(posSel);
		mapTransSel.insert(INT_PAIR(nSel, nSel));
	}

	LISTSEL_MAP::const_iterator iter;

	iter = mapStdSel.begin();
	for(int iStd = 0; iStd < (int)mapStdSel.size(); iStd ++)
	{
		if((mapStdSel.begin()->first + iStd) != iter->first)
			return FALSE;
		iter ++;
	}

	iter = mapTransSel.begin();
	for(int iTrans = 0; iTrans < (int)mapTransSel.size(); iTrans ++)
	{
		if((mapTransSel.begin()->first + iTrans) != iter->first)
			return FALSE;
		iter ++;
	}

	return TRUE;
}

void CCompareTitleDlg::OnBnClickedBtnSnapsel()
{
	if(!IsSelectedMatch())
	{
		AfxMessageBox(IDS_SEL_SINGLELINE_LR);
		return;
	}

	POSITION posSel = m_ListTitleStd.GetFirstSelectedItemPosition();
	int nSelStd = -1;
	while(posSel != NULL)
	{
		nSelStd = m_ListTitleStd.GetNextSelectedItem(posSel);
	}

	int nSelTrans = -1;
	posSel = m_ListTitleTrans.GetFirstSelectedItemPosition();
	while(posSel != NULL)
	{
		nSelTrans = m_ListTitleTrans.GetNextSelectedItem(posSel);
	}

	m_TitleRel.BandRelationNormal(nSelStd, nSelTrans);
	m_ListTitleStd.SetItemCount(m_TitleRel.GetRelCount());
	m_ListTitleTrans.SetItemCount(m_TitleRel.GetRelCount());
	m_ListTitleStd.Invalidate(FALSE);
	m_ListTitleTrans.Invalidate(FALSE);
}
