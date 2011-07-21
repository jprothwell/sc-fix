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
 * res_setting.c
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
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 *============================================================================
 ****************************************************************************/
#include "stdc.h"
#include "customcfg.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif 

#include "mmi_features.h"
#include "phonebookdef.h"

#include "callsetupenum.h"
#include "settingdefs.h"
#include "networksetupgprot.h"
#include "scheduledpoweronoff.h"
#include "organizerdef.h"
#include "globalmenuitems.h"
#include "funandgamesdefs.h"
#include "callhistoryenum.h"
#include <dirent.h>
#define __ONLY_FOR_AUTO_RESGEN__
#include "gui_data_types.h"
#include "phonesetup.h"
#include "settingdefs.h"
#include "screensaverdefs.h"
#include "poweronoffdisplaydefs.h"
#include "profilesenum.h"
#include "shortcutsdefs.h"
#undef __ONLY_FOR_AUTO_RESGEN__

#ifdef __MMI_MULTI_SIM__
#include "mtpnp_ad_resdef.h"
#endif

//#include "custom_equipment.h"

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"

#define ISCATEGORY(x) (category & (1 << x))

extern void PopulateDisplayChar(void);
extern void PopulateBlackList(void);
extern void PopulateExtImage(void);

typedef enum
{
    MENU_ENUM_TIME_AND_DATE,
    MENU_ENUM_SETTING_SCHEDULE_POWER,
    MENU_ENUM_SETTING_LANGUAGE,
#ifdef __MMI_PREFER_INPUT_METHOD__
    MENU_ENUM_SETTING_PREFERED_INPUT_METHOD,
#endif 

#ifdef __MMI_SET_DEF_ENCODING_TYPE__
    MENU_ENUM_SETTING_DEF_ENCODING_TYPE,
#endif 

    MENU_ENUM_IDLE_SCR_DISP,

#if !defined(__MMI_NO_CONTRAST_LEVEL__)
    MENU_ENUM_SETTING_CONTRAST_LEVEL,
#endif 

    MENU_ENUM_WELCOME_TEXT,
    MENU_ENUM_SPEED_DIAL,

#if !defined(__MMI_VERSION_2__)
    MAIN_ENUM_SHORTCUTS,
#endif 

#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
    MENU_ENUM_DEDICATED_KEY,
#endif 

#if (defined(__MMI_NITZ__) && (!defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__)))
    MENU_ENUM_NITZ,
#endif 

#ifdef __FLIGHT_MODE_SUPPORT__
    MENU_ENUM_FLIGHT_MODE,
#endif 

#ifdef __MMI_TVOUT__
    MENU_ENUM_TVOUT,
#endif 

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    MENU_ENUM_HANDWRITING,
#endif 

/* 增加七彩灯的开关项*/
#ifdef RAINBOW_LIGHT_SUPPORT
    MENU_ENUM_RAINBOW_LIGHT,    
#endif

/*  增加键盘背光的颜色选项*/
#ifdef RAINBOW_LIGHT_SUPPORT
    MENU_ENUM_KEYBOARD_COLOR,
#endif

/*  增加跑马灯的开关项*/
#ifdef CIRCLE_LIGHT_SUPPORT
    MENU_ENUM_CIRCLE_LIGHT, 
#endif
#ifdef __MMI_CHANGABLE_FONT__
    MENU_ENUM_FONT_SIZE,
#endif 
    MENU_ENUM_GPIO_SETTING,

    MENU_ENUM_TOTAL_PHONE_SETUP
} PhoneSetupMenus;

typedef enum
{
#ifdef __MMI_STATUS_LED__
    MMI_GPIO_SET_MENU_LED,
#endif
    MMI_GPIO_SET_MENU_BKLITE,
#ifdef __MMI_COVER_CLOSE_SETTING__
    MMI_GPIO_SET_COVER_CLOSE,
#endif 
    MMI_GPIO_SET_MENU_MAX
}mmi_gpio_set_menu_enum;

typedef enum
{
#ifndef __MMI_HIDE_HOMECITY__
    MENU_SETDT_HOMECITY,
#endif 
#ifndef __MMI_SEPARATE_DT_SETTING__
    MENU_SETDT_TIMEANDDATE,
    MENU_SETDT_FORMAT,
#else /* __MMI_SEPARATE_DT_SETTING__ */ 
    MENU_SET_DATEANDFORMAT,
    MENU_SET_TIMEANDFORMAT,
#endif /* __MMI_SEPARATE_DT_SETTING__ */ 

#if (defined(__MMI_NITZ__) && defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__))
    MENU_SETDT_NITZ,
#endif 
    MENU_SETDT_NUM
} PhoneSetup_DateNTime_Menus;

typedef enum
{
#ifndef __MMI_REMOVE_CALLER_ID__
    MANAGECALL_CALLER_ID,
#endif 
#ifndef __MMI_DISABLE_CALL_TIME_DISPLAY__
    MANAGECALL_CALL_TIME_DISPLAY,
#endif 
#ifndef __MMI_REMOVE_CUG__
    MANAGECALL_CUG,
#endif 
#ifdef __IP_NUMBER__
    MANAGECALL_IP_NUMBER,
#endif 
#ifdef __MMI_CM_BLACK_LIST__
    MANAGECALL_BLACK_LIST,
#endif 
#ifdef __CTM_SUPPORT__
    MANAGECALL_TEXT_PHONE,
#endif 
    MANAGECALL_MENU_ITEM_COUNT
} managecall_menu_item_enum;

typedef enum
{
    DISPCHAR_SETTING_WALLPAPER,
#ifdef __MMI_SUB_WALLPAPER__
    DISPCHAR_SETTING_SUBLCD_WALLPAPER,
#endif 
    DISPCHAR_SETTING_SCR_SVR,
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
    DISPCHAR_SETTING_POWER_ON_DISPLAY,
    DISPCHAR_SETTING_POWER_OFF_DISPLAY,
#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */ 
#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
#ifndef PRODUCT_QPAD
    DISPCHAR_SETTING_MAINMENU_STYLE,
#endif 
#endif
    DISPCHAR_SETTING_SHOW_DT_TIME,
#if !((defined(__MMI_MULTI_SIM__))&&(defined( __MMI_MAINLCD_128X160__)||defined( __MMI_MAINLCD_128X128__)))                              
    DISPCHAR_SETTING_SHOW_OWN_NUMBER,
#endif    
#if defined(__MMI_OPERATOR_NAME_MENU__)
    DISPCHAR_SETTING_SHOW_OPN,
#endif 
#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
    DISPCHAR_SETTING_IDLE_CLOCK_TYPE,
#endif 
    DISPCHAR_MENU_ITEM_COUNT
} dispchar_menu_item_enum;

#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
typedef enum
{
#ifdef __MMI_MAINMENU_MATRIX_SUPPORT__
    MAINMENU_STYLE_MATRIX,
#endif 
#ifdef __MMI_MAINMENU_PAGE_SUPPORT__
#ifndef __MMI_TOUCH_SCREEN__
    MAINMENU_STYLE_PAGE,
#endif 
#endif /* __MMI_MAINMENU_PAGE_SUPPORT__ */ 
#ifdef __MMI_MAINMENU_LIST_SUPPORT__
    MAINMENU_STYLE_LIST,
#endif 
#ifdef __MMI_MAINMENU_CIRCULAR_SUPPORT__
    MAINMENU_STYLE_CIRCULAR,
#endif 
#ifdef __MMI_MAINMENU_ROTATE_SUPPORT__
    MAINMENU_STYLE_ROTATE,
#endif 
#ifdef __MMI_MAINMENU_TAB_SUPPORT__
    MAINMENU_STYLE_TAB,
#endif 
    MAINMENU_STYLE_COUNT
} mainmenu_style_enum;

#endif /* __MMI_MAINMENU_STYLE_CHANGE_EN__ */ 

typedef enum
{
    DISPSEL_MENU_SYS,
#ifndef __MMI_FILE_MANAGER_HIDE_MENU__
    DISPSEL_MENU_USER,
#endif 
    DISPSEL_MENU_TOTAL
} dispsel_menu_enum;

typedef enum
{
    AUTODISP_IMG_START,
    AUTODISP_IMG_MAX,
    AUTODISP_ANALOG_CLK,
    AUTODISP_DIGITAL_CLK,
    AUTODISP_VDO_START = AUTODISP_ANALOG_CLK,
    AUTODISP_VDO_MAX = AUTODISP_DIGITAL_CLK,
    AUTODISP_MAX
} autodisp_enum;

typedef enum
{
    AUTODISP_CATEGORY_IMG = 0x0020,
    AUTODISP_CATEGORY_VDO = 0x0040,
    AUTODISP_CATEGORY_MB = 0x0080,
    AUTODISP_CATEGORY_SB = 0x0100,
    AUTODISP_CATEGORY_ANALOG = 0x0200,
    AUTODISP_CATEGORY_DIGITAL = 0x0400,
    AUTODISP_CATEGORY_SWFLASH = 0x0800,
    AUTODISP_CATEGORY_MAX
} autodisp_category_enum;

FILE *res_log;  /* for log */

/*****************************************************************************
 * FUNCTION
 *  PopulateCallBarring
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateCallBarring()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_MENUITEM((MENU8237_SCR8093_MNGCALL_MENU_BARRING,
                              MENU8237_SCR8093_MNGCALL_MENU_MAIN, 4,
                              MENU_CALL_BARRING_OUTGOING,
                              MENU_CALL_BARRING_INCOMING,
                              MENU_CALL_BARRING_CANCELALL,
                              MENU_CALL_BARRING_CHANGEPASS, SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_CALL_BARRING, NULL));

    ADD_APPLICATION_MENUITEM((MENU_CALL_BARRING_CANCELALL,
                              MENU_BARRING_MAIN, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_CALL_BARRING_CANCELALL, NULL));

    ADD_APPLICATION_MENUITEM((MENU_CALL_BARRING_CHANGEPASS,
                              MENU_BARRING_MAIN, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_CALL_BARRING_CHANGEPASS, NULL));

    ADD_APPLICATION_STRING2(STR_MENU_CALL_BARRING, "Call Barring", "STR MENU CALL BARRING");

    ADD_APPLICATION_STRING2(STR_MENU_CALL_BARRING_CANCELALL, "Cancel All", "STR_MENU_CALL_BARRING_CANCELALL");
    ADD_APPLICATION_STRING2(STR_MENU_CALL_BARRING_CHANGEPASS, "Change Barring PW", "STR_MENU_CALL_BARRING_CHANGEPASS");

    ADD_APPLICATION_MENUITEM((MENU_CALL_BARRING_OUTGOING,
                              MENU8237_SCR8093_MNGCALL_MENU_BARRING, 3,
                              MENU_BARRING_OUTGOING_ALLCALLS,
                              MENU_BARRING_OUTGOING_INTERCALL,
                              MENU_BARRING_OUTGOING_INTEREXCEPTHOME,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_BARRING_OUT_CALLS_MAIN, NULL));

    ADD_APPLICATION_MENUITEM((MENU_BARRING_OUTGOING_ALLCALLS,
                              MENU_CALL_BARRING_OUTGOING, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_BARRING_OUTGOING_ALLCALLS, NULL));

    ADD_APPLICATION_MENUITEM((MENU_BARRING_OUTGOING_INTERCALL,
                              MENU_CALL_BARRING_OUTGOING, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_BARRING_OUTGOING_INTERCALL, NULL));

    ADD_APPLICATION_MENUITEM((MENU_BARRING_OUTGOING_INTEREXCEPTHOME,
                              MENU_CALL_BARRING_OUTGOING, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_BARRING_OUTGOING_INTEREXCEPTHOME, NULL));

    ADD_APPLICATION_STRING2(STR_MENU_BARRING_OUT_CALLS_MAIN, "Outgoing Calls  ", "STR_MENU_BARRING_OUT_CALLS_MAIN");

    ADD_APPLICATION_STRING2(STR_MENU_BARRING_OUTGOING_ALLCALLS, "All Calls  ", "STR_MENU_BARRING_OUTGOING_ALLCALLS");

    ADD_APPLICATION_STRING2(STR_MENU_BARRING_OUTGOING_INTERCALL,"International Calls ","STR_MENU_BARRING_OUTGOING_INTERCALL");

    ADD_APPLICATION_STRING2(STR_MENU_BARRING_OUTGOING_INTEREXCEPTHOME,"International Calls Except To Home PLMN","MENU_BARRING_OUTGOING_INTEREXCEPTHOME");

    ADD_APPLICATION_MENUITEM((MENU_CALL_BARRING_INCOMING,
                              MENU8237_SCR8093_MNGCALL_MENU_BARRING, 2,
                              MENU_BARRING_INCOMING_ALLCALLS,
                              MENU_BARRING_INCOMING_ROAMING,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_BARRING_INCOMING_MAIN, NULL));

    ADD_APPLICATION_MENUITEM((MENU_BARRING_INCOMING_ALLCALLS,
                              MENU_CALL_BARRING_INCOMING, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_BARRING_INCOMING_ALLCALLS, NULL));

    ADD_APPLICATION_MENUITEM((MENU_BARRING_INCOMING_ROAMING,
                              MENU_CALL_BARRING_INCOMING, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_BARRING_INCOMING_ROAMING, NULL));

    ADD_APPLICATION_STRING2(STR_MENU_BARRING_INCOMING_MAIN, "Incoming Calls ", "STR_MENU_BARRING_INCOMING_MAIN");

    ADD_APPLICATION_STRING2(STR_MENU_BARRING_INCOMING_ALLCALLS, "All Calls  ", "STR_MENU_BARRING_INCOMING_ALLCALLS");

    ADD_APPLICATION_STRING2(STR_MENU_BARRING_INCOMING_ROAMING, "When Roaming  ", "STR_MENU_BARRING_INCOMING_ROAMING");

    ADD_APPLICATION_MENUITEM((MENU_BARRING_OPTION,
                              0, 3,
                              MENU_BARRING_ACTIVATE,
                              MENU_BARRING_DEACTIVATE,
                              MENU_BARRING_QUERY, SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_BARRING_OPTION, NULL));

    ADD_APPLICATION_MENUITEM((MENU_BARRING_ACTIVATE,
                              MENU_BARRING_OPTION, 0, SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_ACTIVATE, NULL));

    ADD_APPLICATION_MENUITEM((MENU_BARRING_DEACTIVATE,
                              MENU_BARRING_OPTION, 0, SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_DETACTIVATE, NULL));

    ADD_APPLICATION_MENUITEM((MENU_BARRING_QUERY,
                              MENU_BARRING_OPTION, 0, SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_BARRING_QUERY, NULL));

    ADD_APPLICATION_STRING2(STR_MENU_BARRING_OPTION, "Call Barring Options", "STR_MENU_BARRING_OPTION");

    ADD_APPLICATION_STRING2(STR_MENU_BARRING_QUERY, "Query Status", "STR_MENU_BARRING_QUERY");

    ADD_APPLICATION_STRING2(STR_BARRING_OUTGOING_CALL_CAPTION, "Outgoing Calls  ", "STR_SCR_ACTIVATE_DEACTIVATE_RSK");

    ADD_APPLICATION_STRING2(STR_BARRING_PASSWORD_CAP, "Bar. Password  ", "STR_BARRING_PASSWORD_CAP");

    ADD_APPLICATION_IMAGE2(IMG_BARRING_ACIVATE_CAP, CUST_IMG_BASE_PATH "/NoImage.bmp", "IMG_BARRING_ACIVATE_CAP ");

    ADD_APPLICATION_IMAGE2(
        IMG_BARRING_DEACTIVATE_CAP,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "IMG_BARRING_DEACTIVATE_CAP ");

    ADD_APPLICATION_STRING2(STR_OLD_BARRINGPASS_CAP, "Old Password", "STR_OLD_BARRINGPASS_CAP");

    ADD_APPLICATION_STRING2(STR_NEW_BARRINGPASS_CAP, "New Password", "STR_NEW_BARRINGPASS_CAP");

    ADD_APPLICATION_STRING2(STR_CONFIRM_BARRINGPASS_CAP, "Confirm New Password", "STR_CONFIRM_BARRINGPASS_CAP");

    ADD_APPLICATION_IMAGE2(IMG_BARING_CAP, CUST_IMG_BASE_PATH "/NoImage.bmp", "IMG_BARING_CAP ");

    ADD_APPLICATION_STRING2(STR_BARING_CAP, "Barring", "STR_OLD_BARRINGPASS_CAP");

    ADD_APPLICATION_STRING2(
        STR_CHANGE_PSW_SUCCESS_TEXT,
        "Barring Password Changed Successfully !!",
        "STR_CHANGE_PSW_SUCCESS_TEXT");

    ADD_APPLICATION_STRING2(STR_BAOC_A, "Barring All Outgoing Calls ", "Barring All Outgoing Calls.");
    ADD_APPLICATION_STRING2(
        STR_BOIC_A,
        "Barring Outgoing International Calls ",
        "Barring Outgoing International Calls.");
    ADD_APPLICATION_STRING2(
        STR_BOICEXHC_A,
        "Barring Outgoing International Calls Except To Home PLMN ",
        "Barring Outgoing International Calls Except To Home PLMN.");
    ADD_APPLICATION_STRING2(STR_BAIC_A, "Barring All Incoming Calls ", "Barring All Incoming Calls.");
    ADD_APPLICATION_STRING2(
        STR_BAICROAM_A,
        "Barring All Incoming Calls When Roaming ",
        "Barring All Incoming Calls When Roaming.");
    ADD_APPLICATION_STRING2(STR_BAC_A, "Barring All Calls ", "Barring All Calls.");
    ADD_APPLICATION_STRING2(STR_BOC_A, "Barring Outgoing Calls ", "Barring Outgoing Calls.");
    ADD_APPLICATION_STRING2(STR_BIC_A, "Barring Incoming Calls ", "Barring Incoming Calls.");
    ADD_APPLICATION_STRING2(STR_DEFAULT_A, "Unknown Barring Service Type ", "Unknown Barring Service Type.");
    ADD_APPLICATION_STRING2(STR_ACTIVATED, "Activated\n", "Activated.");
    ADD_APPLICATION_STRING2(STR_DEACTIVATED, "Deactivated\n", "Deactivated.");
    ADD_APPLICATION_STRING2(STR_NOT_PROVIDED, "Not Provided\n", "Not Provided.");


    ADD_APPLICATION_STRING2(STR_SECONDS, "Seconds ", "Seconds.");
}


/*****************************************************************************
 * FUNCTION
 *  PopulateCallForward
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateCallForward(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_SS_CF_ALLCOND_MENU__
    ADD_APPLICATION_MENUITEM((MENU8237_SCR8093_MNGCALL_MENU_FWD,
                              MENU8237_SCR8093_MNGCALL_MENU_MAIN, 7,
                              MENU8238_MNGCALL_CALFWD_VOICECALL,
                              MENU8239_CALFWD_VOICECALL_UNREACHABLE,
                              MENU8239_CALFWD_VOICECALL_NOANSWER,
                              MENU8239_CALFWD_VOICECALL_BUSY,
                              MENU8239_CALFWD_VOICECALL_ALLCOND,
                              MENU8238_MNGCALL_CALFWD_DATACALL,
                              MENU_CALL_FWD_CANCELALL,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_MENU8238_MNGCALL_CALLFWD_CAPTION, NULL));
#else
    ADD_APPLICATION_MENUITEM((MENU8237_SCR8093_MNGCALL_MENU_FWD,
                              MENU8237_SCR8093_MNGCALL_MENU_MAIN, 6,
                              MENU8238_MNGCALL_CALFWD_VOICECALL,
                              MENU8239_CALFWD_VOICECALL_UNREACHABLE,
                              MENU8239_CALFWD_VOICECALL_NOANSWER,
                              MENU8239_CALFWD_VOICECALL_BUSY,
                              MENU8238_MNGCALL_CALFWD_DATACALL,
                              MENU_CALL_FWD_CANCELALL,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_MENU8238_MNGCALL_CALLFWD_CAPTION, NULL));
#endif

    ADD_APPLICATION_MENUITEM((MENU8238_MNGCALL_CALFWD_VOICECALL,
                              MENU8237_SCR8093_MNGCALL_MENU_FWD, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU8238_MNGCALL_CALLFWD_VOICE, NULL));

    ADD_APPLICATION_MENUITEM((MENU_CALL_FWD_CANCELALL,
                              MENU8237_SCR8093_MNGCALL_MENU_FWD, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_CALL_FWD_CANCELALL, NULL));

    ADD_APPLICATION_MENUITEM((MENU8238_MNGCALL_CALFWD_DATACALL,
                              MENU8237_SCR8093_MNGCALL_MENU_FWD, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU8238_MNGCALL_CALLFWD_DATA, NULL));

    ADD_APPLICATION_MENUITEM((MENU8239_CALFWD_VOICECALL_UNREACHABLE,
                              MENU8237_SCR8093_MNGCALL_MENU_FWD, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU8239_CALFWD_VOICECALL_UNREACHABLE, NULL));

    ADD_APPLICATION_MENUITEM((MENU8239_CALFWD_VOICECALL_NOANSWER,
                              MENU8237_SCR8093_MNGCALL_MENU_FWD, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU8239_CALFWD_VOICECALL_NOANSWER, NULL));
#if 0
    ADD_APPLICATION_MENUITEM((MENU8239_CALFWD_VOICECALL_ALLCOND,
                              MENU8237_SCR8093_MNGCALL_MENU_FWD, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU8239_CALFWD_VOICECALL_ALLCOND, NULL));
#endif
    ADD_APPLICATION_MENUITEM((MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS,
                              0, 6,
                              MENU_NOANSWER_TIME_05_SECONDS,
                              MENU_NOANSWER_TIME_10_SECONDS,
                              MENU_NOANSWER_TIME_15_SECONDS,
                              MENU_NOANSWER_TIME_20_SECONDS,
                              MENU_NOANSWER_TIME_25_SECONDS,
                              MENU_NOANSWER_TIME_30_SECONDS,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS, NULL));

    ADD_APPLICATION_MENUITEM((MENU_NOANSWER_TIME_05_SECONDS,
                              MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_05_SECONDS, NULL));
    ADD_APPLICATION_MENUITEM((MENU_NOANSWER_TIME_10_SECONDS,
                              MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_10_SECONDS, NULL));
    ADD_APPLICATION_MENUITEM((MENU_NOANSWER_TIME_15_SECONDS,
                              MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_15_SECONDS, NULL));
    ADD_APPLICATION_MENUITEM((MENU_NOANSWER_TIME_20_SECONDS,
                              MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_20_SECONDS, NULL));
    ADD_APPLICATION_MENUITEM((MENU_NOANSWER_TIME_25_SECONDS,
                              MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_25_SECONDS, NULL));
    ADD_APPLICATION_MENUITEM((MENU_NOANSWER_TIME_30_SECONDS,
                              MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_30_SECONDS, NULL));

    ADD_APPLICATION_MENUITEM((MENU8239_CALFWD_VOICECALL_BUSY,
                              MENU8237_SCR8093_MNGCALL_MENU_FWD, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU8239_CALFWD_VOICECALL_BUSY, NULL));

    ADD_APPLICATION_MENUITEM((MENU8239_CALFWD_VOICECALL_UNCONDITIONALY,
                              MENU8237_SCR8093_MNGCALL_MENU_FWD, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU8239_CALFWD_VOICECALL_UNCONDITIONALY, NULL));

    ADD_APPLICATION_MENUITEM((MENU8240_MNGCALL_CALLFWD_FWD_OPTION,
                              0, 3,
                              MENU8240_MNGCALL_CALLFWD_FWD_ON,
                              MENU8240_MNGCALL_CALLFWD_FWD_OFF,
                              MENU8240_MNGCALL_CALLFWD_FWD_QUERY,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU8240_MNGCALL_CALLFWD_FWD_OPTION, NULL));

    ADD_APPLICATION_MENUITEM((MENU8240_MNGCALL_CALLFWD_FWD_ON,
                              MENU8240_MNGCALL_CALLFWD_FWD_OPTION, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_ACTIVATE, NULL));

    ADD_APPLICATION_MENUITEM((MENU8240_MNGCALL_CALLFWD_FWD_OFF,
                              MENU8240_MNGCALL_CALLFWD_FWD_OPTION, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_DETACTIVATE, NULL));

    ADD_APPLICATION_MENUITEM((MENU8240_MNGCALL_CALLFWD_FWD_QUERY,
                              MENU8240_MNGCALL_CALLFWD_FWD_OPTION, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU8240_MNGCALL_CALLFWD_FWD_QUERY, NULL));

    ADD_APPLICATION_STRING2(STR_MENU8238_MNGCALL_CALLFWD_CAPTION, "Call Divert ", "CALL FWD MAIN SCREEN CAPTION ");

    ADD_APPLICATION_STRING2(
        SCR_CALL_FWD_ENTER_NO_ANSWER_TIME_CAP,
        "No Answer Time",
        "SCR_CALL_FWD_ENTER_NO_ANSWER_TIME_CAP ");

    ADD_APPLICATION_STRING2(
        STR_MENU8238_MNGCALL_CALLFWD_VOICE,
        "Divert All Voice Calls",
        " CALL FORWARD MAIN SCREEN VOICE CALL OPTION");

    ADD_APPLICATION_STRING2(STR_MENU8238_MNGCALL_CALLFWD_FAX, "Fax Calls", " CALL FORWARD MAIN SCREEN FAX CALL OPTION");

    ADD_APPLICATION_STRING2(
        STR_MENU8238_MNGCALL_CALLFWD_DATA,
        "Divert All Data Calls",
        " CALL FORWARD MAIN SCREEN DATA CALL OPTION");

    ADD_APPLICATION_STRING2(
        STR_MENU8239_CALFWD_VOICECALL_OPTION,
        "Voice Calls",
        " CALL FORWARD VOICE CALLS OPTION SCREEN  CAPTION");

    ADD_APPLICATION_STRING2(
        STR_MENU8239_CALFWD_VOICECALL_UNREACHABLE,
        "Divert If Unreachable",
        " CALL FORWARD VOICE CALLS OPTION UNREACHABLE");

    ADD_APPLICATION_STRING2(
        STR_MENU8239_CALFWD_VOICECALL_NOANSWER,
        "Divert If No Answer",
        " CALL FORWARD VOICE CALLS OPTION NO ANSWER ");
#if 0
    ADD_APPLICATION_STRING2(
        STR_MENU8239_CALFWD_VOICECALL_ALLCOND,
        "Divert For All Conditional",
        " CALL FORWARD VOICE CALLS OPTION ALL CONDITIONAL ");
#endif
    ADD_APPLICATION_STRING2(
        STR_MENU8239_CALFWD_VOICECALL_NOANSWER_OPTIONS,
        "Divert If No Answer Options",
        " CALL FORWARD VOICE CALLS OPTION NO ANSWER ");

    ADD_APPLICATION_STRING2(
        STR_MENU8239_CALFWD_VOICECALL_BUSY,
        "Divert If Busy",
        " CALL FORWARD VOICE CALLS OPTION IF BUSY ");

    ADD_APPLICATION_STRING2(
        STR_MENU8239_CALFWD_VOICECALL_UNCONDITIONALY,
        "Unconditional",
        " CALL FORWARD VOICE CALLS OPTION UNCONDITIONAL ");

    ADD_APPLICATION_STRING2(STR_MENU_CALL_FWD_CANCELALL, "Cancel All Divert ", " STR_MENU_CALL_FWD_CANCELALL ");

    ADD_APPLICATION_STRING2(STR_05_SECONDS, " 5 Seconds", "5 Seconds");
    ADD_APPLICATION_STRING2(STR_10_SECONDS, " 10 Seconds", "10 Seconds");
    ADD_APPLICATION_STRING2(STR_15_SECONDS, " 15 Seconds", "15 Seconds");
    ADD_APPLICATION_STRING2(STR_20_SECONDS, " 20 Seconds", "20 Seconds");
    ADD_APPLICATION_STRING2(STR_25_SECONDS, " 25 Seconds", "25 Seconds");
    ADD_APPLICATION_STRING2(STR_30_SECONDS, " 30 Seconds", "30 Seconds");

    ADD_APPLICATION_STRING2(
        STR_SCR8105_MNGCALL_CALLFWD_BUSY_SCREEN_CAPTION,
        "Busy...",
        "CALL FWD BUSY SCREEN CAPTION ");

    ADD_APPLICATION_STRING2(
        STR_SCR8105_MNGCALL_CALLFWD_BUSY_SCREEN_TEXT,
        "Please Wait...",
        "CALL FWD BUSY SCREEN TEXT  ");

    ADD_APPLICATION_STRING2(
        STR_MENU8240_MNGCALL_CALLFWD_FWD_OPTION,
        "Call Divert Options",
        "CALL FRWD OPTIONS CAPTION  ");

    ADD_APPLICATION_STRING2(STR_MENU8240_MNGCALL_CALLFWD_FWD_QUERY, "Query Status", "CALL FRWD QUERY OPTION ");

    ADD_APPLICATION_STRING2(STR_CALL_FWD_ENTER_NUM_CAP, "Divert Number", "STR_CALL_FWD_ENTER_NUM_CAP");

#ifndef __BW_MMI__

    ADD_APPLICATION_IMAGE2(
        IMG_SCR8103_CAPTION,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "CALL FORWARD VOICE CALL SCREEN  CAPTION IMAGE ");

    ADD_APPLICATION_IMAGE2(
        IMG_SCR8105_CAPTION,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "CALL FORWARD OPTIONS RSK IMAGE ");

    ADD_APPLICATION_IMAGE2(
        IMG_SCR8106_CAPTION,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "CALL FORWARD ON SUCCESS CAPTION ");

    ADD_APPLICATION_IMAGE2(IMG_SCR8107_CAPTION, CUST_IMG_BASE_PATH "/NoImage.bmp", "CALL FORWARD ON FAIL CAPTION ");

    ADD_APPLICATION_IMAGE2(
        IMG_SCR8108_CAPTION,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "CALL FORWARD QUERY FAIL CAPTION ");

#endif /* __BW_MMI__ */ 

    ADD_APPLICATION_STRING2(STR_HASH_VALUE, "#", "STR_HASH_VALUE");

    ADD_APPLICATION_STRING2(STR_L4_CFU, "Call Forward Unconditional ", "Call Forward Unconditional.");
    ADD_APPLICATION_STRING2(STR_L4_CFB, "Call Forward When Busy ", "Call Forward When Busy.");
    ADD_APPLICATION_STRING2(STR_L4_CFNRY, "Call Forward When Not Reply ", "Call Forward When Not Reply");
    ADD_APPLICATION_STRING2(STR_L4_CFNRC, "Call Forward When Not Reachable ", "Call Forward When Not Reachable");
    ADD_APPLICATION_STRING2(STR_L4_CFA, "All Call Forward ", "All Call Forward");
    ADD_APPLICATION_STRING2(STR_L4_CFC, "All Conditional Call Forward ", "All Conditional Call Forward");
    ADD_APPLICATION_STRING2(STR_FWD_UNKNOWN_SERVICE, "Unknown Service Type ", "Unknown Service Type");

}


/*****************************************************************************
 * FUNCTION
 *  PopulateCallWait
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateCallWait()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_MENUITEM((MENU8237_SCR8093_MNGCALL_MENU_WAIT,
                              MENU8237_SCR8093_MNGCALL_MENU_MAIN, 3,
                              MENU8237_MNGCALL_CALWAIT_MENU_ON,
                              MENU8237_MNGCALL_CALWAIT_MENU_OFF,
                              MENU8237_MNGCALL_CALWAIT_MENU_QUERY,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SCR8094_MNGCALL_CALWAIT_MENU_CAPTION, NULL));

    ADD_APPLICATION_MENUITEM((MENU8237_MNGCALL_CALWAIT_MENU_ON,
                              MENU8237_SCR8093_MNGCALL_MENU_WAIT, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_ACTIVATE, NULL));

    ADD_APPLICATION_MENUITEM((MENU8237_MNGCALL_CALWAIT_MENU_OFF,
                              MENU8237_SCR8093_MNGCALL_MENU_WAIT, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_DETACTIVATE, NULL));

    ADD_APPLICATION_MENUITEM((MENU8237_MNGCALL_CALWAIT_MENU_QUERY,
                              MENU8237_SCR8093_MNGCALL_MENU_WAIT, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SCR8094_MNGCALL_CALWAIT_MENU_QUERY, NULL));

    ADD_APPLICATION_STRING2(STR_SCR8094_MNGCALL_CALWAIT_MENU_CAPTION, "Call Waiting", "CALL WAITING MENU CAPTION");

    ADD_APPLICATION_STRING2(STR_SCR8094_MNGCALL_CALWAIT_MENU_QUERY, "Query Status", "CALL WAITING QUERY OPTION ");

    ADD_APPLICATION_STRING2(STR_SCR8096_MNGCALL_CALLWAIT_ON, "Call Waiting ", "CALL WAITING ON string");

    ADD_APPLICATION_STRING2(
        STR_SCR8100_MNGCALL_CALLWAIT_ON_STATUS_CAPTION,
        "Success",
        "CALL WAITING ON STATUS SCR CAPTION");

#ifndef __BW_MMI__
    ADD_APPLICATION_IMAGE2(
        IMG_SCR_CALL_SETUP_REQ_CAP,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "IMG_SCR_CALL_SETUP_REQ_CAP");

    ADD_APPLICATION_IMAGE2(
        IMG_SCR_CALL_SETUP_REQ_LSK,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "IMG_SCR_CALL_SETUP_REQ_LSK");

    ADD_APPLICATION_IMAGE2(
        IMG_SCR8095_CAPTION,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "CALL WAITING SCR CAPTION IMAGE ");

    ADD_APPLICATION_IMAGE2(IMG_SCR8095_TEXT, CUST_IMG_BASE_PATH "/NoImage.bmp", "CALL WAITING SCR TEXT IMAGE ");

    ADD_APPLICATION_IMAGE2(
        IMG_SCR8096_CAPTION,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "CALL WAITING SCR ON SUCCESS CAPTION IMAGE ");

    ADD_APPLICATION_IMAGE2(
        IMG_SCR8097_CAPTION,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "CALL WAITING SCR ON FAILURE CAPTION IMAGE");

    ADD_APPLICATION_IMAGE2(
        IMG_SCR8098_CAPTION,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "CALL WAITING SCR OFF SUCCESS CAPTION IMAGE");

    ADD_APPLICATION_IMAGE2(
        IMG_SCR8099_CAPTION,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "CALL WAITING SCR OFF FAILURE CAPTION IMAGE");

    ADD_APPLICATION_IMAGE2(
        IMG_SCR8100_CAPTION,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "CALL WAITING SCR STATUS ON CAPTION IMAGE");

    ADD_APPLICATION_IMAGE2(
        IMG_SCR8101_CAPTION,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "CALL WAITING SCR STATUS OFF CAPTION IMAGE");
#endif /* __BW_MMI__ */ 
    ADD_APPLICATION_STRING2(STR_REQ_ABORT_TEXT, "Abort Req", "STR_REQ_ABORT_TEXT");
}


/*****************************************************************************
 * FUNCTION
 *  PopulateCallerId
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateCallerId(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_MENUITEM((MENU_CALLER_ID,
                              MENU8237_SCR8093_MNGCALL_MENU_MAIN, 3,
                              MENU_CID_SET_NWK,
                              MENU_CID_HIDE_ID,
                              MENU_CID_SHOW_ID, SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_CALLER_ID, NULL));

    ADD_APPLICATION_MENUITEM((MENU_CID_SET_NWK, MENU_CALLER_ID, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_CLI_SET_NETWORK, NULL));

    ADD_APPLICATION_MENUITEM((MENU_CID_HIDE_ID, MENU_CALLER_ID, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_CLI_HIDE_ID, NULL));

    ADD_APPLICATION_MENUITEM((MENU_CID_SHOW_ID, MENU_CALLER_ID, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_CLI_SEND_ID, NULL));

    ADD_APPLICATION_STRING2(STR_MENU_CALLER_ID, "Caller ID", "Menu Item String for Caller Id");

    ADD_APPLICATION_STRING2(
        STR_CLI_SET_NETWORK,
        "Set By Network",
        "String for allowing user to set caller id by network");

    ADD_APPLICATION_STRING2(STR_CLI_HIDE_ID, "Hide Id", "String for allowing user to set CLIR");
    ADD_APPLICATION_IMAGE2(STR_CLI_HIDE_ID, CUST_IMG_BASE_PATH "/NoImage.bmp", "Manage Calls Caller Id Hide");

    ADD_APPLICATION_STRING2(STR_CLI_SEND_ID, "Send Id", "String for allowing user to set CLIP");
    ADD_APPLICATION_IMAGE2(IMG_CALLER_ID_TITLE_ICN, CUST_IMG_BASE_PATH "/NoImage.bmp", "Manage Calls Caller Id ");

    ADD_APPLICATION_STRING2(STR_CLIP_STATUS_TITLE, "CLIP Status", "CLIP Status Title");
    ADD_APPLICATION_IMAGE2(IMG_CLIP_STATUS_TITLE, CUST_IMG_BASE_PATH "/NoImage.bmp", "CLIP Status Title Icon");

    ADD_APPLICATION_STRING2(STR_CLIR_STATUS_TITLE, "CLIR Status", "CLIR Status Title");
    ADD_APPLICATION_IMAGE2(IMG_CLIR_STATUS_TITLE, CUST_IMG_BASE_PATH "/NoImage.bmp", "CLIR Status Title Icon");

    ADD_APPLICATION_STRING2(STR_CLIP_STATUS_ACT_TXT, "Call ID Presentation Provided", "Message Text for CLIP Status");
    ADD_APPLICATION_STRING2(
        STR_CLIP_STATUS_NACT_TXT,
        "Call ID Presentation Not Provided",
        "Message Text for CLIP Status");
    ADD_APPLICATION_STRING2(STR_CLIR_STATUS_ACT_TXT, "Call ID Restriction Provided", "Message Text for CLIR Status");
    ADD_APPLICATION_STRING2(
        STR_CLIR_STATUS_ACT_PERMANENTLY_TXT,
        "Call ID Restriction Activated Permantly",
        "Message Text for CLIR Status");
    ADD_APPLICATION_STRING2(
        STR_CLIR_STATUS_ACT_DEFAULT_RESTRICT_TXT,
        "Call ID Restriction Activated By Default",
        "Message Text for CLIR Status");
    ADD_APPLICATION_STRING2(
        STR_CLIR_STATUS_ACT_DEFAULT_ALLOW_TXT,
        "Call ID Restriction Not Activated By Default",
        "Message Text for CLIR Status");
    ADD_APPLICATION_STRING2(
        STR_CLIR_STATUS_NACT_TXT,
        "Call ID Restriction Not Provided",
        "Message Text for CLIR Status");
    ADD_APPLICATION_STRING2(STR_COLP_STATUS_TITLE, "COLP Status", "COLP Status Title");
    ADD_APPLICATION_STRING2(STR_COLR_STATUS_TITLE, "COLR Status", "COLR Status Title");
    ADD_APPLICATION_STRING2(STR_COLP_STATUS_ACT_TXT, "Connected Line ID Presentation Provided", "COLP Status Title");
    ADD_APPLICATION_STRING2(STR_COLR_STATUS_ACT_TXT, "Connected Line ID Restriction Provided", "COLR Status Title");
    ADD_APPLICATION_STRING2(
        STR_COLP_STATUS_NACT_TXT,
        "Connected Line ID Presentation Not Provided",
        "COLP Status Title");
    ADD_APPLICATION_STRING2(
        STR_COLR_STATUS_NACT_TXT,
        "Connected Line ID Restriction Not Provided",
        "COLR Status Title");

    ADD_APPLICATION_STRING2(STR_CNAP_STATUS_TITLE, "CNAP Status", "CNAP Status Title");
    ADD_APPLICATION_STRING2(STR_CNAP_STATUS_ACT_TXT, "Calling Name Presentation Provided", "CNAP Status Title");
    ADD_APPLICATION_STRING2(STR_CNAP_STATUS_NACT_TXT, "Calling Name Presentation Not Provided", "CNAP Status Title");

    ADD_APPLICATION_STRING2(STR_CCBS_STATUS_TITLE, "CCBS Status", "CCBS Status Title");
    ADD_APPLICATION_STRING2(STR_CCBS_STATUS_ACT_TXT, "CCBS Provided", "CCBS Status Title");
    ADD_APPLICATION_STRING2(STR_CCBS_STATUS_NACT_TXT, "CCBS Not Provided", "CCBS Status Title");

    ADD_APPLICATION_STRING2(STR_CLIP_SETUP_REQ_LSK_ERR, "Invalid Command", "CLIR/CLIP Status Error Message");
    ADD_APPLICATION_IMAGE2(IMG_CLIP_SETUP_REQ_LSK_ERR, CUST_IMG_BASE_PATH "/NoImage.bmp", "CLIR Status Title Icon");
}


/*****************************************************************************
 * FUNCTION
 *  PopulateLanguageStrings
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateLanguageStrings(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_STRING2(STR_MENU_LANGUAGE, "Language", "Language Menu String");
    ADD_APPLICATION_STRING2(STR_LANGUAGESCR_AUTO, "Auto Language", "English selected");
    ADD_APPLICATION_STRING2(STR_LANGUAGESCR_AUTO_SEL, "Auto Language selected", "English selected");
    ADD_APPLICATION_STRING2(STR_LANGUAGESCR_ENG, "English selected", "English selected");
    ADD_APPLICATION_STRING2(STR_LANGUAGESCR_TRCH, "Traditional Chinese selected", "Traditional Chinese selected");
    ADD_APPLICATION_STRING2(STR_LANGUAGESCR_SICH, "Simplified Chinese selected", "Simplified Chinese selected");
#if defined(__MMI_PREFER_INPUT_METHOD__)        /* { */
    ADD_APPLICATION_STRING2(STR_MENU_INPUT_METHOD, "Prefered Input Methods", "menu string for phone setup");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_U_ABC, "Multitap ABC", "Input methods list Option 1");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_L_ABC, "Multitap abc", "Input methods list Option 2");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_ABC, "Smart ABC", "Input methods list Option 3");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_ABC, "Smart abc", "Input methods list Option 4");

#if defined(__MMI_MULTITAP_HINDI__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_HINDI, "Multitap Hindi", "Input methods Option Hindi");
#endif 


#if defined(__MMI_MULTITAP_MARATHI__)  
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_MARATHI, "Multitap Marathi", "Input methods Option Marathi");
#endif 

#if defined(__MMI_MULTITAP_HEBREW__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_HEBREW, "Multitap Hebrew", "Input methods Option HEBREW");
#endif 

#if defined (__MMI_MULTITAP_THAI__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_THAI, "Multitap Thai", "Input methods Option Thai");
#endif 

#if defined(__MMI_MULTITAP_DANISH__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_U_DANISH, "Multitap DA", "Input methods list Option Multitap DANISH");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_L_DANISH, "Multitap da", "Input methods list Option Multitap danish");
#endif /* defined(__MMI_MULTITAP_DANISH__) */ 

#if defined(__MMI_MULTITAP_POLISH__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_U_POLISH, "Multitap PO","Input methods list Option Multitap POLISH");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_L_POLISH, "Multitap po", "Input methods list Option Multitap polish");
#endif /* defined(__MMI_MULTITAP_POLISH__) */ 


#if defined(__MMI_T9_DANISH__) || defined(__MMI_ZI_DANISH__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_DANISH, "Smart DA", "Input methods list Option Smart DANISH");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_DANISH, "Smart da", "Input methods list Option Smart danish");
#endif /* defined(__MMI_T9_DANISH__) || defined(__MMI_ZI_DANISH__) */ 

#if defined(__MMI_T9_POLISH__) || defined(__MMI_ZI_POLISH__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_POLISH, "Smart PO", "Input methods list Option Smart POLISH");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_POLISH, "Smart po", "Input methods list Option Smart polish");
#endif /* defined(__MMI_T9_POLISH__) || defined(__MMI_ZI_POLISH__) */ 

#if defined(__MMI_MULTITAP_FRENCH__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_U_FRENCH, "Multitap FR", "Input methods list Option Multitap FRENCH");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_L_FRENCH, "Multitap fr", "Input methods list Option Multitap french");
#endif /* defined(__MMI_MULTITAP_FRENCH__) */ 

#if defined(__MMI_MULTITAP_MYANMAR__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_MYANMAR, "Multitap My", "Input methods list Option Multitap Myanmar");
#endif

#if defined(__MMI_T9_FRENCH__) || defined(__MMI_ZI_CA_FRENCH__) || defined(__MMI_ZI_EU_FRENCH__) || defined(__MMI_SMART_FRENCH__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_FRENCH, "Smart FR", "Input methods list Option Smart FRENCH");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_FRENCH, "Smart fr", "Input methods list Option Smart french");
#endif /* defined(__MMI_T9_FRENCH__) || defined(__MMI_ZI_CA_FRENCH__) || defined(__MMI_ZI_EU_FRENCH__) */ 

#if defined(__MMI_SMART_URDU__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_URDU, "Smart Urdu", "Input methods list Option Smart Urdu");
#endif

#if defined(__MMI_SMART_PUNJABI__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_PUNJABI, "Smart Punjabi", "Input methods list Option Smart Punjabi");
#endif

#if defined(__MMI_SMART_TELUGU__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_TELUGU, "Smart Telugu", "Input methos list Option Smart Telugu");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_TELUGU, "Smart Telugu", "Smart telugu");
#endif

#if defined(__MMI_SMART_BENGALI__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_BENGALI, "Smart Bengali", "Input Methos list Option Smart Bengaili");
#endif

#if defined(__MMI_SMART_TAMIL__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_TAMIL, "Smart Tamil", "Input methods list Option Smart Tamil");
#endif

#if defined(__MMI_T9_GERMAN__)||defined(__MMI_ZI_GERMAN__)||defined(__MMI_SMART_GERMAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_U_GERMAN, "Multitap DE", "Input methods list Option Multitap GERMAN");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_L_GERMAN, "Multitap de", "Input methods list Option Multitap german");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_GERMAN,"Smart DE","Input methods list Option Smart DE");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_GERMAN,"Smart de","Input methods list Option Smart de");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_GERMAN, "Smart DE", "Input methods list Option Smart GERMAN");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_GERMAN, "Smart de", "Input methods list Option Smart german");
#endif /* defined(__MMI_T9_GERMAN__) || defined(__MMI_ZI_GERMAN__) */ 

#if defined(__MMI_T9_SPANISH__)||defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)||defined(__MMI_SMART_SPANISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_SPANISH,"Smart ES","Input methods list Option Smart ES");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_SPANISH,"Smart es","Input methods list Option Smart es");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_SPANISH, "Smart ES", "Input methods list Option Smart SPANISH");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_SPANISH, "Smart es", "Input methods list Option Smart spanish");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_U_SPANISH, "Multitap ES", "Input methods list Option Multitap SPANISH");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_L_SPANISH, "Multitap es","Input methods list Option Multitap spanish");
#endif /* defined(__MMI_T9_SPANISH__) || defined(__MMI_ZI_EU_SPANISH__) || defined(__MMI_ZI_SA_SPANISH__) */ 

#if defined(__MMI_T9_ITALIAN__)||defined(__MMI_ZI_ITALIAN__)||defined(__MMI_SMART_ITALIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_ITALIAN,"Multitap IT","Input methods list Option Multitap IT");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_ITALIAN,"Multitap it","Input methods list Option Multitap it");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_ITALIAN,"Smart IT","Input methods list Option Smart IT");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_ITALIAN,"Smart it","Input methods list Option Smart it");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_ITALIAN, "Smart IT", "Input methods list Option Smart ITALIAN");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_ITALIAN, "Smart it", "Input methods list Option Smart italian");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_U_ITALIAN,"Multitap IT","Input methods list Option Multitap ITALIAN");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_L_ITALIAN,"Multitap it","Input methods list Option Multitap italian");
#endif /* defined(__MMI_T9_ITALIAN__) || defined(__MMI_ZI_ITALIAN__) */ 

#if defined(__MMI_MULTITAP_RUSSIAN__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_U_RUSSIAN, "Multitap RU", "Input methods list Option Multitap RUSSIAN");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_L_RUSSIAN, "Multitap ru", "Input methods list Option Multitap russian");
#endif /* defined(__MMI_MULTITAP_RUSSIAN__) */ 

#if defined(__MMI_MULTITAP_BULGARIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_BULGARIAN,
        "Multitap BG",
        "Input methods list Option Multitap BULGARIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_BULGARIAN,
        "Multitap bg",
        "Input methods list Option Multitap bulgarian");
#endif /* defined(__MMI_MULTITAP_BULGARIAN__) */ 

#if defined(__MMI_MULTITAP_ARABIC__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_ARABIC,
        "Multitap Arabic",
        "Input methods list Option Multitap AR");
#endif /* defined(__MMI_MULTITAP_ARABIC__) */ 
//PMT START PERSIAN
#if defined(__MMI_MULTITAP_PERSIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_PERSIAN,
        "Multitap Persian",
        "Input methods list Option Multitap FA");
#endif 
//PMT END PERSIAN
#if defined ( __MMI_T9_RUSSIAN__) || defined(__MMI_ZI_RUSSIAN__) || defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__) || defined(__MMI_SMART_RUSSIAN__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_RUSSIAN, "Smart RU", "Input methods list Option Smart RUSSIAN");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_RUSSIAN, "Smart ru", "Input methods list Option Smart russian");
#endif /* defined ( __MMI_T9_RUSSIAN__) || defined(__MMI_ZI_RUSSIAN__) || defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__) */ 

#if defined ( __MMI_T9_BULGARIAN__) || defined(__MMI_ZI_BULGARIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_U_BULGARIAN,
        "Smart BG",
        "Input methods list Option Smart BULGARIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_L_BULGARIAN,
        "Smart bg",
        "Input methods list Option Smart bulgarian");
#endif /* defined ( __MMI_T9_BULGARIAN__) || defined(__MMI_ZI_BULGARIAN__) */ 

#if defined(__MMI_ZI_THAI__) || defined(__MMI_T9_THAI__) || defined(__MMI_SMART_THAI__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_THAI, "Smart Thai", "Input methods list Option Smart Thai");
#endif 

#if defined(__MMI_ZI_HEBREW__) || defined(__MMI_T9_HEBREW__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_HEBREW, "Smart Hebrew", "Input methods list Option Smart Hebrew");
#endif 

    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_ARABIC, "Smart Arabic", "Input methods list Option Smart Arabic");
//PMT START PERSIAN
#ifdef __MMI_SMART_PERSIAN__
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_PERSIAN, "Smart Persian", "Input methods list Option Smart Persian");
#endif
//PMT END PERSIAN

#if defined(__MMI_ZI_HINDI__) || defined(__MMI_T9_HINDI__)||defined(__MMI_SMART_HINDI__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_HINDI, "Smart Hindi", "Input methods list Option Smart Hindi");
#endif 

#if defined(__MMI_MULTITAP_TURKISH__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_TURKISH,
        "Multitap TR",
        "Input methods list Option Multitap TURKISH");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_TURKISH,
        "Multitap tr",
        "Input methods list Option Multitap turkish");
#endif /* defined(__MMI_MULTITAP_TURKISH__) */ 

#if defined(__MMI_ZI_TURKISH__) || defined(__MMI_T9_TURKISH__)||defined(__MMI_SMART_TURKISH__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_TURKISH, "Smart TR", "Input methods list Option Smart TURKISH");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_TURKISH, "Smart tr", "Input methods list Option Smart turkish");
#endif /* defined(__MMI_ZI_TURKISH__) || defined(__MMI_T9_TURKISH__) */ 

#if defined(__MMI_MULTITAP_PORTUGUESE__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_PORTUGUESE,
        "Multitap PT",
        "Input methods list Option Multitap PORTUGUESE");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_PORTUGUESE,
        "Multitap pt",
        "Input methods list Option Multitap portuguese");
#endif /* defined(__MMI_MULTITAP_PORTUGUESE__) */ 

#if defined(__MMI_GB_SMART_PORTUGUESE__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_U_PORTUGUESE,
        "Smart PT",
        "Input methods list Option Smart PORTUGUESE");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_L_PORTUGUESE,
        "Smart pt",
        "Input methods list Option Smart portuguese");
#endif /* defined(__MMI_T9_PORTUGUESE__) || defined(__MMI_ZI_EU_PORTUGUESE__) || defined(__MMI_ZI_SA_PORTUGUESE__) || defined(__MMI_ZI_BZ_PORTUGUESE__) */ 

#if defined(__MMI_MULTITAP_INDONESIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_INDONESIAN,
        "Multitap ID",
        "Input methods list Option Multitap INDONESIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_INDONESIAN,
        "Multitap id",
        "Input methods list Option Multitap indonesian");
#endif /* defined(__MMI_MULTITAP_INDONESIAN__) */ 

#if defined(__MMI_ZI_INDONESIAN__) || defined(__MMI_T9_INDONESIAN__)||defined(__MMI_SMART_INDONESIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_U_INDONESIAN,
        "Smart ID",
        "Input methods list Option Smart INDONESIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_L_INDONESIAN,
        "Smart id",
        "Input methods list Option Smart indonesian");
#endif /* defined(__MMI_ZI_INDONESIAN__) || defined(__MMI_T9_INDONESIAN__) */ 

#if defined(__MMI_MULTITAP_CZECH__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_CZECH,
        "Multitap CZECH",
        "Input methods list Option Multitap CZECH");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_CZECH,
        "Multitap czech",
        "Input methods list Option Multitap czech");
#endif /* defined(__MMI_MULTITAP_CZECH__) */ 

#if defined(__MMI_ZI_CZECH__) || defined(__MMI_T9_CZECH__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_CZECH, "Smart CZECH", "Input methods list Option Smart CZECH");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_CZECH, "Smart czech", "Input methods list Option Smart czech");
#endif /* defined(__MMI_ZI_CZECH__) || defined(__MMI_T9_CZECH__) */ 

#if defined(__MMI_MULTITAP_MALAY__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_MALAY,
        "Multitap MS",
        "Input methods list Option Multitap MALAY");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_MALAY,
        "Multitap ms",
        "Input methods list Option Multitap malay");
#endif /* defined(__MMI_MULTITAP_MALAY__) */ 
    /* PMT START LANGUAGE 20060104 */
#if defined(__MMI_ZI_MALAY__) || defined(__MMI_T9_MALAY__) || defined(__MMI_SMART_MALAY__)
    /* PMT END LANGUAGE 20060104 */
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_MALAY, "Smart MS", "Input methods list Option Smart MALAY");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_MALAY, "Smart ms", "Input methods list Option Smart malay");
#endif /* defined(__MMI_ZI_MALAY__) || defined(__MMI_T9_MALAY__) */ 

#if defined(__MMI_MULTITAP_VIETNAMESE__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_U_VIETNAMESE, "Multitap VI", "Input methods list Option Multitap VIETNAMESE");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_L_VIETNAMESE, "Multitap vi", "Input methods list Option Multitap vietnamese");
#endif /* defined(__MMI_MULTITAP_VIETNAMESE__) */ 

#if defined(__MMI_ZI_VIETNAMESE__) || defined(__MMI_T9_VIETNAMESE__) || defined(__MMI_SMART_VIETNAMESE__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_VIETNAMESE,"Smart VI","Input methods list Option Smart VIETNAMESE");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_VIETNAMESE,"Smart vi","Input methods list Option Smart vietnamese");
#endif /* defined(__MMI_ZI_VIETNAMESE__) || defined(__MMI_T9_VIETNAMESE__) */ 

#if defined(__MMI_MULTITAP_FINNISH__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_FINNISH,
        "Multitap FI",
        "Input methods list Option Multitap FINNISH");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_FINNISH,
        "Multitap fi",
        "Input methods list Option Multitap finnish");
#endif /* defined(__MMI_MULTITAP_FINNISH__) */ 

#if defined(__MMI_T9_FINNISH__) || defined(__MMI_ZI_FINNISH__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_FINNISH, "Smart FI", "Input methods list Option Smart FINNISH");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_FINNISH, "Smart fi", "Input methods list Option Smart finnish");
#endif /* defined(__MMI_T9_FINNISH__) || defined(__MMI_ZI_FINNISH__) */ 

#if defined(__MMI_MULTITAP_HUNGARIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_HUNGARIAN,
        "Multitap HUNGARIAN",
        "Input methods list Option Multitap HUNGARIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_HUNGARIAN,
        "Multitap hungarian",
        "Input methods list Option Multitap hungarian");
#endif /* defined(__MMI_MULTITAP_HUNGARIAN__) */ 

#if defined(__MMI_ZI_HUNGARIAN__) || defined(__MMI_T9_HUNGARIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_U_HUNGARIAN,
        "Smart HUNGARIAN",
        "Input methods list Option Smart HUNGARIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_L_HUNGARIAN,
        "Smart hungarian",
        "Input methods list Option Smart hungarian");
#endif /* defined(__MMI_ZI_HUNGARIAN__) || defined(__MMI_T9_HUNGARIAN__) */ 

#if defined(__MMI_MULTITAP_NORWEGIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_NORWEGIAN,
        "Multitap NO",
        "Input methods list Option Multitap NORWEGIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_NORWEGIAN,
        "Multitap no",
        "Input methods list Option Multitap norwegian");
#endif /* defined(__MMI_MULTITAP_NORWEGIAN__) */ 

#if defined(__MMI_T9_NORWEGIAN__) || defined(__MMI_ZI_NORWEGIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_U_NORWEGIAN,
        "Smart NO",
        "Input methods list Option Smart NORWEGIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_L_NORWEGIAN,
        "Smart no",
        "Input methods list Option Smart norwegian");
#endif /* defined(__MMI_T9_NORWEGIAN__) || defined(__MMI_ZI_NORWEGIAN__) */ 

#if defined(__MMI_MULTITAP_SLOVAK__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_SLOVAK,
        "Multitap SK",
        "Input methods list Option Multitap SLOVAK");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_SLOVAK,
        "Multitap sk",
        "Input methods list Option Multitap slovak");
#endif /* defined(__MMI_MULTITAP_SLOVAK__) */ 

#if defined(__MMI_T9_SLOVAK__) || defined(__MMI_ZI_SLOVAK__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_SLOVAK, "Smart SK", "Input methods list Option Smart SLOVAK");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_SLOVAK, "Smart sk", "Input methods list Option Smart slovak");
#endif /* defined(__MMI_T9_SLOVAK__) || defined(__MMI_ZI_SLOVAK__) */ 

#if defined(__MMI_MULTITAP_DUTCH__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_DUTCH,
        "Multitap NL",
        "Input methods list Option Multitap DUTCH");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_DUTCH,
        "Multitap nl",
        "Input methods list Option Multitap dutch");
#endif /* defined(__MMI_MULTITAP_DUTCH__) */ 

#if defined(__MMI_ZI_DUTCH__) || defined(__MMI_T9_DUTCH__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_DUTCH, "Smart NL", "Input methods list Option Smart DUTCH");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_DUTCH, "Smart nl", "Input methods list Option Smart dutch");
#endif /* defined(__MMI_ZI_DUTCH__) || defined(__MMI_T9_DUTCH__) */ 

#if defined(__MMI_MULTITAP_SWEDISH__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_SWEDISH,
        "Multitap SV",
        "Input methods list Option Multitap SWEDISH");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_SWEDISH,
        "Multitap sv",
        "Input methods list Option Multitap swedish");
#endif /* defined(__MMI_MULTITAP_SWEDISH__) */ 

#if defined(__MMI_T9_SWEDISH__) || defined(__MMI_ZI_SWEDISH__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_SWEDISH, "Smart SV", "Input methods list Option Smart SWEDISH");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_SWEDISH, "Smart sv", "Input methods list Option Smart swedish");
#endif /* defined(__MMI_T9_SWEDISH__) || defined(__MMI_ZI_SWEDISH__) */ 

#if defined(__MMI_MULTITAP_CROATIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_CROATIAN,
        "Multitap HR",
        "Input methods list Option Multitap CROATIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_CROATIAN,
        "Multitap hr",
        "Input methods list Option Multitap croatian");
#endif /* defined(__MMI_MULTITAP_CROATIAN__) */ 

#if defined(__MMI_T9_CROATIAN__) || defined(__MMI_ZI_CROATIAN__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_CROATIAN, "Smart HR", "Input methods list Option Smart CROATIAN");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_CROATIAN, "Smart hr", "Input methods list Option Smart croatian");
#endif /* defined(__MMI_T9_CROATIAN__) || defined(__MMI_ZI_CROATIAN__) */ 

#if defined(__MMI_MULTITAP_ROMANIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_ROMANIAN,
        "Multitap RO",
        "Input methods list Option Multitap ROMANIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_ROMANIAN,
        "Multitap ro",
        "Input methods list Option Multitap romanian");
#endif /* defined(__MMI_MULTITAP_ROMANIAN__) */ 

#if defined(__MMI_ZI_ROMANIAN__) || defined(__MMI_T9_ROMANIAN__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_ROMANIAN, "Smart RO", "Input methods list Option Smart ROMANIAN");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_ROMANIAN, "Smart ro", "Input methods list Option Smart romanian");
#endif /* defined(__MMI_ZI_ROMANIAN__) || defined(__MMI_T9_ROMANIAN__) */ 

#if defined(__MMI_MULTITAP_SLOVENIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_SLOVENIAN,
        "Multitap SL",
        "Input methods list Option Multitap SLOVENIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_SLOVENIAN,
        "Multitap sk",
        "Input methods list Option Multitap slovenian");
#endif /* defined(__MMI_MULTITAP_SLOVENIAN__) */ 

#if defined(__MMI_T9_SLOVENIAN__) || defined(__MMI_ZI_SLOVENIAN__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_U_SLOVENIAN,
        "Smart SL",
        "Input methods list Option Smart SLOVENIAN");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_SMART_L_SLOVENIAN,
        "Smart sl",
        "Input methods list Option Smart slovenian");
#endif /* defined(__MMI_T9_SLOVENIAN__) || defined(__MMI_ZI_SLOVENIAN__) */ 

#if defined(__MMI_MULTITAP_GREEK__)
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_U_GREEK,
        "Multitap EL",
        "Input methods list Option Multitap GREEK");
    ADD_APPLICATION_STRING2(
        STR_INPUT_METHOD_MULTITAP_L_GREEK,
        "Multitap el",
        "Input methods list Option Multitap greek");
#endif /* defined(__MMI_MULTITAP_GREEK__) */ 

#if defined(__MMI_T9_GREEK__) || defined(__MMI_ZI_GREEK__)
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_U_GREEK, "Smart EL", "Input methods list Option Smart GREEK");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SMART_L_GREEK, "Smart el", "Input methods list Option Smart greek");
#endif /* defined(__MMI_T9_GREEK__) || defined(__MMI_ZI_GREEK__) */ 

    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_NUMERIC, "Numeric", "Input methods list Option 5");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_BPMF, "Multitap BoPoMo", "Input methods list Option 6");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_BPMF, "Smart BoPoMo", "Input methods list Option 7");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_TR_STROKE, "Traditional Chinese Stroke", "Input methods list Option 8");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_PINYIN, "Multitap Pinyin", "Input methods list Option 9");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_PINYIN, "Smart PinYin", "Input methods list Option 10");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_SM_STROKE, "Simplified Chinese Stroke", "Input methods list Option 11");
#endif /* defined(__MMI_PREFER_INPUT_METHOD__) */ /* } */

    ADD_APPLICATION_IMAGE2(IMG_MENU_LANGUAGE, CUST_IMG_BASE_PATH "/NoImage.bmp", "Language Menu String");
}


/*****************************************************************************
 * FUNCTION
 *  PopulateFontSizeResData
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
int PopulateFontSizeResData(void)
{
#ifdef __MMI_CHANGABLE_FONT__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_DIALLING_FONT__
    ADD_APPLICATION_MENUITEM((MENU_ID_PHNSET_FONT_SIZE, MENU9102_INITIAL_SETUP, 3,
                              MENU_ID_PHNSET_FONT_SIZE_THEME_DEFAULT,
                              MENU_ID_PHNSET_FONT_SIZE_SMALL,
                              MENU_ID_PHNSET_FONT_SIZE_MEDIUM,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_PHNSET_FONT_SIZE, 0));
#else /* __MMI_DIALLING_FONT__ */ 
    ADD_APPLICATION_MENUITEM((MENU_ID_PHNSET_FONT_SIZE, MENU9102_INITIAL_SETUP, 4,
                              MENU_ID_PHNSET_FONT_SIZE_THEME_DEFAULT,
                              MENU_ID_PHNSET_FONT_SIZE_SMALL,
                              MENU_ID_PHNSET_FONT_SIZE_MEDIUM,
                              MENU_ID_PHNSET_FONT_SIZE_LARGE,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_PHNSET_FONT_SIZE, 0));
#endif /* __MMI_DIALLING_FONT__ */ 

    ADD_APPLICATION_STRING2(STR_ID_PHNSET_FONT_SIZE, "Font Size", "Font Size-option");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_FONT_SIZE_THEME_DEFAULT, "Theme Default", "Theme Default - option");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_FONT_SIZE_SMALL, "Small", "Small - option");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_FONT_SIZE_MEDIUM, "Medium", "Medium - option");

#ifndef __MMI_DIALLING_FONT__
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_FONT_SIZE_LARGE, "Large", "Large - option");
#endif 
#endif /* __MMI_CHANGABLE_FONT__ */ 
}   /* end of PopulateFontSizeResData */


/*****************************************************************************
 * FUNCTION
 *  PopulateGPIOSettingResData
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
int PopulateGPIOSettingResData(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_COVER_CLOSE_SETTING__
    ADD_APPLICATION_MENUITEM((MENU_ID_PHNSET_GPIO_SETTING, MENU9102_INITIAL_SETUP,
                              MMI_GPIO_SET_MENU_MAX, 
                          #ifdef __MMI_STATUS_LED__
                              MENU_ID_PHNSET_GPIO_LED,
                          #endif
                              MENU_ID_PHNSET_GPIO_BACKLITE,
                              MENU_ID_PHNSET_GPIO_COVER_CLOSE,
                              SHOW, (MOVEABLEACROSSPARENT | SHORTCUTABLE), DISP_LIST, STR_ID_PHNSET_GPIO_SETTING, 0));
#else /* __MMI_COVER_CLOSE_SETTING__ */ 
#if 0
    ADD_APPLICATION_MENUITEM((MENU_ID_PHNSET_GPIO_SETTING, MENU9102_INITIAL_SETUP,
                              MMI_GPIO_SET_MENU_MAX, 
#ifdef __MMI_STATUS_LED__
                              MENU_ID_PHNSET_GPIO_LED,
#endif
                              MENU_ID_PHNSET_GPIO_BACKLITE,
                              SHOW, (MOVEABLEACROSSPARENT | SHORTCUTABLE), DISP_LIST, STR_ID_PHNSET_GPIO_SETTING, 0));
#endif
#endif /* __MMI_COVER_CLOSE_SETTING__ */ 

#ifdef __MMI_STATUS_LED__
#if 0
    ADD_APPLICATION_MENUITEM((MENU_ID_PHNSET_GPIO_LED, MENU_ID_PHNSET_GPIO_SETTING, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_ID_PHNSET_GPIO_LED, 0));
#endif
#endif 
#if 0
    ADD_APPLICATION_MENUITEM((MENU_ID_PHNSET_GPIO_BACKLITE, MENU_ID_PHNSET_GPIO_SETTING, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_ID_PHNSET_GPIO_BACKLITE, 0));
    ADD_APPLICATION_MENUITEM((MENU_ID_PHNSET_GPIO_COVER_CLOSE, MENU_ID_PHNSET_GPIO_SETTING, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_ID_PHNSET_GPIO_COVER_CLOSE, 0));

   

#ifdef __MMI_STATUS_LED__
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_GPIO_LED, "Signal LED", "Title");
#endif
#endif
    ADD_APPLICATION_STRING2(STR_LIGHT_SETTIMER_CAPTION, "Misc Setting", "Title");
    ADD_APPLICATION_STRING2(STR_LIGHT_LCD_BACKLIGHT, "BackLight Setting", "Title");

//    ADD_APPLICATION_STRING2(STR_ID_PHNSET_GPIO_BACKLITE_LEVEL, "Brightness", "Brightness");
//    ADD_APPLICATION_STRING2(STR_ID_PHNSET_GPIO_BACKLITE_HF_TIME, "Time", "Time");
//    ADD_APPLICATION_STRING2(STR_ID_PHNSET_GPIO_COVER_CLOSE, "Cover Close Setting", "Title");
//    ADD_APPLICATION_STRING2(STR_ID_PHNSET_GPIO_COVER_CLOSE_BACK_TO_IDLE, "Back To Idle Screen", "Title");
//    ADD_APPLICATION_STRING2(STR_ID_PHNSET_GPIO_COVER_CLOSE_REJECT_CALL, "Reject Call", "Title");
//    ADD_APPLICATION_STRING2(STR_ID_PHNSET_GPIO_COVER_CLOSE_KEYPAD_LOCK, "Lock Keypad", "Title");
//    ADD_APPLICATION_STRING2(STR_ID_PHNSET_GPIO_SUCCESS, "Success", "Title");

}   /* end of PopulateGPIOSettingResData */


/*****************************************************************************
 * FUNCTION
 *  PopulateFlightModeResData
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __FLIGHT_MODE_SUPPORT__

void PopulateFlightModeResData(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_STRING2(STR_MENU_FLIGHT_MODE, "Flight Mode", "Flight Mode Menu String");
    ADD_APPLICATION_STRING2(STR_FLIGHT_MODE_NORMAL, "Normal Mode", "Normal Mode");
    ADD_APPLICATION_STRING2(STR_FLIGHT_MODE_SILENT, "Flight Mode", "Flight Mode");
    ADD_APPLICATION_STRING2(STR_FLIGHT_MODE_QUERY, "Query When Power On", "Query");
    ADD_APPLICATION_STRING2(STR_FLIGHT_MODE_CONFIRM, "Are you sure to switch mode?", "Confirmation");
    ADD_APPLICATION_STRING2(STR_FLIGHT_MODE_RSK, "Mode", "Mode");
    ADD_APPLICATION_STRING2(STR_FLIGHT_MODE_PROGRESS, "In Progress", "Progress");

    ADD_APPLICATION_IMAGE2(IMG_MENU_FLIGHT_MODE, CUST_IMG_BASE_PATH "/NoImage.bmp", "Flight Mode Menu Icon");
    ADD_APPLICATION_IMAGE2(
        IMG_FLIGHT_MODE_PROGRESS,
        CUST_IMG_PATH "/MainLCD/UIElement/Progress/PROGRESS.GIF",
        "Progress Image");
}

#endif /* __FLIGHT_MODE_SUPPORT__ */ 


/*****************************************************************************
 * FUNCTION
 *  PopulateLineSwitch
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateLineSwitch(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_MENUITEM((MENU_LINE_SWCH,
                              MENU8237_SCR8093_MNGCALL_MENU_MAIN, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_LINE_SWITCH, NULL));

    ADD_APPLICATION_MENUITEM((MENU_LSW_LN1, MENU_LINE_SWCH, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_LSW_LINE1, IMG_LSW_LINE1));

    ADD_APPLICATION_MENUITEM((MENU_LSW_LN2, MENU_LINE_SWCH, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_LSW_LINE2, IMG_LSW_LINE2));

    ADD_APPLICATION_STRING2(STR_LSW_TITLE, "Line Switching", "Title of Screen for Line Switching");
    ADD_APPLICATION_IMAGE2(
        IMG_LSW_TITLE,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "Image Title for Screen of line switching");

    ADD_APPLICATION_STRING2(STR_LINE_SWITCH, "Line Switching", "String Menu Item Line Switching");

    ADD_APPLICATION_STRING2(STR_LSW_LINE1, "Line 1", "Line 1 option");
    ADD_APPLICATION_IMAGE2(IMG_LSW_LINE1, CUST_IMG_BASE_PATH "/NoImage.bmp", "Image for Line 1 option");

    ADD_APPLICATION_STRING2(STR_LSW_LINE2, "Line 2", "Line 2 option");
    ADD_APPLICATION_IMAGE2(IMG_LSW_LINE1, CUST_IMG_BASE_PATH "/NoImage.bmp", "Image for Line 2 option");

    ADD_APPLICATION_STRING2(STR_LINE1_SET, "Line 1 Selected", "Line 1 Success Message ");
    ADD_APPLICATION_STRING2(STR_LINE1_SET_ERR, "Unknown Error", "Line 1 Failure Message ");

    ADD_APPLICATION_STRING2(STR_LINE2_SET, "Line 2 Selected", "Line 2 Success Message ");
    ADD_APPLICATION_STRING2(STR_LINE2_SET_ERR, "Unknown Error", "Line 2 Failure Message ");

    ADD_APPLICATION_STRING2(STR_SETTING_LINE_SEL, "Sel", "Line Select Message ");

}


/*****************************************************************************
 * FUNCTION
 *  populateManageCalls
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void populateManageCalls()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	ADD_APPLICATION_MENUITEM((MENU8237_SCR8093_MNGCALL_MENU_MAIN,MAIN_MENU_SETTINGS_MENUID,
						#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
								6 + MMI_SIM_NUMBER
						#else  /* No Defined __MMI_MULTI_SIM__ */
								10
						#endif /* __MMI_MULTI_SIM__ */
						#ifdef MAGIC_AE_SUPPORT
								+1
						#endif
								,
						#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
								MULTI_MENU(MENU_MTPNP_SIM1_CALL_SETTING),
						#else  /* No Defined __MMI_MULTI_SIM__ */
								MENU_CALLER_ID,
								MENU8237_SCR8093_MNGCALL_MENU_WAIT,
								MENU8237_SCR8093_MNGCALL_MENU_FWD,
								MENU8237_SCR8093_MNGCALL_MENU_BARRING,
								//MENU_LINE_SWCH,
						#endif /* __MMI_MULTI_SIM__ */
								MENU_SETTING_CALL_REMINDER,
								MENU_SETTING_CALL_TIME_DISPLAY,
								MENU9140_AUTO_REDIAL,
								MENU_SETTING_BLACK_LIST,
								MENU_SETTING_IP_NUMBER,
								MENU_CALL_SETTING_SOUND_WARNING,
						#ifdef MAGIC_AE_SUPPORT
								MENU_CALL_MAGIC_SOUND,
						#endif
								SHOW,MOVEABLEWITHINPARENT,DISP_LIST,STR_SCR8093_MNGCALL_MENU_CAPTION,IMG_SETTING_CALL_SETUP));
#ifdef MAGIC_AE_SUPPORT
ADD_APPLICATION_MENUITEM((MENU_CALL_MAGIC_SOUND, MENU8237_SCR8093_MNGCALL_MENU_MAIN, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_SETTING_CALL_MAGIC_SOUND, NULL));
ADD_APPLICATION_STRING2(STR_SETTING_CALL_MAGIC_SOUND, "Magic sound", "Magic sound");
ADD_APPLICATION_STRING2(STR_MAGIC_SOUND_OFF, "Off", "Magic sound off");
ADD_APPLICATION_STRING2(STR_MAGIC_SOUND_MAN_TO_WOMAN, "Man to woman", "Magic sound Man to woman");
ADD_APPLICATION_STRING2(STR_MAGIC_SOUND_WOMAN_TO_MAN, "Woman to man", "Magic sound Woman to man");
#endif

    ADD_APPLICATION_MENUITEM((MENU_SETTING_CALL_CLOSED_GP, MENU8237_SCR8093_MNGCALL_MENU_MAIN, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_SETTING_CALL_CLOSED_GP, NULL));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_CALL_TIME_DISPLAY, MENU8237_SCR8093_MNGCALL_MENU_MAIN, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_SETTING_CALL_TIME_DISPLAY, NULL));

    ADD_APPLICATION_MENUITEM((MENU9140_AUTO_REDIAL, MENU8237_SCR8093_MNGCALL_MENU_MAIN, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU9140_AUTO_REDIAL, NULL));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_CALL_REMINDER, MENU8237_SCR8093_MNGCALL_MENU_MAIN, 3,
                              MENU_SETTING_CR_OFF,
                              MENU_SETTING_CR_SING,
                              MENU_SETTING_CR_PERIOD, SHOW, NONMOVEABLE, DISP_LIST, STR_SETTING_CALL_REMINDER, NULL));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_CR_OFF, MENU8237_SCR8093_MNGCALL_MENU_MAIN, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_OFF, NULL));
    ADD_APPLICATION_MENUITEM((MENU_SETTING_CR_SING, MENU8237_SCR8093_MNGCALL_MENU_MAIN, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SETTING_CR_SING, NULL));
    ADD_APPLICATION_MENUITEM((MENU_SETTING_CR_PERIOD, MENU8237_SCR8093_MNGCALL_MENU_MAIN, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SETTING_CR_PERIOD, NULL));

    ADD_APPLICATION_MENUITEM((MENU8237_SCR8093_MNGCALL_MENU_CUG,
                              MENU8237_SCR8093_MNGCALL_MENU_MAIN, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SCR8093_MNGCALL_MENU_CUG, NULL));


    ADD_APPLICATION_STRING2(STR_SCR8093_MNGCALL_MENU_CAPTION, "Call Setup ", "MANAGE CALLS MAIN MENU CAPTION");

    ADD_APPLICATION_STRING2(STR_SCR8093_MNGCALL_MENU_FRWD, "Call Divert", "MANAGE CALLS MENU CALL FORWARDING OPTION ");

    ADD_APPLICATION_STRING2(
        STR_SCR8093_MNGCALL_MENU_WAITING,
        "Call Waiting  ",
        "MANAGE CALLS MENU CALL WAITING OPTION");

    ADD_APPLICATION_STRING2(
        STR_SCR8093_MNGCALL_MENU_BARRING,
        "Call Barring  ",
        "MANAGE CALLS MENU CALL BARRING OPTION ");

    ADD_APPLICATION_STRING2(STR_SCR8093_MNGCALL_MENU_CUG, "Closed User Group ", "MANAGE CALLS MENU CUG OPTION ");

    ADD_APPLICATION_STRING2(STR_SETTING_CALL_REMINDER, "Call Time Reminder", "Call Time Reminder.");

    ADD_APPLICATION_STRING2(STR_SETTING_CR_SING, "Single", "Call Time Reminder Single.");

    ADD_APPLICATION_STRING2(STR_SETTING_CR_PERIOD, "Periodic", "Call Time Reminder Periodic.");

    ADD_APPLICATION_STRING2(STR_SETTING_CR_RANGE1, "30 - 60 (sec.)", "Call Time Reminder Periodic Range.");
    ADD_APPLICATION_STRING2(STR_SETTING_CR_RANGE2, "1 - 3000 (sec.)", "Call Time Reminder Single Range.");

#ifndef __BW_MMI__
    ADD_APPLICATION_IMAGE2(
        IMG_CALLSETUP_MAIN_MENU,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "MANAGE CALLS MAIN MENU IMAGE CAPTION");
    ADD_APPLICATION_STRING2(STR_MENU_SETTING_CALL_TIME_DISPLAY, "Call Time Display", "Call Time Display Menu.");
    ADD_APPLICATION_STRING2(STR_MENU9140_AUTO_REDIAL, "Auto Redial", "Auto Redial.");
    ADD_APPLICATION_STRING2(STR_SETTING_CALL_CLOSED_GP, "Closed User Group", "Closed User Group.");
#endif /* __BW_MMI__ */ 

#if defined(__DOUBLE_PHONE_CALL__)	//  自动重拨sim2的提示
    ADD_APPLICATION_STRING2(STR_TWIN_A_ID_SIM2_AUTO_REDIAL_NOT_SUPPORTED,"Function Not Supported","Function Not Supported");
#endif
    ADD_APPLICATION_STRING2(STR_L4_ALL_TELESERVICES, "For All Teleservices ", "For All Teleservices .");
    ADD_APPLICATION_STRING2(STR_L4_TELEPHONY, "For Telephony ", "For Telephony .");
    ADD_APPLICATION_STRING2(STR_L4_ALL_DATA_TELESERVICES, "For All Data Teleservices ", "For All Data Teleservices .");
    ADD_APPLICATION_STRING2(STR_L4_FACSIMILE_SERVICES, "For Facimile Services ", "For Facimile Services .");
    ADD_APPLICATION_STRING2(STR_SHORT_MESSAGE_SERVICES, "For SMS Services ", "For SMS Services .");
    ADD_APPLICATION_STRING2(
        STR_L4_ALL_TELESERVICES_EXCEPT_SMS,
        "For All Teleservices Except SMS ",
        "For All Teleservices Except SMS .");
    ADD_APPLICATION_STRING2(
        STR_L4_VOICE_GROUP_CALL_SERVICE,
        "For Voice Group Call Services ",
        "For Voice Group Call Services .");
    ADD_APPLICATION_STRING2(
        STR_L4_VOICE_BROADCAST_SERVICE,
        "For Voice Broadcast Services ",
        "For Voice Broadcast Services .");
    ADD_APPLICATION_STRING2(STR_L4_ALL_BEARER_SERVICES, "For All Bearer Services ", "For All Bearer Services .");
    ADD_APPLICATION_STRING2(STR_L4_ALL_ASYNC_SERVICES, "For All Async Services ", "For All Async Services .");
    ADD_APPLICATION_STRING2(STR_L4_ALL_SYNC_SERVICES, "For All Sync Services ", "For All Sync Services .");
    ADD_APPLICATION_STRING2(STR_L4_ALL_DATA_CIRCUIT_SYNC, "For All Data Circuit Sync ", "For All Data Circuit Sync .");
    ADD_APPLICATION_STRING2(
        STR_L4_ALL_DATA_CIRCUIT_ASYNC,
        "For All Data Circuit Async ",
        "For All Data Circuit Async .");
    ADD_APPLICATION_STRING2(
        STR_L4_ALL_GPRS_BEARER_SERVICES,
        "For All GPRS Bearer Services ",
        "For All GPRS Bearer Services .");
	#if 0
    ADD_APPLICATION_STRING2(
        STR_L4_BS_ALLPADACCESSCA_SERVICES,
        "For All Dedicated PAD Access ",
        "For All Dedicated PAD Access .");
    ADD_APPLICATION_STRING2(
        STR_L4_BS_ALLDATAPDS_SERVICES,
        "For All DataPDS Services ",
        "For All DataPDS Services .");
#endif
    ADD_APPLICATION_STRING2(STR_UNKNOWN_CAUSE, "Unknown Cause!", "Unknown Cause!.");
    ADD_APPLICATION_STRING2(STR_CTR_REM, "Set Reminder Timer", "Set Reminder Timer!.");

    PopulateCallWait();
    PopulateCallForward();
    PopulateCallBarring();
    PopulateCallerId();
    PopulateLineSwitch();

#ifndef __MMI_REMOVE_CUG__
    PopulategrouplistMenu();
#endif 

#ifdef __IP_NUMBER__
    PopulateIPNumber();
#endif 

#ifdef __MMI_CM_BLACK_LIST__
    PopulateBlackList();
#endif 


}


/*****************************************************************************
 * FUNCTION
 *  PopulateNetworkSetupResData
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateNetworkSetupResData(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("File: [%s]  Line: [%d] <<PopulateNetworkSetupResData.>\n", __FILE__, __LINE__));

#ifdef __MMI_GPRS_FEATURES__
ADD_APPLICATION_MENUITEM((MENU9185_NETWORK_SETUP,
			#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
				MENU_MTPNP_DM_NETWORK_SETUP,
			#else
				MAIN_MENU_SETTINGS_MENUID,
			#endif
#if defined(__GSM_RAT__) && defined(__WCDMA_RAT__)
                              2+
#endif
#ifdef __MMI_USER_CTRL_PLMN_SUPPORT__
                              1+
#endif				
				3,
				MENU9186_SELECTION_MODE,
#ifdef __MMI_WLAN_FEATURES__
                          MENU_ID_NETSET_PREFERRED_MODE,
#else
                          MENU9187_PREFERRED_NETWORKS,
#endif
				MENU9188_BAND_SELECTION,
#if defined(__GSM_RAT__) && defined(__WCDMA_RAT__)
                              MENU_ID_NETSET_SERVICE_SELECTION,
                              MENU_ID_NETSET_NETWORK_INFO,
#endif
#ifdef __MMI_USER_CTRL_PLMN_SUPPORT__
                              MENU_ID_NETSET_USER_CTRL_PLMN,
#endif
				SHOW, MOVEABLEACROSSPARENT|INSERTABLE|SHORTCUTABLE, DISP_LIST,
				#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
				STRING_MTPNP_CARD1_NETWORK_SETUP,
				#else
				STR_ID_NETSET_MITEM9185,
				#endif
				IMG_SETTING_NETWORK));

    ADD_APPLICATION_MENUITEM((MENU9189_GPRS_CONNECTION_MODE, MENU9185_NETWORK_SETUP, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_MITEM9189, NULL));
    ADD_APPLICATION_STRING2(STR_ID_NETSET_MITEM9189, "GPRS Connection", "GPRS Connection Mode");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_GPRS_CONNECTION_MODE_ALWAYS, "Always", "GPRS connection mode: Always");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_GPRS_CONNECTION_MODE_WHENNEEDED, "When Needed", "GPRS connection mode: When Needed");

#else /* __MMI_GPRS_FEATURES__ */ 
ADD_APPLICATION_MENUITEM((MENU9185_NETWORK_SETUP,
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
				MENU_MTPNP_DM_NETWORK_SETUP,
#else
				MAIN_MENU_SETTINGS_MENUID,
#endif
				3,
				MENU9186_SELECTION_MODE,
				MENU9187_PREFERRED_NETWORKS,
				MENU9188_BAND_SELECTION,
				SHOW, MOVEABLEACROSSPARENT|INSERTABLE|SHORTCUTABLE, DISP_LIST,
				#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
				STRING_MTPNP_CARD1_NETWORK_SETUP,
				#else
				STR_ID_NETSET_MITEM9185,
				#endif
				IMG_SETTING_NETWORK));
#endif /* __MMI_GPRS_FEATURES__ */ 

#ifdef __MMI_USER_CTRL_PLMN_SUPPORT__
    ADD_APPLICATION_MENUITEM((MENU_ID_NETSET_USER_CTRL_PLMN, MENU9185_NETWORK_SETUP, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_USER_CTRL_PLMN, NULL));

    ADD_APPLICATION_MENUITEM((MENU_ID_NETSET_USER_CTRL_PLMN_OPTIONS,
                              0, 2,
                              MENU_ID_NETSET_USER_CTRL_PLMN_ADD,
                              MENU_ID_NETSET_USER_CTRL_PLMN_DEL,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_OPTIONS, NULL));

    ADD_APPLICATION_MENUITEM((MENU_ID_NETSET_USER_CTRL_PLMN_ADD, MENU_ID_NETSET_USER_CTRL_PLMN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_ADD, NULL));

    ADD_APPLICATION_MENUITEM((MENU_ID_NETSET_USER_CTRL_PLMN_DEL, MENU_ID_NETSET_USER_CTRL_PLMN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_DELETE, NULL));

    ADD_APPLICATION_STRING2(STR_ID_NETSET_USER_CTRL_PLMN, "PLMN management", "PLMN management");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_OP_NAME_CAPTION, "Operator Name", "Operator Name");
    ADD_APPLICATION_STRING2(
        STR_ID_NETSET_UNKNOWN_PLMN_MSG,
        "Unable to find the operator name from this phone! Do you want to input it manually?",
        "Unknown PLMN");
#endif /* __MMI_USER_CTRL_PLMN_SUPPORT__ */ 

    //ADD_APPLICATION_STRING2(STR_ID_NETSET_DUPLICATED_PLMN, "Duplicated", "Duplicated PLMN");

#ifdef __MMI_WLAN_FEATURES__
    ADD_APPLICATION_MENUITEM((MENU_ID_NETSET_PREFERRED_MODE, MENU9185_NETWORK_SETUP, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_PREFERRED_MODE, NULL));

    ADD_APPLICATION_STRING2(STR_ID_NETSET_PREFERRED_MODE, "Preferred Mode", "Preferred Mode");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_PREFERRED_MODE_GSM_ONLY, "GSM Only", "GSM Only");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_PREFERRED_MODE_WLAN_ONLY, "WLAN Only", "WLAN Only");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_PREFERRED_MODE_GSM_PREFERRED, "GSM Preferred", "GSM Preferred");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_PREFERRED_MODE_WLAN_PREFERRED, "WLAN Preferred", "WLAN Preferred");
#endif /* __MMI_WLAN_FEATURES__ */

    ADD_APPLICATION_MENUITEM((MENU9186_SELECTION_MODE, MENU9185_NETWORK_SETUP,
#ifdef __MANUAL_MODE_NW_SEL__
                              1+
#endif
#ifdef __MMI_WLAN_FEATURES__
                              1+
#endif
                              2,
                              MENU9186_SELECTION_MODE_AUTOMATIC,
                              MENU9186_SELECTION_MODE_MANUAL,
#ifdef __MANUAL_MODE_NW_SEL__
                              MENU9186_SELECTION_MODE_CHOICE,
#endif
#ifdef __MMI_WLAN_FEATURES__
                              MENU9187_PREFERRED_NETWORKS,
#endif
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_MITEM9186, NULL));

    ADD_APPLICATION_MENUITEM((MENU9186_SELECTION_MODE_MANUAL, MENU9186_SELECTION_MODE, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST,
#ifdef __MANUAL_MODE_NW_SEL__
                              STR_ID_NETSET_SELECTION_SELECT_NETWORK,
#else
                              STR_ID_NETSET_MITEM9186_MANUAL,
#endif
                              NULL));

    ADD_APPLICATION_MENUITEM((MENU9186_SELECTION_MODE_AUTOMATIC, MENU9186_SELECTION_MODE, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST,
#ifdef __MANUAL_MODE_NW_SEL__
                              STR_ID_NETSET_SELECTION_NEW_SEARCH,
#else
                              STR_ID_NETSET_MITEM9186_AUTOMATIC,
#endif
                              NULL));

#ifdef __MANUAL_MODE_NW_SEL__
    ADD_APPLICATION_MENUITEM((MENU9186_SELECTION_MODE_CHOICE, MENU9186_SELECTION_MODE, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_SELECTION_MODE_CHOICE, NULL));
    ADD_APPLICATION_STRING2(STR_ID_NETSET_SELECTION_MODE_CHOICE, "Selection Mode", "Network Selection Mode");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_SELECTION_NEW_SEARCH, "New Search", "New Search");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_SELECTION_SELECT_NETWORK, "Select Network", "Select Network");
#endif /* __MANUAL_MODE_NW_SEL__ */

    ADD_APPLICATION_MENUITEM((MENU9187_PREFERRED_NETWORKS, MENU9185_NETWORK_SETUP, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_MITEM9187, NULL));
    ADD_APPLICATION_MENUITEM((MENU9188_BAND_SELECTION, MENU9185_NETWORK_SETUP, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_MITEM9188, NULL));

#if defined(__GSM_RAT__) && defined(__WCDMA_RAT__)
    ADD_APPLICATION_MENUITEM((MENU_ID_NETSET_SERVICE_SELECTION, MENU9185_NETWORK_SETUP, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_SERVICE_SELECTION, NULL));
    ADD_APPLICATION_MENUITEM((MENU_ID_NETSET_NETWORK_INFO, MENU9185_NETWORK_SETUP, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_NETWORK_INFO, NULL));
    ADD_APPLICATION_STRING2(
        STR_ID_NETSET_SERVICE_SELECTION,
        "Service Selection",
        "Menu item string for Service Selection");
    ADD_APPLICATION_STRING2(
        STR_ID_NETSET_NETWORK_INFO,
        "Network Information",
        "Menu item string for Network Information");
#endif /* defined(__GSM_RAT__) && defined(__WCDMA_RAT__) */ 

    //ADD_APPLICATION_STRING2(STR_ID_NETSET_ITEM_WCDMA, "WCDMA", "Menu item string for WCDMA");
    //ADD_APPLICATION_STRING2(STR_ID_NETSET_ITEM_GSM, "GSM/GPRS", "Menu item string for GSM");
    //ADD_APPLICATION_STRING2(STR_ID_NETSET_ITEM_ALL, "Dual Mode", "Menu item string for Dual Mode");

    ADD_APPLICATION_STRING2(STR_ID_NETSET_MITEM9185, "Network Setup", "Menu item string for Network Setup");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_MITEM9186, "Network Selection", "Menu item string for Selection Mode");

    ADD_APPLICATION_STRING2(STR_ID_NETSET_MITEM9186_MANUAL, "Manual", "Menu item string for Selection Mode->Manual");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_MITEM9186_AUTOMATIC, "Automatic", "Menu item string for Selection Mode->Automatic");

    ADD_APPLICATION_STRING2(STR_ID_NETSET_MITEM9187, "Preferred Networks", "Menu item string for Preferred Networks");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_MITEM9188, "Band Selection", "Menu item string for Band Selection");

//[221] Added by Ryh 2009/02/12								[BEGIN]
#ifdef __MMI_MULTI_SIM__
	ADD_APPLICATION_STRING(STRING_MTPNP_SELECTION_BAND,"Band Selection");
	ADD_APPLICATION_STRING2(STR_SIM1_FIXED_DAIL_CAPTION,"SIM1 Fixed Dial","SIM1 Fixed Dial Menu.");
	ADD_APPLICATION_STRING2(STR_SIM2_FIXED_DAIL_CAPTION,"SIM2 Fixed Dial","SIM2 Fixed Dial Menu.");
	ADD_APPLICATION_STRING2(STR_SIM1_BARRED_DIAL_CAPTION,"SIM1 Barred Dial","SIM1 Barred Dial Menu.");
	ADD_APPLICATION_STRING2(STR_SIM2_BARRED_DIAL_CAPTION,"SIM2 Barred Dial","SIM2 Barred Dial Menu.");
#endif
//[221] Added by Ryh 2009/02/12								[END]

    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_MITEM9185,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "Menu item image for Network Setup");
    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_MITEM9186_MANUAL,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "Menu item image for Selection Mode->Manual");
    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_MITEM9186_AUTOMATIC,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "Menu item image for Selection Mode->Automatic");

    ADD_APPLICATION_STRING2(STR_ID_NETSET_SCR9185_CAPTION, "Network Setup", "Title string for Network Setup menu");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_SCR9186_CAPTION, "Network Selection", "Title string for Selection Mode menu");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_SCR9188_CAPTION, "Band Selection", "Title string for Band Selection menu");

    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_ABORT_LSK,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "LSK image for Searching Network screen");

    ADD_APPLICATION_STRING2(
        STR_ID_NETSET_SCR_SELECT_NETWORK_CAPTION,
        "Network List",
        "Title string for Select Network screen");

    ADD_APPLICATION_STRING2(STR_ID_NETSET_BAND_900, "900", "Band Selection sub menu 900");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_BAND_1800, "1800", "Band Selection sub menu 1800");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_BAND_DUAL, "Dual Band", "Band Selection sub menu Dual Band");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_BAND_850, "850", "Band Selection sub menu 850");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_BAND_1900, "1900", "Band Selection sub menu 1900");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_BAND_850_1900, "850/1900", "Band Selection sub menu 850/1900");

    ADD_APPLICATION_STRING2(STR_ID_NETSET_BAND_AUTO, "Auto", "Band Selection sub menu auto");

    ADD_APPLICATION_STRING2(STR_ID_NETSET_BAND_850_1800, "850/1800", "Band Selection sub menu 850/1800");

    ADD_APPLICATION_STRING2(STR_ID_NETSET_SEARCHIN_NW, "Searching", "Searching Network Message.");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_REGISTERING_NW, "Registering", "Registering Network Message.");

    ADD_APPLICATION_STRING2(STD_ID_NETSET_NW_SELECT_FAIL, "Fail to register!", "Fail to register!");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_NW_LIST_FAIL, "Fail to search network!", "Fail to search network!");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_SET_BAND_FAIL, "Fail to set band!", "Fail to set band!");
#ifdef __MMI_SLIM_PROGRESS_ANIMATION__
    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_NW_SEARCH,
        CUST_IMG_PATH "/MainLCD/UIElement/Progress/PROGRESS.GIF",
        "Progress Image");
#else /* __MMI_SLIM_PROGRESS_ANIMATION__ */ 
    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_NW_SEARCH,
        CUST_IMG_PATH "/MainLCD/Active/Searching/SEARCH.gif",
        "Img Associated with Network Search Screen.");
#endif /* __MMI_SLIM_PROGRESS_ANIMATION__ */ 

    PopulatePrefNwk();
}


/*****************************************************************************
 * FUNCTION
 *  PopulatePhoneSetupResData
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulatePhoneSetupResData(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("File: [%s]  Line: [%d] <<PopulateNetworkSetupResData.>\n", __FILE__, __LINE__));

    ADD_APPLICATION_MENUITEM((MENU_IDLE_SCR_DISP, MENU9102_INITIAL_SETUP, DISPCHAR_MENU_ITEM_COUNT,     /* menu account */
                              MENU_SETTING_WALLPAPER,   /* menu list */
#ifdef __MMI_SUB_WALLPAPER__
                              MENU_SETTING_WALLPAPER_SUB,
#endif 
                              MENU_SETTING_SCR_SVR,
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
                              MENU_SETTING_POWER_ON_DISPLAY, MENU_SETTING_POWER_OFF_DISPLAY,
#endif 
#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
#ifndef PRODUCT_QPAD
                              MENU_SETTING_MAINMENU_STYLE,
#endif 
#endif 
                              MENU_SETTING_SHOW_DT_TIME, 
#if !((defined(__MMI_MULTI_SIM__))&&(defined( __MMI_MAINLCD_128X160__)||defined( __MMI_MAINLCD_128X128__)))                              
                              MENU_SETTING_SHOW_OWN_NUMBER,
#endif                              
#if defined(__MMI_OPERATOR_NAME_MENU__)
                              MENU_SETTING_SHOW_OPN,
#endif 
#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
                              MENU_SETTING_IDLE_CLOCK_TYPE,
#endif    
                              SHOW, NONMOVEABLE, DISP_LIST, STR_IDLE_SCR_DISP, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_SHOW_DT_TIME, MENU_IDLE_SCR_DISP, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SETTING_SHOW_DT_TIME, 0));
	
	ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_DOWNLOAD, MENU_SETTING_WALLPAPER, 0,
								SHOW, NONMOVEABLE, DISP_LIST, STR_WALLPAPER_DOWNLOAD, 0));
#if 0
#if defined(__MMI_FILE_MANAGER__)

#ifdef __DAILY_WALLPAPER__      /* Wallpaper daily change */
    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER, MENU_IDLE_SCR_DISP, 2,
                              MENU_SETTING_WALLPAPER_FIX,
                              MENU_SETTING_WALLPAPER_ROTATE, SHOW, SHORTCUTABLE, DISP_LIST, STR_SETTING_WALLPAPER, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_FIX, MENU_SETTING_WALLPAPER, 2,
                              MENU_SETTING_WALLPAPER_SYSTEM,
                              MENU_SETTING_WALLPAPER_DOWNLOAD, SHOW, SHORTCUTABLE, DISP_LIST, STR_WALLPAPER_FIX, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_ROTATE, MENU_SETTING_WALLPAPER, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_WALLPAPER_ROTATE, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_SYSTEM, MENU_SETTING_WALLPAPER, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_WALLPAPER_SYSTEM, 0));
#ifdef __MMI_SUB_WALLPAPER__
    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_SUB, MENU_IDLE_SCR_DISP, 2,
                              MENU_SETTING_WALLPAPER_FIX,
                              MENU_SETTING_WALLPAPER_ROTATE,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_SETTING_WALLPAPER_SUB, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_FIX, MENU_SETTING_WALLPAPER_SUB, 2,
                              MENU_SETTING_WALLPAPER_SYSTEM,
                              MENU_SETTING_WALLPAPER_DOWNLOAD, SHOW, SHORTCUTABLE, DISP_LIST, STR_WALLPAPER_FIX, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_ROTATE, MENU_SETTING_WALLPAPER_SUB, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_WALLPAPER_ROTATE, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_SYSTEM, MENU_SETTING_WALLPAPER_SUB, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_WALLPAPER_SYSTEM, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_DOWNLOAD, MENU_SETTING_WALLPAPER_SUB, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_WALLPAPER_DOWNLOAD, 0));
#endif /* __MMI_SUB_WALLPAPER__ */ 
#else /* __DAILY_WALLPAPER__ */ /* No wallpaper daily change, normal case */
    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER, MENU_IDLE_SCR_DISP,
                              DISPSEL_MENU_TOTAL, MENU_SETTING_WALLPAPER_SYSTEM,
#ifndef __MMI_FILE_MANAGER_HIDE_MENU__
                              MENU_SETTING_WALLPAPER_DOWNLOAD,
#endif 
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_SETTING_WALLPAPER, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_SYSTEM, MENU_SETTING_WALLPAPER, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_WALLPAPER_SYSTEM, 0));
#ifdef __MMI_SUB_WALLPAPER__
    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_SUB, MENU_IDLE_SCR_DISP, 1,
                              MENU_SETTING_WALLPAPER_SYSTEM,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_SETTING_WALLPAPER_SUB, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_SYSTEM, MENU_SETTING_WALLPAPER_SUB, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_WALLPAPER_SYSTEM, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_DOWNLOAD, MENU_SETTING_WALLPAPER_SUB, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_WALLPAPER_DOWNLOAD, 0));
#endif /* __MMI_SUB_WALLPAPER__ */ 

#endif /* __DAILY_WALLPAPER__ */ 

#else /* defined(__MMI_FILE_MANAGER__) */ 
    /* Wap and FileManager are both not support */
#ifdef __DAILY_WALLPAPER__
    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER, MENU_IDLE_SCR_DISP, 2,
                              MENU_SETTING_WALLPAPER_FIX,
                              MENU_SETTING_WALLPAPER_ROTATE, SHOW, SHORTCUTABLE, DISP_LIST, STR_SETTING_WALLPAPER, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_FIX, MENU_SETTING_WALLPAPER, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_WALLPAPER_FIX, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_ROTATE, MENU_SETTING_WALLPAPER, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_WALLPAPER_ROTATE, 0));
#ifdef __MMI_SUB_WALLPAPER__
    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_SUB, MENU_IDLE_SCR_DISP, 2,
                              MENU_SETTING_WALLPAPER_FIX,
                              MENU_SETTING_WALLPAPER_ROTATE,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_SETTING_WALLPAPER_SUB, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_FIX, MENU_SETTING_WALLPAPER_SUB, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_WALLPAPER_FIX, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_ROTATE, MENU_SETTING_WALLPAPER_SUB, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_WALLPAPER_ROTATE, 0));
#endif /* __MMI_SUB_WALLPAPER__ */ 
#else /* __DAILY_WALLPAPER__ */ 
    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER, MENU_IDLE_SCR_DISP, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_SETTING_WALLPAPER, 0));
#ifdef __MMI_SUB_WALLPAPER__
    ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_SUB, MENU_IDLE_SCR_DISP, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_SETTING_WALLPAPER_SUB, 0));
#endif /* __MMI_SUB_WALLPAPER__ */ 
#endif /* __DAILY_WALLPAPER__ */ 

#endif /* defined(__MMI_FILE_MANAGER__) */ /* #if defined( __MMI_FILE_MANAGER__ ) */
#endif 
    ADD_APPLICATION_MENUITEM((MENU_SETTING_SCR_SVR, MENU_IDLE_SCR_DISP, 2,
                              MENU_SETTING_SS_SETTING,
                              MENU_SETTING_SS_SELECT, SHOW, SHORTCUTABLE, DISP_LIST, STR_MENU_SETTING_SCREEN_SVR, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_SS_SETTING, MENU_SETTING_SCR_SVR, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SCR_SCREENSAVER_SETTINGS, 0));

#if defined(__MMI_FILE_MANAGER__)
    ADD_APPLICATION_MENUITEM((MENU_SETTING_SS_SELECT, MENU_SETTING_SCR_SVR,
                              DISPSEL_MENU_TOTAL, MENU_SELECT_SS_SELECT_SYSTEM,
#ifndef __MMI_FILE_MANAGER_HIDE_MENU__
                              MEUN_SELECT_SS_SELECT_DOWNLOAD,
#endif 
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_SELECT, 0));

    ADD_APPLICATION_MENUITEM((MEUN_SELECT_SS_SELECT_DOWNLOAD, MENU_SETTING_SS_SELECT, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SCR_SCREENSAVER_DOWNLOAD, 0));

#else /* defined(__MMI_FILE_MANAGER__) */ 

    ADD_APPLICATION_MENUITEM((MENU_SETTING_SS_SELECT, MENU_SETTING_SCR_SVR, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_SELECT, 0));

#endif /* defined(__MMI_FILE_MANAGER__) */ 

#ifdef __MMI_POWER_ON_OFF_DISPLAY__
    ADD_APPLICATION_MENUITEM((MENU_SETTING_POWER_ON_DISPLAY, MENU_IDLE_SCR_DISP, 2,
                              MENU_SETTING_POWER_ON_SYSTEM,
                              MENU_SETTING_POWER_ON_DOWNLOAD,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SETTING_POWER_ON_DISPLAY, 0));
    ADD_APPLICATION_MENUITEM((MENU_SETTING_POWER_ON_SYSTEM, MENU_SETTING_POWER_ON_DISPLAY, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_ID_PHNSET_ON_OFF_SYSTEM, 0));
    ADD_APPLICATION_MENUITEM((MENU_SETTING_POWER_ON_DOWNLOAD, MENU_SETTING_POWER_ON_DISPLAY, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_ID_PHNSET_ON_OFF_DOWNLOAD, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_POWER_OFF_DISPLAY, MENU_IDLE_SCR_DISP, 2,
                              MENU_SETTING_POWER_OFF_SYSTEM,
                              MENU_SETTING_POWER_OFF_DOWNLOAD,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SETTING_POWER_OFF_DISPLAY, 0));
    ADD_APPLICATION_MENUITEM((MENU_SETTING_POWER_OFF_SYSTEM, MENU_SETTING_POWER_OFF_DISPLAY, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_ID_PHNSET_ON_OFF_SYSTEM, 0));
    ADD_APPLICATION_MENUITEM((MENU_SETTING_POWER_OFF_DOWNLOAD, MENU_SETTING_POWER_OFF_DISPLAY, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_ID_PHNSET_ON_OFF_DOWNLOAD, 0));

#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */ 

    ADD_APPLICATION_MENUITEM((MENU_SETTING_SHOW_OWN_NUMBER, MENU_IDLE_SCR_DISP, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_OWN_NO, 0));
#if defined (__LED__) || defined (__MMI_RUNNING_LED__)	
    ADD_APPLICATION_MENUITEM((MENU_SETTING_ENABLE_ROLL_LED, MENU9102_INITIAL_SETUP, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_SETTING_ENABLE_ROLL_LED, 0));
#endif

#if defined(__MMI_OPERATOR_NAME_MENU__)
    ADD_APPLICATION_MENUITEM((MENU_SETTING_SHOW_OPN, MENU_IDLE_SCR_DISP, 0, SHOW, NONMOVEABLE, DISP_LIST, STR_OPN, 0));
#endif 

#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
#ifndef PRODUCT_QPAD
    ADD_APPLICATION_MENUITEM((MENU_SETTING_MAINMENU_STYLE, MENU_IDLE_SCR_DISP, MAINMENU_STYLE_COUNT,    /* item number */
                              /* menu items */
#ifdef __MMI_MAINMENU_MATRIX_SUPPORT__
                              MENU_SETTING_MAINMENU_STYLE_MATRIX,
#endif 
#ifdef __MMI_MAINMENU_PAGE_SUPPORT__
#ifndef __MMI_TOUCH_SCREEN__
                              MENU_SETTING_MAINMENU_STYLE_PAGE,
#endif 
#endif /* __MMI_MAINMENU_PAGE_SUPPORT__ */ 
#ifdef __MMI_MAINMENU_LIST_SUPPORT__
                              MENU_SETTING_MAINMENU_STYLE_LIST,
#endif 
#ifdef __MMI_MAINMENU_CIRCULAR_SUPPORT__
                              MENU_SETTING_MAINMENU_STYLE_CIRCULAR,
#endif 
#ifdef __MMI_MAINMENU_ROTATE_SUPPORT__
                              MENU_SETTING_MAINMENU_STYLE_ROTATE,
#endif 
#ifdef __MMI_MAINMENU_TAB_SUPPORT__
                              MENU_SETTING_MAINMENU_STYLE_TAB,
#endif 
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MAINMENU_STYLE, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_MAINMENU_STYLE_MATRIX, MENU_SETTING_MAINMENU_STYLE, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MAINMENU_STYLE_MATRIX, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_MAINMENU_STYLE_PAGE, MENU_SETTING_MAINMENU_STYLE, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MAINMENU_STYLE_PAGE, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_MAINMENU_STYLE_CIRCULAR, MENU_SETTING_MAINMENU_STYLE, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MAINMENU_STYLE_CIRCULAR, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_MAINMENU_STYLE_ROTATE, MENU_SETTING_MAINMENU_STYLE, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MAINMENU_STYLE_ROTATE, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_MAINMENU_STYLE_LIST, MENU_SETTING_MAINMENU_STYLE, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MAINMENU_STYLE_LIST, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_MAINMENU_STYLE_TAB, MENU_SETTING_MAINMENU_STYLE, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_MAINMENU_STYLE_TAB, 0));
#endif
#endif /* __MMI_MAINMENU_STYLE_CHANGE_EN__ */ 
#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
    ADD_APPLICATION_MENUITEM((MENU_SETTING_IDLE_CLOCK_TYPE, MENU_IDLE_SCR_DISP, 0,
                              SHOW, (MOVEABLEWITHINPARENT | SHORTCUTABLE), DISP_LIST, STR_ID_IDLE_CLOCK_TYPE, 0));
    ADD_APPLICATION_STRING2(STR_ID_IDLE_CLOCK_TYPE, "Clock Type", "Idle Clock Type");
#endif /* defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) */ 

    /* Speed Dial */
    ADD_APPLICATION_MENUITEM((MENU_SETTING_SPEED_DIAL, MENU9102_INITIAL_SETUP, 2,
                              MENU_SETTING_SPEED_STATUS,
                              MITEM113_PBOOK_SPEED_DIAL,
                              SHOW, (MOVEABLEACROSSPARENT | SHORTCUTABLE), DISP_LIST, STR_MENU_SETTING_SPEED_DIAL, 0));
    ADD_APPLICATION_MENUITEM((MENU_SETTING_SPEED_STATUS, MENU_SETTING_SPEED_DIAL, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SETTING_SPEED_STATUS, 0));
    ADD_APPLICATION_MENUITEM((MITEM113_PBOOK_SPEED_DIAL, MENU_SETTING_SPEED_DIAL, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SCR_SPEED_DIAL_MAIN_CAPTION, IMG_GLOBAL_L2));

    /* Speed Dial - Operation */
    ADD_APPLICATION_MENUITEM((MENU_ID_SPEED_DIAL_OPERATION, 0, 2,
                              MENU_ID_SPEED_DIAL_OP_EDIT,
                              MENU_ID_SPEED_DIAL_OP_DEL, SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_SETTING_SPEED_DIAL, 0));
    ADD_APPLICATION_MENUITEM((MENU_ID_SPEED_DIAL_OP_EDIT, MENU_ID_SPEED_DIAL_OPERATION, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_EDIT, 0));
    ADD_APPLICATION_MENUITEM((MENU_ID_SPEED_DIAL_OP_DEL, MENU_ID_SPEED_DIAL_OPERATION, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_DELETE, IMG_GLOBAL_L2));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_POWER_SAVING, MENU9102_INITIAL_SETUP, 2,
                              MENU_POWER_SAVING_ON,
                              MENU_POWER_SAVING_OFF, SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_SETTING_POWER_SAVING, 0));

    ADD_APPLICATION_MENUITEM((MENU_POWER_SAVING_OFF, MENU_SETTING_POWER_SAVING, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_OFF, 0));

    ADD_APPLICATION_MENUITEM((MENU_POWER_SAVING_ON, MENU_SETTING_POWER_SAVING, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_ON, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_CONTRAST_LEVEL, MENU9102_INITIAL_SETUP, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_MENU_SETTING_CONTRAST_LEVEL, 0));
#ifdef __MMI_NITZ__
    ADD_APPLICATION_MENUITEM((MENU_SETTING_MENU_TREE, MENU9102_INITIAL_SETUP, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_SETTING_MENU_TREE, 0));
#endif /* __MMI_NITZ__ */ 
    ADD_APPLICATION_MENUITEM((MENU_SETTING_SCHEDULE_POWER, MENU9102_INITIAL_SETUP, 2,
                              MENU_SETTING_SCHEDULE_POWER_SET1,
                              MENU_SETTING_SCHEDULE_POWER_SET2,
                              SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST, STR_MENU_SETTING_SCHEDULE_POWER,
                              0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_SCHEDULE_POWER_SET1, MENU_SETTING_SCHEDULE_POWER, 2,
                              MENU_SETTING_SCH_POW_SET1_POW_ON,
                              MENU_SETTING_SCH_POW_SET1_POW_OFF,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_SETTING_SCHEDULE_POWER_SET1, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_SCHEDULE_POWER_SET2, MENU_SETTING_SCHEDULE_POWER, 2,
                              MENU_SETTING_SCH_POW_SET1_POW_ON,
                              MENU_SETTING_SCH_POW_SET1_POW_OFF,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_SETTING_SCHEDULE_POWER_SET2, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_SCH_POW_SET1_POW_ON, MENU_SETTING_SCHEDULE_POWER_SET2, 2,
                              MENU9171_ON,
                              MENU9172_OFF, SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_SETTING_SCH_POW_SET1_POW_ON, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_SCH_POW_SET1_POW_OFF, MENU_SETTING_SCHEDULE_POWER_SET2, 2,
                              MENU9171_ON,
                              MENU9172_OFF, SHOW, NONMOVEABLE, DISP_LIST, STR_MENU_SETTING_SCH_POW_SET1_POW_OFF, 0));

    ADD_APPLICATION_STRING2(STR_MENU_SETTING_SPEED_DIAL, "Speed Dial", "Speed Dial Menu.");

    ADD_APPLICATION_STRING2(
        STR_SETTING_CALL_TIME_DISPLAY_CAPTION,
        "Call Time Display",
        "Call Time Display Menu Caption.");

    ADD_APPLICATION_STRING2(STR_MENU_SETTING_POWER_SAVING, "Power Saving", "Power Saving Menu.");
    ADD_APPLICATION_STRING2(STR_SETTING_POWER_SAVING_CAPTION, "Power Saving", "Power Saving Menu Caption.");

    ADD_APPLICATION_STRING2(STR_MENU_SETTING_CONTRAST_LEVEL, "Contrast", "Contrast Level Menu Caption.");
    ADD_APPLICATION_STRING2(STR_SETTING_CONTRAST_LEVEL_CAPTION, "Contrast Level", "Contrast Level Caption.");
#if defined(__MMI_NITZ__)
    ADD_APPLICATION_STRING2(STR_MENU_SETTING_MENU_TREE, "Auto update of date and time", "Menu Tree Menu.");
#endif 
    ADD_APPLICATION_STRING2(STR_MENU_SETTING_SCREEN_SVR, "Screen Saver", "Screen Saver Menu Caption.");
    ADD_APPLICATION_STRING2(STR_SETTING_WALLPAPER, "Wallpaper", "Wallpaper Menu Caption.");
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
    ADD_APPLICATION_STRING2(STR_SETTING_POWER_ON_DISPLAY, "Power On Display", "Power on Menu Caption.");
    ADD_APPLICATION_STRING2(STR_SETTING_POWER_OFF_DISPLAY, "Power Off Display", "Power off Menu Caption.");
#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */ 
#ifdef __MMI_SUB_WALLPAPER__
    ADD_APPLICATION_STRING2(STR_SETTING_WALLPAPER_SUB, "Sub Wallpaper", "Wallpaper Menu Caption");
#endif 
    ADD_APPLICATION_STRING2(STR_SETTING_SHOW_DT_TIME, "Show Date and Time", "Show Date and Time Menu Caption.");

    ADD_APPLICATION_STRING2(STR_OWN_NO, "Show Owner Number", "Show Own Number Option.");

#if defined(__MMI_OPERATOR_NAME_MENU__)
    ADD_APPLICATION_STRING2(STR_OPN, "Show Operator Name", "Show Own Number Option.");
#endif 
    ADD_APPLICATION_STRING2(STR_IDLE_SCR_DISP, "Idle Screen Display", "Idle Screen Display Option.");
    ADD_APPLICATION_STRING2(STR_MENU_SETTING_SCHEDULE_POWER, "Schedule Power On/Off", "Scheduled Power On/Off Menu.");
    ADD_APPLICATION_STRING2(STR_MENU_SETTING_SCHEDULE_POWER_SET1, "Set 1", "Set 1 Option.");
    ADD_APPLICATION_STRING2(STR_MENU_SETTING_SCHEDULE_POWER_SET2, "Set 2", "Set 2 Option.");
    ADD_APPLICATION_STRING2(STR_MENU_SETTING_SCH_POW_SET1_POW_ON, "Power On", "On Option.");
    ADD_APPLICATION_STRING2(STR_MENU_SETTING_SCH_POW_SET1_POW_OFF, "Power Off", "Off Option.");
    ADD_APPLICATION_STRING2(STR_SETTING_SPEED_STATUS, "Enable Speed Dial", "Off Option.");

    ADD_APPLICATION_STRING2(STR_SETTING_MAIN_LCD, "Main LCD", "Main LCD Option.");
    ADD_APPLICATION_STRING2(STR_SETTING_SUB_LCD, "Sub LCD", "Sub LCD Option.");

    ADD_APPLICATION_STRING2(STR_POWER_SAVE_TIMER_CAPTION, "Power Save Timer", "Power Save Timer Caption.");
    ADD_APPLICATION_STRING2(STR_DONE, "Done", "Done Option.");
    ADD_APPLICATION_STRING2(STR_EDIT, "Edit", "Edit Option.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_GREET_STATUS,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/Common/ST_STATU.PBM",
        "Setting Submenu image for Greeting text status.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_GREET_TEXT,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/Greeting/ST_NOTE.PBM",
        "Setting Submenu image for Greeting Text .");

    ADD_APPLICATION_STRING2(STR_SCR_SPEED_DIAL_MAIN_CAPTION, "Set Number", "STR_SCR_SPEED_DIAL_MAIN_CAPTION");
    ADD_APPLICATION_STRING2(STR_SCR_SPEED_DIAL_LIST_CAPTION, "Set Number", "STR_SCR_SPEED_DIAL_LIST_CAPTION");
    ADD_APPLICATION_STRING2(STR_SPEEDDIAL_NO_NUM_TODIAL_TEXT, "No Number To Dial!", "STR_SPEEDDIAL_PHONEBOOK_LIST");

#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
    ADD_APPLICATION_STRING2(STR_MAINMENU_STYLE, "MainMenu Style", "MainMenu Style Title");
    ADD_APPLICATION_STRING2(STR_MAINMENU_STYLE_MATRIX, "Matrix", "MainMenu Style Matrix");
    ADD_APPLICATION_STRING2(STR_MAINMENU_STYLE_PAGE, "Page", "MainMenu Style Page");
    ADD_APPLICATION_STRING2(STR_MAINMENU_STYLE_CIRCULAR, "Circular", "MainMenu Style Circular");
    ADD_APPLICATION_STRING2(STR_MAINMENU_STYLE_ROTATE, "Rotate", "MainMenu Style Rotate");
    ADD_APPLICATION_STRING2(STR_MAINMENU_STYLE_LIST, "List", "MainMenu Style List");
    ADD_APPLICATION_STRING2(STR_MAINMENU_STYLE_TAB, "Tab", "MainMenu Style List");
#endif /* __MMI_MAINMENU_STYLE_CHANGE_EN__ */ 
#if defined (__LED__) || defined (__MMI_RUNNING_LED__)	
    ADD_APPLICATION_STRING2(STR_MENU_SETTING_ENABLE_ROLL_LED, "Running LED Switch","SETTING RUNNING LED Status");
#endif
    PRINT_INFORMATION(("Populating Display Char.  Resources\n"));
    PopulateDisplayChar();
    PopulateExtImage();
}

#ifdef __MMI_SOUND_EFFECT__


/*****************************************************************************
 * FUNCTION
 *  PopulateSoundEffectResData
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateSoundEffectResData(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("File: [%s]  Line: [%d] <<PopulateSoundEffectResData.>\n", __FILE__, __LINE__));

    /* add menuitems */
#ifndef PRODUCT_QPAD
    ADD_APPLICATION_MENUITEM((MENU_SETTING_SOUND_EFFECT, MAIN_MENU_SETTINGS_MENUID,                                    
				#if defined(__MMI_BG_SOUND_EFFECT__)
							1+
				#endif
				#if defined(__MMI_AUDIO_REVERB_EFFECT__)
							1+
				#endif
				#if defined(__MMI_AUDIO_SURROUND_EFFECT__)
							1+
				#endif
							1,
                               MENU_SETTING_AUDIO_EQUALIZIER,
                           #if defined(__MMI_BG_SOUND_EFFECT__)
                               MENU_ID_BGSND_CALL_SETUP,
                           #endif
                           #if defined(__MMI_AUDIO_REVERB_EFFECT__)
                               MENU_SETTING_AUDIO_REVERB_EFFECT,
                           #endif
                           #if defined(__MMI_AUDIO_SURROUND_EFFECT__)
                               MENU_SETTING_AUDIO_SURROUND_EFFECT,
                           #endif
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_SETTING_SOUND_EFFECT,
                               IMG_ID_SETTING_SOUND_EFFECT));
						   
    ADD_APPLICATION_MENUITEM((MENU_SETTING_AUDIO_EQUALIZIER, MENU_SETTING_SOUND_EFFECT, 0, SHOW,
                              MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST, STR_ID_SETTING_AUDIO_EQUALIZER,
                              IMG_ID_SETTING_AUDIO_EQUALIZER));
#endif
#ifdef __MMI_EDITABLE_AUDIO_EQUALIZER__
    /* menu items for the setting options. */

    ADD_APPLICATION_MENUITEM((MENU_SETTING_AUDIO_EQUALIZER_SET_OPT, 0, 3, MENU_SETTING_AUDIO_EQUALIZER_SET_OPT_SELECT,
                              MENU_SETTING_AUDIO_EQUALIZER_SET_OPT_EDIT, MENU_SETTING_AUDIO_EQUALIZER_SET_OPT_RENAME,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_OPTIONS, NULL));
    ADD_APPLICATION_MENUITEM((MENU_SETTING_AUDIO_EQUALIZER_SET_OPT_SELECT, MENU_SETTING_AUDIO_EQUALIZER_SET_OPT, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_SELECT, NULL));
    ADD_APPLICATION_MENUITEM((MENU_SETTING_AUDIO_EQUALIZER_SET_OPT_EDIT, MENU_SETTING_AUDIO_EQUALIZER_SET_OPT, 0, SHOW,
                              MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_EDIT, NULL));
    ADD_APPLICATION_MENUITEM((MENU_SETTING_AUDIO_EQUALIZER_SET_OPT_RENAME, MENU_SETTING_AUDIO_EQUALIZER_SET_OPT, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_SETTING_AUDIO_EQUALIZER_SETTING_RENAME,
                              NULL));

    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_EQUALIZER_CAPTION, "Equalizer", "Title string for the Equalizer Menu");
    ADD_APPLICATION_STRING2(
        STR_ID_SETTING_AUDIO_EQUALIZER_REST_FACT_DEFAULT,
        "Restore Factory Default",
        "String for Restoring the Audio Equalizer Setting values to default");
    ADD_APPLICATION_STRING2(
        STR_ID_SETTING_AUDIO_EQUALIZER_SETTING_RENAME,
        "Rename Setting",
        "String for menu item for renaming of audio equalizer setting");
    ADD_APPLICATION_STRING2(
        STR_ID_SETTING_AUDIO_EQUALIZER_SETTIN_NAME_EDIT_TITLE,
        "Enter Name",
        "Title string for setting name editing screen ");
    ADD_APPLICATION_STRING2(
        STR_ID_SETTING_AUDIO_EQUALIZER_SETTING_INVALID_NAME,
        "Invalid Name",
        "Warning string for invalid/empty name");
#endif /* __MMI_EDITABLE_AUDIO_EQUALIZER__ */ 
    /* add string resources */
    ADD_APPLICATION_STRING2(STR_ID_SETTING_SOUND_EFFECT, "Sound Effect", "Menu item string for Sound Effect");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_EQUALIZER, "Equalizer", "Menu item string for Audio Equalizer");
    ADD_APPLICATION_STRING2(
        STR_ID_SETTING_AUDIO_EQUALIZER_SET_1,
        "Normal",
        "Menu item string for Audio Equalizer Set 1");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_EQUALIZER_SET_2, "Bass", "Menu item string for Audio Equalizer Set 2");
    ADD_APPLICATION_STRING2(
        STR_ID_SETTING_AUDIO_EQUALIZER_SET_3,
        "Dance",
        "Menu item string for Audio Equalizer Set 3");
    ADD_APPLICATION_STRING2(
        STR_ID_SETTING_AUDIO_EQUALIZER_SET_4,
        "Classical",
        "Menu item string for Audio Equalizer Set 4");
    ADD_APPLICATION_STRING2(
        STR_ID_SETTING_AUDIO_EQUALIZER_SET_5,
        "Treble",
        "Menu item string for Audio Equalizer Set 5");
    ADD_APPLICATION_STRING2(
        STR_ID_SETTING_AUDIO_EQUALIZER_SET_6,
        "Party",
        "Menu item string for Audio Equalizer Set 6");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_EQUALIZER_SET_7, "Pop", "Menu item string for Audio Equalizer Set 7");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_EQUALIZER_SET_8, "Rock", "Menu item string for Audio Equalizer Set 8");

    /* add image resources */
    ADD_APPLICATION_IMAGE2(
        IMG_ID_SETTING_SOUND_EFFECT,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_SNDEFT.pbm",
        "Menu item image for Sound Effect");
    ADD_APPLICATION_IMAGE2(
        IMG_ID_SETTING_AUDIO_EQUALIZER,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SoundEffect/ST_AUDIO_EQ.pbm",
        "Menu item image for Audio Equalizer");

    /*
     *   Populate Audio Reverb Effect Resource.
     */
#ifdef __MMI_AUDIO_REVERB_EFFECT__
    /* Add menuitems */
    ADD_APPLICATION_MENUITEM((MENU_SETTING_AUDIO_REVERB_EFFECT, MENU_SETTING_SOUND_EFFECT, 0,
                              SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST, 
                              STR_ID_SETTING_AUDIO_REVERB_EFFECT, IMG_ID_SETTING_AUDIO_REVERB_EFFECT));

    /* Add string resources */
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_REVERB_EFFECT, "Reverb Effect", "Menu item string for Audio Reverb Effect");

    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_REVERB_EFFECT_SET1, "Concert", "Menu item string for Audio Reverb Effect");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_REVERB_EFFECT_SET2, "Shower", "Menu item string for Audio Reverb Effect");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_REVERB_EFFECT_SET3, "Church", "Menu item string for Audio Reverb Effect");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_REVERB_EFFECT_SET4, "Small Room", "Menu item string for Audio Reverb Effect");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_REVERB_EFFECT_SET5, "Opera", "Menu item string for Audio Reverb Effect");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_REVERB_EFFECT_SET6, "Bath Room", "Menu item string for Audio Reverb Effect");

    /* add image resources */
    ADD_APPLICATION_IMAGE2(
        IMG_ID_SETTING_AUDIO_REVERB_EFFECT,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SoundEffect/ST_AUDIO_REVERB.pbm",
        "Menu item image for Audio Reverb Effect");
#endif /* __MMI_AUDIO_REVERB_EFFECT__ */ 

    /*
     *   Populate Audio Surround Effect Resource.
     */
#ifdef __MMI_AUDIO_SURROUND_EFFECT__
    /* Add menuitems */
    ADD_APPLICATION_MENUITEM((MENU_SETTING_AUDIO_SURROUND_EFFECT, MENU_SETTING_SOUND_EFFECT, 0,
                              SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST,
                              STR_ID_SETTING_AUDIO_SURROUND_EFFECT, IMG_ID_SETTING_AUDIO_SURROUND_EFFECT));

    /* Add string resources */
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_SURROUND_EFFECT, "3D Surround Effect", "Menu item string for Audio Surround Effect");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_SURROUND_PLUG_EAPHONE, "Plug earphone for the effect", "Menu item string for Audio Surround Effect");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_SURROUND_MODE1, "Mode 1", "Menu item string for Audio Surround Effect");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_SURROUND_MODE2, "Mode 2", "Menu item string for Audio Surround Effect");

    /* add image resources */
    ADD_APPLICATION_IMAGE2(
        IMG_ID_SETTING_AUDIO_SURROUND_EFFECT,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SoundEffect/ST_AUDIO_3D.pbm",
        "Menu item image for Audio Surround Effect");
#endif /*__MMI_AUDIO_SURROUND_EFFECT__*/

#if defined(__MMI_AUDIO_SURROUND_EFFECT__) && defined(__MMI_AUDIO_REVERB_EFFECT__)
    ADD_APPLICATION_STRING2(STR_ID_SETTING_AUDIO_EFFECT_NO_COEXIST, "Reverb and surround can't turn on together", "Menu item string for Audio Surround Effect");
#endif /*#if defined(__MMI_AUDIO_SURROUND_EFFECT__) || (__MMI_AUDIO_REVERB_EFFECT__)*/

}
#endif /* __MMI_SOUND_EFFECT__ */ 


/*****************************************************************************
 * FUNCTION
 *  PopulatePrefNwk
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulatePrefNwk(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_STRING2(
        STR_ID_NETSET_PRF_NWK_TITLE,
        "Preferred Networks",
        "Menu item string for Preferred Network ");
    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_PRF_NWK_TITLE,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "Title image for Preferred Network Menu");
    ADD_APPLICATION_IMAGE2(IMG_ID_NETSET_LSK_ABORT, CUST_IMG_BASE_PATH "/NoImage.bmp", "LSK Button Image for Abort");
    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_PNWK_GET_LIST_MSG,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "Image to show user with Preferred Network Message");

    ADD_APPLICATION_STRING2(
        STR_ID_NETSET_LSK_OPTIONS,
        "Preferred Networks Options",
        "String for options button in preferred network");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_PRR_NWK_NEW, "Create New", "Create New preferred network");
    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_LSK_OPTIONS,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "LSK Icon for Options button");

    ADD_APPLICATION_MENUITEM((MITEM_PRF_NWK_OPTIONS_PSEUDO,
                              0, 4,
                              MITEM_PRR_NWK_ADD,
                              MITEM_PRR_NWK_NEW,
                              MITEM_PRR_NWK_EDIT,
                              MITEM_PRR_NWK_DELETE,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_LSK_OPTIONS, NULL));

    ADD_APPLICATION_MENUITEM((MITEM_PRR_NWK_ADD, MITEM_PRF_NWK_OPTIONS_PSEUDO, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_MITEM_PRF_NWK_ADD, NULL));

    ADD_APPLICATION_MENUITEM((MITEM_PRR_NWK_NEW, MITEM_PRF_NWK_OPTIONS_PSEUDO, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_PRR_NWK_NEW, NULL));

    ADD_APPLICATION_MENUITEM((MITEM_PRR_NWK_DELETE, MITEM_PRF_NWK_OPTIONS_PSEUDO, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_DELETE,
                              IMG_ID_NETSET_MITEM_PRF_NWK_DELETE));

    ADD_APPLICATION_MENUITEM((MITEM_PRR_NWK_EDIT, MITEM_PRF_NWK_OPTIONS_PSEUDO, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_MITEM_PRF_NWK_EDIT,
                              IMG_ID_NETSET_MITEM_PRF_NWK_EDIT));

    /* diamond, 2006/03/08 New PLMN management */
	#if 0
    ADD_APPLICATION_MENUITEM((MENU_ID_ADD_FROM_LIST_OPTIONS, 0, 3,
                              MENU_ID_ADD_FROM_LIST_DONE,
                              MENU_ID_ADD_FROM_LIST_SORT_BY_DEFAULT,
                              MENU_ID_ADD_FROM_LIST_SORT_BY_ALPHABET,
                              SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_GLOBAL_OPTIONS, IMG_SETTING_NETWORK));

    ADD_APPLICATION_MENUITEM((MENU_ID_ADD_FROM_LIST_DONE, MENU_ID_ADD_FROM_LIST_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_SELECT, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_ADD_FROM_LIST_SORT_BY_DEFAULT, MENU_ID_ADD_FROM_LIST_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_PLMN_LIST_SORT_BY_DEFAULT, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_ADD_FROM_LIST_SORT_BY_ALPHABET, MENU_ID_ADD_FROM_LIST_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_PLMN_LIST_SORT_BY_ALPHABET, 0));
#endif
    ADD_APPLICATION_STRING2(STR_ID_NETSET_MITEM_PRF_NWK_ADD, "Add From List", "Menu Item Add Preferred Network");

    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_MITEM_PRF_NWK_DELETE,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "Icon for Menu Item to Delete Preferred Network");

    ADD_APPLICATION_STRING2(STR_ID_NETSET_MITEM_PRF_NWK_EDIT, "Change Priority", "Menu Item Edit Preferred Network");
    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_MITEM_PRF_NWK_EDIT,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "Icon for Menu Item to Edit Preferred Network");

    ADD_APPLICATION_STRING2(
        STR_ID_NETSET_PRF_NWK_ADDED_ERR,
        "Unable To Add Network",
        "Message String for Deleted Preferred Network Error");
    ADD_APPLICATION_STRING2(
        STR_ID_NETSET_PRF_ADD_SHW_NWK_TITLE,
        "PLMN LIST",
        "Title String for Error Screen Preferred Network");
    ADD_APPLICATION_STRING2(
        STR_ID_NETSET_PRF_ADD_SHW_NWK_PRIORITY_TITLE,
        "Priority",
        "Title String for User Priority Preferred Network");

    ADD_APPLICATION_STRING2(STR_ID_NETSET_PRR_MCC_CAPTION, "MCC/MNC", "MCC/MNC String");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_PRR_PR_CAPTION, "Priority", "Priority String");
    //ADD_APPLICATION_STRING2(STR_ID_NETSET_PRR_SV_CAPTION, "Service", "Service String");

#ifdef __MANUAL_MODE_NW_SEL__
    ADD_APPLICATION_STRING2(STR_ID_NETSET_MANUAL_MODE_TITLE, "Select Network", "Select Network As Manual Mode");
    ADD_APPLICATION_STRING2(STR_ID_NETSET_MANUAL_MODE_SHORT, "Sel. Network", "Select Network As Manual Mode");
#endif /* __MANUAL_MODE_NW_SEL__ */ 

    /* diamond, 2006/03/08 New PLMN management */
    //ADD_APPLICATION_STRING2(STR_ID_PLMN_LIST_SORT_BY_ALPHABET, "Sort by alphabet", "Sort PLMN list");
    //ADD_APPLICATION_STRING2(STR_ID_PLMN_LIST_SORT_BY_DEFAULT, "Sort by default", "Sort PLMN list");

    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_PRF_HOME_PLMN,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/ST_HOME.PBM",
        "home img");
    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_PRF_ADDNEW_MMC,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/preferredNetworks/SB_MMC.pbm",
        "MMC Image");
    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_PRF_ADDNEW_POY,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/Common/SB_POY.pbm",
        "POY Image");

    /* diamond, 2006/06/05 forbidden PLMN icons */
	#if 0
    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_FORBIDDEN_PLMN,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_Off.pbm",
        "Forbidden PLMN image");

    ADD_APPLICATION_IMAGE2(
        IMG_ID_NETSET_UNKNOWN_PLMN,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_Unknow.pbm",
        "Unknown PLMN image");
	#endif
}


/*****************************************************************************
 * FUNCTION
 *  PopulateSpofRes
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateSpofRes(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_STRING2(SPOF_MAIN_TITLE_STRINGID, "Sched. Power On/Off", "Title of Scheuled Power On/Off Screen.");
    ADD_APPLICATION_IMAGE2(SPOF_EDIT_BUTTON_IMAGEID, CUST_IMG_BASE_PATH "/EmptyImage.bmp", "Icon for Edit Button.");
    ADD_APPLICATION_IMAGE2(
        SPOF_TYPEPOWERON_IMAGEID,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/Poweronoff/ST_PON.PBM",
        "Icon for Type of SPOF on/off.");
    ADD_APPLICATION_IMAGE2(
        SPOF_TYPEPOWEROFF_IMAGEID,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/Poweronoff/ST_POFF.PBM",
        "Icon for Type of SPOF on/off.");
    ADD_APPLICATION_IMAGE2(
        SPOF_BLANK_IMAGEID,
        CUST_IMG_BASE_PATH "/EmptyImage.bmp",
        "Icon for Title of Alarms Option Screen.");
    ADD_APPLICATION_IMAGE2(SPOF_SETTING_STATUS_IMAGEID, CUST_IMG_PATH "/MainLCD/SubMenu/Settings/common/ST_STATU.PBM", "Setting Image for Power on off Status icon");    /* have to check */
    ADD_APPLICATION_STRING2(SPOF_EDIT_TITLE_STRINGID, "Edit Power on/off", "Title of Alarms Option Screen.");
    ADD_APPLICATION_STRING2(SPOF_STATUS_STRINGID, "Status", "Status Caption");
    ADD_APPLICATION_STRING2(SPOF_DISABLE_STRINGID, "Disable", "Disable.");
    ADD_APPLICATION_STRING2(SPOF_ENABLE_STRINGID, "Enable", "Enable");
    ADD_APPLICATION_STRING2(SPOF_TIME_CONFLICT, "Conflict", "Conflict String");
    ADD_APPLICATION_STRING2(SPOF_POWER_OFF_CONFIRM, "Power Off?", "power off String");

}


/*****************************************************************************
 * FUNCTION
 *  AutoGenDispCharRes
 * DESCRIPTION
 *  Automatically generate all display char. resources.
 * PARAMETERS
 *  void
 * RETURNS
 *  True: success; False: can't open the folder of all images
 *****************************************************************************/
#ifndef LOW_COST_SUPPORT
MMI_BOOL AutoGenDispCharRes(void)
{
}


#endif /* LOW_COST_SUPPORT */


/*****************************************************************************
 * FUNCTION
 *  PopulateDisplayChar
 * DESCRIPTION
 *  Populate all display char. resources.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateDisplayChar(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /* Populating Imcoming call reminder waring , add by kenneth */
    ADD_APPLICATION_STRING2(STR_SETTING_SOUND_WARNING, "Sound warning", "Imcoming call reminder waring");
    ADD_APPLICATION_STRING2(STR_SETTING_CLOSE, "Close", "Imcoming call reminder waring");
    ADD_APPLICATION_STRING2(STR_SETTING_1_MINIUTES, "One Minute", "Imcoming call reminder waring");
    ADD_APPLICATION_STRING2(STR_SETTING_5_MINIUTES, "Five Minute", "Imcoming call reminder waring");
    ADD_APPLICATION_STRING2(STR_SETTING_10_MINITES, "Ten Minute", "Imcoming call reminder waring");

    /* Populating blacklist grouplist , add by kenneth */
    ADD_APPLICATION_STRING2(STR_SETTING_CALLGROUP_LIST,"CallGroupBlackRedlist","CallGroupBlacklist");
    ADD_APPLICATION_STRING2(STR_SETTING_BLACK_RED_LIST_NONE,"None","CallGroupBlacklist");
    ADD_APPLICATION_STRING2(STR_SETTING_BLACK_LIST_ACTIVE,"CallGroupBlacklist","CallGroupBlacklist");
    ADD_APPLICATION_STRING2(STR_SETTING_RED_LIST_ACTIVE,"CallGroupRedlist","CallGroupBlacklist");
 
    ADD_APPLICATION_STRING2(STR_DEDICATED_OK,"OK Key","Dedicated Key");


    /* Populating Wallpaper dependent Resources */
    ADD_APPLICATION_STRING2(STR_WALLPAPER_CAPTION, "Wallpaper", "STR_MENU_WALLPAPER_TYPES");
    ADD_APPLICATION_STRING2(STR_WALLPAPER_SYSTEM, "System", "STR_WALLPAPER_SYSTEM");
    ADD_APPLICATION_STRING2(STR_WALLPAPER_DOWNLOAD, "Download", "STR_WALLPAPER_DOWNLOAD");
#ifdef __DAILY_WALLPAPER__
    ADD_APPLICATION_STRING2(STR_WALLPAPER_FIX, "Fix", "STR_WALLPAPER_FIX");
    ADD_APPLICATION_STRING2(STR_WALLPAPER_ROTATE, "Rotate", "STR_WALLPAPER_ROTATE");
#endif /* __DAILY_WALLPAPER__ */ 

#ifdef __MMI_THEMES_V2_SUPPORT__
    ADD_APPLICATION_IMAGE2(IMG_ID_DISPCHAR_THEME_WALLPAPER, CUST_IMG_PATH "/MainLCD/IdleScreen/Wallpaper/WALL01.gif", "wallpaper download");
    ADD_APPLICATION_IMAGE2(IMG_ID_DISPCHAR_THEME_SCREENSAVER, CUST_IMG_PATH "/MainLCD/IdleScreen/Wallpaper/WALL01.gif", "wallpaper download");
#endif 

    ADD_APPLICATION_IMAGE2(IMG_WALLPAPER_SYSTEM, CUST_IMG_BASE_PATH "/NoImage.bmp", "IMG_WALLPAPER_SYSTEM");
    ADD_APPLICATION_IMAGE2(IMG_WALLPAPER_DOWNLAOD, CUST_IMG_BASE_PATH "/NoImage.bmp", "IMG_WALLPAPER_DOWNLAOD");
    ADD_APPLICATION_IMAGE2(IMG_WALLPAPER_CAPTION, CUST_IMG_BASE_PATH "/NoImage.bmp", "IMG_WALLPAPER_CAPTION");
    ADD_APPLICATION_STRING2(STR_WALLPAPER_SELECT_CAPTION, "Wallpaper Select", "STR_WALLPAPER_SET");
    ADD_APPLICATION_STRING2(STR_WPSS_IMG_ERR_TYPE, "Type not support", "User defined wallpaper error message");
    ADD_APPLICATION_STRING2(STR_WPSS_IMG_ERR_FILE, "Image file error", "User defined wallpaper error message");
    ADD_APPLICATION_STRING2(STR_WPSS_IMG_ERR_SIZE, "File size too large", "User defined wallpaper error message");
    ADD_APPLICATION_STRING2(STR_WPSS_IMG_ERR_DIMESION, "Image size to large", "User defined wallpaper error message");
    ADD_APPLICATION_STRING2(
        STR_IDLE_WP_DECODE_ERR,
        "Image error, Restore Default Wallpaper",
        "User defined wallpaper error message");
    ADD_APPLICATION_STRING2(
        STR_IDLE_SS_DECODE_ERR,
        "Image error, Restore Default Screen Saver",
        "User defined wallpaper error message");
//    ADD_APPLICATION_STRING2(STR_ID_WP_FRAME_ERR, "Image error: Frame > 30", "User defined wallpaper error message");
   // ADD_APPLICATION_STRING2(STR_ID_SUB_WP_FRAME_ERR, "Image error: Frame > 20", "User defined wallpaper error message");

    /* Populating Screensaver dependent Resources */
    ADD_APPLICATION_STRING2(STR_SCR_SCREENSAVER_CAPTION, "Screen Saver", "SS CAPTION");
    ADD_APPLICATION_STRING2(STR_SCR_SCREENSAVER_SETTINGS, "Setting", "Setting");
    ADD_APPLICATION_STRING2(SCR_SCREENSAVER_SETTING_CAPTION, "Screen Saver Setting", "Screen Saver Setting");
    ADD_APPLICATION_STRING2(STR_SCR_SCREENSAVER_SYSTEM, "System", "System");
    ADD_APPLICATION_STRING2(STR_SCR_SCREENSAVER_DOWNLOAD, "Download", "Download");
    ADD_APPLICATION_STRING2(STR_SCREENSAVER_VIEW_CAPTION, "Screen Saver", "Screen Saver");
    ADD_APPLICATION_STRING2(STR_NO_SCREENSAVER_AVAIL, "Screen Saver List Is Empty", "Screen Saver List Is Empty");
    ADD_APPLICATION_STRING2(STR_SCREENSAVER_5SEC, "5 Sec", "5SEC");
    ADD_APPLICATION_STRING2(STR_SCREENSAVER_15SEC, "15 Sec", "15SEC ");
    ADD_APPLICATION_STRING2(STR_SCREENSAVER_30SEC, "30 Sec", "30SEC");
    ADD_APPLICATION_STRING2(STR_SCREENSAVER_1MINUTE, "1 Minute", "1MIN");
    ADD_APPLICATION_STRING2(STR_SCREENSAVER_STATUS, "Status:", "STATUS");
    ADD_APPLICATION_STRING2(STR_SCREENSAVER_WAITING, "Waiting:", "WAITING");
    ADD_APPLICATION_STRING2(STR_SCREENSAVER_EMPTY, "Empty", "Empty Sting");

#ifdef __MMI_LCD_PARTIAL_ON__
    ADD_APPLICATION_STRING2(STR_SCREENSAVER_DISPLAY_TIME, "Display Time", "Screen Saver for partial display ");
#endif 
#ifdef __MMI_THEMES_V2_SUPPORT__
    ADD_APPLICATION_STRING2(STR_ID_DISPCHAR_THEME_WALLPAPER, "Theme Image", "Wallpaper Menu string");
    ADD_APPLICATION_STRING2(STR_ID_DISPCHAR_THEME_SCREENSAVER, "Theme Image", "Wallpaper Menu string");
#endif 

    ADD_APPLICATION_IMAGE2(
        IMAGE_SETTING_TIME,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/Common/ST_TIME.PBM",
        "Icon for Time In  Category 57");

#ifdef __MMI_POWER_ON_OFF_DISPLAY__
    /* Populating Power on/off dependent Resources */
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_CAPTION, "Power On/Off Display", "STR_ID_PHNSET_ON_OFF_CAPTION");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_SYSTEM, "System", "STR_ID_PHNSET_ON_OFF_SYSTEM");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_DOWNLOAD, "User Deine", "STR_ID_PHNSET_ON_OFF_DOWNLOAD");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_BAD_FILE, "Invalid File", "STR_ID_PHNSET_ON_OFF_BAD_FILE");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_SELECT_CAPTION, "On/off Select", "STR_WALLPAPER_SET");
    ADD_APPLICATION_IMAGE2(IMG_ID_PHNSET_ON_OFF_SYSTEM, CUST_IMG_BASE_PATH "/NoImage.bmp", "IMG_WALLPAPER_SYSTEM");
    ADD_APPLICATION_IMAGE2(
        IMG_ID_PHNSET_ON_OFF_DOWNLAOD,
        CUST_IMG_BASE_PATH "/NoImage.bmp",
        "IMG_WALLPAPER_DOWNLAOD");
    ADD_APPLICATION_IMAGE2(IMG_ID_PHNSET_ON_OFF_CAPTION, CUST_IMG_BASE_PATH "/NoImage.bmp", "IMG_WALLPAPER_CAPTION");
#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */ 

#ifdef __MMI_ANALOG_CLOCK__
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_DISPCHAR_ANALOG, "Analog Clock", "DisplayCharList");
#endif 
#ifdef __MMI_DIGITAL_CLOCK__
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_DISPCHAR_DIGITAL, "Digital Clock", "DisplayCharList");
#endif 
#ifdef __MMI_THEMES_V2_SUPPORT__
    ADD_APPLICATION_IMAGE2(IMG_ID_DISPCHAR_THEME_WALLPAPER, CUST_IMG_PATH "/MainLCD/IdleScreen/WallPaper/wall01.gif", "Image associated with screen saver.");
    ADD_APPLICATION_IMAGE2(IMG_ID_DISPCHAR_THEME_SCREENSAVER, CUST_IMG_PATH "/MainLCD/IdleScreen/WallPaper/wall01.gif", "Image associated with screen saver.");
#endif 
#ifdef __OTG_ENABLE__
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_DISPCHAR_INVALID_PATH, "Invalid Folder", "OTG doesn't support dispchar");
#endif 
#if 0
    if (!res_log)
    {
        res_log = fopen("PhnsetAutoGen.log", "w+");
    }

#ifndef LOW_COST_SUPPORT
    if (!AutoGenDispCharRes())
#endif
    {
    #if defined(__MMI_DISPCHAR_M3D__)
        int max_m3d = 3;
    #else 
        int max_m3d = 0;
    #endif 
    	int j = 0;

        fprintf(res_log, "Can't open the directory: %s\n", CUST_IMG_PATH "/DispChar");

        /* Generate Wallpaper Images */
    #ifndef __MMI_UI_TECHNO_IDLESCREEN_BAR__
    #ifdef __MMI_ANALOG_CLOCK__
        fprintf(
            res_log,
            "                ADD_APPLICATION_IMAGE3(IMG_ID_DISPCHAR_ANALOG_CLK=%d, path=%s)\n",
            IMG_ID_DISPCHAR_ANALOG_CLK,
            CUST_IMG_PATH "/MainLCD/IdleScreen/WallPaper/analog.gif");
        ADD_APPLICATION_IMAGE3(
            IMG_ID_DISPCHAR_ANALOG_CLK,
            CUST_IMG_PATH "/MainLCD/IdleScreen/WallPaper/analog.gif",
            "Image associated with Wallpaper.");
    #endif /* __MMI_ANALOG_CLOCK__ */ 

    #ifdef __MMI_DIGITAL_CLOCK__
        fprintf(
            res_log,
            "                ADD_APPLICATION_IMAGE3(IMG_ID_DISPCHAR_DIGITAL_CLK=%d, path=%s)\n",
            IMG_ID_DISPCHAR_DIGITAL_CLK,
            CUST_IMG_PATH "/MainLCD/IdleScreen/WallPaper/digital.gif");
        ADD_APPLICATION_IMAGE3(
            IMG_ID_DISPCHAR_DIGITAL_CLK,
            CUST_IMG_PATH "/MainLCD/IdleScreen/WallPaper/digital.gif",
            "Image associated with Wallpaper.");
    #endif /* __MMI_DIGITAL_CLOCK__ */ 
    #endif /* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */ 

 #ifdef __MMI_POWER_ON_OFF_DISPLAY__	
	fprintf(res_log, "                ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_ON_0=%d, path=%s)\n",IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1,CUST_IMG_PATH"/MainLCD/Active/poweronoff/POON.gif");
	ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1, CUST_IMG_PATH"/MainLCD/Active/poweronoff/POON.gif","Power ON image");
	fprintf(res_log, "                ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_OFF_0=%d, path=%s)\n",IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1,CUST_IMG_PATH"/MainLCD/Active/poweronoff/pooff.gif");
	ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1,CUST_IMG_PATH"/MainLCD/Active/poweronoff/pooff.gif","Power OFF image");
    #endif

    #if defined(__MMI_WALLPAPER_NUM__)
        for (i = 0; i < __MMI_WALLPAPER_NUM__; i++)
    #else
        for (i = 0; i < MAX_WALLPAPER_IMG - MAX_OTHER_WALLPAPER; i++)
    #endif
        {
            char gif_path[] = CUST_IMG_PATH "/MainLCD/IdleScreen/WallPaper/wall01.gif";
            char m3d_path[] = CUST_IMG_PATH "/MainLCD/Active/M3D/m3d01.m3d";

            char *path = NULL;
            int position = 0;
            int gif_position = sizeof(CUST_IMG_PATH "/MainLCD/IdleScreen/WallPaper/wall");
            int m3d_position = sizeof(CUST_IMG_PATH "/MainLCD/Active/M3D/m3d");
            
            if (i < (MAX_WALLPAPER_IMG - MAX_OTHER_WALLPAPER - max_m3d))
            {
                position = gif_position;

                path = &gif_path[0];
                path[position] = '1' + i;
            }
            else
            {
                position = m3d_position;
                path = &m3d_path[0];

            #if defined(__MMI_DISPCHAR_M3D_FISH__)
                path[position] = '1' + j;
            #elif defined(__MMI_DISPCHAR_M3D_PENGUIN__)
                path[position] = '4' + j;
            #elif defined(__MMI_DISPCHAR_M3D_BALL__)
                path[position] = '7' + j;
            #endif 
            	j++;
            }

            fprintf(res_log, "                ADD_APPLICATION_IMAGE3(i = %d, path=%s)\n", i, path);
            ADD_APPLICATION_IMAGE3((WALLPAPER_1 + i), path, "Image associated with wallpaer.");
            ADD_APPLICATION_IMAGE3((SCREENSAVER_1 + i), path, "Image associated with screensaver.");
        #ifdef __MMI_POWER_ON_OFF_DISPLAY__
            gif_path[gif_position] = '1' + i;
            //ADD_APPLICATION_IMAGE3((IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1 + i + 1), gif_path, "Image associated with poweron disp.");
            //ADD_APPLICATION_IMAGE3((IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1 + i + 1), gif_path, "Image associated with poweroff disp.");
        #endif /* __MMI_POWER_ON_OFF_DISPLAY__ */ 
        }


    #ifdef __MMI_SWFLASH__
        ADD_APPLICATION_STRING2(STR_ID_DISPCHAR_SWFLASH_0,"SWFlash 1","DisplayCharList");
        ADD_APPLICATION_STRING2(STR_ID_DISPCHAR_SWFLASH_1,"SWFlash 2","DisplayCharList");

        ADD_APPLICATION_IMAGE2(IMG_ID_DISPCHAR_SWFLASH_0,CUST_IMG_PATH"/MainLCD/SWFlash/IC_M01.CMP","Built-in SWFlash file 1");
        ADD_APPLICATION_IMAGE2(IMG_ID_DISPCHAR_SWFLASH_1,CUST_IMG_PATH"/MainLCD/SWFlash/IC_M02.CMP","Built-in SWFlash file 2");
    #endif /* __MMI_SWFLASH__ */
    
        /* Generate Sub Wallpaper images */
    #ifdef __MMI_SUB_WALLPAPER__
        ADD_APPLICATION_IMAGE2(
            IMG_ID_PHNSET_SUB_WP_0,
            CUST_IMG_PATH "/SubLCD/WallPaper/wall01.gif",
            "Image associated with wallpaper.");
        ADD_APPLICATION_IMAGE2(
            IMG_ID_PHNSET_SUB_WP_1,
            CUST_IMG_PATH "/SubLCD/WallPaper/wall02.gif",
            "Image associated with wallpaper.");
        ADD_APPLICATION_IMAGE2(
            IMG_ID_PHNSET_SUB_WP_2,
            CUST_IMG_PATH "/SubLCD/WallPaper/wall03.gif",
            "Image associated with wallpaper.");
        ADD_APPLICATION_IMAGE2(
            IMG_ID_PHNSET_SUB_WP_3,
            CUST_IMG_PATH "/SubLCD/WallPaper/wall04.gif",
            "Image associated with wallpaper.");
        ADD_APPLICATION_IMAGE2(
            IMG_ID_PHNSET_SUB_WP_4,
            CUST_IMG_PATH "/SubLCD/WallPaper/wall05.gif",
            "Image associated with wallpaper.");
        ADD_APPLICATION_IMAGE2(
            IMG_ID_PHNSET_SUB_WP_5,
            CUST_IMG_PATH "/SubLCD/WallPaper/wall06.gif",
            "Image associated with wallpaper.");
        ADD_APPLICATION_IMAGE2(
            IMG_ID_PHNSET_SUB_WP_6,
            CUST_IMG_PATH "/SubLCD/WallPaper/wall07.gif",
            "Image associated with wallpaper.");
        ADD_APPLICATION_IMAGE2(
            IMG_ID_PHNSET_SUB_WP_7,
            CUST_IMG_PATH "/SubLCD/WallPaper/wall08.gif",
            "Image associated with wallpaper.");
        ADD_APPLICATION_IMAGE2(
            IMG_ID_PHNSET_SUB_WP_8,
            CUST_IMG_PATH "/SubLCD/WallPaper/wall09.gif",
            "Image associated with wallpaper.");
        ADD_APPLICATION_IMAGE2(
            IMG_ID_PHNSET_SUB_WP_9,
            CUST_IMG_PATH "/SubLCD/WallPaper/wall10.gif",
            "Image associated with wallpaper.");
    #endif /* __MMI_SUB_WALLPAPER__ */ 
#if 0
    #if defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__) && defined(MJPG_SUPPORT)
        ADD_APPLICATION_MEDIA(
            IMG_ID_PHNSET_SS_VDO_START,
            CUST_IMG_PATH "/MainLCD/Active/Poweronoff/PWRONOFF.avi");
    #elif defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__) && defined(MP4_CODEC)
        ADD_APPLICATION_MEDIA(
            IMG_ID_PHNSET_SS_VDO_START,
            CUST_IMG_PATH "/MainLCD/Active/Poweronoff/PWRONOFF.mp4");
    #endif 
#endif
        /* Generate Power on/off display images */
    #ifndef __MMI_POWER_ON_OFF_DISPLAY__
    #ifdef LOW_COST_SUPPORT
        fprintf(res_log, "                ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_ON_ANIMATION_DEFAULT=%d, path=%s)\n",IMG_ID_PHNSET_ON_ANIMATION_DEFAULT, CUST_IMG_PATH"/MainLCD/Active/poweronoff/logo.bmp");
        ADD_APPLICATION_IMAGE3(
            IMG_ID_PHNSET_ON_ANIMATION_DEFAULT,
            CUST_IMG_PATH "/MainLCD/Active/poweronoff/logo.bmp",
            "Power ON image");
        fprintf(res_log, "                ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT=%d, path=%s)\n",IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT, CUST_IMG_PATH"/MainLCD/Active/poweronoff/logo.bmp");
        ADD_APPLICATION_IMAGE3(
            IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT,
            CUST_IMG_PATH "/MainLCD/Active/poweronoff/logo.bmp",
            "Power OFF image");
    #else /* LOW_COST_SUPPORT */
        ADD_APPLICATION_IMAGE3(
            IMG_ID_PHNSET_ON_ANIMATION_DEFAULT,
            CUST_IMG_PATH "/MainLCD/Active/poweronoff/POON.gif",
            "Power ON image");
        ADD_APPLICATION_IMAGE3(
            IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT,
            CUST_IMG_PATH "/MainLCD/Active/poweronoff/pooff.gif",
            "Power OFF image");
    #endif /* LOW_COST_SUPPORT */    
    #elif defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__) && defined(MJPG_SUPPORT)
	#if 0
        ADD_APPLICATION_MEDIA(
            IMG_ID_PHNSET_ON_VDO_0,
            CUST_IMG_PATH "/MainLCD/Active/Poweronoff/PWRONOFF.avi");
        ADD_APPLICATION_MEDIA(
            IMG_ID_PHNSET_OFF_VDO_0,
            CUST_IMG_PATH "/MainLCD/Active/Poweronoff/PWRONOFF.avi");
	#endif
    #elif defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__) && defined(MP4_CODEC)
        ADD_APPLICATION_MEDIA(
            IMG_ID_PHNSET_ON_VDO_0,
            CUST_IMG_PATH "/MainLCD/Active/Poweronoff/PWRONOFF.mp4");
        ADD_APPLICATION_MEDIA(
            IMG_ID_PHNSET_OFF_VDO_0,
            CUST_IMG_PATH "/MainLCD/Active/Poweronoff/PWRONOFF.mp4");
    #endif 
    }
#endif

}


/*****************************************************************************
 * FUNCTION
 *  PopulateBarredDialList
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateBarredDialList(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_MENUITEM((MENU8236_BDL_VIEW_OPTIONS_MAIN,
                              0, 3,
                              MENU8236_BDL_VIEW_OPTIONS_EDIT,
                              MENU8236_BDL_VIEW_OPTIONS_ADD,
                              MENU8236_BDL_VIEW_OPTIONS_DELETE,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_OPTIONS, 0));

    ADD_APPLICATION_MENUITEM((MENU8236_BDL_VIEW_OPTIONS_ADD,
                              MENU8236_BDL_VIEW_OPTIONS_MAIN, 2,
                              MENU_FDL_NAME,
                              MENU_FDL_NUMBER, SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_ADD, 0));

    ADD_APPLICATION_MENUITEM((MENU_FDL_NAME,
                              MENU8236_BDL_VIEW_OPTIONS_ADD, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTING_FDL_NAME, IMG_NAME));

    ADD_APPLICATION_MENUITEM((MENU_FDL_NUMBER,
                              MENU8236_BDL_VIEW_OPTIONS_ADD, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTING_FDL_NUMBER, IMG_MOBILE_NUMBER));

    ADD_APPLICATION_MENUITEM((MENU8236_BDL_VIEW_OPTIONS_EDIT,
                              MENU8236_BDL_VIEW_OPTIONS_MAIN, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_EDIT, 0));

    ADD_APPLICATION_MENUITEM((MENU8236_BDL_VIEW_OPTIONS_DELETE,
                              MENU8236_BDL_VIEW_OPTIONS_MAIN, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_DELETE, 0));

    ADD_APPLICATION_STRING2(STR_MENU8236_BDL_VIEW_OPTIONS_MAIN, "BDN List ", "BDL OPTION CAPTION");

    ADD_APPLICATION_STRING2(STR_SCR8079_BDL_VIEW_LIST_CAPTION, "BDN List", "BDL VIEW LIST CAPTION");

}


/*****************************************************************************
 * FUNCTION
 *  PopulateFixedDialList
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateFixedDialList(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_MENUITEM((MENU8234_FDL_VIEW_OPTIONS_MAIN,
                              0, 3,
                              MENU8234_FDL_VIEW_OPTIONS_EDIT,
                              MENU8234_FDL_VIEW_OPTIONS_ADD,
                              MENU8234_FDL_VIEW_OPTIONS_DELETE,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_OPTIONS, 0));

    ADD_APPLICATION_MENUITEM((MENU8234_FDL_VIEW_OPTIONS_ADD,
                              MENU8234_FDL_VIEW_OPTIONS_MAIN, 2,
                              MENU_FDL_NAME,
                              MENU_FDL_NUMBER, SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_ADD, 0));

    ADD_APPLICATION_MENUITEM((MENU_FDL_NAME,
                              MENU8234_FDL_VIEW_OPTIONS_ADD, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTING_FDL_NAME, IMG_NAME));

    ADD_APPLICATION_MENUITEM((MENU_FDL_NUMBER,
                              MENU8234_FDL_VIEW_OPTIONS_ADD, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTING_FDL_NUMBER, IMG_MOBILE_NUMBER));

    ADD_APPLICATION_MENUITEM((MENU8234_FDL_VIEW_OPTIONS_EDIT,
                              MENU8234_FDL_VIEW_OPTIONS_MAIN, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_EDIT, 0));

    ADD_APPLICATION_MENUITEM((MENU8234_FDL_VIEW_OPTIONS_DELETE,
                              MENU8234_FDL_VIEW_OPTIONS_MAIN, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_DELETE, 0));

    ADD_APPLICATION_STRING2(STR_MENU8234_FDL_VIEW_OPTIONS_MAIN, "FDN List ", "FDL MENU OPTION CAPTION");

    ADD_APPLICATION_STRING2(STR_SETTING_FDL_NAME, "Name", "NAME TEXT ");

    ADD_APPLICATION_STRING2(STR_SETTING_FDL_NUMBER, "Number", "NUMBER TEXT");

    ADD_APPLICATION_STRING2(STR_SETTING_FDL_EDIT_ENTRY, "Edit Entry", "EDIT ENTRY TEXT ");

    ADD_APPLICATION_STRING2(STR_SCR8054_CALL_FDL_CAPTION, "FDN List ", "FDL LIST CAPTION");

}


/*****************************************************************************
 * FUNCTION
 *  populateSecuritySetup
 * DESCRIPTION
 *  populating various resources
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void populateSecuritySetup(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	U8 i;
    ADD_APPLICATION_MENUITEM((MENU9101_SECURITY, MAIN_MENU_SETTINGS_MENUID,
						#ifdef __MMI_AUTO_KEYPAD_LOCK__
								1 +
						#endif
								5+(MMI_SIM_NUMBER - 1)*3,
								
								MULTI_MENU(MENU9169_SIM_LOCK),
								MULTI_MENU(MENU9167_FIXED_DIAL),
								MULTI_MENU(MENU9168_BARRED_DAIL),
								MENU9165_PHONE_LOCK,
						#ifdef __MMI_AUTO_KEYPAD_LOCK__
								MENU9178_AKPL_TIME,
						#endif
								MENU9170_CHANGE_PASSWORD,

                              SHOW, MOVEABLEACROSSPARENT | INSERTABLE | SHORTCUTABLE, DISP_LIST, STR_MENU9101_SECURITY,
                              IMG_SETTING_SECURITY));


    ADD_APPLICATION_MENUITEM((MENU9165_PHONE_LOCK, MENU9101_SECURITY, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_MENU9165_PHONE_LOCK, 0));

#ifdef __MMI_AUTO_KEYPAD_LOCK__
    ADD_APPLICATION_MENUITEM((MENU9178_AKPL_TIME, MENU9101_SECURITY, 5,
                              MENU9177_AKPL_OFF,
                              MENU9178_AKPL_TIME_5,
                              MENU9178_AKPL_TIME_30,
                              MENU9178_AKPL_TIME_60,
                              MENU9178_AKPL_TIME_2,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_MENU9166_AUTO_KEYPAD_LOCK, 0));

    ADD_APPLICATION_MENUITEM((MENU9177_AKPL_OFF, MENU9178_AKPL_TIME, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_MENU9177_AKPL_OFF, 0));

    ADD_APPLICATION_MENUITEM((MENU9178_AKPL_TIME_5, MENU9178_AKPL_TIME, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR9178_AKPL_TIME_5, 0));

    ADD_APPLICATION_MENUITEM((MENU9178_AKPL_TIME_30, MENU9178_AKPL_TIME, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR9178_AKPL_TIME_30, 0));

    ADD_APPLICATION_MENUITEM((MENU9178_AKPL_TIME_60, MENU9178_AKPL_TIME, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR9178_AKPL_TIME_60, 0));

    ADD_APPLICATION_MENUITEM((MENU9178_AKPL_TIME_2, MENU9178_AKPL_TIME, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR9178_AKPL_TIME_2, 2));
#endif /* __MMI_AUTO_KEYPAD_LOCK__ */ 

#if defined (__MMI_MULTI_SIM__)&& !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		ADD_APPLICATION_MENUITEM((MENU9167_FIXED_DIAL + i,MENU9101_SECURITY,2,
					MENU_SETTING_FDL_MODE,
					MENU_SETTING_FDL_LIST,
					SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_SIM1_FIXED_DAIL_CAPTION + i, 0));
		
		ADD_APPLICATION_MENUITEM((MENU9168_BARRED_DAIL + i ,MENU9101_SECURITY,2,
					MENU_SETTING_BDL_MODE,
					MENU_SETTING_BDL_LIST,
					SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_SIM1_BARRED_DIAL_CAPTION + i,0));

		 ADD_APPLICATION_MENUITEM((MENU9169_SIM_LOCK + i, MENU9101_SECURITY, 0,
					SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STRING_MTPNP_CARD1_PINLOCK + i, 0));
	}
#else
	ADD_APPLICATION_MENUITEM((MENU9167_FIXED_DIAL,MENU9101_SECURITY,2,
				MENU_SETTING_FDL_MODE,
				MENU_SETTING_FDL_LIST,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_MENU9167_FIXED_DIAL,0));

	ADD_APPLICATION_MENUITEM((MENU9168_BARRED_DAIL, MENU9101_SECURITY, 2,
                              MENU_SETTING_BDL_MODE,
                              MENU_SETTING_BDL_LIST,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_MENU9168_BARRED_DAIL, 0));
	
	ADD_APPLICATION_MENUITEM((MENU9169_SIM_LOCK, MENU9101_SECURITY, 0,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_MENU9169_SIM_LOCK, 0));
#endif

    ADD_APPLICATION_MENUITEM((MENU_SETTING_FDL_MODE, MENU9167_FIXED_DIAL, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTING_FDL_MODE, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_FDL_LIST, MENU9167_FIXED_DIAL, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTING_FDL_LIST, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_BDL_MODE, MENU9168_BARRED_DAIL, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTING_BDL_MODE, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_BDL_LIST, MENU9168_BARRED_DAIL, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTING_BDL_LIST, 0));

   

    ADD_APPLICATION_MENUITEM((MENU9173_CP_PHONE_LOCK, MENU9170_CHANGE_PASSWORD, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_MENU9173_CP_PHONE_LOCK, 0));

    ADD_APPLICATION_MENUITEM((MENU9175_CP_PIN2, MENU9170_CHANGE_PASSWORD, 0,
								SHOW, MOVEABLEWITHINPARENT, DISP_LIST, 
								#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
								STR_MENU9175_CP_SIM1_PIN2, 
								#else
								STR_MENU9175_CP_PIN2, 
								#endif
								0));
#ifdef __MMI_MULTI_SIM__
	ADD_APPLICATION_MENUITEM((MENU_ID_GSM_CHANGE_PIN_SELECT_SIM, 0,MMI_SIM_NUMBER,
								MULTI_MENU(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM1),                               
								MENU_ID_GSM_CHANGE_PIN_SELECT_SIM2,
								SHOW, MOVEABLEWITHINPARENT, DISP_LIST, 
								STR_GLOBAL_DIAL,0));    
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		ADD_APPLICATION_MENUITEM((MENU_ID_GSM_CHANGE_PIN_SELECT_SIM1 + i,
									MENU_ID_GSM_CHANGE_PIN_SELECT_SIM, 0,
									SHOW,   MOVEABLEWITHINPARENT,   DISP_LIST,
									STR_ID_UCM_SIM1_VOICE_CALL + i,                                
									0));
	}
#endif

/*****Strings*****/
    ADD_APPLICATION_STRING2(STR_MENU9101_SECURITY, "Security Setup", "Settings Security.");
    ADD_APPLICATION_STRING2(STR_SCR9002_SECURITY_CAPTION, "Security Setup", "Security Setup Caption.");
    ADD_APPLICATION_STRING2(STR_MENU9165_PHONE_LOCK, "Phone Lock", "Phone Lock Menu.");
#ifdef __MMI_AUTO_KEYPAD_LOCK__
    ADD_APPLICATION_STRING2(STR_MENU9166_AUTO_KEYPAD_LOCK, "Auto Keypad Lock", "Auto KeyPad Lock Menu.");
#endif 
    ADD_APPLICATION_STRING2(STR_MENU9167_FIXED_DIAL, "Fixed Dial", "Fixed Dial Menu.");
    ADD_APPLICATION_STRING2(STR_MENU9168_BARRED_DAIL, "Barred Dial", "Barred Dial Menu.");
	ADD_APPLICATION_STRING2(STR_MENU9170_CHANGE_PASSWORD,"Change Password","Change Password Menu.");
	ADD_APPLICATION_STRING2(STR_MENU9173_CP_PHONE_LOCK,"Phone Lock Password","Change Phone Lock Menu.");
	
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	ADD_APPLICATION_STRING2(STR_MENU9174_CP_SIM1_PIN1,"SIM1 PIN","Change SIM Lock Menu.");
	ADD_APPLICATION_STRING2(STR_MENU9175_CP_SIM1_PIN2,"SIM1 PIN2","Change PIN2 Menu.");
#else
	ADD_APPLICATION_STRING2(STR_MENU9174_CP_PIN1,"PIN","Change SIM Lock Menu.");
	ADD_APPLICATION_STRING2(STR_MENU9175_CP_PIN2,"PIN2","Change PIN2 Menu.");
	ADD_APPLICATION_STRING2(STR_MENU9169_SIM_LOCK,"SIM Lock","PIN Lock Menu.");
#endif

#ifdef __MMI_AUTO_KEYPAD_LOCK__
    ADD_APPLICATION_STRING2(STR_SCR9022_AUTO_KEYPAD_LOCK_CAPTION, "Auto Keypad Lock", "Auto Keypad Lock Menu.");
#endif 

    ADD_APPLICATION_STRING2(STR_SCR9023_FIXED_DAIL_CAPTION, "Fixed Dial", "Fixed Dial Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9024_BARRED_DIAL_CAPTION, "Barred Dial", "Barred Dial Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9025_SIM_LOCK_CAPTION, "SIM Lock", "SIM Lock Screen Caption.");
//    ADD_APPLICATION_STRING2(STR_DIFF_PIN, "Different PIN codes!", "Pin codes Different String.");
    ADD_APPLICATION_STRING2(STR_ID_PIN_NOT_SUPPORT, "PIN not support!", "PIN not support!");
    ADD_APPLICATION_STRING2(STR_ID_PIN2_NOT_SUPPORT, "PIN2 not support!", "PIN2 not support!");
    //ADD_APPLICATION_STRING2(STR_ID_SECSET_UNSPECIFIED_ERROR, "Please try later!", "Please try later!");

#ifdef __MMI_AUTO_KEYPAD_LOCK__
    ADD_APPLICATION_STRING2(STR_MENU9177_AKPL_OFF, "None", "Off Option For AutoKeyPadLock Option.");

    ADD_APPLICATION_STRING2(STR9178_AKPL_TIME_5, "5 sec.", "Time 5 Sec For Auto KeyPad Lock.");
    ADD_APPLICATION_STRING2(STR9178_AKPL_TIME_30, "30 sec.", "Time 30 Sec For Auto KeyPad Lock.");
    ADD_APPLICATION_STRING2(STR9178_AKPL_TIME_60, "1 min.", "Time 60 sec For Auto KeyPad Lock.");
    ADD_APPLICATION_STRING2(STR9178_AKPL_TIME_2, "5 min.", "Time 5 Min For Auto KeyPad Lock.");
#endif /* __MMI_AUTO_KEYPAD_LOCK__ */ 

    ADD_APPLICATION_STRING2(STR_ENABLE_PIN, "PIN Lock Must Be Enabled!", "Enable PIN Lock String.");
    ADD_APPLICATION_STRING2(STR_SETTING_PUK, "Enter PUK", "Enter PUK Screen String.");
    ADD_APPLICATION_STRING2(STR_SETTING_FDL_LIST, "Fixed Dial List", "Fixed Dial List option.");
    ADD_APPLICATION_STRING2(STR_SETTING_FDL_MODE, "Mode", "Fixed Dial Mode Option.");
    ADD_APPLICATION_STRING2(STR_NO_FDL, "SIM Card Not Support!", "SIM Card Not Support Message.");
    ADD_APPLICATION_STRING2(STR_SETTING_BDL_LIST, "Barred Dial List", "Barred Dial List option.");
    ADD_APPLICATION_STRING2(STR_SETTING_BDL_MODE, "Mode", "Barred Dial Mode Option.");
    ADD_APPLICATION_STRING2(STR_SETTING_SOS, "SOS", "RSK associated with Drop emergency Call.");
    ADD_APPLICATION_STRING2(STR_SETTING_SIM_LOCK, "SIM Locked!", "SIM Locked.");
    ADD_APPLICATION_STRING2(STR_SETTING_SIM_UNLOCK, "SIM Unlocked!", "SIM UnLocked.");
    ADD_APPLICATION_STRING2(STR_SETTING_PHONE_LOCK, "Phone Locked!", "Phone Locked.");
    ADD_APPLICATION_STRING2(STR_SETTING_PHONE_UNLOCK, "Phone Unlocked!", "Phone UnLocked.");
    ADD_APPLICATION_STRING2(STR_SETTING_OLD_PIN, "Enter Old PIN", "Enter Old PIN.");
    ADD_APPLICATION_STRING2(STR_SETTING_NEW_PIN, "New PIN", "New PIN.");
    ADD_APPLICATION_STRING2(STR_SETTING_OLD_PIN2, "Enter Old PIN2", "Enter Old PIN2.");
    ADD_APPLICATION_STRING2(STR_SETTING_NEW_PIN2, "New PIN2", "New PIN2.");
    ADD_APPLICATION_STRING2(STR_DIFF_PIN, "Different PIN codes!", "Different PIN Codes Message.");	
    ADD_APPLICATION_STRING2(STR_DIFF_PIN2, "Different PIN2 codes!", "Different PIN2 Codes Message.");
    ADD_APPLICATION_STRING2(STR_DIFF_PHONE, "Different Passwords!", "Different PhoneLock Message.");

    ADD_APPLICATION_STRING2(STR_WRONG_PIN, "Wrong PIN Code!", "Wrong PIN Code Message.");
    ADD_APPLICATION_STRING2(STR_PIN_CHANGE, "PIN changed successfully!", "PIN changed successfully Message.");
    ADD_APPLICATION_STRING2(STR_WRONG_PIN2, "Wrong PIN2 Code!", "Wrong PIN2 Code Message.");
    ADD_APPLICATION_STRING2(STR_PIN2_CHANGE, "PIN2 changed successfully!", "PIN2 changed successfully Message.");
    ADD_APPLICATION_STRING2(STR_PHONE_CHANGE, "Password changed!", "Password changed successfully Message.");
    ADD_APPLICATION_STRING2(STR_SETTING_ENTER_PUK2, "Enter PUK2", "Enter PUK2 Message.");

    ADD_APPLICATION_STRING2(STR_SETTING_ENTER_NEW_PIN2, "Enter New PIN2", "Enter New PIN2 Message.");

    ADD_APPLICATION_STRING2(STR_SCR9026_CHANGE_PASSWORD_CAPTION, "Change Password", "Change Password Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9027_PIN2_CAPTION, "Enter PIN2", "Enter PIN2 Caption String.");

    ADD_APPLICATION_STRING2(STR_SCR9028_PIN1_CAPTION, "Enter PIN", "Enter PIN1 Caption String.");

    ADD_APPLICATION_STRING2(STR_SCR9029_CURRENT_PW_CAPTION, "Enter Old Password", "Enter Old Password String.");
    ADD_APPLICATION_STRING2(STR_NEW_PIN1, "Enter New PIN", "Enter New PIN1 String.");
    ADD_APPLICATION_STRING2(STR_SCR9030_NEW_PW_CAPTION, "New Password", "Enter New Password String.");

    ADD_APPLICATION_STRING2(STR_SCR9031_CONFIRM_PW_CAPTION, "Enter Again", "Enter Confirm Password String.");
    ADD_APPLICATION_STRING2(STR_SETTING_WRONG_PUK2, "Wrong PUK2", "Wrong PUK2 String.");
    ADD_APPLICATION_STRING2(STR_SETTING_WRONG_PUK, "Wrong PUK", "Wrong PUK String.");
    ADD_APPLICATION_STRING2(STR_SETTING_PIN2_BLOCK_TITLE, "PIN2 Blocked", "PIN2 Blocked Title String.");
    ADD_APPLICATION_STRING2(
        STR_SETTING_PIN2_BLOCK_MSG,
        "Please Contact Service Provider",
        "PIN2 Blocked Message String.");

    ADD_APPLICATION_STRING2(STR_PASS_TRIALS_REMAIN, "Trials Remain: ", "Password trials remain message.");

    ADD_APPLICATION_STRING2(STR_FDL_IS_ON, "FDN Enabled", "FDN Enable Message.");
    ADD_APPLICATION_STRING2(STR_FDL_IS_OFF, "FDN Disabled", "FDN Disabled Message.");
    ADD_APPLICATION_STRING2(STR_BDL_IS_ON, "BDL Enabled", "BDL Enabled Message.");
    ADD_APPLICATION_STRING2(STR_BDL_IS_OFF, "BDL Disabled", "BDL Disabled Message.");
    ADD_APPLICATION_STRING2(STR_SETTING_PIN_BLK, "Wrong PIN! PIN is blocked.", "Wrong PIN! PIN is blocked Message.");
    ADD_APPLICATION_STRING2(
        STR_SETTING_PIN2_BLK,
        "Wrong PIN2! PIN2 is blocked.",
        "Wrong PIN2! PIN2 is blocked Message.");
    ADD_APPLICATION_STRING2(STR_SETTING_PIN2_BLK2, "PIN2 is blocked.", "PIN2 is blocked Message.");
    ADD_APPLICATION_STRING2(
        STR_INVALID_PINPUKPLOCK_LENGTH,
        "Invalid Length!",
        "Running Text on Incorrect PIN/PUK length Message.");
    ADD_APPLICATION_STRING2(STR_SETTING_STORAGE_FULL, "Storage Full!", "Sotrage Full Message .");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_SECURITY,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_SECUR.PBM",
        "Setting Submenu image for Security Setup.");

    PopulateFixedDialList();
    PopulateBarredDialList();

}


#ifdef __MMI_TTS_FEATURES__
/*****************************************************************************
 * FUNCTION
 *  PopulateTTSPlayerResData
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateTTSPlayerResData(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("File: [%s]  Line: [%d] <<PopulateTTSPlayerResData.>\n", __FILE__, __LINE__));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_TTS_PLAYER, MAIN_MENU_SETTINGS_MENUID,                                    
				   6,
				   MENU_ID_TTS_READ_MSG,
				   MENU_ID_TTS_READ_MENU,
				   MENU_ID_TTS_READ_CALL,
				   MENU_ID_TTS_ALARM_SETTING,
				   MENU_ID_TTS_SETTING,
				   MENU_ID_TTS_HELP,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_SETTING_TTS_PLAYER,
                               NULL));
	
    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_READ_MSG, MENU_SETTING_TTS_PLAYER,                                    
				   2,
				   MENU_ID_TTS_READ_MSG_AUTO,
				   MENU_ID_TTS_READ_MSG_CONTROL,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_SETTING_TTS_READ_MSG,
                               NULL));



    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_READ_MSG_AUTO, MENU_ID_TTS_READ_MSG,                                    
		                 0,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_TTS_READ_MSG_AUTO,
                               NULL));
    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_READ_MSG_CONTROL, MENU_ID_TTS_READ_MSG,                                    
		                 0,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_TTS_READ_MSG_CONTROL,
                               NULL));

	//menu read menu
    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_READ_MENU, MENU_SETTING_TTS_PLAYER,                                    
				   2,
				   MENU_ID_TTS_READ_MENU_OPEN,
				   MENU_ID_TTS_READ_MENU_CLOSE,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_SETTING_TTS_READ_MENU,
                               NULL));

    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_READ_MENU_OPEN, MENU_ID_TTS_READ_MENU,                                    
		                 0,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_TTS_READ_MENU_OPEN,
                               NULL));
    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_READ_MENU_CLOSE, MENU_ID_TTS_READ_MENU,                                    
		                 0,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_TTS_READ_MENU_CLOSE,
                               NULL));

	//menu read call
    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_READ_CALL, MENU_SETTING_TTS_PLAYER,                                    
				   2,
				   MENU_ID_TTS_READ_CALL_OPEN,
				   MENU_ID_TTS_READ_CALL_CLOSE,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_SETTING_TTS_READ_CALL,
                               NULL));
	    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_READ_CALL_OPEN, MENU_ID_TTS_READ_CALL,                                    
		                 0,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_TTS_READ_CALL_OPEN,
                               NULL));
    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_READ_CALL_CLOSE, MENU_ID_TTS_READ_CALL,                                    
		                 0,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_TTS_READ_CALL_CLOSE,
                               NULL));

	//menu tts alarm setting
    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_ALARM_SETTING, MENU_SETTING_TTS_PLAYER,                                    
				   0,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_SETTING_TTS_ALARM_SETTING,
                               NULL));
        
	//menu tts setting
    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_SETTING, MENU_SETTING_TTS_PLAYER,                                    
				   0,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_SETTING_TTS_SETTING,
                               NULL));
	//menu help
    ADD_APPLICATION_MENUITEM((MENU_ID_TTS_HELP, MENU_SETTING_TTS_PLAYER,                                    
				   0,
                               SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_ID_SETTING_TTS_HELP,
                               NULL));


    /* add string resources */
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TTS_PLAYER, "Sinovoice", "Menu item string for TTS setting");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TTS_READ_MSG, "Voice Sms Read", "Menu item string for read msg");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TTS_READ_MENU, "Voice Menu Read", "Menu item string for read menu");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TTS_READ_CALL, "Voice CALL Read", "Menu item string for read call");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TTS_ALARM_SETTING, "Voice Alarm Setting", "Menu item string for tts alarm");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TTS_SETTING, "Voice Effect Setting", "Menu item string for tts setting");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TTS_HELP, "Help", "Menu item string for help");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TTS_HELP_DESCRIPTION, "Help Description", "Help Description");
	
    ADD_APPLICATION_STRING2(STR_ID_TTS_READ_MSG_AUTO, "Automatic", "Menu item string for auto");
    ADD_APPLICATION_STRING2(STR_ID_TTS_READ_MSG_CONTROL, "Manual", "Menu item string for control");
	
    ADD_APPLICATION_STRING2(STR_ID_TTS_READ_MENU_OPEN, "On", "Menu item string for open");
    ADD_APPLICATION_STRING2(STR_ID_TTS_READ_MENU_CLOSE, "Off", "Menu item string for close");

    ADD_APPLICATION_STRING2(STR_ID_TTS_READ_CALL_OPEN, "On", "Menu item string for open");
    ADD_APPLICATION_STRING2(STR_ID_TTS_READ_CALL_CLOSE, "Off", "Menu item string for close");

    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_EVERY_HOUR, "Read Every Hour", "Menu item string for read every hour");
    ADD_APPLICATION_STRING2(STR_ID_TTS_ALARM_READ_DATE, "Read Date", "Menu item string for read date");
    ADD_APPLICATION_STRING2(STR_ID_TTS_ALARM_READ_WEEK, "Read Week", "Menu item string for read week");

    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_OPEN, "Open", "Menu item string for alarm open");
    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_CLOSE, "Close", "Menu item string for alarm close");
	
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_VOLUME, "Volume", "Menu item string for volume");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_SPEED, "Read Speed", "Menu item string for speed");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_EFFECT, "Sound Effect", "Menu item string for effect");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_STYLE, "Voice Style", "Menu item string for style");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_TUNE, "Voice Tune", "Menu item string for tone");
	
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_VOLUME_1, "Volume 1", "Menu item string for volume 1");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_VOLUME_2, "Volume 2", "Menu item string for volume 2");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_VOLUME_3, "Volume 3", "Menu item string for volume 3");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_VOLUME_4, "Volume 4", "Menu item string for volume 4");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_VOLUME_5, "Volume 5", "Menu item string for volume 5");

    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_SPEED_FAST, "Fast", "Menu item string for speed fast");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_SPEED_NOR, "Normal", "Menu item string for speed normal");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_SPEED_SLOW, "Slow", "Menu item string for speed slow");

    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_EFFECT_BASE, "Off", "Menu item string for effect off");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_EFFECT_CHORUS, "Chorus", "Menu item string for effect chorus");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_EFFECT_ECHO, "Echo", "Menu item string for effect echo");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_EFFECT_NEARFAR, "Near Far", "Menu item string for effect near far");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_EFFECT_REVERB, "Reverb", "Menu item string for effect reverb");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_EFFECT_ROBOT, "Robot", "Menu item string for effect robot");

    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_STYLE_CLEAR, "Clear", "Menu item string for style clear");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_STYLE_NORMAL, "Normal", "Menu item string for style normal");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_STYLE_PLAIN, "Plain", "Menu item string for style plain");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_STYLE_VIVID, "Vivid", "Menu item string for style vivid");

    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_TUNE_DEEP, "Deep", "Menu item string for tune deep");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_TUNE_NORMAL, "Normal", "Menu item string for tune normal");
    ADD_APPLICATION_STRING2(STR_ID_TTS_SETTING_TUNE_STRIDENT, "Strident", "Menu item string for tune strident");

    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_TODAY, "Today is", "Menu item string for tune strident");
    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_NOW, "Now is", "Menu item string for tune strident");

    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_AM, "AM", "Menu item string for tune strident");
    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_PM, "PM", "Menu item string for tune strident");

    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_YEAR, "Year", "Menu item string for tune strident");
    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_MONTH, "Month", "Menu item string for tune strident");
    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_DAY, "Day", "Menu item string for tune strident");

    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_HOUR, "Hour", "Menu item string for tune strident");
    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_MINTUE, "Mintue", "Menu item string for tune strident");
    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_ZHENG, "Zheng", "Menu item string for tune strident");
    ADD_APPLICATION_STRING2(STR_ID_TTS_AlARM_WEEK, "Week", "Menu item string for tune strident");

    ADD_APPLICATION_STRING2(STR_ID_TTS_CALL_CONTENT, "Incoming call", "Menu item string for tune strident");
	
}


#endif


//////////////////////////////////////////////////////////////////////////////////

void PopulateSettingMenu(void)
{
	U8 i;

#if defined (__MMI_MAINLCD_128X160__)||defined( __MMI_MAINLCD_128X128__)
ADD_APPLICATION_MENUITEM((MAIN_MENU_SETTINGS_MENUID,IDLE_SCREEN_MENU_ID,
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
		1+
#endif
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
		1+
#endif
	#ifdef __MMI_TTS_FEATURES__
	  1+
	  #endif
	  
#ifdef __MMI_SOUND_EFFECT__
		1+
#endif
		6,
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
		MENU_SETTING_CALIBRATION,
#endif
		MAIN_MENU_PROFILES_MENUID, 
		MENU9102_INITIAL_SETUP,
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
		MENU_MTPNP_DM_SETTING,	
		MENU_MTPNP_DM_NETWORK_SETUP,
#else
		MENU9185_NETWORK_SETUP,
#endif
		MENU9101_SECURITY,
	#ifdef __MMI_TTS_FEATURES__
		    MENU_SETTING_TTS_PLAYER,
       #endif
		
#ifdef __MMI_SOUND_EFFECT__
		MENU_SETTING_SOUND_EFFECT,
#endif
		MENU8237_SCR8093_MNGCALL_MENU_MAIN,
		MENU_SETTING_RESTORE,
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,1,MAIN_MENU_SETTINGS_TEXT,
		MAIN_MENU_SETTINGS_ICON));
#elif defined __MMI_MAINLCD_176X220__
ADD_APPLICATION_MENUITEM((MAIN_MENU_SETTINGS_MENUID,IDLE_SCREEN_MENU_ID,
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
		1+
#endif
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
		1+
#endif
	#ifdef __MMI_TTS_FEATURES__
         1+
	#endif

#ifdef __MMI_SOUND_EFFECT__
		1+
#endif
		5,
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
		MENU_SETTING_CALIBRATION,
#endif
	//	MAIN_MENU_PROFILES_MENUID, 
		MENU9102_INITIAL_SETUP,
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
		MENU_MTPNP_DM_SETTING,	
		MENU_MTPNP_DM_NETWORK_SETUP,
#else
		MENU9185_NETWORK_SETUP,
#endif
		MENU9101_SECURITY,
		#ifdef __MMI_TTS_FEATURES__
		    MENU_SETTING_TTS_PLAYER,
       #endif
	
#ifdef __MMI_SOUND_EFFECT__
		MENU_SETTING_SOUND_EFFECT,
#endif
		MENU8237_SCR8093_MNGCALL_MENU_MAIN,
		MENU_SETTING_RESTORE,
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,1,MAIN_MENU_SETTINGS_TEXT,
		MAIN_MENU_SETTINGS_ICON));
#elif defined __MMI_MAINLCD_220X176__
ADD_APPLICATION_MENUITEM((MAIN_MENU_SETTINGS_MENUID,IDLE_SCREEN_MENU_ID,
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
		1+
#endif
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
		1+
#endif
#ifdef PRODUCT_QPAD
		1+
#endif
		5,
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
		MENU_SETTING_CALIBRATION,
#endif
#ifdef PRODUCT_QPAD
		MAIN_MENU_PROFILES_MENUID, 
#endif
		MENU9102_INITIAL_SETUP,
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
		MENU_MTPNP_DM_SETTING,	
		MENU_MTPNP_DM_NETWORK_SETUP,
#else
		MENU9185_NETWORK_SETUP,
#endif
		MENU9101_SECURITY,
		MENU8237_SCR8093_MNGCALL_MENU_MAIN,
		MENU_SETTING_RESTORE,
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,1,MAIN_MENU_SETTINGS_TEXT,
		MAIN_MENU_SETTINGS_ICON));
#elif defined __MMI_MAINLCD_240X320__
ADD_APPLICATION_MENUITEM((MAIN_MENU_SETTINGS_MENUID,IDLE_SCREEN_MENU_ID,
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
		1+
#endif
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
		1+
#endif
#ifdef __MMI_TTS_FEATURES__
               1+ 
#endif
#ifdef __MMI_SOUND_EFFECT__
		1+
#endif
		5,
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
		MENU_SETTING_CALIBRATION,
#endif
		MENU9102_INITIAL_SETUP,
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
		MENU_MTPNP_DM_SETTING,	
		MENU_MTPNP_DM_NETWORK_SETUP,
#else
		MENU9185_NETWORK_SETUP,
#endif
		MENU9101_SECURITY,
	#ifdef __MMI_TTS_FEATURES__
		    MENU_SETTING_TTS_PLAYER,
       #endif
		
#ifdef __MMI_SOUND_EFFECT__
		MENU_SETTING_SOUND_EFFECT,
#endif
		MENU8237_SCR8093_MNGCALL_MENU_MAIN,
		MENU_SETTING_RESTORE,
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,1,MAIN_MENU_SETTINGS_TEXT,
		MAIN_MENU_SETTINGS_ICON));
#endif
	
	ADD_APPLICATION_MENUITEM((MAIN_MENU_SETTINGS_MENUID_NO_SIM,MENU_SIM_OPTION_ID,
	#ifdef __MMI_TTS_FEATURES__
		    1+
       #endif

#ifdef __MMI_SOUND_EFFECT__
#ifndef PRODUCT_QPAD
			1 + 
#endif
#endif

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
			1 + 
#endif
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
			1 + 
#endif
#if !defined __MMI_MAINLCD_240X320__
			4,
#else
			3,
#endif			
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
			MENU_SETTING_CALIBRATION,
#endif
#if !defined __MMI_MAINLCD_240X320__
			MAIN_MENU_PROFILES_MENUID,
#endif			
			MENU9102_INITIAL_SETUP_NO_SIM,
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
			MENU_MTPNP_DM_SETTING,
#endif
			MENU9101_SECURITY_NO_SIM,
			MENU_SETTING_RESTORE,
	#ifdef __MMI_TTS_FEATURES__
		    MENU_SETTING_TTS_PLAYER,
       #endif
			
#ifdef __MMI_SOUND_EFFECT__
#ifndef PRODUCT_QPAD
			MENU_SETTING_SOUND_EFFECT,
#endif
#endif
			SHOW,MOVEABLEWITHINPARENT|INSERTABLE,1,MAIN_MENU_SETTINGS_TEXT,
			MAIN_MENU_SETTINGS_ICON));

#ifdef PRODUCT_QPAD
	/*Profile*/
	ADD_APPLICATION_MENUITEM((MAIN_MENU_PROFILES_MENUID,MAIN_MENU_SETTINGS_MENUID,
	#ifdef __MMI_BT_PROFILE__
	6,
	#else
	5,
	#endif
	MAIN_MENU_PROFILES_GENERAL,
	MAIN_MENU_PROFILES_MEETING,
	MAIN_MENU_PROFILES_OUTDOOR,
	MAIN_MENU_PROFILES_INDOOR,
	MAIN_MENU_PROFILES_HEADSET,
	#ifdef __MMI_BT_PROFILE__
	MAIN_MENU_PROFILES_BT,
	#endif
	0,MOVEABLEACROSSPARENT | SHORTCUTABLE,DISP_LIST,MAIN_MENU_PROFILES_TEXT,NULL));
#endif
		//xiaoke 08.12.31 
#ifdef __DOUBLE_SIMCARD_211__  
ADD_APPLICATION_MENUITEM((MAIN_MENU_SETTINGS_DOUBLE_SIMCARD_ID,MAIN_MENU_SETTINGS_MENUID,2,
		MENU_SETTINGS_DOUBLE_SIMCARD_SIMCARD1_ID,
		MENU_SETTINGS_DOUBLE_SIMCARD_SIMCARD2_ID,
		0,MOVEABLEACROSSPARENT | SHORTCUTABLE,1,STR_DOUBLE_SIMCARD_OPTION,IMG_ID_DOUBLE_SIMCARD_ICON));
	
ADD_APPLICATION_MENUITEM((MENU_SETTINGS_DOUBLE_SIMCARD_SIMCARD1_ID,MAIN_MENU_SETTINGS_DOUBLE_SIMCARD_ID,0,
		0,MOVEABLEACROSSPARENT | SHORTCUTABLE,1,STR_DOUBLE_SIMCARD1_OPTION,IMG_ID_CAMERA_APP_ICON));
	
ADD_APPLICATION_MENUITEM((MENU_SETTINGS_DOUBLE_SIMCARD_SIMCARD2_ID,MAIN_MENU_SETTINGS_DOUBLE_SIMCARD_ID,0,
		0,MOVEABLEACROSSPARENT | SHORTCUTABLE,1,STR_DOUBLE_SIMCARD2_OPTION,IMG_ID_CAMERA_APP_ICON));
#endif
		//xiaoke 08.12.31 

ADD_APPLICATION_MENUITEM((MENU_CALL_SETTING_SOUND_WARNING,MENU8237_SCR8093_MNGCALL_MENU_MAIN,0,
				SHOW,NONMOVEABLE, DISP_LIST  ,STR_SETTING_SOUND_WARNING,NULL));
ADD_APPLICATION_MENUITEM((MENU_SETTING_BLACK_LIST,MENU8237_SCR8093_MNGCALL_MENU_MAIN,3,
				MENU_SETTING_BLACK_LIST_MODE,
				MENU_SETTING_BLACK_LIST_NUM,
				CM_BL_AND_RL_STATUS,	
				SHOW, NONMOVEABLE, DISP_LIST,STR_SETTING_BLACK_LIST,0));
	
ADD_APPLICATION_MENUITEM((MENU_BARRING_MAIN,MENU8237_SCR8093_MNGCALL_MENU_MAIN,2,
				MENU_SETTING_DEF_ENCODING_TYPE,
				MENU_SETTING_DEF_ENCODING_BIG2,
				SHOW,NONMOVEABLE, DISP_LIST ,STR_SETTING_BLACK_LIST,NULL));
ADD_APPLICATION_MENUITEM((MENU_SETTING_LANGUAGE,MENU9102_INITIAL_SETUP,0,
				SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST,STR_MENU_LANGUAGE,0));
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
ADD_APPLICATION_MENUITEM((MENU_SETTING_CHANGE_WIN_WITH_DYNAMIC,MENU9102_INITIAL_SETUP,0,
				SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST,STR_ID_SETTING_CHANGE_WIN_STYLE,0));
#endif
ADD_APPLICATION_MENUITEM((MENU_SETTING_PREFERED_INPUT_METHOD,MENU9102_INITIAL_SETUP,0,
				SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST,STR_MENU_INPUT_METHOD,0));
	
ADD_APPLICATION_MENUITEM((MENU_SETTING_DEF_ENCODING_TYPE, MENU_BARRING_MAIN,0,
                SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST,STR_SETTING_BLACK_LIST_MODE,0));
        
ADD_APPLICATION_MENUITEM((MENU_SETTING_DEF_ENCODING_BIG2, MENU_BARRING_MAIN,0,
                SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST,STR_SETTING_BLACK_LIST_NUM,0));
//ADD_APPLICATION_MENUITEM((MENU3101_THEMES1,MENU9102_INITIAL_SETUP,0,
//				SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST,STR_MENU3101_THEMES,0));
ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_KEYS,MENU9102_INITIAL_SETUP, 5,
				MENU_SETTING_DEDICATED_UP_KEY,
				MENU_SETTING_DEDICATED_DOWN_KEY,
				MENU_SETTING_DEDICATED_LEFT_KEY,
				MENU_SETTING_DEDICATED_RIGHT_KEY,
				MENU_SETTING_DEF_ENCODING_UCS2,
				SHOW, NONMOVEABLE, DISP_LIST,STR_DEDICATED_KEY,0));
ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_UP_KEY,MENU_SETTING_DEDICATED_KEYS, 0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_DEDICATED_UP_KEY,0));
ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_DOWN_KEY,MENU_SETTING_DEDICATED_KEYS, 0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_DEDICATED_DOWN,0));
ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_LEFT_KEY,MENU_SETTING_DEDICATED_KEYS, 0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_DEDICATED_LEFT,0));
ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_RIGHT_KEY,MENU_SETTING_DEDICATED_KEYS, 0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_DEDICATED_RIGHT,0));
	
ADD_APPLICATION_MENUITEM((MENU_SETTING_DEF_ENCODING_UCS2, MENU_SETTING_DEDICATED_KEYS,0,
                	SHOW, NONMOVEABLE, DISP_LIST,STR_DEDICATED_OK,0));

ADD_APPLICATION_MENUITEM((MENU9101_SECURITY_NO_SIM,MAIN_MENU_SETTINGS_MENUID_NO_SIM,3,
//				MENU9169_SIM_LOCK,
				MENU9165_PHONE_LOCK,
				MENU9178_AKPL_TIME,
//				MENU9167_FIXED_DIAL,
				MENU9170_CHANGE_PASSWORD_NO_SIM,
//				MENU_SECURITY_SETTING_BLACK_LIST,
				SHOW, NONMOVEABLE, DISP_LIST,STR_MENU9101_SECURITY,IMG_SETTING_SECURITY));
ADD_APPLICATION_MENUITEM((MENU_SECURITY_SETTING_BLACK_LIST,MENU9101_SECURITY,0,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_MENU_ADD_TO_BLACKLIST,NULL));
ADD_APPLICATION_MENUITEM((MENU_SETTING_RESTORE,MAIN_MENU_SETTINGS_MENUID,0,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_MENU_RESTORE,IMG_SETTING_STORE));
#if 0
ADD_APPLICATION_MENUITEM((MENU_SETTINGS_ABOUT,MAIN_MENU_SETTINGS_MENUID,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_MENU_SETTINGS_ABOUT,0));
#endif
ADD_APPLICATION_MENUITEM((MENU_DISP_SETUP,MAIN_MENU_MSGCLUB_MENUID,
				6
				,
				MENU9141_TIME_AND_DATE,
				MENU_SETTING_SCHEDULE_POWER,
				MENU_SETTING_LANGUAGE,
//#if !defined (__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)
//				MENU3101_THEMES1,
//#endif				
				MENU_SETTING_PREFERED_INPUT_METHOD,
				MENU_IDLE_SCR_DISP,
				MENU9146_WELCOME_TEXT,
				SHOW, MOVEABLEACROSSPARENT, DISP_LIST,STR_MENU9102_INITIAL_SETUP,IMG_SETTING_PHONE));
//[221] Added by Ryh 2009/02/12								[BEGIN]
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
ADD_APPLICATION_MENUITEM((MENU_MTPNP_DM_SETTING,MAIN_MENU_SETTINGS_MENUID, 4,
				MENU_MTPNP_DM_DUALCARD,
				MENU_MTPNP_DM_CARD1,
				MENU_MTPNP_DM_CARD2,
				MENU_MTPNP_DM_FLIGHT,
				SHOW, MOVEABLEACROSSPARENT, DISP_LIST,STRING_MTPNP_DM_SETTING,0));
ADD_APPLICATION_MENUITEM((MENU_MTPNP_DM_DUALCARD,MENU_MTPNP_DM_SETTING, 0,
				SHOW, MOVEABLEACROSSPARENT, DISP_LIST,STRING_MTPNP_DM_DUALCARD,0));
ADD_APPLICATION_MENUITEM((MENU_MTPNP_DM_CARD1,MENU_MTPNP_DM_SETTING, 0,
				SHOW, MOVEABLEACROSSPARENT, DISP_LIST,STRING_MTPNP_DM_CARD1,0));
ADD_APPLICATION_MENUITEM((MENU_MTPNP_DM_CARD2,MENU_MTPNP_DM_SETTING, 0,
				SHOW, MOVEABLEACROSSPARENT, DISP_LIST,STRING_MTPNP_DM_CARD2,0));
ADD_APPLICATION_MENUITEM((MENU_MTPNP_DM_FLIGHT,MENU_MTPNP_DM_SETTING, 0,
				SHOW, MOVEABLEACROSSPARENT, DISP_LIST,STRING_MTPNP_DM_FLIGHT,0));
#endif /*defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)*/
//[221] Added by Ryh 2009/02/12								[END]
ADD_APPLICATION_MENUITEM((MENU9102_INITIAL_SETUP,MAIN_MENU_SETTINGS_MENUID, 
#ifdef __MMI_SET_DEF_ENCODING_TYPE__
                              1+
#endif 
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
                              1+
#endif 
#if !defined (__MMI_MAINLCD_176X220__) &&!defined (__MMI_MAINLCD_220X176__)
				1+
#endif
#if defined (__LED__) || defined (__MMI_RUNNING_LED__)	
            1+
#endif
#ifdef __MMI_TOUCH_SCREEN__
               1+
#endif
#if defined(MOTION_SENSOR_SUPPORT)
				1+
#endif
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
				1+
#endif

				7,
				MENU9141_TIME_AND_DATE,
				MENU_SETTING_SCHEDULE_POWER,
				MENU_SETTING_LANGUAGE,
//#if !defined (__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)
//				MENU3101_THEMES1,
//#endif				
#ifdef __MMI_SET_DEF_ENCODING_TYPE__
                            MENU_SETTING_DEF_ENCODING_TYPE,
#endif 
				
				MENU_SETTING_PREFERED_INPUT_METHOD,
				MENU_IDLE_SCR_DISP,
   				MENU9146_WELCOME_TEXT,	
				MENU_SETTING_SPEED_DIAL,
#if !defined (__MMI_MAINLCD_176X220__) &&!defined (__MMI_MAINLCD_220X176__)
				MAIN_MENU_SHORTCUTS_MENUID,
#endif
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
                              MENU_SETTING_DEDICATED_KEYS,
#endif 
#if defined (__LED__) || defined (__MMI_RUNNING_LED__)	
                MENU_SETTING_ENABLE_ROLL_LED,
#endif
#ifdef __MMI_TOUCH_SCREEN__
               MENU_SETTING_HANDWRITING,
#endif
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
	MENU_SETTING_CHANGE_WIN_WITH_DYNAMIC,
#endif
#if defined(MOTION_SENSOR_SUPPORT)
			  MENU_SETTING_MOTION_SENSOR,
#endif
				//MENU_SETTING_FLIGHT_MODE,
				SHOW, MOVEABLEACROSSPARENT, DISP_LIST,STR_MENU9102_INITIAL_SETUP,IMG_SETTING_PHONE));
ADD_APPLICATION_MENUITEM((MENU9102_INITIAL_SETUP_NO_SIM,MAIN_MENU_SETTINGS_MENUID_NO_SIM,
#if !defined (__MMI_MAINLCD_176X220__) &&!defined (__MMI_MAINLCD_220X176__)
				1+
#endif
				7,
				MENU9141_TIME_AND_DATE,
				MENU_SETTING_SCHEDULE_POWER,
				MENU_SETTING_LANGUAGE,
				//MENU3101_THEMES1,
				MENU_SETTING_PREFERED_INPUT_METHOD,
				MENU_IDLE_SCR_DISP,
   				MENU9146_WELCOME_TEXT,	
	//			MENU_SETTING_SPEED_DIAL,
#if !defined (__MMI_MAINLCD_176X220__) &&!defined (__MMI_MAINLCD_220X176__)
				MAIN_MENU_SHORTCUTS_MENUID,
#endif				
				MENU_SETTING_DEDICATED_KEYS,
	//			MENU_SETTING_FLIGHT_MODE,
				SHOW, MOVEABLEACROSSPARENT, DISP_LIST,STR_MENU9102_INITIAL_SETUP,IMG_SETTING_PHONE));

#if !defined (__MMI_MAINLCD_176X220__) &&!defined (__MMI_MAINLCD_220X176__)
ADD_APPLICATION_MENUITEM((MAIN_MENU_SHORTCUTS_MENUID,MENU9102_INITIAL_SETUP,0,
				SHOW,MOVEABLEWITHINPARENT,1,MAIN_MENU_SHORTCUTS_TEXT,IMG_SETTING_SHORTCUT));
#endif
ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_MENUID,0,3,
				EXTRA_SHORTCUTS_OPTION_ADD_MENUID,
				EXTRA_SHORTCUTS_OPTION_EDIT_MENUID,
				EXTRA_SHORTCUTS_OPTION_DELETE_MENUID,
				SHOW,MOVEABLEWITHINPARENT,1,STR_GLOBAL_OPTIONS,NULL));
ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_ADD_MENUID,EXTRA_SHORTCUTS_OPTION_MENUID,0,
				SHOW,MOVEABLEWITHINPARENT,1,STR_GLOBAL_ADD,NULL));
ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_EDIT_MENUID,EXTRA_SHORTCUTS_OPTION_MENUID,0,
				SHOW,MOVEABLEWITHINPARENT,1,STR_GLOBAL_EDIT,NULL));
ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_DELETE_MENUID,EXTRA_SHORTCUTS_OPTION_MENUID,0,
				SHOW,MOVEABLEWITHINPARENT,1,STR_GLOBAL_DELETE,NULL));

ADD_APPLICATION_MENUITEM((MENU9141_TIME_AND_DATE,MENU9102_INITIAL_SETUP,3,
                            MENU9142_SETHOMECITY,
				MENU9143_SETTIMEANDDATE,
				MENU9144_SETTIMEANDDATEFORMAT,
				SHOW, MOVEABLEACROSSPARENT, DISP_LIST,STR_MENU9141_TIME_AND_DATE,0));
ADD_APPLICATION_MENUITEM((MENU9146_WELCOME_TEXT,MENU9102_INITIAL_SETUP, 0,
				SHOW, MOVEABLEACROSSPARENT|SHORTCUTABLE, DISP_LIST,STR_MENU9146_WELCOME_TEXT,0));

ADD_APPLICATION_MENUITEM((MENU9170_CHANGE_PASSWORD,MENU9101_SECURITY,
				3 + 2*(MMI_SIM_NUMBER-1),
				MULTI_MENU(MENU9174_CP_PIN1),
				MULTI_MENU(MENU9175_CP_PIN2),
				MENU9173_CP_PHONE_LOCK,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_MENU9170_CHANGE_PASSWORD,0));

ADD_APPLICATION_MENUITEM((MENU9170_CHANGE_PASSWORD_NO_SIM,MENU9101_SECURITY_NO_SIM,1,
				MENU9173_CP_PHONE_LOCK,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_MENU9170_CHANGE_PASSWORD,0));
ADD_APPLICATION_MENUITEM((MENU9171_ON,MENU_SETTING_SCH_POW_SET1_POW_ON,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_GLOBAL_ON,0));
ADD_APPLICATION_MENUITEM((MENU9172_OFF,MENU_SETTING_SCH_POW_SET1_POW_ON,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_GLOBAL_OFF,0));
ADD_APPLICATION_MENUITEM((MENU9174_CP_PIN1,MENU9170_CHANGE_PASSWORD,0,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST,
				#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
				STR_MENU9174_CP_SIM1_PIN1,
				#else
				STR_MENU9174_CP_PIN1,
				#endif
				0));
				
#ifdef __MMI_MULTI_SIM__
for(i = 0; i < MMI_SIM_NUMBER-1; i++)
{
	ADD_APPLICATION_MENUITEM((MENU_MTPNP_CP_PIN1+i,MENU9170_CHANGE_PASSWORD,0,
					SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STRING_MTPNP_CP_PIN1+i,0));
	ADD_APPLICATION_MENUITEM((MENU_MTPNP_CP_PIN2+i,MENU9170_CHANGE_PASSWORD,0,
					SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STRING_MTPNP_CP_PIN2+i,0));	
}
#endif

ADD_APPLICATION_MENUITEM((MENU9147_OFF,SERVICES_DATA_CONNECT_GPRS_MENU_ID,1,
				MENU9173_CP_PHONE_LOCK,
				SHOW, NONMOVEABLE, DISP_LIST,STR_MENU9170_CHANGE_PASSWORD,0));

ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER,MENU_IDLE_SCR_DISP,2,
				MENU_SETTING_WALLPAPER_SYSTEM,
				MENU_SETTING_WALLPAPER_DOWNLOAD,
				SHOW, SHORTCUTABLE, DISP_LIST,STR_SETTING_WALLPAPER,0));
ADD_APPLICATION_MENUITEM((MENU_SETTING_WALLPAPER_SYSTEM,MENU_SETTING_WALLPAPER,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_WALLPAPER_SYSTEM,0));
ADD_APPLICATION_MENUITEM((MENU_SETTING_SS_SELECT,MENU_SETTING_SCR_SVR,2,
				MENU_SELECT_SS_SELECT_SYSTEM,
				MEUN_SELECT_SS_SELECT_DOWNLOAD,
				SHOW, NONMOVEABLE, DISP_LIST,STR_GLOBAL_SELECT,0));
ADD_APPLICATION_MENUITEM((MENU_SELECT_SS_SELECT_SYSTEM,MENU_SETTING_SS_SELECT,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_SCR_SCREENSAVER_SYSTEM,0));
ADD_APPLICATION_MENUITEM((MENU_SETTING_IP_NUMBER,MENU8237_SCR8093_MNGCALL_MENU_MAIN,0,
				HIDE,MOVEABLEWITHINPARENT, DEFAULT,STR_SETTING_IP_NUMBER,18560));
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
ADD_APPLICATION_MENUITEM((MENU_SETTING_CALIBRATION,MAIN_MENU_SETTINGS_MENUID,0,
				SHOW, SHORTCUTABLE,DISP_LIST,STR_CALIBRATION_SETTING,50054));
#endif
ADD_APPLICATION_MENUITEM((MENU_SETTING_SPEED_DIAL,MENU9102_INITIAL_SETUP,2,
				MENU_SETTING_SPEED_STATUS,
				MITEM113_PBOOK_SPEED_DIAL,
				SHOW,MOVEABLEACROSSPARENT|SHORTCUTABLE, DISP_LIST,STR_MENU_SETTING_SPEED_DIAL,0));
ADD_APPLICATION_MENUITEM((MENU_ID_SPEED_DIAL_OPERATION,0,2,
				MENU_ID_SPEED_DIAL_OP_EDIT,
				MENU_ID_SPEED_DIAL_OP_DEL,
				SHOW, NONMOVEABLE, DISP_LIST,STR_MENU_SETTING_SPEED_DIAL,0));
ADD_APPLICATION_MENUITEM((MENU_ID_SPEED_DIAL_OP_EDIT,MENU_ID_SPEED_DIAL_OPERATION,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_GLOBAL_EDIT,0));
ADD_APPLICATION_MENUITEM((MENU_ID_SPEED_DIAL_OP_DEL,MENU_ID_SPEED_DIAL_OPERATION,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_GLOBAL_DELETE,IMG_GLOBAL_L2));
ADD_APPLICATION_MENUITEM((MENU_SETTING_CALL_TIME_DISPLAY,MENU8237_SCR8093_MNGCALL_MENU_MAIN,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_MENU_SETTING_CALL_TIME_DISPLAY,NULL));
ADD_APPLICATION_MENUITEM((MENU_SETTING_MENU_TREE,MENU9102_INITIAL_SETUP,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_MENU_SETTING_MENU_TREE,0));
ADD_APPLICATION_MENUITEM((MENU9186_SELECTION_MODE_MANUAL,MENU9186_SELECTION_MODE,0,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_NETSET_MITEM9186_MANUAL,NULL));
ADD_APPLICATION_MENUITEM((MENU9186_SELECTION_MODE_AUTOMATIC,MENU9186_SELECTION_MODE,0,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_NETSET_MITEM9186_AUTOMATIC,NULL));
ADD_APPLICATION_MENUITEM((MENU_POWER_SAVING_ON,MENU_SETTING_POWER_SAVING,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_GLOBAL_ON,0));
ADD_APPLICATION_MENUITEM((MENU_POWER_SAVING_OFF,MENU_SETTING_POWER_SAVING,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_GLOBAL_OFF,0));
ADD_APPLICATION_MENUITEM((MENU3001_THEMES_LIST,0,3,
				MENU_THEMES_ACTIVATE,
				MENU3201_UPDATE_PERIOD,
				MENU3202_SET_ORDER,
				SHOW,NONMOVEABLE,1,STR_SCR3002_THEMES_OPTIONS_CAPTION,0));
ADD_APPLICATION_MENUITEM((MITEM113_PBOOK_SPEED_DIAL,MENU_SETTING_SPEED_DIAL,0,
				SHOW, NONMOVEABLE, DISP_LIST,STR_SCR_SPEED_DIAL_MAIN_CAPTION,IMG_GLOBAL_L2));
ADD_APPLICATION_MENUITEM((MENU9142_SETHOMECITY,MENU9141_TIME_AND_DATE,0,
				SHOW, (MOVEABLEWITHINPARENT | SHORTCUTABLE), DISP_LIST,STR_MENU9142_SETHOMECITY,0));
ADD_APPLICATION_MENUITEM((MENU9143_SETTIMEANDDATE,MENU9141_TIME_AND_DATE,0,
				SHOW, (MOVEABLEWITHINPARENT | SHORTCUTABLE), DISP_LIST,STR_ID_PHNSET_SET_DT,0));
ADD_APPLICATION_MENUITEM((MENU9144_SETTIMEANDDATEFORMAT,MENU9141_TIME_AND_DATE,0,
				SHOW, (MOVEABLEWITHINPARENT | SHORTCUTABLE), DISP_LIST,STR_MENU9144_SETTIMEANDDATEFORMAT,0));
	//To Be modify B
/*	ADD_APPLICATION_MENUITEM((J2ME_MENU_FULLSCREEN_EDITOR_OPTION,0,2,
				J2ME_MENU_FULLSCREEN_EDITOR_OPTION_DONE,
				J2ME_MENU_FULLSCREEN_EDITOR_OPTION_CANCEL,
				SHOW,NONMOVEABLE,DISP_LIST,STR_SIM_SWITCH_MAIN,50062));*/ /* Kecx delete on 20080625 */
ADD_APPLICATION_MENUITEM((J2ME_MENU_FULLSCREEN_EDITOR_OPTION_DONE,J2ME_MENU_FULLSCREEN_EDITOR_OPTION,1,
				J2ME_MENU_FULLSCREEN_EDITOR_OPTION_INPUT_METHOD,
				SHOW,NONMOVEABLE,DISP_LIST,STR_SIM_1,0));
ADD_APPLICATION_MENUITEM((J2ME_MENU_FULLSCREEN_EDITOR_OPTION_CANCEL,J2ME_MENU_FULLSCREEN_EDITOR_OPTION,1,
				J2ME_MENU_FULLSCREEN_EDITOR_OPTION_INPUT_METHOD,
				SHOW,NONMOVEABLE,DISP_LIST,STR_SIM_2,0));
ADD_APPLICATION_MENUITEM((J2ME_MENU_FULLSCREEN_EDITOR_OPTION_INPUT_METHOD,J2ME_MENU_FULLSCREEN_EDITOR_OPTION_DONE,0,
				SHOW,NONMOVEABLE,DISP_LIST,STR_SIM_SWITCH_ASK,0));
	//To Be modify E

#ifdef __MMI_MULTI_SIM__
ADD_APPLICATION_MENUITEM((MENU_MTPNP_DM_NETWORK_SETUP,
				MAIN_MENU_SETTINGS_MENUID, MMI_SIM_NUMBER,
				MULTI_MENU(MENU9185_NETWORK_SETUP),
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_NETSET_MITEM9185, IMG_SETTING_NETWORK));

for(i = 0; i < MMI_SIM_NUMBER; i++)
{
	ADD_APPLICATION_MENUITEM((MENU_MTPNP_SIM1_CALL_SETTING + i,
				MENU8237_SCR8093_MNGCALL_MENU_MAIN,
				4,
				MENU8237_SCR8093_MNGCALL_MENU_WAIT,
				MENU8237_SCR8093_MNGCALL_MENU_FWD,
				MENU8237_SCR8093_MNGCALL_MENU_BARRING,
				MENU_LINE_SWCH,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST, 
				STRING_MTPNP_CARD1_CALL_SETTING + i, 0));
	ADD_APPLICATION_MENUITEM((MENU_MTPNP_NETWORK_SETUP+i,
				MENU_MTPNP_DM_NETWORK_SETUP,
				3,
				MENU9186_SELECTION_MODE,
				MENU9187_PREFERRED_NETWORKS,
				MENU9188_BAND_SELECTION,
				SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STRING_MTPNP_NETWORK_SETUP+i, IMG_SETTING_NETWORK));
}
	
#endif /* __MMI_MULTI_SIM__ */

#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__

ADD_APPLICATION_STRING2(STR_ID_SETTING_CHANGE_WIN_STYLE, "change screen style", "change screen style");
ADD_APPLICATION_STRING2(STR_ID_SETTING_CHANGE_WIN_STYLE_MODE, "style", "style");
#endif
#if defined(__MMI_SET_DEF_ENCODING_TYPE__)
    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEF_ENCODING_TYPE, MENU9102_INITIAL_SETUP, 2,
                              MENU_SETTING_DEF_ENCODING_BIG2,
                              MENU_SETTING_DEF_ENCODING_GB2312,
                              SHOW, (MOVEABLEACROSSPARENT | SHORTCUTABLE), DISP_LIST, STR_DEF_ENCODING_TYPE_TITLE, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEF_ENCODING_BIG2, MENU_SETTING_DEF_ENCODING_TYPE, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEF_ENCODING_TYPE_BIG5, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEF_ENCODING_GB2312, MENU_SETTING_DEF_ENCODING_TYPE, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEF_ENCODING_TYPE_GB2312, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEF_ENCODING_UCS2, MENU_SETTING_DEF_ENCODING_TYPE, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEF_ENCODING_TYPE_UCS2, 0));

    ADD_APPLICATION_STRING2(STR_DEF_ENCODING_TYPE_TITLE, "Default Encoding Type", "Default Encoding Type");
    ADD_APPLICATION_STRING2(STR_DEF_ENCODING_TYPE_BIG5, "Big5", "Big5");
    ADD_APPLICATION_STRING2(STR_DEF_ENCODING_TYPE_GB2312, "GB2312", "GB2312");
    ADD_APPLICATION_STRING2(STR_DEF_ENCODING_TYPE_UCS2, "UCS2", "UCS2");
#endif /* defined(__MMI_SET_DEF_ENCODING_TYPE__) */ 

#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_KEYS, MENU9102_INITIAL_SETUP, 4,
                              MENU_SETTING_DEDICATED_UP_KEY,
                              MENU_SETTING_DEDICATED_DOWN_KEY,
                              MENU_SETTING_DEDICATED_LEFT_KEY,
                              MENU_SETTING_DEDICATED_RIGHT_KEY,
                              SHOW, (MOVEABLEWITHINPARENT | SHORTCUTABLE), DISP_LIST, STR_DEDICATED_KEY,
                              IMG_SETTING_PHONE));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_UP_KEY, MENU_SETTING_DEDICATED_KEYS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEDICATED_UP_KEY, IMG_SETTING_PHONE));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_DOWN_KEY, MENU_SETTING_DEDICATED_KEYS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEDICATED_DOWN, IMG_SETTING_PHONE));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_LEFT_KEY, MENU_SETTING_DEDICATED_KEYS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEDICATED_LEFT, IMG_SETTING_PHONE));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_RIGHT_KEY, MENU_SETTING_DEDICATED_KEYS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEDICATED_RIGHT, IMG_SETTING_PHONE));

    ADD_APPLICATION_STRING2(STR_DEDICATED_KEY, "Dedicated Key", "Dedicated key");
    ADD_APPLICATION_STRING2(STR_DEDICATED_UP_KEY, "Up", "Up key");
    ADD_APPLICATION_STRING2(STR_DEDICATED_DOWN, "Down", "Down key");
    ADD_APPLICATION_STRING2(STR_DEDICATED_LEFT, "Left", "Left key");
    ADD_APPLICATION_STRING2(STR_DEDICATED_RIGHT, "Right", "Right key");
#endif /* defined(__MMI_DEDICATED_KEY_SHORTCUTS__) */ 
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    ADD_APPLICATION_STRING2(STR_HANDWRITING_SETTING, "Handwriting", "Handwriting Setting");
    ADD_APPLICATION_STRING2(STR_CALIBRATION_SETTING, "Pen Calibration", "Calibration Setting");

    ADD_APPLICATION_STRING2(STR_HANDWRITING_PEN_SPEED, "Pen Speed", "Pen Speed");
    ADD_APPLICATION_STRING2(STR_HANDWRITING_PEN_COLOR, "Pen Color", "Pen Color");

    ADD_APPLICATION_STRING2(STR_HANDWRITING_FAST, "Fast", "Pen Speed Fast");
    ADD_APPLICATION_STRING2(STR_HANDWRITING_MEDIUM, "Medium", "Pen Speed Medium");
    ADD_APPLICATION_STRING2(STR_HANDWRITING_SLOW, "Slow", "Pen Speed Slow");

    ADD_APPLICATION_STRING2(
        STR_CALIBRATION_WELCOME_TEXT,
        "Calibration wizard. You have to calibrate the touch-screen. Click anywhere on the screen with your stylus to start calibration.",
        "Calibration Screen Welcome Text");
    ADD_APPLICATION_STRING2(
        STR_CALIBRATION_TOP_LEFT_TEXT,
        "Click on the top-left point.",
        "Calibration Screen Top Left Text");
    ADD_APPLICATION_STRING2(
        STR_CALIBRATION_BOTTOM_RIGHT_TEXT,
        "Click on the bottom-right point.",
        "Calibration Screen Bottom Right Text");
    ADD_APPLICATION_STRING2(
        STR_CALIBRATION_CENTER_TEXT,
        "Click on the center point.",
        "Calibration Screen Center Text");

    ADD_APPLICATION_STRING2(
        STR_CALIBRATION_ARROW_CENTER_TEXT,
        "Click on the center point.",
        "Calibration Screen Center Text");

    ADD_APPLICATION_MENUITEM((MENU_SETTING_HANDWRITING, MENU9102_INITIAL_SETUP, 0, SHOW,
                              NONMOVEABLE, DISP_LIST, STR_HANDWRITING_SETTING, 0));

    ADD_APPLICATION_IMAGE2(
        IMG_CALIBRATION_TOP_LEFT,
        CUST_IMG_PATH "/MainLCD/UIElement/Calibration/CALI_SMALL.gif",
        "IMG_CALIBATION_TOP_LEFT");
    ADD_APPLICATION_IMAGE2(
        IMG_CALIBRAION_BOTTOM_RIGHT,
        CUST_IMG_PATH "/MainLCD/UIElement/Calibration/CALI_SMALL.gif",
        "IMG_CALIBRATION_BOTTOM_RIGHT");
    ADD_APPLICATION_IMAGE2(
        IMG_CALIBRATION_CENTER,
        CUST_IMG_PATH "/MainLCD/UIElement/Calibration/CALI_CENTER.gif",
        "IMG_CALIBRATION_CENTER");

#ifdef __MMI_HANDWRITING_PAD__
    ADD_APPLICATION_STRING2(
        STR_TEST_CONTROL_AREA_WELCOME_TEXT,
        "Click on each control area.",
        "Test control area description");
#endif /* __MMI_HANDWRITING_PAD__ */ 

#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 
#if defined(MOTION_SENSOR_SUPPORT)
	ADD_APPLICATION_MENUITEM((MENU_SETTING_MOTION_SENSOR,MENU9102_INITIAL_SETUP,2,
					SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST,STR_SETTING_MOTION_SENSOR,0));
	ADD_APPLICATION_MENUITEM((MENU_SETTING_MOTION_SENSOR_ON,MENU_SETTING_MOTION_SENSOR,0,
					SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST,STR_GLOBAL_ON,0));
	ADD_APPLICATION_MENUITEM((MENU_SETTING_MOTION_SENSOR_OFF,MENU_SETTING_MOTION_SENSOR,0,
					SHOW, MOVEABLEACROSSPARENT | SHORTCUTABLE, DISP_LIST,STR_GLOBAL_OFF,0));
	
    ADD_APPLICATION_STRING2(STR_SETTING_MOTION_SENSOR, "Motion Sensor", "Motion Sensor Setting");
#endif

    ADD_APPLICATION_STRING2(STR_MENU9100_CALL_SETTING, "Call Setup", "Settings Call.");
    ADD_APPLICATION_STRING2(STR_MENU9102_INITIAL_SETUP, "Phone Setup", "Phone Setup.");
    ADD_APPLICATION_STRING2(STR_MENU9141_TIME_AND_DATE, "Time and Date", "Time And Date Selection.");

    ADD_APPLICATION_STRING2(STR_SETTIME_UNFINISHED, "Unfinish! (Year range from 2000 ~ 2030)", "Set Time Unfinish");
    ADD_APPLICATION_STRING2(STR_MENU9142_SETHOMECITY, "Set Home City", "Set Home City Title.");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_SET_DT, "Set Time/Date", "Set Time and Date.");
#ifdef __MMI_SEPARATE_DT_SETTING__
    ADD_APPLICATION_STRING2(
        STR_DATE_TIME_SET_FORMAT_CAPTION,
        "Set Date and Time..",
        "Set Date and Time Format String caption");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_SET_DATEANDFORMAT, "Set Date and Format", "Set Date and Format.");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_SET_TIMEANDFORMAT, "Set Time and Format", "Set Time and Format");
#endif /* __MMI_SEPARATE_DT_SETTING__ */ 
    ADD_APPLICATION_STRING2(STR_MENU9144_SETTIMEANDDATEFORMAT, "Set Format", "Set Time and Date Format.");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_SET_DST, "Day Light Saving", "Set Day Light Saving.");

    ADD_APPLICATION_STRING2(STR_ENTER_TIME_FORMAT_DISP_CAPTION, "HH:MM", "Display Time Entry Format.");
    ADD_APPLICATION_STRING2(STR_ENTER_DATE_FORMAT_DISP_CAPTION, "YYYY/MM/DD", "Display Date Entry Format.");
    ADD_APPLICATION_STRING2(STR_TIME_FORMAT_CAPTION, "Time Format:", "Time format Sring");
    ADD_APPLICATION_STRING2(STR_DATE_FORMAT_CAPTION, "Date Format:", "Set Date Format String");

    ADD_APPLICATION_STRING2(
        STR_DATE_TIME_SET_FORMAT_CAPTION,
        "Set Date and Time..",
        "Set Date and Time Format String caption");

    ADD_APPLICATION_STRING2(STR_MENU9146_WELCOME_TEXT, "Greeting Text", "Set Welcome Text.");

    ADD_APPLICATION_STRING2(STR_DD_MMM_YYYY_H, "DD-MMM-YYYY", "Date Format 23-Jan-03.");
    ADD_APPLICATION_STRING2(STR_DD_MM_YYYY_S, "DD/MM/YYYY", "Date Format 23-01-03.");
    ADD_APPLICATION_STRING2(STR_MM_DD_YYYY_S, "MM/DD/YYYY", "Date Format 01/23/03.");
    ADD_APPLICATION_STRING2(STR_YYYY_MM_DD_S, "YYYY/MM/DD", "Date Format 23/01/03.");
    ADD_APPLICATION_STRING2(STR_YYYY_MM_DD_H, "YYYY-MM-DD", "Date Format 01/23/03.");
    ADD_APPLICATION_STRING2(STR_MMM_DD_YYYY, "MMM DD,YYYY", "Date Format 23/01/03.");

    ADD_APPLICATION_STRING2(STR_SCR9000_SETTINGS_CAPTION, "Settings", "Select Setting Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9001_CALL_SETTING_CAPTION, "Call Setup", "Select Call Setting Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9041_TIMEANDDATE_CAPTION, "Set Time/Date", "Select Time and Date Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9042_SETTIME_CAPTION, "Set Time", "Select Set Time Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9043_SETTIMEFORMAT_CAPTION, "Set Time Format", "Select Set Time Format Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9044_SETDATE_CAPTION, "Set Date", "Select Set Date Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9045_SETDATEFORMAT_CAPTION, "Set Date Format", "Select Set Date Format Menu.");

    ADD_APPLICATION_STRING2(STR_24, "24", "Time Format 24.");
    ADD_APPLICATION_STRING2(STR_12, "12", "Time Format 12.");

    ADD_APPLICATION_STRING2(STR_SCR9033_AUTO_REDIAL_CAPTION, "Auto Redial", "Enter Auto Redial Menu.");

#if defined(__MMI_APHORISM__)
/* under construction !*/
/* under construction !*/
#endif /* defined(__MMI_APHORISM__) */ 

#ifdef __MMI_TVOUT__
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT, "TV Out", "STR_ID_SETTING_TVOUT");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_SETTING, "Setting", "STR_ID_SETTING_TVOUT_SETTING");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_FORMAT, "Visual Format", "STR_ID_SETTING_TVOUT_FORMAT");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_FORMAT_PAL, "PAL", "STR_ID_SETTING_TVOUT_FORMAT_PAL");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_FORMAT_NTSC, "NTSC", "STR_ID_SETTING_TVOUT_FORMAT_NTSC");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_AUD_PATH, "Audio Output", "STR_ID_SETTING_TVOUT_AUD_PATH");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_AUD_PATH_BOTH, "TV and Phone", "STR_ID_SETTING_TVOUT_AUD_PATH_BOTH");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_AUD_PATH_TV, "TV Only", "STR_ID_SETTING_TVOUT_AUD_PATH_TV");

    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_ADJUST, "Adjust", "STR_ID_SETTING_TVOUT_ADJUST");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_ADJUST_Y_GAMMA, "Gamma Y", "STR_ID_SETTING_TVOUT_MM_STYLE_PHONE");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_ADJUST_U, "U", "STR_ID_SETTING_TVOUT_MM_STYLE_FULLSCR");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_ADJUST_V, "V", "STR_ID_SETTING_TVOUT_MM_STYLE_FULLSCR");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_PLUG_IN, "TV-Out plugged in", "STR_ID_SETTING_TVOUT_PLUG_IN");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_PLUG_OUT, "TV-Out plugged out", "STR_ID_SETTING_TVOUT_PLUG_OUT");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_NOT_READY, "TV-Out Not Ready", "STR_ID_SETTING_TVOUT_NOT_READY");
#endif /* __MMI_TVOUT__ */ 

    ADD_APPLICATION_STRING2(STR_MENU9146_ON, "Greeting Text:", "On Option.");
    ADD_APPLICATION_STRING2(STR_MENU9146_OFF, "Status", "Off Option.");
    ADD_APPLICATION_STRING2(STR_RESTORE_SUCCESS, "Setting Restored!", "Restore Success Message.");
    ADD_APPLICATION_STRING2(STR_FDL_NOT_READY, "FDN not ready.", "FDN not Ready Try Again Message.");
    ADD_APPLICATION_STRING2(STR_BDL_NOT_READY, "Please try later.", "Please try later.");
    ADD_APPLICATION_STRING2(STR_MENU_RESTORE, "Restore Factory Settings", "Restore Factory Setting Option.");
    ADD_APPLICATION_STRING2(STR_MENU_SETTINGS_ABOUT, "About", "Menu item to display the copyright information");
    ADD_APPLICATION_STRING2(
        STR_SETTING_CODE_VERIFIED,
        "Code Accepted!",
        "String associated with Code Verified Screen.");
    ADD_APPLICATION_IMAGE2(
        IMG_TIME_AND_DATE_BLANK,
        CUST_IMG_BASE_PATH "/EmptyImage.bmp",
        "Image for Time And Date Selection.");
    ADD_APPLICATION_IMAGE2(
        IMG_MENU9142_SETHOMECITY,
        CUST_IMG_BASE_PATH "/EmptyImage.bmp",
        "Image Set Home City Caption.");
    ADD_APPLICATION_IMAGE2(
        IMG_DATE_TIME_SET_FORMAT_CAPTION,
        CUST_IMG_BASE_PATH "/EmptyImage.bmp",
        "Image Date and Time Format String caption.");
    ADD_APPLICATION_IMAGE2(
        IMG_SCR_SETTING_CAPTION,
        CUST_IMG_PATH "/MainLCD/TitleBar/TB_ST.PBM",
        "Title Image For Settings Main Screen.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_CALL_SETUP,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_SCALL.PBM",
        "Setting Submenu image for Call Setup.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_NETWORK,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_SNET.PBM",
        "Setting Submenu image for Network Setup.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_PHONE,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_SPH.PBM",
        "Setting Submenu image for Phone Setup.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_STORE,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_STORE.PBM",
        "Setting Submenu image for Restore Setting.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_SHORTCUT,
        CUST_IMG_PATH "/MainLCD/SubMenu/Funandgame/SB_Favor.pbm",
        "Setting Submenu image for Time and Date.");
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    ADD_APPLICATION_IMAGE2(
        IMG_CALIBRATION_MENU_ICON,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_PCA.pbm",
        "Setting Submenu image for Pen calibration.");
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 

#ifndef __MMI_SUBLCD__
    ADD_APPLICATION_IMAGE2(
        IMG_CONTRAST_BACKGROUND,
        CUST_IMG_PATH "/MainLCD/UIElement/ValueMap/MAPBG.gif",
        "Background image for Contrast screen");
#endif /* __MMI_SUBLCD__ */ 

    ADD_APPLICATION_STRING2(
        STR_SETTING_RESTORE_ERR1,
        "Password is minimum 4 digits!",
        "Password is minimum 4 digits message.");
    ADD_APPLICATION_STRING2(STR_SETTING_RESTORE_CONFIRM, "This action will clear all information in phone of yours(phonebook,message and so on),ARE YOU SURE?", "Restore factory confirmation message.");
    ADD_APPLICATION_STRING2(
        STR_SETTING_RESTORE_PROCESSING,
        "Restore Processing...",
        "Restore factory confirmation message.");
    ADD_APPLICATION_STRING2(STR_SETTING_WRONG, "Wrong Password!", "Wrong.");

    ADD_APPLICATION_STRING2(STR_SETTING_BLOCK, "Password Blocked!", "Password Blocked!");

    ADD_APPLICATION_STRING2(STR_SETTING_WRONG_TIME, "Please Enter Correct Date/Time", "Notification Msg");
    ADD_APPLICATION_STRING2(STR_RESTORE_PHONE, "Enter Password", "Phone code check password.");
	
	ADD_APPLICATION_STRING2(STR_ID_SHORTCUTS_LIST_FULL,"List Fullp","List Full");

#ifdef __FLIGHT_MODE_SUPPORT__
    PopulateFlightModeResData();
#endif 
#if (defined __UNI_MULTI_SIM_CARD__)	
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_CARD_MENU ,"Select SIM Card","Select SIM Card.");
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_CARD_POPUP ,"Switch SIM card now, rebooting...","Switch SIM card now, rebooting...");
/*UNI@lizongzheng_20080201 14:08:26  双卡设置,放进快捷方式,作为候选  */
	ADD_APPLICATION_MENUITEM((MENU_SETTING_MULTI_SIM_CARD,MAIN_MENU_SETTINGS_MENUID,0,
		SHOW, SHORTCUTABLE |MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_MULTI_SIM_CARD_MENU,IMG_SETTING_NETWORK));
	#if defined(__UNI_MULTI_SIM_CARD_SERVER__)
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_CARD_SERVER ,"Dual Online","Dual Online");
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_CARD_CHANGE,"No Sim And Switch Card?","No Sim And Switch Card?");
	/*UNI@liugang_20070724 提示信息修改*/
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_CARD_DISPLAY,"Double waits the service to open please do confirm two SIM card insertion is correct!","Double waits the service to open please do confirm two SIM card insertion is correct!");
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_DISPLAY_MENU,"In idle hint dual online","In idle hint dual online");
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_FAILED_NO_SIM,"No Sim","No Sim");
    	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_FAILED_NO_NET_SIM1,"SIM1 Network nonsupport","SIM1 Network nonsupport");
    	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_FAILED_NO_NET_SIM2,"SIM2 Network nonsupport","SIM2 Network nonsupport");
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_SERVER_HELLP,"help","help");
	ADD_APPLICATION_STRING2(STR_ID_MULTE_SIM_MENU_HELP,"help","help");
	
	ADD_APPLICATION_MENUITEM((MENU_SETTING_MULTI_SIM_SERVER,MAIN_MENU_SETTINGS_MENUID,4,
		MENU_SETTING_SERVER_OPEN,
		MENU_SETTING_SERVER_CLOSE,
		MENU_SETTING_SERVER_POPU,
		MENU_SETTING_SERVER_HELP,
		SHOW, SHORTCUTABLE |MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_MULTI_SIM_CARD_SERVER,IMG_SETTING_NETWORK));
	ADD_APPLICATION_MENUITEM((MENU_SETTING_SERVER_OPEN,MENU_SETTING_MULTI_SIM_SERVER,0,
		SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_GLOBAL_ON,0));
	ADD_APPLICATION_MENUITEM((MENU_SETTING_SERVER_CLOSE,MENU_SETTING_MULTI_SIM_SERVER,0,
		SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_GLOBAL_OFF,0));
	ADD_APPLICATION_MENUITEM((MENU_SETTING_SERVER_POPU,MENU_SETTING_MULTI_SIM_SERVER,0,
		SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_MULTI_SIM_DISPLAY_MENU,0));
	ADD_APPLICATION_MENUITEM((MENU_SETTING_SERVER_HELP,MENU_SETTING_MULTI_SIM_SERVER,0,
		SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_MULTE_SIM_MENU_HELP,0));
	//UNI@liugang_20070802	双卡项目放在一个菜单里
	ADD_APPLICATION_MENUITEM((MENU_SETTING_MULTI_SIM_SET,MAIN_MENU_SETTINGS_MENUID,2,
		MENU_SETTING_MULTI_SIM_CARD,
		MENU_SETTING_MULTI_SIM_SERVER,
		SHOW, SHORTCUTABLE |MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_MULTI_SIM_CARD_MENU,IMG_SETTING_NETWORK));
	#endif
#endif 
    PopulateFontSizeResData();
    PopulateGPIOSettingResData();

    PopulateNetworkSetupResData();
    populateManageCalls();
    PopulatePhoneSetupResData();
    populateSecuritySetup();
    PopulateLanguageStrings();
#ifdef __MMI_SOUND_EFFECT__
    PopulateSoundEffectResData();
#endif 
#ifdef __MMI_TTS_FEATURES__
 PopulateTTSPlayerResData();
#endif

#ifdef __FLIGHT_MODE_SUPPORT__
    PopulateFlightModeResData();
#endif 
}

#if 0
/*****************************************************************************
 * FUNCTION
 *  PopulateSettingMenu
 * DESCRIPTION
 *  populating various resources
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateSettingMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	//PopulateSettingMenu();
    ADD_APPLICATION_MENUITEM((MENU9100_CALL_SETTING, MAIN_MENU_SETTINGS_MENUID, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_MENU9100_CALL_SETTING, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_RESTORE, MAIN_MENU_SETTINGS_MENUID, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_MENU_RESTORE, IMG_SETTING_STORE));

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    ADD_APPLICATION_MENUITEM((MENU_SETTING_CALIBRATION, MAIN_MENU_SETTINGS_MENUID, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_CALIBRATION_SETTING, IMG_CALIBRATION_MENU_ICON));
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 
//[221] Added by Ryh 2009/02/12								[END]
ADD_APPLICATION_MENUITEM((MENU9102_INITIAL_SETUP,MAIN_MENU_SETTINGS_MENUID, 9,
				MENU9141_TIME_AND_DATE,
				MENU_SETTING_SCHEDULE_POWER,
				MENU_SETTING_LANGUAGE,
				//MENU3101_THEMES1,
				MENU_SETTING_PREFERED_INPUT_METHOD,
				MENU_IDLE_SCR_DISP,
   				MENU9146_WELCOME_TEXT,	
				MENU_SETTING_SPEED_DIAL,
				MAIN_MENU_SHORTCUTS_MENUID,
				MENU_SETTING_DEDICATED_KEYS,
				//MENU_SETTING_FLIGHT_MODE,
				SHOW, MOVEABLEACROSSPARENT, DISP_LIST,STR_MENU9102_INITIAL_SETUP,IMG_SETTING_PHONE));

#if 0
    ADD_APPLICATION_MENUITEM((MENU9102_INITIAL_SETUP, MAIN_MENU_SETTINGS_MENUID, 
				#ifdef __MMI_DOUBLE_SIM__
				1+
				#endif

	                       MENU_ENUM_TOTAL_PHONE_SETUP,
                              MENU9141_TIME_AND_DATE, MENU_SETTING_SCHEDULE_POWER, MENU_SETTING_LANGUAGE,
		              #ifdef __MMI_DOUBLE_SIM__
		 		MENU_SWITCH_SIM,
		 		#endif

#ifdef __MMI_PREFER_INPUT_METHOD__
                              MENU_SETTING_PREFERED_INPUT_METHOD,
#endif 
#ifdef __MMI_SET_DEF_ENCODING_TYPE__
                              MENU_SETTING_DEF_ENCODING_TYPE,
#endif 
                              MENU_IDLE_SCR_DISP,
#if !defined(__MMI_NO_CONTRAST_LEVEL__)
                              MENU_SETTING_CONTRAST_LEVEL,
#endif 
                              MENU9146_WELCOME_TEXT, MENU_SETTING_SPEED_DIAL,
#if !defined(__MMI_VERSION_2__)
                              MAIN_MENU_SHORTCUTS_MENUID,
#endif 
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
                              MENU_SETTING_DEDICATED_KEYS,
#endif 
#if (defined(__MMI_NITZ__) && (!defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__)))
                              MENU_SETTING_MENU_TREE,
#endif 
#ifdef __FLIGHT_MODE_SUPPORT__
                              MENU_SETTING_FLIGHT_MODE,
#endif 
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
                              MENU_SETTING_HANDWRITING,
#endif 
#ifdef __MMI_TVOUT__
                              MENU_ID_SETTING_TVOUT,
#endif 
#ifdef __MMI_CHANGABLE_FONT__
                              MENU_ID_PHNSET_FONT_SIZE,
#endif 
                              MENU_ID_PHNSET_GPIO_SETTING,
                              SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_MENU9102_INITIAL_SETUP, IMG_SETTING_PHONE));
#endif

    ADD_APPLICATION_MENUITEM((MENU9146_WELCOME_TEXT, MENU9102_INITIAL_SETUP, 0,
                              SHOW, (MOVEABLEACROSSPARENT | SHORTCUTABLE), DISP_LIST, STR_MENU9146_WELCOME_TEXT, 0));
    PopulateFontSizeResData();
    PopulateGPIOSettingResData();

    /* TV out related */
#ifdef __MMI_TVOUT__
    ADD_APPLICATION_MENUITEM((MENU_ID_SETTING_TVOUT, MENU9102_INITIAL_SETUP, 2,
                              MENU_ID_SETTING_TVOUT_SETTING,
                              MENU_ID_SETTING_TVOUT_ADJUST, SHOW, NONMOVEABLE, DISP_LIST, STR_ID_SETTING_TVOUT, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_SETTING_TVOUT_SETTING, MENU_ID_SETTING_TVOUT, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_ID_SETTING_TVOUT_SETTING, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_SETTING_TVOUT_ADJUST, MENU_ID_SETTING_TVOUT, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_ID_SETTING_TVOUT_ADJUST, 0));
#endif /* __MMI_TVOUT__ */ 

/**************************************************************/
    /* Robin 0722
       NOTICE!!  This menu is not configure here
       It is dynamic list initialized in EntryPhnsetTimeAndDate() */
/**************************************************************/
    ADD_APPLICATION_MENUITEM((MENU9141_TIME_AND_DATE, MENU9102_INITIAL_SETUP, MENU_SETDT_NUM,
#ifndef __MMI_SHOW_HOMECITY__
                              MENU9142_SETHOMECITY,
#endif 
#ifndef __MMI_SEPARATE_DT_SETTING__
                              MENU9143_SETTIMEANDDATE, MENU9144_SETTIMEANDDATEFORMAT,
#else 
                              MENU_ID_SETTING_DATEANDFORMAT, MENU_ID_SETTING_TIMEANDFORMAT,
#endif 
#if (defined(__MMI_NITZ__) && defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__))
                              MENU9145_SETTIMEANDDATENITZ,
#endif 
                              SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_MENU9141_TIME_AND_DATE, 0));

#ifndef __MMI_SHOW_HOMECITY__
    ADD_APPLICATION_MENUITEM((MENU9142_SETHOMECITY, MENU9141_TIME_AND_DATE, 0,
                              SHOW, (MOVEABLEWITHINPARENT | SHORTCUTABLE), DISP_LIST, STR_MENU9142_SETHOMECITY, 0));
#endif /* __MMI_SHOW_HOMECITY__ */ 
#ifndef __MMI_SEPARATE_DT_SETTING__
    ADD_APPLICATION_MENUITEM((MENU9143_SETTIMEANDDATE, MENU9141_TIME_AND_DATE, 0,
                              SHOW, (MOVEABLEWITHINPARENT | SHORTCUTABLE), DISP_LIST, STR_ID_PHNSET_SET_DT, 0));

    ADD_APPLICATION_MENUITEM((MENU9144_SETTIMEANDDATEFORMAT, MENU9141_TIME_AND_DATE, 0,
                              SHOW, (MOVEABLEWITHINPARENT | SHORTCUTABLE), DISP_LIST, STR_MENU9144_SETTIMEANDDATEFORMAT,
                              0));
#else /* __MMI_SEPARATE_DT_SETTING__ */ 
    ADD_APPLICATION_MENUITEM((MENU_ID_SETTING_DATEANDFORMAT, MENU9141_TIME_AND_DATE, 0,
                              SHOW, (MOVEABLEWITHINPARENT | SHORTCUTABLE), DISP_LIST, STR_ID_PHNSET_SET_DATEANDFORMAT,
                              0));

    ADD_APPLICATION_MENUITEM((MENU_ID_SETTING_TIMEANDFORMAT, MENU9141_TIME_AND_DATE, 0,
                              SHOW, (MOVEABLEWITHINPARENT | SHORTCUTABLE), DISP_LIST, STR_ID_PHNSET_SET_TIMEANDFORMAT,
                              0));
#endif /* __MMI_SEPARATE_DT_SETTING__ */ 
#if (defined(__MMI_NITZ__) && defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__))
    ADD_APPLICATION_MENUITEM((MENU9145_SETTIMEANDDATENITZ, MENU9141_TIME_AND_DATE, 0,
                              SHOW, (MOVEABLEACROSSPARENT | SHORTCUTABLE), DISP_LIST, STR_MENU_SETTING_MENU_TREE, 0));
#endif /* (defined(__MMI_NITZ__) && defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__)) */ 

    ADD_APPLICATION_MENUITEM((MENU_SETTING_LANGUAGE, MENU9102_INITIAL_SETUP, 0,
                              SHOW, (MOVEABLEACROSSPARENT | SHORTCUTABLE), DISP_LIST, STR_MENU_LANGUAGE, 0));

#ifdef __FLIGHT_MODE_SUPPORT__
    ADD_APPLICATION_MENUITEM((MENU_SETTING_FLIGHT_MODE, MENU9102_INITIAL_SETUP, 0,
                              SHOW, SHORTCUTABLE, DISP_LIST, STR_MENU_FLIGHT_MODE, 0));
#endif /* __FLIGHT_MODE_SUPPORT__ */ 

#if defined(__MMI_PREFER_INPUT_METHOD__)
    ADD_APPLICATION_MENUITEM((MENU_SETTING_PREFERED_INPUT_METHOD, MENU9102_INITIAL_SETUP, 0,
                              SHOW, (MOVEABLEACROSSPARENT | SHORTCUTABLE), DISP_LIST, STR_MENU_INPUT_METHOD, 0));
#endif /* defined(__MMI_PREFER_INPUT_METHOD__) */ 

#if defined(__MMI_SET_DEF_ENCODING_TYPE__)
    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEF_ENCODING_TYPE, MENU9102_INITIAL_SETUP, 2,
                              MENU_SETTING_DEF_ENCODING_BIG2,
                              MENU_SETTING_DEF_ENCODING_GB2312,
                              SHOW, (MOVEABLEACROSSPARENT | SHORTCUTABLE), DISP_LIST, STR_DEF_ENCODING_TYPE_TITLE, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEF_ENCODING_BIG2, MENU_SETTING_DEF_ENCODING_TYPE, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEF_ENCODING_TYPE_BIG5, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEF_ENCODING_GB2312, MENU_SETTING_DEF_ENCODING_TYPE, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEF_ENCODING_TYPE_GB2312, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEF_ENCODING_UCS2, MENU_SETTING_DEF_ENCODING_TYPE, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEF_ENCODING_TYPE_UCS2, 0));

    ADD_APPLICATION_STRING2(STR_DEF_ENCODING_TYPE_TITLE, "Default Encoding Type", "Default Encoding Type");
    ADD_APPLICATION_STRING2(STR_DEF_ENCODING_TYPE_BIG5, "Big5", "Big5");
    ADD_APPLICATION_STRING2(STR_DEF_ENCODING_TYPE_GB2312, "GB2312", "GB2312");
    ADD_APPLICATION_STRING2(STR_DEF_ENCODING_TYPE_UCS2, "UCS2", "UCS2");
#endif /* defined(__MMI_SET_DEF_ENCODING_TYPE__) */ 

    ADD_APPLICATION_MENUITEM((MENU9171_ON, MENU_SETTING_SCH_POW_SET1_POW_ON, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_ON, 0));

    ADD_APPLICATION_MENUITEM((MENU9172_OFF, MENU_SETTING_SCH_POW_SET1_POW_ON, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_GLOBAL_OFF, 0));

#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_KEYS, MENU9102_INITIAL_SETUP, 4,
                              MENU_SETTING_DEDICATED_UP_KEY,
                              MENU_SETTING_DEDICATED_DOWN_KEY,
                              MENU_SETTING_DEDICATED_LEFT_KEY,
                              MENU_SETTING_DEDICATED_RIGHT_KEY,
                              SHOW, (MOVEABLEWITHINPARENT | SHORTCUTABLE), DISP_LIST, STR_DEDICATED_KEY,
                              IMG_SETTING_PHONE));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_UP_KEY, MENU_SETTING_DEDICATED_KEYS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEDICATED_UP_KEY, IMG_SETTING_PHONE));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_DOWN_KEY, MENU_SETTING_DEDICATED_KEYS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEDICATED_DOWN, IMG_SETTING_PHONE));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_LEFT_KEY, MENU_SETTING_DEDICATED_KEYS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEDICATED_LEFT, IMG_SETTING_PHONE));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_DEDICATED_RIGHT_KEY, MENU_SETTING_DEDICATED_KEYS, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_DEDICATED_RIGHT, IMG_SETTING_PHONE));

    ADD_APPLICATION_STRING2(STR_DEDICATED_KEY, "Dedicated Key", "Dedicated key");
    ADD_APPLICATION_STRING2(STR_DEDICATED_UP_KEY, "Up", "Up key");
    ADD_APPLICATION_STRING2(STR_DEDICATED_DOWN, "Down", "Down key");
    ADD_APPLICATION_STRING2(STR_DEDICATED_LEFT, "Left", "Left key");
    ADD_APPLICATION_STRING2(STR_DEDICATED_RIGHT, "Right", "Right key");
#endif /* defined(__MMI_DEDICATED_KEY_SHORTCUTS__) */ 

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    ADD_APPLICATION_STRING2(STR_HANDWRITING_SETTING, "Handwriting", "Handwriting Setting");
    ADD_APPLICATION_STRING2(STR_CALIBRATION_SETTING, "Pen Calibration", "Calibration Setting");

    ADD_APPLICATION_STRING2(STR_HANDWRITING_PEN_SPEED, "Pen Speed", "Pen Speed");
    ADD_APPLICATION_STRING2(STR_HANDWRITING_PEN_COLOR, "Pen Color", "Pen Color");

    ADD_APPLICATION_STRING2(STR_HANDWRITING_FAST, "Fast", "Pen Speed Fast");
    ADD_APPLICATION_STRING2(STR_HANDWRITING_MEDIUM, "Medium", "Pen Speed Medium");
    ADD_APPLICATION_STRING2(STR_HANDWRITING_SLOW, "Slow", "Pen Speed Slow");

    ADD_APPLICATION_STRING2(
        STR_CALIBRATION_WELCOME_TEXT,
        "Calibration wizard. You have to calibrate the touch-screen. Click anywhere on the screen with your stylus to start calibration.",
        "Calibration Screen Welcome Text");
    ADD_APPLICATION_STRING2(
        STR_CALIBRATION_TOP_LEFT_TEXT,
        "Click on the top-left point.",
        "Calibration Screen Top Left Text");
    ADD_APPLICATION_STRING2(
        STR_CALIBRATION_BOTTOM_RIGHT_TEXT,
        "Click on the bottom-right point.",
        "Calibration Screen Bottom Right Text");
    ADD_APPLICATION_STRING2(
        STR_CALIBRATION_CENTER_TEXT,
        "Click on the center point.",
        "Calibration Screen Center Text");

    ADD_APPLICATION_MENUITEM((MENU_SETTING_HANDWRITING, MENU9102_INITIAL_SETUP, 0, SHOW,
                              NONMOVEABLE, DISP_LIST, STR_HANDWRITING_SETTING, 0));

    ADD_APPLICATION_IMAGE2(
        IMG_CALIBRATION_TOP_LEFT,
        CUST_IMG_PATH "/MainLCD/UIElement/Calibration/CALI_SMALL.gif",
        "IMG_CALIBATION_TOP_LEFT");
    ADD_APPLICATION_IMAGE2(
        IMG_CALIBRAION_BOTTOM_RIGHT,
        CUST_IMG_PATH "/MainLCD/UIElement/Calibration/CALI_SMALL.gif",
        "IMG_CALIBRATION_BOTTOM_RIGHT");
    ADD_APPLICATION_IMAGE2(
        IMG_CALIBRATION_CENTER,
        CUST_IMG_PATH "/MainLCD/UIElement/Calibration/CALI_CENTER.gif",
        "IMG_CALIBRATION_CENTER");

#ifdef __MMI_HANDWRITING_PAD__
    ADD_APPLICATION_STRING2(
        STR_TEST_CONTROL_AREA_WELCOME_TEXT,
        "Click on each control area.",
        "Test control area description");
#endif /* __MMI_HANDWRITING_PAD__ */ 

#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 

    ADD_APPLICATION_STRING2(STR_MENU9100_CALL_SETTING, "Call Setup", "Settings Call.");
    ADD_APPLICATION_STRING2(STR_MENU9102_INITIAL_SETUP, "Phone Setup", "Phone Setup.");
    ADD_APPLICATION_STRING2(STR_MENU9141_TIME_AND_DATE, "Time and Date", "Time And Date Selection.");

    ADD_APPLICATION_STRING2(STR_SETTIME_UNFINISHED, "Unfinish! (Year range from 2000 ~ 2030)", "Set Time Unfinish");
    ADD_APPLICATION_STRING2(STR_MENU9142_SETHOMECITY, "Set Home City", "Set Home City Title.");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_SET_DT, "Set Time/Date", "Set Time and Date.");
#ifdef __MMI_SEPARATE_DT_SETTING__
    ADD_APPLICATION_STRING2(
        STR_DATE_TIME_SET_FORMAT_CAPTION,
        "Set Date and Time..",
        "Set Date and Time Format String caption");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_SET_DATEANDFORMAT, "Set Date and Format", "Set Date and Format.");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_SET_TIMEANDFORMAT, "Set Time and Format", "Set Time and Format");
#endif /* __MMI_SEPARATE_DT_SETTING__ */ 
    ADD_APPLICATION_STRING2(STR_MENU9144_SETTIMEANDDATEFORMAT, "Set Format", "Set Time and Date Format.");
    ADD_APPLICATION_STRING2(STR_ID_PHNSET_SET_DST, "Day Light Saving", "Set Day Light Saving.");

    ADD_APPLICATION_STRING2(STR_ENTER_TIME_FORMAT_DISP_CAPTION, "HH:MM", "Display Time Entry Format.");
    ADD_APPLICATION_STRING2(STR_ENTER_DATE_FORMAT_DISP_CAPTION, "YYYY/MM/DD", "Display Date Entry Format.");
    ADD_APPLICATION_STRING2(STR_TIME_FORMAT_CAPTION, "Time Format:", "Time format Sring");
    ADD_APPLICATION_STRING2(STR_DATE_FORMAT_CAPTION, "Date Format:", "Set Date Format String");

    ADD_APPLICATION_STRING2(
        STR_DATE_TIME_SET_FORMAT_CAPTION,
        "Set Date and Time..",
        "Set Date and Time Format String caption");

    ADD_APPLICATION_STRING2(STR_MENU9146_WELCOME_TEXT, "Greeting Text", "Set Welcome Text.");

    ADD_APPLICATION_STRING2(STR_DD_MMM_YYYY_H, "DD-MMM-YYYY", "Date Format 23-Jan-03.");
    ADD_APPLICATION_STRING2(STR_DD_MM_YYYY_S, "DD/MM/YYYY", "Date Format 23-01-03.");
    ADD_APPLICATION_STRING2(STR_MM_DD_YYYY_S, "MM/DD/YYYY", "Date Format 01/23/03.");
    ADD_APPLICATION_STRING2(STR_YYYY_MM_DD_S, "YYYY/MM/DD", "Date Format 23/01/03.");
    ADD_APPLICATION_STRING2(STR_YYYY_MM_DD_H, "YYYY-MM-DD", "Date Format 01/23/03.");
    ADD_APPLICATION_STRING2(STR_MMM_DD_YYYY, "MMM DD,YYYY", "Date Format 23/01/03.");

    ADD_APPLICATION_STRING2(STR_SCR9000_SETTINGS_CAPTION, "Settings", "Select Setting Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9001_CALL_SETTING_CAPTION, "Call Setup", "Select Call Setting Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9041_TIMEANDDATE_CAPTION, "Set Time/Date", "Select Time and Date Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9042_SETTIME_CAPTION, "Set Time", "Select Set Time Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9043_SETTIMEFORMAT_CAPTION, "Set Time Format", "Select Set Time Format Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9044_SETDATE_CAPTION, "Set Date", "Select Set Date Menu.");

    ADD_APPLICATION_STRING2(STR_SCR9045_SETDATEFORMAT_CAPTION, "Set Date Format", "Select Set Date Format Menu.");

    ADD_APPLICATION_STRING2(STR_24, "24", "Time Format 24.");
    ADD_APPLICATION_STRING2(STR_12, "12", "Time Format 12.");

    ADD_APPLICATION_STRING2(STR_SCR9033_AUTO_REDIAL_CAPTION, "Auto Redial", "Enter Auto Redial Menu.");

#if defined(__MMI_APHORISM__)
/* under construction !*/
/* under construction !*/
#endif /* defined(__MMI_APHORISM__) */ 

#ifdef __MMI_TVOUT__
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT, "TV Out", "STR_ID_SETTING_TVOUT");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_SETTING, "Setting", "STR_ID_SETTING_TVOUT_SETTING");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_FORMAT, "Visual Format", "STR_ID_SETTING_TVOUT_FORMAT");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_FORMAT_PAL, "PAL", "STR_ID_SETTING_TVOUT_FORMAT_PAL");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_FORMAT_NTSC, "NTSC", "STR_ID_SETTING_TVOUT_FORMAT_NTSC");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_AUD_PATH, "Audio Output", "STR_ID_SETTING_TVOUT_AUD_PATH");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_AUD_PATH_BOTH, "TV and Phone", "STR_ID_SETTING_TVOUT_AUD_PATH_BOTH");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_AUD_PATH_TV, "TV Only", "STR_ID_SETTING_TVOUT_AUD_PATH_TV");

    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_ADJUST, "Adjust", "STR_ID_SETTING_TVOUT_ADJUST");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_ADJUST_Y_GAMMA, "Gamma Y", "STR_ID_SETTING_TVOUT_MM_STYLE_PHONE");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_ADJUST_U, "U", "STR_ID_SETTING_TVOUT_MM_STYLE_FULLSCR");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_ADJUST_V, "V", "STR_ID_SETTING_TVOUT_MM_STYLE_FULLSCR");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_PLUG_IN, "TV-Out plugged in", "STR_ID_SETTING_TVOUT_PLUG_IN");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_PLUG_OUT, "TV-Out plugged out", "STR_ID_SETTING_TVOUT_PLUG_OUT");
    ADD_APPLICATION_STRING2(STR_ID_SETTING_TVOUT_NOT_READY, "TV-Out Not Ready", "STR_ID_SETTING_TVOUT_NOT_READY");
#endif /* __MMI_TVOUT__ */ 

    ADD_APPLICATION_STRING2(STR_MENU9146_ON, "Greeting Text:", "On Option.");
    ADD_APPLICATION_STRING2(STR_MENU9146_OFF, "Status", "Off Option.");
    ADD_APPLICATION_STRING2(STR_RESTORE_SUCCESS, "Setting Restored!", "Restore Success Message.");
    ADD_APPLICATION_STRING2(STR_FDL_NOT_READY, "FDN not ready.", "FDN not Ready Try Again Message.");
    ADD_APPLICATION_STRING2(STR_BDL_NOT_READY, "Please try later.", "Please try later.");
    ADD_APPLICATION_STRING2(STR_MENU_RESTORE, "Restore Factory Settings", "Restore Factory Setting Option.");
    ADD_APPLICATION_STRING2(STR_MENU_SETTINGS_ABOUT, "About", "Menu item to display the copyright information");
    ADD_APPLICATION_STRING2(
        STR_SETTING_CODE_VERIFIED,
        "Code Accepted!",
        "String associated with Code Verified Screen.");
    ADD_APPLICATION_IMAGE2(
        IMG_TIME_AND_DATE_BLANK,
        CUST_IMG_BASE_PATH "/EmptyImage.bmp",
        "Image for Time And Date Selection.");
    ADD_APPLICATION_IMAGE2(
        IMG_MENU9142_SETHOMECITY,
        CUST_IMG_BASE_PATH "/EmptyImage.bmp",
        "Image Set Home City Caption.");
    ADD_APPLICATION_IMAGE2(
        IMG_DATE_TIME_SET_FORMAT_CAPTION,
        CUST_IMG_BASE_PATH "/EmptyImage.bmp",
        "Image Date and Time Format String caption.");
    ADD_APPLICATION_IMAGE2(
        IMG_SCR_SETTING_CAPTION,
        CUST_IMG_PATH "/MainLCD/TitleBar/TB_ST.PBM",
        "Title Image For Settings Main Screen.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_CALL_SETUP,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_SCALL.PBM",
        "Setting Submenu image for Call Setup.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_NETWORK,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_SNET.PBM",
        "Setting Submenu image for Network Setup.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_PHONE,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_SPH.PBM",
        "Setting Submenu image for Phone Setup.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_STORE,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_STORE.PBM",
        "Setting Submenu image for Restore Setting.");
    ADD_APPLICATION_IMAGE2(
        IMG_SETTING_SHORTCUT,
        CUST_IMG_PATH "/MainLCD/SubMenu/Funandgame/SB_Favor.pbm",
        "Setting Submenu image for Time and Date.");
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    ADD_APPLICATION_IMAGE2(
        IMG_CALIBRATION_MENU_ICON,
        CUST_IMG_PATH "/MainLCD/SubMenu/Settings/SB_PCA.pbm",
        "Setting Submenu image for Pen calibration.");
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 

#ifndef __MMI_SUBLCD__
    ADD_APPLICATION_IMAGE2(
        IMG_CONTRAST_BACKGROUND,
        CUST_IMG_PATH "/MainLCD/UIElement/ValueMap/MAPBG.gif",
        "Background image for Contrast screen");
#endif /* __MMI_SUBLCD__ */ 

    ADD_APPLICATION_STRING2(
        STR_SETTING_RESTORE_ERR1,
        "Password is minimum 4 digits!",
        "Password is minimum 4 digits message.");
    ADD_APPLICATION_STRING2(STR_SETTING_RESTORE_CONFIRM, "ARE YOU SURE?", "Restore factory confirmation message.");
    ADD_APPLICATION_STRING2(
        STR_SETTING_RESTORE_PROCESSING,
        "Restore Processing...",
        "Restore factory confirmation message.");
    ADD_APPLICATION_STRING2(STR_SETTING_WRONG, "Wrong Password!", "Wrong.");

    ADD_APPLICATION_STRING2(STR_SETTING_BLOCK, "Password Blocked!", "Password Blocked!");

    ADD_APPLICATION_STRING2(STR_SETTING_WRONG_TIME, "Please Enter Correct Date/Time", "Notification Msg");
    ADD_APPLICATION_STRING2(STR_RESTORE_PHONE, "Enter Password", "Phone code check password.");

    PopulateNetworkSetupResData();
    populateManageCalls();
    PopulatePhoneSetupResData();
    populateSecuritySetup();
    PopulateLanguageStrings();
#ifdef __MMI_SOUND_EFFECT__
    PopulateSoundEffectResData();
#endif 

#ifdef __FLIGHT_MODE_SUPPORT__
    PopulateFlightModeResData();
#endif 
#if (defined __UNI_MULTI_SIM_CARD__)	
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_CARD_MENU ,"Select SIM Card","Select SIM Card.");
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_CARD_POPUP ,"Switch SIM card now, rebooting...","Switch SIM card now, rebooting...");
/*UNI@lizongzheng_20080201 14:08:26  双卡设置,放进快捷方式,作为候选  */
	ADD_APPLICATION_MENUITEM((MENU_SETTING_MULTI_SIM_CARD,MAIN_MENU_SETTINGS_MENUID,0,
		SHOW, SHORTCUTABLE |MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_MULTI_SIM_CARD_MENU,IMG_SETTING_NETWORK));
	#if defined(__UNI_MULTI_SIM_CARD_SERVER__)
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_CARD_SERVER ,"Dual Online","Dual Online");
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_CARD_CHANGE,"No Sim And Switch Card?","No Sim And Switch Card?");
	/*UNI@liugang_20070724 提示信息修改*/
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_CARD_DISPLAY,"Double waits the service to open please do confirm two SIM card insertion is correct!","Double waits the service to open please do confirm two SIM card insertion is correct!");
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_DISPLAY_MENU,"In idle hint dual online","In idle hint dual online");
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_FAILED_NO_SIM,"No Sim","No Sim");
    	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_FAILED_NO_NET_SIM1,"SIM1 Network nonsupport","SIM1 Network nonsupport");
    	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_FAILED_NO_NET_SIM2,"SIM2 Network nonsupport","SIM2 Network nonsupport");
	ADD_APPLICATION_STRING2(STR_ID_MULTI_SIM_SERVER_HELLP,"help","help");
	ADD_APPLICATION_STRING2(STR_ID_MULTE_SIM_MENU_HELP,"help","help");
	
	ADD_APPLICATION_MENUITEM((MENU_SETTING_MULTI_SIM_SERVER,MAIN_MENU_SETTINGS_MENUID,4,
		MENU_SETTING_SERVER_OPEN,
		MENU_SETTING_SERVER_CLOSE,
		MENU_SETTING_SERVER_POPU,
		MENU_SETTING_SERVER_HELP,
		SHOW, SHORTCUTABLE |MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_MULTI_SIM_CARD_SERVER,IMG_SETTING_NETWORK));
	ADD_APPLICATION_MENUITEM((MENU_SETTING_SERVER_OPEN,MENU_SETTING_MULTI_SIM_SERVER,0,
		SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_GLOBAL_ON,0));
	ADD_APPLICATION_MENUITEM((MENU_SETTING_SERVER_CLOSE,MENU_SETTING_MULTI_SIM_SERVER,0,
		SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_GLOBAL_OFF,0));
	ADD_APPLICATION_MENUITEM((MENU_SETTING_SERVER_POPU,MENU_SETTING_MULTI_SIM_SERVER,0,
		SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_MULTI_SIM_DISPLAY_MENU,0));
	ADD_APPLICATION_MENUITEM((MENU_SETTING_SERVER_HELP,MENU_SETTING_MULTI_SIM_SERVER,0,
		SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_MULTE_SIM_MENU_HELP,0));
	//UNI@liugang_20070802	双卡项目放在一个菜单里
	ADD_APPLICATION_MENUITEM((MENU_SETTING_MULTI_SIM_SET,MAIN_MENU_SETTINGS_MENUID,2,
		MENU_SETTING_MULTI_SIM_CARD,
		MENU_SETTING_MULTI_SIM_SERVER,
		SHOW, SHORTCUTABLE |MOVEABLEWITHINPARENT, DISP_LIST,STR_ID_MULTI_SIM_CARD_MENU,IMG_SETTING_NETWORK));
	#endif
#endif 
}
#endif

#ifndef __MMI_REMOVE_CUG__


/*****************************************************************************
 * FUNCTION
 *  PopulategrouplistMenu
 * DESCRIPTION
 *  Populate Menu for GroupList
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulategrouplistMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_MENUITEM((MAIN_MENU_GROUPLIST_OPTIONS_MENUID, 0, 5,
                              MAIN_MENU_GROUPLIST_EDIT,
                              MAIN_MENU_GROUPLIST_ADD,
                              MAIN_MENU_GROUPLIST_ACTIVATE,
                              MAIN_MENU_GROUPLIST_DEACTIVATE,
                              MAIN_MENU_GROUPLIST_DELETE, 0, NONMOVEABLE, 1, MAIN_MENU_GROUPLIST_TEXT, NULL));

    ADD_APPLICATION_MENUITEM((MAIN_MENU_GROUPLIST_EDIT, MAIN_MENU_GROUPLIST_OPTIONS_MENUID, 0, 0,
                              NONMOVEABLE, 1, STR_GLOBAL_EDIT, NULL));

    ADD_APPLICATION_MENUITEM((MAIN_MENU_GROUPLIST_ADD, MAIN_MENU_GROUPLIST_OPTIONS_MENUID, 0, 0, NONMOVEABLE, 1,
                              STR_GLOBAL_ADD, NULL));

    ADD_APPLICATION_MENUITEM((MAIN_MENU_GROUPLIST_ACTIVATE, MAIN_MENU_GROUPLIST_OPTIONS_MENUID, 0, 0, NONMOVEABLE, 1,
                              STR_GLOBAL_ACTIVATE, NULL));

    ADD_APPLICATION_MENUITEM((MAIN_MENU_GROUPLIST_DEACTIVATE, MAIN_MENU_GROUPLIST_OPTIONS_MENUID, 0, 0, NONMOVEABLE, 1,
                              STR_GLOBAL_DETACTIVATE, NULL));

    ADD_APPLICATION_MENUITEM((MAIN_MENU_GROUPLIST_DELETE, MAIN_MENU_GROUPLIST_OPTIONS_MENUID, 0, 0, NONMOVEABLE, 1,
                              STR_GLOBAL_DELETE, NULL));

    ADD_APPLICATION_MENUITEM((MAIN_MENU_GROUPLIST_OPTIONS_MENUID_SUB, IDLE_SCREEN_MENU_ID, 4,
                              MAIN_MENU_GROUPLIST_EDIT,
                              MAIN_MENU_GROUPLIST_ACTIVATE,
                              MAIN_MENU_GROUPLIST_DEACTIVATE,
                              MAIN_MENU_GROUPLIST_DELETE, 0, NONMOVEABLE, 1, MAIN_MENU_GROUPLIST_TEXT, NULL));

    ADD_APPLICATION_MENUITEM((MENU_CUG_INPUT_METHOD_AND_DONE_OPTION_ID, IDLE_SCREEN_MENU_ID, 2,
                              MENU_CUG_DONE_OPTION_ID,
                              MENU_CUG_INPUT_METHOD_OPTION_ID, 0, NONMOVEABLE, 1, MAIN_MENU_GROUPLIST_TEXT, NULL));

    ADD_APPLICATION_MENUITEM((MENU_CUG_DONE_OPTION_ID, MENU_CUG_INPUT_METHOD_AND_DONE_OPTION_ID, 0, 0, NONMOVEABLE, 1,
                              STR_GLOBAL_DONE, NULL));

    ADD_APPLICATION_MENUITEM((MENU_CUG_INPUT_METHOD_OPTION_ID, MENU_CUG_INPUT_METHOD_AND_DONE_OPTION_ID, 0, 0,
                              NONMOVEABLE, 1, STR_GLOBAL_INPUT_METHOD, NULL));

    ADD_APPLICATION_STRING2(STR_GROUPLIST_INVALID_INDEX, "Invalid index", "Invalid Index");
    ADD_APPLICATION_STRING2(MAIN_MENU_GROUPLIST_TEXT, "Closed User Group Options", "Closed User Group Options");
    ADD_APPLICATION_STRING2(STR_GROUPLIST_CAPTION, "Group List", "GroupList");
    ADD_APPLICATION_STRING2(STR_GROUPLIST_EDIT_INDEX, "Edit Index", "Index");
    ADD_APPLICATION_STRING2(STR_GROUPLIST_ADD_INDEX, "Add Index", "Index");
    ADD_APPLICATION_STRING2(STR_ACTIVATE_MESSAGE, "GroupList Activated", "GroupList Activated");
    ADD_APPLICATION_STRING2(STR_DEACTIVATE_MESSAGE, "GroupList Deactivated", "GroupList Deactivated");
    ADD_APPLICATION_STRING2(STR_GROUPLIST_DEACTIVATED, "Deactivated", "Deactivated");
    ADD_APPLICATION_STRING2(STR_GROUPLIST_OPTION_CAPTION, "Options", "Options caption");
}

#endif /* __MMI_REMOVE_CUG__ */ 

#ifdef __IP_NUMBER__


/*****************************************************************************
 * FUNCTION
 *  PopulateIPNumber
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateIPNumber(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U8 i;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_MENUITEM((MENU_SETTING_IP_NUMBER,
                              MENU8237_SCR8093_MNGCALL_MENU_MAIN, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SETTING_IP_NUMBER, NULL));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_IP_NUM_OPN, 0, 
								MMI_SIM_NUMBER+1,
								MULTI_MENU(MENU_IP_NUM_ACTIVATE), 
								MENU_IP_NUM_EDIT, 
								0, NONMOVEABLE, 1, STR_SETTING_IP_NUMBER, NULL));
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
	ADD_APPLICATION_MENUITEM((MENU_IP_NUM_ACTIVATE + i, MENU_SETTING_IP_NUM_OPN, 0,
							0, NONMOVEABLE, 1, 
							#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
							STRING_MASTER_IP_ACTIVATE + i,
							#else
							STR_GLOBAL_ACTIVATE, 
							#endif
							NULL));
	}
    ADD_APPLICATION_MENUITEM((MENU_IP_NUM_EDIT, MENU_SETTING_IP_NUM_OPN, 0, 0, NONMOVEABLE, 1, STR_GLOBAL_EDIT, NULL));

    ADD_APPLICATION_STRING2(STR_SETTING_IP_NUMBER, "IP Number", "STR_SETTING_IP_NUMBER");
}
#endif /* __IP_NUMBER__ */ 

#ifdef __MMI_CM_BLACK_LIST__


/*****************************************************************************
 * FUNCTION
 *  PopulateBlackList
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateBlackList(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_MENUITEM((MENU_SETTING_BLACK_LIST,
                              MENU8237_SCR8093_MNGCALL_MENU_MAIN, 2,
                              MENU_SETTING_BLACK_LIST_MODE,
                              MENU_SETTING_BLACK_LIST_NUM, SHOW, NONMOVEABLE, DISP_LIST, STR_SETTING_BLACK_LIST, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_BLACK_LIST_MODE, MENU_SETTING_BLACK_LIST, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SETTING_BLACK_LIST_MODE, 0));

    ADD_APPLICATION_MENUITEM((MENU_SETTING_BLACK_LIST_NUM, MENU_SETTING_BLACK_LIST, 0,
                              SHOW, NONMOVEABLE, DISP_LIST, STR_SETTING_BLACK_LIST_NUM, 0));

    ADD_APPLICATION_STRING2(STR_SETTING_BLACK_LIST, "Black List", "STR_SETTING_BLACK_LIST");
    ADD_APPLICATION_STRING2(STR_SETTING_BLACK_LIST_MODE, "Mode", "STR_SETTING_BLACK_LIST_MODE");
    ADD_APPLICATION_STRING2(STR_SETTING_BLACK_LIST_NUM, "Black List Number", "STR_SETTING_BLACK_LIST_NUM");
}
#endif /* __MMI_CM_BLACK_LIST__ */ 


/*****************************************************************************
 * FUNCTION
 *  PopulateExtImage
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateExtImage(void)
{
/* Add Wallpaper Save screen poweron off String */
ADD_APPLICATION_STRING2(STR_WALLPAPER_1,"Image 1","WallPaper");
ADD_APPLICATION_STRING2(STR_WALLPAPER_2,"Image 2","WallPaper");
ADD_APPLICATION_STRING2(STR_WALLPAPER_3,"Image 3","WallPaper");
ADD_APPLICATION_STRING2(STR_WALLPAPER_4,"Image 4","WallPaper");
ADD_APPLICATION_STRING2(STR_WALLPAPER_5,"Image 5","WallPaper");

ADD_APPLICATION_STRING2(STR_SCREENSAVER_SS1,"Screen Saver 1","Screen Saver");
ADD_APPLICATION_STRING2(STR_SCREENSAVER_SS2,"Screen Saver 2","Screen Saver");
ADD_APPLICATION_STRING2(STR_SCREENSAVER_SS3,"Screen Saver 3","Screen Saver");
ADD_APPLICATION_STRING2(STR_SCREENSAVER_SS4,"Screen Saver 4","Screen Saver");
ADD_APPLICATION_STRING2(STR_SCREENSAVER_SS5,"Screen Saver 5","Screen Saver");

ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_1,"Image 1","Power onoff Image");
ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_2,"Image 2","Power onoff Image");
ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_3,"Image 3","Power onoff Image");
ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_4,"Image 4","Power onoff Image");
ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_5,"Image 5","Power onoff Image");

//ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_SELECT_CAPTION,"Annation","Power onoff Image");
ADD_APPLICATION_STRING2(STR_ID_PHNSET_ON_OFF_ANIMATION,"Annation 1","Power onoff Image");
  
/* generatror power on/off image */
ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1,CUST_IMG_PATH"/MainLCD/Active/poweronoff/poon.gif","Power ON/OFF image");
#if !defined (__MMI_RESOURCE_SLIM__)
ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1,CUST_IMG_PATH"/MainLCD/Active/poweronoff/pooff.gif","Power ON/OFF image");
#else
ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1,CUST_IMG_PATH"/MainLCD/Active/poweronoff/poon.gif","Power ON/OFF image"); 
#endif
ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_ON_OFF_1,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall01.gif","Power ON/OFF image");
ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_ON_OFF_2,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall02.gif","Power ON/OFF image");
#if !defined(__MMI_RESOURCE_SLIM_ON_128X160__)
ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_ON_OFF_3,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall03.gif","Power ON/OFF image");
#if !defined (__MMI_RESOURCE_SLIM__)
ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_ON_OFF_4,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall04.gif","Power ON/OFF image");
ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_ON_OFF_5,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall05.gif","Power ON/OFF image");
ADD_APPLICATION_IMAGE3(IMG_ID_PHNSET_ON_OFF_6,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall06.gif","Power ON/OFF image");
#endif
#endif

/* generatror Wallpaper image */
ADD_APPLICATION_IMAGE3(WALLPAPER_1,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall01.gif","Wallpaper image");
ADD_APPLICATION_IMAGE3(WALLPAPER_2,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall02.gif","Wallpaper image");
#if !defined(__MMI_RESOURCE_SLIM_ON_128X160__)
ADD_APPLICATION_IMAGE3(WALLPAPER_3,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall03.gif","Wallpaper image");
#if !defined (__MMI_RESOURCE_SLIM__)
ADD_APPLICATION_IMAGE3(WALLPAPER_4,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall04.gif","Wallpaper image");
ADD_APPLICATION_IMAGE3(WALLPAPER_5,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall05.gif","Wallpaper image");
ADD_APPLICATION_IMAGE3(WALLPAPER_6,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall06.gif","Wallpaper image");
#endif
#endif

//ADD_APPLICATION_IMAGE3(WALLPAPER_7,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall07.gif","Wallpaper image");
//ADD_APPLICATION_IMAGE3(WALLPAPER_8,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall08.gif","Wallpaper image");

/* generatror analog/digital image */
#ifdef __IMG_ANALOG_CLOCK__
ADD_APPLICATION_IMAGE3(IMG_MAX_WALLPAPER+1,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/analog.gif","Analog Clock image");
ADD_APPLICATION_IMAGE3(IMG_MAX_WALLPAPER+2,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/Digital.gif","Digital Clock image");
#endif

/* generatror Savescreen image */
ADD_APPLICATION_IMAGE3(SCREENSAVER_1,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall01.gif","SaveScreen image");
ADD_APPLICATION_IMAGE3(SCREENSAVER_2,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall02.gif","SaveScreen image");
#if !defined(__MMI_RESOURCE_SLIM_ON_128X160__)
ADD_APPLICATION_IMAGE3(SCREENSAVER_3,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall03.gif","SaveScreen image");
#if !defined (__MMI_RESOURCE_SLIM__)
ADD_APPLICATION_IMAGE3(SCREENSAVER_4,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall04.gif","SaveScreen image");
ADD_APPLICATION_IMAGE3(SCREENSAVER_5,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall05.gif","SaveScreen image");
ADD_APPLICATION_IMAGE3(SCREENSAVER_6,CUST_IMG_PATH"/MainLCD/IdleScreen/WallPaper/wall06.gif","SaveScreen image");
#endif

#endif
}
#endif /* DEVELOPER_BUILD_FIRST_PASS */ 

