#pragma once


// CTitleFilterDlg dialog

class CTitleFilterDlg : public CDialog
{
	DECLARE_DYNAMIC(CTitleFilterDlg)

public:
	CTitleFilterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTitleFilterDlg();

// Dialog Data
	enum { IDD = IDD_DLG_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	void UpdateUIStates();
public:
	CString m_strSpecChar;
	BOOL	m_bEmpty;
	BOOL	m_bSpecChar;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChkRemovespecchar();
	afx_msg void OnBnClickedChkRemoveempty();
};
