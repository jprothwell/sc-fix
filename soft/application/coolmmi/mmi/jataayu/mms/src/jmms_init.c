#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_init.c"

/***************************************************************************
 *
 * File Name : jmms_init.c
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
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND 
 * AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
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
 ***************************************************************************/

/** 
 * @file		jmms_init.c
 * @ingroup		jmms
 * @brief	
 *
 **/

#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_init.c"

 #ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

#include "jmms_inc.h"
#include "jmms_composedlg.h"
#include "jmms_storedlg.h"
#include "messagesresourcedata.h"


 /***************************************************************************
 * Macros
 **************************************************************************/


/***************************************************************************
* Global variables declarations
**************************************************************************/
JC_BOOLEAN g_bIsMMSAPPActive;

/***************************************************************************
* Local Function Prototypes
***************************************************************************/

/***************************************************************************
* All Local Function Definitions
***************************************************************************/

/***************************************************************************
* All Global Function Definitions
**************************************************************************/
void initJMms ()
{						
	mmi_trace (1, "@@@ initJMms  @@@") ;	
	SetHiliteHandler(MMS_MAIN_SCREEN_MENUID, jMms_highlight_handler);
	
	SetHiliteHandler(MMS_SEND_ONLY, jMms_Highlight_Send);
	SetHiliteHandler(MMS_SEND_AND_SAVE, jMms_Highlight_Send_And_Save);
	SetHiliteHandler(MMS_SAVE, jMms_Highlight_Save);
	SetHiliteHandler(MMS_EXIT_COMPOSE, jMms_Highlight_Exit_Compose);

	SetHiliteHandler(MMS_RESEND_SEND_ONLY, jMms_Highlight_Resend_Send);
	SetHiliteHandler(MMS_RESEND_SEND_AND_SAVE, jMms_Highlight_Resend_Send_And_Save);
	
	SetHintHandler(MMS_MAIN_INBOX_MENUID,jMms_inbox_hint_handler);
	SetHintHandler(MMS_MAIN_SENT_MENUID,jMms_sent_highlight_handler);
	SetHintHandler(MMS_MAIN_OUTBOX_MENUID,jMms_outbox_highlight_handler);
	SetHintHandler(MMS_MAIN_DRAFT_MENUID,jMms_draft_highlight_handler);

	jMms_Set_Edit_Content_Highlight_Handler();
	
	jMms_init_context () ;
	jMms_comm_init () ;		
	jMms_init_engine() ;
} 
extern U8 r2MMIFlagCached;
extern BOOL r2lMMIFlag  ;
void jMMS_SetMMSAppStatus(JC_BOOLEAN bIsActive)
{
	g_bIsMMSAPPActive = bIsActive;
		#if 1  //for Arabic displaying 
    if(bIsActive )
    {
        r2MMIFlagCached = r2lMMIFlag;
        r2lMMIFlag = FALSE;
    }
    else
    {
        if(r2MMIFlagCached!=0xff)
        {
            r2lMMIFlag = r2MMIFlagCached;
            r2MMIFlagCached = 0xff;
        }
    }
        #endif
}

JC_BOOLEAN jMMS_GetMMSAppStatus()
{
	return g_bIsMMSAPPActive;
}

void jMMS_processAppEvent(JC_EVENT_DATA *pEventData) 
{	
	if(E_JMMS_EVENT_TIMER == pEventData->iEventType)
	{
		if(g_pstJMmsAppInfo->hCurrMsg)
		{
			jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg, E_VIEW_TIMER,JC_NULL) ;
		}
	}
	else if(E_JMMS_EVENT_PAGE_PRESENTATION == pEventData->iEventType)
	{
		jMMS_HandlePresenatationEvnts(pEventData->pEventInfo) ;
	}
	else if (E_JMMS_EVENT_MSG_RETRIEVE_SUCCESS == pEventData->iEventType)
	{
		ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA *pstEventInfo = (ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA *)pEventData->pEventInfo ;
		jMMS_ProcessCommunicatorFetchRequest (pstEventInfo->uiMsgID, E_JMMS_NODE_ACK_SEND, E_JMMS_MSG_RETRIEVED) ;
	}
	else if (E_JMMS_EVENT_MSG_AUTO_RETRIEVE_SUCCESS == pEventData->iEventType)
	{
		ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA *pstEventInfo = (ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA *)pEventData->pEventInfo ;
		jMMS_ProcessCommunicatorFetchRequest (pstEventInfo->uiMsgID, E_JMMS_NODE_NOTIFICATION_RESP_SEND, E_JMMS_MSG_RETRIEVED) ;
	}
	else if (E_JMMS_EVENT_MSG_AUTO_RETRIEVE_FAIL == pEventData->iEventType)
	{
		ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA *pstEventInfo = (ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA *)pEventData->pEventInfo ;
		jMMS_ProcessCommunicatorFetchRequest (pstEventInfo->uiMsgID, E_JMMS_NODE_NOTIFICATION_RESP_SEND, E_JMMS_MSG_DEFERRED) ;
	}
	else if (E_JMMS_EVENT_MSG_SEND_SUCCESS == pEventData->iEventType ||
				E_JMMS_EVENT_MSG_SEND_FAIL == pEventData->iEventType ||
				E_JMMS_EVENT_MSG_RETRIEVE_FAIL == pEventData->iEventType)
	{
		jMMS_ProcessCommunicatorFetchRequest (0, E_JMMS_NODE_IDLE, E_JMMS_MSG_NIL) ;
	}
	else if (E_JMMS_EVENT_DATACONN_SUCCESS == pEventData->iEventType ||
				E_JMMS_EVENT_DATACONN_FAIL == pEventData->iEventType)
	{
		jMMS_ProcessCommunicatorFetchRequest (0, E_JMMS_NODE_IDLE, E_JMMS_MSG_NIL) ;
	}
	mmi_trace(1,"JDD_LOG: jMMS_processAppEvent  Event Id [%d]",pEventData->iEventType ) ;
}

void  jMMS_HandlePresenatationEvnts(void* pvEventInfo)
{
  PAGE_PRESENTATION_IND *pPagePresentationInfo = (PAGE_PRESENTATION_IND *)pvEventInfo;
  mmi_trace(1," @@@ jMMS_HandlePresenatationEvnts is called with [%d]", pPagePresentationInfo->eState);
  switch (pPagePresentationInfo->eState)
	{
		case E_STATE_PAGE_START:
				jMms_SetMmsViewTitle();
			break;		
		case E_STATE_PAGE_END:
			break;
		case E_STATE_PAGE_PAUSE:
			break;
		case E_STATE_PRESENTATION_END:
			if(GetActiveScreenId() == SCR_ID_MMS_VIEW)
			{
				jmms_HandleViewPresentationEnd();
			}
			else
			{
				mmi_trace(1," @@@ E_STATE_PRESENTATION_END is called");
			}	
			break;
		default :
			break;
	}
}

void jMms_SetMmsViewTitle()
{
#define PMSTRINT_LEN 48
#define PMS_MAZ_SLIDE_PERCENT 8
#define TITLE_BUF_LEN 16

    JC_UINT8    currPage = 0;
    JC_UINT8    totalPages = 0;
    JC_RETCODE  eRet = JC_OK;    
	JC_INT8 titlebuff[TITLE_BUF_LEN];
	JC_INT8 tmpStr_UCS2[TITLE_BUF_LEN];	
	JC_INT8 pmstrDest[PMSTRINT_LEN];
	JC_BOOLEAN bDisplayAudio =  E_FALSE;
	JC_BOOLEAN bDisplayAttachment =  E_FALSE;

	if(!(g_pstJMmsAppInfo->hCurrMsg))
	{
		mmi_trace(1,"$$$ ASSSSSSSSERTTTTTTT");
		return;
	}    
    eRet = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &totalPages) ;
    eRet = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg,&currPage);
	if( totalPages == 0)
		totalPages = 1;
	if(currPage == 0)
		currPage = 1;
    memset(pmstrDest, 0, sizeof(pmstrDest));
    if (JC_OK == eRet)
    {
		pfnUnicodeStrncat(pmstrDest, GetString(STR_ID_MMS_SLIDE), PMSTRINT_LEN/2-PMS_MAZ_SLIDE_PERCENT);
	       jc_memset(titlebuff,0,sizeof(titlebuff));  
		jc_sprintf(titlebuff, "  %d/%d", currPage, totalPages);
		AnsiiToUnicodeString(tmpStr_UCS2, titlebuff);
		pfnUnicodeStrncat(pmstrDest, tmpStr_UCS2,PMS_MAZ_SLIDE_PERCENT);
		
		ChangeTitleString((U8*)pmstrDest);
		if (GetActiveScreenId() == SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN)
		{
			MEDIA_INFO	*pMediaInfo		= JC_NULL ;
			JC_UINT8	ucMediaCount	= 0 ;
			if (0 != g_pstJMmsAppInfo->ActivePageMediaIds[2])
			{
				bDisplayAudio =  E_TRUE;
			}

			eRet = jdi_MsgGetMediaList ( g_pstJMmsAppInfo->hCurrMsg, currPage , &pMediaInfo , 
										&ucMediaCount, E_TRUE) ;
			if(ucMediaCount > 0)
			{
				bDisplayAttachment =  E_TRUE;
			}			
		}
		else if (GetActiveScreenId() == SCR_ID_MMS_VIEW)
		{
			MEDIA_INFO	*pMediaInfo		= JC_NULL ;
			JC_UINT8	ucMediaCount	= 0 ;			

			eRet = jdi_MsgGetMediaList ( g_pstJMmsAppInfo->hCurrMsg, currPage , &pMediaInfo , 
										&ucMediaCount, E_FALSE) ;
		    while(pMediaInfo)
			{
				if (pMediaInfo->eMediaType == E_MEDIA_AUDIO && pMediaInfo->uiMediaID != 0)
					{
						bDisplayAudio =  E_TRUE;
					}
				pMediaInfo = pMediaInfo->pNext ;
			}

			eRet = jdi_MsgGetMediaList ( g_pstJMmsAppInfo->hCurrMsg, currPage , &pMediaInfo , 
										&ucMediaCount, E_TRUE) ;
			if(ucMediaCount > 0)
			{
				bDisplayAttachment =  E_TRUE;
			}				

		}

		if((bDisplayAudio == E_TRUE) && (bDisplayAttachment == E_FALSE))
		{
			ChangeTitleIcon(IMG_MMS_AUDIO_ICON);	
		}
		else if((bDisplayAttachment == E_TRUE) && (bDisplayAudio == E_FALSE))
		{
			ChangeTitleIcon(IMG_MMS_ATTACHMENT_ICON);
		}
		else if((bDisplayAttachment == E_TRUE) && (bDisplayAudio == E_TRUE))
		{
			ChangeTitleIcon(IMG_MMS_ATTACHMENT_AUDIO_ICON);
		}	
		else
		{
			ChangeTitleIcon(IMG_SMS_ENTRY_SCRN_CAPTION);
		
		}		
		draw_title();
    }		
}

#endif //__JATAAYU_APP__

