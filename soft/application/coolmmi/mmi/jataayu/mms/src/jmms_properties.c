/***************************************************************************
*
* File Name : MMSMMIMsgProperties.c
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
* File Description   jmms_properties.h
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
* System Include Files
**************************************************************************/



/***************************************************************************
* User Include Files
**************************************************************************/
#include "jmms_inc.h"
#include "jmms_staticdefines.h"
#include "jmms_properties.h"
#include "datetimegprot.h"
#include "phonesetupgprots.h"
#include "messagesresourcedata.h"
#include "jmms_view.h"
/***************************************************************************
* Macros Definitions
**************************************************************************/

/***************************************************************************
* Global variables declarations
**************************************************************************/

/***************************************************************************
* Local Function Prototypes
***************************************************************************/
/* Explicitly declared here, but it should be declared in Worldclock.h */
S32 GetTimeZone (U8 cityIndex) ;

void jMMS_MsgPropertiesHandler()
{	
	MESSAGE_LIST		*pstMsgDetail = NULL ;	
							
	jmms_GetMsgDetailsByID (g_pstJMmsAppInfo->hMsgStore, g_pstJMmsAppInfo->eCurrentFolderID,
							g_pstJMmsAppInfo->uiHighlightedMsgId, &pstMsgDetail) ;
	
	if (pstMsgDetail!=NULL && pstMsgDetail->pMessageHeaders->pHeaders)
	{
	g_pstJMmsAppInfo->pszScreeBuff = jdd_MemAlloc(JMMS_MAX_SCREEN_BUFFER + 1 , sizeof(JC_CHAR));
	mmi_trace (1, "JDD_LOG: jMMS_MsgPropertiesHandler >> pstMsgDetail->pMessageHeaders->eMsgType is [%d]", pstMsgDetail->pMessageHeaders->eMsgType) ;
	switch((JC_UINT32)pstMsgDetail->pMessageHeaders->eMsgType)
	{
		case E_MSG_TYPE_SEND:
			jmms_PropertiesSetSendHeaders(pstMsgDetail->pMessageHeaders->pHeaders, pstMsgDetail->uiBufferLen, pstMsgDetail->uiMsgSaveDate) ;
		break ;

		case E_MSG_TYPE_NOTIFICATION:
			jmms_PropertiesNotificationHeaders(pstMsgDetail->pMessageHeaders->pHeaders, pstMsgDetail->uiMsgSaveDate) ;
		break ;

		case E_MSG_TYPE_RECEIVE:
			jmms_PropertiesRetrieveHeaders(pstMsgDetail->pMessageHeaders->pHeaders, pstMsgDetail->uiBufferLen) ;
		break ; 

	    case E_MSG_TYPE_DELIVERY_REPORT :
	        jmms_PropertiesDeliveryReportHeaders(pstMsgDetail->pMessageHeaders->pHeaders) ;
	    break ;

	    case E_MSG_TYPE_READ_REPORT_RESPONSE :
	        jmms_PropertiesReadResponseHeaders(pstMsgDetail->pMessageHeaders->pHeaders) ;
	    break ;
		
	}

	jmms_MMIPropertiesScreenEntryHandler();
	
}

}

void jmms_PropertiesSetSendHeaders(MESSAGE_HEADERS *pHeaders, JC_UINT32 uiBufferLen, JC_UINT32	uiMsgSaveDate)
{
	MESSAGE_SEND	*pSendHeader = (MESSAGE_SEND *)pHeaders ;



#if 0
	//title
	jc_wstrcpy (g_pstJMmsAppInfo->pszScreeBuff, GetString (STR_ID_MMS_MSG_DETAILS));
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;

	//type
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)GetString (STR_ID_MMS_MSG_TYPE)) ;
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)GetString (STR_ID_MMS_MSG_SENDTYPE_DETAILS)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
#endif /* Guoj delete. It's not used now at 2009-11-4 */

#if 0	
	if(pSendHeader->pTransactionID)
	{
		jdi_CUtilsTcsStrCat("\nTrID:", g_pstJMmsAppInfo->pszScreeBuff) ;
		jdi_CUtilsTcsStrCat(pSendHeader->pTransactionID, g_pstJMmsAppInfo->pszScreeBuff) ;		
	}
#endif


	jmms_ShowSubject(pSendHeader->pSubject);
	jmms_ShowVersion(pSendHeader->pVersion);
	jmms_ShowFrom(pSendHeader->pFrom);
	jmms_ShowCC(pSendHeader->pCc);
	jmms_ShowBcc(pSendHeader->pBcc);
	jmms_ShowTo(pSendHeader->pTo);
	jmms_ShowClass(pSendHeader->eMsgClass);	
	jmms_ShowPriority(pSendHeader->eMsgPriority);
//	jmms_ShowDeliveryReport(pSendHeader->bDeliveryReport);
	jmms_ShowDateTime(uiMsgSaveDate, E_FALSE);
	jmms_ShowSize(uiBufferLen);
	

	
#if 0
	jdi_CUtilsTcsStrCat("\nHide Sender:", g_pstJMmsAppInfo->pszScreeBuff) ;
	if(pSendHeader->bHideSender == E_FALSE)
		jdi_CUtilsTcsStrCat("No", g_pstJMmsAppInfo->pszScreeBuff) ;
	else
		jdi_CUtilsTcsStrCat("Yes", g_pstJMmsAppInfo->pszScreeBuff) ;	
#endif	


#if 0	
	if(cMinorVersion > 1)
	{
		if(pSendHeader->pMBoxInfo)
		{
			jdi_CUtilsTcsStrCat("\nMBox Store:", g_pstJMmsAppInfo->pszScreeBuff) ;
			if(pSendHeader->pMBoxInfo->bMBoxStore == E_FALSE)
			{
				jdi_CUtilsTcsStrCat("No", g_pstJMmsAppInfo->pszScreeBuff) ;
			}
			else
			{
				jdi_CUtilsTcsStrCat("Yes", g_pstJMmsAppInfo->pszScreeBuff) ;
			}			
		}
	}
	if(cMinorVersion > 1)
	{
		if(pSendHeader->pReplyChargingInfo)
		{
			JC_INT8	TempStr[10] = {0, };

			jdi_CUtilsTcsStrCat("\nRC Type:", g_pstJMmsAppInfo->pszScreeBuff) ;
			jdi_CUtilsTcsStrCat(ReplyChargingStr[pSendHeader->pReplyChargingInfo->eReplyCharging], g_pstJMmsAppInfo->pszScreeBuff) ;
			
			jdi_CUtilsTcsStrCat("\nRC Time:", g_pstJMmsAppInfo->pszScreeBuff) ;
			jc_itoa(pSendHeader->pReplyChargingInfo->uiRelativeTime, TempStr, 10) ;
			jdi_CUtilsTcsStrCat( TempStr, g_pstJMmsAppInfo->pszScreeBuff) ;	

			jdi_CUtilsTcsStrCat("\nRC Size:", g_pstJMmsAppInfo->pszScreeBuff) ;
			jc_itoa(pSendHeader->pReplyChargingInfo->uiSize, TempStr, 10) ;
			jdi_CUtilsTcsStrCat(TempStr, g_pstJMmsAppInfo->pszScreeBuff) ;
		}
	}
#endif	
}

void jmms_PropertiesNotificationHeaders (MESSAGE_HEADERS *pHeaders, JC_UINT32 uiMsgSaveDate)
{	
	MESSAGE_NOTIFICATION	*pNotifyHeader = (MESSAGE_NOTIFICATION *)pHeaders ;

#if 0
	//title
	jc_wstrcpy (g_pstJMmsAppInfo->pszScreeBuff, GetString (STR_ID_MMS_MSG_DETAILS));
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	
	//type
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
				(JC_CHAR*)GetString (STR_ID_MMS_MSG_TYPE)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
				(JC_CHAR*)GetString (STR_ID_MMS_MSG_NOTIFICATIONTYPE_DETAILS)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
#endif /* Guoj delete. It's not used now at 2009-11-4 */
	

	jmms_ShowSubject(pNotifyHeader->pSubject);
	jmms_ShowVersion(pNotifyHeader->pVersion);
	jmms_ShowFrom(pNotifyHeader->pFrom);
	jmms_ShowClass(pNotifyHeader->eMsgClass);	
	jmms_ShowPriority(pNotifyHeader->eMsgPriority);
//	jmms_ShowDeliveryReport(pNotifyHeader->bDeliveryReport);
	jmms_ShowDateTime(uiMsgSaveDate, E_FALSE);
	jmms_ShowSize(pNotifyHeader->uiMessageSize);



#if 0	
	if(cMinorVersion > 1)
	{	
		jdi_CUtilsTcsStrCat("\nMBox Stored:", g_pstJMmsAppInfo->pszScreeBuff) ;
		if(pNotifyHeader->bMMBoxStored == E_FALSE)
		{
			jdi_CUtilsTcsStrCat("No", g_pstJMmsAppInfo->pszScreeBuff) ;
		}
		else
		{
			jdi_CUtilsTcsStrCat("Yes", g_pstJMmsAppInfo->pszScreeBuff) ;
		}	

		jdi_CUtilsTcsStrCat("\nDistributable:", g_pstJMmsAppInfo->pszScreeBuff) ;
		if(pNotifyHeader->bDistribution == E_FALSE)
		{
			jdi_CUtilsTcsStrCat("No", g_pstJMmsAppInfo->pszScreeBuff) ;
		}
		else
		{
			jdi_CUtilsTcsStrCat("Yes", g_pstJMmsAppInfo->pszScreeBuff) ;
		}
	}

	if (cMinorVersion > 0)
	{
		if(pNotifyHeader->pReplyChargingInfo)
		{
			jdi_CUtilsTcsStrCat("\nRC Type:", g_pstJMmsAppInfo->pszScreeBuff) ;
			jdi_CUtilsTcsStrCat(ReplyChargingStr[pNotifyHeader->pReplyChargingInfo->eReplyCharging],\
								g_pstJMmsAppInfo->pszScreeBuff) ;			

			jdi_CUtilsTcsStrCat("\nRC Time:", g_pstJMmsAppInfo->pszScreeBuff) ;
			jc_itoa(pNotifyHeader->pReplyChargingInfo->uiRelativeTime, TempStr, 10) ;
			jdi_CUtilsTcsStrCat(TempStr, g_pstJMmsAppInfo->pszScreeBuff) ;
			

			jdi_CUtilsTcsStrCat("\nRC Size:", g_pstJMmsAppInfo->pszScreeBuff) ;
			jc_itoa(pNotifyHeader->pReplyChargingInfo->uiSize, TempStr, 10) ;
			jdi_CUtilsTcsStrCat(TempStr, g_pstJMmsAppInfo->pszScreeBuff) ;			
		}
	}
	if(pNotifyHeader->pContentLocation)//location
	{
		jdi_CUtilsTcsStrCat("\nLocation:", g_pstJMmsAppInfo->pszScreeBuff) ;
		jdi_CUtilsTcsStrCat(pNotifyHeader->pContentLocation, g_pstJMmsAppInfo->pszScreeBuff) ;
	}
#endif		
}

void jmms_PropertiesRetrieveHeaders(MESSAGE_HEADERS *pHeaders, JC_UINT32 uiBufferLen)
{
	MESSAGE_RETRIEVAL	*pRetrieveHeader = (MESSAGE_RETRIEVAL *)pHeaders ;
	jc_memset(g_pstJMmsAppInfo->pszScreeBuff, 0 , sizeof(JC_CHAR));
	
#if 0
	//title
	jc_wstrcpy (g_pstJMmsAppInfo->pszScreeBuff, GetString (STR_ID_MMS_MSG_DETAILS));
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;

	//type
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)GetString (STR_ID_MMS_MSG_TYPE)) ;
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)GetString (STR_ID_MMS_MSG_RTVTYPE_DETAILS)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
#endif /* Guoj delete. It's not used now at 2009-11-4 */

	jmms_ShowSubject(pRetrieveHeader->pSubject);
	jmms_ShowVersion(pRetrieveHeader->pVersion);
	jmms_ShowFrom(pRetrieveHeader->pFrom);
	jmms_ShowTo(pRetrieveHeader->pTo);
	jmms_ShowCC(pRetrieveHeader->pCc);

	jmms_ShowClass(pRetrieveHeader->eMsgClass);	
	jmms_ShowPriority(pRetrieveHeader->eMsgPriority);
//	jmms_ShowDeliveryReport(pRetrieveHeader->bDeliveryReport);
//	jmms_ShowReadReport(pRetrieveHeader->bReadReport);
	jmms_ShowDateTime(pRetrieveHeader->uiDate, E_TRUE);
	jmms_ShowRetrieveStatus(pRetrieveHeader->pStatusText);
	jmms_ShowSize(uiBufferLen);
	
	
	
#if 0
	if(cMinorVersion > 1)
	{
		jdi_CUtilsTcsStrCat("\nDistributable:", g_pstJMmsAppInfo->pszScreeBuff) ;
		if(pRetrieveHeader->bDistribution == E_FALSE)
		{
			jdi_CUtilsTcsStrCat("No", g_pstJMmsAppInfo->pszScreeBuff) ;
		}
		else
		{
			jdi_CUtilsTcsStrCat("Yes", g_pstJMmsAppInfo->pszScreeBuff) ;
		}	
	}
	if(cMinorVersion > 0)
	{
		if(pRetrieveHeader->pReplyChargingInfo)
		{
			JC_INT8	TempStr[10] = {0, };

			jdi_CUtilsTcsStrCat("\nRC Type:", g_pstJMmsAppInfo->pszScreeBuff) ;
			jdi_CUtilsTcsStrCat(ReplyChargingStr[pRetrieveHeader->pReplyChargingInfo->eReplyCharging], g_pstJMmsAppInfo->pszScreeBuff) ;
			

			jdi_CUtilsTcsStrCat("\nRC Time:", g_pstJMmsAppInfo->pszScreeBuff) ;
			jc_itoa(pRetrieveHeader->pReplyChargingInfo->uiRelativeTime, TempStr, 10) ;
			jdi_CUtilsTcsStrCat(TempStr, g_pstJMmsAppInfo->pszScreeBuff) ;
			

			jdi_CUtilsTcsStrCat("\nRC Size:", g_pstJMmsAppInfo->pszScreeBuff) ;
			jc_itoa(pRetrieveHeader->pReplyChargingInfo->uiSize, TempStr, 10) ;
			jdi_CUtilsTcsStrCat(TempStr, g_pstJMmsAppInfo->pszScreeBuff) ;
			
		}
	}
#endif	
}

void	jmms_PropertiesDeliveryReportHeaders(MESSAGE_HEADERS *pHeaders)
{    
	DELIVERY_REPORT *pDeliveryRepHeader = NULL ;
    pDeliveryRepHeader = (DELIVERY_REPORT *)pHeaders ;


#if 0
	//title
	jc_memset(g_pstJMmsAppInfo->pszScreeBuff, 0 , sizeof(JC_CHAR));
	jc_wstrcpy (g_pstJMmsAppInfo->pszScreeBuff, GetString (STR_ID_MMS_MSG_DETAILS));
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;

	//type
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)GetString (STR_ID_MMS_MSG_TYPE)) ;
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)GetString (STR_ID_MMS_DELIVERY_REPORT)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
#endif /* Guoj delete. It's not used now at 2009-11-4 */
	
	jmms_ShowVersion(pDeliveryRepHeader->pVersion);
	jmms_ShowTo(pDeliveryRepHeader->pTo);
	jmms_ShowDateTime(pDeliveryRepHeader->uiDate, E_TRUE);

	jmms_ShowDeliveryStatus(pDeliveryRepHeader->eDeliveryStatus);

}

void	jmms_PropertiesReadResponseHeaders(MESSAGE_HEADERS *pHeaders)
{    
	READ_REPORT_RESPONSE *pReadRepResHeader = (READ_REPORT_RESPONSE *)pHeaders ;
#if 0

 	//title  
	jc_memset(g_pstJMmsAppInfo->pszScreeBuff, 0 , sizeof(JC_CHAR));
	jc_wstrcpy (g_pstJMmsAppInfo->pszScreeBuff, GetString (STR_ID_MMS_MSG_DETAILS));
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;

	//type
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)GetString (STR_ID_MMS_MSG_TYPE)) ;
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)GetString (STR_ID_MMS_READ_REPORT)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
#endif /* Guoj delete. It's not used now at 2009-11-4 */

	jmms_ShowVersion(pReadRepResHeader->pVersion);
	jmms_ShowFrom(pReadRepResHeader->pFrom);
	jmms_ShowTo(pReadRepResHeader->pTo);

	jmms_ShowDateTime(pReadRepResHeader->uiReceivedTime, E_TRUE);

	jmms_ShowReadReportStatus(pReadRepResHeader->eReadStatus);
}

void jmms_MMIPropertiesScreenEntryHandler()
{
	EntryNewScreen(SCR_ID_MMS_DELIVERY_REPORT, jmms_MMIReportScreenExitHandler, jMMS_MsgPropertiesHandler, NULL);
	ShowCategory78Screen(STR_ID_MMS_PROPERTY,IMG_SMS_ENTRY_SCRN_CAPTION,
						0, 0,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						NULL,
						(U8*) g_pstJMmsAppInfo->pszScreeBuff,
						jc_tcslen(g_pstJMmsAppInfo->pszScreeBuff),
						NULL);
	SetLeftSoftkeyFunction (0,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);	
}
#endif
