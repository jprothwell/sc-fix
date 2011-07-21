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

/*******************************************************************************
 * Filename:
 * ---------
 *  XMLVerifier.h
 *
 * Project:
 * --------
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 *------------------------------------------------------------------------------
 * 
 *
 *------------------------------------------------------------------------------
 *==============================================================================
 *******************************************************************************/

// XMLVerifier.h: interface for the CXMLVerifier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLVERIFIER_H__C87FABE5_5F37_4618_8989_EBE53E4D7204__INCLUDED_)
#define AFX_XMLVERIFIER_H__C87FABE5_5F37_4618_8989_EBE53E4D7204__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MessageManager.h"

class CXMLVerifier  
{
public:
	CXMLVerifier();
	virtual ~CXMLVerifier();

	CMessageManager *m_MMObj;

	int VerifyLists();
	int VerifyFormatList();	
	int VerifyOutGoingMsg(CPtrList *plstOutGoingMsg);
	int VerifyIncomingMsg(CPtrList *plstIncomingMsg);
	int VerifyAsyncMsg(CPtrList *plstAsyncMsg);
	int VerifyAppList();
	int VerifyBootupList(CPtrList* pBootupList);	
	int VerifyFreqList();
	int VerifyDataList();
	
	
	
	//--------------------- Additions for NWSimulator 2.5 -----------------------------------------
	int CheckMessageReplication(UINT32 iMsgId, CPtrList *msgList);
	int CheckOneFmtMultiMsgAssociativity(CString format, CString&);
	int CheckFormatReplication(CString FormatName);
	int CheckDataListRedefination(CString DatalistName);
	int CheckForRedefinationofDataset(CString DatalistName, CString DatasetName);
	int CrossCheckFormatAndDataset();
	int FoundAndCheckedForReplication(CList <UINT32,UINT32&> &CheckedMsgList, UINT32 MsgNo);
	int IsAllreadyCheckedForReplication(CList <UINT32,UINT32&> &CheckedMsgList, UINT32 MsgNo);


	int  IsAllreadyCheckedForReplication(CList <CString,CString&> &CheckedList, CString Check);
	void FoundAndCheckedForReplication(CList <CString,CString&> &CheckedList, CString Checked);
	int IsAnAsyncMessage(UINT32 MessageId);


	//---------------------------------------------------------------------------------------------

	
	 CString m_XMLErrors;
	 CStdioFile m_fErrorFile;
	 int AppIdErrorPrintedStatus;
	 CString AppIdError[MAX_APPLICATIONS]; 

};

#endif // !defined(AFX_XMLVERIFIER_H__C87FABE5_5F37_4618_8989_EBE53E4D7204__INCLUDED_)
