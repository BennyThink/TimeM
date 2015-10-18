// CheckListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CheckListCtrl.h"


// CCheckListCtrl

IMPLEMENT_DYNAMIC(CCheckListCtrl, CListCtrl)

CCheckListCtrl::CCheckListCtrl()
{

}

CCheckListCtrl::~CCheckListCtrl()
{
}

BEGIN_MESSAGE_MAP(CCheckListCtrl, CListCtrl)
	ON_MESSAGE(MSG_CHKHDR_CHECK, &CCheckListCtrl::OnHdrChk)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CCheckListCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()

// CCheckListCtrl message handlers

//void CCheckListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//}

void CCheckListCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_hdrCtrl.SubclassWindow(::GetDlgItem(m_hWnd, 0));
	m_hdrCtrl.CheckCol(0, 1);
	CListCtrl::PreSubclassWindow();
}

CImageList* CCheckListCtrl::SetImageList(CImageList* pImageList, int nImageListType)
{
	m_hdrCtrl.SetImageList(pImageList);
	return CListCtrl::SetImageList(pImageList, nImageListType);
}

void CCheckListCtrl::SetColumnImage(int nCol, int nImage)
{
	m_hdrCtrl.SetItemImage(nCol, nImage);
}

void CCheckListCtrl::SetHdrChkImage(UINT nBmpID, int cx, int cy, COLORREF clrMask)
{
	CBitmap bmpImage;
	bmpImage.LoadBitmap(nBmpID);
	m_imgChk.Create(cx, cy, ILC_COLOR24|ILC_MASK, 4, 1);
	m_imgChk.Add(&bmpImage, clrMask);
	bmpImage.DeleteObject();
	m_hdrCtrl.SetChkImage(&m_imgChk);
}

LRESULT CCheckListCtrl::OnHdrChk(WPARAM wParam, LPARAM lParam)
{
	BOOL bChk = FALSE;
	switch(lParam)
	{
	case HDR_CHKSTATE_FALSE:
		bChk = FALSE;
		break;
	case HDR_CHKSTATE_TRUE:
		bChk = TRUE;
		break;
	default:
		return 0;
	}
	int nCnt = GetItemCount();
	for(int i = 0; i < nCnt; i ++)
	{
		SetCheck(i, bChk);
	}
	return 0;
}

void CCheckListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nCnt = GetItemCount();
	int nState = HDR_CHKSTATE_NON;
	for(int i = 0; i < nCnt; i ++)
	{
		if(GetCheck(i))
		{
			if(nState == HDR_CHKSTATE_NON)
				nState = HDR_CHKSTATE_TRUE;
			else if(nState == HDR_CHKSTATE_FALSE)
				nState = HDR_CHKSTATE_PART;
		}
		else
		{
			if(nState == HDR_CHKSTATE_NON)
				nState = HDR_CHKSTATE_FALSE;
			else if(nState == HDR_CHKSTATE_TRUE)
				nState = HDR_CHKSTATE_PART;
		}
	}

	m_hdrCtrl.CheckCol(0, nState);
	::SendMessage(GetParent()->GetSafeHwnd(), MSG_LISTITEM_CHANGED, GetDlgCtrlID(), (LPARAM)pNMHDR);

	*pResult = -1;
}

int CCheckListCtrl::GetNextCheckItem(int nStartItem)
{
	int nLoopStart = 0;
	int nItemCnt = GetItemCount();
	if(nStartItem > nItemCnt - 2)
		return -1;
	if(nStartItem > -1)
		nLoopStart = nStartItem + 1;
	for(int i = nLoopStart; i < nItemCnt; i ++)
	{
		if(GetCheck(i))
			return i;
	}
	return -1;
}

int CCheckListCtrl::GetPrevCheckItem(int nStartItem)
{
	int nItemCnt = GetItemCount();
	int nLoopStart = nItemCnt - 1;
	if(nStartItem == 0)
		return -1;
	if(-1 != nStartItem)
		nLoopStart = nStartItem - 1;
	for(int i = nLoopStart; i > -1; i --)
	{
		if(GetCheck(i))
			return i;
	}
	return -1;
}
