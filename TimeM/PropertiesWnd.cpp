
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "TimeM.h"
#include "TimeMDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_WM_DESTROY()

	ON_COMMAND(ID_VIDEO_OPEN, &CPropertiesWnd::OnVFileOpen)
	ON_COMMAND(ID_VIDEO_PLAY_PAUSE, &CPropertiesWnd::OnVideoPlayPause)
	ON_COMMAND(ID_VIDEO_STOP, &CPropertiesWnd::OnVideoStop)
	ON_COMMAND(ID_VIDEO_STEPONE, &CPropertiesWnd::OnVideoStepone)
	ON_COMMAND(ID_VIDEO_FORWARD1S, &CPropertiesWnd::OnVideoForward1s)
	ON_COMMAND(ID_VIDEO_BACK1S, &CPropertiesWnd::OnVideoBack1s)
	ON_COMMAND(ID_VIDEO_FORWARD5S, &CPropertiesWnd::OnVideoForward5s)
	ON_COMMAND(ID_VIDEO_BACK5S, &CPropertiesWnd::OnVideoBack5s)
	ON_COMMAND(ID_VIDEO_SYNCTITLE, &CPropertiesWnd::OnVideoSynctitle)
	ON_COMMAND(ID_TITLE_SNAPLINE, &CPropertiesWnd::OnTitleSnapline)
	ON_COMMAND(ID_TITLE_SNAPAFTER, &CPropertiesWnd::OnTitleSnapafter)
	ON_COMMAND(ID_TITLE_SNAPALL, &CPropertiesWnd::OnTitleSnapall)
	ON_COMMAND(ID_TITLE_SNAPLINEEND, &CPropertiesWnd::OnTitleSnaplineend)
	ON_COMMAND(ID_TITLE_SNAPLINESTART, &CPropertiesWnd::OnTitleSnaplinestart)
	ON_COMMAND(ID_TITLE_INSERT, &CPropertiesWnd::OnTitleInsert)
	ON_COMMAND(ID_TITLE_MULTIINSERT, &CPropertiesWnd::OnTitleMultiinsert)
	ON_COMMAND(ID_OPTION_AUTOSAVE, &CPropertiesWnd::OnOptionAutosave)
	ON_UPDATE_COMMAND_UI(ID_OPTION_AUTOSAVE, &CPropertiesWnd::OnUpdateOptionAutosave)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_SYNCTITLE, &CPropertiesWnd::OnUpdateVideoSynctitle)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TITLE_SNAPLINE, ID_TITLE_SNAPBETWEENTWOMARK, &CPropertiesWnd::OnTitleSnapUpdateCmdUIRange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.bottom - cyTlb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

	if(::IsWindow(m_wndPlay.m_hWnd))
	{
		CRect rcPos;
		GetPlayerRect(rcPos);
		m_wndPlay.MoveWindow(rcPos);
	}
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_MAINFRAME);
	m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDR_MAINFRAME : IDR_MAINFRAME, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(TRUE);

	AdjustLayout();
	CreateAxWin();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
}

BOOL CPropertiesWnd::CreateAxWin()
{
	/////////////////
	//
	BOOL bRet = TRUE;
	HRESULT  hr;
	AtlAxWinInit();
	CComPtr<IAxWinHostWindow>	spHost;
    m_dwAdviseCookie = 0;

	CRect rcClient;
	GetPlayerRect(rcClient);
	m_wndPlay.Create(m_hWnd, rcClient, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0);

	hr = m_wndPlay.QueryHost(&spHost);
	if(SUCCEEDED(hr))
	{
		hr = spHost->CreateControl(CComBSTR(_T("{6BF52A52-394A-11d3-B153-00C04F79FAA6}")), m_wndPlay, 0);
	}

	if(SUCCEEDED(hr))
	{
		m_wndPlay.QueryControl(&m_spWMPPlayer);
		m_Player.AttachDispatch(m_spWMPPlayer);
		m_Player.get_settings().put_autoStart(FALSE);
	}
	else
	{
		return FALSE;
	}

	////////////////
	//
    CComPtr<IConnectionPointContainer>  spConnectionContainer;
    CComPtr<IWMPEvents>                 spEventListener;

    hr = CComWMPEventDispatch::CreateInstance(&m_pEventListener);
	if(SUCCEEDED(hr))
	{
		spEventListener = m_pEventListener;

		hr = m_spWMPPlayer->QueryInterface(&spConnectionContainer);
		if(SUCCEEDED(hr))
		{
			// See if OCX supports the IWMPEvents interface
			hr = spConnectionContainer->FindConnectionPoint(__uuidof(IWMPEvents), &m_spConnectionPoint);
			if (FAILED(hr))
			{
				// If not, try the _WMPOCXEvents interface, which will use IDispatch
				hr = spConnectionContainer->FindConnectionPoint(__uuidof(_WMPOCXEvents), &m_spConnectionPoint);
			}

			if(SUCCEEDED(hr))
			{
				hr = m_spConnectionPoint->Advise(spEventListener, &m_dwAdviseCookie);
				m_pEventListener->SetPlayer(&m_Player);
			}
		}
	}

	return bRet;
}

void CPropertiesWnd::CloseAxWin()
{
    // stop listening to events

    if (m_spConnectionPoint)
    {
        if (0 != m_dwAdviseCookie)
            m_spConnectionPoint->Unadvise(m_dwAdviseCookie);
        m_spConnectionPoint.Release();
    }

	m_Player.DetachDispatch();
    // close the OCX

    if (m_spWMPPlayer)
    {
        m_spWMPPlayer->close();
        m_spWMPPlayer.Release();
    }
}

void CPropertiesWnd::GetPlayerRect(LPRECT lpRect)
{
	GetClientRect(lpRect);
	CSize sizeToolBar = m_wndToolBar.CalcFixedLayout(FALSE, TRUE);
	lpRect->bottom -= sizeToolBar.cy;
}

double CPropertiesWnd::IntToDbl(int nValue)
{
	return ((double)nValue)/1000;
}

int CPropertiesWnd::DblToInt(double dblValue)
{
	return (int)(dblValue * 1000);
}

HDC CPropertiesWnd::GetVideoDC()
{
	return m_wndPlay.GetDC();
}

void CPropertiesWnd::ReleaseDC(HDC hDC)
{
	m_wndPlay.ReleaseDC(hDC);
}

//////////////////////////
int	CPropertiesWnd::GetMediaDuration()
{
	CWMPMedia3 pMedia = m_Player.get_currentMedia();
	return DblToInt(pMedia.get_duration());
}

int	CPropertiesWnd::GetCurrentPosition()
{
	CWMPControls3 ctrlPlay;
	long nState = GetPlayState();
	switch(nState)
	{
	case wmppsPlaying:
	case wmppsScanForward:
	case wmppsScanReverse:
	case wmppsPaused:
		ctrlPlay = m_Player.get_controls();
		return DblToInt(ctrlPlay.get_currentPosition());
	default:
		return -1;
	}
}

void CPropertiesWnd::SetCurrentPosition(int nPos)
{
	CWMPControls3 ctrlPlay = m_Player.get_controls();
	ctrlPlay.put_currentPosition(IntToDbl(nPos));
}

long CPropertiesWnd::GetPlayState()
{
	return m_Player.get_playState();
}

BOOL CPropertiesWnd::VideoFileOpen(LPCTSTR lpszFile)
{
	m_Player.put_URL(lpszFile);
	return TRUE;
}

void CPropertiesWnd::OnVFileOpen()
{
	OnVideoFileOpen();
}

BOOL CPropertiesWnd::OnVideoFileOpen()
{
	// TODO: Add your command handler code here
	// MKV, TS, MP4, RMVB，MPG, MPEG
	TCHAR szFilter[] = _T("常用媒体格式(*.AVI;*.MKV;*.TS;*.MP4;*.RMVB;*.MPG;*.MPEG)|*.AVI;*.MKV;*.TS;*.MP4;*.RMVB;*.MPG;*.MPEG|All Files (*.*)|*.*||");
	CFileDialog dlgOpen(TRUE, 0, 0, 0, szFilter);
	if(IDOK == dlgOpen.DoModal())
	{
		m_Player.put_URL(dlgOpen.GetPathName());
		return TRUE;
	}
	return FALSE;
}

void CPropertiesWnd::OnVideoPlayPause()
{
	// TODO: Add your command handler code here
	CWMPControls3 ctrlPlay = m_Player.get_controls();

	long lState = m_Player.get_playState();
	switch(lState)
	{
	case wmppsStopped:
	case wmppsPaused:
		ctrlPlay.play();
		break;
	case wmppsPlaying:
		ctrlPlay.pause();
		break;
	}
}

void CPropertiesWnd::OnVideoStop()
{
	// TODO: Add your command handler code here
	CWMPControls3 ctrlPlay = m_Player.get_controls();
	ctrlPlay.stop();
}

void CPropertiesWnd::OnVideoStepone()
{
	// TODO: Add your command handler code here
	CWMPControls3 ctrlPlay = m_Player.get_controls();
	ctrlPlay.step(1);
}

void CPropertiesWnd::OnVideoForward1s()
{
	// TODO: Add your command handler code here
	CWMPControls3 ctrlPlay = m_Player.get_controls();
	double dblPos = ctrlPlay.get_currentPosition();
	dblPos += 1;
	ctrlPlay.put_currentPosition(dblPos);
}

void CPropertiesWnd::OnVideoBack1s()
{
	// TODO: Add your command handler code here
	CWMPControls3 ctrlPlay = m_Player.get_controls();
	double dblPos = ctrlPlay.get_currentPosition();
	dblPos -= 1;
	ctrlPlay.put_currentPosition(dblPos);
}

void CPropertiesWnd::OnVideoForward5s()
{
	// TODO: Add your command handler code here
	CWMPControls3 ctrlPlay = m_Player.get_controls();
	double dblPos = ctrlPlay.get_currentPosition();
	dblPos += 5;
	ctrlPlay.put_currentPosition(dblPos);
}

void CPropertiesWnd::OnVideoBack5s()
{
	// TODO: Add your command handler code here
	CWMPControls3 ctrlPlay = m_Player.get_controls();
	double dblPos = ctrlPlay.get_currentPosition();
	dblPos -= 5;
	ctrlPlay.put_currentPosition(dblPos);
}

void CPropertiesWnd::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: Add your message handler code here
	CloseAxWin();
}

void CPropertiesWnd::OnVideoSynctitle()
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
				pDoc->OnVideoSynctitle();
		}
	}
}

void CPropertiesWnd::OnUpdateOptionAutosave(CCmdUI *pCmdUI)
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
				pDoc->OnUpdateOptionAutosave(pCmdUI);
		}
	}
}

void CPropertiesWnd::OnUpdateVideoSynctitle(CCmdUI *pCmdUI)
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
				pDoc->OnUpdateVideoSynctitle(pCmdUI);
		}
	}
}

void CPropertiesWnd::OnTitleSnapUpdateCmdUIRange(CCmdUI *pCmdUI)
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
				pDoc->OnTitleSnapUpdateCmdUIRange(pCmdUI);
		}
	}
}

void CPropertiesWnd::OnTitleSnapline()
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
				pDoc->OnTitleSnapline();
		}
	}
}

void CPropertiesWnd::OnTitleSnapafter()
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
				pDoc->OnTitleSnapafter();
		}
	}
}

void CPropertiesWnd::OnTitleSnapall()
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
				pDoc->OnTitleSnapall();
		}
	}
}

void CPropertiesWnd::OnTitleSnaplineend()
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
				pDoc->OnTitleSnaplineend();
		}
	}
}

void CPropertiesWnd::OnTitleSnaplinestart()
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
				pDoc->OnTitleSnaplinestart();
		}
	}
}

void CPropertiesWnd::OnTitleInsert()
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
				pDoc->OnTitleInsert();
		}
	}
}

void CPropertiesWnd::OnTitleMultiinsert()
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
				pDoc->OnTitleMultiinsert();
		}
	}
}

void CPropertiesWnd::OnOptionAutosave()
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
				pDoc->OnOptionAutosave();
		}
	}
}
