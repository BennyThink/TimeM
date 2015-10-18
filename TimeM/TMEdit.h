#pragma once

// CTMEdit

class CTMEdit : public CEdit
{
	DECLARE_DYNAMIC(CTMEdit)

public:
	CTMEdit();
	virtual ~CTMEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
