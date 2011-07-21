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
 *	SATProts.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   SIM application toolkit related functions
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
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/**************************************************************

	FILENAME	: SATProts.h

  	PURPOSE		: SAT application 

 

 

	DATE		: Mar 24,03

**************************************************************/

#ifndef _COOLSAND_SATPROTS_H
#define _COOLSAND_SATPROTS_H
 
void SATMessageWaitTimer(void);
void SATStopWaitTimer(void);
void SATPleaseWaitScreen(U16 string, U16 image);
void PleaseWaitActualScreen(void);
void StartSATTimerNoResponsefromUser(FuncPtr pfunc);
void StopSATTimerNoResponsefromUser(void);
void TemporaryStopSATTimerNoResponsefromUser(void);
void RestartSATTimerNoResponsefromUser(void);

void StartSATTimerHelp(FuncPtr pfunc);
void StopSATTimerHelp(void);
void TemporaryStopSATTimerHelp(void);
void RestartSATTimerHelp(void);

void InitSimAppToolkit(void);
void ExitSATToIdle(void);
void ExitVariables(void);
void SATHistoryHandler(void);
void SATSaveHistory(void);

void SATSetupMenuInd(void * msgPtr);
void GoToSATMainMenu(void);
void SendTerminalResponseSetupMenu(U8 res,U8 *cmdDetail);
void SATSetupMenuRsp(void * msgPtr);
//micha0216
void SATMenuSelectRsp(void *msgPtr);
void ExitSATMainMenu(void);
void SATMenuIndex(S32 index);
void EnvelopeSetupMenuOK(void);
void EnvelopeSetupMenuHelp(void);
void EnvelopeSetupMenu(U8 itemId, U8 isHelpInfoRequested);
void FreeMainMenu(SATSETUPMENU *mainMenu);
void SATCompletePreviousTransaction(void);

void SATSelectItemInd(void *msgPtr);
void SATSelectItemRsp(void *msgPtr);
void FreeSubMenu(void);
void TerminalResponseSelectItemOK(void);
void TerminalResponseSelectItemMoveBack(void);
void TerminalResponseSelectItemHelp(void);
void TerminalResponseSelectItemExpire(void);
void TerminalResponseSelectItem(U8 res, U8 itemId, U8 *cmdDetail);
void GoToSATSubMenu(void);
void SelectItemTimer(void);
void SelectItemWrongData(void);

void SATDisplayTextInd(void *msgPtr);
void SATDisplayTextRsp(void *msgPtr);
void GoToSATDisplayTextScreen(void);
void TerminalResponseDisplayTextOK(void);
void TerminalResponseDisplayTextMoveBack(void);
void TerminalResponseDisplayTextExpire(void);
void TerminalResponseDisplayText(U8 res, U8 *cmdDetail);
void FreeDisplayText(void);
void DisplayTextTimer(void);


void SATGetInputInd(void * msgPtr);
void SATGetInputRsp(void *msgPtr);
void GoToGetInput(void);
void TerminalResponseGetInputMoveBack(void);
void TerminalResponseGetInputHelp(void);
void TerminalResponseGetInputExpire(void);
void TerminalResponseGetInputOK(void);
void TerminalResponseGetInput(U8 res, U8 *cmdDetail);
void FreeGetInput(void);
void GetInputTimer(void);

void SATGetInkeyInd(void * msgPtr);
void SATGetInkeyRsp(void *msgPtr);
void GoToGetInkey(void);
void TerminalResponseGetInkeyMoveBack(void);
void TerminalResponseGetInkeyOK(void);
void TerminalResponseGetInkeyYes(void);
void TerminalResponseGetInkeyNo(void);
void TerminalResponseGetInkeyHelp(void);
void TerminalResponseGetInkeyExpire(void);
void TerminalResponseGetInkey(U8 res, U8 isYesSelected, U8 *cmdDetail);
void FreeGetInkey(void);
void GetInkeyTimer(void);
void SATHiliteYesNo(S32 index);
void TerminalResponseGetInkeyYesNo(void);


void SATCallSetupInd(void *msgPtr);
void SATCallSetupStage1Rsp(void *msgPtr);
void SATCallSetupStage2Rsp(void *msgPtr);
void GoToCallConformationScreen(void);
void TerminalResponseCallSetupExpire(void);
void SATCallSetupTerminalResponseMoveBack(void);
void TerminalResponseCallSetup(U8 res, U8 addInfo,U8 *cmdDetail);
void SATCallSetupStage1Req(void);
void FreeCallSetup(void);
void GoToCallConformationScreen(void);
void SATCallExitCause(U8 cause);
void SATCMCallBackScreen(void);
void SATCMCallBackScreenExitHandler(void);
U8 SATCallRedialStatus(void);

void SATSendSMSInd(void *msgPtr);
void SATSendSMSStage1Rsp(void *msgPtr);
void SATSendSMSStage2Rsp(void *msgPtr);
void TerminalResponseSendSMS(U8 res, U8 cause, U8 *cmdDetail);
void SATSendSMSStage1Req(void);
void GoToSMSConformationScreen(void);
void SATSendSmsTerminalResponseUnable(void);
void SATSendSmsTerminalResponseExpire(void);
void DisplaySMSWindow(void);
void SATEntryScrForInputMethodAndDone(void);
void SATInputMethodAndDoneHiliteHandler(S32 index);
void SATGoBacknHistoryFromInputMethod(void);
void TerminalResponseGetInputOKDone(void);
void FreeSendSms(void);

void SATSendSSInd(void *msgPtr);
void SATSendSSStage1Rsp(void *msgPtr);
void SATSendSSStage2Rsp(void *msgPtr);
void TerminalResponseSendSS(U8 res, U8 len, U8* info, U8 *cmdDetail);
void SATSendSSStage1Req(mmi_sat_send_ss_ind_struct *sendSSInd);
void DisplaySSWindow(void);
void FreeSendSS(void);

void SATSendUSSDInd(void *msgPtr);
void SATSendUSSDStage1Rsp(void *msgPtr);
void SATSendUSSDStage2Rsp(void *msgPtr);
void TerminalResponseSendUSSD(U8 res, U8 len, U8* info, U8 *cmdDetail);
void SATSendUSSDStage1Req(mmi_sat_send_ussd_ind_struct *sendSSInd);
void DisplayUSSDWindow(void);
void FreeSendUSSD(void);

void SATMMIInfoInd(void *msgPtr);
void ExitMMIInfo(void);
void MMIInfoKey(void);

void SATPlayToneInd(void *msgPtr);
void SATPlayToneRsp(void *msgPtr);
void DisplayPlayToneWindow(void);
void SATPlayToneTimer(void);
void TerminalResponsePlayTone(U8 res, U8 *cmdDetail);
void TerminalResponsePlayToneBackward(void);
void TerminalResponsePlayToneOK(void);
void TerminalResponsePlayToneExpire(void);
void FreePlayTone(void);
void ExitPlayTone(void);


void SATSendReadSimFileRsp(void *msgPtr);
void SATSendReadSimFileReq(U16 fileIdx, U16 recordNo, U16 length);
void SendImageDataRecordReq(mmi_smu_read_sim_rsp_struct *satReadSimRes);
void SendImageInstRecordReq(mmi_smu_read_sim_rsp_struct *satReadSimRes);
void SendImageLenRecordReq(U8 iconId);
void SendColorDataCallBack(mmi_smu_read_sim_rsp_struct *satReadSimRes);

void SATSendSSUSSDError(void);
void SATEVDLBrowserTerminationInd(void * msgPtr);
void SATEVDLBrowserTermination(U8 res);
void SATLangNotifyInd(void *msgPtr);
void SATSetLanguage(PU8 Lang);
void TerminalResponseLangNotify(U8 res, U8 *cmdDetail);
void SATLaunchBrowserInd(void *msgPtr);
void GoToLaunchBrowserConformationScreen(void);
void LaunchBrowserConformationOK(void);
void WAPLaunchBrowserRsp (void *msgPtr);
void SATSendLaunchBrowserTerminalResponseExpire(void);
void SATSendLaunchBrowserTerminalResponseUnable(void);
void TerminalResponseLaunchBrowser(U8 res, U8 add_info, U8 *cmdDetail);
void SATLaunchBrowserRsp(void *msgPtr);
void FreeLaunchBrowser(void);

void SATSendDTMFInd(void *msgPtr);
void GoToDTMFConformationScreen(void);
void SATSendDTMFTerminalResponseExpire(void);
void SATSendDTMFTerminalResponseUnable(void);
void SATSendDTMFStage1Req(void);
void DisplayDTMFWindow(void);
void SATSendDTMFAbortReq(void);
void SATSendDTMFStage1Rsp(void *msgPtr);
void TerminalResponseSendDTMF(U8 res, U8 cause, U8 *cmdDetail);
void SATSendDTMFStage2Rsp(void *msgPtr);
void FreeSendDTMF(void);

void SATSetupIdleDisplayInd(void *msgPtr);
void TerminalResponseSetupIdleDisplay(U8 res, U8 *cmdDetail);

void(*SATGetIconCompleteCallBack)(void);

void SATGetIconDataCallBack(void);
void SATClearAllIcons(void);

void SendImageInstRecordReq(mmi_smu_read_sim_rsp_struct *satReadSimRes);
void SendImageDataRecordReq(mmi_smu_read_sim_rsp_struct *satReadSimRes);
void SendImageDataCallBack(mmi_smu_read_sim_rsp_struct *satReadSimRes);
void SATRunATCommandInd(void *msgptr);
void DisplayATCommandScreen(void);
void ExitDisplayATCommandScreen(void);
void DisplayATCommandTimerOver(void);
void SATGenericExitHandler(void) ;
//micha0714
void SATNoFurtherCommandInd(void *msgPtr);
//micha0201
void SATSIMFileChangeInd(void *msgPtr);
void GoToSATRebootScreen(void);
void SATRefreshRebootTimeOut(void);
//micha1016
void GoToSATSetupIdleDisplay(void);

//micha0125
U8 SATScrKeyHandler(KEYBRD_MESSAGE *eventKey);
void SetIsInSATScreen(void);
void ClearIsInSATScreen(void);


#ifdef __SATCE__
void SATOpenChannelInd(void *msgPtr);
void GoToOpenChannelConformationScreen(void);
void SATOpenChannelTerminalResponseExpire(void);
void SATOpenChannelTerminalResponseUnable(void);
void TerminalResponseOpenChannel(U8 res, U8 *cmdDetail);
void SATOpenChannelReq(void);
void DisplayOpenChannelWindow(void);
void SATOpenChannelRes(void *msgPtr);
void FreeOpenChannel(void);
void SATCloseChannelInd(void *msgPtr);
void DisplayCloseChannelScreen(void);
void ExitCloseChannelScreen(void);
void CloseChannelCommandTimerOver(void);
void SATSendDataInd(void *msgPtr);
void GoToSendDataConformationScreen(void);
void SATSendDataTerminalResponseExpire(void);
void SATSendDataTerminalResponseUnable(void);
void TerminalResponseSendData(U8 res, U8 *cmdDetail);
void SATSendDataReq(void);
void DisplaySendDataWindow(void);
void SATSendDataRes(void *msgPtr);
void FreeSendData(void);
void SATRecvDataInd(void *msgPtr);
void GoToRecvDataConformationScreen(void);
void SATRecvDataTerminalResponseExpire(void);
void SATRecvDataTerminalResponseUnable(void);
void TerminalResponseRecvData(U8 res, U8 *cmdDetail);
void SATRecvDataReq(void);
void DisplayRecvDataWindow(void);
void SATRecvDataRes(void *msgPtr);
void FreeRecvData(void);
#endif

 
void mmi_sat_restart_please_wait_timer(void);

extern U16 gCurrLangIndex;
extern void RefreshNwProviderName(U8 *UpdatePlmn, U8 simID);
#ifdef __MMI_CLAMCLOSE_BACK2IDLE__
extern void DisplayIdleScreen(void);
extern BOOL IsClamClose(void);
#endif
#ifdef MMI_ON_HARDWARE_P
extern kal_bool is_test_sim(void);
#endif

//micha0901
extern void MakeHold(void);
void mmi_sat_dualsim_sendmsg(MYQUEUE *Msg);
extern void SendTerminalResponseSetupMenuEx(U8 destId, U8 res, U8* cmdDetail);
#ifdef __MMI_MULTI_SIM__
extern void TerminalResponseSelectItemEx(U8 destId, U8 res, U8 itemId, U8 *cmdDetail);
extern void TerminalResponseDisplayTextEx(U8 destId, U8 res, U8 *cmdDetail);
extern void TerminalResponseGetInputEx(U8 destId, U8 res, U8 *cmdDetail);
extern void TerminalResponseGetInkeyEx(U8 destId, U8 res, U8 isYesSelected, U8 *cmdDetail);
extern void TerminalResponseCallSetupEx(U8 destId, U8 res, U8 addInfo, U8 *cmdDetail);
extern void TerminalResponseSendSMS_2(U8 destId, U8 res, U8 cause, U8 *cmdDetail);
extern void TerminalResponseSendSS_2(U8 destId, U8 res, U8 len, U8 *info, U8 *cmdDetail);
extern void TerminalResponseSendUSSD_2(U8 destId, U8 res, U8 len, U8 *info, U8 *cmdDetail);
#endif

#endif /* _COOLSAND_SATPROTS_H */


