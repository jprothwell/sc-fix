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
 * USBDevice.c
 *
 * Project:
 * --------
 *   MMI
 *
 * Description:
 * ------------
 *   This file is for mmi configure usb device
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#include "mmi_features.h" 

#ifdef __MMI_USB_SUPPORT__

#include "bootup.h"
#include "globalconstants.h"

#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "timerevents.h"
#include "eventsgprot.h"
#include "eventsdcl.h"
#include "protocolevents.h"
#include "commonscreens.h" /* displaypopup */
#include "wgui_categories.h"
#include "wgui_categories_idlescreen.h"  /* showcategory106screen */
#include "wgui_categories_popup.h" /* showcategory162screen, showcategory64screen */
#include "wgui_status_icons.h"  /* status_icon_sublcd_battery_strength */
#include "globaldefs.h"
#include "globalscrenum.h"
#include "historygprot.h" 
#include "simdetectiongexdcl.h"  /* to access g_charbat_context */
#include "idleappprot.h"
#include "eventsdcl.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "allappgprot.h"
#include "taskinit.h"
#include "mmitaskprot.h" /* initframework */
#include "sublcdhistorygprot.h"
#include "mmi_msg_struct.h"
#ifdef __MMI_IRDA_SUPPORT__   
#include "irdammigprots.h" /* mmi_irda_is_open() */
#endif

/* Trace Header Files */
#include "debuginitdef.h" /* define trace class type */

#include "phonesetupgprots.h"

/* USB Device Headder Files */
#include "extdevicedefs.h"
#include "usbdevicegprot.h"
#include "usbdevicedefs.h"
#include "usbdevicegexdcl.h"
#include "mmi_trace.h"
#ifdef __MMI_ENGINEER_MODE__
#include "engineermodedef.h" 
#endif

#undef __NEWSIMULATOR 

 
#ifdef MMI_ON_HARDWARE_P
#include "lcd_sw.h" 
#include "l4dr.h"
#include "uart_sw.h"
#include "lcd_sw.h" 
#include "lcd_sw_inc.h"
#include "gdi_include.h"
#include "lcd_if.h"
#else
#include "extdevicedummyinc.h"
#endif
#include "gpioinc.h"

#ifdef __MMI_WEBCAM__
#include "mdi_datatype.h"
#include "mdi_webcam.h"
#include "idleappdef.h" 						/* for popup_screenid */
#include "sublcdhistorygprot.h"				/* sublcd */
#include "wgui_categories_multimedia.h"	/* cat331 */
#include "keybrd.h"								/* keypad tone */
#include "usbvideo_drv.h"
#include "uvideos_stream_m.h"
#endif /* __MMI_WEBCAM__ */
#ifdef __J2ME__
#include "javaprot.h"
#endif
#include "filesystemdef.h"
#include "settingprofile.h"
#include "usb_interface.h"
#include "filemgr.h"
#ifdef MMI_ON_HARDWARE_P	
#ifdef __MMI_USB_DOWNLOAD_FR__
#include "uctls_m.h"
#endif
#endif
#ifdef __MMI_FM_RADIO__
extern void mmi_fmrdo_search_all_channels_stop( void );
#endif /* __MMI_FM_RADIO__ */


// FIXME Once the adaptation layer has a header file,
// use it for this variable ?
// IDEALLY : call a function from adaptation to do that:
// intermodules global variable are completly forbidden
extern BOOL g_hasMountedTFlashFS;

/***************************************************************************** 
* Define
*****************************************************************************/
#define WEBCAM_CLAM_OPEN_ROTATE 		MDI_WEBCAM_PREVIEW_ROTATE_0
#define WEBCAM_CLAM_CLOSE_ROTATE		MDI_WEBCAM_PREVIEW_ROTATE_0
		
#define MAX_DEINIT_CALLBACK   sizeof(mmi_usb_deinit_callback)/sizeof(mmi_usb_ms_callback_struct) - 1

/***************************************************************************** 
* Typedef
*****************************************************************************/
typedef U8(*ChkFuncPtr) (void);

typedef struct
{
    U16 eventID;
    PsFuncPtr RspHdlr;
    FuncPtr callback;
    ChkFuncPtr checkfunc;
} mmi_usb_ms_callback_struct;

/***************************************************************************** 
* Extern Variable
*****************************************************************************/
extern U8 subMenuData[MAX_SUB_MENUS][MAX_SUB_MENU_SIZE];
extern PU8 subMenuDataPtrs[MAX_SUB_MENUS];
extern charbat_context_struct g_charbat_context;

/***************************************************************************** 
* Extern Functions
*****************************************************************************/
extern U16 	GetTotalCallCount(void);
extern void ClearKeyEvents(void);
extern void PopulateResData(void);
extern void InitUnicodeSupport(void);
extern U8 	AlmIsRTCPwron(void);
extern BOOL IsClamClose(void);
extern void SetClamState(U16 clam_state);
extern void StopPowerOnOffLED(void);
#ifdef __MMI_AUDIO_PLAYER__
extern void mmi_audply_stop_playing(void);
extern void mmi_audply_do_single_stop_action(void);
#endif /* __MMI_AUDIO_PLAYER__ */ 
/***************************************************************************** 
* Local Variable
*****************************************************************************/
MMI_USB_CONTEXT g_mmi_usb_cntx;
MMI_USB_CONTEXT* mmi_usb_ptr = &g_mmi_usb_cntx;
static S32 highlightedItemIndex = 0;

/***************************************************************************** 
* Local Functions
*****************************************************************************/
#ifdef __MMI_WEBCAM__
static void mmi_usb_webcam_exit_app_scr(void);
static void mmi_usb_webcam_event_hdlr(MDI_RESULT ret, BOOL camera_workable);
static U8 mmi_usb_webcam_del_screen_id_hdlr(void *ptr);
static void mmi_usb_webcam_rsk_release(void);
static void mmi_usb_webcam_capture_key_press(void);
static void mmi_usb_webcam_plug_out(void);


#ifdef __MMI_SUBLCD__
static void mmi_usb_webcam_entry_sublcd_scr(void);
static void mmi_usb_webcam_exit_sublcd_scr(void);
#endif /* __MMI_SUBLCD__ */ 

#if 0 // defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__)
static U8 mmi_usb_webcam_gpio_event_hdlr(mmi_frm_int_event_type interrupt_event);
static U8 mmi_usb_webcam_gpio_post_event_hdlr(mmi_frm_int_event_type interrupt_event);

#endif /* defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__) */ 
#endif /* __MMI_WEBCAM__ */ 

static void mmi_usb_app_deinit(void *p);

/***************************************************************************** 
* Local Variable
*****************************************************************************/
static mmi_usb_ms_callback_struct mmi_usb_deinit_callback[] = 
{
#if 0
	{0, NULL, PhnsetWPUseDefault, NULL},		/* suspend wallpaper */
	{0, NULL, PhnsetSSUseDefault, NULL},		/* suspend screensaver */
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
	{0, NULL, PhnsetPwrOnOffUseDefault, NULL},	/* suspend power on/off disp. */
#endif
#endif
#ifdef __MMI_AUDIO_PLAYER__
	{0, NULL, mmi_audply_stop_playing, NULL},
	{0, NULL, mmi_audply_do_single_stop_action, NULL},
#endif /* __MMI_AUDIO_PLAYER__ */ 
#if 0
#ifdef __MMI_EMAIL__
	{MSG_ID_MMI_EMAIL_DEINIT_RSP, mmi_email_ps_deinit_rsp, mmi_email_deinit, mmi_email_check_to_deinit},
#endif 
	{0, NULL, fmgr_init_drive, NULL},
#ifdef MMS_SUPPORT
	{MSG_ID_MMS_ENTER_USB_MODE_RSP, NULL, mms_on_enter_usb_mode, mmi_usb_common_check_to_deinit},
#endif 
#ifdef __MMI_VIDEO_RECORDER__
	{0, NULL, mmi_vdorec_usb_mode_hdlr, NULL},
#endif 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	{0, NULL, mmi_tm_init_theme_usb_mode_plugin_callback, NULL},
#endif 
#ifdef __MMI_EBOOK_READER__
	{0, NULL, mmi_ebook_usb_mode_on, NULL},
#endif
#endif
#ifdef __J2ME__
    {0, NULL, jam_enter_usb_mode, (ChkFuncPtr)jam_usb_mode_check},
#endif
	/* add callback before this */
	{0, NULL, NULL, NULL}
};
static U8 counter_of_deinit = 0;

static BOOL usbIsExist = FALSE; //chenhe add tmp for jasper

extern void InitHardwareEvents(void);

/*****************************************************************************
* FUNCTION
*  InitializeUsbScr
*
* DESCRIPTION
*   This function is initial function for usb boot mode
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED
*
*****************************************************************************/

void InitializeUsbScr(void)
{
	InitUnicodeSupport();

#ifdef MMI_ON_HARDWARE_P
   gdi_init();  /* Init graphic lib */
#endif
     
   InitMMIUsbContext();
   SetUsbStatus( MMI_USBSTATUS_MS );
      
	InitFramework();
	InitHardwareEvents();
	InitUsbModeEventHandler();
	setup_UI_wrappers();
	PopulateResData();
	initialize_UI_demo();
	ShowUsbScr();
   /* Enter USB mode will begin charge itself */
   /* Turn this flag on to prevent backlight off */
	g_charbat_context.isChargerConnected = 1;
	//g_charbat_context.PowerOnCharger=1;
	TurnOnBacklight(1);
}


kal_bool INT_USBBoot(void)
{
	return FALSE;
}
/*****************************************************************************
* FUNCTION
*  InitMMIUsbContext
*
* DESCRIPTION
*   This function is to initialize the mmi usb context.
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED
*
*****************************************************************************/

void InitMMIUsbContext(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if( MMIUSB_GET_FLAG(MMIUSB_MASK_BEFORE_POWERON) == MMI_TRUE )
   {
         /* Usb has been pluged in before power on */
         SetUsbStatus( MMI_USBSTATUS_PLUGIN );
         MMI_USB_CTX(usb_status_prev) = MMI_USBSTATUS_PLUGOUT;
   }
   else
   {
         /* Usb is not pluged in */
         SetUsbStatus( MMI_USBSTATUS_PLUGOUT );
         MMI_USB_CTX(usb_status_prev) = MMI_USBSTATUS_UNKNOW;
   }

   MMI_USB_CTX(ps_port_prev) = uart_port1;
   MMI_USB_CTX(tst_port_prev) = uart_port2;
   MMI_USB_CTX(EndKeyFuncPtr) = NULL;

	#ifdef __MMI_WEBCAM__
		mmi_usb_ptr->webcam_state 				= MMI_USB_WEBCAM_DISCONNECT;
		mmi_usb_ptr->webcam_is_sub_display 	= FALSE;
		mmi_usb_ptr->webcam_is_clam_close	= FALSE;
	#endif /* __MMI_WEBCAM__ */
      
}


/*****************************************************************************
* FUNCTION
*  InitUsbModeEventHandler
*
* DESCRIPTION
*   This function is to initialize the event handler
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: None
*
*****************************************************************************/
void L4KeyHandle(void *paraBuff)
{
}
void InitUsbModeEventHandler(void)
{
	mmi_trace(1,"InitUsbModeEventHandler");
#ifdef MMI_ON_HARDWARE_P
	//SetProtocolEventHandler(L4KeyHandle,MSG_ID_MMI_EQ_KEYPAD_DETECT_IND);
#endif

	//SetProtocolEventHandler(UsbDetectIndHdlr, PRT_EQ_USB_DETECT_IND );
	//SetProtocolEventHandler(UsbBatStatusIndHdlr, PRT_BATTERY_STATUS_IND);
	//SetProtocolEventHandler(UsbGpioDetectIndHdlr, PRT_EQ_GPIO_DETECT_IND);
#ifdef __MMI_USB_COPY_RIGHT__	
	//SetProtocolEventHandler(UsbPowerOnFinishHdlr, PRT_EQ_USB_POWER_ON_FINISH_IND);
#endif
	
   //SetProtocolEventHandler(UsbConfigRspHdlr, PRT_EQ_USBCONFIG_RSP );
}



/* ===========================================================================================  */
/* ===  Query Functions  */
/* ===========================================================================================  */

/*****************************************************************************
* FUNCTION
*  GetUsbStatus
*
* DESCRIPTION
*   This function is to get the usb state
*
* PARAMETERS: void
*
* RETURNS: enum MMI_USB_STATUS
*
* GLOBALS AFFECTED
*
*****************************************************************************/

MMI_USB_STATUS GetUsbStatus(void)
{
     MMI_TRACE((MMI_TRACE_STATE, MMI_USB_CUR_STATE, MMI_USB_CTX(usb_status) ));

     return MMI_USB_CTX(usb_status);
}


/*****************************************************************************
* FUNCTION
*  SetUsbStatus
*
* DESCRIPTION
*   This function is to set the usb state
*
* PARAMETERS: enum MMI_USB_STATUS
*
* RETURNS: void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
extern BOOL g_UsbChargerIn;
void SetUsbStatus( MMI_USB_STATUS state )
{
/*
    UINT8 tflash[20] = {0};

    MMI_TRACE((MMI_TRACE_STATE, MMI_USB_STATE_TRANSITION, MMI_USB_CTX(usb_status), state ));

    AnsiiToUnicodeString(tflash, TFLASH_ROOT);

    switch(state)
    {
    case MMI_USBSTATUS_PLUGIN:
        // Umount FS, access is reserved for remote OS
        // FIXME: Critical section/semaphore here ?
        g_hasMountedTFlashFS = FALSE;
        FS_Unmount(tflash, TRUE);
        break;
    case MMI_USBSTATUS_PLUGOUT:
        // Remount FS, access is reserved internal 
        // FIXME: Critical section/semaphore here ?
        FS_Mount("mmc0", tflash, 0, FS_TYPE_FAT);
        g_hasMountedTFlashFS = TRUE;
        break;
    default:
        break;
    }
*/
	g_UsbChargerIn = (MMI_USBSTATUS_MS_PROC == state || MMI_USBSTATUS_WEBCAM == state )?TRUE:FALSE;
    MMI_USB_CTX(usb_status) = state;
}


/*****************************************************************************
* FUNCTION
*  GetUsbDefAppPortSetting
*
* DESCRIPTION
*   This function is to get defined usb port app
*
* PARAMETERS: enum MMI_USB_STATUS
*
* RETURNS: void
*
* GLOBALS AFFECTED
*
*****************************************************************************/

U8 GetUsbDefAppSetting(void)
{
   S16 pError;
   U8 app;

   ReadValue(NVRAM_DEFAULT_USB_APP, &app, DS_BYTE, &pError);
   
   if( pError != NVRAM_READ_SUCCESS )
      app = USB_SWITCH_PORT_APP_DATA;
   else if( app == 0xFF )
   {
      app = USB_SWITCH_PORT_APP_DATA;
      WriteValue( NVRAM_DEFAULT_USB_APP, &app, DS_BYTE, &pError );
   }

   ASSERT( (app == USB_SWITCH_PORT_APP_DATA) || (app == USB_SWITCH_PORT_APP_DEBUG) );
   
   return app;
}


/*****************************************************************************
* FUNCTION
*  SetUsbBeforePowerOnFlag
*
* DESCRIPTION
*   This function is to set usb plug in/out flag before power on.
*   (The usb detection indication might come to mmi before power on indication)
*
* PARAMETERS: void
*
* RETURNS: enum MMI_USB_STATUS
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void SetUsbBeforePowerOnFlag(BOOL flag)
{
     if( flag == MMI_TRUE )
       MMIUSB_SET_FLAG(MMIUSB_MASK_BEFORE_POWERON);
     else
       MMIUSB_RESET_FLAG(MMIUSB_MASK_BEFORE_POWERON);
}



/*****************************************************************************
* FUNCTION
*  PermitUsbCfgScreenDisplay
*
* DESCRIPTION
*   This function check the if usb config screen display allowed or not.
*
* PARAMETERS: void
*
* RETURNS: T/F
*
* GLOBALS AFFECTED
*
*****************************************************************************/
BOOL PermitUsbCfgScreenDisplay(void)
{
     if( IdleIsPowerOnPeriod() == 1 /* During Power on */ 
         || GetTotalCallCount() > 0 /* During Call */ 
         || IsPowerOffPeriod() == MMI_TRUE )
     {
       MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_CONF_SCR_NOT_ALLOW ));
       return MMI_FALSE;
     }
     else
     {
       MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_CONF_SCR_ALLOW));
       return MMI_TRUE;
     }
}





/* ============================================================================= */
/* ===  Screen Handling Functions  */
/* ============================================================================= */
/*                                                                               */  
/*  USB device config screen flow:                                               */
/*                                                                               */  
/*  (1) Plug in                                                                  */
/*  ===============                                                              */
/*  == 1. MS     ==                                                              */
/*  == 2. COM    ==                                                              */
/*  ==           ==                                                              */
/*  ==           ==                                                              */
/*  ==           ==                                                              */
/*  ==           ==                                                              */
/*  ===============                                                              */
/*  == OK  = BACK==                                                              */
/*  ===============                                                              */
/*                                                                               */  
/*   --> UsbDetectIndHdlr()                                                      */  
/*     --> EntryUsbDetectQueryScr()                                              */  
/*                                                                               */  
/*                                                                               */  
/*                                                                               */  
/*  (2) Mass storage:                                                            */
/*  ================         ===============          ===============            */
/*  ==            ==         ==           ==          ==           ==            */
/*  == ********** ==         ==  *******  ==          ==     **    ==            */
/*  == * Shut-  * ==         == ********* ==          ==    **     ==            */
/*  == *  Down  * ==         == **+***+** ==          ==   *USB    ==            */
/*  == *  ....  * ==  --->   == ********* ==   --->   ==  ****     ==            */
/*  == *        * ==         == ***&&&*** ==          ==   *MODE   ==            */
/*  == ********** ==         ==  *******  ==          ==   **      ==            */
/*  ==            ==         ==   *****   ==          ==   *  **   ==            */
/*  ================         == Animation ==          ==     **    ==            */
/*  == OK  = BACK ==         ==   .....   ==          ==      **   ==            */
/*  ================         ===============          ===============            */
/* ( Message exchanging      ( Shutdown Ani.)        (Reboot in USB Mode)        */  
/* trasition screen 3 sec )                                                      */  
/*                                                                               */  
/*   -> UsbDetectScrLskHdlr()                           ->InitializeUsbScr()     */  
/*    -> UsbConfigFirstPage()                                                    */  
/*     -> UsbConfigRspHdlr()                                                     */  
/*      -> UsbConfigMassStorage()                                                */  
/*                                                                               */  
/*                          --> ShutdownSystemOperation()                        */  
/*                                                                               */  
/*                                                                               */  
/*                                                                               */  
/*  (2) COM port emulator:                                                       */
/*  ================         ===============                                     */
/*  ==           ===         ==           ==                                     */
/*  == ********** ==         ==  Back     ==                                     */
/*  == * Config * ==         ==       to  ==                                     */
/*  == *  ..... * ==         ==  History  ==                                     */
/*  == *        * ==  --->   ==    Screen ==                                     */
/*  == *        * ==         ==           ==                                     */
/*  == ********** ==         ==           ==                                     */
/*  ==            ==         ==           ==                                     */
/*  ================         ==           ==                                     */
/*  == OK  = BACK ==         ==           ==                                     */
/*  ================         ===============                                     */
/* ( Message exchanging      ( Shutdown Ani.)                                    */  
/* trasition screen 3 sec )                                                      */  
/*                                                                               */  
/*   -> UsbDetectScrLskHdlr()                                                    */
/*    -> UsbConfigFirstPage()                                                    */  
/*     -> UsbConfigRspHdlr()                                                     */  
/*      -> UsbConfigCDCACM()                                                     */  
/*       -> SendUsbGetUartPortToHW()                                             */  
/*        -> UsbGetUartPortRspHdlr()                                             */  
/*         -> SendUsbUartSwitchToHW()                                            */  
/*          -> UsbUartSwitchRspHdlr()                                            */  
/*           -> GoBackHistory()                                                  */  
/*                                                                               */  
/*                                                                               */  
/* ============================================================================= */


#ifdef   __MMI_SUBLCD__
/*****************************************************************************
* FUNCTION
*  ShowSubLCDUsbScr
*
* DESCRIPTION
*   This function is to show sublcd screen for usb boot mode
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED : none
*
*****************************************************************************/
void ShowSubLCDUsbScr(void)
{
	ExecSubLCDCurrExitHandler();
	ShowCategory304Screen(IMG_SUB_USB_POWER_ON,0);
	AnimateStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);	
	UpdateStatusIcons();
	SetSubLCDExitHandler(ExitSubLCDUsbScr);
}


/*****************************************************************************
* FUNCTION
*  ExitSubLCDUsbScr
*
* DESCRIPTION
*   This function is to exit handler for sublcd screen in usb boot mode
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void ExitSubLCDUsbScr(void)
{
	SubLCDHistoryNode SubLCDHistory;
	SubLCDHistory.entryFuncPtr=ShowSubLCDUsbScr;
	AddSubLCDHistory(&SubLCDHistory);
}

/*****************************************************************************
* FUNCTION
*  ShowSubLCDUsbChgrCmpltScr
*
* DESCRIPTION
*   This function is to show sublcd charging complete screen for usb boot mode
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED : none
*
*****************************************************************************/
void ShowSubLCDUsbChgrCmpltScr(void)
{
	ExecSubLCDCurrExitHandler();
	ShowCategory304Screen(IMG_USB_SUB_CHGR_CMPLT,0);
	HideStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);	
	ShowStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);	
	UpdateStatusIcons();
	SetSubLCDExitHandler(ExitSubLCDUsbChgrCmpltScr);
}


/*****************************************************************************
* FUNCTION
*  ExitSubLCDUsbChgrCmpltScr
*
* DESCRIPTION
*   This function is to exit handler for sublcd charging complete screen in usb boot mode
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void ExitSubLCDUsbChgrCmpltScr(void)
{
	SubLCDHistoryNode SubLCDHistory;
	SubLCDHistory.entryFuncPtr=ShowSubLCDUsbChgrCmpltScr;
	AddSubLCDHistory(&SubLCDHistory);
}


#endif /* __MMI_SUBLCD__ */



/*****************************************************************************
* FUNCTION
*  ShowUsbScr
*
* DESCRIPTION
*   This function is to display main screen for usb boot mode 
*
* PARAMETERS: void
*
* RETURNS: none
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void ShowUsbChgCmpltScr(void)
{
	EntryNewScreen(SCR_USB_CHGR_CMPLT, NULL, ShowUsbChgCmpltScr, NULL);
	DinitHistory();
	ShowCategory9Screen(0, IMG_USB_CHGR_CMPLT, NULL);

#ifdef __MMI_SUBLCD__
	ChangeStatusIconLevel(STATUS_ICON_SUBLCD_BATTERY_STRENGTH,100);
	ShowSubLCDScreen(ShowSubLCDUsbChgrCmpltScr);
#endif

   ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
   SetKeyHandler(UsbPwnOnThenPwnOn,KEY_END,KEY_LONG_PRESS);

}



/*****************************************************************************
* FUNCTION
*  ShowUsbScr
*
* DESCRIPTION
*   This function is to display main screen for usb boot mode 
*
* PARAMETERS: void
*
* RETURNS: none
*
* GLOBALS AFFECTED
*
*****************************************************************************/

void ShowUsbScr(void)
{
	EntryNewScreen(SCR_USB_POWER_ON, NULL, ShowUsbScr, NULL);
	
	DinitHistory();
   ShowCategory65Screen((U8*)GetString(STR_USB_POWER_ON_PROCESSING),0,NULL);
      MCI_USBConnect();

#if 0
#ifdef __MMI_USB_COPY_RIGHT__
   ShowCategory65Screen((U8*)GetString(STR_USB_POWER_ON_PROCESSING),0,NULL);
#else
  // ShowCategory106Screen(IMG_USB_POWER_ON, NULL, NULL);
#endif   
   #endif
#ifdef   __MMI_SUBLCD__
   ShowSubLCDScreen(ShowSubLCDUsbScr);
#endif

   SetKeyHandler(UsbPwnOnThenPwnOn,KEY_END,KEY_LONG_PRESS);
}




/*****************************************************************************
* FUNCTION
*  EntryUsbDetectQueryScr
*
* DESCRIPTION
*   This function is to display "Usb Config:" screen as received USB_DETECT_IND
*
* PARAMETERS: void
*
* RETURNS: none
*
* GLOBALS AFFECTED
*
*****************************************************************************/

void EntryUsbDetectQueryScr(void)
{
   return;
   #if 0
   U8* guiBuffer;
   U8 *listItemsIcons[8];
   S32 idx;

   MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_DETECT_QUERY_SCR));

   /*************************************************/
   /* Turn on backlight before execute exit handler */
   /* Turn on backlight will blt out the LCD buffer */
   /* If execute exit handler before turn on BL     */
   /* migh see the blue background because turn on  */
   /* BL only blt out layer 0, in idle screen,      */
   /* layer 0 is not wallpaper, wallpaper locate at */
   /* layre 1.                                      */
   /*************************************************/
   TurnOnBacklight(1);
   /* EntryNewScreen will clear all key handler */
   MMI_USB_CTX(EndKeyFuncPtr) = GetKeyHandler(KEY_END ,KEY_EVENT_DOWN) ;

   EntryNewScreen(SCR_DEV_USBDETECT, ExitUsbDetectQueryScr, EntryUsbDetectQueryScr, NULL);

#if defined( __MMI_UI_STYLE_3__) || defined(__MMI_UI_STYLE_4__) 
   entry_full_screen();
#endif

   /* Set mmi usb status */
	MMI_USB_CTX(usb_status_prev) = GetUsbStatus();
	SetUsbStatus( MMI_USBSTATUS_CONF_PAGE );

	guiBuffer = GetCurrGuiBuffer (SCR_DEV_USBDETECT);		 

#if (defined(__MMI_USB_CDCACM_ONLY__) || defined(__MMI_USB_MS_ONLY__) )
   {
      U16 str_title;

#ifdef __MMI_USB_CDCACM_ONLY__

	/* CDCACM Mode only */
      highlightedItemIndex = MMI_USB_CONFIG_MENU_CDCACM;
      str_title = STR_USB_CONFIG_CDCACM_ONLY;

      #else /* __MMI_USB_MS_ONLY__ */

      /* MS Mode only */
      highlightedItemIndex = MMI_USB_CONFIG_MENU_MS;
      str_title = STR_USB_CONFIG_MS_ONLY;
      
      #endif  /* __MMI_USB_CDCACM_ONLY__ */

     ShowCategory165Screen( STR_GLOBAL_YES, IMG_GLOBAL_YES, \
                                                STR_GLOBAL_NO, IMG_GLOBAL_NO,  \
                             get_string(str_title), IMG_GLOBAL_QUESTION,0);
   }

#else 

    /* Both Mass Storage and CDCACM mode */
   if( g_pwr_context.PowerOnMode == POWER_ON_KEYPAD \
       || g_pwr_context.PowerOnMode == POWER_ON_EXCEPTION )
   {

		/* If normal power on, display selection menus (MS/COM) */

    RegisterHighlightHandler(UsbDetectScrHlightHdlr);

    memset(listItemsIcons,0,sizeof(listItemsIcons));
    memset(subMenuDataPtrs,0,sizeof(subMenuDataPtrs));

    idx = 0;
    subMenuDataPtrs[idx++] = (PU8) GetString(STR_USB_CONFIG_MS);
  #if 0  
	 #ifdef __MMI_WEBCAM__
	    subMenuDataPtrs[idx++] = (PU8) GetString(STR_ID_USB_CONFIG_WEBCAM);
	 #endif /* __MMI_WEBCAM__ */
    
    subMenuDataPtrs[idx++] = (PU8) GetString(STR_USB_CONFIG_CDCACM_DATA);
#endif //chenhe for jasperII

#if 1
    ShowCategory32Screen( (PU8)GetString(STR_GLOBAL_OK),NULL,
							(PU8)GetString(STR_GLOBAL_OK),(PU8)GetImage(IMG_GLOBAL_OK),
							(PU8)GetString(STR_GLOBAL_BACK),(PU8)GetImage(IMG_GLOBAL_BACK),
							idx, subMenuDataPtrs,
							listItemsIcons,0, 0, guiBuffer);
#else
    ShowCategory32Screen( (PU8)GetString(STR_USB_CONFIG_TITLE),NULL,
							(PU8)GetString(STR_GLOBAL_OK),(PU8)GetImage(IMG_GLOBAL_OK),
							(PU8)GetString(STR_GLOBAL_BACK),(PU8)GetImage(IMG_GLOBAL_BACK),
							idx, subMenuDataPtrs,
							listItemsIcons,0, 0, guiBuffer);
#endif 
   	}
    else if ( g_pwr_context.PowerOnMode != POWER_ON_USB )
   	{
		/* If alarm/charger power on, display MS config confirm */

       ShowCategory165Screen( STR_GLOBAL_YES, IMG_GLOBAL_YES, \
                                                STR_GLOBAL_NO, IMG_GLOBAL_NO,  \
                             get_string(STR_USB_CONFIG_MS_ONLY), IMG_GLOBAL_QUESTION,0);
    }

#endif /* __MMI_USB_CDCACM_ONLY__ */

    SetLeftSoftkeyFunction( UsbDetectScrLskHdlr ,KEY_EVENT_UP);
    SetRightSoftkeyFunction(UsbDetectScrRskHdlr ,KEY_EVENT_UP);
    
    //UsbEndKeyFuncPtr = GetKeyHandler(KEY_END ,KEY_EVENT_DOWN) ;

    SetKeyHandler(UsbDetectScrENDKeyHdlr, KEY_END ,KEY_EVENT_DOWN);
    
    //SetExitHandler (SCR_DEV_USBDETECT, ExitUsbDetectQueryScr);

#ifdef __MMI_FM_RADIO__
	mmi_fmrdo_search_all_channels_stop();
#endif // __MMI_FM_RADIO__
#endif

}



/*****************************************************************************
* FUNCTION
*  ExitUsbDetectQueryScr
*
* DESCRIPTION
*   This function is exit handler for  "Usb Config:" screen
*
* PARAMETERS: void
*
* RETURNS: none
*
* GLOBALS AFFECTED
*
*****************************************************************************/

void ExitUsbDetectQueryScr(void)
{
   MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_EXIT_DETECT_QUERY_SCR));
#if defined( __MMI_UI_STYLE_3__) || defined(__MMI_UI_STYLE_4__) 
   leave_full_screen();
#endif
/*
   history currHistory;
   currHistory.scrnID = SCR_DEV_USBDETECT;
	currHistory.entryFuncPtr = EntryUsbDetectQueryScr;
	pfnUnicodeStrcpy((S8*)currHistory.inputBuffer, (S8*)L"");
	GetCategory32History (currHistory.guiBuffer);
	AddHistory (currHistory);
*/

   /* If exit from config page, It has to know  */
	if( GetUsbStatus() == MMI_USBSTATUS_CONF_PAGE )
	  SetUsbStatus( MMI_USB_CTX(usb_status_prev) );

}

	


/* ============================================================================= */
/* ===  Message Handling  Function  */
/* ============================================================================= */
/*                                                                               */  
/*  USB device config message flow:                                              */
/*                                                                               */  
/*  (1) Mass storage:                                                            */  
/*                                                                               */  
   
/*   ========                          =======                                   */  
/*      |                                 |                                      */  
/*      |   MSG_ID_MMI_EQ_USBDETECT_IND   |                                      */  
/*      |<--------------------------------|                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      |   MSG_ID_MMI_EQ_USBCONFIG_REQ   |                                      */  
/*      |-------------------------------->|                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      |   MSG_ID_MMI_EQ_USBCONFIG_RSP   |                                      */  
/*      |<--------------------------------|                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      |   MSG_ID_MMI_EQ_POWER_OFF_REQ   |                                      */  
/*      |-------------------------------->|                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      |   MSG_ID_MMI_EQ_POWER_OFF_RSP   |                                      */  
/*      |<--------------------------------|                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      |   MSG_ID_MMI_EQ_POWER_ON_IND    |                                      */  
/*      |<--------------------------------|                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*   ========                          =======                                   */  
/*                                                                               */  
/*                                                                               */  
/*                                                                               */  
/*                                                                               */  
/*  (2) COM Port emulator:                                                       */  
/*                                                                               */  
   
/*   ========                          =======                                   */  
/*      |                                 |                                      */  
/*      |   MSG_ID_MMI_EQ_USBDETECT_IND   |                                      */  
/*      |<--------------------------------|                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      |   MSG_ID_MMI_EQ_USBCONFIG_REQ   |                                      */  
/*      |-------------------------------->|                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      |   MSG_ID_MMI_EQ_USBCONFIG_RSP   |                                      */  
/*      |<--------------------------------|                                      */  
/*      |                                 |                                      */  
/*  ##########################################################################   */  
/*  #   |                                 |    ** Interface change 040927 ** #   */  
/*  #   |                                 |    L4C will switch specified app #   */  
/*  #   |   MSG_ID_MMI_EQ_GET_UART_REQ    |    to usb port and store the     #   */  
/*  #   |-------------------------------->|    previous port. When USB device#   */  
/*  #   |                                 |    plug out, will switch back    #   */  
/*  #   |                                 |    by L4C itself, so it is not   #   */  
/*  #   |   MSG_ID_MMI_EQ_GET_UART_RSP    |    necessary to get UART and     #   */  
/*  #   |<--------------------------------|    keep in MMI again.            #   */  
/*  #   |                                 |                                  #   */  
/*  ##########################################################################   */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      MSG_ID_MMI_EQ_USBUART_SWITCH_PORT_REQ                                    */  
/*      |<--------------------------------|                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      MSG_ID_MMI_EQ_USBUART_SWITCH_PORT_RSP                                    */  
/*      |-------------------------------->|                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*      |                                 |                                      */  
/*   ========                          =======                                   */  
/*                                                                               */  
/*                                                                               */  
/*                                                                               */  
/* ============================================================================= */

/*****************************************************************************
* FUNCTION
*  UsbDetectScrHlightHdlr
*
* DESCRIPTION
*   This function assign the highlight item (usb function) for usb config.
*
* PARAMETERS: highlight function idx
*
* RETURNS: void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void UsbDetectScrHlightHdlr(S32 index)
{
	highlightedItemIndex = index;
}




/*****************************************************************************
* FUNCTION
*  UsbDetectScrENDKeyHdlr
*
* DESCRIPTION
*   This function is END key handler at usb config page.
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void UsbDetectScrENDKeyHdlr(void)
{
   MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_DETECT_SCR_END_KEY_HDLR ));

   if( GetUsbStatus() == MMI_USBSTATUS_CONF_PAGE )
      GoBackHistory();
   else
      DeleteScreenIfPresent(SCR_DEV_USBDETECT);

   SetUsbStatus( MMI_USBSTATUS_IGNORED );
   
   if( MMI_USB_CTX(EndKeyFuncPtr) )
   {
      MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_DETECT_SCR_END_KEY, MMI_USB_CTX(EndKeyFuncPtr) ));
      ( *(MMI_USB_CTX(EndKeyFuncPtr)) )();

       MMI_USB_CTX(EndKeyFuncPtr) = NULL;
   }

}



/*****************************************************************************
* FUNCTION
*  UsbDetectScrRskHdlr
*
* DESCRIPTION
*   This function is right softkey handler at usb config page.
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED
*
*****************************************************************************/

void UsbDetectScrRskHdlr(void)
{
   GoBackHistory();
   DeleteScreenIfPresent(SCR_DEV_USBDETECT);
   SetUsbStatus( MMI_USBSTATUS_IGNORED );
   
}




/*****************************************************************************
* FUNCTION
*  UsbDetectScrLskHdlr
*
* DESCRIPTION
*   This function is left softkey handler at usb config page.
*   (Press left softkey means start to config usb device as certain function)
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: highlightedItemIndex, the usb function index
*
*****************************************************************************/

void UsbDetectScrLskHdlr(void)
{
mmi_trace(1,"UsbDetectScrLskHdlr,highlightedItemIndex is %d",highlightedItemIndex);
	switch( highlightedItemIndex )
	{
	
	case MMI_USB_CONFIG_MENU_MS: /* MASS Storage */
		 #ifdef MMI_ON_HARDWARE_P
		    MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_CONF_AS_MS));
          UsbConfigFirstPage(DEVUSB_CFG_ACTION_MASS_STORAGE);
		 #else
          ShutdownSystemOperation();
		 #endif
		break;

#ifdef __MMI_WEBCAM__	
	case MMI_USB_CONFIG_MENU_WEBCAM: /* Webcam */
		 #ifdef MMI_ON_HARDWARE_P
          UsbConfigFirstPage(DEVUSB_CFG_ACTION_WEBCAM);
		 #else
          GoBackHistory();
		 #endif
		break;
#endif /* __MMI_WEBCAM__ */

	case MMI_USB_CONFIG_MENU_CDCACM: /*Com port emulator */
		 #ifdef MMI_ON_HARDWARE_P
		    MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_CONF_AS_CDCACM));
          UsbConfigFirstPage(DEVUSB_CFG_ACTION_CDC_ACM);
		 #else
          GoBackHistory();
		 #endif
		break;

	case MMI_USB_CONFIG_MENU_RESERVED:  /* reserved */
	default:
      MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_CONF_AS_NOTHING));
		GoBackHistory();
		break;
	}
}



/*****************************************************************************
* FUNCTION
*  PendingUsbDetectionTimeoutHdlr
*
* DESCRIPTION
*   This function try to display usb config screen if usb device still there.
*   If could not display, will wait a period of time and retry again.
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void PendingUsbDetectionTimeoutHdlr(void)
{
      MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_PEND_DETECT_TIMEOUT));

      StopTimer(PENDING_USBDETECTION_HDLR_TIMER);

      MMIUSB_RESET_FLAG(MMIUSB_MASK_PENDING_TIMER );

      if( GetUsbStatus() == MMI_USBSTATUS_PLUGIN )
      {
            if( PermitUsbCfgScreenDisplay() == MMI_TRUE )
            {
               MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_PEND_DETECT_TIMEOUT_ALLOW));
               EntryUsbDetectQueryScr();
            }
            else
            {
               MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_PEND_DETECT_TIMEOUT_NOT_ALLOW));
               PendingUsbDetectionHdlr();
            }
      }
}



/*****************************************************************************
* FUNCTION
*  PendingUsbDetectionHdlr
*
* DESCRIPTION
*     This function is to handle panding usb detection event. When usb detected
*  at critical time and is not allow to display config screen, the detected event
*  will be pending. After the time over, shall call this to deal with the pending
*  event. This will start a timer then show config screen to prevent screen overlap
*  case by function call.
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void PendingUsbDetectionHdlr(void)
{
   MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_START_TIMER));

   if( GetUsbStatus() == MMI_USBSTATUS_PLUGIN )
   {
      MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_START_PEND_TIMEER));

      MMIUSB_SET_FLAG(MMIUSB_MASK_PENDING_TIMER );
	   StartTimer(PENDING_USBDETECTION_HDLR_TIMER, PENDING_USBDETECTION_HDLR_TIMER_DUR, PendingUsbDetectionTimeoutHdlr );
   }
}


/*****************************************************************************
* FUNCTION
*  UsbBatStatusIndHdlr
*
* DESCRIPTION
*     This function is usb mode battery status indication handler
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/
void UsbBatStatusIndHdlr(void *info)
{
	mmi_eq_battery_status_ind_struct *p=(mmi_eq_battery_status_ind_struct*) info;

	switch(p->battery_status)
	{
		case PMIC_CHARGE_COMPLETE:
		   ShowUsbChgCmpltScr();
		   break;

		default:
		   break;
	}
}


/*****************************************************************************
* FUNCTION
*  UsbGpioDetectIndHdlr
*
* DESCRIPTION
*     This function is usb mode battery status indication handler
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/
void UsbGpioDetectIndHdlr(void * info)
{
	mmi_eq_gpio_detect_ind_struct *gpioDetectInd;
	gpioDetectInd = (mmi_eq_gpio_detect_ind_struct *) info;
	
	switch(gpioDetectInd->gpio_device)
	{
	
#ifdef __MMI_CLAMSHELL__

      /* Clam open */
		case EXT_DEV_CLAM_OPEN:
			gMMIClamState=1;
			if(!g_pwr_context[0].PowerOnPeriod)
			{
//				lcd_backlight = 0;
				if(AlmIsRTCPwron())
 					TurnOnBacklight(0);	
				else
				   TurnOnBacklight(1);
			}
		   break;

		   
      /* Clam close */
		case EXT_DEV_CLAM_CLOSE:
			gMMIClamState=0;

			if(!g_pwr_context[0].PowerOnPeriod)
			{
				CloseBacklight();
			}
			
   		break;

#endif /* __MMI_CLAMSHELL__ */

      default:
         break;
	}	

    
   /* Shall reset this flag in the end of msg handler func */
   SetInterruptPopupDisplay(POPUP_ALL_ON);
}

#ifdef __MMI_USB_COPY_RIGHT__
/*****************************************************************************
* FUNCTION
*  UsbPowerOnFinishHdlr
*
* DESCRIPTION
*     This function is usb power on finish indication event handler
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/
void UsbPowerOnFinishHdlr(void *info)
{
	EntryNewScreen(SCR_USB_POWER_ON_DONE, NULL, (FuncPtr)UsbPowerOnFinishHdlr, NULL);
	
	DinitHistory();

   ShowCategory106Screen(IMG_USB_POWER_ON, NULL, NULL);
   
#ifdef   __MMI_SUBLCD__
   ShowSubLCDScreen(ShowSubLCDUsbScr);
#endif

   SetKeyHandler(UsbPwnOnThenPwnOn,KEY_END,KEY_LONG_PRESS);	
}
#endif





/**************************************************************


	FUNCTION NAME		: ExitUSBConnectingScr

  	PURPOSE				: exit USB connecting screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
void ExitUSBConnectingScr(void)
{
	
//	MCI_USBDisconnect();
	
		/* stop all playing animation */
	gdi_image_stop_animation_all();

	/* disable multi layer */
	gdi_layer_multi_layer_disable();

	/* resume alignment timer */
	UI_disable_alignment_timers();

	/* resume LCD patten */
	StopLEDPatternPowerOn();

	/* resume background audio */
	mdi_audio_resume_background_play();
	

}


/**************************************************************


	FUNCTION NAME		: EntryUSBConnectingScr

  	PURPOSE				: Show USB connecting screen when USB is connecting

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: void 

	RETURNS				: void

 

**************************************************************/
#include "extdevicedefs.h"
#include "keybrd.h"
#include "mci.h"
extern void mmi_camera_entry_app_screen();
extern void 	AudioStopReq(U16 soundId);
extern pBOOL isInCall(void);
extern pBOOL is_phb_in_progress(void);

#ifdef JATAAYU_SUPPORT
extern BOOL isInWapApp(void);
extern BOOL isInMMSApp(void);
#endif

extern void Usb_stop_application(void);
void EntryUSBConnectingScr(void)
{
	//EntryNewScreen(SCR_USB_CONNECTING, ExitUSBConnectingScr, EntryUSBConnectingScr, NULL);
		entry_full_screen();
		mmi_trace(1,"EntryUSBConnectingScr");
		
		/*modify to ensure set the wall paper before refresh screen*/
		Usb_stop_application();
		
		//GoBackHistory();//chenhe to exit all app
		
#if 0
		/* force all playing keypad tone off */
		AudioStopReq(GetCurKeypadTone());

		/* disable key pad tone */
		mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
#endif
		/* stop background play */
		//mdi_audio_stop_all();
		//mdi_audio_suspend_background_play();
		
		/*when entry mess storage mode of usb,there are several app need to disable(deinit),
		mmi_usb_app_deinit this function deinit the apps when needed,
		you can also add additional function in mmi_usb_deinit_callback to deinit the specific app*/
		mmi_usb_app_deinit(NULL);

		/* disalbe align timer and enter game */
		UI_disable_alignment_timers();

		/* stop LCD patten */
		StopLEDPatternBackGround();
#if (CSW_EXTENDED_API_PMD_USB == 0)
		MCI_USBConnect();//fix bug 12925 gdm 090703
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
		SetUsbStatus( MMI_USBSTATUS_MS_PROC );		
		IdleSetStatusIcon(STATUS_ICON_USB_CONNETCED);

		//GoBackHistory();//not display ,but go back
#ifdef JATAAYU_SUPPORT
		if ((!isInCall())
		#ifdef JATAAYU_SUPPORT_WAP
			&&(!isInWapApp())
		#endif
		#ifdef JATAAYU_SUPPORT_MMS
			&&(!isInMMSApp())
		#endif
			&&(!is_phb_in_progress()))
		{
			DisplayIdleScreen();
		}
		else
		{
			GoBackHistory();
		}
#else
		if (!isInCall())
		{
			DisplayIdleScreen();
		}
		else
		{
			GoBackHistory();

		}
#endif

}

void UsbGoBackHistory(void)
{
	SetUsbStatus( MMI_USBSTATUS_PLUGOUT );
	GoBackHistory();
}

#ifdef __MMI_WEBCAM__
static void mmi_usb_webcam_handler(void)
{
	if (!isInCall())
	{
		mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_CONNECTED;
		
		mmi_usb_webcam_entry_app_scr();
		DeleteScreenIfPresent(SCR_DEV_USBDETECT);
	}
	else
	{
		GoBackHistory();
	}
		
}
#endif

#ifdef __MMI_SUPPORT_JME__
extern BOOL checkVMOn(void);
extern BOOL checkVmBackGround(void);

#endif
void usbHilightHdr(S32 index)
{
	switch(index)
	{
		case 0:
			SetLeftSoftkeyFunction(EntryUSBConnectingScr,KEY_EVENT_UP);

			break;
#ifdef __MMI_WEBCAM__
		case 1:
			SetLeftSoftkeyFunction(mmi_usb_webcam_handler,KEY_EVENT_UP);

			break;			
#endif			
		default:
			break;
	}
		SetRightSoftkeyFunction(UsbGoBackHistory,KEY_EVENT_UP);

}
void UsbPluginENDKeyHdlr(void)
{
	SetUsbStatus( MMI_USBSTATUS_PLUGOUT );
	//mmi_trace(1,"chenhe,!(checkVMOn()&&!checkVmBackGround() is %d",!(checkVMOn()&&!checkVmBackGround());
#ifdef JATAAYU_SUPPORT
	if ((!isInCall())
	#ifdef JATAAYU_SUPPORT_WAP
		&&(!isInWapApp())
	#endif
	#ifdef JATAAYU_SUPPORT_MMS
		&&(!isInMMSApp())
	#endif
		&&(!is_phb_in_progress())
	#ifdef __MMI_SUPPORT_JME__
		&&!(checkVMOn()&&!checkVmBackGround())
	#endif
		)
	{
		DisplayIdleScreen();
	}
	else
	{
		GoBackHistory();
	}
#else
	if (!isInCall()&&(!is_phb_in_progress())
	#ifdef __MMI_SUPPORT_JME__
		&&!(checkVMOn()&&!checkVmBackGround())
	#endif
	)
	{
		DisplayIdleScreen();
	}
	else
	{
		GoBackHistory();

	}
#endif

}
void UsbPlugin()
{
	UINT8 *gui_buffer_p;
	UINT16 itemStrId[] = {STR_USB_CONFIG_MS
						 #ifdef __MMI_WEBCAM__
						 ,STR_ID_USB_CONFIG_WEBCAM
						 #endif
						 };
	UINT16 itemIconId = STR_GLOBAL_OK;
	U8* item_popups[] = { NULL,NULL};
	INT32 NumOfItems = 1;
#ifdef __MMI_WEBCAM__
	NumOfItems++;
#endif
	EntryNewScreen(SCR_DEV_USBDETECT, NULL, UsbPlugin,NULL);
	gui_buffer_p = GetCurrGuiBuffer( SCR_DEV_USBDETECT );
//    mmi_trace(1,"UsbPlugin,STR_USB_WEB_CAM is %d",STR_USB_WEB_CAM);
	RegisterHighlightHandler(usbHilightHdr);
#ifdef TGT_GALLITE_G800
        ClearAllKeyHandler();
#endif
	if( g_charbat_context.ChargingComplete == 0)   //added by guojian for bug 5999
	{
		AnimateStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
	}
	ShowCategory52Screen(STR_GLOBAL_OPTIONS,0,STR_GLOBAL_OK,0,STR_GLOBAL_BACK,0,NumOfItems,(U16*)&itemStrId,(U16*)&itemIconId,(U8**)item_popups,0,0,gui_buffer_p);

	//SetLeftSoftkeyFunction(EntryUSBConnectingScr,KEY_EVENT_UP);
	SetKeyHandler(UsbPluginENDKeyHdlr, KEY_END ,KEY_EVENT_DOWN);

	//Set Key handlers
	//SetKeyHandler( EntryUSBConnectingScr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	//SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );	
}

/*****************************************************************************
* FUNCTION
*  UsbDetectIndHdlr
*
* DESCRIPTION
*     This function is usb detect indication event handler
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/
int checkUsbstate(void)
{
//	mmi_trace(0,"checkUsbstate,state is %d",GetUsbStatus());
	if( GetUsbStatus() == MMI_USBSTATUS_MS_PROC)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

extern void BatteryIndicationPopup(U16 stringId);
extern void IdleScreenChargerDisconnected(void);
#ifdef __MMI_USB_DOWNLOAD_FR__
extern BOOL pm_GetUsbDownLoadStatus(VOID);
extern void HandleUSBDownloadModel(void);
#endif
BOOL g_UsbChargerIn = 0;

void UsbDetectIndHdlr(void *info)
{
   MMI_USB_STATUS usb_state = GetUsbStatus();
#ifdef MMI_ON_HARDWARE_P	
#ifdef __MMI_USB_DOWNLOAD_FR__
   UCTLS_ID_T uctlsMode = uctls_GetMode();
#endif
#endif
	mmi_eq_usbdetect_ind_struct *p=(mmi_eq_usbdetect_ind_struct *)info;
    mmi_trace(1,"UsbDetectIndHdlr,p->action is %d,usb_state is %d",p->action,usb_state);
	TurnOnBacklight(TRUE);
	switch(p->action)
	{
		case DEVUSB_DETECT_ACTION_PLUGIN:
		{
#ifdef MMI_ON_HARDWARE_P
#ifdef __MMI_USB_DOWNLOAD_FR__
			if(pm_GetUsbDownLoadStatus())
				{
					HandleUSBDownloadModel();
					if(MMI_USBSTATUS_PLUGOUT == usb_state||MMI_USBSTATUS_UNKNOW ==  usb_state)
					{
						SetUsbStatus( MMI_USBSTATUS_DETECTED);
						g_UsbChargerIn = 1;
						
						EntryUSBConnectingScr();
					#if (CSW_EXTENDED_API_PMD_USB == 0)
						BatteryIndicationPopup(STR_USB_CHARGER_CONNECTED);
					#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */

					}
				}
			else
#endif
#endif
				{
					if(MMI_USBSTATUS_PLUGOUT == usb_state||MMI_USBSTATUS_UNKNOW ==  usb_state)
					{
						SetUsbStatus( MMI_USBSTATUS_DETECTED);
						g_UsbChargerIn = 1;
						UsbPlugin();
					//qiff add for bug: 12918
					#if (CSW_EXTENDED_API_PMD_USB == 0)
						BatteryIndicationPopup(STR_USB_CHARGER_CONNECTED);
					#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
					}
				}
			break;
		}
		case DEVUSB_DETECT_ACTION_PLUGOUT:
		{
			
                    mmi_trace(1,"DEVUSB_DETECT_ACTION_PLUGOUT,1111111111");
#ifdef MMI_ON_HARDWARE_P
#ifdef __MMI_USB_DOWNLOAD_FR__
		if(UCTLS_ID_TRACE == uctlsMode)
			{
				HandleUSBDownloadModel();
			}
#endif
#endif
			{
// Beginning--modified by kecx for fixing bug 13662 on 20090917    
                      if (POPUP_SCREENID==GetActiveScreenId())
                      {                        
                             GoBackHistory();
                      }
// end--modified by kecx for fixing bug 13662 on 20090917    
			if(MMI_USBSTATUS_DETECTED == usb_state)
			{
				mmi_trace(1,"DEVUSB_DETECT_ACTION_PLUGOUT,22222");
				if(GetActiveScreenId() == SCR_DEV_USBDETECT)
				{
				      GoBackHistory();
				}
				DeleteScreenIfPresent(SCR_DEV_USBDETECT);
                      //ShutdownSystemOperation();
			}
			if (MMI_USBSTATUS_MS_PROC == usb_state)
			{
#if (CSW_EXTENDED_API_PMD_USB == 0)

				MCI_USBDisconnect();
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */

			fmgr_init_context();

			}
#ifdef __MMI_WEBCAM__
            else if (MMI_USBSTATUS_WEBCAM == usb_state)
            {
#ifdef MMI_ON_HARDWARE_P
            
            	#if (CSW_EXTENDED_API_PMD_USB == 0)
				uctls_ChargerStatus(UCTLS_CHARGER_STATUS_DISCONNECTED);
				#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
#endif				
                mmi_usb_webcam_plug_out();
            }
#endif /* __MMI_WEBCAM__ */ 
			SetUsbStatus(MMI_USBSTATUS_PLUGOUT);
			IdleResetStatusIcon(STATUS_ICON_USB_CONNETCED);
#if (CSW_EXTENDED_API_PMD_USB == 0)
			BatteryIndicationPopup(STR_USB_CHARGER_REMOVED);
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
	if(IsScreenPresent (SCR_DEV_USBDETECT))
	{
	 	DeleteBetweenScreen (SCR_DEV_USBDETECT, SCR_DEV_USBDETECT);
	}
			IdleScreenChargerDisconnected();
			}
			break;
		}
		default:
			break;
	}

   #if 0
   MMI_USB_STATUS usb_state;
mmi_trace(1,"UsbDetectIndHdlr");
   MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_DETECT_SCR ));

   usb_state = GetUsbStatus();

   //chenhe for jasperII +
   usb_state = MMI_USBSTATUS_PLUGOUT;
     #if 0 

   p->action = DEVUSB_DETECT_ACTION_PLUGIN;
#endif   
   //chenhe for jasperII -
   mmi_trace(1,"UsbDetectIndHdlr,usb_state is %d,p->action is %d",usb_state,p->action);
   
#ifndef MMI_ON_HARDWARE_P
   if( 1 ) /* PC Simulator */
#else
   if( p->action == DEVUSB_DETECT_ACTION_PLUGIN )
#endif
   {
         MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_DETECT_SCR_PLUGIN ));
         
         switch(usb_state)
         {
            case MMI_USBSTATUS_PLUGOUT:
            {
				mmi_trace(1,"UsbDetectIndHdlr111");
               SetUsbStatus( MMI_USBSTATUS_PLUGIN );

               if( PermitUsbCfgScreenDisplay() == MMI_TRUE )
               {
                  if( MMIUSB_GET_FLAG(MMIUSB_MASK_PENDING_TIMER) )
                  {
                        MMIUSB_RESET_FLAG(MMIUSB_MASK_PENDING_TIMER );
                        StopTimer(PENDING_USBDETECTION_HDLR_TIMER);
                  }
                  
                  EntryUsbDetectQueryScr();
               }
            }
               break;

            case MMI_USBSTATUS_MS:
               /* USB power on */
               break;
               
            case MMI_USBSTATUS_MS_PROC:
            case MMI_USBSTATUS_CDCACM_PROC:
            //case MMI_USBSTATUS_CDCACM_SWCHPORT:
            case MMI_USBSTATUS_UNKNOW:
            case MMI_USBSTATUS_PLUGIN:
            case MMI_USBSTATUS_CONF_PAGE:
            case MMI_USBSTATUS_IGNORED:
            case MMI_USBSTATUS_CDCACM:
            case MMI_USBSTATUS_WEBCAM:
            default:
               #ifndef __PRODUCTION_RELEASE__
               ASSERT(0);
               #endif
               break;
         }
         
   } /* p->action == DEVUSB_DETECT_ACTION_PLUGIN */
   else  if( p->action == DEVUSB_DETECT_ACTION_PLUGOUT )
   {
         MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_DETECT_SCR_PLUGOUT ));

         StopTimer(PENDING_USBDETECTION_HDLR_TIMER);

         switch(usb_state)
         {
            case MMI_USBSTATUS_CONF_PAGE:
					/* detect last query screen is in history */
					DeleteScreenIfPresent(SCR_DEV_USBDETECT);
               GoBackHistory();
               break;
               
            case MMI_USBSTATUS_CDCACM:
            //case MMI_USBSTATUS_CDCACM_SWCHPORT:
            {
               /*******************************/
               /** Interface change 040927   **/
               /** See the description above **/
               /*******************************/
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
               #endif
            }
               break;

            case MMI_USBSTATUS_IGNORED:
            case MMI_USBSTATUS_MS_PROC:
            case MMI_USBSTATUS_CDCACM_PROC:
            case MMI_USBSTATUS_PLUGIN:
            case MMI_USBSTATUS_PLUGOUT:
            case MMI_USBSTATUS_MS:
               DeleteScreenIfPresent(SCR_DEV_USBDETECT);
               break;

				#ifdef __MMI_WEBCAM__
	            case MMI_USBSTATUS_WEBCAM:
	            	DeleteScreenIfPresent(SCR_DEV_USBDETECT);
	            	mmi_usb_webcam_plug_out();
	            	break;
				#endif /* __MMI_WEBCAM__ */

            case MMI_USBSTATUS_UNKNOW:
            default:
               #ifndef __PRODUCTION_RELEASE__
               ASSERT(0);
               #endif
               DeleteScreenIfPresent(SCR_DEV_USBDETECT);
               break;
         }

         SetUsbStatus( MMI_USBSTATUS_PLUGOUT );
         //MMI_USB_CTX(usb_status_prev) = MMI_USBSTATUS_PLUGOUT;
   }
   else
     ASSERT(0);
   #endif
   
}




/*****************************************************************************
* FUNCTION
*  UsbDetectIndHdlr
*
* DESCRIPTION
*     Display transition screen and send usb config request (MSG_ID_MMI_EQ_USBCONFIG_REQ) to HW
*
* PARAMETERS: mode, ms or cdcacm
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void UsbConfigFirstPage( U8 mode )
{
   MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_CONF_FIRST_PAGE, mode ));
   
   if(  GetUsbStatus() == MMI_USBSTATUS_PLUGOUT )
   {
       //GoBackHistory();
       DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
       DeleteScreenIfPresent(SCR_DEV_USBDETECT);
       return;
   }
#ifdef __MMI_IRDA_SUPPORT__   
   else if ( mode == DEVUSB_CFG_ACTION_CDC_ACM && mmi_irda_is_open() == MMI_TRUE )
   {
   	   /* currently, when IrComm is active, switch port not allow! */
       //GoBackHistory();
       DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
       DeleteScreenIfPresent(SCR_DEV_USBDETECT);
       return;
   }
#endif

#ifdef __MMI_WEBCAM__
	if( mode == DEVUSB_CFG_ACTION_WEBCAM )
	{
		MMI_ASSERT(mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_DISCONNECT);
		
		mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_CONNECTED;
		
		mmi_usb_webcam_entry_app_scr();
	   DeleteScreenIfPresent(SCR_DEV_USBDETECT);
		return;
	}
#endif /* __MMI_WEBCAM__ */

   //ExecuteCurrExitHandler();
   EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
   
   DeleteScreenIfPresent(SCR_DEV_USBDETECT);
  
   ClearKeyEvents();
   ClearAllKeyHandler();
   ClearKeyHandler(KEY_END,KEY_EVENT_UP);
   ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
   ClearKeyHandler(KEY_END,KEY_LONG_PRESS);

   if( mode == DEVUSB_CFG_ACTION_MASS_STORAGE )
   {
   mmi_trace(1,"UsbConfigFirstPage,mode is DEVUSB_CFG_ACTION_MASS_STORAGE");
      SetUsbStatus( MMI_USBSTATUS_MS_PROC );
      ShowCategory64Screen(STR_USB_SHUTDOWN_SCR,IMG_GLOBAL_WARNING,NULL);
      SendUsbConfigToHW( DEVUSB_CFG_ACTION_MASS_STORAGE  );
   }
   else if( mode == DEVUSB_CFG_ACTION_CDC_ACM )
   {
      /* avoid entry UART setting screen */
      #ifdef __MMI_ENGINEER_MODE__
      DeleteScreenIfPresent(EM_DEV_INLINE_CONFIG_UART_MENU_SCR);
      DeleteScreenIfPresent(EM_DEV_SET_UART_MENU_SCR);
      #endif

      SetUsbStatus( MMI_USBSTATUS_CDCACM_PROC );
      ShowCategory64Screen(STR_USB_CDCACM_SETTING_SCR,IMG_GLOBAL_WARNING,NULL);
      SendUsbConfigToHW( DEVUSB_CFG_ACTION_CDC_ACM );
   }
   else
      ASSERT(0);

}


/*****************************************************************************
* FUNCTION
*  UsbConfigRspHdlr
*
* DESCRIPTION
*     This is usb config response handler
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/
BOOL usbGetState(void)
{
	return usbIsExist;
}
void UsbConfigRspHdlr(void *info )
{
#ifdef MMI_ON_HARDWARE_P

	 mmi_eq_usbconfig_rsp_struct *p_usbcfg_struct = (mmi_eq_usbconfig_rsp_struct *)info;

    MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_CONF_RSP_HDLR ));

     if( p_usbcfg_struct->result  != KAL_TRUE || GetUsbStatus() == MMI_USBSTATUS_PLUGOUT) 
     {
         /* Config error or USB device pluged out, drv will be reset so stop further processing */
         GoBackHistory();
         DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
         return;
     }

     switch( p_usbcfg_struct->mode )
      {
      case DEVUSB_CFG_ACTION_MASS_STORAGE 	/* USB_MASS_STORAGE */:
	  	usbIsExist = TRUE;
         UsbConfigMassStorage();
         break;
         
      case DEVUSB_CFG_ACTION_CDC_ACM  			/* USB_CDC_ACM */:
         UsbConfigCDCACM();
         break;

      case DEVUSB_CFG_ACTION_WEBCAM  			/* USB_WEBCAM */:
         /* Do nothing */
         break;
         
      default:
         GoBackHistory();
         break;
      }
#else

	if( highlightedItemIndex == MMI_USB_CONFIG_MENU_MS )
      ShutdownSystemOperation();
	else
		GoBackHistory();

#endif /* MMI_ON_HARDWARE_P */
}


/*****************************************************************************
* FUNCTION
*  UsbCfgMSTimeoutHdlr
*
* DESCRIPTION
*     This fucntion is mass storage following action after usb conigure response comes, 
*   (MSG_ID_MMI_EQ_USBCONFIG_RSP) and the screen timer expire
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void UsbCfgMSTimeoutHdlr(void)
{
   //if(g_charbat_context.PowerOnCharger || (AlmIsRTCPwron() == 1))
   if(   g_pwr_context[0].PowerOnMode != POWER_ON_KEYPAD 
   	  && g_pwr_context[0].PowerOnMode != POWER_ON_EXCEPTION  )
     UsbSendMsgToHW(PRT_POWER_OFF_REQ, NULL, NULL);
   else
   	ShutdownSystemOperation();
}



/*****************************************************************************
* FUNCTION
*  UsbConfigMassStorage
*
* DESCRIPTION
*     This fucntion is mass storage following action after usb conigure response comes, 
*   (MSG_ID_MMI_EQ_USBCONFIG_RSP)
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void UsbConfigMassStorage(void)
{
   MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_CONF_MS ));
   
   	StartTimer( USB_SHUTDOWN_SCR_TIMER, USB_SHUTDOWN_SCR_TIMER_DUR, UsbCfgMSTimeoutHdlr);
}


/*****************************************************************************
* FUNCTION
*  UsbConfigCDCACM
*
* DESCRIPTION
*     This fucntion is cdcacm following action after usb conigure response comes, 
*   (MSG_ID_MMI_EQ_USBCONFIG_RSP)
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void UsbConfigCDCACM(void)
{
   /*******************************/
   /** Interface change 040927   **/
   /** See the description above **/
   /*******************************/
   S16 pError;
   U8 app;
   
   MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_CONF_CDCACM ));
/*   
   SetProtocolEventHandler (UsbGetUartPortRspHdlr, MSG_ID_MMI_EQ_GET_UART_RSP);
   SendUsbGetUartPortToHW();
*/

   ReadValue(NVRAM_DEFAULT_USB_APP, &app, DS_BYTE, &pError);

   if( pError != NVRAM_READ_SUCCESS )
      app=USB_SWITCH_PORT_APP_DATA;
   else if( app == 0xFF )
   {
      app=USB_SWITCH_PORT_APP_DATA;
      WriteValue( NVRAM_DEFAULT_USB_APP, &app, DS_BYTE, &pError );
   }

   ASSERT( (app == USB_SWITCH_PORT_APP_DATA) || (app == USB_SWITCH_PORT_APP_DEBUG) );

   SendUsbUartSwitchToHW( app, uart_port_usb );
}



/*****************************************************************************
* FUNCTION
*  UsbUartSwitchRspHdlr
*
* DESCRIPTION
*     This is uart switch port response handler
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void UsbUartSwitchRspHdlr(void *info)
{
   mmi_eq_usbuart_switch_port_rsp_struct * rsp_p = info;
   MMI_USB_STATUS usb_state;

   MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_UART_SWITCH_RSP_HDLE ));

   usb_state = GetUsbStatus();
   
   /*if(  usb_state != MMI_USBSTATUS_PLUGOUT )
     GoBackHistory();*/

   if( !(rsp_p->result) )
      DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
   else
   {
      if( usb_state != MMI_USBSTATUS_PLUGOUT  )
      {
        /* Finish CDCACM config and port switch, change state */
        SetUsbStatus(MMI_USBSTATUS_CDCACM);
      }
      DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
      DeleteScreenIfPresent(SCR_DEV_USBDETECT);

      /*******************************/
      /** Interface change 040927   **/
      /** See the description above **/
      /*******************************/
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
      #endif
      
   }
}



/*****************************************************************************
* FUNCTION
*  UsbGetUartPortRspHdlr
*
* DESCRIPTION
*     This is uart get port response handler
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void UsbGetUartPortRspHdlr(void *info)
{
   mmi_eq_get_uart_rsp_struct* rsp_p = info;
   S16 pError;
   U8 app;

   MMI_TRACE((MMI_TRACE_FUNC, MMI_USB_FUNC_GET_UART_PORT_RSP_HDLE ));

   /* Error case, stop subsequence action and return */
   if( GetUsbStatus() == MMI_USBSTATUS_PLUGOUT || rsp_p->result != MMI_TRUE )
   {
      GoBackHistory();
      DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
      return;
   }

   /* Get uart port successfully, store in context */
   MMI_USB_CTX(ps_port_prev) =  rsp_p->ps_uart_port;
   MMI_USB_CTX(tst_port_prev) =  rsp_p->tst_uart_port;

   /* Get the target application to switch COM port */
   /* (set from engineer mode, ps(data) or tst(debug) ) */
   ReadValue(NVRAM_DEFAULT_USB_APP, &app, DS_BYTE, &pError);
   if( pError != NVRAM_READ_SUCCESS )
      app=USB_SWITCH_PORT_APP_DATA;
   else if( app == 0xFF )
   {
      app=USB_SWITCH_PORT_APP_DATA;
      WriteValue( NVRAM_DEFAULT_USB_APP, &app, DS_BYTE, &pError );
   }

   ASSERT( (app == USB_SWITCH_PORT_APP_DATA) || (app == USB_SWITCH_PORT_APP_DEBUG) );

   if( ((app==USB_SWITCH_PORT_APP_DATA) && ( MMI_USB_CTX(tst_port_prev) == uart_port_usb))  \
      || ((app==USB_SWITCH_PORT_APP_DEBUG) && ( MMI_USB_CTX(ps_port_prev) == uart_port_usb))  )
   { 
      /* Port confict with other application, other application has already set usb as commulation port */
      GoBackHistory();
      DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
      return;
   }
   else
   {
      /* Switch the uart port of defined application to usb port */
      //SetUsbStatus( MMI_USBSTATUS_CDCACM_SWCHPORT );
      MMIUSB_SET_FLAG(MMIUSB_MASK_PORT_SWITCHING);
      SendUsbUartSwitchToHW( app, uart_port_usb );
   }

}




/* ===========================================================================================  */
/* ===  Send Message to Protocol Stack  */
/* ===========================================================================================  */

/*****************************************************************************
* FUNCTION
*  SendUsbConfigToHW
*
* DESCRIPTION
*     This function is to send usb config request to PS
*
* PARAMETERS: mode, ms or cdcacm
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void SendUsbConfigToHW(U8 mode)
{
#ifdef MMI_ON_HARDWARE_P

   mmi_eq_usbconfig_req_struct *p_param;
     mmi_eq_usbconfig_rsp_struct *presponse;
	 presponse = OslConstructDataPtr(sizeof(mmi_eq_usbconfig_rsp_struct));
	p_param = OslConstructDataPtr(sizeof(mmi_eq_usbconfig_req_struct));
	p_param->mode = mode;
	p_param->reserved = 0;

   SetProtocolEventHandler(UsbConfigRspHdlr, MSG_ID_MMI_EQ_USBCONFIG_RSP );
	UsbSendMsgToHW(MSG_ID_MMI_EQ_USBCONFIG_REQ, (void *)p_param,  (void *)NULL );
	//chenhe for jasper+
	presponse->result = TRUE;
	presponse->mode = DEVUSB_CFG_ACTION_MASS_STORAGE;
	UsbConfigRspHdlr(presponse);
	//chenhe for jasper-
#else
	UsbConfigRspHdlr(NULL); 
#endif /* MMI_ON_HARDWARE_P */
}


/*****************************************************************************
* FUNCTION
*  SendUsbConfigToHW
*
* DESCRIPTION
*     This function is to send switch port request to PS
*
* PARAMETERS: 
*     app: data or tst
*     new_port: uart 1, uart 2, usb, ircomm...etc
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void SendUsbUartSwitchToHW(U8 app, U8 new_port)
{
#ifdef MMI_ON_HARDWARE_P

     mmi_eq_usbuart_switch_port_req_struct *p_param;

     MMI_TRACE((MMI_TRACE_G7_MISC, MMI_USB_G7_SEND_SWITCH_PORT, app, new_port));
     
	  p_param = OslConstructDataPtr(sizeof(mmi_eq_usbuart_switch_port_req_struct));
     
     /*******************************/
     /** Interface change 040927   **/
     /** See the description above **/
     /*******************************/
     //p_param->new_port = new_port;
     p_param->app = app;

     SetProtocolEventHandler(UsbUartSwitchRspHdlr, PRT_EQ_USBUART_SWITCH_PORT_RSP );
     UsbSendMsgToHW(PRT_EQ_USBUART_SWITCH_PORT_REQ, (void *)p_param,  (void *)NULL );

#endif /* MMI_ON_HARDWARE_P */
}


/*****************************************************************************
* FUNCTION
*  SendUsbGetUartPortToHW
*
* DESCRIPTION
*     This function is to send usb get port request to PS
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void SendUsbGetUartPortToHW(void)
{
#ifdef MMI_ON_HARDWARE_P
	UsbSendMsgToHW(MSG_ID_MMI_EQ_GET_UART_REQ, (void *)NULL,  (void *)NULL );
#endif /* MMI_ON_HARDWARE_P */
}




/*****************************************************************************
* FUNCTION
*  SendUsbGetUartPortToHW
*
* DESCRIPTION
*     This function is to send message to PS
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void UsbSendMsgToHW(U32 msg_id, 
                     void *local_param_ptr, 
                     void *peer_buf_ptr)
{
#ifdef MMI_ON_HARDWARE_P
	MYQUEUE Message;
     
	Message.oslMsgId = msg_id;
   Message.oslDataPtr = (oslParaType *) local_param_ptr;
   Message.oslPeerBuffPtr= peer_buf_ptr;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	OslMsgSendExtQueue(&Message);

#endif


}


/*****************************************************************************
* FUNCTION
*  UsbPwnOnThenPwnOn
*
* DESCRIPTION
*     This function is to send power on request to PS
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*
*****************************************************************************/

void UsbPwnOnThenPwnOn(void)
{
	MYQUEUE Message;
	mmi_eq_power_on_req_struct *myMsgPtr;

   PRINT_INFORMATION_2((MMI_TRACE_INFO, " USB POWERKEY_ON!!!!"));

/* [Bear] 2004/3/30 */
	#if (defined(__MMI_SUBLCD__) && defined(MMI_ON_HARDWARE_P) )
		lcd_power_on(SUB_LCD, 0);
	#endif	

	
	myMsgPtr = (mmi_eq_power_on_req_struct*) OslConstructDataPtr(sizeof(mmi_eq_power_on_req_struct));
	myMsgPtr->fun = CHARGING_RESET;
	myMsgPtr->rst = 0;
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	Message.oslMsgId = MSG_ID_MMI_EQ_POWER_ON_REQ;
   Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
}




/*****************************************************************************
*
*	USB-WebCam                                                 
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_usb_webcam_entry_app_scr()
* DESCRIPTION
*   entry of webcam screen 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef MMI_ON_WIN32
MDI_RESULT mdi_webcam_stop(void)
{

}
#endif
#ifdef __MMI_WEBCAM__
#define MMI_ID_TYPE  U16
void mmi_usb_webcam_entry_app_scr(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/
	S32 image_width;
	S32 image_height;
	U16 banding = 0;
	MDI_RESULT ret;

#if 0//JiaShuo Delete for webcam
	S32 pause_jpeg_count;
	S32 jpeg_file_len = 0;
	S32 jpeg_image_width, jpeg_image_height;
	S32 drv_image_width, drv_image_height;
	S32 i;
	PU8 jpeg_file_ptr = NULL;
	MMI_ID_TYPE str_id = 0;
	mmi_event_notify_enum err_type = 0;
#endif
	
	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
   				#ifdef MMI_ON_HARDWARE_P

	EntryNewScreen(SCR_ID_USB_WEBCAM, mmi_usb_webcam_exit_app_scr, mmi_usb_webcam_entry_app_scr, NULL);

	ClearInputEventHandler(MMI_DEVICE_KEY);

	/* eanble keypad tone, since this might enter from idle when key pad is locked */
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);

	/* disable partial screen saver */
	mmi_phnset_disable_partial_ss();

#ifdef __MMI_SUBLCD__
	if (!mmi_usb_ptr->webcam_is_sub_display)
	{
		ForceSubLCDScreen(mmi_usb_webcam_entry_sublcd_scr);
	}
#endif /* __MMI_SUBLCD__ */ 

   /***************************************************************************** 
   * register interrupt event hdlr
   *****************************************************************************/
	#if 0//JiaShuo Delete for webcam
	mmi_frm_block_general_interrupt_event(MMI_FRM_INT_COMMON_INTERRUPT_EVENT | MMI_FRM_INT_MMI_JAVA_TIMEALARM_PUSH);
	#endif
	/* hook del screen callback */
	SetDelScrnIDCallbackHandler(SCR_ID_USB_WEBCAM, mmi_usb_webcam_del_screen_id_hdlr);

#if 0 //defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__)
	/* register event hdlr */
	mmi_frm_block_interrupt_event_handler(
		MMI_FRM_INT_GPIO, 
		mmi_usb_webcam_gpio_event_hdlr, 
		mmi_usb_webcam_gpio_post_event_hdlr);
#endif /* defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__) */ 

	/* avoid status bar */
	entry_full_screen();

	/* stop bg music */
	mdi_audio_suspend_background_play();

	/* stop LED patten */
	StopLEDPatternBackGround();
#ifdef  __MMI_BACKGROUND_CALL__    
	if (mmi_ucm_app_incoming_call(MMI_UCM_CALL_TYPE_ALL) > 0 ||
		mmi_ucm_app_outgoing_call(MMI_UCM_CALL_TYPE_ALL) > 0 ||
		mmi_ucm_app_total_call(MMI_UCM_CALL_TYPE_NO_CSD) > 0)
	{
		/* draw bg */
		gdi_layer_clear(GDI_COLOR_WHITE);
		gdi_image_get_dimension_id(IMG_ID_WEBCAM_BG_INCALL, &image_width, &image_height);

		gdi_image_draw_animation_id(
			(UI_device_width - image_width) >> 1,
			(UI_device_height - image_height) >> 1,
			IMG_ID_WEBCAM_BG_INCALL,
			NULL);

		gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
		
		SetKeyHandler(mmi_usb_webcam_rsk_release, KEY_RSK, KEY_EVENT_UP);

		SetUsbStatus(MMI_USBSTATUS_WEBCAM);
		return;
	}
	else
#endif /* __MMI_BACKGROUND_CALL__ */
	{
#ifdef HORIZONTAL_CAMERA
		gdi_lcd_set_rotate(GDI_LAYER_ROTATE_270);
		gdi_layer_resize(UI_device_height, UI_device_width);

		/* draw bg */
		gdi_layer_clear(GDI_COLOR_WHITE);
		gdi_image_get_dimension_id(IMG_ID_WEBCAM_BG, &image_width, &image_height);

		gdi_image_draw_animation_id(
			(UI_device_height - image_width) >> 1,
			(UI_device_width - image_height) >> 1,
			IMG_ID_WEBCAM_BG,
			NULL);

		gdi_layer_blt_previous(0, 0, UI_device_height - 1, UI_device_width - 1);
#else /* HORIZONTAL_CAMERA */ 
		/* draw bg */
		gdi_layer_clear(GDI_COLOR_WHITE);
		gdi_image_get_dimension_id(IMG_ID_WEBCAM_BG, &image_width, &image_height);

		gdi_image_draw_animation_id(
			(UI_device_width - image_width) >> 1,
			(UI_device_height - image_height) >> 1,
			IMG_ID_WEBCAM_BG,
			NULL);

		gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
#endif /* HORIZONTAL_CAMERA */ 
	}
	SetKeyHandler(mmi_usb_webcam_rsk_release, KEY_RSK, KEY_EVENT_UP);
	
#if defined(__MMI_CAMERA_KEY__)
	SetKeyHandler(mmi_usb_webcam_capture_key_press, KEY_CAMERA, KEY_EVENT_DOWN);
#else
	SetKeyHandler(mmi_usb_webcam_capture_key_press, KEY_ENTER, KEY_EVENT_DOWN);
#endif
	if (mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_CONNECTED)
	{
		#if 0//JiaShuo Delete for webcam
		if (mdi_camera_detect_sensor_id(MDI_CAMERA_MAIN) != MDI_RES_CAMERA_SUCCEED)
		{
			return;
		}

		/* set pause jpeg file size limit */
		pause_jpeg_count = mdi_webcam_get_pause_jpeg_count();

		for (i = 0; i < pause_jpeg_count; i++)
		{
			switch (i)
			{
				case 0:
					jpeg_file_len = sizeof(webcam_pause_img_0);
					gdi_image_get_dimension_mem(GDI_IMAGE_TYPE_JPG,(PU8) webcam_pause_img_0, jpeg_file_len, &jpeg_image_width, &jpeg_image_height);
					jpeg_file_ptr = (PU8)webcam_pause_img_0;
					break;

				case 1:
					jpeg_file_len = sizeof(webcam_pause_img_1);
					gdi_image_get_dimension_mem(GDI_IMAGE_TYPE_JPG, (PU8) webcam_pause_img_1, jpeg_file_len, &jpeg_image_width, &jpeg_image_height);			   
					jpeg_file_ptr = (PU8)webcam_pause_img_1;
					break;

				case 2:
					jpeg_file_len = sizeof(webcam_pause_img_2);
					gdi_image_get_dimension_mem(GDI_IMAGE_TYPE_JPG, (PU8) webcam_pause_img_2, jpeg_file_len, &jpeg_image_width, &jpeg_image_height);
					jpeg_file_ptr = (PU8)webcam_pause_img_2;
					break;

				default:
					MMI_ASSERT(0);
					break;
			}
			mdi_webcam_get_pause_jpeg_info(i, &drv_image_width, &drv_image_height);
			mdi_webcam_set_pause_jpeg_file(i, (PU8) jpeg_file_ptr, jpeg_file_len, jpeg_image_width, jpeg_image_height);
		}
		#endif
		/* set mmi usb state and notify driver to set state */
		SetUsbStatus(MMI_USBSTATUS_WEBCAM);
#if 0//JiaShuo Change for webcam
		SendUsbConfigToHW(DEVUSB_CFG_ACTION_WEBCAM);
#else
#ifdef MMI_ON_HARDWARE_P
        uvideos_RegisterCallback(USBVideo_Send_Msg);
		uctls_SetMode(UCTLS_ID_2);
		uctls_ChargerStatus(UCTLS_CHARGER_STATUS_CONNECTED);
#endif
#endif
		
		#if 0//JiaShuo Delete for webcam
		banding = mmi_camera_get_camera_banding();
		#endif
		/* start webcam */
		if (IsClamClose())
		{
			ret = mdi_webcam_start(WEBCAM_CLAM_CLOSE_ROTATE, banding, mmi_usb_webcam_event_hdlr);
		}
		else
		{
			ret = mdi_webcam_start(WEBCAM_CLAM_OPEN_ROTATE, banding, mmi_usb_webcam_event_hdlr);
		}

		if (ret == MDI_RES_WEBCAM_ERR_FAILED)
		{
			/* USB is connected, but camera part cant start correctly */

			mdi_webcam_stop();
			mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_CONNECTED;
			
			#if 0//JiaShuo Delete for webcam
			str_id = mdi_util_get_mdi_error_info(ret, &err_type);
			mmi_display_popup((UI_string_type)GetString(str_id), err_type);
			#endif
			
			DeleteNHistory(1);
		}
		else
		{
			mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_LISTENING;
		}

	}
	else if (mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_PAUSE)
	{
	#ifdef MMI_ON_HARDWARE_P

		/* resume webcam */
		if (IsClamClose())
		{
			ret = mdi_webcam_resume(WEBCAM_CLAM_CLOSE_ROTATE);
		}
		else
		{
			ret = mdi_webcam_resume(WEBCAM_CLAM_OPEN_ROTATE);
		}
		#endif
		if (ret == MDI_RES_WEBCAM_ERR_FAILED)
		{
			#ifdef MMI_ON_HARDWARE_P

			mdi_webcam_stop();
			#endif
			mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_CONNECTED;
			#if 0//JiaShuo Delete for webcam
			str_id = mdi_util_get_mdi_error_info(ret, &err_type);
			mmi_display_popup((UI_string_type)GetString(str_id), err_type);
			#endif
			DeleteScreenIfPresent(SCR_ID_USB_WEBCAM);
		}
		else
		{
			mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_RUNNING;
		}
	}
	else if (mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_DISCONNECT)
	{
	
		#if 0//JiaShuo Delete for webcam
		mmi_display_popup(
			(UI_string_type)GetString(STR_ID_USB_NOTIFY_WEBCAM_ERROR), 
			MMI_EVENT_FAILURE);  
		#endif
		DeleteScreenIfPresent(SCR_ID_USB_WEBCAM);
	}
	else
	{
		MMI_ASSERT(0);	/* shall not enter here */
	}
#endif
}


/*****************************************************************************
 * FUNCTION
 *	mmi_usb_webcam_exit_app_scr
 * DESCRIPTION
 *	exit of webcam screen
 * PARAMETERS
 *	void
 * RETURNS
 *	void
 *****************************************************************************/
static void mmi_usb_webcam_exit_app_scr(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/
	U16 screen_id;
	MDI_RESULT ret;

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
#if 0//JiaShuo Delete for webcam
	MMI_TRACE(MMI_COMMON_TRC_G8_DEV, MMI_TRC_WEBCAM_EXIT_APP_SCR);
	MMI_TRACE(MMI_COMMON_TRC_G8_DEV, MMI_TRC_WEBCAM_STATE, mmi_usb_ptr->webcam_state);	 
#endif

#ifdef HORIZONTAL_CAMERA
	gdi_lcd_set_rotate(GDI_LAYER_ROTATE_0);
	gdi_layer_resize(UI_device_width, UI_device_height);
	gdi_layer_clear(GDI_COLOR_WHITE);
#endif /* HORIZONTAL_CAMERA */ 

	/* restore partial screen saver */
	mmi_phnset_enable_partial_ss();
	/* stop bg drawing */
	gdi_image_stop_animation_all();

	/* resume background audio */
	mdi_audio_resume_background_play();

	/* resume LCD patten */
	StopLEDPatternPowerOn();

	/* check who exit webcam screen */
	screen_id = GetActiveScreenId();

	if (mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_RUNNING)
	{
	
		/* pause webcam */
		ret = mdi_webcam_pause();
		if (ret == MDI_RES_WEBCAM_ERR_FAILED)
		{
			/* USB is connected, but camera part cannot start correctly */
				#ifdef MMI_ON_HARDWARE_P

			mdi_webcam_stop(); 
				#endif
			mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_CONNECTED;
		}
		else
		{
			mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_PAUSE;
		}

	}
	else if (mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_LISTENING)
	{
		#ifdef MMI_ON_HARDWARE_P

		mdi_webcam_stop();
		#endif
		mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_CONNECTED;
	}

#ifdef __MMI_SUBLCD__
	/* exit sublcd */
	if (mmi_usb_ptr->webcam_is_sub_display)
	{
		GoBackSubLCDHistory();
		mmi_usb_ptr->webcam_is_sub_display = FALSE;
	}
#endif /* __MMI_SUBLCD__ */ 

#if 0 // defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__)
	/* unhook clam close handler */
	mmi_frm_clear_interrupt_event_handler(MMI_FRM_INT_CLAM_CLOSE);
#endif /* defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__)*/ 

	/* clear all key handler - restore to default key handling for end key */
	ClearInputEventHandler(MMI_DEVICE_KEY);
#if 0//JiaShuo Delete for webcam
	mmi_frm_clear_interrupt_event_handler(MMI_FRM_INT_WAP_MMS | MMI_FRM_INT_BT_TASK);
#endif
}


/*****************************************************************************
 * FUNCTION
 *	mmi_usb_webcam_is_active
 * DESCRIPTION
 *	is webcam active or not
 * PARAMETERS
 *	void
 * RETURNS
 *	MMI_BOOL
 *****************************************************************************/
MMI_BOOL mmi_usb_webcam_is_active(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	if (mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_DISCONNECT)
	{
		return MMI_FALSE;
	}
	else
	{
		return MMI_TRUE;
	}

}


/*****************************************************************************
 * FUNCTION
 *	mmi_usb_webcam_event_hdlr
 * DESCRIPTION
 *	callback function when webcam screen start has some problem
 * PARAMETERS
 *	ret 	[IN]		
 * RETURNS
 *	void
 *****************************************************************************/
static void mmi_usb_webcam_event_hdlr(MDI_RESULT ret, BOOL camera_workable)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
#if 0//JiaShuo Delete for webcam
	MMI_TRACE(MMI_COMMON_TRC_G8_DEV, MMI_TRC_WEBCAM_EVENT_HDLR, ret);	 
	MMI_TRACE(MMI_COMMON_TRC_G8_DEV, MMI_TRC_WEBCAM_STATE, mmi_usb_ptr->webcam_state); 
#endif

	switch (ret)
	{
		case MDI_RES_WEBCAM_SUCCEED:
			/* start successful */
			if(mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_LISTENING)
			{
				mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_RUNNING;
			}
			break;

		case MDI_RES_WEBCAM_ERR_FAILED:
			if(mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_LISTENING)
			{
				#ifdef MMI_ON_HARDWARE_P

				mdi_webcam_stop();
				#endif
				mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_DISCONNECT;

				if(!camera_workable)
				{
					#if 0//JiaShuo Delete for webcam
					mmi_display_popup((UI_string_type)GetString(STR_ID_USB_NOTIFY_WEBCAM_CAM_ERROR), MMI_EVENT_FAILURE);
					#endif
					DeleteScreenIfPresent(SCR_ID_USB_WEBCAM);
				}
				/* If camera is workable, the fail is due to cable plug out.					 */ 
				/* So, don't display pop up or it will have a conflict with "USB plug out popup" */ 			
			}
			break;

		case MDI_RES_WEBCAM_DRV_ABORT:
			if (mmi_usb_ptr->webcam_state != MMI_USB_WEBCAM_DISCONNECT)
			{
				#ifdef MMI_ON_HARDWARE_P

				mdi_webcam_stop();
				#endif
				mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_DISCONNECT;
				#if 0//JiaShuo Delete for webcam
				mmi_display_popup((UI_string_type)GetString(STR_ID_USB_WEBCAM_STOPPED), MMI_EVENT_FAILURE);
				#endif

				DeleteScreenIfPresent(SCR_ID_USB_WEBCAM);
			}
			break;

		default:
			/* shall not enter here */
			MMI_ASSERT(0);
			break;
	}

}


/*****************************************************************************
 * FUNCTION
 *	mmi_usb_webcam_del_screen_id_hdlr
 * DESCRIPTION
 *	callback function when webcam screen removed from history webcam screen
 * PARAMETERS
 *	ptr 	[?] 	
 * RETURNS
 *	void
 *****************************************************************************/
static U8 mmi_usb_webcam_del_screen_id_hdlr(void *ptr)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	/* exit by other screens */
	if ((mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_RUNNING) ||
		(mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_LISTENING) ||
		(mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_PAUSE))
	{
		/* close webcam */
			#ifdef MMI_ON_HARDWARE_P

		mdi_webcam_stop();
			#endif
		mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_DISCONNECT;
	}
	else if (mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_CONNECTED)
	{
		mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_DISCONNECT;
	}

	return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *	mmi_usb_webcam_gpio_event_hdlr
 * DESCRIPTION
 *	clam handler
 * PARAMETERS
 *	ptr 	[?] 	
 * RETURNS
 *	void
 *****************************************************************************/
#if 0 //defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__)
static U8 mmi_usb_webcam_gpio_event_hdlr(mmi_frm_int_event_type interrupt_event)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/
	MDI_RESULT ret;
	MMI_ID_TYPE str_id = 0;
	mmi_event_notify_enum err_type = 0;
	
	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	if (interrupt_event == MMI_FRM_INT_CLAM_CLOSE)
	{
		mmi_usb_ptr->webcam_is_clam_close = TRUE;
		ret = mdi_webcam_rotate(WEBCAM_CLAM_CLOSE_ROTATE);		  
		if (ret == MDI_RES_WEBCAM_ERR_FAILED)
		{
	#ifdef MMI_ON_HARDWARE_P

			mdi_webcam_stop();
		#endif
			/* USB is connected, but camera part cant start correctly */
			mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_CONNECTED;

			str_id = mdi_util_get_mdi_error_info(ret, &err_type);
			mmi_display_popup((UI_string_type)GetString(str_id), err_type);

			DeleteScreenIfPresent(SCR_ID_USB_WEBCAM);
			return FALSE;
		}
		SetClamState(EXT_DEV_CLAM_CLOSE);
		mmi_gpio_detect_send_ack(EXT_DEV_CLAM_CLOSE);			 
		/* return TRUE means will block this interrut from passing default handler */
		return TRUE;
	}
	else if (interrupt_event == MMI_FRM_INT_CLAM_OPEN)
	{
		mmi_usb_ptr->webcam_is_clam_close = FALSE;
		ret = mdi_webcam_rotate(WEBCAM_CLAM_OPEN_ROTATE);
		if (ret == MDI_RES_WEBCAM_ERR_FAILED)
		{
			#ifdef MMI_ON_HARDWARE_P

			mdi_webcam_stop();
			#endif
			/* USB is connected, but camera part cant start correctly */
			mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_CONNECTED;

			str_id = mdi_util_get_mdi_error_info(ret, &err_type);
			mmi_display_popup((UI_string_type)GetString(str_id), err_type);

			DeleteScreenIfPresent(SCR_ID_USB_WEBCAM);
			return FALSE;
		}
		SetClamState(EXT_DEV_CLAM_OPEN);
		mmi_gpio_detect_send_ack(EXT_DEV_CLAM_OPEN);
		/* return TRUE means will block this interrut from passing default handler */
		return TRUE;
	}
	else if (interrupt_event & MMI_FRM_INT_EARPHONE)
	{
		SetInterruptPopupDisplay(POPUP_ALL_OFF);
	}

	return FALSE;

}


/*****************************************************************************
 * FUNCTION
 *	mmi_usb_webcam_gpio_post_event_hdlr
 * DESCRIPTION
 *	clam handler
 * PARAMETERS
 *	ptr 	[?] 	
 * RETURNS
 *	void
 *****************************************************************************/
static U8 mmi_usb_webcam_gpio_post_event_hdlr(mmi_frm_int_event_type interrupt_event)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	if (interrupt_event & MMI_FRM_INT_EARPHONE)
	{
		SetInterruptPopupDisplay(POPUP_ALL_ON);
	}

	return FALSE;

}
#endif /* defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__) */
 

/*****************************************************************************
 * FUNCTION
 *	mmi_usb_webcam_rsk_release
 * DESCRIPTION
 *	rsk release handler, will stop webcam
 * PARAMETERS
 *	void
 * RETURNS
 *	void
 *****************************************************************************/
static void mmi_usb_webcam_rsk_release(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	if ((mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_LISTENING) ||
		(mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_RUNNING) ||
		(mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_PAUSE))
	{
		/* close webcam */
			#ifdef MMI_ON_HARDWARE_P

		mdi_webcam_stop();
			#endif
		mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_DISCONNECT;
	}
#ifdef  __MMI_BACKGROUND_CALL__ 
	else if (mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_CONNECTED)
	{
		mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_DISCONNECT;
	}
#endif
	else
	{
#if 0//JiaShuo Delete for webcam
		MMI_ASSERT(0);
#endif
	}

	GoBackHistory();
}



/*****************************************************************************
 * FUNCTION
 *	mmi_usb_webcam_capture_key_press
 * DESCRIPTION
 *	capture key is pressed, will capture an image to PC side
 * PARAMETERS
 *	void
 * RETURNS
 *	void
 *****************************************************************************/
static void mmi_usb_webcam_capture_key_press(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	#if 0//JiaShuo Delete for webcam
	if (mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_RUNNING)
	{
		mdi_webcam_capture();

		/* flush all key event */
		ClearKeyEvents();
	}
	#endif
}


/*****************************************************************************
 * FUNCTION
 *	mmi_usb_webcam_plug_out
 * DESCRIPTION
 *	webcam calbe plug out
 * PARAMETERS
 *	void
 * RETURNS
 *	void
 *****************************************************************************/
static void mmi_usb_webcam_plug_out(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/
	U16 screen_id;

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	/* stop webcam if it is running */
	if ((mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_RUNNING) ||
		(mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_LISTENING) ||
		(mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_CONNECTED) || 
		(mmi_usb_ptr->webcam_state == MMI_USB_WEBCAM_PAUSE))
	{
		/* stop webcam */
			#ifdef MMI_ON_HARDWARE_P

		mdi_webcam_stop();
			#endif
		mmi_usb_ptr->webcam_state = MMI_USB_WEBCAM_DISCONNECT;
	}

	screen_id = GetActiveScreenId();

	DeleteScreenIfPresent(SCR_ID_USB_WEBCAM);

	/* if is in webcam screen, leave it */
	if (screen_id == SCR_ID_USB_WEBCAM)
	{
		GoBackHistory();
	}
}


/*****************************************************************************
 * FUNCTION
 *	mmi_usb_webcam_entry_sublcd_scr
 * DESCRIPTION
 *	entry sublcd screen, if still remain on standard MMI sublcd screen, time update will
 *	redraw sublcd while mainlcd is previewing, will cause driver assert.
 * PARAMETERS
 *	void
 * RETURNS
 *	void
 *****************************************************************************/
#if defined(__MMI_SUBLCD__)
static void mmi_usb_webcam_entry_sublcd_scr(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/
	S32 lcd_width;
	S32 lcd_height;
	S32 image_width;
	S32 image_height;

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	ExecSubLCDCurrExitHandler();
	ShowCategory331Screen();
	SetSubLCDExitHandler(mmi_usb_webcam_exit_sublcd_scr);

	gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
	gdi_lcd_get_dimension(&lcd_width, &lcd_height);

	gdi_layer_reset_clip();

	gdi_layer_clear_background(GDI_COLOR_BLACK);

	gdi_image_get_dimension_id(IMG_ID_WEBCAM_SUB_BG, &image_width, &image_height);
	gdi_image_draw_id((lcd_width - image_width) >> 1, (lcd_height - image_height) >> 1, IMG_ID_WEBCAM_SUB_BG);
	gdi_layer_blt_base_layer(0, 0, lcd_width - 1, lcd_height - 1);

	gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);

	mmi_usb_ptr->webcam_is_sub_display = TRUE;
}


/*****************************************************************************
 * FUNCTION
 *	mmi_usb_webcam_exit_sublcd_scr
 * DESCRIPTION
 *	exit sublcd screen
 * PARAMETERS
 *	void
 * RETURNS
 *	void
 *****************************************************************************/
static void mmi_usb_webcam_exit_sublcd_scr(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	SubLCDHistoryNode SubLCDHistory;

	SubLCDHistory.entryFuncPtr = mmi_usb_webcam_entry_sublcd_scr;
	AddSubLCDHistory(&SubLCDHistory);

	mmi_usb_ptr->webcam_is_sub_display = FALSE;
}
#endif /* defined(__MMI_SUBLCD__) */ 
#endif /* __MMI_WEBCAM__ */ 


/*****************************************************************************
 * FUNCTION
 *  mmi_usb_app_deinit
 * DESCRIPTION
 *  App de-init for entering USB mode, ex: close file handler in use
 * PARAMETERS
 *  p       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_usb_app_deinit(void *p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (; counter_of_deinit < MAX_DEINIT_CALLBACK + 1; counter_of_deinit++)
    {
        U8 to_perform = TRUE;

        if (counter_of_deinit &&
            mmi_usb_deinit_callback[counter_of_deinit - 1].RspHdlr &&
            mmi_usb_deinit_callback[counter_of_deinit - 1].checkfunc &&
            mmi_usb_deinit_callback[counter_of_deinit - 1].checkfunc())
        {
            mmi_usb_deinit_callback[counter_of_deinit - 1].RspHdlr(p);
            SetProtocolEventHandler(
                mmi_usb_deinit_callback[counter_of_deinit - 1].RspHdlr,
                mmi_usb_deinit_callback[counter_of_deinit - 1].eventID);
        }

        if (mmi_usb_deinit_callback[counter_of_deinit].checkfunc)
        {
            to_perform = mmi_usb_deinit_callback[counter_of_deinit].checkfunc();
        }

        if (to_perform)
        {
            if (mmi_usb_deinit_callback[counter_of_deinit].callback)
            {
                mmi_usb_deinit_callback[counter_of_deinit].callback();
            }
            else
            {
                MMI_ASSERT(counter_of_deinit == MAX_DEINIT_CALLBACK);
            }

            if (mmi_usb_deinit_callback[counter_of_deinit].eventID != 0)
            {
                SetProtocolEventHandler(mmi_usb_app_deinit, mmi_usb_deinit_callback[counter_of_deinit].eventID);
                counter_of_deinit++;
                return;
            }
        }
    }
#if 0
    /* reach here only when all callback are done */
    if (mmi_usb_deinit_callback[MAX_DEINIT_CALLBACK].callback == NULL)
    {
        SendUsbConfigToHW(DEVUSB_CFG_ACTION_MASS_STORAGE);
    }
#endif

    counter_of_deinit = 0;
    mmi_usb_deinit_callback[MAX_DEINIT_CALLBACK].callback = NULL;
}



#endif /* __MMI_USB_SUPPORT__ */


