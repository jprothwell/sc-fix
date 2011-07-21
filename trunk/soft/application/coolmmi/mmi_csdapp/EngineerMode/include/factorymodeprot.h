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
 * FactoryModeProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for factory mode
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

#ifndef _FACTORY_MODE_PROT_H
#define _FACTORY_MODE_PROT_H


 
#include "mmi_features.h" 
//CSD End

#ifdef __MMI_FACTORY_MODE__  

#include "mmi_data_types.h"

extern void InitFactoryMode(void);
extern void InitFactoryModeSetting(void);
extern void FM_SetGpioReq(U8 type, U8 level); 
extern void FM_SendADCStartReq(void);
extern void FM_SendADCStopReq(void);
extern void FM_SendPlayAudioReq(U8 index);
extern void FM_SendStopAudioReq(U8 index);
extern void FM_SendSetAudioModeReq(U8 mode);

extern void FM_SendMsg(U32 msg_id, 
                     void *local_param_ptr, 
                     void *peer_buf_ptr);
//Lisen 0630
extern void EntryFMMenu (void);
extern void FM_ATEntryMenuHdlr(void *inMsg);
extern void ExitFMMenu (void);

extern void FM_PrepareVersionPopUp(UI_string_type *list);
extern void HightlightFMVersionHdlr(void);
extern void EntryFMVersionMenu(void);

//Lisen 0921
extern void HightlightFMVersionCommonHdlr(void);
extern void HightlightFMVersionSwHdlr(void);
extern void HightlightFMVersionHwHdlr(void);
extern void HightlightFMVersionSummaryHdlr(void);
extern void EntryFMVersionSummaryMenu(void);
#if defined(__PROJECT_GALLITE_C01__)

extern void EntryFMVersionSwMenu(void);
extern void EntryFMVersionHwMenu(void);
#endif
extern void HighlightFactoryModeAutoTestHdlr(void);
extern void ExitSWFMMenu (void);

extern void FM_PrepareResourceBinPopUp(UI_string_type *list);
extern void HightlightFMResourceBinHdlr(void);
extern void EntryFMResourceBinMenuHdlr(void);

extern void HightlightFMHintItemHdlr(void);
extern void HightlightFMSwVerHdlr(void);
extern void HightlightFMHwVerHdlr(void);
extern void HightlightFMHwVerHdlr(void);
extern void HightlightFMMelodyVerHdlr(void);
extern void HightlightFMSerialNoHdlr(void);

extern void FM_PrepareLEDList(void);
extern void HightlightFMLEDHdlr(void);
extern void EntryFMLEDMenuHdlr(void);
extern void FM_LEDToggleLSK(void);
extern void FM_ExecuteLEDFunc(void);

extern void HightlightFMLCDHdlr(void);
extern void FM_HandleInitialColor(U8 color_r, U8 color_g, U8 color_b);
extern void FM_HandleColorSetChange(void);
extern void ExitFMColorSetChange(void);
extern void EntryFMColorTest(void);
extern void EntryFMLCDMenuHdlr(void);
extern void ExitFMLCDMenuHdlr(void);
extern void HightlightFMLCDCertainHdlr(void);
extern void FM_StartCertainLCDTest(void);
extern void FM_ATLCDTestMsgHdlr( void *inMsg);


extern void HightlightFMLCDContrastHdlr(void);
extern void EntryFMLCDContrastMenuHdlr(void);

extern void HightlightFMLCDContrastMainHdlr(void);
extern void HightlightFMLCDContrastSubHdlr(void);
extern void FM_ShowLCDContrastScreen(void);
extern void ExitFMLCDContrastMainHdlr(void);
extern void ExitFMLCDContrastSubHdlr(void);
extern void EntryFMLCDContrastSliderHdlr(void);


extern void FM_LCDContrastLevelUp(void);
extern void FM_LCDContrastLevelDown(void);
extern void FM_LCDSetContrastLevel(void);


extern void HightlightFMKeypadHdlr(void);
extern void EntryFMKeypadTest(void);
extern void FM_KeypadCheck(void);
extern void FM_KeypadInit(void);
extern void FM_KeypadSet(U16 key_code);
extern U8 FM_KeypadPass(void);
extern void FM_HandleKeypadTest(void);
extern void FM_HandleKeypadTestKeyDown(void);
extern void FM_ATKeypadTestMsgHdlr(void* inMsg);
extern BOOL FM_IsInKeypadTest(void);

extern void HightlightFMLoudSpkHdlr(void);
extern void EntryFMLoudSpkHdlr(void);

extern void HightlightFMEchoLoopHdlr(void);
extern void EntryFMEchoLoopHdlr(void);

extern void HightlightFMVibratorHdlr(void);
extern void EntryFMVibratorHdlr(void);
extern void FM_HandleVibratorRsp(void);
extern void FM_StopVibrator(void);


extern void HightlightFMADCHdlr(void);
extern void FM_PrepareADCList(void);
extern void EntryFMADCMenu(void);
extern void FM_UpdateADCHdlr();
extern void FM_StopADCMenu(void);
extern void ExitFMADCMenu(void);

extern void HightlightFMReceiverHdlr(void);
extern void EntryFMReceiverHdlr(void);

extern void HightlightFMHeadsetHdlr(void);
extern void EntryFMHeadsetHdlr(void);

extern void HightlightFMChargerHdlr(void);
extern void FM_EntryChargerHdlr(void);

extern void HightlightFMRingToneHdlr(void);
extern void EntryFMRingToneHdlr(void);

extern void HightlightFMRTCHdlr(void);
extern void EntryFMRTCHdlr(void);
extern void ExitFMRTCInitScr(void);
extern void FM_HandleRTCRsp(void);

//Lisen 0804
extern void FM_MTBF(void);
extern void HightlightFMMTBFHdlr(void);
extern void EntryFMMTBF(void);
extern void ExitFMMTBF(void);

extern void HightlightFMMemoryCardHdlr(void);
extern void FM_MemoryCardResponseHandler(void* info);
extern void FM_MemoryCardReq(void);
extern void EntryFMMemoryCardMenu(void);

extern void HightlightFMUARTHdlr(void);
extern void FM_UARTResponseHandler(void* info);
extern void FM_UARTReq(void);
extern void EntryFMUARTMenu(void);

extern void HightlightFMCameraHdlr(void);
#ifdef __MMI_CAMERA__
extern void EntryFMSetCameraParaMenu(void);
extern void ExitFMSetCameraParaMenu(void);
extern void EntryFM_camera_preview_screen(void);
extern void ExitFM_camera_preview_screen(void);
extern void EntryFM_camera_sublcd_screen(void);
extern void ExitFM_camera_sublcd_screen(void);
#endif

extern void HightlightFMNandFlashHdlr(void);
extern void EntryFMNandFlashMenu(void);
extern void ExitFMNandFlashMenu(void);

/*===== Auto Testing ===== */
extern void FM_AutoTest_LED(void);
extern void FM_AutoTest_CLAM(void);
extern void FM_AutoTest_SubLCM(void);
extern void FM_AutoTest_Version(void);
extern void FM_AutoTest_Backlight(void);
extern void FM_AutoTest_LCD(void);
extern void FM_AutoTest_Keypad(void);
extern void FM_AutoTest_Receiver(void);
extern void FM_AutoTest_MIC(void);
extern void FM_AutoTest_Speaker(void);
extern void FM_AutoTest_Headset(void);
//Lisen 01262005
extern void FM_AutoTest_Double_Speaker(void);
extern void FM_AutoTest_CAMERA(void);
extern void FM_AutoTest_MemoryCard(void);
extern void FM_AutoTest_VIB(void);
extern void FM_AutoTest_Battery(void);
extern void FM_AutoTest_Melody(void);
extern void FM_AutoTest_Charger(void);
extern void FM_AutoTest_NAND(void);
extern void FM_AutoTest_UART(void);
extern void FM_ManualTest_RTC(void);

extern void EntryFMMenuAutoTest(void);
extern void ExitFMMenuAutoTest(void);
extern void EntryFMAutoTestReport(void);
extern void EntryFMAutoTestSummary(void);
extern void EntryFMAutoTestResult(void);
extern void EntryFMAutoTestStart(void);
extern void ExitFMAutoTestStart(void);

extern void FM_InitAutoTest(void);

#ifdef __MMI_TOUCH_SCREEN__
extern void HighlightFMParallelTestHdlr(void);
extern void EntryFMPenParallelTest(void);
extern void ExitFMPenParallelTest(void);
extern void FM_ParallelTestEndKeyTimeout(void);
extern void FM_AutoTest_Pen_Parallel_Test(void);

extern void HighlightFMNCrossMTestHdlr(void);
extern void EntryFMPenNCrossMTest(void);
extern void FM_AutoTest_Pen_N_Cross_M_Test(void);
#endif /*__MMI_TOUCH_SCREEN__*/

/* Varible. */


/*******************/
/* Message Handler */
/*******************/
extern void FM_SendMsg(U32 msg_id, void *local_param_ptr, void *peer_buf_ptr);
extern void FM_SendKeyEventReq(U8 status, U8 code);
extern BOOL FM_GetValidMarker(UINT8 *pStatus);
extern void EntryAutoTest(void);


#endif /* __MMI_FACTORY_MODE__ */

#endif /* !_FACTORY_MODE_PROT_H */


