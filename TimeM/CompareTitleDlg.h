#pragma once
#include "TitleUnit.h"
#include "CompareTitleListCtrl.h"
#include "TitleCompare.h"

// CCompareTitleDlg dialog

class CCompareTitleDlg : public CDialog
{
	DECLARE_DYNAMIC(CCompareTitleDlg)

public:
	CCompareTitleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCompareTitleDlg();

// Dialog Data
	enum { IDD = IDD_DLG_COMPARETITLE };

	BOOL IsSelectedMatch();
	BOOL DoFilePrompt(CString& strFile);
	void GetDispInfoTitleItem(NMLVDISPINFO *pDispInfo, PTITLE_UNIT pUnit);
	void SyncListScroll(const CListCtrl& listSrc, CListCtrl& listTag);

public:
	BOOL			m_bLockScroll;
	CCompareTitleListCtrl	m_ListTitleStd;
	CCompareTitleListCtrl	m_ListTitleTrans;

	CTitleCompare	m_TitleRel;

	CString			m_strStdFile;
	CString			m_strTransFile;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBtnBrowtrans();
	afx_msg void OnBnClickedBtnBrowstd();

	afx_msg void OnLvnGetdispinfoListStdtitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoListTranstitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnLvnEndScrollListStdtitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndScrollListTranstitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListStdtitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListTranstitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedChkSyncscroll();
	afx_msg LRESULT OnSyncListScroll(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSnapsel();
};
