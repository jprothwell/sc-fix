/***************************************************************************
*
* File Name : jmms_receive.c
*
* IMPORTANT NOTICE
*
* Please note that any and all title and/or intellectual property rights 
* in and to this Software or any part of this (including without limitation 
* any images, photographs, animations, video, audio, music, text and/or 
* "applets," incorporated into the Software), herein mentioned to as 
* "Software", the accompanying printed materials, and any copies of the 
* Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
* or Jataayu's suppliers as the case may be. The Software is protected by 
* copyright, including without limitation by applicable copyright laws, 
* international treaty provisions, other intellectual property laws and 
* applicable laws in the country in which the Software is being used. 
* You shall not modify, adapt or translate the Software, without prior 
* express written consent from Jataayu. You shall not reverse engineer, 
* decompile, disassemble or otherwise alter the Software, except and 
* only to the extent that such activity is expressly permitted by 
* applicable law notwithstanding this limitation. Unauthorized reproduction 
* or redistribution of this program or any portion of it may result in severe 
* civil and criminal penalties and will be prosecuted to the maximum extent 
* possible under the law. Jataayu reserves all rights not expressly granted. 
* 
* THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
* KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
* FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
* DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
* ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
* SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
* EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
* ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
* OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
* TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
*
**************************************************************************
*
* File Description
* ----------------

***************************************************************************/
#ifdef JATAAYU_SUPPORT
#include "jmms_inc.h"
#include "jmms_interface.h"
#include "jmms_communicator.h"
#include "jmms_sendrecv.h"
#include "jmms_composeutils.h"
#include "jmms_cb.h"
#include "jmms_composedlg.h"
#ifdef __MMI_SUPPORT_JME__
extern s8 vm_status;
#endif
#include "jwap_push.h"
#include "messagesmiscell.h"
#include "gpioinc.h" 

extern BOOL checkVMOn(void);

JC_RETCODE jmms_SendMms (JC_BOOLEAN bIsEdit)
{
	JC_RETCODE			eRet			= JC_OK ;
	MESSAGE_HEADERS		*pMsgHeaders	= JC_NULL ;
	CONTENT_DATA		EncapsulatedContent = {0, } ;
	JC_CHAR *psURL;
	JC_UINT32 uiMessagID = 0;
	E_JMMS_MSG_SUBSTATE	eMsgSubState = E_JMMS_MSG_NIL ;
	JC_UINT32 uiSimId = 0;// deafult is SIM1
	
	
	jmms_UpdateMessageHeaders();
	eRet = jdi_MsgGetMessageHeaders (g_pstJMmsAppInfo->hCurrMsg, &pMsgHeaders) ;
	

    mmi_trace( 1, "JDD_LOG: DR = %d, RR = %d, PR =%d, ET= %d, DT = %d" ,g_pstJMmsAppInfo->stMMSSetting.bDeliveryReport,
		g_pstJMmsAppInfo->stMMSSetting.bReadReport,g_pstJMmsAppInfo->stMMSSetting.ePriority + 1,
		g_pstJMmsAppInfo->stMMSSetting.uiExpiryTime, g_pstJMmsAppInfo->stMMSSetting.uiDeliveryTime) ;
		             
	
	eRet = jdi_MsgGetEncapsulatedBuffer (g_pstJMmsAppInfo->hCurrMsg, &EncapsulatedContent) ;
	if (JC_OK == eRet)
	{
#ifdef __MMI_MULTI_SIM__
		uiSimId = g_pstJMmsAppInfo->send_simid ;
#endif	
		mmi_trace(1, "JDD_LOG: storing the sending message in outbox") ;
		eRet = jMMS_SaveOrUpdateMsg (E_JMMS_OUTBOX_FOLDER, &uiMessagID, &EncapsulatedContent, pMsgHeaders, uiSimId) ;
		mmi_trace(1, "JDD_LOG: jmms_SendMms::jMMS_SaveOrUpdateMsg ulMessagID value is [%d] and rCode [%X]", uiMessagID, -eRet) ;
	}
	if (JC_OK == eRet)
	{
		mmi_trace(1, "JDD_LOG: stored in outbox with msgid %d retcode %d",  uiMessagID, eRet);
		
		jdi_CUtilsCharToTchar (JC_NULL,g_pstJMmsAppInfo->pcMMSCUrl, &psURL) ;
		switch (g_pstJMmsAppInfo->eCurrentFolderID)
		{
			case E_JMMS_INBOX_FOLDER:
				eMsgSubState = E_JMMS_MSG_EDIT_FROM_INBOX ;
				break ;
			case E_JMMS_OUTBOX_FOLDER:
				eMsgSubState = (E_TRUE == bIsEdit) ?E_JMMS_MSG_EDIT_FROM_OUTBOX :E_JMMS_MSG_SEND_FRM_OUTBOX ;
				break ;
			case E_JMMS_SENT_FOLDER:
				eMsgSubState = (E_TRUE == bIsEdit) ?E_JMMS_MSG_EDIT_FROM_SEND :E_JMMS_MSG_SEND_COPY ;
				break ;
			case E_JMMS_DRAFT_FOLDER:
				eMsgSubState = E_JMMS_MSG_EDIT_FROM_DRAFTS ;
				break ;
			case E_JMMS_NONE_FOLDER:
				eMsgSubState = E_JMMS_MSG_NEW_COMPOSE ;
				break ;
			default:
				break ;
		}
		eRet = jMMS_ProcessCommunicatorFetchRequest (uiMessagID, E_JMMS_NODE_SEND, eMsgSubState) ;
	}
	if(EncapsulatedContent.bIsStream == E_FALSE)
	{
		jdd_MemFree(EncapsulatedContent.pBuffer) ;
	}	
	else
	{
		if(EncapsulatedContent.pStream != JC_NULL)
		{
			jdd_MemFree(EncapsulatedContent.pStream) ;
		}
	}	
	return eRet ;
}

JC_RETCODE jMMS_HandleFinalDownloadState (CONTENT_DATA *pMMSData, JC_UINT32 uiMessageID)
{
	JC_RETCODE			eRet = JC_OK ;
	MSG_HANDLE			hTempMsg =JC_NULL ;
	MESSAGE_HEADERS 	*pMsgHeaders =JC_NULL ;

	mmi_trace(12, "JDD_LOG: entered in jMMS_HandleFinalDownloadState");
	if (pMMSData != JC_NULL)
	{
		eRet = jdi_MsgParseEncapsulatedBuffer (&hTempMsg, pMMSData, E_TYPE_MMS, E_FALSE) ;
		mmi_trace(12, "JDD_LOG: jdi_MsgParseEncapsulatedBuffer rcode is [%d]", eRet );
		if (eRet == JC_OK)
		{
			eRet = jdi_MsgGetMessageHeaders (hTempMsg, &pMsgHeaders) ;
			mmi_trace(12, "JDD_LOG: jdi_MsgGetMessageHeaders rcode is [%d] and pMsgHeaders is [%d]", eRet, pMsgHeaders) ;
		}			
		if (eRet == JC_OK && JC_NULL != pMsgHeaders)
		{
			mmi_trace(12, "JDD_LOG: message type %d", pMsgHeaders->eMsgType) ;
			switch (pMsgHeaders->eMsgType)
			{
				case E_MSG_TYPE_SEND_RESPONSE:
				{
					MESSAGE_SEND_RESPONSE *pSendHdr = (MESSAGE_SEND_RESPONSE *)pMsgHeaders->pHeaders ;
					
					mmi_trace(12, "JDD_LOG: E_MSG_TYPE_SEND_RESPONSE, HTTP Status [%d], MSG ID [%u]", pSendHdr->eStatus, uiMessageID) ;
					if (pSendHdr->eStatus == E_RESP_OK)
					{
						if (uiMessageID != 0)
						{
							if(g_MMS_send_require == SEND_AND_SAVE)
							{
								mmi_trace(12, "JDD_LOG: moving message %d from outbox to sent items", uiMessageID);
								eRet = jmms_MoveMmsByID (E_JMMS_OUTBOX_FOLDER, E_JMMS_SENT_FOLDER, uiMessageID) ;
								mmi_trace(12, "JDD_LOG: jmms_MoveMmsByID returns [%X]", -eRet);
							}
							else if(g_MMS_send_require == SEND_ONLY)
							{
								mmi_trace(12, "JDD_LOG: delete message %d from outbox", uiMessageID);
								eRet = jMMS_DeleteMsg(E_JMMS_OUTBOX_FOLDER, uiMessageID, E_FALSE);
								mmi_trace(12, "JDD_LOG: jMMS_DeleteMsg returns [%X]", -eRet);
							}
							jmms_UpdateMmsIcon();
						}
					}
					else
					{
						eRet = JC_ERR_GENERAL_FAILURE ;
						mmi_trace(12, "JDD_LOG: send fail with HTTP status [%d]", pSendHdr->eStatus) ;
					}			
				}
				break ;
				case E_MSG_TYPE_RECEIVE:
				{
					mmi_trace(0 , "JDD_LOG: saving in inbox with message ID [%d]", uiMessageID) ;
					/* the message is  updated with existing notification.  extrabytes parmeter can be 0 as this paramter is 
						not used in update case */
					eRet = jMMS_SaveOrUpdateMsg (E_JMMS_INBOX_FOLDER, &uiMessageID, pMMSData, pMsgHeaders, 0) ;
					// commented as new mms icon is shown when we get a notification
					if (eRet == JC_OK)
						jmms_UpdateMmsIcon () ;
					mmi_trace(12, "JDD_LOG: jMMS_SaveOrUpdateMsg rcode is [%d]", eRet );
				}
				break;
				case E_MSG_TYPE_READ_REPORT_RESPONSE:
				{
					/* TODO */
					mmi_trace (0, "JDD_LOG: jMMS_HandleFinalDownloadState >> TODO E_MSG_TYPE_READ_REPORT_RESPONSE is NOT HANDLED") ;
				}
				break;
				default:
				break;
			}
			if (JC_NULL != hTempMsg)
		    {
				jdi_MsgDeInit (hTempMsg) ;
		    }
		}
		else
		{
			mmi_trace (0, "JDD_LOG: jMMS_HandleFinalDownloadState::jdi_MsgParseEncapsulatedBuffer () returned [%d]", eRet) ;
		}
	}	
	return eRet ;
}


#ifdef JOTA_PROVISION_APP_ENABLE	
void prv_AddToProvList(ST_COMM_PUSH_IND *pstProvPushData);
void prv_MMIDisplayNewProvAlertScreen();

#endif

#ifdef MANUAL_DOWNLOAD_MMS
#include "cameraapp.h"
extern camera_context_struct g_camera_cntx;
#include "vdorecgprot.h"
#include "vdoplygprot.h"
#endif


JC_RETCODE jMMS_HandlePushMsg (void *pvAppData, ST_COMM_PUSH_IND *pstPushData)
{
	MSG_HANDLE			hMsgHandle ;
	MESSAGE_HEADERS		*pstMsgHdrs ;
	CONTENT_DATA		stPushInfo ={0, } ;
	JC_RETCODE			rCode = JC_OK ;
	JC_UINT32 			uiSimId = 0 ; // deafult is SIM1 for single SIM
	mmi_trace(12,"JPROV_LOG: jMMS_HandlePushMsg recvd");
	
	if (pstPushData != JC_NULL && pstPushData->pcContentType != JC_NULL &&
		pstPushData->pucBuffer != JC_NULL)
	{
		if (jc_strstr(pstPushData->pcContentType, CS_MMS_CONTENT_TYPE) )
		{
			stPushInfo.pBuffer = pstPushData->pucBuffer ;
			stPushInfo.uiTotalSize = stPushInfo.uiCurSize = pstPushData->uiLength ;
			rCode = jdi_MsgParseEncapsulatedBuffer (&hMsgHandle, &stPushInfo, E_TYPE_MMS, E_FALSE) ;
			if (JC_OK == rCode)
			{
				rCode = jdi_MsgGetMessageHeaders (hMsgHandle, &pstMsgHdrs) ;
				if (JC_OK == rCode)
				{
					JC_UINT32				uiMsgIndex = 0 ;
					#ifdef __MMI_MULTI_SIM__
					uiSimId  = jc_atoi( pstPushData->pcPPGHostAddress ); 
					#endif
					
					if (pstMsgHdrs->eMsgType == E_MSG_TYPE_NOTIFICATION)
					{
						MESSAGE_NOTIFICATION	*pNotification = pstMsgHdrs->pHeaders ;
						

						mmi_trace(12,(PCSTR)"JDD_LOG: MMS_RecvReportRespEvt New MMS Download[%s]",pNotification->pContentLocation) ;
						if(JC_OK == rCode)
						{
							rCode = jMMS_SaveOrUpdateMsg (E_JMMS_INBOX_FOLDER, &uiMsgIndex, &stPushInfo, pstMsgHdrs, uiSimId) ;
							if (0 != uiMsgIndex)
							{
    							jmms_UpdateMmsIcon();
								if( !(
	                                                      #ifdef MANUAL_DOWNLOAD_MMS
									(g_camera_cntx.app_state != CAMERA_STATE_EXIT) 	  /* in camera app */
									#if defined(__MMI_VIDEO_RECORDER__)
									 || (mmi_vdorec_is_in_app())								  /* in video recorder */
									 #endif
									#if defined(__MMI_VIDEO_PLAYER__)
									 || (mmi_vdoply_is_in_app()) 									  /* in video player */  
									 #endif
									 ||
					                         #endif				 
	 					 	   	#if defined( __NATIVE_JAVA__)
									 (checkVMOn())||
								#endif	 
								         0
									 ) )
	                            
	                         			       {
									jMMS_ProcessCommunicatorFetchRequest (uiMsgIndex, E_JMMS_NODE_AUTO_RETRIEVE, E_JMMS_MSG_NIL) ;
                        	                            }
								else
								{
									TurnOnBacklight(1);
									PlaySmsVibratorOnce();

									DisplayPopup(GetString(STR_ID_MMS_MESSAGE_RECIEVED), IMG_GLOBAL_ACTIVATED, \
																			1, 2000, 13);

								}
							}
							else
							{
								//error handle
								if (FILE_MODULE == ((-rCode) & 0x00FFFF00) && JC_ERR_FILE_NOT_FOUND != rCode)
								{
									DisplayPopup (GetString (STR_ID_MMS_FILE_SYSTEM_FULL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
									jmms_CallbackGoBackHistory();
								}
								else
								{
									DisplayPopup (GetString (STR_ID_MMS_INF_FAIL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
									jmms_CallbackGoBackHistory();
								}
							}
						}
					}
					else if (pstMsgHdrs->eMsgType == E_MSG_TYPE_DELIVERY_REPORT ||
								pstMsgHdrs->eMsgType == E_MSG_TYPE_READ_REPORT_RESPONSE)
					{
						mmi_trace(12,"JDD_LOG: jMMS_HandlePushMsg >> Push Message of type [%d] recived", pstMsgHdrs->eMsgType) ;
						rCode = jMMS_SaveOrUpdateMsg (E_JMMS_INBOX_FOLDER, &uiMsgIndex, &stPushInfo, pstMsgHdrs, uiSimId) ;
						if (0 != uiMsgIndex)
						{
							jmms_UpdateMmsIcon();
							jmms_ShowNewMMSPopup(uiMsgIndex);
						}
						else
						{
							//error handle
							if (FILE_MODULE == ((-rCode) & 0x00FFFF00) && JC_ERR_FILE_NOT_FOUND != rCode)
							{
								DisplayPopup (GetString (STR_ID_MMS_FILE_SYSTEM_FULL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
								jmms_CallbackGoBackHistory();
							}
							else
							{
								DisplayPopup (GetString (STR_ID_MMS_INF_FAIL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
								jmms_CallbackGoBackHistory();
							}

						}
					}
					else
					{
						mmi_trace(1, "JDD_LOG: unhandled push information");
					}
					/* pstMsgHdrs data to be freed */
				}
				else
				{
					mmi_trace(1, "JDD_LOG: jdi_MsgGetMessageHeaders () failed with rCode [%X]", -rCode) ;
				}
				jdi_MsgDeInit (hMsgHandle) ;
			}
			else
			{
				mmi_trace(1, "JDD_LOG: jdi_MsgParseEncapsulatedBuffer () failed with rCode [%X]", -rCode) ;
			}
		}
#ifdef JOTA_PROVISION_APP_ENABLE		
		else if(jc_strstr (pstPushData->pcContentType, CS_OTA_CONTENT_TYPE1) ||
			jc_strstr (pstPushData->pcContentType, CS_OTA_CONTENT_TYPE2) )
		{
			mmi_trace(1, "JOTA_LOG:push recived");
	
			prv_AddToProvList(pstPushData);
			if(IsMMIInIdleState())
			{
				prv_MMIDisplayNewProvAlertScreen();
			}
			
			//jprv_MMIDisplayNewProvAlertScreen();
			
		}
#endif //	JOTA_PROVISION_APP_ENABLE	
		else
		{
#ifdef JWAP_PUSH_ENABLE		
			mmi_trace(1, "JDD_LOG: pstPushData->pcContentType [%s]", pstPushData->pcContentType) ;
			if (jc_strstr (pstPushData->pcContentType, CS_WAP_SI_CONTENT_TYPE) ||
				jc_strstr (pstPushData->pcContentType, CS_WAP_SIC_CONTENT_TYPE) ||
				jc_strstr (pstPushData->pcContentType, CS_WAP_SL_CONTENT_TYPE) ||
				jc_strstr (pstPushData->pcContentType, CS_WAP_SLC_CONTENT_TYPE))
			{
				jWap_HandlePushMsg (pstPushData) ;
			}
#endif	//JWAP_PUSH_ENABLE		
		}
		/* pstPushData data to be freed */
	}
	return rCode ;
}

void jMMS_SendSelfEvent (E_JMMS_APP_EVENT eEventToSend, ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA *pstEventInfo)
{
	JC_EVENT_DATA 		stEvent ;
	JC_RETCODE			eRet ;
	
	/* Send a self event to the application indicating to start processing the next node */
	memset (&stEvent, 0, sizeof(JC_EVENT_DATA)) ;
	stEvent.srcTaskInfo.iTaskID   = 
	stEvent.srcTaskInfo.iAppID	  = 
	stEvent.destTaskInfo.iTaskID  = 
	stEvent.destTaskInfo.iAppID   = E_TASK_MMS_UI ;
	stEvent.iEventType				= eEventToSend ;
	stEvent.uiEventInfoSize = sizeof (ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA) ;
	eRet = jdd_QueueAlloc (&stEvent) ;
	if (JC_OK == eRet)
	{
		ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA *pstEventData = (ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA *)stEvent.pEventInfo ;
		jc_memcpy (pstEventData, pstEventInfo, stEvent.uiEventInfoSize) ;
		jdd_QueueSend (&stEvent) ;
	}
}

#endif
