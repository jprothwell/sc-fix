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
 *	DateTimeGprot.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines function prototypes for date/time realted utilities.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: DateTimeGprot.h

  	PURPOSE		: .

 

 

	DATE		: .

**************************************************************/
#ifndef _MMI_DATETIMEGPROT_H
#define _MMI_DATETIMEGPROT_H

#include "mmi_data_types.h"
//#include "datetimedef.h"
#include "datetimetype.h"
#include "alarmdef.h"  //dyj del 20060514
#include "callmanagementstruct.h"

/* 
** Define
*/
#define YEARFORMATE	2000
#define MAX_CM_CALLS	MAX_CALLS

/* 
** Typedef 
*/
typedef void (*DateTimeFuncPtr)(MYTIME *); 

/* 
** Extern Global Variable
*/
extern DateTimeFuncPtr CMDateTimeCallBack[MAX_CM_CALLS];
extern U8 CMCallCount;
extern U8 CMCallBackCount;


/* 
** Extern Global Function
*/
extern U16 FindMaxTime(MYTIME *t1, MYTIME *t2);
extern MYTIME *GetTimeDifference(MYTIME *t1, MYTIME *t2, MYTIME *result);
extern void GetDateTime(MYTIME *t);
extern void SetDateTime(void *t);
extern U8 DOW(U16 year, U8 month, U8 day);
extern U8 LastDayOfMonth(U8 month,U16 year);
extern MYTIME IncrementTime(MYTIME timeToIncrement,MYTIME increment);
extern void DecrementTime(MYTIME timeToDecrement,MYTIME decrement,MYTIME *Result);
extern void DecrementTime2(MYTIME timeToDecrement,MYTIME decrement,MYTIME *Result);
extern TIME_EQUALITY CompareTime(MYTIME referenceTime,MYTIME compareTime,MYTIME* diff);
extern TIME_EQUALITY CompareTimeNotDate(MYTIME referenceTime,MYTIME compareTime,MYTIME* diff);		/*wangrui add 20080105   for fixbug 10180*/
extern void DateTimerIndication(void* p);
extern U16 IsLeapYear(U16 year);
extern void DTGetRTCTime(MYTIME *t);

extern void CMGetExactTime(DateTimeFuncPtr callback);
extern void CMGetExactTimeCB(void *buf);
extern void CMCallBackTimer(void);
extern FLOAT mmi_dt_get_tz(void);
extern U8 mmi_dt_is_valid(MYTIME *t);
extern MYTIME mmi_dt_utc_to_rtc(FLOAT tz, MYTIME utc);
extern MYTIME mmi_conv_tz_to_mytime(FLOAT tz);

#ifdef MMI_ON_HARDWARE_P
kal_int32 mmi_dt_mytime_2_utc_sec(MYTIME *currTime);
kal_uint8 mmi_dt_utc_sec_2_mytime(kal_int32 utc_sec,MYTIME* result);
#endif

#endif // _MMI_DATETIMEGPROT_H


