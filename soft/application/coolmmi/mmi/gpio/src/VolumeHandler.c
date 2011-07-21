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
 * VolumeHandler.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Volume Key Handlers
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: VolumeHandler.c

  	PURPOSE		: Volume Key Handlers

 

 

	DATE		: July 22,03

**************************************************************/
#include "mmi_data_types.h"
#include "volumehandler.h"
#include "globalconstants.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "profilesenum.h"

#include "timerevents.h"
#include "callmanagementstruct.h"
#include "callmanagementgprot.h"
#include "callmanagementiddef.h"
#include "gpioinc.h"
#include "keybrd.h"
#include "idleappdef.h"
#include "adp_events.h"
#include "mainmenudef.h"

#ifdef __MMI_AUDIO_PLAYER__
BOOL mmi_audply_is_playing( void );
void mmi_audply_press_inc_volume( void );
void mmi_audply_press_dec_volume( void );
#endif // __MMI_AUDIO_PLAYER__

#ifdef __MMI_FM_RADIO__
BOOL mmi_fmrdo_is_power_on( void );
void mmi_fmrdo_inc_volume( void );
void mmi_fmrdo_dec_volume( void );
#endif // __MMI_FM_RADIO__ 

extern S32 volume_level_UI;
FuncPtr volumeDecreaseFunction;
FuncPtr volumeIncreaseFunction;
FuncPtr volumeMinFunction;
FuncPtr volumeMaxFunction;
FuncPtr hardwareVolumeFunction;
U16 volumeCaption;
S32 *initialVolume;
U16	LSKCaption;
U16	RSKCaption;
FuncPtr	LSKCallback;
FuncPtr	RSKCallback;
FuncPtr	ExitCallback;
U32 volumeTimer;
U8 updownFlag;
extern U8 gKeyPadLockFlag;
S32 volLevel;

extern void ChangeKeyVolumeFromIdle(void);	//added by jinzh:20070721
extern BOOL isSetKeypadVolume;		    //added by jinzh:20070721
 
#if defined(__MMI_TOUCH_SCREEN__) //&& defined(__MMI_WALLPAPER_ON_BOTTOM__)

#define LONGPRESS_DELAY 6

//variable for synchronizing the time of KEY_LONG_PRESS event with MMI_PEN_EVENT_LONG_TAP event
U16 callback_timer = 0;

#endif
 

/****************************************************************************
	FUNCTION NAME		: SetDefaultVolumeKeyHandlers

  	PURPOSE				: Sets Default Volume Handlers
						
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/

void SetDefaultVolumeKeyHandlers(void)
{

	U8 isValid=0;
	if(GetCurrentState()==CM_INCOMING_STATE)
	{
		volLevel=GetRingVolumeLevel();
		SetVolumeKeyHandlerFunctions(NULL,NULL, 
									 NULL,NULL, 	
									  STR_COMMON_VOLUME_LEVEL_RING_CAPTION, &volLevel,
									  0, 0,
									  NULL,NULL, 1000, RingVolumeExitFunction,1,VOLUMEHANDLER_RING_TONE);  //guojian modify 0 -> 1 for bug 10624
		isValid=1;
	}
	else if(GetCurrentState()!=CM_IDLE_STATE && GetActiveScreenId()!=SCR_CM_REQUESTINGUSSD) //don't change volume in SS requested screen
	{
		volLevel=GetModeSpeechVolume();
		SetVolumeKeyHandlerFunctions(NULL,NULL, 
									 NULL,NULL, 	
                            //CSD added by JL 031125 for string ID not found
									  STR_VOLUME_LEVEL_CAPTION/*JL comment STR_DEFAULT_VOLUME_CAPTION*/, &volLevel,
									  0, 0,
									  NULL,NULL, 1000, SpeechVolumeExitFunction,1,VOLUMEHANDLER_SPEECH_TONE);
		isValid=1;
	}
	if((g_idle_context.IsOnIdleScreen && !gKeyPadLockFlag) )// wangzl: 090625 disable this for bug 12785 || g_idle_context.IsOnDialerScreen)
	{
		volLevel=GetKeypadVolumeLevel();
		SetVolumeKeyHandlerFunctions(ChangeKeyVolumeFromIdle, ChangeKeyVolumeFromIdle,  //changed by jinzh:20070721
									 NULL, NULL,	
                            //CSD added by JL 031125 for string ID not found
									 STR_COMMON_VOLUME_LEVEL_KEY_CAPTION/*JL comment  STR_KEY_TONE_VOLUME_CAPTION*/,&volLevel,
									 0, 0,
									 NULL,NULL, 2000,KeyPadVolumeExitFunction, 0,VOLUMEHANDLER_KEY_TONE);
		isValid=1;
	}

	if(isValid)
	{
	
		if(updownFlag)
		{
			SetKeyHandler(SetKeyPadVolUp,KEY_UP_ARROW,KEY_EVENT_DOWN);
			SetKeyHandler(SetKeyPadVolMaxLongPress,KEY_UP_ARROW,KEY_LONG_PRESS);
			SetKeyHandler(SetKeyPadVolDown,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
			SetKeyHandler(SetKeyPadVolMinLongPress,KEY_DOWN_ARROW,KEY_LONG_PRESS);
		}
		
		SetKeyHandler(SetKeyPadVolUp,KEY_VOL_UP,KEY_EVENT_DOWN);
		SetKeyHandler(SetKeyPadVolMaxLongPress,KEY_VOL_UP,KEY_LONG_PRESS);
		SetKeyHandler(SetKeyPadVolDown,KEY_VOL_DOWN,KEY_EVENT_DOWN);
		SetKeyHandler(SetKeyPadVolMinLongPress,KEY_VOL_DOWN,KEY_LONG_PRESS);
	}

}


/****************************************************************************
	FUNCTION NAME		: SetVolumeKeyHandlerFunctions

  	PURPOSE				: Init Function to be called before using Volume Screen
						
	INPUT PARAMETERS	: decreaseFunction: Function to be called when volume down key is pressed.
							increaseFunction: Function to be called when volume up key is pressed.
							minFunction: Function to be called when volume down key is long pressed.
							maxFunction: Function to be called when volume up key is long pressed.
							caption : String Id of caption of screen
							volume: Initial volume level to be displayed in screen
							captionLSK: String Id of LSK
							captionRSK: String Id of RSK
							callbackLSK: Function called when LSK is pressed
							callbackRSK: Function called when RSK is pressed
							timerDuration: Timer duration if the screen is supposed to be a popup screen
							callbackExit: Function to be called when screen is Exited.
							Updown: Flag whether the functionality is needed on up down key also.
							type: Type of volume to be adjusted like Keypad, Ring tone, Speech, etc.


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/

void SetVolumeKeyHandlerFunctions(FuncPtr decreaseFunction,
							FuncPtr increaseFunction,
							FuncPtr minFunction,
							FuncPtr maxFunction,
							U16 caption, S32 *volume,
							U16 captionLSK, U16 captionRSK,
							FuncPtr callbackLSK, FuncPtr callbackRSK,
							U32 timerDuration, FuncPtr callbackExit, U8 updown,VOLUMEHANDLER_TYPE type)
{
	volumeDecreaseFunction=decreaseFunction;
	volumeIncreaseFunction=increaseFunction;
	volumeMinFunction=minFunction;
	volumeMaxFunction=maxFunction;
	volumeCaption=caption;
	initialVolume=volume;
	LSKCaption=captionLSK;
	RSKCaption=captionRSK;
	LSKCallback=callbackLSK;
	RSKCallback=callbackRSK;
	volumeTimer=timerDuration;
	ExitCallback=callbackExit;
	updownFlag=updown;
	if(type==VOLUMEHANDLER_RING_TONE)	hardwareVolumeFunction=RingToneVolumeFunction;
	else if (type==VOLUMEHANDLER_SPEECH_TONE) hardwareVolumeFunction=SpeechVolumeFunction;
	else hardwareVolumeFunction=KeyPadVolumeFunction;
}

/**************************************************************

	FUNCTION NAME		: EntryScrSetKeyPadVolumeLevel

  	PURPOSE				:  To Increase Volume level of keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntryScrSetKeyPadVolumeLevel(void)
{
	U8 *guiBuffer;
	guiBuffer=GetCurrGuiBuffer(SCR_KEY_TONE_VOLUME);
	
	EntryNewScreen(SCR_KEY_TONE_VOLUME, StopKeyPadVolumeLevelTimer, NULL, NULL);

    ShowCategory105Screen(volumeCaption,IMG_PROFILES_TITLEE,
						 LSKCaption, 0,
						 RSKCaption, 0,(S32*)initialVolume,guiBuffer);

	if(volumeTimer) SetGroupKeyHandler(ExitScrSetKeyPadVolumeLevel,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,(U16)KEY_EVENT_DOWN);
	
	SetLeftSoftkeyFunction(LSKCallback,KEY_EVENT_UP);	
	SetRightSoftkeyFunction(RSKCallback,KEY_EVENT_UP);	
	
	#if ( defined(__NO_SIDE_VOLUME_KEY_46__)||defined(__NO_SIDE_VOLUME_KEY_46__)||defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__) )
	SetKeyHandler(ShowVerticalBarUp,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(ShowVerticalBarMax,KEY_RIGHT_ARROW,KEY_LONG_PRESS);
	SetKeyHandler(ShowVerticalBarDown,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(ShowVerticalBarMin,KEY_LEFT_ARROW,KEY_LONG_PRESS);
	#else
	SetKeyHandler(ShowVerticalBarUp,KEY_VOL_UP,KEY_EVENT_DOWN);
	SetKeyHandler(ShowVerticalBarMax,KEY_VOL_UP,KEY_LONG_PRESS);
	SetKeyHandler(ShowVerticalBarDown,KEY_VOL_DOWN,KEY_EVENT_DOWN);
	SetKeyHandler(ShowVerticalBarMin,KEY_VOL_DOWN,KEY_LONG_PRESS);
	#endif

	if(updownFlag)
	{
	#ifdef __MMI_MAINLCD_220X176__
		SetKeyHandler(ShowVerticalBarUp,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(ShowVerticalBarMax,KEY_RIGHT_ARROW,KEY_LONG_PRESS);
		SetKeyHandler(ShowVerticalBarDown,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(ShowVerticalBarMin,KEY_LEFT_ARROW,KEY_LONG_PRESS);
	#else
		SetKeyHandler(ShowVerticalBarUp,KEY_UP_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(ShowVerticalBarMax,KEY_UP_ARROW,KEY_LONG_PRESS);
		SetKeyHandler(ShowVerticalBarDown,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(ShowVerticalBarMin,KEY_DOWN_ARROW,KEY_LONG_PRESS);
	#endif		
	}

	if(volumeTimer) StartTimer(KEY_VOL_TIMER_ID, KEY_VOLUME_TIMEOUT,ExitScrSetKeyPadVolumeLevel);

	//SetExitHandler(SCR_KEY_TONE_VOLUME,StopKeyPadVolumeLevelTimer);

}

/**************************************************************

	FUNCTION NAME		: SetKeyPadVolDown

  	PURPOSE				:  To Increase Volume level of keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetKeyPadVolDown(void)
{
	mmi_trace(g_sw_CC," Func: %s ", __FUNCTION__);
	isSetKeypadVolume=TRUE;    //Added by jinzh:20070721
#ifdef __MMI_AUDIO_PLAYER__
	if( mmi_audply_is_playing() )
		mmi_audply_press_dec_volume();
	else
#endif //  __MMI_AUDIO_PLAYER__
#ifdef __MMI_FM_RADIO__
	if( mmi_fmrdo_is_power_on() )
		mmi_fmrdo_dec_volume();
	else
#endif // __MMI_FM_RADIO__
	{
		EntryScrSetKeyPadVolumeLevel();
		ShowVerticalBarDown();
	}
}

/**************************************************************

	FUNCTION NAME		: SetKeyPadVolUp

  	PURPOSE				:  To Increase Volume level of keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetKeyPadVolUp(void)
{
	mmi_trace(g_sw_CC,"Func: %s ", __FUNCTION__);
	isSetKeypadVolume=TRUE;     //Added by jinzh:20070721
#ifdef __MMI_AUDIO_PLAYER__
	if( mmi_audply_is_playing() )
		mmi_audply_press_inc_volume();
	else
#endif //  __MMI_AUDIO_PLAYER__
#ifdef __MMI_FM_RADIO__
	if( mmi_fmrdo_is_power_on() )
		mmi_fmrdo_inc_volume();
	else
#endif // __MMI_FM_RADIO__
	{
		EntryScrSetKeyPadVolumeLevel();
		ShowVerticalBarUp();
	}
}

/**************************************************************

	FUNCTION NAME		: EntryScrSetKeyPadVolumeLevel

  	PURPOSE				:  To Increase Volume level of keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetKeyPadVolMinLongPress(void)
{
	EntryScrSetKeyPadVolumeLevel();
	ShowVerticalBarMin();
}

/**************************************************************

	FUNCTION NAME		: EntryScrSetKeyPadVolumeLevel

  	PURPOSE				:  To Increase Volume level of keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetKeyPadVolMaxLongPress(void)
{
	EntryScrSetKeyPadVolumeLevel();
	ShowVerticalBarMax();
}
/**************************************************************

	FUNCTION NAME		: ShowVerticalBarUp

  	PURPOSE				:  To Increase Volume level of keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowVerticalBarUp(void)
{
	if(volumeTimer) StopTimer(KEY_VOL_TIMER_ID);
	show_vertical_bar_next();
	*initialVolume=volume_level_UI;
	hardwareVolumeFunction();
	if(volumeIncreaseFunction )volumeIncreaseFunction();
	if(volumeTimer) StartTimer(KEY_VOL_TIMER_ID, KEY_VOLUME_TIMEOUT,ExitScrSetKeyPadVolumeLevel);
}
/**************************************************************

	FUNCTION NAME		: ShowVerticalBarDown

  	PURPOSE				:  To Increase Volume level of keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowVerticalBarDown(void)
{
	if(volumeTimer) StopTimer(KEY_VOL_TIMER_ID);
	show_vertical_bar_previous();
	*initialVolume=volume_level_UI;
	hardwareVolumeFunction();
	if(volumeDecreaseFunction )volumeDecreaseFunction();
	if(volumeTimer) StartTimer(KEY_VOL_TIMER_ID, KEY_VOLUME_TIMEOUT,ExitScrSetKeyPadVolumeLevel);
}
/**************************************************************

	FUNCTION NAME		: ShowVerticalBarMax

  	PURPOSE				:  To Increase Volume level of keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowVerticalBarMax(void)
{
	if(volumeTimer) StopTimer(KEY_VOL_TIMER_ID);
	show_vertical_bar(LEVEL7);
	*initialVolume=volume_level_UI;
	hardwareVolumeFunction();
	if(volumeMaxFunction )volumeMaxFunction();
	if(volumeTimer) StartTimer(KEY_VOL_TIMER_ID, KEY_VOLUME_TIMEOUT,ExitScrSetKeyPadVolumeLevel);
}
/**************************************************************

	FUNCTION NAME		: ShowVerticalBarMin

  	PURPOSE				:  To Increase Volume level of keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowVerticalBarMin(void)
{
	if(volumeTimer) StopTimer(KEY_VOL_TIMER_ID);
	show_vertical_bar(LEVEL1);
	*initialVolume=volume_level_UI;
	hardwareVolumeFunction();
	if(volumeMinFunction )volumeMinFunction();
	if(volumeTimer) StartTimer(KEY_VOL_TIMER_ID, KEY_VOLUME_TIMEOUT,ExitScrSetKeyPadVolumeLevel);
}
/**************************************************************

	FUNCTION NAME		: StopKeyPadVolumeLevelTimer

  	PURPOSE				:  To Increase Volume level of keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void StopKeyPadVolumeLevelTimer(void)
{
	if(volumeTimer) StopTimer(KEY_VOL_TIMER_ID);
	if(ExitCallback) ExitCallback();
}

/**************************************************************

	FUNCTION NAME		: ExitScrSetKeyPadVolumeLevel

  	PURPOSE				:  To Increase Volume level of keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ExitScrSetKeyPadVolumeLevel(void)
{
	if(volumeTimer) StopTimer(KEY_VOL_TIMER_ID);
	GoBackHistory();
}

/****************************************************************************
	FUNCTION NAME		: SetVolumeKeyHandlerFunctions

  	PURPOSE				: Sets Default Volume Handlers
						
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void SpeechVolumeFunction(void)
{
	SetSpeechVolume(volume_level_UI);
}

/****************************************************************************
	FUNCTION NAME		: RingToneVolumeFunction

  	PURPOSE				: Ring Tone Volume Function
						
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void RingToneVolumeFunction(void)
{
	SetRingerVolume(volume_level_UI);
}

/****************************************************************************
	FUNCTION NAME		: SpeechVolumeExitFunction

  	PURPOSE				: Speech volume Exit function
						
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void SpeechVolumeExitFunction(void)
{
	SaveSpeechVolume((U8)volume_level_UI);
}

/****************************************************************************
	FUNCTION NAME		: RingVolumeExitFunction

  	PURPOSE				: Ring volume Exit function
						
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void RingVolumeExitFunction(void)
{
	SetActivatedRingVolumeLevel((MMI_VOLUME_LEVEL_ENUM)volume_level_UI);  //reopen by jinzh:20070719
}

/****************************************************************************
	FUNCTION NAME		: KeyPadVolumeFunction

  	PURPOSE				: Keypad volume function
						
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void KeyPadVolumeFunction(void)
{
	SetKeyPadVolume(volume_level_UI);
}
/****************************************************************************
	FUNCTION NAME		: KeyPadVolumeExitFunction

  	PURPOSE				: KeyPad Volume Exit Function
						
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void KeyPadVolumeExitFunction(void)
{isSetKeypadVolume=FALSE;	//Added by jinzh:20070721
	//CSD Brian added for setting activated keypad volume, 2003/12/02
	SetActivatedKeypadVolumeLevel((MMI_VOLUME_LEVEL_ENUM)volume_level_UI);  //zhoumn opened for bug 10264@2007/03/07
}

 
#if defined(__MMI_TOUCH_SCREEN__) //&& defined(__MMI_WALLPAPER_ON_BOTTOM__)
/*****************************************************************************
* FUNCTION
*  CallShowVerticalBarMax
*
* DESCRIPTION
*   Callback function for time synchronization of KEY_LONG_PRESS
*   and MMI_PEN_EVENT_LONG_TAP event
*
* PARAMETERS
*  void
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*  callback_timer
*
*****************************************************************************/
void CallShowVerticalBarMax(void)
{
	callback_timer++;
	if(callback_timer >= LONGPRESS_DELAY)
	{
		ShowVerticalBarMax();
		callback_timer = 0;
	}
}

/*****************************************************************************
* FUNCTION
*  CallShowVerticalBarMin
*
* DESCRIPTION
*   Callback function for time synchronization of KEY_LONG_PRESS
*   and MMI_PEN_EVENT_LONG_TAP event
*
* PARAMETERS
*  void
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
* 
*****************************************************************************/
void CallShowVerticalBarMin(void)
{
	callback_timer++;
	if(callback_timer >= LONGPRESS_DELAY)
	{
		ShowVerticalBarMin();
		callback_timer = 0;
	}
}

/*****************************************************************************
* FUNCTION
*  ResetCallbackTimer
*
* DESCRIPTION
*   Callback function of PEN_EVENT_UP event for reseting the
*   synchronization variable.
*
* PARAMETERS
*  void
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
* 
*****************************************************************************/
void ResetCallbackTimer(void)
{
	callback_timer = 0;
}

#endif
 
