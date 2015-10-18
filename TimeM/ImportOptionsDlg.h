#pragma once


// CImportOptionsDlg dialog

class CImportOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CImportOptionsDlg)

public:
	CImportOptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportOptionsDlg();
	BOOL	m_bSingleLine;

// Dialog Data
	enum { IDD = IDD_DLG_IMPORTOPTION };

	BOOL GetSingleLineOption();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
