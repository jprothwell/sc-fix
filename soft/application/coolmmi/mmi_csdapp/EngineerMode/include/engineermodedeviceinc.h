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
 * EngineerModeDeviceInc.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for engineer mode device port interface 
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
 
#ifndef _ENGINEER_MODE_DEVICE_H
#define _ENGINEER_MODE_DEVICE_H

 
#include "mmi_features.h" 
//CSD End

#ifdef __MMI_ENGINEER_MODE__  

#include "mmi_data_types.h"


/* ===== Defination ============*/
#define EM_HW_LEVEL_ACT_GET 0
#define EM_HW_LEVEL_ACT_SET 1

#define SET_GPIO_ACTION  0
#define SET_GPO_ACTION  1



/* ===== Type Define ============*/
#ifdef MMI_ON_HARDWARE_P
typedef struct _LCDSET {
   CUSTOM_EM_LCD_TYPE Type;
   CUSTOM_EM_LCD_FUNC_TYPE Func;
   U8 Count;
   U8 ParamBuf[8];
   custom_em_lcd_param_set_struct Param;
} LCDSET;

typedef struct _PWMSET {
   U8 Count;
   CUSTOM_EM_PWM_TYPE Type;
   custom_em_pwm_param_set_struct Param;
} PWMSET;
#endif


/* ===== Function Declaration ============*/
void EntryEmLCDMenu(void);
void HighlightEmLCD(void);

void HighlightEmLCDSub(void);
void EntryEmLCDSubMenu(void);

void HighlightEmLCDMain(void);
void EntryEmLCDMainMenu(void);

void LSKEmLCDSetLCDHandler(void);
void EntryEmLCDSetLCDNextParam(void);

void HighlightEmLCDSetContrast(void);
void EntryEmLCDSetContrast(void);

void HighlightEmLCDSetBias(void);
void EntryEmLCDSetBias(void);

void HighlightEmLCDSetLineRate(void);
void EntryEmLCDSetLineRate(void);

void HighlightEmLCDSetTemperature(void);
void EnrtyEmLCDSetTemperature(void);

void HighlightEmGPIO(void);
void EntryEmGPIOMenu(void);

#ifdef CAMERA_MODULE
void HighlightEmExtCamera(void);
void EntryEmExtCameraMenu(void);

void HighlightEmExtCameraWrite(void);
void EntryEmExtCameraWriteMenu(void);
void ExitEmExtCameraWriteMenu (void);

void HighlightEmExtCameraRead(void);
void EntryEmExtCameraReadMenu(void);
void ExitEmExtCameraReadMenu (void);

#endif

void HighlightEmGPIOList(void);
void EntryEmGPIOListMenu(void);
void LSKEmGPIOListMenu(void);

void HighlightEmGPIOSet(void);
void HighlightEmGPOSet(void);
void EntryEmGPIOSet(void);
void LSKEmGPIOSetCertain(void);

void ExitEMGPIODetail (void);
void EMGPIOHighlightHandler(S32 index);
void EMGPIODoneFunc(void) ;
void EntryEMGPIODetail(void) ;

void ExitEMPWMDetail (void);
void EMPWMHighlightHandler(S32 index);
void EMPWMDoneFunc(void) ;
void EntryEMPWMDetail(void) ;


void HighlightEmPWM(void);
void EntryEmPWMMenu(void);
void LSKEmPWMMenu(void);
void EntryEmPWMSetParam(void);
void LSKEmPWMStop(void);

void HighlightEmEINT(void);
void EmPrepareEINTList(void);
void EntryEmEINTMenu(void);
void TestUpdateEmEINT(void);
void UpdateEmEINT(void);
void ExitEmEINTMenu(void);
void StopEmEINTMeun(void);
void UpdateEmEINTBatteryRecvHandler(void *inMsg);
void UpdateEmEINTGPIORecvHandler(void *inMsg);


void HighlightEmADC(void);
void EmPrepareADCList(void);
void EntryEmADCMenu(void);
void TestUpdateEmADC(void);
void UpdateEmADC(void* inMsg);
void ExitEmADCMenu(void);
void StopEmADCMenu(void);
void SendEmADCStartReq(void);   
void SendEmADCStopReq(void);

#if defined(NAND_SUPPORT) //&& defined(MMI_ON_HARDWARE_P) 
void HighlightEmNANDFormat(void);
void EntryEmNANDFormatMenu(void);
#endif

void HighlightEmClam(void);
void EntryClamMenu(void);

void HighlightEmHWLevel(void);
void EntryHWLevelMenu(void);
void PrepareHWLevelList(void);
void GetHightlightAndEnterHWList(void);
void EntryHWLevelDisplyDetail(void);
void EntryChangeHWLevel(void);
void EmHWLevelInfoAccess( U16 Type, U8 Item, U8 action, S32 *value );
void ApplyHWLevel(void);
void CheckHWLevelInfoDirty(void);
void SendSetHWLevelInfoReq(void);
void SendSetHWLevelInfoRsp(void* inMsg);

#if defined( __MMI_BT_SUPPORT__)
void InitEngineerModeBT(void);
void HighlightEmBTMenu(void);
void EntryEmBTMenu(void);
extern void HighlightEmBTGeneralTestMenu(void);
extern void EntryEmBTGeneralTestMenu(void);
void EntryEmBTTSMode(void);
void HighlightEmBTENTTSMenu(void);
void HighlightEmBTAddSCOLnk(void);
void HighlightEmBTRmvSCOLnk(void);
void HintEmBTENTTSMenu( U16 index );
extern void HighlightEmBTLoopback(void);
#if defined(__MMI_BT_COOLSAND_SUPPORT__)
extern void HighlightEmBTExitTestMode(void);
extern void EMExitBTTestMode(void);
#endif
extern void HighlightEmBTAVRCPCmdList(void);
extern void ExitEMBTLoopback(void);
extern void EntryEmBTLoopback(void);
extern void EMBTLoopbackReq(void);
extern void EntryEmBTInProgress(void);
extern void EmBTInProgressAbort(void);
void EntryEmBTGetFWVersion(void);
void EntryEmBTAddSCOLnk(void);
void EntryEmBTRmvSCOLnk(void);
void HighlightEmBTGetFWVersionMenu(void);
void HintEmBTGetFWVersionMenu( U16 index );

extern void EntryEmBTTestMode(void);
extern void HighlightEmBTTestMenu(void);
extern void EntryEmBTDebugTrace(void);
extern void HighlightEmBTDebugTrace(void);
extern void EntryEmBTAddAudioConnection(void);
extern void HighlightEmBTAddAudioConnection(void);
extern void EntryEmBTRemoveAudioConnection(void);
extern void HighlightEmBTRemoveAudioConnection(void);


#ifdef __MMI_EM_BT_RF_TEST__
     #ifdef __BT_RDA5868__

extern void EntryEmDevBtRfVCO(void);
extern void EntryEmDevBtRfSingleHop(void);
extern void EntryEmDevBtRfCMU200(void);
extern void EntryEmDevBtRfC3000(void);
extern void EntryEmDevBtRf4010(void);

extern void HighlightEmDevBtRfVCO(void);
extern void HighlightEmDevBtRfSingleHop(void);
extern void HighlightEmDevBtRfCMU200(void);
extern void HighlightEmDevBtRfTC3000(void);
extern void HighlightEmDevBtRfN4010(void);
#endif
extern void HighlightEmDevBtRfTest(void);
extern void EntryEmDevBtRfTestConfirm(void);
extern void EmBtRfTestDisableSleepMode(void);
extern void EmBtRfTestSleepModeHdlr(void* inMsg);
extern void EmBtRfTestEnterFlightMode(void);
extern void EmBtRfTestPowerOnBT(void);
extern void EmBtRfTestPowerOnBTRspHdlr(void *inMsg);

extern void EntryEmDevBtRfTest(void);
extern void EmBtRfTestExit(void);

extern void HighlightEmDevBtRfTX(void);
extern void HighlightEmDevBtRfRX(void);
extern void EntryEmDevBtRfTXRX(void);
extern void HighlightHandlerEmBtRFTestTxRxnline(S32 index) ;
extern void HighlightHandlerBtRfPacketTypeInline(S32 index) ;
extern void HighlightHandlerBtRfTxRxHopingInline(S32 index) ;
extern void EMBtRfTestTXRXSendReq(void);
extern void EMBtRfTestSendReq(void);
extern MMI_BOOL EmBtRfTestCheckTXRXValue(void);
extern void EmBtRfTestTXRXRspHdlr(void *inMsg);
extern void EmBtRfTestPwrCtrlLevelRspHdlr(void *inMsg);
extern void EntryEmDevBtRfTesting(void);
extern void EmBtRfTestPwrLevelUp(void);
extern void EmBtRfTestPwrLevelDown(void);
extern void ExitEmDevBtRfTXRX(void);

extern void HighlightEmDevBtRfBdAddr(void);
extern void EntryEmDevBtRfBdAddr(void);
extern void ExitEmDevBtRfBdAddr(void);
extern void EmBtRfTestSetBdAddrRspHdlr(void *inMsg);

extern void HighlightEmDevBtRfBdEnterTestMode(void);

#endif /* __MMI_EM_BT_RF_TEST__ */
#endif

//#ifdef __MMI_BT_COOLSAND_SUPPORT__
//#define __MMI_EM_BT_GET_CHIP_VERION__
//#endif /* __MMI_BT_SUPPORT__ */
#ifdef __MMI_EM_BT_GET_CHIP_VERION__
extern void HighlightEmBTGetChipVersionMenu(void);
extern void EmDevBtChipVersion(void) ;
extern void EmBtGetChipVersionRspHdlr(void *inMsg);
#endif


#endif /* __MMI_ENGINEER_MODE__ */

#endif /* _ENGINEER_MODE_DEVICE_H */


