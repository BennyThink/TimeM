#pragma once

#include "TitleListCtrl.h"

#define	MSG_QUERYCELLTEXTCOLOR	(WM_USER+100)
#define	MSG_QUERYCELLBKCOLOR	(WM_USER+101)
// CClrListCtrl

class CClrListCtrl : public CTitleListCtrl
{
	DECLARE_DYNAMIC(CClrListCtrl)

public:
	CClrListCtrl();
	virtual ~CClrListCtrl();

	virtual COLORREF OnGetItemFocusColor();
	virtual COLORREF OnGetCellTextColor(int nRow, int nCol);
	virtual COLORREF OnGetCellBkColor(int nRow, int nCol);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
protected:
	DECLARE_MESSAGE_MAP()
};
