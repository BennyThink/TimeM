#pragma once


// CCPSelDlg dialog

class CCPSelDlg : public CDialog
{
	DECLARE_DYNAMIC(CCPSelDlg)

public:
	CCPSelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCPSelDlg();

// Dialog Data
	enum { IDD = IDD_DLG_CPSEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strCP;
};
