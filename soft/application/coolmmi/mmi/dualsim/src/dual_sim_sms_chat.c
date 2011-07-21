#ifndef __DUAL_SIM_SMS_CHAT_C__
#define __DUAL_SIM_SMS_CHAT_C__

#include "mmi_features.h"

#ifdef __MMI_MULTI_SIM__

#ifdef __MMI_MESSAGES_CHAT__

#include "globaldefs.h"
#include "globalscrenum.h"
#include "frameworkstruct.h"
#include "gui_data_types.h"
#include "eventsgprot.h"
#include "mmi_msg_struct.h"
#include "simdetectiondef.h"
#include "simdetectionhwmapfile.h"
#include "debuginitdef.h"
#include "wgui_status_icons.h"

#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalconstants.h"
#include "debuginitdef.h"	/*debug info*/
#include "protocolevents.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_categories_cm.h"
#include "wgui_categories_inputs.h"
#include "wgui_status_icons.h"
#include "allappgprot.h"
#include "phonebookdef.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "phonebooktypes.h"

#include "callsdefs.h"
#include "settingstructs.h"
#include "settinggprots.h"
#include "simdetectiongprot.h"
#include "protocolevents.h"
#include "callfixeddial.h"
#include "barreddiallist.h"
#include "settingprofile.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "commonscreens.h"
#include "mmi_features.h"
#include "callsetupenum.h"
#include "settingprot.h"
#include "callmanagementiddef.h"
#include "networksetupgprot.h"
#include "organizergprot.h"
#include "idleappdef.h"
#include "idleappprot.h"
#include "simdetectiongexdcl.h"
#include "settingsgexdcl.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "securitysetup.h"
#include "callmanagementgprot.h"
#include "protocolevents.h"
#include "smsapi.h"
#include "smsstruct.h"
#include "wgui_status_icons.h"
#include "smsguiinterfacetype.h"
#include "mmi_msg_context.h"
#include"historygprot.h"
#include "smsguiinterfaceprot.h"
#include "smsfunc.h"
#include "phonebookgprot.h"
#include "commonscreens.h"
#include "settingprofile.h"

#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"

#include "mtpnp_ad_resdef.h"
#include "messagesresourcedata.h"
#include "messagesexdcl.h"
#include "cphsprotocolhandlers.h"
#include "incomingcallmanagementprot.h"
#include "mainmenudef.h"
#include "statusiconres.h"
#include "dual_sim_call.h"
#include "dual_sim.h"
#include "unit_test.h"
#include "callstructuremanagementprot.h"
#include "csp.h"
#include "gbhmi.h"
#include "alarmframeworkprot.h"
#include "callsetup.h"
#include "callhistorygprots.h"
#include "wgui_categories_idlescreen.h"
#include "chatgtype.h"
#include "chatappprot.h"
#include "chatappgprot.h"
#include "chatappdef.h"
#include "dual_sim_sms_chat.h"



static void mmi_msg_chat_dual_mode_start_menu_handle(void);
static void mmi_msg_chat_select_menu_handle(void);

extern void HighlightChatMultiStartMenu(void);

static E_MTPNP_AD_CHANNELS g_chat_channel[MAX_NO_OF_CHAT_ROOM];

static chat_dual_mode_select_entry_state_enum dual_mode_select_enrty_state;

extern chat_context_struct g_chat_cntx;


void MTPNP_PFAL_SetDualmode_Chat_Handler(void)
{
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
    		SetHiliteHandler(MENU_MTPNP_CHARSTART_MASTER_NEW + i, HighlightChatMultiStartMenu);
	}
}

void MTPNP_PFAL_Set_Chat_Resume_Channel(E_MTPNP_AD_CHANNELS channel, MTPNP_INT16 chat_room)
{
	g_chat_channel[chat_room] = channel;
}

E_MTPNP_AD_CHANNELS MTPNP_PFAL_Get_Chat_Resume_Channel(MTPNP_INT16 chat_room)
{
	return g_chat_channel[chat_room];
}


void mmi_msg_chat_set_dual_mode_select_state(chat_dual_mode_select_entry_state_enum state)
{
	dual_mode_select_enrty_state = state;
}


static void mmi_msg_chat_dual_mode_start_menu_handle(void)
{
	if (!mmi_ucs2strlen((PS8) g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.destinationNo))
	{
		DisplayPopup((PU8) GetString(CHATROOM_EMPTY_ERROR), IMG_GLOBAL_WARNING, FALSE, 2000, WARNING_TONE);
	}
	else
	{
		mmi_msg_chat_set_dual_mode_select_state(MMI_CHAT_SELECT_START_NEW_MENU);
		EntryChooseChatScreen();
	}
}


void HighlightChatDualmodeStartMenu(void)
{
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    SetLeftSoftkeyFunction(mmi_msg_chat_dual_mode_start_menu_handle, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(mmi_msg_chat_dual_mode_start_menu_handle, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


static void mmi_msg_chat_select_menu_handle(void)
{
	U8 reqSrcMod;
	MTPNP_INT16 curr_room_no = GetCurrentChatRoomNo();

	reqSrcMod = GetReqSrcModIndex();

	MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
	MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, reqSrcMod);
	MTPNP_PFAL_Set_Chat_Resume_Channel(reqSrcMod, curr_room_no);

	switch(dual_mode_select_enrty_state)
	{
		case MMI_CHAT_SELECT_START_NEW_MENU:
			memset(g_chat_cntx.writeChatBuffer, 0, (MAX_MSG_SIZE + 1) * ENCODING_LENGTH);
			EntryChatWrite();
			break;

		case MMI_CHAT_SELECT_SMS_CHAT_MENU:
			ActivateChatRoom(curr_room_no);
			UpdateChatRoom(curr_room_no, RECEIVING_CHAT);
			EntryChatRoom();
			DeleteInvitationMessage();
			break;

		default:
			MMI_ASSERT(0);
			break;
	}
}

void HighlightChatMultiStartMenu(void)
{
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

    SetLeftSoftkeyFunction(mmi_msg_chat_select_menu_handle, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(mmi_msg_chat_select_menu_handle, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

void EntryChooseChatScreen(void)
{
    MTPNP_UINT16 nStrItemList[MAX_SUB_MENUS];   /* Stores the strings id of submenus returned */
    MTPNP_UINT16 nNumofItem;    /* Stores no of children in the submenu */
    MTPNP_UINT16 nDispAttribute;        /* Stores display attribue */
    MTPNP_UINT8 *guiBuffer;     /* Buffer holding history data */
    MTPNP_UINT8 *ItemType[MAX_SUB_MENUS];

    /* 1 Call Exit Handler */
    EntryNewScreen(SCR_CHOOSE_CHAT, MTPNP_NULL, EntryChooseChatScreen, MTPNP_NULL);

    /* 2 Get current screen to gui buffer  for history purposes */
    guiBuffer = GetCurrGuiBuffer(SCR_CHOOSE_CHAT);

    MTPNP_AD_SetMenuHideUnhide(MENU_MTPNP_CHARSTART_MASTER_NEW);
    /* 3. Retrieve no of child of menu item to be displayed */
     
    nNumofItem = GetNumOfChild_Ext(MENU_START_NEW);

    /* 4. Get attribute of menu to be displayed */
    nDispAttribute = GetDispAttributeOfItem(MENU_START_NEW);

    /* 5. Retrieve string ids in sequence of given menu item to be displayed */
    GetSequenceStringIds_Ext(MENU_START_NEW, nStrItemList);

    /* 6 Set current parent id */
    SetParentHandler(MENU_START_NEW);

    /* 7 Register highlight handler to be called in menu screen */
    RegisterHighlightHandler(MultiSimMenuHighLightHandler);

    /* 8 Construct the Hints List */
    ConstructHintsList(MENU_START_NEW, ItemType);

    ShowCategory52Screen(STRING_MTPNP_SETTING, CHAT_IMAGE_APP_ID,
                         STR_GLOBAL_OK, IMG_GLOBAL_OK,
                         STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
                         nNumofItem, nStrItemList, (MTPNP_UINT16 *) gIndexIconsImageList, ItemType, 0, 0, guiBuffer);

    /* 9.Register function with right softkey */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void EntryChatNotSupportScreen(void)
{
    DisplayPopup((MTPNP_UINT8 *) GetString(STRING_MTPNP_CHAT_NOT_SUPPORT),
                 IMG_GLOBAL_ERROR, 0, ST_NOTIFYDURATION, ERROR_TONE);
}

void MTPNP_PFAL_Refresh_StatusBar_Chat_Display(E_MTPNP_AD_STATUS status)
{
    MTPNP_INT16 i;

	kal_prompt_trace(MOD_MMI, "MTPNP_PFAL_Refresh_StatusBar_Chat_Display");

	switch(status)
	{
		case MTPNP_AD_FLIGHT:
	    	for (i = 0; i < MAX_NO_OF_CHAT_ROOM; i++)
	    	{
	    		if (g_chat_cntx.chatRoomInfo[i].status == MMI_MSG_CHAT_ENABLE)
	    		{
					SetCurrentChatRoomNo(i);
					EndChatRoom();
	    		}
	    	}
			break;
			
		case MTPNP_AD_CARD1:
			{
				MTPNP_INT16 preIndex = GetCurrentChatRoomNo();
				
		    	for (i = 0; i < MAX_NO_OF_CHAT_ROOM; i++)
		    	{
		    		if ((g_chat_cntx.chatRoomInfo[i].status == MMI_MSG_CHAT_ENABLE) &&
						(MTPNP_PFAL_Get_Chat_Resume_Channel(i) == MTPNP_AD_CHANNEL_SLAVE))
		    		{
						SetCurrentChatRoomNo(i);
						EndChatRoom();
		    		}
		    	}
				SetCurrentChatRoomNo(preIndex);
			}
			break;
			
		case MTPNP_AD_CARD2:
			{
				MTPNP_INT16 preIndex;
				preIndex = GetCurrentChatRoomNo();

		    	for (i = 0; i < MAX_NO_OF_CHAT_ROOM; i++)
		    	{
		    		if ((g_chat_cntx.chatRoomInfo[i].status == MMI_MSG_CHAT_ENABLE) &&
						(MTPNP_PFAL_Get_Chat_Resume_Channel(i) == MTPNP_AD_CHANNEL_MASTER))
		    		{
						SetCurrentChatRoomNo(i);
						EndChatRoom();
		    		}
		    	}
				SetCurrentChatRoomNo(preIndex);
			}
			break;
			
		case MTPNP_AD_DUALCARD:
		default:
			break;
	}
}

#endif
#endif
#endif
