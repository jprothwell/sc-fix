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
 * SimDetectionGprot.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Global functions declarations for Sim Detction module
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: SimDetectionGprot.h

  	PURPOSE		: Global functions declarations for Sim 
					Detction module.

 

	AUTHOR		: Deepali

	DATE		: 12/03/03

**************************************************************/

#ifndef _SIMDETECTION_GPROT_H
#define _SIMDETECTION_GPROT_H

#include "mmi_data_types.h"
#include "simdetectiontype.h"
#include "frameworkstruct.h"

#include "mmi_features.h"
void ReadPinQuery(void);
void SendMessageQuerySIMCard(void);
void EnterPINagain(void);
void ValidatePIN(void);
void ValidatePhoneLock(void);
void EntryScrEnterPIN(void);
void EntryMsgIdGetPinRsp(void *in);
void EntryScrnRequestSimInsertion(void);
void SimPasswdReqResponse(void *);
void SimQueryFailureRsp(void*);
void ServiceIndicationStringDisplay(void *);

void EntryScrnSimBlockRsp(void);
void ExitScrPinBlocked(void);

void EntryScrIncorrectPUK(void);
void EntryScrIncorrectPin(void);
void EntryScrEnterPUK(void);
void MakeSOSCall(void);
void EntryScrPinBlocked(void);
void EntryScrEnterNewPINFirstTime(void);
void EntryScrEnterNewPINSecondTime(void);
void EntryScrPinDoNotMatch(void);
void ValidateFirstAndSecondPin(void);
void ShowMakeSOSCallScr(void);
void InitNetWorkBootUpEventsCallBack(void);
void NetWorkAttachedIndication(void *inMsg);
void EntryScrEmergencyCallDialing(void);
void EntryScrEnterPhoneLock(void);
void NetWorkSignalLevelIndication(void*);
void NetWorkServiceTypeIndication(void*);
void ExitShowMakeSOSCallScr(void);
void NumberOfPLMNSuported(void*);
void ChangeRoamingState(UINT8 sim_id);
void RspToTestRoamingEnabled(void *);
void RequestToTestRoamingEnabled(void);
void EntryScrInvalidPUKTransiantScreen(void);
void EntryScrIncorrectPUK(void);
void EntryScrIncorrectPhoneLock(void);

pBOOL IsSOSCall(void);
pBOOL IsSOSPossible(void);
void EntryScrNoNetworkServiceMessage(void);
void EntryScrInvalidPINPUKPLOCKLength(void);
//void CallBackPowerOnAnimationComplete(void);
void ExitLowBatteryScrAtPowerOn(void);
void EntryScrSOSErrorMessage(U16);
void InitSimDetection(void);
void InitSimOptionApp(void);
void EntryScrnSimErrorScreen(void);
void ExitScrnSimErrorScreen(void);
void EntryScrSimOptionMenu(void);
void ShowWelcomeScr(void);

 
#if defined(__MMI_APHORISM__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
void BeforeEntryIdleScr(void);
// CSD end

void ExitScrPUKBlocked(void);

PS8 GetCurrentPLMNName(S8*);

void EntryScrNotSOSNumMessage(void);
//void PopulatePLMNList(void);
//void PopulateAphorismList(void);
void HandleInsertSIMScrDigitEntry(void);
void DialSOSNumber(void);
void ExitDialSOSNumber(void);

void GoToSOSDialScr(void);
void SimECCInfoSave(void*);

void EntryScrCodeVerified(void);
void CodeVerifiedTimeoutFn(void);
void ExitCodeVerifiedScr(void);

pBOOL CheckValidEmergencyNo1(S8*);
pBOOL CheckSIMSOSEntries(S8*);

PS8 GetHomePLMNName(void);
void AddHistoryForBootUp(FuncPtr funcPtr);
void SSCHandleEnterPUK(void);
void ExitSSCHandleEnterPUK(void);
void ValidateSSCStringPUK(void);
void MakeCallSSCStringPUK(void);

void ScrBootUpPasswordaValidation(U8*,U8*,S32);
void PUKLengthValidation(U8*,U8*,S32);


// CSD added, for SIM refresh and abnormal reset 0912
U8 GetAbnormalReset(void);
void SetAbnormalReset(void);
void ResetAbnormalReset(void);
// CSD end

void ExitAphorismScreen(void);
void ExitWelcomeScreen(void);

void RemoveStatusIconsForSimRemoved(void);
void FirstTimeTurnOnBackLight(void);
void DisplayInvalidSimInfo(void);

//micha0120
void UpdateGPRSStatusIcon(U8);
void UpdateGPRSStatusIconExt(U8 gprs_state, U8 nSimID);
void GPRSStatusUpdateIndication(void *info);
void GPRSStatusGetGPRSModeReq(void);
void GPRSStatusGetGPRSModeRsp(void *info);
void GPRSStatusDetachGPRSReq(void);
void GPRSStatusDetachGPRSRsp(void *info);

#ifdef _NETWORK_CIPHER_SUPPORT_
void NetWorkCipherIndication(void *info);
#endif //GSM/GPRS ciphering indication

#ifdef __MANUAL_MODE_NW_SEL__
void PLMNSelectionModeIndication(void *info);
void PLMNGetSelectionModeRsp(void *info);
void PLMNGetSelectionModeReq(void);
#endif

/*****SubLCDFunctions********/
void ShowSubLCDSimError(void);
void ExitSubLCDSimError(void);
void ShowSubLCDBlockedSIM(void);
void ExitSubLCDBlockedSIM(void);
void ShowSubLCDEnterPIN(void);
void ExitSubLCDEnterPIN(void);
void ShowSubLCDEnterPUK(void);
void ExitSubLCDEnterPUK(void);
void ShowSubLCDEnterPhoneLock(void);
void ExitSubLCDEnterPhoneLock(void);

void PowerOnBatteryIndicationCompletePhase2(void);
#if defined(  __FS_CHECKDRIVE_SUPPORT__ )
void PWR_NAND_SanityCheck_response_handler(void);
#endif

pBOOL mmi_bootup_get_active_flight_mode(void);
/***************************/
#endif // _SIMDETECTION_GPROT_H


