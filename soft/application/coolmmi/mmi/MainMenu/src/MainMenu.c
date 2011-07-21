/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
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
 * mainmenu.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for main menu
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**********************************************************************************
	Filename:		mainmenu.c
 
	Date Created:	September-07-2002
	Contains:		Main menu application
**********************************************************************************/
#define __NEWSIMULATOR
#include "mmi_trace.h"
#include "stdc.h"
#include "mmi_features.h"
#include "l4dr.h"
#include "l4dr1.h"

#include "allappgprot.h"
//#include "custdatares.h"
//#include "custmenures.h"
#include "frameworkstruct.h"
//#include "custmenures.h"
#include "globalconstants.h"
#include "eventsgprot.h"
#include "mmiappfnptrs.h"
#include "historygprot.h"
#include "phonebookdef.h"
#include "mainmenudef.h"
#include "mainmenuprot.h"
#include "wgui_categories.h"
#include "wgui_categories_mm.h"
#include "miscfunctions.h"
#include "profilesenum.h"
#include "settinggprots.h"
#include "settingstructs.h" //added by zhoumn@2007/07/24
//micha0202
//#include "networksetupdefs.h"
//#include "connectgprots.h"
#include "funandgamesdefs.h"
#include "funandgamesprots.h"
 
#include "callhistorymain.h"
#include "settingdefs.h"
#include "globaldefs.h"
#include "simdetectiongexdcl.h"
//#ifdef __MOD_SMSAL__
#include "messagesexdcl.h"
//#endif
#include "commonscreens.h"

#include "mmi.h"    
//micha0601
#include "service.h"
#ifdef __SAT__
#include "satgprots.h"
#endif
#ifdef __MMI_FILE_MANAGER__
#include "filemgr.h"
#endif
#include "shortcutsprots.h"

#include "funandgamesprots.h"
#include "wgui_status_icons.h"
 
//#include "callhistorygprots.h"
#include "mainmenudef.h"
 
//#include "callsdefs.h"
#include "managecallmain.h"
#include "idleappdef.h"
#include "idleappprot.h"

 
//#include "callhistoryenum.h"
#include "callsetupenum.h"
#include "unicodexdcl.h"

#include "organizergprot.h"
#include "organizerdef.h"
#include "settingprot.h"
#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
#include "gui_custom_menus.h"
#include "custmenures.h"
#ifdef __MMI_WGUI_MINI_TAB_BAR__
#include "wgui_tab_bars.h"
#endif
#include "usbdevicegprot.h"
extern U8 PhnsetGetMainMenuStyle(void);
#endif

#if (defined(__MMI_IRDA_SUPPORT__) || defined(__MMI_USB_SUPPORT__)|| defined(__MMI_BT_SUPPORT__)  )
#include "extdevicedefs.h"
#endif

 
#include "debuginitdef.h"
 

#include "settingprofile.h"
#include "phonebooktypes.h"
#include "settingsgdcl.h"

#if defined (__MMI_EBOOK_READER__)
#include "ebookgprot.h"
#include "ebookprot.h"
#endif 
#include "simdetectiongprot.h"
#if defined(__MMI_MAINLCD_220X176__)
#include "audioplayerdef.h"
extern void HighlightAudioPlayer(void);
#endif
#undef __NEWSIMULATOR

extern void EntryIdleScreen(void);
 
extern void EntryScrProfiles(void);
extern void mmi_phb_reset_scr_id(void); 

#ifdef __MMI_MAINLCD_220X176__
extern void EntryAlmMenu(void); 
#endif
//micha0601
extern void GoToSATMainMenu(void);

#ifdef	__MMI_KEYPAD_LOCK_PATTERN_2__   
extern void IdleHandleKeypadLockProcess(void);
extern void IdleHandlePoundKeyForKeypadLock(void);
extern void IdleHandleStarKeyForKeypadLock(void);
#endif

extern U16 gMainMenuTitleIcon[];
 
extern s32 main_menu_index;
extern s32 MMI_main_menu_type;
//extern UI_3D_circular_menu	wgui_CM3D_menu;
 

extern mmi_phb_context_struct g_phb_cntx;
extern pwr_context_struct g_pwr_context[];
#if defined(__SG_MMI_MEDIA_SPECIAL_MENU__)
extern void exit_multimedia_menu_screen(void);
#endif
 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
extern void shuffle_draw_white(void);
#endif
 

#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
extern BOOL inMatrixNoTitleFlag; 
#endif

#if defined( __MMI_FM_RADIO__ ) && defined( __MMI_FM_RADIO_CHECK_CHIP_OR_HIDE__ )
extern void mmi_fmrdo_skip_highlight_hdlr( S32 index ); // defined in FMRadioSrc.c
extern U16 mmi_fmrdo_check_chip_or_hide( U16 n_items, U16* string_ids, U16* icon_ids ); // defined in FMRadioSrc.c
#endif

extern VOID mmi_display_usb_busy(void) ;
extern void EntrySMSMainMenuList(void);

/**************************************************************

	FUNCTION NAME		: handle_mainmenu_right_softkey_up

  	PURPOSE				: Go to previous screen when rt soft key
						  is released


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void handle_mainmenu_right_softkey_up(void)
{	GoBackHistory();
}

/**************************************************************

	FUNCTION NAME		: highlight_mainmenu_profiles

  	PURPOSE				: Associate the key handler
						  with left and rt soft key


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

	AUTHOR				:Ashima

**************************************************************/

void highlight_mainmenu_profiles(void)
{	
 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

	SetLeftSoftkeyFunction(EntryScrProfiles,KEY_EVENT_UP);
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);
#ifdef __MMI_MAINLCD_128X160__
	//SetKeyHandler(EntryScrProfiles, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    	//SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
#endif

#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_MAINLCD_128X128__)&& !defined(__MMI_MAINLCD_160X128__)
#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
		SetKeyHandler(EntryScrProfiles,KEY_RIGHT_ARROW,KEY_EVENT_UP);
#endif
}

#ifdef __MMI_MAINLCD_220X176__
/**************************************************************

	FUNCTION NAME		: highlight_mainmenu_alarm

  	PURPOSE				: Associate the key handler
						  with left and rt soft key


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

	AUTHOR				:Li Cong

**************************************************************/

void highlight_mainmenu_alarm(void)
{	
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

	SetLeftSoftkeyFunction(EntryAlmMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);
    #if  !defined(__MMI_MAINLCD_128X128__)
#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
		SetKeyHandler(EntryAlmMenu,KEY_RIGHT_ARROW,KEY_EVENT_UP);
    #endif
}

#endif

/**************************************************************

	FUNCTION NAME		: highlight_mainmenu_phonebook

  	PURPOSE				: Associate the key handler
						  with left and rt soft key


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
/*Wangzl Add Start  Ver: TBM780  on 2007-8-23 16:8 */
BOOL is_phb_init_done(void)
{
	U8 SimRemoveFlag = 0;
	U16 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].SimRemoveFlag == 1)
		{
			SimRemoveFlag = 1;
			break;
		}
	}
	mmi_trace(1,"is_phb_init_done,g_phb_cntx.phb_ready is %d,%d",g_phb_cntx.phb_ready,SimRemoveFlag);
	return ((g_phb_cntx.phb_ready == 1) || SimRemoveFlag);
}
/*Wangzl Add End   Ver: TBM780  on 2007-8-23 16:8 */
	/*wangrui Add for fixbug 10195Start :  on 2008-10-27*/
BOOL is_phb_init_Finishi(void)
{
	U8 SimRemoveFlag = 0;
	U16 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].SimRemoveFlag == 1)
		{
			SimRemoveFlag = 1;
			break;
		}
	}
	mmi_trace(1,"is_phb_init_Finishi,g_phb_cntx.phb_ready is %d,%d",g_phb_cntx.phb_ReadyFinishi,SimRemoveFlag);
	return ((g_phb_cntx.phb_ReadyFinishi == TRUE) || SimRemoveFlag );
}
	/*wangrui Add for fixbug 10195Start end*/


void highlight_mainmenu_phonebook(void)
{	
#if 1  //dyj del 20060514
 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

	SetLeftSoftkeyFunction(mmi_phb_entry_main_menu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);

#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_MAINLCD_128X128__)
#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
		SetKeyHandler(mmi_phb_entry_main_menu,KEY_RIGHT_ARROW,KEY_EVENT_UP);
#endif		
#endif
}

/**************************************************************

	FUNCTION NAME		: highlight_mainmenu_messages

  	PURPOSE				: Associate the key handler
						  with left and rt soft key


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void highlight_mainmenu_messages(void)
{	

#if 1   //dyj del 20060514
 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif


#ifdef __MOD_SMSAL__
	SetLeftSoftkeyFunction(EntryScrMessagesMenuList, KEY_EVENT_UP);// sms-code
//	SetLeftSoftkeyFunction(EntrySMSMainMenuList, KEY_EVENT_UP);           //add by yaosq 20070108   从短信直接进入收件箱界面
#endif
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);
#endif
}

/**************************************************************

	FUNCTION NAME		: highlight_mainmenu_funandgames

  	PURPOSE				: Associate the key handler
						  with left and rt soft key


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void highlight_mainmenu_funandgames(void)
{	

 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

    SetLeftSoftkeyFunction(mmi_fng_entry_screen, KEY_EVENT_UP);
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);

#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_MAINLCD_128X128__)
#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
	SetKeyHandler(mmi_fng_entry_screen,KEY_RIGHT_ARROW,KEY_EVENT_UP);
#endif
}


/**************************************************************

	FUNCTION NAME		: highlight_mainmenu_organizer

  	PURPOSE				: Associate the key handler
						  with left and rt soft key


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void highlight_mainmenu_organizer(void)
{

 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

	SetLeftSoftkeyFunction(EntryOrganizer,KEY_EVENT_UP);
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);

#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_MAINLCD_128X128__)&& !defined(__MMI_MAINLCD_160X128__)
#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
		SetKeyHandler(EntryOrganizer,KEY_RIGHT_ARROW,KEY_EVENT_UP);
#endif
}
#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
#define SCR_MMATERIAL       MMEDIA_BASE+200
#define SCR_MMODETOOLS      SETTINGS_BASE+200
void EntryModeTools(void)
{
	U16		ItemList[MAX_SUB_MENUS];
	U16		ItemIcons[MAX_SUB_MENUS];
	U16		nItems;
	U8*		guiBuffer;

	EntryNewScreen(SCR_MMODETOOLS,NULL, EntryModeTools, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_MMODETOOLS);
	nItems = GetNumOfChild(MAIN_MENU_MODETOOLS_MENUID);
	GetSequenceStringIds(MAIN_MENU_MODETOOLS_MENUID,ItemList);
	GetSequenceImageIds(MAIN_MENU_MODETOOLS_MENUID,ItemIcons);
	SetParentHandler(MAIN_MENU_MODETOOLS_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(MAIN_MENU_MODETOOLS_MENUID);
#endif

#ifdef __MMI_NUMBER_MENU__
	ShowCategory52Screen(	MAIN_MENU_MODETOOLS_TEXT,MAIN_MENU_TITLE_SETTINGS_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, (PU16)gIndexIconsImageList, 0, 0, 0, guiBuffer);
#else
	ShowCategory52Screen(	MAIN_MENU_MODETOOLS_TEXT,MAIN_MENU_TITLE_SETTINGS_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, ItemIcons, 0, 0, 0, guiBuffer);
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

void EntryMaterial(void)
{
	U16		ItemList[MAX_SUB_MENUS];
	U16		ItemIcons[MAX_SUB_MENUS];
	U16		nItems;
	U8*		guiBuffer;
	BOOL 	IsSimValid = MMI_FALSE;
	U16 		i;

	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
	EntryNewScreen(SCR_MMATERIAL,NULL, EntryMaterial, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_MMATERIAL);
	nItems = GetNumOfChild(MAIN_MENU_MATERIAL_MENUID);
	GetSequenceStringIds(MAIN_MENU_MATERIAL_MENUID,ItemList);
	GetSequenceImageIds(MAIN_MENU_MATERIAL_MENUID,ItemIcons);
	SetParentHandler(MAIN_MENU_MATERIAL_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].IsSimValid)
		{
			IsSimValid = MMI_TRUE;
			break;
		}
	}
#if defined(__MMI_VRSD__) || defined(__MMI_VRSI__)
   /* No SIM, remove voice dial menu items */
   if (!IsSimValid)
      nItems-=1;
#endif

#if defined(__MMI_PHB_CALLER_RES_SETTING__)
   /* No SIM, remove caller picture menu items */
   if (!IsSimValid)
   {
   	S32 MenuItemId;
   	
   	MenuItemId = GetChildMenuIDIndexByParentMenuID(MAIN_MENU_MATERIAL_MENUID,MENU_ID_PHB_CALLER_PIC_1);
   	if(MenuItemId != -1)	/*Menu Exists.*/
   	{
	   	nItems-=1;
   	}
   }
#endif
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(MAIN_MENU_MATERIAL_MENUID);
#endif
 
#ifdef __MMI_NUMBER_MENU__
	ShowCategory52Screen(	MAIN_MENU_MATERIAL_TEXT,MAIN_MENU_TITLE_MULTIMEDIA_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, (PU16)gIndexIconsImageList, 0, 0, 0, guiBuffer);
#else
	ShowCategory52Screen(	MAIN_MENU_MATERIAL_TEXT,MAIN_MENU_TITLE_MULTIMEDIA_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, ItemIcons, 0, 0, 0, guiBuffer);
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

void highlight_mainmenu_modetools(void)
{

	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(EntryModeTools,KEY_EVENT_UP);
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);

#ifndef __MMI_MAINLCD_160X128__
#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
		SetKeyHandler(EntryModeTools,KEY_RIGHT_ARROW,KEY_EVENT_UP);
#endif
}

void highlight_mainmenu_material(void)
{

	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(EntryMaterial,KEY_EVENT_UP);
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);

#ifndef __MMI_MAINLCD_160X128__
#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
		SetKeyHandler(EntryMaterial,KEY_RIGHT_ARROW,KEY_EVENT_UP);
#endif
}

#endif


#ifdef __MMI_MAINLCD_220X176__
/**************************************************************








	FUNCTION NAME		: highlight_mainmenu_tools

  	PURPOSE				: Associate the key handler
						  with left and rt soft key


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void highlight_mainmenu_tools(void)
{

 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

	SetLeftSoftkeyFunction(EntryTools,KEY_EVENT_UP);
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);

#if !defined(__MMI_MAINLCD_220X176__)
#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
		SetKeyHandler(EntryTools,KEY_RIGHT_ARROW,KEY_EVENT_UP);
#endif
}

#endif

/**************************************************************

	FUNCTION NAME		: highlight_mainmenu_settings

  	PURPOSE				: Associate the key handler
						  with left and rt soft key


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void highlight_mainmenu_settings(void)
{	
#if 1 //dyj del 20060514
 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_MAINLCD_128X128__) && !defined(__MMI_MAINLCD_160X128__)
#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
		SetKeyHandler(EntryScrSettingMenu,KEY_RIGHT_ARROW,KEY_EVENT_UP);
#endif
//micha0420
	SetLeftSoftkeyFunction(EntryScrSettingMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);
#endif
}


#ifdef __MMI_MESSAGES_CLUB__
/**************************************************************

	FUNCTION NAME		: highlight_mainmenu_msgclub

  	PURPOSE				: Associate the key handler
						  with left and rt soft key


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void highlight_mainmenu_msgclub (void)
{	
#if 0
 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

#ifdef __MOD_SMSAL__
	SetLeftSoftkeyFunction(mmi_msg_club_entry_level_1,KEY_EVENT_UP);// sms-code
#endif
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);
#endif
}
#endif

/**************************************************************

	FUNCTION NAME		: highlight_mainmenu_shortcuts

  	PURPOSE				: Associate the key handler
						  with left and rt soft key


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void highlight_mainmenu_shortcuts(void)
{
#if !defined(__MMI_MAINLCD_128X128__)
 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

	SetLeftSoftkeyFunction(EntryShctInMainMenu,KEY_EVENT_UP);

#if defined(__MMI_VERSION_2__) && !defined(__MMI_MESSAGES_CLUB__)
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);	
#else
	SetKeyHandler(EntryShctInMainMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#endif
#endif
}

#if defined(__MMI_EBOOK_READER__)


/*****************************************************************************
 * FUNCTION
 *  highlight_mainmenu_ebook
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void highlight_mainmenu_ebook(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(ebr_entry_book_shel_screen, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
    SetKeyHandler(ebr_entry_book_shel_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);//modified by renyh 2008/7/29
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);//modified by renyh 2008/7/29
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */	

}
#endif /* defined(__MMI_EBOOK_READER__) */ 

/**************************************************************

	FUNCTION NAME		: highlight_mainmenu_services

  	PURPOSE				: Associate the key handler
						  with left and rt soft key


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void highlight_mainmenu_services(void)
{	
#if 1// wangbei open 20060724
 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

//micha1223
#ifdef __MMI_MERGE_SAT_TO_MAINMENU__
//micha1217
#ifndef WAP_SUPPORT
//micha0601
	if (IsSATPresent(0))
#ifdef __MMI_MULTI_SIM__
		SetLeftSoftkeyFunction(GoToServicesMenu,KEY_EVENT_UP);
#else
		SetLeftSoftkeyFunction(GoToSATMainMenu,KEY_EVENT_UP);
#endif
	else	
		SetLeftSoftkeyFunction(GoToServicesMenu,KEY_EVENT_UP);
#else
	SetLeftSoftkeyFunction(GoToServicesMenu,KEY_EVENT_UP);
#endif
	
#else

	SetLeftSoftkeyFunction(GoToServicesMenu,KEY_EVENT_UP);
	#if !defined(__MMI_MAINLCD_128X128__)
	SetKeyHandler(GoToServicesMenu,KEY_RIGHT_ARROW,KEY_EVENT_UP);
	#endif

#endif
	
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);
#endif
}

/**************************************************************

	FUNCTION NAME		: initalize_main_menu_application

  	PURPOSE				: Set all highlte handler for main menu


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void initalize_main_menu_application(void)
{
	U8 i;
	SetHiliteHandler(MAIN_MENU_PROFILES_MENUID,highlight_mainmenu_profiles);
#if defined(__MMI_MAINLCD_220X176__)
	SetHiliteHandler(MAIN_MENU_MP3_PLAYER_MENUID, HighlightAudioPlayer );
#endif
	SetHiliteHandler(MAIN_MENU_PHONEBOOK_MENUID,highlight_mainmenu_phonebook);
	SetHiliteHandler(MAIN_MENU_PHONEBOOK_NO_SIM_MENUID,highlight_mainmenu_phonebook);
	SetHiliteHandler(MAIN_MENU_MESSAGES_MENUID,highlight_mainmenu_messages);
	SetHiliteHandler(MAIN_MENU_FUNANDGAMES_MENUID,highlight_mainmenu_funandgames);
	SetHiliteHandler(MAIN_MENU_ORGANIZER_MENUID,highlight_mainmenu_organizer);
	SetHiliteHandler(MAIN_MENU_SETTINGS_MENUID,highlight_mainmenu_settings);
	SetHiliteHandler(MAIN_MENU_SETTINGS_MENUID_NO_SIM,highlight_mainmenu_settings);//added by zhoumn@2007/07/24
#ifdef __MMI_MESSAGES_CLUB__
	SetHiliteHandler(MAIN_MENU_MSGCLUB_MENUID,highlight_mainmenu_msgclub);
#endif
	SetHiliteHandler(MAIN_MENU_SHORTCUTS_MENUID,highlight_mainmenu_shortcuts);
	SetHiliteHandler(MAIN_MENU_SERVICES_MENUID,highlight_mainmenu_services);

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MAIN_MENU_CALL_HISTORY+i,highlight_mainmenu_callhistory);
	}
	 	

#ifdef __T800_MAIN_MENU__
	SetHiliteHandler(MAIN_MENU_CALL_CENTER,highlight_mainmenu_callcenter);
	SetHiliteHandler(MAIN_MENU_CALL_HISTORY,highlight_mainmenu_callhistory);
#endif

	SetHiliteHandler(MAIN_MENU_MULTIMEDIA_MENUID,highlight_mainmenu_multimedia);

	
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
	SetHiliteHandler(MAIN_MENU_OPTION_AVZONE_MENUID,highlight_mainmenu_avzone);
#endif

#if defined(__MMI_MAINLCD_160X128__) && defined(__MMI_BLACKBERRY_QWERTY__)
	SetHiliteHandler(MAIN_MENU_MODETOOLS_MENUID,highlight_mainmenu_modetools);
	SetHiliteHandler(MAIN_MENU_MATERIAL_MENUID,highlight_mainmenu_material);
#endif
#if defined(__MMI_FILE_MANAGER__)
	SetHiliteHandler(MAIN_MENU_FILE_MNGR_MENUID,highlight_mainmenu_filemngr);
#endif

#if defined(__MMI_VERSION_2__)
	SetHiliteHandler(MAIN_MENU_EXTRA_MENUID,highlight_mainmenu_extra);
#endif	
	 
	initialize_mainmenu_title_icons();
}


/**************************************************************

	FUNCTION NAME		: main_menu_shortcut_executer

  	PURPOSE				: Shortcut handler to main menu


	INPUT PARAMETERS	: S32 index

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void main_menu_shortcut_executer(S32 index)
{
	S32 n_items=GetNumOfChild(IDLE_SCREEN_MENU_ID);
	if(index>=0 && index<n_items) execute_left_softkey_function(KEY_EVENT_UP);
}

 
#if defined(__MMI_MAINMENU_TAB_SUPPORT__) && defined(__MMI_MAINMENU_STYLE_CHANGE_EN__)  
#include "wgui_categories_tab.h"
/*************************************************************************

	FUNCTION NAME		: set_main_menu_tab_pane

  	PURPOSE				: Set tab pane function for main menu in tab pane

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void set_main_menu_tab_pane(void)
{
	UI_string_ID_type mm_stringIDs[MAX_MAIN_MENU_ITEMS];
	UI_image_ID_type mm_iconIDs[MAX_MAIN_MENU_ITEMS];
	UI_image_ID_type mm_iconIDs_display[MAX_MAIN_MENU_ITEMS];
	UI_image_ID_type *iconList;
	S32 n_items;
	S32 i;
	n_items=GetNumOfChild(IDLE_SCREEN_MENU_ID);
	GetSequenceStringIds(IDLE_SCREEN_MENU_ID,mm_stringIDs);
	GetSequenceImageIds(IDLE_SCREEN_MENU_ID,mm_iconIDs);
	for( i=0;  i<(MAIN_MENU_TAB_ICON_IMGBASE_MAX - MAIN_MENU_TAB_ICON_IMGBASE); i++)
	 	 mm_iconIDs_display[i] = MAIN_MENU_TAB_ICON_IMGBASE+( mm_iconIDs[i] -MAIN_MENU_ICON_IMGBASE  ) ;
	iconList=mm_iconIDs_display;
	SetParentHandler(IDLE_SCREEN_MENU_ID);
	register_tab_pane_hilite_handler(ExecuteCurrHiliteHandler);
	set_tab_pane(n_items,mm_stringIDs,iconList,0);
}
#endif
 

void main_menu_custom_highlight_handler(S32 index)
{
#ifndef __MMI_MATRIX_MAIN_MENU_NO_TITLEBAR__ 

#if  defined (__MMI_MAINLCD_128X160__)
	U16 no_sim_mm_title_icons[] = {
								    MAIN_MENU_TITLE_PHONEBOOK_ICON,
								    MAIN_MENU_TITLE_MULTIMEDIA_ICON,
								    MAIN_MENU_TITLE_FUNANDGAMES_ICON,
								    MAIN_MENU_TITLE_ORGANIZER_ICON,
								    MAIN_MENU_TITLE_SETTINGS_ICON,
								    MAIN_MENU_TITLE_FILE_MNGR_ICON,
								    MAIN_MENU_TITLE_PROFILES_ICON};
#else
	U16 no_sim_mm_title_icons[] = {
								    MAIN_MENU_TITLE_PHONEBOOK_ICON,
								    MAIN_MENU_TITLE_MULTIMEDIA_ICON,
								    MAIN_MENU_TITLE_FUNANDGAMES_ICON,
								    MAIN_MENU_TITLE_ORGANIZER_ICON,
								    MAIN_MENU_TITLE_SETTINGS_ICON,
								    MAIN_MENU_TITLE_FILE_MNGR_ICON,
								    MAIN_MENU_TITLE_PROFILES_ICON,
								    MAIN_MENU_TITLE_EXTRA_APP_ICON};
#endif
#endif
  
   #ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
		gdi_layer_lock_frame_buffer();
   #endif
   
//CSD JL add no key clear hilite
	ExecuteCurrHiliteHandler_Ext(index);
//CSD JL end
 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
		gdi_layer_unlock_frame_buffer();
		coolsand_UI_BLT_double_buffer(0, UI_device_height-MMI_BUTTON_BAR_HEIGHT,
			UI_device_width - 1, UI_device_height  - 1);
#endif
 
 
#ifndef __MMI_MATRIX_MAIN_MENU_NO_TITLEBAR__ 
	if (MMI_main_menu_type!=ROTATE_MENU && MMI_main_menu_type!=CIRCULAR_3D_MENU) 
	{
		if(GetActiveScreenId() == SCR_SIM_OPTION)
		{
			ChangeTitleIcon((U16)no_sim_mm_title_icons[index]);
		}
		else
		{
			ChangeTitleIcon((U16)gMainMenuTitleIcon[index]);
		}

		draw_title();
	}
#endif
 
	//if (MMI_main_menu_type == ROTATE_MENU)
	main_menu_index=index;
}

 
void EntryMainMenuFromIdleScreen(void)
{
#ifdef	__MMI_KEYPAD_LOCK_PATTERN_2__
    g_idle_context.ToMainMenuScrFromIdleApp = 1;
#endif
	mmi_trace(1,"EntryMainMenuFromIdleScreen");
    goto_main_menu(); 
}

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
extern void ShowCategory414Screen( U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,S32 number_of_items,U16* list_of_items,U16* list_of_icons,U16* list_of_icons2,  U16* list_of_menuitem_id, S32 flags,S32 highlighted_item,U8* history_buffer);
void list_matrix_main_menu_highlight_handler(S32 index)
{
	S32 n_items;
	U16 nMenuItemList[MAX_SUB_MENUS];

	UI_string_ID_type mm_stringIDs[MAX_SUB_MENUS];
	UI_image_ID_type mm_iconIDs[MAX_SUB_MENUS];
	byte *history_buffer;
	list_matrix_menu_category_history *h;

	GetSequenceItemIds (IDLE_SCREEN_MENU_ID, nMenuItemList);
	if ( nMenuItemList[index]==MAIN_MENU_FILE_MNGR_MENUID )
		return;
	history_buffer=GetCurrGuiBuffer(MAIN_MENU_SCREENID);
	GetSequenceStringIds(nMenuItemList[index],mm_stringIDs);
	GetSequenceImageIds(nMenuItemList[index],mm_iconIDs);
	SetParentHandler(nMenuItemList[index]);
	n_items=GetNumOfChild(nMenuItemList[index]);
	MMI_matrix_highlight_handler=ExecuteCurrHiliteHandler;
	ChangeTitleString((U8*)GetString(GetStringIdOfItem(nMenuItemList[index])));
	MMI_title_icon=NULL;
	draw_title();
	if ( history_buffer!=NULL)
	{
		h = (list_matrix_menu_category_history*)history_buffer;
		ShowCat414Matrix(n_items,mm_stringIDs,mm_iconIDs,MATRIX_MENU,0,&(h->matrix_menu_history));
	}
	else
		ShowCat414Matrix(n_items,mm_stringIDs,mm_iconIDs,MATRIX_MENU,0,NULL);
}
#endif

#ifdef __MMI_WGUI_MINI_TAB_BAR__
extern U16* GetSeqItems(U16 ParentItemId);
#endif

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#ifdef __MMI_BLACK_BERRY_MAIN_MENU__
typedef struct 
{
	U16 menuid;
	U16 imageid;
}BBM_IMAGE;

const BBM_IMAGE ept_bbm_image[] = 
{
	{MAIN_MENU_MESSAGES_MENUID	,0},
	{MAIN_MENU_CALL_HISTORY		,1},
	{MAIN_MENU_PHONEBOOK_MENUID,2},
	{MAIN_MENU_PROFILES_MENUID	,3},
	{MAIN_MENU_SETTINGS_MENUID	,4},
	{MAIN_MENU_FUNANDGAMES_MENUID,5},
	{MENU_ID_CAMERA_APP			,6},
	{MENU_ID_VDOREC_APP			,7},
	{AUDIO_PLAYER_MAIN_MENUID	,8},
	{MENU_ID_VDOPLY_APP			,9},
	{MENU_ID_FMRDO_MAIN			,10},
	{MENU_CONN_BT_MAIN			,11},
	{MAIN_MENU_SERVICES_MENUID	,12},
	{MENU_ID_SNDREC_MAIN		,13},
	{MAIN_MENU_FILE_MNGR_MENUID	,14},
	{ORGANIZER_CALCULATOR_MENU		,15},
	{MENU_ID_IMGVIEW_APP			,16},
	{MENU_IDLE_SCR_DISP			,17},
	{ORGANIZER_CALENDER_MENU	,18},
	{ORGANIZER_ALARM_MENU		,19},
	{MAIN_MENU_EBOOK_MENUID		,20},
	{ORGANIZER_WORLDCLOCK_MENU	,21},
	{ORGANIZER_TODOLIST_MENU		,22},
	{ORGANIZER_CURRENCYCONVERTOR_MENU	,23},
	{MENU_STOPWATCH			,24},
	{MENU_SETTING_SCHEDULE_POWER	,25},
};

U16 bbm_mainmenu_idx[MAX_MAIN_MENU_ITEMS];

U16 bbm_get_image_idx_by_menu(U16 menuid)
{
	U16 idx = 0;
	int i  =0 ;
	int size  = sizeof(ept_bbm_image) / sizeof(BBM_IMAGE);

	for( i = 0; i < size; i++)
	{
		if(menuid == ept_bbm_image[i].menuid)
		{
			idx = ept_bbm_image[i].imageid;
			break;
		}
	}

	return idx;
}

#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */

/**************************************************************

	FUNCTION NAME		: goto_main_menu

  	PURPOSE				: Display the main menu.



	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void goto_main_menu(void)
{

	UI_string_ID_type mm_stringIDs[MAX_MAIN_MENU_ITEMS];
	UI_image_ID_type mm_iconIDs[MAX_MAIN_MENU_ITEMS];
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
	UI_image_ID_type mm_iconID2[MAX_MAIN_MENU_ITEMS];
	U16 nMenuItemList[MAX_SUB_MENUS];
#endif
	byte *history_buffer;
	S32 n_items;
	S32 attributes;
	U8 HighlightMenu=0;
	UI_image_ID_type *iconList;

#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
	U8 menu_style=0;
	UI_image_ID_type mm_iconIDs_display[MAX_MAIN_MENU_ITEMS];
	
#endif
	U8 i=0;
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
     U8 j=0;
#endif
	mmi_phb_reset_scr_id();  //dyj del 20060514

	EntryNewScreen(MAIN_MENU_SCREENID,exit_main_menu, NULL, NULL);

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#ifdef __MMI_BLACK_BERRY_MAIN_MENU__
    /* hide empty extra menu */
    if (GetNumOfChild_Ext(MAIN_MENU_EXTRA_MENUID) == 0)
    {
        mmi_frm_hide_menu_item(MAIN_MENU_EXTRA_MENUID);
    }

    /* hide empty multimedia menu */
    if (GetNumOfChild_Ext(MAIN_MENU_MULTIMEDIA_MENUID) == 0)
    {
        mmi_frm_hide_menu_item(MAIN_MENU_MULTIMEDIA_MENUID);
    }

    /* hide empty fun and games menu */
    if (GetNumOfChild_Ext(MAIN_MENU_FUNANDGAMES_MENUID) == 0)
    {
        mmi_frm_hide_menu_item(MAIN_MENU_FUNANDGAMES_MENUID);
    }
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */

	history_buffer=GetCurrGuiBuffer(MAIN_MENU_SCREENID);
	n_items=GetNumOfChild(IDLE_SCREEN_MENU_ID);
	GetSequenceStringIds(IDLE_SCREEN_MENU_ID,mm_stringIDs);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#ifdef __MMI_BLACK_BERRY_MAIN_MENU__
	{
		U16 bbm_ids[MAX_MAIN_MENU_ITEMS];
		int i = 0;
	
		GetSequenceItemIds(IDLE_SCREEN_MENU_ID,bbm_ids);
	
		ASSERT(n_items <=MAX_MAIN_MENU_ITEMS);
		
		for( i = 0; i < n_items; i++)
		{
			mm_iconIDs[i ] = MAIN_MENU_ICON_01 + bbm_get_image_idx_by_menu(bbm_ids[i]);
			bbm_mainmenu_idx[i] = mm_iconIDs[i];
		}
	}
#else
	GetSequenceImageIds(IDLE_SCREEN_MENU_ID,mm_iconIDs);
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */

	SetParentHandler(IDLE_SCREEN_MENU_ID);
	attributes=GetDispAttributeOfItem(IDLE_SCREEN_MENU_ID);

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
	GetSequenceItemIds (IDLE_SCREEN_MENU_ID, nMenuItemList);
#endif
 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_setup_mini_tab_bar(MMI_content_x, MMI_content_y, MMI_content_width, 20, UI_COLOR_BLACK, UI_COLOR_RED, UI_COLOR_GREY, 9/*n_items*/, GetSeqItems(IDLE_SCREEN_MENU_ID), NULL);
	mmi_wgui_mini_tab_bar_set_flag_on(0x00000002/*WGUI_MINI_TAB_BAR_IN_PARENT*/);
#endif
 

	//RegisterHighlightHandler(main_menu_custom_highlight_handler);

#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
      menu_style  = PhnsetGetMainMenuStyle();
	switch( menu_style )
	   {
#ifdef __MMI_MAINMENU_MATRIX_SUPPORT__  
	      case PHNSET_MAINMENU_STYLE_MATRIX:
	         attributes = MATRIX_MENU;
	         for( i=0;  i<(MAIN_MENU_MATRIX_ICON_IMGBASE_MAX-MAIN_MENU_MATRIX_ICON_IMGBASE); i++)
			 {
				#if defined(__MMI_MAINLCD_220X176__)
				mm_iconIDs_display[i] = MAIN_MENU_MATRIX_CALL_HISTORY_ICON+i ; 
				#elif defined(__MMI_USE_DRAW_FOCUS_IN_MAINMENU__)
				mm_iconIDs_display[i] =MAIN_MENU_SELECT_BG_ICON;
				#else
				mm_iconIDs_display[i] = MAIN_MENU_MATRIX_ICON_IMGBASE+( mm_iconIDs[i] -MAIN_MENU_ICON_IMGBASE  ) ;
				#endif
			 }
	         iconList = mm_iconIDs_display;
	         break;
#endif

#ifdef __MMI_MAINMENU_PAGE_SUPPORT__  
	      case PHNSET_MAINMENU_STYLE_PAGE:
	         attributes = PAGE_MENU;
	         for( i=0;  i<(MAIN_MENU_PAGE_ICON_IMGBASE_MAX-MAIN_MENU_PAGE_ICON_IMGBASE); i++)
	            mm_iconIDs_display[i] = MAIN_MENU_PAGE_ICON_IMGBASE+( mm_iconIDs[i] -MAIN_MENU_ICON_IMGBASE  ) ;
	         iconList = mm_iconIDs_display;
	         break;
#endif

#ifdef __MMI_MAINMENU_LIST_SUPPORT__  
	      case PHNSET_MAINMENU_STYLE_LIST:
	         attributes = LIST_MENU;
	         for( i=0;  i<(MAIN_MENU_LIST_ICON_IMGBASE_MAX-MAIN_MENU_LIST_ICON_IMGBASE); i++)
	            mm_iconIDs_display[i] = MAIN_MENU_LIST_ICON_IMGBASE+( mm_iconIDs[i] -MAIN_MENU_ICON_IMGBASE  ) ;
	         iconList = mm_iconIDs_display;
	         break;
#endif

#ifdef __MMI_MAINMENU_CIRCULAR_SUPPORT__ 
	      case PHNSET_MAINMENU_STYLE_CIRCULE:
	         attributes = CIRCULAR_3D_MENU;
	         for( i=0;  i<(MAIN_MENU_CIRCULAR_ICON_IMGBASE_MAX-MAIN_MENU_CIRCULAR_ICON_IMGBASE); i++)
	            mm_iconIDs_display[i] = MAIN_MENU_CIRCULAR_ICON_IMGBASE+( mm_iconIDs[i] -MAIN_MENU_ICON_IMGBASE  ) ;
	         iconList = mm_iconIDs_display;
	         break;
#endif

#ifdef __MMI_MAINMENU_ROTATE_SUPPORT__
		 case PHNSET_MAINMENU_STYLE_ROTATE:
	         attributes = ROTATE_MENU;
		 for( i=0;  i<(MAIN_MENU_ROTATE_ICON_IMGBASE_MAX-MAIN_MENU_ROTATE_ICON_IMGBASE); i++)
	            mm_iconIDs_display[i] = MAIN_MENU_ROTATE_ICON_IMGBASE+( mm_iconIDs[i] -MAIN_MENU_ICON_IMGBASE  ) ;
	         iconList = mm_iconIDs_display;
	         break;
#endif

 
#ifdef __MMI_MAINMENU_TAB_SUPPORT__  
		 case PHNSET_MAINMENU_STYLE_TAB:
			 {
				register_set_tab_pane_callback(set_main_menu_tab_pane,
				NULL,
				NULL);
				show_category_one_icon_tab_pane();
				return;
			 }
#endif
 
	      default:
	         iconList = mm_iconIDs;
	         break;
	   }
#else
#if defined(__MMI_USE_DRAW_FOCUS_IN_MAINMENU__)
 for( i=0;  i<n_items; i++)

			 {
				mm_iconIDs[i] =MAIN_MENU_SELECT_BG_ICON;
			 }
 #endif
	iconList = mm_iconIDs;
#endif
 
/*
 if((attributes==CIRCULAR_3D_MENU)||(attributes==ROTATE_MENU))
	RegisterHighlightHandler(ExecuteCurrHiliteHandler_Ext); //CSD JL add no key clear hilite
 else
	RegisterHighlightHandler(main_menu_custom_highlight_handler);
 */
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
	RegisterHighlightHandler(list_matrix_main_menu_highlight_handler);
#else
	RegisterHighlightHandler(main_menu_custom_highlight_handler);
#endif
 
 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
	coolsand_UI_register_button_shuffle(shuffle_draw_white);
#endif
 
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
         for( j=0;  j<4; j++)
	            mm_iconID2[j] = mm_iconIDs[j]+1 ;
#endif
#ifdef __T800_MAIN_MENU__
    if (((n_items + 2)/ 3) % 2)
    {
        HighlightMenu = ((((n_items + 2)/ 3) * 3) - 1) >> 1;
    }
    else
    {
        HighlightMenu = ((((n_items + 2)/ 3) * 3) - 1 - 3) >> 1;
    }
#endif

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
	ShowCategory414Screen(MAIN_MENU_TITLE_TEXT,MAIN_MENU_TITLE_ICON,STR_GLOBAL_OK,IMG_GLOBAL_OK,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,n_items,mm_stringIDs,iconList,mm_iconID2,nMenuItemList,attributes,HighlightMenu,(byte*)history_buffer);
#else
#ifdef __MMI_MAINLCD_220X176__
	/*No title in the main menu screen.*/
	ShowCategory14Screen(0,0,STR_GLOBAL_OK,IMG_GLOBAL_OK,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,n_items,mm_stringIDs,iconList,attributes,HighlightMenu,(byte*)history_buffer);
#else
	ShowCategory14Screen(MAIN_MENU_TITLE_TEXT,MAIN_MENU_TITLE_ICON,STR_GLOBAL_OK,IMG_GLOBAL_OK,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,n_items,mm_stringIDs,iconList,attributes,HighlightMenu,(byte*)history_buffer);
#endif
#endif
#if (defined(__MMI_KEYPAD_LOCK_PATTERN_2__) && !defined(__MMI_DISABLE_KEYPAD_LOCK__) )
/*zhangm Add Start For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
//     if( g_idle_context.ToMainMenuScrFromIdleApp == 1)
//      {
//	    StartTimer(KEYPAD_LOCK_TIMER, KEYPAD_LOCK_TIMEOUT, IdleHandleKeypadLockProcess);
//  	    SetKeyHandler(IdleHandlePoundKeyForKeypadLock,KEY_STAR,KEY_EVENT_UP);	
//      }
	SetKeyHandler(IdleHandleStarKeyForKeypadLock,KEY_STAR,KEY_LONG_PRESS);
/*zhangm Add end For 10821 Ver: GreenStone  on 2008-12-19 16:23 */
#endif
	register_menu_shortcut_selected(main_menu_shortcut_executer);   
	  SetRightSoftkeyFunction(UI_dummy_function,KEY_EVENT_DOWN); 
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);   
  
#ifdef	__MMI_KEYPAD_LOCK_PATTERN_2__
	if(g_idle_context.ToMainMenuScrFromIdleApp)
		SetKeyHandler(IdleHandleStarKeyForKeypadLock,KEY_SEND,KEY_EVENT_DOWN);
#endif
}

/**************************************************************

	FUNCTION NAME		: exit_main_menu

  	PURPOSE				: Store the  snap shot
						  before displaying next screen


	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void exit_main_menu(void)
{	history_t main_menu_history;
	S16		nHistory =	0;
	pfnUnicodeStrcpy ((S8*)main_menu_history.inputBuffer, (S8*)&nHistory);
	main_menu_history.scrnID=MAIN_MENU_SCREENID;
	main_menu_history.entryFuncPtr=goto_main_menu;
	GetCategoryHistory(main_menu_history.guiBuffer);
	AddHistory(main_menu_history);
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_mini_tab_bar_set_flag_off(WGUI_MINI_TAB_BAR_IN_PARENT);
#endif
#ifdef	__MMI_KEYPAD_LOCK_PATTERN_2__   
	g_idle_context.ToMainMenuScrFromIdleApp = 0;
#endif
}

#ifdef __T800_MAIN_MENU__
 void InitCallLogMainMenuHanler(void)
{

}
extern void HighlightManageCalls(void);
void mmi_entry_call_center(void)
{
	U8* guiBuffer;
	U16 nStrItemList[MAX_SUB_MENUS];
	U16 nImgIltemList[MAX_SUB_MENUS];
	U16 nNumofItem;
	U8 *hintList[MAX_SUB_MENUS]; /*Array for hint*/

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_entry_call_center.>\n",__FILE__,__LINE__));

	EntryNewScreen(SCR_CALL_CENTER, NULL, mmi_entry_call_center,NULL);
    
	guiBuffer = GetCurrGuiBuffer(SCR_CALL_CENTER);		
      nNumofItem = GetNumOfChild(MAIN_MENU_CALL_CENTER);
      GetSequenceStringIds(MAIN_MENU_CALL_CENTER, nStrItemList);
      GetSequenceImageIds(MAIN_MENU_CALL_CENTER, nImgIltemList);
      SetParentHandler(MAIN_MENU_CALL_CENTER);
      ConstructHintsList(MAIN_MENU_CALL_CENTER, hintList);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	InitCallLogMainMenuHanler();
	SetHiliteHandler(MENU8237_SCR8093_MNGCALL_MENU_MAIN, HighlightManageCalls);

	ShowCategory52Screen(STR_ID_CALL_CENTER, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList,nImgIltemList,
						  hintList,
						  0,0, guiBuffer);   
}

void highlight_mainmenu_callcenter(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

    /* ucm module need  to be added to support this function */
    SetLeftSoftkeyFunction(mmi_entry_call_center, KEY_EVENT_UP);
//    SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up, KEY_EVENT_UP);
}
#endif

void highlight_mainmenu_callhistory(void)
{
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
		ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
		ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

	SetLeftSoftkeyFunction(EntryCHISTMainMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(handle_mainmenu_right_softkey_up,KEY_EVENT_UP);

	#if defined(__MMI_MULTI_SIM__)
	SetCHISContext(GetReqSrcModIndex());
	#endif

#if defined(__MMI_MULTI_SIM__) && !defined(__MMI_MAINLCD_128X128__)
	
	if (GetActiveScreenId() != MAIN_MENU_SCREENID)
	{
		SetKeyHandler(EntryCHISTMainMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	}
#endif /* __MMI_MULTI_SIM__ */
}

#ifdef __MMI_MAINLCD_160X128__
extern void mmi_audply_entry_main();
#endif

void highlight_mainmenu_multimedia(void)
{ 
 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

#ifdef __MMI_MAINLCD_160X128__
	SetLeftSoftkeyFunction(mmi_audply_entry_main,KEY_EVENT_UP);
#else
	SetLeftSoftkeyFunction(EntryMainMultimedia,KEY_EVENT_UP);
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_MAINLCD_128X128__) && !defined(__MMI_MAINLCD_160X128__)
#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
		SetKeyHandler(EntryMainMultimedia,KEY_RIGHT_ARROW,KEY_EVENT_UP);
#endif

}

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
void highlight_mainmenu_avzone(void)
{
#if 0   //dyj del 20050614
 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

	SetLeftSoftkeyFunction(EntryMainAVZone,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context.PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context.PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
		SetKeyHandler(EntryMainAVZone,KEY_RIGHT_ARROW,KEY_EVENT_UP);
#endif
}
#endif


void highlight_mainmenu_filemngr(void)
{
	TRACE_FMGR_FUNCTION();
/*WUZC Modify Start For MMIFS Ver:    on 2007-1-19 10:53 */

#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

#ifdef __MMI_FILE_MANAGER__
	SetLeftSoftkeyFunction(fmgr_launch,KEY_EVENT_UP);
#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_MAINLCD_160X128__)
	#if defined(__MMI_VERSION_2__)  
		#ifdef __FLIGHT_MODE_SUPPORT__
		if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
		#else
		if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
		#endif
			SetKeyHandler(fmgr_launch,KEY_RIGHT_ARROW,KEY_EVENT_UP);

       #else
   	   	SetKeyHandler(fmgr_launch, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	#endif
#endif
#else

    SetLeftSoftkeyFunction(NULL,KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#endif
/*WUZC Modify End  For MMIFS Ver:    on 2007-1-19 10:53 */
}

#if defined(__MMI_VERSION_2__)
void highlight_mainmenu_extra(void)
{

 
#ifdef __MMI_MATRIX_MAIN_MENU_FULL_BACKGROUND__
	if (inMatrixNoTitleFlag)
	{
	ChangeLeftSoftkeyByStringLength(MMI_highlighted_item_text, NULL);
	ChangeRightSoftkeyByStringLength((UI_string_type)GetString(STR_GLOBAL_BACK), (UI_image_type)GetImage(IMG_GLOBAL_BACK));
	}
	else 
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	}
#else
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#endif

	SetLeftSoftkeyFunction(EntryMainExtra,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_MAINLCD_128X128__) && !defined(__MMI_MAINLCD_160X128__)
#ifdef __FLIGHT_MODE_SUPPORT__
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR || mmi_bootup_get_active_flight_mode() != 0)
#else
	if(g_pwr_context[0].PrevScreenIndicator != ENTER_IDLE_SCR)
#endif
		SetKeyHandler(EntryMainExtra,KEY_RIGHT_ARROW,KEY_EVENT_UP);	
#endif
}
#endif

 
#define SCR_FILE_MANAGER	FILE_MANAGER_BASE
#define SCR_MMEDIA			MMEDIA_BASE
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
#define	SCR_AVZONE			MMEDIA_BASE+1
#endif
#define SCR_EXTRA			EXTRA_APP_BASE

extern SecuritySetupContext g_SecuritySetupContext;
/**************************************************************

	FUNCTION NAME		: is_phb_init_done

  	PURPOSE				: 用于标志是否可以进入多媒体,屏保,背光,键盘锁


	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: is phb init done

 

**************************************************************/
extern int checkUsbstate(void);

void EntryMainMultimedia(void)
{
	U16		ItemList[MAX_SUB_MENUS];
	U16		ItemIcons[MAX_SUB_MENUS];
	U16		nItems;
	U8*		guiBuffer;
	BOOL 	IsSimValid = MMI_FALSE;
	U16 		i;
#if 0
	if(!is_phb_init_done())
	{
	    //开机初始化 stringid =25803  modified by renyh  2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_PROGRESS/*IMG_GLOBAL_ACTIVATED*/, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);//modified by lin for bug 6761@20070929
		return;
	}
#endif
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
#if defined(__SG_MMI_MEDIA_SPECIAL_MENU__)
	EntryNewScreen(SCR_MMEDIA,exit_multimedia_menu_screen,EntryMainMultimedia,NULL);
#else
	EntryNewScreen(SCR_MMEDIA,NULL, EntryMainMultimedia, NULL);
#endif	
	guiBuffer = GetCurrGuiBuffer(SCR_MMEDIA);
	nItems = GetNumOfChild(MAIN_MENU_MULTIMEDIA_MENUID);
	GetSequenceStringIds(MAIN_MENU_MULTIMEDIA_MENUID,ItemList);
	GetSequenceImageIds(MAIN_MENU_MULTIMEDIA_MENUID,ItemIcons);
	SetParentHandler(MAIN_MENU_MULTIMEDIA_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].IsSimValid)
		{
			IsSimValid = MMI_TRUE;
			break;
		}
	}
#if defined(__MMI_VRSD__) || defined(__MMI_VRSI__)
   /* No SIM, remove voice dial menu items */
   if (!IsSimValid)
      nItems-=1;
#endif

#if defined(__MMI_PHB_CALLER_RES_SETTING__)
   /* No SIM, remove caller picture menu items */
   if (!IsSimValid)
   {
   	S32 MenuItemId;
   	
   	MenuItemId = GetChildMenuIDIndexByParentMenuID(MAIN_MENU_MULTIMEDIA_MENUID,MENU_ID_PHB_CALLER_PIC_1);
   	if(MenuItemId != -1)	/*Menu Exists.*/
   	{
	   	nItems-=1;
   	}
   }
#endif

#if defined( __MMI_FM_RADIO__ ) && defined( __MMI_FM_RADIO_CHECK_CHIP_OR_HIDE__ )
	RegisterHighlightHandler( mmi_fmrdo_skip_highlight_hdlr );
	nItems = mmi_fmrdo_check_chip_or_hide( nItems, ItemList, ItemIcons );
#else
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
#endif
 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(MAIN_MENU_MULTIMEDIA_MENUID);
#endif
 
#ifdef __MMI_NUMBER_MENU__
#ifdef __MMI_MAINLCD_220X176__
	ShowCategory52Screen(	MAIN_MENU_MMEDIA_TEXT,GetRootTitleIcon(MAIN_MENU_MULTIMEDIA_MENUID),
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								nItems, ItemList, (PU16)gIndexIconsImageList, 0, 0, 0, guiBuffer);

#else

#if defined(__SG_MMI_MEDIA_SPECIAL_MENU__)
	setup_media_screen_menu();
	ShowCategorymediamenuScreen(STR_GLOBAL_OK,STR_GLOBAL_BACK, guiBuffer);
	mmi_display_multimedia_menu();
#else
	ShowCategory52Screen(	MAIN_MENU_MMEDIA_TEXT,MAIN_MENU_TITLE_MULTIMEDIA_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, (PU16)gIndexIconsImageList, 0, 0, 0, guiBuffer);
#endif

#endif
#else
#ifdef __MMI_MAINLCD_220X176__
	ShowCategory52Screen(	MAIN_MENU_MMEDIA_TEXT,GetRootTitleIcon(MAIN_MENU_MULTIMEDIA_MENUID),
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, ItemIcons, 0, 0, 0, guiBuffer);
#else

	ShowCategory52Screen(	MAIN_MENU_MMEDIA_TEXT,MAIN_MENU_TITLE_MULTIMEDIA_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, ItemIcons, 0, 0, 0, guiBuffer);
#endif
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
 #if !defined(__MMI_MAINLCD_128X128__)
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
 #endif
}


void EntryMainFileMngr(void)
{
	/* shall be implemented later */
}

void ExitMainFileMngr(void)
{
	/* shall be implemented later */
}

#if defined(__MMI_VERSION_2__)
void EntryMainExtra(void)
{
	U16		ItemList[MAX_SUB_MENUS];
	U16		nItems;
	U8*		guiBuffer;
  
  #ifndef __MMI_NUMBER_MENU__
	U16		ItemIcons[MAX_SUB_MENUS];
  #endif

	EntryNewScreen(SCR_EXTRA,NULL, EntryMainExtra, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_EXTRA);
	nItems = GetNumOfChild(MAIN_MENU_EXTRA_MENUID);
	GetSequenceStringIds(MAIN_MENU_EXTRA_MENUID,ItemList);
  #ifndef __MMI_NUMBER_MENU__
	GetSequenceImageIds(MAIN_MENU_EXTRA_MENUID,ItemIcons);
  #endif
	SetParentHandler(MAIN_MENU_EXTRA_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

#ifdef __MMI_NUMBER_MENU__
	ShowCategory52Screen(	MAIN_MENU_EXTRA_TEXT,MAIN_MENU_TITLE_EXTRA_APP_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, (PU16)gIndexIconsImageList, 0, 0, 0, guiBuffer	);

#else
	ShowCategory52Screen(	MAIN_MENU_EXTRA_TEXT,MAIN_MENU_TITLE_EXTRA_APP_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, ItemIcons, 0, 0, 0, guiBuffer	);
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#if !defined(__MMI_MAINLCD_128X128__)
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
#endif
}

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
void EntryMainAVZone(void)
{
	U16		ItemList[MAX_SUB_MENUS];
	U16		ItemIcons[MAX_SUB_MENUS];
	U16		nItems;
	U8*		guiBuffer;
	BOOL 	IsSimValid = MMI_FALSE;
	U16 		i;

	EntryNewScreen(SCR_AVZONE,NULL, EntryMainAVZone, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_AVZONE);
	nItems = GetNumOfChild(MAIN_MENU_OPTION_AVZONE_MENUID);
	GetSequenceStringIds(MAIN_MENU_OPTION_AVZONE_MENUID,ItemList);
	GetSequenceImageIds(MAIN_MENU_OPTION_AVZONE_MENUID,ItemIcons);
	SetParentHandler(MAIN_MENU_OPTION_AVZONE_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].IsSimValid)
		{
			IsSimValid = MMI_TRUE;
			break;
		}
	}

#if defined(__MMI_VRSD__) || defined(__MMI_VRSI__)
   /* No SIM, remove voice dial menu items */
   if (!IsSimValid)
      nItems-=1;
#endif

#if defined(__MMI_PHB_CALLER_RES_SETTING__)
   /* No SIM, remove caller picture menu items */
   if (!IsSimValid)
   {
   	S32 MenuItemId;
   	
   	MenuItemId = GetChildMenuIDIndexByParentMenuID(MAIN_MENU_OPTION_AVZONE_MENUID,MENU_ID_PHB_CALLER_PIC_1);
   	if(MenuItemId != -1)	/*Menu Exists.*/
   	{
	   	nItems-=1;
   	}
   }
#endif

#if defined( __MMI_FM_RADIO__ ) && defined( __MMI_FM_RADIO_CHECK_CHIP_OR_HIDE__ )
	RegisterHighlightHandler( mmi_fmrdo_skip_highlight_hdlr );
	nItems = mmi_fmrdo_check_chip_or_hide( nItems, ItemList, ItemIcons );
#else
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
#endif
/*
 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(MAIN_MENU_AVZONE_MENU_ID);
#endif
 
*/
	ShowCategory52Screen(	MAIN_MENU_AVZONE_TEXT,MAIN_MENU_TITLE_MULTIMEDIA_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, ItemIcons, 0, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
#endif

#endif

