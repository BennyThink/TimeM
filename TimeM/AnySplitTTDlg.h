#pragma once

#include "TitleSplitorWnd.h"
// CAnySplitTTDlg dialog
class CAnySplitTTDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnySplitTTDlg)

public:
	CAnySplitTTDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnySplitTTDlg();

// Dialog Data
	enum { IDD = IDD_DLG_ANYSPLITTITLE };

	PTITLE_UNIT			m_pUnit;
	CString				m_strAssCode;
	VT_TITLE			m_vtBreak;
	CString				m_strTTLen;
	BOOL				m_bSingleLang;

	CTitleSplitorWnd	m_wndSplitor;
	const VT_TITLE& GetBreakTitles(){return m_vtBreak;};
	void InitTitleUnit(PTITLE_UNIT pUnit){m_pUnit = pUnit;};
	CString GetAssCode(const CString& strLine);
	BOOL CheckSplitPartner();
	BOOL CheckSplitHdrTail();
	BOOL IsSingleLang(){return m_bSingleLang;};
	void BreakTitleUnitSingleLang();
	void BreakTitleUnitDualLang();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRemovesplitor();
	afx_msg void OnBnClickedChkNew();
	afx_msg LRESULT OnSplitRemove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSplitNew(WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedRbtnSinglang();
	afx_msg void OnBnClickedRbtnDuallang();
};
