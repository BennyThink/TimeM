
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "TimeM.h"

#include "TitleLView.h"
#include "MainFrm.h"
#include "MultiReplaceDlg.h"
#include "TitleOptions.h"
#include "SaveOptionPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
extern UINT g_nMsg_SyncVideoTitle;

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_OPTION_SETTINGS, &CMainFrame::OnViewCustomize)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_REGISTERED_MESSAGE(g_nMsg_SyncVideoTitle, OnSyncVideoTitle)
	ON_MESSAGE(MSG_TM_DOREPLACE, OnEditDoReplace)
	ON_REGISTERED_MESSAGE(AFX_WM_CUSTOMIZETOOLBAR, &CMainFrame::OnToolbarCustomEnd)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_OUTPUTWND, &CMainFrame::OnViewOutputwnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUTWND, &CMainFrame::OnUpdateViewOutputwnd)
	ON_COMMAND(ID_VIEW_PROPERTIESWND, &CMainFrame::OnViewPropertieswnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIESWND, &CMainFrame::OnUpdateViewPropertieswnd)
	ON_COMMAND(ID_VIEW_TASKPANE, &CMainFrame::OnViewTaskpane)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TASKPANE, &CMainFrame::OnUpdateViewTaskpane)
	ON_COMMAND_RANGE(ID_SUBTITLE_REL1, ID_HELP_LESSON, &CMainFrame::OnSubTitleWeb)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2003);
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::PerformSettings()
{
	KillTimer(ID_TIMER_BACKUP);
	theApp.LoadTitleSettings();
	UINT nTimeBak = 60*1000*AfxGetApp()->GetProfileInt(_T("Settings"), _T("BackupTime"), 10);
	BOOL bAutoBak = AfxGetApp()->GetProfileInt(_T("Settings"), _T("IsAutoBackup"), TRUE);
	if(bAutoBak)
	{
		SetTimer(ID_TIMER_BACKUP, nTimeBak, NULL);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = TRUE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndMenuBar.SetRecentlyUsedMenus(FALSE);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);


//	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
//		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
//	{
//		TRACE0("未能创建工具栏\n");
//		return -1;      // fail to create
//	}

//	CString strToolBarName;
//	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
//	ASSERT(bNameValid);
//	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
//	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
//	DockPane(&m_wndToolBar);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndTaskPane.EnableDocking(CBRS_ALIGN_LEFT);
	DockPane(&m_wndTaskPane);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);
	m_wndTitlePane.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndTitlePane);
	m_wndTitlePane.DockToWindow(&m_wndProperties, CBRS_ALIGN_RIGHT);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();
/*
	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 15), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}
*/
	PerformSettings();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	// Create Task Pane
	CString strTaskWnd;
	bNameValid = strTaskWnd.LoadString(IDS_TASKLIST_WND);
	ASSERT(bNameValid);
	if (!m_wndTaskPane.Create(strTaskWnd, this, CRect(0, 0, 160, 400), TRUE, ID_VIEW_TASKPANE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	// Create properties window
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 400, 400), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	// Create title detail edit window
	CString strTitlePane;
	bNameValid = strTitlePane.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndTitlePane.Create (strTitlePane, this, TRUE, 
							 MAKEINTRESOURCE (IDD_TITLE_FORM), 
							 WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI, 
							 ID_VIEW_OUTPUTWND))
	{
		TRACE0("Failed to create Dialog Bar\n");
		return FALSE; // failed to create
	}

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CList <CRuntimeClass*, CRuntimeClass*> lstCustomPages;
	lstCustomPages.AddTail (RUNTIME_CLASS (CTitleOptionsPage));
	lstCustomPages.AddTail (RUNTIME_CLASS (CSaveOptionPage));
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog (this,
		TRUE /* Automatic menus scaning */, 
		AFX_CUSTOMIZE_MENU_SHADOWS | AFX_CUSTOMIZE_TEXT_LABELS | 
		AFX_CUSTOMIZE_MENU_ANIMATIONS, // default parameters
		&lstCustomPages); // pointer to the list of runtime classes of the custom property pages

	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();

}

LRESULT CMainFrame::OnToolbarCustomEnd(WPARAM wp, LPARAM lp)
{
	if(FALSE == wp)
	{
		PerformSettings();
	}
	return 0;
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

LRESULT CMainFrame::OnSyncVideoTitle(WPARAM wParam, LPARAM lParam)
{
	POSITION posDocTmpl = AfxGetApp()->GetFirstDocTemplatePosition();
	while(posDocTmpl)
	{
		CDocTemplate* pDocTmpl = AfxGetApp()->GetNextDocTemplate(posDocTmpl);
		POSITION posDoc = pDocTmpl->GetFirstDocPosition();
		while(posDoc)
		{
			CTimeMDoc* pDoc = (CTimeMDoc*)pDocTmpl->GetNextDoc(posDoc);
			if(NULL != pDoc)
				pDoc->SyncVideoTitle();
		}
	}
	return 0;
}

LRESULT CMainFrame::OnEditDoReplace(WPARAM wParam, LPARAM lParam)
{
	POSITION posDocTmpl = AfxGetApp()->GetFirstDocTemplatePosition();
	while(posDocTmpl)
	{
		CDocTemplate* pDocTmpl = AfxGetApp()->GetNextDocTemplate(posDocTmpl);
		POSITION posDoc = pDocTmpl->GetFirstDocPosition();
		while(posDoc)
		{
			CTimeMDoc* pDoc = (CTimeMDoc*)pDocTmpl->GetNextDoc(posDoc);
			if(NULL != pDoc)
				pDoc->DoReplace();
		}
	}
	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(ID_TIMER_BACKUP == nIDEvent)
	{
		POSITION posDocTmpl = AfxGetApp()->GetFirstDocTemplatePosition();
		while(posDocTmpl)
		{
			CDocTemplate* pDocTmpl = AfxGetApp()->GetNextDocTemplate(posDocTmpl);
			POSITION posDoc = pDocTmpl->GetFirstDocPosition();
			while(posDoc)
			{
				CTimeMDoc* pDoc = (CTimeMDoc*)pDocTmpl->GetNextDoc(posDoc);
				if(NULL != pDoc)
					pDoc->DoBackup();
			}
		}
	}

	CMDIFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::OnViewOutputwnd()
{
	// TODO: Add your command handler code here
	m_wndTitlePane.ShowPane(!m_wndTitlePane.IsPaneVisible(), FALSE, FALSE);
}

void CMainFrame::OnUpdateViewOutputwnd(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndTitlePane.IsPaneVisible());
}

void CMainFrame::OnViewPropertieswnd()
{
	// TODO: Add your command handler code here
	m_wndProperties.ShowPane(!m_wndProperties.IsPaneVisible(), FALSE, FALSE);
}

void CMainFrame::OnUpdateViewPropertieswnd(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndProperties.IsPaneVisible());
}

void CMainFrame::OnViewTaskpane()
{
	// TODO: Add your command handler code here
	m_wndTaskPane.ShowPane(!m_wndTaskPane.IsPaneVisible(), FALSE, FALSE);
}

void CMainFrame::OnUpdateViewTaskpane(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndTaskPane.IsPaneVisible());
}

void CMainFrame::OnSubTitleWeb(UINT nCmdID)
{
	CString strUrl;
	strUrl.LoadString(nCmdID);
	if(!strUrl.IsEmpty())
		::ShellExecute(NULL, NULL, _T("iexplore"), strUrl, NULL, SW_SHOW);
}

