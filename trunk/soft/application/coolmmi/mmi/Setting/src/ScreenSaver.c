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
 * ScreenSaver.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Screen Saver application
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: ScreenSaver.c

  	PURPOSE		: Resource IDs for the FunAndGames Application

 

 

	DATE		: 

**************************************************************/
#ifndef _SCREENSAVER_C
#define _SCREENSAVER_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "unicodexdcl.h"
#include "idleappdef.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "downloaddefs.h"
#include "commonscreens.h"
#include "custdatares.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "custmenures.h"
#include "wgui_categories.h"
#include "wgui_categories_idlescreen.h"
#include "mainmenudef.h"
#include "timerevents.h"
#include "settingprofile.h"
#include "globaldefs.h"
#if defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__) 
	#if defined(__MMI_POWER_ON_OFF_DISPLAY__)
	/*MUST be allocated before WallpaperDefs.h*/
	#include "poweronoffdisplaydefs.h"
	#endif//(__MMI_POWER_ON_OFF_DISPLAY__)
#include "wgui_categories_multimedia.h"  /* showcategory331screen() */
#include "gdi_include.h"  /* gdi_result */
#include "vdoplystrenum.h"
#endif//(__MMI_SCREENSAVER_DISPLAY_VIDEO__) 
#include "screensaverdefs.h"
#include "wallpaperdefs.h"
#include "wpssprotos.h"
#include "wgui_inline_edit.h"
#include "downloadprotos.h"
#include "debuginitdef.h"
#include "historygprot.h"
#include "settingdefs.h"
#include "settingprot.h"
#include "phonesetup.h"
#ifdef __MMI_FILE_MANAGER__
#include "filemgr.h"
#include "filemanagerdef.h"
#endif
//#include "filemanagergprot.h"  // zrx del 20060612
#include "conversions.h"
#include "debuginitdef.h"
extern U8 PhnsetGetDefEncodingType(void);
#undef __NEWSIMULATOR
 
#ifdef __MMI_THEMES_V2_SUPPORT__
#include "mmithemes.h" //ritesh
#endif
 
/* 
** Define
*/

/* 
** Typedef 
*/

/* 
** Local Variable
*/
static PS8	error_str_ptr;


/* 
** Global Variable
*/
extern PHNSET_WPSS_FILENAME_STRUCT phnset_wpss_filename;
extern S8 *phnset_strp;  
extern U8 isCorrectSet;

#ifdef __MMI_SAVE_SS_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_SS_SPEEDUP__  20091125*/
const U16 PHNSET_PREDEFINED_SS_PATH_JPG[]= {0X63, 0X3a, 0X5c, 0X55, 0X73, 0X65, 0X72, 0X53, 0X53, 0X2e, 0X6a, 0X70, 0X67, 0X00, 0X00, 0X00, };// "c:\\UserSS.jpg\0";  
const U16 PHNSET_PREDEFINED_SS_PATH_PBM[]={0X63, 0X3a, 0X5c, 0X55, 0X73, 0X65, 0X72, 0X53, 0X53, 0X2e, 0X50, 0X42, 0X4d, 0X00, 0X00, 0X00, };// "c:\\UserSS.pbm\0"; 
S8 *idle_screen_screensaver_name_original = (S8 *)phnset_wpss_filename.screensave;
S8 *idle_screen_screensaver_name ;
#endif
/* 
** Global Function
*/

extern void PhnsetWpUsrDefPreView(void);
#ifdef __MMI_SAVE_SS_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_SS_SPEEDUP__  20091125*/
extern void SetIdleScreenSaveName(UI_string_type name);
extern void wgui_save_ScreenSave_to_speedup(void);
#endif
void PhnsetScrSvrInlineStatusSelectHiliteHdlr(S32 item_index);

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetWallpaper
* DESCRIPTION
*   This function is highlight handler for Phonesetup->IdleScreenDisplay->ScreenSaver
*   Following Functions:   EntryPhnsetScrSvr
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void HighlightPhnsetScreenSaver(void)
{
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_SS;
	
	ChangeLeftSoftkey(STR_GLOBAL_OK,0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(EntryPhnsetScrSvr, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryPhnsetScrSvr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef __MMI_WAP__
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else
/* under construction !*/
#endif
/* under construction !*/
#endif /* 0 */


/*****************************************************************************
* FUNCTION
*  HighlightPhnsetScrSvrSetting
* DESCRIPTION
*   This function is highlight handler for Phonesetup->IdleScreenDisplay->ScreenSaver -> Setting
*   Following Functions:   EntryPhnsetScrSvrSettings
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void HighlightPhnsetScrSvrSetting(void)
{
	SetLeftSoftkeyFunction(EntryPhnsetScrSvrSettings,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetScrSvrSettings,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*  HighlightPhnsetScrSvrSelect
* DESCRIPTION
*   This function is highlight handler for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select
*   Following Functions:   EntryPhnsetScrSvrSystem ( EntryScrSaverSystem //Wap Disable )
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void HighlightPhnsetScrSvrSelect(void)
{
#if (defined( __MMI_WAP__) ||(defined( __MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__) )  )

      /* Display 2 menuitems: System, User Defined */
	SetLeftSoftkeyFunction(EntryPhnsetScrSvrSelect,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetScrSvrSelect,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

#else

      /* Download and File Manager not exist, go to system screen saver list directly */
	SetLeftSoftkeyFunction(EntryPhnsetScrSvrSystem,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetScrSvrSystem,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ClearKeyHandler( KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

#endif
}


/*****************************************************************************
* FUNCTION
*  HighlightPhnsetScrSvrSystem 
* DESCRIPTION
*   This function is highlight handler for Phonesetup->IdleScreenDisplay->ScreenSaver -> (Select) -> System
*   Following Functions:   EntryPhnsetScrSvrSystem
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
#if (defined( __MMI_WAP__) ||defined( __MMI_FILE_MANAGER__)  )
void HighlightPhnsetScrSvrSystem(void)
{
	SetLeftSoftkeyFunction(EntryPhnsetScrSvrSystem,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetScrSvrSystem,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#endif


/*****************************************************************************
* FUNCTION
*  HighlightPhnsetScrSvrDwnl
* DESCRIPTION
*   This function is highlight handler for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> Download
*   Following Functions:   EntryPhnsetScrSvrDwnl
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
#if defined( __MMI_FILE_MANAGER__ )
void HighlightPhnsetScrSvrFmgr(void)
{
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_SS;

	SetLeftSoftkeyFunction(EntryPhnsetWallpaperFmgr /*EntryPhnsetScrSvrFmgr*/,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetWallpaperFmgr /*EntryPhnsetScrSvrFmgr*/,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#elif defined( __MMI_WAP__ )
void HighlightPhnsetScrSvrDwnl(void)
{
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_SS;

	SetLeftSoftkeyFunction(EntryPhnsetScrSvrDwnl,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetScrSvrDwnl,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#endif



/*****************************************************************************
* FUNCTION
*  HintPhnsetScrSvrSetting
* DESCRIPTION
*   This function is hint handler for Phonesetup->IdleScreenDisplay->ScreenSaver -> Setting
*   Following Functions:   
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void HintPhnsetScrSvrSetting(U16 index)
{
	switch (PhnsetGetCurScrSvrStatus())
	{
		case SET_SS_STATUS_ON:
			pfnUnicodeStrcpy((PS8)hintData[index],GetString(STR_GLOBAL_ON));
			break;

#ifdef __MMI_LCD_PARTIAL_ON__
		case SET_SS_STATUS_POWERSAVING:
			pfnUnicodeStrcpy((PS8)hintData[index],GetString(STR_SCREENSAVER_DISPLAY_TIME));
			break;
#endif /* __MMI_LCD_PARTIAL_ON__ */

		default:
			pfnUnicodeStrcpy((PS8)hintData[index],GetString(STR_GLOBAL_OFF));
			break;
	}
}



/*****************************************************************************
* FUNCTION
*  HintPhnsetScrSvrSelect 
* DESCRIPTION
*   This function is hint handler for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select
*   Following Functions:   
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void HintPhnsetScrSvrSelect(U16 index)
{
     PS8 p=PhnsetGetCurScrSvrName();

	if( p )
	   pfnUnicodeStrcpy((PS8)hintData[index], p );
}



/*****************************************************************************
* FUNCTION
*  EntryPhnsetScrSvr
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay->ScreenSaver
*   Following Functions:   
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void EntryPhnsetScrSvr(void)
{
	U8*	guiBuffer;
	U16 ScreenSaverScrText[MAX_SUB_MENUS];
	U8 *ScreenSaverHintsList[MAX_SUB_MENUS];
	U16 nNumofItem;
	
	EntryNewScreen(SCR_MAIN_SCREENSAVER, NULL, EntryPhnsetScrSvr, NULL);
	g_phnset_cntx_p->ScrAnchor = SCR_MAIN_SCREENSAVER;

   /***** Set Hint/Hightlight Handler *****/
	SetHintHandler(MENU_SETTING_SS_SETTING, HintPhnsetScrSvrSetting);
	SetHintHandler(MENU_SETTING_SS_SELECT,	HintPhnsetScrSvrSelect);
	SetHiliteHandler(MENU_SETTING_SS_SETTING, HighlightPhnsetScrSvrSetting);
	SetHiliteHandler(MENU_SETTING_SS_SELECT, HighlightPhnsetScrSvrSelect);

#if defined( __MMI_FILE_MANAGER__)
	SetHiliteHandler(MENU_SELECT_SS_SELECT_SYSTEM, HighlightPhnsetScrSvrSystem);
	SetHiliteHandler(MEUN_SELECT_SS_SELECT_DOWNLOAD, HighlightPhnsetScrSvrFmgr);
#elif defined( __MMI_WAP__)
	SetHiliteHandler(MENU_SELECT_SS_SELECT_SYSTEM, HighlightPhnsetScrSvrSystem);
	SetHiliteHandler(MEUN_SELECT_SS_SELECT_DOWNLOAD, HighlightPhnsetScrSvrDwnl);
#endif
      /***** End of Hint/Hightlight Handler *****/


	guiBuffer = GetCurrGuiBuffer(SCR_MAIN_SCREENSAVER);
	nNumofItem = GetNumOfChild(	MENU_SETTING_SCR_SVR);
	GetSequenceStringIds(MENU_SETTING_SCR_SVR,ScreenSaverScrText);

	SetParentHandler(MENU_SETTING_SCR_SVR);
	
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/*Set Hint Handler of Select to NULL if Screen Saver is Off*/
	if(PhnsetGetCurScrSvrStatus() == SET_SS_STATUS_ON)
		SetHintHandler(MENU_SETTING_SS_SELECT,HintPhnsetScrSvrSelect);
	else
		SetHintHandler(MENU_SETTING_SS_SELECT,NULL);
	
	ConstructHintsList(MENU_SETTING_SCR_SVR, ScreenSaverHintsList);

	ShowCategory52Screen(	STR_SCR_SCREENSAVER_CAPTION, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_OK, nNumofItem, ScreenSaverScrText,
							(PU16)gIndexIconsImageList, ScreenSaverHintsList, 1, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}




/* 
** Phonesetup->IdleScreenDisplay->ScreenSaver -> Setting
*/

/*****************************************************************************
* FUNCTION
*  PhnsetScrSvrInlineHiliteHdlr
* DESCRIPTION
*   This function is Inline selection highlight handler for Phonesetup->IdleScreenDisplay->ScreenSaver -> Setting
*   Following Functions:   PhnsetScrSvrSettingLskHdlr
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void PhnsetScrSvrInlineHiliteHdlr(S32 nIndex) {

	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(PhnsetScrSvrSettingLskHdlr);	
}

#ifdef __MMI_LCD_PARTIAL_ON__

/*****************************************************************************
* FUNCTION
*  PhnsetScrSvrInlineStatusSelectHiliteHdlr
* DESCRIPTION
*  Highlight handler of the inline select for screen saver status 
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void PhnsetScrSvrInlineStatusSelectHiliteHdlr(S32 item_index)
{
	if (item_index == 2 || item_index == 0) 
	{
		DisableInlineItem(&(wgui_inline_items[3]),3);
		Category57ChangeItemText(3, (U8 *) GetString(STR_GLOBAL_INVALID));
	} //modified by lin for screensaver @20070806
	else
	{
		EnableInlineItem(&(wgui_inline_items[3]),3);
		Category57ChangeItemText(3, (U8 *) subMenuDataPtrs[SET_SS_SETTING_INLINE_SELECT_5SEC + g_phnset_cntx_p->SSWaitStatusInlineHilite]);
	}
	RedrawCategory57Screen();
}


#endif /* __MMI_LCD_PARTIAL_ON__ */


/*****************************************************************************
* FUNCTION
*  PhnsetScrSvrFillInline
* DESCRIPTION
*   This function is to fill inline selection items for Phonesetup->IdleScreenDisplay->ScreenSaver -> Setting
*     
* PARAMETERS
*  a  IN/OUT      InlineItem *SSaverInline
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void PhnsetScrSvrFillInline(InlineItem *SSaverInline)
{
	g_phnset_cntx_p->SSStatusInlineHilite  = g_phnset_cntx_p->curSSStatus;
	SetInlineItemActivation( &SSaverInline[1],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemActivation( &SSaverInline[3],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	
	/*
	StatusSelection[0] = (PU8)GetString(STR_GLOBAL_OFF);
	StatusSelection[1] = (PU8)GetString(STR_GLOBAL_ON);

	WaitSelection[0] = (PU8)GetString(STR_SCREENSAVER_5SEC);
	WaitSelection[1] = (PU8)GetString(STR_SCREENSAVER_15SEC);
	WaitSelection[2] = (PU8)GetString(STR_SCREENSAVER_30SEC);
	WaitSelection[3] = (PU8)GetString(STR_SCREENSAVER_1MINUTE);

	SSaverSettingStatic[0] = (PU8)GetString(STR_SCREENSAVER_STATUS);
	SSaverSettingStatic[1] = (PU8)GetString(STR_SCREENSAVER_WAITING);

	SetInlineItemCaption( &SSaverInline[0], SSaverSettingStatic[0] );
	SetInlineItemSelect( &SSaverInline[1], 2, StatusSelection, &gScreenSaverStatusInline);
	SetInlineItemCaption( &SSaverInline[2], SSaverSettingStatic[1] );
	*/

	switch(g_phnset_cntx_p->curSSWaitTime)
	{
		case 5 :	g_phnset_cntx_p->SSWaitStatusInlineHilite = 0; break;
		case 15 :	g_phnset_cntx_p->SSWaitStatusInlineHilite = 1; break;
		case 30 :	g_phnset_cntx_p->SSWaitStatusInlineHilite = 2; break;
		case 60 :	g_phnset_cntx_p->SSWaitStatusInlineHilite = 3; break;
		default	:	g_phnset_cntx_p->SSWaitStatusInlineHilite = 2; break;		//Deafult Value
	}
	
	subMenuDataPtrs[SET_SS_SETTING_INLINE_CAPTION_STATUS] = (PU8)GetString(STR_SCREENSAVER_STATUS);
	subMenuDataPtrs[SET_SS_SETTING_INLINE_SELECT_OFF] = (PU8)GetString(STR_GLOBAL_OFF);
	subMenuDataPtrs[SET_SS_SETTING_INLINE_SELECT_ON] = (PU8)GetString(STR_GLOBAL_ON);
#ifdef __MMI_LCD_PARTIAL_ON__
	subMenuDataPtrs[SET_SS_SETTING_INLINE_SELECT_POWER_SAVING] = (PU8)GetString(STR_SCREENSAVER_DISPLAY_TIME);
#endif /* __MMI_LCD_PARTIAL_ON__ */
	subMenuDataPtrs[SET_SS_SETTING_INLINE_CAPTION_WAITING] = (PU8)GetString(STR_SCREENSAVER_WAITING);
	subMenuDataPtrs[SET_SS_SETTING_INLINE_SELECT_5SEC] = (PU8)GetString(STR_SCREENSAVER_5SEC);
	subMenuDataPtrs[SET_SS_SETTING_INLINE_SELECT_15SEC] = (PU8)GetString(STR_SCREENSAVER_15SEC);
	subMenuDataPtrs[SET_SS_SETTING_INLINE_SELECT_30SEC] = (PU8)GetString(STR_SCREENSAVER_30SEC);
	subMenuDataPtrs[SET_SS_SETTING_INLINE_SELECT_1MIN] = (PU8)GetString(STR_SCREENSAVER_1MINUTE);

	SetInlineItemCaption( &SSaverInline[0], subMenuDataPtrs[SET_SS_SETTING_INLINE_CAPTION_STATUS] );
#ifdef __MMI_LCD_PARTIAL_ON__
	SetInlineItemSelect( &SSaverInline[1], 3, &subMenuDataPtrs[SET_SS_SETTING_INLINE_SELECT_OFF], &(g_phnset_cntx_p->SSStatusInlineHilite) );
	RegisterInlineSelectHighlightHandler(&SSaverInline[1], PhnsetScrSvrInlineStatusSelectHiliteHdlr);
#else
	SetInlineItemSelect( &SSaverInline[1], 2, &subMenuDataPtrs[SET_SS_SETTING_INLINE_SELECT_OFF], &(g_phnset_cntx_p->SSStatusInlineHilite) );
#endif /* __MMI_LCD_PARTIAL_ON__ */
	SetInlineItemCaption( &SSaverInline[2], subMenuDataPtrs[SET_SS_SETTING_INLINE_CAPTION_WAITING] );
	SetInlineItemSelect( &SSaverInline[3], MAX_SS_INLINE_EDIT, &subMenuDataPtrs[SET_SS_SETTING_INLINE_SELECT_5SEC], &(g_phnset_cntx_p->SSWaitStatusInlineHilite));
} 


/*****************************************************************************
* FUNCTION
*  EntryPhnsetScrSvrSettings
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay->ScreenSaver -> Setting
*   Following Functions:   PhnsetScrSvrSettingLskHdlr
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void EntryPhnsetScrSvrSettings(void)
{
	U8*	guiBuffer;
	U16 ScreenSaverListIcon[MAX_SS_INLINE_EDIT] = { IMG_SETTING_GREET_TEXT, 0, IMAGE_SETTING_TIME, 0};
	InlineItem	SSSettingInLine[MAX_SS_INLINE_EDIT];
	S16 error;
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history

	EntryNewScreen(SCR_SCREENSAVER_SETTING, ExitPhnsetScrSvrSettings, NULL, NULL);
	isCorrectSet = 0;
	InitializeCategory57Screen();

	guiBuffer = GetCurrGuiBuffer(SCR_SCREENSAVER_SETTING);
	memset( (void *)SSSettingInLine, 0, sizeof( InlineItem )*MAX_SS_INLINE_EDIT );
	ReadValue( NVRAM_SCREENSAVER_STATUS, &(g_phnset_cntx_p->curSSStatus), DS_BYTE, &error);
	ReadValue( NVRAM_SCREENSAVER_WAITING, &(g_phnset_cntx_p->curSSWaitTime), DS_BYTE, &error);

#ifndef __MMI_LCD_PARTIAL_ON__
	/* If __MMI_LCD_PARTIAL_ON__ is turned off without reseting NVRAM */
	if (g_phnset_cntx_p->curSSStatus == (S32) SET_SS_STATUS_POWERSAVING)
	{
		g_phnset_cntx_p->curSSStatus = 0xFF;
	}
#endif /* ~ __MMI_LCD_PARTIAL_ON__ */

	if( g_phnset_cntx_p->curSSStatus == 0xFF )
	{
		g_phnset_cntx_p->curSSStatus = RstGetScrSaverStatusDefault();
		WriteValue(NVRAM_SCREENSAVER_STATUS, &(g_phnset_cntx_p->curSSStatus), DS_BYTE, &error);
	}

	if( g_phnset_cntx_p->curSSWaitTime == 0xFF )
	{
		g_phnset_cntx_p->curSSWaitTime = RstGetScrSaverWaitingDefault();
		WriteValue(NVRAM_SCREENSAVER_WAITING, &(g_phnset_cntx_p->curSSWaitTime), DS_BYTE, &error);
	}

	PhnsetScrSvrFillInline(SSSettingInLine);
	SetLeftSoftkeyFunction(PhnsetScrSvrSettingLskHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	RegisterHighlightHandler(PhnsetScrSvrInlineHiliteHdlr);
	
	inputBuffer = GetCurrNInputBuffer ( SCR_SCREENSAVER_SETTING, &inputBufferSize );    //added for inline edit history
	
	if(inputBuffer!=NULL)                                            //added for inline edit history
		SetCategory57Data(SSSettingInLine, MAX_SS_INLINE_EDIT, inputBuffer);//sets the data 
 
	ShowCategory57Screen(SCR_SCREENSAVER_SETTING_CAPTION,IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,STR_GLOBAL_BACK, IMG_GLOBAL_BACK, MAX_SS_INLINE_EDIT, ScreenSaverListIcon, SSSettingInLine, 0, guiBuffer);
	DisableCategory57ScreenDone();

	SetCategory57RightSoftkeyFunctions(PhnsetScrSvrSettingLskHdlr,GoBackHistory); 
}



/*****************************************************************************
* FUNCTION
*  ExitPhnsetScrSvrSettings
* DESCRIPTION
*   This function is Exit function for Phonesetup->IdleScreenDisplay->ScreenSaver -> Setting
*   Following Functions:   
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void ExitPhnsetScrSvrSettings(void)
{
		history_t h;
		S16 nHistory = 0;
		U16 inputBufferSize; 
		CloseCategory57Screen();        
		h.scrnID = SCR_SCREENSAVER_SETTING;
		h.entryFuncPtr = EntryPhnsetScrSvrSettings;
		pfnUnicodeStrcpy((S8*)h.inputBuffer, ( S8*)&nHistory);
		GetCategory57History(h.guiBuffer);
		inputBufferSize = (S16)GetCategory57DataSize();     //added for inline edit history
		GetCategory57Data((U8*) h.inputBuffer );         //added for inline edit history
		AddNHistory(h, inputBufferSize);                       //added for inline edit history

	if(isCorrectSet)
		DeleteUptoScrID(SCR_MAIN_SCREENSAVER);	

}



/*****************************************************************************
* FUNCTION
*  PhnsetScrSvrSettingLskHdlr
* DESCRIPTION
*   This function is left softkey handler for Phonesetup->IdleScreenDisplay->ScreenSaver -> Setting
*   Ancestor Functions:   EntryPhnsetScrSvrSettings
*                                  PhnsetScrSvrInlineHiliteHdlr
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void PhnsetScrSvrSettingLskHdlr(void)
{
	S16 error;
	g_phnset_cntx_p->curSSStatus= (U8)g_phnset_cntx_p->SSStatusInlineHilite;
	
	switch(g_phnset_cntx_p->SSWaitStatusInlineHilite)
	{
		case 0 :	g_phnset_cntx_p->curSSWaitTime = 5; break;
		case 1 :	g_phnset_cntx_p->curSSWaitTime = 15; break;
		case 2 :	g_phnset_cntx_p->curSSWaitTime = 30; break;
		case 3 :	g_phnset_cntx_p->curSSWaitTime = 60; break;
	}

	WriteValue(NVRAM_SCREENSAVER_STATUS, &(g_phnset_cntx_p->curSSStatus), DS_BYTE, &error);
	WriteValue(NVRAM_SCREENSAVER_WAITING, &(g_phnset_cntx_p->curSSWaitTime), DS_BYTE, &error);

	isCorrectSet = 1;
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
}



/* 
** Phonesetup->IdleScreenDisplay->ScreenSaver -> Select
*/
/*****************************************************************************
* FUNCTION
*  EntryPhnsetScrSvrSelect
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select
*   Ancestor Functions:   EntryPhnsetScrSvrSettings
*                                  PhnsetScrSvrInlineHiliteHdlr
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   gIndexIconsImageList
*****************************************************************************/
#if (defined( __MMI_WAP__) ||defined( __MMI_FILE_MANAGER__)  )
void EntryPhnsetScrSvrSelect(void)
{
	U8 *guiBuffer;
	U16 ScreenSaverSelectList[MAX_SUB_MENUS];
	U16 nNumofItem;

	EntryNewScreen(SCR_SCREENSAVER_SELECT, NULL, EntryPhnsetScrSvrSelect, NULL);
	
	g_phnset_cntx_p->ScrAnchor = SCR_SCREENSAVER_SELECT;
	
	guiBuffer = GetCurrGuiBuffer(SCR_SCREENSAVER_SELECT);

	nNumofItem = GetNumOfChild( MENU_SETTING_SS_SELECT);
	GetSequenceStringIds(MENU_SETTING_SS_SELECT,ScreenSaverSelectList);

	SetParentHandler(MENU_SETTING_SS_SELECT);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler); /* HighlightPhnsetScrSvrSystem, HighlightPhnsetScrSvrDwnl */

	ShowCategory15Screen(	STR_SCR_SCREENSAVER_CAPTION, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, nNumofItem,
							ScreenSaverSelectList, (PU16)gIndexIconsImageList,  1, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}


#endif /* defined( __MMI_WAP__) ||defined( __MMI_FILE_MANAGER__) */



/*****************************************************************************
* FUNCTION
*  PhnsetConstSysScrSvrList
* DESCRIPTION
*   This function is to construct system screen saver list  
*        for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> System
*     
* PARAMETERS
*  a  IN/OUT      U8 **pSysSSStrList, U16 *pSysSSImgIDList, S16 *HighliteItem 
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   gCurrentScreenSaver
*****************************************************************************/
U8 PhnsetConstSysScrSvrList(U8 **pSysSSStrList, U16 *pSysSSImgIDList, S16 *HighliteItem )
{
	S32 i=0;
	U16 strID=0;
//	DYNIMAGEINFO *Info= NULL;

#if defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__) 
	/******* Add a video *******/
	for(i=0;i<MAX_SCREENSAVER_VIDEO;i++)
	{
		if( pSysSSImgIDList ) 
	   	pSysSSImgIDList[i] = SCREENSAVER_VIDEO1+i;
		
		if( pSysSSStrList ) 
		   pSysSSStrList[i] = (PU8)GetString(STR_SCREENSAVER_VIDEO1);

		if( HighliteItem )
			if( g_phnset_cntx_p->curSsID == pSysSSImgIDList[i] )
				*HighliteItem = i;
	}
#endif//defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__) && !defined(WIN32)
	/******* For System Screen Saver *******/
	for( i = 0 ; i < MAX_SYSTEM_SCREENSAVER ; i++ )
	{
		strID = STR_SCREENSAVER_SS1 + i;

		if( pSysSSImgIDList )
		   pSysSSImgIDList[i+MAX_SCREENSAVER_VIDEO] = SCREENSAVER_1 + i;

		if( pSysSSStrList )
		   pSysSSStrList[i+MAX_SCREENSAVER_VIDEO] = (PU8)GetString(strID);

		if( HighliteItem )
               if( g_phnset_cntx_p->curSsID == pSysSSImgIDList[i+MAX_SCREENSAVER_VIDEO] )
			   *HighliteItem = i+MAX_SCREENSAVER_VIDEO;
	}

	
	/******* For Factory Screen Saver *******/
	/*g_phnset_cntx_p->SSFctyImgNum = GetFullScreenFactoryImagesInfo(&Info);

	for(  i = 0; i < g_phnset_cntx_p->SSFctyImgNum; i++ )
	{
		strID = STR_SCREENSAVER_SS1 + MAX_SYSTEM_SCREENSAVER + i;
		
		if(pSysSSImgIDList)
		   pSysSSImgIDList[MAX_SCREENSAVER_VIDEO + MAX_SYSTEM_SCREENSAVER + i] = Info[i].ImagesId;

		if(pSysSSStrList)
		pSysSSStrList[MAX_SCREENSAVER_VIDEO + MAX_SYSTEM_SCREENSAVER + i] = (PU8)GetString(strID);
		
		if(HighliteItem)
		   if( g_phnset_cntx_p->curSsID == Info[i].ImagesId )
			*HighliteItem = MAX_SCREENSAVER_VIDEO + MAX_SYSTEM_SCREENSAVER + i;
	}

	i=MAX_SCREENSAVER_VIDEO+ MAX_SYSTEM_SCREENSAVER +g_phnset_cntx_p->SSFctyImgNum -1;*/ //deleted by lin for scrsaver constructlist amount @20070605        
 
#ifndef __MMI_UI_TECHNO_IDLESCREEN_BAR__
 
	/******* For Analog Clock Screen Saver *******/
#ifdef __MMI_ANALOG_CLOCK__

	i++;	//index +1
	
	if(pSysSSImgIDList)
   	   pSysSSImgIDList[i] = ANALOG_CLOCK;

	if(pSysSSStrList)
   	   pSysSSStrList[i] = (PU8)GetString(STR_SCREENSAVER_ANALOG);

	if(HighliteItem)
	   if( g_phnset_cntx_p->curSsID == ANALOG_CLOCK )
			*HighliteItem = i;

#endif

#ifdef __MMI_DIGITAL_CLOCK__		
#if !defined(__MMI_MAINLCD_220X176__)
	i++; 	//index +1
	
   if( pSysSSImgIDList ) 
		pSysSSImgIDList[i] = DIGITAL_CLOCK;
     
	if( pSysSSStrList ) 
		pSysSSStrList[i] = (PU8)GetString(STR_SCREENSAVER_DIGITAL);

	if(HighliteItem)
	   if( g_phnset_cntx_p->curSsID == DIGITAL_CLOCK )
			*HighliteItem = i;
	
#endif
#endif
#ifdef __MMI_THEMES_V2_SUPPORT__
	   if(tm_get_current_theme_screensaver())
	   {
		i++;
	   if( pSysSSImgIDList ) 
   	  
		pSysSSImgIDList[i] = CURRENT_THEME_SCREENSAVER;
     
     if( pSysSSStrList ) 
   	
		pSysSSStrList[i] = (PU8)GetString(STR_SCREENSAVER_CURR_THEME);

	if(HighliteItem)
	   if( g_phnset_cntx_p->curSsID ==CURRENT_THEME_SCREENSAVER )
			
	   *HighliteItem = i;
	   }
#endif

 
#endif	/* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */
 

    return i;    //modified by lin for scrsaver constructlist amount @2070605

}

/*****************************************************************************
* FUNCTION
*  EntryPhnsetScrSvrSystem
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> System
*   Following Functions:   EntryPhnsetScrSvrSysView
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   gIndexIconsImageList
*****************************************************************************/
void EntryPhnsetScrSvrSystem(void)
{
	U8 *guiBuffer;
	U8 item_amount;
	
	EntryNewScreen(SCR_SCREENSAVER_SYSTEM_LIST, NULL, EntryPhnsetScrSvrSystem, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_SCREENSAVER_SYSTEM_LIST);
	
	RegisterHighlightHandler(PhnsetScrSvrSysHiliteHdlr);

	g_phnset_cntx_p->curHightlightItem=0;
	item_amount = PhnsetConstSysScrSvrList(subMenuDataPtrs, g_phnset_cntx_p->SysWPSSList, (S16 *)&g_phnset_cntx_p->curHightlightItem);
	
	/*Variable To Keep Is Dont Added On Scrolling*/
	g_phnset_cntx_p->WPSSPreviewScrFlag = 1;
	ShowCategory36Screen(STR_GLOBAL_SELECT,
								IMG_SCR_SETTING_CAPTION,
								STR_GLOBAL_VIEW, 
								0,
								STR_GLOBAL_BACK, 
								IMG_GLOBAL_BACK,
								item_amount, 
								subMenuDataPtrs, 
								(U16)(g_phnset_cntx_p->curHightlightItem), 
								guiBuffer);

	SetLeftSoftkeyFunction(EntryPhnsetScrSvrSysView,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetScrSvrSysView,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}



/*****************************************************************************
* FUNCTION
*  PhnsetScrSvrSysHiliteHdlr
* DESCRIPTION
*   This function is highlight handler for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> System
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   gHighlightSystemScreenSaver
*****************************************************************************/
void PhnsetScrSvrSysHiliteHdlr( S32 nIndex)
{
#if defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__)
	if(nIndex<MAX_SCREENSAVER_VIDEO)
	{
		ChangeLeftSoftkey(STR_ID_VDOPLY_PLAY, 0);
	}
	else
		ChangeLeftSoftkey(STR_GLOBAL_VIEW, 0);
#endif//defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__) && !defined(WIN32)
	g_phnset_cntx_p->curHightlightItem = (U16)nIndex;
}



/*****************************************************************************
* FUNCTION
*  EntryPhnsetScrSvrSysView 
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> System ->View
*   Following Functions:   EntryScrSetScreenSaver
*                                   PhnsetScrSvrSysViewPre
*                                   PhnsetScrSvrSysViewNext
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   gDontAddHistoryOnScroll
*   gSetSysWPSSList
*
*****************************************************************************/
void EntryPhnsetScrSvrSysView(void)
{
	PU8 ssName;
	U8 *guiBuffer;

	/* Notice: g_phnset_cntx_p->SysWPSSList Has been constructed at  EntryPhnsetScrSvrSystem() */
	U16 screenSaverId = g_phnset_cntx_p->SysWPSSList[g_phnset_cntx_p->curHightlightItem]; 

	guiBuffer = GetCurrGuiBuffer(SCR_SCREENSAVER_VIEW);

	isCorrectSet = 0;

	if(g_phnset_cntx_p->WPSSPreviewScrFlag)
	{
	   //ExecuteCurrExitHandler();
	   EntryNewScreen(SCR_SCREENSAVER_VIEW, ExitPhnsetScrSvrSysView, EntryPhnsetScrSvrSysView, NULL);
	}

#if defined(__MMI_DRAW_MANAGER__)
	wgui_set_touch_title_bar_buttons();//enable the display of title bar buttons
#endif

#if defined(__MMI_TOUCH_SCREEN__)
	register_touch_button_handlers(NULL, PhnsetScrSvrSysViewPre,
			NULL, PhnsetScrSvrSysViewNext);
#endif

	ssName = (PU8)PhnsetGetScrSvrNameById(screenSaverId);
#if defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__) 
	if(screenSaverId < SCREENSAVER_VIDEO_MAX&& screenSaverId > SCREENSAVER_VIDEO_START)
	{
	#if defined(__MMI_DRAW_MANAGER__)
		ShowCategory225Screen(
									STR_SCREENSAVER_VIDEO1,		/* title_str */
									0,								/* title_icon */
									STR_GLOBAL_OK,				/* lsk_str */
									IMG_GLOBAL_OK,				/* lsk_icon */
									STR_GLOBAL_BACK,			/* rsk_str */
									IMG_GLOBAL_BACK,			/* rsk_icon */
									screenSaverId,				/* video_id */
									NULL,							/* video_filename */
									1,								/* repeat_count */
									TRUE,							/* is_visual_update */
									FALSE,						/* is_play_audio */
									TRUE,							/* is_lcd_no_sleep */
									GDI_COLOR_BLACK,			/* bg_color */
									PhnsetWpSsPlayCallBack,
									guiBuffer);
	#else
		ShowCategory225Screen(
									STR_SCREENSAVER_VIDEO1,		/* title_str */
									IMG_SCR_SETTING_CAPTION,/* title_icon */
									STR_GLOBAL_OK,				/* lsk_str */
									IMG_GLOBAL_OK,				/* lsk_icon */
									STR_GLOBAL_BACK,			/* rsk_str */
									IMG_GLOBAL_BACK,			/* rsk_icon */
									screenSaverId,				/* video_id */
									NULL,							/* video_filename */
									1,								/* repeat_count */
									TRUE,							/* is_visual_update */
									FALSE,						/* is_play_audio */
									TRUE,							/* is_lcd_no_sleep */
									GDI_COLOR_BLACK,			/* bg_color */
									PhnsetWpSsPlayCallBack,
									guiBuffer);
	#endif
	}
	else
#endif//defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__) && !defined(WIN32)
#ifdef __MMI_ANALOG_CLOCK__
	/*Category 86 to View Analog Clock Screen Saver*/
	if((screenSaverId == ANALOG_CLOCK))
	{
		ShowCategory86Screen(STR_SCREENSAVER_ANALOG,0,STR_GLOBAL_OK, IMG_GLOBAL_OK,STR_GLOBAL_BACK,
								IMG_GLOBAL_BACK, ANALOG_CLOCK, NULL, guiBuffer,0); 
	}
	else
#endif
#ifdef __MMI_DIGITAL_CLOCK__		
	/*Category 128 to View Digital Clock Screen Saver*/
	if(screenSaverId == DIGITAL_CLOCK)
	{	
		 ShowCategory128Screen(STR_SCREENSAVER_DIGITAL,0,STR_GLOBAL_OK, IMG_GLOBAL_OK,STR_GLOBAL_BACK,
						IMG_GLOBAL_BACK,DIGITAL_CLOCK, 1,1,1, guiBuffer,0);

	}
	else
#endif
#ifdef __MMI_THEMES_V2_SUPPORT__		
	if(screenSaverId == CURRENT_THEME_SCREENSAVER)
	{	
	#if defined(__MMI_DRAW_MANAGER__)
		 ShowCategory129Screen(ssName,0,STR_GLOBAL_OK, IMG_GLOBAL_OK,STR_GLOBAL_BACK,
						IMG_GLOBAL_BACK,CURRENT_THEME_SCREENSAVER, guiBuffer);
	#else
		 ShowCategory129Screen(ssName,IMG_SCR_SETTING_CAPTION,STR_GLOBAL_OK, IMG_GLOBAL_OK,STR_GLOBAL_BACK,
						IMG_GLOBAL_BACK,CURRENT_THEME_SCREENSAVER, guiBuffer);
	#endif
	}
	else
#endif
	/*Category 129 to view other Screensavers*/
	{
	#if defined(__MMI_DRAW_MANAGER__)
		ShowCategory129Screen(ssName, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK,
				STR_GLOBAL_BACK, IMG_GLOBAL_BACK, screenSaverId, guiBuffer);
	#else
		ShowCategory129Screen(ssName, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
							screenSaverId, guiBuffer);
	#endif
	}
	
	SetLeftSoftkeyFunction(PhnsetScrSvrSysViewLskHdlr,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#ifdef __MMI_NOKIA_STYLE_N800__	
        ClearKeyHandler( KEY_LSK,  KEY_EVENT_DOWN);
        ClearKeyHandler( KEY_RSK,  KEY_EVENT_DOWN);

        SetKeyHandler(PhnsetScrSvrSysViewLskHdlr , KEY_LSK,  KEY_EVENT_UP);
        SetKeyHandler(GoBackHistory , KEY_RSK,  KEY_EVENT_UP);
#endif


 
/* changed the up-down arrow key handlers to left-right arrow key handlers
	for having the similar behaviour of keypad and touchscreen. */
//	SetKeyHandler(PhnsetScrSvrSysViewPre,		KEY_UP_ARROW,	KEY_EVENT_UP);
//	SetKeyHandler(PhnsetScrSvrSysViewNext,	KEY_DOWN_ARROW,	KEY_EVENT_UP);
//BUG IMAGE VIEW START 20050917
// change the key handler for KEY UP event to KEY DOWN event.
	SetKeyHandler(PhnsetScrSvrSysViewPre, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(PhnsetScrSvrSysViewNext, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
//BUG IMAGE VIEW END 20050917
 


	/*reserve for video volume
	SetKeyHandler(PhnsetScrSvrSysViewPre,		KEY_VOL_UP,		KEY_EVENT_DOWN);
	SetKeyHandler(PhnsetScrSvrSysViewPre,		KEY_VOL_UP,		KEY_LONG_PRESS);
	
	SetKeyHandler(PhnsetScrSvrSysViewNext,	KEY_VOL_DOWN,	KEY_EVENT_DOWN);
	SetKeyHandler(PhnsetScrSvrSysViewNext,	KEY_VOL_DOWN,	KEY_LONG_PRESS);
	*/
	
	//SetGenericExitHandler(SCR_SCREENSAVER_VIEW, ExitPhnsetScrSvrSysView, EntryPhnsetScrSvrSysView);
}


/*****************************************************************************
* FUNCTION
*  ExitPhnsetScrSvrSysView
* DESCRIPTION
*   This function is Exit function for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> System ->View
*   Ancestor Functions:   
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   gDontAddHistoryOnScroll
*
*****************************************************************************/
void ExitPhnsetScrSvrSysView(void)
{
	if(isCorrectSet)
		DeleteUptoScrID(g_phnset_cntx_p->ScrAnchor);

	g_phnset_cntx_p->WPSSPreviewScrFlag=1;
}


	
/*****************************************************************************
* FUNCTION
*  PhnsetScrSvrSysViewPre
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> System ->View
*   Ancestor Functions:   EntryPhnsetScrSvrSysView
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   gHighlightSystemScreenSaver
*   gDontAddHistoryOnScroll
*
*****************************************************************************/
void PhnsetScrSvrSysViewPre(void)
{
	ExitCategoryFunction();
	if(g_phnset_cntx_p->curHightlightItem <= 0 )
	{
#ifdef __MMI_THEMES_V2_SUPPORT__
	   if(tm_get_current_theme_screensaver())
	   {
			g_phnset_cntx_p->curHightlightItem = 
											g_phnset_cntx_p->SSFctyImgNum+
											MAX_SYSTEM_SCREENSAVER+
											MAX_OTHER_SCREENSAVER+
											MAX_SCREENSAVER_VIDEO+1;// +1 for Theme screen saver
	   }
	   else
#endif
	   {
			g_phnset_cntx_p->curHightlightItem = 
											g_phnset_cntx_p->SSFctyImgNum+
											MAX_SYSTEM_SCREENSAVER+
											MAX_OTHER_SCREENSAVER+
											MAX_SCREENSAVER_VIDEO;
	   }
	}
	
	g_phnset_cntx_p->curHightlightItem--;
	g_phnset_cntx_p->WPSSPreviewScrFlag = 0;
	EntryPhnsetScrSvrSysView();
}



/*****************************************************************************
* FUNCTION
*  PhnsetScrSvrSysViewNext
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> System ->View
*   Ancestor Functions:   EntryPhnsetScrSvrSysView
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   gHighlightSystemScreenSaver
*   gDontAddHistoryOnScroll
*
*****************************************************************************/
void PhnsetScrSvrSysViewNext(void)
{
	ExitCategoryFunction();
#ifdef __MMI_THEMES_V2_SUPPORT__
	   if(tm_get_current_theme_screensaver())
	   {
			if(g_phnset_cntx_p->curHightlightItem >= 
									(g_phnset_cntx_p->SSFctyImgNum+
									MAX_SYSTEM_SCREENSAVER+
									MAX_OTHER_SCREENSAVER 
									+MAX_SCREENSAVER_VIDEO+1)-1)
		
				g_phnset_cntx_p->curHightlightItem = -1;
	   }
	   else
#endif
	   {
			if(g_phnset_cntx_p->curHightlightItem >= 
								g_phnset_cntx_p->SSFctyImgNum+
								MAX_SYSTEM_SCREENSAVER+
								MAX_OTHER_SCREENSAVER 
								+MAX_SCREENSAVER_VIDEO-1)
			g_phnset_cntx_p->curHightlightItem = -1;
	   }

	
	g_phnset_cntx_p->curHightlightItem++;
	g_phnset_cntx_p->WPSSPreviewScrFlag = 0;
	EntryPhnsetScrSvrSysView();
}




/*****************************************************************************
* FUNCTION
*  PhnsetScrSvrSysViewLskHdlr
* DESCRIPTION
*   This function is Left softkey for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> System ->View
*   Ancestor Functions: 
* 
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   gDontAddHistoryOnScroll
*
*****************************************************************************/
void PhnsetScrSvrSysViewLskHdlr(void)
{
	S16 error;
	g_phnset_cntx_p->curSsID = g_phnset_cntx_p->SysWPSSList[g_phnset_cntx_p->curHightlightItem];
	WriteValue(NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
	isCorrectSet = 1;
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, 
	SUCCESS_TONE);
}

#ifdef __MMI_THEMES_V2_SUPPORT__
void ThemeManagerSetScreensaver(U16 nId)
{
	S16 error;

	g_phnset_cntx_p->curSsID = nId;
	WriteValue( NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
}
#endif

#if ( defined(__MMI_WAP__) )
/*****************************************************************************
* FUNCTION
*  PhnsetConstDwnlScrSvrList
* DESCRIPTION
*   This function is to build the download screen saver list
*   Follow functions: 
*
* PARAMETERS
*  a  IN/OUT      Names : SS name list pointer array
*                       DLSSImgID : SS image id listvoid
*                       HligtItem: SS highlight item
*  b  IN       
* RETURNS
*  DL screen saver numbers
* GLOBALS AFFECTED
*     gHighlightDownloadWallpaper
*     gCurrentWallpaper
*****************************************************************************/
U8 PhnsetConstDwnlScrSvrList(U8 **Names, U16 *DLSSImgID, S16 *HligtItem )
{
      DYNIMAGEINFO *Info=NULL;
	U8 numDLImgs=0;

	numDLImgs = GetDownloadImagesInfo(&Info);				

	/*If No Download Image Found, return 0 ( will trigger show Empty PopUp) */
	if( numDLImgs == 0 )
		return 0;
	else
	{
		U8 i;
		//U8 *Names[MAX_SUB_MENUS];
		//U8 *guiBuffer;
		for(i=0; i< numDLImgs; i++)
		{
			DLSSImgID[i]=Info[i].ImagesId;

                if( Names )
                  Names[i]=Info[i].ImageName;

                if( HligtItem )
			   if(g_phnset_cntx_p->curSsID==DLSSImgID[i])
				 *HligtItem = i;
		}
		return numDLImgs;
	}
}


/*****************************************************************************
* FUNCTION
*  EntryPhnsetScrSvrDwnl
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> Download
*   Follow functions: EntryPhnsetScrSvrDwnlView
*
* PARAMETERS
*  a  IN/OUT      Names : Wallpaper name list pointer array
*                       DLWallpaperImgID : Wallpaper image id listvoid
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*     gHighlightDownloadWallpaper
*     gCurrentWallpaper
*****************************************************************************/
void EntryPhnsetScrSvrDwnl(void)
{
	U8 *Names[MAX_SUB_MENUS];
      
	//ExecuteCurrExitHandler();

	g_phnset_cntx_p->curHightlightItem= 0;
   g_phnset_cntx_p->SSDwnlImgNum = PhnsetConstDwnlScrSvrList( Names, g_phnset_cntx_p->DwnlWPSSList, (S16 *)&(g_phnset_cntx_p->curHightlightItem));

	if( g_phnset_cntx_p->SSDwnlImgNum == 0 )
	{
		DisplayPopup((PU8)GetString(STR_SCREENSAVER_EMPTY), IMG_GLOBAL_EMPTY, 1, ST_NOTIFYDURATION, EMPTY_LIST_TONE);
	}
	else
	{
	      U8 *guiBuffer;
          
		//ExecuteCurrExitHandler();
		EntryNewScreen(SCR_SCREENSAVER_DOWNLOAD_LIST, NULL, EntryPhnsetScrSvrDwnl, NULL);

		g_phnset_cntx_p->WPSSPreviewScrFlag = 1;

		guiBuffer = GetCurrGuiBuffer(SCR_SCREENSAVER_DOWNLOAD_LIST);
		
		RegisterHighlightHandler(PhnsetDwnlScrSvrHiliteHdlr);

		ShowCategory36Screen(STR_GLOBAL_SELECT,IMG_SCR_SETTING_CAPTION,STR_GLOBAL_VIEW, 0,
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								g_phnset_cntx_p->SSDwnlImgNum, (U8**)Names, g_phnset_cntx_p->curHightlightItem, guiBuffer);

		SetLeftSoftkeyFunction(EntryPhnsetScrSvrDwnlView,	KEY_EVENT_UP);
		SetKeyHandler(EntryPhnsetScrSvrDwnlView,			KEY_RIGHT_ARROW,	KEY_EVENT_DOWN);

		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

		//SetGenericExitHandler(SCR_SCREENSAVER_DOWNLOAD_LIST, NULL, EntryPhnsetScrSvrDwnl);
	}

}


/*****************************************************************************
* FUNCTION
*  UpdateDeletedImageScreenSaver
* DESCRIPTION
*   This function is to set screen saver to default one if the download image is deleted
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void UpdateDeletedImageScreenSaver(U16 ImageId)
{
	S16 error;
	if( ImageId == g_phnset_cntx_p->curSsID )
	{
		g_phnset_cntx_p->curSsID = RstGetScrSaverImgIdDefault();
		WriteValue(NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
	}
}
//#elif defined(__MMI_FILE_MANAGER__)
#endif


/*****************************************************************************
* FUNCTION
*  PhnsetWallpaperBadFileCallBack
* DESCRIPTION
*   This function is call back function for showcategories33screen, 
*   If display user defined image got problem in idle screen, this function will be executed
*   Follow functions: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*****************************************************************************/
void DispIdleScrSvrBadFilePopup(void)
{
	if(error_str_ptr)
		   DisplayPopup((PU8)error_str_ptr, \
                                                   IMG_GLOBAL_UNFINISHED, 1, \
                                                   UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	else
	   DisplayPopup((PU8)GetString(STR_IDLE_SS_DECODE_ERR), \
                                                   IMG_GLOBAL_UNFINISHED, 1, \
                                                   UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	error_str_ptr=NULL;

}


S32 PhnsetScreenSaverBadFileCallBack(void)
{
	PhnsetRstScrSvrDefault();
	
	error_str_ptr = NULL;
   /* Set timer and Display popup for user */
   DispIdleScrSvrBadFilePopup();
   //DispIdleScrSvrBadFilePopup();
   
   return g_phnset_cntx_p->curSsID;
}


#ifdef __MMI_FILE_MANAGER__
void PhnsetScrSvrForwardFromFmgr(void *path, int is_short)
{
   if(path)
   {

   		/* Prevent memory leck, after buffer is free, set pointer to NULL */
		ASSERT( phnset_strp == NULL );

		g_phnset_cntx_p->isShort = is_short;
		
		phnset_strp = OslMalloc( FMGR_PATH_BUFFER_SIZE );
		if( phnset_strp )
			pfnUnicodeStrcpy((S8 *)phnset_strp, (S8*)path);
		else
		{
			GoBackToHistory(SCR_IDLE_SCR_DISP);
			return;
		}

		g_phnset_cntx_p->wpss_type=MMI_PHNSET_TYPE_SS;

		g_phnset_cntx_p->ScrAnchor = GetCurrScrnId();

		PhnsetWpUsrDefPreView();
   }
   else
   /* run to here means some kinds of image error occur */
	 DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
}

extern UI_image_ID_type  idle_screen_screensaver_ID;
void PhnsetSetScrSvrByFileName( S8 *path )
{
  S16 error;
#ifdef __MMI_SAVE_SS_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_SS_SPEEDUP__  20091125*/
  U16 type;
#endif

  if(g_phnset_cntx_p->isShort)
    g_phnset_cntx_p->curSsID = 1;
  else
    g_phnset_cntx_p->curSsID = 0;

  pfnUnicodeStrcpy((S8 *)phnset_wpss_filename.screensave, (S8*)path);

  mmi_trace(g_sw_WpSsPwon, "gdm:PhnsetSetScrSvrByFileName, g_phnset_cntx_p->curSsID=%d", g_phnset_cntx_p->curSsID);
  WriteValue(NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
  
#ifdef __MMI_SAVE_SS_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_SS_SPEEDUP__  20091125*/
	/*Guojian Modify Start For Video ScreenSaver #9300 Ver: GreenStone  on 2008-7-31 16:7 */
    if (idle_screen_screensaver_ID)
    {
        type = gdi_image_get_type_from_id(idle_screen_screensaver_ID);
    }
    else
    {
        type = gdi_image_get_type_from_file(idle_screen_screensaver_name);
    }
		
	if ( type == GDI_IMAGE_TYPE_BMP_FILE || type == GDI_IMAGE_TYPE_JPG_FILE)
	{
		//add gdm 080616
	   SetIdleScreenSaveName((UI_string_type)phnset_wpss_filename.screensave);
	   wgui_save_ScreenSave_to_speedup();
	   mmi_trace(g_sw_WpSsPwon, "gdm:exit PhnsetSetScrSvrByFileName");
	   memset((S8*)phnset_wpss_filename.screensavelayer, 0, (PHNSET_MAX_FILE_LEN+PHNSET_MAX_PATH_LEN+PHNSET_MAX_EXT_LEN) );
	   pfnUnicodeStrcpy((S8*)phnset_wpss_filename.screensavelayer,(S8*) idle_screen_screensaver_name);

	}
#endif

	/*Guojian Modify End  For Video ScreenSaver #9300 Ver: GreenStone  on 2008-7-31 16:7 */
  WriteRecord( NVRAM_EF_WPSS_FILENAME_LID, 1, &phnset_wpss_filename, sizeof(phnset_wpss_filename), &error );

  isCorrectSet = 1;

  if( g_idle_context.IsOnIdleScreen )
   	EntryIdleScreen();
}

#endif

PS8 PhnsetGetCurScrSvrFileName(void)
{
#ifdef __MMI_SAVE_SS_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_SS_SPEEDUP__  20091125*/
   if ( pfnUnicodeStrcmp((PS8)phnset_wpss_filename.screensavelayer, (PS8)PHNSET_PREDEFINED_SS_PATH_PBM) == 0 )
     return (PS8)phnset_wpss_filename.screensavelayer;
#endif
   return (PS8)phnset_wpss_filename.screensave;
}



#if ( defined(__MMI_WAP__) || defined(__MMI_FILE_MANAGER__) )
/*****************************************************************************
* FUNCTION
*  PhnsetDwnlScrSvrHiliteHdlr
* DESCRIPTION
*   This function is highlight handler for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> Download
*   Follow functions: 
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetDwnlScrSvrHiliteHdlr( S32 nIndex)
{
	g_phnset_cntx_p->curHightlightItem = (U32)nIndex;
}



/*****************************************************************************
* FUNCTION
*  EntryPhnsetScrSvrDwnlView
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> Download -> View
*   Following functions:   PhnsetScrSvrDwnlViewLskHdlr
*                                  PhnsetScrSvrDwnlViewPre
*                                  PhnsetScrSvrDwnlViewNext
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*  
*****************************************************************************/
void EntryPhnsetScrSvrDwnlView(void)
{
	PU8 ssName;
	U8 *guiBuffer;
	U16 screenSaverId = g_phnset_cntx_p->DwnlWPSSList[g_phnset_cntx_p->curHightlightItem];

	guiBuffer = GetCurrGuiBuffer(SCR_SCREENSAVER_DOWNLOAD_VIEW);

	if(g_phnset_cntx_p->WPSSPreviewScrFlag)
	{
	   //ExecuteCurrExitHandler();
	   EntryNewScreen(SCR_SCREENSAVER_DOWNLOAD_VIEW, ExitPhnsetScrSvrDwnlView, EntryPhnsetScrSvrDwnlView, NULL);
	}
	
	ssName = (PU8)PhnsetGetScrSvrNameById(screenSaverId);

 
#if defined(__MMI_TOUCH_SCREEN__)
//BUG IMAGE VIEW START 20050917
//register the handlers for pen down event.
	register_touch_button_handlers(NULL, PhnsetScrSvrDwnlViewPre,
			NULL, PhnsetScrSvrDwnlViewNext);
//BUG IMAGE VIEW END 20050917
#endif

#if defined(__MMI_DRAW_MANAGER__)
	wgui_set_touch_title_bar_buttons();//enable the display of title bar buttons

	ShowCategory129Screen(ssName, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK,
			STR_GLOBAL_BACK, IMG_GLOBAL_BACK, screenSaverId, guiBuffer);
#else
	ShowCategory129Screen(ssName, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
							screenSaverId, guiBuffer);
#endif
 

	SetLeftSoftkeyFunction(PhnsetScrSvrDwnlViewLskHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#ifdef __MMI_NOKIA_STYLE_N800__
        ClearKeyHandler( KEY_LSK,  KEY_EVENT_DOWN);
        ClearKeyHandler( KEY_RSK,  KEY_EVENT_DOWN);

        SetKeyHandler(PhnsetScrSvrDwnlViewLskHdlr , KEY_LSK,  KEY_EVENT_UP);
        SetKeyHandler(GoBackHistory , KEY_RSK,  KEY_EVENT_UP);
#endif

 
/* changed the up-down arrow key handlers to left-right arrow key handlers
	for having the similar behaviour of keypad and touchscreen. */
//	SetKeyHandler(PhnsetScrSvrDwnlViewPre,		KEY_UP_ARROW,	KEY_EVENT_UP);
//BUG IMAGE VIEW START 20050917
// change the key handler for KEY UP event to KEY DOWN event.
	SetKeyHandler(PhnsetScrSvrDwnlViewPre, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
//BUG IMAGE VIEW END 20050917
 
	SetKeyHandler(PhnsetScrSvrDwnlViewPre,		KEY_VOL_UP,		KEY_EVENT_DOWN);
	SetKeyHandler(PhnsetScrSvrDwnlViewPre,		KEY_VOL_UP,		KEY_LONG_PRESS);

 
//	SetKeyHandler(PhnsetScrSvrDwnlViewNext,		KEY_DOWN_ARROW,	KEY_EVENT_UP);
//BUG IMAGE VIEW START 20050917
// change the key handler for KEY UP event to KEY DOWN event.
	SetKeyHandler(PhnsetScrSvrDwnlViewNext, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
//BUG IMAGE VIEW END 20050917
 
	SetKeyHandler(PhnsetScrSvrDwnlViewNext,		KEY_VOL_DOWN,	KEY_EVENT_DOWN);
	SetKeyHandler(PhnsetScrSvrDwnlViewNext,		KEY_VOL_DOWN,	KEY_LONG_PRESS);
	
	//SetGenericExitHandler(SCR_SCREENSAVER_DOWNLOAD_VIEW, ExitPhnsetScrSvrDwnlView, EntryPhnsetScrSvrDwnlView);
}


/*****************************************************************************
* FUNCTION
*  ExitPhnsetScrSvrDwnlView
* DESCRIPTION
*   This function is Exit function for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> Download -> View
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  gDontAddHistoryOnScroll
*****************************************************************************/
void ExitPhnsetScrSvrDwnlView(void)
{
	g_phnset_cntx_p->WPSSPreviewScrFlag=1;
}



/*****************************************************************************
* FUNCTION
*  PhnsetScrSvrDwnlViewPre
* DESCRIPTION
*   This function is to view preview screen saver
*     for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> Download -> View
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*  
*****************************************************************************/
void PhnsetScrSvrDwnlViewPre(void)
{
	ExitCategoryFunction();	
	if(g_phnset_cntx_p->curHightlightItem == 0 )
		g_phnset_cntx_p->curHightlightItem = g_phnset_cntx_p->SSDwnlImgNum;
	
	g_phnset_cntx_p->curHightlightItem--;
	g_phnset_cntx_p->WPSSPreviewScrFlag = 0;
	EntryPhnsetScrSvrDwnlView();
}



/*****************************************************************************
* FUNCTION
*  PhnsetScrSvrDwnlViewNext 
* DESCRIPTION
*   This function is to view next screen saver
*     for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> Download -> View
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*  
*****************************************************************************/
void PhnsetScrSvrDwnlViewNext(void)
{
	if(g_phnset_cntx_p->curHightlightItem >= g_phnset_cntx_p->SSDwnlImgNum-1 )
		g_phnset_cntx_p->curHightlightItem = -1;
	
	g_phnset_cntx_p->curHightlightItem++;
	g_phnset_cntx_p->WPSSPreviewScrFlag = 0;
	EntryPhnsetScrSvrDwnlView();
}


/*****************************************************************************
* FUNCTION
*  PhnsetScrSvrDwnlViewLskHdlr 
* DESCRIPTION
*   This function is Left softkey handler 
*        for Phonesetup->IdleScreenDisplay->ScreenSaver -> Select -> Download -> View
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetScrSvrDwnlViewLskHdlrDone(void)
{
	GoBackToHistory(SCR_MAIN_SCREENSAVER);
}
void PhnsetScrSvrDwnlViewLskHdlr(void)
{
	S16 error;
	if(MMI_FS_GetFileSizeByName((U8*)phnset_wpss_filename.screensave) > GLOBAL_MMI_IMAGE_LIMIT_SIZE) //chenhe add for bug 9675. as we decode speed is solid , we avoid of large file
	{
		DisplayPopup((PU8)GetString(STR_ID_FMGR_FILE_LIMIT_FILE_TOO_LARGE), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
		fmgr_reset_app_select();/*not forword from fmgr*/
	}
	else
	{
		DisplayPopupCallBack((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, 
		SUCCESS_TONE,PhnsetScrSvrDwnlViewLskHdlrDone);
		g_phnset_cntx_p->curSsID = g_phnset_cntx_p->DwnlWPSSList[g_phnset_cntx_p->curHightlightItem];
		WriteValue(NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
	}
}



#endif /* __MMI_WAP__ */





/*
** Externel Interfaces
*/ 

/*****************************************************************************
* FUNCTION
*  PhnsetRstScrSvrDefault
* DESCRIPTION
*   This function is to set screen saver status/waiting time/image to default value
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
BOOL PhnsetRstScrSvrDefault(void)
{
	S16 error;
	
	g_phnset_cntx_p->curSsID = RstGetScrSaverImgIdDefault();
#if defined(__PROJECT_GALLITE_C01__) 
	g_phnset_cntx_p->curSSStatus = RstGetScrSaverStatusDefault();
	g_phnset_cntx_p->curSSWaitTime = RstGetScrSaverWaitingDefault();

	WriteValue( NVRAM_SCREENSAVER_STATUS, &(g_phnset_cntx_p->curSSStatus), DS_BYTE, &error);
	WriteValue( NVRAM_SCREENSAVER_WAITING, &(g_phnset_cntx_p->curSSWaitTime), DS_BYTE, &error);
#endif
	WriteValue( NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);

	if(error == NVRAM_WRITE_SUCCESS)
	return TRUE;
	else
	return FALSE;
}
//licheng add for usb
void PhnsetRstScrSvrDefaultTmp(void)
{
	S16 error;
	g_phnset_cntx_p->curSsID = RstGetScrSaverImgIdDefault();
	WriteValue(NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
}

void PhnsetScrSvrRestore(U16 SsID,BOOL IsRestore)
{	
	S16 error;
	S8 tempname[256];
	mmi_trace(g_sw_SETTING, "LXL-- %s entry %s function, File: %s, Line: %d,IsRestore=%d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__,IsRestore);	
	if(IsRestore)
	{
		ReadRecord( NVRAM_EF_WPSS_FILENAME_LID, 1, &phnset_wpss_filename, sizeof(phnset_wpss_filename), &error );
		mmi_trace(g_sw_SETTING, "LXL-- %s entry %s function, File: %s, Line: %d,error=%d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__,error);			
		UnicodeToAnsii(tempname, (S8 *)phnset_wpss_filename.screensave);
		mmi_trace(g_sw_SETTING, "LXL-- %s entry %s function, File: %s, Line: %d,name=%s", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__,tempname);			
		if( error == NVRAM_READ_SUCCESS )
		{
			PhnsetSetScrSvrByFileName( (S8 *)phnset_wpss_filename.screensave );
		}
		else
		{
			memset( &phnset_wpss_filename, 0, sizeof(phnset_wpss_filename) );
			g_phnset_cntx_p->curSsID = RstGetScrSaverImgIdDefault();
			WriteValue(NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
		}
	}
	else
	{
		g_phnset_cntx_p->curSsID = SsID;
		WriteValue(NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
	}
}
/*****************************************************************************
* FUNCTION
*  PhnsetGetCurScrSvr
* DESCRIPTION
*   This function is to get screen saver image ID to default value
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetGetCurScrSvr(U16 *screenSaver)
{
	S16 error;
	ReadValue(NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
	*screenSaver = g_phnset_cntx_p->curSsID;
}



/*****************************************************************************
* FUNCTION
*  PhnsetGetScrSvrSetings
* DESCRIPTION
*   This function is to get screen saver setting status
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetGetScrSvrSetings(U8* ScreenSaverStatus, U8* ScreenSaverWait)
{
	*ScreenSaverStatus = (U8)g_phnset_cntx_p->curSSStatus;
	*ScreenSaverWait = (U8)g_phnset_cntx_p->curSSWaitTime;
}


/*****************************************************************************
* FUNCTION
*  PhnsetReadNvramScrSvrSettings
* DESCRIPTION
*   This function is to read screen saver setting status/waiting time/image from nvram
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetReadNvramScrSvrSettings(void)
{
	S16 error;
	ReadValue( NVRAM_SCREENSAVER_STATUS, &(g_phnset_cntx_p->curSSStatus), DS_BYTE, &error);
	ReadValue( NVRAM_SCREENSAVER_WAITING, &(g_phnset_cntx_p->curSSWaitTime), DS_BYTE, &error);
	ReadValue( NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);

#ifndef __MMI_LCD_PARTIAL_ON__
	/* If __MMI_LCD_PARTIAL_ON__ is turned off without reseting NVRAM */
	if (g_phnset_cntx_p->curSSStatus == (S32) SET_SS_STATUS_POWERSAVING)
	{
		g_phnset_cntx_p->curSSStatus = 0xFF;
	}
#endif /* ~ __MMI_LCD_PARTIAL_ON__ */

	if( g_phnset_cntx_p->curSSStatus == 0xFF )
	{
		g_phnset_cntx_p->curSSStatus = RstGetScrSaverStatusDefault();
		WriteValue(NVRAM_SCREENSAVER_STATUS, &(g_phnset_cntx_p->curSSStatus), DS_BYTE, &error);
	}

	if( g_phnset_cntx_p->curSSWaitTime == 0xFF )
	{
		g_phnset_cntx_p->curSSWaitTime = RstGetScrSaverWaitingDefault();
		WriteValue(NVRAM_SCREENSAVER_WAITING, &g_phnset_cntx_p->curSSWaitTime, DS_BYTE, &error);
	}
	
	if( g_phnset_cntx_p->curSsID == 0xFFFF )
	{
		g_phnset_cntx_p->curSsID = RstGetScrSaverImgIdDefault();
		WriteValue(NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
	}
	else if(g_phnset_cntx_p->curSsID == 0 
			|| g_phnset_cntx_p->curSsID == 1)//should hint short file name
	{
	    ReadRecord( NVRAM_EF_WPSS_FILENAME_LID, 1, &phnset_wpss_filename, sizeof(phnset_wpss_filename), &error );
	    if( error != NVRAM_READ_SUCCESS )
	    {
 	       memset( &phnset_wpss_filename, 0, sizeof(phnset_wpss_filename) );
 		  g_phnset_cntx_p->curSsID = RstGetScrSaverImgIdDefault();
		  WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
	    }
	}
}



/*****************************************************************************
* FUNCTION
*  PhnsetGetCurScrSvrName
* DESCRIPTION
*   This function is to get screen saver name string ptr
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
PS8 PhnsetGetCurScrSvrName(void)

{
#ifdef __MMI_FILE_MANAGER__	
	if( g_phnset_cntx_p->curSsID == 0 )
	   return PhnsetGetFileNamePointer( (S8 *)phnset_wpss_filename.screensave);
	else if(g_phnset_cntx_p->curSsID==1)
	{
		U8 temp[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];
		PS8 p;
		
		/* rename short name file should pass converted short name to editor,
			so that the display will be the same as in File Manager */
		p = PhnsetGetFileNamePointer( (S8 *)phnset_wpss_filename.screensave);

		mmi_chset_mixed_text_to_ucs2_str((U8* )temp, 
													(FMGR_MAX_FILE_LEN+1)* ENCODING_LENGTH, 
													(U8*)p, 
													(mmi_chset_enum)PhnsetGetDefEncodingType() );
		p = (PS8)&temp[0];
		return p;

	}
	else
#endif	
		return PhnsetGetScrSvrNameById(g_phnset_cntx_p->curSsID);
}



/*****************************************************************************
* FUNCTION
*  PhnsetGetScrSvrNameById
* DESCRIPTION
*   This function is to get screen saver name string ptr by image ID
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
PS8 PhnsetGetScrSvrNameById(U16 Id)
{
	static S8 ssName[MAX_IMAGE_NAME_WIDTH];
	//This Is System Screen Saver
	U32 tempVariable;
#if defined( __MMI_SCREENSAVER_DISPLAY_VIDEO__ )
	if(Id < SCREENSAVER_VIDEO_MAX	&&Id> SCREENSAVER_VIDEO_START)
		pfnUnicodeStrcpy((PS8)ssName, GetString((U16)STR_SCREENSAVER_VIDEO1));
	else
#endif// defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ ) && !defined(WIN32)
#if 1 //  modified by lin for saver screen @20070620
	if( Id > SCREENSAVER_BASE && Id <= SCREENSAVER_BASE + MAX_SYSTEM_SCREENSAVER )
	{
 		tempVariable= STR_SCREENSAVER_SS1 + Id - (SCREENSAVER_BASE) - 1;
		pfnUnicodeStrcpy((PS8)ssName, GetString((U16)tempVariable));
	}
	else if((tempVariable = getFactoryImageRelativeIndex(Id)) != 0 )
	{
/*		tempVariable = STR_SCREENSAVER_SS1 +  MAX_SYSTEM_SCREENSAVER + tempVariable - 1;
		pfnUnicodeStrcpy((PS8)ssName, GetString((U16)tempVariable));
*/
		DYNIMAGEINFO *Info;
		U8 i;
		U8 noFactImages=GetFullScreenFactoryImagesInfo(&Info);
		
		for(  i=0; i<noFactImages; i++ )
		{
			if( Id == Info[i].ImagesId )
			{
				tempVariable = STR_SCREENSAVER_SS1 + MAX_SYSTEM_SCREENSAVER + i;
				break;
			}
		}
		pfnUnicodeStrcpy((PS8)ssName, GetString((U16)tempVariable));
	}
	#endif
#ifdef __MMI_ANALOG_CLOCK__
	else if( Id == ANALOG_CLOCK )
	{
		tempVariable = STR_SCREENSAVER_ANALOG;
		pfnUnicodeStrcpy((PS8)ssName, GetString((U16)tempVariable));
	}
#endif

#ifdef __MMI_DIGITAL_CLOCK__		
	else if( Id == DIGITAL_CLOCK )
	{
		tempVariable = STR_SCREENSAVER_DIGITAL;
		pfnUnicodeStrcpy((PS8)ssName, GetString((U16)tempVariable));
	}
#endif
	
#ifdef __MMI_THEMES_V2_SUPPORT__
	else if( Id == CURRENT_THEME_SCREENSAVER )
	{
		tempVariable = STR_SCREENSAVER_CURR_THEME;
		pfnUnicodeStrcpy((PS8)ssName, GetString((U16)tempVariable));
	}
#endif
	
#ifdef __MMI_WAP__
	else
	{
		memset(ssName,0,MAX_IMAGE_NAME_WIDTH);
		pfnUnicodeStrcpy((PS8)ssName, GetImageNameInFileSystem(Id));
	}
#endif
	return (PS8)ssName;
}



/*****************************************************************************
* FUNCTION
*  PhnsetGetCurScrSvrStatus
* DESCRIPTION
*   This function is to get screen saver status on/off
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  0 OFF, 1 ON
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
SET_SS_SETTING_STATUS PhnsetGetCurScrSvrStatus(void)
{
	switch (g_phnset_cntx_p->curSSStatus)
	{
		case 1:
			return SET_SS_STATUS_ON;
#ifdef __MMI_LCD_PARTIAL_ON__
		case 2:
			return SET_SS_STATUS_POWERSAVING;
#endif /* __MMI_LCD_PARTIAL_ON__ */
		default:
			return SET_SS_STATUS_OFF;
	}
}



/*****************************************************************************
* FUNCTION
*  PhnsetIsScrSvrAnalogClock
* DESCRIPTION
*   This function is to query if screen set Analog Clock
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  0 OFF, 1 ON
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
BOOL PhnsetIsScrSvrAnalogClock(void)
{
#ifdef __MMI_ANALOG_CLOCK__
	if( g_phnset_cntx_p->curSsID == ANALOG_CLOCK )
		return TRUE;
	else
#endif
		return FALSE;
}


/*****************************************************************************
* FUNCTION
*  PhnsetIsScrSvrDigitalClock
* DESCRIPTION
*   This function is to query if screen set Digital Clock
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  0 OFF, 1 ON
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
BOOL PhnsetIsScrSvrDigitalClock(void)
{

#ifdef __MMI_DIGITAL_CLOCK__

	if( g_phnset_cntx_p->curSsID == DIGITAL_CLOCK )
		return TRUE;
	else

#endif
		return FALSE;

}

BOOL PhnsetIsScrSvrPartialOn(void)
{
#ifdef __MMI_LCD_PARTIAL_ON__
	if (g_phnset_cntx_p->curSSStatus == (S32) SET_SS_STATUS_POWERSAVING)
		return TRUE;
	else
#endif /* __MMI_LCD_PARTIAL_ON__ */
		return FALSE;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_phnset_disable_partial_ss
 * DESCRIPTION
 *  To disable partial screensaver.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_phnset_disable_partial_ss(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_LCD_PARTIAL_ON__
    g_phnset_cntx_p->curSSStatus = (S32) SET_SS_STATUS_OFF;
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  mmi_phnset_enable_partial_ss
 * DESCRIPTION
 *  To enable partial screensaver.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_phnset_enable_partial_ss(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_LCD_PARTIAL_ON__
    S16 error;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_LCD_PARTIAL_ON__
    ReadValue(NVRAM_SCREENSAVER_STATUS, &(g_phnset_cntx_p->curSSStatus), DS_BYTE, &error);
#endif 
}


#ifdef __MMI_THEMES_V2_SUPPORT__
BOOL PhnsetIsScrSvrCurrentTheme(void)
{

	if( g_phnset_cntx_p->curSsID == CURRENT_THEME_SCREENSAVER )
		return TRUE;
	else
		return FALSE;

}
#endif

#endif /* _SCREENSAVER_C */


