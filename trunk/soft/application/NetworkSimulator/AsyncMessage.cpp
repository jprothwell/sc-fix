// AsyncMessage.cpp: implementation of the CAsyncMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoolTerminalDll.h"
#include "AsyncMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAsyncMessage::CAsyncMessage()
{

}

CAsyncMessage::~CAsyncMessage()
{

}

CAsyncMessage::CAsyncMessage(const CAsyncMessage& AsyncMessage)
{
  this->RemoveAll();
  this->Append(AsyncMessage);
  this->m_strAsyncName = AsyncMessage.m_strAsyncName;
}

CAsyncMessage& CAsyncMessage::operator=(const CAsyncMessage& AsyncMessage)
{
  if(this == &AsyncMessage)
  {
    return *this;
  }

  this->RemoveAll();
  this->Append(AsyncMessage);
  this->m_strAsyncName = AsyncMessage.m_strAsyncName;

  return *this;
}

CAsyncMessageArray& CAsyncMessageArray::operator=(CAsyncMessageArray& AsyncMessageArray)
{
  if(this == &AsyncMessageArray)
  {
    return *this;
  }

  this->RemoveAll();
  this->Append(AsyncMessageArray);

  return *this;
}

int CAsyncMessageArray::Find(CString strName)
{
  int nIndex = -1;
  for (int i=0;i<this->GetSize();i++)
  {
    this->GetAt(i).m_strAsyncName.MakeUpper();
    strName.MakeUpper();
    if (this->GetAt(i).m_strAsyncName == strName)
    {
      nIndex = i;
    }
  }

  return nIndex;
}
