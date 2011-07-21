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
 *  XMLVerifier.cpp
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
 *------------------------------------------------------------------------------
 *==============================================================================
 *******************************************************************************/

// XMLVerifier.cpp: implementation of the CXMLVerifier class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLVerifier.h"

#include "Message.h"
#include "Format.h"
#include "Data.h"
#include "DataList.h"
#include "DataType.h"
#include "GlobalConstants.h"
#include "NWGCMLHandler.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLVerifier::CXMLVerifier()
{
	//m_XMLErrors= 
	m_MMObj = new CMessageManager();
	AppIdErrorPrintedStatus=0;
	
}

CXMLVerifier::~CXMLVerifier()
{
  if (m_MMObj)
  {
    delete m_MMObj;
  }
}



int CXMLVerifier::VerifyLists()
{

	int flagIncomingMsg,flagOutGoingMsg,flagFormatList,flagDataList,flagAsyncMsg,flagFreqlist,flagBootupList;
	flagIncomingMsg=flagOutGoingMsg=flagFormatList=flagDataList=flagAsyncMsg = 1; // 1 means no error
	flagFreqlist = flagBootupList = 1;
	int flagCrossCheck =1;

	flagIncomingMsg = VerifyIncomingMsg(m_MMObj->m_plstIncomingMsg);
	flagOutGoingMsg = VerifyOutGoingMsg(m_MMObj->m_plstOutGoingMsg);
	flagFormatList  = VerifyFormatList();
	flagDataList    = VerifyDataList();
	flagAsyncMsg    = VerifyAsyncMsg(m_MMObj->m_plstAsyncMsg);
	flagFreqlist	= VerifyFreqList();
	flagBootupList	= VerifyBootupList(m_MMObj->m_pBootupList);

	flagCrossCheck = CrossCheckFormatAndDataset();

  BOOL bVerify = (flagFreqlist&&flagIncomingMsg&&flagOutGoingMsg&&flagFormatList&&flagDataList&&flagAsyncMsg&&flagCrossCheck);

#ifdef _DUAL_SIM_
  int flagIncomingMsg2 = 1;
  int flagOutGoingMsg2 = 1;
  int flagAsyncMsg2 = 1;
  int flagBootupList2 = 1;

  flagIncomingMsg2 = VerifyIncomingMsg(m_MMObj->m_plstIncomingMsg2);
  flagOutGoingMsg2 = VerifyOutGoingMsg(m_MMObj->m_plstOutGoingMsg2);
  flagBootupList2	= VerifyBootupList(m_MMObj->m_pBootupList2);
  flagAsyncMsg2    = VerifyAsyncMsg(m_MMObj->m_plstAsyncMsg2);

  bVerify = bVerify&&flagIncomingMsg2&&flagOutGoingMsg2&&flagBootupList2&&flagAsyncMsg2;
#endif /*_DUAL_SIM_*/


	if(bVerify)// 1 means no error
	{
		if(m_XMLErrors.GetLength())
		{
			AfxMessageBox(CString("XML Errors \n\n") + m_XMLErrors);	
		}
		return 1; //Success, no error, but warning may be there
	}
	else  // error is there
	{
		AfxMessageBox(CString("XML Errors \n\n")+m_XMLErrors);
		if(m_fErrorFile.Open(XML_ERROR_FILE_NAME,CFile::modeCreate|CFile::modeWrite|CFile::typeText)==TRUE) 
		{
			m_XMLErrors = CString("XML Errors \n\n")+m_XMLErrors;
			int length = m_XMLErrors.GetLength();
			m_fErrorFile.Write((char *)CMessageManager::UnicodeToAscii(m_XMLErrors),length);
			m_fErrorFile.Close();
		}
		else
		{
			 AfxMessageBox(CString("Error in File Handling : XMLError file"));
		}
		return 0; // error
	}
}




int CXMLVerifier::CheckMessageReplication(UINT32 iMsgId, CPtrList *msgList)
{

	//returns the no of occurance of iMsgId
	CMessage *pMsg ;
	int occurance=0;

	if(msgList)
	{
		if(msgList->GetCount())
		{
			occurance=0;
			for( POSITION pos = msgList->GetHeadPosition(); pos != NULL;)
			{
				pMsg = (CMessage*)(msgList->GetNext( pos )) ;
				if(pMsg->GetMessageId() == iMsgId)				
				{
					occurance++;
				}
			}
		}
	}
	return occurance>1?1:0;
}

int CXMLVerifier::FoundAndCheckedForReplication(CList <UINT32,UINT32&> &CheckedMsgList, UINT32 MsgNo)
{
	
	CheckedMsgList.AddTail(MsgNo);
	return 1;
}


int CXMLVerifier::IsAllreadyCheckedForReplication(CList <UINT32,UINT32&> &CheckedMsgList, UINT32 MsgNo)
{
	UINT32 mId;
	int isChecked =0;
	if(CheckedMsgList.GetCount())
	{
		for(POSITION pos = CheckedMsgList.GetHeadPosition();pos!=NULL;)
		{
			mId = (UINT32) CheckedMsgList.GetNext(pos);
			if(mId==MsgNo)
			{
				isChecked =1;
			}
	
		}
	}

	return isChecked;
}


int CXMLVerifier::VerifyIncomingMsg(CPtrList *plstIncomingMsg)
{	
	//take one message from incomming list
	//check that its AppId is there in the appidlist, traverse the appid list for each
	//check its response message id exists or not, traverse the outgoing message list

  CIncomingMsg *IncMsg;
  CApplication *App_ptr;
  COutgoingMessage *outgoing_msg;
  
  CList <UINT32,UINT32&> CheckedMsgList;
  
  UINT32 AppId, RspMsgId;
  int AppIdStatus,RspMsgIdStatus,ErrorStatus=0;//will be one even if single error occure 
  
  if(plstIncomingMsg && plstIncomingMsg->GetCount())
  {
    for(POSITION pos = plstIncomingMsg->GetHeadPosition();pos!=NULL;)
    {
      IncMsg = (CIncomingMsg *) plstIncomingMsg->GetNext(pos);
      if(IncMsg->GetMessageId() !=0)
      {
        if(!IncMsg->GetMessageName().CompareNoCase(_T("")))
        {
          m_XMLErrors = m_XMLErrors+CString("WARNING 3001 : Name of Incoming Message  is missing - [")+m_MMObj->LongToCString(IncMsg->GetMessageId())+CString("]\n");	
          ErrorStatus = ErrorStatus || 0; //OR with 0 because its an warning not error
        }

        if(IsAllreadyCheckedForReplication(CheckedMsgList,IncMsg->GetMessageId())!=1)
        {
          //Check for replicated message id, ie redefination
          if(CheckMessageReplication(IncMsg->GetMessageId(), plstIncomingMsg)==1)
          {
            m_XMLErrors = m_XMLErrors+CString("ERROR 1001 : Redefination of Incoming Message [")+m_MMObj->LongToCString(IncMsg->GetMessageId())+CString("]\n");	
            ErrorStatus = ErrorStatus || 1;
            
            FoundAndCheckedForReplication(CheckedMsgList,IncMsg->GetMessageId());
          }
        }
        
        AppId = IncMsg->GetAppId(); 
        
        // now search for the appid in appid list for existance
        AppIdStatus=0;
        if(m_MMObj->m_pAppList && m_MMObj->m_pAppList->GetCount())
        {
          for(POSITION pos =  m_MMObj->m_pAppList->GetHeadPosition();pos!= NULL;)
          {
            App_ptr = (CApplication *) (m_MMObj->m_pAppList->GetNext (pos));
            if(App_ptr->GetAppId()==AppId)
            {
              AppIdStatus =1; // appid found, its ok
              break; // if found, get out of loop
            }
          }
          if(AppIdStatus==0) // means not found
          {
            //m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[MsgId - ")+m_MMObj->LongToCString(IncMsg->GetMessageId())+CString("] Invalid Application Id - ")+m_MMObj->LongToCString(AppId)+CString("\n") ;
            AppIdError[AppId] = AppIdError[AppId] +CString(" ")+m_MMObj->LongToCString(IncMsg->GetMessageId());
            ErrorStatus=ErrorStatus||1;
          }
        }
        else
        {
          if(AppIdErrorPrintedStatus==0)
          {
            m_XMLErrors = m_XMLErrors+CString("ERROR 1002: Application Id are not defined in XML")+CString("\n");
            ErrorStatus=ErrorStatus||1;
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
            if(m_MMObj->m_plstOutGoingMsg)
            {
              for(POSITION pos=m_MMObj->m_plstOutGoingMsg->GetHeadPosition();pos!=NULL;)
              {
                outgoing_msg = (COutgoingMessage*)m_MMObj->m_plstOutGoingMsg->GetNext(pos);
                if(outgoing_msg->GetMessageId() ==RspMsgId)
                {
                  RspMsgIdStatus=1; // Response Message id is there, its ok
                  break;			// break the loop
                }
              }
              
              if(RspMsgIdStatus==0)
              {
                m_XMLErrors = m_XMLErrors+CString("ERROR 1003: ")+CString("MsgId - [")+m_MMObj->LongToCString(IncMsg->GetMessageId())+CString("] Invalid Response Message Id - ")+m_MMObj->LongToCString(RspMsgId)+CString("\n");
                ErrorStatus=ErrorStatus||1;
              }
            }
            else
            {
              m_XMLErrors = m_XMLErrors+CString("ERROR 1004: Outgoing Message List is empty")+CString("\n");
              ErrorStatus=ErrorStatus||1;
            }
          }
        }
        else
        {
          m_XMLErrors = m_XMLErrors+CString("ERROR 1036: ")+CString("MsgId - [")+m_MMObj->LongToCString(IncMsg->GetMessageId())+CString("] Response message not found\n");
          ErrorStatus=ErrorStatus||1;
        }
      }
      else
      {
        m_XMLErrors = m_XMLErrors+CString("ERROR 1034: Incomplete data for Incoming Message\n");
        ErrorStatus=ErrorStatus||1;
      }
    }
  }
  else
  {
    AfxMessageBox(_T("ERROR 1005: Incoming Msg list is Empty"));
    ErrorStatus=ErrorStatus||1;
  }
  int index;
  for(index=0;index<=MAX_APPLICATIONS-1;index++)
  {
    if(AppIdError[index].GetLength()>0)
    {
      m_XMLErrors = m_XMLErrors+CString("ERROR 1005: ")+CString("For Incoming Messages - [")+AppIdError[index]+CString("] : Invalid AppId - [")+m_MMObj->LongToCString(index)+CString("]\n");
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


int CXMLVerifier::CheckOneFmtMultiMsgAssociativity(CString format, CString& MsgidsHavingSameFormat)
{

	CMessage *pMsg ;
	int occurance=0,commaStatus=0;
	CString Comma;

	if(m_MMObj->m_plstOutGoingMsg)
	{
		if(m_MMObj->m_plstOutGoingMsg->GetCount())
		{
			
			for( POSITION pos = m_MMObj->m_plstOutGoingMsg->GetHeadPosition(); pos != NULL;)
			{
				pMsg = (CMessage*)(m_MMObj->m_plstOutGoingMsg->GetNext( pos )) ;
				if(!pMsg->GetFormat().CompareNoCase(format))				
				{
					if(commaStatus ==1) Comma = CString(",");
					MsgidsHavingSameFormat = MsgidsHavingSameFormat+Comma+m_MMObj->LongToCString(pMsg->GetMessageId());
					occurance++;
					commaStatus =1;
				}
			}
		}
	}
	return occurance>1?1:0;
}


int CXMLVerifier::VerifyOutGoingMsg(CPtrList *plstOutGoingMsg)
{	
	//take one message from out going list
	//check that its AppId is there in the appidlist, traverse the appid list for each message
	//check that its format name is there in the formatlist, traverse format list for each message
	COutgoingMessage *OutGoingMsg;
	CApplication *App_ptr;
	CString FormatName,PluginName;

	CList <UINT32,UINT32&> CheckedMsgList;

	UINT32 AppId;
	int AppIdStatus,FormatNameStatus,ErrorStatus=0;//ErrorStatus will be one even if single error occure 

	if(plstOutGoingMsg && plstOutGoingMsg->GetCount())
	{
		for(POSITION pos = plstOutGoingMsg->GetHeadPosition();pos!=NULL;)
		{
			OutGoingMsg = (COutgoingMessage *) plstOutGoingMsg->GetNext(pos);
			if(OutGoingMsg->GetMessageId()!=0)
			{
			

			if(!OutGoingMsg->GetMessageName().CompareNoCase(_T("")))
			{
				m_XMLErrors = m_XMLErrors+CString("WARNING 3002 : Name of Outgoing Message  is missing - [")+m_MMObj->LongToCString(OutGoingMsg->GetMessageId())+CString("]\n");	
				ErrorStatus = ErrorStatus || 0; //OR with 0 because its an warning not error
			}
			
			// Check if both Format as well as plugin exist
			if((OutGoingMsg->GetFormat().CompareNoCase(_T("Null")))&&((OutGoingMsg->GetFormat().CompareNoCase(_T("")))&&(OutGoingMsg->GetPluginName().CompareNoCase(_T("")))))
			{
				m_XMLErrors = m_XMLErrors+CString("ERROR 1006 : Format and Plugin can never exist togather ")+m_MMObj->LongToCString(OutGoingMsg->GetMessageId())+CString("\n");	
				ErrorStatus = ErrorStatus || 1; //OR with 0 because its an warning not error
			}


			// Check if both Format as well as plugin are miss
			if((!OutGoingMsg->GetFormat().CompareNoCase(_T("")))&&(!OutGoingMsg->GetPluginName().CompareNoCase(_T(""))))
			{
				m_XMLErrors = m_XMLErrors+CString("ERROR 1006 : Format and Plugin both are miss ")+m_MMObj->LongToCString(OutGoingMsg->GetMessageId())+CString("\n");	
				ErrorStatus = ErrorStatus || 1; //OR with 0 because its an warning not error
			}

			
			//Checking for replicated outgoing messages
			if(IsAllreadyCheckedForReplication(CheckedMsgList,OutGoingMsg->GetMessageId())!=1)
			{
				if(CheckMessageReplication(OutGoingMsg->GetMessageId(), plstOutGoingMsg)==1)
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR 1007 : Redefination of Outgoing Message [")+m_MMObj->LongToCString(OutGoingMsg->GetMessageId())+CString("]\n");	
					ErrorStatus = ErrorStatus || 1;
					FoundAndCheckedForReplication(CheckedMsgList,OutGoingMsg->GetMessageId());
				}
			}


	
			AppId = OutGoingMsg->GetAppId(); 
			
			// now search for the appid in appid list for existance
			AppIdStatus=0; 
			if(m_MMObj->m_pAppList && m_MMObj->m_pAppList->GetCount())
			{
				for(POSITION pos =  m_MMObj->m_pAppList->GetHeadPosition();pos!= NULL;)
				{

     				App_ptr = (CApplication *) (m_MMObj->m_pAppList->GetNext (pos));
					if(App_ptr->GetAppId()==AppId)
					{
						AppIdStatus =1; // appid found, its ok
						break; // if found, get out of loop
					}
				}

				if(AppIdStatus==0) // means not found
				{
					//m_XMLErrors = m_XMLErrors+CString("ERROR : ")+CString("[MsgId - ")+m_MMObj->LongToCString(OutGoingMsg->GetMessageId())+CString("] Invalid Application Id - ")+m_MMObj->LongToCString(AppId)+CString("\n") ;
					AppIdError[AppId] = AppIdError[AppId] +CString(" ")+m_MMObj->LongToCString(OutGoingMsg->GetMessageId());
					ErrorStatus = ErrorStatus || 1;
					
				}
			}
			else
			{
				if(AppIdErrorPrintedStatus==0) //AppIdErrorPrintedStatus is a member variable
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR 1002: Application Id are not defined in XML")+CString("\n");
					ErrorStatus = ErrorStatus || 1;
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
					m_XMLErrors = m_XMLErrors+CString("ERROR 1008: ")+CString("[MsgId - ")+m_MMObj->LongToCString(OutGoingMsg->GetMessageId())+CString("] Format is \"Null\" and Plugin is not there\n") ;
					ErrorStatus = ErrorStatus || 1;
				}
			}
			else
			{
				if(FormatName.GetLength()) // some format is there and it is not "NULL"
				{
			// Now search this format name in the list of formats
				

			if(m_MMObj->m_pFormatList)
			{
				CFormat *pFormat;
				FormatNameStatus=0;
				for(POSITION pos = m_MMObj->m_pFormatList->GetHeadPosition(); pos != NULL;)
				{
					pFormat = (CFormat*)m_MMObj->m_pFormatList->GetNext( pos ) ;
					if(!FormatName.CompareNoCase(pFormat->GetName()))				
					{
						FormatNameStatus=1;
						break;
					}
				
				}	
				if(FormatNameStatus==0) // means not found
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR 1009: ")+CString("[MsgId - ")+m_MMObj->LongToCString(OutGoingMsg->GetMessageId())+CString("] Invalid Format Name - [")+FormatName+CString("]\n") ;
					ErrorStatus = ErrorStatus || 1;
				}
					}
					else
					{
						m_XMLErrors = m_XMLErrors+CString("ERROR 1010: Format list is Empty\n") ;
						ErrorStatus = ErrorStatus || 1;
					}
				}
				else // format is ""
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR 1011: ")+CString("[MsgId - ")+m_MMObj->LongToCString(OutGoingMsg->GetMessageId())+CString("] Invalid Format entry \"\" \n");
					ErrorStatus = ErrorStatus || 1;
			}

		}
	}
	else
	{
		m_XMLErrors = m_XMLErrors+CString("ERROR 1035: Incomplete data for Outgoing Message\n");
		ErrorStatus = ErrorStatus || 1;
	}
		
	}
	}
	else
	{
		AfxMessageBox(_T("ERROR 1012: Outgoing Msg list is Empty"));
		ErrorStatus = ErrorStatus || 1;
	}

	int index;
	for(index=0;index<=MAX_APPLICATIONS-1;index++)
	{
		if(AppIdError[index].GetLength()>0)
		{
			m_XMLErrors = m_XMLErrors+CString("ERROR 1013: ")+CString("[For Outgoing Messages - ")+AppIdError[index]+CString(" : Invalid AppId - [")+m_MMObj->LongToCString(index)+CString("]\n");
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


int  CXMLVerifier::CrossCheckFormatAndDataset()
{
	// Algo Specification
	// Take one format, Get its var list, cout var list let x
	// Find datalist name for this format
	// Find dataset for this datalist
	// count no of var in this datalist let y
	// Compare x and y, they must be equal



	CDataList cdatalist,*ptrdatalist; 
	CData dataObj;
	CList <CString,CString&> CheckedDatasetList;
	
	int ErrorStatus=0;
    
	if(m_MMObj->m_pFormatList)
	{
		CFormat *pFormat;
		
		for(POSITION pos = m_MMObj->m_pFormatList->GetHeadPosition(); pos != NULL;)
		{
			
			pFormat = (CFormat*)m_MMObj->m_pFormatList->GetNext( pos ) ;
			
			//now for this format, obtain datalist
			cdatalist = pFormat->GetDataList();
			
			if((cdatalist.GetName()).GetLength()>0) //Datalist name is there
			{
				if(m_MMObj->m_pDataList)
				{
					
					for(POSITION pos = m_MMObj->m_pDataList->GetHeadPosition(); pos != NULL;) //search for the datalist name in data list ptrdatalist
					{
						ptrdatalist = (	CDataList*) m_MMObj->m_pDataList->GetNext( pos ) ;
						if(!(cdatalist.GetName()).CompareNoCase(ptrdatalist->GetName()))
						{

							CList<CData,CData&>& datasetlist = ptrdatalist->GetData(); 

							if(datasetlist.GetCount()) // Dataset list is there
							{					
								for( POSITION datapos = datasetlist.GetHeadPosition(); datapos != NULL;)
								{
									dataObj = (datasetlist.GetNext( datapos ));

									if(IsAllreadyCheckedForReplication(CheckedDatasetList,dataObj.GetName())!=1)
									{
									if(  dataObj.GetValueList().GetCount()!= pFormat->GetVarList().GetCount())
									{
											m_XMLErrors = m_XMLErrors+CString("ERROR 1031: Format - [")+pFormat->GetName()+CString("] DataList - [")+ptrdatalist->GetName()+CString("] Dataset - [")+dataObj.GetName()+CString("] Conflict in no. of Var\n");
										ErrorStatus = ErrorStatus || 1;
											FoundAndCheckedForReplication(CheckedDatasetList,dataObj.GetName());

									}
								}

								}
							}
						
						}
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

int CXMLVerifier::CheckFormatReplication(CString FormatName)
{
	int occurance=0;
	if(m_MMObj->m_pFormatList)
	{
		CFormat *pFormat;
		
		for(POSITION pos = m_MMObj->m_pFormatList->GetHeadPosition(); pos != NULL;)
		{
			pFormat = (CFormat*)m_MMObj->m_pFormatList->GetNext( pos ) ;
			
			if((pFormat->GetName()).GetLength()) 
			{
				if(!pFormat->GetName().CompareNoCase(FormatName))
				{
					occurance++;
				}
			}
			
		}
	}
	return occurance>1?1:0;
	
}

	
void CXMLVerifier::FoundAndCheckedForReplication(CList <CString,CString&> &CheckedList, CString Checked)
{
	CheckedList.AddTail(Checked);
}


int CXMLVerifier::IsAllreadyCheckedForReplication(CList <CString,CString&> &CheckedList, CString Check)
{
	CString strObj;
	int isChecked =0;
	if(CheckedList.GetCount())
	{
		for(POSITION pos = CheckedList.GetHeadPosition();pos!=NULL;)
		{
			strObj = (CString) CheckedList.GetNext(pos);
			if(!strObj.CompareNoCase(Check))
			{
				isChecked =1;
			}
	
		}
	}

	return isChecked;
}


int CXMLVerifier::VerifyFormatList()
{
	//get one format from format list
	//find the data list name associated with it
	//Check that datalist name in the list of datalist
	CDataList cdatalist,*ptrdatalist; 
	CString inputmode,MsgIdsForSameFormat;
	int DataListNameStatus,ErrorStatus=0;

	CList <CString,CString&> CheckedFmtList;
    
	if(m_MMObj->m_pFormatList)
	{
		CFormat *pFormat;
		
		for(POSITION pos = m_MMObj->m_pFormatList->GetHeadPosition(); pos != NULL;)
		{
			pFormat = (CFormat*)m_MMObj->m_pFormatList->GetNext( pos ) ;
			if((pFormat->GetName()).GetLength()) // if format is there but format name is not defined
			{

				//------- Check that is this format is associated with more than one messages ? --------------
				MsgIdsForSameFormat.Empty();
				
				if(CheckOneFmtMultiMsgAssociativity(pFormat->GetName(),MsgIdsForSameFormat)==1)
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR 1014: Format - [")+pFormat->GetName()+CString("] is associated with Msgs [")+MsgIdsForSameFormat+CString("]\n");
					ErrorStatus = ErrorStatus || 1;
				}
				
				
				//---------------------------------------------------------------------------------------------

				
				//------- Check for redefination of format ----------------------------------------------------
				//Some wot we have to write in error like this : format ----- is duplicate
				

				if(IsAllreadyCheckedForReplication(CheckedFmtList,pFormat->GetName())!=1)
				{
					if(CheckFormatReplication(pFormat->GetName()) ==1)
					{
						m_XMLErrors = m_XMLErrors+CString("ERROR 1015: Format - [")+pFormat->GetName()+CString("] is redefined \n");
						ErrorStatus = ErrorStatus || 1;
						FoundAndCheckedForReplication(CheckedFmtList,pFormat->GetName());
					}
				}
				
				//---------------------------------------------------------------------------------------------


                cdatalist = pFormat->GetDataList();
				if((cdatalist.GetName()).GetLength()>0) //Datalist name is there
				
				{
					if(m_MMObj->m_pDataList)
					{
						DataListNameStatus=0;
						for(POSITION pos = m_MMObj->m_pDataList->GetHeadPosition(); pos != NULL;) //search for the datalist name in data list ptrdatalist
						{
							ptrdatalist = (	CDataList*) m_MMObj->m_pDataList->GetNext( pos ) ;
							if(!(cdatalist.GetName()).CompareNoCase(ptrdatalist->GetName()))
							{
								DataListNameStatus=1;
								break;							
							}
						}
						if(DataListNameStatus==0) // Datalist does not exists
						{
							m_XMLErrors = m_XMLErrors+CString("ERROR 1016: Format - [")+pFormat->GetName()+CString("] DataList Name not found in DataLists\n");
							ErrorStatus = ErrorStatus || 1;
						}


					}
					else // if datlist is not there
					{
					
					}
				}
				else // format is there but datalist name is not there
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR 1017: Format - [")+pFormat->GetName()+CString("] Invalid DataList Name\n");
					ErrorStatus = ErrorStatus || 1;
				}
				
				
				inputmode = pFormat->GetInputMode(); // get the input mode
				if(inputmode.GetLength()==0) // Input  mode is empty
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR 1018: Format - [")+pFormat->GetName()+CString("] InputMode not defined\n");
					ErrorStatus = ErrorStatus || 1;
				}
				else
				{
					if((inputmode.CompareNoCase(CString("Dialog"))) && (inputmode.CompareNoCase(CString("Dataset")))) // Input mode is something other than Dialog,Dataset
					{
						m_XMLErrors = m_XMLErrors+CString("ERROR 1019: Format - [")+pFormat->GetName()+CString("] InputMode can be Dialog or Dataset\n");
						ErrorStatus = ErrorStatus || 1;
					}
				}
			}
			else
			{
				m_XMLErrors = m_XMLErrors+CString("ERROR 1020: Format - [")+pFormat->GetName()+CString("] Format is defined without name\n");
				ErrorStatus = ErrorStatus || 1;
			}
		}
	}
	else
	{
		m_XMLErrors = m_XMLErrors+CString("ERROR 1021: Format List is not available\n");
		ErrorStatus = ErrorStatus || 1;

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

int CXMLVerifier::CheckDataListRedefination(CString DatalistName)
{
	CDataList cdatalist,*ptrdatalist; 
	int Occurance=0;

	if(m_MMObj->m_pDataList && m_MMObj->m_pDataList->GetCount())
	{
		for(POSITION pos = m_MMObj->m_pDataList->GetHeadPosition(); pos != NULL;) 
		{
			ptrdatalist = (	CDataList*) m_MMObj->m_pDataList->GetNext( pos ); // ptrdatalist  is having first node
		
			if((ptrdatalist->GetName()).GetLength()) // name tag of datalist object itself should be there 
			{	
				if(!ptrdatalist->GetName().CompareNoCase(DatalistName))
				{
					Occurance++;
				}
			}
		}
	}

	return Occurance>1?1:0;
}


int CXMLVerifier::CheckForRedefinationofDataset(CString DatalistName, CString DatasetName)
{
	CDataList cdatalist,*ptrdatalist; 
	CData dataObj;
	int occurance=0;


	if(m_MMObj->m_pDataList && m_MMObj->m_pDataList->GetCount())
	{
		
		for(POSITION pos = m_MMObj->m_pDataList->GetHeadPosition(); pos != NULL;) 
		{
			ptrdatalist = (	CDataList*) m_MMObj->m_pDataList->GetNext( pos );
		
			if((ptrdatalist->GetName()).GetLength()) // name tag of datalist object itself should be there 
			{	
				if(!ptrdatalist->GetName().CompareNoCase(DatalistName))
				{
				
					CList<CData,CData&>& datasetlist = ptrdatalist->GetData(); 
					if(datasetlist.GetCount()) // Dataset list is there
					{					
						for( POSITION datapos = datasetlist.GetHeadPosition(); datapos != NULL;)
						{
							dataObj = (datasetlist.GetNext( datapos ));
							if(!dataObj.GetName().CompareNoCase(DatasetName))
							{
								occurance ++;
							}
						}
					}
					return occurance>1?1:0;
				}
			}
		}
	}

  return occurance;
}


int CXMLVerifier::VerifyDataList()
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

	CList <CString,CString&> CheckedDatalistList;
	CList <CString,CString&> CheckedDatasetList;




	if(m_MMObj->m_pDataList && m_MMObj->m_pDataList->GetCount())
	{
		
		for(POSITION pos = m_MMObj->m_pDataList->GetHeadPosition(); pos != NULL;) 
		{
			ptrdatalist = (	CDataList*) m_MMObj->m_pDataList->GetNext( pos ); // ptrdatalist  is having first node
		
			if((ptrdatalist->GetName()).GetLength()) // name tag of datalist object itself should be there 
			{	
				//Check for redefination of Datalist

				if(IsAllreadyCheckedForReplication(CheckedDatalistList,ptrdatalist->GetName())!=1)
				{
					if(CheckDataListRedefination(ptrdatalist->GetName()) ==1)
					{
						m_XMLErrors = m_XMLErrors+CString("ERROR 1022: Redefinatin of Datalist - [") +ptrdatalist->GetName()+CString("]\n");		
						ErrorStatus=ErrorStatus||1;
						FoundAndCheckedForReplication(CheckedDatalistList,ptrdatalist->GetName());
					}
				}





				if((ptrdatalist->GetDefault()).GetLength() >0)
				{
					if((ptrdatalist->GetDefault()).CompareNoCase(CString("")))// data set is not having "NULL" tag
					{
						CList<CData,CData&>& datasetlist = ptrdatalist->GetData(); //data set list, now we have to search that dafault
						       							   // tag in the list of dataset and it should be there	
						if(datasetlist.GetCount()) // Dataset list is there
						{					
							for( POSITION datapos = datasetlist.GetHeadPosition(); datapos != NULL;)
							{
								DataSetNameStatus=0;
								dataObj = (datasetlist.GetNext( datapos ));
																

								if(IsAllreadyCheckedForReplication(CheckedDatasetList,dataObj.GetName())!=1)
								{
									if(CheckForRedefinationofDataset(ptrdatalist->GetName(),dataObj.GetName()) ==1)
									{
										m_XMLErrors = m_XMLErrors+CString("ERROR 1023: DataList - [")+ptrdatalist->GetName()+CString("] Dataset [")+dataObj.GetName()+CString("] is redefined\n");		
										ErrorStatus=ErrorStatus||1;
										FoundAndCheckedForReplication(CheckedDatasetList,dataObj.GetName());
									}
								}

								
								if(!dataObj.GetName().CompareNoCase(ptrdatalist->GetDefault())||(!ptrdatalist->GetDefault().CompareNoCase(_T("Null")))) // 
								{
									DataSetNameStatus=1;
									break;
								}
							}

							if(DataSetNameStatus==0)
                            {
								m_XMLErrors = m_XMLErrors+CString("ERROR 1024: DataList - [")+ptrdatalist->GetName()+CString("] Default Dataset is not there in the list of Dataset\n");		
								ErrorStatus = ErrorStatus || 1;
							}
						}
						else
						{
							//m_XMLErrors = m_XMLErrors+CString("ERROR : DataList - [")+cdatalist.GetName()+CString("] Dataset list is not there\n");
						}

					}
					
				}
				else // default tag is empty
				{
					m_XMLErrors = m_XMLErrors+CString("ERROR 1025: DataList - [")+ptrdatalist->GetName()+CString("] DataList's Default tag can'nt be Empty\n");
					ErrorStatus = ErrorStatus || 1;
				}
			}
			else // datalist name tag  is not there
			{
				m_XMLErrors = m_XMLErrors+CString("ERROR 1026: DataList is there, but with out name\n");		
				ErrorStatus = ErrorStatus || 1;
			}
		}

	}
	else
	{
		m_XMLErrors = m_XMLErrors+CString("ERROR 1027: DataList is Empty\n");	
		ErrorStatus = ErrorStatus || 1;
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

int CXMLVerifier::IsAnAsyncMessage(UINT32 MessageId)
{
	CAsyncMessageSeq *ptrAsyncMsgSeq;
	UINT32 msgid;
	int foundStatus=0;
	
	COutgoingMessage *outgoing_msg = NULL; 
	for(POSITION pos = m_MMObj->m_plstAsyncMsg->GetHeadPosition(); pos != NULL;) 
	{
		ptrAsyncMsgSeq = (CAsyncMessageSeq*) m_MMObj->m_plstAsyncMsg->GetNext(pos); 
		CList<UINT32,UINT32&>& asyncmsgseq = ptrAsyncMsgSeq->GetMessageList();
		if(asyncmsgseq.GetCount())
		{
			
			for( POSITION datapos = asyncmsgseq.GetHeadPosition(); datapos != NULL;)
			{
				msgid = (asyncmsgseq.GetNext(datapos));
				if(msgid ==MessageId)
				{
					foundStatus=1;
					break;
				}
			}
		}
		if(foundStatus ==1) break;
	}
	return foundStatus;
}


int CXMLVerifier::VerifyAsyncMsg(CPtrList *plstAsyncMsg)
{
	CAsyncMessageSeq *ptrAsyncMsgSeq;
	UINT32 msgid;
	int msgidvalidstatus =0,ErrorStatus=0;
	COutgoingMessage *outgoing_msg = NULL;
  if (!m_MMObj->m_plstAsyncMsg)
  {
    return 0;
  }
	for(POSITION pos = plstAsyncMsg->GetHeadPosition(); pos != NULL;) 
	{
		ptrAsyncMsgSeq = (CAsyncMessageSeq*) plstAsyncMsg->GetNext(pos); 
		CList<UINT32,UINT32&>& asyncmsgseq = ptrAsyncMsgSeq->GetMessageList();
		if(asyncmsgseq.GetCount())
		{
			
			for( POSITION datapos = asyncmsgseq.GetHeadPosition(); datapos != NULL;)
			{
				msgid = (asyncmsgseq.GetNext(datapos));
						
				if(m_MMObj->m_plstOutGoingMsg)
				{
					msgidvalidstatus = 0;
					for(POSITION pos=m_MMObj->m_plstOutGoingMsg->GetHeadPosition();pos!=NULL;)
					{
						outgoing_msg = (COutgoingMessage*)m_MMObj->m_plstOutGoingMsg->GetNext(pos);
						if(outgoing_msg->GetMessageId() ==msgid)
						{
							msgidvalidstatus=1; // Response Message id is there, its ok
							break;			// break the loop
						}
					}

					if(msgidvalidstatus ==0) // ionvalid msg id
					{
						m_XMLErrors = m_XMLErrors+CString("ERROR 1028: ")+CString("[MsgId - ")+m_MMObj->LongToCString(msgid)+CString("] Message Id not found, Invalid Async Message Sequence - ")+m_MMObj->LongToCString(ptrAsyncMsgSeq->GetMessageId())+CString("\n") ;
						ErrorStatus = ErrorStatus || 1;
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


int CXMLVerifier::VerifyAppList()		
{return 1;}

int CXMLVerifier::VerifyBootupList(CPtrList* pBootupList)	
{
	CBootUpMsg *ptrBootUpMsgSeq;
	UINT32 msgid;
	int msgidvalidstatus =0,ErrorStatus=0;
	COutgoingMessage *outgoing_msg = NULL;
	
	if(m_MMObj->m_pBootupList)
	{
		for(POSITION pos = pBootupList->GetHeadPosition(); pos != NULL;) 
		{
			ptrBootUpMsgSeq = (CBootUpMsg*) pBootupList->GetNext(pos); 
			CList<UINT32,UINT32>& bootupmsglst = ptrBootUpMsgSeq->GetBootMessageList();
			if(bootupmsglst.GetCount())
			{
				
				for( POSITION datapos = bootupmsglst.GetHeadPosition(); datapos != NULL;)
				{
					msgid = (bootupmsglst.GetNext( datapos ));
					if(m_MMObj->m_plstOutGoingMsg)
					{
						msgidvalidstatus = 0;
						for(POSITION pos=m_MMObj->m_plstOutGoingMsg->GetHeadPosition();pos!=NULL;)
						{
							outgoing_msg = (COutgoingMessage*)m_MMObj->m_plstOutGoingMsg->GetNext(pos);
							if(outgoing_msg->GetMessageId() ==msgid)
							{
								msgidvalidstatus=1; // Response Message id is there, its ok
								break;			// break the loop
							}
						}

						if(msgidvalidstatus ==0) // ionvalid msg id
						{
							m_XMLErrors = m_XMLErrors+CString("ERROR 1029: ")+CString("[MsgId - ")+m_MMObj->LongToCString(msgid)+CString("] Message Id not found, Invalid Boot Up Sequence - ")+m_MMObj->LongToCString(ptrBootUpMsgSeq->GetMessageId())+CString("\n") ;
							ErrorStatus=ErrorStatus||1;
						}
					}
				}		
			}
		}
	}
	else
	{
		m_XMLErrors = m_XMLErrors+CString("ERROR 1033: Bootup Message list is not defined in XML \n") ;
		ErrorStatus=ErrorStatus||1;
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

int CXMLVerifier::VerifyFreqList()	
{
	CFreqUsedMsg *ptrFreqUsedMsgSeq;
	UINT32 seqid;
	int seqidvalidstatus =0,ErrorStatus=0;

	CAsyncMessageSeq *sequence; 
	if(m_MMObj->m_pFreqList)
	{
		for(POSITION pos = m_MMObj->m_pFreqList->GetHeadPosition(); pos != NULL;) 
		{
			ptrFreqUsedMsgSeq = (CFreqUsedMsg*) m_MMObj->m_pFreqList->GetNext(pos); 
			CList<UINT32,UINT32>& frequsedmsglst = ptrFreqUsedMsgSeq->GetFreqUsedMessageList();
			if(frequsedmsglst.GetCount())
			{

				for( POSITION datapos = frequsedmsglst.GetHeadPosition(); datapos != NULL;)
				{
					seqid = (frequsedmsglst.GetNext( datapos ));
					if(m_MMObj->m_plstAsyncMsg)
					{
						seqidvalidstatus = 0;
						for(POSITION pos=m_MMObj->m_plstAsyncMsg->GetHeadPosition();pos!=NULL;)
						{
							sequence = (CAsyncMessageSeq*)m_MMObj->m_plstAsyncMsg->GetNext(pos);
							if(sequence->GetMessageId() ==seqid)
							{		
								seqidvalidstatus=1; // Sequence id is there, its ok
								break;			// break the loop
							}
						}

						if(seqidvalidstatus ==0) // ionvalid sequence id
						{
							m_XMLErrors = m_XMLErrors+CString("ERROR 1030:")+CString("[seqid - ")+m_MMObj->LongToCString(seqid)+CString(" ]Sequence Id not found")+CString("\n") ;
							ErrorStatus=ErrorStatus||1;
						}
					}
				}		
			}
		}
	}
	else
	{
		m_XMLErrors = m_XMLErrors+CString("ERROR 1032: Frequently used Message list is not defined in XML \n") ;
		ErrorStatus=ErrorStatus||1;
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