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
 *	ChatApprot.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Chat App.
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

	FILENAME	: ChatAppprot.h

  	PURPOSE		: Chat application 

 

	 

	DATE		: Feb 18,04

**************************************************************/

#include "mmi_features.h"
#ifdef __MMI_MESSAGES_CHAT__

#ifndef _CHATPROT_H
#define	_CHATPROT_H



void HighlightChatRooM1(void);
void HighlightChatRooM2(void);
void HighlightChatRoomInfo(void);
void HighlightChatStartMenu(void);
void HighlightChatResumeMenu(void);
void HighlightChatEndMenu(void);
void HighlightChatSend(void);
void HighlightChatReply(void);
void HighlightChatUseNumMethod(void);
void HighlightChatClearMethod(void);
void HighlightChatTempLeaveMethod(void);
void HighlightChatQuitMethod(void);
void HighlititeChatSaveOutBox(void);
void HighlightChatTemplate(void);
void HighlightChatPhbNum(void);
void HighlightChatPhbNam(void);
void HighlightChatInputMethod(void);

void EntryChatOption(void);
void EntryChatRoomOption(void);
void EntryChatWrite(void);
void EntryChatWriteOption(void);
void EntryChatRoomInfo(void);
void ExitChatRoomInfo(void);
void FillChatRoomActivationStruct(void);

void PreEntryChatQuitConfirm(void);
void PreSaveChatInfo(void);

void SaveChatRoomInfo(void);
void ClearChatRoom(void);
void EntryChatQuitConfirm(void);
void TempleaveChatApp(void);

void EnterPhoneBookForChatNumberApp(void);
void EnterPhoneBookForChatNameApp(void);
void SetNameNumberForChatScreen(U8*,U8*);
void GoBackHistoryFromChatInputMethod(void);
pBOOL CheckDuplicateNumber(PU8 DestinationNo,S32 index);
pBOOL CheckChatNumber(PU8 DestNum,S8 status,S32 index);
void FillChatRoomInfo(PU8 name,PU8 destnum,S32 index);

void ExitChatConfirm(void);
void ChatInboxInterface(PU8 destnum);
S16 CheckActiveChatRoom(void);
void ReplaceChatRoom(void);
void QuitChatRoom(void);
void EntryClearChatRoomConfirm(void);
void GoBackQuitScreen(void);

void ChatNumberScreenEmpty(void);
void ChatNumberScreenNotEmpty(void);
void ChatInlineHandler(S32 index);

void EntryChatReplaceConfirm(void);
void EndChatRoom(void);
void GoBackChatHistory(void);
void SaveChatMsgToOutBox(void);
S16 GetUCS2CharIndex(PS8 buffer);
void ShowSendingSMSChatProgressScreen(void);
void ExitChatWrite(void);
void GetHeader(PU8 dest,S16 index);
void AddHeadertoMessage(void);
PU8 CopyToBuffer(U8 packet[],S32 length,S16 index, U8 flag);

U16 IsNewChatSmsIndScrn(void);
void EntryNotifyNewChatMessageArrival(void);
void ExitNotifyNewChatMessageArrival(void);
void NotifymessageLeftSoftKeyFunction(void);
void GoBackHistoryFromNewChatMessageScreen(void);
void GoBackHistoryFromNewChatMessageScreenForEndKey(void);
U8 IsthereAnyNewChatMessage(void);
pBOOL NewChatMsgIndPresentForKeypadLock(void);
void DisplayNewChatMsgINDAfterKeypadUnlocked (void);

pBOOL ModifyChatRoom(U16 index, U8 flag, void* data, S8* content);
void DeletenCharFromEMSBuffer(S16 count);
U32 GetChatInboxIndex(void);


#endif
#endif


