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

	FILENAME	: SendMessage.cpp

  	PURPOSE		: Declaration file.

	REMARKS		: nil

	AUTHOR		: Gurdev

	DATE		: Nov,04

**************************************************************/

#include "stdafx.h"
#include "MessageManager.h"
#include "Message.h"
#include "Format.h"
#include "Data.h"
#include "DataList.h"
#include "DataType.h"
#include "TaskInit.h"
#include "NSRThread.h"
#include "GlobalConstants.h"
#include "NWGCMLHandler.h"
#include "XMLVerifier.h"
extern void* MyMallocDataPtr(U32 size);
extern CNSRThread *myNSRThread;

CPtrList* CMessageManager::m_pFormatList = NULL;		// This declaration is 
CPtrList* CMessageManager::m_plstOutGoingMsg = NULL;	// because their storage class is static
CPtrList* CMessageManager::m_plstIncomingMsg = NULL;	
CPtrList* CMessageManager::m_plstAsyncMsg = NULL;		
CPtrList* CMessageManager::m_pAppList = NULL;			
CPtrList* CMessageManager::m_pBootupList = NULL;		
CPtrList* CMessageManager::m_pFreqList = NULL;			
CPtrList*	CMessageManager::m_pDataList = NULL;
CPtrList* CMessageManager::m_TimerList = NULL;	
CStdioFile CMessageManager::m_fErrorFile;
CString   CMessageManager::m_XMLErrors;
int     CMessageManager::AppIdErrorPrintedStatus;
CString CMessageManager::AppIdError[MAX_APPLICATIONS];
CString CMessageManager::m_strXMLFile = 	_T("");

// Add by zhuoxz, 2009-09-28
#ifdef _DUAL_SIM_
CPtrList* CMessageManager::m_plstOutGoingMsg2 = NULL;
CPtrList* CMessageManager::m_plstIncomingMsg2 = NULL;
CPtrList* CMessageManager::m_plstAsyncMsg2 = NULL;
CPtrList* CMessageManager::m_pBootupList2 = NULL;
#endif /*_DUAL_SIM_*/

CMessageManager::CMessageManager(void)
{
  m_IncomingMsgDataList = NULL;
}


CMessageManager::~CMessageManager(void)
{
}

BOOL CMessageManager::InitializeXMLLists(CString strMessageFile)
{
	CFile fCFile;
	if(fCFile.Open(strMessageFile,CFile::modeRead)==FALSE) 
	{
		AfxMessageBox(CString("Unable to open XML file, Not exist file"));
		return FALSE;
	}
	else
	{
		fCFile.Close();
	}

  CMessageManager::m_strXMLFile = strMessageFile;
	CNWGCMLHandler GCMLHandler = CNWGCMLHandler((LPTSTR)strMessageFile.LockBuffer());
	MSXML::IXMLDOMDocument *m_pDom;
	if(GCMLHandler.GetDOM(m_pDom) == SUCCESS)
	{
		m_pFormatList		= GCMLHandler.GetFormatsList(); 
		m_plstOutGoingMsg	= GCMLHandler.GetOutGoingMessageList(TAG_OUTGOINGMSG);
		m_plstIncomingMsg	= GCMLHandler.GetIncomingMessageList(TAG_INCOMINGMSG);
		m_plstAsyncMsg		= GCMLHandler.GetAsyncMessageList(TAG_ASYNCMSGSEQ);
		m_pAppList			= GCMLHandler.GetApplicationList(); 
		m_pBootupList		= GCMLHandler.GetBootUpMessageList(TAG_BOOTUPMSG);
		m_pFreqList			= GCMLHandler.GetFrequentlyUsedMessageList();
		m_pDataList			= GCMLHandler.GetDataList();
		m_TimerList			= new CPtrList();
#ifdef _DUAL_SIM_
    m_plstOutGoingMsg2	= GCMLHandler.GetOutGoingMessageList(TAG_OUTGOINGMSG2);
    m_plstIncomingMsg2	= GCMLHandler.GetIncomingMessageList(TAG_INCOMINGMSG2);
    m_plstAsyncMsg2		= GCMLHandler.GetAsyncMessageList(TAG_ASYNCMSGSEQ2);
    m_pBootupList2		= GCMLHandler.GetBootUpMessageList(TAG_BOOTUPMSG2);
#endif

		CXMLVerifier xmlVerifier;
		if(xmlVerifier.VerifyLists()!=0)
		{
		  AMLOGINFO(_T("Success In Initialization of XML"));
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		AMLOGERROR(_T("Error in getting the XML initialized"));
		//AfxMessageBox(_T("Error in reading GCML.txt file"));
		return FALSE;
	}
	
  return TRUE;
}

void CMessageManager::ReleaseXMLLists()
{
  DELETE_MEM_ARR(CMessageManager::m_pFormatList);
  DELETE_MEM_ARR(CMessageManager::m_plstOutGoingMsg);
  DELETE_MEM_ARR(CMessageManager::m_plstIncomingMsg);
  DELETE_MEM_ARR(CMessageManager::m_plstAsyncMsg);
  DELETE_MEM_ARR(CMessageManager::m_pAppList);
  DELETE_MEM_ARR(CMessageManager::m_pBootupList);
  DELETE_MEM_ARR(CMessageManager::m_pFreqList);
  DELETE_MEM_ARR(CMessageManager::m_pDataList);

// Add by zhuoxz, 2009-9-28
#ifdef _DUAL_SIM_
  DELETE_MEM_ARR(CMessageManager::m_plstOutGoingMsg2);
  DELETE_MEM_ARR(CMessageManager::m_plstIncomingMsg2);
  DELETE_MEM_ARR(CMessageManager::m_plstAsyncMsg2);
  DELETE_MEM_ARR(CMessageManager::m_pBootupList2);
#endif

  if (m_TimerList)
  {
    delete m_TimerList;
  }
}

int CMessageManager::VerifyLists()
{
	int flagIncomingMsg,flagOutGoingMsg,flagFormatList,flagDataList,flagAsyncMsg,flagFreqlist,flagBootupList;
	flagIncomingMsg=flagOutGoingMsg=flagFormatList=flagDataList=flagAsyncMsg = 1; // 1 means no error
	flagFreqlist = flagBootupList = 1;
	
	flagIncomingMsg = VerifyIncomingMsg();
	flagOutGoingMsg = VerifyOutGoingMsg();
	flagFormatList  = VerifyFormatList();
	flagDataList    = VerifyDataList();
	flagAsyncMsg    = VerifyAsyncMsg();
	flagFreqlist	= VerifyFreqList();
	flagBootupList	= VerifyBootupList();
	if(flagIncomingMsg&&flagOutGoingMsg&&flagFormatList&&flagDataList&&flagAsyncMsg)// 1 means no error
	{
		return 1; //Success, no error
	}
	else  // error is there
	{
		AfxMessageBox(m_XMLErrors);
		if(m_fErrorFile.Open(
			CCoolModule::GetModulePath()+XML_ERROR_FILE_NAME,CFile::modeCreate|CFile::modeWrite|CFile::typeText)==TRUE) 
		{
			int length = m_XMLErrors.GetLength();
			m_fErrorFile.Write((char *)UnicodeToAscii(m_XMLErrors),length);
			m_fErrorFile.Close();
		}
		else
		{
			 AfxMessageBox(CString("Error in File Handling : XMLError file"));
		}
		return 0; // error
	}
}

int CMessageManager::VerifyIncomingMsg()
{	
	//take one message from incomming list
	//check that its AppId is there in the appidlist, traverse the appid list for each
	//check its response message id exists or not, traverse the outgoing message list
	CIncomingMsg *IncMsg;
	CApplication *App_ptr;
	COutgoingMessage *outgoing_msg;
	UINT32 AppId, RspMsgId;
	int AppIdStatus,RspMsgIdStatus,ErrorStatus=0;//will be one even if single error occure 

	if(m_plstIncomingMsg && m_plstIncomingMsg->GetCount())
	{
		for(POSITION pos = m_plstIncomingMsg->GetHeadPosition();pos!=NULL;)
		{
			IncMsg = (CIncomingMsg *) m_plstIncomingMsg->GetNext(pos);
			AppId = IncMsg->GetAppId(); 
			
			// now search for the appid in appid list for existance
			AppIdStatus=0;
			if(m_pAppList && m_pAppList->GetCount())
			{
				for(POSITION pos =  m_pAppList->GetHeadPosition();pos!= NULL;)
				{

     				App_ptr = (CApplication *) (m_pAppList->GetNext (pos));
					if(App_ptr->GetAppId()==AppId)
					{
						AppIdStatus =1; // appid found, its ok
						break; // if found, get out of loop
					}
				}

				if(AppIdStatus==0) // means not found
				{
					//m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[MsgId - ")+LongToCString(IncMsg->GetMessageId())+CString("] Invalid Application Id - ")+LongToCString(AppId)+CString("\n") ;
					AppIdError[AppId] = AppIdError[AppId] +CString(" ")+LongToCString(IncMsg->GetMessageId());
					ErrorStatus=1;
					
				}
			}
			else
			{
				if(AppIdErrorPrintedStatus==0)
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR : Application Id are not defined in XML")+CString("\n");
					ErrorStatus=1;
					AppIdErrorPrintedStatus=1;
				}
			}

			//----------------- now search for Resp message id ---------------------
			//take a list of response message ids
			CList<UINT32, UINT32>& lstRespId = IncMsg->GetResponseMessageList();

			//check if the list is empty
			if(lstRespId.GetCount()) // response messageid list is not empty
			{
				for( POSITION posResp = lstRespId.GetHeadPosition(); posResp != NULL; )
				{			
					
					RspMsgId  = (UINT32) lstRespId.GetNext(posResp);
					RspMsgIdStatus=0;
					if(m_plstOutGoingMsg)
					{
						for(POSITION pos=m_plstOutGoingMsg->GetHeadPosition();pos!=NULL;)
						{
							outgoing_msg = (COutgoingMessage*)m_plstOutGoingMsg->GetNext(pos);
							if(outgoing_msg->GetMessageId() ==RspMsgId)
							{
								RspMsgIdStatus=1; // Response Message id is there, its ok
								break;			// break the loop
							}
						}
						
						if(RspMsgIdStatus==0)
						{
							m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[MsgId - ")+LongToCString(IncMsg->GetMessageId())+CString("] Invalid Response Message Id - ")+LongToCString(RspMsgId)+CString("\n");
							ErrorStatus=1;
						}
					}
					else
					{
						m_XMLErrors = m_XMLErrors+CString("ERROR : Outgoing Message List is empty")+CString("\n");
						ErrorStatus=1;
					}
				}
			}
	else
	{
	
	}
}
	}
	else
	{
		AfxMessageBox(_T("ERROR : Incoming Msg list is Empty"));
		ErrorStatus=1;
	}

	int index;
	for(index=0;index<=MAX_APPLICATIONS-1;index++)
	{
		if(AppIdError[index].GetLength()>0)
		{
			m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[For Incoming Messages - ")+AppIdError[index]+CString(" : Invalid AppId - ")+LongToCString(index)+CString("\n");
		}
		AppIdError[index] = CString(""); // because outgoing varifier also have to fill

	}

	if(ErrorStatus==1)
	{
		
        return 0; // Some error occur
	}
	else
	{
		return 1; // No Error Occure
	}
}

int CMessageManager::VerifyOutGoingMsg()
{	
	//take one message from out going list
	//check that its AppId is there in the appidlist, traverse the appid list for each message
	//check that its format name is there in the formatlist, traverse format list for each message
	COutgoingMessage *OutGoingMsg;
	CApplication *App_ptr;
	CString FormatName,PluginName;
	
	UINT32 AppId;
	int AppIdStatus,FormatNameStatus,ErrorStatus=0;//ErrorStatus will be one even if single error occure 

	if(m_plstOutGoingMsg && m_plstOutGoingMsg->GetCount())
	{
		for(POSITION pos = m_plstOutGoingMsg->GetHeadPosition();pos!=NULL;)
		{
			OutGoingMsg = (COutgoingMessage *) m_plstOutGoingMsg->GetNext(pos);
			AppId = OutGoingMsg->GetAppId(); 
			
			// now search for the appid in appid list for existance
			AppIdStatus=0; 
			if(m_pAppList && m_pAppList->GetCount())
			{
				for(POSITION pos =  m_pAppList->GetHeadPosition();pos!= NULL;)
				{

     				App_ptr = (CApplication *) (m_pAppList->GetNext (pos));
					if(App_ptr->GetAppId()==AppId)
					{
						AppIdStatus =1; // appid found, its ok
						break; // if found, get out of loop
					}
				}

				if(AppIdStatus==0) // means not found
				{
					//m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[MsgId - ")+LongToCString(OutGoingMsg->GetMessageId())+CString("] Invalid Application Id - ")+LongToCString(AppId)+CString("\n") ;
					AppIdError[AppId] = AppIdError[AppId] +CString(" ")+LongToCString(OutGoingMsg->GetMessageId());
					ErrorStatus=1;
					
				}
			}
			else
			{
				if(AppIdErrorPrintedStatus==0) //AppIdErrorPrintedStatus is a member variable
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR : Application Id are not defined in XML")+CString("\n");
					ErrorStatus=1;
					AppIdErrorPrintedStatus=1;
				}
			}
			
			// following is the chech for availability of format name in format list
			FormatName = OutGoingMsg->GetFormat();
			PluginName = OutGoingMsg->GetPluginName();
			if(!FormatName.CompareNoCase(CString("null"))) // If format is Null, that means plugin should be there
			{
				if(PluginName.GetLength()==0)
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[MsgId - ")+LongToCString(OutGoingMsg->GetMessageId())+CString("] Format is \"Null\" and Plugin is not there\n") ;
					ErrorStatus=1;
				}
			}
			else
			{
				if(FormatName.GetLength()) // some format is there and it is not "NULL"
				{
			// Now search this format name in the list of formats
			if(m_pFormatList)
			{
				CFormat *pFormat;
				FormatNameStatus=0;
				for(POSITION pos = m_pFormatList->GetHeadPosition(); pos != NULL;)
				{
					pFormat = (CFormat*)m_pFormatList->GetNext( pos ) ;
					if(!FormatName.CompareNoCase(pFormat->GetName()))				
					{
						FormatNameStatus=1;
						break;
					}
				
				}	
				if(FormatNameStatus==0) // means not found
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[MsgId - ")+LongToCString(OutGoingMsg->GetMessageId())+CString("] Invalid Format Name - ")+FormatName+CString("\n") ;
					ErrorStatus=1;
				}
					}
					else
					{
						m_XMLErrors = m_XMLErrors+CString("ERROR : Format list is Empty\n") ;
						ErrorStatus=1;
					}
				}
				else // format is ""
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[MsgId - ")+LongToCString(OutGoingMsg->GetMessageId())+CString("] Invalid Format entry \"\" \n");
					ErrorStatus=1;
			}

		}
	}
	}
	else
	{
		AfxMessageBox(_T("ERROR : Outgoing Msg list is Empty"));
		ErrorStatus=1;
	}

	int index;
	for(index=0;index<=MAX_APPLICATIONS-1;index++)
	{
		if(AppIdError[index].GetLength()>0)
		{
			m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[For Outgoing Messages - ")+AppIdError[index]+CString(" : Invalid AppId - ")+LongToCString(index)+CString("\n");
		}
	}

	if(ErrorStatus==1)
	{
		return 0; // Some error occur
	}
	else
	{
		return 1; // No Error Occure
	}
}


int CMessageManager::VerifyFormatList()
{
	//get one format from format list
	//find the data list name associated with it
	//Check that datalist name in the list of datalist
	CDataList cdatalist,*ptrdatalist; 
	CString inputmode;
	int DataListNameStatus,ErrorStatus=0;
    
	if(m_pFormatList)
	{
		CFormat *pFormat;
		
		for(POSITION pos = m_pFormatList->GetHeadPosition(); pos != NULL;)
		{
			pFormat = (CFormat*)m_pFormatList->GetNext( pos ) ;
			if((pFormat->GetName()).GetLength()) // if format is there but format name is not defined
			{
                cdatalist = pFormat->GetDataList();
				if((cdatalist.GetName()).GetLength()>0) //Datalist name is there
				
				{
					if(m_pDataList)
					{
						DataListNameStatus=0;
						for(POSITION pos = m_pDataList->GetHeadPosition(); pos != NULL;) //search for the datalist name in data list ptrdatalist
						{
							ptrdatalist = (	CDataList*) m_pDataList->GetNext( pos ) ;
							if(!(cdatalist.GetName()).CompareNoCase(ptrdatalist->GetName()))
							{
								DataListNameStatus=1;
								break;							
							}
						}
						if(DataListNameStatus==0) // Datalist does not exists
						{
							m_XMLErrors = m_XMLErrors+CString("ERROR : [Format - ")+pFormat->GetName()+CString("] DataList Name not found in DataLists\n");
							ErrorStatus=1;
						}


					}
					else // if datlist is not there
					{
					
					}
				}
				else // format is there but datalist name is not there
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR : [Format - ")+pFormat->GetName()+CString("] Invalid DataList Name\n");
					ErrorStatus=1;
				}
				
				
				inputmode = pFormat->GetInputMode(); // get the input mode
				if(inputmode.GetLength()==0) // Input  mode is empty
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR : [Format - ")+pFormat->GetName()+CString("] InputMode not defined\n");
					ErrorStatus=1;
				}
				else
				{
					if((inputmode.CompareNoCase(CString("Dialog"))) && (inputmode.CompareNoCase(CString("Dataset")))) // Input mode is something other than Dialog,Dataset
					{
						m_XMLErrors = m_XMLErrors+CString("ERROR : [Format - ")+pFormat->GetName()+CString("] InputMode can be Dialog or Dataset\n");
						ErrorStatus=1;
					}
				}
			}
			else
			{
				m_XMLErrors = m_XMLErrors+CString("ERROR : [Format - ")+pFormat->GetName()+CString("] Format is defined without name\n");
				ErrorStatus=1;
			}
		}
	}
	else
	{
		m_XMLErrors = m_XMLErrors+CString("ERROR : Format List is not available\n");
		ErrorStatus=1;

	}

	if(ErrorStatus==1)
	{
		return 0; // Some error occur
	}
	else
	{
		return 1; // No Error Occure
	}

			
	
}

int CMessageManager::VerifyDataList()
{
	//take a datalist object from the list of datalist
	//there must be some name for every datalist object
	//there must be some text in the defaulf tag, i can never be "" (empty)
	//if it have some text the two cases are there as follow
	//1.) the text could be "NULL"(<- is tag text), then leave to search it in dataset list, its ok
	//2.) text is not empty, and not "NULL", then there must be some dataset against
	//    this name in the list of dataset

	int ErrorStatus=0;//ErrorStatus will be one even if single error occur
	CDataList cdatalist,*ptrdatalist; 
	int DataSetNameStatus;
	
	CData dataObj;


	if(m_pDataList && m_pDataList->GetCount())
	{
		
		for(POSITION pos = m_pDataList->GetHeadPosition(); pos != NULL;) 
		{
			ptrdatalist = (	CDataList*) m_pDataList->GetNext( pos ); // ptrdatalist  is having first node
		
			if((ptrdatalist->GetName()).GetLength()) // name tag of datalist object itself should be there 
			{
				if((ptrdatalist->GetDefault()).GetLength() >0)
				{
					if((ptrdatalist->GetDefault()).CompareNoCase(CString("NULL")))// data set is not having "NULL" tag
					{
						CList<CData,CData&>& datasetlist = ptrdatalist->GetData(); //data set list, now we have to search that dafault
						       							   // tag in the list of dataset and it should be there	
						if(datasetlist.GetCount()) // Dataset list is there
						{					
							for( POSITION datapos = datasetlist.GetHeadPosition(); datapos != NULL;)
							{
								DataSetNameStatus=0;
								dataObj = (datasetlist.GetNext( datapos ));
								if(!dataObj.GetName().CompareNoCase(ptrdatalist->GetDefault())) // 
								{
									DataSetNameStatus=1;
									break;
								}
							}

							if(DataSetNameStatus==0)
                            {
								m_XMLErrors = m_XMLErrors+CString("ERROR : [DataList - ")+ptrdatalist->GetName()+CString("] Default Dataset is not there in the list of Dataset\n");		
								ErrorStatus=1;
							}
						}
						else
						{
							//m_XMLErrors = m_XMLErrors+CString("ERROR : [DataList - ")+cdatalist.GetName()+CString("] Dataset list is not there\n");
						}

					}
					
				}
				else // default tag is empty
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR : [DataList - ")+ptrdatalist->GetName()+CString("] DataList's Default tag can'nt be Empty\n");
					ErrorStatus=1;
				}
			}
			else // datalist name tag  is not there
			{
				m_XMLErrors = m_XMLErrors+CString("ERROR : DataList is there, but with out name\n");		
				ErrorStatus=1;
			}
		}

	}
	else
	{
		m_XMLErrors = m_XMLErrors+CString("ERROR : DataList is Empty\n");	
		ErrorStatus=1;
	}
	
	if(ErrorStatus==1)
	{
		return 0; // Some error occur
	}
	else
	{
		return 1; // No Error Occure
	}

}

int CMessageManager::VerifyAsyncMsg()
{
	CAsyncMessageSeq *ptrAsyncMsgSeq;
	UINT32 msgid;
	int msgidvalidstatus =0,ErrorStatus=0;
	COutgoingMessage *outgoing_msg; 
	for(POSITION pos = m_plstAsyncMsg->GetHeadPosition(); pos != NULL;) 
	{
		ptrAsyncMsgSeq = (CAsyncMessageSeq*) m_plstAsyncMsg->GetNext(pos); 
		CList<UINT32,UINT32&>& asyncmsgseq = ptrAsyncMsgSeq->GetMessageList();
		if(asyncmsgseq.GetCount())
		{
			
			for( POSITION datapos = asyncmsgseq.GetHeadPosition(); datapos != NULL;)
			{
				msgid = (asyncmsgseq.GetNext( datapos ));
				if(m_plstOutGoingMsg)
				{
					msgidvalidstatus = 0;
					for(POSITION pos=m_plstOutGoingMsg->GetHeadPosition();pos!=NULL;)
					{
						outgoing_msg = (COutgoingMessage*)m_plstOutGoingMsg->GetNext(pos);
						if(outgoing_msg->GetMessageId() ==msgid)
						{
							msgidvalidstatus=1; // Response Message id is there, its ok
							break;			// break the loop
						}
					}

					if(msgidvalidstatus ==0) // ionvalid msg id
					{
						m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[MsgId - ")+LongToCString(msgid)+CString("] Message Id not found, Invalid Async Message Sequence - ")+LongToCString(ptrAsyncMsgSeq->GetMessageId())+CString("\n") ;
						ErrorStatus=1;
					}
				}
			}		
		}
	}
		if(ErrorStatus==1)
		{
			return 0; // Some error occur
		}
		else
		{
			return 1; // No Error Occure
		}
	
	
}


int CMessageManager::VerifyAppList()		
{return 1;}

int CMessageManager::VerifyBootupList()	
{
	CBootUpMsg *ptrBootUpMsgSeq;
	UINT32 msgid;
	int msgidvalidstatus =0,ErrorStatus=0;
	COutgoingMessage *outgoing_msg; 
	for(POSITION pos = m_pBootupList->GetHeadPosition(); pos != NULL;) 
	{
		ptrBootUpMsgSeq = (CBootUpMsg*) m_pBootupList->GetNext(pos); 
		CList<UINT32,UINT32>& bootupmsglst = ptrBootUpMsgSeq->GetBootMessageList();
		if(bootupmsglst.GetCount())
		{
			
			for( POSITION datapos = bootupmsglst.GetHeadPosition(); datapos != NULL;)
			{
				msgid = (bootupmsglst.GetNext( datapos ));
				if(m_plstOutGoingMsg)
				{
					msgidvalidstatus = 0;
					for(POSITION pos=m_plstOutGoingMsg->GetHeadPosition();pos!=NULL;)
					{
						outgoing_msg = (COutgoingMessage*)m_plstOutGoingMsg->GetNext(pos);
						if(outgoing_msg->GetMessageId() ==msgid)
						{
							msgidvalidstatus=1; // Response Message id is there, its ok
							break;			// break the loop
						}
					}

					if(msgidvalidstatus ==0) // ionvalid msg id
					{
						m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[MsgId - ")+LongToCString(msgid)+CString("] Message Id not found, Invalid Boot Up Sequence - ")+LongToCString(ptrBootUpMsgSeq->GetMessageId())+CString("\n") ;
						ErrorStatus=1;
					}
				}
			}		
		}
	}
	if(ErrorStatus==1)
	{
		return 0; // Some error occur
	}
	else
	{
		return 1; // No Error Occure
	}
}

int CMessageManager::VerifyFreqList()	
{
  CFreqUsedMsg *ptrFreqUsedMsgSeq;
  UINT32 seqid;
  int seqidvalidstatus =0,ErrorStatus=0;
  CAsyncMessageSeq *sequence; 
  for(POSITION pos = m_pFreqList->GetHeadPosition(); pos != NULL;) 
  {
    ptrFreqUsedMsgSeq = (CFreqUsedMsg*) m_pFreqList->GetNext(pos); 
    CList<UINT32,UINT32>& frequsedmsglst = ptrFreqUsedMsgSeq->GetFreqUsedMessageList();
    if(frequsedmsglst.GetCount())
    {
      
      for( POSITION datapos = frequsedmsglst.GetHeadPosition(); datapos != NULL;)
      {
        seqid = (frequsedmsglst.GetNext( datapos ));
        if(m_plstAsyncMsg)
        {
          seqidvalidstatus = 0;
          for(POSITION pos=m_plstAsyncMsg->GetHeadPosition();pos!=NULL;)
          {
            sequence = (CAsyncMessageSeq*)m_plstAsyncMsg->GetNext(pos);
            if(sequence->GetMessageId() ==seqid)
            {		
              seqidvalidstatus=1; // Sequence id is there, its ok
              break;			// break the loop
            }
          }
          
          if(seqidvalidstatus ==0) // ionvalid sequence id
          {
            m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[seqid - ")+LongToCString(seqid)+CString(" ]Sequence Id not found")+CString("\n") ;
            ErrorStatus=1;
          }
        }
      }		
    }
  }
  if(ErrorStatus==1)
  {
    return 0; // Some error occur
  }
  else
  {
    return 1; // No Error Occure
  }
}

/*void* CMessageManager::GetDataMemoryForMessage(CMessage *pMessage,UINT32 *iSize,CString strDataSet)
{
	void *vTempRet= NULL;
	//return the void * for the 
	CFormat *pFormat;// = new CFormat();	
	CDataList dataList;	
	CData dataObj;		
	pFormat = GetFormatForMessage(pMessage);
	if(pFormat!=NULL)
	{
		if(!(pMessage->GetFormat().CompareNoCase(pFormat->GetName())))				
		{
			//the format is found 
			//now check if the input mode is dialog and also the dataset name is not there...then we 
			//have to pop up a generic dialog...
			if(pFormat->GetInputMode().CompareNoCase(_T("DataSet")) && (!strDataSet.CompareNoCase(_T(""))))
			{
  				/*CGenericInput gen_input_dlg;
				if(parent_ptr)
					INPTRLIST * IncomingMessagePtr = parent_ptr->SendMessage(WM_GET_INC_MSG_FOR_OUT,0,0);
  				//INPTRLIST * IncomingMessagePtr = (INPTRLIST *) m_IncomingMsgDataList->GetTail() ;
  				gen_input_dlg.OutGoingMsgPtr = (COutgoingMessage *)pMessage; 
  				gen_input_dlg.IncomingMsgID =  IncomingMessagePtr->stMessage.nMsgType;
  				gen_input_dlg.DoModal();
  				if(!gen_input_dlg.data_set_to_be_sent.GetName().CompareNoCase(_T("")))
  					return(NULL);
  				vTempRet = GetBufferForSocket(pMessage,gen_input_dlg.data_set_to_be_sent,GetFormatForMessage(gen_input_dlg.OutGoingMsgPtr)->GetVarList(),iSize);
  					return vTempRet;*/
			/*}
			else
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
								vTempRet = GetBufferForSocket(pMessage,dataObj,varList,iSize);//matched the data list
								return vTempRet;
							}
						}
						else if(!dataObj.GetName().CompareNoCase(dataList.GetDefault()))
						{
							//default data object stuffing to be done
							vTempRet = GetBufferForSocket(pMessage,dataObj,varList,iSize);
							return vTempRet;
						}
						
					}
					//handle the case in case both the above condition becomes false
					vTempRet = GetBufferForSocket(pMessage,dataObj,varList,iSize);
					return vTempRet;
								
				}//end of type check
			}//end of check dialog
		}//		
	}
	else
		AMLOGERROR(_T("GetData Memory for  Message error"));
	return  vTempRet;
}*/

void* CMessageManager::GetBufferForSocket(CMessage *pMessage,CData& dataObj,CList<CVar,CVar&>& varList,UINT32 *iSize)
{
	UINT32 iTotalSize = GetTotalSizeOfVariable(varList);
	*iSize = iTotalSize;
	//void* buffer1 = malloc(iTotalSize);
	void* buffer1 = MyMallocDataPtr(iTotalSize);
	void* buffer = buffer1;	
	memset(buffer,'k',iTotalSize);
	//CDataType datatype;
	CString strEval;
	INT16 iNoOfElements;
	for( POSITION pos = varList.GetHeadPosition(); pos != NULL;)
	{
		CVar&  cVar = (CVar&)(varList.GetNext( pos )) ;
		CList<CValue,CValue&>& valueList= dataObj.GetValueList();
		for( POSITION valpos = valueList.GetHeadPosition(); valpos != NULL;)
		//matched the name
		{
			CValue& valueObj = (CValue&)(valueList.GetNext( valpos )) ;			
			if(valueObj.GetName() == cVar.GetName())
			{
				//name matched 
				CDataType& datatype = cVar.GetDataType();
				CString cVarName = datatype.GetName();
				////get the type of the data
				BYTE type = valueObj.GetSourceType();
				////if 0 than constant
				if(type == 0)
				{
					if(cVarName.Find(_T("array")) > 0)
					{
						iNoOfElements = cVar.GetCount();
						UINT32 arraySize;// = (iNoOfElements)*(cVar.GetValueSize());
						strEval = valueObj.GetSourceValue();					
						if(cVarName.Find(_T("nt"),0) > 0)
						{
							int* Array = (int*)malloc(iNoOfElements*datatype.GetSize());
							int* back = Array;
							arraySize =	iNoOfElements*datatype.GetSize();
							memset(Array,'0',arraySize);						
							strEval.Insert((strEval.GetLength()+1),',');
							//its the integer array
							for(int iCount=0;iCount < iNoOfElements ;iCount++)
							{
								//this is the number of elements exist in the array
								//so look out for the number 
								CString First = strEval;
								int TotalLen = First.GetLength();
								int iComma = strEval.Find(_T(","),0);
								First = First.Left(iComma);							
								int value = _ttoi(First);
								//Array[iCount] = _ttoi(First);
								memcpy(Array,&value,datatype.GetSize());
								if((iCount+1) < iNoOfElements)
								++Array;
								strEval = strEval.Right((TotalLen - (iComma+1)));
							}
							memcpy(buffer,back,arraySize);
							//buffer = Array;						
							buffer = (char*)buffer+arraySize;
							break;
								
						}
						else if(cVarName.Find(_T("te"),0) > 0)
						{
							char* Array = (char*)malloc(iNoOfElements*datatype.GetSize());
							char* back = Array;
							arraySize =	iNoOfElements*datatype.GetSize();
							memset(Array,'0',arraySize);						
							strEval.Insert((strEval.GetLength()+1),',');
							//its the integer array
							//int n = 22
							//	memset(buffer,22,22);
							//	buffer = (char*)buffer+arraySize;
							//	break;
							for(int iCount=0;iCount < iNoOfElements ;iCount++)
							{
								//this is the number of elements exist in the array
								//so look out for the number 
								CString First = strEval;
								int TotalLen = First.GetLength();
								int iComma = strEval.Find(_T(","),0);
								CString First1 = First.Left(iComma);							
								char value = _ttoi(First1);
								*Array = value;//Array[iCount] = _ttoi(First);
								//memcpy(Array,&value,datatype.GetSize());
								if((iCount+1) < iNoOfElements)
								++Array;
								strEval = strEval.Right((TotalLen - (iComma+1)));
							}
							memcpy(buffer,back,arraySize);
							//buffer = Array;						
							buffer = (char*)buffer+arraySize;
							break;
								
						}
						else if(cVarName.Find(_T("har"),0) > 0)
						{								
							char* Array = (char*)malloc(iNoOfElements*datatype.GetSize());
							char* back = Array;
							memset(Array,0,iNoOfElements);
							arraySize =	iNoOfElements;
							strEval.Insert((strEval.GetLength()+1),',');
							for(int iCount=0;iCount < iNoOfElements ;++iCount)
							{
								//this is the number of elements exist in the array
								//so look out for the number 
								CString First = strEval;
								int TotalLen = First.GetLength();
								int iComma = strEval.Find(_T(","),0);
								First = First.Left(iComma);							
								memcpy(Array,First.GetBuffer(0),datatype.GetSize());
								if((iCount+1) < iNoOfElements)
								++Array;
								//Array[iCount] = (char)First.GetBuffer(0);
								strEval = strEval.Right((TotalLen - (iComma+1)));
							}
							memcpy(buffer,back,iNoOfElements);
							//buffer = Array;
							buffer = (char*)buffer+arraySize;						
							break;					
						}//its the char array
						else if(cVarName.Find(_T("tra"),0) > 0)
						{							
							strEval.Insert((strEval.GetLength()+1),',');
							arraySize = iNoOfElements*(cVar.GetValueSize());
							for(int iCount=0;iCount < iNoOfElements ;++iCount)
							{
								//this is the number of elements exist in the array
								//so look out for the number 
								CString First = strEval;
								int TotalLen = First.GetLength();
								int iComma = strEval.Find(_T(","),0);
								First = First.Left(iComma);
								WideCharToMultiByte(CP_ACP, 0,(LPCWSTR)First.GetBuffer(0),-1,(char*)buffer,arraySize,NULL,NULL);
								//memcpy(buffer,First.GetBuffer(0),cVar.GetValueSize());
								//buffer = (char*)First.GetBuffer(0);
								strEval = strEval.Right((TotalLen - (iComma+1)));
								buffer = (char*)buffer+(cVar.GetValueSize());
							}								
							break;
						}//its the string array
						//stuff the buffer now with the values;						
					}
					else
					{					
						int value = _ttoi(valueObj.GetSourceValue());
						memcpy(buffer,&value,datatype.GetSize());
						buffer = (char*)buffer + datatype.GetSize();
						break;					
					}
				}//that variable is contant
				else if(type == 3)//the varaiable is not constant ,, it can be file req ,, dialog
				{
					//the value is requested
					//get the value from the incoming message data pointer and stuff it in buffer
					//void* GetBufferFromIncomingMessage(CMessage *pMessage,CDatatype Datatype,CValue Valueobj)					
					GetBufferFromIncomingMessage(buffer,_ttoi(valueObj.GetSourceValue()));					
				}
			}
		}			
	}
return buffer1;		
}


void CMessageManager::GetBufferFromIncomingMessage(void *pBuffer,int nIndex)
{
	int nNoOfVariable = 0;
	INT16 iNoOfElements;
	//get the value at the given index from the void data buffer
	INPTRLIST *stPtrListIncomingMsg = NULL;	
	for( POSITION pos = m_IncomingMsgDataList->GetHeadPosition(); pos != NULL;)
	{
		stPtrListIncomingMsg = (INPTRLIST*)(m_IncomingMsgDataList->GetNext( pos ));
		if(stPtrListIncomingMsg->index == m_inComingMsgSelIndex)
		{
			CMessage *pMessage = LocateMessageInList(m_plstIncomingMsg,stPtrListIncomingMsg->stMessage.nMsgType);
			if(pMessage)
			{
				CFormat *pFormat;// = new CFormat();	
				CDataList dataList;	
				CData dataObj;		
				pFormat = GetFormatForMessage(pMessage);
				if(pFormat!=NULL)
				{
					CList<CVar,CVar&>& varList = pFormat->GetVarList();
					dataList = pFormat->GetDataList();
					CList<CData,CData&>& data= dataList.GetData();
					if(data.GetCount() > 0)
					{
						for( POSITION datapos = data.GetHeadPosition(); datapos != NULL;)
						{
							dataObj = (data.GetNext( datapos ));
							break;
						}
					}
					//CList<CData,CData&>& data= dataList.GetData();			
					for( POSITION pos = varList.GetHeadPosition(); pos != NULL;)
					{
						CVar&  cVar = (CVar&)(varList.GetNext( pos )) ;
						nNoOfVariable++;
						CList<CValue,CValue&>& valueList= dataObj.GetValueList();
						for( POSITION valpos = valueList.GetHeadPosition(); valpos != NULL;)
						//matched the name
						{
							CValue& valueObj = (CValue&)(valueList.GetNext( valpos )) ;			
							if(valueObj.GetName() == cVar.GetName())
							{
								//name matched 
								CDataType& datatype = cVar.GetDataType();
								CString cVarName = datatype.GetName();
								if(cVarName.Find(_T("array")) > 0)
								{									
									if(cVarName.Find(_T("nt"),0) > 0)
									{
										iNoOfElements = cVar.GetCount();
										if(nNoOfVariable == nIndex)//this is the variable
										{
											memcpy(pBuffer,stPtrListIncomingMsg->stMessage.nDataPtr,iNoOfElements*datatype.GetSize());
											pBuffer = (char*)pBuffer + (iNoOfElements*datatype.GetSize());
										}
										break;
											
									}
									else if(cVarName.Find(_T("har"),0) > 0)
									{		
										iNoOfElements = cVar.GetCount();
										if(nNoOfVariable == nIndex)//this is the variable
										{
											memcpy(pBuffer,stPtrListIncomingMsg->stMessage.nDataPtr,iNoOfElements*datatype.GetSize());
											pBuffer = (char*)pBuffer + (iNoOfElements*datatype.GetSize());										
										}
										break;					
									}//its the char array
									else if(cVarName.Find(_T("tra"),0) > 0)
									{										
										iNoOfElements = (cVar.GetCount()*cVar.GetValueSize());
										if(nNoOfVariable == nIndex)//this is the variable
										{
											memcpy(pBuffer,stPtrListIncomingMsg->stMessage.nDataPtr,iNoOfElements*datatype.GetSize());
											pBuffer = (char*)pBuffer + (iNoOfElements*datatype.GetSize());
										}
										break;
									}//its the string array
								//stuff the buffer now with the values;						
								}								
								else
								{
									if(nNoOfVariable == nIndex)//this is the variable
									{								
										memcpy(pBuffer,stPtrListIncomingMsg->stMessage.nDataPtr,datatype.GetSize());									
										pBuffer = (char*)pBuffer + datatype.GetSize();
									}
									break;
								}								
							}
						}
					}
				}
			}
			else
			{
				//pMessage is null
			}	
		}	
	}		
}


CMessage* CMessageManager::LocateMessageInList(CPtrList *msgList,UINT32 iMsgId)
{
	//returns the CMessage Object for the iMsgId
  CMessage *pMsg ;
  if(msgList)
  {
    if(msgList->GetCount())
    {
      for( POSITION pos = msgList->GetHeadPosition(); pos != NULL;)
      {
        pMsg = (CMessage*)(msgList->GetNext( pos )) ;
        CString cs1= pMsg->GetPluginName();
        if(pMsg->GetMessageId() == iMsgId)				
          return pMsg;
      }
    }
  }
  return NULL;
}

UINT32 CMessageManager::LocateMessageIdForName(CPtrList *msgList,CString MsgName)
{
	//returns the CMessage Object for the iMsgId
	CMessage *pMsg ;
	if(msgList->GetCount())
	{
		for( POSITION pos = msgList->GetHeadPosition(); pos != NULL;)
		{
			pMsg = (CMessage*)(msgList->GetNext( pos )) ;		
			if(!MsgName.CompareNoCase(pMsg->GetMessageName()))
			{
				return(pMsg->GetMessageId());
			}				
		}
	}
	return NULL;
}

UINT32 CMessageManager::GetTotalSizeOfVariable(CList<CVar,CVar&>& varList)
{
	UINT32 iTotal = 0;
	CDataType datatype;
	if(varList.GetCount() > 0)
	{
		for( POSITION pos = varList.GetHeadPosition(); pos != NULL;)
		{			
			CVar&  cVar = (CVar&)(varList.GetNext( pos )) ;
			datatype = cVar.GetDataType();
			CString cVarName = datatype.GetName();
			if(cVarName.Find(_T("array")) > 0)
			{
				if(cVarName.Find(_T("nt"),0) > 0)
				{
					iTotal+=4*(cVar.GetCount());
				}
				else if(cVarName.Find(_T("har"),0) > 0)
				{
					iTotal+=(cVar.GetValueSize())*(cVar.GetCount());
				}
				else if(cVarName.Find(_T("tra"),0) > 0)
				{
          TRACE2("ValueSize = %d,Count = %d\n",cVar.GetValueSize(),cVar.GetCount());
					iTotal+=(cVar.GetValueSize())*(cVar.GetCount());
				}
				else if(cVarName.Find(_T("yte"),0) > 0)
				{
					iTotal+=(cVar.GetValueSize())*(cVar.GetCount());
				}

			}
			else
			iTotal+=datatype.GetSize();		
		}			
	}
	return iTotal;
}

CString CMessageManager::LocateMessageNameInList(CPtrList *msgList,UINT16 iMsgId)
{
	//returns the CMessage Object for the iMsgId
	CMessage *pMsg;
	if(msgList->GetCount())
	{
		for( POSITION pos = msgList->GetHeadPosition(); pos != NULL;)
		{
			pMsg = (CMessage*)(msgList->GetNext( pos )) ;
			if(pMsg->GetMessageId() == iMsgId)				
				return pMsg->GetMessageName();
		}
	}
	return _T("");
}

CFormat* CMessageManager::GetFormatForMessage(CMessage *pMessage)
{
	if(pMessage!=NULL)
	{
		CFormat *pFormat;
		if(m_pFormatList->GetCount() > 0)//check if emtpy message list 
		{	
			for( POSITION pos = m_pFormatList->GetHeadPosition(); pos != NULL;)
			{
				pFormat = (CFormat*)(m_pFormatList->GetNext( pos )) ;
				if(!(pMessage->GetFormat().CompareNoCase(pFormat->GetName())))				
				{
					return pFormat;
				}
			}
		}	
	}
	return NULL;
}

//----------------------------------------------------------------------------------
//------------------------ Gurdev Additions ----------------------------------------
//----------------------------------------------------------------------------------

/*---------------------------------------------------------------------------------*/
/* This function receive message id and dataset name as input parameter and -------*/
/* fill the 'datasetdetails' parameters with associative values -------------------*/
/* User of this function can use this and evaluate freq, timer out of it ----------*/
/*---------------------------------------------------------------------------------*/

/******************************************************************************

	FUNCTION NAME		: GetDataSetDetails

  	PURPOSE				: Called to fill the details of dataset
	
	INPUT PARAMETERS	: MessageID, Dataset and datasetdetails which is a 
						  reference veriable and will be filled by the function.		

	OUTPUT PARAMETERS	: CData datasetdetails

	AUTHOR				: Gurdev Singh

	RETURNS				: void
		 
	REMARKS				: MFC framework calls this function and the details 
						  returned could be parsed to get dataset attribute.
*******************************************************************************/


void CMessageManager::GetDataSetDetails(UINT32 MsgId,CString sDataSet,CData &datasetdetails)
{
  CString  msgformat;
  CPtrList * ptr_to_list_of_outgoing_msg = m_plstOutGoingMsg;
  CPtrList * ptr_to_format_list; // Pointer to format list
  COutgoingMessage * ptr_to_outgoing_msg;
  
  if(ptr_to_list_of_outgoing_msg->GetCount())
  {
    for(POSITION pos =  ptr_to_list_of_outgoing_msg->GetHeadPosition();pos!= NULL;)
    {
      ptr_to_outgoing_msg = (COutgoingMessage *) (ptr_to_list_of_outgoing_msg->GetNext(pos));
      
      if(ptr_to_outgoing_msg->GetMessageId() == MsgId)
      {
        msgformat = ptr_to_outgoing_msg->GetFormat();
      }
    }
  }
  
  ptr_to_format_list = m_pFormatList;
  CFormat* FmtPtr;
  CDataList datalist;
  //find the format name and if found break
  if(ptr_to_format_list->GetCount())
  {
    for(POSITION pos =  ptr_to_format_list->GetHeadPosition();pos!= NULL;)
    {
      FmtPtr = (CFormat *) (ptr_to_format_list->GetNext (pos));
      CString name = FmtPtr->GetName();
      if(!msgformat.CompareNoCase((LPCTSTR)name))
      {
        datalist = FmtPtr->GetDataList(); 
        break;
      }
    }
  }
  
  CData dataobj;
  CString datalistname = datalist.GetName(); 
  //get the data list from datalist
  CList <CData,CData&>&  dataset = datalist.GetData();
  CList <CVar,CVar&>&    varlist = FmtPtr->GetVarList();
  if(dataset.GetCount() > 0 && varlist.GetCount() > 0)
  {
    
    for(POSITION pos =  dataset.GetHeadPosition();pos!= NULL;)
    {
      dataobj = dataset.GetNext(pos);
      if(sDataSet.GetLength())// non default, if length > 0 
      {
        if(!(sDataSet.CompareNoCase(dataobj.GetName())))
        {	
          datasetdetails = dataobj;
          break;
        }
      }
      else					// Default dataset will be considered
      {
        if(!(datalist.GetDefault().CompareNoCase(dataobj.GetName())))
        {	
          datasetdetails = dataobj;
          break;
        }
      }
    }
  }
  else
  {
    datasetdetails.SetFrequency(0);
    datasetdetails.SetTimer (0);
  }
	
}



/******************************************************************************

	FUNCTION NAME		: GetTimer

  	PURPOSE				: Function to find the Timer value
	
	INPUT PARAMETERS	: UINT32, CString (MessageId,DatasetName)

	OUTPUT PARAMETERS	: Timer Value

	AUTHOR				: Gurdev Singh

	RETURNS				: UINT32
		 
	REMARKS				: MFC framework calls this function to find the Timer
						  associated with the datalist of Message by giving 
						  the MessageId and Dataset Name. 
*******************************************************************************/

UINT32 CMessageManager::GetTimer(UINT32 MsgId,CString sDataSet)
{
	CData cdata;
	GetDataSetDetails(MsgId,sDataSet,cdata);
	return cdata.GetTimer();
}

/******************************************************************************

	FUNCTION NAME		: GetTimer  (Overloaded)

  	PURPOSE				: Function to find the Timer value
	
	INPUT PARAMETERS	: CString, CString (MessageName,DatasetName)

	OUTPUT PARAMETERS	: Timer Value

	AUTHOR				: Gurdev Singh

	RETURNS				: UINT32
		 
	REMARKS				: MFC framework calls this function to find the 
						  Timer associated with the datalist of Message 
						  by giving the MessageId and Dataset Name. 
*******************************************************************************/




UINT32 CMessageManager::GetTimer(CString MsgName,CString sDataSet)
{
	CData cdata;
	UINT32 MsgId;
	MsgId = GetMsgIdFromMsgName(MsgName);
	GetDataSetDetails(MsgId,sDataSet,cdata);
	return cdata.GetTimer();
}



/******************************************************************************

	FUNCTION NAME		: GetFreq

  	PURPOSE				: Function to find the Frequency value
	
	INPUT PARAMETERS	: UINT32, CString (MessageId,DatasetName)

	OUTPUT PARAMETERS	: Frequency Value

	AUTHOR				: Gurdev Singh

	RETURNS				: UINT32
		 
	REMARKS				: MFC framework calls this function to find the 
						  Frequency associated with the datalist of Message 
						  by giving the MessageId and Dataset Name. 
*******************************************************************************/

UINT32 CMessageManager::GetFreq(UINT32 MsgId,CString sDataSet)
{
	CData cdata;
	GetDataSetDetails(MsgId,sDataSet,cdata);
	return cdata.GetFrequency();
}

/******************************************************************************

	FUNCTION NAME		: GetFreq (Overloaded)

  	PURPOSE				: Function to find the Frequency value
	
	INPUT PARAMETERS	: CString, CString (MessageName,DatasetName)

	OUTPUT PARAMETERS	: Frequency Value

	AUTHOR				: Gurdev Singh

	RETURNS				: UINT32
		 
	REMARKS				: MFC framework calls this function to find the 
						  Frequency associated with the datalist of Message 
						  by giving the MessageId and Dataset Name. 
*******************************************************************************/


UINT32 CMessageManager::GetFreq(CString MsgName,CString sDataSet)
{
	CData cdata;
	UINT32 MsgId;
	MsgId = GetMsgIdFromMsgName(MsgName);
	GetDataSetDetails(MsgId,sDataSet,cdata);
	return cdata.GetFrequency();
}

/******************************************************************************

	FUNCTION NAME		: GetDefaultDataSetForMessage

  	PURPOSE				: Function to find the default data set for a message
	
	INPUT PARAMETERS	: UINT32 MessageID

	OUTPUT PARAMETERS	: CString (Name of the default dataset)

	AUTHOR				: Gurdev Singh

	RETURNS				: CString
		 
	REMARKS				: MFC framework calls this function to find the default 
						  dataset associated with the datalist of Messageby 
						  giving the MessageId.
*******************************************************************************/

CString CMessageManager::GetDefaultDataSetForMessage(UINT32 MsgId)
{
	CString  msgformat;
	CPtrList * ptr_to_list_of_outgoing_msg = m_plstOutGoingMsg;
	CPtrList * ptr_to_format_list;
	COutgoingMessage * ptr_to_outgoing_msg;
	
	if(ptr_to_list_of_outgoing_msg->GetCount())
	{
		for(POSITION pos =  ptr_to_list_of_outgoing_msg->GetHeadPosition();pos!= NULL;)
		{
     		ptr_to_outgoing_msg = (COutgoingMessage *) (ptr_to_list_of_outgoing_msg->GetNext(pos));

			if(ptr_to_outgoing_msg->GetMessageId() == MsgId)
			{
				msgformat = ptr_to_outgoing_msg->GetFormat();
			}
		}
	}

	ptr_to_format_list= m_pFormatList;
	CFormat* FmtPtr;
	CDataList datalist;
    if(ptr_to_format_list->GetCount())
	{
		for(POSITION pos =  ptr_to_format_list->GetHeadPosition();pos!= NULL;)
		{
     		FmtPtr = (CFormat *) (ptr_to_format_list->GetNext (pos));
			CString name = FmtPtr->GetName();
			if(!msgformat.CompareNoCase((LPCTSTR)name))
			{
				datalist = FmtPtr->GetDataList();
				return datalist.GetDefault();
			}
		}
	}
	return CString("");
}


/******************************************************************************

	FUNCTION NAME		: GetMsgIdFromMsgName

  	PURPOSE				: Function to find the Message id by giving the Message
						  Name
	
	INPUT PARAMETERS	: CString MsgName

	OUTPUT PARAMETERS	: Message Id

	AUTHOR				: Gurdev Singh

	RETURNS				: UINT32
		 
	REMARKS				: MFC framework calls this function to find the Message 
						  Id by giving the message name.
*******************************************************************************/
UINT32 CMessageManager::GetMsgIdFromMsgName(CString MsgName)
{
	CString  msgformat;
	CPtrList * ptr_to_list_of_outgoing_msg = m_plstOutGoingMsg;
//	CPtrList * ptr_to_format_list;
	COutgoingMessage * ptr_to_outgoing_msg;
	
	if(ptr_to_list_of_outgoing_msg->GetCount())
	{
		for(POSITION pos =  ptr_to_list_of_outgoing_msg->GetHeadPosition();pos!= NULL;)
		{
     		ptr_to_outgoing_msg = (COutgoingMessage *) (ptr_to_list_of_outgoing_msg->GetNext(pos));

			if(ptr_to_outgoing_msg->GetMessageName() == MsgName)
			{
				return ptr_to_outgoing_msg->GetMessageId(); 
			}
		}
	}
	
	return 0;
}



void CMessageManager::SetDataSetDetails(UINT32 MsgId,CString sDataSet,CString sDefaultDataSet,CString sInputMode, UINT32 Freq, UINT32 Timer)
{
	CNWGCMLHandler nwgcmlhandler;
	CString  msgformat;
	CPtrList * ptr_to_list_of_outgoing_msg = m_plstOutGoingMsg;
	COutgoingMessage * ptr_to_outgoing_msg;
	
	if(ptr_to_list_of_outgoing_msg->GetCount())
	{
		for(POSITION pos =  ptr_to_list_of_outgoing_msg->GetHeadPosition();pos!= NULL;)
		{
     		ptr_to_outgoing_msg = (COutgoingMessage *) (ptr_to_list_of_outgoing_msg->GetNext(pos));
			
			if(ptr_to_outgoing_msg->GetMessageId() == MsgId)
			{
				msgformat = ptr_to_outgoing_msg->GetFormat();
			}
		}
	}

	CFormat* FmtPtr;
	CData dataobj;
	CDataList datalist;

	//find the format name and if found break
	if(m_pFormatList)
	{
		if(m_pFormatList->GetCount())
		{
			for(POSITION pos =  m_pFormatList->GetHeadPosition();pos!= NULL;)
			{
     			FmtPtr = (CFormat *) (m_pFormatList->GetAt(pos));
				CString name = FmtPtr->GetName();
				if(!msgformat.CompareNoCase((LPCTSTR)name))
				{
					datalist = FmtPtr->GetDataList(); 
					CList <CData,CData&>& datasetlist = datalist.GetData();
					if(datasetlist.GetCount())
                    {
                        for(POSITION pos =  datalist.GetData().GetHeadPosition();pos!= NULL;)
						{
                            dataobj = datalist.GetData().GetAt(pos);
                            if(!(sDataSet.CompareNoCase(dataobj.GetName())))
                            {	
                                dataobj.SetFrequency(Freq);
                                dataobj.SetTimer(Timer);
                                datalist.GetData().SetAt(pos,dataobj);
								nwgcmlhandler.UpDateDataListInDom(datalist,dataobj); 

								if(sDefaultDataSet.GetLength()) 
								{
									datalist.SetDefault(sDefaultDataSet); // to set default dataset
								}
								break;
							}
                            datalist.GetData().GetNext(pos);  
                        }
                    }
					FmtPtr->SetDataList(datalist); 
					if(sInputMode.GetLength())
					{
						FmtPtr->SetInputMode(sInputMode);// 
						nwgcmlhandler.UpDateInputModeInDom(*FmtPtr); 
					}
					break;
				}
				m_pFormatList->GetNext (pos);
			}
		}
	}
}


/******************************************************************************

	FUNCTION NAME		: GetMessageListForApp

  	PURPOSE				: Function to find the Messages associated with an application
						  Name
	
	INPUT PARAMETERS	: CString AppName ( name of the application)

	OUTPUT PARAMETERS	: CPtrList of Messages

	AUTHOR				: Pankaj Adlakha

	RETURNS				: CPtrList *
		 
	REMARKS				: MFC framework calls this function to find the Message 
						  Ids by giving the application name.
*******************************************************************************/

CList<CString,CString&>* CMessageManager::GetMessageListForApp(CString AppName)
{
	char *ptr_to_msgid;
	CPtrList * ptr_to_outgoing_msg_lst = m_plstOutGoingMsg;
	COutgoingMessage * ptr_to_outgoing_msg;
	UINT32 appid_selected = _ttoi(AppName);
	char * buffer_for_str = (char*) malloc(20);
	CList<CString,CString&>* message_list = new CList<CString,CString&>();
	CString message_name;
	if(ptr_to_outgoing_msg_lst)
	{
		if(ptr_to_outgoing_msg_lst->GetCount())
		{
			for(POSITION pos =  ptr_to_outgoing_msg_lst->GetHeadPosition();pos!= NULL;)
			{
     			ptr_to_outgoing_msg = (COutgoingMessage *) (ptr_to_outgoing_msg_lst->GetNext (pos));
				UINT32 AppId = ptr_to_outgoing_msg->GetAppId();
				if(AppId == appid_selected)
				{
					UINT msgid = ptr_to_outgoing_msg->GetMessageId();
					ptr_to_msgid = itoa(msgid,buffer_for_str,10);
					message_name = (CString (ptr_to_msgid) + ":" + (ptr_to_outgoing_msg->GetMessageName()));
					message_list->AddTail(message_name);
				}
			}
		}
	}
	free(buffer_for_str);
	return(message_list);
}


int CMessageManager::GetPluginStatusFromMsgId(UINT32 MsgId)
{
	
	CString sMsgId, msgformat;
    CPtrList *ptrOutGoingMsgList=m_plstOutGoingMsg;
	COutgoingMessage *ptrOutGoingMsg;

	if(ptrOutGoingMsgList->GetCount())
	{
		for(POSITION pos = ptrOutGoingMsgList->GetHeadPosition();pos!= NULL;)
		{
     		ptrOutGoingMsg = (COutgoingMessage *) (ptrOutGoingMsgList->GetNext (pos));
			if(ptrOutGoingMsg->GetMessageId()== MsgId)
			{
				msgformat = ptrOutGoingMsg->GetFormat();
				break;
			}
		}
	}

	if(msgformat.CompareNoCase(CString("NULL"))) // if msgformat is null means plugin
	{
		return 0;
	}
	else
	{
		return 1;
	}
}




void CMessageManager::SaveDataSetValues(UINT32 MsgId, CData& dataset)
{
	CPtrList * ptr_to_outgoing_lst = m_plstOutGoingMsg;
	COutgoingMessage * ptr_to_outgoing_msg;
	CString msgformat;
	if(ptr_to_outgoing_lst)
	{
		if(ptr_to_outgoing_lst->GetCount())
		{
			for(POSITION pos =  ptr_to_outgoing_lst->GetHeadPosition();pos!= NULL;)
			{
    			ptr_to_outgoing_msg = (COutgoingMessage *) (ptr_to_outgoing_lst->GetNext (pos));
				if(ptr_to_outgoing_msg->GetMessageId() == MsgId)
				{
					msgformat = ptr_to_outgoing_msg->GetFormat();
				}
			}
		}
		CFormat* FmtPtr;
		CData temp_dataset; 
    CNWGCMLHandler gcmlHandler;
		if(m_pFormatList)
		{
			if(m_pFormatList->GetCount())
			{
				for(POSITION pos1 =  m_pFormatList->GetHeadPosition();pos1!= NULL;)
				{
					FmtPtr = (CFormat *) (m_pFormatList->GetAt(pos1));
					CString name = FmtPtr->GetName();
					if(!msgformat.CompareNoCase((LPCTSTR)name))
					{
						CDataList datalist = FmtPtr->GetDataList();
						CList<CData,CData&>& datasetlist = datalist.GetData();
						if(datasetlist.GetCount())
						{
							for(POSITION pos = datalist.GetData().GetHeadPosition();pos!=NULL;)
							{
								temp_dataset = datalist.GetData().GetAt(pos);
								if(!temp_dataset.GetName().CompareNoCase(dataset.GetName()))
								{
									datalist.GetData().SetAt(pos,dataset);
									FmtPtr->SetDataList(datalist);//..AddValue(dataset);
									m_pFormatList->SetAt(pos1,FmtPtr);
									gcmlHandler.UpDateDataListInDom(datalist,dataset);
									break;
								}
								datalist.GetData().GetNext(pos);
								if(pos == NULL)
								{
									datalist.AddValue(dataset);
									FmtPtr->SetDataList(datalist);//..AddValue(dataset);
									m_pFormatList->SetAt(pos1,FmtPtr);
									gcmlHandler.SetDataSetInDom(dataset,datalist);
								}

							}
						}
						
						break;
					}
					m_pFormatList->GetNext(pos1);
				}
			}
		}
	}

}
void CMessageManager::SaveMessageAttributes(UINT32 MsgId,CString DefDSName,CString InputMode)
{
	COutgoingMessage* ptr_to_outgoing_msg;
	if(m_plstOutGoingMsg)
	{
		if(m_plstOutGoingMsg->GetCount())
		{
			for(POSITION pos =  m_plstOutGoingMsg->GetHeadPosition();pos!= NULL;)
			{
    			ptr_to_outgoing_msg = (COutgoingMessage *) (m_plstOutGoingMsg->GetNext (pos));
				if(ptr_to_outgoing_msg->GetMessageId() == MsgId)
				{
					CString fmtname = ptr_to_outgoing_msg->GetFormat();
					if(m_pFormatList->GetCount())
					{
						for(POSITION pos1 =  m_pFormatList->GetHeadPosition();pos1!= NULL;)
						{
							CFormat *FmtPtr = (CFormat *) (m_pFormatList->GetAt(pos1));
							CString name = FmtPtr->GetName();
							if(!fmtname.CompareNoCase((LPCTSTR)name))
							{
								CDataList datalist = FmtPtr->GetDataList();
								datalist.SetDefault(DefDSName);
								FmtPtr->SetInputMode(InputMode);
								FmtPtr->SetDataList(datalist);
								m_pFormatList->SetAt(pos1,FmtPtr);
								CNWGCMLHandler nwgcmlhandler;
								nwgcmlhandler.UpDateInputModeInDom(*FmtPtr);
								//dataset is being sent only for namesake....
								CData dataset;
								nwgcmlhandler.UpDateDefaultDataSetInDom(datalist,dataset);
								CGCMLGenerator generate_xml;
								generate_xml.MakeGCML(CMessageManager::m_strXMLFile.LockBuffer());
								break;
							}
							m_pFormatList->GetNext(pos1);
						}
					}
					break;
					
				}
			}
		}
	}
}


CString CMessageManager::LongToCString(long int number)
{
	CString strNumber;
	char *dummy;
	dummy = (char*) malloc(sizeof(char)*20);
	strNumber = CString(ltoa(number,dummy,10));
	return strNumber;
}

char* CMessageManager::UnicodeToAscii(CString UnicodeCString)
{
	char *AsciiCharString;
	int loopcounter;
	long unicodecstringlength,countchar =0;;
	unicodecstringlength = UnicodeCString.GetLength();
	
	AsciiCharString = (char*) malloc(sizeof(char)*(unicodecstringlength));

	for(loopcounter=0;loopcounter<=unicodecstringlength-1;loopcounter++)
	{
		if(UnicodeCString.GetAt(loopcounter) != 0) // Second Byte which is having value 0
		{
			AsciiCharString[loopcounter] = (char)UnicodeCString.GetAt(loopcounter);
		}
	}
	AsciiCharString[loopcounter] =0;

	return AsciiCharString;
}

void CMessageManager::SetFilePath(CString strFilePath)
{
  m_strXMLFile = strFilePath;
}

CString CMessageManager::GetFilePath()
{
  return m_strXMLFile;
}