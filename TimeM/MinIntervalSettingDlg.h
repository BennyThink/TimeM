#pragma once
#include "afxwin.h"
#include "TitleTimeEdit.h"


// CMinIntervalSettingDlg dialog

class CMinIntervalSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CMinIntervalSettingDlg)

public:
	CMinIntervalSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMinIntervalSettingDlg();

// Dialog Data
	enum { IDD = IDD_DLG_MIN_INTERVAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString			m_strMinInterval;
	CTitleTimeEdit	m_EditOffset;
};
