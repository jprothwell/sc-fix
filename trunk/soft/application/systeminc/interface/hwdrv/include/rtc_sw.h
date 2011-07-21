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
 *    rtc_sw.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for RTC driver and adaption.
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
#ifndef _RTC_SW_H
#define _RTC_SW_H

#define ONE_SHOT_DIS		0
#define ONE_SHOT_EN		1

/*Y:Year,M:Month,W:DOW,D:DOM,H:Hour,M:Min,S:SEC*/
#define AL_EN_None		               0
#define AL_EN_ChkS                     1
#define AL_EN_ChkMS                    2
#define AL_EN_ChkHMS                   3
#define AL_EN_ChkDHMS                  4
#define AL_EN_ChkWHMS                  5
#define AL_EN_ChkMDHMS                 6
#define AL_EN_ChkYMDHMS                7
#define AL_EN_NoChange                 8
                                       
#define TC_EN_None		               0
#define TC_EN_ChkS                     1
#define TC_EN_ChkM                     2
#define TC_EN_ChkH                     3
#define TC_EN_ChkD                     4
#define TC_EN_ChkW                     5
#define TC_EN_ChkMon                   6
#define TC_EN_ChkY                     7
#define TC_EN_NoChange                 8

#define RTC_BBPU_KEY		               0x4300
                                       
#define RTC_BBPU_ALARM_SW	            0x0001
#if ( (defined(MT6205)) || (defined(MT6208))  )
   #define RTC_BBPU_PWR_POLARITY	      0x0002
#endif    

#if defined (MT6205B) || defined(MT6218B) || defined(MT6219) || (defined(MT6217))|| (defined(MT6228))|| (defined(MT6229)|| (defined(MT6226))|| (defined(MT6227)) )
   #define RTC_BBPU_WRITE_EN           0x0002
   #define RTC_BBPU_PWR_SW	            0x0004
   #define RTC_BBPU_AUTOPDN            0x0008
#endif    

#if(  defined(MT6218) )
   #define RTC_BBPU_PWR_POLARITY       0x0002
   #define RTC_BBPU_PWR_SW	            0x0004
   #define RTC_BBPU_AUTOPDN            0x0008
#endif    
                                       
#if ( defined(MT6205) )                
   #define RTC_BBPU_PWR_SW	            0x0004
   #define RTC_BBPU_PDN_XOSC32K		   0x0010
#endif /*(MT6205)*/
#if ( (defined(MT6208)) )
   #define RTC_BBPU_PDN_XOSC32K		   0x0004
   #define RTC_BBPU_PWR_SW	            0x0010
#endif  

#if ( (defined(MT6205)) || (defined(MT6208)))
   #define RTC_BBPU_POWERON        (RTC_BBPU_PDN_XOSC32K|RTC_BBPU_PWR_SW|RTC_BBPU_PWR_POLARITY|RTC_BBPU_ALARM_SW|RTC_BBPU_KEY)
   #define RTC_BBPU_POWEROFF       (RTC_BBPU_PDN_XOSC32K|RTC_BBPU_PWR_POLARITY|RTC_BBPU_ALARM_SW|RTC_BBPU_KEY)
#endif    

#if defined (MT6205B) || defined(MT6218B) || defined(MT6219) || (defined(MT6217))|| (defined(MT6228))|| (defined(MT6229))|| (defined(MT6226))|| (defined(MT6227)) 
   #define RTC_BBPU_POWERON        (RTC_BBPU_PWR_SW|RTC_BBPU_WRITE_EN|RTC_BBPU_ALARM_SW|RTC_BBPU_AUTOPDN|RTC_BBPU_KEY)
   #define RTC_BBPU_POWEROFF       (RTC_BBPU_WRITE_EN|RTC_BBPU_ALARM_SW|RTC_BBPU_AUTOPDN|RTC_BBPU_KEY)
#endif    
/*0227 TY removes RTC_BBPU_ALARM_SW*/
#if( defined(MT6218))
   #define RTC_BBPU_POWERON        (RTC_BBPU_PWR_SW|RTC_BBPU_PWR_POLARITY|RTC_BBPU_ALARM_SW|RTC_BBPU_AUTOPDN|RTC_BBPU_KEY)
   #define RTC_BBPU_POWEROFF       (RTC_BBPU_PWR_POLARITY|RTC_BBPU_ALARM_SW|RTC_BBPU_AUTOPDN|RTC_BBPU_KEY)
#endif    

typedef struct __rtc 
{
	kal_uint8		rtc_sec;    /* seconds after the minute   - [0,59]  */
	kal_uint8		rtc_min;    /* minutes after the hour     - [0,59]  */
	kal_uint8		rtc_hour;   /* hours after the midnight   - [0,23]  */
	kal_uint8		rtc_day;    /* day of the month           - [1,31]  */
	kal_uint8		rtc_mon;    /* months 		               - [1,12] */
	kal_uint8		rtc_wday;   /* days in a week 		      - [1,7] */
	kal_uint8		rtc_year;   /* years                      - [0,127] */
} t_rtc;

typedef struct 
{
	void (*rtc_tcfunc)(void);
	void (*rtc_alfunc)(void); 
}rtc_callbac;

typedef struct
{
   kal_hisrid hisr;
}rtc_module;

extern void RTC_BBPUOpen(kal_bool open);
extern void RTC_BBPWRUP_init(void);
extern kal_bool isPWROnByRTC(void);
extern void RTC_IRQOneShotEnable(kal_uint8 oneShotEN);
extern void RTC_Config(kal_uint8 AL_EN,kal_uint8 TC_EN);
extern void RTC_InitTC_Time(t_rtc *rtctime);
extern void RTC_GetTime(t_rtc *rtctime);
extern void RTC_SetAlarm(t_rtc *rtctime);
extern void RTC_GetALTime(t_rtc *rtctime);
extern void RTC_LISR(void);
extern kal_bool RTC_isFisrtOn(void);
extern void RTC_setPWRKey(void);
extern void RTC_init(void (*RTC_TCCallback)(void),void (*RTC_ALCallback)(void));
extern kal_bool RTC_is_MS_FirstPowerOn(void);

#endif

