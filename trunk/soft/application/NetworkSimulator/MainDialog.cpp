// MainDialog.cpp : implementation file
//

#include "stdafx.h"
#include "NetworkSimulator.h"
#include "MainDialog.h"
#include "Dialog\SelectMessageDlg.h"
#include "Simulator\TaskInit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDialog dialog


CMainDialog::CMainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  m_bAllowClose = FALSE;
}


void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDialog)
	DDX_Control(pDX, IDC_LIST_OUTGOING_MESSAGES, m_lstOutGoingMessages);
	DDX_Control(pDX, IDC_LIST_INCOMING_MESSAGES, m_lstIncomingMessages);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
	//{{AFX_MSG_MAP(CMainDialog)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_MODIFY_BOOT_MSG, OnModifyBootMsg)
	ON_COMMAND(IDM_SEND_MESSAGES, OnSendMessages)
	ON_WM_NCDESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INCOMING_MESSAGES, OnDblclkListIncomingMessages)
	//}}AFX_MSG_MAP
  ON_MESSAGE(WM_SEND_MSG_SIM,OnMessageRecieve)
  ON_MESSAGE(WM_WRITE_MSG_SIM,OnMessageSend)
  ON_MESSAGE(WM_SEND_ASYNC_EVENT_MSG,OnSendAsyncMessage)
  ON_COMMAND_RANGE(ID_BUTTON_MSG1,ID_BUTTON_MSG10,OnSendMsgFromTBR)
  ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDialog message handlers

BOOL CMainDialog::InitSimulator(int nXPos,int nYPos)
{
  if(!CMainSimulator::InitSimulator())
  {
    CMainSimulator::ExitSimulator();
    return FALSE;
  }

  this->SetWindowPos(NULL,nXPos,nYPos,0,0,SWP_NOSIZE);
  this->ShowWindow(SW_SHOW);
  //this->UpdateWindow();

  return TRUE;
}

BOOL CMainDialog::Bootup()
{
  return this->SendBootupMessage();
}

void CMainDialog::AllowClose(BOOL bClose)
{
  this->m_bAllowClose = bClose;
}

int CMainDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CMainSimulator::SetListCtrl(&m_lstIncomingMessages,&m_lstOutGoingMessages);
  CMainSimulator::SetWnd(this);
  
	return 0;
}

BOOL CMainDialog::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
  int nScreenHieght = GetSystemMetrics(SM_CYSCREEN);

  cs.cx = nScreenWidth;
  cs.cy = nScreenHieght;
  if (nScreenWidth>600)
  {
    cs.x = (nScreenWidth-600)/2;
    cs.cx = 600;
  }
  if (nScreenHieght>400)
  {
    cs.y = (nScreenHieght-400)/2;
    cs.cy = 400;
  }
  
	return CDialog::PreCreateWindow(cs);
}

void CMainDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bAllowClose)
  {
    CDialog::OnClose();
  }
  else
  {
    this->ShowWindow(SW_HIDE);
  }
}

void CMainDialog::OnDestroy() 
{
  this->ExitSimulator();

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
  
}

void CMainDialog::OnModifyBootMsg() 
{
	// TODO: Add your command handler code here
	CMainSimulator::OnModifyBootMsg();
}

void CMainDialog::OnSendMessages() 
{
	// TODO: Add your command handler code here
	CMainSimulator::OnSendMessages();
}

LRESULT CMainDialog::OnMessageSend(WPARAM wParam, LPARAM lParam)
{
	//display it in the list now or call up the DCML query for gettting the mesages
  return CMainSimulator::OnSendMessage((MYQUEUE1*)lParam);
}


LRESULT CMainDialog::OnMessageRecieve(WPARAM wParam, LPARAM lParam)
{
  CMainSimulator::OnRecieveMessage(lParam);

  return 0;
	//call the message plugin if exist
}

/* wParam - SIM Number(SIM1 or SIM2) */
LRESULT CMainDialog::OnSendAsyncMessage(WPARAM wParam, LPARAM lParam)
{
	//message recvd when async message event is to be send
  ASSERT(wParam == SIM1 || wParam == SIM2 || wParam == (SIM1 | SIM2));
  PopUpMsgStruct* pstPopMsg = (PopUpMsgStruct*)lParam;

  if (wParam & SIM1)
  {
    CMainSimulator::OnSendAsyncMessage(MOD_PST, pstPopMsg);
  }
  if (wParam & SIM2)
  {
    CMainSimulator::OnSendAsyncMessage(MOD_PST_2, pstPopMsg);
  }
  
  delete pstPopMsg;
  return 0;
}

void CMainDialog::OnNcDestroy() 
{
	CDialog::OnNcDestroy();
	
	// TODO: Add your message handler code here
	delete this;
}

void CMainDialog::OnDblclkListIncomingMessages(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
  /*
  int nIndex = 0;
  CSelectMessageDlg popDlg;//set the string map with mesage id message response map
  popDlg.SetLibraryIdentifier(library_identifier);
  CString strMsgId;
  COutgoingMessage *pOutGoingMessage = NULL;
  CPtrList *plstRespMsgs = NULL;
  
  AMLOGINFO(_T("Incoming Message List Clicked"));
  
  POSITION pos = m_lstIncomingMessages.GetFirstSelectedItemPosition();
  if(pos!=NULL)
  {
    m_nIncomingListIndex = m_lstIncomingMessages.GetNextSelectedItem(pos);
    m_inComingMsgSelIndex = m_nIncomingListIndex;//contains the selected item
    strMsgId = m_lstIncomingMessages.GetItemText(m_nIncomingListIndex,0);
    AMLOGINFO(_T("Msg Id selected is -> %s"),strMsgId);
    CMessage *pMessage = m_pMessageManager->LocateMessageInList(m_pMessageManager->m_plstIncomingMsg,_ttoi(strMsgId));
    
    if((pMessage)) 
    {
      
      CPtrList *ptRespList = GetAllResponseMsgs(pMessage->GetMessageId());
      ClickedMsgId=pMessage->GetMessageId();
      RsponseMode = GetRspMode(pMessage->GetMessageId());
      CMessage *pOutGoingMsg,*pMessageOut;
      
      if(ptRespList->GetCount() > 1)
      {
        popDlg.m_strMessageId = strMsgId;
        plstRespMsgs = GetAllResponseMsgs(_ttoi(strMsgId.GetBuffer(0)));
        popDlg.m_lstRespMessages = plstRespMsgs;//stuff the CptrList		
        popDlg.DoModal();
      }
      
      if(ptRespList->GetCount() == 1) // if there is only one response message
      {
        POSITION pos = ptRespList->GetHeadPosition();				
        pOutGoingMsg = (COutgoingMessage*)(ptRespList->GetNext( pos )) ;
        pMessageOut = m_pMessageManager->LocateMessageInList(m_pMessageManager->m_plstOutGoingMsg,pOutGoingMsg->GetMessageId());					
        
        if((pMessageOut->GetPluginName().CompareNoCase(_T("")))&&pMessageOut->GetPluginName().CompareNoCase(_T("Null")))
        {
          popDlg.m_strMessageId = strMsgId;
          plstRespMsgs = GetAllResponseMsgs(_ttoi(strMsgId.GetBuffer(0)));
          popDlg.m_lstRespMessages = plstRespMsgs;//stuff the CptrList		
          popDlg.DoModal();
        }
        else
        {
          
          CFormat *pFormatOut = m_pMessageManager->GetFormatForMessage(pMessageOut);
          //Input mode is Dialog 
          if((RsponseMode==RSP_MANUAL)&&(!pFormatOut->GetInputMode().CompareNoCase(_T("Dialog"))))
          {
            IsClickOnIncomingMsgList=1;
            SendMsg(pMessageOut->GetMessageId());				
          }
          else //Input mode is dataset 
          {
            if(pMessage!=NULL)
            {
              popDlg.m_strMessageId = strMsgId;
              plstRespMsgs = GetAllResponseMsgs(_ttoi(strMsgId.GetBuffer(0)));
              popDlg.m_lstRespMessages = plstRespMsgs;//stuff the CptrList		
              popDlg.DoModal();
            }
          }
        }
        
      }
    }
  }
  */
	*pResult = 0;
}

void CMainDialog::OnSendMsgFromTBR(UINT nID)
{
  SendAsyncMessageToMMI(MOD_PST,GetSeqNameForButton(nID));
}

BOOL CMainDialog::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
  ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);
  
  // if there is a top level routing frame then let it handle the message
  if (GetRoutingFrame() != NULL) return FALSE;
  
  // to be thorough we will need to handle UNICODE versions of the message also !!
  TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
  TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
  //TCHAR szFullText[512];
  CString strTipText;
  UINT nID = pNMHDR->idFrom;
  
  if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
    pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
  {
    // idFrom is actually the HWND of the tool 
    nID = ::GetDlgCtrlID((HWND)nID);
  }
  
  if (nID != 0) // will be zero on a separator
  {
    //AfxLoadString(nID, szFullText);
    strTipText= GetSeqNameForButton(nID);
    
#ifndef _UNICODE
    if (pNMHDR->code == TTN_NEEDTEXTA)
    {
      lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
    }
    else
    {
      _mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
    }
#else
    if (pNMHDR->code == TTN_NEEDTEXTA)
    {
      _wcstombsz(pTTTA->szText, strTipText,sizeof(pTTTA->szText));
    }
    else
    {
      lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
    }
#endif
    
    *pResult = 0;
    
    // bring the tooltip window above other popup windows
    ::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,SWP_NOACTIVATE|
      SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER); return TRUE;
  }
	 return TRUE;
}
