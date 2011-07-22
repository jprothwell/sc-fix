/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *	Res_UnifiedMessage.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file populates resource for Unified Message.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef _MMI_RES_UM_C
#define _MMI_RES_UM_C
#include "stdc.h"
#include "mmi_features.h"

#ifdef __MMI_UNIFIED_MESSAGE__		


#include "customcfg.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#include "unifiedmessagedef.h"
#include "messagesresourcedata.h"

#ifdef __MMI_UNIFIED_COMPOSER__
#include "unifiedcomposerdef.h"
#endif

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"


/**************************************************************

	FUNCTION NAME		: PopulateUnifiedMessageResData

  	PURPOSE				: Register all the resources required by Unified Message

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PopulateUnifiedMessageResData (void)
{
	ADD_APPLICATION_STRING2 (STR_UM_MAIN_ID, "Messages", "Unified message menu item String");
	ADD_APPLICATION_STRING2 (STR_UM_WRITE_MSG_ID, "Write Message", "Unified message write msg menu item String");
	ADD_APPLICATION_STRING2 (STR_UM_INBOX_ID, "Inbox", "Unified message inbox menu item String");
	ADD_APPLICATION_STRING2 (STR_UM_UNSENT_ID, "Outbox", "Unified message unsent  menu item String");
	ADD_APPLICATION_STRING2 (STR_UM_SENT_ID, "Sent", "Unified message sent menu item String");
	ADD_APPLICATION_STRING2 (STR_UM_DRAFT_ID, "Drafts", "Unified message draft menu item String");
	ADD_APPLICATION_STRING2 (STR_UM_DELETE_FOLDER_ID, "Delete Folder", "Unified message delete folder menu item String");
	ADD_APPLICATION_STRING2 (STR_UM_TEMPLATE_ID, "Template", "Unified message template menu item String");
	ADD_APPLICATION_STRING2 (STR_UM_SETTING_ID, "Message Settings", "Unified message setting menu item String");
	ADD_APPLICATION_STRING2 (STR_UM_LOADING_ID, "Loading",	"UM while loading Message String");
	ADD_APPLICATION_STRING2 (STR_UM_MSG_ID, "msg(s)",	"UM message number String");
	ADD_APPLICATION_STRING2 (STR_UM_DELETE_FOLDER_INBOX_ID, "Delete Inbox",	"UM delete folder delete inbox confirm String");
	ADD_APPLICATION_STRING2 (STR_UM_DELETE_FOLDER_UNSENT_ID, "Delete Outbox",	"UM delete folder delete unsent confirm String");
	ADD_APPLICATION_STRING2 (STR_UM_DELETE_FOLDER_SENT_ID, "Delete Sent",	"UM delete folder delete sent confirm String");
	ADD_APPLICATION_STRING2 (STR_UM_DELETE_FOLDER_DRAFT_ID, "Delete Drafts",	"UM delete folder delete draft confirm String");	
	ADD_APPLICATION_STRING2 (STR_UM_DELETE_FOLDER_ALL_ID, "Delete All Folders",	"UM delete all folders confirm String");
	ADD_APPLICATION_STRING2 (STR_UM_DELETING_ID,		"Deleting",	"Deleting Process Indicator UM Screen Caption String");
	ADD_APPLICATION_STRING2 (STR_UM_NEW_MSG_ID,		"New Message!",	"UM New Message string in idle screen indication");
	ADD_APPLICATION_STRING2 (STR_UM_UNREAD_MSG_NUMBER_ID,		"Unread",	"UM number of unread messages string in idle screen indication");
	ADD_APPLICATION_STRING2 (STR_UM_SEND_FAILED_SAVE_SUCCESS_ID,		"Not Sent, Stored in Outbox",	"UM Send Fail and Saved Successfully Message String");
	ADD_APPLICATION_STRING2 (STR_UM_SEND_SAVE_SUCCESS_ID,		"Sent and Stored",	"UM Send and Save Successfully Message String");
	ADD_APPLICATION_STRING2 (STR_UM_EMPTY_SUBJECT_ID,		"<No Subject>",	"UM No subject UM String");
	ADD_APPLICATION_STRING2 (STR_UM_MAIN_MENU_TITLE_ID,		"Message Center",	"Main menu message String");
	ADD_APPLICATION_STRING2 (STR_UM_INCOMPLETE_ID,		"<Incomplete>",	"UM Incomplete msg subject String");
	ADD_APPLICATION_STRING2 (STR_UM_NOT_SUPPORT_ID,		"<Not Supported>",	"UM Unsupport msg subject String");
	ADD_APPLICATION_STRING2 (STR_UM_SMS_ID, "Text Message", "Unified message SMS menu item String");
	ADD_APPLICATION_STRING2 (STR_UM_MMS_ID, "Multimedia Message", "Unified message MMS menu item String");
	
	
				
	ADD_APPLICATION_IMAGE2 (IMG_UM_MAIN_ID,
								CUST_IMG_PATH"/MainLCD/UniMessage/Uni_MG.pbm",
							 	"UM menu item Image");
	ADD_APPLICATION_IMAGE2 (IMG_UM_WRITE_MSG_ID,
								CUST_IMG_PATH"/MainLCD/UniMessage/Uni_WRITE.pbm",
								"Write Message UM menu item Image");	
	ADD_APPLICATION_IMAGE2 (IMG_UM_INBOX_ID,
								CUST_IMG_PATH"/MainLCD/UniMessage/Uni_INBOX.pbm",
								"inbox UM menu item Image");
	ADD_APPLICATION_IMAGE2 (IMG_UM_UNSENT_ID,
								CUST_IMG_PATH"/MainLCD/UniMessage/Uni_Unsent.pbm",
								"Unsent UM menu item Image");
	ADD_APPLICATION_IMAGE2 (IMG_UM_SENT_ID,
								CUST_IMG_PATH"/MainLCD/UniMessage/Uni_Sent.pbm",
								"Sent UM menu item Image");
	ADD_APPLICATION_IMAGE2 (IMG_UM_DRAFT_ID,
								CUST_IMG_PATH"/MainLCD/UniMessage/Uni_DraftsBox.PBM",
								"Draft UM menu item Image");
	ADD_APPLICATION_IMAGE2 (IMG_UM_DELETE_FOLDER_ID, 	
								CUST_IMG_PATH"/MainLCD/UniMessage/Uni_Delete.PBM",
								"Delete Folder UM menu item Image");
	ADD_APPLICATION_IMAGE2 (IMG_UM_TEMPLATE_ID,
								CUST_IMG_PATH"/MainLCD/UniMessage/Uni_Temp.pbm",
								"Template UM menu item Image");
	ADD_APPLICATION_IMAGE2 (IMG_UM_SETTING_ID,
								CUST_IMG_PATH"/MainLCD/UniMessage/Uni_Set.pbm",
							 	"Settings UM menu item Image");
	ADD_APPLICATION_IMAGE2 (IMG_UM_DELETE_FOLDER_ALL_ID,
								CUST_IMG_PATH"/MainLCD/UniMessage/Uni_DeleteAll.pbm",
							 	"Delete All UM menu item Image");
	ADD_APPLICATION_IMAGE2 (IMG_UM_ENTRY_SCRN_CAPTION,
								CUST_IMG_PATH"/MainLCD/TitleBar/TB_MG.PBM",
								"UM Menu item common Image");

	/* UM main menu  */
	ADD_APPLICATION_MENUITEM((MENU_ID_UM_MAIN,
										MAIN_MENU_MESSAGES_MENUID,	UM_MENU_TOTAL_NUM,
										MENU_ID_UM_WRITE_MSG,
										MENU_ID_UM_INBOX,
										MENU_ID_UM_UNSENT,
										MENU_ID_UM_SENT,
										MENU_ID_UM_DRAFT,
										MENU_ID_UM_DELETE_FOLDER,
										MENU_ID_UM_TEMPLATE,
										MENU_ID_UM_SETTING,										
										SHOW,	MOVEABLEWITHINPARENT|SHORTCUTABLE,	DISP_LIST,
										STR_UM_MAIN_ID,	IMG_UM_MAIN_ID));
	
	/* first level  */
	ADD_APPLICATION_MENUITEM((MENU_ID_UM_WRITE_MSG,
									MENU_ID_UM_MAIN,
									UM_SUPPORT_TYPE_TOTAL_NUM,
									MENU_ID_UM_WRITE_MSG_SMS,
									MENU_ID_UM_WRITE_MSG_MMS,
									SHOW, 	MOVEABLEWITHINPARENT|SHORTCUTABLE, 	DISP_LIST,
									STR_UM_WRITE_MSG_ID, 	IMG_UM_WRITE_MSG_ID));

	ADD_APPLICATION_MENUITEM((MENU_ID_UM_INBOX,
									MENU_ID_UM_MAIN,
									0,
									SHOW, 	MOVEABLEWITHINPARENT|SHORTCUTABLE, 	DISP_LIST,
									STR_UM_INBOX_ID, 	IMG_UM_INBOX_ID));

	ADD_APPLICATION_MENUITEM((MENU_ID_UM_UNSENT,
									MENU_ID_UM_MAIN,
									0,
									SHOW, 	MOVEABLEWITHINPARENT|SHORTCUTABLE, 	DISP_LIST,
									STR_UM_UNSENT_ID, 	IMG_UM_UNSENT_ID));

	ADD_APPLICATION_MENUITEM((MENU_ID_UM_SENT,
									MENU_ID_UM_MAIN,
									0,
									SHOW, 	MOVEABLEWITHINPARENT|SHORTCUTABLE, 	DISP_LIST,
									STR_UM_SENT_ID, 	IMG_UM_SENT_ID));

	ADD_APPLICATION_MENUITEM((MENU_ID_UM_DRAFT,
									MENU_ID_UM_MAIN,
									0,
									SHOW, 	MOVEABLEWITHINPARENT|SHORTCUTABLE, 	DISP_LIST,
									STR_UM_DRAFT_ID, 	IMG_UM_DRAFT_ID));
	
	ADD_APPLICATION_MENUITEM((MENU_ID_UM_DELETE_FOLDER,
									MENU_ID_UM_MAIN,
									(UM_MSG_FOLDER_TOTAL_NUM+1), /* plus 1 for "delete all folder" menu item */
									MENU_ID_UM_DELETE_FOLDER_INBOX,
									MENU_ID_UM_DELETE_FOLDER_UNSENT,
									MENU_ID_UM_DELETE_FOLDER_SENT,
									MENU_ID_UM_DELETE_FOLDER_DRAFT,
									MENU_ID_UM_DELETE_FOLDER_ALL,
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_DELETE_FOLDER_ID, 	IMG_UM_DELETE_FOLDER_ID));
	
	ADD_APPLICATION_MENUITEM((MENU_ID_UM_TEMPLATE,
									MENU_ID_UM_MAIN,
									UM_SUPPORT_TYPE_TOTAL_NUM,
									MENU_ID_UM_TEMPLATE_SMS,
									MENU_ID_UM_TEMPLATE_MMS,
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_TEMPLATE_ID, 	IMG_UM_TEMPLATE_ID));	

    ADD_APPLICATION_MENUITEM2((MENU_ID_UM_SETTING,
                                    MENU_ID_UM_MAIN,
                                    MESSAGES_MENU_MSG_SETTINGS_MENUID, 
                                    MENU_ID_UM_SETTING_MMS,
                                #ifdef __MMI_UNIFIED_COMPOSER__
                                    MENU_ID_UC_SETTING_PREFERRED_TYPE,
                                #endif /* __MMI_UNIFIED_COMPOSER__ */                                    
                                    MENU_ITEM_END,
                                    SHOW, MOVEABLEWITHINPARENT, DISP_LIST, 
                                    STR_UM_SETTING_ID,IMG_UM_SETTING_ID));

	/* Second level */
	ADD_APPLICATION_MENUITEM((MENU_ID_UM_WRITE_MSG_SMS,
									MENU_ID_UM_WRITE_MSG,
									0,
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_SMS_ID, 	IMG_MESSAGE_SMS_MENUENTRY));	

	ADD_APPLICATION_MENUITEM((MENU_ID_UM_WRITE_MSG_MMS,
									MENU_ID_UM_WRITE_MSG,
									0,
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_MMS_ID, 	IMG_MESSAGE_MMS_MENUENTRY));

	ADD_APPLICATION_MENUITEM((MENU_ID_UM_DELETE_FOLDER_INBOX,
									MENU_ID_UM_DELETE_FOLDER,
									0, 
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_INBOX_ID, 	IMG_UM_INBOX_ID));	

	ADD_APPLICATION_MENUITEM((MENU_ID_UM_DELETE_FOLDER_UNSENT,
									MENU_ID_UM_DELETE_FOLDER,
									0, 
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_UNSENT_ID, 	IMG_UM_UNSENT_ID));	

	ADD_APPLICATION_MENUITEM((MENU_ID_UM_DELETE_FOLDER_SENT,
									MENU_ID_UM_DELETE_FOLDER,
									0, 
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_SENT_ID, 	IMG_UM_SENT_ID));	

	ADD_APPLICATION_MENUITEM((MENU_ID_UM_DELETE_FOLDER_DRAFT,
									MENU_ID_UM_DELETE_FOLDER,
									0, 
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_DRAFT_ID, 	IMG_UM_DRAFT_ID));	

	ADD_APPLICATION_MENUITEM((MENU_ID_UM_DELETE_FOLDER_ALL,
									MENU_ID_UM_DELETE_FOLDER,
									0, 
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_DELETE_FOLDER_ALL_ID, 	IMG_UM_DELETE_FOLDER_ALL_ID));	


	ADD_APPLICATION_MENUITEM((MENU_ID_UM_TEMPLATE_SMS,
									MENU_ID_UM_TEMPLATE,
									0,
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_SMS_ID, 	IMG_MESSAGE_SMS_MENUENTRY));

	ADD_APPLICATION_MENUITEM((MENU_ID_UM_TEMPLATE_MMS,
									MENU_ID_UM_TEMPLATE,
									0,
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_MMS_ID, 	IMG_MESSAGE_MMS_MENUENTRY));
	
/*	ADD_APPLICATION_MENUITEM((MENU_ID_UM_SETTING_SMS,
									MENU_ID_UM_SETTING,
									0,
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_SMS_MENUENTRY, 	IMG_MESSAGE_SMS_MENUENTRY));
*/
	ADD_APPLICATION_MENUITEM((MENU_ID_UM_SETTING_MMS,
									MENU_ID_UM_SETTING,
									0,
									SHOW, 	MOVEABLEWITHINPARENT, 	DISP_LIST,
									STR_UM_MMS_ID, 	IMG_MESSAGE_MMS_MENUENTRY));
	
}
#endif
#endif  //#ifndef __MMI_UNIFIED_MESSAGE__
#endif  //#ifndef _MMI_RES_UM_C

