// ClrButton.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "ClrButton.h"


// CClrButton

IMPLEMENT_DYNAMIC(CClrButton, CButton)

CClrButton::CClrButton()
{
	m_clrBtn = GetSysColor(COLOR_3DFACE);
}

CClrButton::~CClrButton()
{
}


BEGIN_MESSAGE_MAP(CClrButton, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CClrButton::OnBnClicked)
END_MESSAGE_MAP()

// CClrButton message handlers

void CClrButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dcBtn;
	dcBtn.Attach(lpDrawItemStruct->hDC);

	CRect rcBtn;
	GetClientRect(rcBtn);
	dcBtn.DrawFrameControl(rcBtn, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_FLAT);
	rcBtn.DeflateRect(2, 2);
	dcBtn.FillSolidRect(rcBtn, m_clrBtn);
	if(ODS_FOCUS == (ODS_FOCUS&lpDrawItemStruct->itemState))
		dcBtn.DrawFocusRect(rcBtn);
	dcBtn.Detach();
}

void CClrButton::OnBnClicked()
{
	CColorDialog clrDlg(m_clrBtn, CC_ANYCOLOR, this);
	if(clrDlg.DoModal() == IDOK)
	{
		m_clrBtn = clrDlg.GetColor();
		Invalidate(FALSE);
	}
}
