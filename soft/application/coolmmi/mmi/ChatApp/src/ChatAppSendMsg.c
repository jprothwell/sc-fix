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
 * ChatAppSendMsg.C
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: ChatAppSendMsg.c

  	PURPOSE		: Chat application 

 

	 

	DATE		: Feb 18,04

**************************************************************/
#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "mmi_trace.h"
#ifdef __MMI_MESSAGES_CHAT__
#ifndef _CHAT_APPSENDMSG_
#define _CHAT_APPSENDMSG_
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

#include "eventsgprot.h"
#include "historygprot.h"
#include "globalmenuitems.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "wgui_categories.h"
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

#include "messagesexdcl.h"
#include "messagesmiscell.h"
#include "smsguiinterfaceprot.h"
#include "protocolevents.h"
#include "gui_ems.h"
#include "wgui_status_icons.h"
#include "messagesresourcedata.h"
#include "settingprofile.h"
#include "gpioinc.h"
#include "asciiprot.h"
#include "keybrd.h"		// for clearkeyevents
#include "alarmframeworkprot.h"

#undef __NEWSIMULATOR
#include "l4dr.h"
#include "customer_ps_inc.h"
#include "mmi_msg_context.h"
#include "smsapi.h"
#include "smsstruct.h"
#include "smsfunc.h"

#include "dual_sim.h"
#include "dual_sim_sms_chat.h"

/****************Static Variable Start*********/
static U16 gCurrScrnNewChatSmsInd=0;
static S8 gHeader[3*ENCODING_LENGTH]; //={": "};
static S8 gInvitation[4*ENCODING_LENGTH]; //={":-)"};
static U16 gNewChatMsgScreenToDeleteFromHistory;
static pBOOL gChatMsgIndPresentForKeypadLock;
static U32 gChatInboxIndex;

#ifdef __MMI_MULTI_SIM__
static E_MTPNP_AD_CHANNELS chat_new_msg_channel;
#endif 

/****************Static Variable End*********/
/****************Global Variable Start*********/
extern pBOOL gChatInvitation;
U8 gIsItChatSmsArrivalInd;
U8 gIsThereAnyNewChatSMSToInd;
pBOOL gChatRoomScreenPresent;
/****************Global Variable End*********/

/****************Function Start*********/
/****************Function End*********/

/***********Extern Variable Start*************/
extern chat_context_struct g_chat_cntx;
extern U16 gChatMsgFlag;
extern pBOOL gBlinkChatFlag[MAX_NO_OF_CHAT_ROOM];
extern pBOOL gInputBoxUcs2Flag;
/***********Extern Variable End*************/
extern PS8 lookUpNumber (PS8 number);
extern BOOL GetUCS2Flag(S8*);
extern EMSData * GetEMSDataForView              (EMSData ** p, U8 force) ;
extern EMSData * GetEMSDataForEdit              (EMSData ** p, U8 force) ;
extern U8        DeInitMessagesEMSBuffer        (msg_ems_data_struct * pE);
extern void GetNewChatMsgIndDisplayString (U8 *number, U8 length, U8 type);

/******************/
/*Handle Chat Header*/
/******************/
/*****************************************************************************
* FUNCTION
*  InitChatHeader
* DESCRIPTION
*   Init chat header string
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void InitChatHeader (void)
{
		AnsiiToUnicodeString((PS8)gHeader,": ");
		AnsiiToUnicodeString((PS8)gInvitation,":-)");
}

/*****************************************************************************
* FUNCTION
*  GetHeader
* DESCRIPTION
*   It Gets Header string
*
* PARAMETERS
*  Header String and Chat Room Number
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GetHeader(PU8 dest,S16 index)
{

	pBOOL flag=FALSE;

	if(g_chat_cntx.chatRoomInfo[index].chatRoomCounter==0)
{
		
		pfnUnicodeStrcpy((PS8)dest,gInvitation);
		flag=TRUE;
		

	}
	if((pfnUnicodeStrlen((PS8)g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.nickName))>0)
	{
        if (flag == TRUE)
        {
            pfnUnicodeStrcat((PS8) dest, g_chat_cntx.chatRoomInfo[index].chatInfo.nickName);
        }
        else
        {
            pfnUnicodeStrcpy((PS8) dest, g_chat_cntx.chatRoomInfo[index].chatInfo.nickName);
        }
        pfnUnicodeStrcat((PS8) dest, gHeader);

    }
    else if (flag == TRUE)
    {
        pfnUnicodeStrcat((PS8) dest, gHeader);
    }
    else
    {
        pfnUnicodeStrcpy((PS8) dest, gHeader);
    }
}

/*****************************************************************************
* FUNCTION
*  RemoveHeaderInfo
* DESCRIPTION
*   It Removes header info from message
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void RemoveHeaderInfo(void)
{
	U8		aHeader[(MAX_SMS_CHAT_HEADER_SIZE+1)*ENCODING_LENGTH];
	S16		nStrLen;
	
	GetHeader(aHeader,GetCurrentChatRoomNo());
	nStrLen = (S16)pfnUnicodeStrlen((PS8)aHeader);
	DeletenCharFromEMSBuffer(nStrLen);
}

/*****************************************************************************
* FUNCTION
*  AddHeadertoMessage
* DESCRIPTION
*   It adds Message header to Chat Message
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void AddHeadertoMessage(void)
{
	EMSData * pEms ;
	U8		aHeader[(MAX_SMS_CHAT_HEADER_SIZE+1)*ENCODING_LENGTH];
	S32		nStrLen;

	GetEMSDataForEdit (& pEms, 0);
	ResetCurrentPosition(pEms);
	GetHeader(aHeader,GetCurrentChatRoomNo());
	nStrLen = pfnUnicodeStrlen((PS8)aHeader);
	AddString(pEms,aHeader,(U16)(nStrLen*2),NULL); //check length
}

	
/*****************************************************************************
* FUNCTION
*  CopyToBuffer
* DESCRIPTION
*   It Copies Chat Room Data to Chat Buffer
*
* PARAMETERS
*  Chat Message Buffer,Message length, Chat room Number
* RETURNS
*  Buffer pointer
* GLOBALS AFFECTED
*   none
*****************************************************************************/
PU8 CopyToBuffer(U8 packet[],S32 length,S16 index, U8 flag)
{
  
	S8 temp[MAX_DIGITS*ENCODING_LENGTH];
	S8* name;
  	PU8 pointer;
  	U32	nLimiter	= 0;
  	S32 prelength = 0;
	S32 len;
	BOOL noNickname = TRUE;
	BOOL headerAtFirst = FALSE;
	#if 0 //wangzl: 090707 modify for bug13058 and 13073
	len=pfnUnicodeStrlen((S8*)gHeader);
	if(length >= len)
	{
		if (pfnUnicodeStrncmp((PS8)packet,gHeader,len)==0)
			noNickname = TRUE;		
	}
	#else
	S16 i;
	len = pfnUnicodeStrlen((PS8)gHeader);
	if(length>len)
	{
		if(pfnUnicodeStrncmp((PS8)packet, gHeader, len) == 0)
			headerAtFirst = TRUE;
		if(!headerAtFirst)
		{
			for(i = 1; i<MAX_SMS_CHAT_HEADER_SIZE && i<length;i++)
			{
				if(pfnUnicodeStrncmp((PS8)(packet+i*2),gHeader,len)==0)
				{
					noNickname = FALSE;
					break;
				}
			}
		}
	}
	#endif
	mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) CopyToBuffer, noNickname is:%d", noNickname);
  	if(noNickname)
  	{
  		if(flag==SENDING_CHAT)
			pfnUnicodeStrcpy(temp, GetString(CHATROOM_LOCAL_DEFAULT));
  		else
  		{
		name = lookUpNumber ((S8*)g_chat_cntx.chatRoomInfo[index].chatInfo.destinationNo);
		if(pfnUnicodeStrlen(name))
			pfnUnicodeStrcpy((S8*)temp, name);
		else
			pfnUnicodeStrcpy((S8*)temp, (S8*)g_chat_cntx.chatRoomInfo[index].chatInfo.destinationNo);
		//wangzl: 090707 add for bug13058 and 13073
		if(!headerAtFirst)
			pfnUnicodeStrcat(temp, gHeader);
		//wangzl: 090707 add
  		}
  		prelength=(pfnUnicodeStrlen(temp))*ENCODING_LENGTH;
  	}
	pointer=AllocMemory(prelength+length+2,index);
	if(pointer!=NULL)
	{ /*return NULL*/
		if(prelength)
  			memcpy(pointer, temp, prelength);
		memcpy((pointer+prelength),packet,length);
		memcpy((pointer+prelength+length),&nLimiter,ENCODING_LENGTH);
	}
	
	return pointer;	
}

/*****************************************************************************
* FUNCTION
*  DeletenCharFromEMSBuffer
* DESCRIPTION
*   It delets chars from EMS buffer
*
* PARAMETERS
*  Total number of chars to be deleted
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DeletenCharFromEMSBuffer(S16 count)
{
	EMSData * pEms ;
	GetEMSDataForEdit (& pEms, 0);
	CancelCurrentPosition(pEms,(kal_uint16)(count+1));
	GoToEndPosition(pEms);
}

kal_uint16 CSD_CancelCurrentPosition(EMSData *data, kal_uint16 steps) //guojian added for funtion RemoveInvitationChar
{

	/*Guojian Add Start For 10467 Ver: GreenStone  on 2008-11-25 16:7 */
	if (data->dcs == SMSAL_UCS2_DCS)
	{
		data->textBuffer = data->textBuffer + (steps - 1) * 2;
		data->textLength -= (steps-1)*2;
	}
	else
	{
		data->textBuffer = data->textBuffer + steps - 1;
		data->textLength -= steps-1;
	}
	/*Guojian Add End  For 10467 Ver: GreenStone  on 2008-11-25 16:7 */
	
	return 1;
}


/*****************************************************************************
* FUNCTION
*  RemoveInvitationChar
* DESCRIPTION
*   It removes invitation chars from message
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void RemoveInvitationChar(void)
{
	EMSData * pEms ;
	S16 len=(S16)pfnUnicodeStrlen(gInvitation);
	GetEMSDataForView (& pEms, 0);
	ResetCurrentPosition(pEms);
	ForwardCurrentPosition(pEms,len);
/*Guojian Modify Start For 10467 Ver: GreenStone  on 2008-12-1 10:15 */
//	CancelCurrentPosition(pEms,(kal_uint16)(len+1));
	CSD_CancelCurrentPosition(pEms,(kal_uint16)(len+1));
/*Guojian Modify End  For 10467 Ver: GreenStone  on 2008-12-1 10:15 */
}
	
/*****************************************************************************
* FUNCTION
*  SetChatInboxIndex
* DESCRIPTION
*   Sets invition message index from inbox index
*
* PARAMETERS
*  Inbox message index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SetChatInboxIndex(U32 index)
{
	gChatInboxIndex=index;
}

/*****************************************************************************
* FUNCTION
*  GetChatInboxIndex
* DESCRIPTION
*   It returns index of message
*
* PARAMETERS
*  none
* RETURNS
*  chat invitation index
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U32 GetChatInboxIndex(void)
{
	return gChatInboxIndex;
}


/*****************/
/*Receive Chat Msg*/
/*****************/
#ifdef __MMI_MULTI_SIM__
/*****************************************************************************
 * FUNCTION
 *  mmi_msg_chat_get_new_msg_channel
 * DESCRIPTION
 *  Get the new message channel(master or slave)
 * PARAMETERS
 *  void
 * RETURNS
 *  E_MTPNP_AD_CHANNELS
 *****************************************************************************/
static E_MTPNP_AD_CHANNELS mmi_msg_chat_get_new_msg_channel(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/


	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	return chat_new_msg_channel;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_msg_chat_set_new_msg_channel
 * DESCRIPTION
 *  Set the new message channel
 * PARAMETERS
 
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_msg_chat_set_new_msg_channel(U16 msg_index)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/


	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	chat_new_msg_channel = mmi_frm_msg_get_simid_by_l4_index(msg_index);
}
#endif 
/*****************************************************************************
* FUNCTION
*  CheckForChatNumber
* DESCRIPTION
*   It Checks whether a new message is Chat room message or not
*
* PARAMETERS
*  Destination number,number length,type
* RETURNS
*  Status flag[0/1]
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 CheckForChatNumber (PU8 number, U8 len, U8 type)
{
	S16 i;
	U8 num1[44];
	
	
	for(i=0;i<MAX_NO_OF_CHAT_ROOM;i++)
	{
		UnicodeToAnsii((PS8)num1, (PS8)g_chat_cntx.chatRoomInfo[i].chatInfo.destinationNo);
		
		num1[42]='\0';
		num1[43]='\0';
		
		if((ComparePhoneNum((PU8)num1,(PU8) number))&&(g_chat_cntx.chatRoomInfo[i].status==ENABLE))
		{
			#ifdef __MMI_MULTI_SIM__
			if (MTPNP_PFAL_Get_Chat_Resume_Channel(i) == mmi_msg_chat_get_new_msg_channel())
			#endif 
			{
			SetMessagechatRoomNo(i);
			return 1;
			}
		}
}
return 0;
}

/*****************************************************************************
* FUNCTION
*  CheckForChatMessage
* DESCRIPTION
*   It Checks whether a new message is Chat room message or not
*
* PARAMETERS
*  Destination number,number length,type
* RETURNS
*  Status flag[0/1]
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 CheckForChatMessage(void *data, int withobject, void *content)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_frm_sms_deliver_msg_struct *msginfo = (mmi_frm_sms_deliver_msg_struct*) data;
    U8 number[MSG_NUM_LEN];
    U8 addrlen = 0;
    U8 type = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_SMS, "MMI_SMS: (Chat) CheckForChatMessage");
    /* the message with ems object or smart message should not be treated as a chat message */
    if ((withobject) || (msginfo->dcs == SMSAL_8BIT_DCS))
    {
        return FALSE;
    }

    /* incomplete message is not allowed to be dispatched to chat room */
    if (!mmi_frm_sms_check_awaited_complete_msg(msginfo->msg_data[0]))
    {
        return FALSE;
    }
#ifdef __MMI_MULTI_SIM__
    mmi_msg_chat_set_new_msg_channel(msginfo->index);
#endif 
    memset((S8*) number, 0, MSG_NUM_LEN);
    addrlen = (msginfo->addr_number.length > MSG_NUM_LEN) ? MSG_NUM_LEN : msginfo->addr_number.length;
    if (msginfo->addr_number.type == CSMCC_INTERNATIONAL_ADDR)
    {
        number[0] = '+';
        type = 1;
    }
    memcpy(number + type, msginfo->addr_number.number, addrlen);
    /* chat message will dispatch to chatroom only if the current screen is chatroom or the chatroom screen is in the history */
    if (GetMessagesCurrScrnID() == CHAT_ROOM_SCREEN_ID || IsScreenPresent(CHAT_ROOM_SCREEN_ID))
    {
        /* if (CheckForChatNumber(number, addrlen, type) && CheckForInvitationMsg((U8*)unicodecontent, length, FALSE)) */
        if (CheckForChatNumber(number, addrlen, type))
        {
            if (type == 1)
            {
                GetNewChatMsgIndDisplayString(number, (U8) (addrlen + 1), type);
            }
            else
            {
                GetNewChatMsgIndDisplayString(number, addrlen, type);
            }
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************
* FUNCTION
*  CheckForInvitationMsg
* DESCRIPTION
*   It checks for invitation function
*
* PARAMETERS
*  Chat room Buffer data,length and invitation flag
* RETURNS
*  Status
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL CheckForInvitationMsg(PU8 data,S32 length,pBOOL flag)
{
	S32 len;
	mmi_trace(g_sw_SMS, "MMI_SMS: (Chat) CheckForInvitationMsg");
	len=pfnUnicodeStrlen((S8*)gInvitation);
	if(length>0)
	{
		if (pfnUnicodeStrncmp((PS8)data,gInvitation,len)==0)
		{
			gChatInvitation=flag; 
			return TRUE;
		}
		else 
			return FALSE;
	}
	else 
		return FALSE;
}
	
/*****************************************************************************
* FUNCTION
*  DeleteInvitationMessage
* DESCRIPTION
*   Deletes invitation chars from Chat Invitation message
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DeleteInvitationMessage(void)
{
	S16 index=GetCurrentChatRoomNo();
	gChatMsgFlag=1;
	DeleteChatMsg((S32)GetChatInboxIndex());
	gChatInvitation=FALSE;
	g_chat_cntx.chatRoomInfo[index].status=ENABLE;
	ShowChatStatusIcon(index);
	
}
	
	
/*****************************************************************************
* FUNCTION
*  DisplayChatPopUpWindow
* DESCRIPTION
*   It Displays Chat Pop Up on idle screen and adds message to Chat room
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DisplayChatPopUpWindow(void)
{
	gChatMsgIndPresentForKeypadLock=FALSE;
	gIsItChatSmsArrivalInd=TRUE;
	gIsThereAnyNewChatSMSToInd=TRUE;
	PlayMessageArrivalTone();
	TurnOnBacklight(1);

 	mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) DisplayChatPopUpWindow 000");
	if(GetMessagesCurrScrnID()==CHAT_ROOM_SCREEN_ID)
	{
		if(GetMessagechatRoomNo()!=GetCurrentChatRoomNo())
			BlinkChatIcon(GetMessagechatRoomNo());
	}
	else
	{
		BlinkChatIcon(GetMessagechatRoomNo());
	}


	if (IsMMIInIdleState () || IsNewChatSmsIndScrn ()) 
	{
		ClearKeyEvents ();	// key flussing

		MMI_ASSERT(g_idle_context.IsOnIdleScreen!=1  &&  g_idle_context.ScreenSaverRunFlag!=1);
		MMI_ASSERT(gCurrScrnNewChatSmsInd!=SCR_NEWCHATMSGARRIVALNOTIFICATION);

		if((IsScreenPresent(SCR_NEWCHATMSGARRIVALNOTIFICATION) == TRUE) || IsNewChatSmsIndScrn())
			gNewChatMsgScreenToDeleteFromHistory = 1;
		if(IsKeyPadLockState ())
		{
			gChatMsgIndPresentForKeypadLock = TRUE;
			EntryIdleScreen (); 
			
		}
		else
			EntryNotifyNewChatMessageArrival(); 
	}
	else
	{
		 mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) DisplayChatPopUpWindow 111");
		if(GetMessagesCurrScrnID()==CHAT_ROOM_SCREEN_ID)
		{ 
			 mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) DisplayChatPopUpWindow 222");
			if(GetMessagechatRoomNo()==GetCurrentChatRoomNo())
			{
         			mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) DisplayChatPopUpWindow 333");
				gIsItChatSmsArrivalInd=FALSE;
				gIsThereAnyNewChatSMSToInd=FALSE;
				EntryChatRoom();
			}
		}
		
	}
	
}


/*****************************************************************************
* FUNCTION
*  DisplayChatPopUpWindow
* DESCRIPTION
*   It Displays Chat Pop Up on idle screen and adds message to Chat room
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void mmi_msg_get_msg_newchat (void);
extern  void mmi_msg_get_msg_rsp(void* data, module_type mod, U16 result);

#include "smsguiinterfacetype.h"
U8 HandleChatMessage(void *data, void *content)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_SMS, "MMI_SMS: (Chat) HandleChatMessage");
    ModifyChatRoom(GetMessagechatRoomNo(), RECEIVING_CHAT, (void*)data, (S8*) content);
    DisplayChatPopUpWindow();
    return FALSE;
}


/*****************************************************************************
* FUNCTION
*  IsNewChatSmsIndScrn
* DESCRIPTION
*   Checks wheather new pop Chat message screen is on idle screen or not
*   Currently no use, but keep for future implementation when idle receiving chat message
* PARAMETERS
*  none
* RETURNS
*  status[0/1]
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 IsNewChatSmsIndScrn(void)
{
	return (gCurrScrnNewChatSmsInd==SCR_NEWCHATMSGARRIVALNOTIFICATION)?1:0;
}
 
/*****************************************************************************
* FUNCTION
*  EntryNotifyNewChatMessageArrival
* DESCRIPTION
*   It displays pop up for new Chat message received
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryNotifyNewChatMessageArrival(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_NEWCHATMSGARRIVALNOTIFICATION, ExitNotifyNewChatMessageArrival, NULL, NULL);
    if (gNewChatMsgScreenToDeleteFromHistory)
    {
        DeleteNScrId(SCR_NEWCHATMSGARRIVALNOTIFICATION);
    }
    gCurrScrnNewChatSmsInd = SCR_NEWCHATMSGARRIVALNOTIFICATION;

    ShowCategory154Screen(
        CHATROOM_NEW_MSG_RCVD,
        CHAT_IMAGE_APP_ID,
        STR_GLOBAL_CHAT,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_OK,
        (PU8) GetString(STR_CHAT_FROM),
        (PU8) GetNewChatSmsIndDisplayStr(),
        IMG_NEW_MESSAGE_NOTIFICATION_MSG,
        NULL);
    SetRightSoftkeyFunction(GoBackHistoryFromNewChatMessageScreen, KEY_EVENT_UP);
    SetLeftSoftkeyFunction(NotifymessageLeftSoftKeyFunction, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistoryFromNewChatMessageScreenForEndKey, KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  ExitNotifyNewChatMessageArrival
* DESCRIPTION
*   Exit screen of new message arrival
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ExitNotifyNewChatMessageArrival(void)
{
	gCurrScrnNewChatSmsInd=0;
}

/*****************************************************************************
* FUNCTION
*  GoBackHistoryFromNewChatMessageScreen
* DESCRIPTION
*   Wrapper function for Calling GoBackHistory function
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GoBackHistoryFromNewChatMessageScreen(void)
{
	gCurrScrnNewChatSmsInd=0;
	gIsItChatSmsArrivalInd=FALSE;
	gIsThereAnyNewChatSMSToInd=FALSE;
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  NotifymessageLeftSoftKeyFunction
* DESCRIPTION
*   Goes to Chat room from Idle screen when Chat is selected
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void NotifymessageLeftSoftKeyFunction(void)
{
	gIsThereAnyNewChatSMSToInd=FALSE;
	SetCurrentChatRoomNo(GetMessagechatRoomNo());
	EntryChatRoom();
}

/*****************************************************************************
* FUNCTION
*  GoBackHistoryFromNewChatMessageScreenForEndKey
* DESCRIPTION
*   Wrapper function for calling EndKey functionality
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GoBackHistoryFromNewChatMessageScreenForEndKey(void)
{
	gNewChatMsgScreenToDeleteFromHistory = 0;
	gIsThereAnyNewChatSMSToInd=FALSE;
	DisplayIdleScreen();
}

/*****************************************************************************
* FUNCTION
*  IsThereAnyNewChatMessage
* DESCRIPTION
*   Returns whether there is new Chat message or not
*
* PARAMETERS
*  none
* RETURNS
*  status
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 IsThereAnyNewChatMessage(void)
{
	return gIsThereAnyNewChatSMSToInd;
}

/*****************************************************************************
* FUNCTION
*  NewChatMsgIndPresentForKeypadLock
* DESCRIPTION
*   It returns Keypad state when new Chat Messsage arrives
*
* PARAMETERS
*  none
* RETURNS
*  status
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL NewChatMsgIndPresentForKeypadLock(void)
{
	return gChatMsgIndPresentForKeypadLock;
}

/*****************************************************************************
* FUNCTION
*  DisplayNewChatMsgINDAfterKeypadUnlocked
* DESCRIPTION
*   It Sets flag for new chat message when keypad is unlocked
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DisplayNewChatMsgINDAfterKeypadUnlocked (void)
{
	gChatMsgIndPresentForKeypadLock=FALSE;
	EntryNotifyNewChatMessageArrival(); 
}

/*****************************************************************************
* FUNCTION
*  ModifyChatRoom
* DESCRIPTION
*   It modifies Chat room upon arrival of new message and deletes from Inbox
*
* PARAMETERS
*  Chat Room Number and Receiving /Sending flag
* RETURNS
*  status
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern U16 current_chat_sms_index;
void mmi_frm_sms_delete_chat_sms (PsFuncPtrU16 callback, module_type mod_src, U8 type, U16 index);
pBOOL ModifyChatRoom(U16 index, U8 flag, void* data, S8* content)
{
	mmi_frm_sms_deliver_msg_struct* msgentry= (mmi_frm_sms_deliver_msg_struct*)data;
	U16 length = 0;//msgentry->no_msg_data; /* it is not the actual length of the data, ex. the length of 706B is 2, not 1 */
	S16 len = 0;
	S8 *unicodecontent = NULL;
	PU8 point=NULL;
  

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) ModifyChatRoom, msgentry->dcs is %d",msgentry->dcs);
  
	/* content might be unicode or ascii, convert it to unicode and store it to unicodecontent */
	if (msgentry->dcs == SMSAL_UCS2_DCS)
	{
		len = (S16)pfnUnicodeStrlen(content);
		unicodecontent = OslMalloc((len+1)*2);
		memcpy(unicodecontent, content, len*2);
		memset(unicodecontent+len*2, 0, 2);
	}
	else
	{
		len = strlen(content);
		unicodecontent = OslMalloc((len+1)*2);
		AnsiiNToUnicodeString(unicodecontent, content, len);
		memset(unicodecontent+len*2, 0, 2);
	}

	length = pfnUnicodeStrlen(unicodecontent)*2;
	if(CheckForInvitationMsg((U8*)unicodecontent, length, FALSE))
	{
		mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) ModifyChatRoom, remove invitation char");
		len=(S16)pfnUnicodeStrlen(gInvitation);
		memcpy((S8*)unicodecontent, (S8*)unicodecontent+len*2, length-len*2);
		memset((S8*)unicodecontent+length-len*2, 0, len*2);
	}

	IncrementCounter(index);
	point=CopyToBuffer((U8*)unicodecontent,(S32)pfnUnicodeStrlen(unicodecontent)*2,index, flag);
	if(point==NULL)
	{
		DisplayPopup((PU8)GetString(CHATROOM_STORAGE_FULL_ERROR), IMG_GLOBAL_ERROR, FALSE, 2000, ERROR_TONE);		
		gChatMsgFlag=0;
	}
	else
	{
		mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) ModifyChatRoom, not NULL point");
		g_chat_cntx.chatRoomInfo[index].chatStrings[GetNumOfMessage(index)-1].chat_message_string=point;
		g_chat_cntx.chatRoomInfo[index].chatStrings[GetNumOfMessage(index)-1].flag=flag;
		//mmi_frm_sms_delete_chat_sms(NULL, MOD_MMI,MMI_FRM_SMS_APP_AWAITS , current_chat_sms_index);//MMI_FRM_SMS_APP_AWAITS
		mmi_frm_sms_delete_sms(NULL, MOD_MMI,MMI_FRM_SMS_APP_AWAITS , msgentry->index);//MMI_FRM_SMS_APP_AWAITS
	}
	/* free unicodecontent buffer */
	if (unicodecontent!=NULL)
	{
		OslMfree(unicodecontent);
		unicodecontent = NULL;
	}

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  UpdateChatRoom
* DESCRIPTION
*   It updates Chat room upon arrival of new message 
*
* PARAMETERS
*  Chat Room Number and Receiving /Sending flag
* RETURNS
*  status
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL UpdateChatRoom(U16 index,U8 flag)
{
	EMSData * pEms ;
	PU8 point=NULL;
	if(flag==SENDING_CHAT)
	{
		GetEMSDataForEdit (& pEms, 0);
		if((CheckForInvitationMsg(pEms->textBuffer,pEms->textLength,FALSE)))
		{
			S16 len=(S16 )pfnUnicodeStrlen(gInvitation);
				ResetCurrentPosition(pEms);
				ForwardCurrentPosition(pEms,len);
				DeletenCharFromEMSBuffer(len);
		}
	}
	else 
		GetEMSDataForView (& pEms, 0);
	
	if( pEms->listHead == NULL)
	{
		
		IncrementCounter(index);
		point=CopyToBuffer(pEms->textBuffer,(S32)pEms->textLength,index, flag);
		if(point==NULL)
		{
			DisplayPopup((PU8)GetString(CHATROOM_STORAGE_FULL_ERROR), IMG_GLOBAL_ERROR, FALSE, 2000, ERROR_TONE);		
			return FALSE;
		}
		g_chat_cntx.chatRoomInfo[index].chatStrings[GetNumOfMessage(index)-1].chat_message_string=point;
		g_chat_cntx.chatRoomInfo[index].chatStrings[GetNumOfMessage(index)-1].flag=flag;
		
			
		return TRUE;
		
	}
	return TRUE;
	
}

/**************/
/*Send Chat Msg*/
/**************/
S8* chatbuff;
void CallbackChatMsg(void *dummy, module_type mod, U16 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	switch(result)
	{
        case MMI_FRM_SMS_OK:
            DisplayPopup(
                (PU8) GetString(STR_SMS_SEND_SUCCESS),
                IMG_SEND_SUCCESS_PIC_MSG,
                1,
                MESSAGES_POPUP_TIME_OUT,
                (U8) SUCCESS_TONE);
            mmi_frm_sms_delete_screen_history();
            OnSMSChatSendSuccess();
            break;
        case MMI_FRM_SMS_ABORT:
            /* do not keep chat content when sending fail */
            ReleaseEMSEditBuffer();
            DisplayPopup(
                (PU8) GetString(STR_ABORT_SENDING_SMS),
                IMG_GLOBAL_ACTIVATED,
                1,
                MESSAGES_POPUP_TIME_OUT,
                (U8) SUCCESS_TONE);
            mmi_frm_sms_delete_screen_history();
			OnSMSChatSendAbort();
			break;
        case MMI_FRM_SMS_NOTREADY:
            DisplayPopup(
                (PU8) GetString(STR_SMS_MSG_NOT_READY_YET),
                IMG_SEND_FAIL_PIC_MSG,
                1,
                MESSAGES_POPUP_TIME_OUT,
                (U8) ERROR_TONE);
            break;
        default:
            /* do not keep chat content when sending fail */
            ReleaseEMSEditBuffer();
            DisplayPopup(
                (PU8) GetString(STR_SMS_SEND_FAILED),
                IMG_SEND_FAIL_PIC_MSG,
                1,
                MESSAGES_POPUP_TIME_OUT,
                (U8) ERROR_TONE);
            mmi_frm_sms_delete_screen_history();
			OnSMSChatSendFailure();
			break;
	}
	if (chatbuff!=NULL)
	{
		OslMfree(chatbuff);
		chatbuff=NULL;
	}
}

/*****************************************************************************
* FUNCTION
*  SendChatMsg
* DESCRIPTION
*   It sends chat message
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SendChatMsg(void)
{
#if 0//(defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
  mmi_frm_sms_send_struct * sendData=OslMalloc(sizeof(mmi_frm_sms_send_struct));

  EMSData*	pEMS;
  U8      aHeader[(MAX_SMS_CHAT_HEADER_SIZE+1)*ENCODING_LENGTH];
	U8*	handledBuffer = NULL;
	U8* handledHeaderBuffer =NULL;
  
  GetHeader(aHeader,GetCurrentChatRoomNo());
  GetEMSDataForEdit (& pEMS, 0);
  memcpy(g_chat_cntx.writeChatBuffer , pEMS->textBuffer, pEMS->textLength);

  
	memset((S8*)sendData, 0, sizeof(mmi_frm_sms_send_struct));
	memset(sendData->number, 0, MAX_DIGITS_SMS);
	UnicodeToAnsii((S8*)sendData->number, (S8*)g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.destinationNo);
	mmi_frm_sms_send_sms(CallbackChatMsg, MOD_MMI, sendData);
	OslMfree(sendData);

#else
	mmi_frm_sms_send_struct* sendData=OslMalloc(sizeof(mmi_frm_sms_send_struct));
	U8      aHeader[(MAX_SMS_CHAT_HEADER_SIZE+1)*ENCODING_LENGTH];
	U16     buffLen;
	U8*	handledBuffer = NULL;
	U8* handledHeaderBuffer =NULL;
	U16 writeChatBufferCharNum;
	U16 headerCharNum;
	BOOL isUcs2Char=0;
	#if 0
  	EMSData*	pEMS;
  	U16 stringlength;
  	GetEMSDataForEdit (& pEMS, 0);
  	memcpy(g_chat_cntx.writeChatBuffer , pEMS->textBuffer, pEMS->textLength);
	#endif /* Guoj delete. It's not used now */


	memset((S8*)sendData, 0, sizeof(mmi_frm_sms_send_struct));
	
	writeChatBufferCharNum = pfnUnicodeStrlen((PS8)g_chat_cntx.writeChatBuffer); 
	isUcs2Char = mmi_frm_sms_check_UCS2((U16*)g_chat_cntx.writeChatBuffer , writeChatBufferCharNum);
	mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) SendChatMsg, gInputBoxUcs2Flag=%d,isUcs2Char=%d", gInputBoxUcs2Flag,isUcs2Char);
    	if (GetUCS2Flag(g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.nickName) || gInputBoxUcs2Flag ||
        	isUcs2Char)
    	{
        	sendData->dcs = SMSAL_UCS2_DCS;
    	}
    	else
    	{
        	sendData->dcs = SMSAL_DEFAULT_DCS;
    	}
    	UnicodeToAnsii(
       	 (S8*) sendData->number,
        	(S8*) g_chat_cntx.chatRoomInfo[GetCurrentChatRoomNo()].chatInfo.destinationNo);
    	GetHeader(aHeader, GetCurrentChatRoomNo());
    	headerCharNum = pfnUnicodeStrlen((PS8) aHeader);
		
	mmi_trace(g_sw_SMS,"MMI_SMS:(Chat) sendData->dcs=%d", sendData->dcs);
	if(sendData->dcs==SMSAL_DEFAULT_DCS)
	{
		handledHeaderBuffer=OslMalloc((headerCharNum*2+1)*ENCODING_LENGTH);
		memset((S8*)handledHeaderBuffer, 0, ((headerCharNum*2+1)*ENCODING_LENGTH));
		mmi_frm_sms_handle_extension_char((U16*)handledHeaderBuffer, (U16*)aHeader,headerCharNum);

		handledBuffer = OslMalloc((writeChatBufferCharNum * 2 + 1) * ENCODING_LENGTH);
		memset((S8*) handledBuffer, 0, ((writeChatBufferCharNum * 2 + 1) * ENCODING_LENGTH));
		mmi_frm_sms_handle_extension_char(
			(U16*) handledBuffer,
			(U16*) g_chat_cntx.writeChatBuffer,
			writeChatBufferCharNum);
	}
	else
	{
		handledHeaderBuffer=OslMalloc((headerCharNum+1)*ENCODING_LENGTH);
		memset((S8*)handledHeaderBuffer, 0, ((headerCharNum+1)*ENCODING_LENGTH));
		memcpy((S8*)handledHeaderBuffer , (S8*)aHeader, headerCharNum*ENCODING_LENGTH);

		handledBuffer=OslMalloc((writeChatBufferCharNum+1)*ENCODING_LENGTH);
		memset((S8*)handledBuffer, 0, ((writeChatBufferCharNum+1)*ENCODING_LENGTH));
		memcpy((S8*)handledBuffer , (S8*)g_chat_cntx.writeChatBuffer, writeChatBufferCharNum*ENCODING_LENGTH);
	}

	sendData->stringlength = pfnUnicodeStrlen((PS8) handledHeaderBuffer) + pfnUnicodeStrlen((PS8) handledBuffer);

	mmi_trace(g_sw_SMS, "MMI_SMS:(Chat) stringlength = %d ",sendData->stringlength);
	 
	buffLen =
        (((sendData->stringlength + 1) * 2) <
         (MMI_FRM_SMS_MSG_LEN * 10)) ? ((sendData->stringlength + 1) * 2) : (MMI_FRM_SMS_MSG_LEN * 10);
	chatbuff=OslMalloc(buffLen);
	memset(chatbuff, 0, buffLen);

	pfnUnicodeStrcpy(chatbuff, (S8*)handledHeaderBuffer);
	pfnUnicodeStrcat(chatbuff, (S8*)handledBuffer);
	sendData->string = OslMalloc(buffLen);
	memset(sendData->string, 0, buffLen);
	memcpy(sendData->string, chatbuff, buffLen);

#if 0
	pEMS->textLength = buffLen - 2;  //the end will be '\0'
	memcpy(pEMS->textBuffer, chatbuff, buffLen);
	pEMS->dcs = sendData->dcs;
#endif /* Guoj delete. It's not used now */

	if (chatbuff!=NULL)
	{
		OslMfree(chatbuff);
		chatbuff=NULL;
	}
	buffLen = ((pfnUnicodeStrlen((PS8)aHeader)+writeChatBufferCharNum)+1)*2; 
	buffLen=((buffLen)<(MMI_FRM_SMS_MSG_LEN*10))?(buffLen):(MMI_FRM_SMS_MSG_LEN*10);
	chatbuff=OslMalloc(buffLen);
	memset(chatbuff, 0, buffLen);
	pfnUnicodeStrcpy(chatbuff, (S8*)aHeader);
	pfnUnicodeStrcat(chatbuff, (S8*)g_chat_cntx.writeChatBuffer);

	
	DeleteScreenIfPresent(CHAT_OPTION_SCREEN_ID_2);	

	mmi_frm_sms_send_sms(CallbackChatMsg, MOD_MMI, sendData);
	OslMfree(sendData);
	OslMfree(handledBuffer);
   	OslMfree(handledHeaderBuffer);	

#endif
}

/*****************************************************************************
* FUNCTION
*  PreSendChatMsg
* DESCRIPTION
*   Before send chat message, check DCS and SC number
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PreSendChatMsg(void)
{
	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	if(mmi_frm_sms_check_action_pending())
	{
        DisplayPopup(
            (PU8) GetString(STR_SMS_MSG_NOT_READY_YET),
            IMG_GLOBAL_UNFINISHED,
            1,
            MESSAGES_POPUP_TIME_OUT,
            (U8) ERROR_TONE);
        return;
    }
    if (!IsScreenPresent(CHAT_SCREEN_START_NEW_ID))
    {
        ExitChatWrite();
    }
    SendChatMsg();
}


/*****************************************************************************
* FUNCTION
*  OnSMSChatSendAbort
* DESCRIPTION
*   It delets Screen from history upon abort of Message send
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void OnSMSChatSendAbort(void)
{
	RemoveHeaderInfo();
	DeleteScreenIfPresent(CHAT_OPTION_SCREEN_ID);
}


/*****************************************************************************
* FUNCTION
*  OnSMSChatSendFailure
* DESCRIPTION
*   It delets Screen from history upon failure of Message send
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void OnSMSChatSendFailure(void)
{
	DeleteScreenIfPresent(CHAT_SCREEN_START_NEW_ID);
	DeleteScreenIfPresent(CHAT_OPTION_SCREEN_ID);
        gInputBoxUcs2Flag=0;
	memset(g_chat_cntx.writeChatBuffer,0,((MAX_MSG_SIZE+1)*ENCODING_LENGTH));
}

/*****************************************************************************
* FUNCTION
*  OnSMSChatSendSuccess
* DESCRIPTION
*   It delets Screen from history upon success of Message send
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void OnSMSChatSendSuccess(void)
{
	S16 index=GetCurrentChatRoomNo();
	PU8 point=NULL;
	if(IsScreenPresent(CHAT_ROOM_SCREEN_ID))
		HistoryReplace(CHAT_ROOM_SCREEN_ID, CHAT_ROOM_SCREEN_ID, EntryChatRoom);
	else if(IsScreenPresent(CHAT_SCREEN_START_NEW_ID))
		HistoryReplace(CHAT_SCREEN_START_NEW_ID, CHAT_ROOM_SCREEN_ID, EntryChatRoom);

	DeleteScreenIfPresent(CHAT_SCREEN_START_NEW_ID);
	DeleteScreenIfPresent(CHAT_OPTION_SCREEN_ID);
	
	if((g_chat_cntx.chatRoomInfo[index].status==DISABLE))
		ActivateChatRoom(GetCurrentChatRoomNo());
        
        gInputBoxUcs2Flag=0;
	memset(g_chat_cntx.writeChatBuffer,0,((MAX_MSG_SIZE+1)*ENCODING_LENGTH));
	IncrementCounter(index);
	if(CheckForInvitationMsg((U8*)chatbuff, pfnUnicodeStrlen(chatbuff)*2, FALSE))
	{
		S16 len=(S16)pfnUnicodeStrlen(gInvitation);
		memcpy((S8*)chatbuff, (S8*)chatbuff+len*2, pfnUnicodeStrlen(chatbuff)*2-len*2);
		memset((S8*)chatbuff+pfnUnicodeStrlen(chatbuff)*2-len*2, 0, len*2);
}
	point=CopyToBuffer((U8*)chatbuff,pfnUnicodeStrlen(chatbuff)*2,index, SENDING_CHAT);
	if(point==NULL)
	{
		DisplayPopup((PU8)GetString(CHATROOM_STORAGE_FULL_ERROR), IMG_GLOBAL_ERROR, FALSE, 2000, ERROR_TONE);		
		return;
	}
	g_chat_cntx.chatRoomInfo[index].chatStrings[GetNumOfMessage(index)-1].chat_message_string=point;
	g_chat_cntx.chatRoomInfo[index].chatStrings[GetNumOfMessage(index)-1].flag=SENDING_CHAT;
}

/*****************************************************************************
 * FUNCTION
 *  CallbackSaveChatMsg
 * DESCRIPTION
 *  
 * PARAMETERS
 *  status      [?]         
 *  mod         [IN]        
 *  result      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void CallbackSaveChatMsg(void *status, module_type mod, U16 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 sendresult = *(U8*) status;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (result)
    {
        case MMI_FRM_SMS_OK:
            if (sendresult == SMSAL_STO_UNSENT)
            {
                DisplayPopup(
                    (PU8) GetString(STR_SMS_SEND_FAILED_SAVE_SUCCESS),
                    IMG_GLOBAL_ACTIVATED,
                    1,
                    MESSAGES_POPUP_TIME_OUT,
                    (U8) SUCCESS_TONE);
            }
            else
            {
                DisplayPopup(
                    (PU8) GetString(STR_SMS_SEND_SAVE_SUCESS),
                    IMG_GLOBAL_ACTIVATED,
                    1,
                    MESSAGES_POPUP_TIME_OUT,
                    (U8) SUCCESS_TONE);
            }
            break;
        case MMI_FRM_SMS_MEMFULL:
            if (sendresult == SMSAL_STO_UNSENT)
            {
                DisplayPopup(
                    (PU8) GetString(STR_SMS_SEND_SAVE_FAILED),
                    IMG_GLOBAL_UNFINISHED,
                    1,
                    MESSAGES_POPUP_TIME_OUT,
                    (U8) SUCCESS_TONE);
            }
            else
            {
                DisplayPopup(
                    (PU8) GetString(STR_SMS_SEND_SUCESS_SAVE_FILED),
                    IMG_GLOBAL_ACTIVATED,
                    1,
                    MESSAGES_POPUP_TIME_OUT,
                    (U8) SUCCESS_TONE);
            }
            break;
        default:
            DisplayPopup(
                (PU8) GetString(STR_SMS_SEND_SAVE_FAILED),
                IMG_GLOBAL_UNFINISHED,
                1,
                MESSAGES_POPUP_TIME_OUT,
                (U8) ERROR_TONE);
            break;
    }
    if (IsScreenPresent(SCR_ID_MSG_WRITE))
    {
        SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_WRITE);
    }
    else
    {
        SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_PROCESSING);
    }
    DeleteMessagesHistoryNodes();
    AlmEnableSPOF();
}


/*****************************************************************************
 * FUNCTION
 *  SaveChatMsg
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SaveChatMsg(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_frm_sms_send_struct *sendData = OslMalloc(sizeof(mmi_frm_sms_send_struct));

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset((S8*) sendData, 0, sizeof(mmi_frm_sms_send_struct));
    sendData->status = SMSAL_STO_UNSENT;
    memset((S8*) sendData->number, 0, MAX_DIGITS_SMS);
    mmi_frm_sms_save_sms(CallbackSaveChatMsg, MOD_MMI, sendData);
    OslMfree(sendData);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_msg_chat_get_invitation_pattern
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S8 *mmi_msg_chat_get_invitation_pattern(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (S8*) gInvitation;
}

#endif /* _CHAT_APPSENDMSG_ */ 
#endif /* __MMI_MESSAGES_CHAT__ */ 

