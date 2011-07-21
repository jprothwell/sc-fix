/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    adp_CC_Events.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*      transform message   .                                  */
/*     @vivian  2006-07-13.                                                       */
/******************************************************************************/

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"


 
#include "wgui_categories_inputs.h"
#include "imeres.h"
 
#include "globalconstants.h"
#include "taskinit.h"
#include "mmitaskprot.h"
#include "thrdosinterface.h"
#include "globalstruct.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "asynceventsgprot.h"
#include "alarmframeworkprot.h"    
#include "callmanagementgprot.h"
#include "callmanagementstruct.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "service.h"
#include "satgprots.h"
#include "cphsprotocolhandlers.h"
#include "engineermodeprot.h"
#include "factorymodeprot.h"
#include "gpioinc.h"
#include "phonebookgprot.h"
#include "callhistorymain.h"
#include "funandgamesprots.h"
#include "messagesresourcedata.h"
#include "organizergprot.h"
#include "alarmgprot.h"
#include "scheduledpoweronoffgprot.h"
#include "unicodedcl.h"
#include "unicodedef.h"
#include "organizergprot.h"
#include "idleappprot.h"
#include "conversions.h"
#include "nvramprot.h"
 
#include <csw.h>
#include "mmi.h"
#include "uim.h"
#include "adp_events.h"
#include "adp_eventsredef.h"
#include "cfw.h"   
#include "phonebooktypes.h"  

 
#ifdef __MMI_THEMES_V2_SUPPORT__
#include "thememanager.h"
#endif

#include "messagesexdcl.h"

#ifdef __MMI_USB_SUPPORT__
#include "usbdevicegprot.h"
   #ifdef MMI_ON_HARDWARE_P
   extern kal_bool INT_USBBoot(void);
   #endif
#endif

#ifdef __MMI_IRDA_SUPPORT__
#include "irdammigprots.h"
#endif


#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
#endif

#ifdef __MMI_FILE_MANAGER__
#include "filemanagergprot.h"
#include "msdc_def.h"
#endif

#ifdef __MMI_WEBCAM__
#include "mdi_datatype.h"
#include "mdi_webcam.h"
#endif /* __MMI_WEBCAM__ */

#include "soundeffect.h"

#include "shortcutsprots.h"
#include "sublcdhistorygprot.h"
#include "todolistdef.h"

#include "wpssprotos.h"
#include "calleridgprot.h"

#if defined(__MMI_STOPWATCH__)
#include "stopwatchgprot.h"
#endif

#if defined(__MMI_EMAIL__)		
#include "emailappgprot.h"
#endif // def __MMI_EMAIL__	
#include "osthreadinterfacegprot.h"

#include "eventsgprot.h"
#include "allappgprot.h"
#include "wrappergprot.h"
#include "queuegprot.h"

#include "keybrd.h"
#include "nvramexdcl.h"
#include "simdetectiongprot.h"
#include "wgui_status_icons.h"
#include "poweronchargerprot.h"
#include "settinggprots.h"
#include "simdetectiongexdcl.h"
#include "phonesetupgprots.h" /* phnsetreadnvramcalibrationdata */
#include "callstructuremanagementprot.h"

#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
#endif /*__MMI_SMART_MESSAGE_MO__ && !__MMI_MESSAGES_EMS__*/

 
#ifdef __MMI_POC__
#include "pocgprot.h"
#endif

#undef __NEWSIMULATOR

#ifdef MMI_ON_HARDWARE_P
#include "lcd_sw_inc.h"
#include "sublcd.h"
#else
#include "lcd_if.h"
#endif
#include "composeringtoneprots.h"

#ifdef __MMI_IMPS__
#include "mmi_imps_gprot.h"
#endif

//dyj add start 20060616
#include "device.h"
#include "stack_msgs.h"
#include <cswtype.h>
#include "mmi_msg_struct.h"
#include "cmerrortypes.h"
#include "satstruct.h"
#include "mmi_trace.h"
#include "unit_test.h"

#ifdef __ADP_MULTI_SIM__
ADP_CC_CONTEXT_T g_CCContext[4] =
{
	{ADP_SIM_1, },
	{ADP_SIM_2, },
	{ADP_SIM_3, },
	{ADP_SIM_4, },
};

ADP_CC_CONTEXT_T *g_pCCContext = &g_CCContext[0];

#define ADP_CC_MSG_SRC_MOD  adp_SimId2IlmMod(g_pCCContext->sim_id)
#else
#define ADP_CC_MSG_SRC_MOD MOD_L4C
#endif

#define  CC_TEST_SWITCH		1//switch of test
#define ACTIVE_DIAL				1	//initiate a new call when have active call
#define ACTIVE_EMERGENCY		2	//initiate emergency call when have call
static UINT8	g_AdpCcState = 0;	//store call state
static mmi_ss_parsing_string_req_struct g_MmiParseStrReq;//store new call request message

static void Adp_ActiveDialStateReleaseProcess(COS_EVENT *pCosEvent);
extern BOOL SS_Adaption_ParseStringREQ(mmi_ss_parsing_string_req_struct *pReq);//zhanggh
//static void Test_PrintReturnEventText(msg_type_t  EventId);
static void Adp_ActiveDialStateMultiRspProcess(COS_EVENT* pCosEvent);
static void Adp_SendCCParsingStringResult(csmss_string_op_enum OpCode);
static UINT32  Adp_GetDialCallInfoAndDialCall(mmi_ss_parsing_string_req_struct *pMmiParseStrReq, UINT8 *pCallIndex);

#define __MMI_CONTROL_SWAP_CALL_RESPONSE__  //wangzl: 090616 add for 11770
#ifdef __MMI_CONTROL_SWAP_CALL_RESPONSE__
U8 nWaitSwapCallResponse= 0;
#endif
//#define __MMI_SPEEDUP_RELEASE_CALL__
#ifdef __MMI_SPEEDUP_RELEASE_CALL__
U8 nDummySendReleaseCallMsg = 0;
#endif
/**************************************************************

	FUNCTION NAME		: Adp_GetCallInfo 

  	PURPOSE				: get call information
	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	:

	RETURNS				: TRUE: have call information
						  	  FALSE: no call information
	REMARKS			: 

**************************************************************/
BOOL Adp_GetCallInfo(CFW_CC_CURRENT_CALL_INFO *pCallInfo, UINT8  CallStatus)
{
	CFW_CC_CURRENT_CALL_INFO 	CallInfo[MAX_CALLS];
	UINT8	Count = 0;
	BOOL	RtnValue = FALSE;   
	UINT8	i;

	ASSERT(pCallInfo != NULL);
	memset(CallInfo, 0, MAX_CALLS * sizeof(CFW_CC_CURRENT_CALL_INFO));
	
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	RtnValue = UT_CFW_CcGetCurrentCall(CallInfo, &Count, g_pCCContext->sim_id);
#else
	RtnValue = CFW_CcGetCurrentCall(CallInfo, &Count, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
	RtnValue = CFW_CcGetCurrentCall(CallInfo, &Count);
#endif /* __ADP_MULTI_SIM__ */
	mmi_trace(g_sw_ADP_CC, "MMI_Call:(WARN) Adp_GetCallInfo Count = %d RtnValue=0x%x", Count, RtnValue);
	if (ERR_SUCCESS ==  RtnValue )
	{
		
		if ((Count > 0) && (Count <= MAX_CALLS))
		{
			for (i = 0; i < Count; i++)
			{
				if (CallStatus == CallInfo[i].status)
				{
					*pCallInfo = CallInfo[i];
					return  TRUE;
					
				}
			}
		}
	}

	return	FALSE;
}

/**************************************************************
	FUNCTION NAME		: Adp_IsDigit 

  	PURPOSE			    : check if char is digit
	INPUT PARAMETERS	: INT8 CharValue

	OUTPUT PARAMETERS:NULL

	RETURNS			    : TRUE: char is digit
						  FALSE: char is not digit
**************************************************************/
BOOL Adp_IsDigit(INT8  CharValue)
{
	if ((CharValue >= '0')&&(CharValue <= '9'))
		return	TRUE;
	else
		return	FALSE;	
}

/*----------------------------------------------------------------------------*/
/*
Name: 	Adp_IsCCString 
Desc: 	Tries to recognize if input string is a valid call number. The
      		following forms are allowed:
      		'12345'       Pure number with/without international '+'
      		'12345D236'   Number with DTMF

      		and hopefully, various conbinations of the above.

Params: Input string
Returns: T/F
Caveats:
*/
/*----------------------------------------------------------------------------*/
INT8 branchbuf[80] ;//man
static BOOL Adp_IsCCString( INT8 *s )
{
	INT8 	len;
	UINT16 	i;
	UINT16 j = 0;

	ASSERT(s != NULL);

	len = strlen(s);
	mmi_trace(1, "MMI_Call: Adp_IsCCString  = %s\n",s);//man

	memset(branchbuf , 0x0,sizeof(branchbuf));
	if (0 == len)
		return	FALSE;
	
	for(i = 0 ; i < len ; i++)
	{
		if(s[i] == 'P'||s[i] == 'p'||s[i] == 'W'||s[i] == 'w')	
		{
			int num_end = i;
			while(i < len)
			{
         		branchbuf[j++] = s[i++];
			}
			s[num_end] ='\0';  //所有的+p +w 的号码，都先拨打p 前面的，再处理p后面的号码/*Guojian Add  For 8870 Ver: Jessamine  on 2008-6-26 16:2 */
			break;
		}
	}
	mmi_trace(1, "MMI_Call: branchbuf = %s\n",branchbuf);//man
	
	len = strlen(s);	/*Guojian Add  For 8870 Ver: Jessamine  on 2008-6-26 16:2 */

	// special case for #+number, it is a number, we just dial
	if ((s[0] == '#') && (s[len-1] != '#'))//zhanggh for SS operation 2005.07.26
		return TRUE;
	
	if ((s[0] == '+') || Adp_IsDigit (s[0]) || ((s[0] == '*') && (s[len-1] != '#')))
	{
		for  (i = 1; i < len - 1; i++)
		{
			if (!(Adp_IsDigit (s[i]) 
				|| s[i] == 'P'	//CHAR_DTMF ||   /* allow DTMF separator */
				|| s[i] == 'p'
				|| s[i] == '*'
				|| s[i] == '#'
				|| s[i] == 'w'
				|| s[i] == 'W'))  //man
				
				return FALSE;
		}

		return TRUE;
	}
	
	return FALSE;
}

/*------------------------------------------------------------------------*/
/*
Name: Adp_IsSSString 
Desc: Returns TRUE if something which looks like it is a valid SS
string has been entered. Note that the real parsing for such strings is
done in the SS entity, and therefore this routine only has a peremptory
look at the start and end of the string.
*/
/*------------------------------------------------------------------------*/
static BOOL Adp_IsSSString( INT8 *s )
{
	
	INT8 len;
 
//vivian add callStarting process begin
	CFW_CC_CURRENT_CALL_INFO	CallInfo;
	BOOL		CallStarting;

	ASSERT(s != NULL);
	
	CallStarting = Adp_GetCallInfo (&CallInfo, CFW_CM_STATUS_ACTIVE);
	
//vivian add callStarting process end

	len = strlen( s );

	if (0 == len)
		return	FALSE;
	
	if ((len > 4) && (s[len - 1] == '#') && (s[len - 2] != '#') && (s[len - 2] != '*'))
		return TRUE;
 

	if ( ( (s[0] == '*') || (s[0] == '#') ) && (s[len - 1] == '#') && (s[len - 2] != '#')  && (s[len - 2] != '*'))
		return TRUE;

	if ((len < 3) && (len > 0))
	{
		/* There is at least one call active */
		if ( !CallStarting && s[0] == '1' )
			return FALSE;
		
		return TRUE;
	}

	return FALSE;
}

/**************************************************************
	FUNCTION NAME		: Adp_CheckCallType
	
	PURPOSE			   : check call type and retrun effective call number
	
	INPUT PARAMETER    : INT8 *s
						  UINT8* pCallType
	OUT	 PARAMETERS	   : pCallType

	RETURN				:UINT8*
**************************************************************/
static UINT8* Adp_CheckCallType(INT8 *s, UINT8* pCallType)
{
	ASSERT(s != NULL);
	if (s[0] == '+')
	{
		*pCallType = CFW_TELNUMBER_TYPE_INTERNATIONAL;
		return &s[1];
	}
	else
	{
		*pCallType = CFW_TELNUMBER_TYPE_UNKNOWN;
		return s;
	}
	
}


// 
// /**************************************************************
// 	FUNCTION NAME		: adp_CC_SendMessageToMMI
// 	PURPOSE			    : Send message about CC to CSD MMI task.
// 	INPUT PARAMETERS	: local_para_struct *msg_ptr
// 						: msg_type msg_id
// 	OUTPUT PARAMETERS   : NULL
// 	RETURN				: NULL
// **************************************************************/
// static void adp_CC_SendMessageToMMI(local_para_struct *msg_ptr, msg_type_t msg_id)
// {
// 	//don't add ASSERT(msg_ptr != NULL) 
// 	
// 	ilm_struct  ilm_ptr;
// 
// #ifdef __ADP_MULTI_SIM__
// 	if (g_pCCContext->sim_id == ADP_SIM_1)
// 	{
// 		ilm_ptr.src_mod_id = MOD_L4C; 
// 	}
// 	else
// 	{
// 		ilm_ptr.src_mod_id = MOD_L4C_2; 
// 	}
// #else
// 	ilm_ptr.src_mod_id = MOD_L4C; 
// #endif
// 
// 	ilm_ptr.dest_mod_id = MOD_MMI;
// 	ilm_ptr.local_para_ptr= (local_para_struct*)msg_ptr;
// 	ilm_ptr.peer_buff_ptr = NULL;
// 	ilm_ptr.msg_id = msg_id;               
// 	OslMsgSendExtQueue(&ilm_ptr);  	
// 
// 	#if CC_TEST_SWITCH
// 	Test_PrintReturnEventText(ilm_ptr.msg_id);
// 	#endif
// }
#ifdef __ADP_MULTI_SIM__
static U8 nDTMFSimId = 0;
#endif /* __ADP_MULTI_SIM__ */
static	void startDtmf()//man call
{
	int i;
	mmi_trace(g_sw_ADP_CC, "MMI_Call: Func: %s branchbuf: %s", __FUNCTION__, branchbuf);
	for(i = 1;i < strlen(branchbuf);i++ )
	{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		UT_CFW_CcPlayTone(branchbuf[i], nDTMFSimId);
#else
		CFW_CcPlayTone(branchbuf[i], nDTMFSimId);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
		CFW_CcPlayTone(branchbuf[i]);
#endif /* __ADP_MULTI_SIM__ */
	}
}
/**************************************************************
	FUNCTION NAME		: adp_CC_proc_INITATESPEECHCALL_rsp
	PURPOSE			    : process EV_CFW_CC_INITIATE_SPEECH_CALL_RSP event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_INITATESPEECHCALL_rsp(COS_EVENT* pCosEvent)
{
	CFW_CC_CURRENT_CALL_INFO	CallInfo;
	UINT8   nType;
	UINT32 nParam1;
	UINT32 nParam2;
	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29c0);
	nType = HIUINT8(pCosEvent->nParam3);
	if ((0 == nType) || (0x01 == nType))
	{	//success
		mmi_cc_call_connect_ind_struct *pMmiInitiateCallRsp;
		
		pMmiInitiateCallRsp = (mmi_cc_call_connect_ind_struct*)OslMalloc(sizeof(mmi_cc_call_connect_ind_struct));

		ASSERT(pMmiInitiateCallRsp != NULL);
		memset(pMmiInitiateCallRsp, 0, sizeof(mmi_cc_call_connect_ind_struct));	
		
		Adp_GetCallInfo (&CallInfo, CFW_CM_STATUS_ACTIVE);
		
		SUL_GsmBcdToAscii(CallInfo.dialNumber.nTelNumber,
							CallInfo.dialNumber.nSize,
							pMmiInitiateCallRsp->num.number);
		
		pMmiInitiateCallRsp->num.type = CallInfo.dialNumber.nType;

		if (branchbuf[0] != 0)
		{
			strcat(pMmiInitiateCallRsp->num.number, branchbuf);
		}
		pMmiInitiateCallRsp->num.length = strlen(pMmiInitiateCallRsp->num.number);
		pMmiInitiateCallRsp->call_id = CallInfo.idx;
		mmi_trace(g_sw_ADP_CC, "MMI_Call: MSG_ID_MMI_CC_CALL_CONNECT_IND num.type=%d, num.length=%d",pMmiInitiateCallRsp->num.type, pMmiInitiateCallRsp->num.length);

		Adp_SendMessageToMMI((local_para_struct *)pMmiInitiateCallRsp, MSG_ID_MMI_CC_CALL_CONNECT_IND, ADP_CC_MSG_SRC_MOD);
		if(branchbuf[0] == 'p' || branchbuf[0] == 'P')
		{
		#ifdef __ADP_MULTI_SIM__
			nDTMFSimId = g_pCCContext->sim_id;
		#endif /* __ADP_MULTI_SIM__ */
		   StartTimer(CM_DIAL_P_TIMER, 3000, startDtmf);
		   mmi_trace(g_sw_ADP_CC, "MMI_Call: start a timer for branch call");
		}
		else if(branchbuf[0] == 'w' || branchbuf[0] == 'W')
		{
			mmi_cc_start_auto_dtmf_ind_struct *pMmiDTMFMsg;
			pMmiDTMFMsg = (mmi_cc_start_auto_dtmf_ind_struct*)OslMalloc(sizeof(mmi_cc_start_auto_dtmf_ind_struct));
			
			ASSERT(pMmiDTMFMsg != NULL);
			memset(pMmiDTMFMsg, 0, sizeof(pMmiDTMFMsg));
			pMmiDTMFMsg->call_id = 0;
			strcpy(pMmiDTMFMsg->digits,&branchbuf[1]);
			Adp_SendMessageToMMI((local_para_struct *)pMmiDTMFMsg, 
								MSG_ID_MMI_CC_START_AUTO_DTMF_IND, ADP_CC_MSG_SRC_MOD);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: MSG_ID_MMI_CC_START_AUTO_DTMF_IND digits = %s",pMmiDTMFMsg->digits);//man
			
		}
	}
	else
	{//error
		mmi_cc_call_release_ind_struct *pMmiReleaseIndRsp;
		nParam1 = pCosEvent->nParam1;
		nParam2 = pCosEvent->nParam2;
		
		pMmiReleaseIndRsp = 
			(mmi_cc_call_release_ind_struct*)OslMalloc(sizeof(mmi_cc_call_release_ind_struct));

		ASSERT(pMmiReleaseIndRsp != NULL);
		memset(pMmiReleaseIndRsp, 0, sizeof(mmi_cc_call_release_ind_struct));

		mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) EV_CFW_CC_INITIATE_SPEECH_CALL_RSP ERROR!!!!!!!!!!");
		//Adp_GetCallInfo (&CallInfo, CFW_CM_STATUS_DIALING);
		if (0x10 == nType)
		{
			pMmiReleaseIndRsp->call_id = nParam1;
			pMmiReleaseIndRsp->cause = CM_MM_CONN_TIMEOUT;
		}
		else if (0xF0 == nType)
		{
			pMmiReleaseIndRsp->call_id = HIUINT16(nParam2);
			if (nParam1 != 0)
			{
				pMmiReleaseIndRsp->cause = nParam1/*ERR_NETWORK_FAILURE*/;
			}
			else //if(LOUINT16(nParam2) != 0)
			{
				pMmiReleaseIndRsp->cause = CM_TEMPORARY_FAILURE;			
			}
		}
		mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) pMmiReleaseIndRsp->call_id=%d, pMmiReleaseIndRsp->cause=%d", pMmiReleaseIndRsp->call_id, pMmiReleaseIndRsp->cause);
		Adp_SendMessageToMMI((local_para_struct *)pMmiReleaseIndRsp, 
											MSG_ID_MMI_CC_CALL_RELEASE_IND, ADP_CC_MSG_SRC_MOD);
	}
	TBM_EXIT(0x29c0);
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_RELEASECALL_rsp
	PURPOSE			    : process EV_CFW_CC_RELEASE_CALL_RSP event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_RELEASECALL_rsp(COS_EVENT* pCosEvent)
{
	UINT8   nType;	
	mmi_cc_call_release_ind_struct *pMmiReleaseIndRsp, *pMmiAthRsp;
	TBM_ENTRY(0x29c1);
	ASSERT(pCosEvent != NULL);	
	
	nType = HIUINT8(pCosEvent->nParam3);
	
	if ((0x10 == nType) || (0xF0 == nType))
	{//success

		pMmiReleaseIndRsp = 
			(mmi_cc_call_release_ind_struct*)OslMalloc(sizeof(mmi_cc_call_release_ind_struct));

		pMmiAthRsp = 
			(mmi_cc_call_release_ind_struct*)OslMalloc(sizeof(mmi_cc_call_release_ind_struct));

		ASSERT(pMmiReleaseIndRsp != NULL);
		memset(pMmiReleaseIndRsp, 0, sizeof(mmi_cc_call_release_ind_struct));

		ASSERT(pMmiAthRsp != NULL);
		memset(pMmiAthRsp, 0, sizeof(mmi_cc_call_release_ind_struct));

		{//send release ind 	
		
			if (0x10 == nType)
			{
				pMmiReleaseIndRsp->call_id = LOUINT8(pCosEvent->nParam1);
			}
			else
			{
				pMmiReleaseIndRsp->call_id = HIUINT16(pCosEvent->nParam2);
			}
			
			pMmiReleaseIndRsp->cause = 0;
			
			mmi_trace(g_sw_ADP_CC, "MMI_Call: pMmiReleaseIndRsp->call_id=%d", pMmiReleaseIndRsp->call_id);
			
			#if CC_TEST_SWITCH
			{
				CFW_CC_CURRENT_CALL_INFO	CswCallInfo[MAX_CALLS];
				UINT8	CswCallInfoCnt=0;
				memset(CswCallInfo,0x0,MAX_CALLS*sizeof(CFW_CC_CURRENT_CALL_INFO) );
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				UT_CFW_CcGetCurrentCall(CswCallInfo, &CswCallInfoCnt, g_pCCContext->sim_id);	
#else
				CFW_CcGetCurrentCall(CswCallInfo, &CswCallInfoCnt, g_pCCContext->sim_id);	
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
				CFW_CcGetCurrentCall(CswCallInfo, &CswCallInfoCnt);	
#endif /* __ADP_MULTI_SIM__ */
				mmi_trace(g_sw_ADP_CC, "MMI_Call: &&&INFO_IND->CswCallInfoCnt=%d\n",CswCallInfoCnt);
			}
			#endif

			memcpy(pMmiAthRsp, pMmiReleaseIndRsp, sizeof(mmi_cc_call_release_ind_struct));

			Adp_SendMessageToMMI((local_para_struct *)pMmiAthRsp, MSG_ID_MMI_CC_ATH_RSP, ADP_CC_MSG_SRC_MOD);

			Adp_SendMessageToMMI((local_para_struct *)pMmiReleaseIndRsp, 
											MSG_ID_MMI_CC_CALL_RELEASE_IND, ADP_CC_MSG_SRC_MOD);
		}

		if (g_AdpCcState != 0)
		{
			Adp_ActiveDialStateReleaseProcess(pCosEvent);
		}
		
	}

	else
	{//send release failure
		ASSERT(0);
		mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) adp_CC_proc_RELEASECALL_rsp << ERR_TEMPORARY_FAILURE");
	}
	TBM_EXIT(0x29c1);
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_CALLHOLDMULTIPARTY_rsp
	PURPOSE			    : process EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_CALLHOLDMULTIPARTY_rsp(COS_EVENT* pCosEvent)
{
	UINT8   nType;

	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29c2);
	nType = HIUINT8(pCosEvent->nParam3);
	#ifdef __MMI_SPEEDUP_RELEASE_CALL__
	if(nDummySendReleaseCallMsg > 0)
	{
		nDummySendReleaseCallMsg --;
        	mmi_trace(0, "MMI_Call: FUNC:%s nDummySendReleaseCallMsg %d", __FUNCTION__, nDummySendReleaseCallMsg);

		return;
	}
	#endif
	mmi_trace(g_sw_ADP_CC, "MMI_Call: adp_CC_proc_CALLHOLDMULTIPARTY_rsp g_AdpCcState=%d", g_AdpCcState);
	if (g_AdpCcState == ACTIVE_DIAL)
	{
		Adp_ActiveDialStateMultiRspProcess(pCosEvent);
	}
	
	else
	{
		if ((0x0F == nType) || (0 == nType))
		{//success

			mmi_cc_chld_rsp_struct *pMmiChldRsp = 
				(mmi_cc_chld_rsp_struct*)OslMalloc(sizeof(mmi_cc_chld_rsp_struct));
			
			ASSERT(pMmiChldRsp != NULL);
			memset(pMmiChldRsp, 0, sizeof(mmi_cc_chld_rsp_struct));

			pMmiChldRsp->result = KAL_TRUE;//not use (L4C_OK)
			
			Adp_SendMessageToMMI((local_para_struct *)pMmiChldRsp, MSG_ID_MMI_CC_CHLD_RSP, ADP_CC_MSG_SRC_MOD);
			
		}

		else if ((0x10 == nType) || ((0xF0 == nType) && (pCosEvent->nParam1 == 0)) || (0x01 == nType))
		{//release
			{
				mmi_cc_chld_rsp_struct *pMmiChldRsp = 
					(mmi_cc_chld_rsp_struct*)OslMalloc(sizeof(mmi_cc_chld_rsp_struct));
				
				ASSERT(pMmiChldRsp != NULL);
				memset(pMmiChldRsp, 0, sizeof(mmi_cc_chld_rsp_struct));

				pMmiChldRsp->result = KAL_TRUE;//not use (L4C_OK)
				
				Adp_SendMessageToMMI((local_para_struct *)pMmiChldRsp, MSG_ID_MMI_CC_CHLD_RSP, ADP_CC_MSG_SRC_MOD);
			}
			{
				mmi_cc_call_release_ind_struct *pMmiReleaseIndRsp = 
									(mmi_cc_call_release_ind_struct*)OslMalloc(sizeof(mmi_cc_call_release_ind_struct));
				ASSERT(pMmiReleaseIndRsp != NULL);
				memset(pMmiReleaseIndRsp, 0, sizeof(mmi_cc_call_release_ind_struct));

				if (0xF0 == nType)
				{
					pMmiReleaseIndRsp->call_id = HIUINT16(pCosEvent->nParam2);
				}
				else if ((0x10 == nType) || (0x01 == nType))
				{
					pMmiReleaseIndRsp->call_id = LOUINT8(pCosEvent->nParam1);
				}
				
				pMmiReleaseIndRsp->cause = 0;

				mmi_trace(g_sw_ADP_CC, "MMI_Call: pMmiReleaseIndRsp->call_id = %d", pMmiReleaseIndRsp->call_id);
				mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>MSG_ID_MMI_CC_CALL_RELEASE_IND<- adp_CC_proc_CALLHOLDMULTIPARTY_rsp\n");
				Adp_SendMessageToMMI((local_para_struct *)pMmiReleaseIndRsp, 
										MSG_ID_MMI_CC_CALL_RELEASE_IND, ADP_CC_MSG_SRC_MOD);
				TBM_EXIT(0x29c2);
				return;		
			}

		}

		else
		{//nTpye == 0xF1
		
			mmi_cc_chld_rsp_struct *pMmiChldRsp = 
				(mmi_cc_chld_rsp_struct*)OslMalloc(sizeof(mmi_cc_chld_rsp_struct));

			ASSERT(pMmiChldRsp != NULL);
			memset(pMmiChldRsp, 0, sizeof(mmi_cc_chld_rsp_struct));
			
			pMmiChldRsp->result = KAL_FALSE;
			pMmiChldRsp->cause = ERR_NETWORK_FAILURE;
			
			mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) >>>>ERR_SUCCESS != adp_CC_proc_CALLHOLDMULTIPARTY_rsp\n");

			Adp_SendMessageToMMI((local_para_struct *)pMmiChldRsp, 
											MSG_ID_MMI_CC_CHLD_RSP, ADP_CC_MSG_SRC_MOD);
		}
	}
	TBM_EXIT(0x29c2);

}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_ACCEPTSPEECHCALL_rsp
	PURPOSE			    : process EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_ACCEPTSPEECHCALL_rsp(COS_EVENT* pCosEvent)
{
	
 	UINT32 nParam1;
	//UINT32 nParam2;
	UINT8   nType;
	CFW_CC_CURRENT_CALL_INFO	CallInfo;
	mmi_cc_ata_rsp_struct *pMmiAtaRsp;
	TBM_ENTRY(0x29c3);
	ASSERT(pCosEvent != NULL);

	nType = HIUINT8(pCosEvent->nParam3);
	nParam1 = pCosEvent->nParam1;
	//nParam2 = pCosEvent->nParam2;
	
	pMmiAtaRsp = (mmi_cc_ata_rsp_struct*)OslMalloc(sizeof(mmi_cc_ata_rsp_struct));
	
	ASSERT(pMmiAtaRsp != NULL);
	memset(pMmiAtaRsp, 0, sizeof(mmi_cc_ata_rsp_struct));	

	Adp_GetCallInfo (&CallInfo, CFW_CM_STATUS_ACTIVE);
	pMmiAtaRsp->call_mode = CallInfo.mode;
		
	if (0x0F == nType)
	{//success
		pMmiAtaRsp->result.flag = L4C_OK;
		pMmiAtaRsp->result.cause = 0;
	}

	else if (0x10 == nType)
	{//timeout error
		pMmiAtaRsp->result.flag = L4C_ERROR;
		pMmiAtaRsp->result.cause = CM_MM_CONN_TIMEOUT;
	}

	else if ((0xF0 == nType))
		//&& (LOUINT16(nParam2) = CFW_CC_CAUSE_LOW_LAYER_FAIL))
	{
		pMmiAtaRsp->result.flag = L4C_ERROR;
		pMmiAtaRsp->result.cause = nParam1;
	}

	else
	{
		ASSERT(0);
	}
	mmi_trace(TRUE,"MMI_Call:(WARN) %s EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP cause=%d", __FUNCTION__, pMmiAtaRsp->result.cause);
	
	Adp_SendMessageToMMI((local_para_struct *)pMmiAtaRsp, 
									MSG_ID_MMI_CC_ATA_RSP, ADP_CC_MSG_SRC_MOD);

	if (L4C_ERROR == pMmiAtaRsp->result.flag)
	{
		mmi_cc_call_release_ind_struct *pMmiReleaseIndRsp = 
							(mmi_cc_call_release_ind_struct*)OslMalloc(sizeof(mmi_cc_call_release_ind_struct));
		ASSERT(pMmiReleaseIndRsp != NULL);
		memset(pMmiReleaseIndRsp, 0, sizeof(mmi_cc_call_release_ind_struct));

		if (0x10 == nType)
		{
			pMmiReleaseIndRsp->call_id = pCosEvent->nParam1;
		}
		else
		{//0xF0 == nType
			pMmiReleaseIndRsp->call_id = HIUINT16(pCosEvent->nParam2);
		}
		
		pMmiReleaseIndRsp->cause = 0;

		mmi_trace(g_sw_ADP_CC, "MMI_Call: pMmiReleaseIndRsp->call_id = %d", pMmiReleaseIndRsp->call_id);
		mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) >>>>MSG_ID_MMI_CC_CALL_RELEASE_IND<- adp_CC_proc_CALLHOLDMULTIPARTY_rsp\n");
		Adp_SendMessageToMMI((local_para_struct *)pMmiReleaseIndRsp, 
								MSG_ID_MMI_CC_CALL_RELEASE_IND, ADP_CC_MSG_SRC_MOD);
		TBM_EXIT(0x29c3);
		return;		
	}
	TBM_EXIT(0x29c3);
}

/**************************************************************
	FUNCTION NAME		: Adp_SendCCParsingStringResul
	PURPOSE			    : send  result of  parsing string
	INPUT PARAMETERS	: csmss_string_op_enum OpCode
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void Adp_SendCCParsingStringResult(csmss_string_op_enum OpCode)
{
	mmi_ss_parsing_string_rsp_struct *pMmiParseStrRsq =
			(mmi_ss_parsing_string_rsp_struct*)OslMalloc(sizeof(mmi_ss_parsing_string_rsp_struct));
	TBM_ENTRY(0x29c4);
	ASSERT(pMmiParseStrRsq != NULL);
	memset(pMmiParseStrRsq, 0, sizeof(mmi_ss_parsing_string_rsp_struct));	

	if (CC_OPERATION == OpCode)
	{
		pMmiParseStrRsq->info.opcode = CC_OPERATION;
		pMmiParseStrRsq->result.flag = 1;
	}
	else
	{
		pMmiParseStrRsq->info.opcode = OpCode;
		pMmiParseStrRsq->result.flag = 0;
	}
	Adp_SendMessageToMMI((local_para_struct *)pMmiParseStrRsq, 
									MSG_ID_MMI_SS_PARSING_STRING_RSP, ADP_CC_MSG_SRC_MOD);
	TBM_EXIT(0x29c4);
}

/**************************************************************
	FUNCTION NAME		: Adp_SendCCDailIndMessage
	PURPOSE			    	: send  dial ind message
	INPUT PARAMETERS	: UINT8 CallIndex
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
void Adp_SendCCDailIndMessage(UINT8 CallIndex, module_type DestMode)
{
	//send MSG_ID_MMI_CC_DIAL_IND
	mmi_cc_dial_ind_struct *pMmiDialInd = 
		(mmi_cc_dial_ind_struct*)OslMalloc(sizeof(mmi_cc_dial_ind_struct));
	
	ASSERT(pMmiDialInd != NULL);
	memset(pMmiDialInd, 0, sizeof(mmi_cc_dial_ind_struct));
	
	pMmiDialInd->result.flag = L4C_OK;
	pMmiDialInd->call_id = CallIndex;//call index

	Adp_SendMessageToMMI((local_para_struct *)pMmiDialInd, 
									MSG_ID_MMI_CC_DIAL_IND, DestMode);	
}

/**************************************************************
	FUNCTION NAME		: Adp_ActiveDialStateReleaseProcess
	PURPOSE			    : process EV_CFW_CC_RELEASE_CALL_IND massage when  active call to dial call 
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void Adp_ActiveDialStateReleaseProcess(COS_EVENT* pCosEvent)
{
	CFW_CC_CURRENT_CALL_INFO	CallActiveInfo;
	CFW_CC_CURRENT_CALL_INFO	CallInfo[MAX_CALLS];
	
	UINT8 Count;
	BOOL HaveActiveCall = FALSE;

	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29c5);

	memset(&CallActiveInfo, 0, sizeof(CallActiveInfo));
	memset(CallInfo, 0, sizeof(CallInfo));
	
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_CcGetCurrentCall(CallInfo, &Count, g_pCCContext->sim_id);
#else
	CFW_CcGetCurrentCall(CallInfo, &Count, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
	CFW_CcGetCurrentCall(CallInfo, &Count);
#endif /* __ADP_MULTI_SIM__ */
	HaveActiveCall = Adp_GetCallInfo (&CallActiveInfo, CFW_CM_STATUS_ACTIVE);
	
	mmi_trace(g_sw_ADP_CC, "MMI_Call: Adp_ActiveDialStateReleaseProcess g_AdpCcState=%d, Count=%d, HaveActiveCall=%d", g_AdpCcState, Count, HaveActiveCall);
	if (((g_AdpCcState == ACTIVE_EMERGENCY) && (Count == 0))
		|| ((g_AdpCcState == ACTIVE_DIAL) && (HaveActiveCall == FALSE)))
	{
		UINT32 InitiateCallRtn;
		UINT8	CallIndex;

		
		g_AdpCcState = 0;
		
		InitiateCallRtn = Adp_GetDialCallInfoAndDialCall(&g_MmiParseStrReq, &CallIndex);
		mmi_trace(g_sw_ADP_CC, "MMI_Call:(WARN) CallIndex=%d, InitiateCallRtn=0x%x", CallIndex, InitiateCallRtn);
		if (ERR_SUCCESS != InitiateCallRtn)
		{
			Adp_SendCCParsingStringResult(INVALID_OPERATION);
		}
		else
		{
			Adp_SendCCParsingStringResult(CC_OPERATION);
			Adp_SendCCDailIndMessage(CallIndex, ADP_CC_MSG_SRC_MOD);
		}
	}
	TBM_EXIT(0x29c5);
}

/**************************************************************
	FUNCTION NAME		: Adp_ActiveDialStateMultiRspProcess
	PURPOSE			    : process EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP massage when  active call to dial call 
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void Adp_ActiveDialStateMultiRspProcess(COS_EVENT* pCosEvent)
{
	CFW_CC_CURRENT_CALL_INFO	CallInfo;
	UINT8 nType;
	ASSERT(pCosEvent != NULL);
	
	nType = HIUINT8(pCosEvent->nParam3);
	TBM_ENTRY(0x29c6);
	//success
	if (0 == nType)
	{
		if (Adp_GetCallInfo (&CallInfo, CFW_CM_STATUS_ACTIVE) == FALSE)
		{
			UINT32 InitiateCallRtn;
			UINT8	CallIndex;
			
			g_AdpCcState = 0;
			
			InitiateCallRtn = Adp_GetDialCallInfoAndDialCall(&g_MmiParseStrReq, &CallIndex);
			mmi_trace(g_sw_ADP_CC, "MMI_Call:(WARN) Adp_ActiveDialStateMultiRspProcess CallIndex=%d, InitiateCallRtn=0x%x", CallIndex, InitiateCallRtn);
			if (ERR_SUCCESS != InitiateCallRtn)
			{
				Adp_SendCCParsingStringResult(INVALID_OPERATION);
			}
			else
			{
				Adp_SendCCParsingStringResult(CC_OPERATION);
				Adp_SendCCDailIndMessage(CallIndex, ADP_CC_MSG_SRC_MOD);
			}
		}
	}

	else
	{
		mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) Adp_ActiveDialStateMultiRspProcess");
		g_AdpCcState = 0;
		Adp_SendCCParsingStringResult(INVALID_OPERATION);
	}
	TBM_EXIT(0x29c6);
}

/**************************************************************
	FUNCTION NAME		: Adp_GetDialCallInfoAndDialCall
	PURPOSE			    : get dial number information and Initiate a Call
	INPUT PARAMETERS	: mmi_ss_parsing_string_req_struct *pMmiParseStrReq, UINT8 *pCallIndex
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
#ifndef __ADP_MULTI_SIM__
extern UINT32 CFW_CcEmcDialEx(UINT8* pDialNumber, UINT8 nDialNumberSize, UINT8 *pIndex);
extern UINT32 CFW_CcInitiateSpeechCallEx(CFW_DIALNUMBER* pDialNumber, UINT8 *pIndex, UINT16 nUTI);
#endif
static UINT32  Adp_GetDialCallInfoAndDialCall(mmi_ss_parsing_string_req_struct *pMmiParseStrReq, UINT8 *pCallIndex)
{
	UINT32 InitiateCallRtn;
	UINT8 DailNumASCIISize;
	UINT8 DailNumBCDSize;
	UINT8* pAsciiDialNum;
	UINT8 AsciiDialNum[MAX_DIGITS_USSD + 11] = {0};//vivian add 2008/5/15  MAX_IP_NUMBER_LEN + 1
	CFW_DIALNUMBER	 DailNumberText;
	UINT8 CallType;
	TBM_ENTRY(0x29c7);
	
	pAsciiDialNum = Adp_CheckCallType(pMmiParseStrReq->input, &CallType);

	if( pMmiParseStrReq ->ip_string[0] != '\0')
	{
		mmi_trace(g_sw_ADP_CC, "MMI_Call: Adp_GetDialCallInfoAndDialCall have ip string: %s \n", pMmiParseStrReq ->ip_string);
		strcpy(AsciiDialNum, pMmiParseStrReq ->ip_string);
		strcat(AsciiDialNum, pAsciiDialNum);
	}
	else
	{
		mmi_trace(g_sw_ADP_CC, "MMI_Call: Adp_GetDialCallInfoAndDialCall don't have ip string \n");
		strcpy(AsciiDialNum, pAsciiDialNum);
	}
	
	mmi_trace(g_sw_ADP_CC, "MMI_Call:  number string: %s \n",AsciiDialNum);
	DailNumASCIISize = strlen(AsciiDialNum);
	DailNumBCDSize = DailNumASCIISize / 2 + DailNumASCIISize % 2;
	DailNumberText.pDialNumber = (UINT8*)OslMalloc(DailNumBCDSize);
	ASSERT(DailNumberText.pDialNumber != NULL);
	memset(DailNumberText.pDialNumber, 0, DailNumBCDSize);
	
	SUL_AsciiToGsmBcd(AsciiDialNum, DailNumASCIISize, DailNumberText.pDialNumber);

	#if CC_TEST_SWITCH
	{
		UINT8	i;
		for (i = 0; i < DailNumBCDSize; i++)
		{
			mmi_trace(g_sw_ADP_CC, "MMI_Call: pDialNumber[%d] = 0x%x \n", 
						i, *((UINT8*)&DailNumberText.pDialNumber[i]));
		}

	}
	#endif
	
	if (pMmiParseStrReq->IsEmergencyNo == TRUE)
	{
		//InitiateCallRtn = CFW_CcEmcDial(DailNumberText.pDialNumber, DailNumBCDSize);
		mmi_trace(g_sw_ADP_CC, "MMI_Call: Emergency Call");
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		InitiateCallRtn = UT_CFW_CcEmcDialEx(DailNumberText.pDialNumber, DailNumBCDSize, pCallIndex, g_pCCContext->sim_id);
#else
		InitiateCallRtn = CFW_CcEmcDialEx(DailNumberText.pDialNumber, DailNumBCDSize, pCallIndex, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
		InitiateCallRtn = CFW_CcEmcDialEx(DailNumberText.pDialNumber, DailNumBCDSize, pCallIndex);
#endif /* __ADP_MULTI_SIM__ */

	}
	else
	{	
		DailNumberText.nDialNumberSize = DailNumBCDSize;
		DailNumberText.nType = CallType;
		DailNumberText.nClir = 0;	//保持业务原来的设置.
		mmi_trace(g_sw_ADP_CC, "MMI_Call: normal Call");
		
		//InitiateCallRtn = CFW_CcInitiateSpeechCall(&DailNumberText, 6);
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		InitiateCallRtn = UT_CFW_CcInitiateSpeechCallEx(&DailNumberText, pCallIndex, FREE_UTI, g_pCCContext->sim_id);
#else
		InitiateCallRtn = CFW_CcInitiateSpeechCallEx(&DailNumberText, pCallIndex, FREE_UTI, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
		InitiateCallRtn = CFW_CcInitiateSpeechCallEx(&DailNumberText, pCallIndex, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
	}
	
	OslMfree(DailNumberText.pDialNumber);
	TBM_EXIT(0x29c7);
	return InitiateCallRtn;	
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_PARSINGSTRING_req
	PURPOSE			    : process MSG_ID_MMI_SS_PARSING_STRING_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_PARSINGSTRING_req(COS_EVENT* pCosEvent)
{
	ilm_struct *oslMessage;
	mmi_ss_parsing_string_req_struct *pMmiParseStrReq;
	TBM_ENTRY(0x29c8);
	ASSERT(pCosEvent != NULL);

	oslMessage = (ilm_struct *) pCosEvent->nParam1;
	
	pMmiParseStrReq = 
		(mmi_ss_parsing_string_req_struct*)oslMessage->local_para_ptr;

	if (Adp_IsCCString (pMmiParseStrReq->input))
	{//is call string
	
		CFW_CC_CURRENT_CALL_INFO CallInfo[MAX_CALLS];
		CFW_CC_CURRENT_CALL_INFO	CallActiveInfo;
		CFW_CC_CURRENT_CALL_INFO	CallHoldInfo;
		UINT8	Count = 0;
		UINT8	CallIndex;
		UINT32 InitiateCallRtn;
		BOOL IsEmergencyNo = pMmiParseStrReq->IsEmergencyNo;
		BOOL HaveActiveCall = FALSE;
		BOOL HaveHoldCall = FALSE;

		memset(CallInfo, 0, sizeof(CallInfo));
		
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		UT_CFW_CcGetCurrentCall(CallInfo, &Count, g_pCCContext->sim_id);
#else
		CFW_CcGetCurrentCall(CallInfo, &Count, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
		CFW_CcGetCurrentCall(CallInfo, &Count);
#endif /* __ADP_MULTI_SIM__ */
		HaveActiveCall = Adp_GetCallInfo (&CallActiveInfo, CFW_CM_STATUS_ACTIVE);
		HaveHoldCall = Adp_GetCallInfo (&CallHoldInfo, CFW_CM_STATUS_HELD);
			
		mmi_trace(g_sw_ADP_CC, "MMI_Call:  Func: %s callcount=%d, HaveActiveCall=%d, HaveHoldCall=%d, IsEmergencyNo=%d", __FUNCTION__, Count, HaveActiveCall, HaveHoldCall, IsEmergencyNo);
		if ((Count == 0)//no call 
			|| ((IsEmergencyNo == FALSE) && (HaveActiveCall == FALSE)))
		{
			mmi_trace(g_sw_ADP_CC, "MMI_Call: initiate a call");
			InitiateCallRtn = Adp_GetDialCallInfoAndDialCall(pMmiParseStrReq, &CallIndex);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: CallIndex=%d", CallIndex);

			if (ERR_SUCCESS != InitiateCallRtn)
			{
				mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR)  InitiateCallRtn=0x%x", InitiateCallRtn);
				Adp_SendCCParsingStringResult(INVALID_OPERATION);
			}
			else
			{
				Adp_SendCCParsingStringResult(CC_OPERATION);
				Adp_SendCCDailIndMessage(CallIndex, ADP_CC_MSG_SRC_MOD);
			}

		}
		else //have call
		{
			if (IsEmergencyNo == TRUE)
			{
				mmi_trace(g_sw_ADP_CC, "MMI_Call: Emergency Call release all call");
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				InitiateCallRtn = UT_CFW_CcReleaseCall(g_pCCContext->sim_id);
#else
				InitiateCallRtn = CFW_CcReleaseCall(g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
				InitiateCallRtn = CFW_CcReleaseCall();
#endif /* __ADP_MULTI_SIM__ */
				g_AdpCcState = ACTIVE_EMERGENCY;
			}
			else if (HaveHoldCall == TRUE)//have hold and active call
			{
				mmi_trace(g_sw_ADP_CC, "MMI_Call: have hold and active call");
				Adp_SendCCParsingStringResult(INVALID_OPERATION);
				TBM_EXIT(0x29c8);
				return;
			}
			
			else //have active call
			{
				mmi_trace(g_sw_ADP_CC, "MMI_Call: Hold all Active Call");
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				InitiateCallRtn = UT_CFW_CcCallHoldMultiparty(2, 0xFF, g_pCCContext->sim_id);
#else
				InitiateCallRtn = CFW_CcCallHoldMultiparty(2, 0xFF, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
				InitiateCallRtn = CFW_CcCallHoldMultiparty(2, 0xFF);
#endif /* __ADP_MULTI_SIM__ */
				g_AdpCcState = ACTIVE_DIAL;		
			}

			g_MmiParseStrReq = *(pMmiParseStrReq);
			mmi_trace(g_sw_ADP_CC,"MMI_Call:(WARN) Func: %s InitiateCallRtn=0x%x", __FUNCTION__, InitiateCallRtn);			
		}

	}
	
	else if (Adp_IsSSString (pMmiParseStrReq->input))
	{//is SS string
		mmi_trace(g_sw_ADP_CC,"MMI_Call: Func: %s is SS string", __FUNCTION__);
#ifdef __ADP_MULTI_SIM__
		adp_ConfigureContext(ADP_MODULE_SS, g_pCCContext->sim_id); //get the context var
#endif
		SS_Adaption_ParseStringREQ(pMmiParseStrReq);
	}
	
	else
	{
		mmi_trace(g_sw_ADP_CC,"MMI_Call:(ERROR) Func: %s Not CC and SS string", __FUNCTION__);
		Adp_SendCCParsingStringResult(INVALID_OPERATION);
	}
	TBM_EXIT(0x29c8);
}

#if 	CC_TEST_SWITCH
//code for test
char *CC_CHLDREQText[]=	//message text content
{
	"CSMCC_REL_HELD_OR_UDUB", /*0*/
	"CSMCC_REL_ACTIVE_AND_ACCEPT", /*1*/
	"CSMCC_REL_SPECIFIC_CALL", /*2*/
	"CSMCC_HOLD_ACTIVE_AND_ACCEPT", /*3*/
	"CSMCC_HOLD_ACTIVE_EXCEPT_SPECIFIC_CALL", /*4*/
	"CSMCC_ADD_HELD_CALL", /*5*/
	"CSMCC_EXPLICIT_CALL_TRANSFER", /*6*/
	"CSMCC_ACTIVATE_CCBS_CALL", /*7*/
	"CSMCC_REL_ALL_EXCEPT_WAITING_CALL", /*8*/
	"CSMCC_REL_SPECIFIC_ACTIVE_CALL", /* 9 */
	"CSMCC_SWAP_CALL", /* 10 */
	"CSMCC_REL_HELD",  /* 11 */
	"CSMCC_REL_ACTIVE",  /* 12 */
	"CSMCC_REL_ALL",  /* 13 */
};
#endif

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_DEFLECT_req
	PURPOSE			    : process MSG_ID_MMI_CC_CALL_DEFLECT_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_DEFLECT_req(COS_EVENT *pCosEvent)
{
	ilm_struct *oslMessage;
	mmi_cc_call_deflect_req_struct *pMmiDeflectReq;
	TBM_ENTRY(0x29c9);
	ASSERT(pCosEvent != NULL);
	
	oslMessage = (ilm_struct *) pCosEvent->nParam1;
	pMmiDeflectReq = (mmi_cc_call_deflect_req_struct*)oslMessage->local_para_ptr;

	{
		mmi_cc_call_deflect_rsp_struct *pMmiDeflectRsp = 
				(mmi_cc_call_deflect_rsp_struct*)OslMalloc(sizeof(mmi_cc_call_deflect_rsp_struct));

		ASSERT(pMmiDeflectRsp != NULL);
		memset(pMmiDeflectRsp, 0, sizeof(mmi_cc_call_deflect_rsp_struct));

		pMmiDeflectRsp->result = FALSE;
		pMmiDeflectRsp->cause = ERR_TEMPORARY_FAILURE;

		Adp_SendMessageToMMI((local_para_struct *)pMmiDeflectRsp, 
										MSG_ID_MMI_CC_CALL_DEFLECT_RSP, ADP_CC_MSG_SRC_MOD);
	}
	TBM_EXIT(0x29c9);
}
#ifdef __MMI_SPEEDUP_RELEASE_CALL__
static void adp_CC_dummy_release_call_msg(kal_uint8 call_id)
{
	mmi_cc_chld_rsp_struct *pMmiChldRsp = NULL;
	mmi_cc_call_release_ind_struct *pMmiReleaseIndRsp = NULL;

	#ifdef __MMI_CONTROL_SWAP_CALL_RESPONSE__
	if(nWaitSwapCallResponse)
		return;
	#endif
	pMmiChldRsp = (mmi_cc_chld_rsp_struct*)OslMalloc(sizeof(mmi_cc_chld_rsp_struct));

	ASSERT(pMmiChldRsp != NULL);
	memset(pMmiChldRsp, 0, sizeof(mmi_cc_chld_rsp_struct));

	pMmiChldRsp->result = KAL_TRUE;//not use (L4C_OK)

	Adp_SendMessageToMMI((local_para_struct *)pMmiChldRsp, MSG_ID_MMI_CC_CHLD_RSP, ADP_CC_MSG_SRC_MOD);


	pMmiReleaseIndRsp = (mmi_cc_call_release_ind_struct*)OslMalloc(sizeof(mmi_cc_call_release_ind_struct));
	ASSERT(pMmiReleaseIndRsp != NULL);
	memset(pMmiReleaseIndRsp, 0, sizeof(mmi_cc_call_release_ind_struct));

	pMmiReleaseIndRsp->call_id = call_id;
	pMmiReleaseIndRsp->cause = 0;

	Adp_SendMessageToMMI((local_para_struct *)pMmiReleaseIndRsp, MSG_ID_MMI_CC_CALL_RELEASE_IND, ADP_CC_MSG_SRC_MOD);
	
	nDummySendReleaseCallMsg ++;

	mmi_trace(0, "MMI_CALL: adp_CC_dummy_release_call_msg, nDummySendReleaseCallMsg is %d",nDummySendReleaseCallMsg);
}
#endif

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_CHLD_req
	PURPOSE			    : process MSG_ID_MMI_CC_CHLD_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_CHLD_req(COS_EVENT* pCosEvent)
{
	ilm_struct *oslMessage;
	mmi_cc_chld_req_struct *pMmiChldReq;
	UINT32	RtnValue;
	TBM_ENTRY(0x29ca);
	ASSERT(pCosEvent != NULL);
	
	oslMessage = (ilm_struct *) pCosEvent->nParam1;
	pMmiChldReq = (mmi_cc_chld_req_struct*)oslMessage->local_para_ptr;
	
	mmi_trace(g_sw_ADP_CC, "MMI_Call: pMmiChldReq->opcode = %s", CC_CHLDREQText[pMmiChldReq->opcode]);
	mmi_trace(g_sw_ADP_CC, "MMI_Call: pMmiChldReq->call_id = %d", pMmiChldReq->call_id);
	switch (pMmiChldReq->opcode)
	{
		//end a specific active call
		case CSMCC_REL_SPECIFIC_ACTIVE_CALL:
		case CSMCC_REL_SPECIFIC_CALL:
		{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_CcCallHoldMultiparty(1, pMmiChldReq->call_id, g_pCCContext->sim_id);
#else
			RtnValue = CFW_CcCallHoldMultiparty(1, pMmiChldReq->call_id, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue = CFW_CcCallHoldMultiparty(1, pMmiChldReq->call_id);
#endif /* __ADP_MULTI_SIM__ */
		}
			break;

		//release active call
		case CSMCC_REL_ACTIVE:
		{
			CFW_CC_CURRENT_CALL_INFO 	CallInfo[MAX_CALLS];
			UINT8	Count = 0;
			UINT8	i;
			
			memset(CallInfo, 0, MAX_CALLS * sizeof(CFW_CC_CURRENT_CALL_INFO));

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_CcGetCurrentCall(CallInfo, &Count, g_pCCContext->sim_id);
#else
			RtnValue = CFW_CcGetCurrentCall(CallInfo, &Count, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue = CFW_CcGetCurrentCall(CallInfo, &Count);
#endif /* __ADP_MULTI_SIM__ */
			if (ERR_SUCCESS ==  RtnValue)
			{
				if ((Count > 0) && (Count <= MAX_CALLS))
				{
					for (i = 0; i < Count; i++)
					{
						if (CFW_CM_STATUS_ACTIVE == CallInfo[i].status)
						{
							
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
							RtnValue = UT_CFW_CcCallHoldMultiparty(1, CallInfo[i].idx,g_pCCContext->sim_id);
#else
							RtnValue = CFW_CcCallHoldMultiparty(1, CallInfo[i].idx,g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
							RtnValue = CFW_CcCallHoldMultiparty(1, CallInfo[i].idx);
#endif /* __ADP_MULTI_SIM__ */

							mmi_trace(g_sw_ADP_CC, "MMI_Call:(WARN) >>>>call_id = %d, RtnValue=0x%x", CallInfo[i].idx, RtnValue);							
						}
					}
				}
			}

			else
			{
				ASSERT(0);
			}
		}
			break;

		//release active call and accept waiting call
		case CSMCC_REL_ACTIVE_AND_ACCEPT:
		{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_CcCallHoldMultiparty(1, 0xFF, g_pCCContext->sim_id);
#else
			RtnValue = CFW_CcCallHoldMultiparty(1, 0xFF, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue = CFW_CcCallHoldMultiparty(1, 0xFF);
#endif /* __ADP_MULTI_SIM__ */
		}
			break;

		//release all call except waiting call
		case CSMCC_REL_ALL_EXCEPT_WAITING_CALL:
		{
			CFW_CC_CURRENT_CALL_INFO 	CallInfoAll[MAX_CALLS];
			UINT8	Count = 0;
			UINT8	i;
			
			memset(CallInfoAll, 0, MAX_CALLS * sizeof(CFW_CC_CURRENT_CALL_INFO));
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_CcGetCurrentCall(CallInfoAll, &Count, g_pCCContext->sim_id);
#else
			RtnValue = CFW_CcGetCurrentCall(CallInfoAll, &Count, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue = CFW_CcGetCurrentCall(CallInfoAll, &Count);
#endif /* __ADP_MULTI_SIM__ */
			if (ERR_SUCCESS == RtnValue)
			{
				if ((Count > 0) && (Count <= MAX_CALLS))
				{
					for (i = 0; i < Count; i++)
					{
//						if ((CFW_CM_STATUS_HELD == CallInfoAll[i].status)
//							|| (CFW_CM_STATUS_ACTIVE == CallInfoAll[i].status)
//							|| (CFW_CM_STATUS_DIALING == CallInfoAll[i].status)
						if(CFW_CM_STATUS_WAITING != CallInfoAll[i].status)//man
						{
							
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
							RtnValue = UT_CFW_CcCallHoldMultiparty(1, CallInfoAll[i].idx, g_pCCContext->sim_id);
#else
							RtnValue = CFW_CcCallHoldMultiparty(1, CallInfoAll[i].idx, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
							RtnValue = CFW_CcCallHoldMultiparty(1, CallInfoAll[i].idx);
#endif /* __ADP_MULTI_SIM__ */
							#ifdef __MMI_SPEEDUP_RELEASE_CALL__
							if(ERR_SUCCESS == RtnValue)
							{                     
							    adp_CC_dummy_release_call_msg(CallInfoAll[i].idx);
							}
							#endif
							mmi_trace(g_sw_ADP_CC, "MMI_Call:(WARN) >>>>call_id = %d, RtnValue=0x%x", CallInfoAll[i].idx, RtnValue);
							
						}
					}
				}
			}
			else if (ERR_CFW_NO_CALL_INPROGRESS == RtnValue) //when release IND come before release REQ, it will cause this error. fix bug 14239
			{
				mmi_trace(g_sw_ADP_CC, "MMI_Call:(WARN) ERR_CFW_NO_CALL_INPROGRESS == RtnValue");
				return;
			}
			
		}
			break;

		//hold active call and accept waiting call
		case CSMCC_HOLD_ACTIVE_AND_ACCEPT:
		{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_CcCallHoldMultiparty(2, 0xFF, g_pCCContext->sim_id);
#else
			RtnValue = CFW_CcCallHoldMultiparty(2, 0xFF, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue = CFW_CcCallHoldMultiparty(2, 0xFF);
#endif /* __ADP_MULTI_SIM__ */
		}
			break;

		//MITEM_IAH_OPT_TRANSFER
		case CSMCC_EXPLICIT_CALL_TRANSFER:
		{
			RtnValue = ERR_CFW_INVALID_PARAMETER;
		}
			break;

		//add a hold call to conference
		case CSMCC_ADD_HELD_CALL:
		{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_CcCallHoldMultiparty(3, 0xFF,g_pCCContext->sim_id);
#else
			RtnValue = CFW_CcCallHoldMultiparty(3, 0xFF,g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue = CFW_CcCallHoldMultiparty(3, 0xFF);
#endif /* __ADP_MULTI_SIM__ */
		}
			break;

		//swap  hold call and active call and disregard the waiting call
		case CSMCC_SWAP_CALL:
		{
			CFW_CC_CURRENT_CALL_INFO	CswCallInfo;
			U8 ncmd;
			if (Adp_GetCallInfo (&CswCallInfo, CFW_CM_STATUS_WAITING))
			{
				ncmd = 4;
			}
			else
			{
				ncmd = 2;
			}
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_CcCallHoldMultiparty(ncmd, 0xFF, g_pCCContext->sim_id);
#else
			RtnValue = CFW_CcCallHoldMultiparty(ncmd, 0xFF, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue = CFW_CcCallHoldMultiparty(ncmd, 0xFF);
#endif /* __ADP_MULTI_SIM__ */
			#ifdef __MMI_CONTROL_SWAP_CALL_RESPONSE__
			if(Adp_GetCallInfo (&CswCallInfo, CFW_CM_STATUS_ACTIVE) && Adp_GetCallInfo (&CswCallInfo, CFW_CM_STATUS_HELD))
				nWaitSwapCallResponse = 2;
			#endif
		}
			break;

		//split a call
		case CSMCC_HOLD_ACTIVE_EXCEPT_SPECIFIC_CALL:
		case CSMCC_ACTIVATE_CCBS_CALL:
		{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_CcCallHoldMultiparty(2, pMmiChldReq->call_id, g_pCCContext->sim_id);
#else
			RtnValue = CFW_CcCallHoldMultiparty(2, pMmiChldReq->call_id, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue = CFW_CcCallHoldMultiparty(2, pMmiChldReq->call_id);
#endif /* __ADP_MULTI_SIM__ */
		}
			break;

		//release all call
		case CSMCC_REL_ALL:
		{
			//RtnValue = CFW_CcReleaseCall();
			CFW_CC_CURRENT_CALL_INFO 	CallInfoAll[MAX_CALLS];
			UINT8	Count = 0;
			UINT8	i;

			memset(CallInfoAll, 0, sizeof(CallInfoAll));
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_CcGetCurrentCall(CallInfoAll, &Count, g_pCCContext->sim_id);
#else
			RtnValue = CFW_CcGetCurrentCall(CallInfoAll, &Count, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue = CFW_CcGetCurrentCall(CallInfoAll, &Count);
#endif /* __ADP_MULTI_SIM__ */
			if (ERR_SUCCESS == RtnValue)
			{
				if ((Count > 0) && (Count <= MAX_CALLS))
				{
					for (i = 0; i < Count; i++)
					{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
						RtnValue = UT_CFW_CcCallHoldMultiparty(1, CallInfoAll[i].idx, g_pCCContext->sim_id);
#else
						RtnValue = CFW_CcCallHoldMultiparty(1, CallInfoAll[i].idx, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
						RtnValue = CFW_CcCallHoldMultiparty(1, CallInfoAll[i].idx);
#endif /* __ADP_MULTI_SIM__ */
						mmi_trace(g_sw_ADP_CC, "MMI_Call:(WARN)call_id = %d, RtnValue=0x%x", CallInfoAll[i].idx, RtnValue);

					}

				}
			}
			
		}
			break;

		case CSMCC_REL_HELD_OR_UDUB:
		{

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_CcCallHoldMultiparty(0, 0xff, g_pCCContext->sim_id);
#else
			RtnValue = CFW_CcCallHoldMultiparty(0, 0xff, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue = CFW_CcCallHoldMultiparty(0, 0xff);
#endif /* __ADP_MULTI_SIM__ */

			mmi_trace(g_sw_ADP_CC, "MMI_Call: CSMCC_REL_HELD_OR_UDUB RtnValue=0x%x", RtnValue);
#if 0
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue =  UT_CFW_CcRejectCall(g_pCCContext->sim_id);
#else
			RtnValue =  CFW_CcRejectCall(g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue =  CFW_CcRejectCall();
#endif /* __ADP_MULTI_SIM__ */
#endif /* Guoj delete. It's not used now at 2009-10-14 */

		}
			break;

		case CSMCC_REL_HELD:
		{
			CFW_CC_CURRENT_CALL_INFO 	CallInfoAll[MAX_CALLS];
			UINT8	Count = 0;
			UINT8	i;
			
			memset(CallInfoAll, 0, MAX_CALLS * sizeof(CFW_CC_CURRENT_CALL_INFO));
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_CcGetCurrentCall(CallInfoAll, &Count, g_pCCContext->sim_id);
#else
			RtnValue = CFW_CcGetCurrentCall(CallInfoAll, &Count, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			RtnValue = CFW_CcGetCurrentCall(CallInfoAll, &Count);
#endif /* __ADP_MULTI_SIM__ */
			if (ERR_SUCCESS == RtnValue)
			{
				if ((Count > 0) && (Count <= MAX_CALLS))
				{
					for (i = 0; i < Count; i++)
					{
						if (CFW_CM_STATUS_HELD == CallInfoAll[i].status)
						{
							
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
							RtnValue = UT_CFW_CcCallHoldMultiparty(1, CallInfoAll[i].idx, g_pCCContext->sim_id);
#else
							RtnValue = CFW_CcCallHoldMultiparty(1, CallInfoAll[i].idx, g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
							RtnValue = CFW_CcCallHoldMultiparty(1, CallInfoAll[i].idx);
#endif /* __ADP_MULTI_SIM__ */

							mmi_trace(g_sw_ADP_CC, "MMI_Call:(WARN) >>>>call_id = %d, RtnValue=0x%x", CallInfoAll[i].idx, RtnValue);
							
						}
					}
				}
			}
		}
			break;

		default:
		{
			RtnValue = ERR_CFW_INVALID_PARAMETER;
		}
			break;
	}

	mmi_trace(g_sw_ADP_CC, "MMI_Call:(WARN) adp_CC_proc_CHLD_req RtnValue = 0x%x", RtnValue);
	if (ERR_SUCCESS != RtnValue)
	{
		mmi_cc_chld_rsp_struct *pMmiChldRsp = 
			(mmi_cc_chld_rsp_struct*)OslMalloc(sizeof(mmi_cc_chld_rsp_struct));

		ASSERT(pMmiChldRsp != NULL);
		memset(pMmiChldRsp, 0, sizeof(mmi_cc_chld_rsp_struct));
		
		pMmiChldRsp->result = KAL_FALSE;
		pMmiChldRsp->cause = ERR_TEMPORARY_FAILURE;
		
		mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) >>>>ERR_SUCCESS != RtnValue\n");

		Adp_SendMessageToMMI((local_para_struct *)pMmiChldRsp, 
										MSG_ID_MMI_CC_CHLD_RSP, ADP_CC_MSG_SRC_MOD);
	
	}
	TBM_EXIT(0x29ca);
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_ATH_req
	PURPOSE			    : process MSG_ID_MMI_CC_ATH_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
extern SATCONTEXT gSATContext;
static void adp_CC_proc_ATH_req(COS_EVENT* pCosEvent)
{
	UINT32 	RejectCallRtn;

	ilm_struct *oslMessage;
	mmi_cc_ath_req_struct *pMmiAthReq;
	
	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29cb);
	oslMessage = (ilm_struct *) pCosEvent->nParam1;
	pMmiAthReq = (mmi_cc_ath_req_struct*)oslMessage->local_para_ptr;
	
	/*
	if (L4C_DISCONNECT_MO == pMmiAthReq->op_code)
	{
		
		//process MO 
	}
	if (L4C_DISCONNECT_MT == pMmiAthReq->op_code)
	{
		CFW_CC_CURRENT_CALL_INFO	CallInfo;
		if (Adp_GetCallInfo (&CallInfo, CFW_CM_STATUS_INCOMING))
		{
			CFW_CcCallHoldMultiparty(1, CallInfo.idx);
		}
		//RejectCallRtn = CFW_CcRejectCall();
		//process MT
	}
	*/
	if(gSATContext.isInSATCall)
	{ 
		gSATContext.isInSATCall = 0;
	}

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	RejectCallRtn = CFW_CcRejectCall(g_pCCContext->sim_id);
#else
	RejectCallRtn = CFW_CcRejectCall(g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
	RejectCallRtn = CFW_CcRejectCall();
#endif /* __ADP_MULTI_SIM__ */
	
	mmi_trace(g_sw_ADP_CC,"MMI_Call:(WARN) Func: %s RejectCallRtn=0x%x", __FUNCTION__,RejectCallRtn);
	
	 
	if (ERR_SUCCESS != RejectCallRtn)
	{
		Adp_SendMessageToMMI((local_para_struct *)NULL, 
										MSG_ID_MMI_CC_ATH_RSP, ADP_CC_MSG_SRC_MOD);	
	}
	TBM_EXIT(0x29cb);
}
/**************************************************************
	FUNCTION NAME		: adp_CC_proc_GETCALLLIST_req
	PURPOSE			    : process MSG_ID_MMI_CC_GET_CALL_LIST_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_GETCALLLIST_req(COS_EVENT* pCosEvent)
{
	UINT8	CswCallInfoCnt=0 ;
	UINT8	i=0;
	UINT32 result = 0;
	CFW_CC_CURRENT_CALL_INFO	CswCallInfo[MAX_CALLS];
	
	mmi_cc_get_call_list_rsp_struct* pMmiGetCallListRsq;	

	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29cc);
	pMmiGetCallListRsq = (mmi_cc_get_call_list_rsp_struct*)OslMalloc(sizeof(mmi_cc_get_call_list_rsp_struct));
	
	ASSERT(pMmiGetCallListRsq != NULL);
	memset(pMmiGetCallListRsq, 0, sizeof(mmi_cc_get_call_list_rsp_struct));	
	memset(CswCallInfo, 0, MAX_CALLS * sizeof(CFW_CC_CURRENT_CALL_INFO));

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	result=UT_CFW_CcGetCurrentCall(CswCallInfo, &CswCallInfoCnt, g_pCCContext->sim_id);	
#else
	result=CFW_CcGetCurrentCall(CswCallInfo, &CswCallInfoCnt, g_pCCContext->sim_id);	
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
	result=CFW_CcGetCurrentCall(CswCallInfo, &CswCallInfoCnt);	
#endif /* __ADP_MULTI_SIM__ */
	
	mmi_trace(g_sw_ADP_CC, "MMI_Call:(WARN) adp_CC_proc_GETCALLLIST_req CswCallInfoCnt=%d  result= 0x%x", CswCallInfoCnt, result);
	
	pMmiGetCallListRsq->result = KAL_TRUE;
	pMmiGetCallListRsq->call_list.length = CswCallInfoCnt;

	if(ERR_SUCCESS==result)
	{
		for (i = 0; i < CswCallInfoCnt&&i<L4C_MAX_CALL_LIST_LEN; i++)
		{
			pMmiGetCallListRsq->call_list.list[i].call_id = CswCallInfo[i].idx;
			pMmiGetCallListRsq->call_list.list[i].call_state = CswCallInfo[i].status;
			pMmiGetCallListRsq->call_list.list[i].mpty = CswCallInfo[i].multiparty;
			SUL_GsmBcdToAscii(CswCallInfo[i].dialNumber.nTelNumber, 
								CswCallInfo[i].dialNumber.nSize, 
								pMmiGetCallListRsq->call_list.list[i].number);

			mmi_trace(g_sw_ADP_CC, "MMI_Call: CswCallInfo[%d].idx = %d", i, CswCallInfo[i].idx);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: CswCallInfo[%d].status = %d", i, CswCallInfo[i].status);
		}
	}
	Adp_SendMessageToMMI((local_para_struct *)pMmiGetCallListRsq, 
										MSG_ID_MMI_CC_GET_CALL_LIST_RSP, ADP_CC_MSG_SRC_MOD);
	TBM_EXIT(0x29cc);
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_STARTDTMF_req
	PURPOSE			    : process adp_CC_proc_STARTDTMF_req event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_STARTDTMF_req(COS_EVENT* pCosEvent)
{
	INT8		DigitValue;
	ilm_struct *oslMessage;
	mmi_cc_start_dtmf_req_struct* pMmiDtmfReq;
	UINT32 nRet = 0;
    
	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29cd);
	oslMessage = (ilm_struct *) pCosEvent->nParam1;
	pMmiDtmfReq = (mmi_cc_start_dtmf_req_struct*)oslMessage->local_para_ptr;
	DigitValue = pMmiDtmfReq->digit;
	
	if (((DigitValue >= '0') && (DigitValue <= '9'))
		|| (DigitValue == '#')
		|| (DigitValue == '*')
		|| ((DigitValue >= 'A') && (DigitValue <= 'D')))
	{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		nRet = UT_CFW_CcPlayTone(pMmiDtmfReq->digit,g_pCCContext->sim_id);
#else
		nRet = CFW_CcPlayTone(pMmiDtmfReq->digit,g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
		nRet = CFW_CcPlayTone(pMmiDtmfReq->digit);
#endif /* __ADP_MULTI_SIM__ */
	}
	else
	{
		mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) adp_CC_proc_STARTDTMF_req ????%c\n", pMmiDtmfReq->digit);
	}
    
	mmi_trace(g_sw_ADP_CC, "MMI_Call:func:%s nRet= %d", __FUNCTION__, nRet);

	TBM_EXIT(0x29cd);
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_ATA_req
	PURPOSE			    : process MSG_ID_MMI_CC_ATA_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_ATA_req(COS_EVENT* pCosEvent)
{
	UINT32 	AcceptCallRtn;

	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29ce);
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	AcceptCallRtn = UT_CFW_CcAcceptSpeechCall(g_pCCContext->sim_id);
#else
	AcceptCallRtn = CFW_CcAcceptSpeechCall(g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
	AcceptCallRtn = CFW_CcAcceptSpeechCall();	
#endif /* __ADP_MULTI_SIM__ */
	mmi_trace(g_sw_ADP_CC,"MMI_Call:(WARN) Func: %s AcceptCallRtn= 0x%x", __FUNCTION__,AcceptCallRtn);
	if (ERR_SUCCESS != AcceptCallRtn)
	{
		mmi_cc_ata_rsp_struct *pMmiAtaRsp = 
			(mmi_cc_ata_rsp_struct*)OslMalloc(sizeof(mmi_cc_ata_rsp_struct));

		ASSERT(pMmiAtaRsp != NULL);
		memset(pMmiAtaRsp,0,sizeof(mmi_cc_ata_rsp_struct));	
		pMmiAtaRsp->result.flag = L4C_ERROR;
		pMmiAtaRsp->result.cause = ERR_CSMCC_ILLEGAL_CALL_ID;

		Adp_SendMessageToMMI((local_para_struct *)pMmiAtaRsp, 
										MSG_ID_MMI_CC_ATA_RSP, ADP_CC_MSG_SRC_MOD);
	}
	TBM_EXIT(0x29ce);
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_UPDATEALS_req
	PURPOSE			    : process MSG_ID_MMI_CPHS_UPDATE_ALS_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_UPDATEALS_req(COS_EVENT* pCosEvent)
{
	
	mmi_cphs_update_als_rsp_struct* rsp_ptr; 

	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29cf);
	rsp_ptr = OslMalloc(sizeof(mmi_cphs_update_als_rsp_struct));
	ASSERT(rsp_ptr != NULL);
	
	memset(rsp_ptr, 0, sizeof(mmi_cphs_update_als_rsp_struct));  

	rsp_ptr->res = MMI_TRUE;

	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, 
									PRT_MMI_CPHS_UPDATE_ALS_RSP, ADP_CC_MSG_SRC_MOD);
	TBM_EXIT(0x29cf);
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_SETCLIRFLAG_req
	PURPOSE			    : process MSG_ID_MMI_SS_SET_CLIR_FLAG_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_SETCLIRFLAG_req(COS_EVENT* pCosEvent)
{			
	mmi_ss_set_clir_flag_rsp_struct *rsp_ptr;
	
	ASSERT(pCosEvent != NULL);

	rsp_ptr = OslMalloc(sizeof(mmi_ss_set_clir_flag_rsp_struct));

	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, sizeof(mmi_ss_set_clir_flag_rsp_struct));

	rsp_ptr->result = 1;

	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, 
									PRT_MMI_SS_SET_CLIR_FLAG_RSP, ADP_CC_MSG_SRC_MOD);
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_SPEECHCALL_ind
	PURPOSE			    : process EV_CFW_CC_CALL_INFO_IND event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_CALLINFO_ind(COS_EVENT* pCosEvent)
{
	UINT32 nParam1;
    	UINT32 nParam2;
    	UINT8   nType;

	ASSERT(pCosEvent != NULL);
	
	nParam1 = pCosEvent->nParam1;
    	nParam2 = pCosEvent->nParam2;
    	nType = HIUINT8(pCosEvent->nParam3);

	if ((nType == 0) && (nParam1 == 1))
	{//indicate sounder activity
	
		//send MSG_ID_MMI_CC_DIAL_IND
		mmi_cc_internal_alert_ind_struct *pMmiAlertInd = 
			(mmi_cc_internal_alert_ind_struct*)OslMalloc(sizeof(mmi_cc_internal_alert_ind_struct));
		
		ASSERT(pMmiAlertInd != NULL);
		memset(pMmiAlertInd, 0, sizeof(mmi_cc_internal_alert_ind_struct));
		
		pMmiAlertInd->alert= TRUE;

		Adp_SendMessageToMMI((local_para_struct *)pMmiAlertInd, 
										MSG_ID_MMI_CC_INTERNAL_ALERT_IND, ADP_CC_MSG_SRC_MOD);		
	}
	
#if 0
	if ((nType == 1) && (nParam1 == 0))
	{//release a call
	
		mmi_cc_call_release_ind_struct *pMmiReleaseIndRsp = 
				(mmi_cc_call_release_ind_struct*)OslMalloc(sizeof(mmi_cc_call_release_ind_struct));

		ASSERT(pMmiReleaseIndRsp != NULL);
		
		pMmiReleaseIndRsp->call_id = LOUINT8(nParam2);
		pMmiReleaseIndRsp->cause = 0;//need modify
		mmi_trace(g_sw_ADP_CC, "MMI_Call: adp_CC_proc_CALLINFO_ind Release Call: %d", pMmiReleaseIndRsp->call_id);
		
		#if CC_TEST_SWITCH
		{
			CFW_CC_CURRENT_CALL_INFO	CswCallInfo[MAX_CALLS];
			UINT8	CswCallInfoCnt;
#ifdef __ADP_MULTI_SIM__
			CFW_CcGetCurrentCall(CswCallInfo, &CswCallInfoCnt, g_pCCContext->sim_id);	
#else  /* No Defined __ADP_MULTI_SIM__ */
			CFW_CcGetCurrentCall(CswCallInfo, &CswCallInfoCnt);	
#endif /* __ADP_MULTI_SIM__ */
			mmi_trace(g_sw_ADP_CC, " &&&INFO_IND->CswCallInfoCnt=%d\n",CswCallInfoCnt);
		}
		#endif

		Adp_SendMessageToMMI((local_para_struct *)pMmiReleaseIndRsp, 
										MSG_ID_MMI_CC_CALL_RELEASE_IND, ADP_CC_MSG_SRC_MOD);
		
	}
#endif /* Guoj delete. It's not used now at 2010-1-8 */
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_SPEECHCALL_ind
	PURPOSE			    : process EV_CFW_CC_RELEASE_CALL_IND event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_RELEASECALL_ind(COS_EVENT* pCosEvent)
{
	UINT32 nParam1;
	UINT32 nParam2;
//	U16 cause;
	//UINT8   nType;
#ifdef MMI_ON_HARDWARE_P	
	#ifdef __MMI_SPEEDUP_RELEASE_CALL__
	if(nDummySendReleaseCallMsg > 0)
	{
		nDummySendReleaseCallMsg --;
        	mmi_trace(0, "MMI_Call: FUNC:%s nDummySendReleaseCallMsg %d", __FUNCTION__, nDummySendReleaseCallMsg);

		return;
	}
	#endif
	
	mmi_cc_call_release_ind_struct *pMmiReleaseIndRsp;
	TBM_ENTRY(0x29d0);	
	ASSERT(pCosEvent != NULL);

	nParam1 = pCosEvent->nParam1;
	nParam2 = pCosEvent->nParam2;
	//nType   = HIUINT8(pCosEvent->nParam3);
	pMmiReleaseIndRsp = (mmi_cc_call_release_ind_struct*)OslMalloc(sizeof(mmi_cc_call_release_ind_struct));

	ASSERT(pMmiReleaseIndRsp != NULL);
	memset(pMmiReleaseIndRsp, 0, sizeof(mmi_cc_call_release_ind_struct));
	
	#if CC_TEST_SWITCH
	{
		CFW_CC_CURRENT_CALL_INFO	CswCallInfo[MAX_CALLS];
		UINT8	CswCallInfoCnt=0;
		memset(CswCallInfo,0x0,MAX_CALLS*sizeof(CFW_CC_CURRENT_CALL_INFO) );
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		UT_CFW_CcGetCurrentCall(CswCallInfo, &CswCallInfoCnt, g_pCCContext->sim_id);	
#else
		CFW_CcGetCurrentCall(CswCallInfo, &CswCallInfoCnt, g_pCCContext->sim_id);	
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
		CFW_CcGetCurrentCall(CswCallInfo, &CswCallInfoCnt);	
#endif /* __ADP_MULTI_SIM__ */
		mmi_trace(g_sw_ADP_CC, "MMI_Call: &&&RELEASE_CALL_IND->CswCallInfoCnt=%d\n",CswCallInfoCnt);
	}
	#endif
	
	pMmiReleaseIndRsp->call_id = LOUINT8(nParam1);
	
	
	pMmiReleaseIndRsp->cause = nParam2;

	mmi_trace(g_sw_ADP_CC,"MMI_Call:(WARN) Func: %s pMmiReleaseIndRsp->cause=%d", __FUNCTION__,pMmiReleaseIndRsp->cause);
	Adp_SendMessageToMMI((local_para_struct *)pMmiReleaseIndRsp, 
										MSG_ID_MMI_CC_CALL_RELEASE_IND, ADP_CC_MSG_SRC_MOD);	
	
	if (g_AdpCcState != 0)
	{
		Adp_ActiveDialStateReleaseProcess(pCosEvent);
	}
	TBM_EXIT(0x29d0);
#endif
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_SPEECHCALL_ind
	PURPOSE			    : process EV_CFW_CC_SPEECH_CALL_IND event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
#define CALL_FORWARDING 0
static void adp_CC_proc_SPEECHCALL_ind(COS_EVENT* pCosEvent)
{
	UINT32 nParam1;
	//UINT32 nParam2;
	//UINT8   nType;
	CFW_CC_CURRENT_CALL_INFO	CswCallInfo;
	CFW_SPEECH_CALL_IND *pCswSpeechCallInfo;
	TBM_ENTRY(0x29d1);	
	ASSERT(pCosEvent != NULL);
	
	nParam1 = pCosEvent->nParam1;
	pCswSpeechCallInfo = (CFW_SPEECH_CALL_IND *)nParam1;
		
	if (Adp_GetCallInfo (&CswCallInfo, CFW_CM_STATUS_INCOMING))
	{				
		mmi_cc_call_ring_ind_struct *pMmiCallRingInfo = 
			(mmi_cc_call_ring_ind_struct*)OslMalloc(sizeof(mmi_cc_call_ring_ind_struct));


		ASSERT(pMmiCallRingInfo != NULL);
		memset(pMmiCallRingInfo, 0, sizeof(mmi_cc_call_ring_ind_struct));	



		pMmiCallRingInfo->call_id = CswCallInfo.idx;
		pMmiCallRingInfo->call_type = CswCallInfo.mode;

		mmi_trace(g_sw_ADP_CC, "MMI_Call: MSG_ID_MMI_CC_CALL_RING_IND &&&pMmiCallRingInfo->call_id=%d nCode=%d\n",pMmiCallRingInfo->call_id, pCswSpeechCallInfo->nCode);

		SUL_GsmBcdToAscii(pCswSpeechCallInfo->TelNumber.nTelNumber, 
							pCswSpeechCallInfo->TelNumber.nSize, 
							pMmiCallRingInfo->num.number);

		pMmiCallRingInfo->num.type = pCswSpeechCallInfo->TelNumber.nType;
		pMmiCallRingInfo->num.length = strlen(pMmiCallRingInfo->num.number);
		//auto answer process  ->0 is no auto answer
		pMmiCallRingInfo->auto_answer = 0;
		if(pCswSpeechCallInfo->nCode == CALL_FORWARDING)
		{
			pMmiCallRingInfo->mt_type = MT_TYPE_CALL_FORWARD;
		}
		else
		{
			pMmiCallRingInfo->mt_type = MT_TYPE_NONE;
		}
		
		Adp_SendMessageToMMI((local_para_struct *)pMmiCallRingInfo, 
										MSG_ID_MMI_CC_CALL_RING_IND, ADP_CC_MSG_SRC_MOD);	
	}
	else if (Adp_GetCallInfo (&CswCallInfo, CFW_CM_STATUS_WAITING))
	{
		mmi_cc_call_wait_ind_struct *pMmiCallWaitInfo = 
			(mmi_cc_call_wait_ind_struct*)OslMalloc(sizeof(mmi_cc_call_wait_ind_struct));

		ASSERT(pMmiCallWaitInfo != NULL);
		memset(pMmiCallWaitInfo, 0, sizeof(mmi_cc_call_wait_ind_struct));	

		mmi_trace(g_sw_ADP_CC, "MMI_Call: MSG_ID_MMI_CC_CALL_WAIT_IND");

		pMmiCallWaitInfo->call_id = CswCallInfo.idx;
		pMmiCallWaitInfo->call_mode = CswCallInfo.mode;
		
		SUL_GsmBcdToAscii(CswCallInfo.dialNumber.nTelNumber, 
			CswCallInfo.dialNumber.nSize, pMmiCallWaitInfo->num.number);
		
		pMmiCallWaitInfo->num.type = CswCallInfo.dialNumber.nType;
		pMmiCallWaitInfo->num.length = strlen(pMmiCallWaitInfo->num.number);

		Adp_SendMessageToMMI((local_para_struct *)pMmiCallWaitInfo, 
										MSG_ID_MMI_CC_CALL_WAIT_IND, ADP_CC_MSG_SRC_MOD);	
	}

	else
	{
//		ASSERT(0);
		mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) CSW ERROR!\n");
	}
	TBM_EXIT(0x29d1);
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_CRSSINFO_ind
	PURPOSE			    : process EV_CFW_CC_CRSSINFO_IND event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_ERROR_ind(COS_EVENT* pCosEvent)
{
	UINT32 nParam1 = 0;
	UINT32 nParam2 = 0;
	UINT8   nType  = 0;	
	mmi_cc_call_release_ind_struct *pMmiReleaseIndRsp;
		
	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29d2);
	nType   = HIUINT8(pCosEvent->nParam3);
	nParam2 = pCosEvent->nParam2;
	
	pMmiReleaseIndRsp = (mmi_cc_call_release_ind_struct*)OslMalloc(sizeof(mmi_cc_call_release_ind_struct));

	ASSERT(pMmiReleaseIndRsp != NULL);
	memset(pMmiReleaseIndRsp, 0, sizeof(mmi_cc_call_release_ind_struct));
	
	if ((0x10 == nType) && (CFW_CC_CAUSE_RECOVERY_ON_TIMER_EXPIRY == nParam2))
	{//timeout
		pMmiReleaseIndRsp->call_id = LOUINT8(nParam1);
	
		pMmiReleaseIndRsp->cause = (kal_uint8)CM_MM_CONN_TIMEOUT;
		mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) <<ERR_TIMEOUT_FAILURE\n");
	}
	if (0xF0 == nType)
	{
		pMmiReleaseIndRsp->call_id = HIUINT16(nParam2);
		pMmiReleaseIndRsp->cause = (kal_uint8)nParam1;
	}
	mmi_trace(1, "MMI_Call:(ERROR) <<adp_CC_proc_ERROR_ind EV_CFW_CC_ERROR_IND cause=%d\n", pMmiReleaseIndRsp->cause);
	Adp_SendMessageToMMI((local_para_struct *)pMmiReleaseIndRsp, 
										MSG_ID_MMI_CC_CALL_RELEASE_IND, ADP_CC_MSG_SRC_MOD);	
	TBM_EXIT(0x29d2);
	
}

/**************************************************************
	FUNCTION NAME		: adp_CC_proc_CRSSINFO_ind
	PURPOSE			    : process EV_CFW_CC_CRSSINFO_IND event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
static void adp_CC_proc_CRSSINFO_ind(COS_EVENT* pCosEvent)
{
	UINT32 nParam1;
	UINT32 nParam2;
	//UINT8  nType;
	BOOL   needToSendMsg = TRUE;
	kal_uint8	nSsType = 0;
	mmi_cc_notify_ss_ind_struct* pMmiSSNotifyInd = (mmi_cc_notify_ss_ind_struct*)NULL;
	
	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29d3);
	nParam1 = pCosEvent->nParam1;
	nParam2 = pCosEvent->nParam2;
	
	if (nParam1 != 0xFF)
	{
		switch (nParam1)//MO SS information
		{
			case 0:
				//unconditional call forwarding is active
				nSsType = CSMCC_NOTIFY_CFU;
				break;
				
			case 1:
				//some of the conditional call forwardings are active
				nSsType = CSMCC_NOTIFY_CFC;
				break;
				
			case 2:
				//call has been forwarded.
				nSsType = CSMCC_NOTIFY_CF;
				break;
				
			case 3:
				//The out call is in waiting state.
				nSsType = CSMCC_NOTIFY_CW;
				break;
				
			case 4:
				//this is a CUG call (also <index> present) 
				nSsType = CSMCC_NOTIFY_CUG;
				break;
				
			case 5:
				//outgoing calls are barred.
				nSsType = CSMCC_NOTIFY_BAOC;
				break;
				
			case 6:
				//incoming calls are barred.
				nSsType = CSMCC_NOTIFY_BAIC;
				break;
				
			case 7:
				//CLIR suppression rejected.
				nSsType = CSMCC_NOTIFY_CLIR_REJECT;
				break;
				
			case 8:
				//call has been deflected.
				nSsType = CSMCC_NOTIFY_CD;
				break;
			default:
				//The program can't go here,
				mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) adp_CC_proc_CRSSINFO_ind impossible execute nParam1");
				ASSERT(0);
				break;
		}
	}
	else
	{
		switch (nParam2)//MT SS information
		{
			case 0:
				//this is a forwarded call (MT call setup).
				nSsType = CSMCC_NOTIFY_CF;
				break;
				
			case 1:
				//this is a CUG call (also <index> present) (MT call setup).
				nSsType = CSMCC_NOTIFY_CUG;
				break;
				
			case 2:
				//call has been put on hold (during a voice call). 
				nSsType = CSMCC_NOTIFY_CALL_HELD;
				#ifdef __MMI_CONTROL_SWAP_CALL_RESPONSE__
				if(nWaitSwapCallResponse)
				{
					needToSendMsg = FALSE;
				}
				#endif
				break;
			case 3:
				//call has been retrieved (during a voice call).
				nSsType = CSMCC_NOTIFY_CALL_RETRIEVED;
				#ifdef __MMI_CONTROL_SWAP_CALL_RESPONSE__
				if(nWaitSwapCallResponse)
					needToSendMsg = FALSE;
				#endif
				break;
				
			case 4:
				//multiparty call entered (during a voice call).
				nSsType = CSMCC_NOTIFY_MPTY;
				break;
				
			case 5:
				//call on hold has been released (this is not a SS notification)
				//(during a voice call).
				nSsType = CSMCC_NOTIFY_HELD_CALL_RELEASED;
				break;
				
			case 6:
				//forward check SS message received (can be received whenever).
				needToSendMsg = FALSE;//CSD MMI has no this type.
				nSsType = 0;
				break;
				
			case 7:
				//call is being connected (alerting) with the remote party in alerting
				//state in explicit call transfer operation (during a voice call).
				nSsType = CSMCC_NOTIFY_ECT_ALERTING;
				break;
			case 8:
				//call has been connected with the other remote party in explicit call
				//transfer operation (also number and subaddress parameters may be present)
				//(during a voice call or MT call setup).
				needToSendMsg = FALSE;//CSD MMI has no this type.
				nSsType = 0;
				break;
				
			case 9:
				//this is a deflected call (MT call setup).
				nSsType = CSMCC_NOTIFY_CD;
				break;
				
			case 10:
				//additional incoming call forwarded.
				nSsType = CSMCC_NOTIFY_CF;
				break;
				
			default:
				//The program can't go here,
				//Include the value 0xFF.
				mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) adp_CC_proc_CRSSINFO_ind impossible execute nParam2");
				ASSERT(0);
				break;
		}
	}

	mmi_trace(g_sw_ADP_CC,"MMI_Call:(WARN) Func: %s needToSendMsg=%d", __FUNCTION__, needToSendMsg);
	if (needToSendMsg == TRUE)
	{
		pMmiSSNotifyInd = (mmi_cc_notify_ss_ind_struct*)OslMalloc(sizeof(mmi_cc_notify_ss_ind_struct));
		ASSERT(pMmiSSNotifyInd != NULL);
		memset(pMmiSSNotifyInd, 0, sizeof(mmi_cc_notify_ss_ind_struct));

		pMmiSSNotifyInd->type = nSsType;

		Adp_SendMessageToMMI((local_para_struct *)pMmiSSNotifyInd, 
										MSG_ID_MMI_CC_NOTIFY_SS_IND, ADP_CC_MSG_SRC_MOD);	
	}
	TBM_EXIT(0x29d3);

}

#if 	CC_TEST_SWITCH
//code for test
const char *const CC_REQText[]= //Request message text content
{
	"MSG_ID_MMI_CC_RESET_ACM_REQ",
	"MSG_ID_MMI_CC_RESET_ACM_RSP",
	"MSG_ID_MMI_CC_GET_ACM_REQ",
	"MSG_ID_MMI_CC_GET_ACM_RSP",
	"MSG_ID_MMI_CC_SET_MAX_ACM_REQ",
	"MSG_ID_MMI_CC_SET_MAX_ACM_RSP",
	"MSG_ID_MMI_CC_GET_MAX_ACM_REQ",
	"MSG_ID_MMI_CC_GET_MAX_ACM_RSP",
	"MSG_ID_MMI_CC_GET_CCM_REQ",
	"MSG_ID_MMI_CC_GET_CCM_RSP",
	"MSG_ID_MMI_CC_SET_CUG_REQ",
	"MSG_ID_MMI_CC_SET_CUG_RSP",
	"MSG_ID_MMI_CC_GET_CUG_REQ",
	"MSG_ID_MMI_CC_GET_CUG_RSP",
	"MSG_ID_MMI_CC_CHLD_REQ",
	"MSG_ID_MMI_CC_CHLD_RSP",
	"MSG_ID_MMI_CC_HANGUP_REQ",
	"MSG_ID_MMI_CC_HANGUP_RSP",
	"MSG_ID_MMI_CC_ATH_REQ",
	"MSG_ID_MMI_CC_ATH_RSP",
	"MSG_ID_MMI_CC_GET_CALL_LIST_REQ",
	"MSG_ID_MMI_CC_GET_CALL_LIST_RSP",
	"MSG_ID_MMI_CC_CALL_DEFLECT_REQ",
	"MSG_ID_MMI_CC_CALL_DEFLECT_RSP",
	"MSG_ID_MMI_CC_START_DTMF_REQ",
	"MSG_ID_MMI_CC_START_DTMF_RSP",
	"MSG_ID_MMI_CC_STOP_DTMF_REQ",
	"MSG_ID_MMI_CC_STOP_DTMF_RSP",
	"MSG_ID_MMI_CC_START_AUTO_DTMF_REQ",
	"MSG_ID_MMI_CC_START_AUTO_DTMF_RSP",
	"MSG_ID_MMI_CC_START_AUTO_DTMF_IND",
	"MSG_ID_MMI_CC_START_AUTO_DTMF_FINISH_IND",
	"MSG_ID_MMI_CC_ATA_REQ",
	"MSG_ID_MMI_CC_ATA_RSP",
	"MSG_ID_MMI_CC_DISCONNECT_AUX_REQ",
	"MSG_ID_MMI_CC_DISCONNECT_AUX_RSP",
	"MSG_ID_MMI_CC_SET_LINE_REQ",
	"MSG_ID_MMI_CC_SET_LINE_RSP",
	"MSG_ID_MMI_CC_DIAL_REQ",
	"MSG_ID_MMI_CC_DIAL_RSP",
	"MSG_ID_MMI_CC_DIAL_REQ_IND",
	"MSG_ID_MMI_CC_DIAL_IND",
	"MSG_ID_MMI_CC_CALL_MODIFY_REQ",
	"MSG_ID_MMI_CC_CALL_MODIFY_RSP",
	"MSG_ID_MMI_CC_SET_SINGLE_NUM_REQ",
	"MSG_ID_MMI_CC_SET_SINGLE_NUM_RSP",
	"MSG_ID_MMI_CC_GET_SINGLE_NUM_REQ",
	"MSG_ID_MMI_CC_GET_SINGLE_NUM_RSP",
	"MSG_ID_MMI_CC_CCM_IND",
	"MSG_ID_MMI_CC_INTERNAL_ALERT_IND",
	"MSG_ID_MMI_CC_CALL_WAIT_IND",
	"MSG_ID_MMI_CC_CALL_RING_IND",
	"MSG_ID_MMI_CC_CALL_CONNECT_IND",
	"MSG_ID_MMI_CC_CALL_DISCONNECT_IND",
	"MSG_ID_MMI_CC_CALL_RELEASE_IND",
	"MSG_ID_MMI_CC_SPEECH_IND",
	"MSG_ID_MMI_CC_NAME_IND",
	"MSG_ID_MMI_CC_NOTIFY_SS_IND",
	"MSG_ID_MMI_CC_NOTIFY_SS_CUG_IND",
	"MSG_ID_MMI_CC_NOTIFY_SS_ECT_IND",
	"MSG_ID_MMI_CC_NOTIFY_SS_CNAP_IND",
	"MSG_ID_MMI_CC_NOTIFY_SS_CCBS_IND",
	"MSG_ID_MMI_CC_CALL_ACCEPT_REQ_IND",
	"MSG_ID_MMI_CC_CALL_ABORT_REQ_IND",
	"MSG_ID_MMI_CC_CALL_SYNC_REQ_IND",
	"MSG_ID_MMI_CC_REL_COMP_REQ"
};

const char* const CC_RSPText[]=	//response message text content
{
	"EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP",
	"EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP",
	"EV_CFW_CC_INITIATE_SPEECH_CALL_RSP",
	"EV_CFW_CC_RELEASE_CALL_RSP",
	"EV_CFW_SS_QUERY_CLIP_RSP",
	"EV_CFW_SS_SET_CALL_WAITING_RSP",       
	"EV_CFW_SS_QUERY_CALL_WAITING_RSP",     
	"EV_CFW_SS_SET_CALL_FORWARDING_RSP",   
	"EV_CFW_SS_QUERY_CALL_FORWARDING_RSP",  
	"EV_CFW_SS_SET_FACILITY_LOCK_RSP",      
	"EV_CFW_SS_GET_FACILITY_LOCK_RSP",      
	"EV_CFW_SS_CHANGE_PWD_RSP",             
	"EV_CFW_SS_QUERY_FACILITY_LOCK_RSP",    
	"EV_CFW_SS_QUERY_CLIR_RSP",             
	"EV_CFW_SS_QUERY_COLP_RSP",             
	"EV_CFW_SS_SEND_USSD_RSP",              
	"EV_CFW_SS_TERMINATE_RSP",              
	"EV_CFW_SS_QUERY_COLR_RSP"
};

const char* const CC_INDText[]=  //indication message text content
{
	"EV_CFW_INIT_IND",
	"EV_CFW_SRV_STATUS_IND",
	"EV_CFW_NW_SIGNAL_QUALITY_IND",
	" ",
	"EV_CFW_NW_REG_STATUS_IND",
	"EV_CFW_CC_SPEECH_CALL_IND",
	"EV_CFW_CC_CRSSINFO_IND",
	"EV_CFW_NEW_SMS_IND",
	"EV_CFW_SMS_INFO_IND",
	"EV_CFW_CC_CALL_INFO_IND",
	"EV_CFW_SIM_STATUS_IND",
	"EV_CFW_SS_USSD_IND",
	"EV_CFW_CC_RELEASE_CALL_IND",
	"EV_CFW_CC_ERROR_IND",
	"EV_CFW_SAT_CMDTYPE_IND",
	"EV_CFW_PBK_IND",
	"EV_CFW_NW_NETWORKINFO_IND",
	"EV_CFW_GPRS_CXT_ACTIVE_IND",
	"EV_CFW_GPRS_CXT_DEACTIVE_IND",
	"EV_CFW_GPRS_MOD_IND",
	"EV_CFW_GPRS_STATUS_IND",
	"EV_CFW_GPRS_DATA_IND",
	"EV_CFW_XX_IND_END"
};

const char* const CallStateText[]=  //call state text content
{
	"CM_IDLE_STATE=0",
	"CM_OUTGOING_STATE",
	"CM_INCOMING_STATE",
	"CM_ACTIVE_STATE",
	"CM_HOLD_STATE",
	"CM_DISCONNECTING_STATE",
	"CM_STATE_UNKNOWN"
};

/**************************************************************
	FUNCTION NAME		: Test_PrintfGetEventText

  	PURPOSE			    : print event name 

	INPUT PARAMETERS	: msg_type 	EventId

	OUTPUT PARAMETERS	: NULL

	RETURNS				: void 

	REMARKS				: for test 

**************************************************************/
void Test_PrintfGetEventText(msg_type_t EventId)
{
	if (EventId >= MSG_ID_MMI_CC_RESET_ACM_REQ
		&& EventId <= MSG_ID_MMI_CC_REL_COMP_REQ)
	{
		mmi_trace(g_sw_ADP_CC, " >>>> %s \n", CC_REQText[EventId -MSG_ID_MMI_CC_RESET_ACM_REQ]);
	}
	
	else if (EventId == MSG_ID_MMI_SS_PARSING_STRING_REQ)
	{
		mmi_trace(g_sw_ADP_CC, " >>>> MSG_ID_MMI_SS_PARSING_STRING_REQ \n");
	}
	
	else if (EventId >= EV_CFW_INIT_IND
		&& EventId <= EV_CFW_XX_IND_END)
	{
		mmi_trace(g_sw_ADP_CC, " >>>> %s \n", CC_INDText[EventId -EV_CFW_INIT_IND]);
	}
	
	else if (EventId >= EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP 
		&& EventId <= EV_CFW_CC_RELEASE_CALL_RSP)//EV_CFW_CC_RSP_END_)
	{
		
		mmi_trace(g_sw_ADP_CC, " >>>> = %s \n", CC_RSPText[EventId -EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP]);
	}	
}

/**************************************************************
	FUNCTION NAME		: Test_PrintCallState

  	PURPOSE			    : print call state value

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void 

	REMARKS				: for test 

**************************************************************/
void Test_PrintCallState(void)
{
	CALL_STATE	CurrentState,PrevState;
	CurrentState = GetCurrentState();
	PrevState = GetPreviousState();
	
	mmi_trace(g_sw_ADP_CC, "????CurrentState = %s \n", CallStateText[CurrentState]);
	mmi_trace(g_sw_ADP_CC, "????PrevState = %s \n", CallStateText[PrevState]);
}
#if 0

/**************************************************************
	FUNCTION NAME		: Test_PrintReturnEventText

  	PURPOSE			    : print return event text content

	INPUT PARAMETERS	: msg_type 	EventId

	OUTPUT PARAMETERS	: NULL

	RETURNS				: void 

	REMARKS				: for test 

**************************************************************/
static void Test_PrintReturnEventText(msg_type_t  EventId)
{
	if (EventId >= MSG_ID_MMI_CC_RESET_ACM_REQ
		&& EventId <= MSG_ID_MMI_CC_REL_COMP_REQ)
	{
		mmi_trace(g_sw_ADP_CC, " <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
		mmi_trace(g_sw_ADP_CC, " <<<< %s  %d", CC_REQText[EventId -MSG_ID_MMI_CC_RESET_ACM_REQ],EventId);
		mmi_trace(g_sw_ADP_CC, " <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
	}

	else if (EventId == MSG_ID_MMI_SS_PARSING_STRING_RSP)
	{
		mmi_trace(g_sw_ADP_CC, " <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
		mmi_trace(g_sw_ADP_CC, " <<<< MSG_ID_MMI_SS_PARSING_STRING_RSP \n");
		mmi_trace(g_sw_ADP_CC, " <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
	}

	//Test_PrintCallState();
}
#endif /* Guoj delete. It's not used now */

#endif

/**************************************************************

	FUNCTION NAME		: ADP_DispatchCcMsg

  	PURPOSE				: Message Adaption for Call Control  

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: NULL

	RETURNS				: TRUE->message be processed
							  FALSE->message not processed 
	REMARKS				: 

**************************************************************/
static BOOL ADP_DispatchCcReqMsg(COS_EVENT* pCosEvent);
static BOOL ADP_DispatchCcRspMsg(COS_EVENT* pCosEvent);
static BOOL ADP_DispatchCcIndMsg(COS_EVENT* pCosEvent);
BOOL ADP_DispatchCcMsg(COS_EVENT* pCosEvent)
{
	//BOOL	EventProcessed = FALSE;
	ASSERT(pCosEvent != NULL);

	
	
	if (ADP_DispatchCcReqMsg(pCosEvent))
		return TRUE;
	
	if (ADP_DispatchCcRspMsg(pCosEvent))
		return TRUE;
	
	if (ADP_DispatchCcIndMsg(pCosEvent))
		return TRUE;
	
	return FALSE;

/*
	#if 1
	//Test_PrintfGetEventText(pCosEvent->nEventId);
	//CheckAndPrintMsgId(pCosEvent->nEventId);
	#endif
	
	if (pCosEvent->nEventId >= MSG_ID_MMI_CC_RESET_ACM_REQ
		&& pCosEvent->nEventId <= MSG_ID_MMI_CC_REL_COMP_REQ)
	{
		EventProcessed = ADP_DispatchCcReqMsg(pCosEvent);
	}
	
	else if (pCosEvent->nEventId == MSG_ID_MMI_SS_PARSING_STRING_REQ)
	{
		EventProcessed = ADP_DispatchCcReqMsg(pCosEvent);
	}

	else if (pCosEvent->nEventId >= EV_CFW_IND_BASE
		&& pCosEvent->nEventId <= EV_CFW_IND_END_)
	{
		EventProcessed = ADP_DispatchCcIndMsg(pCosEvent);
	}
	
	else if (pCosEvent->nEventId >= EV_CFW_CC_RSP_BASE 
		&& pCosEvent->nEventId <= EV_CFW_CC_RSP_END_)
	{
		
		EventProcessed = ADP_DispatchCcRspMsg(pCosEvent);
	}
	
	return	EventProcessed;*/
	
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchCcIndMsg 

  	PURPOSE				: Message Adaption for Call Control  IND

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: NULL

	RETURNS				: TRUE->message be processed
							  FALSE->message not processed
	REMARKS				: 

**************************************************************/
//#if defined(__MMI_BT_HP_SUPPORT__)|| defined(__MMI_BT_SUPPORT__)
static void adp_CC_proc_SPEECHON_ind(COS_EVENT* pCosEvent)
{
	UINT32 nParam1;
  mmi_cc_speech_ind_struct *pMmiCallWaitInfo = NULL;

	TBM_ENTRY(0x29d1);	
	ASSERT(pCosEvent != NULL);
	
	pMmiCallWaitInfo = (mmi_cc_speech_ind_struct*)OslMalloc(sizeof(mmi_cc_speech_ind_struct));

	ASSERT(pMmiCallWaitInfo != NULL);
	memset(pMmiCallWaitInfo, 0, sizeof(mmi_cc_speech_ind_struct));	
		
	nParam1 = pCosEvent->nParam1;
	
	 if (nParam1 != 0)
	 {
		pMmiCallWaitInfo->on_off = 1;
	 }
	 else
	 {
		pMmiCallWaitInfo->on_off = 0;
	 }
	 
	Adp_SendMessageToMMI((local_para_struct *)pMmiCallWaitInfo, 
										MSG_ID_MMI_CC_SPEECH_IND, ADP_CC_MSG_SRC_MOD);	
	 
	TBM_EXIT(0x29d1);
}
//#endif
static BOOL ADP_DispatchCcIndMsg(COS_EVENT* pCosEvent)
{
	UINT32 nParam1;
	UINT32 nParam2;
	UINT8 nType;	

	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29d4);
	nParam1 = pCosEvent->nParam1;
	nParam2 = pCosEvent->nParam2;
	nType = HIUINT8(pCosEvent->nParam3);	

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	adp_ConfigureContext(ADP_MODULE_CC, nCCSimId);
#else  /* No Defined ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
	adp_ConfigureContext(ADP_MODULE_CC, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#endif
	
	switch (pCosEvent->nEventId)
	{
//#if defined(__MMI_BT_HP_SUPPORT__) || defined(__MMI_BT_SUPPORT__)
		case EV_DM_SPEECH_IND:
		{
			mmi_trace(g_sw_ADP_CC, "MMI_Call: MSG >>>>EV_DM_SPEECH_IND");
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>ntype = %x", nType);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam1 = %x", nParam1);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam2 = %x", nParam2);
			adp_CC_proc_SPEECHON_ind(pCosEvent);
		}
			break;
//#endif			
		case  EV_CFW_CC_SPEECH_CALL_IND:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>EV_CFW_CC_SPEECH_CALL_IND");
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>ntype = %x", nType);
			mmi_trace(g_sw_ADP_CC, "MMI_Call:>>>>nParam1 = %x", nParam1);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam2 = %x", nParam2);
			adp_CC_proc_SPEECHCALL_ind(pCosEvent);
		}
			break;

		case	EV_CFW_CC_CRSSINFO_IND:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>EV_CFW_CC_CRSSINFO_IND");
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>ntype = %x", nType);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam1 = %x", nParam1);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam2 = %x", nParam2);

			adp_CC_proc_CRSSINFO_ind(pCosEvent);
		}
			break;
			
		case      EV_CFW_CC_RELEASE_CALL_IND:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>EV_CFW_CC_RELEASE_CALL_IND");
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>ntype = %x", nType);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam1 = %x", nParam1);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam2 = %x", nParam2);
			adp_CC_proc_RELEASECALL_ind(pCosEvent);
		}
			break;
			
		case	EV_CFW_CC_CALL_INFO_IND:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call:>>>>EV_CFW_CC_CALL_INFO_IND");
			mmi_trace(g_sw_ADP_CC, "MMI_Call:>>>>ntype = %x", nType);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam1 = %x", nParam1);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam2 = %x", nParam2);
			adp_CC_proc_CALLINFO_ind(pCosEvent);
		}
			break;

		case	EV_CFW_CC_ERROR_IND:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call:(ERROR) Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) >>>>EV_CFW_CC_ERROR_IND");
			mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) >>>>ntype = %x", nType);
			mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) >>>>nParam1 = %x", nParam1);
			mmi_trace(g_sw_ADP_CC, "MMI_Call:(ERROR) >>>>nParam2 = %x", nParam2);
			adp_CC_proc_ERROR_ind(pCosEvent);			
		}
			break;
			
		default:
		{
			TBM_EXIT(0x29d4);
			return FALSE;
		}
	}
	TBM_EXIT(0x29d4);
	return TRUE; 
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchCcReqMsg 

  	PURPOSE				: Message Adaption for Call Control  REQ

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: NULL

	RETURNS				: TRUE->message be processed
							  FALSE->message not processed
	REMARKS				: 

**************************************************************/
static BOOL ADP_DispatchCcReqMsg(COS_EVENT* pCosEvent)
{
	ilm_struct *oslMessage;
	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29d5);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_CC, adp_GetMMISimID(ADP_GET_MMI_DEST_MOD(pCosEvent->nParam3)));
#endif

	switch  (pCosEvent->nEventId)
	{
		case MSG_ID_MMI_CC_CALL_DEFLECT_REQ:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>MSG_ID_MMI_CC_CALL_DEFLECT_REQ");
			adp_CC_proc_DEFLECT_req(pCosEvent);
		}
			break;
			
		case MSG_ID_MMI_CC_ATA_REQ://accept a call req
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>MSG_ID_MMI_CC_ATA_REQ");
			adp_CC_proc_ATA_req(pCosEvent);
		}
			break;

		case MSG_ID_MMI_CC_START_DTMF_REQ:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>MSG_ID_MMI_CC_START_DTMF_REQ");

			adp_CC_proc_STARTDTMF_req(pCosEvent);	
		}
			break;
			
		case MSG_ID_MMI_CC_GET_CALL_LIST_REQ:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>MSG_ID_MMI_CC_GET_CALL_LIST_REQ");
			adp_CC_proc_GETCALLLIST_req(pCosEvent);			
		}
			break;
			
		case  MSG_ID_MMI_CC_ATH_REQ://reject a call req
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>MSG_ID_MMI_CC_ATH_REQ");
			adp_CC_proc_ATH_req(pCosEvent);	
		}
			break;
			
		case MSG_ID_MMI_CC_CHLD_REQ:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>MSG_ID_MMI_CC_CHLD_REQ");

			adp_CC_proc_CHLD_req(pCosEvent);
		}
			break;
			
		case  MSG_ID_MMI_SS_PARSING_STRING_REQ:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>MSG_ID_MMI_SS_PARSING_STRING_REQ");
			adp_CC_proc_PARSINGSTRING_req(pCosEvent);		
		}
			break;
		
		case MSG_ID_MMI_CPHS_UPDATE_ALS_REQ:               //add by yaosq for LineSwitch
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>MSG_ID_MMI_CPHS_UPDATE_ALS_REQ");
			adp_CC_proc_UPDATEALS_req(pCosEvent);		
		}
			break;
			
		case MSG_ID_MMI_SS_SET_CLIR_FLAG_REQ:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: MSG_ID_MMI_SS_SET_CLIR_FLAG_REQ SIM:%d",g_pCCContext->sim_id);
			#endif
			adp_CC_proc_SETCLIRFLAG_req(pCosEvent);
		}
			break;
		case MSG_ID_MMI_CC_START_AUTO_DTMF_REQ://man
		{
			int i;
			mmi_cc_start_auto_dtmf_req_struct *pMmiDTMFMsg;
			ASSERT(pCosEvent != NULL);
			mmi_trace(g_sw_ADP_CC, "MMI_Call:  MSG >>>>MSG_ID_MMI_CC_START_AUTO_DTMF_REQ");
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			oslMessage = (ilm_struct *) pCosEvent->nParam1;
			pMmiDTMFMsg = (mmi_cc_start_auto_dtmf_req_struct*)oslMessage->local_para_ptr;
			for( i = 0;i < strlen(pMmiDTMFMsg->digit);i++ )//man call
			{
				mmi_trace(1, "MMI_Call: CFW_CcPlayTone=%c",pMmiDTMFMsg->digit[i]);//man
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				UT_CFW_CcPlayTone(pMmiDTMFMsg->digit[i], g_pCCContext->sim_id);
#else
				CFW_CcPlayTone(pMmiDTMFMsg->digit[i], g_pCCContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
				CFW_CcPlayTone(pMmiDTMFMsg->digit[i]);
#endif /* __ADP_MULTI_SIM__ */
            }
		}
			
		default:
		{
			TBM_EXIT(0x29d5);
			return FALSE;
		}
	}
	TBM_EXIT(0x29d5);
	return TRUE; 
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchCcRspMsg 

  	PURPOSE				: Message Adaption for Call Control  RSP

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: NULL

	RETURNS				: TRUE->message be processed
							  FALSE->message not processed
	REMARKS				: 

**************************************************************/
static BOOL ADP_DispatchCcRspMsg(COS_EVENT* pCosEvent)
{
	UINT32 nParam1;
	UINT32 nParam2;
	UINT8   nType;	

	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29d6);
	nParam1 = pCosEvent->nParam1;
	nParam2 = pCosEvent->nParam2;
	nType = HIUINT8(pCosEvent->nParam3);	

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	adp_ConfigureContext(ADP_MODULE_CC, nCCSimId);
#else  /* No Defined ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
	adp_ConfigureContext(ADP_MODULE_CC, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#endif

	switch (pCosEvent->nEventId)
	{
		case  EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP:
		{//accept a call
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP");
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>ntype = %x", nType);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam1 = %x", nParam1);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam2 = %x", nParam2);
			adp_CC_proc_ACCEPTSPEECHCALL_rsp(pCosEvent);
		}
			break;
			
		case  EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP:
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP");
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>ntype = %x", nType);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam1 = %x", nParam1);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam2 = %x", nParam2);
			#ifdef __MMI_CONTROL_SWAP_CALL_RESPONSE__
			if(nWaitSwapCallResponse)
				nWaitSwapCallResponse --;
			if(!nWaitSwapCallResponse)
			#endif
			{
				adp_CC_proc_CALLHOLDMULTIPARTY_rsp(pCosEvent);
			}
		}
			break;
			
		case  EV_CFW_CC_RELEASE_CALL_RSP:
		{//end a call
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: MSG >>>>EV_CFW_CC_RELEASE_CALL_RSP");
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>ntype = %x", nType);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam1 = %x", nParam1);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam2 = %x", nParam2);
			adp_CC_proc_RELEASECALL_rsp(pCosEvent);

		}
			break;
			
		case 	EV_CFW_CC_INITIATE_SPEECH_CALL_RSP:
		{//initiate a call 
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(TRUE,"MMI_Call: Func: %s SIM:%d", __FUNCTION__,g_pCCContext->sim_id);
			#endif
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>EV_CFW_CC_INITIATE_SPEECH_CALL_RSP");
			mmi_trace(g_sw_ADP_CC, "MMI_Call:>>>>ntype = %x", nType);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam1 = %x", nParam1);
			mmi_trace(g_sw_ADP_CC, "MMI_Call: >>>>nParam2 = %x", nParam2);
			adp_CC_proc_INITATESPEECHCALL_rsp(pCosEvent);
			
		}
			break;
			
		default:
		{
			TBM_EXIT(0x29d6);	
			return FALSE;
		}
	}
	TBM_EXIT(0x29d6);
	return TRUE; 
}

