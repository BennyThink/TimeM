// TitleSplitorWnd.cpp : implementation file
//

#include "stdafx.h"
#include "TitleSplitorWnd.h"


// CTitleSplitorWnd

IMPLEMENT_DYNAMIC(CTitleSplitorWnd, CWnd)

/////////////////////////////////////////////////////////
//
CTitleSplitorWnd::CTitleSplitorWnd()
{
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_pCapSplit = NULL;
	m_bCapMouse = FALSE;
	ZeroMemory(&m_TextMetrics, sizeof(TEXTMETRIC));
	CFont* pfonGui = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pfonGui->GetLogFont(&m_lf);
	m_nLineSpace = 2;
	m_ptOrg = CPoint(0, 0);
	m_bClickNew = FALSE;
}

CTitleSplitorWnd::~CTitleSplitorWnd()
{
	RemoveAllSplitor();
	int nSize = m_vtLines.size();
	for(int iLine = 0; iLine < nSize; iLine ++)
	{
		delete m_vtLines[iLine];
	}
	m_vtLines.clear();
}

/////////////////////////////////////////////////////////
//	功能函数
void CTitleSplitorWnd::SetFontLog(LPLOGFONT plf)
{
	CopyMemory(&m_lf, plf, sizeof(LOGFONT));
}

void CTitleSplitorWnd::GetFontLog(LPLOGFONT plf)
{
	CopyMemory(plf, &m_lf, sizeof(LOGFONT));
}

void CTitleSplitorWnd::SetLineSpace(int nSpace)
{
	m_nLineSpace = nSpace;
}

int CTitleSplitorWnd::GetMaxWidth()
{
	int nStrCnt = (int)m_vtLines.size();

	CFont fonDraw;
	fonDraw.CreateFontIndirect(&m_lf);
	CWindowDC dcCalc(GetDesktopWindow());
	CFont* pfonOld = dcCalc.SelectObject(&fonDraw);

	int nWidth = 0;
	int nTmp = 0;
	for(int iStr = 0; iStr < nStrCnt; iStr ++)
	{
		nTmp = 65535;
		SnapXPosToTextSplit(m_vtLines[iStr]->strLine, nTmp);
		if(nWidth < nTmp)
			nWidth = nTmp;
	}

	dcCalc.SelectObject(pfonOld);
	fonDraw.DeleteObject();
	return nWidth + 20;
}

void CTitleSplitorWnd::SetScroll()
{
	// Set VScrollInfo
	int nLineHeight = GetLineHeight();
	if(0 == nLineHeight)
		return;
	int nLineCnt = GetLinesCount();
	CRect rcClient;
	GetClientRect(rcClient);

	SCROLLINFO si;
	GetScrollInfo(SB_VERT, &si);
	si.fMask = SIF_ALL;
	si.nMax = nLineCnt;
	si.nMin = 0;
	si.nPage = rcClient.Height()/nLineHeight;
	SetScrollInfo(SB_VERT, &si);

	// Set HScrollInfo

	GetScrollInfo(SB_HORZ, &si);
	si.fMask = SIF_ALL;
	si.nMax = GetMaxWidth();
	si.nMin = 0;
	si.nPage = rcClient.Width();
	SetScrollInfo(SB_HORZ, &si);
}

void CTitleSplitorWnd::GetDrawArea(LPRECT prcRect)
{
	GetClientRect(prcRect);
	int nLineHeight = GetLineHeight();
	if(0 == nLineHeight)
		return;
	int nLineCnt = GetLinesCount();
	int nMaxWT = GetMaxWidth();

	prcRect->top = 0;
	prcRect->left = 0;
	if(prcRect->right < nMaxWT)
		prcRect->right = nMaxWT + 40;
	if(prcRect->bottom < nLineHeight*(nLineCnt + 2))
		prcRect->bottom = nLineHeight*(nLineCnt + 2);
}


BOOL CTitleSplitorWnd::InitMetrics()
{
	CWindowDC dcCalc(GetDesktopWindow());
	CFont fonDisp;
	fonDisp.CreateFontIndirect(&m_lf);

	CFont* pOldFont = dcCalc.SelectObject(&fonDisp);
	dcCalc.GetTextMetrics(&m_TextMetrics);
	dcCalc.SelectObject(pOldFont);

	fonDisp.DeleteObject();
	return TRUE;
}

void CTitleSplitorWnd::SetColors(const VT_COLORS& vtColors)
{
	m_vtColors = vtColors;
}

COLORREF CTitleSplitorWnd::GetLineBkColor(int nLine)
{
	int nSize = m_vtColors.size();
	if(nLine < nSize)
	{
		return m_vtColors[nLine];
	}
	return GetSysColor(COLOR_WINDOW);
}

void CTitleSplitorWnd::OnDraw(CDC* pDC)
{
	CRect rcClient;
	GetDrawArea(&rcClient);
	CFont fonDraw;
	fonDraw.CreateFontIndirect(&m_lf);
	CFont* pfonOld = pDC->SelectObject(&fonDraw);
	pDC->FillSolidRect(rcClient, GetSysColor(COLOR_WINDOW));

	CString strText;
	int nStrCnt = (int)m_vtLines.size();
	int nVertStart = m_TextMetrics.tmExternalLeading + m_nLineSpace;
	for(int iStr = 0; iStr < nStrCnt; iStr ++)
	{
		strText = m_vtLines[iStr]->strLine;

		int*	pADist = new int[strText.GetLength()];
		ZeroMemory(pADist, sizeof(int)*strText.GetLength());
		for(int iCnt = 0; iCnt < strText.GetLength(); iCnt ++)
		{
			pDC->GetCharWidth(strText.GetAt(iCnt), strText.GetAt(iCnt), pADist + iCnt);
			pADist[iCnt] += DEF_SLT_WIDTH;
		}

		CRect rcLine;
		rcLine.top = iStr*GetLineHeight();
		rcLine.bottom = (iStr + 1)*GetLineHeight();
		rcLine.left = rcClient.left;
		rcLine.right = rcClient.right;
		pDC->FillSolidRect(rcLine, GetLineBkColor(iStr));
		pDC->ExtTextOut(DEF_SLT_WIDTH, nVertStart, ETO_CLIPPED, rcClient, strText, pADist);

		delete [] pADist;

		int nSize = (int)m_vtLines[iStr]->vtSplitors.size();
		for(int iSplit = 0; iSplit < nSize; iSplit ++)
		{
			CSTSplittor* pSplit = m_vtLines[iStr]->vtSplitors[iSplit];
			pSplit->Draw(pDC);
		}
		nVertStart += GetLineHeight();
	}

	pDC->SelectObject(pfonOld);
	fonDraw.DeleteObject();
}

void CTitleSplitorWnd::RemoveAllSplitor()
{
	int nSizeLine = (int)m_vtLines.size();
	for(int iLine = 0; iLine < nSizeLine; iLine ++)
	{
		int nSize = (int)m_vtLines[iLine]->vtSplitors.size();
		for(int iSplit = 0; iSplit < nSize; iSplit ++)
		{
			delete m_vtLines[iLine]->vtSplitors[iSplit];
		}
		m_vtLines[iLine]->vtSplitors.clear();
	}
	if(IsWindow(m_hWnd))
	{
		Invalidate();
	}
}

void CTitleSplitorWnd::RemoveCurrSplitor()
{
	if(NULL == m_pCapSplit)
		return;
	Remove(m_pCapSplit);
	m_pCapSplit = NULL;
	if(IsWindow(m_hWnd))
	{
		Invalidate();
	}
}

CSTSplittor* CTitleSplitorWnd::AddNewSplit(int nLine)
{
	if((nLine > -1)&&(nLine < GetLinesCount()))
	{
		CSTSplittor* pNewSplit = new CSTSplittor();
		pNewSplit->SetObjSize(DEF_SLT_WIDTH, GetLineHeight());
		pNewSplit->VertMoveTo(m_TextMetrics.tmExternalLeading + nLine * GetLineHeight());
		pNewSplit->SetLineNum(nLine);
		pNewSplit->SetCharNum(0);

		m_vtLines[nLine]->vtSplitors.push_back(pNewSplit);
		if(IsWindow(m_hWnd))
		{
			Invalidate();
		}
		CWnd* pParent = GetParent();
		if(NULL != pParent)
			pParent->PostMessage(MSG_NEWSPLIT, 0, 0);
		return pNewSplit;
	}
	return NULL;
}

void CTitleSplitorWnd::Break(CSTSplittor* pSplit)
{
	int nLine = pSplit->GetLineNum();
	if((nLine > -1)&&(nLine < GetLinesCount()))
	{
		int nSize = (int)m_vtLines[nLine]->vtSplitors.size();
		for(int iSplit = 0; iSplit < nSize; iSplit ++)
		{
			if(m_vtLines[nLine]->vtSplitors[iSplit] == pSplit)
			{
				m_vtLines[nLine]->vtSplitors.erase(m_vtLines[nLine]->vtSplitors.begin() + iSplit);
				break;
			}
		}
	}
}

void CTitleSplitorWnd::Remove(CSTSplittor* pSplit)
{
	int nLine = pSplit->GetLineNum();
	if((nLine > -1)&&(nLine < GetLinesCount()))
	{
		int nSize = (int)m_vtLines[nLine]->vtSplitors.size();
		for(int iSplit = 0; iSplit < nSize; iSplit ++)
		{
			if(m_vtLines[nLine]->vtSplitors[iSplit] == pSplit)
			{
				delete pSplit;
				m_vtLines[nLine]->vtSplitors.erase(m_vtLines[nLine]->vtSplitors.begin() + iSplit);
				CWnd* pParent = GetParent();
				if(NULL != pParent)
					pParent->PostMessage(MSG_REMOVESPLIT, 0, 0);

				break;
			}
		}
	}
}

int CTitleSplitorWnd::SnapXPosToTextSplit(CString& strText, int& nXPos)
{
	CWindowDC dcCalc(this);
	CFont fonDraw;
	fonDraw.CreateFontIndirect(&m_lf);
	CFont* pfonOld = dcCalc.SelectObject(&fonDraw);

	int nRet = -1;
	int nPos = 0;
	int* pADist = new int[strText.GetLength()];
	ZeroMemory(pADist, sizeof(int)*strText.GetLength());

	int iCnt = 0;
	for(; iCnt < strText.GetLength(); iCnt ++)
	{
		dcCalc.GetCharWidth(strText.GetAt(iCnt), strText.GetAt(iCnt), pADist + iCnt);
		pADist[iCnt] += DEF_SLT_WIDTH;
		nPos += pADist[iCnt];
		if(nPos > nXPos)
		{
			if((nPos - nXPos) > (nXPos - nPos + pADist[iCnt]))
			{
				nRet = nPos - pADist[iCnt];
			}
			else
			{
				nRet = nPos;
				iCnt ++;
			}
			break;
		}
	}
	if(nXPos > nPos)
		nRet = nPos;
	delete [] pADist;
	dcCalc.SelectObject(pfonOld);
	fonDraw.DeleteObject();
	nXPos = nRet;
	return iCnt;
}

void CTitleSplitorWnd::MoveInText(CString& strText, int nXPos)
{
	int nPos = nXPos;
	int nChar = SnapXPosToTextSplit(strText, nPos);
	if(nPos != -1)
	{
		TryMoveSplit(m_pCapSplit, nPos, nChar);
	}
}

void CTitleSplitorWnd::TryMoveSplit(CSTSplittor* pSplit, int nPos, int nCharPos)
{
	if(NULL == pSplit)
		return;
	if(nPos != pSplit->GetPos())
	{
		CRect rcBound = pSplit->GetBound();
		rcBound.OffsetRect(-m_ptOrg.x, -m_ptOrg.y);
		InvalidateRect(rcBound);

		pSplit->HorzMoveTo(nPos);
		pSplit->SetCharNum(nCharPos);

		Break(pSplit);
		InsertSortPos(pSplit);

		rcBound = pSplit->GetBound();
		rcBound.OffsetRect(-m_ptOrg.x, -m_ptOrg.y);
		InvalidateRect(rcBound);
	}
}

void CTitleSplitorWnd::GetLineText(int nLine, CString& strText)
{
	int nStrCnt = (int)m_vtLines.size();
	if((nLine > -1)&&(nStrCnt > nLine))
	{
		strText = m_vtLines[nLine]->strLine;
	}
}

int CTitleSplitorWnd::SnapYosToLine(int nYPos)
{
	int nLines = GetLinesCount();
	int nHLine = GetLineHeight();
	int nLine = nYPos / nHLine;
	if(nLine < nLines)
		return nLine;
	return -1;
}

CSTSplittor* CTitleSplitorWnd::HitSplittors(const CPoint& point)
{
	CSTSplittor* pSplit = NULL;
	int nLineCnt = (int)m_vtLines.size();
	for(int iLine = 0; iLine < nLineCnt; iLine ++)
	{
		int nSize = (int)m_vtLines[iLine]->vtSplitors.size();
		for(int iSplit = 0; iSplit < nSize; iSplit ++)
		{
			pSplit = m_vtLines[iLine]->vtSplitors[iSplit];
			if(pSplit->HitTest(point))
				return pSplit;
		}
	}
	return NULL;
}

void CTitleSplitorWnd::InsertSortPos(CSTSplittor* pSplitNew)
{
	int nLine = pSplitNew->GetLineNum();
	if((nLine > -1)&&(nLine < GetLinesCount()))
	{
		int nSize = m_vtLines[nLine]->vtSplitors.size();
		CSTSplittor* pSplit;
		for(int iSplit = 0; iSplit < nSize; iSplit ++)
		{
			pSplit = m_vtLines[nLine]->vtSplitors[iSplit];
			if(pSplitNew->GetCharNum() > pSplit->GetCharNum())
				continue;
			m_vtLines[nLine]->vtSplitors.insert(m_vtLines[nLine]->vtSplitors.begin() + iSplit, pSplitNew);
			return;
		}
		m_vtLines[nLine]->vtSplitors.push_back(pSplitNew);
	}
}

int CTitleSplitorWnd::GetLinesCount()
{
	return (int)m_vtLines.size();
}

int CTitleSplitorWnd::GetLineHeight()
{
	return m_TextMetrics.tmExternalLeading + m_TextMetrics.tmHeight + m_nLineSpace;
}

BOOL CTitleSplitorWnd::CreateSplitWnd(CStringArray& astrTitle, const CRect& rcWnd, CWnd* pParent)
{
	int nID = 1001;
	CWnd* pWnd = pParent->GetDlgItem(nID);
	while(pWnd == NULL)
	{
		nID ++;
		pWnd = pParent->GetDlgItem(nID);
	}

	PSP_LINE pSpLine = NULL;

	int nStrCnt = astrTitle.GetCount();
	for(int iStr = 0; iStr < nStrCnt; iStr ++)
	{
		if(astrTitle[iStr].IsEmpty())
		{
			astrTitle.RemoveAt(iStr);
			iStr --;
			nStrCnt --;
			continue;
		}
		pSpLine = new SP_LINE;
		pSpLine->strLine = astrTitle[iStr];
		m_vtLines.push_back(pSpLine);
	}

	return CreateEx(WS_EX_STATICEDGE, NULL, 
		_T(""), WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
		rcWnd, pParent, nID);
}

/////////////////////////////////////////////////////////
// 消息响应函数, 注意，必需增加SS_NOTIFY属性
BEGIN_MESSAGE_MAP(CTitleSplitorWnd, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CTitleSplitorWnd message handlers

BOOL CTitleSplitorWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTitleSplitorWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcDraw;
	GetDrawArea(rcDraw);
	CBitmap bmMem;
	bmMem.CreateCompatibleBitmap(&dc, rcDraw.Width(), rcDraw.Height());
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	dcMem.SelectObject(&bmMem);

//	dcMem.SetViewportOrg(m_ptOrg);
	OnDraw(&dcMem);

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, m_ptOrg.x, m_ptOrg.y, SRCCOPY);
	dcMem.DeleteDC();
	bmMem.DeleteObject();
}

void CTitleSplitorWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);
	if(!m_bCapMouse)
	{
		if(NULL != HitSplittors(point + m_ptOrg))
		{
			m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
		}
		else
		{
			m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		}
	}
	else
	{
		int nLine = SnapYosToLine(m_pCapSplit->GetBound().top);
		if(nLine != -1)
		{
			MoveInText(m_vtLines[nLine]->strLine, point.x + m_ptOrg.x);
		}
	}
}

BOOL CTitleSplitorWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	::SetCursor(m_hCursor);
	return TRUE;
}

void CTitleSplitorWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bClickNew)
	{
		int nLine = SnapYosToLine(point.y + m_ptOrg.y);
		int nXPos = point.x + m_ptOrg.x;
		CString strText;
		GetLineText(nLine, strText);
		if(!strText.IsEmpty())
		{
			int nChar = SnapXPosToTextSplit(strText, nXPos);
			if(nXPos != -1)
			{
				CSTSplittor* pSplit = AddNewSplit(nLine);
				if(pSplit != NULL)
				{
					TryMoveSplit(pSplit, nXPos, nChar);
					m_bClickNew = FALSE;
				}
			}
		}
	}
	else
	{
		CSTSplittor* pSplit = HitSplittors(point + m_ptOrg);
		if(NULL != pSplit)
		{
			SetCapture();
			m_bCapMouse = TRUE;
			if(m_pCapSplit != NULL)
			{
				m_pCapSplit->SetObjFocus(FALSE);
				CRect rcBound = m_pCapSplit->GetBound();
				rcBound.OffsetRect(m_ptOrg);
				InvalidateRect(rcBound);
			}
			m_pCapSplit = pSplit;
			m_pCapSplit->SetObjFocus();
			CRect rcBound = m_pCapSplit->GetBound();
			rcBound.OffsetRect(m_ptOrg);
			InvalidateRect(rcBound);
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CTitleSplitorWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bCapMouse)
	{
		m_bCapMouse = FALSE;
		ReleaseCapture();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

int CTitleSplitorWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitMetrics();
//	SetScroll();

	return 0;
}

void CTitleSplitorWnd::PreSubclassWindow()
{
	InitMetrics();

	CWnd::PreSubclassWindow();
}

void CTitleSplitorWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CRect rcClient;
	GetClientRect(rcClient);
	switch(nSBCode)
	{
	case SB_ENDSCROLL:		//   End scroll.
		break;
	case SB_LEFT:			//   Scroll to far left.
		m_ptOrg.x = 0;
		break;
	case SB_RIGHT:			//   Scroll to far right.
		m_ptOrg.x = GetMaxWidth() - rcClient.Width();
		break;
	case SB_LINELEFT:		//   Scroll left.
		if(m_ptOrg.x > 0)
			m_ptOrg.x --;
		break;
	case SB_LINERIGHT:		//   Scroll right.
		if(m_ptOrg.x < GetMaxWidth() - rcClient.Width())
			m_ptOrg.x ++;
		break;
	case SB_PAGELEFT:		//   Scroll one page left.
		if(m_ptOrg.x > 0)
			m_ptOrg.x -= rcClient.Width();
		if(m_ptOrg.x < 0)
			m_ptOrg.x = 0;
		break;
	case SB_PAGERIGHT:		//   Scroll one page right.
		if(m_ptOrg.x < GetMaxWidth() - rcClient.Width())
			m_ptOrg.x += rcClient.Width();
		if(m_ptOrg.x > GetMaxWidth() - rcClient.Width())
			m_ptOrg.x = GetMaxWidth() - rcClient.Width();
		break;
	case SB_THUMBPOSITION:	//   Scroll to absolute position. The current position is specified by the nPos parameter.
	case SB_THUMBTRACK:		//   Drag scroll box to specified position. The current position is specified by the nPos parameter.
		m_ptOrg.x = nPos;
		if(m_ptOrg.x > GetMaxWidth() - rcClient.Width())
			m_ptOrg.x = GetMaxWidth() - rcClient.Width();
		if(m_ptOrg.x < 0)
			m_ptOrg.x = 0;
		break;
	}
	SetScrollPos(SB_HORZ, m_ptOrg.x);
	Invalidate();
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTitleSplitorWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CRect rcClient;
	GetClientRect(rcClient);
	int nLH = GetLineHeight();
	int nMaxHT = (GetLinesCount() + 1 - (rcClient.Height()/nLH))*nLH;
	int nPageHT = (rcClient.Height()/nLH)*nLH;
	switch(nSBCode)
	{
	case SB_ENDSCROLL:		//   End scroll.
		break;
	case SB_BOTTOM:			//   Scroll to bottom.
		m_ptOrg.y = nMaxHT;
		break;
	case SB_TOP:			//   Scroll to top.
		m_ptOrg.y = 0;
		break;
	case SB_LINEDOWN:		//   Scroll one line down.
		if(m_ptOrg.y < nMaxHT)
			m_ptOrg.y += nLH;
		if(m_ptOrg.y > nMaxHT)
			m_ptOrg.y = nMaxHT;
		break;
	case SB_LINEUP:			//   Scroll one line up.
		if(m_ptOrg.y > 0)
			m_ptOrg.y -= nLH;
		if(m_ptOrg.y < 0)
			m_ptOrg.y = 0;
		break;
	case SB_PAGEDOWN:		//   Scroll one page down.
		if(m_ptOrg.y < nMaxHT)
			m_ptOrg.y += nPageHT;
		if(m_ptOrg.y > nMaxHT)
			m_ptOrg.y = nMaxHT;
		break;
	case SB_PAGEUP:			//   Scroll one page up.
		if(m_ptOrg.y > 0)
			m_ptOrg.y -= nPageHT;
		if(m_ptOrg.y < 0)
			m_ptOrg.y = 0;
		break;
	case SB_THUMBPOSITION:	//   Scroll to the absolute position. The current position is provided in nPos.
	case SB_THUMBTRACK:		//   Drag scroll box to specified position. The current position is provided in nPos.
		m_ptOrg.y = nPos * nLH;
		if(m_ptOrg.y > nMaxHT)
			m_ptOrg.y = nMaxHT;
		if(m_ptOrg.y < 0)
			m_ptOrg.y = 0;
		break;
	}

	SetScrollPos(SB_VERT, m_ptOrg.y/nLH);
	Invalidate();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTitleSplitorWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	SetScroll();
}

BOOL CTitleSplitorWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta > 0)
		SendMessage(WM_VSCROLL, SB_LINEUP, 0);
	else
		SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

UINT CTitleSplitorWnd::OnGetDlgCode()
{
	// TODO: Add your message handler code here and/or call default
	return DLGC_WANTARROWS|DLGC_WANTALLKEYS|DLGC_WANTCHARS;
}

void CTitleSplitorWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_PRIOR:
		SendMessage(WM_VSCROLL, SB_PAGEUP, 0);
		break;
	case VK_NEXT:
		SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
		break;
	case VK_UP:
		SendMessage(WM_VSCROLL, SB_LINEUP, 0);
		break;
	case VK_DOWN:
		SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
		break;
	case VK_LEFT:
		SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
		break;
	case VK_RIGHT:
		SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
		break;
	case VK_HOME:
		SendMessage(WM_HSCROLL, SB_LEFT, 0);
		break;
	case VK_END:
		SendMessage(WM_HSCROLL, SB_RIGHT, 0);
		break;
	case VK_ESCAPE:
		SetClickNew(FALSE);
		break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTitleSplitorWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}
