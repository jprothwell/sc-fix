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

	FILENAME	: GCMLGenerator.h

  	PURPOSE		: This is the header file for GCML generator. This class is used 
					to generate GCML whenever required.

	REMARKS		: nil

	AUTHOR		: Kuldeep

	DATE		: Nov 10,04

**************************************************************/

#if !defined(AFX_GCMLGENERATOR_H__1099DFD7_8541_4579_BC48_FDBAA94AA920__INCLUDED_)
#define AFX_GCMLGENERATOR_H__1099DFD7_8541_4579_BC48_FDBAA94AA920__INCLUDED_

#include "DCMLHandler.h"
#include "DataList.h"
#include "Format.h"
#include "Message.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CGCMLGenerator  
{
public:
	static void Free();
	void SetDOM(MSXML::IXMLDOMDocument *pDom);
	CGCMLGenerator();
	virtual ~CGCMLGenerator();	
	ERR MakeGCML(LPTSTR pFileName);
	ERR GetNewDataNode(CData& data,MSXML::IXMLDOMNode*& pNode );
	ERR UpdateDataListNode(CDataList dataList,MSXML::IXMLDOMNamedNodeMap* pNodeMap);
	ERR UpdateDataSetNode(CData& dataSet,MSXML::IXMLDOMNamedNodeMap* pNodeMap);
	ERR UpdateFormatNode(CFormat& format,MSXML::IXMLDOMNamedNodeMap* pNodeMap);
	ERR UpdateValueNode(CValue value,MSXML::IXMLDOMNamedNodeMap* pNodeMap);
	ERR UpdateIncomingMessageNode(CMessage msg,MSXML::IXMLDOMNamedNodeMap* pNodeMap);
	ERR GetNewMsgIdNode(UINT32 MsgId,MSXML::IXMLDOMNode*& pNode );
	ERR CreateMessageIdNode(UINT32 MsgId, MSXML::IXMLDOMElement*& pNewNode);

	

private:
	ERR GetMultiBytechar(WCHAR* pString,BYTE*& ptcMultiByte,UINT32* pBytes);
	ERR CreateValueNode(CValue& value, MSXML::IXMLDOMElement*& pNewNode);
	ERR CreateDataNode(CData& data, MSXML::IXMLDOMElement*& pNewNode);
	ERR EditDataSetNode(CData& dataSet,MSXML::IXMLDOMNamedNodeMap* pNodeMap);
	ERR EditDataListNode(CDataList dataList,MSXML::IXMLDOMNamedNodeMap* pNodeMap);
	ERR EditFormatNode(CFormat& format,MSXML::IXMLDOMNamedNodeMap* pNodeMap);
	ERR EditValueNode(CValue value, MSXML::IXMLDOMNamedNodeMap* pNodeMap);
	ERR EditIncomingMessageNode(CMessage msg,MSXML::IXMLDOMNamedNodeMap* pNodeMap);
private:
	CDCMLHandler m_DCMLObj;

};

#endif // !defined(AFX_GCMLGENERATOR_H__1099DFD7_8541_4579_BC48_FDBAA94AA920__INCLUDED_)
