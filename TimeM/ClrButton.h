#pragma once


// CClrButton

class CClrButton : public CButton
{
	DECLARE_DYNAMIC(CClrButton)

	COLORREF m_clrBtn;
public:
	CClrButton();
	virtual ~CClrButton();

	COLORREF GetColor(){return m_clrBtn;};
	void SetColor(COLORREF clrBtn){m_clrBtn = clrBtn;};

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
};


