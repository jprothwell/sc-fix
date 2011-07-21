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

	FILENAME	: NSWTHread.cpp

  	PURPOSE		: File to raed data from Stack queue

	REMARKS		: nil

	AUTHOR		:  Kuldeep

	DATE		: Oct 5,03

**************************************************************/

#include <time.h>
#include "stdafx.h"


//#include "MessageStruct.h"
#include "NSRThread.h"
#include "ProtocolEvents.h"
#include "MMI_Data_Types.h"
#include "windows.h"
#include "NSWThread.h"


#define SERV_HOST_ADDR	"192.168.0.203"
#define SERV_HOST_ADDR_DEFAULT	"127.0.0.1"
#define SERV_TCP_PORT_NW	8001

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNSRThread* myThread;
//extern CMMINWSimulatorApp theApp;

NWRECEIVEQ CNSWThread::m_pReceiveQ;
oslMsgqid CNSWThread::m_QHandle;

/////////////////////////////////////////////////////////////////////////////
// CNSWThread

IMPLEMENT_DYNCREATE(CNSWThread, CWinThread)

/*CNSWThread::CNSWThread(CWnd* UIHandler, CCallHandle *callhandle)
{
	mySockfd = 0;
	m_ScreenHandle = UIHandler;
	callHandle = callhandle;
	reset_all_cost = false;
}*/
CNSWThread::CNSWThread()
{
	m_bExit = FALSE;
  m_hWindow = NULL;
//	m_SocketId = 0;	
}

CNSWThread::~CNSWThread()
{
}

BOOL CNSWThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here	
	/*m_cNSWSocket = new CNWSocket();
	if(m_cNSWSocket !=NULL)
	{
		if(!m_cNSWSocket->Create(SERV_TCP_PORT_NW,SOCK_STREAM,(LPCTSTR)SERV_HOST_ADDR))
		{
			AfxMessageBox(_T("Socket creation Failure"));		
			return FALSE;
		}		
		if ((m_cNSWSocket->m_hSocket > 0) && (m_cNSWSocket->Connect((LPCTSTR)SERV_HOST_ADDR, SERV_TCP_PORT_NW)))
			return TRUE;
			AfxMessageBox(_T("Socket creation fails"));
		 return FALSE;
	}*/
		return TRUE;
}

int CNSWThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNSWThread, CWinThread)
	//{{AFX_MSG_MAP(CNSWThread)	
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/**************************************************************

	FUNCTION NAME		: ReadEventFromMMI

  	PURPOSE				: Read messages from the MMI and delegates to other functions

	INPUT PARAMETERS	: WPARAM wParam, LPARAM lParam

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 

**************************************************************/
 

void CNSWThread::ReadEventFromMMI(WPARAM wParam, LPARAM lParam)
{
  MYQUEUE1 msg;
  MYQUEUE mmiMsg;
  //	NWSSOCKREAD toRead;
  //	void *nDataPtr;
  static buffer[20];
  INT nWrittenData = 1;
  unsigned short iTotalBytesRead	=	0;
  
  if(m_pReceiveQ && m_QHandle)
  {
    m_pReceiveQ(m_QHandle,&mmiMsg );
    if(mmiMsg.oslMsgId != 10000)
    {
      msg.nEventType = (short)mmiMsg.oslSrcId;
      
      msg.nMsgType = (short)mmiMsg.oslMsgId;
      msg.size = 100;
      msg.nDataPtr =mmiMsg.oslDataPtr; 			
      if(msg.nMsgType == 0)
      {
        if(msg.nDataPtr)
          free((((char*)msg.nDataPtr)-4));
      }
      else
      {
        m_Msg = msg;
        if (m_hWindow)
        {
          SendMessage(m_hWindow,WM_SEND_MSG_SIM,0,mmiMsg.oslDestId);
        }
      }
    }
    else
    {
      m_bExit = TRUE;
    }
  }
  //PostMessage(m_hWindow,WM_SEND_MSG_SIM,0,0);			
  //theApp.dlg.DisplayMessageInApp(&msg);
		
}

/**************************************************************

	FUNCTION NAME		: Run() 

  	PURPOSE				: Function contains while loop 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 

**************************************************************/
int CNSWThread::Run() 
{
	// TODO: Add your specialized code here and/or call the base class	
	while(1)
	{
		ReadEventFromMMI(0,0);
		if(m_bExit)
			break;

	}
	return 0;
	//return CWinThread::Run();
}

void CNSWThread::SetWindowHandle(HWND pHwnd)
{
	if(pHwnd)
		m_hWindow = pHwnd;	
}

MYQUEUE1 CNSWThread::GetMessage()
{
	return m_Msg;
}