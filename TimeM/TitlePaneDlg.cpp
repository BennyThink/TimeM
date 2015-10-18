// TitlePaneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "TitlePaneDlg.h"

#include "TitleHelper.h"

// CTitlePaneDlg

IMPLEMENT_DYNAMIC(CTitlePaneDlg, CPaneDialog)

CTitlePaneDlg::CTitlePaneDlg()
	:m_strStartTime(_T(""))
	, m_strEndTime(_T(""))
	, m_strTitle(_T(""))
	, m_strCharCount(_T(""))
{
}

CTitlePaneDlg::~CTitlePaneDlg()
{
}

void CTitlePaneDlg::DoDataExchange(CDataExchange* pDX)
{
	CPaneDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_START, m_strStartTime);
	DDX_Text(pDX, IDC_EDIT_END, m_strEndTime);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_EDIT_CHARCOUNT, m_strCharCount);
	DDX_Control(pDX, IDC_EDIT_START, m_EditStart);
	DDX_Control(pDX, IDC_EDIT_END, m_EditEnd);
	DDX_Control(pDX, IDC_EDIT_TITLE, m_TitleCtrl);
	DDX_Control(pDX, IDC_EDIT_CHARCOUNT, m_CntLineCtrl);
}

BEGIN_MESSAGE_MAP(CTitlePaneDlg, CPaneDialog)
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_EDIT_START, &CTitlePaneDlg::OnEnKillfocusEditStart)
	ON_EN_KILLFOCUS(IDC_EDIT_END, &CTitlePaneDlg::OnEnKillfocusEditEnd)
	ON_EN_KILLFOCUS(IDC_EDIT_TITLE, &CTitlePaneDlg::OnEnKillfocusEditTitle)
	ON_EN_CHANGE(IDC_EDIT_TITLE, &CTitlePaneDlg::OnEnChangeEditTitle)
	ON_EN_VSCROLL(IDC_EDIT_TITLE, &CTitlePaneDlg::OnEnVscrollEditTitle)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_TITLE, &CTitlePaneDlg::OnEnSetfocusEditTitle)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

CTimeMDoc* CTitlePaneDlg::GetDocument() const
{
	CTimeMDoc* pDoc = NULL;
	POSITION posDocTmpl = AfxGetApp()->GetFirstDocTemplatePosition();
	while(posDocTmpl)
	{
		CDocTemplate* pDocTmpl = AfxGetApp()->GetNextDocTemplate(posDocTmpl);
		POSITION posDoc = pDocTmpl->GetFirstDocPosition();
		while(posDoc)
		{
			pDoc = (CTimeMDoc*)pDocTmpl->GetNextDoc(posDoc);
			if((NULL != pDoc)
				&&(pDoc->IsKindOf(RUNTIME_CLASS(CTimeMDoc))))
				break;
			else
				pDoc = NULL;
		}
	}
	return pDoc;
}

// CTitlePaneDlg message handlers
void CTitlePaneDlg::AdjustLayout()
{
	if(IsWindow(m_EditStart.m_hWnd)
		&&IsWindow(m_CntLineCtrl.m_hWnd)
		&&IsWindow(m_TitleCtrl.m_hWnd))
	{
		CRect rcStart;
		m_EditStart.GetWindowRect(rcStart);
		ScreenToClient(rcStart);

		CRect rcCntLine;
		m_CntLineCtrl.GetWindowRect(rcCntLine);
		ScreenToClient(rcCntLine);

		CRect rcTitle;
		m_TitleCtrl.GetWindowRect(rcTitle);
		ScreenToClient(rcTitle);

		CRect rcClient;
		GetClientRect(rcClient);
		if(rcClient.Height() > 120)
		{
			rcCntLine.bottom = rcClient.bottom - afxGlobalData.m_nAutoHideToolBarMargin;
			rcTitle.bottom = rcClient.bottom - afxGlobalData.m_nAutoHideToolBarMargin;
		}

		if(rcClient.Width() > 400)
		{
			rcCntLine.right = rcClient.right - afxGlobalData.m_nAutoHideToolBarMargin;
			rcCntLine.left = rcCntLine.right - 40;
			rcTitle.right = rcCntLine.left - 1;
		}
		m_CntLineCtrl.MoveWindow(rcCntLine);
		m_TitleCtrl.MoveWindow(rcTitle);
	}
}

void CTitlePaneDlg::OnSize(UINT nType, int cx, int cy)
{
	CPaneDialog::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CTitlePaneDlg::OnSaveItem()
{
	CTimeMDoc* pDoc = GetDocument();
	if(pDoc == NULL)
		return;
	if(pDoc->SaveCurrentTitleItem(m_strStartTime, m_strEndTime, m_strTitle))
		pDoc->UpdateAllViews(NULL);
}

void CTitlePaneDlg::OnLoadItem()
{
	CTimeMDoc* pDoc = GetDocument();
	if(pDoc == NULL)
		return;
	const PTITLE_UNIT pUnit = pDoc->GetTitleItem(pDoc->GetCurrentPos());
	if(pUnit == NULL)
		return;

	m_strStartTime	= CTitleHelper::FormatTime(pUnit->nStart);
	m_strEndTime	= CTitleHelper::FormatTime(pUnit->nEnd);
	m_strTitle		= pUnit->content;
	
	CTitleHelper::CountTitleLine(m_strTitle, m_strCharCount);

	UpdateData(FALSE);
}

void CTitlePaneDlg::OnEnKillfocusEditStart()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	OnSaveItem();
}

void CTitlePaneDlg::OnEnKillfocusEditEnd()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	OnSaveItem();
}

void CTitlePaneDlg::OnEnSetfocusEditTitle()
{
	// TODO: Add your control notification handler code here
}

void CTitlePaneDlg::OnEnKillfocusEditTitle()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	OnSaveItem();
}

void CTitlePaneDlg::OnEnChangeEditTitle()
{
	// TODO:  Add your control notification handler code here
	m_TitleCtrl.GetWindowText(m_strTitle);

	CTitleHelper::CountTitleLine(m_strTitle, m_strCharCount);
	UpdateLine(m_TitleCtrl, m_CntLineCtrl);
	OnSaveItem();

	m_CntLineCtrl.SetWindowText(m_strCharCount);
}

void CTitlePaneDlg::OnEnVscrollEditTitle()
{
	// TODO: Add your control notification handler code here
	UpdateLine(m_TitleCtrl, m_CntLineCtrl);
}

void CTitlePaneDlg::UpdateLine(CEdit& editSrc, CEdit& editTag)
{
	int nSrcLine = editSrc.GetFirstVisibleLine();
	int nTagLine = editTag.GetFirstVisibleLine();
	editTag.LineScroll(nSrcLine - nTagLine);
}

void CTitlePaneDlg::UpdateDocData(BOOL bSave)
{
	if(bSave)
		OnSaveItem();
	else
		OnLoadItem();
}

BOOL CTitlePaneDlg::IsInputFocus()
{
	return (::GetFocus() == m_TitleCtrl.GetSafeHwnd());
}

void CTitlePaneDlg::FocusTitle()
{
	m_TitleCtrl.SetFocus();
}

BOOL CTitlePaneDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
//	if((::GetFocus() == m_TitleCtrl.GetSafeHwnd()))
//		return PreTranslateInput(pMsg);

	return CPaneDialog::PreTranslateMessage(pMsg);
}

BOOL CTitlePaneDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, afxGlobalData.clrBarFace);
	return TRUE;
//	return CPaneDialog::OnEraseBkgnd(pDC);
}

HBRUSH CTitlePaneDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPaneDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch(pWnd->GetDlgCtrlID())
	{
	case IDC_EDIT_TITLE:
	case IDC_EDIT_START:
	case IDC_EDIT_END:
		break;
	case IDC_EDIT_CHARCOUNT:
		hbr = CreateSolidBrush(afxGlobalData.clrBarLight);
		pDC->SetBkColor(afxGlobalData.clrBarLight);
		break;
	default:
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkColor(afxGlobalData.clrBarFace);
		break;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CTitlePaneDlg::OnCancel()
{
}

void CTitlePaneDlg::OnOK()
{
}

void CTitlePaneDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

//	CPaneDialog::OnClose();
}
