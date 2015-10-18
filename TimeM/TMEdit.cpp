// TMEdit.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "TMEdit.h"


// CTMEdit

IMPLEMENT_DYNAMIC(CTMEdit, CEdit)

CTMEdit::CTMEdit()
{

}

CTMEdit::~CTMEdit()
{
}

BEGIN_MESSAGE_MAP(CTMEdit, CEdit)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()
// CTMEdit message handlers

void CTMEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CEdit::OnContextMenu(pWnd, point);
}
