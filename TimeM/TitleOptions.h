#pragma once

#include "TitleParams.h"
#include "TitleTimeEdit.h"

// CTitleOptionsPage dialog

class CTitleOptionsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTitleOptionsPage)

public:
	CTitleOptionsPage();
	virtual ~CTitleOptionsPage();

// Dialog Data
	enum { IDD = IDD_DLG_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	int		GetTimeDelay(){return m_nTimeDelay;};
	int		GetTitleDefLen(){return m_nTitleDeflen;};
	void	SetTimeDelay(int nTimeDelay);
	void	SetTitleDefLen(int nDefLen);
	CString	GetZipPath(){return m_strToolPath;};
	void	SetZipPath(const CString& strZipPath){m_strToolPath = strZipPath;};
protected:
	CString			m_strTimeDelay;
	CString			m_strDefTitleLen;
	CTitleTimeEdit	m_EditTimeDelay;
	CTitleTimeEdit	m_EditTimeDefLen;
	int				m_nTimeDelay;
	int				m_nTitleDeflen;
	CString			m_strToolPath;
public:
	afx_msg void OnBnClickedBtnBrowziptool();
	afx_msg void OnEnKillfocusEditTimeOffset();
	afx_msg void OnEnKillfocusEditTimeDeflen();
	afx_msg void OnBnClickedRbtnBack();
	afx_msg void OnBnClickedRbtnForward();
};
