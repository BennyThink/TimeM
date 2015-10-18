#pragma once
#include "titletimeedit.h"


// CInputExtraTimeDlg dialog

class CInputExtraTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputExtraTimeDlg)

public:
	CInputExtraTimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputExtraTimeDlg();

// Dialog Data
	enum { IDD = IDD_DLG_EXTRATIME_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTitleTimeEdit	m_TimeExtra;
	CString			m_strTimeExtra;
};
