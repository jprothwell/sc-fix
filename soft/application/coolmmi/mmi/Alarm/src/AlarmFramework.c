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
 *	AlarmFramework.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements alarm scheduler for alarm, to do list, schedule power on/off and theme.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: AlarmFramework.c

  	PURPOSE		: Alarm Application

 

 

	DATE		: Jun 16,03

**************************************************************/
#ifndef _MMI_ALARMFRAMEWORK_C
#define _MMI_ALARMFRAMEWORK_C

/*  Include: MMI header file */
#include "mmi_features.h" 

/* to be put in include path TBDGS*/
#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "alarmprot.h"
#include "alarmgprot.h"
#include "datetimegprot.h"
#include "alarmframeworkprot.h"
#include "nvramenum.h"
#include "debuginitdef.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "sublcdhistorygprot.h"
#include "scheduledpoweronoffgprot.h"
#include "gpioinc.h"
#include "profilegprots.h"
#include "queuegprot.h"
#include "mmithemes.h"
#include "protocolevents.h"
#include "allappgprot.h"
#include "asynceventsgprot.h"
#include "eventsgprot.h"
#include "settinggprots.h"
#include "composeringtoneprots.h"
#include "downloadprotos.h"
#include "poweronchargerprot.h"
//#include "populateresgprot.h"
#include "scheduledpoweronoff.h"
#include "todolistdef.h"
#include "wrappergprot.h"
#include "audioinc.h"
#include "idleappprot.h"
//#include <time.h>	//delete by liuxn 060616
#include "globalscrenum.h"
#ifdef MMI_ON_HARDWARE_P
#include "resource_audio.h"
#include "lcd_sw.h"
#include "lcd_sw_inc.h"
#include "sublcd.h"
#endif
#ifdef __J2ME__
#include "javadef.h"
#endif
#undef __NEWSIMULATOR
/*  Include: PS header file */
//... Add More PS header
//#include "gdi_include.h"
#include "simdetectiongexdcl.h"
#include "bootup.h"
//#include "mmitaskprot.h"
#include "usbdevicegprot.h"		//qiff open for compile warnning //delete by liuxn 060616
#include "mmi_trace.h"
#include "tm.h"

#if defined(__MMI_FM_RADIO__)
#include "fmradiodef.h"
#include "fmradioprot.h"
#include "fmradiotype.h"
extern mmi_fmrdo_struct g_fmrdo; 
#endif


#include "networksetupgprot.h"
/* 
** Typedef 
*/


/* 
** Define
*/
#define ALM_AFTER_CM_DURATUION 5000
#define ALM_MAX_DELAY_ALLOWANCE	7	/* maximum delay of each alarm */
#if defined(__PROJECT_GALLITE_C01__)
#define ALM_TOLERANCE_TIME 		5000	//10000
#else
#define ALM_TOLERANCE_TIME 		10000
#endif
#define ALM_POWER_OFF_TOLERANCE	(POWER_OFF_TIMER_STAGE_1 + POWER_OFF_TIMER_STAGE_2 + ALM_TOLERANCE_TIME)

//add by liuxn 060616 start
#if defined(__MMI_THEMES_APPLICATION__)
extern thm_contex_struct	g_thm_cntx;
#endif

//add by liuxn 060616 end
/* 
** Local Variable
*/

/* 
** Local Function
*/

/* 
** Global Variable
*/
alm_queue_node_struct gAlmQueue[ALM_NUM_OF_ALAMRS];
alm_framework_context_struct g_alm_frm_cntx;
extern U8 IsBeforePowerOn;
#ifdef __MMI_FACTORY_MODE__
extern U8 gFactoryAlarm;
#endif

#ifdef MMI_ON_HARDWARE_P
	extern kal_uint16 *MainLCD_ShadowBuf_p;
	extern kal_uint16 *SubLCD_ShadowBuf_p;
	extern U8 gFScheckdiskflag;
	extern rtc_format_struct gclockData;
#endif

extern void mdi_audio_suspend_background_play();
const U8 gAlmDayOfWeek[] =
{
	0x01,			// DAY_SUN
	0x02,			// DAY_MON
	0x04,			// DAY_TUE
	0x08,			// DAY_WED
	0x10,			// DAY_THU
	0x20,			// DAY_FRI
	0x40			// DAY_SAT
};

/* 
** Global Function
*/
extern void QuitSystemOperation(void);
extern void InitUnicodeSupport(void);
extern void ResetLanguageSettingFlag(void);
extern U16 GetTotalCallCount(void);
extern void MMICheckDiskDisplay(void);
extern void ShutDownSystemOperation2(void);
extern void PmgInitExtMelodyStruct(void);
extern void InitFramework(void);
extern void gdi_init(void);
extern void ShutdownSystemOperation(void);
extern void InitSoundEffect(void);

#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)
extern void mmi_fmsr_util_get_setting_for_alarm(U16 index, MYTIME *alarmTime, U8 *freq, U8 * weekday);
extern void mmi_fmsr_alarm_expire_ind(U16 index, BOOL power_on_record);
extern void mmi_fmsr_init_app(void);
#endif

#ifdef MMI_ON_HARDWARE_P
#ifdef __EM_MODE__
extern void custom_em_pwn_cycle(kal_bool is_on);
#endif
#endif

/*****************************************************************************
* FUNCTION
*	InitAlarmHandler
* DESCRIPTION
*	Initialize Alarm handler
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void InitAlarmHandler(void)
{
#ifdef MMI_ON_HARDWARE_P
	SetProtocolEventHandler(AlmExpiryHandler,MSG_ID_MMI_EQ_ALARM_IND);
#endif
}


/*****************************************************************************
* FUNCTION
*	InitAlarmFrameWork
* DESCRIPTION
*	Initialize global context and retrieve all kinds of alarm from NVRAM.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void InitAlarmFrameWork(void)
{
	S32 i,j;

	InitAlarmHandler();

	AlmInitNvram();
	SPOFInitNvRam();

	#if defined(__MMI_TODOLIST__)
		InitToDoListApp();
	#endif
	#ifdef __MMI_THEMES_APPLICATION__
		ReadThemesFromNVRAM();// added by lin for saving default theme @20070627
	#endif

	AlmInitQueue();

	memset(&g_alm_frm_cntx, 0, sizeof(alm_framework_context_struct));
	g_alm_frm_cntx.CurrAlmIndex = 0xff;

	for(i=0;i<ALM_NUM_OF_ALAMRS && gAlmQueue[i].Index < ALM_INDEX_TOTAL; i++);

	if(i)
	{
		g_alm_frm_cntx.EnabledAlarms = (U8)i;
		AlmCheckAlarmState();
	}

	mmi_trace(g_sw_ALARM, "gdm: alarm total =%d", i);

	for (j=0;j < i; j++)
		mmi_trace(g_sw_ALARM, "gdm:gAlmQueue[%d].index=%d, TIme.day=%d, TIme.hour=%d, Time.MIN=%d", j, gAlmQueue[j].Index, gAlmQueue[j].Time.nDay, gAlmQueue[j].Time.nHour, gAlmQueue[j].Time.nMin);
	
}


/*****************************************************************************
* FUNCTION
*	AlmCancelAlarmRsp
* DESCRIPTION
*	Protocol Handler for Message Alarm Del Response
*	Enabled Expiry Handler
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void AlmCancelAlarmRsp(void *msg)
{
#ifdef MMI_ON_HARDWARE_P
	SetProtocolEventHandler(AlmExpiryHandler,MSG_ID_MMI_EQ_ALARM_IND);
#endif
}


/*****************************************************************************
* FUNCTION
*	AlmSetAlarmRSP
* DESCRIPTION
*	Protocol Handler for Message Alarm Set Response
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void AlmSetAlarmRSP(void *msg)
{
#ifdef MMI_ON_HARDWARE_P
	mmi_eq_set_rtc_time_rsp_struct *alarmRes;
	alarmRes=(mmi_eq_set_rtc_time_rsp_struct *) msg;

#ifdef MMI_ON_HARDWARE_P
	SetProtocolEventHandler(AlmExpiryHandler,MSG_ID_MMI_EQ_ALARM_IND);
#endif

	mmi_trace(g_sw_ALARM, "gdm: received MSG_ID_MMI_EQ_EXE_RTC_TIMER_RSP=%d into AlmSetAlarmRSP(): alarmRes->result=%d", MSG_ID_MMI_EQ_EXE_RTC_TIMER_RSP, alarmRes->result);

	if(alarmRes->result/*==MMI_TRUE*/)
	{
		#ifdef MMI_ON_HARDWARE_P
			#ifdef __MMI_FACTORY_MODE__
				if(gFactoryAlarm == 1)
				{
					gFactoryAlarm = 0;
					if(g_alm_frm_cntx.IsATPwrOffAnimation)
						ShutdownSystemOperation();
					else
						ShutdownSystemOperation();
						//custom_em_pwn_cycle(KAL_FALSE);
					return;
				}
			#endif
		#endif

		if(g_alm_frm_cntx.IsWaitForPwroff/* == ALM_STATE_POWER_OFF*/)
		{
			g_alm_frm_cntx.IsWaitForPwroff = 0;
			/* if charger connected or power on confirmation screen present */
			if(!g_alm_cntx.IsInPwronConfirm)
			{
				if(g_charbat_context.isChargerConnected)
				{
					g_alm_frm_cntx.IsPwronAlarm = FALSE;
					GoBackBothLCDHistory();			
				}				
				else
				{
					AlmSendPowerOff();
				}
			}
		}
	}
#endif
}


/*****************************************************************************
* FUNCTION
*	PendingAlarmReminder
* DESCRIPTION
*	Execute pending after a certain period after call or first time entry idle screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void PendingAlarmReminder(void)
{
	if(g_alm_frm_cntx.IsPendingAlm && g_alm_frm_cntx.IsAlmTonePlaying == FALSE)
		StartTimer(ALARM_TIMER_WAIT_FOR_CALL, ALM_AFTER_CM_DURATUION, AlmHandlePendingTimeout);
	else if(g_alm_frm_cntx.IsAlmTonePlaying)
		/*dongwg Modify Start For 6041 Ver: TBM780  on 2007-8-6 14:50 */
		//StartTimer(ALARM_ALERT_NOTIFYDURATION_TIMER, ALM_EXPIRE_DURATION, AlmHandleAlarmTimeout);
		coolsand_UI_start_timer(ALM_EXPIRE_DURATION,AlmHandleAlarmTimeout);
		/*dongwg Modify End  For 6041 Ver: TBM780  on 2007-8-6 14:50 */
}


/*****************************************************************************
* FUNCTION
*	PendingAlarmInPwronPeriod
* DESCRIPTION
*	Execute pending alarm in security check screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void PendingAlarmInPwronPeriod(void)
{
	if(g_alm_frm_cntx.IsPendingAlm)
		AlmHandlePendingTimeout();
}


/*****************************************************************************
* FUNCTION
*	AlmActualTime
* DESCRIPTION
*   Calculates the time when alarm will expire as per the frequency and weekdays.
* PARAMETERS
*	t			IN		time of the alarm.
*	frequency	IN		repeat of the alarm.
*	weekdays	IN		weekdays of the alarm.
*						Each bit is set for every day. Bit 0 for sunday and so on
*	is_tdl	IN		if the alarm is to do list.
* RETURNS
*	Next expired time of the alarm.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
MYTIME AlmActualTime(MYTIME t, FREQ_ALARM frequency, U8 weekDays, U8 is_tdl)
{
	MYTIME currTime, incTime;
	TIME_EQUALITY diffStatus;
	U8 i;

	DTGetRTCTime(&currTime);
	memset(&incTime, 0, sizeof(incTime));

	if( CompareTime(t, currTime, NULL) != TIME_GREATER)
	{
	   if((!is_tdl) || ((is_tdl==TRUE) && (frequency != WEEKLY) && (frequency != MONTHLY)))
	   {
   		t.nYear=currTime.nYear;
   		t.nMonth=currTime.nMonth;
   		t.nDay=currTime.nDay;		
	   }
	   else if(frequency == MONTHLY) /* to do list */
	   {
	      t.nYear = currTime.nYear;
	      t.nMonth = currTime.nMonth;
	   }
	}
	t.nSec = 0;
	t.DayIndex=DOW(t.nYear,t.nMonth,t.nDay);
	
	switch(frequency)
	{
		case ONCE:
		case EVERYDAY:			
		case ALM_FREQ_THM_EVERYDAY:
			diffStatus=CompareTime(t,currTime, NULL);
			if(diffStatus==TIME_LESS || diffStatus==TIME_EQUAL)
			{
				incTime.nDay=1;
				t=IncrementTime(t,incTime);
				
				if(frequency == ALM_FREQ_THM_EVERYDAY)	/* expired at 00:00:00 */
					t.nHour = t.nMin = 0;
			}
			break;
		case DAYS:
			for(i=0;i<7;i++)
			{
				if(gAlmDayOfWeek[(currTime.DayIndex+i)%7] & weekDays) 
					break;
			}
			if(!i) /* if it's the same day */
			{
				diffStatus=CompareTime(t,currTime, NULL);
				if(diffStatus==TIME_LESS || diffStatus==TIME_EQUAL)
				{
					/* Find Next Day When then the alarm is scheduled */
					for(i=1;i<7;i++)
					{
						if(gAlmDayOfWeek[(currTime.DayIndex+i)%7] & weekDays) break;
					}
				}
				else
				{
					break;
				}
			}
			incTime.nDay=i;
			t=IncrementTime(t,incTime);
			break;
		case WEEKLY:
			if(t.DayIndex==currTime.DayIndex)
			{
				t.nYear=currTime.nYear;
				t.nMonth=currTime.nMonth;
				t.nDay=currTime.nDay;		
				diffStatus=CompareTime(t,currTime, NULL);
				if(diffStatus==TIME_LESS || diffStatus==TIME_EQUAL)
				{
					incTime.nDay=7;
  					currTime.nHour = t.nHour;
  					currTime.nMin = t.nMin;
  					currTime.nSec = t.nSec;
					t=IncrementTime(currTime,incTime);
  					t.nSec = 0;
				}
			}
			else
			{
				diffStatus=CompareTime(t,currTime, NULL);
				if(diffStatus==TIME_LESS || diffStatus==TIME_EQUAL)
				{						
   					incTime.nDay=(7+t.DayIndex - currTime.DayIndex)%7;
   					currTime.nHour = t.nHour;
   					currTime.nMin = t.nMin;
   					currTime.nSec = t.nSec;
   					t=IncrementTime(currTime,incTime);
   					t.nSec = 0;
				}
			}
			break;
		case MONTHLY:
			diffStatus=CompareTime(t,currTime, NULL);
			if(diffStatus==TIME_LESS || diffStatus==TIME_EQUAL || (LastDayOfMonth(t.nMonth, t.nYear) < t.nDay))
			{
			   U8 i_tmp;
			   if(t.nMonth == currTime.nMonth) /* the same month */
			   {
				if(currTime.nMonth == 12)
				{
					i_tmp = 1;
					t.nYear++;
				}
				else
					i_tmp =(currTime.nMonth+1);
			   }
			   else
				i_tmp=currTime.nMonth;
			   
			for(; i_tmp <= 12; i_tmp++)
			{
   			t.nMonth = i_tmp;
			   if(LastDayOfMonth(t.nMonth, t.nYear) >= t.nDay)
				      break;
				}
			}
		break;
		case ALM_FREQ_THM_HOURLY:
			incTime.nHour = 1;
			t = IncrementTime(currTime,incTime);
			t.nSec = t.nMin = 0;
			break;
		case ALM_FREQ_THM_WEEKLY:
			t.nDay=currTime.nDay;
			t.nHour = 0,t.nMin = 0;
			t.DayIndex = weekDays;
			if(t.DayIndex==currTime.DayIndex)
			{
				diffStatus=CompareTime(t,currTime, NULL);
				if(diffStatus==TIME_LESS || diffStatus==TIME_EQUAL)
				{
					incTime.nDay=7;
					t=IncrementTime(t,incTime);
				}
			}
			else
			{
				incTime.nDay=(7+t.DayIndex - currTime.DayIndex)%7;
				t=IncrementTime(t,incTime);
			}			
 		    break;
		case ALM_FREQ_THM_MONTHLY:
			incTime.nMonth=1;
			t=IncrementTime(currTime,incTime);
			t.nSec = t.nMin = t.nHour = 0;
			t.nDay = 1;
			break;
		case ALM_FREQ_THM_SEASONALLY:
			t.nDay = 1;
			t.nHour = t.nMin = 0;
			switch(currTime.nMonth)
			{
				case 1:
				case 2:
				case 3: 
					t.nMonth = 4;
					break; 
				case 4:
				case 5:
				case 6: 
					t.nMonth = 7;
					break;
				case 7:
				case 8:
				case 9:
					t.nMonth = 10;
					break;
				default: /*case 10: case 11: case 12: */
					t.nMonth = 1;
					t.nYear =(currTime.nYear + 1);
					break;
			}
			break;
		default:
			break;
	}
	return t;
}


/*****************************************************************************
* FUNCTION
*	AlmsStopAlarmTone
* DESCRIPTION
*   Stops Any Alarm Tone that may be playing
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
static U8 curr_alert_type = 0; //qiff add for keep current use alert setting for exit the window

void AlmsStopAlarmTone(void)
{
	if(g_alm_frm_cntx.IsAlmTonePlaying)
	{
#ifndef __MMI_PROFILE_ALERT_TYPE_DOMINANT__
 
		//curr_alert_type = GetMtCallAlertTypeEnum();
		if(curr_alert_type!= MMI_VIBRATION_ONLY && curr_alert_type != MMI_SILENT)
#endif
		{

				//guojian modified for 8764 20080619
			if(curr_alert_type == MMI_VIBRATION_THEN_RING)
			{
				stopRequestedTone(ALARM_TONE); 
				mdi_audio_resume_background_play();
				VibratorOff();

			}
			else if(curr_alert_type == MMI_VIBRATION_AND_RING)
			{
				stopRequestedTone(ALARM_TONE); 
				mdi_audio_resume_background_play();
				VibratorOff();
			}
			else
			{
				stopRequestedTone(ALARM_TONE); 
				mdi_audio_resume_background_play();

			}
		}
		else
		{
			VibratorOff();
		}
		g_alm_frm_cntx.IsAlmTonePlaying=FALSE;
	}
}


/*****************************************************************************
* FUNCTION
*	playAlarmTone
* DESCRIPTION
*   Play alarm tone After startTimer
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void playAlarmTone(void)		//Added by jinzh:20070703
{
	playRequestedTone(ALARM_TONE);
}
/*dongwg Add Start For 6067 Ver: TBM780  on 2007-8-3 16:43 */
/*****************************************************************************
* FUNCTION
*	AlmstopVibratorThenRing
* DESCRIPTION
*   Play alarm tone After Vibrator
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmstopVibratorThenRing(void)	
{	
	VibratorOff();
	coolsand_UI_cancel_timer(AlmstopVibratorThenRing);
	mdi_audio_suspend_background_play();
	playAlarmTone();
}
/*dongwg Add End  For 6067 Ver: TBM780  on 2007-8-3 16:43 */
/*****************************************************************************
* FUNCTION
*	AlmPlayAlarmTone
* DESCRIPTION
*   Play alarm tone if it's not playing.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmPlayAlarmTone(void)
{
	if(g_alm_frm_cntx.IsAlmTonePlaying == FALSE)
	{
#ifndef __MMI_PROFILE_ALERT_TYPE_DOMINANT__
		 curr_alert_type = GetMtCallAlertTypeEnum();
	
		if(curr_alert_type != MMI_VIBRATION_ONLY && curr_alert_type != MMI_SILENT)
#endif
		{
			 
#ifdef __MMI_ALM_AUDIO_OPTIONS__
			if(g_alm_frm_cntx.CurrAlmIndex >= ALM_ALM_START && g_alm_frm_cntx.CurrAlmIndex < ALM_SPOF_START)
			{
				if((g_alm_cntx.AlmList[g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START].AudioOption == ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL)||(g_alm_cntx.AlmList[g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START].AudioOption == ALM_AUDIO_OPTIONS_FMRADIO_MANUAL))
				{
					mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER_BOTH, (mdi_callback) mmi_fmrdo_preempted );
					mdi_fmr_set_freq( g_alm_cntx.AlmList[g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START].AudioOptionValue);
				}
				else //this is for the case when tone is the selected audio option for alarm
				{
					playRequestedTone(ALARM_TONE);
				}
			}
			else
				playRequestedTone(ALARM_TONE);
#else		
			/*dongwg Add Start For 6067 Ver: TBM780  on 2007-8-3 16:49 */
			if(curr_alert_type == MMI_VIBRATION_THEN_RING)
			{
				VibratorOn();
				coolsand_UI_start_timer(3000,AlmstopVibratorThenRing);
			}
			else if(curr_alert_type == MMI_VIBRATION_AND_RING)
			{
				VibratorOn();
				mdi_audio_suspend_background_play();
				playAlarmTone();
			}
			/*dongwg Add End  For 6067 Ver: TBM780  on 2007-8-3 16:49 */
			else
			{
				mdi_audio_suspend_background_play();     //Added by jinzh:20070703
				playAlarmTone();   //Added by jinzh:20070703
			}
#endif
		}
		else
		{
			VibratorOn();//dongwg move it here For 6067 Ver: TBM780 on 2007-8-3 16:43
		}
		 
		g_alm_frm_cntx.IsAlmTonePlaying=TRUE;
	}
}


/*****************************************************************************
* FUNCTION
*	AlmInitLCDModule
* DESCRIPTION
*   Initialize main and sub lcd
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmInitLCDModule(void)
{
	gdi_init();
}


/*****************************************************************************
* FUNCTION
*	AlmInitRTCPwron
* DESCRIPTION
*   Initialize all required data and modules for RTC power on.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmInitRTCPwron(void)
{
	InitUnicodeSupport();
	g_pwr_context[0].PowerOnPeriod=1;
	InitFramework(); /* Initialize framework */
	//InitHardwareEvents();	//delete by liuxn 060616
	setup_UI_wrappers();
	PopulateResData();
	PowerAndEndKeyHandler();	/*deepali*/
	initialize_UI_demo();
	InitProfileApp();
	InitMMI_App();
	InitChargerPwrOn();

	//InitDownloadNVRAM();	//delete by liuxn 060616
	initnvramprofapp();
	InitSettingNVRAM();
#if defined(__MMI_RING_COMPOSER__) && defined(__MMI_IMELODY_SUPPORT__)		
	InitRingToneComposer();
#endif

#ifdef __MMI_USB_SUPPORT__
      InitMMIUsbContext();
#endif
/*do this init for g800 crash alarm when poweroff,the reason is use bt varibles in musictartmode in med*/
#ifdef TGT_GALLITE_G800
#ifdef __MMI_BT_SUPPORT__
         mdi_audio_init_event_hdlrs(); //added by zhujianguo. 2009.6.16
        mmi_bt_init();//licheng add for RDA bt project   
#endif
#endif
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	PmgInitExtMelodyStruct();
#endif

	InitAlarmFrameWork();

	InitSoundEffect();

	g_alm_frm_cntx.IsPwronAlarm=TRUE;

	AlmExecPwrOffAlarm();
}


/*****************************************************************************
* FUNCTION
*	AlmInitRTCPwron
* DESCRIPTION
 
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmSendPwronReq()
{
#ifdef MMI_ON_HARDWARE_P
	MYQUEUE Message;
	mmi_eq_power_on_req_struct *myMsgPtr;
	myMsgPtr = (mmi_eq_power_on_req_struct*) OslConstructDataPtr(sizeof(mmi_eq_power_on_req_struct));
	myMsgPtr->fun = 2;
	myMsgPtr->rst = 0;
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	Message.oslMsgId = PRT_EQ_POWER_ON_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
	g_charbat_context.PowerOnCharger = 0; //add by panxu 2007-8-10 to change the power on mode
#endif 
}

/*****************************************************************************
* FUNCTION
*	AlmSetAlarm
* DESCRIPTION
*   To insert alarm into queue and register the nearest alarm to RTC.
*	Update alarm profile of L4C if it's normal alarm.
* PARAMETERS
*	index	IN	index of alarm
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void AlmSetAlarm(U8 index)
{
#ifdef MMI_ON_HARDWARE_P
	AlmInsertIntoQueue(index,TRUE);
	AlmStartAlarmOnHW(gAlmQueue[0].Time);
#endif
}

/*****************************************************************************
* FUNCTION
*	AlmSetAlarm
* DESCRIPTION
*   To remove alarm from queue and register the nearest alarm to RTC.
*	Update alarm profile of L4C if it's normal alarm.
* PARAMETERS
*	index	IN	index of alarm
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void AlmCancelAlarm(U8 index)
{
#ifdef MMI_ON_HARDWARE_P	
	AlmRemoveFromQueue(index, TRUE);

	/* re-schedule alarm of RTC, shall be implemented in another function */
	if(g_alm_frm_cntx.EnabledAlarms == 0)
		AlmStopAlarmOnHW();
	else
		AlmStartAlarmOnHW(gAlmQueue[0].Time);

	 
//	if(AlmGetType(index) == ALM_TYPE_ALARM)
//		AlmDelRTCInfoForAT(index - ALM_ALM_START);
	
#endif
}
/*****************************************************************************
* FUNCTION
*	AlmGetAlarmTime
* DESCRIPTION
*   Get the time of specific queue node.
* PARAMETERS
*	index	IN	index of alarm
* RETURNS
*	Expired time of the alarm.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
MYTIME AlmGetAlarmTime(U8 index)
{	
	U8 AlmType;
	MYTIME alarmTime, currTime, incTime;
	U8 Freq = 0;
	U8 WeekDays = 0;
	U8 is_todolist = FALSE;
	
	AlmType = AlmGetType(index);

	DTGetRTCTime(&currTime);
	memset(&alarmTime, 0, sizeof(MYTIME));
	
	switch(AlmType)
	{
		case ALM_TYPE_THEME:
		{
#if defined(__MMI_THEMES_APPLICATION__)
			Freq = g_thm_cntx.CurrUpdatePeriod+ALM_FREQ_THM_OFF;
			WeekDays = 1;
			break;
#endif
		}
		case ALM_TYPE_TDL:
		{
#if defined(__MMI_TODOLIST__)
			index -= ALM_TDL_START;
			alarmTime = g_tdl_cntx.TaskList[index].Time;
			Freq = g_tdl_cntx.TaskList[index].Repeat;
			WeekDays = g_tdl_cntx.TaskList[index].Days;
			is_todolist = TRUE;
#endif
			break;
		}
		case ALM_TYPE_ALARM:
		{
			index -= ALM_ALM_START;
			alarmTime.nHour = g_alm_cntx.AlmList[index].Hour;
			alarmTime.nMin = g_alm_cntx.AlmList[index].Min;
			Freq = g_alm_cntx.AlmList[index].Freq;
			WeekDays= g_alm_cntx.AlmList[index].Days;
			if(g_alm_cntx.AlmList[index].Snooze)
			{
				memset(&incTime, 0, sizeof(MYTIME));
				//incTime.nMin = (g_alm_cntx.AlmList[index].Snooze*ALM_SNOOZE_INTERVAL);
				incTime.nMin = AlmGetSnoozeInterval(index);/*qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__;*/ 
				alarmTime = IncrementTime(currTime, incTime);
			}
			break;
		}
		case ALM_TYPE_SPON:
		{
			index -= ALM_SPON_START;
			alarmTime.DayIndex=	DOW(currTime.nYear,currTime.nMonth,currTime.nDay);
			alarmTime.nDay = currTime.nDay;
			alarmTime.nHour = g_spof_cntx.SPOFList[index].Hour;
			alarmTime.nMin = g_spof_cntx.SPOFList[index].Min;
			alarmTime.nMonth = currTime.nMonth;
			alarmTime.nYear = currTime.nYear;
			Freq = EVERYDAY;
			WeekDays = ALM_ALL_DAYS;
			break;
		}
		case ALM_TYPE_SPOF:
		{
			index -= ALM_SPOF_START;
			alarmTime.DayIndex=	DOW(currTime.nYear,currTime.nMonth,currTime.nDay);
			alarmTime.nDay = currTime.nDay;
			alarmTime.nHour = g_spof_cntx.SPOFList[index].Hour;
			alarmTime.nMin = g_spof_cntx.SPOFList[index].Min;
			alarmTime.nMonth = currTime.nMonth;
			alarmTime.nYear = currTime.nYear;
			Freq = EVERYDAY;
			WeekDays = ALM_ALL_DAYS;
			break;
		}
		case ALM_TYPE_FACTORY:
		{
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_ENGINEER_MODE__) ) 	
			DTGetRTCTime(&currTime);
			memset(&incTime, 0, sizeof(MYTIME));
			incTime.nSec = ALM_FACTORY_RTC_TIME;
			return IncrementTime(currTime, incTime);
#endif
		}
		case ALM_TYPE_PWR_RESET:
		{
			DTGetRTCTime(&currTime);
			memset(&incTime, 0, sizeof(MYTIME));
			incTime.nSec = (g_alm_frm_cntx.ATPwrCycle%60);
			incTime.nMin = (g_alm_frm_cntx.ATPwrCycle/60);
			return IncrementTime(currTime, incTime);
		}

		case ALM_TYPE_FMSR:
		{
#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)	
			index -= ALM_FMSR_START;
			mmi_fmsr_util_get_setting_for_alarm(index, &alarmTime, &Freq, &WeekDays);
			is_todolist = TRUE;
#endif
			break;
		}
	}

	return AlmActualTime(alarmTime, (FREQ_ALARM)Freq, WeekDays, is_todolist);
}



/*****************************************************************************
* FUNCTION
*	AlmInsertIntoQueue
* DESCRIPTION
*   Insert an alarm node to the queue.
* PARAMETERS
*	index	IN	index of alarm
*	saveflag	IN	flag to determine save directly to NVRAM or not
* RETURNS
*	The position of alarm in queue.
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
U8 AlmInsertIntoQueue(U8 index, U8 saveflag)
{
	S32 i,j;
	TIME_EQUALITY equality;
	MYTIME	alarmTime;

	if(index >= ALM_INDEX_TOTAL)	/* exceeds boundary */
		return 0;

	AlmRemoveFromQueue(index, FALSE); /* node is in queue, remove it first */

	alarmTime = AlmGetAlarmTime(index);
	
	/* insert into proper position */
	for(i=0; i<g_alm_frm_cntx.EnabledAlarms; i++)
	{
		equality = CompareTime(alarmTime, gAlmQueue[i].Time, NULL);
		if((equality == TIME_LESS) || (equality == TIME_EQUAL && index < gAlmQueue[i].Index)) /* insert node here */
		{
			/*	shift all nodes right */
			for(j=g_alm_frm_cntx.EnabledAlarms-1; j>=i; j--)
				memcpy(&gAlmQueue[j+1], &gAlmQueue[j], sizeof(alm_queue_node_struct));
			break;
		}
	}

	gAlmQueue[i].Index = index;
	gAlmQueue[i].Time = alarmTime;
	gAlmQueue[i].IsExpired = 0;

	g_alm_frm_cntx.EnabledAlarms++;

	/* write alarm queue to NVRAM */
	if(saveflag)
		AlmWriteQueueToNvram();

	return (U8)i;

}


/*****************************************************************************
* FUNCTION
*	AlmRemoveFromQueue
* DESCRIPTION
*   Remove an alarm node from the queue.
* PARAMETERS
*	index		IN	index of alarm
*	saveflag	IN	flag to determine save directly to NVRAM or not
* RETURNS
*	Number of enabled alarms.
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
U8 AlmRemoveFromQueue(U8 index, U8 saveflag)
{
	S32 i;

	if(gAlmQueue[0].Index > ALM_INDEX_TOTAL)	/* queue is empty */
		return 0;

	for(i=0; i<ALM_NUM_OF_ALAMRS; i++)
		if(gAlmQueue[i].Index  == index)
			break;

	if(i == ALM_NUM_OF_ALAMRS)	/* alarm node not found */
		return 0;

	/* remove from queue */
	memcpy(&gAlmQueue[i], &gAlmQueue[i+1], sizeof(alm_queue_node_struct)*(ALM_NUM_OF_ALAMRS-i-1));

	g_alm_frm_cntx.EnabledAlarms--;

	memset(&gAlmQueue[g_alm_frm_cntx.EnabledAlarms], 0xff, sizeof(alm_queue_node_struct));

	/* write alarm queue to NVRAM */
	if(saveflag)
		AlmWriteQueueToNvram();

	return g_alm_frm_cntx.EnabledAlarms;
}

U8 AlmIsExpireAllowed(void)
{
	U8 state;
	pBOOL PowerOnCheck;
	U16 call_count = GetTotalCallCount();

	if(IsBeforePowerOn == 1)
		PowerOnCheck = MMI_FALSE; /* the case of alarm expiration + power on indication */
	else if(!IdleIsPowerOnPeriod())
		PowerOnCheck = MMI_TRUE;  /* the case of idle screen reached + alarm expiration */
	else if(IsScreenPresent(SCR_OPEN_SCREEN_ID))
		PowerOnCheck = MMI_TRUE;  /* the case of power on animation + MT call + IMEI + call end + alarm expiration */
	else if(IsScreenPresent(SCR_ID_NETSET_MANUAL_SEARCH_PROGRESS))
		PowerOnCheck = MMI_FALSE;   /* the case of network search manual progress */
	else if(IsScreenPresent(SCR_ID_NETSET_AUTO_SEARCH_PROGRESS))
		PowerOnCheck = MMI_FALSE;  /* the case of network search auto progress */
	else if(IsScreenPresent(SCR_ID_NETSET_MANUAL_REG_PROGRESS))
		PowerOnCheck = MMI_TRUE;  /* the case of network register auto progress */
	else
		PowerOnCheck = MMI_FALSE; /* the case of power on animation + alarm expiration */

/*dongwg Modify Start For 6299 Ver: TBM780  on 2007-9-13 16:32 */
//zough add for   20061130 begin
	state = ((call_count == 0) && (PowerOnCheck == MMI_TRUE) && (!g_alm_frm_cntx.IsExpiryDisabled));
	//state = ((call_count == 0) && (!g_alm_frm_cntx.IsExpiryDisabled));
//zough add for   20061130 end
/*dongwg Modify End  For 6299 Ver: TBM780  on 2007-9-13 16:32 */
	mmi_trace(1," AlmIsExpireAllowed %d , %d, %d, %d",state,call_count,PowerOnCheck,g_alm_frm_cntx.IsExpiryDisabled);

	return state;
}

/*****************************************************************************
* FUNCTION
*	AlmHandleAlarmTimeout
* DESCRIPTION
*   Timer expiry handler for normal alarm.
*	Exit previous alarm screen.
* PARAMETERS
*	index	IN	index of alarm
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void AlmHandleAlarmTimeout(void)
{
	/*dongwg Modify Start For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	//StopMyTimer(ALARM_ALERT_NOTIFYDURATION_TIMER);
	coolsand_UI_cancel_timer(AlmHandleAlarmTimeout);
	/*dongwg Modify End  For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	
	if(g_alm_frm_cntx.CurrAlmIndex == ALM_FACTORY)
	{
		AlmSendPowerOff();
	}
	else
	{
		if(AlmIsExpireAllowed())
		{
			g_alm_frm_cntx.IsExitByTimer = TRUE;
			AlmExitPreviousAlarm();
			g_alm_frm_cntx.IsExitByTimer = FALSE;

			if(g_alm_frm_cntx.IsPwronAlarm)
				AlmExecPwrOffAlarm();
			else
				AlmExecPwrOnAlarm();
			
			g_alm_frm_cntx.IsPendingAlm = FALSE;
		}
		else
		{
			g_alm_frm_cntx.IsPendingAlm = TRUE;
		}
	}
}



/*****************************************************************************
* FUNCTION
*	AlmExitPreviousAlarm
* DESCRIPTION
*   Execute exit handler of previous alarm to to do list alam.
*	Exit previous alarm screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmExitPreviousAlarm(void)
{
	U8 almType;

	/* if some alarm is previous displayed, execute previous exit handler */
	if(g_alm_frm_cntx.CurrAlmIndex < ALM_INDEX_TOTAL)
	{
		/* stop previous alarm */
		almType = AlmGetType(g_alm_frm_cntx.CurrAlmIndex);
		if(almType == ALM_TYPE_ALARM)
			/*qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__ */
			AlmSnoozeHandler();
			//AlmStopHandler();
#if defined(__MMI_TODOLIST__)
		/*dongwg Modify Start For 6138 Ver: TBM780  on 2007-8-27 17:58 */
		else if(almType == ALM_TYPE_TDL /*|| g_alm_frm_cntx.IsPwronAlarm == FALSE*/)
		/*dongwg Modify End  For 6138 Ver: TBM780  on 2007-8-27 17:58 */
			TDLExitReminderScreen();
#endif
	}

	g_alm_frm_cntx.CurrAlmIndex = 0xff;
}


/*****************************************************************************
* FUNCTION
*	EntryAlmDummyScr
* DESCRIPTION
*	Dummy screen to exit previous application (Java)
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void EntryAlmDummyScr(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
}

/*****************************************************************************
* FUNCTION
*	AlmExpiryHandler
* DESCRIPTION
*   To set alarm to RTC
* PARAMETERS
*	index	IN	index of alarm
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmExpiryHandler(void *msg)
{
	mmi_trace(g_sw_ALARM, "gdm: recived MSG_ID_MMI_EQ_ALARM_IND=%d into AlmExpiryHandler()!!!!!!!", MSG_ID_MMI_EQ_ALARM_IND);
	
#ifdef MMI_ON_HARDWARE_P
	mmi_trace(1,"AlmExpiryHandler  ");
// qiff del for compile warnning	mmi_eq_alarm_ind_struct    *local_data = (mmi_eq_alarm_ind_struct *)msg;
	/* update global time */

	/*dongwg Modify Start For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	//StopMyTimer(ALARM_ALERT_NOTIFYDURATION_TIMER);
	coolsand_UI_cancel_timer(AlmHandleAlarmTimeout);
	/*dongwg Modify End  For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	StopMyTimer(ALARM_TIMER_WAIT_FOR_CALL);

	//begin: add by zough 2006-11-8
/*	gclockData.rtc_wday = local_data->rtc_time.rtc_wday;
	gclockData.rtc_year = local_data->rtc_time.rtc_year;
	gclockData.rtc_mon = local_data->rtc_time.rtc_mon;
	gclockData.rtc_day = local_data->rtc_time.rtc_day;
	gclockData.rtc_hour = local_data->rtc_time.rtc_hour;
	gclockData.rtc_min = local_data->rtc_time.rtc_min;
	gclockData.rtc_sec = local_data->rtc_time.rtc_sec;*/
	//end: add by zough 2006-11-8

	/* execute exit handler of previous alarm */
	g_alm_frm_cntx.IsExitByTimer = TRUE;
	AlmExitPreviousAlarm();
	g_alm_frm_cntx.IsExitByTimer = FALSE;

	if(!AlmIsExpireAllowed())
	{
		g_alm_frm_cntx.IsPendingAlm = TRUE;
		return;
	}

	if(AlmIsPowerOffMode())
		g_alm_frm_cntx.IsPwronAlarm = TRUE;
	mmi_trace(1,"AlmExpiryHandler  %d",g_alm_frm_cntx.IsPwronAlarm);

	/* execute next alarm */
	if(g_alm_frm_cntx.IsPwronAlarm)
		AlmExecPwrOffAlarm();
	else
		AlmExecPwrOnAlarm();

#endif
}


/*****************************************************************************
* FUNCTION
*	AlmExecPwrOnAlarm
* DESCRIPTION
*   Popup one alarm entry from queue and execute corresponding handler.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmExecPwrOnAlarm(void)
{
#ifdef MMI_ON_HARDWARE_P
	U8 almType,almIndex;
	MYTIME currTime,alarmTime, incTime;
	TIME_EQUALITY equality;
	U8 NexAlmToShow = FALSE;
	U8 NextAlmToSet = FALSE;
	U8 AlmSnoozeTime, tmpIndex;
	U8 IsSPOFPending = FALSE;
#ifdef __J2ME__
	U8 IsJavaPlaying = FALSE;
#endif

	/*dongwg Modify Start For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	//StopMyTimer(ALARM_ALERT_NOTIFYDURATION_TIMER);
	coolsand_UI_cancel_timer(AlmHandleAlarmTimeout);
	/*dongwg Modify End  For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	g_alm_frm_cntx.CurrAlmIndex = 0xff;
	/* no alarm expired, to prevent abnormal case */
	if(!g_alm_frm_cntx.EnabledAlarms)
	{
		mmi_trace(1,"AlmExecPwrOnAlarm   g_alm_frm_cntx.EnabledAlarms = %d  ",g_alm_frm_cntx.EnabledAlarms  );
		MMI_TRACE((MMI_TRACE_G1_FRM, MMI_RTC_ALARM_EMPTY_QUEUE));
		g_alm_frm_cntx.IsPendingAlm = FALSE;
		return;
	}
	if(!AlmIsExpireAllowed())
	{
		mmi_trace(1,"AlmExecPwrOnAlarm   AlmIsExpireAllowed() = %d  ",AlmIsExpireAllowed()  );
		g_alm_frm_cntx.IsPendingAlm = TRUE;
		return;
	}
	DTGetRTCTime(&currTime);
	/* to prevent abnormal case */
	if(CompareTime(gAlmQueue[0].Time, currTime, NULL) == TIME_GREATER)
	{
		mmi_trace(1,"AlmExecPwrOnAlarm   TIME_GREATER  " );
		AlmStartAlarmOnHW(gAlmQueue[0].Time);
		g_alm_frm_cntx.IsPendingAlm = FALSE;
		return;
	}

	almIndex = gAlmQueue[0].Index;
	almType = AlmGetType(almIndex);
	mmi_trace(1,"AlmExecPwrOnAlarm   almType = %d ,g_alm_frm_cntx.IsSpofDisabled = %d  ",almType , g_alm_frm_cntx.IsSpofDisabled );
	/* spof is disabled, find out next alarm to execute */
	if(almType == ALM_TYPE_SPOF && g_alm_frm_cntx.IsSpofDisabled)
	{
		g_alm_frm_cntx.IsPendingAlm = TRUE;
		if(g_alm_frm_cntx.EnabledAlarms > 1) /* more alarms in queue */
		{
			if(CompareTime(gAlmQueue[1].Time, currTime, NULL) == TIME_GREATER)
			{
				AlmStartAlarmOnHW(gAlmQueue[1].Time);
				return;
			}
			else
			{
				almIndex = gAlmQueue[1].Index;
				almType = AlmGetType(almIndex);
				IsSPOFPending = TRUE;
			}
		}
		else
			return;
	}

	g_alm_frm_cntx.CurrAlmIndex = almIndex;

#ifdef __J2ME__
	if(GetActiveScreenId() == SCR_JAVA)
	{
		EntryAlmDummyScr(); /* for Java */
		IsJavaPlaying = TRUE;
	}
#endif

	/* re-organizer alarm queue */
//	if(!g_alm_frm_cntx.IsPendingAlm)
	{
		/* if it's a pending alarm, no need to re-schedule right now */
		if(AlmGetRepeat(almIndex) == ONCE)
		{
			AlmRemoveFromQueue(almIndex, TRUE);
		}
		else if(almType != ALM_TYPE_ALARM)	/* alarm shall be inserted into queue after stopped */
		{
			AlmInsertIntoQueue(almIndex,TRUE);
		}
		else
		{
			tmpIndex = almIndex-ALM_ALM_START;
			AlmSnoozeTime = g_alm_cntx.AlmList[tmpIndex].Snooze;
			g_alm_cntx.AlmList[tmpIndex].Snooze = 0;
			AlmInsertIntoQueue(almIndex, TRUE);
			g_alm_cntx.AlmList[tmpIndex].Snooze = AlmSnoozeTime; 
		}
	}	

	/* set another RTC if requried */
	if(g_alm_frm_cntx.EnabledAlarms > 0) /* second alarm exists */
	{
		memset(&incTime, 0, sizeof(MYTIME));
		incTime.nSec = 30;
		alarmTime = IncrementTime(currTime, incTime);
		equality = CompareTime(gAlmQueue[0].Time, alarmTime, NULL);
		/* if next alarm will expire within 30 seconds */
		if(equality == TIME_LESS || equality == TIME_EQUAL) /*alarm expired */
		{
			/* increment 30 seconds and then set another alarm */
			NexAlmToShow = TRUE;
			NextAlmToSet = TRUE;
		}
		else
		{
			NextAlmToSet = TRUE;
			alarmTime = gAlmQueue[0].Time;
		}
	}

	if(NextAlmToSet)
		AlmStartAlarmOnHW(alarmTime);
	/*dongwg Add Start For 6041 Ver: TBM780  on 2007-8-6 14:45 */
	/* no pending alarm */
	if(!NexAlmToShow)
	{
		if(almType == ALM_TYPE_TDL || almType == ALM_TYPE_ALARM)
			/*dongwg Modify Start For 6041 Ver: TBM780  on 2007-8-6 14:50 */
			//StartTimer(ALARM_ALERT_NOTIFYDURATION_TIMER, ALM_EXPIRE_DURATION, AlmHandleAlarmTimeout);
			coolsand_UI_start_timer(ALM_EXPIRE_DURATION,AlmHandleAlarmTimeout);
			/*dongwg Modify End  For 6041 Ver: TBM780  on 2007-8-6 14:50 */
	}

	/* execute corrpesonding handler */
	/*dongwg Add End  For 6041 Ver: TBM780  on 2007-8-6 14:45 */
	//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_RTC_ALARM_EXPIRY_TYPE, (S32)almType));
	
	mmi_trace(1," AlmExecPwrOnAlarm  almType = %d   ",almType);
	
	switch(almType)
	{
		case ALM_TYPE_THEME:
#if defined(__MMI_THEMES_APPLICATION__)
			ThemeAlarmCallBack();//modified by lin for theme @20070705
#ifdef __J2ME__
			if(IsJavaPlaying ==TRUE)
				GoBackHistory();
#endif
			AlmExecPwrOnAlarm();
#endif
			break;
		case ALM_TYPE_SPON:
			/* balance previous SPOF if exists */
			if(IsSPOFPending)
				AlmInsertIntoQueue(gAlmQueue[0].Index, TRUE);
#ifdef __J2ME__			
			if(IsJavaPlaying ==TRUE)
				GoBackHistory(); 
#endif
			AlmExecPwrOnAlarm();
			break;
		case ALM_TYPE_TDL:
#if defined(__MMI_TODOLIST__)
			EntryTDLReminder();
#endif
			break;
		case ALM_TYPE_ALARM:
			EntryAlmIndicationScreen();
			break;
		case ALM_TYPE_SPOF:
			SpofHandler(almIndex);
			break;
		case ALM_TYPE_FMSR:
#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)	
			mmi_fmsr_alarm_expire_ind((U16)(g_alm_frm_cntx.CurrAlmIndex - ALM_FMSR_START), TRUE);
#endif			
			break;
		default://case ALM_TYPE_FACTORY:
			break;
	}
/*dongwg Del Start For 6041 Ver: TBM780  on 2007-8-6 14:46 */
	#if 0
	//dongwg add start
	if(!NexAlmToShow)
	{
		if(almType == ALM_TYPE_TDL || almType == ALM_TYPE_ALARM)
			StartTimer(ALARM_ALERT_NOTIFYDURATION_TIMER, ALM_EXPIRE_DURATION, AlmHandleAlarmTimeout);
	}
	//dongwg add end
	#endif
/*dongwg Del End  For 6041 Ver: TBM780  on 2007-8-6 14:46 */
	/* because alarm has be executed and re-scheduled, it's not in pending state now */
	g_alm_frm_cntx.IsPendingAlm = FALSE;

#endif
}


/*****************************************************************************
* FUNCTION
*	AlmExecPwrOffAlarm
* DESCRIPTION
*   RTC power on handler
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmExecPwrOffAlarm(void)
{
	U8 almType,almIndex;
	MYTIME currTime,alarmTime, incTime;
	TIME_EQUALITY equality;
	U8 NexAlmToShow = FALSE;
	U8 NextAlmToSet = FALSE;

	/* no alarm expired, to prevent abnormal case */
	/*dongwg Modify Start For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	//StopMyTimer(ALARM_ALERT_NOTIFYDURATION_TIMER);
	coolsand_UI_cancel_timer(AlmHandleAlarmTimeout);
	/*dongwg Modify End  For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	g_alm_frm_cntx.CurrAlmIndex = 0xff;

#ifdef MMI_ON_WIN32
	EntryAlmIndicationScreen();
	return;
#endif

	if(!g_alm_frm_cntx.EnabledAlarms)
	{
		MMI_TRACE((MMI_TRACE_G1_FRM, MMI_RTC_ALARM_EMPTY_QUEUE));
#ifdef MMI_ON_HARDWARE_P
		AlmSendPowerOff();
#endif
		return;
	}

	/* compare the time */
	almIndex = gAlmQueue[0].Index;
	DTGetRTCTime(&currTime);
	if(CompareTime(gAlmQueue[0].Time, currTime, NULL) == TIME_GREATER)
	{
		g_alm_frm_cntx.IsWaitForPwroff = TRUE;
		AlmStartAlarmOnHW(gAlmQueue[0].Time);
		return;
	}

	almType = AlmGetType(almIndex);
	g_alm_frm_cntx.CurrAlmIndex = almIndex;

	if(AlmGetRepeat(almIndex) == ONCE)
		AlmRemoveFromQueue(almIndex, TRUE);
	else if(almType != ALM_TYPE_ALARM)	/* alarm shall be inserted into queue after stopped */
		AlmInsertIntoQueue(almIndex,TRUE);
	else
	{
		U8 tmpIndex, AlmSnoozeTime;
		tmpIndex = almIndex-ALM_ALM_START;
		AlmSnoozeTime = g_alm_cntx.AlmList[tmpIndex].Snooze;
		g_alm_cntx.AlmList[tmpIndex].Snooze = 0;
		AlmInsertIntoQueue(almIndex, TRUE);
		g_alm_cntx.AlmList[tmpIndex].Snooze = AlmSnoozeTime; 
	}

	if(g_alm_frm_cntx.EnabledAlarms > 0) /* second alarm exists */
	{
		/* increment 30 seconds and then set another alarm */
		memset(&incTime, 0, sizeof(MYTIME));
		incTime.nSec = 30;
		alarmTime = IncrementTime(currTime, incTime);

		equality = CompareTime(gAlmQueue[0].Time, alarmTime, NULL);
		if(equality == TIME_LESS || equality == TIME_EQUAL) /*alarm expired */
		{
			NexAlmToShow = TRUE;
			NextAlmToSet = TRUE;
		}
		else
		{
			alarmTime = gAlmQueue[0].Time;
			NextAlmToSet = TRUE;
		}
	}

	/* no pending alarm */
	if(!NexAlmToShow)
		if(almType == ALM_TYPE_ALARM || almType == ALM_TYPE_FACTORY \
			/*dongwg Add Start For 6138 Ver: TBM780  on 2007-8-27 19:33 */
			|| almType == ALM_TYPE_TDL)
			/*dongwg Add End  For 6138 Ver: TBM780  on 2007-8-27 19:33 */
			/*dongwg Modify Start For 6041 Ver: TBM780  on 2007-8-6 14:50 */
			//StartTimer(ALARM_ALERT_NOTIFYDURATION_TIMER, ALM_EXPIRE_DURATION, AlmHandleAlarmTimeout);
			coolsand_UI_start_timer(ALM_EXPIRE_DURATION,AlmHandleAlarmTimeout);
			/*dongwg Modify End  For 6041 Ver: TBM780  on 2007-8-6 14:50 */

	if(NextAlmToSet)
		AlmStartAlarmOnHW(alarmTime);	

	MMI_TRACE((MMI_TRACE_G1_FRM, MMI_RTC_ALARM_EXPIRY_TYPE, (S32)almType));

	switch(almType)
	{
		case ALM_TYPE_THEME:
#if defined(__MMI_THEMES_APPLICATION__)	
			ThemeAlarmCallBack();//modified by lin for theme @20070705
			AlmExecPwrOffAlarm();
#endif			
			break;
		case ALM_TYPE_TDL:
#if defined(__MMI_TODOLIST__)
			EntryTDLReminder();
			/*dongwg Add Start For 6582 Ver: TBM780  on 2007-9-14 14:46 */
			g_pwr_context[0].PowerOnPeriod=0;
			/*dongwg Add End  For 6582 Ver: TBM780  on 2007-9-14 14:46 */
/*dongwg Modify Start For 6138 Ver: TBM780  on 2007-8-24 16:29 */
#endif/*#else*/
		break;
//#endif			
/*dongwg Modify End  For 6138 Ver: TBM780  on 2007-8-24 16:29 */
		case ALM_TYPE_SPOF:
		{
			if(NextAlmToSet)		
				g_alm_frm_cntx.IsWaitForPwroff = TRUE;
			else
				AlmSendPowerOff();
			break;
		}
		case ALM_TYPE_ALARM:
		case ALM_TYPE_FACTORY:
			AlmInitLCDModule();
			EntryAlmIndicationScreen();
			g_pwr_context[0].PowerOnPeriod=0;
			break;
		case ALM_TYPE_SPON:
			AlmSendPowerOn();
			break;
		case ALM_TYPE_PWR_RESET:
			AlmSendPowerOn();
			break;
		case ALM_TYPE_FMSR:
#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)	
			AlmInitLCDModule();
			mmi_fmsr_init_app();
			mmi_fmsr_alarm_expire_ind((U16)(g_alm_frm_cntx.CurrAlmIndex - ALM_FMSR_START), FALSE);
			g_pwr_context[0].PowerOnPeriod=0;
#endif			
			break;			
	}
}


/*****************************************************************************
* FUNCTION
*	AlmWriteQueueToNvram
* DESCRIPTION
*   Write alarm queue to nvram.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmWriteQueueToNvram(void)
{
	S16 error;
	WriteRecord(NVRAM_EF_ALM_QUEUE_LID, (U16)1,(void*)gAlmQueue, NVRAM_ALM_QUEUE_SIZE, &error);
}


/*****************************************************************************
* FUNCTION
*	AlmInitQueue
* DESCRIPTION
*   Read alarm queue from nvram. 
*	Reset alarm queue if 
*		1. index misordered.
*		2. the expired time of first alarm is far from current time.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmInitQueue(void)
{
	S16 error;
	MYTIME currTime,refTime;

	ReadRecord(NVRAM_EF_ALM_QUEUE_LID,(U16)1,(void*)gAlmQueue,NVRAM_ALM_QUEUE_SIZE, &error);

	if(gAlmQueue[0].Index >= ALM_INDEX_TOTAL/* && gAlmQueue[0].Index != 0xff*/)/*o prevent alarm queue being corrupted */ 
		AlmReInitialize();
	else
	{
		memset(&refTime,0,sizeof(MYTIME));
		refTime.nMin = ALM_MAX_DELAY_ALLOWANCE;
		refTime = IncrementTime(gAlmQueue[0].Time, refTime);

		DTGetRTCTime(&currTime);

		if(CompareTime(currTime, refTime, NULL) == TIME_GREATER)
		{
			mmi_trace(g_sw_ALARM, "gdm:here currTime TIME_GREATER");
			AlmReInitialize();
		}
		/*wangrui add 20080105 begin  for fixbug 10180*/
		else if(CompareTimeNotDate(currTime, gAlmQueue[0].Time, NULL) == TIME_LESS)
		{
			mmi_trace(g_sw_ALARM, "gdm:here currTime TIME_LESS");
			AlmReInitialize();
		}
		/*wangrui add 20080105 end*/  
	}
}


/*****************************************************************************
* FUNCTION
*	AlmFindNodeInQueue
* DESCRIPTION
*   To see if the alarm node is currently in alarm queue
* PARAMETERS
*	index	IN	index of alarm
* RETURNS
*	0xff is the node is not in queue; otherwise, return the position of the alarm node.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
U8 AlmFindNodeInQueue(U8 index)
{
	U8 i;
	for (i=0;i<NUM_OF_ALM;i++)
	{
		if(gAlmQueue[i].Index == index)
			return i;

		if(gAlmQueue[i].Index > ALM_INDEX_TOTAL)
			return 0xff;
	}
	return 0;
}


/*****************************************************************************
* FUNCTION
*	AlmGetType
* DESCRIPTION
*   To get the type of alarm
* PARAMETERS
*	index	IN	index of alarm
* RETURNS
*	Enum value of alarm type
* GLOBALS AFFECTED
*	None
*****************************************************************************/
U8 AlmGetType(U8 index)
{
	if(index < ALM_THM_START)
		return ALM_TYPE_SPON;
	else if(index < ALM_TDL_START)
		return ALM_TYPE_THEME;
	else if(index < ALM_ALM_START)
		return ALM_TYPE_TDL;
#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)
	else if(index < ALM_FMSR_START)		/*Add FM Schedule Record*/
		return ALM_TYPE_ALARM;
	else if(index < ALM_SPOF_START)
		return ALM_TYPE_FMSR;
#else
	else if(index < ALM_SPOF_START)		/*No FM Schedule Record*/
		return ALM_TYPE_ALARM;
#endif
	else if (index < ALM_FACTORY)
		return ALM_TYPE_SPOF;
	else if(index == ALM_FACTORY)
		return ALM_TYPE_FACTORY;
	else
		return ALM_TYPE_PWR_RESET;
}

/*****************************************************************************
* FUNCTION
*	AlmGetRepeat
* DESCRIPTION
*   To get the repeat type of an alarm.
* PARAMETERS
*	index	IN	index of alarm
* RETURNS
*	Enum value of repeat type
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 AlmGetRepeat(U8 index)
{
#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)			
	MYTIME alarmTime;
	U8 Freq = 0;
	U8 WeekDays = 0;
#endif

	switch(AlmGetType(index))
	{
#if defined(__MMI_THEMES_APPLICATION__)	
		case ALM_TYPE_THEME:
			return g_thm_cntx.CurrUpdatePeriod+ALM_FREQ_THM_OFF;
#endif
#if defined(__MMI_TODOLIST__)
		case ALM_TYPE_TDL:
			return g_tdl_cntx.TaskList[index-ALM_TDL_START].Repeat;
#endif
		case ALM_TYPE_ALARM:
			return g_alm_cntx.AlmList[index - ALM_ALM_START].Freq;
		case ALM_TYPE_FACTORY:
		case ALM_TYPE_PWR_RESET:
			return ONCE;
#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)			
		case ALM_TYPE_FMSR:
			index -= ALM_FMSR_START;
			mmi_fmsr_util_get_setting_for_alarm(index, &alarmTime, &Freq, &WeekDays);
			return Freq;
#endif		
		default: // case ALM_TYPE_SPOF: // case ALM_TYPE_SPON:
			return EVERYDAY;
	}
}


/*****************************************************************************
* FUNCTION
*	AlmGetStringValue
* DESCRIPTION
*	Compute the string value of given integer. The string will dispaly exact 2 digits.
* PARAMETERS
*	StringVal	IN/OUT	pointer to store the result.
*	Val			IN		value to be computed
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmGetStringValue(S8* StringVal, U8 Val)
{
	S8 StrVal[16];

	sprintf(StrVal,"%02d",Val);
	AnsiiToUnicodeString(StringVal,StrVal);
}


/*****************************************************************************
* FUNCTION
*	AlmIsRTCPwron
* DESCRIPTION
*	To see if the it's RTC power on or not.
* PARAMETERS
*	None.
* RETURNS
*	The state of power on alarm.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 AlmIsRTCPwron(void)
{
	return g_alm_frm_cntx.IsPwronAlarm;
}


/*****************************************************************************
* FUNCTION
*	AlmStartAlarmOnHW
* DESCRIPTION
*	To register RTC alarm on HW
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmStartAlarmOnHW(MYTIME alarmTime)
{
	#ifdef MMI_ON_HARDWARE_P
		MYQUEUE Message;
		mmi_eq_set_rtc_time_req_struct *alarmReq;
		Message.oslSrcId=MOD_MMI;
		Message.oslDestId=MOD_L4C;
	 	Message.oslMsgId = MSG_ID_MMI_EQ_EXE_RTC_TIMER_REQ;
		alarmReq =  (mmi_eq_set_rtc_time_req_struct *) 
	               OslConstructDataPtr(sizeof(mmi_eq_set_rtc_time_req_struct));
		alarmReq->rtc_type = RTC_ALARM_IND;
		alarmReq->set_type = RTC_SETTING_TYPE_DATETIME;
   		alarmReq->info.alarm_format=DEVICE_AL_EN_ChkYMDHMS;
		alarmReq->info.alarm_index = RTC_ALARM1_IND; /* Should be always zeor as only 1 alarm*/
		alarmReq->info.type = RTC_ALARM_SOUND; 
		alarmReq->info.text[0] = '\0';
		alarmReq->info.recurr = RTC_ALARM_DAY1;
		alarmReq->info.data_time.rtc_year = alarmTime.nYear-YEARFORMATE;
		alarmReq->info.data_time.rtc_wday = alarmTime.DayIndex;   	 
		alarmReq->info.data_time.rtc_mon = alarmTime.nMonth;
		alarmReq->info.data_time.rtc_day = alarmTime.nDay;
		alarmReq->info.data_time.rtc_hour = alarmTime.nHour;
		alarmReq->info.data_time.rtc_min = alarmTime.nMin;
		alarmReq->info.data_time.rtc_sec = alarmTime.nSec;
		Message.oslDataPtr = (oslParaType *)alarmReq;
		Message.oslPeerBuffPtr = NULL;
		
		SetProtocolEventHandler(AlmSetAlarmRSP,MSG_ID_MMI_EQ_EXE_RTC_TIMER_RSP);
		ClearProtocolEventHandler(MSG_ID_MMI_EQ_ALARM_IND);
		OslMsgSendExtQueue(&Message);
#endif
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
}


/*****************************************************************************
* FUNCTION
*	AlmStopAlarmOnHW
* DESCRIPTION
*	To del RTC from HW.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmStopAlarmOnHW(void)
{
#ifdef MMI_ON_HARDWARE_P
		MYQUEUE Message;

		mmi_eq_del_rtc_timer_req_struct *alarmReq;
	 	Message.oslSrcId=MOD_MMI;
		Message.oslDestId=MOD_L4C;
		
		Message.oslMsgId =  MSG_ID_MMI_EQ_EXE_DEL_RTC_TIMER_REQ;

		alarmReq =  (mmi_eq_del_rtc_timer_req_struct *) 
	               OslConstructDataPtr( sizeof(mmi_eq_del_rtc_timer_req_struct));
	 
		alarmReq->rtc_type = RTC_ALARM_IND;
 		alarmReq->rtc_index = RTC_ALARM1_IND; 
		/* CSD end */
		Message.oslDataPtr = (oslParaType *)alarmReq;
		Message.oslPeerBuffPtr = NULL;
		SetProtocolEventHandler(AlmCancelAlarmRsp,MSG_ID_MMI_EQ_EXE_DEL_RTC_TIMER_RSP);
		ClearProtocolEventHandler(MSG_ID_MMI_EQ_ALARM_IND);
		
		OslMsgSendExtQueue(&Message);
#else
		StopMyTimer(ALARM_SIMULATE_ON_WIN32);
#endif

}


/*****************************************************************************
* FUNCTION
*	AlmReInitialize
* DESCRIPTION
*	Re construct whole alarm queue and realted variables.
* PARAMETERS
*	None.
* RETURNS
*	The state of power on alarm.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmReInitialize(void)
{
	S32 i;
	MYTIME currTime;

	AlmDisableExpiryHandler();
	
	AlmStopAlarmOnHW();

	memset(gAlmQueue, 0xff, sizeof(gAlmQueue));	

	g_alm_frm_cntx.EnabledAlarms = 0;

	/* theme */
#if defined(__MMI_THEMES_APPLICATION__)
	if(g_thm_cntx.CurrUpdatePeriod != THM_UPDATE_OFF)
		AlmInsertIntoQueue(ALM_THM_START, FALSE);
#endif

	/* tdl */
#if defined(__MMI_TODOLIST__)
	DTGetRTCTime(&currTime);
	for(i=0; i<NUM_OF_TDL; i++)
	{
		if(g_tdl_cntx.TaskList[i].Present== 1 && g_tdl_cntx.TaskList[i].Alarm == 1)
		{
			if(g_tdl_cntx.TaskList[i].Repeat == ONCE)
			{
				if(CompareTime(g_tdl_cntx.TaskList[i].Time, currTime, NULL) == TIME_GREATER)
					AlmInsertIntoQueue((U8)(i+ALM_TDL_START), FALSE);	
			}
			else
				AlmInsertIntoQueue((U8)(i+ALM_TDL_START), FALSE);
		}
	}
#endif

	/* schedule power on off */
	for(i=0; i<NUM_OF_SPOF; i++)
		if(g_spof_cntx.SPOFList[i].Status == 1)
		{
			if(g_spof_cntx.SPOFList[i].Type == SPOF_POWERON)
				AlmInsertIntoQueue((U8)(i+ALM_SPON_START), FALSE);
			else
				AlmInsertIntoQueue((U8)(i+ALM_SPOF_START), FALSE);
		}

	/* alarm */
	for(i=0; i<NUM_OF_ALM; i++)
	{
		#if 0//gdm 090605 fix bug 11768
		g_alm_cntx.AlmList[i].Snooze = 0;
		#else
		mmi_trace(g_sw_ALARM, "gdm: into AlmReInitialize(): g_alm_cntx.AlmList[%d].Snooze=%d", i, g_alm_cntx.AlmList[i].Snooze);
		if( g_alm_cntx.AlmList[i].Snooze > MAX_SNOOZE_CHANCES)
			g_alm_cntx.AlmList[i].Snooze = 0;
		#endif
		
		if(g_alm_cntx.AlmList[i].State == 1)
			AlmInsertIntoQueue((U8)(i+ALM_ALM_START), FALSE);
	}

	AlmWriteQueueToNvram();

	if(gAlmQueue[0].Index != 0xff)
		AlmStartAlarmOnHW(gAlmQueue[0].Time);

	AlmEnableExpiryHandler();
}


/*****************************************************************************
* FUNCTION
*	AlmSendPowerOn
* DESCRIPTION
*	Display check disk screen, reset language, then send power on request.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmSendPowerOn(void) 
{
	AlmsStopAlarmTone();
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
/*Guojian Del Start For 10810 Ver: GreenStone  on 2008-12-19 13:47. the logo will be displayed when keypad poweron */
	#if 0
	MMICheckDiskDisplay();
	#endif
/*Guojian Del End  For 10810 Ver: GreenStone  on 2008-12-19 13:47 */
	ResetLanguageSettingFlag();		
	AlmSendPwronReq();
/* turn on backlight to make logo visible for the user */
	TurnOnBacklight(TRUE);
}


/*****************************************************************************
* FUNCTION
*	AlmSendPowerOff
* DESCRIPTION
*	Send power off request.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmSendPowerOff(void) 
{
	AlmCommonExitProcedure();
	QuitSystemOperation();
}


/*****************************************************************************
* FUNCTION
*	AlmCheckAlarmState
* DESCRIPTION
*	If first alarm time > currTime, start first alarm on hardware.
*	Else, set flag to indicate there is some pending alarm.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmCheckAlarmState(void) 
{
	MYTIME currTime;

	DTGetRTCTime(&currTime);

	if(CompareTime(gAlmQueue[0].Time, currTime, NULL) == TIME_GREATER)
	{
		AlmStartAlarmOnHW(gAlmQueue[0].Time);
	}
	else
	{
		g_alm_frm_cntx.IsPendingAlm = TRUE;
	}
}


/*****************************************************************************
* FUNCTION
*	AlmHandlePendingTimeout
* DESCRIPTION
*	Execute pending alarm
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmHandlePendingTimeout(void) 
{
	if(!AlmIsExpireAllowed())
	{
		g_alm_frm_cntx.IsPendingAlm = TRUE;
	}
	else
	{
		U8 almType;
		MMI_TRACE((MMI_TRACE_G1_FRM, MMI_RTC_ALARM_PENDING_TIMEOUT));

		almType = AlmGetType(g_alm_frm_cntx.CurrAlmIndex);

		/* stop previous alarm */
		AlmExitPreviousAlarm();

		if(almType != ALM_TYPE_ALARM)
		{
			/* alarm will be executed in the snooze handler. */
			if(AlmIsPowerOffMode())
				AlmExecPwrOffAlarm();
			else
				AlmExecPwrOnAlarm();
		}
	}
}


/*****************************************************************************
* FUNCTION
*	AlmDisableSPOF
* DESCRIPTION
*	Disable schedule power off functionality.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmDisableSPOF(void) 
{
	MMI_TRACE((MMI_TRACE_G1_FRM, MMI_RTC_ALARM_DISABLE_SPOF));
	g_alm_frm_cntx.IsSpofDisabled = TRUE;
}


/*****************************************************************************
* FUNCTION
*	AlmEnableSPOF
* DESCRIPTION
*	Enable schedule power off functionality.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmEnableSPOF(void)
{
	MMI_TRACE((MMI_TRACE_G1_FRM, MMI_RTC_ALARM_ENABLE_SPOF));
	if(g_alm_frm_cntx.IsSpofDisabled)
	{
		g_alm_frm_cntx.IsSpofDisabled = FALSE;
		PendingAlarmReminder();
	}
}


/*****************************************************************************
* FUNCTION
*	AlmDisableExpiryHandler
* DESCRIPTION
*	Disable alarm epxiry handler.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmDisableExpiryHandler(void)
{
	MMI_TRACE((MMI_TRACE_G1_FRM, MMI_RTC_ALARM_DISABLE_ALARM));   
	g_alm_frm_cntx.IsExpiryDisabled = TRUE;
}


/*****************************************************************************
* FUNCTION
*	AlmEnableExpiryHandler
* DESCRIPTION
*	Enable alarm epxiry handler.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmEnableExpiryHandler(void)
{
	MMI_TRACE((MMI_TRACE_G1_FRM, MMI_RTC_ALARM_ENABLE_ALARM));  
	if(g_alm_frm_cntx.IsExpiryDisabled)
	{
		g_alm_frm_cntx.IsExpiryDisabled = FALSE;
		PendingAlarmReminder();
	}
}


/*****************************************************************************
* FUNCTION
*	AlmDeInit
* DESCRIPTION
*	Inspec queue status and pre-set next alarm if required.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmDeInit(void)
{
#ifdef MMI_ON_HARDWARE_P

	if(gAlmQueue[0].Index < ALM_INDEX_TOTAL)
	{	
		MYTIME currTime, incTime;
	
		DTGetRTCTime(&currTime);
		
		memset(&incTime,0,sizeof(MYTIME));
		incTime.nSec = (ALM_POWER_OFF_TOLERANCE/1000);

		currTime = IncrementTime(currTime, incTime);

		if(CompareTime(gAlmQueue[0].Time, currTime , NULL) != TIME_GREATER)/* alarm almost expire */
		{
			AlmStartAlarmOnHW(currTime);
		}
	}
#endif
}


/*****************************************************************************
* FUNCTION
*	AlmIsTonePlaying
* DESCRIPTION
*	Query if alarm tone playing.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 AlmIsTonePlaying(void)
{
   return g_alm_frm_cntx.IsAlmTonePlaying;
}

//qiff disable the function:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__
#if 0
U8 AlmGetSnoozeInterval(void)
{
	U8 snooze;
	S16 error;
	ReadValue(NVRAM_ALM_SNOOZE_TIME, &snooze, DS_BYTE, &error);

	if(snooze == 0 || snooze > 59)
	{
		snooze = ALM_SNOOZE_INTERVAL;
		WriteValue(NVRAM_ALM_SNOOZE_TIME, &snooze, DS_BYTE, &error);
 	}
 
	return snooze;
}
#endif
/*****************************************************************************
* FUNCTION
*	FactorySetAlarm
* DESCRIPTION
*	Set facrtory mode alarm.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_ENGINEER_MODE__)  )   
void FactorySetAlarm(U8 sec)
{
	AlmSetAlarm(ALM_FACTORY);	
}

#endif /* (defined(MMI_ON_HARDWARE_P) && defined(__MMI_ENGINEER_MODE__)  ) */

/*****************************************************************************
* FUNCTION
*	AlmATPowerReset
* DESCRIPTION
*	Set an reboot alarm for AT command
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void AlmATPowerReset(U8 power_off, U8 time)
{
#ifdef __MMI_FACTORY_MODE__
	/* reuse the flag of factory alarm */
	gFactoryAlarm = 1;

	if(power_off)
		g_alm_frm_cntx.ATPwrCycle = time;
	else
		g_alm_frm_cntx.ATPwrCycle = ALM_FACTORY_RTC_TIME;

	g_alm_frm_cntx.IsATPwrOffAnimation = power_off;
	
	AlmSetAlarm(ALM_PWR_RESET);
#endif

}


/*****************************************************************************
* FUNCTION
*	AlmIsPowerOffMode
* DESCRIPTION
*	Function to check if it's currently in power off mode
* PARAMETERS
*	None.
* RETURNS
*	TRUE if alarm shall be executed as power off mode; otherwise, FALSE.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
BOOL AlmIsPowerOffMode(void)
{
	mmi_trace(g_sw_ALARM, "MMI_ALARM: entry %s()  function, File: %s, Line: %d,g_alm_frm_cntx.IsPwronAlarm=%d", __FUNCTION__, __FILE__, __LINE__,g_alm_frm_cntx.IsPwronAlarm);	
	mmi_trace(g_sw_ALARM, "MMI_ALARM: entry %s()  function, File: %s, Line: %d,g_pwr_context.PowerOnMode=%d", __FUNCTION__, __FILE__, __LINE__,g_pwr_context[0].PowerOnMode);	
	mmi_trace(g_sw_ALARM, "MMI_ALARM: entry %s()  function, File: %s, Line: %d,g_charbat_context.PowerOnCharger=%d", __FUNCTION__, __FILE__, __LINE__,g_charbat_context.PowerOnCharger);	
	return ((g_alm_frm_cntx.IsPwronAlarm == TRUE) ||(g_pwr_context[0].PowerOnMode == POWER_ON_ALARM) || g_charbat_context.PowerOnCharger);
}

#endif// #ifndef _MMI_ALARMFRAMEWORK_C


