// MultiReplaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "TitleFile.h"
#include "MultiReplaceDlg.h"


// CMultiReplaceDlg dialog

IMPLEMENT_DYNAMIC(CMultiReplaceDlg, CDialog)

CMultiReplaceDlg::CMultiReplaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiReplaceDlg::IDD, pParent)
	, m_strTarget(_T(""))
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

CMultiReplaceDlg::~CMultiReplaceDlg()
{
}

void CMultiReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
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

BEGIN_MESSAGE_MAP(CMultiReplaceDlg, CDialog)
	ON_BN_CLICKED(IDC_CHK_USEBATCH, &CMultiReplaceDlg::OnBnClickedChkUsebatch)
	ON_BN_CLICKED(IDC_BTN_ADDITEM, &CMultiReplaceDlg::OnBnClickedBtnAdditem)
	ON_BN_CLICKED(IDC_BTN_REMOVE, &CMultiReplaceDlg::OnBnClickedBtnRemove)
	ON_BN_CLICKED(IDC_BTN_LOADTPL, &CMultiReplaceDlg::OnBnClickedBtnLoadtpl)
	ON_BN_CLICKED(IDC_BTN_SAVETPL, &CMultiReplaceDlg::OnBnClickedBtnSavetpl)
	ON_EN_CHANGE(IDC_EDIT_TARGETTEXT, &CMultiReplaceDlg::OnEnChangeEditTargettext)
	ON_MESSAGE(MSG_LISTITEM_CHANGED, &CMultiReplaceDlg::OnListItemChanged)
	ON_BN_CLICKED(IDC_CHK_INCCHS, &CMultiReplaceDlg::OnBnClickedChkIncchs)
	ON_BN_CLICKED(IDC_CHK_INCENG, &CMultiReplaceDlg::OnBnClickedChkInceng)
	ON_BN_CLICKED(IDC_BTN_CLOSETPL, &CMultiReplaceDlg::OnBnClickedBtnClosetpl)
END_MESSAGE_MAP()

// CMultiReplaceDlg message handlers

BOOL CMultiReplaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetWindowRect(m_rcOrg);

	OnBnClickedChkUsebatch();

	m_RplItemList.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES );
	m_RplItemList.SetHdrChkImage(IDB_BMP_LISTCHK, 16, 16, RGB(255, 0, 255));
	m_RplItemList.InsertColumn(0, _T(""), 0, 27);
	m_RplItemList.InsertColumn(1, _T("查找内容"), 0, 140);
	m_RplItemList.InsertColumn(2, _T("替换为"), 0, 140);
	m_RplItemList.SetColumnImage(0, 0);

	UpdateCtrlStates();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMultiReplaceDlg::UpdateCtrlStates()
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

void CMultiReplaceDlg::OnBnClickedChkUsebatch()
{
	UpdateData();
	CRect rcWnd;
	CRect rcBatch;
	GetWindowRect(rcWnd);
	CWnd* pWndBatch = GetDlgItem(IDC_LIST_RPLITEMS);
	pWndBatch->GetWindowRect(rcBatch);
	rcWnd.bottom = rcBatch.top;
	if(m_bUseBatch)
	{
		MoveWindow(rcWnd.left, rcWnd.top, m_rcOrg.Width(), m_rcOrg.Height());
	}
	else
	{
		MoveWindow(rcWnd);
	}
	UpdateCtrlStates();
}

void CMultiReplaceDlg::OnBnClickedBtnAdditem()
{
	UpdateData();

	int nItem = m_RplItemList.InsertItem(m_RplItemList.GetItemCount(), _T(""));
	m_RplItemList.SetItemText(nItem, 1, m_strTarget);
	m_RplItemList.SetItemText(nItem, 2, m_strResult);
	m_bModified = TRUE;
	UpdateCtrlStates();
}

void CMultiReplaceDlg::OnBnClickedBtnRemove()
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

BOOL CMultiReplaceDlg::DoPromptOpenTpl()
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

BOOL CMultiReplaceDlg::ReadStrLine(PBUF_READ pReadData, CString& strLine)
{
	strLine.Empty();
	if(pReadData->dwBufOffset < pReadData->dwBufSize)
	{
		LPBYTE pData = pReadData->bufFile + pReadData->dwBufOffset;
		if(pReadData->nUnicodeState == 0)
		{
			LPSTR pAData = (LPSTR)pData;
			LPSTR pEnter = StrStrA(pAData, "\r\n");
			if(pEnter == pAData)
			{
				pReadData->dwBufOffset += 2*sizeof(CHAR);
				return TRUE;
			}
			if(pEnter == NULL)
			{
				int nWSize = MultiByteToWideChar(
					CP_ACP, 0,
					pAData, pReadData->dwBufSize - pReadData->dwBufOffset,
					NULL, 0);
				if(nWSize > 0)
				{
					MultiByteToWideChar(
						CP_ACP, 0,
						pAData, pReadData->dwBufSize - pReadData->dwBufOffset,
						strLine.GetBufferSetLength(nWSize), nWSize);
					strLine.ReleaseBuffer();
				}
				pReadData->dwBufOffset = pReadData->dwBufSize;
				return TRUE;
			}

			ULONG nLen = (ULONG)(pEnter - pAData);
			int nWSize = MultiByteToWideChar(
				CP_ACP, 0,
				pAData, nLen,
				NULL, 0);
			if(nWSize > 0)
			{
				MultiByteToWideChar(
					CP_ACP, 0,
					pAData, nLen,
					strLine.GetBufferSetLength(nWSize), nWSize);
				strLine.ReleaseBuffer();
			}
			pReadData->dwBufOffset += (nLen + 2)*sizeof(CHAR);
		}
		else
		{
			LPWSTR pWData = (LPWSTR)pData;
			LPWSTR pEnter = StrStrW(pWData, L"\r\n");
			if(pEnter == pWData)
			{
				pReadData->dwBufOffset += 2*sizeof(WCHAR);
				return TRUE;
			}
			if(pEnter == NULL)
			{
				CopyMemory(strLine.GetBufferSetLength((pReadData->dwBufSize - pReadData->dwBufOffset)/sizeof(WCHAR)),
					pData, (pReadData->dwBufSize - pReadData->dwBufOffset));
				strLine.ReleaseBuffer();
				pReadData->dwBufOffset = pReadData->dwBufSize;
				return TRUE;
			}

			ULONG nLen = (ULONG)(pEnter - pWData);
			CopyMemory(strLine.GetBufferSetLength(nLen),
				pData, nLen * sizeof(WCHAR));
			strLine.ReleaseBuffer();
			pReadData->dwBufOffset += (nLen + 2)*sizeof(WCHAR);
		}
		return TRUE;
	}
	return FALSE;
}

void CMultiReplaceDlg::OnBnClickedBtnLoadtpl()
{
	CFile fileOpen;
	if(!DoPromptOpenTpl())
		return;
	CString strLine;
	CString strFirst;
	CString strSecond;
	BUF_READ rbData;

	if(fileOpen.Open(m_strTplFileName, CFile::modeRead)&&CTitleFile::PrepareRead(&rbData, &fileOpen))
	{
		CTitleFile::FillBuffer(&rbData, &fileOpen);
		rbData.nUnicodeState = CTitleFile::TestUnicodeBuf(rbData.bufFile, rbData.dwBufSize);
		CTitleFile::StepSignature(&rbData);
		CTitleFile::ProcessReverse(&rbData);
		while(ReadStrLine(&rbData, strLine))
		{
			if(strLine.IsEmpty())
			{
				strFirst.Empty();
				strSecond.Empty();
				continue;
			}

			if(strFirst.IsEmpty())
			{
				strFirst = strLine;
				continue;
			}
			if(strSecond.IsEmpty())
				strSecond = strLine;

			int nItem = m_RplItemList.InsertItem(m_RplItemList.GetItemCount(), _T(""));
			m_RplItemList.SetItemText(nItem, 1, strFirst);
			m_RplItemList.SetItemText(nItem, 2, strSecond);
			strFirst.Empty();
			strSecond.Empty();
		}

		m_bModified = FALSE;
		fileOpen.Close();
		UpdateData(FALSE);
	}
}

BOOL CMultiReplaceDlg::DoPromptSaveTpl()
{
	TCHAR szFilter[] = _T("批量替换模板文件(*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlgSave(FALSE, _T("txt"), 0, OFN_OVERWRITEPROMPT, szFilter);
	if(IDOK == dlgSave.DoModal())
	{
		m_strTplFileName = dlgSave.GetPathName();
		m_bModified = TRUE;
		UpdateData(FALSE);
		return TRUE;
	}
	return FALSE;
}

void CMultiReplaceDlg::OnBnClickedBtnSavetpl()
{
	// TODO: Add your control notification handler code here
	if(m_bModified)
	{
		if(m_strTplFileName.IsEmpty())
		{
			if(!DoPromptSaveTpl())
				return;
		}
		CFile fileSave;
		if(fileSave.Open(m_strTplFileName, CFile::modeCreate|CFile::modeWrite))
		{
			WORD wUFlag = 0xFEFF;
			fileSave.Write(&wUFlag, 2);

			int nCnt = m_RplItemList.GetItemCount();
			for(int iItem = 0; iItem < nCnt; iItem ++)
			{
				CString strLine = m_RplItemList.GetItemText(iItem, 1);
				fileSave.Write(strLine.GetBuffer(), strLine.GetLength() * sizeof(TCHAR));
				fileSave.Write(_T("\r\n"), 2*sizeof(TCHAR));
				strLine = m_RplItemList.GetItemText(iItem, 2);
				fileSave.Write(strLine.GetBuffer(), strLine.GetLength() * sizeof(TCHAR));
				fileSave.Write(_T("\r\n"), 2*sizeof(TCHAR));
				fileSave.Write(_T("\r\n"), 2*sizeof(TCHAR));
			}
			fileSave.SetLength(fileSave.Seek(0, CFile::current));
			fileSave.Close();
		}
	}
	m_bModified = FALSE;
}

void CMultiReplaceDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_bUseBatch)
	{
		m_mapUnits.clear();
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
	AfxGetMainWnd()->SendMessage(MSG_TM_DOREPLACE, 0, 0);
}

void CMultiReplaceDlg::OnEnChangeEditTargettext()
{
	UpdateCtrlStates();
}

LRESULT CMultiReplaceDlg::OnListItemChanged(WPARAM wParam, LPARAM lParam)
{
	UpdateCtrlStates();
	return 0;
}

void CMultiReplaceDlg::OnBnClickedChkIncchs()
{
	UpdateCtrlStates();
}

void CMultiReplaceDlg::OnBnClickedChkInceng()
{
	UpdateCtrlStates();
}

BOOL CMultiReplaceDlg::DoPromptSave()
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

void CMultiReplaceDlg::OnBnClickedBtnClosetpl()
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
