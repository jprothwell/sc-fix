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
 * IdleAppProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Idle screen prototype header
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

/**********************************************************************************
	Filename:		idleapp_prot.h
	Author:
	Date Created:	September-13-2002
	Contains:		Idle screen application
**********************************************************************************/

#ifndef __IDLEAPP_PROT_H__
#define __IDLEAPP_PROT_H__

//Lisen 01252005
U8 IdleScreenEmergencyCallCheckNKeypadLock(KEYBRD_MESSAGE *eventKey);

void EntryIdleScreen(void);
void ExitIdleScreen(void);

void HandleIdleScreenDigitEntry(void);
void HandleIdleScreenCSKHandler(void);
void IdleScreenDigitHandler(void);
void ExitIdleScreenDigitHandler(void);
void IdleDialPadSavePhoneBook(void);
void IdleDialPadCall(void);
void IdleSetKeyPadLockFlag(void);

//U16 AutoKeyPadLockTime(void);
void IdleSetStatusIcon(S16);
void IdleResetStatusIcon(S16);
//void IdleEnterNameScreen(void);
void IdleCallSpeedDialOnLongPress(void);
void IdleAbbrevDialling(void);
void IdleShowScrInvalidLocation(void);

void IdleShowScrNoPhoneNumEntry(void);

void IdleShowScrNoSpeedDialNumSaved(void);

void IdleHandlePoundKeyForKeypadLock(void);
void IdleHandleStarKeyForKeypadLock(void);
void IdleHandleKeypadLockProcess(void);

 
void IdleScreenSaverGroupKeyHandler(void);
void ExitScreenSaver(void);
void EntryIdleSetScreenSaver(void);

void LeavePartialOnScreenSaverIfOk(void);
void EnterPartialOnScreenSaverIfOk(void);
void RedrawPartialOnScreenSaverIfOk(void);
void EntryPartialOnScreenSaver(void);

void IdleToggleProfile(void);
void IdleStopToggleTimer(void);
pBOOL IsChineseSet(void);
pBOOL IsTrChineseSet(void);
pBOOL IsSmChineseSet(void);
U8 SubLCDIdleScreen(void *funcptr);
void ShowIdleAppSubLcd(void);
void EntryIdleAppSubLcdWithErrorMsg(U8* message,U16 message_icon,U8* history_buffer);

void IdleDisableLongPress(void);
void IdleRefreshServiceIndicationArea(void);
void IdleRSKPressOnScreenSaver(void);
U8 IdleIsPowerOnPeriod(void);
void IdleSetLRKey(void);
void RedrawIdleScreen(void);

void ExitSubLCDIdleScr(void);


extern void EntryScrQuickAccessMenuLockKpad(void);
extern void EntryShctInIdle(void);
extern void PopulateCommonCategoryResources(void);

/* screen saver */
extern void mmi_scrsvr_lcd_sleepin_handler(void);
extern void mmi_scrsvr_lcd_sleepout_handler(void);

extern BOOL mmi_is_idle_reached(void);

extern void mmi_idle_restart_keypad_lock_timer(void);
extern void mmi_idle_restart_screensaver_timer(void);
BOOL mmi_idle_is_allow_to_display_song_name(void);
#endif


