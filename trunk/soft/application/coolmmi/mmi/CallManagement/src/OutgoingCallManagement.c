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
 *   OutgoingCallmanagement.c
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
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/CallManagement/src/OutgoingCallManagement.c-arc  $
 * 
 *    Rev 1.4   Dec 30 2006 11:48:20   lin
 * 调整屏蔽的代码
 * 
 *    Rev 1.3   Nov 30 2006 10:57:02   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:37:28   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:23:46   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:32:54   zhangxb
 * Initial revision.
 * 
 *    Rev 1.2   Sep 19 2006 19:10:52   zhoumn
 * No change.
 * 
 *    Rev 1.1   Aug 28 2006 17:38:44   zhangxb
 * 合并了PSI和Simon修改的一些代码.
 * 
 *    Rev 1.0   Jul 06 2006 17:42:16   zhangxb
 *
 * 
 *******************************************************************************/

/**************************************************************

	FILENAME	: OutgoingCallmanagement.c

  	PURPOSE		: Call Management application

 

	AUTHOR		: Subramanya BG

	DATE		: Nov 11,02

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
#include "globalscrenum.h"
#include "keybrd.h"
#include "timerevents.h"
#include "commonscreens.h"
#include "wgui.h"
#include "wgui_categories_popup.h"
#include "wgui_categories_cm.h"
#include "settingdefs.h"
#include "datetimegprot.h"
//micha0521
#include "satdefs.h"
#include "satgprots.h"
#include "simdetectiongexdcl.h"
#include "gpioinc.h"
#include "csp.h"
#include "callsetup.h"
#include "profilegprots.h"
#include "managecallmain.h"
#include "phonebookdef.h"
#include "phonebookgprot.h"
#include "messagesexdcl.h"
#include "messagesresourcedata.h"
#include "smsapi.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#ifdef __MMI_VOICE_MEMO__
#include "voicememoprot.h"
#endif
#ifdef __MMI_SOUND_RECORDER__
#include "soundrecorderprot.h"
#endif
#ifdef __MMI_AUDIO_PLAYER__
#include "audioplayerprot.h"
#endif


#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
#endif

#include "callmanagementiddef.h"
#include "callmanagementstruct.h"
#include "callstructuremanagementprot.h"
#include "cmerrortypes.h"
#include "cmerrorhandlersprot.h"
#include "historyhelperprot.h"
#include "commonstubsprot.h"
#include "pscallhelperdef.h"
#include "pscallhelpergprot.h"
#include "callmanagementgprot.h"
#include "incomingcallmanagementprot.h"
#include "outgoingcallhelperprot.h"
#include "callhistoryenum.h"
#include "outgoingcallmanagementprot.h"
/*Jinzh Add Start For 6232 Ver: TBM780  on 2007-8-14 18:50 */
#include "audioplayerdef.h"
/*Jinzh Add End  For 6232 Ver: TBM780  on 2007-8-14 18:50 */
//micha1115
#include "callsetupenum.h"
#include "idleappprot.h"
#include "mmi_trace.h"
#include "callsdefs.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "mtpnp_ad_resdef.h"
#endif
#undef __NEWSIMULATOR
#include "profilesdefs.h"//zhou siyou add 20090730

#define TEMP_NO 10
#define SCR_TEXT_LEN 50

static OUTGOING_CALL gCurrOutcall; //This is a global variable usefull till the
static PU8	gcallList[MAX_CALLS];
static U16	gcallListImage[MAX_CALLS];
static U16	gcallListlen = 0;
static U16	gHilitedCall=0;
static U8	gListOperation=0;
BOOL g_ussd_abort = FALSE;

U16	gtmpOutgoingIndex;
#ifdef __MMI_SUPPORT_JME__
extern s8 vm_status;
#endif
extern MYTIME gTimeStructForCallDuration;
extern const U16 CMIdleScreenDigits[];
extern S8 DTMFPadCallBuffer[MAX_DIGITS_USSD];
extern PU8 gdispnameForSubLCD;
extern S8 gUSSDSendReqbuf[];
extern void MMIProcessActionMsg(void);
#if defined(__MMI_PHB_CALL_SHOW_PICTURE_FROM_FILE__)
extern S8* mmi_phb_image_get_path_from_id(U16 index);
#endif
extern void  aud_speech_set_mode(kal_bool on);
extern audply_state_enum mmi_audply_get_state();
extern pBOOL GetOtherRejectedFlag(void);
extern void ClearInputEventHandler(U16 device);
extern void rda_bt_hfg_send_reject_cfm(void);
/**************************************************************

	FUNCTION NAME		: ReinitializeOutgoingCall()

  	PURPOSE				: To ReInitialize the Outgoing Call Management Application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReinitializeOutgoingCall(void)
{
	U8 i;
	
	SetDialIndication(FALSE);
#ifdef __IP_NUMBER__
	SetTempUseIPNumber(FALSE);
#endif

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetProtocolEventHandlerExt(PsCBackPlayInternalTone, PRT_PLAY_GSM_TONE, i);
		SetProtocolEventHandlerExt(OutgoingCallConnected, PRT_OUTGOINGCALL_CONNECTED, i);
		SetProtocolEventHandlerExt(PsCbackOutgoingCallIdSync, PRT_OUTGOINGCALL_EVENT_RSP, i);
		SetProtocolEventHandlerExt(PsCBackUSSDResponse,PRT_USSD_RES_SUCCESS, i);
	}
}

/**************************************************************

	FUNCTION NAME		: InitOutGoingCall()

  	PURPOSE				: To Initialize the Outgoing Call Management Application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
	                      application management.This function is invoked from InitIncomingCall.

**************************************************************/
void InitOutGoingCall(void)
{
	ReinitializeOutgoingCall();

	SetHiliteHandler(MITEM2011_CM_S_ACTIVE_END, HiliteM2019AllActiveEnd);
	SetHiliteHandler(MITEM2011_CM_END_SINGLE, HiliteM2011EndSingle);
	SetHiliteHandler(MITEM2011_CM_INC_END_SINGLE, HiliteM2011IncEndSingle);
	SetHiliteHandler(MITEM2019_CM_ALL_ACTIVE_END, HiliteM2019AllActiveEnd);
	SetHiliteHandler(MITEM2020_CM_ALL_HELD_END, HiliteM2020AllHeldEnd);
	SetHiliteHandler(MITEM2050_CM_END_HELD_ENDALL_HELD, HiliteM2020AllHeldEnd);
	SetHiliteHandler(MITEM2011_CM_END, HiliteM2016AllCallEnd);
	SetHiliteHandler(MITEM2011_CM_END_ALL, HiliteM2016AllCallEnd);
	SetHiliteHandler(MITEM2016_CM_ALL_CALL_END, HiliteM2016AllCallEnd);
	SetHiliteHandler(MITEM_OUTGOING_REJECT,HiliteIncomingReject);

	SetHiliteHandler(MITEM2010_CM_ACTIVE_HOLD, HiliteM2010ActiveHold);
	SetHiliteHandler(MITEM2010_CM_M_ACTIVE_HOLD, HiliteM2010ActiveHold);
	SetHiliteHandler(MITEM2012_CM_HELD_RETRIEVE, HiliteM2012HeldRetrieve);
	SetHiliteHandler(MITEM2012_CM_M_HELD_RETRIEVE, HiliteM2012HeldRetrieve);
	SetHiliteHandler(MITEM2014_CM_CALL_SWAP, HiliteM2014Swap);
	SetHiliteHandler(MITEM2015_CM_CALL_CONFER, HiliteM2015Conference);
	SetHiliteHandler(MITEM2017_CM_CALL_TRANSFER, HiliteM2017Transfer);
	SetHiliteHandler(MITEM2018_CM_CALL_SPLIT, HiliteM2018Split);

	SetHiliteHandler(MITEM2031_CM_PBOOK, HiliteM2031PBookAppEntry);
	SetHiliteHandler(MITEM2032_CM_SMS, HiliteM2032MsgAppEntry);
#ifdef __MMI_VOICE_MEMO__
	SetHiliteHandler(MITEM2022_CM_VOICE_MEMO, HiliteM2022VoiceMemo);
#endif
#ifdef __MMI_SOUND_RECORDER__
	SetHiliteHandler(MITEM2026_CM_SOUND_RECORDER, HiliteM2026SoundRecorder);
#endif
#ifdef __MMI_AUDIO_PLAYER__
   SetHiliteHandler(MITEM2025_CM_AUDIO_PLAYER, HiliteM2025AudioPlayer);
#endif

#if defined (__MMI_BT_SUPPORT__)

   SetHiliteHandler(MITEM_CM_BT_SETPATH, HiliteBTCMSetPath);
#endif

#if defined(__MMI_BT_SUPPORT__)
    SetHiliteHandler(MITEM_CM_BT_CONNECT_LAST_HEADSET, HiliteBTCMConnectLastBTHeadset);
    SetHintHandler(MITEM_CM_BT_CONNECT_LAST_HEADSET, HintBTConnectLastBTHeadset);
#endif


	SetHiliteHandler(MITEM2020_CM_MUTE, HiliteM2020Mute);
	SetHiliteHandler(MITEM2024_CM_DTMF, HiliteM2024DTMF);
	SetHiliteHandler(MITEM2021_CM_NEWCALL, HiliteM2021NewCall);

	SetHintHandler(MITEM2020_CM_MUTE, Hint2020Mute);
	SetHintHandler(MITEM2024_CM_DTMF, Hint2024DTMF);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2011EndSingle(void)

  	PURPOSE				: hilite handler to invoke End Single function call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2011EndSingle(void)
{
	PRINT_INFORMATION(("\n 2011 End Single \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
#ifdef __GPRS_MODE__
   SetLeftSoftkeyFunction(HangupHilited,KEY_EVENT_UP);
#else
	SetLeftSoftkeyFunction(EndSingleAndGoBack,KEY_EVENT_UP);
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2011IncEndSingle(void)

  	PURPOSE				: hilite handler to invoke End Single function call
  							  when MT call present

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2011IncEndSingle(void)
{
	PRINT_INFORMATION(("\n 2011 End Single \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(EndSingleAndGoBack,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2011SingleActiveEnd(void)

  	PURPOSE				: Hilite function for selecting active call to end

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  otherwise a screen to select an active call to end must be Shown

**************************************************************/
void HiliteM2011SingleActiveEnd(void)
{
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(EndSingleActiveAndGoBack, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2019AllActiveEnd(void)

  	PURPOSE				: hilite handler to invoke function that hangup all the active call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2019AllActiveEnd(void)
{
	PRINT_INFORMATION(("\n 2019 AllActiveEnd \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(HangupallActive,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2020AllHeldEnd(void)

  	PURPOSE				: hilite handler to invoke the function that hangup all the heldcall

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2020AllHeldEnd(void)
{
	PRINT_INFORMATION(("\n 2020 AllHeldEnd \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(HangupallHeld,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2016AllCallEnd(void)

  	PURPOSE				: hilite handler to invoke the hangup of all the calls

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2016AllCallEnd(void)
{
	PRINT_INFORMATION(("\n 2016 AllCallEnd \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(KbCBackEndAllCallsExceptIncoming,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteIncomingReject(void)

  	PURPOSE				: hilite handler to incoming reject call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteIncomingReject(void)
{
	PRINT_INFORMATION(("\n HiliteIncomingReject\n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(KbCBackCallIncomingRejected,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2010ActiveHold(void)

  	PURPOSE				: hilite handler to invoke the hold function call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2010ActiveHold(void)
{
	PRINT_INFORMATION(("\n 2010 ActiveHold\n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(MakeActiveCallHold,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2012HeldRetrieve(void)

  	PURPOSE				: hilite handler to invoke the Retrieve function call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2012HeldRetrieve(void)
{
	PRINT_INFORMATION(("\n 2012 HeldRetrieve \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(RetrieveHeldCall,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2014Swap(void)

  	PURPOSE				: hilite handler to invoke the Swap function call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2014Swap(void)
{
	PRINT_INFORMATION(("\n 2014 Swap \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(SwapCall,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2015Conference(void)

  	PURPOSE				: hilite handler to invoke the Conference function call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2015Conference(void)
{
	PRINT_INFORMATION(("\n 2015 Conference \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(ConferenceCall,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2017Transfer(void)

  	PURPOSE				: hilite handler to invoke the function that transfer the call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2017Transfer(void)
{
	PRINT_INFORMATION(("\n 2017 Transfer \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(TransferCall,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2018Split(void)

  	PURPOSE				: hilite handler to invoke the Split function call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2018Split(void)
{
	PRINT_INFORMATION(("\n 2018 Split \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
#ifdef __GPRS_MODE__
	SetLeftSoftkeyFunction(SplitHilited,KEY_EVENT_UP);
#else
	SetLeftSoftkeyFunction(SplitAndGoBack,KEY_EVENT_UP);
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: DummyEnterPhbFrmActiveCall(void)

  	PURPOSE				: set CM screen flag before go to PHB application

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void DummyEnterPhbFrmActiveCall(void)
{
   SetCMScrnFlag(FALSE);
   mmi_phb_call_enter_from_active_call();
}

/**************************************************************

	FUNCTION NAME		: HiliteM2031PBookAppEntry(void)

  	PURPOSE				: hilite handler to invoke the Pbook app entry

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2031PBookAppEntry(void)
{
	PRINT_INFORMATION(("\n 2031 PBookAppEntry \n"));

	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetKeyHandler( DummyEnterPhbFrmActiveCall,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(DummyEnterPhbFrmActiveCall,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: DummyPreEntryScrMessagesMenuList(void)

  	PURPOSE				: set CM screen flag before go to SMS application

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void DummyPreEntryScrMessagesMenuList(void)
{
	SetCMScrnFlag(FALSE);
   PreEntryScrMessagesMenuList();
}

/**************************************************************

	FUNCTION NAME		: HiliteM2032MsgAppEntry(void)

  	PURPOSE				: hilite handler to invoke the Msg App Entry function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2032MsgAppEntry(void)
{
	PRINT_INFORMATION(("\n 2032 MsgAppEntry \n"));

	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetKeyHandler( DummyPreEntryScrMessagesMenuList,KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(DummyPreEntryScrMessagesMenuList,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

#ifdef __MMI_VOICE_MEMO__
/**************************************************************

	FUNCTION NAME		: DummyEntryVoiceMemoInCall(void)

  	PURPOSE				: set CM screen flag before go to Voice Memo application

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void DummyEntryVoiceMemoInCall(void)
{
   SetCMScrnFlag(FALSE);
   EntryVoiceMemoInCall();
}

/**************************************************************

	FUNCTION NAME		: HiliteM2022VoiceMemo(void)

  	PURPOSE				: hilite handler to invoke the VoiceMemo function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2022VoiceMemo(void)
{
	PRINT_INFORMATION(("\n 2022 Voice Memo \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(DummyEntryVoiceMemoInCall,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
#endif

#ifdef __MMI_SOUND_RECORDER__
/**************************************************************

	FUNCTION NAME		: DummyEntrySoundRecorderInCall(void)

  	PURPOSE				: set CM screen flag before go to Sound Recorder application

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void DummyEntrySoundRecorderInCall(void)
{
   SetCMScrnFlag(FALSE);
   mmi_sndrec_entry_in_call();
}

/**************************************************************

	FUNCTION NAME		: HiliteM2022VoiceMemo(void)

  	PURPOSE				: hilite handler to invoke the VoiceMemo function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2026SoundRecorder(void)
{
	PRINT_INFORMATION(("\n 2026 Sound Recorder \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(DummyEntrySoundRecorderInCall,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
#endif

#ifdef __MMI_AUDIO_PLAYER__
/**************************************************************

	FUNCTION NAME		: DummyEntryAudioPlayerInCall(void)

  	PURPOSE				: set CM screen flag before go to Audio Player application

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void DummyEntryAudioPlayerInCall(void)
{
   SetCMScrnFlag(FALSE);
   mmi_audply_entry_main();
}

/**************************************************************

	FUNCTION NAME		: HiliteM2025AudioPlayer(void)

  	PURPOSE				: hilite handler to invoke the AudioPlayer function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2025AudioPlayer(void)
{
	PRINT_INFORMATION(("\n 2025 Audio Player \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(DummyEntryAudioPlayerInCall,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
#endif


#if defined (__MMI_BT_SUPPORT__)
void HiliteBTCMSetPath(void)
{
	PRINT_INFORMATION(("\n CM--BT Set Voice Path \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(ProcessBTSetVoicePath,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

void ProcessBTSetVoicePath(void)
{
	if( mmi_bt_is_incall_aud_swap2bt() == MMI_TRUE )  
	{
		/* Current voice path in BT, shall transfer to AG */
		mmi_bt_switch_voice_path_incall(MMI_FALSE);
		#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#endif
	}
	else
	{
		/* Current voice path in AG, shall transfer to BT */
		mmi_bt_switch_voice_path_incall(MMI_TRUE);
		#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#endif
	
	}
}

#endif

#if defined(__MMI_BT_SUPPORT__)
/* Configure Default Headset */
/*****************************************************************************
 * FUNCTION
 *  HiliteBTCMConnectLastBTHeadset
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HiliteBTCMConnectLastBTHeadset(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("\n CM--BT Connect Last BT Headset \n"));

    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_SCR1003_LSK);
    SetLeftSoftkeyFunction(ProcessBTCMConnectLastBTHeadset, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  ProcessBTCMConnectLastBTHeadset
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ProcessBTCMConnectLastBTHeadset(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	mmi_bt_connect_last_headset();
}

/*****************************************************************************
 * FUNCTION
 *  HintBTConnectLastBTHeadset
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void HintBTConnectLastBTHeadset(U16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    U16 len = (UCS2Strlen((S8*)mmi_bt_get_default_headset_device_name()) < (MAX_SUB_MENU_SIZE - 1)) ? 
                UCS2Strlen((S8*)mmi_bt_get_default_headset_device_name()) : (MAX_SUB_MENU_SIZE - 1);
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset(hintData[index], 0, MAX_SUB_MENU_SIZE);
    if (len > 0)
    {
       UCS2Strncpy((S8*) hintData[index], (S8*) mmi_bt_get_default_headset_device_name(), len);
    }
}

#endif /* defined(__MMI_BT_SUPPORT__) */ 

/**************************************************************

	FUNCTION NAME		: HiliteM2020Mute(void)

  	PURPOSE				: hilite handler to invoke the Mute function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2020Mute(void)
{
	PRINT_INFORMATION(("\n 2020 Mute \n"));

	if (IsPhoneInMute())
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF,IMG_SCR1002_OFF_LSK);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON,IMG_SCR1002_ON_LSK);
	}

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(OptionMuteorUnMutetheCall,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2024DTMF(void)

  	PURPOSE				: hilite handler to invoke the DTMF function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2024DTMF(void)
{
	PRINT_INFORMATION(("\n 2024 DTMF \n"));

	if(GetDTMFFlag()==TRUE)
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF,IMG_SCR1002_OFF_LSK);
	}else
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON,IMG_SCR1002_ON_LSK);
	}

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(DTMFFunction,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2023LoudSp(void)

  	PURPOSE				: hilite handler to invoke the LoudSpeaker function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2023LoudSp(void)
{
	PRINT_INFORMATION(("\n 2023 Loud Speaker \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(LoudSpeakerFunction/*fromOptions*/,KEY_EVENT_UP);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: HiliteM2021NewCall(void)

  	PURPOSE				: hilite handler to invoke the NewCall function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HiliteM2021NewCall(void)
{
	PRINT_INFORMATION(("\n 2021 New Call \n"));

	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_SCR1003_LSK);
	SetLeftSoftkeyFunction(EnterScreenAndHandleDigit,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: Hint2020Mute()

  	PURPOSE				: hint handler to get mute status

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void Hint2020Mute(U16 index)
{
   if (IsPhoneInMute()==TRUE)
		pfnUnicodeStrcpy( (S8 *)hintData[index],
		                   (S8 *)GetString( STR_GLOBAL_ON ));
	else
		pfnUnicodeStrcpy( (S8 *)hintData[index],
		                   (S8 *)GetString( STR_GLOBAL_OFF));
	return;
}

/**************************************************************

	FUNCTION NAME		: Hint2024DTMF()

  	PURPOSE				: hint handler to get dtmf status

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void Hint2024DTMF(U16 index)
{
   if (GetDTMFFlag()==TRUE)
		pfnUnicodeStrcpy( (S8 *)hintData[index],
		                   (S8 *)GetString( STR_GLOBAL_ON ));
	else
		pfnUnicodeStrcpy( (S8 *)hintData[index],
		                   (S8 *)GetString( STR_GLOBAL_OFF));
	return;
}

/**************************************************************

	FUNCTION NAME		: MakeMOForAT(void)

  	PURPOSE				: Makes MO Call from AT Command

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakeMOForAT(PS8 num, U8 length, U8 callType)
{
	PHB_CM_INTERFACE phb_data;
	
 	if (GetCurrentState() == CM_OUTGOING_STATE)
 	{
		/* already MO, ignore it */
		return;
 	}

	memset (&gCurrOutcall,0,sizeof (OUTGOING_CALL));
	if(strlen((void*)num) > MAX_NUMBER)
	{
		PRINT_INFORMATION (("XXX Number too big XXX \n"));
		ShowCallManagementErrorMessage(ERR_INVALID_NUMBER_FORMAT);
		return;
	}
	if (length==0)
		pfnUnicodeStrcpy((PS8)gCurrOutcall.Number,(PS8)GetString(STR_UNKNOWN_CALLER));
	else
	        AnsiiToUnicodeString((PS8)gCurrOutcall.Number,(PS8)num);

	SetPreviousState((CALL_STATE)GetCurrentState());
	SetCurrentState(CM_OUTGOING_STATE);

	gtmpOutgoingIndex=GetFirstFreeIndex();
	AddNewCallInfo((PU8)gCurrOutcall.Number,GetCurrentState(),GetPreviousState(),
						CM_CALL_MO,(CM_CALL_HANDLE)(gtmpOutgoingIndex+1), callType);

   SetCCFlag(TRUE);
	SetDialIndication(FALSE);

	/* MO call */
	/* look up dialed number from phonebook first anyway */
#ifdef __MMI_MULTI_SIM__
	phb_data = mmi_phb_call_get_data_for_call_mgnt((PU8)gCurrOutcall.Number, MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL));
#else
	phb_data = mmi_phb_call_get_data_for_call_mgnt((PU8)gCurrOutcall.Number);
#endif	

	SetCMPhoneBookStruct(&phb_data);
   SetOutgoingNamefromPhonebook();
	
	if (callType == CM_CSD_CALL)
	   stopRequestedTone(MESSAGE_TONE);

#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)OutgoingCallConnected, PRT_OUTGOINGCALL_CONNECTED, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (OutgoingCallConnected, PRT_OUTGOINGCALL_CONNECTED);
#endif
	OutgoingProcessCMEvent(CM_PS_SHOW_OUTGOING_CALL_SCREEN,NULL);
}

void mmi_call_in_progress(void)
{
	EntryNewScreen(SCR_IN_PROGRESS_PHB, NULL, NULL,NULL);

	ShowCategory65Screen(GetString(STR_CALLS_IN_PROGRESS_TEXT),IMG_PROCESSING_PHONEBOOK,NULL);

	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
}

/**************************************************************

	FUNCTION NAME		: OutgoingProcessCMEvent(CM_EVENT_TYPES ,void* )

  	PURPOSE				: This is the Entry function for all the Events while the Call-Management application
						  is Active.

	INPUT PARAMETERS	: CM_EVENT_TYPES & void*(pointer to the event's associated structure)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  The keyboard events have KB in there defination and Protocol has PS.

**************************************************************/

void OutgoingProcessCMEvent(CM_EVENT_TYPES inEvenType,void* MsgStruct)
{
	ACTION_RESULT result=CM_CALL_FAILURE;

	mmi_trace(g_sw_CC,"MMI_Call: In OutgoingProcessCMEvent inEvenType=%d\n",inEvenType);

	switch(inEvenType)
	{
	case CM_PS_SHOW_OUTGOING_CALL_SCREEN:
		ProcessShowOutgoingCallScreen();
		break;

	case CM_PS_SHOW_REQUESTING_CALL_SCREEN:
//qiff 2008/10/24 modify for GS PRJ. bug:10160
#if 1
            if (GetActiveScreenId() == SCR_CM_REQUESTINGUSSD)
            {
                /* current active screen is SS requesting, display and remove previous*/
                EntryScrUSSDReq();
                RemoveSSReqScr();
                SetDelScrnIDCallbackHandler(SCR_CM_REQUESTINGUSSD, (HistoryDelCBPtr) SSReqHistoryDelHdlr);                
            }
            else if (IsScreenPresent(SCR_CM_REQUESTINGUSSD))
            {
                /* SS requesting screen is in history, do nothing */
            }
            else
            {
                /* current active screen is NOT SS requesting, not in history also, display requesting screen*/
                EntryScrUSSDReq();
            }
#else
		EntryScrUSSDReq();
		if (IsScreenPresent(CM_SCR_MARKER) == FALSE)
		AddMarkerToHistory();
#endif
//qiff 2008/10/24 end
		break;

	case CM_PS_USSD_RECVD:
		ProcessUSSDrecieved(MsgStruct);
		break;

	case CM_KB_OUTGOINGCALL:
		/* MO SEND key */
		result=ProcessKBOutgoingEvent(MsgStruct);

		break;

	case CM_PS_CALLCONNECTED:
		/* connect ind */
		/*jinzh Add Start For 5563 Ver: tbm780  on 2007-8-1 19:14 */
		aud_speech_set_mode(KAL_TRUE);
		/*jinzh Add End  For 5563 Ver: tbm780  on 2007-8-1 19:14 */
		/*Jinzh Add Start For 6232 Ver: TBM780  on 2007-8-14 15:38 */
		SetSpeechVolume(GetModeSpeechVolume());
		/*Jinzh Add End  For 6232 Ver: TBM780  on 2007-8-14 15:38 */
		result=ProcessPSCallconnectedEvent(MsgStruct);
		break;

	case CM_KB_HANGUPREQ:
		/* 1X send, drop single call */
		mmi_call_in_progress();
		result=ProcessKBHangupReqEvent(MsgStruct);
		break;

	case CM_PS_HANGUPSUC:
		result=ProcessPSHangupSucEvent(MsgStruct);
		break;

	case CM_KB_HANGUPALLREQ:
		/* END key in CM */
		mmi_call_in_progress();
		result=ProcessKBHangupallReqEvent(MsgStruct);
		break;

	case CM_PS_HANGUPALLSUC:
		result=ProcessPSHangupallSucEvent(MsgStruct);
		break;

	case CM_KB_HANGUPALLACTREQ:
		/* Menu End All Active */
		result=ProcessKBHangupallActReqEvent(MsgStruct);
		break;

	case CM_PS_HANGUPALLACTSUC:
		result=ProcessPSHangupallActSucEvent(MsgStruct);
		break;

	case CM_KB_HANGUPALLHLDREQ:
		result=ProcessKBHangupallHldReqEvent(MsgStruct);
		break;

	case CM_PS_HANGUPALLHLDSUC:
		result=ProcessPSHangupallHldSucEvent(MsgStruct);
		break;

	case CM_KB_HOLDREQ:
		result=ProcessKBHoldReqEvent(MsgStruct);
		break;

	case CM_PS_HOLDSUC:
		result=ProcessPSHoldSucEvent(MsgStruct);
		break;

	case CM_KB_RETRIEVEREQ:
		result=ProcessKBRetrieveReqEvent(MsgStruct);
		break;

	case CM_PS_RETRIEVESUC:
		result=ProcessPSRetrieveSucEvent(MsgStruct);
		break;

	case CM_KB_SWAPREQ:
		result=ProcessKBSwapReqEvent(MsgStruct);
		break;

	case CM_PS_SWAPSUC:
		result=ProcessPSSwapSucEvent(MsgStruct);
		break;

	case CM_KB_CONFREQ:
		result=ProcessKBConfReqEvent(MsgStruct);
		break;

	case CM_PS_CONFSUC:
		result=ProcessPSConfSucEvent(MsgStruct);
		break;

	case CM_KB_SPLITREQ:
		result=ProcessKBSplitReqEvent(MsgStruct);
		break;

	case CM_PS_SPLITSUC:
		result=ProcessPSSplitSucEvent(MsgStruct);
		break;

	case CM_KB_ECT_REQ:
		result=ProcessKBEctReqEvent(MsgStruct);
		break;

	/*case CM_PS_ECT_SUC:
		result=ProcessPSEctSuccessful(MsgStruct);
		break;*/

	case CM_KB_CALL_DEFLECT_REQ:
		MakePsCallDeflect((void*)PsCBackCallDeflected,(PU8)MsgStruct);
		ShowCategory302Screen(NULL);
		break;

	case CM_PS_INCOMING_CALL_DEFLECT:
	 	ShowCategory302Screen(NULL);
		break;

	default:
		PRINT_INFORMATION(("\n CM Event Handler does not exist \n"));
		break;
	}
	PRINT_INFORMATION_2 ((MMI_TRACE_G5_CM, "*-------[OutgoingCallManagement.c] OutgoingProcessCMEvent: result = "
						  " %d-------*\n", (int) result));
}

/**************************************************************

	FUNCTION NAME		: ProcessShowOutgoingCallScreen(void)

  	PURPOSE				: This is the function to show outgoing call only when the Call list is recieved

	INPUT PARAMETERS	: void*(this points to OUTGOING_CALL structure)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  In Active state,it makes the request to hold and after success it initiates the call.

**************************************************************/
void ProcessShowOutgoingCallScreen(void)
{
	PRINT_INFORMATION (("Call MO In ProcessShowOutgoingCallScreen GetPreviousState=%d \n", GetPreviousState()));

	switch(GetPreviousState())
	{
	case CM_IDLE_STATE:
		/* do not have to suspend audio for CSD call */
		if (GetOutgoingCallType() != CM_CSD_CALL)
		   mdi_audio_suspend_background_play();

		SetSpeechVolume(GetModeSpeechVolume()); 

		EntryScr1001OutgoingCall();
		if (IsScreenPresent(CM_SCR_MARKER) == FALSE)
		{
			AddMarkerToHistory();
		}
		break;

	case CM_INCOMING_STATE:
	case CM_HOLD_STATE:
	case CM_ACTIVE_STATE:
		/*zhangm Add Start For 11136 Ver: GreenStone  on 2009-2-3 9:49 */
		/* do not have to suspend audio for CSD call */
		if (GetOutgoingCallType() != CM_CSD_CALL)
		   mdi_audio_suspend_background_play();	
		/*zhangm Add End  For 11136 Ver: GreenStone  on 2009-2-3 9:49 */
		EntryScr1001OutgoingCall();
		if(GetOutgoingCallOrigin() == CM_CALL_MO_STK)
		{
			AddMarkerToHistory();
		}
		break;

	default:
		PRINT_INFORMATION(("\n KBOutgoingEvent CM State Machine is in incorrect state \n"));
		break;
	}
}

/**************************************************************

	FUNCTION NAME		: ProcessKBOutgoingEvent(void* )

  	PURPOSE				: This is the function to process Keyboard(Request) Event CM_KB_OUTGOINGCALL.

	INPUT PARAMETERS	: void*(this points to OUTGOING_CALL structure)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  In Active state,it makes the request to hold and after success it initiates the call.

**************************************************************/
ACTION_RESULT ProcessKBOutgoingEvent(void* MsgStruct)
{
	memcpy(&gCurrOutcall,MsgStruct,sizeof(OUTGOING_CALL));

	mmi_trace(g_sw_CC,"MMI_Call: Func: %s GetCurrentState()=%d", __FUNCTION__,GetCurrentState());
	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		mmi_trace(g_sw_CC,"MMI_Call: Func: %s CM_IDLE_STATE AllowMoreCalls()=%d, DTMFPadCallBuffer[0]=%d", __FUNCTION__, AllowMoreCalls(), DTMFPadCallBuffer[0]);
		if(AllowMoreCalls() && DTMFPadCallBuffer[0] != 0)
		{
			SetPreviousState(CM_IDLE_STATE);
			SetCurrentState(CM_OUTGOING_STATE);

			gtmpOutgoingIndex=GetFirstFreeIndex();
			AddNewCallInfo(gCurrOutcall.Number,CM_OUTGOING_STATE,
				       CM_IDLE_STATE,CM_CALL_MO,(CM_CALL_HANDLE )(gtmpOutgoingIndex+1), CSMCC_VOICE_CALL);

			MakePsInitiateCall((PU8)DTMFPadCallBuffer,(void*)OutgoingCallConnected);
		}
		else
		{
			ShowCallManagementErrorMessage(NO_ACTIVE_CALL);
			return CM_CALL_FAILURE;
		}
		break;

	case CM_OUTGOING_STATE:
	   PRINT_INFORMATION (("MMI_Call: In ProcessKBOutgoingEvent already outgoing, can't make MO\n"));
		ShowCallManagementErrorMessage(NO_ACTIVE_CALL);
		return CM_CALL_FAILURE;

	case CM_INCOMING_STATE:
		/* send SS when MT call and previous USSR seession exists,
			need to abort previous session */
		DeleteScreenIfPresent(ITEM_SCR_USSN_MSG);
		DeleteScreenIfPresent(ITEM_SCR_USSR_EDIT);
	case CM_HOLD_STATE:
	case CM_ACTIVE_STATE:
		mmi_trace(TRUE,"MMI_Call: Func: %s CM_ACTIVE_STATE/CM_HOLD_STATE AllowMoreCalls()=%d", __FUNCTION__, AllowMoreCalls());
		if(AllowMoreCalls())
		{
			SetPreviousState(GetCurrentState());
			SetCurrentState(CM_OUTGOING_STATE);

			gtmpOutgoingIndex=GetFirstFreeIndex();
			AddNewCallInfo(gCurrOutcall.Number,GetCurrentState(),
	  			       CM_IDLE_STATE,CM_CALL_MO,(CM_CALL_HANDLE )(gtmpOutgoingIndex+1), CSMCC_VOICE_CALL);

			MakePsInitiateCall((PU8)DTMFPadCallBuffer, (void*)OutgoingCallConnected);
		}
		else
		{
			ShowCallManagementErrorMessage(NO_ACTIVE_CALL);
			return CM_CALL_FAILURE;
		}
		break;

	default:
		PRINT_INFORMATION(("MMI_Call:(ERROR) KBOutgoingEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}

	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessPSCallconnectedEvent(void* )

  	PURPOSE				: This is the function to process Protocol(Response) Event CM_PS_CALLCONNECTED.

	INPUT PARAMETERS	: void*(this points to CM_CALL_HANDLE of the call which requested CM_KB_OUTGOINGCALL)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  state of the State m/c.

**************************************************************/
ACTION_RESULT ProcessPSCallconnectedEvent(void* MsgStruct)
{
	CM_CALL_HANDLE	callHandle;
	CM_CALL_HANDLE *handle;

	handle = (CM_CALL_HANDLE*) MsgStruct;
	callHandle = *handle;
	SetCallHandleForStartTimeUpdate(callHandle);

	if(callHandle == -1)
	{
		PRINT_INFORMATION(("\n PSCallconnectedEvent Getting the Connected Handle has -1 \n"));
		return CM_CALL_FAILURE;
	}

	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n PSCallconnectedEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		MakeHold();
		SetPreviousState(GetCurrentState());
		SetCurrentState(CM_ACTIVE_STATE);
    	StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);//niej
		SetCallState(gtmpOutgoingIndex,CM_ACTIVE_STATE,FALSE);

		if(GetWapCallPresent())
		{
			ShowSubLCDScreen(EntrySubLcdScr1002ActiveCall);
			CMGetExactTime(UpdateCallStartTimeforWapCall);

			 
			if (IsScreenPresent(CM_SCR_MARKER))
			{
				DeleteNScrId(CM_SCR_MARKER);
				GoBackHistory();

			}else
			{
				GoBackHistory();
			}
			break;
		}

		if(GetTotalCallCount() > 1)
		{
			CM_CALL_HANDLE call_handle;
			call_handle = GetIncomingCallHandle();
			if (call_handle != -1)
			{
				PHB_CM_INTERFACE phb_data;
				
				SetPreviousState(CM_ACTIVE_STATE);
				SetCurrentState(CM_INCOMING_STATE);
				if (GetNotifySSFlag() == TRUE)
   	      		{
		   	         SetNotifySSFlag(FALSE);
		   	         SetCbackAfterSS(NULL);
				}
				/* initiate call start time without get into active call screen */
				CMGetExactTime(UpdateCallStartTimeforWapCall);
				
				/* MT call caller picture, ringtone...will be overwrite by MO call, 
				it needs to be restored after MO call connected*/
#ifdef __MMI_MULTI_SIM__
				phb_data = mmi_phb_call_get_data_for_call_mgnt(GetCallNumber(call_handle), MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL));
#else
				phb_data = mmi_phb_call_get_data_for_call_mgnt(GetCallNumber(call_handle));
#endif		

#ifdef __MMI_INCOMING_CALL_VIDEO__
		         /* waiting call and ccbs invoke can't use video for display */
#ifdef  __MMI_SUPPORT_JME__
		         if ((GetTotalCallCount() > 0 || GetCCBSFlag() == TRUE)  || (vm_status))
#else
		         if (GetTotalCallCount() > 0 || GetCCBSFlag() == TRUE)
#endif
		         {
		         	phb_data.videoId = 0;
		         }
#endif				
				SetCMPhoneBookStruct(&phb_data);
				
				GoBackToHistory(ITEMSCR_INCOMING_CALL);
			}
			else
				GoBackToHistory(SCR_CM_ACTIVECALLSCREEN);
		}
		else
		{
			if (IsScreenPresent(SCR_CM_ACTIVECALLSCREEN))
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
		}
		break;

	case CM_INCOMING_STATE:
		PRINT_INFORMATION(("\n PSCallconnectedEvent CM_INCOMING_STATE not handled still \n"));
		break;

	case CM_ACTIVE_STATE:
		PRINT_INFORMATION(("\n PSCallconnectedEvent CM_ACTIVE_STATE not handled still \n"));
		break;

	case CM_HOLD_STATE:
		PRINT_INFORMATION(("\n PSCallconnectedEvent CM_HOLD_STATE not handled still \n"));
		break;

	default:
		PRINT_INFORMATION(("\n PSCallconnectedEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessKBHangupReqEvent(void* )

  	PURPOSE				: This is the function to process Keyboard(Request) Event CM_KB_HANGUPREQ.

	INPUT PARAMETERS	: void*(this points to CM_CALL_HANDLE of the call to get dropped)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  The chances of this being invoked in idle state is very less.

**************************************************************/
ACTION_RESULT ProcessKBHangupReqEvent(void* MsgStruct)
{
	CM_CALL_HANDLE	handle;
	CALL_STATE     state;

	PRINT_INFORMATION(("XXX In ProcessKBHangupReqEvent\n GetCurrentState()=%d",GetCurrentState()));

	if (GetInternalAlert() == TRUE)
		StopInternalOutgoingTone();

	handle=*((CM_CALL_HANDLE*)MsgStruct);

	state = GetCallState(handle);
	mmi_trace(g_sw_CC,"MMI_Call: Func: %s GetDialIndication()=%d,GetOutgoingCallType()=%d", __FUNCTION__,GetDialIndication(),GetOutgoingCallType());
	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\nKBHangupEvent CM_IDLE_STATE not handled still \n"));
		break;
	case CM_OUTGOING_STATE:
		SetCallflag(handle,CM_HANGUP_REQUESTED,TRUE);

		mmi_trace(g_sw_CC,"MMI_Call: Func: %s CM_OUTGOING_STATE handle:%d", __FUNCTION__, handle);
		SetCallState(handle, CM_DISCONNECTING_STATE, TRUE);
		MakePsEndSelectiveCall((void*)HangupReqSucess, handle);

		break;
	case CM_INCOMING_STATE:
		 
		// Drop call refinement is not applicable when waiting call exists
		SetCallflag(handle,CM_HANGUP_REQUESTED,TRUE);
		MakePsEndSelectiveCall((void*)HangupReqSucess, handle);
		break;
		 
	case CM_ACTIVE_STATE:
	case CM_HOLD_STATE:
		SetCallflag(handle,CM_HANGUP_REQUESTED,TRUE);
		 
		SetCallState(handle, CM_DISCONNECTING_STATE, TRUE);
		//EntryScr1002ActiveCall();
		 
		MakePsEndSelectiveCall((void*)HangupReqSucess, handle);
		break;
	default:
		PRINT_INFORMATION(("\n KBHangupEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessPSHangupSucEvent(void* )

  	PURPOSE				: This is the function to process Protocol(Response) Event CM_PS_HANGUPSUC.

	INPUT PARAMETERS	: void*(this points to CM_CALL_HANDLE of the call which requested CM_KB_HANGUPREQ)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  if necessary it also changes the state of the State m/c.

**************************************************************/
ACTION_RESULT ProcessPSHangupSucEvent(void* MsgStruct)
{
	CM_CALL_HANDLE *handle;
	U8 flag=FALSE;

	mmi_trace(g_sw_CC,"XXX In ProcessPSHangupSucEvent XXX GetCurrentState()=%d",GetCurrentState());


	handle = (CM_CALL_HANDLE*)MsgStruct;
	if ((*handle == -1) || (GetMMIStructIndexof(*handle) == -1))
	{
		mmi_trace(g_sw_CC,("\nBad call handle -1 in HangupSuc\n"));
		return CM_CALL_FAILURE;
	}

	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		mmi_trace(g_sw_CC,("\n PSHangupSucEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		mmi_trace(g_sw_CC,("XXX In ProcessPSHangupSucEvent : Outgoing StateXXX"));
		LogCallInfoForCallHistory(*handle);
		if(GetCallState(*handle)==CM_OUTGOING_STATE)
		{
			mmi_trace(g_sw_CC,("XXX In ProcessPSHangupSucEvent : Call being dropped is outgoingXXX"));
			flag=TRUE;
			GetEndTimeAndNotifyCallAborted();
			if (GetOutgoingCallOrigin() == CM_CALL_MO_STK)
			   SATCallExitCause(SAT_CALL_DISCONNECT_BEFORE_CONNECT);
		}
		else
		{
			GetEndTimeAndNotifyEndCallDuration(*handle);
		}
		ResetCallflag((*handle),CM_HANGUP_REQUESTED,TRUE);
		SetCallState((*handle),CM_IDLE_STATE,TRUE);
		if(GetTotalCallCount()==0)
		{
			mmi_trace(g_sw_CC,("XXX In ProcessPSHangupSucEvent :GetTotalCallCount=0XXX"));
			SetPreviousState(GetCurrentState());
			SetCurrentState (CM_IDLE_STATE);
		}
		else
		{
			mmi_trace(g_sw_CC,("XXX In ProcessPSHangupSucEvent :GetTotalCallCount not 0 XXX"));
			if(flag)
			{
				SetCurrentState (GetPreviousState());
				SetPreviousState (CM_OUTGOING_STATE);
			}
			else if(CM_OUTGOING_STATE==GetCurrentState())
			{
				break;
			}

			if(GetTotalCallCount()==GetTotalHoldCallCount())
			{
				SetPreviousState(GetCurrentState());
				SetCurrentState(CM_HOLD_STATE);
			}
		}
		break;

	case CM_INCOMING_STATE:
		mmi_trace(g_sw_CC,("XXX In ProcessPSHangupSucEvent : Incoming StateXXX"));
		LogCallInfoForCallHistory(*handle);
		// rel active and accept waiting but waiting is released by network
		if (GetIncomingCallHandle() == *handle)
		{
			SetCurrentState(GetPreviousState());
			SetPreviousState(CM_INCOMING_STATE);
			SetCallState ((*handle), CM_IDLE_STATE, TRUE);
		}
		else
		{
			GetEndTimeAndNotifyEndCallDuration(*handle);
			SetCallState ((*handle), CM_IDLE_STATE, TRUE);

			if (GetTotalActiveCallCount() != 0)
				SetPreviousState (CM_ACTIVE_STATE);
			else if (GetTotalHoldCallCount() != 0)
				SetPreviousState (CM_HOLD_STATE);
		}
		break;

	case CM_HOLD_STATE:
	case CM_ACTIVE_STATE:
		mmi_trace(g_sw_CC,("XXX In ProcessPSHangupSucEvent : HOLD/Active StateXXX"));
		LogCallInfoForCallHistory(*handle);
		GetEndTimeAndNotifyEndCallDuration(*handle);
		ResetCallflag((*handle),CM_HANGUP_REQUESTED,TRUE);
		SetCallState((*handle),CM_IDLE_STATE,TRUE);

		if(GetTotalCallCount()==0)
		{
			SetPreviousState(GetCurrentState());
			SetCurrentState (CM_IDLE_STATE);
		}
		else if((GetTotalActiveCallCount()==0)&&(GetTotalCallCount()==GetTotalHoldCallCount()))
		{
			SetPreviousState(GetCurrentState());
			SetCurrentState (CM_HOLD_STATE);
		}
		break;

	default:
		mmi_trace(g_sw_CC,("\n PSHangupSucEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessKBHangupallReqEvent(void* )

  	PURPOSE				: This is the function to process Keyboard(Request) Event CM_KB_HANGUPALLREQ.

	INPUT PARAMETERS	: void*(this points to NULL as the Hangup is requested on all calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  Invoke the protocol function irrespective of the state.
						  This is always Invoked in Active state.

**************************************************************/
ACTION_RESULT ProcessKBHangupallReqEvent(void* MsgStruct)
{
	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n KBHangupallReqEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
		 
		// Drop call refinement is not applicable when waiting call exists
		SetAllHangupFlag();
		MakePsHangupallRequest((void*)HangupallReqSucess);
		break;
		 
	case CM_OUTGOING_STATE:
	case CM_HOLD_STATE:
	case CM_ACTIVE_STATE:
		SetAllHangupFlag();
		 
		SetAllExceptWaitingCallState(CM_DISCONNECTING_STATE);
		//EntryScr1002ActiveCall();
		 
		MakePsHangupallRequest((void*)HangupallReqSucess);
		break;
	default:
		PRINT_INFORMATION(("\n KBHangupallReqEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessPSHangupallSucEvent(void* )

  	PURPOSE				: This is the function to process Protocol(Response) Event CM_PS_HANGUPALLSUC.

	INPUT PARAMETERS	: void*(this is Null as the request CM_KB_HANGUPALLREQ is on all calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  to IDLE.It also changes the state m/c to IDLE.

**************************************************************/
ACTION_RESULT ProcessPSHangupallSucEvent(void* MsgStruct)
{
	CM_CALL_HANDLE* handle;
   CALL_STATE state;

	handle = (CM_CALL_HANDLE*) MsgStruct;

	 
	if ((*handle) == -1 || GetMMIStructIndexof(*handle) == -1)
	{
		/* call already dropped */
		PRINT_INFORMATION(("\nBad call handle -1 in HangupAllSuc\n"));
		return CM_CALL_FAILURE;
	}
	 
	LogCallInfoForCallHistory(*handle);

   state = GetCallState(*handle);
   if (state != CM_STATE_UNKNOWN)
   {
   	/* incoming call will be handled in SetCallState */
      if(state!=CM_INCOMING_STATE)
	{
		GetEndTimeAndNotifyEndCallDuration(*handle);
	}
	SetCallState((*handle),CM_IDLE_STATE,TRUE);
   }

	if (GetTotalActiveCallCount () + GetTotalHoldCallCount () > 0)
		return CM_CALL_SUCCESS;

	switch(GetCurrentState())
	{
		case CM_IDLE_STATE:
			PRINT_INFORMATION(("\n PSHangupallSucEvent CM_IDLE_STATE not handled still \n"));
			break;

		case CM_INCOMING_STATE:
		case CM_OUTGOING_STATE:
		case CM_HOLD_STATE:
		case CM_ACTIVE_STATE:
			if(GetTotalCallCount()==0)
			{
				SetPreviousState(GetCurrentState());
		 		SetCurrentState(CM_IDLE_STATE);
			}
			else if (state == CM_INCOMING_STATE || 
				state == CM_OUTGOING_STATE)
			{
				SetCurrentState(GetPreviousState());
				SetPreviousState(state);
			}
			break;
		default:
			PRINT_INFORMATION(("\n PSHangupallSucEvent CM State m/c Corrupted \n"));
			return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessKBHangupallActReqEvent(void* )

  	PURPOSE				: This is the function to process Keyboard(Request) Event CM_KB_HANGUPALLACTREQ.

	INPUT PARAMETERS	: void*(this points to NULL as the Hangup is requested on all Active calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  Invoke the protocol function irrespective of the state.
						  This is always Invoked in Active state as active call take precedence over held call.

**************************************************************/
ACTION_RESULT ProcessKBHangupallActReqEvent(void* MsgStruct)
{
	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n KBHangupallActReqEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n KBHangupallActReqEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
		 
		// Drop call refinement is not applicable when waiting call exists
		SetAllActHangupFlag();
		MakePsHangupallActiveRequest((void*)HangupallActReqSucess);
		break;
		 
	case CM_ACTIVE_STATE:
		SetAllActHangupFlag();
		 
		SetAllActiveCallState(CM_DISCONNECTING_STATE);
		//EntryScr1002ActiveCall();
		 
		MakePsHangupallActiveRequest((void*)HangupallActReqSucess);
		break;

	case CM_HOLD_STATE:
		PRINT_INFORMATION(("\n KBHangupallActReqEvent CM_HOLD_STATE not handled still \n"));
		break;

	default:
		PRINT_INFORMATION(("\n KBHangupallActReqEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessPSHangupallActSucEvent(void* )

  	PURPOSE				: This is the function to process Protocol(Response) Event CM_PS_HANGUPALLACTSUC.

	INPUT PARAMETERS	: void*(this is Null as the request CM_KB_HANGUPALLACTREQ is on all Active calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  to IDLE.If necessary,It also changes the state of the state m/c.

**************************************************************/
ACTION_RESULT ProcessPSHangupallActSucEvent(void* MsgStruct)
{
	CM_CALL_HANDLE* handle;

	PRINT_INFORMATION (("\n *** In ProcessPSHangupallActSucEvent\n"));

	handle = (CM_CALL_HANDLE*)MsgStruct;

	 
	if ((*handle) == -1)
	{
		PRINT_INFORMATION(("\nBad call handle -1 in HangupAllActSuc\n"));
		return CM_CALL_FAILURE;
	}
	 
	LogCallInfoForCallHistory(*handle);
	GetEndTimeAndNotifyEndCallDuration(*handle);
	SetCallState (*handle, CM_IDLE_STATE, TRUE);
	if (GetTotalActiveCallCount() >= 1)
	{
		return CM_CALL_SUCCESS;
	}
	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n PSHangupallActSucEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n PSHangupallActSucEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
	case CM_ACTIVE_STATE:
		MakeAllActHangup();

		if(GetTotalCallCount()==0)
		{
			SetPreviousState(GetCurrentState());
		 	SetCurrentState(CM_IDLE_STATE);
		}
		break;

	case CM_HOLD_STATE:
		PRINT_INFORMATION(("\n PSHangupallActSucEvent CM_HOLD_STATE not handled still \n"));
		break;

	default:
		PRINT_INFORMATION(("\n Illegal state to be in  \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessKBHangupallHldReqEvent(void* )

  	PURPOSE				: This is the function to process Keyboard(Request) Event CM_KB_HANGUPALLHLDREQ.

	INPUT PARAMETERS	: void*(this points to NULL as the Hangup is requested on all Hold State calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  Invoke the protocol function irrespective of the state.
						  This is Invoked in Active or Hold state.

**************************************************************/
ACTION_RESULT ProcessKBHangupallHldReqEvent(void* MsgStruct)
{
	SetAllHldHangupFlag();
	 
	// Drop call refinement is not applicable when waiting call exists
	if (GetCurrentState() != CM_INCOMING_STATE)
	{
		SetAllHeldCallState(CM_DISCONNECTING_STATE);
		//EntryScr1002ActiveCall();
	}
	 
	MakePsSendRelHeld((void*)HangupallHldReqSucess);
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessPSHangupallHldSucEvent(void* )

  	PURPOSE				: This is the function to process Protocol(Response) Event CM_PS_HANGUPALLHLDSUC.

	INPUT PARAMETERS	: void*(this is Null as the request CM_KB_HANGUPALLHLDREQ is on all Held/Hold state calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  to IDLE.If necessary,It also changes the state of the state m/c.

**************************************************************/
ACTION_RESULT ProcessPSHangupallHldSucEvent(void* MsgStruct)
{
	CM_CALL_HANDLE* handle;

	handle = (CM_CALL_HANDLE*)MsgStruct;

	 
	if ((*handle) == -1)
	{
		PRINT_INFORMATION(("\nBad call handle -1 in HangupAllHldSuc\n"));
		return CM_CALL_FAILURE;
	}
	 
	LogCallInfoForCallHistory(*handle);
	GetEndTimeAndNotifyEndCallDuration(*handle);

	SetCallState((*handle),CM_IDLE_STATE,TRUE);

	if (GetTotalHoldCallCount() > 0)
		return CM_CALL_SUCCESS;

	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n PSHangupallHldSucEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n PSHangupallHldSucEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
		PRINT_INFORMATION(("\n PSHangupallHldSucEvent CM_INCOMING_STATE not handled still \n"));
		break;

	case CM_ACTIVE_STATE:
	case CM_HOLD_STATE:
		if(GetTotalCallCount()==0)
		{
			SetPreviousState(GetCurrentState());
		 	SetCurrentState(CM_IDLE_STATE);
		}
		break;

	default:
		PRINT_INFORMATION(("\n PSHangupallHldSucEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessKBHoldReqEvent(void* )

  	PURPOSE				: This is the function to process Keyboard(Request) Event CM_KB_HOLDREQ.

	INPUT PARAMETERS	: void*(this points to NULL as the hold is request on all active calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  Invoke the protocol function irrespective of the state.
						  The chances of this being invoked in Idle or Hold  state is very less.

**************************************************************/
ACTION_RESULT ProcessKBHoldReqEvent(void* MsgStruct)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U8 flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GetChldReqSent() != CM_ACTION_NONE)
    {
        return CM_CALL_FAILURE;
    }

    flag = GetAllCallFlags();
    mmi_trace(g_sw_CC,"MMI_Call Func: %s flag:%d", __FUNCTION__, flag);
    if (((flag & CM_SWAP_REQUESTED) != 0) || 
    	((flag & CM_CONF_REQUESTED) != 0) ||
    	((flag & CM_ECT_REQUESTED) != 0))
    {
        return CM_CALL_FAILURE;
    }

	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n KBHoldReqEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n KBHoldReqEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
		PRINT_INFORMATION(("\n KBHoldReqEvent CM_INCOMING_STATE not handled still \n"));
		break;

	case CM_ACTIVE_STATE:
		SetHoldFlag();
		MakePsActiveHold((void*)HoldReqSucess);
		break;

	case CM_HOLD_STATE:
		PRINT_INFORMATION(("\n KBHoldReqEvent CM_HOLD_STATE not handled still \n"));
		break;

	default:
		PRINT_INFORMATION(("\n KBHoldReqEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}

	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessPSHoldSucEvent(void* )

  	PURPOSE				: This is the function to process Protocol(Response) Event CM_PS_HOLDSUC.

	INPUT PARAMETERS	: void*(this is Null as the request CM_KB_HOLDREQ is on all active call)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  ACTIVE to HOLD if necessary it also changes the state of the State m/c.

**************************************************************/
ACTION_RESULT ProcessPSHoldSucEvent(void* MsgStruct)
{
	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n PSHoldSucEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n PSHoldSucEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
		PRINT_INFORMATION(("\n PSHoldSucEvent CM_INCOMING_STATE \n"));
		MakeHold();
		EntryScr1005NotifyHoldSucess();
		SyncCallList();
		break;

	case CM_ACTIVE_STATE:
		PRINT_INFORMATION(("\n PSHoldSucEvent CM_ACTIVE_STATE \n"));
		MakeHold();
		EntryScr1005NotifyHoldSucess();
		break;

	case CM_HOLD_STATE:
		PRINT_INFORMATION(("\n HoldSucEvent CM_HOLD_STATE not handled still \n"));
		break;

	default:
		PRINT_INFORMATION(("\n PSHoldSucEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessKBRetrieveReqEvent(void* )

  	PURPOSE				: This is the function to process Keyboard(Request) Event CM_KB_RETRIEVEREQ.

	INPUT PARAMETERS	: void*(this points to NULL as the Retrieve is request on all held calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  Invoke the protocol function irrespective of the state.
						  The chances of this being invoked in Idle or Active state is very less.

**************************************************************/
ACTION_RESULT ProcessKBRetrieveReqEvent(void* MsgStruct)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U8 flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GetChldReqSent() != CM_ACTION_NONE)
    {
        return CM_CALL_FAILURE;
    }

    flag = GetAllCallFlags();
    if (((flag & CM_SWAP_REQUESTED) != 0) || 
    	((flag & CM_CONF_REQUESTED) != 0) ||
    	((flag & CM_ECT_REQUESTED) != 0))
    {
        return CM_CALL_FAILURE;
    }

	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n KBRetrieveReqEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n KBRetrieveReqEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
	case CM_ACTIVE_STATE:
	case CM_HOLD_STATE:
		SetRetrieveFlag();
		MakePsActiveHold((void*)RetrieveReqSucess);
		break;

	default:
		PRINT_INFORMATION(("\n KBRetrieveReqEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessPSRetrieveSucEvent(void* )

  	PURPOSE				: This is the function to process Protocol(Response) Event CM_PS_RETRIEVESUC.

	INPUT PARAMETERS	: void*(this is Null as the request CM_KB_RETRIEVEREQ is on all Held/Hold calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  HOLD to ACTIVE if necessary it also changes the state of the State m/c.

**************************************************************/
ACTION_RESULT ProcessPSRetrieveSucEvent(void* MsgStruct)
{
	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n PSRetrieveSucEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n PSRetrieveSucEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
		PRINT_INFORMATION(("\n PSRetrieveSucEvent CM_INCOMING_STATE \n"));
		MakeRetrieve();
		EntryScr1006NotifyRetrieveSucess();
		SetPreviousState(CM_ACTIVE_STATE);
		break;

	case CM_HOLD_STATE:
	case CM_ACTIVE_STATE:
		MakeRetrieve();
		EntryScr1006NotifyRetrieveSucess();
		if((GetTotalCallCount()==GetTotalActiveCallCount())&&(GetTotalHoldCallCount()==0))
		{
			SetPreviousState(GetCurrentState());
		 	SetCurrentState(CM_ACTIVE_STATE);
		}
		break;
	default:
		PRINT_INFORMATION(("\n PSRetrieveSucEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessKBSwapReqEvent(void* )

  	PURPOSE				: This is the function to process Keyboard(Request) Event CM_KB_SWAPREQ.

	INPUT PARAMETERS	: void*(this points to NULL as the Swap is requested on all calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  Invoke the protocol function irrespective of the state.
						  This is always Invoked in Active state.

**************************************************************/
ACTION_RESULT ProcessKBSwapReqEvent(void* MsgStruct)
{
  	U8 flag;
		
  	flag = GetAllCallFlags();

    if (GetChldReqSent() != CM_ACTION_NONE ||     
    	((flag & CM_SWAP_REQUESTED) != 0) || 
    	((flag & CM_CONF_REQUESTED) != 0) ||
    	((flag & CM_ECT_REQUESTED) != 0))
    {
        return CM_CALL_FAILURE;
    }
   
	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n KBSwapReqEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n KBSwapReqEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_HOLD_STATE:
	case CM_INCOMING_STATE:
	case CM_ACTIVE_STATE:
		SetSwapFlag();
		MakePsSwapCallRequest((void*)SwapReqSucess);
		break;
	default:
		PRINT_INFORMATION(("\n KBSwapReqEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessPSSwapSucEvent(void* )

  	PURPOSE				: This is the function to process Protocol(Response) Event CM_PS_SWAPSUC.

	INPUT PARAMETERS	: void*(this is Null as the request CM_KB_SWAPREQ is on all calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  HOLD to ACTIVE & viceversa.

**************************************************************/
ACTION_RESULT ProcessPSSwapSucEvent(void* MsgStruct)
{
   if(GetTotalCallCount()>0)
	{
		SyncCallList();
	}

	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		mmi_trace(g_sw_CC, "\n PSSwapSucEvent CM_IDLE_STATE not handled still \n");
		break;

	case CM_OUTGOING_STATE:
		mmi_trace(g_sw_CC, "\n PSSwapSucEvent CM_OUTGOING_STATE not handled still \n");
		break;

	case CM_INCOMING_STATE:
		MakeSwap();
		// if (GetTotalActiveCallCount() + GetTotalHoldCallCount() ==  1)
		// {
			// DeleteScreenIfPresent(ITEMSCR_INCOMING_CALL);
			// SetIncomingCallStateActive();
			// SetPreviousState (GetCurrentState());
			// SetCurrentState(CM_ACTIVE_STATE);
		// }
		EntryScr1007NotifySwapSucess();
		break;

	case CM_HOLD_STATE:
		MakeSwap();
		EntryScr1007NotifySwapSucess();
		break;

	case CM_ACTIVE_STATE:
		MakeSwap();
		EntryScr1007NotifySwapSucess();
		break;

	default:
		mmi_trace(g_sw_CC, "\n PSSwapSucEvent CM State m/c Corrupted \n");
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessKBConfReqEvent(void* )

  	PURPOSE				: This is the function to process Keyboard(Request) Event CM_KB_CONFREQ.

	INPUT PARAMETERS	: void*(this points to NULL as the Conference is requested on all calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  Invoke the protocol function irrespective of the state.
						  This is always Invoked in Active state.

**************************************************************/
ACTION_RESULT ProcessKBConfReqEvent(void* MsgStruct)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U8 flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flag = GetAllCallFlags();
    if (((flag & CM_SWAP_REQUESTED) != 0) || 
    	((flag & CM_CONF_REQUESTED) != 0) ||
    	((flag & CM_ECT_REQUESTED) != 0))
    {
        return CM_CALL_FAILURE;
    }
	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n KBConfReqEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n KBConfReqEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
	case CM_ACTIVE_STATE:
		SetConfFlag();
		MakePsConfCallRequest();
		break;
	case CM_HOLD_STATE:
		PRINT_INFORMATION(("\n KBConfReqEvent CM_HOLD_STATE not handled still \n"));
		break;

	default:
		PRINT_INFORMATION(("\n KBConfReqEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessPSConfSucEvent(void* )

  	PURPOSE				: This is the function to process Protocol(Response) Event CM_PS_CONFSUC.

	INPUT PARAMETERS	: void*(this is Null as the request CM_KB_CONFREQ is on all calls)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  to ACTIVE.

**************************************************************/
ACTION_RESULT ProcessPSConfSucEvent(void* MsgStruct)
{
	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n PSConfSucEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n PSConfSucEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
	case CM_ACTIVE_STATE:
		MakeConf();
		EntryScr1008NotifyConfSucess();
		break;

	case CM_HOLD_STATE:
		PRINT_INFORMATION(("\n PSConfSucEvent CM_HOLD_STATE not handled still \n"));
		break;

	default:
		PRINT_INFORMATION(("\n PSConfSucEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessKBSplitReqEvent(void* )

  	PURPOSE				: This is the function to process Keyboard(Request) Event CM_KB_SPLITREQ.

	INPUT PARAMETERS	: void*(this points to CM_CALL_HANDLE of the call to be splitted in the conference)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  from the group of active calls.
						  Invoke the protocol function irrespective of the state.
						  This is always Invoked in Active state.

**************************************************************/
ACTION_RESULT ProcessKBSplitReqEvent(void* MsgStruct)
{
	CM_CALL_HANDLE	callIndex;

	memcpy((void*)&callIndex,MsgStruct,sizeof(CM_CALL_HANDLE));

	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n KBSplitReqEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n KBSplitReqEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
		if (GetTotalActiveCallCount() >= 2)
		{
			SetCallflag(callIndex,CM_SPLIT_REQUESTED,TRUE);
			MakePsSplitRequest(callIndex);
		}
		else
			PRINT_INFORMATION(("\n Not enuf calls to make a split request \n"));
		break;

	case CM_ACTIVE_STATE:
		if (GetTotalActiveCallCount() >= 2)
		{
			SetCallflag(callIndex,CM_SPLIT_REQUESTED,TRUE);
			MakePsSplitRequest(callIndex);
		}
		else
			PRINT_INFORMATION(("\n Not enuf calls to make a split request \n"));
		break;

	case CM_HOLD_STATE:
		PRINT_INFORMATION(("\n KBSplitReqEvent CM_HOLD_STATE not handled still \n"));
		break;

	default:
		PRINT_INFORMATION(("\n KBSplitReqEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessPSSplitSucEvent(void* )

  	PURPOSE				: This is the function to process Protocol(Response) Event CM_PS_SPLITSUC.

	INPUT PARAMETERS	: void*(this points to CM_CALL_HANDLE of the call that requested CM_KB_SPLITREQ)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  other calls states are changed to HOLD.
**************************************************************/
ACTION_RESULT ProcessPSSplitSucEvent(void* MsgStruct)
{
	switch(GetCurrentState())
	{
	case CM_IDLE_STATE:
		PRINT_INFORMATION(("\n PSSplitSucEvent CM_IDLE_STATE not handled still \n"));
		break;

	case CM_OUTGOING_STATE:
		PRINT_INFORMATION(("\n PSSplitSucEvent CM_OUTGOING_STATE not handled still \n"));
		break;

	case CM_INCOMING_STATE:
	case CM_ACTIVE_STATE:
		MakeSplitSucess();
		EntryScr1010NotifySplitSucess();
		break;

	case CM_HOLD_STATE:
		PRINT_INFORMATION(("\n PSSplitSucEvent CM_HOLD_STATE not handled still \n"));
		break;

	default:
		PRINT_INFORMATION(("\n PSSplitSucEvent CM State m/c Corrupted \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessKBEctReqEvent(void* )

  	PURPOSE				: This is the function to process Keyboard(Request) Event CM_KB_HANGUPREQ.

	INPUT PARAMETERS	: void*(this points to CM_CALL_HANDLE of the call to get dropped)

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 
						  The chances of this being invoked in idle state is very less.

**************************************************************/
ACTION_RESULT ProcessKBEctReqEvent(void* MsgStruct)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    flag = GetAllCallFlags();
    if (((flag & CM_SWAP_REQUESTED) != 0) || 
    	((flag & CM_CONF_REQUESTED) != 0) ||
    	((flag & CM_ECT_REQUESTED) != 0))
    {
        return CM_CALL_FAILURE;
    }
	switch(GetCurrentState())
	{
	case CM_OUTGOING_STATE:
		if (GetTotalActiveCallCount() == 1 || GetTotalHoldCallCount() == 1)
		{
			MakePsEctRequest();
		}
		else
			PRINT_INFORMATION(("\n Not Enuf Calls to make transfer \n"));
		break;

	case CM_INCOMING_STATE:
	case CM_ACTIVE_STATE:
	case CM_HOLD_STATE:
		if (GetTotalActiveCallCount() == 1 && GetTotalHoldCallCount() == 1)
		{
			MakePsEctRequest();
		}
		else
			PRINT_INFORMATION(("\n Not Enuf Calls to make transfer \n"));
		break;

	default:
		PRINT_INFORMATION(("\n ECT Default does not exit \n"));
		return CM_CALL_FAILURE;
	}
	return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: ProcessPSEctSuccessful(void* )

  	PURPOSE				: This is the function to process Protocol(Response) Event CM_PS_ECT_SUCCESS.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: ACTION_RESULT

	RETURNS				: This returns CM_CALL_SUCCESS if success or CM_CALL_FAILURE if failure.

 

**************************************************************/
ACTION_RESULT ProcessPSEctSuccessful(void* MsgStruct)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 count;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("XXX - In ProcessPSEctSuccessful XXX\n"));

    /* only active mpty can be split */
    for (count = 0; count < MAX_CALLS; count++)
    {
        if (cm_p->state_info.AllCalls[count].status_flag & CM_ECT_REQUESTED)
        {
            ResetCallflag(count, CM_ECT_REQUESTED, FALSE);
        }
    }

    return CM_CALL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: PsCBackCallDeflected(void*)

  	PURPOSE				: This function is called back by the protocol stack when the
	                      Incoming Call is deflected

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackCallDeflected(void* info)
{
	U16 result;

	if (CheckCallDeflectSuccess(info, &result))
	{
		PRINT_INFORMATION (("XXX - In PsCBackCallDeflected XXX\n"));
		OutgoingProcessCMEvent(CM_PS_INCOMING_CALL_DEFLECT, info);
		//ProcessIncomingEvents (CM_PS_INCOMING_CALL_DEFLECT, info);
	}
	else
	{
		PRINT_INFORMATION (("XXX - FAILED FAILED FAILED In PsCBackCallDeflected XXX\n"));
		ProcessIncomingFailureEvents (CM_PS_INCOMING_CALL_DEFLECT, result);
		DeleteUptoScrID(ITEMSCR_INCOMING_CALL);
	}
}

/**************************************************************

	FUNCTION NAME		: EndSingleAndGoBack(void)

  	PURPOSE				: This function display the list of active and held calls
						  to choose the call that needs to be ended.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void EndSingleAndGoBack(void)
{
	S16 dummy;

	gcallListlen=GetAllDispNameorNum(gcallList,gcallListImage,&dummy);

	if(gcallListlen!=GetTotalActiveCallCount() + GetTotalHoldCallCount())
		PRINT_INFORMATION(("\nFatal Error in CM Call Structure 4\n"));

	gListOperation=1;
	EntryScr1009CallListScreen();
}

void EndSingleActiveAndGoBack(void)
{
	gcallListlen=GetAllActDispNameorNum(gcallList);

	gListOperation=1;
	EntryScr1009CallListScreen();
}

/**************************************************************

	FUNCTION NAME		: SplitAndGoBack(void)

  	PURPOSE				: This function display the list of active call
						  to choose the call that needs to be splitted.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void SplitAndGoBack(void)
{
	gcallListlen=GetAllActDispNameorNum(gcallList);

	if(gcallListlen!=GetTotalActiveCallCount())
		PRINT_INFORMATION(("\nFatal Error in CM Call Structure 4\n"));

	gListOperation=2;
	EntryScr1009CallListScreen();
}

/**************************************************************

	FUNCTION NAME		: SetCurrHiliteCall(S32 )

  	PURPOSE				: This is the hilite handler for the gcallList listview displayed.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetCurrHiliteCall(S32 hiliteIndex)
{
	gHilitedCall =(U16) hiliteIndex;
}

/**************************************************************

	FUNCTION NAME		: HangupHilited(void)

  	PURPOSE				: Hangup the selected or hilted call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  and invoke the hangup
**************************************************************/
void HangupHilited(void)
{
	CM_CALL_HANDLE temp;

	temp=GetCallHandle(gcallList[gHilitedCall]);
	HangupCall(temp);
}

/**************************************************************

	FUNCTION NAME		: SplitHilited(void)

  	PURPOSE				: Split the selected or hilted call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  and invoke the Splitcall
**************************************************************/
void SplitHilited(void)
{
	CM_CALL_HANDLE temp;

	temp=GetCallHandle(gcallList[gHilitedCall]);
	SplitCall(temp);
}

/**************************************************************

	FUNCTION NAME		: EntryScr1009CallListScreen(void)

  	PURPOSE				: display the list of strings stored by gcallList

	INPUT PARAMETERS	: gListOperation,gcallList,gcallListlen global variables filled by other function

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  depending on the scenario the LSK operation is also depending on
						  gListOperation.If gListOperation==1 then operation is HANGUP request.
						  If gListOperation==2 then the operation is SPLIT.

**************************************************************/
void EntryScr1009CallListScreen(void)
{
	U8* guiBuffer;

   EntryNewScreen(SCR1009_CM_CALLLISTSCREEN, NULL,
	               EntryScr1009CallListScreen, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR1009_CM_CALLLISTSCREEN);

	SetParentHandler(0);
	RegisterHighlightHandler(SetCurrHiliteCall);

	ShowCategory6Screen(STR_SCR1009_CAPTION, IMG_SCR1009_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  gcallListlen, gcallList,NULL, 0, guiBuffer);

	if(gListOperation==1)
		SetLeftSoftkeyFunction(HangupHilited,KEY_EVENT_UP);
	else if(gListOperation==2)
		SetLeftSoftkeyFunction(SplitHilited,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: EntryScr1001OutgoingCall(void)

  	PURPOSE				: Shows the outgoing call screen

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void rda_bt_hfg_send_CallSetup_1_ind(void);
#ifdef __IP_NUMBER__
extern BOOL bUseIPForRedial;
#endif
void EntryScr1001OutgoingCall(void)
{
	PU8 dispname;
	//PU8 guiBuffer;
	S8* imgPath = NULL;
	U8* IPNum = NULL;
	U16 nOutGoingImgId=0;
	U16	nRskImgId=0;
	U16   nRskStringId=0;
	U16 calling_str_id = STR_SCR1001_RUNNING;
	PHB_CM_INTERFACE *PhoneNumberStruct;

	mmi_trace(g_sw_CC,"MMI_Call: *** In EntryScr1001OutgoingCall \n");

   TurnOnBacklight(1);
   SetCbackAfterSS(NULL);
	SetAbortCallRequestedFlag(FALSE);
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
	SetCMScrnFlag(TRUE);


#ifdef __MMI_NUMBER_LOCATION__
	dispname=GetDispNameorNumWithAddress();
#else
	dispname=GetDispNameorNum();
#endif
   	EntryNewScreen(SCR1001_CM_OUTGOINGCALLSCREEN, ExitScr1001OutgoingCall,
	               NULL, NULL);
	//guiBuffer = GetCurrGuiBuffer(SCR1001_CM_OUTGOINGCALLSCREEN);
	PhoneNumberStruct=GetCMPhoneBookStruct();
	nOutGoingImgId = PhoneNumberStruct->pictureId;
	if (nOutGoingImgId == IMG_PHB_DEFAULT)
		nOutGoingImgId = IMG_CM_STATE_OUTGOING;

#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	calling_str_id = STRING_MTPNP_CALL_SIM1 + MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL);
#endif /* defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_) */

   UnicodeToAnsii((PS8)subMenuDataPtrs[0], (PS8)PhoneNumberStruct->name);
   UnicodeToAnsii((PS8)subMenuDataPtrs[1], (PS8)PhoneNumberStruct->number);
   UnicodeToAnsii((PS8)subMenuDataPtrs[2], (PS8)dispname);   
   mmi_trace(g_sw_CC,"MMI_Call:  *** In EntryScr1001OutgoingCall,  number=%s\n", subMenuDataPtrs[1]);
//	mmi_trace(g_sw_CC,"Call Screen *** In EntryScr1001OutgoingCall, display name=%s\n", subMenuDataPtrs[2]);   

#if defined(__MMI_PHB_CALL_SHOW_PICTURE_FROM_FILE__)
   if(nOutGoingImgId == 1) 
   {
   	/* use file image */
		imgPath = mmi_phb_image_get_path_from_id(PhoneNumberStruct->record_index);
		if(imgPath == NULL)
		   nOutGoingImgId = IMG_CM_STATE_OUTGOING;
	}
#endif

	/*if(nOutGoingImgId==IMG_PHB_DEFAULT || ((nOutGoingImgId == 1) && imgPath == NULL))
		nOutGoingImgId=GetImgIdofState(CM_OUTGOING_STATE); //needs to be changed later.*/
	 
	/*if (GetDisconnectingCallHandle() != -1)
		nOutGoingImgId=GetImgIdofState(CM_DISCONNECTING_STATE);*/
	 

#ifdef __IP_NUMBER__
	if (GetTempUseIPNumber() == TRUE)
	{
		U8 nsimid = 0;
		#ifdef __MMI_MULTI_SIM__
		nsimid = MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL);
		#endif
		IPNum = (U8*)GetCurrIPNumberUCS2(nsimid);
		bUseIPForRedial = TRUE;
	}
	else
	{
		bUseIPForRedial = FALSE;
	}
#endif		
	
	if(GetTotalCallCount()>1)
	{
		/* more than one call */
		
		 
		if (GetDisconnectingCallHandle() != -1)
		{
		   if (nOutGoingImgId == 1)
		   	{
			  
			  ShowCategory18Screen(0,0,0,0,0, calling_str_id, dispname, IPNum, IMG_CM_STATE_OUTGOING, imgPath,TRUE, NULL);
		   	}
		   else
		   	{
		   	
			ShowCategory18Screen(0,0,0,0,0, calling_str_id, dispname, IPNum, nOutGoingImgId, NULL,TRUE, NULL);
		   	}
          }
		else
		 
		{
			if(nOutGoingImgId == 1)
			{
				ShowCategory18Screen(0,STR_GLOBAL_OPTIONS,IMG_SCR1001_LSK,STR_GLOBAL_ABORT,
						IMG_SCR1001_RSK,calling_str_id, dispname, IPNum, IMG_CM_STATE_OUTGOING, imgPath,FALSE, NULL);
			}
			else
			{
				ShowCategory18Screen(0,STR_GLOBAL_OPTIONS,IMG_SCR1001_LSK,STR_GLOBAL_ABORT,
						 IMG_SCR1001_RSK,calling_str_id, dispname, IPNum, nOutGoingImgId, NULL,FALSE,NULL);
			}
		}
		 
		SetLeftSoftkeyFunction (EntryScrOutgoingMultipleOptions, KEY_EVENT_UP);
		SetRightSoftkeyFunction(DropRequest,KEY_EVENT_UP);
//		SetKeyHandler(HangupAll,KEY_END, KEY_EVENT_DOWN);
		SetKeyHandler(DropRequest,KEY_END, KEY_EVENT_DOWN);

		if (!EmergencyCallPresent())
		SetGroupKeyHandler(EnterScreenAndHandleDigit,(PU16)CMIdleScreenDigits,
								MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);
	}
	else
	{
		/* only one call */
#ifdef 	__MMI_CALL_MO_LOUDSPK	

#if( !defined(__MMI_BT_SUPPORT__))
		nRskImgId=IMG_SCR1002_LOUDSP_RSK;
		nRskStringId=GetHFreeHeld() ? STR_SCR1002_LOUDSP_H_HELD_RSK : STR_SCR1002_LOUDSP_H_FREE_RSK;
#else
		/* Bluetooth Support */
		if( /*mmi_bt_is_aud2hf() != MMI_TRUE */mmi_bt_is_incall_aud_swap2bt() != MMI_TRUE  )
		{
			nRskImgId=IMG_SCR1002_LOUDSP_RSK;
			nRskStringId=GetHFreeHeld() ? STR_SCR1002_LOUDSP_H_HELD_RSK : STR_SCR1002_LOUDSP_H_FREE_RSK;
		}
		else
	       {
			nRskImgId = IMG_SCR1001_RSK;
			nRskStringId = STR_MITEM2011_CM_END;

		}
#endif
#else
			nRskImgId = IMG_SCR1001_RSK;
			nRskStringId = STR_GLOBAL_ABORT;

#endif
		if(nOutGoingImgId == 1)
		{
			ShowCategory18Screen(0,0, 0, nRskStringId, nRskImgId,
										calling_str_id, dispname, IPNum, IMG_CM_STATE_OUTGOING, imgPath, FALSE,NULL);
		}
		else
		{
			ShowCategory18Screen(0,0, 0, nRskStringId, nRskImgId,
										calling_str_id, dispname, IPNum, nOutGoingImgId, NULL,FALSE,NULL);
		}

#ifdef 	__MMI_CALL_MO_LOUDSPK	
#if ( !defined(__MMI_BT_SUPPORT__))
		SetRightSoftkeyFunction(LoudSpeakerFunction,KEY_EVENT_UP);
#else
		/* Bluetooth Support */
		if( mmi_bt_is_incall_aud_swap2bt() != MMI_TRUE )
		{
			SetRightSoftkeyFunction(LoudSpeakerFunction,KEY_EVENT_UP);
		}
		else
		{
			SetRightSoftkeyFunction(DropRequest,KEY_EVENT_UP);
		}
#endif
#else
		SetRightSoftkeyFunction(DropRequest,KEY_EVENT_UP);
#endif

		SetKeyHandler(DropRequest,KEY_END, KEY_EVENT_DOWN);
    }
	if (GetAlertingFlag())
	{
		SetGroupKeyHandler(EnterScreenAndHandleDigit,(PU16)CMIdleScreenDigits,
							MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);
	}	
	#ifdef __MMI_BT_SUPPORT__		
	if (mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_ACL))//zhou siyou add ,20090730			
		rda_bt_hfg_send_CallSetup_1_ind();
	#endif	
	gdispnameForSubLCD=dispname;
	ShowSubLCDScreen(EntrySubLcdScr1001OutgoingCall);
}

/**************************************************************

	FUNCTION NAME		: ExitScr1001OutgoingCall(void)

  	PURPOSE				: Typical exit screen to add the history variable of the screen.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScr1001OutgoingCall(void)
{
	if(GetCurrentState()==CM_OUTGOING_STATE)
	{
		history_t   Scr1001;
		S16		nHistory	=	0;

		Scr1001.scrnID = SCR1001_CM_OUTGOINGCALLSCREEN;
		Scr1001.entryFuncPtr = EntryScr1001OutgoingCall;
		pfnUnicodeStrcpy((S8*)Scr1001.inputBuffer, (S8*)&nHistory);
		GetCategoryHistory(Scr1001.guiBuffer);
		AddHistory(Scr1001);
	}
}

/**************************************************************

	FUNCTION NAME		: EntrySubLcdScr1001OutgoingCall

  	PURPOSE				: Entry screen for Sub Lcd OutGoing Call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void EntrySubLcdScr1001OutgoingCall(void)
{
	ExecSubLCDCurrExitHandler();

	ShowCategory311Screen(gdispnameForSubLCD,IMG_CM_SUBLCD_CALLING,NULL);
	SetSubLCDExitHandler(ExitSubLcdScr1001OutgoingCall);
}

/**************************************************************

	FUNCTION NAME		: ExitSubLcdScr1001OutgoingCall

  	PURPOSE				: Exit Handler for Sub Lcd OutGoing Call Screen

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void ExitSubLcdScr1001OutgoingCall(void)
{
	if (GetCurrentState()==CM_OUTGOING_STATE &&
		!IsSubLCDScreenPresent(SUB_LCD_SCR1001_CM_OUTGOINGCALLSCREEN))
	{
		SubLCDHistoryNode SubLCDHistory;

		SubLCDHistory.entryFuncPtr=EntrySubLcdScr1001OutgoingCall;
		AddSubLCDHistoryWithScrID(&SubLCDHistory,SUB_LCD_SCR1001_CM_OUTGOINGCALLSCREEN);
	}
}

/**************************************************************

	FUNCTION NAME		: EntryScrOutgoingMultipleOptions(void)

  	PURPOSE				: This function is the entry screen for the outgoing call
						  option menu when there are more than one calls

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrOutgoingMultipleOptions(void)
{
	U8* guiBuffer;
	static U8*	PopUpList[10];
	U16 	nNumofItem;		 /* Stores no of children in the submenu*/
	U16 	nStrItemList[10];/* Stores the strings id of submenus returned */
	U16 menuId;
	S16 nActiveCall;
	U16 nHeldCall;

	nActiveCall = GetTotalActiveCallCount();
	nHeldCall = GetTotalHoldCallCount();

	if (GetIncomingCallHandle() == -1)
	{
		/* no waiting call */
		if (nActiveCall == 1 && nHeldCall == 0)
		{
			menuId = MITEM2008_CM_WAIT_ACT_LSKOPTIONS;
		}
		else if (nActiveCall == 0 && nHeldCall == 1)
		{
			menuId = MITEM2008_CM_WAIT_S_HOLD_LSKOPTIONS;
		}
		else if (nActiveCall > 1 && nHeldCall == 0)
		{
			menuId = MITEM2008_CM_WAIT_MACT_LSKOPTIONS;
		}
		else if (nActiveCall == 0 && nHeldCall > 1)
		{
			menuId = MITEM2008_CM_WAIT_M_HOLD_LSKOPTIONS;
		}
		else
			return;
	}
	else
	{
		/* waiting call present */
		if (nActiveCall == 1 && nHeldCall == 0)
		{
			menuId = MITEM2008_CM_INC_ACT_LSKOPTIONS;
		}
		else if (nActiveCall == 0 && nHeldCall == 1)
		{
			menuId = MITEM2008_CM_INC_S_HOLD_LSKOPTIONS;
		}
		else if (nActiveCall > 1 && nHeldCall == 0)
		{
			menuId = MITEM2008_CM_INC_MACT_LSKOPTIONS;
		}
		else if (nActiveCall == 0 && nHeldCall > 1)
		{
			menuId = MITEM2008_CM_INC_M_HOLD_LSKOPTIONS;
		}
		else
			return;
	}

   EntryNewScreen(ITEM_SCR_OUT_WAIT_HOLD_OPN, NULL,
	               EntryScrOutgoingMultipleOptions, NULL);

	SetCMScrnFlag(TRUE);

	guiBuffer = GetCurrGuiBuffer(ITEM_SCR_OUT_WAIT_HOLD_OPN);
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

	SetKeyHandler(HangupAll,KEY_END, KEY_EVENT_DOWN);

	SetSideVolumeKeysForCM();
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: EntryScr1002ActiveCall(void)

  	PURPOSE				: This displays the list of active/Held calls or the single active/held call.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScr1002ActiveCall(void)
{
	SetCbackAfterSS(NULL);
	if(GetNotifySSFlag())
	{
		PRINT_INFORMATION ((" XXX EntryScr1002ActiveCall: Notify Flag Set XXX\n"));
		/* between call connected (MO or MT) and get time rsp to show active call screen, 
	    if the call is dropped, we stilll need to log the call as connected. */
		if(GetCallHandleForStartTimeUpdate())
		{
			MYTIME t;
			
			DTGetRTCTime(&t);   //invoke RTC time
			UpdateCallStartTimeAndLogCall(GetCallHandleForStartTimeUpdate(),&t);
			SetCallHandleForStartTimeUpdate(0);
		}
		
		 SetCbackAfterSS((FuncPtr)EntryScr1002ActiveCall);
		 return;
	}
    
	DeleteScreenIfPresent(SCR1003_CM_ACTIVECALLOPTIONS);    
	if(GetExitScrnID() != SCR_CM_ACTIVECALLSCREEN)
	{
		EntryNewScreen(SCR_CM_ACTIVECALLSCREEN, NULL, NULL, NULL);
	}

	ReplaceNewScreenHandler(SCR_CM_ACTIVECALLSCREEN, NULL, EntryScr1002ActiveCall);

	SetTransitionToActiveFlag(TRUE);

#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	if (is_redrawing_old_screens())
	{
		MYTIME curtime;
		DTGetRTCTime(&curtime);
		DummyScr1002ActiveCall(&curtime);
	}
	else
#endif	
	CMGetExactTime(DummyScr1002ActiveCall);
}

/**************************************************************

	FUNCTION NAME		: DummyScr1002ActiveCall(void)

	PURPOSE				: This is a dummy function for EntryScr1002ActiveCall()
						  this is made because of asynchronous CMGetExactTime

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  response events.

**************************************************************/
#ifdef __MMI_MULTI_SIM__
	U8	namesarray[MAX_CALLS][MAX_PB_NAME_LENGTH*2 + 40];
#endif
void DummyScr1002ActiveCall(MYTIME *t)
{
	TIMEANDINDEX timeStruct;
	U8*	guiBuffer;
	PU8	strDispnames[MAX_CALLS];
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	PU8	pSimStr;
	U8 	dashsign[4];
#endif /* defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_) */
	U16	nImgIds[MAX_CALLS];
	U16   nRskStringId=0, nLskStringId=0;
	U16	nRskImgId=0;
	S16 	firstActiveCallIndex;
	S16		nActiveCall=0;
	U16		nHoldCall=0;
	U8 		type=255;
	U8		nAllcall;
	BOOL IsSimValid = MMI_FALSE;
	U16 i = 0;

#ifdef __GPRS_MODE__
   S16 dummy;
#endif

	mmi_trace(g_sw_CC,"MMI_Call:  In EntryScr1002ActiveCall\n");

   SetTransitionToActiveFlag(FALSE);
   	 
   	/* CMScrnFlag will be FALSE if CM screen is covered with other application. 
   	If it is the case, don't set back to TRUE for disconnecting process. */
   	//SetCMScrnFlag(TRUE);
	 
   
	/* between call connected (MO or MT) and get time rsp to show active call screen, 
	    if the call is dropped, we stilll need to log the call as connected. */
	if(GetCallHandleForStartTimeUpdate())
	{
		UpdateCallStartTimeAndLogCall(GetCallHandleForStartTimeUpdate(),t);
		SetCallHandleForStartTimeUpdate(0);
	}
   	
	nActiveCall = GetTotalActiveCallCount();
	nHoldCall = GetTotalHoldCallCount();

	if (nActiveCall +  nHoldCall == 0)
   {
      GetOutOfCMApplication();
		return;
   }

	if (GetCurrentState() == CM_INCOMING_STATE)
		return;
	
	if (GetExitScrnID() != SCR_CM_ACTIVECALLSCREEN)
	{
      EntryNewScreen(SCR_CM_ACTIVECALLSCREEN, NULL, EntryScr1002ActiveCall, NULL);
	}
	else
		ExitCategoryFunction();

	 //ShowSubLCDScreen(EntrySubLcdScr1002ActiveCall);     //tangjie del 20060713

	/* SS presnt, don't show active call, restore SS screen */
	if (IsScreenPresent(ITEM_SCR_USSN_MSG) || IsScreenPresent(SCR_CM_REQUESTINGUSSD))
	{
		ResumeSSScrInCall();
		SetCMScrnFlag(FALSE);
		return;
	}

	SetCMScrnFlag(TRUE);
	
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
	ClearAllKeyHandler();

	guiBuffer = GetCurrGuiBuffer(SCR_CM_ACTIVECALLSCREEN);
	//tangjie del begin 20060713
//JIASHUO ADD FOR WAP/MMS,20090212
#ifdef __GPRS_MODE__
	RegisterHighlightHandler(SetCurrHiliteCall);
   gcallListlen=GetAllDispNameorNum(gcallList,gcallListImage,&dummy);
#endif
//tangjie del end 20060713

	nAllcall=GetAllDispNameorNum(strDispnames,nImgIds,&firstActiveCallIndex);
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	pSimStr = GetString(STRING_MTPNP_SIM1 + MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL));

	memset(dashsign, 0x00, 4);
	AnsiiToUnicodeString(dashsign,"-");
	memset(namesarray[0], 0x00, MAX_CALLS * (MAX_PB_NAME_LENGTH*2 + 40));
	for (i=0; i< nAllcall; i++)
	{
		UCS2Strcpy(namesarray[i], pSimStr);
		UCS2Strcat(namesarray[i],dashsign);
		UCS2Strcat(namesarray[i],strDispnames[i]);
		strDispnames[i] = namesarray[i];
	}
#endif /*  defined( MTPNP_AD_CALL_CHANNEL) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_) */
	if(nAllcall != nActiveCall + nHoldCall)
		mmi_trace(g_sw_CC,"MMI_Call: Fatal Error in CM Call Structure 1\n");

	mmi_trace(g_sw_CC,"MMI_Call: Active Calls = %d Hold calls = %d GetTotalCallCount = %d", nActiveCall, nHoldCall,GetTotalCallCount());

	 
	if (GetDisconnectingCallHandle() == -1)
	{
      /* no call in disconnecting state */
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].IsSimValid)
		{
			IsSimValid = MMI_TRUE;
			break;
		}
	}
	if ((IsSimValid) && !EmergencyCallPresent() && !IdleIsPowerOnPeriod())
	{
	   nLskStringId=STR_GLOBAL_OPTIONS;
	}
	else
	{
	   nLskStringId=0;
	}

	if(GetLSpkNowOff())
	{
		SetLSpkNowOff(FALSE);
		StartTimer(CM_NOTIFYDURATION_TIMER, CM_NOTIFY_TIMEOUT-1000, EntryScrLoudSpNotifyScreen);
	}

	if (LoudSpeaker()!=FALSE) //Loud Speaker is ON, to show H-Held
		SetHFreeHeld(TRUE);
	else
		SetHFreeHeld(FALSE);

#if (!defined(__MMI_BT_SUPPORT__))

	nRskImgId=IMG_SCR1002_LOUDSP_RSK;
	nRskStringId=GetHFreeHeld() ? STR_SCR1002_LOUDSP_H_HELD_RSK : STR_SCR1002_LOUDSP_H_FREE_RSK;
#else
	/* Bluetooth Support */
	if( /*mmi_bt_is_aud2hf() != MMI_TRUE */mmi_bt_is_incall_aud_swap2bt() != MMI_TRUE  )
	{
		nRskImgId=IMG_SCR1002_LOUDSP_RSK;
		nRskStringId=GetHFreeHeld() ? STR_SCR1002_LOUDSP_H_HELD_RSK : STR_SCR1002_LOUDSP_H_FREE_RSK;
	}
	else
	{
		nRskStringId=STR_MITEM2011_CM_END;
	}
#endif
	}
	 
   
	InitializeTimeStructure (&timeStruct,t);
#if 0
#ifndef DISABLE_CM_MACROS
	PRINT_INFORMATION(("Time 2 category is - "));
	PRINT_INFORMATION (("%d year\n", timeStruct.time.nYear));
	PRINT_INFORMATION (("%d month\n", timeStruct.time.nMonth));
	PRINT_INFORMATION (("%d day\n", timeStruct.time.nDay));
	PRINT_INFORMATION (("%d hour\n", timeStruct.time.nHour));
	PRINT_INFORMATION (("%d minute\n", timeStruct.time.nMin));
	PRINT_INFORMATION (("%d second\n", timeStruct.time.nSec));
#endif
#endif

	SetParentHandler(0);
//tangjie del 20060713
	#if 0
	#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
	#endif
	#endif
//tangjie del 20060713
  #if ((!defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
   DrawSimulateCenterKey();
  #endif
	if ((nActiveCall + nHoldCall) > 1)
	{
		 
      /* some call is in disconnecting state */
		if (GetDisconnectingCallHandle() != -1)
			ShowCategory19Screen(0,0,0,0, nAllcall, strDispnames, nImgIds,
								    (UI_time*)&timeStruct.time,firstActiveCallIndex, guiBuffer);
      else  /* no call in disconnecting state */
		 
		ShowCategory19Screen(nLskStringId, IMG_SCR1002_LSK, nRskStringId,
								  nRskImgId, nAllcall, strDispnames, nImgIds,
								  (UI_time*)&timeStruct.time,firstActiveCallIndex, guiBuffer);

	}
	else
	{
	   /* only 1 call */
	   type = GetActiveCallType();
	   if (type == CSMCC_FAX_CALL)
	      nImgIds[0] = IMG_FAX_CALL_CONNECT;
	   else if (type == CSMCC_DATA_CALL)
         nImgIds[0] = IMG_DATA_CALL_CONNECT;
      else
      {
	   if (nImgIds[0] == IMG_CM_STATE_ACTIVE)
	   	nImgIds[0] = IMG_CM_STATE_SINGLE_ACTIVE;
	   else if (nImgIds[0] == IMG_CM_STATE_HOLD)
	   	nImgIds[0] = IMG_CM_STATE_SINGLE_HOLD;
	     
         /*else if (nImgIds[0] == IMG_CM_STATE_DISCONNECTING)
         nImgIds[0] = IMG_CM_STATE_SINGLE_DISCONNECTING;*/
      }
	   if (GetDisconnectingCallHandle() != -1)
         ShowCategory20Screen(0,0,0,0, strDispnames[0],(UI_time*)&timeStruct.time,
               nImgIds[0],MMI_TRUE,guiBuffer);
	   else
	    
	   ShowCategory20Screen(nLskStringId, IMG_SCR1002_LSK, nRskStringId,
               nRskImgId, strDispnames[0],(UI_time*)&timeStruct.time,nImgIds[0],MMI_FALSE,guiBuffer);
	   
	}

 
	#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && (defined __MMI_TOUCH_SCREEN__) )
		SetCenterSoftkeyFunction(EnterScreenAndHandleDigit, KEY_EVENT_UP);
	#endif
 
  #if ( !(defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
  SimulateCenterKeyDailFunction(EnterScreenAndHandleDigit, KEY_EVENT_UP);
  #endif


	 
	if (GetDisconnectingCallHandle() != -1)
		SetKeyHandler(ReleaseCall, KEY_END, KEY_EVENT_DOWN);
	else
	{
	 
	SetLeftSoftkeyFunction(EntryScr1003CMActiveCallOptions,KEY_EVENT_UP);

#ifndef __MMI_BT_SUPPORT__
	SetRightSoftkeyFunction(LoudSpeakerFunction,KEY_EVENT_UP);
#else
	/* Bluetooth Support */
	if( mmi_bt_is_incall_aud_swap2bt() != MMI_TRUE )
	{
		SetRightSoftkeyFunction(LoudSpeakerFunction,KEY_EVENT_UP);
	}
	else
	{
		SetRightSoftkeyFunction(HangupAll,KEY_EVENT_UP);
	}
#endif

	if (nActiveCall > 0 && nHoldCall > 0)
	{
		SetKeyHandler(SwapCall,KEY_SEND, KEY_EVENT_DOWN);
#ifdef __SENDKEY2_SUPPORT__
		SetKeyHandler(SwapCall,KEY_SEND1, KEY_EVENT_DOWN);
		SetKeyHandler(SwapCall,KEY_SEND2, KEY_EVENT_DOWN);
#endif

	}
	else if (nActiveCall == 0)
	{
		SetKeyHandler(RetrieveHeldCall,KEY_SEND, KEY_EVENT_DOWN);
#ifdef __SENDKEY2_SUPPORT__
		SetKeyHandler(RetrieveHeldCall,KEY_SEND1, KEY_EVENT_DOWN);
		SetKeyHandler(RetrieveHeldCall,KEY_SEND2, KEY_EVENT_DOWN);
#endif

	}
	else if (IsSimValid && !EmergencyCallPresent())
	{
		SetKeyHandler(MakeActiveCallHold,KEY_SEND, KEY_EVENT_DOWN);
#ifdef __SENDKEY2_SUPPORT__
		SetKeyHandler(MakeActiveCallHold,KEY_SEND1, KEY_EVENT_DOWN);
		SetKeyHandler(MakeActiveCallHold,KEY_SEND2, KEY_EVENT_DOWN);
#endif
	}
	SetKeyHandler(HangupAll,KEY_END, KEY_EVENT_DOWN);

#ifdef __MMI_CM_ARROW_KEY_TO_PHB__
	SetKeyHandler(mmi_phb_list_enter_last_from_active_call, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phb_list_enter_first_from_active_call, KEY_UP_ARROW, KEY_EVENT_DOWN);
#endif
	SetGroupKeyHandler(EnterScreenAndHandleDigit,(PU16)CMIdleScreenDigits,
							MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);

     
        type = GetActiveCallType();
     

	if (type != CSMCC_FAX_CALL && type != CSMCC_DATA_CALL)
	SetSideVolumeKeysForCM();
}
}

/**************************************************************

	FUNCTION NAME		: EntrySubLcdScr1002ActiveCall

  	PURPOSE				: Entry screen for Sub Lcd Active Call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void EntrySubLcdScr1002ActiveCall(void)
{
	ExecSubLCDCurrExitHandler();

	ShowCategory304Screen(IMG_CM_STATE_CONNECTED,NULL);
	SetSubLCDExitHandler(ExitSubLcdScr1002ActiveCall);
}

/**************************************************************

	FUNCTION NAME		: ExitSubLcdScr1002ActiveCall

  	PURPOSE				: Exit Handler for Sub Lcd Active Call Screen

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void ExitSubLcdScr1002ActiveCall(void)
{
	if(!IsSubLCDScreenPresent(SUB_LCD_SCR_CM_ACTIVECALLSCREEN))
	{
		SubLCDHistoryNode SubLCDHistory;

		SubLCDHistory.entryFuncPtr=EntrySubLcdScr1002ActiveCall;
		AddSubLCDHistoryWithScrID(&SubLCDHistory,SUB_LCD_SCR_CM_ACTIVECALLSCREEN);
	}
}

/**************************************************************

	FUNCTION NAME		: EntryScr1003CMActiveCallOptions(void)

  	PURPOSE				: This handles the LSK functionality for the active screen.
						  Displays options depending on the scenario.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScr1003CMActiveCallOptions(void)
{
	U16 parentId;
	U8* guiBuffer;
	U8* inputBuffer;
	static U8*		PopUpList[40];
	U16 nNumofItem;
	U16 nStrItemList[20];
	S16	nActiveCall=0;
	U16	nHoldCall=0;

	/* Added For CPHS */
	/*U16 isConfPresent=0;
	U16 isHoldPresent=0;
	S32 maskingByte=-1;*/

	nActiveCall = GetTotalActiveCallCount();
	nHoldCall = GetTotalHoldCallCount();

	PRINT_INFORMATION(("\n active option: active:%d, held:%d\n", nActiveCall, nHoldCall));

	if (nActiveCall + nHoldCall == 0)
	   return;

	if((nActiveCall==1)&&(nHoldCall==0))
	{
	   U8 type;

	   type = GetActiveCallType();
	   if (type == CSMCC_DATA_CALL || type == CSMCC_FAX_CALL)
	   {
         parentId = 	MITEM2001_CM_SINGLE_DATA_LSKOPTIONS;
	   }
	   else
	   {
			parentId = MITEM2001_CM_SINGLE_ACTIVE_LSKOPTIONS;
			//isHoldPresent=1; /* Added For CPHS */
		}
	}
	else if((nActiveCall==0)&&(nHoldCall==1))
	{
		parentId = MITEM2002_CM_SINGLE_HOLD_LSKOPTIONS;
	}
	else if((nActiveCall==1)&&(nHoldCall==1))
	{
		parentId = MITEM2003_CM_S_ACTIVE_S_HOLD_LSKOPTIONS;
		//isConfPresent=1; /* Added For CPHS */
	}
	else if((nActiveCall>1)&&(nHoldCall==0))
	{
		parentId = MITEM2004_CM_M_ACTIVE_LSKOPTIONS;
	}
	else if((nActiveCall==0)&&(nHoldCall>1))
	{
		parentId = MITEM2005_CM_M_HOLD_LSKOPTIONS;
	}
	else if((nActiveCall>1)&&(nHoldCall==1))
	{
		parentId = MITEM2006_CM_M_ACTIVE_S_HOLD_LSKOPTIONS;
		//isConfPresent=1; /* Added For CPHS */
	}
	else if((nActiveCall==1)&&(nHoldCall>1))
	{
		parentId = MITEM2007_CM_S_ACTIVE_M_HOLD_LSKOPTIONS;
		//isConfPresent=1; /* Added For CPHS */
	}
	else
		return;

	PRINT_INFORMATION(("\n parent:%d\n", parentId));

	EntryNewScreen(SCR1003_CM_ACTIVECALLOPTIONS, ExitScr1003CMActiveCallOptions,
	               NULL, NULL);

	SetCMScrnFlag(TRUE);
	guiBuffer = GetCurrGuiBuffer(SCR1003_CM_ACTIVECALLOPTIONS);
	if (IsScreenPresent(SCR1003_CM_ACTIVECALLOPTIONS))
	{
            inputBuffer = GetCurrInputBuffer(SCR1003_CM_ACTIVECALLOPTIONS);
            if(inputBuffer != NULL)
            {
                if (inputBuffer[0] != (nActiveCall + nHoldCall))
                {
                    /* call count changed, always highlight the first item */
                    guiBuffer = NULL;
                }           
            }
            else
            {
                guiBuffer = NULL;
            }

	}
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	SetParentHandler(parentId);




#if defined( __MMI_BT_SUPPORT__ )
	/*************************************************/
	/** Notice!!!*/
	/** 1. The voice transfer menu item should be the last one in the option menu*/
	/** 2. The voice transfer menu item is only added in XXX_CM_XXX_ACTIVE_XXX parentId. */
	/** 	So this screen has to removed this menu item while BT headset connected */
	/*************************************************/

	if( mmi_bt_is_hs_connected() != MMI_TRUE )
	{
		switch(parentId)
		{
			case MITEM2001_CM_SINGLE_DATA_LSKOPTIONS:
			case MITEM2002_CM_SINGLE_HOLD_LSKOPTIONS:
			case MITEM2005_CM_M_HOLD_LSKOPTIONS:
				break;
			default:				
                mmi_frm_hide_menu_item(MITEM_CM_BT_SETPATH);
                break;

        }
    }
#endif	
#if defined(__MMI_BT_SUPPORT__)
	/* Configure Default Headset */
    if (mmi_bt_is_power_on(MMI_FALSE) == MMI_FALSE || 
        mmi_bt_is_default_hs_existed() == MMI_FALSE)
    {
        switch (parentId)
        {
            case MITEM2001_CM_SINGLE_DATA_LSKOPTIONS:
            case MITEM2002_CM_SINGLE_HOLD_LSKOPTIONS:
            case MITEM2005_CM_M_HOLD_LSKOPTIONS:
                break;
            default:
                mmi_frm_hide_menu_item(MITEM_CM_BT_CONNECT_LAST_HEADSET);
                break;

		}
	}
#endif

     
    nNumofItem = GetNumOfChild_Ext(parentId);
    GetSequenceStringIds_Ext(parentId, nStrItemList);

    ConstructHintsList(parentId, PopUpList);
	PRINT_INFORMATION(("\n active option: %d\n", nNumofItem));

	ShowCategory55Screen(STR_SCR1002_CAPTION, IMG_SCR1002_CAPTION,
						 STR_GLOBAL_OK, IMG_SCR1003_LSK,
						 STR_GLOBAL_BACK, IMG_SCR1003_RSK,
						nNumofItem,nStrItemList,
						(U8 **)PopUpList,0,guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(HangupAll,KEY_END, KEY_EVENT_DOWN);

	SetSideVolumeKeysForCM();
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


void ExitScr1003CMActiveCallOptions(void)
{
	history_t   Scr1001;

#if defined(__MMI_BT_SUPPORT__)
    mmi_frm_unhide_menu_item(MITEM_CM_BT_SETPATH);
	/* Configure Default Headset */
    mmi_frm_unhide_menu_item(MITEM_CM_BT_CONNECT_LAST_HEADSET);
#endif 
	Scr1001.scrnID = SCR1003_CM_ACTIVECALLOPTIONS;
	Scr1001.entryFuncPtr = EntryScr1003CMActiveCallOptions;
    memset (Scr1001.inputBuffer, 0, sizeof (Scr1001.inputBuffer));
	Scr1001.inputBuffer[0] = (U8)GetTotalCallCount();
	GetCategoryHistory(Scr1001.guiBuffer);
	AddHistory(Scr1001);
}
/**************************************************************

	FUNCTION NAME		: EntryScr1004NotifyEndCallDuration(void)

  	PURPOSE				: This is a notification which does not have history and used when the
						  Hangup of a single call is success with a timeout of CM_NOTIFY_TIMEOUT.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void mmi_bth_send_msg(U32 dest_module, U32 sap_id, U32 msg_id, void *p_local_para, void *p_peer_buff);
void EntryScr1004NotifyEndCallDuration(void)
{
	static S8 MessageString[SCR_TEXT_LEN];
	S8 TimeString[TEMP_NO*2];
	S8 TempBuf[TEMP_NO];
	U16 img_id=IMG_ERROR_NOTIFY;
	 
	TurnOnBacklight(1);
	mmi_trace(g_sw_CC," MMI_Call: In EntryScr1004NotifyEndCallDurationXXX GetNotifySSFlag()=%d\n",GetNotifySSFlag());

	SetCbackAfterSS(NULL);
	if(GetNotifySSFlag())
	{
		mmi_trace(g_sw_CC,(" XXX EntryScr1004NotifyEndCallDuration: Notify Flag Set XXX GetCurrentState()=%d\n"),GetCurrentState());
		if ((GetCurrentState() == CM_OUTGOING_STATE) &&
		   (-1 != GetOutgoingCallHandle()))
		   SetCbackAfterSS((FuncPtr)EntryScr1001OutgoingCall);
		else if (/*GetCurrentState() == CM_IDLE_STATE &&*/
		        GetPreviousState() == CM_OUTGOING_STATE || 
		        GetPreviousState() == CM_ACTIVE_STATE)
         SetCbackAfterSS((FuncPtr)EntryScr1004NotifyEndCallDuration);
		else
		   SetCbackAfterSS((FuncPtr)EntryScr1002ActiveCall);
			ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
			StartTimer(CM_NOTIFYDURATION_TIMER, CM_NOTIFY_TIMEOUT, GoBackfromNotifyScr);
		return;
	}

   EntryNewScreen(SCR1004_CM_DISPENDCALLDURATION,
                  ExitScr1004NotifyEndCallDuration,
	               NULL, NULL);
	
	if(GetActivatedCcbsFlag()==TRUE)
	{
		StartTimer(CM_NOTIFYDURATION_TIMER,1, GoBackfromNotifyScr);
		return;
	}

	AnsiiToUnicodeString(TempBuf,"\n\0");

	mmi_trace(g_sw_CC,"MMI_Call: Func: %s GetDropRequestFlag()=%d, GetCurrentState()=%d", __FUNCTION__,GetDropRequestFlag(),GetCurrentState());
	if(GetDropRequestFlag() == TRUE && GetCurrentState() != CM_OUTGOING_STATE)
	{
		pfnUnicodeStrcpy((PS8)MessageString,(PS8)GetString(STR_CALL_ABORTED));
		SetDropRequestFlag(FALSE);
		SetCallEndedBeforeConnFlag(FALSE);
		img_id = IMG_CM_STATE_ENDED;//man
	}
	else
	{
		U16 error_code;

	   switch(GetCallEndCause())
		{
		case ERR_UNASSIGNED_NUM:
		case ERR_INVALID_NUMBER_FORMAT:
	   		error_code = ERROR_UNRECOGNIZED_NUMBER;
			break;

		case ERR_CHANNEL_UN_ACCP:
		case ERR_NO_CIRCUIT_CHANNEL_AVAIL:
		case ERR_NETWORK_OUT_OF_ORDER:
		case ERR_QOS_UNAVAIL:
		case ERR_INVALID_TRANSIT_NW_SEL:
		case ERR_CSMCC_NO_RESPONSE_FROM_NW:
		case ERR_NETWORK_FAILURE:
	   	case ERR_CM_MM_RR_CONN_RELEASE:
		   	error_code = ERROR_NETWORK_FAIL;
			break;

		case ERR_REQUESTED_CKT_CHANEL_NOT_AVIL:
		case ERR_RESOURCE_UNAVAIL_UNSPECIFIED:
		case ERR_CM_MM_AUTH_FAILURE:
	   		error_code = ERROR_NETWORK_NOT_AVAILABLE;
			break;

		case ERR_OPR_DTR_BARRING:
		case ERR_CALL_BARRED:
		case ERR_IC_BAR_CUG:
		case ERR_BEARER_CAP_NOT_AVAIL:
		case ERR_CM_MM_ACCESS_BARRED:
	   		error_code = ERROR_BARRED_CALL;
			break;

		case ERR_NO_USER_RESPONDING:
	   		error_code = ERROR_NOT_RESPONDING;
			break;

		case ERR_NO_ANSWER_ON_ALERT:
	   		error_code = ERROR_NO_ANSWER;
			break;

		case ERR_REQ_FACILITY_UNAVAIL:
		case ERR_IE_NOT_IMPLEMENTED:
		case ERR_CONDITIONAL_MM_IE_ERROR:
		case ERR_CM_MM_ASSIGNMENT_REJECT:
		   	error_code = ERROR_UNAVAILABLE;
			break;

		case ERR_IMSI_UNKNOWN_IN_HLR:
		case ERR_IMSI_UNKNOWN_IN_VLR:
		case ERR_CM_MM_IMSI_DETACH:
		   	error_code = ERROR_IMSI_UNKNOWN;
			break;

		case ERR_IMEI_NOT_ACCEPTED:
	   		error_code = ERROR_IMEI_ERROR;
			break;

   		case ERR_REQ_FAC_NOT_SUBS:
		case ERR_PLMN_NOT_ALLOWED:
		case ERR_LOCATION_AREA_NOT_ALLOWED:
		case ERR_ROAMING_AREA_NOT_ALLOWED:
		case ERR_GPRS_NOT_ALLOWED_IN_PLMN:
		case ERR_NO_SUITABLE_CELLS_IN_LA:
		case ERR_MSC_TEMP_NOT_REACHABLE:
		case ERR_CM_MM_ABORT_BY_NW:
		case ERR_CM_MM_STATE_NOT_ALLOWED_CM:
		case ERR_BEARER_CAP_NOT_AUTHORISED:
	   		error_code = ERROR_NETWORK_NOT_ALLOWED;
			break;

		case ERR_SERV_OPTION_NOT_SUPPORTED:
		case ERR_REQ_SERV_OPTION_NOT_SUBSCRIBED:
		case ERR_SERV_OPTION_TEMP_OUT_OF_ORDER:
		case ERR_CALL_CANNOT_BE_IDENTIFIED:
	   		error_code = ERROR_COMMAND_NOT_SUPPORTED;
			break;

		   case ERR_ACM_EXCEEDED:
		   	error_code = ERROR_ACM_EXCEEDED;
   		   break;

		   case ERR_USER_BUSY:
	   		error_code = ERROR_USER_BUSY;
   		   SetErrorToneFlag(TRUE);
   		   AudioPlayReq(TONE_BUSY_CALL_GSM, DEVICE_AUDIO_PLAY_INFINITE);
		      break;

		   default:
			error_code = STR_CALL_ENDED;
			img_id = IMG_CM_STATE_ENDED;
#ifdef __MMI_BT_SUPPORT__
            if (mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_ACL))
            	{
    		   mmi_bth_send_msg(MOD_BT, BT_APP_SAP, MSG_ID_BT_HFG_INCOMING_DISCONNECT_REQ, NULL, NULL);
            	}
            
#endif
				break;
		}

	      pfnUnicodeStrcpy(MessageString,(PS8)GetString(error_code));
  		 pfnUnicodeStrcat((PS8)MessageString,TempBuf);

		SetCallEndCause(0);

		if(TRUE==GetCallEndedBeforeConnFlag())
		{
		   mmi_trace(g_sw_CC,(" MMI_Call: In EntryScr1004NotifyEndCallDurationXXX -- No time\n"));
			SetCallEndedBeforeConnFlag(FALSE);
		}
		else
		{
			mmi_trace(g_sw_CC,(" MMI_Call: In EntryScr1004NotifyEndCallDurationXXX -- print time\n"));
			duration_string ((UI_time*)&gTimeStructForCallDuration, (UI_string_type)TimeString, DT_ACTIVE_CALL_SCREEN);
			pfnUnicodeStrcat ((PS8)MessageString, TimeString);
		}
	}

  	ShowCategory63Screen((U8*)MessageString,img_id,NULL);

    ClearInputEventHandler(MMI_DEVICE_ALL);
//	SetKeyHandler((FuncPtr)GoBackfromNotifyScr, KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);

	SetPOPUpFlag(TRUE);
	StartTimer(CM_NOTIFYDURATION_TIMER, CM_NOTIFY_TIMEOUT, GoBackfromNotifyScr);
}

/**************************************************************

	FUNCTION NAME		: ExitScr1004NotifyEndCallDuration(void)

  	PURPOSE				: This just stops timer which was started in the entry function.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  added as the notification does not need history.

**************************************************************/
void ExitScr1004NotifyEndCallDuration(void)
{
	StopTimer(CM_NOTIFYDURATION_TIMER);

	if (GetErrorToneFlag() == TRUE)
	{
	   SetErrorToneFlag(FALSE);
	   AudioStopReq(TONE_BUSY_CALL_GSM);
	}

	if(GetPOPUpFlag())
	{
		SetPOPUpFlag(FALSE);
		AdjustHistoryForCM();
	}

	if(CMAllowInterrupt())
	{
		MMIProcessActionMsg();
	}
}

/**************************************************************

	FUNCTION NAME		: EntryScrNotifyCallAborted(void)

  	PURPOSE				: This is a notification which does not have history and used when the
						  Hangup of a single call is success with a timeout of CM_NOTIFY_TIMEOUT.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrNotifyCallAborted(void)
{
	static U8 MessageString[TEMP_NO*3];

   EntryNewScreen(SCR_CM_NOTIFYCALLABORTED,
                  ExitScrNotifyCMPopUp,
	               NULL, NULL);

	PRINT_INFORMATION ((" XXX In EntryScrNotifyCallAborted\n"));

	if(GetDropRequestFlag()==TRUE)
	{
		pfnUnicodeStrcpy((PS8)MessageString,(PS8)GetString(STR_CALL_ABORTED));
		SetDropRequestFlag(FALSE);
	}else
	{
		pfnUnicodeStrcpy((PS8)MessageString,(PS8)GetString(STR_CALL_ENDED));
	}

	SetCallEndedBeforeConnFlag(FALSE);

	ShowCategory63Screen(MessageString,IMG_CM_STATE_ENDED,NULL);

	ClearAllKeyHandler();

	SetPOPUpFlag(TRUE);
	StartTimer(CM_NOTIFYDURATION_TIMER, CM_NOTIFY_TIMEOUT, GoBackfromNotifyScr);
}

/**************************************************************

	FUNCTION NAME		: EntryScr1005NotifyHoldSucess(void)

  	PURPOSE				: This is a notification which does not have history and used when the
						  Hold request is success with a timeout of CM_NOTIFY_TIMEOUT.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScr1005NotifyHoldSucess(void)
{
	PRINT_INFORMATION(("\n EntryScr1005NotifyHoldSucess \n"));
	mmi_trace(1," EntryScr1005NotifyHoldSucess ");
	EntryScrNotifyCHLDSucess(STR_NFY_CALL_HLD, TRUE);
}

/**************************************************************

	FUNCTION NAME		: EntryScr1006NotifyRetrieveSucess(void)

  	PURPOSE				: This is a notification which does not have history and used when the
						  Retrieve request is success with a timeout of CM_NOTIFY_TIMEOUT.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScr1006NotifyRetrieveSucess(void)
{

	EntryScrNotifyCHLDSucess(STR_NFY_CALL_RTVD, FALSE);

}

/**************************************************************

	FUNCTION NAME		: EntryScr1007NotifySwapSucess(void)

  	PURPOSE				: This is a notification which does not have history and used when the
						  Swap request is success with a timeout of CM_NOTIFY_TIMEOUT.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScr1007NotifySwapSucess(void)
{
	if (GetTotalActiveCallCount()>0 && GetTotalHoldCallCount()>0)
		EntryScrNotifyCHLDSucess(STR_NFY_CALL_SWAP, FALSE);
	else if (GetTotalActiveCallCount()>0)
		EntryScrNotifyCHLDSucess(STR_NFY_CALL_RTVD, FALSE);
	else if (GetTotalHoldCallCount()>0)
		EntryScrNotifyCHLDSucess(STR_NFY_CALL_HLD, TRUE);
}

/**************************************************************

	FUNCTION NAME		: EntryScr1008NotifyConfSucess(void)

  	PURPOSE				: This is a notification which does not have history and used when the
						  Conference request is success with a timeout of CM_NOTIFY_TIMEOUT.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScr1008NotifyConfSucess(void)
{

	EntryScrNotifyCHLDSucess(STR_NFY_CALL_CONF, FALSE);
}

/**************************************************************

	FUNCTION NAME		: EntryScr1010NotifySplitSucess(void)

  	PURPOSE				: This is a notification which does not have history and used when the
						  Split request is success with a timeout of CM_NOTIFY_TIMEOUT.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScr1010NotifySplitSucess(void)
{

	EntryScrNotifyCHLDSucess(STR_NFY_CALL_SPLIT, FALSE);
}

/**************************************************************

	FUNCTION NAME		: EntryScrNotifyCHLDSucess(void)

  	PURPOSE				: This is a notification which does not have history and used when the
						  Split request is success with a timeout of CM_NOTIFY_TIMEOUT.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrNotifyCHLDSucess(U16 notify_string, pBOOL play_tone)
{
   EntryNewScreen(SCR1010_CM_DISPSPLITCALLSUCESS,
                  ExitScrNotifyCMPopUpTone,
	               NULL, NULL);

#ifndef __MMI_CM_SHOW_CHLD_DONE__
	ShowCategory62Screen(notify_string,IMG_GLOBAL_ACTIVATED,NULL);
#else
	ShowCategory62Screen(STR_ACTION_DONE,IMAGE_CCBS_ACTIVATED,NULL);
#endif

	SetLeftSoftkeyFunction(GoBackfromNotifyScr,KEY_EVENT_UP);

	if(play_tone == TRUE && GetTotalCallCount()==0)
	{
		playRequestedTone(SUCCESS_TONE_IN_CALL);
		SetErrorToneFlag(TRUE);
	}

	SetKeysForPOPUp();
	SetPOPUpFlag(TRUE);
	StartTimer(CM_NOTIFYDURATION_TIMER, CM_NOTIFY_TIMEOUT, GoBackfromNotifyScr);
}

/**************************************************************

	FUNCTION NAME		: ExitScrNotifyCMPopUpTone(void)

  	PURPOSE				: This just stops timer which was started in the entry function.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  added as the notification does not need history.

**************************************************************/
void ExitScrNotifyCMPopUpTone(void)
{
	if(GetErrorToneFlag()==TRUE)
	{
		stopRequestedTone(SUCCESS_TONE);
		SetErrorToneFlag(FALSE);
	}
	StopTimer(CM_NOTIFYDURATION_TIMER);

	if(GetPOPUpFlag())
	{
		SetPOPUpFlag(FALSE);
		AdjustHistoryForCM();
	}
}

/**************************************************************

	FUNCTION NAME		: EntryScrLoudSpNotifyScreen(void)

  	PURPOSE				: display the notification screen in case of lous speaker

	INPUT PARAMETERS	: gListOperation,gcallList,gcallListlen global variables filled by other function

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void EntryScrLoudSpNotifyScreen(void)
{
	//U8* guiBuffer;

	PRINT_INFORMATION (("XXX EntryScrLoudSpNotifyScreen\n"));

   EntryNewScreen(SCR_CM_NOTIFY_LOUDSPEAKER,
                  ExitScrNotifyCMPopUp,
	               NULL, NULL);

	//guiBuffer = GetCurrGuiBuffer(SCR_CM_NOTIFY_LOUDSPEAKER);

	if (LoudSpeaker() == TRUE)
	   ShowCategory62Screen(STR_NFY_LOUDSPEAKER_ON,IMG_GLOBAL_ACTIVATED,NULL);
	else
	   ShowCategory62Screen(STR_NFY_LOUDSPEAKER_OFF,IMG_GLOBAL_ACTIVATED,NULL);

	SetLeftSoftkeyFunction(GoBackfromNotifyScr,KEY_EVENT_UP);
	SetPOPUpFlag(TRUE);
	SetKeysForPOPUp();
	StartTimer(CM_NOTIFYDURATION_TIMER, CM_NOTIFY_TIMEOUT, GoBackfromNotifyScr);
}

/**************************************************************

	FUNCTION NAME		: ExitScrNotifyCMPopUp(void)

  	PURPOSE				: This function is the exit screen for for displaying SS info to user

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrNotifyCMPopUp(void)
{
	StopTimer(CM_NOTIFYDURATION_TIMER);
	if(GetPOPUpFlag())
	{
		SetPOPUpFlag(FALSE);
		AdjustHistoryForCM();
	}
}

/************************ CISS *************************************/

/**************************************************************

	FUNCTION NAME		: EntryScrUSSDReq

  	PURPOSE				: This function shows USSD screen

	INPUT PARAMETERS	: Nil

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/
void EntryScrUSSDReq(void)
{
	PRINT_INFORMATION (("*** In EntryScrUSSDReq\n"));

	SetAbortCallRequestedFlag(FALSE);

	EntryNewScreen(SCR_CM_REQUESTINGUSSD, NULL,
	               EntryScrUSSDReq, NULL);

//micha1115
	DeleteScreenIfPresent(ITEM_SCR_USSD_MSG);
	DeleteScreenIfPresent(ITEM_SCR_USSN_MSG);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_OUT);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_IN);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_PASS);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_OLD_PASS);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_OPTION);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_NEW_PASS);
	DeleteScreenIfPresent(SCR_ID_CALLSET_BARRING_CFRM_PASS);
	DeleteScreenIfPresent(SCR_ID_CALLSET_FORWARD_OPTIONS);
	DeleteScreenIfPresent(SCR_ID_CALLSET_FORWARD_NUMBER);
	DeleteScreenIfPresent(SCR_ID_CALLSET_FORWARD_NO_ANS_TIME);
	DeleteScreenIfPresent(SCR_ID_CALLSET_WAITING_MAIN);


	ShowCategory8Screen(STR_USSD_REQ_CAPTION, /*IMG_CM_USSD_REQUEST*/IMG_SETTING_CALL_SETUP,
						0,0,
						STR_GLOBAL_ABORT , IMG_SCR_USSD_REQ_LSK,
						STR_USSD_REQ_DISP_SCR,IMG_CM_SS_OUTGOING,
						NULL);

//micha0915
	SetDelScrnIDCallbackHandler( SCR_CM_REQUESTINGUSSD, (HistoryDelCBPtr) SSReqHistoryDelHdlr);

	SetRightSoftkeyFunction(AbortUSSDRequest,KEY_EVENT_UP);
	SetKeyHandler(AbortUSSDRequestEND,KEY_END, KEY_EVENT_DOWN);
}

//micha0915
/*****************************************************************************
* FUNCTION
*     SSReqHistoryDelHdlr
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
void CBackSSReqHistoryDelReq(void *info)
{
    SetAbortCallRequestedFlag(FALSE);
}

U8 SSReqHistoryDelHdlr( void* in_param )
{
	PRINT_INFORMATION (("*** SSReqHistoryDelHdlr\n"));
	
	if(FALSE==GetAbortCallRequestedFlag())
	{
		SetAbortCallRequestedFlag(TRUE);
		MakePsDropUSSDReq(CBackSSReqHistoryDelReq);
	}else
	{
		PRINT_INFORMATION (("*** In  SSReqHistoryDelHdlr.. Not sending ATH\n"));
	}
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*     RemoveSSReqScr
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
void RemoveSSReqScr( void)
{
	SetDelScrnIDCallbackHandler( SCR_CM_REQUESTINGUSSD, (HistoryDelCBPtr) NULL);
	DeleteScreenIfPresent(SCR_CM_REQUESTINGUSSD);
	DeleteScreenIfPresent(ITEM_SCR_USSD_MSG);
}

/**************************************************************

	FUNCTION NAME		: SendUSSDRequest(void)

  	PURPOSE				: Invokes the helper function to hangup or disconnect the call
						  which is yet to connect.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SendUSSDRequest(void)
{
    U8	USSDInputAnsiBuffer[MAX_DIGITS_USSD];
    mmi_trace (g_sw_CC, "func = %s, g_ussd_abort = %d", __FUNCTION__, g_ussd_abort);

    if(g_ussd_abort == TRUE)
    {
        RemoveSSReqScr();
        GoBackHistory();
        return;
    }
    UnicodeToAnsii(USSDInputAnsiBuffer, gUSSDSendReqbuf);
    MakePsSendUSSDReq(USSDInputAnsiBuffer);
}

/**************************************************************

	FUNCTION NAME		: AbortUSSDRequest(void)

  	PURPOSE				: Invokes the helper function to hangup or disconnect the call
						  which is yet to connect.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void AbortUSSDRequest(void)
{
	mmi_trace (g_sw_CC, "func = %s, g_ussd_abort = %d", __FUNCTION__, g_ussd_abort);
    
	if(g_ussd_abort == TRUE)
	{
		GoBackHistory();
		return;
	}
       
	if(FALSE==GetAbortCallRequestedFlag())
	{	
		SetAbortCallRequestedFlag(TRUE);
		MakePsDropUSSDReq(CBackDropUSSDReq);
	}else
	{
		PRINT_INFORMATION (("*** In AbortUSSDRequest Request .. Not sending ATH\n"));
	}
}

/**************************************************************

	FUNCTION NAME		: AbortUSSDRequestEND(void)

	PURPOSE				: This is called when end key is pressed
						  on USSD screen

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void AbortUSSDRequestEND(void)
{
	mmi_trace (g_sw_CC, "func = %s, g_ussd_abort = %d", __FUNCTION__, g_ussd_abort);
    
	if(g_ussd_abort == TRUE)
	{
		DisplayIdleScreen();
		return;
	}
    
	if(FALSE==GetAbortCallRequestedFlag())
	{	
		SetAbortCallRequestedFlag(TRUE);
		MakePsDropUSSDReq(CBackDropUSSDReqEND);
		
	}else
	{
		PRINT_INFORMATION (("*** In AbortUSSDRequest Request .. Not sending ATH\n"));
	}
}

/**************************************************************

	FUNCTION NAME		: ProcessUSSDrecieved

  	PURPOSE				: This function is called by the state machine function in
						  response to the USSD data recieved by the MS

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#define WELCOME_USSD_NEXTTIME	0x02

void ProcessUSSDrecieved(void* info)
{
    if(((MMI_SS_USSD_RSP *)info)->result.flag == L4C_OK)
    {
        DeriveUSSDbuf(info);
        
        if ( (GetIncomingCallHandle() == -1) || (GetActiveScreenId() == SCR_CM_REQUESTINGUSSD))
        {
            RemoveSSReqScr();

            if(((MMI_SS_USSD_RSP *)info)->op_code != 0x06)
            {
                g_ussd_abort = FALSE;
            }

            if(((MMI_SS_USSD_RSP *)info)->op_code == WELCOME_USSD_NEXTTIME)
            {
                EntryScrDisplayUSSDEnd();
            }
            else if(((MMI_SS_USSD_RSP *)info)->op_code == 0x00)
            {
                EntryScrDisplayUSSDOkEnd();
            }	
            else if(((MMI_SS_USSD_RSP *)info)->op_code == 0x06)
            {
                g_ussd_abort = TRUE;
            }	
            else
            {
                EntryScrDisplayUSSDRes();
            }
            }
        else 
        {
            if(((MMI_SS_USSD_RSP *)info)->op_code == WELCOME_USSD_NEXTTIME)
            {
                HistoryReplace(SCR_CM_REQUESTINGUSSD, ITEM_SCR_USSN_MSG, EntryScrDisplayUSSDEnd);
            }
            else if(((MMI_SS_USSD_RSP *)info)->op_code == 0x00)
            {
                HistoryReplace(SCR_CM_REQUESTINGUSSD, ITEM_SCR_USSN_MSG, EntryScrDisplayUSSDOkEnd);
            }
            else
            {
                HistoryReplace(SCR_CM_REQUESTINGUSSD, ITEM_SCR_USSN_MSG, EntryScrDisplayUSSDRes);
            }
        }
    }
    else
    {
        aud_speech_set_mode(KAL_FALSE);//bug 10229, zhangm

        if ( (GetIncomingCallHandle() == -1) || (GetActiveScreenId() == SCR_CM_REQUESTINGUSSD)) 
        {
#ifdef  __MMI_SS_SHOW_CAUSE__
            EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
            RemoveSSReqScr();
            ShowCauseString( ((MMI_SS_USSD_RSP *)info)->result.cause, temp_string);
            ShowCategory74Screen(STR_TITLE_SCR_USSD_MSG,0,
            STR_GLOBAL_OK,IMG_GLOBAL_OK,
            0,0,
            (PU8)temp_string, pfnUnicodeStrlen(temp_string),
            NULL);
            SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
#else
            DisplayPopup( (PU8) GetString( STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0, CS_NOTIFYDURATION, ERROR_TONE);
            RemoveSSReqScr();
#endif
        }
        else 
        {
            RemoveSSReqScr();
        }
    }

    SetDelScrnIDCallbackHandler(ITEM_SCR_USSN_MSG, (HistoryDelCBPtr) NULL);
    SetDelScrnIDCallbackHandler(ITEM_SCR_USSR_EDIT, (HistoryDelCBPtr) NULL);
    DeleteScreenIfPresent(ITEM_SCR_USSN_MSG);
    DeleteScreenIfPresent(ITEM_SCR_USSR_EDIT);
}

