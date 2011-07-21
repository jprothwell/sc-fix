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

#include "Message.h"
#include "Format.h"
#include "Data.h"
#include "DataList.h"
#include "DataType.h"
#include "List.h"
#include "SimAppToolkit.h"
#include "GCMLGenerator.h"
#pragma once
#define MAX_APPLICATIONS 50

struct SendMessageDetails
{
	CMessage *clMessage;
	TCHAR * strDataList ;
	UINT32 Freq, Timer;
	UINT32 TimerId;
	CData* dataset;
};
class CMessageManager
{
public:
	CMessageManager(void);
	~CMessageManager(void);
	CFormat * GetFormatForMessage(CMessage*);
	void* GetBufferForSocket(CMessage *pMessage,CData& dataObj,CList<CVar,CVar&>& varList,UINT32 *iSize);
	void GetBufferFromIncomingMessage(void *pBuffer,int nIndex);
	CPtrList *m_IncomingMsgDataList;
	int m_inComingMsgSelIndex;
	CMessage* LocateMessageInList(CPtrList *msgList,UINT32 iMsgId);
	UINT32 LocateMessageIdForName(CPtrList *msgList,CString MsgName);
	UINT32 GetTotalSizeOfVariable(CList<CVar,CVar&>& varList);
	CString LocateMessageNameInList(CPtrList *msgList,UINT16 iMsgId);
	CList<CString,CString&>* GetMessageListForApp(CString AppName);
	void SaveDataSetValues(UINT32 MsgId, CData& dataset);
	int GetPluginStatusFromMsgId(UINT32 MsgId); 
	//------------------------ Gurdev Additions --------------------------------------
	void	GetDataSetDetails(UINT32, CString, CData&);
	CString GetDefaultDataSetForMessage(UINT32);
	UINT32	GetTimer(UINT32,CString);
	UINT32	GetFreq (UINT32,CString);
	
	CString GetDefaultDataSetForMessage(CString);
	UINT32	GetTimer(CString,CString);
	UINT32	GetFreq (CString,CString);
	
	UINT32 GetMsgIdFromMsgName(CString);
  void SetDataSetDetails(UINT32 MsgId,CString sDataSet,CString sDefaultDataSet,CString sInputMode, UINT32 Freq, UINT32 Timer);
  void SaveMessageAttributes(UINT32 MsgId,CString DefDSName, CString InputMode);
  static void ReleaseXMLLists();
	static BOOL InitializeXMLLists(CString strMessageFile);
	static int VerifyLists();
	static int VerifyFormatList();	
	static int VerifyOutGoingMsg();
	static int VerifyIncomingMsg();
	static int VerifyAsyncMsg();
	static int VerifyAppList();		
	static int VerifyBootupList();	
	static int VerifyFreqList();
	static int VerifyDataList();
	static CString LongToCString(long int number);
	static char* UnicodeToAscii(CString UnicodeCString);

	static CString m_XMLErrors;
	static CStdioFile m_fErrorFile;
	static int AppIdErrorPrintedStatus;
	static CString AppIdError[MAX_APPLICATIONS]; //

public:
	
	static CPtrList *m_pFormatList, *m_plstOutGoingMsg, *m_plstIncomingMsg;//global list of messages
	static CPtrList *m_plstAsyncMsg,*m_pAppList,*m_pBootupList,*m_pFreqList;
	static CPtrList *m_pDataList;
	static CPtrList * m_TimerList;

// Add by zhuoxz, 2009-09-28
#ifdef _DUAL_SIM_
  static CPtrList *m_plstOutGoingMsg2, *m_plstIncomingMsg2;//global list of messages
  static CPtrList *m_plstAsyncMsg2,*m_pBootupList2;
#endif


// Add by zhuoxz,2008-09-01
public:
  static void SetFilePath(CString strFilePath);
  static CString GetFilePath();

// Add by zhuoxz,2008-09-01
private:
  static CString m_strXMLFile;
	

};
