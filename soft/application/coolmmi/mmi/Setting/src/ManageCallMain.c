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
 *	ManageCallMain.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   <file description>.
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
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/**************************************************************

	FILENAME	: ManageCall.c

  	PURPOSE		: CallSetup Main Menu 

 

	AUTHOR		: Vanita Jain

	DATE		: 

**************************************************************/
#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "csp.h"
#include "mmi_trace.h"
#include "mainmenudef.h"
#include "phonebooktypes.h"
 
#include "phonebookgprot.h"
 
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "callmanagementiddef.h"
#include "callmanagementstruct.h"
 
#ifdef MMI_ON_HARDWARE_P
#include "l4c_common_enum.h"
#endif
 
#include "callsetup.h"
#include "managecallmain.h"
#include "settingprot.h"
#include "settingdefs.h"
#include "callsetupenum.h"
#include "commonscreens.h"
#include "settinggprots.h"
#include "messagesexdcl.h"
#include "mmi_features.h"
#include "wgui_categories_inputs.h"
#include "settingdefs.h"
//micha0901
#include "profilegprots.h"
#include "wgui_categories_popup.h"

#include "callmanagementstruct.h"
#include "callmanagementgprot.h"
#include "callstructuremanagementprot.h"

#ifdef MAGIC_AE_SUPPORT
#ifdef MMI_ON_HARDWARE_P
#include "jtae.h"
#endif
#endif

#undef __NEWSIMULATOR 
#include "phonebookgprot.h"
#include "callsdefs.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#include "mtpnp_ad_resdef.h"
#endif

CallSetupContext g_callset_context[MMI_SIM_NUMBER];
CallSetupContext *g_callset_cntx_p = g_callset_context;


static U8 gCurrSoundWarnningIndex=0; //added by licheng for jasmine SoundWarnning

extern MMI_OWNER_NUMBER_STRUCT gDispOwnerNumber[MMI_SIM_NUMBER][2];
extern MMI_OWNER_NUMBER_STRUCT *g_pDispOwnerNumber;

#ifdef __IP_NUMBER__
static U8 IPSetting[MMI_SIM_NUMBER];
static U8 IPNumber[MAX_IP_NUMBER][(MAX_IP_NUMBER_LEN+1)*ENCODING_LENGTH];
static U8 newIPNum[(MAX_IP_NUMBER_LEN+1)*ENCODING_LENGTH];
static S32 curIPNumIndex = 0;
#endif
 
#ifdef __MMI_CM_BLACK_LIST__
static pBOOL gBlackListFlag=FALSE;
static U8 BlackListNum[MAX_BLACK_LIST_NUMBER][(MAX_BLACK_LIST_NUMBER_LEN+1)*ENCODING_LENGTH]; /* black list number stored in NVRAM */
static U8 dispBlackListNum[MAX_BLACK_LIST_NUMBER][(MAX_BLACK_LIST_NUMBER_LEN+1)*ENCODING_LENGTH]; /* temp black list number for displaying reason */
static S32 curBlackListNumIndex = 0;
#endif
 
#ifdef __MMI_SS_SHOW_CAUSE__
const SSCauseDisplayStruct CauseDisplay[] =
{
	{ CM_UNASSIGNED_NUM, "cm unassigned num"},
	{ CM_NO_ROUTE_TO_DESTINATION, "cm no route to destination"},
	{ CM_CHANNEL_UN_ACCP, "cm channel un accp"},
	{ CM_OPR_DTR_BARRING, "cm opr dtr barring"},
	{ CM_CALL_BARRED, "cm call barred"},
	{ CM_RESERVED, "cm reserved"},
	{ CM_NORMAL_CALL_CLR, "cm normal call clr"},
	{ CM_USER_BUSY, "cm user busy"},
	{ CM_NO_USER_RESPONDING, "cm no user responding"},
	{ CM_NO_ANSWER_ON_ALERT, "cm no answer on alert"},
	{ CM_CALL_REJECTED, "cm call rejected"},
	{ CM_NUMBER_CHANGED, "cm number changed"},
	{ CM_PRE_EMPTION, "cm pre emption"},
	{ CM_NON_SEL_USER_CLEAR, "cm non sel user clear"},
	{ CM_DEST_OUT_OF_ORDER, "cm dest out of order"},
	{ CM_INVALID_NUMBER_FORMAT, "cm invalid number format"},
	{ CM_FACILITY_REJECT, "cm facility reject"},
	{ CM_RES_STATUS_ENQ, "cm res status enq"},
	{ CM_NORMAL_UNSPECIFIED, "cm normal unspecified"},
	{ CM_NO_CIRCUIT_CHANNEL_AVAIL, "cm no circuit channel avail"},
	{ CM_NETWORK_OUT_OF_ORDER, "cm network out of order"},
	{ CM_TEMPORARY_FAILURE, "cm temporary failure"},
	{ CM_SWITCH_EQUIPMENT_CONGESTION, "cm switch equipment congestion"},
	{ CM_ACCESS_INFO_DISCARDED, "cm access info discarded"},
	{ CM_REQUESTED_CKT_CHANEL_NOT_AVIL, "cm requested ckt chanel not avil"},
	{ CM_RESOURCE_UNAVAIL_UNSPECIFIED, "cm resource unavail unspecified"},
	{ CM_QOS_UNAVAIL, "cm qos unavail"},
	{ CM_REQ_FAC_NOT_SUBS, "cm req fac not subs"},
	{ CM_IC_BAR_CUG, "cm ic bar cug"},
	{ CM_BEARER_CAP_NOT_AUTHORISED, "cm bearer cap not authorised"},
	{ CM_BEARER_CAP_NOT_AVAIL, "cm bearer cap not avail"},
	{ CM_SER_UNAVAILABLE, "cm ser unavailable"},
	{ CM_BEARER_SER_UNIMPL, "cm bearer ser unimpl"},
	{ CM_ACM_EXCEEDED, "cm acm exceeded"},
	{ CM_REQ_FACILITY_UNAVAIL, "cm req facility unavail"},
	{ CM_RESTR_DIGITAL_INFO, "cm restr digital info"},
	{ CM_SER_OPT_UNIMPL, "cm ser opt unimpl"},
	{ CM_INVALID_TI_VALUE, "cm invalid ti value"},
	{ CM_USER_NOT_IN_CUG, "cm user not in cug"},
	{ CM_INCOMPATIBLE_DEST, "cm incompatible dest"},
	{ CM_INVALID_TRANSIT_NW_SEL, "cm invalid transit nw sel"},
	{ CM_SEMANTIC_ERR, "cm semantic err"},
	{ CM_INVALID_MANDATORY_INF, "cm invalid mandatory inf"},
	{ CM_MSG_TYPE_UNIMPL, "cm msg type unimpl"},
	{ CM_MSG_TYPE_NOT_COMPATIBLE, "cm msg type not compatible"},
	{ CM_IE_NON_EX, "cm ie non ex"},
	{ CM_COND_IE_ERR, "cm cond ie err"},
	{ CM_INCOMP_MESG_WITH_STATE, "cm incomp mesg with state"},
	{ CM_RECOVERY_ON_TIMER_EXPIRY, "cm recovery on timer expiry"},
	{ CM_PROTOCOL_ERR_UNSPECIFIED, "cm protocol err unspecified"},
	{ CM_INTER_WRK_UNSPECIFIED, "cm inter wrk unspecified"},
	{ TELEMATIC_INT_WRK_NOT_SUPPORT, "telematic int wrk not support"},
	{ SMS_TYPE0_NOT_SUPPORT, "sms type0 not support"},
	{ CANNOT_REPLACE_MSG, "cannot replace msg"},
	{ UNSPECIFIED_PID_ERROR, "unspecified pid error"},
	{ ALPHABET_NOT_SUPPORT, "alphabet not support"},
	{ MSG_CLASS_NOT_SUPPORT, "msg class not support"},
	{ UNSPECIFIED_TP_DCS, "unspecified tp dcs"},
	{ CMD_CANNOT_ACTION, "cmd cannot action"},
	{ CMD_NOT_SUPPORT, "cmd not support"},
	{ UNSPECIFIED_CMD_ERROR, "unspecified cmd error"},
	{ TPDU_NOT_SUPPORT, "tpdu not support"},
	{ SC_BUSY, "sc busy"},
	{ NO_SC_SUBSCRIPTION, "no sc subscription"},
	{ SC_SYS_FAILURE, "sc sys failure"},
	{ INVALID_SME_ADDR, "invalid sme addr"},
	{ DEST_SME_BARRED, "dest sme barred"},
	{ SM_REJ_DUP_SM, "sm rej dup sm"},
	{ TP_VPF_NOT_SUPPORT, "tp vpf not support"},
	{ TP_VP_NOT_SUPPORT, "tp vp not support"},
	{ SIM_STORAGE_FULL, "sim storage full"},
	{ NO_SMS_STO_IN_SIM, "no sms sto in sim"},
	{ ERROR_IN_MS, "error in ms"},
	{ MEM_CAP_EXCEEDED, "mem cap exceeded"},
	{ SMS_SAT_BUSY, "sms sat busy"},
	{ SMS_SAT_DL_ERROR, "sms sat dl error"},
	{ SMS_CP_RETRY_EXCEED, "sms cp retry exceed"},
	{ SMS_RP_TR1M_TIMEOUT, "sms rp tr1m timeout"},
	{ SMS_CONNECTION_BROKEN, "sms connection broken"},
	{ UNSPECIFIED_TP_FCS, "unspecified tp fcs"},
	{ INVALID_PDU_MODE_PARA, "invalid pdu mode para"},
	{ INVALID_TEXT_MODE_PARA, "invalid text mode para"},
	{ SMS_SIM_FAILURE, "sms sim failure"},
	{ MEM_FAILURE, "mem failure"},
	{ INVALID_MEM_INDEX, "invalid mem index"},
	{ MEM_FULL, "mem full"},
	{ SCA_UNKNOWN, "sca unknown"},
	{ NO_CNMA_EXPECTED, "no cnma expected"},
	{ SMSAL_UNSPECIFIED_ERROR_CAUSE, "smsal unspecified error cause"},
	{ SMSAL_NO_ERROR, "smsal no error"},
	{ MSG_LEN_EXCEEDED, "msg len exceeded"},
	{ INVALID_REQ_PARAMETER, "invalid req parameter"},
	{ SMS_ME_STORAGE_FAILURE, "sms me storage failure"},
	{ SMSAL_INVALID_BEARER, "smsal invalid bearer"},
	{ SMSAL_INVALID_SERVICE_MODE, "smsal invalid service mode"},
	{ SMSAL_INVALID_STORAGE_TYPE, "smsal invalid storage type"},
	{ SMSAL_INVALID_MSG_FORMAT, "smsal invalid msg format"},
	{ TOO_MANY_MO_CONCAT_MSG, "too many mo concat msg"},
	{ SMSAL_NOT_READY, "smsal not ready"},
	{ SMSAL_NO_MO_SERVICE, "smsal no mo service"},
	{ NOT_SUPP_SR_CMD_IN_STORAGE, "not supp sr cmd in storage"},
	{ SMSAL_NOT_SUPPORT_MSG_TYPE, "smsal not support msg type"},
	{ CSMCC_CMD_NOT_ALLOW, "csmcc cmd not allow"},
	{ CSMCC_ILLEGAL_CALL_ID, "csmcc illegal call id"},
	{ CSMCC_CALL_ALLOC_FAIL, "csmcc call alloc fail"},
	{ CSMCC_BC_FILL_FAIL, "csmcc bc fill fail"},
	{ CSMCC_CALL_RE_EST, "csmcc call re est"},
	{ CSMCC_ILLEGAL_DTMF_TONE, "csmcc illegal dtmf tone"},
	{ CSMCC_ILLEGAL_BC, "csmcc illegal bc"},
	{ CSMCC_MODIFY_ACTUAL_MODE, "csmcc modify actual mode"},
	{ CSMCC_DATA_ACT_FAIL, "csmcc data act fail"},
	{ CSMCC_NO_RESPONSE_FROM_NW, "csmcc no response from nw"},
	{ CSMCC_CALL_ACCEPT_NOT_ALLOW, "csmcc call accept not allow"},
	{ CM_SS_ERR_UNKNOWNSUBSCRIBER, "cm ss err unknownsubscriber"},
	{ CM_SS_ERR_ILLEGALSUBSCRIBER, "cm ss err illegalsubscriber"},
	{ CM_SS_ERR_BEARERSERVICENOTPROVISIONED, "cm ss err bearerservicenotprovisioned"},
	{ CM_SS_ERR_TELESERVICENOTPROVISIONED, "cm ss err teleservicenotprovisioned"},
	{ CM_SS_ERR_ILLEGALEQUIPMENT, "cm ss err illegalequipment"},
	{ CM_SS_ERR_CALLBARRED, "cm ss err callbarred"},
	{ CM_SS_ERR_ILLEGALSS_OPERATION, "cm ss err illegalss operation"},
	{ CM_SS_ERR_SS_ERRORSTATUS, "cm ss err ss errorstatus"},
	{ CM_SS_ERR_SS_NOTAVAILABLE, "cm ss err ss notavailable"},
	{ CM_SS_ERR_SS_SUBSCRIPTIONVIOLATION, "cm ss err ss subscriptionviolation"},
	{ CM_SS_ERR_SS_INCOMPABILITY, "cm ss err ss incompability"},
	{ CM_SS_ERR_FACILITYNOTSUPPORTED, "cm ss err facilitynotsupported"},
	{ CM_SS_ERR_ABSENTSUBSCRIBER, "cm ss err absentsubscriber"},
	{ CM_SS_ERR_SHORTTERMDENIAL, "cm ss err shorttermdenial"},
	{ CM_SS_ERR_LONGTERMDENIAL, "cm ss err longtermdenial"},
	{ CM_SS_ERR_SYSTEMFAILURE, "cm ss err systemfailure"},
	{ CM_SS_ERR_DATAMISSING, "cm ss err datamissing"},
	{ CM_SS_ERR_UNEXPECTEDDATAVALUE, "cm ss err unexpecteddatavalue"},
	{ CM_SS_ERR_PW_REGISTRATIONFAILURE, "cm ss err pw registrationfailure"},
	{ CM_SS_ERR_NEGATIVEPW_CHECK, "cm ss err negativepw check"},
	{ CM_SS_ERR_NUMBEROFPW_ATTEMPTSVIOLATION, "cm ss err numberofpw attemptsviolation"},
	{ CM_SS_ERR_POSITIONMETHODFAILURE, "cm ss err positionmethodfailure"},
	{ CM_SS_ERR_UNKNOWNALPHABET, "cm ss err unknownalphabet"},
	{ CM_SS_ERR_USSD_BUSY, "cm ss err ussd busy"},
	{ CM_SS_ERR_REJECTEDBYUSER, "cm ss err rejectedbyuser"},
	{ CM_SS_ERR_REJECTEDBYNETWORK, "cm ss err rejectedbynetwork"},
	{ CM_SS_ERR_DEFLECTIONTOSERVEDSUBSCRIBER, "cm ss err deflectiontoservedsubscriber"},
	{ CM_SS_ERR_SPECIALSERVICECODE, "cm ss err specialservicecode"},
	{ CM_SS_ERR_INVALIDDEFLECTIONTONUMBER, "cm ss err invaliddeflectiontonumber"},
	{ CM_SS_ERR_MAXNUMBEROFMPTY_PARTICIPANTSEXCEEDED, "cm ss err maxnumberofmpty participantsexceeded"},
	{ CM_SS_ERR_RESOURCESNOTAVAILABLE, "cm ss err resourcesnotavailable"},
	{ CM_SS_GENERALPROBLEM_UNRECOGNIZEDCOMPONENT, "cm ss generalproblem unrecognizedcomponent"},
	{ CM_SS_GENERALPROBLEM_MISTYPEDCOMPONENT, "cm ss generalproblem mistypedcomponent"},
	{ CM_SS_GENERALPROBLEM_BADLYSTRUCTUREDCOMPONENT, "cm ss generalproblem badlystructuredcomponent"},
	{ CM_SS_INVOKEPROBLEM_DUPLICATEINVOKEID, "cm ss invokeproblem duplicateinvokeid"},
	{ CM_SS_INVOKEPROBLEM_UNRECOGNIZEDOPERATION, "cm ss invokeproblem unrecognizedoperation"},
	{ CM_SS_INVOKEPROBLEM_MISTYPEDPARAMETER, "cm ss invokeproblem mistypedparameter"},
	{ CM_SS_INVOKEPROBLEM_RESOURCELIMITATION, "cm ss invokeproblem resourcelimitation"},
	{ CM_SS_INVOKEPROBLEM_INITIATINGRELEASE, "cm ss invokeproblem initiatingrelease"},
	{ CM_SS_INVOKEPROBLEM_UNRECOGNIZEDLINKDID, "cm ss invokeproblem unrecognizedlinkdid"},
	{ CM_SS_INVOKEPROBLEM_LINKEDRESONSEUNEXPECTED, "cm ss invokeproblem linkedresonseunexpected"},
	{ CM_SS_INVOKEPROBLEM_UNEXPECTEDLINKEDOPERATION, "cm ss invokeproblem unexpectedlinkedoperation"},
	{ CM_SS_RETURNRESULTPROBLEM_RR_UNRECOGNIZEDINVOKEID, "cm ss returnresultproblem rr unrecognizedinvokeid"},
	{ CM_SS_RETURNRESULTPROBLEM_RR_RETURNRESULTUNEXPECTED, "cm ss returnresultproblem rr returnresultunexpected"},
	{ CM_SS_RETURNRESULTPROBLEM_RR_MISTYPEDPARAMETER, "cm ss returnresultproblem rr mistypedparameter"},
	{ CM_SS_RETURNERRORPROBLEM_RE_UNRECOGNIZEDINVOKEID, "cm ss returnerrorproblem re unrecognizedinvokeid"},
	{ CM_SS_RETURNERRORPROBLEM_RE_RETURNERRORUNEXPECTED, "cm ss returnerrorproblem re returnerrorunexpected"},
	{ CM_SS_RETURNERRORPROBLEM_RE_UNRECOGNIZEDERROR, "cm ss returnerrorproblem re unrecognizederror"},
	{ CM_SS_RETURNERRORPROBLEM_RE_UNEXPECTEDERROR, "cm ss returnerrorproblem re unexpectederror"},
	{ CM_SS_RETURNERRORPROBLEM_RE_MISTYPEDPARAMETER, "cm ss returnerrorproblem re mistypedparameter"},
	{ CM_MM_CAUSE_NONE, "cm mm cause none"},
	{ CM_MM_IMSI_UNKNOWN_IN_HLR, "cm mm imsi unknown in hlr"},
	{ CM_MM_ILLEGAL_MS, "cm mm illegal ms"},
	{ CM_MM_IMSI_UNKNOWN_IN_VLR, "cm mm imsi unknown in vlr"},
	{ CM_MM_IMEI_NOT_ACCEPTED, "cm mm imei not accepted"},
	{ CM_MM_ILLEGAL_ME, "cm mm illegal me"},
	{ CM_MM_GPRS_NOT_ALLOWED, "cm mm gprs not allowed"},
	{ CM_MM_GPRS_NON_GPRS_NOT_ALLOWED, "cm mm gprs non gprs not allowed"},
	{ CM_MM_MS_ID_NOT_DERIVED_BY_NW, "cm mm ms id not derived by nw"},
	{ CM_MM_IMPLICIT_DETACH, "cm mm implicit detach"},
	{ CM_MM_PLMN_NOT_ALLOWED, "cm mm plmn not allowed"},
	{ CM_MM_LOCATION_AREA_NOT_ALLOWED, "cm mm location area not allowed"},
	{ CM_MM_ROAMING_AREA_NOT_ALLOWED, "cm mm roaming area not allowed"},
	{ CM_MM_GPRS_NOT_ALLOWED_IN_PLMN, "cm mm gprs not allowed in plmn"},
	{ CM_MM_NO_SUITABLE_CELLS_IN_LA, "cm mm no suitable cells in la"},
	{ CM_MM_MSC_TEMP_NOT_REACHABLE, "cm mm msc temp not reachable"},
	{ CM_MM_NETWORK_FAILURE, "cm mm network failure"},
	{ CM_MM_MAC_FAILURE, "cm mm mac failure"},
	{ CM_MM_SYNC_FAILURE, "cm mm sync failure"},
	{ CM_MM_CONGESTION, "cm mm congestion"},
	{ CM_MM_SERV_OPTION_NOT_SUPPORTED, "cm mm serv option not supported"},
	{ CM_MM_REQ_SERV_OPTION_NOT_SUBSCRIBED, "cm mm req serv option not subscribed"},
	{ CM_MM_SERV_OPTION_TEMP_OUT_OF_ORDER, "cm mm serv option temp out of order"},
	{ CM_MM_CALL_CANNOT_BE_IDENTIFIED, "cm mm call cannot be identified"},
	{ CM_MM_NO_PDP_CONTEXT_ACTIVATED, "cm mm no pdp context activated"},
	{ CM_MM_RETRY_UPON_ENTRY_INTO_NEWCELL_MIN, "cm mm retry upon entry into newcell min"},
	{ CM_MM_RETRY_UPON_ENTRY_INTO_NEWCELL_MAX, "cm mm retry upon entry into newcell max"},
	{ CM_MM_SEMANTICALLY_INCORRECT_MSG, "cm mm semantically incorrect msg"},
	{ CM_MM_INVALID_MM_MAND_INFO, "cm mm invalid mm mand info"},
	{ CM_MM_MSG_TYPE_NON_EXISTENT, "cm mm msg type non existent"},
	{ CM_MM_MSG_TYPE_NOT_IMPLEMENTED, "cm mm msg type not implemented"},
	{ CM_MM_MSG_TYPE_INCOMPAT_WITH_PROTO_STATE, "cm mm msg type incompat with proto state"},
	{ CM_MM_IE_NOT_IMPLEMENTED, "cm mm ie not implemented"},
	{ CM_MM_CONDITIONAL_MM_IE_ERROR, "cm mm conditional mm ie error"},
	{ CM_MM_MSG_NOT_COMPAT_WITH_PROTO_STATE, "cm mm msg not compat with proto state"},
	{ CM_MM_PROTO_ERROR_UNSPECIFIED, "cm mm proto error unspecified"},
	{ CM_MM_ACCESS_BARRED, "cm mm access barred"},
	{ CM_MM_ASSIGNMENT_REJECT, "cm mm assignment reject"},
	{ CM_MM_RANDOM_ACCES_FAILURE, "cm mm random acces failure"},
	{ CM_MM_RR_NO_SERVICE, "cm mm rr no service"},
	{ CM_MM_RR_CONN_RELEASE, "cm mm rr conn release"},
	{ CM_MM_AUTH_FAILURE, "cm mm auth failure"},
	{ CM_MM_IMSI_DETACH, "cm mm imsi detach"},
	{ CM_MM_ABORT_BY_NW, "cm mm abort by nw"},
	{ CM_MM_CONN_TIMEOUT, "cm mm conn timeout"},
	{ CM_MM_CM_ENQUEUE_FAIL, "cm mm cm enqueue fail"},
	{ CM_MM_NOT_UPDATED, "cm mm not updated"},
	{ CM_MM_STATE_NOT_ALLOWED_CM, "cm mm state not allowed cm"},
	{ CM_MM_EMERGENCY_NOT_ALLOWED, "cm mm emergency not allowed"},
	{ CM_MM_NO_SERVICE, "cm mm no service"},
	{ CM_MM_ACCESS_CLASS_BARRED, "cm mm access class barred"},
	{ SIM_CAUSE_CMD_SUCCESS, "sim cause cmd success"},
	{ SIM_CAUSE_CMD_FAIL, "sim cause cmd fail"},
	{ SIM_CAUSE_FATAL_ERROR, "sim cause fatal error"},
	{ SIM_CAUSE_NO_INSERTED, "sim cause no inserted"},
	{ SIM_CAUSE_CHV_NOT_INIT, "sim cause chv not init"},
	{ SIM_CAUSE_CHV_VERIFY_ERROR, "sim cause chv verify error"},
	{ SIM_CAUSE_CHV_BLOCK, "sim cause chv block"},
	{ SIM_CAUSE_ACCESS_NOT_ALLOW, "sim cause access not allow"},
	{ SIM_CAUSE_SAT_CMD_BUSY, "sim cause sat cmd busy"},
	{ SIM_CAUSE_DL_ERROR, "sim cause dl error"},
	{ SIM_CAUSE_MEMORY_PROBLEM, "sim cause memory problem"},
	{ SIM_CAUSE_TECHNICAL_PROBLEM, "sim cause technical problem"},
	{ SIM_CAUSE_PUK_UNLOCK, "sim cause puk unlock"},
	{ (U16) NULL, ""}
};

#endif

#ifdef MAGIC_AE_SUPPORT
typedef enum
{
	MAGIC_SOUND_OFF=0,
	MAGIC_SOUND_MAN_TO_WOMAN,
	MAGIC_SOUND_WOMAN_TO_MAN,
	MAGIC_SOUND_MAX
}MAGIC_SOUND_STATE;

U16 MagicSoundStateStrID[MAGIC_SOUND_MAX] =
{
	STR_MAGIC_SOUND_OFF,
	STR_MAGIC_SOUND_MAN_TO_WOMAN,
	STR_MAGIC_SOUND_WOMAN_TO_MAN
};
extern INT32 vois_Set_MorphVoice( MAGIC_SOUND_STATE iValue);//jtAEParam nAeID


void CallsetSetMagicSound(U8 state)
{
	//call magic sound setting api according to current state
	#ifdef MMI_ON_HARDWARE_P
	vois_Set_MorphVoice(state);	
	#endif
	
	mmi_trace(1, "CallsetSetMagicSound, state is %d",state);
}
void InitMagicSound(void)
{
	U8 data;
	S16 error;
	ReadValue( NVRAM_SETTING_MAGIC_SOUND_STATE, &data, DS_BYTE , &error);
	if (data == 0xFF)
		data=0;
	g_callset_cntx_p->MagicSoundState= data;
	mmi_trace(1, "InitMagicSound, current state is %d", g_callset_cntx_p->MagicSoundState);
	///CallsetSetMagicSound(g_callset_cntx_p->MagicSoundState);
}

void CallsetMagicSoundHiliteHandler(S32 index)
{
	g_callset_cntx_p->MagicSoundIndex = (U8)index;
}

void CallsetMagicSoundLSKHandler(void)
{
	U8 data;
	S16 error;

	data = g_callset_cntx_p->MagicSoundState= g_callset_cntx_p->MagicSoundIndex;
	WriteValue( NVRAM_SETTING_MAGIC_SOUND_STATE, &data, DS_BYTE , &error);
	
	//CallsetSetMagicSound(g_callset_cntx_p->MagicSoundState);
	
	mmi_trace(1, "CallsetMagicSoundLSKHandler, user select is %d", g_callset_cntx_p->MagicSoundState);
	
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
	DeleteNHistory(1);

}

void EntryCallsetMagicSound(void)
{
	U8* pGuiBuffer;
	S32 totalIndex=MAGIC_SOUND_MAX;
	S32 i;
	U16 hiliteItem = g_callset_cntx_p->MagicSoundState;

	EntryNewScreen(SCR_SETTING_MAGIC_SOUND, NULL, EntryCallsetMagicSound, NULL);

	pGuiBuffer = GetCurrGuiBuffer(SCR_SETTING_MAGIC_SOUND);

	RegisterHighlightHandler(CallsetMagicSoundHiliteHandler);

	for(i = 0; i < MAGIC_SOUND_MAX; i ++)
	{
		subMenuDataPtrs[i] = (PU8)GetString(MagicSoundStateStrID[i]); 
	}

	ShowCategory36Screen(  STR_SETTING_CALL_MAGIC_SOUND, MAIN_MENU_TITLE_SETTINGS_ICON,
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						totalIndex, (U8**)subMenuDataPtrs, hiliteItem, pGuiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(CallsetMagicSoundLSKHandler, KEY_EVENT_UP);
	SetKeyHandler(CallsetMagicSoundLSKHandler, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
void HighlightCALLSETMagicSoud(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	SetLeftSoftkeyFunction(EntryCallsetMagicSound,KEY_EVENT_UP);
	SetKeyHandler(EntryCallsetMagicSound, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}
void HintCALLSETMagicSound(U16 index)
{
	//set hint data according to magic sound state
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(MagicSoundStateStrID[g_callset_cntx_p->MagicSoundState]));
}
#endif
#ifdef __MMI_AUTO_PLUS_ENABLE__
void InitAutoPlus(void)
{
	U8 data;
	S16 error;
	ReadValue( NVRAM_SETTING_AUTO_PLUSE_STATE, &data, DS_BYTE , &error);
	if (data == 0xFF)
		data=0;
	g_callset_cntx_p->AutoPlusState= data;
	mmi_trace(1, "InitAutoPlus, current state is %d", g_callset_cntx_p->AutoPlusState);
}

void	vois_SetAGC(bool);

void CALLSETsetAutoPlus(U8 state)
{
	//call auto plus setting api according to current state
	#ifdef MMI_ON_HARDWARE_P
	switch(state)
	{
		case 0://close
			vois_SetAGC(FALSE);
			break;
		case 1://open
			vois_SetAGC(TRUE);		
			break;
		default:
			break;
	}
	#endif
	mmi_trace(1, "CALLSETsetAutoPlus, state is %d",state);
}

void EnableDisableAutoPlus(void)
{
	U8 data;
	S16 error;
	if(g_callset_cntx_p->AutoPlusState)
	{
		g_callset_cntx_p->AutoPlusState = 0;
	}
	else
	{
		g_callset_cntx_p->AutoPlusState = 1;
	}
	
	data = g_callset_cntx_p->AutoPlusState;
	
	WriteValue( NVRAM_SETTING_AUTO_PLUSE_STATE, &data, DS_BYTE , &error);
	
	mmi_trace(1, "EnableDisableAutoPlus, current state is %d", g_callset_cntx_p->AutoPlusState);
	
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
	
}
void HighlightCALLSETAutoPlus(void)
{
	if(g_callset_cntx_p->AutoPlusState)
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF, 0);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON, 0);
	}
	SetLeftSoftkeyFunction(EnableDisableAutoPlus, KEY_EVENT_UP);
	SetKeyHandler(EnableDisableAutoPlus, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
void HintCALLSETAutoPlus(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if(g_callset_cntx_p->AutoPlusState)
	{
		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
	}
	else
	{
		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
	}
}
#endif
/**************************************************************

	FUNCTION NAME		: populateManageCalls(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void HighlightPhnsetLang(void);

void InitManageCalls()
{
#ifdef __MMI_MULTI_SIM__
	SetCallSetContext(MMI_SIM_1);
#endif
	InitCallerId();
	InitCallWaiting();
	InitCallForwarding();
	InitCallBarring();
	InitLineSwitch();
	InitGroupListApp();

	SetHiliteHandler(MENU8237_SCR8093_MNGCALL_MENU_MAIN,HighlightManageCalls);
	SetHiliteHandler(MENU_SETTING_CALL_REMINDER,HighlightCALLSETCallTimeReminder);
	SetHiliteHandler(MENU_SETTING_CALL_TIME_DISPLAY,  HighlightCALLSETCallTimeDisplay);
	SetHiliteHandler(MENU9140_AUTO_REDIAL,  HighlightCALLSETAutoRedial);

#ifdef MAGIC_AE_SUPPORT
	InitMagicSound();
	SetHiliteHandler(MENU_CALL_MAGIC_SOUND, HighlightCALLSETMagicSoud);
	SetHintHandler(MENU_CALL_MAGIC_SOUND,HintCALLSETMagicSound);
#endif

#ifdef __MMI_AUTO_PLUS_ENABLE__
	InitAutoPlus();
	SetHiliteHandler(MENU_CALL_AUTO_PLUS, HighlightCALLSETAutoPlus);
	SetHintHandler(MENU_CALL_AUTO_PLUS, HintCALLSETAutoPlus);
#endif

//micha0908
#ifndef  __MMI_REMOVE_CUG__	
	SetHiliteHandler(MENU_SETTING_CALL_CLOSED_GP,HighlightCALLSETClosedUserGroup);
#endif

	SetHintHandler(MENU_CALLER_ID		,HintCALLSETCallerID);
	SetHintHandler(MENU_LINE_SWCH		,HintCALLSETLineSwitch);
	SetHintHandler(MENU_SETTING_CALL_REMINDER		,HintCALLSETCallTimeReminder);
	SetHintHandler(MENU_SETTING_CALL_TIME_DISPLAY		,HintCALLSETCallTimeDisplay);
	SetHintHandler(MENU9140_AUTO_REDIAL		,HintCALLSETAutoRedial);

#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CallSetting_Init();
#endif

	g_callset_cntx_p->SSCFNumFromPHB = FALSE;


	#ifdef __IP_NUMBER__
	SetHiliteHandler(MENU_SETTING_IP_NUMBER, HighlightIPNumber);
	InitIPNumber();
	#endif
	 
	#ifdef __MMI_CM_BLACK_LIST__
	SetHiliteHandler(MENU_SETTING_BLACK_LIST, HighlightBlackList);
	SetHintHandler(MENU_SETTING_BLACK_LIST, HintBlackList);
	InitBlackList();
	#endif
	 

  //Start by Licheng
  InitSoundWarnningTime();
  SetHiliteHandler(MENU_CALL_SETTING_SOUND_WARNING, HighlightCallsetSoundWarnning);	
  SetHintHandler(MENU_CALL_SETTING_SOUND_WARNING		,HintCALLSETSoundWarnningTime);
  
}
void InitSoundWarnningTime(void)
{
  U8 data;
  S16 error;
  ReadValue( NVRAM_SETTING_SOUND_WARNNING_TIME, &data, DS_BYTE , &error);
  if (data > 3)
    data=0;
  gCurrSoundWarnningIndex = data;
  g_callset_cntx_p->SoundWarnningIndex=data;
}
void HintCALLSETSoundWarnningTime(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	switch(gCurrSoundWarnningIndex)
	{
	case 0 :
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_SETTING_CLOSE));
		break;
	case 1:
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_SETTING_1_MINIUTES));
		break;
	case 2:
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_SETTING_5_MINIUTES));
		break;
  case 3:
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_SETTING_10_MINITES));
  default:
    mmi_trace(1,"HintCALLSETSoundWarnningTime####################################################################\n");
		break;

	}

}
/*gaosh Add Start For rstdefault Ver: TBM780_R_V1.0.7_20071129_1740  on 2007-12-7 17:8 */
BOOL CallsetRstDefault(void)
{
	S16 error1,error2,error3;
	U8 data_ON  	= 1;
	U8 data_OFF 	= 0;
	
	WriteValue( NVRAM_SETTING_CTR,  &data_OFF, DS_BYTE , &error1);
	WriteValue( NVRAM_SETTING_CALL_TIME_DISPLAY, &data_ON, DS_BYTE , &error2);
	WriteValue( NVRAM_BLACK_LIST_SETTING, &data_OFF, DS_BYTE, &error3);
	
	if((error1 == NVRAM_WRITE_SUCCESS)
		&&(error2 == NVRAM_WRITE_SUCCESS)
		&&(error3 == NVRAM_WRITE_SUCCESS))
		return TRUE;
	else
		return FALSE;
}
/*gaosh Add End  For rstdefault Ver: TBM780_R_V1.0.7_20071129_1740  on 2007-12-7 17:8 */
void HighlightCallsetSoundWarnning(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	SetLeftSoftkeyFunction(EntryCallsetSoundWarnning,KEY_EVENT_UP);
	SetKeyHandler(EntryCallsetSoundWarnning, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void CallsetSoundWarnningHiliteHandler(S32 index)
{
	g_callset_cntx_p->SoundWarnningIndex = (U8)index;
}

void EntryCallsetSoundWarnning(void)
{
	U8* pGuiBuffer;
	S32 totalIndex=4;
	U16 hiliteItem = gCurrSoundWarnningIndex;
//	U8 i;

	EntryNewScreen(SCR_SOUND_WARNNING_SELECTION, NULL, EntryCallsetSoundWarnning, NULL);

//	totalLang = (S32) GetLanguages(& (g_phnset_cntx_p->ppLang) );

	pGuiBuffer = GetCurrGuiBuffer(SCR_SOUND_WARNNING_SELECTION);

	RegisterHighlightHandler(CallsetSoundWarnningHiliteHandler);

  subMenuDataPtrs[0] = (PU8)GetString( STR_SETTING_CLOSE );
  subMenuDataPtrs[1] = (PU8)GetString( STR_SETTING_1_MINIUTES);
  subMenuDataPtrs[2] = (PU8)GetString( STR_SETTING_5_MINIUTES );
  subMenuDataPtrs[3] = (PU8)GetString( STR_SETTING_10_MINITES );

	ShowCategory36Screen(  STR_SETTING_SOUND_WARNING, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  totalIndex, (U8**)subMenuDataPtrs, hiliteItem, pGuiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(CallsetSoundWarnningLSKHandler, KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}

void CallsetSoundWarnningLSKHandler(void)
{

	U8 data;
	S16 error;
	U16 index;
//  mmi_trace(0,"CallsetSoundWarnningLSKHandler@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
  ReadValue( NVRAM_SETTING_SOUND_WARNNING_TIME, &data, DS_BYTE , &error);
  index = GetCurrHiliteID();
//  mmi_trace(0,"gCurrSoundWarnningIndex = %d g_callset_cntx_p->SoundWarnningIndex = %d, index = %d, data = %d\n",gCurrSoundWarnningIndex,g_callset_cntx_p->SoundWarnningIndex,index,data);

  if (data != g_callset_cntx_p->SoundWarnningIndex)
  {
    data = gCurrSoundWarnningIndex = g_callset_cntx_p->SoundWarnningIndex;
    WriteValue( NVRAM_SETTING_SOUND_WARNNING_TIME, &data, DS_BYTE , &error);

    //pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_LANGUAGESCR_AUTO)
    pfnUnicodeStrcpy((PS8)hintData[0],(PS8)GetString(STR_SETTING_CLOSE));
    pfnUnicodeStrcpy((PS8)hintData[1],(PS8)GetString(STR_SETTING_1_MINIUTES));
    pfnUnicodeStrcpy((PS8)hintData[2],(PS8)GetString(STR_SETTING_5_MINIUTES));
    pfnUnicodeStrcpy((PS8)hintData[3],(PS8)GetString(STR_SETTING_10_MINITES));
    
    Category52ChangeItemDescription(index,hintData[data]);
    RedrawCategoryFunction();
    if (GetMissedCallFlag())
    {
      SetSoundWarnningTimer();
    }
    /*Added to resolve the problem that the reminder sound warning still respond after close sound warning- for bug-12652 by liuxl on 20090617 */	
    if(gCurrSoundWarnningIndex==0)
    {
	ClearSoundWarnningTimer();
    }
    //RefreshNwProviderName(gPLMN);
  }

  DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
  DeleteNHistory(1);

}

static BOOL g_SettingSoundWarnning;
extern PROFILE gcurrentprofile;
void SoundWarnningProcess(void)
{
//  mmi_trace(0,"SoundWarnningProcess@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
  if (!IsSilentModeActivated() && !IsMeetingModeActivated()) 
  {  
    //modify to fix a problem the missed calls still have reminder warning tone if we have unread missed calls when switch off the Warning tone by liuxl on 20090610  
    //AudioPlayReq(gcurrentprofile.extraTone.warningTone,(U8)DEVICE_AUDIO_PLAY_ONCE); //added by jinzh:20070724
    /*modify the process to the missed call sound warning by liuxl on 20090624*/			
#if defined(__PROJECT_GALLITE_C01__)
    playRequestedTone(MESSAGE_TONE); 
#else    
    playRequestedTone(MISSED_CALL_WARNING_TONE); 
#endif
    g_SettingSoundWarnning = TRUE;
  }
  SetSoundWarnningTimer();
}
extern U16  StartMyTimer(U16 nTimerId, U32 nTimeDuration, oslTimerFuncPtr TimerExpiryFunction);
extern U16 StopMyTimer(U16 nTimerId);
void SetSoundWarnningTimer(void)
{
#if 0
  U8 data;
  S16 error;
  U32 timeDuration;
  ReadValue( NVRAM_SETTING_SOUND_WARNNING_TIME, &data, DS_BYTE , &error);
#endif 
  switch (gCurrSoundWarnningIndex)
  {
    case 0:   //closed
      ClearSoundWarnningTimer();
      break;
    case 1:   //1 minute
      StartMyTimer(CALL_SETTING_SOUND_WARNNING_TIMER,60*1000,(oslTimerFuncPtr)SoundWarnningProcess);
      break;
    case 2:   //5 minutes
      StartMyTimer(CALL_SETTING_SOUND_WARNNING_TIMER,5*60*1000,(oslTimerFuncPtr)SoundWarnningProcess);
      break;
    case 3:   //10 minutes
      StartMyTimer(CALL_SETTING_SOUND_WARNNING_TIMER,10*60*1000,(oslTimerFuncPtr)SoundWarnningProcess);
      break;
    default:
      mmi_trace(1,"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$SetSoundWarnningTimer wrong gCurrSoundWarnningIndex(%d) \n",gCurrSoundWarnningIndex);
  }
}

void ClearSoundWarnningTimer(void)
{
 // mmi_trace(0,"ClearSoundWarnningTimer@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
  if (g_SettingSoundWarnning)
  {
    g_SettingSoundWarnning = FALSE;
    AudioStopReq(gcurrentprofile.extraTone.warningTone);
  }
  StopMyTimer(CALL_SETTING_SOUND_WARNNING_TIMER);
}
void HintCALLSETCallerID(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	switch(g_callset_cntx_p->CallerID)
	{
	case 0 :
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_CLI_HIDE_ID));
		break;
	case 1:
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_CLI_SEND_ID));
		break;
	case 2:
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_CLI_SET_NETWORK));
		break;
	}

}

void HintCALLSETLineSwitch(U16 index)
{
#ifdef __MMI_LINE_SWITCH_SHOW_NUMBER__		
	S8 tempstr[10];
#endif
#ifdef __MMI_MULTI_SIM__
	g_pDispOwnerNumber = gDispOwnerNumber[0];
#endif
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if(g_callset_cntx_p->LineID == LINE1)
	{
		pfnUnicodeStrcpy((S8*)g_callset_cntx_p->LineInfo, ( S8*)GetString(STR_LSW_LINE1));
#ifdef __MMI_LINE_SWITCH_SHOW_NUMBER__		
		if(pfnUnicodeStrlen((S8*)g_pDispOwnerNumber[0].name) > 0) {
			AnsiiToUnicodeString((S8*)tempstr, (S8*)",");
			pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) tempstr);
			pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) g_pDispOwnerNumber[0].name);
		}
		else if (pfnUnicodeStrlen((S8*)g_pDispOwnerNumber[0].number) > 0) {
			AnsiiToUnicodeString((S8*)tempstr, (S8*)",");
			pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) tempstr);
			pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) g_pDispOwnerNumber[0].number);
		}
#endif		
	}
	else
	{
		pfnUnicodeStrcpy((S8*)g_callset_cntx_p->LineInfo, ( S8*)GetString(STR_LSW_LINE2));
#ifdef __MMI_LINE_SWITCH_SHOW_NUMBER__		
		if(pfnUnicodeStrlen((S8*)g_pDispOwnerNumber[1].name) > 0) {
			AnsiiToUnicodeString((S8*)tempstr, (S8*)",");
			pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) tempstr);
			pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) g_pDispOwnerNumber[1].name);
		}
		else if (pfnUnicodeStrlen((S8*)g_pDispOwnerNumber[1].number) > 0) {
			AnsiiToUnicodeString((S8*)tempstr, (S8*)",");
			pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) tempstr);
			pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) g_pDispOwnerNumber[1].number);
		}
#endif		
	}
	pfnUnicodeStrcpy((S8*)hintData[index],(PS8)g_callset_cntx_p->LineInfo);

}

void HintCALLSETCallTimeReminder(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	switch(CallTimeReminderStatus())
	{
	case 0 :
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));		
		break;
	case 1:
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_SETTING_CR_SING));		
		break;
	case 2:
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_SETTING_CR_PERIOD));		
		break;
	}
	
}

void HintCALLSETCallTimeDisplay(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if(GetCallTimeDisplay())
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_ON));						
	}
	else
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));						
	}
	
}

void HintCALLSETAutoRedial(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if(AutoRedialOnOffStatus())
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_ON));				
	}
	else
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));				
	}

}


/**************************************************************

	FUNCTION NAME		: HighlightManageCalls(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightManageCalls()
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	SetKeyHandler(EntryCALLSETMain, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetLeftSoftkeyFunction(EntryCALLSETMain,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: populateManageCalls(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#ifdef __MMI_MULTI_SIM__
void CALLSETHighLightHandler(S32 hiliteid)
{
	S32 hiliteItemID = 0;
	
	hiliteItemID = GetSeqItemId_Ext((U16)MENU8237_SCR8093_MNGCALL_MENU_MAIN, (S16)hiliteid);
	if(hiliteItemID >= MENU_MTPNP_SIM1_CALL_SETTING && hiliteItemID <= MENU_MTPNP_SIM4_CALL_SETTING)
		SetCallSetContext(hiliteItemID - MENU_MTPNP_SIM1_CALL_SETTING);
	
	ExecuteCurrHiliteHandler(hiliteid);
}
#endif
void EntryCALLSETMain()
{
	U16 nStrItemList[MAX_SUB_MENUS];/* Stores the strings id of submenus returned */
	U16 nNumofItem;		 /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;		/* Buffer holding history data */
	/*U32 maskingByte=0xFFFFFFFF;
	U32 menuItemId=0xFFFFFFFF;*/
	U8 *CallSetupType[MAX_SUB_MENUS];

		
		


					U16 ItemIcons[MAX_SUB_MENUS];


	EntryNewScreen(SCR_ID_CALLSET_MAIN, NULL, EntryCALLSETMain, NULL);


#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	MTPNP_AD_SetMenuHideUnhide(MENU_MTPNP_SIM1_CALL_SETTING);
#endif


	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_MAIN);	

	/* 3. Retrieve no of child of menu item to be displayed */
	 
	nNumofItem = GetNumOfChild_Ext(MENU8237_SCR8093_MNGCALL_MENU_MAIN);

	/* 4. Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU8237_SCR8093_MNGCALL_MENU_MAIN);
	
	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	 
	GetSequenceStringIds_Ext(MENU8237_SCR8093_MNGCALL_MENU_MAIN, nStrItemList);

	#ifdef __MMI_NUMBER_MENU__
	#else
	GetSequenceImageIds_Ext(MENU8237_SCR8093_MNGCALL_MENU_MAIN,ItemIcons);
	#endif
	/* 6 Set current parent id*/
	SetParentHandler(MENU8237_SCR8093_MNGCALL_MENU_MAIN);

	ConstructHintsList(MENU8237_SCR8093_MNGCALL_MENU_MAIN, CallSetupType);

	/* 7 Register highlight handler to be called in menu screen */
	#ifdef __MMI_MULTI_SIM__
	RegisterHighlightHandler(CALLSETHighLightHandler);
	#else
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	#endif
	

	/* 8 Display Category1 Screen */

 

	#ifdef __MMI_NUMBER_MENU__
	ShowCategory52Screen(STR_SCR8093_MNGCALL_MENU_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList,(U16 *)gIndexIconsImageList,
					  CallSetupType,
					  0,0,guiBuffer);
	#else
	ShowCategory52Screen(STR_SCR8093_MNGCALL_MENU_CAPTION, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList,ItemIcons,
						  CallSetupType,
						  0,0,guiBuffer);
	#endif

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);	
}


/**************************************************************

	FUNCTION NAME		: AppendBScodeString( U8 bscode_input, PS8 temp_string)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void AppendBScodeString( U8 bscode_input, PS8 temp_string)
{
	switch( (U8) bscode_input) {
	case L4_ALL_TELESERVICES:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_ALL_TELESERVICES));
		break;
	case L4_TELEPHONY:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_TELEPHONY));
		break;
	case L4_ALL_DATA_TELESERVICES:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_ALL_DATA_TELESERVICES));
		break;
	case L4_FACSIMILE_SERVICES:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_FACSIMILE_SERVICES));
		break;
	case L4_SHORT_MESSAGE_SERVICES:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_SHORT_MESSAGE_SERVICES));
		break;
	case L4_ALL_TELESERVICES_EXCEPT_SMS:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_ALL_TELESERVICES_EXCEPT_SMS));
		break;
	case L4_VOICE_GROUP_CALL_SERVICE:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_VOICE_GROUP_CALL_SERVICE));
		break;
	case L4_VOICE_BROADCAST_SERVICE:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_VOICE_BROADCAST_SERVICE));
		break;
	case L4_ALL_BEARER_SERVICES:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_ALL_BEARER_SERVICES));
		break;
	case L4_ALL_ASYNC_SERVICES:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_ALL_ASYNC_SERVICES));
		break;
	case L4_ALL_SYNC_SERVICES:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_ALL_SYNC_SERVICES));
		break;
	case L4_ALL_DATA_CIRCUIT_SYNC:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_ALL_DATA_CIRCUIT_SYNC));
		break;
	case L4_ALL_DATA_CIRCUIT_ASYNC:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_ALL_DATA_CIRCUIT_ASYNC));
		break;
	case L4_ALL_GPRS_BEARER_SERVICES:
		pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_L4_ALL_GPRS_BEARER_SERVICES));
		break;
	}
}




/**************************************************************

	FUNCTION NAME		: HighlightCALLSETAutoRedial(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETAutoRedial(void)
{
	g_callset_cntx_p->AutoRedialInput=(U8)GetCurrHiliteID();
	if(g_callset_cntx_p->AutoRedialStatus== 0)
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*1 Change left soft key icon and label */
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrAutoRedial,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/**************************************************************

	FUNCTION NAME		: EntryScrAutoRedial(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrAutoRedial(void)
{
	U8 data = 0;
	S16 error;

	/* Store the updated Value */

	if (g_callset_cntx_p->AutoRedialStatus == 0) {
	data = 1;
	}

	WriteValue( NVRAM_SETTING_AUTOREDIAL, &data, DS_BYTE , &error);
	if(data)
	{
 		pfnUnicodeStrcpy((S8*)hintData[g_callset_cntx_p->AutoRedialInput],(PS8)GetString(STR_GLOBAL_ON));
		Category52ChangeItemDescription(g_callset_cntx_p->AutoRedialInput,(U8*)GetString(STR_GLOBAL_ON));
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
		g_callset_cntx_p->AutoRedialStatus = 1;
	}
	else
	{
 		pfnUnicodeStrcpy((S8*)hintData[g_callset_cntx_p->AutoRedialInput],(PS8)GetString(STR_GLOBAL_OFF));
		Category52ChangeItemDescription(g_callset_cntx_p->AutoRedialInput,(U8*)GetString(STR_GLOBAL_OFF));
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
		g_callset_cntx_p->AutoRedialStatus = 0;
	}
	RedrawCategoryFunction();
	HighlightCALLSETAutoRedial();
}


/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallTimeDisplay(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallTimeDisplay(void)
{
	g_callset_cntx_p->CTDInput=(U8)GetCurrHiliteID();
	if(g_callset_cntx_p->CTDStatus == 1)
	{
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryCallTimeDisplay,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/**************************************************************

	FUNCTION NAME		: EntryCallTimeDisplay(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCallTimeDisplay(void)
{
	U8 data = 0;
	S16 error;

	if (g_callset_cntx_p->CTDStatus == 0) {
		data = 1;
	}
	WriteValue( NVRAM_SETTING_CALL_TIME_DISPLAY, &data, DS_BYTE , &error);
	if(data)
	{
 		pfnUnicodeStrcpy((S8*)hintData[g_callset_cntx_p->CTDInput],(PS8)GetString(STR_GLOBAL_ON));		
		Category52ChangeItemDescription(g_callset_cntx_p->CTDInput,(U8*)GetString(STR_GLOBAL_ON));
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
		g_callset_cntx_p->CTDStatus = 1;
	}
	else
	{
		pfnUnicodeStrcpy((S8*)hintData[g_callset_cntx_p->CTDInput],(PS8)GetString(STR_GLOBAL_OFF));		
		Category52ChangeItemDescription(g_callset_cntx_p->CTDInput,(U8*)GetString(STR_GLOBAL_OFF));
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
		g_callset_cntx_p->CTDStatus = 0;		
	}
	RedrawCategoryFunction();
	HighlightCALLSETCallTimeDisplay();
}

//micha0908
#ifndef  __MMI_REMOVE_CUG__	

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETClosedUserGroup(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETClosedUserGroup()
{
		/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	SetKeyHandler(EntryCALLSETCUGMain, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	SetLeftSoftkeyFunction(EntryCALLSETCUGMain,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

#endif

/**************************************************************

	FUNCTION NAME		: HighlightCALLSETCallTimeReminder(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETCallTimeReminder(void)
{
		/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	SetLeftSoftkeyFunction(EntryCALLSETCallTimeReminder,KEY_EVENT_UP);

//micha0424
	SetKeyHandler(EntryCALLSETCallTimeReminder, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

/**************************************************************

	FUNCTION NAME		: EntryCALLSETCallTimeReminder(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntryCALLSETCallTimeReminder(void)
{
	U16 nStrItemList[3]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
//micha1229
//	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */

	EntryNewScreen(SCR_ID_CALLSET_CTR, NULL, EntryCALLSETCallTimeReminder, NULL);


	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_CTR);	

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_SETTING_CALL_REMINDER);

	/* 4. Get attribute of menu to be displayed */
//	nDispAttribute = GetDispAttributeOfItem(MENU_SETTING_CALL_REMINDER);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_SETTING_CALL_REMINDER,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_SETTING_CALL_REMINDER);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(CtrHandler);

	/* 8 Display Category1 Screen */

	ShowCategory11Screen(  STR_SETTING_CALL_REMINDER, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  nNumofItem, nStrItemList,(U16)g_callset_cntx_p->CTRStatusInput, guiBuffer);
	
	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SendCtr, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(0, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/**************************************************************

	FUNCTION NAME		: CtrHandler(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

  	Author				: Vanita Jain

 

**************************************************************/

void CtrHandler(S32 index)
{
	g_callset_cntx_p->CTRStatusInput = (U8)index;
}



/**************************************************************

	FUNCTION NAME		: SendCtr(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/


void SendCtr(void)
{
	U8 data;
	S16 error;

	data = g_callset_cntx_p->CTRStatusInput;
	if(data == 0)
	{
		WriteValue( NVRAM_SETTING_CTR, &data, DS_BYTE , &error);
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),IMG_GLOBAL_ACTIVATED,1,ST_NOTIFYDURATION,SUCCESS_TONE);
//micha0706
		DeleteScreenIfPresent(SCR_ID_CALLSET_CTR);
	}
	else
	{
		EntryCALLSETCtrTime();
	}
}



/**************************************************************

	FUNCTION NAME		: EntryCALLSETCtrTime(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/


void EntryCALLSETCtrTime(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	U8* inputBuffer;
	
	EntryNewScreen(SCR_ID_CALLSET_CTR_TIME, ExitCALLSETCtrTime, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_CALLSET_CTR_TIME);
	inputBuffer = GetCurrInputBuffer(SCR_ID_CALLSET_CTR_TIME);
	
	if(inputBuffer==NULL ) {
		if ( 	g_callset_cntx_p->CTRStatusInput != 0 && coolsand_UI_atoi((UI_string_type)g_callset_cntx_p->CTRNumber) != 0
				&& g_callset_cntx_p->CTRStatus == g_callset_cntx_p->CTRStatusInput) {
#ifdef __COUNTDOWN_PERIODIC_CTR_MODE__
			if (g_callset_cntx_p->CTRStatusInput != 1) {
			S8 temp_buf[6];
			S32 ctr_time = 0;
			ctr_time = coolsand_UI_atoi((UI_string_type)g_callset_cntx_p->CTRNumber);
			if ((ctr_time <= 60) && (ctr_time >= 0)) {
				ctr_time = 60 - ctr_time;
				sprintf(temp_buf, "%d", ctr_time);
				memset(g_callset_cntx_p->CTRNumberInput,0,MAX_CTR_NUM_DIGITS*ENCODING_LENGTH);
				AnsiiToUnicodeString((PS8)g_callset_cntx_p->CTRNumberInput, temp_buf );
			}
			else {
				memset(g_callset_cntx_p->CTRNumberInput,0,6*ENCODING_LENGTH);
			}
			}
			else {
				pfnUnicodeStrcpy((S8*)g_callset_cntx_p->CTRNumberInput, ( S8*)g_callset_cntx_p->CTRNumber);
			}
#else
			pfnUnicodeStrcpy((S8*)g_callset_cntx_p->CTRNumberInput, ( S8*)g_callset_cntx_p->CTRNumber);
#endif
		}
		else {			
		memset(g_callset_cntx_p->CTRNumberInput,0,MAX_CTR_NUM_DIGITS*ENCODING_LENGTH);
		}
	}
	else
		pfnUnicodeStrcpy((S8*)g_callset_cntx_p->CTRNumberInput, ( S8*)inputBuffer);
	
	if(g_callset_cntx_p->CTRStatusInput == 1)
	{
		ShowCategory69Screen(STR_CTR_REM,MAIN_MENU_TITLE_SETTINGS_ICON,
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						STR_SETTING_CR_RANGE2,INPUT_TYPE_NUMERIC,g_callset_cntx_p->CTRNumberInput,
						5,guiBuffer);
	}
	else
	{
#ifdef __COUNTDOWN_PERIODIC_CTR_MODE__
		ShowCategory69Screen(STR_CTR_REM,MAIN_MENU_TITLE_SETTINGS_ICON,
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						STR_SETTING_CR_RANGE3,INPUT_TYPE_NUMERIC,g_callset_cntx_p->CTRNumberInput,
						3,guiBuffer);
#else
		ShowCategory69Screen(STR_CTR_REM,MAIN_MENU_TITLE_SETTINGS_ICON,
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						STR_SETTING_CR_RANGE1,INPUT_TYPE_NUMERIC,g_callset_cntx_p->CTRNumberInput,
						3,guiBuffer);
#endif
	}
	
	SetLeftSoftkeyFunction(EntryCtrTimeDone, KEY_EVENT_UP);
	SetCategory69RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}
/**************************************************************

	FUNCTION NAME		: EntryCtrTimeDone()

  	PURPOSE				: gets name input from user

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCtrTimeDone(void)
{
	U8 err=0;
	historyNode *History;
	
	if(g_callset_cntx_p->CTRStatusInput == 1)
	{
		if(coolsand_UI_atoi((UI_string_type)g_callset_cntx_p->CTRNumberInput) < 1 || 
			coolsand_UI_atoi((UI_string_type)g_callset_cntx_p->CTRNumberInput) > 3000)
		{
			err =1;
		}
	}
	else
	{
#ifdef __COUNTDOWN_PERIODIC_CTR_MODE__
		if(coolsand_UI_atoi((UI_string_type)g_callset_cntx_p->CTRNumberInput) < 0 || 
			coolsand_UI_atoi((UI_string_type)g_callset_cntx_p->CTRNumberInput) > 30)
#else
		if(coolsand_UI_atoi((UI_string_type)g_callset_cntx_p->CTRNumberInput) < 30 || 
			coolsand_UI_atoi((UI_string_type)g_callset_cntx_p->CTRNumberInput) > 60)
#endif			
		{
			err =1;
		}
	}
	if (pfnUnicodeStrlen((PS8)g_callset_cntx_p->CTRNumberInput) == 0) {
		err = 1;
	}
	
	if (err == 1)
	{
		err =0;
		DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_DONE) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		if(GetHistoryScrID(SCR_ID_CALLSET_CTR_TIME,&History)==ST_SUCCESS)
			{
				if(History->inputBuffer!=NULL)
					memset(History->inputBuffer,0,ENCODING_LENGTH);
				if(History->guiBuffer!=NULL)
					memset(History->guiBuffer,0,2);
			}
	}
	else
	{
		SendCtrTime();
	}
}
/**************************************************************

	FUNCTION NAME		: ExitCtrTimeDone(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitCtrTimeDone(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	S16 nHistory = 0;
	stopRequestedTone(WARNING_TONE);
	Scr.scrnID = SCR_SETTING_CTR_TIME_DONE;
	Scr.entryFuncPtr = EntryCtrTimeDone;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}

void SendCtrTime()
{
	S16 error;
	U16 time =0;

	U16 data;

	data = g_callset_cntx_p->CTRStatusInput;
	time = (U16)coolsand_UI_atoi((UI_string_type)g_callset_cntx_p->CTRNumberInput);

#ifdef __COUNTDOWN_PERIODIC_CTR_MODE__
	if (data == 2) {
		time = 60 - time;
	}
#endif

	WriteValue( NVRAM_SETTING_CTR, &data, DS_BYTE , &error);
	WriteValue( NVRAM_SETTING_CTR_TIME, &time, DS_SHORT , &error);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,0);
//micha0706
	DeleteScreenIfPresent(SCR_ID_CALLSET_CTR);
	DeleteScreenIfPresent(SCR_ID_CALLSET_CTR_TIME);
}


void ExitCALLSETCtrTime(void)
{
	history_t   Scr; /* Variable to hold the history data*/

	Scr.scrnID = SCR_ID_CALLSET_CTR_TIME;
	Scr.entryFuncPtr = EntryCALLSETCtrTime;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)g_callset_cntx_p->CTRNumberInput);
	
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}


#ifdef __MMI_SS_SHOW_CAUSE__

void ShowCauseString( U16 cause_number, PS8 temp_string)
{
	U8 i;
	S8 TempBuff[MAX_DISP_UCS2];

	pfnUnicodeStrcpy((PS8)temp_string,(PS8)GetString(STR_GLOBAL_NOT_DONE));
	for ( i = 0; CauseDisplay[i].cause_number != (U16) NULL; i++) {
		if ( cause_number == CauseDisplay[i].cause_number) {
			AnsiiToUnicodeString((PS8)TempBuff,(PS8)CauseDisplay[i].cause_string);
			pfnUnicodeStrcat( (PS8)temp_string, (PS8) TempBuff);
			return;
		}
	}
	pfnUnicodeStrcat((PS8)temp_string,(PS8)GetString(STR_UNKNOWN_CAUSE));
}

#endif

/**************************************************************

	FUNCTION NAME		: GetCtrStatus

  	PURPOSE				: Show whether PowerSaving Feature is on or off

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

	Author				: Vanita Jain

 

**************************************************************/

SETTING_CTR GetCtrStatus(void)
{
	U8 data = 0;
	S16 error;

	ReadValue( NVRAM_SETTING_CTR, &data, DS_BYTE , &error);
//micha1229
	return((SETTING_CTR) data);
}

/**************************************************************

	FUNCTION NAME		: GetCtrTime

  	PURPOSE				: Show whether PowerSaving Feature is on or off

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

	Author				: Vanita Jain

 

**************************************************************/

U16 GetCtrTime(void)
{
	S16 error;
	U16 time =0;

	ReadValue( NVRAM_SETTING_CTR_TIME, &time, DS_SHORT , &error);

	return time;

}

/**************************************************************

	FUNCTION NAME		: AutoRedialOnOffStatus

  	PURPOSE				: Show whether AutoRedial Feature is on or off

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

	Author				: Vanita Jain

 

**************************************************************/

MMI_BOOL AutoRedialOnOffStatus(void)
{
	U8 data = 0;
	S16 error;

	ReadValue( NVRAM_SETTING_AUTOREDIAL, &data, DS_BYTE , &error);

	if(data == 1) {
#ifdef __MMI_MULTI_SIM__
		g_callset_context[MMI_SIM_1].AutoRedialStatus = 1;
#else
		g_callset_cntx_p->AutoRedialStatus = 1;
#endif
		return MMI_TRUE;
	}
	else if(data == 0xff)
	{
		data =0;
		WriteValue( NVRAM_SETTING_AUTOREDIAL, &data, DS_BYTE , &error);
#ifdef __MMI_MULTI_SIM__
		g_callset_context[MMI_SIM_1].AutoRedialStatus = 0;
#else
		g_callset_cntx_p->AutoRedialStatus = 0;
#endif
		return MMI_FALSE;
	}
	else {
#ifdef __MMI_MULTI_SIM__
		g_callset_context[MMI_SIM_1].AutoRedialStatus = 0;
#else
		g_callset_cntx_p->AutoRedialStatus = 0;		
#endif
		return MMI_FALSE;
	}
}



/**************************************************************

	FUNCTION NAME		: CallTimeReminderOnOffStatus

  	PURPOSE				: Show whether AutoRedial Feature is on or off

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

	Author				: Vanita Jain

 

**************************************************************/

U8 CallTimeReminderStatus(void)
{
	U8 data = 0;
	U16 ctrTime = 0;
	S16 error;
	S8 temp_buf[6];

	ReadValue( NVRAM_SETTING_CTR, &data, DS_BYTE , &error);
	ReadValue( NVRAM_SETTING_CTR_TIME, &ctrTime, DS_SHORT , &error);

	if (ctrTime == 0xff)
	{
		ctrTime = 0;
		WriteValue( NVRAM_SETTING_CTR_TIME, &ctrTime, DS_SHORT , &error);
	}

	if(data == 0xff)
	{
		data =0;
		WriteValue( NVRAM_SETTING_CTR, &data, DS_BYTE , &error);
	}
#ifdef __MMI_MULTI_SIM__
	g_callset_cntx_p = &g_callset_context;//guoyt modify for bug 19100(arose by multi sim app)[proj g800]@20101216
	g_callset_context[MMI_SIM_1].CTRStatusInput = data;
	g_callset_context[MMI_SIM_1].CTRStatus = data;
#else
	g_callset_cntx_p->CTRStatusInput = data;
	g_callset_cntx_p->CTRStatus = data;
#endif

	if ( ctrTime == 0 || data == 0 ) 
	{
#ifdef __MMI_MULTI_SIM__
		memset(g_callset_context[MMI_SIM_1].CTRNumberInput,0,MAX_CTR_NUM_DIGITS*ENCODING_LENGTH);
		memset(g_callset_context[MMI_SIM_1].CTRNumber,0,MAX_CTR_NUM_DIGITS*ENCODING_LENGTH);
#else
		memset(g_callset_cntx_p->CTRNumberInput,0,MAX_CTR_NUM_DIGITS*ENCODING_LENGTH);
		memset(g_callset_cntx_p->CTRNumber,0,MAX_CTR_NUM_DIGITS*ENCODING_LENGTH);
#endif
	}
	else 
	{
		sprintf(temp_buf, "%d", ctrTime);
#ifdef __MMI_MULTI_SIM__
		AnsiiToUnicodeString((PS8)g_callset_context[MMI_SIM_1].CTRNumberInput, temp_buf );
		pfnUnicodeStrcpy((PS8)g_callset_context[MMI_SIM_1].CTRNumber,(PS8)g_callset_context[MMI_SIM_1].CTRNumberInput);		
#else
		AnsiiToUnicodeString((PS8)g_callset_cntx_p->CTRNumberInput, temp_buf );
		pfnUnicodeStrcpy((PS8)g_callset_cntx_p->CTRNumber,(PS8)g_callset_cntx_p->CTRNumberInput);		
#endif
	}
	return(data);
}


/**************************************************************

	FUNCTION NAME		: GetCallTimeDisplay

  	PURPOSE				: Show whether CallTime Display Feature is on or off

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: MMI_BOOL

	Author				: Vanita Jain

 

**************************************************************/

MMI_BOOL GetCallTimeDisplay(void)
{
#ifdef __MMI_DISABLE_CALL_TIME_DISPLAY__
	return MMI_TRUE;
#else
	U8 data = 0;
	S16 error;

	ReadValue( NVRAM_SETTING_CALL_TIME_DISPLAY, &data, DS_BYTE , &error);

	if(data == 1) {
#ifdef __MMI_MULTI_SIM__
		g_callset_context[MMI_SIM_1].CTDStatus = 1;
#else
		g_callset_cntx_p->CTDStatus = 1;
#endif
		return MMI_TRUE;
	}
	else if(data == 0xff)
	{
		data =1;		//Default =ON
#ifdef __MMI_MULTI_SIM__
		g_callset_context[MMI_SIM_1].CTDStatus = 1;
#else
		g_callset_cntx_p->CTDStatus = 1;		
#endif
		WriteValue( NVRAM_SETTING_CALL_TIME_DISPLAY, &data, DS_BYTE , &error);
		return MMI_TRUE;
	}
	else 
	{
#ifdef __MMI_MULTI_SIM__
		g_callset_context[MMI_SIM_1].CTDStatus = 0;
#else
		g_callset_cntx_p->CTDStatus = 0;
#endif
		return MMI_FALSE;
	}
#endif	
}

//micha0921 begin
/**************************************************************

	FUNCTION NAME		: EntryCALLSETGeneralSSResutScr(void )

  	PURPOSE				: entry function for call waiting result screen 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETGeneralSSResutScr(void)
{

	EntryNewScreen(SCR_ID_CALLSET_GENERAL_SS_RESULT, NULL, EntryCALLSETGeneralSSResut, NULL);
	RemoveSSReqScr();
	
	ShowCategory74Screen(STR_GLOBAL_DONE,/*0*/IMG_SETTING_CALL_SETUP,
					STR_GLOBAL_OK,IMG_GLOBAL_OK,0,0,
					g_callset_cntx_p->SSDisplayBuffer, pfnUnicodeStrlen((PS8)g_callset_cntx_p->SSDisplayBuffer), NULL); 
	SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: EntryCALLSETGeneralSSResut(void )

  	PURPOSE				: entry function for call waiting result screen 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryCALLSETGeneralSSResut(void)
{
	if ( (GetIncomingCallHandle() == -1) || (GetActiveScreenId() == SCR_CM_REQUESTINGUSSD)) {
		RemoveSSReqScr();
		EntryCALLSETGeneralSSResutScr();
	}
	else {
		HistoryReplace(SCR_CM_REQUESTINGUSSD, SCR_ID_CALLSET_GENERAL_SS_RESULT, EntryCALLSETGeneralSSResutScr);
	}
}

//micha0921 end

#ifdef __IP_NUMBER__

void InitIPNumber(void)
{
	U8 i;
	for( i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MENU_IP_NUM_ACTIVATE+i, HighlightIPNumActivate);
	}
	SetHiliteHandler(MENU_IP_NUM_EDIT, HighlightIPNumEdit);
}
void HighlightIPNumber(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(EntryScrViewIPNumbers, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryScrViewIPNumbers, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void LoadIPNumbers(void)
{
	U8 i;
	S16 error;

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		ReadValue(NVRAM_IP_NUMBER_SETTING + i, &(IPSetting[i]), DS_BYTE, &error);
	}
	ReadRecord(NVRAM_EF_IP_NUMBER_LID, 1, IPNumber, sizeof(IPNumber), &error);
}

void EntryScrViewIPNumbers(void)
{

	U16   highlite=0;
	U8*		guiBuffer;
	U8 i;
	U16	PPIcons[MAX_IP_NUMBER];
	PU8  nStrItems[MAX_IP_NUMBER];
	U8 IconState[MAX_IP_NUMBER];


	EntryNewScreen(SCR_SETTING_IP_NUMBER, NULL, EntryScrViewIPNumbers, NULL);


	for (i=0; i<MAX_IP_NUMBER; i++)
	{
		PPIcons[i]=0;
		if (IPNumber[i][0] == 0)
			nStrItems[i] = (PU8)GetString(STR_GLOBAL_EMPTY);
		else
			nStrItems[i] = IPNumber[i];
	}
#ifndef __MMI_MULTI_SIM__
	if (IPSetting[0] != 0xff)
	{
		highlite = IPSetting[0];
		PPIcons[IPSetting[0]] = IMG_VICON;
	}
#else
	memset(IconState, 0, sizeof(IconState));
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(IPSetting[i] != 0xFF 
			#ifdef __MMI_MULTI_SIM__
			&&MTPNP_PFAL_Is_Card_Usable(i)
			#endif
			)
		{
			highlite = IPSetting[i] < highlite ? IPSetting[i]:highlite;
			IconState[IPSetting[i]] |= (0x01<<i); 
		}
	}
	for(i = 0; i < MAX_IP_NUMBER; i++)
	{
		if(IconState[i])
		{
			PPIcons[i] = IMG_MTPNP_VICON_1 + IconState[i] - 1;
		}
	}
#endif
	RegisterHighlightHandler(SetCurIPNumberIndex);
	guiBuffer = GetCurrGuiBuffer(SCR_SETTING_IP_NUMBER);

	ShowCategory53Screen( STR_SETTING_IP_NUMBER, MAIN_MENU_TITLE_SETTINGS_ICON,
	STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, STR_GLOBAL_BACK,
	IMG_GLOBAL_BACK, MAX_IP_NUMBER, (U8**)nStrItems,
	(U16*)PPIcons,NULL,0, (U16)highlite,guiBuffer );

	SetLeftSoftkeyFunction(EntryScrIPNumberOptions, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
   
}

void SetCurIPNumberIndex(S32 index)
{
   curIPNumIndex = index;
}

void EntryScrIPNumberOptions (void)
{
	U8*		guiBuffer;
	U16      nNumofItem;
#ifdef __MMI_MULTI_SIM__
	U8 i;
	U16      nStrItem[MMI_SIM_NUMBER+1];
#else
	U16      nStrItem[3];
#endif
	EntryNewScreen(SCR_SETTING_IP_NUM_OPN, NULL, EntryScrIPNumberOptions, NULL);	

	guiBuffer = GetCurrGuiBuffer(SCR_SETTING_IP_NUM_OPN);

#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	MTPNP_AD_SetMenuHideUnhide(MENU_IP_NUM_ACTIVATE);
#endif

	nNumofItem = GetNumOfChild_Ext(MENU_SETTING_IP_NUM_OPN);
	GetSequenceStringIds_Ext(MENU_SETTING_IP_NUM_OPN,nStrItem);
	SetParentHandler(MENU_SETTING_IP_NUM_OPN);
	#ifdef __MMI_MULTI_SIM__
	RegisterHighlightHandler( MultiSimMenuHighLightHandler );
	#else
	RegisterHighlightHandler( ExecuteCurrHiliteHandler);
	#endif
	
#ifndef __MMI_MULTI_SIM__
	if(curIPNumIndex == IPSetting[0]) // man for ip
	{
		nStrItem[0] = STR_GLOBAL_DETACTIVATE;
	}
#else
	for(i = 0; i < nNumofItem -1; i++)
	{
		if(curIPNumIndex == IPSetting[i])
			nStrItem[i] = STRING_MASTER_IP_DEACTIVATE + GetReqSrcModeIndexByIndex(i);
	}
#endif
	ShowCategory15Screen(STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_SETTINGS_ICON,
					STR_GLOBAL_OK, IMG_GLOBAL_OK,
					STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					nNumofItem, nStrItem,
					(U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	return;
}

void HighlightIPNumActivate(void)
{
   SetLeftSoftkeyFunction(ActivateIPNumber, KEY_EVENT_UP);
   SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
   SetKeyHandler(ActivateIPNumber, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void HighlightIPNumEdit(void)
{
   memset(newIPNum, 0, sizeof(newIPNum));
   pfnUnicodeStrcpy((PS8)newIPNum, (PS8)IPNumber[curIPNumIndex]);
   SetLeftSoftkeyFunction(EntryScrIPNumberEdit, KEY_EVENT_UP);
   SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
   SetKeyHandler(EntryScrIPNumberEdit, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
   SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void ActivateIPNumber(void)
{
   	S16 error;
	U8 reqSrcMod = 0;
	#ifdef __MMI_MULTI_SIM__
	reqSrcMod = GetReqSrcModIndex();
	#endif
	if (IPNumber[curIPNumIndex][0] == 0)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, 1000, SUCCESS_TONE);
	}
	else
	{
	   	if(curIPNumIndex == IPSetting[reqSrcMod])	//deactivate man for ip
		{
			IPSetting[reqSrcMod] = 0xff;	
		}
		else
		{
	 		IPSetting[reqSrcMod] = curIPNumIndex;
		}
		WriteValue(NVRAM_IP_NUMBER_SETTING+reqSrcMod, &(IPSetting[reqSrcMod]), DS_BYTE, &error);
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, 1000, SUCCESS_TONE);
	}
	DeleteScreenIfPresent(SCR_SETTING_IP_NUM_OPN); 
	
}

void EntryScrIPNumberEdit (void)
{
	U8*	guiBuffer;

	EntryNewScreen(SCR_SETTING_IP_NUM_EDIT, NULL, EntryScrIPNumberEdit, NULL);

	guiBuffer = GetCurrGuiBuffer( SCR_SETTING_IP_NUM_EDIT ); 

#if 0 //fix bug 15531
	if( guiBuffer == NULL)
	   memset( newIPNum, 0, 20 );
#endif /* Guoj delete. It's not used now at 2009-12-31 */
	
	ShowCategory5Screen( STR_SETTING_IP_NUMBER,
	                     MAIN_MENU_TITLE_SETTINGS_ICON, STR_GLOBAL_OK,
	                     IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                     INPUT_TYPE_PHONE_NUMBER, ( PU8 ) newIPNum, MAX_IP_NUMBER_LEN+1,
	                     guiBuffer );
	
	SetLeftSoftkeyFunction( SetIPNumber, KEY_EVENT_UP );
	SetCategory5RightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

void SetIPNumber(void)
{
	S16 error;
	U8 i;

	if (pfnUnicodeStrlen((PS8)newIPNum) >= MAX_IP_NUMBER_LEN)
	{
		newIPNum[MAX_IP_NUMBER_LEN*2]=0;
		newIPNum[MAX_IP_NUMBER_LEN*2+1]=0;
	}
	pfnUnicodeStrcpy((PS8)IPNumber[curIPNumIndex], (PS8)newIPNum);

	WriteRecord(NVRAM_EF_IP_NUMBER_LID, 1, IPNumber, sizeof(IPNumber), &error);
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if((IPSetting[i] == curIPNumIndex)&&(pfnUnicodeStrlen((PS8)newIPNum) == 0))//man for ip
		{
			IPSetting[i] = 0xff;
			WriteValue(NVRAM_IP_NUMBER_SETTING + i, &IPSetting[i], DS_BYTE, &error);
		}
	}

	GoBackToHistory(SCR_SETTING_IP_NUMBER);
	DeleteScreenIfPresent(SCR_SETTING_IP_NUM_EDIT);   
}

void GetCurrIPNumber(U8* num_str, U8 nSimID)
{
	if (num_str != NULL)
	{
		num_str[0] = 0;

		if (IPSetting[nSimID] != 0xff)
			UnicodeToAnsii((PS8)num_str, (PS8)IPNumber[IPSetting[nSimID]]);
	}
}

S8 *GetCurrIPNumberUCS2(U8 nSimID)
{
	if (IPSetting[nSimID] != 0xff)
         	return (S8*)IPNumber[IPSetting[nSimID]];
	else
		return NULL;
}

BOOL IsIPEnable(U8 nSimID)
{
	if (IPSetting[nSimID] != 0xff 
		#ifdef __MMI_MULTI_SIM__
		&& MTPNP_PFAL_Is_Card_Usable(nSimID)
		#endif
		)
		return TRUE;
	return FALSE;
}
void SetIPMenuOption(U32 menuID)
{
	U8 i;
	U8 j = 0;
	U8 usedSimIndex = 0;
	#ifdef __MMI_MULTI_SIM__
	if(isInCall())
	{
		usedSimIndex = MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL);
	}
	#endif
	for(i= 0; i < MMI_SIM_NUMBER; i++)
	{
		mmi_frm_hide_menu_item(menuID + i);
		if(IsIPEnable(i))
		{
			if(!isInCall())
			{
				mmi_frm_unhide_menu_item(menuID + i);
				#ifdef __MMI_MULTI_SIM__
				SetReqSrcModeIndex(j++, i);
				#endif
			}
			else
			{
				if(i == usedSimIndex)
				{
					mmi_frm_unhide_menu_item(menuID + i);
					#ifdef __MMI_MULTI_SIM__
					SetReqSrcModeIndex(j++, i);
					#endif
				}
			}
		}
	}
}
#endif/*__IP_NUMBER__*/
 
#ifdef __MMI_CM_BLACK_LIST__

#include "callhistoryenum.h"
void InitBlackList(void)
{
	SetHiliteHandler(MENU_SETTING_BLACK_LIST_MODE, HighlightBlackListMode);
	SetHiliteHandler(MENU_SETTING_BLACK_LIST_NUM, HighlightBlackListNum);
	SetHintHandler(MENU_SETTING_BLACK_LIST_MODE, HintBlackListMode);
	SetHiliteHandler(CM_BL_AND_RL_STATUS, black_red_list_status);
}

void HighlightBlackList(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(EntryScrBlackList, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryScrBlackList, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void HighlightBlackListMode(void)
{
	if (GetBlackListMode() == TRUE)
		ChangeLeftSoftkey(STR_GLOBAL_OFF, 0);
	else
		ChangeLeftSoftkey(STR_GLOBAL_ON, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryScrBlackListMode,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


void AddBlackListNum(void)
{
	U16 index;
	S16 error;
	   	  
	for (index=0; index<MAX_BLACK_LIST_NUMBER; index++)
		pfnUnicodeStrcpy((PS8)BlackListNum[index], (PS8)dispBlackListNum[index]);
   	WriteRecord(NVRAM_EF_BLACK_LIST_LID, 1, BlackListNum, sizeof(BlackListNum), &error);
  	//GoBackToHistory(SCR_CALL_HISTORY_MAIN_MENU);

	//Renwy modify begine 2008/09/18 for bug 9755
  	//EntryScrBlackListNum();
  	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
		             IMG_GLOBAL_ACTIVATED, 1,
		             CS_NOTIFYDURATION,SUCCESS_TONE);

	DeleteNHistory(1);
	//Renwy modify end 2008/09/18 for bug 9755
}


void GotoCallHistoryMenu()
{
  	//GoBackToHistory(SCR_CALL_HISTORY_MAIN_MENU);//Renwy modify 2008/09/18 for bug 9755
	GoBacknHistory(1);//Renwy modify 2008/09/18 for bug 9755
}
void EntryScrAddtoBlackListNumConfirm( void )
{
	DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,
		                   STR_GLOBAL_NO,IMG_GLOBAL_NO,
		                   get_string(STR_GLOBAL_SAVE),
		                   IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(AddBlackListNum,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GotoCallHistoryMenu,KEY_EVENT_UP);
}



void BlackListAdded(void)
{
  	//U16 index;
	
	//for (index=0; index<MAX_BLACK_LIST_NUMBER; index++)
		//pfnUnicodeStrcpy((PS8)dispBlackListNum[index], (PS8)BlackListNum[index]);

  ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(EntryScrAddtoBlackListNumConfirm, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryScrAddtoBlackListNumConfirm, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
extern BOOL gSecuritySetting;

void HighlightBlackListNum(void)
{
	U16 index;
	for (index=0; index<MAX_BLACK_LIST_NUMBER; index++)
		pfnUnicodeStrcpy((PS8)dispBlackListNum[index], (PS8)BlackListNum[index]);
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(EntryScrBlackListNum, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryScrBlackListNum, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void HintBlackList(U16 index)
{
	memset((S8 *)hintData[index],0,MAX_SUB_MENU_SIZE);
	if(GetBlackListMode() == TRUE)
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_ON));						
	}
	else
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));						
	}
}

void HintBlackListMode(U16 index)
{
	memset((S8 *)hintData[index],0,MAX_SUB_MENU_SIZE);
	if(GetBlackListMode() == TRUE)
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_ON));						
	}
	else
	{
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));						
	}
}
extern void mmi_phb_blackAndRedList_set(void);
void black_red_list_status(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetKeyHandler(mmi_phb_blackAndRedList_set, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(mmi_phb_blackAndRedList_set, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void EntryScrBlackList(void)
{
	U8*	guiBuffer;
   	U16	nNumofItem;
   	U16	nStrItem[3];
   	U8 *BlackListType[MAX_SUB_MENUS];

	EntryNewScreen(SCR_SETTING_BLACK_LIST, NULL, EntryScrBlackList, NULL);	

	guiBuffer = GetCurrGuiBuffer(SCR_SETTING_BLACK_LIST);

	nNumofItem = GetNumOfChild(MENU_SETTING_BLACK_LIST);
	GetSequenceStringIds(MENU_SETTING_BLACK_LIST,nStrItem);
	SetParentHandler(MENU_SETTING_BLACK_LIST);
	ConstructHintsList(MENU_SETTING_BLACK_LIST, BlackListType);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory52Screen(STR_SETTING_BLACK_LIST,MAIN_MENU_TITLE_SETTINGS_ICON, 
	STR_GLOBAL_OK,IMG_GLOBAL_OK,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
	nNumofItem, nStrItem,
	(U16 *)gIndexIconsImageList, BlackListType,
	0, 0,	guiBuffer );
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void EntryScrBlackListMode(void)
{
	U16 index;
   	index = GetCurrHiliteID();
   
	if(GetBlackListMode() == TRUE)
	{
		SetBlackListMode(FALSE);
 		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));		
		Category52ChangeItemDescription(0,(U8*)GetString(STR_GLOBAL_OFF));
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}
	else
	{
		SetBlackListMode(TRUE);
		pfnUnicodeStrcpy((S8*)hintData[index],(PS8)GetString(STR_GLOBAL_ON));		
		Category52ChangeItemDescription(0,(U8*)GetString(STR_GLOBAL_ON));
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	RedrawCategoryFunction();
	HighlightBlackListMode();
	
}

void EntryScrBlackListNum(void)
{
	U8* guiBuffer;		   /* Buffer holding history data */
	U8* inputBuffer;     /* added for inline edit history */
	U16 inputBufferSize; /* added for inline edit history */
	U16 index;
	U16 BlackListNumImageList[6];

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_SETTING_BLACK_LIST_NUM, ExitScrBlackListNum, NULL, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer( SCR_SETTING_BLACK_LIST_NUM );

	/* 3 Set current parent id*/
	InitializeCategory57Screen();

	for (index=0; index<MAX_BLACK_LIST_NUMBER; index++)
	{
		SetInlineItemActivation(&wgui_inline_items[index],
							INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
		SetInlineItemTextEdit(&wgui_inline_items[index],(PU8)dispBlackListNum[index], 
							MAX_BLACK_LIST_NUMBER_LEN+1,
							INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_PLUS_CHARACTER_HANDLING);
		DisableInlineItemHighlight(&wgui_inline_items[index]);
	}
	BlackListNumImageList[0] = IMG_GLOBAL_L1;
	BlackListNumImageList[1] = IMG_GLOBAL_L2;
	BlackListNumImageList[2] = IMG_GLOBAL_L3;
	BlackListNumImageList[3] = IMG_GLOBAL_L4;
	BlackListNumImageList[4] = IMG_GLOBAL_L5;
   /* added for inline edit history */
	inputBuffer = GetCurrNInputBuffer (SCR_SETTING_BLACK_LIST_NUM, &inputBufferSize);

	RegisterHighlightHandler(SetCurrBlackListNumIndex);

   /* added for inline edit history */
	if((guiBuffer != NULL) && (guiBuffer[0]!=0))
	{
	   /* sets the data */
	   SetCategory57Data(wgui_inline_items, MAX_BLACK_LIST_NUMBER, inputBuffer);
	}

	DisableCategory57ScreenDone();
	
	ShowCategory57Screen(STR_SETTING_BLACK_LIST_NUM,
	MAIN_MENU_TITLE_SETTINGS_ICON,
	STR_GLOBAL_OK, IMG_GLOBAL_OK,
	STR_GLOBAL_BACK, IMG_GLOBAL_BACK, MAX_BLACK_LIST_NUMBER,
	BlackListNumImageList, wgui_inline_items, curBlackListNumIndex, guiBuffer);

	SetCategory57RightSoftkeyFunctions(EntryScrSetBlackListNumConfirm, GoBackHistory);
}

void ExitScrBlackListNum( void )
{
	history_t currHistory;
	U16 inputBufferSize;                //added for inline edit history
	S16 nHistory	= 0;

	CloseCategory57Screen();
	currHistory.scrnID = SCR_SETTING_BLACK_LIST_NUM;
	currHistory.entryFuncPtr = EntryScrBlackListNum;
	pfnUnicodeStrcpy((S8 *)currHistory.inputBuffer, (S8 *)&nHistory);
	GetCategoryHistory(currHistory.guiBuffer);

   //added for inline edit history
	inputBufferSize = (U16)GetCategory57DataSize( );
	GetCategory57Data ((U8 *) currHistory.inputBuffer);
	AddNHistory(currHistory, inputBufferSize);
	return;
}

void EntryScrSetBlackListNumConfirm( void )
{
	DisplayConfirm(STR_GLOBAL_YES,IMG_GLOBAL_YES,
		                   STR_GLOBAL_NO,IMG_GLOBAL_NO,
		                   get_string(STR_GLOBAL_SAVE),
		                   IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(SetBlackListNum,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoToBlackList,KEY_EVENT_UP);
}

void GoToBlackList(void)
{
	if (gSecuritySetting)
	{
		GoBackToHistory(SCR_ID_SECSET_MAIN);
	}
	else
		GoBackToHistory(SCR_SETTING_BLACK_LIST);
}

void SetBlackListMode(pBOOL flag)
{
	S16 error;
	
	gBlackListFlag = flag;
	WriteValue(NVRAM_BLACK_LIST_SETTING, &gBlackListFlag, DS_BYTE, &error);
}

pBOOL GetBlackListMode(void)
{
	return gBlackListFlag;
}

void SetBlackListNum(void)
{
	U16 index;
	S16 error;
	S16 result;
	for (index=0; index<MAX_BLACK_LIST_NUMBER; index++)
		pfnUnicodeStrcpy((PS8)BlackListNum[index], (PS8)dispBlackListNum[index]);
   	result = WriteRecord(NVRAM_EF_BLACK_LIST_LID, 1, BlackListNum, sizeof(BlackListNum), &error);
	if (result == -1 || error == NVRAM_WRITE_FAIL)
	{
		mmi_trace(1,"------------------------------SetBlackListNum -DisplayPopup-STR_GLOBAL_ERROR------------------------------\n");
		DisplayPopup( (U8*)GetString(STR_GLOBAL_ERROR),  IMG_GLOBAL_ERROR, MMI_FALSE, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE );
	}
	else
	{
		mmi_trace(1,"------------------------------SetBlackListNum -DisplayPopup-STR_GLOBAL_DONE------------------------------\n");
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	}	
	if (gSecuritySetting)
	{
		GoBackToHistory(SCR_ID_SECSET_MAIN);
	}
  	else GoBackToHistory(SCR_SETTING_BLACK_LIST);
}

void LoadBlackListNum(void)
{
	S16 error;
  U16 index,index2;
  U8 number[MAX_BLACK_LIST_NUMBER_LEN+1];
	INT32   ret = 0;
	memset(BlackListNum,0,sizeof(BlackListNum));
  	memset(dispBlackListNum,0,sizeof(dispBlackListNum));
 	ReadValue(NVRAM_BLACK_LIST_SETTING, &gBlackListFlag, DS_BYTE, &error);
	if(gBlackListFlag == 0xff)
	{
		gBlackListFlag = 0;
		WriteValue(NVRAM_BLACK_LIST_SETTING, &gBlackListFlag, DS_BYTE, &error);
	}
	ret = ReadRecord(NVRAM_EF_BLACK_LIST_LID, 1, BlackListNum, sizeof(BlackListNum), &error);
	/*modify the condition to judge whether the reading result is success or not for bug- by liuxl on 20090622 */
	if(error == NVRAM_READ_SUCCESS)
	{
		for (index=0;index<MAX_BLACK_LIST_NUMBER;index++)
		{  
			UnicodeToAnsii(number, BlackListNum[index]);
			for (index2=0;index2<strlen(number);index2++)
			if ((number[index2]<'0') || (number[index2]>'9'))
			{
				if(number[index2] != '+')
				{
					memset(BlackListNum[index],0,(MAX_BLACK_LIST_NUMBER_LEN+1)*ENCODING_LENGTH);
					break;
				}
			}
//			mmi_trace(0,"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$index = %d, number = %s\n",index,number);
		}
	}
	else	 
	{
		memset(BlackListNum,0,sizeof(BlackListNum));
		WriteRecord(NVRAM_EF_BLACK_LIST_LID, 1, BlackListNum, sizeof(BlackListNum), &error);
		ReadRecord(NVRAM_EF_BLACK_LIST_LID, 1, BlackListNum, sizeof(BlackListNum), &error);
	}
}

void SetCurrBlackListNumIndex(S32 index)
{
	curBlackListNumIndex = index;
}

pBOOL IsBlackListNum(PU8 number)
{
	U16 index;
	U8 tmpNumber[MAX_NUMBER];
	U8 tmpBlackListNumber[MAX_NUMBER];
	U32 newNumber, newBlackListNum ;
	
	memset(tmpNumber, 0, ENCODING_LENGTH);
	UnicodeToAnsii((PS8)tmpNumber, (PS8)number);

	newNumber = mmi_phb_util_convert_number_to_int(tmpNumber);
	
	for (index=0; index<MAX_BLACK_LIST_NUMBER; index++)
	{
   		UnicodeToAnsii((PS8)tmpBlackListNumber, (PS8)BlackListNum[index]);
   		newBlackListNum = mmi_phb_util_convert_number_to_int(tmpBlackListNumber);
		if (newNumber == newBlackListNum)
			return TRUE;

	}
	return FALSE;
}
#include "callsstruct.h"
extern call_history_context_struct *chis_p;

U16 SearchEmptyEntry(U8 *arry,U16 rows, U16 columns)
{
  U8 i = 0;
  for (i=0; i< rows; i++)
    if (*(arry+i*columns)==0 && *(arry+i*columns+1)==0)
      return i;
  return i;
}

void HightlightReceivedAddToBlackList(void)
{
  U8 index =  0,index1= 0,index2 = 0;
   U8 number[MAX_BLACK_LIST_NUMBER_LEN+1]={0};//Renwy modify 2008/09/05 for bug 9642

  //LoadBlackListNum();

  	//Renwy modify 2008/09/05 for bug 9642
	for (index=0; index<MAX_BLACK_LIST_NUMBER; index++)
		pfnUnicodeStrcpy((PS8)dispBlackListNum[index], (PS8)BlackListNum[index]);
	
	if( chis_p->recvdCalls[chis_p->currRecvdCallIndex].number[0] == 0 )
	{
	}
	else
	{
   		index1 = SearchEmptyEntry((U8*)dispBlackListNum,MAX_BLACK_LIST_NUMBER,(MAX_BLACK_LIST_NUMBER_LEN+1)*ENCODING_LENGTH);
		for (index=0;index<MAX_BLACK_LIST_NUMBER;index++)
		{
		 	if (0==pfnUnicodeStrncmp((PS8)dispBlackListNum[index],(S8 *)chis_p->recvdCalls[chis_p->currRecvdCallIndex].number,(MAX_BLACK_LIST_NUMBER_LEN+1)))
		   		break;
		}
		if ((index == MAX_BLACK_LIST_NUMBER)&&(index1 != MAX_BLACK_LIST_NUMBER))
		{
 			index = index1;
		}
   		else if ((index == MAX_BLACK_LIST_NUMBER)&&(index1 == MAX_BLACK_LIST_NUMBER))
		{	
		  	for (index2=0; index2<MAX_BLACK_LIST_NUMBER-1; index2++)
			    	pfnUnicodeStrcpy((PS8)dispBlackListNum[index2], (PS8)dispBlackListNum[index2+1]);
		  	index = index2;
		}
		
   		SetCurrBlackListNumIndex(index);

	 	//pfnUnicodeStrcpy((PS8)BlackListNum[index],
		   	//(S8 *)chis_p->recvdCalls[chis_p->currRecvdCallIndex].number );
		pfnUnicodeStrncpy((PS8)dispBlackListNum[index],
		   		(S8 *)chis_p->recvdCalls[chis_p->currRecvdCallIndex].number ,(MAX_BLACK_LIST_NUMBER_LEN+1));
		   
		//Renwy modify end 2008/09/05 for bug 9642
	
  		UnicodeToAnsii(number, dispBlackListNum[index]);
  		mmi_trace(1,"index1 = %d, index2 = %d, index = %d, number = %s\n",index1,index2,index,number);
	}
  //HighlightBlackListNum();
  BlackListAdded();
}
void HightlightMissedAddToBlackList(void)
{
  U8 index =0,index1 = 0,index2 =0;
   U8 number[MAX_BLACK_LIST_NUMBER_LEN+1]={0};//Renwy modify 2008/09/05 for bug 9642
  //LoadBlackListNum();

  	//Renwy modify 2008/09/05 for bug 9642
	for (index=0; index<MAX_BLACK_LIST_NUMBER; index++)
		pfnUnicodeStrcpy((PS8)dispBlackListNum[index], (PS8)BlackListNum[index]);
	
  	if( chis_p->missedCalls[chis_p->currMissedCallIndex].number[0] == 0 )
	{
	}
	else
	{
		index1 = SearchEmptyEntry((U8*)dispBlackListNum,MAX_BLACK_LIST_NUMBER,(MAX_BLACK_LIST_NUMBER_LEN+1)*ENCODING_LENGTH);
		for (index=0;index<MAX_BLACK_LIST_NUMBER;index++)
		{
			if (0==pfnUnicodeStrncmp((PS8)dispBlackListNum[index],(S8 *)chis_p->missedCalls[chis_p->currMissedCallIndex].number,(MAX_BLACK_LIST_NUMBER_LEN+1)))
			break;
		}
		if ((index == MAX_BLACK_LIST_NUMBER)&&(index1 != MAX_BLACK_LIST_NUMBER))
		{
 			index = index1;
		}
		else if ((index == MAX_BLACK_LIST_NUMBER)&&(index1 == MAX_BLACK_LIST_NUMBER))
		{	
			for (index2=0; index2<MAX_BLACK_LIST_NUMBER-1; index2++)
			    	pfnUnicodeStrcpy((PS8)dispBlackListNum[index2], (PS8)dispBlackListNum[index2+1]);
			index = index2;
		}
		
   		SetCurrBlackListNumIndex(index);

	 	//pfnUnicodeStrcpy((PS8)BlackListNum[index],
		   	//(S8 *)chis_p->missedCalls[chis_p->currMissedCallIndex].number );
		pfnUnicodeStrncpy((PS8)dispBlackListNum[index],
		   	(S8 *)chis_p->missedCalls[chis_p->currMissedCallIndex].number, (MAX_BLACK_LIST_NUMBER_LEN+1) );
		//Renwy modify end 2008/09/05 for bug 9642
	
  		UnicodeToAnsii(number, dispBlackListNum[index]);
  		mmi_trace(1,"index1 = %d, index2 = %d, index = %d, number = %s\n",index1,index2,index,number);
	}
  //HighlightBlackListNum();
  BlackListAdded();
}

void HightlightDialedAddToBlackList(void)
{
  U8 index = 0,index1 = 0,index2 = 0;
  U8 number[MAX_BLACK_LIST_NUMBER_LEN+1]={0};//Renwy modify 2008/09/05 for bug 9642
//  LoadBlackListNum();

	//Renwy modify 2008/09/05 for bug 9642
	for (index=0; index<MAX_BLACK_LIST_NUMBER; index++)
		pfnUnicodeStrcpy((PS8)dispBlackListNum[index], (PS8)BlackListNum[index]);
	
  	if( chis_p->dialedCalls[chis_p->currDialedCallIndex].number[0] == 0 )
	{
	}
	else
	{
   		index1 = SearchEmptyEntry((U8*)dispBlackListNum,MAX_BLACK_LIST_NUMBER,(MAX_BLACK_LIST_NUMBER_LEN+1)*ENCODING_LENGTH);

		mmi_trace(g_sw_CC, "index1 =%d", index1);
	
		for (index=0;index<MAX_BLACK_LIST_NUMBER;index++)
		{
			if (0==pfnUnicodeStrncmp((PS8)dispBlackListNum[index],(S8 *)chis_p->dialedCalls[chis_p->currDialedCallIndex].number,(MAX_BLACK_LIST_NUMBER_LEN*ENCODING_LENGTH)))
				break;
		}
		if ((index == MAX_BLACK_LIST_NUMBER)&&(index1 != MAX_BLACK_LIST_NUMBER))
		{
		  	index = index1;
		}
		else if ((index == MAX_BLACK_LIST_NUMBER)&&(index1 == MAX_BLACK_LIST_NUMBER))
		{	
		  	for (index2=0; index2<MAX_BLACK_LIST_NUMBER-1; index2++)
			    	pfnUnicodeStrcpy((PS8)dispBlackListNum[index2], (PS8)dispBlackListNum[index2+1]);
		  	index = index2;
		}

		mmi_trace(g_sw_CC, "index =%d", index);
   		SetCurrBlackListNumIndex(index);

		//Renwy modify begine for bug 9642 2008/09/05
		//pfnUnicodeStrcpy((PS8)BlackListNum[index],
						//(S8 *)chis_p->dialedCalls[chis_p->currDialedCallIndex].number );
		pfnUnicodeStrncpy((PS8)dispBlackListNum[index],
						(S8 *)chis_p->dialedCalls[chis_p->currDialedCallIndex].number ,(MAX_BLACK_LIST_NUMBER_LEN*ENCODING_LENGTH));

		UnicodeToAnsii(number, dispBlackListNum[index]);
		//Renwy modify end 2008/09/05 for bug 9642
  	
  		mmi_trace(1,"index1 = %d, index2 = %d, index = %d, number = %s\n",index1,index2,index,number);
	}
  //HighlightBlackListNum();
  //HighlightBlackList();
  BlackListAdded();
}

void HightlightRejectedAddToBlackList(void)
{
	U8 index = 0,index1 = 0,index2 = 0;
   	U8 number[MAX_BLACK_LIST_NUMBER_LEN+1]={0};//Renwy modify 2008/09/05 for bug 9642
	//  LoadBlackListNum();

	//Renwy modify 2008/09/05 for bug 9642
	for (index=0; index<MAX_BLACK_LIST_NUMBER; index++)
		pfnUnicodeStrcpy((PS8)dispBlackListNum[index], (PS8)BlackListNum[index]);

  	if( chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number[0] == 0 )
	{
	}
	else
	{
   		index1 = SearchEmptyEntry((U8*)dispBlackListNum,MAX_BLACK_LIST_NUMBER,(MAX_BLACK_LIST_NUMBER_LEN+1)*ENCODING_LENGTH);
   		for (index=0;index<MAX_BLACK_LIST_NUMBER;index++)
		{
			if (0==pfnUnicodeStrncmp((PS8)dispBlackListNum[index],(S8 *)chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number,(MAX_BLACK_LIST_NUMBER_LEN+1)))
			break;
		}
	   	if ((index == MAX_BLACK_LIST_NUMBER)&&(index1 != MAX_BLACK_LIST_NUMBER))
	   	{
	     		index = index1;
	   	}
	   	else if ((index == MAX_BLACK_LIST_NUMBER)&&(index1 == MAX_BLACK_LIST_NUMBER))
	   	{	
	      		for (index2=0; index2<MAX_BLACK_LIST_NUMBER-1; index2++)
			    	pfnUnicodeStrcpy((PS8)dispBlackListNum[index2], (PS8)dispBlackListNum[index2+1]);
	      		index = index2;
	   	}
		
	   	SetCurrBlackListNumIndex(index);

		 //pfnUnicodeStrcpy((PS8)BlackListNum[index],
			   //(S8 *)chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number );
		pfnUnicodeStrncpy((PS8)dispBlackListNum[index],
			   (S8 *)chis_p->rejectedCalls[chis_p->currRejectedCallIndex].number, (MAX_BLACK_LIST_NUMBER_LEN+1) );
		//Renwy modify end 2008/09/05 for bug 9642
		
	  	UnicodeToAnsii(number, dispBlackListNum[index]);
	  	mmi_trace(1,"index1 = %d, index2 = %d, index = %d, number = %s\n",index1,index2,index,number);
	}
  //HighlightBlackListNum();
  BlackListAdded();
}


#endif
 


