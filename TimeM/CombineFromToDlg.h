#pragma once


// CCombineFromToDlg dialog

class CCombineFromToDlg : public CDialog
{
	DECLARE_DYNAMIC(CCombineFromToDlg)

public:
	CCombineFromToDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCombineFromToDlg();

// Dialog Data
	enum { IDD = IDD_DLG_RANGETITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_nLineFrom;
	UINT m_nLineTo;
};
