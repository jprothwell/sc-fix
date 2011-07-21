#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_composedlg.c"
 /***************************************************************************
 *
 * File Name : jmms_composedlg.c
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
 * @file		jmms_composedlg.c
 * @ingroup		jMMS
 * @brief		Describes the MMI Compose related API's.
 *
 **/
 
 #ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/


#include "jmms_inc.h"

#include "jmms_cb.h"
#include "jmms_addrdlg.h"
#include "jmms_composeutils.h"
#include "jmms_composedlg.h"
#include "jmms_objectpicker.h"
#include "jmms_storedlg.h"
#include "messagesresourcedata.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
#include "settingprofile.h"
 /***************************************************************************
 * Macros
 **************************************************************************/
#define MAX_ATTACHMET_NUMBER (10)
/***************************************************************************
* Global variables declarations
**************************************************************************/
INT g_iSelectIndex ;
JC_UINT8	g_uiAttachListCount ;
JC_UINT8	**g_pListItems ;
JC_UINT8	g_uiMediaListCount;
JC_UINT8	**g_pMediaListItems;

/***************************************************************************
* Local Function Prototypes
***************************************************************************/

/***************************************************************************
* All Local Function Definitions
***************************************************************************/

/***************************************************************************
* All Global Function Definitions
**************************************************************************/

void jmms_ComposeEditTextDone()
{
	JC_CHAR *psInputValue = JC_NULL ;
	JC_RETCODE rCode = JC_OK ;
	
	jMMS_MMIGetInputBoxValue (&psInputValue); 
	if(psInputValue)
	{		
		rCode = jmms_AddTextMediaToCurrSlide(psInputValue);	
	}	
	jMMS_HandleTextMediaError(rCode) ;	
}

JC_RETCODE jmms_AddTextMediaToCurrSlide(JC_CHAR *psTextBuff)
{
	JC_RETCODE rCode = JC_OK;
	
	if(g_pstJMmsAppInfo->ActivePageMediaIds[0])
	{
		rCode = jdi_MsgUpdateInlineText(g_pstJMmsAppInfo->hCurrMsg, (JC_UINT8 *)psTextBuff, 
										jc_tcslen(psTextBuff)*sizeof(JC_CHAR), g_pstJMmsAppInfo->ActivePageIndex, 
										g_pstJMmsAppInfo->ActivePageMediaIds[0]) ;
		if(JC_OK == rCode)
		{
			jc_tcscpy(g_pstJMmsAppInfo->pActiveTextBuffer,psTextBuff) ;
		}
		else
		{			
			DisplayPopup(GetString(STR_ID_MMS_MESSAGE_SIZE_EXCEED), IMG_GLOBAL_INFO, 1, 2000, 1) ;
		}
	}
	else
	{
		rCode = AddTextMedia(psTextBuff,g_pstJMmsAppInfo->ActivePageIndex) ;
	}
	return rCode;
}

void jmms_ComposeEditTextCancelHandle()
{
	DeleteNHistory(1) ;
	GoBackHistory();
}

void jmms_ComposeEditTextOptionHltHandler (S32 index)
{
	switch(index)
	{
	case 0:
		SetLeftSoftkeyFunction (jmms_ComposeEditTextDone, KEY_EVENT_UP ) ;
		break;
		
	case 1:
		SetLeftSoftkeyFunction (jmms_ComposeEditTextCancelHandle, KEY_EVENT_UP ) ;
		break;

	case 2:
		SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP ) ;
		break ;
	}	
}

void jmms_ComposeEditTextLSKhandler ()
{
	U16 item_texts[] = {STR_GLOBAL_DONE, \
						STR_GLOBAL_CANCEL, \
						STR_GLOBAL_INPUT_METHOD} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, \
						IMG_GLOBAL_L3} ;
	
	EntryNewScreen (SCR_ID_MMS_EDITTEXT_OPTION_SCREEN, 0, 0, 0) ;
	RegisterHighlightHandler (jmms_ComposeEditTextOptionHltHandler);
	ShowCategory52Screen (STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION, 
						STR_GLOBAL_OK, 0,
						STR_GLOBAL_BACK, 0, 
						3, item_texts, item_icons,
						0, 0, 0, 0);	
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP );
}

extern void GBSetInputboxLSKFunction(FuncPtr f);

void jmms_ComposeEditTextEntryHandler()
{
	jc_tcscpy((UINT16*)g_pstJMmsAppInfo->pcInputBuff, \
					(UINT16*)g_pstJMmsAppInfo->pActiveTextBuffer) ;
	jmms_ComposeEditTextHandler();
}

void jmms_ComposeEditTextHandler()

{
	U8* guiBuffer = JC_NULL;
	
	EntryNewScreen (SCR_ID_MMS_COMPOSE_EDIT_TEXT_SCREEN, JC_NULL, \
					jmms_ComposeEditTextHandler, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_COMPOSE_EDIT_TEXT_SCREEN);	
	ShowCategory5Screen (STR_ID_MMS_EDIT_TEXT, IMG_SMS_ENTRY_SCRN_CAPTION,
						STR_GLOBAL_OPTIONS, 0,
						STR_GLOBAL_BACK, 0,
						INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
						(U8*)jMms_getInputBuff(), jMMS_MAX_INPUT_SIZE + 1, //256 char
						guiBuffer);
	
#ifdef __MMI_GB__
		GBSetInputboxLSKFunction (jmms_ComposeEditTextLSKhandler);
#endif	
	SetLeftSoftkeyFunction (jmms_ComposeEditTextLSKhandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistory );
	
}

/*To Be removed*/
void jMMS_POPupMessage(JC_INT8 *pcMessage)
{
	JC_RETCODE rCode = 0 ;
	JC_CHAR *pmDest = JC_NULL ; 
	
	rCode = jdi_CUtilsCharToTchar (JC_NULL,pcMessage, &pmDest) ;
	if(JC_OK == rCode && pmDest)
	{
		DisplayPopup((U8*)pmDest, IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
		jdd_MemFree(pmDest) ;
	}
}
void Go_to_slide_screen()
{
	JC_RETCODE rCode = JC_OK;
	JC_UINT8 ucTotalPages;
	
	rCode = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucTotalPages);

	if (0 == ucTotalPages)
	{
		GoBackToHistory(SCR_ID_MMS_ADDRESS_SCREEN);
	}
	else
	{
		GoBackToHistory(SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN);
	}
}

void Go_to_slide_screen_cb()
{
	JC_RETCODE rCode = JC_OK;
	JC_UINT8 ucTotalPages;
	
	rCode = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucTotalPages);

	if (0 == ucTotalPages)
	{
		DeleteUptoScrID(SCR_ID_MMS_ADDRESS_SCREEN);
	}
	else
	{
		DeleteUptoScrID(SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN);
	}
}

void jMms_AddNewSlide_entry_handler ()
{
	JC_RETCODE rCode ;
	
	rCode = jmms_MMIAddPage();
	if(JC_OK != rCode  )
	{		
		DisplayPopup(GetString (STR_ID_MMS_MAX_LIMIT), IMG_GLOBAL_INFO,
								1, 1500, 1);
		Go_to_slide_screen_cb();
	}
	else
	{			
		DisplayPopup(GetString (STR_ID_MMS_INFO_NEW_SLIDE), IMG_GLOBAL_INFO,
								1, 1500, 1);
		Go_to_slide_screen_cb();
	}
}

void jMMS_Replace_Image_Media()
{	
	ImagePicker(E_MEDIA_IMAGE) ;
}

void jMMS_Replace_Video_Media()
{	
	ImagePicker(E_MEDIA_VIDEO) ;
}


void jMMS_Replace_Audio_Media()
{	
	ImagePicker(E_MEDIA_AUDIO) ;
}

void jMMS_Replace_Other_Media()
{	
	JC_RETCODE rCode = JC_OK;
	if (E_FALSE == g_pstJMmsAppInfo->bIsViolatingContentClass)
	{
		rCode = jdi_MsgDeleteMedia(g_pstJMmsAppInfo->hCurrMsg,g_pstJMmsAppInfo->ActivePageIndex,
									g_pstJMmsAppInfo->ActivePageMediaIds[4],0);
		g_pstJMmsAppInfo->ActivePageMediaIds[4] = 0;
		ImagePicker(E_MEDIA_OTHERS) ;
	}
	else
	{
		rCode = jdi_MsgAsyncErrorIndCBHandler (g_pstJMmsAppInfo->hCurrMsg,
											E_WARNING_CREATION_MODE_ADDMEDIA,
											JC_OK, JC_NULL) ;
		g_pstJMmsAppInfo->bIsViolatingContentClass = E_FALSE ;
		jMMS_HandleMediaError (rCode) ;
	}
}

void JMMS_Cancel_Selection()
{
	JC_RETCODE rCode ;
	if (E_FALSE == g_pstJMmsAppInfo->bIsViolatingContentClass)
	{
            GoBackHistory () ;
	}
	else
	{
		g_pstJMmsAppInfo->bIsViolatingContentClass = E_FALSE ;
		
		rCode = jdi_MsgAsyncErrorIndCBHandler (g_pstJMmsAppInfo->hCurrMsg,
											E_WARNING_CREATION_MODE_ADDMEDIA,
											JC_ERR_USER_CANCELLED, JC_NULL) ;
            jMMS_HandleMediaError (rCode) ;		
            DeleteScreenIfPresent(SCR_CONFIRM_SCREEN);
	}
}

void jmms_ShowConfirmationForRestrictedMediaAdd (void)
{
	jMMS_GetUserConfirmation (GetString(STR_ID_MMS_MSG_RESTRICTED), jMMS_Replace_Other_Media, JMMS_Cancel_Selection, jmms_ShowConfirmationForRestrictedMediaAdd) ;	
}

void jMms_ComposeInsertAudio_entry_handler (void)
{	
	mmi_trace (1, "@@@ jMms_ComposeInsertAudio_entry_handler @@@") ;
	if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[2] ||
		0 != g_pstJMmsAppInfo->ActivePageMediaIds[3])
	{	
		if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[3])
		{
			DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_ID_MMS_CNF_VIDEO_ADD), 0, 1) ;
		}
		else
		{
			DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_ID_MMS_CNF_IMAGE_ADD), 0, 1) ;				
		}
		SetLeftSoftkeyFunction(jMMS_Replace_Audio_Media,KEY_EVENT_UP) ;
		SetRightSoftkeyFunction(JMMS_Cancel_Selection,KEY_EVENT_UP) ;
	}
	else
	{
		ImagePicker(E_MEDIA_AUDIO) ;
	}
	mmi_trace (1, "@@@ jMms_ComposeInsertAudio_entry_handler Exit@@@") ;
}

void jMms_ComposeInsertImage_entry_handler (void)
{	
	mmi_trace (1, "@@@ jMms_ComposeInsertImage_entry_handler @@@") ;
	if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[1] ||
		0 != g_pstJMmsAppInfo->ActivePageMediaIds[3])
	{
		if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[3])
		{
			DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_ID_MMS_CNF_VIDEO_ADD), 0, 1) ;
		}
		else
		{
			DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_ID_MMS_CNF_IMAGE_ADD), 0, 1) ;				
		}
		SetLeftSoftkeyFunction(jMMS_Replace_Image_Media,KEY_EVENT_UP) ;
		SetRightSoftkeyFunction(JMMS_Cancel_Selection,KEY_EVENT_UP) ;
	}
	else
	{
		ImagePicker(E_MEDIA_IMAGE) ;
	}
	mmi_trace (1, "@@@ jMms_ComposeInsertImage_entry_handler Exit@@@") ;
}

void jMms_ComposeInsertVideo_entry_handler (void)
{	
	mmi_trace (1, "@@@ jMms_ComposeInsertVideo_entry_handler @@@") ;
	if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[1] ||
		0 != g_pstJMmsAppInfo->ActivePageMediaIds[2] ||
		0 != g_pstJMmsAppInfo->ActivePageMediaIds[3])
	{
	    if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[1] && 
			0 != g_pstJMmsAppInfo->ActivePageMediaIds[2])
		{
			DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_ID_MMS_CNF_IMAGE_AUDIO_ADD), 0, 1) ;
		}
		else if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[1])
		{
			DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_ID_MMS_CNF_IMAGE_ADD), 0, 1) ;
		}
		else if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[2])
		{
			DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_ID_MMS_CNF_AUDIO_ADD), 0, 1) ;
		}
		else
		{
			DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_ID_MMS_CNF_VIDEO_ADD ), 0, 1) ;
		}
		
		SetLeftSoftkeyFunction(jMMS_Replace_Video_Media,KEY_EVENT_UP) ;
		SetRightSoftkeyFunction(JMMS_Cancel_Selection,KEY_EVENT_UP) ;
	}
	else
	{
		ImagePicker(E_MEDIA_VIDEO) ;
	}
	mmi_trace (1, "@@@ jMms_ComposeInsertVideo_entry_handler Exit@@@") ;
}


void jMms_ComposeFormAttachmentList (JC_UINT8	**pListItems, \
										MEDIA_INFO *pMediaList, JC_UINT8 uiAttachListCount)
{
	JC_INT32 	iIndex ;	
	for (iIndex = 0; iIndex < uiAttachListCount; iIndex++)
	{		
		/*Getting File name ignoring bIsStream member */
		pListItems [iIndex] = (JC_UINT8*) pMediaList->pMimeInfo->MimeHeaders.pObjectName;
		pMediaList = pMediaList->pNext ;
	}	
}

void jMms_composeAddAttachment ()
{
	ImagePicker (E_MEDIA_OTHERS) ;
}

void jMms_composeDeleteAttachment ()
{
	JC_RETCODE rCode = JC_OK;
	MEDIA_INFO	*pMediaList = JC_NULL ;
	JC_UINT8	ucMediaCount = 0 ;
	JC_UINT8	uiCounter = 0 ;

	rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, 0, &pMediaList, \
								&ucMediaCount, E_TRUE) ;
	
	while(uiCounter != g_iSelectIndex)
	{
		pMediaList = pMediaList->pNext ;
		uiCounter++ ;
	}
	if(JC_OK == rCode)
	{
		rCode = jdi_MsgDeleteMedia (g_pstJMmsAppInfo->hCurrMsg, 0, 
									pMediaList->uiMediaID, E_TRUE) ;
	}
	if (rCode == JC_OK)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, \
			1, 2000, 1);		
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, \
			1, 2000, 1);			
	}
	GoBackToHistory(SCR_ID_MMS_ATTACHMENT);
}

void jMms_composeDeleteAttachmentConfirmation ()
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						 (UI_string_type)GetString(STR_GLOBAL_DELETE), IMG_GLOBAL_QUESTION, WARNING_TONE); 
	
	SetLeftSoftkeyFunction(jMms_composeDeleteAttachment,KEY_EVENT_UP);	
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void HandleClearAllAttachYes ()
{	
	JC_RETCODE iErr ;
	MEDIA_INFO	*pMediaList = JC_NULL ;
	JC_UINT8	ucMediaCount = 0, ucIndex ;
	JC_UINT32 * puiAttMediaList = NULL;

	iErr = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, 0, &pMediaList, \
								&ucMediaCount, E_TRUE) ;
	
	if (iErr== JC_OK && ucMediaCount)
	{
		puiAttMediaList = jdd_MemAlloc(ucMediaCount,sizeof(JC_UINT32));

		for (ucIndex = 0 ; pMediaList != NULL ; ucIndex++)
		{
				puiAttMediaList[ucIndex] = pMediaList->uiMediaID;
				pMediaList = pMediaList->pNext;
		}
		for ( ucIndex = 0; ucIndex < ucMediaCount; ucIndex++)
		{
			if(puiAttMediaList[ucIndex] != 0)
			{
				iErr = jdi_MsgDeleteMedia  (g_pstJMmsAppInfo->hCurrMsg, 0, \
											puiAttMediaList[ucIndex], E_TRUE) ;			
			}
			else
			{
				iErr = JC_ERR_GENERAL_FAILURE;
			}
			if (iErr)
			{
				break;				
			}
		}
		jdd_MemFree(puiAttMediaList);
	}
	if (iErr == JC_OK)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, \
			1, 2000, 1);	
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, \
			1, 2000, 1);
	}
	GoBackToHistory(SCR_ID_MMS_ATTACHMENT);
}

void HandleClearAllAttachNo ()
{
	GoBackHistory () ;
}
void jMms_composeDeleteAllAttachment ()
{
	DisplayConfirm (STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, \
			IMG_GLOBAL_NO, (UI_string_type)get_string(STR_GLOBAL_DELETE_ALL), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction (HandleClearAllAttachYes,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (HandleClearAllAttachNo,KEY_EVENT_UP) ;	
}

void jMms_Attachment_Add_Higglighthandler(void)
{
	SetLeftSoftkeyFunction (jMms_composeAddAttachment, KEY_EVENT_UP) ;
}
void jMms_Attachment_Delete_Higglighthandler(void)
{
	SetLeftSoftkeyFunction (jMms_composeDeleteAttachmentConfirmation, KEY_EVENT_UP) ;
}
void jMms_Attachment_DeleteAll_Higglighthandler(void)
{
	SetLeftSoftkeyFunction (jMms_composeDeleteAllAttachment, KEY_EVENT_UP) ;
}
void jMms_composeAttachmentOptions ()
{
	U8* guiBuffer ; 
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
	
	EntryNewScreen (SCR_ID_MMS_ATTACHMENT_OPTIONS, NULL, NULL, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_ATTACHMENT_OPTIONS) ;	

	if(g_uiAttachListCount < MAX_ATTACHMET_NUMBER)
	{
		mmi_frm_unhide_menu_item(MMS_ATTACHMENT_ADD);
	}
	else
	{
		mmi_frm_hide_menu_item(MMS_ATTACHMENT_ADD);
	}
	
	numItems = GetNumOfChild_Ext(MMS_ATTACHMENT_OPT);
	GetSequenceStringIds_Ext(MMS_ATTACHMENT_OPT, nStrItemList);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler) ;
	SetParentHandler(MMS_ATTACHMENT_OPT);
	ShowCategory15Screen (STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION, 
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						numItems, nStrItemList, (PU16)gIndexIconsImageList, 
						1, 0, guiBuffer) ;
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);	
}

	
void jMms_composeAttachmentHighlightHandler (int iSelectIndex)
{
	g_iSelectIndex = iSelectIndex ;
}

void jMms_composeAttachmentBackHandler ()
{	
	GoBackHistory () ;
}

void jmms_FreeAttachmentListMemory()
{
	if(g_pListItems)
	{		
		jdd_MemFree(g_pListItems);
		g_pListItems = JC_NULL;
	}
}

void jMms_ComposeInsertAttachment_exit_handler()
{
	jmms_FreeAttachmentListMemory();
}

void jMms_ComposeInsertAttachment_entry_handler (void)
{	
	U8 			*guiBuffer ;	
	JC_UINT8	uiAttachListCount ;
	JC_RETCODE	rCode = JC_OK;
	MEDIA_INFO	*pMediaList = JC_NULL ;
	
	rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, 0, &pMediaList, \
				&uiAttachListCount, E_TRUE) ;

	
	EntryNewScreen (SCR_ID_MMS_ATTACHMENT, jMms_ComposeInsertAttachment_exit_handler, \
					jMms_ComposeInsertAttachment_entry_handler, NULL) ;
	
	RegisterHighlightHandler (jMms_composeAttachmentHighlightHandler) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_ATTACHMENT) ;
			
	if(0 < uiAttachListCount)
	{
		g_uiAttachListCount = uiAttachListCount;
		g_pListItems = jdd_MemAlloc(uiAttachListCount, sizeof(JC_UINT8*));
	
		jMms_ComposeFormAttachmentList (g_pListItems, pMediaList, uiAttachListCount) ;	
	
		ShowCategory84Screen(STR_ID_MMS_ADD_ATTACHMENT, IMG_SMS_ENTRY_SCRN_CAPTION,
					         STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
					         STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					         uiAttachListCount, (U8**)g_pListItems,
							 (PU16)gIndexIconsImageList, LIST_MENU,
							 0, guiBuffer);
		
		SetLeftSoftkeyFunction (jMms_composeAttachmentOptions, KEY_EVENT_UP);
	}
	else
	{
		ShowCategory84Screen(STR_ID_MMS_ADD_ATTACHMENT,  IMG_SMS_ENTRY_SCRN_CAPTION,
							STR_GLOBAL_ADD, 0,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							0, 0,
							NULL, LIST_MENU,
							0, guiBuffer) ;
		
		SetLeftSoftkeyFunction(jMms_composeAddAttachment, KEY_EVENT_UP);
	}
	SetRightSoftkeyFunction(jMms_composeAttachmentBackHandler, KEY_EVENT_UP);	
}

JC_RETCODE jmms_MMIViewMmsPlaySlide (JC_UINT8 PageIndex, JC_BOOLEAN bPlayPresentation)
{
	JC_RETCODE	eRet	= JC_OK ;
	EVENT_SET_ACTIVE_SLIDE SetSlidedata = {0} ;

	SetSlidedata.ucPageIndex = PageIndex;
	SetSlidedata.bPlayPresentation = bPlayPresentation;	

	eRet = jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg, 
						E_VIEW_SET_ACTIVE_SLIDE, (void*)(&SetSlidedata));
	return eRet;
}
void jMms_ComposeDeleteImage_confirm(void)
{
	jMMS_GetUserConfirmation(GetString(STR_ID_MMS_CNF_DELETE), \
		jMms_DeleteImage_Msg,Go_to_slide_screen,jMms_ComposeDeleteImage_confirm) ;
}

void jMms_ComposeDeleteVideo_confirm(void)
{
	jMMS_GetUserConfirmation(GetString(STR_ID_MMS_CNF_DELETE), \
		jMms_DeleteVideo_Msg,Go_to_slide_screen,jMms_ComposeDeleteVideo_confirm) ;
}

void jMms_ComposeDeleteImage_handler()
{
	if(0 == g_pstJMmsAppInfo->ActivePageMediaIds[1])
	{		
		DisplayPopup(GetString (STR_ID_MMS_INFO_IMAGE_DELETE), IMG_GLOBAL_INFO,
								1, 2000, 1);
		Go_to_slide_screen_cb();
	}
	else
	{		
		jMms_ComposeDeleteImage_confirm();
	}
}

void jMms_ComposeDeleteAudio_confirm(void)
{
	jMMS_GetUserConfirmation(GetString(STR_ID_MMS_CNF_DELETE),jMms_DeleteAudio_Msg,Go_to_slide_screen,jMms_ComposeDeleteAudio_confirm) ;

}
void jMms_ComposeDeleteAudio_handler()
{
	if(JC_NULL == g_pstJMmsAppInfo->ActivePageMediaIds[2])
	{		
		DisplayPopup(GetString (STR_ID_MMS_INFO_AUDIO_DELETE), IMG_GLOBAL_INFO,
								1, 2000, 1);
		Go_to_slide_screen_cb();
		
	}
	else
	{		
		jMms_ComposeDeleteAudio_confirm();
	}
}

void jMms_ComposeDeleteVideo_handler()
{
	if(JC_NULL == g_pstJMmsAppInfo->ActivePageMediaIds[3])
	{		
		Go_to_slide_screen_cb();		
	}
	else
	{		
		jMms_ComposeDeleteVideo_confirm();
	}
}


void jMms_ComposeDeleteSlide_confirm(void)
{
	jMMS_GetUserConfirmation(GetString(STR_ID_MMS_DELETE_SLIDE), 
								jMms_DeletePage_Msg,Go_to_slide_screen,jMms_ComposeDeleteSlide_confirm) ;
}
void jMms_ComposeDeleteSlide_handler()
{
	JC_UINT8    uiCurrActivePage = 0;
    JC_RETCODE  rCode = JC_OK;
	
	rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg, &uiCurrActivePage);
	
	if(uiCurrActivePage <= 0)
	{		
		DisplayPopup(GetString (STR_ID_MMS_NO_MEDIA_TO_DELETE), IMG_GLOBAL_INFO,
								1, 2000, 1);
		Go_to_slide_screen_cb();
	}
	else
	{		
		jMms_ComposeDeleteSlide_confirm();
	}
}
#if 0
void jMms_ComposeInsertContent_highlight_handler (int iSelectIndex) 
{
	mmi_trace (1, "@@@ jMms_ComposeInsertContent_highlight_handler Index: %d @@@", iSelectIndex) ;
	
	switch (iSelectIndex)
	{
	case 0:
#ifdef __MMI_MULTI_SIM__
		SetLeftSoftkeyFunction (MTPNP_mms_send_msg_select_sim_entry_handler, KEY_EVENT_UP) ;
#else
		SetLeftSoftkeyFunction (jMms_SendMsgEntryHandler, KEY_EVENT_UP) ;
#endif
		break;
	case 1:
		SetLeftSoftkeyFunction (jMms_ComposeInsertImage_entry_handler, KEY_EVENT_UP) ;
		break ;
	case 2:
		SetLeftSoftkeyFunction (jMms_ComposeInsertAudio_entry_handler, KEY_EVENT_UP) ;
		break ;
	case 3:
		SetLeftSoftkeyFunction (jMms_ComposeInsertAttachment_entry_handler, KEY_EVENT_UP) ;
		break;
	case 4:	
		SetLeftSoftkeyFunction (jmms_ComposeEditTextEntryHandler, KEY_EVENT_UP) ;
		break ;
	case 5:
		SetLeftSoftkeyFunction (jMms_AddNewSlide_entry_handler, KEY_EVENT_UP) ;
		break ;
	case 6:
		SetLeftSoftkeyFunction (jMms_ComposeDeleteImage_handler, KEY_EVENT_UP) ;
		break;
	case 7:
		SetLeftSoftkeyFunction (jMms_ComposeDeleteAudio_handler, KEY_EVENT_UP) ;
		break;	
	case 8:	
		SetLeftSoftkeyFunction (jmms_SetSlideTimerHandler, KEY_EVENT_UP) ;
		break ;
	case 9:	
		SetLeftSoftkeyFunction (jMms_ComposeDeleteSlide_handler, KEY_EVENT_UP) ;
		break ;	
	case 10:
		SetLeftSoftkeyFunction (jMms_template_entry_handler, KEY_EVENT_UP) ;
		break ;
	case 11:
		SetLeftSoftkeyFunction (jmms_PreviewMsg, KEY_EVENT_UP) ;
		break ;
	case 12:
		SetLeftSoftkeyFunction (jMms_saveMsgHandler, KEY_EVENT_UP) ;
		break ;	
	default:		 
		break ;		
	}
	
}
#endif
void jMms_Highlight_Insert_Image(void)
{
	SetLeftSoftkeyFunction (jMms_ComposeInsertImage_entry_handler, KEY_EVENT_UP) ;
}

void jMms_Highlight_Insert_Audio(void)
{
	SetLeftSoftkeyFunction (jMms_ComposeInsertAudio_entry_handler, KEY_EVENT_UP) ;
}

void jMms_Highlight_Insert_Video(void)
{
	SetLeftSoftkeyFunction (jMms_ComposeInsertVideo_entry_handler, KEY_EVENT_UP) ;
}

void jMms_Highlight_Insert_Attachment(void)
{
	SetLeftSoftkeyFunction (jMms_ComposeInsertAttachment_entry_handler, KEY_EVENT_UP) ;
}

void jMms_Highlight_Edit_Text(void)
{
	SetLeftSoftkeyFunction (jmms_ComposeEditTextEntryHandler, KEY_EVENT_UP) ;
}

void jMms_Highlight_Add_NewSlide(void)
{
	SetLeftSoftkeyFunction (jMms_AddNewSlide_entry_handler, KEY_EVENT_UP) ;
}

void jMms_Highlight_Delete_Image(void)
{
	SetLeftSoftkeyFunction (jMms_ComposeDeleteImage_handler, KEY_EVENT_UP) ;
}

void jMms_Highlight_Delete_Audio(void)
{
	SetLeftSoftkeyFunction (jMms_ComposeDeleteAudio_handler, KEY_EVENT_UP) ;
}

void jMms_Highlight_Delete_Video(void)
{
	SetLeftSoftkeyFunction (jMms_ComposeDeleteVideo_handler, KEY_EVENT_UP) ;
}

void JMms_Highlight_Set_SlideTimer(void)
{
	SetLeftSoftkeyFunction (jmms_SetSlideTimerHandler, KEY_EVENT_UP) ;
}

void jMms_Highlight_Delete_Slide(void)
{
	SetLeftSoftkeyFunction (jMms_ComposeDeleteSlide_handler, KEY_EVENT_UP) ;
}

void jMms_Highlight_Template(void)
{
	SetLeftSoftkeyFunction (jMms_template_entry_handler, KEY_EVENT_UP) ;
}

void jMms_Highlight_Preview(void)
{
	SetLeftSoftkeyFunction (jmms_PreviewMsg, KEY_EVENT_UP) ;
}

void jMms_Set_Edit_Content_Highlight_Handler(void)
{
	SetHiliteHandler(MMS_INSERT_IMAGE, jMms_Highlight_Insert_Image);
	SetHiliteHandler(MMS_INSERT_AUDIO, jMms_Highlight_Insert_Audio);
	SetHiliteHandler(MMS_INSERT_VIDEO, jMms_Highlight_Insert_Video);	
	SetHiliteHandler(MMS_ADD_ATTACHMENT, jMms_Highlight_Insert_Attachment);
	SetHiliteHandler(MMS_EDIT_TEXT, jMms_Highlight_Edit_Text);
	SetHiliteHandler(MMS_ADD_SLIDE, jMms_Highlight_Add_NewSlide);
	SetHiliteHandler(MMS_DELETE_IMAGE, jMms_Highlight_Delete_Image);
	SetHiliteHandler(MMS_DELETE_AUDIO, jMms_Highlight_Delete_Audio);
	SetHiliteHandler(MMS_DELETE_VIDEO, jMms_Highlight_Delete_Video);
	SetHiliteHandler(MMS_SET_SLIDE_TIMER, JMms_Highlight_Set_SlideTimer);
	SetHiliteHandler(MMS_DELETE_SLIDE, jMms_Highlight_Delete_Slide);
	SetHiliteHandler(MMS_TEMPLATE, jMms_Highlight_Template);
	SetHiliteHandler(MMS_PREVIEW, jMms_Highlight_Preview);
	
	SetHiliteHandler(MMS_ATTACHMENT_ADD, jMms_Attachment_Add_Higglighthandler);
	SetHiliteHandler(MMS_ATTACHMENT_DELETE, jMms_Attachment_Delete_Higglighthandler);
	SetHiliteHandler(MMS_ATTACHMENT_DELETE_ALL, jMms_Attachment_DeleteAll_Higglighthandler);
}
JC_RETCODE jmms_ComposeSlideClose(void)
{
	JC_RETCODE	eRet	= JC_OK ;
	mmi_trace (1,"$$$jmms_ComposeSlideClose is called");
	jMMS_SetPrimaryMMIStatus(E_FALSE);

	if (g_pstJMmsAppInfo->hCurrMsg)
	{
		eRet = jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg,E_VIEW_CLOSE,JC_NULL);
	}  
    return eRet;
}

void jmms_ComposeEditContentOptions (void)
{
	U8* guiBuffer;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
	JC_RETCODE rCode = JC_OK;
	JC_UINT8    uiCurrActivePage = 0;

	rCode= jmms_ComposeSlideClose();	
	EntryNewScreen (SCR_ID_MMS_COMPOSE_SCREEN, NULL, jmms_ComposeEditContentOptions, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_COMPOSE_SCREEN) ;

	if(0 == g_pstJMmsAppInfo->ActivePageMediaIds[1])
		mmi_frm_hide_menu_item(MMS_DELETE_IMAGE);
	else
		mmi_frm_unhide_menu_item(MMS_DELETE_IMAGE);

	if(0 == g_pstJMmsAppInfo->ActivePageMediaIds[2])
		mmi_frm_hide_menu_item(MMS_DELETE_AUDIO);
	else
		mmi_frm_unhide_menu_item(MMS_DELETE_AUDIO);
	
	if(0 == g_pstJMmsAppInfo->ActivePageMediaIds[3])
		mmi_frm_hide_menu_item(MMS_DELETE_VIDEO);
	else
		mmi_frm_unhide_menu_item(MMS_DELETE_VIDEO);	
	
	rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg, &uiCurrActivePage);
	
	if(uiCurrActivePage <= 0)
		mmi_frm_hide_menu_item(MMS_DELETE_SLIDE);
	else
		mmi_frm_unhide_menu_item(MMS_DELETE_SLIDE);
		
	
	numItems = GetNumOfChild_Ext(MMS_EDITCONTENT_OPTION);
	GetSequenceStringIds_Ext(MMS_EDITCONTENT_OPTION, nStrItemList);
	
	SetParentHandler(MMS_EDITCONTENT_OPTION);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler) ;
	ShowCategory15Screen (STR_ID_MMS_ADDR_INSERTCONTENT, IMG_SMS_ENTRY_SCRN_CAPTION, 
							STR_GLOBAL_OK, 0, 
							STR_GLOBAL_BACK, 0,
							numItems, nStrItemList, (PU16)gIndexIconsImageList, 
							1 , 0, guiBuffer);	
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);	
}

void jmms_GotoComposeMainScreen()
{
	jmms_ComposeSlideClose();
	GoBackHistory();
}

JC_RETCODE jmms_ComposeViewStart ()
{
	JC_RETCODE	eRet	= JC_OK ;
	JC_RECT		stRect	= {0, } ;    
    DeviceCharacteristics stDevChars = {0,} ; 
	EVENT_START_VIEW StartDisplay ;

	jc_memset(&StartDisplay, 0, sizeof(EVENT_START_VIEW));
       
	eRet = jdi_MsgView(g_pstJMmsAppInfo->hCurrMsg, JC_NULL) ;    
  
   	if (eRet == JC_OK)
   	{   	
        jdd_MMIGetDeviceCharacteristics (&stDevChars) ;
		stRect.iLeft = stDevChars.uiWindowX ;
		stRect.iTop = 0;
		stRect.uiWidth = stDevChars.uiRenderingWidth  ;
		stRect.uiHeight = stDevChars.uiRenderingHeight ;

		mmi_trace (1,"@@@ jmms_ComposeViewStart () value is [%d]",stRect.uiHeight );
    
        StartDisplay.uiWindowID = g_pstJMmsAppInfo->uiViewWindow; 
	    StartDisplay.Rect = stRect ;
        
        StartDisplay.bViewPageOnly = E_TRUE;
        StartDisplay.ucPageIndex = g_pstJMmsAppInfo->ActivePageIndex;     
        StartDisplay.bStaticView = E_TRUE ;
        eRet = jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg,E_VIEW_START,(void*)(&StartDisplay));
        mmi_trace(1,"jdi_MsgViewHandleEvent [%d]",eRet);
    }
    
	return eRet ;
}

void jmms_DisplayComposeSlideExitHanler (void)
{
	jmms_ComposeSlideClose () ;
}

void jmms_DisplayComposeSlideMMI()
{
	U8* guiBuffer;
	JC_RETCODE rCode = JC_OK;

	jmms_PrepareCurrSlideForDisplay() ;
	rCode = jmms_ComposeViewStart();

	jMMS_SetPrimaryMMIStatus(E_TRUE);
	EntryNewScreen (SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN, jmms_DisplayComposeSlideExitHanler, jmms_DisplayComposeSlideMMI, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_COMPOSE_NEWMSG_SCREEN) ;

	ShowCategory7Screen(JC_NULL, IMG_SMS_ENTRY_SCRN_CAPTION,
						STR_GLOBAL_OPTIONS, 0, 
						STR_GLOBAL_BACK, 0, 
						NULL,guiBuffer) ;	
		
	rCode = jmms_MMIViewMmsPlaySlide (g_pstJMmsAppInfo->ActivePageIndex, E_FALSE);
	SetRightSoftkeyFunction(jmms_GotoComposeMainScreen, KEY_EVENT_UP);
	SetLeftSoftkeyFunction (jmms_ComposeEditContentOptions, KEY_EVENT_UP) ;
	jmms_ComposeSlideRegisterKeyHandler () ;
}

void jmms_EditContentEntryHandler ()
{	
	JC_UINT8 ucTotalPages;
	jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucTotalPages) ;
	
	if (0 == ucTotalPages)
	{
	    g_pstJMmsAppInfo->ActivePageIndex = 0;
		jmms_MMIAddPage ();
	}	
	jmms_DisplayComposeSlideMMI();
}

void jmms_SlideTimerDoneHandler()
{
	JC_CHAR *psInputValue = JC_NULL ;	
	JC_RETCODE rCode = JC_OK;	
	JC_UINT32 uiSlideTimer;

	jMMS_MMIGetInputBoxValue (&psInputValue);	
	uiSlideTimer = jdi_CUtilsTtoI (NULL, psInputValue) ;
	if((0 == uiSlideTimer) || (60 < uiSlideTimer))
	{
		//warning to 	show "Enter Between 1 and 60"	
		DisplayPopup(GetString(STR_ID_MMS_ENTER_WARNING), IMG_GLOBAL_INFO, 1, 2000, 1);
	}
	else
	{		
		rCode = jmms_SetCurrPageSlideDuration(uiSlideTimer*1000);
		DisplayPopup(GetString (STR_GLOBAL_DONE), IMG_GLOBAL_INFO,
								1, 1500, 1);
		Go_to_slide_screen_cb();
	}
	jdd_MemFree(psInputValue);
}

void jmms_SetSlideTimerHandler()
{
	U8* guiBuffer;
	S16 required_input_modes[] = {INPUT_MODE_123, -1};
	JC_CHAR 		*pmTemp ;
	JC_UINT32 uiSlideTime;

	jmms_GetCurPageSlideDuration(&uiSlideTime);

	pmTemp  = jdi_CUtilsItoT(NULL,uiSlideTime/1000);
	jc_wstrcpy (g_pstJMmsAppInfo->pcInputBuff, pmTemp) ;

	EntryNewScreen (SCR_ID_MMS_COMPOSE_SLIDE_TIMER_SCREEN, NULL, 
					jmms_SetSlideTimerHandler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_COMPOSE_SLIDE_TIMER_SCREEN) ;
	ShowCategory5Screen_ext(STR_ID_MMS_NEW_MMS, IMG_SMS_ENTRY_SCRN_CAPTION,
							   STR_GLOBAL_OK, IMG_GLOBAL_OK,
							   STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							   INPUT_TYPE_NUMERIC,
							   jMms_getInputBuff (),
							   3, NULL,
							   0, required_input_modes,	NULL);	
	 	
	SetLeftSoftkeyFunction (jmms_SlideTimerDoneHandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);	
	jdd_MemFree(pmTemp);
	
}

void jmms_PreviewMsg()
{
	JC_RETCODE rCode ;
	rCode = jmms_ViewMms (JC_NULL, E_FALSE);
}

JC_RETCODE	jmms_GetCurPageSlideDuration(JC_UINT32* pSlideTime)
{
    JC_RETCODE          eRet = JC_OK ;
    JC_UINT8              ucPageIndex=0;

	ucPageIndex = g_pstJMmsAppInfo->ActivePageIndex ;
    eRet = jdi_MsgGetPageTimer(g_pstJMmsAppInfo->hCurrMsg,ucPageIndex,pSlideTime);    
    if( (eRet == JC_OK) && (*pSlideTime==0))
    {
        eRet = jdi_MsgSetPageTimer(g_pstJMmsAppInfo->hCurrMsg,ucPageIndex, \
			g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime);    
        *pSlideTime = g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime;//use default value               
    }        
	return eRet;        
}

JC_RETCODE	jmms_SetCurrPageSlideDuration(JC_UINT32 uiSlideTimer)
{
    JC_UINT8 ucPageIndex = g_pstJMmsAppInfo->ActivePageIndex;
    return jdi_MsgSetPageTimer(g_pstJMmsAppInfo->hCurrMsg,ucPageIndex,uiSlideTimer);        
}

JC_RETCODE jmms_MMIAddPage ( void )
{
	JC_RETCODE	rCode = JC_OK ;
	JC_UINT8  ucTotalPages = 0, ucMaxPages = 0;
			
	rCode = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucTotalPages) ;
    rCode = jdi_MsgGetMaxPages(g_pstJMmsAppInfo->hCurrMsg, &ucMaxPages) ;
	mmi_trace(1," AutoSign value[%d], Current Page[%d], Maxpages[%d]", \
			g_pstJMmsAppInfo->stMMSSetting.bIsAutoSignatureOn, ucTotalPages,ucMaxPages);

	if((g_pstJMmsAppInfo->stMMSSetting.bIsAutoSignatureOn && ucTotalPages >= ucMaxPages-1) || 
		ucTotalPages >= ucMaxPages)
	{
		rCode = JC_ERR_MSG_MAX_REACHED;
	}
	else
	{
		rCode = jdi_MsgAddPage (g_pstJMmsAppInfo->hCurrMsg, (JC_UINT8)(g_pstJMmsAppInfo->ActivePageIndex + 1) ) ;	  
		if(rCode == JC_OK)
		{    	
			g_pstJMmsAppInfo->ActivePageIndex ++;
			rCode = jmms_SetCurrPageSlideDuration(g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime);
	     }	
	}
    return rCode ;
}
void jMms_AbortCompose(void)
{
	jmms_CallbackGoBackHistory();
	GoBackHistory();
}
void jMms_ExitComposeHandler(void)
{
	mmi_trace(1,"JDD_LOG: jMms_ExitComposeHandler");
	if(jmms_ComposeWithModification())
	{
		DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,
					STR_GLOBAL_NO,IMG_GLOBAL_NO,
					get_string(STR_ID_MMS_EXIT_QUESTION), IMG_GLOBAL_QUESTION, WARNING_TONE);
		SetLeftSoftkeyFunction(jMms_saveMsgHandler,KEY_EVENT_UP);
		SetRightSoftkeyFunction(jMms_AbortCompose,KEY_EVENT_UP);
	}
	else
	{
		jMms_AbortCompose();
	}
}

void jMms_AppAbortCompose(void)
{
	jmms_CallbackGoBackHistory();
}

void jMms_AppExitComposeHandler(void)
{
	mmi_trace(1,"JDD_LOG: jMms_ExitComposeHandler");
	if(jmms_ComposeWithModification())
	{
		DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,
					STR_GLOBAL_NO,IMG_GLOBAL_NO,
					get_string(STR_ID_MMS_EXIT_QUESTION), IMG_GLOBAL_QUESTION, WARNING_TONE);
		SetLeftSoftkeyFunction(jMms_saveMsgHandler,KEY_EVENT_UP);
		SetRightSoftkeyFunction(jMms_AppAbortCompose,KEY_EVENT_UP);
	}
	else
	{
		jMms_AbortCompose();
	}
}

U16 g_MMS_send_require = 0;

void jMms_Highlight_Send(void)
{
	g_MMS_send_require = SEND_ONLY;
	#ifdef __MMI_MULTI_SIM__
	SetLeftSoftkeyFunction (MTPNP_mms_send_msg_select_sim_entry_handler, KEY_EVENT_UP) ;
	#else
	SetLeftSoftkeyFunction (jMms_SendMsgEntryHandler, KEY_EVENT_UP) ;
	#endif
}
void jMms_Highlight_Send_And_Save(void)
{
	g_MMS_send_require = SEND_AND_SAVE;
	#ifdef __MMI_MULTI_SIM__
	SetLeftSoftkeyFunction (MTPNP_mms_send_msg_select_sim_entry_handler, KEY_EVENT_UP) ;
	#else
	SetLeftSoftkeyFunction (jMms_SendMsgEntryHandler, KEY_EVENT_UP) ;
	#endif
}
void jMms_Highlight_Save(void)
{
	SetLeftSoftkeyFunction (jMms_saveMsgHandler, KEY_EVENT_UP);
}
void jMms_Highlight_Exit_Compose(void)
{
	SetLeftSoftkeyFunction(jMms_ExitComposeHandler, KEY_EVENT_UP);
}
void jMms_Highlight_App_Exit_Compose(void)
{
	SetLeftSoftkeyFunction(jMms_AppExitComposeHandler, KEY_EVENT_UP);
}
void jmms_ComposeDoneOption(void)
{
	U8* guiBuffer;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
	
	mmi_trace(1,"JDD_LOG: jmms_ComposeDoneOption");
	EntryNewScreen (SCR_ID_MMS_COMPOSE_DONE_SCREEN, NULL, jmms_ComposeDoneOption, NULL) ;

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_COMPOSE_DONE_SCREEN);
	
	numItems = GetNumOfChild_Ext(MMS_COMPOSE_DONE_OPTION);
	GetSequenceStringIds_Ext(MMS_COMPOSE_DONE_OPTION, nStrItemList);	

	SetParentHandler(MMS_COMPOSE_DONE_OPTION);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler) ;
	
	ShowCategory15Screen (STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION, 
							STR_GLOBAL_OK, 0, 
							STR_GLOBAL_BACK, 0,
							numItems, nStrItemList, (PU16)gIndexIconsImageList, 
							1 , 0, guiBuffer);	
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}
#endif //__JATAAYU_APP__

