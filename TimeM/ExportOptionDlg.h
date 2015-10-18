#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "CheckListCtrl.h"


// CExportOptionDlg dialog

#define	FILE_FORMAT_NONE	0x0000
#define	FILE_FORMAT_SRT		0x0001
#define	FILE_FORMAT_SSA		0x0002
#define	FILE_FORMAT_ASS		0x0003

#define	LANG_FLAG_ENG		0x0001
#define	LANG_FLAG_CHS		0x0002
#define	LANG_FLAG_CHT		0x0004

typedef struct tagFileUnit
{
	WORD	wFmtID;
	WORD	wLangFlag;
	BOOL	bUnicode;
	CString	strName;
}UNIT_FILE, *PUNIT_FILE;

typedef vector<UNIT_FILE> VT_FILEUNIT;

typedef struct tagExportParam
{
	VT_FILEUNIT	vtFiles;
	BOOL		bZipOut;
	CString		strFmtHeader;
	CString		strPreCode;
	CString		strPostCode;
}EXPORT_PARAM, *LPEXPORT_PARAM;

class CExportOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(CExportOptionDlg)

public:
	CExportOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExportOptionDlg();

// Dialog Data
	enum { IDD = IDD_DLG_EXPORTOPTION };

	CCheckListCtrl	m_FileFmt;
	CString			m_strEngName;
	CString			m_strChsName;
	CString			m_strChtName;
	EXPORT_PARAM	m_ExportParam;

	int				m_nFocus;
	CString			m_strOutPath;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void UpdateUIStates();
	CString FmtFileUnit(UNIT_FILE& unit);
protected:
	virtual void OnOK();
public:
	afx_msg LRESULT OnListItemChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnSetass();
	afx_msg void OnBnClickedChkUnicode();
	afx_msg void OnLvnEndScrollListFilefmt(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListFilefmt(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnBrow();
};
