#pragma once
#include "afxwin.h"
#include "TitleTimeEdit.h"


// CInsertCountDlg dialog

class CInsertCountDlg : public CDialog
{
	DECLARE_DYNAMIC(CInsertCountDlg)

public:
	CInsertCountDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInsertCountDlg();

// Dialog Data
	enum { IDD = IDD_DLG_INSERTCOUNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTitleTimeEdit m_InitTime;

	CString m_strTimeInit;
	UINT	m_nLineCount;
	BOOL	m_bEmpty;
	BOOL	m_bInitTime;
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChkInittime();
};
