#pragma once


// CSaveOptionPage dialog

class CSaveOptionPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSaveOptionPage)

public:
	CSaveOptionPage();
	virtual ~CSaveOptionPage();

// Dialog Data
	enum { IDD = IDD_DLG_SAVESETTING };

	UINT	m_nTimeDelay;
	UINT	m_nMaxBak;
	CString m_strBakPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL HasRegistered(LPCTSTR pszType);
	void RegFileType(LPCTSTR pszType);
	void UnRegFileType(LPCTSTR pszType);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChkAutosave();
	afx_msg void OnBnClickedChkAutobak();
	afx_msg void OnEnKillfocusEditTimedelay();
	afx_msg void OnEnKillfocusEditBakmax();
	afx_msg void OnBnClickedBtnBrow();
	afx_msg void OnBnClickedChkSrtassociate();
	afx_msg void OnBnClickedChkAssassociate();
	afx_msg void OnBnClickedChkSsaassociate2();
	afx_msg void OnBnClickedChkSsaassociate();
};
