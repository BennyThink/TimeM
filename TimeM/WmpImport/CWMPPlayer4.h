// .\WmpImport\CWMPPlayer4.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

#include "CWMPControls3.h"
#include "CWMPMedia3.h"
#include "CWMPSettings2.h"

/////////////////////////////////////////////////////////////////////////////
// CWMPPlayer4

class CWMPPlayer4 : public COleDispatchDriver
{
public:
	CWMPPlayer4() {}		// Calls COleDispatchDriver default constructor
	CWMPPlayer4(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWMPPlayer4(const CWMPPlayer4& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}
/*
protected:
	DECLARE_DYNCREATE(CWMPPlayer4)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x6BF52A52, 0x394A, 0x11D3, { 0xB1, 0x53, 0x0, 0xC0, 0x4F, 0x79, 0xFA, 0xA6 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}
*/

// Attributes
public:
enum
{
    wmplcUnknown = 0,
    wmplcClear = 1,
    wmplcInfoChange = 2,
    wmplcMove = 3,
    wmplcDelete = 4,
    wmplcInsert = 5,
    wmplcAppend = 6,
    wmplcPrivate = 7,
    wmplcNameChange = 8,
    wmplcMorph = 9,
    wmplcSort = 10,
    wmplcLast = 11
}WMPPlaylistChangeEventType;
enum
{
    wmposUndefined = 0,
    wmposPlaylistChanging = 1,
    wmposPlaylistLocating = 2,
    wmposPlaylistConnecting = 3,
    wmposPlaylistLoading = 4,
    wmposPlaylistOpening = 5,
    wmposPlaylistOpenNoMedia = 6,
    wmposPlaylistChanged = 7,
    wmposMediaChanging = 8,
    wmposMediaLocating = 9,
    wmposMediaConnecting = 10,
    wmposMediaLoading = 11,
    wmposMediaOpening = 12,
    wmposMediaOpen = 13,
    wmposBeginCodecAcquisition = 14,
    wmposEndCodecAcquisition = 15,
    wmposBeginLicenseAcquisition = 16,
    wmposEndLicenseAcquisition = 17,
    wmposBeginIndividualization = 18,
    wmposEndIndividualization = 19,
    wmposMediaWaiting = 20,
    wmposOpeningUnknownURL = 21
}WMPOpenState;
enum
{
    wmppsUndefined = 0,
    wmppsStopped = 1,
    wmppsPaused = 2,
    wmppsPlaying = 3,
    wmppsScanForward = 4,
    wmppsScanReverse = 5,
    wmppsBuffering = 6,
    wmppsWaiting = 7,
    wmppsMediaEnded = 8,
    wmppsTransitioning = 9,
    wmppsReady = 10,
    wmppsReconnecting = 11,
    wmppsLast = 12
}WMPPlayState;
enum
{
    wmpsdlsDownloading = 0,
    wmpsdlsPaused = 1,
    wmpsdlsProcessing = 2,
    wmpsdlsCompleted = 3,
    wmpsdlsCancelled = 4
}WMPSubscriptionDownloadState;
enum
{
    WMP_WRITENAMES_TYPE_CD_BY_TOC = 0,
    WMP_WRITENAMES_TYPE_CD_BY_CONTENT_ID = 1,
    WMP_WRITENAMES_TYPE_CD_BY_MDQCD = 2,
    WMP_WRITENAMES_TYPE_DVD_BY_DVDID = 3
}WMP_WRITENAMESEX_TYPE;

// Operations
public:

	void close()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString get_URL()
	{
		CString result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_URL(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_openState()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_playState()
	{
		long result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CWMPControls3 get_controls()
	{
		LPDISPATCH result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return CWMPControls3(result);
	}
	CWMPSettings2 get_settings()
	{
		LPDISPATCH result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return CWMPSettings2(result);
	}
	CWMPMedia3 get_currentMedia()
	{
		LPDISPATCH result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return CWMPMedia3(result);
	}
	void put_currentMedia(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_mediaCollection()
	{
		LPDISPATCH result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_playlistCollection()
	{
		LPDISPATCH result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_versionInfo()
	{
		CString result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void launchURL(LPCTSTR bstrURL)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bstrURL);
	}
	LPDISPATCH get_network()
	{
		LPDISPATCH result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_currentPlaylist()
	{
		LPDISPATCH result;
		InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_currentPlaylist(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_cdromCollection()
	{
		LPDISPATCH result;
		InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_closedCaption()
	{
		LPDISPATCH result;
		InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_isOnline()
	{
		BOOL result;
		InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Error()
	{
		LPDISPATCH result;
		InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_status()
	{
		CString result;
		InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_dvd()
	{
		LPDISPATCH result;
		InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH newPlaylist(LPCTSTR bstrName, LPCTSTR bstrURL)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, bstrName, bstrURL);
		return result;
	}
	LPDISPATCH newMedia(LPCTSTR bstrURL)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, bstrURL);
		return result;
	}
	BOOL get_enabled()
	{
		BOOL result;
		InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_enabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_fullScreen()
	{
		BOOL result;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_fullScreen(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_enableContextMenu()
	{
		BOOL result;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_enableContextMenu(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void put_uiMode(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_uiMode()
	{
		CString result;
		InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_stretchToFit()
	{
		BOOL result;
		InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_stretchToFit(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_windowlessVideo()
	{
		BOOL result;
		InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_windowlessVideo(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_isRemote()
	{
		BOOL result;
		InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_playerApplication()
	{
		LPDISPATCH result;
		InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void openPlayer(LPCTSTR bstrURL)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bstrURL);
	}


};
