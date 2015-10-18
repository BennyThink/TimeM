// .\WmpImport\CWMPMedia3.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CWMPMedia3

class CWMPMedia3 : public COleDispatchDriver
{
public:
	CWMPMedia3() {}		// Calls COleDispatchDriver default constructor
	CWMPMedia3(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWMPMedia3(const CWMPMedia3& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:

	BOOL get_isIdentical(LPDISPATCH pIWMPMedia)
	{
		BOOL result;
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x2fb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, pIWMPMedia);
		return result;
	}
	CString get_sourceURL()
	{
		CString result;
		InvokeHelper(0x2ef, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_name()
	{
		CString result;
		InvokeHelper(0x2fc, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_name(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2fc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_imageSourceWidth()
	{
		long result;
		InvokeHelper(0x2f0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_imageSourceHeight()
	{
		long result;
		InvokeHelper(0x2f1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_markerCount()
	{
		long result;
		InvokeHelper(0x2f2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	double getMarkerTime(long MarkerNum)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2f3, DISPATCH_METHOD, VT_R8, (void*)&result, parms, MarkerNum);
		return result;
	}
	CString getMarkerName(long MarkerNum)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2f4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, MarkerNum);
		return result;
	}
	double get_duration()
	{
		double result;
		InvokeHelper(0x2f5, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	CString get_durationString()
	{
		CString result;
		InvokeHelper(0x2f6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_attributeCount()
	{
		long result;
		InvokeHelper(0x2f7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString getAttributeName(long lIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2f8, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, lIndex);
		return result;
	}
	CString getItemInfo(LPCTSTR bstrItemName)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2f9, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, bstrItemName);
		return result;
	}
	void setItemInfo(LPCTSTR bstrItemName, LPCTSTR bstrVal)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x2fa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bstrItemName, bstrVal);
	}
	CString getItemInfoByAtom(long lAtom)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2fd, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, lAtom);
		return result;
	}
	BOOL isMemberOf(LPDISPATCH pPlaylist)
	{
		BOOL result;
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x2fe, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, pPlaylist);
		return result;
	}
	BOOL isReadOnlyItem(LPCTSTR bstrItemName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2ff, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bstrItemName);
		return result;
	}
	LPDISPATCH get_Error()
	{
		LPDISPATCH result;
		InvokeHelper(0x300, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long getAttributeCountByType(LPCTSTR bstrType, LPCTSTR bstrLanguage)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x301, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bstrType, bstrLanguage);
		return result;
	}
	VARIANT getItemInfoByType(LPCTSTR bstrType, LPCTSTR bstrLanguage, long lIndex)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 ;
		InvokeHelper(0x302, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, bstrType, bstrLanguage, lIndex);
		return result;
	}


};
