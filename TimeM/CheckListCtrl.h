#pragma once

#include "CheckHdrCtrl.h"
// CCheckListCtrl

#define	MSG_LISTITEM_CHANGED	(WM_USER+60)

class CCheckListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CCheckListCtrl)

public:
	CCheckListCtrl();
	virtual ~CCheckListCtrl();

	int GetPrevCheckItem(int nStartItem = -1);
	int GetNextCheckItem(int nStartItem = -1);
	CImageList* SetImageList(CImageList* pImageList, int nImageListType);
	void SetColumnImage(int nCol, int nImage);
	void SetHdrChkImage(UINT nBmpID, int cx, int cy, COLORREF clrMask);

//	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	DECLARE_MESSAGE_MAP()
	
	afx_msg LRESULT OnHdrChk(WPARAM wParam, LPARAM lParam);

	virtual void PreSubclassWindow();
protected:
	CCheckHdrCtrl m_hdrCtrl;
	CImageList		m_imgChk;
public:
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};
