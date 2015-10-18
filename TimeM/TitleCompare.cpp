#include "StdAfx.h"
#include "TitleCompare.h"

CTitleCompare::CTitleCompare(void)
{
}

CTitleCompare::~CTitleCompare(void)
{
}

void CTitleCompare::ReBuildCompare()
{
	m_VtCompare.clear();

	COMPARE_STATE compState;
	PTITLE_UNIT	pLeftUnit;
	PTITLE_UNIT	pRightUnit;
	int nSize = m_vtCombination.size();
	for(int iItem = 0; iItem < nSize; iItem ++)
	{
		compState.nId = iItem;
		pLeftUnit = m_ActLeft.GetItem(m_vtCombination[iItem].nLeftID, FALSE);
		pRightUnit = m_ActRight.GetItem(m_vtCombination[iItem].nRightID, FALSE);
		if(pLeftUnit == NULL)
		{
			compState.nStartTimeState	= COMPARE_LEFTEMPTY;
			compState.nEndTimeState		= COMPARE_LEFTEMPTY;
			compState.nContentState		= COMPARE_LEFTEMPTY;
		}
		else if(pRightUnit == NULL)
		{
			compState.nStartTimeState	= COMPARE_RIGHTEMPTY;
			compState.nEndTimeState		= COMPARE_RIGHTEMPTY;
			compState.nContentState		= COMPARE_RIGHTEMPTY;
		}
		else
		{
			compState.nStartTimeState = 
				(pLeftUnit->nStart == pRightUnit->nStart)?COMPARE_EQUAL:COMPARE_NOTEQUAL;
			compState.nEndTimeState = 
				(pLeftUnit->nEnd == pRightUnit->nEnd)?COMPARE_EQUAL:COMPARE_NOTEQUAL;
			compState.nContentState = 
				(pLeftUnit->content.CompareNoCase(pRightUnit->content) == 0)?COMPARE_EQUAL:COMPARE_NOTEQUAL;
		}
		m_VtCompare.push_back(compState);
	}
}

BOOL CTitleCompare::BuildRelation(UINT uType)
{
	BOOL bRet = CTitleRelation::BuildRelation(uType);
	ReBuildCompare();
	return bRet;
}

BOOL CTitleCompare::BandRelation(int nLeft, int nRight)
{
	BOOL bRet = CTitleRelation::BandRelation(nLeft, nRight);
	ReBuildCompare();
	return bRet;
}

BOOL CTitleCompare::BandRelationNormal(int nLeft, int nRight)
{
	BOOL bRet = CTitleRelation::BandRelationNormal(nLeft, nRight);
	ReBuildCompare();
	return bRet;
}

const COMPARE_STATE& CTitleCompare::GetItemCompareState(int nRow)
{
	return m_VtCompare[nRow];
}
