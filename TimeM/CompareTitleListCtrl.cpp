#include "StdAfx.h"
#include "CompareTitleListCtrl.h"

CCompareTitleListCtrl::CCompareTitleListCtrl(void)
{
	m_pCompare = NULL;
	m_nDirect = 0;
}

CCompareTitleListCtrl::~CCompareTitleListCtrl(void)
{
}

void CCompareTitleListCtrl::AttachCompare(CTitleCompare* pCompTitle, int nDirect)
{
	m_pCompare = pCompTitle;
	m_nDirect = nDirect;
}
	
COLORREF CCompareTitleListCtrl::StateToTextColor(int nState)
{
	switch(nState)
	{
	case COMPARE_NOTEQUAL:
		return RGB(0, 0, 255);
	case COMPARE_LEFTEMPTY:
		return RGB(0, 200, 0);
	case COMPARE_RIGHTEMPTY:
		return RGB(255, 0, 0);
	case COMPARE_EQUAL:
	default:
		return GetTextColor();
	}
}

COLORREF CCompareTitleListCtrl::StateToBkColor(int nState)
{
	switch(nState)
	{
	case COMPARE_NOTEQUAL:
		return RGB(225, 241, 255);
	case COMPARE_LEFTEMPTY:
	case COMPARE_RIGHTEMPTY:
		return RGB(204, 218, 241);
	case COMPARE_EQUAL:
	default:
		return GetBkColor();
	}
}

COLORREF CCompareTitleListCtrl::OnGetCellBkColor(int nRow, int nCol)
{
	if((nRow < m_pCompare->GetRelCount())
		&&(nRow > -1))
	{
		const COMPARE_STATE& compState = m_pCompare->GetItemCompareState(nRow);
		switch(nCol)
		{
		case 0:
			return StateToBkColor(compState.nStartTimeState);
		case 1:
			return StateToBkColor(compState.nEndTimeState);
		case 2:
			return StateToBkColor(compState.nContentState);
		}
	}
	return CTitleListCtrl::OnGetCellBkColor(nRow, nCol);
}

COLORREF CCompareTitleListCtrl::OnGetCellTextColor(int nRow, int nCol)
{
	if((nRow < m_pCompare->GetRelCount())
		&&(nRow > -1))
	{
		const COMPARE_STATE& compState = m_pCompare->GetItemCompareState(nRow);
		switch(nCol)
		{
		case 0:
			return StateToTextColor(compState.nStartTimeState);
		case 1:
			return StateToTextColor(compState.nEndTimeState);
		case 2:
			return StateToTextColor(compState.nContentState);
		}
	}
	return CTitleListCtrl::OnGetCellTextColor(nRow, nCol);
}
