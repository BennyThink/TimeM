#pragma once


// CCheckHdrCtrl

#define	MSG_CHKHDR_CHECK	(WM_USER + 50)

#define	HDR_CHKSTATE_NON	0
#define	HDR_CHKSTATE_FALSE	1
#define	HDR_CHKSTATE_PART	2
#define HDR_CHKSTATE_TRUE	3

class CCheckHdrCtrl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CCheckHdrCtrl)

public:
	CCheckHdrCtrl();
	virtual ~CCheckHdrCtrl();

	CImageList* GetImageList() const;
	CImageList* SetImageList(CImageList* pImageList);
	void		SetChkImage(CImageList* pImageList);
	int			GetItemImage(int nItem);
	void		SetItemImage(int nItem, int nImage);

	// nState = 0, NOCHECK;
	//			1, FALSE;
	//			2, Middle State;
	//			3, Full Check;
	void		CheckCol(int nItem, int nState);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	CImageList* m_pImageList;
	CMap<int, int, int, int> m_mapImageIndex;

	CImageList* m_pChkImageList;
	CMap<int, int, int, int> m_mapChkStateIndex;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHdnItemclick(NMHDR *pNMHDR, LRESULT *pResult);
};

