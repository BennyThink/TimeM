#include "StdAfx.h"
#include "resource.h"
#include "StateTitleListCtrl.h"
#include "AnySplitTTDlg.h"
#include "TitleHelper.h"

CStateTitleListCtrl::CStateTitleListCtrl(void)
{
	m_pRelVTTitle = NULL;
	m_nDirect = 0;
}

CStateTitleListCtrl::~CStateTitleListCtrl(void)
{
}

void CStateTitleListCtrl::AttachVTTitle(CTitleRelation* pRelVTTitle, int nDirect)
{
	m_pRelVTTitle = pRelVTTitle;
	m_nDirect = nDirect;
}

COLORREF CStateTitleListCtrl::OnGetCellBkColor(int nRow, int nCol)
{
	if(nCol == 0)
	{
		return GetSysColor(COLOR_SCROLLBAR);
	}

	return CTitleListCtrl::OnGetCellBkColor(nRow, nCol);
}

COLORREF CStateTitleListCtrl::OnGetCellTextColor(int nRow, int nCol)
{
	if((nRow < 0)||(nRow > ((int)m_pRelVTTitle->GetRelCount() - 1)))
		return CTitleListCtrl::OnGetCellTextColor(nRow, nCol);

	if(m_nDirect == 0)
	{
		int nLeftID = m_pRelVTTitle->GetRelItem(nRow).nLeftID;
		VT_TITLE& vtTitle = m_pRelVTTitle->GetLeftVT();
		if(((int)vtTitle.size() > nLeftID)
			&&(nLeftID > -1)
			&&((vtTitle[nLeftID]->dwFlags&FLAG_STATEMASK) == FLAG_TITLELOCK))
			return GetSysColor(COLOR_GRAYTEXT);
	}
	else
	{
		int nRightID = m_pRelVTTitle->GetRelItem(nRow).nRightID;
		VT_TITLE& vtTitle = m_pRelVTTitle->GetRightVT();
		if(((int)vtTitle.size() > nRightID)
			&&(nRightID > -1)
			&&((vtTitle[nRightID]->dwFlags&FLAG_STATEMASK) == FLAG_TITLELOCK))
			return GetSysColor(COLOR_GRAYTEXT);
	}

	return CTitleListCtrl::OnGetCellTextColor(nRow, nCol);
}

BEGIN_MESSAGE_MAP(CStateTitleListCtrl, CTitleListCtrl)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_TITLE_DELETE, &CStateTitleListCtrl::OnTitleDelete)
	ON_COMMAND(ID_TITLE_BREAKBYUSER, &CStateTitleListCtrl::OnTitleBreakbyuser)
	ON_COMMAND(ID_TITLE_CBONELINE, &CStateTitleListCtrl::OnTitleCboneline)
	ON_COMMAND(ID_TITLE_CBMULTILINE, &CStateTitleListCtrl::OnTitleCbmultiline)
END_MESSAGE_MAP()

void CStateTitleListCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_CBLISTCONTEXT);
	CMenu* pSubContext = menu.GetSubMenu(0);
	if(pSubContext != NULL)
	{
		if(GetSelectedCount() < 1)
		{
			pSubContext->EnableMenuItem(ID_TITLE_DELETE, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_BREAKBYUSER, MF_BYCOMMAND|MF_GRAYED);
		}
		if(GetSelectedCount() < 2)
		{
			pSubContext->EnableMenuItem(ID_TITLE_CBONELINE, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_CBMULTILINE, MF_BYCOMMAND|MF_GRAYED);
		}

		pSubContext->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

void CStateTitleListCtrl::NoSelect()
{
	POSITION posSel = GetFirstSelectedItemPosition();
	while(posSel)
	{
		int nSel = GetNextSelectedItem(posSel);
		SetItemState(nSel, 0, LVIS_SELECTED);
	}
}

void CStateTitleListCtrl::OnTitleDelete()
{
	CActionHelper& actor = 
		(m_nDirect == 1)
		?m_pRelVTTitle->GetRightActor()
		:m_pRelVTTitle->GetLeftActor();

	DWORD dwSelCnt = GetSelectedCount();
	if(dwSelCnt > 0)
	{
		int nPosSave = GetFocusItem();
		int nSel;
		LISTSEL_MAP mapDel;
		POSITION posSel = GetFirstSelectedItemPosition();
		while(posSel != NULL)
		{
			nSel = GetNextSelectedItem(posSel);
			if(m_nDirect == 1)// 0 = left, 1 = right
				nSel = m_pRelVTTitle->GetRelItem(nSel).nRightID;
			else
				nSel = m_pRelVTTitle->GetRelItem(nSel).nLeftID;
			if(nSel != -1)
				mapDel.insert(INT_PAIR(nSel, nSel));
		}

		LISTSEL_MAP::reverse_iterator itrRDel;

		actor.BeginRecordAction();
		for(itrRDel =  mapDel.rbegin(); itrRDel != mapDel.rend(); itrRDel++)
		{
			actor.Erase(itrRDel->first);
		}
		actor.EndRecordAction();

		if(nPosSave > m_pRelVTTitle->GetRelCount() - 1)
		{
			SetFocusItem(m_pRelVTTitle->GetRelCount() - 1);
		}

		NoSelect();
		CWnd* pParent = GetParent();
		if(NULL != pParent)
			pParent->SendMessage(MSG_REBUILDRELATION, 0, m_nDirect);
	}
}

void CStateTitleListCtrl::OnTitleBreakbyuser()
{
	CActionHelper& actor = 
		(m_nDirect == 1)
		?m_pRelVTTitle->GetRightActor()
		:m_pRelVTTitle->GetLeftActor();

	int nFocus = GetFocusItem();
	if(m_nDirect == 1)// 0 = left, 1 = right
		nFocus = m_pRelVTTitle->GetRelItem(nFocus).nRightID;
	else
		nFocus = m_pRelVTTitle->GetRelItem(nFocus).nLeftID;
	if(nFocus == -1)
		return;

	PTITLE_UNIT pUnit = actor.GetItem(nFocus, FALSE);
	CAnySplitTTDlg dlgAnySplit;
	dlgAnySplit.InitTitleUnit(pUnit);
	if(IDOK == dlgAnySplit.DoModal())
	{
		const VT_TITLE& vtTitles = dlgAnySplit.GetBreakTitles();
		int nBreakCnt = vtTitles.size();
		actor.BeginRecordAction();
		actor.Erase(nFocus);
		for(int iBreak = 0; iBreak < nBreakCnt; iBreak ++)
		{
			actor.Insert(nFocus + iBreak, vtTitles[iBreak]);
		}
		actor.EndRecordAction();

		// SendMessage to parent
		CWnd* pParent = GetParent();
		if(NULL != pParent)
			pParent->SendMessage(MSG_REBUILDRELATION, 1, m_nDirect);
	}
	return;
}

void CStateTitleListCtrl::OnTitleCboneline()
{
	DWORD dwSelCnt = GetSelectedCount();
	if(dwSelCnt < 2)
		return;

	CActionHelper& actor = 
		(m_nDirect == 1)
		?m_pRelVTTitle->GetRightActor()
		:m_pRelVTTitle->GetLeftActor();

	int iItem = -1;
	int	iSaveItem = -1;
	POSITION posSel = GetFirstSelectedItemPosition();
	PTITLE_UNIT pUnitStart = NULL;
	PTITLE_UNIT pUnitNext = NULL;
	LISTSEL_MAP mapDel;
	CString strUpLine;
	CString strDnLine;

	actor.BeginRecordAction();
	while(posSel)
	{
		iItem = GetNextSelectedItem(posSel);
		if(pUnitStart == NULL)
			iSaveItem = iItem;
		if(m_nDirect == 1)// 0 = left, 1 = right
			iItem = m_pRelVTTitle->GetRelItem(iItem).nRightID;
		else
			iItem = m_pRelVTTitle->GetRelItem(iItem).nLeftID;

		if(iItem == -1)
			continue;

		if(pUnitStart == NULL)
		{
			pUnitStart = actor.GetItem(iItem);
			CTitleHelper::BreakString(pUnitStart->content, strUpLine, strDnLine);
		}
		else
		{
			pUnitNext = actor.GetItem(iItem, FALSE);

			if(pUnitStart->nStart > pUnitNext->nStart)
				pUnitStart->nStart = pUnitNext->nStart;
			if(pUnitStart->nEnd < pUnitNext->nEnd)
				pUnitStart->nEnd = pUnitNext->nEnd;

			CString strFirst, strOthers;

			CTitleHelper::RemoveAssCode(pUnitNext->content);
			CTitleHelper::BreakString(pUnitNext->content, strFirst, strOthers);
			if(!strFirst.IsEmpty())
			{
				strUpLine += _T(" ");
				strUpLine += strFirst;
			}
			if(!strOthers.IsEmpty())
			{
				strDnLine += _T(" ");
				strDnLine += strOthers;
			}
			mapDel.insert(INT_PAIR(iItem, iItem));
		}
	}

	pUnitStart->content = strUpLine;
	pUnitStart->content += _T("\r\n");
	pUnitStart->content += strDnLine;

	LISTSEL_MAP::reverse_iterator itrRDel;
	for(itrRDel =  mapDel.rbegin(); itrRDel != mapDel.rend(); itrRDel++)
	{
		actor.Erase(itrRDel->first);
	}

	actor.EndRecordAction();

	NoSelect();
	SetFocusItem(iSaveItem);
	// sendmessage to parent
	CWnd* pParent = GetParent();
	if(NULL != pParent)
		pParent->SendMessage(MSG_REBUILDRELATION, 2, m_nDirect);
}

void CStateTitleListCtrl::OnTitleCbmultiline()
{
	DWORD dwSelCnt = GetSelectedCount();
	if(dwSelCnt < 2)
		return;

	CActionHelper& actor = 
		(m_nDirect == 1)
		?m_pRelVTTitle->GetRightActor()
		:m_pRelVTTitle->GetLeftActor();

	int iItem = -1;
	int iSaveItem = -1;
	POSITION posSel = GetFirstSelectedItemPosition();
	PTITLE_UNIT pUnitStart = NULL;
	PTITLE_UNIT pUnitNext = NULL;
	LISTSEL_MAP mapDel;
	actor.BeginRecordAction();
	while(posSel)
	{
		iItem = GetNextSelectedItem(posSel);
		if(pUnitStart == NULL)
			iSaveItem = iItem;
		if(m_nDirect == 1)// 0 = left, 1 = right
			iItem = m_pRelVTTitle->GetRelItem(iItem).nRightID;
		else
			iItem = m_pRelVTTitle->GetRelItem(iItem).nLeftID;

		if(iItem == -1)
			continue;

		if(pUnitStart == NULL)
		{
			pUnitStart = actor.GetItem(iItem);
		}
		else
		{
			pUnitNext = actor.GetItem(iItem, FALSE);

			if(pUnitStart->nStart > pUnitNext->nStart)
				pUnitStart->nStart = pUnitNext->nStart;
			if(pUnitStart->nEnd < pUnitNext->nEnd)
				pUnitStart->nEnd = pUnitNext->nEnd;

			CTitleHelper::RemoveAssCode(pUnitNext->content);
			pUnitStart->content += _T("\r\n");
			pUnitStart->content += pUnitNext->content;
			mapDel.insert(INT_PAIR(iItem, iItem));
		}
	}

	LISTSEL_MAP::reverse_iterator itrRDel;
	for(itrRDel =  mapDel.rbegin(); itrRDel != mapDel.rend(); itrRDel++)
	{
		actor.Erase(itrRDel->first);
	}
	actor.EndRecordAction();

	NoSelect();
	SetFocusItem(iSaveItem);
	// sendmessage to parent
	CWnd* pParent = GetParent();
	if(NULL != pParent)
		pParent->SendMessage(MSG_REBUILDRELATION, 3, m_nDirect);
}
