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
 *   Res_IdleScreen.c
 *
 * Project:
 * --------
 *
 * Description:
 * ------------
 *   Populate Resource for idle screen application. 
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
 *------------------------------------------------------------------------------
 *============================================================================
 ****************************************************************************/

#include "stdc.h"
#include "customcfg.h"


#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#include "idleappdef.h"
#include "gui_themes.h"
#include "wgui_categories.h"
#include "wgui_categories_defs.h"

#ifdef __MMI_VOIP__
#include "voipdef.h"
#endif

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"


/*****************************************************************************
 * FUNCTION
 *  PopulateIdleMenuRes
 * DESCRIPTION
 *  To Initialize the resources for the idle app
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateIdleMenuRes(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /* Population external resources. This must be moved later */

    PopulateStatusIconResources();
#if(ENABLE_CIRCULAR_3D_MENU)
    PopulateCircular3DMenuResources();
#endif
    PopulateCommonCategoryResources();
    PopulateThemeResources();

#if defined( __MMI_UI_ANIMATED_TITLE_EFFECT__) || defined(__MMI_UI_LIST_HIGHLIGHT_EFFECTS__)
    PopulateTitleListAnimationResources();
#endif

#ifdef __MMI_VOIP__
    ADD_APPLICATION_MENUITEM((MENU_ID_DIALER_SCREEN_OPTIONS, 0, 4,
                              MENU_ID_DIALER_SCREEN_OPTIONS_DIAL,
                              MENU_ID_DIALER_SCREEN_OPTIONS_PHB_SAVE,
                              MENU_ID_DIALER_SCREEN_OPTIONS_INPUT_METHOD,
                              MENU_ID_DIALER_SCREEN_OPTIONS_CHANGE_MODE,
                              SHOW, MOVEABLEACROSSPARENT, DISP_LIST, STR_GLOBAL_OPTIONS, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_DIALER_SCREEN_OPTIONS_DIAL, MENU_ID_DIALER_SCREEN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_DIAL, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_DIALER_SCREEN_OPTIONS_PHB_SAVE, MENU_ID_DIALER_SCREEN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_VOIP_SAVE_PHONEBOOK, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_DIALER_SCREEN_OPTIONS_INPUT_METHOD, MENU_ID_DIALER_SCREEN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_VOIP_INPUT_METHOD, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_DIALER_SCREEN_OPTIONS_CHANGE_MODE, MENU_ID_DIALER_SCREEN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ID_DIALER_SCREEN_OPTIONS_CHANGE_MODE, 0));

    ADD_APPLICATION_STRING2(STR_ID_DIALER_SCREEN_OPTIONS_GSM_DIAL, "Voice Call", "Dial by GSM");
    ADD_APPLICATION_STRING2(STR_ID_DIALER_SCREEN_OPTIONS_WLAN_DIAL, "VoIP Call", "Dial by WLAN");
    ADD_APPLICATION_STRING2(STR_ID_DIALER_SCREEN_OPTIONS_CHANGE_MODE, "Change Dial Mode", "Change Dial Mode");
#endif /* __MMI_VOIP__ */

    ADD_APPLICATION_IMAGE2(RADIO_ON_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIELement/Radiobutton/RO_S.BMP","Image associated with Radio On.");
    ADD_APPLICATION_IMAGE2(RADIO_OFF_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIELement/Radiobutton/RO_UNS.BMP","Image associated with Radio Off.");
    ADD_APPLICATION_IMAGE2(CHECKBOX_ON_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIELement/Checkbox/CK_S.BMP","Image associated with CheckBox On.");
    ADD_APPLICATION_IMAGE2(CHECKBOX_OFF_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIELement/Checkbox/CK_UNS.BMP","Image associated with CheckBox Off.");
    ADD_APPLICATION_IMAGE2(IMG_IDLE_NETWORK_SRCH_SCR,	CUST_IMG_PATH"/MainLCD/Active/Searching/SEARCH.gif","Img Associated with Network Search Screen.");
    ADD_APPLICATION_IMAGE2(TEMP_SUBLCD_ANIMATION_IMAGE_ID,CUST_IMG_PATH"/SUBLCD/Active/SB_ALARM.GIF","Image associated with Temp Scr of Sub LCD");

    //ADD_APPLICATION_IMAGE2(UNDER_CONSTRUCTION_IMAGE_ID,CUST_IMG_BASE_PATH"/UnderC.bmp","Image assciated for the screen under construction.");

#if defined(__MMI_TOUCH_IDLESCREEN_SHORTCUTS__)
    ADD_APPLICATION_IMAGE2(IMG_IDLE_MAINMENU_SHORCUT,CUST_IMG_PATH"/MainLCD/IdleScreen/Touch/TH_MM_UP.gif","Idle Screen Main Menu shorcut image.");
    ADD_APPLICATION_IMAGE2(IMG_IDLE_MAINMENU_SHORCUT_PRESSED,CUST_IMG_PATH"/MainLCD/IdleScreen/Touch/TH_MM_DOWN.gif","Idle Screen Main Menu shorcut pressed image.");
    ADD_APPLICATION_IMAGE2(IMG_IDLE_MESSAGE_SHORTCUT,CUST_IMG_PATH"/MainLCD/IdleScreen/Touch/TH_MG_UP.gif","Idle Screen Message shorcut image.");
    ADD_APPLICATION_IMAGE2(IMG_IDLE_MESSAGE_SHORTCUT_PRESSED,CUST_IMG_PATH"/MainLCD/IdleScreen/Touch/TH_MG_DOWN.gif","Idle Screen Message shorcut pressed image.");
    ADD_APPLICATION_IMAGE2(IMG_IDLE_PHONEBOOK_SHORTCUT,CUST_IMG_PATH"/MainLCD/IdleScreen/Touch/TH_PB_UP.gif","Idle Screen Phonebook shorcut image.");
    ADD_APPLICATION_IMAGE2(IMG_IDLE_PHONEBOOK_SHORTCUT_PRESSED,CUST_IMG_PATH"/MainLCD/IdleScreen/Touch/TH_PB_DOWN.gif","Idle Screen Phonebook shorcut pressed image.");
    ADD_APPLICATION_IMAGE2(IMG_IDLE_DIALING_SCREEN_SHORTCUT,CUST_IMG_PATH"/MainLCD/IdleScreen/Touch/TH_DS_UP.gif","Idle Screen Dialing Screen shorcut image.");
    ADD_APPLICATION_IMAGE2(IMG_IDLE_DIALING_SCREEN_SHORTCUT_PRESSED,CUST_IMG_PATH"/MainLCD/IdleScreen/Touch/TH_DS_DOWN.gif","Idle Screen Dialing Screen shorcut pressed image.");
#endif /* __MMI_TOUCH_IDLESCREEN_SHORTCUTS__ */

    ADD_APPLICATION_STRING2(TEMP_SUBLCD_ANIMATION_STR_ID, "Hello", "String associated with Temp Scr of Sub LCD.");

    ADD_APPLICATION_STRING2(DIAL_PAD_MO_CALL_LSK_STR_ID, "Save", "String associated with Dial pad screen LSK.");
    ADD_APPLICATION_STRING2(IDLE_SCREEN_LSK_TEXT, "Menu", "String associated with Idle screen LSK.");
    ADD_APPLICATION_STRING2(IDLE_SCREEN_RSK_TEXT, "Name", "String associated with Idle screen RSK.");

    ADD_APPLICATION_STRING2(KEYPAD_LOCKED_RSK_TEXT, "Unlock", "String associated with Keypad Locked Idle screen LSK.");

#ifdef __MMI_TOUCH_SCREEN__
    ADD_APPLICATION_STRING2(SCREEN_LOCKED_TEXT, "Locked", "String associated with Screen Locked Idle screen for touch screen.");
#endif

    ADD_APPLICATION_STRING2(STR_IDLE_NETWORK_SRCH_SCR, "Searching", "String Associated with Network Search Screen.");

    ADD_APPLICATION_STRING2(STR_INVALID_LOCATION, "Invalid Location", "String Associated with Invalid location Screen.");

    ADD_APPLICATION_STRING2(STR_NO_PHONE_NUM, "No Phone Book entry", "String Associated with Number Not In Phone Book Screen.");

    ADD_APPLICATION_STRING2(STR_NO_SPEED_DIAL_NUM_SAVED, "No Speed Dial Number Saved", "String Associated with No Speed Dial Number Saved Screen.");
#if defined( __MMI_DATE_TIME_SETTING_REMINDER__)
    ADD_APPLICATION_STRING2(STR_DATE_TIME_UPDATE_REMINDER, "Please Set Date and Time", "String Associated with Date and Time reminder");
#endif

#ifdef __MMI_SUBLCD_MASTER_MODE__
    ADD_APPLICATION_STRING2(STR_IDLE_DUMMY_SCREEN, "Exit SubLCD......", "For SubLCD master mode");
#endif

#ifdef __UNI_MMI_IDLE_SLIDE_UNLOCK__ /*  ���Ӵ����������*/
    ADD_APPLICATION_IMAGE2(WGUI_IDLE_SLIDE_UNLOCK_BACKGROUND,CUST_IMG_PATH"/MainLCD/IdleScreen/Touch/TH_SLIDE_BG.gif","Idle Screen Slide Background.");
    ADD_APPLICATION_IMAGE2(WGUI_IDLE_SLIDE_UNLOCK_SLIDER,CUST_IMG_PATH"/MainLCD/IdleScreen/Touch/TH_SLIDER.gif","Idle Screen Slider.");
#endif
}


/*****************************************************************************
 * FUNCTION
 *  PopulateMainDemoRes
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void PopulateMainDemoRes(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ADD_APPLICATION_STRING(DEMO_UNER_CONS_TEXT, "Under Construction");
    ADD_APPLICATION_STRING(DEMO_ICON_TEXT, "Pixtel");
    ADD_APPLICATION_IMAGE(DEMO_CONSTRUCTION_IMAGE_ID, CUST_IMG_BASE_PATH"/UnderC.bmp");
    ADD_APPLICATION_IMAGE(DEMO_UNDER_CONSTRUCTION_TITLE_ICON, CUST_IMG_BASE_PATH"/TitleIcon.bmp");
}

#endif


