#pragma once

#include "TitleUnit.h"
#include "StateTitleListCtrl.h"
#include "TitleRelation.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "TitleTimeEdit.h"

// CCombinTitleTransDlg dialog

class CCombinTitleTransDlg : public CDialog
{
	DECLARE_DYNAMIC(CCombinTitleTransDlg)

public:
	CCombinTitleTransDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCombinTitleTransDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_DLG_TITLECOMBINE };
public:
	BOOL			m_bLockScroll;

	BOOL			m_bStdModified;
	BOOL			m_bStdUnicode;
	CString			m_strStdFmtHdr;
	CString			m_strStdFile;
	CStateTitleListCtrl	m_ListTitleStd;

	BOOL			m_bTransModified;
	BOOL			m_bTransUnicode;
	CString			m_strTransFmtHdr;
	CString			m_strTransFile;
	CStateTitleListCtrl	m_ListTitleTrans;

	CTitleRelation	m_TitleRel;

	CString m_strTitle1;
	CString m_strStart1;
	CString m_strEnd1;
	CTitleTimeEdit	m_EditStart1;
	CTitleTimeEdit	m_EditEnd1;
	int		m_nFocus1;

	CString m_strTitle2;
	CString m_strStart2;
	CString m_strEnd2;
	CTitleTimeEdit	m_EditStart2;
	CTitleTimeEdit	m_EditEnd2;
	int		m_nFocus2;
	CMFCMenuButton	m_mbtnCombine;
	CMenu			m_menuCombine;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void OpenExternal(LPCTSTR lpszFile);
	BOOL DoFilePrompt(CString& strFile);
	void SyncListScroll(const CListCtrl& listSrc, CListCtrl& listTag);
	void GetDispInfoTitleItem(NMLVDISPINFO *pDispInfo, PTITLE_UNIT pUnit, int nNo);
	BOOL IsSelectedMatch();

	void SetStdModifiedFlag(){m_bStdModified = TRUE;};
	void SaveStdModified(){m_bStdModified = FALSE;};
	BOOL IsStdModified(){return m_bStdModified;};

	void SetTransModifiedFlag(){m_bTransModified = TRUE;};
	void SaveTransModified(){m_bTransModified = FALSE;};
	BOOL IsTransModified(){return m_bTransModified;};

	void RebuildRelationAll();
	void UpdateCtrlState();

	void CombineSelected();
	void CombineAll();
	void CombineFromTo();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBrowstd();
	afx_msg void OnBnClickedBtnBrowtrans();
	afx_msg LRESULT OnSyncListScroll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyListRebuild(WPARAM wParam, LPARAM lParam);

	afx_msg void OnLvnGetdispinfoListStdtitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoListTranstitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnLvnItemchangedListStdtitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListTranstitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndScrollListStdtitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndScrollListTranstitle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSnapsel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedChkSyncscroll();
	afx_msg void OnBnClickedBtnUndo();
	afx_msg void OnBnClickedBtnRedo();
	afx_msg void OnEnChangeEditStart1();
	afx_msg void OnEnChangeEditEnd1();
	afx_msg void OnEnChangeEditTitle1();
	afx_msg void OnEnChangeEditStart2();
	afx_msg void OnEnChangeEditEnd2();
	afx_msg void OnEnChangeEditTitle2();
	afx_msg void OnBnClickedBtnEditstd();
	afx_msg void OnBnClickedBtnReloadstd();
	afx_msg void OnBnClickedBtnEdittrans();
	afx_msg void OnBnClickedBtnReloadtrans();
	afx_msg void OnBnClickedBtnUndo2();
	afx_msg void OnBnClickedBtnRedo2();
	afx_msg void OnBnClickedSave2();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
