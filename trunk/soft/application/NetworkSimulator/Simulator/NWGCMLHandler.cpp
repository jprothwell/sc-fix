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

	FILENAME	: NWGCMLHandler.cpp

  	PURPOSE		: It is the middle layer between the XML layer and the MMI intyerface
					layer.

	REMARKS		: nil

	AUTHOR		:  Vikram

	DATE		: Oct 10,03

**************************************************************/

#include "stdafx.h"
#include "NWGCMLHandler.h"
#include "ObjectSearch.h"
#include "GCMLGenerator.h"
//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//void CNWGCMLHandler::FreeAll();

CNWGCMLHandler::CNWGCMLHandler()//:CDCMLHandler()
{
}

CNWGCMLHandler::CNWGCMLHandler(LPTSTR pFileName):CDCMLHandler(pFileName)
{
}

CNWGCMLHandler::~CNWGCMLHandler()
{

}

/**************************************************************

	FUNCTION NAME		: GetDataList

  	PURPOSE				: Return a list of all data objects to the upper layer

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: CPtrList*

	REMARKS				: This function looks for all data lists in the XML.
							Makes objects of these data lists and returns a list of 
							objects.

**************************************************************/
 

CPtrList* CNWGCMLHandler::GetDataList()
{
	MSXML::IXMLDOMNodeList*		pNodeList; 
	MSXML::IXMLDOMNode*			pNode; 
	UINT16	iCount	=	0;
	INT32	nTotalNodes			=	0;
	CPtrList*	pPtrList	=	NULL;
	CDataList*	pData	=	NULL;

	if(GetTotalNodesOfType(TAG_DATALIST,pNodeList,&nTotalNodes) == SUCCESS)
	{
		while(nTotalNodes)
		{
			if(!pPtrList)
      {
        ALLOCATE_MEM_ARR(pPtrList,CPtrList,10);
      }
			pNodeList->nextNode(&pNode);
			ALLOCATE_MEM(pData,CDataList);
			GetDataListDetails(pNode,*pData);
			pPtrList->AddTail(pData);
			--nTotalNodes;
		}
	}

	return pPtrList;
}

/**************************************************************

	FUNCTION NAME		: GetDataTypeList

  	PURPOSE				: Return a list of all datatype objects to the upper layer

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: CPtrList*

	REMARKS				: This function looks for all datatype lists in the XML.
							Makes objects of these datatype lists and returns a list of 
							objects.

**************************************************************/
 

CPtrList* CNWGCMLHandler::GetDataTypeList()
{
	MSXML::IXMLDOMNodeList*		pNodeList; 
	MSXML::IXMLDOMNode*			pNode; 
	MSXML::IXMLDOMNamedNodeMap*		pNodeMap	=	NULL;

	UINT16	iCount	=	0;
	INT32	nTotalNodes			=	0;
	LPTSTR	pVal	=	NULL;
	CPtrList*	pPtrList	=	NULL;
	CDataType*	pDataype	=	NULL;

	if(GetTotalNodesOfType(TAG_DATATYPE,pNodeList,&nTotalNodes) == SUCCESS)
	{
		while(nTotalNodes)
		{
			if(!pPtrList)
				ALLOCATE_MEM_ARR(pPtrList,CPtrList,10);
			pNodeList->nextNode(&pNode);
			pDataype = new CDataType;
			//ALLOCATE_MEM(pDataype,CDataType);
			pNodeMap = GetAllAttributes(pNode);
			if(pNodeMap)
			{
				GetAttributeValue(pNodeMap,TAG_NAME,pVal);
				if(pVal){
					pDataype->SetName(CString(pVal));
					DELETE_MEM_ARR(pVal);
				}
				GetAttributeValue(pNodeMap,TAG_SIZE,pVal);
				if(pVal){
					pDataype->SetSize(_ttoi(CString(pVal)));
					DELETE_MEM_ARR(pVal);
				}
			}
			pPtrList->AddTail(pDataype);
			--nTotalNodes;
		}//while(nTotalNodes)
	}
	return pPtrList;
}
///kmalik added the support for getting the application list from xml
CPtrList* CNWGCMLHandler::GetApplicationList()
{
	MSXML::IXMLDOMNodeList*		pNodeList; 
	MSXML::IXMLDOMNode*			pNode; 
	MSXML::IXMLDOMNamedNodeMap*		pNodeMap	=	NULL;

	UINT16	iCount	=	0;
	INT32	nTotalNodes			=	0;
	LPTSTR	pVal	=	NULL;
	CPtrList*	pPtrList	=	NULL;
	CApplication*	pApp	=	NULL;

	if(GetTotalNodesOfType(TAG_APPLICATION,pNodeList,&nTotalNodes) == SUCCESS)
	{
		while(nTotalNodes)
		{
			if(!pPtrList)
      {
        ALLOCATE_MEM_ARR(pPtrList,CPtrList,10);
      }
			pNodeList->nextNode(&pNode);
			pApp = new CApplication;
			//ALLOCATE_MEM(pDataype,CDataType);
			pNodeMap = GetAllAttributes(pNode);
			if(pNodeMap)
			{
				GetAttributeValue(pNodeMap,TAG_ID,pVal);
				if(pVal){
					pApp->SetAppId(_ttoi(CString(pVal)));
					DELETE_MEM_ARR(pVal);
				}
				GetAttributeValue(pNodeMap,TAG_NAME,pVal);
				if(pVal){
					pApp->SetApplicationName(CString(pVal));
					DELETE_MEM_ARR(pVal);
				}
			}
			pPtrList->AddTail(pApp);
			--nTotalNodes;
		}//while(nTotalNodes)
	}
	return pPtrList;
}

/**************************************************************

	FUNCTION NAME		: GetFormatsList

  	PURPOSE				: Return a list of all format objects to the upper layer

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: CPtrList*

	REMARKS				: This function looks for all format in the XML.
							Makes objects of these formats and returns a list of 
							objects.

**************************************************************/
 

CPtrList* CNWGCMLHandler::GetFormatsList()
{
	MSXML::IXMLDOMNodeList*		pNodeList; 
	MSXML::IXMLDOMNode*			pNode; 
	UINT16	iCount	=	0;
	INT32	nTotalNodes			=	0;
	CPtrList*	pPtrList	=	NULL;	
	CPtrList*	pDataList	=	NULL;
	CPtrList*	pDataTypeList	=	NULL;
	CFormat*	pFormat	=	NULL;

	if(GetTotalNodesOfType(TAG_FORMATS,pNodeList,&nTotalNodes) == SUCCESS)
	{
		pDataList = GetDataList();
		pDataTypeList = GetDataTypeList();
		while(nTotalNodes)
		{
			if(!pPtrList)
      {
        ALLOCATE_MEM_ARR(pPtrList,CPtrList,10);
      }
			pNodeList->nextNode(&pNode);
			ALLOCATE_MEM(pFormat,CFormat);
			GetFormatDetails(pNode,*pFormat,pDataList,pDataTypeList);
			pPtrList->AddTail(pFormat);
			--nTotalNodes;
		}
	}

	return pPtrList;
}

/**************************************************************

	FUNCTION NAME		: GetDataListDetails

  	PURPOSE				: Gets the Data list informationn object from DOM node

	INPUT PARAMETERS	: MSXML::IXMLDOMNode*

	OUTPUT PARAMETERS	: CDataList

	RETURNS				: ERR

	REMARKS				: This function creates a DataList object out of the DOM node 
							and passes the object as an output paramater

**************************************************************/

ERR CNWGCMLHandler::GetDataListDetails(MSXML::IXMLDOMNode* pNode, CDataList& data)
{
	MSXML::IXMLDOMNode*		pChildNode	=	NULL;
	MSXML::IXMLDOMNamedNodeMap*		pNodeMap	=	NULL;
	MSXML::IXMLDOMNodeList*		pChildNodeList; 
	LPTSTR		pVal	=	NULL;

	pNodeMap = GetAllAttributes(pNode);
	if(pNodeMap)
	{
		GetAttributeValue(pNodeMap,TAG_NAME,pVal);
		if(pVal){
			data.SetName(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}
		//get default data set name
		GetAttributeValue(pNodeMap,TAG_DEFAULT,pVal);
		if(pVal){
			data.SetDefault(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}
		
		RELEASE_OBJECT(pNodeMap)
	}

	if(GetAllChildren(pNode,pChildNodeList) == SUCCESS)
	{
		long iListSize = GetListSize(pChildNodeList);
		while(iListSize)
		{
			GetNextNode(pChildNodeList,pChildNode);
			CData	dataObj;

			dataObj.GetValueList().RemoveAll();
			if(GetDataDetails(pChildNode,dataObj) == SUCCESS)
			{
				data.AddValue(dataObj);
			}
			RELEASE_OBJECT(pChildNode);
			--iListSize;
		}
		RELEASE_OBJECT(pChildNodeList);
	}

	return SUCCESS;
}


/**************************************************************

	FUNCTION NAME		: GetDataDetails

  	PURPOSE				: Gets the Data object informationn object from DOM node

	INPUT PARAMETERS	: MSXML::IXMLDOMNode*

	OUTPUT PARAMETERS	: CData

	RETURNS				: ERR

	REMARKS				: This function creates a Data object out of the DOM node 
							and passes the object as an output paramater

**************************************************************/


ERR CNWGCMLHandler::GetDataDetails(MSXML::IXMLDOMNode* pNode, CData& data)
{
	MSXML::IXMLDOMNamedNodeMap*		pNodeMap	=	NULL;
	MSXML::IXMLDOMNode*		pChildNode	=	NULL;
	MSXML::IXMLDOMNodeList*		pChildNodeList; 

	LPTSTR		pVal	=	NULL;
	pNodeMap = GetAllAttributes(pNode);
	if(pNodeMap)
	{
		GetAttributeValue(pNodeMap,TAG_NAME,pVal);
		if(pVal){
			data.SetName(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}
		//added for description
		GetAttributeValue(pNodeMap,TAG_DESCRIPTION,pVal);
		if(pVal)
		{
			data.SetDescription(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}
		//added for frequency
		GetAttributeValue(pNodeMap,TAG_FREQUENCY,pVal);
		if(pVal)
		{
			data.SetFrequency(_ttoi(CString(pVal)));
			DELETE_MEM_ARR(pVal);
		}
		//added for timer
		GetAttributeValue(pNodeMap,TAG_TIMER,pVal);
		if(pVal)
		{
			data.SetTimer(_ttoi(CString(pVal)));
			DELETE_MEM_ARR(pVal);
		}

		if(GetAllChildren(pNode,pChildNodeList) == SUCCESS)
		{
			long iListSize = GetListSize(pChildNodeList);
			while(iListSize)
			{
				CValue		val;

				GetNextNode(pChildNodeList,pChildNode);
				pVal = GetAttributeValue(pChildNode,TAG_NAME);
				if(pVal)
				{
					val.SetName(pVal);
					DELETE_MEM_ARR(pVal);
				}

				pVal = GetAttributeValue(pChildNode,TAG_DESCRIPTION);
				if(pVal)
				{
					val.SetDescription(pVal);
					DELETE_MEM_ARR(pVal);
				}

				pVal = GetAttributeValue(pChildNode,TAG_SOURCE);
				if(pVal)
				{
					CString csType(pVal);
					int	nPos=0;
					if( (nPos=csType.Find(_T("[ct]"))) != -1)
					{
						val.SetSourceType(CONSTANT);
						val.SetSourceValue(csType.Mid(nPos+4));
						
					}
					else if((nPos=csType.Find(_T("[dlg]"))) !=-1)
					{
						val.SetSourceType(DLG);
						val.SetSourceValue(csType.Mid(nPos+5));

					}
					else if((nPos=csType.Find(_T("[file]"))) !=-1)
					{
						val.SetSourceType(FL);
						val.SetSourceValue(csType.Mid(nPos+6));

					}
					else if((nPos=csType.Find(_T("[req]"))) !=-1)
					{
						val.SetSourceType(REQ);
						val.SetSourceValue(csType.Mid(nPos+5));

					}
					


					DELETE_MEM_ARR(pVal);
				}
				data.AddValue(val);
				RELEASE_OBJECT(pChildNode);
				--iListSize;
			}
			RELEASE_OBJECT(pChildNodeList);
		}

	}
	return SUCCESS;


}


/**************************************************************

	FUNCTION NAME		: GetFormatDetails

  	PURPOSE				: Gets the Format object informationn object from DOM node

	INPUT PARAMETERS	: MSXML::IXMLDOMNode*,CPtrList* pDataList,CPtrList* pDataTypeList)

	OUTPUT PARAMETERS	: CFormat

	RETURNS				: ERR

	REMARKS				: This function looks for the CFormat object inside the DOM by matching it
							with thr Data objects list.

**************************************************************/


ERR CNWGCMLHandler::GetFormatDetails(MSXML::IXMLDOMNode* pNode, CFormat& format,CPtrList* pDataList,CPtrList* pDataTypeList)
{
	MSXML::IXMLDOMNamedNodeMap*		pNodeMap	=	NULL;
	MSXML::IXMLDOMNode*		pChildNode	=	NULL;
	MSXML::IXMLDOMNodeList*		pChildNodeList; 

	LPTSTR		pVal	=	NULL;
	CDataType*	pdataType;
	CDataList*		pdataObj;
	pNodeMap = GetAllAttributes(pNode);

	if(pNodeMap)
	{
		GetAttributeValue(pNodeMap,TAG_NAME,pVal);
		if(pVal){
			format.SetName(CString(pVal));
// 			if(CString(_T("fmt4")) == _T("fmt4"))
// 				int kkk = 0;
			DELETE_MEM_ARR(pVal);
		}
		GetAttributeValue(pNodeMap,TAG_PLUGIN,pVal);
		if(pVal){
			format.SetPlugin(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}

		GetAttributeValue(pNodeMap,TAG_DLGNAME,pVal);
		if(pVal){
			format.SetDialog(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}

		GetAttributeValue(pNodeMap,TAG_INPUTMODE,pVal);
		if(pVal){
			format.SetInputMode(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}
		if(GetAllChildren(pNode,pChildNodeList) == SUCCESS)
		{
			CVar		var;

			long iListSize = GetListSize(pChildNodeList);
			CObjectSearch<CDataType> objectSearch;
			while(iListSize)
			{
				GetNextNode(pChildNodeList,pChildNode);
				pVal = GetAttributeValue(pChildNode,TAG_NAME);
				if(pVal)
				{
					var.SetName(pVal);
					DELETE_MEM_ARR(pVal);
				}
				pVal = GetAttributeValue(pChildNode,TAG_TYPE);
				if(pVal)
				{

					if(objectSearch.GetObject(pDataTypeList,CString(pVal),&pdataType)==SUCCESS)
					{
						var.SetDataType(*pdataType);
					}

					DELETE_MEM_ARR(pVal);
				}
				pVal = GetAttributeValue(pChildNode,TAG_COUNT);
				if(pVal)
				{
					var.SetCount(_ttoi(CString(pVal)));
					DELETE_MEM_ARR(pVal);
				}
				pVal = GetAttributeValue(pChildNode,TAG_VALUESIZE);
				if(pVal)
				{
					var.SetValueSize(_ttoi(CString(pVal)));
					DELETE_MEM_ARR(pVal);
				}
				pVal = GetAttributeValue(pChildNode,TAG_DESCRIPTION);
				if(pVal)
				{
					var.SetDescription(CString(pVal));
					DELETE_MEM_ARR(pVal);
				}
				format.AddVariable(var);
				RELEASE_OBJECT(pChildNode);
				--iListSize;
			}
			RELEASE_OBJECT(pChildNodeList);
		}
		GetAttributeValue(pNodeMap,TAG_DATALISTNAME,pVal);
		if(pVal){
			CObjectSearch<CDataList> dataSearch;
			if(dataSearch.GetObject(pDataList,pVal,&pdataObj)==SUCCESS)
			{
				format.SetDataList(*pdataObj);
			}
			DELETE_MEM_ARR(pVal);
		}
		RELEASE_OBJECT(pNodeMap)
	}      
	return SUCCESS;


}

/**************************************************************

	FUNCTION NAME		: GetIncomingMessageList

  	PURPOSE				: Return a list of all Incoming messages objects to the upper layer

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: CPtrList*

	REMARKS				: This function looks for all the incoming messages in the XML.
							Makes objects of these incoming messages and returns a list of 
							objects.

**************************************************************/

CPtrList* CNWGCMLHandler::GetIncomingMessageList(LPTSTR pNodeName)
{
	MSXML::IXMLDOMNodeList*		pNodeList; 
	MSXML::IXMLDOMNode*			pNode; 
	UINT16	iCount	=	0;
	INT32	nTotalNodes			=	0;
	CPtrList*	pPtrList	=	NULL;
	CIncomingMsg*	pIncomingMessage	=	NULL;

	if(GetTotalNodesOfType(pNodeName,pNodeList,&nTotalNodes) == SUCCESS)
	{
		while(nTotalNodes)
		{
			if(!pPtrList)
      {
        ALLOCATE_MEM_ARR(pPtrList,CPtrList,10);
      }
			pNodeList->nextNode(&pNode);
			ALLOCATE_MEM(pIncomingMessage,CIncomingMsg);
			GetIncomingMessageDetails(pNode,*pIncomingMessage);
			pPtrList->AddTail(pIncomingMessage);
			--nTotalNodes;
			RELEASE_OBJECT(pNode);
		}

	}

	return pPtrList;
}

/**************************************************************

	FUNCTION NAME		: GetOutGoingMessageList

  	PURPOSE				: Return a list of all Outgoing messages objects to the upper layer

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: CPtrList*

	REMARKS				: This function looks for all the outgoing messages in the XML.
							Makes objects of these outgoing messages and returns a list of 
							objects.

**************************************************************/

CPtrList* CNWGCMLHandler::GetOutGoingMessageList(LPTSTR pNodeName)
{
	MSXML::IXMLDOMNodeList*		pNodeList; 
	MSXML::IXMLDOMNode*			pNode; 
	UINT16	iCount	=	0;
	INT32	nTotalNodes			=	0;
	CPtrList*	pPtrList	=	NULL;
	COutgoingMessage*	pOutgoingMessage	=	NULL;

	if(GetTotalNodesOfType(pNodeName,pNodeList,&nTotalNodes) == SUCCESS)
	{
		while(nTotalNodes)
		{
			if(!pPtrList)
      {
        ALLOCATE_MEM_ARR(pPtrList,CPtrList,10);
      }
			pNodeList->nextNode(&pNode);
			ALLOCATE_MEM(pOutgoingMessage,COutgoingMessage);
			GetOutgoingMessageDetails(pNode,*pOutgoingMessage);
			pPtrList->AddTail(pOutgoingMessage);
			--nTotalNodes;
			RELEASE_OBJECT(pNode);
		}

	}

	return pPtrList;
}


/**************************************************************

	FUNCTION NAME		: GetAsyncMessageList

  	PURPOSE				: Return a list of all Async messages objects to the upper layer

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: CPtrList*

	REMARKS				: This function looks for all the Async messages in the XML.
							Makes objects of these Async messages and returns a list of 
							objects.

**************************************************************/

CPtrList* CNWGCMLHandler::GetAsyncMessageList(LPTSTR pNodeName)
{
	MSXML::IXMLDOMNodeList*		pNodeList; 
	MSXML::IXMLDOMNode*			pNode; 
	UINT16	iCount	=	0;
	INT32	nTotalNodes			=	0;
	CPtrList*	pPtrList	=	NULL;
	CAsyncMessageSeq*	pMessageSeq	=	NULL;

	if(GetTotalNodesOfType(pNodeName,pNodeList,&nTotalNodes) == SUCCESS)
	{
		while(nTotalNodes)
		{
			if(!pPtrList)
      {
        ALLOCATE_MEM_ARR(pPtrList,CPtrList,10);
      }
			pNodeList->nextNode(&pNode);
			ALLOCATE_MEM(pMessageSeq,CAsyncMessageSeq);
			GetAsyncMessageDetails(pNode,*pMessageSeq);
			pPtrList->AddTail(pMessageSeq);
			--nTotalNodes;
			RELEASE_OBJECT(pNode);
		}

	}

	return pPtrList;
}

/**************************************************************

	FUNCTION NAME		: GetAsyncMessageDetails

  	PURPOSE				: Gets the AsyncMessage informationn object from DOM node

	INPUT PARAMETERS	: MSXML::IXMLDOMNode*

	OUTPUT PARAMETERS	: CAsyncMessageSeq

	RETURNS				: ERR

	REMARKS				: This function creates a CAsyncMessageSeq object out of the DOM node 
							and passes the object as an output paramater

**************************************************************/

ERR CNWGCMLHandler::GetAsyncMessageDetails(MSXML::IXMLDOMNode* pNode, CAsyncMessageSeq& message)
{
	ERR			err		=	FAILURE;
	MSXML::IXMLDOMNode*		pChildNode	=	NULL;
	MSXML::IXMLDOMNodeList*		pChildNodeList; 
	MSXML::IXMLDOMNamedNodeMap*		pNodeMap	=	NULL;
	LPTSTR		pVal	=	NULL;

	pNodeMap = GetAllAttributes(pNode);
	if(pNodeMap)
	{
		GetAttributeValue(pNodeMap,TAG_ID,pVal);
		if(pVal){
			message.SetMessageId(_ttoi(CString(pVal)));
			DELETE_MEM_ARR(pVal);
		}
		GetAttributeValue(pNodeMap,TAG_NAME,pVal);
		if(pVal){
			message.SetMessageName(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}

		GetAttributeValue(pNodeMap,TAG_APPID,pVal);
		if(pVal){
			message.SetAppId(_ttoi(CString(pVal)));
			DELETE_MEM_ARR(pVal);
		}

		RELEASE_OBJECT(pNodeMap)
	}

	if(GetAllChildren(pNode,pChildNodeList) == SUCCESS)
	{
		long iListSize = GetListSize(pChildNodeList);
		while(iListSize)
		{
			GetNextNode(pChildNodeList,pChildNode);
			pVal = GetAttributeValue(pChildNode,TAG_ID);
			if(pVal)
			{
				message.AddMessage(_ttoi(pVal));
				DELETE_MEM_ARR(pVal);
			}
			RELEASE_OBJECT(pChildNode);
			--iListSize;
		}
		RELEASE_OBJECT(pChildNodeList);
	}


	return err;
}

/**************************************************************

	FUNCTION NAME		: GetIncomingMessageDetails

  	PURPOSE				: Gets the Incoming message informationn object from DOM node

	INPUT PARAMETERS	: MSXML::IXMLDOMNode*

	OUTPUT PARAMETERS	: CIncomingMsg

	RETURNS				: ERR

	REMARKS				: This function creates a CIncomingMsg object out of the DOM node 
							and passes the object as an output paramater

**************************************************************/

ERR CNWGCMLHandler::GetIncomingMessageDetails(MSXML::IXMLDOMNode* pNode, CIncomingMsg& message)
{
	ERR			err		=	FAILURE;
	MSXML::IXMLDOMNode*		pChildNode	=	NULL;
	MSXML::IXMLDOMNodeList*		pChildNodeList; 
	LPTSTR		pval	=	NULL;

	if((err = GetMessageDetails(pNode,message)) == SUCCESS)
	{
		if(GetAllChildren(pNode,pChildNodeList) == SUCCESS)
		{
			long iListSize = GetListSize(pChildNodeList);
			while(iListSize)
			{
				GetNextNode(pChildNodeList,pChildNode);
				pval = GetAttributeValue(pChildNode,TAG_ID);
				if(pval)
				{
					message.AddResponseMessageId(_ttoi(pval));
					DELETE_MEM_ARR(pval);
				}
				pval = GetAttributeValue(pChildNode,TAG_INPUT_RESPONSE_SEQ);
				if(pval)
				{
					message.AddAsyncSeqId(_ttoi(pval));
					DELETE_MEM_ARR(pval);
				}
				RELEASE_OBJECT(pChildNode);
				--iListSize;
			}
			RELEASE_OBJECT(pChildNodeList);
		}

	}

	return err;
}

/**************************************************************

	FUNCTION NAME		: GetOutgoingMessageDetails

  	PURPOSE				: Gets the Outgoing message informationn object from DOM node

	INPUT PARAMETERS	: MSXML::IXMLDOMNode*

	OUTPUT PARAMETERS	: COutgoingMessage

	RETURNS				: ERR

	REMARKS				: This function creates a COutgoingMessage object out of the DOM node 
							and passes the object as an output paramater

**************************************************************/

ERR CNWGCMLHandler::GetOutgoingMessageDetails(MSXML::IXMLDOMNode* pNode, COutgoingMessage& message)
{
	ERR			err		=	FAILURE;

	if((err = GetMessageDetails(pNode,message)) == SUCCESS)
	{


	}

	return err;
}
/**************************************************************

	FUNCTION NAME		: GetMessageDetails

  	PURPOSE				: Gets the  message informationn object from DOM node

	INPUT PARAMETERS	: MSXML::IXMLDOMNode*

	OUTPUT PARAMETERS	: CMessage

	RETURNS				: ERR

	REMARKS				: This function creates a CMessage object out of the DOM node 
							and passes the object as an output paramater

**************************************************************/


ERR CNWGCMLHandler::GetMessageDetails(MSXML::IXMLDOMNode* pNode, CMessage& message)
{
	MSXML::IXMLDOMNamedNodeMap*		pNodeMap	=	NULL;
	LPTSTR		pVal	=	NULL;

	pNodeMap = GetAllAttributes(pNode);
	if(pNodeMap)
	{
		GetAttributeValue(pNodeMap,TAG_ID,pVal);
		if(pVal){
			message.SetMessageId(_ttoi(CString(pVal)));
			DELETE_MEM_ARR(pVal);
		}

		GetAttributeValue(pNodeMap,TAG_NAME,pVal);
		if(pVal){
			message.SetMessageName(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}

		GetAttributeValue(pNodeMap,TAG_APPID,pVal);
		if(pVal){
			message.SetAppId(_ttoi(CString(pVal)));
			DELETE_MEM_ARR(pVal);
		}
		GetAttributeValue(pNodeMap,TAG_FORMATNAME,pVal);
		if(pVal){
			message.SetFormat(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}
		GetAttributeValue(pNodeMap,TAG_PLUGIN,pVal);
		if(pVal){
			message.SetPluginName(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}
		//get response mode auto / manual
		GetAttributeValue(pNodeMap,TAG_RESPONSE,pVal);
		if(pVal){
			message.SetResponseMode(CString(pVal));
			DELETE_MEM_ARR(pVal);
		}


	}
	RELEASE_OBJECT(pNodeMap);

	return SUCCESS;
}
/**************************************************************

	FUNCTION NAME		: FreeAll

  	PURPOSE				: Frees the DOM memory

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: nil
**************************************************************/

void CNWGCMLHandler::FreeAll()
{
	if(m_pXMLDomDocument)
		RELEASE_OBJECT(m_pXMLDomDocument)
}


/**************************************************************

	FUNCTION NAME		: GetBootUpMessageList

  	PURPOSE				: Return a list of all Boot Up message objects to the upper layer

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: CPtrList*

	REMARKS				: This function looks for all the boot up messages in the XML.
							Makes objects of these boot up messages and returns a list of 
							objects.

**************************************************************/

CPtrList* CNWGCMLHandler::GetBootUpMessageList(LPTSTR pNodeName)
{
	MSXML::IXMLDOMNodeList*		pNodeList; 
	MSXML::IXMLDOMNode*			pNode; 
	UINT16	iCount	=	0;
	INT32	nTotalNodes			=	0;
	CPtrList*	pPtrList	=	NULL;
	CBootUpMsg*	pBootUpMessage	=	NULL;

	if(GetTotalNodesOfType(pNodeName,pNodeList,&nTotalNodes) == SUCCESS)
	{
		while(nTotalNodes)
		{
			if(!pPtrList)
      {
        ALLOCATE_MEM_ARR(pPtrList,CPtrList,10);
      }
			pNodeList->nextNode(&pNode);
			ALLOCATE_MEM(pBootUpMessage,CBootUpMsg);
			GetBootUpMessageDetails(pNode,*pBootUpMessage);
			pPtrList->AddTail(pBootUpMessage);
			--nTotalNodes;
			RELEASE_OBJECT(pNode);
		}
	}
	return pPtrList;
}



/**************************************************************

	FUNCTION NAME		: GetBootUpMessageDetails

  	PURPOSE				: Gets the Boot up messages informationn object from DOM node

	INPUT PARAMETERS	: MSXML::IXMLDOMNode*

	OUTPUT PARAMETERS	: CBootUpMsg

	RETURNS				: ERR

	REMARKS				: This function creates a CBootUpMsg object out of the DOM node 
							and passes the object as an output paramater

**************************************************************/

ERR CNWGCMLHandler::GetBootUpMessageDetails(MSXML::IXMLDOMNode* pNode, CBootUpMsg& message)
{
	ERR			err		=	FAILURE;
	MSXML::IXMLDOMNode*		pChildNode	=	NULL;
	MSXML::IXMLDOMNodeList*		pChildNodeList; 
	LPTSTR		pval	=	NULL;
	if(GetAllChildren(pNode,pChildNodeList) == SUCCESS)
	{
		long iListSize = GetListSize(pChildNodeList);
		while(iListSize)
		{
			GetNextNode(pChildNodeList,pChildNode);
			pval = GetAttributeValue(pChildNode,TAG_ID);
			if(pval)
			{
				message.AddInBootMessageId(_ttoi(pval));
				DELETE_MEM_ARR(pval);
			}
			RELEASE_OBJECT(pChildNode);
			--iListSize;
		}
		RELEASE_OBJECT(pChildNodeList);
	}
	return err;
}

/**************************************************************

	FUNCTION NAME		: GetFrequentlyUsedMessageList

  	PURPOSE				: Return a list of all Boot Up message objects to the upper layer

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: CPtrList*

	REMARKS				: This function looks for all the boot up messages in the XML.
							Makes objects of these boot up messages and returns a list of 
							objects.

**************************************************************/

CPtrList* CNWGCMLHandler::GetFrequentlyUsedMessageList()
{
	MSXML::IXMLDOMNodeList*		pNodeList; 
	MSXML::IXMLDOMNode*			pNode; 
	UINT16	iCount	=	0;
	INT32	nTotalNodes			=	0;
	CPtrList*	pPtrList	=	NULL;
	CFreqUsedMsg*	pFreqUsedMessage	=	NULL;

	if(GetTotalNodesOfType(TAG_FREQUSED,pNodeList,&nTotalNodes) == SUCCESS)
	{
		while(nTotalNodes)
		{
			if(!pPtrList)
      {
        ALLOCATE_MEM_ARR(pPtrList,CPtrList,10);
      }
			pNodeList->nextNode(&pNode);
			ALLOCATE_MEM(pFreqUsedMessage,CFreqUsedMsg);
			GetFrequentlyUsedMessageDetails(pNode,*pFreqUsedMessage);
			pPtrList->AddTail(pFreqUsedMessage);
			--nTotalNodes;
			RELEASE_OBJECT(pNode);
		}
	}
	return pPtrList;
}



/**************************************************************

	FUNCTION NAME		: GetFrequentlyUsedMessageDetails

  	PURPOSE				: Gets the Freq used messages informationn object from DOM node

	INPUT PARAMETERS	: MSXML::IXMLDOMNode*

	OUTPUT PARAMETERS	: CFreqUsedMsg

	RETURNS				: ERR

	REMARKS				: This function creates a CFreqUsedMsg object out of the DOM node 
							and passes the object as an output paramater

**************************************************************/

ERR CNWGCMLHandler::GetFrequentlyUsedMessageDetails(MSXML::IXMLDOMNode* pNode, CFreqUsedMsg& message)
{
	ERR			err		=	FAILURE;
	MSXML::IXMLDOMNode*		pChildNode	=	NULL;
	MSXML::IXMLDOMNodeList*		pChildNodeList; 
	LPTSTR		pval	=	NULL;

	if(GetAllChildren(pNode,pChildNodeList) == SUCCESS)
	{
		long iListSize = GetListSize(pChildNodeList);
		while(iListSize)
		{
			GetNextNode(pChildNodeList,pChildNode);
			pval = GetAttributeValue(pChildNode,TAG_ID);
			if(pval)
			{
				message.AddInFreqUsedMessageId(_ttoi(pval));
				DELETE_MEM_ARR(pval);
			}
			RELEASE_OBJECT(pChildNode);
			--iListSize;
		}
		RELEASE_OBJECT(pChildNodeList);
	}

	return err;
}
BOOL CNWGCMLHandler::SetDataSetInDom(CData& dataSet,CDataList dataList)
{	
	return(AddDataSetInDataList(dataSet,dataList));
}

ERR  CNWGCMLHandler::AddDataSetInDataList(CData& dataSet,CDataList datalist)
{
    MSXML::IXMLDOMNodeList*		pNodeList;
	ERR		err		=	FAILURE;
	MSXML::IXMLDOMNode*			pNode;
	MSXML::IXMLDOMNode*			pNewNode;
	MSXML::IXMLDOMNode*			pOutNode;
	UINT16	iCount	=	0;
	INT32	nTotalNodes			=	0;
	CDataList*	pDataFromDOM	=	NULL;
	CPtrList*	pPtrList	=	NULL;
	CGCMLGenerator generator;
	if(GetTotalNodesOfType(TAG_DATALIST,pNodeList,&nTotalNodes) == SUCCESS)
	{
		while(nTotalNodes)
		{
			CString dataListName;
			pNodeList->nextNode(&pNode);
			ALLOCATE_MEM(pDataFromDOM,CDataList);
			///have to get out the details here			
			GetDataListDetails(pNode,*pDataFromDOM);
			dataListName = pDataFromDOM->GetName();
			if(!dataListName.CompareNoCase(datalist.GetName()))
			{
				//DATA LIST FOUND ADD DATA SET HERE
				if((err = generator.GetNewDataNode(dataSet,pNewNode)) == SUCCESS)
					{
						AddChild(pNode,(MSXML::IXMLDOMElement*)pNewNode,pOutNode);
						RELEASE_OBJECT(pNewNode);
						RELEASE_OBJECT(pNode);
						RELEASE_OBJECT(pOutNode);
						err = SUCCESS;
	//					generator.MakeGCML(CMessageManager::m_strXMLFile);//enable this to generate the xml file
						break;
					}
				}
				--nTotalNodes;			
		}
	}
	return err;
}

ERR CNWGCMLHandler::GetDataSetToAddFromDataList(CData& dataNew,CDataList newDataList,CDataList *dataListFromDOM)
{
	//return the newly added data
	CData dataDom;
	BOOL found = FALSE;
	CList<CData,CData&>& newdata= newDataList.GetData();
	CList<CData,CData&>& domdata= dataListFromDOM->GetData();	
	if((newdata.GetCount() > 0) && (domdata.GetCount() > 0))
	{
		for( POSITION datapos = newdata.GetHeadPosition(); datapos != NULL;)
		{
			dataNew = (newdata.GetNext( datapos ));
			found = FALSE;
			for( POSITION pos = domdata.GetHeadPosition(); pos != NULL;)
			{
				dataDom = (domdata.GetNext(pos));
				if(!dataDom.GetName().CompareNoCase(dataNew.GetName()))
				{
					found = TRUE;
					break;
				}				
			}
			if(!found)
				break;
		}
	}
	if(!found)
		return SUCCESS;
	else
		return FAILURE;
}

/****************************************************************************************

	FUNCTION NAME		: UpdateBootupMsgListInDom

  	PURPOSE				: Update the newly prepared bootup message sequence in the DOM

	INPUT PARAMETERS	: CList <UINT32, UINT32>& 

	OUTPUT PARAMETERS	: BOOL

	RETURNS				: ERR

	REMARKS				: This function first remove the exisiting message sequence list 
						  and then writes the new bootup message sequence. 

*****************************************************************************************/

BOOL CNWGCMLHandler::UpdateBootupMsgListInDom(CList <UINT32, UINT32>& MsgList,LPTSTR pNodeName)
{	
	if(RemoveBootupListEntries(pNodeName)==SUCCESS)
	{
		if(AddBootupMsgIdinList(MsgList,pNodeName)==SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
		return false;
}


/****************************************************************************************

	FUNCTION NAME		: RemoveBootupListEntries

  	PURPOSE				: Remove the previous bootup message sequence list from the DOM

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: ERR

	RETURNS				: ERR

	REMARKS				: This function will remove the exisiting message sequence list 
						  from the DOM

*****************************************************************************************/


ERR  CNWGCMLHandler::RemoveBootupListEntries(LPTSTR pNodeName)
{
	MSXML::IXMLDOMNodeList*		pNodeList;
  MSXML::IXMLDOMNodeList*		pChildNodeList;
	
	MSXML::IXMLDOMNode*			pNode;
	MSXML::IXMLDOMNode*			pParentNode;
	MSXML::IXMLDOMNode*			pOutNode;
	
	CGCMLGenerator generator;
	LPTSTR	pName = NULL;
	INT32 nTotalNodes=0;
	
	if(GetTotalNodesOfType(pNodeName,pNodeList,&nTotalNodes) == SUCCESS)
	{
		while(nTotalNodes)
		{
			GetNextNode(pNodeList,pNode);
			GetAllChildren(pNode,pChildNodeList);
			long iListSize = GetListSize(pChildNodeList);
			while(iListSize)
			{	
				GetNextNode(pChildNodeList,pNode);
				GetParent(pNode,pParentNode);
				RemoveChild(pParentNode,pNode,pOutNode);
				RELEASE_OBJECT(pOutNode);
				iListSize --;
			}
			nTotalNodes --;
		}

		return SUCCESS;
	}
	else
		return FAILURE;
}

/****************************************************************************************

	FUNCTION NAME		: AddBootupMsgIdinList

  	PURPOSE				: Add newly prepared bootup message sequence list in the DOM

	INPUT PARAMETERS	: CList <UINT32, UINT32>&

	OUTPUT PARAMETERS	: ERR

	RETURNS				: ERR

	REMARKS				: This function will add the newlt prepared message sequence list 
						  in the DOM

*****************************************************************************************/


ERR  CNWGCMLHandler::AddBootupMsgIdinList(CList <UINT32, UINT32>& MsgList,LPTSTR pNodeName)
{
  MSXML::IXMLDOMNodeList*		pNodeList;
	ERR	err	=	FAILURE;
	MSXML::IXMLDOMNode*			pNode;
	MSXML::IXMLDOMNode*			pNewNode;
	MSXML::IXMLDOMNode*			pOutNode;
	UINT16	iCount	=	0;
	INT32	nTotalNodes			=	0;
	INT32   MessageId;
	CDataList*	pDataFromDOM	=	NULL;
	CPtrList*	pPtrList	=	NULL;
	CGCMLGenerator generator;

	if(GetTotalNodesOfType(pNodeName,pNodeList,&nTotalNodes) == SUCCESS)
	{
		while(nTotalNodes)
		{
			pNodeList->nextNode(&pNode);
			
			for( POSITION pos=MsgList.GetHeadPosition();pos!=NULL;)
			{
				MessageId = (UINT32) MsgList.GetNext(pos);
				if((err = generator.GetNewMsgIdNode(MessageId,pNewNode)) == SUCCESS)
				{
					AddChild(pNode,(MSXML::IXMLDOMElement*)pNewNode,pOutNode);
					RELEASE_OBJECT(pNewNode);
					RELEASE_OBJECT(pOutNode);
					err = SUCCESS;
					//enable this to generate the xml file
					//break;
				}
			}
			generator.MakeGCML(this->m_pFileName);
			RELEASE_OBJECT(pNode);
		--nTotalNodes;			
		}

		return SUCCESS;

	}
	else
  {
		return FAILURE;
  }
}





BOOL CNWGCMLHandler::UpDateDataListInDom(CDataList dataList,CData& changedDataSet)
{	
	return(UpdateDataList(dataList,changedDataSet));
}


//have to add the newly update data list in DOM
ERR  CNWGCMLHandler::UpdateDataList(CDataList dataList,CData& changedDataSet)
{
    MSXML::IXMLDOMNodeList*		pNodeList;
    MSXML::IXMLDOMNodeList*		pChildNodeList;
	ERR		err		=	FAILURE;
	MSXML::IXMLDOMNode*			pNode;
	MSXML::IXMLDOMNode*			pChildNode;
	MSXML::IXMLDOMNamedNodeMap*	pNodeMap	=	NULL;
	CGCMLGenerator generator;
	LPTSTR	pName = NULL;
	INT32 nTotalNodes=0;
	if(GetTotalNodesOfType(TAG_DATALIST,pNodeList,&nTotalNodes) == SUCCESS)
	{
		//find the data list and remove the data list from the DOM
		//now add the new data list having edited data set
		//UpdateDataListNode
		while((nTotalNodes)&&(err == FAILURE))
		{
			GetNextNode(pNodeList,pNode);
			pNodeMap = GetAllAttributes(pNode);
			GetAttributeValue(pNodeMap,TAG_NAME,pName);
			//check the data list match if yes then update the data list as well as data set
			if(!dataList.GetName().CompareNoCase(pName))
			{
				//matched data list name
				if(generator.UpdateDataListNode(dataList,pNodeMap) == SUCCESS)
				{
					err = SUCCESS;
					//continure as the data list is update
					//get all the child and look for data set to be updated changedDataSet
					if(GetAllChildren(pNode,pChildNodeList) == SUCCESS)
					{
						long iListSize = GetListSize(pChildNodeList);
						while(iListSize)
						{
							GetNextNode(pChildNodeList,pChildNode);
							pNodeMap = GetAllAttributes(pChildNode);
							GetAttributeValue(pNodeMap,TAG_NAME,pName);
							if(!changedDataSet.GetName().CompareNoCase(pName))
							{
								//get the data details
								//now update this data set
								if(generator.UpdateDataSetNode(changedDataSet,pNodeMap)==SUCCESS)
								{
									err = SUCCESS;
									CList<CValue,CValue&>& valueList= changedDataSet.GetValueList();
									POSITION valpos = valueList.GetHeadPosition();
									//matched the name										
										if(GetAllChildren(pChildNode,pChildNodeList) == SUCCESS)
										{
											iListSize = GetListSize(pChildNodeList);
											while((iListSize)&& (valpos != NULL))
											{
												//now update the values from the new value list in dom
												CValue& valueObj = (CValue&)(valueList.GetNext( valpos ));
												GetNextNode(pChildNodeList,pChildNode);
												pNodeMap = GetAllAttributes(pChildNode);
												GetAttributeValue(pNodeMap,TAG_NAME,pName);
												GetAttributeValue(pNodeMap,TAG_SOURCE,pName);
												if(generator.UpdateValueNode(valueObj,pNodeMap)== SUCCESS)
													err = SUCCESS;									
											}
									}
								}
								generator.MakeGCML(this->m_pFileName);
									break;								
							}								
							RELEASE_OBJECT(pChildNode);
							--iListSize;
						}
						RELEASE_OBJECT(pChildNodeList);
					}
				}
			}//if data list name are matched
			--nTotalNodes;
		}//while total number of nodes > 1
			RELEASE_OBJECT(pNodeList);
			RELEASE_OBJECT(pNode);
			RELEASE_OBJECT(pNodeMap);
	}
	return err;
}

BOOL CNWGCMLHandler::UpDateDefaultDataSetInDom(CDataList dataList,CData& changedDataSet)
{
	return(UpdateDefaultDataSet(dataList,changedDataSet));
}

//for updating the default dataset of a datalist in GCML
ERR  CNWGCMLHandler::UpdateDefaultDataSet(CDataList dataList,CData& changedDataSet)
{
    MSXML::IXMLDOMNodeList*		pNodeList;
    //MSXML::IXMLDOMNodeList*		pChildNodeList;
	ERR		err		=	FAILURE;
	MSXML::IXMLDOMNode*			pNode;
	//MSXML::IXMLDOMNode*			pChildNode;
	MSXML::IXMLDOMNamedNodeMap*	pNodeMap	=	NULL;
	CGCMLGenerator generator;
	LPTSTR	pName = NULL;
	INT32 nTotalNodes=0;
	if(GetTotalNodesOfType(TAG_DATALIST,pNodeList,&nTotalNodes) == SUCCESS)
	{
		//find the data list and remove the data list from the DOM
		//now add the new data list having edited data set
		//UpdateDataListNode
		while((nTotalNodes)&&(err == FAILURE))
		{
			GetNextNode(pNodeList,pNode);
			pNodeMap = GetAllAttributes(pNode);
			GetAttributeValue(pNodeMap,TAG_NAME,pName);
			//check the data list match if yes then update the data list as well as data set
			if(!dataList.GetName().CompareNoCase(pName))
			{
				//matched data list name
				if(generator.UpdateDataListNode(dataList,pNodeMap) == SUCCESS)
				{
					err = SUCCESS;
					break;
				}
			}
			--nTotalNodes;
		}
	}
	return err;
}

BOOL CNWGCMLHandler::UpDateInputModeInDom(CFormat& format)
{	
	return(UpdateFormat(format));
}

ERR CNWGCMLHandler::UpdateFormat(CFormat& format)
{
	MSXML::IXMLDOMNodeList*		pNodeList;
    ERR		err		=	FAILURE;
	MSXML::IXMLDOMNode*			pNode;
	MSXML::IXMLDOMNamedNodeMap*	pNodeMap	=	NULL;
	CGCMLGenerator generator;
	LPTSTR	pName = NULL;
	INT32 nTotalNodes=0;
	//update the input mode of format in dOM
	if(GetTotalNodesOfType(TAG_FORMATS,pNodeList,&nTotalNodes) == SUCCESS)
	{
		//find the format and update the input mode.
		while(nTotalNodes)
		{
			GetNextNode(pNodeList,pNode);
			pNodeMap = GetAllAttributes(pNode);
			GetAttributeValue(pNodeMap,TAG_NAME,pName);
			//check the data list match if yes then update the data list as well as data set
			if(!format.GetName().CompareNoCase(pName))
			{
				//matched data list name
				if(generator.UpdateFormatNode(format,pNodeMap) == SUCCESS)
				{
					err = SUCCESS;
					//generator.MakeGCML();
					break;
				}				
			}
			--nTotalNodes;
		}
		RELEASE_OBJECT(pNodeList)
		RELEASE_OBJECT(pNodeMap)
		RELEASE_OBJECT(pNode)
	}
	return err;
}

BOOL CNWGCMLHandler::UpDateResponseModeInDom(CMessage msg)
{	
	return(UpdateResponseMode(msg));
}

ERR CNWGCMLHandler::UpdateResponseMode(CMessage msg)
{
	MSXML::IXMLDOMNodeList*		pNodeList;
    ERR		err		=	FAILURE;
	MSXML::IXMLDOMNode*			pNode;
	MSXML::IXMLDOMNamedNodeMap*	pNodeMap	=	NULL;
	CGCMLGenerator generator;
	LPTSTR	pName = NULL;
	INT32 nTotalNodes=0;
	//update the input mode of format in dOM
	if(GetTotalNodesOfType(TAG_INCOMINGMSG,pNodeList,&nTotalNodes) == SUCCESS)
	{
		//find the format and update the input mode.
		while(nTotalNodes)
		{
			GetNextNode(pNodeList,pNode);
			pNodeMap = GetAllAttributes(pNode);
			GetAttributeValue(pNodeMap,TAG_NAME,pName);
			//check the data list match if yes then update the data list as well as data set
			if(!msg.GetMessageName().CompareNoCase(pName))
			{
				//matched data list name
				if(generator.UpdateIncomingMessageNode(msg,pNodeMap) == SUCCESS)
				{
					err = SUCCESS;
					generator.MakeGCML(this->m_pFileName);
					break;
				}				
			}
			--nTotalNodes;
		}
		RELEASE_OBJECT(pNodeList)
		RELEASE_OBJECT(pNodeMap)
		RELEASE_OBJECT(pNode)
	}
	return err;
}
