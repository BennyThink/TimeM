
// TimeM.h : TimeM Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "TitleParams.h"
#include "TitleUnit.h"


// CTimeMApp:
// �йش����ʵ�֣������ TimeM.cpp
//

class CTimeMApp : public CWinAppEx
{
public:
	CTimeMApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
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

// ʵ��
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg BOOL OnOpenRecentFile(UINT nID);
	DECLARE_MESSAGE_MAP()
};

extern CTimeMApp theApp;
