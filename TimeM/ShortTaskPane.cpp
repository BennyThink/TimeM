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
	int nGroup = AddGroup(_T("字幕文件"));
	AddTask(nGroup, _T("打开字幕"), 0, ID_FILE_OPEN);
	AddTask(nGroup, _T("打开视频"), 1, ID_VIDEO_OPEN);
	AddTask(nGroup, _T("新字幕"), 2, ID_FILE_NEW);
	AddTask(nGroup, _T("导入文本"), 3, ID_FILE_IMPORT);
	AddTask(nGroup, _T("追加字幕"), 4, ID_FILE_APPEND);
	AddTask(nGroup, _T("CC处理"), 5, ID_TOOL_PREPROC);
	AddTask(nGroup, _T("输出字幕"), 6, ID_FILE_EXPORT);
	AddTask(nGroup, _T("保存字幕"), 18, ID_FILE_SAVE);

	nGroup = AddGroup(_T("字幕编辑"));
	AddTask(nGroup, _T("合并至单行尾部"), 7, ID_TITLE_CBONELINE);
	AddTask(nGroup, _T("合并至新行"), 8, ID_TITLE_CBMULTILINE);
	AddTask(nGroup, _T("拆分字幕"), 9, ID_TITLE_BREAKBYUSER);

	nGroup = AddGroup(_T("时间轴编辑"));
	AddTask(nGroup, _T("对齐当前行"), 10, ID_TITLE_SNAPLINE);
	AddTask(nGroup, _T("对齐后续行"), 11, ID_TITLE_SNAPAFTER);
	AddTask(nGroup, _T("对齐全部行"), 12, ID_TITLE_SNAPALL);
	AddTask(nGroup, _T("对齐至前标记"), 13, ID_TITLE_SNAPPREVMARK);
	AddTask(nGroup, _T("对齐前后标记行"), 14, ID_TITLE_SNAPBETWEENTWOMARK);
	AddTask(nGroup, _T("平移时间"), 15, ID_TITLE_MOVETIME);
	AddTask(nGroup, _T("双语合并"), 16, ID_TOOL_CMBYUSER);
	AddTask(nGroup, _T("检查除错"), 17, ID_TOOL_CHECK);

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
