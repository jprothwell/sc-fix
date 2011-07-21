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
 *	Organizer.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements main menu screen of Oranizer applications.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**********************************************************************************
	Filename:		Organizer.c
	Author:			Gautam
	Date Created:
	Contains:		Organizer Main Screen
**********************************************************************************/
#ifndef _MMI_ORGANIZER_C
#define _MMI_ORGANIZER_C

/*  Include: MMI header file */
#define __NEWSIMULATOR
#include "stdc.h"
#include "mmi_features.h"
#include "l4dr.h"
#include "l4dr1.h"

#include "wgui_categories.h"
#include "custdatares.h"
#include "globaldefs.h"
#include "historygprot.h"
#include "unicodexdcl.h"
#include "mainmenudef.h"
#include "custmenures.h"
#include "organizerdef.h"
#include "calendarprot.h"		/* for calendar */
#include "calendardef.h"
#include "todolistdef.h"		/* for to do list */
#include "todolistenum.h"		
#include "alarmgprot.h"			/* for alarm */
#include "calculator.h"			/* for calculator */
#include "unitconvertor.h"		/* for converters */
#include "currencyconvertor.h"
#include "worldclock.h" 		/* for world clock */
#include "calorie.h"			/* for health */
#include "bmi.h"
#include "healthmenstural.h"
#include "settinggprots.h"		/* for aphorism */
#include "settingprot.h"
#include "simdetectiongexdcl.h"
#include "messagesexdcl.h"
#ifdef __MMI_MESSAGES_CLUB__
#include "satgprots.h"
#endif
#include "nvramenum.h"

#ifdef __MMI_NOTEPAD__
#include "notepaddef.h"
#endif
// Beginning--added by kecx for flashlight function on 20081110
#ifdef __MMI_FLASHLIGHT__
#ifdef MMI_ON_HARDWARE_P
#include "hal_tcu.h"
#include "hal_gpio.h"
#include "pmd_m.h"
#endif
#include "settingprofile.h"
#include "commonscreens.h"
#endif
// end--added by kecx for flashlight function on 20081110

#undef __NEWSIMULATOR
/* 
** Define
*/
#define APR_TEXT_CHARCTERS		3

/* 
** Typedef 
*/

/* 
** Local Variable
*/

/* 
** Local Function
*/
#if defined(__MMI_BMI__) || defined(__MMI_CALORIE__) || defined(__MMI_MENSTRUAL__)
	void EntryOrgHealthApp(void);
	void HighlightOrgHealthMenu(void);
#endif

/* 
** Global Variable
*/

// Beginning--added by kecx for flashlight function on 20081110
#ifndef __MMI_FLASHLIGHT__
	extern const U16 gIndexIconsImageList[];
#else
	UINT8 gFlashlightItemIdex = 1; //flash light is closed in default
#endif
// end--added by kecx for flashlight function on 20081110
// Beginning--added by kecx for flashlight shortcut key on 20081120
#ifdef __MOD_FLASHLIGHT__
	UINT8 iShortcutSwitch=0;
#endif
// End--added by kecx for flashlight shortcut key on 20081120

#if defined(__MMI_APHORISM__)
/* under construction !*/
#endif


/* 
** Global Function
*/


#if defined(__MMI_BMI__) || defined(__MMI_CALORIE__) || defined(__MMI_MENSTRUAL__)
/*****************************************************************************
* FUNCTION
*  EntryOrgHealthApp
* DESCRIPTION
*   Display list of health applications.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryOrgHealthApp(void) 
{
	U8* guiBuffer;
	U16 nStrItemList[HALETH_ENUM_TOTAL];
	U16 nNumofItem;

	EntryNewScreen(SCR_HEALTH_MENU, NULL, EntryOrgHealthApp, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_HEALTH_MENU);
	nNumofItem = GetNumOfChild(ORGANIZER_HEALTH_MENU);
	GetSequenceStringIds(ORGANIZER_HEALTH_MENU, nStrItemList);
	SetParentHandler(ORGANIZER_HEALTH_MENU);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	ShowCategory15Screen(STR_SCR_HEALTH_CAPTION, GetRootTitleIcon(ORGANIZER_HEALTH_MENU),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (PU16)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  HighlightOrgHealthMenu
* DESCRIPTION
*   Highlight handler of Health menu item. 
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightOrgHealthMenu(void) 
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryOrgHealthApp,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(EntryOrgHealthApp,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
#endif

// Beginning--added by kecx for flashlight function on 20081110
#if defined(__MMI_FLASHLIGHT__)
/*****************************************************************************
* FUNCTION
*        EntryOrgFlashlightApp
* DESCRIPTION
*        Display the status of flashlight.
* PARAMETERS
*        None.
* RETURNS
*        None.
* GLOBALS AFFECTED
*        None
*****************************************************************************/
void EntryOrgFlashlightApp(void) 
{
	U16 nStrItemList[15];         /* Stores the strings id of submenus returned */
	U16 nNumofItem;               /* Stores no of children in the submenu*/
	U16 nDispAttribute;            /* Stores display attribue */
	U8* guiBuffer;                     /* Buffer holding history data */
	U8 i;
	U8 data=0;
	U16 FlashlightMenuID[]={
							ORGANIZER_FLASHLIGHT_OPEN_MENUID,
							ORGANIZER_FLASHLIGHT_CLOSE_MENUID
						};

	EntryNewScreen(ORGANIZER_FLASHLIGHT_SCREENID, NULL, EntryOrgFlashlightApp, NULL);

	guiBuffer = GetCurrGuiBuffer(ORGANIZER_FLASHLIGHT_SCREENID);

	nNumofItem = GetNumOfChild(ORGANIZER_FLASHLIGHT_MENUID);
	nDispAttribute = GetDispAttributeOfItem(ORGANIZER_FLASHLIGHT_MENUID);
	GetSequenceStringIds(ORGANIZER_FLASHLIGHT_MENUID,nStrItemList);
	SetParentHandler(ORGANIZER_FLASHLIGHT_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	i = gFlashlightItemIdex;

	data = GetIndexOfStringId(ORGANIZER_FLASHLIGHT_MENUID,FlashlightMenuID[i]);
	ShowCategory11Screen(ORGANIZER_MENU_FLASHLIGHT_STRID, 0,
						 STR_GLOBAL_OK, 0,
						 STR_GLOBAL_BACK, 0,
						 nNumofItem, nStrItemList,(U16)data,guiBuffer);
}

/*****************************************************************************
* FUNCTION
*        HighlightOrgFlashlightMenu
* DESCRIPTION
*        Highlight handler of Flashlight menu item. 
*        Register key handlers.
* PARAMETERS
*        None.
* RETURNS
*        None.
* GLOBALS AFFECTED
*        None
*****************************************************************************/
void HighlightOrgFlashlightMenu(void) 
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetLeftSoftkeyFunction(EntryOrgFlashlightApp,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryOrgFlashlightApp,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*	ExitFlashLightScreen
* DESCRIPTION
*   	Exit flashlight screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitFlashLightScreen(void)
{
	history_t   Scr; 
	S16 nHistory = 0;
	U16 inputBufferSize;                
 	
	Scr.scrnID = ORGANIZER_FLASHLIGHT_MENUID;
	CloseCategory57Screen();
	Scr.entryFuncPtr = EntryOrgFlashlightApp;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data ((U8*) Scr.inputBuffer);		
	AddNHistory(Scr, inputBufferSize);				

}

/*****************************************************************************
* FUNCTION
*	FlashlightGoOrganizer
* DESCRIPTION
*   	Go back to organizer list.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void FlashlightGoOrganizer(void) 
{
	GoBackToHistory(ORGANIZER_SCREENID);
}

/**************************************************************
	FUNCTION NAME		: HighlightFlashlightOpen(void)
  	PURPOSE				: Highlight Flashlight Open Switch
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/	
void HighlightFlashlightOpen(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(FlashlightOpen,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/**************************************************************
	FUNCTION NAME		: HighlightFlashlightClose(void)
  	PURPOSE				: Highlight Flashlight Close Switch
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/	
void HighlightFlashlightClose(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(FlashlightClose,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/**************************************************************
	FUNCTION NAME		: SetFlashlightStatus(UINT8 iSwitch)
  	PURPOSE				: Set Flashlight status
	INPUT PARAMETERS	: UINT8 iSwitch
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/	
void  SetFlashlightStatus(UINT8 iSwitch)
{
	FlashLight(iSwitch);                   
}

/**************************************************************
	FUNCTION NAME		: FlashlightOpen(void)
  	PURPOSE				: Open Flashlight
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/	
void FlashlightOpen(void)
{
	gFlashlightItemIdex=0;
	SetFlashlightStatus(FL_OPEN); 
	iShortcutSwitch=FL_OPEN;                // added by kecx for flashlight Shortcut Key on 20081120
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( ORGANIZER_FLASHLIGHT_MENUID );
}

/**************************************************************
	FUNCTION NAME		: FlashlightClose(void)
  	PURPOSE				: Close Flashlight
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/	
void FlashlightClose(void)
{
	gFlashlightItemIdex=1;
	SetFlashlightStatus(FL_CLOSE);
	iShortcutSwitch=FL_CLOSE;               // added by kecx for flashlight Shortcut Key on 20081120
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( ORGANIZER_FLASHLIGHT_MENUID );
}
#endif
// End--added by kecx for flashlight function on 20081110
// Beginning--added by kecx for flashlight shortcut key on 20081120
#ifdef __MOD_FLASHLIGHT__
/**************************************************************
	FUNCTION NAME		: SwitchFlashlightStatus(void)
  	PURPOSE				: Switch Flashlight Status
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/	
void  SwitchFlashlightStatus(void)
{
	if(iShortcutSwitch)
	{
		iShortcutSwitch=FL_CLOSE;
	}
	else
	{
		iShortcutSwitch=FL_OPEN;
	}
	FlashLight(iShortcutSwitch);    
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( ORGANIZER_FLASHLIGHT_MENUID );
}

/*****************************************************************************
* FUNCTION
* 	Torch_light(UINT8 Torch_On)
* DESCRIPTION
*   	Switch flashlight.
* PARAMETERS
*	UINT8 Torch_On
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void Torch_light(UINT8 Torch_On)
{
#ifdef MMI_ON_HARDWARE_P
	if(Torch_On==1)
		pmd_SetLevel(PMD_LEVEL_LED3,1);
	else
		pmd_SetLevel(PMD_LEVEL_LED3, 0);
#endif
}
#endif
// End--added by kecx for flashlight shortcut key on 20081120


/*****************************************************************************
* FUNCTION
*  OrgInit
* DESCRIPTION
*   Initialize each applications in Organizer.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
void OrgInit(void) 
{
	/* calendar */
	#if defined(__MMI_CALENDAR__)
		ClndrInit();
	#endif

	/* to do list */

	/* alarm */
	AlmInit();

	/* calucartor */
	#if defined (__MMI_CALCULATOR__)
		CalcInit();
	#endif

	/* world clock */
	#if defined (__MMI_WORLD_CLOCK__)
		WcInit();
	#endif

	/* unit converter */
	#if defined (__MMI_UNIT_CONVERTER__)
		UconvInit();
	#endif

	/* currency converter */
	#if defined (__MMI_CURRENCY_CONVERTER__)
		CconvInit();
	#endif

	/* health */
	#if defined(__MMI_BMI__) || defined(__MMI_CALORIE__) || defined(__MMI_MENSTRUAL__)
		SetHiliteHandler(ORGANIZER_HEALTH_MENU,HighlightOrgHealthMenu);
	#endif

	/* bmi */
	#if defined(__MMI_BMI__)
		SetHiliteHandler(ORGANIZER_HEALTH_MENU_BMI,HighlightBMIMenu);
	#endif
	/* menstrual */
	#if defined(__MMI_MENSTRUAL__)
		MensInit();
	#endif

	/* calorie */
	#if defined(__MMI_CALORIE__)
/* under construction !*/
	#endif

	/* aphorism */
	#if defined(__MMI_APHORISM__)
/* under construction !*/
	#endif
}
//add by liuxn 060616 start
#else
void OrgInit(void) 
{

	/* calendar */
	#if defined(__MMI_CALENDAR__)
		ClndrInit();
	#endif

		AlmInit();
	
	/* calucartor */
	#if defined (__MMI_CALCULATOR__)
		CalcInit();
	#endif
	
	/* world clock */
	#if defined (__MMI_WORLD_CLOCK__)
		WcInit();
	#endif
	
	/* unit converter */
	#if defined (__MMI_UNIT_CONVERTER__)
		UconvInit();
	#endif

	/* currency converter */
	#if defined (__MMI_CURRENCY_CONVERTER__)
		CconvInit();
	#endif

	/* health */
	#if defined(__MMI_BMI__) || defined(__MMI_CALORIE__) || defined(__MMI_MENSTRUAL__)
		SetHiliteHandler(ORGANIZER_HEALTH_MENU,HighlightOrgHealthMenu);
	#endif

	/* bmi */
	#if defined(__MMI_BMI__)
		SetHiliteHandler(ORGANIZER_HEALTH_MENU_BMI,HighlightBMIMenu);
	#endif
	/* flashlight--added by kecx for flashlight function on 20081110 */
	#if defined(__MMI_FLASHLIGHT__)
	       	SetHiliteHandler(ORGANIZER_FLASHLIGHT_MENUID,HighlightOrgFlashlightMenu);
		SetHiliteHandler(ORGANIZER_FLASHLIGHT_OPEN_MENUID,HighlightFlashlightOpen);
		SetHiliteHandler(ORGANIZER_FLASHLIGHT_CLOSE_MENUID,HighlightFlashlightClose);
	#endif
	/* menstrual */
	#if defined(__MMI_MENSTRUAL__)
		MensInit();
	#endif

	//add by liuxn 060616 start
	#if defined(__MMI_TODOLIST__)
		InitToDoListApp();
	#endif
	//add by liuxn 060616 end

	#ifdef __MMI_NOTEPAD__
	InitNotepadtApp();
	#endif
}
#endif
//add by liuxn 060616 end


/*****************************************************************************
* FUNCTION
*  OrgDeInit
* DESCRIPTION
*   Deinitialize each application of Organizer
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/

void OrgDeInit(void)
{	
	#if defined(__MMI_CALENDAR__)
		ClndrDeInit();
	#endif

	#if defined (__MMI_CALCULATOR__)
		CalcDeInit();
	#endif

	#if defined (__MMI_CURRENCY_CONVERTER__)
		CconvDeInit();
	#endif
	
	#if defined (__MMI_UNIT_CONVERTER__)
		UconvDeInit();
	#endif
	
	#if defined(__MMI_BMI__)
		BmiDeInit();
	#endif
	
	#if defined(__MMI_MENSTRUAL__)
		MensDeInit();
	#endif

	#if defined(__MMI_CALORIE__)
/* under construction !*/
	#endif
	
}


/*****************************************************************************
* FUNCTION
*  EntryOrganizer
* DESCRIPTION
*   Entry Organizer menu screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryOrganizer(void) 
{
	U8 i=0;
	U8* guiBuffer;
	S32 num_of_items = 0;
	U16 item_list[MAX_SUB_MENUS];
	U8* item_string[MAX_SUB_MENUS];
	U16 item_icon [MAX_SUB_MENUS];
	U8 *pop_up_organizer[MAX_SUB_MENUS];

#ifdef __MMI_MESSAGES_CLUB__
	U32 menuItemId=-1;
	U32 maskingByte=-1;
#endif
	TBM_ENTRY(0x28D8);

	EntryNewScreen(ORGANIZER_SCREENID, NULL, EntryOrganizer, NULL);

	guiBuffer = GetCurrGuiBuffer(ORGANIZER_SCREENID);

	SetParentHandler(MAIN_MENU_ORGANIZER_MENUID);

	num_of_items = GetNumOfChild(MAIN_MENU_ORGANIZER_MENUID);

	GetSequenceStringIds(MAIN_MENU_ORGANIZER_MENUID, item_list);
	GetSequenceImageIds(MAIN_MENU_ORGANIZER_MENUID,item_icon);

	for(i=0; i<num_of_items; i++)
		item_string[i]=(U8*)GetString(item_list[i]);

	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	memset(pop_up_organizer,0,sizeof(pop_up_organizer));

#if defined(__MMI_APHORISM__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

#ifdef __MMI_MESSAGES_CLUB__
	menuItemId=GetChildMenuIDIndexByParentMenuID(MAIN_MENU_ORGANIZER_MENUID, MAIN_MENU_SERVICES_MENUID);
	if(menuItemId!=-1)
	{
#ifndef WAP_SUPPORT
		if((!g_pwr_context[0].IsSimValid)||(!IsSATPresent(0)))
			ResetBit(maskingByte,menuItemId);
#ifdef __MMI_MERGE_SAT_TO_MAINMENU__
		else
		{
			if ( IsSATPresent(0))
				item_string[menuItemId] = GetSATMainMenuAlphaID();
		}
#endif
#endif
	}
	MaskStringItems(pop_up_organizer,(U8)num_of_items,maskingByte);
	MaskItems(item_icon,(U8)num_of_items,maskingByte);
	MaskHiliteItems(MAIN_MENU_ORGANIZER_MENUID, maskingByte);
	num_of_items=MaskItems(item_list,(U8)num_of_items,maskingByte);
#endif

 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(MAIN_MENU_ORGANIZER_MENUID);
#endif
 
#ifdef __MMI_NUMBER_MENU__
	ShowCategory53Screen(ORGANIZER_TITLE_STRINGID,ORGANIZER_TITLE_IMAGEID,STR_GLOBAL_OK,
	IMG_GLOBAL_OK,STR_GLOBAL_BACK, IMG_GLOBAL_BACK,num_of_items,item_string,(PU16)gIndexIconsImageList,
	pop_up_organizer,0,/*currHiliteItemOrg*/0,guiBuffer);


#else
	ShowCategory53Screen(ORGANIZER_TITLE_STRINGID,ORGANIZER_TITLE_IMAGEID,STR_GLOBAL_OK,
	IMG_GLOBAL_OK,STR_GLOBAL_BACK, IMG_GLOBAL_BACK,num_of_items,item_string,item_icon,
	pop_up_organizer,0,/*currHiliteItemOrg*/0,guiBuffer);
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	TBM_EXIT(0x28D8);
}
#ifdef __MMI_MAINLCD_220X176__
/*****************************************************************************
* FUNCTION
*  EntryTools
* DESCRIPTION
*   Entry Tools menu screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryTools(void) 
{
	U8 i=0;
	U8* guiBuffer;
	S32 num_of_items = 0;
	U16 item_list[MAX_SUB_MENUS];
	U8* item_string[MAX_SUB_MENUS];
	U16 item_icon [MAX_SUB_MENUS];
	U8 *pop_up_organizer[MAX_SUB_MENUS];
#ifdef __MMI_MESSAGES_CLUB__
	U32 menuItemId=-1;
	U32 maskingByte=-1;
#endif
	TBM_ENTRY(0x28D8);
	EntryNewScreen(ORGANIZER_SCREENID, NULL, EntryTools, NULL);
	guiBuffer = GetCurrGuiBuffer(ORGANIZER_SCREENID);
	SetParentHandler(MAIN_MENU_ORGANIZER_MENUID);
	num_of_items = GetNumOfChild(MAIN_MENU_ORGANIZER_MENUID);
	GetSequenceStringIds(MAIN_MENU_ORGANIZER_MENUID, item_list);
	GetSequenceImageIds(MAIN_MENU_ORGANIZER_MENUID,item_icon);
	for(i=0; i<num_of_items; i++)
		item_string[i]=(U8*)GetString(item_list[i]);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	memset(pop_up_organizer,0,sizeof(pop_up_organizer));
#ifdef __MMI_MESSAGES_CLUB__
	menuItemId=GetChildMenuIDIndexByParentMenuID(MAIN_MENU_ORGANIZER_MENUID, MAIN_MENU_SERVICES_MENUID);
	if(menuItemId!=-1)
	{
#ifndef WAP_SUPPORT
		if((!g_pwr_context[0].IsSimValid)||(!IsSATPresent(0)))
			ResetBit(maskingByte,menuItemId);
#ifdef __MMI_MERGE_SAT_TO_MAINMENU__
		else
		{
			if ( IsSATPresent(0))
				item_string[menuItemId] = GetSATMainMenuAlphaID();
		}
#endif
#endif
	}
	MaskStringItems(pop_up_organizer,(U8)num_of_items,maskingByte);
	MaskItems(item_icon,(U8)num_of_items,maskingByte);
	MaskHiliteItems(MAIN_MENU_ORGANIZER_MENUID, maskingByte);
	num_of_items=MaskItems(item_list,(U8)num_of_items,maskingByte);
#endif
 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(MAIN_MENU_ORGANIZER_MENUID);
#endif
 
#ifdef __MMI_NUMBER_MENU__
	ShowCategory53Screen(ORGANIZER_TITLE_STRINGID,ORGANIZER_TITLE_IMAGEID,STR_GLOBAL_OK,
	IMG_GLOBAL_OK,STR_GLOBAL_BACK, IMG_GLOBAL_BACK,num_of_items,item_string,(PU16)gIndexIconsImageList,
	pop_up_organizer,0,/*currHiliteItemOrg*/0,guiBuffer);
#else
	ShowCategory53Screen(ORGANIZER_TITLE_STRINGID,ORGANIZER_TITLE_IMAGEID,STR_GLOBAL_OK,
	IMG_GLOBAL_OK,STR_GLOBAL_BACK, IMG_GLOBAL_BACK,num_of_items,item_string,item_icon,
	pop_up_organizer,0,/*currHiliteItemOrg*/0,guiBuffer);
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	TBM_EXIT(0x28D8);
}
#endif

#endif // #ifndef _MMI_ORGANIZER_C


