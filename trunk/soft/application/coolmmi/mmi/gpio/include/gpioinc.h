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
 * gpioInc.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Header file for GPIO Utility Functions
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: gpioInc.h

  	PURPOSE		: Header file for GPIO Utility Functions

 

 

	DATE		: Aug' 1, 2003

**************************************************************/

#ifndef MMI_GPIO_H
#define MMI_GPIO_H

#ifdef MMI_ON_WIN32
typedef unsigned char   kal_uint8;
typedef unsigned short int      kal_uint16;
typedef unsigned int            kal_uint32;
#endif
#include "kal_non_specific_general_types.h"
#include "device.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "custom_led_patterns.h"
#include "settingprofile.h"

extern U8 gMMIClamState;
extern U8 isEarphonePlugged;

/*Jinzh Modify Start For 6424 Ver: TBM780  on 2007-8-28 17:37 */
extern U8 isEarphonePlugged_1;  
/*Jinzh Modify End  For 6424 Ver: TBM780  on 2007-8-28 17:37 */
/*************************************************/
 
/*** Device Manager ***/
extern U32 gDeviceState;

#define MMIDEVMSK_VIBON     0x00000001
#define MMIDEVMSK_VIBSMS	     0x00000002
#define MMIDEVMSK_RESERVED_2      0x00000004
#define MMIDEVMSK_RESERVED_3	     0x00000008
#define MMIDEVMSK_RESERVED_4      0x00000010
#define MMIDEVMSK_RESERVED_5      0x00000020
#define MMIDEVMSK_RESERVED_6      0x00000040
#define MMIDEVMSK_RESERVED_7      0x00000080

#define MMIDEV_GET_FLAG(a)       ( (gDeviceState & a) == 0 ? MMI_FALSE : MMI_TRUE )
#define MMIDEV_SET_FLAG(a)       ( gDeviceState |=  a )
#define MMIDEV_RESET_FLAG(a)   ( gDeviceState &=  ~a )
#define MMIDEV_QRY_OCCUPY()       ( (gDeviceState == 0) ? MMI_FALSE : MMI_TRUE )

/*************************************************/

//------- Data Types Decleration -----------------------------
extern U8 prevMode;
/* gpio devices detection indication */
void GpioDetectInd(void * info);


/* Set the audio to the silent mode. Any sound will be useless. */
//void SetSilentModeReq(MMI_BOOL onoff);

/* sets the Mute on */
void SetMuteReq(audio_type_enum audio, MMI_BOOL onoff);

void SetGPIOLevelReq(gpio_device_enum device, U8 level);
void PlayPatternReq(U8 pattern, U8 action);
void PlayPatternReq_Ext(U8 pattern, U8 action, U32 dur);
void PlayPatternCallerGroup(MMI_LEDLIGHT_STATUS_ENUM pattern);
void StopPatternCallerGroup(MMI_LEDLIGHT_STATUS_ENUM pattern);
void SetModeSpeechVolume(void);
void SaveSpeechVolume(U8 level);
U8 GetModeSpeechVolume(void);
void EnableHandsFree(void);
void DisbleHandsFree(void);

void SendAudioModeRequest(U8 mode);
void EarPhoneScrExitOut(void);
void EarPhonePlugOutScrTimeout(void);
void EarPhonePlugInScrTimeout(void);
void EarphoneDisplayPopup( BOOL is_plugin );
void EarphoneConnectHdlr( BOOL is_in );
void GpioEarphonePlugIn(void);
void GpioEarphonePlugOut(void);
BOOL mmi_gpio_is_earphone_plug_in( void );

void ShowEarphoneSubLCDPopup(void);
void EarPhonePlugInScrExit(void);


void GetRingToneRange(U8 *startId, U8*totalId);
void GetMidiRange(U8 *startId, U8*totalId);
void GetToneRange(U8 *startId, U8*totalId);
void GetMiscToneRange(U8 *startId, U8*totalId);
void SendPlayPatternReqToHW(U8 pattern, U8 action, U32 dur);
void custom_cfg_gpio_set_level_onlythis(gpio_device_enum device, U8 level);


#if (defined __MMI_CLAMCLOSE_BACK2IDLE__ || defined __MMI_SUBLCD_SHOW_ANALOG_CLOCK__)
void ClamCloseGotoIdleScreen(void);
#endif

#define SetRingerVolume(volume) mdi_audio_set_volume(MDI_VOLUME_CTN,(U8)(volume))
//#define SetKeyPadVolume(volume) mdi_audio_set_volume(MDI_VOLUME_KEY,(U8)(volume))
#define SetKeyPadVolume(volume) (GetHandsetInPhone()==1? mdi_audio_set_volume(MDI_VOLUME_KEY,(U8)(volume)) : DM_SetAudioVolume((volume) ))
#define SetSpeechVolume(volume) mdi_audio_set_volume(MDI_VOLUME_SPH,(U8)(volume))
//#define SetSpeechVolume(volume) (((GetHandsetInPhone()==1)||(AUD_MODE_LOUDSPK == GetSpeakerWorkStatus()))? mdi_audio_set_volume(MDI_VOLUME_SPH,(U8)(volume)) : DM_SetAudioVolume((volume) + 1)) 


/*******************************************************/
 
/* AT command will query MMI when needed               */
/*******************************************************/
//#define SilentModeOn() SetSilentModeReq(1)
//#define SilentModeOff() SetSilentModeReq(0)

#define MuteOnMicrophone() SetMuteReq(MDI_DEVICE_MICROPHONE,1)
#define MuteOffMicrophone() SetMuteReq(MDI_DEVICE_MICROPHONE,0)

#define MuteOnSpeaker() SetMuteReq(MDI_DEVICE_SPEAKER,1)
#define MuteOffSpeaker() SetMuteReq(MDI_DEVICE_SPEAKER,0)


/*
#define LCDBackLightOn() SetGPIOLevelReq(GPIO_DEV_LED_LCD,LED_LCD_ON)
#define LCDBackLightOff() SetGPIOLevelReq(GPIO_DEV_LED_LCD,LED_LCD_OFF)

#define KeyPadBackLightOn() SetGPIOLevelReq(GPIO_DEV_LED_KEY,1)
#define KeyPadBackLightOff() SetGPIOLevelReq(GPIO_DEV_LED_KEY,0)

#define SetBackLightStatusOff()			SetGPIOLevelReq(GPIO_DEV_LED_STATUS,LED_LIGHT_ALL_OFF)
#define SetBackLightStatusRedOn()		SetGPIOLevelReq(GPIO_DEV_LED_STATUS,LED_LIGHT_RED_ON)
#define SetBackLightStatusGreenOn()		SetGPIOLevelReq(GPIO_DEV_LED_STATUS,LED_LIGHT_GREEN_ON)
#define SetBackLightStatusBlueOn()		SetGPIOLevelReq(GPIO_DEV_LED_STATUS,LED_LIGHT_BLUE_ON)
#define SetBackLightStatusRedOff()		SetGPIOLevelReq(GPIO_DEV_LED_STATUS,LED_LIGHT_RED_OFF)
#define SetBackLightStatusGreenOff()		SetGPIOLevelReq(GPIO_DEV_LED_STATUS,LED_LIGHT_GREEN_OFF)
#define SetBackLightStatusBlueOff()		SetGPIOLevelReq(GPIO_DEV_LED_STATUS,LED_LIGHT_BLUE_OFF)
#define SetBackLightStatus(color)			SetGPIOLevelReq(GPIO_DEV_LED_STATUS,color)

*/
#define LCDBackLightOn() SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD,5);\
						 SetGPIOLevelReq(GPIO_DEV_LED_SUBLCD,5); 	
						 
#define LCDBackLightOff() SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD,0);\
						  SetGPIOLevelReq(GPIO_DEV_LED_SUBLCD,0);
	
#define KeyPadBackLightOn() SetGPIOLevelReq(GPIO_DEV_LED_KEY,5)
#define KeyPadBackLightOff() SetGPIOLevelReq(GPIO_DEV_LED_KEY,0)

#define SetBackLightStatusOff()			SetGPIOLevelReq((gpio_device_enum)GPIO_DEV_LED_STATUS_1,LED_LIGHT_LEVEL0);\
										SetGPIOLevelReq((gpio_device_enum)GPIO_DEV_LED_STATUS_2,LED_LIGHT_LEVEL0);\
										SetGPIOLevelReq((gpio_device_enum)GPIO_DEV_LED_STATUS_3,LED_LIGHT_LEVEL0);

#define SetBackLightStatusRedOn()		SetGPIOLevelReq((gpio_device_enum)GPIO_DEV_LED_STATUS_1,LED_LIGHT_LEVEL5)
#define SetBackLightStatusGreenOn()		SetGPIOLevelReq((gpio_device_enum)GPIO_DEV_LED_STATUS_2,LED_LIGHT_LEVEL5)
#define SetBackLightStatusBlueOn()		SetGPIOLevelReq((gpio_device_enum)GPIO_DEV_LED_STATUS_3,LED_LIGHT_LEVEL5)

#define SetBackLightStatusRedOff()		SetGPIOLevelReq((gpio_device_enum)GPIO_DEV_LED_STATUS_1,LED_LIGHT_LEVEL0)
#define SetBackLightStatusGreenOff()	SetGPIOLevelReq((gpio_device_enum)GPIO_DEV_LED_STATUS_2,LED_LIGHT_LEVEL0)
#define SetBackLightStatusBlueOff()		SetGPIOLevelReq((gpio_device_enum)GPIO_DEV_LED_STATUS_3,LED_LIGHT_LEVEL0)

#if defined (__LED__) || (defined (__MMI_RUNNING_LED__) && !defined (MMI_ON_WIN32))

#define StartLEDPatternPowerOn()				showPowerOnLED()  
#define StopLEDPatternPowerOn()					StopPowerOnOffLED()

#define StartLEDPatternPowerOff()				showPowerOffLED()
#define StopLEDPatternPowerOff()				StopPowerOnOffLED()
#define StartLEDPatternMusicRunning()           showMusicRunningLED()   
#define StopLEDPatternMusicRunning()           stopMusicRunningLED()
#else
#define StartLEDPatternPowerOn()				//PlayPatternReq(LED_POWER_ON, 1)   // zrx del 20060612
#define StopLEDPatternPowerOn()					//PlayPatternReq(LED_POWER_ON, 0)  // zrx del 20060612

/*modify bu wuzc at 20061119*/
#define StartLEDPatternPowerOff()				//PlayPatternReq(LED_POWER_OFF, 1)  delete by jinzh:20070716
#define StopLEDPatternPowerOff()				//PlayPatternReq(LED_POWER_OFF, 0)  delete by jinzh:20070716
/*modify bu wuzc at 20061119*/
#define StartLEDPatternMusicRunning()             
#define StopLEDPatternMusicRunning()           

#endif
#define StartLEDPatternBatteryWarning()			//PlayPatternReq(LED_BATTERY_WARNING, 1) // zrx del 20060612
#define StopLEDPatternBatteryWarning()			//PlayPatternReq(LED_BATTERY_WARNING, 0) // zrx del 20060612

#define StartLEDPatternBatteryLow()				//PlayPatternReq(LED_BATTERY_LOW, 1) // zrx del 20060612
#define StopLEDPatternBatteryLow()				//PlayPatternReq(LED_BATTERY_LOW, 0) // zrx del 20060612

#define StartLEDPatternConnect900()				//PlayPatternReq(LED_Connect900, 1) // zrx del 20060612
#define StopLEDPatternConnect900()				//PlayPatternReq(LED_Connect900, 0) // zrx del 20060612

#define StartLEDPatternConnect1800()			//PlayPatternReq(LED_Connect1800, 1) // zrx del 20060612
#define StopLEDPatternConnect1800()				//PlayPatternReq(LED_Connect1800, 0) // zrx del 20060612

 
#define StartLEDPatternConnect1900()			//PlayPatternReq(LED_Connect1900, 1) // zrx del 20060612
#define StopLEDPatternConnect1900()			//	PlayPatternReq(LED_Connect1900, 0) // zrx del 20060612

#define StartLEDPatternRoaming()			   // PlayPatternReq(LED_Roaming, 1) // zrx del 20060612
#define StopLEDPatternRoaming()				//PlayPatternReq(LED_Roaming, 0) // zrx del 20060612

#define StartLEDPatternLimitService()			//PlayPatternReq(LED_LimitService, 1) // zrx del 20060612
#define StopLEDPatternLimitService()			//PlayPatternReq(LED_LimitService, 0) // zrx del 20060612

#define StartLEDPatternNoService()			 //   PlayPatternReq(LED_NoService, 1) // zrx del 20060612
#define StopLEDPatternNoService()			//	PlayPatternReq(LED_NoService, 0) // zrx del 20060612

#define StartLEDPatternBackGround()           // PlayPatternReq(GetBackgroundPattern(), 1) // zrx del 20060612
#define StopLEDPatternBackGround()          //   SendPlayPatternReqToHW(GetBackgroundPattern(), 0) // zrx del 20060612
//CSD End
/*modified start by wuzc at 20061121*/
#define StartLEDPatternCharging()				//PlayPatternReq(LED_OnCharging, 1) delete by jinzh:20070716
#define StopLEDPatternCharging()				//PlayPatternReq(LED_OnCharging, 0) delete by jinzh:20070716
/*modified start by wuzc at 20061121*/

#define StartLEDPatternFullCharging()			//PlayPatternReq(LED_FullCharging, 1) // zrx del 20060612
#define StopLEDPatternFullCharging()			//PlayPatternReq(LED_FullCharging, 0) // zrx del 20060612

#define StartLEDPatternMessageIn()				//PlayPatternReq(LED_MessageIn, 1) // zrx del 20060612
#define StopLEDPatternMessageIn()				//PlayPatternReq(LED_MessageIn, 0) // zrx del 20060612

#define StartLEDPatternMessageRemind()			//PlayPatternReq(LED_MessageRemind, 1) // zrx del 20060612
#define StopLEDPatternMessageRemind()			//PlayPatternReq(LED_MessageRemind, 0) // zrx del 20060612

#define StartLEDPatternCaller1()				//PlayPatternReq(LED_Caller1, 1) // zrx del 20060612
#define StopLEDPatternCaller1()				//	PlayPatternReq(LED_Caller1, 0) // zrx del 20060612

#define StartLEDPatternCaller2()				//PlayPatternReq(LED_Caller2, 1) // zrx del 20060612
#define StopLEDPatternCaller2()				//	PlayPatternReq(LED_Caller2, 0) // zrx del 20060612

#define StartLEDPatternCaller3()				//PlayPatternReq(LED_Caller3, 1) // zrx del 20060612
#define StopLEDPatternCaller3()				//	PlayPatternReq(LED_Caller3, 0) // zrx del 20060612

#define StartLEDPatternCaller4()				//PlayPatternReq(LED_Caller4, 1) // zrx del 20060612
#define StopLEDPatternCaller4()					//PlayPatternReq(LED_Caller4, 0) // zrx del 20060612

#define StartLEDPatternCaller5()				//PlayPatternReq(LED_Caller5, 1) // zrx del 20060612
#define StopLEDPatternCaller5()				//	PlayPatternReq(LED_Caller5, 0) // zrx del 20060612

#define StartLEDPatternCaller6()				//PlayPatternReq(LED_Caller6, 1) // zrx del 20060612
#define StopLEDPatternCaller6()				//	PlayPatternReq(LED_Caller6, 0) // zrx del 20060612

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */

/*************************************************/
 
/*** Device Manager ***/

#define VibratorOn()\
{\
	if( MMIDEV_QRY_OCCUPY() == MMI_FALSE )\
	{\
		PlayPatternReq(VIB_PATTERN_1,PLAY_REPEAT);\
	}\
	MMIDEV_SET_FLAG(MMIDEVMSK_VIBON);\
}
#define VibratorOff()\
{\
	MMIDEV_RESET_FLAG(MMIDEVMSK_VIBON);\
	if( MMIDEV_QRY_OCCUPY() == MMI_FALSE )\
	{\
		PlayPatternReq(VIB_PATTERN_1,PLAY_STOP);\
	}\
}

#define PlayVibratorOnce()\
{\
	if( MMIDEV_QRY_OCCUPY() == MMI_FALSE )\
	{\
		PlayPatternReq(VIB_PATTERN_1,PLAY_ONCE);\
	}\
}

#define PlaySmsVibratorOnce()\
{\
	if( MMIDEV_QRY_OCCUPY() == MMI_FALSE )\
	{\
		PlayPatternReq(VIB_PATTERN_3,PLAY_ONCE);\
	}\
}

#define PlayVibratorDur( n )\
{\
	if( MMIDEV_QRY_OCCUPY() == MMI_FALSE )\
	{\
		PlayPatternReq_Ext(VIB_PATTERN_3,PLAY_DUR, n);\
	}\
}


/*************************************************/



#define IsHeadSetMode() (prevMode==AUD_MODE_HEADSET) ? 1 : 0;
#define IsClamOpen()    gMMIClamState


U8 GetBackgroundPattern(void);
BOOL IsClamToneAllow(void);


 
extern MMI_BOOL IsBacklightOn(void);
extern void TurnOnBacklight(U8 TimerFlag);
extern void TurnOffBacklight(void);
extern void CloseBacklight(void);
extern void mmi_gpio_backlight_ctrl_req_hdlr(void *msg);
// CSD end
extern void mmi_gpio_led_ctrl_req_hdlr(void *msg);


 
extern void InitHWBacklightTest(void);
extern void SetBacklightMode(void* info);
extern void SetBacklightModeExe(U8 mode);
// CSD end

extern void GpioCtrlReqHdlr( void *inMsg );
extern void GpioPlayReqHdlr( void *inMsg );

extern void KeyPadLCDBackLightStop(void);
extern BOOL mmi_is_lcd_backlight_on(void);

#endif

