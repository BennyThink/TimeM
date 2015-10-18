// ShortTaskPane.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "ShortTaskPane.h"


// CShortTaskPane

IMPLEMENT_DYNAMIC(CShortTaskPane, CMFCTasksPane)

CShortTaskPane::CShortTaskPane()
{

}

CShortTaskPane::~CShortTaskPane()
{
}

BEGIN_MESSAGE_MAP(CShortTaskPane, CMFCTasksPane)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CShortTaskPane message handlers

int CShortTaskPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCTasksPane::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetIconsList (IDB_BMP_TASKS, 24);

	// TODO:  Add your specialized creation code here
	int nGroup = AddGroup(_T("��Ļ�ļ�"));
	AddTask(nGroup, _T("����Ļ"), 0, ID_FILE_OPEN);
	AddTask(nGroup, _T("����Ƶ"), 1, ID_VIDEO_OPEN);
	AddTask(nGroup, _T("����Ļ"), 2, ID_FILE_NEW);
	AddTask(nGroup, _T("�����ı�"), 3, ID_FILE_IMPORT);
	AddTask(nGroup, _T("׷����Ļ"), 4, ID_FILE_APPEND);
	AddTask(nGroup, _T("CC����"), 5, ID_TOOL_PREPROC);
	AddTask(nGroup, _T("�����Ļ"), 6, ID_FILE_EXPORT);
	AddTask(nGroup, _T("������Ļ"), 18, ID_FILE_SAVE);

	nGroup = AddGroup(_T("��Ļ�༭"));
	AddTask(nGroup, _T("�ϲ�������β��"), 7, ID_TITLE_CBONELINE);
	AddTask(nGroup, _T("�ϲ�������"), 8, ID_TITLE_CBMULTILINE);
	AddTask(nGroup, _T("�����Ļ"), 9, ID_TITLE_BREAKBYUSER);

	nGroup = AddGroup(_T("ʱ����༭"));
	AddTask(nGroup, _T("���뵱ǰ��"), 10, ID_TITLE_SNAPLINE);
	AddTask(nGroup, _T("���������"), 11, ID_TITLE_SNAPAFTER);
	AddTask(nGroup, _T("����ȫ����"), 12, ID_TITLE_SNAPALL);
	AddTask(nGroup, _T("������ǰ���"), 13, ID_TITLE_SNAPPREVMARK);
	AddTask(nGroup, _T("����ǰ������"), 14, ID_TITLE_SNAPBETWEENTWOMARK);
	AddTask(nGroup, _T("ƽ��ʱ��"), 15, ID_TITLE_MOVETIME);
	AddTask(nGroup, _T("˫��ϲ�"), 16, ID_TOOL_CMBYUSER);
	AddTask(nGroup, _T("������"), 17, ID_TOOL_CHECK);

//	SetTasksIconVertOffset (4);

	return 0;
}

void CShortTaskPane::ChangeGlobalFont(int nStep)
{
	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);
	afxGlobalData.fontRegular.DeleteObject();
	lf.lfHeight -= nStep;
	afxGlobalData.fontRegular.CreateFontIndirect(&lf);

	afxGlobalData.fontUnderline.GetLogFont(&lf);
	afxGlobalData.fontUnderline.DeleteObject();
	lf.lfHeight -= nStep;
	afxGlobalData.fontUnderline.CreateFontIndirect(&lf);

	afxGlobalData.fontBold.GetLogFont(&lf);
	afxGlobalData.fontBold.DeleteObject();
	lf.lfHeight -= nStep;
	afxGlobalData.fontBold.CreateFontIndirect(&lf);
}

void CShortTaskPane::OnDrawTasks(CDC* pDC, CRect rectWorkArea)
{
//	ChangeGlobalFont(1);
	CMFCTasksPane::OnDrawTasks(pDC, rectWorkArea);
//	ChangeGlobalFont(-1);
}
