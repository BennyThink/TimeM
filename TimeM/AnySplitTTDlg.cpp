// AnySplitTTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimeM.h"
#include "AnySplitTTDlg.h"
#include "TitleHelper.h"


// CAnySplitTTDlg dialog

IMPLEMENT_DYNAMIC(CAnySplitTTDlg, CDialog)

CAnySplitTTDlg::CAnySplitTTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnySplitTTDlg::IDD, pParent)
	, m_strTTLen(_T("00:00:00,000"))
{
	m_pUnit = NULL;
	m_bSingleLang = FALSE;
}

CAnySplitTTDlg::~CAnySplitTTDlg()
{
}

void CAnySplitTTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TTLEN, m_strTTLen);
}


BEGIN_MESSAGE_MAP(CAnySplitTTDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_REMOVESPLITOR, &CAnySplitTTDlg::OnBnClickedBtnRemovesplitor)
	ON_BN_CLICKED(IDC_CHK_NEW, &CAnySplitTTDlg::OnBnClickedChkNew)
	ON_MESSAGE(MSG_REMOVESPLIT, &CAnySplitTTDlg::OnSplitRemove)
	ON_MESSAGE(MSG_NEWSPLIT, &CAnySplitTTDlg::OnSplitNew)
	ON_BN_CLICKED(IDC_RBTN_SINGLANG, &CAnySplitTTDlg::OnBnClickedRbtnSinglang)
	ON_BN_CLICKED(IDC_RBTN_DUALLANG, &CAnySplitTTDlg::OnBnClickedRbtnDuallang)
END_MESSAGE_MAP()


// CAnySplitTTDlg message handlers

BOOL CAnySplitTTDlg::OnInitDialog()
{
	if(NULL == m_pUnit)
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_strTTLen = CTitleHelper::FormatTime(m_pUnit->nEnd - m_pUnit->nStart);

	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CWnd* pFrame = GetDlgItem(IDC_STATIC_FRM);
	if(NULL != pFrame)
	{
		CRect rcWnd;
		pFrame->GetWindowRect(rcWnd);
		ScreenToClient(rcWnd);

		CStringArray astr;
		CTitleHelper::BreakStringEachLine(m_pUnit->content, astr);
		VT_COLORS vtClrs;
		int nStrCnt = astr.GetCount();
		int nState = 0;
		for(int iStr = 0; iStr < nStrCnt; iStr ++)
		{
			if(m_strAssCode.IsEmpty())
				m_strAssCode = GetAssCode(astr[iStr]);
			CTitleHelper::RemoveAssCode(astr[iStr]);
			astr[iStr].Trim(_T("* "));
			if(astr[iStr].IsEmpty())
				continue;
			if(CTitleHelper::IsDBCSString(astr[iStr]))
			{
				nState |= 0x01;
				vtClrs.push_back(RGB(204, 255, 204));
			}
			else
			{
				nState |= 0x02;
				vtClrs.push_back(RGB(128, 204, 240));
			}
		}
		if(3 == nState)
			CheckRadioButton(IDC_RBTN_SINGLANG, IDC_RBTN_DUALLANG, IDC_RBTN_DUALLANG);
		else
			CheckRadioButton(IDC_RBTN_SINGLANG, IDC_RBTN_DUALLANG, IDC_RBTN_SINGLANG);

		LOGFONT lf;
		m_wndSplitor.GetFontLog(&lf);
		lf.lfHeight -= 2;
		m_wndSplitor.SetFontLog(&lf);
		m_wndSplitor.CreateSplitWnd(astr, rcWnd, this);
		m_wndSplitor.SetColors(vtClrs);
	}
	CheckDlgButton(IDC_CHK_NEW, TRUE);
	m_wndSplitor.SetClickNew(IsDlgButtonChecked(IDC_CHK_NEW));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAnySplitTTDlg::OnBnClickedBtnRemovesplitor()
{
	// TODO: Add your control notification handler code here
	m_wndSplitor.RemoveCurrSplitor();
}

void CAnySplitTTDlg::OnBnClickedChkNew()
{
	// TODO: Add your control notification handler code here
	m_wndSplitor.SetClickNew(IsDlgButtonChecked(IDC_CHK_NEW));
}

LRESULT CAnySplitTTDlg::OnSplitRemove(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CAnySplitTTDlg::OnSplitNew(WPARAM wParam, LPARAM lParam)
{
	CheckDlgButton(IDC_CHK_NEW, m_wndSplitor.IsClickNew());
	return 0;
}

void CAnySplitTTDlg::BreakTitleUnitSingleLang()
{
	int nTLen = m_pUnit->nEnd - m_pUnit->nStart;
	int nStrLen = m_pUnit->content.GetLength();

	const VT_SPLINES& splines = m_wndSplitor.GetSplitLines();
	PTITLE_UNIT pUnitNew = NULL;
	int nLineCnt = splines.size();
	CString strLineTail;
	int nStartSave = m_pUnit->nStart;
	int nEndSave = m_pUnit->nStart;
	int nOffsetCur = 0;
	int nOffsetLineStart = 0;

	for(int iLine = 0; iLine < nLineCnt; iLine ++)
	{
		CString strLine = splines[iLine]->strLine;
		int nSpCnt = splines[iLine]->vtSplitors.size();
		int nLinePrevPos = 0;
		for(int iSplit = 0; iSplit < nSpCnt; iSplit ++)
		{
			int nPos = splines[iLine]->vtSplitors[iSplit]->GetCharNum();
			nOffsetCur = nOffsetLineStart + nPos;
			nEndSave = m_pUnit->nStart + (int)((__int64)((__int64)nOffsetCur * (__int64)nTLen) / nStrLen);

			pUnitNew = new TITLE_UNIT;
			InitializeUnit(pUnitNew);
			pUnitNew->nStart = nStartSave;
			pUnitNew->nEnd = nEndSave;
			pUnitNew->dwFlags = 0;
			if(strLineTail.IsEmpty())
			{
				pUnitNew->content = strLine.Left(nPos - nLinePrevPos);
			}
			else
			{
				pUnitNew->content = strLineTail + _T("\r\n");
				pUnitNew->content += strLine.Left(nPos - nLinePrevPos);
				strLineTail.Empty();
			}
			m_vtBreak.push_back(pUnitNew);

			strLine = strLine.Right(strLine.GetLength() - nPos + nLinePrevPos);
			nStartSave = nEndSave + 1;
			nLinePrevPos = nPos;
		}
		nOffsetLineStart = nOffsetCur;
		if(strLineTail.IsEmpty())
			strLineTail = strLine;
		else
		{
			strLineTail += _T("\r\n");
			strLineTail += strLine;
		}
	}
	pUnitNew = new TITLE_UNIT;
	InitializeUnit(pUnitNew);
	pUnitNew->nStart = nStartSave;
	pUnitNew->nEnd = m_pUnit->nEnd;
	pUnitNew->dwFlags = 0;
	pUnitNew->content = strLineTail;
	m_vtBreak.push_back(pUnitNew);
}

void CAnySplitTTDlg::BreakTitleUnitDualLang()
{
	const VT_SPLINES& splines = m_wndSplitor.GetSplitLines();
	PTITLE_UNIT pUnitNew = NULL;
	int nLineCnt = splines.size();

	int nStrLen = 0;
	for(int iLine = 0; iLine < nLineCnt; iLine += 2)
	{
		nStrLen += splines[iLine]->strLine.GetLength();
	}
	int nTLen = m_pUnit->nEnd - m_pUnit->nStart;

	CString strLineTail1;
	CString strLineTail2;

	int nStartSave = m_pUnit->nStart;
	int nEndSave = m_pUnit->nStart;
	int nOffsetCur = 0;
	int nOffsetLineStart = 0;

	for(int iLine = 0; iLine < nLineCnt; iLine += 2)
	{
		CString strLine1 = splines[iLine]->strLine;
		CString strLine2 = splines[iLine + 1]->strLine;
		VT_SPLITOR& vtSplitors1 = splines[iLine]->vtSplitors;
		VT_SPLITOR& vtSplitors2 = splines[iLine + 1]->vtSplitors;
		int nSpCnt = vtSplitors1.size();
		int nLinePrevPos1 = 0;
		int nLinePrevPos2 = 0;
		for(int iSplit = 0; iSplit < nSpCnt; iSplit ++)
		{
			int nPos1 = vtSplitors1[iSplit]->GetCharNum();
			int nPos2 = vtSplitors2[iSplit]->GetCharNum();
			nOffsetCur = nOffsetLineStart + nPos1;
			nEndSave = m_pUnit->nStart + (int)((__int64)((__int64)nOffsetCur * (__int64)nTLen) / nStrLen);

			pUnitNew = new TITLE_UNIT;
			InitializeUnit(pUnitNew);
			pUnitNew->nStart = nStartSave;
			pUnitNew->nEnd = nEndSave;
			pUnitNew->dwFlags = 0;
			if(strLineTail1.IsEmpty()&&strLineTail2.IsEmpty())
			{
				pUnitNew->content = strLine1.Left(nPos1 - nLinePrevPos1);
				pUnitNew->content += _T("\r\n");
				pUnitNew->content += m_strAssCode;
				pUnitNew->content += strLine2.Left(nPos2 - nLinePrevPos2);
			}
			else
			{
				pUnitNew->content = strLineTail1 + _T("\r\n");
				pUnitNew->content += strLineTail2 + _T("\r\n");
				pUnitNew->content += strLine1.Left(nPos1 - nLinePrevPos1);
				pUnitNew->content += _T("\r\n");
				pUnitNew->content += m_strAssCode;
				pUnitNew->content += strLine2.Left(nPos2 - nLinePrevPos2);
			}
			m_vtBreak.push_back(pUnitNew);

			strLine1 = strLine1.Right(strLine1.GetLength() - nPos1 + nLinePrevPos1);
			strLine2 = strLine2.Right(strLine2.GetLength() - nPos2 + nLinePrevPos2);
			nStartSave = nEndSave + 1;
			nLinePrevPos1 = nPos1;
			nLinePrevPos2 = nPos2;
		}
		nOffsetLineStart = nOffsetCur;

		if(strLineTail1.IsEmpty())
			strLineTail1 = strLine1;
		else
		{
			strLineTail1 += _T("\r\n");
			strLineTail1 += strLine1;
		}

		if(strLineTail2.IsEmpty())
			strLineTail2 = strLine2;
		else
		{
			strLineTail2 += _T("\r\n");
			strLineTail2 += strLine2;
		}
	}
	pUnitNew = new TITLE_UNIT;
	InitializeUnit(pUnitNew);
	pUnitNew->nStart = nStartSave;
	pUnitNew->nEnd = m_pUnit->nEnd;
	pUnitNew->dwFlags = 0;
	pUnitNew->content = strLineTail1;
	pUnitNew->content += _T("\r\n");
	pUnitNew->content += m_strAssCode;
	pUnitNew->content += strLineTail2;
	m_vtBreak.push_back(pUnitNew);
}

void CAnySplitTTDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_bSingleLang = IsDlgButtonChecked(IDC_RBTN_SINGLANG);
	if(!CheckSplitPartner())
	{
		AfxMessageBox(IDS_NOTMATCHSP);
		return;
	}

	if(!CheckSplitHdrTail())
	{
		AfxMessageBox(IDS_WITHVALIDSPLIT);
		return;
	}

	if(m_bSingleLang)
	{
		BreakTitleUnitSingleLang();
	}
	else
	{
		BreakTitleUnitDualLang();
	}

	CDialog::OnOK();
}

BOOL CAnySplitTTDlg::CheckSplitPartner()
{
	if(IsDlgButtonChecked(IDC_RBTN_SINGLANG))
		return TRUE;
	const VT_SPLINES& splines = m_wndSplitor.GetSplitLines();
	int nLineCnt = (int)splines.size();
	int nSpCntOdd = 0;
	int nSpCntEven = 0;
	for(int iLine = 0; iLine < nLineCnt; iLine ++)
	{
		if(0 == (iLine%2))
			nSpCntEven = (int)splines[iLine]->vtSplitors.size();
		else
		{
			nSpCntOdd = (int)splines[iLine]->vtSplitors.size();
			if(nSpCntEven != nSpCntOdd)
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CAnySplitTTDlg::CheckSplitHdrTail()
{
	const VT_SPLINES& splines = m_wndSplitor.GetSplitLines();
	int nLineCnt = (int)splines.size();
	for(int iLine = 0; iLine < nLineCnt; iLine ++)
	{
		int nSpCnt = (int)splines[iLine]->vtSplitors.size();
		for(int iSplit = 0; iSplit < nSpCnt; iSplit ++)
		{
			int nChar = splines[iLine]->vtSplitors[iSplit]->GetCharNum();
			int nCharCnt = splines[iLine]->strLine.GetLength();
			if(IsDlgButtonChecked(IDC_RBTN_SINGLANG))
			{
				if(((iSplit == 0)&&(nChar == 0))
					||((iSplit == (nLineCnt - 1))&&(nCharCnt == nChar)))
				{
					return FALSE;
				}
			}
			else
			{
				if((nChar == 0)||(nCharCnt == nChar))
					return FALSE;
			}
		}
	}
	return TRUE;
}

void CAnySplitTTDlg::OnBnClickedRbtnSinglang()
{
	// TODO: Add your control notification handler code here
	VT_COLORS vtClrs;
	const VT_SPLINES& splines = m_wndSplitor.GetSplitLines();
	int nStrCnt = (int)splines.size();
	for(int iStr = 0; iStr < nStrCnt; iStr ++)
	{
		vtClrs.push_back(RGB(204, 255, 204));
	}
	m_wndSplitor.SetColors(vtClrs);
	m_wndSplitor.Invalidate();
}

void CAnySplitTTDlg::OnBnClickedRbtnDuallang()
{
	// TODO: Add your control notification handler code here
	VT_COLORS vtClrs;
	const VT_SPLINES& splines = m_wndSplitor.GetSplitLines();
	int nStrCnt = (int)splines.size();
	for(int iStr = 0; iStr < nStrCnt; iStr ++)
	{
		if(iStr%2 == 0)
			vtClrs.push_back(RGB(204, 255, 204));
		else
			vtClrs.push_back(RGB(128, 204, 240));
	}
	m_wndSplitor.SetColors(vtClrs);
	m_wndSplitor.Invalidate();
}

CString CAnySplitTTDlg::GetAssCode(const CString& strLine)
{
	CString strAssCode;
	LPCTSTR pData = (LPCTSTR)strLine;
	int nLen = strLine.GetLength();
	DWORD dwState = 0;
	int nPos = -1;
	for(int iChar = 0; iChar < nLen; iChar ++)
	{
		switch(pData[iChar])
		{
		case _T('{'):
			dwState ++;
			break;
		case _T('}'):
			dwState --;
			break;
		default:
			if(dwState > 0)
			{
				continue;
			}
			else if(dwState < 0)
			{
				strAssCode.Empty();
				return strAssCode;
			}
			nPos = iChar;
			break;
		}
		if(nPos > -1)
		{
			strAssCode = strLine.Left(nPos);
			break;
		}
	}
	return strAssCode;
}
