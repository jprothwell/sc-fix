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
 *	AlarmFrameworkProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file is defines constant, enum value, structure, and function prototypes used in alarm framework.
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
  *******************************************************************************/

 /**************************************************************

	FILENAME	: AlarmFrameWorkProt.h

  	PURPOSE		: Alarms application 

	REMARKS		: nil
 

 

	DATE		: Jun 16,03

**************************************************************/

#ifndef _COOLSAND_ALARMFRAMEWORKPROT_H
#define _COOLSAND_ALARMFRAMEWORKPROT_H

#include "cswtype.h"
#include "mmi_features.h" 
#include "datetimetype.h"

/* 
** Define
*/
#define	NUM_OF_THM				1
#define	NUM_OF_TDL				10	
#define	NUM_OF_ALM				3
#define	NUM_OF_SPOF				4
#define	NUM_OF_FAC_ALM			1

#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)
#define	NUM_OF_FMSR				4
#else
#define	NUM_OF_FMSR				0
#endif

#define	ALM_NUM_OF_ALAMRS	(NUM_OF_TDL+NUM_OF_THM+NUM_OF_ALM+NUM_OF_SPOF+NUM_OF_FAC_ALM+NUM_OF_FMSR)

#define	ALM_ALL_DAYS			0x7F
#define	ALM_WEEK_DAYS			0x3e	/* 0111110, default day of week */

#define	ALM_SNOOZE_INTERVAL 	4 /*in minutes*/
#define	ALM_EXPIRE_DURATION		30000
#define	ALM_FACTORY_RTC_TIME	3

#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)
#define NVRAM_ALM_QUEUE_SIZE		230
#else
#define NVRAM_ALM_QUEUE_SIZE		190
#endif
#define	NVRAM_ALM_QUEUE_TOTAL		1

#define	NVRAM_ALM_SPOF_DATA_SIZE		250
#define	NVRAM_ALM_SPOF_DATA_TOTAL		2

/* 
** Typedef 
*/
typedef struct
{
	U8	CurrAlmIndex;		/* index of current expired alarm */
	U8	EnabledAlarms;
	U8	IsAlmTonePlaying;
	U8 	IsPwronAlarm;		/* tells if power on is bcos of alarm */ 
	U8	IsExitByTimer;
	U8	IsWaitForPwroff;
	U8	IsPendingAlm;
	U8	IsSpofDisabled;
	U8	IsExpiryDisabled;
	U8	IsATPwrOffAnimation;
	U8	ATPwrCycle;
}alm_framework_context_struct;

typedef struct
{	
	MYTIME	Time;
	U8		Index;
	U8		IsExpired;
}alm_queue_node_struct;

typedef enum	
{
	ALM_SPON_START = 0,
	ALM_THM_START = (NUM_OF_SPOF),
	ALM_TDL_START = (NUM_OF_SPOF+NUM_OF_THM),
	ALM_ALM_START = (NUM_OF_SPOF + NUM_OF_THM + NUM_OF_TDL),
	ALM_FMSR_START = (NUM_OF_SPOF + NUM_OF_THM + NUM_OF_TDL + NUM_OF_ALM),	/*For Schedule FM Record*/
	ALM_SPOF_START = (NUM_OF_SPOF + NUM_OF_THM + NUM_OF_TDL + NUM_OF_ALM + NUM_OF_FMSR),
	ALM_FACTORY	= (ALM_NUM_OF_ALAMRS + NUM_OF_SPOF),
	ALM_PWR_RESET,
	ALM_INDEX_TOTAL
}ALM_INDEX_ENUM;

typedef enum
{
	ALM_TYPE_SPON = 0,
	ALM_TYPE_THEME,
	ALM_TYPE_TDL,
	ALM_TYPE_ALARM,
	ALM_TYPE_FMSR,
	ALM_TYPE_SPOF,
	ALM_TYPE_FACTORY,
	ALM_TYPE_PWR_RESET
}ALM_TYPE_ENUM;

typedef enum 
{
	ONCE,
	EVERYDAY,
	DAYS,
	WEEKLY,
	MONTHLY,
	ALM_FREQ_THM_OFF,
	ALM_FREQ_THM_HOURLY,
	ALM_FREQ_THM_EVERYDAY,
	ALM_FREQ_THM_WEEKLY,
	ALM_FREQ_THM_MONTHLY,	
	ALM_FREQ_THM_SEASONALLY	
}FREQ_ALARM;


/* 
** Extern Global Variable
*/
extern alm_framework_context_struct g_alm_frm_cntx;

/* 
** Extern Global Function
*/
	/* global */
extern void InitAlarmFrameWork(void);
extern void AlmDeInit(void);
extern void AlmInitRTCPwron(void);
extern void PendingAlarmReminder(void);
extern void PendingAlarmInPwronPeriod(void);
extern U8 AlmIsRTCPwron(void);
extern void AlmReInitialize(void);
extern void AlmDisableSPOF(void);
extern void AlmEnableSPOF(void);
extern U8 AlmIsTonePlaying(void);


	/* alarm related files */
extern void AlmSendPowerOn(void);
extern void AlmSendPowerOff(void);
extern void AlmPlayAlarmTone(void);
extern void AlmsStopAlarmTone(void);
extern void AlmExecPwrOffAlarm(void);
extern void AlmExecPwrOnAlarm(void);
extern void AlmSetAlarm(U8 index);
extern void AlmCancelAlarm(U8 index);
extern void AlmGetStringValue(S8* StringVal, U8 Val);


	/* local */
extern void AlmStartAlarmOnHW(MYTIME alarmTime);
extern void AlmStopAlarmOnHW(void);
extern U8 AlmInsertIntoQueue(U8 index, U8 saveflag);
extern U8 AlmRemoveFromQueue(U8 index, U8 saveflag);
extern MYTIME AlmActualTime(MYTIME t, FREQ_ALARM frequency, U8 weekDays, U8 is_tdl);
extern U8 AlmFindNodeInQueue(U8 index);
extern U8 AlmGetType(U8 index);
extern void AlmWriteQueueToNvram(void);
extern void AlmInitQueue(void);
extern U8 AlmGetRepeat(U8 index);
extern void AlmExpiryHandler(void *msg);
extern void AlmExitPreviousAlarm(void);
extern void AlmSetAlarmRSP(void *msg);
extern void AlmHandleAlarmTimeout(void);
extern void AlmHandlePendingTimeout(void);
extern void AlmCheckAlarmState(void);
extern void AlmInitLCDModule(void);
extern void AlmDisableExpiryHandler(void);
extern void AlmEnableExpiryHandler(void);
extern MYTIME AlmGetAlarmTime(U8 index);
extern void AlmATPowerReset(U8 power_off, U8 time);
//qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__
extern U8 AlmGetSnoozeInterval(U8 almIndex);
extern BOOL AlmIsPowerOffMode(void);

#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_ENGINEER_MODE__)  )   
extern void FactorySetAlarm(U8 sec);
#endif /* defined(MMI_ON_HARDWARE_P) && defined(__MMI_ENGINEER_MODE__) */


#endif


