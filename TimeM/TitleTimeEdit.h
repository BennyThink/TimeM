#pragma once


#define VK_0		0x30
#define VK_1		0x31
#define VK_2		0x32
#define VK_3		0x33
#define VK_4		0x34
#define VK_5		0x35
#define VK_6		0x36
#define VK_7		0x37
#define VK_8		0x38
#define VK_9		0x39

// CTitleTimeEdit

class CTitleTimeEdit : public CEdit
{
	DECLARE_DYNAMIC(CTitleTimeEdit)

public:
	CTitleTimeEdit();
	virtual ~CTitleTimeEdit();
protected:
	TCHAR	m_szTimeChar[16];	// 00:00:00,000

protected:
	DECLARE_MESSAGE_MAP()

	void Update();

	void DeleteRange(int nStart, int nEnd);

	void KeyInBack();
	void KeyInNumeric(UINT nChar);
	void KeyInUp();
	void KeyInDown();
	void KeyInDel();

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnSetfocus();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);

	afx_msg LONG OnCut(UINT wParam, LONG lParam);
	afx_msg LONG OnCopy(UINT wParam, LONG lParam);
	afx_msg LONG OnPaste(UINT wParam, LONG lParam);
	afx_msg LONG OnClear(UINT wParam, LONG lParam);
	afx_msg LONG OnSetText(UINT wParam, LONG lParam);
	afx_msg LONG OnGetText(UINT wParam, LONG lParam);
	afx_msg LONG OnGetTextLength(UINT wParam, LONG lParam);
	afx_msg LONG OnSetSel(UINT wParam, LONG lParam);
protected:
	virtual void PreSubclassWindow();
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
};


