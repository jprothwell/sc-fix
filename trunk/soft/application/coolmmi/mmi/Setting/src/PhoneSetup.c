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
 *  PhoneSetup.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  This file is intends for phone setup application
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

	FILENAME	: PhoneSetup.c

  	PURPOSE		: nil

 

	AUTHOR		: Vanita Jain

	DATE		: May 29, 2003

**************************************************************/
#ifndef _PHONESETUP_C
#define _PHONESETUP_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "custdatares.h"
#include "custmenures.h"
#include "eventsgprot.h"
#include "debuginitdef.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "commonscreens.h"
#include "globaldefs.h"
#include "unicodexdcl.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "settingsgexdcl.h"
#include "settinggprots.h"
#include "wpssprotos.h"
#include "athandlerprot.h" /* sendatsetgreeting() */
#include "mainmenudef.h" /* main_menu_title_settings_icon */
#include "queuegprot.h" /* osl send message */
#include "settingprot.h"  /* phonesetup prototype */
#include "csp.h" /* csp_value_added_services_service_group, csp_language */
#include "settingprofile.h" /* warning_tone */
#include "settingstructs.h" //added by zhoumn@2007/07/24

#include "profilegprots.h" /* playrequestedtone() */
#include "messagesexdcl.h" /* getchildmenuidindexbyparentmenuid()  */
#include "scheduledpoweronoff.h"
#include "screensaverdefs.h"
#include "wallpaperdefs.h"
#include "downloaddefs.h"
#include "phonesetup.h"
#include "phonesetupgprots.h"
#include "conversions.h"
#include "gpioinc.h" /* backlight */
#include "mmi_trace.h"
#include "drv.h"
#include "lcd_sw.h"

#include "gbhmi.h"//added by zhoumn for the bug of editor full @2007/06/16

#include "shortcutsdefs.h" //add by panxu for short cut


#ifdef __MMI_TOUCH_SCREEN__
#include "touch_screen_cfg.h"
#include "touchscreengprot.h"
#endif /* __MMI_TOUCH_SCREEN__ */

#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
#include "wgui_datetime.h"
#endif

#ifdef __MMI_AUTO_LANG_EN__
#include "custresdef.h"
#include "sscstringhandle.h"
#endif

#ifdef MMI_ON_HARDWARE_P
#include "lcd_if.h"
#endif

#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
   #include "shortcutsprots.h"
#endif

#ifdef __MMI_PREFER_INPUT_METHOD__
   #include "preferedinputmethodprot.h"   /* getinputmethodindex(),  initpreferedinputmethod() */
#endif

/*  Include: PS header file */
#ifdef MMI_ON_HARDWARE_P
   #include "l4dr.h"
   #include "lcd_sw.h"
   #include "lcd_sw_inc.h"
   #include "sublcd.h"
#endif

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif

#ifdef __MMI_TVOUT__
	#include "mdi_tv.h"
#endif /* __MMI_TVOUT__ */
#include "simdetectiondef.h"


#undef __NEWSIMULATOR 

/* 
** Define
*/

/* 
** Typedef 
*/
typedef struct _AutoLang {
	l4c_cbs_language_enum LanType;
	S8* LanSSC;
} StructAutoLang;


/* 
** Local Variable
*/
static U8 *greet_text_on_off[2];

#ifdef __MMI_AUTO_LANG_EN__
static StructAutoLang auto_lang_struct[] = {

	/* ISO 639 2-alphabet Language Codes */
	{ CBS2_ZH, SSC_SCHINESE },
	{ CBS2_TW, SSC_TCHINESE },
	{ CBS0_ENGLISH, SSC_ENGLISH },
	{ CBS0_SPANISH, SSC_SPANISH},
	{ CBS2_TH, SSC_THAI},
	{ CBS0_FRENCH, SSC_FRENCH },
	{ CBS0_GERMAN, SSC_GERMAN },
	{ CBS0_ITALIAN, SSC_ITALIAN },
	{ CBS0_PORTUGUESE, SSC_PORTUGUESE },
	{ CBS1_RUSSIAN, SSC_RUSSIAN },
	{ CBS0_TURKISH, SSC_TURKISH },
	{ CBS2_VI, SSC_VIETNAMESE },
	{ CBS2_ID, SSC_INDONESIAN },
	{ CBS2_MS, SSC_MALAY },
	{ CBS1_ARABIC, SSC_ARABIC },
	
	{ CBS0_LANG_UNSPECIFIED , NULL /* Language UnSpecificed */ },
	{ CBS_INVALID , NULL /* Language UnSpecificed */ }
};

#define MMI_NUM_OF_AUTOLANG_ELEM   (sizeof(auto_lang_struct) / sizeof(auto_lang_struct[0]))
#endif /* __MMI_AUTO_LANG_EN__ */

#ifdef __MMI_TOUCH_SCREEN__

//#include "nvram_user_defs.h" /* for nvram_cache_size */

/* Handwriting setting */
#define MAX_PEN_SPEED 3
#define MAX_PEN_COLORS 8

#define DEFAULT_PEN_SPEED 1
#define DEFAULT_PEN_COLOR 1

 
// #define MAX_PEN_CALIBRATION_ERROR	6

#ifdef __MMI_HAND_WRITING__
static S32 gInlinePenSpeed;
static S32 gInlinePenColorIndex;

static U8 *g_pen_speed_str_list[MAX_PEN_SPEED];
static const color_t g_pen_color_list[MAX_PEN_COLORS] = 
{
	{0,255,255},



	{255,0,0},

	{0,255,0},
	{255,255,0},
	{255,0,255},
	{255,128,0},
	{0,0,128},
	{0,0,0}
};
#endif
/* Calibration */
#define NUM_CALIBRATION_POINTS 3

static PHNSET_CALIBRATION_STATE_ENUM g_phnset_calibration_screen_state = CALIBRATION_NULL_STATE;
static BOOL g_PenCalibrationFinished = TRUE;

#endif /* __MMI_TOUCH_SCREEN__ */

/* 
** Global Variable
*/
PHNSET_CNTX g_phnset_cntx;
PHNSET_CNTX *g_phnset_cntx_p = &g_phnset_cntx;



/* Lagnuage */
extern sLanguageDetails *gLanguageArray;
extern U16 gCurrLangIndex;
extern U8 *ItemType[8];
extern U8 gPLMN[MMI_SIM_NUMBER][MAX_PLMN_LEN_MMI+1];
extern PU8 subMenuDataPtrs[MAX_SUB_MENUS];


#ifdef __MMI_PREFER_INPUT_METHOD__
   extern U8*  inputMethodList[9];
   extern U16   currInputMethodIndex;
#endif

#ifdef MMI_ON_HARDWARE_P
   extern kal_uint8 MainLCD_Contrast_Level_Info[15];
   extern kal_uint8 SubLCD_Contrast_Level_Info[15];
#endif

#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
extern U8 change_window_style;
extern U8 enablechange_window_flag;
#endif

#if defined(MMI_2D_SENSOR_SUPPORT)||defined(CAM_MD_SUPPORT)
//for IS_IDLE_ARRIVED
#include "simdetectiongexdcl.h"
void HighlightPhnset2dSensor(void);
extern void csd_2d_Sensor_switch_on(void);
extern void csd_2d_Sensor_irq_on(void);
extern void csd_2d_Sensor_switch_off(void);
extern UINT8 g_gpio_4_value,g_gpio_5_value;
extern void PhnsetSetWallpaperByImgID(U16 imgID);
extern S32 is_on_idlescreen(void);
#endif
#ifdef __MMI_SCREEN_ROTATE__
extern BOOL csd_mmi_is_screen_rotate_enable(void);
#endif 

/* 
** Global Function
*/
extern void SetProfileSettingCustomFuns (void);
extern void RefreshNwProviderName(U8 *UpdatePlmn, U8 simID);
extern void RefreshSlaveNwProviderName(U8 *UpdatePlmn);

extern void HighlightShctOptionEdit();
extern void HighlightShctOptionAdd();
extern void HighlightShctOptionDelete();
extern void ShowCategory155Screen(U16 title,U16 title_icon,
										U16 left_softkey,U16 left_softkey_icon,
										U16 right_softkey,U16 right_softkey_icon,
										U16 bg_image_ID,U16 message,S32 l_limit,
										S32 u_limit,S32 *current_value,U8* history_buffer);
extern void RegisterCategory155ChangeCallback(void (*f)(s32 value));

#ifdef __MMI_PREFER_INPUT_METHOD__
extern U16 GetInputMethodIndex(U16 inputType);
#endif

#if defined(MOTION_SENSOR_SUPPORT)
extern void HighlightPhnsetMotionSensor(void);
#endif

/*****************************************************************************
* Local Function 
*****************************************************************************/
#ifdef __MMI_TVOUT__
	static void mmi_phnset_highlight_tvout(void);
	static void mmi_phnset_entry_tvout_screen(void);
	static void mmi_phnset_highlight_tvout_format(void);
	static void mmi_phnset_highlight_tvout_style(void);
	static void mmi_phnset_hint_tvout_format(U16 index);
	static void mmi_phnset_hint_tvout_style(U16 index);
	static void mmi_phnset_entry_tvout_format_screen(void);
	static void mmi_phnset_entry_tvout_style_screen(void);
	static void mmi_phnset_init_tvout(void);
	static void mmi_phnset_tvout_event_hdlr(MDI_RESULT result);
#endif /* __MMI_TVOUT__ */

#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
void HighlightPhnsetChangeWinStyle(void);
#endif

void PhnsetReadMainCntrstSetting(void)
{
	S16 error;
		ReadValue( NVRAM_SETTING_CONTRAST_LEVEL, &(g_phnset_cntx_p->MainCntrst), DS_BYTE , &error);
		if(g_phnset_cntx_p->MainCntrst>5)
		{
			g_phnset_cntx_p->MainCntrst = 5;
			WriteValue( NVRAM_SETTING_CONTRAST_LEVEL, &(g_phnset_cntx_p->MainCntrst), DS_BYTE , &error);
		}
    if(g_phnset_cntx_p->MainCntrst<1)
		{
			g_phnset_cntx_p->MainCntrst = 1;
			WriteValue( NVRAM_SETTING_CONTRAST_LEVEL, &(g_phnset_cntx_p->MainCntrst), DS_BYTE , &error);
		}
}
/*****************************************************************************
* FUNCTION
*  InitPhoneSetupCntx
* DESCRIPTION
*   This function is to initialize phone setup context
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void InitPhoneSetupCntx(void) 
{
	S16 Err=0;

   /* Common */
   g_phnset_cntx_p->curHightlightItem = 0;
   g_phnset_cntx_p->ScrAnchor = IDLE_SCREEN_ID;

   /*Set Time and Date*/
   g_phnset_cntx_p->currentCity = 0;
   g_phnset_cntx_p->CurHomeCityStrID = 0;
   g_phnset_cntx_p->CurHomeCityStrPtr = 0;
   g_phnset_cntx_p->CurTimeFmt = 0;
   g_phnset_cntx_p->CurDateFmt = 0;
   g_phnset_cntx_p->AM_PM_flag = NULL;
   
   ReadValue(NVRAM_SETTING_DT_DST, &(g_phnset_cntx_p->currentDST), DS_BYTE, &Err);
   if (g_phnset_cntx_p->currentDST == 0xff)
   {
   	g_phnset_cntx_p->currentDST = 0;
   	WriteValue(NVRAM_SETTING_DT_DST, &g_phnset_cntx_p->currentDST, DS_BYTE, &Err);
   }
   
   memset( &(g_phnset_cntx_p->settime),  0, sizeof(PHNSET_DATETIME_STRUCT) );

   /* NITZ */
   g_phnset_cntx_p->NITZDirtyFlag = MMI_FALSE;
   g_phnset_cntx_p->curNITZStatus = 0;

   /* Phone Setup */
    g_phnset_cntx_p->ShowOwnNumDirtyFlag = MMI_FALSE;
    g_phnset_cntx_p->ShowDtDirtyFlag = MMI_FALSE;
   // g_phnset_cntx_p->MainCntrst = 0;
   PhnsetReadMainCntrstSetting();
    g_phnset_cntx_p->curShowDtStatus = 0;
    g_phnset_cntx_p->curShowOwnNumStatus = 0;
//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)
    g_phnset_cntx_p->curShowOPNStatus = 0;
#endif

   #ifdef __MMI_SUBLCD__
    g_phnset_cntx_p->SubCntrst = 0;
   #endif
   #ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
    g_phnset_cntx_p->MenuStyle = 0;
   #endif
   #ifdef __MMI_SET_DEF_ENCODING_TYPE__
   g_phnset_cntx_p->EncodingType= PHNSET_ENCOD_BIG5;
   #endif

   /* Language */
   g_phnset_cntx_p->ppLang = NULL;
   g_phnset_cntx_p->UsrLangIdx = 0;
   g_phnset_cntx_p->LP = CBS0_LANG_UNSPECIFIED;

   /* Wallpaper & Screen Saver */
    g_phnset_cntx_p->curtWpID = 0;
   #ifdef __MMI_SUB_WALLPAPER__
    g_phnset_cntx_p->curtSubWpID = 0;
   #endif
    g_phnset_cntx_p->WPSSPreviewScrFlag = 1;
    g_phnset_cntx_p->WPFctyImgNum = 0;
    g_phnset_cntx_p->SSFctyImgNum = 0;
    g_phnset_cntx_p->WPDwnlImgNum = 0;
    g_phnset_cntx_p->SSDwnlImgNum = 0;
    g_phnset_cntx_p->curSSStatus = 0 ;
    g_phnset_cntx_p->curSSWaitTime = 0;
    g_phnset_cntx_p->SSStatusInlineHilite = 0;
    g_phnset_cntx_p->SSWaitStatusInlineHilite = 2;
	g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_WP;

    memset(g_phnset_cntx_p->SysWPSSList, 0, sizeof(g_phnset_cntx_p->SysWPSSList) );  
    memset(g_phnset_cntx_p->DwnlWPSSList, 0, sizeof(g_phnset_cntx_p->DwnlWPSSList)  );
   
 #if 0 // zrx del 20060612   
#ifdef __MMI_SET_DEF_ENCODING_TYPE__
	SetHiliteHandler(MENU_SETTING_DEF_ENCODING_TYPE,HighlightPhnsetDefEncoding);
	SetHintHandler(MENU_SETTING_DEF_ENCODING_TYPE, HintPhnsetDefEncoding);
	ReadValue( NVRAM_SETTING_DEF_ENCODING, &(g_phnset_cntx_p->EncodingType), DS_BYTE, &Err );
 	 if( g_phnset_cntx_p->EncodingType == 0xFF || Err != NVRAM_READ_SUCCESS )
	   g_phnset_cntx_p->EncodingType = PHNSET_ENCOD_BIG5;


   switch( g_phnset_cntx_p->EncodingType )
   {
      case PHNSET_ENCOD_BIG5: 
           #ifdef __MMI_CHSET_BIG5__
           g_chset_text_encoding = MMI_CHSET_BIG5;
           #else
           ASSERT(0); /* Encoding type not support */
           #endif
      	    break;
      case PHNSET_ENCOD_GB2312: 
#ifdef __MMI_CHSET_GB2312__
          g_chset_text_encoding = MMI_CHSET_GB2312;
#else
          ASSERT(0); /* Encoding type not support */
#endif
      	    break;
      default: 
           g_chset_text_encoding =MMI_CHSET_BIG5;
         break;
   }
   

#else  /* Set default to BIG5 */
		g_chset_text_encoding = MMI_CHSET_BIG5;
#endif /* __MMI_SET_DEF_ENCODING_TYPE__ */
#endif

#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
    ReadValue(NVRAM_SET_MAINMENU_STYLE, &(g_phnset_cntx_p->MenuStyle), DS_BYTE, &Err  );
 	 if( g_phnset_cntx_p->MenuStyle == 0xFF || Err != NVRAM_READ_SUCCESS )
	   g_phnset_cntx_p->MenuStyle = 0;
#endif

#ifdef __MMI_TOUCH_SCREEN__
	ReadValue(NVRAM_PEN_SPEED,&(g_phnset_cntx_p->pen_speed),DS_BYTE,&Err);
	if(g_phnset_cntx_p->pen_speed == 0xFF || Err != NVRAM_READ_SUCCESS || g_phnset_cntx_p->pen_speed >= MAX_PEN_SPEED)
		g_phnset_cntx_p->pen_speed = DEFAULT_PEN_SPEED;
		
	ReadValue(NVRAM_PEN_COLOR,&(g_phnset_cntx_p->pen_color),DS_BYTE,&Err);
	if(g_phnset_cntx_p->pen_color== 0xFF || Err != NVRAM_READ_SUCCESS || g_phnset_cntx_p->pen_color >= MAX_PEN_COLORS)
		g_phnset_cntx_p->pen_color = DEFAULT_PEN_COLOR;
#endif /* __MMI_TOUCH_SCREEN__ */

//luanyy add for tc9751 start
#if defined (__LED__) || defined (__MMI_RUNNING_LED__)	
        ReadValue(NVRAM_PHN_ENABLE_SCROLL_LIGHT, &(g_phnset_cntx_p->curEnableRollLedStatus), DS_BYTE, &Err);
	   if (g_phnset_cntx_p->curEnableRollLedStatus == 0xff)
	   {
	   	g_phnset_cntx_p->curEnableRollLedStatus = 1;
	   	WriteValue(NVRAM_PHN_ENABLE_SCROLL_LIGHT, &g_phnset_cntx_p->curEnableRollLedStatus, DS_BYTE, &Err);
	   }
#endif
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
ReadValue( NVRAM_CHANGE_SCREEN_MODE, &change_window_style, DS_BYTE , &Err);
if(change_window_style == 0xff)
{
change_window_style = 9;
WriteValue( NVRAM_CHANGE_SCREEN_MODE, &change_window_style, DS_BYTE , &Err);
}

if(change_window_style == 9)
{
	enablechange_window_flag = 0;
}
else
{
	enablechange_window_flag = 1;
}
#endif
	#ifdef __FT_IDLE_SMART_DIAL__
	InitDialStyle();
	#endif

//luanyy add for tc9751 end

}


/*****************************************************************************
* FUNCTION
*  InitPhoneSetupApp
* DESCRIPTION
*   This function is to register highlight handler for Phonesetup
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void InitPhoneSetupApp(void) {
TRACE_SYS_FUNCTION();

	PRINT_INFORMATION(("File: [%s]  Line: [%d] <<InitNetworkSetupApp.>\n",__FILE__,__LINE__));
    
 
#ifdef __MMI_DISP_SETUP_LEVEL_UP__
	SetHiliteHandler(MENU_DISP_SETUP,HighlightDispSetup);
#endif

	//SetHiliteHandler(MENU_SETTING_SPEED_DIAL,  HighlightPhbSpeedDial);  /* Move to SpeedDial.c */
#if !defined(__MMI_NO_CONTRAST_LEVEL__)
	SetHiliteHandler(MENU_SETTING_CONTRAST_LEVEL,HighlightPhnsetContrastLevel);
#endif
#ifdef __FT_IDLE_SMART_DIAL__
	SetHiliteHandler(MENU_ID_PHNSET_SMART_DIAL_STYLE, HighlightPhnsetDialStyle);
	SetHintHandler(MENU_ID_PHNSET_SMART_DIAL_STYLE, HintPhnsetDialStyle);
#endif

	SetHiliteHandler(MENU_SETTING_SCHEDULE_POWER,HighlightSPOF);
	SetHiliteHandler(MENU_IDLE_SCR_DISP,HighlightPhnsetIdleScrDisp);
	SetHiliteHandler(MENU_SETTING_SHOW_OWN_NUMBER,HighlightPhnsetShowOwnNo);
	//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)	
	SetHiliteHandler(MENU_SETTING_SHOW_OPN,HighlightPhnsetShowOPN);
#endif	
	SetHiliteHandler(MENU_SETTING_SHOW_DT_TIME,HighlightPhnsetShowDtTime);
	//SetHiliteHandler(547,HighlightPhnsetEnableRollLed); //luanyy add for tc9751
	//SetHiliteHandler(MITEM113_PBOOK_SPEED_DIAL,HighlightPhbSpeedDialSetNum); /* Move to SpeedDial.c */
	//SetHiliteHandler(MENU_SETTING_SPEED_STATUS,HighlightPhbSpeedDialStatus); /* Move to SpeedDial.c */
	SetHiliteHandler(MENU_SETTING_LANGUAGE,  HighlightPhnsetLang);

#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
	SetHiliteHandler(MENU_SETTING_CHANGE_WIN_WITH_DYNAMIC,  HighlightPhnsetChangeWinStyle);
#endif

	SetHiliteHandler(MENU9146_WELCOME_TEXT,HighlightPhnsetWelcomeText);
	SetHiliteHandler(MENU_SETTING_SCR_SVR, HighlightPhnsetScreenSaver );
	SetHiliteHandler(MENU_SETTING_WALLPAPER,  HighlightPhnsetMainWallpaper);
	#ifdef __MMI_SUB_WALLPAPER__
	SetHiliteHandler(MENU_SETTING_WALLPAPER_SUB,HighlightPhnsetSubWallpaper);
	#endif

#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
	SetHintHandler(MENU_SETTING_MAINMENU_STYLE,HintPhnsetMainMenuStyle);

	SetHiliteHandler( MENU_SETTING_MAINMENU_STYLE, HighlightPhnsetMainMenuStyle );
	SetHiliteHandler( MENU_SETTING_MAINMENU_STYLE_MATRIX, HighlightPhnsetMainMenuStyleSelect );
	SetHiliteHandler( MENU_SETTING_MAINMENU_STYLE_PAGE, HighlightPhnsetMainMenuStyleSelect );
	SetHiliteHandler( MENU_SETTING_MAINMENU_STYLE_CIRCULAR, HighlightPhnsetMainMenuStyleSelect );
	SetHiliteHandler( MENU_SETTING_MAINMENU_STYLE_ROTATE, HighlightPhnsetMainMenuStyleSelect );	 
	SetHiliteHandler( MENU_SETTING_MAINMENU_STYLE_LIST, HighlightPhnsetMainMenuStyleSelect );
#endif
#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
	SetHintHandler(MENU_SETTING_IDLE_CLOCK_TYPE,HintPhnsetIdleClockType);
	SetHiliteHandler( MENU_SETTING_IDLE_CLOCK_TYPE, HighlightPhnsetIdleClockType);
#endif

#ifndef __MMI_HIDE_HOMECITY__
	SetHiliteHandler(MENU9142_SETHOMECITY,  HighlightPhnsetSetHomeCity);
#endif

#ifndef __MMI_SEPARATE_DT_SETTING__
	SetHiliteHandler(MENU9143_SETTIMEANDDATE, HighlightPhnsetSetDT);
	SetHiliteHandler(MENU9144_SETTIMEANDDATEFORMAT, HighlightPhnsetSetFormat);
#else
	SetHiliteHandler(MENU_ID_SETTING_DATEANDFORMAT, HighlightPhnsetSetDateAndFormat);
	SetHiliteHandler(MENU_ID_SETTING_TIMEANDFORMAT, HighlightPhnsetSetTimeAndFormat);
#endif
 
#ifdef __FLIGHT_MODE_SUPPORT__
	SetHiliteHandler(MENU_SETTING_FLIGHT_MODE,  HighlightPhnsetFlightMode);
#endif
 

#ifndef __MMI_HIDE_HOMECITY__
	SetHintHandler((U16)MENU9142_SETHOMECITY, HintPhnsetTimeAndDate);
#endif

	SetHintHandler(MENU9146_WELCOME_TEXT,HintPhnsetWelcomeText);
	SetHintHandler(MENU_SETTING_LANGUAGE,HintPhnsetLanguage);


	SetHintHandler(MENU_SETTING_SHOW_OWN_NUMBER,HintPhnsetOwnerNum);
	//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)	
	SetHintHandler(MENU_SETTING_SHOW_OPN,HintPhnsetOPN);
#endif

	SetHintHandler(MENU_SETTING_SHOW_DT_TIME,HintPhnsetShowDateTime);
#if defined (__LED__) || defined (__MMI_RUNNING_LED__)	
    SetHintHandler(MENU_SETTING_ENABLE_ROLL_LED,HintPhnsetEnableRollLed);  
    SetHiliteHandler(MENU_SETTING_ENABLE_ROLL_LED,HighlightPhnsetEnableRollLed);
#endif
	SetHintHandler(MENU_SETTING_WALLPAPER,HintPhnsetWallpaper);
	#ifdef __MMI_SUB_WALLPAPER__
	SetHintHandler(MENU_SETTING_WALLPAPER_SUB,HintPhnsetSubWallpaper);
	#endif
	SetHintHandler(MENU_SETTING_SCR_SVR,HintPhnsetScreenSaver);

#ifdef __DAILY_WALLPAPER__	
	SetHiliteHandler(MENU_SETTING_WALLPAPER_FIX,  HighlightPhnsetWallpaperFix);
	SetHintHandler( MENU_SETTING_WALLPAPER_ROTATE, HintPhnsetWallpaperRotate );
#endif

#ifdef __MMI_NITZ__
#ifdef __MMI_AUTOTIMEZONE_IN_DATEANDTIME__
	SetHintHandler(MENU9145_SETTIMEANDDATENITZ,HintPhnsetNITZ);
	SetHiliteHandler(MENU9145_SETTIMEANDDATENITZ,HighlightPhnsetNITZ);
#else
	SetHintHandler(MENU_SETTING_MENU_TREE,HintPhnsetNITZ);
	SetHiliteHandler(MENU_SETTING_MENU_TREE,HighlightPhnsetNITZ);
#endif	
#endif

#ifdef __MMI_PREFER_INPUT_METHOD__
	SetHintHandler(MENU_SETTING_PREFERED_INPUT_METHOD,HintPhnsetPreferedInputMethod);
#endif

#ifdef __MMI_DEDICATED_KEY_SHORTCUTS__
	SetHiliteHandler(MENU_SETTING_DEDICATED_KEYS,HighlightDedicatedKeys);
#endif

	//short cut add by panxu 2007-8-15
	SetHiliteHandler(EXTRA_SHORTCUTS_OPTION_EDIT_MENUID, HighlightShctOptionEdit);
	SetHiliteHandler(EXTRA_SHORTCUTS_OPTION_DELETE_MENUID, HighlightShctOptionDelete);
	SetHiliteHandler(EXTRA_SHORTCUTS_OPTION_ADD_MENUID, HighlightShctOptionAdd);

#ifdef __MMI_TOUCH_SCREEN__
	#ifdef __MMI_HAND_WRITING__
	SetHiliteHandler(MENU_SETTING_HANDWRITING,HighlightHandwritingSetting);
	#endif
	SetHiliteHandler(MENU_SETTING_CALIBRATION,HighlightCalibrationSetting);
#endif

#ifdef __MMI_POWER_ON_OFF_DISPLAY__
	SetHiliteHandler(MENU_SETTING_POWER_ON_DISPLAY,HighlightPhnsetPowerOn);
	SetHintHandler(MENU_SETTING_POWER_ON_DISPLAY,HintPhnsetPwrOnDisplay);
	SetHiliteHandler(MENU_SETTING_POWER_OFF_DISPLAY,HighlightPhnsetPowerOff);
	SetHintHandler(MENU_SETTING_POWER_OFF_DISPLAY,HintPhnsetPwrOffDisplay);
#endif

 
#ifdef __FLIGHT_MODE_SUPPORT__
	SetHintHandler(MENU_SETTING_FLIGHT_MODE,  HintPhnsetFlightMode);
#endif
 

#ifdef __MMI_TVOUT__
	SetHiliteHandler(MENU_ID_SETTING_TVOUT, mmi_phnset_highlight_tvout);
	SetHiliteHandler(MENU_ID_SETTING_TVOUT_FORMAT, mmi_phnset_highlight_tvout_format);
	SetHiliteHandler(MENU_ID_SETTING_TVOUT_MM_STYLE, mmi_phnset_highlight_tvout_style);	

	SetHintHandler(MENU_ID_SETTING_TVOUT_FORMAT, mmi_phnset_hint_tvout_format);
	SetHintHandler(MENU_ID_SETTING_TVOUT_MM_STYLE, mmi_phnset_hint_tvout_style);	
#endif /* __MMI_TVOUT__ */
#if defined(MMI_2D_SENSOR_SUPPORT)|| defined(CAM_MD_SUPPORT)
	SetHiliteHandler(MENU_SETTING_2D_SENSOR_SWITCH,HighlightPhnset2dSensor);
#endif
#if defined(MOTION_SENSOR_SUPPORT)
	SetHiliteHandler(MENU_SETTING_MOTION_SENSOR,HighlightPhnsetMotionSensor);
#endif


}



/*****************************************************************************
* FUNCTION
*  EntryPhnsetScr
* DESCRIPTION
*   This function is Entry function for Phonesetup
*   Following functions: HintPhnsetTimeAndDate
*                                HighlighPhnsettSPOF
*                                HighlightPhnsetLang
*                                HintPhnsetPreferedInputMethod
*                                HighlightPhnsetContrastLevel
*                                HighlightPhnsetWelcomeText
*                                HighlightPhbSpeedDial
*                                HighlightPhnsetNITZ
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
void EntryPhnsetScr(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem = 0 ;      /* Stores no of children in the submenu*/
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer = 0;       /* Buffer holding history data */
	U8 *ItemType_t[MAX_SUB_MENUS]={0x00};
	/*U32 maskingByte=-1;
	U32 menuItemId=-1;*/
		#ifdef __MMI_NUMBER_MENU__
#else
				U16 ItemIcons[MAX_SUB_MENUS];
#endif

	TBM_ENTRY(0x28D2);
	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR9003_INITIALSETUP, ExitPhnsetScr, EntryPhnsetScr, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR9003_INITIALSETUP);	


	
#if 1 //modified by zhoumn@2007/07/24
	if (HasValidSIM())
	{
		nNumofItem = GetNumOfChild_Ext(MENU9102_INITIAL_SETUP);
		GetSequenceStringIds_Ext(MENU9102_INITIAL_SETUP,nStrItemList);
#ifdef __MMI_NUMBER_MENU__
#else
		GetSequenceImageIds_Ext(MENU9102_INITIAL_SETUP,ItemIcons);
#endif
		SetParentHandler(MENU9102_INITIAL_SETUP);
	}
	else
	{
		nNumofItem = GetNumOfChild_Ext(MENU9102_INITIAL_SETUP_NO_SIM);
		GetSequenceStringIds_Ext(MENU9102_INITIAL_SETUP_NO_SIM,nStrItemList);
		#ifdef __MMI_NUMBER_MENU__
#else
		GetSequenceImageIds_Ext(MENU9102_INITIAL_SETUP_NO_SIM,ItemIcons);
		#endif
		SetParentHandler(MENU9102_INITIAL_SETUP_NO_SIM);
	}

#else
	/* 3. Retrieve no of child of menu item to be displayed */
	 
	nNumofItem = GetNumOfChild_Ext(MENU9102_INITIAL_SETUP);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU9102_INITIAL_SETUP);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	 
	GetSequenceStringIds_Ext(MENU9102_INITIAL_SETUP,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU9102_INITIAL_SETUP);
#endif
	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

		/* 8 Construct the Hints List */

#if !defined(__MMI_NOKIA_STYLE_N800__)
        
	if (HasValidSIM())
	{
		ConstructHintsList(MENU9102_INITIAL_SETUP, ItemType_t);
	}
	else
	{
		ConstructHintsList(MENU9102_INITIAL_SETUP_NO_SIM, ItemType_t);
	}
#endif

#ifdef __MMI_NUMBER_MENU__

	ShowCategory52Screen(  STR_MENU9102_INITIAL_SETUP,MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList,(PU16)gIndexIconsImageList,
#ifdef __MMI_NOKIA_STYLE_N800__
						  0,
#else
                                                  ItemType_t,
#endif
						  0,0,guiBuffer);

#else
	ShowCategory52Screen(  STR_MENU9102_INITIAL_SETUP,MAIN_MENU_TITLE_SETTINGS_ICON,
					  STR_GLOBAL_OK, IMG_GLOBAL_OK,
					  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					  nNumofItem,nStrItemList,ItemIcons,
					  ItemType_t,
					  0,0,guiBuffer);

#endif
	//zhangm bug9643 2008-9-5
	register_fixed_list_shortcut_handler();
	//zhangm bug9643 2008-9-5
	
	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	TBM_EXIT(0x28D2);
}




/*****************************************************************************
* FUNCTION
*  ExitPhnsetScr
* DESCRIPTION
*   This function is Exit function for Phonesetup
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitPhnsetScr(void)
{

#if (  defined(__MMI_NITZ__) && !defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__) )
	S16 error;

	/* Only Write To NVRAM if the value is changes for ON/OFF Keys*/
	if( g_phnset_cntx_p->NITZDirtyFlag == TRUE )
	{
		WriteValue( NVRAM_SETTING_AUTOUPDATE_DT_TIME, &(g_phnset_cntx_p->curNITZStatus), DS_BYTE , &error);
		g_phnset_cntx_p->NITZDirtyFlag = FALSE;
	}
#endif

//luanyy add for tc9751 start
#ifdef __MMI_RUNNING_LED__
	{
		S16 Err;
		WriteValue(NVRAM_PHN_ENABLE_SCROLL_LIGHT, &(g_phnset_cntx_p->curEnableRollLedStatus), DS_BYTE, &Err);
	}
#endif
//luanyy add for tc9751 end
}


/*****************************************************************************
* FUNCTION
*  HighlightPhnsetScr
* DESCRIPTION
*   This function is highlight handler for Phonesetup
*   Follow functions: EntryPhnsetScr
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetScr(void)
{
	TRACE_SYS_FUNCTION();
	
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryPhnsetScr,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetScr, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


 
#ifdef __MMI_DISP_SETUP_LEVEL_UP__

/*****************************************************************************
* FUNCTION
*  HighlightDispSetting
* DESCRIPTION
*   This function is highlight handler for Display Setup menu item (same level with pPhone Setup) 
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
void HighlightDispSetup(void)
{
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/*Register function for left soft key */
	SetLeftSoftkeyFunction(EntryDispSetup,KEY_EVENT_UP);
	SetKeyHandler(EntryDispSetup, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  EntryDispSetup
* DESCRIPTION
*   This function is Entry function for Display Setup
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
void EntryDispSetup(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem = 0;      /* Stores no of children in the submenu*/
	U16 nDispAttribute = 0;  /* Stores display attribue */
	U8* guiBuffer = NULL;       /* Buffer holding history data */
	U8 *ItemType_t[MAX_SUB_MENUS];
	//U32 maskingByte=-1;
	//U32 menuItemId=-1;

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_DISP_SETUP, NULL, EntryDispSetup, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_DISP_SETUP);

   /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_DISP_SETUP);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_DISP_SETUP);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_DISP_SETUP,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_DISP_SETUP);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* 8 Construct the Hints List */
	ConstructHintsList(MENU_DISP_SETUP, ItemType_t);

	ShowCategory52Screen(STR_DISP_SETUP, MAIN_MENU_TITLE_SETTINGS_ICON,
						      STR_GLOBAL_OK, IMG_GLOBAL_OK,
						      STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						      nNumofItem,nStrItemList,(PU16)gIndexIconsImageList,
						      ItemType_t,
						      0,0,guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	/* 10. Register Exit handler */
	//SetExitHandler(SCR9003_INITIALSETUP, ExitPhnsetScr);
	//SetGenericExitHandler( SCR_DISP_SETUP, NULL, EntryDispSetup);
}




#endif /* __MMI_DISP_SETUP_LEVEL_UP__ */


/*****************************************************************************
* FUNCTION
*  HighlightPhnsetIdleScrDisp
* DESCRIPTION
*   This function is highlight handler for Phonesetup-> IdleScreenDisplay
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
void HighlightPhnsetIdleScrDisp(void)
{
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/*Register function for left soft key */
	SetLeftSoftkeyFunction(EntryPhnsetIdleScrDisp,KEY_EVENT_UP);
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(EntryPhnsetIdleScrDisp, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	/*Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
}




/*****************************************************************************
* FUNCTION
*  EntryPhnsetIdleScrDisp
* DESCRIPTION
*   This function is Exit function for Phonesetup->IdleScreenDisplay
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryPhnsetIdleScrDisp(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8 *IdleScrDispType[MAX_SUB_MENUS];
		#ifdef __MMI_NUMBER_MENU__
	#else
				U16 ItemIcons[MAX_SUB_MENUS];
#endif

	TBM_ENTRY(0x28D1);
	/*Call Exit Handler*/
	EntryNewScreen(SCR_IDLE_SCR_DISP, ExitPhnsetIdleScrDisp, EntryPhnsetIdleScrDisp, NULL);

	/*Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_IDLE_SCR_DISP);
//chenqiang add for not display main menu style when no sim card  begain
#ifdef __MMI_MULTI_SIM__
	mmi_frm_unhide_menu_item(MENU_SETTING_MAINMENU_STYLE);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined (__MMI_MAINLCD_220X176__) || defined(__MMI_SYMBIAN_STYLE_IDLE__) 
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	mmi_frm_hide_menu_item(MENU_SETTING_SHOW_OWN_NUMBER);
#else 
	#if !((defined(__MMI_MULTI_SIM__))&&(defined( __MMI_MAINLCD_128X160__)  || defined(__MMI_MAINLCD_160X128__)))                              
	mmi_frm_unhide_menu_item(MENU_SETTING_SHOW_OWN_NUMBER);           
	#endif  
#endif  
	
	if(MTPNP_AD_Get_UsableSide_Number() == 0)
	{
		mmi_frm_hide_menu_item(MENU_SETTING_MAINMENU_STYLE);
#if !((defined(__MMI_MULTI_SIM__))&&(defined( __MMI_MAINLCD_128X160__) || defined(__MMI_MAINLCD_160X128__)))     
		mmi_frm_hide_menu_item(MENU_SETTING_SHOW_OWN_NUMBER);
		#endif  
	}
#endif
//chenqiang add for not display main menu style when no sim card  end
	/*Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext(MENU_IDLE_SCR_DISP);

	/*Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_IDLE_SCR_DISP);

	/*Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext(MENU_IDLE_SCR_DISP,nStrItemList);
	#ifdef __MMI_NUMBER_MENU__
	#else
	GetSequenceImageIds_Ext(MENU_IDLE_SCR_DISP,ItemIcons);
	#endif

	/*Set current parent id*/
	SetParentHandler(MENU_IDLE_SCR_DISP);

	/*Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
	/*If Screen Saver is Off then Set The HintHnadler to NULL instead of
	passing empty screen*/
	if(PhnsetGetCurScrSvrStatus() != SET_SS_STATUS_OFF)
		SetHintHandler(MENU_SETTING_SCR_SVR,HintPhnsetScreenSaver);
	else
		SetHintHandler(MENU_SETTING_SCR_SVR,NULL);

	/*Constrct Hint for Idle Screen Display*/
	ConstructHintsList(MENU_IDLE_SCR_DISP, IdleScrDispType);

	/*Display Category Screen */
	#ifdef __MMI_NUMBER_MENU__
	ShowCategory52Screen(  STR_IDLE_SCR_DISP, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList,(PU16)gIndexIconsImageList,
						  IdleScrDispType,
						  0,0,guiBuffer);

	#else
	
	ShowCategory52Screen(  STR_IDLE_SCR_DISP, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList,ItemIcons,
						  IdleScrDispType,
						  0,0,guiBuffer);
	#endif
	/*Register function with softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	TBM_EXIT(0x28D1);
}



/*****************************************************************************
* FUNCTION
*  ExitPhnsetIdleScrDisp
* DESCRIPTION
*   This function is Exit function for Phonesetup->IdleScreenDisplay
*   Functionality: Save 1. Save Owner number  2. Show date and time status
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitPhnsetIdleScrDisp(void)
{
	S16 error;

	/* Save NVRAM only when value is changed for date time and 
		own type on idle screen*/
	if( g_phnset_cntx_p->ShowDtDirtyFlag == TRUE )
	{
		WriteValue( NVRAM_SETTING_STATUS_DT_TIME, &(g_phnset_cntx_p->curShowDtStatus), DS_BYTE , &error);
		g_phnset_cntx_p->ShowDtDirtyFlag = FALSE;
	}
	if( g_phnset_cntx_p->ShowOwnNumDirtyFlag == TRUE )
	{
		WriteValue( NVRAM_SETTING_OWNER_NO, &(g_phnset_cntx_p->curShowOwnNumStatus), DS_BYTE , &error);
		 g_phnset_cntx_p->ShowOwnNumDirtyFlag = FALSE;
	}
	//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)	
	if( g_phnset_cntx_p->ShowOPNDirtyFlag == TRUE )
	{
		WriteValue( NVRAM_SETTING_OPN_DISPLAY, &(g_phnset_cntx_p->curShowOPNStatus), DS_BYTE , &error);
		 g_phnset_cntx_p->ShowOPNDirtyFlag = FALSE;
	}
#endif	
}

#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
/*****************************************************************************
* FUNCTION
*  HintPhnsetMainMenuStyle
* DESCRIPTION
*   This function is Hint handler for Phonesetup->IdleScreenDisplay --> Mainmenu style
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
void HintPhnsetMainMenuStyle(U16 index)
{
     PS8 p=NULL;
     U8 style = PhnsetGetMainMenuStyle();

     switch( style )
     {
 #ifdef __MMI_MAINMENU_MATRIX_SUPPORT__  
        case PHNSET_MAINMENU_STYLE_MATRIX:
           p=GetString(STR_MAINMENU_STYLE_MATRIX);
           break;
 #endif

 #ifdef __MMI_MAINMENU_PAGE_SUPPORT__  
        case PHNSET_MAINMENU_STYLE_PAGE:
           p=GetString(STR_MAINMENU_STYLE_PAGE);
           break;
 #endif

 #ifdef __MMI_MAINMENU_LIST_SUPPORT__  
        case PHNSET_MAINMENU_STYLE_LIST:
           p=GetString(STR_MAINMENU_STYLE_LIST);
           break;
 #endif
           
#ifdef __MMI_MAINMENU_CIRCULAR_SUPPORT__ 
        case PHNSET_MAINMENU_STYLE_CIRCULE:
           p=GetString(STR_MAINMENU_STYLE_CIRCULAR);
           break;
#endif

#ifdef __MMI_MAINMENU_ROTATE_SUPPORT__
	case PHNSET_MAINMENU_STYLE_ROTATE:
           p=GetString(STR_MAINMENU_STYLE_ROTATE);			 
           break;
#endif

#ifdef __MMI_MAINMENU_TAB_SUPPORT__  
	case PHNSET_MAINMENU_STYLE_TAB:
           p=GetString(STR_MAINMENU_STYLE_TAB);			
           break;
#endif

        default:
           p=GetString(STR_GLOBAL_EMPTY);
           break;
     }
	pfnUnicodeStrcpy((PS8)hintData[index],(PS8)p);
	
}


/*****************************************************************************
* FUNCTION
*  HighlightPhnsetMainMenuStyle
* DESCRIPTION
*   This function is Highlight handler for Phonesetup->IdleScreenDisplay --> Mainmenu style
*   Follow functions: EntryPhnsetMainMenuStyle
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetMainMenuStyle(void)
{
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/*Register function for left soft key */
	SetLeftSoftkeyFunction(EntryPhnsetMainMenuStyle,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetMainMenuStyle, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  EntryPhnsetMainMenuStyle
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay --> Main Menu Style
*   Following functions: HighlightPhnsetMainMenuStyleSelect
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryPhnsetMainMenuStyle(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	//S16 Err=0;
	//U8 *IdleScrDispType[MAX_SUB_MENUS];

	TBM_ENTRY(0x28D3);
	/*Call Exit Handler*/
	EntryNewScreen(SCR_MAINMENU_STYLE, NULL, EntryPhnsetMainMenuStyle, NULL);

	/*Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_MAINMENU_STYLE);
	
	/*Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_SETTING_MAINMENU_STYLE);

	/*Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_SETTING_MAINMENU_STYLE);

	/*Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_SETTING_MAINMENU_STYLE, nStrItemList);

	/*Set current parent id*/
	SetParentHandler(MENU_SETTING_MAINMENU_STYLE);

	/*Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
	/*Display Category Screen */
	ShowCategory11Screen (STR_MAINMENU_STYLE, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, g_phnset_cntx_p->MenuStyle, guiBuffer);

	/*Register function with softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	TBM_EXIT(0x28D3);

}



/*****************************************************************************
* FUNCTION
*  HighlightPhnsetMainMenuStyleSelect
* DESCRIPTION
*   This function is Highlight handler for Phonesetup->IdleScreenDisplay --> Mainmenu style
*   Follow functions: PhnsetMainMenuStyleSelectLskHdlr
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetMainMenuStyleSelect(void)
{
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/*Register function for left soft key */
	SetLeftSoftkeyFunction(PhnsetMainMenuStyleSelectLskHdlr,KEY_EVENT_UP);

	/*Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*  PhnsetMainMenuStyleSelectLskHdlr
* DESCRIPTION
*   This function is left softkey handling function 
*     for Phonesetup->IdleScreenDisplay -> MainMenu style -> Select
*   Functionality: Save the setting menu style
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetMainMenuStyleSelectLskHdlr(void)
{
      U8 HighlightItem=0;
      S16 Err;
      HighlightItem = (U8) GetHighlightedItem();

      if( g_phnset_cntx_p->MenuStyle != HighlightItem )
      {
         g_phnset_cntx_p->MenuStyle = HighlightItem;
         WriteValue( NVRAM_SET_MAINMENU_STYLE, &(g_phnset_cntx_p->MenuStyle), DS_BYTE, &Err );
      }

	 DisplayPopup((U8*)GetString(STR_GLOBAL_DONE),IMG_GLOBAL_ACTIVATED,1,2000,SUCCESS_TONE);
      DeleteNScrId(SCR_MAINMENU_STYLE);
}



/*****************************************************************************
* FUNCTION
*  PhnsetGetMainMenuStyle
* DESCRIPTION
*   This function is to get setting main menu style
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 PhnsetGetMainMenuStyle(void)
{
   return g_phnset_cntx_p->MenuStyle;
}
#endif /* __MMI_MAINMENU_STYLE_CHANGE_EN__ */



/*****************************************************************************
* FUNCTION
*  HighlightPhnsetShowDtTime
* DESCRIPTION
*   This function is highlight handler for Phonesetup->IdleScreenDisplay-> Show Date and Time
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
void HighlightPhnsetShowDtTime(void)
{
	/* If Show Date Time if On Show LSK as OFF otherwise On.*/	
	if(g_phnset_cntx_p->curShowDtStatus)
		{
			ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
		}
		else
		{
			ChangeLeftSoftkey(STR_GLOBAL_ON,0);
		}

	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(PhnsetToggleShowDtTime,KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*  PhnsetToggleShowDtTime
* DESCRIPTION
*   This function is Left softkey handler 
*        for Phonesetup->IdleScreenDisplay-> Show Date and Time
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetToggleShowDtTime(void)
{
	U16 index;
	index = GetCurrHiliteID();

	/* if show date time is on chnage it off*/
	if(g_phnset_cntx_p->curShowDtStatus)
		g_phnset_cntx_p->curShowDtStatus=0;
	/* else chnage it on*/
	else
		g_phnset_cntx_p->curShowDtStatus=1;

	/* Flag to Indicate Value is changed, will be used on exit Handler of menu*/
	g_phnset_cntx_p->ShowDtDirtyFlag = TRUE;

	if(g_phnset_cntx_p->curShowDtStatus)
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}

	/* Redraw the Category Screen As Hint Handler and LSK are going to Chnage*/
	RedrawCategoryFunction();
	
	/* After redraw hight the ShowDateTime*/
	HighlightPhnsetShowDtTime();
}

#if defined (__LED__) || defined (__MMI_RUNNING_LED__)	
/*****************************************************************************
* FUNCTION
*  HighlightPhnsetEnableRollLed
* DESCRIPTION
*   This function is highlight handler for Phonesetup->Roll Led-> Enable Roll Led
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
void HighlightPhnsetEnableRollLed(void)
{
	/* If Show Date Time if On Show LSK as OFF otherwise On.*/	
	if(g_phnset_cntx_p->curEnableRollLedStatus)
		{
			ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
		}
		else
		{
			ChangeLeftSoftkey(STR_GLOBAL_ON,0);
		}

	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(PhnsetToggleEnableRollLed,KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  PhnsetToggleEnableRollLed
* DESCRIPTION
*   This function is Left softkey handler 
*        for Phonesetup->Enable Roll Led
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetToggleEnableRollLed(void)
{
	U16 index;
	index = GetCurrHiliteID();

	/* if show date time is on chnage it off*/
	if(g_phnset_cntx_p->curEnableRollLedStatus)
		g_phnset_cntx_p->curEnableRollLedStatus=0;
	/* else chnage it on*/
	else
		g_phnset_cntx_p->curEnableRollLedStatus=1;

	///* Flag to Indicate Value is changed, will be used on exit Handler of menu*/
	//g_phnset_cntx_p->ShowDtDirtyFlag = TRUE;

	if(g_phnset_cntx_p->curEnableRollLedStatus)
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}

	/* Redraw the Category Screen As Hint Handler and LSK are going to Chnage*/
	RedrawCategoryFunction();
	
	/* After redraw hight the ShowDateTime*/
	HighlightPhnsetEnableRollLed();
}
//luanyy add for tc9751 end

#endif

#if defined(MOTION_SENSOR_SUPPORT)
kal_uint8 MotionSensor_GetSetting(void)
{
      	S16 error;
	U8 tmpmsensor;

	 ReadValue(NVRAM_SETTING_MOTION_SENSOR,&tmpmsensor, DS_BYTE,&error);

	 if(tmpmsensor == 0xFF)
	 	tmpmsensor = 1;

	return tmpmsensor;
} 
void HiliteHandlerMotionSensorSetting(S32 index) 
{
	g_phnset_cntx_p->curHightlightItem = (U8) index;
	return;
}
void PhnsetMontionSensor(void)
{
	S16 error;
	U8 tmpmsensor;

	if( g_phnset_cntx_p->curHightlightItem )
	{
		tmpmsensor = 0;
		WriteValue(NVRAM_SETTING_MOTION_SENSOR,&tmpmsensor, DS_BYTE,&error);
	}
	else
	{
		tmpmsensor = 1;
		WriteValue(NVRAM_SETTING_MOTION_SENSOR,&tmpmsensor, DS_BYTE,&error);
	}		
	
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);

	DeleteNHistory(1);

} 
void EntryPhnsetMotionSensor(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8 tmpmsensor;

	//S16 Err=0;
	//U8 *IdleScrDispType[MAX_SUB_MENUS];
	S16 error;
	/*Call Exit Handler*/
	EntryNewScreen(SCR_SETTING_MOTION_SENSOR, NULL, EntryPhnsetMotionSensor, NULL);

	/*Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_SETTING_MOTION_SENSOR);
	
	/*Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_SETTING_MOTION_SENSOR);

	/*Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_SETTING_MOTION_SENSOR);

	/*Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_SETTING_MOTION_SENSOR, nStrItemList);

	/*Set current parent id*/
	SetParentHandler(MENU_SETTING_MOTION_SENSOR);

	/*Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(HiliteHandlerMotionSensorSetting);
	
	ReadValue(NVRAM_SETTING_MOTION_SENSOR,&tmpmsensor, DS_BYTE,&error);
	if(tmpmsensor == 0xFF || tmpmsensor == 1)
		g_phnset_cntx_p->curr_motion_sensor = 0;
	else
		g_phnset_cntx_p->curr_motion_sensor = 1;
	
	/*Display Category Screen */	
	ShowCategory11Screen (STR_SETTING_MOTION_SENSOR,MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, g_phnset_cntx_p->curr_motion_sensor, guiBuffer);

	/*Register function with softkey */
	SetLeftSoftkeyFunction(PhnsetMontionSensor,KEY_EVENT_UP);
	SetKeyHandler(PhnsetMontionSensor, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}
void HighlightPhnsetMotionSensor(void)
{
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryPhnsetMotionSensor,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetMotionSensor, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
 }

#endif

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetShowOwnNo
* DESCRIPTION
*   This function is highlight handler for Phonesetup->IdleScreenDisplay->Show Owner Number
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
void HighlightPhnsetShowOwnNo(void)
{
	U16 index;
	index = GetCurrHiliteID();

	/*If Owner Number Is On Change The LSK to OFF otherwise ON.*/
	if( g_phnset_cntx_p->curShowOwnNumStatus == 1 )
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}
	
	/*Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(PhnsetToggleShowOwnNo,KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*  PhnsetToggleShowOwnNo
* DESCRIPTION
*   This function is Left softkey handler 
*        for Phonesetup->IdleScreenDisplay-> Show owner number
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetToggleShowOwnNo(void)
{
	U16 index;
	index = GetCurrHiliteID();

	/*If Own Number Display if Off then turn it On Pressing LSK*/
	if( g_phnset_cntx_p->curShowOwnNumStatus == 0 )
	{
		g_phnset_cntx_p->curShowOwnNumStatus = 1;
	}
	else
	{
		g_phnset_cntx_p->curShowOwnNumStatus = 0;
	}

	/* Flag to write NVRAM in Exit Handler*/
	 g_phnset_cntx_p->ShowOwnNumDirtyFlag = TRUE;

	if(g_phnset_cntx_p->curShowOwnNumStatus)
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}

	RedrawCategoryFunction();
	HighlightPhnsetShowOwnNo();
}


//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)
/*****************************************************************************
* FUNCTION
*  HighlightPhnsetShowOPN
* DESCRIPTION
*   This function is highlight handler for Phonesetup->IdleScreenDisplay->Show Operator Name
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
void HighlightPhnsetShowOPN(void)
{
	U16 index;
	index = GetCurrHiliteID();

	/*If Owner Number Is On Change The LSK to OFF otherwise ON.*/
	if( g_phnset_cntx_p->curShowOPNStatus == 1 )
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}
	
	/*Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(PhnsetToggleShowOPN,KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  PhnsetToggleShowOPN
* DESCRIPTION
*   This function is Left softkey handler 
*        for Phonesetup->IdleScreenDisplay-> Show Operator Name
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetToggleShowOPN(void)
{
	U16 index;
	index = GetCurrHiliteID();

	/*If Own Number Display if Off then turn it On Pressing LSK*/
	if( g_phnset_cntx_p->curShowOPNStatus == 0 )
	{
		g_phnset_cntx_p->curShowOPNStatus = 1;
	}
	else
	{
		g_phnset_cntx_p->curShowOPNStatus = 0;
	}

	/* Flag to write NVRAM in Exit Handler*/
	 g_phnset_cntx_p->ShowOPNDirtyFlag = TRUE;

	if(g_phnset_cntx_p->curShowOPNStatus)
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}

	RedrawCategoryFunction();
	HighlightPhnsetShowOPN();
}
#endif
/*****************************************************************************
* FUNCTION
*  PhnsetSetShowDTStatus
* DESCRIPTION
*   This function is to set show data time statu on/off
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
void PhnsetSetShowDTStatus( U8 status )
{
   g_phnset_cntx_p->curShowDtStatus = status;
}

/*
** ( Phonesetup-> SpeedDial -> Set Number ) functions are in speedDial.c
*/




#if !defined(__MMI_NO_CONTRAST_LEVEL__)
/*****************************************************************************
* FUNCTION
*  HighlightPhnsetContrastLevel
* DESCRIPTION
*   This function is highlight handler for Phonesetup-> Contrast
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
void HighlightPhnsetContrastLevel(void)
{
	TRACE_SYS_FUNCTION();
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryPhnsetContrastLevel,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetContrastLevel, KEY_RIGHT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*  EntryPhnsetContrastLevel
* DESCRIPTION
*   This function is Entry function for Phonesetup-> Contrast
*   Following functions: PhnsetContrastLevelEndHdlr, PhnsetContrastLevelLskHdlr, 
*                                HighlightPhnsetContrastLevelMain, HighlightPhnsetContrastLevelSub
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
typedef void (*FuncPtr_S32)(s32 i);   

void EntryPhnsetContrastLevel(void)
{
	U8* guiBuffer,*MainLcd =NULL,*SubLcd = NULL;       /* Buffer holding history data */

	TRACE_SYS_FUNCTION();
	/*Call Exit Handler*/
	EntryNewScreen(SCR_SETTING_CONTRAST_LEVEL, NULL, EntryPhnsetContrastLevel, NULL);

	/*Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_SETTING_CONTRAST_LEVEL);	

	SubLcd = (U8*)GetString(STR_SETTING_SUB_LCD);
	MainLcd= (U8*)GetString(STR_SETTING_MAIN_LCD);

	/*Set current parent id*/
	SetParentHandler(MENU_SETTING_CONTRAST_LEVEL);

#ifdef __MMI_SUBLCD__	
	ShowCategory88Screen(  STR_MENU_SETTING_CONTRAST_LEVEL, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, 0,
						  0, 0,MainLcd,SubLcd,
						    1,15,&(g_phnset_cntx_p->MainCntrst) ,1,15,&(g_phnset_cntx_p->SubCntrst), guiBuffer);
        RegisterCategory88ChangeCallbacks(HighlightPhnsetContrastLevelMain,HighlightPhnsetContrastLevelSub);
#else
	ShowCategory155Screen(STR_MENU_SETTING_CONTRAST_LEVEL, MAIN_MENU_TITLE_SETTINGS_ICON,
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						0, 0, IMG_CONTRAST_BACKGROUND, 0,
						1, 5, &(g_phnset_cntx_p->MainCntrst),guiBuffer);
        RegisterCategory155ChangeCallback((FuncPtr_S32)HighlightPhnsetContrastLevelMain);
#endif
	
	/* Register function with right softkey */
	SetKeyHandler(PhnsetContrastLevelEndHdlr, KEY_END, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(PhnsetContrastLevelLskHdlr, KEY_EVENT_UP);

}




/*****************************************************************************
* FUNCTION
*  PhnsetContrastLevelEndHdlr
* DESCRIPTION
*   This function is END key handler for Phonesetup-> Constract
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetContrastLevelEndHdlr(void)
{
	PhnsetRestoreToNvramContrast();
	DisplayIdleScreen();
}



/*****************************************************************************
* FUNCTION
*  HighlightPhnsetContrastLevelMain
* DESCRIPTION
*   This function is highlight handler for Phonesetup-> Contrast -> MainLCD
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
void HighlightPhnsetContrastLevelMain(S32 index)
{
	TRACE_SYS_FUNCTION();
	
	if( index >0 && index <=15 )
	{
	    g_phnset_cntx_p->MainCntrst = index;
	    PhnsetMainLCDContrast((U8)index);
	}
}



/*****************************************************************************
* FUNCTION
*  HighlightPhnsetContrastLevelSub
* DESCRIPTION
*   This function is highlight handler for Phonesetup-> Contrast -> SubLCD
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
#ifdef __MMI_SUBLCD__	
void HighlightPhnsetContrastLevelSub(S32 index)
{
	if( index >0 && index <=15 )
	{
	    g_phnset_cntx_p->SubCntrst = index;
	    PhnsetSubLCDContrast(index);
	}
}
#endif /* __MMI_SUBLCD__ */



/*****************************************************************************
* FUNCTION
*  PhnsetContrastLevelLskHdlr
* DESCRIPTION
*   This function is Left softkey handler 
*        for Phonesetup->Constract -> Main/Sub LCD
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetContrastLevelLskHdlr(void)
{
	S16 error;
	U8 data;

	TRACE_SYS_FUNCTION();
	ReadValue( NVRAM_SETTING_CONTRAST_LEVEL, &data, DS_BYTE , &error);
	if( data != g_phnset_cntx_p->MainCntrst )
	{
	   WriteValue( NVRAM_SETTING_CONTRAST_LEVEL, &(g_phnset_cntx_p->MainCntrst), DS_BYTE , &error);
	}

#ifdef __MMI_SUBLCD__	
	ReadValue( NVRAM_SETTING_CONTRAST_SUBLCD_LEVEL, &data, DS_BYTE , &error);
	if( data != g_phnset_cntx_p->SubCntrst )
	{
	   WriteValue( NVRAM_SETTING_CONTRAST_SUBLCD_LEVEL, &(g_phnset_cntx_p->SubCntrst), DS_BYTE , &error);
	}
#endif

	GoBackHistory();	
}

#endif /* #if !defined(__MMI_NO_CONTRAST_LEVEL__) */


/*****************************************************************************
* FUNCTION
*  HighlightPhnsetWelcomeText
* DESCRIPTION
*   This function is highlight handler for Phonesetup-> Greeting Text
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
void HighlightPhnsetWelcomeText(void)
{
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryPhnsetWelcomeText,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetWelcomeText, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  PhnsetFillWelcomeInlineEditStruct
* DESCRIPTION
*   This function is to fill inline editor screen for Phonesetup-> Greeting Text
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
static void PhnsetFillWelcomeInlineEditStruct(void *pInlineStruct ) 
{
	S8* WelcomeString=NULL;
	InlineItem *InlineStruct = (InlineItem *)pInlineStruct;

	WelcomeString=GetWelcomeText();
	/*CurrSelect*/ g_phnset_cntx_p->curHightlightItem = (U32)IsShowWelcomeScr1();//fixed for restore factory default

	pfnUnicodeStrncpy((S8*)WelcomeText, WelcomeString, MAX_SETTING_WELCOME_TEXT);
	OslMfree(WelcomeString);

	greet_text_on_off[1] = (U8*)GetString(STR_GLOBAL_ON);
	greet_text_on_off[0] = (U8*)GetString(STR_GLOBAL_OFF);

#if 1
	SetInlineItemActivation(&InlineStruct[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&InlineStruct[0], (PU8)(U8 *) GetString(STR_MENU9146_OFF));

	SetInlineItemActivation(&InlineStruct[2], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&InlineStruct[2], (PU8)(U8 *) GetString(STR_MENU9146_ON));

     SetInlineItemActivation(&InlineStruct[1],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
	SetInlineItemSelect(&InlineStruct[1],2,greet_text_on_off,(S32 *)&(g_phnset_cntx_p->curHightlightItem) /*CurrSelect*/);

	SetInlineItemActivation(&InlineStruct[3],KEY_LSK,KEY_EVENT_UP);
	SetInlineItemFullScreenEdit(&InlineStruct[3],STR_MENU9146_WELCOME_TEXT,MAIN_MENU_TITLE_SETTINGS_ICON,(PU8)WelcomeText,
	MAX_SETTING_WELCOME_TEXT + 1,INPUT_TYPE_ALPHANUMERIC_SENTENCECASE);
	SetInlineFullScreenEditCustomFunction(&InlineStruct[3], SettingInlineCustomFunction);

#else
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
#endif
}



/*****************************************************************************
* FUNCTION
*  EntryPhnsetWelcomeText
* DESCRIPTION
*   This function is Entry function for Phonesetup-> Greeting Text
*   Following functions: EntryWelcomeTextOn
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
void EntryPhnsetWelcomeText(void)
{
	U16 nNumofItem=4;      /* Stores no of children in the submenu*/
	U8* inputBuffer;     /* added for inline edit history */
	U8* guiBuffer;       /* Buffer holding history data */
	U16 inputBufferSize;
	U16		ItemIcons[4]={IMG_SETTING_GREET_STATUS, 0, IMG_SETTING_GREET_TEXT, 0};
   InlineItem greet_text_inline_edit_list[4];

   EntryNewScreen(SCR9146_WELCOME_TEXT, ExitPhnsetWelcomeText, NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR9146_WELCOME_TEXT);

	InitializeCategory57Screen();
	memset( greet_text_inline_edit_list, 0, sizeof(greet_text_inline_edit_list) );
     PhnsetFillWelcomeInlineEditStruct( (void *) greet_text_inline_edit_list);
	inputBuffer = GetCurrNInputBuffer (SCR9146_WELCOME_TEXT, &inputBufferSize );    //added for inline edit history
	if(inputBuffer != NULL)
		SetCategory57Data(greet_text_inline_edit_list, nNumofItem, inputBuffer);

	ClearHighlightHandler();

	ShowCategory57Screen(STR_MENU9146_WELCOME_TEXT,MAIN_MENU_TITLE_SETTINGS_ICON,
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						nNumofItem,ItemIcons,greet_text_inline_edit_list,
	               0,guiBuffer);

#if defined(__MMI_GB__)                                                          //add by yaosq 20061110 for leftsoft in profile
	mmi_frm_set_highlight_handler();
	GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone); 
#endif
	/* Register function for right soft key */
	SetCategory57RightSoftkeyFunctions( EntryPhnsetWelcomeTextDoneHdlr, GoBackHistory);

}


/*****************************************************************************
* FUNCTION
*  ExitPhnsetWelcomeText
* DESCRIPTION
*   This function is Exit function for Phonesetup-> Greeting Text
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitPhnsetWelcomeText(void)
{
	history_t   Scr; 
	S16 nHistory = 0;
	U16 inputBufferSize;                //added for inline edit history

	Scr.scrnID = SCR9146_WELCOME_TEXT;
	CloseCategory57Screen();
	Scr.entryFuncPtr = EntryPhnsetWelcomeText;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, (U16)inputBufferSize);                       //added for inline edit history
}



/*****************************************************************************
* FUNCTION
*  EntryPhnsetWelcomeTextDoneHdlr
* DESCRIPTION
*   This function is done key handler  for Phonesetup-> Greeting Text -> Done
*   Functionality: Query Yes/No
*   Following functions:  PhnsetWelcomeTextDoneLskHdlr
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryPhnsetWelcomeTextDoneHdlr(void)
{
	U8* guiBuffer;       /* Buffer holding history data */

	/*Call Exit Handler*/
	EntryNewScreen(SCR9146_WELCOME_TEXT_ON, NULL, NULL, NULL);

	/*Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR9146_WELCOME_TEXT_ON);	

   /*Display Category1 Screen */
	ShowCategory162Screen( STR_GLOBAL_YES, IMG_GLOBAL_YES,
       STR_GLOBAL_NO, IMG_GLOBAL_NO, STR_GLOBAL_SAVE,IMG_GLOBAL_QUESTION,guiBuffer);


	SetLeftSoftkeyFunction(PhnsetWelcomeTextDoneLskHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBack2TheHistory,KEY_EVENT_UP);

   /* Play warning tone */
	playRequestedTone(WARNING_TONE);
}



/*****************************************************************************
* FUNCTION
*  PhnsetWelcomeTextDoneLskHdlr
* DESCRIPTION
*   This function is Left softkey handler 
*        for Phonesetup-> Greeting Text -> Done -> Yes
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetWelcomeTextDoneLskHdlr(void)
{
	S16 error;

	//ExecuteCurrExitHandler();

	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	
	WriteValue( NVRAM_SETTING_WELCOME_TEXT,(U8*) &(g_phnset_cntx_p->curHightlightItem) /* CurrSelect */, DS_BYTE , &error);
	WriteRecord(NVRAM_EF_SETTING_LID, 1,(void *) WelcomeText,(U16) (MAX_SETTING_WELCOME_TEXT * ENCODING_LENGTH), &error);


	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),IMG_GLOBAL_ACTIVATED,1,ST_NOTIFYDURATION,SUCCESS_TONE);
	DeleteNHistory(1);

}




/*****************************************************************************
* FUNCTION
*  HighlightSPOF
* DESCRIPTION
*   This function is Highlight handler for Phonesetup->IdleScreenDisplay --> Mainmenu style
*   Follow functions: EntryPhnsetMainMenuStyle
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightSPOF(void)
{

	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntrySpofApp,KEY_EVENT_UP);
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(EntrySpofApp, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
}



/*
** ( Phonesetup-> SPOF ) functions are in ScheduledPowerOnOff.c
*/




/*
** Query function begin:
*/


/*****************************************************************************
* FUNCTION
*  PhnsetGetShowOwnerNumStatus
* DESCRIPTION
*   This function is to get show owner number on/off status
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
MMI_BOOL PhnsetGetShowOwnerNumStatus(void)
{
	U8 data = 0;
	S16 error;

	ReadValue( NVRAM_SETTING_OWNER_NO, &data, DS_BYTE , &error);

	if(data == 1)
		return MMI_TRUE;
	else
		return MMI_FALSE;
}

//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)
/*****************************************************************************
* FUNCTION
*  PhnsetGetShowOPN
* DESCRIPTION
*   This function is to get show owner number on/off status
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
MMI_BOOL PhnsetGetShowOPNStatus(void)
{
	U8 data = 0;
	S16 error;

	ReadValue( NVRAM_SETTING_OPN_DISPLAY, &data, DS_BYTE , &error);

	if(data == 0)
		return MMI_FALSE;
	else
		return MMI_TRUE;
}
#endif

/*****************************************************************************
* FUNCTION
*  PhnsetGetShowDateTimeStatus
* DESCRIPTION
*   This function is to get show date/time on/off status
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
MMI_BOOL PhnsetGetShowDateTimeStatus(void)
{
	U8 data = 0;
	S16 error;

	ReadValue( NVRAM_SETTING_STATUS_DT_TIME, &data, DS_BYTE , &error);

	if(data == 0)
		return MMI_FALSE;
	else
		return MMI_TRUE;
}

//luanyy add for tc9751 start
/*****************************************************************************
* FUNCTION
*  PhnsetGetSrollLightStatus
* DESCRIPTION
*   This function is to get enable srcoll light
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
#if defined (__LED__) || defined (__MMI_RUNNING_LED__)	
MMI_BOOL PhnsetGetSrollLightStatus(void)
{
	U8 data = 0;
	S16 error;

	ReadValue( NVRAM_PHN_ENABLE_SCROLL_LIGHT, &data, DS_BYTE , &error);

	if(data == 0)
		return MMI_FALSE;
	else
		return MMI_TRUE;
}
#endif
//luanyy add for tc9751 end


/*****************************************************************************
* FUNCTION
*  PhnsetSetWelcomeText
* DESCRIPTION
*   This function is to set welcome text for AT command
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
MMI_BOOL PhnsetSetWelcomeText(BOOL mode,  U8 dcs, U8 *text )
{
	S16 error;
	S32 AtMode;
	U8 AtWelcomeText[(MAX_SETTING_WELCOME_TEXT +1) * ENCODING_LENGTH];
	
     memset( AtWelcomeText, 0, sizeof( AtWelcomeText ) );
	if( dcs == L4_ASCII )
	{
	    AnsiiNToUnicodeString( (S8 *)AtWelcomeText, (S8 *)text, MAX_SETTING_WELCOME_TEXT );
	}
	else if(  dcs == L4_UCS2  )
	{
          memcpy( AtWelcomeText,  text, sizeof (AtWelcomeText) );
      }

     WriteRecord(NVRAM_EF_SETTING_LID, 1,(void *) AtWelcomeText,(U16) (MAX_SETTING_WELCOME_TEXT * ENCODING_LENGTH), &error);

	if(error == 4)
	{
	     if( mode == 0 )
	      AtMode = 0;
	     else
	      AtMode = 1;
	     WriteValue( NVRAM_SETTING_WELCOME_TEXT,(U8*) &AtMode, DS_BYTE , &error);
	     
		if( error == 4) 
		return MMI_TRUE;
		else
   		   return MMI_FALSE;
	}
	else
	{
		return MMI_FALSE;
	}

}


/*****************************************************************************
* FUNCTION
*  PhnsetGetWelcomeText
* DESCRIPTION
*   This function is to get welcome text for AT command
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/

void PhnsetGetWelcomeText( S8* buf, U16 buf_size )
{
	S16 error;
	U8 data[(MAX_SETTING_WELCOME_TEXT+1)*ENCODING_LENGTH];
	U16 max_buf_size;

	memset(data, 0, sizeof(data) );
	ReadRecord(NVRAM_EF_SETTING_LID, 1, data, (MAX_SETTING_WELCOME_TEXT*ENCODING_LENGTH), &error);
	
	if( buf_size < (MAX_SETTING_WELCOME_TEXT+1)*ENCODING_LENGTH )
		max_buf_size = buf_size;
	else
		max_buf_size = (MAX_SETTING_WELCOME_TEXT+1)*ENCODING_LENGTH;
	
	memset(buf, 0, buf_size );
	if(data[0] != 0xFF )
	   memcpy( buf, data, (max_buf_size-ENCODING_LENGTH) );

}


/**************************************************************

	FUNCTION NAME		: PhnsetRestoreToNvramContrast(void)

  	PURPOSE				: Set Contrast Level From Stored Values

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: MMI_BOOL

	Author				: Vanita Jain

 

**************************************************************/
MMI_BOOL PhnsetRestoreToNvramContrast(void)
{
	S16 error;
	U8 data=0;

	ReadValue( NVRAM_SETTING_CONTRAST_LEVEL, &data, DS_BYTE , &error);
	g_phnset_cntx_p->MainCntrst = data;
	PhnsetMainLCDContrast(data);

#ifdef __MMI_SUBLCD__		
	data =0;
	ReadValue( NVRAM_SETTING_CONTRAST_SUBLCD_LEVEL, &data, DS_BYTE , &error);
	g_phnset_cntx_p->SubCntrst = data;
	PhnsetSubLCDContrast(data);
#endif

	return MMI_TRUE;
}




/*
** Hint Handler begin:
*/

/*****************************************************************************
* FUNCTION
*  HintPhnsetLanguage
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Language
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhnsetLanguage(U16 index)
{
	S16 error;
	U8 data=0;
	
	ReadValue( NVRAM_SETTING_LANG, &data, DS_BYTE , &error);

#ifdef __MMI_AUTO_LANG_EN__
	if( data == 0xFF ) /* Auto Language Selected */
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_LANGUAGESCR_AUTO) );
	}
	else
#endif
	{
	pfnUnicodeStrcpy((PS8)hintData[index],(PS8)gLanguageArray[data].aName);
	}

}



/*****************************************************************************
* FUNCTION
*  HintPhnsetPreferedInputMethod
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Language
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
#ifdef __MMI_PREFER_INPUT_METHOD__
extern U16 GetPrefferedInputMethodByCurrentLang(void);
void HintPhnsetPreferedInputMethod(U16 index)
{
	S16 error;
	U16 data=0;
	ReadValue( NVRAM_SETTING_PREFER_INPUT_METHOD, &data, DS_SHORT , &error);
	if(data == 0xFF || error != NVRAM_READ_SUCCESS)
	{
		data = GetPrefferedInputMethodByCurrentLang();
	}
	InitPreferedInputMethod();
       pfnUnicodeStrcpy((PS8)hintData[index],(PS8)inputMethodList[GetInputMethodIndex(data)]);
}
#endif

#ifdef __MMI_TOUCH_SCREEN__
static void (*g_phnset_factory_default_finish_callback)(void);
#ifdef __MMI_HAND_WRITING__
/*****************************************************************************
* FUNCTION
*  HighlightHandwritingSetting
* DESCRIPTION
*	This function is highlight handler for Handwriting setting screen
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightHandwritingSetting(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryHandWritingSettingScr,KEY_EVENT_UP);
	SetKeyHandler(EntryHandWritingSettingScr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif

/*****************************************************************************
* FUNCTION
*  HighlightCalibrationSetting
* DESCRIPTION
*	This function is highlight handler for Calibration setting screen
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightCalibrationSetting(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryCalibrationSettingScr,KEY_EVENT_UP);
	SetKeyHandler(EntryCalibrationSettingScr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

#ifdef __MMI_HAND_WRITING__
/*****************************************************************************
* FUNCTION
*  SaveHandwritingSetting
* DESCRIPTION
*   This function saves pen setting in Settings->PhoneSetup->PenSetting screen
*   Functionality: 
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  gpen_settings
*****************************************************************************/
void SaveHandwritingSetting(void)
{
	S16 pError;
	g_phnset_cntx_p->pen_speed = gInlinePenSpeed;
	g_phnset_cntx_p->pen_color = gInlinePenColorIndex;

	WriteValue(NVRAM_PEN_SPEED,&(g_phnset_cntx_p->pen_speed),DS_BYTE,&pError);
	if(pError != NVRAM_WRITE_SUCCESS)
		PRINT_INFORMATION(("NVRAM PenSetting Write Failed. Error -%d",pError));
	WriteValue(NVRAM_PEN_COLOR,&(g_phnset_cntx_p->pen_color),DS_BYTE,&pError);
	if(pError != NVRAM_WRITE_SUCCESS)
		PRINT_INFORMATION(("NVRAM Pen Setting Write Failed. Error -%d",pError));

	GoBeyondMarkerScr(SCR_HANDWRITING_SETTING);
}


/*****************************************************************************
* FUNCTION
*  GoBackHandwritingSettingConfirm
* DESCRIPTION
*   This function exits pen setting screen when user doesn't save pen settings after changing in Settings->PhoneSetup->PenSetting screen
*   Functionality: 
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void GoBackHandwritingSettingConfirm(void)
{
	GoBeyondMarkerScr(SCR_HANDWRITING_SETTING);
}


/*****************************************************************************
* FUNCTION
*  EntryScrHandwritingSettingConfirm
* DESCRIPTION
*   This function displays pop up to confirm pen settings change before saving them in Settings->PhoneSetup->PenSetting screen
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryScrHandwritingSettingConfirm(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO ,IMG_GLOBAL_NO,
						get_string(STR_GLOBAL_SAVE),
						IMG_GLOBAL_QUESTION,WARNING_TONE);
	
	SetLeftSoftkeyFunction(SaveHandwritingSetting,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHandwritingSettingConfirm,KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*  HandwritingSettingInlineColor
* DESCRIPTION
*   This function updates softkey and iInlinePen color according to change in Pen Color
*
* PARAMETERS
*  a  IN
*  b  OUT
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HandwritingSettingInlineColor(U16 index)
{
	if(index < MAX_PEN_COLORS)
	{
		if (index != gInlinePenColorIndex)
		{
			set_wgui_inline_list_menu_changed();
		}
		gInlinePenColorIndex = index;
	}
}


/*****************************************************************************
* FUNCTION
*  EntryHandWritingSettingScr
* DESCRIPTION
*   Entry function for Pen Setting Screen
*
* PARAMETERS
*  a  IN
*  b  OUT
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryHandWritingSettingScr(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 *guiBuffer;
	U8 *inputBuffer;
	U16 inputBufferSize;
	U16 list_of_icons[4];
	
	S32 iCount;
	S16 item_offset = 0;  

	EntryNewScreen(SCR_HANDWRITING_SETTING, ExitHandWritingSettingScr,NULL,NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_HANDWRITING_SETTING);
	inputBuffer = GetCurrNInputBuffer ( SCR_HANDWRITING_SETTING, &inputBufferSize );
	
	for(iCount = 0; iCount< MAX_PEN_SPEED; ++iCount)
	{
			g_pen_speed_str_list[iCount] = (U8*)GetString((U16)(iCount+STR_HANDWRITING_FAST));
	}

	gInlinePenSpeed = g_phnset_cntx_p->pen_speed;
	gInlinePenColorIndex = g_phnset_cntx_p->pen_color;
	
	/* start of Category 57 */
	InitializeCategory57Screen();
	
	/* Pen speed */
	SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString(STR_HANDWRITING_PEN_SPEED));

	SetInlineItemActivation(&wgui_inline_items[item_offset], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemSelect(&wgui_inline_items[item_offset++],MAX_PEN_SPEED,g_pen_speed_str_list, &gInlinePenSpeed);

	/*Pen Color*/
	SetInlineItemCaption(&wgui_inline_items[item_offset++], (PU8)GetString(STR_HANDWRITING_PEN_COLOR));

	SetInlineItemActivation(&wgui_inline_items[item_offset], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemColorSelect(&wgui_inline_items[item_offset++],
									MAX_PEN_COLORS,
									(U16)gInlinePenColorIndex,
									(color_t*)g_pen_color_list,
									HandwritingSettingInlineColor);
	
	if(inputBuffer != NULL)
	{
		SetCategory57Data(wgui_inline_items, item_offset, inputBuffer);
	}

	list_of_icons[0] = IMG_GLOBAL_L1;
	list_of_icons[1] = 0;
	list_of_icons[2] = IMG_GLOBAL_L2;
	list_of_icons[3] = 0;
//GetRootTitleIcon(MENU_SETTING_HANDWRITING)
	ShowCategory57Screen(STR_HANDWRITING_SETTING, GetRootTitleIcon(MENU_SETTING_HANDWRITING),
		STR_GLOBAL_OK, IMG_GLOBAL_OK,STR_GLOBAL_BACK, IMG_GLOBAL_BACK,item_offset, list_of_icons, wgui_inline_items, 0, guiBuffer);

	SetCategory57RightSoftkeyFunctions(EntryScrHandwritingSettingConfirm, GoBackHistory);
}


/*****************************************************************************
* FUNCTION
*  ExitHandWritingSettingScr
* DESCRIPTION
*   Exit function for Pen Setting Screen
*
* PARAMETERS
*  a  IN
*  b  OUT
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitHandWritingSettingScr(void)
{
	history_t h;
	U16 inputBufferSize;

	CloseCategory57Screen();
	h.scrnID = SCR_HANDWRITING_SETTING;
	h.entryFuncPtr = EntryHandWritingSettingScr;
	GetCategoryHistory( h.guiBuffer );
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data( (U8*) h.inputBuffer );
	AddNHistory( h, inputBufferSize );
}
#endif

/*****************************************************************************
* FUNCTION
*  CalibrationSettingPenDown
* DESCRIPTION
* 	PenDown Handler for Calibration Setting Screen
* PARAMETERS
*	point	IN	
* RETURNS
*  void
* GLOBALS AFFECTED
*  g_phnset_calibration_screen_state
*****************************************************************************/
static void CalibrationSettingPenDown(mmi_pen_point_struct point)
{
    switch(g_phnset_calibration_screen_state)
    {
        case CALIBRATION_WELCOME_STATE:

            g_phnset_calibration_screen_state = CALIBRATION_1ST_POINT_STATE;
            UpdateCalibrationScreen(
                    IMG_CALIBRATION_TOP_LEFT, 
                    TP_CALIB_1_X,
                    TP_CALIB_1_Y,
                    STR_CALIBRATION_ARROW_CENTER_TEXT,
                    CALIBRATION_1ST_POINT_STATE, MMI_TRUE);

            break;

        case CALIBRATION_1ST_POINT_STATE:

            g_phnset_calibration_screen_state = CALIBRATION_2ND_POINT_STATE;
            UpdateCalibrationScreen(
                    IMG_CALIBRATION_TOP_LEFT,
                    TP_CALIB_2_X,
                    TP_CALIB_2_Y,
                    STR_CALIBRATION_ARROW_CENTER_TEXT,
                    CALIBRATION_2ND_POINT_STATE, MMI_TRUE);
            mmi_pen_set_calibration_data( point.x, point.y, 0);
            break;

        case CALIBRATION_2ND_POINT_STATE:

            g_phnset_calibration_screen_state = CALIBRATION_3RD_POINT_STATE;
            UpdateCalibrationScreen(
                    IMG_CALIBRATION_TOP_LEFT,
                    TP_CALIB_3_X,
                    TP_CALIB_3_Y,
                    STR_CALIBRATION_ARROW_CENTER_TEXT,
                    CALIBRATION_3RD_POINT_STATE, MMI_TRUE);

            mmi_pen_set_calibration_data( point.x, point.y, 1);
            break;

        case CALIBRATION_3RD_POINT_STATE:
            g_phnset_calibration_screen_state = CALIBRATION_VERIFY_STATE;
            UpdateCalibrationScreen(
                    IMG_CALIBRATION_TOP_LEFT,
                    TP_CALIB_VERIFY_X,
                    TP_CALIB_VERIFY_Y,
                    STR_CALIBRATION_ARROW_CENTER_TEXT,
                    CALIBRATION_VERIFY_STATE, MMI_TRUE);
            mmi_pen_set_calibration_data( point.x, point.y, 2);
            break;
        case CALIBRATION_VERIFY_STATE:
		if( mmi_pen_set_calibration_data( point.x, point.y, 3))
		{ 
			#ifdef MMI_ON_HARDWARE_P
			mmi_pen_end_calibration( );
			#endif
			WriteNvramCalibrationSet(TRUE);
			if(g_phnset_factory_default_finish_callback != NULL)
			{
				DisplayPopupCallBack((U8*)GetString(STR_GLOBAL_DONE),  IMG_GLOBAL_ACTIVATED, 
									MMI_FALSE, UI_POPUP_NOTIFYDURATION_TIME, (U8)SUCCESS_TONE, g_phnset_factory_default_finish_callback);
				g_phnset_factory_default_finish_callback = NULL;

			}
			else
			{
				DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  IMG_GLOBAL_ACTIVATED, 
								MMI_FALSE, UI_POPUP_NOTIFYDURATION_TIME, (U8)SUCCESS_TONE );
			}
			g_PenCalibrationFinished = TRUE;
			DeleteNScrId(SCR_CALIBRATION_SETTING);
		}
		else
		{ 
			g_phnset_calibration_screen_state = CALIBRATION_1ST_POINT_STATE;
			UpdateCalibrationScreen(
				IMG_CALIBRATION_TOP_LEFT, 
				TP_CALIB_1_X,
				TP_CALIB_1_Y,
				STR_CALIBRATION_ARROW_CENTER_TEXT,
				CALIBRATION_1ST_POINT_STATE, MMI_TRUE);

			mmi_pen_start_calibration( );
		}
		break;
        default:
            MMI_ASSERT(0);
            break;
    }
}

/*****************************************************************************
* FUNCTION
*  PhnsetDefaultCalibrationDone
* DESCRIPTION
*   return to normal start up flow
*
* PARAMETERS
*	callback		IN		callback function after calibration is finished.
* RETURNS
*  void
* GLOBALS AFFECTED
*  g_phnset_calibration_cntx_p
*****************************************************************************/
extern void FlightModeCheckBeforeAnimation(U8 flightmode);
void PhnsetDefaultCalibrationDone(void)
{
	FlightModeCheckBeforeAnimation(FLIGHTMODE_OFF);
}
void ExitFactoryDefaultCalibration(void)
{
	DisplayPopupCallBack((U8*)GetString(STR_GLOBAL_ABORT),  IMG_GLOBAL_WARNING, 
									MMI_FALSE, UI_POPUP_NOTIFYDURATION_TIME, (U8)WARNING_TONE, PhnsetDefaultCalibrationDone);
}

/*****************************************************************************
* FUNCTION
*  EntryCalibrationSettingScr
* DESCRIPTION
*   Entry function for Calibration Setting Screen
*
* PARAMETERS
*	void
* RETURNS
*  void
*****************************************************************************/
void EntryCalibrationSettingScr(void)
{
	EntryNewScreen(SCR_CALIBRATION_SETTING, ExitCalibrationSettingScr, NULL, NULL);

	g_PenCalibrationFinished = FALSE;

	/*2 Show Calibration Welcome screen */
	g_phnset_calibration_screen_state = CALIBRATION_WELCOME_STATE;
	ShowCalibrationScreen(0, 0, 0, STR_CALIBRATION_WELCOME_TEXT, CALIBRATION_WELCOME_STATE);

	TurnOnBacklight(0);

	/* 3. Register Pen Handlers 
	 * Because hardware can not get correct ADC value on PenUp, we use PenDown for calibration. */
	ClearAllKeyHandler();
	mmi_pen_register_up_handler(mmi_pen_dummy_hdlr);
	mmi_pen_register_down_handler(CalibrationSettingPenDown);
	mmi_pen_register_move_handler(mmi_pen_dummy_hdlr);
	mmi_pen_register_abort_handler(mmi_pen_dummy_hdlr);
	mmi_pen_register_repeat_handler(mmi_pen_dummy_hdlr);
	mmi_pen_register_long_tap_handler(mmi_pen_dummy_hdlr);
	if(g_phnset_factory_default_finish_callback != NULL)
		SetKeyHandler(ExitFactoryDefaultCalibration, KEY_END, KEY_EVENT_DOWN);

    mmi_pen_start_calibration( );
}


/*****************************************************************************
* FUNCTION
*  ExitCalibrationSettingScr
* DESCRIPTION
*   Exit function for Calibration Setting Screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitCalibrationSettingScr(void)
{
    history_t h;
    U16 nHistory = 0;
    if( !g_PenCalibrationFinished)
    {
        /* Calibration screen should be put in history, e.g. "Invalid SIM" popup on boot */
        h.scrnID = SCR_CALIBRATION_SETTING;
        h.entryFuncPtr = EntryCalibrationSettingScr;
        pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
        memset(h.guiBuffer, 0, sizeof(h.guiBuffer));
        AddHistory(h);
    }

    g_phnset_calibration_screen_state = CALIBRATION_NULL_STATE;

    TurnOffBacklight();

    //Clear unfinished calibration process if necessary.
#ifdef MMI_ON_HARDWARE_P
    mmi_pen_end_calibration( );
    if( mmi_pen_read_calibration( ))
    { 
        mmi_pen_unblock( );
    }
    else
    { 
        mmi_pen_block( );
    }
#endif
}

/*****************************************************************************
* FUNCTION
*  PhnsetNeedFactoryDefaultCalibration
* DESCRIPTION
*	Whether pen is calibrated.
*
* PARAMETERS
*  void
* RETURNS
*  BOOL
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
BOOL PhnsetNeedFactoryDefaultCalibration(void)
{
#ifdef MMI_ON_WIN32
	/* PC Simulator does not support NVRAM */
	return MMI_FALSE;
#else
	if (!g_phnset_cntx_p->is_default_calibration_set)
		return MMI_TRUE;
	else 
		return MMI_FALSE;
#endif
}

/*****************************************************************************
* FUNCTION
*  PhnsetReadNvramCalibrationData
* DESCRIPTION
*   Function to read calibration data from NVRAM
* PARAMETERS
*  void
* RETURNS
*  VOID
* GLOBALS AFFECTED
*  NONE
*****************************************************************************/
void PhnsetReadNvramCalibrationData(void)
{
	S16 error = 0;
	S32 ret = 0;
	U8 temp = 0;
	g_phnset_cntx_p->is_default_calibration_set = MMI_FALSE;
	ret = ReadRecord(NVRAM_EF_CALIBRATION_SET, 1, &temp, sizeof(temp), &error);
	
	if(temp == 1)
	{
		g_phnset_cntx_p->is_default_calibration_set = MMI_TRUE;
	}
}
/*****************************************************************************
* FUNCTION
*  WriteNvramCalibrationData
* DESCRIPTION
*	Writes calibration setting in NVRAM
* PARAMETERS
*  calibration_data  IN	
* RETURNS
*  void
* GLOBALS AFFECTED
*  NONE
*****************************************************************************/
void WriteNvramCalibrationSet(BOOL done)
{
	U8 temp = done;
	S16 error = 0;
	S32 ret = 0;
	

	ret  = WriteRecord(NVRAM_EF_CALIBRATION_SET,1,(void*)&temp,sizeof(temp),&error);

	g_phnset_cntx_p->is_default_calibration_set = done;
}


/*****************************************************************************
* FUNCTION
*  EntryPhnseFactoryDefaultCalibrationScr
* DESCRIPTION
*   Entry function for Calibration Screen of factory default value
*
* PARAMETERS
*	callback		IN		callback function after calibration is finished.
* RETURNS
*  void
* GLOBALS AFFECTED
*  g_phnset_calibration_cntx_p
*****************************************************************************/
void EntryPhnseFactoryDefaultCalibrationScr(void)
{
	g_phnset_factory_default_finish_callback = PhnsetDefaultCalibrationDone;
	EntryCalibrationSettingScr();
	//ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
}
#ifdef __MMI_HAND_WRITING__
/*****************************************************************************
* FUNCTION
*  PhnsetGetHandwritingColor
* DESCRIPTION
*	Get handwriting stroke color
*
* PARAMETERS
*  void
* RETURNS
*  color
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
color_t PhnsetGetHandwritingColor(void)
{
	MMI_ASSERT(g_phnset_cntx_p->pen_color < MAX_PEN_COLORS);
	return g_pen_color_list[g_phnset_cntx_p->pen_color];
}


/*****************************************************************************
* FUNCTION
*  PhnsetGetHandwritingSpeed
* DESCRIPTION
*	Get handwriting speed
*
* PARAMETERS
*  void
* RETURNS
*  PHNSET_HANDWRITING_SPEED_ENUM
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
PHNSET_HANDWRITING_SPEED_ENUM PhnsetGetHandwritingSpeed(void)
{
	switch (g_phnset_cntx_p->pen_speed)
	{
		case 0:
			return PHNSET_HANDWRITING_FAST;
		case 1:
			return PHNSET_HANDWRITING_MEDIUM;
		case 2:
			return PHNSET_HANDWRITING_SLOW;
		default:
			MMI_ASSERT(0);
			return PHNSET_HANDWRITING_MEDIUM;
	}
}
#endif

#endif /* __MMI_TOUCH_SCREEN__ */

/*****************************************************************************
* FUNCTION
*  HintPhnsetWelcomeText
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Greeting Text
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhnsetWelcomeText(U16 index)
{
	if(IsShowWelcomeScr1())
	{
//qiff modify for bug:10174 2008/10/25	
	#if 0
	//qiff modify for bug 9844 on greenstone PRJ.
		//pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
		if((WelcomeText[0]==0)&&(WelcomeText[1]==0))
		{
			S8* WelcomeString=NULL;
			WelcomeString=GetWelcomeText();
			pfnUnicodeStrncpy((S8*)WelcomeText, WelcomeString, MAX_SETTING_WELCOME_TEXT);
			OslMfree(WelcomeString);
		}
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)WelcomeText);
		#endif
	        pfnUnicodeStrcpy((PS8) hintData[index], (PS8) GetString(STR_GLOBAL_ON));
//qiff modify end	
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
	}
}



/*****************************************************************************
* FUNCTION
*  HintPhnsetOwnerNum
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Language
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhnsetOwnerNum(U16 index)
{
	if(PhnsetGetShowOwnerNumStatus())
	{
	     g_phnset_cntx_p->curShowOwnNumStatus = 1;
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
	}
	else
	{
	     g_phnset_cntx_p->curShowOwnNumStatus = 0;
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
	}
}

//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)
/*****************************************************************************
* FUNCTION
*  HintPhnsetOPN
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Language
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhnsetOPN(U16 index)
{
	if(PhnsetGetShowOPNStatus())
	{
	     g_phnset_cntx_p->curShowOPNStatus = 1;
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
	}
	else
	{
	     g_phnset_cntx_p->curShowOPNStatus = 0;
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
	}
}
#endif

/*****************************************************************************
* FUNCTION
*  HintPhnsetShowDateTime
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Language
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhnsetShowDateTime(U16 index)
{
	if(PhnsetGetShowDateTimeStatus())
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
	}
}

//luanyy add for tc9751 start
/*****************************************************************************
* FUNCTION
*  HintPhnsetEnableRollLed
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Language
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
#if defined (__LED__) || defined (__MMI_RUNNING_LED__)	
void HintPhnsetEnableRollLed(U16 index)
{
	if(PhnsetGetSrollLightStatus())
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
	}
}
#endif
//luanyy add for tc9751 end

/*****************************************************************************
* FUNCTION
*  HintPhnsetWallpaper
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Display Character -> Wallpaper
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhnsetWallpaper(U16 index)
{
     PS8 p;

     p=PhnsetGetCurWallpaperName();
     if( p)
   	   pfnUnicodeStrcpy((PS8)hintData[index], p);
}

#ifdef __MMI_SUB_WALLPAPER__
/*****************************************************************************
* FUNCTION
*  HintPhnsetSubWallpaper
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Display Character -> Wallpaper
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhnsetSubWallpaper(U16 index)
{
     PS8 name;

     name=PhnsetGetCurSubWallpaperName();
     if( name)
   	   pfnUnicodeStrcpy((PS8)hintData[index], name);
}
#endif
#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
void ReadPhnsetIdleClockType(void)
{
	S16 error;	
	ReadValue( NVRAM_PHNSET_IDLE_CLOCK_TYPE, &(g_phnset_cntx_p->curtCLKType), DS_BYTE , &error);

	if(g_phnset_cntx_p->curtCLKType>1)
	{
		g_phnset_cntx_p->curtCLKType=0;
		WriteValue( NVRAM_PHNSET_IDLE_CLOCK_TYPE, &(g_phnset_cntx_p->curtCLKType), DS_BYTE , &error);
	}
	wgui_set_clock_type(g_phnset_cntx_p->curtCLKType);
}
void HintPhnsetIdleClockType(U16 index)
{
	switch(g_phnset_cntx_p->curtCLKType)
	{
/*	
			case MMI_PHNSET_IDLECLKTYPE_NONE:
				pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_NONE));
				break;				
*/				
			case MMI_PHNSET_IDLECLKTYPE_DIGITAL:
				pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_WALLPAPER_DIGITAL));
				break;				
			case MMI_PHNSET_IDLECLKTYPE_ANALOG:
				pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_WALLPAPER_ANALOG));
				break;				
			default:
				MMI_ASSERT(0);
	}
}
void PhnsetIdleClockTypeHiliteHandler(S32 index)
{
	g_phnset_cntx_p->curHightlightItem = (U32)index;

}
void PhnsetIdleClockTypeCallBack( void)
{
	GoBackToHistory(SCR_IDLE_SCR_DISP);
}

void PhnsetIdleClockType( void)
{
	S16 error;	
	g_phnset_cntx_p->curtCLKType = g_phnset_cntx_p->curHightlightItem;

	WriteValue( NVRAM_PHNSET_IDLE_CLOCK_TYPE, &(g_phnset_cntx_p->curtCLKType), DS_BYTE , &error);
	wgui_set_clock_type(g_phnset_cntx_p->curtCLKType);
	DisplayPopupCallBack((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, 
	SUCCESS_TONE,PhnsetIdleClockTypeCallBack);
}

void EntryPhnsetIdleClockType(void)
{
	U8* pGuiBuffer;

	EntryNewScreen(SCR_ID_PHNSET_IDLE_CLOCK_TYPE, NULL, EntryPhnsetIdleClockType, NULL);


	pGuiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_IDLE_CLOCK_TYPE);


/*
	subMenuDataPtrs[MMI_PHNSET_IDLECLKTYPE_NONE] = (PU8)GetString( STR_GLOBAL_NONE );;
*/	
	subMenuDataPtrs[MMI_PHNSET_IDLECLKTYPE_ANALOG] = (PU8)GetString( STR_WALLPAPER_ANALOG );
	subMenuDataPtrs[MMI_PHNSET_IDLECLKTYPE_DIGITAL] = (PU8)GetString( STR_WALLPAPER_DIGITAL );

	if(g_phnset_cntx_p->curtCLKType>1)
		ReadPhnsetIdleClockType();

	RegisterHighlightHandler(PhnsetIdleClockTypeHiliteHandler);

	
	ShowCategory36Screen(  STR_ID_IDLE_CLOCK_TYPE, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MMI_PHNSET_IDLECLKTYPE_MAX, (U8**)subMenuDataPtrs, g_phnset_cntx_p->curtCLKType, pGuiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(PhnsetIdleClockType, KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}
void HighlightPhnsetIdleClockType(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	SetLeftSoftkeyFunction(EntryPhnsetIdleClockType,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetIdleClockType, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}



#endif
/*****************************************************************************
* FUNCTION
*  HintPhnsetScreenSaver
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Display Character -> Screen Saver
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhnsetScreenSaver(U16 index)
{
	switch (PhnsetGetCurScrSvrStatus())
	{
		case SET_SS_STATUS_ON:
		{
			PS8 p=PhnsetGetCurScrSvrName();
			if( p )
				pfnUnicodeStrcpy((PS8)hintData[index], p );
			else 
				pfnUnicodeStrcpy((PS8)hintData[index],"");
		}
		break;
		
#ifdef __MMI_LCD_PARTIAL_ON__
		case SET_SS_STATUS_POWERSAVING:
			pfnUnicodeStrcpy((PS8)hintData[index], (const S8 *) GetString(STR_SCREENSAVER_DISPLAY_TIME));
			break;
#endif /* __MMI_LCD_PARTIAL_ON__ */

		default:
			pfnUnicodeStrcpy((PS8)hintData[index],"");
	        //memset(hintData[index],0,MAX_SUB_MENU_HINT_SIZE);
	}
}



/*****************************************************************************
* FUNCTION
*  PhnsetMainLCDContrast
* DESCRIPTION
*   This function is to set main LCD contrast
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
#include "drv.h"

void PhnsetMainLCDContrast(U8 level)
{
	TRACE_EF(g_sw_SYSTEM, "the level = %d", level);
	if(level>15 || level < 1) 
	   return;

#ifdef MMI_ON_HARDWARE_P
//	 DRV_LcdIoControl(DRV_LCD_IOCTL_CONTRAST, level, NULL);
//	lcd_bright_level(MAIN_LCD, MainLCD_Contrast_Level_Info[level-1]);
#if 0
	if (ERR_SUCCESS == DRV_LcdIoControl(DRV_LCD_IOCTL_CONTRAST, level, NULL))

	{
		TRACE_SYS_FUNC("ERR_SUCCESS");
	}
	else
	{
		TRACE_SYS_FUNC("!ERR_SUCCESS");
	}
#else
	//MainLCD->BrightLevel(level); //This is no use, and the phone has no LCD contrast function. And this will make flash when the power on.
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, level);
#endif	
#endif	

}



/*****************************************************************************
* FUNCTION
*  PhnsetSubLCDContrast
* DESCRIPTION
*   This function is to set sub LCD contrast
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetSubLCDContrast(U8 level)
{

	if(level>15 || level < 1) 
	   return;

#if (defined(__MMI_SUBLCD__) && defined(MMI_ON_HARDWARE_P) )
	lcd_bright_level(SUB_LCD, SubLCD_Contrast_Level_Info[level-1]);
#endif	
}



/*****************************************************************************
* FUNCTION
*  PhnsetChangeMainLCDContrastValue
* DESCRIPTION
*   This function is to set  main LCD contrast in phone setup context
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetChangeMainLCDContrastValue( U8 level )
{
   g_phnset_cntx_p->MainCntrst = level;
}


#ifdef __MMI_SUBLCD__
/*****************************************************************************
* FUNCTION
*  PhnsetChangeSubLCDContrastValue
* DESCRIPTION
*   This function is to set  sub LCD contrast in phone setup context
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetChangeSubLCDContrastValue( U8 level )
{
   g_phnset_cntx_p->SubCntrst = level;
}
#endif


/*****************************************************************************
* FUNCTION
*  SettingInlineCustomFunction
* DESCRIPTION
*   This function is to be register as the inline editor full screen editor LSK handler
*        for Phonesetup-> Greeting Text -> Greeting Text
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void SettingInlineCustomFunction (void)
{
	PRINT_INFORMATION (("*-------[SmsSettingsGuiInterface.c] SetProfileSettingCustomFuns -------*\n"));
	ChangeLeftSoftkey (STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
	SetLeftSoftkeyFunction (EntryScrForInputMethodAndDone, KEY_EVENT_UP);
	SetInputMethodAndDoneCaptionIcon(MAIN_MENU_TITLE_SETTINGS_ICON);
}


#ifdef __MMI_SET_DEF_ENCODING_TYPE__
/*****************************************************************************
* FUNCTION
*  HighlightPhnsetDefEncoding
* DESCRIPTION
*   This function is Highlight handler for Phonesetup-> Default Encoding Type
*   Follow functions: EntryPhnsetLang
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/

void HighlightPhnsetDefEncoding(void)
{
     /* Change softkey label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK); 
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK); 

     /* Register entry key handler */
	SetLeftSoftkeyFunction(EntryPhnsetDefEncoding, KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetDefEncoding, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

     /* Register back key handler */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  HintPhnsetDefEncoding
* DESCRIPTION
*   This function is Hint handler for Phonesetup-> Default Encoding Type
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

void HintPhnsetDefEncoding( U16 index )
{
      PS8 hintStr=NULL;
      
      switch(g_phnset_cntx_p->EncodingType)
      {
         case 0:  hintStr = (PS8) GetString(STR_DEF_ENCODING_TYPE_BIG5);  break;
         case 1:  hintStr = (PS8) GetString(STR_DEF_ENCODING_TYPE_GB2312);  break;
         case 2:  hintStr = (PS8) GetString(STR_DEF_ENCODING_TYPE_UCS2);  break;
         default:
            hintStr = (PS8) GetString(STR_DEF_ENCODING_TYPE_BIG5);
            break;
       }
       pfnUnicodeStrcpy( (PS8)hintData[index], hintStr );

}


/*****************************************************************************
* FUNCTION
*  PhnsetDefEncodingHiliteHandler
* DESCRIPTION
*   This function is Highlight handler for Phonesetup->Default Encoding Type
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
void PhnsetDefEncodingHiliteHandler(S32 index)
{
	g_phnset_cntx_p->curHightlightItem = (U32)index;

}


/*****************************************************************************
* FUNCTION
*  PhnsetDefEncodingSelectHandler
* DESCRIPTION
*   This function is LSK handler for Phonesetup->Default Encoding Type -> X
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

void PhnsetDefEncodingSelectHandler(void)
{
   S16 error;
   BOOL is_suc = MMI_TRUE;


   if( g_phnset_cntx_p->EncodingType != g_phnset_cntx_p->curHightlightItem )
   {
        #ifdef MMI_ON_HARDWARE_P
        switch (g_phnset_cntx_p->curHightlightItem)
        {
      case PHNSET_ENCOD_BIG5: 
           #ifdef __MMI_CHSET_BIG5__
           g_chset_text_encoding = MMI_CHSET_BIG5;
           #else
           //g_chset_text_encoding =MMI_CHSET_BASE;
           is_suc = MMI_FALSE;
           #endif
      	    break;
      case PHNSET_ENCOD_GB2312: 
#ifdef __MMI_CHSET_GB2312__
          g_chset_text_encoding = MMI_CHSET_GB2312;
	  #else
      	   // g_chset_text_encoding = MMI_CHSET_BASE; 
           is_suc = MMI_FALSE;
	   #endif
      	    break;
         default: 
           //g_chset_text_encoding =MMI_CHSET_BASE;
           is_suc = MMI_FALSE;
            break;
        }
        #endif /* MMI_ON_HARDWARE_P */

        g_phnset_cntx_p->EncodingType = (U8) (g_phnset_cntx_p->curHightlightItem);
        WriteValue( NVRAM_SETTING_DEF_ENCODING, &(g_phnset_cntx_p->curHightlightItem), DS_BYTE, &error);
   }


  if( MMI_TRUE == is_suc )
   DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  IMG_GLOBAL_ACTIVATED, \
                           MMI_FALSE, UI_POPUP_NOTIFYDURATION_TIME, (U8)SUCCESS_TONE );
   else
   	DisplayPopup( (U8*)GetString(STR_GLOBAL_ERROR),  IMG_GLOBAL_ERROR, \
                           MMI_FALSE, UI_POPUP_NOTIFYDURATION_TIME, (U8)WARNING_TONE );

   DeleteScreenIfPresent( SCR_DEF_ENCODING_TYPE );
}

/*****************************************************************************
* FUNCTION
*  EntryPhnsetDefEncoding
* DESCRIPTION
*   This function is Entry function for Phonesetup-> Default encoding type
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/

void EntryPhnsetDefEncoding(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	//U8 *HintList[MAX_SUB_MENUS];

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_DEF_ENCODING_TYPE, NULL, EntryPhnsetDefEncoding, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_DEF_ENCODING_TYPE);	

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild( MENU_SETTING_DEF_ENCODING_TYPE );

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem( MENU_SETTING_DEF_ENCODING_TYPE );

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_SETTING_DEF_ENCODING_TYPE, nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_SETTING_DEF_ENCODING_TYPE);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(PhnsetDefEncodingHiliteHandler);

	/* 8 Construct the Hints List */
	//ConstructHintsList(MENU_SETTING_DEF_ENCODING_TYPE, HintList);

	/* 9. Display Category screen */
	ShowCategory11Screen (STR_DEF_ENCODING_TYPE_TITLE, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, g_phnset_cntx_p->EncodingType, guiBuffer);


	/* 10.Register function with softkey */
	SetLeftSoftkeyFunction(PhnsetDefEncodingSelectHandler,KEY_EVENT_UP);
	SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

#endif /* __MMI_SET_DEF_ENCODING_TYPE__ */


/*****************************************************************************
* FUNCTION
*  PhnsetGetDefEncodingType
* DESCRIPTION
*   This function is export function for applications to query the current encoding type setting
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

U8 PhnsetGetDefEncodingType(void)
{
   #ifdef __MMI_SET_DEF_ENCODING_TYPE__
     return g_chset_text_encoding;
   #else
     return MMI_CHSET_GB2312;
   #endif

}


/*
** Language Setting
*/

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetLang
* DESCRIPTION
*   This function is Highlight handler for Phonesetup->Language
*   Follow functions: EntryPhnsetLang
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetLang(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	SetLeftSoftkeyFunction(EntryPhnsetLang,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetLang, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
void PhnsetChangeWinStyleLSKHandler(void)
{
S16 error;
//	U16 index;
	S32 Idx = g_phnset_cntx_p->curHightlightItem;
//	BOOL isAutoLangSet = MMI_FALSE;
if(Idx == 0)
{
	enablechange_window_flag = 0;
}
else
{
	enablechange_window_flag = 1;
}
	change_window_style = Idx+9;
      	 WriteValue( NVRAM_CHANGE_SCREEN_MODE, &change_window_style, DS_BYTE , &error);
    DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
    DeleteNHistory(1);
}


/*****************************************************************************
* FUNCTION
*  EntryPhnsetIdleScrDisp
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryPhnsetChangeWinStyle(void)
{
	U8* pGuiBuffer;
	S32 totalstyle =11;
	U16 hiliteItem = change_window_style - 9;
	U8 i;
	S8 temp_char[100];
       const S8* changewinstyle[12] = {
	   	       " 1",
			" 2",
			" 3",
			" 4",
			" 5",
			" 6",
			" 7",
			" 8",
			" 9",
			" 10",
			" 11",
			" 12"};
			
	EntryNewScreen(SCR_ID_SETTING_CHANGE_WIN_STYLE, NULL, EntryPhnsetChangeWinStyle, NULL);
	pfnUnicodeStrcpy((S8*)subMenuData[0], (S8*)GetString(STR_GLOBAL_OFF));
	subMenuDataPtrs[0]=subMenuData[0];
	for( i=0; i< totalstyle -1; i++ )
		{
		memset(temp_char,0,100);
		AnsiiToUnicodeString(temp_char,(S8*)changewinstyle[i]);
		pfnUnicodeStrcpy((S8*)subMenuData[i+1], (S8*)GetString(STR_ID_SETTING_CHANGE_WIN_STYLE_MODE));
		UCS2Strncat((S8*)subMenuData[i+1], (S8*)temp_char,pfnUnicodeStrlen((PS8)temp_char));
			subMenuDataPtrs[i+1]=subMenuData[i+1];
		}
	pGuiBuffer = GetCurrGuiBuffer(SCR_ID_SETTING_CHANGE_WIN_STYLE);

	RegisterHighlightHandler(PhnsetLangHiliteHandler);
	
	ShowCategory36Screen(  STR_ID_SETTING_CHANGE_WIN_STYLE, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  totalstyle, (U8**)subMenuDataPtrs, hiliteItem, pGuiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(PhnsetChangeWinStyleLSKHandler, KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
* FUNCTION
*  HighlightPhnsetLang
* DESCRIPTION
*   This function is Highlight handler for Phonesetup->Language
*   Follow functions: EntryPhnsetLang
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetChangeWinStyle(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	SetLeftSoftkeyFunction(EntryPhnsetChangeWinStyle,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetChangeWinStyle, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif
/*****************************************************************************
* FUNCTION
*  EntryPhnsetIdleScrDisp
* DESCRIPTION
*   This function is Entry function for Phonesetup->IdleScreenDisplay
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryPhnsetLang(void)
{
	U8* pGuiBuffer;
	S32 totalLang;
	U16 hiliteItem = gCurrLangIndex;
	U8 i;

	EntryNewScreen(SCR_LANGUAGE_SELECTION, NULL, EntryPhnsetLang, NULL);

	totalLang = (S32) GetLanguages(& (g_phnset_cntx_p->ppLang) );

#ifdef __MMI_AUTO_LANG_EN__
	{	
		S16 error;
		U8 data=0;
		
		ReadValue( NVRAM_SETTING_LANG, &data, DS_BYTE , &error);
		if( data == 0xFF ) /* Auto Language set */
			hiliteItem = 0;
		else
			hiliteItem+=1;
	}
	subMenuDataPtrs[0] = (PU8)GetString( STR_LANGUAGESCR_AUTO );
	for( i=0; i< totalLang; i++ )
		subMenuDataPtrs[i+1] = g_phnset_cntx_p->ppLang[i];
	totalLang += 1;
#else
	for( i=0; i< totalLang; i++ )
		subMenuDataPtrs[i] = g_phnset_cntx_p->ppLang[i];
#endif
	pGuiBuffer = GetCurrGuiBuffer(SCR_LANGUAGE_SELECTION);

	RegisterHighlightHandler(PhnsetLangHiliteHandler);
	
	ShowCategory36Screen(  STR_MENU_LANGUAGE, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  totalLang, (U8**)subMenuDataPtrs, hiliteItem, pGuiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(PhnsetLangLSKHandler, KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
* FUNCTION
*  PhnsetLangHiliteHandler
* DESCRIPTION
*   This function is Highlight handler for Phonesetup->Language
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
void PhnsetLangHiliteHandler(S32 index)
{
	g_phnset_cntx_p->curHightlightItem = (U32)index;

}


/*****************************************************************************
* FUNCTION
*  PhnsetLangLSKHandler
* DESCRIPTION
*   This function is left softkey handling function 
*                                                  for Phonesetup->Language
*   Functionality: Save the setting menu style
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
extern pBOOL IsArabicSet(void);
extern pBOOL IsPersianSet(void);
extern pBOOL IsUrduSet(void);
void PhnsetLangLSKHandler(void)
{
	U8 data;
	S16 error;
	U16 index;
	U16 i;
	S32 LangIdx = g_phnset_cntx_p->curHightlightItem;
	BOOL isAutoLangSet = MMI_FALSE;

#ifdef __MMI_AUTO_LANG_EN__
	if( g_phnset_cntx_p->curHightlightItem == 0 ) /* Auto Language selected */
	{
       LangIdx = 0;
       isAutoLangSet = MMI_TRUE;
	}
	else
	   LangIdx -= 1;
#endif

   ReadValue( NVRAM_SETTING_LANG, &data, DS_BYTE , &error);


	/************************************************************************/
	 
	/************************************************************************/
    /* if( (LangIdx != (S32) gCurrLangIndex) *** Different Language			*/
	/*   || (data != 0xFF && isAutoLangSet == MMI_TRUE)  *** Auto Lang Set  */
	/*   || (data == 0xFF && isAutoLangSet == MMI_FALSE) *** Auto Lang UnSet*/
	/*   )																	*/
	/************************************************************************/

	if( (LangIdx != (S32) gCurrLangIndex)\
	   || (data != 0xFF && isAutoLangSet == MMI_TRUE)\
	   || (data == 0xFF && isAutoLangSet == MMI_FALSE)\
	   )
   {
      index = GetCurrHiliteID();


      if( isAutoLangSet == MMI_TRUE )
      {
#ifdef __MMI_AUTO_LANG_EN__ 
/* only when __MMI_AUTO_LANG_EN__ compile option turn on, the isAutoLangSet is possible to be TRUE  */
      	 U16 autoLangIdx = PhnsetGetAutoLangIdx();
      	 data = 0xFF;
      	 SetCurrentLanguage((S8)autoLangIdx);
      	 WriteValue( NVRAM_SETTING_LANG, &data, DS_BYTE , &error);
         pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_LANGUAGESCR_AUTO) );

	/********************************************************/
	 
	/* AT command will query MMI when needed                */
	/* Send to PS for AT+CLAE                               */
	/********************************************************/
      //	 SendLangChange2AT((U8)autoLangIdx);  // zrx del 20060612
#endif /* __MMI_AUTO_LANG_EN__ */
      }
      else
      {
	     data =(U8) LangIdx;
      	 SetCurrentLanguage((S8)data);
      WriteValue( NVRAM_SETTING_LANG, &data, DS_BYTE , &error);
         pfnUnicodeStrcpy((PS8)hintData[index],(PS8)g_phnset_cntx_p->ppLang[(U8)LangIdx]);

		/********************************************************/
		 
		/* AT command will query MMI when needed                */
		/* Send to PS for AT+CLAE                               */
		/********************************************************/
      	// SendLangChange2AT((U8)LangIdx); // zrx del 20060612
      }
   //not refreash window   	
      //Category52ChangeItemDescription(2,hintData[index]);
     // RedrawCategoryFunction();
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
      	RefreshNwProviderName(gPLMN[i], i);
	}
      
#ifdef __MMI_WORLD_CLOCK_SUBLCD_SETTING__
      if ( gIsSubLcdOnIdleScreen == 1)
         ShowIdleAppSubLcd();
#endif
    }
   if(IsPersianSet() || IsArabicSet()||IsUrduSet())
   		r2lMMIFlag = TRUE;
   else
   		r2lMMIFlag = FALSE;

   #ifdef __MMI_PREFER_INPUT_METHOD__
   SetInputMethodFromNVRAM();
   #endif
    DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
    DeleteNHistory(1);
}

/*****************************************************************************
* FUNCTION
*  RstLanguageDefault
* DESCRIPTION
*   This function is to set the defalut language
*   
*
*****************************************************************************/
BOOL RstLanguageDefault(void)
{
	U8 data = 1; //chinse language, 0 is english
	S16 error = 0;
	WriteValue( NVRAM_SETTING_LANG, &data, DS_BYTE , &error);

	return (error == NVRAM_WRITE_SUCCESS);
	
}
#ifdef __MMI_AUTO_LANG_EN__
/*****************************************************************************
* FUNCTION
*  PhnsetGetAutoLangIdx
* DESCRIPTION
*   This function is to read language setting from nvram and set
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U16 PhnsetGetAutoLangIdx(void)
{
	U8 lang_idx;

	if(  g_phnset_cntx_p->LP != CBS_INVALID
		&& g_phnset_cntx_p->LP != CBS0_LANG_UNSPECIFIED  )
	{
		for( lang_idx = 0; lang_idx < MMI_NUM_OF_AUTOLANG_ELEM; lang_idx++ )
		{
			if( g_phnset_cntx_p->LP == auto_lang_struct[lang_idx].LanType )
			{
				if (auto_lang_struct[lang_idx].LanSSC ) 
				return GetLanguageIndex((S8 *)auto_lang_struct[lang_idx].LanSSC);
				else
					return 0;
			}
		}

		if( lang_idx == MMI_NUM_OF_AUTOLANG_ELEM ) /* Not found! LP Not support */
		{
			#ifdef MMI_ON_HARDWARE_P
			return (U16)GetLanguageIndex((S8 *)MMI_DEFLANGSSC_FOR_AUTO);
			#else
			return 0;
			#endif
		}
	}
	else
	{
		#ifdef MMI_ON_HARDWARE_P
		return (U16)GetLanguageIndex((S8 *)MMI_DEFLANGSSC_FOR_AUTO);
		#else
		return 0;
		#endif
	}

}


/*****************************************************************************
* FUNCTION
*  PhnsetPLPInd
* DESCRIPTION
*   Process and store LP in phonesetup context
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
#define MMI_MAX_LP 5
void PhnsetPLPInd( U8 *LPArr )
{
	U8 i=0, j=0;
	
	for( i=0; i< MMI_MAX_LP; i++ )
	{
		/* found LP from language_type/SSC mapping table --> auto_lang_struct[]  */
		for(j=0; j< MMI_NUM_OF_AUTOLANG_ELEM; j++ )
		{
			if( LPArr[i] == auto_lang_struct[j].LanType )
				break;
		}
				
		/* Find language array (gLanguageArray) using found SSC */
		if( j != MMI_NUM_OF_AUTOLANG_ELEM )
		{
			if ( auto_lang_struct[i].LanSSC )
				if( GetLanguageIndex( (S8 *)auto_lang_struct[i].LanSSC ) != 0xFFFF )
				{
					PhnsetSetLP( LPArr[i] );
					break;
				}
		}
	}

}

/*****************************************************************************
* FUNCTION
*  PhnsetSetLP
* DESCRIPTION
*   This function is for SIM info indication to set LP
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetSetLP(U8 LP)
{
	g_phnset_cntx_p->LP = LP;
}


/*****************************************************************************
* FUNCTION
*  PhnsetGetLang
* DESCRIPTION
*   This function is to get user set language
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 PhnsetGetLang(void)
{
	return g_phnset_cntx_p->UsrLangIdx;
}


#endif /* __MMI_AUTO_LANG_EN__ */

/*****************************************************************************
* FUNCTION
*  PhnsetSetLangByNvramValue
* DESCRIPTION
*   This function is to read language setting from nvram and set
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
MMI_BOOL PhnsetSetLangByNvramValue(void)
{
	S16 error;
	U8 data=0;
	ReadValue( NVRAM_SETTING_LANG, &data, DS_BYTE , &error);
	g_phnset_cntx_p->UsrLangIdx = data;

	if(data == 0xFF) 
	{
#ifdef __MMI_AUTO_LANG_EN__
        /* If read nvram before SIM LP comming, treat as unspecified language */
        /* PhonsetGetAugoLanIdx will return default language index */
		data = PhnsetGetAutoLangIdx();
#else
		data =0;
		WriteValue( NVRAM_SETTING_LANG, &data, DS_BYTE , &error);
#endif
	}
	
	SetCurrentLanguage(data); /* Func in ReadRes.c */
	if(IsArabicSet() || IsPersianSet()||IsUrduSet())
		r2lMMIFlag = TRUE;			//guoyt added@090928 for arabic string display direction when system boot up.
	return MMI_TRUE;
}



/*****************************************************************************
* FUNCTION
*  PhnsetReinitHomeCityIdx
* DESCRIPTION
*   This function is to rearrange home city global array and set home city highlight item
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetReinitHomeCityIdx(void)
{
     /**
        * This Is Needed As Set Home City NVRAM Index Doesnt Change On Language Change
	   * As The Global City Array Is Chnaged. 
	   **/
	   
     /**************************************************************/  
 
	PhnsetReadNvmHomeCityIdxNStoreCityName();
	PhnsetReinitHomeCity();
	/* 
     /**************************************************************/
}

/*****************************************************************************
* FUNCTION
*  PhnsetSetHomeCityPtr
* DESCRIPTION
*   This function is to set current homecity string ptr
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetSetHomeCityPtr( U8* str )
{
   g_phnset_cntx_p->CurHomeCityStrPtr = str;
}


#if (defined(__MMI_NITZ__) && !defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__) )

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetNITZ
* DESCRIPTION
*   This function is highlight handler for Phonesetup-> Auto Update Date and Time
*   Follow functions:tEntryAutoUpdateDtTime
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetNITZ(void)
{
	U16 index;
	index = GetCurrHiliteID();

	/*Set the LSK to Off when Auto Update is ON*/
	if( g_phnset_cntx_p->curNITZStatus == 1 )
		{
			ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
		}
		else
		{
			ChangeLeftSoftkey(STR_GLOBAL_ON,0);
		}

	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

	/* Change left soft key icon and label */
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryPhnsetNITZ,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  EntryPhnsetNITZ
* DESCRIPTION
*   This function is Left softkey handler for Phonesetup-> Auto Update Date and Time
*   Functionality: Toggle status only
*
* PARAMETERS
*  a  IN/OUT
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryPhnsetNITZ(void)
{
	U16 index;
	index = GetCurrHiliteID();

	/*Toggle Auto Update of Date And Time*/
	if( g_phnset_cntx_p->curNITZStatus == 0 )
		g_phnset_cntx_p->curNITZStatus = 1;
	else 
		g_phnset_cntx_p->curNITZStatus = 0;

	/* Store the updated Value */
	g_phnset_cntx_p->NITZDirtyFlag = TRUE;
	if(g_phnset_cntx_p->curNITZStatus)
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
		Category52ChangeItemDescription(index,hintData[index]);
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}
	RedrawCategoryFunction();
	HighlightPhnsetNITZ();
	
}

/*****************************************************************************
* FUNCTION
*  HintPhnsetSetLanguage
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Auto Update of Date and Time
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhnsetNITZ(U16 index)
{
	if(PhnsetGetNITZFuncStatus())
	{
           g_phnset_cntx_p->curNITZStatus = 1;  
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
	}
	else
	{
	     g_phnset_cntx_p->curNITZStatus = 0;  
		pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
	}
}


/*****************************************************************************
* FUNCTION
*  PhnsetGetNITZFuncStatus
* DESCRIPTION
*   This function is to get NITZ function on/off status
*   Functionality: Only exist when (__MMI_NITZ__) and (!__MMI_AUTOTIMEZONE_IN_DATEANDTIME__)
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
MMI_BOOL PhnsetGetNITZFuncStatus(void)
{
	U8 data = 0;
	S16 error;

	ReadValue( NVRAM_SETTING_AUTOUPDATE_DT_TIME, &data, DS_BYTE , &error);

	if(data == 1)		
		return MMI_TRUE;
	else if(data == 0xff)
	{
		data =RstGetNITZStatusDefault();		
		WriteValue( NVRAM_SETTING_AUTOUPDATE_DT_TIME, &data, DS_BYTE , &error);
		return MMI_FALSE;
	}
	else
		return MMI_FALSE;
}
#endif /* defined(__MMI_NITZ__) && !defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__)  */


/*****************************************************************************
*
*	TV Out 
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_phnset_highlight_tvout()
* DESCRIPTION
*	highlight tvout item
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __MMI_TVOUT__
static void mmi_phnset_highlight_tvout(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	SetLeftSoftkeyFunction(mmi_phnset_entry_tvout_screen, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif /* __MMI_TVOUT__ */



/*****************************************************************************
* FUNCTION
*	 mmi_phnset_entry_tvout_screen()
* DESCRIPTION
*	entry phone setup tv out screen
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __MMI_TVOUT__
static void mmi_phnset_entry_tvout_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	item_list[32];
	U16	item_icons[32];
	U16	num_of_items;
	U8		*gui_buffer;
	U8		*popup_list[16];
	S32	i;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_SETTING_TVOUT, NULL, mmi_phnset_entry_tvout_screen, NULL);

	gui_buffer = GetCurrGuiBuffer(SCR_ID_SETTING_TVOUT);

	num_of_items = GetNumOfChild(MENU_ID_SETTING_TVOUT);
	GetSequenceStringIds(MENU_ID_SETTING_TVOUT, item_list);
	GetSequenceImageIds(MENU_ID_SETTING_TVOUT, item_icons);
	SetParentHandler(MENU_ID_SETTING_TVOUT);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i = 0; i < num_of_items; i++)
		item_icons[i] = (U16) (IMG_GLOBAL_L1+i);

	for(i = 0 ; i < num_of_items; i++)
		popup_list[i] = NULL;

	ConstructHintsList(MENU_ID_SETTING_TVOUT, popup_list);

	ShowCategory52Screen(STR_ID_SETTING_TVOUT,	
								MAIN_MENU_TITLE_SETTINGS_ICON,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								num_of_items, 
								item_list,
								item_icons, 
								(U8**)popup_list, 
								0, 
								0,
								gui_buffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);	
}
#endif /* __MMI_TVOUT__ */



/*****************************************************************************
* FUNCTION
*	 mmi_phnset_highlight_tvout_format()
* DESCRIPTION
*	highlight tvout format item
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __MMI_TVOUT__
static void mmi_phnset_highlight_tvout_format(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	SetLeftSoftkeyFunction(mmi_phnset_entry_tvout_format_screen, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif /* __MMI_TVOUT__ */


/*****************************************************************************
* FUNCTION
*	 mmi_phnset_highlight_tvout_style()
* DESCRIPTION
*	highlight tvout style item
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __MMI_TVOUT__
static void mmi_phnset_highlight_tvout_style(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	SetLeftSoftkeyFunction(mmi_phnset_entry_tvout_style_screen, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif /* __MMI_TVOUT__ */


/*****************************************************************************
* FUNCTION
*	 mmi_phnset_hint_tvout_format()
* DESCRIPTION
*   set tv out format menuitem hint
* PARAMETERS
*	 index 	IN		index of selected item
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_TVOUT__
static void mmi_phnset_hint_tvout_format(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	memset( (PS8)hintData[(index)], 0, sizeof(hintData[(index)]));

	if(g_phnset_cntx_p->tvout_format == MDI_TV_OUTPUT_FORMAT_NTSC)
	{
	 	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_SETTING_TVOUT_FORMAT_NTSC)));
	}
	else if(g_phnset_cntx_p->tvout_format == MDI_TV_OUTPUT_FORMAT_PAL)
	{
	 	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_SETTING_TVOUT_FORMAT_PAL)));
	}
	else
	{
		MMI_ASSERT(0); /* not supported yet */
	}
}
#endif /* __MMI_TVOUT__ */


/*****************************************************************************
* FUNCTION
*	 mmi_phnset_hint_tvout_style
* DESCRIPTION
*   set tv out style item hint
* PARAMETERS
*	 index 	IN		index of selected item
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_TVOUT__
static void mmi_phnset_hint_tvout_style(U16 index)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	memset((PS8)hintData[(index)], 0, sizeof(hintData[(index)]));

	if(g_phnset_cntx_p->tvout_style == SETTING_TVOUT_STYLE_PHONE)
	{
	 	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_SETTING_TVOUT_MM_STYLE_PHONE)));
	}
	else
	{
 		pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_SETTING_TVOUT_MM_STYLE_FULLSCR)));
	}
	
}
#endif /* __MMI_TVOUT__ */


/*****************************************************************************
* FUNCTION
*	 mmi_phnset_entry_tvout_format_screen()
* DESCRIPTION
*	entry phone setup tv out format screen
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __MMI_TVOUT__
static void mmi_phnset_entry_tvout_format_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	item_list[32];
	U16	item_icons[32];
	U16	num_of_items;
	U8		*gui_buffer;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_SETTING_TVOUT_FORMAT, NULL, mmi_phnset_entry_tvout_format_screen, NULL);

	gui_buffer = GetCurrGuiBuffer(SCR_ID_SETTING_TVOUT_FORMAT);

	num_of_items = GetNumOfChild(MENU_ID_SETTING_TVOUT_FORMAT);
	GetSequenceStringIds(MENU_ID_SETTING_TVOUT_FORMAT, item_list);
	GetSequenceImageIds(MENU_ID_SETTING_TVOUT_FORMAT, item_icons);
	SetParentHandler(MENU_ID_SETTING_TVOUT_FORMAT);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
	ShowCategory11Screen(STR_ID_SETTING_TVOUT_FORMAT, 
								MAIN_MENU_TITLE_SETTINGS_ICON, 
								STR_GLOBAL_OK, IMG_GLOBAL_OK,
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
								num_of_items, 
								item_list,
								g_phnset_cntx_p->tvout_format,	
								gui_buffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);	
}
#endif /* __MMI_TVOUT__ */



/*****************************************************************************
* FUNCTION
*	 mmi_phnset_entry_tvout_style_screen()
* DESCRIPTION
*	entry phone setup tv out style screen
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __MMI_TVOUT__
static void mmi_phnset_entry_tvout_style_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	item_list[32];
	U16	item_icons[32];
	U16	num_of_items;
	U8		*gui_buffer;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_SETTING_TVOUT_STYLE, NULL, mmi_phnset_entry_tvout_style_screen, NULL);

	gui_buffer = GetCurrGuiBuffer(SCR_ID_SETTING_TVOUT_STYLE);

	num_of_items = GetNumOfChild(MENU_ID_SETTING_TVOUT_MM_STYLE);
	GetSequenceStringIds(MENU_ID_SETTING_TVOUT_MM_STYLE, item_list);
	GetSequenceImageIds(MENU_ID_SETTING_TVOUT_MM_STYLE, item_icons);
	SetParentHandler(MENU_ID_SETTING_TVOUT_MM_STYLE);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory11Screen(STR_ID_SETTING_TVOUT_MM_STYLE, 
								MAIN_MENU_TITLE_SETTINGS_ICON, 
								STR_GLOBAL_OK, IMG_GLOBAL_OK,
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
								num_of_items, 
								item_list,
								g_phnset_cntx_p->tvout_style,	
								gui_buffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);	
}
#endif /* __MMI_TVOUT__ */


/*****************************************************************************
* FUNCTION
*	 mmi_phnset_init_tvout()
* DESCRIPTION
*	init tvout
* PARAMETERS
*	 nil
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __MMI_TVOUT__
static void mmi_phnset_init_tvout(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 	error;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
   mdi_tvout_init(mmi_phnset_tvout_event_hdlr);

	/* load format setting */
	ReadValue(NVRAM_SETTING_TVOUT_FORMAT, &g_phnset_cntx_p->tvout_format, DS_SHORT, &error);

	/* not init */
	if(g_phnset_cntx_p->tvout_format == 0xffff)
	{
		g_phnset_cntx_p->tvout_format = MDI_TV_OUTPUT_FORMAT_NTSC;
	}	

	/* load style setting */
	ReadValue(NVRAM_SETTING_TVOUT_STYLE, &g_phnset_cntx_p->tvout_style, DS_SHORT, &error);

	/* not init */
	if(g_phnset_cntx_p->tvout_style == 0xffff)
	{
		g_phnset_cntx_p->tvout_style = SETTING_TVOUT_STYLE_PHONE;
	}	

   mdi_tvout_set_format(g_phnset_cntx_p->tvout_format);
   
}
#endif /* __MMI_TVOUT__ */



/*****************************************************************************
* FUNCTION
*	 mmi_phnset_tvout_event_hdlr()
* DESCRIPTION
*	init tvout
* PARAMETERS
*	 result		IN			event result
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __MMI_TVOUT__
static void mmi_phnset_tvout_event_hdlr(MDI_RESULT result)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	MDI_RESULT ret;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	if(result == MDI_RES_TV_ENABLE)
	{
		DisplayPopup((PU8)GetString(STR_ID_SETTING_TVOUT_PLUG_IN), IMG_ID_POPUP_TVOUT, 1, ST_NOTIFYDURATION, SUCCESS_TONE);

		if(g_phnset_cntx_p->tvout_style = SETTING_TVOUT_STYLE_PHONE)
		{
			/* start mode1 */
			mdi_tvout_set_mode(	MDI_TV_MODE_LCD_SCREEN,
										0,						/* layer_width */
										0);					/* layer_height */

			ret = mdi_tvout_start();

			if(ret < 0)
			{
				DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, ST_NOTIFYDURATION, WARNING_TONE);
				return;
			}
		}									
	}
	else if(result == MDI_RES_TV_DISABLE)
	{
		DisplayPopup((PU8)GetString(STR_ID_SETTING_TVOUT_PLUG_OUT), IMG_ID_POPUP_TVOUT, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	}
	else
	{
		MMI_ASSERT(0);
	}
}
#endif /* __MMI_TVOUT__ */


/*****************************************************************************
* FUNCTION
*	mmi_phnset_tvout_get_style()
* DESCRIPTION
*	get display style of multimedia applcation
* PARAMETERS
*	 result		IN			event result
* RETURNS
*	 void
* GLOBALS AFFECTED
*	 nil
*****************************************************************************/
#ifdef __MMI_TVOUT__
U16 mmi_phnset_tvout_get_style(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/			
	return g_phnset_cntx_p->tvout_style;
}
#endif /* __MMI_TVOUT__ */
#ifdef __FT_IDLE_SMART_DIAL__
U16 SmartDialStyleOn=0;
U16 IsSmartDialStyle(void)
{
	return SmartDialStyleOn;
}

void SetSmartDialStyle(U16 flag)
{
	SmartDialStyleOn = flag;
}

void ChangeDialStyle(void)
{
	U16 style;
	style = IsSmartDialStyle();
	if(style == 0)
	{
		style =1;
	}
	else
	{
		 style =0;
	}
	SetSmartDialStyle(style);
	SaveDialStyle();
}
void InitDialStyle(void)
{
	S16 error;
	U8 data;
	
	ReadValue(NVRAM_SETTING_DIAL_STYLE, &(data), DS_BYTE, &error);
	
	if(data!=0 && data !=1)
	{
		data =1;
	}
	
	WriteValue(NVRAM_SETTING_DIAL_STYLE, &(data), DS_BYTE, &error); 
	SetSmartDialStyle(data);
}

void SaveDialStyle(void)
{
	S16 error;
	U8 data;

	data = IsSmartDialStyle();
	WriteValue(NVRAM_SETTING_DIAL_STYLE, &(data), DS_BYTE, &error);
}
extern S8 *mmi_ucs2cpy(S8 *strDestination, const S8 *strSource);
void EntryPhnsetDialStyle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 index, data;

    index = GetCurrHiliteID();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	 
    if (GetExitScrnID() != POPUP_SCREENID)
    {
		ChangeDialStyle();
		data = IsSmartDialStyle();
        if (data == 0)
        {	
            DisplayPopup(
                (U8*) GetString(STR_GLOBAL_OFF),
                IMG_GLOBAL_ACTIVATED,
                1,
                UI_POPUP_NOTIFYDURATION_TIME,
                (U8) SUCCESS_TONE);
            mmi_ucs2cpy((PS8) hintData[index], (PS8) GetString(STR_GLOBAL_OFF));
            Category52ChangeItemDescription(index, hintData[index]);
        }
        else
        {
        	DisplayPopup(
                (U8*) GetString(STR_GLOBAL_ON),
                IMG_GLOBAL_ACTIVATED,
                1,
                UI_POPUP_NOTIFYDURATION_TIME,
                (U8) SUCCESS_TONE);
            mmi_ucs2cpy((PS8) hintData[index], (PS8) GetString(STR_GLOBAL_ON));
            Category52ChangeItemDescription(index, hintData[index]);
        }
    }

}

void HighlightPhnsetDialStyle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 index, data;

    index = GetCurrHiliteID();
	data = IsSmartDialStyle();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (data == 1)
    {
        ChangeLeftSoftkey(STR_GLOBAL_OFF, 0);
    }
    else
    {
        ChangeLeftSoftkey(STR_GLOBAL_ON, 0);
    }

    /* Change right soft key icon and label */
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

    SetLeftSoftkeyFunction(EntryPhnsetDialStyle, KEY_EVENT_UP);
    SetKeyHandler(0, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

    /* Register function for right soft key */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 *  HintPhnsetGPIOLED
 * DESCRIPTION
 *  ...
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void HintPhnsetDialStyle(U16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U16 data;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	data = IsSmartDialStyle();
    if (data == 1)
    {
        mmi_ucs2cpy((PS8) hintData[index], (PS8) GetString(STR_GLOBAL_ON));
    }
    else
    {
        mmi_ucs2cpy((PS8) hintData[index], (PS8) GetString(STR_GLOBAL_OFF));
    }
}

#endif
#if defined(MMI_2D_SENSOR_SUPPORT)||defined(CAM_MD_SUPPORT)

#include "gui.h"
#include "simdetectiongprot.h"
#include "quickaccessmenuitem_def.h"

#include "globalscrenum.h"
//#include "calculator.h"
//#include "unitconvertor.h"
//#include "currencyconvertor.h"
#include "poweronoffdisplaydefs.h"
#include "settingdefs.h"
//#include "calendardef.h"

//#include "organizerdef.h"
#include "factorymodedef.h"
#include "mmi_features_camera.h"
//#include "cameraresdefs.h"
#include "wallpaperdefs.h"
#include "screensaverdefs.h"

extern BOOL csd_mmi_is_screen_rotate_enable(void);
extern void (*RedrawCategoryFunction) (void);
extern BOOL mmi_audply_is_playing(void);
extern void mmi_audply_press_next_button_down();
extern void mmi_audply_press_next_button_up();
extern void mmi_audply_press_prev_button_down();
extern void mmi_audply_press_prev_button_up();
extern BOOL mmi_audply_is_rotate_enable(void);
extern BOOL csd_mmi_is_screen_rotate_enable(void);
extern void (*RedrawCategoryFunction) (void);
extern BOOL mmi_audply_is_playing(void);
extern void mmi_audply_press_next_button_down();
extern void mmi_audply_press_next_button_up();
extern void mmi_audply_press_prev_button_down();
extern void mmi_audply_press_prev_button_up();
extern BOOL mmi_audply_is_rotate_enable(void);
extern void mmi_audply_handle_rotate_event(mmi_frm_screen_rotate_enum rotate);
extern BOOL mmi_wallpaper_handle_rotate_event(mmi_frm_screen_rotate_enum rotate);
extern UI_image_ID_type  idle_screen_wallpaper_ID;
extern U16   gKeyPadLockFlag;
extern U8 isIdleScreenReached;
extern PHNSET_CNTX *g_phnset_cntx_p;
extern void VibratePlayOnce();
extern BOOL IsVibratorEnable(void);
extern BOOL IsRingEnable(void);
extern U8 PhnsetConstSysWallpaperList( U8 **pSysWallpaperStrList, U16 *pSysWallpaperImgIDList, S16 *HighliteItem );
extern void PhnsetSetWallpaperByImgID(U16 imgID);
extern void playRequestedTone(ALL_TONE_ENUM playtone);

#ifdef WIN32
UINT8 g_gpio_4_value,g_gpio_5_value;

void csd_2d_Sensor_switch_on(void)
{

}

void csd_2d_Sensor_switch_off(void)
{

}
#endif



#if defined(__MMI_ANALOGTV__)
extern void mmi_atv_rotate_switch(U8 direct); 
#endif

U8 csd_get_2d_Sensor_switch_setting(void);

#if defined(MMI_2D_SENSOR_SUPPORT)
BOOL g_2dSensor_enable_by_backlight = FALSE;
void csd_mmi_turnoff_2d_sensor_by_backlight(void)
{
	S16 error;
	U8 tmpmsensor;

    ReadValue(NVRAM_SETTING_2D_SENSOR, &tmpmsensor, DS_BYTE, &error);

    mmi_trace(1," csd_mmi_turnoff_2d_sensor_by_backlight %d ",tmpmsensor );

    if(tmpmsensor==0)
    {
        csd_2d_Sensor_switch_off();
        g_2dSensor_enable_by_backlight = TRUE;
    }
}
void csd_mmi_turnon_2d_sensor_by_backlight(void)
{
	S16 error;
	U8 tmpmsensor;

    ReadValue(NVRAM_SETTING_2D_SENSOR, &tmpmsensor, DS_BYTE, &error);

    if((tmpmsensor==0)&&g_2dSensor_enable_by_backlight)
    {
        csd_2d_Sensor_switch_on();
    }
}

void csd_mmi_app_handle_2d_sensor_message(BOOL para1,BOOL para2)
{
	static mmi_frm_screen_rotate_enum rotate ;
	mmi_trace(1, "csd_mmi_app_handle_2d_sensor_message(%d,%d)",para1,para2);

//handle the message until power on complete

      if(!IS_IDLE_ARRIVED)
            return;
	if((!para1) && (!para2))
		rotate = MMI_FRM_SCREEN_ROTATE_0;
	else if((!para1) && para2)
		rotate = MMI_FRM_SCREEN_ROTATE_90;
        else if(para1 && (!para2))
		rotate = MMI_FRM_SCREEN_ROTATE_270 ;
	else	
		rotate =MMI_FRM_SCREEN_ROTATE_180 ;
    
	mmi_frm_screen_rotate_req(rotate);
}

/*********************************************************
**FUNCTION:	csd_init_2d_Sensor_when_system_boot_up
**DESCRIPTION:	initial 2d Sensor setting according system
**		settings.
**PARAMETER:	NULL
**RETURN:	NULL
**
*********************************************************/
void csd_init_2d_Sensor_when_system_boot_up(void)
{
	if(csd_get_2d_Sensor_switch_setting()==0)
		csd_2d_Sensor_switch_on();
       else
		csd_2d_Sensor_switch_off();
}
#endif
#ifndef MMI_ON_WIN32
extern BOOL AudioStartStream(UINT32 *buffer, UINT32 len, DM_PlayRingType codec, BOOL loop_mode, DM_SpeakerGain volumn);
extern BOOL mmi_fmrdo_is_power_on( void);
#endif
#if defined(CAM_MD_SUPPORT)

#include "med_api.h"
#include "audioplayerdef.h"
void csd_handle_sw_motionsensor_event(void *data);
void csd_start_sw_motionsensor(void)
{
    
    if(!mmi_fmrdo_is_power_on())
    {
        if((!csd_get_2d_Sensor_switch_setting())&&(GetActiveScreenId()==SCR_ID_AUDPLY_MAIN||GetActiveScreenId()== IDLE_SCREEN_ID))
      {
     #ifdef MMI_ON_HARDWARE_P
            media_cam_md_open(MOD_MMI , 0, 0);
     #endif
            SetProtocolEventHandler(csd_handle_sw_motionsensor_event, MSG_ID_MEDIA_CAM_MD_WORK_IND);
           
        }
       
    }
}
void csd_stop_sw_motionsensor(void)
{
        if(!csd_get_2d_Sensor_switch_setting())
        {
            ClearProtocolEventHandler( MSG_ID_MEDIA_CAM_MD_WORK_IND);
	#ifdef MMI_ON_HARDWARE_P
            media_cam_md_close( MOD_MMI);
       #endif
    }
       
}

void csd_handle_sw_motionsensor_event(void *data)
{
        mmi_trace(1," csd_handle_sw_motionsensor_event  ");
        mmi_audply_handle_rotate_event(1);
        mmi_wallpaper_handle_rotate_event(1);
#if defined(__MMI_ANALOGTV__)    
        mmi_atv_rotate_switch(1);
#endif

}
#endif

/*************************************************************************
**FUNCTION:	csd_check_2d_Sensor_switch_setting
**DESCRIPTION:	This function is to check 2d Sensor's setting
**PARAMETERS:	NULL
**RETURNS:	if 2d Sensor switch setting is on,return 1;else 0
*************************************************************************/
U8 csd_get_2d_Sensor_switch_setting(void)
{
	U8 flag_switch;
	S16 error;
	
	ReadValue(NVRAM_SETTING_2D_SENSOR,&flag_switch,DS_BYTE,&error);
	if(0xFF == flag_switch)
		flag_switch = 1;
	return flag_switch;
}


void Phnset2dSensor(void)
{
	S16 error;
	U8 tmpmsensor;

	if(g_phnset_cntx_p->curHightlightItem )
	{
		tmpmsensor = 1;
		WriteValue(NVRAM_SETTING_2D_SENSOR,&tmpmsensor, DS_BYTE,&error);
 #if defined(MMI_2D_SENSOR_SUPPORT)       
		csd_2d_Sensor_switch_off();
 #endif
	}
	else
	{
		tmpmsensor = 0;
		WriteValue(NVRAM_SETTING_2D_SENSOR,&tmpmsensor, DS_BYTE,&error);
#if defined(MMI_2D_SENSOR_SUPPORT)
		csd_2d_Sensor_switch_on();
#endif
	}		
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
	DeleteNHistory(1);
} 

void HiliteHandler2DSensorSetting(S32 index) 
{
	g_phnset_cntx_p->curHightlightItem = (U8) index;
	return;
}

void EntryPhnset2dSensor(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; 
	U16 nNumofItem; 	 
	//U16 nDispAttribute; 
	U8* guiBuffer;
	U8 tmpmsensor;
	S16 error;
	
	EntryNewScreen(SCR_SETTING_2D_SENSOR, NULL, EntryPhnset2dSensor, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_SETTING_2D_SENSOR);
	nNumofItem = GetNumOfChild(MENU_SETTING_2D_SENSOR_SWITCH);

	GetSequenceStringIds(MENU_SETTING_2D_SENSOR_SWITCH,nStrItemList);
	SetParentHandler(MENU_SETTING_2D_SENSOR_SWITCH);

	RegisterHighlightHandler(HiliteHandler2DSensorSetting);
	ReadValue(NVRAM_SETTING_2D_SENSOR,&tmpmsensor, DS_BYTE,&error);
	if(tmpmsensor == 0xFF || tmpmsensor == 1)
		g_phnset_cntx_p->curr_2d_sensor = 1;
	else
		g_phnset_cntx_p->curr_2d_sensor = 0;

	ShowCategory11Screen(STR_SETTING_2D_SENSOR,
							MAIN_MENU_TITLE_SETTINGS_ICON,
							STR_GLOBAL_OK,
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,
							IMG_GLOBAL_BACK,
							nNumofItem,
							nStrItemList,
							g_phnset_cntx_p->curr_2d_sensor,
							guiBuffer);
	
	SetLeftSoftkeyFunction(Phnset2dSensor,KEY_EVENT_UP);
	SetKeyHandler(Phnset2dSensor, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


void HighlightPhnset2dSensor(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetLeftSoftkeyFunction(EntryPhnset2dSensor,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnset2dSensor, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
	

void mmi_audply_handle_rotate_event(mmi_frm_screen_rotate_enum rotate)
{
    if(mmi_audply_is_playing()&&mmi_audply_is_rotate_enable()&&!gKeyPadLockFlag)
    {
        switch(rotate)
        {
            case MMI_FRM_SCREEN_ROTATE_90:
                    mmi_audply_press_prev_button_down();
                    mmi_audply_press_prev_button_up();
                break;
            case MMI_FRM_SCREEN_ROTATE_270:
                    mmi_audply_press_next_button_down();
                    mmi_audply_press_next_button_up();
                break;
            default:
                break;
        }
    }

}

static const U8 motion_tone[] = 
{
0xFF,0xF2,0x48,0xC0,0x2A,0x1B,0x00,0x1B,
0x59,0xBE,0x96,0x27,0x43,0x58,0x02,0xB2,
0x30,0xE8,0x00,0x24,0x60,0x66,0x88,0x10,
0x42,0x61,0xD1,0x0B,0xF4,0xC3,0x88,0x42,
0x77,0x73,0x44,0x7A,0x66,0xF6,0x80,0xD8,
0x03,0xC1,0xF0,0x7B,0x00,0x40,0x24,0x1F,
0x15,0x25,0x93,0xCF,0xDB,0x1E,0x6E,0xF7,
0xEC,0x63,0x36,0x31,0x95,0x2F,0x7B,0xE1,
0x33,0xEF,0xB8,0x63,0x2B,0x63,0x19,0xEF,
0x7D,0xEF,0x7D,0xFB,0x19,0xCB,0x19,0x5F,
0xFE,0xF7,0xDF,0x0C,0x65,0x3C,0xD0,0xE3,
0x0B,0x87,0xE0,0xFB,0xF2,0x87,0x38,0x7F,
0xFF,0xF9,0x73,0xE0,0xF8,0x7F,0x2F,0x29,
0x2E,0x18,0x5C,0x10,0x51,0xCF,0x89,0xCF,
0x88,0xC3,0xFF,0x28,0x70,0xA1,0xC8,0x3E,
0xF0,0xFF,0x2E,0x7D,0x5F,0x4E,0xAB,0x91,
0x85,0x80,0x10,0x41,0x86,0x0B,0x07,0x13,
0xC8,0x17,0x41,0x41,0x3A,0x9C,0x87,0xA4,
0xFF,0xF2,0x48,0xC0,0x3F,0x43,0x13,0x1E,
0x59,0x96,0xE6,0x37,0x86,0x90,0x02,0x85,
0x13,0x4D,0xCD,0xA2,0xD3,0x51,0xA1,0x89,
0x74,0xBC,0x92,0xCC,0x4B,0x81,0xC5,0x80,
0xA1,0x85,0xA4,0x28,0x25,0x22,0x84,0x22,
0xCE,0x18,0x34,0x49,0x58,0x56,0xE5,0xB2,
0xB9,0x30,0x89,0xF2,0x8D,0x15,0x1E,0x77,
0x46,0x5A,0x2F,0xA8,0xB6,0x5C,0x45,0x55,
0xAE,0x93,0xD4,0x8B,0x31,0xE4,0xD4,0xE8,
0x26,0x8A,0x9F,0xF5,0x55,0x5B,0xBA,0x09,
0x94,0xC9,0xF2,0xA9,0x7E,0xF0,0x90,0x75,
0x90,0xA9,0x5A,0x90,0x0E,0x81,0x00,0x9D,
0xA1,0xA9,0xEC,0xF5,0x3E,0x7F,0xF8,0x84,
0xEF,0xD4,0xCF,0xFA,0x25,0x06,0x3B,0xF2,
0xCD,0x62,0xBF,0x54,0x51,0x5B,0x73,0x5A,
0xF8,0x40,0x95,0x01,0xF8,0x88,0x42,0x29,
0x51,0x42,0x04,0x82,0x70,0xC2,0x0C,0x9C,
0xA3,0xCB,0xA5,0x56,0xFF,0xDC,0xA5,0xD9,
0xFF,0xF2,0x48,0xC0,0x2B,0x65,0x1A,0x14,
0xF8,0xBE,0xC6,0x37,0xC3,0xC0,0x02,0x65,
0xA6,0x45,0x9C,0xB2,0x0C,0x52,0x0D,0x03,
0x81,0x02,0x33,0x37,0x2D,0xFF,0x87,0xED,
0xEA,0x57,0x6C,0xA2,0x55,0x7F,0xE8,0x17,
0x17,0x47,0xD1,0xA9,0x23,0x4D,0x0B,0x0B,
0x7F,0x01,0x84,0x45,0x42,0x87,0x4C,0x99,
0x2F,0xFF,0xFF,0xFF,0xFF,0x59,0xA5,0x2A,
0xBD,0xCC,0x37,0xD0,0xD4,0xBE,0xC5,0xEC,
0xD7,0x72,0xD9,0x23,0x00,0x41,0xFF,0x1B,
0x23,0xEE,0x19,0x03,0x1B,0x15,0x6F,0x36,
0x25,0x38,0x46,0xD5,0xDB,0x6F,0xFF,0xF9,
0x60,0x0C,0x8E,0x45,0x48,0x02,0xC1,0xA4,
0x89,0x03,0x00,0x80,0x58,0x72,0x4E,0x8B,
0x6C,0xE4,0xB6,0xCB,0x3C,0x2D,0x48,0xA4,
0x8A,0x48,0x8E,0x0C,0x3C,0x80,0x20,0x19,
0x21,0x84,0xE8,0x86,0x3F,0xFF,0xFF,0x2C,
0x41,0x17,0x36,0x14,0x96,0x15,0x57,0xFF,
0xFF,0xF2,0x48,0xC0,0x00,0xCF,0x47,0x15,
0x49,0x42,0xCA,0x46,0x1A,0x4C,0x92,0xF6,
0xF1,0xD4,0xDB,0xFE,0xFF,0xD5,0x73,0x54,
0x2F,0x34,0x32,0xDF,0xF4,0xFB,0x2D,0xC6,
0xA4,0x00,0x97,0x6E,0x1A,0xE4,0xE2,0x81,
0x99,0x95,0x75,0x27,0xAE,0xEB,0x15,0xEE,
0x65,0xA6,0x35,0x9B,0xE2,0xF7,0x7D,0x23,
0x80,0x3E,0x54,0x39,0x31,0xD6,0x14,0x4F,
0x46,0x16,0x7A,0xA0,0xF5,0x85,0x58,0x98,
0xB8,0xEA,0xF8,0x6A,0x48,0xB7,0x8C,0xD1,
0x62,0x72,0x8B,0x05,0x26,0x16,0x0A,0x47,
0x0B,0x35,0xFB,0x6D,0x34,0x6E,0x22,0x56,
0x8F,0x7E,0xEF,0xE5,0x8E,0xFF,0xF5,0x7B,
0xA9,0xD8,0xDA,0x15,0xBB,0xFA,0xB5,0x5B,
0x6D,0x26,0x0E,0xCE,0x01,0xF4,0x06,0x83,
0x4C,0x2B,0x2F,0xA8,0x7D,0x3B,0x68,0xDE,
0x05,0x88,0x78,0x80,0x9C,0x97,0x17,0xAA,
0xFD,0xD7,0x79,0x8E,0xF5,0xAD,0xE3,0x77,
0xFF,0xF2,0x48,0xC0,0xA7,0x2F,0x72,0x16,
0xF9,0x46,0xAA,0x2E,0x1B,0xD0,0x92,0x1D,
0xDB,0xB4,0x8E,0xC5,0x64,0x57,0x50,0xD4,
0x33,0x51,0xC8,0x8E,0x82,0x9A,0x29,0x01,
0xA6,0xBB,0x28,0x55,0x51,0xB9,0x79,0xE9,
0x54,0x9A,0x9E,0xB8,0x04,0xBB,0x28,0x7D,
0x60,0x13,0x15,0x46,0x7A,0x87,0xCC,0xF5,
0xF7,0xF7,0xB8,0xC6,0xAA,0xEA,0x78,0x90,
0xEA,0x15,0x55,0x0E,0x61,0xF3,0x20,0xA1,
0xD7,0xC0,0x0A,0xB1,0x85,0x5D,0xFF,0xCF,
0x45,0x68,0x8B,0x04,0xC7,0x28,0xF4,0x6E,
0xBE,0xA7,0xBD,0x24,0xCA,0x0F,0x92,0x0A,
0xA2,0xC1,0xEC,0xC3,0x6F,0x0E,0xA5,0xC6,
0xEC,0x6A,0x39,0x29,0xC4,0xA8,0x1A,0xA9,
0xA1,0x6A,0x9B,0x96,0xFB,0x6F,0x0F,0x44,
0x39,0x66,0x7B,0x94,0xE3,0x38,0xED,0x7D,
0xCA,0xFA,0xC8,0xE8,0x48,0x01,0xB4,0x16,
0xB7,0x23,0x35,0xC5,0x4C,0xCA,0x7E,0xA5,
0xFF,0xF2,0x48,0xC0,0x35,0xA1,0x97,0x1C,
0x51,0x8A,0xA1,0x6E,0x1E,0x0C,0x92,0xE3,
0xB5,0xB2,0x42,0x77,0xDE,0xA1,0xF0,0x1F,
0xFB,0xD7,0x70,0xA9,0x87,0x38,0x52,0xAF,
0xB6,0xC5,0x8B,0x05,0x12,0xA0,0xC2,0xD5,
0xA1,0x61,0xC2,0x3C,0xF4,0x81,0x5D,0x63,
0xFB,0xFF,0x58,0xF3,0x53,0x5A,0x9E,0xD1,
0x61,0xEF,0xDF,0x55,0xB6,0xF3,0x6F,0x6D,
0xFF,0xFF,0xCF,0xB7,0xA4,0x08,0x34,0xB5,
0xAF,0x68,0xD4,0x09,0x45,0x05,0x96,0xB0,
0x3A,0xDD,0xFF,0xFF,0xFF,0xFA,0x13,0x62,
0xB7,0xFE,0x06,0x6B,0x5C,0x2D,0xEF,0x49,
0xAE,0x67,0x64,0x55,0xA0,0x03,0x42,0x08,
0x65,0x60,0x74,0x30,0x03,0x6D,0x02,0x05,
0x56,0xDA,0x04,0xC0,0xEF,0x4F,0x27,0x6E,
0x69,0x99,0x3A,0x70,0xD2,0x82,0x46,0x46,
0xD3,0x89,0x9E,0xA8,0xAC,0xBA,0xDC,0xD1,
0x22,0xF2,0x68,0x39,0x17,0x37,0x34,0x52,
0xFF,0xF2,0x48,0xC0,0xBB,0x03,0xA6,0x1D,
0x6A,0x02,0xA6,0x37,0x46,0x78,0x02,0x0C,
0x8A,0x8C,0x8B,0xD5,0xAE,0xE8,0x26,0x9D,
0x66,0x66,0xE8,0xA2,0x70,0xC4,0x99,0x2E,
0xB2,0xF5,0x33,0x10,0x43,0x13,0x73,0x73,
0x32,0x7C,0x31,0x20,0x62,0x71,0x50,0x06,
0xA4,0x0D,0xA2,0x0D,0xD1,0x03,0x69,0x0B,
0xA4,0x9D,0xFF,0x06,0xE0,0x0E,0x50,0x2D,
0x98,0xE3,0x37,0x26,0xC8,0xB9,0xB8,0xE6,
0x05,0xF6,0x1A,0x64,0x38,0xAE,0x43,0x46,
0x38,0x66,0x86,0x58,0x8A,0xFF,0xB7,0x27,
0x0D,0x1D,0x34,0xFA,0x08,0x33,0x91,0x52,
0x1A,0x6C,0x63,0x38,0x4D,0x1B,0xEB,0xDB,
0x7D,0xFF,0x5B,0xD7,0xA7,0xD3,0x76,0xFD,
0x24,0xBF,0xFB,0x7F,0xF7,0x7B,0x32,0xDF,
0xFF,0xF4,0xD3,0x76,0xEB,0xFA,0x2A,0x49,
0xEA,0x49,0x68,0xA3,0x52,0x5F,0xF4,0x10,
0x64,0xD3,0xA7,0xA2,0x5D,0x20,0x24,0xD1,
0xFF,0xF2,0x48,0xC0,0xB2,0xDC,0xB1,0x2D,
0x7C,0x82,0xDA,0x57,0x86,0x90,0x02,0x16,
0x22,0xC6,0x26,0xAD,0x6D,0xD9,0x6D,0x6E,
0x30,0x01,0x25,0x80,0x09,0xDF,0xDF,0xC6,
0x20,0x8E,0x25,0xD4,0x48,0x47,0x60,0xD4,
0x38,0x01,0x31,0xB9,0xAC,0xF6,0x6B,0xA4,
0xAE,0x5E,0xBF,0x16,0x3E,0xF8,0x93,0x83,
0xAA,0xA3,0x7A,0x3F,0x33,0x9A,0x97,0xFF,
0xFF,0xFC,0x7F,0xBF,0x6B,0xE7,0x68,0xC0,
0x51,0xA0,0x47,0x24,0x47,0x7B,0x6B,0xE3,
0xB6,0xB4,0xB5,0xAE,0x05,0xC4,0x41,0x22,
0xD3,0x4D,0xC8,0x07,0xEF,0x1A,0x13,0x96,
0x7E,0x8F,0x41,0x1F,0x50,0xBB,0xAC,0xBC,
0x51,0x6D,0xD0,0xA3,0xC9,0xF1,0x5B,0xBA,
0xF5,0x6A,0x7B,0x5B,0x4E,0x8A,0xFE,0xCB,
0x87,0x3A,0x2E,0x46,0xF4,0x1F,0x90,0x36,
0x18,0x8C,0x41,0xA1,0xB2,0x84,0x03,0x57,
0x57,0xAC,0xCE,0xE1,0xC6,0xCF,0x46,0xAB,
0xFF,0xF2,0x48,0xC0,0x83,0xA3,0x7C,0x19,
0x49,0x76,0xD2,0x4F,0x83,0x30,0x02,0xF6,
0x82,0x48,0x48,0x13,0x4A,0xB3,0x5A,0x3A,
0x8A,0x07,0x9C,0x8D,0xB2,0xAD,0xD5,0x16,
0xEF,0xD9,0x1F,0x37,0x73,0xFB,0xD5,0x6D,
0x1A,0xCB,0xA2,0xCE,0xA3,0x24,0x93,0x43,
0x64,0x8F,0xA4,0x99,0x52,0xC4,0x31,0xB9,
0x08,0xD2,0x61,0x24,0xE1,0x25,0xA4,0xDB,
0x2E,0x72,0xFD,0xD7,0xBA,0xD9,0xB3,0xA9,
0xE3,0x75,0xAE,0xE7,0x28,0x3C,0x31,0x5E,
0x86,0x9D,0xFF,0xFF,0xFF,0xF7,0x55,0x24,
0x7E,0xD6,0xF7,0x97,0xD4,0xF5,0x20,0xEA,
0xDA,0xCB,0xEC,0x65,0x61,0x29,0xC6,0xA8,
0x5F,0xA0,0x36,0x12,0x99,0x92,0x91,0xB1,
0x20,0x60,0x66,0x22,0x37,0xAB,0x79,0xEF,
0xF5,0x76,0xE6,0xF5,0xCD,0x76,0xE4,0x3A,
0x38,0xB7,0x85,0x81,0xB4,0xA7,0x55,0xB7,
0x96,0xC2,0x22,0xB4,0x16,0xE6,0x2D,0x6F,
0xFF,0xF2,0x48,0xC0,0x12,0x7A,0x97,0x1A,
0xD9,0xF6,0xB6,0x2F,0xC3,0x58,0x02,0x0C,
0xB2,0xC3,0x9C,0xFE,0x73,0xBF,0x50,0x8E,
0x54,0xFD,0x2B,0x3B,0x96,0xCD,0x43,0x4C,
0x54,0xBA,0x33,0xB2,0x24,0x25,0x08,0xA8,
0x48,0x48,0x3E,0x48,0xC0,0x34,0x05,0x8F,
0x3C,0x94,0x3E,0xFA,0x94,0x74,0xE3,0x1B,
0x38,0x7F,0x28,0x37,0xEB,0x32,0x78,0xAA,
0x6B,0xB9,0xAD,0xD4,0x6D,0xAD,0x3D,0xBF,
0xFF,0xBF,0x53,0x86,0x38,0xF7,0xFF,0xFF,
0xFF,0xC4,0x2B,0x43,0x6F,0xA4,0x60,0x2C,
0x05,0x3C,0x59,0x02,0xA4,0x24,0x8B,0x1A,
0xAA,0xF1,0x8D,0x65,0xE0,0x43,0x2A,0x38,
0xA8,0x0F,0x90,0x36,0x19,0x90,0x4E,0x38,
0xC4,0xC0,0xA9,0x82,0xEB,0xD1,0xAD,0xAA,
0x6E,0xDA,0xAD,0xF7,0xBF,0x9F,0xB2,0xEC,
0x0C,0xC8,0xC6,0x7E,0x55,0x0D,0xD5,0x99,
0xDD,0x75,0x6B,0x57,0x3B,0xD6,0xBF,0xA6,
0xFF,0xF2,0x48,0xC0,0xB5,0xC9,0xAC,0x20,
0x62,0x36,0xB2,0x2E,0x1E,0x12,0xB6,0xAB,
0xAD,0xAD,0xDB,0xBB,0x8D,0x8C,0xB7,0x2A,
0xD0,0x5A,0xAC,0x74,0x1E,0xA3,0x0F,0xB1,
0x06,0x4C,0x3C,0x58,0x54,0x30,0x1C,0x9E,
0x53,0x01,0xA2,0xC3,0xC5,0xC1,0xC1,0x41,
0x74,0x59,0x47,0xE3,0xDD,0x26,0xAA,0x69,
0x99,0xA6,0x20,0xAB,0xA3,0x93,0xED,0x5B,
0xBF,0xFF,0xFF,0xFE,0x5D,0x2C,0xA5,0xE8,
0x65,0x1F,0xC9,0xC5,0x2C,0xC3,0x71,0x41,
0xE2,0x25,0xEC,0x0C,0x81,0x68,0xCF,0x81,
0x62,0x34,0x91,0xE0,0x91,0xC2,0xC4,0x05,
0x4B,0x6A,0xB9,0x82,0xFE,0x7F,0x8C,0xC2,
0x87,0x74,0xE0,0x19,0xCF,0xF4,0xA8,0xC0,
0x2B,0x4D,0x46,0x15,0x52,0xA3,0xB7,0x3D,
0xC6,0xB5,0xF5,0xBB,0x5B,0x5E,0x97,0xC6,
0x33,0xF3,0x27,0x92,0x1E,0x2B,0x88,0xD4,
0xF5,0x8B,0x4D,0x6B,0xE3,0x0C,0x6C,0x0C,
0xFF,0xF2,0x48,0xC0,0x7B,0xA2,0xAB,0x1A,
0xD9,0xFE,0xB6,0x2E,0x1B,0xD0,0xB6,0x2B,
0x1D,0x3E,0xA9,0x1B,0xC5,0xB8,0xD6,0x55,
0x1B,0xCD,0x4A,0x96,0x64,0x6B,0x06,0x53,
0xB3,0x34,0xB8,0x44,0xCE,0xB1,0x4B,0x53,
0xE3,0x52,0xC0,0xDC,0x47,0xAE,0x3A,0x72,
0x78,0xCB,0x7D,0x52,0x67,0x9F,0x39,0xF9,
0xFF,0x1F,0xE7,0x52,0x78,0x3F,0xFF,0xFF,
0xFE,0xE2,0x48,0x75,0xB7,0x69,0x90,0xB9,
0x4A,0x7A,0x92,0xE3,0xDB,0xDA,0x3B,0x8B,
0xA5,0x1F,0x6C,0x0C,0x81,0x68,0xD6,0x85,
0xB8,0xE0,0x98,0x2B,0x39,0xE6,0x54,0x0F,
0xFA,0xC4,0xC4,0x3A,0x52,0x15,0x27,0xCF,
0xAB,0x19,0x76,0x08,0xCD,0x0E,0x84,0xD3,
0x7A,0xE1,0x89,0x86,0x6F,0x0A,0xFF,0x1E,
0xFB,0xDE,0xBF,0xCE,0xFD,0x3F,0xDD,0xB5,
0xAD,0xB5,0xDE,0x48,0xDA,0x9A,0x3E,0x73,
0xBC,0x4F,0x6C,0x40,0x70,0x7E,0xEA,0x45,
0xFF,0xF2,0x48,0xC0,0xAB,0x7E,0xC0,0x20,
0x0A,0x3A,0xA5,0x6E,0x1B,0xDE,0xB6,0x49,
0xEC,0xD4,0xA0,0x2F,0x8A,0x12,0x6A,0x7C,
0x1A,0x29,0x74,0x34,0xBC,0xA9,0x15,0xAA,
0x46,0x17,0x1B,0x67,0x5F,0x5F,0x38,0x7B,
0x68,0xB2,0x47,0x84,0xFA,0xB1,0xDF,0x38,
0x77,0x70,0xE3,0xC0,0x81,0x14,0x53,0x90,
0x8C,0xFF,0xFF,0xFF,0xFF,0xFA,0x74,0x90,
0xEB,0x4B,0x76,0x0E,0x43,0x85,0x93,0x1B,
0x59,0x02,0xE4,0x0B,0x49,0x30,0x8D,0x1F,
0x61,0x04,0x1C,0x1F,0x43,0x4B,0x7A,0x5F,
0xC6,0xAE,0x33,0xF5,0x7A,0xDD,0xF1,0x92,
0x0C,0xF3,0xC4,0x93,0x1F,0x8A,0x03,0xA9,
0xC6,0x0C,0x77,0xD1,0x7D,0x6D,0x2C,0xD6,
0x85,0x8C,0xD3,0x38,0xB6,0xBE,0xBE,0x3C,
0x5C,0x7A,0x5E,0x4D,0xB2,0xEE,0x0A,0xB6,
0xD1,0xA3,0xDA,0x67,0x07,0x04,0xB3,0x1A,
0x72,0x75,0x43,0x2E,0x1B,0xD5,0x90,0x0D,
0xFF,0xF2,0x48,0xC0,0x02,0xE3,0xC0,0x1D,
0xBA,0x02,0xA9,0x6E,0x1B,0xDE,0xB6,0x02,
0xBA,0x55,0x5A,0x62,0x32,0x59,0xFC,0xF9,
0xBF,0x85,0x07,0x75,0x95,0xAB,0x7B,0x8F,
0x0B,0x30,0x23,0xB5,0x33,0xBD,0xD6,0x2A,
0xC5,0x24,0x3A,0x6E,0x0E,0xB5,0xF7,0xFF,
0xFB,0xDE,0xB3,0x78,0xBF,0xFF,0xFF,0xFF,
0xDB,0xD7,0xEA,0xE8,0xA6,0x36,0xB5,0xE5,
0xF8,0xBA,0x16,0x9C,0x7C,0x81,0x20,0x90,
0xC6,0xEA,0xAD,0x92,0xD3,0x21,0x08,0xB4,
0x6D,0x66,0x3E,0x27,0x8D,0x69,0xA7,0xFA,
0xF6,0x91,0x76,0x05,0x1B,0x1E,0x49,0x8A,
0x3F,0x46,0xBD,0x92,0x0B,0x73,0x9E,0xED,
0x99,0xB6,0xEB,0x54,0xB6,0xAD,0x9A,0xEB,
0xC4,0x85,0x7C,0xC2,0xB3,0xE8,0x49,0xE5,
0xA8,0x3A,0x82,0xD2,0xF6,0x68,0x6E,0x19,
0x64,0x67,0x8E,0xAA,0x3A,0xD4,0xA7,0x4A,
0xE9,0x54,0x9E,0x53,0xA6,0xD7,0x4A,0x32,
0xFF,0xF2,0x48,0xC0,0x7A,0x1B,0xCA,0x1E,
0xEA,0x3E,0xA5,0x8E,0x1B,0xDE,0xB6,0xC2,
0x54,0x19,0x4D,0xCA,0x16,0xF3,0xA9,0xC9,
0xEC,0xD4,0x9F,0x3F,0x10,0x34,0xFB,0xC0,
0xBB,0xC9,0xB5,0x2D,0x7C,0x0A,0xDB,0x0F,
0x61,0x4C,0xEF,0xFF,0xFF,0xFF,0xEB,0x11,
0xAA,0x7F,0xFF,0xFF,0xFF,0xAB,0xAA,0x9B,
0x24,0xAF,0x4F,0xF4,0xFA,0x7A,0x6F,0x06,
0x44,0xA1,0xF4,0x04,0x48,0x51,0x3E,0xC0,
0x42,0x58,0xE3,0x35,0x55,0x85,0x1B,0x3A,
0xCC,0x18,0xB8,0xB7,0xB5,0xEA,0xDA,0xA9,
0x0C,0xE0,0x0E,0x84,0xC9,0x0D,0x89,0x2C,
0xDB,0xAD,0x6B,0x06,0xDE,0xB5,0xAD,0x7F,
0xB7,0xB6,0xFC,0x63,0x4D,0x4A,0x31,0x8C,
0x52,0x22,0x15,0x4B,0xF5,0x5D,0x2F,0xFD,
0x88,0x40,0x30,0x02,0x0D,0x22,0x15,0x22,
0x95,0xA1,0xD8,0xAA,0x86,0x9A,0x58,0x54,
0x88,0x84,0x32,0xCC,0x11,0x35,0x74,0x8A,
0xFF,0xF2,0x48,0xC0,0x0C,0xFA,0xCF,0x1F,
0x52,0x36,0x99,0x6E,0x1B,0xDE,0xB6,0x91,
0x6A,0xA8,0x48,0x49,0x73,0xDA,0xA8,0x60,
0xD5,0xE5,0x90,0x8A,0x9A,0x90,0x34,0x78,
0x15,0x02,0xDC,0xDF,0xFF,0xFF,0xFE,0x59,
0xFD,0xBC,0x4D,0xFF,0x11,0x56,0x77,0xF5,
0x7A,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xF2,0x48,0xC0,0x0C,0x8F,0xD2,0x1C,
0x8A,0x06,0x75,0x4C,0x1B,0xD2,0xB6,0xFF,
0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0xF2,0x48,0xC0,0x92,0x30,0xE0,0x18,
0x80,0x02,0x5C,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

};

#ifndef MMI_ON_WIN32
extern BOOL AudioStartStream(UINT32 *buffer, UINT32 len, DM_PlayRingType codec, BOOL loop_mode, DM_SpeakerGain volumn);
extern BOOL mmi_fmrdo_is_power_on( void);
#endif
BOOL mmi_wallpaper_handle_rotate_event(mmi_frm_screen_rotate_enum rotate)
{

	S32 i;
	U16 SysWpIDList[MAX_WALLPAPER_IMG], ImgId = 0;

	mmi_trace(4,"EntryIdleShakeChangeWallpaper");
	
	if((GetActiveScreenId()==IDLE_SCREEN_ID)&&idle_screen_wallpaper_ID&&!gKeyPadLockFlag&&isIdleScreenReached)
	{

		g_phnset_cntx_p->wpss_type = MMI_PHNSET_TYPE_WP;
		memset(SysWpIDList, 0, sizeof(SysWpIDList));
              PhnsetConstSysWallpaperList(NULL, SysWpIDList, NULL);
                switch(rotate)
                {
                    case MMI_FRM_SCREEN_ROTATE_90:
                        if(IsVibratorEnable())
                        {
                            mmi_trace(1," PlayVibratorOnce  ");
                            VibratePlayOnce();
                           // playRequestedTone(WARNING_TONE); //add by panxu 20061228
                        }
                        {
                           // AudioStartStream((UINT32 *)motion_tone, sizeof(motion_tone), 13, 0, 4);
                            mdi_audio_play_string((void*)motion_tone,sizeof(motion_tone),MDI_FORMAT_DAF,2,NULL,NULL);
                        }

                           
                        if(IsVibratorEnable())
                        {
                            mmi_trace(1," PlayVibratorOnce  ");
                            VibratePlayOnce();
                          //  playRequestedTone(WARNING_TONE); //add by panxu 20061228
                        }
                        #if 0
                        if(!mmi_audply_is_playing()&&!mmi_fmrdo_is_power_on()&&IsRingEnable())
                               AudioStartStream((UINT32 *)motion_tone, sizeof(motion_tone), 13, 0, 4);
                        #endif
                        // COS_Sleep(10);
                		i = MAX_WALLPAPER_IMG-1;
                        while (i >0)
                        {
                            if (idle_screen_wallpaper_ID == SysWpIDList[i])
                            {
                                i--;
                                i %= MAX_WALLPAPER_IMG;
                                ImgId = SysWpIDList[i];
                                break;
                            }
                            i--;
                        }	
                        if(i == 0)
                        {   
                            if(idle_screen_wallpaper_ID == SysWpIDList[0] )
                            {
                                i = MAX_WALLPAPER_IMG-1;
                                while(SysWpIDList[i]==0&&i>0)
                                    i--;
                                ImgId = SysWpIDList[i];               
                            }
                            else
                            {
                                ImgId = SysWpIDList[0];   
                            }
                        }
            		PhnsetSetWallpaperByImgID(ImgId);
                   mmi_trace(1," IsVibratorEnable  ");
                   if(mmi_audply_is_playing()||mmi_fmrdo_is_power_on())
                   {
                           // mdi_audio_resume_background_play();                       
                   }
                   else
                   {
                     //  AudioStopStream();
                   }                   
                  
                    break;
                    case MMI_FRM_SCREEN_ROTATE_270:
                        if(IsVibratorEnable())
                        {
                            mmi_trace(1," PlayVibratorOnce  ");
                            VibratePlayOnce();
                          //  playRequestedTone(WARNING_TONE); //add by panxu 20061228
                        }
                           // AudioStartStream((UINT32 *)motion_tone, sizeof(motion_tone), 13, 0, 4);
                            mdi_audio_play_string((void*)motion_tone,sizeof(motion_tone),MDI_FORMAT_DAF,2,NULL,NULL);

                        if(IsVibratorEnable())
                        {
                            mmi_trace(1," PlayVibratorOnce  ");
                            VibratePlayOnce();
                        }
    
                        i = 0;
                        while (i < MAX_WALLPAPER_IMG)
                        {
                            if (idle_screen_wallpaper_ID == SysWpIDList[i])
                            {
                                i++;
                                i %= MAX_WALLPAPER_IMG;
                                ImgId = SysWpIDList[i];
                                break;
                            }
                            i++;
                        }		
                        if (ImgId == 0)
                            ImgId = SysWpIDList[0];
                        PhnsetSetWallpaperByImgID(ImgId);
                        break;
                    default:
                        break;
            }

            return TRUE;
	}
    return FALSE;
}
#endif
#endif /* _PHONESETUP_C */
