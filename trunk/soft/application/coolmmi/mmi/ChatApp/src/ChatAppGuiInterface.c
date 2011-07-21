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
 * ChatAppGuiInterface.C
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
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: ChatAppGuiInterface.c

  	PURPOSE		: To Provide GUI Interface functions  

 

	 

	DATE		: Feb 18,04

**************************************************************/

#define __NEWSIMULATOR 
#include "mmi_features.h"

#ifdef __MMI_MESSAGES_CHAT__
#include "mmi_trace.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"


#include "eventsgprot.h"
#include "historygprot.h"
#include "globalmenuitems.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "custmenures.h"
#include "commonscreens.h"
#include "unicodexdcl.h"
#include "timerevents.h"
#include "debuginitdef.h"
#include "protocolevents.h"

#include "chatappdef.h"
#include "chatgtype.h"
#include "chatappprot.h"
#include "chatappgexdcl.h"
#include "chatappgprot.h"
#include "emsmiscell.h"

#include "settingprofile.h"
#include "messagesl4def.h"
#include "messagesexdcl.h"
#include "smsguiinterfaceprot.h"
#include "phonebooktypes.h"
#include "phonebookgprot.h"
#include "phonebookprot.h"
#include "wgui_ems_categories.h"
#include "profilegprots.h"



#include "nvramenum.h"
#include "nvramprot.h"
#include "messagesresourcedata.h"
#undef __NEWSIMULATOR
#include "l4dr.h"
#include "phonebookdef.h"
#include "smsapi.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim_sms_chat.h"
#endif

/****************Static Variable Start*********/
static U8 gSetChatFlagNameNumber=0;	
static S32 gHilightedChatRoomInfoItem;
static U8 gNickname[(MAX_NICKNAME_LENGTH +1)*ENCODING_LENGTH];
static U8 gDestinationNo[(MAX_PHONE_NUMBER_LENGTH +2)*ENCODING_LENGTH];
static U16 gDetailChatRoomInfoImageList[4];
static pBOOL gQuitFlag;
static S16 currentMsgCount;
static S8	gNumbersList[(MAX_PHONE_NUMBER_LENGTH +2)*ENCODING_LENGTH];

/****************Static Variable End*********/

/****************Static Function Start*********/
/****************Static Function End*********/
/****************Global Variable Start*********/
pBOOL gInputBoxUcs2Flag;
pBOOL gOutBoxSaveFlag;
/****************Global Variable End*********/

/****************Function Start*********/
 /****************Function End*********/

/*********Extern Variable**********/
//Deepali
extern chat_context_struct g_chat_cntx;
extern MMI_PHB_LIST_VIEW phbListView;
extern U8 gIsItChatSmsArrivalInd;
extern U8 gIsThereAnyNewChatSMSToInd;
extern wgui_inline_item	wgui_inline_items[];

/*********Extern Function**********/
extern S32 GetInlineListMenuChanged(void); 
extern EMSData * GetEMSDataForEdit              (EMSData ** p, U8 force) ;
extern void ReleaseEMSEditBuffer (void);


 /**************************End****************************/

/*****************************************************************************
* FUNCTION
*  SetChatHiliteHandlers
* DESCRIPTION
*   It Sets Hilite Handler of Chat Menu items
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SetChatHiliteHandlers(void)
{
	SetHiliteHandler (MENU_CHAT_ROOM1, HighlightChatRooM1);
	SetHiliteHandler (MENU_CHAT_ROOM2, HighlightChatRooM2);
	SetHiliteHandler (MENU_CHATROOM_INFO,HighlightChatRoomInfo);
#ifdef __MMI_MULTI_SIM__
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	SetHiliteHandler (MENU_START_NEW,HighlightChatMasterStartMenu);
#else
	SetHiliteHandler (MENU_START_NEW,HighlightChatDualmodeStartMenu);
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
	MTPNP_PFAL_SetDualmode_Chat_Handler();
#else
	SetHiliteHandler (MENU_START_NEW,HighlightChatStartMenu);
#endif/* __MMI_MULTI_SIM__ */
	
	SetHiliteHandler (MENU_RESUME_CHAT,HighlightChatResumeMenu);
	SetHiliteHandler (MENU_END_CHAT,HighlightChatEndMenu);
	
	SetHiliteHandler (MENU_CHATMSG_SEND,HighlightChatSend);
	SetHiliteHandler (MENU_CHATAPP_TEMPLATE,HighlightChatTemplate);
	SetHiliteHandler (MENU_CHAT_PHB_NUM,HighlightChatPhbNum);
	SetHiliteHandler (MENU_CHAT_PHB_NAM,HighlightChatPhbNam);
	SetHiliteHandler (MENU_CHAT_INPUT_METHOD,HighlightChatInputMethod);
	
	SetHiliteHandler (MENU_CHAT_REPLY_SCRN,HighlightChatReply);
	SetHiliteHandler (MENU_CHAT_USE_NUM_SCRN,HighlightChatUseNumMethod);
	SetHiliteHandler (MENU_CHAT_CLEAR_SCRN,HighlightChatClearMethod);
	SetHiliteHandler (MENU_CHAT_TEMP_SCRN,HighlightChatTempLeaveMethod);
	SetHiliteHandler (MENU_CHAT_QUIT_SCRN,HighlightChatQuitMethod);
//	SetHiliteHandler (MENU_CHAT_SAVE,HighlititeChatSaveOutBox);

}

	
/****************/
/*Highlight Function*/
/****************/
/*****************************************************************************
* FUNCTION
*  HighlightChatRooM1
* DESCRIPTION
*   Hilite function of Chat Room1
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatRooM1(void)
{
	SetCurrentChatRoomNo(CHAT_ROOM1);
	gChatInvitation=FALSE;
	gDestinationNo[0]='\0';
	gNickname[0]='\0';
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryChatOption,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (EntryChatOption, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
}

/*****************************************************************************
* FUNCTION
*  HighlightChatRooM2
* DESCRIPTION
*   Hilite function of Chat Room2
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatRooM2(void)
{
	SetCurrentChatRoomNo(CHAT_ROOM2);
	gChatInvitation=FALSE;
	gDestinationNo[0]='\0';
	gNickname[0]='\0';
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryChatOption,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(EntryChatOption, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightChatRoomInfo
* DESCRIPTION
*   Hilite function of Chat Room Info
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatRoomInfo(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryChatRoomInfo,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(EntryChatRoomInfo, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
	
/*****************************************************************************
* FUNCTION
*  HighlightChatStartMenu
* DESCRIPTION
*   Hilite function of Chat Start Menu
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatStartMenu(void)
{
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
    if(!MTPNP_PFAL_Is_CardAbsent(0))
    {
        HighlightChatMasterStartMenu();
    }
#else
    ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
    /* reset message content in case the message in different chatroom using different encoding scheme */
    memset(g_chat_cntx.writeChatBuffer, 0, (MAX_MSG_SIZE+1)*ENCODING_LENGTH);
    SetLeftSoftkeyFunction(EntryChatWrite,KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(EntryChatWrite, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
}

/*****************************************************************************
* FUNCTION
*  HighlightChatResumeMenu
* DESCRIPTION
*   Hilite function of Chat Resume Menu
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatResumeMenu(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryChatRoom,KEY_EVENT_UP);
	SetKeyHandler (EntryChatRoom, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
	
/*****************************************************************************
* FUNCTION
*  HighlightChatEndMenu
* DESCRIPTION
*   Hilite function of Chat End Menu
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatEndMenu(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(PreEntryChatQuitConfirm,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (PreEntryChatQuitConfirm, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
	
/*****************************************************************************
* FUNCTION
*  HighlightChatSend
* DESCRIPTION
*   Hilite Send function
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatSend(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(PreSendChatMsg,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (PreSendChatMsg, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
	
/*****************************************************************************
* FUNCTION
*  HighlightChatTemplate
* DESCRIPTION
*   Hilite ChatTemplate
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatTemplate(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
    	SetLeftSoftkeyFunction(mmi_msg_entry_template_list, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_entry_template_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
	
/*****************************************************************************
* FUNCTION
*  HighlightChatPhbNum
* DESCRIPTION
*   Hilite Function for Phone Book Number
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatPhbNum(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EnterPhoneBookForChatNumberApp,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (EnterPhoneBookForChatNumberApp, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightChatPhbNam
* DESCRIPTION
*   Hilite Phone Book Name function
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatPhbNam(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EnterPhoneBookForChatNameApp,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (EnterPhoneBookForChatNameApp, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightChatInputMethod
* DESCRIPTION
*   Hilite Function for Chat Input Method
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatInputMethod(void)
{
	SetInputMethodAndDoneCaptionIcon(CHAT_IMAGE_APP_ID);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryInputMethodScreen,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistoryFromChatInputMethod);
	SetKeyHandler (EntryInputMethodScreen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightChatReply
* DESCRIPTION
*   Hilite function of Chat Reply Screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatReply(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
        /* reset message content in case the message in different chatroom using different encoding scheme */
        memset(g_chat_cntx.writeChatBuffer, 0, (MAX_MSG_SIZE+1)*ENCODING_LENGTH);
	SetLeftSoftkeyFunction(EntryChatWrite,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (EntryChatWrite, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
	
/*****************************************************************************
* FUNCTION
*  HighlightChatUseNumMethod
* DESCRIPTION
*   Hilite function of Chat Use Number method
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatUseNumMethod(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(mmi_msg_entry_use_number,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_entry_use_number, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightChatClearMethod
* DESCRIPTION
*   Hilite Handler of Chat Clear Method
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatClearMethod(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryClearChatRoomConfirm,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (EntryClearChatRoomConfirm, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightChatTempLeaveMethod
* DESCRIPTION
*   Hilite Handler of Chat Temp Leave Method
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatTempLeaveMethod(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(TempleaveChatApp,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (TempleaveChatApp, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
	
/*****************************************************************************
* FUNCTION
*  HighlightChatQuitMethod
* DESCRIPTION
*   Hilite Handler of Chat Quit Method
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightChatQuitMethod(void)
{
	gQuitFlag=TRUE;
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryChatQuitConfirm,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler (EntryChatQuitConfirm, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
	
/*****************************************************************************
* FUNCTION
*  HighlititeChatSaveOutBox
* DESCRIPTION
*   This is Hilight Function of Chat save 
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlititeChatSaveOutBox(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(SaveChatMsgToOutBox, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (SaveChatMsgToOutBox, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  EntryChatOption
* DESCRIPTION
*   Entry function of Chat Room Option (before enter chat room)
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryChatOption(void)
{
	U8* guiBuffer;
	S32 numOfItems = 0;
	U16 itemList[MAX_SUB_MENUS];
	U16 itemIcon [MAX_SUB_MENUS];
	U16 scrId=0;

	EntryNewScreen(CHAT_SCREEN_ROOM_ID, NULL, EntryChatOption, NULL);
	if(g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].status==ENABLE) 
		scrId=MENU_ACTIVE_CHAT_ROOM;
	else 
		scrId=MENU_DEACTIVE_CHAT_ROOM;

	guiBuffer = GetCurrGuiBuffer(CHAT_SCREEN_ROOM_ID);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetParentHandler(scrId);

	numOfItems = GetNumOfChild(scrId);

	GetSequenceStringIds(scrId, itemList);
	GetSequenceImageIds(scrId,itemIcon);

	RegisterHighlightHandler (ExecuteCurrHiliteHandler);

    ShowCategory52Screen(
        CHAT_ROOM_TEXT,
        CHAT_IMAGE_APP_ID,
        STR_GLOBAL_OPTIONS,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        numOfItems,
        itemList,
        (U16*) gIndexIconsImageList,
        NULL,
        0,
        0,
        guiBuffer);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  EntryChatRoomInfo
* DESCRIPTION
*   Entry function of Chat Room Info
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryChatRoomInfo(void)
{
	U8* guiBuffer;
	U16 inputBufferSize;    
	U8* inputBuffer;
	EntryNewScreen(CHAT_ROOM_INFO_ID, ExitChatRoomInfo, NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(CHAT_ROOM_INFO_ID);
	if(guiBuffer==NULL) 
	{
		pfnUnicodeStrcpy((S8*)gNickname,(S8*)g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.nickName);
		pfnUnicodeStrcpy((S8*)gDestinationNo,(S8*)g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.destinationNo);
		gHilightedChatRoomInfoItem=1;	
}

	inputBuffer = GetCurrNInputBuffer(CHAT_ROOM_INFO_ID, &inputBufferSize);
	
	RegisterHighlightHandler(ChatInlineHandler);
	if (GetInlineDoneFlag(guiBuffer))
		SetInlineDoneFlag(guiBuffer);
	
	InitializeCategory57Screen();
	FillChatRoomActivationStruct();

	if(inputBuffer!=NULL ) 
		SetCategory57Data(wgui_inline_items, 4, inputBuffer); 

    ShowCategory57Screen(
        CHAT_ROOM_INFO_STR,
        CHAT_IMAGE_APP_ID,
        STR_GLOBAL_OPTIONS,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        4,
        gDetailChatRoomInfoImageList,
        wgui_inline_items,
        gHilightedChatRoomInfoItem,
        guiBuffer);

	SetCategory57RightSoftkeyFunctions(PreSaveChatInfo,GoBackHistory);

}

/*****************************************************************************
* FUNCTION
*  ExitChatRoomInfo
* DESCRIPTION
*   Exit function of Chat Room Info
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ExitChatRoomInfo(void)
{
	history_t	h;
	U16 inputBufferSize=0;
	
	CloseCategory57Screen();
	
	h.scrnID = CHAT_ROOM_INFO_ID;
	h.entryFuncPtr = EntryChatRoomInfo;
	gHilightedChatRoomInfoItem=(U8) GetHighlightedItem();
	GetCategoryHistory(h.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();  
	GetCategory57Data((U8*) h.inputBuffer);       
	AddNHistory(h, inputBufferSize);    
}

	
/*****************************************************************************
* FUNCTION
*  EntryChatWrite
* DESCRIPTION
*   It provides Edit screen for writing Chat Message
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void mmi_msg_go_back_from_write_msg (void);
extern void mmi_msg_send_msg_to_only_entry (void);
U8 mmi_frm_sms_check_action_pending(void);
void AlmDisableSPOF(void) ;
void GBSetInputboxLSKFunction(FuncPtr f);
void EntryChatWrite(void)
{
#if 1
	U8* guiBuffer;
  	mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) EntryChatWrite");
	if(!pfnUnicodeStrlen((PS8)g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.destinationNo))
			DisplayPopup((PU8)GetString(CHATROOM_EMPTY_ERROR), IMG_GLOBAL_ERROR, FALSE, 2000, ERROR_TONE);
	else
	{
		EntryNewScreen(CHAT_SCREEN_START_NEW_ID, ExitChatWrite, EntryChatWrite, NULL);
		guiBuffer=GetCurrGuiBuffer(CHAT_SCREEN_START_NEW_ID);
		ShowCategory5Screen(STR_CHAT_WRITE_MESSAGE_TITLE, CHAT_IMAGE_APP_ID,
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,(S16)(INPUT_TYPE_ALPHANUMERIC_SENTENCECASE|INPUT_TYPE_FORTY_FOUR_LESS_CHARACTER),g_chat_cntx.writeChatBuffer,MAX_MSG_SIZE+1,guiBuffer);




		SetLeftSoftkeyFunction (EntryChatWriteOption, KEY_EVENT_UP);
#if defined(__MMI_GB__)
		GBSetInputboxLSKFunction(EntryChatWriteOption);  
#endif

		SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
		SetKeyHandler(PreSendChatMsg, KEY_SEND, KEY_EVENT_UP);
	}
#endif 
#if 0
	U8* 		guiBuffer=NULL;
	EMSData*	pEMS;
	U16 writeChatBufferCharNum;


	if(mmi_frm_sms_check_action_pending())
	{
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
  
	if(!pfnUnicodeStrlen((PS8)g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.destinationNo))
			DisplayPopup((PU8)GetString(CHATROOM_EMPTY_ERROR), IMG_GLOBAL_ERROR, FALSE, 2000, ERROR_TONE);
	else
	{
		EntryNewScreen(CHAT_SCREEN_START_NEW_ID, ExitChatWrite, EntryChatWrite, NULL);
		AlmDisableSPOF();
		guiBuffer=GetCurrGuiBuffer(CHAT_SCREEN_START_NEW_ID);
		SetMessagesCurrScrnID(CHAT_SCREEN_START_NEW_ID);
		GetEMSDataForEdit (& pEMS, 0);
		SetMessagesCurrScrnID(CHAT_SCREEN_START_NEW_ID);
		writeChatBufferCharNum = pfnUnicodeStrlen((PS8)g_chat_cntx.writeChatBuffer); 
		pEMS->textLength += writeChatBufferCharNum*ENCODING_LENGTH;
		/*Guojian Add Start For 8817 Ver: Jassmine  on 2008-6-23 11:7 */
		pEMS->CurrentPosition.OffsetToText = pEMS->textLength;
		/*Guojian Add End  For 8817 Ver: Jassmine  on 2008-6-23 11:7 */
		
		pfnUnicodeStrncat(pEMS->textBuffer, (S8*)g_chat_cntx.writeChatBuffer, writeChatBufferCharNum*ENCODING_LENGTH);
		memset(g_chat_cntx.writeChatBuffer, 0, (MAX_MSG_SIZE+1)*ENCODING_LENGTH);
		ShowCategory28Screen (STR_CHAT_WRITE_MESSAGE_TITLE, CHAT_IMAGE_APP_ID,
						  			STR_GLOBAL_OPTIONS, IMG_SMS_COMMON_NOIMAGE,
						  			STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE,
						  			pEMS, guiBuffer );

		SetLeftSoftkeyFunction (EntryChatWriteOption, KEY_EVENT_UP);
#if defined(__MMI_GB__)
		GBSetInputboxLSKFunction(EntryChatWriteOption);  
#endif
		SetKeyHandler(PreSendChatMsg, KEY_SEND, KEY_EVENT_UP);//mmi_msg_entry_send_option
		RegisterInputMethodScreenCloseFunction (Messages2GoBackHistory);
		SetCategory28RightSoftkeyFunction (mmi_msg_go_back_from_write_msg, KEY_EVENT_UP);

}
  #endif
}
	
/*****************************************************************************
* FUNCTION
*  ExitChatWrite
* DESCRIPTION
*   Exit function of Chat Edit Screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ExitChatWrite(void)
{
	if(GetInputBoxUCS2Count()) 
			gInputBoxUcs2Flag=1;
	else
			gInputBoxUcs2Flag=0;

	}
	
/*****************************************************************************
* FUNCTION
*  EntryChatWriteOption
* DESCRIPTION
*   Entry function of Chat Option Screen (write message)
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryChatWriteOption(void)
	{
	U8* guiBuffer;
	S32 numOfItems = 0;
	U16 itemList[MAX_SUB_MENUS];
	
  	mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) EntryChatWriteOption");
	EntryNewScreen(CHAT_OPTION_SCREEN_ID, NULL, EntryChatWriteOption, NULL);

     	gHilightedChatRoomInfoItem=0;
	guiBuffer = GetCurrGuiBuffer(CHAT_OPTION_SCREEN_ID);
	SetParentHandler(MENU_CHAT_OPTION);
	numOfItems = GetNumOfChild(MENU_CHAT_OPTION);
	GetSequenceStringIds(MENU_CHAT_OPTION, itemList);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);


	ShowCategory52Screen(CHAT_ROOM_TEXT,CHAT_IMAGE_APP_ID,
		STR_GLOBAL_OK,IMG_GLOBAL_OK,
		STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
		numOfItems,itemList,(U16 *)gIndexIconsImageList,NULL,0,0,guiBuffer);



	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	GetCurrEndKeyDownHandler();
	SetKeyHandler (ExecCurrEndKeyDownHandler, KEY_END, KEY_EVENT_UP);
}
	
/*****************************************************************************
* FUNCTION
*  EntryChatRoom
* DESCRIPTION
*   Entry Screen for Chat Room 
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryChatRoom()
{
	U8* guiBuffer;
	S32 numOfItems = 0;
	char *phNumber;
	S16 index=GetCurrentChatRoomNo();

	#ifdef __MMI_MULTI_SIM__
	E_MTPNP_AD_CHANNELS channel = MTPNP_PFAL_Get_Chat_Resume_Channel(index);
	MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
	MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, channel);
	#endif
	if(GetMessagesCurrScrnID()!=CHAT_ROOM_SCREEN_ID)
		EntryNewScreen(CHAT_ROOM_SCREEN_ID, ExitChatRoom, EntryChatRoom, NULL);


	if(IsScreenPresent(SCR_ID_MSG_INBOX_MSG))
	{
		if(!IsScreenPresent(CHAT_SCREEN_ROOM_ID))
			HistoryReplace(SCR_ID_MSG_INBOX_MSG, CHAT_SCREEN_ROOM_ID, EntryChatOption);
		
		DeleteScreenIfPresent(SCR_ID_MSG_INBOX_LIST);
		DeleteScreenIfPresent(SCR_ID_MSG_INBOX_MSG);
		DeleteScreenIfPresent(SCR_ID_MSG_INBOX_OPTION);
	}

	guiBuffer = GetCurrGuiBuffer(CHAT_ROOM_SCREEN_ID);

	SetParentHandler(MENU_CHAT_ROOM_SCRN);

	numOfItems = GetNumOfMessage(index);

	RegisterHighlightHandler (ExecuteCurrHiliteHandler);


	
	phNumber=lookUpNumber((PS8)g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.destinationNo);
	if (phNumber)
{
		if( pfnUnicodeStrcmp(phNumber,""))
			pfnUnicodeStrncpy((PS8)gNumbersList,(PS8) phNumber, (U32)(sizeof(gNumbersList)/ENCODING_LENGTH)-1);
		else 
			pfnUnicodeStrncpy((PS8)gNumbersList,(PS8) g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.destinationNo, (U32)(sizeof(gNumbersList)/ENCODING_LENGTH)-1);
	}
	else 
		pfnUnicodeStrncpy((PS8)gNumbersList,(PS8) g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.destinationNo, (U32)(sizeof(gNumbersList)/ENCODING_LENGTH)-1);

	if(GetMessagechatRoomNo()==index)
	{
		gIsItChatSmsArrivalInd=FALSE;
		gIsThereAnyNewChatSMSToInd=FALSE;
	}
	if(g_chat_cntx.blinkChatFlag[index])
	{
		HideChatStatusIcon(index);
		ShowChatStatusIcon(index);
		g_chat_cntx.blinkChatFlag[index]=FALSE;
	}

	SetMessagesCurrScrnID(CHAT_ROOM_SCREEN_ID);

    ShowCategory305Screen(
        0,
        CHAT_IMAGE_APP_ID,
        STR_GLOBAL_OPTIONS,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        numOfItems,
        g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatStrings,
        guiBuffer,
        (PU8) gNumbersList,
        IMG_GLOBAL_OK);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetLeftSoftkeyFunction(EntryChatRoomOption, KEY_EVENT_UP);
    GetCurrEndKeyDownHandler();
    SetKeyHandler(ExecCurrEndKeyDownHandler, KEY_END, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  ExitChatRoom
* DESCRIPTION
*   Exit Screen for Chat Room 
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ExitChatRoom(void)
{
	SetMessagesCurrScrnID(0);
}

/*****************************************************************************
* FUNCTION
*  EntrySmsMemoryFullIndInChatApp
* DESCRIPTION
*   Entry SMS memory full indication in chat room screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntrySmsMemoryFullIndInChatApp (void)	// Magesh
{
	if(GetMessagesCurrScrnID()==CHAT_ROOM_SCREEN_ID)
		DisplayPopup((PU8)GetString(CHATROOM_STORAGE_FULL_ERROR), IMG_GLOBAL_ERROR, FALSE, 2000, ERROR_TONE);
}


/*****************************************************************************
* FUNCTION
*  EntryChatRoomOption
* DESCRIPTION
*   Entry function for Chat Room option (chat room)
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryChatRoomOption(void)
{
	U8* guiBuffer=NULL;
	S32 numofitems = 0;
	U16 itemList[MAX_SUB_MENUS];
	EntryNewScreen(CHAT_OPTION_SCREEN_ID_2, NULL, EntryChatRoomOption, NULL);

	guiBuffer = GetCurrGuiBuffer(CHAT_OPTION_SCREEN_ID_2);
	SetParentHandler(MENU_CHAT_OPTION_SCRN);

	numofitems = GetNumOfChild(MENU_CHAT_OPTION_SCRN);

	GetSequenceStringIds(MENU_CHAT_OPTION_SCRN, itemList);
	

	RegisterHighlightHandler (ExecuteCurrHiliteHandler);



	ShowCategory52Screen(STR_GLOBAL_OPTIONS,CHAT_IMAGE_APP_ID,
		STR_GLOBAL_OK,IMG_GLOBAL_OK,
		STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
		numofitems,itemList,(U16 *)gIndexIconsImageList,NULL,0,0,guiBuffer);



	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	
}


/*****************************************************************************
* FUNCTION
*  PreEntryChatQuitConfirm
* DESCRIPTION
*   Wrapper function of GotoQuitChatScreen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PreEntryChatQuitConfirm(void)
{
	gQuitFlag=FALSE;
	EntryChatQuitConfirm();
}

/*****************************************************************************
* FUNCTION
*  EntryChatQuitConfirm
* DESCRIPTION
*   Ask from user before quitting from Chat Room
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryChatQuitConfirm(void)
{
	UI_string_type str;

	if(gQuitFlag==TRUE)
		str=get_string(STR_QUIT_CHAT_ROOM_QUERY);
	else 
		str=get_string(STR_END_QUERY);
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO, 
	str, IMG_GLOBAL_QUESTION, WARNING_TONE);
		
	SetLeftSoftkeyFunction(QuitChatRoom, KEY_EVENT_UP);
	if(gQuitFlag==TRUE)
		SetRightSoftkeyFunction(GoBackChatHistory, KEY_EVENT_UP);	
	else 
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);	
}

/*****************************************************************************
* FUNCTION
*  EntryClearChatRoomConfirm
* DESCRIPTION
*   It confirms from User before clearing Chat Room
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryClearChatRoomConfirm(void)
{

	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO, 
	get_string(STR_CLEAR_CHAT_ROOM_QUERY), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(ClearChatRoom, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
	
/*****************************************************************************
* FUNCTION
*  EntryChatReplaceConfirm
* DESCRIPTION
*   This function Displace Yes/No pop up before replacing Chat Room
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryChatReplaceConfirm(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO, 
	get_string(STR_REPLACE_CHAT_INFO_QUERY), IMG_GLOBAL_QUESTION, WARNING_TONE);
		
	SetLeftSoftkeyFunction(ReplaceChatRoom, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*  EnterPhoneBookForChatNumberApp
* DESCRIPTION
*   This function calls phone book function to get Phone book number
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EnterPhoneBookForChatNumberApp(void)
{
	phbListView=MMI_PHB_LIST_FOR_CHAT;
	g_phb_cntx.highlight_entry = 0;
	gSetChatFlagNameNumber=CHAT_PHB_NUMBER;
	mmi_phb_list_pre_entry_second_level();
}

/*****************************************************************************
* FUNCTION
*  EnterPhoneBookForChatNameApp
* DESCRIPTION
*   This function calls phonebook function to get phone book name
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EnterPhoneBookForChatNameApp(void)
{
	/* empty number entry could be selected without checking */
	phbListView=MMI_PHB_LIST_FOR_CHAT_NO_CHECK;
	g_phb_cntx.highlight_entry = 0;
	gSetChatFlagNameNumber=CHAT_PHB_NAME;
	mmi_phb_list_pre_entry_second_level();
}


/*****************************************************************************
* FUNCTION
*  InsertTemplateToChat
* DESCRIPTION
*   To Insert template in Chat message
*
* PARAMETERS
*  template to be inserted
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void InsertTemplateToChat (S8 * templ)
{
	history_t	temphistory;
	historyNode *History;
	mmi_trace(g_sw_SMS, "MMI_SMS:(Chat) InsertTemplateToChat");

    if (templ)
    {
        memset(&temphistory, 0, sizeof(temphistory));
        GetHistory(CHAT_SCREEN_START_NEW_ID, &temphistory);
        AppendCategory5String(
            INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
            g_chat_cntx.writeChatBuffer,
            MAX_MSG_SIZE,
            (U8*) templ,
            temphistory.guiBuffer);
        if (GetHistoryScrID(CHAT_SCREEN_START_NEW_ID, &History) == ST_SUCCESS)
        {
            memcpy(History->guiBuffer, temphistory.guiBuffer, MAX_GUI_BUFFER);
        }
    }
    DeleteUptoScrID(CHAT_SCREEN_START_NEW_ID);
    GoBackHistory();
    DeInitTemplates();
}


/*****************************************************************************
* FUNCTION
*  SetNameNumberForChatScreen
* DESCRIPTION
*   This function Set Phone Book Name or Number to Edit screen
*
* PARAMETERS
*  name or number
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SetNameNumberForChatScreen(U8 name[], U8 number[])
{
	history_t	temphistory;
	historyNode *History;
	if(gSetChatFlagNameNumber==CHAT_PHB_NAME)
	{
        if (pfnUnicodeStrlen((S8*) name))
        {
            memset(&temphistory, 0, sizeof(history_t));
            GetHistory(CHAT_SCREEN_START_NEW_ID, &temphistory);
            AppendCategory5String(
                INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
                g_chat_cntx.writeChatBuffer,
                MAX_MSG_SIZE,
                name,
                temphistory.guiBuffer);
            if (GetHistoryScrID(CHAT_SCREEN_START_NEW_ID, &History) == ST_SUCCESS)
            {
                memcpy(History->guiBuffer, temphistory.guiBuffer, MAX_GUI_BUFFER);
            }
		}
		DeleteUptoScrID(CHAT_SCREEN_START_NEW_ID);
		GoBackHistory();
	}
	else if(gSetChatFlagNameNumber==CHAT_PHB_NUMBER)
	{
        if (pfnUnicodeStrlen((S8*) number))
        {
            memset(&temphistory, 0, sizeof(history_t));
            GetHistory(CHAT_SCREEN_START_NEW_ID, &temphistory);
            AppendCategory5String(
                INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
                g_chat_cntx.writeChatBuffer,
                MAX_MSG_SIZE,
                number,
                temphistory.guiBuffer);
            if (GetHistoryScrID(CHAT_SCREEN_START_NEW_ID, &History) == ST_SUCCESS)
            {
                memcpy(History->guiBuffer, temphistory.guiBuffer, MAX_GUI_BUFFER);
            }
        }
        DeleteUptoScrID(CHAT_SCREEN_START_NEW_ID);
		GoBackHistory();
	}
	else if(gSetChatFlagNameNumber==CHAT_PHB_NUMBER_CHATROOMINFO)
	{
		S32 size;
		S16 plusLen=0;
		memset (&temphistory, 0, sizeof (history_t));
		GetHistory (CHAT_ROOM_INFO_ID, &temphistory);	
		if(number[0]=='+' )
			plusLen=MAX_PHONE_NUMBER_LENGTH+1;
		else 
			plusLen=MAX_PHONE_NUMBER_LENGTH;
		if(pfnUnicodeStrlen ((PS8)number) > plusLen)
		{
			DisplayPopup ((PU8)GetString (STR_SMS_NUM_LEN_EXCEEDED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			return;
		}	
		pfnUnicodeStrncpy((S8*)gDestinationNo,(S8*)number,plusLen);
		AppendInlineItemTextEditString(INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING,gDestinationNo,wgui_inline_items,4,&size,&temphistory, 1);
		/* ensure DeleteNHistory(1) to delete chat room info screen in case there are two phonebook screens presented */
		DeleteUptoScrID(CHAT_ROOM_INFO_ID);
		DeleteNHistory(1);
		AddNHistory (temphistory, (U16)size);
		GoBackHistory();
	}
}


/*****************************************************************************
* FUNCTION
*  GoBackHistoryFromChatInputMethod
* DESCRIPTION
*   Wrapper function to call GoBackHistory from Input Method function
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GoBackHistoryFromChatInputMethod(void)
{
	DeleteUptoScrID(CHAT_SCREEN_START_NEW_ID);
	GoBackHistory();
}


/*****************************************************************************
* FUNCTION
*  ChatInlineHandler
* DESCRIPTION
*   Hilite Handler for Destination Number option of Chat Room Info
*
* PARAMETERS
*  Hilited index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ChatInlineHandler(S32 index)
	{
	switch(index)
	{
		case 1:
			gSetChatFlagNameNumber=0;
			 
			EnableCategory57ScreenDone();
			if (GetInlineListMenuChanged())
			{
				ChangeRightSoftkey (STR_GLOBAL_DONE, IMAGE_ID_NULL);
			}
			break;
		case 3://check!!!!
			if(pfnUnicodeStrlen((S8 *)gDestinationNo)==0)
			{
				gSetChatFlagNameNumber=CHAT_PHB_NUMBER_CHATROOMINFO;
				phbListView = MMI_PHB_LIST_FOR_CHAT;
				ChangeLeftSoftkey (STR_SMS_COMMON_SEARCH, IMG_SMS_COMMON_NOIMAGE);
				SetLeftSoftkeyFunction (mmi_phb_list_pre_entry_second_level, KEY_EVENT_UP);
				ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
				SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
			}
			 
			DisableCategory57ScreenDone();
			RegisterInputBoxEmptyFunction (ChatNumberScreenEmpty);
			RegisterInputBoxNotEmptyFunction (ChatNumberScreenNotEmpty);
			
			break;
		default:
			break;
	}
}

/*****************************************************************************
* FUNCTION
*  ChatNumberScreenEmpty
* DESCRIPTION
*   This function is called when there is no data on Edit Screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ChatNumberScreenEmpty(void)
	{
	gSetChatFlagNameNumber=CHAT_PHB_NUMBER_CHATROOMINFO;
	phbListView = MMI_PHB_LIST_FOR_CHAT;
	g_phb_cntx.highlight_entry = 0;
	ChangeLeftSoftkey (STR_SMS_COMMON_SEARCH, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (mmi_phb_list_pre_entry_second_level, KEY_EVENT_UP);
	 
	//ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	//SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  ChatNumberScreenNotEmpty
* DESCRIPTION
*   This function is called when there is some data on Edit Screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ChatNumberScreenNotEmpty(void)
{
	gSetChatFlagNameNumber=0;
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(PreSaveChatInfo,KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  ChatInlineCustomFunction
* DESCRIPTION
*   This function fills Chat Room Activation recods
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ChatInlineCustomFunction (void)
{
	SetInputMethodAndDoneCaptionIcon(CHAT_IMAGE_APP_ID);
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
	SetLeftSoftkeyFunction (EntryScrForInputMethodAndDone, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  FillChatRoomActivationStruct
* DESCRIPTION
*   Fill chat room info for inline editor
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void FillChatRoomActivationStruct(void)
{
	SetInlineItemCaption(&wgui_inline_items[0],(U8 *)GetString(CHATROOM_NICKNAME));
	gDetailChatRoomInfoImageList[0]=IMG_CHAT_NICKNAME;
	SetInlineItemActivation(&wgui_inline_items[1],KEY_LSK,KEY_EVENT_UP );
	SetInlineItemFullScreenEdit(&wgui_inline_items[1],CHATROOM_NICKNAME,CHAT_IMAGE_APP_ID,(PU8)gNickname, MAX_NICKNAME_LENGTH+1, (INPUT_TYPE_ALPHANUMERIC_UPPERCASE));
	SetInlineFullScreenEditCustomFunction(&wgui_inline_items[1], ChatInlineCustomFunction);
	gDetailChatRoomInfoImageList[1]=0;
	SetInlineItemCaption(&wgui_inline_items[2],(U8 *)GetString(CHATROOM_DESTINATION));
	gDetailChatRoomInfoImageList[2]=IMG_CHAT_DESTINATION;

	SetInlineItemActivation(&wgui_inline_items[3],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemTextEdit(&wgui_inline_items[3], (PU8)gDestinationNo, MAX_PHONE_NUMBER_LENGTH+1, INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING );
	DisableInlineItemHighlight(&wgui_inline_items[3]);
	
	gDetailChatRoomInfoImageList[3]=0;

  #if defined(__MMI_GB__)    
 	 mmi_frm_set_highlight_handler();
	GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone);
	
  #endif
}

/*****************************************************************************
* FUNCTION
*  GoBackHistoryPreChatInfoSaveKey
* DESCRIPTION
*   This is wrapper function for calling GobackHistory from Saving Chat Room Info
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GoBackHistoryPreChatInfoSaveKey(void)
{
//	CloseCategory57Screen(); 
//	DeleteUptoScrID(CHAT_ROOM_INFO_ID);
	GoBacknHistory(1);
}

/*****************************************************************************
* FUNCTION
*  PreSaveChatInfo
* DESCRIPTION
*   This function asks user Befor saving Chat Room info
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PreSaveChatInfo(void)
{
	if(!GetInlineListMenuChanged())
	{
		GoBackHistory();
		return;
	}
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO, 
		get_string(STR_SAVE_CHAT_INFO), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(SaveChatRoomInfo, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistoryPreChatInfoSaveKey, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  SaveChatRoomInfo
* DESCRIPTION
*   This function Calls Number Check and number save functions
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SaveChatRoomInfo(void)
{
		S32 index=GetCurrentChatRoomNo();
	if(CheckChatNumber(gDestinationNo,g_chat_cntx.chatRoomInfo[index].status,index))
	{
			FillChatRoomInfo(gNickname,gDestinationNo,index);
			DisplayPopup ((PU8)GetString (STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, FALSE, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
			DeleteScreenIfPresent(CHAT_ROOM_INFO_ID);
		}
}

/*****************************************************************************
* FUNCTION
*  FillChatRoomInfo
* DESCRIPTION
*   This function Writes Chat Room Info on NVRAM
*
* PARAMETERS
*  Nickname,Destination Number and Chat Room Number
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void FillChatRoomInfo(PU8 name,PU8 destnum,S32 index)
{
	S16 error;
	S16 roomCount;
	chat_info_nvram_struct infostruct[MAX_NO_OF_CHAT_ROOM];
	
	pfnUnicodeStrcpy((S8*)(g_chat_cntx.chatRoomInfo[index].chatInfo.nickName),(S8*)name);
	pfnUnicodeStrcpy((S8*)(g_chat_cntx.chatRoomInfo[index].chatInfo.destinationNo),(S8*)destnum);
	/*memset(&gChatRoomInfo[index].chatInfo.destinationNo[len*ENCODING_LENGTH],0,ENCODING_LENGTH);*/
	for(roomCount=0;roomCount<MAX_NO_OF_CHAT_ROOM;roomCount++){
		U16 index2 = (U16)roomCount+1;
		pfnUnicodeStrcpy((S8*)(infostruct[roomCount].nickName),(S8*)g_chat_cntx.chatRoomInfo[roomCount].chatInfo.nickName);
		UnicodeToAnsii((S8*)(infostruct[roomCount].destinationNo),(S8*)(g_chat_cntx.chatRoomInfo[roomCount].chatInfo.destinationNo));
		WriteRecord (NVRAM_EF_CHAT_ROOM_INFO_LID,index2,(void*)&infostruct[roomCount],NVRAM_CHAT_ROOM_INFO_RECORD_SIZE,& error);		
	}
}

/*****************************************************************************
* FUNCTION
*  CheckDuplicateNumber
* DESCRIPTION
*   This function Writes Chat Room Info on NVRAM
*
* PARAMETERS
*  Destination number and Chat Room Number
* RETURNS
*  status
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL CheckDuplicateNumber(PU8 DestNo,S32 index)
{
	U8 num1[(MAX_PHONE_NUMBER_LENGTH+2)*ENCODING_LENGTH];
	U8 num2[(MAX_PHONE_NUMBER_LENGTH+2)*ENCODING_LENGTH];
		
	memset(num1,0x00,(MAX_PHONE_NUMBER_LENGTH+2)*ENCODING_LENGTH);
	memset(num2,0x00,(MAX_PHONE_NUMBER_LENGTH+2)*ENCODING_LENGTH);
	UnicodeToAnsii((PS8)num1, (PS8)g_chat_cntx.chatRoomInfo[index].chatInfo.destinationNo);
	UnicodeToAnsii((PS8)num2, (PS8)DestNo);
	if (mmi_phb_util_convert_number_to_int(num2)==INVALID_NUMBER)
		return FALSE;
	else
		return ComparePhoneNum((PU8)num1,(PU8)num2);
}
			
/*****************************************************************************
 * FUNCTION
 *  mmi_msg_chat_check_ussd_number
 * DESCRIPTION
 *  Check if the number is USSD string
 * PARAMETERS
 *  number          [?]         And Number length
 *  number_len      [IN]        
 * RETURNS
 *  TRUE means the number is USSD string, Otherwise FALSE
 *****************************************************************************/
U8 mmi_msg_chat_check_ussd_number(U16 *number, S32 number_len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 ussd_pattern = (U16) '#';

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (pfnUnicodeStrncmp((S8*) (number + number_len - 1), (S8*) & ussd_pattern, 1) == 0)
    {
        return TRUE;
    }
    return FALSE;
}
/*****************************************************************************
* FUNCTION
*  CheckChatNumber
* DESCRIPTION
*   This function Checks for Duplicate Number
*
* PARAMETERS
*  Destination number ,Chat Room status and Chat Room Number
* RETURNS
*  status
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL CheckChatNumber(PU8 DestNum,S8 status,S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 len = pfnUnicodeStrlen((S8*) DestNum);
    S32 checkindex = (index == 0) ? 1 : 0;
    U8 ascii_num[MAX_PHONE_NUMBER_LENGTH];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    memset(ascii_num, 0,MAX_PHONE_NUMBER_LENGTH);
    UnicodeNToAnsii((S8*) ascii_num, (S8*) DestNum, len * ENCODING_LENGTH);
    
	if(!len && (status==ENABLE))
    {
        DisplayPopup((PU8) GetString(CHATROOM_EMPTY_ERROR), (U16) IMG_GLOBAL_ERROR, FALSE, 2000, ERROR_TONE);
        return FALSE;
    }

    /* else if (CheckDuplicateNumber(DestNum,checkindex) && pfnUnicodeStrlen((PS8)DestNum)) */
    else if (CheckDuplicateNumber(DestNum, checkindex))
    {
        DisplayPopup((PU8) GetString(CHATROOM_DUPLICATE_ERROR), IMG_GLOBAL_ERROR, FALSE, 2000, ERROR_TONE);
        return FALSE;
    }
    else if (mmi_msg_chat_check_ussd_number((U16*) DestNum, len))
    {
        DisplayPopup((PU8) GetString(STR_ID_PHB_NUM_INVALID), IMG_GLOBAL_ERROR, FALSE, 2000, ERROR_TONE);
        return FALSE;
    }
    else if (!mmi_frm_sms_check_dst_number(ascii_num, (U8) len))
    {
        DisplayPopup((PU8) GetString(STR_ID_PHB_NUM_INVALID), IMG_GLOBAL_ERROR, FALSE, 2000, ERROR_TONE);
        return FALSE;
    }
    return TRUE;

}

	
/*****************************************************************************
* FUNCTION
*  ChatUseNumberScreen
* DESCRIPTION
*   Wrapper function to call function which extracts number from chat message
*
* PARAMETERS
*  none
* RETURNS
*  total use number record
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 ChatUseNumberScreen(void)
{
	U16 totalusenumber=ObtainChatNumbersFromMessageTextWrapper((char *)g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.destinationNo,GetCurrentChatRoomNo() );
	return totalusenumber;
}

/*****************************************************************************
* FUNCTION
*  TempleaveChatApp
* DESCRIPTION
*   Temp leave function from Chat Room screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void TempleaveChatApp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 screen_id;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (IsScreenPresent(CHAT_SCREEN_ID))
    {
        GoBackToHistory(CHAT_SCREEN_ID);
    }
    else if (IsScreenPresent(CHAT_SCREEN_ROOM_ID) && 
             GetPreviousScrnIdOf(CHAT_SCREEN_ROOM_ID, &screen_id))
    {
        GoBackToHistory(screen_id);
    }
    else
    {
        GoBackHistory();
    }
}

/*****************************************************************************
* FUNCTION
*  GoBackChatHistory
* DESCRIPTION
*   Wrapper function of Gobackhistory
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GoBackChatHistory(void)
{
	GoBacknHistory(1);
}

/*****************************************************************************
* FUNCTION
*  ExitChatConfirm
* DESCRIPTION
*   Exit Function of confirmation screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ExitChatConfirm(void)
{
	stopRequestedTone(WARNING_TONE);
}

/*****************************************************************************
* FUNCTION
*  GoBackQuitScreen
* DESCRIPTION
*   GoBack Quit Chat Room Screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GoBackQuitScreen(void)
{
	DeleteScreenIfPresent(CHAT_OPTION_SCREEN_ID_2);
	DeleteScreenIfPresent(CHAT_ROOM_SCREEN_ID);
	DeleteScreenIfPresent(CHAT_SCREEN_ROOM_ID);
}

/*****************************************************************************
* FUNCTION
*  QuitChatRoom
* DESCRIPTION
*   This funtion quits from Chat room and goback to previous screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void QuitChatRoom(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EndChatRoom();
    DisplayPopup(
        (PU8) GetString(STR_GLOBAL_DONE),
        IMG_GLOBAL_ACTIVATED,
        FALSE,
        MESSAGES_POPUP_TIME_OUT,
        (U8) SUCCESS_TONE);
    GoBackQuitScreen();
}
	
/*****************************************************************************
* FUNCTION
*  EndChatRoom
* DESCRIPTION
*   This funtion Reset all Chat room variables and make it disable
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EndChatRoom(void)
{
	S32 msgCount;
	S16 index=GetCurrentChatRoomNo();
	if(GetMessagechatRoomNo()==index)
	{
				gIsItChatSmsArrivalInd=FALSE;
				gIsThereAnyNewChatSMSToInd=FALSE;
	}
	g_chat_cntx.chatRoomInfo[index].status=DISABLE;
		ResetMemoryBuffer(index);
	for(msgCount=0;msgCount<g_chat_cntx.chatRoomInfo[index].chatRoomCounter;msgCount++)
		g_chat_cntx.chatRoomInfo[index].chatStrings[msgCount].chat_message_string=NULL;
	gChatInvitation=FALSE;
	ResetCounter(index);
	HideChatStatusIcon(index);
}

/*****************************************************************************
* FUNCTION
*  ClearChatRoom
* DESCRIPTION
*   This function displays Done pop up and Calls Clear function for Chat Room
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ClearChatRoom(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (PU8) GetString(STR_GLOBAL_DONE),
        IMG_GLOBAL_ACTIVATED,
        FALSE,
        MESSAGES_POPUP_TIME_OUT,
        (U8) SUCCESS_TONE);
    ClearChat(GetCurrentChatRoomNo());
    DeleteScreenIfPresent(CHAT_OPTION_SCREEN_ID_2);
}

/*****************************************************************************
* FUNCTION
*  ClearChat
* DESCRIPTION
*   This function clears Chat room by resseting all varaibles
*
* PARAMETERS
*  Chat Room Number
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ClearChat(S16 index)
{
	S32 msgCount;
	ResetMemoryBuffer(index);
	for(msgCount=0;msgCount<g_chat_cntx.chatRoomInfo[index].chatRoomCounter;msgCount++)
		g_chat_cntx.chatRoomInfo[index].chatStrings[msgCount].chat_message_string=NULL;
	
	ResetCounter(GetCurrentChatRoomNo());
}

/*****************************************************************************
* FUNCTION
*  ChatInboxInterface
* DESCRIPTION
*   This is an interface function when Chat is selected from Inbox
*
* PARAMETERS
*  Destination number
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ChatInboxInterface(PU8 destnum)
{
	S16 duplicateNumberStatus=0;
	S16 status=0;
	pfnUnicodeStrcpy((PS8)gDestinationNo, (PS8)destnum);
	while(status<MAX_NO_OF_CHAT_ROOM)
	{
		duplicateNumberStatus=CheckDuplicateNumber(destnum,status);	
		if(duplicateNumberStatus)
		{
			SetCurrentChatRoomNo(status);				
			if(g_chat_cntx.chatRoomInfo[status].status==DISABLE)
			{
				if(gChatInvitation) 
				{
					ActivateChatRoom(status);
					UpdateChatRoom(status,RECEIVING_CHAT);
					EntryChatRoom();
					DeleteInvitationMessage();
				}
				else
					EntryChatOption();
			}
			else
				EntryChatRoom();
			return;
		}
		status++;
	}
	status=CheckActiveChatRoom();
	if(status>=0)
	{		
		SetCurrentChatRoomNo(status);
		FillChatRoomInfo((PU8)g_chat_cntx.chatRoomInfo[status].chatInfo.nickName,destnum,status);
		if(gChatInvitation) 
		{
			ActivateChatRoom(status);
			UpdateChatRoom(status,RECEIVING_CHAT);
			EntryChatRoom();
			DeleteInvitationMessage();
		}
		else
			EntryChatOption();
		return;
	}
	else
		EntryChatReplaceConfirm();
}

/*****************************************************************************
* FUNCTION
*  CheckActiveChatRoom
* DESCRIPTION
*   This function returns Chat room number if not active else -1
*
* PARAMETERS
*  none
* RETURNS
*  chat room count
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S16 CheckActiveChatRoom(void)
{
	S16 chatRoomCount;
	for(chatRoomCount=0;chatRoomCount<MAX_NO_OF_CHAT_ROOM;chatRoomCount++)
	{
		if(g_chat_cntx.chatRoomInfo[chatRoomCount].status==DISABLE) 
			return chatRoomCount;
	}
	 return -1;
}

/*****************************************************************************
* FUNCTION
*  ReplaceChatRoom
* DESCRIPTION
*   This function Replaces first Chat Room with currently selected Chat Room
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ReplaceChatRoom(void)
{
		S16 status=0;
		S32 checkindex=(status==0)?1:0;	
		S16 duplicateNumberStatus=CheckDuplicateNumber(gDestinationNo,checkindex);
		if(duplicateNumberStatus)
		{
			
			DisplayPopup((PU8)GetString(CHATROOM_DUPLICATE_ERROR), IMG_GLOBAL_ERROR, FALSE, 2000, ERROR_TONE);
			DeleteScreenIfPresent(SCR_CHAT_REPLACE_QUERY);
				return ;
		}
		if(gChatInvitation) 
		{
			SetCurrentChatRoomNo(status);
			EndChatRoom();
			FillChatRoomInfo((PU8)g_chat_cntx.chatRoomInfo[status].chatInfo.nickName,gDestinationNo,status);
			ActivateChatRoom(status);
			UpdateChatRoom(status,RECEIVING_CHAT);
			EntryChatRoom();
			DeleteInvitationMessage();
		}
		else
		{
			history_t currHistory;
			historyNode *History;
			
			SetCurrentChatRoomNo(status);
			EndChatRoom();
			FillChatRoomInfo((PU8)g_chat_cntx.chatRoomInfo[status].chatInfo.nickName,gDestinationNo,status);
	
			if((GetHistoryScrID(CHAT_SCREEN_ROOM_ID,&History) == ST_FAILURE))
			{
					
				currHistory.scrnID = CHAT_SCREEN_ROOM_ID;
				currHistory.entryFuncPtr = EntryChatOption;
				memset ((S8*)currHistory.inputBuffer, 0, ENCODING_LENGTH);
				memset ((S8*)currHistory.guiBuffer, 0, ENCODING_LENGTH);
				
				AddHistory (currHistory);
				
			}
			EntryChatRoomInfo();
		}
		
		
}

/*****************************************************************************
* FUNCTION
*  GetChatMsgToOutBox
* DESCRIPTION
*   It get chat message to save to outbox
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GetChatMsgToOutBox(void)
{
	
	S16 nMessageCount;
	EMSData * pEms ;
	S8 newLineChar[4],endOfChar[4];
	S16 totalMsgSize=0,size=0,totalMsgCount;
	S16 maxBufferSize=MAX_MSG_SIZE+MAX_SMS_CHAT_HEADER_SIZE;
	S16 maxUcsLength=MAX_MSG_SIZE_UCS2;
	S16 roomIndex=GetCurrentChatRoomNo();
	totalMsgCount=GetNumOfMessage(roomIndex);
	if(totalMsgCount==0)
		return;
	AnsiiToUnicodeString((PS8)newLineChar,"\n");
	AnsiiToUnicodeString((PS8)endOfChar,"\0");

	ReleaseEMSEditBuffer();
	GetEMSDataForEdit (& pEms, 1);
	ResetCurrentPosition(pEms);
/**********************************UCS CHECK *******************************/
	for(nMessageCount=0;nMessageCount<totalMsgCount;nMessageCount++)
	{
	
		size =(S16 )(pfnUnicodeStrlen((S8 *)g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[nMessageCount].chat_message_string));
		if((totalMsgSize + size+1)<=maxBufferSize){
			if(GetUCS2Flag((PS8)g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[nMessageCount].chat_message_string))
			{
				
				
				if((totalMsgSize + size+1)<=maxUcsLength)
				{
					maxBufferSize=maxUcsLength;
					EMSSetDCS(pEms,SMSAL_UCS2_DCS);
				}
				else 
				{
						S16 UcsLength=0;
						S16 indexUcs=GetUCS2CharIndex((PS8)g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[nMessageCount].chat_message_string);
						if(indexUcs==-1)
							break;
						UcsLength=totalMsgSize+indexUcs+1;
						if(UcsLength<=maxBufferSize)
						{
							if(UcsLength<=maxUcsLength)
							{
								maxBufferSize=maxUcsLength;
								EMSSetDCS(pEms,SMSAL_UCS2_DCS);

							}
							else
							{
								maxBufferSize=UcsLength;

							}
						}
						
					}
				break;	
			}
			
		
			totalMsgSize+=size+1;
		}
		else
		{
			size=maxBufferSize-totalMsgSize-1;
			if(size>0)
			{
				if(GetUCS2Flag((PS8)g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[nMessageCount].chat_message_string))
				{
						S16 UcsLength=0;
						S16 indexUcs=GetUCS2CharIndex((PS8)g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[nMessageCount].chat_message_string);
						if(indexUcs==-1)
							break;

						UcsLength=totalMsgSize+indexUcs+1;
						
						if(UcsLength<=maxBufferSize)
							maxBufferSize=UcsLength;
						else
                            			maxBufferSize=totalMsgSize;
				}
				break;
			}
	}
	}
/****************************UCS END************************************/

	
	totalMsgSize=0;
	for(nMessageCount=0;nMessageCount<totalMsgCount;nMessageCount++){
		
		size =(S16 )(pfnUnicodeStrlen((S8 *)g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[nMessageCount].chat_message_string));
		if((totalMsgSize + size+1)<=maxBufferSize)
		{
			
			
			AddString(pEms,g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[nMessageCount].chat_message_string,size,NULL);
			
			AddString(pEms,(PU8)newLineChar,1,NULL);
		
			totalMsgSize+=size+1;
		}
		else
		{
			size=maxBufferSize-totalMsgSize-1;
			if(size>0)
			{
				
				AddString(pEms,g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[nMessageCount].chat_message_string,size,NULL);
				AddString(pEms,(PU8)newLineChar,1,NULL);
				g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[nMessageCount].chat_message_string+=size*ENCODING_LENGTH;
			}
			break;
		}
		
		
	}
	
	currentMsgCount=nMessageCount;
}

/*****************************************************************************
* FUNCTION
*  SaveChatMsgToOutBox
* DESCRIPTION
*   This  saves Chat Message to OutBox
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SaveChatMsgToOutBox(void)
{
	S16 roomIndex=GetCurrentChatRoomNo();
	GetChatMsgToOutBox();
	gOutBoxSaveFlag=TRUE;
	mmi_msg_set_phb_send_case(MMI_SAVE_CHAT);
	mmi_msg_get_number_from_phb(NULL, g_chat_cntx.chatRoomInfo[roomIndex].chatInfo.destinationNo);
}



/*****************************************************************************
* FUNCTION
*  AdjustMemoryPointers
* DESCRIPTION
*   This  Replaces Saved Pointers with renmaining chat data pointer
*
* PARAMETERS
*  none
* RETURNS
*  status
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void SaveChatMsg(void);
U8 AdjustMemoryPointers(void )
{
	S16 iCount,jCount,nCount,roomIndex;
	S16 remainingMsgCount;

	roomIndex=GetCurrentChatRoomNo();
	remainingMsgCount=g_chat_cntx.chatRoomInfo[roomIndex].chatRoomCounter - currentMsgCount;
	jCount=currentMsgCount;
	for(iCount=0;iCount<remainingMsgCount ;iCount++,jCount++)
	{
		   g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[iCount].chat_message_string=g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[jCount].chat_message_string;		   
		   g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[iCount].flag=g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[jCount].flag;
			
	}
	for(nCount=iCount;nCount<remainingMsgCount+currentMsgCount;nCount++)
		   g_chat_cntx.chatRoomInfo[roomIndex].chatStrings[nCount].chat_message_string=NULL;
	
	if((remainingMsgCount>=0))
	{
		   g_chat_cntx.chatRoomInfo[roomIndex].chatRoomCounter=remainingMsgCount;
		   ResetRoundedFlag(roomIndex);
	}

	else
		   ClearChat(roomIndex);
		
	if(GetNumOfMessage(roomIndex)>0)
	{
			GetChatMsgToOutBox();
			gOutBoxSaveFlag=TRUE;
		SaveChatMsg();
			return TRUE;
	}
	else 
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*  GetUCS2CharIndex
* DESCRIPTION
*   It returns UCS char position
*
* PARAMETERS
*  Data Buffer
* RETURNS
*  Ucs char position
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S16 GetUCS2CharIndex(PS8 buffer) 
{
	U8 index, bufferLen;
	S16 jCount;
	bufferLen = (U8)(pfnUnicodeStrlen(buffer)*ENCODING_LENGTH);
	jCount=-1;
	for(index = 1; index< bufferLen; index += 2)
	{
		jCount++;
	   	if(buffer[index] != 0x00)
		  return jCount;
	}
	return -1;   
}

#endif


