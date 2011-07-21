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
 * Wallpaper.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for wallpaper
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

/**************************************************************

	FILENAME	: Wallpaper.c

  	PURPOSE		: Resource IDs for the FunAndGames Application

 

 

	DATE		: 

**************************************************************/
#ifndef _WALLPAPER_C
#define _WALLPAPER_C

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
#include "downloadprotos.h"
#include "commonscreens.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "wgui_categories.h"
#include "wgui_categories_idlescreen.h"
#include "mainmenudef.h"
#include "timerevents.h"
#include "globaldefs.h"
#include "settingprot.h"
#include "wgui_inline_edit.h"
#include "debuginitdef.h"
#include "historygprot.h"
#include "wpssprotos.h"
#include "settinggprots.h"
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
/* MUST be allocated before WallpaperDefs.h */
#include "poweronoffdisplaydefs.h"
#endif
#include "screensaverdefs.h"
#include "wallpaperdefs.h"
#include "settingdefs.h"
#include "settingprofile.h"
#include "downloaddefs.h"
#include "downloadprotos.h"
#include "phonesetup.h"
//#include "filemanagergprot.h"  // zrx del 20060612
 
#ifdef __MMI_THEMES_V2_SUPPORT__
#include "thememanager.h"
#endif
 
/* For user defined file image */
#include "gdi_lcd_config.h"
#include "gdi_datatype.h"
#include "gdi_image.h"
//#include "med_main.h"

#ifdef __MMI_FILE_MANAGER__
/*WUZC Add Start For MMIFS Ver:    on 2007-1-12 11:25 */
#include "filemanagergprot.h"
#include "filesystemdef.h"
/*WUZC Add End  For MMIFS Ver:    on 2007-1-12 11:25 */
#include "filemgr.h"
#include "filemanagerdef.h"
#endif

#include "simdetectiongexdcl.h"
#include "conversions.h"
extern U8 PhnsetGetDefEncodingType(void);

#undef __NEWSIMULATOR

#include "gdi_include.h"  /* gdi_result */
#include "wgui_categories_multimedia.h"  /* showcategory222screen() */
#include "idleappprot.h"

/* Dynamic alloc memory for file path buffer */
S8 *phnset_strp = NULL;  

/* Store END key handler function pointer */
/* When previewing the user define wallpaper & Screen saver */
/* Press END key shall to release file path buffer and execute stored END key handler */
FuncPtr phnset_wpfp = NULL; 

static PS8	error_str_ptr;

/* 
** Define
*/

/************************************************************
  *   This Marco define the acceptable wallpaper/screen saver file format.
  *   Currently, we support .JPG .GIF and .BMP
  ************************************************************/
#define IMG_TYPE_VAILD( x ) ( \
           (x == GDI_IMAGE_TYPE_JPG_FILE) ||  \
           (x == GDI_IMAGE_TYPE_GIF_FILE) || \
           (x == GDI_IMAGE_TYPE_BMP_FILE) )



/***************************************************************
  *   This Marco define the invaild image dimesion for wallpaper/screen saver
  *   Currently, we only stop the error image. We're not yet define the upper bound.
  ***************************************************************/
#define IMG_DIMESION_INVAILD( x, y ) ( \
           ( x < 0 ) || (y <=0 ) \
           )


/***************************************************************
  *   This Value define the acceptable file size for wallpaper / screen saver
  *   Prevent from long decoding time 
  ***************************************************************/
#ifndef __MMI_WPSS_IMG_SIZE_LIM__
#define __MMI_WPSS_IMG_SIZE_LIM__ 10
#endif

#ifndef __MMI_WPSS_JPG_SIZE_LIM__
#define __MMI_WPSS_JPG_SIZE_LIM__ 150
#endif


#define IMG_SIZE_BOUNDARY  (__MMI_WPSS_IMG_SIZE_LIM__ * 1024)  // default 10 K

#define JPG_SIZE_BOUNDARY  (__MMI_WPSS_JPG_SIZE_LIM__ *1024)  // default 150 K

/* 
** Typedef 
*/

/* 
** Local Variable
*/

/* 
** Global Variable
*/
PHNSET_WPSS_FILENAME_STRUCT phnset_wpss_filename;
S8 *idle_screen_wallpaper_name = (S8 *)phnset_wpss_filename.wallpaper;
S8 *idle_screen_wallpaper_name_original = (S8 *)phnset_wpss_filename.wallpaper;
UI_image_ID_type  idle_screen_wallpaper_ID;

#ifdef __MMI_SAVE_WP_SPEEDUP__/*gdm modify to add  the switch __MMI_SAVE_WP_SPEEDUP__  091125*/
const U16 PHNSET_PREDEFINED_WP_PATH_JPG[]= {0X65, 0X3a, 0X5c, 0X55, 0X73, 0X65, 0X72, 0X57, 0X50, 0X2e, 0X6a, 0X70, 0X67, 0X00, 0X00, 0X00, };// "c:\\UserWP.jpg\0";  ; // zrx add 20060612
const U16 PHNSET_PREDEFINED_WP_PATH_PBM[]={0X65, 0X3a, 0X5c, 0X55, 0X73, 0X65, 0X72, 0X57, 0X50, 0X2e, 0X50, 0X42, 0X4d, 0X00, 0X00, 0X00, };// "c:\\UserWP.PBM\0";   // zrx del 20060612 // zrx add 20060612
const U16 PHNSET_PREDEFINED_WP_PATH_GIF[]= {0X65, 0X3a, 0X5c, 0X55, 0X73, 0X65, 0X72, 0X57, 0X50, 0X2e, 0X67, 0X69, 0X66, 0X00, 0X00, 0X00, };// "c:\\UserWP.gif\0"; 
#endif

#ifdef __MMI_SUB_WALLPAPER__
PHNSET_WPSS_FILENAME_STRUCT phnset_wpss_sub_filename;
S8	*idle_screen_wallpaper_sub_name = (S8* )phnset_wpss_sub_filename.wallpaper;  
#endif

U8 isCorrectSet;
/* 
** Global Function
*/
extern void SetIdleScreenWallpaperID(U16 image_ID);
#ifdef __MMI_SAVE_WP_SPEEDUP__/*gdm modify to add  the switch __MMI_SAVE_WP_SPEEDUP__  091125*/
extern void SetIdleScreenWallpaperName(UI_string_type name);
extern void wgui_save_wallpaper_to_speedup(void);
#endif
static void DispIdleScrWPBadFilePopup(void);
 
static FuncPtr CallbackWpSsLSK = NULL; 
#ifdef __MMI_SUB_WALLPAPER__
extern void SUBLCD_SetIdleScreenWallpaperID(U16 image_ID);
extern void SUBLCD_SetIdleScreenWallpaperName(UI_string_type name);   
extern void SUBLCD_UpdateWallpaper(void);
#endif
void PhnsetWpUsrDefPreView(void);

#include "mmi_trace.h"

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetWallpaper
* DESCRIPTION
*   This function is highlight handler for Setting->Phonesetup->IdleScreenDisplay->Wallpaper.
*   Following Functions:   EntryPhnsetWallpaperFixRotate
*                                   EntryPhnsetWallpaperSel
*                                   EntryPhnsetWallpaperSys
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   IdleScreenDisplay
*****************************************************************************/
void HighlightPhnsetWallpaper(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

#if (defined (__DAILY_WALLPAPER__) )
      /* Go to menu with 2 items: Fix, Rotate */
     	SetKeyHandler(EntryPhnsetWallpaperFixRotate, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
     	SetLeftSoftkeyFunction(EntryPhnsetWallpaperFixRotate,KEY_EVENT_UP);

#elif (defined (__MMI_WAP__) || (defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__) ) ) 
	//#ifdef __MMI_SUB_WALLPAPER__
	//	if(WALLPAPER_TYPE == WALLPAPER_SUB)
	//	{
			/* Direct enter system wallpaper list menu */
			SetLeftSoftkeyFunction(EntryPhnsetWallpaperSys,KEY_EVENT_UP);
			SetKeyHandler(EntryPhnsetWallpaperSys, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
			g_phnset_cntx_p->ScrAnchor = SCR_IDLE_SCR_DISP;//SCR_WALLPAPER_SYSTEM_LIST;
	//	}
	//	else
	//	{
	//#endif
	      /* Go to menu with 2 items: System, Download */
		SetLeftSoftkeyFunction(EntryPhnsetWallpaperSel,KEY_EVENT_UP);
		SetKeyHandler(EntryPhnsetWallpaperSel, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		g_phnset_cntx_p->ScrAnchor = SCR_MAIN_WALLPAPER;
	//#ifdef __MMI_SUB_WALLPAPER__
	//	}
	//#endif
#else
      /* Direct enter system wallpaper list menu */
	SetLeftSoftkeyFunction(EntryPhnsetWallpaperSys,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetWallpaperSys, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	g_phnset_cntx_p->ScrAnchor = SCR_WALLPAPER_SYSTEM_LIST;

#endif

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetMainWallpaper
* DESCRIPTION
*   This function is highlight handler for Setting->Phonesetup->IdleScreenDisplay->Wallpaper.
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   IdleScreenDisplay
*****************************************************************************/

void HighlightPhnsetMainWallpaper(void)
{
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_WP;
	HighlightPhnsetWallpaper();
}
#ifdef __MMI_SUB_WALLPAPER__

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetMainWallpaper
* DESCRIPTION
*   This function is highlight handler for Setting->Phonesetup->IdleScreenDisplay->sublcd Wallpaper.
*     
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   IdleScreenDisplay
*****************************************************************************/
void HighlightPhnsetSubWallpaper(void)
{
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_SUB_WP;
	HighlightPhnsetWallpaper();
}
#endif

#ifdef __DAILY_WALLPAPER__
/*****************************************************************************
* FUNCTION
*  HighlightPhnsetWallpaperFix
* DESCRIPTION
*   This function is highlight handler for Setting->Phonesetup->IdleScreenDisplay->Wallpaper->Fix
*   Following Functions:   EntryPhnsetWallpaperSel
*                                   EntryPhnsetWallpaperSys
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   IdleScreenDisplay
*****************************************************************************/
void HighlightPhnsetWallpaperFix(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

#if( defined(__MMI_WAP__) || defined(__MMI_FILE_MANAGER__) )

      /* Go to menu with 2 items: System, Download */
	SetKeyHandler(EntryPhnsetWallpaperSel, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryPhnsetWallpaperSel,KEY_EVENT_UP);
	g_phnset_cntx_p->ScrAnchor = SCR_MAIN_WALLPAPER_FIX;

#else

      /* Direct go to system wallpaper list */
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryPhnsetWallpaperSys,KEY_EVENT_UP);
	g_phnset_cntx_p->ScrAnchor = SCR_WALLPAPER_SYSTEM_LIST;

#endif

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetWallpaperRotate
* DESCRIPTION
*   This function is highlight handler for Setting->Phonesetup->IdleScreenDisplay->Wallpaper->Rotate
*   Following Functions:   PhnsetWallpaperRotateLSK
*                                   
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   Null
*****************************************************************************/
void HighlightPhnsetWallpaperRotate(void)
{
	if( PhnsetGetWallpaperRotateStatus() == MMI_TRUE )
   	   ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	else
   	   ChangeLeftSoftkey(STR_GLOBAL_ON ,0);

	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetLeftSoftkeyFunction(PhnsetWallpaperRotateLSK,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler( NULL, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  PhnsetWallpaperRotateLSK
* DESCRIPTION
*   This function is Left softkey handler for Setting->Phonesetup->IdleScreenDisplay->Wallpaper->Rotate
*   Functionality: Toggle Rotate Status
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   hintData
*****************************************************************************/
void PhnsetWallpaperRotateLSK(void)
{
    S16 error;
    U8 status;
    U16 index = GetCurrHiliteID();

    if( PhnsetGetWallpaperRotateStatus() == MMI_TRUE ) /* Toggle */
    {
       status = MMI_FALSE;
 	  ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	  pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
    }
    else
    {
       status = MMI_TRUE;
  	  ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	  pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
     }
    
    WriteValue(NVRAM_WALLPAPER_ROTATE_STATUS,&status,DS_BYTE,&error);
    Category52ChangeItemDescription(index,hintData[index]);
    RedrawCategoryFunction();    
    
}


/*****************************************************************************
* FUNCTION
*  HintPhnsetWallpaperRotate
* DESCRIPTION
*   This function is Hint handler for Wallpaper Rotate status
*   Functionality: Show hint for rotate status
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   hintData
*****************************************************************************/
void HintPhnsetWallpaperRotate(U16 index)
{
     if( PhnsetGetWallpaperRotateStatus() == MMI_TRUE )
   	   pfnUnicodeStrcpy( (PS8)hintData[index], (PS8) GetString(STR_GLOBAL_ON) );
     else
   	   pfnUnicodeStrcpy( (PS8)hintData[index], (PS8) GetString(STR_GLOBAL_OFF) );
}



/*****************************************************************************
* FUNCTION
*  PhnsetGetWallpaperRotateStatus
* DESCRIPTION
*   This function is to query rotate status
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*   T/F
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
BOOL PhnsetGetWallpaperRotateStatus( void )
{
     U8 status = 0x0;
     S16 error;
     
     ReadValue(NVRAM_WALLPAPER_ROTATE_STATUS, (void *)&status,DS_BYTE,&error);

     if( status == 0x0 || error != NVRAM_READ_SUCCESS )
      return MMI_FALSE;
     else
      return MMI_TRUE;
}


/*****************************************************************************
* FUNCTION
*  EntryPhnsetWallpaperFixRotate
* DESCRIPTION
*   This function is Entry function for Setting->Phonesetup->IdleScreenDisplay->Wallpaper 
*    when __DAILY_WALLPAPER__ is ON
*   Functionality: Display "Fix/Rotate" Screen
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void EntryPhnsetWallpaperFixRotate(void)
{
	U8 *guiBuffer;
	U16 nNumofItem;
	U16 nDispAttribute;  /* Stores display attribue */
	U8 *IdleScrDispType[MAX_SUB_MENUS];
	U16 WallpaperSelectList[MAX_SUB_MENUS]; //= { STR_WALLPAPER_SYSTEM, STR_WALLPAPER_DOWNLOAD };

	EntryNewScreen(SCR_MAIN_WALLPAPER_FIX, NULL, EntryPhnsetWallpaperFixRotate, NULL);

	SetHiliteHandler(MENU_SETTING_WALLPAPER_FIX,   HighlightPhnsetWallpaperFix);
	SetHiliteHandler(MENU_SETTING_WALLPAPER_ROTATE, HighlightPhnsetWallpaperRotate);

	guiBuffer = GetCurrGuiBuffer(SCR_MAIN_WALLPAPER_FIX);
	nNumofItem = GetNumOfChild(MENU_SETTING_WALLPAPER);
	nDispAttribute = GetDispAttributeOfItem(MENU_SETTING_WALLPAPER);
	GetSequenceStringIds(MENU_SETTING_WALLPAPER,WallpaperSelectList);
	SetParentHandler(MENU_SETTING_WALLPAPER);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	ConstructHintsList(MENU_SETTING_WALLPAPER, IdleScrDispType);


	ShowCategory52Screen(  STR_WALLPAPER_CAPTION, IMG_SCR_SETTING_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,WallpaperSelectList,(PU16)gIndexIconsImageList,
						  IdleScrDispType,
						  1,0,guiBuffer);


	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}




/*****************************************************************************
* FUNCTION
*  PhnsetWallPaperDailyChange
* DESCRIPTION
*   This function is to change wallpaper to next one in system folder
*   Functionality: ex: Referenced by DateTimerIndication
*
* PARAMETERS
*  a  IN/OUT      
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/
void PhnsetWallPaperDailyChange(void)
{
    U8 curWallpaperId;
    
    curWallpaperId=PhnsetGetCurWallPaperIdx();
    if( curWallpaperId == 0xFF )
    {
		#ifdef __MMI_SUB_WALLPAPER__
		g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_WP;
		#endif

         PhnsetRstWallpaperDefault();
         if( g_idle_context.IsOnIdleScreen )
      	   EntryIdleScreen();
         
         return;
    }

    if( (curWallpaperId+1) > PhnsetGetWallpaperIdxBound() )
      curWallpaperId = 0;
    else
      curWallpaperId++;

    PhnsetSetDisplayFromAT( curWallpaperId , MMI_PHNSET_TYPE_WP );
}


/*****************************************************************************
* FUNCTION
*  PhnsetGetCurWallPaperIdx
* DESCRIPTION
*   This function is to get the current wallpaper index in gSystemWallpaperList 
*   Functionality: ex: Referenced by PhnsetWallPaperDailyChange
*
* PARAMETERS
*  a  IN/OUT      
*  b  IN       
* RETURNS
*  U8 index
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/
U8 PhnsetGetCurWallPaperIdx(void)
{
    U8 idx=0;
    U16 SysWpIDList[MAX_WALLPAPER_NUMBERS];
    
   PhnsetConstSysWallpaperList( NULL, SysWpIDList, NULL );

    for( idx=0; idx < (MAX_SYSTEM_WALLPAPER + MAXIMUM_FULLSCREEN_FACTORY_IMAGES); idx++ ) 
    {
         if( g_phnset_cntx_p->curtWpID == SysWpIDList[idx] )
            break;
    }
    
    if( idx >= (MAX_SYSTEM_WALLPAPER + MAXIMUM_FULLSCREEN_FACTORY_IMAGES) )
      idx = 0xFF; /* Not Found! */

    return idx;
}



#endif /* __DAILY_WALLPAPER__ */






/*****************************************************************************
* FUNCTION
*  PhnsetConstSysWallpaperList
* DESCRIPTION
*   This function is to construct wallpaper system folder list,  
*       for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) ->System
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*
*   gSystemWallpaperList : IMG ID List
*   WallpaperSystemList : STR pointer list
*   gCurrentWallpaper : Current wapper IMG ID
*   gHighlightSystemWallpaper
*   noFactoryImages : Number of factory download image
*
*****************************************************************************/
U8 PhnsetConstSysWallpaperList( U8 **pSysWallpaperStrList, U16 *pSysWallpaperImgIDList, S16 *HighliteItem )
{
//	DYNIMAGEINFO *Info;
	U8 i = 0;
	U16 strID = 0;
	U8 wp_cnt = MAX_SYSTEM_WALLPAPER;
 
#ifdef __MMI_THEMES_V2_SUPPORT__
	pBOOL result = MMI_FALSE;
#endif
 
/*********SYSTEM WALLPAPER**************/	
	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)
	{
		for( i=0; i<MAX_SYSTEM_WALLPAPER ; i++ )
		{
			strID = STR_WALLPAPER_1 + i;

			if( pSysWallpaperImgIDList ) 
		   		pSysWallpaperImgIDList[i] = WALLPAPER_1 + i;
		
			if( pSysWallpaperStrList ) 
		   		pSysWallpaperStrList[i] = (PU8)GetString(strID);
		
			if( HighliteItem )
			{
		   		if( g_phnset_cntx_p->curtWpID == pSysWallpaperImgIDList[i] )
					*HighliteItem = i;
			}
		}
	}
#ifdef  __MMI_SUB_WALLPAPER__
	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SUB_WP)
	{
		for( i=0; i<MAX_SYSTEM_SUB_WALLPAPER ; i++ )
		{
			strID = STR_SUB_WALLPAPER_1 + i;

			if( pSysWallpaperImgIDList ) 
				pSysWallpaperImgIDList[i] = SUB_WALLPAPER_1 + i;
		
			if( pSysWallpaperStrList ) 
				pSysWallpaperStrList[i] = (PU8)GetString(strID);
			
			if( HighliteItem )
				if( g_phnset_cntx_p->curtSubWpID == pSysWallpaperImgIDList[i] )
					*HighliteItem = i;
		   
		}
	}
#endif
	else
		MMI_ASSERT(0);

/*********FACTORY WALLPAPER*************/
/*    g_phnset_cntx_p->WPFctyImgNum = GetFullScreenFactoryImagesInfo(&Info);

	for(  i=0; i<g_phnset_cntx_p->WPFctyImgNum; i++ )
	{
		strID = STR_WALLPAPER_1 + MAX_SYSTEM_WALLPAPER + i;

		if( pSysWallpaperImgIDList ) 
		   pSysWallpaperImgIDList[MAX_SYSTEM_WALLPAPER + i] = Info[i].ImagesId;

		if( pSysWallpaperStrList ) 
              pSysWallpaperStrList[MAX_SYSTEM_WALLPAPER + i] = (PU8)GetString(strID);

		if( HighliteItem )
   		   if( g_phnset_cntx_p->curtWpID == Info[i].ImagesId )
			   *HighliteItem = MAX_SYSTEM_WALLPAPER + i;
	}

    wp_cnt+=g_phnset_cntx_p->WPFctyImgNum;
*/ //modified by lin for factory wallpaper @2007/04/16
 
#ifndef __MMI_UI_TECHNO_IDLESCREEN_BAR__
 
/*********ANALOG CLOCK WALLPAPER*************/

#ifdef __MMI_ANALOG_CLOCK__
	#ifdef __MMI_SUB_WALLPAPER__
	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)
	#endif
	{
	      if( pSysWallpaperImgIDList ) 
	         pSysWallpaperImgIDList[MAX_SYSTEM_WALLPAPER + i] = ANALOG_CLOCK_WALLPAPER;
      
	      if( pSysWallpaperStrList ) 
	         pSysWallpaperStrList[MAX_SYSTEM_WALLPAPER + i] = (PU8)GetString(STR_WALLPAPER_ANALOG);

		if( HighliteItem )
	         if( g_phnset_cntx_p->curtWpID == ANALOG_CLOCK_WALLPAPER )
            *HighliteItem = MAX_SYSTEM_WALLPAPER + i;
	
		i++;
		wp_cnt++;
	}
#endif
	
/********* DIGITAL CLOCK WALLPAPER*************/
#ifdef __MMI_DIGITAL_CLOCK__	
#if !defined(__MMI_MAINLCD_220X176__)
	#ifdef __MMI_SUB_WALLPAPER__
	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)
	#endif
	{
		pSysWallpaperImgIDList[MAX_SYSTEM_WALLPAPER + i] = DIGITAL_CLOCK_WALLPAPER;
		pSysWallpaperStrList[MAX_SYSTEM_WALLPAPER + i] = (PU8)GetString(STR_WALLPAPER_DIGITAL);

		if( HighliteItem )
		   if( g_phnset_cntx_p->curtWpID == DIGITAL_CLOCK_WALLPAPER )
			*HighliteItem = MAX_SYSTEM_WALLPAPER + i;

	    wp_cnt++;
	}
#endif	
#endif
 
#endif	/* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */
 
/**************LIST COMPLETED HERE*********************/

#ifdef __MMI_THEMES_V2_SUPPORT__

	#ifdef __MMI_SUB_WALLPAPER__
	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)
	#endif
	{
		result=tm_get_current_theme_wallapaper();
		if(result==TRUE)
		{
			pSysWallpaperImgIDList[MAX_SYSTEM_WALLPAPER + i] = CURRENT_THEME_WALLPAPER;
			pSysWallpaperStrList[MAX_SYSTEM_WALLPAPER + i] = (PU8)GetString(STR_CURR_WALLPAPER);
			if( HighliteItem )
			   if( g_phnset_cntx_p->curtWpID == CURRENT_THEME_WALLPAPER )
				*HighliteItem = MAX_SYSTEM_WALLPAPER + i;
	    wp_cnt++;
		}
	}
#endif

    return (wp_cnt);
	
}




/*****************************************************************************
* FUNCTION
*  EntryPhnsetWallpaperSys
* DESCRIPTION
*   This function is to entry function for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) ->System
*   Functionality: Display system wallpaper list
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*
*   gHighlightSystemWallpaper
*   gDontAddHistoryOnScroll : Prevent add history When scrolling in preview screen
*   WallpaperSystemList
*   noFactoryImages
*
*****************************************************************************/
void EntryPhnsetWallpaperSys(void)
{
	U8 *guiBuffer;
	U8 item_amount;
	
	EntryNewScreen(SCR_WALLPAPER_SYSTEM_LIST, NULL, EntryPhnsetWallpaperSys, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_WALLPAPER_SYSTEM_LIST);

	RegisterHighlightHandler(PhnsetWallpaperSysHiliteHandler);

	g_phnset_cntx_p->curHightlightItem = 0;
	item_amount = PhnsetConstSysWallpaperList(subMenuDataPtrs, g_phnset_cntx_p->SysWPSSList , (S16 *)&(g_phnset_cntx_p->curHightlightItem));

	g_phnset_cntx_p->WPSSPreviewScrFlag = 1;
/*
	ShowCategory36Screen(STR_WALLPAPER_SELECT_CAPTION,IMG_SCR_SETTING_CAPTION,STR_GLOBAL_VIEW, 0,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							MAX_SYSTEM_WALLPAPER+noFactoryImages+MAX_OTHER_WALLPAPER, WallpaperSystemList, gHighlightSystemWallpaper, guiBuffer);
*/
#ifdef __MMI_THEMES_V2_SUPPORT__	
	if(tm_get_current_theme_wallapaper())
	ShowCategory36Screen(STR_WALLPAPER_SELECT_CAPTION,IMG_SCR_SETTING_CAPTION,STR_GLOBAL_VIEW, 0,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						#ifdef __MMI_SUB_WALLPAPER__
							((g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)?(MAX_SYSTEM_WALLPAPER+g_phnset_cntx_p->WPFctyImgNum+MAX_OTHER_WALLPAPER+1)
							: MAX_SYSTEM_SUB_WALLPAPER),
						#else
							(MAX_SYSTEM_WALLPAPER+g_phnset_cntx_p->WPFctyImgNum+MAX_OTHER_WALLPAPER+1), 
						#endif
							subMenuDataPtrs, 
						#ifdef __MMI_SUB_WALLPAPER__
								((g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)?((U16)(g_phnset_cntx_p->curHightlightItem))
								:(g_phnset_cntx_p->curtSubWpID-SUB_WALLPAPER_1)),
						#else
								(U16)(g_phnset_cntx_p->curHightlightItem),
						#endif
							guiBuffer);
   else
#endif
	ShowCategory36Screen(STR_WALLPAPER_SELECT_CAPTION,IMG_SCR_SETTING_CAPTION,STR_GLOBAL_VIEW, 0,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							item_amount, 
							subMenuDataPtrs, 
							(U16)(g_phnset_cntx_p->curHightlightItem),
							guiBuffer);

	SetLeftSoftkeyFunction(EntryPhnsetWallpaperSysView,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetWallpaperSysView,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	
}





/*****************************************************************************
* FUNCTION
*  ExitWallpaperSystem
* DESCRIPTION
*   This function is Highlight hanlder when scrolling at wallpaper list screen
*         for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) ->System
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetWallpaperSysHiliteHandler( S32 nIndex)
{
	g_phnset_cntx_p->curHightlightItem = (U16)nIndex;
}



/*****************************************************************************
* FUNCTION
*  EntryPhnsetWallpaperSysView
* DESCRIPTION
*   This function is Entry function to preview wallpaper
*         for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) ->System/Download -> View
*   Follow functions: PhnsetWallpaperSysViewPre
*                            PhnsetWallpaperSysViewNext
*                            PhnsetWallpaperSet
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gSystemWallpaperList
*     gHighlightSystemWallpaper
*     gDontAddHistoryOnScroll
*****************************************************************************/
void EntryPhnsetWallpaperSysView(void)
		{	
	PU8 wallpaperName;
	U8 *guiBuffer;
	
	U16 wallpaperId = g_phnset_cntx_p->SysWPSSList[g_phnset_cntx_p->curHightlightItem];

	guiBuffer = GetCurrGuiBuffer(SCR_WALLPAPER_VIEW);

	isCorrectSet = 0;
		
	if(g_phnset_cntx_p->WPSSPreviewScrFlag)
	{
	   //ExecuteCurrExitHandler();
	   EntryNewScreen(SCR_WALLPAPER_VIEW, ExitPhnsetWallpaperSysView, EntryPhnsetWallpaperSysView, NULL);
	}

	wallpaperName = (PU8)PhnsetGetWallpaperNameById(wallpaperId);

#if defined(__MMI_DRAW_MANAGER__)
	wgui_set_touch_title_bar_buttons();//enable the display of title bar buttons
#endif

#if defined(__MMI_TOUCH_SCREEN__)
	register_touch_button_handlers(NULL, PhnsetWallpaperSysViewPre,
			NULL, PhnsetWallpaperSysViewNext);
#endif

#ifdef __MMI_ANALOG_CLOCK__
	/* Category 86 if to View the Analog Clock*/
	if((wallpaperId == ANALOG_CLOCK_WALLPAPER))
	{
	#if defined(__MMI_DRAW_MANAGER__)
		ShowCategory86Screen(STR_WALLPAPER_ANALOG, 0, STR_GLOBAL_OK,
				IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				ANALOG_CLOCK_WALLPAPER, NULL, guiBuffer, 0);
	#else
		ShowCategory86Screen(STR_WALLPAPER_ANALOG,IMG_SCR_SETTING_CAPTION,STR_GLOBAL_OK, IMG_GLOBAL_OK,STR_GLOBAL_BACK,
								IMG_GLOBAL_BACK,  ANALOG_CLOCK_WALLPAPER,NULL, guiBuffer,0); 
	#endif
	}
	else
#endif

#ifdef __MMI_DIGITAL_CLOCK__	
	/* Category 128 to view Digital Clock*/
	if(wallpaperId == DIGITAL_CLOCK_WALLPAPER)
	{
	#if defined(__MMI_DRAW_MANAGER__)
		ShowCategory128Screen(STR_WALLPAPER_DIGITAL, 0, STR_GLOBAL_OK,
				IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				DIGITAL_CLOCK_WALLPAPER, 1, 1, 1, guiBuffer, 0);
	#else
		ShowCategory128Screen(STR_WALLPAPER_DIGITAL,IMG_SCR_SETTING_CAPTION,STR_GLOBAL_OK, IMG_GLOBAL_OK,STR_GLOBAL_BACK,
						IMG_GLOBAL_BACK,DIGITAL_CLOCK_WALLPAPER, 1,1,1, guiBuffer,0);
	#endif
	}
	else
#endif
#ifdef __MMI_THEMES_V2_SUPPORT__
	if(wallpaperId == CURRENT_THEME_WALLPAPER)
	{
	#if defined(__MMI_DRAW_MANAGER__)
		ShowCategory129Screen(wallpaperName,0,STR_GLOBAL_OK, IMG_GLOBAL_OK,STR_GLOBAL_BACK,
			IMG_GLOBAL_BACK,CURRENT_THEME_WALLPAPER, guiBuffer);
	#else
		ShowCategory129Screen(wallpaperName,IMG_SCR_SETTING_CAPTION,STR_GLOBAL_OK, IMG_GLOBAL_OK,STR_GLOBAL_BACK,
			IMG_GLOBAL_BACK,CURRENT_THEME_WALLPAPER, guiBuffer);
	#endif
	}
	else
#endif
	/*For Normal Viewing*/
	{
	#if defined(__MMI_DRAW_MANAGER__)
		ShowCategory129Screen(wallpaperName, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK, wallpaperId, guiBuffer);
	#else
		ShowCategory129Screen(wallpaperName, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK, wallpaperId, guiBuffer);
	#endif
	}

	SetLeftSoftkeyFunction(PhnsetWallpaperSet,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#ifdef    __MMI_NOKIA_STYLE_N800__ 
        ClearKeyHandler( KEY_LSK,  KEY_EVENT_DOWN);
        ClearKeyHandler( KEY_RSK,  KEY_EVENT_DOWN);

        SetKeyHandler(PhnsetWallpaperSet , KEY_LSK,  KEY_EVENT_UP);
        SetKeyHandler(GoBackHistory , KEY_RSK,  KEY_EVENT_UP);
#endif

	/* change the key handler for KEY UP event to KEY DOWN event.*/
	SetKeyHandler(PhnsetWallpaperSysViewPre, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(PhnsetWallpaperSysViewNext, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  ExitPhnsetWallpaperSysView
* DESCRIPTION
*   This function is Exit function for preview wallpaper
*         for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) ->System/Download -> View
*   Functionality: Add to history
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gDontAddHistoryOnScroll
*****************************************************************************/
void ExitPhnsetWallpaperSysView(void)
{
	if(isCorrectSet)
		DeleteUptoScrID(g_phnset_cntx_p->ScrAnchor);
	
	g_phnset_cntx_p->WPSSPreviewScrFlag=1;
}
/*****************************************************************************
* FUNCTION
*  PhnsetWallpaperSysViewPre
* DESCRIPTION
*   This function is to view previous wallpaper
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gHighlightSystemWallpaper
*     gDontAddHistoryOnScroll
*****************************************************************************/
void PhnsetWallpaperSysViewPre(void)
{
	ExitCategoryFunction();
	if(g_phnset_cntx_p->curHightlightItem <= 0 )
	#ifdef __MMI_SUB_WALLPAPER__
	{
		if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)
		{
		#ifdef __MMI_THEMES_V2_SUPPORT__
			if(tm_get_current_theme_wallapaper())
			{
				g_phnset_cntx_p->curHightlightItem = g_phnset_cntx_p->WPFctyImgNum+MAX_SYSTEM_WALLPAPER+MAX_OTHER_WALLPAPER+1;
			}
			else
		#endif
			{
				g_phnset_cntx_p->curHightlightItem = g_phnset_cntx_p->WPFctyImgNum+MAX_SYSTEM_WALLPAPER+MAX_OTHER_WALLPAPER;
			}
		}
		else
			g_phnset_cntx_p->curHightlightItem = MAX_SYSTEM_SUB_WALLPAPER;
	}
	#else
	{
	#ifdef __MMI_THEMES_V2_SUPPORT__
		if(tm_get_current_theme_wallapaper())
		{
			g_phnset_cntx_p->curHightlightItem = g_phnset_cntx_p->WPFctyImgNum+MAX_SYSTEM_WALLPAPER+MAX_OTHER_WALLPAPER+1;
		}
		else
	#endif
		{
			g_phnset_cntx_p->curHightlightItem = g_phnset_cntx_p->WPFctyImgNum+MAX_SYSTEM_WALLPAPER+MAX_OTHER_WALLPAPER;
		}
	}
	#endif
	
	g_phnset_cntx_p->curHightlightItem--;
	g_phnset_cntx_p->WPSSPreviewScrFlag = 0;
	EntryPhnsetWallpaperSysView();
}



/*****************************************************************************
* FUNCTION
*  PhnsetWallpaperSysViewNext
* DESCRIPTION
*   This function is to view next wallpaper
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gHighlightSystemWallpaper
*     gDontAddHistoryOnScroll
*****************************************************************************/
void PhnsetWallpaperSysViewNext(void)
{
	ExitCategoryFunction();
	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)
	{
	#ifdef __MMI_THEMES_V2_SUPPORT__
		if(tm_get_current_theme_wallapaper())
		{
			if(g_phnset_cntx_p->curHightlightItem >= (g_phnset_cntx_p->WPFctyImgNum+MAX_SYSTEM_WALLPAPER+MAX_OTHER_WALLPAPER + 1) - 1)
				g_phnset_cntx_p->curHightlightItem = -1;
		}
		else
	#endif
		{
			if(g_phnset_cntx_p->curHightlightItem >= g_phnset_cntx_p->WPFctyImgNum+MAX_SYSTEM_WALLPAPER+MAX_OTHER_WALLPAPER-1 )
				g_phnset_cntx_p->curHightlightItem = -1;
		}
	}
#ifdef __MMI_SUB_WALLPAPER__
	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SUB_WP)
	{
		if(g_phnset_cntx_p->curHightlightItem >= MAX_SYSTEM_SUB_WALLPAPER-1)
			g_phnset_cntx_p->curHightlightItem = -1;
	}
#endif
	else
		MMI_ASSERT(0);
	g_phnset_cntx_p->curHightlightItem++;
	g_phnset_cntx_p->WPSSPreviewScrFlag = 0;
	EntryPhnsetWallpaperSysView();
}




/*****************************************************************************
* FUNCTION
*  PhnsetWallpaperSet
* DESCRIPTION
*   This function is Left softkey handler for set/preview wallpaper
*   Functionality: Save to nvram, change globol variable gCurrentWallpaper
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gCurrentWallpaper
*     gSystemWallpaperList
*     gHighlightSystemWallpaper
*     IdleScreenDisplay
*****************************************************************************/
void PhnsetWallpaperSet(void)
{
	S16 error;
	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)
	{
		g_phnset_cntx_p->curtWpID = g_phnset_cntx_p->SysWPSSList[g_phnset_cntx_p->curHightlightItem];
		WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
		SetIdleScreenWallpaperID(g_phnset_cntx_p->curtWpID);
	    #ifdef __MMI_WALLPAPER_ON_BOTTOM__
	        wgui_set_wallpaper_change();     
	    #endif 	
	}
#ifdef __MMI_SUB_WALLPAPER__
	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SUB_WP)
	{
		g_phnset_cntx_p->curtSubWpID = g_phnset_cntx_p->SysWPSSList[g_phnset_cntx_p->curHightlightItem];
		WriteValue(NVRAM_SETWALLPAPER_SUB, &(g_phnset_cntx_p->curtSubWpID), DS_SHORT, &error);
		SUBLCD_SetIdleScreenWallpaperID(g_phnset_cntx_p->curtSubWpID);
		SUBLCD_UpdateWallpaper();
	}
#endif	
	else
		MMI_ASSERT(0);

	isCorrectSet = 1;

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
}

 
void ThemeManagerSetWallpaper(U16 nId)
{
	S16 error;
	g_phnset_cntx_p->curtWpID = nId;
	WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
	SetIdleScreenWallpaperID(nId);
}
 

#if (defined(__MMI_WAP__) || defined(__MMI_FILE_MANAGER__) )

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetWallpaperSys
* DESCRIPTION
*   This function is Highlight handler for Setting->Phonesetup->IdleScreenDisplay->Wallpaper->System 
*   Following Function: EntryPhnsetWallpaperSys
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void HighlightPhnsetWallpaperSys(void)
{
	SetLeftSoftkeyFunction(EntryPhnsetWallpaperSys,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetWallpaperSys, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  EntryPhnsetWallpaperSel
* DESCRIPTION
*   This function is Entry function for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix)
*   Following Function: Show (System/Download) Menu
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   gIndexIconsImageList
*****************************************************************************/
void EntryPhnsetWallpaperSel(void)
{
	U8 *guiBuffer;
	U16 nNumofItem;
	U16 WallpaperSelectList[MAX_SUB_MENUS]; //= { STR_WALLPAPER_SYSTEM, STR_WALLPAPER_DOWNLOAD };

   EntryNewScreen(SCR_MAIN_WALLPAPER, NULL, EntryPhnsetWallpaperSel, NULL);
	g_phnset_cntx_p->ScrAnchor = SCR_MAIN_WALLPAPER;

	guiBuffer = GetCurrGuiBuffer(SCR_MAIN_WALLPAPER);

#ifdef __DAILY_WALLPAPER__
     nNumofItem = GetNumOfChild(MENU_SETTING_WALLPAPER_FIX );
	GetSequenceStringIds(MENU_SETTING_WALLPAPER_FIX,WallpaperSelectList);
	SetParentHandler(MENU_SETTING_WALLPAPER_FIX);
#else
	nNumofItem = GetNumOfChild(MENU_SETTING_WALLPAPER);
	GetSequenceStringIds(MENU_SETTING_WALLPAPER,WallpaperSelectList);
	SetParentHandler(MENU_SETTING_WALLPAPER);
#endif

	SetHiliteHandler(MENU_SETTING_WALLPAPER_SYSTEM,   HighlightPhnsetWallpaperSys);
	
#if defined(__MMI_FILE_MANAGER__)
	SetHiliteHandler(MENU_SETTING_WALLPAPER_DOWNLOAD, HighlightPhnsetWallpaperFmgr);
#elif defined( __MMI_WAP__ )
	SetHiliteHandler(MENU_SETTING_WALLPAPER_DOWNLOAD, HighlightPhnsetWallpaperDwnl);
#endif
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory15Screen(	STR_WALLPAPER_CAPTION, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, nNumofItem,
							WallpaperSelectList, (PU16)gIndexIconsImageList,  1, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
#endif


#ifdef __MMI_FILE_MANAGER__
/*****************************************************************************
* FUNCTION
*  HighlightPhnsetWallpaperFmgr
* DESCRIPTION
*   This function is Highlight handler for Setting->Phonesetup->IdleScreenDisplay->Wallpaper->User Define 
*   Following Function: EntryPhnsetWallpaperFmgr
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void HighlightPhnsetWallpaperFmgr(void)
{
	SetLeftSoftkeyFunction(EntryPhnsetWallpaperFmgr,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetWallpaperFmgr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  PhnsetStopInvaildUserDefineImg
* DESCRIPTION
*   This function is call back function for file manager
*   Following Function: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
BOOL PhnsetStopInvaildUserDefImg(S8 *path)
{
#ifdef MMI_ON_HARDWARE_P

   GDI_RESULT ret;
   U16 img_type = 0;
   S32 img_width = 0, img_height = 0;
   kal_uint32 filesize = 0;
   FS_HANDLE fh = 0;

  mmi_trace(g_sw_WpSsPwon, "gdm:in PhnsetStopInvaildUserDefImg");
   if( path )
   {
      /* check image file format */
      img_type = gdi_image_get_type_from_file(path);
      if( !IMG_TYPE_VAILD( img_type) )
      {
   	      PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "Phnset():[$$$]img_type:%d[$$$]\n", img_type));
	      DisplayPopup((PU8)GetString(STR_WPSS_IMG_ERR_TYPE), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	   return MMI_FALSE;
      }

      /* check image dimension */
      ret = gdi_image_get_dimension_file(path, &img_width, &img_height );
      if( ret <0 )
      {
          PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "Phnset():[$$$]ret:%d[$$$]\n", ret));
 	      DisplayPopup((PU8)GetString(STR_WPSS_IMG_ERR_FILE), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	   return MMI_FALSE;
      }

      if( IMG_DIMESION_INVAILD( img_width, img_height ) )
      {
          PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "Phnset():[$$$]img_width:%d, img_height:%d[$$$]\n", img_width,img_height ));
	      DisplayPopup((PU8)GetString(STR_WPSS_IMG_ERR_DIMESION), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	   return MMI_FALSE;
      }


      /* check image file size */
      fh = pfopen((U8*)path, FS_READ_ONLY, 0);
      if( fh <= 0 )
      {
          PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "Phnset():[$$$]FS return:%d[$$$]\n", fh ));
	      DisplayPopup((PU8)GetString(STR_WPSS_IMG_ERR_FILE), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	   return MMI_FALSE;
      }
      
//      FS_GetFileSize( fh, &filesize ); // simon del
      pfclose( fh );
      if( ((img_type != GDI_IMAGE_TYPE_JPG_FILE) && (filesize > IMG_SIZE_BOUNDARY))||
          ((img_type == GDI_IMAGE_TYPE_JPG_FILE) && (filesize > JPG_SIZE_BOUNDARY)) )
      {
          PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "Phnset():[$$$]type:%d,  size:%d[$$$]\n", img_type, filesize ));
	      DisplayPopup((PU8)GetString(STR_WPSS_IMG_ERR_SIZE), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	   return MMI_FALSE;
      }

 
      /* check file size buffer*/
      /*********************************************
        * To Sync with func gdi_image_draw_file.               
        * GDI should provide checking image vaild function   
        *********************************************/
      ret = gdi_image_util_check_decoder_cap(img_type,UI_device_width,UI_device_height);
      if( ret < 0 )
      {
	      PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "Phnset():[$$$]ret:%d [$$$]\n", ret ));
    	  DisplayPopup((PU8)GetString(STR_WPSS_IMG_ERR_DIMESION), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
      	   return MMI_FALSE;
      }
      /*********************************************/

   }
   else
      return MMI_FALSE;

#endif

   return MMI_TRUE;
}


/*****************************************************************************
* FUNCTION
*  PhnsetWallpaperForwardFromFmgr
* DESCRIPTION
*   This function is call back function for file manager
*   Following Function: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void PhnsetWallpaperForwardFromFmgr(void *path, int is_short)
{
	g_phnset_cntx_p->isShort = (S32)is_short;

	if(path)
	{
		/* Prevent memory leck, after buffer is free, set pointer to NULL */
		ASSERT( phnset_strp == NULL );

		phnset_strp = OslMalloc( FMGR_PATH_BUFFER_SIZE );
		if( phnset_strp )
			pfnUnicodeStrcpy((S8 *)phnset_strp, (S8*)path);
		else
		{
			GoBackToHistory(SCR_IDLE_SCR_DISP);
			return;
		}

		g_phnset_cntx_p->wpss_type=MMI_PHNSET_TYPE_WP;
		g_phnset_cntx_p->ScrAnchor = GetCurrScrnId();
		PhnsetWpUsrDefPreView();
	}
	else
		DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
}

 

/*****************************************************************************
* FUNCTION
*  PhnsetSubWallpaperForwardFromFmgr
* DESCRIPTION
*   This function is call back function for file manager
*   Following Function: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
#ifdef __MMI_SUB_WALLPAPER__

void PhnsetSubWallpaperForwardFromFmgr(void *path, int is_short)
{
	g_phnset_cntx_p->isShort = (S32)is_short;

	if(path)
	{
		/* Prevent memory leck, after buffer is free, set pointer to NULL */
		ASSERT( phnset_strp == NULL );

		phnset_strp = OslMalloc( FMGR_PATH_BUFFER_SIZE );
		if( phnset_strp )
			pfnUnicodeStrcpy((S8 *)phnset_strp, (S8*)path);
		else
		{
			GoBackToHistory(SCR_IDLE_SCR_DISP);
			return;
		}

		g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_SUB_WP;
		g_phnset_cntx_p->ScrAnchor = GetCurrScrnId();

		PhnsetWpUsrDefPreView();
	}
	else
		DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
}

#endif
 

/*****************************************************************************
* FUNCTION
*  PhnsetWpUsrDefFreeBuf
* DESCRIPTION
*   This function is to free file name string buffer
*   Following Function: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/

void PhnsetWpUsrDefFreeBuf(void)
{
	if(phnset_strp)
    {
    	OslMfree( phnset_strp );
	    phnset_strp = NULL;
	}
}


/*****************************************************************************
* FUNCTION
*  PhnsetWpUsrDefSelect
* DESCRIPTION
*   This function is LSK handler for set user defined wallpaper/screen saver
*   Following Function: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void PhnsetWpUsrDefSelect(void)
{
	mmi_trace(g_sw_WpSsPwon, "gdm:into PhnsetWpUsrDefSelect g_phnset_cntx_p->wpss_type=%d",g_phnset_cntx_p->wpss_type);
	if(MMI_FS_GetFileSizeByName((U8*)phnset_strp) > GLOBAL_MMI_IMAGE_LIMIT_SIZE) //chenhe add for bug 9675. as we decode speed is solid , we avoid of large file
	{
		DisplayPopup((PU8)GetString(STR_ID_FMGR_FILE_LIMIT_FILE_TOO_LARGE), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	}
	else if( g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP )
	{
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
		wgui_set_wallpaper_change();
#endif 
		if(mmi_fmgr_util_file_limit_check(FMGR_LIMIT_WALLPAPER, (PS8)phnset_strp, &error_str_ptr))
		{
			PhnsetSetWallpaperByFileName((S8*)phnset_strp);
			DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
		}
		else
			DispIdleScrWPBadFilePopup();
	}
#ifdef __MMI_SUB_WALLPAPER__
	else if( g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SUB_WP)
	{
		if(mmi_fmgr_util_file_limit_check(FMGR_LIMIT_SUB_WALLPAPER, (PS8)phnset_strp, &error_str_ptr))
		{
			PhnsetSetWallpaperByFileName((S8*)phnset_strp);
			DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
		}
		else
			DispIdleScrWPBadFilePopup();
	}
#endif
	else if( g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SS )
	{
		if(mmi_fmgr_util_file_limit_check(FMGR_LIMIT_SCREEN_SAVER, (PS8)phnset_strp, &error_str_ptr))
		{
			PhnsetSetScrSvrByFileName((S8*)phnset_strp);
			DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
		}
		else
			DispIdleScrWPBadFilePopup();
		
    }
	else
		ASSERT(0);
	if(IsScreenPresent(SCR_MAIN_WALLPAPER)||IsScreenPresent(SCR_MAIN_SCREENSAVER))
		fmgr_reset_app_select();/*not forword from fmgr*/
}


/*****************************************************************************
* FUNCTION
*  PhnsetWpUsrDefGoBack
* DESCRIPTION
*   This function is RSK handler for set user define wallpaper/screensaver
*   Following Function: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
U8 PhnsetWpUsrDefPreViewScrDelCB(void *ptr)
{
	PhnsetWpUsrDefFreeBuf();

    return MMI_FALSE;
}

/*****************************************************************************
* FUNCTION
*  PhnsetWpUsrDefGoBack
* DESCRIPTION
*   This function is RSK handler for set user define wallpaper/screensaver
*   Following Function: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void PhnsetWpUsrDefGoBack(void)
{
	PhnsetWpUsrDefFreeBuf();
    
    GoBackHistory();
}


/*****************************************************************************
* FUNCTION
*  PhnsetWpUsrDefPreViewENDKeyHdlr
* DESCRIPTION
*   This function is to END key handler for wallpaper/screen preview screen
*   Following Function: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void PhnsetWpUsrDefPreViewENDKeyHdlr(void)
{
    /* Has to free the file name string buffer  */
    PhnsetWpUsrDefFreeBuf();
    
    /* Execute the stored END key handler */
    if(phnset_wpfp)
    	(*phnset_wpfp)();
}
/*****************************************************************************
* FUNCTION
*  ExitPhnsetWpUsrDefPreView
* DESCRIPTION
*  Exit handler of PhnsetWpUsrDefPreView
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void ExitPhnsetWpUsrDefPreView(void)
{
	if(isCorrectSet)
	{
		PhnsetWpUsrDefFreeBuf();
		DeleteUptoScrID(g_phnset_cntx_p->ScrAnchor);
	}
}

/*****************************************************************************
* FUNCTION
*  PhnsetWpUsrDefPreView
* DESCRIPTION
*   This function is to preview user define wallpaper/screensaver
*   Following Function: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void PhnsetWpUsrDefPreView(void)
{
	U8		*gui_buffer;
	EntryNewScreen(SCR_IDLE_SCR_DISP_WP_PREVIEW, ExitPhnsetWpUsrDefPreView, PhnsetWpUsrDefPreView, NULL);
	gui_buffer = GetCurrGuiBuffer(SCR_IDLE_SCR_DISP_WP_PREVIEW);

	CallbackWpSsLSK = PhnsetWpUsrDefSelect;
	isCorrectSet = 0;
#if defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__)
	if(g_phnset_cntx_p->wpss_type==MMI_PHNSET_TYPE_SS
		&&mmi_fmgr_get_file_group(phnset_strp) == FMGR_GROUP_VIDEO)
	{
		ShowCategory225Screen(
										STR_GLOBAL_VIEW,	/* title_str */
										IMG_SCR_SETTING_CAPTION,/* title_icon */
										STR_GLOBAL_OK,				/* lsk_str */
										IMG_GLOBAL_OK,				/* lsk_icon */
										STR_GLOBAL_BACK,			/* rsk_str */
										IMG_GLOBAL_BACK,			/* rsk_icon */
										0,								/* video_id */
										(S8*)phnset_strp,			/* video_filename */
										1,								/* repeat_count */
										TRUE,							/* is_visual_update */
										TRUE,							/* is_play_audio */
										TRUE,							/* is_lcd_no_sleep */
										GDI_COLOR_WHITE,			/* bg_color */
										PhnsetWpSsPlayCallBack,
										gui_buffer);
	}
	else
#endif//defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__) && !defined(WIN32)
		ShowCategory222Screen (
									STR_GLOBAL_VIEW, 
									GetRootTitleIcon(MENU_SETTING_WALLPAPER),
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK,  
									GDI_COLOR_WHITE,	NULL, 
									(S8*)phnset_strp, 
									FALSE,
									PhnsetWpSsPlayCallBack);

	SetDelScrnIDCallbackHandler(SCR_IDLE_SCR_DISP_WP_PREVIEW, PhnsetWpUsrDefPreViewScrDelCB);

	phnset_wpfp = GetKeyHandler( KEY_END, KEY_EVENT_DOWN );
	SetKeyHandler (PhnsetWpUsrDefPreViewENDKeyHdlr, KEY_END, KEY_EVENT_DOWN);


	SetRightSoftkeyFunction (PhnsetWpUsrDefGoBack, KEY_EVENT_UP);
	SetLeftSoftkeyFunction (CallbackWpSsLSK, KEY_EVENT_UP);


}


/*****************************************************************************
* FUNCTION
*  PhnsetWallpaperSelectFmgrCallBack
* DESCRIPTION
*   This function is call back function for IdleScreenDisplay->Wallpaper->User Define 
*   Following Function: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void PhnsetWallpaperSelectFmgrCallBack(void *path, int is_short)
{
	g_phnset_cntx_p->isShort = (S32)is_short;
	
   if(path)
   {
	if(( g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP
		&&!mmi_fmgr_util_file_limit_check(FMGR_LIMIT_WALLPAPER, (PS8)path, &error_str_ptr))
#ifdef __MMI_SUB_WALLPAPER__
	||( g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SUB_WP 
		&&!mmi_fmgr_util_file_limit_check(FMGR_LIMIT_SUB_WALLPAPER, (PS8)path, &error_str_ptr) )
#endif
	||( g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SS 
		&&!mmi_fmgr_util_file_limit_check(FMGR_LIMIT_SCREEN_SAVER, (PS8)path, &error_str_ptr) )
		)
	{
		DispIdleScrWPBadFilePopup();
		PhnsetWpUsrDefFreeBuf();
	     	return;
	}
	
	  /* Prevent memory leck, after buffer is free, set pointer to NULL */
	  ASSERT( phnset_strp == NULL );

	  phnset_strp = OslMalloc( FMGR_PATH_BUFFER_SIZE );
      if( phnset_strp )
	      pfnUnicodeStrcpy((S8 *)phnset_strp, (S8*)path);
	  else
	  {
	      GoBackToHistory(SCR_IDLE_SCR_DISP);
	      return;
	  }

	  PhnsetWpUsrDefPreView();
	  
   }
   else
		GoBackToHistory(g_phnset_cntx_p->ScrAnchor);
}

/*****************************************************************************
* FUNCTION
*  EntryPhnsetWallpaperFmgr
* DESCRIPTION
*   This function is Entry function for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) -> User Define
*   Follow functions: 
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gHighlightDownloadWallpaper
*     noDownloadImages
*     gDownloadWallpaperList
*     gCurrentWallpaper
*     gDontAddHistoryOnScroll
*****************************************************************************/
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
#if ( defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
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
/* under construction !*/
#endif /* defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__)  */      
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else /* 0 */

/* Entry root directory directly */
extern void EntryPhnsetWallpaperFmgr(void);

void EntryPhnsetWallpaperFmgr(void)
{

#if ( defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )

   //S16 phoneDrvLetter;
   //S8 temp[3];
   //UI_character_type dir_phone_root[4];
	FMGR_FILTER filter;

	FMGR_FILTER_INIT(&filter);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_BMP);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_GIF);
#ifdef JPG_DECODE		         	         	
	FMGR_FILTER_SET(&filter, FMGR_TYPE_JPG);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_JPEG);
#endif
	FMGR_FILTER_SET(&filter,FMGR_TYPE_PNG);
	FMGR_FILTER_SET(&filter,FMGR_TYPE_WBMP);
	FMGR_FILTER_SET(&filter,FMGR_TYPE_WBM);
	FMGR_FILTER_SET(&filter,FMGR_TYPE_EMS);
	FMGR_FILTER_SET(&filter,FMGR_TYPE_ANM);
	FMGR_FILTER_SET(&filter,FMGR_TYPE_PBM);
	
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER_DOT);

   /*
   #define MMI_PUBLIC_DRV  FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM)
   phoneDrvLetter = FS_GetDrive( FS_DRIVE_V_NORMAL, 2, FS_DRIVE_I_SYSTEM | FS_DRIVE_V_NORMAL );
   sprintf( temp, "%c:\\", (S8) phoneDrvLetter );
   AnsiiToUnicodeString( (S8*)dir_phone_root, temp );
   */
#if !defined(WIN32)
#if defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__)||defined(__MMI_POWER_ON_OFF_DISPLAY_VIDEO__)

	if(
#if defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__)
		g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SS
#if defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ )
	||	g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF
	||	g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON
#endif// defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ )
#else
#if defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ )
		g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF
	||	g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON
#endif// defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ )
#endif// defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__)
		)
	{
//#ifdef MP4_CODEC  //guojian deleted for jassmine 20080508
		FMGR_FILTER_SET(&filter, FMGR_TYPE_3GP);
		FMGR_FILTER_SET(&filter, FMGR_TYPE_MP4);
		FMGR_FILTER_SET(&filter, FMGR_TYPE_RM);
		FMGR_FILTER_SET(&filter, FMGR_TYPE_RMVB);
//#endif
#ifdef MJPG_SUPPORT
		FMGR_FILTER_SET(&filter, FMGR_TYPE_AVI); 
#endif
	}
#endif //defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__)||defined(__MMI_POWER_ON_OFF_DISPLAY_VIDEO__)
#endif //!defined(WIN32)

#ifdef __MMI_POWER_ON_OFF_DISPLAY__
	if(	g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWRON
		|| g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_PWROFF)
	{
		/*fix bug 13584-change icon from setting to file manager*/
		mmi_fmgr_select_path_and_enter (\
	   		APP_SETTINGS,\
	   		FMGR_SELECT_FILE|FMGR_SELECT_REPEAT, \
	    		filter, \
				(S8*)L"root", 
	    		PhnsetPwrOnOffSelectFmgrCallBack);
	}
	else
#endif //__MMI_POWER_ON_OFF_DISPLAY__
	{
    	/*fix bug 13584-change icon from setting to file manager*/	
    	mmi_fmgr_select_path_and_enter (\
   	APP_SETTINGS,\
   	FMGR_SELECT_FILE|FMGR_SELECT_REPEAT, \
    filter, \
	(S8*)L"root", 
    PhnsetWallpaperSelectFmgrCallBack);
	}
#endif /* defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__)  */      

}
#endif /* 0 */


#endif


/*****************************************************************************
* FUNCTION
*  DispIdleScrWPBadFilePopup
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
void DispIdleScrWPBadFilePopup(void)
{
	if(error_str_ptr)
		   DisplayPopup((PU8)error_str_ptr, \
                                                   IMG_GLOBAL_UNFINISHED, 1, \
                                                   UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	else
	   DisplayPopup((PU8)GetString(STR_IDLE_WP_DECODE_ERR), \
                                                   IMG_GLOBAL_UNFINISHED, 1, \
                                                   UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	error_str_ptr=NULL;
}


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
void PhnsetWallpaperBadFileCallBack(U16 imgID)
{
	mmi_trace(g_sw_WpSsPwon, "gdm:into PhnsetWallpaperBadFileCallBack");
	error_str_ptr = NULL;
	if(!IdleIsPowerOnPeriod())
		StartTimer(IDLE_SHOW_BADFILE_WARNING_TIMER, 1000,  DispIdleScrWPBadFilePopup );

	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_WP;

	if( imgID == 0 )
		PhnsetRstWallpaperDefault();
	else
		PhnsetSetWallpaperByImgID( imgID );
}

 
#ifdef __MMI_SUB_WALLPAPER__

void PhnsetWallpaperSubBadFileCallBack(U16 imgID)
{
	error_str_ptr = NULL;
	StartTimer(IDLE_SHOW_BADFILE_WARNING_TIMER, 1000,  DispIdleScrWPBadFilePopup );

	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_SUB_WP;
	
	if( imgID == 0 )
		PhnsetRstWallpaperDefault();
	else
		PhnsetSetWallpaperByImgID( imgID );
}

#endif
 

PHNSET_TYPE_DISP PhnsetGetDisplayType(U16 id)
{
	if((id>=WALLPAPER_1 && id<IMG_WALLPAPER_SYSTEM)
#if !defined( __MMI_RESOURCE_SLIM_ON_128X160__)	
    #if !defined( __MMI_RESOURCE_SLIM__)	
	||(id>=SCREENSAVER_1 && id<=SCREENSAVER_9)
	#else
    ||(id>=SCREENSAVER_1 && id<=SCREENSAVER_3)
    #endif
#else
	||(id>=SCREENSAVER_1 && id<=SCREENSAVER_2)
#endif
#ifdef __MMI_ANALOG_CLOCK__
		||(id== ANALOG_CLOCK) 
#endif		
#ifdef __MMI_DIGITAL_CLOCK__
		||(id== DIGITAL_CLOCK) 
#endif		
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
		||(id>=IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1 && id<=IMG_ID_PHNSET_ON_ANIMATION_MAX)
		||(id>=IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1 && id<=IMG_ID_PHNSET_OFF_ANIMATION_MAX)
#if !defined( __MMI_RESOURCE_SLIM_ON_128X160__)	
        #if !defined( __MMI_RESOURCE_SLIM__)	
	    ||(id>=IMG_ID_PHNSET_ON_OFF_1 && id<=IMG_ID_PHNSET_ON_OFF_9)
	    #else
        ||(id>=IMG_ID_PHNSET_ON_OFF_1 && id<=IMG_ID_PHNSET_ON_OFF_3)
        #endif
#else		
#if defined(__PROJECT_GALLITE_C01__) 
		||(id>=IMG_ID_PHNSET_ON_OFF_1 && id<=IMG_ID_PHNSET_ON_OFF_3)
#else
		||(id>=IMG_ID_PHNSET_ON_OFF_1 && id<=IMG_ID_PHNSET_ON_OFF_2)
#endif		
#endif		
#else
		||(id==IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1)
		||(id==IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1)
#endif		
	)
		return PHNSET_TYPE_DISP_IMAGE;
	else
		return PHNSET_TYPE_DISP_VIDEO;
}

void PhnsetWpSsPlayCallBack(S32 result)
{
	if(result>=0){
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
#ifdef __MMI_FILE_MANAGER__
		if(phnset_strp)
			CallbackWpSsLSK = PhnsetWpUsrDefSelect;
		else
#endif//__MMI_FILE_MANAGER__
		{
			if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SS)
				CallbackWpSsLSK = PhnsetScrSvrSysViewLskHdlr;
			else
				ASSERT(0);
		}
	}
	else
	{
		ChangeLeftSoftkey(0,0);
		ChangeRightSoftkey(0,0);

		CallbackWpSsLSK = UI_dummy_function;

		error_str_ptr = GetString(STR_GLOBAL_UNSUPPORTED_FORMAT);
		DispIdleScrWPBadFilePopup();
#ifdef __MMI_FILE_MANAGER__
		PhnsetWpUsrDefFreeBuf();
#endif//__MMI_FILE_MANAGER__

		/* delete to file select screen again */
		DeleteNHistory(1);

 	}

	SetLeftSoftkeyFunction(CallbackWpSsLSK,KEY_EVENT_UP);
}

#ifdef __MMI_WAP__

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetWallpaperDwnl
* DESCRIPTION
*   This function is Highlight handler for Setting->Phonesetup->IdleScreenDisplay->Wallpaper->Download 
*   Following Function: EntryPhnsetWallpaperDwnl
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*   NULL
*****************************************************************************/
void HighlightPhnsetWallpaperDwnl(void)
{
	TRACE_AUD_FUNCTION();
	SetLeftSoftkeyFunction(EntryPhnsetWallpaperDwnl,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetWallpaperDwnl, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  PhnsetConstDwnlWallpaperList
* DESCRIPTION
*   This function is Entry function for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) -> Download
*   Follow functions: EntryPhnsetWallpaperDwnlView
*
* PARAMETERS
*  a  IN/OUT      Names : Wallpaper name list pointer array
*                       DLWallpaperImgID : Wallpaper image id listvoid
*  b  IN       
* RETURNS
*  DL wallpaper numbers
* GLOBALS AFFECTED
*     gHighlightDownloadWallpaper
*     gCurrentWallpaper
*****************************************************************************/
U8 PhnsetConstDwnlWallpaperList(U8 **Names, U16 *DLWallpaperImgID, S16 *HiliteItem )
{
      DYNIMAGEINFO *Info=NULL;
	U8 numDLImgs=0;

	//gHighlightDownloadWallpaper = 0;
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
			DLWallpaperImgID[i]=Info[i].ImagesId;

                if( Names )
                  Names[i]=Info[i].ImageName;

			if( HiliteItem )
			   if(g_phnset_cntx_p->curtWpID==DLWallpaperImgID[i])
				*HiliteItem = i;
		}
		return numDLImgs;
	}
}


/*****************************************************************************
* FUNCTION
*  EntryPhnsetWallpaperDwnl
* DESCRIPTION
*   This function is Entry function for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) -> Download
*   Follow functions: EntryPhnsetWallpaperDwnlView
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gHighlightDownloadWallpaper
*     noDownloadImages
*     gDownloadWallpaperList
*     gCurrentWallpaper
*     gDontAddHistoryOnScroll
*****************************************************************************/
void EntryPhnsetWallpaperDwnl(void)
{

	U8 *Names[MAX_SUB_MENUS];
	
	//ExecuteCurrExitHandler();

   g_phnset_cntx_p->curHightlightItem = 0;
	g_phnset_cntx_p->WPDwnlImgNum = \
	PhnsetConstDwnlWallpaperList(Names, g_phnset_cntx_p->DwnlWPSSList, (S16 *)&(g_phnset_cntx_p->curHightlightItem));

	if( g_phnset_cntx_p->WPDwnlImgNum == 0 )
	      DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, ST_NOTIFYDURATION, EMPTY_LIST_TONE);
	else
	{
		U8 *guiBuffer;
		
	   g_phnset_cntx_p->WPSSPreviewScrFlag = 1;

		guiBuffer = GetCurrGuiBuffer(SCR_WALLPAPER_DOWNLOAD_LIST);
		
		//ExecuteCurrExitHandler();
		EntryNewScreen(SCR_WALLPAPER_DOWNLOAD_LIST, NULL, EntryPhnsetWallpaperDwnl, NULL);
		
		RegisterHighlightHandler(PhnsetWallpaperDwnlHiliteHandler);

		ShowCategory36Screen(STR_WALLPAPER_SELECT_CAPTION,IMG_SCR_SETTING_CAPTION,STR_GLOBAL_VIEW, 0,
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								g_phnset_cntx_p->WPDwnlImgNum, (U8**)Names, g_phnset_cntx_p->curHightlightItem, guiBuffer);

		SetLeftSoftkeyFunction(EntryPhnsetWallpaperDwnlView,	KEY_EVENT_UP);
		SetKeyHandler(EntryPhnsetWallpaperDwnlView,			KEY_RIGHT_ARROW,	KEY_EVENT_DOWN);
	
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

		//SetGenericExitHandler(SCR_WALLPAPER_DOWNLOAD_LIST, NULL, EntryPhnsetWallpaperDwnl);
	}
}




/*****************************************************************************
* FUNCTION
*  PhnsetWallpaperDwnlHiliteHandler
* DESCRIPTION
*   This function is Highlight handler for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) -> Download
*   Follow functions: Save current hig
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/
void PhnsetWallpaperDwnlHiliteHandler( S32 nIndex)
{
	g_phnset_cntx_p->curHightlightItem = (U8)nIndex;
}



/*****************************************************************************
* FUNCTION
*  EntryPhnsetWallpaperDwnlView
* DESCRIPTION
*   This function is Entry function to preview wallpaper
*         for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) -> Download -> View
*   Follow functions: PhnsetWallpaperDwnlViewPre
*                            PhnsetWallpaperDwnlViewNext
*                            PhnsetWallpaperDwnlSet
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gDownloadWallpaperList
*     gHighlightDownloadWallpaper
*     gDontAddHistoryOnScroll
*****************************************************************************/
void EntryPhnsetWallpaperDwnlView(void)
{
	PU8 wallpaperName;
	U8 *guiBuffer;
	U16 wallpaperId = g_phnset_cntx_p->DwnlWPSSList[g_phnset_cntx_p->curHightlightItem];

	guiBuffer = GetCurrGuiBuffer(SCR_WALLPAPER_DOWNLOAD_VIEW);

	if(g_phnset_cntx_p->WPSSPreviewScrFlag)
	{
	   //ExecuteCurrExitHandler();
	   EntryNewScreen(SCR_WALLPAPER_DOWNLOAD_VIEW, ExitPhnsetWallpaperDwnlView, EntryPhnsetWallpaperDwnlView, NULL);
	}

	wallpaperName = (PU8)PhnsetGetWallpaperNameById(wallpaperId);

 
#if defined(__MMI_TOUCH_SCREEN__)
	register_touch_button_handlers(PhnsetWallpaperDwnlViewPre, NULL,
			PhnsetWallpaperDwnlViewNext, NULL);
#endif

#if defined(__MMI_DRAW_MANAGER__)
	wgui_set_touch_title_bar_buttons();//enable the display of title bar buttons

	ShowCategory129Screen(wallpaperName, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK,
			STR_GLOBAL_BACK, IMG_GLOBAL_BACK, wallpaperId, guiBuffer);
#else
	ShowCategory129Screen(wallpaperName, IMG_SCR_SETTING_CAPTION, STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
							wallpaperId, guiBuffer);
#endif
 

	SetLeftSoftkeyFunction(PhnsetWallpaperDwnlSet,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#ifdef    __MMI_NOKIA_STYLE_N800__ 
        SetKeyHandler(PhnsetWallpaperDwnlSet , KEY_LSK,  KEY_EVENT_UP);
        SetKeyHandler(GoBackHistory , KEY_RSK,  KEY_EVENT_UP);
        ClearKeyHandler( KEY_LSK,  KEY_EVENT_DOWN);
        ClearKeyHandler( KEY_RSK,  KEY_EVENT_DOWN);
#endif
	
 
/* changed the up-down arrow key handlers to left-right arrow key handlers
	for having the similar behaviour of keypad and touchscreen. */
//	SetKeyHandler(PhnsetWallpaperDwnlViewPre,		KEY_UP_ARROW,	KEY_EVENT_UP);
//BUG IMAGE VIEW START 20050917
// change the key handler for KEY UP event to KEY DOWN event.
	SetKeyHandler(PhnsetWallpaperDwnlViewPre,		KEY_LEFT_ARROW,	KEY_EVENT_DOWN);
//BUG IMAGE VIEW END 20050917
 
	SetKeyHandler(PhnsetWallpaperDwnlViewPre,		KEY_VOL_UP,		KEY_EVENT_DOWN);
	SetKeyHandler(PhnsetWallpaperDwnlViewPre,		KEY_VOL_UP,		KEY_LONG_PRESS);

 
//	SetKeyHandler(PhnsetWallpaperDwnlViewNext,	KEY_DOWN_ARROW,	KEY_EVENT_UP);
//BUG IMAGE VIEW START 20050917
// change the key handler for KEY UP event to KEY DOWN event.
	SetKeyHandler(PhnsetWallpaperDwnlViewNext,	KEY_RIGHT_ARROW,	KEY_EVENT_DOWN);
//BUG IMAGE VIEW END 20050917
 
	SetKeyHandler(PhnsetWallpaperDwnlViewNext,	KEY_VOL_DOWN,	KEY_EVENT_DOWN);
	SetKeyHandler(PhnsetWallpaperDwnlViewNext,	KEY_VOL_DOWN,	KEY_LONG_PRESS);
		
   //SetGenericExitHandler(SCR_WALLPAPER_DOWNLOAD_VIEW, ExitPhnsetWallpaperDwnlView, EntryPhnsetWallpaperDwnlView);
}



/*****************************************************************************
* FUNCTION
*  ExitPhnsetWallpaperDwnlView
* DESCRIPTION
*   This function is Exit function for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) -> Download -> View
*   Follow functions: Add to history
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gDontAddHistoryOnScroll
*****************************************************************************/
void ExitPhnsetWallpaperDwnlView(void)
{
	g_phnset_cntx_p->WPSSPreviewScrFlag=1;
}


/*****************************************************************************
* FUNCTION
*  PhnsetWallpaperDwnlViewPre
* DESCRIPTION
*   This function is to view previous wallpaper
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gHighlightDownloadWallpaper
*     noDownloadImages
*     gDontAddHistoryOnScroll
*****************************************************************************/
void PhnsetWallpaperDwnlViewPre(void)
{
	if(g_phnset_cntx_p->curHightlightItem <= 0 )
		g_phnset_cntx_p->curHightlightItem = g_phnset_cntx_p->WPDwnlImgNum;
	
	g_phnset_cntx_p->curHightlightItem--;
	g_phnset_cntx_p->WPSSPreviewScrFlag = 0;
	EntryPhnsetWallpaperDwnlView();
}




/*****************************************************************************
* FUNCTION
*  PhnsetWallpaperDwnlViewNext
* DESCRIPTION
*   This function is to view previous wallpaper
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gHighlightDownloadWallpaper
*     noDownloadImages
*     gDontAddHistoryOnScroll
*****************************************************************************/
void PhnsetWallpaperDwnlViewNext(void)
{
	if(g_phnset_cntx_p->curHightlightItem >= g_phnset_cntx_p->WPDwnlImgNum-1 )
		g_phnset_cntx_p->curHightlightItem = -1;
	
	g_phnset_cntx_p->curHightlightItem++;
	g_phnset_cntx_p->WPSSPreviewScrFlag = 0;
	EntryPhnsetWallpaperDwnlView();
}




/*****************************************************************************
* FUNCTION
*  PhnsetWallpaperDwnlSet
* DESCRIPTION
*   This function is left softkey handler preview wallpaper screen
*         for Setting->Phonesetup->IdleScreenDisplay->Wallpaper (->Fix) -> Download -> View ->
*   Functionality: Set Download image as wallpaper
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       void
* RETURNS
*  void
* GLOBALS AFFECTED
*     gDownloadWallpaperList
*     gHighlightDownloadWallpaper
*     gDontAddHistoryOnScroll
*****************************************************************************/
void PhnsetWallpaperDwnlSetDone(void)
{
	GoBackToHistory(g_phnset_cntx_p->ScrAnchor);
}
void PhnsetWallpaperDwnlSet(void)
{
	S16 error;

	if(MMI_FS_GetFileSizeByName((U8*)phnset_wpss_filename.wallpaper) > GLOBAL_MMI_IMAGE_LIMIT_SIZE) //chenhe add for bug 9675. as we decode speed is solid , we avoid of large file
	{
		DisplayPopup((PU8)GetString(STR_ID_FMGR_FILE_LIMIT_FILE_TOO_LARGE), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
		fmgr_reset_app_select();/*not forword from fmgr*/
	}
	else
	{
		DisplayPopupCallBack((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, 
		SUCCESS_TONE,PhnsetWallpaperDwnlSetDone);
		g_phnset_cntx_p->curtWpID = g_phnset_cntx_p->DwnlWPSSList[g_phnset_cntx_p->curHightlightItem];
		WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
		SetIdleScreenWallpaperID(g_phnset_cntx_p->curtWpID);
	}
}



/*****************************************************************************
* FUNCTION
*  UpdateDeletedImageWallpaper
* DESCRIPTION
*   This function is to set wallpaper to default one if the downloaded image is deleted
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       ImageId
* RETURNS
*  void
* GLOBALS AFFECTED
*     gCurrentWallpaper
*****************************************************************************/
void UpdateDeletedImageWallpaper(U16 ImageId)
{
	if( ImageId == g_phnset_cntx_p->curtWpID )
		PhnsetRstWallpaperDefault();

	 
	#ifdef __MMI_SUB_WALLPAPER__
	if( ImageId == g_phnset_cntx_p->curtSubWpID )
		RstGetWallpaperSubImgIdDefault();
	#endif
	 
}

#endif   /* __MMI_WAP__  */



/* 
** Externel Interfaces
*/
/*****************************************************************************
* FUNCTION
*  PhnsetRstWallpaperDefault
* DESCRIPTION
*   This function is to set wallpaper to default one
*
* PARAMETERS
*  a  IN/OUT      void
*  b  IN       ImageId
* RETURNS
*  void
* GLOBALS AFFECTED
*     gCurrentWallpaper
*****************************************************************************/
pBOOL PhnsetRstWallpaperDefault(void)
{
	S16 error;	
	mmi_trace(g_sw_WpSsPwon, "gdm:into PhnsetRstWallpaperDefault");

/*WeiD Modify Start For 6126 Ver: TBM780  on 2007-8-20 20:24 */
//	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)
	if(1)
/*WeiD Modify End  For 6126 Ver: TBM780  on 2007-8-20 20:24 */
	{
		g_phnset_cntx_p->curtWpID = RstGetWallpaperImgIdDefault();

	#ifdef MMI_ON_WIN32
		if(g_phnset_cntx_p->curtWpID==0xFFFF)
			g_phnset_cntx_p->curtWpID = WALLPAPER_2;
	#endif
	
		WriteValue( NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
		SetIdleScreenWallpaperID(g_phnset_cntx_p->curtWpID);
		if(error == NVRAM_WRITE_SUCCESS)
			return TRUE;
	}
#ifdef __MMI_SUB_WALLPAPER__
	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SUB_WP)
	{
		g_phnset_cntx_p->curtSubWpID = RstGetWallpaperSubImgIdDefault();//RstGetWallpaperImgIdDefault();
		WriteValue( NVRAM_SETWALLPAPER_SUB, &(g_phnset_cntx_p->curtSubWpID), DS_SHORT, &error);
	
		SUBLCD_SetIdleScreenWallpaperID(g_phnset_cntx_p->curtSubWpID);
		if(error == NVRAM_WRITE_SUCCESS)
			return TRUE;

	}
#endif		
	else
		MMI_ASSERT(0);

	return FALSE;
}





/*****************************************************************************
* FUNCTION
*  readWallpaperSettings
* DESCRIPTION
*   This function is to Read wallpaper from Nvram, and set as current wallpaper
*   Functionality: Referenced at InitNvramData when bootup
*
* PARAMETERS
*  a  IN/OUT      void *
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*     gCurrentWallpaper
*****************************************************************************/
void readWallpaperSettings(void)
{
	S16 error;

	mmi_trace(g_sw_WpSsPwon, "gdm:into readWallpaperSettings,NVRAM_FUNANDGAMES_SETWALLPAPER=%d",NVRAM_FUNANDGAMES_SETWALLPAPER);
	ReadValue( NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);

	if( (g_phnset_cntx_p->curtWpID) == 0xFFFF )
	{
		g_phnset_cntx_p->curtWpID = RstGetWallpaperImgIdDefault();

	#ifdef MMI_ON_WIN32  
		if(g_phnset_cntx_p->curtWpID==0xFFFF)
			g_phnset_cntx_p->curtWpID = WALLPAPER_2;
	#endif
	
		WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
	}
	
	if(g_phnset_cntx_p->curtWpID == 0 
	|| g_phnset_cntx_p->curtWpID == 1 )//should hint short file name
	{
		ReadRecord( NVRAM_EF_WPSS_FILENAME_LID, 1, &phnset_wpss_filename, sizeof(phnset_wpss_filename), &error );
		if( error == NVRAM_READ_SUCCESS &&pfnUnicodeStrlen(phnset_wpss_filename.wallpaper))
		{
			PhnsetSetWallpaperByFileName( (S8 *)phnset_wpss_filename.wallpaper );
		}
		else
		{
			memset( &phnset_wpss_filename, 0, sizeof(phnset_wpss_filename) );
			g_phnset_cntx_p->curtWpID = RstGetWallpaperImgIdDefault();
			WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
			SetIdleScreenWallpaperID(g_phnset_cntx_p->curtWpID);
		}
	}
	else
		SetIdleScreenWallpaperID(g_phnset_cntx_p->curtWpID);
	
	#ifdef __MMI_SUB_WALLPAPER__
	ReadValue( NVRAM_SETWALLPAPER_SUB, &(g_phnset_cntx_p->curtSubWpID), DS_SHORT, &error);

	if( (g_phnset_cntx_p->curtSubWpID) == 0xFFFF )
	{
		g_phnset_cntx_p->curtSubWpID = RstGetWallpaperSubImgIdDefault();  
		WriteValue(NVRAM_SETWALLPAPER_SUB, &(g_phnset_cntx_p->curtSubWpID), DS_SHORT, &error);
	}

 
	if( g_phnset_cntx_p->curtSubWpID == 0 )
	{
                ReadRecord( NVRAM_EF_WPSS_FILENAME_LID, 3, &phnset_wpss_sub_filename, sizeof(phnset_wpss_sub_filename), &error );
                if( error == NVRAM_READ_SUCCESS )
                        SUBLCD_SetIdleScreenWallpaperName( (UI_string_type)phnset_wpss_sub_filename.wallpaper );
                else
                {
                        memset( &phnset_wpss_sub_filename, 0, sizeof(phnset_wpss_sub_filename) );
                        g_phnset_cntx_p->curtSubWpID = RstGetWallpaperSubImgIdDefault();  
                        WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtSubWpID), DS_SHORT, &error);
                        SUBLCD_SetIdleScreenWallpaperID(g_phnset_cntx_p->curtWpID);
                }
	}
	else
		SUBLCD_SetIdleScreenWallpaperID(g_phnset_cntx_p->curtSubWpID);
		
	//if( g_phnset_cntx_p->curtSubWpID != 0 )
	//	SUBLCD_SetIdleScreenWallpaperID(g_phnset_cntx_p->curtSubWpID);

 


#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
	ReadPhnsetIdleClockType();
#endif

	
#endif
	
}



/*****************************************************************************
* FUNCTION
*  PhnsetGetCurWallpaperName
* DESCRIPTION
*   This function is to get current name string pointer of wallpaper
*   Functionality: ex: Referenced by hint windows
*
* PARAMETERS
*  a  IN/OUT      void *
*  b  IN       
* RETURNS
*  PS8 wallpaper name string pointer
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/
PS8 PhnsetGetFileNamePointer( PS8 path )
{
    U16 len;
    U16 deter=0x002F; /* '/' */ //解决从T卡上设置墙纸、开关机动画时，hint显示错误的bug//modified by zhoumn@2007/07/26
    S16 *p = (S16 *)path;

    len = (U16)pfnUnicodeStrlen( path );
    for( ; len>0; len-- )
    {
         if( *(p+len) == deter )
            return (PS8)(p+len+1);
    }
    
    return NULL;
}

PS8 PhnsetGetCurWallpaperName(void)
{
#ifdef __MMI_FILE_MANAGER__	
	if( g_phnset_cntx_p->curtWpID == 0 )
	   return PhnsetGetFileNamePointer( (PS8 )phnset_wpss_filename.wallpaper);
	else if(g_phnset_cntx_p->curtWpID==1)
	{
		U8 temp[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];
		PS8 p;
		
		/* rename short name file should pass converted short name to editor,
			so that the display will be the same as in File Manager */
		p = PhnsetGetFileNamePointer( (PS8 )phnset_wpss_filename.wallpaper);

		mmi_chset_mixed_text_to_ucs2_str((U8* )temp, 
													(FMGR_MAX_FILE_LEN+1)* ENCODING_LENGTH, 
													(U8*)p, 
													(mmi_chset_enum)PhnsetGetDefEncodingType() );
		p = (PS8)&temp[0];

		return p;

	}
	else
#endif//__MMI_FILE_MANAGER__	
		return PhnsetGetWallpaperNameById(g_phnset_cntx_p->curtWpID);
}

#ifdef __MMI_SUB_WALLPAPER__
/*****************************************************************************
* FUNCTION
*  PhnsetGetCurSubWallpaperName
* DESCRIPTION
*   This function is to get current name string pointer of sublcd wallpaper
*   Functionality: ex: Referenced by hint windows
*
* PARAMETERS
*  a  IN/OUT      void *
*  b  IN       
* RETURNS
*  PS8 wallpaper name string pointer
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/
PS8 PhnsetGetCurSubWallpaperName(void)
{
	/* not supported now */
	if( g_phnset_cntx_p->curtSubWpID == 0 )
		return PhnsetGetFileNamePointer( (PS8 )phnset_wpss_sub_filename.wallpaper);  
	else
		return PhnsetGetWallpaperNameById(g_phnset_cntx_p->curtSubWpID);
}
#endif


/*****************************************************************************
* FUNCTION
*  PhnsetGetWallpaperNameById
* DESCRIPTION
*   This function is to translate image ID to name string pointer
*   Functionality: ex: Referenced by hint windows
*
* PARAMETERS
*  a  IN/OUT      
*  b  IN       U16 image ID
* RETURNS
*  Wallpaper Name as PS8 passed as U16 as its ID
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/
PS8 PhnsetGetWallpaperNameById(U16 Id)
{
	//static S8 wallpaperName[MAX_IMAGE_NAME_WIDTH];
	PS8 pstr=NULL;
	
	U32 tempVariable;

	
	if( Id > WALLPAPER_BASE && Id <= WALLPAPER_BASE + MAX_SYSTEM_WALLPAPER )
	{
	     	/* System Wallpaper Range */
 		tempVariable = STR_WALLPAPER_1 + Id - (WALLPAPER_BASE) - 1;
		//pfnUnicodeStrcpy((PS8)wallpaperName, GetString((U16)tempVariable));
		pstr = GetString((U16)tempVariable);
	}
#ifdef __MMI_SUB_WALLPAPER__
	else if(Id >= SUB_WALLPAPER_1 && Id < SUB_WALLPAPER_1+MAX_SYSTEM_SUB_WALLPAPER)
	{
		tempVariable = STR_SUB_WALLPAPER_1 + Id - (SUB_WALLPAPER_1);
		pstr = GetString((U16)tempVariable);
	}
#endif
#if 0  // zrx del 20060612
	else if((tempVariable = getFactoryImageRelativeIndex(Id)) != 0 )
        {
         /* Factory Wallpaper Range*/
		DYNIMAGEINFO *Info;
		U8 i;
		U8 noFactImages=GetFullScreenFactoryImagesInfo(&Info);
		
		for(  i=0; i<noFactImages; i++ )
		{
			if( Id == Info[i].ImagesId )
			{
				tempVariable = STR_WALLPAPER_1 + MAX_SYSTEM_WALLPAPER + i;
				break;
			}
		}
		//pfnUnicodeStrcpy((PS8)wallpaperName, GetString((U16)tempVariable));
		pstr = GetString((U16)tempVariable);
	}
	#endif
	
#ifdef __MMI_ANALOG_CLOCK__
	else if( Id == ANALOG_CLOCK_WALLPAPER )
	{
           /*Analog Clock Wallpaper*/
		tempVariable = STR_WALLPAPER_ANALOG;
		//pfnUnicodeStrcpy((PS8)wallpaperName, GetString((U16)tempVariable));
		pstr = GetString((U16)tempVariable);
	}
#endif

#ifdef __MMI_DIGITAL_CLOCK__	
	else if( Id == DIGITAL_CLOCK_WALLPAPER )
	{
           /*Digital Clock Wallpaper*/
		tempVariable = STR_WALLPAPER_DIGITAL;
		//pfnUnicodeStrcpy((PS8)wallpaperName, GetString((U16)tempVariable));
		pstr = GetString((U16)tempVariable);
}
#endif
#ifdef __MMI_THEMES_V2_SUPPORT__	
	else if( Id == CURRENT_THEME_WALLPAPER )
	{
      /*Digital Clock Wallpaper*/
		tempVariable = STR_CURR_WALLPAPER;
		pstr = GetString((U16)tempVariable);
}
#endif

#ifdef __MMI_WAP__
	else
     {
         /*WAP Download Wallpaper*/
	     //memset(wallpaperName, 0, MAX_IMAGE_NAME_WIDTH);
         //pfnUnicodeStrcpy((PS8)wallpaperName, GetImageNameInFileSystem(Id));
         pstr = GetImageNameInFileSystem(Id);
	}
#endif

	//return (PS8)wallpaperName;
	return pstr;

}


/*****************************************************************************
* FUNCTION
*  PhnsetSetWallpaperByImgID
* DESCRIPTION
*   This function is to set current wallpaper using index by AT command
*
* PARAMETERS
*  a  IN/OUT      
*  b  IN       U16 image ID
* RETURNS
*  void
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/
void PhnsetSetWallpaperByImgID(U16 imgID)
{
    	S16 error;

	if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)
	{
		g_phnset_cntx_p->curtWpID = imgID;
		WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
		SetIdleScreenWallpaperID(g_phnset_cntx_p->curtWpID);
		
		if( g_idle_context.IsOnIdleScreen )
			EntryIdleScreen();

	}
#ifdef __MMI_SUB_WALLPAPER__
	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SUB_WP)
	{
		g_phnset_cntx_p->curtSubWpID = imgID;
		WriteValue(NVRAM_SETWALLPAPER_SUB, &(g_phnset_cntx_p->curtSubWpID), DS_SHORT, &error);
		SUBLCD_SetIdleScreenWallpaperID(g_phnset_cntx_p->curtSubWpID);
	}
#endif
	else
		MMI_ASSERT(0);

	isCorrectSet = 1;

}

/*****************************************************************************
* FUNCTION
*  PhnsetSetWallpaperByFileName
* DESCRIPTION
*   This function is to set current wallpaper using index by AT command
*
* PARAMETERS
*  a  IN/OUT      
*  b  IN       U16 image ID
* RETURNS
*  void
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/
extern U8 isIdleScreenReached;
void PhnsetSetWallpaperByFileName( S8 *path )
{
  S16 error;

    S8			ext_name[20];
    PS8		ext_ptr = NULL;
#ifdef __MMI_SAVE_WP_SPEEDUP__/*gdm modify to add  the switch __MMI_SAVE_WP_SPEEDUP__  091125*/
    #if 0
    S16     i = 0;
    S8	teflag_wp_gif = 0;
    INT32 iCpyRet = 0;
    #endif
#endif
  isCorrectSet = 1;

  mmi_trace(g_sw_WpSsPwon, "gdm:into PhnsetSetWallpaperByFileName,g_phnset_cntx_p->wpss_type=%d",g_phnset_cntx_p->wpss_type);
  if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_WP)
  {
    if(g_phnset_cntx_p->isShort)
      g_phnset_cntx_p->curtWpID = 1;
    else
      g_phnset_cntx_p->curtWpID = 0;

    pfnUnicodeStrcpy((S8 *)phnset_wpss_filename.wallpaper, (S8*)path);
    mmi_trace(g_sw_WpSsPwon, "gdm:into PhnsetSetWallpaperByFileName,phnset_wpss_filename.wallpaper=");
  
    ext_ptr = mmi_fmgr_extract_ext_file_name(path);

    memset(ext_name, 0, sizeof(ext_name));
    UnicodeNToAnsii(ext_name, ext_ptr, 5*ENCODING_LENGTH);

#ifdef __MMI_SAVE_WP_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_WP_SPEEDUP__  091125*/
       #if 0//dont' save gif wallpaper to NARAM modify  gdm 091125 
	    for(i = 0 ; i < 12 ; i++)
	    {
	      if(ext_name[i] != 0)
	      {
	        if(ext_name[i] >= 'a')
	        ext_name[i] -= 'a' - 'A';
	      }
	    }
    
	    if(strcmp(ext_name, "GIF") == 0)
	    {
	      if ( strcmp((PS8)path, (PS8)PHNSET_PREDEFINED_WP_PATH_GIF) == 0 )
	       {
	         mmi_trace(g_sw_WpSsPwon, "gdm:name same");
	         iCpyRet = MMI_FS_Move((U8*)path, (U8*)PHNSET_PREDEFINED_WP_PATH_GIF, FS_MOVE_COPY, NULL,1);
	       }
	        else
	       {
	         mmi_trace(g_sw_WpSsPwon, "gdm:name differ");
	         iCpyRet = MMI_FS_Move((U8*)path, (U8*)PHNSET_PREDEFINED_WP_PATH_GIF, FS_MOVE_COPY, NULL, 0);
	       }
	        if ( iCpyRet != FS_NO_ERROR)
	       {
	         mmi_trace(g_sw_WpSsPwon, "gdm:in PhnsetSetWallpaperByFileName(),  gif call MMI_FS_Move() error");
	         teflag_wp_gif = -1;
	        }
	        else
	          teflag_wp_gif = 1;
	     }
        #endif 
#endif
	    mmi_trace(g_sw_WpSsPwon, "gdm:PhnsetSetWallpaperByFileName, g_phnset_cntx_p->curtWpID=%d", g_phnset_cntx_p->curtWpID);
	    WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
	    WriteRecord( NVRAM_EF_WPSS_FILENAME_LID, 1, &phnset_wpss_filename, sizeof(phnset_wpss_filename), &error );

		idle_screen_wallpaper_ID = 0;

#ifdef __MMI_SAVE_WP_SPEEDUP__/*gdm modify to add  the switch of __MMI_SAVE_WP_SPEEDUP__*/
	    SetIdleScreenWallpaperName((UI_string_type)phnset_wpss_filename.wallpaper);
	
		if(isIdleScreenReached)
		{
			wgui_save_wallpaper_to_speedup();
		}

       #if 0
	    if(( teflag_wp_gif == 1) && (strcmp(ext_name, "GIF") == 0) ) 
	      idle_screen_wallpaper_name=(S8*) PHNSET_PREDEFINED_WP_PATH_GIF;
	    else if(( teflag_wp_gif == -1) && (strcmp(ext_name, "GIF") == 0) ) 
	      idle_screen_wallpaper_name = idle_screen_wallpaper_name_original;
      #endif
#endif

	    if( g_idle_context.IsOnIdleScreen )
	  	{
	      EntryIdleScreen();
	  	}
	}
#ifdef __MMI_SUB_WALLPAPER__
	else if(g_phnset_cntx_p->wpss_type == MMI_PHNSET_TYPE_SUB_WP)
	{
		g_phnset_cntx_p->curtSubWpID = 0;
		pfnUnicodeStrcpy((S8 *)phnset_wpss_sub_filename.wallpaper, (S8*)path);
		WriteValue(NVRAM_SETWALLPAPER_SUB, &(g_phnset_cntx_p->curtSubWpID), DS_SHORT, &error);
		WriteRecord( NVRAM_EF_WPSS_FILENAME_LID, 3, &phnset_wpss_sub_filename, sizeof(phnset_wpss_sub_filename), &error );
		SUBLCD_SetIdleScreenWallpaperName((UI_string_type) path);
		SUBLCD_UpdateWallpaper();
	}
#endif
	else
	{
		MMI_ASSERT(0);
	}
mmi_trace(g_sw_WpSsPwon,"gdm:exit PhnsetSetWallpaperByFileName,idle_screen_wallpaper_name");
mmi_trace(g_sw_WpSsPwon,"gdm:exit PhnsetSetWallpaperByFileName,phnset_wpss_filename.wallpaper=");
 
}


/*****************************************************************************
* FUNCTION
*  PhnsetSetDisplayFromAT
* DESCRIPTION
*   This function is to set current wallpaper using index by AT command
*
* PARAMETERS
*  a  IN/OUT      
*  b  IN       U16 image ID
* RETURNS
*  void
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/
BOOL PhnsetSetDisplayFromAT(U8 idx, U8 type)
{
     U16 SysWpIDList[MAX_SYS_WPSS_NUMBERS];
	U8 max_idx = 0;
	switch(type)
	{
		case MMI_PHNSET_TYPE_WP:
		{
			max_idx = PhnsetConstSysWallpaperList( NULL, SysWpIDList, NULL );

			if( idx < max_idx ) /*unsigned comparison with 0 is always true: '>=' */
      {
      	S16 error;
           
      	g_phnset_cntx_p->curtWpID = SysWpIDList[idx];
      	WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
      	WriteValue(RESTORE_DEFAULT_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);  

      	SetIdleScreenWallpaperID(g_phnset_cntx_p->curtWpID);

      	if( g_idle_context.IsOnIdleScreen )
      	   EntryIdleScreen();
      }
     else
     	return MMI_FALSE;
		}
		break;
		case MMI_PHNSET_TYPE_SS:
		{
			max_idx = PhnsetConstSysScrSvrList( NULL, SysWpIDList, NULL );
			if( idx <  max_idx) /*unsigned comparison with 0 is always true: '>=' */
			{
      		S16 error;
           
				g_phnset_cntx_p->curSsID = SysWpIDList[idx];
				WriteValue(NVRAM_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);
				WriteValue(RESTORE_DEFAULT_CURRENT_SCREENSVER_ID, &(g_phnset_cntx_p->curSsID), DS_SHORT, &error);  
			}
     		else
     			return MMI_FALSE;
			
		}
		break;	
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
		case MMI_PHNSET_TYPE_PWRON:
		{
			max_idx = PhnsetConstSysPwrOnOffList( NULL, SysWpIDList, NULL );
			if( idx <  max_idx) /*unsigned comparison with 0 is always true: '>=' */
			{
      		S16 error;
           
				g_phnset_cntx_p->curtPwrOnID= SysWpIDList[idx];
				WriteValue(NVRAM_CURRENT_POWER_ON_DISPLAY, &(g_phnset_cntx_p->curtPwrOnID), DS_SHORT, &error);
				WriteValue(NVRAM_RESTORE_POWER_ON_DISPLAY, &(g_phnset_cntx_p->curtPwrOnID), DS_SHORT, &error);  
			}
     		else
     			return MMI_FALSE;
			
		}
		break;	
		case MMI_PHNSET_TYPE_PWROFF:
		{
			max_idx = PhnsetConstSysPwrOnOffList( NULL, SysWpIDList, NULL );
			if( idx <  max_idx) /*unsigned comparison with 0 is always true: '>=' */
			{
      		S16 error;
           
				g_phnset_cntx_p->curtPwrOffID= SysWpIDList[idx];
				WriteValue(NVRAM_CURRENT_POWER_OFF_DISPLAY, &(g_phnset_cntx_p->curtPwrOffID), DS_SHORT, &error);
				WriteValue(NVRAM_RESTORE_POWER_OFF_DISPLAY, &(g_phnset_cntx_p->curtPwrOffID), DS_SHORT, &error);  
			}
     		else
     			return MMI_FALSE;

		}
		break;	
#endif
	}
     return MMI_TRUE;
     
}

/*****************************************************************************
* FUNCTION
*  PhnsetGetDisplayBound
* DESCRIPTION
*   This function is to get index boundary by AT command
*
* PARAMETERS
*  a  IN/OUT      
*  b  IN       U16 image ID
* RETURNS
*  U8 boundary
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/

U8 PhnsetGetATDisplayBound(void)
{
	U8 bound=5;

#ifdef	__MMI_POWER_ON_OFF_DISPLAY__
		bound+=2;
#endif

     return bound;
}


/*****************************************************************************
* FUNCTION
*  PhnsetGetWallpaperIdxBound
* DESCRIPTION
*   This function is to get index boundary by AT command
*
* PARAMETERS
*  a  IN/OUT      
*  b  IN       U16 image ID
* RETURNS
*  U8 boundary
* GLOBALS AFFECTED
*     NULL
*****************************************************************************/
U8 PhnsetGetWallpaperIdxBound()
{
	DYNIMAGEINFO *Info;
     return (MAX_SYSTEM_WALLPAPER + GetFullScreenFactoryImagesInfo(&Info) );
}

#ifdef __MMI_THEMES_V2_SUPPORT__
BOOL PhnsetIsWPCurrentTheme(void)
{
	if( g_phnset_cntx_p->curtWpID == CURRENT_THEME_WALLPAPER )
		return TRUE;
	else
		return FALSE;
}

#endif
//liuxl add for usb
/*to judge whether the specified WallPaper is  from T-Flash or not*/
BOOL IsPhnsetWallPaperFromExternal(U16 WpID, U8 Type)
{
	S16 error;
	U8 filename[(PHNSET_MAX_FILE_LEN+PHNSET_MAX_PATH_LEN+PHNSET_MAX_EXT_LEN)];
	if((WpID == 0) ||(WpID == 1))
	{
		ReadRecord( NVRAM_EF_WPSS_FILENAME_LID, 1, &phnset_wpss_filename, sizeof(phnset_wpss_filename), &error );
		if( error == NVRAM_READ_SUCCESS )
		{

			if ( Type ==  MMI_PHNSET_TYPE_WP )
			{
				UnicodeToAnsii(filename, (S8*)phnset_wpss_filename.wallpaper);
			}
			else if ( Type == MMI_PHNSET_TYPE_SS )
			{
				UnicodeToAnsii(filename, (S8*)phnset_wpss_filename.screensave);
			}
			else
				return FALSE;
			
			if((strncmp((S8 *)filename, "D:/", 3) == 0) ||(strncmp((S8 *)filename, "d:/", 3) == 0) )
			{
				return TRUE;
			}
			else
			{			
				return FALSE;
			}
		}
		else
		{		
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

/*set the default file as wall paper*/
void PhnsetRstWallPaperDefaultTmp(void)
{
	S16 error;
	g_phnset_cntx_p->curtWpID = RstGetWallpaperImgIdDefault();
	WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
	SetIdleScreenWallpaperID(g_phnset_cntx_p->curtWpID);
}

/*restore the specified id as wall paper*/
void PhnsetWallPaperRestore(U16 WpID,BOOL IsRestore)
{
	S16 error;
	S8 tempname[256];
	if(IsRestore)
	{
		ReadRecord( NVRAM_EF_WPSS_FILENAME_LID, 1, &phnset_wpss_filename, sizeof(phnset_wpss_filename), &error );
		if( error == NVRAM_READ_SUCCESS )
		{
			#ifdef __MMI_WALLPAPER_ON_BOTTOM__
				wgui_set_wallpaper_change();
			#endif 
			#if 1//When pull out the USB,should show wallpaper, gdm 091023 fix bug 15007
		    g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_WP;
			#endif
			UnicodeToAnsii(tempname, (S8 *)phnset_wpss_filename.wallpaper);
			mmi_trace(g_sw_SETTING, "LXL-- %s entry %s function, File: %s, Line: %d,name=%s", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__,tempname);			
			PhnsetSetWallpaperByFileName( (S8 *)phnset_wpss_filename.wallpaper );
		}
		else
		{
			memset( &phnset_wpss_filename, 0, sizeof(phnset_wpss_filename) );
			g_phnset_cntx_p->curtWpID = RstGetWallpaperImgIdDefault();
			WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
			SetIdleScreenWallpaperID(g_phnset_cntx_p->curtWpID);
		}
	}
	else
	{
		g_phnset_cntx_p->curtWpID = WpID;
		WriteValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
		SetIdleScreenWallpaperID(g_phnset_cntx_p->curtWpID);
	}
}

/*get th current wall paper ID*/
void PhnsetGetCurWallPaper(U16 *wallPaper)
{
	S16 error;
	ReadValue(NVRAM_FUNANDGAMES_SETWALLPAPER, &(g_phnset_cntx_p->curtWpID), DS_SHORT, &error);
	*wallPaper = g_phnset_cntx_p->curtWpID;
}
#if defined(MOTION_SENSOR_SUPPORT)
void EntryIdleShakeChangeWallpaper(void)
{
	S32 i;
	U16 SysWpIDList[MAX_WALLPAPER_IMG], ImgId = 0;

	mmi_trace(4,"EntryIdleShakeChangeWallpaper");
	
	if(GetActiveScreenId()==IDLE_SCREEN_ID)
	{
		g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_WP;
		memset(SysWpIDList, 0, sizeof(SysWpIDList));
	    PhnsetConstSysWallpaperList(NULL, SysWpIDList, NULL);
		i = 0;
		while (i < MAX_WALLPAPER_IMG)
		{
			if (idle_screen_wallpaper_ID == SysWpIDList[i])
			{
				//do 
				//{
					i++;
					i %= MAX_WALLPAPER_IMG;
					ImgId = SysWpIDList[i];
				//} while (ImgId == IMG_ID_PHNSET_WP_7);
				break;
			}
			else if (SysWpIDList[i] == 0)
				break;
			i++;
		}		
		if (ImgId == 0)
			ImgId = SysWpIDList[0];
			//playRequestedTone(KEYPAD_TONE);
			//TurnOnBacklight(0); 
			//motion_sensor_shake(TRUE);
			PhnsetSetWallpaperByImgID(ImgId);
			//TurnOffBacklight();

	}
}

#endif /*MOTION_SENSOR_SUPPORT*/

#endif /* _WALLPAPER_C */




