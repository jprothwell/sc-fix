
/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                                           */
/* Filename:    adp_Sms_Events_study.c                                                     */
/*                                                                                                   */
/* Description:                                                                                 */
/*     Pan Xu study code to wirte this file. Please:)                               */
/*     @Pan Xu  2006-09-7.                                                               */
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
#include "gsm7bitdefaultalphabet.h"
 
#include <csw.h>
#include "mmi.h"
#include "uim.h"
#include "unit_test.h"
#include "adp_nw_event.h"
#include "adp_nw_event_prv.h"
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
#include "networksetupgprot.h"
//debug add b
#include "mmi_trace.h"
//debug add e
#include "simdetectionhwmapfile.h"
#include "cfw.h"

//[221] Added by ZXB 2009/02/06								[BEGIN]
#ifdef __ADP_MULTI_SIM__

static void ProcCfunFailed(void)
{
	mmi_nw_cfun_state_rsp_struct *p = NULL;

	p = (mmi_nw_cfun_state_rsp_struct *)OslMalloc(sizeof(mmi_nw_cfun_state_rsp_struct));
	MMI_ASSERT(p != NULL);
	memset(p, 0, sizeof(mmi_nw_cfun_state_rsp_struct));
	p->result = KAL_FALSE;

	mmi_trace(g_sw_ADP_NW, "ModeSetting: PRT_NW_CFUN_STATE_RSP to MMI, ->result==KAL_FALSE");
	g_pNwContext->sim_id = 0;
	AdpNW_SendMessageToMMI((local_para_struct *)p, PRT_NW_CFUN_STATE_RSP);
}

static void ProcCfunSuccess(void)
{
	mmi_nw_cfun_state_rsp_struct *p = NULL;

	p = (mmi_nw_cfun_state_rsp_struct *)OslMalloc(sizeof(mmi_nw_cfun_state_rsp_struct));
	MMI_ASSERT(p != NULL);
	memset(p, 0, sizeof(mmi_nw_cfun_state_rsp_struct));
	p->result = KAL_TRUE;

	mmi_trace(g_sw_ADP_NW, "ModeSetting: PRT_NW_CFUN_STATE_RSP to MMI, ->result==KAL_TRUE");
	g_pNwContext->sim_id = 0;
	AdpNW_SendMessageToMMI((local_para_struct *)p, PRT_NW_CFUN_STATE_RSP);
}


///////////////////////////////////////////////////////////////////
// Add module context data structure and global var for dual-sim.
ADP_NW_CONTEXT_T g_NwContext[4] =
{
	{ADP_SIM_1, },
	{ADP_SIM_2, },
	{ADP_SIM_3, },
	{ADP_SIM_4, },
};

ADP_NW_CONTEXT_T *g_pNwContext = &g_NwContext[0];

#endif
//[221] Added by ZXB 2009/02/06								[END]


/**************************************************************

	FUNCTION NAME		: AdpNW_SendMessageToMMI

	PURPOSE				: send message from adapter to mmi

	PARAMETERS	              :

	REMARKS			:

**************************************************************/

static void AdpNW_SendMessageToMMI(local_para_struct *msg_ptr,  msg_type_t msg_id)
{
	ilm_struct  ilm;

	//ASSERT(msg_ptr != NULL);
	
	ilm.dest_mod_id = MOD_MMI;
#ifdef __ADP_MULTI_SIM__
	ilm.src_mod_id = adp_SimId2IlmMod(g_pNwContext->sim_id);
#else  /* No Defined __ADP_MULTI_SIM__ */
	ilm.src_mod_id = MOD_L4C;
#endif /* __ADP_MULTI_SIM__ */
	ilm.sap_id = CSM_L4C_SAP; //have no macro
	ilm.local_para_ptr = msg_ptr;
	ilm.peer_buff_ptr = NULL;
	ilm.msg_id = msg_id;

	OslMsgSendExtQueue(&ilm);
}

/**************************************************************

	FUNCTION NAME		: CswBand2CSDBand

	PURPOSE				: transform band value

	PARAMETERS	              :

	REMARKS			:The band value from csw is diffrent from the needed value in mmi

**************************************************************/

static void CswBand2CSDBand(UINT8 *CSDBand,  UINT8 cswBand)
{
	switch (cswBand)
	{

	case  CFW_NW_BAND_GSM_900P:

	case  CFW_NW_BAND_GSM_900E:

	case  CFW_NW_BAND_GSM_900P | CFW_NW_BAND_GSM_900E:
		*CSDBand = MMI_BAND_900;
		break;

	case  CFW_NW_BAND_DCS_1800:
		*CSDBand = MMI_BAND_1800;
		break;

	case  CFW_NW_BAND_PCS_1900:
		*CSDBand = MMI_BAND_1900;
		break;

	case  CFW_NW_BAND_GSM_850:
		*CSDBand = MMI_BAND_850;
		break;

	case  CFW_NW_BAND_GSM_900P | CFW_NW_BAND_DCS_1800:

	case  CFW_NW_BAND_GSM_900E | CFW_NW_BAND_DCS_1800:

	case  CFW_NW_BAND_GSM_900P | CFW_NW_BAND_GSM_900E | CFW_NW_BAND_DCS_1800:
		*CSDBand = MMI_BAND_DUAL;
		break;

	case  CFW_NW_BAND_GSM_850 | CFW_NW_BAND_PCS_1900:
		*CSDBand = MMI_BAND_850_1900;
		break;

	case  CFW_NW_BAND_GSM_850 | CFW_NW_BAND_DCS_1800:
		*CSDBand = MMI_BAND_850_1800;
		break;

	default:
		*CSDBand = MMI_BAND_AUTO;
		break;

	}
}

/**************************************************************

	FUNCTION NAME		: CswBand2CSDBand

	PURPOSE				: transform band value

	PARAMETERS	              : 

	REMARKS			:The band value from csw is diffrent from the needed value in mmi  

**************************************************************/

static void CSDBand2CswBand(UINT8 *cswBand,  UINT8 CSDBand)
{
	UINT8 CSDBand_Auto = 0
#ifdef __GSM850__
				| CFW_NW_BAND_GSM_850
#endif
#ifdef __EGSM900__
				| CFW_NW_BAND_GSM_900P | CFW_NW_BAND_GSM_900E 
#endif
#ifdef __DCS1800__
				| CFW_NW_BAND_DCS_1800
#endif
#ifdef __PCS1900__
				| CFW_NW_BAND_PCS_1900
#endif
				;
	switch (CSDBand)
	{
		case MMI_BAND_900:
			*cswBand = CFW_NW_BAND_GSM_900P | CFW_NW_BAND_GSM_900E;
			break;

		case MMI_BAND_1800:
			*cswBand = CFW_NW_BAND_DCS_1800;
			break;

		case MMI_BAND_1900:
			*cswBand = CFW_NW_BAND_PCS_1900;
			break;

		case MMI_BAND_850:
			*cswBand = CFW_NW_BAND_GSM_850;
			break;

		case MMI_BAND_DUAL:
			*cswBand = CFW_NW_BAND_GSM_900P | CFW_NW_BAND_GSM_900E | CFW_NW_BAND_DCS_1800;
			break;

		case MMI_BAND_850_1900:
			*cswBand = CFW_NW_BAND_GSM_850 | CFW_NW_BAND_PCS_1900;
			break;
		case MMI_BAND_850_1800:
			*cswBand = CFW_NW_BAND_GSM_850 | CFW_NW_BAND_DCS_1800;
			break;
			
		default:  //MMI_BAND_AUTO
			*cswBand = CSDBand_Auto;
			break;
	}
}


/**************************************************************

	FUNCTION NAME		: CswPLMN2CSDPLMN

	PURPOSE				: transform PLMN display format

	PARAMETERS	              : 

	REMARKS			:The PLMN  format from csw is diffrent from the needed PLMN format in mmi  

**************************************************************/

void CswPLMN2CSDPLMN(UINT8 *pCSDPLMN,  UINT8 *pCswPLMN,  UINT8 nLength)
{
	UINT8 nIndex = 0;
	
	ASSERT(pCswPLMN != NULL);
	ASSERT(pCSDPLMN != NULL);

	for (nIndex = 0; nIndex < nLength; ++nIndex)
	{
		if (pCswPLMN[nIndex] == 0x0f)
		{
			//mmi_trace(g_sw_ADP_NW,  ">>>> pCswPLMN[nIndex] =%d>>>>\r\n", pCswPLMN[nIndex]);
			pCSDPLMN[nIndex] = 0;
		}
		else
		{
			//mmi_trace(g_sw_ADP_NW,  ">>>> pCswPLMN[nIndex] =%d>>>>\r\n", pCswPLMN[nIndex]);
			pCSDPLMN[nIndex] = pCswPLMN[nIndex] + 48;
		}
	}

	pCSDPLMN[6] = 0;
}

/**************************************************************

	FUNCTION NAME		: CSDPLMN2CswPLMN

	PURPOSE				: transform PLMN display format

	PARAMETERS	              : 

	REMARKS			:The PLMN  format from csw is diffrent from the needed PLMN format in mmi  

**************************************************************/

void CSDPLMN2CswPLMN(UINT8 *pCswPLMN,  UINT8 *pCSDPLMN,  UINT8 nLength)
{
	UINT8 nIndex = 0;
	
	ASSERT(pCswPLMN != NULL);
	ASSERT(pCSDPLMN != NULL);
	
	for (nIndex = 0; nIndex < nLength; ++nIndex)
	{
		if (pCSDPLMN[nIndex] == 0)
		{
			pCswPLMN[nIndex] = 0x0f;
		}
		else
		{
			pCswPLMN[nIndex] = pCSDPLMN[nIndex] - 48;
		}
	}
}

/**************************************************************

	FUNCTION NAME		: AdaptAbortGetPLMNListToIdle

	PURPOSE				: abort getting PLMN list srevice

	PARAMETERS	              : 

	REMARKS			:

**************************************************************/

static UINT8 AdaptAbortGetPLMNListToIdle(COS_EVENT* pCosEvent)
{
    ASSERT(pCosEvent != NULL);
    
	mmi_trace(g_sw_ADP_NW,  "MMI_NW: >>>> entry AdaptAbortGetPLMNListToIdle >>>>\r\n");
#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT

	if (ERR_SUCCESS != UT_CFW_NwAbortListOperators(FREE_UTI, g_pNwContext->sim_id))
#else
	if (ERR_SUCCESS != CFW_NwAbortListOperators(FREE_UTI, g_pNwContext->sim_id))
#endif

#else
	if (ERR_SUCCESS != CFW_NwAbortListOperators(FREE_UTI))
#endif

	{
		mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>>AdaptAbortGetPLMNListToIdle  return synchronization error >>>>\r\n");
		AdpNW_SendMessageToMMI(NULL, PRT_NW_ABORT_PLMN_LIST_RSP);
	}

	return 0;
}

/**************************************************************

	FUNCTION NAME		: DoNETSETAbortPLMNListRsp

	PURPOSE				: send message PRT_NW_ABORT_PLMN_LIST_RSP to mmi

	PARAMETERS	              :

	REMARKS			:

**************************************************************/

static UINT8 DoNETSETAbortPLMNListRsp(COS_EVENT * pCosEvent)
{
	UINT8 nType;
	static UINT8 i = 0;	
	TBM_ENTRY(0x29d7);
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry DoNETSETAbortPLMNListRsp >>>>\r\n");
	ASSERT(pCosEvent != NULL);
	nType = HIUINT8(pCosEvent->nParam3);
	
	mmi_trace(g_sw_ADP_NW,  "MMI_NW: >>>> nType = 0x%x >>>>\r\n",nType);
	if (nType == 0)
		AdpNW_SendMessageToMMI(NULL, PRT_NW_ABORT_PLMN_LIST_RSP);
	else if (0xF0 == nType)
	{
		i++;
		
		if (i > 1)
			AdpNW_SendMessageToMMI(NULL, PRT_NW_ABORT_PLMN_LIST_RSP);
		else
		{
#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			UT_CFW_NwAbortListOperators(FREE_UTI, g_pNwContext->sim_id);
#else
			CFW_NwAbortListOperators(FREE_UTI, g_pNwContext->sim_id);
#endif
#else
			CFW_NwAbortListOperators(FREE_UTI);

#endif
		}
	}
	else
		ASSERT(0);

	TBM_EXIT(0x29d7);

	return 0;
}

/**************************************************************

	FUNCTION NAME		: AdaptEntryNETSETSelectionModeReq

	PURPOSE				: send network selection mode to network

	PARAMETERS	              :

	REMARKS			:

**************************************************************/

static UINT8 AdaptEntryNETSETSelectionModeReq(COS_EVENT* pCosEvent)
{
	ilm_struct *OslMsg;
	mmi_nw_set_plmn_select_mode_req_struct* pSelPLMNMod;
	TBM_ENTRY(0x29d8);
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry AdaptEntryNETSETSelectionModeReq >>>>\r\n");
	ASSERT(pCosEvent != NULL);

	SelModeorRegiFlag = 0;
	OslMsg = (ilm_struct *) pCosEvent->nParam1;
	pSelPLMNMod = (mmi_nw_set_plmn_select_mode_req_struct*)OslMsg->local_para_ptr;

#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT

	if (ERR_SUCCESS != UT_CFW_NwSetRegistration(NULL, pSelPLMNMod->mode, FREE_UTI, g_pNwContext->sim_id))
#else
	if (ERR_SUCCESS != CFW_NwSetRegistration(NULL, pSelPLMNMod->mode, FREE_UTI, g_pNwContext->sim_id))
#endif
#else
	if (ERR_SUCCESS != CFW_NwSetRegistration(NULL, pSelPLMNMod->mode, FREE_UTI))
#endif
	{
		mmi_nw_set_plmn_select_mode_rsp_struct *p;
		p = (mmi_nw_set_plmn_select_mode_rsp_struct *) OslMalloc(sizeof(mmi_nw_set_plmn_select_mode_rsp_struct));
		ASSERT(p != NULL);
		memset(p, 0, sizeof(mmi_nw_set_plmn_select_mode_rsp_struct));
		p->result = KAL_FALSE;
		AdpNW_SendMessageToMMI((local_para_struct *)p, PRT_NW_SET_PLMN_SELECT_MODE_RSP);
	}
	TBM_EXIT(0x29d8);

	return 0;
}

/**************************************************************

	FUNCTION NAME		: DoEntryNETSETSelectionModeRsp

	PURPOSE				: send PRT_NW_SET_PLMN_SELECT_MODE_RSP to mmi

	PARAMETERS	              :

	REMARKS			:

**************************************************************/

static UINT8 DoEntryNETSETSelectionModeRsp(COS_EVENT * pCosEvent)
{
	UINT8 nType;
	mmi_nw_set_plmn_select_mode_rsp_struct* pSelPLMNMod;

	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29d9);
	pSelPLMNMod = (mmi_nw_set_plmn_select_mode_rsp_struct *) OslMalloc(sizeof(mmi_nw_set_plmn_select_mode_rsp_struct));
	ASSERT(pSelPLMNMod != NULL);
	memset(pSelPLMNMod, 0, sizeof(mmi_nw_set_plmn_select_mode_rsp_struct));
	
	nType = HIUINT8(pCosEvent->nParam3);
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry DoEntryNETSETSelectionModeRsp nType=%d", nType);
	
	if (nType == 0)
		pSelPLMNMod->result = KAL_TRUE;
	else if (nType == 0xF0)
		pSelPLMNMod->result = KAL_FALSE;
	else 
		ASSERT(0);

	AdpNW_SendMessageToMMI((local_para_struct *)pSelPLMNMod, PRT_NW_SET_PLMN_SELECT_MODE_RSP);
	TBM_EXIT(0x29d9);

	return 0;
}

/**************************************************************

	FUNCTION NAME		: AdaptManuAutoNetworkRregisterReq

	PURPOSE				: manual or auto register network

	PARAMETERS	              : 

	REMARKS			:

**************************************************************/

static UINT8 AdaptManuAutoNetworkRregisterReq(COS_EVENT* pCosEvent)
{
	ilm_struct *OslMsg;
	mmi_nw_set_plmn_req_struct* pSetPLMNReq;
	UINT8 nOper[MAX_PLMN_LEN];
	UINT8 ret;
	TBM_ENTRY(0x29da);
	ASSERT(pCosEvent != NULL);

	SelModeorRegiFlag = 1;
	OslMsg = (ilm_struct *) pCosEvent->nParam1;
	pSetPLMNReq = (mmi_nw_set_plmn_req_struct*)OslMsg->local_para_ptr;

	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry AdaptManuAutoNetworkRregisterReq pSetPLMNReq->opcode=%d", pSetPLMNReq->opcode);
	if (pSetPLMNReq->opcode == AUTO_SELECT)
	{
#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		ret = UT_CFW_NwSetRegistration(NULL, pSetPLMNReq->opcode, FREE_UTI, g_pNwContext->sim_id);
#else
		mmi_trace(g_sw_ADP_NW,"MMI_NW: Func: %s simid:%d", __FUNCTION__, g_pNwContext->sim_id);
		ret = CFW_NwSetRegistration(NULL, pSetPLMNReq->opcode, FREE_UTI, g_pNwContext->sim_id);
#endif
#else
		ret = CFW_NwSetRegistration(NULL, pSetPLMNReq->opcode, FREE_UTI);
#endif
	}
	else
	{
		CSDPLMN2CswPLMN(nOper,  pSetPLMNReq->oper, MAX_PLMN_LEN);
#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		ret = UT_CFW_NwSetRegistration(nOper, pSetPLMNReq->opcode, FREE_UTI, g_pNwContext->sim_id);
#else
		mmi_trace(g_sw_ADP_NW,"MMI_NW: Func: %s simid:%d", __FUNCTION__, g_pNwContext->sim_id);
		ret = CFW_NwSetRegistration(nOper, pSetPLMNReq->opcode, FREE_UTI, g_pNwContext->sim_id);
#endif
#else
		ret = CFW_NwSetRegistration(nOper, pSetPLMNReq->opcode, FREE_UTI);

#endif
	}

	//mmi_trace(g_sw_ADP_NW, ">>>> pSetPLMNReq->oper[0] = %d,pSetPLMNReq->opcode =%d>>>>\r\n",pSetPLMNReq->oper[0],pSetPLMNReq->opcode);
	//mmi_trace(g_sw_ADP_NW, ">>>> pSetPLMNReq->oper[1] = %d>>>>\r\n",pSetPLMNReq->oper[1]);
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry AdaptManuAutoNetworkRregisterReq ret=0x%x", ret);
	if (ret != ERR_SUCCESS)
	{
		mmi_nw_set_plmn_rsp_struct *pSetPLMNRsp;
		pSetPLMNRsp = (mmi_nw_set_plmn_rsp_struct *)OslMalloc(sizeof(mmi_nw_set_plmn_rsp_struct));
		ASSERT(pSetPLMNRsp != NULL);
		memset(pSetPLMNRsp, 0, sizeof(mmi_nw_set_plmn_rsp_struct));
		pSetPLMNRsp->result.flag = L4C_ERROR;
		AdpNW_SendMessageToMMI((local_para_struct *)pSetPLMNRsp, PRT_NW_SET_PLMN_RSP);
	}
	TBM_EXIT(0x29da);

	return 0;
}

/**************************************************************

	FUNCTION NAME		: DoNETSETManuAutoSelectNetworkRsp

	PURPOSE				: send message PRT_NW_SET_PLMN_RSP to mmi

	PARAMETERS	              : 

	REMARKS			:

**************************************************************/

static UINT8 DoNETSETManuAutoSelectNetworkRsp(COS_EVENT* pCosEvent)
{
	UINT8 nType;
	mmi_nw_set_plmn_rsp_struct* pSetPLMNRsp;
	UINT8 *CswRegiPlMN;

	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29db);
	pSetPLMNRsp = (mmi_nw_set_plmn_rsp_struct *)OslMalloc(sizeof(mmi_nw_set_plmn_rsp_struct));
	ASSERT(pSetPLMNRsp != NULL);
	memset(pSetPLMNRsp, 0, sizeof(mmi_nw_set_plmn_rsp_struct));

	CswRegiPlMN = (UINT8 *)pCosEvent->nParam1;
	nType = HIUINT8(pCosEvent->nParam3);

	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry DoNETSETManuAutoSelectNetworkRsp nType=%d", nType);
	if (nType == 0)
	{
		CswPLMN2CSDPLMN(pSetPLMNRsp->plmn, CswRegiPlMN, MAX_PLMN_LEN);
		pSetPLMNRsp->result.flag = L4C_OK;
		pSetPLMNRsp->status = L4C_RAC_OK;
	}
	else if (nType == 0xF0)
	{
		pSetPLMNRsp->result.flag = L4C_ERROR;
		//pSetPLMNRsp->status = pCosEvent->nParam1;  
	}
	else
		ASSERT(0);
	AdpNW_SendMessageToMMI((local_para_struct *)pSetPLMNRsp, PRT_NW_SET_PLMN_RSP);
	TBM_EXIT(0x29db);

	return 0;
}

//[221] Added by ZXB 2009/02/10								[BEGIN]
#ifdef __ADP_MULTI_SIM__
U8 CfunReqBuffered = 0;
U8 CfunReqForTemp = 0;
static UINT8 DoNwCfunRsp(COS_EVENT *pCosEvent)
{
	UINT32 ret = ERR_SUCCESS;
	U8 set = ADP_GetCfwDmSetting();
	U8 i;
	CFW_COMM_MODE bEnable;
	
	TRC_ENTER_FUNC(g_sw_ADP_NW);

	mmi_trace(g_sw_ADP_NW,"ModeSetting Func: %s nparam1: %x, nparam2: %x, nparam3: %x", __FUNCTION__, pCosEvent->nParam1, pCosEvent->nParam2, pCosEvent->nParam3);

#ifdef ADP_DUALSIM_MODE_CLOSE_NOSIM
	if (pCosEvent->nParam2 == 1)  //function ADP_CloseNoSIM() called. don't need RSP.
	{
		if(!CfunReqBuffered)
		{
			ADP_SimulateNoSignal(g_pNwContext->sim_id);
			ADP_CloseNoSIM();
			return 0;
		}
		else
		{
			CfunReqBuffered--;
			if(CfunReqBuffered != 0)
				return 0;
		}
	}
#endif

	////////////////////////////////////////
	// CFW will return error type when no sim inserted in card slot, 
	// but it has done this switch operation. So, need check this case.
	if (adp_GetCswEvType(pCosEvent) == 0xF0 && pCosEvent->nParam1 != ERR_CME_SIM_NOT_INSERTED)
	{
		ProcCfunFailed();
		return 0;
	}
	
	g_DmPrevState = set;
	mmi_trace(g_sw_ADP_NW, "DoNwCfunRsp, g_DmPrevState is 0x%x, g_DmState is 0x%x", g_DmPrevState,g_DmState);
	if (g_DmState == g_DmPrevState)
	{
		ProcCfunSuccess();
		ADP_InitCloseNoSIM();
		ADP_CloseNoSIM();
	}
	else
	{
		for(i = 0; i < ADP_SIM_NUMBER; i++)
		{
			if((g_DmState&(0x01<<i)) != (g_DmPrevState&(0x01<<i)))
			{
				if(g_DmState&(0x01<<i))
				{
					bEnable = CFW_ENABLE_COMM;
				}
				else
				{
					bEnable = CFW_DISABLE_COMM;
				}
				ret = CFW_SetComm(bEnable, 0,  FREE_UTI, i);
				if(ERR_SUCCESS != ret)
				{
					mmi_trace(g_sw_ADP_NW, "DoNwCfunRsp, Call CFW_SetComm() error. ret=0x%x,bEnable is %d", ret,bEnable);
					ProcCfunFailed();
				}
				else
				{
					mmi_trace(g_sw_ADP_NW, "DoNwCfunRsp, Call CFW_SetComm() success. bEnable is %d", bEnable);
				}
				break;
			}	
		}
	}

	return 0;
}


static UINT8 AdaptNwProcCfunReq(COS_EVENT *pCosEvent)
{
	ilm_struct *p_ilm = (ilm_struct *)pCosEvent->nParam1;
	mmi_nw_cfun_state_req_struct *p = (mmi_nw_cfun_state_req_struct *)p_ilm->local_para_ptr;
	U8 set = ADP_GetCfwDmSetting();
	UINT32 ret = ERR_SUCCESS;
	U8 i;
	CFW_COMM_MODE bEnable;

	g_DmPrevState = set;
	g_DmState = p->cfun_state;

	mmi_trace(g_sw_ADP_NW,"AdaptNwProcCfunReq, g_DmState=0x%x, g_DmPrevState=0x%x", g_DmState, g_DmPrevState);

	if (g_DmState == g_DmPrevState)
	{
		ProcCfunFailed();
		return 0;
	}

#ifdef ADP_DUALSIM_MODE_CLOSE_NOSIM
	CfunReqBuffered = ADP_ReopNoSIM();
	if(CfunReqBuffered != 0)
		return 0;
#endif

	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		if((g_DmState&(0x01<<i)) != (g_DmPrevState&(0x01<<i)))
		{
			if(g_DmState&(0x01<<i))
			{
				bEnable = CFW_ENABLE_COMM;
			}
			else
			{
				bEnable = CFW_DISABLE_COMM;
			}
			ret = CFW_SetComm(bEnable, 0, FREE_UTI, i);
			if(ERR_SUCCESS != ret)
			{
				mmi_trace(g_sw_ADP_NW, "AdaptNwProcCfunReq, Call CFW_SetComm() error. ret=0x%x, bEnable is %d", ret, bEnable);
				ProcCfunFailed();
			}
			else
			{
				mmi_trace(g_sw_ADP_NW, "AdaptNwProcCfunReq, Call CFW_SetComm() success. bEnable is %d", bEnable);
			}
			break;
		}
			
	}

	return 0;
}
extern BOOL gSIMIsNotExist[];
extern BOOL bNoSIMHandled[];
static U8 AdaptNwFindFirstUsedSIMIndex(U8 nStartIndex)
{
	U8 dmSet = 0;
	U8 nSIM = 0;
	U8 i;
	dmSet = ADP_GetCfwDmSetting();

	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		if(!gSIMIsNotExist[i]&&(dmSet&(0x01<<i)))
		{
			nSIM++;
		}
	}
	mmi_trace(g_sw_ADP_NW, "AdaptNwFindFirstUsedSIMIndex, used sim number is %d", nSIM);
	//search firset sim index on using
	if(nSIM)
	{
		for(i = nStartIndex; i < ADP_SIM_NUMBER;i++)
		{
			if(!gSIMIsNotExist[i]&&dmSet&(0x01<<i))
				break;
		}
	}
	else
	{
		for(i = nStartIndex; i < ADP_SIM_NUMBER; i++)
		{
			if(dmSet & (0x01<<i)&&!bNoSIMHandled[i])
				break;
		}
	}
	mmi_trace(g_sw_ADP_NW, "AdaptNwFindFirstUsedSIMIndex, used sim index is %d", i);
	return i;
}
static void CloseStackFailed(void)
{
	mmi_nw_cfun_state_rsp_struct *p = NULL;

	p = (mmi_nw_cfun_state_rsp_struct *)OslMalloc(sizeof(mmi_nw_cfun_state_rsp_struct));
	MMI_ASSERT(p != NULL);
	memset(p, 0, sizeof(mmi_nw_cfun_state_rsp_struct));
	p->result = KAL_FALSE;

	mmi_trace(g_sw_ADP_NW, "ModeSetting: PRT_NW_CFUN_TEMP_CLOSE_STACK_RSP to MMI, ->result==KAL_FALSE");
	g_pNwContext->sim_id = 0;
	CfunReqForTemp = 0;
	AdpNW_SendMessageToMMI((local_para_struct *)p, PRT_NW_CFUN_TEMP_CLOSE_STACK_RSP);
}

static void CloseStackSuccess(void)
{
	mmi_nw_cfun_state_rsp_struct *p = NULL;

	p = (mmi_nw_cfun_state_rsp_struct *)OslMalloc(sizeof(mmi_nw_cfun_state_rsp_struct));
	MMI_ASSERT(p != NULL);
	memset(p, 0, sizeof(mmi_nw_cfun_state_rsp_struct));
	p->result = KAL_TRUE;

	mmi_trace(g_sw_ADP_NW, "ModeSetting: PRT_NW_CFUN_TEMP_CLOSE_STACK_RSP to MMI, ->result==KAL_TRUE");
	g_pNwContext->sim_id = 0;
	CfunReqForTemp = 0;
	AdpNW_SendMessageToMMI((local_para_struct *)p, PRT_NW_CFUN_TEMP_CLOSE_STACK_RSP);
}
static UINT8 CloseStackRsp(COS_EVENT *pCosEvent)
{
	U8 startSIMIndex = g_pNwContext->sim_id + 1;
	
	UINT32 ret = ERR_SUCCESS;
	
	if (adp_GetCswEvType(pCosEvent) == 0xF0 && pCosEvent->nParam1 != ERR_CME_SIM_NOT_INSERTED)
	{
		CloseStackFailed();
		return 0;
	}

	startSIMIndex = AdaptNwFindFirstUsedSIMIndex(startSIMIndex);
	if(startSIMIndex < ADP_SIM_NUMBER)
	{
		ret = CFW_SetComm(CFW_DISABLE_COMM, 1, FREE_UTI, startSIMIndex);
		
		if(ERR_SUCCESS != ret)
		{
			mmi_trace(g_sw_ADP_NW, "AdaptNwProcTempCfunCloseStackReq, Call CFW_SetComm() for sim%d error. ret=0x%x", startSIMIndex, ret);
			CloseStackFailed();
		}
		else
		{
			mmi_trace(g_sw_ADP_NW, "AdaptNwProcCfunReq, Call CFW_SetComm() fir sim%d success. ", startSIMIndex);
		}
	}
	else
	{
		CloseStackSuccess();
	}
	
}
static UINT8 AdaptNwProcTempCfunCloseStackReq(COS_EVENT *pCosEvent)
{
	U8 startSIMIndex = 0;
	UINT32 ret = ERR_SUCCESS;
	
	startSIMIndex = AdaptNwFindFirstUsedSIMIndex(startSIMIndex);

	
	if(startSIMIndex < ADP_SIM_NUMBER)
	{
		ret = CFW_SetComm(CFW_DISABLE_COMM, 1, FREE_UTI, startSIMIndex);
		
		if(ERR_SUCCESS != ret)
		{
			mmi_trace(g_sw_ADP_NW, "AdaptNwProcTempCfunCloseStackReq, Call CFW_SetComm() for sim%d error. ret=0x%x", startSIMIndex, ret);
			CloseStackFailed();
		}
		else
		{
			mmi_trace(g_sw_ADP_NW, "AdaptNwProcCfunReq, Call CFW_SetComm() fir sim%d success. ", startSIMIndex);
		}
	}
	else
	{
		CloseStackSuccess();
	}
	
}
#endif
//[221] Added by ZXB 2009/02/10								[END]

/**************************************************************

	FUNCTION NAME		: AdaptNETGetImeiReq

	PURPOSE			: get imei

	PARAMETERS	              : 

	REMARKS			:

**************************************************************/

static UINT8 AdaptNETGetImeiReq(COS_EVENT *pCosEvent)//man for imei
{
	mmi_nw_get_imei_rsp_struct *GetImeiRsp;
	U8 i;
	ASSERT(pCosEvent != NULL);

	GetImeiRsp = (mmi_nw_get_imei_rsp_struct *)OslMalloc(sizeof(mmi_nw_get_imei_rsp_struct));
	ASSERT(GetImeiRsp != NULL);
	memset(GetImeiRsp, 0, sizeof(mmi_nw_get_imei_rsp_struct));


	for(i = 0 ; i < ADP_SIM_NUMBER; i++)
	{
		#ifdef __ADP_MULTI_SIM__
		CFW_EmodGetIMEI(GetImeiRsp->imei[i], (UINT8 *)&GetImeiRsp->result, i);
		#else
		CFW_EmodGetIMEI(GetImeiRsp->imei, (UINT8 *)&GetImeiRsp->result);
		#endif
	}
	
	AdpNW_SendMessageToMMI((local_para_struct *)GetImeiRsp, PRT_NW_GET_IMEI_RSP);

	return 0;
}

/**************************************************************

	FUNCTION NAME		: AdaptNETSETGetBandSelectionReq

	PURPOSE				: get band value and send PRT_NW_GET_BAND_RSP to mmi

	PARAMETERS	              :

	REMARKS			:

**************************************************************/

static UINT8 AdaptNETSETGetBandSelectionReq(COS_EVENT *pCosEvent)
{
	UINT8 Band;
	mmi_nw_get_band_rsp_struct *GetBandRsp;
	UINT16 GetBandFlag;

	ASSERT(pCosEvent != NULL);
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry AdaptNETSETGetBandSelectionReq >>>>\r\n");

	GetBandRsp = (mmi_nw_get_band_rsp_struct *)OslMalloc(sizeof(mmi_nw_get_band_rsp_struct));
	ASSERT(GetBandRsp != NULL);
	memset(GetBandRsp, 0, sizeof(mmi_nw_get_band_rsp_struct));

#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	GetBandFlag = UT_CFW_CfgNwGetFrequencyBand(&Band, g_pNwContext->sim_id);
#else
	GetBandFlag = CFW_CfgNwGetFrequencyBand(&Band, g_pNwContext->sim_id);
#endif
#else
	GetBandFlag = CFW_CfgNwGetFrequencyBand(&Band);
#endif

	if (GetBandFlag == ERR_SUCCESS)
	{
		GetBandRsp->result = KAL_TRUE;
		CswBand2CSDBand(&(GetBandRsp->type), Band);
		mmi_trace(g_sw_ADP_NW, ">>>> the band = 0x%x, the mmi band = 0x%x", Band, GetBandRsp->type);
	}
	else
	{
		GetBandRsp->result = KAL_FALSE;
	}

	AdpNW_SendMessageToMMI((local_para_struct *)GetBandRsp, PRT_NW_GET_BAND_RSP);

	return 0;
}

/**************************************************************

	FUNCTION NAME		: AdaptNETSETGetPLMNListReq

	PURPOSE				: get available PLMN list

	PARAMETERS	              :

	REMARKS			:

**************************************************************/

static UINT8 AdaptNETSETGetPLMNListReq(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);

#ifdef __ADP_MULTI_SIM__
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry AdaptNETSETGetPLMNListReq >>>>for sim%d", g_pNwContext->sim_id);

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT

	if (ERR_SUCCESS != UT_CFW_NwGetAvailableOperators(FREE_UTI, g_pNwContext->sim_id))
#else
	if (ERR_SUCCESS != CFW_NwGetAvailableOperators(FREE_UTI, g_pNwContext->sim_id))
#endif

#else
	if (ERR_SUCCESS != CFW_NwGetAvailableOperators(FREE_UTI))
#endif
	{
		mmi_nw_get_plmn_list_rsp_struct *GetPLMNListRsp;
		GetPLMNListRsp = (mmi_nw_get_plmn_list_rsp_struct *)OslMalloc(sizeof(mmi_nw_get_plmn_list_rsp_struct));
		ASSERT(NULL != GetPLMNListRsp);
		memset(GetPLMNListRsp, 0, sizeof(mmi_nw_get_plmn_list_rsp_struct));
		GetPLMNListRsp->result.flag = L4C_ERROR;
		AdpNW_SendMessageToMMI((local_para_struct *)GetPLMNListRsp, PRT_NW_GET_PLMN_LIST_RSP);
	}

	return 0;
}

/**************************************************************

	FUNCTION NAME		: DoNETSETGetPLMNListRsp

	PURPOSE				: send message PRT_NW_GET_PLMN_LIST_RSP to mmi

	PARAMETERS	              :

	REMARKS			:

**************************************************************/

static UINT8 DoNETSETGetPLMNListRsp(COS_EVENT* pCosEvent)
{
	UINT8 nType;
	UINT8 num = 0;
	CFW_NW_OPERATOR_INFO *CswGetPLMNList;
	mmi_nw_get_plmn_list_rsp_struct *GetPLMNListRsp;
	TBM_ENTRY(0x29dc);
	ASSERT(NULL != pCosEvent);
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry DoNETSETGetPLMNListRsp >>>>\r\n");
	
	GetPLMNListRsp = (mmi_nw_get_plmn_list_rsp_struct *)OslMalloc(sizeof(mmi_nw_get_plmn_list_rsp_struct));
	ASSERT(NULL != GetPLMNListRsp);
	memset(GetPLMNListRsp, 0, sizeof(mmi_nw_get_plmn_list_rsp_struct));

	CswGetPLMNList = (CFW_NW_OPERATOR_INFO*)pCosEvent->nParam1;
	nType = HIUINT8(pCosEvent->nParam3);	

	if (0 == nType)
	{
		GetPLMNListRsp->result.flag = L4C_OK;
		GetPLMNListRsp->result.cause = 0;
		GetPLMNListRsp->length = (UINT8)pCosEvent->nParam2;
		for (num = 0; num < pCosEvent->nParam2; num++)
		{
			CswPLMN2CSDPLMN(GetPLMNListRsp->list[num].plmn_id, CswGetPLMNList[num].nOperatorId, MAX_PLMN_LEN);
			GetPLMNListRsp->list[num].status = CswGetPLMNList[num].nStatus;
		}
	}
	else if (nType == 0xF0)
	{
		GetPLMNListRsp->result.flag  = L4C_ERROR;
		GetPLMNListRsp->result.cause = 0;
	}
	else
		ASSERT(0);

	AdpNW_SendMessageToMMI((local_para_struct *)GetPLMNListRsp, PRT_NW_GET_PLMN_LIST_RSP);
	TBM_EXIT(0x29dc);

	return 0;
}

/**************************************************************

	FUNCTION NAME		: DoNITZHandler

	PURPOSE				: send message PRT_MSG_ID_MMI_NW_TIME_ZONE_IND to mmi

	PARAMETERS	              : 

	REMARKS			:display time when time zone changes

**************************************************************/

static UINT8 DoNITZHandler(COS_EVENT *pCosEvent)
{
    mmi_nw_time_zone_ind_struct *TimeZoneInd;
    CFW_NW_NETWORK_INFO *NetworkInfo;

    ASSERT(NULL != pCosEvent);

    TimeZoneInd = (mmi_nw_time_zone_ind_struct *)OslMalloc(sizeof(mmi_nw_time_zone_ind_struct));
    ASSERT(NULL != TimeZoneInd);
    memset(TimeZoneInd, 0, sizeof(mmi_nw_time_zone_ind_struct));

    NetworkInfo = (CFW_NW_NETWORK_INFO *)pCosEvent->nParam1;

    TimeZoneInd->nw_time_zone_timeP = NetworkInfo->nUniversalTimeZoneLen;
    TimeZoneInd->nw_time_zone_time.year = NetworkInfo->nUniversalTimeZone[0];
    TimeZoneInd->nw_time_zone_time.month = NetworkInfo->nUniversalTimeZone[1];
    TimeZoneInd->nw_time_zone_time.day = NetworkInfo->nUniversalTimeZone[2];
    TimeZoneInd->nw_time_zone_time.hour = NetworkInfo->nUniversalTimeZone[3];
    TimeZoneInd->nw_time_zone_time.min = NetworkInfo->nUniversalTimeZone[4];
    TimeZoneInd->nw_time_zone_time.sec = NetworkInfo->nUniversalTimeZone[5];
    TimeZoneInd->nw_time_zone_time.time_zone = NetworkInfo->nUniversalTimeZone[6];

    TimeZoneInd->nw_time_zoneP = NetworkInfo->nLocalTimeZoneLen;
    TimeZoneInd->nw_time_zone = NetworkInfo->nLocalTimeZone;

    TimeZoneInd->nw_day_light_saving_timeP = NetworkInfo->nDaylightSavingTimeLen;
    TimeZoneInd->nw_day_light_saving_time = NetworkInfo->nDaylightSavingTime;

    TimeZoneInd->full_nw_nameP = NetworkInfo->nNwkFullNameLen;
    TimeZoneInd->full_nw_name_dcs = NetworkInfo->nNwkNames[0] & 0xF0;
    if(TimeZoneInd->full_nw_name_dcs & 0x10)
    {
        TimeZoneInd->full_nw_name_dcs = SMSAL_UCS2_DCS;
    }
    TimeZoneInd->full_nw_name_add_ci = NetworkInfo->nNwkNames[0] & 0x0F;
    TimeZoneInd->full_nw_name_len = TimeZoneInd->full_nw_nameP - 1;
    memcpy(TimeZoneInd->full_nw_name, NetworkInfo, 24);
    TimeZoneInd->full_nw_name[23] = 0x00;

    TimeZoneInd->short_nw_nameP = NetworkInfo->nNwkShortNameLen;
    TimeZoneInd->short_nw_name_dcs = NetworkInfo->nNwkNames[TimeZoneInd->full_nw_nameP] & 0xF0;
    if(TimeZoneInd->short_nw_name_dcs & 0x10)
    {
        TimeZoneInd->short_nw_name_dcs = SMSAL_UCS2_DCS;
    }    
    TimeZoneInd->short_nw_name_add_ci = NetworkInfo->nNwkNames[TimeZoneInd->full_nw_nameP] & 0x0F;
    TimeZoneInd->short_nw_name_len= TimeZoneInd->short_nw_nameP - 1;
    memcpy(TimeZoneInd->short_nw_name, NetworkInfo, 24);
    TimeZoneInd->short_nw_name[23] = 0x00;
    
    mmi_trace(g_sw_ADP_NW, "ADP_MMI_NW func:%s, %d, %d, %d, %d, %d, %d, %d", 
    __FUNCTION__, 
    NetworkInfo->nUniversalTimeZone[0],
    NetworkInfo->nUniversalTimeZone[1],
    NetworkInfo->nUniversalTimeZone[2],
    NetworkInfo->nUniversalTimeZone[3],
    NetworkInfo->nUniversalTimeZone[4],
    NetworkInfo->nUniversalTimeZone[5],
    NetworkInfo->nUniversalTimeZone[6]);
    
    mmi_trace(g_sw_ADP_NW, "ADP_MMI_NW func:%s %d, %d, %d, %d, %d, %d", 
    __FUNCTION__, 
    TimeZoneInd->full_nw_name_dcs,
    TimeZoneInd->full_nw_name_add_ci,
    TimeZoneInd->full_nw_name_len,
    TimeZoneInd->short_nw_name_dcs,
    TimeZoneInd->short_nw_name_add_ci,
    TimeZoneInd->short_nw_name_len);    
    //kal_uint8	plmn[MAX_PLMN_LEN+1];   

    AdpNW_SendMessageToMMI((local_para_struct *)TimeZoneInd, PRT_MSG_ID_MMI_NW_TIME_ZONE_IND);

    return 0;
}
void SetBandFirstPowerOn()
{
	BOOL bFPO = FALSE;
	U8 i;
	UINT8 CSDBand_Auto = 0
#ifdef __GSM850__
				| CFW_NW_BAND_GSM_850
#endif
#ifdef __EGSM900__
				| CFW_NW_BAND_GSM_900P | CFW_NW_BAND_GSM_900E 
#endif
#ifdef __DCS1800__
				| CFW_NW_BAND_DCS_1800
#endif
#ifdef __PCS1900__
				| CFW_NW_BAND_PCS_1900
#endif
				;
	CFW_CfgCheckFirstPowerOnFlag(&bFPO);
	mmi_trace(g_sw_ADP_NW,"MMI_NW: Func: %s bFPO:%d, CSDBand_Auto:0x%x", __FUNCTION__, bFPO, CSDBand_Auto);
	if (bFPO)
	{
		int ret;
#if 0
#ifdef __ADP_MULTI_SIM__
		ret = CFW_NwSetFrequencyBand(CSDBand_Auto, FREE_UTI, ADP_SIM_1);
		mmi_trace(TRUE,"guoj++ 111 Func: %s ret=0x%x", __FUNCTION__, ret);
		ret = CFW_NwSetFrequencyBand(CSDBand_Auto, FREE_UTI, ADP_SIM_2);
		mmi_trace(TRUE,"guoj++ 222 Func: %s ret=0x%x", __FUNCTION__, ret);
#else
		CFW_NwSetFrequencyBand(CSDBand_Auto, FREE_UTI);
#endif
#else
		for(i = 0; i < ADP_SIM_NUMBER; i++)
		{
			#ifdef __ADP_MULTI_SIM__
			ret = CFW_CfgNwSetFrequencyBand(CSDBand_Auto, i);
			#else
			ret = CFW_CfgNwSetFrequencyBand(CSDBand_Auto);
			#endif
		}
#endif
	}	
}
/**************************************************************

	FUNCTION NAME		: AdaptNETSETSetBandSelectionReq

	PURPOSE				: set band value

	PARAMETERS	              :

	REMARKS			:
**************************************************************/

static UINT8 AdaptNETSETSetBandSelectionReq(COS_EVENT *pCosEvent)
{
	ilm_struct *OslMsg;
	mmi_nw_set_preferred_band_req_struct *SetPrefBandReq;
	UINT8 ncswband;
	UINT32 ret;

	ASSERT(NULL != pCosEvent);

	OslMsg = (ilm_struct *)pCosEvent->nParam1;
	SetPrefBandReq = (mmi_nw_set_preferred_band_req_struct *)OslMsg->local_para_ptr;

	CSDBand2CswBand(&ncswband, SetPrefBandReq->band);
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> SetPrefBandReq->band = %d >>>>\r\n", SetPrefBandReq->band);
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> ncswband = %d >>>>\r\n", ncswband);
#ifdef __ADP_MULTI_SIM__
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry AdaptNETSETSetBandSelectionReq >>>>for sim%d", g_pNwContext->sim_id);
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT

	ret = UT_CFW_NwSetFrequencyBand(ncswband, FREE_UTI, g_pNwContext->sim_id);
#else
	ret = CFW_NwSetFrequencyBand(ncswband, FREE_UTI, g_pNwContext->sim_id);
#endif
#else
	ret = CFW_NwSetFrequencyBand(ncswband, FREE_UTI);
#endif
	if(ret != ERR_SUCCESS)
	{
		mmi_nw_set_preferred_band_rsp_struct *SetPrefBandRsp;
		SetPrefBandRsp = (mmi_nw_set_preferred_band_rsp_struct *)OslMalloc(sizeof(mmi_nw_set_preferred_band_rsp_struct));
		ASSERT(NULL != SetPrefBandRsp);
		memset(SetPrefBandRsp, 0, sizeof(mmi_nw_set_preferred_band_rsp_struct));
		SetPrefBandRsp->result = MMI_FALSE;
		mmi_trace(g_sw_ADP_NW, "MMI_NW: call CFW_NwSetFrequencyBand failed with ret: 0x%x", ret);
		AdpNW_SendMessageToMMI((local_para_struct *)SetPrefBandRsp, PRT_NW_SET_BAND_RSP);
	}

	return 0;
}

/**************************************************************

	FUNCTION NAME		: DoNETSETSetBandSelectionRsp

	PURPOSE				: send message PRT_NW_SET_BAND_RSP to mmi

	PARAMETERS	              :

	REMARKS			:
**************************************************************/

static UINT8 DoNETSETSetBandSelectionRsp(COS_EVENT *pCosEvent)
{
	UINT8 nType;
	mmi_nw_set_preferred_band_rsp_struct *SetPrefBandRsp;

	ASSERT(NULL != pCosEvent);
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry DoNETSETSetBandSelectionRsp >>>>");
#ifdef __ADP_MULTI_SIM__	
	mmi_trace(g_sw_ADP_NW, "MMI_NW: for sim%d", g_pNwContext->sim_id);
#endif

	SetPrefBandRsp = (mmi_nw_set_preferred_band_rsp_struct *)OslMalloc(sizeof(mmi_nw_set_preferred_band_rsp_struct));
	ASSERT(NULL != SetPrefBandRsp);
	memset(SetPrefBandRsp, 0, sizeof(mmi_nw_set_preferred_band_rsp_struct));

	nType = HIUINT8(pCosEvent->nParam3);

	if (nType == 0)
	{
		SetPrefBandRsp->result = MMI_TRUE;
	}
	else if (nType == 0xF0)
	{
		SetPrefBandRsp->result = MMI_FALSE;
	}
	else
		ASSERT(0);

	AdpNW_SendMessageToMMI((local_para_struct *)SetPrefBandRsp, PRT_NW_SET_BAND_RSP);

	return 0;
}

/**************************************************************

	FUNCTION NAME		: AdaptRequestRacDetach

	PURPOSE				: detach the network registration

	PARAMETERS	              :

	REMARKS			:
**************************************************************/

static UINT8 AdaptRequestRacDetach(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry AdaptRequestRacDetach >>>>\r\n");

#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	if (ERR_SUCCESS != UT_CFW_NwDeRegister(FREE_UTI, g_pNwContext->sim_id))
#else
	if (ERR_SUCCESS != CFW_NwDeRegister(FREE_UTI, g_pNwContext->sim_id))
#endif
#else
	if (ERR_SUCCESS != CFW_NwDeRegister(FREE_UTI))
#endif
	{
		mmi_nw_pwroff_detach_rsp_struct *PwroffDetachRsp;
		PwroffDetachRsp = (mmi_nw_pwroff_detach_rsp_struct *)OslMalloc(sizeof(mmi_nw_pwroff_detach_rsp_struct));
		ASSERT(NULL != PwroffDetachRsp);
		memset(PwroffDetachRsp, 0, sizeof(mmi_nw_pwroff_detach_rsp_struct));
		PwroffDetachRsp->result.flag = L4C_ERROR;
		AdpNW_SendMessageToMMI((local_para_struct *)PwroffDetachRsp, MSG_ID_MMI_NW_PWROFF_DETACH_RSP);
	}

	return 0;
}

/**************************************************************

	FUNCTION NAME		: DoNwPlmnSetRSP

	PURPOSE				: send MSG_ID_MMI_NW_PWROFF_DETACH_RSP to mmi

	PARAMETERS	              :

	REMARKS			:
**************************************************************/

static UINT8 DoNwPlmnSetRSP(COS_EVENT *pCosEvent)
{
	UINT8 nType;
	mmi_nw_pwroff_detach_rsp_struct *PwroffDetachRsp;

	ASSERT(NULL != pCosEvent);
	mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> entry DoNwPlmnSetRSP >>>>\r\n");

	nType = HIUINT8(pCosEvent->nParam3);

	PwroffDetachRsp = (mmi_nw_pwroff_detach_rsp_struct *)OslMalloc(sizeof(mmi_nw_pwroff_detach_rsp_struct));
	ASSERT(NULL != PwroffDetachRsp);
	memset(PwroffDetachRsp, 0, sizeof(mmi_nw_pwroff_detach_rsp_struct));

	if (nType == 0)
	{
		PwroffDetachRsp->result.flag = L4C_OK;
	}
	else if (nType == 0xF0)
	{
		PwroffDetachRsp->result.flag = L4C_ERROR;
	}
	else
		ASSERT(0);

	AdpNW_SendMessageToMMI((local_para_struct *)PwroffDetachRsp, MSG_ID_MMI_NW_PWROFF_DETACH_RSP);

	//mmi_trace(g_sw_ADP_NW, ">>>> send  MSG_ID_MMI_NW_PWROFF_DETACH_RSP>>>>\r\n");

	return 0;
}


#ifdef __MMI_STOP_SIGNAL_REPORT__
UINT8 AdaptStopSignalReport(COS_EVENT *pCosEvent)
{
	mmi_nw_stop_signal_req_struct *pStopSignalReq;
	ilm_struct *OslMsg;
	U32 ret;
	U8 i;
	ASSERT(pCosEvent != NULL);

	OslMsg = (ilm_struct *) pCosEvent->nParam1;
	pStopSignalReq = (mmi_nw_stop_signal_req_struct*)OslMsg->local_para_ptr;

	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
	#ifdef __ADP_MULTI_SIM__
		ret= CFW_SetQualReport(pStopSignalReq->nStart, i);
	#else
		ret= CFW_SetQualReport(pStopSignalReq->nStart);
	#endif
	}
	return 0;
}
#endif

/**************************************************************

	FUNCTION NAME		: ADP_DispatchNwIndMsg

	PURPOSE				: process all active stack event
	
	PARAMETERS	              : 

	REMARKS			:
**************************************************************/

static BOOL ADP_DispatchNwIndMsg(COS_EVENT* pCosEvent)
{
	TBM_ENTRY(0x29b7);
	ASSERT(pCosEvent != NULL);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_NW, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif

	DoNITZHandler(pCosEvent);
	TBM_EXIT(0x29b7);
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchNwReqMsg

	PURPOSE				: process all message from mmi

	PARAMETERS	              :

	REMARKS			:
**************************************************************/
static BOOL ADP_DispatchNwReqMsg(COS_EVENT *pCosEvent)
{
	TBM_ENTRY(0x29b6);
	ASSERT(pCosEvent != NULL);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_NW, adp_GetMMISimID(ADP_GET_MMI_DEST_MOD(pCosEvent->nParam3)));
#endif

	switch (pCosEvent->nEventId)
	{
		case PRT_NW_GET_IMEI_REQ://man for imei
			mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> receive PRT_NW_GET_IMEI_REQ>>>>\r\n");
			AdaptNETGetImeiReq(pCosEvent);
		break;
		case PRT_NW_GET_BAND_REQ:
			mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> receive PRT_NW_GET_BAND_REQ>>>>\r\n");
			AdaptNETSETGetBandSelectionReq(pCosEvent);
			break;

		case PRT_NW_GET_PLMN_LIST_REQ:
			mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> receive PRT_NW_GET_PLMN_LIST_REQ>>>>\r\n");
			AdaptNETSETGetPLMNListReq(pCosEvent);	 
			break;

		case PRT_NW_SET_PLMN_REQ:
			mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> receive PRT_NW_SET_PLMN_REQ>>>>\r\n");
			AdaptManuAutoNetworkRregisterReq(pCosEvent);
			break;
			
		case PRT_NW_ABORT_PLMN_LIST_REQ:
			mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> receive PRT_NW_ABORT_PLMN_LIST_REQ>>>>\r\n");
			AdaptAbortGetPLMNListToIdle(pCosEvent);
			break;
			
		case PRT_NW_SET_PLMN_SELECT_MODE_REQ:
			mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> receive PRT_NW_SET_PLMN_SELECT_MODE_REQ>>>>\r\n");
			AdaptEntryNETSETSelectionModeReq(pCosEvent);
			break;
			
		case PRT_NW_SET_BAND_REQ:
			mmi_trace(g_sw_ADP_NW, "MMI_NW: >>>> receive PRT_NW_SET_BAND_REQ>>>>\r\n");
			AdaptNETSETSetBandSelectionReq(pCosEvent);
			break;
			
//This message PRT_PREFERRED_NETWORKS_REQ is processed in SIM events
#if 0
		case PRT_PREFERRED_NETWORKS_REQ:
			mmi_trace(g_sw_ADP_NW, ">>>> receive PRT_PREFERRED_NETWORKS_REQ>>>>\r\n");
			AdaptNETSETGetPreferredNetworkReq(pCosEvent);
			break;

		case MSG_ID_MMI_NW_SET_PREFERRED_BAND_REQ:
			break;

		case PRT_SET_PREFERRED_NETWORKS_REQ:
			AdaptNETSETAddorDeletePrefNetworkReq(pCosEvent);
			break;
#endif

	case MSG_ID_MMI_NW_PWROFF_DETACH_REQ:
		AdaptRequestRacDetach(pCosEvent);
		break;

//[221] Added by ZXB 2009/02/10								[BEGIN]
#ifdef __ADP_MULTI_SIM__

	case MSG_ID_MMI_NW_CFUN_STATE_REQ:
		AdaptNwProcCfunReq(pCosEvent);
		break;
#endif
//[221] Added by ZXB 2009/02/10								[END]

case PRT_NW_CFUN_TEMP_CLOSE_STACK_REQ:
		CfunReqForTemp = 1;
		AdaptNwProcTempCfunCloseStackReq(pCosEvent);
		break;

#ifdef __MMI_STOP_SIGNAL_REPORT__
	case MSG_ID_MMI_NW_RX_STOP_REQ:
		AdaptStopSignalReport(pCosEvent);
		break;
#endif
	default:
		return FALSE;
	}
	TBM_EXIT(0x29b6);
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchNwRspMsg

	PURPOSE				: process all passive stack event
	
	PARAMETERS	              : 

	REMARKS			:
**************************************************************/

static BOOL ADP_DispatchNwRspMsg(COS_EVENT* pCosEvent)
{
	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29df);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_NW, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif

	switch (pCosEvent->nEventId)
	{

	case EV_CFW_NW_ABORT_LIST_OPERATORS_RSP:
		mmi_trace(g_sw_ADP_NW, "MMI_NW:  EV_CFW_NW_ABORT_LIST_OPERATORS_RSP>>>>\r\n");
		DoNETSETAbortPLMNListRsp(pCosEvent);
		break;

	case EV_CFW_NW_SET_REGISTRATION_RSP:
		mmi_trace(g_sw_ADP_NW, "MMI_NW:  EV_CFW_NW_SET_REGISTRATION_RSP SelModeorRegiFlag=%d", SelModeorRegiFlag);

		if (SelModeorRegiFlag)
			DoNETSETManuAutoSelectNetworkRsp(pCosEvent);
		else
			DoEntryNETSETSelectionModeRsp(pCosEvent);

		break;

	case EV_CFW_NW_GET_AVAIL_OPERATOR_RSP:
		mmi_trace(g_sw_ADP_NW, "MMI_NW:  EV_CFW_NW_GET_AVAIL_OPERATOR_RSP>>>>\r\n");
		DoNETSETGetPLMNListRsp(pCosEvent);

		break;

	case EV_CFW_NW_SET_BAND_RSP:
		mmi_trace(g_sw_ADP_NW, "MMI_NW:  EV_CFW_NW_SET_BAND_RSP>>>>\r\n");
		DoNETSETSetBandSelectionRsp(pCosEvent);

		break;

//This message PRT_PREFERRED_NETWORKS_REQ is processed in SIM events
#if 0

	case EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP:
		mmi_trace(g_sw_ADP_NW, ">>>> receive EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP>>>>\r\n");

		DoNETSETGetPreferredNetworkRsp(pCosEvent);

		break;

	case EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP :
		mmi_trace(g_sw_ADP_NW, ">>>> receive EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP>>>>\r\n");

		DoNETSETAddorDeletePrefNetworkRsp(pCosEvent);

		break;

#endif

	case EV_CFW_NW_DEREGISTER_RSP:
		DoNwPlmnSetRSP(pCosEvent);

		break;

//[221] Added by ZXB 2009/02/10								[BEGIN]
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	case EV_CFW_SET_COMM_RSP:
#else
	case EV_CFW_SET_COMM_RSP:
#endif	
		if(CfunReqForTemp)
		{
			CloseStackRsp(pCosEvent);
		}
		else
		{
			DoNwCfunRsp(pCosEvent);
		}

		break;

#endif
//[221] Added by ZXB 2009/02/10								[END]

	default:
		TBM_EXIT(0x29df);

		return FALSE;
	}

	TBM_EXIT(0x29df);

	return TRUE;

}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchNwMsg

	PURPOSE				: process all message

	PARAMETERS	              :

	REMARKS			:
**************************************************************/
extern void Modify_SmarchAttStatus(void);
//add for QQ, 2010-05-10
#ifdef __MMI_SUPPORT_JME__
extern void jmetoc_onpause();
extern void jmetoc_onrestart();
extern BOOL smarchbox_bg_status(VOID);
#endif
//add end

BOOL ADP_DispatchNwMsg(COS_EVENT* pCosEvent)
{
	ASSERT(pCosEvent != NULL);
#ifndef MMI_ON_WIN32
	if (pCosEvent->nEventId >= MSG_ID_MMI_NW_GET_IMEI_REQ && pCosEvent->nEventId <= MSG_ID_MMI_NW_CFUN_STATE_REQ)
		return ADP_DispatchNwReqMsg(pCosEvent);

	else if (pCosEvent->nEventId >= EV_CFW_NW_RSP_BASE && pCosEvent->nEventId <= EV_CFW_NW_RSP_END_)
		return ADP_DispatchNwRspMsg(pCosEvent);

	else if (pCosEvent->nEventId == EV_CFW_NW_NETWORKINFO_IND)
		return ADP_DispatchNwIndMsg(pCosEvent);
  else if(pCosEvent->nEventId == EV_CFW_ATT_STATUS_IND)
  {
    Modify_SmarchAttStatus();
    return TRUE;
  }
  //add for QQ, 2010-05-10
  #ifdef __MMI_SUPPORT_JME__
   else  if(pCosEvent->nEventId == EV_CFW_CC_STATUS_IND)
  {
  		mmi_trace(1, "For QQ :  EV_CFW_CC_STATUS_IND, status: 0x%x",pCosEvent->nParam1);
	if (smarchbox_bg_status())
	{
	    if(0x01==pCosEvent->nParam1)
			jmetoc_onpause();
	    else if(0x00==pCosEvent->nParam1)
	    	      jmetoc_onrestart();
	}
    return TRUE;
  }
   #endif
   //add end
   else
     return FALSE;
#else
	return FALSE;
#endif
}

