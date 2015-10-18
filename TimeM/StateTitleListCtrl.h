#pragma once
#include "titlelistctrl.h"
#include "TitleRelation.h"

#define	MSG_REBUILDRELATION (WM_USER+55)

class CStateTitleListCtrl :
	public CTitleListCtrl
{
	CTitleRelation*	m_pRelVTTitle;
	int	m_nDirect;	// 0 = left, 1 = right
public:
	CStateTitleListCtrl(void);
	~CStateTitleListCtrl(void);

	void AttachVTTitle(CTitleRelation* pRelVTTitle, int nDirect);
	void NoSelect();

	virtual COLORREF OnGetCellBkColor(int nRow, int nCol);
	virtual COLORREF OnGetCellTextColor(int nRow, int nCol);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnTitleDelete();
	afx_msg void OnTitleBreakbyuser();
	afx_msg void OnTitleCboneline();
	afx_msg void OnTitleCbmultiline();
};
