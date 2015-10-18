#pragma once


// CSplitNumDlg dialog

class CSplitNumDlg : public CDialog
{
	DECLARE_DYNAMIC(CSplitNumDlg)

public:
	CSplitNumDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSplitNumDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SPLITFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_nSplitNum;
};
