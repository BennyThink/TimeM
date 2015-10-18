#pragma once

#include "ActionHelper.h"

typedef struct tagRelationPair
{
	int nLeftID;
	int nRightID;
}PAIR_REL, *PPAIR_REL;

typedef vector<PAIR_REL> VT_RELATION;

#define	BUILD_TYPE_BYNORMAL		0x00
#define	BUILD_TYPE_ORGTEXT		0x01
#define	BUILD_TYPE_STARTTIME	0x02
#define	BUILD_TYPE_ENDTIME		0x03

class CTitleRelation
{
public:
	CTitleRelation(void);
	~CTitleRelation(void);
protected:
	CActionHelper	m_ActLeft;
	CActionHelper	m_ActRight;
	VT_RELATION		m_vtCombination;

protected:
	int DoLocateOrgText(int nStart, const VT_TITLE& vtData, const CString& strTag);
	int DoLocateStartTime(int nStart, const VT_TITLE& vtData, const int nStartTime);
	int DoLocateEndTime(int nStart, const VT_TITLE& vtData, const int nEndTime);
public:
	CActionHelper& GetLeftActor(){return m_ActLeft;};
	CActionHelper& GetRightActor(){return m_ActRight;};

	VT_TITLE& GetLeftVT(){return m_ActLeft.GetVtTitle();};
	VT_TITLE& GetRightVT(){return m_ActRight.GetVtTitle();};

	int GetRelCount(){return m_vtCombination.size();};
	PAIR_REL GetRelItem(int nItem){return m_vtCombination[nItem];};

	BOOL BuildRelation(UINT uType);
	BOOL BandRelation(int nLeft, int nRight);
	BOOL BandRelationNormal(int nLeft, int nRight);
};
