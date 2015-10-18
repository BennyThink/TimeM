#pragma once
#include "titlelistctrl.h"
#include "TitleCompare.h"

class CCompareTitleListCtrl :
	public CTitleListCtrl
{
	CTitleCompare* m_pCompare;
	int	m_nDirect;
public:
	CCompareTitleListCtrl(void);
	~CCompareTitleListCtrl(void);

	void AttachCompare(CTitleCompare* pCompTitle, int nDirect);

	COLORREF StateToTextColor(int nState);
	COLORREF StateToBkColor(int nState);

	virtual COLORREF OnGetCellTextColor(int nRow, int nCol);
	virtual COLORREF OnGetCellBkColor(int nRow, int nCol);
};
