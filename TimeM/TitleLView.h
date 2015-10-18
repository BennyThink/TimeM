#pragma once

#include "TimeMDoc.h"
#include "MLListView.h"

// CTitleLView view

class CTitleLView : public CMLListView
{
	DECLARE_DYNCREATE(CTitleLView)

protected:
	CTitleLView();           // protected constructor used by dynamic creation
	virtual ~CTitleLView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	CTimeMDoc* GetDocument() const;

	void SelectItem(int nItem);
	UINT GetSelectedCount();
	POSITION GetFirstSelectedItemPosition();
	int GetNextSelectedItem(POSITION& pos);
	void NoSelect();
	virtual void OnSortChanged(int nCol, BOOL bAscending);
	virtual COLORREF OnGetCellBkColor(int nRow, int nCol);
	virtual COLORREF OnGetCellTextColor(int nRow, int nCol);

	HFONT	m_hFontDisp;
	int		m_nEditRow;
	int		m_nEditCol;
	BOOL	m_bCapture;
	int		m_nStartSel;
	BOOL	m_bHideAssCode;

	void EnsureCenter(int nPos);
protected:
	DECLARE_MESSAGE_MAP()
public:
	void GetEdit(int& nRow, int& nCol);
	void SetEdit(int nRow, int nCol);
	virtual HFONT OnGetCellFont(int nRow, int nCol, DWORD dwData = 0);
	virtual void GetItemText(int nItem, int nCol, CString& strItem);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLvnOdstatechanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEditSelectAll();
	afx_msg void OnLvnOdfinditem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHideAsscode();
	afx_msg void OnUpdateHideAsscode(CCmdUI *pCmdUI);
};
