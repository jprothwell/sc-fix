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
 *  VoiceMemoProt.h
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
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _VOICE_MEMO_PROT_H
#define _VOICE_MEMO_PROT_H
#include "mmi_data_types.h"

#ifdef __MMI_VOICE_MEMO__
void populateVoiceMemoMenu(void);
void InitVoiceMemoApp(void);
void EntryVoiceMemoInCall(void);
void EntryVoiceMemoIdle(void);
void EntryVoiceMemoMenu(void);
void HighlightVoiceMemo(void);
void GetVoiceMemoIndex (S32 index);
void ExitScrVoiceMemoList (void);
void EntryVoiceMemoList (U16   no_of_vm,
   UI_string_type    file_name_list,
   BOOL    more_entry,
   U32  free_space);
void EntryVoiceMemoRecord(void);
void HighlightVoiceMemoRecord(void);
void EntryVoiceMemoPlay(void);
void HighlightVoiceMemoPlay(void);
void EntryVoiceMemoAppend(void);
void HighlightVoiceMemoAppend(void);
void EntryVoiceMemoDelete(void);
void ExitVoiceMemoDelete(void);
void HighlightVoiceMemoDelete(void);
void EntryVoiceMemoDeleteAll(void);
void ExitVoiceMemoDeleteAll(void);
void HighlightVoiceMemoDeleteAll(void);
void EntryVoiceMemoOptionSave(void);
void ExitVoiceMemoOptionSave(void);
void HighlightVoiceMemoOptionSave(void);
void HighlightVoiceMemoOptionInputMethod(void);
void EntryVoiceMemoRenameOption (void);
void ExitVoiceMemoRenameOption(void);
void EntryVoiceMemoRename (void);
void ExitVoiceMemoRename (void);
void HighlightVoiceMemoRename(void);
void KbCBackRecordResume(void);
void KbCBackRecordPause(void);
void KbCBackRecordStop(void);
void KbCBackPlayResume(void);
void KbCBackPlayPause(void);
void KbCBackPlayStop(void);
void KbCBackAppendResume(void);
void KbCBackAppendPause(void);
void KbCBackAppendStop(void);
void VoiceMemoRecordMap(U32 index);
void VoiceMemoDummyFunction(void);
void VoiceMemoResetEventHandler(void);
void ExitVoiceMemo(void);
void EntryVoiceMemoRecordRename (void);
void ExitVoiceMemoRecordRename (void);
void EntryVoiceMemoAppendRename (void);
void ExitVoiceMemoAppendRename (void);
void EntryVoiceMemoAbortQuery (void);
void ExitVoiceMemoAbortQuery(void);



void SendVoiceMemoGetInfoReq(void);
void SendVoiceMemoRecordReq(void);
void SendVoiceMemoPlayReq(void);
void SendVoiceMemoRecordResumeReq(void);
void SendVoiceMemoPlayResumeReq(void);
void SendVoiceMemoRecordPauseReq(void);
void SendVoiceMemoPlayPauseReq(void);
void SendVoiceMemoRecordStopReq(void);
void SendVoiceMemoPlayStopReq(void);
void SendVoiceMemoAbortReq(void);
void SendVoiceMemoAppendReq(void);
void SendVoiceMemoDeleteMsg (void);
void SendVoiceMemoDeleteAllMsg (void);
void SendVoiceMemoRenameMsg (void);

void VoiceMemoExceptionHandler(U8 cause);
void VoiceMemoDeleteRecord(U16 index);

void GoBackHistory2(void);
void GoBackHistory1(void);
void DeleteVoiceMemoHistoryNodes (U16 screenId);
void DeleteVoiceMemoHistoryNodesG (void);
void SetVoiceMemoDeleteHistoryNodes(U16 screenId);
void VoiceMemoSaveFileName(UI_string_type file_name);



void VoiceMemoGetInfoRes(void* inMsg);
void VoiceMemoRecordRes(void* inMsg);
void VoiceMemoPlayRes(void* inMsg);
void VoiceMemoStopRes(void* inMsg);
void VoiceMemoDeleteRes(void* inMsg);
void VoiceMemoAbortRes(void* inMsg);
void VoiceMemoPauseRes(void* inMsg);
void VoiceMemoResumeRes(void* inMsg);
void VoiceMemoAppendRes(void* inMsg);
void VoiceMemoRenameRes(void* inMsg);
void VoiceMemoRecordFinishInd(void* inMsg);
void VoiceMemoPlayFinishInd(void* inMsg);


void VoiceMemoRecordReq(U8 dir);
void VoiceMemoPauseReq(void);
void VoiceMemoStopReq(void);
void VoiceMemoAbortReq(void);
void VoiceMemoResumeReq(void);
void VoiceMemoAppendReq(UI_string_type filename);
void VoiceMemoPlayReq(UI_string_type filename);
void VoiceMemoDeleteReq(UI_string_type filename);
void VoiceMemoDeleteAllReq(void);
void VoiceMemoRenameReq(UI_string_type old, UI_string_type new);
void VoiceMemoGetInfoReq(void);



void VoiceMemoSendMsg(U16 msg_id, 
                     void *local_param_ptr, 
                     void *peer_buf_ptr);

#ifdef VOICE_RECOGNITION_TEST
void VRSampleStopRes(void* inMsg);
void VRSampleRecordRes(void* inMsg);
void VRSampleRecordFinishInd(void* inMsg);
void VRSampleDeleteRes(void* inMsg);
void VRSampleGetInfoRes(void* inMsg);
kal_bool VRSampleRecordReq(U8 dir);
#endif /*VOICE_RECOGNITION_TEST*/

#endif /* VM_SUPPORT */

#endif /* _VOICE_MEMO_PROT_H */


