#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_mmi_cb.c"
 /***************************************************************************
 *
 * File Name : jmms_mmi_cb.c
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
 * @file		jmms_mmi_cb.c
 * @ingroup		jMMS
 * @brief	
 *
 **/

#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_mmi_cb.c"
#ifdef JATAAYU_SUPPORT 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

#include "jmms_inc.h"

#include "jmms_addrdlg.h"
#include "jmms_storedlg.h"
#include "jmms_composedlg.h"
#include "jmms_composedlg.h"
#include "jmms_composeutils.h"
#include "jmms_interface.h"
#include "jmms_sendrecv.h"
#include "jmms_communicator.h"
#include "fs.h"
#include "messagesresourcedata.h"
#include "debuginitdef.h"
#include "ts.h"
#include "wgui_touch_screen.h"
 /***************************************************************************
 * Macros
 **************************************************************************/


/***************************************************************************
* Global variables declarations
**************************************************************************/
E_JMMS_QUEUE_NODE_STATE g_eNodeState;
JC_UINT32 g_uiResultValue;
#define JMMS_SCROLL_PMMI_OFFSET 10

/***************************************************************************
* Local Function Prototypes
***************************************************************************/

/***************************************************************************
* All Local Function Definitions
***************************************************************************/

/***************************************************************************
* All Global Function Definitions
**************************************************************************/

/*
void jmms_SendMmsProgressScreenExitHandler()
{
	history_t currHistory;	
	currHistory.scrnID = SCR_ID_SENDING_SCREEN;
	currHistory.entryFuncPtr = jmms_SendMmsProgressScreenEntryHandler;
	AddHistory( currHistory );	
}
*/
extern void jmms_setInterruptHandler(void);
extern void jmms_clearInterruptHandler(void);
extern void mmi_show_qq_select_confirm(void (*default_entry) (void));
extern BOOL checkVMOn(void);


static  void jmms_SendMmsProgressScreenExitHandler()
{
	jmms_clearInterruptHandler();
}
static  void jmms_SendMmsProgressScreenEntryHandler()
{
	U8* guiBuffer;
	U16 eStrID = (E_JMMS_NODE_SEND == g_eNodeState) ? STR_ID_MMS_SENDING : \
					STR_ID_MMS_DOWNLOADING ;
	
	EntryNewScreen(SCR_ID_SENDING_SCREEN, jmms_SendMmsProgressScreenExitHandler, \
					jmms_SendMmsProgressScreenEntryHandler, JC_NULL);
	
	jmms_setInterruptHandler();
	
	guiBuffer = GetCurrGuiBuffer (SCR_ID_SENDING_SCREEN) ;
	ShowCategory72Screen(STR_ID_MMS_TEXT,IMG_SMS_ENTRY_SCRN_CAPTION,
						0,0,
						STR_GLOBAL_CANCEL, 0,
						(U8*)GetString(eStrID), g_uiResultValue, guiBuffer);	

	SetRightSoftkeyFunction(show_ExitProgress_screen,KEY_EVENT_DOWN);	
	SetKeyHandler(jmms_SendProgressEndKeyHandler, KEY_END, KEY_EVENT_DOWN) ;
}
void show_progress_screen(E_JMMS_QUEUE_NODE_STATE eNodeState, JC_UINT32 uiResultValue)
{
 	g_eNodeState = eNodeState;
	g_uiResultValue = uiResultValue;
	mmi_trace(g_sw_MMS,"JDD_LOG: Start to show_progress_screen g_eNodeState:%d, g_uiResultValue:%d", g_eNodeState,g_uiResultValue);
	jmms_SendMmsProgressScreenEntryHandler();
}


void jmms_update_progress_screen(E_JMMS_QUEUE_NODE_STATE eNodeState, JC_UINT32 uiResultValue)
{
	U8* guiBuffer;
	U16 eStrID = (E_JMMS_NODE_SEND == g_eNodeState) ? STR_ID_MMS_SENDING : \
					STR_ID_MMS_DOWNLOADING ;

	if(GetActiveScreenId() != SCR_ID_SENDING_SCREEN)
		return ;
	
	guiBuffer = GetCurrGuiBuffer (SCR_ID_SENDING_SCREEN) ;

	
	g_eNodeState = eNodeState;
	g_uiResultValue = uiResultValue;

	ShowCategory72Screen(STR_ID_MMS_TEXT,IMG_SMS_ENTRY_SCRN_CAPTION,
						0,0,
						STR_GLOBAL_CANCEL, 0,
						(U8*)GetString(eStrID), g_uiResultValue, guiBuffer);	
	if (MMS_MAX_PR0GRESS_PENCENT > g_uiResultValue) 
	{
		SetRightSoftkeyFunction(show_ExitProgress_screen,KEY_EVENT_DOWN);	
		SetKeyHandler(jmms_SendProgressEndKeyHandler, KEY_END, KEY_EVENT_DOWN) ;
	}
	else //Can't be canceled when perform 100%
	{
		ClearKeyHandler(KEY_RSK, KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	}	
}

void jmms_SendProgressEndKeyHandler (void)
{   
  	g_pstJMmsAppInfo->bEndKeyPressed = E_TRUE;
	show_ExitProgress_screen();
}
 
void show_ExitProgress_screen (void)
{
	ST_REQUEST_LIST	*pstHeadRequestNode ;
	pstHeadRequestNode = jMMS_GetReqRespNodeByState (E_JMMS_NODE_SEND) ;
	if (JC_NULL == pstHeadRequestNode)
	{
		pstHeadRequestNode = jMMS_GetReqRespNodeByState (E_JMMS_NODE_RETRIEVE) ;
	}
	if (JC_NULL != pstHeadRequestNode)
	{
		mmi_trace(1,"JDD_LOG: show_ExitProgress_screen >> uiReferenceID [%d], Node State [%d]",
			pstHeadRequestNode->uiReferenceID, pstHeadRequestNode->eNodeState) ;
		if (0 != pstHeadRequestNode->uiReferenceID &&
				E_FALSE == pstHeadRequestNode->bIsToRemove)
		{
			jdi_CommunicationStopReq (g_pstJMmsAppInfo->hCommunicator, pstHeadRequestNode->uiReferenceID) ;
			pstHeadRequestNode->eStatus = E_HTTP_GONE ;
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_GLOBAL_ABORT), IMG_GLOBAL_WARNING,\
							1,	2000, 1) ;
			jmms_CallbackGoBackHistory();
			jMMS_RemoveReqRespNodeByMsgID (pstHeadRequestNode->uiMsgID) ;
		}
	}
	else
	{
		mmi_trace(1,"JDD_LOG: show_ExitProgress_screen >> No Send or Retrieve node found") ;
	}
}
void jmms_abort_auto_downloading(UINT8 nSimID)
{
	ST_REQUEST_LIST	*pstHeadRequestNode = g_pstJMmsAppInfo->pReqRespList;
	mmi_trace(g_sw_MMS, "JDD_LOG: jmms_abort_auto_downloading, nSimID is:%d", nSimID);
	if(JC_NULL != pstHeadRequestNode)
	{
		mmi_trace(g_sw_MMS, "JDD_LOG: jmms_abort_auto_downloading, uiReferenceID [%d], Node State [%d], nSimID is: [%d]",
				pstHeadRequestNode->uiReferenceID, pstHeadRequestNode->eNodeState, pstHeadRequestNode->uiSimID);
		if(0!= pstHeadRequestNode->uiReferenceID && pstHeadRequestNode->uiSimID == nSimID
			&& pstHeadRequestNode->bIsToRemove == E_FALSE)
		{
			jdi_CommunicationStopReq (g_pstJMmsAppInfo->hCommunicator, pstHeadRequestNode->uiReferenceID) ;
			pstHeadRequestNode->eStatus = E_HTTP_GONE ;
		}
		// remove all the nodes with SIMID = nSimID except the head node
		jMMS_RemAllReqRespNodeBySIM(nSimID);
	}
	
}
BOOL isInMMSApp(void)
{
	U16 screen_id = SCR_ID_MMS_MAIN_SCREEN;
	mmi_trace(TRUE,"JWAP_LOG: isInMMSApp  ");

	for(;screen_id < SCR_ID_MMS_END;screen_id++)
	{
		if(IsScreenPresent(screen_id))
			return TRUE;
	}
	screen_id = GetActiveScreenId();
	if((screen_id >=SCR_ID_MMS_MAIN_SCREEN )&&(screen_id < SCR_ID_MMS_END))
	{
		return TRUE;
	}

	return FALSE ;
}

void jmms_CheckHistoryData(void)
{
	U16 s_top,e_end,bingo;

	DeleteScreenIfPresent(SCR_CONFIRM_SCREEN);
	s_top = GetCurrScrnId();
	
	switch (g_pstJMmsAppInfo->eCurrentFolderID)
	{
		case E_JMMS_INBOX_FOLDER:
			e_end = (SCR_ID_MMS_INBOX_SCREEN) ;
			break ;
		case E_JMMS_OUTBOX_FOLDER:
		{
			JC_UINT32 uiMsgListCount = jMMS_GetTotalNumberOfMsgsInFolder (E_JMMS_OUTBOX_FOLDER) ;
			if (0 != uiMsgListCount)
			{
				e_end = (SCR_ID_MMS_OUTBOX_SCREEN) ;
			}
			else
			{
				e_end = (SCR_ID_MMS_MAIN_SCREEN) ;
			}
		}
			break ;
		case E_JMMS_SENT_FOLDER:
			e_end = (SCR_ID_MMS_SENT_SCREEN) ;
			break ;
		case E_JMMS_DRAFT_FOLDER:
			e_end = (SCR_ID_MMS_DRAFT_SCREEN) ;
			break ;
		case E_JMMS_NONE_FOLDER:
			e_end = (SCR_ID_MMS_MAIN_SCREEN) ;
			break ;
		default:
			e_end = IDLE_SCREEN_ID;
			MMI_ASSERT(0);
			break ;	
	}

	mmi_trace(1,"jdd_log: CheckHistoryData  start = %d , end = %d ", SCR_ID_MMS_MAIN_SCREEN,SCR_ID_MMS_END );
	mmi_trace(1,"jdd_log: CheckHistoryData  s_top = %d  ",  s_top);

	if(s_top < SCR_ID_MMS_MAIN_SCREEN||s_top > SCR_ID_MMS_END)
	{
		GetPreviousScrnIdOf(s_top,&bingo);
		while((bingo < SCR_ID_MMS_MAIN_SCREEN)||(bingo > SCR_ID_MMS_END))
		{
			mmi_trace(1,"jmms_CheckHistoryData  bingo = %d  ",  bingo);

			if(!GetPreviousScrnIdOf(bingo,&bingo))
				break;
		}
		if(bingo !=  e_end )
		{
			GetNextScrnIdOf(e_end,  &e_end);
			DeleteBetweenScreen(e_end , bingo);
		}
	}
	/*
	else if(g_pstJMmsAppInfo->bLaunchedMMSFromOtherApp)
	{
		e_end = SCR_ID_MMS_ADDRESS_SCREEN;
		DeleteUptoScrID(e_end) ;
	}*/	
	else
	{
		if(IsScreenPresent(e_end))         
        { 
        	DeleteUptoScrID(e_end) ; 
        } 
        else 
        { 
        	mmi_trace(1,"jdd_log: jmms_CheckHistoryData  here");
            DeleteNHistory(1); 
            while((GetCurrScrnId() >= SCR_ID_MMS_MAIN_SCREEN)&&(GetCurrScrnId() <= SCR_ID_MMS_END )) 
            { 
                DeleteNHistory(1); 
            }
			GoBackHistory();
        }
	}

}

void jmms_CallbackGoBackHistory()
{
	jmms_MMIComposeDeInit () ;
	mmi_trace(1, "JDD_LOG: go to folder [%d]",g_pstJMmsAppInfo->eCurrentFolderID);
	if(g_pstJMmsAppInfo->bEndKeyPressed == E_TRUE)
	{
		mmi_trace(1,"JDD_LOG: SetKeyHandler has called");
		g_pstJMmsAppInfo->bEndKeyPressed = E_FALSE;
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}
	jmms_CheckHistoryData();
	//g_pstJMmsAppInfo->bLaunchedMMSFromOtherApp = E_FALSE;
	
}

void jMms_SendMsgEntryHandler ()
{
	if (jMMS_IsReqNodeUnderProgress())
	{
		DisplayPopup((PU8)GetString(STR_ID_MMS_WAIT_AND_RETRY), IMG_GLOBAL_WARNING, 1,	2000, 1) ;
		return;
	}
	
	//jMms_ComposeAutoSignature();
	//get updated to,cc,bcc from global array
	jMms_addrGetValueFromList();
	jMms_SendMsgHandler (E_TRUE) ;
}

JC_BOOLEAN bCheckAutoSignature = E_TRUE;
void jMMS_Send_Without_Signature_Check(void)
{
	bCheckAutoSignature = E_FALSE;
	jMms_SendMsgEntryHandler();
}
void JMMS_Goback_To_Compose(void)
{
	GoBackToHistory(SCR_ID_MMS_ADDRESS_SCREEN);
}
void jMms_SendMsgHandler (JC_BOOLEAN bIsEdit)
{
	JC_RETCODE rCode ;
	JC_UINT32 uiModuleNum = 0;
	JC_UINT8 ucTotalPages;
	if(jc_tcslen(g_pstJMmsAppInfo->stMmsHeader.pTo) || jc_tcslen(g_pstJMmsAppInfo->stMmsHeader.pCc) ||
		jc_tcslen(g_pstJMmsAppInfo->stMmsHeader.pBcc))
	{
		rCode = jMms_ComposeAutoSignature();
		if(JC_ERR_MSG_MAX_REACHED == rCode && bCheckAutoSignature)
		{
			jMMS_GetUserConfirmation (GetString(STR_ID_MMS_NO_SAPCE_FOR_SIGNATURE), jMMS_Send_Without_Signature_Check, JMMS_Goback_To_Compose,jMms_SendMsgEntryHandler) ;
			return;
		}		
		bCheckAutoSignature = E_TRUE;
		jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucTotalPages) ;	
		if (0 == ucTotalPages)
		{//avoid no page mms
			jmms_MMIAddPage();
		}
		mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
		rCode = jmms_SendMms (bIsEdit) ;
		if (JC_OK != rCode)
		{
			jMMS_ErrorHandler (rCode, NULL) ;
			uiModuleNum = (-rCode) & 0x00FFFF00 ;
			if(FILE_MODULE == uiModuleNum && JC_ERR_FILE_NOT_FOUND != rCode)
			{
				DeleteUptoScrID(SCR_ID_MMS_ADDRESS_SCREEN);
			}
			else
			{
				jmms_CallbackGoBackHistory();
			}
		}
		jmms_UpdateMmsIcon();
	}
	else
	{
		DisplayPopup(GetString (STR_ID_MMS_ADD_ADDRESS), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		DeleteUptoScrID(SCR_ID_MMS_ADDRESS_SCREEN);
	}
}

void jMms_saveMsgHandler (void)
{
	JC_RETCODE			rCode = JC_OK;
	JC_UINT32			uiMessageID = 0 ;
	CONTENT_DATA		stEncapsulatedContent = { 0, } ;
	MESSAGE_HEADERS*	pMsgHeaders = JC_NULL ;
	JC_UINT8 ucTotalPages;

	/* TODO: Validation -
		1. Space availability to store the message,
		2. Maximum number of messages saved */
	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
	
	jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucTotalPages) ;
	
	if (0 == ucTotalPages)
	{//avoid no page mms
		jmms_MMIAddPage();
	}
	
	if (JC_NULL != g_pstJMmsAppInfo->hMsgStore)
	{
		jMms_addrGetValueFromList();
		jmms_UpdateMessageHeaders();
		rCode = jdi_MsgGetEncapsulatedBuffer (g_pstJMmsAppInfo->hCurrMsg, &stEncapsulatedContent) ;
		mmi_trace (1, "JDD_LOG: jMms_saveMsgHandler >> jdi_MsgGetEncapsulatedBuffer: %d @@@", rCode) ;
		if (JC_OK == rCode)
		{
			rCode = jdi_MsgGetMessageHeaders (g_pstJMmsAppInfo->hCurrMsg, &pMsgHeaders) ;
			if (JC_OK == rCode /*&& JC_NULL != pMsgHeaders*/)
			{
				rCode = jMMS_SaveOrUpdateMsg (E_JMMS_DRAFT_FOLDER, &uiMessageID, &stEncapsulatedContent, pMsgHeaders, 0) ;
			}			
			if (E_FALSE == stEncapsulatedContent.bIsStream)
			{
				jdd_MemFree (stEncapsulatedContent.pBuffer) ;
			}
			else
			{
				jdd_MemFree (stEncapsulatedContent.pFileName) ;
			}
		}
	}
	if (JC_OK == rCode)
	{

		jmms_UpdateMmsIcon();
		DisplayPopup (GetString (STR_GLOBAL_SAVED),\
										IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
		jmms_CallbackGoBackHistory();
	
		//DisplayPopup((PU8)GetString (STR_GLOBAL_SAVED), IMG_GLOBAL_ACTIVATED, 1, 2000, 13) ;
	}
	else
	{
		if (FILE_MODULE == ((-rCode) & 0x00FFFF00) && JC_ERR_FILE_NOT_FOUND != rCode)
		{
			DisplayPopup (GetString (STR_ID_MMS_FILE_SYSTEM_FULL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			//jmms_CallbackGoBackHistory();
			DeleteUptoScrID(SCR_ID_MMS_ADDRESS_SCREEN);
		}
		else
		{
			DisplayPopup (GetString (STR_ID_MMS_INF_FAIL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			jmms_CallbackGoBackHistory();
		}
	}	
	//DeleteUptoScrID (SCR_ID_MMS_MAIN_SCREEN) ;
}

void jMms_mainMenu_highlight_handler (int iSelectIndex) 
{	
	mmi_trace (1, "@@@ jMms_mainMenu_highlight_handler Index: %d @@@", iSelectIndex) ;
	
	switch (iSelectIndex)
	{
		case 0:
			SetLeftSoftkeyFunction (jMms_Write_Msg_entry_handler, KEY_EVENT_UP) ;
			break ;
		case 1:
			SetLeftSoftkeyFunction (jMms_inbox_entry_handler, KEY_EVENT_UP) ;
			break ;
		case 2:
			SetLeftSoftkeyFunction (jMms_sent_entry_handler, KEY_EVENT_UP) ;
			break ;
		case 3:
			SetLeftSoftkeyFunction (jMms_outbox_entry_handler, KEY_EVENT_UP) ;
			break ;
		case 4:
			SetLeftSoftkeyFunction (jMms_draft_entry_handler, KEY_EVENT_UP) ;
			break ;
		case 5:			
			SetLeftSoftkeyFunction (jMms_settings_entry_handler, KEY_EVENT_UP) ;
		break ;		
		case 6:
			SetLeftSoftkeyFunction(jMms_Aboutus_entry_handler, KEY_EVENT_UP);
			break;
		default:			
			break ;		
	}

}
void jmms_ResetGlobalVal()
{
	mmi_trace(g_sw_MMS,"JDD_LOG: jmms_ResetGlobalVal");
	if (g_pstJMmsAppInfo->hCurrMsg)
	{
		jdi_MsgDeInit(g_pstJMmsAppInfo->hCurrMsg) ;
		g_pstJMmsAppInfo->hCurrMsg = 0 ;
	}
	if(g_pstJMmsAppInfo)
	{	
		if(g_pstJMmsAppInfo->pcInputBuff)
		{
			jc_memset(g_pstJMmsAppInfo->pcInputBuff,0,jMMS_MAX_INPUT_SIZE) ;
		}
		
		//jc_memset(g_pstJMmsAppInfo->stMMSCompose,0,sizeof(JMMS_COMPOSE) *10 ) ;//TO FIX
		g_pstJMmsAppInfo->bIsAutoSignatureAdded = E_FALSE ;
		jc_memset(&g_pstJMmsAppInfo->stMmsHeader,0,sizeof(JMMS_HEADER) ) ;
		g_pstJMmsAppInfo->ActivePageIndex = 0;
		//jc_memset(&g_pstJMmsAppInfo->stMMSSetting,0,sizeof(JMMS_SETTINGS)) ;
		jc_memset ( g_pstJMmsAppInfo->ActivePageMediaIds , 0 ,\
					sizeof (g_pstJMmsAppInfo->ActivePageMediaIds) );  		         
		
		jc_memset(g_pstJMmsAppInfo->pActiveTextBuffer,0,sizeof(JC_CHAR)*(JMMS_MAX_TEXT_PER_SLIDE+1));
		//jc_memset(g_pstAddrInfo,0,sizeof(ST_JMMS_ADDR_INFO));
		//jdd_MemFree(g_pstAddrInfo);
		//g_pstAddrInfo = NULL;
		g_pstJMmsAppInfo->eViewState = E_JMMS_VIEW_NONE;
		g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_NONE ;
	}	
}

extern  void jddmem_DisableMemLeakFinder (void);
void jmms_MainMenuExitHandler()
{
	JC_RETCODE rCode = JC_OK;
	jMms_DestroyMmsViewWindow();
	jMMS_SetMMSAppStatus(E_FALSE);
	if (JC_NULL == g_pstJMmsAppInfo->pReqRespList && g_pstJMmsAppInfo->hMsgStore)
	{
		rCode = jdi_StoreDeinit (g_pstJMmsAppInfo->hMsgStore) ;
		g_pstJMmsAppInfo->hMsgStore = JC_NULL ;
		mmi_trace (1, "JDD_LOG: jmms_MainMenuExitHandler jdi_StoreDeinit[%X]", -rCode) ;
	}	
}

void jmms_EndKeyExitHandler()
{
	JC_RETCODE rCode = JC_OK;
	mmi_trace(1,"Exit key has been called");
//	jMMS_Setting_deInitProfiles ();
	if (E_JMMS_PREVIEW_NONE != g_pstJMmsAppInfo->ePreviewState) 
	{
		rCode = jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg,E_VIEW_CLOSE,JC_NULL);
		mmi_trace(1,"JDD_LOG jmms_EndKeyExitHandler rcode [%d],state [%d]", \
						rCode, g_pstJMmsAppInfo->ePreviewState );
	}
	jmms_MMIComposeDeInit();
	jMms_DestroyMmsViewWindow ();
	jmms_ResetGlobalVal();
	jMMS_SetMMSAppStatus(E_FALSE);
	if (JC_NULL == g_pstJMmsAppInfo->pReqRespList && g_pstJMmsAppInfo->hMsgStore)
	{
		rCode = jdi_StoreDeinit (g_pstJMmsAppInfo->hMsgStore) ;
		g_pstJMmsAppInfo->hMsgStore = JC_NULL ;
		mmi_trace (1, "JDD_LOG: jmms_ExitHandler::jdi_StoreDeinit:%X", -rCode) ;
	}
	//DisplayIdleScreen();
}


/**************************************************************

	FUNCTION NAME		: jMms_inbox_hint_handler

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void jMms_inbox_hint_handler(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

    JC_UINT32 UnreadMsg;
    JC_UINT32 TotMsg;
    
	memset( (PS8)hintData[index], 0, sizeof(hintData[index]));

     jmms_GetReadMsgStatus(&UnreadMsg,&TotMsg,E_JMMS_INBOX_FOLDER);
		sprintf( (S8 *)value, "%d ", TotMsg );
		AnsiiToUnicodeString( (PS8)hintData[index], (PS8)value);
		pfnUnicodeStrncat( (S8 *)hintData[index],
		                   (S8 *)GetString(STR_ID_MMS_TEXT),16);
	
	return;
}

/**************************************************************

	FUNCTION NAME		: jMms_sent_highlight_handler

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void jMms_sent_highlight_handler(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];
    JC_UINT32 UnreadMsg;
    JC_UINT32 TotMsg;
	memset( (PS8)hintData[index], 0, sizeof(hintData[index]));
     jmms_GetReadMsgStatus(&UnreadMsg,&TotMsg,E_JMMS_SENT_FOLDER);

		sprintf( (S8 *)value, "%d ", TotMsg );
		AnsiiToUnicodeString( (PS8)hintData[index], (PS8)value);
		pfnUnicodeStrncat( (S8 *)hintData[index],
		                   (S8 *)GetString(STR_ID_MMS_TEXT), 16);
	
	return;
}

void jMms_outbox_highlight_handler(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];
       JC_UINT32 UnreadMsg;
       JC_UINT32 TotMsg;
	memset( (PS8)hintData[index], 0, sizeof(hintData[index]));
       jmms_GetReadMsgStatus(&UnreadMsg,&TotMsg,E_JMMS_OUTBOX_FOLDER);
		sprintf( (S8 *)value, "%d ", TotMsg );
		AnsiiToUnicodeString( (PS8)hintData[index], (PS8)value);
		pfnUnicodeStrncat( (S8 *)hintData[index],
		                   (S8 *)GetString(STR_ID_MMS_TEXT), 16);
	
	return;
}

/**************************************************************

	FUNCTION NAME		: jMms_draft_highlight_handler

  	PURPOSE				: 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void jMms_draft_highlight_handler(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];
    JC_UINT32 UnreadMsg;
    JC_UINT32 TotMsg;
    
    memset( (PS8)hintData[index], 0, sizeof(hintData[index]));

     jmms_GetReadMsgStatus(&UnreadMsg,&TotMsg,E_JMMS_DRAFT_FOLDER);
    sprintf( (S8 *)value, "%d ", TotMsg );
    AnsiiToUnicodeString( (PS8)hintData[index], (PS8)value);
    pfnUnicodeStrncat( (S8 *)hintData[index],
                       (S8 *)GetString(STR_ID_MMS_TEXT), 16);

	return;
}

void jMms_mainMenu_entry_handler (void)
{
	U8* guiBuffer; 
	U16 item_texts[] = {STR_ID_MMS_NEW_MMS,
						STR_ID_MMS_INBOX,
						STR_ID_MMS_SENT,
						STR_ID_MMS_OUTBOX,
						STR_ID_MMS_DRAFT,
						STR_ID_MMS_SETTINGS,
						STR_ID_MMS_ABOUT
						} ;
	//add code for getting vds0 diskinfo
	INT32 Ret;
	FS_INFO psFSInfo;
	U8* PopUpList[MAX_SUB_MENUS];

	Ret = FS_GetFSInfo(CS_FS_MMS_IDENTIFIER,&psFSInfo);

	mmi_trace(1, "*********The result is %d!************\n",Ret);

	if(Ret == ERR_SUCCESS)
	{
	    mmi_trace(1, "************The TOTAL SIZE is %d !************",(UINT32)psFSInfo.iTotalSize);
	    mmi_trace(1, "************The USED SIZE is %d !************",(UINT32)psFSInfo.iUsedSize);
	}
	else
	{
	    mmi_trace(1, "************FAIL to get the information of FS !************");
	}
    //end disk info
	jMMS_SetMMSAppStatus(E_TRUE);	
	jmms_ResetGlobalVal();	
    
	SetHiliteHandler(MMS_EXIT_COMPOSE, jMms_Highlight_Exit_Compose);
	SetDelScrnIDCallbackHandler(SCR_ID_MMS_ADDRESS_SCREEN,NULL); 
	
	EntryNewScreen (SCR_ID_MMS_MAIN_SCREEN, NULL, jMms_mainMenu_entry_handler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_MAIN_SCREEN) ;	
	SetParentHandler(MMS_MAIN_SCREEN_MENUID);
	RegisterHighlightHandler (jMms_mainMenu_highlight_handler) ;
	ConstructHintsList(MMS_MAIN_SCREEN_MENUID, PopUpList);
	
	ShowCategory52Screen(STR_ID_MMS_TEXT, IMG_SMS_ENTRY_SCRN_CAPTION, 
						STR_GLOBAL_OK, 0, 
						STR_GLOBAL_BACK, 0,
						7, item_texts, (PU16)gIndexIconsImageList,
						PopUpList ,
						1,0, guiBuffer);

	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);		
	SetDelScrnIDCallbackHandler(SCR_ID_MMS_MAIN_SCREEN,jmms_ApplicationExitHandler); 
}

void jmms_MMSEntryHandler()
{
	JC_RETCODE rCode ;
	
	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(jmms_MMSEntryHandler);

		return;
	}
		
	if (JC_NULL == g_pstJMmsAppInfo->hMsgStore)
	{
		rCode = jdi_StoreInit (&g_pstJMmsAppInfo->hMsgStore, 0) ;
		mmi_trace (1, "JDD_LOG: jmms_MMSEntryHandler::jdi_StoreInit [%X]", -rCode) ;
	}
	
	jMms_createMmsViewWindow();
	jMms_mainMenu_entry_handler();
}
extern void jddmem_ShowMemLeakFinder();
void jMms_highlight_handler (void)
{
	#ifdef ENABLE_MEM_LEAK_FINDER
	jddmem_ShowMemLeakFinder ();// for wap
	#endif
	SetLeftSoftkeyFunction (jmms_MMSEntryHandler ,KEY_EVENT_UP) ;
}

void jmms_HandleComposeSlideKeyEvents(U16 keyCode)
{
	JC_UINT8 uiTotalPages;
	JC_RETCODE rCode = JC_OK;
	EVENT_SCROLL stScrollData = {0,} ;
	stScrollData.uiScrollOffset = JMMS_SCROLL_PMMI_OFFSET ;

	switch (keyCode)
	{
		case KEY_UP_ARROW:
			{
				stScrollData.eDirection = E_DIRECTION_UP ;
				jdi_MsgViewHandleEvent (g_pstJMmsAppInfo->hCurrMsg, E_VIEW_SCROLL, 
							(void*)(&stScrollData));
			}
			break ;
		case KEY_DOWN_ARROW:
			{
				stScrollData.eDirection = E_DIRECTION_DOWN ;
				jdi_MsgViewHandleEvent (g_pstJMmsAppInfo->hCurrMsg, E_VIEW_SCROLL, 
							(void*)(&stScrollData));
			}
			break ;
			
		case KEY_LEFT_ARROW:
			{			
				jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &uiTotalPages) ;
				if(g_pstJMmsAppInfo->ActivePageIndex == 1)
				{
					//dont decrement g_pstJMmsAppInfo->ActivePageIndex 
					break;
				}
				else
				{				
					g_pstJMmsAppInfo->ActivePageIndex -= 1;
					jmms_PrepareCurrSlideForDisplay() ;
				}
				rCode = jmms_MMIViewMmsPlaySlide (g_pstJMmsAppInfo->ActivePageIndex, E_FALSE);			
			}			 
			break ;		
		case KEY_RIGHT_ARROW:
			{
				jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &uiTotalPages) ;		
				
				if(uiTotalPages == g_pstJMmsAppInfo->ActivePageIndex)
				{				
					//dont increment g_pstJMmsAppInfo->ActivePageIndex
					break;
				}
				else
				{				
					g_pstJMmsAppInfo->ActivePageIndex += 1;
					jmms_PrepareCurrSlideForDisplay() ;
				}
				rCode = jmms_MMIViewMmsPlaySlide (g_pstJMmsAppInfo->ActivePageIndex, E_FALSE);			
			}
			break ;
		default:		
			break ;
	}		
	return ;
}
#ifdef __MMI_TOUCH_SCREEN__

JC_INT32 gucPrevPenY = -1;

void jMMS_Generic_Pen_Handler (mmi_keypads_enum keyCode, mmi_pen_point_struct *pstPenCooridinates)
{
	JC_RETCODE rCode = JC_OK;
	EVENT_SCROLL stScrollData = {0,} ;
	DEVICE_CHARACTERISTICS 	stDeviceChar ={0,} ;
	JC_RECT stScrollCarRect={0,},stScrollBarRect={0,} ;
	
	jdd_MMIGetDeviceCharacteristics (&stDeviceChar) ;
	
	jdd_GetScrollInfoForWindow (g_pstJMmsAppInfo->uiViewWindow, &stScrollCarRect,&stScrollBarRect);
	mmi_trace(1, "pen co-ordiantes x[%d] y[%d] carrect x[%d] y [%d] w[%d] h[%d] barrect x[%d] y [%d] w[%d] h[%d]",
			pstPenCooridinates->x,pstPenCooridinates->y,
			stScrollCarRect.iLeft,stScrollCarRect.iTop, stScrollCarRect.uiWidth, stScrollCarRect.uiHeight,
			stScrollBarRect.iLeft, stScrollBarRect.iTop, stScrollBarRect.uiWidth, stScrollBarRect.uiHeight);
			
	
	if (KEY_STAR == keyCode || KEY_POUND == keyCode)
	{
		
	     if ( pstPenCooridinates->x >= stScrollBarRect.iLeft &&
		pstPenCooridinates->x <= stScrollBarRect.iLeft+ stScrollBarRect.uiWidth &&
		pstPenCooridinates->y >= stScrollBarRect.iTop &&
		pstPenCooridinates->y <= stScrollBarRect.iTop + stScrollBarRect.uiHeight)
		{
				
				//check if the pendo-ordinate is in the region of scroll car
				if ( pstPenCooridinates->y >= stScrollCarRect.iTop &&
				pstPenCooridinates->y <= stScrollCarRect.iTop + stScrollCarRect.uiHeight)
				{

				   if( KEY_POUND == keyCode )
				   	{
					  stScrollData.bStylusEvent = E_FALSE ;
					  stScrollData.bNewPosition = E_FALSE ;
					  //stScrollData.uiScrollOffset = pstPenCooridinates->y - stDeviceChar.uiWindowY ;
					  if(gucPrevPenY != -1 )
					  {
					     mmi_trace(1, "pstPenCooridinates->y[%d],gucPrevPenY[%d]", 
						 	pstPenCooridinates->y,gucPrevPenY ); 
					     if ( pstPenCooridinates->y > gucPrevPenY )
					     {
						 	stScrollData.uiScrollOffset = (pstPenCooridinates->y -  gucPrevPenY)*jdd_GetScrollOffsetFactor(g_pstJMmsAppInfo->uiViewWindow);
							stScrollData.eDirection = E_DIRECTION_DOWN ;
					     }
						 else
						 {
							 stScrollData.uiScrollOffset = (gucPrevPenY - pstPenCooridinates->y)*jdd_GetScrollOffsetFactor(g_pstJMmsAppInfo->uiViewWindow);
							 stScrollData.eDirection = E_DIRECTION_UP ;
						 }


	 					rCode = jdi_MsgViewHandleEvent (g_pstJMmsAppInfo->hCurrMsg, E_VIEW_SCROLL, (void*)(&stScrollData));
					  }
					  else
					  {
					  	mmi_trace(1, "gucPrevPenY == -1 . ignored");
						
					  }
					  gucPrevPenY = pstPenCooridinates->y;
						
				   }
				   else
				   	{
				  		mmi_trace(1, "pen down on the scroll car . ignored"); 	
						gucPrevPenY = -1;
				   	}
				}
				
				
				else if(pstPenCooridinates->y > stScrollCarRect.iTop)
				{
				    mmi_trace(1,"pen event : scroll down");
					stScrollData.eDirection = E_DIRECTION_DOWN ;
					stScrollData.uiScrollOffset = JMMS_SCROLL_PMMI_OFFSET ;
					 stScrollData.bStylusEvent = E_FALSE ;
				     stScrollData.bNewPosition = E_FALSE ;
				 	  //stScrollData.uiScrollOffset = jdd_GetScrollOffset(g_pstJMmsAppInfo->uiViewWindow, 
					  //					pstPenCooridinates->y);
					  
					
					rCode = jdi_MsgViewHandleEvent (g_pstJMmsAppInfo->hCurrMsg, E_VIEW_SCROLL, 
										(void*)(&stScrollData));	  
					  gucPrevPenY = -1;
				}
				else if(pstPenCooridinates->y < stScrollCarRect.iTop)
				{
				    mmi_trace(1,"pen event : scroll up");
					stScrollData.eDirection = E_DIRECTION_UP ;
					stScrollData.uiScrollOffset = JMMS_SCROLL_PMMI_OFFSET ;
					stScrollData.bStylusEvent = E_FALSE ;
				     stScrollData.bNewPosition = E_FALSE ;
				 	  //stScrollData.uiScrollOffset = stScrollData.uiScrollOffset = jdd_GetScrollOffset(g_pstJMmsAppInfo->uiViewWindow, 
					  //					pstPenCooridinates->y);
					
					rCode = jdi_MsgViewHandleEvent (g_pstJMmsAppInfo->hCurrMsg, E_VIEW_SCROLL, 
										(void*)(&stScrollData));	
					  gucPrevPenY = -1;
				}
				else
				{
					gucPrevPenY = -1;
				}		        
        }
		else
		{
			mmi_trace(1, "pen event ignored : out of scroll bar region");

			if (KEY_STAR == keyCode)
			{
				wgui_general_SK_pen_down_hdlr(*pstPenCooridinates);
			}
			if (KEY_ENTER == keyCode)
			{
				wgui_general_SK_pen_up_hdlr(*pstPenCooridinates);
			}
		}

	}
}
#endif
void jmms_ComposeSlideLeftKeyHandler ()
{
	jmms_HandleComposeSlideKeyEvents (KEY_LEFT_ARROW) ;
}

void jmms_ComposeSlideRightKeyHandler()
{
	jmms_HandleComposeSlideKeyEvents (KEY_RIGHT_ARROW) ;
}
void jmms_ComposeSlideUpKeyHandler()
{
	jmms_HandleComposeSlideKeyEvents (KEY_UP_ARROW) ;
}
void jmms_ComposeSlideDownKeyHandler()
{
	jmms_HandleComposeSlideKeyEvents (KEY_DOWN_ARROW) ;
}

#ifdef __MMI_TOUCH_SCREEN__
void jMMS_PenDownHandler (mmi_pen_point_struct stPenCoordinates)
{
	jMMS_Generic_Pen_Handler (KEY_STAR, &stPenCoordinates) ;
	
}

void jMMS_PenMoveHandler (mmi_pen_point_struct stPenCoordinates)
{
	jMMS_Generic_Pen_Handler (KEY_POUND, &stPenCoordinates) ;
}
#endif

void jmms_ComposeSlideRegisterKeyHandler()
{	
	SetKeyHandler(jmms_ComposeSlideLeftKeyHandler, KEY_LEFT_ARROW, KEY_EVENT_DOWN) ;
	SetKeyHandler(jmms_ComposeSlideRightKeyHandler, KEY_RIGHT_ARROW, KEY_EVENT_DOWN) ;
	SetKeyHandler(jmms_ComposeSlideUpKeyHandler, KEY_UP_ARROW, KEY_EVENT_DOWN) ;
	SetKeyHandler(jmms_ComposeSlideDownKeyHandler, KEY_DOWN_ARROW, KEY_EVENT_DOWN) ;	
	//SetKeyHandler(jmms_ComposeSlideEndKeyHandler, KEY_END, KEY_EVENT_DOWN) ;
#ifdef __MMI_TOUCH_SCREEN__
	mmi_wgui_register_pen_down_handler (jMMS_PenDownHandler) ;
	//mmi_wgui_register_pen_up_handler (jMMS_PenUpHandler) ;
	mmi_wgui_register_pen_move_handler (jMMS_PenMoveHandler) ;
#endif
}

/* Temporary Implementation for About Us Screen */
/* Need to find a suitable category screen for this */
static U8* jmms_aboutScreen [3] ;


void jMms_Aboutus_entry_handler ()
{	
	jmms_aboutScreen [0] = (U8 *)GetString(STR_ID_MMS_ABOUT_US_MMS_CLIENT);		
	jmms_aboutScreen [1] = (U8 *)GetString(STR_ID_MMS_ABOUT_US_VERSION) ;		
	jmms_aboutScreen [2] = (U8 *)GetString(STR_ID_MMS_ABOUT_US_BUILD) ;	
	EntryNewScreen(SCR_ID_MMS_ABOUT_US, NULL, jMms_Aboutus_entry_handler, NULL);
	ShowCategory6Screen(STR_ID_MMS_ABOUT, IMG_SMS_ENTRY_SCRN_CAPTION,
				          0, 0,
				          STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				          3,jmms_aboutScreen,NULL,0,NULL);

	SetLeftSoftkeyFunction (0,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;	
}

JC_UINT8 jmms_ApplicationExitHandler(void* p)
{
	mmi_trace(1,"JMMS_LOG: jmms_ApplicationExitHandler ");
	jmms_EndKeyExitHandler();

	return 0;
}

#endif //_JATAAYU_APP__

