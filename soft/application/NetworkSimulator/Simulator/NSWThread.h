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

	FILENAME	: NSRThread.h

  	PURPOSE		: Declaration file.

	REMARKS		: nil

	AUTHOR		: Kuldeep

	DATE		: Aug 5,03

**************************************************************/

#if !defined(AFX_NSWTHREAD_H__627FC848_8D08_424A_9780_8CB9EBE6C97D__INCLUDED_)
#define AFX_NSWTHREAD_H__627FC848_8D08_424A_9780_8CB9EBE6C97D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NSWThread.h : header file
//

#include "NWSocket.h"
#include "GlobalDefines.h"
#include "..\SimulatorStart.h"	// Added by ClassView


/////////////////////////////////////////////////////////////////////////////
// CNSWThread thread

class CNSWThread : public CWinThread
{
	DECLARE_DYNCREATE(CNSWThread)
protected:
	           // protected constructor used by dynamic creation

// Attributes
public:
	//int m_SocketId;
	bool success;
	SOCKET m_SocketId;
	CNWSocket *m_cNSWSocket;
	HWND m_hWindow;
	MYQUEUE1 m_Msg; 
	BOOL	m_bExit;
//	CMessageAppManager m_MessageManager;
// Operations
public:
//	CNSWThread(CWnd* UIHandler, CCallHandle *callhandle);
	CNSWThread();
	static NWRECEIVEQ m_pReceiveQ;
	static oslMsgqid m_QHandle;
	void ReadEventFromMMI(WPARAM wParam, LPARAM lParam);
	void MapAndShowInGui(MYQUEUE1* Message);
	bool MessageEvents(MYQUEUE1* RcvMsg);	
	void SetWindowHandle(HWND pHwnd);
	MYQUEUE1 GetMessage();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNSWThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNSWThread();

	// Generated message map functions
	//{{AFX_MSG(CNSWThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CWnd* m_ScreenHandle;
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NSWTHREAD_H__627FC848_8D08_424A_9780_8CB9EBE6C97D__INCLUDED_)
