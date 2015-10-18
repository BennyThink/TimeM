
#pragma once
#include "wmp.h"

#include ".\WMPImport\CWMPPlayer4.h"
#include ".\WMPImport\CWMPControls3.h"
#include ".\WMPImport\CWMPMedia3.h"
#include ".\WMPImport\CWMPSettings2.h"
#include ".\WMPImport\CWMPEventDispatch.h"

#include <atlwin.h>

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
// Construction
public:
	CPropertiesWnd();

	void AdjustLayout();

// Attributes
public:

	void GetPlayerRect(LPRECT lpRect);
	BOOL CreateAxWin();
	void CloseAxWin();

	double IntToDbl(int nValue);
	int DblToInt(double dblValue);

	int	GetMediaDuration();
	int	GetCurrentPosition();
	void SetCurrentPosition(int nPos);
	long GetPlayState();

	BOOL VideoFileOpen(LPCTSTR lpszFile);
	HDC GetVideoDC();
	void ReleaseDC(HDC hDC);
protected:
	CPropertiesToolBar			m_wndToolBar;
	CWMPPlayer4					m_Player;
	CAxWindow					m_wndPlay;
    CComPtr<IWMPPlayer>			m_spWMPPlayer;
    CComPtr<IConnectionPoint>	m_spConnectionPoint;
    CComWMPEventDispatch*		m_pEventListener;
    DWORD						m_dwAdviseCookie;

// Implementation
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

public:
	BOOL OnVideoFileOpen();
	afx_msg void OnVFileOpen();
	afx_msg void OnVideoPlayPause();
	afx_msg void OnVideoStop();
	afx_msg void OnVideoStepone();
	afx_msg void OnVideoForward1s();
	afx_msg void OnVideoBack1s();
	afx_msg void OnVideoForward5s();
	afx_msg void OnVideoBack5s();
	afx_msg void OnVideoSynctitle();
	afx_msg void OnTitleSnapline();
	afx_msg void OnTitleSnapafter();
	afx_msg void OnTitleSnapall();
	afx_msg void OnTitleSnaplineend();
	afx_msg void OnTitleSnaplinestart();
	afx_msg void OnTitleInsert();
	afx_msg void OnTitleMultiinsert();
	afx_msg void OnOptionAutosave();
	afx_msg void OnUpdateVideoSynctitle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOptionAutosave(CCmdUI *pCmdUI);
	afx_msg void OnTitleSnapUpdateCmdUIRange(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};

