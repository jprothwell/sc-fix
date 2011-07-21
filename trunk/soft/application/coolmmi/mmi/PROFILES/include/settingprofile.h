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
 * SettingProfile.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for profiles application structure definitions
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

	FILENAME	: SettingProfile.h

  	PURPOSE		: Profiles application structure definitions

 

	AUTHOR		: Ashima Bajaj

	DATE		: 

**************************************************************/

#ifndef _PIXCOM_PROFILES_H
#define _PIXCOM_PROFILES_H

#include "mmi_data_types.h"
#include "wgui_categories.h"
#include "miscfunctions.h"
#include "historygprot.h"
#include "custmenures.h"
#include "unicodexdcl.h"
#include "audioinc.h"
#define MAX_ELEMENTS 7
#define  STRING_LENGTH 12
#define USE_NVRAM_FOR_PROFILES

#ifndef NVRAM_PROFILES_RECORD_SIZE
		#define NVRAM_PROFILES_RECORD_SIZE		308
#endif

extern U8 gprofileheadset;

#define MMI_VOLUME_LEVEL_ENUM U8

#define LEVEL1 0
#define LEVEL2 1
#define LEVEL3 2
#define LEVEL4 3
#define LEVEL5 4
#define LEVEL6 5
#define LEVEL7 6


/*typedef enum {
CALL_ALERT_SILENT,
RING,
VIBRATION_ONLY,
VIBRATION_AND_RING,
VIBRATION_THEN_RING
}MT_CALL_ALERT_TYPE_ENUM;
*/
#define MMI_LEDLIGHT_STATUS_ENUM U8

#define NO_COLOR 0
#define COLOR_PATTERN1 1
#define COLOR_PATTERN2 2
#define COLOR_PATTERN3 3
#define COLOR_PATTERN4 4
#define COLOR_PATTERN5 5
#define COLOR_PATTERN6 6


typedef struct {
U8 setsec;
}SET_TIMER;


typedef struct {
MMI_LEDLIGHT_STATUS_ENUM status;
pBOOL lcdBacklight;
SET_TIMER timer;
}LIGHT;

#define MMI_RING_TYPE_ENUM U8

#define SINGLE		0
#define REPEAT		1
#define ASCENDING	2

#define POWEROFF_TONE_ENUM U16

#define POWEROFF_SILENT  0
#define POWEROFF_TONE1 SOUND_1
#define POWEROFF_TONE2 ((SOUND_1 )+ (1))

#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
#define	POWEROFF_TONE3 ((SOUND_1 )+ (2))
#define	POWEROFF_TONE4 ((SOUND_1 )+ (3))
#define	POWEROFF_TONE5 ((SOUND_1 )+ (4))
#define	POWEROFF_TONE6 ((SOUND_1 )+ (5))
#define	POWEROFF_TONE7 ((SOUND_1 )+ (6))
#define	POWEROFF_TONE8 ((SOUND_1 )+ (7))
#define	POWEROFF_TONE9 ((SOUND_1 )+ (8))
#define	POWEROFF_TONE10 ((SOUND_1 )+ (1))
#endif

#define POWERON_TONE_ENUM U16

#define POWERON_SILENT  0
#define POWERON_TONE1 SOUND_1
#define POWERON_TONE2 ((SOUND_1 )+ (1))

#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
#define POWERON_TONE3 ((SOUND_1 )+ (2))
#define POWERON_TONE4 ((SOUND_1 )+ (3))
#define POWERON_TONE5 ((SOUND_1 )+ (4))
#define POWERON_TONE6 ((SOUND_1 )+ (5))
#define POWERON_TONE7 ((SOUND_1 )+ (6))
#define POWERON_TONE8 ((SOUND_1 )+ (7))
#define POWERON_TONE9 ((SOUND_1 )+ (8))
#define POWERON_TONE10 ((SOUND_1 )+ (9))
#endif

#define COVER_OPEN_TONE_ENUM U16

#define COVER_OPEN_SILENT  0
#define TONE1 SOUND_1
#define TONE2 ((SOUND_1 )+ (1))
#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
#define	TONE3 ((SOUND_1 )+ (2))
#define TONE4 ((SOUND_1 )+ (3))
#define	TONE5 ((SOUND_1 )+ (4))
#define	TONE6 ((SOUND_1 )+ (5))
#define	TONE7 ((SOUND_1 )+ (6))
#define	TONE8 ((SOUND_1 )+ (7))
#define	TONE9 ((SOUND_1 )+ (8))
#define	TONE10 ((SOUND_1 )+ (9))
#endif

#define COVER_CLOSE_TONE_ENUM U16

#define	COVER_CLOSE_SILENT  0
#define COVER_CLOSE_TONE1  SOUND_1
#define COVER_CLOSE_TONE2 ((SOUND_1 )+ (1))

#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
#define COVER_CLOSE_TONE3 ((SOUND_1 )+ (2))
#define COVER_CLOSE_TONE4 ((SOUND_1 )+ (3))
#define COVER_CLOSE_TONE5 ((SOUND_1 )+ (4))
#define COVER_CLOSE_TONE6 ((SOUND_1 )+ (5))
#define	COVER_CLOSE_TONE7 ((SOUND_1 )+ (6))
#define	COVER_CLOSE_TONE8 ((SOUND_1 )+ (7))
#define	COVER_CLOSE_TONE9 ((SOUND_1 )+ (8))
#define	COVER_CLOSE_TONE10 ((SOUND_1 )+ (9))
#endif

#define MESSAGE_TONE_ENUM U16
#define MESSAGE_SILENT  0
#define MESSAGE_TONE1 SOUND_1
#define MESSAGE_TONE2 ((SOUND_1 )+ (1))
#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
#define MESSAGE_TONE3 ((SOUND_1 )+ (2))
#define MESSAGE_TONE4 ((SOUND_1 )+ (3))
#define MESSAGE_TONE5 ((SOUND_1 )+ (4))
#define MESSAGE_TONE6 ((SOUND_1 )+ (5))
#define MESSAGE_TONE7 ((SOUND_1 )+ (6))
#define MESSAGE_TONE8 ((SOUND_1 )+ (7))
#define MESSAGE_TONE9 ((SOUND_1 )+ (8))
#define MESSAGE_TONE10 ((SOUND_1 )+ (9))
#endif	
#define KEYPAD_TONE_ENUM U8

#define KEYPAD_SILENT 0
#define KEYPAD_CLICK 1
#define KEYPAD_TONE 2
#define KEYPAD_TONE_HUMAN_VOICE_1 3
#define KEYPAD_TONE_HUMAN_VOICE_2 4
#define KEYPAD_TONE_HUMAN_VOICE_3 5
#define KEYPAD_TONE_HUMAN_VOICE_4 6
#define KEYPAD_TONE_HUMAN_VOICE_5 7
#define KEYPAD_TONE_HUMAN_VOICE_6 8
#define KEYPAD_TONE_CUST_1 9
#define KEYPAD_TONE_CUST_2 10
#define KEYPAD_TONE_CUST_3 11
#define KEYPAD_TONE_CUST_4 12
#define KEYPAD_TONE_CUST_5 13
#define KEYPAD_TONE_CUST_6 14
	
#define ALARM_TONE_ENUM U16


#define	ALARM_TONE1  MIN_RING_TONE_ID
#define	ALARM_TONE2 ((MIN_RING_TONE_ID)+(1))
#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
#define	ALARM_TONE3 ((MIN_RING_TONE_ID)+(2))
#define	ALARM_TONE4 ((MIN_RING_TONE_ID)+(3))
#define	ALARM_TONE5 ((MIN_RING_TONE_ID)+(4))
#define	ALARM_TONE6 ((MIN_RING_TONE_ID)+(5))
#define	ALARM_TONE7 ((MIN_RING_TONE_ID)+(6))
#define	ALARM_TONE8 ((MIN_RING_TONE_ID)+(7))
#define	ALARM_TONE9 ((MIN_RING_TONE_ID)+(8))
#define	ALARM_TONE10 ((MIN_RING_TONE_ID)+(9))
#endif
#define	ALARM_TONE16  MIN_MIDI_ID
#define	ALARM_TONE17 ((MIN_MIDI_ID)+(1))

#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
#define	ALARM_TONE18 ((MIN_MIDI_ID)+(2))
#define	ALARM_TONE19 ((MIN_MIDI_ID)+(3))
#define	ALARM_TONE20 ((MIN_MIDI_ID)+(4))
#define	ALARM_TONE21 ((MIN_MIDI_ID)+(5))
#define	ALARM_TONE22 ((MIN_MIDI_ID)+(6))
#define	ALARM_TONE23 ((MIN_MIDI_ID)+(7))
#define	ALARM_TONE24 ((MIN_MIDI_ID)+(8))
#define	ALARM_TONE25 ((MIN_MIDI_ID)+(9))
#define	ALARM_TONE26 ((MIN_MIDI_ID)+(10))
#define	ALARM_TONE27 ((MIN_MIDI_ID)+(11))
#define	ALARM_TONE28 ((MIN_MIDI_ID)+(12))
#define	ALARM_TONE29 ((MIN_MIDI_ID)+(13))
#define	ALARM_TONE30 ((MIN_MIDI_ID)+(14))
#endif	

typedef struct{
   U16 ringTone;	
   POWEROFF_TONE_ENUM powerOffTone;
   POWERON_TONE_ENUM powerOnTone;
   COVER_OPEN_TONE_ENUM coverOpenTone;
   COVER_CLOSE_TONE_ENUM coverCloseTone;
   MESSAGE_TONE_ENUM messageTone;
   KEYPAD_TONE_ENUM keypadTone;
   U16 alarmTone;
   #ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
   U16 card2_ringTone;
   U16 card2_messageTone;
   #endif                                                        //Add by Renyh for 221 project   20090213
} TONE_SETUP;
 

/*typedef enum{
	CALL_TIME_REMINDER_OFF,
	CALL_TIME_REMINDER_SINGLE,
	CALL_TIME_REMINDER_REPEAT
}CALL_TIME_REMINDER_ENUM;
*/
typedef struct {
pBOOL coverAnswer; 
pBOOL anyKey; 
pBOOL automatic;
}MMI_ANSWERING_MODE;

#define MMI_FONT_SIZE_ENUM U8

#define THEME_DEFAULT 0
#define SMALL 1 
#define MEDIUM 2 
#define LARGE 3


typedef struct{
pBOOL errorTone;
pBOOL connectTone;
pBOOL campOnTone;
pBOOL warningTone;
}EXTRA_TONE;

typedef struct {
MMI_VOLUME_LEVEL_ENUM   ringVolumeLevel;
MMI_VOLUME_LEVEL_ENUM   keypadVolumeLevel;
MMI_VOLUME_LEVEL_ENUM   loudSpeakerVolumeLevel;
MMI_ALERT_TYPE mtCallAlertTypeEnum;
LIGHT   light;
MMI_RING_TYPE_ENUM ringTypeEnum;
TONE_SETUP toneSetup;
MMI_ANSWERING_MODE answeringMode;
MMI_FONT_SIZE_ENUM fontSizeEnum;
MMI_INTELLIGENT_CALL_ALERT_TYPE intelligentCallAlert;
EXTRA_TONE extraTone;
} PROFILE;

#define ALL_TONE_ENUM U8

#define ERROR_TONE  1
#define CONNECT_TONE 2
#define CAMP_ON_TONE 3
#define WARNING_TONE 4
#define INCOMING_CALL_TONE 5
#define ALARM_TONE 6
#define POWER_ON_TONE 7
#define POWER_OFF_TONE 8
#define COVER_OPEN_TONE 9
#define COVER_CLOSE_TONE 10
#define MESSAGE_TONE 11
#define KEYPAD_PLAY_TONE 12
#define SUCCESS_TONE 13
#define SAVE_TONE 14
#define EMPTY_LIST_TONE 15
#define GENERAL_TONE 16
#define SMS_IN_CALL_TONE 17
#define AUX_TONE	18
#define WARNING_TONE_IN_CALL 19 
#define ERROR_TONE_IN_CALL 20
#define CONNECT_TONE_IN_CALL 21
#define SUCCESS_TONE_IN_CALL 22
//Brian added for battery indication, 2003/11/17
#define BATTERY_LOW_TONE 23
#define BATTERY_WARNING_TONE 24
#define CALL_REMINDER_TONE 25
#define CCBS_TONE 26
#define CARD2_MESSAGE_TONE 27
/*add the process to the missed call sound warning by liuxl on 20090624*/			
#define MISSED_CALL_WARNING_TONE 28
 
//made 12 after hiran's approval.
#define  MAX_PROFILES_SCREEN_DIGITS 12

typedef enum
{
    MMI_PROFILE_GENERAL,
    MMI_PROFILE_MEETING,
    MMI_PROFILE_OUTDOOR,
    MMI_PROFILE_INDOOR,
    MMI_PROFILE_HEADSET,
    MMI_PROFILE_SILENT,
    MMI_PROFILE_BT
} MMI_PROFILE_ENUM_TYPE;


#endif

