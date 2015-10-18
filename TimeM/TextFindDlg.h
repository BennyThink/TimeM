#pragma once


// CTextFindDlg dialog

class CTextFindDlg : public CDialog
{
	DECLARE_DYNAMIC(CTextFindDlg)

public:
	CTextFindDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTextFindDlg();

// Dialog Data
	enum { IDD = IDD_DLG_FIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL	m_bCaseSense;
	BOOL	m_bDNDirect;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strToFind;
};
