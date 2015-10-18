#pragma once

#include "CheckListCtrl.h"

// CReplBatchDlg
typedef struct tagReplaceUnit
{
//	DWORD	dwFlag;
	CString strToFind;
	CString strReplTo;
}REPLACE_UNIT, *PREPLACE_UNIT;

typedef pair<UINT, REPLACE_UNIT> REPL_PAIR;
typedef multimap<UINT, REPLACE_UNIT> MAP_REPLUNITS;
typedef multimap<UINT, REPLACE_UNIT>::const_reverse_iterator RITER_UNIT;

class CReplBatchDlg : public CPaneDialog
{
	DECLARE_DYNAMIC(CReplBatchDlg)

public:
	CReplBatchDlg();
	virtual ~CReplBatchDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

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
	MAP_REPLUNITS	m_mapUnits;
public:
	afx_msg void OnBnClickedChkUsebatch();
	afx_msg void OnBnClickedBtnAdditem();
	afx_msg void OnBnClickedBtnRemove();
	afx_msg void OnBnClickedBtnLoadtpl();
	afx_msg void OnBnClickedBtnSavetpl();
protected:
	void UpdateCtrlStates();
	BOOL DoPromptOpenTpl();
	BOOL DoPromptSave();
	BOOL DoPromptSaveTpl();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnChangeEditTargettext();
	afx_msg LRESULT OnListItemChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedChkIncchs();
	afx_msg void OnBnClickedChkInceng();
	afx_msg void OnBnClickedBtnClosetpl();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
};


