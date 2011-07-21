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

/*******************************************************************************
 * Filename:
 * ---------
 *  PhoneSetup.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef PHONESETUP_H_
#define PHONESETUP_H_

/*************************************************************
 *  Header File Dependance Check
 *************************************************************/
/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-10 9:51 */
#include "wallpaperdefs.h"
#include "downloaddefs.h"
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-10 9:51 */

#include "gui_data_types.h"

#ifndef WALLPAPER_H
#error "WallpaperDefs.h should be included"
#endif

#ifndef _MMI_DOWNLOADDEFS_H
#error "DownloadDefs.h should be included"
#endif



/*************************************************************
 *  Defination
 *************************************************************/
#define MAX_SS_INLINE_EDIT				4
#define PHNSET_MAX_FILE_LEN				(41)	/* Same size as menuitem's length */
#define PHNSET_MAX_PATH_LEN				(260)
#define PHNSET_MAX_EXT_LEN				(5)

//qiff add for limit WP/SS/PWRON/PWROFF size
#if !defined(GLOBAL_MMI_IMAGE_LIMIT_SIZE)
#define GLOBAL_MMI_IMAGE_LIMIT_SIZE (200*1024)
#endif

/*************************************************************
 *  Enum Value
 *************************************************************/
typedef enum {
   SET_SS_SETTING_INLINE_CAPTION_STATUS,
   SET_SS_SETTING_INLINE_SELECT_OFF,
   SET_SS_SETTING_INLINE_SELECT_ON,
   SET_SS_SETTING_INLINE_SELECT_POWER_SAVING,
   SET_SS_SETTING_INLINE_CAPTION_WAITING,
   SET_SS_SETTING_INLINE_SELECT_5SEC,
   SET_SS_SETTING_INLINE_SELECT_15SEC,
   SET_SS_SETTING_INLINE_SELECT_30SEC,
   SET_SS_SETTING_INLINE_SELECT_1MIN,

   SET_SS_SETTING_INLINE_MAX_ID
} SET_SS_SETTING_INLINE_ENUM; 

typedef enum {
	SET_SS_STATUS_OFF = 0,
	SET_SS_STATUS_ON = 1,
	SET_SS_STATUS_POWERSAVING = 2
} SET_SS_SETTING_STATUS;

typedef enum {
	SETTING_TVOUT_STYLE_PHONE,
	SETTING_TVOUT_STYLE_FULLSCREEN
} setting_tvout_enum;

#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
typedef enum{
/*
	MMI_PHNSET_IDLECLKTYPE_NONE,
*/	
	MMI_PHNSET_IDLECLKTYPE_ANALOG,
	MMI_PHNSET_IDLECLKTYPE_DIGITAL,
	MMI_PHNSET_IDLECLKTYPE_MAX
}PHNSET_IDLECLKTYPE_ENUM;
#endif
/*************************************************************
 *  Structure Defination
 *************************************************************/
typedef struct {
   U16	wallpaper[(PHNSET_MAX_FILE_LEN+PHNSET_MAX_PATH_LEN+PHNSET_MAX_EXT_LEN)];
   U16	screensave[(PHNSET_MAX_FILE_LEN+PHNSET_MAX_PATH_LEN+PHNSET_MAX_EXT_LEN)];
#ifdef __MMI_SAVE_SS_SPEEDUP__ /*gdm modify to add  the switch of __MMI_SAVE_SS_SPEEDUP__  20091125*/
   U16	screensavelayer[(PHNSET_MAX_FILE_LEN+PHNSET_MAX_PATH_LEN+PHNSET_MAX_EXT_LEN)];
#endif
} PHNSET_WPSS_FILENAME_STRUCT;

#ifdef __MMI_POWER_ON_OFF_DISPLAY__
typedef struct {
   U16	pwrondisplay[(PHNSET_MAX_FILE_LEN+PHNSET_MAX_PATH_LEN+PHNSET_MAX_EXT_LEN)];
#ifdef __MMI_SAVE_PON_SPEEDUP__ /*gdm modify to add  the switch of __MMI_SAVE_PON_SPEEDUP__  20091125*/
   U16	pwrondisplaylayer[(PHNSET_MAX_FILE_LEN+PHNSET_MAX_PATH_LEN+PHNSET_MAX_EXT_LEN)];
#endif
   U16	pwroffdisplay[(PHNSET_MAX_FILE_LEN+PHNSET_MAX_PATH_LEN+PHNSET_MAX_EXT_LEN)];
} PHNSET_PWRONOFF_FILENAME_STRUCT;
#endif	

typedef struct {
   U8 month;
   U8 date;
   U8 hour;
   U8 minutes;
   U16 year;
} PHNSET_DATETIME_STRUCT;

typedef enum{
	MMI_PHNSET_TYPE_WP
#ifdef __MMI_SUB_WALLPAPER__
	,MMI_PHNSET_TYPE_SUB_WP
#endif
	,MMI_PHNSET_TYPE_SS
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
	,MMI_PHNSET_TYPE_PWRON
	,MMI_PHNSET_TYPE_PWROFF
#endif	
} phnset_wpss_type_enum;


/*
*** Phone Setup context structure
 */
typedef struct {

   S32 curHightlightItem;
   U16 ScrAnchor;  /* Screen Anchor */
   
   /* Set Time and Date */
   U8 currentCity;
   U16 CurHomeCityStrID;
   U8 *CurHomeCityStrPtr;
   U32 CurTimeFmt;
   U32 CurDateFmt;
   S32 currentDST;
   S32 prevDST;
   U8 *AM_PM_flag;

   PHNSET_DATETIME_STRUCT settime;

   /*NITZ*/
   pBOOL NITZDirtyFlag;
   U8 curNITZStatus;

   /*Phone Setup*/
   U8 curShowDtStatus;
   U8 curShowOwnNumStatus;
//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)
   U8 curShowOPNStatus;
#endif
   pBOOL ShowDtDirtyFlag;
   pBOOL ShowOwnNumDirtyFlag;
//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)
   pBOOL ShowOPNDirtyFlag;
#endif
   S32 MainCntrst;

   #ifdef __MMI_SUBLCD__
   S32 SubCntrst;
   #endif

   #ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
   U8 MenuStyle;
   #endif

   #ifdef __MMI_SET_DEF_ENCODING_TYPE__
   U8 EncodingType;
   #endif

   /* Language */
   U8 **ppLang;
   U8 LP;
   U8 UsrLangIdx;

 
#ifdef __FLIGHT_MODE_SUPPORT__
   U8 curFlightMode;	/* current mode */
   U8 selFlightMode;	/* selected mode */
#endif
 

   /* Wallpaper & Screen Saver */
   U8 WPSSPreviewScrFlag;
   U16 curtWpID;
   #ifdef __MMI_SUB_WALLPAPER__
   U16 curtSubWpID;
   #endif
   U16 curSsID;
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
   U16 curtPwrOnID;
   U16 curtPwrOffID;
 #endif
#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) 
 	U16 curtCLKType;
#endif
   U8 WPFctyImgNum;
   U8 SSFctyImgNum;
   U8 WPDwnlImgNum;
   U8 SSDwnlImgNum;
   S32 SSStatusInlineHilite;
   S32 SSWaitStatusInlineHilite;
   S32 curSSStatus;
   S32 curSSWaitTime;
	S32 isShort;
   
   /* Wallpaper & Screen Saver shall code identifier */
   phnset_wpss_type_enum wpss_type;
   
   U16 SysWPSSList[MAX_SYS_WPSS_NUMBERS];  
   U16 DwnlWPSSList[MAX_DL_WPSS_NUMBERS];
#ifdef __MMI_TOUCH_SCREEN__
   U16 pen_speed;
   U16 pen_color; 
   U8 is_default_calibration_set;
#endif /* __MMI_TOUCH_SCREEN__ */

#ifdef __MMI_TVOUT__
	U16	tvout_style;
	U16 	tvout_format;
#endif /* __MMI_TVOUT__ */
#if defined(MMI_2D_SENSOR_SUPPORT)||defined(CAM_MD_SUPPORT)
	U8 curr_2d_sensor;
#endif
#if defined(MOTION_SENSOR_SUPPORT)
	U8         curr_motion_sensor;
#endif

	U8 curEnableRollLedStatus; //luanyy add for tc9751

} PHNSET_CNTX;




/*************************************************************
 *  Global Variable
 *************************************************************/
extern PHNSET_CNTX *g_phnset_cntx_p;




/*************************************************************
 *  Function Prototype
 *************************************************************/
void PhnsetLangHiliteHandler(S32 index);
void PhnsetLangLSKHandler(void);
void EntryPhnsetLang(void);
void EntryPhnsetScr(void);
void ExitPhnsetScr(void);
void EntryPhnsetTimeAndDate(void);
void ExitPhnsetTimeAndDate(void);
void PhnsetTimeAndDateHiliteHandler(S32 index);
void PhnsetSetDTHistoryHdlr(void);
void PhnsetSetTimeResHdlr(void *info);
void PhnsetConstructCityList(void);
void HighlightPhnsetLang(void);
void HighlightPhnsetWelcomeText(void);
void EntryPhnsetWelcomeText(void);
void ExitPhnsetWelcomeText(void);
//static void PhnsetFillWelcomeInlineEditStruct(void *InlineStruct);
void EntryPhnsetWelcomeTextDoneHdlr(void);
void HintPhnsetTimeAndDate(U16 index);
#if !defined(__MMI_NO_CONTRAST_LEVEL__)
void HighlightPhnsetContrastLevel(void);
void EntryPhnsetContrastLevel(void);
void PhnsetContrastLevelLskHdlr(void);
void HighlightPhnsetContrastLevelMain(S32);
void HighlightPhnsetContrastLevelSub(S32);
#endif
#ifdef __MMI_TOUCH_SCREEN__
/* Handwriting setting */
void HighlightHandwritingSetting(void);
void HighlightCalibrationSetting(void);
void SaveHandwritingSetting(void);
void GoBackHandwritingSettingConfirm(void);
void EntryScrHandwritingSettingConfirm(void);
void HandwritingSettingInlineColor(U16 index);
/* Pen calibration */
void EntryCalibrationSettingScr(void);
void ExitCalibrationSettingScr(void);
void WriteNvramCalibrationSet(BOOL done);
#endif /* __MMI_TOUCH_SCREEN__ */
void HighlightPhnsetNITZ(void);
void EntryPhnsetNITZ(void);
void PhnsetWelcomeTextDoneLskHdlr(void);
void HighlightPhnsetSetHomeCity(void);
void HighlightPhnsetSetDT(void);
void HighlightPhnsetSetFormat(void);
/* START: add day light saving setting */
void HighlightPhnsetSetDST(S32 index);
/* END:  add day light saving setting */
void EntryPhnsetSetDT(void);
void ExitPhnsetSetDT(void);
void PhnsetSetDT(void);
void EntryPhnsetSetFormat(void);
void ExitPhnsetSetFormat(void);
void PhnsetSetFormatHiliteHandler(S32 highliteIndex) ;
void PhnsetSetFormat(void);
void PhnsetSetFormatFillInlineStruct( void *);
//__MMI_SEPARATE_DT_SETTING__
void HighlightPhnsetSetTimeAndFormat(void);
void HighlightPhnsetSetDateAndFormat(void);
void EntryPhnsetSetTimeAndFormat(void);
void ExitPhnsetSetTimeAndFormat(void);
void EntryPhnsetSetDateAndFormat(void);
void ExitPhnsetSetDateAndFormat(void);
void PhnsetSetDTAndFormatFillInlineStruct( void *p );
void PhnsetSetDTAndFormat(void);
//__MMI_SEPARATE_DT_SETTING__
S16 PhnsetSetTimeFormat(U8 timeformat) ;
S16 PhnsetSetDateFormat(U8 dateformat);
void EntryPhnsetSetHomeCity(void);
void PhnsetHomeCityIdxHiliteHandler(S32 index);
void PhnsetSetHomeCity(void);
void ExitFunctionHomeCity(void) ;
U8 PhnsetHCGetNearbyIdx(UI_character_type inp);
void PhnsetHCMultiTapHdlr(UI_character_type );
void PhnsetHCMultiTapCompleteHdlr(void);
void PhnsetSetHomeCityPtr( U8* );
void HighlightPhnsetIdleScrDisp(void);
void ExitPhnsetIdleScrDisp(void);
void EntryPhnsetIdleScrDisp(void);
void PhnsetToggleShowOwnNo(void);
void HighlightPhnsetShowOwnNo(void);
//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)
void PhnsetToggleShowOPN(void);
void HighlightPhnsetShowOPN(void);
#endif
void PhnsetToggleShowDtTime(void);
void HighlightPhnsetShowDtTime(void);
void PhnsetToggleEnableRollLed(void); //luanyy add for tc9751
void HighlightPhnsetEnableRollLed(void); //luanyy add for tc9751
void PhnsetContrastLevelEndHdlr(void);
void PhnsetSetTimeResHdlrForAT(void*);
void HintPhnsetNITZ(U16);
void HintPhnsetWelcomeText(U16);
void HintPhnsetLanguage(U16);
void HintPhnsetOwnerNum(U16);
#if defined(MOTION_SENSOR_SUPPORT)
UINT8 MotionSensor_GetSetting(void);
#endif
//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)
void HintPhnsetOPN(U16);
#endif
void HintPhnsetShowDateTime(U16);
#if defined (__LED__) || defined (__MMI_RUNNING_LED__)	
void HintPhnsetEnableRollLed(U16);//luanyy add for tc9751
#endif
void HintPhnsetWallpaper(U16);
#ifdef __MMI_SUB_WALLPAPER__
void HintPhnsetSubWallpaper(U16);
#endif
void HintPhnsetScreenSaver(U16);

#ifdef __MMI_PREFER_INPUT_METHOD__
void HintPhnsetPreferedInputMethod(U16 index);
#endif /* __MMI_PREFER_INPUT_METHOD__ */

#ifdef __MMI_TOUCH_SCREEN__
void HighlightHandwritingSetting(void);
void HighlightCalibrationSetting(void);
void EntryHandWritingSettingScr(void);
void ExitHandWritingSettingScr(void);
#endif /* __MMI_TOUCH_SCREEN__ */

#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
void HintPhnsetMainMenuStyle(U16);
void HighlightPhnsetMainMenuStyle(void);
void EntryPhnsetMainMenuStyle(void);
void HighlightPhnsetMainMenuStyleSelect(void);
void PhnsetMainMenuStyleSelectLskHdlr(void);
U8 PhnsetGetMainMenuStyle(void);
#endif

#ifdef __MMI_SET_DEF_ENCODING_TYPE__
void HighlightPhnsetDefEncoding(void);
void HintPhnsetDefEncoding( U16 index );
void PhnsetDefEncodingHiliteHandler(S32 index);
void PhnsetDefEncodingSelectHandler(void);
void EntryPhnsetDefEncoding(void);
#endif /* __MMI_SET_DEF_ENCODING_TYPE__ */

 
#define __MMI_DISP_SETUP_LEVEL_UP__
#ifdef __MMI_DISP_SETUP_LEVEL_UP__
void HighlightDispSetup(void);
void EntryDispSetup(void);
#endif

 
#ifdef __FLIGHT_MODE_SUPPORT__
void HighlightPhnsetFlightMode(void);
void HintPhnsetFlightMode(U16);
void EntryPhnsetFlightMode(void);
#endif
 

#ifdef __MMI_TVOUT__
	extern U16 mmi_phnset_tvout_get_style(void);
#endif /* __MMI_TVOUT__ */

//void HintSpeedDialOn(U16);
//void DeInitDateTimeSetup(void);
//void HighlightGreetTextOption(S32 );
//void HighlightGreetStatus(S32 );
//void ExitPhnsetContrastLevel(void);
//void ExitPhnsetMainMenuStyle(void);
//void ExitLangSelectScreen(void);

#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
extern void HintPhnsetIdleClockType(U16 index);
extern void HighlightPhnsetIdleClockType(void);
extern void ReadPhnsetIdleClockType(void);
#endif
#ifdef __FT_IDLE_SMART_DIAL__
extern U16 IsSmartDialStyle(void);
extern void SetSmartDialStyle(U16 flag);
extern void ChangeDialStyle(void);
extern void InitDialStyle(void);
extern void SaveDialStyle(void);
extern void EntryPhnsetDialStyle(void);
extern void HighlightPhnsetDialStyle(void);
extern void HintPhnsetDialStyle(U16 index);
#endif

#endif /* PHONESETUP_H_ */





