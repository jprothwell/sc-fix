/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
 /**************************************************************

	FILENAME	: GCMLGenerator.cpp

  	PURPOSE		: This is the implementation file for GCML generator. This class is used 
					to generate GCML whenever required.

	REMARKS		: nil

	AUTHOR		: Kuldeep

	DATE		: Nov 10,04

**************************************************************/

#include "stdafx.h"
#include "GCMLDefines.h"
#include "GCMLGenerator.h"
#include "Data.h"
#include "Value.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGCMLGenerator::CGCMLGenerator()
{
}

CGCMLGenerator::~CGCMLGenerator()
{

}

/***************************************************************/
/**************************************************************

	FUNCTION NAME		: MakeGCML	

  	PURPOSE				: Function dumps a DOM into a file.

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: nil

	RETURNS				: ERR

	REMARKS				: Create a file and dumps the DOM into that file.
**************************************************************/

ERR CGCMLGenerator::MakeGCML(LPTSTR pFileName)
{
  ASSERT(pFileName);
  CDCMLHandler	dcmlHandlerObj(pFileName);
  //CFormatConverter convObj;
  BYTE*  pMultiByteData = NULL;
  ULONG		iBytesWritten	=	0;
  UINT32		nBytesReceived;
  ERR		err		=	FAILURE;
  TCHAR* pBuff = dcmlHandlerObj.GetXML();
  CString cs;
  HANDLE hDir;
  hDir = CreateFile(pFileName, GENERIC_READ | GENERIC_WRITE,
    FILE_SHARE_READ, NULL, OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);
  
  if (hDir != INVALID_HANDLE_VALUE) 
  {
    CloseHandle(hDir);
  }
  
  HANDLE handle = CreateFile(
    pFileName,
    GENERIC_READ|GENERIC_WRITE,
    CREATE_ALWAYS,
    NULL,
    CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    0);
  if(handle )
  {
#ifndef __UCS2_ENCODING
    GetMultiBytechar(pBuff,pMultiByteData,&nBytesReceived);
    nBytesReceived = strlen((const char*)pMultiByteData);
#else
    pMultiByteData = (BYTE*)pBuff;
    nBytesReceived = (wcslen(pBuff)+1)*2;
#endif
    WriteFile(handle,pMultiByteData,nBytesReceived,&iBytesWritten,NULL);
    //m_pCallBackHandler(DCML_DISPLAY_MESSAGE,IDS_STRING120);
#ifndef __UCS2_ENCODING
    
    DELETE_MEM_ARR(pMultiByteData);
    
#endif
    
    CloseHandle(handle);
    err		=	SUCCESS;
  }
  else
    err = CD_NOT_OPENFILE;
  
  return err;
	
}

ERR CGCMLGenerator::GetMultiBytechar(WCHAR* pString,BYTE*& ptcMultiByte,UINT32* pBytes)
{
	ERR		err		=	SUCCESS;
	int iLen = wcslen(pString);
	*pBytes = ++iLen+50;
	ptcMultiByte = new BYTE[*pBytes];

	ASSERT(ptcMultiByte);
	if(!(WideCharToMultiByte(
				CP_ACP, 0,pString,iLen,(LPSTR)ptcMultiByte,*pBytes,NULL,NULL)))	
	{
		DWORD dw = GetLastError();
		DELETE_MEM_ARR(ptcMultiByte)
		err = FAILURE;
	}

	return err;
}

/**************************************************************

	FUNCTION NAME		: Free	

	PURPOSE				: Frees the allocated memory inside the DCML generator.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: Nil
**************************************************************/


void CGCMLGenerator::Free()
{
}


/**************************************************************************************

	FUNCTION NAME		: GetNewMsgIdNode

	PURPOSE				: Get a node for message id

	INPUT PARAMETERS	: UINT32, MSXML::IXMLDOMNode*&

	OUTPUT PARAMETERS	: ERR

	RETURNS				: ERR

	REMARKS				: Nil

****************************************************************************************/

ERR CGCMLGenerator::GetNewMsgIdNode(UINT32 MsgId,MSXML::IXMLDOMNode*& pNode )
{
	return 	CreateMessageIdNode(MsgId,(MSXML::IXMLDOMElement*&)pNode);
}


/****************************************************************************************

	FUNCTION NAME		: CreateMessageIdNode

	PURPOSE				: Creates a node for message id, store the reference in pNewNode.

	INPUT PARAMETERS	: UINT32, IXMLDOMElement*& 

	OUTPUT PARAMETERS	: ERR

	RETURNS				: ERR

	REMARKS				: Nil
******************************************************************************************/

ERR CGCMLGenerator::CreateMessageIdNode(UINT32 MsgId, MSXML::IXMLDOMElement*& pNewNode)
{
	ERR			err			=	SUCCESS;
	VARIANT vtValue;
	_bstr_t	bstr;
	CDCMLHandler	dcmlHandlerObj;
	TCHAR	tcID[10];
	CValue val;
	//create the node for message id and add the attributes into it
	
	if((err = dcmlHandlerObj.CreateElement( TAG_BOOTMESG,pNewNode))== SUCCESS)
	{
		VariantInit(&vtValue);
		vtValue.vt = VT_BSTR;
		vtValue.bstrVal = bstr = _itow(MsgId,tcID,10);
		if(dcmlHandlerObj.AddAttribute(pNewNode,TAG_ID,vtValue) != SUCCESS)
		{
			err = FAILURE;
		}
		else
		{
			err = SUCCESS;
		}
		
	}
	else 
		err = FAILURE;


	return err;
}



/**************************************************************

	FUNCTION NAME		: GetDataNode	

  	PURPOSE				: Function generates A Value node from the oject passed

	INPUT PARAMETERS	: CData& data,MSXML::IXMLDOMNode*& pNode

	OUTPUT PARAMETERS	: nil

	RETURNS				: ERR

	REMARKS				: nil

**************************************************************/


ERR CGCMLGenerator::GetNewDataNode(CData& data,MSXML::IXMLDOMNode*& pNode )
{
	return 	CreateDataNode(data,(MSXML::IXMLDOMElement*&)pNode);
}
/**************************************************************

/**************************************************************

	FUNCTION NAME		: CreateDataNode	

  	PURPOSE				: Function generates a Data node from the object passed

	INPUT PARAMETERS	: CData& data, MSXML::IXMLDOMElement*& pNewNode

	OUTPUT PARAMETERS	: nil

	RETURNS				: ERR

	REMARKS				: It is used internally in the dcml handler class

**************************************************************/

ERR CGCMLGenerator::CreateDataNode(CData& data, MSXML::IXMLDOMElement*& pNewNode)
{
	ERR			err			=	SUCCESS;
	VARIANT vtValue;
	_bstr_t	bstr;
	CDCMLHandler	dcmlHandlerObj;
	TCHAR	tcID[10];
	MSXML::IXMLDOMElement*	pChildNode;
	MSXML::IXMLDOMNode*	pOutNode;
	CValue val;
//create the node for data and add the attributes into it
	if((err = dcmlHandlerObj.CreateElement( TAG_DATA,pNewNode))== SUCCESS)
	{
		VariantInit(&vtValue);
		vtValue.vt = VT_BSTR;
		vtValue.bstrVal = _bstr_t(data.GetName().GetBuffer(0));
		if(dcmlHandlerObj.AddAttribute(pNewNode,TAG_NAME,vtValue) != SUCCESS)
		{
			err = FAILURE;
		}
		vtValue.bstrVal = _bstr_t(data.GetDescription().GetBuffer(0));
		if(dcmlHandlerObj.AddAttribute(pNewNode,TAG_DESCRIPTION,vtValue) != SUCCESS)
		{
			err = FAILURE;
		}
		vtValue.bstrVal = bstr = _itow(data.GetFrequency(),tcID,10);
		if(dcmlHandlerObj.AddAttribute(pNewNode,TAG_FREQUENCY,vtValue) != SUCCESS)
		{
			err = FAILURE;
		}
		vtValue.bstrVal = bstr = _itow(data.GetTimer(),tcID,10);
		if(dcmlHandlerObj.AddAttribute(pNewNode,TAG_TIMER,vtValue) != SUCCESS)
		{
			err = FAILURE;
		}
		///now add the node for the value list after getting the value list from cdata object
		CList<CValue,CValue&>& valueList= data.GetValueList();
		for( POSITION valpos = valueList.GetHeadPosition(); valpos != NULL;)
		{
			val = (CValue&)(valueList.GetNext( valpos )) ;
			if(CreateValueNode(val,pChildNode) == SUCCESS)
			{
				if(dcmlHandlerObj.AddChild(pNewNode,pChildNode,pOutNode) == SUCCESS)
				{
					RELEASE_OBJECT(pOutNode)
				}
				RELEASE_OBJECT(pChildNode)
			}
		}
	}
	return err;
}

ERR CGCMLGenerator::CreateValueNode(CValue& value, MSXML::IXMLDOMElement*& pNewNode)
{
	ERR			err			=	SUCCESS;
	VARIANT vtValue;
	_bstr_t	bstr;
	CDCMLHandler	dcmlHandlerObj;
	CString valueString;
	if((err = dcmlHandlerObj.CreateElement( TAG_VALUE,pNewNode))== SUCCESS)
	{
		VariantInit(&vtValue);
		vtValue.vt = VT_BSTR;
		vtValue.bstrVal = _bstr_t(value.GetName().GetBuffer(0));
		if(dcmlHandlerObj.AddAttribute(pNewNode,TAG_NAME,vtValue) != SUCCESS)
		{
			err = FAILURE;
		}
		vtValue.bstrVal = _bstr_t(value.GetDescription().GetBuffer(0));
		if(dcmlHandlerObj.AddAttribute(pNewNode,TAG_DESCRIPTION,vtValue) != SUCCESS)
		{
			err = FAILURE;
		}
		if(value.GetSourceType() == CONSTANT)
		{
			valueString.Format(_T("%s%s"),_T("[ct]"),value.GetSourceValue());
			/*add the string with value *//*value.GetSourceType + value.GetSourceValue)*/
		}
		else if(value.GetSourceType() == DLG)
		{
			valueString.Format(_T("%s%s"),_T("[dlg]"),value.GetSourceValue());
			/*add the string with value *//*value.GetSourceType + value.GetSourceValue)*/
		}
		else if(value.GetSourceType() == FL)
		{
			valueString.Format(_T("%s%s"),_T("[fl]"),value.GetSourceValue());
			/*add the string with value *//*value.GetSourceType + value.GetSourceValue)*/
		}
		else if(value.GetSourceType() == REQ)
		{
			valueString.Format(_T("%s%s"),_T("req"),value.GetSourceValue());
			/*add the string with value *//*value.GetSourceType + value.GetSourceValue)*/
		}
		vtValue.bstrVal = _bstr_t(valueString.GetBuffer(0));
		if(dcmlHandlerObj.AddAttribute(pNewNode,TAG_SOURCE,vtValue) != SUCCESS)
		{
			err = FAILURE;
		}
	}
	return err;
}

void  CGCMLGenerator::SetDOM(MSXML::IXMLDOMDocument *pDom)
{
	m_DCMLObj.SetDOMHandler(pDom);
}

ERR CGCMLGenerator::UpdateDataListNode(CDataList dataList,MSXML::IXMLDOMNamedNodeMap* pNodeMap)
{
	return 	EditDataListNode(dataList,pNodeMap);
}

ERR CGCMLGenerator::EditDataListNode(CDataList dataList,MSXML::IXMLDOMNamedNodeMap* pNodeMap)
{
	ERR			err			=	SUCCESS;
	CDCMLHandler	dcmlHandlerObj;
	CString	csValue;	
	csValue = dataList.GetDefault();
	if(dcmlHandlerObj.SetAttributeValue(pNodeMap,TAG_DEFAULT,csValue.GetBuffer(0)) != SUCCESS)
	{
		err = FAILURE;
	}
	///now EDIT the node for the value list after getting the value list from cdata object
	return err;
}

ERR CGCMLGenerator::UpdateDataSetNode(CData& dataSet,MSXML::IXMLDOMNamedNodeMap* pNodeMap)
{
	return 	EditDataSetNode(dataSet,pNodeMap);
}

ERR CGCMLGenerator::EditDataSetNode(CData& dataSet,MSXML::IXMLDOMNamedNodeMap* pNodeMap)
{
	ERR			err			=	SUCCESS;
	CDCMLHandler	dcmlHandlerObj;
	CString	csValue;	
	INT32 intValue = dataSet.GetFrequency();
	csValue.Format(_T("%d"),intValue);
	if(dcmlHandlerObj.SetAttributeValue(pNodeMap,TAG_FREQUENCY,csValue.GetBuffer(0)) != SUCCESS)
	{
		err = FAILURE;
	}
	intValue = dataSet.GetTimer();
	csValue.Format(_T("%d"),intValue);
	if(dcmlHandlerObj.SetAttributeValue(pNodeMap,TAG_TIMER,csValue.GetBuffer(0)) != SUCCESS)
	{
		err = FAILURE;
	}	///now EDIT the node for the value list after getting the value list from cdata object
	

	return err;
}

ERR CGCMLGenerator::UpdateFormatNode(CFormat& format,MSXML::IXMLDOMNamedNodeMap* pNodeMap)
{
	return 	EditFormatNode(format,pNodeMap);
}

ERR CGCMLGenerator::EditFormatNode(CFormat& format,MSXML::IXMLDOMNamedNodeMap* pNodeMap)
{
	ERR			err			=	SUCCESS;
	CDCMLHandler	dcmlHandlerObj;
	CString	csValue;
	csValue = format.GetInputMode();
	if(dcmlHandlerObj.SetAttributeValue(pNodeMap,TAG_INPUTMODE,csValue.GetBuffer(0)) != SUCCESS)
	{
		err = FAILURE;
	}
	return err;
}

ERR CGCMLGenerator::UpdateIncomingMessageNode(CMessage msg,MSXML::IXMLDOMNamedNodeMap* pNodeMap)
{
	return 	EditIncomingMessageNode(msg,pNodeMap);
}

ERR CGCMLGenerator::EditIncomingMessageNode(CMessage msg,MSXML::IXMLDOMNamedNodeMap* pNodeMap)
{
	ERR			err			=	SUCCESS;
	CDCMLHandler	dcmlHandlerObj;
	CString	csValue;
	csValue = msg.GetResponseMode();
	if(dcmlHandlerObj.SetAttributeValue(pNodeMap,TAG_RESPONSE,csValue.GetBuffer(0)) != SUCCESS)
	{
		err = FAILURE;
	}
	return err;
}

ERR CGCMLGenerator::UpdateValueNode(CValue value,MSXML::IXMLDOMNamedNodeMap* pNodeMap)
{
	return 	EditValueNode(value,pNodeMap);
}

ERR CGCMLGenerator::EditValueNode(CValue value, MSXML::IXMLDOMNamedNodeMap* pNodeMap)
{
	ERR			err			=	SUCCESS;
	CDCMLHandler	dcmlHandlerObj;
	CString valueString;
	if(value.GetSourceType() == CONSTANT)
	{
		valueString.Format(_T("%s%s"),_T("[ct]"),value.GetSourceValue());
		/*add the string with value *//*value.GetSourceType + value.GetSourceValue)*/
	}
	else if(value.GetSourceType() == DLG)
	{
		valueString.Format(_T("%s%s"),_T("[dlg]"),value.GetSourceValue());
		/*add the string with value *//*value.GetSourceType + value.GetSourceValue)*/
	}
	else if(value.GetSourceType() == FL)
	{
		valueString.Format(_T("%s%s"),_T("[fl]"),value.GetSourceValue());
		/*add the string with value *//*value.GetSourceType + value.GetSourceValue)*/
	}
	else if(value.GetSourceType() == REQ)
	{
		valueString.Format(_T("%s%s"),_T("req"),value.GetSourceValue());
		/*add the string with value *//*value.GetSourceType + value.GetSourceValue)*/
	}
	if(dcmlHandlerObj.SetAttributeValue(pNodeMap,TAG_SOURCE,valueString.GetBuffer(0)) != SUCCESS)
	{
		err = FAILURE;
	}	
	return err;
}