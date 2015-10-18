#pragma once
#include "afxcmn.h"
#include "TitleUnit.h"
#include "TitleListCtrl.h"
#include "TitleRelation.h"


// CCombineTitleByTimeDlg dialog

class CCombineTitleByTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CCombineTitleByTimeDlg)

public:
	CCombineTitleByTimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCombineTitleByTimeDlg();

// Dialog Data
	enum { IDD = IDD_DLG_CMBN_DOC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL			m_bLockScroll;
	CTitleListCtrl	m_ListTitleStd;
	CTitleListCtrl	m_ListTitleTrans;

	CTitleRelation	m_TitleRel;
	BOOL			m_bModified;

	CString			m_strFmtHdr;
	CString			m_strStdFile;
	BOOL			m_bStdUnicode;
	CString			m_strTransFile;
protected:
	BOOL DoFilePrompt(CString& strFile);
	virtual void OnOK();
	BOOL IsSelectedMatch();
	void GetDispInfoTitleItem(NMLVDISPINFO *pDispInfo, PTITLE_UNIT pUnit);
	void SyncListScroll(const CListCtrl& listSrc, CListCtrl& listTag);
	void SetModifiedFlag(){m_bModified = TRUE;};
	void SaveModified(){m_bModified = FALSE;};
	BOOL IsModified(){return m_bModified;};
public:
	afx_msg void OnBnClickedBtnBrowtrans();
	afx_msg void OnBnClickedBtnBrowstd();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnLvnGetdispinfoListStdtitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoListTranstitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnBnClickedBtnSnapauto();
	afx_msg void OnBnClickedBtnSnapsel();
	afx_msg void OnLvnEndScrollListStdtitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndScrollListTranstitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListStdtitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListTranstitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedChkSyncscroll();
	afx_msg LRESULT OnSyncListScroll(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedSave();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedBtnToLeftempty();
	afx_msg void OnBnClickedBtnToRightempty();
};
