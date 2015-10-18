#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "TitleTimeEdit.h"
#include "TimeMDoc.h"

// CInsertIntervalDlg dialog

class CInsertIntervalDlg : public CDialog
{
	DECLARE_DYNAMIC(CInsertIntervalDlg)

public:
	CInsertIntervalDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInsertIntervalDlg();

// Dialog Data
	enum { IDD = IDD_DLG_LISTINTERVAL };

	CTitleTimeEdit	m_EditStart;
	CTitleTimeEdit	m_EditEnd;

	CString			m_strStartTime;
	CString			m_strEndTime;

	CString			m_strTitle;
	CListCtrl		m_ListInterval;
	LISTSEL_MAP*	m_pmapListInterval;
	CTimeMDoc*		m_pDoc;
	int				m_nInterval;
	int				m_nSelTitlePos;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedListInterval(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditEnd();
	afx_msg void OnEnChangeEditStart();
};
