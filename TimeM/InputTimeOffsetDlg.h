#pragma once
#include "titletimeedit.h"


// CInputTimeOffsetDlg dialog

class CInputTimeOffsetDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputTimeOffsetDlg)

public:
	CInputTimeOffsetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputTimeOffsetDlg();

// Dialog Data
	enum { IDD = IDD_DLG_TIME_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	CTitleTimeEdit	m_TimeOffset;
	CString			m_strTimeOffset;
	DWORD			m_dwFlag;
	int				m_nStart;
	int				m_nOffset;
public:
	virtual BOOL OnInitDialog();
	DWORD GetFlags(){return m_dwFlag;};
	int GetOffset(){return m_nOffset;};
	void SetTitleStart(int nStart){m_nStart = nStart;};
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
};
