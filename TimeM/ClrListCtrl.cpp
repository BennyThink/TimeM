// ClrListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "ClrListCtrl.h"


// CClrListCtrl

IMPLEMENT_DYNAMIC(CClrListCtrl, CTitleListCtrl)

CClrListCtrl::CClrListCtrl()
{

}

CClrListCtrl::~CClrListCtrl()
{
}


BEGIN_MESSAGE_MAP(CClrListCtrl, CTitleListCtrl)
END_MESSAGE_MAP()

// CClrListCtrl message handlers
COLORREF CClrListCtrl::OnGetItemFocusColor()
{
	return RGB(0, 0, 0);//GetTextColor();
}

COLORREF CClrListCtrl::OnGetCellTextColor(int nRow, int nCol)
{
	return GetParent()->SendMessage(MSG_QUERYCELLTEXTCOLOR, nRow, nCol);
}

COLORREF CClrListCtrl::OnGetCellBkColor(int nRow, int nCol)
{
	return GetParent()->SendMessage(MSG_QUERYCELLBKCOLOR, nRow, nCol);
}

void CClrListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 20;
}
