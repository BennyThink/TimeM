
// TimeM.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CTimeMApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CTimeMApp::OnFileOpen)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, &CTimeMApp::OnOpenRecentFile)
END_MESSAGE_MAP()


// CTimeMApp ����

CTimeMApp::CTimeMApp()
{
	m_bHiColorIcons = TRUE;

	m_bInProcess = FALSE;
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CTimeMApp ����

CTimeMApp theApp;
CComModule	_Module;

// CTimeMApp ��ʼ��

BOOL CTimeMApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	_pAtlModule = &_Module;
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("YYeTs"));
	LoadStdProfileSettings(8);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

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

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_TimeMTYPE,
		RUNTIME_CLASS(CTimeMDoc),
		RUNTIME_CLASS(CChildFrame),  // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CTitleLView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������
	// ������/��
	m_pMainWnd->DragAcceptFiles();

	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	m_bInProcess = TRUE;
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_bInProcess = FALSE;

	// �������ѳ�ʼ���������ʾ����������и���
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
// 	TCHAR szFilter[] = _T("����ý���ʽ(*.AVI;*.MKV;*.TS;*.MP4;*.RMVB;*.MPG;*.MPEG)|*.AVI;*.MKV;*.TS;*.MP4;*.RMVB;*.MPG;*.MPEG|All Files (*.*)|*.*||");
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

// CTimeMApp ��Ϣ�������

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
