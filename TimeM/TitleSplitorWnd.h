#pragma once

#include "STSplittor.h"
typedef vector<COLORREF> VT_COLORS;

typedef struct tagSplitLine
{
	CString		strLine;
	VT_SPLITOR	vtSplitors;
}SP_LINE, *PSP_LINE;

typedef vector<PSP_LINE> VT_SPLINES;


#define	MSG_NEWSPLIT	(WM_USER+60)
#define	MSG_REMOVESPLIT	(WM_USER+61)

// CTitleSplitorWnd

class CTitleSplitorWnd : public CWnd
{
	DECLARE_DYNAMIC(CTitleSplitorWnd)

public:
	CTitleSplitorWnd();
	virtual ~CTitleSplitorWnd();

	BOOL CreateSplitWnd(CStringArray& astrTitle, const CRect& rcWnd, CWnd* pParent);
	virtual void OnDraw(CDC* pDC);
protected:
	DECLARE_MESSAGE_MAP()

//	MAP_SPLITOR		m_mapSplit;
	VT_SPLINES		m_vtLines;
//	VT_SPLITOR		m_vtSplitor;
	HCURSOR			m_hCursor;
	CSTSplittor*	m_pCapSplit;
	BOOL			m_bCapMouse;
	TEXTMETRIC		m_TextMetrics;
	int				m_nLineSpace;
	LOGFONT			m_lf;
	CPoint			m_ptOrg;
	VT_COLORS		m_vtColors;
	BOOL			m_bClickNew;
public:
	void SetFontLog(LPLOGFONT plf);
	void GetFontLog(LPLOGFONT plf);
	BOOL InitMetrics();
	void SetScroll();
	void SetLineSpace(int nSpace);
	void SetClickNew(BOOL bClickNew = TRUE){m_bClickNew = bClickNew;};
	BOOL IsClickNew(){return m_bClickNew;};
	void InsertSortPos(CSTSplittor* pSplitNew);
	const VT_SPLINES& GetSplitLines(){return m_vtLines;};

	CSTSplittor* AddNewSplit(int nLine);
	void RemoveCurrSplitor();
	void Remove(CSTSplittor* pSplit);
	void Break(CSTSplittor* pSplit);
	void RemoveAllSplitor();

	CSTSplittor* HitSplittors(const CPoint& point);
	void TryMoveSplit(CSTSplittor* pSplit, int nPos, int nCharPos);
	void MoveInText(CString& strText, int nXPos);
	int GetLinesCount();
	int GetLineHeight();
	int SnapXPosToTextSplit(CString& strText, int& nXPos);
	int SnapYosToLine(int nYPos);
	void GetLineText(int nLine, CString& strText);

	int GetMaxWidth();
	void GetDrawArea(LPRECT prcRect);

	COLORREF GetLineBkColor(int nLine);
	void SetColors(const VT_COLORS& vtColors);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
