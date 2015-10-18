#pragma once
#include "TitleUnit.h"

class CActionHelper
{
public:
	CActionHelper();
	~CActionHelper(void);
protected:
	VT_TITLE	m_vtTitle;

	PVT_ACTION	m_pvtAction;
	STK_ACTION	m_stkUndo;
	STK_ACTION	m_stkRedo;
public:
	void BeginRecordAction();
	void EndRecordAction();

	void PopLastRecord();
	void ClearAllAction();
	void ClearStack(STK_ACTION& stkAction);

	// insert
	BOOL Insert(int nPos, PTITLE_UNIT pUnit);
	BOOL PushBack(PTITLE_UNIT pUnit);

	// delete
	BOOL Erase(int nPos);
	BOOL PopBack();

	// modify
	BOOL SaveRecord(int nPos);

	// sort
	BOOL SaveAllUndo();

	/////////////////////
	int Undo();
	int Redo();

	BOOL IsCanUndo();
	BOOL IsCanRedo();

	BOOL UndoUnit(PACTION_UNIT pActUnit);
	BOOL RedoUnit(PACTION_UNIT pActUnit);

	/////////////////////
	PTITLE_UNIT GetItem(int nPos, BOOL bSave = TRUE);
	VT_TITLE& GetVtTitle(){return m_vtTitle;};
	int GetItemCount(){return (int)m_vtTitle.size();}
	void DeleteContents();
};
