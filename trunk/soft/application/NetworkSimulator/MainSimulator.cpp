// MainSimulator.cpp: implementation of the CMainSimulator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetworkSimulator.h"
#include "MainSimulator.h"

#include "Simulator\BootUpMsg.h"
#include "Simulator\NSRThread.h"
#include "Simulator\NSWThread.h"
#include "Simulator\Message.h"

//#include "Afx.h"
// #include "GlobalDefines.h"
// #include "AsyncSendMessage.h"
// #include "NSRThread.h"
// #include "NSWThread.h"
// #include "ProtocolEvents.h"
// #include "PopSelectMsgDlg.h"
// #include "protocolevents.h"
// #include "MMINWSimulatorDlg.h"
#include "Simulator\TaskInit.h"
// #include "DisplayType.h"
// #include "MMSYSTEM.h"
// #include "MsgAttributeChange.h"
// #include "MessageSearch.h"
// #include "AddEditDialog.h"
// #include "mminwsimulatordlg.h"
// #include "RespModeChg.h"
// #include "Afxpriv.h"
// #include "GlobalFrequencyTimerChange.h" 
// #include "BootUpMsg.h"
// #include "FreqUsedMsg.h"
// #include "GenericInput.h"
// #include "GCMLGenerator.h"
// #include "AboutDialog.h"
// #include "ModifyBootupMsgSeq.h"
#include "Simulator\XMLverifier.h"

extern void *HandleWriteEntryRequest();
extern void *HandleGetAllEntriesRequest();
extern void *HandleDeleteEntryRequest();
extern void *HandleSearchEntryRequest();
extern void *HandleGetStorageRequest();
extern void *HandleSetStorageRequest();
extern void SetBufferForPbook(void *Pdata);
extern void *SendSMSResponse();
extern void *SendSMSContectByIndex(UINT32 nSimNum);
extern void *SendGetVoicemailAddressRsp(); //abhi
extern void *SendSetVoicemailAddressRsp(); //abhi
extern void *SendSMSToMMI(UINT32 nSimNum);
extern void *SendMessageList();
extern void *SendMessageNumber();
extern void *SaveSMSMessage(UINT32 nSimNum);
extern void *GetMemoryStorage();
extern void *SetMemoryStorage();
extern void *DeleteSMSRequest(UINT32 nSimNum);
extern void SetBufferForMessage(void *Pdata);
//extern void  AudioPlayReq(void* Req);       //zhuoxz,2008-09-24
//extern void AudioStopReq(void);             //zhuoxz,2008-09-24
extern void *CheckSecurityBootup();
extern void *SettingGPRSGetConnectType(void);
extern void *SettingGPRSSetConnectType(void);
extern void *GetCallPriceUnit(void);
extern void *SetCallPriceUnit(void);
extern void *SettingGetNetworkBand(void);
extern void *SettingSetNetworkBand(void);
extern void *ValidatePinBootup();
extern void SetBufferForSettings(void * dataBuf);
void *CheckPinBS(int nSimNum);
//extern void AudioPlayIMelodyStringReq(void*); //zhuoxz,2008-09-24
//extern void AudioPlayIMelodyFileReq(void*);   //zhuoxz,2008-09-24

extern void* SMSBootUpBeginInd(UINT32 nSimNum);
extern void* SMSSendBootupReadInd(UINT32 nSimNum);
extern void *SendSmsMemStatus();
extern void *GetProfileParamsRsp(UINT32 nSimNum);
extern void *SetProfileSettingParamsRsp(void);
extern void *GetPasswordData(void *dataBuff);
extern void SaveCBMessageSetting();

extern void SetBufferForCallHistory(void* dataBuf);
extern void *SetLastNoRsp(UINT32 nSimNum);
extern void *GetLastNoRsp(UINT32 nSimNum);
extern void * DeleteAllCallsRsp(UINT32 nSimNum);
extern void *IncomingCallInd();
extern void *CheckSimLock();
extern void *BootupFunctions();
extern void *SendCBMessageSetting();


//////////////////////////////////////////////////////////////////////////
// zhuoxz 2008-09-01
#include "Dialog\BootMessageDlg.h"
#include "Dialog\SendMessageDlg.h"
//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


static UINT BASED_CODE buttons[] = 
{
	ID_BUTTON_MSG1,
	ID_BUTTON_MSG2,
	ID_BUTTON_MSG3,
	ID_BUTTON_MSG4,
	ID_BUTTON_MSG5,
    ID_BUTTON_MSG6,
	ID_BUTTON_MSG7,
	ID_BUTTON_MSG8,
	ID_BUTTON_MSG9,
	ID_BUTTON_MSG10,
};

#define RSP_MANUAL 1
#define RSP_AUTO   2

void* MyMallocDataPtr(U32 size);
CNSWThread *myNSWThread;
CNSRThread *myNSRThread;

void* MyMallocDataPtr(U32 size)
{
	void *ptr;
	struct tsize{
		U32 size;
	};
	struct tsize *temp, tempdata;
	int sizeofuint = sizeof(U32);
	temp = (tsize*) malloc(size + sizeofuint);
	tempdata.size=size;
	memcpy(temp, &tempdata, sizeof(tempdata));
	ptr = (void*)((char*)temp + sizeof(tempdata));
	return ptr;	
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainSimulator::CMainSimulator()
{
  m_pWnd = NULL;
  m_lstIncomingMessages = NULL;
  m_lstOutGoingMessages = NULL;
  m_pMessageManager = NULL;
  m_pIncomingMsgDataList = NULL;
#ifdef _DUAL_SIM_
  m_pIncomingMsgDataList2 = NULL;
#endif /*_DUAL_SIM_*/
  m_OutGoingMsg = NULL;

  CNSWThread::m_pReceiveQ = NULL;
  CNSRThread::m_pWriteQ = NULL;
  CNSWThread::m_QHandle = NULL;
  CNSRThread::m_QHandle = NULL;

  m_IsClickOnIncomingMsgList=0;

  m_bIsInit = FALSE;

  // temporarily variable
  m_ptrAyncEventsList = NULL;
  m_MessageDetailPacket.strDataList = NULL;

  myNSRThread = NULL;
  myNSWThread = NULL;
}

CMainSimulator::~CMainSimulator()
{
  if (m_pIncomingMsgDataList)
  {
    delete m_pIncomingMsgDataList;
  }
#ifdef _DUAL_SIM_
  if (m_pIncomingMsgDataList2)
  {
    delete m_pIncomingMsgDataList2;
  }
#endif /*_DUAL_SIM_*/

  if (m_pMessageManager)
  {
    delete m_pMessageManager;
  }

  // temporarily variable
  if (m_ptrAyncEventsList)
  {
    delete m_ptrAyncEventsList;
  }
  if (m_MessageDetailPacket.strDataList)
  {
    free(m_MessageDetailPacket.strDataList);
  }
}

void CMainSimulator::SetWnd(CWnd *pWnd)
{
  this->m_pWnd = pWnd;
}

void CMainSimulator::SetListCtrl(CListDrop *plstMsgSend, CListDrop *plstMsgFrom)
{
  this->m_lstIncomingMessages = plstMsgSend;
  this->m_lstOutGoingMessages = plstMsgFrom;
}

BOOL CMainSimulator::InitSimulator()
{
  ASSERT(m_pWnd);
  if (m_bIsInit)
  {
    return TRUE;
  }
  m_bIsInit = TRUE;

  myNSRThread = new CNSRThread();
  if(!myNSRThread->CreateThread())
  {
    return FALSE;
  }
  myNSWThread = new CNSWThread();
  if(!myNSWThread->CreateThread())
  {
    return FALSE;
  }
	myNSRThread->SetWindowHandle(m_pWnd->m_hWnd);
  myNSWThread->SetWindowHandle(m_pWnd->m_hWnd);

  SetTheLibraryIdentifier(AfxGetInstanceHandle());
  m_pIncomingMsgDataList = new CPtrList();
#ifdef _DUAL_SIM_
  m_pIncomingMsgDataList2 = new CPtrList();
#endif /*_DUAL_SIM_*/

  //m_ImageList = new CImageList();
	hNormalImageList = ImageList_Create(32,32,ILC_MASK | ILC_COLOR32,100,50);
	//HIMAGELIST hList = m_ImageList.Create(16,16, ILC_COLOR8 |ILC_MASK , 1, 1);	
// Load the large icons
	CBitmap cBmpIn,cBmpOut;
	cBmpIn.DeleteObject();	
	cBmpOut.DeleteObject();	
	int n =ImageList_AddMasked(hNormalImageList, (HBITMAP)cBmpIn.operator HBITMAP(), RGB(0,0,0));
	n =ImageList_AddMasked(hNormalImageList, (HBITMAP)cBmpOut.operator HBITMAP(), RGB(0,0,0));
  m_lstIncomingMessages->SendMessage(LVM_SETIMAGELIST,(WPARAM)(int) LVSIL_NORMAL ,(LPARAM)(HIMAGELIST) hNormalImageList);

  TCHAR szProfile[_MAX_PATH+1] = {0};
  TCHAR szFileName[_MAX_PATH+1] = {0};
  TCHAR szDir[_MAX_PATH+1] = {0};
  TCHAR szDriver[_MAX_PATH+1] = {0};
#ifdef _UNICODE
  GetModuleFileNameW(NULL, szFileName, _MAX_PATH);
  _wsplitpath(szFileName, szDriver, szDir, NULL, NULL);

#else
  GetModuleFileNameA(NULL, szFileName, _MAX_PATH);
  _splitpath(szFileName, szDriver, szDir, NULL, NULL);
#endif
  CString strProfile = CString(szDriver) + szDir + _T("\\NetworSimulator.ini");


  TCHAR szValue[_MAX_PATH+1] = {0};
  GetPrivateProfileString(_T("SETTINGS"),_T("MessageFile"),_T(""),(U16*)szValue,_MAX_PATH,strProfile);
#ifdef _DUAL_SIM_
  GetPrivateProfileString(_T("SETTINGS"),_T("MessageFile2"),_T(""),(U16*)szValue,_MAX_PATH,strProfile);
#endif /*_DUAL_SIM_*/
  m_pMessageManager = new CMessageManager();
  if(!m_pMessageManager->InitializeXMLLists(CString(szDriver) + szDir + szValue))
  {
    delete m_pMessageManager;
    m_pMessageManager = NULL;
    return FALSE;
  }

  // must be after InitializeXMLLists()
  if (!InitialiseToolBar())
  {
    return FALSE;
  }

  // Send Message List
  LoadString(library_identifier,IDS_STRING_FROM_MESSAGE_ID,stringID_to_string.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
	int iError = m_lstIncomingMessages->InsertColumn(0,stringID_to_string,NON_EDITABLE,NULL,MAX_STR_RES_LEN,LVCFMT_LEFT,120,0);
  LoadString(library_identifier,IDS_STRING_SEND_MESSAGE_ID,stringID_to_string.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
	iError = m_lstOutGoingMessages->InsertColumn(0,stringID_to_string,NON_EDITABLE,NULL,MAX_STR_RES_LEN,LVCFMT_LEFT,120,0);

	LoadString(library_identifier,IDS_STRING_MESSAGE_NAME,stringID_to_string.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
	iError = m_lstIncomingMessages->InsertColumn(1,stringID_to_string,SELECTABLE,NULL,MAX_STR_RES_LEN,LVCFMT_LEFT,300,0);  
	iError = m_lstOutGoingMessages->InsertColumn(1,stringID_to_string,SELECTABLE,NULL,MAX_STR_RES_LEN,LVCFMT_LEFT,300,0);

	LoadString(library_identifier,IDS_STRING_MESSAGE_TIME,stringID_to_string.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
	iError = m_lstIncomingMessages->InsertColumn(2,stringID_to_string,SELECTABLE,NULL,MAX_STR_RES_LEN,LVCFMT_LEFT,200,0);  
	iError = m_lstOutGoingMessages->InsertColumn(2,stringID_to_string,SELECTABLE,NULL,MAX_STR_RES_LEN,LVCFMT_LEFT,200,0);
	
  ASSERT(m_pWnd);
	LoadString(library_identifier,IDS_STRING_DLG_NWS,stringID_to_string.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
	m_pWnd->SetWindowText(stringID_to_string);
	DWORD dwStyleOld = m_lstOutGoingMessages->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE,0,0);
	dwStyleOld = dwStyleOld^LVS_EX_FULLROWSELECT;
	m_lstOutGoingMessages->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)dwStyleOld);
	dwStyleOld = m_lstIncomingMessages->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE,0,0);
	dwStyleOld = dwStyleOld^LVS_EX_FULLROWSELECT;
	m_lstIncomingMessages->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)dwStyleOld);	
  
  return TRUE;
}

BOOL CMainSimulator::ExitSimulator()
{
  m_bIsInit = FALSE;
  
  // 释放资源
  if (m_pMessageManager)
  {
    this->m_pMessageManager->ReleaseXMLLists();
  }


  // 退出线程
  BOOL rez;
  DWORD exitcd;
  if(myNSRThread)
  {
    rez = GetExitCodeThread(myNSRThread, &exitcd);
    if(rez && (exitcd == STILL_ACTIVE))
      rez = TerminateThread(myNSRThread,0);
    delete myNSRThread;
    myNSRThread = NULL;
  }
  if(myNSWThread)
  {
    rez = GetExitCodeThread(myNSWThread, &exitcd);
    if(rez && (exitcd == STILL_ACTIVE))
      rez = TerminateThread(myNSWThread,0);
    delete myNSWThread;
    myNSWThread = NULL;
  }

  return rez;
}

BOOL CMainSimulator::IsInit()
{
  return m_bIsInit;
}

BOOL CMainSimulator::SendBootupMessage()
{
  if (IsInit() == FALSE)
  {
    return FALSE;
  }


	UINT32 messageid;
	CBootUpMsg *pBootUpMsg;
#ifdef _DUAL_SIM_
  #define SIM_COUNT 2
  CPtrList *ptrCPtrList[SIM_COUNT] = {(CPtrList*)m_pMessageManager->m_pBootupList,(CPtrList*)m_pMessageManager->m_pBootupList2};
  CPtrList *plstOutGoingMsg[SIM_COUNT] = {m_pMessageManager->m_plstOutGoingMsg,m_pMessageManager->m_plstOutGoingMsg2};

  U32 ModeType[SIM_COUNT] = {MOD_PST,MOD_PST_2};
#else
  #define SIM_COUNT 1
  CPtrList *ptrCPtrList[SIM_COUNT] = {(CPtrList*)m_pMessageManager->m_pBootupList};
  CPtrList *plstOutGoingMsg[SIM_COUNT] = {m_pMessageManager->m_plstOutGoingMsg};
  U32 ModeType[SIM_COUNT] = {MOD_PST};
#endif /*_DUAL_SIM_*/


  for (int i=0;i<SIM_COUNT;i++)
  {
	  if(ptrCPtrList[i])
	  {
		  if(ptrCPtrList[i]->GetCount() > 0)//check if emtpy message list 
		  {	
			  POSITION pos = ptrCPtrList[i]->GetHeadPosition();
			  pBootUpMsg = (CBootUpMsg *)(ptrCPtrList[i]->GetNext( pos )) ;
			  CList <UINT32, UINT32>& messageidlist = pBootUpMsg->GetBootMessageList();
			  if(messageidlist.GetCount()>0)
			  {
				  for(pos=messageidlist.GetHeadPosition();pos!=NULL;)
				  {
					  messageid = (UINT32) messageidlist.GetNext(pos); 
					  SendMsg(messageid, plstOutGoingMsg[i],ModeType[i]);
				  }
			  }
		  }
	  }
  }

  return TRUE;
}

void CMainSimulator::SendMsg(UINT32 MsgId, CPtrList *plstOutGoingMsg, U32 ModeType)
{
  CString MsgName;	
	CMessage *msg;	
	msg = m_pMessageManager->LocateMessageInList(plstOutGoingMsg,MsgId);
	if(msg)
	{
		SendMessageToMMI(msg,_T(""),ModeType);
	}
	else
	{
		char *dummy = (char*) malloc(sizeof(char)*10);
		AfxMessageBox(CString("Invalid Message Id :")+CString(itoa(MsgId,dummy,10))+CString("\nPlease check for XML Definations"));
		free(dummy);
	}
}

void CMainSimulator::SendMessageToMMI(CMessage *clMessage, CString strDataSet, U32 ModeType)
{
  //now finally send the message to socket for this message
	//check the plugin name if exist then call it straightway 
	UINT32 iSize = 0,freq,timer;
	int loopcounter;
	CString tempstr,strPlugin = clMessage->GetPluginName();
	MYQUEUE stMsgQueue;
	freq = m_pMessageManager->GetFreq(clMessage->GetMessageId(),strDataSet);  
	// Check if frequency is >1

	CXMLVerifier *XMLVerifierObj = new CXMLVerifier();
	
	if(XMLVerifierObj->IsAnAsyncMessage(clMessage->GetMessageId()) !=1)
	{
		freq = 1;
	}

	delete XMLVerifierObj;

	if (freq>1)
	{
		//get default timer
		timer= m_pMessageManager->GetTimer (clMessage->GetMessageId(),strDataSet);  

    // zhuoxz,2008-09-02
		//SendMessageDetails messagedetailpacket;
		//messagedetailpacket = (SendMessageDetails*) malloc(sizeof(SendMessageDetails));
		if (m_MessageDetailPacket.strDataList)
		{
      free(m_MessageDetailPacket.strDataList);
      memset(&m_MessageDetailPacket,0,sizeof(SendMessageDetails));
		}

		m_MessageDetailPacket.clMessage   = clMessage;
		m_MessageDetailPacket.strDataList = (TCHAR *)malloc( sizeof(TCHAR)* strDataSet.GetLength());
		for(loopcounter=0;loopcounter<strDataSet.GetLength();loopcounter++)
		{
			m_MessageDetailPacket.strDataList[loopcounter] = strDataSet.GetAt(loopcounter);
		}
		m_MessageDetailPacket.strDataList[loopcounter]=0; // Null
		m_MessageDetailPacket.Freq		 = freq-1; 
		m_MessageDetailPacket.Timer		 = timer;
		m_MessageDetailPacket.TimerId	 = NULL;
		SendMessageFreq(clMessage,strDataSet);
		CreateTimer(&m_MessageDetailPacket);
	}
	else
	{
		if(strPlugin ==_T("")) // frequency is 1 and no plugin
		{
			void *pData = NULL;
 			pData = GetDataMemoryForMessage(clMessage,&iSize,strDataSet);
			if(pData!=NULL)  
			{
				stMsgQueue.oslSrcId = ModeType;
				stMsgQueue.oslDestId = MOD_PRT;//PRT_EVENT;//
				stMsgQueue.oslMsgId = clMessage->GetMessageId();
				stMsgQueue.oslDataPtr = pData;
				stMsgQueue.oslPeerBuffPtr = NULL;
				myNSRThread->WriteToSocket(&stMsgQueue);
        
        //stMsgQueue.oslSrcId = MOD_L4C_2;
        //myNSRThread->WriteToSocket(&stMsgQueue);
			}
			else
			AMLOGINFO(_T("Memory NULL Buffer error for Outgoing Message"));
		}				
		else
		{
      CPtrList *pIncomingMsgDataList = NULL;
      if (ModeType == MOD_PST)
      {
        pIncomingMsgDataList = this->m_pIncomingMsgDataList;
      }
#ifdef _DUAL_SIM_
      else if (ModeType == MOD_PST_2)
      {
        pIncomingMsgDataList = this->m_pIncomingMsgDataList2;
      }
#endif
      //if (pIncomingMsgDataList)
      {
		    //call the pluggin straightaway
			  void *pData = PluginFunctionForOutGoingMessage(strPlugin,pIncomingMsgDataList);		
			  stMsgQueue.oslSrcId = ModeType;
			  stMsgQueue.oslDestId = PRT_EVENT;
			  stMsgQueue.oslMsgId = clMessage->GetMessageId();
			  stMsgQueue.oslDataPtr = pData;
			  stMsgQueue.oslPeerBuffPtr = NULL;		
			  myNSRThread->WriteToSocket(&stMsgQueue);

        //stMsgQueue.oslSrcId = MOD_L4C_2;
        //myNSRThread->WriteToSocket(&stMsgQueue);
      }
		}
	}
}

void CMainSimulator::SendAsyncMessageToMMI(U32 ModeType, CString strRecvd)
{
  CPtrList *plstAsyncMsg = m_pMessageManager->m_plstAsyncMsg;
#ifdef _DUAL_SIM_
  if (ModeType == MOD_PST_2)
  {
    plstAsyncMsg = m_pMessageManager->m_plstAsyncMsg2;
  }
#endif /*_DUAL_SIM_*/

  if (m_ptrAyncEventsList)
  {
    delete m_ptrAyncEventsList;
  }
  m_ptrAyncEventsList = GetAllAsyncEventsForApp(plstAsyncMsg, strRecvd);
	for( POSITION pos = m_ptrAyncEventsList->GetHeadPosition(); pos != NULL;)
	{
		SendMessageToMMI((CMessage*)(m_ptrAyncEventsList->GetNext( pos )),_T(""),ModeType);
	}
}

CPtrList* CMainSimulator::GetAllAsyncEventsForApp(CPtrList *plstAsyncMsg, CString strRecvd)
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
            ptrList->AddTail(m_pMessageManager->LocateMessageInList(m_pMessageManager->m_plstOutGoingMsg,messageList.GetNext(posResp)));	
            
          }					
        }
        if(ptrList->GetCount() <= 0)
          AMLOGERROR(_T("Inside GetAllAsync Messages for App no found error"));
      }				
    }
  }	
  return  ptrList;
}

void CMainSimulator::SendMessageFreq(CMessage *clMessage, CString strDataList)
{
  UINT32 iSize = 0;
  CString strPlugin = clMessage->GetPluginName();
  MYQUEUE stMsgQueue;
  
  
	if(strPlugin ==_T("")) // frequency is 1 and no plugin
  {
    void *pData = NULL;
    pData = GetDataMemoryForMessage(clMessage,&iSize,strDataList);
    if(pData!=NULL)  
    {
      stMsgQueue.oslSrcId = MOD_PST;
      stMsgQueue.oslDestId = MOD_PRT;
      stMsgQueue.oslMsgId = clMessage->GetMessageId();
      
      stMsgQueue.oslDataPtr = pData;
      stMsgQueue.oslPeerBuffPtr = NULL;
      
      myNSRThread->WriteToSocket(&stMsgQueue);	
    }
    else
      AMLOGINFO(_T("Memory NULL Buffer error for Outgoing Message"));
  }			
  else
  {
    //call the pluggin straightaway
    void *pData = PluginFunctionForOutGoingMessage(strPlugin);		
    stMsgQueue.oslSrcId = MOD_PST;
    stMsgQueue.oslDestId = MOD_PRT;
    stMsgQueue.oslMsgId = clMessage->GetMessageId();
    stMsgQueue.oslDataPtr = pData;
    stMsgQueue.oslPeerBuffPtr = NULL;
    myNSRThread->WriteToSocket(&stMsgQueue);	
  }
}

void CMainSimulator::SendMessageFreq(CMessage *clMessage, CData *dataset)
{
  UINT32 iSize = 0;
	MYQUEUE stMsgQueue;
	void *pData = NULL;
	CFormat * format_for_msg_to_be_sent =m_pMessageManager->GetFormatForMessage(clMessage);
	CList<CVar,CVar&>& varlist_for_msg_to_be_sent = format_for_msg_to_be_sent->GetVarList();
	pData = m_pMessageManager->GetBufferForSocket(clMessage,*dataset,varlist_for_msg_to_be_sent,&iSize);
	if(pData!=NULL)  
	{
		stMsgQueue.oslSrcId = MOD_PST;
		stMsgQueue.oslDestId = MOD_PRT;
		stMsgQueue.oslMsgId = clMessage->GetMessageId();
		stMsgQueue.oslDataPtr = pData;
		stMsgQueue.oslPeerBuffPtr = NULL;
		myNSRThread->WriteToSocket(&stMsgQueue);	
	}
	else
	AMLOGINFO(_T("Memory NULL Buffer error for Outgoing Message"));	
}

void CMainSimulator::CreateTimer(SendMessageDetails *messagedetailpacket)
{
  UINT32 Timer;
	messagedetailpacket->TimerId = m_pMessageManager->m_TimerList->GetCount()+1; 
	 m_pMessageManager->m_TimerList->AddTail(messagedetailpacket);
	Timer = messagedetailpacket->Timer;
	if (Timer ==0)// because timer can never be 0 for freq >1 
		Timer =1;

  ASSERT(m_pWnd);
	m_pWnd->SetTimer(messagedetailpacket->TimerId, Timer*1000,0); 
}

void* CMainSimulator::GetDataMemoryForMessage(CMessage *pMessage, UINT32 *iSize, CString strDataSet)
{
  ASSERT(m_pMessageManager);
  void *vTempRet= NULL;
  //return the void * for the 
  CFormat *pFormat;// = new CFormat();	
  CDataList dataList;	
  CData dataObj;		
  pFormat = m_pMessageManager->GetFormatForMessage(pMessage);
  if(pFormat!=NULL)
  {
    if(!(pMessage->GetFormat().CompareNoCase(pFormat->GetName())))				
    {
      //the format is found 
      //now check if the input mode is dialog and also the dataset name is not there...then we 
      //have to pop up a generic dialog...
      
      //if input mode is dialog
      // Dialog will pop up
      if((m_IsClickOnIncomingMsgList==1)&&(m_RsponseMode==RSP_MANUAL)&& (!pFormat->GetInputMode().CompareNoCase(_T("Dialog"))))
      {
        /*
        CGenericInput gen_input_dlg;
        INPTRLIST * IncomingMessagePtr = NULL;
        
        if(m_pIncomingMsgDataList->GetCount())
        {
          
          INPTRLIST *IncomingMsg;
          
          for(POSITION pos = m_pIncomingMsgDataList->GetHeadPosition();pos!=NULL;m_pIncomingMsgDataList->GetNext(pos))
          {
            IncomingMsg = (INPTRLIST *) m_pIncomingMsgDataList->GetAt(pos);
            if(IncomingMsg->stMessage.nMsgType==m_ClickedMsgId)
            {
              //IncomingMessagePtr = (INPTRLIST *) m_pIncomingMsgDataList->GetTail() ;
              IncomingMessagePtr = (INPTRLIST *) m_pIncomingMsgDataList->GetAt(pos);
              break;
            }	
          }
        }
        
        gen_input_dlg.OutGoingMsgPtr = (COutgoingMessage *)pMessage; 
        if(IncomingMessagePtr)
          gen_input_dlg.IncomingMsgID =  IncomingMessagePtr->stMessage.nMsgType;
        gen_input_dlg.SetLibraryIdentifier(library_identifier);
        gen_input_dlg.DoModal();
        IsClickOnIncomingMsgList=0;
        if(!gen_input_dlg.data_set_to_be_sent.GetName().CompareNoCase(_T("")))
          return(NULL);
        vTempRet = m_pMessageManager->GetBufferForSocket(pMessage,gen_input_dlg.data_set_to_be_sent,m_pMessageManager->GetFormatForMessage(gen_input_dlg.OutGoingMsgPtr)->GetVarList(),iSize);
        return vTempRet;
        */
      }
      else // if input mode is dataset
      {
        CList<CVar,CVar&>& varList = pFormat->GetVarList();
        dataList = pFormat->GetDataList();
        CList<CData,CData&>& data= dataList.GetData();				
        if((data.GetCount() > 0) && (varList.GetCount() > 0))
        {					
          for( POSITION datapos = data.GetHeadPosition(); datapos != NULL;)
          {
            dataObj = (data.GetNext( datapos ));
            if(strDataSet.GetLength())
            {
              if(!dataObj.GetName().CompareNoCase(strDataSet))
              {
                vTempRet = m_pMessageManager->GetBufferForSocket(pMessage,dataObj,varList,iSize);//matched the data list
                return vTempRet;
              }
            }
            else if(!dataObj.GetName().CompareNoCase(dataList.GetDefault()))
            {
              //default data object stuffing to be done
              vTempRet = m_pMessageManager->GetBufferForSocket(pMessage,dataObj,varList,iSize);
              return vTempRet;
            }
            
          }
          //handle the case in case both the above condition becomes false
          vTempRet = m_pMessageManager->GetBufferForSocket(pMessage,dataObj,varList,iSize);
          return vTempRet;
          
        }//end of type check
      }//end of check dialog
    }//		
  }
  else
    AMLOGERROR(_T("GetData Memory for  Message error"));
  return  vTempRet;
}

void* CMainSimulator::PluginFunctionForOutGoingMessage(CString pLugin, CPtrList *pIncomingMsgDataList)
{
  void *pData = NULL;
	if(!pLugin.CompareNoCase(_T("CreateMsgNetWrkAttach")))
	{
		//call the plugin name for the network attach function 
//		CreateMsgNetWrkAttach();
	}
	else if(!pLugin.CompareNoCase(_T("HandleGetAllEntriesRequest")))
	{
		pData = RetDataPointerForInMessage(1484,pIncomingMsgDataList);
		if(pData)
		{
			SetBufferForPbook(pData);
		pData = HandleGetAllEntriesRequest();
	}
	}
	else if(!pLugin.CompareNoCase(_T("HandleWriteEntryRequest")))
	{
		pData = RetDataPointerForInMessage(1490,pIncomingMsgDataList);
		if(pData)
		{
			SetBufferForPbook(pData);
		pData = HandleWriteEntryRequest();
	}
	}
	else if(!pLugin.CompareNoCase(_T("HandleDeleteEntryRequest")))
	{
		pData = RetDataPointerForInMessage(1492,pIncomingMsgDataList);
		if(pData)
		{
			SetBufferForPbook(pData);
		pData = HandleDeleteEntryRequest();
	}
	}
	else if(!pLugin.CompareNoCase(_T("HandleSearchEntryRequest")))
	{
		pData = RetDataPointerForInMessage(1482,pIncomingMsgDataList);
		if(pData)
		{
			SetBufferForPbook(pData);
		pData = HandleSearchEntryRequest();
	}
	}
	else if(!pLugin.CompareNoCase(_T("HandleGetStorageRequest")))
	{
		//SetBufferForPbook(RetDataPointerForInMessage(109,pIncomingMsgDataList));
		pData = HandleGetStorageRequest();
	}
	else if(!pLugin.CompareNoCase(_T("HandleSetStorageRequest")))
	{
		pData = RetDataPointerForInMessage(1486,pIncomingMsgDataList);
		if(pData)
		{
			SetBufferForPbook(pData);
		pData = HandleSetStorageRequest();
	}
	}
        
	else if(!pLugin.CompareNoCase(_T("satsetupmenuplg")))
	{
		pData=m_SAT.SetupMenuPlugin();
		
	}
	else if(!pLugin.CompareNoCase(_T("satselectitemplg")))
	{
		pData=m_SAT.SelectItemPlugin();
		
	}
	else if(!pLugin.CompareNoCase(_T("satdisplaytxtplg")))
	{
		pData=m_SAT.DisplayTextPlugin();
		
	}
	else if(!pLugin.CompareNoCase(_T("satgetinkeyplg")))
	{
		pData=m_SAT.GetInkeyPlugin();
		
	}
	else if(!pLugin.CompareNoCase(_T("satgetinputplg")))
	{
		pData=m_SAT.GetInputPlugin();
		
	}
	else if(!pLugin.CompareNoCase(_T("SendSMSContectByIndex")))
	{
		SetBufferForMessage(RetDataPointerForInMessage(1519,pIncomingMsgDataList));
		pData = SendSMSContectByIndex(SIM1);
	}
	else if(!pLugin.CompareNoCase(_T("SIM2 SendSMSContectByIndex")))
	{
		SetBufferForMessage(RetDataPointerForInMessage(1519,pIncomingMsgDataList));
		pData = SendSMSContectByIndex(SIM2);
	}
	else if(!pLugin.CompareNoCase(_T("SendGetVoicemailAddressRsp")))
	{
		pData = SendGetVoicemailAddressRsp();
	}
	else if(!pLugin.CompareNoCase(_T("SendSetVoicemailAddressRsp")))
	{
		pData = RetDataPointerForInMessage(1547,pIncomingMsgDataList);
		if(pData)
		{
			SetBufferForMessage(pData);
		pData = SendSetVoicemailAddressRsp();
	}
	}
	else if(!pLugin.CompareNoCase(_T("SendSMSToMMI")))
	{
		//SetBufferForMessage(RetDataPointerForInMessage(1531));
		pData = SendSMSToMMI(SIM1);
	}
#ifdef _DUAL_SIM_
	else if(!pLugin.CompareNoCase(_T("SIM2 SendSMSToMMI")))
	{
		//SetBufferForMessage(RetDataPointerForInMessage(1531));
		pData = SendSMSToMMI(SIM2);
	}
#endif /*_DUAL_SIM_*/
	else if(!pLugin.CompareNoCase(_T("SendSmsMemStatus")))
	{
		pData = SendSmsMemStatus();
	}
	else if(!pLugin.CompareNoCase(_T("SendMessageNumber")))
	{
		//SetBufferForMessage(RetDataPointerForInMessage(1503));
		pData = SendMessageNumber();
	}
  else if(!pLugin.CompareNoCase(_T("DeleteSMSRequest")))
  {
    pData = RetDataPointerForInMessage(1517,pIncomingMsgDataList);
    if(pData)
    {
      SetBufferForMessage(pData);
      pData = DeleteSMSRequest(SIM1);
    }
  }
  else if(!pLugin.CompareNoCase(_T("SIM2 DeleteSMSRequest")))
  {
    pData = RetDataPointerForInMessage(1517,pIncomingMsgDataList);
    if(pData)
    {
      SetBufferForMessage(pData);
      pData = DeleteSMSRequest(SIM2);
    }
  }
  else if(!pLugin.CompareNoCase(_T("SaveSMSMessage")))
  {
    pData = RetDataPointerForInMessage(1523,pIncomingMsgDataList);
    if(pData)
    {
      SetBufferForMessage(pData);
      pData = SaveSMSMessage(SIM1);
    }
  }
  else if(!pLugin.CompareNoCase(_T("SIM2 SaveSMSMessage")))
  {
    pData = RetDataPointerForInMessage(1523,pIncomingMsgDataList);
    if(pData)
    {
      SetBufferForMessage(pData);
      pData = SaveSMSMessage(SIM2);
    }
  }
	else if(!pLugin.CompareNoCase(_T("GetMemoryStorage")))
	{
		//SetBufferForMessage(RetDataPointerForInMessage(1507));
		pData = GetMemoryStorage();
	}
	else if(!pLugin.CompareNoCase(_T("SetMemoryStorage")))
	{
		SetBufferForMessage(RetDataPointerForInMessage(1525,pIncomingMsgDataList));
		pData = SetMemoryStorage();
	}        
  else if(!pLugin.CompareNoCase(_T("SetLastNoRsp")))
  {
    pData = RetDataPointerForInMessage(1500,pIncomingMsgDataList);
    if(pData)
    {
      SetBufferForCallHistory(pData);
      pData = SetLastNoRsp(SIM1);
    }
  }
  else if(!pLugin.CompareNoCase(_T("SIM2 SetLastNoRsp")))
  {
    pData = RetDataPointerForInMessage(1500,pIncomingMsgDataList);
    if(pData)
    {
      SetBufferForCallHistory(pData);
      pData = SetLastNoRsp(SIM2);
    }
  }
  else if(!pLugin.CompareNoCase(_T("GetLastNoRsp")))
  {
    pData = RetDataPointerForInMessage(1498,pIncomingMsgDataList);
    if(pData)
    {
      SetBufferForCallHistory(pData);
      pData = GetLastNoRsp(SIM1);
    }
	}
  else if(!pLugin.CompareNoCase(_T("SIM2 GetLastNoRsp")))
  {
    pData = RetDataPointerForInMessage(1498,pIncomingMsgDataList);
    if(pData)
    {
      SetBufferForCallHistory(pData);
      pData = GetLastNoRsp(SIM2);
    }
	}
  else if(!pLugin.CompareNoCase(_T("DeleteAllCallsRsp")))
  {
    pData = RetDataPointerForInMessage(1502,pIncomingMsgDataList);
    if(pData)
    {
      SetBufferForCallHistory(pData);
      pData = DeleteAllCallsRsp(SIM1);
    }
  }
  else if(!pLugin.CompareNoCase(_T("SIM2 DeleteAllCallsRsp")))
  {
    pData = RetDataPointerForInMessage(1502,pIncomingMsgDataList);
    if(pData)
    {
      SetBufferForCallHistory(pData);
      pData = DeleteAllCallsRsp(SIM2);
    }
  }
	else if(!pLugin.CompareNoCase(_T("IncomingCallInd")))
	{
		pData = IncomingCallInd();
	}
	else if(!pLugin.CompareNoCase(_T("CheckSecurityBootup")))
	{	
		pData = RetDataPointerForInMessage(1444,pIncomingMsgDataList);
		if(pData)
		{
			SetBufferForSettings(pData);
		pData = CheckSecurityBootup();
	}
	}
	else if(!pLugin.CompareNoCase(_T("SettingGPRSGetConnectType")))
	{
		pData = SettingGPRSGetConnectType();
	}
	else if(!pLugin.CompareNoCase(_T("SettingGPRSSetConnectType")))
	{	
		pData = RetDataPointerForInMessage(3011,pIncomingMsgDataList);
		if(pData)
		{
			SetBufferForSettings(pData);
			pData = SettingGPRSSetConnectType();
		}
	}

	else if(!pLugin.CompareNoCase(_T("GetCallPriceUnit")))
	{
		pData = GetCallPriceUnit();
	}
	else if(!pLugin.CompareNoCase(_T("SetCallPriceUnit")))
	{	
		pData = RetDataPointerForInMessage(1456,pIncomingMsgDataList);
		if(pData)
		{
			SetBufferForMessage(pData);
			pData = SetCallPriceUnit();
		}
	}
	else if(!pLugin.CompareNoCase(_T("SettingGetNetworkBand")))
	{
		pData = SettingGetNetworkBand();
	}
	else if(!pLugin.CompareNoCase(_T("SettingSetNetworkBand")))
	{	
		pData = RetDataPointerForInMessage(1440,pIncomingMsgDataList);
		if(pData)
		{
			SetBufferForSettings(pData);
			pData = SettingSetNetworkBand();
		}
	}
	else if(!pLugin.CompareNoCase(_T("BootupFunctions")))
	{	
		pData = RetDataPointerForInMessage(1448,pIncomingMsgDataList);
		SetBufferForSettings(pData);
		pData = BootupFunctions();
	}
	else if(!pLugin.CompareNoCase(_T("SMSBootUpBeginInd")))
	{
						pData = SMSBootUpBeginInd(SIM1);
	}
	else if(!pLugin.CompareNoCase(_T("SIM2 SMSBootUpBeginInd")))
	{
						pData = SMSBootUpBeginInd(SIM2);
	}
	else if(!pLugin.CompareNoCase(_T("SMSSendBootupReadInd")))
	{
		SetBufferForMessage(RetDataPointerForInMessage(1556,pIncomingMsgDataList));
		pData = SMSSendBootupReadInd(SIM1);
	}
	else if(!pLugin.CompareNoCase(_T("SIM2 SMSSendBootupReadInd")))
	{
		SetBufferForMessage(RetDataPointerForInMessage(1556,pIncomingMsgDataList));
		pData = SMSSendBootupReadInd(SIM2);
	}
	else if(!pLugin.CompareNoCase(_T("SendSMSResponse")))
	{
		SetBufferForMessage(RetDataPointerForInMessage(1521,pIncomingMsgDataList));
		pData = SendSMSResponse();
	}
		else if(!pLugin.CompareNoCase(_T("CheckSimLock")))
		pData = CheckSimLock();
	else if(!pLugin.CompareNoCase(_T("GetProfileParamsRsp")))
		pData = GetProfileParamsRsp(SIM1);
	else if(!pLugin.CompareNoCase(_T("SIM2 GetProfileParamsRsp")))
		pData = GetProfileParamsRsp(SIM2);
	else if(!pLugin.CompareNoCase(_T("SetProfileSettingParamsRsp")))
	{
		pData = RetDataPointerForInMessage(1507,pIncomingMsgDataList);
		if(pData)
		{
			SetBufferForMessage(pData);
			pData = SetProfileSettingParamsRsp();
		}
	}
	else if(!pLugin.CompareNoCase(_T("GetPhoneLockFunc")))
	{
		pData = RetDataPointerForInMessage(1460,pIncomingMsgDataList);
		if(pData)
		{
			pData = GetPasswordData(pData);
		}
	}
	else if(!pLugin.CompareNoCase(_T("SendCBMessageSetting")))
	{			
		pData = SendCBMessageSetting();
	}

	return pData;
}

void* CMainSimulator::RetDataPointerForInMessage(int iMsgId, CPtrList *pIncomingMsgDataList)
{
  ASSERT(pIncomingMsgDataList);
  if (pIncomingMsgDataList)
  {
    INPTRLIST *stPtrListIncomingMsg;	
	  for( POSITION pos = pIncomingMsgDataList->GetHeadPosition(); pos != NULL; )
	  {			
		  stPtrListIncomingMsg = (INPTRLIST*)(pIncomingMsgDataList->GetNext( pos ));
		  if(stPtrListIncomingMsg->stMessage.nMsgType == iMsgId)
		  {
			  return(stPtrListIncomingMsg->stMessage.nDataPtr);//removed the entry as message is send
			  //also call the function to set the status or icon for disabling the entry
			  AMLOGINFO(_T("Found the entry for incoming message data list buffer"));
		  }		
	  }
  }

	AMLOGINFO(_T("No Entry for Buffer for incoming message data list buffer"));
	return NULL;
}

void CMainSimulator::DisplayMessageInList(MYQUEUE1 msg, BOOL Flag, int nEventMode)
{
  CPtrList *plstIncomingMsg = m_pMessageManager->m_plstIncomingMsg;
  CPtrList *plstOutGoingMsg = m_pMessageManager->m_plstOutGoingMsg;
  CPtrList *pIncomingMsgDataList = this->m_pIncomingMsgDataList;
#ifdef _DUAL_SIM_
  if (nEventMode == MOD_PST_2)
  {
    plstIncomingMsg = m_pMessageManager->m_plstIncomingMsg2;
    plstOutGoingMsg = m_pMessageManager->m_plstOutGoingMsg2;
    pIncomingMsgDataList = this->m_pIncomingMsgDataList2;
  }
#endif /*_DUAL_SIM_*/

  //get the current location from the list 
	//display the current message on the list
	if(Flag)//incoming messsages
	{
		LVITEM stItem;
		int Count= m_lstIncomingMessages->GetItemCount();
		CString strMsgId;
		strMsgId.Format(_T("%d"),msg.nMsgType);
		stItem.mask = LVIF_IMAGE |  LVIF_TEXT;
		stItem.iItem = Count;
		stItem.pszText = strMsgId.GetBuffer(0);
		stItem.cchTextMax = strMsgId.GetLength()+1;
		stItem.iSubItem = 0;
		stItem.state = LVIS_SELECTED;
		stItem.stateMask = LVIS_SELECTED;
		stItem.lParam = m_IndexInIncomingMsgList;
		stItem.iImage = 1;
		int val = m_lstIncomingMessages->InsertItem(&stItem);

		//m_lstIncomingMessages->InsertItem(Count,strMsgId,NULL);
		CString message_name;
		message_name = m_pMessageManager->LocateMessageNameInList(plstIncomingMsg,(UINT)msg.nMsgType);
		if(!message_name.CompareNoCase(_T("")))
		{
			LoadString(library_identifier,IDS_STRING_MSG_NAME_NOT_SUPPORTED,message_name.GetBuffer(MAX_LANG_RES_STRING_LEN+1),MAX_LANG_RES_STRING_LEN);
		}
		m_lstIncomingMessages->SetItemText(Count,1,message_name);
		strMsgId = GetSystemTimeString();
		m_lstIncomingMessages->SetItemText(Count,2,strMsgId);
		//m_lstIncomingMessages->SetItemText(Count,2,_T("haa haai haai haai"));
		AddMessageToList(m_IncomingMsg,pIncomingMsgDataList);//addd the item in the CList
		m_IndexInIncomingMsgList++;//add and then increment
		m_lstIncomingMessages->SetItemState(val,LVIS_SELECTED,LVIS_SELECTED);
		m_lstIncomingMessages->EnsureVisible(val,FALSE);
		
	}
	else
	{
		int Count= m_lstOutGoingMessages->GetItemCount();			
		CString strMsgId;					
		strMsgId.Format(_T("%d"),msg.nMsgType);
		int val = m_lstOutGoingMessages->InsertItem(Count,strMsgId,NULL);
		strMsgId = GetSystemTimeString();
		m_lstOutGoingMessages->SetItemText(Count,1,m_pMessageManager->LocateMessageNameInList(plstOutGoingMsg,msg.nMsgType));
		m_lstOutGoingMessages->SetItemText(Count,2,strMsgId);
		m_lstOutGoingMessages->EnsureVisible(val,FALSE);
	}
}

void CMainSimulator::SetTheLibraryIdentifier(HINSTANCE lib_id)
{
  library_identifier = lib_id;
}

CString CMainSimulator::GetSystemTimeString()
{
  CString sTime;
  CTime ct ; 
  ct = CTime::GetCurrentTime();
  sTime.Format
  (_T("%02d/%02d/%02d %02d:%02d:%02d"),ct.GetMonth(),ct.GetDay(),ct.GetYear(),ct.GetHour(),ct.GetMinute(),ct.GetSecond()); 

  return sTime;
}

void CMainSimulator::AddMessageToList(MYQUEUE1 stIncomingMsg, CPtrList *pIncomingMsgDataList)
{
  ASSERT(pIncomingMsgDataList);
  
  CheckDeleteMessageIfExist(stIncomingMsg,pIncomingMsgDataList);
	INPTRLIST *stPtrListIncomingMsg = new INPTRLIST;
	stPtrListIncomingMsg->stMessage = stIncomingMsg;
	stPtrListIncomingMsg->index = m_IndexInIncomingMsgList;
	pIncomingMsgDataList->AddTail(stPtrListIncomingMsg);	
}

void CMainSimulator::CheckDeleteMessageIfExist(MYQUEUE1 stIncomingMsg, CPtrList *pIncomingMsgDataList)
{
  //new function so as to update the message entries in ptrlist
  ASSERT(pIncomingMsgDataList);
	INPTRLIST *stPtrListIncomingMsg = NULL;	
	for( POSITION pos = pIncomingMsgDataList->GetHeadPosition(); pos != NULL; )
	{	
		POSITION posCurrent = pos;
		stPtrListIncomingMsg = (INPTRLIST*)(pIncomingMsgDataList->GetNext( pos ));
		if(stPtrListIncomingMsg->stMessage.nMsgType == stIncomingMsg.nMsgType)
		{
			pIncomingMsgDataList->RemoveAt(posCurrent);//removed the entry as message is send
			//also call the function to set the status or icon for disabling the entry
			AMLOGINFO(_T("Removed the entry from the incoming message data list buffer"));
			break;
		}		
	}
}

BOOL CMainSimulator::InitialiseToolBar()
{
  ASSERT(this->m_pWnd);
  if (!m_MostFreqMsgSeqBar.Create(WS_CHILD | TBSTYLE_TOOLTIPS | CCS_TOP, CRect(10,10,90,25),this->m_pWnd,IDR_MAINFRAME))
  {
    return FALSE;
  }
	this->m_pWnd->RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
  m_MostFreqMsgSeqBar.EnableToolTips(TRUE);
  int button_number = 0;
	//Initialise The MsgId Array
	CPtrList* FreqMsgGroupList = m_pMessageManager->m_pFreqList;
	CFreqUsedMsg* FreqMsg;
  if(FreqMsgGroupList)
	{
	  if(FreqMsgGroupList->GetCount())
	  {
		  for(POSITION pos = FreqMsgGroupList->GetHeadPosition();pos!=NULL;)
		  {
			  FreqMsg = (CFreqUsedMsg*)FreqMsgGroupList->GetNext(pos);
			  break;
		  }
	  }
    CList<UINT32,UINT32>& FreqMsgList = FreqMsg->GetFreqUsedMessageList();
    if( FreqMsgList.GetCount())
    {
      for(POSITION pos = FreqMsgList.GetHeadPosition();pos!=NULL && button_number < MAXBUTTONS;)
      {
        MsgIdArray[button_number] = (UINT32) FreqMsgList.GetNext(pos);
        TRACE1("MSG ID = %d\n",MsgIdArray[button_number]);
        //MostFreqMsgSeqBar.EnableButton(buttons[button_number],0);
        CAsyncMessageSeq *pAsyncMessage;
        if(m_pMessageManager->m_plstAsyncMsg->GetCount() > 0)//check if emtpy message list 
        {	
          for( POSITION pos = m_pMessageManager->m_plstAsyncMsg->GetHeadPosition(); pos != NULL;)
          {
            pAsyncMessage = (CAsyncMessageSeq*)(m_pMessageManager->m_plstAsyncMsg->GetNext( pos )) ;
            if(MsgIdArray[button_number] == pAsyncMessage->GetMessageId())
            {						//enable only when the id is in the asyncseq list
              TBBUTTON struct_button_info;
              struct_button_info.fsState = TBSTATE_ENABLED;
              struct_button_info.fsStyle = TBSTYLE_BUTTON;
              struct_button_info.iBitmap = m_MostFreqMsgSeqBar.AddBitmap(1,15044+MsgIdArray[button_number]);
              struct_button_info.idCommand = buttons[button_number];
              struct_button_info.iString = m_MostFreqMsgSeqBar.AddString(IDS_STRING_EMPTY);
              m_MostFreqMsgSeqBar.AddButtons(1,&struct_button_info);
              button_number++;
              break;
            }
          }
        }
      }
    }
  }

  while(button_number <= MAXBUTTONS)  //disable the rest of the buttons....
  {
    m_MostFreqMsgSeqBar.EnableButton(buttons[button_number],0);     
    button_number++;
  }
  m_MostFreqMsgSeqBar.ShowWindow(SW_SHOW);

  ///////////////停靠控制///////////////////
// 	m_MostFreqMsgSeqBar.SetBarStyle(m_MostFreqMsgSeqBar.GetBarStyle() |
// 		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	//m_MostFreqMsgSeqBar.EnableDocking(CBRS_ALIGN_ANY);

  return TRUE;
}

CToolBarCtrl* CMainSimulator::GetToolBar()
{
  return &this->m_MostFreqMsgSeqBar;
}

BOOL CMainSimulator::OnModifyBootMsg()
{
  CBootMessageDlg dlg;
	dlg.SetLibraryIdentifier(library_identifier);
	dlg.DoModal();

  return TRUE;
}

void CMainSimulator::OnSendMessages()
{
	CSendMessageDlg Dlg;
	Dlg.SetLibrayIdentifier(library_identifier);
	Dlg.DoModal();
}

BOOL CMainSimulator::OnSendAsyncMessage(U32 ModeType, PopUpMsgStruct *pstPopMsg)
{
  CPtrList *plstOutGoingMsg = m_pMessageManager->m_plstOutGoingMsg;
#ifdef _DUAL_SIM_
  if (ModeType == MOD_PST_2)
  {
    plstOutGoingMsg = m_pMessageManager->m_plstOutGoingMsg2;
  }
#endif /*_DUAL_SIM_*/
  if (!pstPopMsg)
  {
    return FALSE;
  }

	if(pstPopMsg->nMsgId ==1)
	{
    CMainSimulator::SendAsyncMessageToMMI(ModeType,pstPopMsg->strMsgResponse);
	}
	else if(pstPopMsg->nMsgId == 2)
	{
		AMLOGINFO(_T("Sending Async Message for Msg id %d"),pstPopMsg->strMsgValue);	
		SendMessageToMMI(m_pMessageManager->LocateMessageInList(plstOutGoingMsg,m_pMessageManager->LocateMessageIdForName(plstOutGoingMsg,pstPopMsg->strMsgResponse)),_T(""),ModeType);
	}
	else		 
	{
		AMLOGINFO(_T("Sending Message for Msg id %d"),pstPopMsg->strMsgValue);	
		SendMessageToMMI(m_pMessageManager->LocateMessageInList(plstOutGoingMsg,m_pMessageManager->LocateMessageIdForName(plstOutGoingMsg,pstPopMsg->strMsgResponse)),pstPopMsg->strMsgValue,ModeType);
	}

	return(TRUE);
}

BOOL CMainSimulator::OnRecieveMessage(int nDestId)
{
  CPtrList *plstIncomingMsg = m_pMessageManager->m_plstIncomingMsg;
  CPtrList *plstOutGoingMsg = m_pMessageManager->m_plstOutGoingMsg;
#ifdef _DUAL_SIM_
  if (nDestId == MOD_PST_2)
  {
    plstIncomingMsg = m_pMessageManager->m_plstIncomingMsg2;
    plstOutGoingMsg = m_pMessageManager->m_plstOutGoingMsg2;
  }
#endif /*_DUAL_SIM_*/

  m_IncomingMsg = myNSWThread->GetMessage();
	AMLOGINFO(_T("Message Recieved by simulator"));		
	AMLOGINFO(_T("Event Type is -> %d & Message id is -> %d"),m_IncomingMsg.nEventType,m_IncomingMsg.nMsgType);	
	DisplayMessageInList(m_IncomingMsg,TRUE,nDestId);
	m_MessageId = m_IncomingMsg.nMsgType;
	CMessage *pMessage = m_pMessageManager->LocateMessageInList(plstIncomingMsg,m_IncomingMsg.nMsgType);
	CIncomingMsg *incomingmsg;
	incomingmsg =(CIncomingMsg*) pMessage;
	CList <UINT32, UINT32>& asyncseqlist = incomingmsg->GetAsyncMessageList(); 
	UINT32 sequenceid;
	if(pMessage)
	{
		//call the plugin if exist
		CString strPluginName = pMessage->GetPluginName();
		if(strPluginName!= _T(""))
		{
			//call the plugin function
			PluginFunctionForMessage(strPluginName,NULL);
                        //commented return so as to handle the case when 
                        //incoming msg has a plugin as well as response is auto
			//return(TRUE);
		}
	}
	//send the message if application is auto that is send it in the background if its auto
	if(pMessage!=NULL)
	{
		if(!(CString("auto").CompareNoCase(pMessage->GetResponseMode()))) // If response mode is auto
		{	
			CPtrList *ptRespList = GetAllResponseMsgs(pMessage->GetMessageId(),plstIncomingMsg,plstOutGoingMsg);
			CMessage *pOutGoingMsg;
			if(ptRespList->GetCount() > 0)//this means response message exist
			{
				for( POSITION pos = ptRespList->GetHeadPosition(); pos != NULL; )
				{			
					pOutGoingMsg = (COutgoingMessage*)(ptRespList->GetNext( pos )) ;
					pMessage = m_pMessageManager->LocateMessageInList(plstOutGoingMsg,pOutGoingMsg->GetMessageId());					
					if(pMessage!=NULL)
						SendMessageToMMI(pMessage,_T(""),nDestId);
				}
			}
			/*------------ Check if there is async sequence ------------------*/
			if(asyncseqlist.GetCount() > 0)//this means response message exist
			{
				for( POSITION pos = asyncseqlist.GetHeadPosition(); pos != NULL; )
				{			
					//evaluating the sequence id
					sequenceid= (UINT32)(asyncseqlist.GetNext( pos )) ;
					//sending the async sequence
					SendMsgSequence(nDestId,sequenceid);
				}


			}

		}
	}
	return(TRUE);
}

BOOL CMainSimulator::OnSendMessage(MYQUEUE1 *pOutGoingMsg)
{
	AMLOGINFO(_T("Message Send by simulator"));		
	AMLOGINFO(_T("Event Type is -> %d & Message id is -> %d"),pOutGoingMsg->nEventType,pOutGoingMsg->nMsgType);
	DisplayMessageInList(*pOutGoingMsg,FALSE,pOutGoingMsg->nEventType);

  CPtrList *pIncomingMsgDataList = this->m_pIncomingMsgDataList;
#ifdef _DUAL_SIM_
  if (pOutGoingMsg->nEventType == MOD_PST_2)
  {
    pIncomingMsgDataList = this->m_pIncomingMsgDataList2;
  }
#endif /*_DUAL_SIM_*/
  RetDataPointerForInMessage((pOutGoingMsg->nMsgType)-1,pIncomingMsgDataList); // Modify by zhuoxz, 2009-9-30

  return(TRUE);
}

void CMainSimulator::SetReadQueueHandle(oslMsgqid handle)
{
  CNSWThread::m_QHandle = handle;
}

void CMainSimulator::SetWriteQueueHandle(oslMsgqid handle)
{
  CNSRThread::m_QHandle = handle;
}

void CMainSimulator::SetReadQueueHandler(NWRECEIVEQ pRecvHandler)
{
  CNSWThread::m_pReceiveQ = pRecvHandler;
}

void CMainSimulator::SetWriteQueueHandler(NWWRITEQ pWriteHandler)
{
  CNSRThread::m_pWriteQ = pWriteHandler;
}

CString CMainSimulator::GetSeqNameForButton(UINT32 nID)
{
  for(int index= 0; index<10; index++)
  {
    if(nID == buttons[index])
    {
      break;
    }
  }
  int to_be_sent_msg_id = MsgIdArray[index];
  CAsyncMessageSeq *pAsyncMessage;
  if(m_pMessageManager->m_plstAsyncMsg->GetCount() > 0)//check if emtpy message list 
  {	
    for( POSITION pos = m_pMessageManager->m_plstAsyncMsg->GetHeadPosition(); pos != NULL;)
    {
      pAsyncMessage = (CAsyncMessageSeq*)(m_pMessageManager->m_plstAsyncMsg->GetNext( pos )) ;
      if((UINT32)to_be_sent_msg_id == pAsyncMessage->GetMessageId())
      {
        break;
      }
    }
  }
  if((UINT32)to_be_sent_msg_id == pAsyncMessage->GetMessageId())
  {
    return(pAsyncMessage->GetMessageName());
  }
  else
  {
    return (CString(_T("")));
  }

}

void CMainSimulator::PluginFunctionForMessage(CString pLugin, CPtrList *pIncomingMsgDataList)
{
	//this function has the case switch for the id passed
	if(!pLugin.CompareNoCase(_T("CreateMsgNetWrkAttach")))
	{
		//call the plugin name for the network attach function 
//		CreateMsgNetWrkAttach();
	}
	else if(!pLugin.CompareNoCase(_T("AudioPlayReq")))
	{
		//AudioPlayReq(RetDataPointerForInMessage(1320)); //zhuoxz,2008-09-24
	}
	else if(!pLugin.CompareNoCase(_T("AudioStopReq")))
	{
		//AudioStopReq();   //zhuoxz,2008-09-24
	}
  else if(!pLugin.CompareNoCase(_T("SendSMSResponse")))
	{
		SetBufferForMessage(RetDataPointerForInMessage(1522,pIncomingMsgDataList));
		SendSMSResponse();
	}
	else if(!pLugin.CompareNoCase(_T("CheckPinBS")))
	{
		SetBufferForSettings(RetDataPointerForInMessage(1448,pIncomingMsgDataList));
		CheckPinBS(MOD_PST);
	}
	else if(!pLugin.CompareNoCase(_T("AudioPlayIMelodyStringReq")))
	{
		//AudioPlayIMelodyStringReq(RetDataPointerForInMessage(1324));
	}
	else if(!pLugin.CompareNoCase(_T("AudioPlayIMelodyFileReq")))
	{
		//AudioPlayIMelodyFileReq(RetDataPointerForInMessage(1329));
	}
	else if(!pLugin.CompareNoCase(_T("SaveCBMessageSetting")))
	{
		SetBufferForMessage(RetDataPointerForInMessage(1561,pIncomingMsgDataList));
		SaveCBMessageSetting();
	}

}

CPtrList* CMainSimulator::GetAllResponseMsgs(UINT16 iMsgId, CPtrList *plstIncomingMsg, CPtrList *plstOutGoingMsg)
{
	CIncomingMsg *pIncMessage;	
	CPtrList *lstRespMsg = new CPtrList();
	if(plstIncomingMsg->GetCount())//check if emtpy message list 
	{	
		for( POSITION pos = plstIncomingMsg->GetHeadPosition(); pos != NULL; )
		{			
			pIncMessage = (CIncomingMsg*)(plstIncomingMsg->GetNext( pos )) ;
			if(pIncMessage->GetMessageId() == iMsgId)
			{
				CList<UINT32, UINT32>& lstRespId = pIncMessage->GetResponseMessageList();
				//iResponse = pIncMessage->m_RespMsgList.GetCount(); 
				if(lstRespId.GetCount() > 0)
				{	
					for( POSITION posResp = lstRespId.GetHeadPosition(); posResp != NULL; )
					lstRespMsg->AddTail(m_pMessageManager->LocateMessageInList(plstOutGoingMsg,lstRespId.GetNext(posResp)));
				}				
				break;
			}
		}
		AMLOGINFO(_T("Response Messages for --%d-- are--> %d"),iMsgId,lstRespMsg->GetCount());
	}
//	else
//		theApp.m_LogTrace.WriteLine(_T("Inside GetAllResponseMsgs no entry found error"));				
	return lstRespMsg;	
}

void CMainSimulator::SendMsgSequence(U32 ModeType, UINT32 MessageSequenceId)
{
  CPtrList *plstAsyncMsg = m_pMessageManager->m_plstAsyncMsg;
#ifdef _DUAL_SIM_
  if (ModeType == MOD_PST_2)
  {
    plstAsyncMsg = m_pMessageManager->m_plstAsyncMsg2;
  }
#endif /*_DUAL_SIM_*/
	CAsyncMessageSeq *pAsyncMessage;
	if(plstAsyncMsg->GetCount() > 0)//check if emtpy message list 
	{	
		for( POSITION pos = plstAsyncMsg->GetHeadPosition(); pos != NULL;)
		{
			pAsyncMessage = (CAsyncMessageSeq*)(plstAsyncMsg->GetNext( pos )) ;
			if(pAsyncMessage->GetMessageId()==MessageSequenceId)
				break;
		}
	}
	SendAsyncMessageToMMI(ModeType,pAsyncMessage->GetMessageName());
}
