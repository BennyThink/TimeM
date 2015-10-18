#include "StdAfx.h"
#include "TitleRelation.h"
#include "TitleFile.h"
#include "TitleHelper.h"

CTitleRelation::CTitleRelation(void)
{
}

CTitleRelation::~CTitleRelation(void)
{
	m_ActLeft.DeleteContents();
	m_ActRight.DeleteContents();
}

int CTitleRelation::DoLocateOrgText(int nStart, const VT_TITLE& vtData, const CString& strTag)
{
	int nSize = (int)vtData.size();
	CString strFirst, strRemain;
	for(int iTitle = nStart; iTitle < nSize; iTitle ++)
	{
		CTitleHelper::BreakString(vtData[iTitle]->content, strFirst, strRemain);
		if(strRemain.CompareNoCase(strTag) == 0)
			return iTitle;
	}
	return -1;
}

int CTitleRelation::DoLocateStartTime(int nStart, const VT_TITLE& vtData, const int nStartTime)
{
	int nSize = (int)vtData.size();
	for(int iTitle = nStart; iTitle < nSize; iTitle ++)
	{
		if(nStartTime == vtData[iTitle]->nStart)
			return iTitle;
	}
	return -1;
}

int CTitleRelation::DoLocateEndTime(int nStart, const VT_TITLE& vtData, const int nEndTime)
{
	int nSize = (int)vtData.size();
	for(int iTitle = nStart; iTitle < nSize; iTitle ++)
	{
		if(nEndTime == vtData[iTitle]->nEnd)
			return iTitle;
	}
	return -1;
}

BOOL CTitleRelation::BuildRelation(UINT uType)
{
	PAIR_REL relPair;
	int nRightID = 0;
	int nLeftID = 0;

	relPair.nLeftID		= 0;
	relPair.nRightID	= 0;

	m_vtCombination.clear();

	while(1)
	{

		if(relPair.nLeftID >= m_ActLeft.GetItemCount())
		{
			PAIR_REL relAdd;
			for(int iRight = relPair.nRightID; iRight < m_ActRight.GetItemCount(); iRight ++)
			{
				relAdd.nRightID = iRight;
				relAdd.nLeftID = -1;
				m_vtCombination.push_back(relAdd);
			}
			break;
		}
		
		if(relPair.nRightID >= m_ActRight.GetItemCount())
		{
			PAIR_REL relAdd;
			for(int iLeft = relPair.nLeftID; iLeft < m_ActLeft.GetItemCount(); iLeft ++)
			{
				relAdd.nRightID = -1;
				relAdd.nLeftID = iLeft;
				m_vtCombination.push_back(relAdd);
			}
			break;
		}

		switch(uType)
		{
		case BUILD_TYPE_ORGTEXT:
			{
				CString	strFirst, strLeft, strRight;
				CTitleHelper::BreakString(m_ActLeft.GetItem(relPair.nLeftID, FALSE)->content, strFirst, strLeft);
				nRightID = DoLocateOrgText(relPair.nRightID, m_ActRight.GetVtTitle(), strLeft);

				CTitleHelper::BreakString(m_ActRight.GetItem(relPair.nRightID, FALSE)->content, strFirst, strRight);
				nLeftID = DoLocateOrgText(relPair.nLeftID, m_ActLeft.GetVtTitle(), strRight);
			}
			break;
		case BUILD_TYPE_STARTTIME:
			nRightID = DoLocateStartTime(relPair.nRightID, m_ActRight.GetVtTitle(), m_ActLeft.GetItem(relPair.nLeftID, FALSE)->nStart);
			nLeftID = DoLocateStartTime(relPair.nLeftID, m_ActLeft.GetVtTitle(), m_ActRight.GetItem(relPair.nRightID, FALSE)->nStart);
			break;
		case BUILD_TYPE_ENDTIME:
			nRightID = DoLocateEndTime(relPair.nRightID, m_ActRight.GetVtTitle(), m_ActLeft.GetItem(relPair.nLeftID, FALSE)->nEnd);
			nLeftID = DoLocateEndTime(relPair.nLeftID, m_ActLeft.GetVtTitle(), m_ActRight.GetItem(relPair.nRightID, FALSE)->nEnd);
			break;
		case BUILD_TYPE_BYNORMAL:
		default:
			nRightID = relPair.nRightID;
			nLeftID = relPair.nLeftID;
			break;
		}

		if((nRightID == -1)
			&&(nLeftID == -1))
		{
			//
			PAIR_REL relAdd;
			relAdd.nLeftID = relPair.nLeftID;
			relAdd.nRightID = -1;
			m_vtCombination.push_back(relAdd);
			relPair.nLeftID ++;
			//
			relAdd.nLeftID = -1;
			relAdd.nRightID = relPair.nRightID;
			m_vtCombination.push_back(relAdd);
			relPair.nRightID ++;
		}
		else if(nRightID == -1)
		{
			PAIR_REL relAdd;
			for(int iLeft = relPair.nLeftID; iLeft < nLeftID; iLeft ++)
			{
				relAdd.nLeftID = iLeft;
				relAdd.nRightID = -1;
				m_vtCombination.push_back(relAdd);
			}
			relAdd.nLeftID = nLeftID;
			relAdd.nRightID = relPair.nRightID;
			m_vtCombination.push_back(relAdd);

			relPair.nLeftID = nLeftID;
			//
			relPair.nLeftID ++;
			relPair.nRightID ++;
		}
		else if(nLeftID == -1)
		{
			PAIR_REL relAdd;
			for(int iRight = relPair.nRightID; iRight < nRightID; iRight ++)
			{
				relAdd.nRightID = iRight;
				relAdd.nLeftID = -1;
				m_vtCombination.push_back(relAdd);
			}
			relAdd.nLeftID = relPair.nLeftID;
			relAdd.nRightID = nRightID;
			m_vtCombination.push_back(relAdd);

			relPair.nRightID = nRightID;
			//
			relPair.nLeftID ++;
			relPair.nRightID ++;
		}
		else
		{
			if((nRightID - relPair.nRightID)
				<(nLeftID - relPair.nLeftID))
			{
				PAIR_REL relAdd;
				for(int iRight = relPair.nRightID; iRight < nRightID; iRight ++)
				{
					relAdd.nRightID = iRight;
					relAdd.nLeftID = -1;
					m_vtCombination.push_back(relAdd);
				}
				relAdd.nLeftID = relPair.nLeftID;
				relAdd.nRightID = nRightID;
				m_vtCombination.push_back(relAdd);

				relPair.nRightID = nRightID;
				//
				relPair.nLeftID ++;
				relPair.nRightID ++;
			}
			else
			{
				PAIR_REL relAdd;
				for(int iLeft = relPair.nLeftID; iLeft < nLeftID; iLeft ++)
				{
					relAdd.nLeftID = iLeft;
					relAdd.nRightID = -1;
					m_vtCombination.push_back(relAdd);
				}
				relAdd.nLeftID = nLeftID;
				relAdd.nRightID = relPair.nRightID;
				m_vtCombination.push_back(relAdd);

				relPair.nLeftID = nLeftID;
				//
				relPair.nLeftID ++;
				relPair.nRightID ++;
			}
		}
	}

	return FALSE;
}

BOOL CTitleRelation::BandRelation(int nLeft, int nRight)
{
	if(nLeft == nRight)
		return TRUE;
	if((nLeft < 0)
		||(nLeft > (int)m_vtCombination.size() - 1)
		||(nRight < 0)
		||(nRight > (int)m_vtCombination.size() - 1))
		return FALSE;

	VT_RELATION vtTmp;
	int nMin, nMax;
	nMin = (nLeft > nRight)?nRight:nLeft;
	nMax = (nLeft > nRight)?(nLeft + 1):(nRight + 1);

	for(int iItem = nMin; iItem < nMax; iItem ++)
	{
		vtTmp.push_back(m_vtCombination[iItem]);
	}

	for(int i = 0; i < nMax - nMin; i ++)
	{
		m_vtCombination[nMin + i].nLeftID = -1;
		vtTmp[i].nRightID = -1;
	}

	if(nLeft < nRight)
	{
		m_vtCombination[nRight].nLeftID = vtTmp[0].nLeftID;
		for(int i = 1; i < nMax - nMin; i ++)
		{
			m_vtCombination.insert(m_vtCombination.begin() + nRight + i, vtTmp[i]);
		}
	}
	else
	{
		m_vtCombination[nRight].nLeftID = vtTmp[nMax - nMin - 1].nLeftID;
		for(int i = 0; i < nMax - nMin - 1; i ++)
		{
			m_vtCombination.insert(m_vtCombination.begin() + nRight + i, vtTmp[i]);
		}
	}

	for(int i = nMax + nMax - nMin; i > nMin - 1; i --)
	{
		if((m_vtCombination[i].nLeftID == -1)
			&&(m_vtCombination[i].nRightID == -1))
			m_vtCombination.erase(m_vtCombination.begin() + i);
	}

	return TRUE;
}

BOOL CTitleRelation::BandRelationNormal(int nLeft, int nRight)
{
	if(nLeft == nRight)
		return TRUE;
	if((nLeft < 0)
		||(nLeft > (int)m_vtCombination.size() - 1)
		||(nRight < 0)
		||(nRight > (int)m_vtCombination.size() - 1))
		return FALSE;

	VT_RELATION vtTmp;
	int nMin, nMax;
	nMin = (nLeft > nRight)?nRight:nLeft;
	nMax = (nLeft > nRight)?nLeft:nRight;

	vtTmp = m_vtCombination;

	for(int i = 0; i < nMax - nMin; i ++)
	{
		m_vtCombination[nMin + i].nLeftID = -1;
		vtTmp[nMin + i].nRightID = -1;
	}

	if(nLeft < nRight)
	{
		m_vtCombination[nRight].nLeftID = vtTmp[nLeft].nLeftID;
		for(int i = nRight; i < (int)m_vtCombination.size(); i ++)
		{
			m_vtCombination[i].nLeftID = vtTmp[i - nMax + nMin].nLeftID;
		}
		for(int i = 0; i < nMax - nMin; i ++)
		{
			PAIR_REL relPair;
			relPair.nLeftID = vtTmp[vtTmp.size() - nMax + nMin + i].nLeftID;
			relPair.nRightID = -1;
			m_vtCombination.push_back(relPair);
		}
	}
	else
	{
		m_vtCombination[nRight].nLeftID = vtTmp[nLeft].nLeftID;
		for(int i = 0; i < nMax - nMin; i ++)
		{
			m_vtCombination.insert(m_vtCombination.begin() + nRight + i, vtTmp[nRight + i]);
		}

		for(int i = nLeft; i < (int)vtTmp.size(); i ++)
		{
			m_vtCombination[i].nLeftID = vtTmp[i].nLeftID;
		}
		for(int i = (int)vtTmp.size(); i < (int)m_vtCombination.size(); i ++)
		{
			m_vtCombination[i].nLeftID = -1;
		}
	}

	for(int i = m_vtCombination.size() - 1; i > nMin; i --)
	{
		if((m_vtCombination[i].nLeftID == -1)
			&&(m_vtCombination[i].nRightID == -1))
			m_vtCombination.erase(m_vtCombination.begin() + i);
	}

	return TRUE;
}
