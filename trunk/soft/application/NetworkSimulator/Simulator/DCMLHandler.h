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

	FILENAME	: DCMLHandler.h

  	PURPOSE		: This is the header file for the DCMLHandler class. It gives 
					wrappers over the Microsoft XML handler



**************************************************************/


#if !defined(AFX_DCMLHANDLER_H__A4EF344D_8B2B_4195_826A_3B903E691BE3__INCLUDED_)
#define AFX_DCMLHANDLER_H__A4EF344D_8B2B_4195_826A_3B903E691BE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCMLHandler.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDCMLHandler window
#include <objsafe.h>
#include <afxtempl.h>		// MFC socket extensions
//#ifdef PAPP_WIN32
#import "msxml.dll" named_guids raw_interfaces_only 
//#endif
#include	"GlobalDefines.h"

namespace MSXML
{
#include <msxml.h>
}


class CDCMLHandler
{
public:
//app handler
	ERR GetDOM(MSXML::IXMLDOMDocument*& pDomDoc);
	ERR ParseData(BYTE* pData, UINT32 nSize, MSXML::IXMLDOMDocument *&pDomDoc);

	CDCMLHandler(LPTSTR csFileName);
	CDCMLHandler();
	virtual ~CDCMLHandler();

private:
	
	
	MSXML::IXMLDOMNode* GetAttributeNode(MSXML::IXMLDOMNode *pNode, LPCTSTR pAttributeName);
public:
	ERR GetTotalNodesOfType(LPTSTR pNodeName,MSXML::IXMLDOMNodeList*& pNodeList,INT32* piLength );
	ERR GetFirstChild(MSXML::IXMLDOMNode *pNode,MSXML::IXMLDOMNode*& pChildNode);
	ERR AddChild(MSXML::IXMLDOMNode* pNode, MSXML::IXMLDOMElement* pNewNode,MSXML::IXMLDOMNode* &pOutNode);
	ERR GetNextSibling(MSXML::IXMLDOMNode *pNode,MSXML::IXMLDOMNode*& pSiblingNode);
	LPTSTR GetNodeName(MSXML::IXMLDOMNode *pNode);
	ERR GetNextNode(MSXML::IXMLDOMNodeList *&pNodeList,MSXML::IXMLDOMNode* &pNode);
	MSXML::IXMLDOMNamedNodeMap* GetAllAttributes(MSXML::IXMLDOMNode *pNode);
	LPTSTR GetAttributeValue(MSXML::IXMLDOMNode *pNode, LPTSTR pAttributeName);
	long GetListSize(MSXML::IXMLDOMNodeList *pNodeList);
	ERR GetAllChildren(MSXML::IXMLDOMNode* pNode,MSXML::IXMLDOMNodeList* &pNodeList);
	ERR GetAttributeValue(MSXML::IXMLDOMNamedNodeMap*	pNodeMap, LPWSTR pAttribName,LPWSTR& pAttributeValue);
	ERR CreateNode(VARIANT nodeType,LPTSTR pNodeName, LPTSTR pNodeNameURI,
															MSXML::IXMLDOMNode* &pNewNode);
	ERR CreateElement(LPTSTR pNodeName, MSXML::IXMLDOMElement* &pNewNode);
	ERR AddAttribute(MSXML::IXMLDOMElement* pNode,LPTSTR pAttrName, VARIANT vAttrValue);

	ERR AddNodeBefore(MSXML::IXMLDOMElement* pSourceNode,MSXML::IXMLDOMNode* pNode, VARIANT& vBefore, MSXML::IXMLDOMNode*& pNewNode);
	ERR SetAttributeValue(MSXML::IXMLDOMNamedNodeMap* pNodeMap, LPTSTR pAttribNodeName,LPTSTR pValue);
	ERR RemoveChild(MSXML::IXMLDOMNode *pNode, MSXML::IXMLDOMNode *pChildNode,MSXML::IXMLDOMNode *&pOutNode);
	ERR GetParent(MSXML::IXMLDOMNode *pNode, MSXML::IXMLDOMNode *&pOutNode);
	ERR	CreateClone(MSXML::IXMLDOMNode* pNode,MSXML::IXMLDOMNode*& pOutNode);

	LPTSTR CDCMLHandler::GetXML();
	MSXML::IXMLDOMDocument* GetDOMHandler();
	void SetDOMHandler(MSXML::IXMLDOMDocument*);

public:
		static MSXML::IXMLDOMDocument*		m_pXMLDomDocument;

public:
	LPTSTR m_pFileName;
};


#endif // !defined(AFX_DCMLHANDLER_H__A4EF344D_8B2B_4195_826A_3B903E691BE3__INCLUDED_)
