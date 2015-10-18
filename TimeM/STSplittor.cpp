#include "StdAfx.h"
#include "STSplittor.h"

CSTSplittor::CSTSplittor(void)
{
	m_clrPen = RGB(255, 128, 0);
	m_clrFocus = RGB(0, 0, 255);
	m_nWidth = DEF_SLT_WIDTH;
	m_nHeight = DEF_SLT_HEIGHT;
	m_rcBound = CRect(0, 0, m_nWidth, m_nHeight);
	m_bFocus = FALSE;
	m_nLineNum = 0;
}

CSTSplittor::~CSTSplittor(void)
{
}

const CRect& CSTSplittor::GetBound()
{
	return m_rcBound;
}

BOOL CSTSplittor::HitTest(const CPoint& point)
{
	CRect rcTest = m_rcBound;
	rcTest.InflateRect(1, 0);
	return rcTest.PtInRect(point);
}

void CSTSplittor::Draw(CDC* pDC)
{
	if(m_bFocus)
		pDC->FillSolidRect(m_rcBound, m_clrFocus);
	else
		pDC->FillSolidRect(m_rcBound, m_clrPen);
}

void CSTSplittor::VertMoveTo(int nPos)
{
	m_rcBound.top = nPos;
	m_rcBound.bottom = nPos + m_nHeight;
}

void CSTSplittor::HorzMoveTo(int nPos)
{
	m_rcBound.left = nPos;
	m_rcBound.right = nPos + m_nWidth;
}

void CSTSplittor::SetObjSize(int cx, int cy)
{
	m_nWidth = cx;
	m_nHeight = cy;
	m_rcBound.right = m_rcBound.left + m_nWidth;
	m_rcBound.bottom = m_rcBound.top + m_nHeight;
}
