// TitleTimeEdit.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "TitleTimeEdit.h"


// CTitleTimeEdit

IMPLEMENT_DYNAMIC(CTitleTimeEdit, CEdit)
static TCHAR	s_szDefTMChar[] = _T("00:00:00,000\r\0");
static TCHAR	s_szMaxTMChar[] = _T("99:59:59,999\r\0");

CTitleTimeEdit::CTitleTimeEdit()
{
	StringCbCopy(m_szTimeChar, 16, _T("00:00:00,000\r\0"));
}

CTitleTimeEdit::~CTitleTimeEdit()
{
}

BEGIN_MESSAGE_MAP(CTitleTimeEdit, CEdit)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CTitleTimeEdit::OnEnSetfocus)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR_REFLECT()

	ON_MESSAGE(WM_CUT,OnCut)
	ON_MESSAGE(WM_COPY,OnCopy)
	ON_MESSAGE(WM_PASTE,OnPaste)
	ON_MESSAGE(WM_CLEAR,OnClear)
	ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_MESSAGE(WM_GETTEXT,OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH,OnGetTextLength)
	ON_MESSAGE(EM_SETSEL,OnSetSel)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CTitleTimeEdit message handlers
void CTitleTimeEdit::OnEnSetfocus()
{
	// TODO: Add your control notification handler code here
}

void CTitleTimeEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnLButtonDown(nFlags, point);
}

void CTitleTimeEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnLButtonDblClk(nFlags, point);
}

void CTitleTimeEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnMouseMove(nFlags, point);
}

void CTitleTimeEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnLButtonUp(nFlags, point);
}

HBRUSH CTitleTimeEdit::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

LONG CTitleTimeEdit::OnCut(UINT wParam, LONG lParam)
{
	return 0;
}

LONG CTitleTimeEdit::OnCopy(UINT wParam, LONG lParam)
{
	return (LONG)CEdit::Default();
}

LONG CTitleTimeEdit::OnPaste(UINT wParam, LONG lParam)
{
	return (LONG)CEdit::Default();
}

LONG CTitleTimeEdit::OnClear(UINT wParam, LONG lParam)
{
	return 0;
}

LONG CTitleTimeEdit::OnSetText(UINT wParam, LONG lParam)
{
	CString strInput = (LPCTSTR)lParam;
	int	nLen = strInput.GetLength();
	if(strInput.Compare(m_szTimeChar) != 0)
	{
		for(int iPos = 0; iPos < 12; iPos ++)
		{
			m_szTimeChar[iPos] = s_szDefTMChar[iPos];
			if((iPos < nLen)
				&&(_istdigit(strInput.GetAt(iPos))))
			{
				if(strInput.GetAt(iPos) <= s_szMaxTMChar[iPos])
					m_szTimeChar[iPos] = strInput.GetAt(iPos);
				else
					m_szTimeChar[iPos] = s_szMaxTMChar[iPos];
			}
		}
	}
	return (LONG)CEdit::Default();
}

LONG CTitleTimeEdit::OnGetText(UINT wParam, LONG lParam)
{
	return (LONG)CEdit::Default();
}

LONG CTitleTimeEdit::OnGetTextLength(UINT wParam, LONG lParam)
{
	return 12;
}

LONG CTitleTimeEdit::OnSetSel(UINT wParam, LONG lParam)
{
	return (LONG)CEdit::Default();
}

void CTitleTimeEdit::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CEdit::PreSubclassWindow();
	Update();
}

BOOL CTitleTimeEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bRet = CEdit::Create(dwStyle, rect, pParentWnd, nID);
	Update();
	return bRet;
}

void CTitleTimeEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	TRACE(_T("CTitleTimeEdit::OnKeyDown(\'%c\' (%x), %d, 0x%04x)\n"), 
		nChar, nChar, nRepCnt, nFlags);

	DWORD dwStyle= GetStyle();
	if ((dwStyle & ES_READONLY)== ES_READONLY)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		return ;
	}

	switch(nChar)
	{
	case VK_DELETE:
		KeyInDel();
		break;

	case VK_HOME:
	case VK_END:
	case VK_RIGHT:
	case VK_LEFT:
		CEdit::Default();
		break;
	case VK_UP:
		KeyInUp();
		break;
	case VK_DOWN:
		KeyInDown();
		break;
	}
}

void CTitleTimeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	TRACE(_T("CTitleTimeEdit::OnChar(\'%c\' (%x), %d, 0x%04x)\n"), 
		nChar, nChar, nRepCnt, nFlags);

	DWORD dwStyle= GetStyle();
	if ((dwStyle & ES_READONLY)== ES_READONLY)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return ;
	}

	switch(nChar)
	{
	case VK_0:	case VK_1:	case VK_2:	case VK_3:	case VK_4:
	case VK_5:	case VK_6:	case VK_7:	case VK_8:	case VK_9:
		KeyInNumeric(nChar);
		break;

	case VK_BACK:
		KeyInBack();
		break;
	};
}

void CTitleTimeEdit::Update()
{
	if(::IsWindow(m_hWnd))
	{
		m_szTimeChar[12] = '\r';
		m_szTimeChar[13] = '\0';
		SetWindowText(m_szTimeChar);
	}
}

void CTitleTimeEdit::DeleteRange(int nStart, int nEnd)
{
	for(int iPos = nStart; iPos < nEnd; iPos ++)
	{
		m_szTimeChar[iPos] = s_szDefTMChar[iPos];
	}
}

void CTitleTimeEdit::KeyInBack()
{
	int nCharPos	= CharFromPos(GetCaretPos());

	int nSelStart	= 0;
	int nSelEnd		= 0;
	GetSel(nSelStart, nSelEnd);

	if(nSelStart == nSelEnd)
	{
		if(nCharPos > 0)
		{
			nCharPos --;
			if(s_szDefTMChar[nCharPos] != '0')
				nCharPos --;
			m_szTimeChar[nCharPos] = s_szDefTMChar[nCharPos];
		}
	}
	else
	{
		DeleteRange(nSelStart, nSelEnd);
	}
	Update();
	SetSel(nCharPos, nCharPos);
}

void CTitleTimeEdit::KeyInDel()
{
	int nCharPos = CharFromPos(GetCaretPos());

	int nSelStart	= 0;
	int nSelEnd		= 0;
	GetSel(nSelStart, nSelEnd);

	if(nSelStart == nSelEnd)
	{
		if(nCharPos < 12)
		{
			m_szTimeChar[nCharPos] = s_szDefTMChar[nCharPos];
			nCharPos ++;
			if(s_szDefTMChar[nCharPos] != '0')
				nCharPos ++;
		}
	}
	else
	{
		DeleteRange(nSelStart, nSelEnd);
	}
	Update();
	SetSel(nCharPos, nCharPos);
}

void CTitleTimeEdit::KeyInNumeric(UINT nChar)
{
	int nCharPos = CharFromPos(GetCaretPos());

	int nSelStart	= 0;
	int nSelEnd		= 0;
	GetSel(nSelStart, nSelEnd);
	if(nSelStart == nSelEnd)
	{
		if(nCharPos < 12)
		{
			if((s_szDefTMChar[nCharPos] == '0')
				&&(nChar <= (UINT)s_szMaxTMChar[nCharPos]))
			{
				m_szTimeChar[nCharPos] = nChar;
			}
			nCharPos ++;
			if(s_szDefTMChar[nCharPos] != '0')
				nCharPos ++;
		}
	}
	else
	{
		if((s_szDefTMChar[nSelStart] == '0')
			&&(nChar <= (UINT)s_szMaxTMChar[nCharPos]))
		{
			m_szTimeChar[nSelStart] = nChar;
		}
	}
	Update();
	SetSel(nCharPos, nCharPos);
}

void CTitleTimeEdit::KeyInUp()
{
	int nCharPos = CharFromPos(GetCaretPos());
	if(nCharPos < 3)
	{
		if(m_szTimeChar[1] < s_szMaxTMChar[1])
			m_szTimeChar[1] ++;
		else if(m_szTimeChar[1] == s_szMaxTMChar[1])
		{
			if(m_szTimeChar[0] < s_szMaxTMChar[0])
			{
				m_szTimeChar[0] ++;
				m_szTimeChar[1] = '0';
			}
		}
	}
	else if(nCharPos < 6)
	{
		if(m_szTimeChar[4] < s_szMaxTMChar[4])
			m_szTimeChar[4] ++;
		else if(m_szTimeChar[4] == s_szMaxTMChar[4])
		{
			if(m_szTimeChar[3] < s_szMaxTMChar[3])
			{
				m_szTimeChar[3] ++;
				m_szTimeChar[4] = '0';
			}
		}
	}
	else if(nCharPos < 9)
	{
		if(m_szTimeChar[7] < s_szMaxTMChar[7])
			m_szTimeChar[7] ++;
		else if(m_szTimeChar[7] == s_szMaxTMChar[7])
		{
			if(m_szTimeChar[6] < s_szMaxTMChar[6])
			{
				m_szTimeChar[6] ++;
				m_szTimeChar[7] = '0';
			}
		}
	}
	else
	{
		if(m_szTimeChar[11] < s_szMaxTMChar[11])
			m_szTimeChar[11] ++;
		else if(m_szTimeChar[11] == s_szMaxTMChar[11])
		{
			if(m_szTimeChar[10] < s_szMaxTMChar[10])
			{
				m_szTimeChar[10] ++;
				m_szTimeChar[11] = '0';
			}
			else if(m_szTimeChar[10] == s_szMaxTMChar[10])
			{
				if(m_szTimeChar[9] < s_szMaxTMChar[9])
				{
					m_szTimeChar[9] ++;
					m_szTimeChar[10] = '0';
					m_szTimeChar[11] = '0';
				}
			}
		}
	}
	Update();
	SetSel(nCharPos, nCharPos);
}

void CTitleTimeEdit::KeyInDown()
{
	int nCharPos = CharFromPos(GetCaretPos());
	if(nCharPos < 3)
	{
		if(m_szTimeChar[1] > '0')
			m_szTimeChar[1] --;
		else if(m_szTimeChar[1] == '0')
		{
			if(m_szTimeChar[0] > '0')
			{
				m_szTimeChar[0] --;
				m_szTimeChar[1] = s_szMaxTMChar[1];
			}
		}
	}
	else if(nCharPos < 6)
	{
		if(m_szTimeChar[4] > '0')
			m_szTimeChar[4] --;
		else if(m_szTimeChar[4] == '0')
		{
			if(m_szTimeChar[3] > '0')
			{
				m_szTimeChar[3] --;
				m_szTimeChar[4] = s_szMaxTMChar[4];
			}
		}
	}
	else if(nCharPos < 9)
	{
		if(m_szTimeChar[7] > '0')
			m_szTimeChar[7] --;
		else if(m_szTimeChar[7] ==  '0')
		{
			if(m_szTimeChar[6] > '0')
			{
				m_szTimeChar[6] --;
				m_szTimeChar[7] = s_szMaxTMChar[7];
			}
		}
	}
	else
	{
		if(m_szTimeChar[11] > '0')
			m_szTimeChar[11] --;
		else if(m_szTimeChar[11] == '0')
		{
			if(m_szTimeChar[10] > '0')
			{
				m_szTimeChar[10] --;
				m_szTimeChar[11] = s_szMaxTMChar[11];
			}
			else if(m_szTimeChar[10] == '0')
			{
				if(m_szTimeChar[9] > '0')
				{
					m_szTimeChar[9] --;
					m_szTimeChar[10] = s_szMaxTMChar[10];
					m_szTimeChar[11] = s_szMaxTMChar[11];
				}
			}
		}
	}
	Update();
	SetSel(nCharPos, nCharPos);
}

void CTitleTimeEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CEdit::OnContextMenu(pWnd, point);
}
