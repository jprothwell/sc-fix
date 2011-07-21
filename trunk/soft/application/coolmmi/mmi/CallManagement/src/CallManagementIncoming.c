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
 *  Modification Notice:
 *  --------------------------
 *  This software is modified by COOLSAND Inc. and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of COOLSAND Inc. (C) 2001
 *
 *******************************************************************************/

 /*******************************************************************************
 * Filename:
 * ---------
 *   CallManagementIncoming.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  .......
 *
 * Author:
 * -------
 * -------
 *
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/CallManagement/src/CallManagementIncoming.c-arc  $
 * 
 *    Rev 1.5   Mar 16 2007 10:12:18   lin
 * 1）修改mdi_audio_check_states_and_stop(); 函数
 * 2）添加   mdi_mp4_check_states_and_stop();
 * 
 *    Rev 1.4   Feb 02 2007 20:47:42   lin
 * 添加了几行跟踪调试信息
 * 
 *    Rev 1.3   Nov 30 2006 10:56:58   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:37:26   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:23:44   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:32:52   zhangxb
 * Initial revision.
 * 
 *    Rev 1.2   Sep 19 2006 19:10:50   zhoumn
 * No change.
 * 
 *    Rev 1.1   Aug 28 2006 17:38:42   zhangxb
 * 合并了PSI和Simon修改的一些代码.
 * 
 *    Rev 1.0   Jul 06 2006 17:42:24   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Jul 04 2006 22:56:58   zhangx
 *
 *
 *******************************************************************************/

/**************************************************************

	FILENAME	: CallManagementIncoming.c

  	PURPOSE		: Call Management application

 
				  Skipping nodes it to be decided

	AUTHOR		: Ashish

	DATE		: Nov 8,02

**************************************************************/
#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "sublcdhistorygprot.h"
#include "globaldefs.h"
#include "globalstruct.h"
#include "globalscrenum.h"
#include "keybrd.h"
#include "timerevents.h"
#include "wgui.h"
#include "wgui_categories_popup.h"
#include "wgui_categories_cm.h"
#include "wgui_categories_util.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_multimedia.h"

#include "gdi_include.h"			/* for gdi's color define */

#include "profilegprots.h"
#include "bootup.h"
#include "poweronchargerprot.h"
#include "messagesexdcl.h"
#include "smspshandler.h"
#include "messagesresourcedata.h"
#include "smsapi.h"

#include "alarmframeworkprot.h"
#include "satdefs.h"
#include "satgprots.h"
#include "simdetectiongexdcl.h"
#include "gpioinc.h"
#include "phonebookdef.h"
#include "phonebookgprot.h"
#include "callsetup.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "audioinc.h"

#include "callmanagementiddef.h"
#include "callmanagementstruct.h"
#include "callstructuremanagementprot.h"
#include "cmerrortypes.h"
#include "cmerrorhandlersprot.h"
#include "historyhelperprot.h"
#include "redialhelpergprot.h"
#include "commonstubsprot.h"
#include "pscallhelperdef.h"
#include "pscallhelpergprot.h"
#include "callmanagementgprot.h"
#include "outgoingcallhelperprot.h"
#include "outgoingcallmanagementprot.h"
#include "messagesresourcedata.h"


#include "incomingcallmanagementprot.h"
#include "phonebookdef.h" /*for enum img_phb_default*/
#include "commonscreens.h" // for popup window timer.

#include "profilesdefs.h"//zhou siyou add

#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
#endif

#include "mmi_trace.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#include "mtpnp_ad_resdef.h"
#endif /* __MMI_MULTI_SIM__ */

#undef __NEWSIMULATOR
#ifndef MMI_ON_WIN32
#include "chip_id.h"
#endif
#ifdef __MMI_RUNNING_LED__
#include "led.h"
#include "gpioinc.h"
#endif
#include "nvram_user_defs.h"
#include "settingdefs.h"

//static CM_CALL_HANDLE gCallToEnd;// This is the call Selected for ending
static CM_CALL_HANDLE gMyCallHandle;
static S8 CallDeflectBuffer[MAX_NUMBER];
static U8 gDTMFConfirmBuf[MAX_DTMFBUF_DIGITS];
static U8 cashSign[8];
static U8 AocBuffer[10];
static pBOOL gAttempToReject = FALSE;
static pBOOL gAttempToAnswer = FALSE;
static pBOOL gAcceptIncoming=FALSE;// tells if there is a incoming call waiting to be accepted
//static pBOOL gHold2End=FALSE;// tells if there is a pending hold call 2 end
#ifdef __MMI_TTS_FEATURES__ 
extern nvram_tts_setting_struct g_tts_setting;
#endif

S8  gUSSDSendReqbuf[MAX_DIGITS_USSD * ENCODING_LENGTH + ENCODING_LENGTH];
/*static U16 gIdleScreenDigitsAnyKeyAnswer[MAX_CM_SCREEN_DIGITS_ANYKEY] =
{
													KEY_0,KEY_1,KEY_2,KEY_3,
													KEY_4,KEY_5,KEY_6,KEY_7,
													KEY_8,KEY_9,KEY_STAR,KEY_POUND,
													KEY_UP_ARROW,
													KEY_DOWN_ARROW,
													KEY_LEFT_ARROW,
													KEY_RIGHT_ARROW,
													KEY_CLEAR, KEY_ENTER, KEY_QUICK_ACS
												};*/

S16 CallDeflectDigitKeyCode=-1;
U16 gSSEvent=0;
BOOL isSpeakerMute = FALSE;
extern U8 isEarphonePlugged;

/*Jinzh Modify Start For 6424 Ver: TBM780  on 2007-8-28 17:36 */
extern U8 isEarphonePlugged_1;  
/*Jinzh Modify End  For 6424 Ver: TBM780  on 2007-8-28 17:36 */
extern U32 gSumACMValue;
extern S8  gUSSDbuf[];
extern S8 UnicodeDTMFPadCallBuffer[];
extern const U16 CMIdleScreenDigits[];
extern PU8 gdispnameForSubLCD;
extern void aud_speech_set_mode(kal_bool on);
extern void aud_speech_set_mode_req_hdlr(ilm_struct *ilm_ptr);
extern U8 GetSpeakerWorkStatus();
extern pBOOL GetOtherRejectedFlag(void);
extern void rda_bt_hfg_send_BldnFail_ind(void);
extern void show_call_cost_UI(PU8 symbol, PU8 value);

extern void EntryScrEmergencyCallDialing(void);


#ifdef WAP_SUPPORT
extern int BRAa_bearerCsdIsDisc(void);
#endif

#ifdef __MMI_CLAMSHELL__
extern BOOL IsClamClose(void);
#endif

#ifdef __MMI_MT_CALL_SPEAKER_MUTE__
BOOL GetSpeakerMuteFlag();
void SetSpeakerMuteFlag(BOOL bMute);
void ResumeSpeaker();
#endif

/**************************************************************

	FUNCTION NAME		: ReinitalizeIncomingCall()

  	PURPOSE				: To Re-Initialize the Incoming Call Management Application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReinitalizeIncomingCall (void)
{
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetProtocolEventHandlerExt(PsCBackIncomingCallAccepted, PRT_INCOMINGCALL_ACCEPT_SUCCESS, i);
		SetProtocolEventHandlerExt(PsCBackCallIncoming, PRT_INCOMINGCALL_EVENT, i);
		SetProtocolEventHandlerExt(PsCBackCallWait, PRT_INCOMING_CALL_WAIT, i);
		SetProtocolEventHandlerExt(PsCBackSpeechIndication, PRT_SPEECH_IND_EVENT, i);
		SetProtocolEventHandlerExt(PsCBackAocIndication, PRT_AOC_EVENT_IND, i);
		SetProtocolEventHandlerExt(PsCBackNotifySSInd, PRT_NOTIFY_SS_EVENT, i);
		SetProtocolEventHandlerExt(PsCBackCcbsActivate, PRT_OUTGOING_DISCONNECT_IND, i);
		SetProtocolEventHandlerExt(PsCBackCCBSInvoked, PRT_CCBS_NOTIFY_IND, i);
		SetProtocolEventHandlerExt(PsCBackConfirmDTMFString, PRT_DTMF_CONFIRM_IND, i);
		SetProtocolEventHandlerExt(PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE, i);

		SetProtocolEventHandlerExt(PsCBackUSSNInd, PRT_USSN_IND, i);
		SetProtocolEventHandlerExt(PsCBackUSSRInd, PRT_USSR_IND, i);
		SetProtocolEventHandlerExt(PsCBackUSSDInd, PRT_USSD_IND, i);

		SetProtocolEventHandlerExt(CALLSETClipRsp, PRT_MMI_SS_CLIP_INTERROGATE_IND, i);
		SetProtocolEventHandlerExt(CALLSETClirRsp, PRT_MMI_SS_CLIR_INTERROGATE_IND, i);
		SetProtocolEventHandlerExt(CALLSETColpRsp, PRT_MMI_SS_COLP_INTERROGATE_IND, i);
		SetProtocolEventHandlerExt(CALLSETColrRsp, PRT_MMI_SS_COLR_INTERROGATE_IND, i);
		SetProtocolEventHandlerExt(CALLSETCnapRsp, PRT_MMI_SS_CNAP_INTERROGATE_IND, i);
		SetProtocolEventHandlerExt(CALLSETCcbsQueryRsp, PRT_MMI_SS_CCBS_RSP, i);
		SetProtocolEventHandlerExt(CALLSETEmlppQueryRsp, PRT_MMI_SS_EMLPP_RSP, i);
		SetProtocolEventHandlerExt(CALLSETWaitingRsp, PRT_MMI_SS_CALL_WAIT_RSP, i);
		SetProtocolEventHandlerExt(CALLSETForwardRsp, PRT_MMI_SS_CALL_FORWARD_RSP, i);
		SetProtocolEventHandlerExt(CALLSETBarringRsp, PRT_MMI_SS_CALL_BARRING_RSP, i);
		SetProtocolEventHandlerExt(CALLSETBarringPassRsp, PRT_MMI_SS_CHANGE_PASSWORD_RSP, i);
		SetProtocolEventHandlerExt(CallFactoryAutoDailInd, PRT_CC_FATORY_AUTO_DAIL_IND, i);
	}
	
}



/**************************************************************

	FUNCTION NAME		: InitIncomingCall()

  	PURPOSE				: To Initialize the Incoming Call Management Application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
	                      application management

**************************************************************/
void InitIncomingCall (void)
{
	ReinitalizeIncomingCall();

	SetHiliteHandler(MITEM_INC_OPT_ANSWER, HiliteMenuIncomingAnswer);
	SetHiliteHandler(MITEM_INC_OPT_DEFLN, HiliteMenuIncomingCallDeflection);
	SetHiliteHandler(MITEM_CCBS_OPT_DIAL, HiliteMenuCCBSDial);

	SetHiliteHandler(MITEM_INC_ACT_OPT_END_ACT, HiliteM2019AllActiveEnd);
	SetHiliteHandler(MITEM_IAH_OPT_END_ACT, HiliteM2019AllActiveEnd);
	SetHiliteHandler(MITEM_IMA_OPT_END_ONE_ACT, HiliteM2011SingleActiveEnd);
	SetHiliteHandler(MITEM_IMAH_OPT_END_ONE_ACT, HiliteM2019AllActiveEnd);

	SetHiliteHandler(MITEM_IMA_OPT_END_ALL_ACT, HiliteMenuEndAllCalls);
	SetHiliteHandler(MITEM_INC_MHLD_OPT_END,HiliteMenuEndAllCalls );
	SetHiliteHandler(MITEM_IAH_OPT_END_ALL, HiliteMenuEndAllCalls);

	SetHiliteHandler(MITEM_INC_HLD_OPT_END,HiliteM2020AllHeldEnd );
	SetHiliteHandler(MITEM_IAH_OPT_END_HLD, HiliteM2020AllHeldEnd);
	SetHiliteHandler(MITEM_IAMH_ALL_HELD_END, HiliteM2020AllHeldEnd);

	SetHiliteHandler(MITEM_INC_ACT_OPT_HOLD_ACT, HiliteM2014Swap);
	SetHiliteHandler(MITEM_IMA_OPT_HOLD_CONF, HiliteM2014Swap);
	SetHiliteHandler(MITEM_INC_HLD_OPT_RTRV, HiliteM2014Swap);
	SetHiliteHandler(MITEM_INC_MHLD_OPT_RTRV, HiliteM2014Swap);
	SetHiliteHandler(MITEM_IAH_OPT_SWAP, HiliteM2014Swap);

	SetHiliteHandler(MITEM_IAH_OPT_CONFER, HiliteM2015Conference);
	SetHiliteHandler(MITEM_IAH_OPT_TRANSFER, HiliteM2017Transfer);
	SetHiliteHandler(MITEM_IMA_OPT_SPLIT, HiliteM2018Split);
	SetHiliteHandler(MITEM_INC_ACT_OPT_MUTE, HiliteM2020Mute);
	SetHiliteHandler(MITEM_INC_ACT_OPT_LOUDSP, HiliteM2023LoudSp);

}


/**************************************************************

	FUNCTION NAME		: HiliteMenuIncomingAnswer(void)

  	PURPOSE				: Hilite function for Incoming Answer

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HiliteMenuIncomingAnswer(void)
{
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(KbCBackCallIncomingAccepted,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteMenuIncomingCallDeflection(void)

  	PURPOSE				: Hilite function for Call Deflection

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HiliteMenuIncomingCallDeflection(void)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_HiliteMenuIncomingCallDeflection));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(HandleCallDelfectDigitEntry,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteMenuCCBSDial(void)

  	PURPOSE				: hilite handler to invoke the function that dials the ccbs call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteMenuCCBSDial(void)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_HiliteMenuCCBSDial));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(KbCBackCCBSDialCall,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteMenuEndAllCalls(void)

  	PURPOSE				: Hilite function End all call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HiliteMenuEndAllCalls(void)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_HiliteMenuEndAllCalls));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(KbCBackEndAllCallsExceptIncoming,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: PsCBackCallIncoming(void*)

  	PURPOSE				: This function is called back by the protocol stack when there
	                      is an incoming call

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackCallIncoming (void* info)
{

#ifdef __MMI_MULTI_SIM__
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL,nSimID);
#endif
	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	ProcessIncomingEvents (CM_PS_CALL_INCOMING, info);

}

/**************************************************************

	FUNCTION NAME		: PsCBackCallWait(void*)

  	PURPOSE				: This function is called back by the protocol stack when there
	                      is an incoming call

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackCallWait (void* info)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackCallWait));
	ProcessIncomingEvents (CM_PS_CALL_WAIT, info);
}

/**************************************************************

	FUNCTION NAME		: PsCBackNetworkCallDropped(void*)

  	PURPOSE				: This function is called back by the protocol stack when the
	                      Netowrk drops incoming call

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackNetworkCallDropped(void* info)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackNetworkCallDropped));

	if (GetInternalAlert() == TRUE)
	{
		StopInternalOutgoingTone();
	}
#ifdef __IP_NUMBER__
	SetTempUseIPNumber(FALSE);
#endif
	ProcessIncomingEvents (CM_PS_NWK_CALL_DROP, info);
}

/**************************************************************

	FUNCTION NAME		: PsCBackActivatedCCBS(void*)

  	PURPOSE				: This function is called back by the protocol stack when there
	                      CCBS activation response is received

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackActivatedCCBS(void* info)
{
	U16 result;

	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if (CheckChldRequestSuccess(info, &result))
	{
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_PsCBackActivatedCCBS1));
		ProcessIncomingEvents (CM_PS_ACTIVATED_CCBS, info);
	}
	else
	{
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_PsCBackActivatedCCBS2));
		ProcessIncomingFailureEvents (CM_PS_ACTIVATED_CCBS, result);
	}
}

/**************************************************************

	FUNCTION NAME		: PsCBackCcbsActivate(void*)

  	PURPOSE				: This function is called back by the protocol stack when there
	                      is outgoing call disconnect indication(CCBS)

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackCcbsActivate (void* info)
{
	U16 result;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackCcbsActivate));
	result = CheckRspCcbsFlag(info);
	if (result == TRUE)
	{
		ProcessIncomingEvents (CM_PS_CCBS_ACTIVATE, info);
	}
}

/**************************************************************

	FUNCTION NAME		: PsCBackCCBSInvoked(void*)

  	PURPOSE				: This function is called back by the protocol stack when CCBS
						  notification is there

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackCCBSInvoked (void* info)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackCCBSInvoked));
	SetCCBSFlag(TRUE);
	ProcessIncomingEvents (CM_PS_CCBS_INVOKE, info);
}

/**************************************************************

	FUNCTION NAME		: PsCBackNotifySSInd(void*)

  	PURPOSE				: This function is intended for registering no on operation
						  for PS Callback

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackNotifySSInd(void* info)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackNotifySSInd));
	ProcessIncomingEvents (CM_PS_SS_EVENT, info);
}

/**************************************************************

	FUNCTION NAME		: PsCBackSpeechIndication(void*)

  	PURPOSE				: This function is called back by the protocol stack when the
	                      Held Call is made active.

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackSpeechIndication(void* info)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackSpeechIndication));
	ProcessIncomingEvents (CM_PS_SPEECH_IND, info);
}

/**************************************************************

	FUNCTION NAME		: PsCBackAocIndication(void*)

  	PURPOSE				: This function is called back by the protocol stack when the AOC
						  indication is sent by the PS

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void PsCBackAocIndication(void* info)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackAocIndication));
	ProcessIncomingEvents (CM_PS_AOC_INDICATION, info);
}

/**************************************************************

	FUNCTION NAME		: PsCBackConfirmDTMFString

  	PURPOSE				: This is CBack for -- Indication

	INPUT PARAMETERS	: data from PS

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackConfirmDTMFString(void* info)
{
	MMI_DTMF_CONFIRM_IND *msg=(MMI_DTMF_CONFIRM_IND*)info;

	memset(gDTMFConfirmBuf,0,MAX_DTMFBUF_DIGITS);
	strcpy((PS8)gDTMFConfirmBuf,(PS8)msg->digits);

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackAocIndication));

	ProcessIncomingEvents(CM_PS_CONFIRM_DTMF_STRING,NULL);
}

/**************************************************************

	FUNCTION NAME		: NotifyCMClampOpen

  	PURPOSE				: Clamp Open Handler for CM

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
#ifdef __MMI_CLAMSHELL__
void NotifyCMClampOpen(void)
{
   if(GetCurrentState()==CM_INCOMING_STATE)
   {
	if(TRUE==(((MMI_ANSWERING_MODE *)GetAnswerMode())->coverAnswer))
	{
         KbCBackCallIncomingAccepted();
	}
#ifdef __MMI_INCOMING_CALL_VIDEO__	
	else 
	{
         CM_CALL_HANDLE handle;
         U16 imgId;
	   
         handle = GetIncomingCallHandle();
         imgId = GetCallImgId(handle);
         
         /* overwrite sub LCD with static image */
         if ((imgId & 0x8000) || ((imgId & 0x3fff) >= VDO_ID_PHB_MTCALL_1))
		{
            /* user defined video file or system default video resource */
            // draw static sub LCD
            DisableCategory17VideoUpdate();
            ShowCategory332Screen(IMG_CM_STATE_CONNECTED);
            EnableCategory17VideoUpdate();
         }
		}
#endif	
	}
}

/**************************************************************

	FUNCTION NAME		: NotifyCMClampClose

  	PURPOSE				: Clamp Close Handler for CM

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
extern U8 GetHandsetInPhone(void);
void NotifyCMClampClose(void)
{
	if(1 == GetHandsetInPhone()
#if  defined(__MMI_BT_SUPPORT__)
|| mmi_bt_is_incall_aud_swap2bt() == MMI_TRUE
#endif
	)
	{
		return;
	}
	DeInitCM();
}
#endif


/*****************************************************************************
 * FUNCTION
 *  HangupAllCalls
 * DESCRIPTION
 *  Hangs Up All Calls
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HangupAllCalls(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    CM_CALL_HANDLE handle;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_HangupAllCalls));

#ifdef WAP_SUPPORT
	if (GetWapCallPresent() && BRAa_bearerCsdIsDisc())
    {
        return;
    }
#endif /* WAP_SUPPORT */ 
     
#if 0
    if (GetDisconnectingCallHandle() != -1)
    {
        ReleaseCall();
        return;
    }
#endif /* Guoj delete. It's not used now */

    mmi_trace(g_sw_CC,"MMI_Call: Func: %s GetCurrentState()=%d,GetTotalCallCount()=%d,GetTotalActiveCallCount()=%d,GetTotalHoldCallCount()=%d", __FUNCTION__, GetCurrentState(),GetTotalCallCount(),GetTotalActiveCallCount(),GetTotalHoldCallCount());
 
	if (GetTotalCallCount() == 0)  //guojian added 20090615
		return;
    
    if (GetCurrentState() == CM_OUTGOING_STATE && GetTotalCallCount() == 1)
    {
        handle = GetOutgoingCallHandle();
        SetDropRequestFlag(TRUE);
        if ((GetCallflag(handle, TRUE) & CM_HANGUP_REQUESTED) == 0)
        {
            OutgoingProcessCMEvent(CM_KB_HANGUPREQ, &handle);
        }
    }
    /* hangup single MT call should use reject instead of rel all */
	else if (GetCurrentState() == CM_INCOMING_STATE && GetTotalCallCount() == 1)
    {
        KbCBackCallIncomingRejected();
    }
    else
    {
        U8 flag;

        /* swapping can't decide which call is active and held, 
           issue rel all req instead */
        flag = GetAllCallFlags();
        mmi_trace(g_sw_CC,"MMI_Call: Func: %s flag=%x,gAttempToAnswer=%d", __FUNCTION__,flag,gAttempToAnswer);
        if ((GetTotalActiveCallCount() > 0) && (GetTotalHoldCallCount() > 0) &&
			(((flag & CM_SWAP_REQUESTED) != 0) || ((flag & CM_CONF_REQUESTED) != 0)))
        {
            SetAllCallsHangupFlag();
            ProcessIncomingEvents(CM_KB_HANG_ALL_CALLS, NULL);
        }

    #ifdef __MMI_CLAMSHELL__
        /* attemp to answer waiting call and press END key 
           should rel all calls because we can't be sure of 
           current call state and rel active or rel held */
        else if (GetCurrentState() == CM_INCOMING_STATE && GetTotalCallCount() > 1
                 && IsClamClose() == FALSE && gAttempToAnswer == FALSE)
        {
            KbCBackCallIncomingRejected();
        }
    #endif /* __MMI_CLAMSHELL__ */ 
        else if (GetChldReqSent() == 0)
        {
            if ((GetTotalActiveCallCount() > 0) && (GetTotalHoldCallCount() > 0) &&
                (((flag & CM_SWAP_REQUESTED) == 0) || ((flag & CM_CONF_REQUESTED) == 0))
        #ifdef __MMI_CLAMSHELL__
                && IsClamClose() == FALSE
        #endif 
                )
            {
                /* clam open */
                /* rel active and accept */
                ProcessIncomingEvents(CM_KB_END_ALL_ACTIVE_RETRIEVE_HELD, NULL);
                return;
            }
        }
            SetAllCallsHangupFlag();
            ProcessIncomingEvents(CM_KB_HANG_ALL_CALLS, NULL);
    }
}

/**************************************************************

	FUNCTION NAME		: KbCBackCallIncomingAccepted(void)

  	PURPOSE				: This function is called back by the keyboard handler
	                      when the user selects accepts incoming call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern BOOL HA_audio_StopStream(VOID);
#ifdef MAGIC_AE_SUPPORT
extern void CallsetSetMagicSound(U8 state);
#endif
#ifdef __MMI_AUTO_PLUS_ENABLE__
extern void CALLSETsetAutoPlus(U8 state);
#endif
void KbCBackCallIncomingAccepted (void)
{
	CM_CALL_HANDLE handle;
mmi_trace(1," KbCBackCallIncomingAccepted ");	
   HA_audio_StopStream();

   if( g_charbat_context.LowBatteryTxProhibitFlag == 1)
      SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_TALK_DURCNT_PROHIBIT );
   else
      SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_TALK_DURCNT_NORMAL );
   g_charbat_context.LowBatteryCutOffCallCnt = 0;

   if ((GetTotalActiveCallCount()>0 && GetTotalHoldCallCount()>0) ||
   	CheckAllCallflag() == TRUE)
   {
      ShowCallManagementErrorMessage(NO_ACTIVE_CALL);
      return;
   }

	// preempt auto answer by manual answer before timeout
	if(GetAutoAnswerFlag()==TRUE)
	{
		StopTimer(CM_AUTOANSWER_NOTIFYDURATION_TIMER);
		SetAutoAnswerFlag(FALSE);
	}

	if (GetWapCallPresent() || GetActiveCallType() == CM_DATA_CALL)
   {
   	KbCBackEndActiveAcceptIncomingCall();
   	ClearKeyHandler(KEY_VOL_DOWN, KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_VOL_DOWN, KEY_EVENT_UP);
		ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_UP);
   	return;
   }

   handle = GetIncomingCallHandle();
	if ((GetCallflag(handle, TRUE) & CM_HANGUP_REQUESTED))
	{
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_KbCBackCallIncomingAccepted));
		return;
	}

	
	UnSilencethePhone();
	StopIncomingCallIndication();

	ClearKeyHandler(KEY_VOL_DOWN, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_VOL_DOWN, KEY_EVENT_UP);
	ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_UP);

	ClearKeyHandler(KEY_RSK, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RSK, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);

#ifdef MAGIC_AE_SUPPORT
	CallsetSetMagicSound(g_callset_cntx_p->MagicSoundState);
#endif

#ifdef __MMI_AUTO_PLUS_ENABLE__
	CALLSETsetAutoPlus(g_callset_cntx_p->AutoPlusState);
#endif

	ProcessIncomingEvents (CM_KB_INCOMING_CALL_ACCEPT,NULL);

}

/**************************************************************

	FUNCTION NAME		: KbCBackCallIncomingRejected(void)

  	PURPOSE				: This function is called back by the keyboard handler
	                      when the user selects rejects incoming call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void StopGpadcDetect(void);
extern void SetSpeakerWorkStatus(U8 status);
void KbCBackCallIncomingRejected (void)
{
   gAttempToReject = TRUE;
	UnSilencethePhone();
   StopIncomingCallIndication();
	ProcessIncomingEvents (CM_KB_INCOMING_CALL_REJECT,NULL);
	ClearKeyHandler(KEY_VOL_DOWN, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_VOL_DOWN, KEY_EVENT_UP);
	ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_UP);

   /*if (GetExitScrnID() ==  ITEMSCR_INCOMING_CALL)
   {
#ifdef __MMI_INCOMING_CALL_VIDEO__
      CM_CALL_HANDLE handle;
      U16 imgId;
      
      handle = GetIncomingCallHandle();
      imgId = GetCallImgId(handle);
         
      // pause video 
      if ((imgId & 0x8000) || ((imgId & 0x3fff) >= VDO_ID_PHB_MTCALL_1))
         StopCategory17Video();
      else
#endif
      MOMTHideAnimation();
   }*/
}

#ifdef __MMI_SIDEKEY_SUPPRESS_RING__
/**************************************************************

	FUNCTION NAME		: KbCBackCallIncomingRejectedNoTone(void)

  	PURPOSE				: Reject MT call and stop ring tone when long press
  							  or double press side down key

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void KbCBackCallIncomingRejectedNoTone (void)
{
   gAttempToReject = TRUE;
	UnSilencethePhone();
	ClearKeyHandler(KEY_VOL_DOWN, KEY_EVENT_UP);
	ProcessIncomingEvents (CM_KB_INCOMING_CALL_REJECT,NULL);
}

/**************************************************************

	FUNCTION NAME		: KbCBackCallIncomingSuppressTone(void)

  	PURPOSE				: Set key handler for side down key in its first press

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void KbCBackCallIncomingSuppressTone (void)
{
   SetKeyHandler(KbCBackCallIncomingRejectedNoTone, KEY_VOL_DOWN, KEY_EVENT_UP);
}
#endif

/**************************************************************

	FUNCTION NAME		: KbCBackEndActiveAcceptIncomingCall(void)

  	PURPOSE				: End active and accept waiting call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void KbCBackEndActiveAcceptIncomingCall (void)
{
	StopIncomingCallIndication();
	ProcessIncomingEvents (CM_KB_END_ACTIVE_ACCEPT_INCOMING, NULL);
}

/**************************************************************

	FUNCTION NAME		: KbCBackEndSelectiveActiveCall(void)

  	PURPOSE				: This function is called back by the keyboard handler
	                      when the user selects option to End an active call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
/*void KbCBackEndSelectiveActiveCall(void)
{
	if (gCallToEnd!=-1)
		MakePsEndSelectiveCall ((void*)PsCBackselectedCallEnded, gCallToEnd);
	else
		PRINT_INFORMATION(("CM_TEST File: [%s]  Line: [%d] <<No Call To End>>\n",__FILE__,__LINE__));
}*/

/**************************************************************

	FUNCTION NAME		: KbCBackEndAllCallsExceptIncoming(void)

  	PURPOSE				: This function is called back by the keyboard handler
	                      when the user selects rejects incoming call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void KbCBackEndAllCallsExceptIncoming(void)
{
	UnSilencethePhone();
	OutgoingProcessCMEvent(CM_KB_HANGUPALLREQ,(void*)NULL);
}

/**************************************************************

	FUNCTION NAME		: IgnoreCCBS(void)

  	PURPOSE				: To invoke the function that ignore the CCBS invoke

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void IgnoreCCBS(void)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_IgnoreCCBS));
	SetDialIndication(FALSE);
	SetActivatedCcbsFlag(FALSE);
	DropRequest();
}

/**************************************************************

	FUNCTION NAME		: ActivateCCBS(void)

  	PURPOSE				: To invoke the function that activates the CCBS

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void ActivateCCBS(void)
{
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ActivateCCBS));
	SetActivatedCcbsFlag(TRUE);
	MakePsActivateCCBS((void*)PsCBackActivatedCCBS);
}

/**************************************************************

	FUNCTION NAME		: KbCBackCCBSDialCall(void)

  	PURPOSE				: This function is called back by the keyboard handler
	                      when the user selects dial ccbs

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void KbCBackCCBSDialCall (void)
{
	UnSilencethePhone();
	StopIncomingCallIndication();
	CCBSUpdateCallInfo();
	ProcessIncomingEvents (CM_PS_CCBS_INVOKE_DIAL,NULL);
	ClearKeyHandler(KEY_VOL_DOWN, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_VOL_DOWN, KEY_EVENT_UP);
	ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: KbCBackCCBSInvokeRejected(void)

  	PURPOSE				: This function is called back by the keyboard handler
	                      when the user selects rejects ccbs invoke

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void KbCBackCCBSInvokeRejected (void)
{
	UnSilencethePhone();
	StopIncomingCallIndication();
	ProcessIncomingEvents (CM_PS_CCBS_INVOKE_REJECT,NULL);
	ClearKeyHandler(KEY_VOL_DOWN, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_VOL_DOWN, KEY_EVENT_UP);
	ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_VOL_UP, KEY_EVENT_UP);	
}

/**************************************************************

	FUNCTION NAME		: IgnoreDTMFStringAndGoBackHistory

	PURPOSE				: cancels sending dtmf string

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void IgnoreDTMFStringAndGoBackHistory(void)
{
	PRINT_INFORMATION (("IgnoreDTMFStringAndGoBackHistory\n"));
	
	SetNoSave();
	GoBackHistory();
}

/**************************************************************

	FUNCTION NAME		: ConfirmDTMFString

	PURPOSE				: Send confirm dtmf notification to ps

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void ConfirmDTMFString(void)
{
	SetNoSave();
	MakePsSendDTMFString(gDTMFConfirmBuf);
	StartTimer(CM_NOTIFYDURATION_TIMER,1, GoBackfromNotifyScr);
}

/**************************************************************

	FUNCTION NAME		: SetKeysForPOPUp

  	PURPOSE				: Sets Group Handler for all Keys on Popup

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void SetKeysForPOPUp(void)
{
	SetGroupKeyHandler((FuncPtr)POPUPHandler,(PU16)PresentAllKeys,TOTAL_KEYS,KEY_EVENT_DOWN);
	SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: POPUPHandler

  	PURPOSE				: Group Key Handler for all Keys on CM Popups

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void POPUPHandler(void)
{
	StopTimer(CM_NOTIFYDURATION_TIMER);
	GoBackfromNotifyScr();
}

/**************************************************************

	FUNCTION NAME		: ProcessHeadsetKeyPress

  	PURPOSE				: HeadSet Key Handling Depending on various conditions

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
//extern void mmi_frm_sms_abort_sms (void);  // zrx del 20060612
// zrx add beign 20060612
// wangbei del start 20060614
#if 0
void mmi_frm_sms_abort_sms (void)
{
}
#endif
// wangbei del end
// zrx add end
extern U16 GetCurrScreenID(void);
extern void mmi_frm_sms_abort_sms (void);
void ProcessHeadsetKeyPress(void)
{
	mmi_trace(g_sw_BT," ProcessHeadsetKeyPress ");
	if((GetCurrScreenID()==SCR_ID_MSG_SENDING)&&isInCall())
	{
		mmi_frm_sms_abort_sms();
	}
	else if(GetCurrentState()==CM_INCOMING_STATE)
	{
		if (GetTotalActiveCallCount () >= 1 && GetTotalHoldCallCount() >= 1)
		{
			SwapCall();
		}
		else if (GetTotalCallCount() > 1)
		{
			/* more than 1 call, auto answer can't work */
			if ((gAttempToReject == FALSE) && (gAttempToAnswer == FALSE))
		      KbCBackCallIncomingAccepted();
		}
		else
		{
		   /*Jinzh Modify Start For 6503 Ver: TBM780  on 2007-8-31 10:33 */
		   if ((gAttempToReject == FALSE) && (gAttempToAnswer == FALSE)) 
		   	// && (GetAutoAnswerFlag() == 0))
		   /*Jinzh Modify End  For 6503 Ver: TBM780  on 2007-8-31 10:33 */
				KbCBackCallIncomingAccepted();
		}
	}
	else if(GetCurrentState()==CM_OUTGOING_STATE)
	{
		if(1==GetTotalCallCount())
			DropRequest();
		else
			HangupAll();
	}
	else if((GetCurrentState()==CM_ACTIVE_STATE)||(GetCurrentState()==CM_HOLD_STATE))
	{
		HangupAll();
	}
}

U8 g_ProcessRejectedCall;
/**************************************************************

	FUNCTION NAME		: ProcessIncomingEvents (CM_EVENT_TYPES event, void* info)

  	PURPOSE				: This function is the entry into the Call Management State
						  Machine.

	INPUT PARAMETERS	: Incoming Event Type, Any information associated with the event

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
	                      as per keyboard event and protocol event

**************************************************************/
void ProcessIncomingEvents (CM_EVENT_TYPES event, void* info)
{
	ACTION_RESULT result;
	 
	CM_CALL_HANDLE handle;
	 
    mmi_trace(g_sw_CC,"MMI_Call: +++Func: %s event=%d", __FUNCTION__, event);
    
 
	switch (event)
	{

	/******************* indications *********************/

	case CM_PS_CALL_INCOMING:
		/* incoming call */
		result = ProcessIncomingCallEvent(info);
		if (result != CM_CALL_SUCCESS)
		{
			if (result == CM_NO_INCOMING)
				mmi_trace(g_sw_CC,"File: [%s]  Line: [%d] <<** State Machine ** Cannot Accept anymore incoming calls>>\n",__FILE__,__LINE__);
			else
				mmi_trace(g_sw_CC,"CM_TEST File: [%s]  Line: [%d] <<Error Processing Incoming Call Event>>\n",__FILE__,__LINE__);
		}
		break;

	case CM_PS_CALL_WAIT:
		/* waiting call */
		 
		// automatically release all disconnecting call before incoming call comes in 
		SetAutoReleaseFlag(TRUE);
		ReleaseCall();
		 
		result = ProcessCallWait(info);
		if (result != CM_CALL_SUCCESS)
		{
			mmi_trace(g_sw_CC,"File: [%s]  Line: [%d] <<** Error processing waiting call>>\n",__FILE__,__LINE__);
		}
		break;

	case CM_PS_CCBS_ACTIVATE:
		/* CCBS available ind */
		EntryScrCcbsActivate();
		break;

	case CM_PS_CCBS_INVOKE:
		/* CCBS invoke */
		result = ProcessInvokedCcbs(info);
		if (result != CM_CALL_SUCCESS)
		{
			mmi_trace(g_sw_CC,"CM_TEST File: [%s]  Line: [%d] <<Error Processing CCBS Invoke Event>>\n",__FILE__,__LINE__);
		}
		break;

	case CM_PS_INCOMING_CALL_DROPPED:
		/* MT drop */

		SetIncomingCallDroppedFlag(TRUE);
		result = ProcessIncomingCallRejected(info);
		if (result != CM_CALL_SUCCESS)
		{
			mmi_trace(g_sw_CC,"File: [%s]  Line: [%d] <<** State Machine ** Could Not drop incoming call>>\n",__FILE__,__LINE__);
		}

		aud_speech_set_mode(KAL_FALSE);
		break;

	case CM_PS_NWK_CALL_DROP:
		/* call drop */
#ifdef __MMI_BT_SUPPORT__
		if (mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_ACL))//zhou siyou add ,20090715
			rda_bt_hfg_send_BldnFail_ind();
#endif		
		result = ProcessEndCallIdAndGoBack(info);
		aud_speech_set_mode(KAL_FALSE); //add by liwh
		mmi_trace(g_sw_CC,"MMI_Call: Func: %s CM_PS_NWK_CALL_DROP  isInCall()=%d", __FUNCTION__,isInCall());
	
		if (result != CM_CALL_SUCCESS)
		{
			mmi_trace(g_sw_CC,"CM_TEST File: [%s]  Line: [%d] <<Error Processing Network Drop Event>>\n",__FILE__,__LINE__);
		}
		break;

	case CM_PS_SS_EVENT:
		playRequestedTone(WARNING_TONE_IN_CALL);
		ProcessShowSSEventNotification(info);
		break;

	case CM_PS_SPEECH_IND:
		ReverseSpeechIndication(info);
		break;

	case CM_PS_AOC_INDICATION:
		result = ProcessAocIndication(info);
		if (result != CM_CALL_SUCCESS)
		{
			mmi_trace(g_sw_CC,"File: [%s]  Line: [%d] <<** State Machine ** Error In processing AOC >>\n",__FILE__,__LINE__);
		}
		break;

	case CM_PS_CONFIRM_DTMF_STRING:
		/* DTMF confirm ind */
		StartTimer(CM_AUTO_DTMF_CONFIRM_TIMER, CM_NOTIFY_TIMEOUT, EntryScrConfirmDTMFString);
		break;


	/****************** request/responses ***********************/

	case CM_KB_INCOMING_CALL_ACCEPT:
		/* SEND key in MT call */

		/*Guojian Add Start For 10823 Ver: GreenStone  on 2008-12-18 15:19 */
#ifdef __MMI_MT_CALL_SPEAKER_MUTE__
		ResumeSpeaker();
#endif
		/*Guojian Add End  For 10823 Ver: GreenStone  on 2008-12-18 15:19 */
		SetSpeechVolume(GetModeSpeechVolume());

		ProcessStateCheckIncomingCall();
		break;

	case CM_PS_INCOMING_CALL_CONNECTED:
		/* SEND key rsp */
    		aud_speech_set_mode(KAL_TRUE);
		result = ProcessAcceptIncomingCall(info);
		if (result != CM_CALL_SUCCESS)
		{
			PRINT_INFORMATION(("File: [%s]  Line: [%d] <<** State Machine ** COuld Not accept incoming call>>\n",__FILE__,__LINE__));
		}
		break;

	case CM_PS_ACTIVE_CALL_HELD:
		result=	ProcessCallsHeld(info);
		if (result != CM_CALL_SUCCESS)
		{
			PRINT_INFORMATION(("File: [%s]  Line: [%d] <<** State Machine ** Could Not hold active call>>\n",__FILE__,__LINE__));
		}
		break;

	case CM_KB_INCOMING_CALL_REJECT:
    g_ProcessRejectedCall = 1;
		handle = GetIncomingCallHandle();
		/* END key in MT call */
		mmi_trace(g_sw_CC,"MMI_Call: Func: %s CM_KB_INCOMING_CALL_REJECT GetTotalActiveCallCount() + GetTotalHoldCallCount()=%d", __FUNCTION__, GetTotalActiveCallCount() + GetTotalHoldCallCount());
		if (GetTotalActiveCallCount() + GetTotalHoldCallCount() >= 1)
		{
			 
			SetCallState(handle, CM_DISCONNECTING_STATE, TRUE);
			//EntryScrIncomingCallEvent();
			 
			MakePsSendUDUB((void*)PsCBackIncomingCallRejected);//special case for ending waitng call
		}
		else
		{
			 
			SetCallState(handle, CM_DISCONNECTING_STATE, TRUE);
			//EntryScrIncomingCallEvent();
			 
			MakePsAthRequest((void*)PsCBackIncomingCallRejected);
		}
		break;

	case CM_PS_INCOMING_CALL_REJECTED:

		/* 0 send / END key rsp */
    	result = ProcessIncomingCallRejected(info);
		if (result != CM_CALL_SUCCESS)
		{
			PRINT_INFORMATION(("File: [%s]  Line: [%d] <<** State Machine ** Could Not drop incoming call>>\n",__FILE__,__LINE__));
		}
		break;

	case CM_KB_END_ACTIVE_ACCEPT_INCOMING:
		/* 1 send */
		 
		SetAllActiveCallState(CM_DISCONNECTING_STATE);
		//EntryScr1002ActiveCall();
		 
		gAttempToAnswer = TRUE;
		MakePsHangupallActiveAcceptOtherRequest((void*)PsCBackselectedCallEnded);
#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackCheckChldIncomingCallAccepted, PRT_END_CHLD_RSP, MTPNP_AD_CALL_CHANNEL);
#else
		SetProtocolEventHandler (PsCBackCheckChldIncomingCallAccepted,PRT_END_CHLD_RSP);
#endif
		break;

	case CM_PS_ACTIVE_CALL_DISCONNECTED:
		/* 1 send / Menu End Single release ind */
		OutgoingProcessCMEvent(CM_PS_HANGUPSUC,info);
		break;

	case CM_KB_END_ALL_ACTIVE_RETRIEVE_HELD:
		/* 1 send */
		 
		SetAllActiveCallState(CM_DISCONNECTING_STATE);
		//EntryScr1002ActiveCall();
		 
		MakePsHangupallActiveAcceptOtherRequest((void*)PsCBackActiveCallEndedHeldRetrieved);
		break;

	case CM_PS_ALL_ACTIVE_CALL_DISCONNECTED:
		/* 1 send release ind */
		result=ProcessEndAllActiveCall(info);
		if (result != CM_CALL_SUCCESS)
		{
			PRINT_INFORMATION(("File: [%s]  Line: [%d] <<** State Machine ** Could Not end all active calls>>\n",__FILE__,__LINE__));
		}
		break;

	case CM_PS_HELD_CALL_ACTIVE:
		/* 1 send rsp */
		result = ProcessHold2Active(info);
		if (result != CM_CALL_SUCCESS)
		{
			PRINT_INFORMATION(("File: [%s]  Line: [%d] <<** State Machine ** Could Not make hold call active>>\n",__FILE__,__LINE__));
		}
		break;

	case CM_PS_CCBS_INVOKE_DIAL:
		/* SEND key in CCBS invoke, change to KB */
		result=ProcessCCBSDialRequest(info);
		if (result != CM_CALL_SUCCESS)
		{
			PRINT_INFORMATION(("File: [%s]  Line: [%d] <<** Error processing Dial CCBS>>\n",__FILE__,__LINE__));
		}
		break;

	case CM_PS_ACTIVATED_CCBS:
		/* 5 send rsp */
		EntryScrNotifyCCBSSuccess();
		break;

	case CM_PS_CCBS_INVOKE_REJECT:
		/* END key in CCBS invoke */
		result=ProcessCCBSRejectRequest(info);
		if (result != CM_CALL_SUCCESS)
		{
			PRINT_INFORMATION(("File: [%s]  Line: [%d] <<** Error processing Dial CCBS>>\n",__FILE__,__LINE__));
		}
		break;

	case CM_KB_HANG_ALL_CALLS:
		/* END key in MMI, move to outgoingcallmanagement */
		 
		/*
		#ifdef __MMI_CLAMSHELL__
		if (IsClamClose() == FALSE)
		#endif
		*/
		      SetAllCallState(CM_DISCONNECTING_STATE);
		//EntryScr1002ActiveCall();
		 
		if(-1!=GetIncomingCallHandle())
		{
			StopIncomingCallIndication();
		}
		MakePsHangupallCallsRequest((void*)HangupallReqSucess);
		break;

	default:
			PRINT_INFORMATION(("File: [%s]  Line: [%d] <<Not Valid State Machine event>>\n",__FILE__,__LINE__));
			break;
	}
}

/**************************************************************

	FUNCTION NAME		: ProcessIncomingCallEvent(void* info)

  	PURPOSE				: This function is called by the state machine function
						  hold Process an incoming call

	INPUT PARAMETERS	: any information associated with incoming call

	OUTPUT PARAMETERS	: result of execution of function which may be success or failure

	RETURNS				: void

 

**************************************************************/
#ifdef __MMI_CM_BLACK_LIST__
 UINT8 g_in_idle_blackcall_flag = 0;
#endif
ACTION_RESULT ProcessIncomingCallEvent (void* info)
{
	IncomingCall myIncmg;
	U8 TempNumber[MAX_NUMBER];
	MMI_INCOMING* inc;
	
    	mmi_trace(g_sw_CC, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
     	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessIncomingCallEvent));

	inc = (MMI_INCOMING*) info;
	gAttempToAnswer = FALSE;

	DeriveCallInfo(info, &myIncmg);
	SetAnswerMode (NULL);

	AnsiiToUnicodeString((PS8)TempNumber,(PS8) myIncmg.Number);
	pfnUnicodeStrcpy((PS8) myIncmg.Number,(PS8)TempNumber);

	AlmDisableExpiryHandler();
	switch (GetCurrentState())
	{
	case CM_IDLE_STATE:
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ProcessIncomingCallEvent1));
		
		SetPreviousState(GetCurrentState());
		SetCurrentState(CM_INCOMING_STATE);

    #if defined(__MMI_BT_PROFILE__) && defined(__MMI_HFP_SUPPORT__ )
//    	    mmi_profiles_bt_call_start_callback();
	#endif

		AddNewCallInfo (myIncmg.Number,GetCurrentState(),GetPreviousState(),
						CM_CALL_MT,(CM_CALL_HANDLE)myIncmg.callHandle, myIncmg.callType);
		 
	#ifdef __MMI_CM_BLACK_LIST__
		if (BlockedHandle((CM_CALL_HANDLE)myIncmg.callHandle)==TRUE)
		{
			g_in_idle_blackcall_flag = TRUE;
			ProcessIncomingEvents (CM_KB_INCOMING_CALL_REJECT, info);
			return CM_CALL_SUCCESS;
		}
	#endif
		 
		if(GetPriorityCall()==TRUE)
		{
			return CM_CALL_SUCCESS;
		}

		SetAnswerMode (GetAnsweringMode());

		if (((MMI_ANSWERING_MODE *)GetAnswerMode())->automatic==TRUE)
	   {
		   SetAutoAnswerFlag(TRUE);
		   StartTimer(CM_AUTOANSWER_NOTIFYDURATION_TIMER, CM_AUTOANSWER_NOTIFY_TIMEOUT,KbCBackCallIncomingAccepted);
	   }

		#ifdef __J2ME__
		{
			extern void j2me_enter_incoming_call_notify(void);
			j2me_enter_incoming_call_notify();
		}
		#endif
		EntryScrIncomingCallEvent();

		if(inc->mt_type == MT_TYPE_CALL_FORWARD)
		{
			DisplayPopup((PU8)GetString(STR_NFY_CALL_FWDED), IMG_GLOBAL_ACTIVATED,  TRUE, 1000, SUCCESS_TONE);
		}
		ShowIncomingCallIndication();
		AddMarkerToHistory();
		return CM_CALL_SUCCESS;

	case CM_OUTGOING_STATE:
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ProcessIncomingCallEvent2));

	   if (cm_p->redial_info.RedialTimer == TRUE)
	   {
			SetPreviousState(GetCurrentState());
			SetCurrentState(CM_INCOMING_STATE);

			AddNewCallInfo (myIncmg.Number,GetCurrentState(),GetPreviousState(),
						CM_CALL_MT,(CM_CALL_HANDLE)myIncmg.callHandle, myIncmg.callType);
			 
		#ifdef __MMI_CM_BLACK_LIST__
			if (BlockedHandle((CM_CALL_HANDLE)myIncmg.callHandle)==TRUE)
			{
				ProcessIncomingEvents (CM_KB_INCOMING_CALL_REJECT, info);
				return CM_CALL_SUCCESS;
			}
		#endif
			 
			if(GetPriorityCall()==TRUE)
			{
				return CM_CALL_SUCCESS;
			}
	              ResetRedialAttempts();
			EntryScrIncomingCallEvent();
			if(inc->mt_type == MT_TYPE_CALL_FORWARD)
			{
				DisplayPopup((PU8)GetString(STR_NFY_CALL_FWDED), IMG_GLOBAL_ACTIVATED,  TRUE, 1000, SUCCESS_TONE);
			}				
			ShowIncomingCallIndication();
			if (!IsScreenPresent(CM_SCR_MARKER))
			   AddMarkerToHistory();
			return CM_CALL_SUCCESS;
	   }
	    
	   else
	   {
	   	SetPreviousState(GetCurrentState());
			SetCurrentState(CM_INCOMING_STATE);

		   AddNewCallInfo (myIncmg.Number,GetCurrentState(),GetPreviousState(),
						CM_CALL_MT,(CM_CALL_HANDLE)myIncmg.callHandle, myIncmg.callType);
		 
	#ifdef __MMI_CM_BLACK_LIST__
		if (BlockedHandle((CM_CALL_HANDLE)myIncmg.callHandle)==TRUE)
		{
			ProcessIncomingEvents (CM_KB_INCOMING_CALL_REJECT, info);
			return CM_CALL_SUCCESS;
		}
	#endif
		 
		   if(GetPriorityCall()==TRUE)
		   {
			   return CM_CALL_SUCCESS;
		   }
		   EntryScrIncomingCallEvent();
		   if(inc->mt_type == MT_TYPE_CALL_FORWARD)
		   {
		        DisplayPopup((PU8)GetString(STR_NFY_CALL_FWDED), IMG_GLOBAL_ACTIVATED,  TRUE, 1000, SUCCESS_TONE);
		   }		   
		   ShowIncomingCallIndication();
		   DeleteUptoScrID(CM_SCR_MARKER);
		   return CM_CALL_SUCCESS;
	   }
	default:
		break;
	}
	return CM_CALL_FAILURE;
}

/**************************************************************

	FUNCTION NAME		: ProcessCallWait(void* info)

  	PURPOSE				: This function is called by the state machine function
						  hold Process an incoming call

	INPUT PARAMETERS	: any information associated with incoming call

	OUTPUT PARAMETERS	: result of execution of function which may be success or failure

	RETURNS				: void

 

**************************************************************/
void CallWaitRingToneStartTimer(void)
{
    static U8 count = 0;

    count++;
    StartRingTone (TONE_CALL_WAITING);
    
    if(count < 3)
    {
        if( count%2 )
            coolsand_UI_start_timer(1500, CallWaitRingToneStartTimer);
        else
            coolsand_UI_start_timer(500, CallWaitRingToneStartTimer);            
    }
    else
    {
        count = 0;
    }
}

ACTION_RESULT ProcessCallWait(void* info)
{
	IncomingCall myIncmg;
	U8 TempNumber[MAX_NUMBER];

	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	gAttempToAnswer = FALSE;

	SetPreviousState(GetCurrentState());
	SetCurrentState(CM_INCOMING_STATE);

	SetCallWaitFlag(TRUE);

	DeriveCallInfoFromWait(info, &myIncmg);
	AnsiiToUnicodeString((PS8)TempNumber,(PS8) myIncmg.Number);
	pfnUnicodeStrcpy((PS8) myIncmg.Number,(PS8)TempNumber);

	AddNewCallInfo (myIncmg.Number,GetCurrentState(),GetPreviousState(),
					CM_CALL_MT,(CM_CALL_HANDLE)myIncmg.callHandle,myIncmg.callType);
	 
#ifdef __MMI_CM_BLACK_LIST__
	if (BlockedHandle((CM_CALL_HANDLE)myIncmg.callHandle)==TRUE)
	{
		ProcessIncomingEvents (CM_KB_INCOMING_CALL_REJECT, info);
		return CM_CALL_SUCCESS;
	}
#endif
	 
	#ifdef __J2ME__
	{
		extern void j2me_enter_incoming_call_notify(void);
		j2me_enter_incoming_call_notify();
	}
	#endif
	EntryScrIncomingCallEvent();

	StartRingTone (TONE_CALL_WAITING);
	coolsand_UI_start_timer(500, CallWaitRingToneStartTimer);

	SetSpeechVolume(GetModeSpeechVolume());  //guojian added for bug 10419. set the speech volume back.
	

	if (!IsScreenPresent(CM_SCR_MARKER))
		AddMarkerToHistory();

	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessInvokedCcbs(void*)

  	PURPOSE				: This function is called when CCBS
						  invoked notification is there

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: result of execution of function which may be success or failure

	RETURNS				: void

 

**************************************************************/
ACTION_RESULT ProcessInvokedCcbs (void* info)
{
	IncomingCall myIncmg;
	U8 TempNumber[MAX_NUMBER];

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessInvokedCcbs));

	DeriveCallInfo(info, &myIncmg);
	AnsiiToUnicodeString((PS8)TempNumber,(PS8) myIncmg.Number);
	pfnUnicodeStrcpy((PS8) myIncmg.Number,(PS8)TempNumber);

	if(GetCurrentState()!=CM_INCOMING_STATE)
	{
     		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ProcessInvokedCcbs));

		SetPreviousState(GetCurrentState());
		SetCurrentState(CM_INCOMING_STATE);

		AddNewCallInfo (myIncmg.Number,GetCurrentState(),GetPreviousState(),
					CM_CALL_MT,(CM_CALL_HANDLE)myIncmg.callHandle, CSMCC_VOICE_CALL);

		EntryScrCCBSInvokeEvent();

		if (!IsScreenPresent(CM_SCR_MARKER))
			AddMarkerToHistory();

		return CM_CALL_SUCCESS;
	}
	return CM_CALL_FAILURE;
}

/**************************************************************

	FUNCTION NAME		: ProcessIncomingCallRejected(void* info)

  	PURPOSE				: This function is called by the state machine function reject
						  incoming call

	INPUT PARAMETERS	: any information associated with incoming call

	OUTPUT PARAMETERS	: Success or failure

	RETURNS				: void

 

**************************************************************/
ACTION_RESULT ProcessIncomingCallRejected(void* info)
{
	U8 flag=0;  // Set if previous state is outgoing

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessIncomingCallRejected));
     
   if ((GetTotalCallCount() > 1) &&
   	(GetIncomingCallHandle() != ((MMI_RELEASE_IND *)info)->call_id))
   {
   	/* use chld to reject */
   	/* dropped is not MT call, use default drop call handler */
      PsCBackNetworkCallDropped(info);
      return CM_CALL_SUCCESS;
   }
   else if ((GetTotalCallCount() == 1 && GetCurrentState() != CM_INCOMING_STATE) ||
   		(GetIncomingCallHandle() == -1))
   {
   	/* use ath to reject or call already dropped when ath rsp comes*/
     CM_CALL_HANDLE handle;

     PRINT_INFORMATION(("ProcessIncomingCallRejected: not incoming state"));
   	handle = GetHungUpHandle();
   	if (handle != -1)
   		OutgoingProcessCMEvent(CM_PS_HANGUPSUC,(void*)&handle);
	return CM_CALL_SUCCESS;
   }

   gAttempToReject = FALSE;
	ResetCallflag(GetIncomingCallHandle(), CM_HANGUP_REQUESTED, TRUE);

	
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif

	switch (GetPreviousState())
	{
	case CM_IDLE_STATE:
		g_in_idle_blackcall_flag = FALSE; 
		SetPreviousState(CM_INCOMING_STATE);
		SetCurrentState(CM_IDLE_STATE);
		break;

	case CM_HOLD_STATE:
		if (GetTotalHoldCallCount()>0)
		{
			SetCurrentState(CM_HOLD_STATE);
			SetPreviousState(CM_INCOMING_STATE);
		}
		else
		{
			SetCurrentState(CM_IDLE_STATE);
			SetPreviousState(CM_IDLE_STATE);
		}
		break;

	case CM_ACTIVE_STATE:
		if (GetTotalActiveCallCount()>0)
		{
			SetCurrentState(CM_ACTIVE_STATE);
			SetPreviousState(CM_INCOMING_STATE);
		}
		else
		{
			SetCurrentState(CM_IDLE_STATE);
			SetPreviousState(CM_IDLE_STATE);
		}
		break;

	case CM_INCOMING_STATE:
		if (GetCurrentState() == CM_OUTGOING_STATE)
		{
		}
		else if (GetCurrentState() == CM_INCOMING_STATE)
		{
		}
		else if (GetTotalActiveCallCount() > 0)
		{
			SetCurrentState(CM_ACTIVE_STATE);
		}
		else if (GetTotalHoldCallCount() > 0)
		{
			SetCurrentState(CM_HOLD_STATE);
		}
		else
			SetCurrentState(CM_IDLE_STATE);
		break;

	case CM_OUTGOING_STATE:
		flag=1;
		break;

	default:
		PRINT_INFORMATION (("Previous State is Unknown this should not happen\n"));
		return CM_CALL_FAILURE;
	}

	if(GetIncomingCallDroppedFlag())
	{
		SetIncomingCallDroppedFlag(FALSE);
	   StopIncomingCallIndication();
	}
	//if(GetMissedCallFlag()==FALSE)
		DeleteScreenIfPresent(ITEMSCR_INCOMING_CALL);
	DeleteIncomingCallOptionScreen();

	if(GetCCBSFlag()==TRUE)
	{
		/* don't log CCBS call */
		PurgeIncomingCallStructure();
		GoBackfromNotifyScr();
		SetCCBSFlag(FALSE);
   }
	else
	{
	   LogCallInfoForCallHistory(GetIncomingCallHandle());
	   PurgeIncomingCallStructure();
	}

	if(1==flag)
	{
		UpdateStateMachine();
	}
	

	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessEndCallIdAndGoBack

  	PURPOSE				: This function is called by the state machine function in
						  to make call to protocol stack to get call list to sync with MMI

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
ACTION_RESULT ProcessEndCallIdAndGoBack(void* info)
{
	CM_CALL_HANDLE handle;
	CALL_STATE state;

     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessEndCallIdAndGoBack));

	handle = DeriveCallHandle(info);
	state = GetCurrentState();
	mmi_trace(g_sw_CC,"MMI_Call: Func: %s cause=%d, handle=%d", __FUNCTION__,(U16)((MMI_RELEASE_IND*)info)->cause, handle);
	if (GetMMIStructIndexof(handle) == -1)
	{
		mmi_trace(g_sw_CC,("MMI_Call: call already dropped in ProcessEndCallIdAndGoBack\n"));
		return CM_CALL_SUCCESS;
	}

   SetCallEndCause ((U16)((MMI_RELEASE_IND*)info)->cause);

	if (GetCallDirFlag(handle) == CM_CALL_MO_STK)
	{
		if (state == CM_OUTGOING_STATE)
			SATCallExitCause(SAT_CALL_DISCONNECT_BEFORE_CONNECT);
		else if (state == CM_ACTIVE_STATE || state == CM_HOLD_STATE)
			SATCallExitCause(SAT_CALL_DISCONNECT_AFTER_CONNECT);
	}

	state = GetCallState(handle);
	

     /* when there's pending req on the dropped call, sync all call state with PS */
     if ((GetCallflag(handle, TRUE) & (~CM_HANGUP_REQUESTED)) != 0)
		SyncCallList();
     
	mmi_trace(g_sw_CC,"MMI_Call:  state=%d", state);
	switch (state)
	{
	case CM_INCOMING_STATE:
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ProcessEndCallIdAndGoBack1));

		SetCallEndCause (0);
		ProcessIncomingEvents (CM_PS_INCOMING_CALL_DROPPED, info);
		return CM_CALL_SUCCESS;

	case CM_OUTGOING_STATE:
     		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ProcessEndCallIdAndGoBack2));

		LogCallInfoForCallHistory(GetOutgoingCallHandle());
		GetEndTimeAndLogUnconnectedMOCall();

		if(GetDropRequestFlag() == FALSE)  //guojian added for 10225
		{
			if (IsRedialSet())
			{
				SetRedialNumber();
				mmi_trace(g_sw_SMS,"MMI_Call: Func: %s GetDropRequestFlag()=%d,GetCurrentState()=%d", __FUNCTION__,GetDropRequestFlag(),GetCurrentState());

				OutgoingProcessCMFailureEvent(CM_PS_CALLCONNECTED, GetCallEndCause());
				/* SetCallEndCause(0); */
				SetCallEndedBeforeConnFlag(FALSE);
				return CM_CALL_SUCCESS;
			}
		}

		SetCallState(handle,CM_IDLE_STATE,TRUE);
		UpdateStateMachine();
		if (GetCurrentState() == CM_OUTGOING_STATE)
		{
		   SetCurrentState (GetPreviousState());
		   SetPreviousState (CM_OUTGOING_STATE);
		}

		EntryScr1004NotifyEndCallDuration();
		return CM_CALL_SUCCESS;

	case CM_HOLD_STATE:
	     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ProcessEndCallIdAndGoBack4));

		mmi_trace(g_sw_CC,"MMI_Call: Func: %s CM_HOLD_STATE  GetCurrentState()=%d", __FUNCTION__,GetCurrentState());
		if (GetCurrentState() == CM_OUTGOING_STATE)
		{
			LogCallInfoForCallHistory(handle);
			GetEndTimeAndNotifyEndCallDuration(handle);
			SetCallState(handle,CM_IDLE_STATE,TRUE);
			if(GetTotalHoldCallCount()==GetTotalCallCount())
			{
				SetPreviousState(CM_OUTGOING_STATE);
				SetCurrentState(CM_HOLD_STATE);
			}
			return CM_CALL_SUCCESS;
		}
		SetCallflag (handle, CM_HANGUP_REQUESTED ,TRUE);
		OutgoingProcessCMEvent(CM_PS_HANGUPSUC,(void*)&handle);
		return CM_CALL_SUCCESS;

	case CM_ACTIVE_STATE:
     		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ProcessEndCallIdAndGoBack5));

		mmi_trace(g_sw_CC,"MMI_Call: Func: %s-CM_ACTIVE_STATE-", __FUNCTION__);
		SetCallflag (handle, CM_HANGUP_REQUESTED ,TRUE);
		OutgoingProcessCMEvent(CM_PS_HANGUPSUC,(void*)&handle);
		return CM_CALL_SUCCESS;

	default :
		PRINT_INFORMATION(("MMI_Call:(ERROR)<<** State Machine ** Not valid call state for handle>>\n"));
		return CM_CALL_FAILURE;

	}
	return CM_CALL_FAILURE;
}

/**************************************************************

	FUNCTION NAME		: ProcessShowSSEventNotification

  	PURPOSE				: This function derives SS information from PS and calls necessary
						 entry function to show string

	INPUT PARAMETERS	: data from PS

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ProcessShowSSEventNotification(void* info)
{
	DeriveSSEvent(info);
	EntryScrShowSSEvent();
}

/**************************************************************

	FUNCTION NAME		: ProcessAocIndication

  	PURPOSE				: This function is called by the state machine function in
						  to make call to protocol stack to get call list to sync with MMI

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
ACTION_RESULT ProcessAocIndication(void* info)
{
	U32 acmIndValue;
	U8 temp[8];

   	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessAocIndication));

	SetShowAOC(TRUE);

	memset(cashSign,0,sizeof(cashSign));
	cashSign[0]='$';

	acmIndValue = DeriveCcmValue(info);
	gSumACMValue+=acmIndValue;

	memset((S8*)temp,0,sizeof (temp));
	sprintf ((S8*)temp, "%d",(UINT)gSumACMValue);

	AnsiiToUnicodeString((S8*)AocBuffer,(S8*)temp);

	switch (GetCurrentState())
	{
		case CM_HOLD_STATE:
		case CM_ACTIVE_STATE:
			show_call_cost_UI(cashSign,(PU8)AocBuffer);
			return CM_CALL_SUCCESS;

		default :
			PRINT_INFORMATION(("<<** Cannot Process AOC indication in this state>>\n"));
			return CM_CALL_FAILURE;
	}
}

/**************************************************************

	FUNCTION NAME		: ReverseSpeechIndication(void)

  	PURPOSE				: This function is called reverse the speech

	INPUT PARAMETERS	: Info from the PS

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReverseSpeechIndication(void* info)
{
	pBOOL status;

	status = CheckStatusOfSpeech(info);
	MakePsReverseSpeechStatus(status);
}

/**************************************************************

	FUNCTION NAME		: ProcessStateCheckIncomingCall (void)

  	PURPOSE				: This function is called by the state machine function to
						  decide what to do before accepting an incoming call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ProcessStateCheckIncomingCall(void)
{
   	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessStateCheckIncomingCall));

   gAttempToAnswer = TRUE;

	if (GetTotalCallCount() > 1)
	{
		gAcceptIncoming = TRUE;
		SetHoldFlag();
		MakePsActiveHold((void*)PsCBackActiveCallsHeld);
	}
	else
	{
		MakePsCallAcceptIncoming((void*)PsCBackIncomingCallAccepted);
	}
}

/**************************************************************

	FUNCTION NAME		: PsCBackIncomingCallAccepted(void*)

  	PURPOSE				: This function is called back by the protocol stack when there
	                      Incoming Call is accepted

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackIncomingCallAccepted(void* info)
{
	U16 result;

	gAttempToAnswer = FALSE;

	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----CheckAtaSuccess(info,&result)=%d", __FUNCTION__, __FILE__, __LINE__,CheckAtaSuccess(info,&result));
	if (CheckAtaSuccess(info,&result))
	{
   		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_PsCBackIncomingCallAccepted1));
		ProcessIncomingEvents (CM_PS_INCOMING_CALL_CONNECTED, info);
	}
	else
	{
   		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_PsCBackIncomingCallAccepted2));
		/* accept fail doesn't imply call is disconnected */
		ProcessIncomingFailureEvents (CM_PS_INCOMING_CALL_CONNECTED, result);
	}
}

/**************************************************************

	FUNCTION NAME		: PsCBackCheckChldIncomingCallAccepted(void*)

  	PURPOSE				: This function is required to check the chld response and call
						  necessary function

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  accept incoming call via a chld request.

**************************************************************/
void PsCBackCheckChldIncomingCallAccepted(void* info)
{
	U16 cause;

	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	SetChldReqSent(CM_ACTION_NONE);

	if(CheckChldRequestSuccess(info, &cause))
	{
   		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_PsCBackCheckChldIncomingCallAccepted1));
		ProcessIncomingEvents (CM_PS_INCOMING_CALL_CONNECTED, info);
	}else
	{
   		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_PsCBackCheckChldIncomingCallAccepted2));
#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
		SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
      ProcessIncomingFailureEvents (CM_PS_INCOMING_CALL_CONNECTED, cause);
    }
}

/**************************************************************

	FUNCTION NAME		: PsCBackActiveCallsHeld(void*)

  	PURPOSE				: This function is called back by the protocol stack when there
	                      Active call is held

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackActiveCallsHeld(void* info)
{
   U16 result;

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackActiveCallsHeld));

	if (CheckChldRequestSuccess(info, &result))
	{
   		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_PsCBackActiveCallsHeld1));
		ProcessIncomingEvents (CM_PS_ACTIVE_CALL_HELD, info);
	}
	else
	{
		ResetHoldFlag();
		gAttempToAnswer = FALSE;
   		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_PsCBackActiveCallsHeld2, result));
		ProcessIncomingFailureEvents(CM_PS_ACTIVE_CALL_HELD,result);

#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
		SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
	}
}

/**************************************************************

	FUNCTION NAME		: ProcessCallsHeld(void* info)

  	PURPOSE				: This function is called by the state machine function
						  hold a call

	INPUT PARAMETERS	: any information associated with held call

	OUTPUT PARAMETERS	: result of execution of function which may be success or failure

	RETURNS				: void

 

**************************************************************/
ACTION_RESULT ProcessCallsHeld(void* info)
{
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessCallsHeld));

	if(GetTotalCallCount()>0)
	{
		SyncCallList();
	}

   gAttempToAnswer = FALSE;

	switch (GetPreviousState())
	{
	case CM_INCOMING_STATE:
   		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ProcessCallsHeld));

		SetCurrentState(CM_INCOMING_STATE);
		SetPreviousState(CM_HOLD_STATE);

		MakeHold();
		// GoBackHistory();//man
		return CM_CALL_SUCCESS;

	case CM_ACTIVE_STATE:
		if (gAcceptIncoming)
		{
			MakeHold();
			ProcessIncomingEvents (CM_PS_INCOMING_CALL_CONNECTED, info);
			gAcceptIncoming = FALSE;
		}
		else
		{
		   SetPreviousState(CM_HOLD_STATE);
   		SetCurrentState(CM_ACTIVE_STATE);

			//if(GetMissedCallFlag()==FALSE)
				DeleteScreenIfPresent(ITEMSCR_INCOMING_CALL);

			SetIncomingCallStateActive();
			// GoBackHistory(); //man
		}
		return CM_CALL_SUCCESS;

	case CM_HOLD_STATE:
		SetPreviousState(GetCurrentState());
		/*Guojian Add Start For 11799 11800 Ver: GreenStone  on 2009-6-5 17:32 */
	//	SetCurrentState(CM_HOLD_STATE);
		if (gAcceptIncoming)
		{
			MakeHold();
			ProcessIncomingEvents (CM_PS_INCOMING_CALL_CONNECTED, info);
			gAcceptIncoming = FALSE;
		}
		else
		{
			SetCurrentState(CM_HOLD_STATE);
			GoBackHistory();
		}
		/*Guojian Add End  For 11799 11800 Ver: GreenStone  on 2009-6-5 17:32 */

//		GoBackHistory();
	
		return CM_CALL_SUCCESS;
	default:
		return CM_CALL_FAILURE;
	}
	//return CM_CALL_FAILURE;
}

/**************************************************************

	FUNCTION NAME		: PsCBackIncomingCallRejected(void*)

  	PURPOSE				: This function is called back by the protocol stack when there
	                      Incoming Call is reject

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackIncomingCallRejected (void* info)
{

	SetChldReqSent(CM_ACTION_NONE);

	ProcessIncomingEvents (CM_PS_INCOMING_CALL_REJECTED, info);
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
}

/**************************************************************

	FUNCTION NAME		: ProcessAcceptIncomingCall(void* info)

  	PURPOSE				: This function is called by the state machine function accept
						  incoming call

	INPUT PARAMETERS	: any information associated with incoming call

	OUTPUT PARAMETERS	: result of execution of function which may be success or failure

	RETURNS				: void

 
**************************************************************/
ACTION_RESULT ProcessAcceptIncomingCall(void* info)
{
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessAcceptIncomingCall));

	SetCallHandleForStartTimeUpdate(GetIncomingCallHandle());    //setting which call is to update time 
	switch (GetPreviousState())
	{
		case CM_IDLE_STATE:
			SetPreviousState(GetCurrentState());
			SetCurrentState(CM_ACTIVE_STATE);
			SetIncomingCallStateActive();
			StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);//niej
			EntryScr1002ActiveCall();

			/* check if entering SMS application in call */
			if (IsScreenPresent(SCR_ID_MSG_MAIN_MENU_NO_MMS))
			mmi_frm_sms_delete_action_pending();
			DeleteUptoCmScreen();
			return CM_CALL_SUCCESS;

		case CM_ACTIVE_STATE:
			SetPreviousState(CM_INCOMING_STATE);
			SetCurrentState(CM_ACTIVE_STATE);
			SetIncomingCallStateActive();
			/* SS present, waiting call should not use GoBackToHistory
			to remove SS screen */
			if (IsScreenPresent(ITEM_SCR_USSN_MSG))
				DeleteScreenIfPresent(SCR_CM_ACTIVECALLSCREEN);
			
			if(IsScreenPresent(SCR_CM_ACTIVECALLSCREEN))
			{
				/* check if entering SMS application in call */
				if (IsScreenPresent(SCR_ID_MSG_MAIN_MENU_NO_MMS))
				mmi_frm_sms_delete_action_pending();
				GoBackToHistory(SCR_CM_ACTIVECALLSCREEN);
			}
			else
			{
				EntryScr1002ActiveCall();
			   DeleteScreenIfPresent(ITEM_SCR_INC_ACTIVE_OPN);
			   DeleteScreenIfPresent(ITEM_SCR_INCOMING_OPTIONS);
			   if (!IsScreenPresent(CM_SCR_MARKER))
				AddMarkerToHistory();
			}
			return CM_CALL_SUCCESS;

		case CM_HOLD_STATE:
			SetIncomingCallStateActive();
			if (GetTotalHoldCallCount()>0)
			{
				SetPreviousState(GetCurrentState());
				SetCurrentState(CM_ACTIVE_STATE);
				/* check if entering SMS application in call */
				if (IsScreenPresent(SCR_ID_MSG_MAIN_MENU_NO_MMS))
				mmi_frm_sms_delete_action_pending();
				//GoBackToHistory(SCR_CM_ACTIVECALLSCREEN);
			}
			else
			{
				SetPreviousState(GetCurrentState());
				SetCurrentState(CM_ACTIVE_STATE);
				/* check if entering SMS application in call */
				if (IsScreenPresent(SCR_ID_MSG_MAIN_MENU_NO_MMS))
				mmi_frm_sms_delete_action_pending();
				//GoBackToHistory(SCR_CM_ACTIVECALLSCREEN);
			}

			/* SS present, waiting call should not use GoBackToHistory
			to remove SS screen */
			if (IsScreenPresent(ITEM_SCR_USSN_MSG))
			{
				DeleteScreenIfPresent(SCR_CM_ACTIVECALLSCREEN);
				EntryScr1002ActiveCall();
			   DeleteScreenIfPresent(ITEM_SCR_INC_ACTIVE_OPN);
			   DeleteScreenIfPresent(ITEM_SCR_INCOMING_OPTIONS);				
			}
			else
				GoBackToHistory(SCR_CM_ACTIVECALLSCREEN);

			return CM_CALL_SUCCESS;

		case CM_INCOMING_STATE:
		case CM_OUTGOING_STATE:
			SetPreviousState(CM_INCOMING_STATE);
			SetCurrentState(CM_ACTIVE_STATE);
			SetIncomingCallStateActive();
      StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);//niej
			if(IsScreenPresent(SCR_CM_ACTIVECALLSCREEN))
			{
				/* check if entering SMS application in call */
				if (IsScreenPresent(SCR_ID_MSG_MAIN_MENU_NO_MMS))
				mmi_frm_sms_delete_action_pending();
				GoBackToHistory(SCR_CM_ACTIVECALLSCREEN);
			}
			else
			{
				EntryScr1002ActiveCall();
				/* check if entering SMS application in call */
				if (IsScreenPresent(SCR_ID_MSG_MAIN_MENU_NO_MMS))
				mmi_frm_sms_delete_action_pending();
				DeleteUptoCmScreen();
			}
			return CM_CALL_SUCCESS;

		default:
			return CM_CALL_FAILURE;
	}
}

/**************************************************************

	FUNCTION NAME		: PsCBackselectedCallEnded(void*)

  	PURPOSE				: This function is called back by the protocol stack when there
	                      selected active call ends

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackselectedCallEnded(void* info)
{
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif

	gMyCallHandle = DeriveCallHandle(info);
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackselectedCallEnded,  gMyCallHandle));
	ProcessIncomingEvents (CM_PS_ACTIVE_CALL_DISCONNECTED, &gMyCallHandle);
}

/**************************************************************

	FUNCTION NAME		: PsCBackActiveCallEndedHeldRetrieved(void*)

  	PURPOSE				: This function is called back by the protocol stack when active
  							  call ended for retrieve held

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackActiveCallEndedHeldRetrieved(void *info)
{
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackActiveCallEndedHeldRetrieved));
	gMyCallHandle = DeriveCallHandle(info);

	ProcessIncomingEvents (CM_PS_ALL_ACTIVE_CALL_DISCONNECTED, &gMyCallHandle);
}

/**************************************************************

	FUNCTION NAME		: ProcessEndAllActiveCall (void* info)

  	PURPOSE				: This function is called by the state machine function to
						  end all active call

	INPUT PARAMETERS	: Call Handle

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
ACTION_RESULT ProcessEndAllActiveCall(void* info)
{
	CM_CALL_HANDLE* handle;

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessEndAllActiveCall));

	handle = (CM_CALL_HANDLE*)info;
	 
	if ((*handle) == -1)
	{
		PRINT_INFORMATION(("\nBad call handle -1 in EndAllActiveCall\n"));
		return CM_CALL_FAILURE;
	}
	/* dropped call is already released due to rel complete */
	else if (GetMMIStructIndexof(*handle) == -1)
	{
		PRINT_INFORMATION(("\ncall already dropped in EndAllActiveCall\n"));
		return CM_CALL_SUCCESS;
	}
	 
	LogCallInfoForCallHistory(*handle);
	GetEndTimeAndNotifyEndCallDuration(*handle);
	SetCallState (*handle, CM_IDLE_STATE, TRUE);
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: CBackHangupAllActive

  	PURPOSE				: Hangs Up all Active Calls

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void CBackHangupAllActive(void)
{
	U16 holdCall;

	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	SetChldReqSent(CM_ACTION_NONE);

	/*if (gHold2End)
	{
		gHold2End = FALSE;
		return;
	}*/

	holdCall = GetTotalHoldCallCount();
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_CBackHangupAllActive, holdCall));

	if ((GetCurrentState() != CM_INCOMING_STATE) && !(holdCall >= 1))
	{
		GetOutOfCMApplication();
		return;
	}
	else if (holdCall >= 1)
	{
		return;
	}
}

/**************************************************************

	FUNCTION NAME		: CBackHangupAllActiveAcceptOther

  	PURPOSE				: Hangs Up all Active Calls Except Incoming

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void CBackHangupAllActiveAcceptOther(void *info)
{
   U16 holdCall, result;

	SetChldReqSent(CM_ACTION_NONE);

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
   SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
   
   if (CheckChldRequestSuccess(info, &result))
	{
	holdCall = GetTotalHoldCallCount();
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_CBackHangupAllActiveAcceptOther, holdCall));

	if (holdCall > 0)
		ProcessIncomingEvents (CM_PS_HELD_CALL_ACTIVE,NULL);
}
   else
   {
      /* fail to hold and accept => state not changed */
      ResetAllHangupFlag();
      if (GetTotalCallCount()>0)
      ShowCallManagementErrorMessage(result);
   }
}

/**************************************************************

	FUNCTION NAME		: ProcessHold2Active (void* info)

  	PURPOSE				: This function is called by the state machine function to
						  make hold call active

	INPUT PARAMETERS	: Any information associated with Held Call

	OUTPUT PARAMETERS	: ACTION_RESULT if the function was a success or failure

	RETURNS				: void

 

**************************************************************/
ACTION_RESULT ProcessHold2Active(void* info)
{
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessHold2Active));

  	if(GetTotalCallCount()>0)
	{
		SyncCallList();
	}

	UpdateStateMachine();
	MakeRetrieve();

	mmi_trace(g_sw_CC,"MMI_Call: Func: %s GetCurrentState=%d", __FUNCTION__, GetCurrentState());
	switch (GetCurrentState())
	{
	case CM_INCOMING_STATE:
		SetPreviousState (CM_ACTIVE_STATE);
		GoBackHistory();
		return CM_CALL_SUCCESS;

	case CM_ACTIVE_STATE:
		SetPreviousState (CM_HOLD_STATE);
		GoBackToHistory(SCR_CM_ACTIVECALLSCREEN);
		return CM_CALL_SUCCESS;

	case CM_HOLD_STATE:
	//	EntryScr1006NotifyRetrieveSucess();
		if((GetTotalCallCount()==GetTotalActiveCallCount())&&(GetTotalHoldCallCount()==0))
		{
			SetPreviousState(GetCurrentState());
		 	SetCurrentState(CM_ACTIVE_STATE);
		}
		return CM_CALL_SUCCESS;
	default:
		PRINT_INFORMATION(("File: [%s]  Line: [%d] <<Problem in Hold To active>>\n",__FILE__,__LINE__));
		return CM_CALL_FAILURE;
	}
	//return CM_CALL_FAILURE;
}

/**************************************************************

	FUNCTION NAME		: ProcessCCBSDialRequest(void* info)

  	PURPOSE				: This function is called by the state machine function
						  to dial CCBS call

	INPUT PARAMETERS	: any information associated with ccbs

	OUTPUT PARAMETERS	: result of execution of function which may be success or failure

	RETURNS				: void

 

**************************************************************/
ACTION_RESULT ProcessCCBSDialRequest(void* info)
{
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessCCBSDialRequest));
	MakePsActiveHold((void*)CCBSDialResponse);
	OutgoingProcessCMEvent(CM_PS_SHOW_OUTGOING_CALL_SCREEN,NULL);
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: CCBSDialResponse(void*)

  	PURPOSE				: This function is called back by the protocol
						 CCBS Dial response is recieved

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CCBSDialResponse(void* info)
{
	U16 result;

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_CCBSDialResponse));

	SetCCBSFlag(FALSE);
	if (CheckChldRequestSuccess(info, &result))
	{
  		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_CCBSDialResponse1));
	}
	else
	{
  		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_CCBSDialResponse2));
		ProcessIncomingFailureEvents (CM_PS_CCBS_INVOKE_DIAL, result);
	}
}

/**************************************************************

	FUNCTION NAME		: ProcessCCBSRejectRequest(void* info)

  	PURPOSE				: This function is called by the state machine function
						  to reject CCBS call

	INPUT PARAMETERS	: any information associated with ccbs

	OUTPUT PARAMETERS	: result of execution of function which may be success or failure

	RETURNS				: void

 

**************************************************************/
ACTION_RESULT ProcessCCBSRejectRequest(void* info)
{
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ProcessCCBSRejectRequest));
	MakePsSendUDUB((void*)PsCBackCCBSRejectResponse);
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: PsCBackCCBSRejectResponse(void*)

  	PURPOSE				: This function is called back by the protocol
							stack when USSD data is recieved by n/w

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackCCBSRejectResponse(void* info)
{
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_PsCBackCCBSRejectResponse));

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
	ProcessIncomingEvents (CM_PS_INCOMING_CALL_REJECTED, info);
}

/**************************************************************

	FUNCTION NAME		: AcceptCallForAT

  	PURPOSE				: Accept Incoming Call from AT Command

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void AcceptCallForAT(S16 handle)
{
	StopIncomingCallIndication();
}

#define PHB_PICTURE_TYPE_PIC_FILE                1

/*void AlmDisableExpiryHandler(void)
{
}*/		//delete by liuxn 060616
// zrx add end
#ifdef __MMI_MT_CALL_SPEAKER_MUTE__
/*Guojian Add Start For 10823 Ver: GreenStone  on 2008-12-18 15:15 */
void SetSpeakerMuteFlag(BOOL bMute)
{
	isSpeakerMute = bMute;
}

BOOL GetSpeakerMuteFlag()
{
	return isSpeakerMute;
}

void ResumeSpeaker()
{
	if(GetSpeakerMuteFlag())
	{
	 	SetSpeakerMuteFlag(FALSE);
		MuteOffSpeaker();
		mmi_trace(g_sw_CC," Func: %s restore ring volume", __FUNCTION__);

	}
}

void PsMuteIncommingCall()
{
#if 0
    if (GetSpeakerMuteFlag())
    {
		mmi_trace(g_sw_CC,"guoj++ Func: %s Unmute", __FUNCTION__);

		SetSpeakerMuteFlag(FALSE);
	//	SetRingerVolume(GetRingVolumeLevel());
		MuteOffSpeaker();
        ChangeRightSoftkey(STR_MITEM_INC_ACT_OPT_MUTE, 0);
    }
    else
#endif /* Guoj delete. It's not used now at 2009-7-29 */
	COS_Sleep(200);
	mmi_trace(g_sw_CC,"MMI_Call: Func: %s Mute", __FUNCTION__);
	SetSpeakerMuteFlag(TRUE);
	MuteOnSpeaker();
	VibratorOff(); 
}
/*Guojian Add End  For 10823 Ver: GreenStone  on 2008-12-18 15:15 */

void PsMuteorRejectIncommingCall()
{
    if (GetSpeakerMuteFlag()) //in mute, reject call
    {
		KbCBackCallIncomingRejected();
    }
    else // mute call
    {
		PsMuteIncommingCall();
		ChangeRightSoftkey(INC_SCR_RSK_STR, 0);
    }
	
}
#endif
/**************************************************************

	FUNCTION NAME		: EntryScrIncomingCallEvent(void)

  	PURPOSE				: This function is the entry screen for the incoming call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrIncomingCallEvent(void)
{
    MMI_ANSWERING_MODE *AnsMode;
    PU8 guiBuffer;
    PU8 tmpName;
    S8* imgPath=NULL;
    CM_CALL_HANDLE tmp;
    U16 notification_str_id = 0;
    U16 imgId;
    BOOL  needPlayRing = MMI_FALSE;
#ifdef __MMI_INCOMING_CALL_VIDEO__  
    BOOL	play_now = MMI_FALSE;
#endif
    U8 display_type = WGUI_CATE_MOMT_RES_TYPE_IMAGE_ID;

#ifdef __MMI_TTS_FEATURES__ 
    char temp[128];
    char readText[256];
#endif
    
   StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);//niej 2007-11-21
   AnsMode = GetAnswerMode();

	mmi_trace(g_sw_CC, "MMI_Call: Func: %s AnsMode=%d", __FUNCTION__, AnsMode);
#ifdef __MMI_RUNNING_LED__
    StartLEDPatternMusicRunning();
#endif

   EntryNewScreen(ITEMSCR_INCOMING_CALL, ExitScrIncomingCallEvent, NULL, NULL);

   guiBuffer = GetCurrGuiBuffer(ITEMSCR_INCOMING_CALL);

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_EntryScrIncomingCallEvent));

	ClearKeyEvents();
	DeleteScreenIfPresent(CM_SCR_RDL_TIME);

	//TurnOnBacklight(1);//qiff del for greenstone:bug:9543

   if (guiBuffer == NULL)
   {
   		mdi_audio_suspend_background_play();
   }

   if( IS_LOW_BATTERY() )
   {
		 
		ChgrPlayLowBatTone();
   }

	if((GetTotalCallCount()==1) && (CM_INCOMING_STATE==GetCurrentState()))
	{
		if (GetCallWaitFlag()&&!GetNoTwiceRingFlag())
		{
  			MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_EntryScrIncomingCallEvent));
			SetWait2RingCallFlag(TRUE);
			SetNoTwiceRingFlag(TRUE);
         needPlayRing = MMI_TRUE;
	   }
	}
   if (needPlayRing == MMI_TRUE && gAttempToAnswer == MMI_FALSE)
     ShowIncomingCallIndication();

#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
        notification_str_id = STRING_MTPNP_INCOMING_SIM1 + MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL);
#endif /* defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_) */


	TRACE_FMGR_FUNCTION();
	tmp = GetIncomingCallHandle();
	#ifdef __MMI_NUMBER_LOCATION__
	tmpName = GetCallNameWithAddress(tmp);
	#else
	tmpName = GetCallName(tmp);
	#endif
	imgId = GetCallImgId(tmp);
   gdispnameForSubLCD=tmpName;
	mmi_trace(g_sw_PHB, "imgId = %d", imgId);

#ifdef __MMI_TTS_FEATURES__ 

	//如果开启则直接播放，否则不进行播放。
	if(g_tts_setting.curTTSRingIncommingCallStatus == 0)
	{
		needPlayRing = FALSE;
		memset(readText, 0, 256);
		UCS2Strcpy(readText, (const char *) tmpName);
		//AnsiiToUnicodeString(temp, "给您来电话了");
		UCS2Strcat(readText, GetString(STR_ID_TTS_CALL_CONTENT));
		ejTTS_PlayToPCM(readText, UCS2Strlen(readText)*2);
	}
#endif

#if defined(__MMI_PHB_CALL_SHOW_PICTURE_FROM_FILE__)
   if(imgId == PHB_PICTURE_TYPE_PIC_FILE) 
	{
		imgPath = GetCallImgPath(tmp);
        if (imgPath == NULL)
        {
            imgId = IMG_PHB_DEFAULT;
	     display_type = WGUI_CATE_MOMT_RES_TYPE_IMAGE_ID;
        }
        else
        {
            display_type = WGUI_CATE_MOMT_RES_TYPE_IMAGE_FILE;
            imgId = 0;
        }
	}
#endif

	 
	if (GetDisconnectingCallHandle() != -1)
	{
      /* show sub before main LCD */
      ShowSubLCDScreen(EntrySubLcdScrIncomingCallEvent);

		//imgId = GetImgIdofState(CM_DISCONNECTING_STATE);
#ifdef __MMI_INCOMING_CALL_VIDEO__
		if ((imgId & 0x8000) || ((imgId & 0x3fff) >= VDO_ID_PHB_MTCALL_1) )
		   imgId = IMG_PHB_DEFAULT;
#endif		
		if (display_type == WGUI_CATE_MOMT_RES_TYPE_IMAGE_FILE)
		   ShowCategory18Screen(0,0,0,0,0,notification_str_id, tmpName, NULL, /*INC_SCR_NFY_STR*/ IMG_PHB_DEFAULT, imgPath, TRUE, guiBuffer);
		else
		   ShowCategory18Screen(0,0,0,0,0,notification_str_id, tmpName, NULL, /*INC_SCR_NFY_STR*/ imgId, NULL, TRUE, guiBuffer);
	}
	else
	 
	{
#ifdef __MMI_INCOMING_CALL_VIDEO__  
// dingjian 20080409 Temp
	mmi_trace(g_sw_PHB, "MMI_Call: (imgId & 0x8000)  = %d, ((imgId & 0x3fff) >= VDO_ID_PHB_MTCALL_1)=%d", (imgId & 0x8000) ,((imgId & 0x3fff) >= VDO_ID_PHB_MTCALL_1));

      if ((imgId & 0x8000) || ((imgId & 0x3fff) >= VDO_ID_PHB_MTCALL_1))
      {
         /* use video for display */
         /* user defined video file or system default video resource */
         BOOL video_sound = MMI_FALSE;
         BOOL video_update = MMI_FALSE;

         if (imgId & 0x8000)
         {
            imgPath = GetCallVideoPath(tmp);
                if (imgPath == NULL)
                {
                imgId = IMG_PHB_DEFAULT;
            }
            #ifdef __MMI_SWFLASH__
                else if (imgId & 0x2000)
                {
                    display_type = WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE;
                    imgId = 0;
                }
            #endif /* __MMI_SWFLASH__ */
            else
            {
                    display_type = WGUI_CATE_MOMT_RES_TYPE_VIDEO_FILE;
                    imgId = 0;
                }
            }
         else
         {
            imgPath = NULL;
            imgId &= 0x3fff;
#ifdef __MMI_SWFLASH__ /* Check it's default sw flash or video */
		if (imgId >= MFH_ID_PHB_MTCALL_1)
		{
			display_type = WGUI_CATE_MOMT_RES_TYPE_SWFLASH_ID;
		}
		else
#endif /* __MMI_SWFLASH__ */
		{
                display_type = WGUI_CATE_MOMT_RES_TYPE_VIDEO_ID;
		}
         }
#ifdef __MMI_CLAMSHELL__         
         /* show sub before main LCD */
         if (IsClamClose())
            /* show animated sub LCD */
            ForceSubLCDScreen(EntrySubLcdScrIncomingCallEvent);
         else
#endif            
         {
            /* show static sub LCD */
            video_update = MMI_TRUE;
            ForceSubLCDScreen(EntrySubLcdScrStaticIncomingCallEvent);
         }

         if (cm_p->alert_info.RingToneId == CM_RINGTONE_VIDEO)
            video_sound = MMI_TRUE;

         if ((cm_p->alert_info.AlertType == MMI_RING) || 
             (cm_p->alert_info.AlertType == MMI_VIBRATION_AND_RING) ||
             ((cm_p->alert_info.AlertType == MMI_VIBRATION_THEN_RING) &&
              (GetRingAfterVibFlag() == TRUE)))
				play_now = MMI_TRUE;
     
// Caller Video
         ShowCategory17Screen(0,											/* title_id */
							/*STR_GLOBAL_OPTIONS*/STR_MITEM_INC_OPT_ANSWER,					/* left_softkey */
							/*IMG_GLOBAL_OPTIONS*/0,					/* left_softkey_icon */
							INC_SCR_RSK_STR,						/* right_softkey */
							0,											/* right_softkey_icon */
							notification_str_id,											/* NotificationStringId */ 
							tmpName,									/* NameOrNumber */
							NULL,										/* IP_Number */
							/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-10 17:31 */
							IMG_PHB_DEFAULT,
							/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-10 17:31 */
							imgId,										/* resource_id */
							imgPath,										/* resource_filename */
							display_type,
							0,											/* repeat_count, video only, repeat_count, 0 = infinite */
							video_update,										/* is_visaul_update, video only, update to LCM or not */
							video_sound,										/* is_video_aud, video only, play video's audio */
							play_now,
							guiBuffer);								/* history_buffer */
      }
      else /* use image for display */
#endif      
      {
         /* show sub before main LCD */
         ShowSubLCDScreen(EntrySubLcdScrIncomingCallEvent);
      
// Caller Picture
         /* display image from file */
         if(display_type == WGUI_CATE_MOMT_RES_TYPE_IMAGE_FILE)
			ShowCategory18Screen(0,/*STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS*/STR_MITEM_INC_OPT_ANSWER, 0, INC_SCR_RSK_STR,
			0,notification_str_id, tmpName, NULL, IMG_PHB_DEFAULT, imgPath , FALSE, guiBuffer);
         /* display image from resource */
		else
			ShowCategory18Screen(0,/*STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS*/STR_MITEM_INC_OPT_ANSWER, 0,INC_SCR_RSK_STR,
                  0,notification_str_id, tmpName, NULL, imgId, NULL, FALSE, guiBuffer);
      }
	}
   
	 
	if (GetDisconnectingCallHandle() != -1)
	{
		SetKeyHandler(ReleaseCall,KEY_END,KEY_EVENT_DOWN);
		return;
	}
	else
	 
	{
	if (GetTotalCallCount() > 1)
	{
		/* other call existed */
		/* soft key */
		//SetLeftSoftkeyFunction (EntryScrIncomingOptions, KEY_EVENT_UP);//fixbug 16480-1-2 by wang
		SetLeftSoftkeyFunction(KbCBackCallIncomingAccepted, KEY_EVENT_UP);
		SetRightSoftkeyFunction (KbCBackCallIncomingRejected, KEY_EVENT_UP);

		/* SEND, END key */
		/* when have both active and hold calls, can't accept this incomming call, Adp alyer will return "invalid command" */
//		if (GetTotalActiveCallCount() == 0 || GetTotalHoldCallCount() == 0)  																				//
			SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND, KEY_EVENT_DOWN);
#ifdef __SENDKEY2_SUPPORT__
			SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND1, KEY_EVENT_DOWN);
			SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND2, KEY_EVENT_DOWN);
#endif

#ifdef __MMI_CLAMSHELL__
	   SetKeyHandler(KbCBackCallIncomingRejected,KEY_END,KEY_EVENT_DOWN);
#else
	   SetKeyHandler(KbCBackCallIncomingRejected,KEY_END,KEY_EVENT_DOWN);   //fixbug 16480-1-2 by wang
//		SetKeyHandler(KbCBackEndAllCallsExceptIncoming,KEY_END,KEY_EVENT_DOWN);
#endif

		/* group key */
		if (!EmergencyCallPresent())
		SetGroupKeyHandler(EnterScreenAndHandleDigit,(PU16)CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);

		/* side key */
		if (GetTotalActiveCallCount() > 0)
			SetSideVolumeKeysForCM();
	}
	else
	{
		/* no other call */
		/* group key */
			if (AnsMode->anyKey == TRUE)

			#ifdef __MMI_BLACKBERRY_QWERTY__
				SetGroupKeyHandler(KbCBackCallIncomingAccepted,(PU16)PresentAllKeys_qwert,TOTAL_KEYS_QWERT,KEY_EVENT_DOWN);
			#else
				SetGroupKeyHandler(KbCBackCallIncomingAccepted,(PU16)PresentAllKeys,TOTAL_KEYS,KEY_EVENT_DOWN);
			#endif
			else
				SetGroupKeyHandler(InvisibleDigitHandler,(PU16)CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);

		/* soft key */
		//SetLeftSoftkeyFunction (EntryScrIncomingOptions, KEY_EVENT_UP);
		SetLeftSoftkeyFunction(KbCBackCallIncomingAccepted, KEY_EVENT_UP);
	/*Guojian Modify Start For 10823 Ver: GreenStone  on 2008-12-18 15:16 */
#ifdef __MMI_MT_CALL_SPEAKER_MUTE__
		SetRightSoftkeyFunction (PsMuteorRejectIncommingCall, KEY_EVENT_UP);
		if (GetSpeakerMuteFlag())
		{
			ChangeRightSoftkey(INC_SCR_RSK_STR, 0);
		}
		else
		{
			ChangeRightSoftkey(STR_MITEM_INC_ACT_OPT_MUTE, 0);
		}
#else
		SetRightSoftkeyFunction (KbCBackCallIncomingRejected, KEY_EVENT_UP);
#endif
	/*Guojian Modify End  For 10823 Ver: GreenStone  on 2008-12-18 15:16 */


		/* SEND, END key */
		SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND,KEY_EVENT_DOWN);
#ifdef __SENDKEY2_SUPPORT__
		SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND1, KEY_EVENT_DOWN);
		SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND2, KEY_EVENT_DOWN);
#endif

		SetKeyHandler(KbCBackCallIncomingRejected,KEY_END,KEY_EVENT_DOWN);

		/* side key */
#ifdef __MMI_CLAMSHELL__		
#ifdef __MMI_SIDEKEY_SUPPRESS_RING__
      SetKeyHandler(StopIncomingCallIndication,KEY_VOL_DOWN,KEY_EVENT_DOWN);
		SetKeyHandler(KbCBackCallIncomingSuppressTone,KEY_VOL_DOWN,KEY_EVENT_UP);
		SetKeyHandler(KbCBackCallIncomingRejectedNoTone,KEY_VOL_DOWN,KEY_EVENT_LONG_PRESS);
#else
      SetKeyHandler(KbCBackCallIncomingAccepted,KEY_VOL_UP,KEY_EVENT_DOWN);
		SetKeyHandler(KbCBackCallIncomingRejected,KEY_VOL_DOWN,KEY_EVENT_DOWN);
#endif
#endif
	}
	
	}
	mmi_trace(1," EntryScrIncomingCallEvent --> needPlayRing %d, gAttempToAnswer %d",needPlayRing, gAttempToAnswer);
   
    TurnOnBacklight(1); //qiff add for greenstone:bug:9543

	if (gAttempToAnswer == TRUE)
	   ClearAllKeyHandler();
}

/**************************************************************

	FUNCTION NAME		: ExitScrIncomingCallEvent(void)

  	PURPOSE				: This function is the exit screen for the incoming call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrIncomingCallEvent(void)
{
	history_t   scr101;
	U16 nHistory = 0;
#ifdef __MMI_RUNNING_LED__
    StopLEDPatternMusicRunning();
#endif
	if(-1!=GetIncomingCallHandle())
	{
		pfnUnicodeStrcpy((S8*)scr101.inputBuffer, (PS8)&nHistory);
		scr101.scrnID = ITEMSCR_INCOMING_CALL;
		scr101.entryFuncPtr = EntryScrIncomingCallEvent;
		GetCategoryHistory(scr101.guiBuffer);
		AddHistory(scr101);
	}
}

/**************************************************************

	FUNCTION NAME		: EntrySubLcdScrIncomingCallEvent

  	PURPOSE				: Entry screen for Sub Lcd Incoming Call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void EntrySubLcdScrIncomingCallEvent(void)
{
	ExecSubLCDCurrExitHandler();

	ShowCategory311Screen(gdispnameForSubLCD,IMG_CM_SUBLCD_CALLING,NULL);
	SetSubLCDExitHandler(ExitSubLcdScrIncomingCallEvent);
}

#ifdef __MMI_INCOMING_CALL_VIDEO__
/**************************************************************

	FUNCTION NAME		: EntrySubLcdScrStaticIncomingCallEvent

  	PURPOSE				: Entry screen for Sub Lcd Incoming Call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void EntrySubLcdScrStaticIncomingCallEvent(void)
{
	ExecSubLCDCurrExitHandler();

	ShowCategory332Screen(IMG_CM_STATE_CONNECTED);
	SetSubLCDExitHandler(ExitSubLcdScrStaticIncomingCallEvent);
}
#endif

/**************************************************************

	FUNCTION NAME		: ExitSubLcdScrIncomingCallEvent

  	PURPOSE				: Exit Handler for Sub Lcd Incoming Call Screen

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void ExitSubLcdScrIncomingCallEvent(void)
{
#ifdef __MMI_INCOMING_CALL_VIDEO__
	StopCategory17Video();
#endif

	if (-1!=GetIncomingCallHandle() &&
		!IsSubLCDScreenPresent(SUB_LCD_ITEMSCR_INCOMING_CALL))
	{
		SubLCDHistoryNode SubLCDHistory;

		SubLCDHistory.entryFuncPtr=EntrySubLcdScrIncomingCallEvent;
		AddSubLCDHistoryWithScrID(&SubLCDHistory,SUB_LCD_ITEMSCR_INCOMING_CALL);
	}
}

#ifdef __MMI_INCOMING_CALL_VIDEO__
/**************************************************************

	FUNCTION NAME		: ExitSubLcdScrStaticIncomingCallEvent

  	PURPOSE				: Exit Handler for Sub Lcd Incoming Call Screen

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void ExitSubLcdScrStaticIncomingCallEvent(void)
{
	StopCategory17Video();

	if (-1!=GetIncomingCallHandle() &&
		!IsSubLCDScreenPresent(SUB_LCD_ITEMSCR_INCOMING_CALL))
	{
		SubLCDHistoryNode SubLCDHistory;

		SubLCDHistory.entryFuncPtr=EntrySubLcdScrStaticIncomingCallEvent;
		AddSubLCDHistoryWithScrID(&SubLCDHistory,SUB_LCD_ITEMSCR_INCOMING_CALL);
	}
}
#endif

/**************************************************************

	FUNCTION NAME		: EntrySubLcdScrIncomingCallEvent

  	PURPOSE				: Entry screen for Incoming Call option menu when
  							  there are more than one calls

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void EntryScrIncomingMultipleOptions(void)
{
	PU8 	guiBuffer;
	static U8*	PopUpList[10];
	U16 	nNumofItem;		 /* Stores no of children in the submenu*/
	U16 	nStrItemList[10];/* Stores the strings id of submenus returned */
	U16 	menuId;
	S16 	nActiveCall;
	U16		nHeldCall;

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_EntryScrIncomingMultipleOptions));

	nActiveCall = GetTotalActiveCallCount();
	nHeldCall = GetTotalHoldCallCount();

	if (nActiveCall == 1 && nHeldCall == 0)
	{
		if (GetWapCallPresent())
			menuId = MITEM_INC_WAP_OPT_PSEUDO;
		else
			menuId = MITEM_INC_ACT_OPT_PSEUDO;
	}
	else if (nActiveCall == 0 && nHeldCall == 1)
	{
		menuId = MITEM_INC_HLD_OPT_PSEUDO;
	}
	else if (nActiveCall > 1 && nHeldCall == 0)
	{
		menuId = MITEM_IMA_OPT_PSEUDO;
	}
	else if (nActiveCall == 0 && nHeldCall > 1)
	{
		menuId = MITEM_INC_MHLD_OPT_PSEUDO;
	}
	else if (nActiveCall == 1 && nHeldCall == 1)
	{
		menuId = MITEM_IAH_OPT_PSEUDO;
	}
	else if (nActiveCall > 1 && nHeldCall > 0)
	{
		menuId = MITEM_IMAH_OPT_PSEUDO;
	}
	else if (nActiveCall > 0 && nHeldCall > 1)
	{
		menuId = MITEM_IAMH_OPT_PSEUDO;
	}
	else
		return;

	/* incoming call option menu will not enter other app, 
	no need to set CM flag */
	//SetCMScrnFlag(TRUE);

   EntryNewScreen(ITEM_SCR_INC_MACT_OPN, NULL,
                   EntryScrIncomingMultipleOptions, NULL);

	guiBuffer = GetCurrGuiBuffer(ITEM_SCR_INC_MACT_OPN);		/* Getting the Index from history  */
	SetParentHandler(menuId);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	nNumofItem = GetNumOfChild(menuId);
	GetSequenceStringIds(menuId,nStrItemList);

	ConstructHintsList(menuId, PopUpList);

	ShowCategory55Screen(STR_SCR1002_CAPTION, IMG_SCR1002_CAPTION,
						 STR_GLOBAL_OK, IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						nNumofItem,nStrItemList,
						(U8 **)PopUpList,0,guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	if (nActiveCall == 0 || nHeldCall == 0)
	{
		SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND,KEY_EVENT_DOWN);
#ifdef __SENDKEY2_SUPPORT__
		SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND1, KEY_EVENT_DOWN);
		SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND2, KEY_EVENT_DOWN);
#endif

	}
#ifdef __MMI_CLAMSHELL__
	SetKeyHandler(KbCBackCallIncomingRejected,KEY_END,KEY_EVENT_DOWN);
#else
	SetKeyHandler(KbCBackEndAllCallsExceptIncoming,KEY_END,KEY_EVENT_DOWN);
#endif

	SetSideVolumeKeysForCM();
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: EntryScrIncomingOptions(void)

  	PURPOSE				: This function is the entry screen for the incoming call options

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrIncomingOptions(void)
{
	U8* guiBuffer;
	s32 number_of_items=0;
	U16		list_of_items[32];

   EntryNewScreen(ITEM_SCR_INCOMING_OPTIONS, NULL,
                   EntryScrIncomingOptions, NULL);

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_EntryScrIncomingOptions));

	/* incoming call option menu will not enter other app, 
	no need to set CM flag */
	//SetCMScrnFlag(TRUE);

	guiBuffer = GetCurrGuiBuffer(ITEM_SCR_INCOMING_OPTIONS);		/* Getting the Index from history  */
	number_of_items = GetNumOfChild(MITEM_OPT_PSEUDO);
	GetSequenceStringIds(MITEM_OPT_PSEUDO, list_of_items);
	SetParentHandler(MITEM_OPT_PSEUDO);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory1Screen(STR_SCR1002_CAPTION, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  number_of_items,list_of_items,0,guiBuffer);
/* use category1 instead.
	ShowCategory101Screen(STR_OPT_PSEUDO, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MITEM_OPT_PSEUDO, guiBuffer);
*/
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(KbCBackCallIncomingRejected,KEY_END,KEY_EVENT_DOWN);
	SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND,KEY_EVENT_DOWN);
#ifdef __SENDKEY2_SUPPORT__
	SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND1, KEY_EVENT_DOWN);
	SetKeyHandler(KbCBackCallIncomingAccepted,KEY_SEND2, KEY_EVENT_DOWN);
#endif

	SetSideVolumeKeysForCM();
/*	
#ifdef __MMI_SIDEKEY_SUPPRESS_RING__
   SetKeyHandler(StopIncomingCallIndication,KEY_VOL_DOWN,KEY_EVENT_DOWN);
	SetKeyHandler(KbCBackCallIncomingSuppressTone,KEY_VOL_DOWN,KEY_EVENT_UP);
	SetKeyHandler(KbCBackCallIncomingRejectedNoTone,KEY_VOL_DOWN,KEY_EVENT_LONG_PRESS);
#else
   SetKeyHandler(KbCBackCallIncomingAccepted,KEY_VOL_UP,KEY_EVENT_DOWN);
	SetKeyHandler(KbCBackCallIncomingRejected,KEY_VOL_DOWN,KEY_EVENT_DOWN);
#endif
*/
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: EntryScrCcbsActivate(void)

  	PURPOSE				: This function is the entry screen for displaying CCBS Info to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrCcbsActivate(void)
{
   EntryNewScreen(ITEM_SCR_CCBS_IND, ExitScrCcbsActivate,
                   NULL, NULL);

	ShowCategory123Screen(0,0,STR_GLOBAL_YES,0,
							STR_GLOBAL_NO,0,
							STR_NFY_CCBS,IMG_GLOBAL_QUESTION,NULL);

	SetLeftSoftkeyFunction(ActivateCCBS,KEY_EVENT_UP);
	SetRightSoftkeyFunction(IgnoreCCBS,KEY_EVENT_UP);
	SetKeyHandler(IgnoreCCBS,KEY_END, KEY_EVENT_DOWN);

	if(GetTotalCallCount()==0)
	{
		playRequestedTone(CCBS_TONE);
		SetErrorToneFlag(TRUE);
	}
}

/**************************************************************

	FUNCTION NAME		: ExitScrCcbsActivate(void)

  	PURPOSE				: This function is the exit screen for CCBS info to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrCcbsActivate(void)
{
	if(GetErrorToneFlag()==TRUE)
	{
		stopRequestedTone(CCBS_TONE);
		SetErrorToneFlag(FALSE);
	}
}

/**************************************************************

	FUNCTION NAME		: EntryScrNotifyCCBSSuccess(void)

  	PURPOSE				: This is a notification which does not have history and used when the
						  ccbs activation is success with a timeout of CM_NOTIFY_TIMEOUT.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrNotifyCCBSSuccess(void)
{
   EntryNewScreen(SCR_ECT_NFY_TRNT, ExitScrNotifyCMPopUpTone,
                   NULL, NULL);

	ShowCategory62Screen(CCBS_ACTIVATED_STR,
				IMAGE_CCBS_ACTIVATED,NULL);

	if(GetTotalCallCount()==0)
	{
		playRequestedTone(SUCCESS_TONE_IN_CALL);
		SetErrorToneFlag(TRUE);
	}

	SetPOPUpFlag(TRUE);
	SetKeysForPOPUp();
	StartTimer(CM_NOTIFYDURATION_TIMER, CM_NOTIFY_TIMEOUT, GoBackfromNotifyScr);
}

/**************************************************************

	FUNCTION NAME		: EntryScrCCBSInvokeEvent(void)

  	PURPOSE				: This function is the entry screen for the CCBS Invoke

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrCCBSInvokeEvent(void)
{
	PU8 guiBuffer;
	PU8 tmpName;
	S8* imgPath = NULL;
	CM_CALL_HANDLE tmp;
	U16 imgId;
	BOOL needPlayRing = MMI_FALSE;

	EntryNewScreen(ITEMSCR_CCBS_INVOKE, NULL,
                   EntryScrCCBSInvokeEvent, NULL);

	TurnOnBacklight(1);

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_EntryScrCCBSInvokeEvent));

	if((GetTotalCallCount()==1) && (CM_INCOMING_STATE==GetCurrentState()))
	{
		if (GetCallWaitFlag())
		{
  			MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_EntryScrIncomingCallEvent));
			SetWait2RingCallFlag(TRUE);
			needPlayRing = MMI_TRUE;
		}
	}

	guiBuffer = GetCurrGuiBuffer(ITEMSCR_CCBS_INVOKE);
	tmp = GetIncomingCallHandle();
	#ifdef __MMI_NUMBER_LOCATION__
	tmpName = GetCallNameWithAddress(tmp);
	#else
	tmpName = GetCallName(tmp);
	#endif
	imgId = GetCallImgId(tmp);
#if defined(__MMI_PHB_CALL_SHOW_PICTURE_FROM_FILE__)
	if(imgId == 1) 
	{
		imgPath = GetCallImgPath(tmp);
		if(imgPath == NULL)
			imgId = IMG_PHB_DEFAULT;
	}
#endif

	if (GetTotalCallCount() > 1)
	{
		if(imgId == 1 )
			ShowCategory18Screen(0,STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS,
					STR_GLOBAL_BACK , IMG_GLOBAL_BACK,0,tmpName, NULL, 0, imgPath, FALSE, guiBuffer);					/*INC_SCR_NFY_CCBS_STR*/
		else
			ShowCategory18Screen(0,STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS,
					STR_GLOBAL_BACK , IMG_GLOBAL_BACK,0,tmpName, NULL, imgId, NULL, FALSE, guiBuffer);					/*INC_SCR_NFY_CCBS_STR*/

		SetLeftSoftkeyFunction(EntryScrCCBSInvokeMultipleOptions,KEY_EVENT_UP);

		if (GetTotalActiveCallCount()==0 || GetTotalHoldCallCount()==0)
		{
			SetKeyHandler(KbCBackCCBSDialCall,KEY_SEND,KEY_EVENT_DOWN);
#ifdef __SENDKEY2_SUPPORT__
			SetKeyHandler(KbCBackCCBSDialCall,KEY_SEND1, KEY_EVENT_DOWN);
			SetKeyHandler(KbCBackCCBSDialCall,KEY_SEND2, KEY_EVENT_DOWN);
#endif
		}

#ifdef __MMI_CLAMSHELL__
	   SetKeyHandler(KbCBackCCBSInvokeRejected,KEY_END,KEY_EVENT_DOWN);
#else
		SetKeyHandler(KbCBackEndAllCallsExceptIncoming,KEY_END,KEY_EVENT_DOWN);
#endif

		SetSideVolumeKeysForCM();
	}
	else
	{
		if(imgId == 1 )
			ShowCategory18Screen(0,STR_GLOBAL_DIAL,0,
						STR_GLOBAL_BACK , IMG_GLOBAL_BACK,0,	tmpName, NULL, 0, imgPath,FALSE, guiBuffer);					/*INC_SCR_NFY_CCBS_STR*/
		else
			ShowCategory18Screen(0,STR_GLOBAL_DIAL,0,
						STR_GLOBAL_BACK , IMG_GLOBAL_BACK,0,	tmpName, NULL, imgId, NULL,FALSE, guiBuffer);					/*INC_SCR_NFY_CCBS_STR*/
		SetLeftSoftkeyFunction(KbCBackCCBSDialCall,KEY_EVENT_UP);

		SetKeyHandler(KbCBackCCBSDialCall,KEY_SEND,KEY_EVENT_DOWN);
#ifdef __SENDKEY2_SUPPORT__
		SetKeyHandler(KbCBackCCBSDialCall,KEY_SEND1, KEY_EVENT_DOWN);
		SetKeyHandler(KbCBackCCBSDialCall,KEY_SEND2, KEY_EVENT_DOWN);
#endif

		SetKeyHandler(KbCBackCCBSInvokeRejected,KEY_END,KEY_EVENT_DOWN);

      	SetKeyHandler(KbCBackCCBSDialCall,KEY_VOL_UP,KEY_EVENT_DOWN);
		SetKeyHandler(KbCBackCCBSInvokeRejected,KEY_VOL_DOWN,KEY_EVENT_DOWN);
	}

	SetRightSoftkeyFunction(KbCBackCCBSInvokeRejected,KEY_EVENT_UP);
	SetGroupKeyHandler(EnterScreenAndHandleDigit,(PU16) CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);

  	gdispnameForSubLCD=tmpName;
 	ShowSubLCDScreen(EntrySubLcdScrIncomingCallEvent);

	if (needPlayRing == MMI_TRUE)
	   ShowIncomingCallIndication();
}

/**************************************************************

	FUNCTION NAME		: EntryScrCCBSInvokeMultipleOptions(void)

  	PURPOSE				: This function is the entry screen for the CCBS Invoke
  							  option menu when there are more than one calls

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrCCBSInvokeMultipleOptions(void)
{
	PU8 	guiBuffer;
	static U8*	PopUpList[10];
	U16 	nNumofItem;		 /* Stores no of children in the submenu*/
	U16 	nStrItemList[10];/* Stores the strings id of submenus returned */
	U16 	menuId;
	S16 	nActiveCall;
	U16		nHeldCall;

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_EntryScrCCBSInvokeMultipleOptions));

	nActiveCall = GetTotalActiveCallCount();
	nHeldCall = GetTotalHoldCallCount();

	if (nActiveCall == 1 && nHeldCall == 0)
	{
		menuId = MITEM_CCBS_ACT_OPT_PSEUDO;
	}
	else if (nActiveCall == 0 && nHeldCall == 1)
	{
		menuId = MITEM_CCBS_HLD_OPT_PSEUDO;
	}
	else if (nActiveCall > 1 && nHeldCall == 0)
	{
		menuId = MITEM_CCBS_MACT_OPT_PSEUDO;
	}
	else if (nActiveCall == 0 && nHeldCall > 1)
	{
		menuId = MITEM_CCBS_MHLD_OPT_PSEUDO;
	}
	else if (nActiveCall == 1 && nHeldCall == 1)
	{
		menuId = MITEM_CCBS_ACT_HLD_OPT_PSEUDO;
	}
	else if (nActiveCall > 1 && nHeldCall > 0)
	{
		menuId = MITEM_CCBS_MACT_HLD_OPT_PSEUDO;
	}
	else if (nActiveCall > 0 && nHeldCall > 1)
	{
		menuId = MITEM_CCBS_ACT_MHLD_OPT_PSEUDO;
	}
	else
		return;

	EntryNewScreen(ITEM_SCR_CCBS_ACT_MHLD_OPN, NULL,
                   EntryScrCCBSInvokeMultipleOptions, NULL);

	guiBuffer = GetCurrGuiBuffer(ITEM_SCR_CCBS_ACT_MHLD_OPN);
	SetParentHandler(menuId);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	nNumofItem = GetNumOfChild(menuId);

	GetSequenceStringIds(menuId,nStrItemList);
	ConstructHintsList(menuId, PopUpList);

	ShowCategory55Screen(STR_SCR1002_CAPTION, IMG_SCR1002_CAPTION,
						 STR_GLOBAL_OK, IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						nNumofItem,nStrItemList,
						(U8 **)PopUpList,0,guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	if (GetTotalActiveCallCount()==0 || GetTotalHoldCallCount()==0)
	{
			SetKeyHandler(KbCBackCCBSDialCall,KEY_SEND,KEY_EVENT_DOWN);
#ifdef __SENDKEY2_SUPPORT__
			SetKeyHandler(KbCBackCCBSDialCall,KEY_SEND1, KEY_EVENT_DOWN);
			SetKeyHandler(KbCBackCCBSDialCall,KEY_SEND2, KEY_EVENT_DOWN);
#endif

	}

#ifdef __MMI_CLAMSHELL__
	SetKeyHandler(KbCBackCCBSInvokeRejected,KEY_END,KEY_EVENT_DOWN);
#else
	SetKeyHandler(KbCBackEndAllCallsExceptIncoming,KEY_END,KEY_EVENT_DOWN);
#endif

	SetSideVolumeKeysForCM();
	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  EntryScrShowSSEvent
 * DESCRIPTION
 *  This function is the entry screen for displaying SS info to user
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryScrShowSSEvent(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* U8* guiBuffer; */
    U16 scrIdDisplay;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    scrIdDisplay = GetStrDisplayForId(gSSEvent);
    if (scrIdDisplay == 0)
    {
        return;
    }

    mmi_trace(g_sw_CC ,"MMI_Call: Func: %s gSSEvent=%d", __FUNCTION__, gSSEvent);
    EntryNewScreen(ITEM_SCR_SS_NFY, ExitScrShowSSEvent, NULL, NULL);

    SetNotifySSFlag(TRUE);

    // guiBuffer = GetCurrGuiBuffer(ITEM_SCR_SS_NFY);   /* Getting the Index from history  */

		ShowCategory62Screen(scrIdDisplay,IMG_GLOBAL_INFO,NULL);

    if (GetCurrentState() == CM_OUTGOING_STATE)
    {
        SetRightSoftkeyFunction(DropRequest, KEY_EVENT_UP);
        if (1 == GetTotalCallCount())
        {
            SetKeyHandler(DropRequest, KEY_END, KEY_EVENT_DOWN);
        }
        else
        {
            SetKeyHandler(HangupAll, KEY_END, KEY_EVENT_DOWN);
        }
    }

    SetLeftSoftkeyFunction(GoBackfromNotifyScr, KEY_EVENT_UP);

    SetPOPUpFlag(TRUE);
    SetKeysForPOPUp();
    StartTimer(CM_NOTIFYDURATION_TIMER, CM_NOTIFY_TIMEOUT, GoBackfromNotifyScr);
}

/**************************************************************

	FUNCTION NAME		: ExitScrShowSSEvent(void)

  	PURPOSE				: This function is the exit screen for for displaying SS info to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrShowSSEvent(void)
{
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_ExitScrShowSSEvent));
	StopTimer(CM_NOTIFYDURATION_TIMER);

	if(GetPOPUpFlag())
	{
		SetPOPUpFlag(FALSE);
		AdjustHistoryForCM();
	}

	SetNotifySSFlag(FALSE);

	if(GetCbackAfterSS())
	{
  		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ExitScrShowSSEvent1));

      if (GetCurrentState() == CM_INCOMING_STATE && IsScreenPresent(ITEMSCR_INCOMING_CALL))
		{
		   SetCbackAfterSS(NULL);
		     GoBackToHistory(ITEMSCR_INCOMING_CALL);
  			MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ExitScrShowSSEvent2));
		}
		else if((GetCbackAfterSS()==EntryScr1002ActiveCall)&&IsScreenPresent(SCR_CM_ACTIVECALLSCREEN))
		{
			GoBackToHistory(SCR_CM_ACTIVECALLSCREEN);
  			MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ExitScrShowSSEvent3));
		}
		else
		{
  			MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_ExitScrShowSSEvent4));
			(*GetCbackAfterSS())();
		}
	}
}

/**************************************************************

	FUNCTION NAME		: HandleCallDelfectDigitEntry()

  	PURPOSE				: This function is entered when a key
						  is pressed at the Call Deflect screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HandleCallDelfectDigitEntry(void)
{
	U16 Keycode,Keytype;

	memset (CallDeflectBuffer,0, sizeof(CallDeflectBuffer));

	GetkeyInfo(&Keycode,&Keytype);
	CallDeflectDigitKeyCode=Keycode;

	EntryCallDeflectDigitHandler();
}

/**************************************************************

	FUNCTION NAME		: CallDeflectDigitHandler()

  	PURPOSE				: Inputs the digits of a phone number for Call Deflection

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCallDeflectDigitHandler(void)
{
	byte *history_buffer;

	EntryNewScreen(ITEM_SCR_CALL_DEFLECT_DIGIT_HANDLER_ID,
	                ExitCallDeflectDigitHandler,
                   NULL, NULL);

	cm_p->history_info.IsSave = TRUE;

	history_buffer=GetCurrGuiBuffer(ITEM_SCR_CALL_DEFLECT_DIGIT_HANDLER_ID);

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_EntryCallDeflectDigitHandler));

	RegisterInputBoxEmptyFunction (CallDeflectScreenEmpty);
	RegisterInputBoxNotEmptyFunction (CallDeflectScreenNotEmpty);
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) )
	#if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
    mmi_idle_set_dual_sim_dialing_keypad_call_handler(CallDeflectDigitCall, CallDeflectDigitCall,NULL,NULL);
#endif
	#endif
#endif

	ShowCategory16Screen(CALL_DEFLECT_SEARCH_LSK,0,
					STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
					(U8*)CallDeflectBuffer, MAX_DIGIT,history_buffer);

	if(GetTotalCallCount () == 1)
		SetKeyHandler(KbCBackCallIncomingRejected,KEY_END,KEY_EVENT_DOWN);
	else
   {
#ifdef __MMI_CLAMSHELL__
      SetKeyHandler(KbCBackCallIncomingRejected,KEY_END,KEY_EVENT_DOWN);
#else
		SetKeyHandler(KbCBackEndAllCallsExceptIncoming,KEY_END,KEY_EVENT_DOWN);
#endif
   }

	if((CallDeflectDigitKeyCode>=KEY_0 && CallDeflectDigitKeyCode<=KEY_9) ||
		CallDeflectDigitKeyCode==KEY_STAR || CallDeflectDigitKeyCode==KEY_POUND)
		ExecuteCurrKeyHandler(CallDeflectDigitKeyCode,KEY_EVENT_DOWN);

	SetCategory16RightSoftkeyFunction(CheckBeforeGoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: ExitCallDeflectDigitHandler()

  	PURPOSE				: exit fn for Call Deflect digit handle

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCallDeflectDigitHandler(void)
{
	if(cm_p->history_info.IsSave)
	{
		history_t h;
		S16 nHistory=0;

		h.scrnID=ITEM_SCR_CALL_DEFLECT_DIGIT_HANDLER_ID;
		h.entryFuncPtr=EntryCallDeflectDigitHandler;
		CallDeflectDigitKeyCode=-1;
		pfnUnicodeStrcpy((S8*)h.inputBuffer,(S8*)&nHistory);
		GetCategoryHistory(h.guiBuffer);
		AddHistory(h);
	}
	else
	{
		cm_p->history_info.IsSave=TRUE;
	}
}


/*****************************************************************************
 * FUNCTION
 *  CallDeflectScreenEmpty
 * DESCRIPTION
 *  register function when screen is empty for call deflection
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void CallDeflectScreenEmpty(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(CALL_DEFLECT_SEARCH_LSK, 0);
    SetLeftSoftkeyFunction(mmi_phb_call_enter_from_deflect_option, KEY_EVENT_UP);
    ClearKeyHandler(KEY_SEND, KEY_EVENT_DOWN);

#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) )
#if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
    SetDialingKeypadPhonebookHandler(mmi_phb_call_enter_from_deflect_option);
#else /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */ 
    SetCenterSoftkeyFunction(UI_dummy_function, KEY_EVENT_UP);
#endif /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */ 
#endif /* ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) ) */ 
}


/*****************************************************************************
 * FUNCTION
 *  CallDeflectScreenNotEmpty
 * DESCRIPTION
 *  register function when screen is not empty for call deflection
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void CallDeflectScreenNotEmpty(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    SetLeftSoftkeyFunction(CallDeflectDigitCall, KEY_EVENT_UP);

#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CC_HandleSendKeys(CallDeflectDigitCall,KEY_EVENT_DOWN);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
	ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
	SetKeyHandler(CallDeflectDigitCall,KEY_ENTER,KEY_EVENT_UP );
#endif
#else
    SetKeyHandler(CallDeflectDigitCall, KEY_SEND, KEY_EVENT_DOWN);
#endif
#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) )
#if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
    SetDialingKeypadPhonebookHandler(mmi_phb_call_enter_from_deflect_option);
#else /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */ 

#ifdef __MMI_MULTI_SIM__
     MTPNP_PFAL_CC_SetCenterFunc(CallDeflectDigitCall);
     SetCenterSoftkeyFunction(MTPNP_PFAL_CC_CSKHandler, KEY_EVENT_UP);
#else
    SetCenterSoftkeyFunction(CallDeflectDigitCall, KEY_EVENT_UP);
#endif   /* __MMI_MULTI_SIM__ */
#endif /* defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__) */ 
#endif /* ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) ) */ 
}

/**************************************************************

	FUNCTION NAME		: CallDeflectDigitCall

  	PURPOSE				: Make a call from scratch pad

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CallDeflectDigitCall(void)
{
   SetCallDeflectFlag(TRUE);
	MakeCall(CallDeflectBuffer);
}

/**************************************************************

	FUNCTION NAME		: CBackCallDeflectNumberFromPB(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CBackCallDeflectNumberFromPB(PS8 number)
{
	memcpy (CallDeflectBuffer, number, MAX_NUMBER);
	CallDeflectDigitCall();
}

/**************************************************************

	FUNCTION NAME		: CheckBeforeGoingBackHistory()

  	PURPOSE				: exit fn for Call Deflect digit handle

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CheckBeforeGoBackHistory(void)
{
	SetNoSave();
	if(0==GetTotalCallCount())
	{
		GoBackfromNotifyScr();
	}
	else
	{
		GoBackHistory();
	}
}

/**************************************************************

	FUNCTION NAME		: EntryScrConfirmDTMFString

	PURPOSE				: Entry func to screen prompting for dtmf
						  confirmation

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void EntryScrConfirmDTMFString(void)
{
	U8 tempBuf[MAX_DTMFBUF_DIGITS*ENCODING_LENGTH+40];

	memset(tempBuf,0,sizeof(tempBuf));

	if(GetTotalActiveCallCount()<1)
	{
		StartTimer(CM_NOTIFYDURATION_TIMER,1, GoBackfromNotifyScr);
		return;
	}

	EntryNewScreen(ITEM_SCR_CONFIRM_DTMF_STRING,
	               ExitScrConfirmDTMFString, NULL, NULL);

	cm_p->history_info.IsSave = TRUE;

	pfnUnicodeStrcpy((S8*)tempBuf,(S8*)GetString(STR_DTMF_CONFIRMATION));
	AnsiiToUnicodeString((S8*)tempBuf+(pfnUnicodeStrlen((S8*)tempBuf)*ENCODING_LENGTH),(S8*)gDTMFConfirmBuf);

	PRINT_INFORMATION (("EntryScrConfirmDTMFString show screen 1\n"));
	
#if 0
	ShowCategory66Screen(0,0,STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
							tempBuf,0,NULL);
#endif /* Guoj delete. It's not used now */
    ShowCategory165Screen
    (STR_GLOBAL_YES, 
     IMG_GLOBAL_YES, 
     STR_GLOBAL_NO,
     IMG_GLOBAL_NO, 
     (UI_string_type)tempBuf, 
     IMG_GLOBAL_QUESTION, 
     NULL);
	PRINT_INFORMATION (("EntryScrConfirmDTMFString show screen 2\n"));
	
   SetLeftSoftkeyFunction(ConfirmDTMFString, KEY_EVENT_UP);
   SetRightSoftkeyFunction(IgnoreDTMFStringAndGoBackHistory, KEY_EVENT_UP);

	SetKeyHandler(IgnoreDTMFStringAndGoBackHistory,KEY_END,KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: ExitScrConfirmDTMFString

	PURPOSE				: exit function for EntryScrConfirmDTMFString()

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void ExitScrConfirmDTMFString(void)
{
	if(cm_p->history_info.IsSave)
	{
		history_t   scr101;
		U16 nHistory = 0;

		pfnUnicodeStrcpy((S8*)scr101.inputBuffer, (PS8)&nHistory);
		scr101.scrnID = ITEM_SCR_CONFIRM_DTMF_STRING;
		scr101.entryFuncPtr = EntryScrConfirmDTMFString;
		GetCategoryHistory(scr101.guiBuffer);
		AddHistory(scr101);
	}
	else
	{
		cm_p->history_info.IsSave=TRUE;
	}
}

/**************************************************************

	FUNCTION NAME		: NotifyMONumberChange(void)

  	PURPOSE				: This function displays number change notification

	INPUT PARAMETERS	: U8* newNumber

	OUTPUT PARAMETERS	: U8* ptrDisplayString

	RETURNS				: void

 

**************************************************************/
void NotifyMONumberChange(U8* newNumber,U8* ptrDisplayString)
{
	if(GetCurrentState()!=CM_OUTGOING_STATE)
		return;

	SetCalledNumber(GetOutgoingCallIndex(),newNumber);

   EntryNewScreen(ITEM_SCR_MO_NUM_CHANGE_NFY,
	               ExitScrNotifyCMPopUp, NULL, NULL);

	ClearAllKeyHandler();

	ShowCategory63Screen((U8*)ptrDisplayString,0,NULL);

	SetLeftSoftkeyFunction(GoBackfromNotifyScr,KEY_EVENT_UP);

	SetPOPUpFlag(TRUE);
	SetKeysForPOPUp();
	StartTimer(CM_NOTIFYDURATION_TIMER, CM_NOTIFY_TIMEOUT, GoBackfromNotifyScr);
}

/**************************************************************

	FUNCTION NAME		: InvisibleDigitHandler(void)

  	PURPOSE				: Set group key handler for single MT call screen
  							  instead of enter dial pad screen

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				: void

 

**************************************************************/
void InvisibleDigitHandler(void)
{
	ClearDtmfBuf();

	AuxInvisibleDigitHandler();

//	SetKeyHandler(MakeCallFromDTMFScr, KEY_SEND, KEY_EVENT_DOWN);

#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CC_HandleSendKeys(MakeCallFromDTMFScr, KEY_EVENT_DOWN);
#else
	SetKeyHandler(MakeCallFromDTMFScr, KEY_SEND, KEY_EVENT_DOWN);
#endif

	SetGroupKeyHandler(AuxInvisibleDigitHandler,(PU16)CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: AuxInvisibleDigitHandler(void)

  	PURPOSE				: Convert key digits to ascii to fill dtmf buffer

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				: void

 

**************************************************************/
void AuxInvisibleDigitHandler(void)
{
	S16 keycode,keytype;
	U8 len;

	GetkeyInfo((U16*)&keycode,(U16*)&keytype);
	len = (U8)pfnUnicodeStrlen((S8*)UnicodeDTMFPadCallBuffer);
	if (keycode >= KEY_0 && keycode <= KEY_9)
	{
		UnicodeDTMFPadCallBuffer[len*ENCODING_LENGTH] = keycode+'0';
	}
	else if (keycode == KEY_STAR)
		UnicodeDTMFPadCallBuffer[len*ENCODING_LENGTH] = '*';
	else if (keycode == KEY_POUND)
		UnicodeDTMFPadCallBuffer[len*ENCODING_LENGTH] = '#';
}


/****************************** CISS ***********************************************/


/**************************************************************

	FUNCTION NAME		: PsCBackUSSNInd(void*)

  	PURPOSE				: This function is intended for registering USSN Indication
						  for PS Callback

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackUSSNInd(void* info)
{
	DeriveUSSNbuf(info);
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) NULL);
	DeleteScreenIfPresent(ITEM_SCR_USSN_MSG);

	if ( (GetIncomingCallHandle() == -1) || (GetActiveScreenId() == SCR_CM_REQUESTINGUSSD)) {
		RemoveSSReqScr();
		EntryScrDisplayUSSNMsg();
	}
	else {
		HistoryReplace(SCR_CM_REQUESTINGUSSD, ITEM_SCR_USSN_MSG, EntryScrDisplayUSSNMsg);
	}
}

/**************************************************************

	FUNCTION NAME		: PsCBackUSSRInd(void*)

  	PURPOSE				: This function is intended for registering USSR Indication
						  for PS Callback

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackUSSRInd(void* info)
{
	DeriveUSSRbuf(info);
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) NULL);

	if (((GetCurrentState()!=CM_OUTGOING_STATE) && (GetCurrentState()!=CM_INCOMING_STATE)) ||
		(GetActiveScreenId() == SCR_CM_REQUESTINGUSSD)) {
		DeleteScreenIfPresent(ITEM_SCR_USSN_MSG);
		EntryScrDisplayUSSRMsg();
	}
	else if ((GetCurrentState()==CM_OUTGOING_STATE) || (GetCurrentState()==CM_INCOMING_STATE)) {
		RemoveSSReqScr();
		SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) NULL);
		DeleteScreenIfPresent(ITEM_SCR_USSN_MSG);
		MakePsDropUSSDReq(NULL);
	}
	else {
		HistoryReplace(SCR_CM_REQUESTINGUSSD, ITEM_SCR_USSN_MSG, EntryScrDisplayUSSRMsg);
		SetDelScrnIDCallbackHandler( ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) USSRDelHistoryHdlr);		
	}
}


/**************************************************************

	FUNCTION NAME		: PsCBackUSSDInd(void*)

  	PURPOSE				: This function is intended for registering USSR Indication
						  for PS Callback

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackUSSDInd(void* info)
{   
#ifdef __MMI_MULTI_SIM__
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, nSimID);	
#endif /* __MMI_MULTI_SIM__ */

    ProcessUSSDrecieved(info);
}

/**************************************************************

	FUNCTION NAME		: EntryScrDisplayUSSNMsg(void)

  	PURPOSE				: This function is the entry screen for displaying USSN info to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrDisplayUSSNMsg(void)
{
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) NULL);
	EntryNewScreen(ITEM_SCR_USSN_MSG, NULL, EntryScrDisplayUSSNMsg, NULL);

	if (isInCall()) {
		SetCMScrnFlag(MMI_FALSE);
	}

	RemoveSSReqScr();

	ShowCategory74Screen(STR_TITLE_SCR_USSN_MSG,0,
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
						0,0,
						(PU8)gUSSDbuf, pfnUnicodeStrlen(gUSSDbuf),
						NULL);

#ifdef __MMI_RECEIVE_USSD_TONE__ 	// for KLM only
	playRequestedTone(WARNING_TONE);
#endif

	TurnOnBacklight(1);
	SetLeftSoftkeyFunction(GoBackFromUSSNMsg,KEY_EVENT_UP);
	SetKeyHandler(NULL,KEY_VOL_UP,KEY_EVENT_UP);
	SetKeyHandler(NULL,KEY_VOL_UP,KEY_LONG_PRESS);
	SetKeyHandler(NULL,KEY_VOL_DOWN,KEY_EVENT_UP);
	SetKeyHandler(NULL,KEY_VOL_DOWN,KEY_LONG_PRESS);
}


/**************************************************************

	FUNCTION NAME		: GoBackFromUSSNMsg(void)

  	PURPOSE				: This function is the entry screen for displaying USSN info to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoBackFromUSSNMsg(void)
{
	if (isInCall()) {
		SetCMScrnFlag(MMI_FALSE);
	}
	GoBackHistory();
}


/**************************************************************

	FUNCTION NAME		: EntryScrDisplayUSSRMsg(void)

  	PURPOSE				: This function is the entry screen for displaying USSR info to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrDisplayUSSRMsg(void)
{

	EntryNewScreen(ITEM_SCR_USSN_MSG, NULL, EntryScrDisplayUSSRMsg, NULL);

	RemoveSSReqScr();

	ShowCategory74Screen(STR_TITLE_SCR_USSR_MSG,0,
						STR_GLOBAL_REPLY,0,
						STR_GLOBAL_CANCEL,0,
						(PU8)gUSSDbuf, pfnUnicodeStrlen(gUSSDbuf),
						NULL);

	TurnOnBacklight(1);

#ifdef __MMI_RECEIVE_USSD_TONE__ 	// for KLM only
	playRequestedTone(WARNING_TONE);
#endif

	SetDelScrnIDCallbackHandler( ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) USSRDelHistoryHdlr);

	SetRightSoftkeyFunction(AbortUSSDResultRequest,KEY_EVENT_UP);
	SetKeyHandler(AbortUSSDResultRequestEND,KEY_END, KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(EntryScrEditUSSRMsg,KEY_EVENT_UP);
	SetKeyHandler(NULL,KEY_VOL_UP,KEY_EVENT_UP);
	SetKeyHandler(NULL,KEY_VOL_UP,KEY_LONG_PRESS);
	SetKeyHandler(NULL,KEY_VOL_DOWN,KEY_EVENT_UP);
	SetKeyHandler(NULL,KEY_VOL_DOWN,KEY_LONG_PRESS);
}


/*****************************************************************************
* FUNCTION
*     USSRDelHistoryHdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     U8
* GLOBALS AFFECTED
*     
*****************************************************************************/
U8 USSRDelHistoryHdlr( void* in_param )
{
	MYQUEUE Message;
     MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakePsDropUSSDReq));

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId= MOD_L4C;
	Message.oslMsgId = PRT_SS_STRING_ABORT_REQ;
	Message.oslPeerBuffPtr= NULL;
	Message.oslDataPtr= NULL;

	ClearProtocolEventHandler(PRT_SS_STRING_ABORT_RSP);
	OslMsgSendExtQueue(&Message);

	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: RegisterUSSRMsgKeyFunction

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void RegisterUSSRMsgKeyFunction(U8* text,U8* cursor,S32 length)
{
	/* check the validity of the input password*/
	if((length) < 1)
	{
		/* if length is short then disable lsk */
		ChangeLeftSoftkey(0,0);
	}
	else
	{
		/* if its valid length then enable lsk */
		EnableLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_USSD_input_empty
* DESCRIPTION
*   LSK handle when number input is empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_USSD_input_empty (void)
{
	ChangeLeftSoftkey (0, 0);
	SetLeftSoftkeyFunction (NULL, KEY_EVENT_UP);	
}

/*****************************************************************************
* FUNCTION
*  mmi_USSD_input_not_empty
* DESCRIPTION
*   LSK handle when number input is not empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_USSD_input_not_empty (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, 0);
	SetLeftSoftkeyFunction (SendUSSDRequest, KEY_EVENT_UP);
}


/**************************************************************

	FUNCTION NAME		: EntryScrDisplayUSSDOkEndDelCb(void)

  	PURPOSE				: This function is the entry screen for displaying USSN info to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern BOOL g_ussd_abort;
U8 EntryScrDisplayUSSDDelCB(void *p)
{
    g_ussd_abort = FALSE;
    
    return FALSE;
}

void EntryScrUSSDGetInputRKCB(void)
{
    mmi_trace (g_sw_CC, "func = %s, g_ussd_abort = %d", __FUNCTION__, g_ussd_abort);

    if(g_ussd_abort == TRUE)
    {
        RemoveSSReqScr();;
    }
    
    GoBackHistory();
}

/**************************************************************

	FUNCTION NAME		: EntryScrDisplayUSSDRes(void)

  	PURPOSE				: This function is the entry screen for input USSD choice to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrUSSDGetInput(void)
{
    mmi_trace (g_sw_CC, "func = %s, g_ussd_abort = %d", __FUNCTION__, g_ussd_abort);
    if(g_ussd_abort == TRUE)
    {
        GoBackHistory();
        return;
    }
    
    memset(gUSSDSendReqbuf, 0, MAX_DIGITS_USSD * ENCODING_LENGTH + ENCODING_LENGTH);	

    EntryNewScreen(ITEM_SCR_USSD_MSG, NULL, EntryScrUSSDGetInput, NULL);

    RegisterInputBoxEmptyFunction (mmi_USSD_input_empty);
    RegisterInputBoxNotEmptyFunction (mmi_USSD_input_not_empty);

    ShowCategory171Screen((PU8)GetString(STR_GLOBAL_OK),(PU8)GetImage(IMG_GLOBAL_OK),
    					(PU8)GetString(STR_GLOBAL_BACK),(PU8)GetImage(IMG_GLOBAL_BACK),
    					(PU8)GetString(STR_TITLE_SCR_USSR_EDIT_MSG), NULL,
    					INPUT_TYPE_PHONE_NUMBER,
    					gUSSDSendReqbuf, MAX_DIGITS_USSD,  (U8*)NULL);

    SetCategory171RightSoftkeyFunction(EntryScrUSSDGetInputRKCB,KEY_EVENT_UP);	
    SetKeyHandler(AbortUSSDRequestEND, KEY_END, KEY_EVENT_DOWN);	
}

/**************************************************************

	FUNCTION NAME		: EntryScrDisplayUSSDRes(void)

  	PURPOSE				: This function is the entry screen for displaying USSN info to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrDisplayUSSDRes(void)
{
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) EntryScrDisplayUSSDDelCB);
	EntryNewScreen(ITEM_SCR_USSN_MSG, NULL, EntryScrDisplayUSSDRes, NULL);

	RemoveSSReqScr();

	ShowCategory74Screen(STR_TITLE_SCR_USSD_MSG,0,
						 STR_GLOBAL_REPLY,0,
						 STR_GLOBAL_CANCEL, 0,
						(PU8)gUSSDbuf, pfnUnicodeStrlen(gUSSDbuf),
						NULL);

#ifdef __MMI_RECEIVE_USSD_TONE__ 	// for KLM only
	playRequestedTone(WARNING_TONE);
#endif

	SetRightSoftkeyFunction(AbortUSSDRequest, KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntryScrUSSDGetInput, KEY_EVENT_UP);
	SetKeyHandler(AbortUSSDRequestEND, KEY_END, KEY_EVENT_DOWN);	
}

/**************************************************************

	FUNCTION NAME		: EntryScrDisplayUSSDRes(void)

  	PURPOSE				: This function is the entry screen for displaying USSN info to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrDisplayUSSDOkEnd(void)
{
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) EntryScrDisplayUSSDDelCB);
	EntryNewScreen(ITEM_SCR_USSN_MSG, NULL, EntryScrDisplayUSSDOkEnd, NULL);

	RemoveSSReqScr();

	ShowCategory74Screen(STR_TITLE_SCR_USSD_MSG,0,
						 STR_GLOBAL_OK,0,
						 STR_GLOBAL_CANCEL, 0,
						(PU8)gUSSDbuf, pfnUnicodeStrlen(gUSSDbuf),
						NULL);

#ifdef __MMI_RECEIVE_USSD_TONE__ 	// for KLM only
	playRequestedTone(WARNING_TONE);
#endif

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: EntryScrDisplayUSSDEnd(void)

  	PURPOSE				: This function is the entry screen for displaying USSN info to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void  aud_speech_set_mode(kal_bool on);
void EntryScrDisplayUSSDEnd(void)
{
	aud_speech_set_mode(KAL_FALSE);//bug 10229, zhangm
	
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) EntryScrDisplayUSSDDelCB);
	EntryNewScreen(ITEM_SCR_USSN_MSG, NULL, EntryScrDisplayUSSDEnd, NULL);

	RemoveSSReqScr();

	ShowCategory74Screen(STR_TITLE_SCR_USSD_MSG,0,
						 0,0,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						(PU8)gUSSDbuf, pfnUnicodeStrlen(gUSSDbuf),
						NULL);

#ifdef __MMI_RECEIVE_USSD_TONE__ 	// for KLM only
	playRequestedTone(WARNING_TONE);
#endif

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: EntryScrEditUSSRMsg(void)

  	PURPOSE				: This function is the entry screen for Editing USSR info by user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrEditUSSRMsg(void)
{
	U8* guiBuffer;		/* Buffer holding history data */

   EntryNewScreen(ITEM_SCR_USSR_EDIT,
	               NULL, EntryScrEditUSSRMsg, NULL);
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) NULL);

	DeleteScreenIfPresent(ITEM_SCR_USSR_MSG);
	guiBuffer = GetCurrGuiBuffer(ITEM_SCR_USSR_EDIT); 	

	if (guiBuffer == NULL) {
		memset( g_callset_cntx_p->SSCFNum, 0, (MAX_CC_ADDR_LEN * ENCODING_LENGTH) + 2);
	}
	RegisterInputBoxValidationFunction(RegisterUSSRMsgKeyFunction);
	ShowCategory5Screen(STR_TITLE_SCR_USSR_EDIT_MSG,0,
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						INPUT_TYPE_KEYPAD_NUMERIC,
						(U8 *)g_callset_cntx_p->SSCFNum,MAX_CC_ADDR_LEN,guiBuffer);

	SetDelScrnIDCallbackHandler(ITEM_SCR_USSR_EDIT, (HistoryDelCBPtr) USSRDelHistoryHdlr);

	SetCategory5RightSoftkeyFunction(EditUSSRGoBack,KEY_EVENT_UP);
	SetKeyHandler(AbortUSSDResultRequestEND,KEY_END, KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(SendUSSRMsg,KEY_EVENT_UP);
	SetKeyHandler(NULL,KEY_VOL_UP,KEY_EVENT_UP);
	SetKeyHandler(NULL,KEY_VOL_UP,KEY_LONG_PRESS);
	SetKeyHandler(NULL,KEY_VOL_DOWN,KEY_EVENT_UP);
	SetKeyHandler(NULL,KEY_VOL_DOWN,KEY_LONG_PRESS);

	if(pfnUnicodeStrlen((S8*)g_callset_cntx_p->SSCFNum) == 0)
		ChangeLeftSoftkey(0,0);
}


/**************************************************************

	FUNCTION NAME		: EditUSSRGoBack

  	PURPOSE				: Sends USSR

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void EditUSSRGoBack(void)
{
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSR_EDIT, (HistoryDelCBPtr) NULL);
	GoBackHistory();
}




/**************************************************************

	FUNCTION NAME		: SendUSSRMsg

  	PURPOSE				: Sends USSR

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void SendUSSRMsg(void)
{
	PU8 Buf;
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) NULL);
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSR_EDIT, (HistoryDelCBPtr) NULL);
	DeleteScreenIfPresent(ITEM_SCR_USSN_MSG);
	ClearExitHandler();
//	ClearDtmfBuf();
	FillDtmfBuff((PU8)g_callset_cntx_p->SSCFNum);
	Buf = GetDtmfBuffer();
	MakeMyCall((PS8)Buf);
}

/**************************************************************

	FUNCTION NAME		: AbortUSSDResultRequest

	PURPOSE			:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void AbortUSSDResultRequest(void)
{
  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_AbortUSSDResultRequest));
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) NULL);
	ClearExitHandler();	
	MakePsDropUSSDReq(CBackDropUSSDReq);
}

/**************************************************************

	FUNCTION NAME		: AbortUSSDResultRequestEND

	PURPOSE			:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void AbortUSSDResultRequestEND(void)
{
	if (isInCall()) {
		HangupAllCalls();
		return;
	}

  	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_AbortUSSDResultRequestEND));
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) NULL);
	SetDelScrnIDCallbackHandler(ITEM_SCR_USSR_EDIT, (HistoryDelCBPtr) NULL);
	DeleteScreenIfPresent(ITEM_SCR_USSN_MSG);
	ClearExitHandler();
	MakePsDropUSSDReq(CBackDropUSSDReqEND);
}

/**************************************************************

	FUNCTION NAME		: ResumeSSScrInCall

	PURPOSE			:

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void ResumeSSScrInCall(void)
{
	history_t	temphistory;

	if (IsScreenPresent(ITEM_SCR_USSN_MSG)) {
		GetHistory(ITEM_SCR_USSN_MSG, &temphistory);
		SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) NULL);
		SetDelScrnIDCallbackHandler(ITEM_SCR_USSR_EDIT, (HistoryDelCBPtr) NULL);
		DeleteScreenIfPresent(ITEM_SCR_USSN_MSG);
		DeleteScreenIfPresent(ITEM_SCR_USSR_EDIT);

		(*(temphistory.entryFuncPtr))();
	}
	else if (IsScreenPresent(SCR_CM_REQUESTINGUSSD)) {
		GetHistory(SCR_CM_REQUESTINGUSSD, &temphistory);
		DeleteScreenIfPresent(SCR_CM_REQUESTINGUSSD);

		(*(temphistory.entryFuncPtr))();
    }
}


#ifdef FACTORY_CC_AUTO_DAILED
void CallFactoryAutoDailInd()
{
#ifdef __MMI_MULTI_SIM__
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, nSimID);
#endif /* __MMI_MULTI_SIM__ */
	mdi_audio_stop_all();
	AnsiiToUnicodeString(g_idle_context.DialPadCallBuffer,"112");

	EntryScrEmergencyCallDialing();
}
#endif


