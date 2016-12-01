// TimeMDoc.cpp : CTimeMDoc 类的实现
//

#include "stdafx.h"
#include "TimeM.h"

#include "TitleHelper.h"
#include "TitleFile.h"
#include "ActionHelper.h"

#include "TimeMDoc.h"
#include "ImportOptionsDlg.h"
#include "InsertCountDlg.h"
#include "TitleLView.h"
#include "InputTimeOffsetDlg.h"
#include "InputExtraTimeDlg.h"

#include "AnySplitTTDlg.h"

#include "CombineTitleByTimeDlg.h"
#include "CombinTitleTransDlg.h"
#include "CompareTitleDlg.h"

#include "SSASettings.h"
#include "MinIntervalSettingDlg.h"
#include "InsertIntervalDlg.h"
#include "TitleFilterDlg.h"

#include "CPSelDlg.h"
#include "SplitNumDlg.h"
#include "PropertiesWnd.h"
#include "TitlePaneDlg.h"
#include "MainFrm.h"
#include "ObjIdl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT g_nMsg_SyncVideoTitle = ::RegisterWindowMessage(_T("Synchronize Title of Video"));

DWORD WINAPI PlaySyncThd(LPVOID lpParam);

// CTimeMDoc

IMPLEMENT_DYNCREATE(CTimeMDoc, CDocument)

BEGIN_MESSAGE_MAP(CTimeMDoc, CDocument)
	ON_COMMAND(ID_TITLE_INSERT, &CTimeMDoc::OnTitleInsert)
	ON_COMMAND(ID_FILE_IMPORT, &CTimeMDoc::OnFileImport)
	ON_COMMAND(ID_FILE_APPEND, &CTimeMDoc::OnFileAppend)
	ON_COMMAND(ID_TITLE_MULTIINSERT, &CTimeMDoc::OnTitleMultiinsert)

	ON_COMMAND(ID_TITLE_EXTEND, &CTimeMDoc::OnTitleExtend)
	ON_COMMAND(ID_TITLE_DELETE, &CTimeMDoc::OnTitleDelete)
	ON_COMMAND(ID_TITLE_EXCHANGE, &CTimeMDoc::OnTitleExchange)
	ON_COMMAND(ID_TITLE_BREAKBYUSER, &CTimeMDoc::OnTitleBreakbyuser)
	ON_COMMAND(ID_TITLE_ZEROTIME, &CTimeMDoc::OnTitleZerotime)
	ON_COMMAND(ID_TITLE_EMPTYCONTENT, &CTimeMDoc::OnTitleEmptycontent)
	ON_COMMAND(ID_TITLE_MOVETIME, &CTimeMDoc::OnTitleMovetime)

	ON_COMMAND(ID_TITLE_CBONELINE, &CTimeMDoc::OnTitleCboneline)
	ON_COMMAND(ID_TITLE_CBMULTILINE, &CTimeMDoc::OnTitleCbmultiline)

	ON_COMMAND(ID_TITLE_SNAPLINE, &CTimeMDoc::OnTitleSnapline)
	ON_COMMAND(ID_TITLE_SNAPAFTER, &CTimeMDoc::OnTitleSnapafter)
	ON_COMMAND(ID_TITLE_SNAPALL, &CTimeMDoc::OnTitleSnapall)
	ON_COMMAND(ID_TITLE_SNAPLINEEND, &CTimeMDoc::OnTitleSnaplineend)
	ON_COMMAND(ID_TITLE_SNAPLINESTART, &CTimeMDoc::OnTitleSnaplinestart)
	ON_COMMAND(ID_TITLE_SNAPPREVMARK, &CTimeMDoc::OnTitleSnapprevmark)
	ON_COMMAND(ID_TITLE_SNAPBETWEENTWOMARK, &CTimeMDoc::OnTitleSnapbetweentwomark)

	ON_COMMAND(ID_TITLE_TO25FPS, &CTimeMDoc::OnTitleTo25fps)
	ON_COMMAND(ID_TITLE_FROM25FPS, &CTimeMDoc::OnTitleFrom25fps)

	ON_COMMAND(ID_VIDEO_OPEN, &CTimeMDoc::OnVideoFileOpen)
	ON_COMMAND(ID_VIDEO_PLAY_PAUSE, &CTimeMDoc::OnVideoPlayPause)
	ON_COMMAND(ID_VIDEO_STOP, &CTimeMDoc::OnVideoStop)
	ON_COMMAND(ID_VIDEO_STEPONE, &CTimeMDoc::OnVideoStepone)
	ON_COMMAND(ID_VIDEO_FORWARD1S, &CTimeMDoc::OnVideoForward1s)
	ON_COMMAND(ID_VIDEO_BACK1S, &CTimeMDoc::OnVideoBack1s)
	ON_COMMAND(ID_VIDEO_FORWARD5S, &CTimeMDoc::OnVideoForward5s)
	ON_COMMAND(ID_VIDEO_BACK5S, &CTimeMDoc::OnVideoBack5s)

	ON_COMMAND(ID_TOOL_PREPROC, &CTimeMDoc::OnToolPreproc)
	ON_COMMAND(ID_VIDEO_SYNCTITLE, &CTimeMDoc::OnVideoSynctitle)
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CTimeMDoc::OnEditSelectAll)
	ON_COMMAND(ID_EDIT_COPY, &CTimeMDoc::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CTimeMDoc::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CTimeMDoc::OnEditPaste)
	ON_COMMAND(ID_EDIT_FIND, &CTimeMDoc::OnEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, &CTimeMDoc::OnEditReplace)
	ON_COMMAND(ID_EDIT_FINDNEXT, &CTimeMDoc::OnEditFindnext)
	ON_COMMAND(ID_TOOL_CMBYTIME, &CTimeMDoc::OnToolCmbytime)
	ON_COMMAND(ID_EDIT_UNDO, &CTimeMDoc::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CTimeMDoc::OnEditRedo)

	ON_UPDATE_COMMAND_UI_RANGE(ID_TITLE_CBONELINE, ID_TITLE_CBMULTILINE, &CTimeMDoc::OnTitleCBUpdateCmdUI)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TITLE_EXTEND, ID_TITLE_MOVETIME, &CTimeMDoc::OnTitleUpdateCmdUIRange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TITLE_SNAPLINE, ID_TITLE_SNAPBETWEENTWOMARK, &CTimeMDoc::OnTitleSnapUpdateCmdUIRange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_DEFAULT_CHARSET, ID_GB2312_CHARSET, &CTimeMDoc::OnCharsetDispUpdateUI)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_SYNCTITLE, &CTimeMDoc::OnUpdateVideoSynctitle)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDNEXT, &CTimeMDoc::OnUpdateEditFindnext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CTimeMDoc::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CTimeMDoc::OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_TOOL_CHECK, &CTimeMDoc::OnUpdateToolCheck)
	ON_UPDATE_COMMAND_UI(ID_TITLE_AUTOSNAP, &CTimeMDoc::OnUpdateTitleAutosnap)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT, &CTimeMDoc::OnUpdateFileExport)
	ON_UPDATE_COMMAND_UI(ID_TITLE_PREVLINE, &CTimeMDoc::OnUpdateTitlePrevline)
	ON_UPDATE_COMMAND_UI(ID_TITLE_NEXTLINE, &CTimeMDoc::OnUpdateTitleNextline)
	ON_UPDATE_COMMAND_UI(ID_OPTION_AUTOSAVE, &CTimeMDoc::OnUpdateOptionAutosave)
	ON_COMMAND(ID_EDIT_SORTSTART, &CTimeMDoc::OnEditSortstart)
	ON_COMMAND(ID_EDIT_SORTEND, &CTimeMDoc::OnEditSortend)
	ON_COMMAND(ID_EDIT_SORTCONTENT, &CTimeMDoc::OnEditSortcontent)
	ON_COMMAND(ID_EDIT_SORTLEN, &CTimeMDoc::OnEditSortlen)
	ON_COMMAND(ID_EDIT_MARK, &CTimeMDoc::OnEditMark)
	ON_COMMAND(ID_EDIT_GOTOMARK, &CTimeMDoc::OnEditGotomark)
	ON_COMMAND(ID_TOOL_CMBYUSER, &CTimeMDoc::OnToolCmbyuser)
	ON_COMMAND(ID_TOOL_COMPARE, &CTimeMDoc::OnToolCompare)
	ON_COMMAND(ID_TOOL_CHECK, &CTimeMDoc::OnToolCheck)
	ON_COMMAND(ID_FILE_EXPORT, &CTimeMDoc::OnFileExport)
	ON_COMMAND(ID_TITLE_AUTOSNAP, &CTimeMDoc::OnTitleAutosnap)
	ON_COMMAND(ID_TITLE_SNAPNEXT, &CTimeMDoc::OnTitleSnapnext)
	ON_COMMAND(ID_DEFAULT_CHARSET, &CTimeMDoc::OnDefaultCharset)
	ON_COMMAND(ID_BIG5_CHARSET, &CTimeMDoc::OnBig5Charset)
	ON_COMMAND(ID_GB2312_CHARSET, &CTimeMDoc::OnGb2312Charset)
	ON_COMMAND(ID_FILE_SAVE_AS, &CTimeMDoc::OnFileSaveAs)
	ON_COMMAND(ID_OPTIONS_ASSSETTINGS, &CTimeMDoc::OnOptionsAsssettings)
	ON_COMMAND(ID_TOOL_ADDAUTHOR, &CTimeMDoc::OnToolAddauthor)
	ON_COMMAND(ID_TITLE_PREVLINE, &CTimeMDoc::OnTitlePrevline)
	ON_COMMAND(ID_TITLE_NEXTLINE, &CTimeMDoc::OnTitleNextline)
	ON_COMMAND(ID_EDIT_GOTOPREVMARK, &CTimeMDoc::OnEditGotoprevmark)
	ON_COMMAND(ID_TITLE_DELBYCOND, &CTimeMDoc::OnTitleDelbycond)
	ON_COMMAND(ID_TITLE_SINGLELINETITLE, &CTimeMDoc::OnTitleSinglelinetitle)
	ON_COMMAND(ID_FILE_SPLITTO, &CTimeMDoc::OnFileSplitto)
	ON_COMMAND(ID_OPTION_AUTOSAVE, &CTimeMDoc::OnOptionAutosave)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, &CTimeMDoc::OnUpdateEditReplace)
	//ON_COMMAND(ID_FILE_EXPORTFU, &CTimeMDoc::OnFileExportfu)
END_MESSAGE_MAP()


// CTimeMDoc 构造/析构

CTimeMDoc::CTimeMDoc()
{
	SetCurrentPos(-1);
	m_bUseSync = FALSE;
	m_bThdBreak = FALSE;
	m_bSyncRunning = FALSE;
	m_hSyncThd = NULL;
	m_nSortCol		= 0;
	m_bAscending	= TRUE;
	m_dlgCheckValid.m_pDoc = this;
	m_bAutoSnap	 = FALSE;
	m_dwCharsetForDisp = ANSI_CHARSET;
	m_bUnicode = FALSE;
	m_PlayerWnd = NULL;
	m_DetailWnd = NULL;
	m_bBakModified = FALSE;
	m_nSaveState = 0;
}

CTimeMDoc::~CTimeMDoc()
{
	m_bThdBreak = TRUE;
	MSG msg;
	while(m_bSyncRunning)
	{
		Sleep(100);
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	}
}

CTitleLView* CTimeMDoc::GetTitleView()
{
	CView* pView = NULL;
	POSITION posView = GetFirstViewPosition();
	while(posView != NULL)
	{
		pView = GetNextView(posView);
		if(pView->IsKindOf(RUNTIME_CLASS(CTitleLView)))
			return (CTitleLView*)pView;
	}
	return NULL;
}

CPropertiesWnd* CTimeMDoc::GetPlayerWnd()
{
	if(NULL != m_PlayerWnd)
		return m_PlayerWnd;

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if((pMain != NULL)
		&&(pMain->IsKindOf(RUNTIME_CLASS(CMainFrame))))
	{
		m_PlayerWnd = pMain->GetPropertyWnd();
	}

	return m_PlayerWnd;
}

CTitlePaneDlg* CTimeMDoc::GetDetailWnd()
{
	if(NULL != m_DetailWnd)
		return m_DetailWnd;

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if((pMain != NULL)
		&&(pMain->IsKindOf(RUNTIME_CLASS(CMainFrame))))
	{
		m_DetailWnd = pMain->GetTitlePane();
	}

	return m_DetailWnd;
}

// CTimeMDoc 序列化

void CTimeMDoc::Serialize(CArchive& ar)
{
	CFile* pFile = ar.GetFile();
	if (ar.IsStoring())
	{
		CString strExt = PathFindExtension(pFile->GetFileName());
		DWORD dwFmt = FMTFLAG_SRT;
		CString strFmtHdr;
		if(strExt.CompareNoCase(_T(".ssa")) == 0)
		{
			dwFmt = FMTFLAG_SSA;
			strFmtHdr = m_strFmtHeader.IsEmpty()?CTitleParams::GetDefSSAHeader():m_strFmtHeader;
		}
		else if(strExt.CompareNoCase(_T(".ass")) == 0)
		{
			dwFmt = FMTFLAG_ASS;
			strFmtHdr = m_strFmtHeader.IsEmpty()?CTitleParams::GetDefASSHeader():m_strFmtHeader;
		}
		if(CTitleFile::WriteTitleFile(pFile, m_Action.GetVtTitle(), strFmtHdr, m_strPreCode, m_strPostCode, dwFmt, m_bUnicode))
			DeleteFile(GetAutoSaveFile(GetPathName()));
	}
	else
	{
		CString strAutoFile = GetAutoSaveFile(pFile->GetFileName());
		if(PathFileExists(strAutoFile))
		{
			if(IDYES == AfxMessageBox(IDS_FOUNDAUTOSAVE, MB_YESNO))
			{
				CFile fileAuto;
				if(fileAuto.Open(strAutoFile, CFile::modeRead))
				{
					if(CTitleFile::ReadTitleFile(&fileAuto, m_Action.GetVtTitle(), m_strFmtHeader, m_bUnicode))
						return;
					AfxMessageBox(IDS_AUTOSAVEREADERROR);
				}
			}
		}
		CTitleFile::ReadTitleFile(pFile, m_Action.GetVtTitle(), m_strFmtHeader, m_bUnicode);
	}
}

// CTimeMDoc 诊断

#ifdef _DEBUG
void CTimeMDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTimeMDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CTimeMDoc::UpdateRefWin(CWnd* pWnd)
{
	CTitlePaneDlg* pTitlePane = GetDetailWnd();
	if(pWnd == pTitlePane)
	{
		UpdateAllViews(NULL);
		return;
	}
	UpdateAllViews((CView*)pWnd);
	if(pTitlePane != NULL)
		pTitlePane->UpdateDocData(FALSE);
}

// CTimeMDoc 命令

BOOL CTimeMDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	m_bThdBreak = FALSE;
	m_dwCharsetForDisp = ANSI_CHARSET;
	if(!IsWindow(m_dlgCheckValid.m_hWnd))
		m_dlgCheckValid.Create(CCheckValidDlg::IDD, GetTitleView());//AfxGetMainWnd());
	if(!IsWindow(m_dlgReplace.m_hWnd))
		m_dlgReplace.Create(CMultiReplaceDlg::IDD, GetTitleView());//AfxGetMainWnd());
	if(!theApp.m_bInProcess)
		OnTitleMultiinsert();
	m_hSyncThd = CreateThread(NULL, 0, PlaySyncThd, (LPVOID)this, 0, NULL);
	return TRUE;
}

BOOL CTimeMDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	m_bThdBreak = FALSE;
	m_dwCharsetForDisp = ANSI_CHARSET;
	if(!IsWindow(m_dlgCheckValid.m_hWnd))
		m_dlgCheckValid.Create(CCheckValidDlg::IDD, AfxGetMainWnd());
	if(!IsWindow(m_dlgReplace.m_hWnd))
		m_dlgReplace.Create(CMultiReplaceDlg::IDD, GetTitleView());//AfxGetMainWnd());

	m_hSyncThd = CreateThread(NULL, 0, PlaySyncThd, (LPVOID)this, 0, NULL);
	SetCurrentDirectory(GetPathName());
	return TRUE;
}

void CTimeMDoc::OnCloseDocument()
{
	m_bThdBreak = TRUE;
	MSG msg;
	while(m_bSyncRunning)
	{
		Sleep(100);
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	}
	CDocument::OnCloseDocument();
}

CString CTimeMDoc::GetAutoSaveFile(LPCTSTR lpszPathName)
{
	CString strAutoSaveFile;
	if((NULL == lpszPathName)
		||(0 == lstrlen(lpszPathName)))
	{
		if(!DoFileSave())
			return strAutoSaveFile;
	}

	strAutoSaveFile = lpszPathName;
	strAutoSaveFile += _T(".~a~");
	return strAutoSaveFile;
}

void  CTimeMDoc::SavePlayState()
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if(pPlayer == NULL)
		return;
	m_nSaveState = pPlayer->GetPlayState();
	if(m_nSaveState == wmppsPlaying)
	{
		OnVideoPlayPause();
	}
}

void  CTimeMDoc::RestorePlayState()
{
	if(m_nSaveState == wmppsPlaying)
	{
		OnVideoPlayPause();
	}
}

BOOL CTimeMDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	SavePlayState();

	BOOL bRet = CDocument::OnSaveDocument(lpszPathName);

	RestorePlayState();
	return bRet;
}

void CTimeMDoc::SetModifiedFlagAutoSave(BOOL bModified)
{
	if(bModified)
		SetBakModifiedFlag(bModified);
	if((bModified)&&theApp.m_Params.IsAutoSave())
	{
		CDocument::SetModifiedFlag(bModified);
		WriteToFile(GetAutoSaveFile(GetPathName()));
	}
	else
		CDocument::SetModifiedFlag(bModified);
}

BOOL CTimeMDoc::SaveTitleItem(int nPos, const CString& strStart, const CString& strEnd, const CString& strContent)
{
	PTITLE_UNIT pUnit = m_Action.GetItem(nPos);
	if(pUnit == NULL)
		return FALSE;

	int nStart	= CTitleHelper::GetTimeValue(strStart);
	int nEnd	= CTitleHelper::GetTimeValue(strEnd);
	if((pUnit->nStart == nStart)
		&&(pUnit->nEnd == nEnd)
		&&(pUnit->content.Compare(strContent) == 0))
		return FALSE;

	pUnit->nStart	= nStart;
	pUnit->nEnd		= nEnd;
	pUnit->content	= strContent;
	return TRUE;
}

BOOL CTimeMDoc::SaveCurrentTitleItem(const CString& strStart, const CString& strEnd, const CString& strContent)
{
	WriteLogLine(_T("CTimeMDoc::SaveCurrentTitleItem(\"%s\", \"%s\", \"%s\")\r\n"), 
		strStart, strEnd, strContent);

	CSingleLock singleLock(&m_CriticalSect);
	singleLock.Lock();
	m_Action.BeginRecordAction();
	if(SaveTitleItem(GetCurrentPos(), strStart, strEnd, strContent))
		SetModifiedFlagAutoSave();
	else
		m_Action.PopLastRecord();
	m_Action.EndRecordAction();
	singleLock.Unlock();
	return TRUE;
}

const PTITLE_UNIT CTimeMDoc::GetTitleItem(int nPos)
{
	return m_Action.GetItem(nPos, FALSE);
}

int CTimeMDoc::GetTitleCount()
{
	return m_Action.GetItemCount();
}

int CTimeMDoc::GetCurrentPos()
{
	return m_nPos;
}

void CTimeMDoc::SetCurrentPos(int nPos)
{
	if(nPos + 1 > m_Action.GetItemCount())
		m_nPos = -1;
	else
		m_nPos = nPos;
}

void  CTimeMDoc::SafeChangePos(int nPos)
{
}

void CTimeMDoc::DeleteContents()
{
	m_Action.DeleteContents();
	SetCurrentPos(-1);
	CDocument::DeleteContents();
}

void CTimeMDoc::InsertTitle(int nPos, const CString& strStart, const CString& strEnd, const CString& strTitle)
{
	PTITLE_UNIT pUnit = new TITLE_UNIT;
	InitializeUnit(pUnit);

	pUnit->nStart	= CTitleHelper::GetTimeValue(strStart);
	pUnit->nEnd		= CTitleHelper::GetTimeValue(strEnd);
	pUnit->content	= strTitle;

	m_Action.BeginRecordAction();
	m_Action.Insert(nPos, pUnit);
	m_Action.EndRecordAction();

	SetModifiedFlagAutoSave();
	UpdateRefWin();
}

void CTimeMDoc::InsertOneTitle(int nPos, LPCTSTR lpszTitle, int nTimeLen)
{
	PTITLE_UNIT pUnit = new TITLE_UNIT;
	InitializeUnit(pUnit);

	if(nPos > 0)
		pUnit->nStart = m_Action.GetItem(nPos - 1, FALSE)->nEnd;
	else
		pUnit->nStart = 0;

	pUnit->nEnd		= pUnit->nStart + nTimeLen;
	pUnit->content	= lpszTitle;
	pUnit->dwFlags	= 0;
	m_Action.Insert(nPos, pUnit);
}

int CTimeMDoc::GetMoveOffset(DWORD& dwFlag)
{
	int nPos = GetCurrentPos();
	if(-1 == nPos)
	{
		AfxMessageBox(IDS_CURPOSERROR);
		return 0;
	}

	CInputTimeOffsetDlg dlgInput;
	dlgInput.SetTitleStart(m_Action.GetItem(nPos)->nStart);
	if(IDOK == dlgInput.DoModal())
	{
		dwFlag = dlgInput.GetFlags();
		return dlgInput.GetOffset();
	}
	return 0;
}

void CTimeMDoc::OnTitleInsert()
{
	int nPos = GetCurrentPos();

	WriteLogLine(_T("CTimeMDoc::OnTitleInsert()\r\n"));
	m_Action.BeginRecordAction();
	if(nPos == -1)
		InsertOneTitle(GetTitleCount(), _T(""), theApp.m_Params.GetDefaultLen());
	else
		InsertOneTitle(nPos + 1, _T(""), theApp.m_Params.GetDefaultLen());
	m_Action.EndRecordAction();

	SetModifiedFlagAutoSave();

	UpdateRefWin();
}

void CTimeMDoc::OnFileImport()
{
	CFile	fileTrans;
	if(OpenTransFile(&fileTrans))
	{
		BOOL bSplitByLine = QuerySplitOption();

		if (!CDocument::OnNewDocument())
			return;

		if(CTitleFile::ReadTransFile(&fileTrans, m_Action.GetVtTitle(), bSplitByLine, m_bUnicode))
			SetModifiedFlag(FALSE);     // nStart off with unmodified
	}
	UpdateRefWin();
}

BOOL CTimeMDoc::OpenTransFile(CFile* pFile)
{
	TCHAR szFilter[] = _T("Text files (*.txt)|*.txt|All Files (*.*)|*.*||");

	CFileDialog dlgImportFile(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFilter);
	if(dlgImportFile.DoModal() == IDOK)
	{
		return pFile->Open(dlgImportFile.GetPathName(), CFile::modeRead);
	}
	return FALSE;
}

BOOL CTimeMDoc::QuerySplitOption()
{
	CImportOptionsDlg dlgOption;
	if(dlgOption.DoModal() == IDOK)
	{
		return dlgOption.GetSingleLineOption();
	}
	return FALSE;
}

void CTimeMDoc::OnFileAppend()
{
	CFile	fileAppend;
	if(OpenAppendFile(&fileAppend))
	{
		WriteLogLine(_T("CTimeMDoc::OnFileAppend()\r\n"));
		m_Action.BeginRecordAction();
		BUF_READ rbData;
		CString strExt = PathFindExtension(fileAppend.GetFileName());
		try
		{
			if(CTitleFile::PrepareRead(&rbData, &fileAppend))
			{
				CTitleFile::FillBuffer(&rbData, &fileAppend);
				rbData.nUnicodeState = CTitleFile::TestUnicodeBuf(rbData.bufFile, rbData.dwBufSize);
				CTitleFile::StepSignature(&rbData);
				CTitleFile::ProcessReverse(&rbData);
				if(strExt.CompareNoCase(_T(".ssa")) == 0)
				{
					rbData.dwFmtFlags = FMTFLAG_SSA;
					UINT nOffset = 0;
					if(rbData.nUnicodeState > 0)
					{

						CTitleFile::GetSSAHdr((LPCTSTR)(rbData.bufFile + rbData.dwBufOffset), nOffset);

						rbData.dwBufOffset += nOffset*sizeof(WCHAR);
					}
					else
					{

						CTitleFile::GetSSAHdrA((LPCSTR)(rbData.bufFile + rbData.dwBufOffset), nOffset);

						rbData.dwBufOffset += nOffset*sizeof(CHAR);
					}
				}
				else if(strExt.CompareNoCase(_T(".ass")) == 0)
				{
					rbData.dwFmtFlags = FMTFLAG_ASS;
					UINT nOffset = 0;
					if(rbData.nUnicodeState > 0)
					{

						CTitleFile::GetSSAHdr((LPCTSTR)(rbData.bufFile + rbData.dwBufOffset), nOffset, TRUE);

						rbData.dwBufOffset += nOffset*sizeof(WCHAR);
					}
					else
					{

						CTitleFile::GetSSAHdrA((LPCSTR)(rbData.bufFile + rbData.dwBufOffset), nOffset, TRUE);

						rbData.dwBufOffset += nOffset*sizeof(CHAR);
					}
				}
				else if(strExt.CompareNoCase(_T(".srt")) == 0)
				{
					rbData.dwFmtFlags = FMTFLAG_SRT;
				}
				else
				{
					rbData.dwFmtFlags = FMTFLAG_UNKNOWN;
				}

				PTITLE_UNIT pUnit;
				while(rbData.dwBufOffset < rbData.dwBufSize)
				{
					pUnit = CTitleFile::StepTitleUnit(&rbData);
					if(pUnit != NULL)
						m_Action.PushBack(pUnit);
				}
			}
		}
		catch(...)
		{
		}
		CTitleFile::ClearReadBuf(&rbData);

		m_Action.EndRecordAction();
		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
}

BOOL CTimeMDoc::OpenAppendFile(CFile* pFile)
{
	TCHAR szFilter[] = _T("字幕文件 (*.srt;*.ass;*.ssa)|*.srt;*.ass;*.ssa|All Files (*.*)|*.*||");

	CFileDialog dlgImportFile(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFilter);
	if(dlgImportFile.DoModal() == IDOK)
	{
		return pFile->Open(dlgImportFile.GetPathName(), CFile::modeRead);
	}
	return FALSE;
}

void CTimeMDoc::OnTitleMultiinsert()
{
	int nPos = GetCurrentPos();

	BOOL bEmpty = TRUE;
	int nInsertCnt = 0;
	BOOL bInitTime = FALSE;
	CString strTimeValue;
	CInsertCountDlg dlgInsert;
	dlgInsert.m_strTimeInit = CTitleHelper::FormatTime(theApp.m_Params.GetDefaultLen());
	if(IDOK == dlgInsert.DoModal())
	{
		bEmpty = dlgInsert.m_bEmpty;
		nInsertCnt = dlgInsert.m_nLineCount;
		bInitTime = dlgInsert.m_bInitTime;
		strTimeValue = dlgInsert.m_strTimeInit;
	}
	if(nInsertCnt > 0)
	{
		CString strTitle;
		WriteLogLine(_T("CTimeMDoc::OnTitleMultiinsert(nInsertCnt %d)\r\n"), nInsertCnt);
		m_Action.BeginRecordAction();
		for(int i = 0; i < nInsertCnt; i ++)
		{
			if(!bEmpty)
				strTitle.Format(IDS_INSERTSEQ, i+1);
			if(nPos == -1)
				InsertOneTitle(GetTitleCount(), strTitle, bInitTime?CTitleHelper::GetTimeValue(strTimeValue):0);
			else
				InsertOneTitle(nPos + i + 1, strTitle, bInitTime?CTitleHelper::GetTimeValue(strTimeValue):0);
		}
		m_Action.EndRecordAction();
		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
}

void CTimeMDoc::OnTitleDelete()
{
	CTitleLView* pView = GetTitleView();
	if(pView == NULL)
		return;

	DWORD dwSelCnt = pView->GetSelectedCount();
	if(dwSelCnt > 0)
	{
		int nPosSave = GetCurrentPos();
		int nSel;
		LISTSEL_MAP mapDel;
		POSITION posSel = pView->GetFirstSelectedItemPosition();
		while(posSel != NULL)
		{
			nSel = pView->GetNextSelectedItem(posSel);
			mapDel.insert(INT_PAIR(nSel, nSel));
		}

		LISTSEL_MAP::reverse_iterator itrRDel;

		WriteLogLine(_T("CTimeMDoc::OnTitleDelete(DeleteCnt %d)\r\n"), mapDel.size());
		m_Action.BeginRecordAction();
		for(itrRDel =  mapDel.rbegin(); itrRDel != mapDel.rend(); itrRDel++)
		{
			OnTitleDelete(itrRDel->first);
		}
		m_Action.EndRecordAction();

		if(nPosSave > m_Action.GetItemCount() - 1)
		{
			SetCurrentPos(m_Action.GetItemCount() - 1);
		}

		pView->NoSelect();
		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
}

void CTimeMDoc::OnTitleDelete(int nSel)
{
	m_Action.Erase(nSel);
}

void CTimeMDoc::ExchangeTitleLine(int nItem)
{
	PTITLE_UNIT pUnit = m_Action.GetItem(nItem);
	if(pUnit != NULL)
	{
		CString strUp;
		CString strDn;
		CTitleHelper::BreakString(pUnit->content, strUp, strDn);
		pUnit->content = strDn;
		pUnit->content += _T("\r\n");
		pUnit->content += strUp;
	}
}

void CTimeMDoc::OnTitleExchange()
{
	CTitleLView* pView = GetTitleView();
	if(pView == NULL)
		return;

	DWORD dwSelCnt = pView->GetSelectedCount();
	if(dwSelCnt > 0)
	{
		int nSel;
		m_Action.BeginRecordAction();
		POSITION posSel = pView->GetFirstSelectedItemPosition();
		while(posSel != NULL)
		{
			nSel = pView->GetNextSelectedItem(posSel);
			ExchangeTitleLine(nSel);
		}
		m_Action.EndRecordAction();
		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
}

void CTimeMDoc::OnTitleBreaktwo()
{
	CTitleLView* pView = GetTitleView();
	if(pView == NULL)
		return;

	DWORD dwSelCnt = pView->GetSelectedCount();
	if(dwSelCnt > 0)
	{
		int nSel;
		LISTSEL_MAP mapDel;
		POSITION posSel = pView->GetFirstSelectedItemPosition();
		while(posSel != NULL)
		{
			nSel = pView->GetNextSelectedItem(posSel);
			mapDel.insert(INT_PAIR(nSel, nSel));
		}

		LISTSEL_MAP::reverse_iterator itrRDel;

		m_Action.BeginRecordAction();
		for(itrRDel =  mapDel.rbegin(); itrRDel != mapDel.rend(); itrRDel++)
		{
			PTITLE_UNIT pUnit = m_Action.GetItem(itrRDel->first);
			PTITLE_UNIT pUnitNew = new TITLE_UNIT;
			InitializeUnit(pUnitNew);
			if(CTitleHelper::BreakUnitByShare(2, pUnit, pUnitNew))
			{
//				InsertPreTrans(pUnitNew);
				m_Action.Insert(itrRDel->first + 1, pUnitNew);
			}
			else
			{
				delete pUnitNew;
			}
//			InsertPreTrans(pUnit);
		}
		m_Action.EndRecordAction();
		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
}

void CTimeMDoc::OnTitleBreakbyuser()
{
	int nPos = GetCurrentPos();
	if(nPos == -1)
		return;

	PTITLE_UNIT pUnit = m_Action.GetItem(nPos, FALSE);
	CAnySplitTTDlg dlgAnySplit;
	dlgAnySplit.InitTitleUnit(pUnit);
	if(IDOK == dlgAnySplit.DoModal())
	{
		const VT_TITLE& vtTitles = dlgAnySplit.GetBreakTitles();
		int nBreakCnt = vtTitles.size();
		m_Action.BeginRecordAction();
		m_Action.Erase(nPos);
		for(int iBreak = 0; iBreak < nBreakCnt; iBreak ++)
		{
			m_Action.Insert(nPos + iBreak, vtTitles[iBreak]);
		}
		m_Action.EndRecordAction();
		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
	return;
}

void CTimeMDoc::OnTitleCboneline()
{
	CTitleLView* pView = GetTitleView();
	if(pView == NULL)
		return;

	DWORD dwSelCnt = pView->GetSelectedCount();
	if(dwSelCnt < 2)
		return;

	int iItem = -1;
	int	iSaveItem = -1;
	POSITION posSel = pView->GetFirstSelectedItemPosition();
	PTITLE_UNIT pUnitStart = NULL;
	PTITLE_UNIT pUnitNext = NULL;
	LISTSEL_MAP mapDel;
	CString strUpLine;
	CString strDnLine;
	m_Action.BeginRecordAction();
	while(posSel)
	{
		iItem = pView->GetNextSelectedItem(posSel);
		if(pUnitStart == NULL)
		{
			pUnitStart = m_Action.GetItem(iItem);
			iSaveItem = iItem;
			CTitleHelper::BreakString(pUnitStart->content, strUpLine, strDnLine);
		}
		else
		{
			pUnitNext = m_Action.GetItem(iItem, FALSE);

			if(pUnitStart->nStart > pUnitNext->nStart)
				pUnitStart->nStart = pUnitNext->nStart;
			if(pUnitStart->nEnd < pUnitNext->nEnd)
				pUnitStart->nEnd = pUnitNext->nEnd;

			CString strFirst, strOthers;

			CTitleHelper::RemoveAssCode(pUnitNext->content);
			CTitleHelper::BreakString(pUnitNext->content, strFirst, strOthers);
			if(!strFirst.IsEmpty())
			{
				strUpLine += _T(" ");
				strUpLine += strFirst;
			}
			if(!strOthers.IsEmpty())
			{
				strDnLine += _T(" ");
				strDnLine += strOthers;
			}
			mapDel.insert(INT_PAIR(iItem, iItem));
		}
//		pView->SetFo
	}

	pUnitStart->content = strUpLine;
	pUnitStart->content += _T("\r\n");
	pUnitStart->content += strDnLine;

	LISTSEL_MAP::reverse_iterator itrRDel;
	for(itrRDel =  mapDel.rbegin(); itrRDel != mapDel.rend(); itrRDel++)
	{
		OnTitleDelete(itrRDel->first);
	}

	m_Action.EndRecordAction();

	pView->NoSelect();
	SetCurrentPos(iSaveItem);
	SetModifiedFlagAutoSave();
	UpdateRefWin();
}

void CTimeMDoc::OnTitleCbmultiline()
{
	CTitleLView* pView = GetTitleView();
	if(pView == NULL)
		return;

	DWORD dwSelCnt = pView->GetSelectedCount();
	if(dwSelCnt < 2)
		return;

	int iItem = -1;
	int iSaveItem = -1;
	POSITION posSel = pView->GetFirstSelectedItemPosition();
	PTITLE_UNIT pUnitStart = NULL;
	PTITLE_UNIT pUnitNext = NULL;
	LISTSEL_MAP mapDel;
	m_Action.BeginRecordAction();
	while(posSel)
	{
		iItem = pView->GetNextSelectedItem(posSel);
		if(pUnitStart == NULL)
		{
			pUnitStart = m_Action.GetItem(iItem);
			iSaveItem = iItem;
		}
		else
		{
			pUnitNext = m_Action.GetItem(iItem, FALSE);

			if(pUnitStart->nStart > pUnitNext->nStart)
				pUnitStart->nStart = pUnitNext->nStart;
			if(pUnitStart->nEnd < pUnitNext->nEnd)
				pUnitStart->nEnd = pUnitNext->nEnd;

			CTitleHelper::RemoveAssCode(pUnitNext->content);
			pUnitStart->content += _T("\r\n");
			pUnitStart->content += pUnitNext->content;
			mapDel.insert(INT_PAIR(iItem, iItem));
		}
	}

	LISTSEL_MAP::reverse_iterator itrRDel;
	for(itrRDel =  mapDel.rbegin(); itrRDel != mapDel.rend(); itrRDel++)
	{
		OnTitleDelete(itrRDel->first);
	}
	m_Action.EndRecordAction();

	pView->NoSelect();
	SetCurrentPos(iSaveItem);
	SetModifiedFlagAutoSave();
	UpdateRefWin();
}

void CTimeMDoc::OnTitleTo25fps()
{
	PTITLE_UNIT pUnit;
	m_Action.BeginRecordAction();
	int nSize = GetTitleCount();
	for(int iTitle = 0; iTitle < nSize; iTitle ++)
	{
		pUnit = m_Action.GetItem(iTitle);
		pUnit->nStart = ((LONGLONG)pUnit->nStart * (LONGLONG)23976)/ 25000;
		pUnit->nEnd = ((LONGLONG)pUnit->nEnd * (LONGLONG)23976)/ 25000;
	}
	m_Action.EndRecordAction();
	SetModifiedFlagAutoSave();
	UpdateRefWin();
}

void CTimeMDoc::OnTitleFrom25fps()
{
	PTITLE_UNIT pUnit;
	m_Action.BeginRecordAction();
	int nSize = GetTitleCount();
	for(int iTitle = 0; iTitle < nSize; iTitle ++)
	{
		pUnit = m_Action.GetItem(iTitle);
		pUnit->nStart = ((LONGLONG)pUnit->nStart * (LONGLONG)25000)/ 23976;
		pUnit->nEnd = ((LONGLONG)pUnit->nEnd * (LONGLONG)25000)/ 23976;
	}
	m_Action.EndRecordAction();
	SetModifiedFlagAutoSave();
	UpdateRefWin();
}

void CTimeMDoc::OnTitleZerotime()
{
	CTitleLView* pView = GetTitleView();
	if(pView == NULL)
		return;

	DWORD dwSelCnt = pView->GetSelectedCount();
	if(dwSelCnt > 0)
	{
		int nSel;
		m_Action.BeginRecordAction();
		POSITION posSel = pView->GetFirstSelectedItemPosition();
		while(posSel != NULL)
		{
			nSel = pView->GetNextSelectedItem(posSel);
			OnTitleZerotime(nSel);
		}
		m_Action.EndRecordAction();
		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
}

void CTimeMDoc::OnTitleZerotime(int nSel)
{
	PTITLE_UNIT pUnit = m_Action.GetItem(nSel);
	pUnit->nStart	= 0;
	pUnit->nEnd		= 0;
}

void CTimeMDoc::OnTitleEmptycontent()
{
	CTitleLView* pView = GetTitleView();
	if(pView == NULL)
		return;

	DWORD dwSelCnt = pView->GetSelectedCount();
	if(dwSelCnt > 0)
	{
		int nSel;
		m_Action.BeginRecordAction();
		POSITION posSel = pView->GetFirstSelectedItemPosition();
		while(posSel != NULL)
		{
			nSel = pView->GetNextSelectedItem(posSel);
			OnTitleEmptycontent(nSel);
		}
		m_Action.EndRecordAction();
		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
}

void CTimeMDoc::OnTitleEmptycontent(int nSel)
{
	PTITLE_UNIT pUnit = m_Action.GetItem(nSel);
	pUnit->content.Empty();
}

void CTimeMDoc::OnTitleMovetime()
{
	CTitleLView* pView = GetTitleView();
	if(pView == NULL)
		return;

	DWORD dwFlag;
	int nOffset = GetMoveOffset(dwFlag);
	if(nOffset == 0)
		return;

	DWORD dwSelCnt = pView->GetSelectedCount();
	int nCnt = m_Action.GetItemCount();
	int nStart = -1;
	int nEnd = -1;
	switch(dwFlag)
	{
	case 1:
		if(dwSelCnt > 0)
		{
			int nSel;
			m_Action.BeginRecordAction();
			POSITION posSel = pView->GetFirstSelectedItemPosition();
			while(posSel != NULL)
			{
				nSel = pView->GetNextSelectedItem(posSel);
				OnTitleMovetime(nOffset, nSel);
			}
			m_Action.EndRecordAction();
			SetModifiedFlagAutoSave();
			UpdateRefWin();
		}
		else
		{
			AfxMessageBox(IDS_NOSELITEMFOUND);
		}
		return;
	case 2:
		nStart = 0;
		nEnd = nCnt;
		break;
	case 3:
		nStart = LocatePrevMarkItem();
		nEnd = LocateNextMarkItem();
		if((-1 == nStart)
			||(-1 == nEnd))
		{
			AfxMessageBox(IDS_MARKPOSERROR);
			return;
		}
		break;
	case 4:
		if(-1 == GetCurrentPos())
		{
			AfxMessageBox(IDS_CURPOSERROR);
			return;
		}
		nStart = GetCurrentPos();
		nEnd = nCnt;
		break;
	}
	if((-1 == nStart)
		||(-1 == nEnd))
		return;

	m_Action.BeginRecordAction();
	for(int iPos = nStart; iPos < nEnd; iPos ++)
	{
		OnTitleMovetime(nOffset, iPos);
	}
	m_Action.EndRecordAction();
	SetModifiedFlagAutoSave();
	UpdateRefWin();
}

void CTimeMDoc::OnTitleCBUpdateCmdUI(CCmdUI* pCmdUI)
{
	CTitleLView* pView = GetTitleView();
	if((pView == NULL)
		||(pView->GetSelectedCount() < 2))
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable();
	}
}

void CTimeMDoc::OnTitleUpdateCmdUIRange(CCmdUI* pCmdUI)
{
	CTitleLView* pView = GetTitleView();
	if((pView == NULL)
		||(pView->GetSelectedCount() < 1))
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable();
	}
}

BOOL CTimeMDoc::IsInPause()
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if(pPlayer == NULL)
		return FALSE;
	long nState = pPlayer->GetPlayState();
	switch(nState)
	{
	case wmppsPaused:
		return TRUE;
	case wmppsPlaying:
	case wmppsScanForward:
	case wmppsScanReverse:
	default:
		return FALSE;
	}
}

BOOL CTimeMDoc::IsInPlaying()
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if(pPlayer == NULL)
		return FALSE;
	long nState = pPlayer->GetPlayState();
	switch(nState)
	{
	case wmppsPlaying:
	case wmppsScanForward:
	case wmppsScanReverse:
	case wmppsPaused:
		return TRUE;
	default:
		return FALSE;
	}
}

void CTimeMDoc::OnTitleMovetime(int nOffset, int nSel)
{
	PTITLE_UNIT pUnit = m_Action.GetItem(nSel);
	pUnit->nStart += nOffset;
	if(pUnit->nStart < 0)
		pUnit->nStart = 0;
	else if(pUnit->nStart > 359999999)
		pUnit->nStart = 359999999;

	pUnit->nEnd += nOffset;
	if(pUnit->nEnd < 0)
		pUnit->nEnd = 0;
	else if(pUnit->nEnd > 359999999)
		pUnit->nEnd = 359999999;
}

int CTimeMDoc::GetExtraTime()
{
	CInputExtraTimeDlg dlgExtra;
	if(dlgExtra.DoModal() == IDOK)
	{
		return CTitleHelper::GetTimeValue(dlgExtra.m_strTimeExtra);
	}
	return 0;
}

void CTimeMDoc::TitleExtend(int nItem, int nExtra)
{
	PTITLE_UNIT pUnit = m_Action.GetItem(nItem);
	int nTimeLen = pUnit->nEnd - pUnit->nStart;

	int nTimeMax = nTimeLen + nExtra;
	if((nItem + 1) < GetTitleCount())
	{
		PTITLE_UNIT pUnitNext = m_Action.GetItem(nItem + 1, FALSE);
		nTimeMax = pUnitNext->nStart - pUnit->nStart;
	}

	if(nTimeLen + nExtra > nTimeMax)
	{
		pUnit->nEnd = pUnit->nStart + nTimeMax - 1;
	}
	else
	{
		pUnit->nEnd = pUnit->nEnd + nExtra;
	}
}

void CTimeMDoc::OnTitleExtend()
{
	CTitleLView* pView = GetTitleView();
	if(pView == NULL)
		return;

	int nExtra = GetExtraTime();
	if(nExtra == 0)
		return;

	DWORD dwSelCnt = pView->GetSelectedCount();
	if(dwSelCnt > 0)
	{
		int nSel;
		
		m_Action.BeginRecordAction();
		POSITION posSel = pView->GetFirstSelectedItemPosition();
		while(posSel != NULL)
		{
			nSel = pView->GetNextSelectedItem(posSel);
			TitleExtend(nSel, nExtra);
		}
		m_Action.EndRecordAction();

		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
}

//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
DWORD WINAPI PlaySyncThd(LPVOID lpParam)
{
	CTimeMDoc* pDoc = (CTimeMDoc*)lpParam;
	pDoc->m_bSyncRunning = TRUE;
	while(!pDoc->m_bThdBreak)
	{
		if(pDoc->IsInPlaying())
		{
			if((pDoc->m_bUseSync)
				&&(NULL != theApp.m_pMainWnd))
				theApp.m_pMainWnd->SendMessage(g_nMsg_SyncVideoTitle);
			else
				Sleep(500);
			Sleep(20);
		}
		else
		{
			Sleep(100);
		}
	}
	pDoc->m_bSyncRunning = FALSE;
	return 0;
}

int CTimeMDoc::IsLocateInTitleUnit(int nItemPos, int nPlayPos)
{
	int nSize = GetTitleCount();
	if((nItemPos >= nSize)
		||(nItemPos < 0)
		||(nPlayPos < 0))
		return -1;
	PTITLE_UNIT pUnit1 = m_Action.GetItem(nItemPos, FALSE);
	if(pUnit1->nStart > nPlayPos)
		return -1;
	if(pUnit1->nEnd < nPlayPos)
		return 1;
	return 0;
}

void CTimeMDoc::SyncVideoTitle()
{
	try
	{
		CTitlePaneDlg* pTitlePane = GetDetailWnd();
		CPropertiesWnd* pPlayer = GetPlayerWnd();
		int nPlayPos = pPlayer->GetCurrentPosition();
		int nSize = GetTitleCount();
		for(int iItem = 0; iItem < nSize; iItem ++)
		{
			int nRet = IsLocateInTitleUnit(iItem, nPlayPos);
//			HDC hVideoDC = pPlayer->GetVideoDC();
//			::DrawText(hVideoDC, 
//			pPlayer->ReleaseDC(hVideoDC);
			if(nRet == 0)
			{
				if(GetCurrentPos() != iItem)
				{
					CSingleLock singleLock(&m_CriticalSect);
					singleLock.Lock();
					SetCurrentPos(iItem);
					UpdateRefWin();
					singleLock.Unlock();
				}
				break;
			}
		}
	}
	catch(...)
	{
		OutputDebugString(_T("Catch an error when syncvideotitle!\r\n"));
	}
}

////////////////////////////////////
void CTimeMDoc::InsertPreTrans(PTITLE_UNIT pUnit)
{
	CString strChsInsert;
	strChsInsert.LoadString(IDS_CHSTITLEDEFAULT);
	pUnit->content.Insert(0, strChsInsert);
}

void CTimeMDoc::PreProcessCC()
{
	int nSize = GetTitleCount();
	for(int iTitle = nSize - 1; iTitle >= 0; iTitle --)
	{
		PTITLE_UNIT pUnit = m_Action.GetItem(iTitle);
		CTitleHelper::CCSentence(pUnit->content);
		CTitleHelper::RemoveDupSpace(pUnit->content);
		if(pUnit->content.IsEmpty())
		{
			m_Action.Erase(iTitle);
			continue;
		}
		if(pUnit->content.GetLength() > 60)
		{
			PTITLE_UNIT pUnitNew = new TITLE_UNIT;
			InitializeUnit(pUnitNew);
			if(CTitleHelper::BreakUnitByShare(2, pUnit, pUnitNew))
			{
				InsertPreTrans(pUnitNew);
				m_Action.Insert(iTitle + 1, pUnitNew);
			}
			else
			{
				delete pUnitNew;
			}
		}
		InsertPreTrans(pUnit);
	}
}

////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////

void CTimeMDoc::OnVideoFileOpen()
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if((pPlayer != NULL)
		&&(pPlayer->OnVideoFileOpen()))
	{
		//m_bThdBreak = FALSE;
		//ResumeThread(m_hSyncThd);
	}
	//open video
	
	
		
	



}

void CTimeMDoc::OnVideoPlayPause()
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if(pPlayer != NULL)
		pPlayer->OnVideoPlayPause();
}

void CTimeMDoc::OnVideoStop()
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if(pPlayer != NULL)
		pPlayer->OnVideoStop();
}

void CTimeMDoc::OnVideoStepone()
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if(pPlayer != NULL)
		pPlayer->OnVideoStepone();
}

void CTimeMDoc::OnVideoForward1s()
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if(pPlayer != NULL)
		pPlayer->OnVideoForward1s();
}

void CTimeMDoc::OnVideoBack1s()
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if(pPlayer != NULL)
		pPlayer->OnVideoBack1s();
}

void CTimeMDoc::OnVideoForward5s()
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if(pPlayer != NULL)
		pPlayer->OnVideoForward5s();
}

void CTimeMDoc::OnVideoBack5s()
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if(pPlayer != NULL)
		pPlayer->OnVideoBack5s();
}

void CTimeMDoc::OnToolPreproc()
{
	AfxGetApp()->BeginWaitCursor();
	m_Action.BeginRecordAction();
	PreProcessCC();
	m_Action.EndRecordAction();

	SetModifiedFlagAutoSave();
	UpdateRefWin();
	AfxGetApp()->EndWaitCursor();
}

void CTimeMDoc::OnVideoSynctitle()
{
	m_bUseSync = !m_bUseSync;
}

void CTimeMDoc::OnUpdateVideoSynctitle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bUseSync);
}

void CTimeMDoc::SetPlayPos(int nItem, BOOL bStart)
{
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	if(pPlayer == NULL)
		return;

	if((nItem < 0)
		||nItem > (GetTitleCount() - 1))
		return;

	PTITLE_UNIT pUnit = m_Action.GetItem(nItem, FALSE);
	pPlayer->SetCurrentPosition(bStart?pUnit->nStart:pUnit->nEnd);
}

void CTimeMDoc::ToEditTitle(int nItem)
{
	CTitlePaneDlg* pEditor = GetDetailWnd();
	pEditor->FocusTitle();
}

////////////////////////////////////////////////////
//
void CTimeMDoc::SelectItem(int nItem)
{
	CTitleLView* pList = GetTitleView();
	pList->SelectItem(nItem);
}

void CTimeMDoc::OnEditSelectAll()
{
	CTitleLView* pList = GetTitleView();
	pList->OnEditSelectAll();
}

void CTimeMDoc::OnEditCopy()
{
	HGLOBAL hgblCopy;
	LPTSTR lpstrData;
	HWND hWndMain = AfxGetMainWnd()->GetSafeHwnd();

	CTitleLView* pList = GetTitleView();

	CString strAllSel;
	CString strItem;
	POSITION posSel = pList->GetFirstSelectedItemPosition();
	while(posSel)
	{
		int nSel = pList->GetNextSelectedItem(posSel);
		PTITLE_UNIT pUnit = m_Action.GetItem(nSel, FALSE);

		strItem = CTitleHelper::FormatSRTString(nSel + 1, pUnit);

		strAllSel += strItem;
	}
	if(strAllSel.IsEmpty())
		return;

	int nLen = strAllSel.GetLength();
	if(OpenClipboard(hWndMain))
	{
		hgblCopy = GlobalAlloc(GMEM_MOVEABLE, (nLen + 1)*sizeof(TCHAR));
		lpstrData = (LPTSTR)GlobalLock(hgblCopy);

		CopyMemory(lpstrData, (LPCTSTR)strAllSel, nLen*sizeof(TCHAR));
		lpstrData[nLen] = 0;

		SetClipboardData(CF_TEXT, hgblCopy);
		CloseClipboard();
	}
}

void CTimeMDoc::OnEditCut()
{
	OnEditCopy();
	OnTitleDelete();
}

void CTimeMDoc::OnEditPaste()
{
	HGLOBAL		hgblCopy;
	BUF_READ	bufRead;
	HWND hWndMain = AfxGetMainWnd()->GetSafeHwnd();

	if(!IsClipboardFormatAvailable(CF_TEXT))
		return;

	if(OpenClipboard(hWndMain))
	{
		m_Action.BeginRecordAction();
		int nPastePos = GetCurrentPos();

		hgblCopy = GetClipboardData(CF_TEXT);
		if(hgblCopy != NULL)
		{
			bufRead.bufFile = (LPBYTE)GlobalLock(hgblCopy);
			if(bufRead.bufFile != NULL)
			{
				bufRead.dwBufSize		= GlobalSize(hgblCopy);
				bufRead.dwBufOffset		= 0;
				bufRead.dwFmtFlags		= FMTFLAG_SRT;

				bufRead.nUnicodeState = CTitleFile::TestUnicodeBuf(bufRead.bufFile, bufRead.dwBufSize);
				CTitleFile::StepSignature(&bufRead);
				CTitleFile::ProcessReverse(&bufRead);

				PTITLE_UNIT pUnit;
				while((pUnit = CTitleFile::StepTitleUnit(&bufRead)) != NULL)
				{
					if(nPastePos != -1)
					{
						m_Action.Insert(nPastePos, pUnit);
						nPastePos ++;
					}
					else
					{
						m_Action.PushBack(pUnit);
					}
				}

				GlobalLock(hgblCopy);
			}
		}
		CloseClipboard();
		m_Action.EndRecordAction();
	}
	SetModifiedFlagAutoSave();
	UpdateRefWin();
}

int CTimeMDoc::DoSearch(int nStartFrom, BOOL bCase, BOOL bDN, const CString& strToSearch)
{
	PTITLE_UNIT pUnit;
	int nSize = GetTitleCount();

	int nStart, nEnd, iStep;
	nStart = nStartFrom;
	if(bDN)
	{
		if(nStart == -1)
			nStart = 0;
		nEnd = nSize;
		iStep = 1;
	}
	else
	{
		if(nStart == -1)
			nStart = nSize - 1;
		nEnd = -1;
		iStep = -1;
	}

	LPCTSTR lpszRet = NULL;
	int iTitle = nStart;
	while(iTitle != nEnd)
	{
		pUnit = m_Action.GetItem(iTitle, FALSE);
		if(bCase)
			lpszRet = StrStr(pUnit->content, strToSearch);
		else
			lpszRet = StrStrI(pUnit->content, strToSearch);

		if(lpszRet != NULL)
		{
			return iTitle;
		}
		iTitle += iStep;
	}
	return -1;
}

void CTimeMDoc::OnEditFind()
{
	int nRet;
	if(IDOK == m_dlgFind.DoModal())
	{
		nRet = DoSearch(GetCurrentPos(),
			m_dlgFind.m_bCaseSense,
			m_dlgFind.m_bDNDirect,
			m_dlgFind.m_strToFind);
		if(nRet != -1)
		{
			SetCurrentPos(nRet);
			UpdateRefWin();
		}
	}
}

void CTimeMDoc::EditReplace(const CString& strTarget, const CString& strResult,
							BOOL bCase, BOOL bEng, BOOL bChs, BOOL bSelected)
{
	if(bSelected)
	{
		CTitleLView* pView = GetTitleView();
		POSITION posSel = pView->GetFirstSelectedItemPosition();
		m_Action.BeginRecordAction();
		while(posSel != NULL)
		{
			int nItem = pView->GetNextSelectedItem(posSel);
			PTITLE_UNIT pUnit = m_Action.GetItem(nItem);
			if(CTitleHelper::ReplaceTitle(pUnit, strTarget, strResult, bCase, bEng, bChs))
				SetModifiedFlagAutoSave();
			else
				m_Action.PopLastRecord();
		}
		m_Action.EndRecordAction();
	}
	else
	{
		int nSize = GetTitleCount();
		m_Action.BeginRecordAction();
		for(int iTitle = 0; iTitle < nSize; iTitle ++)
		{
			PTITLE_UNIT pUnit = m_Action.GetItem(iTitle);
			if(CTitleHelper::ReplaceTitle(pUnit, strTarget, strResult, bCase, bEng, bChs))
				SetModifiedFlagAutoSave();
			else
				m_Action.PopLastRecord();
		}
		m_Action.EndRecordAction();
	}
}

void CTimeMDoc::DoReplace()
{
	if(m_dlgReplace.m_bUseBatch)
	{
		RITER_UNIT iterUnit;

		for(iterUnit = m_dlgReplace.m_mapUnits.rbegin(); iterUnit != m_dlgReplace.m_mapUnits.rend(); iterUnit ++)
		{
			EditReplace(iterUnit->second.strToFind,
				iterUnit->second.strReplTo,
				m_dlgReplace.m_bCase,
				m_dlgReplace.m_bIncEng,
				m_dlgReplace.m_bIncChs,
				m_dlgReplace.m_bForSelected);
		}
	}
	else
	{
		EditReplace(m_dlgReplace.m_strTarget,
			m_dlgReplace.m_strResult,
			m_dlgReplace.m_bCase,
			m_dlgReplace.m_bIncEng,
			m_dlgReplace.m_bIncChs,
			m_dlgReplace.m_bForSelected);
	}
	UpdateRefWin();
}

void CTimeMDoc::OnEditReplace()
{
	if(!m_dlgReplace.IsWindowVisible())
	{
//		m_dlgReplace.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		m_dlgReplace.ShowWindow(SW_SHOW);
	}
	else
	{
		m_dlgReplace.ShowWindow(SW_HIDE);
	}
}

/////////////////////////////////////////
//
void CTimeMDoc::OnEditFindnext()
{
	if(m_dlgFind.m_strToFind.IsEmpty())
		OnEditFind();
	else
	{
		int nRet;
		nRet = DoSearch(GetCurrentPos() + (m_dlgFind.m_bDNDirect?1:-1),
			m_dlgFind.m_bCaseSense,
			m_dlgFind.m_bDNDirect,
			m_dlgFind.m_strToFind);
		if(nRet != -1)
		{
			SetCurrentPos(nRet);
			UpdateRefWin();
		}
	}
}

void CTimeMDoc::OnUpdateEditFindnext(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_dlgFind.m_strToFind.IsEmpty());
}

//==============================
int CTimeMDoc::GetSnapOffset()
{
	int nItemPos = GetCurrentPos();
	if(nItemPos == -1)
		return 0;
	CPropertiesWnd* pPlayer = GetPlayerWnd();
	int nPlayPos = pPlayer->GetCurrentPosition()
		+ theApp.m_Params.GetStartDelay();

	PTITLE_UNIT pUnit = m_Action.GetItem(nItemPos, FALSE);
	return nPlayPos - pUnit->nStart;
}

void CTimeMDoc::SnapLines(int nStart, int nEnd, int nOffset)
{
	if((nEnd > m_Action.GetItemCount())
		||(nStart > m_Action.GetItemCount())
		||(nStart < 0)
		||(nEnd < 0))
		return;

	if( 0 == nOffset )
		return;

	if(nEnd > nStart)
	{
		m_Action.BeginRecordAction();
		for(int iItem = nStart; iItem < nEnd; iItem ++)
		{
			PTITLE_UNIT pUnit = m_Action.GetItem(iItem);
			pUnit->nStart += nOffset;
			pUnit->nEnd += nOffset;
		}
		m_Action.EndRecordAction();

		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
}

void CTimeMDoc::OnTitleAutosnap()
{
	m_bAutoSnap = !m_bAutoSnap;
	int nCurrPos = GetCurrentPos();
	if(-1 == nCurrPos)
		SetCurrentPos(0);
	nCurrPos = 0;
	CTitleLView* pView = GetTitleView();
	if(m_bAutoSnap)
	{
		m_nSnapCount = 0;
		if(pView != NULL)
			pView->SetEdit(nCurrPos, 1);
	}
	else
	{
		if(pView)
			pView->SetEdit(-1, -1);
	}
	UpdateRefWin();
}

void CTimeMDoc::OnTitleSnapnext()
{
	if(m_bAutoSnap)
	{
		if((m_nSnapCount%2) == 0)
			OnTitleSnaplinestart();
		else
			OnTitleSnaplineend();

		int nCurrPos = GetCurrentPos();
		CTitleLView* pView = GetTitleView();
		if((-1 == nCurrPos))
		{
			OnTitleAutosnap();
			if(pView != NULL)
				pView->SetEdit(-1, -1);
		}
		else
		{
			m_nSnapCount ++;
			if(pView != NULL)
				pView->SetEdit(nCurrPos, (m_nSnapCount%2)?2:1);
		}
	}
}

void CTimeMDoc::OnTitleSnapUpdateCmdUIRange(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsInPlaying()&&(!(-1 == GetCurrentPos())));
}

void CTimeMDoc::OnTitleSnapline()
{
	int nItemPos = GetCurrentPos();
	SnapLines(nItemPos, nItemPos + 1, GetSnapOffset());
}

void CTimeMDoc::OnTitleSnapafter()
{
	SnapLines(GetCurrentPos(), GetTitleCount(), GetSnapOffset());
}

void CTimeMDoc::OnTitleSnapall()
{
	SnapLines(0, GetTitleCount(), GetSnapOffset());
}

void CTimeMDoc::OnTitleSnapprevmark()
{
	SnapLines(LocatePrevMarkItem(), GetCurrentPos(), GetSnapOffset());
}

void CTimeMDoc::OnTitleSnapbetweentwomark()
{
	SnapLines(LocatePrevMarkItem(), LocateNextMarkItem(), GetSnapOffset());
}

void CTimeMDoc::OnTitleSnaplineend()
{
	int nItemPos = GetCurrentPos();
	if(nItemPos == -1)
		return;

	CPropertiesWnd* pPlayer = GetPlayerWnd();
	int nPlayPos = pPlayer->GetCurrentPosition()
		+ theApp.m_Params.GetStartDelay();

	m_Action.BeginRecordAction();
	PTITLE_UNIT pUnit = m_Action.GetItem(nItemPos);
	if(pUnit != NULL)
		pUnit->nEnd = nPlayPos;
	m_Action.EndRecordAction();

	SetModifiedFlagAutoSave();

	if(nItemPos < GetTitleCount() + 1)
	{
		SetCurrentPos(nItemPos + 1);
	}
	UpdateRefWin();
}

void CTimeMDoc::OnTitleSnaplinestart()
{
	int nItemPos = GetCurrentPos();
	if(nItemPos == -1)
		return;

	CPropertiesWnd* pPlayer = GetPlayerWnd();
	int nPlayPos = pPlayer->GetCurrentPosition()
		+ theApp.m_Params.GetStartDelay();

	m_Action.BeginRecordAction();
	PTITLE_UNIT pUnit = m_Action.GetItem(nItemPos);
	if(pUnit != NULL)
		pUnit->nStart = nPlayPos;
	m_Action.EndRecordAction();

	SetModifiedFlagAutoSave();
	UpdateRefWin();
}

//==============================
void CTimeMDoc::OnToolCmbytime()
{
	CCombineTitleByTimeDlg dlgCBByTime;
	dlgCBByTime.DoModal();
}

void CTimeMDoc::OnEditRedo()
{
	SetCurrentPos(m_Action.Redo());
	SetModifiedFlagAutoSave();
	UpdateRefWin();
}

void CTimeMDoc::OnEditUndo()
{
	SetCurrentPos(m_Action.Undo());
	SetModifiedFlagAutoSave();
	UpdateRefWin();
}

BOOL CTimeMDoc::IsCanUndo()
{
	return m_Action.IsCanUndo();
}

BOOL CTimeMDoc::IsCanRedo()
{
	return m_Action.IsCanRedo();
}

void CTimeMDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Action.IsCanUndo());
}

void CTimeMDoc::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Action.IsCanRedo());
}

int CTimeMDoc::LookForUpLocation(const VT_TITLE& vtTag, const CString& strTag)
{
	int nSize = (int)vtTag.size();
	for(int iTitle = 0; iTitle < nSize; iTitle ++)
	{
		if(StrCmpI(strTag, vtTag[iTitle]->content) < 0)
		{
			return iTitle;
		}
	}
	return nSize;
}

int CTimeMDoc::LookForDnLocation(const VT_TITLE& vtTag, const CString& strTag)
{
	int nSize = (int)vtTag.size();
	for(int iTitle = 0; iTitle < nSize; iTitle ++)
	{
		if(StrCmpI(strTag, vtTag[iTitle]->content) > 0)
		{
			return iTitle;
		}
	}
	return nSize;
}

void CTimeMDoc::SortChanged(int nCol, BOOL bAscending)
{
	m_bAscending	= bAscending;
	m_nSortCol		= nCol;
	if(!CanSort(nCol))
		return;

	if(nCol == 3)
	{
		m_Action.BeginRecordAction();
		m_Action.SaveAllUndo();
		VT_TITLE vtTitleSave = m_Action.GetVtTitle();
		VT_TITLE& vtTitleTag = m_Action.GetVtTitle();
		vtTitleTag.clear();

		int nSize = vtTitleSave.size();
		for(int iTitle = 0; iTitle < nSize; iTitle ++)
		{
			int nPos = bAscending?LookForUpLocation(vtTitleTag, vtTitleSave[iTitle]->content):LookForDnLocation(vtTitleTag, vtTitleSave[iTitle]->content);
			vtTitleTag.insert(vtTitleTag.begin() + nPos, vtTitleSave[iTitle]);
		}

		m_Action.EndRecordAction();
		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
	else
	{

		SORTINT_MAP mapIntSort;
		SORTINT_MAP::iterator itSort;
		SORTINT_MAP::reverse_iterator itRSort;
		PTITLE_UNIT pUnit;
		int nCnt = GetTitleCount();

		for(int iItem = 0; iItem < nCnt; iItem ++)
		{
			pUnit = m_Action.GetItem(iItem, FALSE);
			switch(nCol)
			{
			case 1:
				mapIntSort.insert(make_pair(pUnit->nStart, iItem));
				break;
			case 2:
				mapIntSort.insert(make_pair(pUnit->nEnd, iItem));
				break;
			case 4:
				mapIntSort.insert(make_pair(pUnit->content.GetLength(), iItem));
				break;
			case 5:
				mapIntSort.insert(make_pair(pUnit->nEnd - pUnit->nStart, iItem));
				break;
			case 6:
				CString strItem;
				int nCnt = CTitleHelper::CountTitleLine(pUnit->content, strItem);
				mapIntSort.insert(make_pair(nCnt, iItem));
				break;
			}
		}

		if(mapIntSort.size() > 0)
		{
			m_Action.BeginRecordAction();
			m_Action.SaveAllUndo();
			VT_TITLE vtTitle = m_Action.GetVtTitle();
			m_Action.GetVtTitle().clear();
			if(m_bAscending)
			{
				for(itSort = mapIntSort.begin(); itSort != mapIntSort.end(); itSort ++)
				{
					m_Action.GetVtTitle().push_back(vtTitle[itSort->second]);
				}
			}
			else
			{
				for(itRSort = mapIntSort.rbegin(); itRSort != mapIntSort.rend(); itRSort ++)
				{
					m_Action.GetVtTitle().push_back(vtTitle[itRSort->second]);
				}
			}

			m_Action.EndRecordAction();
			SetModifiedFlagAutoSave();
			UpdateRefWin();
		}
	}
}

BOOL CTimeMDoc::CanSort(int nCol)
{
	if(nCol < 0)
		return FALSE;
	return TRUE;
}

void CTimeMDoc::OnEditSortstart()
{
	CTitleLView* pView = GetTitleView();
	if(pView != NULL)
		pView->SwitchSortColumn(1);
}

void CTimeMDoc::OnEditSortend()
{
	CTitleLView* pView = GetTitleView();
	if(pView != NULL)
		pView->SwitchSortColumn(2);
}

void CTimeMDoc::OnEditSortcontent()
{
	CTitleLView* pView = GetTitleView();
	if(pView != NULL)
		pView->SwitchSortColumn(3);
}

void CTimeMDoc::OnEditSortlen()
{
	CTitleLView* pView = GetTitleView();
	if(pView != NULL)
		pView->SwitchSortColumn(4);
}

void CTimeMDoc::MarkItem(int nItem)
{
	PTITLE_UNIT pUnit = GetTitleItem(nItem);
	if(pUnit != NULL)
	{
		if((pUnit->dwFlags & FLAG_MARKMASK) == FLAG_TITLEMARK1)
		{
			pUnit->dwFlags &= FLAG_MARKCLEAR;
		}
		else
		{
			pUnit->dwFlags &= FLAG_MARKCLEAR;
			pUnit->dwFlags |= FLAG_TITLEMARK1;
		}
	}
	UpdateRefWin();
}

BOOL CTimeMDoc::HasMark(int nItem)
{
	PTITLE_UNIT pUnit = GetTitleItem(nItem);
	if(pUnit != NULL)
	{
		if((pUnit->dwFlags & FLAG_MARKMASK) == FLAG_TITLEMARK1)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int CTimeMDoc::LocatePrevMarkItem()
{
	int nStart = GetCurrentPos();
	int iPos = nStart;
	while(1)
	{
		iPos --;
		if(HasMark(iPos))
		{
			return iPos;
		}

		if(iPos == -1)
			iPos = GetTitleCount();
		if(iPos == nStart - 1)
			break;
	}
	return -1;
}

int CTimeMDoc::LocateNextMarkItem()
{
	int nStart = GetCurrentPos();
	int iPos = nStart + 1;
	while(1)
	{
		if(HasMark(iPos))
		{
			return iPos;
		}

		iPos ++;
		if(iPos == GetTitleCount())
			iPos = 0;
		if(iPos == nStart + 1)
			break;
	}
	return -1;
}

void CTimeMDoc::OnEditMark()
{
	MarkItem(GetCurrentPos());
}

void CTimeMDoc::OnEditGotomark()
{
	int nPos = LocateNextMarkItem(); 
	if(-1 != nPos)
	{
		SetCurrentPos(nPos);
		UpdateRefWin();
	}
}

void CTimeMDoc::OnEditGotoprevmark()
{
	int nPos = LocatePrevMarkItem(); 
	if(-1 != nPos)
	{
		SetCurrentPos(nPos);
		UpdateRefWin();
	}
}

void CTimeMDoc::OnToolCmbyuser()
{
	CCombinTitleTransDlg dlgTransCB;
	dlgTransCB.DoModal();
}

void CTimeMDoc::OnToolCompare()
{
	CCompareTitleDlg dlgCompare;
	dlgCompare.DoModal();
}

void CTimeMDoc::OnToolCheck()
{
	if(!m_dlgCheckValid.IsWindowVisible())
	{
//		m_dlgCheckValid.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		m_dlgCheckValid.ShowWindow(SW_SHOW);
	}
	else
	{
		m_dlgCheckValid.ShowWindow(SW_HIDE);
	}
}

void CTimeMDoc::OnUpdateToolCheck(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_dlgCheckValid.IsWindowVisible());
}

void CTimeMDoc::ChkClear()
{
	m_vtChkParams.clear();
	m_mapChkParams.clear();
}

void CTimeMDoc::ChkExceedSpecChs(int nMaxChs)
{
	PTITLE_UNIT pUnit;
	CHECK_PARAM chkParam;
	int nSize = m_Action.GetItemCount();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		pUnit = m_Action.GetItem(iItem, FALSE);
		if(pUnit != NULL)
		{
			CString strItem = pUnit->content;
			CTitleHelper::RemoveAssCode(strItem);
			if(CTitleHelper::CountChsChar(strItem) > nMaxChs)
			{
				chkParam.nErrType = 1;
				chkParam.nTitleId = iItem;
				m_mapChkParams.insert(make_pair(iItem, chkParam));
			}
		}
	}
}

void CTimeMDoc::ChkStartMoreEnd()
{
	PTITLE_UNIT pUnit;
	CHECK_PARAM chkParam;
	int nSize = m_Action.GetItemCount();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		pUnit = m_Action.GetItem(iItem, FALSE);
		if(pUnit != NULL)
		{
			if(!(pUnit->nEnd > pUnit->nStart))
			{
				chkParam.nErrType = 2;
				chkParam.nTitleId = iItem;
				m_mapChkParams.insert(make_pair(iItem, chkParam));
			}
		}
	}
}

void CTimeMDoc::ChkTimeOverlap()
{
	PTITLE_UNIT pUnit;
	PTITLE_UNIT pUnitPrev;
	CHECK_PARAM chkParam;
	int nSize = m_Action.GetItemCount();
	if(nSize < 2)
		return;
	pUnitPrev = m_Action.GetItem(0, FALSE);
	for(int iItem = 1; iItem < nSize; iItem ++)
	{
		pUnit = m_Action.GetItem(iItem, FALSE);
		if(pUnit != NULL)
		{
			if(pUnit->nStart < pUnitPrev->nEnd)
			{
				chkParam.nErrType = 3;
				chkParam.nTitleId = iItem;
				m_mapChkParams.insert(make_pair(iItem - 1, chkParam));
			}
		}
		pUnitPrev = pUnit;
	}
}

void CTimeMDoc::ChkTimeEmpty()
{
	PTITLE_UNIT pUnit;
	CHECK_PARAM chkParam;
	int nSize = m_Action.GetItemCount();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		pUnit = m_Action.GetItem(iItem, FALSE);
		if(pUnit != NULL)
		{
			if(pUnit->nEnd == 0)
			{
				chkParam.nErrType = 4;
				chkParam.nTitleId = iItem;
				m_mapChkParams.insert(make_pair(iItem, chkParam));
			}
		}
	}
}

void CTimeMDoc::ChkTimeMoreThan(int nMaxTime)
{
	PTITLE_UNIT pUnit;
	CHECK_PARAM chkParam;
	int nSize = m_Action.GetItemCount();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		pUnit = m_Action.GetItem(iItem, FALSE);
		if(pUnit != NULL)
		{
			if((pUnit->nEnd - pUnit->nStart) > nMaxTime)
			{
				chkParam.nErrType = 5;
				chkParam.nTitleId = iItem;
				m_mapChkParams.insert(make_pair(iItem, chkParam));
			}
		}
	}
}

void CTimeMDoc::ChkTimeLessThan(int nMinTime)
{
	PTITLE_UNIT pUnit;
	CHECK_PARAM chkParam;
	int nSize = m_Action.GetItemCount();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		pUnit = m_Action.GetItem(iItem, FALSE);
		if(pUnit != NULL)
		{
			if((pUnit->nEnd - pUnit->nStart) < nMinTime)
			{
				chkParam.nErrType = 6;
				chkParam.nTitleId = iItem;
				m_mapChkParams.insert(make_pair(iItem, chkParam));
			}
		}
	}
}

void CTimeMDoc::ChkTitleLineMoreThan(int nLineMaxCnt)
{
	PTITLE_UNIT pUnit;
	CHECK_PARAM chkParam;
	int nSize = m_Action.GetItemCount();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		pUnit = m_Action.GetItem(iItem, FALSE);
		if(pUnit != NULL)
		{
			if(CTitleHelper::GetCountTitle(pUnit->content) > nLineMaxCnt)
			{
				chkParam.nErrType = 7;
				chkParam.nTitleId = iItem;
				m_mapChkParams.insert(make_pair(iItem, chkParam));
			}
		}
	}
}

void CTimeMDoc::ClearOverlap()
{
	int nSize = (int)m_vtChkParams.size();
	for(int iChk = 0; iChk < nSize; iChk ++)
	{
		if(3 == m_vtChkParams[iChk].nErrType)
		{
			PTITLE_UNIT pUnit = GetTitleItem(m_vtChkParams[iChk].nTitleId);
			PTITLE_UNIT pUnitPrev = GetTitleItem(m_vtChkParams[iChk].nTitleId - 1);
			pUnitPrev->nEnd = pUnit->nStart - 1;
		}
	}
}

void CTimeMDoc::GenerateName(LPTSTR lpszFile, LPCTSTR pOrgName, LPCTSTR lpszLangName, WORD wFmtID)
{
	StringCbCopy(lpszFile, MAX_PATH, PathFindFileName(pOrgName));
	PathRemoveExtension(lpszFile);

	StringCbCat(lpszFile, MAX_PATH, _T("."));
	StringCbCat(lpszFile, MAX_PATH, lpszLangName);

	switch(wFmtID)
	{
	case FILE_FORMAT_SRT:
		StringCbCat(lpszFile, MAX_PATH, _T(".srt"));
		break;
	case FILE_FORMAT_SSA:
		StringCbCat(lpszFile, MAX_PATH, _T(".ssa"));
		break;
	case FILE_FORMAT_ASS:
		StringCbCat(lpszFile, MAX_PATH, _T(".ass"));
		break;
	}
}

void CTimeMDoc::WriteExpTitleFile(LPCTSTR lpszFileName, 
								  VT_TITLE& vtTitle, int iStart, int iEnd,
								  BOOL bUnicode, UINT nCPCode, 
								  DWORD dwFmt, BOOL bUseCode)
{
	CFile fileWrite;
	if(!fileWrite.Open(lpszFileName, CFile::modeWrite|CFile::modeCreate))
		return;

	if(bUnicode)
	{
		WORD wUFlag = 0xFEFF;
		fileWrite.Write(&wUFlag, 2);
	}
	// 处理文件头部的格式信息
	switch(dwFmt)
	{
	case FILE_FORMAT_SRT:
		// Nothing to do
		break;
	case FILE_FORMAT_SSA:
	case FILE_FORMAT_ASS:

		if(!bUnicode)
		{
			CStringA strItemA;
			int nASize = WideCharToMultiByte(nCPCode, 0,
				m_strFmtHeader, m_strFmtHeader.GetLength(),
				NULL, 0, NULL, NULL);
			if(nASize > 0)
			{
				WideCharToMultiByte(nCPCode, 0,
					m_strFmtHeader, m_strFmtHeader.GetLength(),
					strItemA.GetBufferSetLength(nASize), nASize, NULL, NULL);
				fileWrite.Write(strItemA.GetBuffer(), strItemA.GetLength()*sizeof(CHAR));
			}
		}
		else
		{
			fileWrite.Write(m_strFmtHeader, m_strFmtHeader.GetLength()*sizeof(WCHAR));
		}

		break;
	}
	PTITLE_UNIT pUnit = NULL;
	CString strItem;
	int iSrtCnt = 0;
	for(int iTitle = iStart; iTitle <= iEnd; iTitle ++)
	{
		pUnit = vtTitle[iTitle];
		if(pUnit->content.IsEmpty())
			continue;
		switch(dwFmt)
		{
		case FILE_FORMAT_SRT:
			iSrtCnt ++;
			CTitleHelper::RemoveAssCode(pUnit->content);
			strItem = CTitleHelper::FormatSRTString(iSrtCnt, pUnit);
			break;
		case FILE_FORMAT_SSA:
			strItem = CTitleHelper::FormatSSAString(pUnit,
				bUseCode?m_strPreCode:_T(""),
				bUseCode?m_strPostCode:_T(""));
			break;
		case FILE_FORMAT_ASS:
			strItem = CTitleHelper::FormatASSString(pUnit,
				bUseCode?m_strPreCode:_T(""),
				bUseCode?m_strPostCode:_T(""));
			break;
		}

		if(!bUnicode)
		{
			CStringA strItemA;
			int nASize = WideCharToMultiByte(nCPCode, 0,
				strItem, strItem.GetLength(),
				NULL, 0, NULL, NULL);
			if(nASize > 0)
			{
				WideCharToMultiByte(nCPCode, 0,
					strItem, strItem.GetLength(),
					strItemA.GetBufferSetLength(nASize), nASize, NULL, NULL);
				fileWrite.Write(strItemA.GetBuffer(), strItemA.GetLength()*sizeof(CHAR));
			}
		}
		else
		{
			fileWrite.Write(strItem.GetBuffer(), strItem.GetLength()*sizeof(WCHAR));
		}
	}
}

void CTimeMDoc::ZipFile(LPCTSTR lpszFileName, LPCTSTR lpszOutPath, const CStringArray& strSrcFiles)
{
	CString strCmd;
	TCHAR	szFile1[MAX_PATH];
	StringCbCopy(szFile1, MAX_PATH, lpszOutPath);
	StringCbCopy(szFile1, MAX_PATH, PathFindFileName(lpszFileName));
	PathRemoveExtension(szFile1);
	StringCbCat(szFile1, MAX_PATH, _T(".rar"));
	theApp.m_Params.GetZipPath();

	strCmd.Format(_T("\"%s\" a \"%s\""), theApp.m_Params.GetZipPath(), szFile1);

	CString strFile;
	int nCntFiles = strSrcFiles.GetCount();
	for(int iFile = 0; iFile < nCntFiles; iFile ++)
	{
		strFile.Format(_T(" \"%s\""), strSrcFiles[iFile]);
		strCmd += strFile;
	}
		// "<rar>" a "<tag>" "<src>"
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	if(CreateProcess(NULL, strCmd.GetBuffer(), NULL, NULL, FALSE, 0, NULL, lpszOutPath, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	strCmd.ReleaseBuffer();
}

void CTimeMDoc::TransData(VT_TITLE& vtExportTo, VT_TITLE& vtSrc, WORD wFmtID, WORD wLangFlag)
{
	int nSize = vtSrc.size();
	for(int iTitle = 0; iTitle < nSize; iTitle ++)
	{
		PTITLE_UNIT pUnitNew = new TITLE_UNIT;
		InitializeUnit(pUnitNew);
		PTITLE_UNIT pUnit = vtSrc[iTitle];
		pUnitNew->nStart = pUnit->nStart;
		pUnitNew->nEnd	= pUnit->nEnd;

		CStringArray astrLines;
		CTitleHelper::BreakStringEachLine(pUnit->content, astrLines);
		int nCnt = astrLines.GetCount();
		for(int iLine = 0; iLine < nCnt; iLine ++)
		{

			CString strLine1 = astrLines[iLine];
			CTitleHelper::RemoveAssCode(strLine1);
			CString strLine2;
			if((nCnt == 2)&&(iLine == 0))
			{
				strLine2 = astrLines[iLine];
				CTitleHelper::RemoveAssCode(strLine2);
			}

			if(FILE_FORMAT_SRT == wFmtID)
			{
				if(strLine1.IsEmpty())
					continue;
			}

			if((CTitleHelper::IsDBCSString(strLine1))
				||((nCnt == 2)
				&&(strLine1.Compare(strLine2) == 0)
				&&(iLine == 0)))
			{
				if(wLangFlag&LANG_FLAG_CHS)
				{
					if(!pUnitNew->content.IsEmpty())
						pUnitNew->content += _T("\r\n");
					pUnitNew->content += astrLines[iLine];
				}
				else if(wLangFlag&LANG_FLAG_CHT)
				{
					CString strLine = CTitleFile::LCMapStrCN(SORT_CHINESE_PRC, LCMAP_TRADITIONAL_CHINESE, astrLines[iLine]);

					if(!pUnitNew->content.IsEmpty())
						pUnitNew->content += _T("\r\n");
					pUnitNew->content += strLine;
				}
			}
			else
			{
				if(wLangFlag&LANG_FLAG_ENG)
				{
					if(!pUnitNew->content.IsEmpty())
						pUnitNew->content += _T("\r\n");
					pUnitNew->content += astrLines[iLine];
				}
			}
		}
		vtExportTo.push_back(pUnitNew);
	}
}

void CTimeMDoc::OnFileExport()
{
	CString strOutPath;

	strOutPath = m_strPathName;
	PathRemoveFileSpec(strOutPath.GetBuffer());
	strOutPath.ReleaseBuffer();
	strOutPath += _T("\\");

	CExportOptionDlg dlgExport;
	dlgExport.m_ExportParam.strPreCode = m_strPreCode;
	dlgExport.m_ExportParam.strPostCode = m_strPostCode;
	dlgExport.m_ExportParam.strFmtHeader = m_strFmtHeader;
	dlgExport.m_strOutPath = strOutPath;
	if(IDOK == dlgExport.DoModal())
	{
		AfxGetApp()->DoWaitCursor(1);
		strOutPath = dlgExport.m_strOutPath;
		if(strOutPath.GetAt(strOutPath.GetLength() - 1) != _T('\\'))
			strOutPath += _T("\\");
		m_strPreCode = dlgExport.m_ExportParam.strPreCode;
		m_strPostCode = dlgExport.m_ExportParam.strPostCode;
		m_strFmtHeader = dlgExport.m_ExportParam.strFmtHeader.IsEmpty()?CTitleParams::GetDefASSHeader():dlgExport.m_ExportParam.strFmtHeader;
		if((dlgExport.m_ExportParam.bZipOut)
			&&theApp.m_Params.GetZipPath().IsEmpty())
		{
			int nRet = AfxMessageBox(IDS_NOZIPSET, MB_YESNOCANCEL);
			switch(nRet)
			{
			case IDYES:
				AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_VIEW_CUSTOMIZE, 0);
				break;
			case IDNO:
				dlgExport.m_ExportParam.bZipOut = FALSE;
				break;
			case IDCANCEL:
				return;
			}
		}

		CStringArray strSrcFiles;
		CString strOutFile;
		int nFileCnt = (int)dlgExport.m_ExportParam.vtFiles.size();
		for(int iFile = 0; iFile < nFileCnt; iFile ++)
		{
			VT_TITLE	vtExport;
			TCHAR	szFileName[MAX_PATH];
			TransData(vtExport, m_Action.GetVtTitle(), dlgExport.m_ExportParam.vtFiles[iFile].wFmtID, dlgExport.m_ExportParam.vtFiles[iFile].wLangFlag);
			int		nSize = (int)vtExport.size();
			GenerateName(szFileName, m_strPathName,
				dlgExport.m_ExportParam.vtFiles[iFile].strName,
				dlgExport.m_ExportParam.vtFiles[iFile].wFmtID);
			strSrcFiles.Add(szFileName);

			strOutFile = strOutPath;
			strOutFile += szFileName;

			WriteExpTitleFile(strOutFile, vtExport, 0, nSize - 1,
				dlgExport.m_ExportParam.vtFiles[iFile].bUnicode,
				(dlgExport.m_ExportParam.vtFiles[iFile].wLangFlag&LANG_FLAG_CHT)?950:936, 
				dlgExport.m_ExportParam.vtFiles[iFile].wFmtID, 
				(dlgExport.m_ExportParam.vtFiles[iFile].wLangFlag&LANG_FLAG_ENG) == LANG_FLAG_ENG);
			CTitleFile::DeleteContents(vtExport);
		}

		if(dlgExport.m_ExportParam.bZipOut)
			ZipFile(m_strPathName, strOutPath, strSrcFiles);
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CTimeMDoc::OnUpdateTitleAutosnap(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bAutoSnap);
}

void CTimeMDoc::OnDefaultCharset()
{
	m_dwCharsetForDisp = ANSI_CHARSET;
	UpdateRefWin();
}

void CTimeMDoc::OnBig5Charset()
{
	m_dwCharsetForDisp = CHINESEBIG5_CHARSET;
	UpdateRefWin();
}

void CTimeMDoc::OnGb2312Charset()
{
	m_dwCharsetForDisp = GB2312_CHARSET;
	UpdateRefWin();
}

void CTimeMDoc::OnCharsetDispUpdateUI(CCmdUI* pCmdUI)
{
	switch(m_dwCharsetForDisp)
	{
	case CHINESEBIG5_CHARSET:
		pCmdUI->SetCheck(pCmdUI->m_nID == ID_BIG5_CHARSET);
		break;
	case GB2312_CHARSET:
		pCmdUI->SetCheck(pCmdUI->m_nID == ID_GB2312_CHARSET);
		break;
	case ANSI_CHARSET:
	default:
		pCmdUI->SetCheck(pCmdUI->m_nID == ID_DEFAULT_CHARSET);
		break;
	}
}

void CTimeMDoc::OnUpdateFileExport(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->Enable(!m_strPathName.IsEmpty());
}

void CTimeMDoc::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	CCPSelDlg dlgSelCP;
	dlgSelCP.m_strCP = m_bUnicode?_T("Unicode"):_T("ANSI");
	if(IDOK == dlgSelCP.DoModal())
	{
		if(dlgSelCP.m_strCP.CompareNoCase(_T("ANSI")) == 0)
		{
			m_bUnicode = FALSE;
		}
		else
		{
			m_bUnicode = TRUE;
		}
		CDocument::OnFileSaveAs();
	}
}

void CTimeMDoc::OnOptionsAsssettings()
{
	CString strExt = PathFindExtension(GetPathName());
	if(strExt.CompareNoCase(_T(".ssa")) == 0)
	{
		CSSASettings dlgSSASettings(FALSE);
		dlgSSASettings.m_strHdr = m_strFmtHeader.IsEmpty()?CTitleParams::GetDefSSAHeader():m_strFmtHeader;
		dlgSSASettings.m_strPreCode = m_strPreCode;
		dlgSSASettings.m_strPostCode = m_strPostCode;
		if(IDOK != dlgSSASettings.DoModal())
			return;
		m_strFmtHeader = dlgSSASettings.m_strHdr;
		m_strPreCode = dlgSSASettings.m_strPreCode;
		m_strPostCode = dlgSSASettings.m_strPostCode;
	}
	else if(strExt.CompareNoCase(_T(".ass")) == 0)
	{
		CSSASettings dlgSSASettings(TRUE);
		dlgSSASettings.m_strHdr = m_strFmtHeader.IsEmpty()?CTitleParams::GetDefASSHeader():m_strFmtHeader;
		dlgSSASettings.m_strPreCode = m_strPreCode;
		dlgSSASettings.m_strPostCode = m_strPostCode;
		if(IDOK != dlgSSASettings.DoModal())
			return;
		m_strFmtHeader = dlgSSASettings.m_strHdr;
		m_strPreCode = dlgSSASettings.m_strPreCode;
		m_strPostCode = dlgSSASettings.m_strPostCode;
	}
}

void CTimeMDoc::OnToolAddauthor()
{
	// TODO: Add your command handler code here
	CMinIntervalSettingDlg dlgInput;
	dlgInput.m_strMinInterval = CTitleHelper::FormatTime(theApp.m_Params.GetMinInterval());
	if(IDCANCEL == dlgInput.DoModal())
		return;
	theApp.m_Params.SetMinInterval(CTitleHelper::GetTimeValue(dlgInput.m_strMinInterval));
	LISTSEL_MAP	mapSelInterval;
	int nItemCnt = m_Action.GetItemCount();
	for(int iPos = 1; iPos < nItemCnt; iPos ++)
	{
		PTITLE_UNIT pUnit1 = m_Action.GetItem(iPos - 1, FALSE);
		PTITLE_UNIT pUnit2 = m_Action.GetItem(iPos, FALSE);
		int nInterval = pUnit2->nStart - pUnit1->nEnd;
		if(nInterval < theApp.m_Params.GetMinInterval())
			continue;
		mapSelInterval.insert(make_pair(iPos, nInterval));
	}

	if(mapSelInterval.size() > 0)
	{
		CInsertIntervalDlg dlgInsert;
		dlgInsert.m_pmapListInterval = &mapSelInterval;
		dlgInsert.m_pDoc = this;
		dlgInsert.DoModal();
	}
	else
	{
		AfxMessageBox(IDS_NOMATCHINTERVAL);
	}
}

void CTimeMDoc::OnTitlePrevline()
{
	CTitleLView* pTitleView = GetTitleView();
	CTitlePaneDlg* pTitlePane = GetDetailWnd();
	if(pTitlePane != NULL)
		pTitlePane->UpdateDocData();

	SetCurrentPos(GetCurrentPos() - 1);
	UpdateRefWin();
}

void CTimeMDoc::OnTitleNextline()
{
	CTitleLView* pTitleView = GetTitleView();
	CTitlePaneDlg* pTitlePane = GetDetailWnd();
	if(pTitlePane != NULL)
		pTitlePane->UpdateDocData();

	SetCurrentPos(GetCurrentPos() + 1);
	UpdateRefWin();
}

void CTimeMDoc::OnUpdateTitlePrevline(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetCurrentPos() > 0);
}

void CTimeMDoc::OnUpdateTitleNextline(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetCurrentPos() < (GetTitleCount() - 1));
}

void CTimeMDoc::OnTitleDelbycond()
{
	// TODO: Add your command handler code here
	CTitleLView* pView = GetTitleView();
	if(pView == NULL)
		return;

	CTitleFilterDlg dlgFilter;
	if(IDOK == dlgFilter.DoModal())
	{
		dlgFilter.m_bEmpty;
		dlgFilter.m_bSpecChar;
		dlgFilter.m_strSpecChar;

		int nPosSave = GetCurrentPos();
		int nCnt = m_Action.GetItemCount();
		m_Action.BeginRecordAction();
		for(int iTitle = nCnt - 1; iTitle != -1; iTitle --)
		{
			PTITLE_UNIT pUnit = m_Action.GetItem(iTitle, FALSE);

			CString strContent = pUnit->content;
			if(dlgFilter.m_bEmpty&&
				strContent.IsEmpty())
			{
				OnTitleDelete(iTitle);
			}
			if(dlgFilter.m_bSpecChar)
			{
				strContent = strContent.TrimLeft(dlgFilter.m_strSpecChar);
				if(strContent.IsEmpty())
				{
					OnTitleDelete(iTitle);
				}
			}
		}
		m_Action.EndRecordAction();

		if(nPosSave > m_Action.GetItemCount() - 1)
		{
			SetCurrentPos(m_Action.GetItemCount() - 1);
		}

		pView->NoSelect();
		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
}

void CTimeMDoc::OnTitleSinglelinetitle()
{
	CTitleLView* pView = GetTitleView();
	if(pView == NULL)
		return;

	DWORD dwSelCnt = pView->GetSelectedCount();
	if(dwSelCnt > 0)
	{
		int nSel;
		m_Action.BeginRecordAction();
		POSITION posSel = pView->GetFirstSelectedItemPosition();
		while(posSel != NULL)
		{
			nSel = pView->GetNextSelectedItem(posSel);
			PTITLE_UNIT pUnit = m_Action.GetItem(nSel);
			pUnit->content.Replace(_T("\r\n"), _T(" "));
		}
		m_Action.EndRecordAction();
		SetModifiedFlagAutoSave();
		UpdateRefWin();
	}
}

void CTimeMDoc::OnFileSplitto()
{
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];

	CSplitNumDlg dlgSplit;
	if(IDOK == dlgSplit.DoModal())
	{
		if(dlgSplit.m_nSplitNum > 1)
		{
			CString strNumFile;
			int iStart = 0;
			int iEnd = 0;
			int nSize = m_Action.GetItemCount();
			int	nBlockSize = (nSize / dlgSplit.m_nSplitNum + 1);
			for(UINT i = 0; i < dlgSplit.m_nSplitNum; i ++)
			{
				_tsplitpath_s(m_strPathName,
					szDrive, _MAX_DRIVE,
					szDir, _MAX_DIR,
					szFName, _MAX_FNAME,
					szExt, _MAX_EXT);

				strNumFile.Format(_T("%s%s%s%d%s"), szDrive, szDir, szFName, i + 1, szExt);

				iEnd = (i+1)*nBlockSize;
				if(iEnd > (nSize - 1))
					iEnd = nSize;
				// Output File;

				CString strExt = PathFindExtension(strNumFile);
				DWORD dwFmt = FMTFLAG_SRT;
				CString strFmtHdr;
				if(strExt.CompareNoCase(_T(".ssa")) == 0)
				{
					dwFmt = FMTFLAG_SSA;
					strFmtHdr = m_strFmtHeader.IsEmpty()?CTitleParams::GetDefSSAHeader():m_strFmtHeader;
				}
				else if(strExt.CompareNoCase(_T(".ass")) == 0)
				{
					dwFmt = FMTFLAG_ASS;
					strFmtHdr = m_strFmtHeader.IsEmpty()?CTitleParams::GetDefASSHeader():m_strFmtHeader;
				}
				CFile fileOut;
				if(fileOut.Open(strNumFile, CFile::modeCreate|CFile::modeWrite))
					CTitleFile::WriteTitleFile(&fileOut, m_Action.GetVtTitle(), strFmtHdr, m_strPreCode, m_strPostCode, dwFmt, m_bUnicode, iStart, iEnd);

				iStart = iEnd;
			}
		}
	}
}

void CTimeMDoc::OnOptionAutosave()
{
	// TODO: Add your command handler code here
	if(theApp.m_Params.IsAutoSave())
		theApp.m_Params.SetAutoSave(FALSE);
	else
		theApp.m_Params.SetAutoSave(TRUE);
	AfxGetApp()->WriteProfileInt( _T("Settings"), _T("IsAutoSave"), theApp.m_Params.IsAutoSave());
}

void CTimeMDoc::OnUpdateOptionAutosave(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(theApp.m_Params.IsAutoSave());
}

void CTimeMDoc::OnUpdateEditReplace(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_dlgReplace.IsWindowVisible());
}

void CTimeMDoc::DoBackup()
{
	// make sure directory exists
	// check backup file number
	// save file data to backup file
	CString strBkPath = theApp.m_Params.GetBakPath();
	if(!PathFileExists(strBkPath))
		SHCreateDirectoryEx(NULL, strBkPath, NULL);

	int nMax = theApp.m_Params.GetMaxBak();

	TCHAR	szTmpPath[MAX_PATH];
	TCHAR	szFile[MAX_PATH];
	TCHAR	szBkFile[MAX_PATH];
	if(GetBakModifiedFlag())
	{
		_tsplitpath_s(GetPathName(), NULL, 0, NULL, 0, szFile, MAX_PATH, NULL, 0);
		PathCombine(szTmpPath, strBkPath, szFile);
		StringCbCopy(szFile, MAX_PATH, szTmpPath);
		StringCbCat(szTmpPath, MAX_PATH, _T(".tmp"));

		CString strBkExt;
		CStringArray astrBakName;
		if(PathFileExists(szTmpPath))
		{
			for(int iBak = 0; iBak < nMax; iBak ++)
			{
				StringCbCopy(szBkFile, MAX_PATH, szFile);
				strBkExt.Format(_T(".%03d"), iBak + 1);
				StringCbCat(szBkFile, MAX_PATH, strBkExt);

				astrBakName.Add(szBkFile);
			}
			CString strNameSave = astrBakName[nMax - 1];
			for(int iBak = nMax - 2; iBak > -1; iBak --)
			{
				if(PathFileExists(astrBakName[iBak]))
				{
					CopyFile(astrBakName[iBak], strNameSave, FALSE);
				}
				strNameSave = astrBakName[iBak];
			}
			CopyFile(szTmpPath, strNameSave, FALSE);
		}
		if(WriteToFile(szTmpPath))
			SetBakModifiedFlag(FALSE);
	}
}

BOOL CTimeMDoc::WriteToFile(LPCTSTR lpszFileName)
{
	CFile fileSave;
	if(fileSave.Open(lpszFileName, CFile::modeCreate|CFile::modeWrite))
	{
		CString strExt = PathFindExtension(GetPathName());
		DWORD dwFmt = FMTFLAG_SRT;
		CString strFmtHdr;
		if(strExt.CompareNoCase(_T(".ssa")) == 0)
		{
			dwFmt = FMTFLAG_SSA;
			strFmtHdr = m_strFmtHeader.IsEmpty()?CTitleParams::GetDefSSAHeader():m_strFmtHeader;
		}
		else if(strExt.CompareNoCase(_T(".ass")) == 0)
		{
			dwFmt = FMTFLAG_ASS;
			strFmtHdr = m_strFmtHeader.IsEmpty()?CTitleParams::GetDefASSHeader():m_strFmtHeader;
		}
		return CTitleFile::WriteTitleFile(&fileSave, m_Action.GetVtTitle(), strFmtHdr, m_strPreCode, m_strPostCode, dwFmt, m_bUnicode);
	}
	return FALSE;
}

HACCEL CTimeMDoc::GetDefaultAccelerator()
{
	CTitlePaneDlg* pTitleWnd = GetDetailWnd();
	if((pTitleWnd != NULL)
		&&pTitleWnd->IsInputFocus())
	{
		CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
		return pWnd->GetDefaultAccelerator();
	}
	return CDocument::GetDefaultAccelerator();
}

/*
void CTimeMDoc::OnFileExportfu()
{
	CFile fileWrite; 
	if(!fileWrite.Open(lpszFileName, CFile::modeWrite|CFile::modeCreate))
		return;

	PTITLE_UNIT pUnit = NULL;
	CString strItem;
	int iSrtCnt = 0;
	for(int iTitle = iStart; iTitle <= iEnd; iTitle ++)
	{
		pUnit = vtTitle[iTitle];
		if(pUnit->content.IsEmpty())
			continue;
		switch(dwFmt)
		{
		case FILE_FORMAT_SRT:
			iSrtCnt ++;
			CTitleHelper::RemoveAssCode(pUnit->content);
			strItem = CTitleHelper::FormatSRTString(iSrtCnt, pUnit);
			break;
		}

		if(!bUnicode)
		{
			CStringA strItemA;
			int nASize = WideCharToMultiByte(nCPCode, 0,
				strItem, strItem.GetLength(),
				NULL, 0, NULL, NULL);
			if(nASize > 0)
			{
				WideCharToMultiByte(nCPCode, 0,
					strItem, strItem.GetLength(),
					strItemA.GetBufferSetLength(nASize), nASize, NULL, NULL);
				fileWrite.Write(strItemA.GetBuffer(), strItemA.GetLength()*sizeof(CHAR));
			}
		}
		else
		{
			fileWrite.Write(strItem.GetBuffer(), strItem.GetLength()*sizeof(WCHAR));
		}
	}
}
*/