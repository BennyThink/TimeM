// MLListView.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "MLListView.h"


// CMLListView

IMPLEMENT_DYNCREATE(CMLListView, CListView)

CMLListView::CMLListView()
{
	m_nSortCol = 0;
	m_bAscending = TRUE;
}

CMLListView::~CMLListView()
{
}

BEGIN_MESSAGE_MAP(CMLListView, CListView)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CMLListView::OnLvnColumnclick)
END_MESSAGE_MAP()


// CMLListView diagnostics

#ifdef _DEBUG
void CMLListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CMLListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG

void CMLListView::GetItemText(int nItem, int nCol, CString& strItem)
{
	strItem = GetListCtrl().GetItemText(nItem, nCol);
}

void CMLListView::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 20;
}

void CMLListView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dcItem;
	dcItem.Attach(lpDrawItemStruct->hDC);
	lpDrawItemStruct->itemState;

	CHeaderCtrl* pHdr = GetListCtrl().GetHeaderCtrl();
	int nColCount = pHdr->GetItemCount();

	CRect rcCell = lpDrawItemStruct->rcItem;
	rcCell.right = 0;

	CRect rcCellText;
	for(int iCol = 0; iCol < nColCount; iCol ++)
	{
		int nWidthCol = GetListCtrl().GetColumnWidth(iCol);
		rcCell.right = rcCell.left + nWidthCol;

		CString strItem;
		GetItemText(lpDrawItemStruct->itemID, iCol, strItem);

		dcItem.FillSolidRect(rcCell, OnGetCellBkColor(lpDrawItemStruct->itemID, iCol));
		rcCellText = rcCell;

		rcCellText.DeflateRect(2, 1, 0, 0);
		dcItem.SelectObject(OnGetCellFont(lpDrawItemStruct->itemID, iCol));
		dcItem.SetTextColor(OnGetCellTextColor(lpDrawItemStruct->itemID, iCol));
		dcItem.DrawText(strItem, rcCellText, DT_EXTERNALLEADING|DT_LEFT);
		rcCell.left = rcCell.right;
	}

	if((lpDrawItemStruct->itemState & ODS_FOCUS) == ODS_FOCUS)
	{
		CRect rcFocus = lpDrawItemStruct->rcItem;
		rcFocus.DeflateRect(0, 0, 0, 1);
		dcItem.Draw3dRect(rcFocus, OnGetItemFocusColor(), OnGetItemFocusColor());
	}

	dcItem.SelectStockObject(SYSTEM_FONT) ;
	dcItem.Detach();
}

// CMLListView message handlers

BOOL CMLListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_REPORT|LVS_OWNERDRAWFIXED;
	return CListView::PreCreateWindow(cs);
}

BOOL CMLListView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect	rectClient;
	CRect	rectItem;
	CRect	rectFill;
	int		nCntItem	= GetListCtrl().GetItemCount();

	int	nTop = GetListCtrl().GetTopIndex();
	GetClientRect(rectClient);

	GetListCtrl().GetItemRect(nTop, rectItem, LVIR_BOUNDS);
	if(rectClient.top < rectItem.top)
	{
		rectFill = CRect(
			rectClient.left,
			rectClient.top,
			rectClient.right,
			rectItem.top);
		pDC->FillSolidRect(rectFill, GetSysColor(COLOR_WINDOW));
	}

	if(rectClient.left < rectItem.left+3)
	{
		rectFill = CRect(
			rectClient.left,
			rectClient.top,
			rectItem.left + 3,
			rectClient.bottom);
		pDC->FillSolidRect(rectFill, GetSysColor(COLOR_WINDOW));
	}

	GetListCtrl().GetItemRect(nCntItem - 1, rectItem, LVIR_BOUNDS);
	if(rectItem.right < rectClient.right)
	{
		rectFill = CRect(
			rectItem.right,
			rectClient.top,
			rectClient.right,
			rectClient.bottom);
		pDC->FillSolidRect(rectFill, GetSysColor(COLOR_WINDOW));
	}
	if(rectItem.bottom < rectClient.bottom)
	{
		rectFill = CRect(
			rectClient.left,
			rectItem.bottom,
			rectClient.right,
			rectClient.bottom);
		pDC->FillSolidRect(rectFill, GetSysColor(COLOR_WINDOW));
	}


	CRect rcSel(0, 0, 0, 0);
	CRect rcItem(0, 0, 0, 0);
	UINT nSelCnt = GetListCtrl().GetSelectedCount();
	if(nSelCnt > 0)
	{
		POSITION posSel = GetListCtrl().GetFirstSelectedItemPosition();
		while(posSel != NULL)
		{
			GetListCtrl().GetItemRect(GetListCtrl().GetNextSelectedItem(posSel), rcItem, LVIR_BOUNDS);
			rcSel.UnionRect(rcSel, rcItem);
		}
		rectFill.IntersectRect(rcSel, rectClient);
		pDC->FillSolidRect(rectFill, GetSysColor(COLOR_WINDOW));
	}
	return TRUE;	//CListView::OnEraseBkgnd(pDC);
}

int	CMLListView::GetFocusItem()
{
	int nSize = GetListCtrl().GetItemCount();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		if(LVIS_FOCUSED == GetListCtrl().GetItemState(iItem, LVIS_FOCUSED))
			return iItem;
	}
	return -1;
}

void CMLListView::SetFocusItem(int nItem)
{
	int nSize = GetListCtrl().GetItemCount();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		if(nItem == iItem)
			GetListCtrl().SetItemState(iItem, LVIS_FOCUSED, LVIS_FOCUSED);
		else
			GetListCtrl().SetItemState(iItem, 0, LVIS_FOCUSED);
	}
	Invalidate(FALSE);
}

COLORREF CMLListView::OnGetItemFocusColor()
{
	return RGB(255, 128, 128);//GetTextColor();
}

COLORREF CMLListView::OnGetCellTextColor(int nRow, int nCol)
{
	UINT nState = GetListCtrl().GetItemState(nRow, LVIS_SELECTED);
	if(nState == LVIS_SELECTED)
		return RGB(255, 255, 128);
	return GetListCtrl().GetTextColor();
}

COLORREF CMLListView::OnGetCellBkColor(int nRow, int nCol)
{
	UINT nState = GetListCtrl().GetItemState(nRow, LVIS_SELECTED);
	if(nState == LVIS_SELECTED)
		return RGB(0, 64, 128);

	if(nRow %2 == 0)
		return RGB(225, 241, 255);
	else
		return RGB(255, 255, 255);
//	return GetBkColor();
}

HFONT CMLListView::OnGetCellFont(int nRow, int nCol, DWORD dwData)
{ 
	return (HFONT)GetStockObject(DEFAULT_GUI_FONT);
}

void CMLListView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	SwitchSortColumn(pNMLV->iSubItem);

	*pResult = 0;
}

BOOL CMLListView::SwitchSortColumn(int nCol)
{
	if(!CanSort(nCol))
		return FALSE;

	if(m_nSortCol == nCol)
	{
		m_bAscending = !m_bAscending;
		OnSortChanged(m_nSortCol, m_bAscending);
	}
	else
	{
		m_nSortCol = nCol;
		m_bAscending = TRUE;
		OnSortChanged(m_nSortCol, m_bAscending);
	}
	Invalidate(FALSE);
	return TRUE;
}
