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
 *	SettingGprots.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   
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

	FILENAME	: setting_gprot.h

  	PURPOSE		: Settings application global function prototypes

 

	AUTHOR		: Vanita Jain

	DATE		: 

**************************************************************/

#ifndef _PIXCOM_SETTINGGPROTS_H
#define _PIXCOM_SETTINGGPROTS_H
#include "settinggenum.h"

void InitSettingApp(void);
//micha0530
void EntryScrSettingMenu(void);
MMI_BOOL AutoRedialOnOffStatus(void);
U8 CallTimeReminderStatus(void);
SETTING_AKPL_TIME GetAutoKeypadLockTime(void);
MMI_BOOL IsPhoneLock(void);
MMI_BOOL ReadPhoneLockSetting(void);
MMI_BOOL  IsShowWelcomeScr1(void);
S8* GetWelcomeText(void);
MMI_BOOL IsShowAphorismScr1(void);
MMI_BOOL GetCallTimeDisplay(void);
MMI_BOOL PhnsetGetNITZFuncStatus(void);
void SettingChangePasswdPuk(void);
MMI_BOOL PhnsetGetShowOwnerNumStatus(void);

//Lisen 0921-1
#if defined(__MMI_OPERATOR_NAME_MENU__)
MMI_BOOL PhnsetGetShowOPNStatus(void);
#endif
MMI_BOOL PhnsetGetShowDateTimeStatus(void);
MMI_BOOL SetLanguage(U8 *);
extern void PhnsetSetDTFromAT(MYTIME mytime);
extern S16 PhnsetSetTimeFormat(U8 timeformat) ;
extern S16 PhnsetSetDateFormat(U8 dateformat);
BOOL ATSetHomeCity(U8 CityIndex);
extern void Highlight_ScreenSaver_Menu(void);
pBOOL RstRestorePhoneSetup(void);
void GetSettingOptionsStatus(void);
SETTING_CTR GetCtrStatus(void);
U16 GetCtrTime(void);
MMI_BOOL PhnsetSetWelcomeText(BOOL mode,  U8 dcs, U8 *text );  
void PhnsetGetWelcomeText( S8* buf, U16 buf_len );
void InitSettingBootup(void);
void InitSettingNVRAM(void);
void RstrSettingPasswdScrSoftKeyFunc(U8* text,U8* cursor,S32 length);
extern pBOOL SET_DefaultDateFormat(void);
extern pBOOL SET_DefaultTimeFormat(void);
void GoToAddEditScreen(void);
void PhbUpdateSpeedDial(U8, U16, U8, PU8, PU8);
void PhbDeleteAllSpeedDial(U8 storage,U8 type);
MMI_BOOL PhbRestoreSpeedDial(void);
void SettingInlineCustomFunction (void);
void GetDateTimeSetupNvRam(void);
void PhnsetReadNvmHomeCityIdxNStoreCityName(void);
void PhnsetReinitHomeCity(void);
void PhnsetSendSetTimeReqMessage(void);


void InitSecuritySetupApp(void);
void SECSETVerifyPINRsp(void *info);
void SECSETGetPIN2StatusReq(void);
void SECSETChangePasswordPIN1Rsp(void *info);

void SECSETSetSIMIdforPIN(U8 simid);

void SECSETChangePasswordPIN2Rsp(void *info);
void SECSETGetFdlBdlModeInd(void *info);


#endif


