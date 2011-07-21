/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2009, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*       unit_test.c                                                          */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   Unit test source file for 221 project.                                   */
/*                                                                            */
/******************************************************************************/

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"

#include "chip_id.h"
 
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
#include "pmd_m.h"

 
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
#include "btmmigprots.h"
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

#include "mmi_features.h"

#ifdef __MMI_IMPS__
#include "mmi_imps_gprot.h"
#endif

//dyj add start 20060616
#include "device.h"
#include "stack_msgs.h"
#include <cswtype.h>
#include "mmi_msg_struct.h"
//dyj add end
#include "phonebooktypes.h"

#include "mmi_trace.h"

#include "tm.h"
#include "datetimegprot.h"
 

#include "settingsgdcl.h"

#include "nvram_data_items.h"

#include "di.h"
#include "filesystemdef.h"

#include "stack_config.h"
#include "unit_test.h"
#include "testtype2.h"
#include "testtype2control.h"
#include "mmi_trace.h"

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT

U8 nCCSimId;

U8 simulatorSMSId;


typedef BOOL (*pfn_IsNeedDouble)(COS_EVENT *pTarEv, COS_EVENT *pCompEv);

typedef struct _DOUBLE_IND_TAB_T
{
	COS_EVENT ev;
	pfn_IsNeedDouble pfnCheck;
} DOUBLE_IND_TAB_T;

static BOOL check_EV_CFW_INIT_IND(COS_EVENT *pTarEv, COS_EVENT *pCompEv)
{
	switch (HIUINT8(pTarEv->nParam3))
	{

	case CFW_INIT_STATUS_SIMCARD:

	case CFW_INIT_STATUS_SIM:

	case CFW_INIT_STATUS_NO_SIM:
		return TRUE;

	case CFW_INIT_STATUS_PBK:
		return TRUE;

	case CFW_INIT_STATUS_SMS:
		return TRUE;

	default:
		break;
	}

	return FALSE;
}


static BOOL check_EV_CFW_NW_REG_STATUS_IND(COS_EVENT *pTarEv, COS_EVENT *pCompEv)
{
#ifdef __UT_SIMU_CARD2_ABSENT__
	return FALSE;
#else
	return TRUE;
#endif
}


static BOOL check_EV_CFW_NW_SIGNAL_QUALITY_IND(COS_EVENT *pTarEv, COS_EVENT *pCompEv)
{
#ifdef __UT_SIMU_CARD2_ABSENT__
	return FALSE;
#else
	return TRUE;
#endif
}


static DOUBLE_IND_TAB_T g_double_ind_tab[] = {
	// nEventId							nParam1		nParam2		nParam3			pfnCheck
	//------------------------------------------------------------------------------------------------------------------
	{{EV_CFW_NW_REG_STATUS_IND,			0,			0,			0},			check_EV_CFW_NW_REG_STATUS_IND},
	{{EV_CFW_INIT_IND,					0,			0,			0},			check_EV_CFW_INIT_IND},
	{{EV_CFW_NW_SIGNAL_QUALITY_IND,		0,			0,			0},			check_EV_CFW_NW_SIGNAL_QUALITY_IND},
};

void adp_SimualtorFillMsgFlag(UINT8 simID, COS_EVENT *pCosEvent)
{
	pCosEvent->nParam3 |= simID;
}

void UT_SimuCfwMsg(UINT32 nEventId, UINT32 nParam1, UINT32 nParam2, UINT32 nParam3)
{
	COS_EVENT ev = {0};

	ev.nEventId = nEventId;
	ev.nParam1 = nParam1;
	ev.nParam2 = nParam2;
	ev.nParam3 = nParam3;

	COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

BOOL UT_AdpDoubleInd(COS_EVENT *pev)
{
	UINT32 n = ARRAY_SIZE(g_double_ind_tab);
	UINT32 i = 0;
	BOOL fDouble = FALSE;

	////////////////////////////////////////
	// Do not process the IND for SIM-2.

	if (adp_GetCSWSimID(adp_GetCswEvFlag(pev)) == ADP_SIM_2)
		return FALSE;

	for (i = 0; i < n; i++)
	{
		if (pev->nEventId == g_double_ind_tab[i].ev.nEventId)
		{
			if (g_double_ind_tab[i].pfnCheck != NULL)
				fDouble = g_double_ind_tab[i].pfnCheck(pev, &g_double_ind_tab[i].ev);
			else
				fDouble = TRUE;
		}
	}

	if (fDouble)
	{
		COS_EVENT ev2 = {0};

		// TODO: 考虑二级指针的问题.
		memcpy(&ev2, pev, sizeof(COS_EVENT));

		adp_SimualtorFillMsgFlag(ADP_SIM_2, &ev2);

		if (COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev2, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == FALSE)
			mmi_trace(1, "[221]Call COS_SendEvent() failed");

		return TRUE;
	}
	else
		return FALSE;
}

UINT32 UT_CFW_SimEnterAuthentication(UINT8 *pPin, UINT8 nPinSize, UINT8 *pNewPin,
									 UINT8 nNewPinSize, UINT8 nOption, UINT16 nUTI, UINT8 nSimId)
{
	UINT32 r = !ERR_SUCCESS;

	if (nSimId == ADP_SIM_1)
	{
		r = CFW_SimEnterAuthentication(pPin, nPinSize, pNewPin, nNewPinSize, nOption, nUTI);
	}
	else
	{
		COS_EVENT ev = {0};

		ev.nEventId = EV_CFW_SIM_ENTER_AUTH_RSP;
		ev.nParam1 = 0;
		ev.nParam2 = 0;
		ev.nParam3 = ADP_SIM_2; // Set flag to SIM-1.

		r = COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == TRUE ? ERR_SUCCESS : !ERR_SUCCESS;
	}

	return r;
}

////////////////////////////////////////
// UT FOR PHB

extern UINT8 SimStorageID;
extern adp_phb_context_t *adp_pPHBContext;

/****************************************************************
**
**
**
**
**
*****************************************************************/
UINT32 UT_CFW_SimGetPbkStorage(UINT8 nStorage, UINT16 nUTI, UINT8 nSimId)
{
	UINT32 r = ERR_SUCCESS;


	if (nSimId == ADP_SIM_1)
	{
		r = CFW_SimGetPbkStorage(nStorage, nUTI);
	}
	else
	{
		CFW_PBK_STORAGE *pbStorage = (CFW_PBK_STORAGE *)OslMalloc(sizeof(CFW_PBK_STORAGE));

		if (nStorage == CFW_PBK_SIM)
		{
			pbStorage->totalSlot = simulator_get_phb_total_count();
			pbStorage->usedSlot = simulator_get_phb_used_count();
		}
		else
		{
			pbStorage->totalSlot = 10;
			pbStorage->usedSlot = 0;
		}

		pbStorage->storageId = nStorage;

		COS_EVENT ev = {0};

		ev.nEventId = EV_CFW_SIM_GET_PBK_STRORAGE_RSP;
		ev.nParam1 = (UINT32)pbStorage;
		ev.nParam2 = 0;
		adp_SimualtorFillMsgFlag(nSimId, &ev); // Set flag to SIM-1.

		COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == TRUE ? ERR_SUCCESS : !ERR_SUCCESS;
	}

	return r;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
UINT32 UT_CFW_SimGetPbkEntry(UINT8 nStorage, UINT8 nIndex, UINT16 nUTI , UINT8 nSimId)
{
	mmi_trace(g_sw_ADP_PHB, "PHB Func: %s nSimId = %d,nIndex =%d", __FUNCTION__, nSimId, nIndex);

	UINT32 r = ERR_SUCCESS;

	if (nSimId == ADP_SIM_1)
	{
		r = CFW_SimGetPbkEntry(SimStorageID, nIndex, nUTI);
	}
	else
	{
		if (nStorage != CFW_PBK_SIM)
		{
			return ERR_INVALID_TYPE;
		}

		CFW_SIM_PBK_ENTRY_INFO *pbkEntryInfo = (CFW_SIM_PBK_ENTRY_INFO *)OslMalloc(sizeof(CFW_SIM_PBK_ENTRY_INFO));

		CFW_SIM_PBK_ENTRY_INFO *pSimuEntryInfo = NULL;

		COS_EVENT ev = {0};


		pSimuEntryInfo = simulator_get_phb_sim2_node(nIndex - 1);
		mmi_trace(g_sw_ADP_PHB, "PHB Func: %s ,pSimuEntryInfo->phoneIndex = %d", __FUNCTION__, pSimuEntryInfo->phoneIndex);

		memcpy(pbkEntryInfo, pSimuEntryInfo, sizeof(CFW_SIM_PBK_ENTRY_INFO));

		ev.nEventId = EV_CFW_SIM_GET_PBK_ENTRY_RSP;
		ev.nParam1 = (UINT32)pbkEntryInfo;
		ev.nParam2 = 0;
		ev.nParam3 = ADP_SIM_2; // Set flag to SIM-1.
		adp_SimualtorFillMsgFlag(nSimId, &ev);

		COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == TRUE ? ERR_SUCCESS : !ERR_SUCCESS;
	}

	mmi_trace(g_sw_ADP_PHB, "PHB Func: %s r =%d", __FUNCTION__, r);

	return r;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
UINT32 UT_CFW_SimAddPbkEntry(UINT8 nStorage, CFW_SIM_PBK_ENTRY_INFO* pEntryInfo, UINT16 nUTI, UINT8 nSimId)
{
	UINT32 r = ERR_SUCCESS;

	if (nSimId == ADP_SIM_1)
	{
		r = CFW_SimAddPbkEntry(CFW_PBK_SIM, pEntryInfo, nUTI);
	}
	else
	{
		UINT32 nIndex = 0;
		COS_EVENT ev = {0};

		nIndex = simulator_add_phb_sim2_node(pEntryInfo);

		if (nIndex == -1)
		{
			ev.nParam3 |= 0xF000;
			mmi_trace(g_sw_ADP_PHB, "PHB Func: %s ", __FUNCTION__);
		}

		mmi_trace(g_sw_ADP_PHB, "PHB Func: %s nIndex = %d", __FUNCTION__, nIndex);

		ev.nEventId = EV_CFW_SIM_ADD_PBK_RSP;
		ev.nParam1 = nIndex + 1;
		ev.nParam2 = 0;
		ev.nParam3 = ADP_SIM_2; // Set flag to SIM-1.
		adp_SimualtorFillMsgFlag(nSimId, &ev);

		COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == TRUE ? ERR_SUCCESS : !ERR_SUCCESS;
	}

	return r;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
UINT32 UT_CFW_SimDeletePbkEntry(UINT8 nStorage, UINT8 nIndex, UINT16 nUTI, UINT8 nSimId)
{
	UINT32 r = ERR_SUCCESS;
	mmi_trace(g_sw_ADP_PHB, "PHB Func: %s nIndex = %d,nSimId = %d", __FUNCTION__, nIndex, nSimId);

	if (nSimId == ADP_SIM_1)
	{
		r = CFW_SimDeletePbkEntry(CFW_PBK_SIM, nIndex, nUTI);
	}
	else
	{
		COS_EVENT ev = {0};

		simulator_del_phb_sim2_node(nIndex - 1);

		ev.nEventId = EV_CFW_SIM_DELETE_PBK_ENTRY_RSP;
		ev.nParam1 = 0;
		ev.nParam2 = 0;
		ev.nParam3 = ADP_SIM_2; // Set flag to SIM-1.
		adp_SimualtorFillMsgFlag(nSimId, &ev);

		r = COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == TRUE ? ERR_SUCCESS : !ERR_SUCCESS;
	}

	return r;
}

///UT for fdn
/****************************************************************
**
**
**
**
**
*****************************************************************/
UINT32 UT_CFW_SimListCountPbkEntries(UINT8 nStorage, UINT8 nIndexStart, UINT8 nCount, UINT16 nUTI, UINT8 nSimId)
{
	UINT32 r = ERR_SUCCESS;

	if (nSimId == ADP_SIM_1)
	{
		r = CFW_SimListCountPbkEntries(SimStorageID, 1, adp_pPHBContext->nFDNusedSlot, nUTI);
	}
	else
	{
		r = CFW_SimListCountPbkEntries(SimStorageID, 1, adp_pPHBContext->nFDNusedSlot, nUTI);
	}

	return r;
}


///UT for sat
/****************************************************************
**
**
**
**
**
*****************************************************************/
UINT32 UT_CFW_SatActivation(UINT8 nMode, UINT16 nUTI , UINT8 nSimId)
{
	UINT32 r = ERR_SUCCESS;

	if (nSimId == ADP_SIM_1)
	{
		r = CFW_SatActivation(0, 1);
	}
	else
	{
		COS_EVENT ev = {0};

		ev.nEventId = EV_CFW_SAT_ACTIVATION_RSP ;
		ev.nParam1 = 0;
		ev.nParam2 = 0;
		ev.nParam3 = ADP_SIM_2; // Set flag to SIM-1.
		ev.nParam3 |= 0xfffffff;

		r = COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == TRUE ? ERR_SUCCESS : !ERR_SUCCESS;
	}

	return r;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
UINT32 UT_CFW_SatResponse(UINT8 nCmdType, UINT8 nStatus, UINT8 nItemId, VOID* pInputStr, UINT8 InputStrLen, UINT16 nUTI, UINT8 nSimId)
{
	UINT32 r = ERR_SUCCESS;

	if (nSimId == ADP_SIM_1)
	{
		r = CFW_SatResponse(nCmdType, nStatus, nItemId, pInputStr, InputStrLen, nUTI);
	}
	else
	{
		r = CFW_SatResponse(nCmdType, nStatus, nItemId, pInputStr, InputStrLen, nUTI);
	}

	return r;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
UINT32 UT_CFW_SatGetInformation(UINT8 nCmdType, VOID** CmdData , UINT8 nSimId)
{
	UINT32 r = ERR_SUCCESS;

	r = CFW_SatGetInformation(nCmdType,  CmdData);

	return r;
}

///UT for SMS

UINT32 UT_CFW_SmsListMessages(CFW_SMS_LIST * pListInfo,  UINT16 nUTI, UINT8 nSimId)
{
	UINT32 ret = !ERR_SUCCESS;
	TRACE_EF(1, "pListInof startIndex = %d, count = %d, nSimId = %d", pListInfo->nStartIndex, pListInfo->nCount, nSimId);

	if (nSimId == ADP_SIM_1)
	{
		ret = CFW_SmsListMessages(CFW_SMS_STORAGE_ME/*storage1*/, 
									    CFW_SMS_STORED_STATUS_STORED_ALL, 
									    1, 
									    1, 
									    CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER,
									   nUTI);
	}
	else
	{
		COS_EVENT ev = {0};
		ev.nEventId = EV_CFW_SMS_LIST_MESSAGE_RSP;
		U8 *pListHead = NULL;
		U8 count = 0;
		//modify here for simulate exact response;

		if (pListInfo->nStorage == CFW_SMS_STORAGE_SM)
		{
			pListHead = (U8*) simulator_list_map_sms_sim2(pListInfo->nStartIndex - 1, pListInfo->nCount, (U8*) & count);
		}
		else
		{
			pListHead = (U8*) simulator_list_map_sms_me2(pListInfo->nStartIndex - 1, pListInfo->nCount, (U8*) & count);
		}

		ev.nParam1 = (U32)pListHead;

		ev.nParam2 |= ((U32)count) << 16;
		ev.nParam2 |= (pListInfo->nStartIndex - 1) + count;
		ev.nParam3 = ADP_SIM_2;
		ret = COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == TRUE ? ERR_SUCCESS : !ERR_SUCCESS;
	}

	return ret;
}

UINT32  UT_CFW_SmsGetMessageNode(PVOID pListResult, UINT16 nIndex, CFW_SMS_NODE **pNode, UINT8 nSimId)
{
	UINT32 ret = !ERR_SUCCESS;

	if (nSimId == ADP_SIM_1)
	{
		ret = CFW_SmsGetMessageNode(pListResult, nIndex, pNode);
	}
	else
	{
		//wangzl: 填充pNode指向的内容
		* pNode = (CFW_SMS_NODE *)simulator_get_sms_node((U8*)pListResult, (U8)nIndex);

		ret = ERR_SUCCESS;
	}

	return ret;
}

UINT32 UT_CFW_CfgGetSmsStorageInfo(CFW_SMS_STORAGE_INFO* pStorageInfo, UINT16 nStorage, UINT8 nSimId)
{
	UINT32 ret = !ERR_SUCCESS;

	if (nSimId == ADP_SIM_1)
	{
		ret = CFW_CfgGetSmsStorageInfo(pStorageInfo, nStorage);
	}
	else
	{
		if (nStorage == CFW_SMS_STORAGE_SM)
		{
			simulator_get_sms_storage_info(pStorageInfo, SIMULATOR_STORAGE_SIM2);
		}
		else if (nStorage == CFW_SMS_STORAGE_ME)
		{
			simulator_get_sms_storage_info(pStorageInfo, SIMULATOR_STORAGE_ME2);
		}

		ret = ERR_SUCCESS;
	}

	return ret;
}

void SimuSetSMSID(U8 simId)
{
	simulatorSMSId = simId;
}

U8 SimuGetSMSID()
{
	return simulatorSMSId;
}

UINT32 UT_CFW_SmsSendMessage(CFW_DIALNUMBER* pNumber, UINT8* pData, UINT16 nDataSize, UINT16 nUTI, UINT8 nSimId)
{
	UINT32 ret = !ERR_SUCCESS;

	SimuSetSMSID(nSimId);

	if (nSimId == ADP_SIM_1)
	{
		ret = CFW_SmsSendMessage(pNumber, pData, nDataSize, nUTI);
	}
	else
	{

		ret = CFW_SmsSendMessage(pNumber, pData, nDataSize, nUTI);
#if 0
		COS_EVENT ev = {0};
		ev.nEventId = EV_CFW_SMS_SEND_MESSAGE_RSP;
		ev.nParam3 = ADP_SIM_2;
		// TODO: 需要模拟发送
		ret = COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == TRUE ? ERR_SUCCESS : !ERR_SUCCESS;
#endif
	}

	return ret;
}



UINT32 UT_CFW_SmsWriteMessage(CFW_SMS_WRITE*	pSMSWrite, UINT16 nUTI, UINT8 nSimId)
{
	UINT32 ret = !ERR_SUCCESS;
	CFW_SMS_NODE *pNode = NULL;

	if (nSimId == ADP_SIM_1)
	{
		ret = CFW_SmsWriteMessage(&pSMSWrite->sNumber, 
								     pSMSWrite->pData, 
								     pSMSWrite->nDataSize, 
								     pSMSWrite->nStorage, /*adp_pSMSContext->nCurrentStorage*/
								     0, 
								     1 << pSMSWrite->nStatus, 
								     nUTI);
	}
	else
	{
		COS_EVENT ev = {0};
		ev.nEventId = EV_CFW_SMS_WRITE_MESSAGE_RSP;
		ev.nParam3 = ADP_SIM_2;
		pNode = simulator_write_sms_node(pSMSWrite);

		if (pNode != NULL)
		{
			ev.nParam1 |= ((U32)pNode->nStorage) << 16;
			ev.nParam1 |= pNode->nConcatCurrentIndex;

		}

		ret = COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == TRUE ? ERR_SUCCESS : !ERR_SUCCESS;
	}

	return ret;
}

UINT32 UT_CFW_SmsReadMessage(UINT16 nStorage, UINT16 nIndex, UINT16 nUTI, UINT8 nSimId)
{
	UINT32 ret = !ERR_SUCCESS;

	CFW_SMS_NODE* pNode = NULL;

	TRACE_EF(1, "nSimeId = %d", nSimId);

	if (nSimId == ADP_SIM_1)
	{
		ret = CFW_SmsReadMessage(nStorage, nIndex, nUTI);
	}
	else
	{
		COS_EVENT ev = {0};
		ev.nEventId = EV_CFW_SMS_READ_MESSAGE_RSP;

		if (nStorage == CFW_SMS_STORAGE_ME)
		{
			pNode = simulator_get_sms_me2_storage_node(nIndex);

		}
		else
		{
			pNode = simulator_get_sms_sim2_storage_node(nIndex);
		}

		TRACE_EF(1, "%d", pNode->nConcatCurrentIndex);

		ev.nParam1 = (UINT32)pNode;
		ev.nParam3 = 0;
		ev.nParam3 |= ADP_SIM_2;
		ret = COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == TRUE ? ERR_SUCCESS : !ERR_SUCCESS;
	}

	return ret;
}

UINT32 UT_CFW_SmsDeleteMessage(UINT16 nIndex, UINT8 nStatus, UINT16 nStorage, UINT8 nType, UINT16 nUTI, UINT8 nSimId)
{
	UINT32 ret = !ERR_SUCCESS;

	if (nSimId == ADP_SIM_1)
	{
		ret = CFW_SmsDeleteMessage(nIndex, nStatus, nStorage, nUTI);
	}
	else
	{
		COS_EVENT ev = {0};
		ev.nEventId = EV_CFW_SMS_DELETE_MESSAGE_RSP;
		ev.nParam3 = ADP_SIM_2;
		ev.nParam1 |= nIndex;
		ev.nParam2 |= nStorage;
		simulator_delete_sms_node(nIndex, nStorage, nStatus);
		ret = COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == TRUE ? ERR_SUCCESS : !ERR_SUCCESS;
	}

	return ret;

}

UINT32 UT_CFW_CfgGetSmsParam (
  CFW_SMS_PARAMETER* pInfo,
  UINT8 nIndex,
  CFW_SIM_ID nSimID
)
{
	return CFW_CfgGetSmsParam (pInfo, nIndex);
}
/////////////////////////////////////////////////////////////////////////
//CC
UINT32 UT_CFW_CcAcceptSpeechCall(UINT8 nSimId)
{
	nCCSimId = nSimId;
	return CFW_CcAcceptSpeechCall();
}

UINT32 UT_CFW_CcPlayTone(INT8 iTone, UINT8 nSimId)
{
	nCCSimId = nSimId;
	return CFW_CcPlayTone(iTone);
}

UINT32 UT_CFW_CcGetCurrentCall(
	CFW_CC_CURRENT_CALL_INFO CallInfo[7],
	UINT8* pCnt,
	UINT8 nSimId
)
{
	nCCSimId = nSimId;
	return CFW_CcGetCurrentCall(CallInfo, pCnt);
}

UINT32 UT_CFW_CcReleaseCall(UINT8 nSimId)
{
	nCCSimId = nSimId;
	return CFW_CcReleaseCall();
}

UINT32 UT_CFW_CcCallHoldMultiparty(
	UINT8 nCmd,
	UINT8 nIndex,
	UINT8 nSimId
)
{
	nCCSimId = nSimId;
	return CFW_CcCallHoldMultiparty(nCmd, nIndex);
}

UINT32 UT_CFW_CcRejectCall(UINT8 nSimId)
{
	nCCSimId = nSimId;
	return CFW_CcRejectCall();
}

extern UINT32 CFW_CcEmcDialEx(UINT8* pDialNumber, UINT8 nDialNumberSize, UINT8 *pIndex);
extern UINT32 CFW_CcInitiateSpeechCallEx(CFW_DIALNUMBER* pDialNumber, UINT8 *pIndex, UINT16 nUTI);

UINT32 UT_CFW_CcEmcDialEx(
	UINT8* pDialNumber,
	UINT8 nDialNumberSize,
	UINT8 *pIndex,
	UINT8 nSimId
)
{
	nCCSimId = nSimId;
	return CFW_CcEmcDialEx(pDialNumber, nDialNumberSize, pIndex);
}

UINT32 UT_CFW_CcInitiateSpeechCallEx(
	CFW_DIALNUMBER* pDialNumber,
	UINT8 *pIndex,
	UINT16 nUTI,
	UINT8 nSimId
)
{
	nCCSimId = nSimId;
	return CFW_CcInitiateSpeechCallEx(pDialNumber, pIndex, nUTI);
}

UINT32 UT_CFW_CcInitiateSpeechCall (
  CFW_DIALNUMBER* pDialNumber,
  UINT16 nUTI,
  UINT8 nSimId
)
{
	nCCSimId = nSimId;
	return CFW_CcInitiateSpeechCall(pDialNumber, nUTI);
}
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//SS
UINT32 UT_CFW_SsSetCallWaiting(UINT8 nMode, UINT8 nClass, UINT16 nUTI, UINT8 nSimId)
{
	return CFW_SsSetCallWaiting(nMode, nClass, nUTI);
}

UINT32 UT_CFW_SsQueryCallWaiting(UINT8 nClass, UINT16 nUTI, UINT8 nSimId)
{
	return CFW_SsQueryCallWaiting(nClass, nUTI);
}

UINT32 UT_CFW_SsSetFacilityLock(UINT16 nFac, UINT8* pBufPwd, UINT8 nPwdSize, UINT8 nClass, UINT8 nMode, UINT16 nUTI, UINT8 nSimId)
{
	return CFW_SsSetFacilityLock(nFac, pBufPwd, nPwdSize, nClass, nMode, nUTI);
}

UINT32 UT_CFW_SsQueryFacilityLock(UINT16 nFac, UINT8 nClass, UINT16 nUTI, UINT8 nSimId)
{
	return CFW_SsQueryFacilityLock(nFac, nClass, nUTI);
}

UINT32 UT_CFW_SsSetCallForwarding(CFW_SS_SET_CALLFORWARDING_INFO* pCallForwarding, UINT16 nUTI, UINT8 nSimId)
{
	return CFW_SsSetCallForwarding(pCallForwarding, nUTI);
}

UINT32 UT_CFW_SsQueryCallForwarding(UINT8 nReason,  UINT8 nClass,  UINT16 nUTI, UINT8 nSimId)
{
	return CFW_SsQueryCallForwarding(nReason, nClass, nUTI);
}

UINT32 UT_CFW_SsSendUSSD(UINT8* pUsdString, UINT8 nUsdStringSize, UINT8 nOption, UINT8 nDcs, UINT16 nUTI, UINT8 nSimId)
{
	return CFW_SsSendUSSD(pUsdString, nUsdStringSize, nOption, nDcs, nUTI);
}

UINT32 UT_CFW_SsQueryClip(UINT16 nUTI, UINT8 nSimId)
{
	return CFW_SsQueryClip(nUTI);
}

UINT32 UT_CFW_SsQueryClir(UINT16 nUTI, UINT8 nSimId)
{
	return CFW_SsQueryClir(nUTI);
}

UINT32 UT_CFW_SsQueryColp(UINT16 nUTI, UINT8 nSimId)
{
	return CFW_SsQueryColp(nUTI);
}

UINT32 UT_CFW_SsQueryColr(UINT16 nUTI, UINT8 nSimId)
{

	return CFW_SsQueryColr(nUTI);
}

UINT32 UT_CFW_SsChangePassword(UINT16 nFac, UINT8* pBufOldPwd, UINT8 nOldPwdSize, UINT8* pBufNewPwd, UINT8 nNewPwdSize, UINT16 nUTI, UINT8 nSimId)
{
	return CFW_SsChangePassword(nFac, pBufOldPwd, nOldPwdSize, pBufNewPwd, nNewPwdSize, nUTI);
}

////////////////////////////////////////////////////////////////////////////
//UT NW
UINT32 UT_CFW_GetComm(
	CFW_COMM_MODE* pMode,
	CFW_SIM_ID 	nSimID
)
{
	nSimID = nSimID;
	*pMode = CFW_ENABLE_COMM;
	return ERR_SUCCESS;
}

UINT32 UT_CFW_SetComm(
	CFW_COMM_MODE 	nMode,
	UINT16  		nUTI,
	CFW_SIM_ID		nSimID
)
{
	COS_EVENT ev = {0};

	TRC_ENTER_FUNC(g_sw_ADP_NW);

	ev.nEventId = EV_CFW_SET_COMM_RSP;
	ev.nParam1 = nMode;
	ev.nParam2 = 0;
	ev.nParam3 = nSimID;

	mmi_trace(g_sw_ADP_NW, "[221]CFW sendback EV_CFW_SET_COMM_RSP to ADP.");
	COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

	////////////////////////////////////////
	// Simulate an EV_CFW_NW_REG_STATUS_IND and
	// send to ADP when enable a card.

	if (nMode == CFW_ENABLE_COMM)
	{
		COS_EVENT ev1 = {0};

		ev1.nEventId = EV_CFW_NW_REG_STATUS_IND;
		ev1.nParam1 = 1;
		ev1.nParam2 = 0;
		ev1.nParam3 = (2 << 8) | nSimID; // type=2,sim_id=...

		mmi_trace(g_sw_ADP_NW, "[221]Simulate EV_CFW_NW_REG_STATUS_IND when (re-)enable a card..");
		COS_SendEvent(GetMmiTaskHandle(adp_SimId2IlmMod(nSimID)), &ev1, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	}

	return ERR_SUCCESS;
}

UINT32 UT_CFW_NwGetCurrentOperator (
  UINT8 OperatorId[6],
  UINT8* pMode,
  CFW_SIM_ID nSimID
)
{
	return CFW_NwGetCurrentOperator(OperatorId, pMode);
}

UINT32 UT_CFW_CfgGetIndicatorEvent (
  UINT16* ind, CFW_SIM_ID nSimID
)
{
	return CFW_CfgGetIndicatorEvent(ind);
}

UINT32 UT_CFW_CfgSetIndicatorEvent (
  UINT16 ind, CFW_SIM_ID nSimID
)
{
	return CFW_CfgSetIndicatorEvent(ind);
}

UINT32 UT_CFW_SimGetProviderId (
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_SimGetProviderId(nUTI);
}

UINT32 UT_CFW_CfgSetSmsFormat (
  UINT8 nFormat,
  CFW_SIM_ID nSimID
)
{
	return CFW_CfgSetSmsFormat(nFormat);
}

UINT32 UT_CFW_CfgSetSmsShowTextModeParam (
  UINT8 nShow,
  CFW_SIM_ID nSimID
)
{
	return CFW_CfgSetSmsShowTextModeParam(nShow);
}

UINT32 UT_CFW_CfgSetClip (
  UINT8 nEnable,
  CFW_SIM_ID nSimID
)
{
	return CFW_CfgSetClip(nEnable);
}

UINT32 UT_CFW_CfgSetNwStatus (
  UINT8 n,
  CFW_SIM_ID nSimID
)
{
	return CFW_CfgSetNwStatus (n);
}

UINT32 UT_CFW_NwAbortListOperators(
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_NwAbortListOperators(nUTI);
}

UINT32 UT_CFW_NwSetRegistration (
  UINT8 nOperatorId[6],
  UINT8 nMode,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_NwSetRegistration(nOperatorId, nMode, nUTI);
}

UINT32 UT_CFW_CfgNwGetFrequencyBand (
  UINT8* pBand,
  CFW_SIM_ID nSimID
)
{
	return CFW_CfgNwGetFrequencyBand(pBand);
}

UINT32 UT_CFW_NwGetAvailableOperators (
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	 return CFW_NwGetAvailableOperators(nUTI);
}

UINT32 UT_CFW_NwSetFrequencyBand (
  UINT8 nBand,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_NwSetFrequencyBand(nBand,nUTI);
}

UINT32 UT_CFW_NwDeRegister (
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_NwDeRegister(nUTI);
}

UINT32 UT_CFW_SimGetPrefOperatorList(
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_SimGetPrefOperatorList(nUTI);
}

UINT32 UT_CFW_SimSetPrefOperatorList( 
  UINT8* pOperatorList,
  UINT8 nSize, 
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_SimSetPrefOperatorList(pOperatorList, nSize, nUTI);
}

UINT32 UT_CFW_SimGetFacilityLock (
  UINT16 nFac,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_SimGetFacilityLock(nFac, nUTI);
}

UINT32 UT_CFW_SimSetFacilityLock (
  UINT16 nFac,
  UINT8* pBufPwd,
  UINT8 nPwdSize,
  UINT8 nMode,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_SimSetFacilityLock(nFac, pBufPwd, nPwdSize,nMode, nUTI);
}

UINT32 UT_CFW_SimGetAuthenticationStatus (
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
 	return CFW_SimGetAuthenticationStatus(nUTI);
}

UINT32 UT_CFW_SimChangePassword (
  UINT16 nFac,
  UINT8* pBufOldPwd,
  UINT8 nOldPwdSize,
  UINT8* pBufNewPwd,
  UINT8 nNewPwdSize,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_SimChangePassword(nFac, pBufOldPwd, nOldPwdSize, pBufNewPwd, nNewPwdSize, nUTI);
}

UINT32 UT_CFW_CfgGetSmsStorage (
  UINT8* pStorage,
  CFW_SIM_ID nSimID
)
{
	return CFW_CfgGetSmsStorage(pStorage, pStorage);
}

UINT32 UT_CFW_CfgSetNewSmsOption (
  UINT8 nOption,
  UINT8 nNewSmsStorage,
  CFW_SIM_ID nSimID
)
{
	return CFW_CfgSetNewSmsOption(nOption, nNewSmsStorage);
}

UINT32 UT_CFW_CfgSetSmsOverflowInd (
  UINT8 nMode,
   CFW_SIM_ID nSimID
)
{
	return CFW_CfgSetSmsOverflowInd(nMode);
}

UINT32 UT_CFW_CfgSetSmsParam (
  CFW_SMS_PARAMETER * pInfo, 
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_CfgSetSmsParam(pInfo, nUTI);
}

UINT32 UT_CFW_SimGetPrefOperatorListMaxNum(
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{
	return CFW_SimGetPrefOperatorListMaxNum(nUTI);
}
////////////////////////////////////////////////////////////////////////////

#endif

