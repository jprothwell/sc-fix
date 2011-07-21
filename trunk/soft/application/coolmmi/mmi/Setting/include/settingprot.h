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
 *	SettingProt.h
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
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/**************************************************************

	FILENAME	: setting_prot.h

  	PURPOSE		: Settings application function prototypes

 

	AUTHOR		: Vanita Jain

	DATE		: 

**************************************************************/

#ifndef _PIXCOM_SETTINGPROT_H
#define _PIXCOM_SETTINGPROT_H

#include "mmi_data_types.h"
#include "gui_data_types.h"
 
#include "mmi_features.h"
//CSD end

void EntryScrSettingMenu(void);

void EntryScrAutoRedial(void);
void HighlightCALLSETAutoRedial(void);

void SetAutoRedialSuccess(void *info);

void SendPHMessageAndShowScreen(void);

void EntryScrSetTime(void);
void ExitScrSetTime(void);
void HighlightSetTime(void);
void SetTimeFailure(void );

void EntryScrSetTimeFormat(void);
void ExitScrSetTimeFormat(void);
void HighlightSetTimeFormat(void);
void HighlightTimeFormat(S32 format);

void EntryScrSetDate(void);
void ExitScrSetDate(void);
void HighlightSetDate(void);
void SetDateFailure(void );

void EntryScrSetDateFormat(void);
void ExitScrSetDateFormat(void);
void HighlightSetDateFormat(void);
void HighlightDateFormat(S32 format);
void SendMessageDateAndShowScreen(void);

void EntryScr24hours(void);
void ExitScr24hours(void);

void EntryScrAmPm(void);
void ExitScrAmPm(void);

void EntryScrDateMonth(void);
void ExitScrDateMonth(void);


void EntryScrDateMonthYear(void);
void ExitScrtDateMonthYear(void);

void EntryScrMonthDateYear(void);
void ExitScrMonthDateYear(void);


void EntryScrDateMonthYear2(void);
void ExitScrDateMonthYear2(void);

void SetTimeFormatSuccess(void *info);
void SetDateFormatSuccess(void *info);
void SetTimeFormatFailure(void);
void SetDateFormatFailure(void);
void GoBack2TheHistory(void);

void EntryWelcomeTextOff(void);

void ExitWelcomeTextDoneHdlr(void);


void InitPhoneSetupApp(void);

void HighlightCALLSETCallTimeDisplay(void);
void EntryCallTimeDisplay(void);



void HighlightSPOF(void);
void HighlightRstScr(void);

void EntryRstCheckPhoneLock(void);
void EntryRstScr(void);
void RstVerifyPhoneLockReq(void);
void RstSendRestoreMsgToWap(void);


#ifdef __MMI_PREFER_INPUT_METHOD__
U16 RstGetPreferedInputMethodDefault(void);
#endif 

U8 RstGetSpeedDialStatusDefault(void);
U8 RstGetDateFormatDefault(void);
U8 RstGetTimeFormatDefault(void);
U8 RstGetContrastLevelSubDefault(void);
U8 RstGetContrastLevelMainDefault(void);	
U8 RstGetGreetingTextStatusDefault(void);
U8 RetGetAphorismTextStatusDefault(void);
U8 RstGetOwnerNumStatusDefault(void);
U8 RstGetShowDTStatusDefault(void); 
U8 RstGetNITZStatusDefault(void);
U8 RstGetScrSaverStatusDefault(void);
U8 RstGetScrSaverWaitingDefault(void);
U16 RstGetScrSaverImgIdDefault(void);
U16 RstGetWallpaperImgIdDefault(void);
#ifdef __MMI_SUB_WALLPAPER__
U16 RstGetWallpaperSubImgIdDefault(void);  
#endif
void RstWapRspHdlr(void *msg);

void RstRestoreDisplayPopup(pBOOL status);
void RstStartRestore(void);
void EntryRstConfirm(void);
void GoBackfromRestoreTimer(void);
void GoBack3NHistory(void);
void ExitConfirmRestoreFactory(void);
void SECSETGetPIN1StatusReq(void);
void HighlightCALLSETClosedUserGroup(void);
void HighlightCALLSETCallTimeReminder(void);
void CtrHandler(S32);
void EntryCtrTimeDone(void);
void ExitCtrTimeDone(void);
void SendCtrTime(void);
void SendCtr(void);
void HighlightAphorismText(void);
void EntryAphorismText(void);
void SendAphorismOnOff(void);
void ExitScrWaitForResponse(void);
void SECSETGetFdlBdlModeReq(void);
void InitManageCalls(void);
void InitFixedDialList(void);

#endif


