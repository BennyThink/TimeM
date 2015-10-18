
// TimeM.h : TimeM 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "TitleParams.h"
#include "TitleUnit.h"


// CTimeMApp:
// 有关此类的实现，请参阅 TimeM.cpp
//

class CTimeMApp : public CWinAppEx
{
public:
	CTimeMApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
public:
	CTitleParams	m_Params;
	UINT	m_nAppLook;
	BOOL	m_bHiColorIcons;
	BOOL	m_bInProcess;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	void UpdateGlobalFonts(int nStep);

	void SaveStdProfileSettings();  // save options to .INI file
	void LoadStdProfileSettings(UINT nMaxMRU = _AFX_MRU_COUNT);
	void LoadTitleSettings();
	virtual int ExitInstance(); // return app exit code
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);

	BOOL	IsFileVideo(LPCTSTR lpszFile);

// 实现
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg BOOL OnOpenRecentFile(UINT nID);
	DECLARE_MESSAGE_MAP()
};

extern CTimeMApp theApp;
