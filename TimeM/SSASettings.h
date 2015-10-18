#pragma once
#include "afxwin.h"


// CSSASettings dialog

class CSSASettings : public CDialog
{
	DECLARE_DYNAMIC(CSSASettings)

public:
	CSSASettings(BOOL bAdvanced = FALSE, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSSASettings();

// Dialog Data
	enum { IDD = IDD_DLG_SSASETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	CComboBox m_cbLinePrevs;
	CComboBox m_cbLinePosts;

	void ReadRecentCodes();
	void SaveRecentCodes();
public:
	CString m_strStyleName;
	CString m_strInfoName;
	CString m_strEventName;
	CString m_strSSAInfo;
	CString m_strSSAV4Style;
	CString m_strPreCode;
	CString m_strPostCode;
	CString	m_strHdr;
	CString m_strEvents;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
