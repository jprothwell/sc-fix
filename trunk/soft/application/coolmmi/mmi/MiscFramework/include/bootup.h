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
 * BootUp.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for IDs for Boot up applications
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

/*****************************************************************************

	Filename:	BootUp.h

	Purpose:	IDs of BootUp 

 

	Date:		May 22, 2003

	
*******************************************************************************/


#ifndef _POWER_ON_DEF_H
#define _POWER_ON_DEF_H

#include "mmi_data_types.h"


#define CHARGER_CONNECT_SCR_TIMEOUT			1000
#define CHARGER_DISCONNECT_SCR_TIMEOUT		        1000
#define BATTERY_FULL_CHARGE_SCR_TIMEOUT		        1000
#define LOW_BATTERY_SCR_TIMEOUT				1000
 
#define POWER_OFF_CHARGER_BACKLIGHT_TIME	5000
// CSD end

#define POWER_OFF_TIMER_STAGE_1 	4000
#define POWER_OFF_TIMER_STAGE_2 	4000

 
#define LBAT_IDLE_DURCNT_NORMAL        8       /* 8 * 1min = 8min */
#define LBAT_IDLE_DURCNT_PROHIBIT      3       /* 3 * 1min = 3min */
#define LBAT_TALK_DURCNT_NORMAL        60      /* 60 * 1 sec  = 1min  */
#define LBAT_TALK_DURCNT_PROHIBIT      10      /* 10 * 1 sec  = 10sec  */
#define LBAT_CALLDROP_CNT              20      


enum STR_BOOTUP_LIST
{
	STR_CHARGER_POWER_ON			=		CHARGER_BASE+1,
	//STR_ALARM_STOP_MSG,					
	//STR_ALARM_PWRON_LSK,					
//	STR_ALARM_PWRON_RSK,					
	//STR_ALARM_PWRON_MSG,					
//	STR_ALARM_STOP_LSK,
	STR_BATTERY_FULL_CHARGED,
	STR_LOW_BATTERY,
	STR_LOW_BATTERY_MT_PROHIBIT,
	STR_OVER_VOLTAGE_BATTERY,
	STR_OVER_TEMP_BATTERY,
	STR_INVALID_BATTERY,
	STR_OVER_CURRENT_BATTERY,   
	STR_LOW_CURRENT_BATTERY,
	STR_CHARGE_BAD_CONTACT,
	STR_BATTERY_BAD_CONTACT,
	STR_SUBLCD_ABNORMAL_BATTERY,
	STR_CHARGER_CONNECTED,
	STR_CHARGER_REMOVED,
   STR_USB_CHARGER_CONNECTED,
	STR_USB_CHARGER_REMOVED,
	STR_LOW_TEMP_BATTERY,
	STR_CHARGING_TIMEOUT,
	STR_INVALID_CHARGER,
	STR_LOW_BATTERY_SHUTDOWN
};

enum IMG_BOOTUP_LIST
{
	IMG_CHARGER_POWER_ON		=		CHARGER_BASE+1,
	IMG_BATTERY_FULL_CHARGED,
	IMG_SUBLCD_CHARGER_POWER_ON,
	IMG_SUBLCD_CHARGING_COMPLETE,
	IMG_SUBLCD_ABNORMAL_BATTERY
};

enum SCR_BOOTUP__LIST
{
	SCR_CHARGER_POWER_ON		=		CHARGER_BASE+1,	
	SCR_CHARGING_COMPLETE,
	SCR_ABNORMAL_CHARGING,
	SCR_CHARGER_POPUP,
	SCR_LOW_POWEROFF_SCR
};

#endif


