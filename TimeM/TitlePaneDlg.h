#pragma once

#include "TitleTimeEdit.h"
#include "TMEdit.h"
#include "TimeMDoc.h"

// CTitlePaneDlg

class CTitlePaneDlg : public CPaneDialog
{
	DECLARE_DYNAMIC(CTitlePaneDlg)

public:
	CTitlePaneDlg();
	virtual ~CTitlePaneDlg();
public:
	CTMEdit			m_TitleCtrl;
	CTMEdit			m_CntLineCtrl;

	CString			m_strStartTime;
	CString			m_strEndTime;
	CString			m_strTitle;
	CString			m_strCharCount;

	CTitleTimeEdit	m_EditStart;
	CTitleTimeEdit	m_EditEnd;


public:
	BOOL IsInputFocus();
	void FocusTitle();
	CTimeMDoc* GetDocument() const;
	void UpdateDocData(BOOL bSave = TRUE);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	void OnSaveItem();
	void OnLoadItem();

	void UpdateLine(CEdit& editSrc, CEdit& editTag);

	void AdjustLayout();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnKillfocusEditStart();
	afx_msg void OnEnKillfocusEditEnd();
	afx_msg void OnEnKillfocusEditTitle();
	afx_msg void OnEnChangeEditTitle();
	afx_msg void OnEnVscrollEditTitle();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnSetfocusEditTitle();
	afx_msg void OnClose();
};
