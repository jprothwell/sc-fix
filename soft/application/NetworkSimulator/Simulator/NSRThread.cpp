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

	FILENAME	: NSRTHread.cpp

  	PURPOSE		: File to write data to MMI queue

	REMARKS		: nil

	AUTHOR		:  Kuldeep

	DATE		: Oct 5,03

**************************************************************/


#include "stdafx.h"
#include "NSRThread.h"
#include "MMI_Data_Types.h"
#include "ProtocolEvents.h"
#include "GlobalConstants.h"
#include "GlobalDefines.h"
#include "..\SimulatorStart.h"

#include "windows.h"

#define SERV_HOST_ADDR	_T("192.168.0.203")
#define SERV_HOST_ADDR_DEFAULT	_T("127.0.0.1")
#define SERV_TCP_PORT_NR	8000

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define AOC_TIMER 1000
/////////////////////////////////////////////////////////////////////////////
// CMyThread
NWWRITEQ CNSRThread::m_pWriteQ;
oslMsgqid CNSRThread::m_QHandle;

//extern void OslMsgSendExtQueue(MYQUEUE *msg);
 



IMPLEMENT_DYNCREATE(CNSRThread, CWinThread)

CNSRThread::CNSRThread()
{
	m_SocketId = 0;
}
CNSRThread::~CNSRThread()
{
}

void CNSRThread::SetAocValue(unsigned int aoc)
{
	m_aocValue = aoc;
}

BOOL CNSRThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here	
	/*m_cNSRSocket = new CNWSocket();
	if(m_cNSRSocket !=NULL)
	{
		if(!m_cNSRSocket->Create(SERV_TCP_PORT_NR,SOCK_STREAM,SERV_HOST_ADDR))
		{
			AfxMessageBox(_T("Socket creation Fails"));		
			return FALSE;
		}
		//m_cNSWSocket->CreateSocket();
		if ((m_cNSRSocket->m_hSocket > 0) && (m_cNSRSocket->Connect(SERV_HOST_ADDR, SERV_TCP_PORT_NR)))
			//AfxMessageBox(_T("Socket Error"));
		return TRUE;
			AfxMessageBox(_T("Socket creation fails"));
			//exit(0);exit if sockets fails
		 return FALSE;
	}*/
		return TRUE;
}

int CNSRThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNSRThread, CWinThread)
	//{{AFX_MSG_MAP(CNSRThread)
	ON_THREAD_MESSAGE(MMI_MSG, SendEventToMMI)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyThread message handlers

CWnd* CNSRThread::GetMainWnd() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinThread::GetMainWnd();
}

// This calls both the socket calls

		
int CNSRThread::Run()
{
	return CWinThread::Run();
}
 

/**************************************************************

	FUNCTION NAME		: WriteToSocket (MYQUEUE1 Message)

  	PURPOSE				: Function used for writing to the MMI queue

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 

**************************************************************/
void CNSRThread::WriteToSocket (MYQUEUE *Message)
{
  if (m_pWriteQ)
  {
	  MYQUEUE1 *stMsgWritten = (MYQUEUE1*)malloc(sizeof(MYQUEUE1));
	  m_pWriteQ(Message); //ÏòMMI·¢ËÍÄ£ÄâÃüÁî zhuoxz 2008-08-06
	  stMsgWritten->nEventType = (short)Message->oslSrcId;
	  stMsgWritten->nMsgType = (short)Message->oslMsgId;
	  stMsgWritten->size = 100;
	  stMsgWritten->nDataPtr = Message->oslDataPtr; 
	  SendMessage(m_hWindow,WM_WRITE_MSG_SIM,0,LPARAM(stMsgWritten));
    free(stMsgWritten);
  }
  else
  {
#ifdef _DEBUG
    MYQUEUE1 *stMsgWritten = (MYQUEUE1*)malloc(sizeof(MYQUEUE1));
    stMsgWritten->nEventType = (short)Message->oslSrcId;
	  stMsgWritten->nMsgType = (short)Message->oslMsgId;
	  stMsgWritten->size = 100;
	  stMsgWritten->nDataPtr = Message->oslDataPtr; 
    SendMessage(m_hWindow,WM_WRITE_MSG_SIM,0,LPARAM(stMsgWritten));
    free(stMsgWritten);
#endif
    TRACE0("m_pWriteQ == NULL");
  }

	
	return;
	
}


void CNSRThread::SetWindowHandle(HWND pHwnd)
{
	if(pHwnd)
		m_hWindow = pHwnd;	
}

MYQUEUE1 CNSRThread::GetMessage()
{
	return m_Msg;
}

/*WINTERTASKMSG CNSRThread::GetMessage()
{
	return m_taskMsg;
}*/

/**************************************************************

	FUNCTION NAME		: SendEventToMMI(WPARAM wParam, LPARAM lParam, void *message)

  	PURPOSE				: Function used for constructing messsage struct for writing on the socket.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 

**************************************************************/
 
void CNSRThread::SendEventToMMI(WPARAM wParam, LPARAM lParam/*, void *message*/)
{
	//NetInfo *netInfo = (NetInfo *)lParam;
//	MYQUEUE1 Message;
//	resultStruct *resp;	
//	CCM_VALUE ccm;
	//PRT_MESSAGE *TstMsg = (PRT_MESSAGE *)lParam;
	//PRT_MESSAGE PrtMsg;
	switch(wParam)
	{
		case HWR_EVENT:
			switch(lParam)
			{
				case PRT_CHARGERCONNECTED_EVENT:
					//Message.nEventType = HWR_EVENT;
					//Message.nMsgType = PRT_CHARGERCONNECTED_EVENT;
					//Message.size = 0;
					//Message.nDataPtr = NULL;
					break;
			}			
		case PRT_EVENT:
			switch(lParam)
			{
				case PRT_AOC_IND:	
					break;	
				default:
				AfxMessageBox(_T("Bad Msg Posted "));
				break;
			}			
	}			
//	WriteToSocket (Message);	
}