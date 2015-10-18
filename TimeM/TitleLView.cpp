// TitleLView.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "TitleLView.h"
#include "TitleFile.h"
#include "TitleHelper.h"

// CTitleLView

IMPLEMENT_DYNCREATE(CTitleLView, CMLListView)

CTitleLView::CTitleLView()
{
	m_hFontDisp = NULL;
	m_nEditRow = -1;
	m_nEditCol = -1;
	m_bCapture = FALSE;
	m_bHideAssCode = TRUE;
}

CTitleLView::~CTitleLView()
{
	if(m_hFontDisp != NULL)
	{
		DeleteObject(m_hFontDisp);
		m_hFontDisp = NULL;
	}
}

BEGIN_MESSAGE_MAP(CTitleLView, CMLListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CTitleLView::OnLvnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_ODSTATECHANGED, &CTitleLView::OnLvnOdstatechanged)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CTitleLView::OnLvnItemchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CTitleLView::OnNMDblclk)
	ON_NOTIFY_REFLECT(LVN_ODFINDITEM, &CTitleLView::OnLvnOdfinditem)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_HIDE_ASSCODE, &CTitleLView::OnHideAsscode)
	ON_UPDATE_COMMAND_UI(ID_HIDE_ASSCODE, &CTitleLView::OnUpdateHideAsscode)
END_MESSAGE_MAP()


// CTitleLView diagnostics

#ifdef _DEBUG
void CTitleLView::AssertValid() const
{
	CMLListView::AssertValid();
}

#ifndef _WIN32_WCE
void CTitleLView::Dump(CDumpContext& dc) const
{
	CMLListView::Dump(dc);
}
#endif
#endif //_DEBUG

CTimeMDoc* CTitleLView::GetDocument() const
{
	return (CTimeMDoc*)CMLListView::GetDocument();
}

void CTitleLView::GetEdit(int& nRow, int& nCol)
{
	nRow = m_nEditRow;
	nCol = m_nEditCol;
}

void CTitleLView::SetEdit(int nRow, int nCol)
{
	m_nEditRow = nRow;
	m_nEditCol = nCol;
}

// CTitleLView message handlers

int CTitleLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMLListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);

	CString strCol;
	strCol.LoadString(IDS_COL_TITLE_NO);		GetListCtrl().InsertColumn(0, strCol, 0, 35);
	strCol.LoadString(IDS_COL_TITLE_START);		GetListCtrl().InsertColumn(1, strCol, 0, 85);
	strCol.LoadString(IDS_COL_TITLE_END);		GetListCtrl().InsertColumn(2, strCol, 0, 85);
	strCol.LoadString(IDS_COL_TITLE_CONTENT);	GetListCtrl().InsertColumn(3, strCol, 0, 400);
	strCol.LoadString(IDS_COL_TITLE_CHCOUNT);	GetListCtrl().InsertColumn(4, strCol, 0, 40);
	strCol.LoadString(IDS_COL_TITLE_TIMELEN);	GetListCtrl().InsertColumn(5, strCol, 0, 70);
	strCol.LoadString(IDS_COL_TITLE_LINENUM);	GetListCtrl().InsertColumn(6, strCol, 0, 40);

	return 0;
}

COLORREF CTitleLView::OnGetCellTextColor(int nRow, int nCol)
{
	if(nCol == 0)
	{
		return GetListCtrl().GetTextColor();
	}
	return CMLListView::OnGetCellTextColor(nRow, nCol);
}

COLORREF CTitleLView::OnGetCellBkColor(int nRow, int nCol)
{
	if(nCol == 0)
	{
		CTimeMDoc* pDoc = GetDocument();
		if((pDoc != NULL)&&(pDoc->HasMark(nRow)))
			return RGB(255, 128, 0);

		return GetSysColor(COLOR_SCROLLBAR);
	}
	if((m_nEditRow == nRow)
		&&(m_nEditCol == nCol))
		return RGB(255, 255, 0);
	return CMLListView::OnGetCellBkColor(nRow, nCol);
}

void CTitleLView::GetItemText(int nItem, int nCol, CString& strItem)
{
	CTimeMDoc* pDoc = GetDocument();
	if(pDoc != NULL)
	{
		PTITLE_UNIT pUnit = pDoc->GetTitleItem(nItem);
		CString strContent;
		if(CHINESEBIG5_CHARSET == pDoc->GetDispCharset())
			strContent = CTitleFile::LCMapStrCN(SORT_CHINESE_PRC, LCMAP_TRADITIONAL_CHINESE, pUnit->content);
		else
			strContent = pUnit->content;
		if(pUnit != NULL)
		{
			switch(nCol)
			{
			case 0:
				strItem.Format(_T("%d"), nItem + 1);
				break;
			case 1:
				strItem = CTitleHelper::FormatTime(pUnit->nStart);
				break;
			case 2:
				strItem = CTitleHelper::FormatTime(pUnit->nEnd);
				break;
			case 3:
				strItem = strContent;
				if(m_bHideAssCode)
					CTitleHelper::RemoveAssCode(strItem);
				break;
			case 4:
				strItem = strContent;
				if(m_bHideAssCode)
					CTitleHelper::RemoveAssCode(strItem);
				CTitleHelper::CountTitleLine(strItem, strItem);
				break;
			case 5:
				strItem = CTitleHelper::FormatComma(pUnit->nEnd - pUnit->nStart);
				break;
			case 6:
				strItem = strContent;
				if(m_bHideAssCode)
					CTitleHelper::RemoveAssCode(strItem);
				int nCnt = CTitleHelper::CountTitleLine(strItem, strItem);
				strItem.Format(_T("%d"), nCnt);
				break;
			}
		}
	}
}

void CTitleLView::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	CTimeMDoc* pDoc = GetDocument();
	if(pDoc == NULL)
	{
		*pResult = 0;
		return;
	}
	if(pDispInfo->item.mask&LVIF_TEXT)
	{
		PTITLE_UNIT pUnit = pDoc->GetTitleItem(pDispInfo->item.iItem);
		CString strContent;
		if(CHINESEBIG5_CHARSET == pDoc->GetDispCharset())
			strContent = CTitleFile::LCMapStrCN(SORT_CHINESE_BIG5, LCMAP_SIMPLIFIED_CHINESE, pUnit->content);
		else
			strContent = pUnit->content;

		CString strItem;
		if(pUnit != NULL)
		{
			switch(pDispInfo->item.iSubItem)
			{
			case 0:
				StringCbPrintf(pDispInfo->item.pszText, MAX_PATH, _T("%d"), pDispInfo->item.iItem + 1);
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
				StringCbCopy(pDispInfo->item.pszText, MAX_PATH, strContent);
				break;
			case 4:
				CTitleHelper::CountTitleLine(strContent, strItem);
				StringCbCopy(pDispInfo->item.pszText, MAX_PATH, strItem);
				break;
			case 5:
				strItem = CTitleHelper::FormatComma(pUnit->nEnd - pUnit->nStart);
				StringCbCopy(pDispInfo->item.pszText, MAX_PATH, strItem);
				break;
			case 6:
				StringCbPrintf(pDispInfo->item.pszText, MAX_PATH, _T("%d"), CTitleHelper::CountTitleLine(strContent, strItem));
				break;
			}
		}
	}
	*pResult = 0;
}

void CTitleLView::EnsureCenter(int nPos)
{
	int nPerPageCnt = GetListCtrl().GetCountPerPage();
	int nCurrTop = GetListCtrl().GetTopIndex();
	if(nPerPageCnt < 4)
	{
		GetListCtrl().EnsureVisible(nPos, FALSE);
		return;
	}

	CRect rcItem;
	GetListCtrl().GetItemRect(0, rcItem, LVIR_BOUNDS);
	GetListCtrl().Scroll(CSize(0, (nPos - nPerPageCnt/2 - nCurrTop)*rcItem.Height()));
}

void CTitleLView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
	CTimeMDoc* pDoc = GetDocument();
	if(pDoc)
	{
		int nSize = pDoc->GetTitleCount();
		GetListCtrl().SetItemCount(nSize);
		if(nSize > 0)
		{
			GetListCtrl().SetItemState(pDoc->GetCurrentPos(), LVIS_FOCUSED, LVIS_FOCUSED);
			if(pDoc->m_bUseSync||pDoc->m_bAutoSnap)
			{
				EnsureCenter(pDoc->GetCurrentPos());
			}
			else
			{
				GetListCtrl().EnsureVisible(pDoc->GetCurrentPos(), FALSE);
			}
		}
	}
}

BOOL CTitleLView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	dwStyle |= LVS_REPORT|LVS_OWNERDATA|LVS_SORTASCENDING|LVS_SORTDESCENDING;
	return CMLListView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CTitleLView::OnLvnOdstatechanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVODSTATECHANGE pStateChanged = reinterpret_cast<LPNMLVODSTATECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTitleLView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	CTimeMDoc* pDoc = GetDocument();
	if(((pNMLV->uNewState&LVIS_FOCUSED) == 0)
		&&((pNMLV->uOldState&LVIS_FOCUSED) == LVIS_FOCUSED))
	{
		pDoc->SetCurrentPos(-1);
		pDoc->UpdateRefWin(this);
	}
	else if(((pNMLV->uNewState&LVIS_FOCUSED) == LVIS_FOCUSED)
		&&((pNMLV->uOldState&LVIS_FOCUSED) == 0))
	{
		pDoc->SetCurrentPos(pNMLV->iItem);
		pDoc->UpdateRefWin(this);
	}

	*pResult = 0;
}

void CTitleLView::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 32;
}

void CTitleLView::NoSelect()
{
	// TODO: Add your command handler code here
	CTimeMDoc* pDoc = GetDocument();
	if(pDoc == NULL)
		return;

	POSITION posSel = GetListCtrl().GetFirstSelectedItemPosition();
	while(posSel)
	{
		int nSel = GetListCtrl().GetNextSelectedItem(posSel);
		GetListCtrl().SetItemState(nSel, 0, LVIS_SELECTED);
	}

}

UINT CTitleLView::GetSelectedCount()
{
	return GetListCtrl().GetSelectedCount();
}

POSITION CTitleLView::GetFirstSelectedItemPosition()
{
	return GetListCtrl().GetFirstSelectedItemPosition();
}

int CTitleLView::GetNextSelectedItem(POSITION& pos)
{
	return GetListCtrl().GetNextSelectedItem(pos);
}

void CTitleLView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CTimeMDoc* pDoc = GetDocument();
	if(pDoc == NULL)
		return;

	if(pNMItemActivate->iItem > -1)
	{
		switch(pNMItemActivate->iSubItem)
		{
		case 0:
			pDoc->MarkItem(pNMItemActivate->iItem);
			break;
		case 1:
			pDoc->SetPlayPos(pNMItemActivate->iItem, TRUE);
			break;
		case 2:
			pDoc->SetPlayPos(pNMItemActivate->iItem, FALSE);
			break;
		case 3:
		case 4:
			pDoc->ToEditTitle(pNMItemActivate->iItem);
			break;
		}
	}
	*pResult = 0;
}

void CTitleLView::SelectItem(int nItem)
{
	GetListCtrl().SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
}

void CTitleLView::OnEditSelectAll()
{
	int nCnt = GetListCtrl().GetItemCount();
	for(int iItem = 0; iItem < nCnt; iItem ++)
	{
		SelectItem(iItem);
	}
}

void CTitleLView::OnLvnOdfinditem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVFINDITEM pFindInfo = reinterpret_cast<LPNMLVFINDITEM>(pNMHDR);
	// TODO: Add your control notification handler code here
	CTimeMDoc* pDoc = GetDocument();
	int nPos = pDoc->DoSearch(pFindInfo->iStart, FALSE, TRUE, pFindInfo->lvfi.psz);
	*pResult = nPos;
}

void CTitleLView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu menu;
	menu.LoadMenu(IDR_MENU_LISTCONTEXT);
	CMenu* pSubContext = menu.GetSubMenu(0);
	if(pSubContext != NULL)
	{
		if(GetSelectedCount() < 1)
		{
			pSubContext->EnableMenuItem(ID_EDIT_CUT, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_EDIT_COPY, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_EXTEND, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_DELETE, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_EXCHANGE, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_BREAKBYUSER, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_ZEROTIME, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_EMPTYCONTENT, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_MOVETIME, MF_BYCOMMAND|MF_GRAYED);
		}
		if(GetSelectedCount() < 2)
		{
			pSubContext->EnableMenuItem(ID_TITLE_CBONELINE, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_CBMULTILINE, MF_BYCOMMAND|MF_GRAYED);
		}
		CTimeMDoc* pDoc = GetDocument();
		if((!pDoc->IsInPlaying())||(-1 == pDoc->GetCurrentPos()))
		{
			pSubContext->EnableMenuItem(ID_TITLE_SNAPLINE, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_SNAPAFTER, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_SNAPALL, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_SNAPLINEEND, MF_BYCOMMAND|MF_GRAYED);
			pSubContext->EnableMenuItem(ID_TITLE_SNAPLINESTART, MF_BYCOMMAND|MF_GRAYED);
		}
		if(!pDoc->IsCanUndo())
		{
			pSubContext->EnableMenuItem(ID_EDIT_UNDO, MF_BYCOMMAND|MF_GRAYED);
		}
		if(!pDoc->IsCanRedo())
		{
			pSubContext->EnableMenuItem(ID_EDIT_REDO, MF_BYCOMMAND|MF_GRAYED);
		}

		pSubContext->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

void CTitleLView::OnSortChanged(int nCol, BOOL bAscending)
{
	CTimeMDoc* pDoc = GetDocument();
	if(nCol == 0)
		pDoc->SortChanged(nCol, TRUE);
	else
		pDoc->SortChanged(nCol, bAscending);
}

HFONT CTitleLView::OnGetCellFont(int nRow, int nCol, DWORD dwData)
{
	HFONT hFont = CMLListView::OnGetCellFont(nRow, nCol, dwData);
	CFont* pFont = CFont::FromHandle(hFont);
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	CTimeMDoc* pDoc = GetDocument();
	lf.lfCharSet = (BYTE)pDoc->GetDispCharset();
	if(ANSI_CHARSET != lf.lfCharSet)
	{
		if(m_hFontDisp != NULL)
			DeleteObject(m_hFontDisp);
		m_hFontDisp = CreateFontIndirect(&lf);
		return m_hFontDisp;
	}

	return hFont;
}

void CTitleLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bCapture)
	{
		m_bCapture = TRUE;
		SetCapture();
		m_nStartSel = GetListCtrl().HitTest(point);
		if(-1 == m_nStartSel)
			m_nStartSel = 0;
	}

	CMLListView::OnLButtonDown(nFlags, point);
}

void CTitleLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bCapture)
	{
		m_bCapture = FALSE;
		ReleaseCapture();
	}

	CMLListView::OnLButtonUp(nFlags, point);
}

void CTitleLView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bCapture)
	{
//		UINT nFlag = 0;
//		int nItem = GetListCtrl().HitTest(point, &nFlag);
//		for(int iItem = m_nStartSel; iItem < nItem + 1; iItem ++)
//		{
//			GetListCtrl().SetItemState(iItem, LVIS_SELECTED, LVIS_SELECTED);
//		}
	}

	CMLListView::OnMouseMove(nFlags, point);
}

void CTitleLView::OnHideAsscode()
{
	// TODO: Add your command handler code here
	m_bHideAssCode = !m_bHideAssCode;
	Invalidate();
}

void CTitleLView::OnUpdateHideAsscode(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bHideAssCode);
}
