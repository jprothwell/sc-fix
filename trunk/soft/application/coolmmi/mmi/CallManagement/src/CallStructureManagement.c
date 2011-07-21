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
 *   CallStructureManagement.c
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
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/CallManagement/src/CallStructureManagement.c-arc  $
 *
 *    Rev 1.7   Feb 02 2007 20:48:10   lin
 * 添加了几行跟踪调试信息
 *
 *    Rev 1.6   Dec 23 2006 22:35:04   zhangxb
 * 修改trace信息，用mmi_trace()统一调用输出信息
 *
 *    Rev 1.4   Dec 06 2006 12:00:00   lin
 * 1)修改CheckCtrExpiry（）函数
 * 2)修改UpdateCallStartTimeAndLogCall（）函数
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
 *    Rev 1.0   Jul 06 2006 17:42:36   zhangxb
 *
 *******************************************************************************/

#define __NEWSIMULATOR
#include "stdc.h"
#include "mmi_features.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalconstants.h"
#include "debuginitdef.h"
#include "timerevents.h"

#include "wrappergprot.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "keybrd.h"
#include "datetimetype.h"
#include "datetimegprot.h"
#include "phonebookdef.h"
#include "phonebooktypes.h"
#include "phonebookgprot.h"
#include "idleappdef.h"
#include "settinggprots.h"
#include "satgprots.h"
#include "managecallmain.h"
#include "simdetectiongprot.h"

#include "callmanagementiddef.h"
#include "callmanagementstruct.h"
#include "callstructuremanagementprot.h"
#include "callmanagementgprot.h"
#include "commonstubsprot.h"
#include "redialhelpergprot.h"
#include "pscallhelperdef.h"
#include "pscallhelpergprot.h"
#include "incomingcallmanagementprot.h"
#include "outgoingcallmanagementprot.h"
#include "callsgprots.h"
#include "numberlocationgprot.h"
#undef __NEWSIMULATOR

#include "mmi_trace.h"
#include "phonebookgprot.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#include "dual_sim_calllog.h"
#endif /* __MMI_MULTI_SIM__ */

static cm_context_struct g_CallManagement_cntxt;
cm_context_struct *cm_p = &g_CallManagement_cntxt;


//static CALL_STATE gCurrentState = CM_IDLE_STATE; //to manage the current state
//static CALL_STATE gPrevState = CM_IDLE_STATE;	//to manage the previous state
//static CALL_INFO gAllCalls[MAX_CALLS];
//static CALL_INFO gCallStructureForCallLog;

static CM_CALL_HANDLE gCallHandleForStartTimeUpdate;
static MYTIME gTimeStructForEndedCallStartTime;
MYTIME gTimeStructForCallDuration;

static MMI_ANSWERING_MODE gCMAnsMode;
static PHB_CM_INTERFACE gPhoneNumberStruct;
static CHLD_REQ_ACTION gChldReqSent;

//static U16 gNumofActivecalls=0;		//to store the num of active calls.
//static U16 gNumOfHoldCalls=0;		//to store the num of hold calls.
//static U16 gTotalCallCount=0;		//to store the total calls.

static U16 gCallEndCause = 0;
static U32 gCtrTime;
U32 gSumACMValue=0;
#ifdef __MMI_CM_BLACK_LIST__
static pBOOL gClearKeyFlag = TRUE;
#endif
static pBOOL gIsCCCall = TRUE;
static pBOOL gCallAbortRequested = FALSE; //To determine whether abort call has been called once
static pBOOL gDropRequestFlag = FALSE;
static pBOOL gCallAbortReqSentFlag = FALSE;
static pBOOL gCallEndedBeforeConnFlag=FALSE;
static pBOOL gIncomingCallDroppedFlag=FALSE;
static pBOOL gMissedCallFlag = FALSE;
static pBOOL gRejectedCallFlag = FALSE;
static pBOOL gCallWaitFlag=FALSE;
static pBOOL gCcbsFlag = FALSE;
static pBOOL gCcbsFlagActivated = FALSE;
static pBOOL gWait2RingCallFlag= FALSE;
static pBOOL gNoTwiceRingFlag=FALSE;
static pBOOL gErrorToneFlag=FALSE;
static pBOOL gAutoAnswerFlag=FALSE;
 
static pBOOL gAutoReleaseFlag = FALSE;
 
static pBOOL gCallDeflectFlag=FALSE;
static pBOOL gShowAOC=FALSE;
static pBOOL gDTMFKeyUpFlag=TRUE;
static pBOOL gCMScrnFlag=TRUE;
static pBOOL gGetTimeBeforeActive=FALSE;

static pBOOL gWapCallPresent=FALSE;
static pBOOL gPreviousWapCall=FALSE;

static pBOOL gPriority = FALSE;
static pBOOL gSATScrnSavedFlag=FALSE;

static pBOOL gSyncFromAT=FALSE;

#ifdef __IP_NUMBER__
static pBOOL gTempUseIPNum = FALSE;
#endif

extern U8 isIdleScreenReached;
#ifdef __MMI_SUPPORT_JME__
extern s8 vm_status;
#endif

extern UINT8 get_phb_BR(void);
extern void AdjustHistoryForCM(void);
/**************************************************************

	FUNCTION NAME		: InitCallManagement()

  	PURPOSE				: To Initialize the Call Management Application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitCallManagement (void)
{
	InitIncomingCall();
	InitOutGoingCall();
	ResetGlobalValues();
#ifdef __MMI_MULTI_SIM__
	UCMResourceData();
#endif /* __MMI_MULTI_SIM__ */
}

/**************************************************************

	FUNCTION NAME		: ReinitalizeCallManagement()

  	PURPOSE				: To Re-Initialize the Call Management Application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReinitalizeCallManagement(void)
{
     if (cm_p->alert_info.IsCTRStart == TRUE);
         StopTimer(CM_CTR_TIMER);

    ResetGlobalValues();
	ReinitalizeIncomingCall();
	ReinitializeOutgoingCall();
#ifdef __MMI_MULTI_SIM__
	UCMResourceData();
#endif /* __MMI_MULTI_SIM__ */
	SetCurrentState(CM_IDLE_STATE);
	SetPreviousState(CM_IDLE_STATE);
}

/**************************************************************

	FUNCTION NAME		: ResetGlobalValues(void)

  	PURPOSE				: This function resets the state of all the internal
						  call variables. This is to be called at time of exit
						  from CM application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: Index to the location where the call can be stored

	RETURNS				: Index

 

**************************************************************/
void ResetGlobalValues(void)
{
	CALL_INFO	temp;

	memset (&cm_p->alert_info, 0, sizeof(cm_alert_struct));
	memset (&cm_p->redial_info, 0, sizeof(cm_redial_struct));

	/* do not reset history info, notify SS might not finish when get out of CM */

	/* keep call info for call log */
	memcpy (&temp, &cm_p->state_info.CallStructureForCallLog, sizeof(CALL_INFO));
	memset (&cm_p->state_info, 0, sizeof(cm_state_struct));
	memcpy (&cm_p->state_info.CallStructureForCallLog, &temp, sizeof(CALL_INFO));

	cm_p->alert_info.DTMF = TRUE;
	cm_p->history_info.IsSave = FALSE;
	//gNumofActivecalls=0;
	//gNumOfHoldCalls=0;
	//gTotalCallCount=0;
	//gCurrentState = CM_IDLE_STATE;
	//gPrevState = CM_IDLE_STATE;
	gSumACMValue = 0;
	InitCallInfo();
	SetShowAOC(FALSE);
}

/**************************************************************

	FUNCTION NAME		: DeInitCM

	PURPOSE				:

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
#ifdef __MMI_MT_CALL_SPEAKER_MUTE__
extern void ResumeSpeaker();
#endif
void DeInitCM(void)
{

#ifdef __MMI_MT_CALL_SPEAKER_MUTE__
	ResumeSpeaker();
#endif

	if(isInCall())
	{
		HangupAllCalls();
	}
	if (GetCurrentAttempt() > 0)
	{
      ResetRedialAndGoBack();
	}
}

/**************************************************************

	FUNCTION NAME		: InitCallInfo(void)

  	PURPOSE				: Initializes the Call structres array

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void InitCallInfo(void)
{
	U16 index;

	for(index=0;index<MAX_CALLS;index++)
	{
	   memset (&cm_p->state_info.AllCalls[index], 0, sizeof (CALL_INFO));
		cm_p->state_info.AllCalls[index].call_handle = -1;
	}

	cm_p->state_info.NumofActivecalls=0;
	cm_p->state_info.NumofHeldcalls=0;
	cm_p->state_info.TotalCallCount=0;
}

/**************************************************************

	FUNCTION NAME		: ResetCallInfo

  	PURPOSE				: Helper function to reset a particular call structure

	INPUT PARAMETERS	: [1] CM_CALL_HANDLE nCallHandle, // Handle for call structure to reset
						  [2] pBOOL bIsHandle,			  // If the n/w has supplied a handle

   OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void ResetCallInfo(CM_CALL_HANDLE nCallHandle,pBOOL bIsHandle)
{
   S16 index;

	if(bIsHandle)
		index = GetMMIStructIndexof(nCallHandle);
	else
		index = nCallHandle;

   memset (&cm_p->state_info.AllCalls[index], 0, sizeof (CALL_INFO));
	cm_p->state_info.AllCalls[index].call_handle = -1;
}

/**************************************************************

	FUNCTION NAME		: PurgeOutgoingCallsStructure

  	PURPOSE				: Wrapper to reinitialize all outgoing calls structure

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void PurgeOutgoingCallsStructure(void)
{
	U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		 
		if (cm_p->state_info.AllCalls[count].curr_state == CM_OUTGOING_STATE ||
		   (cm_p->state_info.AllCalls[count].prev_state == CM_OUTGOING_STATE && cm_p->state_info.AllCalls[count].curr_state == CM_DISCONNECTING_STATE))
		 
		{
			SetCallState ((CM_CALL_HANDLE)count, CM_IDLE_STATE, FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: PurgeIncomingCallStructure

  	PURPOSE				: Wrapper to reinitialize incoming call structure

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void PurgeIncomingCallStructure(void)
{
	U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		 
		if (cm_p->state_info.AllCalls[count].curr_state == CM_INCOMING_STATE ||
		   (cm_p->state_info.AllCalls[count].prev_state == CM_INCOMING_STATE && cm_p->state_info.AllCalls[count].curr_state == CM_DISCONNECTING_STATE))
		 
		{
			SetCallState ((CM_CALL_HANDLE)count, CM_IDLE_STATE, FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: PurgeAllCalls

  	PURPOSE				: Wrapper to reinitialize all non Idle calls structure

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void PurgeAllCalls(void)
{
	U16 count;

	/* search all non-idle calls and reset them */
	for (count = 0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state != CM_IDLE_STATE)
		{
			SetCallState ((CM_CALL_HANDLE)count, CM_IDLE_STATE, FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SetIncomingCallStateActive

  	PURPOSE				: Function sets the state of incoming call to active

	INPUT PARAMETERS	: MYTIME *time time strucutre to initialize

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetIncomingCallStateActive (void)
{
	U8 count;

	for (count=0;count < MAX_CALLS;count++)
	{
      /* handle race condition:
          when user answer incoming call and drop call very fast, it's possible that the answer
          response comes later than the drop request and the call state will become
          disconnecting, not incoming */
      if (cm_p->state_info.AllCalls[count].curr_state == CM_INCOMING_STATE ||
          (cm_p->state_info.AllCalls[count].curr_state == CM_DISCONNECTING_STATE &&
           cm_p->state_info.AllCalls[count].prev_state == CM_INCOMING_STATE))
		{
			SetCallState ((CM_CALL_HANDLE)cm_p->state_info.AllCalls[count].call_handle,
			             CM_ACTIVE_STATE, TRUE);
		}
	}
}
/**************************************************************

	FUNCTION NAME		: AddNewCallInfo

  	PURPOSE				: Each New Incoming or outging call is stored
						  In the calls array with this function

	INPUT PARAMETERS	: 	[1]	PU8 Number, Number of the call
							[2]	PU8 pBname,	Name of the caller
							[3]	CALL_STATE currentstate, Current state of call wether incoming/outgoing
							[4]	CALL_STATE prevstate,	Previous state of the call
							[5]	CALL_DIR MOflag, 		Whether call is coming or outgoing
							[6]	CM_CALL_HANDLE currentCallHandle,	call handle of the call

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void AddNewCallInfo(PU8 Number,CALL_STATE currentstate,
					CALL_STATE prevstate,CALL_DIR MOflag,
					CM_CALL_HANDLE currentCallHandle, U8 callType)
{
	S16 index;
	PHB_CM_INTERFACE phb_data;

	index = GetFirstFreeIndex();
	mmi_trace(1,"*** AddNewCallInfo index=%d\n", index);

	memset (&gPhoneNumberStruct,0,sizeof(gPhoneNumberStruct));
	memset (&cm_p->state_info.AllCalls[index], 0, sizeof (CALL_INFO));
   gPhoneNumberStruct.pictureId = IMG_PHB_DEFAULT;

	SetPreviousCallWAP(FALSE);

#ifdef __MMI_MULTI_SIM__
	SetCHISToCall();
#endif

	cm_p->state_info.AllCalls[index].call_handle = currentCallHandle;
	cm_p->state_info.AllCalls[index].curr_state = currentstate;
	cm_p->state_info.AllCalls[index].prev_state = prevstate;
	cm_p->state_info.AllCalls[index].status_flag =0;
	cm_p->state_info.AllCalls[index].orgination_flag = MOflag;
	cm_p->state_info.AllCalls[index].call_type = callType;

#ifdef __MMI_CH_QUICK_END__
	/* init quick end time */
	cm_p->alert_info.QuickEndTime = CHISTGetQuickEndTime();
#endif

	if(Number[0]!='\0')
	{
	   /* number existed, search phonebook if necessary */
		pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].number,(PS8)Number);

		if(CM_CALL_MT==MOflag)
		{
		   /* MO call needs to wait till dial_ind to search phonebook for name
		      because number might be changed by SAT */
#ifdef __MMI_MULTI_SIM__
			phb_data = mmi_phb_call_get_data_for_call_mgnt(Number, MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL));
#else
			phb_data = mmi_phb_call_get_data_for_call_mgnt(Number);
#endif
		   SetCMPhoneBookStruct(&phb_data);
         cm_p->state_info.AllCalls[index].name_dcs = gPhoneNumberStruct.name_dcs;
         pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].pBname,(PS8)gPhoneNumberStruct.name);
         if (!(gPhoneNumberStruct.number[0] == 0 && gPhoneNumberStruct.number[1] == 0))
         pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].number,(PS8)gPhoneNumberStruct.number);

            if (MOflag == CM_CALL_MT)
            {
               CMSetupAlertForMT();
            }
#ifdef __MMI_INCOMING_CALL_VIDEO__
            /* waiting call and ccbs invoke can't use video for display */
#ifdef  __MMI_SUPPORT_JME__
            if ((GetTotalCallCount() > 0 || GetCCBSFlag() == TRUE) || (vm_status))
#else
            if (GetTotalCallCount() > 0 || GetCCBSFlag() == TRUE)
#endif
            {
               gPhoneNumberStruct.videoId = 0;
            }
#endif

		#ifdef __MMI_CM_BLACK_LIST__
		   if (IsBlackListNum(Number) == TRUE)
		      cm_p->state_info.AllCalls[index].block_flag = TRUE;
		   else
		      cm_p->state_info.AllCalls[index].block_flag = FALSE;
		#endif
		   //memcpy((PS8)cm_p->state_info.AllCalls[index].pbNumber,(PS8)gPhoneNumberStruct.number,sizeof(cm_p->state_info.AllCalls[index].pbNumber));
		}
		else if (MOflag == CM_CALL_MO_STK)
		{
               PS8 tmpName;
               U8 dcs;

			tmpName = (PS8)SATGetCallDisplay(&dcs);
			if (tmpName != NULL)
			{
                  cm_p->state_info.AllCalls[index].name_dcs = dcs;
                  pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].pBname,tmpName);
			}
         else
         {
            /* if SAT didn't provide name, search phonebook name for SAT call */
#ifdef __MMI_MULTI_SIM__
		phb_data = mmi_phb_call_get_data_for_call_mgnt(Number, MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL));
#else
		phb_data = mmi_phb_call_get_data_for_call_mgnt(Number);
#endif

		SetCMPhoneBookStruct(&phb_data);
            cm_p->state_info.AllCalls[index].name_dcs = gPhoneNumberStruct.name_dcs;
            pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].pBname,(PS8)gPhoneNumberStruct.name);
	         if (!(gPhoneNumberStruct.number[0] == 0 && gPhoneNumberStruct.number[1] == 0))
	         pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].number,(PS8)gPhoneNumberStruct.number);
         }
		}
		else if (CHISTGetDialFromCallLogFlag() != 0)
		{
		   LOG_CALL hilite_item;

         /* dial from call log should directly use name in call log,
            no need to search phonebook */
		   CHISTGetCallLogName(&hilite_item);

		   cm_p->state_info.AllCalls[index].name_dcs = hilite_item.nameDCS;
		   pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].pBname,(PS8)hilite_item.pbName);
			//memcpy((PS8)cm_p->state_info.AllCalls[index].pbNumber,(PS8)cm_p->state_info.AllCalls[index].number,sizeof(cm_p->state_info.AllCalls[index].pbNumber));
		}
	}
	else
	{
		pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].pBname,(PS8)GetString(STR_UNKNOWN_CALLER));
		cm_p->state_info.AllCalls[index].name_dcs = MMI_PHB_UCS2;
	}

	cm_p->state_info.TotalCallCount++;
//	mmi_frm_kbd_set_tone_state(MMI_KEY_VOL_TONE_DISABLED);
}

/**************************************************************

	FUNCTION NAME		: SetCallState

  	PURPOSE				: Sets the call state of a particular call

	INPUT PARAMETERS	: [1] CM_CALL_HANDLE nCallHandle, // Handle for call structure for which flag is to be set
						  [2] CALL_STATE state,			  // State to set
						  [3] pBOOL bIsHandle,			  // If the n/w has supplied a handle

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
						  required for managing the calls like number of hold calls
						  number of active calls etc.

**************************************************************/
extern U8 g_ProcessRejectedCall;
void SetCallState(CM_CALL_HANDLE nCallHandle,CALL_STATE state,pBOOL bIsHandle)
{
	S16 index;

	if(bIsHandle)
		index = GetMMIStructIndexof(nCallHandle);
	else
		index = nCallHandle;
	 
	mmi_trace(g_sw_CC,"MMI_Call: Func: %s state=%d, nCallHandle=%d, index=%d, bIsHandle=%d, prev_state = %d", __FUNCTION__, state, nCallHandle, index, bIsHandle, cm_p->state_info.AllCalls[index].prev_state);
	if (cm_p->state_info.AllCalls[index].curr_state != CM_DISCONNECTING_STATE)
	 
	   cm_p->state_info.AllCalls[index].prev_state = cm_p->state_info.AllCalls[index].curr_state;
	cm_p->state_info.AllCalls[index].curr_state = state;

	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_SetCallState,
					state, cm_p->state_info.AllCalls[index].call_handle));

	if(cm_p->state_info.AllCalls[index].curr_state==CM_ACTIVE_STATE)
	{
	   /* one call becomes active: incoming, outoing, hold -> active*/
		cm_p->state_info.NumofActivecalls++;
		if(cm_p->state_info.AllCalls[index].prev_state==CM_INCOMING_STATE)
		{
			DeleteScreenIfPresent(ITEMSCR_INCOMING_CALL);
		}
		if(cm_p->state_info.AllCalls[index].prev_state==CM_HOLD_STATE)
		{
			cm_p->state_info.NumofHeldcalls--;
		}
	}

	if(cm_p->state_info.AllCalls[index].curr_state==CM_HOLD_STATE)
	{
	   /* one call becomes hold: active -> hold */
		cm_p->state_info.NumofHeldcalls++;
		if(cm_p->state_info.AllCalls[index].prev_state==CM_ACTIVE_STATE)
		{
			cm_p->state_info.NumofActivecalls--;
		}
	}

	if(cm_p->state_info.AllCalls[index].curr_state==CM_IDLE_STATE)
	{
		if(cm_p->state_info.AllCalls[index].prev_state==CM_ACTIVE_STATE)
		{
		   /* one active call ends */
			cm_p->state_info.NumofActivecalls--;
			if(GetWapCallPresent())
			{
			   /* there can be only one active call when wap call present,
			      => the active call is wap call */
				SetWapCallPresent(FALSE);
				SetPreviousCallWAP(TRUE);
			}
		}
		if(cm_p->state_info.AllCalls[index].prev_state==CM_HOLD_STATE)
		{
		   /* one hold call ends */
			cm_p->state_info.NumofHeldcalls--;
		}

		if(cm_p->state_info.AllCalls[index].prev_state==CM_OUTGOING_STATE)
		{
			if(GetTotalCallCount()>1)
			{
				SyncCallList();
			}
			SetAlertingFlag(FALSE);
		   /* one outgoing call ends */
			SetCallEndedBeforeConnFlag(TRUE);
		   /* where to log call? */
			DeleteScreenIfPresent(SCR1001_CM_OUTGOINGCALLSCREEN);
		}
		if(cm_p->state_info.AllCalls[index].prev_state==CM_INCOMING_STATE)
		{
			/* MT call drop when WAP call present, can resume audio */
			if (GetWapCallPresent()) //...
				//mdi_audio_resume_background_play() // zrx del 20060612 ...
				; //...
			if(GetTotalCallCount()>1)
			{
				SyncCallList();
			}
		#ifdef __MMI_CM_BLACK_LIST__
					/* do not log the number that has been blocked */
			if (BlockedHandle(cm_p->state_info.AllCalls[index].call_handle))
			{					
				GetEndTimeAndNotifyCallRejected();
				ResetCallInfo(index,FALSE);
				cm_p->state_info.TotalCallCount--;
				AdjustHistoryForCM();
				return;
		   }
			else
		#endif
		   {
		   /* one incoming call ends */
			SetCallEndedBeforeConnFlag(TRUE);
      //if (!g_ProcessRejectedCall)
			  GetEndTimeAndNotifyCallMissed();
      //else
      //  GetEndTimeAndNotifyCallRejected();
      g_ProcessRejectedCall = 0;
			DeleteScreenIfPresent(ITEMSCR_INCOMING_CALL);
			}
      }
		   /* incoming call exist after call end => it was a waiting call */
      if (GetIncomingCallHandle() != -1 &&
         cm_p->state_info.AllCalls[index].prev_state!=CM_OUTGOING_STATE)
		      SetCallWaitFlag(TRUE);

		ResetCallInfo(index,FALSE);
		cm_p->state_info.TotalCallCount--;
	}






}
 
/**************************************************************

	FUNCTION NAME		: SetAllCallState

  	PURPOSE				: Sets the call state of all calls

	INPUT PARAMETERS	: [1] CALL_STATE state,			  // State to set

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetAllCallState(CALL_STATE state)
{
	U8 count;

	for (count=0; count<MAX_CALLS; count++)
	{
		if ((cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE)||
		   (cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE)||
		   (cm_p->state_info.AllCalls[count].curr_state == CM_OUTGOING_STATE)||
		   (cm_p->state_info.AllCalls[count].curr_state == CM_INCOMING_STATE))
		{
			cm_p->state_info.AllCalls[count].prev_state = cm_p->state_info.AllCalls[count].curr_state;
			cm_p->state_info.AllCalls[count].curr_state = state;
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SetAllExceptWaitingCallState

  	PURPOSE				: Sets the call state of all calls

	INPUT PARAMETERS	: [1] CALL_STATE state,			  // State to set

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetAllExceptWaitingCallState(CALL_STATE state)
{
	U8 count;

	for (count=0; count<MAX_CALLS; count++)
	{
		if ((cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE)||
		   (cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE)||
		   (cm_p->state_info.AllCalls[count].curr_state == CM_OUTGOING_STATE))
		{
			cm_p->state_info.AllCalls[count].prev_state = cm_p->state_info.AllCalls[count].curr_state;
			cm_p->state_info.AllCalls[count].curr_state = state;
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SetAllActiveCallState

  	PURPOSE				: Sets the call state of all active calls

	INPUT PARAMETERS	: [1] CALL_STATE state,			  // State to set

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetAllActiveCallState(CALL_STATE state)
{
	U8 count;

	for (count=0; count<MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE)
		{
			cm_p->state_info.AllCalls[count].prev_state = CM_ACTIVE_STATE;
			cm_p->state_info.AllCalls[count].curr_state = state;
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SetAllHeldCallState

  	PURPOSE				: Sets the call state of all held calls

	INPUT PARAMETERS	: [1] CALL_STATE state,			  // State to set

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetAllHeldCallState(CALL_STATE state)
{
	U8 count;

	for (count=0; count<MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE)
		{
			cm_p->state_info.AllCalls[count].prev_state = CM_HOLD_STATE;
			cm_p->state_info.AllCalls[count].curr_state = state;
		}
	}
}
 
/**************************************************************

	FUNCTION NAME		: GetCallState

  	PURPOSE				: Gets state for a particular call handle

	INPUT PARAMETERS	: call handle

	OUTPUT PARAMETERS	: state of call

	RETURNS				: CM_CALL_STATE

 

**************************************************************/
CALL_STATE GetCallState (CM_CALL_HANDLE handle)
{
	U8 count;

	for (count=0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].call_handle == handle)
		{
			 
			if (cm_p->state_info.AllCalls[count].curr_state != CM_DISCONNECTING_STATE)
			return cm_p->state_info.AllCalls[count].curr_state;
			else
				return cm_p->state_info.AllCalls[count].prev_state;
			 
		}
	}
	return CM_STATE_UNKNOWN;
}

/**************************************************************

	FUNCTION NAME		: SyncCallList

 

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SyncCallList(void)
{
	MakePsGetCallList ((void*)PsCbackSyncCallList);
}

/**************************************************************

	FUNCTION NAME		: PsCbackSyncCallList

  	PURPOSE				: Call back Called after Call List synced

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void PsCbackSyncCallList(void *MsgStruct)
{
	MMI_CALL_LIST_RSP* callRsp;
	MMI_CALL_LIST* callList;
	U8 count;
	S16 index;

	callRsp = (MMI_CALL_LIST_RSP*) MsgStruct;

	if (callRsp->result == KAL_TRUE)
	{
		callList = (MMI_CALL_LIST*)&callRsp->call_list;

		for (count = 0;(count < callList->length)&&(callList->list[count].call_id!=0); count++)
		{
			index=GetMMIStructIndexof(callList->list[count].call_id);

			if(index!=-1)
			{
				switch(callList->list[count].call_state)
				{
					case CLCC_CALL_ACTIVE:
					{
						if (GetSyncCallFromATFlag() == TRUE &&
							cm_p->state_info.AllCalls[index].curr_state == CM_INCOMING_STATE)
						{
							SetCallHandleForStartTimeUpdate(GetIncomingCallHandle());
						}
						cm_p->state_info.AllCalls[index].curr_state=CM_ACTIVE_STATE;
						MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_CallListActive));
						break;
					}
					case CLCC_CALL_HELD:
					{
						cm_p->state_info.AllCalls[index].curr_state=CM_HOLD_STATE;
						MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_CallListHeld));
						break;
					}
					case CLCC_CALL_DIALING:
					{
						cm_p->state_info.AllCalls[index].curr_state=CM_OUTGOING_STATE;
						MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_CallListOutgoing));
						break;
					}
					case CLCC_CALL_ALERTING:
					{
						cm_p->state_info.AllCalls[index].curr_state=CM_OUTGOING_STATE;
						MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_CallListOutgoing));
						break;
					}
					case CLCC_CALL_INCOMING:
					{
						cm_p->state_info.AllCalls[index].curr_state=CM_INCOMING_STATE;
						MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_CallListIncoming));
						break;
					}
					case CLCC_CALL_WAITING:
					{
						cm_p->state_info.AllCalls[index].curr_state=CM_INCOMING_STATE;
						MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_CallListIncoming));
						break;
					}
				}  //End of Switch

			}	//End of if
			else
			{
				MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_ERROR_CallList));
				return;
			}
		}	//End of for

		if(0==(callList->length))
		{
			PurgeAllCalls();
			MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_CallListEmpty));
		}
	}	//End of if
	else
	{
		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_ERROR_CallListFail));
		return;
	}

	UpdateStateMachine();
	UpdateActiveCallScrnIfPresent();
}

/**************************************************************

	FUNCTION NAME		: UpdateStateMachine

  	PURPOSE				: Updates State Machine of CM

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void UpdateStateMachine(void)
{
	S16	NumofActivecalls=0;
	U16	NumOfHoldCalls=0;
	U16	TotalCallCount=0;
	U16 count;
	U8  OutgoingFlag=0;
	U8  IncomingFlag=0;

	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_UpdateStateMachine));

	for (count=0;count<MAX_CALLS;count++)
	{
		if (cm_p->state_info.AllCalls[count].call_handle == -1)
		{
			continue;
		}
		switch(cm_p->state_info.AllCalls[count].curr_state)
		{
			case CM_OUTGOING_STATE:
			{
				OutgoingFlag=1;
				TotalCallCount++;
				break;
			}
			case CM_INCOMING_STATE:
			{
				IncomingFlag=1;
				TotalCallCount++;
				break;
			}
			case CM_ACTIVE_STATE:
			{
				NumofActivecalls++;
				TotalCallCount++;
				break;
			}
			case CM_HOLD_STATE:
			{
				NumOfHoldCalls++;
				TotalCallCount++;
				break;
			}
			default:
				break;
		}

	}
	cm_p->state_info.NumofActivecalls=(U8)NumofActivecalls;
	cm_p->state_info.NumofHeldcalls=(U8)NumOfHoldCalls;
	cm_p->state_info.TotalCallCount=(U8)TotalCallCount;
	if(OutgoingFlag)
	{
		if(GetCurrentState()!=CM_OUTGOING_STATE)
		{
		   SetPreviousState(GetCurrentState());
		   SetCurrentState(CM_OUTGOING_STATE);
		}
		return;
	}
	if(IncomingFlag)
	{
		if(GetCurrentState()!=CM_INCOMING_STATE)
		{
		   SetPreviousState(GetCurrentState());
   		SetCurrentState(CM_INCOMING_STATE);
		}
		return;
	}
	if((cm_p->state_info.NumofHeldcalls==cm_p->state_info.TotalCallCount)&&
		(cm_p->state_info.TotalCallCount!=0))
	{
		if(GetCurrentState()!=CM_HOLD_STATE)
		{
	   	SetPreviousState(GetCurrentState());
		   SetCurrentState(CM_HOLD_STATE);
		}
		return;
	}
	if(cm_p->state_info.NumofActivecalls>0)
	{
		if(GetCurrentState()!=CM_ACTIVE_STATE)
		{
   		SetPreviousState(GetCurrentState());
	   	SetCurrentState(CM_ACTIVE_STATE);
		}
		return;
	}
	if (TotalCallCount == 0)
	{
         SetPreviousState(GetCurrentState());
         SetCurrentState(CM_IDLE_STATE);
	}
}

/**************************************************************

	FUNCTION NAME		: SetHoldFlag

  	PURPOSE				: This function is a wrapper to set state of the hold requested flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetHoldFlag(void)
{
	U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE)
		{
			SetCallflag(count,CM_HOLD_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: ResetHoldFlag

  	PURPOSE				: This function is a wrapper to set state of the hold requested flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void ResetHoldFlag(void)
{
	U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE)
		{
			ResetCallflag(count,CM_HOLD_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SetRetrieveFlag

  	PURPOSE				: This function is a wrapper to set retrieve requested flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetRetrieveFlag(void)
{
	U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE)
		{
			SetCallflag(count,CM_RETRIEVE_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: ResetRetrieveFlag

  	PURPOSE				: This function is a wrapper to set retrieve requested flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void ResetRetrieveFlag(void)
{
	U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE)
		{
			ResetCallflag(count,CM_RETRIEVE_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: GetAllCallFlags

  	PURPOSE				: This function is a wrapper to set swap requested

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
U8 GetAllCallFlags (void)
{
   U8 count, flag = 0;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].call_handle != -1)
		{
			flag |= cm_p->state_info.AllCalls[count].status_flag;
		}
	}
	return flag;
}

/**************************************************************

	FUNCTION NAME		: SetSwapFlag

  	PURPOSE				: This function is a wrapper to set swap requested

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetSwapFlag(void)
{
   U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if ((cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE) ||
		   (cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE))
		{
			SetCallflag(count,CM_SWAP_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: ResetSwapFlag

  	PURPOSE				: This function is a wrapper to set swap requested

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void ResetSwapFlag(void)
{
   U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if ((cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE)||
		   (cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE))
		{
			ResetCallflag(count,CM_SWAP_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SetConfFlag

  	PURPOSE				: This function is a wrapper to set conference requested flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetConfFlag(void)
{
   U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if ((cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE)||
		   (cm_p->state_info.AllCalls[count].curr_state ==CM_ACTIVE_STATE))
		{
			SetCallflag(count,CM_CONF_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: ResetConfFlag

  	PURPOSE				: This function is a wrapper to set conference requested flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void ResetConfFlag(void)
{
   U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if ((cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE)||
		   (cm_p->state_info.AllCalls[count].curr_state ==CM_ACTIVE_STATE))
		{
			ResetCallflag(count,CM_CONF_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: ResetSplitFlag

  	PURPOSE				: This function is a wrapper to set split requested flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void ResetSplitFlag(void)
{
   U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if ((cm_p->state_info.AllCalls[count].status_flag & CM_SPLIT_REQUESTED) != 0)
		{
			ResetCallflag(count,CM_SPLIT_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SetAllHangupFlag

  	PURPOSE				: This function is a wrapper to set hangup all requested flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetAllHangupFlag(void)
{
   U16 count;

   /* release all except waiting */
	for (count = 0; count < MAX_CALLS; count++)
	{
		if ((cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE)||
		   (cm_p->state_info.AllCalls[count].curr_state ==CM_ACTIVE_STATE)||
		   (cm_p->state_info.AllCalls[count].curr_state ==CM_OUTGOING_STATE))
		{
			SetCallflag(count,CM_HANGUP_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SetAllCallsHangupFlag

  	PURPOSE				: This function is a wrapper to set hangup all requested flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetAllCallsHangupFlag(void)
{
   U16 count;

   /* hang up all including waiting or incoming calls */
	for (count = 0; count < MAX_CALLS; count++)
	{
		if(cm_p->state_info.AllCalls[count].curr_state != CM_IDLE_STATE)
		{
			SetCallflag(count,CM_HANGUP_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SetAllActHangupFlag

  	PURPOSE				: This function is a wrapper to set hangup request for all active call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetAllActHangupFlag(void)
{
	U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if(cm_p->state_info.AllCalls[count].curr_state ==CM_ACTIVE_STATE)
		{
			SetCallflag(count,CM_HANGUP_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SetAllHldHangupFlag

  	PURPOSE				: This function is a wrapper to set hangup request for all held calls

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetAllHldHangupFlag(void)
{
   U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if(cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE)
		{
			SetCallflag(count,CM_HANGUP_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: ResetAllHangupFlag

  	PURPOSE				: This function is a wrapper to set hangup all requested flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void ResetAllHangupFlag(void)
{
   U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if ((cm_p->state_info.AllCalls[count].status_flag & CM_HANGUP_REQUESTED) != 0)
		{
			ResetCallflag(count,CM_HANGUP_REQUESTED,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: CheckAllCallflag

  	PURPOSE				: This function is a wrapper to check all call flags

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
BOOL CheckAllCallflag(void)
{
   U16 count;
   U8 flag = 0;

   flag = CM_HOLD_REQUESTED | CM_RETRIEVE_REQUESTED | CM_SPLIT_REQUESTED;

	for (count = 0; count < MAX_CALLS; count++)
	{
		mmi_trace(TRUE,"MMI_Call: Func: %s AllCalls[%d].status_flag=0x%x", __FUNCTION__,count, cm_p->state_info.AllCalls[count].status_flag);
		if ((cm_p->state_info.AllCalls[count].status_flag & flag) != 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: GetChldReqSent(void)

  	PURPOSE				: Helper function to get the
						  action for which CHLD req was sent

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
CHLD_REQ_ACTION GetChldReqSent(void)
{
	return gChldReqSent;
}

/**************************************************************

	FUNCTION NAME		: SetChldReqSent(CHLD_REQ_ACTION action)

  	PURPOSE				: Helper function to set the action for which
						  CHLD req has been sent

	INPUT PARAMETERS	: CHLD_REQ_ACTION

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetChldReqSent(CHLD_REQ_ACTION action)
{
	gChldReqSent=action;
}

/**************************************************************

	FUNCTION NAME		: MakeHold

  	PURPOSE				: This function is a wrapper to hold all active calls

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void MakeHold(void)
{
   U16 count,totalHeld=0;

	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakeHold));
	for (count=0;count<MAX_CALLS;count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE)
		{
			ResetCallflag(count,CM_HOLD_REQUESTED,FALSE);
			SetCallState(count,CM_HOLD_STATE,FALSE);
			totalHeld++;
		}
	}

	if(totalHeld > 0 && GetTotalCallCount()==totalHeld)
	{
		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_MakeHoldSet));
		SetPreviousState(GetCurrentState());
		SetCurrentState(CM_HOLD_STATE);
	}
}

/**************************************************************

	FUNCTION NAME		: MakeRetrieve

  	PURPOSE				: This function is a wrapper to retrieve all held calls

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void MakeRetrieve(void)
{
   U16 count, totalActive=0;

	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakeRetrieve));
	for (count=0;count<MAX_CALLS;count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE)
		{
			ResetCallflag(count,CM_RETRIEVE_REQUESTED,FALSE);
			SetCallState(count,CM_ACTIVE_STATE,FALSE);
		}
	}

	if(totalActive > 0 && GetTotalCallCount()==totalActive)
	{
		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_MakeRetrieveSet));
		SetPreviousState(GetCurrentState());
		SetCurrentState(CM_ACTIVE_STATE);
	}
}

/**************************************************************

	FUNCTION NAME		: MakeSwap

  	PURPOSE				: This function is a wrapper to swap  all held and active calls

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void MakeSwap(void)
{
   U16 count,totalHeld=0,totalActive=0;

	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakeSwap));
	for (count = 0; count < MAX_CALLS; count++)
	{
		if(cm_p->state_info.AllCalls[count].status_flag & CM_SWAP_REQUESTED)
		{
			if(cm_p->state_info.AllCalls[count].curr_state== CM_HOLD_STATE)
			{
				ResetCallflag(count,CM_SWAP_REQUESTED,FALSE);
				SetCallState(count,CM_ACTIVE_STATE,FALSE);
				totalActive++;
			}
			else if(cm_p->state_info.AllCalls[count].curr_state== CM_ACTIVE_STATE)
			{
				ResetCallflag(count,CM_SWAP_REQUESTED,FALSE);
				SetCallState(count,CM_HOLD_STATE,FALSE);
				totalHeld++;
			}
		}
	}
	if(totalHeld > 0 && GetTotalCallCount()==totalHeld)
	{
		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_MakeSwapSetHeld));
		SetPreviousState(GetCurrentState());
		SetCurrentState(CM_HOLD_STATE);
	}
	else if(totalActive > 0 && GetTotalCallCount()==totalActive)
	{
		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_MakeSwapSetActive));
		SetPreviousState(GetCurrentState());
		SetCurrentState(CM_ACTIVE_STATE);
	}
}

/**************************************************************

	FUNCTION NAME		: MakeConf

  	PURPOSE				: This function is a wrapper to conference all held and active calls

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void MakeConf(void)
{
   U16 count;

	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_MakeConf));
	for (count = 0; count < MAX_CALLS; count++)
	{
		if(cm_p->state_info.AllCalls[count].status_flag & CM_CONF_REQUESTED)
		{
			ResetCallflag(count,CM_CONF_REQUESTED,FALSE);
			if(cm_p->state_info.AllCalls[count].curr_state== CM_HOLD_STATE)
			{
			   /* conference makes held -> active, active remains active */
				SetCallState(count,CM_ACTIVE_STATE,FALSE);
			}
		}
	}
}

/**************************************************************

	FUNCTION NAME		: MakeAllActHangup

  	PURPOSE				: This function is a wrapper to hangup all active calls

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void MakeAllActHangup(void)
{
   U16 count;

	for (count = 0; count < MAX_CALLS; count++)
	{
		if(cm_p->state_info.AllCalls[count].status_flag & CM_HANGUP_REQUESTED)
		{
			ResetCallflag(count,CM_HANGUP_REQUESTED,FALSE);
			SetCallState(count,CM_IDLE_STATE,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: MakeSplitSucess

  	PURPOSE				: This function is a wrapper to split conference call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void MakeSplitSucess(void)
{
   U16 count;

   /* only active mpty can be split */
	for (count = 0; count < MAX_CALLS; count++)
	{
		if(cm_p->state_info.AllCalls[count].curr_state ==CM_ACTIVE_STATE)
		{
			if(cm_p->state_info.AllCalls[count].status_flag & CM_SPLIT_REQUESTED)
				ResetCallflag(count,CM_SPLIT_REQUESTED,FALSE);
			else
				SetCallState(count,CM_HOLD_STATE,FALSE);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: AllowMoreCalls

  	PURPOSE				: Function to decide if more incoming calls can be made

	INPUT PARAMETERS	: U16 idx index to the active call

	OUTPUT PARAMETERS	: Call Handle

	RETURNS				: CM_CALL_HANDLE

 

**************************************************************/
pBOOL AllowMoreCalls(void)
{
	U16 temp;

	temp=cm_p->state_info.TotalCallCount;
	if(GetCurrentState()==CM_INCOMING_STATE)
	{
	    
		temp--;
	}
	if (temp >= MAX_CALLS-1)
	{
		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_AllowMoreCalls1));
		return FALSE;
	}
	else if (EmergencyCallPresent())
	{
		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_AllowMoreCalls2));
		return FALSE;
	}
	else if (GetCallAbortReqSentFlag())
	{
		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_AllowMoreCalls3));
		return FALSE;
	}
	else
		return TRUE;
}


/**************************************************************

	FUNCTION NAME		: GetCurrentState

  	PURPOSE				: Helper function to get current state

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: current state

	RETURNS				: CALL_STATE

 

**************************************************************/
CALL_STATE GetCurrentState(void)
{
	return cm_p->state_info.CurrentState;
}

/**************************************************************

	FUNCTION NAME		: GetPreviousState

  	PURPOSE				: Helper function to get previous state

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: current state

	RETURNS				: CALL_STATE

 

**************************************************************/
CALL_STATE GetPreviousState(void)
{
	return cm_p->state_info.PrevState;
}

/**************************************************************

	FUNCTION NAME		: SetCurrentState

  	PURPOSE				: Helper function to set current state

	INPUT PARAMETERS	: CALL_STATE state // state to set as current

	OUTPUT PARAMETERS	: current state

	RETURNS				: nil

 

**************************************************************/
void SetCurrentState(CALL_STATE state)
{
	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_SetCurrentState, state));
	cm_p->state_info.CurrentState = state;
}

/**************************************************************

	FUNCTION NAME		: SetPreviousState

  	PURPOSE				: Helper function to set previous state

	INPUT PARAMETERS	: CALL_STATE state // state to set as previous

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetPreviousState(CALL_STATE state)
{
	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_SetPreviousState, state));
	cm_p->state_info.PrevState = state;
}

/**************************************************************

	FUNCTION NAME		: isInCall

  	PURPOSE				: Function to make check if phone is in call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: if the phone is in call

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL isInCall(void)
{
	if(GetCurrentState()!=CM_IDLE_STATE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/*****************************************************************************
 * FUNCTION
 *  isOutgoingCall
 * DESCRIPTION
 *  For other application to check if outgoing call exists, 
 *  regardless of gsm call or voip call. 
 * PARAMETERS
 *  void
 * RETURNS
 *  TRUE means outgoing call exists, no matter it is in gsm call or voip call;
 *  FALSE means outgoing call not exists, both for gsm call and voip call.
 *****************************************************************************/
pBOOL isOutgoingCall(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_VOIP__
    if ((GetOutgoingCallHandle() != -1) || (mmi_voip_get_outgoing_call_id() != -1))
#else
    if (GetOutgoingCallHandle() != -1)
#endif
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************
 * FUNCTION
 *  isIncomingCall
 * DESCRIPTION
 *  For other application to check if incoming call exists, 
 *  regardless of gsm call or voip call. 
 * PARAMETERS
 *  void
 * RETURNS
 *  TRUE means incoming call exists, no matter it is in gsm call or voip call;
 *  FALSE means incoming call not exists, both for gsm call and voip call.
 *****************************************************************************/
pBOOL isIncomingCall(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_VOIP__
    if ((GetIncomingCallHandle() != -1) || (mmi_voip_get_incoming_call_id() != -1))
#else
    if (GetIncomingCallHandle() != -1)
#endif
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**************************************************************

	FUNCTION NAME		: GetTotalCallCount

  	PURPOSE				: Helper function to get total call count

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: total call count

	RETURNS				: U16

 

**************************************************************/
U16 GetTotalCallCount(void)
{
	return cm_p->state_info.TotalCallCount;
}

/**************************************************************

	FUNCTION NAME		: GetTotalActiveCallCount

  	PURPOSE				: Helper function to get total active call count

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: total active call count

	RETURNS				: U16

 

**************************************************************/
S16 GetTotalActiveCallCount(void)
{
	mmi_trace(g_sw_CC," Func: %s count=%d", __FUNCTION__,cm_p->state_info.NumofActivecalls);
	return cm_p->state_info.NumofActivecalls;
}

/**************************************************************

	FUNCTION NAME		: GetTotalHoldCallCount

  	PURPOSE				: Helper function to get total hold call count

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: total hold call count

	RETURNS				: U16

 

**************************************************************/
U16 GetTotalHoldCallCount(void)
{
	mmi_trace(TRUE,"MMI_Call: Func: %s cm_p->state_info.NumofHeldcalls=%d", __FUNCTION__, cm_p->state_info.NumofHeldcalls);
	return cm_p->state_info.NumofHeldcalls;
}

/**************************************************************

	FUNCTION NAME		: SetCallflag

  	PURPOSE				: Sets a particular call flag

	INPUT PARAMETERS	: [1] CM_CALL_HANDLE nCallHandle, // Handle for call structure for which flag is to be set
						  [2] U8 flag,					  // Flag to set
						  [3] pBOOL bIsHandle,			  // If the n/w has supplied a handle

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetCallflag(CM_CALL_HANDLE nCallHandle,U8 flag,pBOOL bIsHandle)
{
	S16 index;

   if(bIsHandle)
		index = GetMMIStructIndexof(nCallHandle);
	else
		index = nCallHandle;

	mmi_trace(g_sw_CC,"MMI_Call: Func: %s index:%d flag:%d", __FUNCTION__, index, flag);
	cm_p->state_info.AllCalls[index].status_flag |= flag;
}

/**************************************************************

	FUNCTION NAME		: GetCallflag

  	PURPOSE				: Gets call flag

	INPUT PARAMETERS	: [1] CM_CALL_HANDLE nCallHandle, // Handle for call structure for which flag is to be set
						  [2] pBOOL bIsHandle,			  // If the n/w has supplied a handle

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
U8 GetCallflag(CM_CALL_HANDLE nCallHandle,pBOOL bIsHandle)
{
	S16 index;

	if(bIsHandle)
		index = GetMMIStructIndexof(nCallHandle);
	else
		index = nCallHandle;

	if (index == -1)
	{
		PRINT_INFORMATION(("Get Call Flag-- bad Index>>\n"));
		return 0;
	}
	return cm_p->state_info.AllCalls[index].status_flag;
}

/**************************************************************

	FUNCTION NAME		: ResetCallflag

  	PURPOSE				: Sets a particular call flag

	INPUT PARAMETERS	: [1] CM_CALL_HANDLE nCallHandle, // Handle for call structure for which flag is to be set
						  [2] U8 flag,					  // Flag to set
						  [3] pBOOL bIsHandle,			  // If the n/w has supplied a handle

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void ResetCallflag(CM_CALL_HANDLE nCallHandle,U8 flag,pBOOL bIsHandle)
{
   S16 index;

	if(bIsHandle)
		index = GetMMIStructIndexof(nCallHandle);
	else
		index = nCallHandle;

	if (index == -1)
	{
		PRINT_INFORMATION(("Reset Call Flag-- bad Index>>\n"));
		return;
	}
	cm_p->state_info.AllCalls[index].status_flag &= ~flag;
}

/**************************************************************

	FUNCTION NAME		: GetCallDirFlag

  	PURPOSE				: Gets the call direction flag

	INPUT PARAMETERS	: Call Handle

	OUTPUT PARAMETERS	: Call Direction

	RETURNS				: U16

 

**************************************************************/
U16 GetCallDirFlag(CM_CALL_HANDLE handle)
{
	S16 index;

	index = GetMMIStructIndexof(handle);
	return cm_p->state_info.AllCalls[index].orgination_flag;
}

/**************************************************************

	FUNCTION NAME		: GetMMIStructIndexof

  	PURPOSE				: Gets the call index in the array for a specified index

	INPUT PARAMETERS	: [1] CM_CALL_HANDLE nCallIndex,	// Call handle

	OUTPUT PARAMETERS	: Index

	RETURNS				: U16

 

**************************************************************/
S16 GetMMIStructIndexof(CM_CALL_HANDLE nCallIndex)
{
	U16 index;

	for(index=0;index<MAX_CALLS;index++)
	{
		if(cm_p->state_info.AllCalls[index].call_handle == nCallIndex)
		{
			return index;
		}
	}
	mmi_trace(g_sw_CC,("\n Call Structure Corrupted @ GetMMIStructIndexof \n"));
	return -1;
}

/**************************************************************

	FUNCTION NAME		: MapItemId2CallHandle

  	PURPOSE				: Function item Id to the call handle

	INPUT PARAMETERS	: U16 idx index to the active call

	OUTPUT PARAMETERS	: Call Handle

	RETURNS				: CM_CALL_HANDLE

 

**************************************************************/
CM_CALL_HANDLE MapItemId2CallHandle(U16 idx)
{
	U8 count;
	U8 activeCalls=0;

	for (count=0;count<MAX_CALLS;count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE)
		{
			if (idx==activeCalls)
				return cm_p->state_info.AllCalls[count].call_handle;
			activeCalls++;
		}
	}
	return (CM_CALL_HANDLE)-1;
}

/**************************************************************

	FUNCTION NAME		: GetFirstFreeIndex(void)

  	PURPOSE				: This function iterates over the calls strcutre to locate
						  an index to a location where call info can be stored

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: Index to the location where the call can be stored

	RETURNS				: Index

 

**************************************************************/
S16 GetFirstFreeIndex(void)
{
	// must make change here to retrieve call index from PS
	U16 count;

	for (count=0;count<MAX_CALLS;count++)
	{
		if (cm_p->state_info.AllCalls[count].call_handle == -1)
			return count;
	}
	return -1;
}
 
/**************************************************************

	FUNCTION NAME		: GetDisconnectingCallHandle

  	PURPOSE				: Helper function to check if there is any disconnecting call existed

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: CM_CALL_HANDLE

 

**************************************************************/
CM_CALL_HANDLE GetDisconnectingCallHandle(void)
{
	U16 count;

	for (count=0;count<MAX_CALLS;count++)
	{
		if (CM_DISCONNECTING_STATE==cm_p->state_info.AllCalls[count].curr_state)
			return cm_p->state_info.AllCalls[count].call_handle;
	}
	PRINT_INFORMATION(("No Disconnecting Call "));
	return -1;
}
 
/**************************************************************

	FUNCTION NAME		: GetIncomingCallHandle(void)

  	PURPOSE				: Helper function to get handle of incoming calll

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: CM_CALL_HANDLE

 

**************************************************************/
CM_CALL_HANDLE GetIncomingCallHandle(void)
{
	U16 count;

	for (count=0;count<MAX_CALLS;count++)
	{
		 
		mmi_trace(g_sw_CC,"MMI_Call: Func: %s %d: curr_state=%d, prev_state=%d, call_handle=%d", __FUNCTION__, count, cm_p->state_info.AllCalls[count].curr_state, cm_p->state_info.AllCalls[count].prev_state, cm_p->state_info.AllCalls[count].call_handle);
		if (cm_p->state_info.AllCalls[count].curr_state == CM_INCOMING_STATE ||
		   (cm_p->state_info.AllCalls[count].prev_state == CM_INCOMING_STATE && cm_p->state_info.AllCalls[count].curr_state == CM_DISCONNECTING_STATE))
			return cm_p->state_info.AllCalls[count].call_handle;
	}

	mmi_trace(g_sw_CC,("MMI_Call:(WARN) Incoming call not present @ GetIncomingCallHandle\n"));
	return -1;
}

/**************************************************************

	FUNCTION NAME		: SetOutgoingCallHandle

  	PURPOSE				: Sets the call handle for the outgoing call

	INPUT PARAMETERS	: call handle

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetOutgoingCallHandle (CM_CALL_HANDLE handle)
{
	U8 count;

	for (count=0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_OUTGOING_STATE)
		{
			cm_p->state_info.AllCalls[count].call_handle = handle;
			return;
		}

	}
}

/**************************************************************

	FUNCTION NAME		: GetOutgoingCallHandle

  	PURPOSE				: Helper function to get handle of outgoing call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: total hold call count

	RETURNS				: CM_CALL_HANDLE

 

**************************************************************/
CM_CALL_HANDLE GetOutgoingCallHandle(void)
{
	U16 count;

	for (count=0;count<MAX_CALLS;count++)
	{
		 
		if (cm_p->state_info.AllCalls[count].curr_state == CM_OUTGOING_STATE ||
		   (cm_p->state_info.AllCalls[count].prev_state == CM_OUTGOING_STATE && cm_p->state_info.AllCalls[count].curr_state == CM_DISCONNECTING_STATE))
		 
		{
			return cm_p->state_info.AllCalls[count].call_handle;
		}
	}
	return -1;
}

/**************************************************************

	FUNCTION NAME		: GetOutgoingCallIndex

  	PURPOSE				: Helper function to get index of outgoing call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	:

	RETURNS				: CM_CALL_HANDLE

 

**************************************************************/
S8 GetOutgoingCallIndex(void)
{
	INT count;

	for (count=0;count<MAX_CALLS;count++)
	{
		 
		if (cm_p->state_info.AllCalls[count].curr_state == CM_OUTGOING_STATE ||
		   (cm_p->state_info.AllCalls[count].prev_state == CM_OUTGOING_STATE && cm_p->state_info.AllCalls[count].curr_state == CM_DISCONNECTING_STATE))
		 
		{
			return (S8)count;
		}
	}
	return -1;
}

/**************************************************************

	FUNCTION NAME		: GetOutgoingCallOrigin

  	PURPOSE				: Helper function to get origin of outgoing call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	:

	RETURNS				: CALL_DIR

 

**************************************************************/
CALL_DIR GetOutgoingCallOrigin(void)
{
   U16 count;

   for (count=0; count<MAX_CALLS; count++)
   {
      if (cm_p->state_info.AllCalls[count].curr_state== CM_OUTGOING_STATE)
         return cm_p->state_info.AllCalls[count].orgination_flag;
   }
   return CM_CALL_NONE;
}

/**************************************************************

	FUNCTION NAME		: GetCallHandle

  	PURPOSE				: This function is a helper to get handle of by seraching on the name or number

	INPUT PARAMETERS	: PU8 Dispname  - Name or number

	OUTPUT PARAMETERS	: call handle

	RETURNS				: CM_CALL_HANDLE

 

**************************************************************/
CM_CALL_HANDLE GetCallHandle(PU8 Dispname)
{
   U16 count;

	for (count=0;count<MAX_CALLS;count++)
	{
		if(cm_p->state_info.AllCalls[count].call_handle != -1)
		{
			if(*cm_p->state_info.AllCalls[count].pBname)
			{
				if(cm_p->state_info.AllCalls[count].pBname==Dispname)
					return cm_p->state_info.AllCalls[count].call_handle;
			}
			else
			{
				if(cm_p->state_info.AllCalls[count].number ==Dispname)
					return cm_p->state_info.AllCalls[count].call_handle;
			}

		}
	}

	PRINT_INFORMATION(("\n Call Structure Corrupted @ GetCallHandle \n"));
	return -1;
}

/**************************************************************

	FUNCTION NAME		: GetAllDispNameorNum

  	PURPOSE				: This function fills up the array for the respective calls

	INPUT PARAMETERS	: [1] PU8 *Dispnames,	// Array of names or numbers
						  [2] U16 *DispStrids   // Array of image ids

	OUTPUT PARAMETERS	: Number of elements in the array

	RETURNS				: U8

 

**************************************************************/

U8 GetAllDispNameorNum(PU8 *Dispnames,U16	*DispStrids,S16 *HighlightIndex)
{
	S16 index;
	U8 count=0,count1=0,j=0,i=0;
	TIMEANDINDEX tmpTimeBuf;
	TIMEANDINDEX tmpArr[MAX_CALLS];

	for (index=0;index<MAX_CALLS;index++)
	{
		mmi_trace(TRUE,"MMI_Call: Func: %s call_handle=%d, curr_state=%d", __FUNCTION__, cm_p->state_info.AllCalls[index].call_handle, cm_p->state_info.AllCalls[index].curr_state);	
		if ((cm_p->state_info.AllCalls[index].call_handle != -1)&&
		   (cm_p->state_info.AllCalls[index].curr_state!= CM_IDLE_STATE)&&
			(cm_p->state_info.AllCalls[index].curr_state!= CM_INCOMING_STATE))
		{
			if(CM_ACTIVE_STATE==cm_p->state_info.AllCalls[index].curr_state)
			{
				/* Stores the Start Time of the Active Call in tmpArr */
				memcpy(&tmpArr[i].time,&cm_p->state_info.AllCalls[count].start_time,sizeof(MYTIME));
				tmpArr[i].index=count;
				i++; /* Inrease the Active Call Count by 1 */
			}

			if (pfnUnicodeStrlen((PS8)cm_p->state_info.AllCalls[index].pBname)!= 0)
				Dispnames[count]=cm_p->state_info.AllCalls[index].pBname;
			else
				Dispnames[count]=cm_p->state_info.AllCalls[index].number;

			if (CM_DISCONNECTING_STATE==cm_p->state_info.AllCalls[index].curr_state)
				DispStrids[count]=GetImgIdofState(cm_p->state_info.AllCalls[index].prev_state);
			else
				DispStrids[count]=GetImgIdofState(cm_p->state_info.AllCalls[index].curr_state);

			count++;
		}
	}

	/* This loop sorts the Array tmpArr in such a way that
	   the oldest call appears at top */
	for (count1=0;count1<i-1;count1++)
	{
		for (j=0;j<i-1-count1;j++)
		{
			if (FindMaxTime (&tmpArr[j+1].time,&tmpArr[j].time))
			{
				memcpy (&tmpTimeBuf,&tmpArr[j],sizeof(tmpTimeBuf));
				memcpy (&tmpArr[j],&tmpArr[j+1],sizeof(tmpTimeBuf));
				memcpy (&tmpArr[j+1],&tmpTimeBuf,sizeof (tmpTimeBuf));
			}
		}
	}

   /* highlight the latest active call */
	if (i==0)
		*HighlightIndex = 0;
	else
		*HighlightIndex=tmpArr[i-1].index;
	return count;
}

/**************************************************************

	FUNCTION NAME		: GetAllActDispNameorNum

  	PURPOSE				: This function fills up the array for all active calls

	INPUT PARAMETERS	: [1] PU8 *Dispnames,	// Array of names or numbers

	OUTPUT PARAMETERS	: Number of elements in the array

	RETURNS				: U8

 

**************************************************************/
U8 GetAllActDispNameorNum(PU8 *Dispnames)
{
	S16 index;
	U8 count=0;

	for(index=0;index<MAX_CALLS;index++)
	{
		if ((cm_p->state_info.AllCalls[index].call_handle != -1)&&
		   (cm_p->state_info.AllCalls[index].curr_state== CM_ACTIVE_STATE))
		{
			if (pfnUnicodeStrlen((PS8)cm_p->state_info.AllCalls[index].pBname)!= 0)
				Dispnames[count]=cm_p->state_info.AllCalls[index].pBname;
			else
				Dispnames[count]=cm_p->state_info.AllCalls[index].number;
			count++;
		}
	}
	return count;
}

/**************************************************************

	FUNCTION NAME		: GetImgIdofState

  	PURPOSE				: This function returns the respective image id as per the call state

	INPUT PARAMETERS	: [1] CALL_STATE tmpState,	// Call state

	OUTPUT PARAMETERS	: Image Id

	RETURNS				: U16

 

**************************************************************/
U16 GetImgIdofState(CALL_STATE tmpState)
{
	U16 strId=0;

	switch(tmpState)
	{
		case CM_OUTGOING_STATE:
			strId=IMG_CM_STATE_OUTGOING;
			break;

		case CM_INCOMING_STATE:
			strId=IMG_CM_STATE_INCOMING;
			break;

		case CM_ACTIVE_STATE:
			strId=IMG_CM_STATE_ACTIVE;
			break;

		case CM_HOLD_STATE:
			strId=IMG_CM_STATE_HOLD;
			break;
		 
		case CM_DISCONNECTING_STATE:
			strId=IMG_CM_STATE_DISCONNECTING;
			break;
		 
		default:
			PRINT_INFORMATION(("\n CM Event State m/c corrupted \n"));
			break;
	}
	return strId;
}

/**************************************************************

	FUNCTION NAME		: SetCalledNumber

  	PURPOSE				: Sets Call Abort Flag dpeneding on flag

	INPUT PARAMETERS	: flag

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetCalledNumber(S16 index,U8* msg)
{
	memcpy(cm_p->state_info.AllCalls[index].num,cm_p->state_info.AllCalls[index].number,
	         sizeof(cm_p->state_info.AllCalls[index].num));
	memset(cm_p->state_info.AllCalls[index].number,0,sizeof(cm_p->state_info.AllCalls[index].number));

	if(*msg)
	{
		pfnUnicodeStrcpy((S8*)cm_p->state_info.AllCalls[index].number,(S8*)msg);
	}
}

/**************************************************************

	FUNCTION NAME		: SetCalledNumWithType

	PURPOSE				: Sets called number alongwith type field

	INPUT PARAMETERS	: flag

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void SetCalledNumWithType(S16 index,void *msg)
{
	MMI_CONNECT_IND *msgBuf=(MMI_CONNECT_IND*)msg;

	memcpy(cm_p->state_info.AllCalls[index].num,cm_p->state_info.AllCalls[index].number,
	        sizeof(cm_p->state_info.AllCalls[index].num));
	memset(cm_p->state_info.AllCalls[index].number,0,sizeof(cm_p->state_info.AllCalls[index].number));

	cm_p->state_info.AllCalls[index].call_type = msgBuf->call_type;

	if(*(msgBuf->num.number))
	{
		if((msgBuf->num.type) & 0x10)
		{
			AnsiiToUnicodeString((S8*)cm_p->state_info.AllCalls[index].number,"+");
			AnsiiToUnicodeString((S8*)(cm_p->state_info.AllCalls[index].number+2),
			                     (S8*)msgBuf->num.number);
		}
		else
		{
			AnsiiToUnicodeString((S8*)cm_p->state_info.AllCalls[index].number,
			                    (S8*)msgBuf->num.number);
		}
#ifdef __IP_NUMBER__
		if (GetTempUseIPNumber() == FALSE)
#endif
		{
			PHB_CM_INTERFACE phb_data;

			/* IP dial connected number don't search PHB */

			if (gPhoneNumberStruct.dialInList != MMI_PHB_NONE)
				mmi_phb_call_set_dial_from_list(gPhoneNumberStruct.dialInList);

			if (cm_p->redial_info.DialFromPhb != MMI_PHB_NONE)
			   mmi_phb_call_set_dial_from_list(cm_p->redial_info.DialFromPhb);

#ifdef __MMI_MULTI_SIM__
			phb_data = mmi_phb_call_get_data_for_call_mgnt(cm_p->state_info.AllCalls[index].number, MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL));
#else
			phb_data = mmi_phb_call_get_data_for_call_mgnt(cm_p->state_info.AllCalls[index].number);
#endif
			SetCMPhoneBookStruct(&phb_data);
		cm_p->state_info.AllCalls[index].name_dcs = gPhoneNumberStruct.name_dcs;
		pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].pBname,(PS8)gPhoneNumberStruct.name);
         if (!(gPhoneNumberStruct.number[0] == 0 && gPhoneNumberStruct.number[1] == 0))
         pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].number,(PS8)gPhoneNumberStruct.number);
	}
	}
	else
	{
		pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].number,(PS8)GetString(STR_UNKNOWN_CALLER));
	}
}

/**************************************************************

	FUNCTION NAME		: SetOutgoingNamefromPhonebook

	PURPOSE				:

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
void SetOutgoingNamefromPhonebook(void)
{
	S16 index = 0;

	index=GetOutgoingCallIndex();
	mmi_trace(g_sw_CC,"MMI_Call Func: %s outgoing index=%d", __FUNCTION__, index);
	memset((PS8)cm_p->state_info.AllCalls[index].pBname,0,sizeof(cm_p->state_info.AllCalls[index].pBname));

	/*memcpy((PS8)cm_p->state_info.AllCalls[index].pbNumber,(PS8)&gPhoneNumberStruct.number,
	         sizeof(cm_p->state_info.AllCalls[index].pbNumber));*/
	memcpy((PS8)cm_p->state_info.AllCalls[index].pBname,(PS8)gPhoneNumberStruct.name,
	         sizeof(cm_p->state_info.AllCalls[index].pBname));
	cm_p->state_info.AllCalls[index].name_dcs = gPhoneNumberStruct.name_dcs;

}

/**************************************************************

	FUNCTION NAME		: GetOutgoingNumber

  	PURPOSE				: Returns number for outgoing call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: Cause value

	RETURNS				: nil

 

**************************************************************/
PU8 GetOutgoingNumber(void)
{
	U16 count;

	for (count=0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_OUTGOING_STATE)
		{
			return cm_p->state_info.AllCalls[count].number;
		}
	}
	return NULL;
}

/**************************************************************

	FUNCTION NAME		: GetOutgoingName

  	PURPOSE				: Returns name for outgoing call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: Cause value

	RETURNS				: nil

 

**************************************************************/
PU8 GetOutgoingName(void)
{
	U16 count;

	for (count=0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_OUTGOING_STATE && cm_p->state_info.AllCalls[count].pBname)
		{
			return cm_p->state_info.AllCalls[count].pBname;
		}
	}
	return NULL;
}

/**************************************************************

	FUNCTION NAME		: GetOutgoingCallType

	PURPOSE			: Returns the type of outgoing call

	INPUT PARAMETERS	: Nil

	OUTPUT PARAMETERS	: nil

	RETURNS			: Call type

 

**************************************************************/
U8 GetOutgoingCallType(void)
{
	U16 count;

	for (count=0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_OUTGOING_STATE)
		{
			return cm_p->state_info.AllCalls[count].call_type;
		}
	}
	return CSMCC_VOICE_CALL;
}
#ifdef __MMI_CM_BLACK_LIST__
/**************************************************************

	FUNCTION NAME		: BlockedHandle

  	PURPOSE				: Function to check if the call is in black list

	INPUT PARAMETERS	: CM_CALL_HANDLE handle

	OUTPUT PARAMETERS	: nil

	RETURNS				: True means the number is blocked, False means it is not

 

**************************************************************/
pBOOL BlockedHandle(CM_CALL_HANDLE handle)
{
	S16 index = 0;
	U8  black_list_flag, red_list_flag;
	U8 block_flag;

	if (GetBlackListMode())
	{
		index = GetMMIStructIndexof(handle);
	    block_flag = cm_p->state_info.AllCalls[index].block_flag;   //number in black list
	    black_list_flag = mmi_phb_group_in_black_list( cm_p->state_info.AllCalls[index].number); //group in black list
	    red_list_flag = mmi_phb_group_in_red_list( cm_p->state_info.AllCalls[index].number); // group in red list
		if (block_flag     
	            || ((get_phb_BR()==1)&& black_list_flag)  // black mode avail 
 				|| ((get_phb_BR()==2)&& !red_list_flag) )  //red mode avail and not in red list
		{
			mmi_trace(g_sw_CC,"MMI_Call: Func: %s IN __MMI_CM_BLACK_LIST__", __FUNCTION__);
			SetClearKeyFlag(FALSE);
			return TRUE;
		}
		else
		{
			mmi_trace(g_sw_CC,"MMI_Call: Func: %s NOT IN __MMI_CM_BLACK_LIST__", __FUNCTION__);
			SetClearKeyFlag(TRUE);
			return FALSE;
		}
	}
	return FALSE;
}

#endif
/**************************************************************

	FUNCTION NAME		: GetCallName

  	PURPOSE				: Function gets the name or number of incoming call

	INPUT PARAMETERS	: CM_CALL_HANDLE handle

	OUTPUT PARAMETERS	: Pointer to buffer containing name or number

	RETURNS				: PS8

 

**************************************************************/
PU8 GetCallName(CM_CALL_HANDLE handle)
{
	S16 index = GetMMIStructIndexof(handle);

	if (pfnUnicodeStrlen((PS8)cm_p->state_info.AllCalls[index].pBname)!= 0)
		return cm_p->state_info.AllCalls[index].pBname;

	if (!cm_p->state_info.AllCalls[index].number)
	{
	   /* unknown number */
		pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].number,
		               (PS8)GetString(STR_UNKNOWN_CALLER));
	}
	return cm_p->state_info.AllCalls[index].number;
}

#ifdef __MMI_NUMBER_LOCATION__
/**************************************************************

	FUNCTION NAME		: GetCallNameWithAddress

  	PURPOSE				: Function gets the name or number with address of incoming call

	INPUT PARAMETERS	: CM_CALL_HANDLE handle

	OUTPUT PARAMETERS	: Pointer to buffer containing name or number

	RETURNS				: PS8

	REMARKS				: nil

**************************************************************/
#define MAX_ADDRESS_NAME_LENGTH 40
static U8	NameNumerWithAddress[MAX_CM_NUMBER + MAX_ADDRESS_NAME_LENGTH*ENCODING_LENGTH];
PU8 GetCallNameWithAddress(CM_CALL_HANDLE handle)
{
	S8 unicode_space[6];
	BOOL bHasName = FALSE;
	BOOL bNeedSearchAddress = TRUE;
	
	S16 index = GetMMIStructIndexof(handle);

	memset(NameNumerWithAddress, 0, sizeof(NameNumerWithAddress));

	if (pfnUnicodeStrlen((PS8)cm_p->state_info.AllCalls[index].pBname)!= 0)
	{
		pfnUnicodeStrcpy((S8*)NameNumerWithAddress, (const S8*)cm_p->state_info.AllCalls[index].pBname);
		bHasName = TRUE;
	}

	if(!bHasName)
	{
		if (!cm_p->state_info.AllCalls[index].number)
		{
		   /* unknown number */
			pfnUnicodeStrcpy((PS8)cm_p->state_info.AllCalls[index].number,
			               (PS8)GetString(STR_UNKNOWN_CALLER));
		      bNeedSearchAddress = FALSE;
		}
		
		pfnUnicodeStrcpy((S8*)NameNumerWithAddress, (const S8*)cm_p->state_info.AllCalls[index].number);
	}
	
	if(bNeedSearchAddress)
	{
		AnsiiToUnicodeString(unicode_space, " ");
		pfnUnicodeStrcat((S8*)NameNumerWithAddress, (const S8*)unicode_space);
		pfnUnicodeStrcat((S8*)NameNumerWithAddress, (const S8*)getLoactionStrFromNumber(cm_p->state_info.AllCalls[index].number));
	}

	return NameNumerWithAddress;
	
}
#endif
/**************************************************************

	FUNCTION NAME		: GetCallNumber

  	PURPOSE				: Returns number for outgoing call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: Cause value

	RETURNS				: nil

 

**************************************************************/
PU8 GetCallNumber(CM_CALL_HANDLE handle)
{
	S16 index = GetMMIStructIndexof(handle);

	if (index != -1)
		return cm_p->state_info.AllCalls[index].number;
	else
		return NULL;
}

/**************************************************************

	FUNCTION NAME		: GetCallType

  	PURPOSE				: Function gets the type of incoming call

	INPUT PARAMETERS	: CM_CALL_HANDLE handle

	OUTPUT PARAMETERS	: nil

	RETURNS				: Call Type

 

**************************************************************/
U8 GetCallType(CM_CALL_HANDLE handle)
{
	S16 index = GetMMIStructIndexof(handle);

	return cm_p->state_info.AllCalls[index].call_type;
}

/**************************************************************

	FUNCTION NAME		: GetCallImgId

  	PURPOSE				: Function gets the image id for an incoming call

	INPUT PARAMETERS	: CM_CALL_HANDLE handle

	OUTPUT PARAMETERS	: Image Id

	RETURNS				: U16

 

**************************************************************/
U16 GetCallImgId(CM_CALL_HANDLE handle)
{
    S16 index = GetMMIStructIndexof(handle);
    mmi_trace(g_sw_PHB, "MMI_Call: Func: %s index:%d", __FUNCTION__, index);

// Caller Video
#ifdef __MMI_INCOMING_CALL_VIDEO__
    /* incoming call with video set */
    if (cm_p->state_info.AllCalls[index].number && gPhoneNumberStruct.videoId != 0)
    {
        if (gPhoneNumberStruct.videoId & 0x8000)
        {
            /* check file presence before use it */
            S8 *path;
            FS_HANDLE fh;
            path = GetCallVideoPath(handle);
            mmi_trace(1, "-----+++++path = %s+++++-----", path);
            if (path != NULL)
            {
                fh = MMI_FS_Open ((PU8)path, FS_READ_ONLY);//, 0);
                mmi_trace(1, "-----+++++fh = %d+++++-----", fh);
                if (fh >= FS_NO_ERROR)
                {
                    MMI_FS_Close (fh);
                    /* use phonebook video if matched */
                    mmi_trace(1, "-----+++++GetCallImgId-videoId = %d+++++-----", gPhoneNumberStruct.videoId);
                    return gPhoneNumberStruct.videoId;
                }
            }
        }
        else
        {
            return gPhoneNumberStruct.videoId;
        }
    }
#endif

// Caller Picture
    if (cm_p->state_info.AllCalls[index].number && gPhoneNumberStruct.pictureId != 0)
    {
        /* use phonebook image if matched */
        return gPhoneNumberStruct.pictureId;
    }
    else
    {
        /* use default image */
        return IMG_PHB_DEFAULT;
    }
}

#ifdef __MMI_PHB_CALL_SHOW_PICTURE_FROM_FILE__
/**************************************************************

	FUNCTION NAME		: GetCallImgPath

  	PURPOSE				: Function gets the image path for an incoming call

	INPUT PARAMETERS	: CM_CALL_HANDLE handle

	OUTPUT PARAMETERS	: Image path

	RETURNS				: S8 *

 

**************************************************************/
extern S8* mmi_phb_image_get_path_from_id(U16 index);

S8* GetCallImgPath(CM_CALL_HANDLE handle)
{
	U16 RecordIndex;
	S16 index = GetMMIStructIndexof(handle);
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	TRACE_FMGR_FUNCTION();
	if (cm_p->state_info.AllCalls[index].number)
	{
		RecordIndex = gPhoneNumberStruct.record_index;
		if (RecordIndex == 0)
			return NULL;
		else
			return mmi_phb_image_get_path_from_id(RecordIndex);
	}
	else if (pfnUnicodeStrcmp((PS8)cm_p->state_info.AllCalls[index].number,
									GetString(STR_UNKNOWN_CALLER)))
	{
		return NULL;
	}
	return NULL;
}
#endif

#ifdef  __MMI_INCOMING_CALL_VIDEO__
/**************************************************************

	FUNCTION NAME		: GetCallVideoPath

  	PURPOSE				: Function gets the video path for an incoming call

	INPUT PARAMETERS	: CM_CALL_HANDLE handle

	OUTPUT PARAMETERS	: Video path

	RETURNS				: S8 *

 

**************************************************************/
extern S8* mmi_phb_video_get_path_from_id(U16 record_index);

S8* GetCallVideoPath(CM_CALL_HANDLE handle)
{
	U16 RecordIndex;
	S16 index = GetMMIStructIndexof(handle);
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

	if (cm_p->state_info.AllCalls[index].number)
	{
		RecordIndex = gPhoneNumberStruct.video_record_index;
		if (RecordIndex == 0)
			return NULL;
		else
			return mmi_phb_video_get_path_from_id(RecordIndex);
	}
	else if (pfnUnicodeStrcmp((PS8)cm_p->state_info.AllCalls[index].number,
									GetString(STR_UNKNOWN_CALLER)))
	{
		return NULL;
	}
	return NULL;
}
#endif

/**************************************************************

	FUNCTION NAME		: GetActiveCallType

  	PURPOSE				: Function gets the call type for an active call

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	: call type

	RETURNS				: U16

 

**************************************************************/
U8 GetActiveCallType(void)
{
	U16 count;

	for (count=0; count < MAX_CALLS; count++)
	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE)
		{
			return cm_p->state_info.AllCalls[count].call_type;
		}
	}
	return CSMCC_VOICE_CALL;
}

/**************************************************************

	FUNCTION NAME		: GetDispNameorNum

  	PURPOSE				: Gets the name or number for a particular call handle

	INPUT PARAMETERS	: [1] CM_CALL_HANDLE nCallIndex,	// Call handle

	OUTPUT PARAMETERS	: Pointer to the name or number buffer

	RETURNS				: PU8

 

**************************************************************/
PU8 GetDispNameorNum(void)
{
	S16 index;

	for (index=0; index < MAX_CALLS;index++)
	{
		 
		if (cm_p->state_info.AllCalls[index].curr_state == CM_OUTGOING_STATE ||
		    cm_p->state_info.AllCalls[index].curr_state == CM_DISCONNECTING_STATE)
		 
		{
         if((pfnUnicodeStrlen((PS8)cm_p->state_info.AllCalls[index].pBname) == 0)
#ifdef __IP_NUMBER__
            || GetTempUseIPNumber()
#endif
         )
			{
            /* hide *31#, #31# prefix string  */
			   	if ((pfnUnicodeStrncmp((PS8)cm_p->state_info.AllCalls[index].number, (PS8)L"*31#", 4) == 0) ||
			      	(pfnUnicodeStrncmp((PS8)cm_p->state_info.AllCalls[index].number, (PS8)L"#31#", 4) == 0))
			      return &cm_p->state_info.AllCalls[index].number[8];
			   else
				   return cm_p->state_info.AllCalls[index].number;
		   }
			else
			{
				return cm_p->state_info.AllCalls[index].pBname;
			}
	   }
	}
	return NULL;
}
#ifdef __MMI_NUMBER_LOCATION__
/**************************************************************

	FUNCTION NAME		: GetDispNameorNumWithAddress

  	PURPOSE				: Gets the name or number for a particular call handle

	INPUT PARAMETERS	: [1] CM_CALL_HANDLE nCallIndex,	// Call handle

	OUTPUT PARAMETERS	: Pointer to the name or number buffer

	RETURNS				: PU8

	REMARKS				: nil

**************************************************************/
PU8 GetDispNameorNumWithAddress(void)
{
	S16 index;
	S8 unicode_space[6];

	for (index=0; index < MAX_CALLS;index++)
	{
		if (cm_p->state_info.AllCalls[index].curr_state == CM_OUTGOING_STATE ||
			cm_p->state_info.AllCalls[index].curr_state == CM_DISCONNECTING_STATE)
		{
			memset(NameNumerWithAddress, 0, sizeof(NameNumerWithAddress));
			if((pfnUnicodeStrlen((PS8)cm_p->state_info.AllCalls[index].pBname) == 0)
		#ifdef __IP_NUMBER__
				|| GetTempUseIPNumber()
		#endif
			)
			{
				/* hide *31#, #31# prefix string  */
				if ((pfnUnicodeStrncmp((PS8)cm_p->state_info.AllCalls[index].number, (PS8)L"*31#", 4) == 0) ||
					(pfnUnicodeStrncmp((PS8)cm_p->state_info.AllCalls[index].number, (PS8)L"#31#", 4) == 0))
				{
					pfnUnicodeStrcpy((S8*)NameNumerWithAddress, (const S8*)&cm_p->state_info.AllCalls[index].number[8]);
					return NameNumerWithAddress;
				}
				else
				{
					pfnUnicodeStrcpy((S8*)NameNumerWithAddress, (const S8*)cm_p->state_info.AllCalls[index].number);
					AnsiiToUnicodeString(unicode_space, " ");
					pfnUnicodeStrcat((S8*)NameNumerWithAddress, (const S8*)unicode_space);
					pfnUnicodeStrcat((S8*)NameNumerWithAddress, (const S8*)getLoactionStrFromNumber(cm_p->state_info.AllCalls[index].number));
					return NameNumerWithAddress;
				}
			}
			else
			{
				pfnUnicodeStrcpy((S8*)NameNumerWithAddress, (const S8*)cm_p->state_info.AllCalls[index].pBname);
				AnsiiToUnicodeString(unicode_space, " ");
				pfnUnicodeStrcat((S8*)NameNumerWithAddress, (const S8*)unicode_space);
				pfnUnicodeStrcat((S8*)NameNumerWithAddress, (const S8*)getLoactionStrFromNumber(cm_p->state_info.AllCalls[index].number));
				return NameNumerWithAddress;
			}
		}
	}
	return NULL;
}
#endif
#ifdef __MMI_CM_BLACK_LIST__
/**************************************************************

	FUNCTION NAME		: SetClearKeyFlag

  	PURPOSE				: it is the derivative function for auto-rejected black list incoming call to determine
  						  if the clear key handler should be called before rejecting a call

	INPUT PARAMETERS	: pBOOL

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetClearKeyFlag(pBOOL value)
{
	gClearKeyFlag=value;
}

/**************************************************************

	FUNCTION NAME		: GetClearKeyFlag

  	PURPOSE				: it is the derivative function for auto-rejected black list incoming call to determine
  						  if the clear key handler should be called before rejecting a call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: True means the clear key handler should be called, False means it should not

 

**************************************************************/
pBOOL GetClearKeyFlag(void)
{
	return gClearKeyFlag;
}
#endif
/**************************************************************

	FUNCTION NAME		: SetCCFlag

  	PURPOSE				: Sets flag is this is normal voice call

	INPUT PARAMETERS	: TRUE/FALSE

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetCCFlag(pBOOL value)
{
	gIsCCCall = value;
}

/**************************************************************

	FUNCTION NAME		: GetCCFlag

  	PURPOSE				: Returns true if this is voice call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE/FALSE

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL GetCCFlag(void)
{
	return gIsCCCall;
}

/**************************************************************

	FUNCTION NAME		: SetTransitionToActiveFlag

  	PURPOSE				: Sets flag for get time before enter active call
  							  screen

	INPUT PARAMETERS	: TRUE/FALSE

	OUTPUT PARAMETERS	:

	RETURNS				:
 

**************************************************************/
void SetTransitionToActiveFlag (pBOOL flag)
{
	gGetTimeBeforeActive = flag;
}

/**************************************************************

	FUNCTION NAME		: GetTransitionToActiveFlag

  	PURPOSE				: Returns flag for get time before enter active call
  							  screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE/FALSE

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL GetTransitionToActiveFlag (void)
{
   return gGetTimeBeforeActive;
}

void SetAnswerMode (void *mode)
{
	if (mode == NULL)
		memset (&gCMAnsMode, 0, sizeof(MMI_ANSWERING_MODE));
	else
		memcpy (&gCMAnsMode, mode, sizeof(MMI_ANSWERING_MODE));
}

void *GetAnswerMode(void)
{
	return &gCMAnsMode;
}

/**************************************************************

	FUNCTION NAME		: SetAutoAnswerFlag

  	PURPOSE				:

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetAutoAnswerFlag(pBOOL flag)
{
	gAutoAnswerFlag = flag;
}

/**************************************************************

	FUNCTION NAME		: GetAutoAnswerFlag

  	PURPOSE				: Gets the AutoAnswerFlag Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetAutoAnswerFlag(void)
{
	return gAutoAnswerFlag;
}
 
/**************************************************************

	FUNCTION NAME		: SetAutoReleaseFlag

  	PURPOSE				: Set AutoReleaseFlag to determine if auto release is needed

	INPUT PARAMETERS	: TRUE for auto release all disconnecting calls, FALSE otherwise

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetAutoReleaseFlag(pBOOL flag)
{
	gAutoReleaseFlag = flag;
}

/**************************************************************

	FUNCTION NAME		: GetAutoReleaseFlag

  	PURPOSE				: Gets the AutoReleaseFlag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for auto release all disconnecting calls, FALSE otherwise

	RETURNS				: nil

 

**************************************************************/
pBOOL GetAutoReleaseFlag(void)
{
	return gAutoReleaseFlag;
}
 
/**************************************************************

	FUNCTION NAME		: GetHungUpHandle

  	PURPOSE				: Helper function to get handle of hungup call

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: total hold call count

	RETURNS				: U16

 

**************************************************************/
CM_CALL_HANDLE GetHungUpHandle(void)
{
	U8 count;

	for (count=0;count < MAX_CALLS;count++)
	{
		if ((cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE ||
		    cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE) &&
			(cm_p->state_info.AllCalls[count].status_flag & CM_HANGUP_REQUESTED))
		   return cm_p->state_info.AllCalls[count].call_handle;
	}
	return -1;
}

/**************************************************************

	FUNCTION NAME		: SetAbortCallRequestedFlag

  	PURPOSE				: Sets Call Abort Flag dpeneding on flag

	INPUT PARAMETERS	: flag

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetAbortCallRequestedFlag(pBOOL flag)
{
	mmi_trace(g_sw_CC," Func: %s flag=%d", __FUNCTION__,flag);
	gCallAbortRequested=flag;
}

/**************************************************************

	FUNCTION NAME		: GetAbortCallRequestedFlag

  	PURPOSE				: Returns Call Abort Flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: flag

 

**************************************************************/
pBOOL GetAbortCallRequestedFlag(void)
{
	return gCallAbortRequested;
}

/**************************************************************

	FUNCTION NAME		: SetCallAbortReqSentFlag

  	PURPOSE				: Sets Call Abort sent Flag dpeneding on flag

	INPUT PARAMETERS	: flag

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetCallAbortReqSentFlag (pBOOL flag)
{
   gCallAbortReqSentFlag = flag;
}

/**************************************************************

	FUNCTION NAME		: GetCallAbortReqSentFlag

  	PURPOSE				: Returns Call Abort sent Flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: flag

 

**************************************************************/
pBOOL GetCallAbortReqSentFlag(void)
{
   return gCallAbortReqSentFlag;
}

/**************************************************************

	FUNCTION NAME		: SetDropRequestFlag

  	PURPOSE				: Sets the DropRequest Flag

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetDropRequestFlag(pBOOL flag)
{
	mmi_trace(g_sw_CC," Func: %s flag=%d", __FUNCTION__,flag);
	gDropRequestFlag = flag;
}

/**************************************************************

	FUNCTION NAME		: GetDropRequestFlag

  	PURPOSE				: Gets the DropRequest Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetDropRequestFlag(void)
{
	return gDropRequestFlag;
}

/**************************************************************

	FUNCTION NAME		: GetIncomingCallDroppedFlag

	PURPOSE				:

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
pBOOL GetIncomingCallDroppedFlag(void)
{
	return gIncomingCallDroppedFlag;
}

/**************************************************************

	FUNCTION NAME		: SetIncomingCallDroppedFlag

	PURPOSE				:

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
void SetIncomingCallDroppedFlag(pBOOL flag)
{
   /* remote side of MT call drops */
	gIncomingCallDroppedFlag= flag;
}

/**************************************************************

	FUNCTION NAME		: SetMissedCallFlag

  	PURPOSE				: Sets the MissedCall Flag

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetMissedCallFlag(pBOOL flag)
{
	gMissedCallFlag = flag;
}

void SetRejectedCallFlag(pBOOL flag)
{
	gRejectedCallFlag = flag;
}

#if 0
void SetOtherRejectedFlag(pBOOL flag)
{
  gOtherRejectedFlag = flag;
}
#endif /* Guoj delete. It's not used now at 2009-5-26 */
/**************************************************************

	FUNCTION NAME		: GetMissedCallFlag

  	PURPOSE				: Gets the MissedCall Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetMissedCallFlag(void)
{
	return gMissedCallFlag;
}

pBOOL GetRejectedCallFlag(void)
{
	return gRejectedCallFlag;
}

#if 0
pBOOL GetOtherRejectedFlag(void)
{
   return gOtherRejectedFlag;
}
#endif /* Guoj delete. It's not used now at 2009-5-26 */

/**************************************************************

	FUNCTION NAME		: SetCallWaitFlag

  	PURPOSE				: Sets Call Wait Flag dpeneding on flag

	INPUT PARAMETERS	: flag

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetCallWaitFlag(pBOOL flag)
{
	gCallWaitFlag=flag;
}

/**************************************************************

	FUNCTION NAME		: GetCallWaitFlag

  	PURPOSE				: Returns Call Wait Flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: flag

 

**************************************************************/
pBOOL GetCallWaitFlag(void)
{
	return gCallWaitFlag;
}

/**************************************************************

	FUNCTION NAME		: SetNoTwiceRingFlag

	PURPOSE				:

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
void SetNoTwiceRingFlag(pBOOL flag)
{
	gNoTwiceRingFlag = flag;
}

/**************************************************************

	FUNCTION NAME		: GetNoTwiceRingFlag

	PURPOSE				:

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
pBOOL GetNoTwiceRingFlag(void)
{
	return gNoTwiceRingFlag;
}

/**************************************************************

	FUNCTION NAME		: SetWait2RingCallFlag

  	PURPOSE				: Sets Wait To Ring Flag dpeneding on flag

	INPUT PARAMETERS	: flag

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetWait2RingCallFlag(pBOOL flag)
{
	gWait2RingCallFlag=flag;
}

/**************************************************************

	FUNCTION NAME		: GetWait2RingCallFlag

  	PURPOSE				: Gets Wait To Ring Flag

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: flag

 

**************************************************************/
pBOOL GetWait2RingCallFlag(void)
{
	return gWait2RingCallFlag;
}

/**************************************************************

	FUNCTION NAME		: SetCallEndedBeforeConnFlag

  	PURPOSE				:

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetCallEndedBeforeConnFlag(pBOOL flag)
{
	gCallEndedBeforeConnFlag = flag;
}

/**************************************************************

	FUNCTION NAME		: GetCallEndedBeforeConnFlag

  	PURPOSE				:

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetCallEndedBeforeConnFlag(void)
{
	return gCallEndedBeforeConnFlag;
}

/**************************************************************

	FUNCTION NAME		: SetErrorToneFlag

  	PURPOSE				:

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetErrorToneFlag(pBOOL flag)
{
	gErrorToneFlag = flag;
}

/**************************************************************

	FUNCTION NAME		: GetErrorToneFlag

  	PURPOSE				: Gets the ErrorToneFlag Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetErrorToneFlag(void)
{
	return gErrorToneFlag;
}

/**************************************************************

	FUNCTION NAME		: SetShowAOC

  	PURPOSE				:

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetShowAOC(pBOOL flag)
{
	gShowAOC = flag;
}

/**************************************************************

	FUNCTION NAME		: GetShowAOC

  	PURPOSE				: Gets the ActiveToHold Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetShowAOC(void)
{
	return gShowAOC;
}

/**************************************************************

	FUNCTION NAME		: SetDTMFKeyUpFlag

  	PURPOSE				: Set DTMF Key Up Flag

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetDTMFKeyUpFlag(pBOOL flag)
{
	gDTMFKeyUpFlag = flag;
}

/**************************************************************

	FUNCTION NAME		: GetDTMFKeyUpFlag

	PURPOSE				:

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
pBOOL GetDTMFKeyUpFlag(void)
{
	return gDTMFKeyUpFlag;
}

/**************************************************************

	FUNCTION NAME		: SetCMScrnFlag

  	PURPOSE				:

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetCMScrnFlag(pBOOL flag)
{
	gCMScrnFlag = flag;
}

/**************************************************************

	FUNCTION NAME		: GetCMScrnFlag

  	PURPOSE				: Gets the CMScrnFlag Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetCMScrnFlag(void)
{
	return gCMScrnFlag;
}

/**************************************************************

	FUNCTION NAME		: UpdateActiveCallScrnIfPresent

  	PURPOSE				: Redraws Active Call Screen if Present

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void UpdateActiveCallScrnIfPresent(void)
{
	if(GetExitScrnID() == SCR_CM_ACTIVECALLSCREEN)
	{
		MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_INFO_UpdateActiveCallScrnIfPresent));
		EntryScr1002ActiveCall();
	}
	else if ((GetCurrentState() == CM_ACTIVE_STATE || GetCurrentState() == CM_HOLD_STATE)
				&& (GetSyncCallFromATFlag() == TRUE))
	{
		SetSyncCallFromATFlag(FALSE);
		GoBackToHistory(SCR_CM_ACTIVECALLSCREEN);
	}
}

/**************************************************************

	FUNCTION NAME		: EmergencyCallPresent

  	PURPOSE				: Check if emergency call is Present

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE/FALSE

	RETURNS				: nil

 

**************************************************************/
pBOOL EmergencyCallPresent(void)
{
   U16 count;

	for (count=0;count<MAX_CALLS;count++)
	{
		if (CheckValidEmergencyNo1((PS8)cm_p->state_info.AllCalls[count].number))
		{
			return TRUE;
		}
	}
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: SetWapCallPresent

	PURPOSE				: Sets wap call present flag

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
void SetWapCallPresent(pBOOL flag)
{
	gWapCallPresent = flag;
}

/**************************************************************

	FUNCTION NAME		: GetWapCallPresent

	PURPOSE				: Returns wap call present flag

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
pBOOL GetWapCallPresent(void)
{
	return gWapCallPresent;
}

/**************************************************************

	FUNCTION NAME		: SetPreviousCallWAP

	PURPOSE				: Sets previous call is wap flag

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
void SetPreviousCallWAP(pBOOL flag)
{
   gPreviousWapCall = flag;
   return;
}

/**************************************************************

	FUNCTION NAME		: GetPreviousCallWAP

	PURPOSE				: Returns previous call is wap flag

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
pBOOL GetPreviousCallWAP (void)
{
   return gPreviousWapCall;
}

/**************************************************************

	FUNCTION NAME		: IsWapCallPresent

	PURPOSE				: check if wap call present

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
pBOOL IsWapCallPresent (void)
{
   U8 count;

	for (count=0;count<MAX_CALLS;count++)
	{
		if (cm_p->state_info.AllCalls[count].call_type == CSMCC_CSD_CALL)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: SetCallEndCause

	PURPOSE				: Keep call end cause for error display after get time

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
void SetCallEndCause (U16 cause)
{
   gCallEndCause = cause;
}

/**************************************************************

	FUNCTION NAME		: GetCallEndCause

	PURPOSE				: Gets previously saved call end cause for display error

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
U16 GetCallEndCause(void)
{
   return gCallEndCause;
}

#ifdef __IP_NUMBER__
/**************************************************************

	FUNCTION NAME		: SetTempUseIPNumber

	PURPOSE				: Enable/disable temporary IP dialing

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
void SetTempUseIPNumber(pBOOL flag)
{
   gTempUseIPNum = flag;
}

/**************************************************************

	FUNCTION NAME		: GetTempUseIPNumber

	PURPOSE				: Check if temporary IP dialing is enabled

	INPUT PARAMETERS	:

	OUTPUT PARAMETERS	:

	RETURNS				:

 

**************************************************************/
pBOOL GetTempUseIPNumber (void)
{
   return gTempUseIPNum;
}
#endif

/**************************************************************

	FUNCTION NAME		: SetCMPhoneBookStruct

  	PURPOSE				:

	INPUT PARAMETERS	: PHB_CM_INTERFACE

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetCMPhoneBookStruct(PHB_CM_INTERFACE *pNumberStruct)
{
	S8 temp1[50];
	S8 temp2[50];

	UnicodeToAnsii((PS8)temp1, (PS8)pNumberStruct->name);
	UnicodeToAnsii((PS8)temp2, (PS8)pNumberStruct->number);
//	mmi_trace(g_sw_CC,"*** SetCMPhoneBookStruct name=%s, number=%s\n", temp1, temp2);
	mmi_trace(g_sw_CC,"*** SetCMPhoneBookStruct number=%s\n", temp2);

	memcpy (&gPhoneNumberStruct, pNumberStruct, sizeof (PHB_CM_INTERFACE));
}

/**************************************************************

	FUNCTION NAME		: GetCMPhoneBookStruct

  	PURPOSE				: Gets the CMScrnFlag Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: PHB_CM_INTERFACE

	RETURNS				: nil

 

**************************************************************/
PHB_CM_INTERFACE *GetCMPhoneBookStruct(void)
{
	return &gPhoneNumberStruct;
}

/**************************************************************

	FUNCTION NAME		: SetSyncCallListFromATFlag

  	PURPOSE				:

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetSyncCallFromATFlag(pBOOL flag)
{
	gSyncFromAT = flag;
}

/**************************************************************

	FUNCTION NAME		: GetSyncCallListFromATFlag

  	PURPOSE				: Gets the CallDeflectFlag Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetSyncCallFromATFlag(void)
{
	return gSyncFromAT;
}

/**************************************************************

	FUNCTION NAME		: SetCallDeflectFlag

  	PURPOSE				:

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetCallDeflectFlag(pBOOL flag)
{
	gCallDeflectFlag = flag;
}

/**************************************************************

	FUNCTION NAME		: GetCallDeflectFlag

  	PURPOSE				: Gets the CallDeflectFlag Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetCallDeflectFlag(void)
{
	return gCallDeflectFlag;
}

/**************************************************************

	FUNCTION NAME		: SetActivatedCcbsFlag

  	PURPOSE				: Sets the CCBS Activated Flag

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetActivatedCcbsFlag(pBOOL ccbsFlag)
{
   /* activate ccbs */
	gCcbsFlagActivated = ccbsFlag;
}

/**************************************************************

	FUNCTION NAME		: GetActivatedCcbsFlag

  	PURPOSE				: Gets the CCBS activated Flag

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetActivatedCcbsFlag(void)
{
	return gCcbsFlagActivated;
}

/**************************************************************

	FUNCTION NAME		: SetCCBSFlag

  	PURPOSE				: Sets the CCBS Flag

	INPUT PARAMETERS	: TRUE for set, FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetCCBSFlag(pBOOL ccbsFlag)
{
   /* CCBS invoke came */
	gCcbsFlag = ccbsFlag;
}

/**************************************************************

	FUNCTION NAME		: GetCCBSFlag

  	PURPOSE				: Gets the CCBS Flag Status

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
pBOOL GetCCBSFlag(void)
{
	return gCcbsFlag;
}

/**************************************************************

	FUNCTION NAME		: CCBSUpdateCallInfo

  	PURPOSE				: Updates CCBS Call Info

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void CCBSUpdateCallInfo(void)
{
	CM_CALL_HANDLE tmp;
	S16 index = 0;

	tmp = GetIncomingCallHandle();
	index = GetMMIStructIndexof(tmp);

	cm_p->state_info.AllCalls[index].orgination_flag = CM_CALL_MO;
	SetCurrentState(CM_OUTGOING_STATE);
	SetCallState(index,CM_OUTGOING_STATE,FALSE);
}

/**************************************************************

	FUNCTION NAME		: CMAllowInterrupt

  	PURPOSE				: Check if call management is in interruptable
  							  state for WAP to display screen

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
U16 CMAllowInterrupt(void)
{
   U8 count =(U8) GetTotalCallCount();

	/* idle screen not in history means it's in idle screen */
	if ((isIdleScreenReached == TRUE) && (IsScreenPresent(IDLE_SCREEN_ID) == FALSE))
	{
	   /* idle screen allows WAP to show screen */
		return TRUE;
	}
	else if (count == 1 && GetWapCallPresent())
	{
	   /* in WAP call allows WAP to show screen */
      return TRUE;
	}
   else
      return FALSE;
}

/**************************************************************

	FUNCTION NAME		: LogCallInfoForCallHistory

  	PURPOSE				: Keep call info for call log before get time

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void LogCallInfoForCallHistory(CM_CALL_HANDLE handle)
{
	S16 index;

	index = GetMMIStructIndexof(handle);
	mmi_trace(g_sw_CC,"*** LogCallInfoForCallHistory call handle=%d, index=%d\n", handle, index);

	memcpy(&cm_p->state_info.CallStructureForCallLog,
	         &cm_p->state_info.AllCalls[index],sizeof(CALL_INFO));
}

/**************************************************************

	FUNCTION NAME		: UpdateCallStartTimeAndLogCall(handle)

  	PURPOSE				: Helper function to get the call handle of the
						  recent incoming/outgoing or active call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void UpdateCallStartTimeAndLogCall(CM_CALL_HANDLE handle,MYTIME *t)
{
	S16 index;
	index=GetMMIStructIndexof(handle);

   /* start call time reminder once call is connected */
	gCtrTime = GetCtrTime();  // zrx del 20060612

	memcpy(&cm_p->state_info.AllCalls[index].start_time,t,sizeof(MYTIME));
	LogCallWithStartTime(&cm_p->state_info.AllCalls[index]);
}

/**************************************************************

	FUNCTION NAME		: UpdateCallStartTimeforWapCall

 

	INPUT PARAMETERS	: MYTIME *t

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void UpdateCallStartTimeforWapCall(MYTIME *t)
{
	if(GetCallHandleForStartTimeUpdate())
	{
		UpdateCallStartTimeAndLogCall(GetCallHandleForStartTimeUpdate(),t);
		SetCallHandleForStartTimeUpdate(0);
	}
}

/**************************************************************

	FUNCTION NAME		: SetCallHandleForStartTimeUpdate(handle)

  	PURPOSE				: Saves the Call Handler for which start time is
							to be updated

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetCallHandleForStartTimeUpdate(CM_CALL_HANDLE handle)
{
	gCallHandleForStartTimeUpdate=handle;
}

/**************************************************************

	FUNCTION NAME		: GetCallHandleForStartTimeUpdate(handle)

  	PURPOSE				: Returns the Call Handler for which start time is
							to be updated

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
CM_CALL_HANDLE GetCallHandleForStartTimeUpdate(void)
{
	return gCallHandleForStartTimeUpdate;
}

/**************************************************************

	FUNCTION NAME		: LogCall

  	PURPOSE				: Logs Call Duration for Current Call

	INPUT PARAMETERS	: CM_CALL_HANDLE of call

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void ReqUpdateCallLogDuration(CALL_INFO *dailCall, U8 type);
void LogCall(void)
{
   // drop call refine
   // abort MO call state will be disconnecting, but also need to be logged
	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if ((cm_p->state_info.CallStructureForCallLog.curr_state==CM_OUTGOING_STATE)||
	   (cm_p->state_info.CallStructureForCallLog.curr_state==CM_INCOMING_STATE) ||
	   ((cm_p->state_info.CallStructureForCallLog.curr_state==CM_DISCONNECTING_STATE) &&
	   (cm_p->state_info.CallStructureForCallLog.prev_state==CM_OUTGOING_STATE)))
	{
		memcpy(&cm_p->state_info.CallStructureForCallLog.num,
		         &cm_p->state_info.CallStructureForCallLog.number,
      		   sizeof(cm_p->state_info.CallStructureForCallLog.num));
		LogCallWithStartTime(&cm_p->state_info.CallStructureForCallLog);
	}
	if( CompareTime(cm_p->state_info.CallStructureForCallLog.end_time, cm_p->state_info.CallStructureForCallLog.start_time, NULL) != TIME_EQUAL )
{
	if(CM_CALL_MO==cm_p->state_info.CallStructureForCallLog.orgination_flag ||
		CM_CALL_MO_STK==cm_p->state_info.CallStructureForCallLog.orgination_flag)
	{
		CHISTLogDialedCallDuration(&cm_p->state_info.CallStructureForCallLog);
		ReqUpdateCallLogDuration(&cm_p->state_info.CallStructureForCallLog, PHB_LND);
	}
	else if(CM_CALL_MT==cm_p->state_info.CallStructureForCallLog.orgination_flag)
	{
		CHISTLogRecvdCallDuration(&cm_p->state_info.CallStructureForCallLog);
		ReqUpdateCallLogDuration(&cm_p->state_info.CallStructureForCallLog, PHB_LNR);
	}
}
}

/**************************************************************

	FUNCTION NAME		: LogCallWithStartTime

  	PURPOSE				: Logs Call with Start Time

	INPUT PARAMETERS	: CALL_INFO

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void LogCallWithStartTime(CALL_INFO *log)
{
	if(CM_CALL_MO==log->orgination_flag)
	{
		CHISTLogDialedCall(log);
	}else if(CM_CALL_MT==log->orgination_flag)
	{
		CHISTLogRecvdCall(log);
	}
	else
		PRINT_INFORMATION ((" XXX In LogCallWithStartTime, no need to log\n"));
}

/**************************************************************

	FUNCTION NAME		: GetEndTimeAndNotifyCallAborted

  	PURPOSE				: Sets Call Back for Call Aborted Screen

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void GetEndTimeAndNotifyCallAborted(void)
{
	CMGetExactTime(GetDateTimeAndNotifyCallAbortedCBack);
}

/**************************************************************

	FUNCTION NAME		: GetDateTimeAndNotifyCallAbortedCBack

  	PURPOSE				: Call Back Called after End Time For Call Aborted Retrieved

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void GetDateTimeAndNotifyCallAbortedCBack(MYTIME *t)
{
	memcpy(&cm_p->state_info.CallStructureForCallLog.start_time,t,sizeof(MYTIME));
	memcpy(&cm_p->state_info.CallStructureForCallLog.end_time,t,sizeof(MYTIME));

	LogCall();
	if(GetActivatedCcbsFlag()!=TRUE)
	{
		EntryScrNotifyCallAborted();
	}
}

/**************************************************************

	FUNCTION NAME		: GetEndTimeAndNotifyCallAborted

  	PURPOSE				: Sets Call Back for Call Missed Screen

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void GetEndTimeAndNotifyCallMissed(void)
{
	CMGetExactTime(GetDateTimeAndNotifyCallMissedCBack);
}

void GetEndTimeAndNotifyCallRejected(void)
{
#if 0
  SetOtherRejectedFlag(TRUE);
#endif /* Guoj delete. It's not used now at 2009-5-26 */
	CMGetExactTime(GetDateTimeAndNotifyCallRejectedCBack);
}

/**************************************************************

	FUNCTION NAME		: GetDateTimeAndNotifyCallMissedCBack

  	PURPOSE				: Call Back Called after End Time For Call Missed Retrieved

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void GetDateTimeAndNotifyCallMissedCBack(MYTIME *t)
{
	memcpy(&cm_p->state_info.CallStructureForCallLog.start_time,t,sizeof(MYTIME));
	memcpy(&cm_p->state_info.CallStructureForCallLog.end_time,t,sizeof(MYTIME));
	SetMissedCallFlag(TRUE);
  SetSoundWarnningTimer();
	if (GetActivatedCcbsFlag()!=TRUE)
	{
		EntryScr1004NotifyEndCallDuration();
	}
	CHISTLogMissedCall(&cm_p->state_info.CallStructureForCallLog);
}

void GetDateTimeAndNotifyCallRejectedCBack(MYTIME *t)
{
	memcpy(&cm_p->state_info.CallStructureForCallLog.start_time,t,sizeof(MYTIME));
	memcpy(&cm_p->state_info.CallStructureForCallLog.end_time,t,sizeof(MYTIME));
	SetRejectedCallFlag(TRUE);
	CHISTLogRejectedCall(&cm_p->state_info.CallStructureForCallLog);
}

/**************************************************************

	FUNCTION NAME		: GetEndTimeAndLogUnconnectedMOCall

  	PURPOSE				: Sets Call Back for MO Unconnected Call Screen

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void GetEndTimeAndLogUnconnectedMOCall(void)
{
	CMGetExactTime(GetDateTimeAndLogUnconnectedMOCallCBack);
}

/**************************************************************

	FUNCTION NAME		: GetDateTimeAndLogUnconnectedMOCallCBack

  	PURPOSE				: Call Back Called after End Time For Unconnected MO Call Retrieved

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void GetDateTimeAndLogUnconnectedMOCallCBack(MYTIME *t)
{
	memcpy(&cm_p->state_info.CallStructureForCallLog.start_time,t,sizeof(MYTIME));
	memcpy(&cm_p->state_info.CallStructureForCallLog.end_time,t,sizeof(MYTIME));
	LogCall();
}

/**************************************************************

	FUNCTION NAME		: GetEndTimeAndNotifyEndCallDuration

  	PURPOSE				: Sets Call Back for End Time Notify Screen

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
void GetEndTimeAndNotifyEndCallDuration(CM_CALL_HANDLE handle)
{
        {
            MYTIME curtime;
            GetDateTime(&curtime);
            DummyScr1002ActiveCall(&curtime);
        }

	memcpy(&gTimeStructForEndedCallStartTime,
	       &(cm_p->state_info.AllCalls[GetMMIStructIndexof(handle)].start_time),sizeof(MYTIME));
	CMGetExactTime(GetDateTimeAndNotifyEndCallDurationCBack);
}

/**************************************************************

	FUNCTION NAME		: GetDateTimeAndNotifyEndCallDurationCBack

  	PURPOSE				: Call Back Called after End Time For Call Retrieved

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/

void GetDateTimeAndNotifyEndCallDurationCBack(MYTIME *t)
{
	if(NULL==GetTimeDifference(t,&gTimeStructForEndedCallStartTime,&gTimeStructForCallDuration))
	{
		mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
		mmi_trace(g_sw_CC,"\nCall Handle: %d",cm_p->state_info.CallStructureForCallLog.call_handle);
		mmi_trace(g_sw_CC,"\nCall Current State: %d",cm_p->state_info.CallStructureForCallLog.curr_state);
		mmi_trace(g_sw_CC,"\nCall Previous State: %d",cm_p->state_info.CallStructureForCallLog.prev_state);
		mmi_trace(g_sw_CC,"\nCall Origin: %d",cm_p->state_info.CallStructureForCallLog.orgination_flag);
		mmi_trace(g_sw_CC,"\nStart Time : Hour:%d ,Min:%d ,Sec:%d ",gTimeStructForEndedCallStartTime.nHour,gTimeStructForEndedCallStartTime.nMin,gTimeStructForEndedCallStartTime.nSec);
		mmi_trace(g_sw_CC,"\nEnd Time : Hour:%d ,Min:%d ,Sec:%d ",t->nHour,t->nMin,t->nSec);
	}

	memcpy(&cm_p->state_info.CallStructureForCallLog.start_time,
			&gTimeStructForEndedCallStartTime,sizeof(MYTIME));
	memcpy(&cm_p->state_info.CallStructureForCallLog.end_time,t,sizeof(MYTIME));
	LogCall();

	EntryScr1004NotifyEndCallDuration();
	memset (&cm_p->state_info.CallStructureForCallLog, 0, sizeof (CALL_INFO));
}

/**************************************************************

	FUNCTION NAME		: CheckCtrExpiry(handle)

	PURPOSE				: Checks if call-time reminder tone is
						  to be played

	INPUT PARAMETERS	: Time structure t

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void CheckCtrExpiry (void)
{

   MYTIME curr_time, *start_time, duration;
   	U32 duration1, duration2;
   U8 status;

   StartTimer(CM_CTR_TIMER, 1000, CheckCtrExpiry);

   DTGetRTCTime(&curr_time);
   start_time = GetStartCallTime();
   if (start_time == NULL)
   	return;
   GetTimeDifference(&curr_time, start_time, &duration);

   duration1 = cm_p->alert_info.LastCTRTime.nHour * 60 * 60 + cm_p->alert_info.LastCTRTime.nMin * 60 + cm_p->alert_info.LastCTRTime.nSec;
   duration2 = duration.nHour * 60 * 60 + duration.nMin * 60 + duration.nSec;

#ifdef __MMI_CH_QUICK_END__
	if (cm_p->alert_info.QuickEndTime > 0 &&
		cm_p->alert_info.QuickEndTime != 0xffff &&
		cm_p->alert_info.QuickEndTime <= duration2)
	{
		StopTimer(CM_CTR_TIMER);
		if (!(GetTotalCallCount()==1 &&
			  (GetCurrentState()==CM_INCOMING_STATE ||
			  GetCurrentState() == CM_OUTGOING_STATE)))
		KbCBackEndAllCallsExceptIncoming();
		return;
	}
#endif

	status = GetCtrStatus();
   if ((status != CTR_OFF) && (duration1 <= duration2))
   {
   	if(status == CTR_PERIODIC)
   {
         if((cm_p->alert_info.LastCTRTime.nSec < gCtrTime) &&
	   		((gCtrTime <= duration.nSec) || (cm_p->alert_info.LastCTRTime.nSec > duration.nSec)))
	   {
		   playRequestedTone(CALL_REMINDER_TONE);
	   }
   }
   else
   {
      	if (duration1 < gCtrTime && gCtrTime <= duration2)
	   {
	   		if (cm_p->alert_info.QuickEndTime == 0)
	   	StopTimer(CM_CTR_TIMER);
         playRequestedTone(CALL_REMINDER_TONE);
      }
   }
}
   memcpy (&cm_p->alert_info.LastCTRTime, &duration, sizeof (MYTIME));

}

/**************************************************************

	FUNCTION NAME		: InitializeTimeStructure

  	PURPOSE				: Function gets the initialize the time structure before displaying the call

	INPUT PARAMETERS	: MYTIME *time time strucutre to initialize

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void InitializeTimeStructure (TIMEANDINDEX *time,MYTIME *t)
{
	MYTIME tmpTime;
	TIMEANDINDEX tmpTimeBuf;
	TIMEANDINDEX tmpArr[MAX_CALLS];
	U8 validTimes=0;
	U8 count;
	U8 j;

	memset (tmpArr,0, (sizeof (TIMEANDINDEX) * MAX_CALLS));
	memcpy(&tmpTime,t,sizeof(tmpTime));

	for (count=0; count != MAX_CALLS; count++)
	{
		 
		// first get diff of all times
		if (cm_p->state_info.AllCalls[count].call_handle != -1 &&
		    (cm_p->state_info.AllCalls[count].curr_state == CM_ACTIVE_STATE ||
		     cm_p->state_info.AllCalls[count].curr_state == CM_HOLD_STATE ||
		     cm_p->state_info.AllCalls[count].curr_state == CM_DISCONNECTING_STATE))
		 
		{
			if (GetTimeDifference(&tmpTime, &cm_p->state_info.AllCalls[count].start_time,&(tmpArr[validTimes].time)) != NULL)
			{
				tmpArr[validTimes].index=count;
				validTimes++;
			}
		}
	}

	for (count=0;count<validTimes-1;count++)
	{
		for (j=0;j<validTimes-1-count;j++)
		{
			if (FindMaxTime (&tmpArr[j].time, &tmpArr[j+1].time))
			{
				memcpy (&tmpTimeBuf,&tmpArr[j],sizeof(tmpTimeBuf));
				memcpy (&tmpArr[j],&tmpArr[j+1],sizeof(tmpTimeBuf));
				memcpy (&tmpArr[j+1],&tmpTimeBuf,sizeof (tmpTimeBuf));
			}
		}
	}
	memcpy (time,&tmpArr[validTimes - 1], sizeof (tmpArr[validTimes - 1]));
	cm_p->state_info.OldestCallIndex = time->index;

	if ((GetCtrStatus() != CTR_OFF && cm_p->alert_info.IsCTRStart == FALSE)
#ifdef __MMI_CH_QUICK_END__
		|| cm_p->alert_info.QuickEndTime > 0
#endif
		)
	{
         // start call time reminder timer
         cm_p->alert_info.IsCTRStart = TRUE;
         if (IsMyTimerExist(CM_CTR_TIMER) == MMI_FALSE)
         StartTimer(CM_CTR_TIMER, 1000, CheckCtrExpiry);
	}
}

MYTIME * GetStartCallTime(void)
{
   if (cm_p->state_info.AllCalls[cm_p->state_info.OldestCallIndex].call_handle == -1)
   {
      TIMEANDINDEX timeStruct;
      MYTIME t;

      DTGetRTCTime(&t);
      InitializeTimeStructure(&timeStruct, &t);
      if (GetTotalCallCount() == 0)
         return NULL;
   }
	return &(cm_p->state_info.AllCalls[cm_p->state_info.OldestCallIndex].start_time);
}

/**************************************************************

	FUNCTION NAME		: SetPriorityCall

  	PURPOSE				: Sets the Priority Call Option

	INPUT PARAMETERS	: True if the call is a priority call

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
void SetPriorityCall(pBOOL priority)
{
	gPriority = priority;
}

/**************************************************************

	FUNCTION NAME		: GetPriorityCall

  	PURPOSE				: Gets the current priority status

	INPUT PARAMETERS	: TRUE for set FALSE for not set

	OUTPUT PARAMETERS	: none

	RETURNS				: nil

 

**************************************************************/
pBOOL GetPriorityCall(void)
{
	return gPriority;
}

/**************************************************************

	FUNCTION NAME		: SetSATScrnSavedFlag

  	PURPOSE				: Sets SAT Screen Saved Flag

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: TRUE for set, FALSE for not set

	RETURNS				: nil

 

**************************************************************/
void SetSATScrnSavedFlag(pBOOL flag)
{
	gSATScrnSavedFlag = flag;
}

/**************************************************************

	FUNCTION NAME		: GetSATScrnSavedFlag

  	PURPOSE				: Returns SAT Screen Saved Flag

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
pBOOL GetSATScrnSavedFlag(void)
{
	return gSATScrnSavedFlag;
}
void UpdateScr1002ActiveCall(void)
{
     if((isInCall())&&(GetExitScrnID() == SCR_CM_ACTIVECALLSCREEN))
        {
            MYTIME curtime;
            GetDateTime(&curtime);
            DummyScr1002ActiveCall(&curtime);
    	  CMGetExactTime(DummyScr1002ActiveCall);
        }   
}

