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
 *	AlarmDef.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines string, screen, and image ids for alarm application.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: AlarmDef.h

  	PURPOSE		: Alarm application 

	REMARKS		: nil

	AUTHOR		: Gautam Saluja

	DATE		: June 16,03

**************************************************************/
#ifndef _MMI_ALARMDEF_H
#define _MMI_ALARMDEF_H

#include "mmi_data_types.h"
#include "datetimetype.h"
#include "alarmframeworkprot.h"
/* 
** Define
*/
#define SIZE_OF_DATE_STRING	(13*ENCODING_LENGTH) /*Jan, 10 2004 + 1*/

//zough add  20061130 begin
//#define MAX_SNOOZE_CHANCES	10
#define MAX_SNOOZE_CHANCES	3
//zough add  20061130 end

 

#define NUM_STATE_ACTINACT_ALARM 2

#ifdef __MMI_ALM_AUDIO_OPTIONS__			 
#define NUM_AUDIO_OPTIONS 2
#define MAX_FM_CHANNELS 9
#endif     

#define NUM_DAYS 7
#define HOUR_LENGTH 2
#define MINUTE_LENGTH 2
#define COLON_LENGTH  1
#define AM_PM_LENGTH	2
#define SPACE_LENGTH	1
#define SIZE_OF_ALARM_STRING  ((HOUR_LENGTH + MINUTE_LENGTH + COLON_LENGTH + AM_PM_LENGTH + SPACE_LENGTH + 1) * ENCODING_LENGTH)
#define NUM_FREQ_ALARM 5

#define SET_STATE	1
#define UNSET_STATE	0

#define NUM_MONTHS 12
#define NUM_MIN_HR  60
#define NUM_HRS_DAY  24

#ifdef __MMI_ALM_AUDIO_OPTIONS__
#define MAX_MANUAL_INPUT_LEN 6
#define MAX_NAME_INPUT_LEN 21
#define MIN_FM_FREQUENCY 875
#define MAX_FM_FREQUENCY 1080

typedef enum ALM_AUDIO_OPTIONS
{
	ALM_AUDIO_OPTIONS_TONE,
	ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL,
	ALM_AUDIO_OPTIONS_FMRADIO_MANUAL
}ALM_AUDIO_OPTIONS;
#endif

/*+3 for AM /PM*/


/* 
** Typedef 
*/
typedef struct
{
	U8	Hour;
	U8	Min;
	U8	State;
	U8	Freq;
	U8	Snooze;			/* number of snooze time */
	U8	Days;

#ifdef __MMI_ALM_CUST_VOLUME__
	U8	Volume;
#endif
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	U8  	AudioOption;    /*audio option, which is either tone or FM radio*/
	U16	AudioOptionValue;	/* Ring Tone ID or FM Frequency*/ 
#endif
#ifdef __MMI_ALM_SNOOZE_SETTING__
    unsigned char SnoozeInterval;
#endif 
}alm_nvram_struct;
#ifdef __MMI_ALM_SNOOZE_SETTING__
#define ALM_SNOOZE_INTERVAL_SETTING_NUM 11
#endif 

#ifdef __MMI_ALM_AUDIO_OPTIONS__
typedef struct
{
	alm_nvram_struct AlmList[NUM_OF_ALM];
	U16 alm_fm_selected_channel_names[NUM_OF_ALM][MAX_NAME_INPUT_LEN];

}alm_nvram_struct_extended;
#endif

typedef struct
{
	S32 CurrHiliteState;		/* enabled or disabled state */
	S32	CurrHiliteFreq;			/* index of current selected frequency, used in inline selection */
#ifdef __MMI_ALM_CUST_VOLUME__
	S32 CurrHiliteVol;			/* index of current highlighted volume */
#endif
	alm_nvram_struct AlmList[NUM_OF_ALM];
	S8  HourBuf[(HOUR_LENGTH+1)*ENCODING_LENGTH];	/*to take the input for hh*/
	S8  MinBuf[(MINUTE_LENGTH+1)*ENCODING_LENGTH];	/*to take the input for mm*/	
	U8	CurrHiliteAlarm;		/* index of current highlighted alarm */
	U8	IsSaveToHistory;		/* shall alarm screen save to history or not */
	U8	IsDOWSet;				/* to see if DOW component is enabled */
	U8	IsInPwronConfirm;	/* wait for power on confirmation */
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	S32 CurrHiliteAudioOption;  /*the current alarm's audio option*/
	U16 CurrHiliteAlmFMChannelName[MAX_NAME_INPUT_LEN];
	U16 CurrHiliteAudioOptionValue;		/*The value can be <Ring Tone ID> or <FM Frequency>*/
#endif
#ifdef __MMI_ALM_SNOOZE_SETTING__
    S32 CurrHilightSnoozeInterval;
#endif 

}alm_context_struct;



typedef enum MONTH_LIST  
{
	JAN=1,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC
} MONTH_LIST;


typedef enum TIME_EQUALITY
{
	TIME_LESS  = -1,
	TIME_EQUAL,	
	TIME_GREATER
} TIME_EQUALITY;

typedef enum 
{
	SCR_ID_ALM_LIST	=	ALARM_BASE+1,
	SCR_ID_ALM_SAVE_CONFIRM,
	SCR_ID_ALM_INDICATION,
	SCR_ID_ALM_PWRON_CONFIRM,
	SCR_ID_ALM_EDITOR,
	SCR_ID_ALM_SUB_INDICATION,
	SCR_ID_ALM_AUDIO_OPT_FMRADIO,
	SCR_ID_ALM_AUDIO_OPT_FMRADIO_CHANNEL_LIST,
	SCR_ID_ALM_AUDIO_OPT_FMRDO_CHANNEL_OPT,
	SCR_ID_ALM_FMRDO_PREV,
	SCR_ID_ALM_FMRDO_MANUAL_INPUT,
	SCR_ID_ALM_FMRDO_MANUAL_INPUT_OPT,
	SCR_ID_ALM_TONE_SETUP,
	SCR_ID_ALM_TONE_SETUP_EXT_MELODY,
	SCR_ID_ALM_TONE_SETUP_EXT_MELODY_DRV
} SCR_ID_ALM_ENUM;

typedef enum 
{	
	ORGANIZER_MENU_ALARMS_STRINGID = ALARM_BASE+1,
	ALARMS_STOP_STRINGID,
	ALARMS_SNOOZE_STRINGID,
	ALARM_EDIT_TITLE_STRINGID,
	ALARM_STR_DONE,
	ALARM_REPEAT_STRING,
	ALARM_ONCE_STRING,
	ALARM_DAYS_STRING,
	ALARM_EVREYDAY_STRING,
	ALARM_ON_STRING,
	ALARM_OFF_STRING,
	ALARMS_EDIT_SOFTKEY_STRINGID,
	ALARMS_POWER_ON,
	ALARMS_VOLUME,
	STR_ID_ALM_AUDIO_OPTIONS,
	STR_ID_ALM_AUDIO_OPTIONS_TONE,
	STR_ID_ALM_AUDIO_OPTIONS_FMRADIO,
	STR_ID_ALM_AUDIO_OPTIONS_FMRADIO_CHANNELLIST,
	STR_ID_ALM_AUDIO_OPTIONS_FMRADIO_MAN_INPUT,
	STR_ID_ALM_AUDIO_OPT_FMRDO_OPT_PREV,
	STR_ID_ALM_FMRDO_VALID_FM_BANDWIDTH,
	STR_ID_ALM_FMRDO_HEADFONE_REMINDER,
	STR_ID_ALM_FMRDO_AUD_OPT_INCOMPLETE_NOTIFY,
	STR_ID_ALM_FMRDO_PLEASE_PLUG_IN_EARPHONE,
	STR_ID_ALM_FMRDO_INCOMPLETE_CHANNEL_INFO,
	ALARMS_SETTING_SNOOZE_INTERVAL,
	STR_ID_ALM_DEFAULT,
	STR_ID_ALM_1,
	STR_ID_ALM_2,
	STR_ID_ALM_3,
	STR_ID_ALM_4,
	STR_ID_ALM_5,
	STR_ID_ALM_6,
	STR_ID_ALM_7,
	STR_ID_ALM_8,
	STR_ID_ALM_9,
	STR_ID_ALM_10,
	STR_ID_FMRDO_MENU
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#if defined(__MMI_MAINLCD_220X176__) || defined(__MMI_STYLE_E71_QCIF_S__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	,STR_ID_ALM_REDEFINE	
#endif	
} STR_ID_ALM_ENUM;

typedef enum 
{
	ALARMS_MAIN_TITLE_IMAGEID	 = ALARM_BASE+1,			
	ALARMS_EDIT_SOFTKEY_IMAGEID,
	ALARMS_SET_TITLE_IMAGEID,
	ALARM_ANIMATION_INDICATION_IMAGEID,
	ALARM_ANIMATION_INDICATION_SUBLCD_IMAGEID,
	ALARMS_STOP_IMAGEID,
	ALARMS_SNOOZE_IMAGEID,
	ORGANIZER_MENU_ALARMS_IMAGEID,
	ALARM_VOLUME_IMAGEID,
        ALARMS_SET_SNOOZE_PERIOD_IMAGEID,//16160
	IMG_ALM_AUD_OPT

} IMG_ID_ALM_ENUM;

/* 
** Extern Global Variable
*/
extern alm_context_struct g_alm_cntx;
/* 
** Extern Global Function
*/

#endif// #ifndef _MMI_ALARMDEF_H


