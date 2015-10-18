#include "StdAfx.h"
#include "ActionHelper.h"

CActionHelper::CActionHelper()
{
	m_pvtAction = NULL;
}

void CActionHelper::ClearAllAction()
{
	WriteLogLine(_T("ClearStack(m_stkRedo)(%d)\r\n"), m_stkRedo.size());
	ClearStack(m_stkRedo);
	WriteLogLine(_T("ClearStack(m_stkRedo)(%d)\r\n"), m_stkRedo.size());
	WriteLogLine(_T("ClearStack(m_stkUndo)(%d)\r\n"), m_stkUndo.size());
	ClearStack(m_stkUndo);
	WriteLogLine(_T("ClearStack(m_stkUndo)(%d)\r\n"), m_stkUndo.size());
}

CActionHelper::~CActionHelper(void)
{
	ClearAllAction();
}

////////////////
//
void CActionHelper::BeginRecordAction()
{
	if(m_pvtAction != NULL)
		delete m_pvtAction;
	m_pvtAction = new VT_ACTION;
	WriteLogLine(_T("BeginRecordAction m_pvtAction(0x%08x)\r\n"), m_pvtAction);
}

void CActionHelper::EndRecordAction()
{
	if(m_pvtAction == NULL)
		return;
	if(m_pvtAction->size() != 0)
	{
		WriteLogLine(_T("EndRecordAction m_pvtAction(0x%08x)\r\n"), m_pvtAction);
		m_stkUndo.push(m_pvtAction);
		ClearStack(m_stkRedo);
	}
	else
	{
		WriteLogLine(_T("EndRecordAction m_pvtAction(0x%08x), Empty erase!\r\n"), m_pvtAction);
		delete m_pvtAction;
	}
	m_pvtAction = NULL;
}

void CActionHelper::PopLastRecord()
{
	if(m_pvtAction->size() > 0)
	{
		WriteLogLine(_T("PopLastRecord() give up last record!(%d, %d, %s)\r\n"),
			m_pvtAction->at(m_pvtAction->size() - 1)->nActionType,
			m_pvtAction->at(m_pvtAction->size() - 1)->nActionPos,
			m_pvtAction->at(m_pvtAction->size() - 1)->unitTitle.content);
		delete m_pvtAction->at(m_pvtAction->size() - 1);
		m_pvtAction->pop_back();
	}
}

void CActionHelper::ClearStack(STK_ACTION& stkAction)
{
	PVT_ACTION pVtAct = NULL;
	while(stkAction.size() > 0)
	{
		pVtAct = stkAction.top();
		stkAction.pop();
		while(pVtAct->size() > 0)
		{
			delete pVtAct->at(pVtAct->size() - 1);
			pVtAct->pop_back();
		}
		delete pVtAct;
		pVtAct = NULL;
	}
}

/////////////////////////
//	insert
BOOL CActionHelper::Insert(int nPos, PTITLE_UNIT pUnit)
{
	m_vtTitle.insert(m_vtTitle.begin() + nPos, pUnit);
	if(m_pvtAction == NULL)
		return FALSE;
	PACTION_UNIT pAUnit		= new ACTION_UNIT;
	pAUnit->nActionType			= ACTTYPE_INSERT;
	pAUnit->nActionPos			= nPos;
	pAUnit->unitTitle.nStart	= pUnit->nStart;
	pAUnit->unitTitle.nEnd		= pUnit->nEnd;
	pAUnit->unitTitle.content	= pUnit->content;
	pAUnit->unitTitle.dwFlags	= pUnit->dwFlags;
	WriteLogLine(_T("CActionHelper::Insert(%d, %s)\r\n"), nPos, pUnit->content);
	m_pvtAction->push_back(pAUnit);
	return TRUE;
}

BOOL CActionHelper::PushBack(PTITLE_UNIT pUnit)
{
	m_vtTitle.push_back(pUnit);
	if(m_pvtAction == NULL)
		return FALSE;
	PACTION_UNIT pAUnit		= new ACTION_UNIT;
	pAUnit->nActionType			= ACTTYPE_INSERT;
	pAUnit->nActionPos			= m_vtTitle.size() - 1;
	pAUnit->unitTitle.nStart	= pUnit->nStart;
	pAUnit->unitTitle.nEnd		= pUnit->nEnd;
	pAUnit->unitTitle.content	= pUnit->content;
	pAUnit->unitTitle.dwFlags	= pUnit->dwFlags;
	WriteLogLine(_T("CActionHelper::PushBack(%s)\r\n"), pUnit->content);
	m_pvtAction->push_back(pAUnit);
	return TRUE;
}

//	delete
BOOL CActionHelper::PopBack()
{
	if(m_vtTitle.size() == 0)
		return FALSE;

	if(m_pvtAction == NULL)
		return FALSE;
	PACTION_UNIT pAUnit		= new ACTION_UNIT;
	pAUnit->nActionType			= ACTTYPE_DELETE;
	pAUnit->nActionPos			= m_vtTitle.size() - 1;
	pAUnit->unitTitle.nStart		= m_vtTitle[pAUnit->nActionPos]->nStart;
	pAUnit->unitTitle.nEnd		= m_vtTitle[pAUnit->nActionPos]->nEnd;
	pAUnit->unitTitle.content	= m_vtTitle[pAUnit->nActionPos]->content;
	pAUnit->unitTitle.dwFlags	= m_vtTitle[pAUnit->nActionPos]->dwFlags;
	WriteLogLine(_T("CActionHelper::PopBack(%s)\r\n"), pAUnit->unitTitle.content);
	m_pvtAction->push_back(pAUnit);

	delete m_vtTitle[m_vtTitle.size() - 1];
	m_vtTitle.pop_back();
	return TRUE;
}

BOOL CActionHelper::Erase(int nPos)
{
	if((nPos < 0)||(nPos > ((int)m_vtTitle.size() - 1)))
		return FALSE;

	if(m_pvtAction == NULL)
		return FALSE;
	PACTION_UNIT pAUnit		= new ACTION_UNIT;
	pAUnit->nActionType			= ACTTYPE_DELETE;
	pAUnit->nActionPos			= nPos;
	pAUnit->unitTitle.nStart		= m_vtTitle[nPos]->nStart;
	pAUnit->unitTitle.nEnd		= m_vtTitle[nPos]->nEnd;
	pAUnit->unitTitle.content	= m_vtTitle[nPos]->content;
	pAUnit->unitTitle.dwFlags	= m_vtTitle[nPos]->dwFlags;
	WriteLogLine(_T("CActionHelper::Erase(%d, %s)\r\n"), nPos, pAUnit->unitTitle.content);
	m_pvtAction->push_back(pAUnit);
	delete m_vtTitle[nPos];
	m_vtTitle.erase(m_vtTitle.begin() + nPos);
	return TRUE;
}

// modify
BOOL CActionHelper::SaveRecord(int nPos)
{
	if(m_pvtAction == NULL)
		return FALSE;
	PACTION_UNIT pAUnit		= new ACTION_UNIT;
	pAUnit->nActionType			= ACTTYPE_MODIFY;
	pAUnit->nActionPos			= nPos;
	pAUnit->unitTitle.nStart		= m_vtTitle[nPos]->nStart;
	pAUnit->unitTitle.nEnd		= m_vtTitle[nPos]->nEnd;
	pAUnit->unitTitle.content	= m_vtTitle[nPos]->content;
	pAUnit->unitTitle.dwFlags	= m_vtTitle[nPos]->dwFlags;
	WriteLogLine(_T("CActionHelper::SaveRecord(%d, %s)\r\n"), nPos, pAUnit->unitTitle.content);
	m_pvtAction->push_back(pAUnit);
	return TRUE;
}

// sort
BOOL CActionHelper::SaveAllUndo()
{
	int nSize = m_vtTitle.size();
	for(int iTitle = 0; iTitle < nSize; iTitle ++)
	{
		SaveRecord(iTitle);
	}
	return TRUE;
}

/////////////////////////////////////
//
int CActionHelper::Undo()
{
	int nPos = -1;
	if(m_stkUndo.size() ==  0)
		return FALSE;
	PVT_ACTION pVtUAct = m_stkUndo.top();

	VT_ACTION::reverse_iterator rIter;
	PACTION_UNIT pActUnit = NULL;

	WriteLogLine(_T("CActionHelper::Undo(Total Undo %d, Total Redo %d, Current List cnt %d)\r\n"),
		m_stkUndo.size(),
		m_stkRedo.size(),
		pVtUAct->size());

	for(rIter = pVtUAct->rbegin(); rIter != pVtUAct->rend(); rIter ++)
	{
		nPos = (*rIter)->nActionPos;
		UndoUnit(*rIter);
	}

	m_stkUndo.pop();
	m_stkRedo.push(pVtUAct);

	return nPos;
}

int CActionHelper::Redo()
{
	int nPos = -1;
	if(m_stkRedo.size() ==  0)
		return FALSE;
	PVT_ACTION pVtRAct = m_stkRedo.top();

	VT_ACTION::iterator iter;
	PACTION_UNIT pActUnit = NULL;

	WriteLogLine(_T("CActionHelper::Redo(Total Undo %d, Total Redo %d, Current List cnt %d)\r\n"),
		m_stkUndo.size(),
		m_stkRedo.size(),
		pVtRAct->size());

	for(iter = pVtRAct->begin(); iter != pVtRAct->end(); iter ++)
	{
		nPos = (*iter)->nActionPos;
		RedoUnit(*iter);
	}

	m_stkRedo.pop();
	m_stkUndo.push(pVtRAct);

	return nPos;
}

BOOL CActionHelper::UndoUnit(PACTION_UNIT pActUnit)
{
	WriteLogLine(_T("CActionHelper::UndoUnit(%d, %d, %s)\r\n"),
		pActUnit->nActionType,
		pActUnit->nActionPos,
		pActUnit->unitTitle.content);

	switch(pActUnit->nActionType)
	{
	case ACTTYPE_INSERT:
		delete m_vtTitle[pActUnit->nActionPos];
		m_vtTitle.erase(m_vtTitle.begin() + pActUnit->nActionPos);
		break;
	case ACTTYPE_DELETE:
		{
			PTITLE_UNIT pUnit = new TITLE_UNIT;
			InitializeUnit(pUnit);
			pUnit->nStart	= pActUnit->unitTitle.nStart;
			pUnit->nEnd		= pActUnit->unitTitle.nEnd;
			pUnit->content	= pActUnit->unitTitle.content;
			pUnit->dwFlags	= pActUnit->unitTitle.dwFlags;
			m_vtTitle.insert(m_vtTitle.begin() + pActUnit->nActionPos, pUnit);
		}
		break;
	case ACTTYPE_MODIFY:
		{
			TITLE_UNIT unitTmp;
			unitTmp.nStart	= m_vtTitle[pActUnit->nActionPos]->nStart;
			unitTmp.nEnd		= m_vtTitle[pActUnit->nActionPos]->nEnd;
			unitTmp.content	= m_vtTitle[pActUnit->nActionPos]->content;
			unitTmp.dwFlags	= m_vtTitle[pActUnit->nActionPos]->dwFlags;
			m_vtTitle[pActUnit->nActionPos]->nStart		= pActUnit->unitTitle.nStart;
			m_vtTitle[pActUnit->nActionPos]->nEnd		= pActUnit->unitTitle.nEnd;
			m_vtTitle[pActUnit->nActionPos]->content	= pActUnit->unitTitle.content;
			m_vtTitle[pActUnit->nActionPos]->dwFlags	= pActUnit->unitTitle.dwFlags;
			pActUnit->unitTitle.nStart	= unitTmp.nStart;
			pActUnit->unitTitle.nEnd		= unitTmp.nEnd;
			pActUnit->unitTitle.content	= unitTmp.content;
			pActUnit->unitTitle.dwFlags	= unitTmp.dwFlags;
		}
		break;
	}
	return TRUE;
}

BOOL CActionHelper::RedoUnit(PACTION_UNIT pActUnit)
{
	WriteLogLine(_T("CActionHelper::RedoUnit(%d, %d, %s)\r\n"),
		pActUnit->nActionType,
		pActUnit->nActionPos,
		pActUnit->unitTitle.content);
	switch(pActUnit->nActionType)
	{
	case ACTTYPE_INSERT:
		{
			PTITLE_UNIT pUnit = new TITLE_UNIT;
			InitializeUnit(pUnit);
			pUnit->nStart	= pActUnit->unitTitle.nStart;
			pUnit->nEnd		= pActUnit->unitTitle.nEnd;
			pUnit->content	= pActUnit->unitTitle.content;
			pUnit->dwFlags	= pActUnit->unitTitle.dwFlags;
			m_vtTitle.insert(m_vtTitle.begin() + pActUnit->nActionPos, pUnit);
		}
		break;
	case ACTTYPE_DELETE:
		delete m_vtTitle[pActUnit->nActionPos];
		m_vtTitle.erase(m_vtTitle.begin() + pActUnit->nActionPos);
		break;
	case ACTTYPE_MODIFY:
		{
			TITLE_UNIT unitTmp;
			unitTmp.nStart	= m_vtTitle[pActUnit->nActionPos]->nStart;
			unitTmp.nEnd		= m_vtTitle[pActUnit->nActionPos]->nEnd;
			unitTmp.content	= m_vtTitle[pActUnit->nActionPos]->content;
			unitTmp.dwFlags	= m_vtTitle[pActUnit->nActionPos]->dwFlags;
			m_vtTitle[pActUnit->nActionPos]->nStart		= pActUnit->unitTitle.nStart;
			m_vtTitle[pActUnit->nActionPos]->nEnd		= pActUnit->unitTitle.nEnd;
			m_vtTitle[pActUnit->nActionPos]->content	= pActUnit->unitTitle.content;
			m_vtTitle[pActUnit->nActionPos]->dwFlags	= pActUnit->unitTitle.dwFlags;
			pActUnit->unitTitle.nStart	= unitTmp.nStart;
			pActUnit->unitTitle.nEnd		= unitTmp.nEnd;
			pActUnit->unitTitle.content	= unitTmp.content;
			pActUnit->unitTitle.dwFlags	= unitTmp.dwFlags;
		}
		break;
	}
	return TRUE;
}

BOOL CActionHelper::IsCanUndo()
{
	return (m_stkUndo.size() > 0);
}

BOOL CActionHelper::IsCanRedo()
{
	return (m_stkRedo.size() > 0);
}

/////////////////////////////////////
//
PTITLE_UNIT CActionHelper::GetItem(int nPos, BOOL bSave)
{
	if((nPos < 0)||(nPos > ((int)m_vtTitle.size() - 1)))
		return NULL;
	if(bSave)
		SaveRecord(nPos);
	return m_vtTitle[nPos];
}

void CActionHelper::DeleteContents()
{
	int nSize = (int)m_vtTitle.size();
	for(int iTitle = nSize - 1; iTitle > -1; iTitle --)
	{
		delete m_vtTitle[iTitle];
		m_vtTitle.pop_back();
	}
}
