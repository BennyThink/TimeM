#pragma once
#include "clrbutton.h"
#include "TitleTimeEdit.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ClrListCtrl.h"

class CTimeMDoc;

typedef struct tagCheckParam
{
	UINT	nErrType;
	UINT	nTitleId;
}CHECK_PARAM, *PCHECK_PARAM;
typedef vector<CHECK_PARAM> VT_CHKPARAMS;
typedef multimap<UINT, CHECK_PARAM> MAP_CHKPARAMS;
// CCheckValidDlg dialog

class CCheckValidDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckValidDlg)

public:
	CCheckValidDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckValidDlg();

// Dialog Data
	enum { IDD = IDD_DLG_CHECKVALID };

	void SetDocument(CTimeMDoc* pDoc){m_pDoc = pDoc;};
	CTimeMDoc*	m_pDoc;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	CClrButton		m_cbtn1;
	CClrButton		m_cbtn2;
	CClrButton		m_cbtn3;
	CClrButton		m_cbtn4;
	CClrButton		m_cbtn5;
	CClrButton		m_cbtn6;
	CClrButton		m_cbtn7;
	CTitleTimeEdit	m_TimeMax;
	CTitleTimeEdit	m_TimeMin;
	int				m_nChsCntMax;
	int				m_nTitleLineCnt;
	CString			m_strMaxTime;
	CString			m_strMinTime;
	CClrListCtrl	m_ListCheck;
	CSize			m_sizeMin;
	CSize			m_sizeList;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnQueryCellTextColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQueryCellBkColor(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLvnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnBnClickedBtnSelall();
	afx_msg void OnBnClickedBtnClearoverlap();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
