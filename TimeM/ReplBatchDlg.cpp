// ReplBatchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "ReplBatchDlg.h"


// CReplBatchDlg

IMPLEMENT_DYNAMIC(CReplBatchDlg, CPaneDialog)

CReplBatchDlg::CReplBatchDlg()
	: m_strTarget(_T(""))
	, m_strResult(_T(""))
	, m_bCase(FALSE)
	, m_bForSelected(FALSE)
	, m_bUseBatch(FALSE)
	, m_bIncChs(TRUE)
	, m_bIncEng(FALSE)
	, m_bModified(FALSE)
	, m_strTplFileName(_T(""))
{

}

CReplBatchDlg::~CReplBatchDlg()
{
}


BEGIN_MESSAGE_MAP(CReplBatchDlg, CPaneDialog)
	ON_BN_CLICKED(IDC_CHK_USEBATCH, &CReplBatchDlg::OnBnClickedChkUsebatch)
	ON_BN_CLICKED(IDC_BTN_ADDITEM, &CReplBatchDlg::OnBnClickedBtnAdditem)
	ON_BN_CLICKED(IDC_BTN_REMOVE, &CReplBatchDlg::OnBnClickedBtnRemove)
	ON_BN_CLICKED(IDC_BTN_LOADTPL, &CReplBatchDlg::OnBnClickedBtnLoadtpl)
	ON_BN_CLICKED(IDC_BTN_SAVETPL, &CReplBatchDlg::OnBnClickedBtnSavetpl)
	ON_EN_CHANGE(IDC_EDIT_TARGETTEXT, &CReplBatchDlg::OnEnChangeEditTargettext)
	ON_MESSAGE(MSG_LISTITEM_CHANGED, &CReplBatchDlg::OnListItemChanged)
	ON_BN_CLICKED(IDC_CHK_INCCHS, &CReplBatchDlg::OnBnClickedChkIncchs)
	ON_BN_CLICKED(IDC_CHK_INCENG, &CReplBatchDlg::OnBnClickedChkInceng)
	ON_BN_CLICKED(IDC_BTN_CLOSETPL, &CReplBatchDlg::OnBnClickedBtnClosetpl)
	ON_BN_CLICKED(IDOK, &CReplBatchDlg::OnBnClickedOk)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CReplBatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CPaneDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_INCCHS, m_bIncChs);
	DDX_Check(pDX, IDC_CHK_INCENG, m_bIncEng);
	DDX_Check(pDX, IDC_CHK_CASESENSE, m_bCase);
	DDX_Text(pDX, IDC_EDIT_TARGETTEXT, m_strTarget);
	DDX_Text(pDX, IDC_EDIT_RESULTTEXT, m_strResult);
	DDX_Check(pDX, IDC_CHK_USEBATCH, m_bUseBatch);
	DDX_Control(pDX, IDC_LIST_RPLITEMS, m_RplItemList);
	DDX_Text(pDX, IDC_EDIT_TPLFILE, m_strTplFileName);
	if(pDX->m_bSaveAndValidate)
		m_bForSelected = IsDlgButtonChecked(IDC_RBTN_SELECTED);
	else
	{
		if(m_bForSelected)
			CheckRadioButton(IDC_RBTN_SELECTED, IDC_RBTN_ALL, IDC_RBTN_SELECTED);
		else
			CheckRadioButton(IDC_RBTN_SELECTED, IDC_RBTN_ALL, IDC_RBTN_ALL);
	}
}

// CReplBatchDlg message handlers

void CReplBatchDlg::UpdateCtrlStates()
{
	UpdateData();
	CWnd* pWndOK = GetDlgItem(IDOK);
	CWnd* pWndAdd = GetDlgItem(IDC_BTN_ADDITEM);

	if(m_strTarget.IsEmpty())
	{
		pWndAdd->EnableWindow(FALSE);
	}
	else
	{
		pWndAdd->EnableWindow();
	}

	if((!m_bIncChs)&&(!m_bIncEng))
		pWndOK->EnableWindow(FALSE);
	else
	{
		if(m_bUseBatch)
		{
			pWndOK->EnableWindow(FALSE);
			int nCnt = m_RplItemList.GetItemCount();
			for(int iItem = 0; iItem < nCnt; iItem ++)
			{
				if(m_RplItemList.GetCheck(iItem))
					pWndOK->EnableWindow();
			}
		}
		else
		{
			if(m_strTarget.IsEmpty())
			{
				pWndOK->EnableWindow(FALSE);
			}
			else
			{
				pWndOK->EnableWindow();
			}
		}
	}
}

void CReplBatchDlg::OnBnClickedChkUsebatch()
{
	UpdateData();
	UpdateCtrlStates();
}

void CReplBatchDlg::OnBnClickedBtnAdditem()
{
	UpdateData();

	int nItem = m_RplItemList.InsertItem(m_RplItemList.GetItemCount(), _T(""));
	m_RplItemList.SetItemText(nItem, 1, m_strTarget);
	m_RplItemList.SetItemText(nItem, 2, m_strResult);
	m_bModified = TRUE;
	UpdateCtrlStates();
}

void CReplBatchDlg::OnBnClickedBtnRemove()
{

	LISTSEL_MAP mapDel;
	POSITION posSel = m_RplItemList.GetFirstSelectedItemPosition();
	while(posSel != NULL)
	{
		int nItem = m_RplItemList.GetNextSelectedItem(posSel);
		mapDel.insert(INT_PAIR(nItem, nItem));
	}
	LISTSEL_MAP::reverse_iterator itrRDel;
	for(itrRDel =  mapDel.rbegin(); itrRDel != mapDel.rend(); itrRDel++)
	{
		m_RplItemList.DeleteItem(itrRDel->first);
		m_bModified = TRUE;
	}
	UpdateCtrlStates();
}

BOOL CReplBatchDlg::DoPromptOpenTpl()
{
	TCHAR szFilter[] = _T("批量替换模板文件(*.txt)|*.txt|All Files (*.*)|*.*||");

	CFileDialog dlgTplFile(TRUE, 0, NULL, 0, szFilter);
	if(IDOK == dlgTplFile.DoModal())
	{
		OnBnClickedBtnClosetpl();
		m_strTplFileName = dlgTplFile.GetPathName();
		UpdateData(FALSE);
		return TRUE;
	}
	return FALSE;
}

void CReplBatchDlg::OnBnClickedBtnLoadtpl()
{
	CStdioFile fileOpen;
	if(!DoPromptOpenTpl())
		return;
	CString strTarget;
	CString strResult;
	CString strLine;
	
	if(fileOpen.Open(m_strTplFileName, CFile::modeRead))
	{
		while(1)
		{
			if((fileOpen.ReadString(strTarget))
				&&(fileOpen.ReadString(strResult)))
			{
				int nItem = m_RplItemList.InsertItem(m_RplItemList.GetItemCount(), _T(""));
				m_RplItemList.SetItemText(nItem, 1, strTarget);
				m_RplItemList.SetItemText(nItem, 2, strResult);
			}
			else
			{
				break;
			}

			if(fileOpen.ReadString(strLine)
				&&strLine.IsEmpty())
				continue;
			else
				break;
		}

		m_bModified = FALSE;
		fileOpen.Close();
		UpdateData(FALSE);
	}
}

BOOL CReplBatchDlg::DoPromptSaveTpl()
{
	TCHAR szFilter[] = _T("批量替换模板文件(*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlgSave(FALSE, 0, 0, OFN_OVERWRITEPROMPT, szFilter);
	if(IDOK == dlgSave.DoModal())
	{
		m_strTplFileName = dlgSave.GetPathName();
		m_bModified = TRUE;
		UpdateData(FALSE);
		return TRUE;
	}
	return FALSE;
}

void CReplBatchDlg::OnBnClickedBtnSavetpl()
{
	// TODO: Add your control notification handler code here
	if(m_bModified)
	{
		if(m_strTplFileName.IsEmpty())
		{
			if(!DoPromptSaveTpl())
				return;
		}
		CStdioFile fileOpen;
		if(fileOpen.Open(m_strTplFileName, CFile::modeCreate|CFile::modeWrite))
		{
			int nCnt = m_RplItemList.GetItemCount();
			for(int iItem = 0; iItem < nCnt; iItem ++)
			{
				fileOpen.WriteString(m_RplItemList.GetItemText(iItem, 1));
				fileOpen.WriteString(_T("\n"));
				fileOpen.WriteString(m_RplItemList.GetItemText(iItem, 2));
				fileOpen.WriteString(_T("\n"));
				fileOpen.WriteString(_T("\n"));
			}
			fileOpen.SetLength(fileOpen.Seek(0, CFile::current));
			fileOpen.Close();
		}
	}
	m_bModified = FALSE;
}
void CReplBatchDlg::OnEnChangeEditTargettext()
{
	UpdateCtrlStates();
}

LRESULT CReplBatchDlg::OnListItemChanged(WPARAM wParam, LPARAM lParam)
{
	UpdateCtrlStates();
	return 0;
}

void CReplBatchDlg::OnBnClickedChkIncchs()
{
	UpdateCtrlStates();
}

void CReplBatchDlg::OnBnClickedChkInceng()
{
	UpdateCtrlStates();
}

BOOL CReplBatchDlg::DoPromptSave()
{
	int nRet = AfxMessageBox(IDS_REPLTMPL_SAVEPROMPT, MB_YESNOCANCEL);
	switch(nRet)
	{
	case IDYES:
		OnBnClickedBtnSavetpl();
		return TRUE;
	case IDNO:
		return TRUE;
	}
	return FALSE;
}

void CReplBatchDlg::OnBnClickedBtnClosetpl()
{
	// TODO: Add your control notification handler code here
	if(m_bModified)
	{
		if(!DoPromptSave())
			return;
	}
	m_RplItemList.DeleteAllItems();
	m_strTplFileName.Empty();
	m_bModified = FALSE;
	UpdateData(FALSE);
}

int CReplBatchDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPaneDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	HWND hList = ::GetDlgItem(m_hWnd, IDC_LIST_RPLITEMS);
	if(hList != NULL)
	{
		m_RplItemList.Attach(hList);

		m_RplItemList.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES );
		m_RplItemList.SetHdrChkImage(IDB_BMP_LISTCHK, 16, 16, RGB(255, 0, 255));
		m_RplItemList.InsertColumn(0, _T(""), 0, 27);
		m_RplItemList.InsertColumn(1, _T("查找内容"), 0, 140);
		m_RplItemList.InsertColumn(2, _T("替换为"), 0, 140);
		m_RplItemList.SetColumnImage(0, 0);
		m_RplItemList.Detach();
	}

	//UpdateCtrlStates();

	return 0;
}

void CReplBatchDlg::OnBnClickedOk()
{
	if(m_bUseBatch)
	{
		int nCnt = m_RplItemList.GetItemCount();
		PREPLACE_UNIT pUnitNew = NULL;
		for(int iItem = 0; iItem < nCnt; iItem ++)
		{
			if(m_RplItemList.GetCheck(iItem))
			{
				REPLACE_UNIT newUnit;
				newUnit.strToFind = m_RplItemList.GetItemText(iItem, 1);
				newUnit.strReplTo = m_RplItemList.GetItemText(iItem, 2);
				m_mapUnits.insert(REPL_PAIR(newUnit.strToFind.GetLength(), newUnit));
			}
		}
	}
}
