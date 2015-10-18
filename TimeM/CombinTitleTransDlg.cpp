// CombinTitleTransDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "CombinTitleTransDlg.h"
#include "TitleFile.h"
#include "TitleHelper.h"
#include "CombineFromToDlg.h"

#define	MSG_SYNCLISTSCROLL	(WM_USER+101)

// CCombinTitleTransDlg dialog

IMPLEMENT_DYNAMIC(CCombinTitleTransDlg, CDialog)

CCombinTitleTransDlg::CCombinTitleTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCombinTitleTransDlg::IDD, pParent)
	, m_strStdFile(_T(""))
	, m_strTransFile(_T(""))
{
	m_bStdModified		= FALSE;
	m_bTransModified	= FALSE;
	m_bLockScroll		= FALSE;
}

CCombinTitleTransDlg::~CCombinTitleTransDlg()
{
}

void CCombinTitleTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILESTD, m_strStdFile);
	DDX_Text(pDX, IDC_EDIT_FILETRANS, m_strTransFile);
	DDX_Control(pDX, IDC_LIST_STDTITLE, m_ListTitleStd);
	DDX_Control(pDX, IDC_LIST_TRANSTITLE, m_ListTitleTrans);
	DDX_Control(pDX, IDOK, m_mbtnCombine);
}


BEGIN_MESSAGE_MAP(CCombinTitleTransDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSTD, &CCombinTitleTransDlg::OnBnClickedBtnBrowstd)
	ON_BN_CLICKED(IDC_BTN_BROWTRANS, &CCombinTitleTransDlg::OnBnClickedBtnBrowtrans)
	ON_MESSAGE(MSG_SYNCLISTSCROLL, &CCombinTitleTransDlg::OnSyncListScroll)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_STDTITLE, &CCombinTitleTransDlg::OnLvnGetdispinfoListStdtitle)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TRANSTITLE, &CCombinTitleTransDlg::OnLvnGetdispinfoListTranstitle)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STDTITLE, &CCombinTitleTransDlg::OnLvnItemchangedListStdtitle)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TRANSTITLE, &CCombinTitleTransDlg::OnLvnItemchangedListTranstitle)
	ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST_STDTITLE, &CCombinTitleTransDlg::OnLvnEndScrollListStdtitle)
	ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST_TRANSTITLE, &CCombinTitleTransDlg::OnLvnEndScrollListTranstitle)
	ON_BN_CLICKED(IDC_BTN_SNAPSEL, &CCombinTitleTransDlg::OnBnClickedBtnSnapsel)
	ON_BN_CLICKED(IDOK, &CCombinTitleTransDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDSAVE, &CCombinTitleTransDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_CHK_SYNCSCROLL, &CCombinTitleTransDlg::OnBnClickedChkSyncscroll)
	ON_BN_CLICKED(ID_BTN_UNDO, &CCombinTitleTransDlg::OnBnClickedBtnUndo)
	ON_BN_CLICKED(ID_BTN_REDO, &CCombinTitleTransDlg::OnBnClickedBtnRedo)
	ON_BN_CLICKED(IDC_BTN_EDITSTD, &CCombinTitleTransDlg::OnBnClickedBtnEditstd)
//	ON_BN_CLICKED(IDC_BTN_RELOADSTD, &CCombinTitleTransDlg::OnBnClickedBtnReloadstd)
	ON_BN_CLICKED(IDC_BTN_EDITTRANS, &CCombinTitleTransDlg::OnBnClickedBtnEdittrans)
//	ON_BN_CLICKED(IDC_BTN_RELOADTRANS, &CCombinTitleTransDlg::OnBnClickedBtnReloadtrans)
	ON_MESSAGE(MSG_REBUILDRELATION, &CCombinTitleTransDlg::OnNotifyListRebuild)
	ON_BN_CLICKED(ID_BTN_UNDO2, &CCombinTitleTransDlg::OnBnClickedBtnUndo2)
	ON_BN_CLICKED(ID_BTN_REDO2, &CCombinTitleTransDlg::OnBnClickedBtnRedo2)
	ON_BN_CLICKED(IDSAVE2, &CCombinTitleTransDlg::OnBnClickedSave2)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CCombinTitleTransDlg::DoFilePrompt(CString& strFile)
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

// CCombinTitleTransDlg message handlers

void CCombinTitleTransDlg::OnBnClickedBtnBrowstd()
{
	if((!m_strStdFile.IsEmpty())
		&&(IsStdModified()))
	{
		switch(AfxMessageBox(IDS_SAVESTDTIPS, MB_YESNOCANCEL))
		{
		case IDYES:
			OnBnClickedSave();
		case IDNO:
			break;
		case IDCANCEL:
			return;
		}
	}
	CFile fileOpen;
	if((DoFilePrompt(m_strStdFile))
		&&(fileOpen.Open(m_strStdFile, CFile::modeRead)))
	{
		AfxGetApp()->BeginWaitCursor();
		CTitleFile::DeleteContents(m_TitleRel.GetLeftVT());
		CTitleFile::ReadTitleFile(&fileOpen, m_TitleRel.GetLeftVT(), m_strStdFmtHdr, m_bStdUnicode);
		SaveStdModified();
		
		m_TitleRel.GetLeftActor().ClearAllAction();

		RebuildRelationAll();
		AfxGetApp()->EndWaitCursor();
	}
	UpdateData(FALSE);
}

void CCombinTitleTransDlg::OnBnClickedBtnBrowtrans()
{
	if((!m_strTransFile.IsEmpty())
		&&(IsTransModified()))
	{
		switch(AfxMessageBox(IDS_SAVETRANSTIPS, MB_YESNOCANCEL))
		{
		case IDYES:
			OnBnClickedSave2();
		case IDNO:
			break;
		case IDCANCEL:
			return;
		}
	}
	
	CFile fileOpen;
	if((DoFilePrompt(m_strTransFile))
		&&(fileOpen.Open(m_strTransFile, CFile::modeRead)))
	{
		AfxGetApp()->BeginWaitCursor();
		CTitleFile::DeleteContents(m_TitleRel.GetRightVT());

		CTitleFile::ReadTitleFile(&fileOpen, m_TitleRel.GetRightVT(), m_strTransFmtHdr, m_bTransUnicode);
		SaveTransModified();

		m_TitleRel.GetRightActor().ClearAllAction();

		RebuildRelationAll();
		AfxGetApp()->EndWaitCursor();
	}
	UpdateData(FALSE);
}

void CCombinTitleTransDlg::SyncListScroll(const CListCtrl& listSrc, CListCtrl& listTag)
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

LRESULT CCombinTitleTransDlg::OnSyncListScroll(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 0)
		SyncListScroll(m_ListTitleStd, m_ListTitleTrans);
	else
		SyncListScroll(m_ListTitleTrans, m_ListTitleStd);
	return 0;
}

void CCombinTitleTransDlg::GetDispInfoTitleItem(NMLVDISPINFO *pDispInfo, PTITLE_UNIT pUnit, int nNo)
{
	if(pDispInfo->item.mask&LVIF_TEXT)
	{
		if(pUnit != NULL)
		{
			switch(pDispInfo->item.iSubItem)
			{
			case 0:
				StringCbPrintf(pDispInfo->item.pszText, MAX_PATH, _T("%d"), nNo + 1);
				break;
			case 1:
				StringCbCopy(pDispInfo->item.pszText, MAX_PATH,
					CTitleHelper::FormatTime(pUnit->nStart));
				break;
			case 2:
				StringCbCopy(pDispInfo->item.pszText, MAX_PATH,
					CTitleHelper::FormatTime(pUnit->nEnd));
				break;
			case 3:
				StringCbCopy(pDispInfo->item.pszText, MAX_PATH, pUnit->content);
				break;
			}
		}
	}
}

void CCombinTitleTransDlg::OnLvnGetdispinfoListStdtitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	PAIR_REL relPair = m_TitleRel.GetRelItem(pDispInfo->item.iItem);
	PTITLE_UNIT pUnit = CTitleHelper::GetTitleItem(relPair.nLeftID, m_TitleRel.GetLeftVT());
	GetDispInfoTitleItem(pDispInfo, pUnit, relPair.nLeftID);
	*pResult = 0;
}

void CCombinTitleTransDlg::OnLvnGetdispinfoListTranstitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	PAIR_REL relPair = m_TitleRel.GetRelItem(pDispInfo->item.iItem);
	PTITLE_UNIT pUnit = CTitleHelper::GetTitleItem(relPair.nRightID, m_TitleRel.GetRightVT());
	GetDispInfoTitleItem(pDispInfo, pUnit, relPair.nRightID);
	*pResult = 0;
}

void CCombinTitleTransDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: Add your message handler code here and/or call default
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

void CCombinTitleTransDlg::OnLvnItemchangedListStdtitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL);
	*pResult = 0;
}

void CCombinTitleTransDlg::OnLvnItemchangedListTranstitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL, 1);
	*pResult = 0;
}

void CCombinTitleTransDlg::OnLvnEndScrollListStdtitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Internet Explorer 5.5 or greater.
	// The symbol _WIN32_IE must be >= 0x0560.
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL);
	*pResult = 0;
}

void CCombinTitleTransDlg::OnLvnEndScrollListTranstitle(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Internet Explorer 5.5 or greater.
	// The symbol _WIN32_IE must be >= 0x0560.
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);

	PostMessage(MSG_SYNCLISTSCROLL, 1);
	*pResult = 0;
}

BOOL CCombinTitleTransDlg::OnInitDialog()
{
	m_mbtnCombine.m_bDefaultClick = TRUE;
	m_mbtnCombine.m_bStayPressed = TRUE;

	CDialog::OnInitDialog();

	m_menuCombine.LoadMenu(IDR_MENU_COMBINE_EXT);
	m_mbtnCombine.m_hMenu = m_menuCombine.GetSubMenu(0)->GetSafeHmenu();
	m_mbtnCombine.SizeToContent();
	m_mbtnCombine.m_bOSMenu = FALSE;

	CString strCol;

	strCol.LoadString(IDS_COL_TITLE_NO);
	m_ListTitleStd.InsertColumn(0, strCol, 0, 35);
	m_ListTitleTrans.InsertColumn(0, strCol, 0, 35);

	strCol.LoadString(IDS_COL_TITLE_START);
	m_ListTitleStd.InsertColumn(1, strCol, 0, 85);
	m_ListTitleTrans.InsertColumn(1, strCol, 0, 85);

	strCol.LoadString(IDS_COL_TITLE_END);
	m_ListTitleStd.InsertColumn(2, strCol, 0, 85);
	m_ListTitleTrans.InsertColumn(2, strCol, 0, 85);

	strCol.LoadString(IDS_COL_TITLE_CONTENT);
	m_ListTitleStd.InsertColumn(3, strCol, 0, 400);
	m_ListTitleTrans.InsertColumn(3, strCol, 0, 400);

	m_ListTitleStd.AttachVTTitle(&m_TitleRel, 0);
	m_ListTitleTrans.AttachVTTitle(&m_TitleRel, 1);

	CheckDlgButton(IDC_CHK_SYNCSCROLL, BST_CHECKED);

	UpdateCtrlState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCombinTitleTransDlg::IsSelectedMatch()
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

void CCombinTitleTransDlg::OnBnClickedBtnSnapsel()
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

void CCombinTitleTransDlg::CombineSelected()
{
	PTITLE_UNIT pLeftUnit, pRightUnit;
	m_TitleRel.GetLeftActor().BeginRecordAction();
	m_TitleRel.GetRightActor().BeginRecordAction();
	int nCnt = m_TitleRel.GetRelCount();
	POSITION posSel = m_ListTitleTrans.GetFirstSelectedItemPosition();
	while(posSel != NULL)
	{
		int iItem = m_ListTitleTrans.GetNextSelectedItem(posSel);
		if(m_TitleRel.GetRelItem(iItem).nLeftID != -1)
		{
			int nLeft = m_TitleRel.GetRelItem(iItem).nLeftID;
			int nRight = m_TitleRel.GetRelItem(iItem).nRightID;
			if((nLeft != -1)
				&&(nRight != -1))
			{
				pLeftUnit = m_TitleRel.GetLeftActor().GetItem(nLeft);
				pRightUnit = m_TitleRel.GetRightActor().GetItem(nRight);
				if((pLeftUnit != NULL) && (pRightUnit != NULL) && ((pLeftUnit->dwFlags&FLAG_STATEMASK) != FLAG_TITLELOCK))
				{
					pLeftUnit->content += _T("\r\n");
					pLeftUnit->content += pRightUnit->content;
					pLeftUnit->dwFlags &= FLAG_STATECLEAR;
					pLeftUnit->dwFlags |= FLAG_TITLELOCK;
					pRightUnit->dwFlags &= FLAG_STATECLEAR;
					pRightUnit->dwFlags |= FLAG_TITLELOCK;
				}
			}
		}
	}
	m_TitleRel.GetLeftActor().EndRecordAction();
	m_TitleRel.GetRightActor().EndRecordAction();
	UpdateCtrlState();
	m_ListTitleStd.Invalidate(FALSE);
	m_ListTitleTrans.Invalidate(FALSE);

	SetStdModifiedFlag();
	SetTransModifiedFlag();
}

void CCombinTitleTransDlg::CombineAll()
{
	PTITLE_UNIT pLeftUnit, pRightUnit;
	m_TitleRel.GetLeftActor().BeginRecordAction();
	m_TitleRel.GetRightActor().BeginRecordAction();
	int nCnt = m_TitleRel.GetRelCount();
	for(int iRel = 0; iRel < nCnt; iRel ++)
	{
		if(m_TitleRel.GetRelItem(iRel).nLeftID != -1)
		{
			int nLeft = m_TitleRel.GetRelItem(iRel).nLeftID;
			int nRight = m_TitleRel.GetRelItem(iRel).nRightID;
			if((nLeft != -1)
				&&(nRight != -1))
			{
				pLeftUnit = m_TitleRel.GetLeftActor().GetItem(nLeft);
				pRightUnit = m_TitleRel.GetRightActor().GetItem(nRight);
				if((pLeftUnit != NULL) && (pRightUnit != NULL) && ((pLeftUnit->dwFlags&FLAG_STATEMASK) != FLAG_TITLELOCK))
				{
					pLeftUnit->content += _T("\r\n");
					pLeftUnit->content += pRightUnit->content;
					pLeftUnit->dwFlags &= FLAG_STATECLEAR;
					pLeftUnit->dwFlags |= FLAG_TITLELOCK;
					pRightUnit->dwFlags &= FLAG_STATECLEAR;
					pRightUnit->dwFlags |= FLAG_TITLELOCK;
				}
			}
		}
	}
	m_TitleRel.GetLeftActor().EndRecordAction();
	m_TitleRel.GetRightActor().EndRecordAction();
	UpdateCtrlState();
	m_ListTitleStd.Invalidate(FALSE);
	m_ListTitleTrans.Invalidate(FALSE);

	SetStdModifiedFlag();
	SetTransModifiedFlag();
}

void CCombinTitleTransDlg::CombineFromTo()
{
	CCombineFromToDlg dlgFromTo;
	dlgFromTo.m_nLineFrom = 1;
	dlgFromTo.m_nLineTo = m_TitleRel.GetRightActor().GetItemCount();
	if(IDCANCEL == dlgFromTo.DoModal())
		return;
	PTITLE_UNIT pLeftUnit, pRightUnit;
	m_TitleRel.GetLeftActor().BeginRecordAction();
	m_TitleRel.GetRightActor().BeginRecordAction();
	int nCnt = m_TitleRel.GetRelCount();
	for(int iRel = 0; iRel < nCnt; iRel ++)
	{
		if((m_TitleRel.GetRelItem(iRel).nLeftID < dlgFromTo.m_nLineFrom)
			||(m_TitleRel.GetRelItem(iRel).nLeftID > dlgFromTo.m_nLineTo))
			continue;

		if(m_TitleRel.GetRelItem(iRel).nLeftID != -1)
		{
			int nLeft = m_TitleRel.GetRelItem(iRel).nLeftID;
			int nRight = m_TitleRel.GetRelItem(iRel).nRightID;
			if((nLeft != -1)
				&&(nRight != -1))
			{
				pLeftUnit = m_TitleRel.GetLeftActor().GetItem(nLeft);
				pRightUnit = m_TitleRel.GetRightActor().GetItem(nRight);
				if((pLeftUnit != NULL) && (pRightUnit != NULL) && ((pLeftUnit->dwFlags&FLAG_STATEMASK) != FLAG_TITLELOCK))
				{
					pLeftUnit->content += _T("\r\n");
					pLeftUnit->content += pRightUnit->content;
					pLeftUnit->dwFlags &= FLAG_STATECLEAR;
					pLeftUnit->dwFlags |= FLAG_TITLELOCK;
					pRightUnit->dwFlags &= FLAG_STATECLEAR;
					pRightUnit->dwFlags |= FLAG_TITLELOCK;
				}
			}
		}
	}
	m_TitleRel.GetLeftActor().EndRecordAction();
	m_TitleRel.GetRightActor().EndRecordAction();
	UpdateCtrlState();
	m_ListTitleStd.Invalidate(FALSE);
	m_ListTitleTrans.Invalidate(FALSE);

	SetStdModifiedFlag();
	SetTransModifiedFlag();
}

void CCombinTitleTransDlg::OnBnClickedOk()
{
	switch (m_mbtnCombine.m_nMenuResult)
	{
	case ID_CB_ALL:
		CombineAll();
		break;
	case ID_CB_FROMTO:
		CombineFromTo();
		break;
	default:
		CombineSelected();
		break;
	}

}

void CCombinTitleTransDlg::OnBnClickedSave()
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
		CTitleFile::WriteTitleFile(&fileOpen, m_TitleRel.GetLeftVT(), m_strStdFmtHdr, CString(_T("")), CString(_T("")), dwFmt, m_bStdUnicode);
		SaveStdModified();
	}
}

void CCombinTitleTransDlg::UpdateCtrlState()
{
	CWnd* pBtnRedo = GetDlgItem(ID_BTN_REDO);
	if(pBtnRedo != NULL)
	{
		pBtnRedo->EnableWindow(m_TitleRel.GetLeftActor().IsCanRedo());
	}

	CWnd* pBtnUndo = GetDlgItem(ID_BTN_UNDO);
	if(pBtnUndo != NULL)
	{
		pBtnUndo->EnableWindow(m_TitleRel.GetLeftActor().IsCanUndo());
	}

	CWnd* pBtnRedo2 = GetDlgItem(ID_BTN_REDO2);
	if(pBtnRedo2 != NULL)
	{
		pBtnRedo2->EnableWindow(m_TitleRel.GetRightActor().IsCanRedo());
	}

	CWnd* pBtnUndo2 = GetDlgItem(ID_BTN_UNDO2);
	if(pBtnUndo2 != NULL)
	{
		pBtnUndo2->EnableWindow(m_TitleRel.GetRightActor().IsCanUndo());
	}
}

void CCombinTitleTransDlg::OnBnClickedChkSyncscroll()
{
	PostMessage(MSG_SYNCLISTSCROLL);
}

void CCombinTitleTransDlg::OnBnClickedBtnUndo()
{
	m_TitleRel.GetLeftActor().Undo();
	SetStdModifiedFlag();
	RebuildRelationAll();
}

void CCombinTitleTransDlg::OnBnClickedBtnRedo()
{
	m_TitleRel.GetLeftActor().Redo();
	SetStdModifiedFlag();
	RebuildRelationAll();
}

void CCombinTitleTransDlg::OpenExternal(LPCTSTR lpszFile)
{
	if(!PathFileExists(lpszFile))
		return;
	TCHAR	szModule[MAX_PATH];
	GetModuleFileName(NULL, szModule, MAX_PATH);
	CString strCmdLine;
	strCmdLine.Format(_T("\"%s\" \"%s\""), szModule, lpszFile);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	AfxGetApp()->BeginWaitCursor();
	if(CreateProcess(NULL, strCmdLine.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	AfxGetApp()->EndWaitCursor();
	strCmdLine.ReleaseBuffer();
}

void CCombinTitleTransDlg::OnBnClickedBtnEditstd()
{
	if(IsStdModified())
	{
		switch(AfxMessageBox(IDS_SAVESTDTIPS, MB_YESNOCANCEL))
		{
		case IDYES:
			OnBnClickedSave();
		case IDNO:
			break;
		case IDCANCEL:
			return;
		}
	}
	OpenExternal(m_strStdFile);
	OnBnClickedBtnReloadstd();
}

void CCombinTitleTransDlg::OnBnClickedBtnReloadstd()
{
	CFile fileOpen;
	if(fileOpen.Open(m_strStdFile, CFile::modeRead))
	{
		AfxGetApp()->BeginWaitCursor();
		CTitleFile::DeleteContents(m_TitleRel.GetLeftVT());
		CTitleFile::ReadTitleFile(&fileOpen, m_TitleRel.GetLeftVT(), m_strStdFmtHdr, m_bStdUnicode);
		SaveStdModified();
		m_TitleRel.GetLeftActor().ClearAllAction();

		RebuildRelationAll();
		AfxGetApp()->EndWaitCursor();
	}
	UpdateData(FALSE);
}

void CCombinTitleTransDlg::OnBnClickedBtnEdittrans()
{
	if(IsTransModified())
	{
		switch(AfxMessageBox(IDS_SAVETRANSTIPS, MB_YESNOCANCEL))
		{
		case IDYES:
			OnBnClickedSave2();
		case IDNO:
			break;
		case IDCANCEL:
			return;
		}
	}
	OpenExternal(m_strTransFile);
	OnBnClickedBtnReloadtrans();
}

void CCombinTitleTransDlg::OnBnClickedBtnReloadtrans()
{
	CFile fileOpen;
	if(fileOpen.Open(m_strTransFile, CFile::modeRead))
	{
		AfxGetApp()->BeginWaitCursor();
		CTitleFile::DeleteContents(m_TitleRel.GetRightVT());
		CTitleFile::ReadTitleFile(&fileOpen, m_TitleRel.GetRightVT(), m_strTransFmtHdr, m_bTransUnicode);
		SaveTransModified();
		m_TitleRel.GetRightActor().ClearAllAction();
		RebuildRelationAll();
		AfxGetApp()->EndWaitCursor();
	}
	UpdateData(FALSE);
}

void CCombinTitleTransDlg::RebuildRelationAll()
{
	m_TitleRel.BuildRelation(BUILD_TYPE_BYNORMAL);
	m_ListTitleStd.SetItemCount(m_TitleRel.GetRelCount());
	m_ListTitleStd.Invalidate(FALSE);
	m_ListTitleTrans.SetItemCount(m_TitleRel.GetRelCount());
	m_ListTitleTrans.Invalidate(FALSE);
	UpdateCtrlState();
}

LRESULT CCombinTitleTransDlg::OnNotifyListRebuild(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case 0:
		SetStdModifiedFlag();
		break;
	case 1:
		SetTransModifiedFlag();
		break;
	}
	RebuildRelationAll();
	return 0;
}

void CCombinTitleTransDlg::OnBnClickedBtnUndo2()
{
	// TODO: Add your control notification handler code here
	m_TitleRel.GetRightActor().Undo();
	SetTransModifiedFlag();
	RebuildRelationAll();
}

void CCombinTitleTransDlg::OnBnClickedBtnRedo2()
{
	// TODO: Add your control notification handler code here
	m_TitleRel.GetRightActor().Redo();
	SetTransModifiedFlag();
	RebuildRelationAll();
}

void CCombinTitleTransDlg::OnBnClickedSave2()
{
	CFile fileOpen;
	if(fileOpen.Open(m_strTransFile, CFile::modeWrite))
	{
		CString strExt = PathFindExtension(m_strTransFile);
		DWORD dwFmt = FMTFLAG_SRT;
		if(strExt.CompareNoCase(_T(".ssa")) == 0)
		{
			dwFmt = FMTFLAG_SSA;
		}
		else if(strExt.CompareNoCase(_T(".ass")) == 0)
		{
			dwFmt = FMTFLAG_ASS;
		}
		CTitleFile::WriteTitleFile(&fileOpen, m_TitleRel.GetRightVT(), m_strTransFmtHdr, CString(_T("")), CString(_T("")), dwFmt, m_bTransUnicode);
		SaveTransModified();
	}
}

void CCombinTitleTransDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetFocus();
	CDialog::OnLButtonDown(nFlags, point);
}
