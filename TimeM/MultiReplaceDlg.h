#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "CheckListCtrl.h"

#define	MSG_TM_DOREPLACE	(WM_USER+10)

// CMultiReplaceDlg dialog
typedef struct tagReplaceUnit
{
//	DWORD	dwFlag;
	CString strToFind;
	CString strReplTo;
}REPLACE_UNIT, *PREPLACE_UNIT;

typedef pair<UINT, REPLACE_UNIT> REPL_PAIR;
typedef multimap<UINT, REPLACE_UNIT> MAP_REPLUNITS;
typedef multimap<UINT, REPLACE_UNIT>::const_reverse_iterator RITER_UNIT;

class CMultiReplaceDlg : public CDialog
{
	DECLARE_DYNAMIC(CMultiReplaceDlg)

public:
	CMultiReplaceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMultiReplaceDlg();

// Dialog Data
	enum { IDD = IDD_DLG_REPLACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL			m_bUseBatch;
	BOOL			m_bForSelected;

	BOOL			m_bCase;
	BOOL			m_bIncChs;
	BOOL			m_bIncEng;
	CString			m_strTarget;
	CString			m_strResult;

	CString			m_strTplFileName;
	BOOL			m_bModified;

	CCheckListCtrl	m_RplItemList;
	CRect			m_rcOrg;
	MAP_REPLUNITS	m_mapUnits;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChkUsebatch();
	afx_msg void OnBnClickedBtnAdditem();
	afx_msg void OnBnClickedBtnRemove();
	afx_msg void OnBnClickedBtnLoadtpl();
	afx_msg void OnBnClickedBtnSavetpl();
protected:
	virtual void OnOK();
	void UpdateCtrlStates();
	BOOL DoPromptOpenTpl();
	BOOL DoPromptSave();
	BOOL DoPromptSaveTpl();
	BOOL ReadStrLine(PBUF_READ pReadData, CString& strLine);
public:
	afx_msg void OnEnChangeEditTargettext();
	afx_msg LRESULT OnListItemChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedChkIncchs();
	afx_msg void OnBnClickedChkInceng();
	afx_msg void OnBnClickedBtnClosetpl();
};
