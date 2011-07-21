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
 * GeneralDeviceInterface.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Utility functions for GPIO
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/***************************************************************************** 
* Include
*****************************************************************************/
#include "mmi_features.h" 

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "gpioinc.h"
#include "keybrd.h"
#include "mmi_data_types.h"
#include "queuegprot.h"
#include "idleappdef.h"
#include "timerevents.h"
#include "eventsgprot.h"
#include "wgui_categories.h"
#include "wgui_status_icons.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "profilesdefs.h"
#include "idleappprot.h"
#include "simdetectiongexdcl.h"
#include "globaldefs.h"
#include "callmanagementgprot.h"
#include "commonscreens.h"
#include "sublcdhistorygprot.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "nvramtype.h"
#include "debuginitdef.h"
#include "mmiapi_enum.h"
#include "mmiapi_struct.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "lcd_if.h"
#include "gdi_include.h"

#include "custom_equipment.h"

#include "alarmframeworkprot.h"
#include "phonebooktypes.h"
#include "wpssprotos.h"   /* phnsetgetscrsvrsetings(),  */
#include "vdoplyscrenum.h" 

#include "mmi_trace.h"
#include"adp_timer.h"
#include "med_global.h"
#include "fmradiodef.h"
#include "fmradiotype.h"
// 1. to suspend timer in sleep mode 
// 2. to prevent answer call when clam open but no call
 
#ifdef __J2ME__
#include "javadef.h"
#endif

#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
#endif


#ifdef __MMI_SLIDE__
#include "quickaccessmenuitemprot.h"
#include "statusiconres.h"
#endif
#include "led.h"

#undef __NEWSIMULATOR


/*  Include: PS header file */
#ifdef MMI_ON_HARDWARE_P
#include "l4dr.h"
#include "lcd_sw_inc.h"
#include "di.h"
#include "dm.h"
#include "adp_events.h"
#include "callstructuremanagementprot.h"
#include "commonstubsprot.h"
#include "audioplayerprot.h"
#include "mainmenuprot.h"
extern PROFILE gcurrentprofile;
#endif

#if defined(MOTION_SENSOR_SUPPORT)
#include "gsensorapp.h"
#endif

#include "resource_audio.h"
#ifdef DUAL_TFLASH_SUPPORT
#include "mcd_m.h"
#endif
#ifdef __MMI_AUDIO_PLAYER__
extern BOOL mmi_audply_is_play_activated( void );
#endif // __MMI_AUDIO_PLAYER__

#ifdef __MMI_FM_RADIO__
extern BOOL mmi_fmrdo_is_power_on( void );
extern void mmi_fmrdo_power_on_main_screen( BOOL turn_on );
extern void mmi_fmrdo_power_on( BOOL turn_on );
extern mmi_fmrdo_struct g_fmrdo;
#endif // __MMI_FM_RADIO__

static void BacklightTimerHdlr(void);

extern void Usb_stop_application(void);
extern void Usb_resume_application(void);
void TFlashDisplayPopup( BOOL is_plugin );
#if defined(CAM_MD_SUPPORT)
extern void csd_start_sw_motionsensor(void);
extern void csd_stop_sw_motionsensor(void);
#endif

#if defined(MMI_2D_SENSOR_SUPPORT)
extern void csd_mmi_app_handle_2d_sensor_message(BOOL para1,BOOL para2);
extern void csd_mmi_turnoff_2d_sensor_by_backlight(void);
extern void csd_mmi_turnon_2d_sensor_by_backlight(void);
#endif
#if defined(CAM_MD_SUPPORT)
extern void csd_start_sw_motionsensor(void);
extern void csd_stop_sw_motionsensor(void);
#endif

/***************************************************************************** 
* Local variable
*****************************************************************************/
extern event_scheduler *event_scheduler1_ptr;

U8 lcd_backlight=1; /* 0 = dark, 1 = half light, 2 = full light  */
static U8 isTimerSuspend; // Is UI timer (aligned) suspended. 0 == non-suspend, 1 == suspend

static U8 gbacklight_mode = 1;
static U8 gpioOutOfCtrl;
static U8 sublcd_backlight; /* 0 = dark. 1 = light */
static U8 keypad_backlight;	/* 0 = dark. 1 = light */
// 0 = no timer, 1 = timer for half mode backlight , 2 = timer for sleep mode, 3 = in sleep mode 
static U8 gBackLightTimerStatus=1; 
static U8 IsCallerGroupPatternPlaying;

/***************************************************************************** 
* Local function
*****************************************************************************/
static BOOL backlight_check_call_present(void)
{
	/* We should not use GetTotalCallCount() because it does not handle automatic call redial */
	if (isInCall() && !IsWapCallPresent())
		return MMI_TRUE;
	else
		return MMI_FALSE;
}


/***************************************************************************** 
* Global variable
*****************************************************************************/
#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__)&& !defined(__AMNT__)
U8 gMMIClamState=0;  /* Sync with Drv state */
#else
U8 gMMIClamState=1;
#endif

U8 prevMode =AUD_MODE_NORMAL;
U8 isEarphonePlugged=0;

/*Jinzh Add Start For 6424 Ver: TBM780  on 2007-8-28 17:36 */
U8 isEarphonePlugged_1=0;  
/*Jinzh Add End  For 6424 Ver: TBM780  on 2007-8-28 17:36 */
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
 
extern BOOL  gIsInSleepMode;   
#endif

#ifdef __MMI_ENGINEER_MODE__ 
U32	gClamOpenCnt=0;
U32	gClamCloseCnt=0;
#endif


 
/*** Keep device state for concurrency accessing ***/
/*** The concept could be extended  to a Device manager ***/
U32 gDeviceState=0x0;


/***************************************************************************** 
* External variable
*****************************************************************************/
extern U16 gKeyPadLockFlag;
extern pBOOL processingNow;
extern U8 mmi_msg_need_mem_full_ind(void);

#if ( defined(__MMI_ENGINEER_MODE__) || defined(__MMI_FACTORY_MODE__) )
extern U8 gCurrentMode;
#endif 

#ifdef __MMI_BT_PROFILE__
extern BOOL g_mmi_profiles_bt_in_call;

#ifdef __MMI_HFP_SUPPORT__
extern BOOL g_mmi_profiles_bt_always_use_in_call;
#endif /* __MMI_HFP_SUPPORT__ */ 
#endif /* __MMI_BT_PROFILE__ */ 

extern U8 gactivatedprofile;
/***************************************************************************** 
* External functitons
*****************************************************************************/
void SetBacklightMode(void* info);
void GpioSendMsg( U16 src_mod_id, U32 msg_id,  void *local_param_ptr, void *peer_buf_ptr);
extern void UI_BLT_double_buffer(s32 x1, s32 y1, s32 x2 , s32 y2);
#if defined( __MMI_LCMSLEEP_BACK2IDLE__)
void LCMSleepGotoIdleScreen(void);
#endif

#ifdef MMI_ON_WIN32
//Added for BkLight simulation
extern void simulate_lcd_bklight_turn_off(void);
extern void simulate_sublcd_bklight_turn_off(void);
extern void simulate_lcd_bklight_turn_on(void);
extern void simulate_sublcd_bklight_turn_on(void);
#endif /* MMI_ON_WIN32 */





/*******************************************************/
/*                                                     */
/*     Gpio Indication Handler and related function    */
/*                                                     */
/*******************************************************/

BOOL mmi_gpio_is_earphone_plug_in( void )
{
	return (1 == GetHandsetInPhone());
}

/*****************************************************************************
* FUNCTION
*  GpioDetectInd
* DESCRIPTION
*   Event handler of gpio detect indication
*
* PARAMETERS: mmi_eq_gpio_detect_ind_struct *
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
	extern void ShowSubLCDAnalogClockScreen(void);
	void ClamCloseGotoIdleScreen(void);
#endif
extern U8 	gPowerOffPeriod;
void GpioDetectInd(void * info)
{
	mmi_eq_gpio_detect_ind_struct *gpioDetectInd;
	gpioDetectInd = (mmi_eq_gpio_detect_ind_struct *) info;
	
	switch(gpioDetectInd->gpio_device)
	{
//wug modify used to cs200 start 20070821
      /* Earphone plug in/out */
		case EXT_DEV_EARPHONE:
   		if(gpioDetectInd->on_off==1)
   		   EarphoneConnectHdlr( MMI_TRUE );
   		else
   		   EarphoneConnectHdlr( MMI_FALSE );
   		break;
//wug modify used to cs200 end 20070821
//wug modify used to cs200 start 20070821
      /* SEND(END) key press */
		case EXT_DEV_KEY_1: 
			
			if(!IdleIsPowerOnPeriod())
				ProcessHeadsetKeyPress();
			
		   break;
//wug modify used to cs200 end 20070821

		
#ifdef __MMI_CLAMSHELL__

      /* Clam open */
		case EXT_DEV_CLAM_OPEN:
		#if 0
			if((0 == g_pwr_context[0].IdleScreenArrived)||(0 == g_pwr_context[1].IdleScreenArrived)||(gPowerOffPeriod))
				{
				TurnOnBacklight(1);
					return;
				}
		#endif
		   {
			   #ifdef __MMI_ENGINEER_MODE__ 
			   gClamOpenCnt++;
			   #endif /* __MMI_ENGINEER_MODE__ */
		   }
		
			gMMIClamState=1;
//				lcd_backlight = 0;
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
				ForceSubLCDScreen(ShowIdleAppSubLcd);
#endif
			//if(!AlmIsRTCPwron())  //dyj del 20060514
			   TurnOnBacklight(1);

			NotifyCMClampOpen();

			//if(!g_pwr_context.PowerOnPeriod)   //dyj del 20060515
			{
				
				if( IsClamToneAllow() )
				   playRequestedTone(COVER_OPEN_TONE);
				
 
#ifdef __MMI_SLIDE__
				if(gKeyPadLockFlag)
				{
					if(isInCall())
					{
						/* Not display unlock message if there is an incoming call when the slide is closed */
						StopTimer(KEYPADUNLOCKEDMESSAGE_TIMER);
						gKeyPadLockFlag = 0;
						#ifndef __MMI_KEYPADLOCK_WITH_KEYTONE__
						mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
						#endif
						#ifdef __MMI_TOUCH_SCREEN__
						ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_UNLOCK);
						#else
						IdleResetStatusIcon(STATUS_ICON_KEYPAD_LOCK);	
						#endif
					}
					else
					{
						HandlePoundKeyForUnlock();
					}
 
				}
#endif
 

			#ifdef __MMI_CLAMOPEN_STOP_SCRSAVER__
				if( GetActiveScreenId() == IDLE_SCREEN_SCREENSAVER_ID )
					GoBackHistory(); /* Screensaver is running */
				else 
			#endif
					mmi_idle_restart_screensaver_timer();  
			}
			
		   break;

		   
      /* Clam close */
		case EXT_DEV_CLAM_CLOSE:
	#if 0
        if((0 == g_pwr_context[0].IdleScreenArrived)||(0 == g_pwr_context[1].IdleScreenArrived)||(gPowerOffPeriod))
				{
				TurnOnBacklight(1);
					return;
				}
	#endif
if(g_phb_cntx.processing)
	{
		ExecuteCurrKeyHandler(KEY_RSK,KEY_EVENT_DOWN);
		ExecuteCurrKeyHandler(KEY_RSK,KEY_EVENT_UP);
	}
 		   {
            #ifdef __MMI_ENGINEER_MODE__ 
			   gClamCloseCnt++;
			   #endif /* __MMI_ENGINEER_MODE__ */
 		   }

	#ifdef __MMI_SUB_WALLPAPER__
		/* removed , not to update sublcd, video may be playing */
		//SUBLCD_StartWallpaperAnimation();
	#endif

		gMMIClamState=0;

#ifndef  __MMI_SLIDE__				

		NotifyCMClampClose();

#endif //__MMI_SLIDE__				

#if (defined __MMI_CLAMCLOSE_BACK2IDLE__ || defined __MMI_SUBLCD_SHOW_ANALOG_CLOCK__)
		ClamCloseGotoIdleScreen();
#endif
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
//			if ( g_idle_context.IsOnIdleScreen )
				ForceSubLCDScreen(ShowSubLCDAnalogClockScreen);
#endif

			//if(!g_pwr_context.PowerOnPeriod)  //dyj del 20060515
			{
#ifndef  __MMI_SLIDE__				
				CloseBacklight();
#else
				if(AlmIsRTCPwron())   //dyj del 20060514
					TurnOnBacklight(0);	
				else
				   TurnOnBacklight(1);
#endif //__MMI_SLIDE__				
				
				if( IsClamToneAllow() )
					playRequestedTone(COVER_CLOSE_TONE);

				if (GetActiveScreenId() == IDLE_SCREEN_ID)				
				{
					IdleSetKeyPadLockFlag();				
				}	
			}
			
   		break;

#endif /* __MMI_CLAMSHELL__ */
#if defined(MMI_2D_SENSOR_SUPPORT)
		case EXT_DEV_2D_SENSOR:
			csd_mmi_app_handle_2d_sensor_message(gpioDetectInd->para1,gpioDetectInd->para2);
		break;
#endif
#ifdef __TFLASH_MOUNT_DYNAMIC__
		case EXT_DEV_TFLASH_DETECT:
                    TFlashDisplayPopup(gpioDetectInd->on_off);
                break;
#endif
              default:
                 break;
	}	

    
   /* Shall reset this flag in the end of msg handler func */
   SetInterruptPopupDisplay(POPUP_ALL_ON);
}

#if defined(MOTION_SENSOR_SUPPORT)
//extern void iphone_rotate_entry(U32 rotate_type);
//extern short gHorFlag;
void GsensorRotateHdlr(void * info)
{
	mmi_eq_gsensor_rotate_ind_struct *GsensorRotateInd;
	GsensorRotateInd = (mmi_eq_gsensor_rotate_ind_struct *) info;

	mmi_trace(4,"GsensorRotateHdlr->GsensorRotateInd->rotate:%d",GsensorRotateInd->rotate);
	switch(GsensorRotateInd->rotate)
	{
		case GSENSOR_ROCATE_TYPE_ANTICLOCKWISE:
			//iphone_rotate_entry(GSENSOR_ROCATE_TYPE_ANTICLOCKWISE);
			//gHorFlag = 0;
			//Show_IPhone_Idle();
			break;
		case GSENSOR_ROCATE_TYPE_CLOCKWISE:
			//iphone_rotate_entry(GSENSOR_ROCATE_TYPE_CLOCKWISE);
			//gHorFlag = 1;
			//Show_IPhone_Idle();
			break;
		default:
			break;
	}
		

}
#endif

/*******************************************************/
/*                                                     */
/*    Earphone Handling Functions                      */
/*                                                     */
/*******************************************************/


/*****************************************************************************
* FUNCTION
*  EarphoneConnectHdlr
* DESCRIPTION
*   General handler for earphone plug in/out
*
* PARAMETERS: is_in
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
extern void LoudSpeakerFunction(void);
extern pBOOL LoudSpeaker(void);
extern S16  SetLoudSpeaker(pBOOL loudspeaker);
void EarphoneConnectHdlr( BOOL is_in )
{
   /* Do not switch profile if not normal power on mode */
   /* 1. Charge power on --> X */
   /* 2. Alarm power on --> X  */
   /* 3. RTC power on --> X   */
   //if(!g_charbat_context.PowerOnChargingPeriod && !AlmIsRTCPwron() )
	 
	MMI_BOOL Lspk = MMI_FALSE;
	#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
		disable_statusbar_fill_background();
	#endif
	 
//dyj modify start 20060515

        Lspk = LoudSpeaker();

     if( g_pwr_context[0].PowerOnMode == POWER_ON_KEYPAD \
       || g_pwr_context[0].PowerOnMode == POWER_ON_EXCEPTION )
     {
     
            EarphoneDisplayPopup(is_in);
		if( is_in ){
			GpioEarphonePlugIn();
//cong.li added them for simulator to set the handset state.			
#ifdef MMI_ON_WIN32			
			SetHandsetInPhone(1);
#endif
		}else{
			GpioEarphonePlugOut();
#ifdef MMI_ON_WIN32			
			SetHandsetInPhone(0);
#endif
		}
            
            if ((is_in )&&(Lspk == MMI_TRUE) &&isInCall())
            {
                SetLoudSpeaker(MMI_FALSE); ;
                LoudSpeakerFunction();

            }
     }
     else
	 {

		if( is_in )
		{
			SetHandsetInPhone(1);  
			SendAudioModeRequest( AUD_MODE_HEADSET );
		#if (defined (__MMI_STATUS_ICON_EN_EARPHONE__) || defined(__MMI_BT_SUPPORT__))
			IdleSetStatusIcon(STATUS_ICON_EARPHONE_INDICATOR);
		#endif
		}
        else
        {
           SetHandsetInPhone(0);
           SendAudioModeRequest( AUD_MODE_NORMAL );
        }
     }
    
      #ifdef __MMI_UI_DALMATIAN_STATUSBAR__
	      enable_statusbar_fill_background();
      #endif
     
}


/*****************************************************************************
* FUNCTION
*  EarphoneDisplayPopup
* DESCRIPTION
*   Display earphone plug in/out popup
*
* PARAMETERS: BOOL
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void EarphoneDisplayPopup( BOOL is_plugin )
{
   U16 str_id = 0;
   U8 pp_flag, tone_id=0;

    
   /* Framework support Popup display permisstion flag */
   /* Application who wanna stop popup has to intercept related messages and set the flag */
   pp_flag = GetInterruptPopupDisplay();
   if( (pp_flag & POPUP_DSPL_ON) == 0 )
      return;
   
   if( pp_flag & POPUP_TONE_ON )
      tone_id = AUX_TONE;

   /* Not allow to display popup */
    
   /* 2. During power on animation */
   /* 3. Power on mode is neither Keypad nor Exception (Means, Alarm, Charger, Usb) */
   /* 4. In Engineer mode */
   /* 5. In Factory mode  */
   //if(!g_pwr_context.PowerOnPeriod && !g_charbat_context.PowerOnChargingPeriod && !AlmIsRTCPwron())

    if( (GetTransitionToActiveFlag() == MMI_TRUE) || (IdleIsPowerOnPeriod())||(!IS_IDLE_ARRIVED)
      || (g_pwr_context[0].PowerOnMode != POWER_ON_KEYPAD && g_pwr_context[0].PowerOnMode != POWER_ON_EXCEPTION)
      )
    {
	return;
    }
   	

#ifdef __MMI_ENGINEER_MODE__
    if(gCurrentMode == ENGINEERING_MODE)
       return;
#endif

#ifdef __MMI_FACTORY_MODE__
    if(gCurrentMode == FACTORY_MODE)
       return;
#endif

   /* Display popup according plug in/out */
   if( is_plugin == MMI_TRUE ) /* Plug in */
     str_id = STR_EARPHONE_POPUP;
   else /* Plug out */
     str_id = STR_HEADSET_DEACTIVATE_MESSAGE;

   /* Should turn on backlight first to make LCD sleep out */
   TurnOnBacklight(1);

   DisplayPopup( (U8 *)GetString(str_id), \
                 IMG_EARPHONE_POPUP_MAINLCD, 0, 700, tone_id);

#ifdef __MMI_SUBLCD__
   if( MMI_TRUE == is_plugin )
      DisplaySubLcdPopup( IMG_EARPHONE_POPUP_SUBLCD );
#endif

}



/*****************************************************************************
* FUNCTION
*  GpioEarphonePlugIn
* DESCRIPTION
*   General action after earphone plug in
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*
*****************************************************************************/
void GpioEarphonePlugIn(void)
{
  
   
   if (TRUE)   //dyj add  20060515
   {
      PRINT_INFORMATION( ("GpioEarphonePlugIn(): Active Headset profile"));

      //prevMode = AUD_MODE_HEADSET;   Deleted by jinzh:20070730
      //SendAudioModeRequest( prevMode );	Deleted by jinzh:20070730
#if (!defined( __MMI_FM_RADIO_BIND_EARPHONE__))
#ifdef	__MMI_FM_RADIO__
      if(g_fmrdo.is_power_on == FALSE)
      {
          	SendAudioModeRequest( AUD_MODE_HEADSET );  //Changed by jinzh:20070730
      }
      else
      {
          mmi_fmrdo_power_on( FALSE );
          mmi_fmrdo_power_on( TRUE );
      }
#else
	SendAudioModeRequest( AUD_MODE_HEADSET );  //Changed by jinzh:20070730
#endif
#else
#if (!defined( __MMI_FM_RADIO_BIND_EARPHONE__))
    //  if(g_fmrdo.is_power_on == FALSE)
#endif
      {
      	SendAudioModeRequest( AUD_MODE_HEADSET );  //Changed by jinzh:20070730
      }
#endif
	//SetModeSpeechVolume();   //deleted by jinzh:20070731
          #ifdef __MMI_BT_PROFILE__
        if (gactivatedprofile == MMI_PROFILE_BT)
        {
        #ifdef __MMI_HFP_SUPPORT__
            mmi_profiles_bt_disconnect_req(MMI_PROFILES_BT_HFP_SCO);
        #endif 
 
	 #if defined(__MMI_BT_BCHS_SUPPORT__)&& defined(__MMI_A2DP_SUPPORT__)
            mdi_audio_bt_a2dp_turn_off();
        #endif 
//end
        }
    #endif /* __MMI_BT_PROFILE__ */ 
      ActivateHeadset();  //reopend by Jinzh:20070718
#if defined( __MMI_FM_RADIO__ ) && defined( __MMI_FM_RADIO_BIND_EARPHONE__ )
      mmi_fmrdo_power_on_main_screen( TRUE );
#endif
   }
   else
      SendAudioModeRequest( AUD_MODE_HEADSET );

#if (defined (__MMI_STATUS_ICON_EN_EARPHONE__)|| defined(__MMI_BT_SUPPORT__))
	IdleSetStatusIcon(STATUS_ICON_EARPHONE_INDICATOR);
#endif

}



/*****************************************************************************
* FUNCTION
*  GpioEarphonePlugOut
* DESCRIPTION
*   General action after earphone plug out
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void GpioEarphonePlugOut(void)
{
	   
#if defined( __MMI_FM_RADIO__ ) && defined( __MMI_FM_RADIO_BIND_EARPHONE__ )
      mmi_fmrdo_power_on( FALSE );
#endif
    #ifdef __MMI_BT_PROFILE__
    #ifdef __MMI_HFP_SUPPORT__
        if (mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_ACL))
        {
        
         	mmi_trace(g_sw_BT, "mmi_profiles_bt_is_connected::mmi_profiles_bt_activate_profile");
            mmi_profiles_bt_activate_profile();

    #ifdef __MMI_BT_AUDIO_VIA_SCO__
            /* Connect SCO according to HFP audio path setting */
            if (mmi_bt_is_aud2hf())
    #else /*__MMI_BT_AUDIO_VIA_SCO__*/
        #ifdef __MMI_BT_FM_VIA_SCO__
            /* Need to check FM radio if FM via SCO on  */
            if ((g_mmi_profiles_bt_in_call || mdi_audio_is_playing(MDI_AUDIO_PLAY_FM_RADIO)) &&
                (g_mmi_profiles_bt_always_use_in_call || mmi_bt_is_aud2hf()))
        #else
            if (g_mmi_profiles_bt_in_call && (g_mmi_profiles_bt_always_use_in_call || mmi_bt_is_aud2hf()))
        #endif
    #endif /*__MMI_BT_AUDIO_VIA_SCO__*/
            {
                mmi_profiles_bt_connect_req(MMI_PROFILES_BT_HFP_SCO);
            }
        }
    #endif /* __MMI_HFP_SUPPORT__ */ 
 
     #if defined(__MMI_BT_BCHS_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)
        if (mmi_profiles_bt_is_connected(MMI_PROFILES_BT_A2DP_SIG))
        {
            if (gactivatedprofile != MMI_PROFILE_BT)
            {
                mmi_profiles_bt_activate_profile();
            }
            if (!g_mmi_profiles_bt_in_call)
            {
                mdi_audio_bt_a2dp_turn_on();
            #if defined(__MMI_AUDIO_PLAYER__) && defined(__MMI_AUDIO_SPECTRUM_DISPLAY__) && defined(__MMI_A2DP_PROHIBIT_AUDIO_SPECTRUM_DISPLAY__)
             //   mmi_audply_disable_spectrum_display();
            #endif 
            }
        }
    #endif /* __MMI_BT_BCHS_SUPPORT__ && __MMI_A2DP_SUPPORT__ */ 	
    #endif /* __MMI_BT_PROFILE__ */ 



	
        if ((gactivatedprofile == MMI_PROFILE_HEADSET)||(gactivatedprofile == MMI_PROFILE_BT) )
        {
#ifdef TGT_GALLITE_G800               
                 if(
			#ifdef __MMI_A2DP_SUPPORT__
			mmi_audply_is_output_to_bt()&&
                     #endif
			mmi_audply_is_playing())
                    prevMode=AUD_MODE_HEADSET;
#endif                 

              if (prevMode!=AUD_MODE_LOUDSPK)
              {
                 mmi_trace(g_sw_BT,"GpioEarphonePlugOut(): !AUD_MODE_LOUDSPK " );
                 prevMode=AUD_MODE_NORMAL;
                 
                 //mp3 playing and use bt g800 should switch AUD_MODE_HEADSET
#ifdef TGT_GALLITE_G800               
                 if(
		      #ifdef __MMI_A2DP_SUPPORT__
				 	mmi_audply_is_output_to_bt()&&
			#endif
			mmi_audply_is_playing())
                    prevMode=AUD_MODE_HEADSET;
#endif                 
        	#if (!defined( __MMI_FM_RADIO_BIND_EARPHONE__))
			#ifdef	__MMI_FM_RADIO__
              	if(g_fmrdo.is_power_on == FALSE)
			#endif		
        	#endif
                {
                   SendAudioModeRequest(prevMode);
                }
                //SetModeSpeechVolume();    deleted by jinzh:20070731
              }
        	  else   //Added by jinzh:20070730
        	  {
        	  	 SendAudioModeRequest(prevMode);
        	  }
              if ((gactivatedprofile == MMI_PROFILE_HEADSET))
          	{
                          mmi_trace(g_sw_BT,"GpioEarphonePlugOut(): Deactive Headset profile" );
                          DeactivateHeadset();  //dyj del 20060514
          	}
   }
  

#if (defined (__MMI_STATUS_ICON_EN_EARPHONE__)|| defined (__MMI_BT_SUPPORT__))
	#if defined ( __MMI_BT_SUPPORT__ )
	if(!mmi_bt_is_hs_connected())
	#endif	
	{
		IdleResetStatusIcon(STATUS_ICON_EARPHONE_INDICATOR);
	}
#endif

}







/*******************************************************/
/*                                                     */
/*    Clam handling functions                          */
/*                                                     */
/*******************************************************/


/*****************************************************************************
* FUNCTION
*  IsClamToneAllow
* DESCRIPTION
*   Query if clam tone allow to play (No other audio tone is playing, except clam tone it self
*
* PARAMETERS: mmi_eq_gpio_detect_ind_struct *
*
* RETURNS: BOOL
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
#if defined (__MMI_SLIDE__)
BOOL IsClamToneAllow(void)
{
#if 1

   if(  (!g_charbat_context.PowerOnCharger) 
	 && ( !AlmIsRTCPwron())  
	  && ( !isInCall() )  /* Even for CSD call */
	 && ( !AlmIsTonePlaying()) 
#ifdef __MMI_AUDIO_PLAYER__
	  && ( !mmi_audply_is_play_activated())
#endif
#ifdef __MMI_FM_RADIO__
         &&( !mmi_fmrdo_is_power_on() )
#endif // __MMI_FM_RADIO__
     )
     return MMI_TRUE;
   else
     return MMI_FALSE;

#else
	return MMI_TRUE;
#endif
}
#else
BOOL IsClamToneAllow(void)
{
    return MMI_TRUE;
}
#endif

/*****************************************************************************
* FUNCTION
*  IsClamClose
* DESCRIPTION
*   Query clam statue
*
* PARAMETERS: void
*
* RETURNS: BOOL
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
BOOL IsClamClose(void)
{
#ifdef __MMI_CLAMSHELL__

   if( gMMIClamState == 1 ) /* 1: Open */
      return MMI_FALSE;
   else
      return MMI_TRUE;

#else

   return MMI_FALSE;  /* always open */

#endif
}

/*****************************************************************************
* FUNCTION
*  SetClamState
* DESCRIPTION
*  Set clam state
* PARAMETERS
*  clam_state 	IN 	clam state
* RETURNS
*  void 
*****************************************************************************/
void SetClamState(U16 clam_state)
{
#ifdef __MMI_CLAMSHELL__

	if(clam_state == EXT_DEV_CLAM_OPEN)
	{
		gMMIClamState = 1; /* open */
	}
	else if(clam_state == EXT_DEV_CLAM_CLOSE)
	{
		gMMIClamState = 0; /* close */
	}
	else
	{
		MMI_ASSERT(0); /* wrong parameter */
	}
	
#else /* __MMI_CLAMSHELL__ */
	MMI_ASSERT(0);	/* shall not call this function when no clam support */
#endif /* __MMI_CLAMSHELL__ */


}


/*****************************************************************************
* FUNCTION
*  ClamCloseGotoIdleScreen
* DESCRIPTION
*   Function for clam close go back to idle screen 
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
extern S32 is_on_idlescreen(void);
#if (defined( __MMI_CLAMCLOSE_BACK2IDLE__) || defined (__MMI_SUBLCD_SHOW_ANALOG_CLOCK__) )
void ClamCloseGotoIdleScreen(void)
{
   if( (1 == GetHandsetInPhone()
   #if defined(__MMI_BT_SUPPORT__)
         || mmi_bt_is_incall_aud_swap2bt() == MMI_TRUE
   #endif         
         ) && backlight_check_call_present())
      return;

   PRINT_INFORMATION( ("ClamCloseGotoIdleScreen():END Key Press, FP: %x", GetKeyHandler (KEY_END, KEY_EVENT_DOWN) ) );

#ifdef __MMI_FACTORY_MODE__ 
      if (gCurrentMode == FACTORY_MODE)
         return;
#endif
	ExecuteCurrKeyHandler( KEY_END, KEY_EVENT_DOWN );
    
}
#endif






/*******************************************************/
/*                                                     */
/*    LED pattern handling functions                   */
/*                                                     */
/*******************************************************/

/*****************************************************************************
* FUNCTION
*  GetBackgroundPattern
* DESCRIPTION
*   Returns Existing Pattern
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

U8 GetBackgroundPattern( void )
{
#if  1 //dyj del 20060515
    U8 band_pattern = LED_NoService;

    if( g_charbat_context.LowBatteryFlag && (!g_charbat_context.isChargerConnected) )  
    {
      band_pattern = LED_BATTERY_LOW;
    }
#ifdef __MMI_LED_POWERON_CHARGEFULL__
    else if ( g_charbat_context.ChargingComplete == 1 )
    {
      band_pattern = LED_FullCharging;
    }
#endif
    else if ( g_pwr_context[0].RoamingFlag )
    {
      band_pattern = LED_Roaming;
    }
    else if( g_pwr_context[0].CurrentServiceType == FULL_SERVICE )
    {
      switch( g_pwr_context[0].CurrentCampOnBand )
      {
         case NW_BAND_PGSM900:
         case NW_BAND_EGSM900:
         case NW_BAND_RGSM900:
            band_pattern = LED_Connect900;
            break;
         case NW_BAND_DCS1800:
            band_pattern = LED_Connect1800;
            break;
         case NW_BAND_PCS1900:
            band_pattern = LED_Connect1900;
            break;
         default:
            band_pattern = LED_Connect900;
            break;
      }
    }
    else if( g_pwr_context[0].CurrentServiceType == LIMITED_SERVICE )
    {
         band_pattern = LED_LimitService;
    }
    else
    {
         band_pattern = LED_NoService;
    }

    return band_pattern;
 #endif   
}



/*****************************************************************************
* FUNCTION
*  PlayPatternReq
* DESCRIPTION
*   Plays a pattern on vibrator or LED
*
* PARAMETERS: pattern: pattern to be played
*             action: ON/OFF
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void PlayPatternReq(U8 pattern, U8 action)
{
#if 1  //dyj del 20060515
       
      /* To make compiler happy: unsigned comparison with 0 is always true: '>=' */
      /* Could not make sure we won't change the LED_PATTERN_BEGIN to non-zero value */
      signed char pat = (signed char) pattern;
	
#ifdef __MMI_ENGINEER_MODE__ 
      if( gCurrentMode == ENGINEERING_MODE)
         return;
#endif

#ifdef __MMI_FACTORY_MODE__ 
      if( gCurrentMode == FACTORY_MODE)
         return;
#endif

		/* Other module might request to obtain control of LEDs from MMI */
      if( gpioOutOfCtrl == 1 && !(pat >= VIB_PATTERN_BEGIN /*&& pat <= VIB_PATTERN_END comparison is always true*/ ))
         return;

        
       /*   Background LED pattern (for example: Camp on LED, Low battery...etc) */
       /* play request might comes anytime, it will interrupt the on going  */
       /* caller group pattern */
      if( (IsCallerGroupPatternPlaying) \
         && ( ((pat >= LED_PATTERN_BEGIN) && (pat < LED_Caller1)) ||((pat > LED_Caller6) && (pat <= LED_PATTERN_END))  )
         )
         return;

      SendPlayPatternReqToHW(pat, action,0);

      /* Start play CampOn LED pattern */
      if ((pat >= LED_PATTERN_BEGIN && pat < LED_PATTERN_END)
         && (action == PLAY_STOP)
         && (IS_IDLE_ARRIVED)
         && (pat != LED_OnCharging/*Added by jinzh:20070715*/))  
		{TRACE_EF(g_sw_AUDIO,"GetBackgroundPattern()=%d",GetBackgroundPattern());
              SendPlayPatternReqToHW( GetBackgroundPattern(), PLAY_REPEAT , 0);
		}
 #endif
}
void PlayPatternReq_Ext(U8 pattern, U8 action, U32 dur)
{
#if 1  //dyj del 20060515
      /*** Robin 0915 ***/
      /* To make compiler happy: unsigned comparison with 0 is always true: '>=' */
      /* Could not make sure we won't change the LED_PATTERN_BEGIN to non-zero value */
      signed char pat = (signed char) pattern;

#ifdef __MMI_ENGINEER_MODE__ 
      if( gCurrentMode == ENGINEERING_MODE)
         return;
#endif

#ifdef __MMI_FACTORY_MODE__ 
      if( gCurrentMode == FACTORY_MODE)
         return;
#endif

		/* Other module might request to obtain control of LEDs from MMI */
      if( gpioOutOfCtrl == 1 && !(pat >= VIB_PATTERN_BEGIN /*&& pat <= VIB_PATTERN_END comparison is always true*/ ))
         return;

       /* Robin 1223 for SCR #893 */
       /*   Background LED pattern (for example: Camp on LED, Low battery...etc) */
       /* play request might comes anytime, it will interrupt the on going  */
       /* caller group pattern */
      if( (IsCallerGroupPatternPlaying) \
         && ( ((pat >= LED_PATTERN_BEGIN) && (pat < LED_Caller1)) ||((pat > LED_Caller6) && (pat <= LED_PATTERN_END))  )
         )
         return;

      SendPlayPatternReqToHW(pat, action, dur);

      /* Start play CampOn LED pattern */
      if ((pat >= LED_PATTERN_BEGIN && pat < LED_PATTERN_END)
         && (action == PLAY_STOP)
         && (IS_IDLE_ARRIVED)
         && (pat != LED_OnCharging/*Added by jinzh:20070715*/)) //Robin 1203
		{TRACE_EF(g_sw_AUDIO,"GetBackgroundPattern()=%d",GetBackgroundPattern());
              SendPlayPatternReqToHW( GetBackgroundPattern(), PLAY_REPEAT, dur );
		}
 #endif
}

#ifdef __LED__
static	int led_i = 0;  // count for all led on
static  int led_j = 2;  // count for poweron led to display 2 loop
static	int led_t = 5;  // count for which led on
#define LED_ALL_LIGHT_DELAY 500
#define LED_POWER_ON_DELAY 80
#define LED_POWER_OFF_DELAY 100
  void set_led_poweron(int cnt)
{
  
	if (cnt == 0)
	{
		set_led_alllight();
	}
  else if (cnt == 1)
	{
		set_led(LED_RED_5, LED_OPEN);
    set_led(LED_RED_1, LED_OPEN);
    set_led(LED_GREEN_2, LED_OPEN);
	}
	else if (cnt == 2)
	{
		set_led(LED_YELLOW_4, LED_OPEN);
    set_led(LED_RED_5, LED_OPEN);
    set_led(LED_RED_1, LED_OPEN);
	}
	else if (cnt == 3)
	{
		set_led(LED_BLUE_3, LED_OPEN);
    set_led(LED_YELLOW_4, LED_OPEN);
    set_led(LED_RED_5, LED_OPEN);
	}
	else if (cnt == 4)
	{
		set_led(LED_GREEN_2, LED_OPEN);
    set_led(LED_BLUE_3, LED_OPEN);
    set_led(LED_YELLOW_4, LED_OPEN);
	}
	else if (cnt == 5)
	{
		set_led(LED_RED_1, LED_OPEN);
    set_led(LED_GREEN_2, LED_OPEN);
    set_led(LED_BLUE_3, LED_OPEN);
	} 
}
void set_led_poweroff(int cnt)
{
  
	if (cnt == 0)
	{
		set_led_alllight();
	}
  else if (cnt == 1)
	{
		set_led(LED_RED_5, LED_OPEN);
	}
	else if (cnt == 2)
	{
		set_led(LED_YELLOW_4, LED_OPEN);
	}
	else if (cnt == 3)
	{
		set_led(LED_BLUE_3, LED_OPEN);
	}
	else if (cnt == 4)
	{
		set_led(LED_GREEN_2, LED_OPEN);
	}
	else if (cnt == 5)
	{
		set_led(LED_RED_1, LED_OPEN);
	} 
}
void StopPowerOnOffLED(void)
{
  led_i = 0;
  led_j = 2;
	led_t = 5;
  set_led_alldark();
	StopTimer(POWER_ON_OFF_LED);
}
void showPowerOn(void)
{
  if(led_j == 0)
  {
    led_j = 2;
    StopPowerOnOffLED();
    return;
  }
		set_led_open();
		set_led_alldark();
		set_led_poweron(led_t);
		set_led_close();
  if(led_i == 2)
  {
    set_led_open();
		set_led_alldark();
		set_led_poweron(0);
		set_led_close();
    led_i = 0;
    led_t = 5;
    led_j--;
    StartTimer(POWER_ON_OFF_LED, LED_ALL_LIGHT_DELAY, showPowerOn );
    return;
  }
  if(led_t == 1)
  {
    led_t = 5;
    led_i++;
  }
  else
  {
  led_t--;
  }
  StartTimer(POWER_ON_OFF_LED, LED_POWER_ON_DELAY, showPowerOn );
}
void showPowerOff(void)
{
		set_led_open();
		set_led_alldark();
		set_led_poweroff(led_t);
		set_led_close();
  if(led_i == 2)
  {
    set_led_open();
		set_led_alldark();
		set_led_poweroff(0);
		set_led_close();
    led_i = 0;
    led_t = 5;
    StartTimer(POWER_ON_OFF_LED, LED_ALL_LIGHT_DELAY, showPowerOn );
    return;
  }
  if(led_t == 1)
  {
    led_t = 5;
    led_i++;
  }
  else
  {
  led_t--;
  }
  StartTimer(POWER_ON_OFF_LED, LED_POWER_OFF_DELAY, showPowerOn );
}
void showPowerOnLED(void)
{
	StartTimer(POWER_ON_OFF_LED, LED_POWER_ON_DELAY, showPowerOn );
}
void showPowerOffLED(void)
{
	StartTimer(POWER_ON_OFF_LED, LED_POWER_OFF_DELAY, showPowerOff );
}
#endif
/*****************************************************************************
* FUNCTION
*  PlayPatternCallerGroup
* DESCRIPTION
*   Plays pattern as caller group functionality
*
* PARAMETERS: pattern: pattern enum
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void PlayPatternCallerGroup(MMI_LEDLIGHT_STATUS_ENUM pattern)
{
    IsCallerGroupPatternPlaying = 1;
    
	switch(pattern)
	{
		case COLOR_PATTERN1:
			StartLEDPatternCaller1();
			break;
		case COLOR_PATTERN2:
			StartLEDPatternCaller2();
			break;
		case COLOR_PATTERN3:
			StartLEDPatternCaller3();
			break;
		case COLOR_PATTERN4:
			StartLEDPatternCaller4();
			break;
		case COLOR_PATTERN5:
			StartLEDPatternCaller5();
			break;
		case COLOR_PATTERN6:
			StartLEDPatternCaller6();
			break;
	     default:
	          IsCallerGroupPatternPlaying = 0;
	          break;
	}
}


/*****************************************************************************
* FUNCTION
*  StopPatternCallerGroup
* DESCRIPTION
*   Stops pattern as caller group functionality
*
* PARAMETERS: pattern: pattern enum
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

void StopPatternCallerGroup(MMI_LEDLIGHT_STATUS_ENUM pattern)
{
     IsCallerGroupPatternPlaying = 0;
     
	switch(pattern)
	{
		case COLOR_PATTERN1:
			StopLEDPatternCaller1();
			break;
		case COLOR_PATTERN2:
			StopLEDPatternCaller2();
			break;
		case COLOR_PATTERN3:
			StopLEDPatternCaller3();
			break;
		case COLOR_PATTERN4:
			StopLEDPatternCaller4();
			break;
		case COLOR_PATTERN5:
			StopLEDPatternCaller5();
			break;
		case COLOR_PATTERN6:
			StopLEDPatternCaller6();
			break;
	}
}







/*******************************************************/
/*                                                     */
/*    Tone ID/Range Qery functions                     */
/*                                                     */
/*******************************************************/

/*****************************************************************************
* FUNCTION
*  GetRingToneRange
* DESCRIPTION
*   Gets Tone Range for Ring Tones
*
* PARAMETERS: startid: Starting Range Value
*             totalid: Total Tones
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

void GetRingToneRange(U8 *startId, U8*totalId)
{
#if  1   
#ifndef MMI_ON_WIN32
	U8 endId;
	custom_get_fixed_imelody_range(startId, &endId);
	*totalId = endId - *startId + 1;
#else
	*startId=1;
	*totalId=15;
#endif
#endif
}



/*****************************************************************************
* FUNCTION
*  GetMidiRange
* DESCRIPTION
*   Gets Tone Range for Midi Tones
*
* PARAMETERS: startid: Starting Range Value
*             totalid: Total Tones
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

void GetMidiRange(U8 *startId, U8*totalId)
{
#if  1   
#ifndef MMI_ON_WIN32
	U8 endId;
	custom_get_fixed_midi_range(startId, &endId);
	*totalId=endId - *startId + 1;
#else
	*startId=16;
	*totalId=15;
#endif
#endif
}


/*****************************************************************************
* FUNCTION
*  GetToneRange
* DESCRIPTION
*   Gets Tone Range for General Tones
*
* PARAMETERS: startid: Starting Range Value
*             totalid: Total Tones
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

void GetToneRange(U8 *startId, U8*totalId)
{
#if  1   
#ifndef MMI_ON_WIN32	
	U8 endId;
	custom_get_fixed_sound_range(startId, &endId);
	*totalId=endId - *startId + 1;
#else
	*startId=31;
	*totalId=10;
#endif
#endif
}


/*****************************************************************************
* FUNCTION
*  GetMiscToneRange
* DESCRIPTION
*   Gets Tone Range for Misc Tones
*
* PARAMETERS: startid: Starting Range Value
*             totalid: Total Tones
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void GetMiscToneRange(U8 *startId, U8*totalId)
{
#if  1   
#ifndef MMI_ON_WIN32
	U8 endId;
	custom_get_fixed_message_sound_range(startId, &endId);
#if defined(__PROJECT_GALLITE_C01__) 
	*totalId = GetMiscMessageCount();
#else
	*totalId=endId - *startId + 1;
#endif
#else
	*startId=46;
	*totalId=10;
#endif
#endif
}




/*****************************************************************************
* FUNCTION
*  GpioWakeUpTimer
* DESCRIPTION
*   Wake up timer
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void GpioWakeUpTimer(void)
{
	/* resume timer if suspended */
	#if  0   
	if(isTimerSuspend == 1)
	{
		evshed_resume_all_events(event_scheduler1_ptr);
		isTimerSuspend = 0;
	}
	#endif
}



/*******************************************************/
/*                                                     */
/*    Backlight                                        */
/*                                                     */
/*******************************************************/


/*****************************************************************************
* FUNCTION
*  GpioTurnOffKeypadLight
* DESCRIPTION
*   turn off keypad backlight
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

void GpioTurnOffKeypadLight(void)
{
#ifdef MMI_ON_HARDWARE_P	
	/* turn off keypad backlight */	
	StopTimer(GPIO_KEYPAD_LIGHT_TIMER);
	if(keypad_backlight == 1)
	{
		KeyPadBackLightOff();
#ifdef __MMI_RUNNING_LED__
        stopKeyPadLED();
#endif
		keypad_backlight = 0;
	}
#endif
}


/*****************************************************************************
* FUNCTION
*  GpioLCDSleepIn
* DESCRIPTION
*   Wake up LCM
* PARAMETERS:
*   None
* RETURNS: 
*   None
* GLOBALS AFFECTED: 
*   None
*****************************************************************************/
extern VOID Set_BackLight_Flag_False(VOID);
extern VOID DRV_SetStatusSleepIn(BOOL bStatus);
extern void StopMyTimerSleepIN();
extern BOOL aud_is_in_state(INT32 state);
extern void mdi_stop_background_timer(void);
extern volatile BOOL gUsbDetected;
#if defined(__NATIVE_JAVA__)
extern void JVMEntrySleep(void);
extern void JVMEntryActive(void);
#endif

void GpioLCDSleepIn(void)
{
#ifdef MMI_ON_HARDWARE_P
    mmi_trace(g_sw_SYSTEM, "GpioLCDSleepIn");
    if(gIsInSleepMode == FALSE)
    {
        /* stop screensaver if need, since it might playing video */

        /*WangRui rescover for fix bug 10328 , fixbug5860 in  mmi_scrsvr_lcd_sleepin_handler */

        mmi_scrsvr_lcd_sleepin_handler();
        /*WangRui rescover end*/

        if (GetActiveScreenId() == SCR_ID_VDOPLY_PLAYER)//close video before low power.sheen
        {
            EntryNewScreen(SCR_ID_VDOPLY_PLAYER_SLEEPIN, NULL, NULL, NULL);
            mdi_stop_background_timer();//JiaShuo add,stop background timer to avoid music restore playing procedure in exit handler
            // entry full screen - force mainlcd status bar not update 
            entry_full_screen();
        }

        lcd_sleep_in(MAIN_LCD);
        lcd_screen_on(MAIN_LCD, FALSE);
        //添加进入睡眠的函数
        //SetGPIOLevelReq( GPIO_DEV_LED_KEY, LED_LIGHT_LEVEL0 );
        DM_KeyBackLightClose();
        csw_SetResourceActivity(CSW_LP_RESOURCE_KEYBACKLIGHT, CSW_SYS_FREQ_32K);
#ifdef __LED__
        set_led_open();
        set_led_alldark();
        set_led_close();//wug add used to tc9751
#elif defined(__MMI_RUNNING_LED__)
        stopKeyPadLED();
#endif
        StopTimer(TIMER_REAL_TIME_CLOCK);

        if(0 == gUsbDetected)
        {
            // Caoxh add begin 2008-09-27
            // Set the LCD status to sleep in divice layer when LCD is closed.
            DRV_SetStatusSleepIn(TRUE);
            // Relese divice source when LCD close.
            FS_ReleaseDev("MMC0");
#ifdef DUAL_TFLASH_SUPPORT
            FS_ReleaseDev("MMC1");
#endif
        }

        gIsInSleepMode = TRUE;
        Set_BackLight_Flag_False();
#ifdef __MMI_TOUCH_SCREEN__
        mmi_pen_block( );
#endif
        /*if (GetActiveScreenId() == SCR_ID_VDOPLY_PLAYER)
        {
            EntryNewScreen(SCR_ID_VDOPLY_PLAYER_SLEEPIN, NULL, NULL, NULL);
            mdi_stop_background_timer();//JiaShuo add,stop background timer to avoid music restore playing procedure in exit handler
            // entry full screen - force mainlcd status bar not update 
            entry_full_screen();
        }*/
        SuspendAllNoAlignedCosTimer();
#if defined(CAM_MD_SUPPORT)
        csd_stop_sw_motionsensor();
#endif
#if defined(__NATIVE_JAVA__)
		JVMEntrySleep();
#endif		
        mmi_trace(g_sw_SYSTEM, "entry deep sleep");
        // dingjian 20080108 modify for Jasper FF Edition
        //PM_LPMEnable(TRUE); //this function should move later
        DM_LpwMmiInactive();
        csw_SetResourceActivity(CSW_LP_RESOURCE_MMIACTIVE, CSW_SYS_FREQ_32K);
    }
#endif
}


/*****************************************************************************
* FUNCTION
*  GpioLCDSleepOut
* DESCRIPTION
*   Sleep LCM
* PARAMETERS:
*   None
* RETURNS: 
*   None
* GLOBALS AFFECTED: 
*   None
*****************************************************************************/
#include "vdoplyscrenum.h"
extern void PSIDateTimerProc(void);
extern void mmi_vdoply_turnOnBacklight();
extern VOID Set_BackLight_Flag_True(VOID);
extern void  StartMyTimerSleepOUT();
void GpioLCDSleepOut(void)
{
#ifdef MMI_ON_HARDWARE_P
	if(gIsInSleepMode)
	{
		lcd_sleep_out(MAIN_LCD);
		DRV_SetStatusSleepIn(FALSE);

		gIsInSleepMode = FALSE;
		Set_BackLight_Flag_True();

		#ifdef __MMI_SUB_WALLPAPER__
		/* removed , not to update sublcd, video may be playing */
		//SUBLCD_StartWallpaperAnimation();
		#endif
		//添加唤醒的函数
		mmi_trace(g_sw_SYSTEM, "exit deep sleep");
		StartTimer(TIMER_REAL_TIME_CLOCK, 100, PSIDateTimerProc);
		lcd_screen_on(MAIN_LCD, TRUE);
		DM_KeyBackLightOpen();
		csw_SetResourceActivity(CSW_LP_RESOURCE_KEYBACKLIGHT, CSW_SYS_FREQ_52M);
		csw_SetResourceActivity(CSW_LP_RESOURCE_MMIACTIVE, CSW_SYS_FREQ_104M);  
		DM_LpwMmiActive(); 

		if (GetActiveScreenId() == SCR_ID_VDOPLY_PLAYER_SLEEPIN)
		{
			GoBackHistory();
		}



		if (GetActiveScreenId() != SCR_ID_VDOPLY_PLAYER)
		{
			update_mainlcd_dt_display();
			update_sublcd_dt_display();
#if defined( __MMI_SUBLCD__ )
			gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
			gdi_lcd_get_dimension(&lcd_width, &lcd_height);
			UI_BLT_double_buffer(0,0,lcd_width -1, lcd_height -1);
			gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);		
#endif
			//UI_BLT_double_buffer(0,0,UI_device_width -1, UI_device_height -1);
		}


		/* resume screensaver if it is playing before sleep-in, must after lcd sleep out */
		mmi_scrsvr_lcd_sleepout_handler();// qiff move upon 	if (GetActiveScreenId() != SCR_ID_VDOPLY_PLAYER) for display
		//gdi_layer_blt_base_layer(0,0,UI_device_width -1, UI_device_height -1);

#ifdef __MMI_TOUCH_SCREEN__
		mmi_pen_unblock( );
#endif
		ResumeAllNoAlignedCosTimer();
#if defined(CAM_MD_SUPPORT)
          csd_start_sw_motionsensor();
#endif
#if defined(__NATIVE_JAVA__)
        JVMEntryActive();
#endif	

	}
#endif
}


/*****************************************************************************
* FUNCTION
*  GpioTurnOnKeypadLight
* DESCRIPTION
*   turn on keypad backlight
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void GpioTurnOnKeypadLight(void)
{
#ifdef MMI_ON_HARDWARE_P
	/* turn on keypad backlight if currently off and clam open */
	if( keypad_backlight == 0 && gMMIClamState == 1&& gKeyPadLockFlag == 0)
	{
		KeyPadBackLightOn();
		keypad_backlight = 1;
	}
#ifdef __MMI_RUNNING_LED__
        showKeyPadLED();
#endif
    //add by panxu 20061226
	StartTimer(GPIO_KEYPAD_LIGHT_TIMER, get_lcdbacklighttimer()*1000, GpioTurnOffKeypadLight);  //dyj del 20060514
#endif
}

#ifdef MMI_BK_LIGHT_DELAY_TIME
/*****************************************************************************
* FUNCTION
*	mmi_gpio_set_main_lcd_level_5
* DESCRIPTION
*  Set main LCD level to 5.
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
static void mmi_gpio_set_main_lcd_level_5(void)
{
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);
}
#endif


/*****************************************************************************
* FUNCTION
*  IsBacklightOn
* DESCRIPTION
*   Test whether the backlight is on (half-light or full-light)
*
* PARAMETERS: void
*
* RETURNS: MMI_BOOL
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
MMI_BOOL IsBacklightOn(void)
{
	if (lcd_backlight)
		return MMI_TRUE;
	else
		return MMI_FALSE;
}

BOOL KeepHalfLight()
{
#if 0
	if (isInCall()) //when in a call , half light the screen
	{
		return TRUE;
	}
#endif /* Guoj delete. It's not used now at 2009-9-29 */
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*  HalfModeBacklight
* DESCRIPTION
*   Takes the Backlight to Half Mode 
*   (Backlit is dimmed but not black)
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void HalfModeBacklight(void)
{
#ifdef MMI_ON_HARDWARE_P
   if(gbacklight_mode == 1)   // for AT command
   {
		if(sublcd_backlight == 0)
		{
			SetGPIOLevelReq(GPIO_DEV_LED_SUBLCD, LED_LIGHT_LEVEL5);
			sublcd_backlight = 1;         
		}
      
		if(lcd_backlight == 0 && gMMIClamState == 1) // sleep mode
		{
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
        GpioLCDSleepOut();

         UI_BLT_double_buffer(0,0,UI_device_width -1, UI_device_height -1);
//		   gdi_layer_blt_base_layer(0,0,UI_device_width -1, UI_device_height -1);

 			update_mainlcd_dt_display();
	      update_sublcd_dt_display();

			/* resume screensaver if it is playing before sleep-in, must after lcd sleep out */
			mmi_scrsvr_lcd_sleepout_handler();
 			
#endif
			SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL1);
			lcd_backlight = 1;        
		}      
      
		if( isTimerSuspend == 1)
		{
			 
			isTimerSuspend = 0;
		}	

		if( gBackLightTimerStatus > 1)
		{
			StopTimer(BACKLIGHT_TIMER);
			StartTimer(BACKLIGHT_TIMER,LCD_CLAM_OPEN_HALFLITE_TO_OFF_TIME,BacklightTimerHdlr);
			gBackLightTimerStatus = 2;
		}
   }
#endif
}
// CSD end


#if defined(__MMI_SYNC_LCD_GPIO_MODE__)
/*****************************************************************************
* FUNCTION
*	mmi_gpio_turn_on_all_light
* DESCRIPTION
*	Turn on all light to whole bright
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
static void mmi_gpio_turn_on_all_light(void)
{
#ifdef MMI_ON_HARDWARE_P
	GpioLCDSleepOut();
	SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);
	lcd_backlight = 2;
#endif
}

/*****************************************************************************
* FUNCTION
*	mmi_gpio_turn_off_all_light
* DESCRIPTION
*	Turn off all light to whole dark
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
static void mmi_gpio_turn_off_all_light(void)
{
#ifdef MMI_ON_HARDWARE_P
	/* set lcd backlight to half mode */
	if(lcd_backlight != 0) /* backlight is on */
	{
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL0);
		lcd_backlight = 0;
	}
#endif
}


/*****************************************************************************
* FUNCTION
*  TurnOffBacklight
* DESCRIPTION
*   It is typically paired with TurnOnBacklight(0) in order to resume backlight timer.
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void TurnOffBacklight(void)
{
	/* Share the same code for target and PC simulator */

	/* AT command disables backlight mechanism */
	if(gbacklight_mode == 0)
		return;

	if(gBackLightTimerStatus == 0)
	{		
		StopTimer(BACKLIGHT_TIMER);
		StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);  //dyj del 20060515
		gBackLightTimerStatus = 1;
	}
}


BOOL mmi_is_lcd_backlight_on(void)
{
	return(lcd_backlight == 2);
}
/*****************************************************************************
* FUNCTION
*  BacklightTimerHdlr
* DESCRIPTION
*  Timer handler to switch backlight state
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
static void BacklightTimerHdlr(void) //NOT use for our coolsand
{
	mmi_trace(g_sw_SYSTEM, "BacklightTimerHdlr");
	mmi_trace(g_sw_SYSTEM, "gBackLightTimerStatus is:%d",gBackLightTimerStatus);
#ifdef MMI_ON_HARDWARE_P

	/* AT command disables backlight mechanism */
	if(gbacklight_mode == 0)
		return;

	if(gBackLightTimerStatus == 0)
	{		
		StopTimer(BACKLIGHT_TIMER);
		StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);  //dyj del 20060515
		// dingjian 20071115 for Jasper LCD time
		//StartTimer(BACKLIGHT_TIMER,0xFFFF,BacklightTimerHdlr); 
		gBackLightTimerStatus = 1;
	}
	else if(gBackLightTimerStatus == 1) 
	{
		if(lcd_backlight == 2)
		{
			SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL1);			
			lcd_backlight = 1;
		}
		
		StopTimer(BACKLIGHT_TIMER);
		StartTimer(BACKLIGHT_TIMER,LCD_CLAM_OPEN_HALFLITE_TO_OFF_TIME,BacklightTimerHdlr);
		gBackLightTimerStatus = 2;		
	}
	else // gBackLightTimerStatus == 2
	{
		if( lcd_backlight == 1)
		{			
			if(!backlight_check_call_present() && (g_phb_cntx.processing == FALSE))
				 
			//	&& (mdi_audio_get_state() != MDI_AUDIO_RECORD) && (mdi_audio_get_state() != MDI_AUDIO_RECORD_PAUSED))
			{   			    	
				SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL0);						
			#ifdef __MMI_SUB_WALLPAPER__
			/* removed , not to update sublcd, video may be playing */
				//SUBLCD_StopWallpaperAnimation();
			#endif
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
				
			GpioLCDSleepIn();

			 
			#ifdef __MMI_LCMSLEEP_BACK2IDLE__
			LCMSleepGotoIdleScreen();
			#endif

#endif
				lcd_backlight = 0;			
			}			
 		    
			gBackLightTimerStatus = 3;			
		}

		#ifdef __MMI_CLAMSHELL__			
		if (isTimerSuspend == 0 && g_charbat_context.isChargerConnected == 0 && 
			!backlight_check_call_present() && mmi_msg_need_mem_full_ind() == 0 && g_phb_cntx.processing == FALSE)
			 
			//&& mdi_audio_get_state() != MDI_AUDIO_RECORD && mdi_audio_get_state() != MDI_AUDIO_RECORD_PAUSED )
		#else
		if (isTimerSuspend == 0 && g_charbat_context.isChargerConnected == 0 && !backlight_check_call_present() )
			 
		//   && g_phb_cntx.processing == FALSE && mdi_audio_get_state() != MDI_AUDIO_RECORD && mdi_audio_get_state() != MDI_AUDIO_RECORD_PAUSED )
		#endif				
		{
			 
			isTimerSuspend = 1;					
		}
	}
#else                              
     // For PC Simulator only

	if(gbacklight_mode == 1)
	{
		if(gBackLightTimerStatus == 0)
		{
			StopTimer(BACKLIGHT_TIMER);
			StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);  //dyj del 20060515
			gBackLightTimerStatus = 1;
		}
		else if(gBackLightTimerStatus == 1) 
		{
			if(lcd_backlight == 2)
			{
			//simulate lcd bklight turn off
			simulate_lcd_bklight_turn_off();
				lcd_backlight = 1;
			}
			if(sublcd_backlight == 1)
			{
			   //simulate sub lcd bklight turn off
			   simulate_sublcd_bklight_turn_off();
				sublcd_backlight = 0;
			}		
			StopTimer(BACKLIGHT_TIMER);
			StartTimer(BACKLIGHT_TIMER,LCD_CLAM_OPEN_HALFLITE_TO_OFF_TIME,BacklightTimerHdlr);
			gBackLightTimerStatus = 2;
		}
		else // gBackLightTimerStatus == 2
		{
			if(sublcd_backlight == 1)
			{
			//simulate sub lcd bklight turn off
			simulate_sublcd_bklight_turn_off();
				sublcd_backlight = 0;
			}
			if( lcd_backlight == 1)
			{
			simulate_lcd_bklight_turn_off();
			}
				
			gBackLightTimerStatus = 3;			
		}
	}
     // End of /* For PC Simulator only */
#endif
}



/*****************************************************************************
* FUNCTION
*  TurnOnBacklight
* DESCRIPTION
*   Turns On the backlight
*
* PARAMETERS: TimerFlag: 0 permanent backlight, 1 timer backlight
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void TurnOnBacklight(U8 TimerFlag)
{
	PRINT_INFORMATION( ("TurnOnBacklight: %d, %d, %d, %d, %d", TimerFlag, isTimerSuspend, lcd_backlight, gMMIClamState,gBackLightTimerStatus) );

#ifdef MMI_ON_HARDWARE_P
	/* AT command disables backlight mechanism */
	if(gbacklight_mode == 0)
		return;

#ifdef MMI_BK_LIGHT_DELAY_TIME
	StopMyTimer(GPIO_BACKLIGHT_DELAY_TIMER);
#endif

	/* resume timer if suspended */
	if(isTimerSuspend == 1)
	{
		 
		isTimerSuspend = 0;
	}

	/* lcd backlight */
	if(lcd_backlight == 0) 
	{	
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)

		GpioLCDSleepOut();
		if (GetActiveScreenId() != SCR_ID_VDOPLY_PLAYER)
		{
			update_mainlcd_dt_display();
			update_sublcd_dt_display();
			UI_BLT_double_buffer(0,0,UI_device_width -1, UI_device_height -1);
		}
		/* resume screensaver if it is playing before sleep-in */
		mmi_scrsvr_lcd_sleepout_handler();
#endif
#ifdef __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__
#ifdef MMI_BK_LIGHT_DELAY_TIME
		StartTimer(GPIO_BACKLIGHT_DELAY_TIMER,MMI_BK_LIGHT_DELAY_TIME,mmi_gpio_set_main_lcd_level_5);
#else
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);
#endif

      lcd_backlight = 2;
#else /* __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__ */
		if(gMMIClamState == 1) /* clam is opened */
		{
#ifdef MMI_BK_LIGHT_DELAY_TIME
		StartTimer(GPIO_BACKLIGHT_DELAY_TIMER,MMI_BK_LIGHT_DELAY_TIME,mmi_gpio_set_main_lcd_level_5);
#else
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);
#endif

			lcd_backlight = 2;
		}
		else		/* clam is closed */
		{
			SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL1);
			lcd_backlight = 1;
		}
#endif /* __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__ */
	}
	else if(lcd_backlight == 1)
	{

#ifdef __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);		
		lcd_backlight = 2;

#else /* __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__ */
#ifndef __MMI_SLIDE__
		//if(gMMIClamState == 1||!g_pwr_context.idleScreenFirst) /* clam is opened */ //dyj del 20060515
#endif			
		{
			SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);		
			lcd_backlight = 2;
		}
#endif /* __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__ */
	}
	
	/* turn on keypad backlight if currently off and clam open */
	GpioTurnOnKeypadLight();

	/* start/stop timer according to input parameter */
	if(TimerFlag == 0)
	{
		StopTimer(BACKLIGHT_TIMER);
		gBackLightTimerStatus = 0;
	}
	else if(gBackLightTimerStatus != 0) // start a timer
	{
		StopTimer(BACKLIGHT_TIMER);

#ifdef __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__
		StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);  //dyj del 20060515
		gBackLightTimerStatus = 1;

#else /* __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__ */
#if defined (__MMI_CLAMSHELL__)&&!defined(__MMI_SLIDE__)
		if(gMMIClamState == 1)	/* clam is opened */
		{
			StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);   //dyj del 20060515
			gBackLightTimerStatus = 1;
		}
		else	/* clam is closed */
		{
			StartTimer(BACKLIGHT_TIMER,LCD_CLAM_CLOSE_HALFLITE_TO_OFF_TIME,BacklightTimerHdlr);
			gBackLightTimerStatus = 2;
		}
#else
		StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);  //dyj del 20060515
		gBackLightTimerStatus = 1;
#endif
#endif /* __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__ */
	}
#else
     // For PC Simulator only
	if(gbacklight_mode == 1)
	{
		if(sublcd_backlight == 0)
		{
			//simulate turn on sublcd back light
			simulate_sublcd_bklight_turn_on();
			sublcd_backlight = 1;
		}
		
		if(lcd_backlight == 0)
		{	
		//simulate turn on lcd bklight
		simulate_lcd_bklight_turn_on();
		lcd_backlight = 2;
		}
		else if(lcd_backlight == 1)
		{
		//simulate turn on lcd bklight
		simulate_lcd_bklight_turn_on();
		lcd_backlight = 2;
		}
	}
	if(TimerFlag == 0)
	{
		StopTimer(BACKLIGHT_TIMER);
			gBackLightTimerStatus = 0;
	}
	else
	{
		if(gBackLightTimerStatus != 0) // start a timer
		{
			StopTimer(BACKLIGHT_TIMER);
			StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);  //dyj del 20060515
				gBackLightTimerStatus = 1;
			}			
		}
#endif
}


/*****************************************************************************
* FUNCTION
*  CloseBacklight
* DESCRIPTION
*   Immeditae Baclight Close not going thru Half Mode
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void CloseBacklight(void)
{
#ifdef MMI_ON_HARDWARE_P
	/* AT command disables backlight mechanism */
	if(gbacklight_mode == 0)
		return;

	/* stop backlight timer */
	if(gBackLightTimerStatus != 0)
		StopTimer(BACKLIGHT_TIMER);

#ifdef __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__
	if(lcd_backlight == 1) /* backlight is half light */
	{
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);
		lcd_backlight = 2;	
	}
	else if(lcd_backlight == 0) /* backlight is off */
	{

		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);
		lcd_backlight = 2;		
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
		GpioLCDSleepOut();

		update_mainlcd_dt_display();
  		update_sublcd_dt_display();
		gdi_layer_blt_previous(0,0,UI_device_width -1, UI_device_height -1);
		/* resume screensaver if it is playing before sleep-in, must after lcd sleep out */
		mmi_scrsvr_lcd_sleepout_handler();
#endif
	}

#else /*  __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__ */
	/* set lcd backlight to half mode */
	if(lcd_backlight == 2) /* backlight is on */
	{
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL1);
		lcd_backlight = 1;	
	}
	else if(lcd_backlight == 0) /* backlight is off */
	{
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL1);
		lcd_backlight = 1;		
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
		GpioLCDSleepOut();
		update_mainlcd_dt_display();
  		update_sublcd_dt_display();
		gdi_layer_blt_previous(0,0,UI_device_width -1, UI_device_height -1);

		/* resume screensaver if it is playing before sleep-in, must after lcd sleep out */
		mmi_scrsvr_lcd_sleepout_handler();
		
#endif
	}
#endif /* __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__ */

	/* turn off keypad backlight */	
	GpioTurnOffKeypadLight();
#ifdef __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__
	StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);   //dyj del 20060515
	gBackLightTimerStatus = 1;
#else /* __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__ */
	/* start a timer for half mode backlight */
	StartTimer(BACKLIGHT_TIMER,LCD_CLAM_CLOSE_HALFLITE_TO_OFF_TIME,BacklightTimerHdlr);
	gBackLightTimerStatus = 2;
#endif /* __MMI_GPIO_3_STAGE_SUB_BACKLIGHT__ */
#endif
}

#else /* !__MMI_SYNC_LCD_GPIO_MODE__ */
/*****************************************************************************
* FUNCTION
*	mmi_gpio_turn_on_all_light
* DESCRIPTION
*	Turn on all light to whole bright
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
static void mmi_gpio_turn_on_all_light(void)
{
#ifdef MMI_ON_HARDWARE_P
	GpioLCDSleepOut();
	SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);
	lcd_backlight = 2;

	SetGPIOLevelReq(GPIO_DEV_LED_SUBLCD, LED_LIGHT_LEVEL5);
	sublcd_backlight = 2;
#endif
}


/*****************************************************************************
* FUNCTION
*	mmi_gpio_turn_off_all_light
* DESCRIPTION
*	Turn off all light to whole dark
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
static void mmi_gpio_turn_off_all_light(void)
{
#ifdef MMI_ON_HARDWARE_P
	/* set lcd backlight to half mode */
	if(lcd_backlight != 0) /* backlight is on */
	{
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL0);
		lcd_backlight = 0;
	}

	/* turn on sub-lcd baclight */
	if(sublcd_backlight != 0)
	{
		SetGPIOLevelReq(GPIO_DEV_LED_SUBLCD, LED_LIGHT_LEVEL0);
		sublcd_backlight = 0;
	}		
#endif
}
/*****************************************************************************
* FUNCTION
*  TurnOffBacklight
* DESCRIPTION
*   Turns Off the backlight
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void TurnOffBacklight(void)
{
	/* Share the same code for target and PC simulator */

	/* AT command disables backlight mechanism */
	if(gbacklight_mode == 0)
		return;

	if(gBackLightTimerStatus == 0) /* resume backlight mechanism */
	{
		StopTimer(BACKLIGHT_TIMER);
		StartTimer(BACKLIGHT_TIMER, get_lcdbacklighttimer()*1000, BacklightTimerHdlr);  //dyj del 20060515
		gBackLightTimerStatus = 1;
	}
}

/*****************************************************************************
* FUNCTION
*  BacklightTimerHdlr
* DESCRIPTION
*  Timer handler to switch backlight state
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
#ifdef __MMI_SOUND_RECORDER__
extern BOOL Snd_getRecAmrRunState(VOID);//chenhe
#endif
BOOL mmi_is_on_alarm_pwron_confirm_scr(void);

static void BacklightTimerHdlr(void)
{
	mmi_trace(g_sw_SYSTEM, "BacklightTimerHdlr");
	TRACE_SYS_FUNCTION();
	TRACE_EF(g_sw_SYSTEM, "gbacklight_mode = %d, gBackLightTimerStatus = %d, lcd_backlight = %d", gbacklight_mode, gBackLightTimerStatus, lcd_backlight);
	
#ifdef MMI_ON_HARDWARE_P     
	if(gbacklight_mode == 0)
		return;
	
	if(gBackLightTimerStatus == 0) /* resume backlight mechanism */
	{
		StopTimer(BACKLIGHT_TIMER);
		StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr); 
		gBackLightTimerStatus = 1;
	}
	else if(gBackLightTimerStatus == 1) 
	{
		if(lcd_backlight == 2)	/* full light, switch to half light */
		{
			SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL1);				
			lcd_backlight = 1;
		}

		if(sublcd_backlight == 1)
		{			
			SetGPIOLevelReq(GPIO_DEV_LED_SUBLCD, LED_LIGHT_LEVEL0);			
		#ifdef __MMI_SUB_WALLPAPER__
			/* removed , not to update sublcd, video may be playing */
			//SUBLCD_StopWallpaperAnimation();
		#endif
			sublcd_backlight = 0;
		}		


		StopTimer(BACKLIGHT_TIMER);
		
	#ifdef __MMI_CLAMSHELL__	
		if(gMMIClamState == 1)
			StartTimer(BACKLIGHT_TIMER,LCD_CLAM_OPEN_HALFLITE_TO_OFF_TIME,BacklightTimerHdlr);
		else
  			StartTimer(BACKLIGHT_TIMER,LCD_CLAM_CLOSE_HALFLITE_TO_OFF_TIME,BacklightTimerHdlr);
   	#else
		StartTimer(BACKLIGHT_TIMER,LCD_CLAM_OPEN_HALFLITE_TO_OFF_TIME,BacklightTimerHdlr);
   	#endif
		
		if (!KeepHalfLight())
		{
			gBackLightTimerStatus = 2;	
		}	
	
	}
	else // gBackLightTimerStatus == 2
	{
		if(sublcd_backlight == 1)
		{
			SetGPIOLevelReq(GPIO_DEV_LED_SUBLCD, LED_LIGHT_LEVEL0);				
			sublcd_backlight = 0;
		#ifdef __MMI_SUB_WALLPAPER__
			/* removed , not to update sublcd, video may be playing */
			//SUBLCD_StopWallpaperAnimation();
		#endif

		}
			
		if( lcd_backlight == 1)
		{
			//if(!backlight_check_call_present() && (g_phb_cntx.processing == FALSE))//chenhe for gs, 20090119,backlight take effect on RF detection
			//	&& (mdi_audio_get_state() != MDI_AUDIO_RECORD) && (mdi_audio_get_state() != MDI_AUDIO_RECORD_PAUSED))
			{   			    			 
#ifdef __MMI_LCD_PARTIAL_ON__
				/* We do not enter partial display screen saver before idle screen */
				if ((IS_IDLE_ARRIVED ||g_pwr_context[0].SimRemoveFlag) &&
				   PhnsetIsScrSvrPartialOn()&& (gBackLightTimerStatus == 2)
#ifdef __MMI_SOUND_RECORDER__
				    &&!(Snd_getRecAmrRunState())
#endif
				  )//wangzl: add the last qualification, for close backlight after scrsav run a time
				{
					/* 1. Turn off backlight if defined(__MMI_GPIO_DISABLE_SLEEP_MODE__), but keep
					 *		half backlight if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__).
					 *		(Typically __MMI_GPIO_DISABLE_SLEEP_MODE__ means that LCM can 
					 *		 retain display even when backlight is turned off, and hence we
					 *		 can turn off backlight without making LCM sleep.)
					 *
					 * 2. __MMI_LCMSLEEP_BACK2IDLE__ is currently ignored */
					 mmi_trace(g_sw_SYSTEM,"BacklightTimerHdlr>>> will entry screen saver");
					if (gBackLightTimerStatus == 2)
					{
						EnterPartialOnScreenSaverIfOk();
					}
#if defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)	
					SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL0);
					lcd_backlight = 0;
#endif /* __MMI_GPIO_DISABLE_SLEEP_MODE__ */
					StartTimer(BACKLIGHT_TIMER,LCD_CLAM_OPEN_SCRSAV_TO_OFF_TIME,BacklightTimerHdlr);//wangzl:add after running a time, close backlight
				}
				else
				{
				mmi_trace(g_sw_SYSTEM,"BacklightTimerHdlr>>> will close backlight");
#endif /* __MMI_LCD_PARTIAL_ON__ */
			
				SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL0);	
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)	
				GpioLCDSleepIn();

				 
				#ifdef __MMI_LCMSLEEP_BACK2IDLE__
				LCMSleepGotoIdleScreen();
				#endif
#endif
				lcd_backlight = 0;	
#ifdef __MMI_LCD_PARTIAL_ON__
				}
#endif /* __MMI_LCD_PARTIAL_ON__ */
			}			
		   
			gBackLightTimerStatus = 3;
		}

		#ifdef __MMI_CLAMSHELL__			
		if (isTimerSuspend == 0 && g_charbat_context.isChargerConnected == 0 && 
			!backlight_check_call_present() && mmi_msg_need_mem_full_ind() == 0 && g_phb_cntx.processing == FALSE)
			//&& mdi_audio_get_state() != MDI_AUDIO_RECORD && mdi_audio_get_state() != MDI_AUDIO_RECORD_PAUSED )
		#else
		if (isTimerSuspend == 0 && g_charbat_context.isChargerConnected == 0 && !backlight_check_call_present() )
			 
		//    && g_phb_cntx.processing == FALSE && mdi_audio_get_state() != MDI_AUDIO_RECORD && mdi_audio_get_state() != MDI_AUDIO_RECORD_PAUSED )
		#endif				
		{
			 
			isTimerSuspend = 1;					
		}
	}
#else                              
     // For PC Simulator only

	if(gbacklight_mode == 1)
	{
		if(gBackLightTimerStatus == 0)
		{
			StopTimer(BACKLIGHT_TIMER);
			StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);
			gBackLightTimerStatus = 1;
		}
		else if(gBackLightTimerStatus == 1) 
		{
			if(lcd_backlight == 2)
			{
			//simulate lcd bklight turn off
			simulate_lcd_bklight_turn_off();
				lcd_backlight = 1;
			}
			if(sublcd_backlight == 1)
			{
			//simulate sub lcd bklight turn off
			simulate_sublcd_bklight_turn_off();
				sublcd_backlight = 0;
			}		
			StopTimer(BACKLIGHT_TIMER);
			StartTimer(BACKLIGHT_TIMER,LCD_CLAM_OPEN_HALFLITE_TO_OFF_TIME,BacklightTimerHdlr);
			gBackLightTimerStatus = 2;
		}
		else // gBackLightTimerStatus == 2
		{
			if(sublcd_backlight == 1)
			{
			//simulate sub lcd bklight turn off
			simulate_sublcd_bklight_turn_off();
				sublcd_backlight = 0;
			}
			if( lcd_backlight == 1)
			{
			simulate_lcd_bklight_turn_off();
			}
				
			gBackLightTimerStatus = 3;			
		}
	}
     // End of /* For PC Simulator only */
#endif
}

/*Wangzl Add Start  Ver: TBM_780  on 2007-7-31 16:21 */
BOOL mmi_is_lcd_backlight_on(void)
{
	return(lcd_backlight == 2 || gbacklight_mode == 0);
}
/*Wangzl Add End  Ver: TBM_780  on 2007-7-31 16:21 */

/*****************************************************************************
* FUNCTION
*  TurnOnBacklight
* DESCRIPTION
*   Turns On the backlight
*
* PARAMETERS: TimerFlag: 0 permanent backlight, 1 timer backlight
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

void TurnOnBacklight(U8 TimerFlag)
{
#ifdef MMI_ON_HARDWARE_P
	/* AT command disables backlight mechanism */
	if(gbacklight_mode == 0)	
		return;
#ifdef MMI_BK_LIGHT_DELAY_TIME
	StopMyTimer(GPIO_BACKLIGHT_DELAY_TIMER);
#endif
	/* resume timer if suspended */
	if( isTimerSuspend == 1)
	{
		 
		isTimerSuspend = 0;
	}			

	#if 0
	/* turn on sub-lcd backlight if currently off */
	if(sublcd_backlight == 0)
	{
		SetGPIOLevelReq(GPIO_DEV_LED_SUBLCD, LED_LIGHT_LEVEL5);
		sublcd_backlight = 1;
		#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)&&defined( __MMI_SUBLCD__ )		
		{
			S32 lcd_width;
			S32 lcd_height;

			gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
			gdi_lcd_get_dimension(&lcd_width, &lcd_height);
			UI_BLT_double_buffer(0,0,lcd_width -1, lcd_height -1);
			gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);		
		
		}
		#endif//!defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
	}
	#endif
		
	if(lcd_backlight == 0 && gMMIClamState == 1) // sleep mode and clam open
	{	
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__) 			


		GpioLCDSleepOut();

		if (!(GetActiveScreenId() == SCR_ID_VDOPLY_PLAYER 
			#ifdef __J2ME__
			|| GetActiveScreenId() == SCR_JAVA
			#endif
			))
		{
			update_mainlcd_dt_display();
			update_sublcd_dt_display();
			UI_BLT_double_buffer(0,0,UI_device_width -1, UI_device_height -1);
		}
		/* resume screensaver if it is playing before sleep-in */
		mmi_scrsvr_lcd_sleepout_handler();
		
#endif
		
#ifdef MMI_BK_LIGHT_DELAY_TIME
		StartTimer(GPIO_BACKLIGHT_DELAY_TIMER,MMI_BK_LIGHT_DELAY_TIME,mmi_gpio_set_main_lcd_level_5);
#else
		/*Wangzl Modify Start Ver: TBM_780  on 2007-7-31 16:16 */
		//wangzl: already set level in GpioLCDSleepOut()
		//SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);
		/*Wangzl Modify End  Ver: TBM_780  on 2007-7-31 16:16 */
#endif
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);
		lcd_backlight = 2;
	}
	else if(lcd_backlight == 1 && gMMIClamState == 1) // backlight off and clam open
	{
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);
		lcd_backlight = 2;	
	}

		
	/* turn on keypad backlight if currently off and clam open */
	GpioTurnOnKeypadLight();
		
	/* start/stop timer according to input parameter */		
	if(TimerFlag == 0)
	{
		StopTimer(BACKLIGHT_TIMER);
		gBackLightTimerStatus = 0;
	}
	else if(gBackLightTimerStatus != 0) // start a timer
	{			
		StopTimer(BACKLIGHT_TIMER);
		StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);
		gBackLightTimerStatus = 1;		
	}
#else
     // For PC Simulator only
	if(gbacklight_mode == 1)
	{
		if(sublcd_backlight == 0)
		{
			//simulate turn on sublcd back light
			simulate_sublcd_bklight_turn_on();
			sublcd_backlight = 1;
		}
		
		if(lcd_backlight == 0)
		{	
		//simulate turn on lcd bklight
		simulate_lcd_bklight_turn_on();
		lcd_backlight = 2;
		}
		else if(lcd_backlight == 1)
		{
		//simulate turn on lcd bklight
		simulate_lcd_bklight_turn_on();
		lcd_backlight = 2;
		}
	}
	if(TimerFlag == 0)
	{
		StopTimer(BACKLIGHT_TIMER);
			gBackLightTimerStatus = 0;
	}
	else
	{
		if(gBackLightTimerStatus != 0) // start a timer
		{
			StopTimer(BACKLIGHT_TIMER);
			StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);
			gBackLightTimerStatus = 1;
		}			
	}
#endif
}

/*****************************************************************************
* FUNCTION
*  CloseBacklight
* DESCRIPTION
*   Immeditae Baclight Close not going thru Half Mode
*   We do not handle __MMI_LCD_PARTIAL_ON__ because CloseBacklight() is used only in clamshell
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void CloseBacklight()
{
#ifdef MMI_ON_HARDWARE_P
	/* AT command disables backlight mechanism */
	if(gbacklight_mode == 0)
		return;

	/* stop backlight timer */
	if(gBackLightTimerStatus != 0)
	{
		StopTimer(BACKLIGHT_TIMER);
	}
	
	/* turn off main-lcd backlight */
	if(lcd_backlight != 0)
	{
		SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL0);
	#ifdef __MMI_SUB_WALLPAPER__
		/* removed , not to update sublcd, video may be playing */
		//SUBLCD_StopWallpaperAnimation();
	#endif
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
			GpioLCDSleepIn();

			 
			#ifdef __MMI_LCMSLEEP_BACK2IDLE__
			LCMSleepGotoIdleScreen();
			#endif
#endif
		lcd_backlight = 0;	
	}
		
	/* turn off keypad backlight */	
	GpioTurnOffKeypadLight();

	/* turn on sub-lcd baclight */
	if(sublcd_backlight == 0)
	{
		SetGPIOLevelReq(GPIO_DEV_LED_SUBLCD, LED_LIGHT_LEVEL5);
		sublcd_backlight = 1;
	}		

	StartTimer(BACKLIGHT_TIMER,get_lcdbacklighttimer()*1000,BacklightTimerHdlr);		
	gBackLightTimerStatus = 2;
#endif	
}
#endif


/*****************************************************************************
* FUNCTION
*	mmi_gpio_backlight_ctrl_req_hdlr
* DESCRIPTION
*	Handler of MSG_ID_MMIAPI_BACKLIGHT_CONTROL_REQ
* PARAMETERS
*	msg     IN     requested data
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_gpio_backlight_ctrl_req_hdlr(void *msg)
{
#ifdef MMI_ON_HARDWARE_P
	mmiapi_backlight_control_req_struct *req = (mmiapi_backlight_control_req_struct*)msg;
	mmiapi_backlight_control_rsp_struct *rsp = (mmiapi_backlight_control_rsp_struct*)OslConstructDataPtr(sizeof(mmiapi_backlight_control_rsp_struct));

	MYQUEUE Message;

	StopTimer(BACKLIGHT_TIMER);

	if(req->on_off) 
	{
		if(req->disable_timer)
		{
			mmi_gpio_turn_on_all_light();
		}
		else
		{
			if(gBackLightTimerStatus <2 || gbacklight_mode == 0) 
			{
				mmi_gpio_turn_on_all_light();
			} 
			else
			{
				gBackLightTimerStatus = 0;
				TurnOffBacklight();
			}
		}
	}
	else
		mmi_gpio_turn_off_all_light();

	if(gBackLightTimerStatus == 0) /* timer disabled */
		rsp->disable_timer = TRUE;
	else
		rsp->disable_timer = FALSE;

	rsp->on_off = req->on_off;

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_MED;
 	Message.oslMsgId = MSG_ID_MMIAPI_BACKLIGHT_CONTROL_RSP;
	Message.oslDataPtr = (oslParaType *)rsp;
	Message.oslPeerBuffPtr = NULL;

	OslMsgSendExtQueue(&Message);
#endif
}

/*****************************************************************************
* FUNCTION
*	mmi_gpio_led_ctrl_req_hdlr
* DESCRIPTION
*	Handler of MSG_ID_MMIAPI_LED_CONTROL_REQ
* PARAMETERS
*	msg     IN     requested data
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_gpio_led_ctrl_req_hdlr(void *msg)
{
#ifdef MMI_ON_HARDWARE_P
	mmiapi_led_control_req_struct *req = (mmiapi_led_control_req_struct*)msg;
	mmiapi_led_control_rsp_struct *rsp = (mmiapi_led_control_rsp_struct*)OslConstructDataPtr(sizeof(mmiapi_led_control_rsp_struct));

	MYQUEUE Message;

	if( req->on_off )
	{
		// disable led background play pattern
		gpioOutOfCtrl = 1;
		StopLEDPatternBackGround();

		// turn on led
		SetBackLightStatusRedOn();
		SetBackLightStatusGreenOn();
		SetBackLightStatusBlueOn();
	}
	else
	{
		// turn off led
		SetBackLightStatusRedOff();
		SetBackLightStatusGreenOff();
		SetBackLightStatusBlueOff();

		// enable led background play pattern
		gpioOutOfCtrl = 0;
		StartLEDPatternBackGround();
	}
	rsp->on_off = req->on_off;
	
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_MED;
 	Message.oslMsgId = MSG_ID_MMIAPI_LED_CONTROL_RSP;
	Message.oslDataPtr = (oslParaType *)rsp;
	Message.oslPeerBuffPtr = NULL;

	OslMsgSendExtQueue(&Message);
#endif
}

/*****************************************************************************
* FUNCTION
*  InitHWBacklightTest
* DESCRIPTION
*   Initialisation function for Back Light Test
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void InitHWBacklightTest(void)
{
#ifdef MMI_ON_HARDWARE_P
	SetProtocolEventHandler(SetBacklightMode,MSG_ID_MMI_EQ_SET_SLEEP_MODE_REQ_IND);
#endif	
}



/*****************************************************************************
* FUNCTION
*  SetBacklightModeExe
* DESCRIPTION
*   Sets BackLight Mode
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

void SetBacklightModeExe(U8 mode)
{
#ifdef MMI_ON_HARDWARE_P
	if(mode == 1)	// enable backlight mechanism.
	{
		gbacklight_mode = mode;
		TurnOffBacklight();
	}
	else					// disable backlight mechanism.
	{
		gbacklight_mode = 1;
		TurnOnBacklight(0);
		gbacklight_mode = mode;
	}
#endif
}

/*****************************************************************************
* FUNCTION
*  SetBacklightMode
* DESCRIPTION
*   Protocol Handler for MSG_ID_MMI_EQ_SET_SLEEP_MODE_REQ_IND
*
 
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void SetBacklightMode(void* info)
{
#ifdef MMI_ON_HARDWARE_P
	mmi_eq_set_sleep_mode_req_ind_struct* msg = (mmi_eq_set_sleep_mode_req_ind_struct*)info;
	SetBacklightModeExe(msg->on_off);
#endif

}







/*******************************************************/
/*                                                     */
/*    Audio volume path                                */
/*                                                     */
/*******************************************************/


/*****************************************************************************
* FUNCTION
*  EnableHandsFree
* DESCRIPTION
*   Enables Hands Free Mode
*
* PARAMETERS: mode
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

void EnableHandsFree(void)  
{
	U8 level;
	S16 error;
	mmi_trace(g_sw_CC," Func: %s ", __FUNCTION__);
	prevMode=AUD_MODE_LOUDSPK;
	SendAudioModeRequest(AUD_MODE_LOUDSPK);
	ReadValue(NVRAM_LDSPK_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
	if(level==0xFF)
	{
		level=LEVEL4;
		WriteValue(NVRAM_HDSET_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
	}
	SetSpeechVolume(level);
/*JINZH Del Start For 6249 Ver: TBM780  on 2007-8-17 16:46 */
	#if 0
	SetModeSpeechVolume();
	#endif
/*JINZH Del End  For 6249 Ver: TBM780  on 2007-8-17 16:46 */
}



/*****************************************************************************
* FUNCTION
*  DisbleHandsFree
* DESCRIPTION
*   Disables Hands Free Mode
*
* PARAMETERS: mode
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void DisbleHandsFree(void)
{
	U8 level;
	S16 error;
	if(0 == GetHandsetInPhone())
	{
	   mmi_trace(g_sw_CC,"MMI_Call: Func: %s Normal", __FUNCTION__);
	   prevMode=AUD_MODE_NORMAL;
	SendAudioModeRequest(AUD_MODE_NORMAL);
	ReadValue(NVRAM_NORMAL_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
	if(level==0xFF)
	{
		level=LEVEL4;
		WriteValue(NVRAM_NORMAL_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
	}
	SetSpeechVolume(level);

/*Jinzh Del Start For 6249,6246 Ver: TBM780  on 2007-8-17 11:29 */
		#if 0
		SetModeSpeechVolume();
		#endif
/*Jinzh Del End  For 6249,6246 Ver: TBM780  on 2007-8-17 11:29 */
	}
	else
	{
	   mmi_trace(g_sw_CC,"MMI_Call: Func: %s Headset", __FUNCTION__);
	   prevMode=AUD_MODE_HEADSET;
		SendAudioModeRequest(AUD_MODE_HEADSET);
		ReadValue(NVRAM_HDSET_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
		if(level==0xFF)
		{
			level=LEVEL4;
			WriteValue(NVRAM_HDSET_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
		}

	SetSpeechVolume(level);

/*Jinzh Del Start For 6249,6246 Ver: TBM780  on 2007-8-17 11:29 */
		#if 0
		SetModeSpeechVolume();
		#endif
/*Jinzh Del End  For 6249,6246 Ver: TBM780  on 2007-8-17 11:29 */
	}
}



/*****************************************************************************
* FUNCTION
*  SaveSpeechVolume
* DESCRIPTION
*   Saves Speech Volume
*
* PARAMETERS: level: Level
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void SaveSpeechVolume(U8 level)
{
	S16 error;
	switch(prevMode)
	{
      /*Jinzh Modify Start For 6485 Ver: TBM780  on 2007-8-29 14:31 */
      case AUD_MODE_NORMAL:
		WriteValue(NVRAM_NORMAL_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
		break;
	case AUD_MODE_LOUDSPK:
		WriteValue(NVRAM_LDSPK_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
         break;
      case AUD_MODE_HEADSET:
         WriteValue(NVRAM_HDSET_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
         break;
      //case AUD_MODE_LOUDSPK:
         //WriteValue(NVRAM_LDSPK_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
         //break;
      /*Jinzh Modify End  For 6485 Ver: TBM780  on 2007-8-29 14:31 */
	}
	SetSpeechVolume(level);
}


/*****************************************************************************
* FUNCTION
*  SetModeSpeechVolume
* DESCRIPTION
*   Sets Speech Volume as per the mode
*
* PARAMETERS: void
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

void SetModeSpeechVolume(void)
{
	U8 level;
	S16 error;
 
	switch(prevMode)
	{
      /*Jinzh Modify Start For 6485 Ver: TBM780  on 2007-8-29 14:30 */
      case AUD_MODE_NORMAL:
         ReadValue(NVRAM_NORMAL_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
    	 break;
	  case AUD_MODE_LOUDSPK:
	  ReadValue(NVRAM_LDSPK_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
         break;
      case AUD_MODE_HEADSET:
         ReadValue(NVRAM_HDSET_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
         break;
      //case AUD_MODE_LOUDSPK:
         //ReadValue(NVRAM_LDSPK_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
         //break;
      /*Jinzh Modify End  For 6485 Ver: TBM780  on 2007-8-29 14:30 */
	}
	if(level==0xFF)
	{
		level=LEVEL4;
		WriteValue(NVRAM_HDSET_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
		WriteValue(NVRAM_NORMAL_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
		level=LEVEL2;
		WriteValue(NVRAM_LDSPK_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
	}
	SetSpeechVolume(level);
}


/*****************************************************************************
* FUNCTION
*  GetModeSpeechVolume
* DESCRIPTION
*   Gets Speech Volume as per the mode
*
* PARAMETERS: void
*
* RETURNS: Speech Volume
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

U8 GetModeSpeechVolume(void)
{
	U8 level;
	S16 error;
 
	switch(prevMode)
	{
      /*Jinzh Modify Start For 6485 Ver: TBM780  on 2007-8-29 14:31 */
      case AUD_MODE_NORMAL:
         ReadValue(NVRAM_NORMAL_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
	   break;
      case AUD_MODE_LOUDSPK:
 	  ReadValue(NVRAM_LDSPK_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
         break;
      case AUD_MODE_HEADSET:
         ReadValue(NVRAM_HDSET_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
         break;
      //case AUD_MODE_LOUDSPK:
         //ReadValue(NVRAM_LDSPK_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
         //break;
      /*Jinzh Modify End  For 6485 Ver: TBM780  on 2007-8-29 14:31 */
	}
	if(level==0xFF)
      level=LEVEL4;
	return level;

}





/*******************************************************/
/*                                                     */
/*    Device control interface for other modules       */
/*                                                     */
/*******************************************************/


/*****************************************************************************
* FUNCTION
*  GpioCtrlReq
* DESCRIPTION
*   Api for other modules get gpio control
*
* PARAMETERS: action: MMIAPI_DEV_CTRL_GET / MMIAPI_DEV_CTRL_RELEASE
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

kal_uint8 GpioCtrlReq(kal_uint8 action)
{
   U8 result=MMI_TRUE;	
   if( action == MMIAPI_DEV_CTRL_GET )
   {
      if( gpioOutOfCtrl == 1 )
         result = MMI_FALSE;
      else
      {
         gpioOutOfCtrl = 1;
         StopLEDPatternBackGround();
       }
   }
   else if(action == MMIAPI_DEV_CTRL_RELEASE )
   {
      if( gpioOutOfCtrl == 0 )    
         result = MMI_FALSE;
      else
      {
         gpioOutOfCtrl = 0;
         StartLEDPatternBackGround();   
	  GpioTurnOffKeypadLight();
	  TurnOnBacklight(1);
       }
   }
   else
      result = MMI_FALSE;
   
   return result;	
}



/*****************************************************************************
* FUNCTION
*  GpioCtrlReqHdlr
* DESCRIPTION
*   Other module get device control through MMI
*
* PARAMETERS: mmiapi_device_get_ctrl_req_struct *
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void GpioCtrlReqHdlr( void *inMsg )
{
#ifdef MMI_ON_HARDWARE_P
   mmiapi_device_get_ctrl_req_struct *p = (mmiapi_device_get_ctrl_req_struct *)inMsg;
   mmiapi_device_get_ctrl_rsp_struct *p_out;
     
   p_out = OslConstructDataPtr(sizeof(mmiapi_device_get_ctrl_rsp_struct));
   p_out->result = GpioCtrlReq(p->action);

   GpioSendMsg( p->src_mod_id, MSG_ID_MMIAPI_DEVICE_GET_CTRL_RSP, (void*)p_out, (void *)NULL  );
#endif  
}



/*****************************************************************************
* FUNCTION
*  GpioPlayReq
* DESCRIPTION
*   Other module control gpio device 
*
* PARAMETERS: p_type, p_level
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
kal_uint8 GpioPlayReq(kal_uint8 p_type,kal_uint8 p_level)
{
   U8 result=MMI_TRUE;
   U8 device=0, level = 0;
   switch( p_type )
   {
      case MMIAPI_DEV_TYPE_MAINLCD_BL: device = GPIO_DEV_LED_MAINLCD; break;
      case MMIAPI_DEV_TYPE_SUBLCD_BL:  device = GPIO_DEV_LED_SUBLCD; break;
      case MMIAPI_DEV_TYPE_KEYPAD_BL:  device = GPIO_DEV_LED_KEY; break;
      case MMIAPI_DEV_TYPE_LED_1:  device = GPIO_DEV_LED_STATUS_1; break;
      case MMIAPI_DEV_TYPE_LED_2:  device = GPIO_DEV_LED_STATUS_2; break;
      case MMIAPI_DEV_TYPE_LED_3:  device = GPIO_DEV_LED_STATUS_3; break;
      case MMIAPI_DEV_TYPE_VIBRATOR:  device = GPIO_DEV_VIBRATOR; break;
      default:  result = MMI_FALSE;   break;
   }

   switch( p_level )
   {
      case MMIAPI_DEV_LEVEL_0:  level = LED_LIGHT_LEVEL0; break;
      case MMIAPI_DEV_LEVEL_1:  level = LED_LIGHT_LEVEL1; break;
      case MMIAPI_DEV_LEVEL_2:  level = LED_LIGHT_LEVEL2; break;
      case MMIAPI_DEV_LEVEL_3:  level = LED_LIGHT_LEVEL3; break;
      case MMIAPI_DEV_LEVEL_4:  level = LED_LIGHT_LEVEL4; break;
      case MMIAPI_DEV_LEVEL_5:  level = LED_LIGHT_LEVEL5; break;
      default:  result = MMI_FALSE;   break;
   }
   SetGPIOLevelReq( device, level ) ;
   
   return result;
}


/*****************************************************************************
* FUNCTION
*  GpioPlayReqHdlr
* DESCRIPTION
*   Other module play device function through MMI
*
* PARAMETERS: mmiapi_device_play_req_struct *
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void GpioPlayReqHdlr( void *inMsg )
{
#ifdef MMI_ON_HARDWARE_P
   mmiapi_device_play_req_struct *p = (mmiapi_device_play_req_struct *)inMsg;
   mmiapi_device_play_rsp_struct *p_out;
   
   p_out = OslConstructDataPtr(sizeof(mmiapi_device_play_rsp_struct));
   p_out->result = GpioPlayReq(p->type,p->level);
   GpioSendMsg( p->src_mod_id, MSG_ID_MMIAPI_DEVICE_PLAY_RSP, (void*)p_out, (void *)NULL  );
#endif
}






/*******************************************************/
/*                                                     */
/*    Send Message to Lower layer                      */
/*                                                     */
/*******************************************************/

/*****************************************************************************
* FUNCTION
*  SendPlayPatternReqToHW
* DESCRIPTION
*   Send pattern request to H/W
*
* PARAMETERS: pattern: pattern to be played
*             action: ON/OFF
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

void SendPlayPatternReqToHW(U8 pattern, U8 action, U32 dur)
{

	//tangjie del 20060708
	#if 1
	MYQUEUE Message;
   mmi_eq_play_pattern_req_struct *displayLedPattern;

	Message.oslMsgId = MSG_ID_MMI_EQ_PLAY_PATTERN_REQ;
	displayLedPattern = OslConstructDataPtr(sizeof(mmi_eq_play_pattern_req_struct));
	displayLedPattern->pattern = pattern;
	displayLedPattern->action = action;
		displayLedPattern->dur = dur;
   Message.oslDataPtr = (oslParaType *)displayLedPattern;
   Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	OslMsgSendExtQueue(&Message);
	#endif
	//tangjie del end 20060708
}




/*****************************************************************************
* FUNCTION
*  custom_cfg_gpio_set_level_onlythis
* DESCRIPTION
*   Set Levels of Devices like LCD, LEDs
*
* PARAMETERS: device: device
*				  level: Device Level
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
/*Guojian Add Start For 10810 Ver: GreenStone  on 2008-12-19 16:58 */
void custom_cfg_gpio_set_level_onlythis(gpio_device_enum device, U8 level)
{

	MYQUEUE Message;
	mmi_eq_exe_gpio_level_req_struct *setGPIOLevelReq;
	TRACE_SYS_FUNCTION();

	Message.oslMsgId = MSG_ID_MMI_EQ_EXE_GPIO_LEVEL_REQ;
	setGPIOLevelReq = OslConstructDataPtr(sizeof(mmi_eq_exe_gpio_level_req_struct));
	setGPIOLevelReq->gpio_dev_type = device;


	setGPIOLevelReq->gpio_dev_level = level;

	Message.oslDataPtr = (oslParaType *)setGPIOLevelReq;
	Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	OslMsgSendExtQueue(&Message);

  	return;
}
/*Guojian Add End  For 10810 Ver: GreenStone  on 2008-12-19 16:58 */


/*****************************************************************************
* FUNCTION
*  SetGPIOLevelReq
* DESCRIPTION
*   Set Levels of Devices like LCD, LEDs
*
* PARAMETERS: device: device
*				  level: Device Level
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void SetGPIOLevelReq(gpio_device_enum device, U8 level)
{
	//tangjie del begin 20060708

	/*modified start by wuzc at 20061121*/
	#if 1
	MYQUEUE Message;
   mmi_eq_exe_gpio_level_req_struct *setGPIOLevelReq;

	TRACE_SYS_FUNCTION();
	   
#ifdef __LCD_ESD_RECOVERY__
   if( (device == GPIO_DEV_LED_MAINLCD) || (device == GPIO_DEV_LED_SUBLCD) )
   {
    	if(level==LED_LIGHT_LEVEL0)
      	gdi_lcd_stop_esd_recovery_timer();
		else if(level==LED_LIGHT_LEVEL5)
			gdi_lcd_start_esd_recovery_timer();
	}
#endif

	Message.oslMsgId = MSG_ID_MMI_EQ_EXE_GPIO_LEVEL_REQ;
	setGPIOLevelReq = OslConstructDataPtr(sizeof(mmi_eq_exe_gpio_level_req_struct));
	setGPIOLevelReq->gpio_dev_type = device;
  if(level==LED_LIGHT_LEVEL5)
  {
    setGPIOLevelReq->gpio_dev_level = g_phnset_cntx_p->MainCntrst;
  }
  else
  {
	setGPIOLevelReq->gpio_dev_level = level;
  }
   Message.oslDataPtr = (oslParaType *)setGPIOLevelReq;
   Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	OslMsgSendExtQueue(&Message);
	#endif
   //tangjie del end 20060708
  	return;
}


/*****************************************************************************
* FUNCTION
*  SetMuteReq
* DESCRIPTION
*   Mutes the device
*
* PARAMETERS: device: device
*				  onoff: TRUE/FALSE
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/

void SetMuteReq(audio_type_enum device, MMI_BOOL onoff)
{
	MYQUEUE Message;
   mmi_eq_set_mute_req_struct *setMuteReq;
   
	Message.oslMsgId = MSG_ID_MMI_EQ_SET_MUTE_REQ;
	setMuteReq = OslConstructDataPtr(sizeof(mmi_eq_set_mute_req_struct));
	setMuteReq->audio_device = device;
	setMuteReq->mute_onoff = onoff;
   Message.oslDataPtr = (oslParaType *)setMuteReq;
   Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	OslMsgSendExtQueue(&Message);
}


/*****************************************************************************
* FUNCTION
*  SendAudioModeRequest
* DESCRIPTION
*   Changes mode of Audio Like Speaker, Headset, etc. 
*
* PARAMETERS: mode: Mode of Audio
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
extern void ToSetHandSetMode();
extern void ToSetSpeakerMode() ;
extern void ToSetBluetoothMode();
extern void ToSetEarpieceMode() ;

void SendAudioModeRequest(U8 mode)
{
#if 1  //chenhe use syn-set mode to avoid of diffirent task halt leads to crash,basic reason is key event is to adp task,and set audio mode is also in adp task
//and mmi task prority is lowest in (mmi,adp,media task,and key intrrupt is highest)
//if mmi call a play,and play is started successfully,meanwhile,the audio mode is setted correct;at this time
//a lot of key is pressed,then adp is halt by keys,after a time,*****************************************
	switch(mode)
	{
		case AUD_MODE_HEADSET:
				ToSetHandSetMode();
			break;
			
		case AUD_MODE_LOUDSPK:
				ToSetSpeakerMode();//免提模式
			break;
			
		case AUD_MODE_BLUETOOTH:
			#ifdef __MMI_BT_SUPPORT__					
				ToSetBluetoothMode();
			#endif				
			break;	
			
		case AUD_MODE_NORMAL:
		default:		
				ToSetEarpieceMode();
			break;

	}


#else
	 MYQUEUE Message;
    mmi_eq_set_audio_mode_req_struct *myMsgPtr;
    
    myMsgPtr = (mmi_eq_set_audio_mode_req_struct*)OslConstructDataPtr(sizeof(mmi_eq_set_audio_mode_req_struct));
    myMsgPtr->mode = mode;  /*AUD_MODE_LOUDSPK, AUD_MODE_NORMAL, AUD_MODE_HEADSET AUD_MODE_BLUETOOTH audio_mode_enum */
    Message.oslSrcId = MOD_MMI;
    Message.oslDestId = MOD_L4C;
    Message.oslMsgId =MSG_ID_MMI_EQ_SET_AUDIO_MODE_REQ;
    Message.oslDataPtr = (oslParaType *)myMsgPtr;
    Message.oslPeerBuffPtr= NULL;
    OslMsgSendExtQueue(&Message);
#endif
}



/*****************************************************************************
* FUNCTION
*  GpioSendMsg
* DESCRIPTION
*   Send message to other modules
*
* PARAMETERS: mmiapi_device_play_req_struct *
*
* RETURNS: U16 src_mod_id 
*	        U16 msg_id
*	        void *local_param_ptr 
*	        void *peer_buf_ptr
*
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void GpioSendMsg( U16 src_mod_id, U32 msg_id,  void *local_param_ptr, void *peer_buf_ptr)
{

#ifdef MMI_ON_HARDWARE_P
	MYQUEUE Message;
     
	Message.oslMsgId = (oslMsgType)msg_id; /* Type casting, to make compiler happer */
   Message.oslDataPtr = (oslParaType *) local_param_ptr;
   Message.oslPeerBuffPtr= peer_buf_ptr;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId = (oslModuleType) src_mod_id; /* Type casting, to make compiler happer */
	OslMsgSendExtQueue(&Message);

#endif

}


//JL add 06/25
/*****************************************************************************
* FUNCTION
*  KeyPadLCDBackLightStop
* DESCRIPTION
*   KeyPadLCDBackLightStop
*
* PARAMETERS: void
*
* RETURNS: U16 src_mod_id 
*	        U16 msg_id
*	        void *local_param_ptr 
*	        void *peer_buf_ptr
*
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
void KeyPadLCDBackLightStop(void)
{
	TRACE_SYS_FUNCTION();
	StopTimer(TIMER_KEYPAD_BACKLIGHT);
	LCDBackLightOff();
	KeyPadBackLightOff();
}



 
/*****************************************************************************
* FUNCTION
*  LCMSleepGotoIdleScreen
* DESCRIPTION
*   KeyPadLCDBackLightStop
*
* PARAMETERS: void
*
* RETURNS: void
*
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
#if defined(__MMI_LCMSLEEP_BACK2IDLE__)
void LCMSleepGotoIdleScreen(void)
{
   if( 1 == GetHandsetInPhone() && backlight_check_call_present() )
      return;

#if defined( __MMI_FACTORY_MODE__ ) || defined(__MMI_ENGINEER_MODE__)
      if (gCurrentMode == FACTORY_MODE || gCurrentMode == ENGINEERING_MODE)
         return;
#endif

   PRINT_INFORMATION( ("LCMSleepGotoIdleScreen():END Key Press, FP: %x", GetKeyHandler (KEY_END, KEY_EVENT_DOWN) ) );
   ExecuteCurrKeyHandler( KEY_END, KEY_EVENT_DOWN );
    
}
#endif

#ifdef __TFLASH_MOUNT_DYNAMIC__
/*****************************************************************************
* FUNCTION
*  TFlashDisplayPopup
* DESCRIPTION
*   Display TF plug in/out popup
*
* PARAMETERS: BOOL
*
* RETURNS: void
*  
* GLOBALS AFFECTED: none
*  
*****************************************************************************/
extern void fmgr_add_TFlash_content();
extern void fmgr_remove_TFlash_content();
extern BOOL g_hasMountedTFlashFS;

extern pBOOL isInCall(void);

#ifdef JATAAYU_SUPPORT
extern BOOL isInWapApp(void);
extern BOOL isInMMSApp(void);
#endif


void TFlashDisplayPopup( BOOL is_plugin )
{
    INT32 iResult = 0;
    U16 str_id = 0;
    U8 pp_flag, tone_id=0;
    U8 mount;
    UINT8 pMmcDevName[] = {'/',0,'T',0,0,0};
    U8 do_popup = 1;
    mmi_trace(1,"---------------------------T-Flash is plug ?= %d!\n",is_plugin);

    pp_flag = GetInterruptPopupDisplay();

    if( pp_flag & POPUP_TONE_ON )
        tone_id = AUX_TONE;

    if( (GetTransitionToActiveFlag() == MMI_TRUE) || (IdleIsPowerOnPeriod())||(!IS_IDLE_ARRIVED)
    || (g_pwr_context[0].PowerOnMode != POWER_ON_KEYPAD && g_pwr_context[0].PowerOnMode != POWER_ON_EXCEPTION)
    )
    {
        do_popup = 0;
    }


#ifdef __MMI_ENGINEER_MODE__
    if(gCurrentMode == ENGINEERING_MODE)
    {
        do_popup = 0;
    }
#endif

#ifdef __MMI_FACTORY_MODE__
    if(gCurrentMode == FACTORY_MODE)
    {
        do_popup = 0;
    }
#endif

    if( is_plugin == MMI_TRUE ) /* Plug in */
        str_id = STR_GLOBAL_TFLASH_PLUG_IN;
    else /* Plug out */
        str_id = STR_GLOBAL_TFLASH_PLUG_OUT;

    /* Should turn on backlight first to make LCD sleep out */
    TurnOnBacklight(1);


    if(is_plugin )
    {
        if(g_hasMountedTFlashFS == FALSE)
        {
            for(mount = 0;mount < 3;mount++)
            {
                iResult = FS_Mount("MMC0",pMmcDevName, 0,FS_TYPE_FAT);
                mmi_trace(1,"Mount MMC0  %d !\n",iResult);
                if(ERR_SUCCESS == iResult)
                {
                    fmgr_add_TFlash_content();
	#ifdef MMI_ON_HARDWARE_P				
                    Usb_resume_application(); 
	#endif				
                    break;
                 }     
            }
           if(ERR_SUCCESS != iResult)
            {
                str_id = STR_GLOBAL_NOT_DONE;

           }
            
        }
        if(do_popup)
            DisplayPopup( (U8 *)GetString(str_id), IMG_EARPHONE_POPUP_MAINLCD, 0, 700, tone_id);
    }
    else
    {

        if ( g_hasMountedTFlashFS )
        {
            if(do_popup)
                DisplayPopup( (U8 *)GetString(str_id), IMG_EARPHONE_POPUP_MAINLCD, 0, 700, tone_id);
	#ifdef __NATIVE_JAVA__
	    if(!gcj_IsJavaOn())
	#endif		
		{
#ifdef JATAAYU_SUPPORT
            if ((!isInCall())
			#ifdef JATAAYU_SUPPORT_WAP
				&&(!isInWapApp())
			#endif
			#ifdef JATAAYU_SUPPORT_MMS 
			&&(!isInMMSApp())
			#endif
			 )
            DeleteUptoScrID(IDLE_SCREEN_ID);
#else
            if (!isInCall())
            DeleteUptoScrID(IDLE_SCREEN_ID);
#endif
	    }
		
            Usb_stop_application();
#ifdef MMI_ON_HARDWARE_P
            iResult = FS_Unmount(pMmcDevName, TRUE);
#else
		iResult = ERR_SUCCESS;
#endif
            fmgr_remove_TFlash_content();
            if(ERR_SUCCESS == iResult)
            {
                mmi_trace(1,"UnMount MMC0 OK!\n");

            }


        }
    }
    mmi_trace(1,"operation MMC0 result ?= %d!\n",iResult);

}
#endif

/*******************************************************/
 
/* AT command will query MMI when needed               */
/*******************************************************/
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


