#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_storeutils.c"
 /***************************************************************************
 *
 * File Name : jmms_storeutils.c
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
 * @file		jmms_storeutils.c
 * @ingroup		jMMS
 * @brief		Describes the Store Module API's.
 * @written 
 **/
 
 #ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

#include "jmms_inc.h"
#include "settingprofile.h"


#include "jmms_composedlg.h"
#include "jmms_composeutils.h"
#include "jmms_storedlg.h"
#include "jmms_addrdlg.h"

/***************************************************************************
* Macros
**************************************************************************/

//#define TEXT_CONTENT_TYPE			"text/plain"

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/
extern U8 gNeedNewMMSPopOnIdle;
 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

 /***************************************************************************
 * All Global Function Definitions 
**************************************************************************/
void jmms_PrepareComposeFromFolder()
{
	jMms_setMsgProperties () ;
	jMMS_configMsgSmil (g_pstJMmsAppInfo->hCurrMsg) ;
	jmms_MMISetViewCallback (g_pstJMmsAppInfo->hCurrMsg);
	jmms_MMISetComposeCallback (g_pstJMmsAppInfo->hCurrMsg);
}

void jmms_InboxReplyHandler()
{	
	JC_RETCODE 		rCode ;
		
	rCode = jmms_CreateMsgHandleForFolderMsg(E_JMMS_INBOX_FOLDER, g_pstJMmsAppInfo->uiHighlightedMsgId, 
													E_JMMS_REPLY);
	jmms_PrepareComposeFromFolder();
	if(JC_OK == rCode)
	{
		g_pstJMmsAppInfo->ActivePageIndex = 1;
		jmms_initaddrInfo();
		jMMS_GetMessageHeader() ; 
		jmms_ComposeMainScreenEntryHandler();										
	}
	
}

void jmms_InboxReplyAllHandler()
{
	JC_RETCODE rCode ;
		
	rCode = jmms_CreateMsgHandleForFolderMsg(E_JMMS_INBOX_FOLDER, g_pstJMmsAppInfo->uiHighlightedMsgId, 
													E_JMMS_REPLY_ALL);
	jmms_PrepareComposeFromFolder();
	if(JC_OK == rCode)
	{
		g_pstJMmsAppInfo->ActivePageIndex = 1;
		jmms_initaddrInfo();
		jMMS_GetMessageHeader() ; 
		jmms_ComposeMainScreenEntryHandler();										
	}
}

void jmms_InboxForwardHandler()
{
	JC_RETCODE rCode ;
		
	rCode = jmms_CreateMsgHandleForFolderMsg(E_JMMS_INBOX_FOLDER, g_pstJMmsAppInfo->uiHighlightedMsgId, 
													E_JMMS_FORWARD);
	jmms_PrepareComposeFromFolder();
	if(JC_OK == rCode)
	{
		g_pstJMmsAppInfo->ActivePageIndex = 1;
		jmms_initaddrInfo();
		jMMS_GetMessageHeader() ; 
		jmms_ComposeMainScreenEntryHandler();										
	}
}

void jmms_SentForwardHandler()
{
	JC_RETCODE rCode;
		
	rCode = jmms_CreateMsgHandleForFolderMsg(E_JMMS_SENT_FOLDER, g_pstJMmsAppInfo->uiHighlightedMsgId, 
													E_JMMS_FORWARD);
	jmms_PrepareComposeFromFolder();
	if(JC_OK == rCode)
	{
		g_pstJMmsAppInfo->ActivePageIndex = 1;
		jmms_initaddrInfo();
		jMMS_GetMessageHeader() ;
		jmms_ComposeMainScreenEntryHandler();										
	}
}

void jmms_SentEditHandler()
{
	JC_RETCODE rCode ;
		
	rCode = jmms_CreateMsgHandleForFolderMsg(E_JMMS_SENT_FOLDER, g_pstJMmsAppInfo->uiHighlightedMsgId, 
													E_JMMS_EDIT);
	jmms_PrepareComposeFromFolder();
	if(JC_OK == rCode)
	{
		g_pstJMmsAppInfo->ActivePageIndex = 1;
		jmms_initaddrInfo();
		jMMS_GetMessageHeader() ;
		jmms_ComposeMainScreenEntryHandler();										
	}
}

void jmms_OutboxEditHandler()
{
	JC_RETCODE 		rCode;

	rCode = jmms_CreateMsgHandleForFolderMsg(E_JMMS_OUTBOX_FOLDER, g_pstJMmsAppInfo->uiHighlightedMsgId, 
													E_JMMS_EDIT);
	jmms_PrepareComposeFromFolder();
	if(JC_OK == rCode)
	{
		g_pstJMmsAppInfo->ActivePageIndex = 1;
		jmms_initaddrInfo();
		jMMS_GetMessageHeader() ;

		jmms_ComposeMainScreenEntryHandler();										
	}
}

void jmms_OutboxForwardHandler()
{
	JC_RETCODE 		rCode;
	
	rCode = jmms_CreateMsgHandleForFolderMsg(E_JMMS_OUTBOX_FOLDER, g_pstJMmsAppInfo->uiHighlightedMsgId, 
													E_JMMS_FORWARD);
	jmms_PrepareComposeFromFolder();
	if(JC_OK == rCode)
	{
		g_pstJMmsAppInfo->ActivePageIndex = 1;
		jmms_initaddrInfo();
		jMMS_GetMessageHeader() ;
		jmms_ComposeMainScreenEntryHandler();										
	}
}

void jmms_DraftEditHandler()
{
	JC_RETCODE 		rCode;
	
	rCode = jmms_CreateMsgHandleForFolderMsg(E_JMMS_DRAFT_FOLDER, g_pstJMmsAppInfo->uiHighlightedMsgId, 
													E_JMMS_EDIT);
	jmms_PrepareComposeFromFolder();
	if(JC_OK == rCode)
	{
		g_pstJMmsAppInfo->ActivePageIndex = 1;
		jmms_initaddrInfo();
		jMMS_GetMessageHeader() ;

		jmms_ComposeMainScreenEntryHandler();										
	}
}

void jmms_GoBactToMainScreen()
{	
	MESSAGE_LIST 	*pMsglist = JC_NULL ;
	JC_UINT32		uiCount = 0 ;
	JC_RETCODE 		rCode ;
	RETRIEVE_FILTERS	retFilter ={0,};	

	rCode = jmms_SetActiveFolder (g_pstJMmsAppInfo->eCurrentFolderID) ;
	if (JC_OK == rCode)
	{
		retFilter.bIsBufRequired = E_FALSE;
		retFilter.eRetrieve = E_RETRIEVE_BY_INDEX;
		retFilter.uiStartIndex = 1;
		retFilter.uiNumMessage = 0;
		rCode = jmms_GetMsgList (g_pstJMmsAppInfo->hMsgStore, &pMsglist, &retFilter) ;
		if (JC_OK == rCode && pMsglist)
		{
			while (pMsglist)
			{			
				uiCount++ ;
				pMsglist = pMsglist->pNext ;
			}
		}
	}
	mmi_trace(TRUE,"JDD_LOG: Func: %s uiCount=%d", __FUNCTION__, uiCount);
	if (0 == uiCount)
	{
		DeleteUptoScrID(SCR_ID_MMS_MAIN_SCREEN);
	}
	else
	{
		DeleteNHistory(2);
	}
}

void jmms_GoBackToMainScreen()
{
	DeleteUptoScrID(SCR_ID_MMS_MAIN_SCREEN);
}


void jMMS_Store_DeleteMsg (void)
{
    JC_RETCODE rCode = JC_OK ;
	mmi_show_in_progress(STR_ID_MMS_DELETING, IMG_GLOBAL_DELETED);

	mmi_trace(1,"JDD_LOG: jMMS_Store_DeleteMsg[%d]",g_pstJMmsAppInfo->uiHighlightedMsgId) ;
	rCode = jMMS_DeleteMsg (g_pstJMmsAppInfo->eCurrentFolderID, g_pstJMmsAppInfo->uiHighlightedMsgId, E_TRUE) ;
	if (JC_OK == rCode)
	{
		jmms_UpdateMmsIcon () ;
	}

	if (JC_OK == rCode)
	{
		DisplayPopup(GetString (STR_ID_MMS_MESSAGE_DELETE_SUCCESS), IMG_GLOBAL_INFO,1, 2000, 1) ;
		jmms_GoBactToMainScreen();

	}
	else if(JC_ERR_GENERAL_FAILURE == rCode)
	{
		DisplayPopup(GetString (STR_ID_DELETE_FAIL_NODEBUSY), IMG_GLOBAL_INFO,1, 2000, 1) ;
		jmms_GoBackToMainScreen();
	}
	else
	{
		DisplayPopup(GetString (STR_ID_MMS_INF_FAIL), IMG_GLOBAL_INFO,1, 2000, 1) ;
		jmms_GoBactToMainScreen();

	}
}

void jMms_delete_handler()
{	
	jMMS_GetUserConfirmation(GetString(STR_ID_MMS_CNF_DELETE), jMMS_Store_DeleteMsg,jMms_GoBack_History,jMms_delete_handler) ;
}

void jMMS_GetUserConfirmation(JC_UINT8* pcMessage,FPOINTER fpLeft,FPOINTER fpRight,FPOINTER entryFun)
{	
	DisplayNewconfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO,
					(U16*)pcMessage, IMG_GLOBAL_QUESTION, WARNING_TONE,entryFun) ;
	if(fpLeft)
	{
		SetLeftSoftkeyFunction(fpLeft,KEY_EVENT_UP) ;
	}
	if(fpRight)
	{
		SetRightSoftkeyFunction(fpRight,KEY_EVENT_UP) ;
	}	
}

void jMms_deleteAll_Msg()
{
	JC_RETCODE rCode ;
	mmi_show_in_progress(STR_ID_MMS_DELETING, IMG_GLOBAL_DELETED);
	rCode = jMMS_DeleteMsg (g_pstJMmsAppInfo->eCurrentFolderID, 0, E_TRUE) ;
	if (JC_OK == rCode)
	{
		jmms_UpdateMmsIcon () ;
	}
	if (JC_OK == rCode)
	{
		DisplayPopup(GetString (STR_ID_MMS_MESSAGE_DELETE_SUCCESS), IMG_GLOBAL_INFO,1, 2000, 1) ;
		if(E_JMMS_INBOX_FOLDER == g_pstJMmsAppInfo->eCurrentFolderID)
			gNeedNewMMSPopOnIdle = E_FALSE;
		jmms_GoBackToMainScreen();
	}
	else if(JC_ERR_GENERAL_FAILURE == rCode)
    {
		DisplayPopup(GetString (STR_ID_DELETE_FAIL_NODEBUSY), IMG_GLOBAL_INFO,1, 2000, 1) ;
		jmms_GoBackToMainScreen();
    }
	else
	{
		DisplayPopup(GetString (STR_ID_MMS_INF_FAIL), IMG_GLOBAL_INFO,1, 2000, 1) ;
		jmms_GoBackToMainScreen();
	}

}
void jMms_GoBack_History()
{
	GoBackHistory();
}
void jMms_delete_all_handler()
{
	jMMS_GetUserConfirmation(GetString(STR_ID_MMS_CNF_DELETE_ALL), jMms_deleteAll_Msg,jMms_GoBack_History,jMms_delete_all_handler) ;
}

void get_highlited_id(int iSelectIndex)
{
	mmi_trace(1,"get_highlited_id [%d]",g_pstJMmsAppInfo->iFolderItemIndex );
	g_pstJMmsAppInfo->iFolderItemIndex = iSelectIndex;
}


void jMms_ReSend_handler()
{
	JC_RETCODE		rCode ;
	MESSAGE_LIST	*pstMsgDetail = JC_NULL ;

	if (jMMS_IsReqNodeUnderProgress())
	{
		DisplayPopup((PU8)GetString(STR_ID_MMS_WAIT_AND_RETRY), IMG_GLOBAL_WARNING, 1,	2000, 1) ;
		return;
	}
	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);

	rCode = jmms_GetMsgDetailsByID (g_pstJMmsAppInfo->hMsgStore, g_pstJMmsAppInfo->eCurrentFolderID,
		g_pstJMmsAppInfo->uiHighlightedMsgId, &pstMsgDetail) ;
	
	mmi_trace(1,"JDD_LOG: jMms_ReSend_handler >> jmms_GetMsgDetailsByIndex MSG Details is [%u], MSG ID is [%d] and rCode is [%X]",
		pstMsgDetail, pstMsgDetail->uiMessageID, rCode) ;

	if (JC_OK == rCode && pstMsgDetail && 0 != pstMsgDetail->uiMessageID)
	{
#ifdef __MMI_MULTI_SIM__
		pstMsgDetail->uiExtraBytes = g_pstJMmsAppInfo->send_simid ;
		pstMsgDetail->eUpdateFlag = E_STORE_MODIFY ;
		rCode = jdi_StoreUpdateMsgList (g_pstJMmsAppInfo->hMsgStore, E_FALSE) ;
		mmi_trace (1, "JDD_LOG: jMms_ReSend_handler >> jdi_StoreUpdateMsgList () rCode is [%X]", rCode) ;
#endif
		
		if (JC_OK == rCode)
		{
			/* Resend is limited for OUTBOX only */
			rCode = jMMS_ProcessCommunicatorFetchRequest (pstMsgDetail->uiMessageID, \
						E_JMMS_NODE_SEND, E_JMMS_MSG_SEND_FRM_OUTBOX) ;
		}
	}
	if (JC_OK != rCode)
	{
		jMMS_ErrorHandler (rCode, NULL) ;
		jmms_CallbackGoBackHistory();
	}
}

void jmms_SetReadFlag(MESSAGE_LIST*	pstMsgDetail)
{	
	if (pstMsgDetail->bReadFlag == E_FALSE )
	{	
		pstMsgDetail->bReadFlag =  E_TRUE;
		pstMsgDetail->eUpdateFlag = E_STORE_MODIFY;
		jdi_StoreUpdateMsgList  (g_pstJMmsAppInfo->hMsgStore,E_FALSE);
	}	
}

JC_RETCODE	jmms_CreateMsgHandleForFolderMsg(E_JMMS_FOLDER_ID eFolderID, JC_UINT32 uiMsgID, 
													E_JMMS_COMPOSE_STATE eComposeState)
{
    JC_RETCODE          eRet = JC_OK ;
    CONTENT_DATA	    ViewContent = {0, } ;
    MESSAGE_LIST		*pMessageList = JC_NULL;
    RETRIEVE_FILTERS	StoreRetFilter = {0, } ;
	MSG_HANDLE		    hTempMsg = JC_NULL ;

    eRet = jmms_SetActiveFolder (eFolderID) ;
	if (JC_OK == eRet)
	{
		StoreRetFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
		StoreRetFilter.uiStartIndex= 1 ;
		StoreRetFilter.uiNumMessage = 0 ;
		eRet = jmms_GetMsgList (g_pstJMmsAppInfo->hMsgStore, &pMessageList, &StoreRetFilter) ;
	}            
    if (eRet == JC_OK && pMessageList != JC_NULL) 
    {       
        jmms_MMISetMsgContentStream (eFolderID, uiMsgID,&ViewContent);
        eRet = jMMS_GetMsgFromStoreByMsgID (g_pstJMmsAppInfo->hMsgStore, eFolderID, uiMsgID, &ViewContent) ;
        if (eRet != JC_OK)
        {
            if (eRet == JC_ERR_STORE_FILE_CORRUPTED)
            {
            	jdi_StoreRestoreFolderInfo (g_pstJMmsAppInfo->hMsgStore) ;
            }
        }
        else
        {
            if (g_pstJMmsAppInfo->hCurrMsg != JC_NULL)
            {
            	jdi_MsgDeInit (g_pstJMmsAppInfo->hCurrMsg) ;
            	g_pstJMmsAppInfo->hCurrMsg = JC_NULL ;
            }

            //set Re, Fwd Info
            if(eComposeState == E_JMMS_REPLY || eComposeState == E_JMMS_REPLY_ALL
				|| eComposeState == E_JMMS_FORWARD)
            {                
            	eRet = jdi_MsgInit(&hTempMsg, JC_NULL) ;
            	if(eRet == JC_OK)
            	{
                    eRet = jdi_MsgSetMessagingType (hTempMsg,	E_TYPE_MMS);
                	jmms_MMISetComposeCallback(hTempMsg);                	
                }                
            }
            if (eRet == JC_OK)
            {
                //Get New Msghandle by ViewContent
	            switch(eComposeState)
	            {
	                case E_JMMS_REPLY:
	                	eRet = jdi_MsgReply(hTempMsg, &ViewContent, E_FALSE, &g_pstJMmsAppInfo->hCurrMsg) ;
	                    break;	
					case E_JMMS_REPLY_ALL:
	                	eRet = jdi_MsgReply(hTempMsg, &ViewContent, E_TRUE, &g_pstJMmsAppInfo->hCurrMsg) ;
	                    break;		
	                    
	                case E_JMMS_FORWARD:
	                    eRet = jdi_MsgForward(hTempMsg, &ViewContent, &g_pstJMmsAppInfo->hCurrMsg) ;
	                    break;

	                case E_JMMS_EDIT:
	                    eRet = jdi_MsgCompose(&g_pstJMmsAppInfo->hCurrMsg,E_TYPE_MMS, &ViewContent);                    
	                    break;                 
	            }
            }
        }
    }
    
    // Free Content resource
    jmms_FreeContentData (&ViewContent, eRet) ;
    if(g_pstJMmsAppInfo->hCurrMsg == JC_NULL)
    {
        eRet = jdi_MsgInit (&g_pstJMmsAppInfo->hCurrMsg, JC_NULL) ;
    }    
    if(hTempMsg)
    {
    	jdi_MsgDeInit(hTempMsg) ;
    	hTempMsg = JC_NULL ;
    }    
	return eRet;        
}


#endif //__JATAAYU_APP__

