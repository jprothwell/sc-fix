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
 * PowerOnChargerProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for defination for charging application
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

/**********************************************************************************
	Filename:		PowerOnChargerProt.h
	Author:
	Date Created:	September-13-2002
	Contains:		Idle screen application
**********************************************************************************/

#ifndef _POWERONCHARGER_PROT_H
#define _POWERONCHARGER_PROT_H

//extern U8 gLowBatteryTxProhibitFlag;
//extern  U8  gLowBatteryCutOffCallCnt;

#define LBAT_SHUTDOWN_SCR_TIMER_DUR 5000

void PowerOffRsp(void* );
void BatteryStatusRsp(void* );

void InitChargerPwrOn(void);
void PopulateResData(void);
void *PopulateResGetBaseIDTable(void);
void InitUnicodeSupport(void);
void BatteryIndicationPopup(U16 stringId);
void IdleScreenChargerConnected(U8 playtone);
void IdleScreenChargerDisconnected(void);
void ChargerConnectScrTimeout(void);
void ChargerDisConnectScrTimeout(void);
void IdleScreenBatteryFullCharged(void);
void BatteryFullChargedScrTimeout(void);
void BatteryStatusIndication(U8);
void BatteryStatusLowBatteryHandler(U8);  
void ChargingPwnOnThanPwnOn(void);
void ExitScrChargerConnected(void);
void ExitScrChargerDisconnected(void);
void ExitScrBatteryFullCharged(void);
void ShowChargingScr(void);
void ExitChargingScr(void);
void ShowChargingCompleteScr(void);
void ExitChargerPopUpScr(void);
void ExitAbnormalBatteryIndPopup(void);
void ChargingAbnormalHandler( U16 stringId );  
void LowBatteryShutDownScrTimeout(void);
void ChgrPlayLowBatTone(void);   

void AbnormalBatteryIndPopup(U16);

void ShowSubLCDAbnormalPopup(void);
void ShowSubLCDChargingCompleteScr(void);
void ExitSubLCDChargingCompleteScr(void);
void ShowSubLCDChargingScr(void);
void ExitSubLCDChargingScr(void);
void ShowLowPowerShutdownScr(void);
void ChargerDetectTimeoutHdlr(void);

 
void SET_LOW_BATTERY_ALARM_THRESHOLD(U8 cnt);
BOOL IS_LOW_BATTERY(void);
BOOL CHECK_MO_ALLOW(void);
void RESET_LOW_BATTERY_ALARM_CNT(void);

#endif


