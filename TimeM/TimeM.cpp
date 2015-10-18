
// TimeM.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "TimeM.h"
#include "MainFrm.h"

#include "TimeMDoc.h"
#include "ChildFrm.h"
#include "TitleLView.h"
#include "MLListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTimeMApp

BEGIN_MESSAGE_MAP(CTimeMApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CTimeMApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CTimeMApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CTimeMApp::OnFileOpen)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, &CTimeMApp::OnOpenRecentFile)
END_MESSAGE_MAP()


// CTimeMApp 构造

CTimeMApp::CTimeMApp()
{
	m_bHiColorIcons = TRUE;

	m_bInProcess = FALSE;
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CTimeMApp 对象

CTimeMApp theApp;
CComModule	_Module;

// CTimeMApp 初始化

BOOL CTimeMApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	_pAtlModule = &_Module;
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("YYeTs"));
	LoadStdProfileSettings(8);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
//	afxGlobalData.m_bUseSystemFont = TRUE;
//	afxGlobalData.UpdateFonts();
	UpdateGlobalFonts(1);

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_TimeMTYPE,
		RUNTIME_CLASS(CTimeMDoc),
		RUNTIME_CLASS(CChildFrame),  // 自定义 MDI 子框架
		RUNTIME_CLASS(CTitleLView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();

	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	m_bInProcess = TRUE;
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_bInProcess = FALSE;

	// 主窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CTimeMApp::ExitInstance()
{
	int nRet = CWinAppEx::ExitInstance();
	SaveStdProfileSettings();
	return nRet;
}

void CTimeMApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();
}

void CTimeMApp::LoadTitleSettings()
{
	TCHAR	szBakPath[MAX_PATH];
	GetModuleFileName(NULL, szBakPath, MAX_PATH);
	PathRemoveFileSpec(szBakPath);
	StringCbCat(szBakPath, MAX_PATH, _T("\\UserData"));

	m_Params.SetMaxBak(GetProfileInt(_T("Settings"), _T("MaxBackup"), 3));
	m_Params.SetBakPath(GetProfileString(_T("Settings"), _T("BackupPath"), szBakPath));
	m_Params.SetStartDelay(GetProfileInt(_T("Settings"), _T("StartDelay"), 0));
	m_Params.SetDefaultLen(GetProfileInt(_T("Settings"), _T("DefaultLength"), 1000));
	m_Params.SetAutoSave(GetProfileInt( _T("Settings"), _T("IsAutoSave"), TRUE));
	m_Params.SetZipToolsPath(GetProfileString( _T("Settings"), _T("ZipToolPath")));
}

void CTimeMApp::LoadStdProfileSettings(UINT nMaxMRU)
{
	LoadTitleSettings();
	CWinApp::LoadStdProfileSettings(nMaxMRU);
}

void CTimeMApp::UpdateGlobalFonts(int nStep)
{
	LOGFONT lf;
	if (afxGlobalData.fontRegular.GetSafeHandle() != NULL)
	{
		afxGlobalData.fontRegular.GetLogFont(&lf);
		lf.lfHeight += -nStep;
		afxGlobalData.fontRegular.DeleteObject();
		afxGlobalData.fontRegular.CreateFontIndirect(&lf);
	}

	if (afxGlobalData.fontTooltip.GetSafeHandle() != NULL)
	{
		afxGlobalData.fontTooltip.GetLogFont(&lf);
		lf.lfHeight += -nStep;
		afxGlobalData.fontTooltip.DeleteObject();
		afxGlobalData.fontTooltip.CreateFontIndirect(&lf);
	}

	if (afxGlobalData.fontBold.GetSafeHandle() != NULL)
	{
		afxGlobalData.fontBold.GetLogFont(&lf);
		lf.lfHeight += -nStep;
		afxGlobalData.fontBold.DeleteObject();
		afxGlobalData.fontBold.CreateFontIndirect(&lf);
	}

	if (afxGlobalData.fontDefaultGUIBold.GetSafeHandle() != NULL)
	{
		afxGlobalData.fontDefaultGUIBold.GetLogFont(&lf);
		lf.lfHeight += -nStep;
		afxGlobalData.fontDefaultGUIBold.DeleteObject();
		afxGlobalData.fontDefaultGUIBold.CreateFontIndirect(&lf);
	}

	if (afxGlobalData.fontUnderline.GetSafeHandle() != NULL)
	{
		afxGlobalData.fontUnderline.GetLogFont(&lf);
		lf.lfHeight += -nStep;
		afxGlobalData.fontUnderline.DeleteObject();
		afxGlobalData.fontUnderline.CreateFontIndirect(&lf);
	}

	if (afxGlobalData.fontDefaultGUIUnderline.GetSafeHandle() != NULL)
	{
		afxGlobalData.fontDefaultGUIUnderline.GetLogFont(&lf);
		lf.lfHeight += -nStep;
		afxGlobalData.fontDefaultGUIUnderline.DeleteObject();
		afxGlobalData.fontDefaultGUIUnderline.CreateFontIndirect(&lf);
	}

	if (afxGlobalData.fontVert.GetSafeHandle() != NULL)
	{
		afxGlobalData.fontVert.GetLogFont(&lf);
		lf.lfHeight += -nStep;
		afxGlobalData.fontVert.DeleteObject();
		afxGlobalData.fontVert.CreateFontIndirect(&lf);
	}

	if (afxGlobalData.fontVertCaption.GetSafeHandle() != NULL)
	{
		afxGlobalData.fontVertCaption.GetLogFont(&lf);
		lf.lfHeight += -nStep;
		afxGlobalData.fontVertCaption.DeleteObject();
		afxGlobalData.fontVertCaption.CreateFontIndirect(&lf);
	}

	if (afxGlobalData.fontMarlett.GetSafeHandle() != NULL)
	{
		afxGlobalData.fontMarlett.GetLogFont(&lf);
		lf.lfHeight += -nStep;
		afxGlobalData.fontMarlett.DeleteObject();
		afxGlobalData.fontMarlett.CreateFontIndirect(&lf);
	}

	if (afxGlobalData.fontSmall.GetSafeHandle() != NULL)
	{
		afxGlobalData.fontSmall.GetLogFont(&lf);
		lf.lfHeight += -nStep;
		afxGlobalData.fontSmall.DeleteObject();
		afxGlobalData.fontSmall.CreateFontIndirect(&lf);
	}
}

// CTimeMApp customization load/save methods

void CTimeMApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CTimeMApp::LoadCustomState()
{
}

void CTimeMApp::SaveCustomState()
{
}

BOOL CTimeMApp::IsFileVideo(LPCTSTR lpszFile)
{
	LPCTSTR lpszExt = PathFindExtension(lpszFile);
// 	TCHAR szFilter[] = _T("常用媒体格式(*.AVI;*.MKV;*.TS;*.MP4;*.RMVB;*.MPG;*.MPEG)|*.AVI;*.MKV;*.TS;*.MP4;*.RMVB;*.MPG;*.MPEG|All Files (*.*)|*.*||");
	if(StrCmpI(lpszExt, _T(".avi")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".wmv")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".wmp")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".wm")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".asf")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".rm")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".ram")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".rmvb")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".mpg")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".mpeg")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".mpe")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".dat")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".mp4")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".mkv")) == 0)
		return TRUE;
	else if(StrCmpI(lpszExt, _T(".ts")) == 0)
		return TRUE;
	return FALSE;
}

CDocument* CTimeMApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	if(IsFileVideo(lpszFileName))
	{
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		if((NULL != pMain)
			&&(pMain->IsKindOf(RUNTIME_CLASS(CMainFrame))))
		{
			CPropertiesWnd* pVideo = pMain->GetPropertyWnd();
			if(NULL != pVideo)
				pVideo->VideoFileOpen(lpszFileName);
		}
		return NULL;
	}

	if(SaveAllModified())
	{
		CloseAllDocuments(FALSE);
		return CWinAppEx::OpenDocumentFile(lpszFileName);
	}
	return NULL;
}

// CTimeMApp 消息处理程序

void CTimeMApp::OnFileNew()
{
	if(SaveAllModified())
	{
		CloseAllDocuments(FALSE);
		CWinAppEx::OnFileNew();
	}
}

void CTimeMApp::OnFileOpen()
{
	CWinAppEx::OnFileOpen();
}

BOOL CTimeMApp::OnOpenRecentFile(UINT nID)
{
	return CWinAppEx::OnOpenRecentFile(nID);
}
