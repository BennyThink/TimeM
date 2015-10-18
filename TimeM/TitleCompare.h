#pragma once
#include "titlerelation.h"

#define	COMPARE_EQUAL		0x00
#define	COMPARE_NOTEQUAL	0x01
#define	COMPARE_LEFTEMPTY	0x02
#define	COMPARE_RIGHTEMPTY	0x03

typedef struct tagCompareState
{
	int	nId;
	int nStartTimeState;
	int	nEndTimeState;
	int nContentState;
}COMPARE_STATE, *PCOMPARE_STATE;

typedef vector<COMPARE_STATE> VT_COMPARE;

class CTitleCompare :
	public CTitleRelation
{
public:
	CTitleCompare(void);
	~CTitleCompare(void);
protected:
	VT_COMPARE m_VtCompare;
public:
	void ReBuildCompare();

	BOOL BuildRelation(UINT uType);
	BOOL BandRelation(int nLeft, int nRight);
	BOOL BandRelationNormal(int nLeft, int nRight);

	const COMPARE_STATE& GetItemCompareState(int nRow);
};
