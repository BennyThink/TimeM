// TitleListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "TitleListCtrl.h"


// CTitleListCtrl
/////////////////////////////////////////////////////////////////////////////
// CTitleListCtrl

IMPLEMENT_DYNAMIC(CTitleListCtrl, CListCtrl)

CTitleListCtrl::CTitleListCtrl()
{
	m_nFocusItem = -1;
}

CTitleListCtrl::~CTitleListCtrl()
{
}

//{{AFX_MSG_MAP(CTitleListCtrl)
BEGIN_MESSAGE_MAP(CTitleListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_STYLECHANGED, &CTitleListCtrl::OnStyleChanged)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_DRAWITEM_REFLECT()
END_MESSAGE_MAP()
//}}AFX_MSG_MAP

/////////////////////////////////////////////////////////////////////////////
// CTitleListCtrl message handlers

BOOL CTitleListCtrl::InitList()
{
	ModifyStyle(0, LVS_REPORT|LVS_OWNERDRAWFIXED);
	SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	return TRUE;
}

void CTitleListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	if (pThreadState->m_pWndInit == NULL)
	{
		if (!InitList())
		{
			ASSERT(FALSE);
		}
	}
}

int CTitleListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!InitList())
	{
		return -1;
	}

	return 0;
}

BOOL CTitleListCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect	rectClient;
	CRect	rectItem;
	CRect	rectFill;
	int		nCntItem	= GetItemCount();

	int	nTop = GetTopIndex();
	GetClientRect(rectClient);

	GetItemRect(nTop, rectItem, LVIR_BOUNDS);
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

	GetItemRect(nCntItem - 1, rectItem, LVIR_BOUNDS);
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
	UINT nSelCnt = GetSelectedCount();
	if(nSelCnt > 0)
	{
		POSITION posSel = GetFirstSelectedItemPosition();
		while(posSel != NULL)
		{
			GetItemRect(GetNextSelectedItem(posSel), rcItem, LVIR_BOUNDS);
			rcSel.UnionRect(rcSel, rcItem);
		}
		rectFill.IntersectRect(rcSel, rectClient);
		pDC->FillSolidRect(rectFill, GetSysColor(COLOR_WINDOW));
	}

	return TRUE;	//CListView::OnEraseBkgnd(pDC);
}

void CTitleListCtrl::OnSysColorChange()
{
	CListCtrl::OnSysColorChange();

	RedrawWindow();
}

LRESULT CTitleListCtrl::OnStyleChanged(WPARAM wp, LPARAM lp)
{
	int nStyleType = (int) wp;
	LPSTYLESTRUCT lpStyleStruct = (LPSTYLESTRUCT) lp;

	CListCtrl::OnStyleChanged(nStyleType, lpStyleStruct);

	return 0;
}

void CTitleListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
}

/*
clrBkOdd		= RGB(225, 241, 255);
clrBkEven		= RGB(255, 255, 255);
clrBkSel		= RGB(204, 218, 241);
clrBkFocus	= RGB(255, 128, 128);
*/
COLORREF CTitleListCtrl::OnGetItemFocusColor()
{
	return RGB(255, 128, 128);//GetTextColor();
}

COLORREF CTitleListCtrl::OnGetCellTextColor(int nRow, int nCol)
{
	UINT nState = GetItemState(nRow, LVIS_SELECTED);
	if(nCol == 0)
	{
		return GetTextColor();
	}
	if(nState == LVIS_SELECTED)
		return RGB(255, 255, 128);
	return GetTextColor();
}

COLORREF CTitleListCtrl::OnGetCellBkColor(int nRow, int nCol)
{
	UINT nState = GetItemState(nRow, LVIS_SELECTED);
	if(nState == LVIS_SELECTED)
		return RGB(0, 64, 128);

	if(nRow %2 == 0)
		return RGB(225, 241, 255);
	else
		return RGB(255, 255, 255);
}

HFONT CTitleListCtrl::OnGetCellFont(int nRow, int nCol, DWORD dwData)
{ 
	return (HFONT)GetStockObject(DEFAULT_GUI_FONT);
}

void CTitleListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dcItem;
	dcItem.Attach(lpDrawItemStruct->hDC);
	lpDrawItemStruct->itemState;

	CHeaderCtrl* pHdr = GetHeaderCtrl();
	int nColCount = pHdr->GetItemCount();

	CRect rcCell = lpDrawItemStruct->rcItem;
	rcCell.right = 0;

	CRect rcCellText;
	for(int iCol = 0; iCol < nColCount; iCol ++)
	{
		int nWidthCol = GetColumnWidth(iCol);
		rcCell.right = rcCell.left + nWidthCol;

		CString strItem = GetItemText(lpDrawItemStruct->itemID, iCol);

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

void CTitleListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// 需要修改父窗口的OnMeasureItem，并调用SendChildNotifyLastMsg产生反射消息
	lpMeasureItemStruct->itemHeight = 32;
}

int	CTitleListCtrl::GetFocusItem()
{
	int nSize = GetItemCount();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		if(LVIS_FOCUSED == GetItemState(iItem, LVIS_FOCUSED))
			return iItem;
	}
	return -1;
}

void CTitleListCtrl::SetFocusItem(int nItem)
{
	int nSize = GetItemCount();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		if(nItem == iItem)
			SetItemState(iItem, LVIS_FOCUSED, LVIS_FOCUSED);
		else
			SetItemState(iItem, 0, LVIS_FOCUSED);
	}
	Invalidate(FALSE);
}
