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
 *	DateTime.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements date/time related utilities for other applications
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2004
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *	DateTime.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file implements date/time related utilities for other applications
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: DateTime.c

  	PURPOSE		: Utility functions to enable date and time display on idle screen

 

	 

	DATE		: Aug' 28, 2002

**************************************************************/

#ifndef _MMI_DATETIME_C
#define _MMI_DATETIME_C
#include "mmi_features.h"

/*  Include: MMI header file */
#include "stdafx.h"
#include "mmi_data_types.h"
#include "debuginitdef.h"
#include "datetimetype.h"
#include "eventsgprot.h"
#include "datetimegprot.h"
#include "l4dr.h"
//#include <time.h>   // zrx del 20060612
#include "timerevents.h"
#ifdef MMI_ON_HARDWARE_P
#include "rtc_sw.h"
#endif

#include "nvramtype.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "simdetectiongexdcl.h"
#include "alarmframeworkprot.h"

#ifdef __MMI_LCD_PARTIAL_ON__
#include "idleappprot.h"
#endif

#if defined(__MMI_SAVE_CURRENT_TIME__)
#include "custom_mmi_default_value.h"
#include "hal_timers.h"
#endif
#include "nvram_user_defs.h"
#include "settingdefs.h"
#ifdef __MMI_TTS_FEATURES__ 
extern nvram_tts_setting_struct g_tts_setting;
//add by ejTTS
U8 gSaveHour = -1;
#endif

/*  Include: PS header file */

/* 
** Define
*/
#define 	DT_UTC_BASE_YEAR	1970
#define	DT_MONTH_PER_YEAR		12
#define	DT_DAY_PER_YEAR			365
#define	DT_SEC_PER_DAY		86400
#define	DT_SEC_PER_HOUR		3600
#define	DT_SEC_PER_MIN		60
/* 
** Typedef 
*/


/* 
** Local Variable
*/
const U8 gDaysInMonth[NUM_MONTHS] = {31,28,31,30,31,30,31,31,30,31,30,31};


#ifdef MMI_ON_HARDWARE_P
	#define MMI_DT_YEAR_BOUNDARY		30
	extern rtc_format_struct gclockData;

   #ifdef __DAILY_WALLPAPER__
	   rtc_format_struct gWallpaperChangeDate;
   #endif
   
#else		// #ifdef MMI_ON_WIN32
	MYTIME MyTime={0,1,1,1,1,1,0}; // Variable used by Setting Application
	S8 DateTimeStrings[2][30];  
	static S8 NameOfMonth[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov","Dec"};
	static S8 NameOfday[7][4] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
#endif

U8 CMDateTimeRequest=0;
U8 CMCallCount=0;
U8 CMCallBackCount=0;

DateTimeFuncPtr CMDateTimeCallBack[MAX_CM_CALLS];


/* 
** Local Function
*/
static void DTResetTime(void);

/* 
** Global Variable
*/
// extern U8 idleScreenFirst;
// Lisen0522
//tangjie del 20060616
#if 0
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
extern BOOL  gIsInSleepMode;
#endif
#endif
//tangjie del 20060616

//tangjie add begin 20060616
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)
BOOL  gIsInSleepMode = KAL_FALSE;
#endif
//tangjie add end 20060616

/* 
** Global Function
*/
//add by zough 061121 start
#ifndef __MOD_ORGANIZER__
extern FLOAT GetTimeZone(U8 cityIndex);
#else
extern S32 GetTimeZone(U8 cityIndex);
#endif
//add by zough 061121 end
extern U8 PhnsetGetHomeCity(void);


#ifdef MMI_ON_WIN32
#if 0
/*****************************************************************************
* FUNCTION
*	EntryAlmMenu
* DESCRIPTION
*   Fucntion to update year and month, this function is used in PC simulator only.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void MyMonth()
{
	if(MyTime.nMonth == 12)
	{
		MyTime.nMonth = 1;
		MyTime.nYear++;
	}
	else
		MyTime.nMonth++;
}

/*****************************************************************************
* FUNCTION
*	EntryAlmMenu
* DESCRIPTION
*   Fucntion to update and display date and time. This function is used in PC simulator only.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void DateTimerProc(void)
{
//#ifdef MMI_ON_HARDWARE_P
//	L4GetClockTime();
//#else
	S32 CompareDay = 28;
	S16 isLeapYear = 0;
	U8	nDateChanged = 0x0;
	U8	update_flag=0;

/********************************************************
Patch for the demo:: Min_Change_flag has been added to support low power mode.
(May be we should have timer for 60 seconds).
**********************************************************/
	S32 Min_Change_flag = 0;

	StartTimer(TIMER_REAL_TIME_CLOCK,1000,DateTimerProc); 

	if(MyTime.nSec == 59)
	{
		MyTime.nSec = 0;
		Min_Change_flag = 1;
		update_flag=1;
		if(MyTime.nMin == 59)
		{
			MyTime.nMin = 0;
			if(MyTime.nHour == 23)
			{
				MyTime.nHour = 0;
				nDateChanged = 0x1;
				update_flag=1;
				if(MyTime.DayIndex == 6)
					MyTime.DayIndex = 0;
				else
					MyTime.DayIndex++;
				switch(MyTime.nMonth)
				{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
						if(MyTime.nDay == 31)
						{
							MyTime.nDay = 1;
							MyMonth();
						}
						else
						{
							MyTime.nDay++;							
						}
					break;
				case 4:
				case 6:
				case 9:
				case 11:
						if(MyTime.nDay == 30)
						{
							MyTime.nDay = 1;
							MyMonth();
						}
						else
						{
							MyTime.nDay++;
						}
					break;
				case 2: //Check for leap year
						if(((MyTime.nYear % 100) == 0) && ((MyTime.nYear % 400) == 0))
							isLeapYear = 1;
						else if((MyTime.nYear % 4) == 0)
							isLeapYear = 1;
						else //Not a leap year
							isLeapYear = 0;
						if(isLeapYear)
							CompareDay = 29;
						else
							CompareDay = 28;
					//	(isLeapYear) ? CompareDay = 29 : CompareDay = 28;
						if(MyTime.nDay == CompareDay)
						{
							MyTime.nDay = 1;
							MyMonth();
						}
						else
						{
							MyTime.nDay++;
						}	
					break;
				default:
					break;
				}
				//Right now only for time...need to expand to accommodate year and month
				//update
			}
			else
			{
				MyTime.nHour++;
			}
		}
		else
		{
			MyTime.nMin++;
		}
	}
	else
	{
		MyTime.nSec++;
	}
	//sprintf(DateTimeStrings[0],"%s %02d %s %d",NameOfday[MyTime.DayIndex],MyTime.nDay,NameOfMonth[MyTime.nMonth-1],MyTime.nYear);
	//sprintf(DateTimeStrings[1],"%02d:%02d:%02d",MyTime.nHour,MyTime.nMin,MyTime.nSec);	
	if(update_flag)
	{  // Lisen0522
        	update_mainlcd_dt_display();
		update_sublcd_dt_display();
	}
//
}

#endif

/*****************************************************************************
* FUNCTION
*	CMCallBackTimer
* DESCRIPTION
*   Used to simulate CM time function on widows. Calls CM Callback function as set by CM
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CMCallBackTimer(void)
{
	if(CMDateTimeRequest)
	{
		MYTIME curtime;
		GetDateTime(&curtime);
		CMDateTimeCallBack[CMCallBackCount++](&curtime);
		if(CMCallBackCount==MAX_CM_CALLS) CMCallBackCount=0;
		CMDateTimeRequest--;
	}			
}

#endif	


#ifdef MMI_ON_HARDWARE_P
/*****************************************************************************
* FUNCTION
*	SetDateTime
* DESCRIPTION
*   Function to set date and time information to global variable of MMI
* PARAMETERS
*	t	IN	struct variable that stores date and time information from PS
* RETURNS
*	None.
* GLOBALS AFFECTED
*	external_global
*****************************************************************************/
void SetDateTime(void *t)
{
	rtc_format_struct    *local_data = (rtc_format_struct *)t;

	gclockData.rtc_wday = local_data->rtc_wday;
	gclockData.rtc_year = local_data->rtc_year;
	gclockData.rtc_mon = local_data->rtc_mon;
	gclockData.rtc_day = local_data->rtc_day;
	gclockData.rtc_hour = local_data->rtc_hour;
	gclockData.rtc_min = local_data->rtc_min;
	gclockData.rtc_sec = local_data->rtc_sec;

	if(gclockData.rtc_year > MMI_DT_YEAR_BOUNDARY)
	{
		gclockData.rtc_year = 0;	/* reset to 2000 */
		DTResetTime();
	}
	
}
#endif

#if defined(__MMI_SAVE_CURRENT_TIME__)
void	InitSystemTimeNVRAM(void)
{
	S16 error;
	INT32   ret = 0;
	U8 SavedSystemTime[7];
	U32 TotalMinute1,TotalMinute2;
    	TM_SYSTEMTIME  RtcTime,SysRtcTime;
	U8 gSaveCurrenSystemTimeFlag=FALSE;

	ReadValue(NVRAM_SAVE_CURRENT_TIME,&gSaveCurrenSystemTimeFlag, DS_BYTE, &error);
//	hal_HstSendEvent(0x55555555);
//	hal_HstSendEvent(gSaveCurrenSystemTimeFlag);
//	hal_HstSendEvent(0xAAAAAAAA);
	if(gSaveCurrenSystemTimeFlag != 0xff)
	{
		//gSaveCurrenSystemTimeFlag = 0;
		//WriteValue(NVRAM_SAVE_CURRENT_TIME, &gSaveCurrenSystemTimeFlag, DS_BYTE, &error);
		ret = ReadRecord(NVRAM_EF_SAVE_CURRENT_TIME_LID, 1, SavedSystemTime, sizeof(SavedSystemTime), &error);
		RtcTime.uSecond = SavedSystemTime[0];
	        RtcTime.uMinute = SavedSystemTime[1];
	        RtcTime.uHour = SavedSystemTime[2];
	        RtcTime.uDay = SavedSystemTime[3];
	        RtcTime.uMonth = SavedSystemTime[4];
	        RtcTime.uYear = SavedSystemTime[5]+2000; //Change default year is 2010
	        RtcTime.uDayOfWeek = SavedSystemTime[6]; // It's a Sunday

		TM_GetSystemTime(&SysRtcTime);
		
		TotalMinute1 = ((SysRtcTime.uYear-2000)*365+SysRtcTime.uMonth*30+SysRtcTime.uDay)*24*60+SysRtcTime.uMinute;
		TotalMinute2 = ((RtcTime.uYear-2000)*365+RtcTime.uMonth*30+RtcTime.uDay)*24*60+RtcTime.uMinute;

		if(TotalMinute1 < TotalMinute2)
			TM_SetSystemTime(&RtcTime);

	}

}

void	SaveCurrentSystemTime(void)
{
	MYTIME pSystemTime;	
	U8 SavedSystemTime[7];	
	S16 error;
	U8 CurrenSystemTimeFlag;

	ReadValue(NVRAM_SAVE_CURRENT_TIME,&CurrenSystemTimeFlag, DS_BYTE, &error);
	if(CurrenSystemTimeFlag == 0xff)
	{
		CurrenSystemTimeFlag = 0;
		WriteValue(NVRAM_SAVE_CURRENT_TIME, &CurrenSystemTimeFlag, DS_BYTE, &error);
	}
	DTGetRTCTime(&pSystemTime);
	SavedSystemTime[0] = pSystemTime.nSec;
	SavedSystemTime[1] = pSystemTime.nMin;
	SavedSystemTime[2] = pSystemTime.nHour;
	SavedSystemTime[3] = pSystemTime.nDay;
	SavedSystemTime[4] = pSystemTime.nMonth;
	SavedSystemTime[5] = (U8)(pSystemTime.nYear-2000);
	SavedSystemTime[6] = pSystemTime.DayIndex;
	
	WriteRecord(NVRAM_EF_SAVE_CURRENT_TIME_LID, 1, SavedSystemTime, sizeof(SavedSystemTime), &error);
}

#endif        

/*****************************************************************************
* FUNCTION
*	DTResetTimeRsp
* DESCRIPTION
*	Response handler of reset date/time.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	external_global
*****************************************************************************/
void DTResetTimeRsp(void* rsp)
{
#ifdef MMI_ON_HARDWARE_P
	AlmReInitialize();
	SetProtocolEventHandler(L4SetClockRSP,MSG_ID_MMI_EQ_SET_RTC_TIME_RSP);
#endif	
}


/*****************************************************************************
* FUNCTION
*	DTResetTime
* DESCRIPTION
*	Reset Date/Time function when time reaches boundary
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	external_global
*****************************************************************************/
static void DTResetTime(void)
{
#ifdef MMI_ON_HARDWARE_P
	MYQUEUE msgStruct;
	mmi_eq_set_rtc_time_req_struct *time;

	time = OslConstructDataPtr(sizeof (mmi_eq_set_rtc_time_req_struct));
	time->info.alarm_format = 0;
	time->info.alarm_index = 0;
	time->info.type = 0;
	time->info.text[0] = '\0';
	time->info.recurr = 0;

	time->rtc_type = RTC_TIME_CLOCK_IND;
	time->set_type = RTC_SETTING_TYPE_DATETIME;
	time->info.data_time.rtc_sec = 0;
	time->info.data_time.rtc_min = gclockData.rtc_min;
	time->info.data_time.rtc_hour = gclockData.rtc_hour;
	time->info.data_time.rtc_day = gclockData.rtc_day;
	time->info.data_time.rtc_mon = gclockData.rtc_mon;
	time->info.data_time.rtc_wday = gclockData.rtc_wday;
	time->info.data_time.rtc_year =  gclockData.rtc_year;

	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C;
	msgStruct.oslMsgId = MSG_ID_MMI_EQ_SET_RTC_TIME_REQ;
	msgStruct.oslSapId = 0;
	msgStruct.oslDataPtr = (oslParaType *)time;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);

	SetProtocolEventHandler(DTResetTimeRsp, MSG_ID_MMI_EQ_SET_RTC_TIME_RSP);

#endif
}
/*****************************************************************************
* FUNCTION
*	DateTimerIndication
* DESCRIPTION
*   Handler of MSG_ID_MMI_EQ_CLOCK_TICK_IND.
*	1. Update global variable in MMI.
*	2. Update date/time display in IDLE screen.
* PARAMETERS
*	t	IN	struct variable that stores date and time information from PS
* RETURNS
*	None.
* GLOBALS AFFECTED
*	external_global
*****************************************************************************/
void DateTimerIndication(void* p) 
{

#if  MMI_ON_HARDWARE_P

#ifdef __MMI_TTS_FEATURES__ 
	char readText[256];
	//char format[256];
	char temp[64];
	INT16 hour;
	MYTIME time;
#endif

   	mmi_eq_clock_tick_ind_struct    *local_data = (mmi_eq_clock_tick_ind_struct *)p;
	gclockData.rtc_wday = local_data->rtc_time.rtc_wday;
	gclockData.rtc_year = local_data->rtc_time.rtc_year;
	gclockData.rtc_mon = local_data->rtc_time.rtc_mon;
	gclockData.rtc_day = local_data->rtc_time.rtc_day;
	gclockData.rtc_hour = local_data->rtc_time.rtc_hour;
	gclockData.rtc_min = local_data->rtc_time.rtc_min;
	gclockData.rtc_sec = local_data->rtc_time.rtc_sec;

	if(gclockData.rtc_year > MMI_DT_YEAR_BOUNDARY)
	{
		gclockData.rtc_year = 0;	/* reset to 2000 */
		DTResetTime();
	}

#ifdef __DAILY_WALLPAPER__
     if( (IS_IDLE_ARRIVED) && (GetWallpaperRotateStatus() == MMI_TRUE) )
     {
           if( gWallpaperChangeDate.rtc_year == 0xFF \
      	   || gWallpaperChangeDate.rtc_mon == 0xFF  \
      	   || gWallpaperChangeDate.rtc_day == 0xFF )
            {
                  	gWallpaperChangeDate.rtc_wday = gclockData.rtc_wday;
                     gWallpaperChangeDate.rtc_year = gclockData.rtc_year;
                  	gWallpaperChangeDate.rtc_mon = gclockData.rtc_mon;
                  	gWallpaperChangeDate.rtc_day = gclockData.rtc_day;
                  	gWallpaperChangeDate.rtc_hour = gclockData.rtc_hour;
                  	gWallpaperChangeDate.rtc_min = gclockData.rtc_min;
                  	gWallpaperChangeDate.rtc_sec = gclockData.rtc_sec;
            }
           else if( gWallpaperChangeDate.rtc_year != gclockData.rtc_year \
      	   || gWallpaperChangeDate.rtc_mon != gclockData.rtc_mon \
      	   || gWallpaperChangeDate.rtc_day != gclockData.rtc_day )
      	   {
                    //S16 error;
                    WallPaperDailyChange();
                  	gWallpaperChangeDate.rtc_wday = gclockData.rtc_wday;
                     gWallpaperChangeDate.rtc_year = gclockData.rtc_year;
                  	gWallpaperChangeDate.rtc_mon = gclockData.rtc_mon;
                  	gWallpaperChangeDate.rtc_day = gclockData.rtc_day;
                  	gWallpaperChangeDate.rtc_hour = gclockData.rtc_hour;
                  	gWallpaperChangeDate.rtc_min = gclockData.rtc_min;
                  	gWallpaperChangeDate.rtc_sec = gclockData.rtc_sec;
                     //WriteRecord(NVRAM_MMI_EF_WALLPAPER_MODTIME_LID,1,(void *)&gWallpaperChangeDate, sizeof(gWallpaperChangeDate), &error);
      	   }
      }
#endif /* __DAILY_WALLPAPER__ */

#ifdef __MMI_LCD_PARTIAL_ON__
		if (IS_IDLE_ARRIVED || g_pwr_context[0].SimRemoveFlag)
		RedrawPartialOnScreenSaverIfOk();
#endif

       // Lisen0522
#if !defined(__MMI_GPIO_DISABLE_SLEEP_MODE__)       
       if (gIsInSleepMode == KAL_FALSE)
       {
  	  update_mainlcd_dt_display();
       }
#else
  	  update_mainlcd_dt_display();
#endif
       update_sublcd_dt_display();

#ifdef __MMI_TTS_FEATURES__
		//add by ejTTS
	//获取当前时间如果为整点，进行播报
	GetDateTime(&time);
	mmi_trace(1, "tts test, the time is:%d,%d,%d", time.nHour, time.nMin, time.nSec);
	//该点播报过一次后就不在播报
	if(time.nHour != gSaveHour
		&& time.nMin == 0)
	{
		gSaveHour = time.nHour;

		memset(readText, 0, 256);
		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_NOW));
		if(time.nHour == 12)
		{
			hour = time.nHour;
			UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_PM));
			csd_wsprintf(temp, "%d", hour);
			UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_HOUR));
		}
		else if(time.nHour == 0)
		{
			hour = time.nHour;
			UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_AM));
			csd_wsprintf(temp, "%d", hour);
			UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_HOUR));
		}
		else if(time.nHour > 12)
		{
			hour = time.nHour-12;
			UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_PM));
			csd_wsprintf(temp, "%d", hour);
			UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_HOUR));

		}else
		{
			hour = time.nHour;
			UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_AM));
			csd_wsprintf(temp, "%d", hour);
			UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_HOUR));

		}

		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_ZHENG));

				//如果开启则直接播放，否则不进行播放。
		if(g_tts_setting.curTTSAlertAlarmAutoStatus == 0)
		{
			ejTTS_PlayToPCM(readText, UCS2Strlen(readText)*2);
		}
	}
#endif

#else
	DateTimerProc();
#endif
}
 

void PSIDateTimerProc(void)
{
	StartTimer(TIMER_REAL_TIME_CLOCK, 3000, PSIDateTimerProc);
}
 
/*****************************************************************************
* FUNCTION
*	InitTime
* DESCRIPTION
*   Function to initialize golbal variables and local date and time.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	external_global
*****************************************************************************/
void InitTime(void)
{
/*#if 1 //def MMI_ON_HARDWARE_P*/
#ifdef MMI_ON_HARDWARE_P /* zhuoxz,2009-4-20 */
	#ifdef __DAILY_WALLPAPER__
	   memset(&gWallpaperChangeDate, 0xFF, sizeof(gWallpaperChangeDate) );  
	#endif
	StartTimer(TIMER_REAL_TIME_CLOCK, 3000, PSIDateTimerProc);
#else
	SYSTEMTIME systime;
	time_t ltime;
	S8 MyDay[4];
	S16 i;

	GetLocalTime(&systime);
	time( &ltime );
	strncpy(MyDay,ctime(&ltime),3);
         
	MyDay[3] = '\0';
         
	
	for(i=0;i<7;i++)
	{
		if(strncmp(MyDay,NameOfday[i],3) == 0)
			break;
	}

	MyTime.nYear = systime.wYear;
	MyTime.nMonth = (U8)systime.wMonth;
	MyTime.nDay = (U8)systime.wDay;
	MyTime.nHour = (U8)systime.wHour;
	MyTime.nMin = (U8)systime.wMinute;
	MyTime.nSec = (U8)systime.wSecond;
	MyTime.DayIndex = (U8)i;

	sprintf(DateTimeStrings[0],"%s  %02d %s  %d",NameOfday[MyTime.DayIndex],MyTime.nDay,NameOfMonth[MyTime.nMonth - 1],MyTime.nYear);
	//sprintf(DateTimeStrings[1],"%02d::%02d::%02d",MyTime.nHour,MyTime.nMin,MyTime.nSec);
	//sprintf(DateTimeStrings[1],"%02d:%02d",MyTime.nHour,MyTime.nMin);

	StartTimer(TIMER_REAL_TIME_CLOCK,1000,DateTimerProc);
#endif

}


/*****************************************************************************
* FUNCTION
*	InitTime
* DESCRIPTION
*   Function to get the current date and time.
* PARAMETERS
*	t	IN/OUT	variable to store the result.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void GetDateTime(MYTIME *t)
{	
#ifdef MMI_ON_HARDWARE_P
	t->nSec = gclockData.rtc_sec;
    t->nMin = gclockData.rtc_min;
    t->nHour = gclockData.rtc_hour;
    t->nDay = gclockData.rtc_day;
    t->nMonth = gclockData.rtc_mon;
    t->DayIndex = gclockData.rtc_wday;
    t->nYear = YEARFORMATE + gclockData.rtc_year;
#else
	t->DayIndex=MyTime.DayIndex;
	t->nDay=MyTime.nDay;
	t->nHour=MyTime.nHour;
	t->nMin=MyTime.nMin;
	t->nSec=MyTime.nSec;
	t->nMonth=MyTime.nMonth;
	t->nYear=MyTime.nYear;
#endif    
    t->DayIndex=DOW(t->nYear,t->nMonth,t->nDay);
}


/*****************************************************************************
* FUNCTION
*	DOW
* DESCRIPTION
*   Function to compute current day of week.
* PARAMETERS
*	y	IN	year of the date to be computed.	(example, 2004)
*	m	IN	month of the date to be computed
*	d	IN	day of the date to be computed
* RETURNS
*	U8	index of day of week
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 DOW(U16 y, U8 m, U8 d)
{
    if (m < 3)
    {
        m += 13;
        y--;
    }
    else m++;
    
    return (d + 26 * m / 10 + y + y / 4 - y / 100 + y / 400 + 6) % 7;
}


/*****************************************************************************
* FUNCTION
*	DOW
* DESCRIPTION
*   Function to find out if given year is a leap year
* PARAMETERS
*	y	IN	year of the date to be computed	(example, 2004)
* RETURNS	
*	1 if the year is leap; otherwise 0.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U16 IsLeapYear(U16 year)
{
	if((year % 400) == 0)
		return 1;
	else if ((year % 100) == 0)
		return 0;
	else if((year % 4) == 0)
		return 1;
	else
		return 0;
}


/*****************************************************************************
* FUNCTION
*	GetTimeDifference
* DESCRIPTION
*   Function to calculate the difference between 2 times.
* PARAMETERS
*	t1		IN		the target time
*	t2		IN		the base time	(t1 shall be greater than t2)
*	result	IN/OUT	result of t1-t2
* RETURNS	
*	NULL if t1 < t2; otherwise, return result
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
MYTIME *GetTimeDifference(MYTIME *t1, MYTIME *t2, MYTIME *result)
{
	/* Validate year ..no need of validating for month ..*/
 
	if(NULL == t2)
		return NULL;

	if(t1->nYear < t2->nYear) 
		return NULL;
	else
	{
		DecrementTime2(*t1, *t2, result);
		return result;
	}
}


/*****************************************************************************
* FUNCTION
*	FindMaxTime
* DESCRIPTION
*   Function to find the larger one between two times
* PARAMETERS
*	t1		IN		the first time to be compared.
*	t2		IN		the second time to be compared.
* RETURNS	
*	0 if t1 < t2; otherwise 1.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U16 FindMaxTime(MYTIME *t1, MYTIME *t2)
{
	if(t1->nYear == t2->nYear)
	{
		if(t1->nMonth == t2->nMonth)
		{
			if(t1->nDay == t2->nDay)
			{
				if(t1->nHour == t2->nHour)
				{
					if(t1->nMin == t2->nMin)
					{
						if(t1->nSec >= t2->nSec) return 1;
						else return 0;
					}
					else if(t1->nMin > t2->nMin) return 1;
					else return 0;
				}
				else if(t1->nHour > t2->nHour) return 1;
				else return 0;
			}
			else if(t1->nDay > t2->nDay) return 1;
			else return 0;
		}
		else if(t1->nMonth > t2->nMonth) return 1;
		else return 0;
	}
	else if(t1->nYear > t2->nYear) return 1;
	else return 0;		
}


/*****************************************************************************
* FUNCTION
*	CMGetExactTime
* DESCRIPTION
*   Used by CM to get the Exact Time. Asyncronous Call. Callback is passeds
* PARAMETERS
*	callback	IN		callback function to be called when get time response comes back
* RETURNS	
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
//extern void L4GetClockTime(oslTaskFuncPtr FuncRspPtr); // zrx add 20060612
void CMGetExactTime(DateTimeFuncPtr callback)
{
	#ifdef MMI_ON_HARDWARE_P
	{
		/*MYTIME curtime;
		DTGetRTCTime(&curtime);
		callback(&curtime);*/
		PRINT_INFORMATION(("<<** CMGetExactTime ** >> CMCallCount = %d, CMCallBackCount = %d\n", CMCallCount, CMCallBackCount));		
		CMDateTimeRequest++;
		CMDateTimeCallBack[CMCallCount++]=callback;
		if(CMCallCount==MAX_CM_CALLS) CMCallCount=0;
		L4GetClockTime(CMGetExactTimeCB);
		
	}
	#else
	{
		CMDateTimeRequest++;
		CMDateTimeCallBack[CMCallCount++]=callback;
		if(CMCallCount==MAX_CM_CALLS) CMCallCount=0;
		StartTimer(CM_GET_TIME_CALLBACK,1000,CMCallBackTimer);
	}		
	#endif	
}

void CMGetExactTimeCB(void *buf)
{
   #ifdef MMI_ON_HARDWARE_P
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	mmi_eq_get_rtc_time_rsp_struct    *local_data = (mmi_eq_get_rtc_time_rsp_struct *)buf;
	// Lisen0522
	extern void update_mainlcd_dt_display(void);
	extern void update_sublcd_dt_display(void);
   
     /*----------------------------------------------------------------*/
     /* Code Body                                                      */
     /*----------------------------------------------------------------*/

	gclockData.rtc_wday = local_data->rtctime[0].data_time.rtc_wday;
	gclockData.rtc_year = local_data->rtctime[0].data_time.rtc_year,
	gclockData.rtc_mon = local_data->rtctime[0].data_time.rtc_mon,
	gclockData.rtc_day = local_data->rtctime[0].data_time.rtc_day,
	gclockData.rtc_hour = local_data->rtctime[0].data_time.rtc_hour,
	gclockData.rtc_min = local_data->rtctime[0].data_time.rtc_min,
	gclockData.rtc_sec = local_data->rtctime[0].data_time.rtc_sec;

	PRINT_INFORMATION(("<<** CMGetExactTimeCB ** >>\n"));		

   if(CMDateTimeRequest > 0)
	{
		MYTIME curtime;		
		
		PRINT_INFORMATION(("<<** CMGetExactTimeCB ** >> CMCallCount = %d, CMCallBackCount = %d\n", CMCallCount, CMCallBackCount));		
		
		GetDateTime(&curtime);
		CMDateTimeCallBack[CMCallBackCount++](&curtime);
		if(CMCallBackCount==MAX_CM_CALLS) CMCallBackCount=0;
		CMDateTimeRequest--;
		return;
	}

       // Lisen0522
	update_mainlcd_dt_display();
	update_sublcd_dt_display();
	#endif
}

/*****************************************************************************
* FUNCTION
*	LastDayOfMonth
* DESCRIPTION
*   Function to find the last day of specific month.
* PARAMETERS
*	month	IN	month
*	year	IN	year
* RETURNS	
*	number of days
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 LastDayOfMonth(U8 month,U16 year) 
{	
	if(month != FEB) 
		return gDaysInMonth[month-1];
	else
		return gDaysInMonth[1] + IsLeapYear(year);
}


/*****************************************************************************
* FUNCTION
*	IncrementTime
* DESCRIPTION
*   Increase a reference time by some time
* PARAMETERS
*	timeToIncrement	IN			time to be increased
*	decrement		IN			time to increase
* RETURNS	
*	result of computation
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
MYTIME IncrementTime(MYTIME timeToIncrement,MYTIME increment)
{
	MYTIME Result;
	S16 NumOfDays;

	/* second */
	Result.nSec = (timeToIncrement.nSec + increment.nSec);

	/* minute */
	Result.nMin = (timeToIncrement.nMin + increment.nMin);
	if(Result.nSec >= NUM_MIN_HR)
	{
		Result.nSec -= NUM_MIN_HR;
		Result.nMin += 1;
	}

	/* hour */
	Result.nHour = (timeToIncrement.nHour + increment.nHour);
	if(Result.nMin >= NUM_MIN_HR)
	{
		Result.nMin -= NUM_MIN_HR;
		Result.nHour += 1;
	}

	/* day */
	Result.nDay = (timeToIncrement.nDay + increment.nDay);
	if(Result.nHour >= NUM_HRS_DAY)
	{
		Result.nHour -= NUM_HRS_DAY;
		Result.nDay += + 1;
	}

	/* month, year */
	Result.nMonth	=	timeToIncrement.nMonth + increment.nMonth;
	Result.nYear	=	timeToIncrement.nYear + increment.nYear;
	
	if(timeToIncrement.nMonth + increment.nMonth > NUM_MONTHS) 
	{
		Result.nMonth -= NUM_MONTHS;
		Result.nYear++;
	}

	/* correct day */
	NumOfDays = LastDayOfMonth(timeToIncrement.nMonth,timeToIncrement.nYear);
	
	if (Result.nDay > NumOfDays) 
	{
		Result.nDay -=	NumOfDays;

		if(Result.nMonth != DEC ) 
			Result.nMonth += 1;
		else  
		{
			Result.nMonth = JAN;
			Result.nYear += 1;
		}
	} 	

	Result.DayIndex = DOW(Result.nYear,Result.nMonth,Result.nDay);

	return Result;
}


/*****************************************************************************
* FUNCTION
*	DecrementTime2
* DESCRIPTION
*	Decrease a reference time by some time.
*	The difference between DecrementTime is the result may not be a "time"
*	Because it's just to get the "difference", the time may be 1 month, 0 day, 12 minutes
* PARAMETERS
*	timeToDecrement	IN			time to be decreased
*	decrement			IN			time to decrease
*	Result				IN/OUT		result of compuatation
* RETURNS	
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void DecrementTime2(MYTIME timeToDecrement,MYTIME decrement,MYTIME *Result)
{
	if(decrement.nSec != 0)
	{
		if(timeToDecrement.nSec >= decrement.nSec)
			Result->nSec = timeToDecrement.nSec - decrement.nSec;
		else
		{
			Result->nSec = NUM_MIN_HR + timeToDecrement.nSec - decrement.nSec;
			decrement.nMin++;
		}
	}
	else
		Result->nSec = timeToDecrement.nSec;
	
	if(decrement.nMin != 0)
	{
		if(timeToDecrement.nMin >= decrement.nMin)
			Result->nMin = timeToDecrement.nMin - decrement.nMin;
		else
		{
			Result->nMin = NUM_MIN_HR + timeToDecrement.nMin - decrement.nMin;
			decrement.nHour++;
		}
	}
	else
		Result->nMin = timeToDecrement.nMin;
	
	if(decrement.nHour != 0)
	{
		if(timeToDecrement.nHour >= decrement.nHour) 
			Result->nHour = timeToDecrement.nHour - decrement.nHour;
		else 
		{
			Result->nHour = NUM_HRS_DAY + timeToDecrement.nHour - decrement.nHour;
			decrement.nDay++;
		}
	}
	else
		Result->nHour = timeToDecrement.nHour;	
	
	if(decrement.nDay != 0)
	{
		if(timeToDecrement.nDay >= decrement.nDay)
			Result->nDay = timeToDecrement.nDay - decrement.nDay;
		else 
		{
			U8 tmp_last_day;
			decrement.nMonth++;
			if(timeToDecrement.nMonth != JAN)
				tmp_last_day=LastDayOfMonth((U8)(timeToDecrement.nMonth - 1), timeToDecrement.nYear);
			else
				tmp_last_day=31;

			if(tmp_last_day<(tmp_last_day + timeToDecrement.nDay - decrement.nDay))
			{
				if(DEC == timeToDecrement.nMonth)
					tmp_last_day+=31;
				else
					tmp_last_day+=LastDayOfMonth((U8)(timeToDecrement.nMonth), timeToDecrement.nYear);
				decrement.nMonth++;
			}

			Result->nDay = tmp_last_day + timeToDecrement.nDay - decrement.nDay;
		}
	}
	else
		Result->nDay = timeToDecrement.nDay;
	
	if(decrement.nMonth != 0)
	{
		if(timeToDecrement.nMonth >= decrement.nMonth)
			Result->nMonth	=	timeToDecrement.nMonth - decrement.nMonth;
		else
		{
			Result->nMonth	= NUM_MONTHS + timeToDecrement.nMonth - decrement.nMonth;
			decrement.nYear++;
		}
	}
	else
		Result->nMonth = timeToDecrement.nMonth;			

	Result->nYear = timeToDecrement.nYear - decrement.nYear;
	
	Result->DayIndex = DOW(Result->nYear,Result->nMonth,Result->nDay);
}


/*****************************************************************************
* FUNCTION
*	DecrementTime
* DESCRIPTION
*   Decrease a reference time by some time
* PARAMETERS
*	timeToDecrement	IN			time to be decreased
*	decrement		IN			time to decrease
*	Result			IN/OUT		result of compuatation
* RETURNS	
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void DecrementTime(MYTIME timeToDecrement,MYTIME decrement,MYTIME *Result)
{
	if(decrement.nSec != 0)
	{
		if(timeToDecrement.nSec >= decrement.nSec)
			Result->nSec = timeToDecrement.nSec - decrement.nSec;
		else
		{
			Result->nSec = NUM_MIN_HR + timeToDecrement.nSec - decrement.nSec;
			decrement.nMin++;
		}
	}
	else
		Result->nSec = timeToDecrement.nSec;
	
	if(decrement.nMin != 0)
	{
		if(timeToDecrement.nMin >= decrement.nMin)
			Result->nMin = timeToDecrement.nMin - decrement.nMin;
		else
		{
			Result->nMin = NUM_MIN_HR + timeToDecrement.nMin - decrement.nMin;
			decrement.nHour++;
		}
	}
	else
		Result->nMin = timeToDecrement.nMin;
	
	if(decrement.nHour != 0)
	{
		if(timeToDecrement.nHour >= decrement.nHour) 
			Result->nHour = timeToDecrement.nHour - decrement.nHour;
		else 
		{
			Result->nHour = NUM_HRS_DAY + timeToDecrement.nHour - decrement.nHour;
			decrement.nDay++;
		}
	}
	else
		Result->nHour = timeToDecrement.nHour;	
	
	if(decrement.nDay != 0)
	{
		if(timeToDecrement.nDay > decrement.nDay)
			Result->nDay = timeToDecrement.nDay - decrement.nDay;
		else 
		{
			decrement.nMonth++;
			if(timeToDecrement.nMonth != JAN)
				Result->nDay	=	LastDayOfMonth((U8)(timeToDecrement.nMonth - 1), timeToDecrement.nYear) + timeToDecrement.nDay - decrement.nDay;
			else
				Result->nDay	=	31	+ timeToDecrement.nDay - decrement.nDay ;
		}
	}
	else
		Result->nDay = timeToDecrement.nDay;
	
	if(decrement.nMonth != 0)
	{
		if(timeToDecrement.nMonth > decrement.nMonth)
			Result->nMonth	=	timeToDecrement.nMonth - decrement.nMonth;
		else
		{
			Result->nMonth	= NUM_MONTHS + timeToDecrement.nMonth - decrement.nMonth;
			decrement.nYear++;
		}
	}
	else
		Result->nMonth = timeToDecrement.nMonth;			

	Result->nYear = timeToDecrement.nYear - decrement.nYear;
	
	Result->DayIndex = DOW(Result->nYear,Result->nMonth,Result->nDay);

}


/*****************************************************************************
* FUNCTION
*	DTGetRTCTime
* DESCRIPTION
*	Get current RTC time of system.
* PARAMETERS
*	t	IN/OUT	variable to store current time.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void DTGetRTCTime(MYTIME *t)
{
#ifdef MMI_ON_HARDWARE_P
	t_rtc rtc_time;
	RTC_GetTime(&rtc_time);

	t->nYear = rtc_time.rtc_year+YEARFORMATE;
	t->nMonth = rtc_time.rtc_mon;		
	t->nDay = rtc_time.rtc_day;
	t->nHour = rtc_time.rtc_hour;
	t->nMin = rtc_time.rtc_min;
	t->nSec = rtc_time.rtc_sec;
	t->DayIndex=DOW(t->nYear,t->nMonth,t->nDay);
#else
	GetDateTime(&(*t));
#endif
}

/*****************************************************************************
* FUNCTION
*	CompareTime
* DESCRIPTION
*   Compare the equality between 2 times.
* PARAMETERS
*	referenceTime	IN	time to be compare in right-hand side
*	compareTime		IN	time to be compare in left-hand side
*	diff			IN	not yet used
* RETURNS	
*	TIME_LESS		if referenceTime < compareTime
*	TIME_EQUAL		if referenceTime = compareTime
*	TIME_GREATER	if referenceTime > compareTime
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
TIME_EQUALITY CompareTime(MYTIME referenceTime,MYTIME compareTime,MYTIME* diff) {

	TIME_EQUALITY	toReturn;

	if(referenceTime.nYear == compareTime.nYear) {
		if(referenceTime.nMonth == compareTime.nMonth) {
			if(referenceTime.nDay == compareTime.nDay) {
				if(referenceTime.nHour == compareTime.nHour) {
					if(referenceTime.nMin == compareTime.nMin){
						if(referenceTime.nSec == compareTime.nSec) {
							toReturn = TIME_EQUAL;
						} else {
							if(referenceTime.nSec < compareTime.nSec)
								toReturn = TIME_LESS;
							else 
								toReturn = TIME_GREATER;
						}
					} else {
						if(referenceTime.nMin < compareTime.nMin)
							toReturn = TIME_LESS;
						else 
							toReturn = TIME_GREATER;
					} 
				}else {
					if(referenceTime.nHour < compareTime.nHour)
						toReturn = TIME_LESS;
					else 
						toReturn = TIME_GREATER;
				} 
			} else {
				if(referenceTime.nDay < compareTime.nDay)
					toReturn = TIME_LESS;
				else 
					toReturn = TIME_GREATER;
			}
		} else {
			if(referenceTime.nMonth < compareTime.nMonth)
				toReturn = TIME_LESS;
			else 
				toReturn = TIME_GREATER;
		} 
	} else {
		if(referenceTime.nYear < compareTime.nYear)
				toReturn = TIME_LESS;
			else 
				toReturn = TIME_GREATER;
	}

	return toReturn;
}

	/*wangrui add CompareTimeNotDate function 20080105   for fixbug 10180*/
/*****************************************************************************
* FUNCTION
*	CompareTime
* DESCRIPTION
*   Compare the equality between 2 times.
* PARAMETERS
*	referenceTime	IN	time to be compare in right-hand side
*	compareTime		IN	time to be compare in left-hand side
*	diff			IN	not yet used
* RETURNS	
*	TIME_LESS		if referenceTime < compareTime
*	TIME_EQUAL		if referenceTime = compareTime
*	TIME_GREATER	if referenceTime > compareTime
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
TIME_EQUALITY CompareTimeNotDate(MYTIME referenceTime,MYTIME compareTime,MYTIME* diff) {

	TIME_EQUALITY	toReturn;

	if(referenceTime.nHour == compareTime.nHour) {
		if(referenceTime.nMin == compareTime.nMin){
			if(referenceTime.nSec == compareTime.nSec) {
						toReturn = TIME_EQUAL;
			}
			else
			{
				if(referenceTime.nSec < compareTime.nSec)
					toReturn = TIME_LESS;
				else 
					toReturn = TIME_GREATER;
			}
			
		}
		else
		{
			if(referenceTime.nMin < compareTime.nMin)
				toReturn = TIME_LESS;
			else 
				toReturn = TIME_GREATER;
		} 
	}
	else 
	{
		if(referenceTime.nHour < compareTime.nHour)
			toReturn = TIME_LESS;
		else 
			toReturn = TIME_GREATER;
	} 

	return toReturn;
}


/*****************************************************************************
* FUNCTION
*	mmi_dt_get_tz
* DESCRIPTION
*   Get current time zone of MMI
* PARAMETERS
*	none.
* RETURNS	
*	time zone of current home city.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
FLOAT mmi_dt_get_tz(void)
{
	#if 0
	//return GetTimeZone(PhnsetGetHomeCity());  //dyj del 20060514
	#else
	return 0.0;
	#endif
}


/*****************************************************************************
* FUNCTION
*	mmi_dt_is_valid
* DESCRIPTION
*   To compute is the date/time is within reasonable range
* PARAMETERS
*	t	time to be computed
* RETURNS	
*	TURE if date/time valid; otherwise, FALSE
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 mmi_dt_is_valid(MYTIME *t)
{
	/* error handling */
	if(t->nYear > 2030 || t->nYear < YEARFORMATE)
		return FALSE;
	
	if(t->nMonth > 12)
		return FALSE;

	if(t->nMonth == 2)
	{
		if(t->nDay > (gDaysInMonth[t->nMonth-1] + IsLeapYear(t->nYear)))
			return FALSE;
	}
	else
	{
		if(t->nDay > gDaysInMonth[t->nMonth-1])
			return FALSE;
	}

	if(t->nHour > 23)
		return FALSE;

	if(t->nMin > 59)
		return FALSE;

	if(t->nSec > 59)
		return FALSE;

	return TRUE;

}

#ifdef MMI_ON_HARDWARE_P
/*****************************************************************************
* FUNCTION
*	mmi_dt_get_tz_string
* DESCRIPTION
*   Get timezone information of home city in +/- xx:xx format
* PARAMETERS
*	result		IN/OUT		string pointer to store the result
* RETURNS	
*	none.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_dt_get_tz_string(S8* result)
{
#if 0 // zrx del 20060612
	S8 tmpBuff[40];
	S16 homeTimeZone = (S16)(GetTimeZone(PhnsetGetHomeCity())*4);

	if(homeTimeZone < 0)
	{
		homeTimeZone= 0 - homeTimeZone;
		sprintf(tmpBuff,"-%02d%02d", (homeTimeZone/4), (homeTimeZone%4)*15);
	}
	else 
	{
		sprintf(tmpBuff,"+%02d%02d", (homeTimeZone/4), (homeTimeZone%4)*15);
	}

//	AnsiiToUnicodeString(result, tmpBuff);
	strcpy(result, tmpBuff);
#endif
}


/*****************************************************************************
* FUNCTION
*	mmi_dt_conv_utc_to_local
* DESCRIPTION
*   compute utc time to local time
* PARAMETERS
*	tz			IN		time zone
*	utc_time	IN		utc time
* RETURNS	
*	result of time
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
t_rtc mmi_dt_conv_utc_to_local(FLOAT tz, t_rtc *utc_time)
{
#if 0// zrx del 20060612
	t_rtc	outcome;
	MYTIME	origin_t;
	MYTIME	result_t;

	float home_tz = GetTimeZone(PhnsetGetHomeCity());
	tz = home_tz - tz;
	
	origin_t.nYear = utc_time->rtc_year+YEARFORMATE;
	origin_t.nMonth = utc_time->rtc_mon;		
	origin_t.nDay = utc_time->rtc_day;
	origin_t.nHour = utc_time->rtc_hour;
	origin_t.nMin = utc_time->rtc_min;
	origin_t.nSec = utc_time->rtc_sec;

	result_t = mmi_dt_utc_to_rtc(tz, origin_t);

	outcome.rtc_year = result_t.nYear - YEARFORMATE;
	outcome.rtc_mon = result_t.nMonth;
	outcome.rtc_day = result_t.nDay;
	outcome.rtc_hour = result_t.nHour;
	outcome.rtc_min = result_t.nMin;
	outcome.rtc_sec = result_t.nSec;
	outcome.rtc_wday = result_t.DayIndex;

	return outcome;
#else
	t_rtc	outcome = {0};

	return outcome;
#endif
}


#endif
/*****************************************************************************
* FUNCTION
*	mmi_conv_tz_to_mytime
* DESCRIPTION
*   contert timezone to MYTIME structure
* PARAMETERS
*	tz		IN		time zone to be converted
* RETURNS	
*	result in MYTIME structure
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
MYTIME mmi_conv_tz_to_mytime(FLOAT tz)
{
#if 0 // zrx del 20060612
	MYTIME result;
	S16 timezone = (S16)(tz*4);

	memset(&result, 0, sizeof(MYTIME));
	result.nHour = (U8)(timezone/4);
	result.nMin = (U8)((timezone%4)*15);

	return result;
#else
	MYTIME	result = {0};
	
	return result;
#endif
}


/*****************************************************************************
* FUNCTION
*	mmi_dt_utc_to_rtc
* DESCRIPTION
*   compute utc time to rtc time
* PARAMETERS
*	tz			IN		time zone
*	utc			IN		utc time
* RETURNS	
*	result of time
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
MYTIME mmi_dt_utc_to_rtc(FLOAT tz, MYTIME utc)
{
#if 0 // zrx del 20060612
	MYTIME	result;
	MYTIME	incTime;
	U8	negative = FALSE;
	
	if(tz < 0)
	{
		negative = TRUE;
		tz = 0 - tz;
	}

	incTime = mmi_conv_tz_to_mytime(tz);

	if(negative)
		DecrementTime(utc, incTime, &result);
	else
		result = IncrementTime(utc, incTime);

	return result;
#else
	MYTIME	result = {0};

	return result;
#endif
}

/*#ifdef MMI_ON_HARDWARE_P*/ /* delete by zhuoxz, 2009-7-30 */
/*****************************************************************************
* FUNCTION
*	mmi_dt_mytime_2_utc_sec
* DESCRIPTION
*	convert MYTIME format of time to seconds after 1970/1/1
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
kal_int32 mmi_dt_mytime_2_utc_sec(MYTIME *currTime)
{
	kal_uint32 		i;
	kal_uint32			no_of_days = 0;
	kal_int32 		     utc_time;
	kal_int32			is_leap;

	if(currTime->nYear < DT_UTC_BASE_YEAR)
		return 0;
	
	/* year */
	for(i=DT_UTC_BASE_YEAR; i< currTime->nYear; i++)
		no_of_days += (365+IsLeapYear(i));

	/* month */
	is_leap = IsLeapYear(currTime->nYear);
	for(i=1; i<currTime->nMonth; i++)
		no_of_days += LastDayOfMonth(i, currTime->nYear);

	/* day */
	no_of_days += (currTime->nDay-1);

	/* sec */
	utc_time = (kal_uint32)no_of_days*DT_SEC_PER_DAY + (kal_uint32)(currTime->nHour*DT_SEC_PER_HOUR + currTime->nMin*DT_SEC_PER_MIN + currTime->nSec);

	return	 utc_time;
}

/*****************************************************************************
* FUNCTION
*	mmi_dt_utc_sec_2_mytime
* DESCRIPTION
*   convert from UTC second to MYTIME struct
* PARAMETERS
*	utc_sec		IN		uct_sec
*	result		IN		result of time
* RETURNS	
*	TRUE if success; otherwise, FALSE.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
kal_uint8 mmi_dt_utc_sec_2_mytime(kal_int32 utc_sec,MYTIME* result)
{
	kal_int32 sec, day;
	kal_uint16 y;
	kal_uint8 m;
	kal_uint16 d;
	kal_uint8 dst;
	S16 error;

	if(utc_sec < 0)
		return KAL_FALSE;

	ReadValue(NVRAM_SETTING_DT_DST, &dst, DS_BYTE, &error);

	if(dst)
		utc_sec += DT_SEC_PER_HOUR;

	/* hour, min, sec */
	/* hour */
	sec = utc_sec%DT_SEC_PER_DAY;
	result->nHour = sec/DT_SEC_PER_HOUR;	

	/* min */
	sec %= DT_SEC_PER_HOUR;
	result->nMin = sec/DT_SEC_PER_MIN;

	/* sec */
	result->nSec = sec%DT_SEC_PER_MIN;
	

	/* year, month, day */
	/* year */
	/* year */	
	day = utc_sec/DT_SEC_PER_DAY;
	for(y=DT_UTC_BASE_YEAR; day>0 ; y++)
	{
		d = (DT_DAY_PER_YEAR+IsLeapYear(y));
		if(day >= d)
			day -= d;
		else
			break;
	}	
	result->nYear = y;

	for(m = 1; m<DT_MONTH_PER_YEAR; m++)
	{	
		d = LastDayOfMonth(m, y);
		if(day >= d)
			day-=d;
		else
			break;
	}

	result->nMonth = m;
	result->nDay = (kal_uint8)(day+1);

	return KAL_TRUE;
}
/*#endif //MMI_ON_HARDWARE_P*/


/*****************************************************************************
* FUNCTION
*	mmi_dt_is_dst
* DESCRIPTION
*   function to get if daylight saving is enabled in MMI
* PARAMETERS
*	None.
* RETURNS	
*	TRUE if success; otherwise, FALSE.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 mmi_dt_is_dst(void)
{
	U8 dst;
	S16 error;

	ReadValue(NVRAM_SETTING_DT_DST, &dst, DS_BYTE, &error);
	return dst;
}

#endif //#ifndef _MMI_DATETIME_C


