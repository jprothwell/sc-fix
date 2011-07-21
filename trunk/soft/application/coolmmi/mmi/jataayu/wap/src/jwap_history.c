 /***************************************************************************
 *
 * File Name : jwap_history.c
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
 * @file		jwap_history.c
 * @ingroup	jWAP
 * @brief	
 *
 **/

#define MODULE_NAME "JWAP"
#define FILE_NAME "jwap_mmi_cb.c"

#ifdef JATAAYU_SUPPORT 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

#include "jwap_inc.h"



 /***************************************************************************
 * Macros
 **************************************************************************/

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/

 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/

//#include "mmi_trace.h"
//#include "EventsGprot.h"
//#include "HistoryGprot.h"
//#include "wgui_categories.h"
//#include "GlobalDefs.h"
#include "jwap_interface.h"
#include "jwap_history.h"
#include "jdi_browserengine.h"
#include "jwap_inc.h"
#include "jdi_history.h"
#include "jdi_cutils_tcsstr.h"
#include "jwap_mmi_cb.h"
#include "settingprofile.h"
#include "jwap_str_id.h"
#define HISTORY_LIST_ITEM_NUMBER	10


typedef struct __history_info__
{
	ST_HISTORY_DOMAIN*  g_pstHstDomain;
	ST_HISTORY_DOMAIN*  g_pstcurrHistoryDomain ;
	U8				    g_selection ;
	JC_INT32			g_iHistoryCount ;
	JC_INT32			g_history_selected_index ;
	JC_UINT8			*pListItems[HISTORY_LIST_ITEM_NUMBER] ;
} ST_HISTORY_INFO ;


static ST_HISTORY_INFO *g_pstHistory_info ;


void jWap_historyFreeMemory ()
{
	if (g_pstHistory_info)
	{
		while (g_pstHistory_info->g_iHistoryCount)
		{
			if (g_pstHistory_info->pListItems [ \
				g_pstHistory_info->g_iHistoryCount - 1])
			{
				jdd_MemFree (g_pstHistory_info->pListItems [ \
								g_pstHistory_info->g_iHistoryCount - 1]) ;
			}
			g_pstHistory_info->g_iHistoryCount -- ; 
		}

		jdd_MemFree (g_pstHistory_info) ;
		g_pstHistory_info = JC_NULL ;
	}
}

void jWap_historyExit ()
{
	jWap_historyFreeMemory () ;	
	GoBackHistory () ;
}


void jWap_recentPages_entry_handler(void)
{
	U8 			*guiBuffer ;	
	JC_RETCODE  rCode ;

	guiBuffer 	= NULL ;
	rCode 		= JC_OK ;

	mmi_trace (1, "@@@ jWap_recentPages_entry_handler begin @@@") ;
	g_pstHistory_info = (ST_HISTORY_INFO*)jdd_MemAlloc(1,sizeof(ST_HISTORY_INFO)) ;
	if(NULL != g_pstHistory_info)
	{
		rCode = get_list_item(g_pstHistory_info->pListItems);
		if(JC_NULL != g_pstHistory_info->pListItems[0])
		{

			mmi_trace (1, "@@@ jWap_recentPages_entry_handler create screen @@@") ;
			guiBuffer = GetCurrGuiBuffer(SCR_ID_WAP_RECENTPAGES_SCREEN);
			//SetParentHandler(0);
			
			EntryNewScreen(SCR_ID_WAP_RECENTPAGES_SCREEN, NULL, \
				jWap_recentPages_entry_handler, NULL);
			RegisterHighlightHandler(jWap_history_Highlite_Handler);
			ShowCategory84Screen(STR_ID_WAP_RECENT,
				                 IMG_ID_JATAAYU_WAP_ICON,
				                 STR_GLOBAL_OPTIONS,
				                 IMG_GLOBAL_OPTIONS,
				                 STR_GLOBAL_BACK,
				                 IMG_GLOBAL_BACK,
								 g_pstHistory_info->g_iHistoryCount,
								 (U8**)g_pstHistory_info->pListItems,
								 (PU16)gIndexIconsImageList,
								 LIST_MENU,
								 0, 
								 NULL);
			
			SetLeftSoftkeyFunction (jWap_OptionHistory, KEY_EVENT_UP);
			SetRightSoftkeyFunction(jWap_historyExit, KEY_EVENT_UP);			
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_ID_WAP_NO_HISTORY), \
				IMG_GLOBAL_ERROR, 1,1000, 1);
		}
	}
}

void jWap_history_Highlite_Handler(int iSelectIndex)
{
	g_pstHistory_info->g_history_selected_index = iSelectIndex ;
}

void jWap_history_Option_Highlite_Handler(int iSelectIndex)
{
	switch(iSelectIndex)
	{
		case 0:
			SetLeftSoftkeyFunction (jWap_history_go_handler, KEY_EVENT_UP) ;
			break ;
		case 1:
			SetLeftSoftkeyFunction (jWap_history_clear_handler, KEY_EVENT_UP) ;
			break ;
		case 2:
			SetLeftSoftkeyFunction (jWap_history_clearall_handler, KEY_EVENT_UP) ;
			break ;
		default:
			break ;
	}
}

void jWap_OptionHistory()
{
	U8* guiBuffer ; 
	U16 item_texts[] = {STR_ID_WAP_GO, \
						STR_ID_WAP_BM_CLEAR,
						STR_ID_WAP_BM_CLEARALL
						} ;

	mmi_trace (1, "@@@ jWap_options_handler. Items @@@") ;

	EntryNewScreen (SCR_ID_BM_WAP_OPTIONS, NULL, NULL, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_BM_WAP_OPTIONS) ;	
	RegisterHighlightHandler (jWap_history_Option_Highlite_Handler) ;
	
	SetParentHandler(SCR_ID_WAP_RECENTPAGES_SCREEN);
	ShowCategory15Screen (STR_ID_WAP_RECENT, IMG_ID_JATAAYU_WAP_ICON, 
							STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							(sizeof (item_texts)/sizeof (U16)),
							item_texts, (PU16)gIndexIconsImageList, 
							1, 0, guiBuffer) ;
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;	
}

JC_RETCODE get_list_item(U8** pListItems)
{
	JC_RETCODE 			rCode ;
	void				*vHandle ;
	const ST_HISTORY_DOMAIN	*pstHistoryDomain ;
	const ST_HISTORY_HOST*	pstHostTemp ;
	JC_INT32			aCount ;

	rCode 				= JC_OK ;
	vHandle 			= JC_NULL ;
	pstHistoryDomain 	= JC_NULL ;
	pstHostTemp 		= NULL ;
	aCount 				= 0 ;
	
	if(g_pstJWapAppInfo->pvHistory != NULL)
	{
		pstHistoryDomain = jdi_HistorySortDomain (g_pstJWapAppInfo->pvHistory, \
							E_HISTORY_DESCENDING_ORDER, E_HISTORY_SORT_ACCESSED_TIME) ;
		
		if (pstHistoryDomain)
		{
			pstHostTemp = jdi_HistorySortHost (g_pstJWapAppInfo->pvHistory, \
				E_HISTORY_DESCENDING_ORDER, E_HISTORY_SORT_ACCESSED_TIME, \
				pstHistoryDomain) ;
			
			while ((pstHostTemp != NULL) && (aCount < HISTORY_LIST_ITEM_NUMBER))
			{
				if (pstHostTemp->stPageInfo.pmTitle) /* There is no Title. Consider URL */
				{
					pListItems[aCount] = jdd_MemAlloc ( \
											jc_wstrlen (pstHostTemp->stPageInfo.pmTitle) + 1, \
											sizeof(JC_CHAR)) ;
					jc_wstrncpy((JC_CHAR *)pListItems[aCount], \
						pstHostTemp->stPageInfo.pmTitle, \
						jc_wstrlen(pstHostTemp->stPageInfo.pmTitle)) ; 
					aCount++ ;
					pstHostTemp = pstHostTemp->pstNextHost ;
				}
				else
				{
					pListItems[aCount] = jdd_MemAlloc ( \
											jc_wstrlen (pstHostTemp->stPageInfo.pmURL)+1, \
											sizeof(JC_CHAR)) ;
					jc_wstrncpy((JC_CHAR *)pListItems[aCount], \
						pstHostTemp->stPageInfo.pmURL, \
						jc_wstrlen(pstHostTemp->stPageInfo.pmURL)) ; 
					aCount++ ;
					pstHostTemp = pstHostTemp->pstNextHost ;
				}
			}
		}
	}
	g_pstHistory_info->g_iHistoryCount = aCount ;
	return rCode;
}

void GiveFetchRequest(JC_CHAR *pUrl, JC_BOOLEAN bIsReload)
{
	ST_BE_FETCH_REQ	stFetchReq ;
	
	memset (&stFetchReq, 0 , sizeof(ST_BE_FETCH_REQ)) ;
	stFetchReq.pstPostData = jdd_MemAlloc (1, sizeof(ST_BE_POST_DATA)) ;

	stFetchReq.pmURI = (JC_CHAR*)pUrl ;
	stFetchReq.bIsReloadPage = bIsReload;
	stFetchReq.pstPostData->uPostData.pcPostData	=  NULL;
	stFetchReq.pstMimeHeaders = (ST_MIME_HEADERS *)NULL ;
	stFetchReq.pstProxyAuthInfo = (ST_BE_AUTH_INFO *)NULL ;
	stFetchReq.pstAuthInfo =(ST_BE_AUTH_INFO *) NULL ;
	stFetchReq.pstPostData->uiPostLen = 0;
	
	jdi_BEFetchRequest (jWap_Get_Brw_Handle, &stFetchReq) ;
	jdd_MemFree(stFetchReq.pstPostData) ;
}

void jWap_history_go_handler()
{
	const ST_HISTORY_DOMAIN 	*pstHistoryDomain;
	JC_RETCODE 			rCode ;
	const ST_HISTORY_HOST*	pstHostTemp ;
	JC_INT32			aCount ;

	rCode 				= JC_OK;
	pstHostTemp			= NULL ;
	aCount				= 0 ;

	mmi_trace (1, "JDD_LOG: jWap_history_go_handler >> History Index: %d",
		g_pstHistory_info->g_history_selected_index) ;
	if(g_pstJWapAppInfo->pvHistory != NULL)
	{
		pstHistoryDomain = jdi_HistorySortDomain (g_pstJWapAppInfo->pvHistory, \
								E_HISTORY_DESCENDING_ORDER, E_HISTORY_SORT_ACCESSED_TIME) ;
		if (pstHistoryDomain != NULL)
		{
			pstHostTemp = jdi_HistorySortHost (g_pstJWapAppInfo->pvHistory, \
			E_HISTORY_DESCENDING_ORDER, E_HISTORY_SORT_ACCESSED_TIME, \
			pstHistoryDomain) ;
			while (aCount < g_pstHistory_info->g_history_selected_index)
			{					
				pstHostTemp = pstHostTemp->pstNextHost ;
				aCount++ ;
			}
		}
	}

	if (NULL != pstHostTemp)
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
			jWap_send_fetch_request (pstHostTemp->stPageInfo.pmURL) ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_history_go_handler >> pstHostTemp is NULL") ;
	}
	jWap_historyFreeMemory () ;
	DeleteNScrId (SCR_ID_WAP_RECENTPAGES_SCREEN) ;
	
}

void jWap_history_clear_handler()
{
	GetHistoryClearUserConfirmation() ;
}

void jWap_history_clearall_handler()
{
	GetHistoryClearAllUserConfirmation() ;
}


void GetHistoryClearUserConfirmation()
{
	mmi_trace (1, "@@@ GetUserConfirmation. begin");
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, get_string(STR_ID_WAP_CLEAR_CONFIRMATION), IMG_GLOBAL_QUESTION, WARNING_TONE) ;
	SetLeftSoftkeyFunction(HandleClearHistoryYes,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction(HandleHistoryNo,KEY_EVENT_UP) ;
	mmi_trace (1, "@@@ GetUserConfirmation. end");
}

void GetHistoryClearAllUserConfirmation()
{
	mmi_trace (1, "@@@ GetUserConfirmation. begin");
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, get_string(STR_ID_WAP_CLEAR_ALL_CONFIRMATION), IMG_GLOBAL_QUESTION, WARNING_TONE) ;
	SetLeftSoftkeyFunction(HandleClearAllHistoryYes,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction(HandleHistoryNo,KEY_EVENT_UP) ;
	mmi_trace (1, "@@@ GetUserConfirmation. end");
}

void HandleClearHistoryYes()
{
	
	const ST_HISTORY_DOMAIN		*pstHistoryDomain;
	const ST_HISTORY_HOST 		*pstHostTemp ;
	JC_RETCODE				rCode ;
	JC_INT32				aCount ;
	JC_INT32				iHistCount ;

	rCode			= JC_OK ;
	pstHostTemp 	= NULL ; 
	aCount			= 0 ;
	iHistCount = 0 ;
	
	if(NULL != g_pstJWapAppInfo->pvHistory)
	{
		pstHistoryDomain = jdi_HistorySortDomain (g_pstJWapAppInfo->pvHistory, \
				E_HISTORY_DESCENDING_ORDER, E_HISTORY_SORT_ACCESSED_TIME) ;

		
		if (pstHistoryDomain != NULL)
		{	
			pstHostTemp = jdi_HistorySortHost (g_pstJWapAppInfo->pvHistory, \
				E_HISTORY_DESCENDING_ORDER, E_HISTORY_SORT_ACCESSED_TIME, \
				pstHistoryDomain) ;

			while(pstHostTemp != NULL)
			{
				if(aCount < g_pstHistory_info->g_history_selected_index)
				{
					aCount++ ;
					pstHostTemp = pstHostTemp->pstNextHost ;
				}
				else
				{
					break ;
				}
			}
		}
	   			
		if (JC_NULL != pstHostTemp)
		{
			jdi_HistoryClearSpecific (g_pstJWapAppInfo->pvHistory, pstHostTemp->stPageInfo.pmURL) ;		
			
			if (!g_pstHistory_info->g_iHistoryCount)
			{
				jWap_historyFreeMemory () ;
				GoBacknHistory (1) ;
			}
			else
			{
				jWap_historyFreeMemory () ;
				GoBackHistory () ;
			}
			DisplayPopup((PU8)GetString(STR_ID_WAP_DELETE_SUCCESS), IMG_GLOBAL_ACTIVATED, 1, 2000, 1);
		}

		mmi_trace (1, "@@@ jWap_history_clear_handler done @@@") ;			
	}
}

void HandleClearAllHistoryYes()

{
	void			*vHandle ; 
	void 			*vHistoryContext ;
	JC_RETCODE 		rCode ;

	vHistoryContext = NULL ;
	vHandle			= NULL ;
	rCode = JC_OK ;
	
	vHandle = jWap_Get_Brw_Handle();	
	
	if(NULL != vHandle)
	{
		rCode = jdi_BEGetHistoryContext(vHandle,&vHistoryContext);
		if(NULL != vHistoryContext)
		{
			jdi_HistoryClearAll (vHistoryContext, NULL) ;
			jWap_historyFreeMemory () ;
		}
	}
	//GoBacknHistory (1) ;
	DisplayPopup(GetString (STR_ID_WAP_DELETE_SUCCESS), IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
	jWap_GoBackToOption();
	
}

void HandleHistoryNo()
{
	GoBackHistory() ;
}

BOOL isInWapApp(void)
{
	U16 screen_id = SCR_ID_WAP_MAIN_SCREEN;
	mmi_trace(TRUE,"JWAP_LOG: isInWapApp  ");

	for(;screen_id<= SCR_ID_WAP_END;screen_id++)
	{
		if(IsScreenPresent(screen_id))
			return TRUE;
	}
	screen_id = GetActiveScreenId();
	if((screen_id >=SCR_ID_WAP_MAIN_SCREEN )&&(screen_id <= SCR_ID_WAP_END))
	{
		return TRUE;
	}

	return FALSE ;
}

#endif //jwap_history.c

