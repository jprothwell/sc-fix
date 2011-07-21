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
 * SmsMoMtGuiInterface.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for MMI SMS APP.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: SmsGuiHandler.c

  	PURPOSE		: SMS GUI handlers 

 

	AUTHOR		: Manu Vikram and Magesh k, Hiran.

	DATE		: 01-01-2003

**************************************************************/
#include "mmi_trace.h"

#ifdef __MOD_SMSAL__
/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

//... Add More MMI header
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
//...Add MSG header
#include "customer_ps_inc.h"
#include "mmi_msg_context.h"
#include "messagesl4def.h"
#include "messagesresourcedata.h"
#include "messagesmiscell.h"
#include "messagesexdcl.h"
#include "smsguiinterfaceprot.h"
#include "smsguiinterfacetype.h"
#include "smspshandler.h"
#include "cellbroadcasttype.h"
#ifdef __MMI_MESSAGES_CHAT__
#include "chatappgprot.h"
#include "chatappdef.h"
#include "chatgtype.h"
#include "chatappgexdcl.h"
#endif

#include "gui_ems.h"
#include "wgui_ems_categories.h"
#include "gsm7bitdefaultalphabet.h"
//
#include "smsal_l4c_enum.h"
//
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
//
#include "csp.h"
#include "frameworkstruct.h"
#include "mainmenudef.h"
#include "callhistorygprots.h"
#include "callmanagementgprot.h"
#include "callhistoryenum.h"
#include "miscfunctions.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "phonebookgprot.h"
#include "idleappprot.h"
#include "alarmframeworkprot.h"

#include "wgui_status_icons.h"
#include "gpioinc.h"
#include "keybrd.h"
#include "sublcdhistorygprot.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"


#include "smsapi.h"
#include "smsstruct.h"
#include "smsfunc.h"
#ifdef __MMI_EMAIL__
#include "emailappgprot.h"
#endif

#include "dual_sim.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#include "mtpnp_ad_resdef.h"
#endif

#include "settinggenum.h"
#include "securitysetup.h"

#ifdef __MMI_IMPS__
#include "mmi_imps_gprot.h"
#endif
#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__)&&!defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_MYPICTURE__)
#include "wgui_tab_bars.h" 
#include "wgui_categories_mms.h"
#include "wgui_categories_imps.h"
#include "picmsgapitoothers.h"
#include "filesystemdef.h"	
#include "filemanagerdef.h"
#include "filemgr.h"
#include "mytone.h"

#include "mypicture.h"
#include "smrtringtoneconverter.h"
#include "phonebookgprot.h"
S8 RingtoneNameBuffer[NSM_FILE_NAME_BUFF];	
S8 PictureNameBuffer[NSM_FILE_NAME_BUFF];
U8 *gPictureDataBuffer= NULL;
U16 gSelected_Item =0;
U16 gConvertFlag = FALSE;
U8 gPictureMessageError = 0;
U8 TextBuffer[(MAX_PICMSG_DEFAULT_DCS_TEXT_LEN*ENCODING_LENGTH)+(10*ENCODING_LENGTH)];
extern U8 gaddtext_buffer[];

#if defined(__MMI_MYPICTURE__)
extern U8 gfilecount;
#endif /* __MMI_MYPICTURE__ */


extern U8 gNSMBuffer[1024];
extern S8 g_pmsg_TextInMessage[];

U8 *pnsmMessageAsciibuff =  NULL ;
U16 RemoveZerosInBetween(U8 *pOutBuffer, U8 *pInBuffer,U16 size );

#endif

#ifdef __MMI_MESSAGES_USE_URL__
#include "wap.h"
#define MAX_NUM_URL_PATTERN	8
S8 urlHttp[10] = {0x68,0x74,0x74,0x70,0x3a,0x1b,0x5c,0x1b,0x5c,0x00};
S8 urlHttps[11] = {0x68,0x74,0x74,0x70,0x73,0x3a,0x1b,0x5c,0x1b,0x5c,0x00};
const PS8 urlPattern[MAX_NUM_URL_PATTERN]={"http://", "http:\\\\", urlHttp, "https://", "https:\\\\", urlHttps, "www.", "wap."};
#endif

#if defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
#endif
#ifdef __MMI_MESSAGES_INSERT_FROM_PHB__
#include "phonebookprot.h"
extern MMI_PHB_LIST_VIEW phbListView;
#define SMS_INSERT_PHB_NAME			0
#define SMS_INSERT_PHB_NUMBER			1
static U8 gSetFlagNameNumber = 0;
#endif

#ifdef __MMI_USE_LSK_AS_SEND2__
BOOL bUseLSKAsSendKey2 = FALSE;
#endif

#undef __NEWSIMULATOR

//
#define ONE_DIGIT(value)						((value) % 10)
extern EMSData * GetEMSDataForView              (EMSData ** p, U8 force) ;
extern EMSData * GetEMSDataForEdit              (EMSData ** p, U8 force) ;
extern U8        DeInitMessagesEMSBuffer        (msg_ems_data_struct * pE);
extern void GBSetInputboxLSKFunction(FuncPtr f);
extern U8 mmi_frm_sms_get_storage(U8 type, U16 index);
//
//////////////////////////////////////////////////////////////////////////////////////
// local variables
//////////////////////////////////////////////////////////////////////////////////////



/********************************************************************

						variables

********************************************************************/
msg_context_struct g_msg_cntx;

static U16 	msg_popup_strID;
static U16 	msg_popup_imgID;
static U8		msg_popup_tonID;
//wangzl: 090626 add this macro and disable it for bug12579
//#define __MMI_SUSPEND_AUDIO_PLAY_WHEN_POPUP_NEW_MESSAGE_IND__
#ifdef  __MMI_SUSPEND_AUDIO_PLAY_WHEN_POPUP_NEW_MESSAGE_IND__
static U8 	backgroundaudioresume=FALSE;
#endif
static U16 	msg_confirm_strID=STR_GLOBAL_OK;
static FuncPtr	msg_confirm_lskFuncPtr=NULL;
static FuncPtr	msg_confirm_rskFuncPtr=NULL;

static FuncPtr	msg_common_funcPtr=NULL;

S16 processCaptionStrId;
S16 processBodyStrId;
U16 processAnimationImageId;
U16 processRSKStrId;

static msg_in_out_struct  TotalInboxOutboxStatus;

msg_active_profile_struct activeProfile;

msg_msgbox_info_struct msgbox_info;
extern PendingSaveSendDataStruct PendingSaveSendData;

#ifdef __MMI_TTS_FEATURES__ 
#include "nvram_user_defs.h"

char* pTtsText;
size_t nTtsTextSize;
extern nvram_tts_setting_struct g_tts_setting;

void ejTTS_play_text_hdlr(void)
{
	ejTTS_PlayToPCM(pTtsText, nTtsTextSize);
}
void ejTTS_play_stop_hdlr(void)
{
	ejTTSPlayerStop();
}
#endif


/****************************************************************

				generic functions

****************************************************************/
/*****************************************************************************
* FUNCTION
*  mmi_msg_set_msg_icon
* DESCRIPTION
*   Set msg icon for unread/full message.
*
* PARAMETERS
*  a  IN		blink
*  b  IN		unblink
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_msg_icon (U8 blink, U8 unblink)
{
	U16 i=0;
	U16 totalinbox = mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);

	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_set_msg_icon");

	if(blink==TRUE)
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_set_msg_icon blink==TRUE");
	
		HideStatusIcon(STATUS_ICON_INCOMING_SMS);
		HideStatusIcon(STATUS_ICON_SUBLCD_SMS);
		BlinkStatusIcon(STATUS_ICON_INCOMING_SMS);
		BlinkStatusIcon(STATUS_ICON_SUBLCD_SMS);
		UpdateStatusIcons();
		return;
	}
	if(unblink==TRUE)
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_set_msg_icon unblink==TRUE");
	
		HideStatusIcon(STATUS_ICON_INCOMING_SMS);
		HideStatusIcon(STATUS_ICON_SUBLCD_SMS);
	}
	if(g_msg_cntx.msg_full_ind[0]==FALSE)
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_set_msg_icon g_msg_cntx.msg_full_ind==FALSE");
	
		if (totalinbox!=MMI_FRM_SMS_INVALID_INDEX)
		{

			mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_set_msg_icon totalinbox: %d", totalinbox);
		
			while(i<totalinbox)
			{
				if((mmi_frm_sms_get_status(MMI_FRM_SMS_APP_INBOX, i)&0x0f)==MMI_FRM_SMS_APP_UNREAD)
				{
					mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_set_msg_icon UNREAD found");
				
					ShowStatusIcon(STATUS_ICON_INCOMING_SMS) ;
					ShowStatusIcon(STATUS_ICON_SUBLCD_SMS);
					UpdateStatusIcons();
					return;
				}
				i++;
			}
		}

		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_set_msg_icon NO UNREAD found");
		
		HideStatusIcon(STATUS_ICON_INCOMING_SMS);
		HideStatusIcon(STATUS_ICON_SUBLCD_SMS);
	}
	UpdateStatusIcons();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_set_funcptr
* DESCRIPTION
*   Set function pointer for later usage
*
* PARAMETERS
*  funcptr  IN		function pointer for later usage
* RETURNS
*  none
* GLOBALS AFFECTED
*   msg_common_funcPtr
*****************************************************************************/
void mmi_msg_set_funcptr (FuncPtr funcptr)
{
	msg_common_funcPtr=funcptr;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_exec_funcptr
* DESCRIPTION
*   Execute function pointer set earlier
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_exec_funcptr (void)
{
	if (msg_common_funcPtr!=NULL)
		(*msg_common_funcPtr) ();
	msg_common_funcPtr=NULL;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_set_msg_waiting_icon
* DESCRIPTION
*   Set msg icon for waiting vm/email/fax
*
* PARAMETERS
*  a  IN		line
*  b  IN		type
*  c  IN		set/unset
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_msg_waiting_icon(U8 line, U8 type, U8 set)
{
	U8 i=0;
	U8	otherline=(line==0? 1: 0);
	U16 icon[]=
	{
		STATUS_ICON_UNREAD_VOICE_L1,
		STATUS_ICON_UNREAD_VOICE_L2,
		STATUS_ICON_UNREAD_VOICE_L1L2,		
		STATUS_ICON_UNREAD_FAX_L1,
		STATUS_ICON_UNREAD_FAX_L2,
		STATUS_ICON_UNREAD_FAX_L1L2,		
		STATUS_ICON_UNREAD_EMAIL_L1,
		STATUS_ICON_UNREAD_EMAIL_L2,
		STATUS_ICON_UNREAD_EMAIL_L1L2,		
	};

	for(i=0; i<(MAX_WAITING_LINE_NUM+1); i++)
		IdleResetStatusIcon(icon[(type*(MAX_WAITING_LINE_NUM+1))+i]);
	if((set>0)&&(g_msg_cntx.msg_waiting_info[otherline].number[type]))
		IdleSetStatusIcon(icon[(type*(MAX_WAITING_LINE_NUM+1))+MAX_WAITING_LINE_NUM]);
	else if(set>0)
		IdleSetStatusIcon(icon[(type*(MAX_WAITING_LINE_NUM+1))+line]);
	else if(g_msg_cntx.msg_waiting_info[otherline].number[type])
		IdleSetStatusIcon(icon[(type*(MAX_WAITING_LINE_NUM+1))+otherline]);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_exit_generic
* DESCRIPTION
*   Exit generic function for MSG app.
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_exit_generic (void)
{
	#ifdef __MMI_USE_LSK_AS_SEND2__
	bUseLSKAsSendKey2  = FALSE;
	#endif
	g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=GetMessagesCurrScrnID();
	SetMessagesCurrScrnID(0);	//messagesCurrScrnID
 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_disable_mini_tab_bar();
#endif
 
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_generic
* DESCRIPTION
*   Highlight generic function for MSG app
*
* PARAMETERS
*  a  IN		left soft key string
*  b  IN		left soft key image
*  c  IN		right soft key string
*  d  IN		right soft key image
*  e  IN		left soft key function
*  f  IN		right soft key function
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_generic (U16 lskstr, U16 lskimg, U16 rskstr, U16 rskimg, 
																FuncPtr lskfunc, FuncPtr rskfunc)
{
	ChangeLeftSoftkey(lskstr, lskimg);
	ChangeRightSoftkey(rskstr, rskimg);
	SetLeftSoftkeyFunction(lskfunc,KEY_EVENT_UP);
	SetKeyHandler(lskfunc, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(rskfunc,KEY_EVENT_UP);
	SetKeyHandler(rskfunc, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_set_confirm_screen
* DESCRIPTION
*   Set string and function for MSG confirmation screen
*
* PARAMETERS
*  a  IN		string ID
*  b  IN		left soft key function
*  c  IN		right soft key function
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_confirm_screen (U16 id, FuncPtr lskfunc, FuncPtr rskfunc)
{
	msg_confirm_strID=id;
	msg_confirm_lskFuncPtr=lskfunc;
	msg_confirm_rskFuncPtr=rskfunc;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_confirm_generic
* DESCRIPTION
*   Entry MSG generic confirmation screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_confirm_generic (void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,	
                                STR_GLOBAL_NO, IMG_GLOBAL_NO,   
                                get_string(msg_confirm_strID),
                                IMG_GLOBAL_QUESTION, WARNING_TONE); 
    
	SetLeftSoftkeyFunction (msg_confirm_lskFuncPtr, KEY_EVENT_UP);
	SetRightSoftkeyFunction (msg_confirm_rskFuncPtr, KEY_EVENT_UP);
	
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_exit_confirm_generic
* DESCRIPTION
*   Exit MSG generic confirmation screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_exit_confirm_generic (void)
{
	stopRequestedTone(WARNING_TONE);
	SetMessagesCurrScrnID(0);	//messagesCurrScrnID	
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_set_processing_screen
* DESCRIPTION
*   Set string and image for MSG processing screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_processing_screen (U16 captionStrId, U16 bodyStrId, U16 animationImageId, U16 rskStrId)
{
	processCaptionStrId=captionStrId;
	processBodyStrId=bodyStrId;
	processAnimationImageId=animationImageId;
	processRSKStrId=rskStrId;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_end_key_from_processing
* DESCRIPTION
*   Reset display list type before go to idle.
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_end_key_from_processing (void)
{
	if (!GetTotalCallCount())
		g_msg_cntx.toDisplayMessageList=TO_DISPLAY_MESSAGE_LIST_NONE;
	ExecCurrEndKeyDownHandler();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_processing
* DESCRIPTION
*   Reset display list type before go back screen.
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_processing (void)
{
	g_msg_cntx.toDisplayMessageList=TO_DISPLAY_MESSAGE_LIST_NONE;
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_call_handle_from_processing
* DESCRIPTION
*   Reset current screen before enter call screen.
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_call_handle_from_processing (void)
{
	SetMessagesCurrScrnID(0);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_processing_generic
* DESCRIPTION
*   Entry MSG generic processing screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_processing_generic (void)
{
	EntryNewScreen(SCR_ID_MSG_PROCESSING, mmi_msg_exit_processing_generic, mmi_msg_entry_processing_generic, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_processing_generic");
	
	SetMessagesCurrScrnID (SCR_ID_MSG_PROCESSING);	// may be temp func...
	GetCurrEndKeyDownHandler();
 
#ifdef __MMI_UI_STYLE_4__
//passing 0xffff for Title string and Title icon for the disbling the status bar and title display.
	ShowCategory8Screen (0xffff, 0xffff,
							0, 0, processRSKStrId, 0, processBodyStrId, processAnimationImageId, NULL);
#else
	ShowCategory8Screen (processCaptionStrId, IMG_SMS_ENTRY_SCRN_CAPTION,
							0, 0, processRSKStrId, 0, processBodyStrId, processAnimationImageId, NULL);
#endif
 

//wangzl: repeal wangbei's modification, wangbei 对这段代码的理解是有错误的，这段代码不仅仅
//用在save 时，设计时根据rsk的内容决定操作是否可以取消，wangbei错误的修改成了
//总可以取消，而这会引起问题
#if 1
// wangbeidel start 20060617 
// 因为在播放正在存储的动画时，目前不能获取计时器消息而陷入死机的情况，所以暂时先这样调整。
// 这个函数是在编辑短消息完成后，在选项列表中选择"Save"是所进入的屏幕函数

	if(processRSKStrId==0)
	{
		ClearAllKeyHandler ();
		ClearKeyHandler(KEY_END, KEY_EVENT_UP);
		ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
		ClearKeyHandler(KEY_END, KEY_REPEAT);
		if(isInCall())
			SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);
		StartTimer (MESSAGES_INPROGRESS_TIMER_ID, MESSAGES_INPROGRESS_TIME_OUT, mmi_msg_time_out_processing_generic);
	}
	else
	{	   
		SetKeyHandler (mmi_msg_go_back_from_processing, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		SetRightSoftkeyFunction (mmi_msg_go_back_from_processing, KEY_EVENT_UP);
		SetKeyHandler(mmi_msg_end_key_from_processing, KEY_END, KEY_EVENT_DOWN);
	}
	// wangbei del end
#else 
// wangbei add 20060617
	SetKeyHandler (mmi_msg_go_back_from_processing, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (mmi_msg_go_back_from_processing, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_end_key_from_processing, KEY_END, KEY_EVENT_DOWN);
// wangbei add  end
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_exit_processing_generic
* DESCRIPTION
*   Exit MSG generic processing screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_exit_processing_generic (void)
{
	g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=GetMessagesCurrScrnID();
	SetMessagesCurrScrnID(0);	//NOTE!!!!! do not set to zero for incoming call reason
	StopTimer (MESSAGES_INPROGRESS_TIMER_ID);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_time_out_processing_generic
* DESCRIPTION
*   Time out call back of processing screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_time_out_processing_generic (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_time_out_processing_generic");
	StopTimer (MESSAGES_INPROGRESS_TIMER_ID);
	if(GetMessagesCurrScrnID()==SCR_ID_MSG_PROCESSING)
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);		
	DeleteMessagesHistoryNodes();
	DeleteScreenIfPresent(SCR_ID_MSG_PROCESSING);
	mmi_frm_sms_delete_action_pending();//wangzl: if failed, delete action
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_set_popup_generic
* DESCRIPTION
*   Set popup screen.
*
* PARAMETERS
*  a  IN		string ID
*  b  IN		image ID
*  c  IN		tone ID
* RETURNS
*  none
* GLOBALS AFFECTED
*   msg_popup_strID, msg_popup_imgID, msg_popup_tonID
*****************************************************************************/
void mmi_msg_set_popup_generic (U16 strID, U16 imgID, U8 toneID)
{
	msg_popup_strID	=	strID;
	msg_popup_imgID	=	imgID;
	msg_popup_tonID	=	toneID;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_display_popup_generic
* DESCRIPTION
*   Display popup screen.
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_display_popup_generic (void)
{
	DisplayPopup ((PU8)GetString(msg_popup_strID), msg_popup_imgID, 1, MESSAGES_POPUP_TIME_OUT, (U8)msg_popup_tonID);
	DeleteMessagesHistoryNodes ();
	AlmEnableSPOF();
}
/*****************************************************************************
* FUNCTION
*  playShortMessageTone
* DESCRIPTION
*   play Message Tone after startTimer
*
* PARAMETERS
*  a  IN		new message type
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/

void playShortMessageTone(U8 audioId)   //Added by jinzh:20070703
{
	playRequestedTone(audioId);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_new_msg_popup
* DESCRIPTION
*   Entry new message popup.
*
* PARAMETERS
*  a  IN		new message type
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_new_msg_popup (U8 type)
{
	U16 indstr;
	MMI_ALERT_TYPE alertType;
	MESSAGE_TONE_ENUM messagetonetype;//liuxl add the variavle to save message tone setting
	U16 pp_flag = 0;
	TRACE_AUD_FUNCTION();
       
        
       /* Framework support Popup display permisstion flag */
       /* Application who wanna stop popup has to intercept related messages and set the flag */
       pp_flag = GetInterruptPopupDisplay();
       if( (pp_flag & POPUP_DSPL_ON) == 0 )
       {
	       PlaySmsVibratorOnce();
      		return;
       }
       
	indstr=STR_RECEIVED_SMS;
	//wangzl: modify start for bug12082
	//alertType = GetMtCallAlertTypeEnum();
	//liuxl: modify start for bug 13164
	alertType = GetMtCallAlertTypeEnum();

	#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		switch(g_msg_cntx.newMsgSimID)
		{
			case 0:
				messagetonetype = GetMessageToneType();
				break;
			case 1:
				messagetonetype = GetCard2MessageToneType();
				break;
			case 2:
				break;
			case 3:
				break;
			default:
				break;
		}
	#else
		messagetonetype = GetMessageToneType();
	#endif
	//liuxl: modify end for bug 13164
	//wangzl: modify end
	EntryNewScreen(SCR_POPUP_SCREEN, mmi_msg_exit_new_msg_popup, NULL, NULL);
	switch(type)
	{
		case MSG_NEW_VOICEMAIL:
		#ifndef __MMI_MULTI_SIM__
			indstr = STR_VOICEMAIL_IND;
		#else /* __MMI_MULTI_SIM__ */
			#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			indstr = STR_VOICEMAIL_IND;
			#else
			indstr = STRING_MTPNP_MASTER_VOICEMAIL_IND + g_msg_cntx.newMsgSimID;
			#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
		#endif/*__MMI_MULTI_SIM__*/
			break;
		case MSG_NEW_FAX:
		#ifndef __MMI_MULTI_SIM__
			indstr = STR_FAX_IND;
		#else /* __MMI_MULTI_SIM__ */
			#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			indstr = STR_FAX_IND;
			#else
			indstr = STRING_MTPNP_MASTER_FAX_IND + g_msg_cntx.newMsgSimID;
			#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
		#endif/* __MMI_MULTI_SIM__ */
			break;
		case MSG_NEW_EMAIL:
		#ifndef __MMI_MULTI_SIM__
			indstr = STR_EMAIL_IND;
		#else /* __MMI_MULTI_SIM__ */
			#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			indstr = STR_EMAIL_IND;
			#else
			indstr = STRING_MTPNP_MASTER_EMAIL_IND +g_msg_cntx.newMsgSimID;
			#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
		#endif/*__MMI_MULTI_SIM__*/
			break;
		case MSG_NEW_MSG_NORMAL:
		#ifndef __MMI_MULTI_SIM__
			 indstr = STR_RECEIVED_SMS;
		#else /* __MMI_MULTI_SIM__ */
			#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			indstr = STR_RECEIVED_SMS;
			#else
			indstr = STRING_MTPNP_MASTER_RECEIVED_SMS + g_msg_cntx.newMsgSimID;
			#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
		#endif/*__MMI_MULTI_SIM__*/
			break;
		case MSG_NEW_MSG_CLASS0:
			indstr=STR_CLASS0_MESSAGE_CAPTION;
			break;
		case MSG_NEW_DELIVERY_REPORT:
		#ifndef __MMI_MULTI_SIM__
			indstr = STR_DELIVERY_REPORT_MENUENTRY;
		#else /* __MMI_MULTI_SIM__ */
			#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			indstr = STR_DELIVERY_REPORT_MENUENTRY;
			#else
			indstr = STRING_MTPNP_MASTER_DELIVERY_REPORT_MENUENTRY + g_msg_cntx.newMsgSimID;
			#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
		#endif/*__MMI_MULTI_SIM__*/
			break;
#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
#endif
		default:									indstr=STR_RECEIVED_SMS;	break;
	}
	GetCurrEndKeyDownHandler();
	ShowCategory65Screen((PU8)GetString (indstr),IMG_NEW_MESSAGE_NOTIFICATION_MSG,NULL);
	SetGroupKeyHandler(NULL,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,KEY_EVENT_DOWN);	
	SetGroupKeyHandler(GoBackHistory,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	SetKeyHandler(ExecCurrEndKeyDownHandler, KEY_END, KEY_EVENT_DOWN);


	msg_popup_tonID=0;
	
	//wangzl: modify for bug12082
	//if (alertType == MMI_VIBRATION_AND_RING || alertType == MMI_VIBRATION_THEN_RING || alertType == MMI_RING)
	if (messagetonetype !=MESSAGE_SILENT) //liuxl modify for fix bug-13164
	{	
		if (alertType != MMI_VIBRATION_ONLY) //if alertType is MMI_VIBRATION_ONLY,then can't play tone,so no need to get msg_popup_tonID
		{
			if (!GetTotalCallCount())
			{
                        	#ifdef __MMI_DUAL_PROFILE_SUPPORT__
                        		switch(g_msg_cntx.newMsgSimID)
                        		{
                        			case 0:
                        				msg_popup_tonID = MESSAGE_TONE;
                        				break;
                        			case 1:
                        				msg_popup_tonID = CARD2_MESSAGE_TONE;
                        				break;
                        			case 2:
                        				break;
                        			case 3:
                        				break;
                        			default:
                        				break;
                        		}
                        	#else
            			#ifndef __MMI_DUAL_SIM__
            				msg_popup_tonID=(U8)MESSAGE_TONE;
            			#else
            				if (g_msg_cntx.isSlave == FALSE)
            				{
            					msg_popup_tonID = (U8) MESSAGE_TONE;
            				}
            				else
            				{
            					msg_popup_tonID = (U8) CARD2_MESSAGE_TONE;
            				}
            			#endif
                        	#endif
			
			}
			else
				msg_popup_tonID=(U8)SMS_IN_CALL_TONE;
		}
	}
	if ((alertType == MMI_VIBRATION_ONLY) || (alertType == MMI_VIBRATION_AND_RING)
					|| (alertType == MMI_VIBRATION_THEN_RING))
	{
		if(!AlmIsTonePlaying())
			PlaySmsVibratorOnce();
	}	

	if(msg_popup_tonID)
	{
		#ifdef __MMI_SUSPEND_AUDIO_PLAY_WHEN_POPUP_NEW_MESSAGE_IND__
		if((msg_popup_tonID==(U8)MESSAGE_TONE)&&!(mdi_audio_is_background_play_suspended()))
		{
			mdi_audio_suspend_background_play();
			backgroundaudioresume=TRUE;
		}
		#endif
		//playRequestedTone(msg_popup_tonID);
		//StartTimer(MESSAGES_SMS_BOOTUP_TIMER_ID,700,playShortMessageTone);  //CHANGED BY JINZH:20070703
		//COS_Sleep(700);
		playShortMessageTone(msg_popup_tonID);
	}
	#ifdef __MMI_MULTI_SIM__
	g_msg_cntx.newMsgSimID = 0;
	#endif
}
void mmi_msg_new_msg_ind_stop_tone(void)
{
#ifdef __MMI_NOKIA_STYLE_N800__    
	if(msg_popup_tonID) 
	{
		stopRequestedTone(msg_popup_tonID);
		#ifdef __MMI_SUSPEND_AUDIO_PLAY_WHEN_POPUP_NEW_MESSAGE_IND__
		if(backgroundaudioresume)
		{
			mdi_audio_resume_background_play();
			backgroundaudioresume=FALSE;
		}
		#endif
	}
#endif

}
void mmi_msg_new_msg_ind_play_tone(void)
{
#ifdef __MMI_NOKIA_STYLE_N800__    
	U16 indstr;
	MMI_ALERT_TYPE alertType;
	MESSAGE_TONE_ENUM messagetonetype;//liuxl add the variavle to save message tone setting
	U16 pp_flag = 0;
	TRACE_AUD_FUNCTION();
       
        
       /* Framework support Popup display permisstion flag */
       /* Application who wanna stop popup has to intercept related messages and set the flag */
       pp_flag = GetInterruptPopupDisplay();
       if( (pp_flag & POPUP_DSPL_ON) == 0 )
       {
	       PlaySmsVibratorOnce();
      		return;
       }
       
	indstr=STR_RECEIVED_SMS;
	//wangzl: modify start for bug12082
	//alertType = GetMtCallAlertTypeEnum();
	//liuxl: modify start for bug 13164
	alertType = GetMtCallAlertTypeEnum();

	#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		switch(g_msg_cntx.newMsgSimID)
		{
			case 0:
				messagetonetype = GetMessageToneType();
				break;
			case 1:
				messagetonetype = GetCard2MessageToneType();
				break;
			case 2:
				break;
			case 3:
				break;
			default:
				break;
		}
	#else
		messagetonetype = GetMessageToneType();
	#endif

	msg_popup_tonID=0;
	
	if (messagetonetype !=MESSAGE_SILENT) //liuxl modify for fix bug-13164
	{	
		if (alertType != MMI_VIBRATION_ONLY) //if alertType is MMI_VIBRATION_ONLY,then can't play tone,so no need to get msg_popup_tonID
		{
			if (!GetTotalCallCount())
			{
				#ifndef __MMI_DUAL_SIM__
				msg_popup_tonID=(U8)MESSAGE_TONE;
				#else
				if (g_msg_cntx.isSlave == FALSE)
				{
					msg_popup_tonID = (U8) MESSAGE_TONE;
				}
				else
				{
					msg_popup_tonID = (U8) CARD2_MESSAGE_TONE;
				}
				#endif
			}
			else
				msg_popup_tonID=(U8)SMS_IN_CALL_TONE;
		}
	}
	if ((alertType == MMI_VIBRATION_ONLY) || (alertType == MMI_VIBRATION_AND_RING)
					|| (alertType == MMI_VIBRATION_THEN_RING))
	{
		if(!AlmIsTonePlaying())
			PlaySmsVibratorOnce();
	}	

	if(msg_popup_tonID)
	{
		#ifdef __MMI_SUSPEND_AUDIO_PLAY_WHEN_POPUP_NEW_MESSAGE_IND__
		if((msg_popup_tonID==(U8)MESSAGE_TONE)&&!(mdi_audio_is_background_play_suspended()))
		{
			mdi_audio_suspend_background_play();
			backgroundaudioresume=TRUE;
		}
		#endif
		//playRequestedTone(msg_popup_tonID);
		//StartTimer(MESSAGES_SMS_BOOTUP_TIMER_ID,700,playShortMessageTone);  //CHANGED BY JINZH:20070703
		//COS_Sleep(700);
		playShortMessageTone(msg_popup_tonID);
	}
	#ifdef __MMI_MULTI_SIM__
	g_msg_cntx.newMsgSimID = 0;
	#endif	
#endif

}
/*****************************************************************************
* FUNCTION
*  mmi_msg_exit_new_msg_popup
* DESCRIPTION
*   Exit new message popup.
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_exit_new_msg_popup (void)
{
	TRACE_AUD_FUNCTION();
	if(msg_popup_tonID) 
	{
		stopRequestedTone(msg_popup_tonID);
		#ifdef __MMI_SUSPEND_AUDIO_PLAY_WHEN_POPUP_NEW_MESSAGE_IND__
		if(backgroundaudioresume)
		{
			mdi_audio_resume_background_play();
			backgroundaudioresume=FALSE;
		}
		#endif
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_sending
* DESCRIPTION
*   Entry sending screen.
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/*
void mmi_msg_entry_sending (void)
{
	EntryNewScreen(SCR_ID_MSG_SENDING, mmi_msg_exit_generic, mmi_msg_entry_sending, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_sending");

	SetMessagesCurrScrnID (SCR_ID_MSG_SENDING);	// may be temp func...
	
	ShowCategory8Screen (STR_SENDING_SMS, 0, 0,0, STR_GLOBAL_ABORT, IMG_SMS_COMMON_NOIMAGE, 
							 STR_SENDIING_SMS_BODY, IMG_NEW_SMS_SEND, NULL);
	
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	SetKeyHandler(mmi_msg_abort_msg, KEY_END, KEY_EVENT_DOWN);	
	SetRightSoftkeyFunction (mmi_msg_abort_msg, KEY_EVENT_UP);
}
*/
/*****************************************************************************
* FUNCTION
*  mmi_msg_set_msg_num_hint
* DESCRIPTION
*   Set message number hint
*
* PARAMETERS
*  a  IN		total inbox
*  b  IN		total outbox
*  c  IN		total draftbox
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_msg_num_hint(U16 totalInbox, U16 totalOutbox, U16 totalDraftbox)
{
	S8 LocalData1[10];
	S8 LocalData2[10];
	S8 LocalData3[10];
	S8 LocalData1_unicode[10*2];
	S8 LocalData2_unicode[10*2];
	S8 LocalData3_unicode[10*2];
	sprintf(LocalData1, "%d ", totalInbox);
	sprintf(LocalData2, "%d ", totalOutbox);
	sprintf(LocalData3, "%d ", totalDraftbox);

	AnsiiToUnicodeString (LocalData1_unicode, LocalData1);
	AnsiiToUnicodeString (LocalData2_unicode, LocalData2);
	AnsiiToUnicodeString (LocalData3_unicode, LocalData3);
	pfnUnicodeStrcpy((PS8)TotalInboxOutboxStatus.totalInboxStr, LocalData1_unicode);
	pfnUnicodeStrncat ((PS8) TotalInboxOutboxStatus.totalInboxStr, GetString (STR_SMS_COUNT_POPUP_SUFFIX), MAX_SUBMENU_CHARACTERS); 
	
	pfnUnicodeStrcpy((PS8)TotalInboxOutboxStatus.totalOutboxStr,LocalData2_unicode);
	pfnUnicodeStrncat ((PS8) TotalInboxOutboxStatus.totalOutboxStr, GetString (STR_SMS_COUNT_POPUP_SUFFIX), MAX_SUBMENU_CHARACTERS);

#ifdef __MMI_MESSAGES_DRAFT_BOX__
	pfnUnicodeStrcpy((PS8)TotalInboxOutboxStatus.totalDraftboxStr,LocalData3_unicode);
	pfnUnicodeStrncat ((PS8) TotalInboxOutboxStatus.totalDraftboxStr, GetString (STR_SMS_COUNT_POPUP_SUFFIX), MAX_SUBMENU_CHARACTERS);
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_inline_generic_custom_function
* DESCRIPTION
*   Set LSK handle for inline full screen edit
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_inline_generic_custom_function (void)
{
	SetInputMethodAndDoneCaptionIcon(IMG_SMS_ENTRY_SCRN_CAPTION);
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
	SetLeftSoftkeyFunction (EntryScrForInputMethodAndDone, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_inline_generic_done
* DESCRIPTION
*   Highlight inline done handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
// wangbei add start 20060614
pBOOL gChatInvitation;
/*void AlmDisableSPOF(void) 
{
}*/	//delete by liuxn 060616
/*
pBOOL CheckForInvitationMsg(PU8 data,S32 length,pBOOL flag)
{
	 
}
void SetChatInboxIndex(U32 index)
{
}
void RemoveInvitationChar(void)
{
}
U16 ChatUseNumberScreen(void)
{
	return 0;
}
void InsertTemplateToChat (S8 * templ)
{
}
//void EntryChatApp(void) 
//{
//}
void ChatInboxInterface(PU8 destnum)
{
}
*/
// wangbei add end
void mmi_msg_highlight_inline_generic_done (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
								ConfirmInlineFullScreenEdit, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  GetFirstSemiOctetTarget
* DESCRIPTION
*   Get first semi octet
*
* PARAMETERS
*  a  IN		input character
* RETURNS
*  first semi octet of input
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 GetFirstSemiOctetTarget(U8 ch)
{
	return (ch & 0xf0) >> 4;
}

/*****************************************************************************
* FUNCTION
*  GetSecondSemiOctetTarget
* DESCRIPTION
*   Get second semi octet
*
* PARAMETERS
*  a  IN		input character
* RETURNS
*  second semi octet of input
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 GetSecondSemiOctetTarget(U8 ch)
{
	return (ch & 0x0f);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_init_msg_data
* DESCRIPTION
*   Init message data
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   msgbox_info
*****************************************************************************/
#ifndef MMI_ON_HARDWARE_P
extern void InitSMSSetting(void);
#endif
void mmi_msg_init_msg_data (void)
{
	U8 i=0;
#ifndef MMI_ON_HARDWARE_P	
//	U8 index=0;
#endif
	memset(&TotalInboxOutboxStatus, 0, sizeof(msg_in_out_struct));
	memset(&activeProfile, 0, sizeof(msg_active_profile_struct));
#ifdef MMI_ON_HARDWARE_P	
	g_msg_cntx.msg_init=FALSE; //init to be false
#else
	g_msg_cntx.msg_init=TRUE; //init to be false
#endif
	g_msg_cntx.number_from_phb=0; //init to be 0
	g_msg_cntx.msg_ind_index=SMSAL_INVALID_INDEX;
	g_msg_cntx.msg_ind_in_idle=FALSE;
	g_msg_cntx.msg_ind_after_call=FALSE;	
	g_msg_cntx.msg_class0_ind=FALSE;
	
	
	g_msg_cntx.msg_deliver_ind=FALSE;
	g_msg_cntx.msg_waiting_ind=FALSE;
	
	g_msg_cntx.toDisplayMessageList = TO_DISPLAY_MESSAGE_LIST_NONE;
	g_msg_cntx.sendSaveStatus = SEND_SAVE_NIL;
	g_msg_cntx.sendMessageCase=SEND_CASE_SEND_ONLY;
	g_msg_cntx.PhbSmsInterfaceState=MMI_PHB_SMS_INTERFACE_NONE;
	g_msg_cntx.msg_status=MSG_STATUS_IDLE;
	g_msg_cntx.msg_class0_p=NULL;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		g_msg_cntx.msg_full_ind[i] = FALSE;
		g_msg_cntx.msg_exceed_ind[i]=FALSE;
	}
	#ifdef __MMI_MULTI_SIM__
	g_msg_cntx.needSelectSIM = FALSE;
	#endif
//	g_msg_cntx.msg_class0_p->data=NULL;
	for(i=0; i<MAX_STATUS_REPORT_NUM; i++)
		g_msg_cntx.msg_deliver_report[i]=NULL;
	memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
	memset(&g_msg_cntx.msg_waiting_info, 0, MAX_WAITING_LINE_NUM*sizeof(msg_waiting_ind_struct));
#if defined(__MMI_MESSAGES_SEND_BY_GROUP__)|| defined(__MMI_MESSAGES_SEND_TO_MANY__)
	memset(&(g_msg_cntx.msg_send_info), 0, sizeof(msg_send_many_struct));
#endif
	#ifdef __MMI_MESSAGES_TEMPLATE__
	for(i=0; i<NUM_TEMPLATES; i++)
		g_msg_cntx.templates[i]=NULL;
	memset(g_msg_cntx.scratchTemplRec, 0, NVRAM_SMS_TEMPL_RECORD_SIZE+ENCODING_LENGTH);
	#endif
#ifdef __MMI_MESSAGES_USE_URL__
	memset(g_msg_cntx.URLlist, 0x00, sizeof(g_msg_cntx.URLlist));
	g_msg_cntx.URLCount = 0;
#endif	/* __MMI_MESSAGES_USE_URL__ */
#ifndef MMI_ON_HARDWARE_P
	InitSMSSetting();
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_deinit_msg_data
* DESCRIPTION
*   Deinit message data
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_deinit_msg_data (void)
{
	U8 i=0;
	if(g_msg_cntx.msg_class0_p!=NULL)
	{
		OslMfree(g_msg_cntx.msg_class0_p);
		g_msg_cntx.msg_class0_p=NULL;
	}
	for(i=0; i<MAX_STATUS_REPORT_NUM; i++)
	{
		if(g_msg_cntx.msg_deliver_report[i]!=NULL)
			OslMfree(g_msg_cntx.msg_deliver_report[i]);
		g_msg_cntx.msg_deliver_report[i]=NULL;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_write_msg
* DESCRIPTION
*   Enable schedule power off before go back screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_write_msg (void)
{
	AlmEnableSPOF();
	GoBackHistory();
}

#ifdef __SENDKEY2_SUPPORT__
/*****************************************************************************
 * FUNCTION
 *  mmi_msg_entry_master_send_option
 * DESCRIPTION
 *  enter master send option
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_entry_master_send_option(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
	MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
	MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_MASTER);	
    
   	 mmi_msg_entry_send_option();
}

/*****************************************************************************
 * FUNCTION
 *  mmi_msg_entry_slave_send_option
 * DESCRIPTION
 *  enter master send option
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_entry_slave_send_option(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
	MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
	MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);	
    
    mmi_msg_entry_send_option();
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_write_msg
* DESCRIPTION
*   Entry write message.
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern msg_ems_data_struct 	bufForEdit ;   
extern  kal_uint8 ems_input_buffer[1224];               
void mmi_msg_entry_write_msg (void)   
{ 
	U8* 		guiBuffer=NULL;
	EMSData*	pEMS;
	
#ifdef __FLIGHT_MODE_SUPPORT__ 
	#ifdef __MMI_MULTI_SIM__
	if(MTPNP_AD_Is_Flight_Mode())
	{
		DisplayPopup((PU8)GetString(STRING_MTPNP_DM_FLIGHT) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		return;
	}
	#endif		
#endif

	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	if(mmi_frm_sms_check_action_pending())
	{
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
	
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
#else
	EntryNewScreen(SCR_ID_MSG_WRITE, mmi_msg_exit_generic, mmi_msg_entry_write_msg, NULL);
#endif
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_write_msg");	

	AlmDisableSPOF();
	
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_WRITE);	

	SetMessagesCurrScrnID(SCR_ID_MSG_WRITE);
	
	GetEMSDataForEdit (& pEMS, 0);

	SetMessagesCurrScrnID(SCR_ID_MSG_WRITE);

#if defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else
	ShowCategory28Screen (STR_WMESSAGE_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION,
						  			STR_GLOBAL_OPTIONS, IMG_SMS_COMMON_NOIMAGE,
						  			STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE,
						  			pEMS, guiBuffer );
	SetLeftSoftkeyFunction (mmi_msg_entry_option_write, KEY_EVENT_UP);
#if defined(__MMI_GB__)
	GBSetInputboxLSKFunction(mmi_msg_entry_option_write);  
#endif

#ifdef __MMI_MULTI_SIM__
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	SetLeftSoftkeyFunction (mmi_msg_entry_master_send_option, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_entry_master_send_option, KEY_SEND, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_entry_option_write, KEY_ENTER, KEY_EVENT_UP);
#elif defined(TGT_GALLITE_G800)
	#ifdef __SENDKEY2_SUPPORT__
	SetLeftSoftkeyFunction (mmi_msg_entry_option_write, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_entry_option_write, KEY_SEND1, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_entry_option_write, KEY_SEND2, KEY_EVENT_UP);
	#endif
	SetKeyHandler(mmi_msg_entry_option_write, KEY_ENTER, KEY_EVENT_UP);

#else
#ifdef  TGT_TD_4SIM
	SetLeftSoftkeyFunction (mmi_msg_entry_option_write, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_entry_option_write, KEY_SEND, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_entry_option_write, KEY_ENTER, KEY_EVENT_UP);
#else
	//SetKeyHandler(mmi_msg_entry_option_write, KEY_SEND, KEY_EVENT_UP);
	#ifdef __MMI_USE_LSK_AS_SEND2__
	bUseLSKAsSendKey2 = TRUE;
	SetKeyHandler(get_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY),KEY_ENTER,KEY_EVENT_UP);
	#endif
	#if (defined __SENDKEY2_SUPPORT__ &&(MMI_SIM_NUMBER == 2))
	if (MTPNP_AD_Get_UsableSide()==MTPNP_AD_SIMCARD1_USABLE)
	{
	    SetKeyHandler(mmi_msg_entry_master_send_option, KEY_SEND1, KEY_EVENT_UP);
	    SetKeyHandler(mmi_msg_entry_master_send_option, KEY_SEND2, KEY_EVENT_UP);

	    SetKeyHandler(mmi_msg_entry_master_send_option, KEY_SEND, KEY_EVENT_UP);
	    #ifdef __MMI_USE_LSK_AS_SEND2__
	    SetLeftSoftkeyFunction(mmi_msg_entry_master_send_option, KEY_EVENT_UP);
	    #endif
	}
	else if(MTPNP_AD_Get_UsableSide()==MTPNP_AD_SIMCARD2_USABLE)
	{
	    SetKeyHandler(mmi_msg_entry_slave_send_option, KEY_SEND1, KEY_EVENT_UP);
	    SetKeyHandler(mmi_msg_entry_slave_send_option, KEY_SEND2, KEY_EVENT_UP);

	    SetKeyHandler(mmi_msg_entry_slave_send_option, KEY_SEND, KEY_EVENT_UP);
	    #ifdef __MMI_USE_LSK_AS_SEND2__
	    SetLeftSoftkeyFunction(mmi_msg_entry_slave_send_option, KEY_EVENT_UP);
	    #endif
	}
	else if(MTPNP_AD_Get_UsableSide()==MTPNP_AD_DUALSIM_USABLE)
	{
	    SetKeyHandler(mmi_msg_entry_master_send_option, KEY_SEND1, KEY_EVENT_UP);
	    SetKeyHandler(mmi_msg_entry_slave_send_option, KEY_SEND2, KEY_EVENT_UP);

	    SetKeyHandler(mmi_msg_entry_master_send_option, KEY_SEND, KEY_EVENT_UP);
	    #ifdef __MMI_USE_LSK_AS_SEND2__
	    SetLeftSoftkeyFunction(mmi_msg_entry_slave_send_option, KEY_EVENT_UP);
	    #endif
	}
	#if defined(__MMI_GB__)
  	GBSetInputboxLSKFunction(get_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY));  
   	#endif
    #else
    SetKeyHandler(mmi_msg_entry_option_write, KEY_SEND1, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_entry_option_write, KEY_SEND2, KEY_EVENT_UP);
    SetKeyHandler(mmi_msg_entry_option_write, KEY_SEND3, KEY_EVENT_UP);
	#endif /* __SENDKEY2_SUPPORT__ */
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
#endif
#else
	SetKeyHandler(mmi_msg_entry_send_option, KEY_SEND, KEY_EVENT_UP);
#endif
	RegisterInputMethodScreenCloseFunction (Messages2GoBackHistory);
	SetCategory28RightSoftkeyFunction (mmi_msg_go_back_from_write_msg, KEY_EVENT_UP);

#endif	/*	__MMI_SMART_MESSAGE_MO__  && !__MMI_MESSAGES_EMS__ */

}

#if defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_option_write
* DESCRIPTION
*   Entry write option
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_option_write (void)
{
	U8* guiBuffer=NULL;
	U16 numItems=0;
	U16 nStrItemList[MAX_SUB_MENUS];
#if defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
#endif

	EntryNewScreen(SCR_ID_MSG_WRITE_OPTION, mmi_msg_exit_generic, mmi_msg_entry_option_write, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_option_write");	

#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	MTPNP_AD_SetMenuHideUnhide(EMS_ED_OPT_DONE);
#endif
	
#if defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)
	/* under construction !*/
	/* under construction !*/
#else
	numItems = GetNumOfChild_Ext(EMS_ED_OPTIONS_MENU_ID);
	GetSequenceStringIds_Ext(EMS_ED_OPTIONS_MENU_ID, nStrItemList);
	SetParentHandler (EMS_ED_OPTIONS_MENU_ID);
#endif	

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_WRITE_OPTION);		 

	SetMessagesCurrScrnID(SCR_ID_MSG_WRITE_OPTION);
	#ifdef __MMI_MULTI_SIM__
	RegisterHighlightHandler( MultiSimMenuHighLightHandler );
	#else
	RegisterHighlightHandler( ExecuteCurrHiliteHandler);
	#endif

	ShowCategory52Screen (STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION, 
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						numItems, nStrItemList, (U16 *)gIndexIconsImageList,
						NULL , 0, 0, guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_phone_number_empty
* DESCRIPTION
*   Set LSK handle for number screen when input empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_phone_number_empty (void)
{
	ChangeLeftSoftkey (STR_SMS_COMMON_SEARCH, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (mmi_msg_search_phb, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_phone_number_not_empty
* DESCRIPTION
*   Set LSK handle for number screen when input non empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_phone_number_not_empty (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
#ifdef __MMI_MESSAGES_SEND_TO_MANY__
	if(IsScreenPresent(SCR_ID_MSG_SEND_MANY_LIST))
		SetLeftSoftkeyFunction (mmi_msg_save_phone_number, KEY_EVENT_UP);
	else
#endif		
	SetLeftSoftkeyFunction (mmi_msg_send_msg_to_only_entry, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_phone_number
* DESCRIPTION
*   Entry phone number screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern UINT16 TitlePlaceFlag;
void mmi_msg_entry_phone_number (void)
{
	U8* 	guiBuffer;

	TitlePlaceFlag = 1;
	EntryNewScreen(SCR_ID_MSG_NUMBER, mmi_msg_exit_generic, mmi_msg_entry_phone_number, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_phone_number");

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_NUMBER);		//maybe assign NULL directly


	
	RegisterInputBoxEmptyFunction (mmi_msg_phone_number_empty);
	RegisterInputBoxNotEmptyFunction (mmi_msg_phone_number_not_empty);
	SetMessagesCurrScrnID(SCR_ID_MSG_NUMBER);
	ShowCategory5Screen (STR_ENTER_PHONE_NUMBER_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
						  STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE,
						  STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE, 
						  INPUT_TYPE_PHONE_NUMBER/*|INPUT_TYPE_PLUS_CHARACTER_HANDLING*/,
						  (U8*) g_msg_cntx.smsPhoneNumber, MAX_DIGITS_SMS, guiBuffer); 

	SetKeyHandler(mmi_msg_send_msg_to_only_entry, KEY_SEND, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_phone_number_not_empty
* DESCRIPTION
*   Set LSK handle for number screen when input non empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_popup_generic(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId)
{
	EntryNewScreen(SCR_ID_MSG_POPUP, mmi_msg_exit_popup_generic, NULL, NULL);
	ShowCategory65Screen(string,imageId,NULL);
	SetGroupKeyHandler(NULL,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,KEY_EVENT_DOWN);
	SetGroupKeyHandler(mmi_msg_popup_timeout,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,KEY_EVENT_UP);

	StartTimer(POPUP_TIMER,popupDuration,mmi_msg_popup_timeout);
	msg_popup_tonID=toneId;
	if(msg_popup_tonID) playRequestedTone(msg_popup_tonID);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_phone_number_not_empty
* DESCRIPTION
*   Set LSK handle for number screen when input non empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_exit_popup_generic (void)
{
	StopTimer(POPUP_TIMER);
	if(msg_popup_tonID) stopRequestedTone(msg_popup_tonID);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_phone_number_not_empty
* DESCRIPTION
*   Set LSK handle for number screen when input non empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_popup_timeout (void)
{
	StopTimer(POPUP_TIMER);
	if(msg_popup_tonID) 
		stopRequestedTone(msg_popup_tonID);
	mmi_msg_exec_funcptr();
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_send_option
* DESCRIPTION
*   Entry send option screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_send_option (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
	U16 titleID = STR_SEND_OPTIONS_CAPTION;

	if(g_msg_cntx.sendMessageCase == SEND_CASE_REPLY)
	{
//		mmi_msg_reply_msg();
//		return;
	}
	else if ((g_msg_cntx.sendMessageCase == SEND_CASE_SEND_FROM_PHB) || (g_msg_cntx.sendMessageCase == SEND_CASE_SEND_FROM_CLG))
	{
		mmi_msg_send_msg_to_only_entry();
		return;
	}


	EntryNewScreen(SCR_ID_MSG_SEND_OPTION, mmi_msg_exit_generic, mmi_msg_entry_send_option, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_send_option");

	if(g_msg_cntx.sendMessageCase != SEND_CASE_REPLY)
		g_msg_cntx.sendMessageCase = SEND_CASE_SEND_ONLY; //reset
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_SEND_OPTION);
	
	if(g_msg_cntx.sendMessageCase == SEND_CASE_REPLY)
	{
		numItems = 3;
		GetSequenceStringIds(EMS_ED_OPT_DONE, nStrItemList);	
		SetParentHandler (EMS_ED_OPT_DONE);
	}
	else
	{
		numItems = GetNumOfChild_Ext(EMS_ED_OPT_DONE);
		GetSequenceStringIds_Ext(EMS_ED_OPT_DONE, nStrItemList);	
		SetParentHandler (EMS_ED_OPT_DONE);

	#if !defined( __MMI_DUAL_SIM_SINGLE_OPENED_)&&defined(__MMI_MULTI_SIM__)
		titleID = STRING_MTPNP_MASTER_SEND_OPTIONS_CAPTION + MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_SMS_CHANNEL);
	#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
	}
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_SEND_OPTION);
		
	ShowCategory52Screen(titleID,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_option_inbox
* DESCRIPTION
*   Entry inbox option screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_option_inbox (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
#ifdef __MMI_MESSAGES_COPY__
	U8	storageType;
#endif

	EntryNewScreen(SCR_ID_MSG_INBOX_OPTION, mmi_msg_exit_generic, mmi_msg_entry_option_inbox, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_option_inbox");
	/* the deleting screen start point has to be reset to inbox msg screen if inbox option is entered */
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_INBOX_MSG);
	g_msg_cntx.sendMessageCase = SEND_CASE_SEND_ONLY; //reset
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_INBOX_OPTION);
	
#if defined(__MMI_MESSAGES_USE_URL__)
	/* Hide USE URL menu item if incall or wap screen is present */
	if (GetTotalCallCount() > 0 || IsScreenPresent(WAP_SCREEN_WIDGET))
		mmi_frm_hide_menu_item(MENU_ID_SMS_INBOX_OPT_USE_URL);
	else
		mmi_frm_unhide_menu_item(MENU_ID_SMS_INBOX_OPT_USE_URL);
#endif
		
	numItems = GetNumOfChild_Ext(SMS_INBOX_OPTIONS_MENUID);	
	GetSequenceStringIds_Ext(SMS_INBOX_OPTIONS_MENUID, nStrItemList);	
#ifdef __MMI_MESSAGES_COPY__
	storageType = mmi_frm_sms_get_storage(MMI_FRM_SMS_INBOX, (U16)g_msg_cntx.currBoxIndex);

	/*default : copy_to_phone*/
	if(storageType == SMSAL_ME)
	{
		int i =0;
		for(i=0;i<MAX_SUB_MENUS;i++)
		{
			if(nStrItemList[i]==STR_SMS_COPY_TO_PHONE)
			{
				nStrItemList[i]=STR_SMS_COPY_TO_SIM;
				break;
			}
		}
		for(i=0;i<MAX_SUB_MENUS;i++)
		{
			if(nStrItemList[i]==STR_SMS_MOVE_TO_PHONE)
			{
				nStrItemList[i]=STR_SMS_MOVE_TO_SIM;
				break;
			}
		}
	}
#endif	
	SetParentHandler (SMS_INBOX_OPTIONS_MENUID);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_INBOX_OPTION);
		
	ShowCategory52Screen(STR_SCR6028_CAPTION,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_option_outbox
* DESCRIPTION
*   Entry outbox option screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_option_outbox (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
#ifdef __MMI_MESSAGES_COPY__
	U8	storageType=0;
#endif

	EntryNewScreen(SCR_ID_MSG_OUTBOX_OPTION, mmi_msg_exit_generic, mmi_msg_entry_option_outbox, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_option_outbox");
	/* the deleting screen start point has to be reset to outbox msg screen if outbox option is entered */
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_OUTBOX_MSG);
	g_msg_cntx.sendMessageCase = SEND_CASE_SEND_ONLY; //reset
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_OUTBOX_OPTION);
	
#if defined(__MMI_MESSAGES_USE_URL__)
	/* Hide USE URL menu item if incall or wap screen is present */
	if (GetTotalCallCount() > 0 || IsScreenPresent(WAP_SCREEN_WIDGET))
		mmi_frm_hide_menu_item(MENU_ID_SMS_OUTBOX_OPT_USE_URL);
	else
		mmi_frm_unhide_menu_item(MENU_ID_SMS_OUTBOX_OPT_USE_URL);
#endif	
	
	numItems = GetNumOfChild_Ext(SMS_OUTBOX_OPTIONS_MENUID);	
	GetSequenceStringIds_Ext(SMS_OUTBOX_OPTIONS_MENUID, nStrItemList);	
#ifdef __MMI_MESSAGES_COPY__
	if(g_msg_cntx.toDisplayMessageList==TO_DISPLAY_MESSAGE_LIST_OUTBOX)
		storageType = mmi_frm_sms_get_storage(MMI_FRM_SMS_OUTBOX, (U16)g_msg_cntx.currBoxIndex);
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
	else
		storageType = mmi_frm_sms_get_storage(MMI_FRM_SMS_DRAFTS, (U16)g_msg_cntx.currBoxIndex);
	#endif	

	/*default : copy_to_phone*/
	if(storageType == SMSAL_ME)
	{
		int i =0;
		for(i=0;i<MAX_SUB_MENUS;i++)
		{
			if(nStrItemList[i]==STR_SMS_COPY_TO_PHONE)
			{
				nStrItemList[i]=STR_SMS_COPY_TO_SIM;
				break;
			}
		}
		for(i=0;i<MAX_SUB_MENUS;i++)
		{
			if(nStrItemList[i]==STR_SMS_MOVE_TO_PHONE)
			{
				nStrItemList[i]=STR_SMS_MOVE_TO_SIM;
				break;
			}
		}
	}
#endif	
	SetParentHandler (SMS_OUTBOX_OPTIONS_MENUID);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_OUTBOX_OPTION);
		
	ShowCategory52Screen(STR_SCR6028_CAPTION,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_option_default
* DESCRIPTION
*   Entry default option screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_option_default (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_MSG_DEFAULT_OPTION, mmi_msg_exit_generic, mmi_msg_entry_option_default, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_option_default");
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_DEFAULT_OPTION);
	
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
	if (IsScreenPresent(SCR_ID_MSG_OUTBOX_LIST) || IsScreenPresent(SCR_ID_MSG_DRAFTBOX_LIST))
	#else
	if (IsScreenPresent(SCR_ID_MSG_OUTBOX_LIST))
	#endif
	{
		numItems = GetNumOfChild (SMS_OUTBOX_DEFAULT_OPTIONS_MENUID);	
		GetSequenceStringIds(SMS_OUTBOX_DEFAULT_OPTIONS_MENUID, nStrItemList);	
		SetParentHandler (SMS_OUTBOX_DEFAULT_OPTIONS_MENUID);
	}
	else
	{
	numItems = GetNumOfChild (SMS_INBOX_SR_OPTIONS_MENUID);	
	GetSequenceStringIds(SMS_INBOX_SR_OPTIONS_MENUID, nStrItemList);	
	SetParentHandler (SMS_INBOX_SR_OPTIONS_MENUID);
	}
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_DEFAULT_OPTION);
		
	ShowCategory52Screen(STR_SCR6028_CAPTION,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}

#ifdef __MMI_MESSAGES_CHAT__
/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_option_chat
* DESCRIPTION
*   Entry chat message option screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_option_chat (void)
{ 
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_MSG_CHAT_INVITATION, mmi_msg_exit_generic, mmi_msg_entry_option_chat, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_option_chat");
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CHAT_INVITATION);
	
	numItems = GetNumOfChild (SMS_CHAT_INVITATION_MENUID);	
	GetSequenceStringIds(SMS_CHAT_INVITATION_MENUID, nStrItemList);	
	SetParentHandler (SMS_CHAT_INVITATION_MENUID);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_CHAT_INVITATION);
		
	ShowCategory52Screen(STR_GLOBAL_OPTIONS,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_send_option_outbox
* DESCRIPTION
*   Entry outbox send option screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_send_option_outbox (void)
{
	U8 	result = EMS_OK;
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

#if defined(__MMI_SMART_MESSAGE_MT__) || defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif	/* __MMI_SMART_MESSAGE_MT__ || __MMI_SMART_MESSAGE_MO__ && !__MMI_MESSAGES_EMS__ */


	EntryNewScreen(SCR_ID_MSG_OUTBOX_SEND_OPTION, mmi_msg_exit_generic, mmi_msg_entry_send_option_outbox, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_send_option_outbox");

	result = CopyEMSViewToEditBuffer();
	/* copy view content to edit buffer maybe fail due to EMS LIB virtual pack fail */
	if (result != EMS_OK)
	{
		DisplayPopup((PU8)GetString(STR_SMS_FAILURE_MSG_LEN_EXCEEDED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		DeleteScreenIfPresent(SCR_ID_MSG_OUTBOX_SEND_OPTION);
		return;
	}	
	g_msg_cntx.sendMessageCase = SEND_CASE_SEND_ONLY; //reset
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_OUTBOX_SEND_OPTION);
	
	numItems = GetNumOfChild (SMS_OUTBOX_OPT_SEND_MENUID);	
	GetSequenceStringIds(SMS_OUTBOX_OPT_SEND_MENUID, nStrItemList);	
	SetParentHandler (SMS_OUTBOX_OPT_SEND_MENUID);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_OUTBOX_SEND_OPTION);
		
	#if defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#endif

	#if defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#endif
	
	ShowCategory52Screen(STR_SEND_OPTIONS_CAPTION,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
	#ifdef __MMI_MULTI_SIM__
	mmi_msg_set_msg_need_select_sim_when_send(TRUE);	
	#endif
}

#ifdef __MMI_SEND_BY_SEARCH_NAME__	
/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_send_only
* DESCRIPTION
*   Entry send only screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_send_only (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_MSG_SEND_ONLY_OPTION, mmi_msg_exit_generic, mmi_msg_entry_send_only, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_send_only");

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_SEND_ONLY_OPTION);
	
	numItems = GetNumOfChild (SEND_OPT_SEND_ONLY_MENUID);	
	GetSequenceStringIds(SEND_OPT_SEND_ONLY_MENUID, nStrItemList);	
	SetParentHandler (SEND_OPT_SEND_ONLY_MENUID);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_SEND_ONLY_OPTION);
		
	ShowCategory52Screen(STR_SEND_ONLY_MENU_ITEM,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}
#endif

#if defined (__MMI_MESSAGES_SEND_TO_MANY__)
/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_send_to_many_list_option
* DESCRIPTION
*   Entry send to many list option
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_send_to_many_list_option (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_MSG_SEND_MANY_LIST_OPTION, mmi_msg_exit_generic, mmi_msg_entry_send_to_many_list_option, NULL);	

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_SEND_MANY_LIST_OPTION);
	
	numItems = GetNumOfChild (SEND_OPT_SEND_TO_MANY_BY_LIST_OPTION);	
	GetSequenceStringIds(SEND_OPT_SEND_TO_MANY_BY_LIST_OPTION, nStrItemList);	
	SetParentHandler (SEND_OPT_SEND_TO_MANY_BY_LIST_OPTION);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_SEND_MANY_LIST_OPTION);
		
	ShowCategory52Screen(STR_GLOBAL_OPTIONS,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_send_to_many
* DESCRIPTION
*   Entry send to many screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_send_to_many (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_MSG_SEND_MANY_OPTION, mmi_msg_exit_generic, mmi_msg_entry_send_to_many, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_send_to_many");

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_SEND_MANY_OPTION);
	
	numItems = GetNumOfChild (SEND_OPT_SEND_TO_MANY_MENUID);	
	GetSequenceStringIds(SEND_OPT_SEND_TO_MANY_MENUID, nStrItemList);	
	SetParentHandler (SEND_OPT_SEND_TO_MANY_MENUID);	
	
	g_msg_cntx.sendMessageCase = SEND_CASE_SEND_TO_MANY;
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_SEND_MANY_OPTION);
		
	ShowCategory52Screen(STR_SENDTOMANY_MENUENTRY,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_send_to_many_list
* DESCRIPTION
*   Entry send to many list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_send_to_many_list (void)
{
	U8*	guiBuffer=NULL;
	S32	hiliteitem=0;	
	

	EntryNewScreen(SCR_ID_MSG_SEND_MANY_LIST, mmi_msg_exit_generic, mmi_msg_entry_send_to_many_list, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_send_to_many_list");

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_SEND_MANY_LIST);

	if(guiBuffer!=NULL)
		hiliteitem=(g_msg_cntx.currHiliteIndex<MAX_SEND_TO_MANY_ITEMS)?g_msg_cntx.currHiliteIndex:0;

	SetMessagesCurrScrnID(SCR_ID_MSG_SEND_MANY_LIST);

	RegisterHighlightHandler (GetHiliteIndex);

	ShowCategory184Screen(STR_SENDTOMANY_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION, 
							STR_GLOBAL_EDIT, IMG_SMS_COMMON_NOIMAGE,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							MAX_SEND_TO_MANY_ITEMS,
							mmi_msg_send_to_many_list_get_item, 
							mmi_msg_send_to_many_list_get_hint , 
							hiliteitem, 
							guiBuffer);
	
	SetLeftSoftkeyFunction (mmi_msg_entry_send_to_many_list_option, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_entry_send_to_many_list_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_send_to_many_list_get_item
* DESCRIPTION
*   Get send to many list item
*
* PARAMETERS
*  a  IN		item index
*  b  IN/OUT	string buffer
*  c  IN/OUT	image buffer pointer
*  b  IN/OUT	string image mask
* RETURNS
*  TRUE
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_msg_send_to_many_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	S8  temp[MAX_DIGITS*ENCODING_LENGTH];
	MTPNP_CHAR *name;

	if((item_index < 0) || (item_index > MAX_SEND_TO_MANY_ITEMS))
		return FALSE;
	
	pfnUnicodeStrcpy(temp, (S8*)g_msg_cntx.msg_send_info.PhbNumber[item_index]);
	/* do not show name in send to many list in case of duplicated entries in the phonebook */
#if defined(__PROJECT_GALLITE_C01__)
	name = lookUpNumber ((S8*)g_msg_cntx.msg_send_info.PhbNumber[item_index]);
	if(pfnUnicodeStrlen(name))
		pfnUnicodeStrcpy((S8*)str_buff, name);
	else
#endif
	{
		if(pfnUnicodeStrlen(temp))
			pfnUnicodeStrcpy((S8*)str_buff, temp);
		else
			pfnUnicodeStrcpy((S8*)str_buff, (S8*)GetString (STR_GLOBAL_EMPTY_LIST));
	}
	
	*img_buff_p = get_image(gIndexIconsImageList[item_index]);
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_send_to_many_list_get_hint
* DESCRIPTION
*   Get send to many list hint
*
* PARAMETERS
*  a  IN		item index
*  b  IN/OUT	hint array
* RETURNS
*  TRUE: hint display, FALSE: hint not display
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_msg_send_to_many_list_get_hint ( S32 item_index, UI_string_type *hint_array )
{
	if((item_index < 0) || (item_index > MAX_SEND_TO_MANY_ITEMS))
		return FALSE;

        /* do not show name in send to many list in case of duplicated entries in the phonebook */
	return 0;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_phone_number_from_send_to_many
* DESCRIPTION
*   Entry phone number from send to many list edit
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_phone_number_from_send_to_many(void)
{
	memset((S8*)g_msg_cntx.smsPhoneNumber, 0, ENCODING_LENGTH);
	pfnUnicodeStrcpy((S8*)g_msg_cntx.smsPhoneNumber, (S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.currHiliteIndex]);
	mmi_msg_entry_phone_number();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_save_phone_number
* DESCRIPTION
*   Save phone number for send to many list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_save_phone_number(void)
{
	memset((S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.currHiliteIndex], 0, ENCODING_LENGTH);
	pfnUnicodeStrcpy((S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.currHiliteIndex], (S8*)g_msg_cntx.smsPhoneNumber);
	Messages2GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_erase_phone_number
* DESCRIPTION
*   Erase phone number for send to many list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_erase_phone_number(void)
{
	memset((S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.currHiliteIndex], 0, ENCODING_LENGTH);
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_SEND_MANY_LIST_OPTION);
	DisplayPopup ((PU8)GetString (STR_MESSAGES_ERACED_ID), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	DeleteMessagesHistoryNodes ();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_erase_phone_number
* DESCRIPTION
*   Before erase phone number, see if it is empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_erase_phone_number(void)
{
	if(pfnUnicodeStrlen((S8*)g_msg_cntx.msg_send_info.PhbNumber[g_msg_cntx.currHiliteIndex])==0)
	{
		SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_SEND_MANY_LIST_OPTION);
		DisplayPopup ((PU8)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
		DeleteMessagesHistoryNodes();
	}
	else
	{
		mmi_msg_set_confirm_screen(STR_MESSAGES_ERACE_Q_ID, mmi_msg_erase_phone_number, Messages2GoBackHistory);
		mmi_msg_entry_confirm_generic();
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_send_to_many_list_edit
* DESCRIPTION
*   Highlight send to many list edit handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_send_to_many_list_edit (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_entry_phone_number_from_send_to_many, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_send_to_many_list_erase
* DESCRIPTION
*   Highlight send to many list erase handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_send_to_many_list_erase (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_pre_erase_phone_number, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_msg_date_time
* DESCRIPTION
*   Get message date time
*
* PARAMETERS
*  a  IN/OUT	date string
*  b  IN/OUT	time string
*  c  IN		scts
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_get_msg_date_time (S8* dateStr, S8* timeStr, U8 scts[])
{
	UI_character_type d[MAX_TIMESTAMP_SIZE*ENCODING_LENGTH];
	UI_character_type t[MAX_TIMESTAMP_SIZE*ENCODING_LENGTH];
	UI_time	datetime;

	if (dateStr)
	{
		datetime.nDay = scts[2];
		datetime.nMonth= scts[1];
		datetime.nYear= scts[0] + 2000;
		if (datetime.nYear>2090)
			datetime.nYear=datetime.nYear-100;
		date_string(&datetime,(UI_string_type)d,DT_IDLE_SCREEN);
		memset(dateStr, 0, (MAX_TIMESTAMP_SIZE+1)*ENCODING_LENGTH);
		memcpy(dateStr, (S8*)d, MAX_TIMESTAMP_SIZE*ENCODING_LENGTH);
	}

	if (timeStr)
	{
		datetime.nHour= scts[3];
		datetime.nMin= scts[4];
		datetime.nSec= scts[5];
		time_string(&datetime,(UI_string_type)t,DT_IDLE_SCREEN); 
		memset(timeStr, 0, (MAX_TIMESTAMP_SIZE+1)*ENCODING_LENGTH);
		memcpy(timeStr, (S8*)t, MAX_TIMESTAMP_SIZE*ENCODING_LENGTH);		
	}

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_inbox_list
* DESCRIPTION
*   Before enter inbox list, check if reget list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/*
U8 mmi_msg_handle_ready_ind_inbox(void* dummy)
{
	mmi_msg_entry_inbox_list();
	DeleteScreenIfPresent(SCR_ID_MSG_PROCESSING);
	return FALSE;
}
*/
void mmi_msg_pre_entry_inbox_list (void)
{
#ifdef __FLIGHT_MODE_SUPPORT__ 
#ifdef __MMI_MULTI_SIM__
	if(MTPNP_AD_Is_Flight_Mode())
	{
		DisplayPopup((PU8)GetString(STRING_MTPNP_DM_FLIGHT) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		return;
	}
#endif		
#endif

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SMS_Set_Filter_Rule(MTPNP_AD_FILTER_ALL);
#endif

#ifdef MMI_ON_HARDWARE_P
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX)==0)
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	else if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX)==MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_msg_set_processing_screen(STR_MESSAGE_MAIN_MENU_CAPTION, STR_LOADING_SMS, IMG_GLOBAL_PROGRESS, STR_LOADING_INBOX_BACK);
		mmi_msg_entry_processing_generic();
		// SetInterruptEventHandler(mmi_msg_handle_ready_ind_inbox, NULL, PRT_MSG_ID_MMI_SMS_READY_IND);
	}
	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	else if(mmi_frm_sms_check_action_pending())
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else
#endif
		mmi_msg_entry_inbox_list();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_to_inbox_list
* DESCRIPTION
*   Go back to inbox list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/*
void mmi_msg_go_back_to_inbox_list (void)
{
	if(g_msg_cntx.get_inbox_list_needed)
	{
		g_msg_cntx.toDisplayMessageList = TO_DISPLAY_MESSAGE_LIST_INBOX;	
		mmi_msg_set_processing_screen(STR_MESSAGE_MAIN_MENU_CAPTION, STR_LOADING_SMS, IMG_GLOBAL_PROGRESS, 0);
		mmi_msg_entry_processing_generic();
		if (g_msg_cntx.msg_init)
			mmi_msg_get_msg_list_inbox();
	}
	else
		mmi_msg_entry_inbox_list();
}
*/


/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_inbox_list
* DESCRIPTION
*   Entry inbox list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#ifdef __MMI_VIP_FUNCTION__
extern U16 mmi_frm_sms_show_list_index[];
void mmi_VIP_msg_get_suited_number(U16 nMsg, U8 msg_type)
{
	S32 list_index;
	S32 item_index;
	U16 nSuited = 0;
	U16 j = 0;
	U8 number[(MAX_DIGITS_SMS + 1) * ENCODING_LENGTH];

	U8 i;
	
	for(item_index = 0; item_index < nMsg; item_index++)
	{
	#ifdef __MMI_MULTI_SIM__
		if(MTPNP_AD_SMS_Get_Filter_Rule() != MTPNP_AD_FILTER_ALL)
			list_index = mmi_frm_sms_show_list_index[item_index];
		else
	#endif
			list_index = item_index;
		
		memset(number, 0, sizeof(number));
		
		AnsiiToUnicodeString((S8*)number, (S8*)mmi_frm_sms_get_address(msg_type, (U16)list_index));
		
		if(mmi_VIP_is_vip_number(number))
		{
			nSuited++;
		}
		else
		{
			g_vip_context.SMSIndexMap[j++] = item_index;
		}
		
	}
	g_vip_context.nSMSSuited = nSuited;
}
#endif
void mmi_msg_entry_inbox_list (void)
{
	U8*	guiBuffer=NULL;
	S32	hiliteitem=0;	
	#ifdef __MMI_MULTI_SIM__
    	U16 title;
    	U16 left_softkey;
    	U16 left_softkey_icon;
    	S32 numitem = MTPNP_PFAL_SMS_Get_SMS_List_Size(MMI_FRM_SMS_APP_INBOX);
	#else
    	S32 numitem = mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
	#endif

	#ifdef __MMI_VIP_FUNCTION__
	mmi_VIP_msg_get_suited_number(numitem, MMI_FRM_SMS_APP_INBOX);
	numitem -= g_vip_context.nSMSSuited;
	#endif

	if(IsScreenPresent(SCR_ID_MSG_PROCESSING))
	{
		HistoryReplace(SCR_ID_MSG_PROCESSING, SCR_ID_MSG_INBOX_LIST, mmi_msg_entry_inbox_list);
		g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=SCR_ID_MSG_INBOX_LIST;	
		return;
	}
	if(numitem==0)
	{
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
		return;
	}
	/* Most cases of entering inbox list while SMS not ready are checked in mmi_msg_pre_entry_inbox_list.
	    However, If new MT SMS comes during SMS not ready, after deleting/saving/copying/moving SMS, 
		inbox list cannot be entered and user should go back to idle screen. */
	else if(numitem==MMI_FRM_SMS_INVALID_INDEX)
	{
		DeleteScreenIfPresent(SCR_ID_MSG_INBOX_LIST);
		GoBackHistory();
		return;
	}
	EntryNewScreen(SCR_ID_MSG_INBOX_LIST, mmi_msg_exit_generic, mmi_msg_entry_inbox_list, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_inbox_list");
	g_msg_cntx.msg_ind_in_idle=FALSE;
	g_msg_cntx.msg_ind_after_call=FALSE;
	SetMessagesCurrScrnID(SCR_ID_MSG_INBOX_LIST);		
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_INBOX_LIST);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_INBOX_LIST);		 
	RegisterHighlightHandler (GetMsgIndex);
	if (guiBuffer!=NULL)
	{
		#ifdef __MMI_MULTI_SIM__
        	S32 currBoxIndex = MTPNP_PFAL_SMS_Get_Current_Index();
        	hiliteitem=(currBoxIndex < numitem) ? currBoxIndex : 0;
		#else
        	hiliteitem = (g_msg_cntx.currBoxIndex < numitem) ? g_msg_cntx.currBoxIndex : 0;
		#endif
                /* change gui buffer content */
		change_cat184_list_menu_history_highlighted_item(hiliteitem, guiBuffer);
	}
	
	#ifndef __MMI_MULTI_SIM__
	ShowCategory184Screen(
				STR_SCR6010_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
				STR_GLOBAL_OK, IMG_GLOBAL_OK,
				STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				numitem,
				mmi_msg_inbox_list_get_item,
				mmi_msg_inbox_list_get_hint,
				hiliteitem,guiBuffer);
	#else /* defined __MMI_MULTI_SIM__*/
	switch (MTPNP_AD_SMS_Get_Filter_Rule())
	{
		case MTPNP_AD_FILTER_MASTER:
		#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			title = STR_SCR6010_CAPTION;
		#else
			title = STRING_MTPNP_SMS_MASTER_INBOX;
		#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
			break;
		case MTPNP_AD_FILTER_SLAVE:
			title = STRING_MTPNP_SMS_SLAVE_INBOX;
			break;
		case MTPNP_AD_FILTER_SIM3:
			title = STRING_MTPNP_SMS_SIM3_INBOX;
			break;
		case MTPNP_AD_FILTER_SIM4:
			title = STRING_MTPNP_SMS_SIM4_INBOX;
			break;
		case MTPNP_AD_FILTER_ALL:
			title = STR_SCR6010_CAPTION;
			break;
		default:
			title = STR_SCR6010_CAPTION;
			break;
		}

		if (numitem == 0)
		{
			left_softkey = MTPNP_NULL;
			left_softkey_icon = MTPNP_NULL;
		}
		else
		{
			left_softkey = STR_GLOBAL_OK;
			left_softkey_icon = IMG_GLOBAL_OK;
		}

	ShowCategory184Screen(
				title, IMG_SMS_ENTRY_SCRN_CAPTION,
				left_softkey, left_softkey_icon,
				STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				numitem,
				(GetItemFuncPtr)MTPNP_PFAL_Msg_Inbox_List_Get_Item,
				(GetHintFuncPtr)MTPNP_PFAL_Msg_Inbox_List_Get_Hint,
				hiliteitem, guiBuffer);
#endif
	
	SetLeftSoftkeyFunction (mmi_msg_get_msg_inbox, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_get_msg_inbox, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

	#if (defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
	#else
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	#endif
}

#if (defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_inbox_list_get_item
* DESCRIPTION
*   Get inbox list item
*
* PARAMETERS
*  a  IN		item index
*  b  IN/OUT	string buffer
*  c  IN/OUT	image buffer
*  d  IN/OUT	string image mask
* RETURNS
*  TRUE
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_msg_inbox_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	S8 temp[(MAX_DIGITS_SMS+1)*ENCODING_LENGTH];
	S8* name;

	if(((mmi_frm_sms_get_status(MMI_FRM_SMS_APP_INBOX, (U16)item_index)&0xf0)>>4)==SMSAL_MTI_STATUS_REPORT)
		pfnUnicodeStrcpy((S8*)str_buff, (S8*)GetString (STR_INBOX_REPORT_ID));
	else
	{
		memset(temp, 0, (MAX_DIGITS_SMS+1)*ENCODING_LENGTH);
		AnsiiToUnicodeString(temp, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_INBOX, (U16)item_index));
		
		name = lookUpNumber (temp);
		if(pfnUnicodeStrlen(name))
			pfnUnicodeStrcpy((S8*)str_buff, name);
		else
			pfnUnicodeStrcpy((S8*)str_buff, temp);
	}
	
	if((mmi_frm_sms_get_status(MMI_FRM_SMS_APP_INBOX, (U16)item_index)&0x0f)== MMI_FRM_SMS_APP_UNREAD)
	{	
		if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_INBOX, (U16)item_index)&0x0f) == 0)
		{
			*img_buff_p = get_image(IMG_SMS_INBOX_UNREAD_SM);
		}
		else
		{
			*img_buff_p = get_image(IMG_SMS_INBOX_UNREAD_ME);
		}
	}
	else
	{
		if(mmi_frm_sms_check_complete(MMI_FRM_SMS_APP_INBOX, (U16)item_index)==TRUE)
		{	
			if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_INBOX, (U16)item_index)&0x0f) == 0)
			{
				*img_buff_p = get_image(IMG_SMS_INBOX_READ_SM);
			}
			else
			{
				*img_buff_p = get_image(IMG_SMS_INBOX_READ_ME);
			}
		}	
		else
		{
			if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_INBOX, (U16)item_index)&0x0f) == 0)
			{
				*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_SM);
			}
			else
			{
				*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_ME);
			}
		}
	}	

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_inbox_list_get_hint
* DESCRIPTION
*   Get inbox list hint
*
* PARAMETERS
*  a  IN		item index
*  b  IN/OUT	hint array
* RETURNS
*  TRUE: display hint, FALSE: not display hint
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_msg_inbox_list_get_hint ( S32 item_index, UI_string_type *hint_array )
{
	mmi_msg_get_msg_date_time((S8*)hint_array[0], NULL, mmi_frm_sms_get_timestamp(MMI_FRM_SMS_APP_INBOX, (U16)item_index));
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_outbox_list
* DESCRIPTION
*   Before entry outbox list, check if progressing screen is needed
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/*
U8 mmi_msg_handle_ready_ind_outbox(void* dummy)
{
	mmi_msg_entry_outbox_list();
	DeleteScreenIfPresent(SCR_ID_MSG_PROCESSING);
	return FALSE;
}
*/
void mmi_msg_pre_entry_outbox_list (void)
{
	#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SMS_Set_Filter_Rule(MTPNP_AD_FILTER_ALL);
	#endif
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX)==0)
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	else if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX)==MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_msg_set_processing_screen(STR_MESSAGE_MAIN_MENU_CAPTION, STR_LOADING_SMS, IMG_GLOBAL_PROGRESS, STR_LOADING_OUTBOX_BACK);
		mmi_msg_entry_processing_generic();
		// SetInterruptEventHandler(mmi_msg_handle_ready_ind_outbox, NULL, PRT_MSG_ID_MMI_SMS_READY_IND);
	}	
	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	else if(mmi_frm_sms_check_action_pending())
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else
		mmi_msg_entry_outbox_list();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_outbox_list
* DESCRIPTION
*   Entry outbox list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_outbox_list (void)
{
	U8*	guiBuffer=NULL;
	S32	hiliteitem=0;
	#ifdef __MMI_MULTI_SIM__
	U16 title;
	U16 left_softkey;
	U16 left_softkey_icon;
	S32 numitem = MTPNP_PFAL_SMS_Get_SMS_List_Size(MMI_FRM_SMS_APP_OUTBOX);
	#else
	S32 numitem = mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
	#endif

	#ifdef __MMI_VIP_FUNCTION__
	mmi_VIP_msg_get_suited_number(numitem, MMI_FRM_SMS_APP_OUTBOX);
	numitem -= g_vip_context.nSMSSuited;
	#endif

	if(IsScreenPresent(SCR_ID_MSG_PROCESSING))
	{
		HistoryReplace(SCR_ID_MSG_PROCESSING, SCR_ID_MSG_OUTBOX_LIST, mmi_msg_entry_outbox_list);
		g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=SCR_ID_MSG_OUTBOX_LIST;
		return;
	}
	if(numitem==0)
	{
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
		return;
	}
	EntryNewScreen(SCR_ID_MSG_OUTBOX_LIST, mmi_msg_exit_generic, mmi_msg_entry_outbox_list, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_outbox_list");
	SetMessagesCurrScrnID (SCR_ID_MSG_OUTBOX_LIST);	
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_OUTBOX_LIST);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_OUTBOX_LIST);		 
	RegisterHighlightHandler (GetMsgIndex);
	if (guiBuffer!=NULL)
	{
		#ifdef __MMI_MULTI_SIM__
		S32 currBoxIndex = MTPNP_PFAL_SMS_Get_Current_Index();
		hiliteitem=(currBoxIndex < numitem) ? currBoxIndex : 0;
		#else /* __MMI_DUAL_SIM_MASTER__ */
		hiliteitem = (g_msg_cntx.currBoxIndex < numitem) ? g_msg_cntx.currBoxIndex : 0;
		#endif
                /* change gui buffer content */
		change_cat184_list_menu_history_highlighted_item(hiliteitem, guiBuffer);
	}
	#ifndef __MMI_MULTI_SIM__
	ShowCategory184Screen(
				STR_SCR6026_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
				STR_GLOBAL_OK, IMG_GLOBAL_OK,
				STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				numitem,
				mmi_msg_outbox_list_get_item,
				NULL,
				hiliteitem,guiBuffer);
	#else /* __MMI_MULTI_SIM__ */
	switch (MTPNP_AD_SMS_Get_Filter_Rule())
	{
		case MTPNP_AD_FILTER_MASTER:
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			title = STR_SCR6026_CAPTION;
#else
			title = STRING_MTPNP_SMS_MASTER_OUTBOX;
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
			break;

		case MTPNP_AD_FILTER_SLAVE:
			title = STRING_MTPNP_SMS_SLAVE_OUTBOX;
			break;
		case MTPNP_AD_FILTER_SIM3:
			title = STRING_MTPNP_SMS_SIM3_OUTBOX;
			break;
		case MTPNP_AD_FILTER_SIM4:
			title = STRING_MTPNP_SMS_SIM4_OUTBOX;
			break;
		case MTPNP_AD_FILTER_ALL:
			title = STR_SCR6026_CAPTION;
			break;

		default:
			title = STR_SCR6026_CAPTION;
			break;
	}

	if (numitem == 0) 
	{
		left_softkey = MTPNP_NULL;
		left_softkey_icon = MTPNP_NULL;
	} 
	else 
	{
		left_softkey = STR_GLOBAL_OK;
		left_softkey_icon = IMG_GLOBAL_OK;
	}

	ShowCategory184Screen(
				title, IMG_SMS_ENTRY_SCRN_CAPTION,
				left_softkey, left_softkey_icon,
				STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
				numitem,
				(GetItemFuncPtr)MTPNP_PFAL_Msg_Outbox_List_Get_Item,
				NULL,
				hiliteitem, guiBuffer);
#endif 
	
	SetLeftSoftkeyFunction (mmi_msg_get_msg_outbox, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_get_msg_outbox, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_outbox_list_get_item
* DESCRIPTION
*   Get outbox list item
*
* PARAMETERS
*  a  IN		item index
*  b  IN/OUT	string buffer
*  b  IN/OUT	image buffer
*  b  IN/OUT	string image mask
* RETURNS
*  TRUE
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_msg_outbox_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	S8 temp[(MAX_DIGITS_SMS+1)*ENCODING_LENGTH];
	S8* name;
	
	if(strlen((S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_OUTBOX, (U16)item_index)))
	{
		memset(temp, 0, (MAX_DIGITS_SMS+1)*ENCODING_LENGTH);
		AnsiiToUnicodeString(temp, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_OUTBOX, (U16)item_index));

		name = lookUpNumber (temp);
		if(pfnUnicodeStrlen(name))
			pfnUnicodeStrcpy((S8*)str_buff, name);
		else
			pfnUnicodeStrcpy((S8*)str_buff, temp);
	}
	else
		pfnUnicodeStrcpy((S8*)str_buff, (S8*)GetString(STR_OUTBOX_LIST_MESSAGE));
	
	if(mmi_frm_sms_check_complete(MMI_FRM_SMS_APP_OUTBOX, (U16)item_index)==FALSE)
	{
		if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_OUTBOX, (U16)item_index)&0x0f) == 0)
		{
			*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_SM);
		}
		else
		{
			*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_ME);
		}
	}
	else
	{
		if((mmi_frm_sms_get_status(MMI_FRM_SMS_APP_OUTBOX, (U16)item_index)&0x0f)== MMI_FRM_SMS_APP_OUTBOX)
		{
			if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_OUTBOX, (U16)item_index)&0x0f) == 0)
			{
				*img_buff_p = get_image(IMG_SMS_OUTBOX_SEND_SM);
			}
		else
			{
				*img_buff_p = get_image(IMG_SMS_OUTBOX_SEND_ME);
			}
		}
		else
		{
			if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_OUTBOX, (U16)item_index)&0x0f) == 0)
			{
				*img_buff_p = get_image(IMG_SMS_OUTBOX_UNSEND_SM);
			}
			else
			{
				*img_buff_p = get_image(IMG_SMS_OUTBOX_UNSEND_ME);
			}
		}
	}	

	return TRUE;
}

#ifdef __MMI_MESSAGES_DRAFT_BOX__
/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_draftbox_list
* DESCRIPTION
*   Before entry draftbox list, check if progressing screen is needed
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/*
U8 mmi_msg_handle_ready_ind_draftbox(void* dummy)
{
	mmi_msg_entry_draftbox_list();
	DeleteScreenIfPresent(SCR_ID_MSG_PROCESSING);
	return FALSE;
}
*/
void mmi_msg_pre_entry_draftbox_list (void)
{
#ifdef MMI_ON_HARDWARE_P
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS)==0)
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	else if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS)==MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_msg_set_processing_screen(STR_MESSAGE_MAIN_MENU_CAPTION, STR_LOADING_SMS, IMG_GLOBAL_PROGRESS, STR_LOADING_DRAFTBOX_BACK);
		mmi_msg_entry_processing_generic();
		// SetInterruptEventHandler(mmi_msg_handle_ready_ind_draftbox, NULL, PRT_MSG_ID_MMI_SMS_READY_IND);
	}	
	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	else if(mmi_frm_sms_check_action_pending())
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else
#endif
		mmi_msg_entry_draftbox_list();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_draftbox_list
* DESCRIPTION
*   Entry draftbox list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_draftbox_list (void)
{
	U8*	guiBuffer=NULL;
	S32	hiliteitem=0;
	#ifdef __MMI_MULTI_SIM__
	U16 title;
	U16 left_softkey;
	U16 left_softkey_icon;
	S32 numitem = MTPNP_PFAL_SMS_Get_SMS_List_Size(MMI_FRM_SMS_APP_DRAFTS);
	#else
	S32	numitem=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);
	#endif

	if(IsScreenPresent(SCR_ID_MSG_PROCESSING))
	{
		HistoryReplace(SCR_ID_MSG_PROCESSING, SCR_ID_MSG_DRAFTBOX_LIST, mmi_msg_entry_draftbox_list);
		g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=SCR_ID_MSG_DRAFTBOX_LIST;
		return;
	}
	if(numitem==0)
	{
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
		return;
	}
	EntryNewScreen(SCR_ID_MSG_DRAFTBOX_LIST, mmi_msg_exit_generic, mmi_msg_entry_draftbox_list, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_draftbox_list ");
	SetMessagesCurrScrnID (SCR_ID_MSG_DRAFTBOX_LIST);	
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_DRAFTBOX_LIST);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_DRAFTBOX_LIST);		 
	RegisterHighlightHandler (GetMsgIndex);
	if (guiBuffer!=NULL)
	{
		hiliteitem=(g_msg_cntx.currBoxIndex<numitem)?g_msg_cntx.currBoxIndex:0;
                /* change gui buffer content */
		change_cat184_list_menu_history_highlighted_item(hiliteitem, guiBuffer);
	}
#ifdef __MMI_MULTI_SIM__    
	switch (MTPNP_AD_SMS_Get_Filter_Rule())
	{
		case MTPNP_AD_FILTER_MASTER:
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
			title = STR_DRAFTBOX_MENUENTRY;
#else
			title = STRING_MTPNP_SMS_MASTER_DRAFTBOX;
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
			break;

		case MTPNP_AD_FILTER_SLAVE:
			title = STRING_MTPNP_SMS_SLAVE_DRAFTBOX;
			break;
		case MTPNP_AD_FILTER_SIM3:
			title = STRING_MTPNP_SMS_SIM3_DRAFTBOX;
			break;
		case MTPNP_AD_FILTER_SIM4:
			title = STRING_MTPNP_SMS_SIM4_DRAFTBOX;
			break;
		case MTPNP_AD_FILTER_ALL:
			title = STR_DRAFTBOX_MENUENTRY;
			break;

		default:
			title = STR_DRAFTBOX_MENUENTRY;
			break;
	}
#else
    title = STR_DRAFTBOX_MENUENTRY;
#endif    

	ShowCategory184Screen(title, IMG_SMS_ENTRY_SCRN_CAPTION, 
							STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							numitem,
							mmi_msg_draftbox_list_get_item, 
							NULL , 
							hiliteitem, 
							guiBuffer);
	
	SetLeftSoftkeyFunction (mmi_msg_get_msg_draftbox, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_get_msg_draftbox, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_draftbox_list_get_item
* DESCRIPTION
*   Get draftbox list item
*
* PARAMETERS
*  a  IN		item index
*  b  IN/OUT	string buffer
*  b  IN/OUT	image buffer
*  b  IN/OUT	string image mask
* RETURNS
*  TRUE
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_msg_draftbox_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	S8 temp[(MAX_DIGITS_SMS+1)*ENCODING_LENGTH];
	S8* name;

	if(strlen((S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_DRAFTS, item_index)))
	{
		memset(temp, 0, (MAX_DIGITS_SMS+1)*ENCODING_LENGTH);
		AnsiiToUnicodeString(temp, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_DRAFTS, item_index));

		name = lookUpNumber (temp);
		if(pfnUnicodeStrlen(name))
			pfnUnicodeStrcpy((S8*)str_buff, name);
		else
			pfnUnicodeStrcpy((S8*)str_buff, temp);
	}
	else
		pfnUnicodeStrcpy((S8*)str_buff, (S8*)GetString(STR_OUTBOX_LIST_MESSAGE));
	
	if(mmi_frm_sms_check_complete(MMI_FRM_SMS_APP_DRAFTS, item_index)==FALSE)
			*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP);
		else
			*img_buff_p = get_image(IMG_MESSAGE_UNSENT);

	return TRUE;
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_inbox_header
* DESCRIPTION
*   Get inbox message header
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* mmi_msg_get_inbox_header (void)
{
	static S8 buffer [120*ENCODING_LENGTH];
	S8 buf2 [10];
	S8 temp[MAX_DIGITS*ENCODING_LENGTH];
	S8 number[MAX_DIGITS_SMS*ENCODING_LENGTH];
	S8* name;
	S8	ts_date [(MAX_TIMESTAMP_SIZE+1)*ENCODING_LENGTH];
	S8	ts_time [(MAX_TIMESTAMP_SIZE+1)*ENCODING_LENGTH];

 	mmi_trace(g_sw_SMS,"MMI_SMS: mmi_msg_get_inbox_header");
	AnsiiToUnicodeString (buf2, "\n");
	if(PendingSaveSendData.mti == SMSAL_MTI_STATUS_REPORT)
	{
		if(PendingSaveSendData.st==ST_COMP_MSG_RECV_BY_SME)
			pfnUnicodeStrcpy (buffer, GetString (STR_SR_DELIVERD_ID));
		else
			pfnUnicodeStrcpy (buffer, GetString (STR_SR_PENDING_ID));
	}
	else
		pfnUnicodeStrcpy (buffer, GetString (STR_SMS_MESSAGE_HEADER_FROM));
	pfnUnicodeStrcat (buffer, buf2);
	/* if SMS is not ready */
	if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX)==MMI_FRM_SMS_INVALID_INDEX)
		AnsiiToUnicodeString(number, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_NOBOX, g_msg_cntx.msg_ind_index));
	else
	        AnsiiToUnicodeString(number, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_INBOX, (U16)g_msg_cntx.currBoxIndex));
	pfnUnicodeStrcpy(temp, number);
	name = lookUpNumber (temp);
	if(pfnUnicodeStrlen(name))
		pfnUnicodeStrcat(buffer, name);
	else
		pfnUnicodeStrcat(buffer, temp);

	/* if SMS is not ready */
	if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX)==MMI_FRM_SMS_INVALID_INDEX)
		mmi_msg_get_msg_date_time(ts_date, ts_time, mmi_frm_sms_get_timestamp(MMI_FRM_SMS_APP_NOBOX, g_msg_cntx.msg_ind_index));
	else
	        mmi_msg_get_msg_date_time(ts_date, ts_time, mmi_frm_sms_get_timestamp(MMI_FRM_SMS_APP_INBOX, (U16)g_msg_cntx.currBoxIndex));
	pfnUnicodeStrcat (buffer, buf2);
	pfnUnicodeStrcat (buffer, ts_date) ;
	pfnUnicodeStrcat (buffer, buf2);
	pfnUnicodeStrcat (buffer, ts_time) ;
	pfnUnicodeStrcat (buffer, buf2);
	
	return (U8*)buffer;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_inbox_msg
* DESCRIPTION
*   Entry inbox message screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_inbox_msg (void)
{
#ifdef __MMI_TTS_FEATURES__ 
	ejTTSPlayerStop();
#endif

	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_INBOX)!=MMI_FRM_SMS_INVALID_INDEX)
		GoBackHistory();
	else
		ExecCurrEndKeyDownHandler();
}


void mmi_msg_entry_inbox_msg (void)
{
	U8*	guiBuffer=NULL;
	U8* 	pHeader;	
	EMSData*	pEms ;
	U16 strtitle;

	/* in case there is an interrupt cut in before displaying message content */
	if(IsScreenPresent(SCR_ID_MSG_PROCESSING))
	{
		HistoryReplace(SCR_ID_MSG_PROCESSING, SCR_ID_MSG_INBOX_MSG, mmi_msg_entry_inbox_msg);
		return;
	}
	EntryNewScreen(SCR_ID_MSG_INBOX_MSG, mmi_msg_exit_generic, mmi_msg_entry_inbox_msg, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_inbox_msg");
	
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_INBOX_MSG);
	SetMessagesCurrScrnID(SCR_ID_MSG_INBOX_MSG);	
	GetCurrEndKeyDownHandler();
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_MSG_INBOX_MSG);
	GetEMSDataForView(&pEms, 0);
	pHeader = mmi_msg_get_inbox_header();
	g_msg_cntx.number_from_phb=0;
#if defined(__MMI_MESSAGES_EMS__)	
	EnableDisableAudioPlayback();
#endif
#ifdef __MMI_MESSAGES_CHAT__
	if(pEms->listHead == NULL)
	{
		if((CheckForInvitationMsg(pEms->textBuffer,(S32)pEms->textLength,TRUE))||((guiBuffer!=NULL)&&(gChatInvitation)))
		{
			SetChatInboxIndex(PendingSaveSendData.msgboxindex);
			strtitle=STR_CHAT_INVITATION;		
			if(guiBuffer==NULL)
				RemoveInvitationChar();
		}
		else 
		{
			strtitle=STR_SCR6024_CAPTION;
			gChatInvitation=0;
		}
	}
	else
	{
		strtitle=STR_SCR6024_CAPTION;
		gChatInvitation=0;
	}
#else
		strtitle=STR_SCR6024_CAPTION;
#endif
	ShowCategory39Screen ( (U16)strtitle, IMG_SMS_ENTRY_SCRN_CAPTION,
						STR_GLOBAL_OPTIONS, IMG_SMS_COMMON_NOIMAGE,
						STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE,	
						pEms, pHeader, guiBuffer); 
#ifdef __MMI_MESSAGES_CHAT__
	if(strtitle==STR_CHAT_INVITATION)
		SetLeftSoftkeyFunction (mmi_msg_entry_option_chat, KEY_EVENT_UP);
	else 
		SetLeftSoftkeyFunction (mmi_msg_entry_option_inbox, KEY_EVENT_UP);
#else
	SetLeftSoftkeyFunction (mmi_msg_entry_option_inbox, KEY_EVENT_UP);
#endif
	
	SetLeftSoftkeyFunction(mmi_msg_entry_option_inbox, KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_msg_go_back_from_inbox_msg, KEY_EVENT_UP);

#ifdef __MMI_TTS_FEATURES__ 

	if(g_tts_setting.curTTSMsgReadStatus == 0)
	{
		ejTTS_PlayToPCM(pEms->textBuffer, pEms->textLength);
	}else
	{
		pTtsText = pEms->textBuffer;
		nTtsTextSize = pEms->textLength;
		SetKeyHandler(ejTTS_play_text_hdlr, KEY_1, KEY_EVENT_DOWN);
		SetKeyHandler(ejTTS_play_stop_hdlr, KEY_2, KEY_EVENT_DOWN);
	}
#endif	

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_outbox_header
* DESCRIPTION
*   Get outbox message header
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* mmi_msg_get_outbox_header (void)
{
	static S8 buffer [100*ENCODING_LENGTH];
	S8 buf2 [10];
	S8 temp[MAX_DIGITS*ENCODING_LENGTH];
	S8 number[MAX_DIGITS_SMS*ENCODING_LENGTH];
	S8* name;
	U8 msgtype=MMI_FRM_SMS_APP_OUTBOX;
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_get_outbox_header");
#ifdef __MMI_MESSAGES_DRAFT_BOX__
	if(g_msg_cntx.toDisplayMessageList==TO_DISPLAY_MESSAGE_LIST_OUTBOX)
		msgtype=MMI_FRM_SMS_APP_OUTBOX;
	else 
		msgtype=MMI_FRM_SMS_APP_DRAFTS;
#endif

	AnsiiToUnicodeString (buf2, "\n");	
	if(PendingSaveSendData.mti == SMSAL_MTI_UNSPECIFIED)
		pfnUnicodeStrcpy (buffer, GetString (STR_SMS_DOES_NOT_SUPPORT));

	else
	{
		pfnUnicodeStrcpy (buffer, GetString (STR_SMS_MESSAGE_HEADER_TO));
		AnsiiToUnicodeString (buf2, "\n");	
		pfnUnicodeStrcat (buffer, buf2);


		if(pfnUnicodeStrlen((S8*)mmi_frm_sms_get_address(msgtype, (U16)g_msg_cntx.currBoxIndex)))
		{
			AnsiiToUnicodeString(number, (S8*)mmi_frm_sms_get_address(msgtype, (U16)g_msg_cntx.currBoxIndex));
			pfnUnicodeStrcpy(temp, number);

			name = lookUpNumber (temp);
			if(pfnUnicodeStrlen(name))
				pfnUnicodeStrcat(buffer, name);
			else
				pfnUnicodeStrcat(buffer, temp);
		}
		else
		{
			pfnUnicodeStrcpy(temp, GetString (STR_OUTBOX_LIST_MESSAGE));
			pfnUnicodeStrcat(buffer, temp);
		}
	}

	pfnUnicodeStrcat (buffer, buf2);
	
	return (U8*)buffer;
}

#ifdef __MMI_TTS_FEATURES__ 

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_outbox_msg
* DESCRIPTION
*   Entry inbox message screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_outbox_msg (void)
{
	//add by liuwen
	//stop the tts
	ejTTSPlayerStop();
	
	//if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_INBOX)!=MMI_FRM_SMS_INVALID_INDEX)
	GoBackHistory();
	//else
		//ExecCurrEndKeyDownHandler();
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_outbox_msg
* DESCRIPTION
*   Entry outbox message screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_outbox_msg (void)
{
	U8*	guiBuffer=NULL;
	U8* 	pHeader;	
	EMSData*	pEms ;

	/* in case there is an interrupt cut in before displaying message content */
	if(IsScreenPresent(SCR_ID_MSG_PROCESSING))
	{
		HistoryReplace(SCR_ID_MSG_PROCESSING, SCR_ID_MSG_OUTBOX_MSG, mmi_msg_entry_outbox_msg);
		return;
	}
	EntryNewScreen(SCR_ID_MSG_OUTBOX_MSG, mmi_msg_exit_generic, mmi_msg_entry_outbox_msg, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_outbox_msg");
	
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_OUTBOX_MSG);
	SetMessagesCurrScrnID (SCR_ID_MSG_OUTBOX_MSG);	
	
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_OUTBOX_MSG);
	GetEMSDataForView (&pEms, 0);
	pHeader = mmi_msg_get_outbox_header ();
	g_msg_cntx.number_from_phb=0;
	
#if defined(__MMI_MESSAGES_EMS__)	
	EnableDisableAudioPlayback();
#endif
	ShowCategory39Screen ( STR_SCR6024_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
						STR_GLOBAL_OPTIONS, IMG_SMS_COMMON_NOIMAGE,
						STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE,	
						pEms, pHeader, guiBuffer); 
	
	SetLeftSoftkeyFunction (mmi_msg_entry_option_outbox, KEY_EVENT_UP);

#ifdef __MMI_TTS_FEATURES__ 
		SetRightSoftkeyFunction (mmi_msg_go_back_from_outbox_msg, KEY_EVENT_UP);

	//如果自动读则直接播放，否则手动点击1 进行播放。
	if(g_tts_setting.curTTSMsgReadStatus == 0)
	{
		ejTTS_PlayToPCM(pEms->textBuffer, pEms->textLength);
	}else
	{
		pTtsText = pEms->textBuffer;
		nTtsTextSize = pEms->textLength;
		SetKeyHandler(ejTTS_play_text_hdlr, KEY_1, KEY_EVENT_DOWN);
		SetKeyHandler(ejTTS_play_stop_hdlr, KEY_2, KEY_EVENT_DOWN);
	}
#else 	

	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
#endif
	
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_default_msg
* DESCRIPTION
*   Entry default message screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_default_msg (void)
{
	U8*	guiBuffer=NULL;
	U8* 	pHeader;	
	U8	lHeader;

	/* in case there is an interrupt cut in before displaying message content */
	if(IsScreenPresent(SCR_ID_MSG_PROCESSING))
	{
		HistoryReplace(SCR_ID_MSG_PROCESSING, SCR_ID_MSG_DEFAULT_MSG, mmi_msg_entry_default_msg);
		return;
	}
	EntryNewScreen(SCR_ID_MSG_DEFAULT_MSG, mmi_msg_exit_generic, mmi_msg_entry_default_msg, NULL);
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_default_msg");
	
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_DEFAULT_MSG);
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_DEFAULT_MSG);
	SetMessagesCurrScrnID (SCR_ID_MSG_DEFAULT_MSG);	
	
	if(PendingSaveSendData.mti==SMSAL_MTI_STATUS_REPORT)
		pHeader = mmi_msg_get_inbox_header();
	else
		pHeader = mmi_msg_get_outbox_header();
	lHeader=(pfnUnicodeStrlen((S8*)pHeader)+1)*ENCODING_LENGTH;
	ShowCategory74Screen ( STR_SCR6024_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
						   		STR_GLOBAL_OPTIONS, IMG_SMS_COMMON_NOIMAGE,
						   		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  			pHeader, lHeader, guiBuffer);

	SetLeftSoftkeyFunction (mmi_msg_entry_option_default, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_save_use_number
* DESCRIPTION
*   Save use number
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_save_use_number (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_save_use_number");
	if (pfnUnicodeStrlen(g_msg_cntx.numbersList[g_msg_cntx.currHiliteIndex]))
		SaveNumberFromIdleScrn(g_msg_cntx.numbersList[g_msg_cntx.currHiliteIndex],0);
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);		
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_dial_use_number
* DESCRIPTION
*   Dial use number
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_dial_use_number (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_dial_use_number");
	if (pfnUnicodeStrlen(g_msg_cntx.numbersList[g_msg_cntx.currHiliteIndex]))
		MakeCall(g_msg_cntx.numbersList[g_msg_cntx.currHiliteIndex]);
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);		
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_use_number
* DESCRIPTION
*   Entry use number list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_use_number (void)
{
	U16	numbersCount=0;
	U8*	guiBuffer=NULL;
	EMSData * pEMS;
	S8 number[MAX_DIGITS_SMS*ENCODING_LENGTH];
	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_use_number");

	pEMS = GetEMSDataForView (0, 0);
	#ifdef __MMI_MESSAGES_CHAT__
	if(IsScreenPresent(CHAT_ROOM_SCREEN_ID))
		numbersCount=ChatUseNumberScreen();
	else
	#endif
	{
		U8 type=MMI_FRM_SMS_APP_NOBOX;
		if(g_msg_cntx.toDisplayMessageList==TO_DISPLAY_MESSAGE_LIST_INBOX)
			type=MMI_FRM_SMS_APP_INBOX;
		else if(g_msg_cntx.toDisplayMessageList==TO_DISPLAY_MESSAGE_LIST_OUTBOX)
			type=MMI_FRM_SMS_APP_OUTBOX;
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		else
			type=MMI_FRM_SMS_APP_DRAFTS;
		#endif
		/* if SMS is not ready */
		if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX)==MMI_FRM_SMS_INVALID_INDEX)
			AnsiiToUnicodeString(number, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_NOBOX, g_msg_cntx.msg_ind_index));
		else
		        AnsiiToUnicodeString(number, (S8*)mmi_frm_sms_get_address(type, (U16)g_msg_cntx.currBoxIndex));	
		
		#ifdef __MMI_SMART_MESSAGE_MT__
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#endif
		numbersCount=ObtainNumbersFromMessageText((S8*)number, pEMS->textBuffer, pEMS->textLength, 0);
	}

	if (numbersCount > 0)
	{
		EntryNewScreen(SCR_ID_MSG_USE_NUMBER_LIST, mmi_msg_exit_generic, mmi_msg_entry_use_number, NULL);	
	
		guiBuffer = GetCurrGuiBuffer(SCR_ID_MSG_USE_NUMBER_LIST);
		RegisterHighlightHandler(GetHiliteIndex);
		SetMessagesCurrScrnID(SCR_ID_MSG_USE_NUMBER_LIST);
			
		ShowCategory53Screen (STR_USE_NUMBER_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION, 
								  STR_GLOBAL_OPTIONS, IMG_SMS_COMMON_NOIMAGE,
								  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								  numbersCount, (U8**)g_msg_cntx.numbersList, (U16 *)gIndexIconsImageList,
								  NULL , 0, 0, guiBuffer );
		SetLeftSoftkeyFunction(mmi_msg_entry_use_number_option, KEY_EVENT_UP);
		SetKeyHandler(mmi_msg_entry_use_number_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#ifdef __MMI_MULTI_SIM__
		MTPNP_PFAL_CC_HandleSendKeys(mmi_msg_dial_use_number, KEY_EVENT_DOWN);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
		 ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
		SetKeyHandler(mmi_msg_entry_use_number_option,KEY_ENTER,KEY_EVENT_UP );
#endif
#else
		SetKeyHandler(mmi_msg_dial_use_number, KEY_SEND, KEY_EVENT_DOWN);
#endif
		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	}
	else
		DisplayPopup ((PU8)GetString (STR_NONUMBER_NOTIFICATION), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_use_number_option
* DESCRIPTION
*   Entry use number list option
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_use_number_option (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_MSG_USE_NUMBER_OPTION, mmi_msg_exit_generic, mmi_msg_entry_use_number_option, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_use_number_option");
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_USE_NUMBER_OPTION);
	
	numItems = GetNumOfChild (SMS_USE_NUMBER_OPTIONS_MENUID);	
	GetSequenceStringIds(SMS_USE_NUMBER_OPTIONS_MENUID, nStrItemList);	
	SetParentHandler (SMS_USE_NUMBER_OPTIONS_MENUID);

	pfnUnicodeStrcpy(g_msg_cntx.smsPhoneNumber, (S8*)g_msg_cntx.numbersList[g_msg_cntx.currHiliteIndex]);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_USE_NUMBER_OPTION);
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_USE_NUMBER_OPTION);
		
	ShowCategory52Screen(STR_USE_NUMBER_OPT_CAPTION,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_save_use_number
* DESCRIPTION
*   Highlight save use number handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_save_use_number (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_save_use_number, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_dial_use_number
* DESCRIPTION
*   Highlight dial use number handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_dial_use_number (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
								  #ifndef __MMI_MULTI_SIM__
								        mmi_msg_dial_use_number,
								  #else
								        mmi_msg_dial_use_number_ext,
								  #endif
										GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_use_number
* DESCRIPTION
*   Highlight use number handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_use_number (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_use_number, GoBackHistory);
}


/*****************************************************************************
* FUNCTION
*  RefreshMessagesMenuList
* DESCRIPTION
*   Refresh message menu list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void RefreshMessagesMenuList (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: RefreshMessagesMenuList");
	#ifdef __MMI_MESSAGES_MULTI_APP__
	if (GetMessagesCurrScrnID()==SCR_ID_MSG_SMS_MAIN_MENU)
	{
		S32 menuItemId=-1;
		menuItemId=GetChildMenuIDIndexByParentMenuID(MESSAGES_MENU_SMS_MENUID, MESSAGES_MENU_INBOX_MENUID);
		if(menuItemId!=-1)
			Category52ChangeItemDescriptionNoAnimation(menuItemId,TotalInboxOutboxStatus.totalInboxStr);
		menuItemId=GetChildMenuIDIndexByParentMenuID(MESSAGES_MENU_SMS_MENUID, MESSAGES_MENU_OUTBOX_MENUID);
		if(menuItemId!=-1)
			Category52ChangeItemDescriptionNoAnimation(menuItemId,TotalInboxOutboxStatus.totalOutboxStr);
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		menuItemId=GetChildMenuIDIndexByParentMenuID(MESSAGES_MENU_SMS_MENUID, MESSAGES_MENU_DRAFTBOX_MENUID);
		if(menuItemId!=-1)
			Category52ChangeItemDescriptionNoAnimation(menuItemId,TotalInboxOutboxStatus.totalDraftboxStr);		
		#endif
		RedrawCategoryFunction ();
	}
	#else	
	if (GetMessagesCurrScrnID() == SCR_ID_MSG_MAIN_MENU)
	{
		S32 menuItemId=-1;
		menuItemId=GetChildMenuIDIndexByParentMenuID(MAIN_MENU_MESSAGES_MENUID, MESSAGES_MENU_INBOX_MENUID);
		if(menuItemId!=-1)
			Category52ChangeItemDescriptionNoAnimation(menuItemId,TotalInboxOutboxStatus.totalInboxStr);
		menuItemId=GetChildMenuIDIndexByParentMenuID(MAIN_MENU_MESSAGES_MENUID, MESSAGES_MENU_OUTBOX_MENUID);
		if(menuItemId!=-1)
			Category52ChangeItemDescriptionNoAnimation(menuItemId,TotalInboxOutboxStatus.totalOutboxStr);
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		menuItemId=GetChildMenuIDIndexByParentMenuID(MAIN_MENU_MESSAGES_MENUID, MESSAGES_MENU_DRAFTBOX_MENUID);
		if(menuItemId!=-1)
			Category52ChangeItemDescriptionNoAnimation(menuItemId,TotalInboxOutboxStatus.totalDraftboxStr);		
		#endif
		RedrawCategoryFunction ();
	}
	#endif
}


#if defined(__MMI_MESSAGES_TEMPLATE__)
/*****************************************************************************
* FUNCTION
*  mmi_msg_insert_template
* DESCRIPTION
*   Insert template
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_insert_template (void)
{
	S8 *	templ;
	EMSData * pEMS;
	history_t	temphistory;
	historyNode *History;

	#if defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#else
	U8 result=0;
	#endif
	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_insert_template ");

	templ = g_msg_cntx.templates[g_msg_cntx.currHiliteIndex];

#if defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else
	if (templ)
	{
		U16 templateCharNum;		
		BOOL isUcs2Char=0;

		templateCharNum = pfnUnicodeStrlen((PS8)templ); 
		isUcs2Char = mmi_frm_sms_check_UCS2((U16*)templ, templateCharNum);
		
		memset (& temphistory, 0, sizeof (temphistory));
		GetHistory (SCR_ID_MSG_WRITE, &temphistory);
		pEMS = GetEMSDataForEdit (0, 0);

		if(pEMS->dcs == SMSAL_UCS2_DCS || isUcs2Char)
		{
			result = AppendCategory28String (INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
						pEMS, (U8*)templ, temphistory.guiBuffer );
		}
		else	//SMSAL_DEFAULT_DCS
		{
			U8*	handledBuffer;

			handledBuffer=OslMalloc((templateCharNum*2+1)*ENCODING_LENGTH);
			memset((S8*)handledBuffer, 0, ((templateCharNum*2+1)*ENCODING_LENGTH));
			mmi_frm_sms_handle_extension_char((U16*)handledBuffer, (U16*)templ,templateCharNum);
			result = AppendCategory28String (INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
							pEMS, (U8*)handledBuffer, temphistory.guiBuffer );

			OslMfree(handledBuffer);
		}
		if(GetHistoryScrID(SCR_ID_MSG_WRITE,&History)==ST_SUCCESS)
			memcpy(History->guiBuffer, temphistory.guiBuffer, MAX_GUI_BUFFER);		

		if (!result)
			mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_insert_template - ERROR in inserting template");
	}
	if (result==0)
	{
		GoBackToHistory (SCR_ID_MSG_WRITE);
		DisplayPopup ((PU8)GetString (STR_NO_SPACE_TO_INSERT_OBJECT),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);			
	}
	else
		Messages2GoBackHistory ();
#endif
	DeInitTemplates();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_save_template
* DESCRIPTION
*   Save template
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_save_template (void)
{
	if(StoreTemplate(g_msg_cntx.currHiliteIndex))
		DisplayPopup ((PU8)GetString (STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SAVE_TONE);
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	DeleteMessagesHistoryNodes ();
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_erase_template
* DESCRIPTION
*   Erase template
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_erase_template (void)
{
	if(DeleteTemplate (g_msg_cntx.currHiliteIndex))
		DisplayPopup ((PU8)GetString (STR_MESSAGES_ERACED_ID), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	DeleteMessagesHistoryNodes ();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_erase_template
* DESCRIPTION
*   Before erase template, check if it is empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_erase_template (void)
{
	if (!pfnUnicodeStrlen(g_msg_cntx.templates[g_msg_cntx.currHiliteIndex]))
	{
		DisplayPopup ((PU8)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
		DeleteMessagesHistoryNodes();
	}	
	else
	{
		mmi_msg_set_confirm_screen(STR_MESSAGES_ERACE_Q_ID, mmi_msg_erase_template, Messages2GoBackHistory);
		mmi_msg_entry_confirm_generic();
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_template
* DESCRIPTION
*   Deinit template before go back screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_template (void)
{
	DeInitTemplates();
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_end_key_from_template
* DESCRIPTION
*   Deinit template before go to idle
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_end_key_from_template (void)
{
	if(!GetTotalCallCount())
		DeInitTemplates();
	ExecCurrEndKeyDownHandler();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_insert_template_to_chat
* DESCRIPTION
*   Insert template to chat message
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#ifdef __MMI_MESSAGES_CHAT__
void mmi_msg_insert_template_to_chat (void)
{
	InsertTemplateToChat(g_msg_cntx.templates[g_msg_cntx.currHiliteIndex]);
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_template_list
* DESCRIPTION
*   Entry template list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_template_list (void)
{
	U8*	guiBuffer=NULL;
	S32	hiliteitem=0;

	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_template_list");

	if(InitTemplates()==FALSE)
	{
		DisplayPopup ((PU8)GetString (STR_SMS_DOES_NOT_SUPPORT), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);		
		return;
	}

	EntryNewScreen(SCR_ID_MSG_TEMPLATE_LIST, mmi_msg_exit_generic, mmi_msg_entry_template_list, NULL);	
	LoadTemplatesInDisplayList();

	guiBuffer = GetCurrGuiBuffer(SCR_ID_MSG_TEMPLATE_LIST);
	if(guiBuffer!=NULL)
		hiliteitem=(g_msg_cntx.currHiliteIndex<NUM_TEMPLATES)?g_msg_cntx.currHiliteIndex:0;
	RegisterHighlightHandler(GetHiliteIndex);
	SetMessagesCurrScrnID(SCR_ID_MSG_TEMPLATE_LIST);
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_TEMPLATE_LIST);
		
	GetCurrEndKeyDownHandler();
	ShowCategory184Screen(STR_SCRN_T0_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION, 
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
									NUM_TEMPLATES,
									mmi_msg_template_list_get_item, 
									NULL, 
									hiliteitem, 
									guiBuffer);

	SetRightSoftkeyFunction(mmi_msg_go_back_from_template, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_go_back_from_template, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	if(IsScreenPresent(SCR_ID_MSG_WRITE))
	{
		SetLeftSoftkeyFunction(mmi_msg_insert_template, KEY_EVENT_UP);
		SetKeyHandler(mmi_msg_insert_template, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
	#ifdef __MMI_MESSAGES_CHAT__
	else if(IsScreenPresent(CHAT_SCREEN_START_NEW_ID))
	{
		SetLeftSoftkeyFunction(mmi_msg_insert_template_to_chat, KEY_EVENT_UP);
		SetKeyHandler(mmi_msg_insert_template_to_chat, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
	#endif
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);
		SetLeftSoftkeyFunction(mmi_msg_entry_template_option, KEY_EVENT_UP);
		SetKeyHandler(mmi_msg_entry_template_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
	
	SetKeyHandler(mmi_msg_end_key_from_template, KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_template_list_get_item
* DESCRIPTION
*   Get template list item
*
* PARAMETERS
*  a  IN		item index
*  b  IN/OUT	string buffer
*  c  IN/OUT	image buffer
*  d  IN/OUT	string image mask
* RETURNS
*  TRUE
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_msg_template_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	if((item_index < 0) || (item_index >= NUM_TEMPLATES))
		return FALSE;

	if(pfnUnicodeStrlen(g_msg_cntx.templates[item_index])==0)
		pfnUnicodeStrcpy((S8*)str_buff, (S8*)GetString(STR_GLOBAL_EMPTY_LIST));
	else
		pfnUnicodeStrcpy((S8*)str_buff, (S8*)g_msg_cntx.templates[item_index]);
	
	*img_buff_p = get_image(gIndexIconsImageList[item_index]);
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_template_option
* DESCRIPTION
*   Entry template option
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_template_option (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_MSG_TEMPLATE_OPTION, mmi_msg_exit_generic, mmi_msg_entry_template_option, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_template_option");
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_TEMPLATE_OPTION);
	
	numItems = GetNumOfChild (TEMPLATES_OPTIONS_MENUENTRY);	
	GetSequenceStringIds(TEMPLATES_OPTIONS_MENUENTRY, nStrItemList);	
	SetParentHandler (TEMPLATES_OPTIONS_MENUENTRY);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_TEMPLATE_OPTION);
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_TEMPLATE_OPTION);
		
	GetCurrEndKeyDownHandler();
	ShowCategory52Screen(STR_SCRN_T1_OPTIONS_CAPTION,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
	SetKeyHandler(mmi_msg_end_key_from_template, KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_template_edit
* DESCRIPTION
*   Entry template edit screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_template_edit (void)
{
	U8* guiBuffer;
	EntryNewScreen(SCR_ID_MSG_TEMPLATE_EDIT, mmi_msg_exit_generic, mmi_msg_entry_template_edit, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_template_edit");
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_TEMPLATE_EDIT); 
	if (!guiBuffer) GetTemplatesToEdit (g_msg_cntx.scratchTemplRec, g_msg_cntx.currHiliteIndex);
	SetMessagesCurrScrnID(SCR_ID_MSG_TEMPLATE_EDIT);

	GetCurrEndKeyDownHandler();
	ShowCategory5Screen (STR_SCRN_T4_EDIT_TEMPLATE_CAPTION,
					IMG_SMS_ENTRY_SCRN_CAPTION,
					STR_GLOBAL_OPTIONS, IMG_SMS_COMMON_NOIMAGE,
					STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE,
					INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
					(U8*) g_msg_cntx.scratchTemplRec, /*MAX_TEMPLATE_LENGTH+1*/(NVRAM_SMS_TEMPL_RECORD_SIZE/ENCODING_LENGTH), 
					guiBuffer);
#if defined(__MMI_GB__)                                                                           //add by yaosq 20061109
	GBSetInputboxLSKFunction(mmi_msg_entry_template_edit_option); 
#endif
	SetLeftSoftkeyFunction(mmi_msg_entry_template_edit_option, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_end_key_from_template, KEY_END, KEY_EVENT_DOWN);

	RegisterInputMethodScreenCloseFunction(Messages2GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_template_edit_option
* DESCRIPTION
*   Entry template edit option screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_template_edit_option (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_MSG_TEMPLATE_EDIT_OPTION, mmi_msg_exit_generic, mmi_msg_entry_template_edit_option, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_template_edit_option");
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_TEMPLATE_EDIT_OPTION);
	
	numItems = GetNumOfChild (SMS_ED_TEMPL_OPTIONS_MENU_ID);	
	GetSequenceStringIds(SMS_ED_TEMPL_OPTIONS_MENU_ID, nStrItemList);	
	SetParentHandler (SMS_ED_TEMPL_OPTIONS_MENU_ID);
	
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_TEMPLATE_EDIT_OPTION);
		
	GetCurrEndKeyDownHandler();
	ShowCategory52Screen(STR_GLOBAL_OPTIONS,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
	SetKeyHandler(mmi_msg_end_key_from_template, KEY_END, KEY_EVENT_DOWN);
}
/*****************************************************************************
 * FUNCTION
 *  mmi_msg_entry_template_write_msg
 * DESCRIPTION
 *  entry write msg with the template content
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_entry_template_write_msg(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S8 *templ;
    EMSData *pEMS;
    historyNode temphistory;
    U8 result = 1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION_2((MMI_TRACE_G6_SMS, "*[SmsMoMtGuiInterface.c] mmi_msg_entry_template_write_msg *\n"));

    templ = g_msg_cntx.templates[g_msg_cntx.currHiliteIndex];
    pEMS = GetEMSDataForEdit(0, 1);
    
    if (templ)
    {
        U16 templateCharNum;
        BOOL isUcs2Char = 0;

        templateCharNum = UCS2Strlen((PS8) templ);
        isUcs2Char = mmi_frm_sms_check_UCS2((U16*) templ, templateCharNum);

        memset(&temphistory, 0, sizeof(temphistory));

        if (pEMS->dcs == SMSAL_UCS2_DCS || isUcs2Char)
        {
            result = AppendCategory28String(
                        INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
                        pEMS,
                        (U8*) templ,
                        temphistory.guiBuffer);
        }
        else    /* SMSAL_DEFAULT_DCS */
        {
            U8 *handledBuffer;

            handledBuffer = OslMalloc((templateCharNum * 2 + 1) * ENCODING_LENGTH);
            memset((S8*) handledBuffer, 0, ((templateCharNum * 2 + 1) * ENCODING_LENGTH));
            mmi_frm_sms_handle_extension_char((U16*) handledBuffer, (U16*) templ, templateCharNum);
            result = AppendCategory28String(
                        INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
                        pEMS,
                        (U8*) handledBuffer,
                        temphistory.guiBuffer);

            OslMfree(handledBuffer);
        }

        /* GoToEndPosition (pEMS); */

        if (!result)
        {
            PRINT_INFORMATION_2((MMI_TRACE_G6_SMS,
                                 "*[SmsMoMtGuiInterface.c] mmi_msg_insert_template - ERROR in inserting template-*\n"));
        }
    }
    if (result == 0)
    {
        DisplayPopup(
            (PU8) GetString(STR_GLOBAL_ERROR),
            IMG_GLOBAL_UNFINISHED,
            1,
            MESSAGES_POPUP_TIME_OUT,
            (U8) ERROR_TONE);
    }
    else
    {
    #ifdef __MMI_UNIFIED_COMPOSER__
        mmi_msg_entry_uc(NULL, MMI_UC_STATE_WRITE_NEW_MSG, 0, MMI_UC_MSG_TYPE_DEFAULT);
    #else /* __MMI_UNIFIED_COMPOSER__ */
		g_msg_cntx.sendMessageCase = SEND_CASE_SEND_ONLY;
        mmi_msg_entry_write_msg();
    #endif /* __MMI_UNIFIED_COMPOSER__ */   
    }

    DeleteScreenIfPresent(SCR_ID_MSG_TEMPLATE_OPTION);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_save_template
* DESCRIPTION
*   Highlight save template handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_save_template (void)
{
	mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, mmi_msg_save_template, Messages4GoBackHistory);

	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_erase_template
* DESCRIPTION
*   Highlight erase template handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_erase_template (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_erase_template, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_edit_template
* DESCRIPTION
*   Highlight edit template handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_edit_template (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_template_edit, GoBackHistory);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_msg_highlight_template_write_msg
 * DESCRIPTION
 *  Highlight template write msg handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_highlight_template_write_msg(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_msg_highlight_generic(
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        mmi_msg_entry_template_write_msg,
        GoBackHistory);

}
#endif // defined(__MMI_MESSAGES_TEMPLATE__)

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_done
* DESCRIPTION
*   Highlight done handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_done (void)
{
#ifdef __MMI_MULTI_SIM__
	U8 reqSrcMod = GetReqSrcModIndex();
	MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
	MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, reqSrcMod);
#endif

	mmi_msg_highlight_generic(STR_GLOBAL_OK, IMG_GLOBAL_OK, 
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
							mmi_msg_entry_send_option, GoBackHistory);

	if ((g_msg_cntx.sendMessageCase == SEND_CASE_REPLY) || (g_msg_cntx.sendMessageCase == SEND_CASE_SEND_FROM_PHB) || (g_msg_cntx.sendMessageCase == SEND_CASE_SEND_FROM_CLG))
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_input_method
* DESCRIPTION
*   Highlight input method handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_input_method (void)
{
	SetInputMethodAndDoneCaptionIcon(IMG_SMS_ENTRY_SCRN_CAPTION);
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										EntryInputMethodScreen, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_input_method_generic
* DESCRIPTION
*   Highlight input method generic handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_input_method_generic (void)
{
	SetInputMethodAndDoneCaptionIcon(IMG_SMS_ENTRY_SCRN_CAPTION);
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										EntryInputMethodScreen, GoBackHistory);
	RegisterInputMethodScreenCloseFunction (Messages2GoBackHistory);
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_send_only
* DESCRIPTION
*   Highlight send only handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_send_only (void)
{
#ifdef __MMI_SEND_BY_SEARCH_NAME__	
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 	IMG_GLOBAL_BACK, 
								mmi_msg_entry_send_only, GoBackHistory);
#else
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
								mmi_msg_send_msg_to_only_entry, GoBackHistory);
#endif
}

#ifdef __MMI_SEND_BY_SEARCH_NAME__	
/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_send_only_number
* DESCRIPTION
*   Highlight send only number handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_send_only_number (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_send_msg_to_only_entry, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_send_only_name
* DESCRIPTION
*   Highlight send only name handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_send_only_name (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_send_msg_to_only_name_entry, GoBackHistory);
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_send_and_save
* DESCRIPTION
*   Highlight send and save handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_send_and_save (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
						mmi_msg_send_and_save_to_entry, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_save
* DESCRIPTION
*   Highlight save handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_save (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_save_msg_to_entry, GoBackHistory);
}

#if defined(__MMI_MESSAGES_SEND_TO_MANY__)
/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_send_to_many
* DESCRIPTION
*   Highlight send to many handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_send_to_many (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
								mmi_msg_entry_send_to_many, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_send_to_many_list
* DESCRIPTION
*   Highlight send to many list handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_send_to_many_list (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
								mmi_msg_entry_send_to_many_list, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_send_to_many_now
* DESCRIPTION
*   Highlight send to many now handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_send_to_many_now (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
								mmi_msg_send_msg_to_many, GoBackHistory);
}
#endif
#ifdef __MMI_MESSAGES_SEND_BY_GROUP__
/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_send_by_group
* DESCRIPTION
*   Highlight send by group
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_send_by_group (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
								mmi_msg_send_msg_by_group, GoBackHistory);
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_reply
* DESCRIPTION
*   Highlight replay handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_reply (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
								mmi_msg_reply_msg_to_entry, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_send_outbox
* DESCRIPTION
*   Highlight outbox send handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_send_outbox (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_send_option_outbox, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_edit
* DESCRIPTION
*   Highlight edit handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_edit (void)
{

#if defined(__MMI_SMART_MESSAGE_MT__) || defined(__MMI_MYPICTURE__)
	if( mmi_nsm_get_msg_type() == PICTURE_MESSAGE )
	{
		
		//mmi_nsm_init_picmsg_txt_struct(&picmsg_text_struct);
	//	pfnUnicodeStrcpy((S8*)picmsg_text_struct.pic_textbuff,(S8*)g_pmsg_TextInMessage);
#if defined(__MMI_MYPICTURE__)
		if (IsScreenPresent(SCR_ID_MSG_OUTBOX_LIST))
			highlight_mypic_addtext_handler();
		else
#endif
		highlight_picmsg_edit_text();
	}
	else
#endif
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_edit_msg_from_view, GoBackHistory);
}

#ifdef __MMI_MESSAGES_COPY__
/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_copy
* DESCRIPTION
*   Highlight copy handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_copy(void)
{	
	U8	dst_storage=SMSAL_ME;
	switch(g_msg_cntx.toDisplayMessageList)
	{
		case TO_DISPLAY_MESSAGE_LIST_INBOX:
			 if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_INBOX,(U16) g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;
			break;
		case TO_DISPLAY_MESSAGE_LIST_OUTBOX:
			if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_OUTBOX, (U16)g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;
			break;
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case TO_DISPLAY_MESSAGE_LIST_DRAFTBOX:
			if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_DRAFTS, (U16)g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;
			break;
		#endif
	}

	if(dst_storage == SMSAL_SM)
		mmi_msg_set_confirm_screen(STR_SMS_COPY_TO_SIM, mmi_msg_copy_msg, GoBackHistory);	
	else
		mmi_msg_set_confirm_screen(STR_SMS_COPY_TO_PHONE, mmi_msg_copy_msg, GoBackHistory);
	
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
mmi_msg_highlight_copy_all
* DESCRIPTION
*   Highlight copy all inbox handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_copy_all_inbox (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_copy_all_inbox, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_copy_all
* DESCRIPTION
*   Entry copy all list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_copy_all_inbox (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_INBOX)==MMI_FRM_SMS_INVALID_INDEX)
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else
	{
		EntryNewScreen(SCR_ID_MSG_INBOX_COPY_ALL_OPTION, mmi_msg_exit_generic, mmi_msg_entry_copy_all_inbox, NULL);	
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_copy_all_inbox");

		guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_INBOX_COPY_ALL_OPTION);
		
		numItems = GetNumOfChild (SMS_INBOX_OPT_COPY_ALL_MENUID);	
		GetSequenceStringIds(SMS_INBOX_OPT_COPY_ALL_MENUID, nStrItemList);	

		SetParentHandler (SMS_INBOX_OPT_COPY_ALL_MENUID);
		
		RegisterHighlightHandler (ExecuteCurrHiliteHandler);
		SetMessagesCurrScrnID(SCR_ID_MSG_INBOX_COPY_ALL_OPTION);
			
		ShowCategory52Screen(STR_SMS_COPY_ALL,	IMG_SMS_ENTRY_SCRN_CAPTION, 
							  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
							  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
							  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
							  			0, 0,	guiBuffer );
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_copy_all_from_sim_inbox
* DESCRIPTION
*   highlight copy all from sim handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_copy_all_from_sim_inbox(void)
{
	mmi_msg_set_confirm_screen(STR_SMS_FROM_SIM, mmi_msg_copy_msg_all_from_sim_inbox, GoBackHistory);	
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_copy_all_from_phone_inbox
* DESCRIPTION
*   highlight copy all from phone handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_copy_all_from_phone_inbox(void)
{
	mmi_msg_set_confirm_screen(STR_SMS_FROM_PHONE, mmi_msg_copy_msg_all_from_phone_inbox, GoBackHistory);	
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}
/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_copy_all_outbox
* DESCRIPTION
*   Entry copy all outbox list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_copy_all_outbox (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_OUTBOX)==MMI_FRM_SMS_INVALID_INDEX)
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else
	{
		EntryNewScreen(SCR_ID_MSG_OUTBOX_COPY_ALL_OPTION, mmi_msg_exit_generic, mmi_msg_entry_copy_all_outbox, NULL);	
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_copy_all_outbox");

		guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_OUTBOX_COPY_ALL_OPTION);
		
		numItems = GetNumOfChild (SMS_OUTBOX_OPT_COPY_ALL_MENUID);	
		GetSequenceStringIds(SMS_OUTBOX_OPT_COPY_ALL_MENUID, nStrItemList);	

		SetParentHandler (SMS_OUTBOX_OPT_COPY_ALL_MENUID);
		
		RegisterHighlightHandler (ExecuteCurrHiliteHandler);
		SetMessagesCurrScrnID(SCR_ID_MSG_OUTBOX_COPY_ALL_OPTION);
			
		ShowCategory52Screen(STR_SMS_COPY_ALL,	IMG_SMS_ENTRY_SCRN_CAPTION, 
							  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
							  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
							  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
							  			0, 0,	guiBuffer );
	}
}


/*****************************************************************************
* FUNCTION
* mmi_msg_highlight_copy_all_outbox
* DESCRIPTION
*   Highlight copy all outbox handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_copy_all_outbox (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_copy_all_outbox, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_copy_all_from_sim_outbox
* DESCRIPTION
*   highlight copy all from sim handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_copy_all_from_sim_outbox(void)
{
	if(g_msg_cntx.toDisplayMessageList==TO_DISPLAY_MESSAGE_LIST_OUTBOX)
		mmi_msg_set_confirm_screen(STR_SMS_FROM_SIM, mmi_msg_copy_msg_all_from_sim_outbox, GoBackHistory);	
	
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
	else if(g_msg_cntx.toDisplayMessageList == TO_DISPLAY_MESSAGE_LIST_DRAFTBOX)
			mmi_msg_set_confirm_screen(STR_SMS_FROM_SIM, mmi_msg_copy_msg_all_from_sim_draftbox, GoBackHistory);	
	#endif
	
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_copy_all_from_phone_outbox
* DESCRIPTION
*   highlight copy all from phone handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_copy_all_from_phone_outbox(void)
{
	if(g_msg_cntx.toDisplayMessageList==TO_DISPLAY_MESSAGE_LIST_OUTBOX)
		mmi_msg_set_confirm_screen(STR_SMS_FROM_PHONE, mmi_msg_copy_msg_all_from_phone_outbox, GoBackHistory);	
	
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
	else if(g_msg_cntx.toDisplayMessageList == TO_DISPLAY_MESSAGE_LIST_DRAFTBOX)
			mmi_msg_set_confirm_screen(STR_SMS_FROM_PHONE, mmi_msg_copy_msg_all_from_phone_draftbox, GoBackHistory);	
	#endif
	
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_move
* DESCRIPTION
*   Highlight move handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_move(void)
{	
	U8	dst_storage=SMSAL_ME;
	switch(g_msg_cntx.toDisplayMessageList)
	{
		case TO_DISPLAY_MESSAGE_LIST_INBOX:
			 if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_INBOX, (U16)g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;
			break;
		case TO_DISPLAY_MESSAGE_LIST_OUTBOX:
			if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_OUTBOX, (U16)g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;
			break;
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
		case TO_DISPLAY_MESSAGE_LIST_DRAFTBOX:
			if( mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_DRAFTS, (U16)g_msg_cntx.currBoxIndex) == SMSAL_ME)
			 	dst_storage = SMSAL_SM;
			break;
		#endif
	}

	if(dst_storage == SMSAL_SM)
		mmi_msg_set_confirm_screen(STR_SMS_MOVE_TO_SIM, mmi_msg_move_msg, GoBackHistory);	
	else
		mmi_msg_set_confirm_screen(STR_SMS_MOVE_TO_PHONE, mmi_msg_move_msg, GoBackHistory);
	
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
mmi_msg_highlight_move_all
* DESCRIPTION
*   Highlight move all inbox handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_move_all_inbox (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_move_all_inbox, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_move_all
* DESCRIPTION
*   Entry move all list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_move_all_inbox (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_INBOX)==MMI_FRM_SMS_INVALID_INDEX)
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else
	{
		EntryNewScreen(SCR_ID_MSG_INBOX_MOVE_ALL_OPTION, mmi_msg_exit_generic, mmi_msg_entry_move_all_inbox, NULL);	
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_move_all_inbox");

		guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_INBOX_MOVE_ALL_OPTION);
		
		numItems = GetNumOfChild (SMS_INBOX_OPT_MOVE_ALL_MENUID);	
		GetSequenceStringIds(SMS_INBOX_OPT_MOVE_ALL_MENUID, nStrItemList);	

		SetParentHandler (SMS_INBOX_OPT_MOVE_ALL_MENUID);
		
		RegisterHighlightHandler (ExecuteCurrHiliteHandler);
		SetMessagesCurrScrnID(SCR_ID_MSG_INBOX_MOVE_ALL_OPTION);
			
		ShowCategory52Screen(STR_SMS_MOVE_ALL,	IMG_SMS_ENTRY_SCRN_CAPTION, 
							  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
							  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
							  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
							  			0, 0,	guiBuffer );
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_move_all_from_sim_inbox
* DESCRIPTION
*   highlight move all from sim handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_move_all_from_sim_inbox(void)
{
	mmi_msg_set_confirm_screen(STR_SMS_FROM_SIM, mmi_msg_move_msg_all_from_sim_inbox, GoBackHistory);	
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_move_all_from_phone_inbox
* DESCRIPTION
*   highlight move all from phone handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_move_all_from_phone_inbox(void)
{
	mmi_msg_set_confirm_screen(STR_SMS_FROM_PHONE, mmi_msg_move_msg_all_from_phone_inbox, GoBackHistory);	
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}
/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_move_all_outbox
* DESCRIPTION
*   Entry move all outbox list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_move_all_outbox (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_OUTBOX)==MMI_FRM_SMS_INVALID_INDEX)
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else
	{
		EntryNewScreen(SCR_ID_MSG_OUTBOX_MOVE_ALL_OPTION, mmi_msg_exit_generic, mmi_msg_entry_move_all_outbox, NULL);	
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_move_all_outbox");

		guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_OUTBOX_MOVE_ALL_OPTION);
		
		numItems = GetNumOfChild (SMS_OUTBOX_OPT_MOVE_ALL_MENUID);	
		GetSequenceStringIds(SMS_OUTBOX_OPT_MOVE_ALL_MENUID, nStrItemList);	

		SetParentHandler (SMS_OUTBOX_OPT_MOVE_ALL_MENUID);
		
		RegisterHighlightHandler (ExecuteCurrHiliteHandler);
		SetMessagesCurrScrnID(SCR_ID_MSG_OUTBOX_MOVE_ALL_OPTION);
			
		ShowCategory52Screen(STR_SMS_MOVE_ALL,	IMG_SMS_ENTRY_SCRN_CAPTION, 
							  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
							  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
							  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
							  			0, 0,	guiBuffer );
	}
}


/*****************************************************************************
* FUNCTION
* mmi_msg_highlight_move_all_outbox
* DESCRIPTION
*   Highlight move all outbox handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_move_all_outbox (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_move_all_outbox, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_move_all_from_sim_outbox
* DESCRIPTION
*   highlight move all from sim handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_move_all_from_sim_outbox(void)
{
	if(g_msg_cntx.toDisplayMessageList==TO_DISPLAY_MESSAGE_LIST_OUTBOX)
		mmi_msg_set_confirm_screen(STR_SMS_FROM_SIM, mmi_msg_move_msg_all_from_sim_outbox, GoBackHistory);	
	
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
	else if(g_msg_cntx.toDisplayMessageList == TO_DISPLAY_MESSAGE_LIST_DRAFTBOX)
			mmi_msg_set_confirm_screen(STR_SMS_FROM_SIM, mmi_msg_move_msg_all_from_sim_draftbox, GoBackHistory);	
	#endif
	
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_move_all_from_phone_outbox
* DESCRIPTION
*   highlight move all from phone handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_move_all_from_phone_outbox(void)
{
	if(g_msg_cntx.toDisplayMessageList==TO_DISPLAY_MESSAGE_LIST_OUTBOX)
		mmi_msg_set_confirm_screen(STR_SMS_FROM_PHONE, mmi_msg_move_msg_all_from_phone_outbox, GoBackHistory);	
	
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
	else if(g_msg_cntx.toDisplayMessageList == TO_DISPLAY_MESSAGE_LIST_DRAFTBOX)
			mmi_msg_set_confirm_screen(STR_SMS_FROM_PHONE, mmi_msg_move_msg_all_from_phone_draftbox, GoBackHistory);	
	#endif
	
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}
#endif  // __MMI_MESSAGES_COPY_

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_delete
* DESCRIPTION
*   Highlight delete handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_delete (void)
{
	mmi_msg_set_confirm_screen(STR_GLOBAL_DELETE, mmi_msg_delete_msg, GoBackHistory);

	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_confirm_generic, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_delete_all_inbox
* DESCRIPTION
*   Highlight inbox delete all handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_delete_all_inbox (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_delete_msg_all_inbox, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_delete_all_outbox
* DESCRIPTION
*   Highlight outbox delete all handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_delete_all_outbox (void)
{
#if defined(__MMI_MESSAGES_DELETE_ALL_MENU__)
		mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_delete_msg_all_outbox, GoBackHistory);
#else
	if(g_msg_cntx.toDisplayMessageList==TO_DISPLAY_MESSAGE_LIST_OUTBOX)

		mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_delete_msg_all_outbox, GoBackHistory);
#ifdef __MMI_MESSAGES_DRAFT_BOX__
	else
		mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_delete_msg_all_draftbox, GoBackHistory);
#endif
#endif
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

#if defined(__MMI_MESSAGES_DELETE_ALL_MENU__) && defined(__MMI_MESSAGES_DRAFT_BOX__)
/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_delete_all_draftbox
* DESCRIPTION
*   Highlight delete all draftbox handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_delete_all_draftbox (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_delete_msg_all_draftbox, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
#endif

#if defined(__MMI_MESSAGES_EMS__)
/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_save_object
* DESCRIPTION
*   Highlight save object handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_save_object (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_highlight_save_object");
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryScrSaveObjectList, KEY_EVENT_UP);
	SetKeyHandler (EntryScrSaveObjectList, KEY_RIGHT_ARROW, KEY_EVENT_DOWN); 
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP); 
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN); 
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsTextSizeHandler
* DESCRIPTION
*   Highlight Ems TextS ize Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsTextSizeHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryScrFmtTextSizeMenu, KEY_EVENT_UP);
	SetKeyHandler (EntryScrFmtTextSizeMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsTextStyleHandler
* DESCRIPTION
*   Highlight Ems Text Style Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsTextStyleHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryScrFmtTextStyleMenu, KEY_EVENT_UP);
	SetKeyHandler (EntryScrFmtTextStyleMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsAlignmentHandler
* DESCRIPTION
*   Highlight Ems Alignment Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsAlignmentHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryScrFmtAlignmentMenu, KEY_EVENT_UP);
	SetKeyHandler (EntryScrFmtAlignmentMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsNewParagraphHandler
* DESCRIPTION
*   Highlight Ems New Paragraph Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsNewParagraphHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);

	SetLeftSoftkeyFunction (EntryScrFmtNewParagraphMenu, KEY_EVENT_UP);
	SetKeyHandler (EntryScrFmtNewParagraphMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsTextSizeSmallHandler
* DESCRIPTION
*   Highlight Ems Text Size Small Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsTextSizeSmallHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextSize, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsTextSizeMediumHandler
* DESCRIPTION
*   Highlight Ems Text Size Medium Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsTextSizeMediumHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextSize, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsTextSizeLargeHandler
* DESCRIPTION
*   Highlight Ems Text Size Large Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsTextSizeLargeHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextSize, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsTextStyleBoldHandler
* DESCRIPTION
*   Highlight Ems Text Style Bold Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsTextStyleBoldHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextStyle, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsTextStyleItalicsHandler
* DESCRIPTION
*   Highlight Ems Text Style Italics Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsTextStyleItalicsHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextStyle, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsTextStyleUnderlineHandler
* DESCRIPTION
*   Highlight Ems Text Style Underline Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsTextStyleUnderlineHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextStyle, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsTextStyleStrikethroughHandler
* DESCRIPTION
*   Highlight Ems Text Style Strikethrough Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsTextStyleStrikethroughHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextStyle, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsAlignmentAutomaticHandler
* DESCRIPTION
*   Highlight Ems Alignment Automatic Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsAlignmentAutomaticHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextAlignment, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsAlignmentLeftHandler
* DESCRIPTION
*   Highlight Ems Alignment Left Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsAlignmentLeftHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextAlignment, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsAlignmentCenterHandler
* DESCRIPTION
*   Highlight Ems Alignment Center Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsAlignmentCenterHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextAlignment, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsAlignmentRightHandler
* DESCRIPTION
*   Highlight Ems Alignment Right Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsAlignmentRightHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextAlignment, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsNewParagraphAutomaticHandler
* DESCRIPTION
*   Highlight Ems New Paragraph Automatic Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsNewParagraphAutomaticHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextNewParagraph, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsNewParagraphLeftHandler
* DESCRIPTION
*   Highlight Ems New Paragraph Left Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsNewParagraphLeftHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextNewParagraph, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsNewParagraphCenterHandler
* DESCRIPTION
*   Highlight Ems New Paragraph Center Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsNewParagraphCenterHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextNewParagraph, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsNewParagraphRightHandler
* DESCRIPTION
*   Highlight Ems New Paragraph Right Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsNewParagraphRightHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SetFmtTextNewParagraph, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif // defined(__MMI_MESSAGES_EMS__)

#ifdef __MMI_MESSAGES_DELETE_ALL_MENU__
/*****************************************************************************
* FUNCTION
*  HighlightSMSDeleteHandler
* DESCRIPTION
*   Highlight SMS delete handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightSMSDeleteHandler (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_delete, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_delete
* DESCRIPTION
*   Entry delete screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_delete(void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_MSG_DELETE	, mmi_msg_exit_generic, mmi_msg_entry_delete, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_delete");
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_DELETE	);
	
	numItems = GetNumOfChild (MESSAGES_MENU_DELETE_MENUID);	
	GetSequenceStringIds(MESSAGES_MENU_DELETE_MENUID, nStrItemList);	
	SetParentHandler (MESSAGES_MENU_DELETE_MENUID);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_DELETE	);
	SetMessagesCurrScrnID(SCR_ID_MSG_DELETE	);
		
	ShowCategory52Screen(STR_GLOBAL_DELETE,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_delete_all
* DESCRIPTION
*   Highlight delete all handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_delete_all (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_delete_msg_all, GoBackHistory);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
#endif

//CSD Add By Jo for MMS 20030822
#ifdef __MMI_MESSAGES_MULTI_APP__
/*****************************************************************************
* FUNCTION
*  HighlightSMSHandler
* DESCRIPTION
*   Highlight SMS handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightSMSHandler (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: HighlightSMSHandler");
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntrySMSMainMenuList, KEY_EVENT_UP);
	SetKeyHandler (EntrySMSMainMenuList, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
#endif

#ifdef MMS_SUPPORT
/*****************************************************************************
* FUNCTION
*  HighlightMMSHandler
* DESCRIPTION
*   Highlight MMS handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void goto_mms_screen(void);
void HighlightMMSHandler (void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: HighlightMMSHandler");
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
    #ifdef WAP_SUPPORT
	SetLeftSoftkeyFunction (goto_mms_screen, KEY_EVENT_UP);
	SetKeyHandler (goto_mms_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    #endif
}
#endif

#ifdef __MMI_MESSAGES_MULTI_APP__
/*****************************************************************************
* FUNCTION
*  EntrySMSMainMenuList
* DESCRIPTION
*   Entry SMS main menu screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#include "servicedefs.h"
void EntrySMSMainMenuList (void)
{ 
	U8* guiBuffer;
	U16 nStrItemList[MAX_SUB_MENUS];
	U16 numItems;
	U16	itemIcons[MAX_SUB_MENUS];
	U8*	popUpList[MAX_SUB_MENUS];
	/*S32 menuItemId=-1;
	S32 menuItemId1=-1;	
	S32 maskingByte=-1;*/
	U8** pPopUpList=NULL;	
#ifdef __MMI_MAINLCD_220X176__
	if (MTPNP_PFAL_Is_CardValid(0) == 0 && MTPNP_PFAL_Is_CardValid(1) == 0)
	{
		DisplayPopup( (PU8)GetString(SERVICES_NOT_PROVIDED_STRING_ID), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		return;
	};
#endif	
	EntryNewScreen(SCR_ID_MSG_SMS_MAIN_MENU, mmi_msg_exit_generic, EntrySMSMainMenuList, NULL);

#if (defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
#endif
	mmi_trace(g_sw_SMS, "MMI_SMS: EntrySMSMainMenuList");
	SetMessagesCurrScrnID(SCR_ID_MSG_SMS_MAIN_MENU);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_SMS_MAIN_MENU);		
	SetParentHandler (MESSAGES_MENU_SMS_MENUID);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);

	 
	numItems = GetNumOfChild_Ext(MESSAGES_MENU_SMS_MENUID);
	GetSequenceStringIds_Ext(MESSAGES_MENU_SMS_MENUID, nStrItemList);
	GetSequenceImageIds_Ext(MESSAGES_MENU_SMS_MENUID, itemIcons);

	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)!=MMI_FRM_SMS_INVALID_INDEX)
	{
		msgbox_info.totalinbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
		msgbox_info.totaloutbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX);
		msgbox_info.totaldraftbox=mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS);	
	        mmi_msg_set_msg_num_hint (msgbox_info.totalinbox, msgbox_info.totaloutbox, msgbox_info.totaldraftbox);
		RefreshMessagesMenuList();
	}
	
	ConstructHintsList(MESSAGES_MENU_SMS_MENUID, popUpList);
	mmi_msg_set_msg_menu_highlight_handler(); 

	if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)!=MMI_FRM_SMS_INVALID_INDEX)
	{
		pPopUpList = popUpList;
	}
	else
	{
		pPopUpList = NULL;
	}

 
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */

	g_msg_cntx.currBoxIndex=0;

	ShowCategory52Screen (STR_SMS_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION, 
						  STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE,
						  STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE,
						  numItems,
						  nStrItemList, itemIcons,
						  (U8 **) pPopUpList ,
						  0, 0,
						  guiBuffer );

	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP); //CSD Jo
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

#endif

/*****************************************************************************
* FUNCTION
*  HighlightWMessageHandler
* DESCRIPTION
*   Highlight write message handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightWMessageHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (mmi_msg_entry_write_msg, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (mmi_msg_entry_write_msg, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
//	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	g_msg_cntx.sendMessageCase=SEND_CASE_SEND_ONLY;
	g_msg_cntx.PhbSmsInterfaceState=MMI_SEND_ONLY;
}

/*****************************************************************************
* FUNCTION
*  HighlightInboxHandler
* DESCRIPTION
*   Highlight inbox handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightInboxHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (mmi_msg_pre_entry_inbox_list, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (mmi_msg_pre_entry_inbox_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HintInboxHandler
* DESCRIPTION
*   Hint inbox handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HintInboxHandler (U16 index)
{
	memset((PS8)hintData[index], 0, sizeof(hintData[index]));
	pfnUnicodeStrcpy((PS8) hintData[index], (PS8)TotalInboxOutboxStatus.totalInboxStr);
}

/*****************************************************************************
* FUNCTION
*  HighlightOutboxHandler
* DESCRIPTION
*   Highlight outbox handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightOutboxHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (mmi_msg_pre_entry_outbox_list, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (mmi_msg_pre_entry_outbox_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HintOutboxHandler
* DESCRIPTION
*   Hint outbox handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HintOutboxHandler (U16 index)
{
	memset((PS8)hintData[index], 0, sizeof(hintData[index]));
	pfnUnicodeStrcpy((PS8) hintData[index], (PS8)TotalInboxOutboxStatus.totalOutboxStr);
}

#ifdef __MMI_MESSAGES_DRAFT_BOX__
/*****************************************************************************
* FUNCTION
*  HighlightDraftboxHandler
* DESCRIPTION
*   Highlight draftbox handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightDraftboxHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (mmi_msg_pre_entry_draftbox_list, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (mmi_msg_pre_entry_draftbox_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HintDraftboxHandler
* DESCRIPTION
*   Hint draftbox handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HintDraftboxHandler (U16 index)
{
	memset((PS8)hintData[index], 0, sizeof(hintData[index]));
	pfnUnicodeStrcpy((PS8) hintData[index], (PS8)TotalInboxOutboxStatus.totalDraftboxStr);
}
#endif

#if defined(__MMI_MESSAGES_TEMPLATE__)
/*****************************************************************************
* FUNCTION
*  HighlightTemplatesHandler
* DESCRIPTION
*   Highlight template handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightTemplatesHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction (mmi_msg_entry_template_list, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_entry_template_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_insert_template
* DESCRIPTION
*   Highlight insert template handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_insert_template (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_template_list, GoBackHistory);
}
#endif // defined(__MMI_MESSAGES_TEMPLATE__)
#ifdef __MMI_MESSAGES_INSERT_FROM_PHB__
void SetNameNumberForSMSInsert(U8 name[], U8 number[])
{
	U8 *	templ = NULL;
	EMSData * pEMS;
	history_t	temphistory;
	historyNode *History;
	U16 len = 0;

	#if defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)
	/* under construction !*/
	/* under construction !*/
	#else
	U8 result=0;
	#endif
	if(gSetFlagNameNumber == SMS_INSERT_PHB_NUMBER)
	{
		len = pfnUnicodeStrlen((PS8)number);
		templ = OslMalloc((len+1) * ENCODING_LENGTH);
		MMI_ASSERT(NULL != templ);
		memset(templ, 0, (len+1) * ENCODING_LENGTH);
		templ = pfnUnicodeStrcpy(templ, number);
	}
	else if(gSetFlagNameNumber == SMS_INSERT_PHB_NAME)
	{
		len = pfnUnicodeStrlen((PS8)name);
		templ = OslMalloc((len+1) * ENCODING_LENGTH);
		MMI_ASSERT(NULL != templ);
		memset(templ, 0, (len+1) * ENCODING_LENGTH);
		templ = pfnUnicodeStrcpy(templ, name);
	}
	mmi_trace(g_sw_SMS,"MMI_SMS: SetNameNumberForSMSInsert");
	#if defined(__MMI_SMART_MESSAGE_MO__)&& !defined(__MMI_MESSAGES_EMS__)
	/* under construction !*/
	/* under construction !*/
	#else
	if (templ)
	{
		U16 templateCharNum;		
		BOOL isUcs2Char=0;

		templateCharNum = pfnUnicodeStrlen((PS8)templ); 
		isUcs2Char = mmi_frm_sms_check_UCS2((U16*)templ, templateCharNum);
		
		memset (&temphistory, 0, sizeof (temphistory));
		GetHistory (SCR_ID_MSG_WRITE, &temphistory);
		pEMS = GetEMSDataForEdit (0, 0);

		if(pEMS->dcs == SMSAL_UCS2_DCS || isUcs2Char)
		{
			result = AppendCategory28String (INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
						pEMS, (U8*)templ, temphistory.guiBuffer );
		}
		else	//SMSAL_DEFAULT_DCS
		{
			U8*	handledBuffer;

			handledBuffer=OslMalloc((templateCharNum*2+1)*ENCODING_LENGTH);
			memset((S8*)handledBuffer, 0, ((templateCharNum*2+1)*ENCODING_LENGTH));
			mmi_frm_sms_handle_extension_char((U16*)handledBuffer, (U16*)templ,templateCharNum);
			result = AppendCategory28String (INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
							pEMS, (U8*)handledBuffer, temphistory.guiBuffer );

			OslMfree(handledBuffer);
		}
		if(GetHistoryScrID(SCR_ID_MSG_WRITE,&History)==ST_SUCCESS)
			memcpy(History->guiBuffer, temphistory.guiBuffer, MAX_GUI_BUFFER);		

		if (!result)
			mmi_trace(g_sw_SMS, "MMI_SMS: SetNameNumberForSMSInsert - ERROR in inserting phb number or name");

		OslMfree(templ);
		templ = NULL;
	}
	if (result==0)
	{
		GoBackToHistory (SCR_ID_MSG_WRITE);
		DisplayPopup ((PU8)GetString (STR_NO_SPACE_TO_INSERT_OBJECT),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);			
	}
	else
		GoBackToHistory (SCR_ID_MSG_WRITE);
	#endif
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
void EnterPhoneBookForSMSInsertNumber(void)
{
	phbListView=MMI_PHB_LIST_FOR_SMS_INSERT;
	g_phb_cntx.highlight_entry = 0;
	gSetFlagNameNumber = SMS_INSERT_PHB_NUMBER;
	mmi_phb_list_pre_entry_second_level();
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
void EnterPhoneBookForSMSInsertName(void)
{
	phbListView=MMI_PHB_LIST_FOR_SMS_INSERT_NO_CHECK;
	g_phb_cntx.highlight_entry = 0;
	gSetFlagNameNumber = SMS_INSERT_PHB_NAME;
	mmi_phb_list_pre_entry_second_level();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_insert_num
* DESCRIPTION
*   Highlight insert number handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_insert_number(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EnterPhoneBookForSMSInsertNumber,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(EnterPhoneBookForSMSInsertNumber, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
/*****************************************************************************
* FUNCTION
*  mmi_msg_higlight_insert_nam
* DESCRIPTION
*   Highlight insert name handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_higlight_insert_name(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EnterPhoneBookForSMSInsertName,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(EnterPhoneBookForSMSInsertName, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif

/*****************************************************************************
* FUNCTION
*  HighlightVoiceMailHandler
* DESCRIPTION
*   Highlight voice madil handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightVoiceMailHandler (void)
{
	#ifdef __MMI_MULTI_SIM__
	U8 reqSrcMod = GetReqSrcModIndex();
	mmi_frm_sms_set_active_simid(reqSrcMod);
	#endif
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction (mmi_msg_pre_entry_voice_mail_server, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_pre_entry_voice_mail_server, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}	

/*****************************************************************************
* FUNCTION
*  HighlightCBHandler
* DESCRIPTION
*   Highlight CB Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightCBHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	//SetLeftSoftkeyFunction (mmi_msg_pre_entry_cb_main_menu, KEY_EVENT_UP);// wangbei del 20060619
	SetLeftSoftkeyFunction (mmi_msg_entry_cb_main_menu, KEY_EVENT_UP);// wangbei add 20060619
	
	SetKeyHandler (mmi_msg_pre_entry_cb_main_menu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);// wangbei del 20060619
}
#ifdef __MMI_MESSAGES_CHAT__
/*****************************************************************************
* FUNCTION
*  HighlightChatMenu
* DESCRIPTION
*   Highlight Chat Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void EntryChatApp(void) ;
void HighlightChatMenu(void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryChatApp, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (EntryChatApp, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
 
/*****************************************************************************
* FUNCTION
*  EnterSmsChatRoom
* DESCRIPTION
*   Entry Chat application screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EnterSmsChatRoom(void)
{	
	S8 *number;
	S8 *unicodenumber=NULL;
	
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)==MMI_FRM_SMS_INVALID_INDEX)
	{
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
	number = (S8*) mmi_frm_sms_get_sms_address(MMI_FRM_SMS_INBOX, (U16)g_msg_cntx.currBoxIndex);
	if (number)
	{
		unicodenumber = OslMalloc((strlen(number)+1)*ENCODING_LENGTH);
		AnsiiNToUnicodeString(unicodenumber, number, strlen(number));
		memset(unicodenumber+strlen(number)*ENCODING_LENGTH, 0, ENCODING_LENGTH);
		ChatInboxInterface((U8 *)unicodenumber);
	}
	else
	{
		PRINT_INFORMATION (("*-------[SmsMoMtGuiInterface.c] EntryScrUseNumbers (numbersCount <= 0) -------*\n"));
		DisplayPopup ((PU8)GetString (STR_NONUMBER_NOTIFICATION), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
//		DeleteMessagesHistoryNodes ();
	}
	if(unicodenumber!=NULL)
        {
	      OslMfree (unicodenumber);
              unicodenumber=NULL;
        }
}

/*****************************************************************************
* FUNCTION
*  HilightChatInboxMenu
* DESCRIPTION
*   Highlight Inbox Chat Option Handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HilightChatInboxMenu(void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EnterSmsChatRoom, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_set_msg_menu_highlight_handler
* DESCRIPTION
*   Set message menu highlight handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_msg_menu_highlight_handler(void)
{
#ifdef __MMI_MESSAGES_MULTI_APP__
	SetHiliteHandler (MESSAGES_MENU_SMS_MENUID, HighlightSMSHandler);
   #ifdef MMS_SUPPORT
   	SetHiliteHandler (MESSAGES_MENU_MMS_MENUID, HighlightMMSHandler);
   #endif

   #ifdef __MMI_MESSAGES_CHAT__
   	SetHiliteHandler (MENU_CHATAPP_ID, HighlightChatMenu);
   #endif

   #ifdef __MMI_SMARTMESSAGE__
   	SetHiliteHandler(	MENU_ID_MY_PICTURE,		highlight_mypic_handler );
   #endif

   #ifdef __MMI_EMAIL__
   	SetHiliteHandler (MENU_ID_EMAIL_MAIN,	mmi_email_highlight_main_menu);
   #endif

   #ifdef __MMI_IMPS__
	SetHiliteHandler (MENU_ID_IMPS_MAIN,	mmi_imps_hilite_main);
   #endif

	SetHiliteHandler (MESSAGES_MENU_VMAIL_MENUID, HighlightVoiceMailHandler);
	SetHiliteHandler (MESSAGES_MENU_CB_MENUID, HighlightCBHandler);
	
#else
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MESSAGES_MENU_MSG_SETTINGS_MENUID + i, mmi_msg_highlight_msg_settings);
		SetHiliteHandler (MESSAGES_MENU_VMAIL_MENUID + i, HighlightVoiceMailHandler);
	}
	SetHiliteHandler (MESSAGES_MENU_WMESSAGE_MENU_ID, HighlightWMessageHandler);
	SetHiliteHandler (MESSAGES_MENU_INBOX_MENUID, HighlightInboxHandler);
	SetHintHandler (MESSAGES_MENU_INBOX_MENUID, HintInboxHandler);
	SetHiliteHandler (MESSAGES_MENU_OUTBOX_MENUID, HighlightOutboxHandler);
	SetHintHandler (MESSAGES_MENU_OUTBOX_MENUID, HintOutboxHandler);
   #ifdef __MMI_MESSAGES_DRAFT_BOX__
   	SetHiliteHandler (MESSAGES_MENU_DRAFTBOX_MENUID, HighlightDraftboxHandler);
   	SetHintHandler (MESSAGES_MENU_DRAFTBOX_MENUID, HintDraftboxHandler);
   #endif
   #if defined(__MMI_MESSAGES_DELETE_ALL_MENU__)
   	SetHiliteHandler (MESSAGES_MENU_DELETE_MENUID, HighlightSMSDeleteHandler);
   #endif
   #if defined(__MMI_MESSAGES_TEMPLATE__)
   	SetHiliteHandler (MESSAGES_MENU_TEMPLATES_MENUID, HighlightTemplatesHandler);
   #endif
   	SetHiliteHandler (MESSAGES_MENU_CB_MENUID, HighlightCBHandler);
#endif /* __MMI_MESSAGES_MULTI_APP__ */

	SetHiliteHandler (SETUP_MENU_PROFILE_SETTING_MENUID, mmi_msg_highlight_profile_settings);
	SetHiliteHandler (SETUP_MENU_COMMONSETTING_MENUID, mmi_msg_highlight_common_settings);
#if defined(__MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__)
	SetHiliteHandler (MESSAGES_MENU_MEMSTATUS_MENUID, mmi_msg_highlight_memory_status);
	SetHiliteHandler (SETUP_MENU_PREFEREDSTORAGE_MENUID, mmi_msg_highlight_preferred_storage);
	SetHintHandler (SETUP_MENU_PREFEREDSTORAGE_MENUID, mmi_msg_hint_preferred_storage);
#endif // defined(__MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__)
//CSD Joy added for GPRS support, 1216
#ifdef __MMI_GPRS_FEATURES__
	SetHiliteHandler (SETUP_MENU_PREFEREDBEARER_MENUID, mmi_msg_highlight_bearer_settings);
	SetHintHandler (SETUP_MENU_PREFEREDBEARER_MENUID, mmi_msg_hint_bearer_settings);
#endif
//CSD end
#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_SMS_Init_Show_Style();
#endif

}


#if defined(__MMI_MESSAGES_BLACK_LIST__)

static BOOL gMessagesBlackListFlag=FALSE;
static U8 MessagesBlackListNum[MAX_MESSAGES_BLACK_LIST_NUMBER][(MAX_MESSAGES_BLACK_LIST_NUMBER_LEN+1)*ENCODING_LENGTH]; /* black list number stored in NVRAM */
static U8 dispMessagesBlackListNum[MAX_MESSAGES_BLACK_LIST_NUMBER][(MAX_MESSAGES_BLACK_LIST_NUMBER_LEN+1)*ENCODING_LENGTH]; /* temp black list number for displaying reason */
static S32 curMessagesBlackListNumIndex = 0;

void HighlightMessagesBlackList(void);
void HintMessagesBlackList(U16 index);
void HighlightMessagesBlackListMode(void);
void HighlightMessagesBlackListNum(void);
void HintMessagesBlackListMode(U16 index);
BOOL GetMessagesBlackListMode(void);
void ExitScrMessagesBlackListNum( void );
void LoadMessagesBlackListNum(void)
{
	S16 error;
	U16 index,index2;
	U8 number[20];
	INT32   ret = 0;
	
	memset(MessagesBlackListNum,0,sizeof(MessagesBlackListNum));
	memset(dispMessagesBlackListNum,0,sizeof(dispMessagesBlackListNum));
	ReadValue(NVRAM_MESSAGES_BLACK_LIST_SETTING, &gMessagesBlackListFlag, DS_BYTE, &error);
	if(gMessagesBlackListFlag == 0xff)
	{
		gMessagesBlackListFlag = 0;
		WriteValue(NVRAM_MESSAGES_BLACK_LIST_SETTING, &gMessagesBlackListFlag, DS_BYTE, &error);
	}

	ret = ReadRecord(NVRAM_EF_MESSAGES_BLACK_LIST_LID, 1, MessagesBlackListNum, sizeof(MessagesBlackListNum), &error);

	if(error == NVRAM_READ_SUCCESS)
	{
		for (index=0;index<MAX_MESSAGES_BLACK_LIST_NUMBER;index++)
		{  
			UnicodeToAnsii(number, MessagesBlackListNum[index]);
			for (index2=0;index2<strlen(number);index2++)
			if ((number[index2]<'0') || (number[index2]>'9'))
			{
				memset(MessagesBlackListNum[index],0,(MAX_MESSAGES_BLACK_LIST_NUMBER_LEN+1)*ENCODING_LENGTH);
				break;
			}
		}
	}
	else	 
	{
		memset(MessagesBlackListNum,0,sizeof(MessagesBlackListNum));
		WriteRecord(NVRAM_EF_MESSAGES_BLACK_LIST_LID, 1, MessagesBlackListNum, sizeof(MessagesBlackListNum), &error);
		ReadRecord(NVRAM_EF_MESSAGES_BLACK_LIST_LID, 1, MessagesBlackListNum, sizeof(MessagesBlackListNum), &error);
	}
}

void InitMessagesBlackList(void)
{
	SetHiliteHandler(MENU_MESSAGES_BLACK_LIST, HighlightMessagesBlackList);
	SetHintHandler(MENU_MESSAGES_BLACK_LIST, HintMessagesBlackList);
	
	SetHiliteHandler(MENU_MESSAGES_BLACK_LIST_MODE, HighlightMessagesBlackListMode);
	
	SetHiliteHandler(MENU_MESSAGES_BLACK_LIST_NUM, HighlightMessagesBlackListNum);
	SetHintHandler(MENU_MESSAGES_BLACK_LIST_MODE, HintMessagesBlackListMode);

	LoadMessagesBlackListNum();
}

void SetMessagesBlackListMode(pBOOL flag)
{
	S16 error;
	
	gMessagesBlackListFlag = flag;
	WriteValue(NVRAM_MESSAGES_BLACK_LIST_SETTING, &gMessagesBlackListFlag, DS_BYTE, &error);
}

void EntryScrMessagesBlackListMode(void)
{
	U16 index;
   	index = GetCurrHiliteID();
   
	if(GetMessagesBlackListMode() == TRUE)
	{
		SetMessagesBlackListMode(FALSE);
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));		
		Category52ChangeItemDescription(0,(U8*)GetString(STR_GLOBAL_OFF));
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}
	else
	{
		SetMessagesBlackListMode(TRUE);
		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_ON));		
		Category52ChangeItemDescription(0,(U8*)GetString(STR_GLOBAL_ON));
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	RedrawCategoryFunction();
	HighlightMessagesBlackListMode();
	
}


void HighlightMessagesBlackListMode(void)
{
	if (GetMessagesBlackListMode() == TRUE)
		ChangeLeftSoftkey(STR_GLOBAL_OFF, 0);
	else
		ChangeLeftSoftkey(STR_GLOBAL_ON, 0);
	
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryScrMessagesBlackListMode,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void SetCurrMessagesBlackListNumIndex(S32 index)
{
	curMessagesBlackListNumIndex = index;
}

void SetMessagesBlackListNum(void)
{
	U16 index;
	S16 error;
	S16 result;
	for (index=0; index<MAX_MESSAGES_BLACK_LIST_NUMBER; index++)
		pfnUnicodeStrcpy((PS8)MessagesBlackListNum[index], (PS8)dispMessagesBlackListNum[index]);

	result = WriteRecord(NVRAM_EF_MESSAGES_BLACK_LIST_LID, 1, MessagesBlackListNum, sizeof(MessagesBlackListNum), &error);
	
	if (result == -1 || error == NVRAM_WRITE_FAIL)
	{
		DisplayPopup( (U8*)GetString(STR_GLOBAL_ERROR),  IMG_GLOBAL_ERROR, MMI_FALSE, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE );
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	}

	DeleteUptoScrID(SCR_ID_MESSAGES_BLACK_LIST);
}

void GoToMessagesBlackList(void)
{
	GoBackToHistory(SCR_ID_MESSAGES_BLACK_LIST);
}

void EntryScrSetMessagesBlackListNumConfirm( void )
{
	DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,
				STR_GLOBAL_NO,IMG_GLOBAL_NO,
				get_string(STR_GLOBAL_SAVE),
				IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(SetMessagesBlackListNum,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoToMessagesBlackList,KEY_EVENT_UP);
}


void EntryScrMessagesBlackListNum(void)
{
	U8* guiBuffer;		   /* Buffer holding history data */
	U8* inputBuffer;     /* added for inline edit history */
	U16 inputBufferSize; /* added for inline edit history */
	U16 index;
	U16 BlackListNumImageList[MAX_MESSAGES_BLACK_LIST_NUMBER + 1];

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_MESSAGES_BLACK_LIST_NUM, ExitScrMessagesBlackListNum, NULL, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer( SCR_MESSAGES_BLACK_LIST_NUM );

	/* 3 Set current parent id*/
	InitializeCategory57Screen();

	for (index=0; index<MAX_MESSAGES_BLACK_LIST_NUMBER; index++)
	{
		SetInlineItemActivation(&wgui_inline_items[index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
		SetInlineItemTextEdit(&wgui_inline_items[index],(PU8)dispMessagesBlackListNum[index], MAX_MESSAGES_BLACK_LIST_NUMBER_LEN+1,INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING);
		DisableInlineItemHighlight(&wgui_inline_items[index]);
		BlackListNumImageList[index] = IMG_GLOBAL_L1 + index;
	}

	/* added for inline edit history */
	inputBuffer = GetCurrNInputBuffer (SCR_MESSAGES_BLACK_LIST_NUM, &inputBufferSize);

	RegisterHighlightHandler(SetCurrMessagesBlackListNumIndex);

	/* added for inline edit history */
	if((guiBuffer != NULL) && (guiBuffer[0]!=0))
	{
		/* sets the data */
		SetCategory57Data(wgui_inline_items, MAX_MESSAGES_BLACK_LIST_NUMBER, inputBuffer);
	}

	DisableCategory57ScreenDone();

	ShowCategory57Screen(STR_MESSAGES_BLACK_LIST_NUM,
							IMG_SMS_MSG_SETTINGS_MENUENTRY,
							STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, MAX_MESSAGES_BLACK_LIST_NUMBER,
							BlackListNumImageList, wgui_inline_items, curMessagesBlackListNumIndex, guiBuffer);

	SetCategory57RightSoftkeyFunctions(EntryScrSetMessagesBlackListNumConfirm, GoBackHistory);
}

void ExitScrMessagesBlackListNum( void )
{
	history_t currHistory;
	U16 inputBufferSize;                //added for inline edit history
	S16 nHistory	= 0;

	CloseCategory57Screen();
	currHistory.scrnID = SCR_MESSAGES_BLACK_LIST_NUM;
	currHistory.entryFuncPtr = EntryScrMessagesBlackListNum;
	pfnUnicodeStrcpy((S8 *)currHistory.inputBuffer, (S8 *)&nHistory);
	GetCategoryHistory(currHistory.guiBuffer);

	inputBufferSize = (U16)GetCategory57DataSize( );
	GetCategory57Data ((U8 *) currHistory.inputBuffer);
	AddNHistory(currHistory, inputBufferSize);
	return;
}
void HighlightMessagesBlackListNum(void)
{
	U16 index;
	for (index=0; index<MAX_MESSAGES_BLACK_LIST_NUMBER; index++)
		pfnUnicodeStrcpy((PS8)dispMessagesBlackListNum[index], (PS8)MessagesBlackListNum[index]);
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	SetKeyHandler(EntryScrMessagesBlackListNum, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryScrMessagesBlackListNum, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void HintMessagesBlackListMode(U16 index)
{
	memset((S8 *)hintData[index],0,MAX_SUB_MENU_SIZE);
	if(GetMessagesBlackListMode() == TRUE)
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_ON));						
	}
	else
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));						
	}
}
void EntryScrMessagesBlackList(void)
{
	U8*	guiBuffer;
   	U16	nNumofItem;
   	U16	nStrItem[3];
   	U8 *BlackListType[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_MESSAGES_BLACK_LIST, NULL, EntryScrMessagesBlackList, NULL);	

	guiBuffer = GetCurrGuiBuffer(SCR_ID_MESSAGES_BLACK_LIST);

	nNumofItem = GetNumOfChild(MENU_MESSAGES_BLACK_LIST);
	GetSequenceStringIds(MENU_MESSAGES_BLACK_LIST,nStrItem);
	SetParentHandler(MENU_MESSAGES_BLACK_LIST);
	ConstructHintsList(MENU_MESSAGES_BLACK_LIST, BlackListType);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory52Screen(STR_MESSAGES_BLACK_LIST,IMG_SMS_MSG_SETTINGS_MENUENTRY, 
						STR_GLOBAL_OK,IMG_GLOBAL_OK,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
						nNumofItem, nStrItem,
						(U16 *)gIndexIconsImageList, BlackListType,
						0, 0,	guiBuffer );
	
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

BOOL GetMessagesBlackListMode(void)
{
	return gMessagesBlackListFlag;
}

void HighlightMessagesBlackList(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(EntryScrMessagesBlackList, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryScrMessagesBlackList, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
void HintMessagesBlackList(U16 index)
{
	memset((S8 *)hintData[index],0,MAX_SUB_MENU_SIZE);
	if(GetMessagesBlackListMode() == TRUE)
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_ON));						
	}
	else
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));						
	}
}
U8 g_msg_black_list_compare_length = 0xff;
void mmi_msg_black_list_read_compare_length(void)
{
	S16 pError;
	U8 comp_len = 0;

	/*Already read value*/
	if (g_msg_black_list_compare_length != 0xff)
	{
		return;
	}

	ReadRecord(NVRAM_EF_PHB_COMPARE_DIGIT_LID, 1, (void*)&comp_len, 1, &pError);

	if(comp_len < 6)
	{
		g_msg_black_list_compare_length = 6;
	}
	else if (comp_len > 20)
	{
		g_msg_black_list_compare_length = 20;
	}
	else
	{
		g_msg_black_list_compare_length = comp_len;
	}
}
BOOL mmi_msg_black_list_is_number_suited(U8* number1, U8* number2)
{
	U8 num1_len, num2_len;
	U8 temp1[30];
	U8 temp2[30];
	

	memset(temp1, 0, sizeof(temp1));
	memset(temp2, 0, sizeof(temp2));
	num1_len = pfnUnicodeStrlen(number1);
	num2_len = pfnUnicodeStrlen(number2);

	
	if(num1_len >= g_msg_black_list_compare_length && num2_len >= g_msg_black_list_compare_length)
	{
		pfnUnicodeStrcpy(temp1, number1+(num1_len - g_msg_black_list_compare_length)*ENCODING_LENGTH);
		pfnUnicodeStrcpy(temp2, number2+(num2_len - g_msg_black_list_compare_length)*ENCODING_LENGTH);
		
		if(pfnUnicodeStrcmp(temp1, temp2) == 0)
			return MMI_TRUE;
	}
	return MMI_FALSE;
}
BOOL mmi_msg_is_black_number(U8* number)
{
	U8 i;

	if(!GetMessagesBlackListMode())
	{
		return MMI_FALSE;
	}
	
	mmi_msg_black_list_read_compare_length();
	
	for(i = 0; i < MAX_MESSAGES_BLACK_LIST_NUMBER; i++)
	{
		
		if(mmi_msg_black_list_is_number_suited((U8*)number, (U8*)(MessagesBlackListNum[i])))
		{
			mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_is_black_number return TRUE");
			return MMI_TRUE;
		}
	}
	
	return MMI_FALSE;
	
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_msg_set_highlight_handler
* DESCRIPTION
*   Set message highlight handler, for masking menu item
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_highlight_handler (void)
{
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(EMS_ED_OPT_DONE + i, mmi_msg_highlight_done) ;
		SetHiliteHandler (MESSAGES_MENU_MSG_SETTINGS_MENUID + i, mmi_msg_highlight_msg_settings);
		SetHiliteHandler (MESSAGES_MENU_VMAIL_MENUID + i, HighlightVoiceMailHandler);
	}
#ifdef __MMI_MESSAGES_MULTI_APP__
	SetHiliteHandler (MESSAGES_MENU_SMS_MENUID, HighlightSMSHandler);
#endif
#ifdef MMS_SUPPORT
	SetHiliteHandler (MESSAGES_MENU_MMS_MENUID, HighlightMMSHandler);
#endif // #ifdef MMS_SUPPORT
#ifdef __MMI_MESSAGES_CHAT__
	SetHiliteHandler (MENU_CHATAPP_ID, HighlightChatMenu);
#endif
#ifdef __MMI_EMAIL__
	SetHiliteHandler (MENU_ID_EMAIL_MAIN,	mmi_email_highlight_main_menu);
#endif
	SetHiliteHandler (MESSAGES_MENU_WMESSAGE_MENU_ID, HighlightWMessageHandler);
	SetHiliteHandler (MESSAGES_MENU_INBOX_MENUID, HighlightInboxHandler);
	SetHintHandler (MESSAGES_MENU_INBOX_MENUID, HintInboxHandler);
	SetHiliteHandler (MESSAGES_MENU_OUTBOX_MENUID, HighlightOutboxHandler);
	SetHintHandler (MESSAGES_MENU_OUTBOX_MENUID, HintOutboxHandler);
#ifdef __MMI_MESSAGES_DRAFT_BOX__
	SetHiliteHandler (MESSAGES_MENU_DRAFTBOX_MENUID, HighlightDraftboxHandler);
	SetHintHandler (MESSAGES_MENU_DRAFTBOX_MENUID, HintDraftboxHandler);
#endif
#if defined(__MMI_MESSAGES_TEMPLATE__)
	SetHiliteHandler (MESSAGES_MENU_TEMPLATES_MENUID, HighlightTemplatesHandler);
#endif // defined(__MMI_MESSAGES_TEMPLATE__)
	SetHiliteHandler (MESSAGES_MENU_CB_MENUID, HighlightCBHandler);
	#ifdef __MMI_MESSAGES_DELETE_ALL_MENU__
	SetHiliteHandler (MESSAGES_MENU_DELETE_MENUID, HighlightSMSDeleteHandler);
	SetHiliteHandler (MESSAGES_MENU_DELETE_INBOX_MENUID, mmi_msg_highlight_delete_all_inbox);
	SetHiliteHandler (MESSAGES_MENU_DELETE_OUTBOX_MENUID, mmi_msg_highlight_delete_all_outbox);
	#ifdef __MMI_MESSAGES_DRAFT_BOX__
	SetHiliteHandler (MESSAGES_MENU_DELETE_DRAFTBOX_MENUID, mmi_msg_highlight_delete_all_draftbox);
	#endif
	SetHiliteHandler (MESSAGES_MENU_DELETE_ALL_MENUID, mmi_msg_highlight_delete_all);
	#endif
	SetHiliteHandler (SEND_OPT_SEND_ONLY_MENUID, mmi_msg_highlight_send_only);
	#ifdef __MMI_SEND_BY_SEARCH_NAME__
	SetHiliteHandler (SEND_OPT_SEND_BY_NUMBER, mmi_msg_highlight_send_only_number);
	SetHiliteHandler (SEND_OPT_SEND_BY_NAME, mmi_msg_highlight_send_only_name);
	#endif
	SetHiliteHandler (SEND_OPT_SAVE_AND_SEND_MENUID, mmi_msg_highlight_send_and_save);
	SetHiliteHandler (SEND_OPT_SAVE_MENUID, mmi_msg_highlight_save);
#if defined(__MMI_MESSAGES_SEND_TO_MANY__)
	SetHiliteHandler (SEND_OPT_SEND_TO_MANY_MENUID, mmi_msg_highlight_send_to_many);
#endif // defined(__MMI_MESSAGES_SEND_TO_MANY__)
#if defined(__MMI_MESSAGES_SEND_BY_GROUP__)
	SetHiliteHandler (SEND_OPT_SEND_BY_GROUP_MENUID, mmi_msg_highlight_send_by_group);
#endif // defined(__MMI_MESSAGES_SEND_BY_GROUP__)

#if defined(__MMI_MESSAGES_SEND_TO_MANY__)
	SetHiliteHandler (SEND_OPT_SEND_TO_MANY_BY_LIST, mmi_msg_highlight_send_to_many_list);	
	SetHiliteHandler (SEND_OPT_SEND_TO_MANY_NOW, mmi_msg_highlight_send_to_many_now);		
	SetHiliteHandler (SEND_OPT_SEND_TO_MANY_BY_LIST_EDIT, mmi_msg_highlight_send_to_many_list_edit);
	SetHiliteHandler (SEND_OPT_SEND_TO_MANY_BY_LIST_ERASE, mmi_msg_highlight_send_to_many_list_erase);	
#endif // defined(__MMI_MESSAGES_SEND_TO_MANY__)

#ifdef __MMI_MESSAGES_CHAT__
	SetHiliteHandler (SMS_INBOX_OPT_CHAT_MENUID, HilightChatInboxMenu);
#endif
#if defined(__MMI_MYPICTURE__)	
	SetHiliteHandler(	MENU_ID_MY_PICTURE,		highlight_mypic_handler );
	SetHiliteHandler(  MENU_ID_MYPIC_INSERT_TEXT_DONE,highlight_mypic_insert_text_done);
	SetHiliteHandler(	MENU_ID_MYPIC_DONE,	highlight_mypic_done_handler );
#endif
#if defined(__MMI_MYPICTURE__) || defined(__MMI_SMART_MESSAGE_MT__)
	SetHiliteHandler(  MENU_ID_MYPIC_INSERT_TEXT_DONE,highlight_mypic_insert_text_done);
	SetHiliteHandler(	MENU_ID_MYPIC_ADDTEXT,		highlight_mypic_addtext_handler );
	//SetHiliteHandler(	MENU_ID_MYPIC_SEND_ONLY,	highlight_mypic_send_handler);
	SetHiliteHandler(	MENU_ID_MYPIC_SEND_ONLY,	mmi_msg_highlight_send_only);
#if defined(__MMI_MESSAGES_SEND_TO_MANY__)
	SetHiliteHandler(	MENU_ID_MYPIC_SEND_TO_MANY,	mmi_msg_highlight_send_to_many );
#endif
#if defined(__MMI_MESSAGES_SEND_BY_GROUP__)
	SetHiliteHandler(	MENU_ID_MYPIC_SEND_BY_GROUP, mmi_msg_highlight_send_by_group);
#endif
	SetHiliteHandler(  MENU_ID_MYPIC_INSERT_TEXT_INPUT_METHOD,mmi_msg_highlight_input_method_generic);
	SetHiliteHandler(  MENU_ID_PIC_MSG_EDIT_TEXT_DONE,highlight_pic_msg_edit_text_done);
	SetHiliteHandler(  MENU_ID_PIC_MSG_EDIT_TEXT_INPUT_METHOD,mmi_msg_highlight_input_method_generic);

#endif 
	#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_SMS_Init_Show_Style();
	#endif

	SetHiliteHandler (SMS_INBOX_OPT_REPLY_MENUID,	mmi_msg_highlight_reply);
	SetHiliteHandler (SMS_INBOX_OPT_EDIT_MENUID, mmi_msg_highlight_edit);
	SetHiliteHandler (SMS_INBOX_OPT_FORWARD_MENUID, mmi_msg_highlight_edit);
#ifdef __MMI_MESSAGES_COPY__
	SetHiliteHandler (SMS_INBOX_OPT_COPY_MENUID,		mmi_msg_highlight_copy);
	SetHiliteHandler (SMS_INBOX_OPT_COPY_ALL_MENUID,		mmi_msg_highlight_copy_all_inbox);
	SetHiliteHandler (SMS_INBOX_OPT_COPY_ALL_FROM_SIM_MENUID,		mmi_msg_highlight_copy_all_from_sim_inbox);
	SetHiliteHandler (SMS_INBOX_OPT_COPY_ALL_FROM_PHONE_MENUID,		mmi_msg_highlight_copy_all_from_phone_inbox);
	SetHiliteHandler (SMS_INBOX_OPT_MOVE_MENUID,		mmi_msg_highlight_move);
	SetHiliteHandler (SMS_INBOX_OPT_MOVE_ALL_MENUID,		mmi_msg_highlight_move_all_inbox);
	SetHiliteHandler (SMS_INBOX_OPT_MOVE_ALL_FROM_SIM_MENUID,		mmi_msg_highlight_move_all_from_sim_inbox);
	SetHiliteHandler (SMS_INBOX_OPT_MOVE_ALL_FROM_PHONE_MENUID,		mmi_msg_highlight_move_all_from_phone_inbox);
#endif  // __MMI_MESSAGES_COPY_
	SetHiliteHandler (SMS_INBOX_OPT_DELETE_MENUID,		mmi_msg_highlight_delete);
    SetHiliteHandler (SMS_INBOX_OPT_DELETE_ALL_MENUID, mmi_msg_highlight_delete_all_inbox);
	SetHiliteHandler (SMS_INBOX_OPT_USE_NUMBER_MENUID, mmi_msg_highlight_use_number);
#if defined(__MMI_MESSAGES_USE_URL__)
	SetHiliteHandler (MENU_ID_SMS_INBOX_OPT_USE_URL, mmi_msg_highlight_use_url);
	SetHiliteHandler (MENU_ID_SMS_OUTBOX_OPT_USE_URL, mmi_msg_highlight_use_url);
#endif /* __MMI_MESSAGES_USE_URL__ */

#if defined(__MMI_MESSAGES_EMS__)
	SetHiliteHandler (SMS_INBOX_OPT_SAVE_OBJECT_MENUID, mmi_msg_highlight_save_object);
#endif // defined(__MMI_MESSAGES_EMS__)
	
	SetHiliteHandler (SMS_OUTBOX_OPT_SEND_MENUID, mmi_msg_highlight_send_outbox);
	SetHiliteHandler (SMS_OUTBOX_OPT_EDIT_MENUID, mmi_msg_highlight_edit);
#ifdef __MMI_MESSAGES_COPY__
	SetHiliteHandler (SMS_OUTBOX_OPT_COPY_MENUID,		mmi_msg_highlight_copy);
	SetHiliteHandler (SMS_OUTBOX_OPT_COPY_ALL_MENUID,		mmi_msg_highlight_copy_all_outbox);
	SetHiliteHandler (SMS_OUTBOX_OPT_COPY_ALL_FROM_SIM_MENUID,		mmi_msg_highlight_copy_all_from_sim_outbox);
	SetHiliteHandler (SMS_OUTBOX_OPT_COPY_ALL_FROM_PHONE_MENUID,		mmi_msg_highlight_copy_all_from_phone_outbox);  
	SetHiliteHandler (SMS_OUTBOX_OPT_MOVE_MENUID,		mmi_msg_highlight_move);
	SetHiliteHandler (SMS_OUTBOX_OPT_MOVE_ALL_MENUID,		mmi_msg_highlight_move_all_outbox);
	SetHiliteHandler (SMS_OUTBOX_OPT_MOVE_ALL_FROM_SIM_MENUID,		mmi_msg_highlight_move_all_from_sim_outbox);
	SetHiliteHandler (SMS_OUTBOX_OPT_MOVE_ALL_FROM_PHONE_MENUID,		mmi_msg_highlight_move_all_from_phone_outbox);  
#endif  // __MMI_MESSAGES_COPY_
	SetHiliteHandler (SMS_OUTBOX_OPT_DELETE_MENUID, 	mmi_msg_highlight_delete);
	SetHiliteHandler (SMS_OUTBOX_OPT_DELETE_ALL_MENUID, mmi_msg_highlight_delete_all_outbox);
	SetHiliteHandler (SMS_OUTBOX_OPT_USE_NUMBERS_MENUID, mmi_msg_highlight_use_number);
	SetHiliteHandler (SMS_USE_NUM_OPT_DIAL, mmi_msg_highlight_dial_use_number);
	SetHiliteHandler (SMS_USE_NUM_OPT_SAVE, mmi_msg_highlight_save_use_number);

#if defined(__MMI_MESSAGES_TEMPLATE__)
	SetHiliteHandler (TEMPLATES_OPTIONS_ERASE_MENUENTRY, mmi_msg_highlight_erase_template);
	SetHiliteHandler (TEMPLATES_OPTIONS_EDIT_MENUENTRY,	mmi_msg_highlight_edit_template);
	SetHiliteHandler(TEMPLATES_OPTIONS_WRITE_MESSAGE_MENUENTRY, mmi_msg_highlight_template_write_msg);
	
	SetHiliteHandler (SMS_ED_TEMPL_OPT_DONE_MENU, mmi_msg_highlight_save_template);
	SetHiliteHandler (SMS_ED_TEMPL_OPT_INPUT_METHODS_MENU, mmi_msg_highlight_input_method_generic);
#endif // defined(__MMI_MESSAGES_TEMPLATE__)
	
#if defined(__MMI_MESSAGES_EMS__)
	// For NOn-Inline Edit screen
	SetHiliteHandler (SMS_INBOX_OPT_SAVE_OPTION_DONE, HighlightEmsSaveObjectDone);
	SetHiliteHandler (SMS_INBOX_OPT_SAVE_OPTION_INPUT_METHOD, mmi_msg_highlight_input_method_generic);
#endif // defined(__MMI_MESSAGES_EMS__)

	SetHiliteHandler (SETUP_MENU_PROFILE_SETTING_MENUID, mmi_msg_highlight_profile_settings);
	SetHiliteHandler (SETUP_MENU_COMMONSETTING_MENUID, mmi_msg_highlight_common_settings);
#if defined(__MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__)
	SetHiliteHandler (MESSAGES_MENU_MEMSTATUS_MENUID, mmi_msg_highlight_memory_status);
	SetHiliteHandler (SETUP_MENU_PREFEREDSTORAGE_MENUID, mmi_msg_highlight_preferred_storage);
	SetHintHandler (SETUP_MENU_PREFEREDSTORAGE_MENUID, mmi_msg_hint_preferred_storage);
#endif // defined(__MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__)
#ifdef __MMI_GPRS_FEATURES__
	SetHiliteHandler (SETUP_MENU_PREFEREDBEARER_MENUID, mmi_msg_highlight_bearer_settings);
	SetHintHandler (SETUP_MENU_PREFEREDBEARER_MENUID, mmi_msg_hint_bearer_settings);
#endif
	
	SetHiliteHandler (MENU_ITEM_VM_1_EDIT, mmi_msg_highlight_voice_mail_server_edit);
	SetHiliteHandler (MENU_ITEM_VM_3_CONNECT_TO_VOICE, mmi_msg_highlight_voice_mail_server_dial);

	// Set EMS Hilight Hanglers
#if defined(__MMI_SMART_MESSAGE_MO__)&&!defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
#else
	SetHiliteHandler (EMS_ED_OPT_INPUT_METHOD, mmi_msg_highlight_input_method) ;
#endif
	
#if defined(__MMI_MESSAGES_TEMPLATE__)
	SetHiliteHandler (EMS_ED_OPT_USE_TEMPLATES, mmi_msg_highlight_insert_template) ;
#endif // defined(__MMI_MESSAGES_TEMPLATE__)
#if defined(__MMI_MESSAGES_INSERT_FROM_PHB__)
	SetHiliteHandler(EMS_ED_OPT_INS_PHB_NUM, mmi_msg_highlight_insert_number);
	SetHiliteHandler(EMS_ED_OPT_INS_PHB_NAM, mmi_msg_higlight_insert_name);
#endif

#if defined(__MMI_MESSAGES_EMS__)
	SetHiliteHandler (EMS_ED_OPT_INS_OBJ, HighlightEditOptionInsertObject) ;
#endif // defined(__MMI_MESSAGES_EMS__)

#if defined(__MMI_MESSAGES_EMS__)
	SetHiliteHandler (EMS_INS_OBJ_PICTURE, HighlightInsObjPicture) ;
	SetHiliteHandler (EMS_INS_OBJ_MY_PICTURE, HighlightInsObjMyPicture) ;
	SetHiliteHandler (EMS_INS_OBJ_PRED_ANIMATION, HighlightInsObjPredefinedAnimation) ;
	SetHiliteHandler (EMS_INS_OBJ_MY_ANIMATION, HighlightInsObjMyAnimation) ;
#if defined(__MMI_IMELODY_SUPPORT__)	
	SetHiliteHandler (EMS_INS_OBJ_MELODY, HighlightInsObjMelody) ;
	SetHiliteHandler (EMS_INS_OBJ_MY_MELODY, HighlightInsObjMyMelody) ;
#endif // defined(__MMI_IMELODY_SUPPORT__)	
	SetHiliteHandler (EMS_INS_OBJ_PRED_SOUND, HighlightInsObjPredefinedSound) ;
#ifndef __MMI_FILE_MANAGER__
     // wangbei del start 20060614
     #if 0
	SetHiliteHandler (EMS_OBJECT_OPTION_VIEW_MENUENTRY, HighlightEmsObjectViewHandler);
	SetHiliteHandler (EMS_OBJECT_OPTION_SEND_BY_EMS_MENUENTRY, HighlightSendByEMSHandler);
	SetHiliteHandler (EMS_OBJECT_OPTION_DELETE_MENUENTRY, HighlightEmsObjectDeleteHandler);
	#endif
	// wangbei del end
#endif
	SetHiliteHandler (MENU_EMS_FORMATING_TEXT_ID,			HighlightEmsFormattingTextHandler);
	SetHiliteHandler (MENU_EMS_TEXT_SIZE_ID,					HighlightEmsTextSizeHandler);
	SetHiliteHandler (MENU_EMS_TEXT_STYLE_ID,					HighlightEmsTextStyleHandler);
	SetHiliteHandler (MENU_EMS_ALIGNMENT_ID,					HighlightEmsAlignmentHandler);
	SetHiliteHandler (MENU_EMS_NEW_PARAGRAPH_ID,				HighlightEmsNewParagraphHandler);
	SetHiliteHandler (MENU_EMS_TEXT_SIZE_SMALL_ID,			HighlightEmsTextSizeSmallHandler);
	SetHiliteHandler (MENU_EMS_TEXT_SIZE_MEDIUM_ID,			HighlightEmsTextSizeMediumHandler);
	SetHiliteHandler (MENU_EMS_TEXT_SIZE_LARGE_ID,			HighlightEmsTextSizeLargeHandler);
	SetHiliteHandler (MENU_EMS_TEXT_STYLE_BOLD_ID,			HighlightEmsTextStyleBoldHandler);
	SetHiliteHandler (MENU_EMS_TEXT_STYLE_ITALICS_ID,		HighlightEmsTextStyleItalicsHandler);
	SetHiliteHandler (MENU_EMS_TEXT_STYLE_UNDERLINE_ID,	HighlightEmsTextStyleUnderlineHandler);
	SetHiliteHandler (MENU_EMS_TEXT_STYLE_STRIKETHROUGH_ID,	HighlightEmsTextStyleStrikethroughHandler);
	SetHiliteHandler (MENU_EMS_ALIGNMENT_AUTOMATIC_ID,		HighlightEmsAlignmentAutomaticHandler);
	SetHiliteHandler (MENU_EMS_ALIGNMENT_LEFT_ID,			HighlightEmsAlignmentLeftHandler);
	SetHiliteHandler (MENU_EMS_ALIGNMENT_CENTER_ID,			HighlightEmsAlignmentCenterHandler);
	SetHiliteHandler (MENU_EMS_ALIGNMENT_RIGHT_ID,			HighlightEmsAlignmentRightHandler);
	SetHiliteHandler (MENU_EMS_NEW_PARAGRAPH_AUTOMATIC_ID,	HighlightEmsNewParagraphAutomaticHandler);
	SetHiliteHandler (MENU_EMS_NEW_PARAGRAPH_LEFT_ID,		HighlightEmsNewParagraphLeftHandler);
	SetHiliteHandler (MENU_EMS_NEW_PARAGRAPH_CENTER_ID,	HighlightEmsNewParagraphCenterHandler);
	SetHiliteHandler (MENU_EMS_NEW_PARAGRAPH_RIGHT_ID,		HighlightEmsNewParagraphRightHandler);
#endif // defined(__MMI_MESSAGES_EMS__)
#ifdef __MMI_MESSAGES_USE_URL__
	SetHiliteHandler (MENU_ID_SMS_USE_URL_CONNECT,		HighlightUseURLOptConnectHandler);
#endif	/* __MMI_MESSAGES_USE_URL__ */

#if defined(__MMI_MESSAGES_BLACK_LIST__)
	InitMessagesBlackList();
#endif
}

#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef __MMI_MESSAGES_COPY__
/* under construction !*/
#endif	
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined(__MMI_MESSAGES_USE_URL__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef __MMI_MESSAGES_COPY__
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#ifdef __MMI_MESSAGES_DRAFT_BOX__
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif	
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined (MMI_ON_HARDWARE_P)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
    #if defined(__MMI_FILE_MANAGER__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#endif
		#endif
/* under construction !*/
		#ifdef MMI_ON_WIN32
/* under construction !*/
/* under construction !*/
		#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if 0	 
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif		 
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#endif
/* under construction !*/
		#if defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef __MMI_MESSAGES_COPY__
/* under construction !*/
#endif		
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined(__MMI_CLI_ICON__) || defined(__MMI_OPERATOR_LOGO__)	
/* under construction !*/
/* under construction !*/
#endif	/* __MMI_CLI_ICON__ || __MMI_OPERATOR_LOGO__ */
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
			#ifdef __MMI_SMART_MESSAGE_MO__
/* under construction !*/
			#else
/* under construction !*/
			#endif	/* __MMI_SMART_MESSAGE_MO__ */
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined(__MMI_MESSAGES_USE_URL__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef __MMI_MESSAGES_COPY__
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif  // __MMI_MESSAGES_COPY_
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef MMI_ON_HARDWARE_P
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#if defined(__MMI_FILE_MANAGER__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#endif
		#endif
/* under construction !*/
		#ifdef MMI_ON_WIN32
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined(__MMI_SMART_MESSAGE_MT__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /*__MMI_SMART_MESSAGE_MT__ */
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* (__MMI_SMART_MESSAGE_MO__) || (__MMI_SMART_MESSAGE_MT__) */

#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined (MMI_ON_WIN32)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /*MMI_ON_WIN32*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined (MMI_ON_HARDWARE_P)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#if defined(__MMI_FILE_MANAGER__)
	#if defined(MMI_ON_HARDWARE_P)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#endif	/* (MMI_ON_HARDWARE_P) */
	#endif	/* ( __MMI_FILE_MANAGER__)*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif		/* MMI_ON_HARDWARE_P */
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

#if defined(__MMI_MESSAGES_USE_URL__)
static U8 mmi_msg_check_gsm_char(PU16 ch);
static PS16 mmi_msg_to_lower_case(PS8 ptr);
static U8 mmi_msg_get_url_list_from_msgtxt(S8 *MsgTxt);
static S8 *pfnUnicodeStristr(S8 *text, S8 *pattern);
static void mmi_msg_free_url_list(void);
#ifdef MMI_ON_HARDWARE_P
extern S32 widget_ucs2_length(const PU8 s);
extern S32 widget_ucs2_to_utf8_length_in_bytes(const PU8 raw);
extern S32 widget_ucs2_to_utf8_string(PU8 dest,S32 dest_size,PU8 src);
#endif

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_use_url
* DESCRIPTION
*   Highlight use URL handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void mmi_msg_highlight_use_url(void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_use_url, GoBackHistory);
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_use_url
* DESCRIPTION
*   Entry use URL list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void mmi_msg_entry_use_url(void)
{
	U8*	guiBuffer=NULL;
	EMSData * pEMS;
	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_use_url");

	pEMS = GetEMSDataForView (0, 0);

#if defined (__MMI_SMART_MESSAGE_MT__) || (defined (__MMI_SMART_MESSAGE_MO__) && !defined (__MMI_MESSAGES_EMS__))
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else
	g_msg_cntx.URLCount = mmi_msg_get_url_list_from_msgtxt((S8*)pEMS->textBuffer);
#endif
	
	mmi_trace(g_sw_SMS, "MMI_SMS: URL List count : %d", g_msg_cntx.URLCount);
	
	if (g_msg_cntx.URLCount > 0)
	{
		EntryNewScreen(SCR_ID_MSG_USE_URL_LIST, mmi_msg_exit_generic, mmi_msg_entry_use_url, NULL);	
	
		guiBuffer = GetCurrGuiBuffer(SCR_ID_MSG_USE_URL_LIST);
		RegisterHighlightHandler(GetHiliteIndex);
		SetMessagesCurrScrnID(SCR_ID_MSG_USE_URL_LIST);
			
		ShowCategory53Screen (STR_USE_URL_LIST_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION, 
								  STR_GLOBAL_OPTIONS, IMG_SMS_COMMON_NOIMAGE,
								  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								  g_msg_cntx.URLCount, (U8**)g_msg_cntx.URLlist, (U16 *)gIndexIconsImageList,
								  NULL , 0, 0, guiBuffer );
		SetLeftSoftkeyFunction(mmi_msg_entry_use_url_option, KEY_EVENT_UP);
		SetKeyHandler(mmi_msg_entry_use_url_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(mmi_msg_entry_open_url, KEY_SEND, KEY_EVENT_UP);
		SetRightSoftkeyFunction(mmi_msg_go_back_use_url_list, KEY_EVENT_UP);
		SetKeyHandler(mmi_msg_go_back_use_url_list, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(mmi_msg_end_key_from_use_url, KEY_END, KEY_EVENT_DOWN);
	}
	else
		DisplayPopup ((PU8)GetString (STR_NOURL_NOTIFICATION), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_check_gsm_char
* DESCRIPTION
*  Check if it is a gsm character
*
* PARAMETERS
*  a	IN	char
* RETURNS
*  TRUE if it is a gsm character; FALSE otherwise
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static U8 mmi_msg_check_gsm_char(PU16 ch)
{
	U16 ascii_min = 0x0000;
	U16 ascii_max = 0x007f;
	
	if (*ch>=ascii_min && *ch<=ascii_max)
		return 1;
	else
		return 0;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_to_lower_case
* DESCRIPTION
*  Convert ascii upper case character to unicode lower case
*
* PARAMETERS
*  a	IN	ptr	a pointer points to an ascii character
* RETURNS
*  a pointer points to an unicode character
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static PS16 mmi_msg_to_lower_case(PS8 ptr)
{
	S16 two_byte_A = 0x0041;
	S16 two_byte_Z = 0x005A;
	S16 unicode_lower;
	
	if (*((S16*)ptr) >= two_byte_A && *((S16*)ptr) <= two_byte_Z)
	{
		unicode_lower = (S16)('a' + (*ptr) - 'A');
	}
	else
	{
		unicode_lower = *((S16*)(ptr));
	}
	return &unicode_lower;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_url_list_from_msgtxt
* DESCRIPTION
*   Extract all URL from message text
*
* PARAMETERS
*  a	IN	Message text
* RETURNS
*  Total URL count in message text
* GLOBALS AFFECTED
*   g_msg_cntx
*****************************************************************************/
static U8 mmi_msg_get_url_list_from_msgtxt(S8 *MsgTxt)
{
	U8 i;
	S8 Pattern[20];
	PS8 ptr = NULL;
	PS8 min_ptr = NULL;
	PS8 start_ptr = NULL;
	U16 URLlen=0;
	U8 URLCount=0;

	while(pfnUnicodeStrcmp(MsgTxt, "\0"))
	{
		for (i=0; i<MAX_NUM_URL_PATTERN; i++)
		{
			AnsiiToUnicodeString(Pattern, urlPattern[i]);
			/* search the pattern in the MsgTxt */
			ptr = pfnUnicodeStristr(MsgTxt, Pattern);
			/* select the minimum one, i.e. http://www.CSD.com "http://" will be selected instead of "www." */
			if (ptr != NULL && (min_ptr == NULL || ptr < min_ptr))
			{
				min_ptr = ptr;
				ptr = NULL;
			}
		}
		
		if (URLCount < MAX_USE_NUMBERS && min_ptr != NULL)
		{
			start_ptr = min_ptr;
			/* move MsgTxt pointer from last url terminator to this url starter */
			MsgTxt += (min_ptr - MsgTxt);

			if(g_msg_cntx.URLlist[URLCount])
				OslMfree(g_msg_cntx.URLlist[URLCount]);
			g_msg_cntx.URLlist[URLCount] = (S8*)OslMalloc(MAX_URL_LEN);
			memset(g_msg_cntx.URLlist[URLCount], 0x00, MAX_URL_LEN);

			URLlen = 0;
			/* get one url until 0x20, 0x0d, 0x0a, 0x00 */
			while (pfnUnicodeStrncmp(start_ptr, " ", 1) && pfnUnicodeStrncmp(start_ptr, "\r", 1) && pfnUnicodeStrncmp(start_ptr, "\n", 1) && pfnUnicodeStrncmp(start_ptr, "\0", 1))
			{
				/* must reserve two bytes for null terminator */
				if (URLlen < MAX_URL_LEN-2 && mmi_msg_check_gsm_char((PU16)start_ptr))
				{
					/* skip extension character 0x1b */
					if (*(PU16)start_ptr != 0x001b)
					{
						pfnUnicodeStrncpy(g_msg_cntx.URLlist[URLCount]+URLlen, start_ptr, 1);
						URLlen += 2;
					}
					start_ptr += 2;
				}
				 
				else
					break;
			}
			URLCount ++;
			
			MsgTxt = start_ptr;
			start_ptr = NULL;
			min_ptr = NULL;
		}
		 
		else
			break;
	}
	return URLCount;
}

/*****************************************************************************
* FUNCTION
*  pfnUnicodeStristr
* DESCRIPTION
*  finds pattern in given text
*
* PARAMETERS
*	a	IN	Text
*	b	IN	Pattern
* RETURNS
*	a	SUCCESS		Starting address of pattern in text
*	B	FAIL		NULL
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static S8 *pfnUnicodeStristr(S8 *text, S8 *pattern)
{
	S8 *ptr = NULL;
	S8 *ret_addr = NULL;
	BOOL pattern_found	= FALSE;
	
	ptr = pattern;
	while(pfnUnicodeStrcmp(text, "\0"))
	{
		ret_addr = text;
		while (!pfnUnicodeStrncmp((S8*)mmi_msg_to_lower_case(text), ptr, 1))
		{
			ptr  += 2;
			text += 2;
			
			if(!pfnUnicodeStrncmp(text, "\0", 1)) break;
		}	/* while...*/

		if(!pfnUnicodeStrncmp(ptr, "\0", 1)) { pattern_found = TRUE; break; }

		if(ptr != pattern)
			ptr = pattern;
		else	
			text += 2;
	}/* while...*/

	if(!pattern_found)
	ret_addr =NULL;

	return (S8*)ret_addr;
}	/* func()....*/

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_use_url_option
* DESCRIPTION
*   Entry Use URL Options
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void mmi_msg_entry_use_url_option(void)
{

	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];

	EntryNewScreen(SCR_ID_MSG_USE_URL_OPTION, mmi_msg_exit_generic, mmi_msg_entry_use_url_option, NULL);	
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_msg_entry_use_url_option");
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_USE_URL_OPTION);
	
	numItems = GetNumOfChild (MENU_ID_SMS_USE_URL_OPTIONS);	
	GetSequenceStringIds(MENU_ID_SMS_USE_URL_OPTIONS, nStrItemList);	
	SetParentHandler (MENU_ID_SMS_USE_URL_OPTIONS);

	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	
	ShowCategory52Screen(STR_URL_OPT_CAPTION,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}


/*****************************************************************************
* FUNCTION
*  HighlightUseURLOptConnectHandler
* DESCRIPTION
*   Highlight Use URL Connect handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void HighlightUseURLOptConnectHandler(void)
{

	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
									STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
									mmi_msg_entry_open_url, GoBackHistory);	
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_open_url
* DESCRIPTION
*   Entry open an URL in WAP BROWSERS
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void mmi_msg_entry_open_url(void)
{
#ifdef MMI_ON_HARDWARE_P
	S32 utf8URLsize=0;
	PU8 utf8URL=NULL;
	
	if (g_msg_cntx.URLlist[g_msg_cntx.currHiliteIndex] != NULL && widget_ucs2_length((PU8)g_msg_cntx.URLlist[g_msg_cntx.currHiliteIndex])>0)
    	{
       	utf8URLsize=widget_ucs2_to_utf8_length_in_bytes((PU8)g_msg_cntx.URLlist[g_msg_cntx.currHiliteIndex])+1;
       	utf8URL=OslMalloc(utf8URLsize);
       	widget_ucs2_to_utf8_string((PU8)utf8URL,utf8URLsize,(PU8)g_msg_cntx.URLlist[g_msg_cntx.currHiliteIndex]);
    	}
	if (!GetTotalCallCount() && !IsScreenPresent(WAP_SCREEN_WIDGET))
	{
		ClearAllKeyHandler();
		mmi_frm_sms_startup_wap_browser_req((void*)utf8URL);
	}
	else
		DisplayPopup((PU8)GetString(STR_CONNECT_FAILED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	if (utf8URL!=NULL)
	{
		OslMfree(utf8URL);
		utf8URL=NULL;
	}
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_end_key_from_use_url
* DESCRIPTION
*    Free URL list and  return to Idle Screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void mmi_msg_end_key_from_use_url(void)
{
	mmi_msg_free_url_list();
	DisplayIdleScreen();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_use_url_list
* DESCRIPTION
*    Free URL list and  go back
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/

extern void mmi_msg_go_back_use_url_list(void)
{
	mmi_msg_free_url_list();
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_free_url_list
* DESCRIPTION
*   Free URl list and reset URL count
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   a		g_msg_cntx		global message context
*****************************************************************************/
static void mmi_msg_free_url_list(void)
{
	U16 count;
	
	for(count=0 ;count<g_msg_cntx.URLCount; ++count)
	{
		if(g_msg_cntx.URLlist[count])
		OslMfree(g_msg_cntx.URLlist[count]);
	}
	memset(g_msg_cntx.URLlist, 0x00, sizeof(g_msg_cntx.URLlist));
	g_msg_cntx.URLCount = 0;

}
#endif /* __MMI_MESSAGES_USE_URL__ */
/////////////////////////////////////////////////////////////////////////////////////
#ifdef __MMI_MULTI_SIM__
/*****************************************************************************
 * FUNCTION
 *  mmi_msg_set_msg_sim_id
 * DESCRIPTION
 *  Set current message sim id
 * PARAMETERS
 *         
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_set_msg_sim_id(U8 nSimID)
{
	g_msg_cntx.newMsgSimID= nSimID;
}
#endif
#ifdef __MMI_MULTI_SIM__
/*****************************************************************************
 * FUNCTION
 *  mmi_msg_set_msg_need_select_sim_when_send
 * DESCRIPTION
 *  Set if need select sim when send
 * PARAMETERS
 *         
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_set_msg_need_select_sim_when_send(BOOL isNeed)
{
	g_msg_cntx.needSelectSIM = isNeed;
}
#endif
#endif // #ifdef __MOD_SMSAL__


