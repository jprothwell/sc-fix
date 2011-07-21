#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_storedlg.c"
 /***************************************************************************
 *
 * File Name : jmms_storedlg.c
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
 * @file		jmms_storedlg.c
 * @ingroup		jMMS
 * @brief		Describes the Store Dialog API's.
 **/

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
#include "jmms_properties.h"
#include "messagesresourcedata.h"
#ifdef  __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif

 /***************************************************************************
 * Macros
 **************************************************************************/
/***************************************************************************
* Global variables declarations
**************************************************************************/
static U8** gpMsgSubList ;
static U16 *gpMsgIconList;
JC_UINT32 guiMsgListCount;
static	void (*sMMSOptionsLSKHandlers [JMMS_MAX_MENU_ITEM])(void) ;

extern JC_BOOLEAN gNeedNewMMSPopOnIdle;

/***************************************************************************
* Local Function Prototypes
***************************************************************************/

/***************************************************************************
* All Local Function Definitions
***************************************************************************/

/***************************************************************************
* All Global Function Definitions
**************************************************************************/

void jMms_StoreInboxMmsMsgHlthandler(int iSelectIndex)
{
	mmi_trace (1, "JDD_LOG: jMms_StoreInboxMmsMsgHlthandler >> iSelectIndex is [%d]", iSelectIndex) ;
	SetLeftSoftkeyFunction (*sMMSOptionsLSKHandlers [iSelectIndex], KEY_EVENT_UP) ;
}

void jmms_InboxPrepareLSKHandler()

{
	MESSAGE_LIST		*pstMsgDetail = NULL ;
	
	jmms_GetMsgDetailsByIndex (g_pstJMmsAppInfo->hMsgStore, g_pstJMmsAppInfo->eCurrentFolderID,
										g_pstJMmsAppInfo->iFolderItemIndex, &pstMsgDetail);

    // this message id is used in future for all operations like view , delete , view properties, resend, 
    // edit, reply replay all, forward e.tc on this message 
	g_pstJMmsAppInfo->uiHighlightedMsgId = pstMsgDetail->uiMessageID;
	jMms_StoreInbox_handle_highlight_item();	
}

void jMms_StoreInbox_handle_highlight_item()
{
	U8* 			guiBuffer;
	MESSAGE_LIST	*pstMsgDetail = JC_NULL ;
	JC_INT32		iIndex = 0;
	
	U16 item_texts[JMMS_MAX_MENU_ITEM] = {0, } ;

	jc_memset (sMMSOptionsLSKHandlers, 0, sizeof (sMMSOptionsLSKHandlers)) ;

	if (jmms_GetMsgDetailsByID(g_pstJMmsAppInfo->hMsgStore, g_pstJMmsAppInfo->eCurrentFolderID,
										g_pstJMmsAppInfo->uiHighlightedMsgId, &pstMsgDetail) == JC_OK)
	{

	if(E_MSG_TYPE_NOTIFICATION == pstMsgDetail->pMessageHeaders->eMsgType)
	{
		item_texts [iIndex] = STR_ID_MMS_DOWNLOAD ;
	}
	else
	{
		item_texts [iIndex] = STR_ID_MMS_VIEW_MSG ;		
	}
	sMMSOptionsLSKHandlers [iIndex++] =  jmms_ViewMsgFromFolder;

	if ((E_MSG_TYPE_NOTIFICATION == pstMsgDetail->pMessageHeaders->eMsgType) ||
		(E_MSG_TYPE_DELIVERY_REPORT == pstMsgDetail->pMessageHeaders->eMsgType) ||
		(E_MSG_TYPE_READ_REPORT_RESPONSE == pstMsgDetail->pMessageHeaders->eMsgType))
	{
		item_texts [iIndex] =  STR_ID_MMS_DELETE;
		sMMSOptionsLSKHandlers [iIndex++] =  jMms_delete_handler;
		item_texts [iIndex] =  STR_ID_MMS_DELETE_ALL;
		sMMSOptionsLSKHandlers [iIndex++] =  jMms_delete_all_handler;
		item_texts [iIndex] =  STR_ID_MMS_PROPERTY;
		sMMSOptionsLSKHandlers [iIndex++] =  jMMS_MsgPropertiesHandler;
	}
	else
	{
		item_texts [iIndex] =  STR_ID_MMS_REPLY;
		sMMSOptionsLSKHandlers [iIndex++] =  jmms_InboxReplyHandler;
		item_texts [iIndex] =  STR_ID_MMS_REPLY_ALL;
		sMMSOptionsLSKHandlers [iIndex++] =  jmms_InboxReplyAllHandler;
		item_texts [iIndex] =  STR_ID_MMS_FORWARD;
		sMMSOptionsLSKHandlers [iIndex++] =  jmms_InboxForwardHandler;
		item_texts [iIndex] =  STR_ID_MMS_DELETE;
		sMMSOptionsLSKHandlers [iIndex++] =  jMms_delete_handler;
		item_texts [iIndex] =  STR_ID_MMS_DELETE_ALL;
		sMMSOptionsLSKHandlers [iIndex++] =  jMms_delete_all_handler;
		item_texts [iIndex] =  STR_ID_MMS_PROPERTY;
		sMMSOptionsLSKHandlers [iIndex++] =  jMMS_MsgPropertiesHandler;
	}
	
	EntryNewScreen (SCR_ID_MMS_INBOX_OPTIONS_SCREEN, NULL, jMms_StoreInbox_handle_highlight_item, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_INBOX_OPTIONS_SCREEN) ;									
	RegisterHighlightHandler (jMms_StoreInboxMmsMsgHlthandler) ;
	ShowCategory15Screen (STR_ID_MMS_INBOX, IMG_SMS_ENTRY_SCRN_CAPTION,
							STR_GLOBAL_OK, 0,
							STR_GLOBAL_BACK, 0,
							iIndex, item_texts,
							(PU16)gIndexIconsImageList,
							1, 0, guiBuffer
						 	);
	}
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);	
}

void jMms_inbox_entry_handler(void)
{
	g_pstJMmsAppInfo->eCurrentFolderID = E_JMMS_INBOX_FOLDER ;
	jmms_DisplayMsgListInFolder(E_JMMS_INBOX_FOLDER);
}

/*Sent Folder*/
 void jMms_StoreSent_highlight_handler (int iSelectIndex)
{
	mmi_trace (1, "@@@ jMms_StoreSent_highlight_handler Index: %d @@@", iSelectIndex) ;
	switch (iSelectIndex)
	{
		case 0:
			SetLeftSoftkeyFunction (jmms_ViewMsgFromFolder,KEY_EVENT_UP) ;
			break ;
		case 1:
			SetLeftSoftkeyFunction (jmms_SentForwardHandler ,KEY_EVENT_UP) ;
			break ;
		case 2:
			SetLeftSoftkeyFunction (jmms_SentEditHandler ,KEY_EVENT_UP) ;
			break ;
		case 3:
			SetLeftSoftkeyFunction (jMms_delete_handler ,KEY_EVENT_UP) ;
			break ;
		case 4:
			SetLeftSoftkeyFunction (jMms_delete_all_handler ,KEY_EVENT_UP) ;
			break ;
		case 5:
			SetLeftSoftkeyFunction (jMMS_MsgPropertiesHandler ,KEY_EVENT_UP) ;
			break ;
		default:
			break ;
	}

 }

 void jMms_StoreSent_handle_highlight_item()
 {
	 U8* guiBuffer;
	 U16 item_texts[] = {STR_ID_MMS_VIEW_MSG, \
						 STR_ID_MMS_FORWARD,
						 STR_ID_MMS_EDIT_MESSAGE,
						 STR_ID_MMS_DELETE,
						 STR_ID_MMS_DELETE_ALL,
						 STR_ID_MMS_PROPERTY
					 	};

	 EntryNewScreen (SCR_ID_SENT_FOLDER_OPTION_SRCEEN, NULL, jMms_StoreSent_handle_highlight_item, NULL) ;
	 guiBuffer = GetCurrGuiBuffer (SCR_ID_SENT_FOLDER_OPTION_SRCEEN) ;
	 RegisterHighlightHandler (jMms_StoreSent_highlight_handler) ;
	 ShowCategory15Screen (STR_ID_MMS_SENT, IMG_SMS_ENTRY_SCRN_CAPTION,
						 STR_GLOBAL_OK, 0,
						 STR_GLOBAL_BACK, 0,
						 (sizeof (item_texts)/sizeof (U16)), item_texts,
						 (PU16)gIndexIconsImageList,
						 1, 0, guiBuffer);

	 SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);	 
 }

void jMms_sent_entry_handler(void)
{
	g_pstJMmsAppInfo->eCurrentFolderID = E_JMMS_SENT_FOLDER ;
	jmms_DisplayMsgListInFolder (E_JMMS_SENT_FOLDER) ;
}

pBOOL jmms_get_list_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{	
	if((item_index < 0) || ((JC_UINT32)item_index >= guiMsgListCount))
		return FALSE;
	
	jc_wstrcpy((UINT16*)str_buff,(UINT16*)(gpMsgSubList[item_index]));
	*img_buff_p =  (UI_image_type) GetImage(gpMsgIconList[item_index]);
	return TRUE;
}

void jmms_DisplayMsgListInFolder(E_JMMS_FOLDER_ID eFolderID)
{
	U8 *guiBuffer = NULL;
	JC_RETCODE rCode = JC_OK;
	U16 Foldertitle = 0;
	JC_UINT32 uiMsgListCount = jMMS_GetTotalNumberOfMsgsInFolder (eFolderID) ;

	mmi_trace(1, "jmms_DisplayMsgListInFolder is called and uiMsgListCount value is [%d]", uiMsgListCount) ;
	if(0 == uiMsgListCount)
	{
		DisplayPopup(GetString (STR_ID_MMS_FOLDER_EMPTY), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
	}
	else
	{
		switch(eFolderID)
		{
		case E_JMMS_INBOX_FOLDER:
			{
 				gNeedNewMMSPopOnIdle = E_FALSE;
				Foldertitle = STR_ID_MMS_INBOX ;				
				EntryNewScreen(SCR_ID_MMS_INBOX_SCREEN, \
					jmms_DisplayMsgListExitHandler, jMms_inbox_entry_handler, NULL);	
                            guiBuffer = GetCurrGuiBuffer(SCR_ID_MMS_INBOX_SCREEN);
				SetLeftSoftkeyFunction(jmms_InboxPrepareLSKHandler, KEY_EVENT_UP);				
			}
			break;
		case E_JMMS_OUTBOX_FOLDER:
			{
				Foldertitle = STR_ID_MMS_OUTBOX ;
				EntryNewScreen(SCR_ID_MMS_OUTBOX_SCREEN, \
					jmms_DisplayMsgListExitHandler, jMms_outbox_entry_handler, NULL);
                            guiBuffer = GetCurrGuiBuffer(SCR_ID_MMS_OUTBOX_SCREEN);
				SetLeftSoftkeyFunction(jmms_OutboxPrepareLSKHandler, KEY_EVENT_UP);				 
			}
			break;
		case E_JMMS_SENT_FOLDER:
			{
				Foldertitle = STR_ID_MMS_SENT;				
				EntryNewScreen(SCR_ID_MMS_SENT_SCREEN, \
					jmms_DisplayMsgListExitHandler, jMms_sent_entry_handler, NULL);
                            guiBuffer = GetCurrGuiBuffer(SCR_ID_MMS_SENT_SCREEN);
				SetLeftSoftkeyFunction(jmms_SentPrepareLSKHandler, KEY_EVENT_UP);
				
			}
			break;
		case E_JMMS_DRAFT_FOLDER:
			{
				Foldertitle = STR_ID_MMS_DRAFT ;				
				EntryNewScreen(SCR_ID_MMS_DRAFT_SCREEN, \
					jmms_DisplayMsgListExitHandler, jMms_draft_entry_handler, NULL);
                            guiBuffer = GetCurrGuiBuffer(SCR_ID_MMS_DRAFT_SCREEN);
				SetLeftSoftkeyFunction(jmms_DraftPrepareLSKHandler, KEY_EVENT_UP);				
			}
			break;
		case E_JMMS_NONE_FOLDER:
			/* The control shuld not come here */
			break ;
		}
		
		RegisterHighlightHandler(get_highlited_id);
		guiMsgListCount = uiMsgListCount;
		gpMsgSubList = jdd_MemAlloc(guiMsgListCount, sizeof(JC_CHAR *));
		gpMsgIconList = (U16 *)jdd_MemAlloc(guiMsgListCount, sizeof(U16));
		mmi_trace(1, "Current Folder is[%d]",eFolderID);		
		mmi_trace(1, "jmms_GetFolderMsgList rCode is [%d]",rCode);
		mmi_trace(1, "value is gpMsgSubList[%d], gpMsgIconList[%d],guiMsgListCount[%d]",\
						gpMsgSubList,gpMsgIconList,guiMsgListCount);

		if(guiBuffer)
		{//goback from history
			if(g_pstJMmsAppInfo->oldNumberOfMessages > guiMsgListCount)
			{//delete some message
				if(g_pstJMmsAppInfo->eCurrentFolderID >= guiMsgListCount)
				{//delete the last one, the new last one will be highlighted
					change_cat84_list_menu_history_highlighted_item(guiMsgListCount - 1, guiBuffer);
				}
			}
			else if(g_pstJMmsAppInfo->oldNumberOfMessages < guiMsgListCount)
			{//receive some new message, go down the number added
				change_cat84_list_menu_history_highlighted_item(g_pstJMmsAppInfo->iFolderItemIndex + (guiMsgListCount - g_pstJMmsAppInfo->oldNumberOfMessages), guiBuffer);
			}
		}
		g_pstJMmsAppInfo->oldNumberOfMessages = guiMsgListCount;
		jmms_GetFolderMsgList(gpMsgSubList , g_pstJMmsAppInfo->eCurrentFolderID);
		
		ShowCategory184Screen(Foldertitle, IMG_SMS_ENTRY_SCRN_CAPTION, \
								STR_GLOBAL_OPTIONS, 0, 
								STR_GLOBAL_BACK, 0, 
								guiMsgListCount, 
								jmms_get_list_item, 
								0, 0, guiBuffer);
	
		switch(eFolderID)
		{
			case E_JMMS_INBOX_FOLDER:
				{
					SetLeftSoftkeyFunction(jmms_InboxPrepareLSKHandler, KEY_EVENT_UP);
				}
				break;
			case E_JMMS_OUTBOX_FOLDER:
				{
					SetLeftSoftkeyFunction(jmms_OutboxPrepareLSKHandler, KEY_EVENT_UP);
				}
				break;
			case E_JMMS_SENT_FOLDER:
				{
					SetLeftSoftkeyFunction(jmms_SentPrepareLSKHandler, KEY_EVENT_UP);
				}
				break;
			case E_JMMS_DRAFT_FOLDER:
				{
					SetLeftSoftkeyFunction(jmms_DraftPrepareLSKHandler, KEY_EVENT_UP);
				}
				break;
			case E_JMMS_NONE_FOLDER:

			/* The control shuld not come here */
			break ;
		}
		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	}
}

void jmms_DisplayMsgListExitHandler()
{
	JC_UINT32 uiCnt;
	mmi_trace(1, "jmms_DisplayMsgListExitHandler msg count[%d]", guiMsgListCount);

	if(gpMsgSubList)
	{
		for ( uiCnt = 0 ; uiCnt < guiMsgListCount ; uiCnt++)
		{
		   if(gpMsgSubList[uiCnt])
		   	{
				jdd_MemFree(gpMsgSubList[uiCnt]);
		   	}
		}
		jdd_MemFree(gpMsgSubList);
		gpMsgSubList = NULL;
	}
	if(gpMsgIconList)
	{
		jdd_MemFree(gpMsgIconList);
		gpMsgIconList = NULL;
	}
	guiMsgListCount =0;
	mmi_trace(1, "jmms_DisplayMsgListExitHandler is calling finish");
}

/*OutBox*/
extern U16 g_MMS_send_require;
void jMms_Highlight_Resend_Send(void)
{
	g_MMS_send_require = SEND_ONLY;
	#ifdef __MMI_MULTI_SIM__
	SetLeftSoftkeyFunction (MTPNP_mms_resend_select_sim_entry_handler, KEY_EVENT_UP) ;
	#else
	SetLeftSoftkeyFunction (jMms_ReSend_handler, KEY_EVENT_UP) ;
	#endif
}
void jMms_Highlight_Resend_Send_And_Save(void)
{
	g_MMS_send_require = SEND_AND_SAVE;
	#ifdef __MMI_MULTI_SIM__
	SetLeftSoftkeyFunction (MTPNP_mms_resend_select_sim_entry_handler, KEY_EVENT_UP) ;
	#else
	SetLeftSoftkeyFunction (jMms_ReSend_handler, KEY_EVENT_UP) ;
	#endif
}

void jMms_OutBox_ReSend_Option(void)
{
	U8* guiBuffer;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
	
	mmi_trace(1,"JDD_LOG: jMms_OutBox_ReSend_Option");
	EntryNewScreen (SCR_ID_MMS_RESEND_OPTION, NULL, jMms_OutBox_ReSend_Option, NULL) ;

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_RESEND_OPTION);
	
	numItems = GetNumOfChild_Ext(MMS_OUTBOX_RESEND_OPTION);
	GetSequenceStringIds_Ext(MMS_OUTBOX_RESEND_OPTION, nStrItemList);	

	SetParentHandler(MMS_OUTBOX_RESEND_OPTION);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler) ;
	
	ShowCategory15Screen (STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION, 
							STR_GLOBAL_OK, 0, 
							STR_GLOBAL_BACK, 0,
							numItems, nStrItemList, (PU16)gIndexIconsImageList, 
							1 , 0, guiBuffer);	
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}
void jMms_StoreOutBox_highlight_handler (int iSelectIndex)
{
 mmi_trace (1, "@@@ jMms_StoreOutBox_highlight_handler  Index: %d @@@", iSelectIndex) ;
 switch (iSelectIndex)
 {
	 case 0:
		 SetLeftSoftkeyFunction (jmms_ViewMsgFromFolder,KEY_EVENT_UP) ;
		 break ;
	 case 1:
		 SetLeftSoftkeyFunction (jmms_OutboxForwardHandler ,KEY_EVENT_UP) ;
		 break ;
	 case 2:
		SetLeftSoftkeyFunction (jmms_OutboxEditHandler ,KEY_EVENT_UP) ;
		 break ;
	 case 3:
	 	SetLeftSoftkeyFunction (jMms_OutBox_ReSend_Option ,KEY_EVENT_UP) ;
		break ;
	 case 4:
		 SetLeftSoftkeyFunction (jMms_delete_handler ,KEY_EVENT_UP) ;
		 break ;
	 case 5:
		 SetLeftSoftkeyFunction (jMms_delete_all_handler ,KEY_EVENT_UP) ;
		 break ;
	 case 6:
		 SetLeftSoftkeyFunction (jMMS_MsgPropertiesHandler ,KEY_EVENT_UP) ;
		 break ;
	 default:
		 break ;
 }
}

void jmms_SentPrepareLSKHandler()
{
	MESSAGE_LIST		*pstMsgDetail = NULL ;
	
	jmms_GetMsgDetailsByIndex (g_pstJMmsAppInfo->hMsgStore, g_pstJMmsAppInfo->eCurrentFolderID,
										g_pstJMmsAppInfo->iFolderItemIndex, &pstMsgDetail);

	g_pstJMmsAppInfo->uiHighlightedMsgId = pstMsgDetail->uiMessageID;
	jMms_StoreSent_handle_highlight_item();	
}

void jmms_OutboxPrepareLSKHandler()
{
	MESSAGE_LIST		*pstMsgDetail = NULL ;
	
	jmms_GetMsgDetailsByIndex (g_pstJMmsAppInfo->hMsgStore, g_pstJMmsAppInfo->eCurrentFolderID,
										g_pstJMmsAppInfo->iFolderItemIndex, &pstMsgDetail);

	g_pstJMmsAppInfo->uiHighlightedMsgId = pstMsgDetail->uiMessageID;
	jMms_StoreOutbox_handle_highlight_item();	
}

void jmms_DraftPrepareLSKHandler()
{
	MESSAGE_LIST		*pstMsgDetail = NULL ;
	
	jmms_GetMsgDetailsByIndex (g_pstJMmsAppInfo->hMsgStore, g_pstJMmsAppInfo->eCurrentFolderID,
										g_pstJMmsAppInfo->iFolderItemIndex, &pstMsgDetail);

	g_pstJMmsAppInfo->uiHighlightedMsgId = pstMsgDetail->uiMessageID;
	jMms_StoreDraft_handle_highlight_item();	
}


void jMms_StoreOutbox_handle_highlight_item()
 {
	 U8* guiBuffer;
	 U16 item_texts[] = {STR_ID_MMS_VIEW_MSG, \
						 STR_ID_MMS_FORWARD,
						 STR_ID_MMS_EDIT_MESSAGE,
						 STR_ID_MMS_RESEND,
						 STR_ID_MMS_DELETE,
						 STR_ID_MMS_DELETE_ALL,
						 STR_ID_MMS_PROPERTY
						 };

	 EntryNewScreen (SCR_ID_MMS_OUTBOX_OPTION_SRCEEN, NULL, jMms_StoreOutbox_handle_highlight_item, NULL) ;
	 guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_OUTBOX_OPTION_SRCEEN) ;
	 RegisterHighlightHandler (jMms_StoreOutBox_highlight_handler) ;
	 ShowCategory15Screen (STR_ID_MMS_OUTBOX, IMG_SMS_ENTRY_SCRN_CAPTION,
							 STR_GLOBAL_OK, 0,
							 STR_GLOBAL_BACK, 0,
							 (sizeof (item_texts)/sizeof (U16)), item_texts,
							 (PU16)gIndexIconsImageList,
							 1, 0, guiBuffer);

	 SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);	 
 }

void jMms_outbox_entry_handler(void)
{
	g_pstJMmsAppInfo->eCurrentFolderID = E_JMMS_OUTBOX_FOLDER ;
	jmms_DisplayMsgListInFolder (E_JMMS_OUTBOX_FOLDER) ;
}

/*Draft*/
 void jMms_StoreDraft_highlight_handler(int iSelectIndex)
{
 mmi_trace (1, "@@@ jMms_StoreDraft_highlight_handlern @@@") ;
 mmi_trace (1, "@@@ Index: %d @@@", iSelectIndex) ;
 switch (iSelectIndex)
 {
	 case 0:
		 SetLeftSoftkeyFunction (jmms_ViewMsgFromFolder,KEY_EVENT_UP) ;
		 break ;
	 case 1:
		 SetLeftSoftkeyFunction (jmms_DraftEditHandler ,KEY_EVENT_UP) ;
		 break ;
	 case 2:
		SetLeftSoftkeyFunction (jMms_delete_handler,KEY_EVENT_UP) ;
		 break ;
	 case 3:
		 SetLeftSoftkeyFunction (jMms_delete_all_handler,KEY_EVENT_UP) ;
		 break ;
	 case 4:
		 SetLeftSoftkeyFunction (jMMS_MsgPropertiesHandler,KEY_EVENT_UP) ;
		 break ;
	 default:
		 break ;
 }

}

 void jMms_StoreDraft_handle_highlight_item()
 {
	 U8* guiBuffer;
	 U16 item_texts[] = {STR_ID_MMS_VIEW_MSG,
						 STR_ID_MMS_EDIT_MESSAGE,
						 STR_ID_MMS_DELETE,
						 STR_ID_MMS_DELETE_ALL,
						 STR_ID_MMS_PROPERTY
						 };

	 EntryNewScreen (SCR_ID_MMS_FOLDER_DRAFT_OPTION_SRCEEN, NULL, jMms_StoreDraft_handle_highlight_item, NULL) ;
	 guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_FOLDER_DRAFT_OPTION_SRCEEN) ;
	 RegisterHighlightHandler (jMms_StoreDraft_highlight_handler) ;
	 ShowCategory15Screen (STR_ID_MMS_DRAFT, IMG_SMS_ENTRY_SCRN_CAPTION,
						 STR_GLOBAL_OK, 0,
						 STR_GLOBAL_BACK, 0,
						 (sizeof (item_texts)/sizeof (U16)),
						 item_texts, (PU16)gIndexIconsImageList,
						 1, 0, guiBuffer);

	 SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);	 
 }

void jMms_draft_entry_handler(void)
{
	g_pstJMmsAppInfo->eCurrentFolderID = E_JMMS_DRAFT_FOLDER ;
	jmms_DisplayMsgListInFolder (E_JMMS_DRAFT_FOLDER) ;
}

JC_RETCODE jmms_GetFolderMsgList (U8 **pFolderMessageListItems, E_JMMS_FOLDER_ID eFolderID)
{
	MESSAGE_LIST		*pMsglist = JC_NULL ;
	JC_RETCODE			rCode = JC_OK ;
	RETRIEVE_FILTERS	retFilter = {0,} ;
	MESSAGE_LIST		*temp_message_list = JC_NULL ;
	MESSAGE_SEND		*pHeader = {0,} ;
	JC_UINT32			uiItemIndex = 0 ;

	rCode = jmms_SetActiveFolder (eFolderID) ;
	if (JC_OK == rCode)
	{
		retFilter.eRetrieve = E_RETRIEVE_BY_INDEX ;
		retFilter.uiStartIndex = 1 ;
		retFilter.uiNumMessage = 0 ;
		rCode = jmms_GetMsgList (g_pstJMmsAppInfo->hMsgStore, &pMsglist, &retFilter) ;
		mmi_trace(1, "JDD_LOG: jmms_GetFolderMsgList >> jmms_GetMsgList rCode value is [%d]", rCode) ;
	}
	if (JC_OK == rCode)
	{
		temp_message_list = pMsglist ;
		while (JC_NULL != temp_message_list)
		{
			if(eFolderID == E_JMMS_INBOX_FOLDER)
			{
				if(temp_message_list->bReadFlag == E_TRUE)
				{
				#ifdef __MMI_MULTI_SIM__
					#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
					gpMsgIconList[uiItemIndex] = IMG_MMS_INBOX_FOLDER_READ_ICON ;
					#else
					gpMsgIconList[uiItemIndex] = IMG_MMS_INBOX_FOLDER_SIM1_READ_ICON + temp_message_list->uiExtraBytes;
					#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
				#else
					gpMsgIconList[uiItemIndex] = IMG_MMS_INBOX_FOLDER_READ_ICON ;
				#endif
				}
				else
				{
				#ifdef  __MMI_MULTI_SIM__
					#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
						gpMsgIconList[uiItemIndex] = IMG_MMS_INBOX_FOLDER_UNREAD_ICON ;
					#else
					gpMsgIconList[uiItemIndex] = IMG_MMS_INBOX_FOLDER_SIM1_UNREAD_ICON + temp_message_list->uiExtraBytes;
					#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
				#else
					gpMsgIconList[uiItemIndex] = IMG_MMS_INBOX_FOLDER_UNREAD_ICON ;
				#endif
				}
			}
			if(eFolderID == E_JMMS_OUTBOX_FOLDER)
			{
				gpMsgIconList[uiItemIndex] = IMG_MMS_OUTBOX_FOLDER_ICON ;
			}
			if(eFolderID == E_JMMS_SENT_FOLDER)
			{
			#ifdef  __MMI_MULTI_SIM__
				#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
		        	gpMsgIconList[uiItemIndex] = IMG_MMS_SENT_FOLDER_ICON ;
				#else
		        	gpMsgIconList[uiItemIndex] = IMG_MMS_SENT_FOLDER_SIM1_ICON + temp_message_list->uiExtraBytes;
				#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
			#else
				gpMsgIconList[uiItemIndex] = IMG_MMS_SENT_FOLDER_ICON ;
			#endif
			}
			if(eFolderID == E_JMMS_DRAFT_FOLDER)
			{
				gpMsgIconList[uiItemIndex] = IMG_MMS_DRAFT_FOLDER_ICON ;
			}

			if(temp_message_list->pMessageHeaders->eMsgType == E_MSG_TYPE_READ_REPORT_RESPONSE)
			{				
				pFolderMessageListItems[uiItemIndex] = (U8*)jdi_CUtilsTcsDuplicate(NULL, \
										(JC_CHAR*) GetString(STR_ID_MMS_READ_REPORT));
			}
			else if(temp_message_list->pMessageHeaders->eMsgType == E_MSG_TYPE_DELIVERY_REPORT)
			{
				pFolderMessageListItems[uiItemIndex] = (U8*)jdi_CUtilsTcsDuplicate(NULL, \
										(JC_CHAR*) GetString(STR_ID_MMS_DELIVERY_REPORT));
			}
			else if(temp_message_list->pMessageHeaders->eMsgType == E_MSG_TYPE_NOTIFICATION)
			{
				pFolderMessageListItems[uiItemIndex] = (U8*)jdi_CUtilsTcsDuplicate(NULL, \
										(JC_CHAR*) GetString(STR_ID_MMS_MMS_NOTIFICATION));
			}
			else
			{
				pHeader = (MESSAGE_SEND*) temp_message_list->pMessageHeaders->pHeaders;
				if(pHeader && NULL != pHeader->pSubject)
				{
					pFolderMessageListItems[uiItemIndex] = (U8*)jdi_CUtilsTcsDuplicate(NULL,pHeader->pSubject);					
				}
				else
				{
					pFolderMessageListItems[uiItemIndex] = (U8*)jdi_CUtilsTcsDuplicate(NULL, \
										(JC_CHAR*) GetString(STR_ID_MMS_NO_SUBJECT));
				}
			}			
			uiItemIndex++;
			temp_message_list = temp_message_list->pNext;
		}
	}
	return rCode;
}

#endif //__JATAAYU_APP__

