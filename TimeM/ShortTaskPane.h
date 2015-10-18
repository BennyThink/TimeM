#pragma once


// CShortTaskPane

class CShortTaskPane : public CMFCTasksPane
{
	DECLARE_DYNAMIC(CShortTaskPane)

public:
	CShortTaskPane();
	virtual ~CShortTaskPane();

protected:
	DECLARE_MESSAGE_MAP()
	void ChangeGlobalFont(int nStep);

protected:
	virtual void OnDrawTasks(CDC* pDC, CRect rectWorkArea);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
