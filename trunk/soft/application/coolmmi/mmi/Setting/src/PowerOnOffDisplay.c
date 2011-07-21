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
 * PowerOnOffDisplay.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Resource IDs for the Power ON/Off Display
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
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef POWERONOFFDISPLAY_C
#define POWERONOFFDISPLAY_C
 
#include "mmi_features.h" 
#ifdef __MMI_POWER_ON_OFF_DISPLAY__

#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "unicodexdcl.h"
#include "idleappdef.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "downloadprotos.h"
#include "commonscreens.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "wgui_categories.h"
#include "wgui_categories_idlescreen.h"
#include "wgui_categories_multimedia.h"
#include "mainmenudef.h"
#include "timerevents.h"
#include "globaldefs.h"
#include "settingprot.h"
#include "wgui_inline_edit.h"
#include "debuginitdef.h"
#include "historygprot.h"
#include "wpssprotos.h"
#include "settinggprots.h"
#include "wallpaperdefs.h"
#include "poweronoffdisplaydefs.h"
#include "settingdefs.h"
#include "settingprofile.h"
#include "downloaddefs.h"
#include "downloadprotos.h"
#include "phonesetup.h"
#include "mmi_trace.h"
#ifdef __MMI_FILE_MANAGER__   // zrx add 20060612
#include "filemanagergprot.h"
#include "filemanagerdef.h"					/* error string id */
#endif    // zrx add 20060612

/* For user defined file image */
#include "gdi_lcd_config.h"
#include "gdi_datatype.h"
#include "gdi_image.h"
//#include "med_main.h"
#ifdef __MMI_POWER_ON_OFF_DISPLAY_VIDEO__
#include "vdoplystrenum.h"
#include "vdoplygprot.h"
#endif /* __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ */
#ifdef __MMI_FILE_MANAGER__
#include "filemgr.h"
static S8 *phnset_pwr_on_off_strp = NULL;  
#endif

#include "conversions.h"
extern U8 PhnsetGetDefEncodingType(void);

#undef __NEWSIMULATOR

#include "gdi_include.h"  /* gdi_result */
#include "wgui_categories_multimedia.h"  /* showcategory222screen() */
extern pBOOL IsSilentModeActivated(void);
extern void SetIdlePowerOnName(UI_string_type name);
extern void wgui_save_PowerON_to_speedup(void);

extern U8 isCorrectSet;
PHNSET_PWRONOFF_FILENAME_STRUCT phnset_pwronoff_filename;


static void PhnsetPwrOnOffUsrDefFreeBuf(void);
static void PhnsetPwrOnOffUsrDefGoBack(void);
static void ExitPhnsetPwrOnOffSysView( void);

 
static FuncPtr CallbackPWRLSK = NULL; 
static U8 MAX_ON_ANIMATION =IMG_ID_PHNSET_ON_ANIMATION_MAX-IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1;
static U8 MAX_OFF_ANIMATION =IMG_ID_PHNSET_OFF_ANIMATION_MAX-IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1;

#ifdef __MMI_SAVE_PON_SPEEDUP__ /*gdm modify to add  the switch of __MMI_SAVE_PON_SPEEDUP__  20091125*/
const U16 PHNSET_PREDEFINED_PWO_PATH_JPG[]= {0X63, 0X3a, 0X5c,  0X55, 0X73, 0X65, 0X72, 0X50, 0X4f, 0X4e, 0X2e, 0X6a, 0X70, 0X67, 0X00, 0X00, 0X00,};// "c:\\UserPON.jpg\0"; 
const U16 PHNSET_PREDEFINED_PWO_PATH_PBM[]={0X63, 0X3a, 0X5c,  0X55, 0X73, 0X65, 0X72, 0X50, 0X4f, 0X4e, 0X2e, 0X50, 0X42, 0X4d, 0X00, 0X00, 0X00, };// "c:\\UserPON.pbm\0"; 
S8 *idle_screen_poweron_name_original = (S8 *)phnset_pwronoff_filename.pwrondisplay;
S8	*idle_screen_poweron_name;
#endif
/*****************************************************************************
* FUNCTION
*  readPwrOnOffDisplaySettings
* DESCRIPTION
*  This function is to read power on/off display from NVRAM at initialization
*	, and set as current 
*   Functionality: Referenced at InitNvramData when bootup
*
* PARAMETERS
*  a  IN/OUT      void *
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void readPwrOnOffDisplaySettings(void)
{
#if 0 //dyj del 20060514
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 error;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ReadValue( NVRAM_CURRENT_POWER_ON_DISPLAY, &(g_phnset_cntx_p->curtPwrOnID), DS_SHORT, &error);
		
	if( (g_phnset_cntx_p->curtPwrOnID) == 0xFFFF || error == NVRAM_READ_FAIL)
	{
		PhnsetRstPwrOnDisplayDefault();
	}
	else if( (g_phnset_cntx_p->curtPwrOnID) == 0x00 
			|| (g_phnset_cntx_p->curtPwrOnID) == 0x01)//should hint short file name
	{
		ReadRecord( NVRAM_EF_WPSS_FILENAME_LID, 2, &phnset_pwronoff_filename, sizeof(phnset_pwronoff_filename), &error );
		ASSERT(error==NVRAM_READ_SUCCESS);
	}
	ReadValue( NVRAM_CURRENT_POWER_OFF_DISPLAY, &(g_phnset_cntx_p->curtPwrOffID), DS_SHORT, &error);

	if( (g_phnset_cntx_p->curtPwrOffID) == 0xFFFF || error == NVRAM_READ_FAIL)
	{
		PhnsetRstPwrOffDisplayDefault();
	}
	else if((g_phnset_cntx_p->curtPwrOffID) <= 0x01
		&& (g_phnset_cntx_p->curtPwrOnID) > 0x01)//should hint short file name
	{
		ReadRecord( NVRAM_EF_WPSS_FILENAME_LID, 2, &phnset_pwronoff_filename, sizeof(phnset_pwronoff_filename), &error );
		ASSERT(error==NVRAM_READ_SUCCESS);
	}
#endif	

 
	S16 error;

	ReadValue( NVRAM_CURRENT_POWER_ON_DISPLAY, &(g_phnset_cntx_p->curtPwrOnID), DS_SHORT, &error);
		
	if( (g_phnset_cntx_p->curtPwrOnID) == 0xFFFF || error == NVRAM_READ_FAIL)
	{
		PhnsetRstPwrOnDisplayDefault();
	}
	else if( (g_phnset_cntx_p->curtPwrOnID) == 0x00 
			|| (g_phnset_cntx_p->curtPwrOnID) == 0x01)//should hint short file name
	{
		ReadRecord( NVRAM_EF_WPSS_FILENAME_LID, 2, &phnset_pwronoff_filename, sizeof(phnset_pwronoff_filename), &error );
		ASSERT(error==NVRAM_READ_SUCCESS);
	}
	ReadValue( NVRAM_CURRENT_POWER_OFF_DISPLAY, &(g_phnset_cntx_p->curtPwrOffID), DS_SHORT, &error);

	if( (g_phnset_cntx_p->curtPwrOffID) == 0xFFFF || error == NVRAM_READ_FAIL)
	{
		PhnsetRstPwrOffDisplayDefault();
	}
	else if((g_phnset_cntx_p->curtPwrOffID) <= 0x01
		&& (g_phnset_cntx_p->curtPwrOnID) > 0x01)//should hint short file name
	{
		ReadRecord( NVRAM_EF_WPSS_FILENAME_LID, 2, &phnset_pwronoff_filename, sizeof(phnset_pwronoff_filename), &error );
		ASSERT(error==NVRAM_READ_SUCCESS);
	}
 
}

/*****************************************************************************
* FUNCTION
*	HighlightPhnsetPowerOn
* DESCRIPTION
*  hilight handler when move to power on display option
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void HighlightPhnsetPowerOn(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ChangeLeftSoftkey(STR_GLOBAL_OK,0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_PWRON;
	
#if (defined (__MMI_WAP__) || (defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__) ) ) 
      /* Go to menu with 2 items: System, Download */
	SetLeftSoftkeyFunction(EntryPhnsetPowerOnSel,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetPowerOnSel, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	g_phnset_cntx_p->ScrAnchor = SCR_ID_PHNSET_POWER_ON;

#else
      /* Direct enter system wallpaper list menu */
	SetLeftSoftkeyFunction(EntryPhnsetPowerOnOffSys,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetPowerOnOffSys, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	g_phnset_cntx_p->ScrAnchor = SCR_ID_PHNSET_POWER_ON_SYSTEM_LIST;

#endif


	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
/*****************************************************************************
* FUNCTION
*	HighlightPhnsetPowerOff
* DESCRIPTION
*  hilight handler when move to power off display option
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void HighlightPhnsetPowerOff(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	ChangeLeftSoftkey(STR_GLOBAL_OK,0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_PWROFF;
	
#if (defined (__MMI_WAP__) || (defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__) ) ) 
      /* Go to menu with 2 items: System, Download */
	SetLeftSoftkeyFunction(EntryPhnsetPowerOffSel,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetPowerOffSel, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	g_phnset_cntx_p->ScrAnchor = SCR_ID_PHNSET_POWER_OFF;

#else
      /* Direct enter system wallpaper list menu */
	SetLeftSoftkeyFunction(EntryPhnsetPowerOnOffSys,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetPowerOnOffSys, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	g_phnset_cntx_p->ScrAnchor = SCR_ID_PHNSET_POWER_OFF_SYSTEM_LIST;

#endif


	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
/*****************************************************************************
* FUNCTION
*	HighlightPhnsetPwrOnFmgr
* DESCRIPTION
*  hilight handler when move to power on display "user define" option
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void HighlightPhnsetPwrOnFmgr(void)
{
#if 1 //modified by lin for pwron animation dwnl @2007/04/16
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_PWRON;

	SetLeftSoftkeyFunction(EntryPhnsetWallpaperFmgr,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetWallpaperFmgr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	#endif 
}
/*****************************************************************************
* FUNCTION
*	HighlightPhnsetPwrOffFmgr
* DESCRIPTION
*  hilight handler when move to power off display "user define" option
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void HighlightPhnsetPwrOffFmgr(void)
{
#if 1 //modified by lin for pwroff animation dwnl @2007/04/16
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_PWROFF;

	SetLeftSoftkeyFunction(EntryPhnsetWallpaperFmgr,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetWallpaperFmgr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	#endif
}
/*****************************************************************************
* FUNCTION
*	HighlightPhnsetPowerOnSys
* DESCRIPTION
*  hilight handler when move to power on display "System" option
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void HighlightPhnsetPowerOnSys(void)
{
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_PWRON;
	SetLeftSoftkeyFunction(EntryPhnsetPowerOnOffSys,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetPowerOnOffSys, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
/*****************************************************************************
* FUNCTION
*	HighlightPhnsetPowerOffSys
* DESCRIPTION
*  hilight handler when move to power off display "System" option
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void HighlightPhnsetPowerOffSys(void)
{
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_PWROFF;
	SetLeftSoftkeyFunction(EntryPhnsetPowerOnOffSys,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetPowerOnOffSys, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
/*****************************************************************************
* FUNCTION
*	HintPhnsetPwrOnDisplay
* DESCRIPTION
*  hilight handler when move to power off display "System" option
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifndef FMGR_MAX_FILE_LEN
#define  FMGR_MAX_FILE_LEN  MAX_SUBMENU_CHARACTERS  // zrx add 20060612
#endif
void HintPhnsetPwrOnDisplay(U16 index)
{
     PS8 p;

	if(g_phnset_cntx_p->curtPwrOnID==0)
		p = PhnsetGetFileNamePointer( (PS8 )phnset_pwronoff_filename.pwrondisplay);
	else if(g_phnset_cntx_p->curtPwrOnID==1)
	{
		U8 temp[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];
		/* rename short name file should pass converted short name to editor,
			so that the display will be the same as in File Manager */
		p = PhnsetGetFileNamePointer( (PS8 )phnset_pwronoff_filename.pwrondisplay);
#if 0 // zrx del 20060612
		mmi_chset_mixed_text_to_ucs2_str((U8* )temp, 
													(FMGR_MAX_FILE_LEN+1)* ENCODING_LENGTH, 
													(U8*)p, 
													(mmi_chset_enum)PhnsetGetDefEncodingType() );
		#endif
		p = (PS8)&temp[0];
		
	}
	else 
	{
#if defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ )
		if(g_phnset_cntx_p->curtPwrOnID < IMG_ID_PHNSET_ON_OFF_VIDEO_MAX 
			&& g_phnset_cntx_p->curtPwrOnID> IMG_ID_PHNSET_ON_OFF_VIDEO_START)
		{
			p = GetString(STR_ID_PHNSET_ON_OFF_DEFAULT);
		}
		else
#endif// defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ ) && !defined(WIN32)
		if((g_phnset_cntx_p->curtPwrOnID < IMG_ID_PHNSET_ON_ANIMATION_MAX) 
		&& (g_phnset_cntx_p->curtPwrOnID> IMG_ID_PHNSET_ON_OFF_VIDEO_MAX))
		{
			p = GetString(STR_ID_PHNSET_ON_OFF_ANIMATION);
		}
		else
		{
			p = GetString((U16)(STR_ID_PHNSET_ON_OFF_1 + (g_phnset_cntx_p->curtPwrOnID -IMG_ID_PHNSET_ON_OFF_1)));
		}
	}
	
	if(p)
   	   pfnUnicodeStrcpy((PS8)hintData[index], p);
	
}
void HintPhnsetPwrOffDisplay(U16 index)
{
     PS8 p;

	if(g_phnset_cntx_p->curtPwrOffID==0)
	{
		p = PhnsetGetFileNamePointer( (PS8 )phnset_pwronoff_filename.pwroffdisplay);
	}
	else if(g_phnset_cntx_p->curtPwrOffID==1)
	{
		U8 temp[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];
		/* rename short name file should pass converted short name to editor,
			so that the display will be the same as in File Manager */
		p = PhnsetGetFileNamePointer( (PS8 )phnset_pwronoff_filename.pwroffdisplay);
#if 0 // zrx del 20060612
		mmi_chset_mixed_text_to_ucs2_str((U8* )temp, 
													(FMGR_MAX_FILE_LEN+1)* ENCODING_LENGTH, 
													(U8*)p, 
													(mmi_chset_enum)PhnsetGetDefEncodingType() );
		#endif
		p = (PS8)&temp[0];
	}
	else
	{
#if defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ )
		if(g_phnset_cntx_p->curtPwrOffID < IMG_ID_PHNSET_ON_OFF_VIDEO_MAX && 
		g_phnset_cntx_p->curtPwrOffID> IMG_ID_PHNSET_ON_OFF_VIDEO_START)
		{
			p = GetString(STR_ID_PHNSET_ON_OFF_DEFAULT);
		}
		else
#endif //defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ ) && !defined(WIN32)
 
		if((g_phnset_cntx_p->curtPwrOffID < IMG_ID_PHNSET_OFF_ANIMATION_MAX 
		&& g_phnset_cntx_p->curtPwrOffID> IMG_ID_PHNSET_ON_OFF_VIDEO_MAX))
		{
			p = GetString(STR_ID_PHNSET_ON_OFF_ANIMATION);
		}
		else
 
		{
			p = GetString((U16)(STR_ID_PHNSET_ON_OFF_1 + (g_phnset_cntx_p->curtPwrOffID -IMG_ID_PHNSET_ON_OFF_1)));
		}
	}

	
     if( p)
   	   pfnUnicodeStrcpy((PS8)hintData[index], p);
}

void EntryPhnsetPowerOnSel(void)
{
	U8 *guiBuffer;
	U16 nNumofItem;
	U16 PowerOnSelectList[MAX_SUB_MENUS]; //= { STR_ID_PHNSET_ON_OFF_SYSTEM, STR_ID_PHNSET_ON_OFF_DOWNLOAD };

	EntryNewScreen(SCR_ID_PHNSET_POWER_ON, NULL, EntryPhnsetPowerOnSel, NULL);
	g_phnset_cntx_p->ScrAnchor = SCR_ID_PHNSET_POWER_ON;

	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_POWER_ON);

	nNumofItem = GetNumOfChild(MENU_SETTING_POWER_ON_DISPLAY);
	GetSequenceStringIds(MENU_SETTING_POWER_ON_DISPLAY,PowerOnSelectList);
	SetParentHandler(MENU_SETTING_POWER_ON_DISPLAY);

	SetHiliteHandler(MENU_SETTING_POWER_ON_SYSTEM,   HighlightPhnsetPowerOnSys);
	
#if defined(__MMI_FILE_MANAGER__)
	SetHiliteHandler(MENU_SETTING_POWER_ON_DOWNLOAD, HighlightPhnsetPwrOnFmgr);
#elif defined( __MMI_WAP__ )
	SetHiliteHandler(MENU_SETTING_POWER_ON_DOWNLOAD, HighlightPhnsetWallpaperDwnl);
#endif
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory15Screen(STR_SETTING_POWER_ON_DISPLAY, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, nNumofItem,
							PowerOnSelectList, (PU16)gIndexIconsImageList,  1, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

void EntryPhnsetPowerOffSel(void)
{
	U8 *guiBuffer;
	U16 nNumofItem;
	U16 PowerOffSelectList[MAX_SUB_MENUS]; //= { STR_ID_PHNSET_ON_OFF_SYSTEM, STR_ID_PHNSET_ON_OFF_DOWNLOAD };

   EntryNewScreen(SCR_ID_PHNSET_POWER_OFF, NULL, EntryPhnsetPowerOffSel, NULL);
	g_phnset_cntx_p->ScrAnchor = SCR_ID_PHNSET_POWER_OFF;

	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_POWER_OFF);

	nNumofItem = GetNumOfChild(MENU_SETTING_POWER_OFF_DISPLAY);
	GetSequenceStringIds(MENU_SETTING_POWER_OFF_DISPLAY,PowerOffSelectList);
	SetParentHandler(MENU_SETTING_POWER_OFF_DISPLAY);

	SetHiliteHandler(MENU_SETTING_POWER_OFF_SYSTEM,   HighlightPhnsetPowerOffSys);
	
#if defined(__MMI_FILE_MANAGER__)
	SetHiliteHandler(MENU_SETTING_POWER_OFF_DOWNLOAD, HighlightPhnsetPwrOffFmgr);
#elif defined( __MMI_WAP__ )
	SetHiliteHandler(MENU_SETTING_POWER_OFF_DOWNLOAD, HighlightPhnsetWallpaperDwnl);
#endif
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory15Screen(	STR_SETTING_POWER_OFF_DISPLAY, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, nNumofItem,
							PowerOffSelectList, (PU16)gIndexIconsImageList,  1, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}


void EntryPhnsetPowerOnOffSys(void)
{
	U8 *guiBuffer = NULL;
	U8 item_amount = 0;
	
	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON)
	{
		EntryNewScreen(SCR_ID_PHNSET_POWER_ON_SYSTEM_LIST, NULL, EntryPhnsetPowerOnOffSys, NULL);
		guiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_POWER_ON_SYSTEM_LIST);
	}
	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF)
	{
		EntryNewScreen(SCR_ID_PHNSET_POWER_OFF_SYSTEM_LIST, NULL, EntryPhnsetPowerOnOffSys, NULL);
		guiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_POWER_OFF_SYSTEM_LIST);
	}
	else
	{
		ASSERT(0);
	}
	
	RegisterHighlightHandler(PhnsetPowerOnOffSysHiliteHandler);

	g_phnset_cntx_p->curHightlightItem = 0;
	item_amount = PhnsetConstSysPwrOnOffList(subMenuDataPtrs, g_phnset_cntx_p->SysWPSSList , (S16 *)&(g_phnset_cntx_p->curHightlightItem));

	g_phnset_cntx_p->WPSSPreviewScrFlag = 1;

	ShowCategory36Screen(STR_ID_PHNSET_ON_OFF_SYSTEM,IMG_SCR_SETTING_CAPTION,STR_GLOBAL_VIEW, 0,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							item_amount, subMenuDataPtrs, (U16)(g_phnset_cntx_p->curHightlightItem), guiBuffer);

	SetLeftSoftkeyFunction(EntryPhnsetPwrOnOffSysView,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetPwrOnOffSysView,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	
}


void EntryPhnsetPwrOnOffSysView(void){
	U16 strID = 0;
	U8 *guiBuffer = NULL;

	CallbackPWRLSK = PhnsetSetPwrOnOffDisplay;
	isCorrectSet = 0;
  	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON)
  	{
		guiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW);

		if(g_phnset_cntx_p->WPSSPreviewScrFlag)
			EntryNewScreen(SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW, ExitPhnsetPwrOnOffSysView, EntryPhnsetPwrOnOffSysView, NULL);
  	}
  	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF)
  	{
		guiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_POWER_OFF_SYSTEM_VIEW);

		if(g_phnset_cntx_p->WPSSPreviewScrFlag)
			EntryNewScreen(SCR_ID_PHNSET_POWER_OFF_SYSTEM_VIEW, ExitPhnsetPwrOnOffSysView, EntryPhnsetPwrOnOffSysView, NULL);
  	}
	else
	{
		MMI_ASSERT(0);
	}		

#ifdef __MMI_NOKIA_STYLE_N800__
    entry_full_screen();
#endif
 
#if defined(__MMI_DRAW_MANAGER__)
	wgui_set_touch_title_bar_buttons();//enable the display of title bar buttons
#endif

#if defined(__MMI_TOUCH_SCREEN__)
//BUG IMAGE VIEW START 20050917
//register the handlers for pen down event.
	register_touch_button_handlers(NULL, PhnsetPwrOnOffSysViewPre,
			NULL, PhnsetPwrOnOffSysViewNext);
//BUG IMAGE VIEW END 20050917
#endif
 

#if defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ )
	if(g_phnset_cntx_p->curHightlightItem<MAX_ONOFF_VIDEO)
	{
		strID = STR_ID_PHNSET_ON_OFF_DEFAULT;
		
	 
	#if defined(__MMI_DRAW_MANAGER__)
		ShowCategory225Screen(strID,								/* title_str */
									0,								/* title_icon */
									STR_GLOBAL_OK,						/* lsk_str */
									IMG_GLOBAL_OK,						/* lsk_icon */
									STR_GLOBAL_BACK,					/* rsk_str */
									IMG_GLOBAL_BACK,					/* rsk_icon */
									IMG_ID_PHNSET_ON_OFF_DEFAULT1,	/* video_id */
									NULL,									/* video_filename */
									1,										/* repeat_count */
									TRUE,									/* is_visual_update */
									!IsSilentModeActivated(),		/* is_play_audio */
									TRUE,									/* is_lcd_no_sleep */
									GDI_COLOR_BLACK,					/* bg_color */
									PhnsetPwrOnOffPlayCallBack,	/* callback */
									guiBuffer);							/* gui_buffer */
	#else
		ShowCategory225Screen(strID,								/* title_str */
									IMG_SCR_SETTING_CAPTION,		/* title_icon */
									STR_GLOBAL_OK,						/* lsk_str */
									IMG_GLOBAL_OK,						/* lsk_icon */
									STR_GLOBAL_BACK,					/* rsk_str */
									IMG_GLOBAL_BACK,					/* rsk_icon */
									IMG_ID_PHNSET_ON_OFF_DEFAULT1,	/* video_id */
									NULL,									/* video_filename */
									1,										/* repeat_count */
									TRUE,									/* is_visual_update */
									!IsSilentModeActivated(),		/* is_play_audio */
									TRUE,									/* is_lcd_no_sleep */
									GDI_COLOR_BLACK,					/* bg_color */
									PhnsetPwrOnOffPlayCallBack,	/* callback */
									guiBuffer);				
	#endif
	 
	     
			SetLeftSoftkeyFunction(CallbackPWRLSK,KEY_EVENT_UP);
			SetRightSoftkeyFunction (PhnsetPwrOnOffUsrDefGoBack, KEY_EVENT_UP);
	#ifdef __MMI_NOKIA_STYLE_N800__
	        ClearKeyHandler( KEY_LSK,  KEY_EVENT_DOWN)
        ClearKeyHandler( KEY_RSK,  KEY_EVENT_DOWN)

                    SetKeyHandler(CallbackPWRLSK , KEY_LSK,  KEY_EVENT_UP)
                    SetKeyHandler(PhnsetPwrOnOffUsrDefGoBack , KEY_RSK,  KEY_EVENT_UP)

	#endif		
	} 
	else
#endif //defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ ) && !defined(WIN32)
	{
		U8 imgName[MAX_IMAGE_NAME_WIDTH];
		U8 max_animation = 0;

		if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON)
			max_animation = MAX_ON_ANIMATION;
		else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF)		
			max_animation = MAX_OFF_ANIMATION;

		if( (g_phnset_cntx_p->curHightlightItem>(MAX_ONOFF_VIDEO-1))
		&&	(g_phnset_cntx_p->curHightlightItem<(MAX_ONOFF_VIDEO+max_animation)) )
		{
			strID = STR_ID_PHNSET_ON_OFF_ANIMATION;
			pfnUnicodeStrcpy((PS8)imgName, GetString(strID));
			if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON)
 
#if defined(__MMI_DRAW_MANAGER__)
				ShowCategory129Screen(imgName, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1, guiBuffer);
#else
				ShowCategory129Screen(imgName, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1, guiBuffer);
#endif
 
			else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF )
 
#if defined(__MMI_DRAW_MANAGER__)
				ShowCategory129Screen(imgName, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1, guiBuffer);
#else
				ShowCategory129Screen(imgName, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1, guiBuffer);
#endif
 

		}
		else
		{
			strID = STR_ID_PHNSET_ON_OFF_1 + g_phnset_cntx_p->curHightlightItem-(MAX_ONOFF_VIDEO+MAX_ON_ANIMATION);
			pfnUnicodeStrcpy((PS8)imgName, GetString(strID));
 
#if defined(__MMI_DRAW_MANAGER__)
			ShowCategory129Screen(imgName, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK,
					STR_GLOBAL_BACK, IMG_GLOBAL_BACK, (U16) (g_phnset_cntx_p->SysWPSSList[g_phnset_cntx_p->curHightlightItem]), guiBuffer);
#else
			ShowCategory129Screen(imgName, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK, (U16)(g_phnset_cntx_p->SysWPSSList[g_phnset_cntx_p->curHightlightItem]), guiBuffer);
#endif
 
			
		}
		SetLeftSoftkeyFunction(PhnsetSetPwrOnOffDisplay,KEY_EVENT_UP);

		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

#ifdef __MMI_NOKIA_STYLE_N800__
                    SetKeyHandler(PhnsetSetPwrOnOffDisplay , KEY_LSK,  KEY_EVENT_UP);
                    SetKeyHandler(GoBackHistory , KEY_RSK,  KEY_EVENT_UP);
        ClearKeyHandler( KEY_LSK,  KEY_EVENT_DOWN);
        ClearKeyHandler( KEY_RSK,  KEY_EVENT_DOWN);

#endif
	//SetGenericExitHandler(SCR_WALLPAPER_VIEW, ExitPhnsetWallpaperSysView, EntryPhnsetWallpaperSysView);

	}
	

 
/* changed the up-down arrow key handlers to left-right arrow key handlers
	for having the similar behaviour of keypad and touchscreen. */
//		SetKeyHandler(PhnsetPwrOnOffSysViewPre,	KEY_UP_ARROW,	KEY_EVENT_UP);
//		SetKeyHandler(PhnsetPwrOnOffSysViewNext,	KEY_DOWN_ARROW,	KEY_EVENT_UP);
//BUG IMAGE VIEW START 20050917
// change the key handler for KEY UP event to KEY DOWN event.
		SetKeyHandler(PhnsetPwrOnOffSysViewPre,	KEY_LEFT_ARROW,	KEY_EVENT_DOWN);
		SetKeyHandler(PhnsetPwrOnOffSysViewNext,	KEY_RIGHT_ARROW,	KEY_EVENT_DOWN);
//BUG IMAGE VIEW END 20050917
 
		/*reserve for video volume
		SetKeyHandler(PhnsetPwrOnOffSysViewPre,	KEY_VOL_UP,		KEY_EVENT_DOWN);
		SetKeyHandler(PhnsetPwrOnOffSysViewPre,	KEY_VOL_UP,		KEY_LONG_PRESS);

		SetKeyHandler(PhnsetPwrOnOffSysViewNext,	KEY_VOL_DOWN,	KEY_EVENT_DOWN);
		SetKeyHandler(PhnsetPwrOnOffSysViewNext,	KEY_VOL_DOWN,	KEY_LONG_PRESS);
		*/

}

static void PhnsetPwrOnOffUsrDefFreeBuf(void)
{
#ifdef __MMI_FILE_MANAGER__   // zrx add 20060612
	if(phnset_pwr_on_off_strp)
    {
		OslMfree( phnset_pwr_on_off_strp );
		phnset_pwr_on_off_strp = NULL;
	}
	#endif // zrx add 20060612
}


static void PhnsetPwrOnOffUsrDefGoBack(void)
{
	PhnsetPwrOnOffUsrDefFreeBuf();
  	GoBackHistory();
}

void PhnsetPwrOnOffSysViewPre(void)
{
	ExitCategoryFunction();
	if(g_phnset_cntx_p->curHightlightItem <= 0 )
		g_phnset_cntx_p->curHightlightItem = MAX_SYSTEM_WALLPAPER+MAX_ONOFF_VIDEO+MAX_ON_ANIMATION;
	
	g_phnset_cntx_p->curHightlightItem--;
	g_phnset_cntx_p->WPSSPreviewScrFlag = 0;
	EntryPhnsetPwrOnOffSysView();
}

void PhnsetPwrOnOffSysViewNext(void)
{
	ExitCategoryFunction();
	if(g_phnset_cntx_p->curHightlightItem >= MAX_SYSTEM_WALLPAPER+MAX_ONOFF_VIDEO+MAX_ON_ANIMATION-1 )
		g_phnset_cntx_p->curHightlightItem = -1;

	g_phnset_cntx_p->curHightlightItem++;
	g_phnset_cntx_p->WPSSPreviewScrFlag = 0;
	EntryPhnsetPwrOnOffSysView();
}

void PhnsetPowerOnOffSysHiliteHandler( S32 nIndex)
{
#if defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ )
	if(nIndex<MAX_ONOFF_VIDEO)
	{
		ChangeLeftSoftkey(STR_ID_VDOPLY_PLAY, 0);
	}
	else
		ChangeLeftSoftkey(STR_GLOBAL_VIEW, 0);
#endif // defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ ) && !defined(WIN32)
	g_phnset_cntx_p->curHightlightItem = (U16)nIndex;

}

U8 PhnsetConstSysPwrOnOffList( U8 **pSysPwrOnOffStrList, U16 *pSysPwrOnOffImgIDList, S16 *HighliteItem )
{
	S16 i=0;
	U16 strID = 0;
	U8 max_animation = 0;

	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON)
		max_animation = MAX_ON_ANIMATION;
	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF)		
		max_animation = MAX_OFF_ANIMATION;

	
#if defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ )
	for(i=0;i<MAX_ONOFF_VIDEO;i++)
	{
		if( pSysPwrOnOffImgIDList ) 
	   	pSysPwrOnOffImgIDList[i] = IMG_ID_PHNSET_ON_OFF_DEFAULT1+i;
		
		if( pSysPwrOnOffStrList ) 
		   pSysPwrOnOffStrList[i] = (PU8)GetString(STR_ID_PHNSET_ON_OFF_DEFAULT);

		if(( g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON 
			&&g_phnset_cntx_p->curtPwrOnID ==pSysPwrOnOffImgIDList[i] )
			||
			( g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF 
			&&g_phnset_cntx_p->curtPwrOffID ==pSysPwrOnOffImgIDList[i] ))
				*HighliteItem = i;
	}
#endif //defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ ) && !defined(WIN32)

		if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON )
		{
			for(i=0;i<max_animation;i++)
			{
				if( pSysPwrOnOffImgIDList ) 
				   pSysPwrOnOffImgIDList[i+MAX_ONOFF_VIDEO] = IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1 + i;
		
				if( pSysPwrOnOffStrList ) 
				   pSysPwrOnOffStrList[i+MAX_ONOFF_VIDEO] = (PU8)GetString(STR_ID_PHNSET_ON_OFF_ANIMATION);

				if(g_phnset_cntx_p->curtPwrOnID ==pSysPwrOnOffImgIDList[i+MAX_ONOFF_VIDEO])
					*HighliteItem = i+MAX_ONOFF_VIDEO;
			}
		}
		else 	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF )
		{
			for(i=0;i<MAX_OFF_ANIMATION;i++)
			{
				if( pSysPwrOnOffImgIDList ) 
				   pSysPwrOnOffImgIDList[i+MAX_ONOFF_VIDEO] = IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1 + i;
		
				if( pSysPwrOnOffStrList ) 
				   pSysPwrOnOffStrList[i+MAX_ONOFF_VIDEO] = (PU8)GetString(STR_ID_PHNSET_ON_OFF_ANIMATION);

				if(g_phnset_cntx_p->curtPwrOffID ==pSysPwrOnOffImgIDList[i+MAX_ONOFF_VIDEO])
					*HighliteItem = i+MAX_ONOFF_VIDEO;
			}
		}



/*********SYSTEM WALLPAPER**************/	
	for( i=0; i<MAX_SYSTEM_WALLPAPER ; i++ )
{
		strID = STR_ID_PHNSET_ON_OFF_1 + i;

		if( pSysPwrOnOffImgIDList ) 
		   pSysPwrOnOffImgIDList[i+MAX_ONOFF_VIDEO+max_animation] = IMG_ID_PHNSET_ON_OFF_1 + i;
		
		if( pSysPwrOnOffStrList ) 
		   pSysPwrOnOffStrList[i+MAX_ONOFF_VIDEO+max_animation] = (PU8)GetString(strID);
		
		if( HighliteItem )
		{
			if(( g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON 
				&&g_phnset_cntx_p->curtPwrOnID ==pSysPwrOnOffImgIDList[i+MAX_ONOFF_VIDEO+max_animation] )
				||
				( g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF 
				&&g_phnset_cntx_p->curtPwrOffID ==pSysPwrOnOffImgIDList[i+MAX_ONOFF_VIDEO+max_animation] ))
			{
					*HighliteItem = i+MAX_ONOFF_VIDEO+max_animation;
			}
		}
	}


/*********FACTORY WALLPAPER*************
	g_phnset_cntx_p->WPFctyImgNum = GetFullScreenFactoryImagesInfo(&Info);

	for(  i=0; i<g_phnset_cntx_p->WPFctyImgNum; i++ )
	{
		strID = STR_ID_PHNSET_ON_OFF_1 + MAX_SYSTEM_WALLPAPER + i;

		if( pSysPwrOnOffImgIDList ) 
		   pSysPwrOnOffImgIDList[MAX_SYSTEM_WALLPAPER + i+1] = Info[i].ImagesId;

		if( pSysPwrOnOffStrList ) 
              pSysPwrOnOffStrList[MAX_SYSTEM_WALLPAPER + i+1] = (PU8)GetString(strID);

		if( HighliteItem )
   		   if( g_phnset_cntx_p->curtWpID == Info[i].ImagesId )
			   *HighliteItem = MAX_SYSTEM_WALLPAPER + i+1;
	}

    wp_cnt+=g_phnset_cntx_p->WPFctyImgNum;
	*/

	i=MAX_ONOFF_VIDEO+ max_animation+MAX_SYSTEM_WALLPAPER /*+g_phnset_cntx_p->SSFctyImgNum*/ -1;

    return i+1; //amount, will be used to construct list
	
}

/*External Interface*/
void PhnsetGetPwrOnDisp(U16 *id)
{
	*id = g_phnset_cntx_p->curtPwrOnID;
}
PS8 PhnsetGetPwrOnFileName(void)
{
#ifdef __MMI_SAVE_PON_SPEEDUP__ /*gdm modify to add  the switch of __MMI_SAVE_PON_SPEEDUP__  20091125*/
	mmi_trace(g_sw_WpSsPwon, "gdm:PhnsetGetPwrOnFileName, here++++++++++++++");
	if ( pfnUnicodeStrcmp((PS8)phnset_pwronoff_filename.pwrondisplaylayer, (PS8)PHNSET_PREDEFINED_PWO_PATH_PBM) == 0 )
		return (PS8)phnset_pwronoff_filename.pwrondisplaylayer;
#endif

	return (PS8)phnset_pwronoff_filename.pwrondisplay;
}
void PhnsetGetPwrOffDisp(U16 *id)
{
	*id = g_phnset_cntx_p->curtPwrOffID;	
}
PS8 PhnsetGetPwrOffFileName(void)
{
	return (PS8)phnset_pwronoff_filename.pwroffdisplay;
}
pBOOL PhnsetRstPwrOnDisplayDefault(void)
{
	S16 error;	
	ReadValue(NVRAM_RESTORE_POWER_ON_DISPLAY, &(g_phnset_cntx_p->curtPwrOnID), DS_SHORT, &error);
	if(error == NVRAM_READ_FAIL)
		return FALSE;
	if(g_phnset_cntx_p->curtPwrOnID==0xFFFF)
		g_phnset_cntx_p->curtPwrOnID=IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1;
	
	WriteValue( NVRAM_CURRENT_POWER_ON_DISPLAY, &(g_phnset_cntx_p->curtPwrOnID), DS_SHORT, &error);

	if(error == NVRAM_WRITE_SUCCESS)
		return TRUE;
	else
		return FALSE;
}

pBOOL PhnsetGetPwrOnDisplayDefault(U16 *id)
{
	S16 error;	
	
	ReadValue(NVRAM_RESTORE_POWER_ON_DISPLAY, id, DS_SHORT, &error);

	if(*id==0xFFFF)
		*id=IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1;

	
	if(error == NVRAM_READ_FAIL)
		return FALSE;

	return TRUE;
}

pBOOL PhnsetRstPwrOffDisplayDefault(void)
{
	S16 error;	
	ReadValue(NVRAM_RESTORE_POWER_OFF_DISPLAY, &(g_phnset_cntx_p->curtPwrOffID), DS_SHORT, &error);
	if(error == NVRAM_READ_FAIL)
		return FALSE;
	if(g_phnset_cntx_p->curtPwrOffID==0xFFFF)
		g_phnset_cntx_p->curtPwrOffID=IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1;

	WriteValue( NVRAM_CURRENT_POWER_OFF_DISPLAY, &(g_phnset_cntx_p->curtPwrOffID), DS_SHORT, &error);

	if(error == NVRAM_WRITE_SUCCESS)
		return TRUE;
	else
		return FALSE;
}

pBOOL PhnsetGetPwrOffDisplayDefault(U16 *id)
{
	S16 error;	
	
	ReadValue(NVRAM_RESTORE_POWER_OFF_DISPLAY, id, DS_SHORT, &error);
	
	if(*id==0xFFFF)
		*id=IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1;

	if(error == NVRAM_READ_FAIL)
		return FALSE;

	return TRUE;
}



/*****************************************************************************
* FUNCTION
*   PhnsetPwrOnOffPlayCallBack
* DESCRIPTION
*   This function is to set current wallpaper using index by AT command
* PARAMETERS
* 	 result	  IN		play result     
* RETURNS
*   void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void PhnsetPwrOnOffPlayCallBack(S32 result)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	
	if(result>=0)
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

		CallbackPWRLSK = PhnsetSetPwrOnOffDisplay;
	}
	else
	{
		ChangeLeftSoftkey(0,0);
		ChangeRightSoftkey(0,0);
		CallbackPWRLSK = UI_dummy_function;
		
		PhnsetPwrOnOffBadDisplayPopup(NULL);
		PhnsetPwrOnOffUsrDefFreeBuf();

		/* delete to file select screen again */
		DeleteNHistory(1);
	}

		SetLeftSoftkeyFunction(CallbackPWRLSK,KEY_EVENT_UP);
}


void PhnsetPwrOnOffBadDisplayPopup(PS8 error_str)
{
#ifdef __MMI_FILE_MANAGER__   // zrx add end
	if(error_str == NULL)
	{
	  	DisplayPopup(	(PU8)GetString(STR_FMGR_INVALID_FORMAT),
      	          	IMG_GLOBAL_WARNING, 1,
							UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);

	}
	else
	{
	  	DisplayPopup(	(PU8)error_str, IMG_GLOBAL_WARNING, 1,
							UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	}
	#endif  // zrx add 20060612
}
void ExitPhnsetPwrOnOffSysView( void)
{
	if(isCorrectSet)
	{
		DeleteUptoScrID(g_phnset_cntx_p->ScrAnchor);
#ifdef __MMI_FILE_MANAGER__
		PhnsetPwrOnOffUsrDefFreeBuf();
#endif
		g_phnset_cntx_p->WPSSPreviewScrFlag = 1;
	}
	
}
void PhnsetSetPwrOnOffDisplay( void)
{
#ifdef __MMI_FILE_MANAGER__   // zrx add 20060612
	if(phnset_pwr_on_off_strp)	
	{

			PhnsetSetPwrOnOffDisplayByFileName(phnset_pwr_on_off_strp);
	}
	else
	{
		PhnsetSetPwrOnOffDisplayByImgID(g_phnset_cntx_p->SysWPSSList[g_phnset_cntx_p->curHightlightItem]);
	}

	isCorrectSet = 1;

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, 
	SUCCESS_TONE);

	if(IsScreenPresent(SCR_ID_PHNSET_POWER_ON)||IsScreenPresent(SCR_ID_PHNSET_POWER_OFF))
		fmgr_reset_app_select();/*not forword from fmgr*/
	#endif    // zrx add 20060612

}


/*****************************************************************************
* FUNCTION
*  PhnsetSetWallpaperByImgID
* DESCRIPTION
*   This function is to set current wallpaper using index by AT command
** PARAMETERS
*  imgID  	IN       image id
* RETURNS
*  void
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/
void PhnsetSetPwrOnOffDisplayByImgID(U16 imgID )
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	S16 error;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/			  
	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON)
   {
	g_phnset_cntx_p->curtPwrOnID = imgID;

		
	WriteValue(NVRAM_CURRENT_POWER_ON_DISPLAY, &(g_phnset_cntx_p->curtPwrOnID), DS_SHORT, &error);
		
  	}
	else if (g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF)
	{
		g_phnset_cntx_p->curtPwrOffID = imgID;

		WriteValue(NVRAM_CURRENT_POWER_OFF_DISPLAY, &(g_phnset_cntx_p->curtPwrOffID), DS_SHORT, &error);
	}
     	

   if( g_idle_context.IsOnIdleScreen )
      EntryIdleScreen();
}

void PhnsetSetPwrOnOffDisplayByFileName( S8 *path )
{
    	S16 error;
           
	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON)
	{
		if(g_phnset_cntx_p->isShort)
			g_phnset_cntx_p->curtPwrOnID = 1;
		else
			g_phnset_cntx_p->curtPwrOnID = 0;

		WriteValue(NVRAM_CURRENT_POWER_ON_DISPLAY, &(g_phnset_cntx_p->curtPwrOnID), DS_SHORT, &error);
		pfnUnicodeStrcpy((S8 *)phnset_pwronoff_filename.pwrondisplay, (S8*)path);

     mmi_trace(g_sw_WpSsPwon, "gdm:into PhnsetSetPwrOnOffDisplayByImgID");
	}	
 
	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF)
	{
		if(g_phnset_cntx_p->isShort)
			g_phnset_cntx_p->curtPwrOffID = 1;
		else
			g_phnset_cntx_p->curtPwrOffID = 0;
	
		
		WriteValue(NVRAM_CURRENT_POWER_OFF_DISPLAY, &(g_phnset_cntx_p->curtPwrOffID), DS_SHORT, &error);
		pfnUnicodeStrcpy((S8 *)phnset_pwronoff_filename.pwroffdisplay, (S8*)path);
	}
#ifdef __MMI_SAVE_PON_SPEEDUP__ /*gdm modify to add  the switch of __MMI_SAVE_PON_SPEEDUP__  20091125*/
  SetIdlePowerOnName((UI_string_type)phnset_pwronoff_filename.pwrondisplay);
  wgui_save_PowerON_to_speedup();
  mmi_trace(g_sw_WpSsPwon, "gdm:exit PhnsetSetPwrOnOffDisplayByFileName");

  memset(phnset_pwronoff_filename.pwrondisplaylayer, 0, (PHNSET_MAX_FILE_LEN+PHNSET_MAX_PATH_LEN+PHNSET_MAX_EXT_LEN));
  pfnUnicodeStrcpy((S8*)phnset_pwronoff_filename.pwrondisplaylayer, idle_screen_poweron_name);
#endif

	WriteRecord( NVRAM_EF_WPSS_FILENAME_LID, 2, &phnset_pwronoff_filename, sizeof(phnset_pwronoff_filename), &error );

 if( g_idle_context.IsOnIdleScreen )
		EntryIdleScreen();
}

void PhnsetPwrOffDisplayDefaultID(void)
{
	g_phnset_cntx_p->curtPwrOffID=IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1;
}
#ifdef __MMI_FILE_MANAGER__
static FuncPtr phnset_pwronoff = NULL; 
U8 PhnsetPwrOnOffUsrDefPreViewScrDelCB(void *ptr)
{
	PhnsetPwrOnOffUsrDefFreeBuf();

    return MMI_FALSE;
}
void PhnsetPwrOnOffUsrDefPreViewENDKeyHdlr(void)
{
    /* Has to free the file name string buffer  */
    PhnsetPwrOnOffUsrDefFreeBuf();
    
    /* Execute the stored END key handler */
    if(phnset_pwronoff)
    	(*phnset_pwronoff)();
}

void EntryPhnsetPwrOnOffUserView(void)
{
#if defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ )
		U8 		file_type = 0;
		PU8	gui_buffer =NULL;
#endif

		CallbackPWRLSK = PhnsetSetPwrOnOffDisplay;
		isCorrectSet = 0;
#if defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ ) 
		if((file_type = mmi_fmgr_get_file_group(phnset_pwr_on_off_strp)) == FMGR_GROUP_VIDEO)
		{
			if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON)
			{
				EntryNewScreen(SCR_ID_PHNSET_POWER_ON_USER_VIEW, ExitPhnsetPwrOnOffSysView, EntryPhnsetPwrOnOffUserView, NULL);
				gui_buffer = GetCurrGuiBuffer(SCR_ID_PHNSET_POWER_ON_USER_VIEW);
			}
		  	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF)
		  	{
		  		EntryNewScreen(SCR_ID_PHNSET_POWER_OFF_USER_VIEW, ExitPhnsetPwrOnOffSysView, EntryPhnsetPwrOnOffUserView, NULL);
				gui_buffer = GetCurrGuiBuffer(SCR_ID_PHNSET_POWER_OFF_USER_VIEW);
		  	}
			else
			{
				MMI_ASSERT(0);
			}				

			ShowCategory225Screen(STR_GLOBAL_VIEW,		/* title_str */
										IMG_SCR_SETTING_CAPTION,								/* title_icon */
										STR_GLOBAL_OK,				/* lsk_str */
										IMG_GLOBAL_OK,				/* lsk_icon */
										STR_GLOBAL_BACK,			/* rsk_str */
										IMG_GLOBAL_BACK,			/* rsk_icon */
										0,								/* video_id */
										(S8*)phnset_pwr_on_off_strp,			/* video_filename */
										1,								/* repeat_count */
										TRUE,							/* is_visual_update */
										!IsSilentModeActivated(),/* is_play_audio */
										TRUE,							/* is_lcd_no_sleep */
										GDI_COLOR_WHITE,			/* bg_color */
										PhnsetPwrOnOffPlayCallBack,
										gui_buffer);
			
		}
		else if(file_type == FMGR_GROUP_IMAGE)
#endif //defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ ) && !defined(WIN32)
		{
			U8 *guiBuffer;

			if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON)
			{
				guiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_POWER_ON_USER_VIEW);
		   		EntryNewScreen(SCR_ID_PHNSET_POWER_ON_USER_VIEW, ExitPhnsetPwrOnOffSysView, EntryPhnsetPwrOnOffUserView, NULL);
				ShowCategory222Screen (STR_GLOBAL_VIEW,
					GetRootTitleIcon(MENU_SETTING_POWER_ON_DOWNLOAD),
					STR_GLOBAL_OK, IMG_GLOBAL_OK,
					STR_GLOBAL_BACK, IMG_GLOBAL_BACK,  
					GDI_COLOR_WHITE,	NULL, 
					(S8*)phnset_pwr_on_off_strp, 
					FALSE,
					PhnsetPwrOnOffPlayCallBack);
			}
		  	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF)
		  	{
				guiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_POWER_OFF_USER_VIEW);
		  		EntryNewScreen(SCR_ID_PHNSET_POWER_OFF_USER_VIEW, ExitPhnsetPwrOnOffSysView, EntryPhnsetPwrOnOffUserView, NULL);
				ShowCategory222Screen (STR_GLOBAL_VIEW,
					GetRootTitleIcon(MENU_SETTING_POWER_OFF_DOWNLOAD),
					STR_GLOBAL_OK, IMG_GLOBAL_OK,
					STR_GLOBAL_BACK, IMG_GLOBAL_BACK,  
					GDI_COLOR_WHITE,	NULL, 
					(S8*)phnset_pwr_on_off_strp, 
					FALSE,
					PhnsetPwrOnOffPlayCallBack);
				
		  	}



		}		

		SetDelScrnIDCallbackHandler(SCR_ID_PHNSET_POWER_OFF_USER_VIEW, PhnsetPwrOnOffUsrDefPreViewScrDelCB);

		phnset_pwronoff = GetKeyHandler( KEY_END, KEY_EVENT_DOWN );
		SetKeyHandler (PhnsetPwrOnOffUsrDefPreViewENDKeyHdlr, KEY_END, KEY_EVENT_DOWN);

		SetLeftSoftkeyFunction(CallbackPWRLSK,KEY_EVENT_UP);
		SetRightSoftkeyFunction (PhnsetPwrOnOffUsrDefGoBack, KEY_EVENT_UP);

}



/*****************************************************************************
* FUNCTION
*	 PhnsetPwrOnOffSelectFmgrCallBack()
* DESCRIPTION
*   Callback function from filemanager after file selection
* PARAMETERS
*	 path		IN		selected file path
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void PhnsetPwrOnOffSelectFmgrCallBack(void *path, int is_short)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	PS8	error_str_ptr;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_phnset_cntx_p->isShort = (S32)is_short;

   if(path)
   {
	  	/* Prevent memory leck, after buffer is free, set pointer to NULL */
		ASSERT( phnset_pwr_on_off_strp == NULL );

		phnset_pwr_on_off_strp = OslMalloc( FMGR_PATH_BUFFER_SIZE );
		if( phnset_pwr_on_off_strp )
		{
			pfnUnicodeStrcpy((S8 *)phnset_pwr_on_off_strp, (S8*)path);
		}
		else
		{
	  		GoBackToHistory(SCR_IDLE_SCR_DISP);
			return;
		}


		/* file limitation check - limitation is define in file manager util */
		if(!mmi_fmgr_util_file_limit_check(FMGR_LIMIT_POWER_ON_OFF_DSPL, (PS8)phnset_pwr_on_off_strp, &error_str_ptr))
		{
			PhnsetPwrOnOffBadDisplayPopup(error_str_ptr);
			PhnsetPwrOnOffUsrDefFreeBuf();
			return;
		}

		EntryPhnsetPwrOnOffUserView();
		

   }
   else
   {
		GoBackToHistory(g_phnset_cntx_p->ScrAnchor);
   }
}

void PhnsetPwrOnOffForwardFromFmgr(void *path, fmgr_tree_action_enum action, int is_short)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	PS8	error_str_ptr;

	g_phnset_cntx_p->isShort = (S32)is_short;

	if(path)
	{

		/* Prevent memory leck, after buffer is free, set pointer to NULL */
		ASSERT( phnset_pwr_on_off_strp == NULL );

		phnset_pwr_on_off_strp = OslMalloc( FMGR_PATH_BUFFER_SIZE );
		if( phnset_pwr_on_off_strp )
			pfnUnicodeStrcpy((S8 *)phnset_pwr_on_off_strp, (S8*)path);
		else
		{
			GoBackToHistory(SCR_IDLE_SCR_DISP);
			return;
		}

	/* file limitation check - limitation is define in file manager util */
		if(!mmi_fmgr_util_file_limit_check(FMGR_LIMIT_POWER_ON_OFF_DSPL, (PS8)phnset_pwr_on_off_strp, &error_str_ptr))
		{
			PhnsetPwrOnOffBadDisplayPopup(error_str_ptr);
			PhnsetPwrOnOffUsrDefFreeBuf();
			return;
		}

		if(action  == FMGR_ACTION_FWD_POWER_ON)
			g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_PWRON;
		else if (action == FMGR_ACTION_FWD_POWER_OFF)
			g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_PWROFF;

		g_phnset_cntx_p->ScrAnchor = GetCurrScrnId();

		EntryPhnsetPwrOnOffUserView();
   }
   else
		/* run to here means some kinds of image error occur */
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
}




#endif// __MMI_FILE_MANAGER__

#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */
#endif /*POWERONOFFDISPLAY_C*/ 


