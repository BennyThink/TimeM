// TimeMDoc.h : CTimeMDoc 类的接口
//


#pragma once

#include "TitleUnit.h"
#include "ActionHelper.h"
#include "TextFindDlg.h"
#include "CheckValidDlg.h"
#include "ExportOptionDlg.h"
#include "MultiReplaceDlg.h"

#define	STATE_READING	1
#define	STATE_BUFEND	2
#define	STATE_FILEEND	3

class CTitleLView;
class CPropertiesWnd;
class CTitlePaneDlg;

class CTimeMDoc : public CDocument
{
protected: // 仅从序列化创建
	CTimeMDoc();
	DECLARE_DYNCREATE(CTimeMDoc)

// 属性
protected:
	CString			m_strPreCode;
	CString			m_strPostCode;
	CString			m_strFmtHeader;
	BOOL			m_bBakModified;
	LONG			m_nSaveState;
	CCriticalSection	m_CriticalSect;
public:

	DWORD			m_dwCharsetForDisp;

	BOOL			m_bUnicode;
	CActionHelper	m_Action;
	int				m_nPos;

	int				m_nSortCol;
	BOOL			m_bAscending;

	BOOL			m_bAutoSnap;
	UINT			m_nSnapCount;

	BOOL			m_bThdBreak;
	BOOL			m_bSyncRunning;
	BOOL			m_bUseSync;
	HANDLE			m_hSyncThd;
	CTextFindDlg	m_dlgFind;

	CCheckValidDlg	m_dlgCheckValid;
	VT_CHKPARAMS	m_vtChkParams;
	MAP_CHKPARAMS	m_mapChkParams;

	CMultiReplaceDlg m_dlgReplace;

	CPropertiesWnd*	m_PlayerWnd;
	CTitlePaneDlg*	m_DetailWnd;

	void	SavePlayState();
	void	RestorePlayState();
// 操作
public:
	BOOL	GetBakModifiedFlag(){return m_bBakModified;};
	void	SetBakModifiedFlag(BOOL bModified){m_bBakModified = bModified;};
	void	DoBackup();
	virtual void SortChanged(int nCol, BOOL bAscending);
	virtual BOOL CanSort(int nCol);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void	SetModifiedFlagAutoSave(BOOL bModified = TRUE);

	DWORD	GetDispCharset(){return m_dwCharsetForDisp;};
	void	SelectItem(int nItem);
	int		LookForUpLocation(const VT_TITLE& vtTag, const CString& strTag);
	int		LookForDnLocation(const VT_TITLE& vtTag, const CString& strTag);
	int		GetSortCol(){return m_nSortCol;};
	BOOL	IsSortAscending(){return m_bAscending;};

	BOOL	IsCanUndo();
	BOOL	IsCanRedo();

	BOOL	OpenTransFile(CFile* pFile);
	BOOL	QuerySplitOption();

	BOOL	OpenAppendFile(CFile* pFile);

	void	InsertTitle(int nPos, const CString& strStart, const CString& strEnd, const CString& strTitle);
	void	InsertOneTitle(int nPos, LPCTSTR lpszTitle, int nTimeLen);

	int		GetCurrentPos();
	void	SetCurrentPos(int nPos);

	void	SafeChangePos(int nPos);

	void	MarkItem(int nItem);
	BOOL	HasMark(int nItem);
	int		LocateNextMarkItem();
	int		LocatePrevMarkItem();

	BOOL	SaveTitleItem(int nPos, const CString& strStart, const CString& strEnd, const CString& strContent);
	BOOL	SaveCurrentTitleItem(const CString& strStart, const CString& strEnd, const CString& strContent);
	const PTITLE_UNIT GetTitleItem(int nPos);
	int		GetTitleCount();

	void	SyncVideoTitle();
	int		IsLocateInTitleUnit(int nItemPos, int nPlayPos);

	int		GetSnapOffset();
	void	SnapLines(int nStart, int nEnd, int nOffset);

	// CC预处理
	void	PreProcessCC();
	void	InsertPreTrans(PTITLE_UNIT pUnit);

	/////////
	void	ExchangeTitleLine(int nItem);

	void	DoReplace();
	void	EditReplace(const CString& strTarget, const CString& strResult, BOOL bCase, BOOL bEng, BOOL bChs, BOOL bSelected);
// 重写
public:
	void UpdateRefWin(CWnd* pWnd = NULL);

	virtual HACCEL GetDefaultAccelerator();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	void ChkClear();
	void ChkExceedSpecChs(int nMaxChs);
	void ChkStartMoreEnd();
	void ChkTimeOverlap();
	void ChkTimeEmpty();
	void ChkTimeMoreThan(int nMaxTime);
	void ChkTimeLessThan(int nMinTime);
	void ChkTitleLineMoreThan(int nLineMaxCnt);
	void ClearOverlap();

	void GenerateName(LPTSTR lpszFile, LPCTSTR pOrgName, LPCTSTR lpszLangName, WORD wFmtID);
	void WriteExpTitleFile(LPCTSTR lpszFileName, 
		VT_TITLE& vtTitle, int iStart, int iEnd,
		BOOL bUnicode, UINT nCPCode, 
		DWORD dwFmt, BOOL bUseCode);
	void ZipFile(LPCTSTR lpszFileName, LPCTSTR lpszOutPath, const CStringArray& strSrcFiles);
	void TransData(VT_TITLE& vtExportTo, VT_TITLE& vtSrc, WORD wFmtID, WORD wLangFlag);

	CString GetAutoSaveFile(LPCTSTR lpszPathName);
	BOOL WriteToFile(LPCTSTR lpszFileName);
// 实现
public:
	virtual ~CTimeMDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
public:
	virtual void DeleteContents();

	int				GetExtraTime();
	int				GetMoveOffset(DWORD& dwFlag);
	CTitleLView*	GetTitleView();
	CPropertiesWnd*	GetPlayerWnd();
	CTitlePaneDlg*	GetDetailWnd();
	BOOL			IsInPlaying();
	BOOL			IsInPause();

public:
	int		DoSearch(int nStartFrom, BOOL bCase, BOOL bDN, const CString& strToSearch);
	void	TitleExtend(int nItem, int nExtra);
	void	SetPlayPos(int nItem, BOOL bStart);
	void	ToEditTitle(int nItem);
	void	OnTitleMovetime(int nOffset, int nSel);
	void	OnTitleDelete(int nSel);
	void	OnTitleZerotime(int nSel);
	void	OnTitleEmptycontent(int nSel);
public:
	afx_msg void OnTitleMovetime();
	afx_msg void OnTitleInsert();
	afx_msg void OnTitleMultiinsert();
	afx_msg void OnFileImport();
	afx_msg void OnFileAppend();
	afx_msg void OnTitleDelete();
	afx_msg void OnTitleExchange();
	afx_msg void OnTitleBreaktwo();
	afx_msg void OnTitleBreakbyuser();
	afx_msg void OnTitleCboneline();
	afx_msg void OnTitleCbmultiline();
	afx_msg void OnTitleTo25fps();
	afx_msg void OnTitleFrom25fps();
	afx_msg void OnTitleZerotime();
	afx_msg void OnTitleEmptycontent();
	afx_msg void OnTitleSnapline();
	afx_msg void OnTitleSnapafter();
	afx_msg void OnTitleSnapall();
	afx_msg void OnTitleSnaplineend();
	afx_msg void OnTitleSnaplinestart();
	afx_msg void OnTitleStartset();
	afx_msg void OnTitleExtend();

	afx_msg void OnVideoFileOpen();
	afx_msg void OnVideoPlayPause();
	afx_msg void OnVideoStop();
	afx_msg void OnVideoStepone();
	afx_msg void OnVideoForward1s();
	afx_msg void OnVideoBack1s();
	afx_msg void OnVideoForward5s();
	afx_msg void OnVideoBack5s();
	afx_msg void OnToolPreproc();
	afx_msg void OnVideoSynctitle();
	afx_msg void OnUpdateVideoSynctitle(CCmdUI *pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditFind();
	afx_msg void OnEditReplace();
	afx_msg void OnEditUndo();
	afx_msg void OnEditFindnext();
	afx_msg void OnToolCmbytime();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditFindnext(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnTitleUpdateCmdUIRange(CCmdUI* pCmdUI);
	afx_msg void OnTitleSnapUpdateCmdUIRange(CCmdUI* pCmdUI);
	afx_msg void OnCharsetDispUpdateUI(CCmdUI* pCmdUI);
	afx_msg void OnTitleCBUpdateCmdUI(CCmdUI* pCmdUI);
	afx_msg void OnEditSortstart();
	afx_msg void OnEditSortend();
	afx_msg void OnEditSortcontent();
	afx_msg void OnEditSortlen();
	afx_msg void OnEditMark();
	afx_msg void OnEditGotomark();
	afx_msg void OnToolCmbyuser();
	afx_msg void OnToolCompare();
	afx_msg void OnToolCheck();
	afx_msg void OnUpdateToolCheck(CCmdUI *pCmdUI);
	afx_msg void OnFileExport();
	afx_msg void OnTitleAutosnap();
	afx_msg void OnTitleSnapnext();
	afx_msg void OnUpdateTitleAutosnap(CCmdUI *pCmdUI);
	afx_msg void OnDefaultCharset();
	afx_msg void OnBig5Charset();
	afx_msg void OnGb2312Charset();
	afx_msg void OnUpdateFileExport(CCmdUI *pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnOptionsAsssettings();
	afx_msg void OnToolAddauthor();
	afx_msg void OnTitlePrevline();
	afx_msg void OnTitleNextline();
	afx_msg void OnUpdateTitlePrevline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTitleNextline(CCmdUI *pCmdUI);
	afx_msg void OnTitleSnapprevmark();
	afx_msg void OnTitleSnapbetweentwomark();
	afx_msg void OnEditGotoprevmark();
	afx_msg void OnTitleDelbycond();
	afx_msg void OnTitleSinglelinetitle();
	afx_msg void OnFileSplitto();
	afx_msg void OnOptionAutosave();
	afx_msg void OnUpdateOptionAutosave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditReplace(CCmdUI *pCmdUI);
	//afx_msg void OnFileExportfu();

};
