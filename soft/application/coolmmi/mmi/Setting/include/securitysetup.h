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
 *	SecuritySetup.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Security Setup related functions
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

#ifndef _SECURITYSETUP_H
#define _SECURITYSETUP_H

//#define MENU_SECURITY_SETTING_BLACK_LIST  1860

typedef struct {
	U8 HighlightedFDN;
	U8 HighlightedBDN;
	MMI_PHB_ENTRY_STRUCT FDLBDLEntries[MAX_SIM_ENTRIES];
	U8 MaxFDL;
	U8 MaxBDL;
	U8 FDNBDNCount;
	U8 FDNBDNEdit;
	InlineItem FDLBDLInlineDisplay[2];
	U8 FDLBDLName[(MAX_NAME_LEN_FSB_LIST+1)*ENCODING_LENGTH];
	U8 FDLBDLNumber[(MAX_NUMBER_LEN_FSB_LIST+1+1)*ENCODING_LENGTH];
} FDLBDLContext;


/*gaosh Add Start For rstdefault Ver: TBM780_R_V1.0.7_20071129_1740  on 2007-12-7 17:9 */
BOOL PhoneLockRstDefault(void);
/*gaosh Add End  For rstdefault Ver: TBM780_R_V1.0.7_20071129_1740  on 2007-12-7 17:9 */
void HighlightSecurity(void);
void HighlightSIMLock(void);
void HighlightPhoneLock(void);
#ifdef __MMI_AUTO_KEYPAD_LOCK__
void HighlightAutoKeyPadLock(void);
#endif
void HighlightFixedDial(void);
void HighlightFdlMode(void);
void HighlightFdlList(void);
void HighlightBarredDial(void);
void HighlightBdlMode(void);
void HighlightBdlList(void);
void HighlightChangePassword(void);
void HighlightPin1Passwd(void);
void HighlightPin2Passwd(void);
void HighlightPhonePasswd(void);


void HintSimLock(U16 index);
void HintPhoneLock(U16 index);
#ifdef __MMI_AUTO_KEYPAD_LOCK__ 
void HintAutoKeypadLock(U16 index);
#endif
void HintFdl(U16 index);
void HintBdl(U16 index);
void HintFdlMode(U16 index);
void HintBdlMode(U16 index);



void EntrySECSETMain(void);
void EntrySECSETPhoneLock(void);
void ExitSECSETPhoneLock(void);
#ifdef __MMI_AUTO_KEYPAD_LOCK__ 
void EntrySECSETAKPLTime(void);
#endif
void EntrySECSETPIN2(void);
void ExitSECSETPIN2(void);
void EntrySECSETFixDial(void);
void EntrySECSETBarDial(void);
void EntrySECSETChangePassword(void);
void ExitSECSETChangePassword(void);
void EntrySECSETChangePWOldPWD(void);
void ExitSECSETChangePWOldPWD(void);
void EntrySECSETChangePWNewPWD(void);
void ExitSECSETChangePWNewPWD(void);
void EntrySECSETChangePWCfmPWD(void);
void ExitSECSETChangePWCfmPWD(void);
void EntrySECSETPUK1(void);
void ExitSECSETPUK1(void);
void EntrySECSETPUKNewPIN1(void);
void ExitSECSETNewPIN1(void);
void EntrySECSETPUKConfirmPIN1(void);
void ExitSECSETNewPIN1Confirm(void);
void EntrySECSETPUK2(void);
void ExitSECSETPUK2(void);
void EntrySECSETPUK2NewPIN2(void);
void ExitSECSETNewPIN2(void);
void EntrySECSETPUK2ConfirmPIN2(void);
void ExitSECSETNewPIN2Confirm(void);
void EntrySECSETSIMLockPIN1(void);
void ExitSECSETSIMLockPIN1(void);

void SECSETSetSIMLockReq(void);
void SECSETSetSIMLockSuccess(void);
void SECSETSetSIMLockFail(void);
void SECSETSetPhoneLockReq(void);
void SECSETSetPhoneLockSuccess(void);
void SETSETSetPhoneLockFail(void);
void SECSETSMULockRsp(void *info);
void SECSETGetPIN1ForChangeReq(void);
void SECSETGetPIN1ForChangeRsp(void *info);
void SECSETGetPIN2ForChangeReq(void);
void SECSETGetPIN2ForChangeRsp(void *info);
void SECSETGetPIN1StatusReq(void);
void SECSETGetPIN1StatusRsp(void *info);
void SECSETGetPIN2StatusReq(void);
void SECSETGetPIN2StatusRsp(void *info);
void SECSETVerifyPINReq(U8 pin_type, U8* ucs2_pin, U8* ucs2_new_pin, FuncPtr success_callback);
void SECSETVerifyPINRsp(void *info);
void SECSETGetFdlBdlModeReq(void);
void SECSETGetFdlBdlModeRsp(void *info);
void SECSETSetFdlModeReq(void);
void SECSETSetFdlModeRsp(void *info);
void SECSETSetBdlModeReq(void);
void SECSETSetBdlModeRsp(void *info);
void SECSETGetBdlStatusReq(void);
void SECSETGetBdlStatusRsp(void *info);
void SECSETGetFdlStatusReq(void);
void SECSETGetFdlStatusRsp(void *info);
void SECSETChangePasswordPhoneRsp(void *info);
void AutoKeyPadLockTimeHandler(S32 index);
void SendAKPLTimeMessageAndShowScreen(void);
void VerifyPin2(void);
void SendReqToChangePwd(void);
SETTING_AKPL_TIME GetAutoKeypadLockTime(void);
void SettingChangePasswdPuk(void);
void  GoBackToPasswordScr(void);
void DialSoSPUK1(void);
void DialSoSNewPIN1(void);
void DialSoSNewPIN1Conf(void);
void ValidateNewPin1(void);
void ValidateNewPin1Success(void);
void ValidateNewPin2(void);
void EntryFdlList(void);
void EntryBdlList(void);
void ScrSettingPukValidation(U8* text,U8* cursor,S32 length);
void FillCountDownNumber( sim_chv_info_struct	chv_info, U8 nSimID);
void FillCountDownString( U8 TypeOfPass, U8* PassCountdownDisp, U8 nSimID);
void ChangePasswordPIN2SuccessCallBack(void);


extern pBOOL phonebookReady;

extern FDLBDLContext gFDLBDLContext[MMI_SIM_NUMBER];
extern FDLBDLContext *gpFDLBDLContext;

#ifdef __MMI_VIP_FUNCTION__
#include "nvram_user_defs.h"
#include "customer_ps_inc.h"
#include "mmi_msg_context.h"
typedef struct
{
	vip_nvram_data_struct saved_data;
	U8 ConfirmPassword[(MAX_VIP_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH];
	U8 editNumber[(MAX_PB_NUMBER_LENGTH + 1)*ENCODING_LENGTH];
	U8 currHiliteIndex;
	FuncPtr PasswdLenCheckCallBack;
	FuncPtr PasswdSucessCallBack;
	FuncPtr PasswdFailCallBack;
	U8 CallLogIndexMap[MAX_PHB_LN_ENTRY];
	U8 nCallLogSuited;
	U16 PHBIndexMap[MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES*MMI_SIM_NUMBER];
	U16 nPHBSuited;
	U16 SMSIndexMap[MMI_SMS_MAX_MSG_NUM];
	U16 nSMSSuited;
}vip_context_struct;

extern vip_context_struct g_vip_context;
extern BOOL mmi_VIP_is_vip_number(U8* number);
#endif

#endif


