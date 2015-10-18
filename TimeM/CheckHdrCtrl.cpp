// CheckHdrCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CheckHdrCtrl.h"


// CCheckHdrCtrl

IMPLEMENT_DYNAMIC(CCheckHdrCtrl, CHeaderCtrl)

CCheckHdrCtrl::CCheckHdrCtrl()
{
	m_pImageList = NULL;
	m_pChkImageList = NULL;
}

CCheckHdrCtrl::~CCheckHdrCtrl()
{
}

BEGIN_MESSAGE_MAP(CCheckHdrCtrl, CHeaderCtrl)
	ON_NOTIFY_REFLECT(HDN_ITEMCLICKA, &CCheckHdrCtrl::OnHdnItemclick)
	ON_NOTIFY_REFLECT(HDN_ITEMCLICKW, &CCheckHdrCtrl::OnHdnItemclick)
//	ON_NOTIFY(HDN_ITEMCLICKA, 0, &CCheckHdrCtrl::OnHdnItemclick)
//	ON_NOTIFY(HDN_ITEMCLICKW, 0, &CCheckHdrCtrl::OnHdnItemclick)
END_MESSAGE_MAP()

// CCheckHdrCtrl message handlers

//#define ODS_SELECTED		0x0001
//#define ODS_GRAYED		0x0002
//#define ODS_DISABLED		0x0004
//#define ODS_CHECKED		0x0008
//#define ODS_FOCUS			0x0010
//#define ODS_DEFAULT		0x0020
//#define ODS_COMBOBOXEDIT	0x1000
//#define ODS_HOTLIGHT		0x0040
//#define ODS_INACTIVE		0x0080
//#define ODS_NOACCEL		0x0100
//#define ODS_NOFOCUSRECT	0x0200

COLORREF GetNextGradualClr(COLORREF clrSrc)
{
	DWORD nRed		= GetRValue(clrSrc);
	DWORD nGreen	= GetGValue(clrSrc);
	DWORD nBlue		= GetBValue(clrSrc);

	nRed = nRed*947/1000;
	nGreen = nGreen*947/1000;
	nBlue = nBlue*947/1000;

	return RGB(nRed, nGreen, nBlue);
}

void CCheckHdrCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;

	TRACE("CCheckHdrCtrl::DrawItem(%d) State: 0x%04X\r\n", lpDrawItemStruct->itemID, lpDrawItemStruct->itemState);

	dc.Attach( lpDrawItemStruct->hDC );

	// Get the column rect
	CRect rcLabel( lpDrawItemStruct->rcItem );

	// Save DC
	int nSavedDC = dc.SaveDC();

	// Set clipping region to limit drawing within column
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcLabel );
	dc.SelectObject( &rgn );
	rgn.DeleteObject();

	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character
	int offset = dc.GetTextExtent(_T(" "), 1 ).cx*2;

	CPen penSplit;
	penSplit.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
	CPen* pPrevPen = dc.SelectObject(&penSplit);
	dc.MoveTo(lpDrawItemStruct->rcItem.right - 2, lpDrawItemStruct->rcItem.top + 3);
	dc.LineTo(lpDrawItemStruct->rcItem.right - 2, lpDrawItemStruct->rcItem.bottom - 3);
	dc.SelectObject(pPrevPen);
	penSplit.DeleteObject();

	penSplit.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	pPrevPen = dc.SelectObject(&penSplit);
	dc.MoveTo(lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.top + 3);
	dc.LineTo(lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.bottom - 3);
	dc.SelectObject(pPrevPen);
	penSplit.DeleteObject();

	CPen penBottom;
	COLORREF clrLine = GetSysColor(COLOR_BTNFACE);

	for(int i = 0; i < 3; i ++)
	{
		clrLine = GetNextGradualClr(clrLine);
		penBottom.CreatePen(PS_SOLID, 1, clrLine);
		pPrevPen = dc.SelectObject(&penBottom);

		dc.MoveTo(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - 3 + i);
		dc.LineTo(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom - 3 + i);

		dc.SelectObject(pPrevPen);
		penBottom.DeleteObject();
	}
	// Draw image from check state image list
	int nState;
	if(m_pChkImageList
		&&m_mapChkStateIndex.Lookup( lpDrawItemStruct->itemID, nState))
	{
		if( nState != 0)
		{
			m_pChkImageList->Draw(&dc, nState,
				CPoint(rcLabel.left + offset, offset/3),
				ILD_TRANSPARENT);
			IMAGEINFO chkImgInfo;
			if(m_pChkImageList->GetImageInfo( nState, &chkImgInfo))
			{
				rcLabel.left += offset/2 +
					chkImgInfo.rcImage.right - chkImgInfo.rcImage.left;
			}
		}
	}


	// Draw image from image list
	int imageIndex;
	if (m_pImageList &&
		m_mapImageIndex.Lookup( lpDrawItemStruct->itemID, imageIndex ) )
	{
		if( imageIndex != -1 )
		{
			m_pImageList->Draw(&dc, imageIndex,
				CPoint( rcLabel.left + offset,offset/3 ),
				ILD_TRANSPARENT );

			// Now adjust the label rectangle
			IMAGEINFO imageinfo;
			if( m_pImageList->GetImageInfo( imageIndex, &imageinfo ) )
			{
				rcLabel.left += offset/2 +
					imageinfo.rcImage.right - imageinfo.rcImage.left;
			}
		}
	}

	// Get the column text and format
	TCHAR buf[256];
	HD_ITEM hditem;

	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = buf;
	hditem.cchTextMax = 255;

	GetItem( lpDrawItemStruct->itemID, &hditem );

	// Determine format for drawing column label
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP
		| DT_VCENTER | DT_END_ELLIPSIS ;

	if( hditem.fmt & HDF_CENTER)
		uFormat |= DT_CENTER;
	else if( hditem.fmt & HDF_RIGHT)
		uFormat |= DT_RIGHT;
	else
		uFormat |= DT_LEFT;

	// Adjust the rect if the mouse button is pressed on it
	if( lpDrawItemStruct->itemState == ODS_SELECTED )
	{
		rcLabel.left++;
		rcLabel.top += 2;
		rcLabel.right++;
	}

	rcLabel.left += offset;
	rcLabel.right -= offset;

	// Draw column label
	if( rcLabel.left < rcLabel.right )
	{
		dc.DrawText(buf,-1,rcLabel, uFormat);
		TRACE("Text Pos:%d, %d, %d, %d\r\n", rcLabel.left, rcLabel.top, rcLabel.right, rcLabel.bottom);
	}

	// Restore dc
	dc.RestoreDC( nSavedDC );

	// Detach the dc before returning
	dc.Detach();
}

CImageList* CCheckHdrCtrl::GetImageList() const
{
	return m_pImageList;
}

CImageList* CCheckHdrCtrl::SetImageList(CImageList* pImageList)
{
	CImageList *pPrevList = m_pImageList;
	m_pImageList = pImageList;
	return pPrevList;
}

void CCheckHdrCtrl::SetChkImage(CImageList* pImageList)
{
	m_pChkImageList = pImageList;
}

void CCheckHdrCtrl::CheckCol(int nItem, int nState)
{
	m_mapChkStateIndex[nItem] = nState;
	Invalidate();
}

int CCheckHdrCtrl::GetItemImage( int nItem )
{
	int imageIndex;
	if( m_mapImageIndex.Lookup( nItem, imageIndex ) )
		return imageIndex;
	return -1;
}

void CCheckHdrCtrl::SetItemImage( int nItem, int nImage )
{
	// Save the image index
	m_mapImageIndex[nItem] = nImage;

	// Change the item to ownder drawn
	HD_ITEM hditem;

	hditem.mask = HDI_FORMAT;
	GetItem( nItem, &hditem );
	hditem.fmt |= HDF_OWNERDRAW;
	SetItem( nItem, &hditem );

	// Invalidate header control so that it gets redrawn
	Invalidate();
}

void CCheckHdrCtrl::OnHdnItemclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nState;
	if(m_pChkImageList
		&&m_mapChkStateIndex.Lookup( phdr->iItem, nState))
	{
		if(nState != HDR_CHKSTATE_NON)
		{
			if(nState != HDR_CHKSTATE_FALSE)
			{
				m_mapChkStateIndex[phdr->iItem] = HDR_CHKSTATE_FALSE;
			}
			else
			{
				m_mapChkStateIndex[phdr->iItem] = HDR_CHKSTATE_TRUE;
			}
			CWnd* pWnd = GetParent();
			pWnd->SendMessage(MSG_CHKHDR_CHECK, 0, m_mapChkStateIndex[phdr->iItem]);
		}
	}

	*pResult = 0;
}
