#pragma once


// CTitleListCtrl

class CTitleListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CTitleListCtrl)

// Construction
public:
	CTitleListCtrl();

// Attributes
public:
	int	m_nFocusItem;

protected:

// Operations
public:
	int	GetFocusItem();
	void SetFocusItem(int nItem);

	// Support for individual cells text/background colors:
	virtual COLORREF OnGetItemFocusColor();
	virtual COLORREF OnGetCellTextColor(int nRow, int nCol);
	virtual COLORREF OnGetCellBkColor(int nRow, int nCol);
	virtual HFONT OnGetCellFont(int nRow, int nCol, DWORD dwData = 0);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

protected:
	virtual void PreSubclassWindow();

	BOOL InitList();
// Implementation
public:
	virtual ~CTitleListCtrl();

protected:
	//{{AFX_MSG(CTitleListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSysColorChange();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnStyleChanged(WPARAM wp, LPARAM lp);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
