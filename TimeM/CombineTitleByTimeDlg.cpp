// CombineTitleByTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "CombineTitleByTimeDlg.h"
#include "TitleFile.h"
#include "TitleHelper.h"

#define	MSG_SYNCLISTSCROLL	(WM_USER+101)

// CCombineTitleByTimeDlg dialog

IMPLEMENT_DYNAMIC(CCombineTitleByTimeDlg, CDialog)

CCombineTitleByTimeDlg::CCombineTitleByTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCombineTitleByTimeDlg::IDD, pParent)
	, m_strStdFile(_T(""))
	, m_strTransFile(_T(""))
{
	m_bModified		= FALSE;
	m_bLockScroll	= FALSE;
	m_bStdUnicode	= FALSE;
}

CCombineTitleByTimeDlg::~CCombineTitleByTimeDlg()
{
}

void CCombineTitleByTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STDTITLE, m_ListTitleStd);
	DDX_Control(pDX, IDC_LIST_TRANSTITLE, m_ListTitleTrans);
	DDX_Text(pDX, IDC_EDIT_FILESTD, m_strStdFile);
	DDX_Text(pDX, IDC_EDIT_FILETRANS, m_strTransFile);
}

BEGIN_MESSAGE_MAP(CCombineTitleByTimeDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSTD, &CCombineTitleByTimeDlg::OnBnClickedBtnBrowstd)
	ON_BN_CLICKED(IDC_BTN_BROWTRANS, &CCombineTitleByTimeDlg::OnBnClickedBtnBrowtrans)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_STDTITLE, &CCombineTitleByTimeDlg::OnLvnGetdispinfoListStdtitle)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TRANSTITLE, &CCombineTitleByTimeDlg::OnLvnGetdispinfoListTranstitle)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BTN_SNAPAUTO, &CCombineTitleByTimeDlg::OnBnClickedBtnSnapauto)
	ON_BN_CLICKED(IDC_BTN_SNAPSEL, &CCombineTitleByTimeDlg::OnBnClickedBtnSnapsel)
	ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST_STDTITLE, &CCombineTitleByTimeDlg::OnLvnEndScrollListStdtitle)
	ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST_TRANSTITLE, &CCombineTitleByTimeDlg::OnLvnEndScrollListTranstitle)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STDTITLE, &CCombineTitleByTimeDlg::OnLvnItemchangedListStdtitle)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TRANSTITLE, &CCombineTitleByTimeDlg::OnLvnItemchangedListTranstitle)
	ON_BN_CLICKED(IDC_CHK_SYNCSCROLL, &CCombineTitleByTimeDlg::OnBnClickedChkSyncscroll)
	ON_MESSAGE(MSG_SYNCLISTSCROLL, &CCombineTitleByTimeDlg::OnSyncListScroll)
	ON_BN_CLICKED(IDSAVE, &CCombineTitleByTimeDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BTN_TO_LEFTEMPTY, &CCombineTitleByTimeDlg::OnBnClickedBtnToLeftempty)
	ON_BN_CLICKED(IDC_BTN_TO_RIGHTEMPTY, &CCombineTitleByTimeDlg::OnBnClickedBtnToRightempty)
END_MESSAGE_MAP()

BOOL CCombineTitleByTimeDlg::DoFilePrompt(CString& strFile)
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

// CCombineTitleByTimeDlg message handlers

BOOL CCombineTitleByTimeDlg::OnInitDialog()
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

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCombineTitleByTimeDlg::OnBnClickedBtnBrowstd()
{
	CFile fileOpen;
	if((DoFilePrompt(m_strStdFile))
		&&(fileOpen.Open(m_strStdFile, CFile::modeRead)))
	{
		AfxGetApp()->BeginWaitCursor();
		CTitleFile::DeleteContents(m_TitleRel.GetLeftVT());
		CTitleFile::ReadTitleFile(&fileOpen, m_TitleRel.GetLeftVT(), m_strFmtHdr, m_bStdUnicode);
		SaveModified();

		m_TitleRel.BuildRelation(BUILD_TYPE_ORGTEXT);
		m_ListTitleStd.SetItemCount(m_TitleRel.GetRelCount());
		m_ListTitleTrans.SetItemCount(m_TitleRel.GetRelCount());
		AfxGetApp()->EndWaitCursor();
	}
	UpdateData(FALSE);
}

void CCombineTitleByTimeDlg::OnBnClickedBtnBrowtrans()
{
	CFile fileOpen;
	BOOL	bUnicode;
	if((DoFilePrompt(m_strTransFile))
		&&(fileOpen.Open(m_strTransFile, CFile::modeRead)))
	{
		AfxGetApp()->BeginWaitCursor();
		CString strHdr;
		CTitleFile::DeleteContents(m_TitleRel.GetRightVT());
		CTitleFile::ReadTitleFile(&fileOpen, m_TitleRel.GetRightVT(), strHdr, bUnicode);

		m_TitleRel.BuildRelation(BUILD_TYPE_ORGTEXT);
		m_ListTitleStd.SetItemCount(m_TitleRel.GetRelCount());
		m_ListTitleTrans.SetItemCount(m_TitleRel.GetRelCount());
		AfxGetApp()->EndWaitCursor();
	}
	UpdateData(FALSE);
}

BOOL CCombineTitleByTimeDlg::DestroyWindow()
{

	return CDialog::DestroyWindow();
}

void CCombineTitleByTimeDlg::GetDispInfoTitleItem(NMLVDISPINFO *pDispInfo, PTITLE_UNIT pUnit)
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

void CCombineTitleByTimeDlg::OnLvnGetdispinfoListStdtitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	PAIR_REL relPair = m_TitleRel.GetRelItem(pDispInfo->item.iItem);
	PTITLE_UNIT pUnit = CTitleHelper::GetTitleItem(relPair.nLeftID, m_TitleRel.GetLeftVT());
	GetDispInfoTitleItem(pDispInfo, pUnit);
	*pResult = 0;
}

void CCombineTitleByTimeDlg::OnLvnGetdispinfoListTranstitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	PAIR_REL relPair = m_TitleRel.GetRelItem(pDispInfo->item.iItem);
	PTITLE_UNIT pUnit = CTitleHelper::GetTitleItem(relPair.nRightID, m_TitleRel.GetRightVT());
	GetDispInfoTitleItem(pDispInfo, pUnit);
	*pResult = 0;
}

void CCombineTitleByTimeDlg::OnLvnEndScrollListStdtitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Internet Explorer 5.5 or greater.
	// The symbol _WIN32_IE must be >= 0x0560.
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL);
	*pResult = 0;
}

void CCombineTitleByTimeDlg::OnLvnEndScrollListTranstitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Internet Explorer 5.5 or greater.
	// The symbol _WIN32_IE must be >= 0x0560.
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL, 1);
	*pResult = 0;
}

void CCombineTitleByTimeDlg::OnLvnItemchangedListStdtitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL);
	*pResult = 0;
}

void CCombineTitleByTimeDlg::OnLvnItemchangedListTranstitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL, 1);
	*pResult = 0;
}

void CCombineTitleByTimeDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
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

void CCombineTitleByTimeDlg::OnBnClickedBtnSnapauto()
{
	AfxGetApp()->BeginWaitCursor();
	m_TitleRel.BuildRelation(BUILD_TYPE_ORGTEXT);
	m_ListTitleStd.SetItemCount(m_TitleRel.GetRelCount());
	m_ListTitleTrans.SetItemCount(m_TitleRel.GetRelCount());
	AfxGetApp()->EndWaitCursor();
}

BOOL CCombineTitleByTimeDlg::IsSelectedMatch()
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

void CCombineTitleByTimeDlg::OnBnClickedBtnSnapsel()
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

	m_TitleRel.BandRelation(nSelStd, nSelTrans);
	m_ListTitleStd.SetItemCount(m_TitleRel.GetRelCount());
	m_ListTitleTrans.SetItemCount(m_TitleRel.GetRelCount());
	m_ListTitleStd.Invalidate(FALSE);
	m_ListTitleTrans.Invalidate(FALSE);
}

void CCombineTitleByTimeDlg::SyncListScroll(const CListCtrl& listSrc, CListCtrl& listTag)
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

void CCombineTitleByTimeDlg::OnBnClickedChkSyncscroll()
{
	PostMessage(MSG_SYNCLISTSCROLL);
}

LRESULT CCombineTitleByTimeDlg::OnSyncListScroll(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 0)
		SyncListScroll(m_ListTitleStd, m_ListTitleTrans);
	else
		SyncListScroll(m_ListTitleTrans, m_ListTitleStd);
	return 0;
}

void CCombineTitleByTimeDlg::OnBnClickedSave()
{
	CFile fileOpen;
	if(fileOpen.Open(m_strStdFile, CFile::modeWrite))
	{
		CString strExt = PathFindExtension(m_strStdFile);
		DWORD dwFmt = FMTFLAG_SRT;
		if(strExt.CompareNoCase(_T(".ssa")) == 0)
		{
			dwFmt = FMTFLAG_SSA;
		}
		else if(strExt.CompareNoCase(_T(".ass")) == 0)
		{
			dwFmt = FMTFLAG_ASS;
		}
		CTitleFile::WriteTitleFile(&fileOpen, m_TitleRel.GetLeftVT(), m_strFmtHdr, CString(_T("")), CString(_T("")), dwFmt, m_bStdUnicode);
		SaveModified();
	}
}

void CCombineTitleByTimeDlg::OnOK()
{
	PTITLE_UNIT pLeftUnit, pRightUnit;
	CString strRemain, strLeft, strRight;
	int nCnt = m_TitleRel.GetRelCount();
	for(int i = 0; i < nCnt; i++)
	{
		if(m_TitleRel.GetRelItem(i).nLeftID != -1)
		{
			strRemain.Empty();
			strLeft.Empty();
			strRight.Empty();
			int nLeft = m_TitleRel.GetRelItem(i).nLeftID;
			int nRight = m_TitleRel.GetRelItem(i).nRightID;
			if((nLeft != -1)
				&&(nRight != -1))
			{
				pLeftUnit = m_TitleRel.GetLeftVT()[nLeft];
				pRightUnit = m_TitleRel.GetRightVT()[nRight];
				CTitleHelper::BreakString(pRightUnit->content, strRight, strRemain);
				CTitleHelper::BreakString(pLeftUnit->content, strLeft, strRemain);
				pLeftUnit->content = strRight;
				pLeftUnit->content += _T("\r\n");
				pLeftUnit->content += strRemain;
			}
		}
	}

	m_ListTitleStd.Invalidate(FALSE);
	m_ListTitleTrans.Invalidate(FALSE);
	SetModifiedFlag();
}

void CCombineTitleByTimeDlg::OnCancel()
{
	if(IsModified())
	{
		int nRet = AfxMessageBox(IDS_DOC_SAVEPROMPT, MB_YESNOCANCEL);
		switch(nRet)
		{
		case IDYES:
			break;
		case IDNO:
			OnBnClickedSave();
			break;
		case IDCANCEL:
			return;
		}
	}

	CDialog::OnCancel();
}

void CCombineTitleByTimeDlg::OnBnClickedBtnToRightempty()
{
	BOOL bFound = FALSE;
	int nFocus = m_ListTitleTrans.GetFocusItem();
	int iPos = nFocus + 1;
	if(m_TitleRel.GetRelCount() > 0)
	{
		do
		{
			if(m_TitleRel.GetRelItem(iPos).nRightID == -1)
			{
				m_ListTitleTrans.SetFocusItem(iPos);
				m_ListTitleTrans.EnsureVisible(iPos, FALSE);
				bFound = TRUE;
				break;
			}
			iPos ++;
			if(iPos == m_TitleRel.GetRelCount())
				iPos = (nFocus == -1)?-1:0;
		}while(iPos != nFocus);
	}
	if(!bFound)
		AfxMessageBox(IDS_NOTFOUND_OTHERS);
}

void CCombineTitleByTimeDlg::OnBnClickedBtnToLeftempty()
{
	BOOL bFound = FALSE;
	int nFocus = m_ListTitleStd.GetFocusItem();
	int iPos = nFocus + 1;
	if(m_TitleRel.GetRelCount() > 0)
	{
		do
		{
			if(m_TitleRel.GetRelItem(iPos).nLeftID == -1)
			{
				m_ListTitleStd.SetFocusItem(iPos);
				m_ListTitleStd.EnsureVisible(iPos, FALSE);
				bFound = TRUE;
				break;
			}
			iPos ++;
			if(iPos == m_TitleRel.GetRelCount())
				iPos = (nFocus == -1)?-1:0;
		}while(iPos != nFocus);
	}
	if(!bFound)
		AfxMessageBox(IDS_NOTFOUND_OTHERS);
}
