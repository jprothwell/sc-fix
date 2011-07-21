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

	FILENAME	: IncomingMsg.cpp

  	PURPOSE		: Class for Incoming messages from XML

	REMARKS		: nil

	AUTHOR		:  Vikram

	DATE		: Oct 5,03

**************************************************************/



#include "stdafx.h"
#include "IncomingMsg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIncomingMsg::CIncomingMsg():CMessage()
{

}

CIncomingMsg::~CIncomingMsg()
{

}
/**************************************************************

	FUNCTION NAME		: AddResponseMessageId

  	PURPOSE				: Used to add the response message id for an incoming message

	INPUT PARAMETERS	: UINT32
				
	OUTPUT PARAMETERS	: nil

	RETURNS				: void
	REMARKS				: Adds a Response message id to the response message.
				

**************************************************************/

void CIncomingMsg::AddResponseMessageId(UINT32 nId)
{
	m_RespMsgList.AddTail(nId);
}

/**************************************************************

	FUNCTION NAME		: GetResponseMessageList

  	PURPOSE				: Returns the Response message list for the incoming message

	INPUT PARAMETERS	: UINT32
				
	OUTPUT PARAMETERS	: nil

	RETURNS				: CList<UINT32, UINT32>&

	REMARKS				: Returns the response message ids for the incoming message id.
				

**************************************************************/

CList<UINT32, UINT32>& CIncomingMsg::GetResponseMessageList()
{
	return m_RespMsgList;
}

/**************************************************************

	FUNCTION NAME		: AddAsyncSeqId

  	PURPOSE				: Used to add the Async message Sequence id for an incoming message

	INPUT PARAMETERS	: UINT32
				
	OUTPUT PARAMETERS	: nil

	RETURNS				: void
	REMARKS				: Adds a Async Messages sequence id to the incoming message.
				

**************************************************************/

void CIncomingMsg::AddAsyncSeqId(UINT32 nId)
{
	m_AsyncMsgList.AddTail(nId);
}

/**************************************************************

	FUNCTION NAME		: GetAsyncMessageList

  	PURPOSE				: Returns the Async Sequence message Id List for the incoming message

	INPUT PARAMETERS	: UINT32
				
	OUTPUT PARAMETERS	: nil

	RETURNS				: CList<UINT32, UINT32>&

	REMARKS				: Returns the Async Message Sequence ids for the incoming message id.
				

**************************************************************/

CList<UINT32, UINT32>& CIncomingMsg::GetAsyncMessageList()
{
	return m_AsyncMsgList;
}
