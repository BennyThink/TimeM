#pragma once


// CMLListView view

class CMLListView : public CListView
{
	DECLARE_DYNCREATE(CMLListView)

protected:
	CMLListView();           // protected constructor used by dynamic creation
	virtual ~CMLListView();

	int		m_nSortCol;
	BOOL	m_bAscending;
public:
	virtual void GetItemText(int nItem, int nCol, CString& strItem);

	// Support for individual cells text/background colors:
	virtual COLORREF OnGetItemFocusColor();
	virtual COLORREF OnGetCellTextColor(int nRow, int nCol);
	virtual COLORREF OnGetCellBkColor(int nRow, int nCol);
	virtual HFONT OnGetCellFont(int nRow, int nCol, DWORD dwData = 0);

	BOOL IsSortAscending(){return m_bAscending;};
	int GetSortColumn(){return m_nSortCol;};
	int	GetFocusItem();
	void SetFocusItem(int nItem);
	BOOL SwitchSortColumn(int nCol);
	virtual void OnSortChanged(int nCol, BOOL bAscending){};
	virtual BOOL CanSort(int nCol){return TRUE;};
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	// Overridables
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
};
