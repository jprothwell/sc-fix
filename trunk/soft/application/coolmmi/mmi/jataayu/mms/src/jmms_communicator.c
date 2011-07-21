/***************************************************************************
*
* File Name : jmms_Communicator.c
*
* IMPORTANT NOTICE
*
* Please note that any and all title and/or intellectual property rights 
* in and to this Software or any part of this (including without limitation 
* any images, photographs, animations, video, audio, music, text and/or 
* "applets," incorporated into the Software), herein mentioned to as 
* "Software", the accompanying printed materials, and any copies of the 
* Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu") 
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
***************************************************************************
*
*
* File Description
* ----------------
*
* Purpose			: 
* Created By		: 
* Created Date		: 
*
* 
*
* Current Revision	:
*
***************************************************************************
*
*
* Revision Details
* ----------------
* 
* 1. Modified By, Modified Date, Purpose
*
*
*
*
***************************************************************************/
#ifdef JATAAYU_SUPPORT
/***************************************************************************
* Include Files
**************************************************************************/
#include "jmms_inc.h"
#include "jmms_cb.h"
#include "jmms_interface.h"
#include "jmms_communicator.h"
#include "jmms_utils.h"
#include "jmms_staticdefines.h"
#include "jmms_sendrecv.h"
#include "fs.h"
#include "errorcode.h"
#include "settingprofile.h"
#include "datetimegprot.h"
#include "phonesetupgprots.h"
#include "worldclock.h"
#include "jmms_composedlg.h"
#include "messagesresourcedata.h"

/***************************************************************************
* Globals and Structures
**************************************************************************/
/***************************************************************************
* Macros Definitions
**************************************************************************/

/***************************************************************************
* Global variables declarations
**************************************************************************/

/***************************************************************************
* Local Function Prototypes
***************************************************************************/
extern void TurnOnBacklight(U8 TimerFlag);
extern U16 g_MMS_send_require;

JC_CHAR* jMMS_ExtractContentURLFromNotification (JC_UINT32 uiMessageID)
{
	JC_RETCODE				rCode = JC_OK ;
	RETRIEVE_FILTERS		stStoreRetFilter = {0, } ;
	MESSAGE_LIST			*pstMsgList = JC_NULL ;
	MESSAGE_NOTIFICATION	*pstNotification ;
	JC_CHAR				*pContentLocationUrl	= JC_NULL ;
						
	stStoreRetFilter.uiStartIndex = 1 ;
	stStoreRetFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
	rCode = jmms_GetMsgDetailsByID (g_pstJMmsAppInfo->hMsgStore, E_JMMS_INBOX_FOLDER, uiMessageID, &pstMsgList) ;
	if (JC_OK == rCode && JC_NULL != pstMsgList)
	{
		if (pstMsgList->pMessageHeaders &&
			E_MSG_TYPE_NOTIFICATION == pstMsgList->pMessageHeaders->eMsgType)
		{
			pstNotification = pstMsgList->pMessageHeaders->pHeaders ;
			if (JC_NULL != pstNotification && JC_NULL != pstNotification->pContentLocation)
			{
				mmi_trace(1,"JDD_LOG: jMMS_ExtractContentURLFromNotification URL is [%s]", pstNotification->pContentLocation) ;
				rCode = jdi_CUtilsCharToTchar (JC_NULL, pstNotification->pContentLocation, &pContentLocationUrl) ;
			}
		}
		else
		{
			mmi_trace(1,"JDD_LOG: jMMS_ExtractContentURLFromNotification >> Header corrupted") ;
		}
	}
	else
	{
		mmi_trace(1,"JDD_LOG: jMMS_ExtractContentURLFromNotification rCode is [%x]", -rCode) ;
	}
	return pContentLocationUrl ;
}

JC_RETCODE jMMS_GenerateAckData (E_JMMS_MSG_SUBSTATE eMsgState,
	E_JMMS_QUEUE_NODE_STATE eNodeState, JC_UINT32 uiMessageID, CONTENT_DATA *pstEncapsulatedData)
{
	JC_RETCODE				rCode = JC_OK ;
	RETRIEVE_FILTERS		stStoreRetFilter = {0, } ;
	MESSAGE_LIST			*pstMsgList = JC_NULL ;
	MESSAGE_NOTIFICATION	*pstNotification = JC_NULL ;
	MESSAGE_RETRIEVAL		*pstMRetrieveConfPDU = JC_NULL ;
	NOTIFICATION_RESPONSE *pstMNotifyRespIndPDU ;
	MESSAGE_ACK			*pstMAcknowledgeIndPDU = JC_NULL ;
	MESSAGE_HEADERS		*pstNewMessageHeaders ;
	MSG_HANDLE				hResponseHandle = JC_NULL;
						
	stStoreRetFilter.uiStartIndex = 1 ;
	stStoreRetFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
	rCode = jmms_GetMsgDetailsByID (g_pstJMmsAppInfo->hMsgStore, E_JMMS_INBOX_FOLDER, uiMessageID, &pstMsgList) ;
	mmi_trace(1,"JDD_LOG: jMMS_GenerateAckData rCode [%x], eMsgState [%d], eNodeState [%d], uiMsgID [%d]",
		-rCode, eMsgState, eNodeState, uiMessageID) ;
	if (JC_OK == rCode && JC_NULL != pstMsgList && JC_NULL != pstMsgList->pMessageHeaders &&
		JC_NULL != pstMsgList->pMessageHeaders->pHeaders)
	{
		if (E_JMMS_NODE_NOTIFICATION_RESP_SEND == eNodeState && E_JMMS_MSG_DEFERRED == eMsgState)
		{
			pstNotification = (MESSAGE_NOTIFICATION *)pstMsgList->pMessageHeaders->pHeaders ;
		}
		else
		{
			/* For Auto download + Read Report + Ack Ind. */
			pstMRetrieveConfPDU = (MESSAGE_RETRIEVAL *)pstMsgList->pMessageHeaders->pHeaders ;
		}
		
		if (E_JMMS_NODE_NOTIFICATION_RESP_SEND == eNodeState)
		{
		     if((E_JMMS_MSG_RETRIEVED == eMsgState && 
			 	(pstMRetrieveConfPDU->pVersion == NULL ||
				pstMRetrieveConfPDU->pTransactionID == NULL)) 
				||
				(E_JMMS_MSG_DEFERRED == eMsgState && 
				(pstNotification->pVersion == NULL ||
				pstNotification->pTransactionID == NULL)) )
			{
					rCode = JC_ERR_GENERAL_FAILURE;
					mmi_trace(1, " trasaction id or version is NULL. ignore ");
			}
			else
		     {
				
				pstMNotifyRespIndPDU = (NOTIFICATION_RESPONSE *)jdd_MemAlloc (1, sizeof(NOTIFICATION_RESPONSE)) ;
				if ( JC_NULL == pstMNotifyRespIndPDU )
				{
					rCode = JC_ERR_MEMORY_ALLOCATION ;
				}
				if (JC_OK == rCode)
				{
					pstNewMessageHeaders = (MESSAGE_HEADERS *)jdd_MemAlloc (sizeof( MESSAGE_HEADERS), 1) ;
					if (JC_NULL == pstNewMessageHeaders)
					{
						jdd_MemFree (pstMNotifyRespIndPDU) ;
						rCode = JC_ERR_MEMORY_ALLOCATION ;
					}
				}
				if (JC_OK == rCode)
				{
					rCode = jdi_MsgInit (&hResponseHandle, JC_NULL) ;
					if (JC_OK != rCode)
					{
						jdd_MemFree(pstMNotifyRespIndPDU) ;
						jdd_MemFree(pstNewMessageHeaders) ;
					}
				}
				if (JC_OK == rCode)
				{
					rCode = jdi_MsgSetMessageHeaders (hResponseHandle, pstNewMessageHeaders) ;
					if (JC_OK != rCode)
					{
						jdd_MemFree(pstMNotifyRespIndPDU) ;
						jdd_MemFree(pstNewMessageHeaders) ;
						jdi_MsgDeInit (hResponseHandle) ;
						hResponseHandle = JC_NULL ;
					}
					else
					{
						/* Now form the header */
						pstNewMessageHeaders->eMsgType = E_MSG_TYPE_NOTIFICATION_RESPONSE ;
						pstNewMessageHeaders->pHeaders = (void *)pstMNotifyRespIndPDU ;
						if (E_JMMS_MSG_RETRIEVED == eMsgState)
						{
							/* For Auto download */
							pstMNotifyRespIndPDU->eMessageStatus = E_MSG_RETRIEVED ;					
							pstMNotifyRespIndPDU->pVersion = jdi_CUtilsStrDuplicate (JC_NULL, pstMRetrieveConfPDU->pVersion) ;
							pstMNotifyRespIndPDU->pTransactionID = jdi_CUtilsStrDuplicate (JC_NULL, pstMRetrieveConfPDU->pTransactionID) ;
							pstMNotifyRespIndPDU->bDeliveryReport = pstMRetrieveConfPDU->bDeliveryReport ;
						}
						else 
						{
							/* For Deferred */
							if (E_JMMS_MSG_DEFERRED == eMsgState)
							{
								pstMNotifyRespIndPDU->eMessageStatus = E_MSG_DEFERRED ;
								pstMNotifyRespIndPDU->pVersion = jdi_CUtilsStrDuplicate (JC_NULL, pstNotification->pVersion) ;
								pstMNotifyRespIndPDU->pTransactionID = jdi_CUtilsStrDuplicate (JC_NULL, pstNotification->pTransactionID ) ;					
								pstMNotifyRespIndPDU->bDeliveryReport = pstNotification->bDeliveryReport ;
							}
						}
					}
				}
			}
			
			
		}
		else if (E_JMMS_NODE_ACK_SEND == eNodeState)
		{
		     if(pstMRetrieveConfPDU->pVersion != NULL &&
			 	pstMRetrieveConfPDU->pTransactionID != NULL)
		     {
				pstMAcknowledgeIndPDU = (MESSAGE_ACK *) jdd_MemAlloc (1, sizeof (MESSAGE_ACK)) ;
				if ( JC_NULL == pstMAcknowledgeIndPDU )
				{
					rCode = JC_ERR_MEMORY_ALLOCATION ;
				}
				else
				{
					pstNewMessageHeaders = (MESSAGE_HEADERS *) jdd_MemAlloc (sizeof( MESSAGE_HEADERS), 1) ;
					if (JC_NULL == pstNewMessageHeaders)
					{
						jdd_MemFree (pstMAcknowledgeIndPDU) ;
						rCode = JC_ERR_MEMORY_ALLOCATION ;
					}
					else
					{
						rCode = jdi_MsgInit (&hResponseHandle, JC_NULL ) ;
						if (rCode != JC_OK)
						{
							jdd_MemFree (pstMAcknowledgeIndPDU) ;
							jdd_MemFree(pstNewMessageHeaders) ;
						}
						else
						{
							rCode = jdi_MsgSetMessageHeaders (hResponseHandle, pstNewMessageHeaders) ;
							if (JC_OK != rCode)
							{
								jdd_MemFree (pstMAcknowledgeIndPDU) ;
								jdd_MemFree (pstNewMessageHeaders) ;
								jdi_MsgDeInit (hResponseHandle) ;
								hResponseHandle = JC_NULL ;
							}
							else
							{
								/* Now form the header */
								pstNewMessageHeaders->eMsgType = E_MSG_TYPE_ACKNOWLEDGE ;
								pstNewMessageHeaders->pHeaders = (void *) pstMAcknowledgeIndPDU ;

								pstMAcknowledgeIndPDU->pVersion = jdi_CUtilsStrDuplicate (JC_NULL, pstMRetrieveConfPDU->pVersion) ;
								pstMAcknowledgeIndPDU->pTransactionID = jdi_CUtilsStrDuplicate (JC_NULL, pstMRetrieveConfPDU->pTransactionID) ;
								pstMAcknowledgeIndPDU->bDeliveryReport = pstMRetrieveConfPDU->bDeliveryReport ;
							}
						}
					}
				}
		     }
			 else
			 {
			 	rCode = JC_ERR_GENERAL_FAILURE;
				mmi_trace(1, " trasaction id or version is NULL. ignore ");
			 }
		}
		else if (E_JMMS_NODE_READREPORT_MSG_SEND == eNodeState)
		{
			JC_UINT32		uiStringLength = 0 ;
			JC_UINT32		uiTextMediaID = 0 ;
			JC_CHAR		*pmMMSText	= JC_NULL ;
			MESSAGE_SEND	*pstMessageSend = (MESSAGE_SEND *)jdd_MemAlloc (1, sizeof(MESSAGE_SEND)) ;

			mmi_trace(1,"JDD_LOG: Read Report Send as MMS");
			if (JC_NULL == pstMessageSend)
			{
				rCode = JC_ERR_MEMORY_ALLOCATION ;
			}
			else
			{
				pstNewMessageHeaders = (MESSAGE_HEADERS *)jdd_MemAlloc (sizeof (MESSAGE_HEADERS), 1) ;
				if (JC_NULL == pstNewMessageHeaders)
				{
					jdd_MemFree (pstMessageSend) ;
					rCode = JC_ERR_MEMORY_ALLOCATION ;
				}
			}
			if (JC_OK == rCode)
			{
				rCode = jdi_MsgInit (&hResponseHandle, JC_NULL) ;
				if (rCode != JC_OK)
				{
					jdd_MemFree (pstMessageSend) ;
					jdd_MemFree(pstNewMessageHeaders) ;
				}
			}
			if (JC_OK == rCode)
			{
				rCode = jdi_MsgSetMessageHeaders (hResponseHandle, pstNewMessageHeaders) ;
				if (JC_OK != rCode)
				{
					jdd_MemFree (pstMessageSend) ;
					jdd_MemFree(pstNewMessageHeaders) ;
					jdi_MsgDeInit (hResponseHandle) ;
					hResponseHandle = JC_NULL ;
				}
			}
			mmi_trace(1,"JDD_LOG: ReadReport MMS Init rCode[%x]", -rCode);
			if (JC_OK == rCode)
			{
				/* Now form the header */
				pstNewMessageHeaders->eMsgType = E_MSG_TYPE_SEND ;
				pstNewMessageHeaders->pHeaders = (void *)pstMessageSend ;

				pstMessageSend->eMsgClass = E_MSG_AUTO_CLASS ;
				pstMessageSend->bDeliveryReport = E_FALSE ;
				pstMessageSend->bReadReport = E_FALSE ;
			}
			if (JC_OK == rCode)
			{
				//subject
				{	
					uiStringLength += jc_tcslen (GetString (STR_ID_JMMS_SUBJECT)) ;
					uiStringLength += jc_strlen (JMMS_NEW_LINE) ;
					if(pstMRetrieveConfPDU->pSubject)
					{
						uiStringLength += jc_tcslen (pstMRetrieveConfPDU->pSubject) ;
					}
					else
					{
						uiStringLength += jc_tcslen (GetString (STR_GLOBAL_EMPTY_LIST)) ;
					}				
					uiStringLength += jc_strlen (JMMS_NEW_LINE) ;
					uiStringLength += jc_strlen (JMMS_DASH_LINE) ;
				}
				if (pstMRetrieveConfPDU->pFrom != JC_NULL)
				{
					uiStringLength += jc_tcslen (GetString (STR_ID_MMS_MSG_FROM)) ;
					uiStringLength += jc_strlen (JMMS_NEW_LINE) ;
					uiStringLength += jc_tcslen (pstMRetrieveConfPDU->pFrom) ;
					uiStringLength += jc_strlen (JMMS_NEW_LINE) ;
					uiStringLength += jc_strlen (JMMS_DASH_LINE) ;
				}
				if (pstMRetrieveConfPDU->pTo != JC_NULL)
				{
					uiStringLength += jc_tcslen (GetString (STR_ID_JMMS_TO)) ;
					uiStringLength += jc_strlen (JMMS_NEW_LINE) ;
					uiStringLength += jc_tcslen (pstMRetrieveConfPDU->pTo) ;
					uiStringLength += jc_strlen (JMMS_NEW_LINE) ;
					uiStringLength += jc_strlen (JMMS_DASH_LINE) ;
				}

				//date and time
				{
					MYTIME		stTime = {0, } ;
					JC_CHAR	pmcFormatString [64] = {0,} ;
					kal_bool	eRet ;
					
					eRet = mmi_dt_utc_sec_2_mytime (pstMRetrieveConfPDU->uiDate + (GetTimeZone(PhnsetGetHomeCity()) * 60 * 60), &stTime) ;
					
					date_string (&stTime, pmcFormatString, DT_IDLE_SCREEN) ;
					uiStringLength += jc_tcslen (GetString (STR_ID_MMS_MSG_DATE)) ;
					uiStringLength += jc_tcslen (pmcFormatString) ;
					uiStringLength += jc_strlen (JMMS_NEW_LINE) ;


					jc_memset (pmcFormatString, 0 , 64);
					time_string (&stTime, pmcFormatString, DT_IDLE_SCREEN) ;
					uiStringLength += jc_tcslen (GetString (STR_ID_MMS_MSG_TIME)) ;
					uiStringLength += jc_tcslen (pmcFormatString) ;
					uiStringLength += jc_strlen (JMMS_NEW_LINE) ;
					uiStringLength += jc_strlen (JMMS_DASH_LINE) ;

				}
				
				// class
				{
					uiStringLength += jc_tcslen (GetString (STR_ID_JMMS_MSG_CLASS)) ;
					uiStringLength += jc_strlen (JMMS_NEW_LINE) ;
					uiStringLength += jc_tcslen (GetString (MessageClassStr[pstMRetrieveConfPDU->eMsgClass])) ;
					uiStringLength += jc_strlen (JMMS_NEW_LINE) ;
					uiStringLength += jc_strlen (JMMS_DASH_LINE) ;
				}

#if 0				
				if (pstMRetrieveConfPDU->pMessageID != JC_NULL)
				{
					uiStringLength += jc_strlen (GetString (STR_ID_MMS_MESSAGE_ID)) ;
					uiStringLength += jc_strlen (pstMRetrieveConfPDU->pMessageID) ;
				}
#endif				
				if (uiStringLength > 0)
				{
					pmMMSText = (JC_CHAR *)jdd_MemAlloc (sizeof(JC_CHAR), uiStringLength + 1) ;
					if (JC_NULL == pmMMSText) 
					{								
						rCode = JC_ERR_MEMORY_ALLOCATION ;								
					}
					else
					{
						//subject
						{
							jc_wstrcpy (pmMMSText, (JC_CHAR*)GetString (STR_ID_JMMS_SUBJECT)) ;
							jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, pmMMSText) ;
							if(pstMRetrieveConfPDU->pSubject)
							{
								jc_tcscat (pmMMSText, pstMRetrieveConfPDU->pSubject) ;
							}
							else
							{
								jc_tcscat (pmMMSText, (JC_CHAR*)GetString (STR_GLOBAL_EMPTY_LIST)) ;
							}				
							jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, pmMMSText) ;
							jdi_CUtilsTcsStrCat (JMMS_DASH_LINE, pmMMSText) ;
						}
						if (JC_NULL != pstMRetrieveConfPDU->pFrom)
						{
							//jdi_CUtilsTcsStrCat (READ_REPORT_TO, pmMMSText) ;
							jc_tcscat (pmMMSText, (JC_CHAR*)GetString (STR_ID_MMS_MSG_FROM));
							jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, pmMMSText) ;
							jc_tcscat (pmMMSText, pstMRetrieveConfPDU->pFrom);
							jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, pmMMSText) ;
							jdi_CUtilsTcsStrCat (JMMS_DASH_LINE, pmMMSText) ;
						}
						if (JC_NULL != pstMRetrieveConfPDU->pTo)
						{
							jc_tcscat (pmMMSText, (JC_CHAR*)GetString (STR_ID_JMMS_TO));
							jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, pmMMSText) ;
							jc_tcscat (pmMMSText, pstMRetrieveConfPDU->pTo);
							jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, pmMMSText) ;
							jdi_CUtilsTcsStrCat (JMMS_DASH_LINE, pmMMSText) ;
						}
						
						//date time
						{
							MYTIME		stTime = {0, } ;
							JC_CHAR	pmcFormatString [64] = {0,} ;
							kal_bool	eRet ;
					
							eRet = mmi_dt_utc_sec_2_mytime (pstMRetrieveConfPDU->uiDate + (GetTimeZone(PhnsetGetHomeCity()) * 60 * 60), &stTime) ;
					
							date_string (&stTime, pmcFormatString, DT_IDLE_SCREEN) ;
							jc_tcscat (pmMMSText, (JC_CHAR*)GetString (STR_ID_MMS_MSG_DATE));
							jc_tcscat (pmMMSText, pmcFormatString);
							jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, pmMMSText) ;

							jc_memset (pmcFormatString, 0 , 64);
							time_string (&stTime, pmcFormatString, DT_IDLE_SCREEN) ;
							jc_tcscat (pmMMSText, (JC_CHAR*)GetString (STR_ID_MMS_MSG_TIME));
							jc_tcscat (pmMMSText, pmcFormatString);
							jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, pmMMSText) ;
							jdi_CUtilsTcsStrCat (JMMS_DASH_LINE, pmMMSText) ;
						}
						
						// class
						{
							jc_tcscat (pmMMSText, (JC_CHAR*)GetString (STR_ID_JMMS_MSG_CLASS));
							jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, pmMMSText) ;
							jc_tcscat (pmMMSText, (JC_CHAR*)GetString (MessageClassStr[pstMessageSend->eMsgClass]));
							jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, pmMMSText) ;
							jdi_CUtilsTcsStrCat (JMMS_DASH_LINE, pmMMSText) ;
						}
#if 0						
						if (JC_NULL != pstMRetrieveConfPDU->pMessageID)
						{
							//jdi_CUtilsTcsStrCat (READ_REPORT_MESSAGE_ID, pmMMSText) ;
							jc_tcscat (pmMMSText, GetString (STR_ID_MMS_MESSAGEID)) ;
							jdi_CUtilsTcsStrCat (pstMRetrieveConfPDU->pMessageID, pmMMSText) ;
						}
#endif						
						rCode = jmms_AddTextToSlide (hResponseHandle, pmMMSText, &uiTextMediaID) ;
						mmi_trace(1,"JDD_LOG: ReadReport jmms_AddTextToSlide () rCode[%x] [%d]", -rCode, uiTextMediaID) ;
						if (JC_OK == rCode && 0 != uiTextMediaID)
						{
							uiStringLength = 0 ;
							if (JC_NULL != pstMRetrieveConfPDU->pSubject)
							{
								uiStringLength += jc_tcslen (GetString(STR_ID_MMS_READ_REPORT_SUBJECT)) ;
								uiStringLength += jc_tcslen (pstMRetrieveConfPDU->pSubject) ;
							}
							else
							{
								uiStringLength += jc_tcslen (GetString(STR_ID_MMS_READ_REPORT_SUBJECT)) ;
							}
							if (uiStringLength > 0 )
							{
								pstMessageSend->pSubject = (JC_CHAR *)jdd_MemAlloc(sizeof(JC_CHAR), uiStringLength + 1) ;
								if (pstMessageSend->pSubject == JC_NULL)
								{
									rCode = JC_ERR_MEMORY_ALLOCATION ; 
								}
								else
								{
									if (JC_NULL != pstMRetrieveConfPDU->pSubject)
									{
										jc_wstrcpy (pstMessageSend->pSubject, GetString (STR_ID_MMS_READ_REPORT_SUBJECT)) ;										
										jc_tcscat (pstMessageSend->pSubject, pstMRetrieveConfPDU->pSubject) ;
									}
									else
									{
										jc_wstrcpy (pstMessageSend->pSubject, GetString (STR_ID_MMS_READ_REPORT_SUBJECT)) ;
									}	
								}
							}
							if (JC_OK == rCode)
							{
								pstMessageSend->pTo = jdi_CUtilsTcsDuplicate(JC_NULL, pstMRetrieveConfPDU->pFrom) ;
								if (JC_NULL == pstMessageSend->pTo)
								{
									rCode = JC_ERR_MEMORY_ALLOCATION ;
								}
								else
								{
									pstMessageSend->pVersion = jdi_CUtilsStrDuplicate (JC_NULL, pstMRetrieveConfPDU->pVersion);
									if (JC_NULL == pstMessageSend->pVersion)
									{
										rCode = JC_ERR_MEMORY_ALLOCATION ;
									}
									else
									{
										/* Successfully created the message, and jdi_MsgGetEncapsulatedBuffer (hResponseHandle, pstEncapsulatedData)
											will be called at the end */
									}
								}
							}
						}
						else
						{
							rCode = JC_ERR_MSG_INVALID_MEDIA_INFO ;
						}
					}
					if (JC_NULL != pmMMSText)
					{
						jdd_MemFree (pmMMSText) ;
						pmMMSText = JC_NULL ;
					}
				}
			}
			mmi_trace(1,"JDD_LOG: ReadReport MMS Headers rCode[%x]", -rCode);
		}
		else if (E_JMMS_NODE_READREPORT_PDU_SEND == eNodeState)
		{
		       READ_REPORT *pstReadReport;
			mmi_trace(1,"JDD_LOG: Read Report Send as PDU");
			pstReadReport = (READ_REPORT *)jdd_MemAlloc (1, sizeof(READ_REPORT)) ;
			if (JC_NULL == pstReadReport)
			{
				rCode = JC_ERR_MEMORY_ALLOCATION ;
			}
			else
			{
				pstNewMessageHeaders = (MESSAGE_HEADERS *)jdd_MemAlloc (sizeof (MESSAGE_HEADERS), 1) ;
				if (JC_NULL == pstNewMessageHeaders)
				{
					jdd_MemFree(pstReadReport) ;
					rCode = JC_ERR_MEMORY_ALLOCATION ;
				}
			}
			if (JC_OK == rCode)
			{
				rCode = jdi_MsgInit (&hResponseHandle, JC_NULL) ;
				if (rCode != JC_OK)
				{
					jdd_MemFree (pstReadReport) ;
					jdd_MemFree (pstNewMessageHeaders) ;
				}
				else
				{
					rCode = jdi_MsgSetMessageHeaders (hResponseHandle, pstNewMessageHeaders) ;
					if (JC_OK != rCode)
					{
						jdd_MemFree (pstReadReport) ;
						jdd_MemFree (pstNewMessageHeaders) ;
						jdi_MsgDeInit (hResponseHandle) ;
						hResponseHandle = JC_NULL ;
					}
				}
				mmi_trace(1,"JDD_LOG: ReadReport PDU init rCode[%x]", rCode);
			}
			if (JC_OK == rCode)
			{
				/* Now form the header */
				pstNewMessageHeaders->eMsgType = E_MSG_TYPE_READ_REPORT ;
				pstNewMessageHeaders->pHeaders = (void * ) pstReadReport ;

				pstReadReport->eReadStatus = E_STATUS_READ ;
				pstReadReport->pMessageID = jdi_CUtilsStrDuplicate (JC_NULL,pstMRetrieveConfPDU->pMessageID ) ;	
				if (pstReadReport->pMessageID != JC_NULL)
				{
					pstReadReport->pTo = (JC_CHAR *)jdd_MemAlloc(sizeof(JC_CHAR), jc_tcslen(pstMRetrieveConfPDU->pFrom) + 1) ;
					if (pstReadReport->pTo == JC_NULL)
					{
						rCode = JC_ERR_MEMORY_ALLOCATION ;					
					}
					else
					{
						jc_tcscpy(pstReadReport->pTo, pstMRetrieveConfPDU->pFrom) ;
						pstReadReport->pVersion = jdi_CUtilsStrDuplicate(JC_NULL, pstMRetrieveConfPDU->pVersion) ;
						if ( pstReadReport->pVersion != JC_NULL)
						{																		
							/* Successfully created the message, and jdi_MsgGetEncapsulatedBuffer (hResponseHandle, pstEncapsulatedData)
											will be called at the end */
						}
						else
						{
							rCode = JC_ERR_MEMORY_ALLOCATION ;
						}
					}
				}
				else
				{
					rCode = JC_ERR_MEMORY_ALLOCATION ;	
				}
			}
			mmi_trace(1,"JDD_LOG: ReadReport PDU Headers rCode[%x]", rCode);
		}

		if (JC_OK == rCode)
		{
			rCode = jdi_MsgGetEncapsulatedBuffer (hResponseHandle, pstEncapsulatedData) ;
			mmi_trace(1,"JDD_LOG: jMMS_GenerateAckData::jdi_MsgGetEncapsulatedBuffer rCode [%x], bStream [%d], pFilename [%d], buffer [%d], size [%d]",
				-rCode,
				pstEncapsulatedData->bIsStream, pstEncapsulatedData->pFileName,
				pstEncapsulatedData->pBuffer, pstEncapsulatedData->uiCurSize) ;
		}
		if (JC_NULL != hResponseHandle)
		{
			jdi_MsgDeInit (hResponseHandle) ;
		}
	}
	else
	{
		mmi_trace(1,"JDD_LOG: jMMS_GenerateAckData failed") ;
	}
	return rCode ;
}

JC_RETCODE jMMS_ProcessCommunicatorFetchRequest (JC_UINT32 uiMsgID,
					E_JMMS_QUEUE_NODE_STATE eNodeState, E_JMMS_MSG_SUBSTATE eMsgSubState)
{
	JC_RETCODE			rCode = JC_OK ;
	ST_REQUEST_LIST	*pstHeadRequestNode = JC_NULL ;
	JC_CHAR			*pmContentURLLocation = JC_NULL;
	CONTENT_DATA		stContentData = {0, } ;
	JC_UINT32			uiPrevSIMID = 0xFF;

	mmi_trace(12,"JDD_LOG: jMMS_ProcessCommunicatorFetchRequest (uiMsgID [%u], \
		eNodeState [%d], eMsgSubState [%d]), CommState[%d]",
		uiMsgID, eNodeState, eMsgSubState, g_pstJMmsAppInfo->eCommunicatorState) ;
	
	if (E_JMMS_NODE_IDLE != eNodeState)
	{
		if (E_JMMS_NODE_NOTIFICATION_RESP_SEND == eNodeState ||
			E_JMMS_NODE_ACK_SEND == eNodeState)
		{
			if (JC_NULL == g_pstJMmsAppInfo->hMsgStore)
			{
				rCode = jdi_StoreInit (&g_pstJMmsAppInfo->hMsgStore, 0) ;
				mmi_trace (0, "JDD_LOG: jMMS_ProcessCommunicatorFetchRequest::jdi_StoreInit[%X]", -rCode) ;
			}
		}
		
		rCode = jMMS_AddReqRespNode (eNodeState, 0, uiMsgID, eMsgSubState) ;

		if (JC_ERR_QUEUE_EXISTS == rCode)
		{
			rCode = JC_OK ;
		}
	}
	if (JC_OK == rCode)
	{
		if (JC_NULL != g_pstJMmsAppInfo->pReqRespList)
		{	
			if (E_TRUE == g_pstJMmsAppInfo->pReqRespList->bIsToRemove)
			{
				uiPrevSIMID = g_pstJMmsAppInfo->pReqRespList->uiSimID ;
				jMMS_RemoveReqRespNode (g_pstJMmsAppInfo->pReqRespList->uiReferenceID) ;
			}
		}
		pstHeadRequestNode = g_pstJMmsAppInfo->pReqRespList ;

		if (JC_NULL == pstHeadRequestNode)
		{
			rCode = jdi_CommunicationDisconnect (g_pstJMmsAppInfo->hCommunicator) ;
			mmi_trace(12,"JDD_LOG: jMMS_ProcessCommunicatorFetchRequest >> jdi_CommunicationDisconnect () returned [%d]", rCode) ;
			if(g_pstJMmsAppInfo->pcMMSCUrl)
			{
				jdd_MemFree (g_pstJMmsAppInfo->pcMMSCUrl) ;
				g_pstJMmsAppInfo->pcMMSCUrl = JC_NULL;
			}
			//if MMS application is not active
			if (E_FALSE == jMMS_GetMMSAppStatus ())
			{
				// de-intilase the store
				if (g_pstJMmsAppInfo->hMsgStore)
				{
					rCode = jdi_StoreDeinit (g_pstJMmsAppInfo->hMsgStore) ;
					g_pstJMmsAppInfo->hMsgStore = JC_NULL ;
					mmi_trace (0, "JDD_LOG: jMMS_ProcessCommunicatorFetchRequest::jdi_StoreDeinit:[%X]", -rCode) ;
				}
			}
		}

	// this condition holds true only for multi SIM
#ifdef __MMI_MULTI_SIM__
		else if (uiPrevSIMID != pstHeadRequestNode->uiSimID && \
				E_FALSE == pstHeadRequestNode->bIsUnderProgress)
		{

			if (uiPrevSIMID != 0xFF)
			{
				/*	This is the case where, the previous node with different SIM was deleted
					and the SIM for the current node is different */
				mmi_trace(12,"JDD_LOG: disconnecting previous dat connection  on SIM[%d]", uiPrevSIMID) ;
				rCode = jdi_CommunicationDisconnect (g_pstJMmsAppInfo->hCommunicator) ;
				/*	The fetch request should not be processed as the dataconnection is
					getting disconnected */
				pstHeadRequestNode = JC_NULL ;
			}
			else if (E_JMMS_COMM_IDLE == g_pstJMmsAppInfo->eCommunicatorState ||
						E_JMMS_COMM_DISCONNECTED == g_pstJMmsAppInfo->eCommunicatorState)
			{
				mmi_trace(12,"JDD_LOG: setting profile to SIM[%d]",pstHeadRequestNode->uiSimID ) ;
				g_pstJMmsAppInfo->send_simid = pstHeadRequestNode->uiSimID ;
				
				
				jMMS_SettingreadUpdateDataConnInfo () ;
				
			}
			else
			{
				/* The dataconnection is getting disconnected. Wait till it completes */
			}
		}
#else
				jMMS_SettingreadUpdateDataConnInfo () ;
#endif //__MMI_MULTI_SIM__
	}
	if (JC_NULL != pstHeadRequestNode)
	{
		mmi_trace(12,"JDD_LOG: jMMS_ProcessCommunicatorFetchRequest >> Head Node values are [%d], [%d], [%d], [%d], [%d], [%d]",
			pstHeadRequestNode->bIsUnderProgress,
			pstHeadRequestNode->eStatus,
			pstHeadRequestNode->eMsgSubState,
			pstHeadRequestNode->eNodeState,
			pstHeadRequestNode->uiMsgID,
			pstHeadRequestNode->uiReferenceID) ;
		if (E_FALSE == pstHeadRequestNode->bIsUnderProgress && g_pstJMmsAppInfo->pcMMSCUrl != NULL)
		{
			/* Start processing the head node */
			if (E_JMMS_NODE_RETRIEVE == pstHeadRequestNode->eNodeState ||
				E_JMMS_NODE_AUTO_RETRIEVE == pstHeadRequestNode->eNodeState)
			{
				/* Extract the content location url and start fetching it */
				pmContentURLLocation = jMMS_ExtractContentURLFromNotification (pstHeadRequestNode->uiMsgID) ;
				if (JC_NULL != pmContentURLLocation)
				{
					/* Fetch the url and update the pstHeadRequestNode->uiReferenceID */
					pstHeadRequestNode->uiReferenceID = jMMS_FetchData (pmContentURLLocation, JC_NULL) ;
					mmi_trace(12, "JDD_LOG: after call jMMS_FetchData for retrieve, uiReferenceID is: %d", pstHeadRequestNode->uiReferenceID);
					if (0 != pstHeadRequestNode->uiReferenceID)
					{
						pstHeadRequestNode->bIsUnderProgress = E_TRUE ;
						if (E_JMMS_NODE_RETRIEVE == pstHeadRequestNode->eNodeState)
						{
							show_progress_screen (E_JMMS_NODE_RETRIEVE, 0) ;
						}
					}
					else
					{
						rCode = JC_ERR_UNKNOWN ;
						mmi_trace(12,"JDD_LOG: jMMS_ProcessCommunicatorFetchRequest >> jMMS_FetchData () Failed") ;
					}
					jdd_MemFree (pmContentURLLocation) ;
				}
				else
				{
					rCode = JC_ERR_NULL_POINTER;
					mmi_trace(1,"JDD_LOG: notification corrupted") ;		
				}
			}
			else if (E_JMMS_NODE_SEND == pstHeadRequestNode->eNodeState)
			{
				/* Get the encapsulated data from the store and start sending it and update the pstHeadRequestNode->uiReferenceID.
					pstHeadRequestNode->eMsgSubState will indicate which folder to set active */
				rCode = jMMS_GetMsgFromStoreByMsgID (g_pstJMmsAppInfo->hMsgStore, E_JMMS_OUTBOX_FOLDER,
								pstHeadRequestNode->uiMsgID, &stContentData) ;
				if (JC_OK == rCode)
				{
					jdi_CUtilsCharToTchar (JC_NULL, g_pstJMmsAppInfo->pcMMSCUrl, &pmContentURLLocation) ;
					pstHeadRequestNode->uiReferenceID = jMMS_FetchData (pmContentURLLocation, &stContentData) ;
					mmi_trace(12, "JDD_LOG: after call jMMS_FetchData for send, uiReferenceID is: %d", pstHeadRequestNode->uiReferenceID);
					if (0 != pstHeadRequestNode->uiReferenceID)
					{
						pstHeadRequestNode->bIsUnderProgress = E_TRUE ;
						show_progress_screen (E_JMMS_NODE_SEND, 0) ;
					}
					else
					{
						rCode = JC_ERR_UNKNOWN ;
						mmi_trace(12,"JDD_LOG: jMMS_ProcessCommunicatorFetchRequest >> jMMS_FetchData () Failed") ;
					}
					jdd_MemFree (pmContentURLLocation) ;
					jmms_FreeContentData (&stContentData, rCode) ;
				}
			}
			else if (E_JMMS_NODE_NOTIFICATION_RESP_SEND == pstHeadRequestNode->eNodeState ||
				E_JMMS_NODE_ACK_SEND == pstHeadRequestNode->eNodeState ||
				E_JMMS_NODE_READREPORT_PDU_SEND == pstHeadRequestNode->eNodeState ||
				E_JMMS_NODE_READREPORT_MSG_SEND == pstHeadRequestNode->eNodeState)
			{
				/* Generate the acknowledgment data and start sending it and update the uiRequestID */
				rCode = jMMS_GenerateAckData(pstHeadRequestNode->eMsgSubState,
							pstHeadRequestNode->eNodeState,
							pstHeadRequestNode->uiMsgID, &stContentData) ;
				mmi_trace(1,"JDD_LOG: jMMS_GenerateAckData rCode[%x]", -rCode );
				if (JC_OK == rCode)
				{
					jdi_CUtilsCharToTchar (JC_NULL, g_pstJMmsAppInfo->pcMMSCUrl, &pmContentURLLocation) ;
					pstHeadRequestNode->uiReferenceID = jMMS_FetchData (pmContentURLLocation, &stContentData) ;
					mmi_trace(12, "JDD_LOG: after call jMMS_FetchData for response, uiReferenceID is: %d", pstHeadRequestNode->uiReferenceID);
					if (0 != pstHeadRequestNode->uiReferenceID)
					{
						pstHeadRequestNode->bIsUnderProgress = E_TRUE ;
					}
					else
					{
						rCode = JC_ERR_UNKNOWN ;
						mmi_trace(12,"JDD_LOG: jMMS_ProcessCommunicatorFetchRequest >> jMMS_FetchData () Failed") ;
					}
					jdd_MemFree (pmContentURLLocation) ;
					jmms_FreeContentData (&stContentData, rCode) ;
				}
			}
			if (0 == pstHeadRequestNode->uiReferenceID)
			{
				ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA	stEventData = {0, } ;
				/* Incase of any error, we need to process for the next node. Just sending the application evetn
					E_JMMS_EVENT_MSG_SEND_SUCCESS, to remove the head node and process the next node */
				pstHeadRequestNode->bIsToRemove = E_TRUE ;
				stEventData.uiMsgID = pstHeadRequestNode->uiMsgID ;
				jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_SEND_SUCCESS, &stEventData) ;
			}
		}
		else
		{
			mmi_trace(12,"JDD_LOG: jMMS_ProcessCommunicatorFetchRequest >> Head node is under progress") ;
		}
	}
	return rCode ;
}
JC_UINT32 jMMS_FetchData (JC_CHAR *psURL, CONTENT_DATA *pEncBuffer)
{
	JC_RETCODE			eRet			= JC_OK ;
	ST_COMM_FETCH_URL	fetchUrlInfo	= {0, } ;
	JC_UINT32			uiReferenceID	= 1 ;
	
	fetchUrlInfo.pcContentType = JC_NULL ;
	fetchUrlInfo.pmURL = psURL ;
	fetchUrlInfo.pstHeadersInfo = JC_NULL ;
	if (JC_NULL != pEncBuffer)
	{
		fetchUrlInfo.pcContentType = CS_MMS_CONTENT_TYPE ;
		fetchUrlInfo.eCommFetchType = E_COMM_FETCH_POST ;
		if (E_FALSE == pEncBuffer->bIsStream)
		{
			fetchUrlInfo.bIsFile = E_FALSE ;
			fetchUrlInfo.uUserDataMode.pucUserData = pEncBuffer->pBuffer ;
		}
		else
		{
			fetchUrlInfo.bIsFile = E_TRUE ;
			fetchUrlInfo.uUserDataMode.pmFileName = pEncBuffer->pFileName ;
		}
		fetchUrlInfo.uiUserDataLength = pEncBuffer->uiTotalSize ;
	}
	else
	{
		fetchUrlInfo.eCommFetchType = E_COMM_FETCH_GET ;
	}
 	eRet = jdi_CommunicationFetchURL (g_pstJMmsAppInfo->hCommunicator, &fetchUrlInfo, &uiReferenceID) ;
	if (JC_OK != eRet)
	{
		mmi_trace(12, "JDD_LOG: jdi_CommunicationFetchURL %d failed " , eRet) ;
	}
	else
	{
		mmi_trace(12, "JDD_LOG: jdi_CommunicationFetchURL called with request ID [%d]", uiReferenceID) ;
	}
	return uiReferenceID ;
}

/**
 * @brief Adds the node for processing.
 * @param[in] PDUType Specifies the PDU Type.
 * @param[in] uiRefID Specifies the request ID.
 * @param[in] uiMsgID Specifies the unique message ID.
 * @return Returns the status code, JC_OK specifies success otherwise specific error code.
 *
 */
JC_RETCODE jMMS_AddReqRespNode (E_JMMS_QUEUE_NODE_STATE eNodeState, JC_UINT32 uiRefID,
			JC_UINT32 uiMsgID, E_JMMS_MSG_SUBSTATE eMsgSubState)
{
	JC_RETCODE			rCode = JC_OK ;
	ST_REQUEST_LIST	*pNode =JC_NULL ;
	ST_REQUEST_LIST	*pTempNode = g_pstJMmsAppInfo->pReqRespList ;
	E_JMMS_FOLDER_ID	eFolderID ;
	while (JC_NULL != pTempNode && JC_OK == rCode)
	{
		mmi_trace(1,"JDD_LOG: jMMS_AddReqRespNode >> Node with uiRefID [%d], bIsUnderProgress [%d], bIsToRemove [%d], uiMsgID [%d], eNodeState [%d], eMsgSubState [%d])",
				pTempNode->uiReferenceID, pTempNode->bIsUnderProgress,
				pTempNode->bIsToRemove, pTempNode->uiMsgID,
				pTempNode->eNodeState, pTempNode->eMsgSubState) ;
		if (uiMsgID == pTempNode->uiMsgID && eNodeState == pTempNode->eNodeState &&
			eMsgSubState == pTempNode->eMsgSubState)
		{
			mmi_trace (1, "JDD_LOG: jMMS_AddReqRespNode MsgID [%d] with state [%d] already exists",
				uiMsgID, pTempNode->eNodeState) ;
			rCode = JC_ERR_QUEUE_EXISTS ;
 		}
		pTempNode = pTempNode->pNext ;
	}
	if (JC_OK == rCode)
	{
		pNode = (ST_REQUEST_LIST *)jdd_MemAlloc(1, sizeof(ST_REQUEST_LIST)) ;
		if (JC_NULL == pNode)
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
		}
		else
		{
			pNode->eNodeState = eNodeState ;
			pNode->uiReferenceID = uiRefID ;
			pNode->uiMsgID = uiMsgID ;
			pNode->eMsgSubState = eMsgSubState ;
			pNode->rCode = JC_OK ;
			switch (eNodeState)
			{
				case E_JMMS_NODE_SEND:
					eFolderID = E_JMMS_OUTBOX_FOLDER ;
					break ;
				case E_JMMS_NODE_RETRIEVE:
				case E_JMMS_NODE_AUTO_RETRIEVE:
					eFolderID = E_JMMS_INBOX_FOLDER ;
					break ;
				case E_JMMS_NODE_NOTIFICATION_RESP_SEND:
				case E_JMMS_NODE_ACK_SEND:
				case E_JMMS_NODE_READREPORT_PDU_SEND:
				case E_JMMS_NODE_READREPORT_MSG_SEND:
					eFolderID = E_JMMS_INBOX_FOLDER ;
					break ;
				default:
					eFolderID = E_JMMS_INBOX_FOLDER ;
					mmi_trace (1, "JDD_LOG: jMMS_AddReqRespNode () >> Invalid Folder ID") ;
			}
		
			jmms_GetMsgExtraBytes (g_pstJMmsAppInfo->hMsgStore, eFolderID, uiMsgID, &pNode->uiSimID) ;
			if (JC_NULL == g_pstJMmsAppInfo->pReqRespList)
			{
				g_pstJMmsAppInfo->pReqRespList = pNode ;
			}
			else
			{
				pTempNode = g_pstJMmsAppInfo->pReqRespList ;
				while (pTempNode->pNext !=JC_NULL)
				{
					pTempNode = pTempNode->pNext ;
				}
				pTempNode->pNext = pNode ;
			}
			mmi_trace(1,"JDD_LOG: jMMS_AddReqRespNode (eNodeState [%u], uiRefID [%d], uiMsgID [%d], eMsgSubState [%d])",
				eNodeState, uiRefID, uiMsgID, eMsgSubState) ;
		}
	}
	return rCode ;
}
/**
 * @brief judge if there any node under processing.
 */
JC_BOOLEAN jMMS_IsReqNodeUnderProgress(void)
{
	ST_REQUEST_LIST	*pstHeadRequestNode = JC_NULL ;
	pstHeadRequestNode = g_pstJMmsAppInfo->pReqRespList ;
	if ((JC_NULL != pstHeadRequestNode) && (E_TRUE == pstHeadRequestNode->bIsUnderProgress))
		return E_TRUE;
	return E_FALSE;
}

/**
 * @brief Removes the node after processing.
 * @param[in] uiRefID Specifies the request ID.
 * @return void.
 *
 */
void jMMS_RemoveReqRespNode (JC_UINT32 uiRefID)
{
	ST_REQUEST_LIST *pTempNode = JC_NULL ;
	ST_REQUEST_LIST *pPrevNode = JC_NULL ;

	pTempNode = g_pstJMmsAppInfo->pReqRespList ;
	while (JC_NULL != pTempNode)
	{
		if (pTempNode->uiReferenceID == uiRefID)
		{
			if (JC_NULL == pPrevNode)
			{
				g_pstJMmsAppInfo->pReqRespList = g_pstJMmsAppInfo->pReqRespList->pNext ;
			}
			else
			{
				pPrevNode->pNext = pTempNode->pNext ;
			}
			pTempNode->pNext = JC_NULL ;
			if (pTempNode->pFileName)
			{
	                     if(jdd_FSIsFileExist(JC_NULL, pTempNode->pFileName))
					jdd_FSDelete (JC_NULL, pTempNode->pFileName) ;
			}
			jdd_MemFree(pTempNode->pFileName);
			jdd_MemFree (pTempNode->pszContentType) ;
			jdd_MemFree (pTempNode) ;
			mmi_trace(12,"JDD_LOG: removed node with Request ID [%d]", uiRefID) ;
			break ;
		}
		pPrevNode = pTempNode ;
		pTempNode = pTempNode->pNext ;
	}
}

/**
 * @brief Removes all the node asscoaited with SIM1.
 * @param[in] uiMsgID Specifies the message ID.
 * @return void.
 *
 */
void jMMS_RemAllReqRespNodeBySIM(UINT8 nSimID)
{
	ST_REQUEST_LIST *pTempNode = JC_NULL ;
	ST_REQUEST_LIST *pPrevNode = JC_NULL ;
	
	if(g_pstJMmsAppInfo->pReqRespList == JC_NULL)
		return;

	pTempNode = g_pstJMmsAppInfo->pReqRespList->pNext ;
	while (JC_NULL != pTempNode)
	{
		if (pTempNode->uiSimID == nSimID)
		{
			if (JC_NULL == pPrevNode)
			{
				g_pstJMmsAppInfo->pReqRespList = g_pstJMmsAppInfo->pReqRespList->pNext ;
			}
			else
			{
				pPrevNode->pNext = pTempNode->pNext ;
			}
			pTempNode->pNext = JC_NULL ;
			if (pTempNode->pFileName)
			{
				jdd_FSDelete (JC_NULL, pTempNode->pFileName) ;
			}
			jdd_MemFree(pTempNode->pFileName);
			jdd_MemFree (pTempNode->pszContentType) ;
			jdd_MemFree (pTempNode) ;
			mmi_trace(1,"JDD_LOG: Removed Node with Msg ID [%d]", nSimID) ;
			//break ;
		}
		pPrevNode = pTempNode ;
		pTempNode = pTempNode->pNext ;
	}
}

/**
 * @brief Removes the node after processing.
 * @param[in] uiMsgID Specifies the message ID.
 * @return void.
 *
 */
void jMMS_RemoveReqRespNodeByMsgID (JC_UINT32 uiMsgID)
{
	ST_REQUEST_LIST *pTempNode = JC_NULL ;
	ST_REQUEST_LIST *pPrevNode = JC_NULL ;

	pTempNode = g_pstJMmsAppInfo->pReqRespList ;
	while (JC_NULL != pTempNode)
	{
		if (pTempNode->uiMsgID == uiMsgID)
		{
			if (JC_NULL == pPrevNode)
			{
				g_pstJMmsAppInfo->pReqRespList = g_pstJMmsAppInfo->pReqRespList->pNext ;
			}
			else
			{
				pPrevNode->pNext = pTempNode->pNext ;
			}
			pTempNode->pNext = JC_NULL ;
			if (pTempNode->pFileName)
			{
				//if(jdd_FSIsFileExist(JC_NULL, pTempNode->pFileName))
					jdd_FSDelete (JC_NULL, pTempNode->pFileName) ;
				//else
				//	mmi_trace(0,"file is not exist");
			}
			jdd_MemFree(pTempNode->pFileName);
			jdd_MemFree (pTempNode->pszContentType) ;
			jdd_MemFree (pTempNode) ;
			mmi_trace(1,"JDD_LOG: Removed Node with Msg ID [%d]", uiMsgID) ;
			break ;
		}
		pPrevNode = pTempNode ;
		pTempNode = pTempNode->pNext ;
	}
}



/**
 * @brief Gets the node based on the message state.
 * @param[in] eNodeState Specifies the message state.
 * @return Returns the node matching the message state.
 *
 */
ST_REQUEST_LIST *jMMS_GetReqRespNodeByState (E_JMMS_QUEUE_NODE_STATE eNodeState)
{
	ST_REQUEST_LIST *pNode = JC_NULL ;
	ST_REQUEST_LIST *pReqRespList = g_pstJMmsAppInfo->pReqRespList ;
	while (JC_NULL != pReqRespList)
	{
		if (pReqRespList->eNodeState == eNodeState)
		{
			pNode = pReqRespList ;
			break ;
		}
		pReqRespList = pReqRespList->pNext ;
	}
	return pNode ;
}

/**
 * @brief Gets the node based on the message ID.
 * @param[in] eNodeState Specifies the message ID.
 * @return Returns the node matching the message ID.
 *
 */
ST_REQUEST_LIST *jMMS_GetReqRespNodeByMsgID (JC_UINT32 uiMsgID)
{
	ST_REQUEST_LIST *pNode = JC_NULL ;
	ST_REQUEST_LIST *pReqRespList = g_pstJMmsAppInfo->pReqRespList ;
	while (JC_NULL != pReqRespList)
	{
		if (pReqRespList->uiMsgID == uiMsgID)
		{
			pNode = pReqRespList ;
			break ;
		}
		pReqRespList = pReqRespList->pNext ;
	}
	return pNode ;
}

JC_BOOLEAN jMMS_IsMsgIdInGetReqRespNode(JC_UINT32 uiMsgID)
{
	JC_BOOLEAN bIsFound = E_FALSE;
	ST_REQUEST_LIST *pReqRespList = g_pstJMmsAppInfo->pReqRespList ;
	while (JC_NULL != pReqRespList)
	{
		if (pReqRespList->uiMsgID == uiMsgID)
		{
			bIsFound = E_TRUE;
			break ;
		}
		pReqRespList = pReqRespList->pNext ;
	}
	return bIsFound ;
}

JC_BOOLEAN jMMS_FindMsgInGetReqRespNode (MESSAGE_LIST		*pMessageList)
{
    MESSAGE_LIST	*pTempMsgList = pMessageList ;
	JC_BOOLEAN    bIsFound = E_FALSE;

	while (pTempMsgList)
    { 
        if (jMMS_IsMsgIdInGetReqRespNode(pTempMsgList->uiMessageID) == E_TRUE)
        {
			 bIsFound = E_TRUE;
            break;
        }
        pTempMsgList = pTempMsgList->pNext ;
    }
    return bIsFound;
}



/**
 * @brief Gets the node based on the request ID.
 * @param[in] uiRefID Specifies the request ID.
 * @return Returns the node matching the request ID.
 *
 */
ST_REQUEST_LIST* jMMS_GetReqRespNodeByReqID (JC_UINT32 uiRefID)
{
	ST_REQUEST_LIST *pNode = JC_NULL ;
	ST_REQUEST_LIST *pReqRespList = g_pstJMmsAppInfo->pReqRespList ;
	while (JC_NULL != pReqRespList)
	{
		if (pReqRespList->uiReferenceID == uiRefID)
		{
			pNode = pReqRespList ;
			break ;
		}
		pReqRespList = pReqRespList->pNext ;
	}
	return pNode ;
}

/*
 * @ingroup  MMSAppCommunicator
 * @brief    Prepares the message slide with the given read status text 
 *           into the read report message.
 * @param    [in] hMsg -  Handle to the read report message.
 * @param    [in] pmTextBuffer - Message read status string which needs to be inserted into the slide.
 * @param    [out] puiMediaID  - Message read status text media id.
 * @return   JC_OK on success
 */
JC_RETCODE jmms_AddTextToSlide(MSG_HANDLE hMsg, JC_CHAR *pmTextBuffer, 
										   JC_UINT32 *puiMediaID)
{
    JC_RETCODE	rStatus = JC_ERR_INVALID_PARAMETER;
	MEDIA_INFO  stMediaInfo;

	if (jc_tcslen(pmTextBuffer) > 0)
	{
		rStatus = jdi_MsgAddPage (hMsg, 1) ;
		if (rStatus == JC_OK)
		{
			jc_memset(&stMediaInfo, 0x00, sizeof(MEDIA_INFO));
			stMediaInfo.eMediaType = E_MEDIA_TEXT;        
			stMediaInfo.pMimeInfo = (MEDIA_MIME_INFO*)jdd_MemAlloc(1, sizeof(MEDIA_MIME_INFO));
			if (stMediaInfo.pMimeInfo  != JC_NULL)
			{
				stMediaInfo.pMimeInfo->ContentData.bIsStream 	= E_FALSE;
				stMediaInfo.pMimeInfo->MimeHeaders.pContentType = jdi_CUtilsStrDuplicate(JC_NULL, "text/plain");
				stMediaInfo.pMimeInfo->ContentData.uiCurSize 	=
				stMediaInfo.pMimeInfo->ContentData.uiTotalSize	= jc_tcslen(pmTextBuffer) * sizeof(JC_CHAR);

				stMediaInfo.pMimeInfo->ContentData.pBuffer = (JC_UINT8*)jdd_MemAlloc(sizeof(JC_CHAR), jc_tcslen(pmTextBuffer) + 1);
				if (stMediaInfo.pMimeInfo->ContentData.pBuffer != JC_NULL)
				{
					jc_tcscpy((JC_CHAR*)stMediaInfo.pMimeInfo->ContentData.pBuffer, pmTextBuffer);
					//stMediaInfo.PresentationInfo.pRegionName = jdi_CUtilsTcsDuplicate(JC_NULL, (const JC_CHAR*) L"Text");
					jdi_CUtilsCharToTchar(JC_NULL,"Text", &stMediaInfo.PresentationInfo.pRegionName);
					if (JC_NULL != stMediaInfo.PresentationInfo.pRegionName )
					{
						rStatus = jdi_MsgAddMedia (hMsg, &stMediaInfo, 1, puiMediaID) ;
						if (rStatus != JC_OK)
						{
							jdd_MemFree(stMediaInfo.pMimeInfo->MimeHeaders.pContentType);
							jdd_MemFree(stMediaInfo.pMimeInfo->ContentData.pBuffer);
							jdd_MemFree(stMediaInfo.pMimeInfo);
						}
					}
					else
					{
						rStatus = JC_ERR_MEMORY_ALLOCATION;
						jdd_MemFree(stMediaInfo.pMimeInfo->MimeHeaders.pContentType);
						jdd_MemFree(stMediaInfo.pMimeInfo->ContentData.pBuffer);
						jdd_MemFree(stMediaInfo.pMimeInfo);
					}
				}
				else
				{
					rStatus = JC_ERR_MEMORY_ALLOCATION;
					jdd_MemFree(stMediaInfo.pMimeInfo->MimeHeaders.pContentType);
					jdd_MemFree(stMediaInfo.pMimeInfo);
				}
			}
			else
			{
				rStatus = JC_ERR_MEMORY_ALLOCATION;
			}
		}
	}

	return rStatus;
}

/*
 * @ingroup  jMMS_SendReadReport
 * @brief		API which checks whether to send the read report as MMS or PDU
 * @param[in]	pcVersion - Version info
 * @return   E_JMMS_NODE_READREPORT_PDU_SEND or E_JMMS_NODE_READREPORT_MSG_SEND based on the version.
 */
E_JMMS_QUEUE_NODE_STATE jMMS_SendReadReport (const JC_INT8 *pcVersion)
{
	if (0 == jc_strcmp (pcVersion, "1.0"))
	{
		mmi_trace(1,"JDD_LOG: jMMS_SendReadReport as MSG");
		return E_JMMS_NODE_READREPORT_MSG_SEND ;
	}
	else
	{
		mmi_trace(1,"JDD_LOG: jMMS_SendReadReport as PDU");
		return E_JMMS_NODE_READREPORT_PDU_SEND ;
	}
}

JC_RETCODE jMMS_CommCB (void *pvAppArg, E_COMM_INDICATION eCommIndicationType, 
									void *pvIndication)
{
	JC_RETCODE rCode = JC_OK ;
	mmi_trace (0, "JDD_LOG: COMM CB. EventType: %d", eCommIndicationType) ;
	switch (eCommIndicationType)
	{
		case E_PROGRESS_INDICATION: /**< Specifies to the application about the content received from the lower layers (Argument #ST_COMM_PROGRESS_IND) */
		{
			if (JC_NULL != pvIndication)
			{
				rCode = jMMS_HandleProgressIndication ((ST_COMM_PROGRESS_IND *)pvIndication) ;
			}
		}
		break;
		case E_STOP_INDICATION: /**< Specifies the response of a stop request that was previously sent from the application (Argument #ST_COMM_STOP_IND)  */
		case E_STOP_ALL_INDICATION: /**< Specifies the response of stop all request that was previously sent from the application (Argument #ST_COMM_STOP_IND)  */
		{
			if (JC_NULL != pvIndication)
			{
				rCode = jMMS_HandleStopIndication ((ST_COMM_STOP_IND *)pvIndication) ;
			}
		}
		break ;
		case E_PROFILE_CONFIRMATION_INDICATION: /**< Specifies the profile confirmation required for the lower layer to proceed for fetching operations (Argument #ST_COMM_PROFILE_CONFIRMATION_IND) */
		case E_USER_DATA_INDICATION: /**< Specifies that the lower layer requires uses data (post data info) from application to proceed (Argument #ST_USER_DATA_IND)  */
				break ;
		case E_HEADER_INDICATION: /**< Specifies the header information for a fetch request (Argument #ST_COMM_HEADER_IND). */
			{
				rCode = jMMS_HandleHeaderIndication ((ST_COMM_HEADER_IND *)pvIndication) ;
			}
			break ;
			
		case E_FETCH_INDICATION: /**< Specifies the fetch indication for a fetch request (Argument #ST_COMM_FETCH_IND). */
			{
				if (JC_NULL != pvIndication)
				{
					rCode = jMMS_HandleFetchIndication ((ST_COMM_FETCH_IND *)pvIndication) ;
				}
			}
			break ;
			
		case E_ERROR_INDICATION: /**< Specifies to application the error (if any) for a fetch request (Argument #ST_COMM_ERR_IND). */
			{
				if (JC_NULL != pvIndication)
				{
					rCode = jMMS_HandleErrorIndication ((ST_COMM_ERR_IND *)pvIndication) ;
				}
			}
			break ;
			
		case E_PUSH_INDICATION: /**< Specifies the push data that is received by the lower layer (Argument #ST_COMM_PUSH_IND). */
			{				
				rCode = jMMS_HandlePushMsg (pvAppArg, (ST_COMM_PUSH_IND *)pvIndication) ;
                break ;
			}
		case E_SECURITY_INDICATION: /**< Specifies details of the security that is negotiated by the lower layer (Argument #ST_COMM_SECURITY_IND). */
		case E_SECURITY_FAILURE_INDICATION: /**< Specifies details of security failure (if any) (Argument #ST_COMM_SECURITY_FAILURE_IND) . */
		case E_CERT_RESP_INDICATION: /**< Specifies the response of the certificate request which was previously sent by the application (Arguement #ST_COMM_CERT_RESP_IND) */
		case E_COMM_MAX_TRANSPORT_REQUESTS: /**< Specifies the maximum number of requests that can be handled in transport at a time. (Argument passed #JC_INT32*) */
		default:
		break ;
	}	
	return rCode ;
}

JC_RETCODE jMMS_HandleHeaderIndication (ST_COMM_HEADER_IND *pstHeaderIndication)
{
	JC_RETCODE rCode = JC_OK ;
	ST_REQUEST_LIST 	*pReqRespNode = JC_NULL ;
	ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA	stEventData = {0, } ;
	
	pReqRespNode = jMMS_GetReqRespNodeByReqID (pstHeaderIndication->uiRequestID) ;
	if (pReqRespNode && E_HTTP_STATUS_UNKNOWN == pReqRespNode->eStatus)
	{
		mmi_trace(0 , "JDD_LOG: jMMS_HandleHeaderIndication >> Node State [%d], Msg State [%d] and HTTP Status [%d] content type is [%s]",
			pReqRespNode->eNodeState, pReqRespNode->eMsgSubState,
			pstHeaderIndication->eHttpStatus, pstHeaderIndication->pcContentType) ;
		pReqRespNode->eStatus = pstHeaderIndication->eHttpStatus ;
		pReqRespNode->pszContentType = jdi_CUtilsStrDuplicate (JC_NULL, pstHeaderIndication->pcContentType) ;
		if (E_HTTP_OK == pReqRespNode->eStatus || E_HTTP_NO_CONTENT == pReqRespNode->eStatus)
		{
			pReqRespNode->uiContentLength = pstHeaderIndication->uiContentLength ;
			if ((E_JMMS_NODE_AUTO_RETRIEVE == pReqRespNode->eNodeState ||
				E_JMMS_NODE_RETRIEVE == pReqRespNode->eNodeState))
			{
				JC_UINT32	uiMMSFreeSpace = jMMS_GetMMSFreeSpace (TRUE) ;
				mmi_trace (0, "JDD_LOG: jMMS_HandleHeaderIndication >> MMS Required Space [%d] and Available Space [%d]",
							2 * pstHeaderIndication->uiContentLength, uiMMSFreeSpace) ;
				/*	The available space should be twice the content length as it is stored in temporary
					file first and then it is given to the store module */
				//if (uiRet < (CS_FS_THRESHOLD_LIMIT + 2 * pstHeaderIndication->uiContentLength))
				
 				/*	because CS_FS_THRESHOLD_LIMIT = 200k, temporary file can use this space,so not need twice the content length*/
				if (uiMMSFreeSpace < 2 * pstHeaderIndication->uiContentLength)
				{
					rCode = JC_ERR_FILE_WRITE ;

					mmi_trace(12,"JDD_LOG jMMS_HandleHeaderIndication >> Space not enough to download") ;
					jdi_CommunicationFreeReference (g_pstJMmsAppInfo->hCommunicator, pstHeaderIndication->uiRequestID) ;
					pReqRespNode = jMMS_GetReqRespNodeByReqID (pstHeaderIndication->uiRequestID) ;
					if (pReqRespNode)
					{
						if (E_JMMS_NODE_RETRIEVE == pReqRespNode->eNodeState)
						{
							jMMS_ErrorHandler (rCode, NULL) ;
							jmms_CallbackGoBackHistory();
						}
						pReqRespNode->bIsToRemove = E_TRUE ;
						stEventData.uiMsgID = pReqRespNode->uiMsgID ;
						if (E_JMMS_NODE_RETRIEVE == pReqRespNode->eNodeState)
						{
							jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_RETRIEVE_FAIL, &stEventData) ;
						}
						else
						{
							jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_AUTO_RETRIEVE_FAIL, &stEventData) ;
							jMMS_NewMMSAutoRetiveFailPopup(rCode);
						}
					}
				}
			}
		}
		else
		{
			if ( 0 != pstHeaderIndication->uiRequestID )
			{
				jdi_CommunicationFreeReference (g_pstJMmsAppInfo->hCommunicator, pstHeaderIndication->uiRequestID) ;
			}
			pReqRespNode->bIsToRemove = E_TRUE ;
			stEventData.uiMsgID = pReqRespNode->uiMsgID ;
			if (E_JMMS_NODE_SEND == pReqRespNode->eNodeState ||
					E_JMMS_NODE_NOTIFICATION_RESP_SEND == pReqRespNode->eNodeState ||
					E_JMMS_NODE_ACK_SEND == pReqRespNode->eNodeState ||
					E_JMMS_NODE_READREPORT_PDU_SEND == pReqRespNode->eNodeState ||
					E_JMMS_NODE_READREPORT_MSG_SEND == pReqRespNode->eNodeState)
			{
				jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_SEND_FAIL, &stEventData) ;
				if (E_JMMS_NODE_SEND == pReqRespNode->eNodeState)
				{
					DisplayPopup(GetString (STR_ID_MMS_INF_SEND_FAIL),\
										IMG_GLOBAL_ERROR, 1, 2000, 1) ;
					jmms_CallbackGoBackHistory();
				}
				else if(E_JMMS_NODE_READREPORT_PDU_SEND == pReqRespNode->eNodeState ||
					E_JMMS_NODE_READREPORT_MSG_SEND == pReqRespNode->eNodeState)
				{
					mmi_trace(12,"JDD_LOG: read report send fail!!");
					DisplayPopup(GetString(STR_ID_MMS_READREPORT_SENT_FAIL), IMG_GLOBAL_ERROR, 1, 2000, ERROR_TONE);
				}
				else
				{
					/* The progressbar is not shown in this case, so no need to do anything */
				}
			}
			else
			{
				if (pReqRespNode->eNodeState == E_JMMS_NODE_RETRIEVE)
				{
					jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_RETRIEVE_FAIL, &stEventData) ;
					DisplayPopup (GetString (STR_ID_MMS_INF_FAIL),\
										IMG_GLOBAL_ERROR, 1, 2000, 1) ;
					jmms_CallbackGoBackHistory();
				}
				else
				{
					jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_AUTO_RETRIEVE_FAIL, &stEventData) ;
					/* The progressbar is not shown in this case, so no need to do anything */
					jMMS_NewMMSAutoRetiveFailPopup(JC_ERR_UNKNOWN);
				}
			}
		}
	}
	else
	{
		mmi_trace(0 , "JDD_LOG: jMMS_HandleHeaderIndication::jMMS_GetReqRespNodeByReqID () returned NULL") ;
	}
	return rCode ;
}
JC_RETCODE jMMS_HandleFetchIndication (ST_COMM_FETCH_IND *pstFetchIndication)
{
	ST_REQUEST_LIST	*pReqRespNode = JC_NULL ;
	CONTENT_DATA		stMMSData = {0,} ;
	JC_UINT32			uiRefID = 0 ,uiAvailSpace = 0;
	JC_RETCODE			rCode = JC_OK ;
	
	
	pReqRespNode = jMMS_GetReqRespNodeByReqID (pstFetchIndication->uiRequestID) ;
	if (pReqRespNode && E_HTTP_GONE != pReqRespNode->eStatus)
	{
		uiRefID = pReqRespNode->uiReferenceID ;
		mmi_trace(0 , "JDD_LOG: E_FETCH_INDICATION : Node State %d, refid %d, reqnode:%d, current Length %d,rCode: %d", 
				pReqRespNode->eNodeState, uiRefID, pReqRespNode, pstFetchIndication->uiCurrentLength, pReqRespNode->rCode) ;

		if (pstFetchIndication->uiCurrentLength > 0 && JC_OK == pReqRespNode->rCode)
		{
			if (JC_NULL == pReqRespNode->pFileName)
			{
				JC_CHAR*	pFileName =JC_NULL ;
				JC_INT8 	Temp[16] = {0, } ;
				JC_UINT32	uiTempFolderPathLen ;
				JC_CHAR*    pTempFolderPath = JC_NULL;
				
				//STORE_CONFIG_SETTINGS	stStoreSettings = {0, } ;				

				//stStoreSettings.uiMsgType = 0 ;
				//jdd_MsgConfigGetDefaultStoreSettings (&stStoreSettings) ;
				//if (stStoreSettings.pRootPath)
					
				rCode = jdi_CUtilsCharToTchar (JC_NULL, MMS_TEMP_FILE_PATH, \
									&pTempFolderPath);
				if(rCode == JC_OK)
				{
					jc_sprintf(Temp, "%d",pReqRespNode->uiReferenceID) ;
	               			
					uiTempFolderPathLen = jc_tcslen (pTempFolderPath) ;
					pFileName = jdd_MemAlloc(jc_strlen(Temp) + uiTempFolderPathLen + 1, 2);
					if(pFileName)
					{
						jc_wstrcpy(pFileName, pTempFolderPath);					
						jdi_CUtilsTcsStrCat(Temp, pFileName) ;
		                //delete if it already existed
		                mmi_trace(12,"delete temp file if already existed");
						if(jdd_FSIsFileExist(NULL,pFileName))
						    jdd_FSDelete(NULL,pFileName);
					}
					else
					{
						pReqRespNode->rCode = JC_ERR_MEMORY_ALLOCATION;
					}
					jdd_MemFree (pTempFolderPath) ;
					pReqRespNode->pFileName = pFileName ;
				}				
				else
				{
					pReqRespNode->rCode = JC_ERR_MEMORY_ALLOCATION;
				}
				
			}
			
			if(pReqRespNode->eNodeState == E_JMMS_NODE_RETRIEVE || pReqRespNode->eNodeState == E_JMMS_NODE_AUTO_RETRIEVE )
					uiAvailSpace = jMMS_GetMMSFreeSpace(TRUE);
			else
					uiAvailSpace = jMMS_GetMMSFreeSpace(FALSE);	
			
			if (pReqRespNode->pFileName && pstFetchIndication->uiCurrentLength < uiAvailSpace)
			{
				pReqRespNode->hFile = jdd_FSOpen (NULL, pReqRespNode->pFileName, E_CREATE_APPEND_WRITE_MODE) ;
				if (pReqRespNode->hFile)
				{
					mmi_trace(0 , "JDD_LOG: writing in tempfile "); 
					if (1 == jdd_FSWrite (pstFetchIndication->pucBuffer, pstFetchIndication->uiCurrentLength, 1, 
								pReqRespNode->hFile))
					{
						pReqRespNode->uiCurSize += pstFetchIndication->uiCurrentLength ;
					}
					else
					{
						pReqRespNode->rCode = JC_ERR_FILE_WRITE ;
					}
					jdd_FSClose (pReqRespNode->hFile) ;
				}
				else
				{
					pReqRespNode->rCode = JC_ERR_FILE_OPEN ;
				}
			}
			else
			{
					pReqRespNode->rCode = JC_ERR_FILE_WRITE ;  // error code 
			}
		}
		if(pstFetchIndication->bHasMoreData == E_FALSE)
		{
			ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA	stEventData = {0, } ;

			jdi_CommunicationFreeReference (g_pstJMmsAppInfo->hCommunicator, pstFetchIndication->uiRequestID) ;
			mmi_trace(0 , "JDD_LOG: clsoing temp file, rCode is [%X]", -pReqRespNode->rCode); 
			rCode = pReqRespNode->rCode ;
			if(pReqRespNode->hFile && JC_OK == rCode)
			{
				mmi_trace(0 , "JDD_LOG: curr size %d",pReqRespNode->uiCurSize) ;
				if(pReqRespNode->uiCurSize > 0)
				{
					stMMSData.bIsStream = E_TRUE ;
					stMMSData.cbReadCallback = FileStreamReadCB ;
					stMMSData.pFileName = pReqRespNode->pFileName ;
					stMMSData.pStream = (void *)pReqRespNode->pFileName ;
					stMMSData.uiCurSize = 0 ;
					stMMSData.uiTotalSize = pReqRespNode->uiCurSize ;
					if (JC_NULL != pReqRespNode->pszContentType &&
						'\0' != *pReqRespNode->pszContentType &&
						0 == jc_strcmp (pReqRespNode->pszContentType, CS_MMS_CONTENT_TYPE))
					{
						mmi_trace (0, "JDD_LOG:contentlen[%d] downloadedcontent[%d]",pReqRespNode->uiContentLength, stMMSData.uiTotalSize ) ;
						if(stMMSData.uiTotalSize < pReqRespNode->uiContentLength)
						{
							rCode = JC_ERR_NOT_SUPPORTED ;
							mmi_trace (0, "JDD_LOG: complete data not downloaded contentlen[%d] downloadedcontent[%d]",pReqRespNode->uiContentLength,stMMSData.uiTotalSize  ) ;
						}
						else
						{
							rCode = jMMS_HandleFinalDownloadState (&stMMSData, pReqRespNode->uiMsgID) ;
						}
					}
					else
					{
						rCode = JC_ERR_NOT_SUPPORTED ;
						mmi_trace (0, "JDD_LOG: jMMS_HandleFetchIndication >> Not MMS Content type") ;
					}
				}
			}
			pReqRespNode->bIsToRemove = E_TRUE ;
			stEventData.uiMsgID = pReqRespNode->uiMsgID ;
			if (E_JMMS_NODE_SEND == pReqRespNode->eNodeState ||
					E_JMMS_NODE_NOTIFICATION_RESP_SEND == pReqRespNode->eNodeState ||
					E_JMMS_NODE_ACK_SEND == pReqRespNode->eNodeState ||
					E_JMMS_NODE_READREPORT_PDU_SEND == pReqRespNode->eNodeState ||
					E_JMMS_NODE_READREPORT_MSG_SEND == pReqRespNode->eNodeState)
			{
				jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_SEND_SUCCESS, &stEventData) ;
				if (E_JMMS_NODE_SEND == pReqRespNode->eNodeState)
				{
					if (JC_OK == rCode)
					{
						jmms_update_progress_screen (pReqRespNode->eNodeState, 100) ;  //success response reached, display 100%
						TurnOnBacklight(1);
						if(g_MMS_send_require == SEND_AND_SAVE)
						{
							DisplayPopup (GetString (STR_SMS_SEND_SAVE_SUCESS), IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
						}
						else
						{
							DisplayPopup (GetString (STR_ID_MMS_MSG_SEND_SUCCESS), IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
						}
                        
						jmms_CallbackGoBackHistory();
					}
					else
					{
						if (FILE_MODULE == ((-rCode) & 0x00FFFF00) && JC_ERR_FILE_NOT_FOUND != rCode)
						{
							DisplayPopup (GetString (STR_ID_MMS_FILE_SYSTEM_FULL),\
											IMG_GLOBAL_ERROR, 1, 2000, 1) ;
							jmms_CallbackGoBackHistory();
						}
						else
						{
							DisplayPopup (GetString (STR_ID_MMS_INF_SEND_FAIL),\
											IMG_GLOBAL_ERROR, 1, 2000, 1) ;
							jmms_CallbackGoBackHistory();
						}
					}
					TurnOnBacklight(1);					
				}
				else if(E_JMMS_NODE_READREPORT_MSG_SEND == pReqRespNode->eNodeState||
					E_JMMS_NODE_READREPORT_PDU_SEND == pReqRespNode->eNodeState)
				{
					mmi_trace(12,"JDD_LOG: read report send success");
					DisplayPopup(GetString(STR_ID_MMS_READREPORT_SENT_SUCCESS), IMG_GLOBAL_ACTIVATED, 1, 2000, SUCCESS_TONE);
				}
			}
			else
			{
				if (JC_OK == rCode)
				{
					if (E_JMMS_NODE_RETRIEVE == pReqRespNode->eNodeState)
					{
						jmms_update_progress_screen (pReqRespNode->eNodeState, 100) ; //success response reached, display 100%
						jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_RETRIEVE_SUCCESS, &stEventData) ;
						jmms_DownloadCompletePopup();
					}
					else
					{
						jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_AUTO_RETRIEVE_SUCCESS, &stEventData) ;
						jmms_ShowNewMMSPopup(pReqRespNode->uiMsgID);
					}
				}
				else
				{
					if (E_JMMS_NODE_RETRIEVE == pReqRespNode->eNodeState)
					{
						jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_RETRIEVE_FAIL, &stEventData) ;
						if (FILE_MODULE == ((-rCode) & 0x00FFFF00) && JC_ERR_FILE_NOT_FOUND != rCode)
						{
							DisplayPopup(GetString (STR_ID_MMS_FILE_SYSTEM_FULL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
							jmms_CallbackGoBackHistory();
						}
						else
						{
							DisplayPopup(GetString (STR_ID_MMS_INF_FAIL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
							jmms_CallbackGoBackHistory();
						}
					}
					else
					{
						jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_AUTO_RETRIEVE_FAIL, &stEventData) ;
						if (FILE_MODULE == ((-rCode) & 0x00FFFF00) && JC_ERR_FILE_NOT_FOUND != rCode)
						{
							DisplayPopup(GetString (STR_ID_MMS_FILE_SYSTEM_FULL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
						}
						else
						{
							DisplayPopup(GetString (STR_ID_MMS_INF_FAIL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;							
						}
						
					}
				}
			}
		}
	}
	return rCode ;
}

JC_RETCODE jMMS_HandleProgressIndication (ST_COMM_PROGRESS_IND *pstProgressInd)
{
	JC_UINT32 uiResultValue = 0 ;
	ST_REQUEST_LIST *pReqRespNode ;

	mmi_trace(12,"JDD_LOG: E_PROGRESS_INDICATION Req ID:%d StackStatus:%d Recvd %d  Sent %d TotRecv %d TotSend %d", \
				pstProgressInd->uiRequestID, \
				pstProgressInd->eStackStatus, \
				pstProgressInd->uiBytesRecvd,pstProgressInd->uiBytesSent, \
				pstProgressInd->uiTotalBytesToRecv, pstProgressInd->uiTotalBytesToSend) ;
	switch (pstProgressInd->eStackStatus)
	{
		case E_COMM_STACK_SENDING:
		case E_COMM_STACK_RECEIVING:
		{
			pReqRespNode = jMMS_GetReqRespNodeByReqID (pstProgressInd->uiRequestID) ;
			if (pReqRespNode)
			{
				mmi_trace(12,"JDD_LOG: Node State is [%d]", pReqRespNode->eNodeState) ;
				if (E_JMMS_NODE_SEND == pReqRespNode->eNodeState)
				{
					if (pstProgressInd->uiTotalBytesToSend)
					{				
						uiResultValue = ((pstProgressInd->uiBytesSent*MMS_MAX_PR0GRESS_PENCENT)/pstProgressInd->uiTotalBytesToSend) ;
					}
				}
				else if (E_JMMS_NODE_RETRIEVE == pReqRespNode->eNodeState)
				{
					if (pstProgressInd->uiTotalBytesToRecv)
					{				
						uiResultValue = ((pstProgressInd->uiBytesRecvd*MMS_MAX_PR0GRESS_PENCENT)/pstProgressInd->uiTotalBytesToRecv) ;
					}
				}
				if (MMS_MAX_PR0GRESS_PENCENT >= uiResultValue &&
					(E_JMMS_NODE_SEND == pReqRespNode->eNodeState ||
					E_JMMS_NODE_RETRIEVE == pReqRespNode->eNodeState))
				{
					mmi_trace (0,"JDD_LOG: show_progress_screen uiResultValue[%d]",uiResultValue) ;
					jmms_update_progress_screen (pReqRespNode->eNodeState, uiResultValue) ;
				}
			}
		}
			break;
		default:
			break;
	}
	switch (pstProgressInd->eDataConnectionStatus)
	{
		case E_DATA_CONN_CONNECTING_GPRS:
		case E_DATA_CONN_CONNECTING_CSD:
		case E_DATA_CONN_CONNECTING:
			{
				mmi_trace (0, "JDD_LOG: jMMS_HandleProgressIndication>>E_MMS_STACK_CONNECTING") ;
				g_pstJMmsAppInfo->eCommunicatorState = E_JMMS_COMM_CONNECTING ;
			}
			break ;
		case E_DATA_CONN_CONNECTED_GPRS:
		case E_DATA_CONN_CONNECTED_CSD:
		case E_DATA_CONN_CONNECTED:
			{
				mmi_trace (0, "JDD_LOG: jMMS_HandleProgressIndication>>E_MMS_STACK_CONNECTED") ;
				g_pstJMmsAppInfo->eCommunicatorState = E_JMMS_COMM_CONNECTED ;
			}
			break ;
		case E_DATA_CONN_FAIL_GPRS:
		case E_DATA_CONN_FAIL_CSD:
		case E_DATA_CONN_FAIL:
			{
				mmi_trace (0, "JDD_LOG: jMMS_HandleProgressIndication>>E_MMS_STACK_CONNECT_FAIL >> E_BE_STACK_DISCONNECTED") ;
				if (E_JMMS_COMM_IDLE != g_pstJMmsAppInfo->eCommunicatorState)
				{
					ST_REQUEST_LIST *pstHeadRequestNode = g_pstJMmsAppInfo->pReqRespList ;
					ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA	stEventData = {0, } ;
					g_pstJMmsAppInfo->eCommunicatorState = E_JMMS_COMM_IDLE ;
					if (JC_NULL != pstHeadRequestNode)
					{
						stEventData.uiMsgID = pstHeadRequestNode->uiMsgID ;
						jMMS_SendSelfEvent (E_JMMS_EVENT_DATACONN_FAIL, &stEventData) ;
					}
					else
					{
						/* No need to send the self event, as there are no nodes to process */
					}
				}
			}
			break ;
		case E_DATA_CONN_DISCONNECTING_GPRS:
		case E_DATA_CONN_DISCONNECTING_CSD:
		case E_DATA_CONN_DISCONNECTING:
			{
				mmi_trace (0, "JDD_LOG: jMMS_HandleProgressIndication>>E_MMS_STACK_DISCONNECTING") ;
				g_pstJMmsAppInfo->eCommunicatorState = E_JMMS_COMM_DISCONNECTING ;
			}
			break ;
		case E_DATA_CONN_DISCONNECTED_GPRS:
		case E_DATA_CONN_DISCONNECTED_CSD:
		case E_DATA_CONN_DISCONNECTED:
			{
				mmi_trace (0, "JDD_LOG: jMMS_HandleProgressIndication>>E_BE_STACK_DISCONNECTED") ;
				if (E_JMMS_COMM_DISCONNECTED != g_pstJMmsAppInfo->eCommunicatorState)
				{
					ST_REQUEST_LIST *pstHeadRequestNode = g_pstJMmsAppInfo->pReqRespList ;
					ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA	stEventData = {0, } ;
					g_pstJMmsAppInfo->eCommunicatorState = E_JMMS_COMM_DISCONNECTED ;
					if (JC_NULL != pstHeadRequestNode)
					{
						stEventData.uiMsgID = pstHeadRequestNode->uiMsgID ;
						jMMS_SendSelfEvent (E_JMMS_EVENT_DATACONN_SUCCESS, &stEventData) ;
					}
					else
					{
						/* No need to send the self event, as there are no nodes to process */
					}
				}
			}
			break ;
		default:
			break;
	}
	return JC_OK ;
}

JC_RETCODE jMMS_HandleStopIndication (ST_COMM_STOP_IND *pstStopIndication)
{
	ST_REQUEST_LIST	*pReqRespNode = JC_NULL ;
	ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA	stEventData = {0, } ;
	BOOL isAutoRetriveStop = FALSE;

	if (0 != pstStopIndication->uiRequestID)
	{
		jdi_CommunicationFreeReference (g_pstJMmsAppInfo->hCommunicator, pstStopIndication->uiRequestID) ;
	}
	pReqRespNode = jMMS_GetReqRespNodeByReqID (pstStopIndication->uiRequestID) ;
	if (pReqRespNode)
	{
		if(pReqRespNode->eNodeState == E_JMMS_NODE_AUTO_RETRIEVE)
			isAutoRetriveStop = TRUE;
		pReqRespNode->bIsToRemove = E_TRUE ;
		stEventData.uiMsgID = pReqRespNode->uiMsgID ;
		jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_SEND_FAIL, &stEventData) ;
	}
	mmi_trace(12, "JDD_LOG: jMMS_HandleStopIndication, isAutoRetriveStop is:%d", isAutoRetriveStop);
	if(!isAutoRetriveStop)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_ABORT), IMG_GLOBAL_WARNING,\
							1,	2000, 1) ;
		jmms_CallbackGoBackHistory();
	}
	return JC_OK ;
}
JC_RETCODE jMMS_HandleErrorIndication (ST_COMM_ERR_IND *pstErrorIndication)
{
	ST_REQUEST_LIST	*pReqRespNode = JC_NULL ;
	ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA	stEventData = {0, } ;

	mmi_trace(12,"JDD_LOG: Error Callback : E_ERROR_INDICATION [%d]:uiRequestID[%d],rCode[%d] ",pstErrorIndication->eErrType,
															pstErrorIndication->uiRequestID,
															pstErrorIndication->rCode) ;
	pReqRespNode = jMMS_GetReqRespNodeByReqID (pstErrorIndication->uiRequestID) ;
	if (pReqRespNode && E_HTTP_GONE != pReqRespNode->eStatus)
	{
		if (0 != pstErrorIndication->uiRequestID)
		{
			jdi_CommunicationFreeReference (g_pstJMmsAppInfo->hCommunicator, pstErrorIndication->uiRequestID) ;
		}
		if (E_JMMS_NODE_SEND == pReqRespNode->eNodeState ||
			E_JMMS_NODE_RETRIEVE == pReqRespNode->eNodeState
			)
		{
			jMMS_ErrorHandler (pstErrorIndication->rCode, NULL) ;
			jmms_CallbackGoBackHistory();
		}
		else if (E_JMMS_NODE_AUTO_RETRIEVE == pReqRespNode->eNodeState)
		{
			jMMS_NewMMSAutoRetiveFailPopup(pstErrorIndication->rCode);
		}
					
		pReqRespNode->bIsToRemove = E_TRUE ;
		stEventData.uiMsgID = pReqRespNode->uiMsgID ;
		if (E_JMMS_NODE_SEND == pReqRespNode->eNodeState ||
			E_JMMS_NODE_NOTIFICATION_RESP_SEND == pReqRespNode->eNodeState ||
			E_JMMS_NODE_ACK_SEND == pReqRespNode->eNodeState ||
			E_JMMS_NODE_READREPORT_PDU_SEND == pReqRespNode->eNodeState ||
			E_JMMS_NODE_READREPORT_MSG_SEND == pReqRespNode->eNodeState)
		{
			jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_SEND_FAIL, &stEventData) ;
		}
		else if (E_JMMS_NODE_RETRIEVE == pReqRespNode->eNodeState)
		{
			jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_RETRIEVE_FAIL, &stEventData) ;
		}
		else
		{
			jMMS_SendSelfEvent (E_JMMS_EVENT_MSG_AUTO_RETRIEVE_FAIL, &stEventData) ;
		}
	}
	return JC_OK ;
}
#endif


