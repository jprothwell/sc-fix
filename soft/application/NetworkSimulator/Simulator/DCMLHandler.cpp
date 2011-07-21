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

	FILENAME	: DCMLHandler.cpp

  	PURPOSE		: This is the implementation file for the DCMLHandler class.
					It gives wrappers over the Microsoft XML handler.


**************************************************************/
#include "stdafx.h"
//#include <stdio.h>
#include <afx.h>
#include	"Globaldefines.h"
#include "DCMLHandler.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCMLHandler
MSXML::IXMLDOMDocument* CDCMLHandler::m_pXMLDomDocument = NULL;

CDCMLHandler::CDCMLHandler(LPTSTR pFileName)
{
	if(pFileName)
  {
		m_pFileName = new TCHAR[wcslen(pFileName)+1];
  }
	wcscpy(m_pFileName,pFileName);
}

CDCMLHandler::CDCMLHandler()
{
	m_pFileName = NULL;
}

CDCMLHandler::~CDCMLHandler()
{
	DELETE_MEM(m_pFileName)
}



/////////////////////////////////////////////////////////////////////////////
// CDCMLHandler message handlers

ERR CDCMLHandler::GetDOM(MSXML::IXMLDOMDocument *&pDomDoc)
{
	IObjectSafety*	pSafety = NULL;
	DWORD			dwSupported, dwEnabled;
	ERR				err		=	SUCCESS;
	VARIANT_BOOL	bSuccess= FALSE;

	if(m_pXMLDomDocument == NULL)
	{
		CoInitialize(NULL);
		HRESULT hr = CoCreateInstance (MSXML::CLSID_DOMDocument, NULL, 
					 CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, 
					 MSXML::IID_IXMLDOMDocument,reinterpret_cast<void**>(&pDomDoc));

	//	if(hr == REGDB_E_CLASSNOTREG)
	//		AfxMessageBox(_T("abc"));
		if(SUCCEEDED(hr) && pDomDoc)
		{
    		pDomDoc->put_async(VARIANT_FALSE);

			if (SUCCEEDED(pDomDoc->QueryInterface( IID_IObjectSafety,(void**)&pSafety ) ) )
			{
				 pSafety->GetInterfaceSafetyOptions(MSXML::IID_IXMLDOMDocument, &dwSupported, &dwEnabled );
 				 pSafety->SetInterfaceSafetyOptions(MSXML::IID_IXMLDOMDocument,	dwSupported, 0 );
			}

			pDomDoc->put_validateOnParse(VARIANT_FALSE);
			HANDLE handle = CreateFile(
										m_pFileName ,
										GENERIC_READ|GENERIC_WRITE,
										FILE_SHARE_READ,
										NULL,
										OPEN_EXISTING,
										FILE_ATTRIBUTE_NORMAL,
										0);
			if(handle)
			{
				ULONG	iBytesRead;
				BYTE*	pBuff = new byte[MAX_FILE_SIZE+1];
				if(ReadFile(handle,pBuff,MAX_FILE_SIZE,&iBytesRead,NULL))
				{
					pBuff[iBytesRead] = 0;
#ifndef __UCS2_ENCODING
					pDomDoc->loadXML(_bstr_t((char*)pBuff), &bSuccess );
#else
					pDomDoc->loadXML(_bstr_t((WCHAR*)pBuff), &bSuccess );

#endif
					m_pXMLDomDocument = pDomDoc;
					err = SUCCESS;

				}
				else
				{
					pDomDoc->Release();
					err = CD_NOT_OPENFILE;
				}
				CloseHandle(handle);
				DELETE_MEM_ARR(pBuff)
				

			}

/*

			CFile cFile(m_csFileName.GetBuffer(0),CFile::modeRead);
			DWORD dwFileSize = cFile.GetLength();
			BYTE*	pBuff = new byte[dwFileSize+1];
			cFile.Read(pBuff,dwFileSize);
			pBuff[dwFileSize] = 0;
			pDomDoc->loadXML(_bstr_t((char*)pBuff), &bSuccess );
			m_pXMLDomDocument = pDomDoc;
			cFile.Close();
			delete[] pBuff;
*/
			if(pSafety)
				pSafety->Release();

			
			if ( bSuccess == VARIANT_FALSE )
			{
				MSXML::IXMLDOMParseError*	pError = NULL;
				BSTR						bstrError;
				long						lLine;
				CString						LineNo;
				char*						dummy;

				if(err==SUCCESS)
				{
				pDomDoc->get_parseError(&pError);
				pError->get_line(&lLine);
				dummy = (char*) malloc(sizeof(char)*20);
				LineNo= CString(ltoa(lLine,dummy,10));
				free(dummy);
				pError->get_errorCode(&lLine);
				pError->get_reason(&bstrError);
				pError->Release();
				pDomDoc->Release();
				pDomDoc = NULL;
				}
				AfxMessageBox(CString("Syntax Error in XML Document\n")+CString("Line No ")+LineNo+CString(":")+bstrError);
				err = FAILURE;
			}

		}
		else
			err = FAILURE;
	}//if(m_pDom == NULL)
	else
		pDomDoc = m_pXMLDomDocument;
	return err;
}

ERR CDCMLHandler::ParseData(BYTE* pData, UINT32 nSize, MSXML::IXMLDOMDocument *&pDomDoc)
{
	IObjectSafety*	pSafety = NULL;
	DWORD			dwSupported, dwEnabled;
	ERR				err		=	SUCCESS;
	VARIANT_BOOL	bSuccess;

	HRESULT hr = CoCreateInstance (MSXML::CLSID_DOMDocument, NULL, 
					 CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, 
					 MSXML::IID_IXMLDOMDocument,reinterpret_cast<void**>(&pDomDoc));

	if(SUCCEEDED(hr) && pDomDoc)
	{
    	pDomDoc->put_async(VARIANT_FALSE);

		if (SUCCEEDED(pDomDoc->QueryInterface( IID_IObjectSafety,(void**)&pSafety ) ) )
		{
			 pSafety->GetInterfaceSafetyOptions(MSXML::IID_IXMLDOMDocument, &dwSupported, &dwEnabled );
 			 pSafety->SetInterfaceSafetyOptions(MSXML::IID_IXMLDOMDocument,	dwSupported, 0 );
		}

		pDomDoc->put_validateOnParse(VARIANT_FALSE);
		pData[nSize] = 0;
		pDomDoc->loadXML(_bstr_t((char*)pData), &bSuccess );
				err = SUCCESS;


		if(pSafety)
			pSafety->Release();

			
		if ( bSuccess == VARIANT_FALSE )
		{
			MSXML::IXMLDOMParseError*	pError = NULL;
			BSTR						bstrError;
			long						lLine;

			pDomDoc->get_parseError(&pError);
			pError->get_line(&lLine);
			pError->get_errorCode(&lLine);
			pError->get_reason(&bstrError);
			pError->Release();
			pDomDoc->Release();
			pDomDoc = NULL;
		//	AfxMessageBox(bstrError);
			err = FAILURE;
		}

	}
	else
		err = FAILURE;

	return err;
}

ERR CDCMLHandler::GetTotalNodesOfType(LPTSTR pNodeName,MSXML::IXMLDOMNodeList*& pNodeList,INT32* piLength )
{
	ERR		err		=	FAILURE;
	HRESULT	hr;

	if(m_pXMLDomDocument)
	{
		hr = m_pXMLDomDocument->getElementsByTagName(pNodeName, &pNodeList);
		if ( SUCCEEDED(hr) )
		{
			pNodeList->get_length((long*)piLength);
			err = SUCCESS;
		}

	}		
	return err;

}



LPTSTR CDCMLHandler::GetAttributeValue(MSXML::IXMLDOMNode *pNode, LPTSTR pAttribNodeName)
{
	TCHAR *pAttributeValue	= NULL;

	MSXML::IXMLDOMNode*	pAttributeNode = GetAttributeNode(pNode, pAttribNodeName);
	if(pAttributeNode)
	{
		VARIANT vtValue;
		HRESULT hr = pAttributeNode->get_nodeValue(&vtValue);
		_bstr_t bstr(vtValue.bstrVal);
		pAttributeValue = new TCHAR[bstr.length()+1];
		wcscpy(pAttributeValue,bstr.operator wchar_t *());
		SysFreeString(vtValue.bstrVal);
		pAttributeNode->Release();
	}

	return pAttributeValue;
}

ERR CDCMLHandler::RemoveChild(MSXML::IXMLDOMNode *pNode, MSXML::IXMLDOMNode *pChildNode,MSXML::IXMLDOMNode *&pOutNode)
{
	ERR			err		=	FAILURE;

	if(pNode->removeChild(pChildNode,&pOutNode) == S_OK)
		err = SUCCESS;

	return err;

}
ERR CDCMLHandler::SetAttributeValue(MSXML::IXMLDOMNamedNodeMap* pNodeMap, LPTSTR pAttribNodeName,LPTSTR pValue)
{
	ERR			err		=	FAILURE;
	HRESULT		hr;
	MSXML::IXMLDOMNode*	pAttributeNode;
	_bstr_t	bstrName(pAttribNodeName);
	
	pNodeMap->getNamedItem(bstrName, &pAttributeNode);

	if(pAttributeNode)
	{
		VARIANT	var;
		VariantInit(&var);
		var.vt = VT_BSTR;
		var.bstrVal = _bstr_t(pValue);
		hr = pAttributeNode->put_nodeValue(var);
		if(SUCCEEDED(hr))
			err = SUCCESS;
		RELEASE_OBJECT(pAttributeNode);
	}

	return err;
}

ERR CDCMLHandler::GetAttributeValue(MSXML::IXMLDOMNamedNodeMap*	pNodeMap, LPWSTR pAttribName,LPWSTR& pAttributeValue)
{
	ERR		err					=	SUCCESS;

	_bstr_t bstr(pAttribName);
	MSXML::IXMLDOMNode*	pAttributeNode;
	
	pNodeMap->getNamedItem(bstr, &pAttributeNode);
	if(pAttributeNode)
	{
		VARIANT vtValue;
		HRESULT hr = pAttributeNode->get_nodeValue(&vtValue);
		_bstr_t bstr(vtValue.bstrVal);
		pAttributeValue = new TCHAR[bstr.length()+1];
		wcscpy(pAttributeValue,bstr.operator wchar_t *());
		SysFreeString(vtValue.bstrVal);
		pAttributeNode->Release();

	}
	else
		err = FAILURE;
	return err;
}

ERR CDCMLHandler::GetFirstChild(MSXML::IXMLDOMNode *pNode,MSXML::IXMLDOMNode*& pChildNode)
{
	ERR		err		=	FAILURE;
	if(pNode->get_firstChild(&pChildNode) == S_OK)
		err = SUCCESS;
	return err;
}

ERR CDCMLHandler::GetNextSibling(MSXML::IXMLDOMNode *pNode,MSXML::IXMLDOMNode*& pSiblingNode)
{
	ERR		err		=	FAILURE;

	if(pNode->get_nextSibling(&pSiblingNode) == S_OK)
		err = SUCCESS;

	return err;
}

MSXML::IXMLDOMNode* CDCMLHandler::GetAttributeNode(MSXML::IXMLDOMNode *pNode, LPCTSTR pAttributeName)
{
	MSXML::IXMLDOMNode			*pAttribNode = NULL;
	MSXML::IXMLDOMNamedNodeMap	*pAttributes = NULL;

	_bstr_t bstr(pAttributeName);
	HRESULT hr1 = pNode->get_attributes(&pAttributes);
	long len;
	pAttributes->get_length(&len);
	HRESULT hr = pAttributes->getNamedItem(bstr, &pAttribNode);
	pAttributes->Release();

	return pAttribNode;
}

ERR CDCMLHandler::CreateNode(VARIANT nodeType,LPTSTR pNodeName, LPTSTR pNodeNameURI,
															MSXML::IXMLDOMNode* &pNewNode)
{
	ERR		err		=	FAILURE;
	if(m_pXMLDomDocument)
	{

		_bstr_t bstrName(pNodeName);
		_bstr_t bstrURI(pNodeNameURI);
		if(m_pXMLDomDocument->createNode(nodeType,pNodeName,pNodeNameURI,&pNewNode) == S_OK)
			err = SUCCESS;
	}

	return err;
}

ERR CDCMLHandler::CreateElement(LPTSTR pNodeName, MSXML::IXMLDOMElement* &pNewNode)
{
	ERR		err		=	FAILURE;

	if(m_pXMLDomDocument)
	{
		_bstr_t bstrName(pNodeName);
		if(m_pXMLDomDocument->createElement(bstrName,&pNewNode) == S_OK)
			err = SUCCESS;
	}

	return err;
}

ERR CDCMLHandler::AddAttribute(MSXML::IXMLDOMElement* pNode,LPTSTR pAttrName, VARIANT vAttrValue)
{
	ERR		err		=	FAILURE;
	if(pNode->setAttribute(pAttrName,vAttrValue) == S_OK)
		err = SUCCESS;

	return err;
}



ERR CDCMLHandler::AddNodeBefore(MSXML::IXMLDOMElement* pSourceNode, MSXML::IXMLDOMNode* pNode, VARIANT& vBefore, MSXML::IXMLDOMNode* &pNewNode)
{
	ERR		err		=	FAILURE;
	MSXML::IXMLDOMElement * pDOMElement = NULL;
	MSXML::IXMLDOMNode * pDOM = NULL;

	if(pNode->insertBefore(pSourceNode,vBefore,&pNewNode) == S_OK)
			err = SUCCESS;

	return err;
}

ERR CDCMLHandler::AddChild(MSXML::IXMLDOMNode* pNode, MSXML::IXMLDOMElement* pNewNode,MSXML::IXMLDOMNode* &pOutNode)
{
	ERR		err		=	FAILURE;

	if(pNode->appendChild(pNewNode,&pOutNode) == S_OK)
			err = SUCCESS;

	return err;
}

MSXML::IXMLDOMNamedNodeMap* CDCMLHandler::GetAllAttributes(MSXML::IXMLDOMNode *pNode)
{
	MSXML::IXMLDOMNamedNodeMap	*pAttributes = NULL;
	pNode->get_attributes(&pAttributes);

	return pAttributes;
}

LPTSTR CDCMLHandler::GetNodeName(MSXML::IXMLDOMNode *pNode)
{
	BSTR	bstr;
	TCHAR *pNodeName;

	pNode->get_nodeName(&bstr);
	_bstr_t bstrObj(bstr);

	pNodeName = new TCHAR[bstrObj.length()+1];
	wcscpy(pNodeName,bstrObj.operator wchar_t *());

	SysFreeString(bstr);
	return pNodeName;

}

long CDCMLHandler::GetListSize(MSXML::IXMLDOMNodeList *pNodeList)
{
	long		iLength;

	pNodeList->get_length((long*)&iLength);
	//pNodeList->Release();

	return iLength;
}

ERR CDCMLHandler::GetNextNode(MSXML::IXMLDOMNodeList *&pNodeList,MSXML::IXMLDOMNode* &pNode)
{
	ERR		err		=	SUCCESS;
	if(pNodeList->nextNode(&pNode) != S_OK)
		err = FAILURE;
//	pNodeList->Release();

	return err;
}

ERR CDCMLHandler::GetAllChildren(MSXML::IXMLDOMNode* pNode,MSXML::IXMLDOMNodeList* &pNodeList)
{
	ERR		err		=	SUCCESS;
	if(pNode->get_childNodes(&pNodeList) != S_OK)
		err = FAILURE;
	//pNode->Release();

	return err;
}

LPTSTR CDCMLHandler::GetXML()
{
	BSTR			bstrFile;
	TCHAR		*	pBuff	=	NULL;

	if(m_pXMLDomDocument)
	{

		HRESULT	hr = m_pXMLDomDocument->get_xml(&bstrFile);

		if ( SUCCEEDED(hr) )
		{
			_bstr_t bstrObj(bstrFile);

			pBuff = new TCHAR[bstrObj.length()+1];
			wcscpy(pBuff,bstrObj.operator wchar_t *());

			
		}
	}

	return pBuff;
}

ERR CDCMLHandler::GetParent(MSXML::IXMLDOMNode *pNode, MSXML::IXMLDOMNode *&pOutNode)
{
	ERR		err		=	SUCCESS;
	if(pNode->get_parentNode(&pOutNode) != S_OK)
		err = FAILURE;

	return err;
}

MSXML::IXMLDOMDocument* CDCMLHandler::GetDOMHandler()
{
	return m_pXMLDomDocument;
}

void CDCMLHandler::SetDOMHandler(MSXML::IXMLDOMDocument* pDomDocument)
{
	m_pXMLDomDocument = pDomDocument;
}

ERR	CDCMLHandler::CreateClone(MSXML::IXMLDOMNode* pNode,MSXML::IXMLDOMNode*& pOutNode)
{
	ERR		err		=	SUCCESS;

	if(pNode->cloneNode(VARIANT_TRUE,&pOutNode) != S_OK)
		err = FAILURE;

	return err;

}
