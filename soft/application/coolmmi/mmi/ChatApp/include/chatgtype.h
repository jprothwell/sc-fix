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
 *	ChatGtype.h
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: ChatGtype.h

  	PURPOSE		: Chat application 

 

	 

	DATE		: March 1,04

**************************************************************/

#include "mmi_features.h"
#ifdef __MMI_MESSAGES_CHAT__

#ifndef _CHATGTYPE_H
#define	_CHATGTYPE_H
#include "wgui_categories.h"
#include "phonebooktypes.h"

//#include "mmi_data_types.h"
#define MAX_NO_OF_CHAT_ROOM 2
#define DISABLE 0
#define ENABLE 1
#define MMI_MSG_CHAT_ENABLE 1
#define MAX_MSG_SIZE   256

/*if needs to modify this define ,remember to update the define in gui_inputs.c */
#define MAX_MSG_SIZE_UCS2   256

#define MAX_NO_OF_MSG_IN_CHAT_ROOM 100
#define MAX_NICKNAME_LENGTH 8
#define MAX_PHONE_NUMBER_LENGTH 20
#define MAX_SMS_CHAT_HEADER_SIZE	12
#define MAX_PHONE_NUMBER_OR_NAME_LENGTH	((MAX_PB_NAME_LENGTH) > (MAX_PB_NUMBER_LENGTH)?(MAX_PB_NAME_LENGTH):(MAX_PB_NUMBER_LENGTH))
 
#define MAX_BUFFER_SIZE 2048
#define BLANKSTR ''

//Deepali
#define CHAT_PHB_NAME			0
#define CHAT_PHB_NUMBER			1
#define CHAT_PHB_NUMBER_CHATROOMINFO 2

typedef struct{
	S8 nickName[(MAX_NICKNAME_LENGTH +1)*ENCODING_LENGTH];
	U8 destinationNo[(MAX_PHONE_NUMBER_LENGTH +2)*ENCODING_LENGTH];
}chat_info_struct;

typedef struct {
	S8 nickName[(MAX_NICKNAME_LENGTH +1)*ENCODING_LENGTH];
	U8 destinationNo[(MAX_PHONE_NUMBER_LENGTH +2)];
}chat_info_nvram_struct;

typedef struct {
	chat_info_struct chatInfo;
	S8 status;
	S16 chatRoomCounter;
	//PS8 pointerToBuffer[MAX_NO_OF_MSG_IN_CHAT_ROOM];
	chat_message_struct chatStrings[MAX_NO_OF_MSG_IN_CHAT_ROOM];
}chat_room_info_struct;


typedef struct
{
	chat_room_info_struct chatRoomInfo[MAX_NO_OF_CHAT_ROOM];
	pBOOL gChatInvitation;
	pBOOL blinkChatFlag[MAX_NO_OF_CHAT_ROOM];	
	U8 writeChatBuffer[(MAX_MSG_SIZE+1)*ENCODING_LENGTH];
}chat_context_struct;




#ifndef NVRAM_CHAT_ROOM_INFO_RECORD_SIZE
#define NVRAM_CHAT_ROOM_INFO_RECORD_SIZE 40
#endif
#ifndef NVRAM_CHAT_ROOM_INFO_RECORD_TOTAL
#define NVRAM_CHAT_ROOM_INFO_RECORD_TOTAL 2
#endif

#endif
#endif


