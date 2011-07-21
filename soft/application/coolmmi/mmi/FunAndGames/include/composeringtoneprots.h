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
 *	ComposeRingToneProts.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is defines prototypes for Ring Tone Composer application.
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

	FILENAME	: ComposeRingToneProts.h

  	PURPOSE		: Prototypes Funtions for the ComposeRingTone Application

 

 

	DATE		: July-28-2003

**************************************************************/
#ifndef _MMI_COMPRINGTONEPROTOS_H
#define _MMI_COMPRINGTONEPROTOS_H
#include "mmi_features.h"
#if defined(__MMI_RING_COMPOSER__) && defined(__MMI_IMELODY_SUPPORT__)
#include "downloaddefs.h"
/* if some header should be include before this one */

/* 
** Typedef 
*/

/* 
** Define
*/

/* 
** Extern Global Variable
*/

/* 
** Extern Global Function
*/
void InitRingToneComposer(void);
S8 *GetComposedIMelodyNameInFileSystem(U16 AudioId);
U8 GetComposedIMelodyInfo(DYNAUDIOINFO ** downloadAudioInfo);

void HighlightRngcMenu(void);
void EntryRngcApp(void);
void HighlightRngcCompTone(S32 nIndex);
U8 DeleteComposedRingToneByIndex(U16 AudioId);
U8 AddComposedRingTone(S8 *fileName);
void EntryRngcMainMenuOptList(void);
void RngcGotoSelectedOpt(void);
void SetRingToneFilenameBuf(void);
void EntryRngcDeleteToneConfirm(void);
void DeleteCompRingTone(void);
void EntryRngcPlayTone(void);
void ExitRngcPlayTone(void);
void EntryRngcCompToneOptList(void);
void HighlightRngcCompToneOpt(S32 nIndex);

void RngcGotoSelectedCompToneOpt(void);
void EntryRngcPlaySpeed(void);
void HighlightRngcGenericOptHdlr(S32 nIndex);
void SetCurrCompRingToneSpeed(void);
U8 GetIndexCurrCompRingToneSpeed(void);
void EntryRngcInstrument(void);
U8 GetCurrIndexCompRingToneInst(void);
void SetCurrCompRingToneInstrument(void);
void EntryRngcEditFileName( void);
void GetCompRingToneSaveOpt(S32 nIndex);
void RngcDisplayPopupAfterSave(void);
void EntryRngcEditFilenameOptList(void);
S32 SetSaveCompRingTone(void);
void RngcGotoEditFinenameOpt(void);
U16 SearchCompRingToneIndexByFileName(S8* fileName);
void GoBacknHistoryFromInputMethod(void);
void ReadCompRingToneFromNVRAM(void);
void WriteCompRingToneToNVRAM(void);
U8 IsCompToneExisted(PS8 filename);

extern void ExecCurrEndKeyDownHandler (void);
extern void GetCurrEndKeyDownHandler (void);
extern void EntryInputMethodScreen(void);
extern void SetProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID);
extern void ClearProtocolEventHandler(U32 eventID);
//[221] Added by ZXB 2009/02/05								[BEGIN]
// Declare slave protocol event-handler table functions.
#ifdef __MMI_MULTI_SIM__
extern void SetSlaveProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID);
extern void ClearSlaveProtocolEventHandler(U32 eventID);
#endif
//[221] Added by ZXB 2009/02/05								[END]
extern void RegisterInputMethodScreenCloseFunction(void (*f)(void));


extern void EntryRngcComposerEngine(void);
extern void RngcExecAdd(void);


#ifdef MMI_ON_WIN32
extern INT fdelete(U8* fName);
extern INT frename(U8* fName,U8* pName);
#endif

#endif //#if defined(__MMI_RING_COMPOSER__)

#endif // #ifndef _MMI_COMPRINGTONEPROTOS_H


