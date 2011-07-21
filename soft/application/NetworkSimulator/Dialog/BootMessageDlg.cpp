// BootMessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\NetworkSimulator.h"
#include "BootMessageDlg.h"
#include "..\Simulator\AsyncMessageSeq.h"
#include "..\Simulator\NWGCMLHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBootMessageDlg dialog


CBootMessageDlg::CBootMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBootMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBootMessageDlg)
	//}}AFX_DATA_INIT
//  m_ptrCPtrList = NULL;
//  m_pBootUpMsg = NULL;
}


void CBootMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBootMessageDlg)
	DDX_Control(pDX, IDC_TREE_BOOT, m_treeControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBootMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CBootMessageDlg)
	//}}AFX_MSG_MAP
  ON_MESSAGE(WM_ENABLE_SAVE_BUTTON,SetTreeStateChangeFlag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBootMessageDlg message handlers

BOOL CBootMessageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  InitText();
  m_TreeStateChangeFlag =0;
  
  // TODO: Add extra initialization here
  
	m_treeControl.ModifyStyle( TVS_CHECKBOXES, 0 );
  m_treeControl.ModifyStyle( 0, TVS_CHECKBOXES );
  
  HTREEITEM hRoot = m_treeControl.InsertItem(_T("Events"));
  m_treeControl.SetItemData(hRoot, LAYRER0);
  int iCount = 0;

  //char *dummy = (char*) malloc(sizeof(char)*10);
  CString strXMLEvent;
  CPtrList *plstAsyncMsg = m_Messagemanager.m_plstAsyncMsg;
  CPtrList *plstOutGoingMsg = m_Messagemanager.m_plstOutGoingMsg;
  if(plstAsyncMsg->GetCount() > 0)//check if emtpy message list 
  {	
    for( POSITION pos = plstAsyncMsg->GetHeadPosition(); pos != NULL;)
    {
      CAsyncMessageSeq *pAsyncMessage = (CAsyncMessageSeq*)(plstAsyncMsg->GetNext( pos )) ;
      strXMLEvent = pAsyncMessage->GetMessageName();
      CPtrList *ptrAyncEventsList = GetAllAsyncEventsForApp(strXMLEvent,plstAsyncMsg,plstOutGoingMsg);
      HTREEITEM h1 = m_treeControl.InsertItem(strXMLEvent, hRoot);			
      m_treeControl.SetItemData(h1,LAYRER1 | SIM1);
      AddDataListInTree(ptrAyncEventsList,strXMLEvent,h1,SIM1);
    }
  }
#ifdef _DUAL_SIM_
  plstAsyncMsg = m_Messagemanager.m_plstAsyncMsg2;
  plstOutGoingMsg = m_Messagemanager.m_plstOutGoingMsg2;
  if(plstAsyncMsg->GetCount() > 0)//check if emtpy message list 
  {	
    for( POSITION pos = plstAsyncMsg->GetHeadPosition(); pos != NULL;)
    {
      CAsyncMessageSeq *pAsyncMessage = (CAsyncMessageSeq*)(plstAsyncMsg->GetNext( pos )) ;
      strXMLEvent = pAsyncMessage->GetMessageName();
      CPtrList *ptrAyncEventsList = GetAllAsyncEventsForApp(strXMLEvent,plstAsyncMsg,plstOutGoingMsg);
      HTREEITEM h1 = this->FindItem(strXMLEvent, hRoot);
      if (h1 == NULL)
      {
        h1 = m_treeControl.InsertItem(strXMLEvent, hRoot);
      }	
      m_treeControl.SetItemData(h1,LAYRER1 | SIM2);
      AddDataListInTree(ptrAyncEventsList,strXMLEvent,h1,SIM2);
    }
  }
#endif /*_DUAL_SIM_*/

  m_treeControl.Expand(hRoot,TVE_EXPAND);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HTREEITEM CBootMessageDlg::FindItem(CString strItemName, HTREEITEM hParentItem)
{
  HTREEITEM hTreeItem = NULL;
  HTREEITEM hChildItem = m_treeControl.GetChildItem(hParentItem);
  while (hChildItem)
  {
    if(m_treeControl.GetItemText(hChildItem).CompareNoCase(strItemName) == 0)
    {
      hTreeItem = hChildItem;
      break;
    }
    hChildItem = m_treeControl.GetNextSiblingItem(hChildItem);
  }

  return hTreeItem;
}

void CBootMessageDlg::OnOK() 
{
	// TODO: Add extra validation here
  int response = AfxMessageBox(IDS_STRING_WANT_TO_SAVE_CHANGES,MB_YESNOCANCEL);
  if(response == IDYES)
  {
    SaveBootupMsgSeq();
    GetDlgItem(IDOK)->EnableWindow(FALSE);
    m_TreeStateChangeFlag =0;
    GetDlgItem(IDCANCEL)->SetFocus();
  }

	//CDialog::OnOK();
}

void CBootMessageDlg::InitText()
{
  CString str;

  LoadString(m_library_identifier,IDS_STRING_MODI_BOOT_SEQ_TITLE_TEXT,str.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
	SetWindowText(str);

	LoadString(m_library_identifier,IDS_STRING_MODI_BOOT_SEQ_GRP_BOX_TEXT,str.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
  GetDlgItem(IDC_STATIC_GROUP)->SetWindowText(str);

	LoadString(m_library_identifier,IDS_STRING_SAVE,str.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
	GetDlgItem(IDOK)->SetWindowText(str);


	LoadString(m_library_identifier,IDS_STRING_CLOSE,str.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
	GetDlgItem(IDCANCEL)->SetWindowText(str);
}

CPtrList* CBootMessageDlg::GetAllAsyncEventsForApp(CString strRecvd, CPtrList *plstAsyncMsg, CPtrList *plstOutGoingMsg)
{
  CPtrList *ptrList = new CPtrList();
  CAsyncMessageSeq *pAsyncMessage;	
  CString strXMLEvent;
  
  if(plstAsyncMsg->GetCount() > 0)//check if emtpy message list 
  {	
    for( POSITION pos = plstAsyncMsg->GetHeadPosition(); pos != NULL;)
    {
      pAsyncMessage = (CAsyncMessageSeq*)(plstAsyncMsg->GetNext( pos )) ;
      strXMLEvent = pAsyncMessage->GetMessageName();
      if(!strXMLEvent.CompareNoCase(strRecvd))
        //add it into the Map
      {
        //now peek into the outgoing message list for this messagid 
        //and get the CMessage object to be done
        //keep on adding into the Ptr List and return it 
        CList<UINT32, UINT32&>& messageList = pAsyncMessage->GetMessageList();								
        if(messageList.GetCount() > 0)
        {	
          for( POSITION posResp = messageList.GetHeadPosition(); posResp != NULL; )					
          {
            ptrList->AddTail(m_Messagemanager.LocateMessageInList(plstOutGoingMsg,messageList.GetNext(posResp)));	
            
          }					
        }
        if(ptrList->GetCount() <= 0)
        {
          AMLOGERROR(_T("Inside GetAllAsync Messages for App no found error"));
        }
      }				
    }
  }	
  return  ptrList;
}

void CBootMessageDlg::AddDataListInTree(CPtrList *ptrAyncEventsList, CString strRecvd, HTREEITEM htree, DWORD dwSimNum)
{
  int iCount1=0;
  CFormat *pFormat;// = new CFormat();	
  CDataList dataList;	
  CData dataObj;
  char dummy[50];
  CBootUpMsg *pBootUpMsg = NULL;
  CPtrList* pBootupList = NULL;

  pBootupList = m_Messagemanager.m_pBootupList;
#ifdef _DUAL_SIM_
  if (dwSimNum == SIM2)
  {
    pBootupList = m_Messagemanager.m_pBootupList2;
  }
#endif /*_DUAL_SIM_*/

  ASSERT(pBootupList);

  for( POSITION pos = ptrAyncEventsList->GetHeadPosition(); pos != NULL;)
  {
    CMessage* pMessage = (CMessage*)(ptrAyncEventsList->GetNext( pos ));
    pFormat = m_Messagemanager.GetFormatForMessage(pMessage);
    
    HTREEITEM h3 = m_treeControl.InsertItem(CString(itoa(pMessage->GetMessageId(),dummy,10))+CString(":")+pMessage->GetMessageName(), htree);
    m_treeControl.SetItemData(h3,LAYRER2 | dwSimNum);
    
    if(pBootupList)
    {
      if(pBootupList->GetCount() > 0)//check if emtpy message list 
      {	
        POSITION pos = pBootupList->GetHeadPosition();
        pBootUpMsg = (CBootUpMsg *)(pBootupList->GetNext( pos )) ;
      }
    }
    
    CList <UINT32, UINT32>& messageidlist = pBootUpMsg->GetBootMessageList();
    if(messageidlist.GetCount()>0)
    {
      for( POSITION pos=messageidlist.GetHeadPosition();pos!=NULL;)
      {
        if((UINT32) messageidlist.GetNext(pos)== pMessage->GetMessageId())
        {
          m_treeControl.SetCheck(h3,1);
          m_treeControl.SetParentalCheckState(h3,1);
        }
      }
    }
  }
}

void CBootMessageDlg::SaveBootupMsgSeq()
{
  CPtrList* ptrCPtrList = NULL;
  CBootUpMsg* pBootUpMsg = NULL;
  CBootUpMsg* pBootUpMsg2 = NULL;

  CNWGCMLHandler gcmlHandler(CMessageManager::GetFilePath().LockBuffer());
  HCURSOR cursor_to_be_shown = LoadCursor(NULL,IDC_APPSTARTING);
  SetCursor(cursor_to_be_shown);
  
  ptrCPtrList = (CPtrList*)m_Messagemanager.m_pBootupList;
  if(ptrCPtrList)
  {
    if(ptrCPtrList->GetCount() > 0)//check if emtpy message list 
    {	
      POSITION pos = ptrCPtrList->GetHeadPosition();
      pBootUpMsg = (CBootUpMsg *)(ptrCPtrList->GetNext( pos )) ;
    }
  }
  CList <UINT32, UINT32>& messageidlist = pBootUpMsg->GetBootMessageList();
  
  // Removing existing message sequence
  while(messageidlist.GetCount()>0) 
  {
    messageidlist.RemoveTail();
  }

  //Prepare new Bootup Message sequence list
  PrepareBootupMsgSeqList(m_treeControl.GetRootItem(),0,pBootUpMsg,SIM1);
#ifdef _DUAL_SIM_
  ptrCPtrList = (CPtrList*)m_Messagemanager.m_pBootupList2;
  if(ptrCPtrList)
  {
    if(ptrCPtrList->GetCount() > 0)//check if emtpy message list 
    {	
      POSITION pos = ptrCPtrList->GetHeadPosition();
      pBootUpMsg2 = (CBootUpMsg *)(ptrCPtrList->GetNext( pos )) ;
    }
  }
  CList <UINT32, UINT32>& messageidlist2 = pBootUpMsg2->GetBootMessageList();
  
  // Removing existing message sequence
  while(messageidlist2.GetCount()>0) 
  {
    messageidlist2.RemoveTail();
  }

  //Prepare new Bootup Message sequence list
  PrepareBootupMsgSeqList(m_treeControl.GetRootItem(),0,pBootUpMsg2,SIM2);
#endif /*_DUAL_SIM_*/

  cursor_to_be_shown = LoadCursor(NULL,IDC_ARROW);
  SetCursor(cursor_to_be_shown);

  // Now write this message list in DOM
  if (!gcmlHandler.UpdateBootupMsgListInDom(messageidlist,TAG_BOOTUPMSG))
  {
    CString str;
    LoadString(m_library_identifier,IDS_STRING_UPDATE_BOOTUP_FAILED,str.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
  }
#ifdef _DUAL_SIM_
  // Now write this message list in DOM
  if (!gcmlHandler.UpdateBootupMsgListInDom(messageidlist2,TAG_BOOTUPMSG2))
  {
    CString str;
    LoadString(m_library_identifier,IDS_STRING_UPDATE_BOOTUP_FAILED,str.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
  }
#endif /*_DUAL_SIM_*/
}

void CBootMessageDlg::SetLibraryIdentifier(HINSTANCE lib_id)
{
  m_library_identifier = lib_id;
}

void CBootMessageDlg::PrepareBootupMsgSeqList(HTREEITEM root, int node_level,CBootUpMsg* pBootUpMsg,DWORD dwSimNum)
{
  UINT32 MsgNo, msgfoundStatus;
  
  UINT32 messageid;
  
  //cmessagemanager.m_pBootupList->RemoveAll();
  
  if(root!=NULL)
  {
    if(node_level ==2)
    {
      if(m_treeControl.GetCheck(root) && GETSIMNUM(m_treeControl.GetItemData(root)) == dwSimNum)
      {
        MsgNo = _ttoi(m_treeControl.GetItemText(root));
        // Now check this Message in the list of Bootupmessage list
        msgfoundStatus=0;
        
        CList <UINT32, UINT32>& messageidlist = pBootUpMsg->GetBootMessageList();
        
        if(messageidlist.GetCount()>0)
        {
          for(POSITION pos=messageidlist.GetHeadPosition();pos!=NULL;)
          {
            messageid = (UINT32) messageidlist.GetNext(pos); 
            if(messageid == MsgNo)
            {
              msgfoundStatus=1;
            }
          }
        }
        if(msgfoundStatus==0)
        {
          messageidlist.AddTail(MsgNo);
        }
      }
      PrepareBootupMsgSeqList(m_treeControl.GetNextSiblingItem(root),node_level,pBootUpMsg,dwSimNum);
    }
    else
    {
      PrepareBootupMsgSeqList(m_treeControl.GetChildItem(root),node_level + 1,pBootUpMsg,dwSimNum);
      PrepareBootupMsgSeqList(m_treeControl.GetNextSiblingItem(root),node_level,pBootUpMsg,dwSimNum);
    }
  }
  else
    return;
}

void CBootMessageDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
  if(m_TreeStateChangeFlag)
  {
    int response = AfxMessageBox(IDS_STRING_WANT_TO_SAVE_CHANGES,MB_YESNOCANCEL);
    if(response == IDYES)
    {
      SaveBootupMsgSeq(); 
      m_TreeStateChangeFlag =0;
      GetDlgItem(IDOK)->EnableWindow(FALSE);
      CDialog::OnCancel();
    }
    else if(response == IDNO)
    {
      CDialog::OnCancel();
    }
    else
    {
      return;
    }
  }
  else
  {
    CDialog::OnCancel();
  }
}

		
LRESULT CBootMessageDlg::SetTreeStateChangeFlag(WPARAM a,LPARAM treeitem)
{
	m_TreeStateChangeFlag =1;
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	return TRUE;
}
