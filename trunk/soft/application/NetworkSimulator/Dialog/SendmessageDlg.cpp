// SendMessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\NetworkSimulator.h"
#include "SendMessageDlg.h"

#include "..\Simulator\AsyncMessageSeq.h"
#include "..\Simulator\GlobalDefines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendMessageDlg dialog


CSendMessageDlg::CSendMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendMessageDlg)
	//}}AFX_DATA_INIT

  m_LibraryIdentifier = NULL;

}


void CSendMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendMessageDlg)
	DDX_Control(pDX, IDC_TREE_BOOT, m_treeAppControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSendMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CSendMessageDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendMessageDlg message handlers


BOOL CSendMessageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  Inittext();
	InitSampleData();
	m_treeAppControl.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CSendMessageDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

void CSendMessageDlg::SetLibrayIdentifier(HINSTANCE lib_id)
{
  m_LibraryIdentifier = lib_id;
}

void CSendMessageDlg::Inittext()
{
  ASSERT(m_LibraryIdentifier);

  CString str;
	LoadString(m_LibraryIdentifier,IDS_STRING_DLG_SEND_EVENTS, str.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
	SetWindowText(str);
  LoadString(m_LibraryIdentifier,IDS_STRING_SEND_MESSAGE, str.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
  GetDlgItem(IDC_STATIC_GROUP)->SetWindowText(str);
	LoadString(m_LibraryIdentifier,IDS_STRING_SEND, str.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
  GetDlgItem(IDOK)->SetWindowText(str);
	LoadString(m_LibraryIdentifier,IDS_STRING_CLOSE, str.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
  GetDlgItem(IDCANCEL)->SetWindowText(str);
}

void CSendMessageDlg::InitSampleData()
{
	HTREEITEM hRoot = m_treeAppControl.InsertItem(_T("Events"));
	m_treeAppControl.SetItemData(hRoot, LAYRER0);
	int iCount = 0;
	CDataList dataList;	
	CData dataObj;
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

			HTREEITEM h1 = m_treeAppControl.InsertItem(strXMLEvent, hRoot);			
			m_treeAppControl.SetItemData(h1,LAYRER1 | SIM1);
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
			  h1 = m_treeAppControl.InsertItem(strXMLEvent, hRoot);
        m_treeAppControl.SetItemData(h1,0);
      }
      DWORD dwItemData = m_treeAppControl.GetItemData(h1);
      dwItemData = dwItemData | LAYRER1 | SIM2;
			m_treeAppControl.SetItemData(h1,dwItemData);
			AddDataListInTree(ptrAyncEventsList,strXMLEvent,h1,SIM2);
		}
	}
#endif /*_DUAL_SIM_*/
  m_treeAppControl.Expand(hRoot,TVE_EXPAND);
}

CPtrList* CSendMessageDlg::GetAllAsyncEventsForApp(CString strRecvd, CPtrList *plstAsyncMsg, CPtrList *plstOutGoingMsg)
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
				AMLOGERROR(_T("Inside GetAllAsync Messages for App no found error"));
			}				
		}
	}	
	return  ptrList;
}

void CSendMessageDlg::AddDataListInTree(CPtrList *ptrAyncEventsList, CString strRecvd, HTREEITEM htree, DWORD dwSimNum)
{
  int iCount1=0;
  CFormat *pFormat;// = new CFormat();	
  CDataList dataList;	
  CData dataObj;
  for( POSITION pos = ptrAyncEventsList->GetHeadPosition(); pos != NULL;)
  {
    CMessage* pMessage = (CMessage*)(ptrAyncEventsList->GetNext( pos ));
    pFormat = m_Messagemanager.GetFormatForMessage(pMessage);
    HTREEITEM h3 = m_treeAppControl.InsertItem(pMessage->GetMessageName(), htree);
    m_treeAppControl.SetItemData(h3,LAYRER2 | dwSimNum);
    if(pFormat!=NULL)
    {
      CList<CVar,CVar&>& varList = pFormat->GetVarList();
      dataList = pFormat->GetDataList();
      CList<CData,CData&>& data= dataList.GetData();
      if((data.GetCount() > 0) && (varList.GetCount() > 0))
      {
        int iCount2 = 0;
        for( POSITION datapos = data.GetHeadPosition(); datapos != NULL;)
        {
          dataObj = (data.GetNext( datapos ));
          HTREEITEM h4 = m_treeAppControl.InsertItem(dataObj.GetName(), h3);
          m_treeAppControl.SetItemData(h4,LAYRER3 | dwSimNum);
        }
      }
    }
  }

  delete ptrAyncEventsList;
}

void CSendMessageDlg::OnOK() 
{
	// TODO: Add extra validation here
	HTREEITEM htree = m_treeAppControl.GetSelectedItem();
	if(htree)
	{
		if(GETLAYER(m_treeAppControl.GetItemData(htree)) == 0)
		{
			AfxMessageBox(IDS_STRING_PLEASE_SELECT_MESSAGES,MB_OK);
		}
		else
		{
      //CString *strSText = new CString();
      HWND hWnd = ::GetParent(this->m_hWnd);
      HTREEITEM htree = m_treeAppControl.GetSelectedItem();
      DWORD nID = GETLAYER(m_treeAppControl.GetItemData(htree));
      //*strSText = m_treeAppControl.GetItemText(htree);
      if(nID > 0)
      {
        UINT32 nMergeSimNum = GETSIMNUM(m_treeAppControl.GetItemData(htree)); // SIM1,SIM2,SIM1 | SIM2
        PopUpMsgStruct *stMsgStruct = new PopUpMsgStruct;
        if(nID == 3)
        {
          //*strSText = m_treeAppControl.GetItemText(htree);//datalist
          stMsgStruct->strMsgValue = m_treeAppControl.GetItemText(htree);
          //*strSText = m_treeAppControl.GetItemText(m_treeAppControl.GetParentItem(htree));//message
          stMsgStruct->strMsgResponse = m_treeAppControl.GetItemText((m_treeAppControl.GetParentItem(htree)));//message
          stMsgStruct->nMsgId = 3;
          ::PostMessage(hWnd,WM_SEND_ASYNC_EVENT_MSG,nMergeSimNum,(LPARAM) stMsgStruct);
          //datalist is selected
        }
        else if(nID == 2)
        {
          //Message is selected with default datalist
          //*strSText = m_treeAppControl.GetItemText(htree);//message
          stMsgStruct->strMsgResponse = m_treeAppControl.GetItemText(htree);//message
          //*strSText = m_treeAppControl.GetItemText(m_treeAppControl.GetChildItem(htree));//datalist
          stMsgStruct->strMsgValue = m_treeAppControl.GetItemText((m_treeAppControl.GetChildItem(htree)));
          stMsgStruct->nMsgId = 2;
          //stPopMsg->nMsgId
          //stPopMsg->strMsgValue
          ::PostMessage(hWnd,WM_SEND_ASYNC_EVENT_MSG,nMergeSimNum,(LPARAM) stMsgStruct);
        }
        else if(nID == 1)
        {	
          stMsgStruct->nMsgId = 1;
          //*strSText = m_treeAppControl.GetItemText(htree);//message
          stMsgStruct->strMsgResponse = m_treeAppControl.GetItemText(htree);//message
          //::PostMessage(hWnd,WM_SEND_ASYNC_EVENT_MSG,0,LPARAM(strSText));
          ::PostMessage(hWnd,WM_SEND_ASYNC_EVENT_MSG,nMergeSimNum,(LPARAM) stMsgStruct);
        }
        
        //get the value from the xml and pop up the appropriate dialog box and send the message from there
        //AfxMessageBox(_T("Pop up value clicked"));
        //get the event name and then send it to the main handler
        //so that message can be handled and send to socket
        
      }
		}
	}
	else
  {
		AfxMessageBox(IDS_STRING_PLEASE_SELECT_ITEM,MB_OK);
  }
}

HTREEITEM CSendMessageDlg::FindItem(CString strItemName, HTREEITEM hParentItem)
{
  HTREEITEM hTreeItem = NULL;
  HTREEITEM hChildItem = m_treeAppControl.GetChildItem(hParentItem);
  while (hChildItem)
  {
    if(m_treeAppControl.GetItemText(hChildItem).CompareNoCase(strItemName) == 0)
    {
      hTreeItem = hChildItem;
      break;
    }
    hChildItem = m_treeAppControl.GetNextSiblingItem(hChildItem);
  }

  return hTreeItem;
}
