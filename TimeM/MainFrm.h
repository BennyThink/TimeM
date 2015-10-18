
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "PropertiesWnd.h"
#include "ShortTaskPane.h"
#include "TitlePaneDlg.h"

#define	ID_TIMER_BACKUP	0x1001

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
	
public:
	CMainFrame();

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	void PerformSettings();

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CPropertiesWnd*		GetPropertyWnd(){return &m_wndProperties;};
	CTitlePaneDlg*		GetTitlePane(){return &m_wndTitlePane;};
protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar			m_wndMenuBar;
//	CMFCToolBar			m_wndToolBar;
	CMFCStatusBar		m_wndStatusBar;
	CMFCToolBarImages	m_UserImages;
	CPropertiesWnd		m_wndProperties;
	CShortTaskPane		m_wndTaskPane;
	CTitlePaneDlg		m_wndTitlePane;

// ���ɵ���Ϣӳ�亯��
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnToolbarCustomEnd(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg LRESULT OnSyncVideoTitle(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditDoReplace(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnViewOutputwnd();
	afx_msg void OnUpdateViewOutputwnd(CCmdUI *pCmdUI);
	afx_msg void OnViewPropertieswnd();
	afx_msg void OnUpdateViewPropertieswnd(CCmdUI *pCmdUI);
	afx_msg void OnViewTaskpane();
	afx_msg void OnUpdateViewTaskpane(CCmdUI *pCmdUI);
	afx_msg void OnSubTitleWeb(UINT nID);
};


