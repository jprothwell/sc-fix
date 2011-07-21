 /***************************************************************************
 *
 * File Name : jwap_push.c
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
 * @file		jwap_push.c
 * @ingroup	jWAP
 * @brief	
 *
 **/

#define MODULE_NAME "JWAP"
#define FILE_NAME "jwap_push.c"	

#ifdef JATAAYU_SUPPORT
/***************************************************************************
* System Include Files
**************************************************************************/


/***************************************************************************
* User Include Files
**************************************************************************/

#include "jwap_inc.h"
#include "jwap_str_id.h"
#include "messagesmiscell.h"


#ifdef JWAP_PUSH_ENABLE

 /***************************************************************************
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					JWAP_APP
#else
#define __MODULE_ID__					0
#endif

#include "jwap_interface.h"
#include "jdi_browserengine.h"
#include "jdi_history.h"
#include "jdi_cutils_tcsstr.h"
#include "jwap_mmi_cb.h"
#include "jwap_push.h"

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/

ST_WAP_PUSH_INFO		*g_pstPush_info ;
static JC_INT32 		st_iSelectedMsgReservedId ;
static FuncPtr 			fpEndKeyDownHanler=NULL;

 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/
JC_RETCODE jWap_PushInitialise (void)
{
	JC_RETCODE		rCode = JC_OK ;
	JC_CHAR			*pmPushFileName ;
	if (JC_NULL == g_pstPush_info)
	{
		g_pstPush_info = (ST_WAP_PUSH_INFO *)jdd_MemAlloc (sizeof (ST_WAP_PUSH_INFO), 1) ;
		if (g_pstPush_info)
		{
			jdi_CUtilsCharToTchar (JC_NULL, CS_WAP_PUSH_FILE_NAME, &pmPushFileName) ;
			rCode = jdi_PushMsgInitialize (pmPushFileName, JWAP_MAX_PUSH_MSGS, JC_NULL, &g_pstPush_info->vPushHandle) ;
			mmi_trace (1, "JDD_LOG: jWap_PushInitialise >> jdi_PushMsgInitialize () returned [%x]", -rCode) ;
			jdd_MemFree (pmPushFileName) ;

			rCode = jdi_PushMsgGetList (g_pstPush_info->vPushHandle, &g_pstPush_info->pstPushInfo) ;
			mmi_trace (1, "JDD_LOG: jWap_PushInitialise >> jdi_PushMsgGetList () returned [%x]", -rCode) ;
			if (JC_ERR_PUSH_LIST_EMPTY == rCode)
			{
				rCode = JC_OK ;
			}
		}
		else
		{
			mmi_trace (1, "JDD_LOG: jWap_PushInitialise >> Memory Allocation failed for the Push Context") ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_PushInitialise >> Push Context is already initialised") ;
	}
	return rCode ;
}

JC_RETCODE jWap_PushDeInitialise (void)
{
	JC_RETCODE	rCode = JC_ERR_GENERAL_FAILURE ;

	if(g_pstPush_info && g_pstPush_info->vPushHandle)
	{
		rCode = jdi_PushMsgDeInitialize (g_pstPush_info->vPushHandle) ;
	}
	jdd_MemFree (g_pstPush_info) ;
	mmi_trace (1, "JDD_LOG: jWap_PushDeInitialise returned [%x]", -rCode) ;
	g_pstPush_info = JC_NULL ;	
	return rCode ;
}

void jmms_GoBackFromPushOnIdleScreen()
{
	mmi_trace(1,"JDD_LOG: push noti flag set to false %d", __LINE__);
	g_pstJWapAppInfo->bNeedNewPushPopOnIdle = E_FALSE;
	GoBackHistory();
}

void jmms_EndkeyHandlerNewPushMsg(void)
{
	g_pstJWapAppInfo->bNeedNewPushPopOnIdle = E_FALSE;
	mmi_trace(1,"JDD_LOG: push noti flag set to false %d", __LINE__);
	if(NULL != fpEndKeyDownHanler)
	{
		(*fpEndKeyDownHanler)();
	}	
}

void jmms_LaunchSIMsgFromIdleScreen()
{
    g_pstJWapAppInfo->bNeedNewPushPopOnIdle = E_FALSE;
	jWap_LaunchFromIdle (E_WAP_LAUNCH_PUSH_ITEM, g_pstJWapAppInfo->simid, JC_NULL) ;
}

BOOL jwap_NeedNewPushPopupOnIdle(void)
{
	mmi_trace (0, "JDD_LOG: NewPushPopupOnIdle[%d]",
			g_pstJWapAppInfo->bNeedNewPushPopOnIdle) ;
	return g_pstJWapAppInfo->bNeedNewPushPopOnIdle;
}

void jWap_HandlePushMsg (ST_COMM_PUSH_IND *pstPushData)
{
	JC_RETCODE		rCode ;
	ST_PUSH_INFO	stPushInfo = {0, } ;
	JC_INT32		iReservedID = 0 ;
	JC_UINT32		uiUnreadMsg ;
	rCode = jWap_PushInitialise () ;
	if (JC_OK == rCode)
	{
		jdi_PushMsgStatistics (g_pstPush_info->vPushHandle, &g_pstPush_info->uiNumPushMsgs, &uiUnreadMsg) ;
		mmi_trace (1, "JDD_LOG: jWap_HandlePushMsg >> jdi_PushMsgStatistics () returned Total and Unread as [%d %d]",
			g_pstPush_info->uiNumPushMsgs, uiUnreadMsg) ;
		if (JWAP_MAX_PUSH_MSGS <= g_pstPush_info->uiNumPushMsgs)
		{
			rCode = JC_ERR_PUSH_LIST_FULL ;
			DisplayPopup((PU8)GetString(STR_ID_WAP_BM_MAX_LIMIT), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
		else
		{
			rCode = jdi_PushMsgParse (g_pstPush_info->vPushHandle,
							pstPushData->pucBuffer,
							pstPushData->uiLength,
							pstPushData->pcContentType,
							pstPushData->pcOrigAddress,
							&stPushInfo) ;
			mmi_trace (1, "JDD_LOG: jWap_HandlePushMsg >> jdi_PushMsgParse () returned [%x]", -rCode) ;
		}		
		if (JC_OK == rCode)
		{
			JC_UINT8 ucUniqueID ;
			iReservedID = jc_atoi (pstPushData->pcPPGHostAddress) ;
			stPushInfo.iReserved = iReservedID ;
			ucUniqueID = jWap_CreateUniqueID () ;
			stPushInfo.iReserved = jWap_SetUniqueID (iReservedID, ucUniqueID) ;
			/*
			if((E_PUSH_SERVICE_LOADING == stPushInfo.ePushType) && 
				(E_PUSH_ACTION_EXECUTE_HIGH == stPushInfo.eAction))
			{
				mmi_trace (0, "JDD_LOG: setting true") ;
				stPushInfo.bIsRead = E_TRUE;
			}
			*/
			rCode = jdi_PushMsgAdd (g_pstPush_info->vPushHandle, &stPushInfo) ;

			mmi_trace (1, "JDD_LOG: jWap_HandlePushMsg >> jdi_PushMsgAdd () for SIM [%d] returned [%X] with ID [%d]",
				iReservedID, -rCode, ucUniqueID) ;

			if(JC_ERR_PUSH_SL_EXISTS == rCode)
			{
				mmi_trace (0, "JDD_LOG: sl exist") ;
				DisplayPopup(GetString(STR_ID_WAP_SERVICE_EXIST), IMG_GLOBAL_INFO, \
						1, 2000, 13);
			}			
			else
			{
				/*	Intentionally not checking the rCode of jdi_PushMsgAdd (), as it returns
					JC_ERR_PUSH_SERVICE_NO_ENTRY, JC_ERR_PUSH_LIST_FULL or JC_ERR_PUSH_SI_OLDER
					also in success case. Unconditionally calling jdi_PushMsgStorePersistent (). */
				rCode = jdi_PushMsgStorePersistent (g_pstPush_info->vPushHandle) ;
				mmi_trace (1, "JDD_LOG: jWap_HandlePushMsg >> jdi_PushMsgStorePersistent () returned [%x]", -rCode) ;

				jdi_PushMsgStatistics (g_pstPush_info->vPushHandle, &g_pstPush_info->uiNumPushMsgs, &uiUnreadMsg) ;
				mmi_trace (1, "JDD_LOG: jWap_HandlePushMsg >> jdi_PushMsgStatistics () returned Total and Unread as [%d %d]",
				g_pstPush_info->uiNumPushMsgs, uiUnreadMsg) ;
				jwap_showPushIcon(uiUnreadMsg, g_pstPush_info->uiNumPushMsgs);
			}
		}		
#if 0
		/* Test Code. To be removed once the platform side push changes done */
		{
			MMI_SIM_ID_T eSimID = (0 == iReservedID)? MMI_SIM_1: MMI_SIM_2 ;
			if (E_PUSH_SERVICE_LOADING == stPushInfo.ePushType &&
				stPushInfo.pmURL)
			{
				eSimID = MMI_SIM_1 ;
				jWap_LaunchFromIdle (E_WAP_LAUNCH_URL, eSimID, stPushInfo.pmURL) ;
			}
			else if (E_PUSH_SERVICE_INDICATION == stPushInfo.ePushType &&
				E_PUSH_ACTION_SIGNAL_HIGH == stPushInfo.eAction)
			{
				jWap_LaunchFromIdle (E_WAP_LAUNCH_PUSH_ITEM, eSimID, JC_NULL) ;
			}
		}
#endif
		if (JC_OK == rCode)
		{
			if(GetActiveScreenId() == SCR_ID_WAP_SERVICE_INBOX_SCREEN)
			{
				DisplayPopup(GetString(STR_ID_WAP_SERVICE_MSG_RECEIVED), IMG_GLOBAL_ACTIVATED, \
						1, 2000, 13);
				mmi_trace (0, "JDD_LOG: msg rcv on service inbox") ;
				//jWap_push_entry_handler () ;
				//DeleteScreenIfPresent (SCR_ID_WAP_SERVICE_INBOX_SCREEN) ;
			}
			/*
			else if(IsMMIInIdleState() && E_PUSH_SERVICE_LOADING == stPushInfo.ePushType && 
				(E_PUSH_ACTION_EXECUTE_HIGH == stPushInfo.eAction))
			{
				DisplayPopup(GetString(STR_ID_WAP_SERVICE_MSG_RECEIVED), IMG_GLOBAL_ACTIVATED, \
							1, 2000, 13);
				MMI_SIM_ID_T eSimID = (0 == iReservedID)? MMI_SIM_1: MMI_SIM_2 ;				
				jWap_LaunchFromIdle (E_WAP_LAUNCH_URL, eSimID, stPushInfo.pmURL) ;
			}*/
			else if(IsMMIInIdleState() && (E_PUSH_SERVICE_INDICATION == stPushInfo.ePushType || 
				(E_PUSH_SERVICE_LOADING == stPushInfo.ePushType) /*&& 
				E_PUSH_ACTION_EXECUTE_LOW == stPushInfo.eAction)*/))
			{
				MMI_SIM_ID_T eSimID = (0 == iReservedID)? MMI_SIM_1: MMI_SIM_2 ;
				g_pstJWapAppInfo->simid = eSimID;
				EntryNewScreen(SCR_ID_WAP_NEW_PUSH_MSG_IND, NULL, NULL, NULL);
				ShowCategory154Screen(0, 0,STR_ID_WAP_SERVICE_MSG_VIEW, 0, STR_GLOBAL_BACK, 0,
										(PU8)GetString (STR_ID_WAP_SERVICE_MSG_RECEIVED), JC_NULL, 
										IMG_GLOBAL_ACTIVATED, NULL);
				SetRightSoftkeyFunction(jmms_GoBackFromPushOnIdleScreen, KEY_EVENT_UP);
				SetLeftSoftkeyFunction(jmms_LaunchSIMsgFromIdleScreen, KEY_EVENT_UP);
				fpEndKeyDownHanler = GetKeyHandler (KEY_END, KEY_EVENT_DOWN);
				SetKeyHandler(jmms_EndkeyHandlerNewPushMsg, KEY_END, KEY_EVENT_DOWN);				
			}
			else
			{
				DisplayPopup(GetString(STR_ID_WAP_SERVICE_MSG_RECEIVED), IMG_GLOBAL_ACTIVATED, \
						1, 2000, 13);
				mmi_trace(1,"JDD_LOG: push noti flag set to true %d", __LINE__);
			    g_pstJWapAppInfo->bNeedNewPushPopOnIdle = E_TRUE;
			}
		}
		if(g_pstPush_info && g_pstPush_info->vPushHandle)
		{
		    
		   // avoiding the data to be freed
           if(g_pstPush_info->bIsBookMarkAdded != E_TRUE)
           	{
           	    
    			jWap_PushDeInitialise () ;
           	}
		}
	}
}

void jWap_PushFreeMemory (void)
{
	JC_RETCODE rCode ;
	if (g_pstPush_info)
	{
		rCode = jWap_PushDeInitialise () ;
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_PushFreeMemory >> Push context already deinitialised") ;
	}
}

void jWap_PushExit ()
{
	mmi_trace (1, "JDD_LOG: jWap_PushExit >> g_pstJWapAppInfo->eWapLaunchMode [%d]",
		g_pstJWapAppInfo->eWapLaunchMode) ;
	if (E_WAP_LAUNCH_URL == g_pstJWapAppInfo->eWapLaunchMode ||
		E_WAP_LAUNCH_PUSH_ITEM == g_pstJWapAppInfo->eWapLaunchMode ||
		E_WAP_LAUNCH_PUSH_INBOX == g_pstJWapAppInfo->eWapLaunchMode)
	{
		jWap_keyEndHandler () ;
	}
	else
	{
		jWap_PushFreeMemory () ;
		GoBackHistory () ;
	}
}


JC_UINT8 jWap_HandleServiceInboxEndKey(void* p)
{
	mmi_trace(12,"jdd_log:jWap_HandleServiceInboxEndKey");
	g_pstJWapAppInfo->bNeedNewPushPopOnIdle = E_FALSE;
	jWap_PushFreeMemory () ;
	return 0;
}

MMI_SIM_ID_T jWap_GetSIMID (JC_INT32 iReservedField)
{
	if (iReservedField & 1)
	{
		return MMI_SIM_2 ;
	}
	else
	{
		return MMI_SIM_1 ;
	}
}

JC_UINT8 jWap_GetUniqueID (JC_INT32 iReservedField)
{
	JC_UINT8 ucId = (iReservedField & 0xFF000000) >> 24 ;
	mmi_trace (1, "JDD_LOG: jWap_GetUniqueID >> iReservedField [%X] and ucId [%d]",
		iReservedField, ucId) ;
	return ucId ;
}

JC_INT32 jWap_SetUniqueID (JC_INT32 iReservedField, JC_UINT8 ucUniqueID)
{
	JC_INT32 iId = ((ucUniqueID << 24) | iReservedField) ;
	mmi_trace (1, "JDD_LOG: jWap_SetUniqueID >> iReservedField [%X], ucUniqueID [%d] and iId [%X]",
		iReservedField, ucUniqueID, iId) ;
	return iId ;
}

JC_UINT8 jWap_CreateUniqueID (void)
{
	JC_UINT8			ucPushIdArr [JWAP_MAX_PUSH_MSGS + 1]	 = {0, } ;
	ST_PUSH_INFO	*pstPushItem = g_pstPush_info->pstPushInfo ;
	JC_UINT32		uiNumPushMessages = 0 ;
	while (uiNumPushMessages < g_pstPush_info->uiNumPushMsgs && pstPushItem)
	{
		ucPushIdArr [jWap_GetUniqueID (pstPushItem->iReserved) - 1] = 1 ;
		pstPushItem = pstPushItem->pstNext ;
	}
	while (uiNumPushMessages <= g_pstPush_info->uiNumPushMsgs || 0 == uiNumPushMessages)
	{
		if (0 == ucPushIdArr [uiNumPushMessages++])
		{
			break ;
		}
	}
	return uiNumPushMessages ;
}
/**
 * @brief		Gets the push message from the message list based on the index.
 *
 * @param[in]	ucSelectedItem Specifies the index of the push message in
 *					the push message list to be returned.
 * @retval		The pointer of the push message in the list specified by the index.
 * @retval		JC_NULL, in the fail case.
 *
 * ucSelectedItem starts with 0. i.e, If the first push message needs to be retrieved
 * the input argument should be 0.
 */

void jWap_PushGetPushItemFromTheList (JC_UINT8 ucSelectedItem)
{
	JC_RETCODE	rCode = JC_OK ;	
	ST_PUSH_INFO *pstPushItem = JC_NULL ;
	mmi_trace (1, "JDD_LOG: ucSelectedMsg [%d]",ucSelectedItem) ;	
	if (JC_NULL == g_pstPush_info)
	{
		rCode = jWap_PushInitialise () ;
	}
	rCode = jdi_PushMsgGetList (g_pstPush_info->vPushHandle, &g_pstPush_info->pstPushInfo) ;
	if (g_pstPush_info && g_pstPush_info->pstPushInfo)
	{
		pstPushItem = g_pstPush_info->pstPushInfo ;
		if (pstPushItem)
		{
			while (pstPushItem && 0 < ucSelectedItem)
			{
				pstPushItem = pstPushItem->pstNext ;
				ucSelectedItem-- ;
			}
			st_iSelectedMsgReservedId = pstPushItem->iReserved ;
		}
		else
		{
			mmi_trace (1, "JDD_LOG: PushGetPushItemFromTheList g_pstPush_info is NULL") ;
		}
		
	}
}

ST_PUSH_INFO *jWap_PushGetPushItemFromTheListByReservedId (JC_INT32 iReservedID)
{
	JC_RETCODE	rCode = JC_OK ;
	ST_PUSH_INFO *pstPushItem = JC_NULL ;
	if (JC_NULL == g_pstPush_info)
	{
		rCode = jWap_PushInitialise () ;
	}
	if (g_pstPush_info)
	{
	    rCode = jdi_PushMsgGetList (g_pstPush_info->vPushHandle, &g_pstPush_info->pstPushInfo) ;
		pstPushItem = g_pstPush_info->pstPushInfo ;
		while (pstPushItem && iReservedID != pstPushItem->iReserved)
		{
			pstPushItem = pstPushItem->pstNext ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_PushGetPushItemFromTheListByReservedId () >> g_pstPush_info is NULL") ;
	}
	return pstPushItem ;
}

//Dont delete this code
//code is commented so that user can see all the push messages
#if 0
JC_RETCODE jWap_ShowLatestItemDetails (void)
{
	//ST_PUSH_INFO *pstPushInfo ;
	
	jWap_push_entry_handler () ;

	//code is commented so that user can see all the push messages
	/*
	if (g_pstPush_info)
	{
		g_pstPush_info->ucSelectedMsg = 0 ;
		pstPushInfo = jWap_PushGetPushItemFromTheList (g_pstPush_info->ucSelectedMsg) ;
		if (pstPushInfo)
		{
			st_iSelectedMsgReservedId = pstPushInfo->iReserved ;
			mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
			jWap_push_view_handler () ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_ShowLatestItemDetails >> g_pstPush_info is NULL") ;
	}
	*/
	return JC_OK ;
}
#endif

void jWap_push_entry_handler (void)
{
	U8 			*guiBuffer ;
	JC_RETCODE  rCode ;
	JC_UINT32		uiUnreadMsg ;
	ST_PUSH_INFO	*pstPushInfo ;

	guiBuffer 	= NULL ;
	rCode 		= JC_OK ;
	mmi_trace (0, "JDD_LOG: push_entry_handler called") ;
	rCode = jWap_PushInitialise () ;
	if (JC_OK == rCode)
	{
		jdi_PushMsgStatistics (g_pstPush_info->vPushHandle, &g_pstPush_info->uiNumPushMsgs, &uiUnreadMsg) ;
		if (0 < g_pstPush_info->uiNumPushMsgs)
		{
			pstPushInfo = g_pstPush_info->pstPushInfo ;
			if (JC_OK == rCode && pstPushInfo)
			{
				for (uiUnreadMsg = 0; uiUnreadMsg < g_pstPush_info->uiNumPushMsgs && pstPushInfo; uiUnreadMsg++)
				{
					g_pstPush_info->amListItems [uiUnreadMsg] = pstPushInfo->pmURL ;
					/*
					if (MMI_SIM_1 == jWap_GetSIMID (pstPushInfo->iReserved))
					{
						g_pstPush_info->uhIndexIconsImageList [uiUnreadMsg] =
							pstPushInfo->bIsRead == E_FALSE?
								IMG_MMS_INBOX_FOLDER_SIM1_UNREAD_ICON:
									IMG_MMS_INBOX_FOLDER_SIM1_READ_ICON ;
					}
					else
					{
						g_pstPush_info->uhIndexIconsImageList [uiUnreadMsg] =
							pstPushInfo->bIsRead == E_FALSE?
								IMG_MMS_INBOX_FOLDER_SIM2_UNREAD_ICON:
									IMG_MMS_INBOX_FOLDER_SIM2_READ_ICON ;
					}*/
					if (E_FALSE == pstPushInfo->bIsRead)
					{
						g_pstPush_info->uhIndexIconsImageList [uiUnreadMsg] = IMG_WAP_SERVICE_MSG_UNREAD_ICON ;
					}
					else
					{
						g_pstPush_info->uhIndexIconsImageList [uiUnreadMsg] = IMG_WAP_SERVICE_MSG_READ_ICON ;
					}
					pstPushInfo = pstPushInfo->pstNext ;
				}
			}
			guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_SERVICE_INBOX_SCREEN) ;
			EntryNewScreen(SCR_ID_WAP_SERVICE_INBOX_SCREEN, JC_NULL, jWap_push_entry_handler, JC_NULL) ;
			RegisterHighlightHandler (jWap_Push_Highlite_Handler);
			ShowCategory84Screen(STR_ID_WAP_SERVICE_INBOX,
				                 0,
				                 STR_GLOBAL_OPTIONS,
				                 IMG_GLOBAL_OPTIONS,
				                 STR_GLOBAL_BACK,
				                 IMG_GLOBAL_BACK,
								 g_pstPush_info->uiNumPushMsgs,
								 (U8**)g_pstPush_info->amListItems,
								 (PU16)g_pstPush_info->uhIndexIconsImageList,
								 LIST_MENU,
								 0, 
								 NULL);
			
			SetLeftSoftkeyFunction (jWap_PushOption, KEY_EVENT_UP) ;
			SetRightSoftkeyFunction(jWap_PushExit, KEY_EVENT_UP);
			// as this screen is called with launching the wap main screen also, making
			//sure that it release  the memory in all cases
			SetDelScrnIDCallbackHandler(SCR_ID_WAP_SERVICE_INBOX_SCREEN,jWap_HandleServiceInboxEndKey); 
			
		}
		else
		{			
			DisplayPopupCallBack ((PU8)GetString(STR_ID_WAP_NO_SERVICE_MSGS), \
				IMG_GLOBAL_ERROR, 1,1000, 1, jWap_PushExit) ;
		}
	}
}

void jWap_Push_Highlite_Handler (int iSelectIndex)
{
	mmi_trace (1, "JDD_LOG: Push_Highlite_Handler[%d]", iSelectIndex) ;
	if (JC_NULL == g_pstPush_info)
	{
		jWap_PushInitialise () ;
	}
	g_pstPush_info->ucSelectedMsg = iSelectIndex ;
}

void jWap_Push_Option_Highlite_Handler(int iSelectIndex)
{
	switch(iSelectIndex)
	{
		case 0:
			SetLeftSoftkeyFunction (jWap_push_view_handler, KEY_EVENT_UP) ;
			break ;
		case 1:
			SetLeftSoftkeyFunction (jWap_push_add_to_bm_handler, KEY_EVENT_UP) ;
			break ;	
		case 2:
			SetLeftSoftkeyFunction (jWap_push_clear_handler, KEY_EVENT_UP) ;
			break ;
		case 3:
			SetLeftSoftkeyFunction (jWap_push_clearall_handler, KEY_EVENT_UP) ;
			break ;
		default:
			break ;
	}
}

void jWap_PushOption ()
{
	U8* guiBuffer = NULL; 
	U16 item_texts[] = {STR_ID_WAP_VIEW,
						STR_ID_WAP_ADD_TO_BM,
						STR_GLOBAL_DELETE,
						STR_GLOBAL_DELETE_ALL
						} ;
	
	EntryNewScreen (SCR_ID_WAP_SERVICE_MSG_OPTIONS_SCREEN, NULL, jWap_PushOption, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_SERVICE_MSG_OPTIONS_SCREEN) ;		
	
	RegisterHighlightHandler (jWap_Push_Option_Highlite_Handler) ;
	jWap_PushGetPushItemFromTheList(g_pstPush_info->ucSelectedMsg);
	SetParentHandler(SCR_ID_WAP_SERVICE_INBOX_SCREEN);
	ShowCategory15Screen (STR_ID_WAP_SERVICE_INBOX, 0, 
						STR_GLOBAL_OK,
						IMG_GLOBAL_OK,
						STR_GLOBAL_BACK,
						IMG_GLOBAL_BACK,
						(sizeof (item_texts)/sizeof (U16)), 
						item_texts, (PU16)gIndexIconsImageList, 
						1, 0, guiBuffer) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;
}

void jWap_push_view_handler ()
{
	JC_RETCODE rCode ;
	JC_UINT32		uiUnreadMsgs ;
	ST_PUSH_INFO *pstPushInfo = jWap_PushGetPushItemFromTheListByReservedId (st_iSelectedMsgReservedId) ;
	if (pstPushInfo)
	{
		if (E_FALSE == pstPushInfo->bIsRead)
		{
			rCode = jdi_PushMsgSetReadStatus (g_pstPush_info->vPushHandle, E_TRUE, pstPushInfo) ;
			if (JC_OK == rCode)
			{
				rCode = jdi_PushMsgStorePersistent (g_pstPush_info->vPushHandle) ;
				mmi_trace (1, "JDD_LOG: jWap_push_view_handler >> jdi_PushMsgStorePersistent [%X]", -rCode) ;
			}
		}
		jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, jWAP_MAX_INPUT_SIZE) ;
		jc_wstrcpy ((JC_CHAR *)g_pstJWapAppInfo->pcInputBuff, (JC_CHAR*)GetString (STR_ID_WAP_BM_URL)) ;
		jdi_CUtilsTcsStrCat (": ", (JC_CHAR *)g_pstJWapAppInfo->pcInputBuff) ;
		jc_wstrcat ((JC_CHAR *)g_pstJWapAppInfo->pcInputBuff, (JC_CHAR *)pstPushInfo->pmURL) ;

		jdi_CUtilsTcsStrCat ("\r\n\r\n", (JC_CHAR *)g_pstJWapAppInfo->pcInputBuff) ;

		if (pstPushInfo->pmMessage && (0 < jc_tcslen(pstPushInfo->pmMessage)))
		{			
			jc_wstrcat ((JC_CHAR *)g_pstJWapAppInfo->pcInputBuff, (JC_CHAR*)GetString (STR_ID_WAP_BM_TITLE)) ;
			jdi_CUtilsTcsStrCat (": ", (JC_CHAR *)g_pstJWapAppInfo->pcInputBuff) ;		
			jc_wstrcat ((JC_CHAR *)g_pstJWapAppInfo->pcInputBuff, (JC_CHAR *)pstPushInfo->pmMessage) ;
		}
		jdi_PushMsgStatistics (g_pstPush_info->vPushHandle, \
								&g_pstPush_info->uiNumPushMsgs, &uiUnreadMsgs) ;
		jwap_showPushIcon(uiUnreadMsgs, g_pstPush_info->uiNumPushMsgs);
		EntryNewScreen(SCR_ID_WAP_SERVICE_MSG_DETAILS_SCREEN, JC_NULL, jWap_push_view_handler, NULL);
		ShowCategory78Screen(STR_ID_WAP_SERVICE_MSG, IMG_ID_JATAAYU_WAP_ICON,
							STR_ID_WAP_GO, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							NULL,
							(U8*) g_pstJWapAppInfo->pcInputBuff,
							jc_tcslen(g_pstJWapAppInfo->pcInputBuff),
							NULL);
		SetLeftSoftkeyFunction (jWap_push_go_handler,KEY_EVENT_UP) ;
		SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_push_view_handler >> pstPushInfo is NULL") ;
		GoBackHistory () ;
	}
}

/* As per the behaviour in the reference phone, while fetching the push URL,
	the sim for which the push has come need not be checked. Hence if 0 is introduced */
#if 0
void jWap_push_go_handler ()
{
	JC_RETCODE		rCode ;
	MMI_SIM_ID_T	eSimId ;
	ST_PUSH_INFO	*pstPushInfo = jWap_PushGetPushItemFromTheListByReservedId (st_iSelectedMsgReservedId) ;
	if (JC_NULL != pstPushInfo)
	{
		jWap_set_priMmiScreen() ;
		rCode = jWap_config_dispSettings () ;

		if (JC_OK == rCode)
		{
			eSimId = jWap_GetSIMID (pstPushInfo->iReserved) ;
			mmi_trace (1, "JDD_LOG: jWap_push_go_handler >> Mode is [%d], Wap and Push Sim IDs [%d %d]",
					g_pstJWapAppInfo->eWapLaunchMode, g_pstJWapAppInfo->simid, eSimId) ;
			if (E_WAP_LAUNCH_NORMAL == g_pstJWapAppInfo->eWapLaunchMode &&
				g_pstJWapAppInfo->simid != eSimId)
			{
				/*	g_pstJWapAppInfo->pmPushURL is used to identify that there is a
					sim change for fetching push url */
				g_pstJWapAppInfo->pmPushURL = pstPushInfo->pmURL ;
				g_pstJWapAppInfo->bIsDCAvailable = E_FALSE ;
				g_pstJWapAppInfo->original_simid = g_pstJWapAppInfo->simid ;
				g_pstJWapAppInfo->simid = eSimId ;
				mmi_trace (1, "JDD_LOG: jWap_push_go_handler >> with Changed Sim ID [%d]",
					g_pstJWapAppInfo->simid) ;
			}
			else
			{
				mmi_trace (1, "JDD_LOG: jWap_push_go_handler >> with UnChanged Sim ID [%d]",
					g_pstJWapAppInfo->simid) ;
			}
			rCode = jWap_config_connProfile () ;
		}
		if (JC_OK == rCode)
		{
			jWap_setConnectingTitle ((JC_CHAR*)GetString(STR_ID_WAP_CONNECTING)) ;
			jWap_send_fetch_request (pstPushInfo->pmURL) ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_push_go_handler >> pstPushInfo is NULL") ;
	}
	jWap_PushFreeMemory () ;
	DeleteNScrId (SCR_ID_WAP_SERVICE_INBOX_SCREEN) ;	
}
#else
void jWap_push_go_handler ()
{
	JC_RETCODE		rCode ;
	ST_PUSH_INFO	*pstPushInfo = jWap_PushGetPushItemFromTheListByReservedId (st_iSelectedMsgReservedId) ;
	if (JC_NULL != pstPushInfo)
	{
		jWap_set_priMmiScreen() ;
		rCode = jWap_config_dispSettings () ;
		if (JC_OK == rCode)
		{
			rCode = jWap_config_connProfile () ;
		}
		if (JC_OK == rCode)
		{
			jWap_setConnectingTitle ((JC_CHAR*)GetString(STR_ID_WAP_CONNECTING)) ;
			jWap_send_fetch_request (pstPushInfo->pmURL) ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_push_go_handler >> pstPushInfo is NULL") ;
	}
	jWap_PushFreeMemory () ;
	DeleteNScrId (SCR_ID_WAP_SERVICE_INBOX_SCREEN) ;	
}

#endif
void jWap_push_clear_handler()
{
	GetPushClearUserConfirmation () ;
}

void jWap_push_clearall_handler()
{
	GetPushClearAllUserConfirmation () ;
}


void GetPushClearUserConfirmation ()
{
	mmi_trace (1, "JDD_LOG: GetPushClearUserConfirmation >> begin");
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, get_string(STR_ID_WAP_CLEAR_CONFIRMATION), 0, 1) ;
	SetLeftSoftkeyFunction (HandleClearPushYes,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (HandleClearPushNo,KEY_EVENT_UP) ;
	mmi_trace (1, "JDD_LOG: GetPushClearUserConfirmation >> end") ;
}

void GetPushClearAllUserConfirmation ()
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, get_string(STR_ID_WAP_CLEAR_ALL_CONFIRMATION), 0, 1) ;
	SetLeftSoftkeyFunction(HandleClearAllPushYes,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction(HandleClearPushNo,KEY_EVENT_UP) ;
}

void HandleClearPushYes ()
{
	JC_RETCODE		rCode ;
	JC_UINT32		uiUnreadMsgs ;
	ST_PUSH_INFO	*pstPushItem = jWap_PushGetPushItemFromTheListByReservedId (st_iSelectedMsgReservedId) ;
	if (JC_NULL != pstPushItem)
	{
		jdi_PushMsgDelete (g_pstPush_info->vPushHandle, pstPushItem) ;
		rCode = jdi_PushMsgStorePersistent (g_pstPush_info->vPushHandle) ;
		mmi_trace (1, "JDD_LOG: HandleClearPushYes >> jdi_PushMsgStorePersistent () returned [%X]", -rCode) ;
		if (JC_OK == rCode)
		{
			DisplayPopup(GetString (STR_ID_WAP_DELETE_SUCCESS), IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
		}
		jdi_PushMsgStatistics (g_pstPush_info->vPushHandle, \
								&g_pstPush_info->uiNumPushMsgs, &uiUnreadMsgs) ;
		jwap_showPushIcon(uiUnreadMsgs, g_pstPush_info->uiNumPushMsgs);
		if (!g_pstPush_info->uiNumPushMsgs)
		{
			jWap_PushFreeMemory () ;
			GoBacknHistory (2) ;
		}
		else
		{
			jWap_PushFreeMemory () ;
			GoBacknHistory(1) ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: HandleClearPushYes >> DANGER: pstPushItem is NULL") ;
	}
}

void HandleClearAllPushYes ()

{
	JC_RETCODE rCode ;

	/*	jWap_PushInitialise () is called here to fix the following bug:
		Crash if a new push message is added while the option menu is active screen.
		But this is not a good solution. To permanently fix this, push message ID
		can be incorporated in iReserved field */
	jWap_PushInitialise () ;
	jdi_PushMsgDeleteAll (g_pstPush_info->vPushHandle) ;
	rCode = jdi_PushMsgStorePersistent (g_pstPush_info->vPushHandle) ;
	mmi_trace (1, "JDD_LOG: HandleClearAllPushYes >> jdi_PushMsgStorePersistent () returned [%X]", -rCode) ;
	if (JC_OK == rCode)
	{
		DisplayPopup(GetString (STR_ID_WAP_DELETE_SUCCESS), IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
	}
	jwap_UpdatePushIcon();
	jWap_PushFreeMemory () ;
	GoBacknHistory (1) ;
}

void HandleClearPushNo ()
{
	GoBackHistory() ;
}

void jwap_New_Push_Msg_Ind()
{
	U8 				*guiBuffer ;
	JC_RETCODE  	rCode ;
	JC_UINT32		uiUnreadMsg ;
	ST_PUSH_INFO	*pstPushInfo ;

	guiBuffer 	= NULL ;
	rCode 		= JC_OK ;

	rCode = jWap_PushInitialise () ;
	if (JC_OK == rCode)
	{
		jdi_PushMsgStatistics (g_pstPush_info->vPushHandle, &g_pstPush_info->uiNumPushMsgs, \
								&uiUnreadMsg) ;
		if (0 < g_pstPush_info->uiNumPushMsgs)
		{
			pstPushInfo = g_pstPush_info->pstPushInfo ;
			if (JC_OK == rCode && pstPushInfo)
			{/*
				if(E_PUSH_SERVICE_LOADING == pstPushInfo->ePushType && \
					E_PUSH_ACTION_EXECUTE_HIGH == pstPushInfo->eAction)
				{
					DisplayPopup(GetString(STR_ID_WAP_SERVICE_MSG_RECEIVED), IMG_GLOBAL_ACTIVATED, \
							1, 2000, 13);
					//MMI_SIM_ID_T eSimID = (0 == pstPushInfo->iReserved)? MMI_SIM_1: MMI_SIM_2 ;
					//jWap_LaunchFromIdle (E_WAP_LAUNCH_URL, eSimID, pstPushInfo->pmURL) ;
				}
				else*/ if(E_PUSH_SERVICE_INDICATION == pstPushInfo->ePushType ||
						(E_PUSH_SERVICE_LOADING == pstPushInfo->ePushType /*&& \
						E_PUSH_ACTION_EXECUTE_LOW== pstPushInfo->eAction*/))
				{
					MMI_SIM_ID_T eSimID = (0 == pstPushInfo->iReserved)? MMI_SIM_1: MMI_SIM_2 ;
					g_pstJWapAppInfo->simid = eSimID;
					EntryNewScreen(SCR_ID_WAP_NEW_PUSH_MSG_IND, NULL, NULL, NULL);
					ShowCategory154Screen(0, 0,STR_ID_WAP_SERVICE_MSG_VIEW, 0, STR_GLOBAL_BACK, 0,
											(PU8)GetString (STR_ID_WAP_SERVICE_MSG_RECEIVED), JC_NULL, 
											IMG_GLOBAL_ACTIVATED, NULL);
					SetRightSoftkeyFunction(jmms_GoBackFromPushOnIdleScreen, KEY_EVENT_UP);
					SetLeftSoftkeyFunction(jmms_LaunchSIMsgFromIdleScreen, KEY_EVENT_UP);
					fpEndKeyDownHanler = GetKeyHandler (KEY_END, KEY_EVENT_DOWN);
					SetKeyHandler(jmms_EndkeyHandlerNewPushMsg, KEY_END, KEY_EVENT_DOWN);				
				}
			}
		}
	}
	jWap_PushFreeMemory () ;
}

void jWap_push_add_to_bm_handler()
{
	g_pstPush_info->bIsBookMarkAdded = E_TRUE;
	if(NULL == g_pstJWapAppInfo->vBookMarkHandle)
	{
		jWap_bmInitBookMark();
	}
	ST_PUSH_INFO *pstPushInfo = jWap_PushGetPushItemFromTheListByReservedId (st_iSelectedMsgReservedId) ;
	if (pstPushInfo)
	{
		jwap_PrepareBMLaunch(pstPushInfo->pmURL);
	}
}

JC_UINT32 jwap_getUnreadPushMsg(JC_UINT32 *uiTotalMsg)
{
	JC_UINT32		uiUnreadMsg;
	JC_RETCODE		rCode ;
	rCode = jWap_PushInitialise () ;
	if (JC_OK == rCode)
	{	
		jdi_PushMsgStatistics (g_pstPush_info->vPushHandle, &g_pstPush_info->uiNumPushMsgs, &uiUnreadMsg) ;
		mmi_trace (0, "jwap_log: jwap_UpdatePushIcon Statistics Total and Unread[%d %d]",
			g_pstPush_info->uiNumPushMsgs, uiUnreadMsg) ;
		*uiTotalMsg = g_pstPush_info->uiNumPushMsgs;
	}
	mmi_trace (1, "jwap_log: getUnreadPushMsg rCode[%d]", rCode);

	rCode = jWap_PushDeInitialise () ;
	mmi_trace (1, "jwap_log: getUnreadPushMsg deinit rCode[%d]", rCode);
	return uiUnreadMsg;
}
#endif // JWAP_PUSH_ENABLE	
#endif //jwap_push.c

