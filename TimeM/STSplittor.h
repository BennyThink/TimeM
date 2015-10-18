#pragma once

#define	DEF_SLT_WIDTH	2
#define	DEF_SLT_HEIGHT	16

class CSTSplittor
{
public:
	CSTSplittor(void);
	~CSTSplittor(void);
protected:
	CRect		m_rcBound;
	COLORREF	m_clrPen;
	COLORREF	m_clrFocus;
	BOOL		m_bFocus;
	int			m_nWidth;
	int			m_nHeight;
	int			m_nLineNum;
	int			m_nCharNum;
public:
	void SetCharNum(int nChar){m_nCharNum = nChar;};
	int GetCharNum(){return m_nCharNum;};
	void SetLineNum(int nLine){m_nLineNum = nLine;};
	int GetLineNum(){return m_nLineNum;};

	const CRect& GetBound();
	BOOL HitTest(const CPoint& point);
	void Draw(CDC* pDC);

	void SetObjSize(int cx, int cy);

	void SetObjFocus(BOOL bFocus = TRUE){m_bFocus = bFocus;};

	void HorzMoveTo(int nPos);
	void VertMoveTo(int nPos);
	int GetPos(){return m_rcBound.left;};
};

typedef vector<CSTSplittor*> VT_SPLITOR;
typedef multimap<int, CSTSplittor*> MAP_SPLITOR;
