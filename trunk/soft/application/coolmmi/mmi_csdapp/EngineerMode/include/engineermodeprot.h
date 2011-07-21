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
 * EngineerModeProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for engineer mode function prototype declaration
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef _ENGINEER_MODE_PROT_H
#define _ENGINEER_MODE_PROT_H


 
#include "mmi_features.h" 
//CSD End
#ifdef __MMI_ENGINEER_MODE__  


#include "mmi_data_types.h"

#ifdef __TCPIP__ 
#include "soc_api.h"
#include "engineermodesocketdemoapp.h"
#endif

void populateEngineerModeMenu(void);
void InitEngineerMode(void);
void InitEngineerModeSetting(void);
void EntryEngineerModeMenu(void);
void EntryAtEngineerModeMenu(void *inMsg);
void EntryEmGprsMenuNotSupport(void);


void SendEmStartMsg(void);
void SendEmStopMsg(void);
void EntryEmNetworkMenu(void);


void EntryEmAudioMenu(void);
void ExitEmAudioMenu (void);

void EntryEmRingToneMenu(void);
void ExitEmRingToneMenu (void);
void EntryEmNormalModeMenu(void);
void EntryEmLoudSpModeMenu(void);
void EntryEmHeadsetModeMenu(void);
void EntryEmSetVolume(void);

void EntryEmSpeech(void);
void EntryEmMelody(void);
void EntryEmKeyTone(void);
void EntryEmCallTone(void);
void EntryEmSound(void);
void EntryEmMicrophone(void);
void EntryEmSideTone(void);
void EntryEmAudSetEchoParam(void);
void EntryEmAudSetEnergy1(void);
void EntryEmAudSetDelay1(void);
void EntryEmAudSetEnergy2(void);
void EntryEmAudSetDelay2(void);
void EntryEmAudSetMaxSwing(void);
void EmAudSetParam(void);
void EmAudParamUp(void);
void EmAudParamDown(void);
void SendGetAudioParamReq(void);
void EmUpdateAudioParam(U8 *audp);
void EngineerModeGetAudioParamRes(void* inMsg);
void EngineerModeSetAudioParamRes(void* inMsg);

 
void EntryEmSetMode(void);
void EmSetMode(void);
void HighlightEmSetMode(void);
 

 
void EntryEmFIR(void);
void EmSetFIR(void);
void HighlightEmFIR(void);
 

void EntryEmDeviceMenu(void);

void EntryEmSetUartPort(void);
void ExitEmSetUartPort(void);
void EntryEmSetUartMenu(void);
void EntrySetUsbDefAppScr(void);


void SendGetUartReq(void);
void SetHighlightIndex(S32 nIndex);

/* gprs */
void EntryEmGprsMenu(void);
void HighlightEmGprs(void);
void HighlightEmAutoAttach(void);
void HighlightEmActivatePDP(void);
void HighlightEmDeactivatePDP(void);
void HighlightEmSendDataMenu(void);
void HighlightEmSendData(void);
void HighlightEmPDPContext1(void);
void HighlightEmPDPContext2(void);
void HighlightEmPDPContext3(void);
void HighlightEmPDPContext4(void);
void HighlightEmPDPContext5(void);
void HighlightEmPDPContext6(void);
void HighlightEmPDPContext7(void);
void HighlightEmPDPContext8(void);
void HighlightEmPDPContext9(void);
void HighlightEmPDPContext10(void);
void HighlightEmPDPContext11(void);
void HighlightEmPDPContext12(void);
void HighlightEmPDPContext13(void);
void HighlightEmPDPContext14(void);
void HighlightEmPDPContext15(void);
void EntryEmAutoAttach(void);
void EntryEmActivatePDPMenu(void);
void EntryEmDeactivatePDP(void);
void EntryEmSendData(void);
void EntryEmSendDataMenu(void);
void EntryEmPDPContext1(void);
void EntryEmPDPContext2(void);
void EntryEmPDPContext3(void);
void EntryEmPDPContext4(void);
void EntryEmPDPContext5(void);
void EntryEmPDPContext6(void);
void EntryEmPDPContext7(void);
void EntryEmPDPContext8(void);
void EntryEmPDPContext9(void);
void EntryEmPDPContext10(void);
void EntryEmPDPContext11(void);
void EntryEmPDPContext12(void);
void EntryEmPDPContext13(void);
void EntryEmPDPContext14(void);
void EntryEmPDPContext15(void);
void EntryEmBandSelection(void);
void EmAssignBandSelMenuItem( U16 *nStrItemList, U8 *nNumofItem );
void EntryEmMisc(void);
void EntryEmAutoAnswer(void);
void EntryEmHighSpeedSIM(void);
void EntryEmMiscBacklight(void);
void EntryEmSerialPortEnable( void );
void EntryEmAutoReset( void );

//Lisen 012605
void EntryEmMMIDebug(void);
void ExitEmMMIDebug(void);
void EntryEmMemoryDump(void);
void ExitEmMemoryDump(void);

void EntryEmWAP(void);
void EntryEmWAPEnableDebug(void);
void ExitEmWAPEnableDebug(void);
void EntryEmOfflinePage(void);
void ExitEmOfflinePage(void);
void EntryEmUserAgent(void);
void EntryEmAcceptHeader(void);
void EntryEMMMSVersion(void);

void HighlightOnCertainSerialPortMenu( S32 index );
void HighlightEmSerialPortEnable(void);
void SerialPortMenuLSKHandler(void);
void HighlightEmAutoReset(void);

//Lisen 012605
void HighlightEmMMIDebug(void);
void HighlightEmMemoryDump(void);

//Lisen 03092005
void HightlightEMRAMTestHdlr(void);

#if defined(__AMR_SUPPORT__)
void HighlightEmAMR(void);
void EntryEMFAMR(void);
#endif

void HiliteAllTestIdx(S32 hiliteIdx);
U16 GetSelectedTestList(void);

void ReadTestResultFromNVRAM(void);
void WriteTestResultToNVRAM(void);
void ReadTimeFromNVRAM(void);
void WriteCurrentTimeToNVRAM(void);
void ReadSettingsFromNVRAM(void);
void SaveNewItemToNVRAM(void);
void SavePriorityChangeToNVRAM(void);
void SaveRemoveItemToNVRAM(void);


void HiliteAutoTestAddFromList(void);
void EntryScrChangePriority(void);
void HiliteAutoTestChangePriority(void);
void HiliteAutoTestRemove(void);
void HiliteAutoTestCurrSelIdx(S32 hiliteIdx);
void GoBackAutoTestSetting(void);
void EM_InitAutoTest(void);
void HighlightAutoTestSetting(void);

void EntryEmAutoTestSetting(void);

void EntryScrShowAutoTestOptions(void);

void EntryScrEnterItemPriority(void);

void HighlightEmPWRDownCtrl(void);
void EntryEmPWRDownCtrl(void);
void ExitEmPWRDownCtrl(void);


#ifdef __J2ME__
void HighlightEmMiscJ2meTck(void);
#endif

void SendGprsAttachReq(void);
void SendGprsAttachReq(void);
void SendGprsDeactivatePDPReq(void);
void SendGprsSendDataReq(kal_uint32 size);
void SendGprsActivatePDPContext(kal_uint8 pdp_idx );
void EngineerModeGprsAttachReq(void);
void EngineerModeGprsAttachRes(void* inMsg);
void EngineerModeGprsDeactivatePDPReq(void);
void EngineerModeGprsDeactivatePDPRes(void* inMsg);
void EngineerModeGprsSendDataReq(kal_uint32 size);
void EngineerModeGprsSendDataRes(void* inMsg);
void EngineerModeGprsActivatePDPContext(kal_uint8 pdp_idx);
void EngineerModeGprsSetDefinitionRes(void* inMsg);
void EngineerModeGprsSetQosRes(void* inMsg);
void EngineerModeGprsActTestRes(void* inMsg);
void EmSendDataScreenLSKHandler(void);
void HighlightEmSetSleepMode(void);
void EntryEmSetSleepMode(U8 index);
void ExitEmSetSleepMode(void);
void SendSetSleepModeReq(void);



void HighlightEmNetwork(void);


void HighlightEmAudio(void);
void HighlightEmRingTone(void);
void HighlightEmNormalMode(void);
void HighlightEmLoudSpMode(void);
void HighlightEmHeadsetMode(void);
void HighlightEmAudSetEchoParam(void);

void HighlightEmSpeech(void);
void HighlightEmSideTone(void);
void HighlightEmKeyTone(void);
void HighlightEmCallTone(void);
void HighlightEmMelody(void);
void HighlightEmSound(void);
void HighlightEmMicrophone(void);
void HighlightEmLoopBack(void);
void HighlightEmAudSetEnergy1(void);
void HighlightEmAudSetDelay1(void);
void HighlightEmAudSetEnergy2(void);
void HighlightEmAudSetDelay2(void);
void HighlightEmAudSetMaxSwing(void);

#ifdef __MMI_EM_AUDIO_SPEECH_ENHANCEMENT__
extern void mmi_em_sph_enh_main_highlight_hdlr(void);
extern void mmi_em_sph_enh_normal_mode_highlight_hdlr(void);
extern void mmi_em_sph_enh_headset_mode_highlight_hdlr(void);
extern void mmi_em_sph_enh_loudspk_mode_highlight_hdlr(void);
extern void mmi_em_sph_enh_bt_earphone_mode_highlight_hdlr(void);
extern void mmi_em_sph_enh_FM_mode_highlight_hdlr(void);
extern void mmi_em_sph_enh_TV_mode_highlight_hdlr(void);
extern void mmi_em_sph_enh_para_level_list_highlight_hdlr(S32 index);
extern void mmi_em_sph_enh_voice_flow_highlight_hdlr(void);
extern void mmi_em_sph_enh_mucis_flow_highlight_hdlr(void);
extern void mmi_em_sph_enh_side_tone_highlight_hdlr(void);
extern void mmi_em_sph_enh_microphone_highlight_hdlr(void);
extern void mmi_em_sph_enh_digit_gains_highlight_hdlr(void);
extern void mmi_em_sph_enh_analog_gains_highlight_hdlr(void);
extern void mmi_em_sph_enh_microphone_gains_highlight_hdlr(void);
extern void mmi_em_sph_enh_voc_gains_highlight_hdlr(void);
extern void mmi_em_sph_enh_set_defalut_highlight_hdlr(void);
extern void mmi_em_sph_enh_set_default(void);
extern void mmi_em_sph_enh_save_highlight_hdlr(void);
extern void mmi_em_sph_enh_before_save_param_to_flash(void);
#endif


void HighlightEmDevice(void);
void HighlightEmSetUart(void);
void HighlightEmBandSelection(void);

void HighlightEmMisc(void);
void HighlightEmAutoAnswer(void);
void HighlightEmHighSpeedSIM(void);
void HighlightEmMiscBacklight(void);

#ifdef __MMI_TVOUT__
	void HighlightEmTVOut(void);
#endif /* __MMI_TVOUT__ */

#if defined(__MMI_VIDEO__) && defined(ISP_SUPPORT)
	void HighlightVideoHiBitrate(void);
#endif /* __MMI_VIDEO__ && ISP_SUPPORT */

void HighlightEMWAP(void);
void HighlightEmWAPEnableDebug(void);
void HighlightEmWAPOfflinePage(void);
void HighlightEMUserAgent(void);
void HighlightEMAcceptHeader(void);
void HighlightEMMMSVersion(void);

void HighlightAutoTestSetting(void);

void SendSetVolumeReq(void);
void EmAudioVolumeUp(void);
void EmAudioVolumeDown(void);
void EmSetVolume(void);
void SendGetAudioProfileReq(void);
void EmUpdateAudioProfile(U8	*speech,
                     		U8	*keytone,
                     		U8	*melody,
                     		U8	*sound,
                     		U8	*mic,
                     		U8	sidetone);


void EngineerModeStartReq(U32 mod_id, U32 info_req);
void EngineerModeStopReq(U32 mod_id, U32 info_req);
void EngineerModeStatusUpdate(U32 mod_id, U32 info_req);
void EngineerModeStatusIndHdlr(void* inMsg);
void EngineerModeErrorNotice(U16 stringId);
void EngineerModeWarningNotice(U16 stringId);
void EngineerModeSuccessNotice(U16 stringId);
//Lisen 09062005
#ifndef __DYNAMIC_UART_PORT_SWITCHING__
void EngineerModeSuccessRebootNotice(U16 stringId);
#endif
void EngineerModeNoticeText(U8 *string, U16 icon);
void EngineerModeStartRes(void* inMsg);
void EngineerModeStopRes(void* inMsg);
void EngineerModePlayAudioReq(U8 index, U8 type);
void EngineerModeStopAudioReq(U8 index, U8 type);
void EngineerModeStatusInd(void* inMsg);
void EngineerModeSetVolumeReq(U8 volumeType, U8 volumeLevel);
void EngineerModeGetBandReq(void);
void EngineerModeGetBandRsp(void* inMsg);
void EngineerModeSetBandReq(U8 index);
void EngineerModeGetUartReq(U8 ConfApp);
void EngineerModeGetUartRes(void* inMsg);
void EngineerModeSetUartReq(void);
BOOL EmIsPortAllow(U8 port_num );
void EngineerModeSetHighSpeedSIMReq(void);
void EngineerModeSetUartRes(void* inMsg);
void  EngineeringModeUartSendDetachReq(void);
void  EngineerModeDetachUartRes(void* inMsg);
void  EngineeringModeUartSendAttachReq(void);
void  EngineerModeAttachUartRes(void* inMsg);

void EngineerModeGetAudioProfileReq(U8 mode);
void EngineerModeGetAudioProfileRes(void* inMsg);
void EngineerModeSetAudioProfileReq(void *audioProfile);
void EngineerModeSetAudioProfileRes(void* inMsg);
void EngineerModeSetSleepModeReq(U8 index);
void EngineerModeSetSleepModeRes(void* inMsg);
void EngineerModeSetAudioParamReq(void *audioParam);
void EngineerModeGetAudioParamReq(void);
void EngineerModeSetAudioMode(U8 mode);

void EMPWRDownCtrlHighlightHandler(S32 index);
void EMPWRDownCtrlDoneFunc(void) ;
void EntryEmPWRDownCtrl(void) ;
void ExitEmPWRDownCtrl (void);


void idle_screen_show_em_info(void);


void EngineerModeSendMsg(U32 msg_id, 
                     void *local_param_ptr, 
                     void *peer_buf_ptr);
void EngineerModeSendMsg_Ext(U32 msg_id, U16 dest_mod_id, void *local_param_ptr, void *peer_buf_ptr);

void HighlightEMDebugInfo(void);
void EntryEMDebugInfo(void);
void HighlightEMFsTrace(void);
void EntryEMFsTrace(void);
void HighlightEMLastException(void);
void EntryEMLastException(void);

//Lisen 0921
void HighlightEMPowerOnDuration(void);
void EntryEMPowerOnDurationMenu(void);

//Lisen 05032005     Begin
void EntryEmSWDBG(void);
void HighlightEMSWDBG(void);
//Lisen 05032005     End


/* socket test */
#ifdef __TCPIP__ 
void HighlightEmSocket(void);
void HighlightEmDNSQuery(void);
void HighlightEmHTTPGet(void);
void HighlightEmEcho(void);
void HighlightEmDate(void);
void HighlightEmDNSQueryDone(void);
void HighlightEmDNSQueryInputMethod(void);
void HighlightEmHTTPGetDone(void);
void HighlightEmHTTPGetInputMethod(void);
void EntryEmSocketMenu(void);
void EntryEmDNSQuery(void);
void EntryEmDNSOption(void);
void EntryEmHTTPGet(void);
void EntryEmHTTPOption(void);
void EntryEmEcho(void);
void EntryEmDate(void);
void EntryEmSocketInProgress(void);
void EmSocketInProgressAbort(void);
void ExitEmEcho(void);
void ExitEmDate(void);
void EmSocketEchoFillInlineStruct(void);
void EntryEmSocketConfirm(void);
void GoToMainEmSocketScreen(void);
void EmSocketFullLineEditScreen(void);
void EmSocketDateFillInlineStruct(void);
void EmSocketSendReq(void);
void EntryEmSocketResult (void);
void EmSocketShowErrorMsg(void);
void EmSocketRsp (mmi_soc_demo_app_enum app_id_enum, int result,char* response, int response_len);
void EmSocketInitResult(void);
void EmSocketCheckAmPm(char* response);
void EmSocketGoBack2History(void);
#endif /*  __TCPIP__  */

//Lisen 03092005
void EntryEMRAMTestMenu(void);
void HighlightGPRSTest(void);
#if defined(__MMI_TOUCH_SCREEN__)
void HighlightEmPen(void);
void HighlightEmPenStatistics(void);
void HighlightEmPenExit(void);
void EntryEmPenOptions(void);
void EntryEmPenStatisticsList(void);
void EmPenLeaveTest(void);
void EmPenTestClear(void);
#endif /* __MMI_TOUCH_SCREEN__ */
#endif /* __MMI_ENGINEER_MODE__ */

#endif /* _ENGINEER_MODE_PROT_H */


