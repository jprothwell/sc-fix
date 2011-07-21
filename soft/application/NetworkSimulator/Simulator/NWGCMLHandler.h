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

	FILENAME	: NWGCMLHandler.h

  	PURPOSE		: Declaration file.

	REMARKS		: nil

	AUTHOR		: Vikram

	DATE		: Aug 5,03

**************************************************************/

#if !defined(AFX_NWGCMLHANDLER_H__284A478C_CA11_4110_B51D_C2B56776AA2E__INCLUDED_)
#define AFX_NWGCMLHANDLER_H__284A478C_CA11_4110_B51D_C2B56776AA2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DCMLHandler.h"
#include "IncomingMsg.h"
#include "OutgoingMessage.h"

#include "AsyncMessageSeq.h"
#include "GCMLDefines.h"
#include "Format.h"
#include "Data.h"
#include "Application.h"
#include "BootUpMsg.h"
#include "FreqUsedMsg.h"

class CNWGCMLHandler  : public CDCMLHandler
{
public:
	CNWGCMLHandler();
	CNWGCMLHandler(LPTSTR pFileName);
	virtual ~CNWGCMLHandler();
	CPtrList* GetIncomingMessageList(LPTSTR pNodeName);
	
	CPtrList* GetOutGoingMessageList(LPTSTR pNodeName);
	CPtrList* GetFormatsList();
	CPtrList* GetDataList();
	CPtrList* GetAsyncMessageList(LPTSTR pNodeName);
	CPtrList* GetApplicationList();
	CPtrList* GetBootUpMessageList(LPTSTR pNodeName);
	CPtrList* GetFrequentlyUsedMessageList();
	static void FreeAll();
	BOOL SetDataSetInDom(CData& dataSet,CDataList dataList);
	BOOL UpDateDataListInDom(CDataList dataList,CData& changedDataSet);
	BOOL UpDateInputModeInDom(CFormat& format);
	BOOL UpDateResponseModeInDom(CMessage msg);
	BOOL UpDateDefaultDataSetInDom(CDataList dataList,CData& changedDataSet);

	BOOL UpdateBootupMsgListInDom(CList <UINT32, UINT32>& MsgList,LPTSTR pNodeName);
	ERR  RemoveBootupListEntries(LPTSTR pNodeName);
	ERR  AddBootupMsgIdinList(CList <UINT32, UINT32>& MsgList,LPTSTR pNodeName);

private:
	ERR GetMessageDetails(MSXML::IXMLDOMNode* pNode, CMessage& message);
	ERR GetIncomingMessageDetails(MSXML::IXMLDOMNode* pNode, CIncomingMsg& message);
	ERR GetOutgoingMessageDetails(MSXML::IXMLDOMNode* pNode, COutgoingMessage& message);

	ERR GetFormatDetails(MSXML::IXMLDOMNode* pNode, CFormat& format,CPtrList* pDataList,CPtrList* pDataTypeList);
	ERR GetDataDetails(MSXML::IXMLDOMNode* pNode, CData& data);
	ERR GetDataListDetails(MSXML::IXMLDOMNode* pNode, CDataList& data);
	ERR GetAsyncMessageDetails(MSXML::IXMLDOMNode* pNode, CAsyncMessageSeq& message);
	ERR GetBootUpMessageDetails(MSXML::IXMLDOMNode* pNode, CBootUpMsg& message);
	ERR GetFrequentlyUsedMessageDetails(MSXML::IXMLDOMNode* pNode, CFreqUsedMsg& message);
	CPtrList* GetDataTypeList();
	ERR GetDataSetToAddFromDataList(CData& dataNew,CDataList newDataList,CDataList *dataListFromDOM);
	ERR AddDataSetInDataList(CData& dataSet,CDataList datalist);
	ERR UpdateDataList(CDataList dataList,CData& changedDataSet);
	ERR UpdateFormat(CFormat& format);
	ERR UpdateResponseMode(CMessage msg);
	ERR UpdateDefaultDataSet(CDataList dataList, CData& changedDataSet);

};
#endif // !defined(AFX_NWGCMLHANDLER_H__284A478C_CA11_4110_B51D_C2B56776AA2E__INCLUDED_)
