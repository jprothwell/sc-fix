/******************************************************************************/
/*              Copyright (C) 2006, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    adp_SS_Events.c                                               */
/*                                                                            */
/* Description:                                                               */
/*     This file is Adaption layer between CSD MMI and Coolsand CSW Platform  */
/* of Supplementary Service module                                            */
/*     Change CSD MMI message of Supplementary Service module to coolsand     */
/* message, also at reverse routine.                                          */
/*                                                                            */
/*                Author: Zhanggh             Date: 2006-07-06.               */
/******************************************************************************/






/**************************************************************
 * Include Files
**************************************************************/
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
//dyj add end

#include "ctype.h" //wufasong added 2006.11.23 for update csw lib to csw v1.01.20
//yaosq
#include "mmi_trace.h"
#include "unit_test.h"
#include "gsm7bitdefaultalphabet.h"
#ifdef MMI_ON_WIN32 /* zhuoxz,2009-4-27 */
#include "cswtype.h"
#include "mmi_data_types.h"
#endif

/**************************************************************
 * Macros
**************************************************************/
#define MAX_PARMS 5

#define CHR_STAR ('*')
#define CHR_HASH ('#')

#define MAXPASSWORDLEN 4

#define MAX_BUSY_TIMEOUT 30   /* Maximum no answer time for call forwarding */
#define MAX_BUSY_STEP 5    /* Step value for call forwarding timeouts */

#define MAX_USS_DATA_LEN 229

//#define MAX_CC_ADDR_LEN 41   /* see GSM4.80 */

#define CW_DISABLE 0
#define CW_ENABLE 1

#define PARSE_STRING_SUCESS 1
#define PARSE_STRING_FAILED 0

#ifdef __ADP_MULTI_SIM__
ADP_SS_CONTEXT_T g_SSContext[4] =
{
	{ADP_SIM_1,0, },
	{ADP_SIM_2,0, },
	{ADP_SIM_3,0, },
	{ADP_SIM_4,0, },
};

ADP_SS_CONTEXT_T *g_pSSContext = &g_SSContext[0];
#define ADP_SS_MSG_SRC_MOD  adp_SimId2IlmMod(g_pSSContext->sim_id)

void AdpInitAnotherSIM();

#else
ADP_SS_CONTEXT_T g_SSContext = 
{
	0,
};
ADP_SS_CONTEXT_T *g_pSSContext = &g_SSContext;


#define ADP_SS_MSG_SRC_MOD MOD_L4C
#endif


/**************************************************************
 * Type Definitions
**************************************************************/
typedef enum
{
	SSMMI_STAR = 0,
	SSMMI_HASH,
	SSMMI_STARHASH,
	SSMMI_STARSTAR,
	SSMMI_HASHHASH,
	SSMMI_USS
}
SSOpType;

typedef struct
{
	SSOpType type;
	UINT8 parmStart[MAX_PARMS]; /* start position in user string */
	UINT8 parmLen[MAX_PARMS];
}
SSParsedParameters;

typedef struct
{
	UINT8 *pzMmiStr;
	ss_code_enum SSCode;
}
MmiCodeToSsCode;

typedef enum { //                           Basic Service group number
	Adp_ALL_BS_AND_TS               = 0 ,   // 1-12
	//         TELESERVICES
	Adp_ALL_TELESERVICES            = 10, // 1-6 + 12 
	Adp_TELEPHONY                   = 11, // 1
	Adp_ALL_DATA_TELESERVICES       = 12, // 2-6
	Adp_FACSIMILE_SERVICES          = 13, // 6
	Adp_SHORT_MESSAGE_SERVICES      = 16, // 2
	Adp_ALL_TELESERVICES_EXCEPT_SMS = 19, // 1 + 3-6 + 12
	//         VOICE GROUP SERVICES
	Adp_VOICE_GROUP_CALL_SERVICE    = 17,
	Adp_VOICE_BROADCAST_SERVICE     = 18,
	//         BEARER SERVICE
	Adp_ALL_BEARER_SERVICES         = 20, // 7-11
	Adp_ALL_ASYNC_SERVICES          = 21, // 7
	Adp_ALL_SYNC_SERVICES           = 22, // 8
	Adp_ALL_DATA_CIRCUIT_SYNC       = 24, // 8
	Adp_ALL_DATA_CIRCUIT_ASYNC      = 25, // 7
	Adp_ALL_GPRS_BEARER_SERVICES    = 29  // 13
} Adp_bs_code_enum;

typedef struct
{
	U8 ussd_input[MAX_DIGITS_USSD];
	BOOL b_used;
}
USSDSTRING;
/**************************************************************
 * Manifest Constants
**************************************************************/
UINT8 ssPswdServiceCode[] = {'0', '3', 0};
UINT8 ssClipCode[] = {'3', '0', 0};
UINT8 ssClirCode[] = {'3', '1', 0};
UINT8 ssColpCode[] = {'7', '6', 0};
UINT8 ssColrCode[] = {'7', '7', 0};
UINT8 ssCnapCode[] = {'3', '0', '0', 0};
UINT8 ssCfuCode[] = {'2', '1', 0};
UINT8 ssCfbCode[] = {'6', '7', 0};
UINT8 ssCfnryCode[] = {'6', '1', 0};
UINT8 ssCfnrcCode[] = {'6', '2', 0};
UINT8 ssAllForwardingCode[] = {'0', '0', '2', 0};
UINT8 ssAllCondForwardingCode[] = {'0', '0', '4', 0};
UINT8 ssCwCode[] = {'4', '3', 0};
UINT8 ssBaocCode[] = {'3', '3', 0};
UINT8 ssBoicCode[] = {'3', '3', '1', 0};
UINT8 ssBoicExhcCode[] = {'3', '3', '2', 0};
UINT8 ssBaicCode[] = {'3', '5', 0};
UINT8 ssBicroamCode[] = {'3', '5', '1', 0};
UINT8 ssAllCallRestrictionCode[] = {'3', '3', '0', 0};
UINT8 ssBarringOfOutgoingCode[] = {'3', '3', '3', 0};
UINT8 ssBarringOfIncomingCode[] = {'3', '5', '3', 0};

const MmiCodeToSsCode ssMmiCodeToSsCode[]=
{
	{ssClipCode, SS_CLIP},
	{ssClirCode, SS_CLIR},
	{ssColpCode, SS_COLP},
	{ssColrCode, SS_COLR},
	{ssCnapCode, SS_CNAP},
	{ssCfuCode, SS_CFU},
	{ssCfbCode, SS_CFB},
	{ssCfnryCode, SS_CFNRY},
	{ssCfnrcCode, SS_CFNRC},
	{ssAllForwardingCode, SS_ALLFORWARDINGSS},
	{ssAllCondForwardingCode, SS_ALLCONDFORWARDINGSS},
	{ssCwCode, SS_CW},
	{ssBaocCode, SS_BAOC},
	{ssBoicCode, SS_BOIC},
	{ssBoicExhcCode, SS_BOICEXHC},
	{ssBaicCode, SS_BAIC},
	{ssBicroamCode, SS_BICROAM},
	{ssAllCallRestrictionCode, SS_ALLBARRINGSS},
	//{ssBarringOfOutgoingCode, SS_BARRINGOFOUTGOINGCALLS},
	{ssBarringOfIncomingCode, SS_BARRINGOFINCOMINGCALLS},
	{(UINT8 *)NULL, (ss_code_enum)0}
};



/**************************************************************
 * Local Variables
**************************************************************/
static BOOL gCwBsCodePresent = FALSE;
static Adp_bs_code_enum gCwBsCode = 0;
static BOOL CallForwardingFlag = FALSE;
UINT8 gSSCode = 0;
UINT8 gSSUTI = 27;
USSDSTRING ussdstring;

/**************************************************************
 * Local Functions Prototypes
**************************************************************/
static BOOL ADP_DispatchSsReqMsg(COS_EVENT *pCosEvent);

static BOOL ADP_DispatchSsRspMsg(COS_EVENT *pCosEvent);
static BOOL ADP_DispatchSsIndMsg(COS_EVENT *pCosEvent);

static kal_uint8 ParseUserInput(UINT8 *pStr, UINT8 len, SSParsedParameters *pParsed);

static void GetTypeOfOperation(UINT8 *pStr, SSParsedParameters *pParsedParameters, l4_op_code_enum *pOpCode, ss_code_enum *pSSCode);

static void SS_Adaption_SendParseStringRSP(kal_uint8 resultFlag);

static void AdaptProcUssAbortReq(void);
static void AdaptSendUSSDREQ(COS_EVENT* pCosEvent);
void AdpSetCallForwardingState( UINT8 reason, UINT8 mode);
UINT8 AdpGetCallForwardingState( );
U8 SS_ReSendProcUssV1Req();

/**************************************************************
 * Local Functions definitions
**************************************************************/
#if 0
/**************************************************************
	FUNCTION NAME		: AdaptParsingStringREQ
	PURPOSE				: Parse MSG_ID_MMI_SS_PARSING_STRING_REQ signal,
					and then invoke corresponding function.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL AdaptParsingStringREQ(COS_EVENT* pCosEvent)
{
	ilm_struct *ilm_ptr = NULL;
	mmi_ss_parsing_string_req_struct *pReq = NULL;

	ASSERT(pCosEvent != NULL);
	ASSERT(pCosEvent->nEventId == MSG_ID_MMI_SS_PARSING_STRING_REQ);

	ilm_ptr = (ilm_struct *)pCosEvent->nParam1;
	pReq = (mmi_ss_parsing_string_req_struct *)ilm_ptr->local_para_ptr;

	if (IsSSString(pReq->input, pReq->length) || IsCRSSString(pReq->input, pReq->length))
	{
		SS_Adaption_ParseStringREQ(pReq);
	}

	/*Not complete yet*/
}
#endif

/**************************************************************
	FUNCTION NAME		: AdaptCallWaitingRSP
	PURPOSE				: Process call waiting RSP received form CSW.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL AdaptCallWaitingRSP(COS_EVENT *pCosEvent)
{
	mmi_ss_call_wait_rsp_struct *rsp_ptr = NULL;
	UINT8 nType = HIUINT8(pCosEvent->nParam3);

	ASSERT(pCosEvent != NULL);

	rsp_ptr = OslMalloc(sizeof(mmi_ss_call_wait_rsp_struct));
	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, sizeof(mmi_ss_call_wait_rsp_struct));

	if (nType == 0)
	{
		rsp_ptr->result.flag = L4C_OK;

		if (pCosEvent->nEventId == EV_CFW_SS_SET_CALL_WAITING_RSP)
		{
			UINT32 nMode1 = pCosEvent->nParam1;

			rsp_ptr->status = (nMode1 == 1) ? SS_ABIT : SS_QBIT;
			rsp_ptr->count = 1;
			rsp_ptr->list[0] = gCwBsCode;
		}
		else
		{
			UINT32 nMode2 = pCosEvent->nParam1;/*enable or disable BITS*/
			UINT32 nSet = pCosEvent->nParam2;/*indicate or not indicate BITS*/

			rsp_ptr->status = SS_QBIT;

			if (CFW_IS_BIT_SET(nSet, 0))
			{
				if (CFW_IS_BIT_SET(nMode2, 0))
				{
					rsp_ptr->status = SS_ABIT;
					rsp_ptr->list[rsp_ptr->count] = Adp_TELEPHONY;
					rsp_ptr->count++;
				}
			}
			if (CFW_IS_BIT_SET(nSet, 1))
			{
				if (CFW_IS_BIT_SET(nMode2, 1))
				{
					rsp_ptr->status = SS_ABIT;
					rsp_ptr->list[rsp_ptr->count] = Adp_ALL_DATA_TELESERVICES;
					rsp_ptr->count++;
				}
			}
			if (CFW_IS_BIT_SET(nSet, 2))
			{
				if (CFW_IS_BIT_SET(nMode2, 2))
				{
					rsp_ptr->status = SS_ABIT;
					rsp_ptr->list[rsp_ptr->count] = Adp_FACSIMILE_SERVICES;
					rsp_ptr->count++;
				}
			}
			if (CFW_IS_BIT_SET(nSet, 3))
			{
				if (CFW_IS_BIT_SET(nMode2, 3))
				{
					rsp_ptr->status = SS_ABIT;
					rsp_ptr->list[rsp_ptr->count] = Adp_SHORT_MESSAGE_SERVICES;
					rsp_ptr->count++;
				}
			}
		}
	}
	else
	{
		rsp_ptr->result.flag = L4C_ERROR;
		rsp_ptr->result.cause = CM_UNASSIGNED_NUM;//need further development.
	}
	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_WAIT_RSP, ADP_SS_MSG_SRC_MOD);

	return TRUE;
}

/**************************************************************
	FUNCTION NAME		: AdaptSetCallForwardingRSP
	PURPOSE				: Process EV_CFW_SS_SET_CALL_FORWARDING_RSP
				signal received form CSW.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL AdaptSetCallForwardingRSP(COS_EVENT *pCosEvent)
{
	mmi_ss_call_forward_rsp_struct *rsp_ptr = NULL;
	UINT8 nType = HIUINT8(pCosEvent->nParam3);
	CFW_SS_SET_CALLFORWARDING_RSP_INFO *pCswRsp = NULL;
	UINT8 AsciiNumber[MAX_CC_ADDR_LEN] = {0};
	mmi_cphs_display_cfu_ind_struct *ind_ptr;

	ind_ptr = OslMalloc(sizeof(mmi_cphs_display_cfu_ind_struct));
	ASSERT(ind_ptr != NULL);
	memset(ind_ptr, 0, sizeof(mmi_cphs_display_cfu_ind_struct));
	TBM_ENTRY(0x29fc);
	ASSERT(pCosEvent != NULL);

	rsp_ptr = OslMalloc(sizeof(mmi_ss_call_forward_rsp_struct));
	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, sizeof(mmi_ss_call_forward_rsp_struct));

	if (nType == 0)
	{
		pCswRsp = (CFW_SS_SET_CALLFORWARDING_RSP_INFO *)pCosEvent->nParam1;
		rsp_ptr->result.flag = L4C_OK;
		rsp_ptr->count = 1;
    rsp_ptr->list[0].bs_code = pCswRsp->nClass;
		rsp_ptr->list[0].no_reply_timer = pCswRsp->nTime;
		switch (pCswRsp->nMode)

		{
			case CFW_SS_FORWARDING_MODE_DISABLE:
				rsp_ptr->list[0].ss_status = SS_QBIT;
				break;
				
			case CFW_SS_FORWARDING_MODE_ENABLE:
				rsp_ptr->list[0].ss_status = SS_ABIT;
				break;
				
			case CFW_SS_FORWARDING_MODE_REGISTRATION:
				rsp_ptr->list[0].ss_status = SS_ABIT;
				break;
				
			case CFW_SS_FORWARDING_MODE_ERASURE:
				rsp_ptr->list[0].ss_status = SS_QBIT;
				break;
				
			default:
				mmi_trace(g_sw_ADP_SS, "AdaptSetCallForwardingRSP Error pCswRsp->mode,%d", pCswRsp->nMode);
				break;
		}

		mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----pCswRsp->number.nType=%d", __FUNCTION__, __FILE__, __LINE__,pCswRsp->nNumber.nType);
		if (pCswRsp->nNumber.nDialNumberSize != 0)                          //add by yaosq 060822
		{
			SUL_GsmBcdToAscii(pCswRsp->nNumber.pDialNumber, pCswRsp->nNumber.nDialNumberSize, AsciiNumber);
			if(pCswRsp->nNumber.nType == 0x91) //International number
			{
				rsp_ptr->list[0].address[0] = '+';
				strncpy(rsp_ptr->list[0].address+1, AsciiNumber, MAX_CC_ADDR_LEN-1);
				rsp_ptr->list[0].address[MAX_CC_ADDR_LEN-1] = 0;	
			}
			else
			{
				strncpy(rsp_ptr->list[0].address, AsciiNumber, MAX_CC_ADDR_LEN);
				rsp_ptr->list[0].address[MAX_CC_ADDR_LEN-1] = 0;
			}
		}
		switch (pCswRsp->nReason)

		{
			case CFW_SS_FORWARDING_REASON_UNCONDITIONAL:
				rsp_ptr->type = L4_CFU;
				break;
				
			case CFW_SS_FORWARDING_REASON_MOBILEBUSY:
				rsp_ptr->type = L4_CFB;
				break;
				
			case CFW_SS_FORWARDING_REASON_NOREPLY:
				rsp_ptr->type = L4_CFNRY;
				break;
				
			case CFW_SS_FORWARDING_REASON_NOTREACHABLE:
				rsp_ptr->type = L4_CFNRC;
				break;
				
			case CFW_SS_FORWARDING_REASON_ALL_CALLFORWARDING:
				rsp_ptr->type = L4_CFA;
				break;
				
			case CFW_SS_FORWARDING_REASON_ALL_CONDCALLFORWARDING:
				rsp_ptr->type = L4_CFC;
				break;
				
			default:
				mmi_trace(g_sw_ADP_SS, "AdaptSetCallForwardingRSP Error pCswRsp->nReason,%d", pCswRsp->nReason);
				break;
		}

		/*Guojian Add Start For Call Forward Ver: GreenStone  on 2008-11-2 14:0 */
		if(pCswRsp->nReason != 0xFF)
		{
			AdpSetCallForwardingState(pCswRsp->nReason,pCswRsp->nMode);
		}
		/*Guojian Add End  For Call Forward Ver: GreenStone  on 2008-11-2 14:0 */


	}
	else
	{
		rsp_ptr->result.flag = L4C_ERROR;
		rsp_ptr->result.cause = CM_UNASSIGNED_NUM;//need further development.
	}

	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_FORWARD_RSP, ADP_SS_MSG_SRC_MOD);

	/*Guojian Add Start For Call Forward Ver: GreenStone  on 2008-11-2 14:4 */
	ind_ptr->action = AdpGetCallForwardingState();
	Adp_SendMessageToMMI((local_para_struct *)ind_ptr, PRT_MSG_ID_MMI_CPHS_DISPLAY_CFU_IND, ADP_SS_MSG_SRC_MOD); //send the display IND
	/*Guojian Add End  For Call Forward Ver: GreenStone  on 2008-11-2 14:4 */

	TBM_EXIT(0x29fc);
	//OslMfree(pCswRsp->number.pDialNumber);
	//pCswRsp->number.pDialNumber = NULL;

	return TRUE;
}
void AdpSetCallForwardingState( UINT8 reason, UINT8 mode)
{
	if((CFW_SS_FORWARDING_MODE_DISABLE == mode)||(CFW_SS_FORWARDING_MODE_ERASURE == mode))
	{
		switch (reason)
		{
			case CFW_SS_FORWARDING_REASON_UNCONDITIONAL:
				g_pSSContext->forwarding_state &= 0xfe;  //
				break;
				
			case CFW_SS_FORWARDING_REASON_MOBILEBUSY:
				g_pSSContext->forwarding_state &= 0xfd;
				break;
				
			case CFW_SS_FORWARDING_REASON_NOREPLY:
				g_pSSContext->forwarding_state &= 0xfb;
				break;
				
			case CFW_SS_FORWARDING_REASON_NOTREACHABLE:
				g_pSSContext->forwarding_state &= 0xf7;
				break;				
			case CFW_SS_FORWARDING_REASON_ALL_CALLFORWARDING:
				g_pSSContext->forwarding_state &= 0xf0;
				break;
				
			case CFW_SS_FORWARDING_REASON_ALL_CONDCALLFORWARDING:
				g_pSSContext->forwarding_state &= 0xf1;
				break;
				
			default:
				mmi_trace(g_sw_ADP_SS, "AdpSetCallForwardingState CFW_SS_FORWARDING_MODE_DISABLE failure!!reason!!");
				break;
		}
	}
	else if(CFW_SS_FORWARDING_MODE_ENABLE == mode || CFW_SS_FORWARDING_MODE_REGISTRATION == mode)
	{
		switch (reason)
		{
			case CFW_SS_FORWARDING_REASON_UNCONDITIONAL:
				g_pSSContext->forwarding_state |= 0x01;
				break;
				
			case CFW_SS_FORWARDING_REASON_MOBILEBUSY:
				g_pSSContext->forwarding_state |= 0x02;
				break;
				
			case CFW_SS_FORWARDING_REASON_NOREPLY:
				g_pSSContext->forwarding_state |= 0x04;
				break;
				
			case CFW_SS_FORWARDING_REASON_NOTREACHABLE:
				g_pSSContext->forwarding_state |= 0x08;
				break;
				
			case CFW_SS_FORWARDING_REASON_ALL_CALLFORWARDING:  //All call forwarding reasons(includes reasons 0, 1, 2 and 3)
				g_pSSContext->forwarding_state |= 0x0F;
				break;
				
			case CFW_SS_FORWARDING_REASON_ALL_CONDCALLFORWARDING:
				g_pSSContext->forwarding_state |= 0x0E;  //All conditional call forwarding reasons(includes reasons 1, 2 and 3)
				break;
			//cancel query all forwarding state	
			case 0xf0:
				g_pSSContext->forwarding_state &= 0xbf;  // cancel query flag
				break;
			//set query all forwarding state
			case 0xff:
				g_pSSContext->forwarding_state &= 0x00;
				g_pSSContext->forwarding_state = 0x40;   //set query start flag
				break;
				
			default:
				mmi_trace(g_sw_ADP_SS, "AdpSetCallForwardingState CFW_SS_FORWARDING_MODE_ENABLE failure!!reason!!");
				break;
		}
	}
	
	mmi_trace(g_sw_ADP_SS, "AdpSetCallForwardingState reason: %d, mode: %d, g_pSSContext->forwarding_state: %x\n",reason,mode,g_pSSContext->forwarding_state);

	return ;		
}
UINT8 AdpGetCallForwardingState()
{
	mmi_trace(g_sw_ADP_SS, "AdpGetCallForwardingState  g_pSSContext->forwarding_state: %x\n",g_pSSContext->forwarding_state);
	//0x3f if mean's: 0011 1111
//	if(g_pSSContext->forwarding_state&0x3f)
	if(g_pSSContext->forwarding_state&0x01) //only unconditional call forward will show icon
	{
		return 1;
	}
	return 0;
}
void AdpQueryAllCallFwowardingStart(UINT8 nReason)
{
	UINT32 ret =0;
	//set query all forwarding state
	if ( CFW_SS_FORWARDING_REASON_UNCONDITIONAL == nReason)  //start
	{
		AdpSetCallForwardingState(0xff,1);
	}
	
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	ret = UT_CFW_SsQueryCallForwarding(nReason, Adp_TELEPHONY, FREE_UTI, g_pSSContext->sim_id);
#else
	ret = CFW_SsQueryCallForwarding(nReason, Adp_TELEPHONY, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
	ret = CFW_SsQueryCallForwarding(nReason, Adp_TELEPHONY, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
#ifdef __ADP_MULTI_SIM__
	mmi_trace(g_sw_ADP_SS, "AdpQueryAllCallFwowardingStart() nReason: %d  ret = 0x%x sim_id: %d", nReason, ret, g_pSSContext->sim_id);
#endif
	if (ret != ERR_SUCCESS)
	{
		AdpSetCallForwardingState(0xf0,1);//cancel query all forwarding state
#ifdef __ADP_MULTI_SIM__
		AdpInitAnotherSIM();	
#endif /* __ADP_MULTI_SIM__ */
	}
	
	return ;
}

#ifdef __ADP_MULTI_SIM__
BOOL bCFiniting = FALSE;
#endif
BOOL bNeedInit[ADP_SIM_NUMBER] = {FALSE,};

#ifdef __ADP_MULTI_SIM__
void AdpInitCallFwowardingStart(UINT8 nSimId)
#else  /* No Defined __ADP_MULTI_SIM__ */
void AdpInitCallFwowardingStart()
#endif /* __ADP_MULTI_SIM__ */
{
#ifdef __ADP_MULTI_SIM__
	mmi_trace(g_sw_ADP_SS," Func: %s nSimId: %d, bCFiniting: %d", __FUNCTION__, nSimId, bCFiniting);
	
	if (bCFiniting)
	{
		bNeedInit[nSimId] = TRUE;
		return;
	}
	else
	{
		bCFiniting = TRUE;
		adp_ConfigureContext(ADP_MODULE_SS, nSimId);
	}
#endif
	AdpQueryAllCallFwowardingStart(CFW_SS_FORWARDING_REASON_UNCONDITIONAL);//add used to query all call forwarding

}


#ifdef __ADP_MULTI_SIM__
void AdpInitAnotherSIM()
{
	U8 i;
		// init another sim
	bCFiniting = FALSE;
	
	bNeedInit[g_pSSContext->sim_id] = FALSE;
	
	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		if(i != g_pSSContext->sim_id && bNeedInit[i])
		{
			mmi_trace(g_sw_ADP_SS, "SS AdpInitAnotherSIM for SIM%d", i);
			adp_ConfigureContext(ADP_MODULE_SS, i);
			bCFiniting = TRUE;
			AdpQueryAllCallFwowardingStart(CFW_SS_FORWARDING_REASON_UNCONDITIONAL);//add used to query all call forwarding
			break;
		}
	}
}
#endif /* __ADP_MULTI_SIM__ */
BOOL AdaptQueryAllCallForwardingRSP(COS_EVENT *pCosEvent)
{
	UINT8 nType = HIUINT8(pCosEvent->nParam3);
	CFW_SS_QUERY_CALLFORWARDING_RSP *pCswRsp = NULL; //yaosq 060725

	mmi_cphs_display_cfu_ind_struct *rsp_ptr;

	ASSERT(pCosEvent != NULL);
	rsp_ptr = OslMalloc(sizeof(mmi_cphs_display_cfu_ind_struct));
	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, sizeof(mmi_cphs_display_cfu_ind_struct));

	if (nType == 0)      
	{
		pCswRsp = (CFW_SS_QUERY_CALLFORWARDING_RSP *)pCosEvent->nParam1;
		if(pCswRsp->nStatus == 1)
		{	
			AdpSetCallForwardingState(pCswRsp->nReason,CFW_SS_FORWARDING_MODE_ENABLE); //actived
		}
		else
		{
			AdpSetCallForwardingState(pCswRsp->nReason,CFW_SS_FORWARDING_MODE_DISABLE); //deactived
		}
		switch (pCswRsp->nReason)
		{
			case CFW_SS_FORWARDING_REASON_UNCONDITIONAL:
#if 0  //only query unconditional call forward when boot up
				AdpQueryAllCallFwowardingStart(1);//query Forward if the mobile phone is busy
				break;
				
			case CFW_SS_FORWARDING_REASON_MOBILEBUSY:
				AdpQueryAllCallFwowardingStart(2);//query Forward if there is no answer
				break;
				
			case CFW_SS_FORWARDING_REASON_NOREPLY:
				AdpQueryAllCallFwowardingStart(3);//query Forward if the number is unreachable
				break;
				
			case CFW_SS_FORWARDING_REASON_NOTREACHABLE:
#endif /* Guoj delete. It's not used now at 2009-10-21 */
				AdpSetCallForwardingState(0xf0,1);//cancel query all forwarding state
				rsp_ptr->action = AdpGetCallForwardingState();
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MSG_ID_MMI_CPHS_DISPLAY_CFU_IND, ADP_SS_MSG_SRC_MOD);
			#ifdef __ADP_MULTI_SIM__
				StartTimer(ADP_SS_CALL_FORWARD_QUERY_TIMER, 2*1000, AdpInitAnotherSIM); //set timer to ensure the previous MM is released completely
				//AdpInitAnotherSIM();
			#endif /* __ADP_MULTI_SIM__ */
				break;
				
			default:
				mmi_trace(g_sw_ADP_SS, "AdaptQueryAllCallForwardingRSP Error pCswRsp->reason,%d", pCswRsp->nReason);
				break;
		}
	}
	else
	{
		AdpSetCallForwardingState(0xf0,1);//cancel query all forwarding state
		rsp_ptr->action = AdpGetCallForwardingState();
		Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MSG_ID_MMI_CPHS_DISPLAY_CFU_IND, ADP_SS_MSG_SRC_MOD);

#ifdef __ADP_MULTI_SIM__
		AdpInitAnotherSIM();
#endif /* __ADP_MULTI_SIM__ */

		mmi_trace(g_sw_ADP_SS, "AdaptQueryAllCallForwardingRSP nType error!!! nType:0x%x",nType);
	}

	return TRUE;
}
/**************************************************************
	FUNCTION NAME		: AdaptQueryCallForwardingRSP
	PURPOSE				: Process EV_CFW_SS_QUERY_CALL_FORWARDING_RSP
				signal received form CSW.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL AdaptQueryCallForwardingRSP(COS_EVENT *pCosEvent)
{
	mmi_ss_call_forward_rsp_struct *rsp_ptr = NULL;
	UINT8 nType = HIUINT8(pCosEvent->nParam3);
	CFW_SS_QUERY_CALLFORWARDING_RSP *pCswRsp = NULL; //yaosq 060725
	UINT8 AsciiNumber[MAX_CC_ADDR_LEN] = {0};

	mmi_cphs_display_cfu_ind_struct *ind_ptr;

	ind_ptr = OslMalloc(sizeof(mmi_cphs_display_cfu_ind_struct));
	ASSERT(ind_ptr != NULL);
	memset(ind_ptr, 0, sizeof(mmi_cphs_display_cfu_ind_struct));

	ASSERT(pCosEvent != NULL);
	ASSERT(pCosEvent->nEventId == EV_CFW_SS_QUERY_CALL_FORWARDING_RSP);
	TBM_ENTRY(0x29fd);
	rsp_ptr = OslMalloc(sizeof(mmi_ss_call_forward_rsp_struct));
	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, sizeof(mmi_ss_call_forward_rsp_struct));

	if (nType == 0)       //yaosq   060725
	{
		pCswRsp = (CFW_SS_QUERY_CALLFORWARDING_RSP *)pCosEvent->nParam1;
		rsp_ptr->result.flag = L4C_OK;
		rsp_ptr->count = 1;
    rsp_ptr->list[0].bs_code = pCswRsp->nClass;
		rsp_ptr->list[0].no_reply_timer = pCswRsp->nTime;
		if (pCswRsp->nStatus == 0)


		{
			rsp_ptr->list[0].ss_status = SS_QBIT;
		}
		else
		{
			rsp_ptr->list[0].ss_status = SS_ABIT;
		}
		SUL_GsmBcdToAscii(pCswRsp->nNumber.nTelNumber, pCswRsp->nNumber.nSize, AsciiNumber);

	
		if(pCswRsp->nNumber.nType == 0x91) //International number
		{
			rsp_ptr->list[0].address[0] = '+';
			strncpy(rsp_ptr->list[0].address+1, AsciiNumber, MAX_CC_ADDR_LEN-1);
			rsp_ptr->list[0].address[MAX_CC_ADDR_LEN-1] = 0;	
		}
		else
		{
			strncpy(rsp_ptr->list[0].address, AsciiNumber, MAX_CC_ADDR_LEN);
			rsp_ptr->list[0].address[MAX_CC_ADDR_LEN-1] = 0;
		}		
		switch (pCswRsp->nReason)

		{
			case CFW_SS_FORWARDING_REASON_UNCONDITIONAL:
				rsp_ptr->type = L4_CFU;
				break;
				
			case CFW_SS_FORWARDING_REASON_MOBILEBUSY:
				rsp_ptr->type = L4_CFB;
				break;
				
			case CFW_SS_FORWARDING_REASON_NOREPLY:
				rsp_ptr->type = L4_CFNRY;
				break;
				
			case CFW_SS_FORWARDING_REASON_NOTREACHABLE:
				rsp_ptr->type = L4_CFNRC;
				break;
				
			case CFW_SS_FORWARDING_REASON_ALL_CALLFORWARDING:
				rsp_ptr->type = L4_CFA;
				break;
				
			case CFW_SS_FORWARDING_REASON_ALL_CONDCALLFORWARDING:
				rsp_ptr->type = L4_CFC;
				break;
				
			default:
				mmi_trace(g_sw_ADP_SS, "AdaptQueryCallForwardingRSP Error pCswRsp->reason,%d", pCswRsp->nReason);
				break;
		}
	}
	else
	{
		rsp_ptr->result.flag = L4C_ERROR;
		rsp_ptr->result.cause = CM_UNASSIGNED_NUM;//need further development.
	}
	
	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_FORWARD_RSP, ADP_SS_MSG_SRC_MOD);
    
	if (nType == 0)
	{   
		/*Guojian Add Start For Call Forward Ver: GreenStone  on 2008-11-2 14:3 */
		if(pCswRsp->nStatus == 1)
		{	
			AdpSetCallForwardingState(pCswRsp->nReason,CFW_SS_FORWARDING_MODE_ENABLE); //actived
		}
		else
		{
			AdpSetCallForwardingState(pCswRsp->nReason,CFW_SS_FORWARDING_MODE_DISABLE); //deactived
		}
		ind_ptr->action = AdpGetCallForwardingState();
		Adp_SendMessageToMMI((local_para_struct *)ind_ptr, PRT_MSG_ID_MMI_CPHS_DISPLAY_CFU_IND, ADP_SS_MSG_SRC_MOD);
		/*Guojian Add End  For Call Forward Ver: GreenStone  on 2008-11-2 14:3 */
	}
        
	TBM_EXIT(0x29fd);

	return TRUE;
}

/**************************************************************
	FUNCTION NAME		: AdaptCallBarringRSP
	PURPOSE				: Process EV_CFW_SS_QUERY_FACILITY_LOCK_RSP and
	    EV_CFW_SS_SET_FACILITY_LOCK_RSP	signals received form CSW.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL AdaptCallBarringRSP(COS_EVENT *pCosEvent)
{
	mmi_ss_call_barring_rsp_struct *rsp_ptr = NULL;
	UINT8 nType = HIUINT8(pCosEvent->nParam3);
	UINT16 nFac; //= HIUINT16(pCosEvent->nParam1);
	//UINT8 nMode = LOUINT16(pCosEvent->nParam1);
	UINT8 nClass = HIUINT8((UINT16)pCosEvent->nParam2);
	UINT8 nStatus = LOUINT8((UINT16)pCosEvent->nParam2);

	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29fe);
	if (pCosEvent->nEventId == EV_CFW_SS_SET_FACILITY_LOCK_RSP)
	{
		nFac = HIUINT16(pCosEvent->nParam1);
	}
	else
	{
		nFac = pCosEvent->nParam1;
	}
	
	rsp_ptr = OslMalloc(sizeof(mmi_ss_call_barring_rsp_struct));
	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, sizeof(mmi_ss_call_barring_rsp_struct));

	if (CFW_IS_BIT_SET(nClass, 0))
	{
		if (CFW_IS_BIT_SET(nStatus, 0))
		{
			rsp_ptr->list[rsp_ptr->count].bs_code = Adp_TELEPHONY;
			rsp_ptr->list[rsp_ptr->count].ss_status = SS_ABIT;
			rsp_ptr->count++;
		}
	}
	if (CFW_IS_BIT_SET(nClass, 1))
	{
		if (CFW_IS_BIT_SET(nStatus, 1))
		{
			rsp_ptr->list[rsp_ptr->count].bs_code = Adp_ALL_DATA_TELESERVICES;
			rsp_ptr->list[rsp_ptr->count].ss_status = SS_ABIT;
			rsp_ptr->count++;
		}
	}
	if (CFW_IS_BIT_SET(nClass, 2))
	{
		if (CFW_IS_BIT_SET(nStatus, 2))
		{
			rsp_ptr->list[rsp_ptr->count].bs_code = Adp_FACSIMILE_SERVICES;
			rsp_ptr->list[rsp_ptr->count].ss_status = SS_ABIT;
			rsp_ptr->count++;
		}
	}
	if (CFW_IS_BIT_SET(nClass, 3))
	{
		if (CFW_IS_BIT_SET(nStatus, 3))
		{
			rsp_ptr->list[rsp_ptr->count].bs_code = Adp_SHORT_MESSAGE_SERVICES;
			rsp_ptr->list[rsp_ptr->count].ss_status = SS_ABIT;
			rsp_ptr->count++;
		}
	}
	mmi_trace(g_sw_ADP_SS, "AdaptCallBarringRSP  nfac %X,%d,%x",nFac,nFac,pCosEvent->nParam1);
	switch (nFac)
	{
		case CFW_STY_FAC_TYPE_AO:
			rsp_ptr->type = L4_BAOC;
			mmi_trace(g_sw_ADP_SS, "AdaptCallBarringRSP  CFW_STY_FAC_TYPE_AO");
			break;
			
		case CFW_STY_FAC_TYPE_OI:
			rsp_ptr->type = L4_BOIC;
			mmi_trace(g_sw_ADP_SS, "AdaptCallBarringRSP  CFW_STY_FAC_TYPE_OI");
			break;
			
		case CFW_STY_FAC_TYPE_OX:
			rsp_ptr->type = L4_BOICEXHC;
			mmi_trace(g_sw_ADP_SS, "AdaptCallBarringRSP  CFW_STY_FAC_TYPE_OX");
			break;
			
		case CFW_STY_FAC_TYPE_AI:
			rsp_ptr->type = L4_BAIC;
			mmi_trace(g_sw_ADP_SS, "AdaptCallBarringRSP  CFW_STY_FAC_TYPE_AI");
			break;
			
		case CFW_STY_FAC_TYPE_IR:
			rsp_ptr->type = L4_BAICROAM;
			mmi_trace(g_sw_ADP_SS, "AdaptCallBarringRSP  CFW_STY_FAC_TYPE_IR");
			break;
			
		case CFW_STY_FAC_TYPE_AB:
			rsp_ptr->type = L4_BAC;
			mmi_trace(g_sw_ADP_SS, "AdaptCallBarringRSP  CFW_STY_FAC_TYPE_AB");
			break;
			
		case CFW_STY_FAC_TYPE_AG:
			rsp_ptr->type = L4_BOC;
			mmi_trace(g_sw_ADP_SS, "AdaptCallBarringRSP  CFW_STY_FAC_TYPE_AG");
			break;
			
		case CFW_STY_FAC_TYPE_AC:
			rsp_ptr->type = L4_BIC;
			mmi_trace(g_sw_ADP_SS, "AdaptCallBarringRSP  CFW_STY_FAC_TYPE_AC");
			break;
			
		default:
			mmi_trace(g_sw_ADP_SS, "AdaptCallBarringRSP Error nFac, %d", nFac);
			rsp_ptr->type = L4_BAOC;
			break;
	}

	if (nType == 0)
	{
		rsp_ptr->result.flag = L4C_OK;
	}
	else
	{
		rsp_ptr->result.flag = L4C_ERROR;
		if (nType == 0xFA)									//add by yaosq 2007.04.06对不同的错误需要提供不同的提示信息
		rsp_ptr->result.cause = CM_UNASSIGNED_NUM;//need further development.
		else
			rsp_ptr->result.cause = CM_SS_ERR_NEGATIVEPW_CHECK;
	}

	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_BARRING_RSP, ADP_SS_MSG_SRC_MOD);
	TBM_EXIT(0x29fe);

	return TRUE;
}

/**************************************************************
	FUNCTION NAME		: AdaptChangePasswordRSP
	PURPOSE				: Process EV_CFW_SS_CHANGE_PWD_RSP
				signal received form CSW.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL AdaptChangePasswordRSP(COS_EVENT *pCosEvent)
{
	mmi_ss_call_barring_rsp_struct *rsp_ptr = NULL;
	UINT8 nType = HIUINT8(pCosEvent->nParam3);

	ASSERT(pCosEvent != NULL);

	rsp_ptr = OslMalloc(sizeof(mmi_ss_call_barring_rsp_struct));
	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, sizeof(mmi_ss_call_barring_rsp_struct));

	mmi_trace(g_sw_ADP_SS, "AdaptChangePasswordRSP nType = %d", nType);

	if (nType == 0)
	{
		rsp_ptr->result.flag = L4C_OK;
	}
	else
	{
		rsp_ptr->result.flag = L4C_ERROR;
		rsp_ptr->result.cause = CM_SS_ERR_NEGATIVEPW_CHECK;//need further development.
	}

	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CHANGE_PASSWORD_RSP, ADP_SS_MSG_SRC_MOD);

	return TRUE;
}

/**************************************************************
	FUNCTION NAME		: SS_InterrogateClipRSP 
	PURPOSE				: Process EV_CFW_SS_QUERY_CLIP_RSP
				signal received form CSW.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL SS_InterrogateClipRSP(COS_EVENT* pCosEvent)
{
	mmi_ss_clip_interrogate_ind_struct *rsp_ptr = NULL;
	UINT8 nType = HIUINT8(pCosEvent->nParam3);
	
	ASSERT(pCosEvent != NULL);
	
	rsp_ptr = OslMalloc(sizeof(mmi_ss_clip_interrogate_ind_struct));
	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, sizeof(mmi_ss_clip_interrogate_ind_struct));

	if (nType == 0)
	{
		rsp_ptr->result.flag = L4C_OK;
		switch (pCosEvent->nParam2)
		{
			case 0:
				rsp_ptr->status = SS_QBIT;
				break;
				
			case 1:
				switch (pCosEvent->nParam1)
				{
					case 0:
						rsp_ptr->status = SS_QBIT;
						break;
						
					case 1:
						rsp_ptr->status = SS_PBIT;
						break;
						
					default:
						//The program can't go here. CSW do not give this value.
						ASSERT(0);
						break;
				}
				break;
				
			case 2:
				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause =  CM_UNASSIGNED_NUM;//need further development.
				break;
				
			default:
				//The program can't go here. CSW do not give this value.
				ASSERT(0);
				break;
		}
	}
	else
	{
		rsp_ptr->result.flag = L4C_ERROR;
		rsp_ptr->result.cause =  CM_UNASSIGNED_NUM; //need further development.
	}

	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CLIP_INTERROGATE_IND, ADP_SS_MSG_SRC_MOD);

	return TRUE;
}

/**************************************************************
	FUNCTION NAME		: SS_InterrogateClirRSP 
	PURPOSE				: Process EV_CFW_SS_QUERY_CLIR_RSP
				signal received form CSW.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL SS_InterrogateClirRSP(COS_EVENT * pCosEvent)
{
	mmi_ss_clir_interrogate_ind_struct *rsp_ptr = NULL;
	UINT8 nType = HIUINT8(pCosEvent->nParam3);

	ASSERT(pCosEvent != NULL);

	rsp_ptr = OslMalloc(SIZEOF(mmi_ss_clir_interrogate_ind_struct));
	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, SIZEOF(mmi_ss_clir_interrogate_ind_struct));

	if (nType == 0)
	{
		rsp_ptr->result.flag  = L4C_OK;
		switch (pCosEvent->nParam2)
		{
			case 0:                                        //CLIR not provisioned
				rsp_ptr->status = SS_QBIT;
				break;
				
			case 1:                                       // CLIR provisioned in permanent mode
				rsp_ptr->status = SS_PBIT;
				switch (pCosEvent->nParam1)
				{
					case 0:
						rsp_ptr->clir_option = 3;
						break;
						
					case 1:
						rsp_ptr->clir_option = 0;
						break;
						
					case 2:
						rsp_ptr->clir_option = 3;
						break;
						
					default:
						//The program can't go here. CSW do not give this value.
						ASSERT(0);
						break;
				}
				break;
				
			case 2:                                       //unknown (e.g. no network, etc.)
				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM; //need further development.
				break;
				
			case 3:                                      //temporary mode presentation restricted
				rsp_ptr->status = SS_PBIT;
				rsp_ptr->clir_option =1;
				break;
				
			case 4:                                      //CLIR temporary mode presentation allowed
				rsp_ptr->status = SS_PBIT;
				rsp_ptr->clir_option = 2;
				break;
				
			default:
				//The program can't go here. CSW do not give this value.
				ASSERT(0);
				break;
				
		}
			
	}
	else
	{
		rsp_ptr->result.flag = L4C_ERROR;
		rsp_ptr->result.cause = CM_UNASSIGNED_NUM; //need further development.
	}

	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CLIR_INTERROGATE_IND, ADP_SS_MSG_SRC_MOD);

	return TRUE;
}


/**************************************************************
	FUNCTION NAME		: SS_InterrogateColpRSP 
	PURPOSE				: Process EV_CFW_SS_QUERY_COLP_RSP 
				signal received form CSW.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL SS_InterrogateColpRSP(COS_EVENT * pCosEvent)
{
	mmi_ss_colp_interrogate_ind_struct *rsp_ptr = NULL;
	UINT8 nType = HIUINT8(pCosEvent->nParam3);

	ASSERT(pCosEvent != NULL);

	rsp_ptr = OslMalloc(SIZEOF(mmi_ss_colp_interrogate_ind_struct));
	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, SIZEOF(mmi_ss_colp_interrogate_ind_struct));

	if (nType == 0)
	{
		rsp_ptr->result.flag = L4C_OK;
		switch (pCosEvent->nParam2)
		{
			case 0:
				rsp_ptr->status = SS_QBIT;
				break;
				
			case 1:
				switch (pCosEvent->nParam1)
				{
					case 0:
						rsp_ptr->status = SS_RBIT;
						break;
						
					case 1:
						rsp_ptr->status = SS_PBIT;
						break;
						
					default:
						//The program can't go here. CSW do not give this value.
						ASSERT(0);
						break;
				}
				break;
				
			case 2:
				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM; //need further development.
				break;
				
			default:
				//The program can't go here. CSW do not give this value.
				ASSERT(0);
				break;
		}
	}
	else
	{
		rsp_ptr->result.flag = L4C_ERROR;
		rsp_ptr->result.cause = CM_UNASSIGNED_NUM; //need further development.
	}

	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_COLP_INTERROGATE_IND, ADP_SS_MSG_SRC_MOD);
	
	return TRUE;
}

/**************************************************************
	FUNCTION NAME		: SS_InterrogateColrRSP 
	PURPOSE				: Process EV_CFW_SS_QUERY_COLR_RSP  
				signal received form CSW.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL SS_InterrogateColrRSP(COS_EVENT * pCosEvent)
{
	mmi_ss_colr_interrogate_ind_struct *rsp_ptr = NULL;
	UINT8 nType = HIUINT8(pCosEvent->nParam3);

	ASSERT(pCosEvent != NULL);

	rsp_ptr = OslMalloc(SIZEOF(mmi_ss_colr_interrogate_ind_struct));
	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, sizeof(mmi_ss_colr_interrogate_ind_struct));

	if (nType == 0)
	{
		rsp_ptr->result.flag = L4C_OK;
		switch (pCosEvent->nParam2)
		{
			case 0:
				rsp_ptr->status = SS_QBIT;
				break;
				
			case 1:
				switch (pCosEvent->nParam1)
				{
					case 0:
						rsp_ptr->status = SS_RBIT;
						break;
						
					case 1:
						rsp_ptr->status = SS_PBIT;
						break;
						
					default:
						//The program can't go here. CSW do not give this value.
						ASSERT(0);
						break;
				}
				break;
				
			case 2:
				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM; //need further development.
				break;
				
			default:
				//The program can't go here. CSW do not give this value.
				ASSERT(0);
				break;
		}
	}
	else
	{
		rsp_ptr->result.flag = L4C_ERROR;
		rsp_ptr->result.cause = CM_UNASSIGNED_NUM; //need further development.
	}

	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_COLR_INTERROGATE_IND, ADP_SS_MSG_SRC_MOD);

	return TRUE;
}

/**************************************************************
	FUNCTION NAME		: SS_SENDUssdRSP 
	PURPOSE				: Process EV_CFW_SS_SEND_USSD_RSP  
				signal received form CSW.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
#include "sat_def.h"
#include "satstruct.h"	
extern void adp_SAT_SENDUSSD_rsp(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI);
extern SATCONTEXT gSATContext;
extern adp_sat_context_t *g_pSATContext;
#define AT_CODE (0x40)

static BOOL SS_SENDUssdRSP(COS_EVENT * pCosEvent)
{
	mmi_ss_ussd_rsp_struct *rsp_ussdptr = NULL;
	mmi_ss_abort_rsp_struct *rsp_abortptr = NULL;
	CFW_SS_USSD_IND_INFO *pUSSDInd = NULL;
	UINT8 nType = HIUINT8(pCosEvent->nParam3);
	UINT8 nUTI = HIUINT16(pCosEvent->nParam3);	
	U8 ussd_dcs;
	U16 ussdlen = 0;
	U16 i = 0;
    
	ASSERT(pCosEvent != NULL);
	
	mmi_trace(g_sw_ADP_SS, "SS SS_SENDUssdRSP pCosEvent->nParam1= %x, pCosEvent->nParam2 =%x, pCosEvent->nParam3 = %x, nType = %x"
		, pCosEvent->nParam1, pCosEvent->nParam2, pCosEvent->nParam3, nType); 
	
	pUSSDInd = (CFW_SS_USSD_IND_INFO *)pCosEvent->nParam1;
	
	if (nType == 0 && pUSSDInd != NULL)//success
	{
		if(pCosEvent->nParam2 == 2)//abort
		{
			rsp_abortptr = OslMalloc(SIZEOF(mmi_ss_abort_rsp_struct));
			ASSERT(rsp_abortptr != NULL);
			
			mmi_trace(g_sw_ADP_SS, "SS PRT_SS_STRING_ABORT_RSP");	
			gSSUTI = 27;
			rsp_abortptr->result = TRUE;
			Adp_SendMessageToMMI((local_para_struct *)rsp_abortptr, PRT_SS_STRING_ABORT_RSP, ADP_SS_MSG_SRC_MOD);
			adp_SAT_SENDUSSD_rsp(pCosEvent->nParam1, pCosEvent->nParam2, nType, nUTI);
		}
		else
		{
			mmi_trace(g_sw_ADP_SS, "SS dcs=%x StingSize=%x pUSSDInd->nOption = %x",pUSSDInd->nDcs, pUSSDInd->nStingSize,pUSSDInd->nOption);	
		
			if(pUSSDInd->nOption == 0x02)
			{
				if (g_pSATContext->gCurrSatReqId == PRT_MMI_SAT_SEND_USSD_STAGE1_REQ)
				{
            				adp_SAT_SENDUSSD_rsp(pCosEvent->nParam1, pCosEvent->nParam2, nType, nUTI);
            				return TRUE;
				}
			}

			rsp_ussdptr = OslMalloc(SIZEOF(mmi_ss_ussd_rsp_struct));
			ASSERT(rsp_ussdptr != NULL);
			memset(rsp_ussdptr, 0, sizeof(mmi_ss_ussd_rsp_struct));
				
			rsp_ussdptr->op_code= pUSSDInd->nOption;
	
			ADP_USSDParseDCS(pUSSDInd->nDcs, &ussd_dcs);

			if ( 0x101 == pCosEvent->nParam2 )
			{
        
				rsp_ussdptr->dcs = MMI_8BIT_DCS;
				memcpy( rsp_ussdptr->ussd_str, pUSSDInd->pUsdString, pUSSDInd->nStingSize );	
        
			}
			else if (ussd_dcs == ADP_MSG_GSM_7 || ussd_dcs == ADP_MSG_RESERVED)  //7 bit code
			{
				ussdlen = SUL_Decode7Bit(pUSSDInd->pUsdString, rsp_ussdptr->ussd_str, pUSSDInd->nStingSize);
				for(i = 0; i < ussdlen; i ++)
				{
					if(*(rsp_ussdptr->ussd_str + i) == 0)
					{
						*(rsp_ussdptr->ussd_str + i) = AT_CODE;
					}
				}
                        rsp_ussdptr->dcs = MMI_8BIT_DCS;  //ascii coding
			}
			else if (ussd_dcs == ADP_MSG_8_BIT_DATA)
			{
				rsp_ussdptr->dcs = MMI_8BIT_DCS;
				memcpy(rsp_ussdptr->ussd_str, pUSSDInd->pUsdString, pUSSDInd->nStingSize);	
			}
			else if (ussd_dcs == ADP_MSG_UCS2)
			{
				rsp_ussdptr->dcs = MMI_UCS2_DCS;
				
				if(pUSSDInd->pUsdString[0] == 0x81)
					rsp_ussdptr->dcs = MMI_UCS2_81;
				else if(pUSSDInd->pUsdString[0] == 0x82)
					rsp_ussdptr->dcs = MMI_UCS2_82;
				
				mmi_trace(g_sw_ADP_SS, "USSD pUSSDInd->pUsdString[0] = %x",pUSSDInd->pUsdString[0]);	

				memcpy(rsp_ussdptr->ussd_str, pUSSDInd->pUsdString, pUSSDInd->nStingSize);	
			}
			rsp_ussdptr->result.flag = L4C_OK;

			Adp_SendMessageToMMI((local_para_struct *)rsp_ussdptr, PRT_USSD_RES_SUCCESS, ADP_SS_MSG_SRC_MOD);

		}	
	}
	else if(nType != 0)
	{
		mmi_trace(g_sw_ADP_SS,  "SS func: %s nType = %x", __FUNCTION__, nType );

 		if( 0xFC == nType )
 		{
     
 		  SS_ReSendProcUssV1Req();
 		  return TRUE;
      
 		}
 		else if( 0xFA == nType || 0xFB == nType  || 0xFD == nType)
		{
			rsp_ussdptr = OslMalloc(SIZEOF(mmi_ss_ussd_rsp_struct));
			ASSERT(rsp_ussdptr != NULL);
			memset(rsp_ussdptr, 0, sizeof(mmi_ss_ussd_rsp_struct));
			
			rsp_ussdptr->result.flag = L4C_ERROR;
			rsp_ussdptr->result.cause = CM_UNASSIGNED_NUM; //need further development.

			Adp_SendMessageToMMI((local_para_struct *)rsp_ussdptr, PRT_USSD_RES_SUCCESS, ADP_SS_MSG_SRC_MOD);
		}
		else
		{
			mmi_trace(g_sw_ADP_SS,  "SS func: %s pUSSDInd->nOption = %d", __FUNCTION__, pUSSDInd->nOption);
  	
			if(pUSSDInd->nOption == 2)
			{
				rsp_abortptr = OslMalloc(SIZEOF(mmi_ss_abort_rsp_struct));
				ASSERT(rsp_abortptr != NULL);
				gSSUTI = 27;
				rsp_abortptr->result = FALSE;
				Adp_SendMessageToMMI((local_para_struct *)rsp_abortptr, PRT_SS_STRING_ABORT_RSP, ADP_SS_MSG_SRC_MOD);	
			}
			else
			{
				rsp_ussdptr = OslMalloc(SIZEOF(mmi_ss_ussd_rsp_struct));
				ASSERT(rsp_ussdptr != NULL);
				memset(rsp_ussdptr, 0, sizeof(mmi_ss_ussd_rsp_struct));
  			
				rsp_ussdptr->result.flag = L4C_ERROR;
				rsp_ussdptr->result.cause = CM_UNASSIGNED_NUM; //need further development.

				Adp_SendMessageToMMI((local_para_struct *)rsp_ussdptr, PRT_USSD_RES_SUCCESS, ADP_SS_MSG_SRC_MOD);
			}
		}
		
		adp_SAT_SENDUSSD_rsp(pCosEvent->nParam1, pCosEvent->nParam2, nType, nUTI);
	}
	else
	{
		rsp_abortptr = OslMalloc(SIZEOF(mmi_ss_abort_rsp_struct));
		ASSERT(rsp_abortptr != NULL);
			
		mmi_trace(g_sw_ADP_SS, "SS PRT_SS_STRING_ABORT_RSP");	
		gSSUTI = 27;
		rsp_abortptr->result = TRUE;
		Adp_SendMessageToMMI((local_para_struct *)rsp_abortptr, PRT_SS_STRING_ABORT_RSP, ADP_SS_MSG_SRC_MOD);	
		adp_SAT_SENDUSSD_rsp(pCosEvent->nParam1, pCosEvent->nParam2, nType, nUTI);
	}
	
	return TRUE;
}
/**************************************************************
	FUNCTION NAME		: ADP_DispatchSsReqMsg
	PURPOSE				: Message Adaption for Supplementary
			Service REQ.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL ADP_DispatchSsReqMsg(COS_EVENT *pCosEvent)
{
	CFW_SS_SET_CALLFORWARDING_INFO CallForward;
	ASSERT(pCosEvent != NULL);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_SS, adp_GetMMISimID(ADP_GET_MMI_DEST_MOD(pCosEvent->nParam3)));
#endif

	switch (pCosEvent->nEventId)
	{
		case MSG_ID_MMI_SS_PARSING_STRING_REQ:
			// TODO: 不知为何屏蔽掉?
			#if 0
			AdaptParsingStringREQ(pCosEvent);
			#endif
			break;
            case MSG_ID_MMI_SS_CALL_FORWARD_REQ:
			
			memset(&CallForward, 0, sizeof(CFW_SS_SET_CALLFORWARDING_INFO));
      CallForward.nClass = Adp_TELEPHONY;
			CallForward.nReason = CFW_SS_FORWARDING_REASON_UNCONDITIONAL; 
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			UT_CFW_SsQueryCallForwarding(CallForward.nReason, CallForward.nClass, FREE_UTI, g_pSSContext->sim_id);
#else
			CFW_SsQueryCallForwarding(CallForward.nReason, CallForward.nClass, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
#ifdef WAP_MMS_SUPPORT /* zhuoxz,2009-4-28 */
      CFW_SsQueryCallForwarding(CallForward.nReason, CallForward.nClass, FREE_UTI);
#else /* #ifdef WAP_MMS_SUPPORT */
#ifndef MMI_ON_WIN32
			CFW_SsQueryCallForwarding(CallForward.nReason, CallForward.nClass, FREE_UTI);
#else /* #ifndef MMI_ON_WIN32 */
      CFW_SsQueryCallForwarding(CallForward.nReason, CallForward.nClass, FREE_UTI);
#endif /* #ifndef MMI_ON_WIN32 */
#endif /* #ifdef WAP_MMS_SUPPORT */
#endif /* __ADP_MULTI_SIM__ */
	   		 break;
		case MSG_ID_MMI_SS_ABORT_REQ:
			AdaptProcUssAbortReq();		
			break;
		case MSG_ID_MMI_SS_SEND_USSD_REQ:
			AdaptSendUSSDREQ(pCosEvent);
			break;
		default:
			return FALSE;
	}

	return TRUE;
}

/**************************************************************
	FUNCTION NAME		: ADP_DispatchSsRspMsg
	PURPOSE				: Message Adaption for Supplementary
			Service RSP.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static BOOL ADP_DispatchSsRspMsg(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29ff);

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
//do nothing
#else  /* No Defined ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
	adp_ConfigureContext(ADP_MODULE_SS, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#endif

	switch (pCosEvent->nEventId)
	{
		case EV_CFW_SS_SET_CALL_WAITING_RSP:
		case EV_CFW_SS_QUERY_CALL_WAITING_RSP:
			AdaptCallWaitingRSP(pCosEvent);
			break;

		case EV_CFW_SS_SET_CALL_FORWARDING_RSP:
			AdaptSetCallForwardingRSP(pCosEvent);
			break;

		case EV_CFW_SS_QUERY_CALL_FORWARDING_RSP:
			/*Guojian Modify Start For Call Forward Ver: GreenStone  on 2008-11-2 14:2 */
			if (g_pSSContext->forwarding_state&0x40) //if it's power on query status
			{
				AdaptQueryAllCallForwardingRSP(pCosEvent);
			}
			else
			{
				AdaptQueryCallForwardingRSP(pCosEvent);
			}
			break;
			/*Guojian Modify End  For Call Forward Ver: GreenStone  on 2008-11-2 14:2 */

		case EV_CFW_SS_SET_FACILITY_LOCK_RSP:
		case EV_CFW_SS_QUERY_FACILITY_LOCK_RSP:
			
			AdaptCallBarringRSP(pCosEvent);
			break;

		case EV_CFW_SS_CHANGE_PWD_RSP:
			AdaptChangePasswordRSP(pCosEvent);
			break;
			
		case EV_CFW_SS_QUERY_CLIP_RSP:
			SS_InterrogateClipRSP(pCosEvent);
			break;
			   
		case EV_CFW_SS_QUERY_CLIR_RSP :
			SS_InterrogateClirRSP(pCosEvent);
			break;
			
		case EV_CFW_SS_QUERY_COLR_RSP:
			SS_InterrogateColrRSP(pCosEvent);
			break;
			
		case EV_CFW_SS_QUERY_COLP_RSP:
			SS_InterrogateColpRSP(pCosEvent);
			break;

		case EV_CFW_SS_SEND_USSD_RSP:
			SS_SENDUssdRSP(pCosEvent);
			break;

		default:
			TBM_EXIT(0x29ff);
			return FALSE;
	}
	TBM_EXIT(0x29ff);
	return TRUE;
}

/*==========================================================================
 * This function scans the user input string from the position specified
 * and parses upto four * deleimited parameters.
 *
 * On entry it is assumed that the last character is a '#'
 *=========================================================================*/
static void BreaktItUp(UINT8 *pStr, UINT8 len, UINT8 pos, SSParsedParameters *pParsed)
{
	UINT8 parmNum = 0;

	ASSERT(pStr != NULL);
	ASSERT(pParsed != NULL);
	ASSERT(pStr[len-1] == CHR_HASH);

	/* Pos points to the first parameter */
	pParsed->parmStart[parmNum] = pos;

	/* scan string until end of string or upto MAX_PARMS parameter parsed */
	/* or upto the first # */
	while ((parmNum < MAX_PARMS) && (pos <= len))
	{
		if (pStr[pos] == CHR_HASH)
		{
			/* abort scanning after updating the length */
			pParsed->parmLen[parmNum] = pos - pParsed->parmStart[parmNum];
			break;
		}
		else
		{
			/* Not terminator so check if delimitor */
			if (pStr[pos] == CHR_STAR)
			{
				/* It is a delimiter so update the length of the current parm*/
				pParsed->parmLen[parmNum] = pos - pParsed->parmStart[parmNum];
				/* advance pointer and increment the parm number*/
				parmNum++;
				pos++;
				/* and update start position of this parameter */
				if (parmNum < MAX_PARMS)
				{
					pParsed->parmStart[parmNum] = pos;
				}
			}
			else
			{
				/* not a delimitor so increment pos */
				pos++;
			}
		}
	}
}

/*==========================================================================
 * Given a length terminated string, this routine parses it so that upto 4
 * parameters seperated by delimitors is extracted. The start of the string
 * can be one of the following:-
 *
 *                *                            SSMMI_STAR
 *                #                            SSMMI_HASH
 *                *#                           SSMMI_STARHASH
 *                **                           SSMMI_STARSTAR
 *                ##                           SSMMI_HASHHASH
 *                none of the above.           SSMMI_USS
 *
 * the rest of the string will consist of parameters delimited by a '*' and
 * will be terminated by a '#'.
 *
 * Assumptions on entry:-
 *
 *  1) The input string is not empty
 *  2) The last chr is # if len > 2
 *
 *=========================================================================*/
static kal_uint8 ParseUserInput(UINT8 *pStr, UINT8 len, SSParsedParameters *pParsed)
{
	UINT8 i, pos = 0;
	kal_uint8 resultFlag = PARSE_STRING_SUCESS;

	ASSERT(pStr != NULL);
	ASSERT(pParsed != NULL);
	ASSERT(len != 0);
	/* Check that the last chr is # if the length is > 2 */
	/*Correct assert, allow a user_len <=2*/
	//ASSERT((len <= 2) || ((len > 2) && (pStr[len - 1] == CHR_HASH)));

	/* make all parms empty */
	for (i = 0; i < MAX_PARMS; i++)
	{
		pParsed->parmStart[i] = 0;
		pParsed->parmLen[i] = 0;
	}

	/* Not empty so look at the beginning of the string .. treat special if len <=2 */
	if (len <= 2)
	{
		pParsed->type = SSMMI_USS;
		pParsed->parmStart[0] = pos;
		pParsed->parmLen[0] = len;
	}
	else
	{
		/* the string length is 3 or more */
		switch (pStr[pos])
		{
			case CHR_STAR:
				/* it could be * or *# or ** */
				pos++;
				if (pStr[pos + 1] >= '0' && pStr[pos + 1] <= '9')
				{
					switch (pStr[pos])
					{
						case CHR_HASH:
							pParsed->type = SSMMI_STARHASH;
							pos++;
							break;
							
						case CHR_STAR:
							pParsed->type = SSMMI_STARSTAR;
							pos++;
							break;
							
						default:
							pParsed->type = SSMMI_STAR;
							break;
					}
					BreaktItUp(pStr, len, pos, pParsed);
				}
				else
				{
					resultFlag = PARSE_STRING_FAILED;
				}
				break;

			case CHR_HASH:
				/* it could be # or ## */
				pos++;
				if (pStr[pos + 1] >= '0' && pStr[pos + 1] <= '9')
				{
					if (pStr[pos] == CHR_HASH)
					{
						pos++;
						pParsed->type = SSMMI_HASHHASH;
					}
					else
					{
						pParsed->type = SSMMI_HASH;
					}
					BreaktItUp(pStr, len, pos, pParsed);
				}
				else
				{
					resultFlag = PARSE_STRING_FAILED;
				}
				break;

			default:
				/* It does not begin with a * or # so treat it as a uss message */
				pParsed->type = SSMMI_USS;
				pParsed->parmStart[0] = pos;
				pParsed->parmLen[0] = len;
				break;
		}
	}

	return resultFlag;
}

/*==========================================================================
 * This routine is used to check if the length terminated string specified
 * matches the null terminated string
 *=========================================================================*/
static BOOL CmpzStrToLStr(const UINT8 *pZStr, const UINT8 *pLStr, UINT8 strLength)
{
	BOOL retValue = TRUE; /* Assume they match */

	ASSERT(pZStr != NULL);
	ASSERT(pLStr != NULL);

	/* Check if both of same length */
	if (strlen(pZStr) == strLength)
	{
		/* the lengths are the same so they could be the same */
		while (strLength)
		{
			if (*pZStr != *pLStr)
			{
				/* Not same so the strings don't match */
				retValue = FALSE;
				break;
			}
			pZStr++;
			pLStr++;
			strLength--;
		}
	}
	else
	{
		/* Not of same length so set result to FALSE */
		retValue = FALSE;
	}

	/* return the result */
	return retValue;
}

/*==========================================================================
 * This routine is used to determine which of the operations listed below
 * the user mmi string should result in:
 *                     REGISTER,          pSSCode supplied
 *                     ERASE,             pSSCode supplied
 *                     ACTIVATE,          pSSCode supplied
 *                     DEACTIVATE,        pSSCode supplied
 *                     INTERROGATE,       pSSCode supplied
 *                     REG_PASSWORD,      pSSCode supplied
 *                     USSD
 *
 * If it is of the first 5 types then the sscode is also updated
 *=========================================================================*/
static void GetTypeOfOperation(UINT8 *pStr, SSParsedParameters *pParsedParameters, l4_op_code_enum *pOpCode, ss_code_enum *pSSCode)
{
	const MmiCodeToSsCode *pTable = &ssMmiCodeToSsCode[0];
	BOOL found;
	UINT8 *servCode_p = NULL;
	UINT8 servCodeLen;

	ASSERT(pStr != NULL);
	ASSERT(pParsedParameters != NULL);
	ASSERT(pOpCode != NULL);
	ASSERT(pSSCode != NULL);

	/* check if the user string prefix is ussd type */
	if (pParsedParameters->type == SSMMI_USS)
	{
		/* It is */
		*pOpCode = SS_OP_PROCESSUNSTRUCTUREDSS_DATA;
	}
	else
	{
		/* It is NOT but it could be if the service code is meaningless */
		/* so first look through the recognised service code table */
		found = FALSE;
		servCode_p = &pStr[pParsedParameters->parmStart[0]];
		servCodeLen = pParsedParameters->parmLen[0];

		/* search through table */
		while (pTable->pzMmiStr != NULL)
		{
			if (CmpzStrToLStr(pTable->pzMmiStr, servCode_p, servCodeLen) == TRUE)
			{
				/* A match has been found */
				found=TRUE;
				break;
			}
			/* increment the table pointer */
			pTable++;
		}

		if (found == TRUE)
		{
			/* A match found in the table so it is a recognised type */
			*pSSCode = pTable->SSCode;

			switch (pParsedParameters->type)
			{
				case SSMMI_STAR: /* activate or register */
					/* assume that it is activation */
					*pOpCode = SS_OP_ACTIVATESS;
					break;
					
				case SSMMI_HASH: /* deactivate */
					*pOpCode = SS_OP_DEACTIVATESS;
					break;
					
				case SSMMI_STARHASH: /* interrogate */
					*pOpCode = SS_OP_INTERROGATESS;
					break;
					
				case SSMMI_STARSTAR: /* register */
					*pOpCode = SS_OP_REGISTERSS;
					break;
					
				case SSMMI_HASHHASH: /* erase */
					*pOpCode = SS_OP_ERASESS;
					break;
					
				default:
					//The program can not go here.
					mmi_trace(g_sw_ADP_SS, "SS MMI Parsing should never reach here");
					ASSERT(0);					
					break;
			}
		}
		else
		{
			/* not recognised so it could be a reg password, otherwise it is */
			/* an unstructured ss operation type */
			if (CmpzStrToLStr(ssPswdServiceCode, servCode_p, servCodeLen))
			{
				/* Treat this as register password */
				*pOpCode = SS_OP_REGISTERPASSWORD; 
				
				/* and also detrmine the service code which is in the second parameter*/
				if (CmpzStrToLStr(ssBaocCode, &pStr[pParsedParameters->parmStart[1]], pParsedParameters->parmLen[1]))
				{
					/* the barring services code has been supplied */
					*pSSCode = SS_BAOC;
				}
				else if (CmpzStrToLStr(ssBoicCode, &pStr[pParsedParameters->parmStart[1]], pParsedParameters->parmLen[1]))
				{
					*pSSCode = SS_BOIC;
				}
				else if (CmpzStrToLStr(ssBoicExhcCode, &pStr[pParsedParameters->parmStart[1]], pParsedParameters->parmLen[1]))
				{
					*pSSCode = SS_BOICEXHC;
				}
				else if (CmpzStrToLStr(ssBaicCode, &pStr[pParsedParameters->parmStart[1]], pParsedParameters->parmLen[1]))
				{
					*pSSCode = SS_BAIC;
				}
				else if (CmpzStrToLStr(ssBicroamCode, &pStr[pParsedParameters->parmStart[1]], pParsedParameters->parmLen[1]))
				{
					*pSSCode = SS_BICROAM;
				}
				else if (CmpzStrToLStr(ssAllCallRestrictionCode, &pStr[pParsedParameters->parmStart[1]], pParsedParameters->parmLen[1]))
				{
					*pSSCode = SS_ALLBARRINGSS;
				}
				//else if (CmpzStrToLStr(ssBarringOfOutgoingCode, &pStr[pParsedParameters->parmStart[1]], pParsedParameters->parmLen[1]))
				//{
					//*pSSCode = SS_BARRINGOFOUTGOINGCALLS;
				//} 
				else if (CmpzStrToLStr(ssBarringOfIncomingCode, &pStr[pParsedParameters->parmStart[1]], pParsedParameters->parmLen[1]))
				{
					*pSSCode = SS_BARRINGOFINCOMINGCALLS;
				}
				else
				{
					*pSSCode = SS_ALLSS;
				}
				
			}
			else
			{
				/* Treat this as unstructured operatin */
				*pOpCode = SS_OP_PROCESSUNSTRUCTUREDSS_DATA;
			}
			mmi_trace(g_sw_ADP_SS, "GetTypeOfOperation *pSSCode =%x,*pSSCode = %d", *pSSCode,*pSSCode);
		}
	}
}

/*==========================================================================
 * This routine converts a text string assumed to be a number into
 * a binary enum value. If non digit characters are encountered then
 * a false is returned otherwise a true.
 *=========================================================================*/
static BOOL ValidMmiBsCode(UINT8 *pBsCodeStr, UINT8 bsCodeLen, Adp_bs_code_enum *pMmiBsCode)
{
	UINT32 code = 0;
	BOOL retValue = TRUE; /* assume it is ok */

	ASSERT(pBsCodeStr != NULL);
	ASSERT(pMmiBsCode != NULL);
	ASSERT(bsCodeLen != 0);

	while (bsCodeLen != 0)
	{
		/* check if valid digit */
		if (!isdigit(*pBsCodeStr))
		{
			/* A non digit has been encountered */
			retValue = FALSE;
			break;
		}

		/* This is a digit */
		code = (code * 10) + (*pBsCodeStr - '0');

		if (code > 0xFFFF)
		{
			/* the bs code value is too big so flag it as illegal */
			retValue = FALSE;
			break;
		}

		/* advance pointers and decrement count */
		pBsCodeStr++;
		bsCodeLen--;
	}

	/* copy the decoded code into the output variable */
	*pMmiBsCode = (Adp_bs_code_enum)code;
	
	if (!((*pMmiBsCode >= 10 && *pMmiBsCode<=25 && *pMmiBsCode != 14 && *pMmiBsCode != 15) ||(*pMmiBsCode == 29)||(*pMmiBsCode == 0)))
		retValue = FALSE;
	
	mmi_trace(g_sw_ADP_SS, "ValidMmiBsCode *pMmiBsCode %d, %d", *pMmiBsCode, retValue);
	/* return the conversion status */
	return retValue;
}

/*==========================================================================
 * This routine copies the "UINT8" substring starting at
 * 'pSrc' and 'subLen' characters upto 'maxlen' long to the "UINT8" string
 * 'pDst[0]'. It also copies the value of 'subLen' into
 * the destination string length variable provided.
 *=========================================================================*/
static void CopySubString(UINT8 *pSrc, UINT8 subLen, UINT8 *pDst, UINT8 *dstLen, UINT8 maxLen)
{
	ASSERT(pSrc != NULL);
	ASSERT(pDst != NULL);
	ASSERT(dstLen != NULL);

	/* update the destiantion string length */
	*dstLen = subLen = (subLen > maxLen) ? maxLen : subLen;

	/* copy the substring */
	while (subLen != 0)
	{
		*pDst = *pSrc;
		pDst++;
		pSrc++;
		subLen--;
	}
}

/*==========================================================================
 * This routine converts a text string assumed to be a number into
 * a binary 8 bit value. If non digit characters are encountered then
 * a false is returned otherwise a true.
 *=========================================================================*/
static BOOL ValidReplyTime(UINT8 *pTimeStr, UINT8 timeStrLen, INT8 *pTime)
{
	INT16 timeVal = 0;
	BOOL retValue = TRUE; /* assume it is ok */

	ASSERT(pTimeStr != NULL);
	ASSERT(pTime != NULL);
	ASSERT(timeStrLen != 0);

	while (timeStrLen != 0)
	{
		/* check if valid digit */
		if (!isdigit(*pTimeStr))
		{
			/* A non digit has been encountered */
			retValue = FALSE;
			return retValue;                       //add by yaosq 060822
		}

		/* This is a digit */
		timeVal = (timeVal * 10) + (*pTimeStr - '0');

		if (timeVal > 0x7F)
		{
			/* the time value is too big so flag it as illegal */
			retValue = FALSE;
			return retValue;                   //add by yaosq 060822
		}

		/* advance pointers and decrement count */
		pTimeStr++;
		timeStrLen--;
	}

	/* limit the timeval to 0 to 30 seconds and ensure that it is divisible by 5 */
	/* without remainders                                                        */
	if (timeVal > MAX_BUSY_TIMEOUT)
	{
		timeVal = MAX_BUSY_TIMEOUT;
	}
	else if (timeVal < MAX_BUSY_STEP)
	{
		timeVal = MAX_BUSY_STEP;
	}

	/* Clip the timeout value to multiples of 5 */
	timeVal = (timeVal / MAX_BUSY_STEP) * MAX_BUSY_STEP;

	/* copy the decoded time value into the output variable */
	*pTime = (INT8)timeVal;

	/* return the conversion status */
	return retValue;
}

/*Invoke CSW function to set or guery call waiting.*/
static void ADP_SS_SetCallWaiting(l4_op_code_enum OpCode, BOOL BsCodePresent, Adp_bs_code_enum MmiBsCode)
{
	UINT32 ret;
	mmi_ss_call_wait_rsp_struct *rsp_ptr = NULL;
	 TBM_ENTRY(0x29e2);
	if (BsCodePresent == FALSE)
	{
		BsCodePresent = TRUE;
		MmiBsCode = Adp_TELEPHONY;
	}
	gCwBsCodePresent = BsCodePresent;
	gCwBsCode = MmiBsCode;

	switch (OpCode)
	{
		case SS_OP_REGISTERSS:
		case SS_OP_ACTIVATESS:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
			
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsSetCallWaiting(CW_ENABLE, MmiBsCode, FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsSetCallWaiting(CW_ENABLE, MmiBsCode, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsSetCallWaiting(CW_ENABLE, MmiBsCode, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallWaiting()   CFW_SsSetCallWaiting  ret = %d", ret);
				rsp_ptr = OslMalloc(sizeof(mmi_ss_call_wait_rsp_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_call_wait_rsp_struct));
				
				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_WAIT_RSP, ADP_SS_MSG_SRC_MOD);
			}
			break;
			
		case SS_OP_ERASESS:
		case SS_OP_DEACTIVATESS:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
			
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsSetCallWaiting(CW_DISABLE, MmiBsCode, FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsSetCallWaiting(CW_DISABLE, MmiBsCode, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsSetCallWaiting(CW_DISABLE, MmiBsCode, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallWaiting()   CFW_SsSetCallWaiting  ret = %d", ret);
				rsp_ptr = OslMalloc(sizeof(mmi_ss_call_wait_rsp_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_call_wait_rsp_struct));
				
				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_WAIT_RSP, ADP_SS_MSG_SRC_MOD);
			}
			break;
			
		case SS_OP_INTERROGATESS:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
			
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsQueryCallWaiting(MmiBsCode, FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsQueryCallWaiting(MmiBsCode, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsQueryCallWaiting(MmiBsCode, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallWaiting()   CFW_SsQueryCallWaiting  ret = %d", ret);
				rsp_ptr = OslMalloc(sizeof(mmi_ss_call_wait_rsp_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_call_wait_rsp_struct));
				
				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_WAIT_RSP, ADP_SS_MSG_SRC_MOD);
			}
			break;
			
		default:
			//The program can not go here.
			mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallWaiting Error OpCode,%d", OpCode);
			SS_Adaption_SendParseStringRSP(PARSE_STRING_FAILED);
			break;
	}
	 TBM_EXIT(0x29e2);

}

/*Invoke CSW function to set or guery call forwarding.*/
static void ADP_SS_SetCallForward(l4_op_code_enum OpCode, ss_code_enum SSCode, BOOL BsCodePresent, Adp_bs_code_enum MmiBsCode, BOOL TimePresent, INT8 Time, UINT8 numberBCDBytesLen, UINT8 *pNumberBCD)
{
	CFW_SS_SET_CALLFORWARDING_INFO CallForward;
	UINT32 ret;
	mmi_ss_call_forward_rsp_struct *rsp_ptr = NULL;
	TBM_ENTRY(0x29fb);
	memset(&CallForward, 0, sizeof(CFW_SS_SET_CALLFORWARDING_INFO));

	if (BsCodePresent == FALSE)
	{
		BsCodePresent = TRUE;
		MmiBsCode = Adp_TELEPHONY;
	}

#ifdef WAP_MMS_SUPPORT /* zhuoxz,2009-4-28 */
  CallForward.nClass = MmiBsCode;
#else /* #ifdef WAP_MMS_SUPPORT */
#ifndef MMI_ON_WIN32
  CallForward.nClass = MmiBsCode;
#else /* #ifndef MMI_ON_WIN32 */
  CallForward.nClass = MmiBsCode;
#endif /* #ifndef MMI_ON_WIN32 */
#endif /* #ifdef WAP_MMS_SUPPORT */

	mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallForward MmiBsCode = %d", MmiBsCode);
	mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallForward SSCode = 0x%x", SSCode);
	mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallForward OpCode = %d", OpCode);
	switch (SSCode)
	{
		case SS_CFU:
			CallForward.nReason = CFW_SS_FORWARDING_REASON_UNCONDITIONAL;
			break;
			
		case SS_CFB:
			CallForward.nReason = CFW_SS_FORWARDING_REASON_MOBILEBUSY;
			break;
			
		case SS_CFNRY:
			CallForward.nReason = CFW_SS_FORWARDING_REASON_NOREPLY;
			break;
			
		case SS_CFNRC:
			CallForward.nReason = CFW_SS_FORWARDING_REASON_NOTREACHABLE;
			break;
			
		case SS_ALLFORWARDINGSS:
			CallForward.nReason = CFW_SS_FORWARDING_REASON_ALL_CALLFORWARDING;
			break;
			
		case SS_ALLCONDFORWARDINGSS:
			CallForward.nReason = CFW_SS_FORWARDING_REASON_ALL_CONDCALLFORWARDING;
			break;
			
		default:
			CallForward.nReason = CFW_SS_FORWARDING_REASON_UNCONDITIONAL;
			break;
	}

	switch (OpCode)
	{
		case SS_OP_REGISTERSS:
		case SS_OP_ACTIVATESS:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
			
			if (pNumberBCD != NULL)
			{
				CallForward.nNumber.pDialNumber = OslMalloc(numberBCDBytesLen);
				ASSERT(CallForward.nNumber.pDialNumber != NULL);
				memcpy(CallForward.nNumber.pDialNumber, pNumberBCD, numberBCDBytesLen);
			}
			CallForward.nNumber.nDialNumberSize = numberBCDBytesLen;
			CallForward.nNumber.nClir = 0;
			if (CallForwardingFlag == FALSE)
			{
			CallForward.nNumber.nType = CFW_TELNUMBER_TYPE_UNKNOWN;
			}
			else
			{
				CallForward.nNumber.nType = CFW_TELNUMBER_TYPE_INTERNATIONAL;
				CallForwardingFlag = FALSE;
			}

			if (TimePresent == TRUE)
			{
				CallForward.nTime = Time;
			}
			CallForward.nMode = CFW_SS_FORWARDING_MODE_REGISTRATION;
#if 0
			if(OpCode == SS_OP_REGISTERSS)
			{
				CallForward.mode = CFW_SS_FORWARDING_MODE_REGISTRATION;
			}
			else
			{
				CallForward.mode = CFW_SS_FORWARDING_MODE_ENABLE;
			}
#endif			
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsSetCallForwarding(&CallForward, FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsSetCallForwarding(&CallForward, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsSetCallForwarding(&CallForward, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallForward()   CFW_SsSetCallForwarding  ret = %d", ret);
				rsp_ptr = OslMalloc(sizeof(mmi_ss_call_forward_rsp_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_call_forward_rsp_struct));
				
				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_FORWARD_RSP, ADP_SS_MSG_SRC_MOD);
			}
			if (CallForward.nNumber.pDialNumber != NULL)
			{
				OslMfree(CallForward.nNumber.pDialNumber);
				CallForward.nNumber.pDialNumber = NULL;
			}

			break;
			
		case SS_OP_ERASESS:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
			CallForward.nMode = CFW_SS_FORWARDING_MODE_ERASURE;
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsSetCallForwarding(&CallForward, FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsSetCallForwarding(&CallForward, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsSetCallForwarding(&CallForward, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallForward()   CFW_SsSetCallForwarding  ret = %d", ret);
				rsp_ptr = OslMalloc(sizeof(mmi_ss_call_forward_rsp_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_call_forward_rsp_struct));
				
				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_FORWARD_RSP, ADP_SS_MSG_SRC_MOD);
			}
			break;
			
		case SS_OP_DEACTIVATESS:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
			
			CallForward.nMode = CFW_SS_FORWARDING_MODE_DISABLE;
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsSetCallForwarding(&CallForward, FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsSetCallForwarding(&CallForward, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsSetCallForwarding(&CallForward, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallForward()   CFW_SsSetCallForwarding  ret = %d", ret);
				rsp_ptr = OslMalloc(sizeof(mmi_ss_call_forward_rsp_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_call_forward_rsp_struct));
				
				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_FORWARD_RSP, ADP_SS_MSG_SRC_MOD);
			}
			break;
			
		case SS_OP_INTERROGATESS:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
			mmi_trace(g_sw_ADP_SS, "CFW_SsQueryCallForwarding reason = %d", CallForward.nReason);
			mmi_trace(g_sw_ADP_SS, "CFW_SsQueryCallForwarding mode = %d", CallForward.nMode);
      mmi_trace(g_sw_ADP_SS, "CFW_SsQueryCallForwarding class = %d", CallForward.nClass);
			mmi_trace(g_sw_ADP_SS, "CFW_SsQueryCallForwarding time= %d", CallForward.nTime);

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsQueryCallForwarding(CallForward.nReason, CallForward.nClass, FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsQueryCallForwarding(CallForward.nReason, CallForward.nClass, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsQueryCallForwarding(CallForward.nReason, CallForward.nClass, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */


			if (ret != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallForward()   CFW_SsQueryCallForwarding  ret = %d", ret);
				rsp_ptr = OslMalloc(sizeof(mmi_ss_call_forward_rsp_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_call_forward_rsp_struct));
				
				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_FORWARD_RSP, ADP_SS_MSG_SRC_MOD);
			}
			break;
			
		default:
			//The program can not go here.
			mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallForward Error OpCode,%d", OpCode);
			SS_Adaption_SendParseStringRSP(PARSE_STRING_FAILED);
			break;
	}
	TBM_EXIT(0x29fb);

}

/*Invoke CSW function to set or guery call barring.*/
static void ADP_SS_SetCallBarring(l4_op_code_enum OpCode, ss_code_enum SSCode, BOOL BsCodePresent, Adp_bs_code_enum MmiBsCode, UINT8 PasswordLength, UINT8 *pPassword)
{
	UINT16 nFac = 0;
	UINT8 nClass = 0;
	UINT32 ret = 0;
	mmi_ss_call_barring_rsp_struct *rsp_ptr = NULL;
 	TBM_ENTRY(0x29fa);
	ASSERT(pPassword != NULL);

	mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallBarring SSCode = %d", SSCode);
	switch (SSCode)
	{
		case SS_BAOC:			
			nFac = CFW_STY_FAC_TYPE_AO;
			break;
			
		case SS_BOIC:
			nFac = CFW_STY_FAC_TYPE_OI;
			break;
			
		case SS_BOICEXHC:
			nFac = CFW_STY_FAC_TYPE_OX;
			break;
			
		case SS_BAIC:
			nFac = CFW_STY_FAC_TYPE_AI;
			break;
			
		case SS_BICROAM:
			nFac = CFW_STY_FAC_TYPE_IR;
			break;
			
		case SS_ALLBARRINGSS:
			nFac = CFW_STY_FAC_TYPE_AB;
			break;
			
		//case SS_BARRINGOFOUTGOINGCALLS:
			//nFac = CFW_STY_FAC_TYPE_AG;
			//break;
			
		case SS_BARRINGOFINCOMINGCALLS:
			nFac = CFW_STY_FAC_TYPE_AC;
			break;
			
		default:
			break;
	}

	if (BsCodePresent == FALSE)
	{
		BsCodePresent = TRUE;
		MmiBsCode = Adp_TELEPHONY;
	}
	nClass = MmiBsCode;

	switch (OpCode)
	{
		case SS_OP_REGISTERSS:
		case SS_OP_ACTIVATESS:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsSetFacilityLock(nFac, pPassword, PasswordLength, nClass, CFW_CM_MODE_LOCK, FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsSetFacilityLock(nFac, pPassword, PasswordLength, nClass, CFW_CM_MODE_LOCK, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsSetFacilityLock(nFac, pPassword, PasswordLength, nClass, CFW_CM_MODE_LOCK, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallBarring()   CFW_SsSetFacilityLock  ret = %d", ret);
				rsp_ptr = OslMalloc(sizeof(mmi_ss_call_barring_rsp_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_call_barring_rsp_struct));

				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_BARRING_RSP, ADP_SS_MSG_SRC_MOD);
			}
			break;

		case SS_OP_ERASESS:
		case SS_OP_DEACTIVATESS:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsSetFacilityLock(nFac, pPassword, PasswordLength, nClass, CFW_CM_MODE_UNLOCK, FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsSetFacilityLock(nFac, pPassword, PasswordLength, nClass, CFW_CM_MODE_UNLOCK, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsSetFacilityLock(nFac, pPassword, PasswordLength, nClass, CFW_CM_MODE_UNLOCK, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallBarring()   CFW_SsSetFacilityLock  ret = %d", ret);
				rsp_ptr = OslMalloc(sizeof(mmi_ss_call_barring_rsp_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_call_barring_rsp_struct));

				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_BARRING_RSP, ADP_SS_MSG_SRC_MOD);
			}
			break;

		case SS_OP_INTERROGATESS:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsQueryFacilityLock(nFac, nClass, FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsQueryFacilityLock(nFac, nClass, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsQueryFacilityLock(nFac, nClass, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallBarring()   CFW_SsQueryFacilityLock  ret = %d", ret);
				rsp_ptr = OslMalloc(sizeof(mmi_ss_call_barring_rsp_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_call_barring_rsp_struct));

				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CALL_BARRING_RSP, ADP_SS_MSG_SRC_MOD);
			}
			break;
		default:
			//The program can not go here.
			mmi_trace(g_sw_ADP_SS, "ADP_SS_SetCallBarring Error OpCode,%d", OpCode);
			SS_Adaption_SendParseStringRSP(PARSE_STRING_FAILED);
			break;
	}
	TBM_EXIT(0x29fa);
}

/*==========================================================================
 * This function is used to process SS REGISTER REQ to SSCode related REQ.
 *=========================================================================*/
static void SS_RegisterREQ(UINT8 *pStr, SSParsedParameters *pParsed, ss_code_enum SSCode)
{
	BOOL MmiBsCodePresent = FALSE;
	Adp_bs_code_enum MmiBsCode;
	UINT8 PasswordLength = 0;
	UINT8 pwd[MAXPASSWORDLEN] = {0};
	BOOL TimePresent = 0; 
	INT8 Time;
	UINT8 NumberBCD[MAX_CC_ADDR_LEN] = {0};
	UINT8 BytesLenBCD = 0;

	ASSERT(pStr != NULL);
	ASSERT(pParsed != NULL);

	/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
	/* Copy the optional mmi basic service code */
	switch (SSCode)
	{
		case SS_CW:
			/* For call waiitng the BS is in second parameter */
			if (pParsed->parmLen[1] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &MmiBsCode);
			}

			/* Change to CSW request */
			ADP_SS_SetCallWaiting(SS_OP_REGISTERSS, MmiBsCodePresent, MmiBsCode);
			break;

		case SS_BAOC:
		case SS_BOIC:
		case SS_BOICEXHC:
		case SS_BAIC:
		case SS_BICROAM:
		case SS_ALLBARRINGSS:
		case SS_BARRINGOFOUTGOINGCALLS:
		case SS_BARRINGOFINCOMINGCALLS:
			/* The password is required for the call restriction services only */
			/* and will always be in the second paremeter */
			CopySubString(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &pwd[0], &PasswordLength, MAXPASSWORDLEN);

			/* For these BS is in the third parameter */
			if (pParsed->parmLen[2] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[2]], pParsed->parmLen[2], &MmiBsCode);
			}

			ADP_SS_SetCallBarring(SS_OP_REGISTERSS, SSCode, MmiBsCodePresent, MmiBsCode, PasswordLength, pwd);
			break;

		case SS_CFNRY:
		case SS_ALLFORWARDINGSS:
		case SS_ALLCONDFORWARDINGSS:
			/* The reply timeout is always in the fourth parameter */
			if (pParsed->parmLen[3] != 0)
			{
				/* A timer value has been supplied so try and decode it */
				TimePresent = ValidReplyTime(&pStr[pParsed->parmStart[3]], pParsed->parmLen[3], &Time);
			}
			else
			{
				TimePresent = FALSE;
				Time = 0;
			}
			if(SS_CFNRY == SSCode && Time == 0)
			{
				TimePresent = TRUE;
				Time = 5;
			}
			/*Don't break here, WARNING, WARNING, WARNING, WARNING*/
			/*Don't break here, WARNING, WARNING, WARNING, WARNING*/
		case SS_CFU:
		case SS_CFB:
		case SS_CFNRC:
			/* For these BS is in the third parameter */
			if (pParsed->parmLen[2] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[2]], pParsed->parmLen[2], &MmiBsCode);
			}

			memset(NumberBCD, 0, MAX_CC_ADDR_LEN);
			/* The fwd to number is in the second parameter  */
			if (pParsed->parmLen[1] != 0)
			{
				/* A forwarding number has been supplied so try and bcd encode it */
				if (strncmp(&pStr[pParsed->parmStart[1]], "+", 1) == 0)
				{
					SUL_AsciiToGsmBcd(&pStr[pParsed->parmStart[1] + 1], pParsed->parmLen[1] - 1, &NumberBCD[0]);
					CallForwardingFlag = TRUE;
				}
				else
				{
				SUL_AsciiToGsmBcd(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &NumberBCD[0]);
				}
				BytesLenBCD = pParsed->parmLen[1] / 2;
				BytesLenBCD += pParsed->parmLen[1] % 2;
			}

			ADP_SS_SetCallForward(SS_OP_REGISTERSS, SSCode, MmiBsCodePresent, MmiBsCode, TimePresent, Time, BytesLenBCD, NumberBCD);
			break;

		default:
			mmi_trace(g_sw_ADP_SS, "SS_RegisterREQ have any undeveloped SSCode %d", SSCode);
			SS_Adaption_SendParseStringRSP(PARSE_STRING_FAILED);
			break;
	}
}

/*==========================================================================
 * This function is used to process SS ERASE REQ to SSCode related REQ.
 *=========================================================================*/
static void SS_EraseREQ(UINT8 *pStr, SSParsedParameters *pParsed, ss_code_enum SSCode)
{
	BOOL MmiBsCodePresent = FALSE;
	Adp_bs_code_enum MmiBsCode;
	UINT8 PasswordLength = 0;
	UINT8 pwd[MAXPASSWORDLEN] = {0};

	ASSERT(pStr != NULL);
	ASSERT(pParsed != NULL);
	
	switch (SSCode)
	{
		case SS_CW:
			/* For call waiitng the BS is in parm 1 */
			if (pParsed->parmLen[1] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &MmiBsCode);
			}

			/* Change to CSW request */
			ADP_SS_SetCallWaiting(SS_OP_ERASESS, MmiBsCodePresent, MmiBsCode);
			break;

		case SS_BAOC:
		case SS_BOIC:
		case SS_BOICEXHC:
		case SS_BAIC:
		case SS_BICROAM:
		case SS_ALLBARRINGSS:
		case SS_BARRINGOFOUTGOINGCALLS:
		case SS_BARRINGOFINCOMINGCALLS:
			/* The password is required for the call restriction services only */
			CopySubString(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &pwd[0], &PasswordLength, MAXPASSWORDLEN);

			/* For these BS is in parm 2 */
			if (pParsed->parmLen[2] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[2]], pParsed->parmLen[2], &MmiBsCode);
			}

			ADP_SS_SetCallBarring(SS_OP_ERASESS, SSCode, MmiBsCodePresent, MmiBsCode, PasswordLength, pwd);
			break;

		case SS_CFU:
		case SS_CFB:
		case SS_CFNRY:
		case SS_CFNRC:
		case SS_ALLFORWARDINGSS:
		case SS_ALLCONDFORWARDINGSS:
			/* For these BS is in parm 2 */
			if (pParsed->parmLen[2] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[2]], pParsed->parmLen[2], &MmiBsCode);
			}
			else
			{   /* Basic Service code can be included in parm1 in CF erase */
				if (pParsed->parmLen[1] != 0)
				{
					/* A basic service code has been supplied so try and decode it */
					MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &MmiBsCode);
				}
			}

			ADP_SS_SetCallForward(SS_OP_ERASESS, SSCode, MmiBsCodePresent, MmiBsCode, FALSE, 0, 0, NULL);
			break;

		default:
			mmi_trace(g_sw_ADP_SS, "SS_EraseREQ have any undeveloped SSCode %d", SSCode);
			SS_Adaption_SendParseStringRSP(PARSE_STRING_FAILED);
			break;
	}
}

/*==========================================================================
 * This function is used to process SS ACTIVATE REQ to SSCode related REQ.
 *=========================================================================*/
static void SS_ActivateREQ(UINT8 *pStr, SSParsedParameters *pParsed, ss_code_enum SSCode)
{
	BOOL MmiBsCodePresent = FALSE;
	Adp_bs_code_enum MmiBsCode;
	UINT8 PasswordLength = 0;
	UINT8 pwd[MAXPASSWORDLEN] = {0};
	BOOL TimePresent = 0;
	INT8 Time;
	UINT8 NumberBCD[MAX_CC_ADDR_LEN] = {0};
	UINT8 BytesLenBCD = 0;

	ASSERT(pStr != NULL);
	ASSERT(pParsed != NULL);

	switch (SSCode)
	{
		case SS_CW:
			/* For call waiitng the BS is in parm 1 */
			if (pParsed->parmLen[1] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &MmiBsCode);
			}

			/* Change to CSW request */
			ADP_SS_SetCallWaiting(SS_OP_ACTIVATESS, MmiBsCodePresent, MmiBsCode);
			break;

		case SS_BAOC:
		case SS_BOIC:
		case SS_BOICEXHC:
		case SS_BAIC:
		case SS_BICROAM:
		case SS_ALLBARRINGSS:
		case SS_BARRINGOFOUTGOINGCALLS:
		case SS_BARRINGOFINCOMINGCALLS:
			/* The password is required for the call restriction services only */
			CopySubString(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &pwd[0], &PasswordLength, MAXPASSWORDLEN);

			/* For these BS is in parm 2 */
			if (pParsed->parmLen[2] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[2]], pParsed->parmLen[2], &MmiBsCode);
			}

			ADP_SS_SetCallBarring(SS_OP_ACTIVATESS, SSCode, MmiBsCodePresent, MmiBsCode, PasswordLength, pwd);
			break;

		case SS_CFNRY:
		case SS_ALLFORWARDINGSS:
		case SS_ALLCONDFORWARDINGSS:
			/* The reply timeout is always in the fourth parameter */
			if (pParsed->parmLen[3] != 0)
			{
				/* A timer value has been supplied so try and decode it */
				TimePresent = ValidReplyTime(&pStr[pParsed->parmStart[3]], pParsed->parmLen[3], &Time);
			}
			else
			{
				TimePresent = FALSE;
				Time = 0;
			}
			/*Don't break here, WARNING, WARNING, WARNING, WARNING*/
			/*Don't break here, WARNING, WARNING, WARNING, WARNING*/
		case SS_CFU:
		case SS_CFB:
		case SS_CFNRC:
			/* For these BS is in the third parameter */
			if (pParsed->parmLen[2] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[2]], pParsed->parmLen[2], &MmiBsCode);
			}

			memset(NumberBCD, 0, MAX_CC_ADDR_LEN);
			/* The fwd to number is in the second parameter  */
			if (pParsed->parmLen[1] != 0)
			{
				/* A forwarding number has been supplied so try and bcd encode it */
				if (strncmp(&pStr[pParsed->parmStart[1]], "+", 1) == 0)
				{
					SUL_AsciiToGsmBcd(&pStr[pParsed->parmStart[1] + 1], pParsed->parmLen[1] - 1, &NumberBCD[0]);
					CallForwardingFlag = TRUE;
				}
				else
				{
				SUL_AsciiToGsmBcd(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &NumberBCD[0]);
				}
				BytesLenBCD = pParsed->parmLen[1] / 2;
				BytesLenBCD += pParsed->parmLen[1] % 2;
			}
			ADP_SS_SetCallForward(SS_OP_ACTIVATESS, SSCode, MmiBsCodePresent, MmiBsCode, TimePresent, Time, BytesLenBCD, NumberBCD);
			break;

		default:
			mmi_trace(g_sw_ADP_SS, "SS_ActivateREQ have any undeveloped SSCode %d", SSCode);
			SS_Adaption_SendParseStringRSP(PARSE_STRING_FAILED);
			break;
	}
}

/*==========================================================================
 * This function is used to process SS DEACTIVATE REQ to SSCode related REQ.
 *=========================================================================*/
static void SS_DeactivateREQ(UINT8 *pStr, SSParsedParameters *pParsed, ss_code_enum SSCode)
{
	BOOL MmiBsCodePresent = FALSE;
	Adp_bs_code_enum MmiBsCode;
	UINT8 PasswordLength = 0;
	UINT8 pwd[MAXPASSWORDLEN] = {0};

	ASSERT(pStr != NULL);
	ASSERT(pParsed != NULL);

	switch (SSCode)
		{
		case SS_CW:
			/* For call waiitng the BS is in parm 1 */
			if (pParsed->parmLen[1] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &MmiBsCode);
			}

			/* Change to CSW request */
			ADP_SS_SetCallWaiting(SS_OP_DEACTIVATESS, MmiBsCodePresent, MmiBsCode);
			break;

		case SS_BAOC:
		case SS_BOIC:
		case SS_BOICEXHC:
		case SS_BAIC:
		case SS_BICROAM:
		case SS_ALLBARRINGSS:
		case SS_BARRINGOFOUTGOINGCALLS:
		case SS_BARRINGOFINCOMINGCALLS:
			/* The password is required for the call restriction services only */
			CopySubString(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &pwd[0], &PasswordLength, MAXPASSWORDLEN);

			/* For these BS is in parm 2 */
			if (pParsed->parmLen[2] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[2]], pParsed->parmLen[2], &MmiBsCode);
			}

			ADP_SS_SetCallBarring(SS_OP_DEACTIVATESS, SSCode, MmiBsCodePresent, MmiBsCode, PasswordLength, pwd);
			break;

		case SS_CFU:
		case SS_CFB:
		case SS_CFNRY:
		case SS_CFNRC:
		case SS_ALLFORWARDINGSS:
		case SS_ALLCONDFORWARDINGSS:
			/* For these BS is in parm 2 */
			if (pParsed->parmLen[2] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[2]], pParsed->parmLen[2], &MmiBsCode);
			}

			ADP_SS_SetCallForward(SS_OP_DEACTIVATESS, SSCode, MmiBsCodePresent, MmiBsCode, FALSE, 0, 0, NULL);
			break;

		default:
			mmi_trace(g_sw_ADP_SS, "SS_DeactivateREQ have any undeveloped SSCode %d", SSCode);
			SS_Adaption_SendParseStringRSP(PARSE_STRING_FAILED);
			break;
		}
}

/*==========================================================================
 * This function is used to process SS INTERROGATE REQ to SSCode related REQ.
 *=========================================================================*/
static void SS_InterrogateREQ(UINT8 *pStr, SSParsedParameters *pParsed, ss_code_enum SSCode)
{
	UINT32 ret;
	BOOL MmiBsCodePresent = FALSE;
	Adp_bs_code_enum MmiBsCode;
	UINT8 PasswordLength = 0;
	UINT8 pwd[MAXPASSWORDLEN] = {0};

	ASSERT(pStr != NULL);
	ASSERT(pParsed != NULL);

	switch (SSCode)
	{
		case SS_CW:
			/* For call waiitng the BS is in parm 1 */
			if (pParsed->parmLen[1] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &MmiBsCode);
			}

			/* Change to CSW request */
			ADP_SS_SetCallWaiting(SS_OP_INTERROGATESS, MmiBsCodePresent, MmiBsCode);
			break;

		case SS_BAOC:
		case SS_BOIC:
		case SS_BOICEXHC:
		case SS_BAIC:
		case SS_BICROAM:
		case SS_ALLBARRINGSS:
		case SS_BARRINGOFOUTGOINGCALLS:
		case SS_BARRINGOFINCOMINGCALLS:
			/* The password is required for the call restriction services only */
			CopySubString(&pStr[pParsed->parmStart[1]], pParsed->parmLen[1], &pwd[0], &PasswordLength, MAXPASSWORDLEN);

			/* For these BS is in parm 2 */
			if (pParsed->parmLen[2] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[2]], pParsed->parmLen[2], &MmiBsCode);
			}

			ADP_SS_SetCallBarring(SS_OP_INTERROGATESS, SSCode, MmiBsCodePresent, MmiBsCode, PasswordLength, pwd);
			break;

		case SS_CFU:
		case SS_CFB:
		case SS_CFNRY:
		case SS_CFNRC:
		case SS_ALLFORWARDINGSS:
		case SS_ALLCONDFORWARDINGSS:
			/* For these BS is in parm 2 */
			if (pParsed->parmLen[2] != 0)
			{
				/* A basic service code has been supplied so try and decode it */
				MmiBsCodePresent = ValidMmiBsCode(&pStr[pParsed->parmStart[2]], pParsed->parmLen[2], &MmiBsCode);
			}
			ADP_SS_SetCallForward(SS_OP_INTERROGATESS, SSCode, MmiBsCodePresent, MmiBsCode, FALSE, 0, 0, NULL);
			break;
			
		case SS_CLIP:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
			
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsQueryClip(FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsQueryClip(FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsQueryClip(FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_ss_clip_interrogate_ind_struct *rsp_ptr = NULL;
				mmi_trace(g_sw_ADP_SS, "SS_InterrogateREQ()   CFW_SsQueryClip  ret = %d", ret);
				
				rsp_ptr = OslMalloc(sizeof(mmi_ss_clip_interrogate_ind_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_clip_interrogate_ind_struct));

				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CLIP_INTERROGATE_IND, ADP_SS_MSG_SRC_MOD);
			}
			break;
			 
		case SS_CLIR:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
			
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret  = UT_CFW_SsQueryClir(FREE_UTI, g_pSSContext->sim_id);
#else
			ret  = CFW_SsQueryClir(FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret  = CFW_SsQueryClir(FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_ss_clir_interrogate_ind_struct *rsp_ptr = NULL;
				mmi_trace(g_sw_ADP_SS, "SS_InterrogateREQ()   CFW_SsQueryClir  ret = %d", ret);
				
				rsp_ptr = OslMalloc(sizeof(mmi_ss_clir_interrogate_ind_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_clir_interrogate_ind_struct));

				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CLIR_INTERROGATE_IND, ADP_SS_MSG_SRC_MOD);
			}
			break;
			 
		case SS_COLP:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
			
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsQueryColp(FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsQueryColp(FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsQueryColp(FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_ss_colp_interrogate_ind_struct *rsp_ptr = NULL;
				mmi_trace(g_sw_ADP_SS, "SS_InterrogateREQ()   CFW_SsQueryColp  ret = %d", ret);
				
				rsp_ptr = OslMalloc(sizeof(mmi_ss_colp_interrogate_ind_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_colp_interrogate_ind_struct));

				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_COLP_INTERROGATE_IND, ADP_SS_MSG_SRC_MOD);
			}
			break;
			 
		case SS_COLR:
			SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
			
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ret = UT_CFW_SsQueryColr(FREE_UTI, g_pSSContext->sim_id);
#else
			ret = CFW_SsQueryColr(FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
			ret = CFW_SsQueryColr(FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
			if (ret != ERR_SUCCESS)
			{
				mmi_ss_colr_interrogate_ind_struct *rsp_ptr = NULL;
				mmi_trace(g_sw_ADP_SS, "SS_InterrogateREQ()   CFW_SsQueryColr  ret = %d", ret);
				
				rsp_ptr = OslMalloc(sizeof(mmi_ss_colr_interrogate_ind_struct));
				ASSERT(rsp_ptr != NULL);
				memset(rsp_ptr, 0, sizeof(mmi_ss_colr_interrogate_ind_struct));

				rsp_ptr->result.flag = L4C_ERROR;
				rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
				Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_COLR_INTERROGATE_IND, ADP_SS_MSG_SRC_MOD);
			}
			break;

		default:
			mmi_trace(g_sw_ADP_SS, "SS_InterrogateREQ have any undeveloped SSCode %d", SSCode);
			SS_Adaption_SendParseStringRSP(PARSE_STRING_FAILED);
			break;
	}
}

/*==========================================================================
 * This function is used to process SS REGPASSWORD REQ to SSCode related REQ.
 *=========================================================================*/
static void SS_RegPasswordREQ(UINT8 *pStr, SSParsedParameters *pParsed, ss_code_enum SSCode)
{
	UINT32 ret;
	mmi_ss_call_barring_rsp_struct *rsp_ptr = NULL;
	UINT8 OldLength = 0;
	UINT8 OldPassword[MAXPASSWORDLEN] = {0};
	UINT8 NewLength1 = 0;
	UINT8 NewPassword1[MAXPASSWORDLEN] = {0};
	UINT8 NewLength2 = 0;
	UINT8 NewPassword2[MAXPASSWORDLEN] = {0};
	UINT16 nFac = 0;

	ASSERT(pStr != NULL);
	ASSERT(pParsed != NULL);

	/* Copy the first optional password after */
	/*For CSD MMI, the old password is in parameter 1*/
	CopySubString(&pStr[pParsed->parmStart[2]], pParsed->parmLen[2], OldPassword, &OldLength, MAXPASSWORDLEN);

	/* Copy the second optional password after */
	/*For CSD MMI, the new password1 is in parameter 2*/
	CopySubString(&pStr[pParsed->parmStart[3]], pParsed->parmLen[3], NewPassword1, &NewLength1, MAXPASSWORDLEN);

	/* Copy the third optional password after */
	CopySubString(&pStr[pParsed->parmStart[4]], pParsed->parmLen[4], NewPassword2, &NewLength2, MAXPASSWORDLEN);
	
	/*Indicate Parse string success.*/
	SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);

	/*If the two new passwords are not matching with each other, send response with L4C_ERROR flag.*/
	if (!((NewLength1 == NewLength2) && (memcmp(NewPassword1, NewPassword2, NewLength1) == 0)))
	{
		mmi_trace(g_sw_ADP_SS, "SS_RegPasswordREQ the two new passwords are not matching with each other");
		
		rsp_ptr = OslMalloc(sizeof(mmi_ss_call_barring_rsp_struct));
		ASSERT(rsp_ptr != NULL);
		memset(rsp_ptr, 0, sizeof(mmi_ss_call_barring_rsp_struct));

		rsp_ptr->result.flag = L4C_ERROR;
		rsp_ptr->result.cause = CM_SS_ERR_PW_REGISTRATIONFAILURE;
		Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CHANGE_PASSWORD_RSP, ADP_SS_MSG_SRC_MOD);
	}
	else
	{
		mmi_trace(g_sw_ADP_SS, "SS_RegPasswordREQ SSCode = %d", SSCode);
		switch (SSCode)
		{
			case SS_BAOC:
				nFac = CFW_STY_FAC_TYPE_AO;
				break;
				
			case SS_BOIC:
				nFac = CFW_STY_FAC_TYPE_OI;
				break;
				
			case SS_BOICEXHC:
				nFac = CFW_STY_FAC_TYPE_OX;
				break;
				
			case SS_BAIC:
				nFac = CFW_STY_FAC_TYPE_AI;
				break;
				
			case SS_BICROAM:
				nFac = CFW_STY_FAC_TYPE_IR;
				break;
				
			case SS_ALLBARRINGSS:
				nFac = CFW_STY_FAC_TYPE_AB;
				break;
				
			case SS_BARRINGOFOUTGOINGCALLS:
				nFac = CFW_STY_FAC_TYPE_AG;
				break;
				
			case SS_BARRINGOFINCOMINGCALLS:
				nFac = CFW_STY_FAC_TYPE_AC;
				break;
				
			case SS_ALLSS:
				nFac = CFW_STY_FAC_TYPE_AB;
				break;
				
			default:
				/*Register password must refer to a valid call barring service.*/
				/*So the program can not go here.*/
				ASSERT(0);
				break;
		}

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		ret = UT_CFW_SsChangePassword(nFac, OldPassword, OldLength, NewPassword1, NewLength1, FREE_UTI, g_pSSContext->sim_id);
#else
		ret = CFW_SsChangePassword(nFac, OldPassword, OldLength, NewPassword1, NewLength1, FREE_UTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
		ret = CFW_SsChangePassword(nFac, OldPassword, OldLength, NewPassword1, NewLength1, FREE_UTI);
#endif /* __ADP_MULTI_SIM__ */
		if (ret != ERR_SUCCESS)
		{
			mmi_trace(g_sw_ADP_SS, "SS_RegPasswordREQ()   CFW_SsChangePassword  ret = %d", ret);
			
			rsp_ptr = OslMalloc(sizeof(mmi_ss_call_barring_rsp_struct));
			ASSERT(rsp_ptr != NULL);
			memset(rsp_ptr, 0, sizeof(mmi_ss_call_barring_rsp_struct));

			rsp_ptr->result.flag = L4C_ERROR;
			rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
			Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SS_CHANGE_PASSWORD_RSP, ADP_SS_MSG_SRC_MOD);
		}
	}

}

/*==========================================================================
 * This function is used to process SS PROC_USS REQ to SSCode related REQ.
 *=========================================================================*/
U8 SS_ProcUssReq(UINT8 *pStr, UINT8 userLen)
{
	UINT32 ret;
	UINT8 u7bitLen ;
	UINT8 srcLen ;	
	UINT8 Data[MAX_DIGITS_USSD * ENCODING_LENGTH] = {0};

	ASSERT(pStr != NULL);

	//back up for ver 1
	memcpy(ussdstring.ussd_input, pStr, MAX_DIGITS_USSD);
	ussdstring.b_used = FALSE;

	/* copy the data */
	mmi_trace(g_sw_ADP_SS," Func: %s pStr:%s", __FUNCTION__,pStr);
	srcLen = strlen(pStr);
	u7bitLen  = SUL_Encode7Bit(pStr, Data, srcLen);
	u7bitLen  = (srcLen % 8) ? (srcLen * 7 / 8 + 1) : (srcLen * 7 / 8);
	Data[u7bitLen] = '\0';
	
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	ret = UT_CFW_SsSendUSSD(Data, u7bitLen, 3, 0x0F, gSSUTI, g_pSSContext->sim_id);	
#else
	ret = CFW_SsSendUSSD(Data, u7bitLen, 3, 0x0F, gSSUTI, g_pSSContext->sim_id);	
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
	ret = CFW_SsSendUSSD(Data, u7bitLen, 3, 0x0F, gSSUTI);	
#endif /* __ADP_MULTI_SIM__ */
	if (ret != ERR_SUCCESS)
	{
		mmi_trace(g_sw_ADP_SS, "SS_ProcUssReq()   CFW_CmSendUSSD  ret = %d", ret);

#if 0
		mmi_ss_ussd_rsp_struct *rsp_ptr = NULL;
		
		rsp_ptr = OslMalloc(sizeof(mmi_ss_ussd_rsp_struct));
		ASSERT(rsp_ptr != NULL);
		memset(rsp_ptr, 0, sizeof(mmi_ss_ussd_rsp_struct));

		rsp_ptr->result.flag = L4C_ERROR;
		rsp_ptr->result.cause = CM_UNASSIGNED_NUM;
		Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_MMI_SAT_SEND_USSD_STAGE2_RSP, ADP_SS_MSG_SRC_MOD);
#endif /* Guoj delete. It's not used now */

		return PARSE_STRING_FAILED;
	}
	
	mmi_trace(g_sw_ADP_SS, "SS pStr[0] = %x, pStr[1] = %x, pStr[2] = %x, pStr[3] = %x, pStr[4] = %x, pStr[5] = %x", pStr[0], pStr[1], pStr[2], pStr[3], pStr[4], pStr[5]);	
	mmi_trace(g_sw_ADP_SS, "SS Data[0] = %x, Data[1] = %x, Data[2] = %x, Data[3] = %x, Data[4] = %x, Data[5] = %x, u7bitLen = %d, option = %d, dec = %x", Data[0], Data[1], Data[2], Data[3], Data[4], Data[5], u7bitLen, 3, 0x0f);	
	/*Must add code here*/
	return PARSE_STRING_SUCESS;
}


/*==========================================================================
 * This function is used to process SS PROC_USS REQ to SSCode related REQ.
 *=========================================================================*/
U8 SS_ReSendProcUssV1Req()
{
	UINT32 ret;
	UINT8 srcLen ;	

	ussdstring.b_used = TRUE;

	mmi_trace(g_sw_ADP_SS,"USSD Func: %s ussd_input:%s", __FUNCTION__,ussdstring.ussd_input);
	srcLen = strlen(ussdstring.ussd_input);
	
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	ret = UT_CFW_SsSendUSSD(ussdstring.ussd_input, srcLen, 131, 0x0F, gSSUTI, g_pSSContext->sim_id);	
#else
	ret = CFW_SsSendUSSD(ussdstring.ussd_input, srcLen, 131, 0x0F, gSSUTI, g_pSSContext->sim_id);	
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
	ret = CFW_SsSendUSSD(ussdstring.ussd_input, srcLen, 131, 0x0F, gSSUTI);	
#endif /* __ADP_MULTI_SIM__ */
	if (ret != ERR_SUCCESS)
	{
		mmi_trace(g_sw_ADP_SS, "SS_ReSendProcUssV1Req()   CFW_CmSendUSSD  ret = %d", ret);
		return PARSE_STRING_FAILED;
	}
	/*Must add code here*/
	return PARSE_STRING_SUCESS;
}

/**************************************************************
	FUNCTION NAME		: AdaptSendUSSDREQ
	PURPOSE				: Parse PRT_SS_STRING_SEND_USSD_REQ signal,
					and then invoke corresponding function.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static void AdaptSendUSSDREQ(COS_EVENT* pCosEvent)
{
	ilm_struct *ilm_ptr = NULL;
	mmi_ss_parsing_string_req_struct *pReq = NULL;

	ASSERT(pCosEvent != NULL);

	ilm_ptr = (ilm_struct *)pCosEvent->nParam1;
	pReq = (mmi_ss_parsing_string_req_struct *)ilm_ptr->local_para_ptr;
	
	SS_ProcUssReq(pReq->input, pReq->length);
	SS_Adaption_SendParseStringRSP(PARSE_STRING_SUCESS);
}

/**************************************************************
	FUNCTION NAME		: AdaptSendUSSDREQ
	PURPOSE				: Parse PRT_SS_STRING_ABORT_REQ signal,
					and then invoke corresponding function.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
static void AdaptProcUssAbortReq(void)
{
	mmi_ss_abort_rsp_struct *rsp_ptr = NULL;
	UINT32 ret = 0;
	U8	abort_option = 0;

	rsp_ptr = OslMalloc(SIZEOF(mmi_ss_abort_rsp_struct));
	ASSERT(rsp_ptr != NULL);
	
	if (ussdstring.b_used)
	{
		abort_option = 130;  //for ver 1
	}
	else
	{
		abort_option = 2;
	}

        mmi_trace(g_sw_ADP_SS, "SS AdaptProcUssAbortReq gSSCode = %d", gSSCode);	
        
	if(gSSCode == SS_OP_PROCESSUNSTRUCTUREDSS_DATA)
	{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		ret = UT_CFW_SsSendUSSD(NULL, 0, abort_option, 0x0F, gSSUTI, g_pSSContext->sim_id);
#else
		ret = CFW_SsSendUSSD(NULL, 0, abort_option, 0x0F, gSSUTI, g_pSSContext->sim_id);
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
		ret = CFW_SsSendUSSD(NULL, 0, abort_option, 0x0F, gSSUTI);
#endif /* __ADP_MULTI_SIM__ */

		if(ret != ERR_SUCCESS)
		{
			rsp_ptr->result = FALSE;
			mmi_trace(g_sw_ADP_SS, "SS AdaptProcUssAbortReq ret = %x", ret);	
			Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_SS_STRING_ABORT_RSP, ADP_SS_MSG_SRC_MOD);	
		}
	}
	else
	{
		rsp_ptr->result = TRUE;
		Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, PRT_SS_STRING_ABORT_RSP, ADP_SS_MSG_SRC_MOD);	
	}
}



















/**************************************************************
 * Global Functions definitions
**************************************************************/

/**************************************************************
	FUNCTION NAME		: ADP_DispatchSsMsg
	PURPOSE				: Message Adaption for Supplementary
			Service.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
**************************************************************/
BOOL ADP_DispatchSsMsg(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);

	if (pCosEvent->nEventId >= MSG_ID_MMI_SS_GET_EMLPP_PRIORITY_REQ && pCosEvent->nEventId <= MSG_ID_MMI_SS_SET_CLIR_FLAG_RSP)
		return ADP_DispatchSsReqMsg(pCosEvent);
	
	else if (pCosEvent->nEventId >= EV_CFW_SS_RSP_BASE && pCosEvent->nEventId <= EV_CFW_SS_RSP_END_)
		return ADP_DispatchSsRspMsg(pCosEvent);
	else if (pCosEvent->nEventId == EV_CFW_SS_USSD_IND)
		return ADP_DispatchSsIndMsg(pCosEvent);
	else
		return FALSE;
}
/**************************************************************

	FUNCTION NAME		: ADP_DispatchSsIndMsg

  	PURPOSE				: Deal all the csw indication message
	INPUT PARAMETERS	: pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: BOOL

	REMARKS			: 

**************************************************************/
static BOOL ADP_DispatchSsIndMsg(COS_EVENT *pCosEvent)
{
    UINT32 nParam1;
    UINT32 nParam2;
    UINT32 nParam3;
    UINT8 nType;
    ASSERT(pCosEvent != NULL);
#ifdef __ADP_MULTI_SIM__
    adp_ConfigureContext(ADP_MODULE_SS, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif

    nParam1 = pCosEvent->nParam1;
    nParam2 = pCosEvent->nParam2;
    nParam3 = pCosEvent->nParam3;
    nType = HIUINT8(pCosEvent->nParam3);

    switch (pCosEvent->nEventId)
    {
        case EV_CFW_SS_USSD_IND:
        {  
            mmi_ss_ussd_rsp_struct *rsp_ussdptr = NULL;
            CFW_SS_USSD_IND_INFO *pUSSDInd = NULL;
            U8 ussd_dcs;
            U16 ussdlen = 0;
            U16 i = 0;
    
            ASSERT(pCosEvent != NULL);
            gSSUTI = HIUINT16(nParam2);;

            pUSSDInd = (CFW_SS_USSD_IND_INFO *)pCosEvent->nParam1;
            gSSCode = SS_OP_PROCESSUNSTRUCTUREDSS_DATA;
            mmi_trace(g_sw_ADP_SS, "SS EV_CFW_SS_USSD_IND nParam1= %x, nParam2 =%x, nParam3 = %x, pUSSDInd->nOption = %d"
            , pCosEvent->nParam1, pCosEvent->nParam2, pCosEvent->nParam3, pUSSDInd->nOption); 
            
            rsp_ussdptr = OslMalloc(SIZEOF(mmi_ss_ussd_rsp_struct));
            ASSERT(rsp_ussdptr != NULL);
            memset(rsp_ussdptr, 0, sizeof(mmi_ss_ussd_rsp_struct));

            rsp_ussdptr->op_code= pUSSDInd->nOption;

            ADP_USSDParseDCS(pUSSDInd->nDcs, &ussd_dcs);

            if (ussd_dcs == ADP_MSG_GSM_7 || ussd_dcs == ADP_MSG_RESERVED)  //7 bit code
            {
                ussdlen = SUL_Decode7Bit(pUSSDInd->pUsdString, rsp_ussdptr->ussd_str, pUSSDInd->nStingSize);
                for(i = 0; i < ussdlen; i ++)
                {
                    if(*(rsp_ussdptr->ussd_str + i) == 0)
                    {
                        *(rsp_ussdptr->ussd_str + i) = AT_CODE;
                    }
                }           
                rsp_ussdptr->ussd_str[ussdlen] = 0x00;
                rsp_ussdptr->dcs = MMI_8BIT_DCS;  //ascii coding
            }
            else if (ussd_dcs == ADP_MSG_8_BIT_DATA)
            {
                rsp_ussdptr->dcs = MMI_8BIT_DCS;
                memcpy(rsp_ussdptr->ussd_str, pUSSDInd->pUsdString, pUSSDInd->nStingSize);	
            }
            else if (ussd_dcs == ADP_MSG_UCS2)
            {
                rsp_ussdptr->dcs = MMI_UCS2_DCS;

                if(pUSSDInd->pUsdString[0] == 0x81)
                    rsp_ussdptr->dcs = MMI_UCS2_81;
                else if(pUSSDInd->pUsdString[0] == 0x82)
                    rsp_ussdptr->dcs = MMI_UCS2_82;

                memcpy(rsp_ussdptr->ussd_str, pUSSDInd->pUsdString, pUSSDInd->nStingSize);	
            }
            
            rsp_ussdptr->result.flag = L4C_OK;
            mmi_trace(g_sw_ADP_SS, "ussd_dcs = %d", ussd_dcs);
            Adp_SendMessageToMMI((local_para_struct *)rsp_ussdptr, PRT_USSD_IND, ADP_SS_MSG_SRC_MOD);
            adp_SAT_SENDUSSD_rsp(pCosEvent->nParam1, pCosEvent->nParam2, nType, 0);			
        }
        break;

        default:
            return FALSE;
    }

    return TRUE;    
}

/**************************************************************
	FUNCTION NAME		: SS_Adaption_ParseStringREQ
	PURPOSE				: Parse SS string.
	INPUT PARAMETERS	: mmi_ss_parsing_string_req_struct *pReq
**************************************************************/
BOOL SS_Adaption_ParseStringREQ(mmi_ss_parsing_string_req_struct *pReq)
{
	kal_uint8 resultFlag = PARSE_STRING_SUCESS;
	SSParsedParameters Parsed;
	l4_op_code_enum OpCode = 0;
	ss_code_enum SSCode = 0;

	ASSERT(pReq != NULL);

	/* parse the mmi string */
	resultFlag = ParseUserInput(pReq->input, pReq->length, &Parsed);
	if (resultFlag == PARSE_STRING_SUCESS)
	{
		/* Determine the type of operation and process accordingly */
		GetTypeOfOperation(pReq->input, &Parsed, &OpCode, &SSCode);
		gSSCode = OpCode;
		/* process accordingly */
		switch (OpCode)
		{
			case SS_OP_REGISTERSS:
				/*SS Register REQ, invoke CSW function.*/
				SS_RegisterREQ(pReq->input, &Parsed, SSCode);
				break;

			case SS_OP_ERASESS:
				/*SS Erase REQ, invoke CSW function.*/
				SS_EraseREQ(pReq->input, &Parsed, SSCode);
				break;

			case SS_OP_ACTIVATESS:
				/*SS Activate REQ, invoke CSW function.*/
				SS_ActivateREQ(pReq->input, &Parsed, SSCode);
				break;

			case SS_OP_DEACTIVATESS:
				/*SS Deactivate REQ, invoke CSW function.*/
				SS_DeactivateREQ(pReq->input, &Parsed, SSCode);
				break;

			case SS_OP_INTERROGATESS:
				/*SS Interrogate REQ, invoke CSW function.*/
				SS_InterrogateREQ(pReq->input, &Parsed, SSCode);
				break;

			case SS_OP_REGISTERPASSWORD:
				/*GSM2.30 V4.11.0 page 15 states that only registartion shall be allowed*/
				/*this means that process only if * or ** prefix in string           */
				if ((Parsed.type == SSMMI_STAR) || (Parsed.type == SSMMI_STARSTAR))
				{
					/*SS RegPassword REQ, invoke CSW function.*/
					SS_RegPasswordREQ(pReq->input, &Parsed, SSCode);
				}
				else
				{
					/*Indicate Parse string failed.*/
					SS_Adaption_SendParseStringRSP(PARSE_STRING_FAILED);
				}
				break;

			case SS_OP_PROCESSUNSTRUCTUREDSS_DATA:
				/*SS ProcUss REQ, invoke CSW function.*/
				resultFlag = SS_ProcUssReq(pReq->input, pReq->length);
				SS_Adaption_SendParseStringRSP(resultFlag);
				break;

			default:
				/* this should never happen */
				resultFlag = PARSE_STRING_FAILED;
				mmi_trace(g_sw_ADP_SS, "SS_Adaption_ParseStringREQ default resultFlag = %d", resultFlag);
				SS_Adaption_SendParseStringRSP(resultFlag);
				ASSERT(0);				
				break;
		}
	}
	else
	{
		SS_Adaption_SendParseStringRSP(resultFlag);
	}

	return TRUE;
}

/**************************************************************
	FUNCTION NAME		: SS_Adaption_SendParseStringRSP
	PURPOSE				: Send MSG_ID_MMI_SS_PARSING_STRING_RSP to
                CSD MMI task.
	INPUT PARAMETERS	: l4c_result_enum resultFlag
**************************************************************/
static void SS_Adaption_SendParseStringRSP(kal_uint8 resultFlag)
{
	mmi_ss_parsing_string_rsp_struct *rsp_ptr = (mmi_ss_parsing_string_rsp_struct *)OslMalloc(sizeof(mmi_ss_parsing_string_rsp_struct));

	ASSERT(rsp_ptr != NULL);
	memset(rsp_ptr, 0, sizeof(mmi_ss_call_wait_rsp_struct));

	mmi_trace(g_sw_ADP_SS, "SS_Adaption_SendParseStringRSP resultFlag = %d", resultFlag);
	if (resultFlag == PARSE_STRING_FAILED)
	{
		rsp_ptr->info.opcode = INVALID_OPERATION;
	}
	else
	{
		rsp_ptr->info.opcode = SS_OPERATION;
	}
	rsp_ptr->result.flag = resultFlag;

	Adp_SendMessageToMMI((local_para_struct *)rsp_ptr, MSG_ID_MMI_SS_PARSING_STRING_RSP, ADP_SS_MSG_SRC_MOD);

}

