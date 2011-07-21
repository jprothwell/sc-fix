//////////////////////////////////////////////////////////////////////////
// Author:  zhuoxz
// Create:  2008-07-31
// Modified:
// Description:
//  同步消息与同步消息数组,只包含消息code,在使用时需找到CMessageItem或CMessageArray
//////////////////////////////////////////////////////////////////////////

// AsyncMessage.h: interface for the CAsyncMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASYNCMESSAGE_H__38DDC268_22F3_4E76_8507_C51CB00CF0B6__INCLUDED_)
#define AFX_ASYNCMESSAGE_H__38DDC268_22F3_4E76_8507_C51CB00CF0B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxtempl.h"

class CAsyncMessage : public CDWordArray
{
public:
	CAsyncMessage();
	virtual ~CAsyncMessage();
  CAsyncMessage(const CAsyncMessage& AsyncMessage);
public:
  CAsyncMessage& operator=(const CAsyncMessage& AsyncMessage);
public:
  CString m_strAsyncName;
};

class CAsyncMessageArray : public CArray<CAsyncMessage,CAsyncMessage&>
{
public:
  CAsyncMessageArray& operator=(CAsyncMessageArray& AsyncMessageArray);
  int Find(CString strName);
};

#endif // !defined(AFX_ASYNCMESSAGE_H__38DDC268_22F3_4E76_8507_C51CB00CF0B6__INCLUDED_)
