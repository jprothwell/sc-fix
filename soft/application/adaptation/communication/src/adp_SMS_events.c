
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

/*add by wuzc for debug 2006725*/
//#include "mmi_trace.h"
/*add by wuzc for debug 2006725*/


 
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
#include "mmiapi.h"
#include "uim.h"
#include "adp_events.h"
#include "adp_eventsredef.h"
#include "cfw.h"   
#include "phonebooktypes.h"  
#include "mmitask.h"
#include "nvram_data_items.h"

 
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

#include "mmi_trace.h"
#include "nvram_user_defs.h"
typedef struct _IndexMap
{
	BOOL bUsed;
	UINT8 nStorage;
	UINT16 nPhyIndex;
	UINT8 nBoxType;
} IndexMap;
#define BOXTYPE_OUT		1
#define BOXTYPE_IN		2

typedef struct
{
	U16 ref; /* concat. message reference*/
	U8 total_seg; /* total segments*/
	U8 seg; /* indicate which segment*/
}mmi_frm_sms_concat_struct;  //wangwb @2007.06.16

typedef enum
{
	ADP_CFW_SMS_TXT_DELIVERED_NO_HRD_INFO,
	ADP_CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO,
	ADP_CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO,
	ADP_CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO	
} MESSAGE_NODE_TYPE;

typedef enum
{
	MESSAGE_SPACE_AVAILABLE,
	MESSAGE_SPACE_FULL_NO_WAITING,
	MESSAGE_SPACE_FULL_WAITING
} MESSAGE_INFO_TYPE;

#define MAX_LIST_COUNT_SIM 100
#define LIST_COUNT_ME		1
#define LIST_COUNT_SIM		1

#define ADP_MMI_FRM_SMS_INVALID_INDEX 0xffff
#define  SMS_TEST_SWITCH		0//switch of test
//#define MAX_INDEX_MAP_LENGTH 470 //300 this value should be equal with CM_SMS_MAX_MSG_NUM
#define MMI_FRM_SMS_INVALID_INDEX	0xffff      //wangwb add @2007.06.16
//static UINT8 nWriteMessageStatus = 0;
//static BOOL bSIMFull = FALSE;
//static BOOL bMEFull = FALSE;
//static UINT8 nCurrentStorage = CFW_SMS_STORAGE_SM;
//static UINT8 adpDelBitmap[480] = {0};
//static UINT16 nDeleteNum = 0;
//static UINT16 nIndexCount = 0;
//static UINT16 nSMSCountMe = 0;
//static UINT16 nSMSCountSIM = 0;
//UINT16 SMSInME = 0;
//UINT16 SMSListInMeOver = 0;
typedef enum
{
	LIST_STATE_ME_START = 0,
	LIST_STATE_ME_OVER,
	LIST_STATE_SM_START,
	LIST_STATE_SM_OVER
}SMS_LIST_STATE;

#ifndef __ADP_MULTI_SIM__
adp_sms_context_t adp_gSMSContext[1];
static IndexMap indexMap[ PHONE_SMS_ENTRY_COUNT  + SIM_SMS_ENTRY_COUNT ];
#else /*defined __ADP_MULTI_SIM__*/
#include "unit_test.h"
adp_sms_context_t adp_gSMSContext[ADP_SIM_NUMBER];
static IndexMap indexMap[(PHONE_SMS_ENTRY_COUNT/ADP_SIM_NUMBER + SIM_SMS_ENTRY_COUNT)*ADP_SIM_NUMBER];
#endif
adp_sms_context_t *adp_pSMSContext = &adp_gSMSContext[0];

void setActualSMSNumber(void);
static void DealSapceFullWaiting(UINT32 nParam2);

U8 gCurrentSMSSimID = 0;
/**************************************************************

	FUNCTION NAME		: InitIndexMap

  	PURPOSE				: initialize the indexMap global array  

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

	REMARKS			: The indexMap is designed for tow storages to store all the indexs of the storages. 

**************************************************************/
static void InitIndexMap()
{
	static BOOL initFlag = FALSE;
	if(initFlag == FALSE)
	{
		memset(indexMap, 0, sizeof(indexMap));
		initFlag = TRUE;
	}
}
/**************************************************************

	FUNCTION NAME		: InitSMSContext

  	PURPOSE				: initialize the SMS context  

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

	REMARKS			:  

**************************************************************/
static void InitSMSContext(void)
{
	static BOOL initFlag = FALSE;
	if(FALSE == initFlag)
	{
		U8 i;
		memset(adp_gSMSContext, 0, sizeof(adp_gSMSContext));
		
		for(i = 0; i < ADP_SIM_NUMBER; i++)
		{
			adp_gSMSContext[i].nCurrentStorage = CFW_SMS_STORAGE_SM;
			adp_gSMSContext[i].nStartupBeginMsgId = PRT_MSG_ID_MMI_SMS_STARTUP_BEGIN_IND;
			adp_gSMSContext[i].nStartupReadMsgId = PRT_MSG_ID_MMI_SMS_STARTUP_READ_MSG_IND;
			adp_gSMSContext[i].nStartupFinishMsgId = PRT_MSG_ID_MMI_SMS_STARTUP_FINISH_IND;
			adp_gSMSContext[i].nStartupReadyMsgId = PRT_MSG_ID_MMI_SMS_READY_IND;
			adp_gSMSContext[i].nStartIndexInMap = (PHONE_SMS_ENTRY_COUNT/ADP_SIM_NUMBER + SIM_SMS_ENTRY_COUNT)*i;
			#ifdef __ADP_MULTI_SIM__
			adp_gSMSContext[i].nSimId =i;
			#endif
			adp_gSMSContext[i].SMSListState = LIST_STATE_ME_START;
			
		}
		
		initFlag = TRUE;
	}
}




/**************************************************************

	FUNCTION NAME		: GetFreeIndex

  	PURPOSE				: To get the unused index from the index map

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: freeIndex

	RETURNS				: BOOL

	REMARKS			: When we insert a new index into indexMap, we must get the unused index first, using the function.

**************************************************************/
static BOOL GetFreeIndex(UINT16* freeIndex)
{
	UINT16 nIndex = adp_pSMSContext->nStartIndexInMap;	
	for (nIndex = adp_pSMSContext->nStartIndexInMap; (nIndex < adp_pSMSContext->nStartIndexInMap + (PHONE_SMS_ENTRY_COUNT/ADP_SIM_NUMBER + SIM_SMS_ENTRY_COUNT)) && indexMap[nIndex].bUsed; ++nIndex);

	if (nIndex < adp_pSMSContext->nStartIndexInMap + (PHONE_SMS_ENTRY_COUNT/ADP_SIM_NUMBER + SIM_SMS_ENTRY_COUNT))
	{
		*freeIndex = nIndex;
		return TRUE;	
	}
	else
	{
		return FALSE;
	}
}

/**************************************************************

	FUNCTION NAME		: GetLogicIndex

  	PURPOSE				: Acorrding the storage type and the physic index, we will get the index in the indexMap.

	INPUT PARAMETERS	: nPhyIndex, nStorageType

	OUTPUT PARAMETERS	: nLogicIndex

	RETURNS				: BOOL

	REMARKS			: 

**************************************************************/
static BOOL GetLogicIndex(UINT16 nPhyIndex, UINT8 nStorageType, UINT16 *nLogicIndex)
{
	UINT16 nIndex = adp_pSMSContext->nStartIndexInMap;
	for (nIndex = adp_pSMSContext->nStartIndexInMap; nIndex < adp_pSMSContext->nStartIndexInMap + adp_pSMSContext->nIndexCount; ++nIndex)
	{
		if (indexMap[nIndex].nPhyIndex == nPhyIndex && indexMap[nIndex].nStorage == nStorageType && 
			(indexMap[nIndex].bUsed == TRUE))
		{
			*nLogicIndex = nIndex;
			return TRUE;
		}
	}
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: GetLogicIndex return FALSE");
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: SendEventToMMI

  	PURPOSE				: Send the event to mmi queue to be processed.

	INPUT PARAMETERS	: pMessageStruct, messageID

	OUTPUT PARAMETERS	: void

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void SendEventToMMI(local_para_struct *pMessageStruct, msg_type_t messageID)
{
	ilm_struct  ilm_ptr;

	ilm_ptr.local_para_ptr = pMessageStruct;
	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
#ifdef 	__ADP_MULTI_SIM__
	ilm_ptr.src_mod_id = MOD_L4C + adp_pSMSContext->nSimId;
#endif

	ilm_ptr.msg_id = messageID;
	ilm_ptr.sap_id = MNSMS_SAP;
	ilm_ptr.peer_buff_ptr = NULL;

	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************

	FUNCTION NAME		: DummyInitSMSForNoSim

  	PURPOSE				: Initialize the sms.
	INPUT PARAMETERS	: nParam1, nParam2, nType

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: This is a dummy function for without one sim of dual sim

**************************************************************/
#ifdef __ADP_MULTI_SIM__
void DummyInitSMSForNoSim(UINT32 nParam1, UINT32 nParam2, UINT8 nType)
{
	InitIndexMap();
	InitSMSContext();
	SendEventToMMI(NULL, adp_pSMSContext->nStartupBeginMsgId);
	SendEventToMMI(NULL, adp_pSMSContext->nStartupFinishMsgId);
	SendEventToMMI(NULL, adp_pSMSContext->nStartupReadyMsgId);
	mmi_trace(g_sw_ADP_SMS,"ADP_SMS: dummy init for sim %d",adp_pSMSContext->nSimId);
}
#endif

/**************************************************************

	FUNCTION NAME		: AbortMessageREQ

  	PURPOSE				: Send the event to mmi for Abort Message

	INPUT PARAMETERS		: void 

	OUTPUT PARAMETERS	: void

	RETURNS				: void

	REMARKS			: 

**************************************************************/
BOOL bAbortSendMessage = FALSE;
static void AbortMessageREQ(void)
{
	#if 0
	mmi_sms_send_abort_finish_ind_struct *msgRsp = NULL;
	msgRsp = (mmi_sms_send_abort_finish_ind_struct *)OslMalloc(sizeof(mmi_sms_send_abort_finish_ind_struct));
	ASSERT(NULL != msgRsp);
	memset(msgRsp, 0, sizeof(mmi_sms_send_abort_finish_ind_struct));

	msgRsp->result = TRUE;

	SendEventToMMI((local_para_struct *) msgRsp, PRT_MSG_ID_MMI_SMS_SEND_ABORT_FINISH_IND);
	#else
	#ifndef MMI_ON_WIN32
	UINT32 ret = ERR_SUCCESS;
	#ifdef __ADP_MULTI_SIM__
	//ret = CFW_SmsAbortSendMessage(adp_pSMSContext->nSimId);
	#else
	//ret = CFW_SmsAbortSendMessage();
	#endif
	bAbortSendMessage = TRUE;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_SmsAbortSendMessage ret is 0x%x",ret);
	#endif
	#endif
}


/**************************************************************

	FUNCTION NAME		: CswStorage2CSDStorage

  	PURPOSE				: Convert the csw storage type to CSD storage type

	INPUT PARAMETERS	: nCswStorage

	OUTPUT PARAMETERS	: nCSDStorage

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void CswStorage2CSDStorage(UINT8 *nCSDStorage, UINT8 nCswStorage)
{
	switch (nCswStorage)
	{
	case CFW_SMS_STORAGE_ME:
		*nCSDStorage = SMSAL_ME;
		break;
		
	case CFW_SMS_STORAGE_SM:
		*nCSDStorage = SMSAL_SM;
		break;

#ifndef __ADP_MULTI_SIM__
	case CFW_SMS_STORAGE_BM:
		*nCSDStorage = SMSAL_BM;
		break;

	case CFW_SMS_STORAGE_SM_SR:
		*nCSDStorage = SMSAL_SR;
		break;
		
	case CFW_SMS_STORAGE_ME_SR:
		*nCSDStorage = SMSAL_SR;
		break;
		
	case CFW_SMS_STORAGE_MT:
		*nCSDStorage = SMSAL_STORAGE_UNSPECIFIED;
		break;
		
	case CFW_SMS_STORAGE_SM_ME:
		*nCSDStorage = SMSAL_MT;
		break;
#endif
	default:
		*nCSDStorage = SMSAL_STORAGE_UNSPECIFIED;
		break;
	}
}

/**************************************************************

	FUNCTION NAME		: CswError2CSDError

  	PURPOSE				: Convert the csw error to CSD error

	INPUT PARAMETERS	: cswError

	OUTPUT PARAMETERS	: CSDError

	RETURNS				: void

	REMARKS			: 

**************************************************************/
void CswError2CSDError(UINT16 *CSDError, UINT32 cswError)
{
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: Func: %s, cswError=0x%x", __FUNCTION__, cswError);
	switch (cswError)
	{
	case ERR_CFW_INVALID_PARAMETER:
		*CSDError = INVALID_REQ_PARAMETER;
		break;

	case ERR_CMS_INVALID_MEMORY_INDEX:
		*CSDError = INVALID_MEM_INDEX;
		break;
	
	case ERR_CMS_MEMORY_FULL:
		*CSDError = MEM_FULL;
		break;

	case ERR_CMS_UNSPECIFIED_TP_PID_ERROR:
		*CSDError = UNSPECIFIED_PID_ERROR;
		break;

	case ERR_CMS_UNSPECIFIED_TP_DCS_ERROR:
		*CSDError = UNSPECIFIED_TP_DCS;
		break;

	case ERR_CMS_TP_VPF_NOT_SUPPORTED:
		*CSDError = TP_VPF_NOT_SUPPORT;
		break;

	case ERR_CMS_INVALID_PDU_MODE_PARAMETER:
		*CSDError = INVALID_PDU_MODE_PARA;
		break;

	case ERR_CMS_INVALID_TEXT_MODE_PARAMETER:
		*CSDError = INVALID_TEXT_MODE_PARA;
		break;

	case ERR_CMS_SMSC_ADDRESS_UNKNOWN:
		*CSDError = SCA_UNKNOWN;
		break;

	case ERR_CMS_INVALID_CHARACTER_IN_PDU:
	case ERR_CMS_INVALID_CHARACTER_IN_ADDRESS_STRING:
	case ERR_CMS_INVALID_STATUS:
	case ERR_CFW_NOT_EXIST_FREE_UTI:
	case ERR_CFW_UTI_IS_BUSY:
	case ERR_CMS_UNKNOWN_ERROR:
		*CSDError = SMSAL_UNSPECIFIED_ERROR_CAUSE;
		break;
		
	default:
		*CSDError = SMSAL_UNSPECIFIED_ERROR_CAUSE;
		break;
	}
}

/**************************************************************

	FUNCTION NAME		: CswDcs2CSDDcs

  	PURPOSE				: Convert the csw dcs to CSD dcs

	INPUT PARAMETERS	: cswDcs

	OUTPUT PARAMETERS	: CSDDcs

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void CswDcs2CSDDcs(UINT8 *CSDDcs, UINT8 cswDcs)
{
	switch (cswDcs)
	{
	case 0:
		*CSDDcs = SMSAL_DEFAULT_DCS;
		break;
		
	case 1:
		*CSDDcs = SMSAL_8BIT_DCS;
		break;
		
	case 2:
		*CSDDcs = SMSAL_UCS2_DCS;
		break;
		
	default:
		*CSDDcs = SMSAL_RESERVED_DCS;
		break;
	}
}

/**************************************************************

	FUNCTION NAME		: Cswtime2CSDtime

  	PURPOSE				: Convert the csw time to CSD time

	INPUT PARAMETERS	: pCswtime

	OUTPUT PARAMETERS	: pCSDtime

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void Cswtime2CSDtime(UINT8 *pCSDtime, TM_SMS_TIME_STAMP *pCswtime)
{
	UINT16 temp = 0;
	
	ASSERT((pCSDtime != NULL) && (pCswtime != NULL));
	if(pCswtime->uYear >= 2000)
	{
		temp = pCswtime->uYear-2000;
	}
	else
	{
		temp = pCswtime->uYear -1900;
	}
	pCSDtime[0] = ((temp%10)<<4) + (temp/10);
	temp = pCswtime->uMonth;
	pCSDtime[1] = ((temp%10)<<4) + (temp/10);
	temp = pCswtime->uDay;
	pCSDtime[2] = ((temp%10)<<4) + (temp/10);
	temp = pCswtime->uHour;
	pCSDtime[3] = ((temp%10)<<4) + (temp/10);
	temp = pCswtime->uMinute;
	pCSDtime[4] = ((temp%10)<<4) + (temp/10);
	temp = pCswtime->uSecond;
	pCSDtime[5] = ((temp%10)<<4) + (temp/10);
	pCSDtime[6] = 0x00;
	return;
}

/**************************************************************

	FUNCTION NAME		: SetMemFullStatus

  	PURPOSE				: Because there are two storages, we must get the both memory full status to select the memeory.
  	The function is to set the full status of the two storages.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void SetMemFullStatus()
{
	CFW_SMS_STORAGE_INFO storageInfo;
	memset(&storageInfo, 0, sizeof(CFW_SMS_STORAGE_INFO));
	
	#ifdef __ADP_MULTI_SIM__
	#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	if (ERR_SUCCESS == UT_CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM, adp_pSMSContext->nSimId))
	#else /*ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
	if (ERR_SUCCESS == CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM, gCurrentSMSSimID))
	#endif/*ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
	#else /*no define __ADP_MULTI_SIM__*/
	if (ERR_SUCCESS == CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM))
	#endif/*__ADP_MULTI_SIM__*/
	{
		storageInfo.usedSlot == storageInfo.totalSlot ? (adp_pSMSContext->bSIMFull = TRUE) : (adp_pSMSContext->bSIMFull = FALSE);
		adp_pSMSContext->nSMSInSM = storageInfo.usedSlot;
		

		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: SetMemFullStatus, the sm used = %d, total = %d, adp_pSMSContext->bSIMFull = %d", 
					 				storageInfo.usedSlot, storageInfo.totalSlot, adp_pSMSContext->bSIMFull);



		memset(&storageInfo, 0, sizeof(CFW_SMS_STORAGE_INFO));
		
		#ifdef __ADP_MULTI_SIM__
		#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		if (ERR_SUCCESS == UT_CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME, adp_pSMSContext->nSimId))
		#else /*ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
		if (ERR_SUCCESS == CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME, gCurrentSMSSimID))
		#endif/*ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
		#else /*no define __ADP_MULTI_SIM__*/
		if (ERR_SUCCESS == CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME))
		#endif/*__ADP_MULTI_SIM__*/
		{
			storageInfo.usedSlot == storageInfo.totalSlot ? (adp_pSMSContext->bMEFull = TRUE) : (adp_pSMSContext->bMEFull = FALSE);
			adp_pSMSContext->nSMSInME = storageInfo.usedSlot;
			#ifdef __ADP_MULTI_SIM__
			adp_pSMSContext->nListIndex = storageInfo.totalSlot * gCurrentSMSSimID;
			#endif

			if (adp_pSMSContext->nSMSInME == 0)
			{
				adp_pSMSContext->SMSListState = LIST_STATE_SM_START;
				
				if (adp_pSMSContext->nSMSInSM ==0)
				{
					adp_pSMSContext->SMSListState= LIST_STATE_SM_OVER;
				}
				
			}
	
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: SMSListState = %d, Full status, the me used = %d, total = %d, adp_pSMSContext->bMEFull = %d", 
									 adp_pSMSContext->SMSListState, storageInfo.usedSlot, storageInfo.totalSlot, adp_pSMSContext->bMEFull);
		}
	}
}

/**************************************************************

	FUNCTION NAME		: SetCurrentStorage

  	PURPOSE				: Select the current storage for sms operating.
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: For two storage, we must select one not full storage.

**************************************************************/
static void SetCurrentStorage()
{
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry SetCurrentStorage");

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	if (ERR_SUCCESS == UT_CFW_CfgGetSmsStorage(&adp_pSMSContext->nCurrentStorage, adp_pSMSContext->nSimId))
#else
	if (ERR_SUCCESS == CFW_CfgGetSmsStorage(&adp_pSMSContext->nCurrentStorage,  gCurrentSMSSimID))
#endif
#else
	if (ERR_SUCCESS == CFW_CfgGetSmsStorage(&adp_pSMSContext->nCurrentStorage))
#endif
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The current storage = 0x%0x", adp_pSMSContext->nCurrentStorage);
	}

	if (CFW_SMS_STORAGE_SM == adp_pSMSContext->nCurrentStorage)
	{
		if (adp_pSMSContext->bSIMFull)
		{
			adp_pSMSContext->nCurrentStorage = CFW_SMS_STORAGE_ME;
		}
	}
	else
	{
		if (adp_pSMSContext->bMEFull)
		{
			adp_pSMSContext->nCurrentStorage = CFW_SMS_STORAGE_SM;
		}
	}

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_CfgSetNewSmsOption( CFW_SMS_MULTIPART_OPTION_ALL|CFW_SMS_ROUT_DETAIL_INFO, 
              UT_CFW_CFG_RECEIVE_STORAGE(adp_pSMSContext->nCurrentStorage, 0, 0), adp_pSMSContext->nSimId);
#else
	CFW_CfgSetNewSmsOption( CFW_SMS_MULTIPART_OPTION_ALL|CFW_SMS_ROUT_DETAIL_INFO, 
              				CFW_CFG_RECEIVE_STORAGE(adp_pSMSContext->nCurrentStorage, 0, 0), gCurrentSMSSimID);
#endif
#else
	CFW_CfgSetNewSmsOption( CFW_SMS_MULTIPART_OPTION_ALL|CFW_SMS_ROUT_DETAIL_INFO, 
              CFW_CFG_RECEIVE_STORAGE(adp_pSMSContext->nCurrentStorage, 0, 0));
#endif
	
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS:  The set current storage = 0x%0x", adp_pSMSContext->nCurrentStorage);
}
void dummySetSMSNumber(UINT32 number)
{
	CFW_SMS_STORAGE_INFO storageInfo;
	UINT32 ret = ERR_SUCCESS;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: dummySetSMSNumber, number is 0x%x", number);
	
	memset(&storageInfo, 0 ,sizeof(CFW_SMS_STORAGE_INFO));
	#ifndef __ADP_MULTI_SIM__
	ret = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM);
	if(ERR_SUCCESS == ret)
	{
		storageInfo.usedSlot = LOUINT16(number);
		storageInfo.totalSlot = LOUINT16(number);
		CFW_CfgSetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM);
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgGetSmsStorageInfo for SM error = 0x%x", ret);
	}
	ret = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME);
	if(ERR_SUCCESS == ret)
	{
		storageInfo.usedSlot = HIUINT16(number);
		storageInfo.totalSlot = HIUINT16(number);
		CFW_CfgSetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME);
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgGetSmsStorageInfo for ME error = 0x%x", ret);
	}
	#else
	ret = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM, gCurrentSMSSimID);
	if(ERR_SUCCESS == ret)
	{
		storageInfo.usedSlot = LOUINT16(number);
		storageInfo.totalSlot = LOUINT16(number);
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: usedSlot on SM(%d) is %d, totalSlot is %d", gCurrentSMSSimID, storageInfo.usedSlot,storageInfo.totalSlot);
		CFW_CfgSetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM, gCurrentSMSSimID);
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgGetSmsStorageInfo for SM error = 0x%x, sim = %d", ret, adp_pSMSContext->nSimId);
	}
	ret = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME, gCurrentSMSSimID);
	if(ERR_SUCCESS == ret)
	{
		storageInfo.usedSlot = HIUINT16(number) + HIUINT16(number)*gCurrentSMSSimID;
		storageInfo.totalSlot = HIUINT16(number);
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: usedSlot on ME(%d) is %d, totalSlot is %d", gCurrentSMSSimID, storageInfo.usedSlot, storageInfo.totalSlot);
		CFW_CfgSetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME, gCurrentSMSSimID);
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgGetSmsStorageInfo for ME error = 0x%x sim = %d", ret, adp_pSMSContext->nSimId);
	}
	#endif
}
void setActualSMSNumber(void)
{
		CFW_SMS_STORAGE_INFO storageInfo;
		UINT32 ret = ERR_SUCCESS;
		
		memset(&storageInfo, 0 ,sizeof(CFW_SMS_STORAGE_INFO));
		#ifndef __ADP_MULTI_SIM__
		ret = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM);
		if(ERR_SUCCESS == ret)
		{
			storageInfo.usedSlot = adp_pSMSContext->nUsedMessageSM;
			
			storageInfo.usedSlot == storageInfo.totalSlot ? (adp_pSMSContext->bSIMFull = TRUE) : (adp_pSMSContext->bSIMFull = FALSE);
			
			CFW_CfgSetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM);
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: used slot on sm = %d", adp_pSMSContext->nUsedMessageSM);
		}
		else
		{
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgGetSmsStorageInfo for SM error = 0x%x", ret);
		}
		ret = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME);
		if(ERR_SUCCESS == ret)
		{
			storageInfo.usedSlot = adp_pSMSContext->nUsedMessageME;

			storageInfo.usedSlot == storageInfo.totalSlot ? (adp_pSMSContext->bMEFull = TRUE) : (adp_pSMSContext->bMEFull = FALSE);
			
			CFW_CfgSetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME);
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: used slot on me = %d", adp_pSMSContext->nUsedMessageME);
		}
		else
		{
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgGetSmsStorageInfo for ME error = 0x%x", ret);
		}
		#else
		ret = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM, gCurrentSMSSimID);
		if(ERR_SUCCESS == ret)
		{
			storageInfo.usedSlot = adp_pSMSContext->nUsedMessageSM;

			storageInfo.usedSlot == storageInfo.totalSlot ? (adp_pSMSContext->bSIMFull = TRUE) : (adp_pSMSContext->bSIMFull = FALSE);
			
			CFW_CfgSetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM, gCurrentSMSSimID);
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: used slot on sm for sim%d = %d, total slot = %d", gCurrentSMSSimID, adp_pSMSContext->nUsedMessageSM,storageInfo.totalSlot);
		}
		else
		{
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgGetSmsStorageInfo for SM error = 0x%x, sim = %d", ret, gCurrentSMSSimID);
		}
		ret = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME, gCurrentSMSSimID);
		if(ERR_SUCCESS == ret)
		{
			storageInfo.usedSlot = adp_pSMSContext->nUsedMessageME;

			storageInfo.usedSlot == storageInfo.totalSlot ? (adp_pSMSContext->bMEFull = TRUE) : (adp_pSMSContext->bMEFull = FALSE);
			
			CFW_CfgSetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME, gCurrentSMSSimID);
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: used slot on me for sim%d = %d, total slot = %d", gCurrentSMSSimID, adp_pSMSContext->nUsedMessageME,storageInfo.totalSlot);
		}
		else
		{
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgGetSmsStorageInfo for ME error = 0x%x sim = %d", ret, gCurrentSMSSimID);
		}
		#endif

		SetCurrentStorage();

		if (adp_pSMSContext->bSIMFull && adp_pSMSContext->bMEFull)
		{
			mmi_sms_mem_full_ind_struct *pMemFull = OslMalloc(sizeof(mmi_sms_mem_full_ind_struct));
			ASSERT(NULL != pMemFull);
			memset(pMemFull, 0, sizeof(mmi_sms_mem_full_ind_struct));
			DealSapceFullWaiting(0);
			pMemFull->type = SMSAL_MEM_FULL_NORMAL;
			SendEventToMMI((local_para_struct*)pMemFull, PRT_MSG_ID_MMI_SMS_MEM_FULL_IND);
		}
}
/**************************************************************

	FUNCTION NAME		: InitSMS

  	PURPOSE				: Initialize the sms.
	INPUT PARAMETERS	: nParam1, nParam2, nType

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: Before the SMS working normallly, the function will list inbox and outbox messages.

**************************************************************/
void InitSMS(UINT32 nParam1, UINT32 nParam2, UINT8 nType)
{
	UINT32 nResult = 0;
	
	InitIndexMap();
	InitSMSContext();
	
	#ifndef __ADP_MULTI_SIM__
	mmi_trace(g_sw_ADP_SMS,"ADP_SMS: entry InitSMS");
	#else
	mmi_trace(g_sw_ADP_SMS,"ADP_SMS: entry InitSMS for SIM%d", adp_pSMSContext->nSimId);
	#endif


	dummySetSMSNumber(nParam2);
	
	SetMemFullStatus();
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_CfgSetSmsOverflowInd(1, adp_pSMSContext->nSimId);
#else
	CFW_CfgSetSmsOverflowInd(1, gCurrentSMSSimID);
#endif
#else
	CFW_CfgSetSmsOverflowInd(1);
#endif
			
	if (0 != nParam1) //0 != nParam1 means ( 1 == nParam1) || (0 != nParam1)
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The CFW_INIT_STATUS_SMS failed! nParam1 = 0x%x", nParam1);
		
#ifdef __ADP_MULTI_SIM__
		DummyInitSMSForNoSim(nParam1, nParam2, nType);
#endif
		return;
	}
			
	//SendEventToMMI(NULL, PRT_MSG_ID_MMI_SMS_STARTUP_BEGIN_IND);
	SendEventToMMI(NULL, adp_pSMSContext->nStartupBeginMsgId);
	
#if 0 //需要在初始化结束时发送此消息
	SetCurrentStorage();

	if (adp_pSMSContext->bSIMFull && adp_pSMSContext->bMEFull)           //当手机和SIM卡都存储满时,图标闪烁需要发消息
	{
		mmi_sms_mem_full_ind_struct *pMemFull = OslMalloc(sizeof(mmi_sms_mem_full_ind_struct));
		ASSERT(NULL != pMemFull);
		memset(pMemFull, 0, sizeof(mmi_sms_mem_full_ind_struct));
		DealSapceFullWaiting(0);
		pMemFull->type = SMSAL_MEM_FULL_NORMAL;
		SendEventToMMI((local_para_struct*)pMemFull, PRT_MSG_ID_MMI_SMS_MEM_FULL_IND);
	}
#endif
#ifdef __ADP_MULTI_SIM__
	 mmi_trace(g_sw_ADP_SMS, "ADP_SMS: InitSMS, sim%d LIST STATE = %d,adp_pSMSContext->nListIndex is %d",adp_pSMSContext->nSimId,adp_pSMSContext->SMSListState, adp_pSMSContext->nListIndex);
#else
	 mmi_trace(g_sw_ADP_SMS, "ADP_SMS: InitSMS, LIST STATE = %d,adp_pSMSContext->nListIndex is %d",adp_pSMSContext->SMSListState, adp_pSMSContext->nListIndex);
#endif
	 if(adp_pSMSContext->SMSListState == LIST_STATE_ME_START)
	 {
		#ifdef __ADP_MULTI_SIM__
		#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    {
		CFW_SMS_LIST ListInfo;
		ListInfo.nCount = LIST_COUNT_ME;
		ListInfo.nStartIndex = adp_pSMSContext->nListIndex + 1;
		ListInfo.nStorage = CFW_SMS_STORAGE_ME;
		ListInfo.nType = CFW_SMS_TYPE_PP;
		ListInfo.nStatus = CFW_SMS_STORED_STATUS_STORED_ALL;
		ListInfo.nOption = CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER;
		
		nResult = UT_CFW_SmsListMessages(&ListInfo,
										FREE_UTI,
									   	adp_pSMSContext->nSimId);
    }
		#else/*no defined ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
    {
		CFW_SMS_LIST ListInfo;
		U8 Uti = FREE_UTI;
		ListInfo.nCount = LIST_COUNT_ME;
		ListInfo.nStartIndex = adp_pSMSContext->nListIndex + 1;
		ListInfo.nStorage = CFW_SMS_STORAGE_ME;
		ListInfo.nType = CFW_SMS_TYPE_PP;
		ListInfo.nStatus = CFW_SMS_STORED_STATUS_STORED_ALL;
		ListInfo.nOption = CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER;
		#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
		Simu_Rsp_Marker_Struct simuRspMarker;
		simuRspMarker.msgID = EV_CFW_SMS_LIST_MESSAGE_RSP;
		simuRspMarker.nSimID = adp_pSMSContext->nSimId;
		simuRspMarker.Uti = Uti;
		simuRspMarker.pNext = NULL;
		adp_simu_add_Rsp_Marker(&simuRspMarker);
		#endif
		nResult = CFW_SmsListMessages(&ListInfo, 
										Uti, 
										gCurrentSMSSimID);
    }
		#endif /*ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
		#else /*no defined __ADP_MULTI_SIM__*/
		nResult = CFW_SmsListMessages(CFW_SMS_STORAGE_ME/*storage1*/, 
									    CFW_SMS_STORED_STATUS_STORED_ALL, 
									    LIST_COUNT_ME, 
									    1, 
									    CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER,
									   FREE_UTI);
		#endif /*__ADP_MULTI_SIM__*/

		if (nResult != ERR_SUCCESS)
		{
			mmi_trace(g_sw_SMS, "ADP_SMS: call CFW_SmsListMessages for ME error, nresult is:%d", nResult);
			setActualSMSNumber();
			SendEventToMMI(NULL, adp_pSMSContext->nStartupFinishMsgId);
			SendEventToMMI(NULL, adp_pSMSContext->nStartupReadyMsgId);
		}

	

	 }
	 else if(adp_pSMSContext->SMSListState == LIST_STATE_SM_START)
	 {
		 U8 Uti = FREE_UTI;
		#ifdef __ADP_MULTI_SIM__
		CFW_SMS_LIST ListInfo;
		ListInfo.nCount = LIST_COUNT_SIM;
		ListInfo.nStartIndex = adp_pSMSContext->nListIndex + 1;
		ListInfo.nStorage = CFW_SMS_STORAGE_SM;
		ListInfo.nType = CFW_SMS_TYPE_PP;
		ListInfo.nStatus = CFW_SMS_STORED_STATUS_STORED_ALL;
		ListInfo.nOption = CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER;
		#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		nResult = UT_CFW_SmsListMessages(&ListInfo,
									   Uti,
									   adp_pSMSContext->nSimId);
		
		#else/*no defined ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
		#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
		Simu_Rsp_Marker_Struct simuRspMarker;
		simuRspMarker.msgID = EV_CFW_SMS_LIST_MESSAGE_RSP;
		simuRspMarker.nSimID = adp_pSMSContext->nSimId;
		simuRspMarker.Uti = Uti;
		simuRspMarker.pNext = NULL;
		adp_simu_add_Rsp_Marker(&simuRspMarker);
		#endif
		nResult = CFW_SmsListMessages(&ListInfo, 
										Uti, 
										gCurrentSMSSimID);
		#endif /*ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
		#else /*no defined __ADP_MULTI_SIM__*/
		
		nResult = CFW_SmsListMessages(CFW_SMS_STORAGE_SM/*storage*/, 
								   	    CFW_SMS_STORED_STATUS_STORED_ALL, 
								    	    LIST_COUNT_SIM, 
								    	    1, 
								    	    CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER,
								    	    Uti);
		#endif /*__ADP_MULTI_SIM__*/

		if (nResult != ERR_SUCCESS)
		{
			mmi_trace(g_sw_SMS, "ADP_SMS: call CFW_SmsListMessages fro SM error, nresult is:%d", nResult);
			setActualSMSNumber();
			SendEventToMMI(NULL, adp_pSMSContext->nStartupFinishMsgId);
			SendEventToMMI(NULL, adp_pSMSContext->nStartupReadyMsgId);
		}
	 }
	 else if(adp_pSMSContext->SMSListState == LIST_STATE_SM_OVER)
	 {
	 	mmi_trace(g_sw_SMS, "ADP_SMS: directly init finish, send to mmi");
		setActualSMSNumber();
	 	SendEventToMMI(NULL, adp_pSMSContext->nStartupFinishMsgId);
		SendEventToMMI(NULL, adp_pSMSContext->nStartupReadyMsgId);
	 }
}

/**************************************************************

	FUNCTION NAME		: GetProfileNumReq

  	PURPOSE				: Get the profile number
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void GetProfileNumReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI )
{
	mmi_sms_get_profile_num_rsp_struct *pProfileNum = NULL;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry DealGetProfileNumReq");
	ASSERT(NULL != (ilm_struct*)nParam1);
	
	pProfileNum = OslMalloc(sizeof(mmi_sms_get_profile_num_rsp_struct));
	ASSERT(NULL != pProfileNum);
	memset(pProfileNum, 0, sizeof(mmi_sms_get_profile_num_rsp_struct));

	pProfileNum->name_len = 8;
	pProfileNum->profile_no = 1;
	pProfileNum->result = TRUE;

	SendEventToMMI((local_para_struct*)pProfileNum, PRT_MSG_ID_MMI_SMS_GET_PROFILE_NUM_RSP);
}

/**************************************************************

	FUNCTION NAME		: GetProfileReq

  	PURPOSE				: Get the profile
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void GetProfileReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI )
{
	UINT16 error = 0;
	CFW_SMS_PARAMETER smsInfo;
  	mmi_sms_get_profile_params_rsp_struct *pProfileRsp;
	
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS:entry GetProfileReq");

	/**************************************************************************
	//There is just one profile in the current version, so here it's no use for the req message
	mmi_sms_get_profile_params_req_struct *pProfileReq = 
		(mmi_sms_get_profile_params_req_struct *)(((ilm_struct*)nParam1)->local_para_ptr);
	ASSERT(NULL != pProfileReq);
	**************************************************************************/
	ASSERT(NULL != (ilm_struct*)nParam1);
	pProfileRsp = 
		(mmi_sms_get_profile_params_rsp_struct*)OslMalloc(sizeof(mmi_sms_get_profile_params_rsp_struct));
	ASSERT(NULL != 	pProfileRsp);
	memset(pProfileRsp, 0, sizeof(mmi_sms_get_profile_params_rsp_struct));

	memset(&smsInfo, 0, sizeof(CFW_SMS_PARAMETER));
	memset(&pProfileRsp->profile_name, 0, sizeof(l4_name_struct));

#ifdef __ADP_MULTI_SIM__
	ReadRecord(NVRAM_SMSPROFNE_NVRAM_LID + adp_pSMSContext->nSimId, 1, (void *)&pProfileRsp->profile_name, sizeof(l4_name_struct), &error);
#else
	ReadRecord(NVRAM_SMSPROFNE_NVRAM_LID, 1, (void *)&pProfileRsp->profile_name, sizeof(l4_name_struct), &error);
#endif


#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	if (ERR_SUCCESS == UT_CFW_CfgGetSmsParam(&smsInfo, 0, adp_pSMSContext->nSimId))
#else
	if (ERR_SUCCESS == CFW_CfgGetSmsParam(&smsInfo, 0, gCurrentSMSSimID))
#endif
#else
	if (ERR_SUCCESS == CFW_CfgGetSmsParam(&smsInfo, 0))
#endif
	{
		pProfileRsp->result = TRUE;
		
		pProfileRsp->dcs = smsInfo.dcs;

		pProfileRsp->pid = smsInfo.pid;

		pProfileRsp->vp = smsInfo.vp;
		
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS:  get smsInfo.dcs = %d, vp = %d, pid = %d >>>>", smsInfo.dcs, smsInfo.pid,  smsInfo.vp);

		if(smsInfo.nNumber[0] == 0)
		{
			pProfileRsp->sc_addr.length = MAX_CC_ADDR_LEN;
			memset(&pProfileRsp->sc_addr.number, 0, MAX_CC_ADDR_LEN);
		}

		if (smsInfo.nNumber[0] != 0)
		{
		pProfileRsp->sc_addr.length =
			SUL_GsmBcdToAscii((UINT8 *)&(smsInfo.nNumber[2]), smsInfo.nNumber[0] -1, pProfileRsp->sc_addr.number); 
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: SUL_GsmBcdToAscii");

		}//wangwb @2007.06.16
		
		pProfileRsp->sc_addr.type = smsInfo.nNumber[1];

		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: the sc length = %d ", pProfileRsp->sc_addr.length);
	}
	else
	{
		pProfileRsp->result = FALSE;
	}

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: get profile send response to MMI");
	SendEventToMMI((local_para_struct*)pProfileRsp, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
}

/**************************************************************

	FUNCTION NAME		: SetProfileReq

  	PURPOSE				: Set the profile
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: 

**************************************************************/
static void SetProfileReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	UINT16 error = 0;
  UINT32 nGetResult;
  mmi_sms_set_profile_params_req_struct *pProfileReq ;
  mmi_sms_set_profile_params_rsp_struct *pProfileRsp ;
  UINT32 nSetResult;
	CFW_SMS_PARAMETER smsInfo;
	ASSERT(NULL != (ilm_struct*)nParam1);
	pProfileReq = 
		(mmi_sms_set_profile_params_req_struct *)(((ilm_struct*)nParam1)->local_para_ptr);
	ASSERT(NULL != pProfileReq);

	pProfileRsp = 
		(mmi_sms_set_profile_params_rsp_struct*)OslMalloc(sizeof(mmi_sms_set_profile_params_rsp_struct));
	ASSERT(NULL != pProfileRsp);
	memset(pProfileRsp, 0, sizeof(mmi_sms_set_profile_params_rsp_struct));

	memset(&smsInfo, 0, sizeof(CFW_SMS_PARAMETER));
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry SetProfileReq>> pProfileReq->profile_name.name_length =%d",pProfileReq->profile_name.name_length );
#ifdef __ADP_MULTI_SIM__
	WriteRecord(NVRAM_SMSPROFNE_NVRAM_LID + adp_pSMSContext->nSimId, 1, (void *)&pProfileReq->profile_name, sizeof(l4_name_struct), &error);	
#else
	WriteRecord(NVRAM_SMSPROFNE_NVRAM_LID, 1, (void *)&pProfileReq->profile_name, sizeof(l4_name_struct), &error);	
#endif
	
//	WriteRecord(NVRAM_SMSPROSC_NVRAM_LID, 1, (void *)&pProfileReq->sc_addr, sizeof(l4c_number_struct), &error);
#if 0//TBM780 hansl 2007/6/15 B
	CFW_SMSFileName = pfopen("C:/SMSProfile_Name.dat", FS_O_CREAT|FS_O_RDWR, 0);                //add by yaosq 20061124
	if (CFW_SMSFileName != NULL)
		CFW_SMSWriteName = pfwrite(CFW_SMSFileName, &pProfileReq->profile_name, sizeof(l4_name_struct));
	pfclose(CFW_SMSFileName);

	CFW_SMSFileSc = pfopen("C:/SMSProfile_Sc.dat", FS_O_CREAT|FS_O_RDWR, 0);                //add by yaosq 20061124
	if (CFW_SMSFileSc != NULL)
		CFW_SMSWriteSc = pfwrite(CFW_SMSFileSc, &pProfileReq->sc_addr, sizeof(l4c_number_struct));
	pfclose(CFW_SMSFileSc);
#endif//TBM780 hansl 2007/6/15 E
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	 nGetResult = UT_CFW_CfgGetSmsParam(&smsInfo, 0, adp_pSMSContext->nSimId);
#else
	nGetResult = CFW_CfgGetSmsParam(&smsInfo, 0, gCurrentSMSSimID);
#endif

#else
	 nGetResult = CFW_CfgGetSmsParam(&smsInfo, 0);
#endif
	if (ERR_SUCCESS == nGetResult)
	{
		smsInfo.pid = pProfileReq->pid;
		smsInfo.vp = pProfileReq->vp;

		/*Wangzl Add Start For 5771 Ver: TBM_780  on 2007-8-9 15:14 */
		smsInfo.nNumber[0] = (pProfileReq->sc_addr.length + 1) / 2 + 1;
		smsInfo.nNumber[1] = pProfileReq->sc_addr.type;
		SUL_AsciiToGsmBcd(pProfileReq->sc_addr.number,pProfileReq->sc_addr.length,&smsInfo.nNumber[2]);
		/*Wangzl Add End  For 5771 Ver: TBM_780  on 2007-8-9 15:14 */
		smsInfo.nIndex = 0;
		smsInfo.bDefault = TRUE;
		smsInfo.nSaveToSimFlag = 1;
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		nSetResult = UT_CFW_CfgSetSmsParam(&smsInfo, FREE_UTI, adp_pSMSContext->nSimId);
#else
		nSetResult = CFW_CfgSetSmsParam(&smsInfo, FREE_UTI, gCurrentSMSSimID);
#endif

#else
		nSetResult = CFW_CfgSetSmsParam(&smsInfo, FREE_UTI);

#endif
		if (ERR_SUCCESS == nSetResult)
		{
			pProfileRsp->cause = 0;
			pProfileRsp->result = TRUE;
			if(smsInfo.nSaveToSimFlag != 1)   //当存储到SIM卡上时，等待异步事件到后才发送
			{
				mmi_trace(g_sw_ADP_SMS,"ADP_SMS: send success resposne to mmi");
				SendEventToMMI((local_para_struct *)pProfileRsp, PRT_MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_RSP);
			}
		}
		else
		{
			CswError2CSDError(&pProfileRsp->cause, nSetResult);
			pProfileRsp->result = FALSE;
			SendEventToMMI((local_para_struct *)pProfileRsp, PRT_MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_RSP);
		}
	}
	else
	{
		CswError2CSDError(&pProfileRsp->cause, nGetResult);
		pProfileRsp->result = FALSE;
		SendEventToMMI((local_para_struct *)pProfileRsp, PRT_MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_RSP);
	}

}


//当存储到SIM卡上时，异步事件发生
static void SendGetProfileRsp(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
  mmi_sms_set_profile_params_rsp_struct *pProfileRsp = NULL;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry SendGetProfileRsp, nType is: %d", nType);
	
	pProfileRsp = (mmi_sms_set_profile_params_rsp_struct*)OslMalloc(sizeof(mmi_sms_set_profile_params_rsp_struct));
	ASSERT(NULL != pProfileRsp);
	memset(pProfileRsp, 0, sizeof(mmi_sms_set_profile_params_rsp_struct));
	 
	if (0 == nType)
	{
		pProfileRsp->cause = 0;
		pProfileRsp->result = TRUE;
	}
	else
	{
		pProfileRsp->result = FALSE;
		CswError2CSDError(&pProfileRsp->cause, nParam1);

		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The send message RSP faill reason = 0x%0x", nParam1);
	}

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: pProfileRsp->cause = 0x%0x, pProfileRsp->result = 0x%0x", pProfileRsp->cause,pProfileRsp->result);
	
	SendEventToMMI((local_para_struct*)pProfileRsp, PRT_MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_RSP);
}

/**************************************************************

	FUNCTION NAME		: GetCommonReq

  	PURPOSE				: Get the common setting
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void GetCommonReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	UINT32 nResult;
	mmi_sms_get_common_params_rsp_struct *pCommonRsp ;
	CFW_SMS_PARAMETER smsInfo;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry GetCommonReq");
	ASSERT(NULL != (ilm_struct*)nParam1);
	pCommonRsp = 
		(mmi_sms_get_common_params_rsp_struct*)OslMalloc(sizeof(mmi_sms_get_common_params_rsp_struct));
	ASSERT(NULL != pCommonRsp);
	memset(pCommonRsp, 0, sizeof(mmi_sms_get_common_params_rsp_struct));

	memset(&smsInfo, 0, sizeof(CFW_SMS_PARAMETER));

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	 nResult = UT_CFW_CfgGetSmsParam(&smsInfo, 0, adp_pSMSContext->nSimId);
#else
	 nResult = CFW_CfgGetSmsParam(&smsInfo, 0, gCurrentSMSSimID);
#endif

#else
	 nResult = CFW_CfgGetSmsParam(&smsInfo, 0);

#endif

	if (ERR_SUCCESS == nResult)
	{
		pCommonRsp->result = TRUE;
		
		pCommonRsp->bearer_service = smsInfo.bearer;

		pCommonRsp->fo = 0;

		pCommonRsp->reply_path = smsInfo.rp;

		pCommonRsp->status_report = smsInfo.ssr;
	}
	else
	{
		pCommonRsp->result = FALSE;
	}
	
	SendEventToMMI((local_para_struct*)pCommonRsp, PRT_MSG_ID_MMI_SMS_GET_COMMON_PARAMS_RSP);
}

/**************************************************************

	FUNCTION NAME		: SetCommonReq

  	PURPOSE				: Set the common setting
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void SetCommonReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	mmi_sms_set_common_params_req_struct *pCommonReq ;
	mmi_sms_set_common_params_rsp_struct *pCommonRsp;
	CFW_SMS_PARAMETER smsInfo;
	UINT32 nGetResult;
	UINT32 nSetResult;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry SetCommonReq");
	ASSERT(NULL != (ilm_struct*)nParam1);
	pCommonReq = 
		(mmi_sms_set_common_params_req_struct *)(((ilm_struct*)nParam1)->local_para_ptr);
	ASSERT(NULL != pCommonReq);

	pCommonRsp =
		(mmi_sms_set_common_params_rsp_struct*)OslMalloc(sizeof(mmi_sms_set_common_params_rsp_struct));
	ASSERT(NULL != pCommonRsp);
	memset(pCommonRsp, 0, sizeof(mmi_sms_set_common_params_rsp_struct));

	memset(&smsInfo, 0, sizeof(CFW_SMS_PARAMETER));

#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	nGetResult = UT_CFW_CfgGetSmsParam(&smsInfo, 0, adp_pSMSContext->nSimId);
#else
	nGetResult = CFW_CfgGetSmsParam(&smsInfo, 0, gCurrentSMSSimID);
#endif

#else
	nGetResult = CFW_CfgGetSmsParam(&smsInfo, 0);
#endif

	if (ERR_SUCCESS == nGetResult)
	{
		smsInfo.bearer = pCommonReq->bearer_service;
		smsInfo.rp = pCommonReq->reply_path;
		smsInfo.ssr = pCommonReq->status_report;
		smsInfo.nIndex = 0;
		smsInfo.bDefault = TRUE;
		smsInfo.nSaveToSimFlag = 0;
#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		nSetResult = UT_CFW_CfgSetSmsParam(&smsInfo, FREE_UTI, adp_pSMSContext->nSimId);
#else
		nSetResult = CFW_CfgSetSmsParam(&smsInfo, FREE_UTI, gCurrentSMSSimID);
#endif

#else
	nSetResult = CFW_CfgSetSmsParam(&smsInfo, FREE_UTI);
#endif
		if (ERR_SUCCESS == nSetResult)
		{
			pCommonRsp->cause = 0;
			pCommonRsp->result = TRUE;
		}
		else
		{
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgSetSmsParam failed ,return 0x%x", nSetResult);
			CswError2CSDError(&pCommonRsp->cause, nSetResult);
			pCommonRsp->result = FALSE;
		}
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgGetSmsParam failed ,return 0x%x", nGetResult);
		CswError2CSDError(&pCommonRsp->cause, nGetResult);
		pCommonRsp->result = FALSE;
	}

	SendEventToMMI((local_para_struct*)pCommonRsp, PRT_MSG_ID_MMI_SMS_SET_COMMON_PARAMS_RSP);
}

/**************************************************************

	FUNCTION NAME		: GetPreferredStorageReq

  	PURPOSE				: Get the preferred storage
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void GetPreferredStorageReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	UINT8 nStorage;
	UINT32 ret = ERR_SUCCESS;
	mmi_sms_get_preferred_storage_rsp_struct *pPreStorageRsp;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry GetPreferredSorageReq");
	ASSERT(NULL != (ilm_struct*)nParam1);
	pPreStorageRsp = 
		(mmi_sms_get_preferred_storage_rsp_struct*)OslMalloc(sizeof(mmi_sms_get_preferred_storage_rsp_struct));
	ASSERT(NULL != pPreStorageRsp);
	memset(pPreStorageRsp, 0, sizeof(mmi_sms_get_preferred_storage_rsp_struct));

	 nStorage = 0;
	 
#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	ret = UT_CFW_CfgGetSmsStorage(&nStorage, adp_pSMSContext->nSimId);
#else
	ret = CFW_CfgGetSmsStorage(&nStorage,  gCurrentSMSSimID);
#endif
#else
	ret = CFW_CfgGetSmsStorage(&nStorage);
#endif

	if(ERR_SUCCESS == ret)
	{
		pPreStorageRsp->result = TRUE;
		CswStorage2CSDStorage(&pPreStorageRsp->info.mem2, nStorage);

		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The current storage is %0x, preferred storage is %0x", adp_pSMSContext->nCurrentStorage, nStorage);
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgGetSmsStorage failed, ret value is 0x%x", ret);
		pPreStorageRsp->result = FALSE;
	}
	
	SendEventToMMI((local_para_struct*)pPreStorageRsp, PRT_MSG_ID_MMI_SMS_GET_PREFERRED_STORAGE_RSP);
}

/**************************************************************

	FUNCTION NAME		: SetPreferredStorageReq

  	PURPOSE				: Set the preferred storage
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void SetPreferredStorageReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	mmi_sms_set_preferred_storage_req_struct *pPreStorageReq;
	mmi_sms_set_preferred_storage_rsp_struct *pPreStorageRsp;
	UINT8 nStorage;
	UINT32 ret;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry SetPreferredStorageReq ");
	ASSERT(NULL != (ilm_struct*)nParam1);
	pPreStorageReq = 
		(mmi_sms_set_preferred_storage_req_struct *)(((ilm_struct*)nParam1)->local_para_ptr);
	ASSERT(NULL != pPreStorageReq);

	pPreStorageRsp = 
		(mmi_sms_set_preferred_storage_rsp_struct*)OslMalloc(sizeof(mmi_sms_set_preferred_storage_rsp_struct));
	ASSERT(NULL != pPreStorageRsp);
	memset(pPreStorageRsp, 0, sizeof(mmi_sms_set_preferred_storage_rsp_struct));

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: SetPreferredStorageReq mem1 = %d, mem2 = %d, mem3 = %d", 
		pPreStorageReq->mem1, pPreStorageReq->mem2, pPreStorageReq->mem3);
	
	pPreStorageRsp->param.mem2 = pPreStorageReq->mem2;
	
	 nStorage = 0;
	if (pPreStorageReq->mem2 == SMSAL_SM_PREFER ||pPreStorageReq->mem2 == SMSAL_SM)
	{
		nStorage =  CFW_SMS_STORAGE_SM; 
	}
	else
	{
		nStorage = CFW_SMS_STORAGE_ME;
	}

#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	ret = CFW_CfgSetSmsStorage(nStorage);
#else
	ret = CFW_CfgSetSmsStorage(nStorage, gCurrentSMSSimID);
#endif

#else
	ret = CFW_CfgSetSmsStorage(nStorage);
#endif
	if(ret == ERR_SUCCESS)
	{
			
			pPreStorageRsp->result = TRUE;

			SetCurrentStorage();
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgSetSmsStorage failed!!! ret is: 0x%x",ret);
		pPreStorageRsp->result = FALSE;
	}
	
	SendEventToMMI((local_para_struct*)pPreStorageRsp, PRT_MSG_ID_MMI_SMS_SET_PREFERRED_STORAGE_RSP);
}

/**************************************************************

	FUNCTION NAME		: GetMessageNumReq

  	PURPOSE				: Get the message number to get the percent used in the storage
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void GetMessageNumReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	mmi_sms_get_msg_num_rsp_struct *pMsgNumRsp;
	UINT8 nStorage1;
	UINT8 nStorage2;
UINT32 nResult;
	CFW_SMS_STORAGE_INFO storageInfo;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry GetMessageNumReq");

	pMsgNumRsp = 
		(mmi_sms_get_msg_num_rsp_struct*)OslMalloc(sizeof(mmi_sms_get_msg_num_rsp_struct));
	ASSERT(NULL != pMsgNumRsp);
	memset(pMsgNumRsp, 0, sizeof(mmi_sms_get_msg_num_rsp_struct));


	memset(&storageInfo, 0, sizeof(CFW_SMS_STORAGE_INFO));

	nStorage1 = 0;
	nStorage2 = 0;

#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_CfgGetSmsStorage(&nStorage1,adp_pSMSContext->nSimId);
#else
	CFW_CfgGetSmsStorage(&nStorage1,gCurrentSMSSimID);
#endif
#else
	CFW_CfgGetSmsStorage(&nStorage1);
#endif

#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	nResult = UT_CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM, adp_pSMSContext->nSimId);
#else
	nResult = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM, gCurrentSMSSimID);
#endif
#else
	nResult = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_SM);
#endif

	if (ERR_SUCCESS == nResult)
	{
		pMsgNumRsp->in_sim_no = storageInfo.usedSlot - adp_pSMSContext->nSMSCountSIM;
		pMsgNumRsp->out_sim_no = 0;
		pMsgNumRsp->is_sim_card_change = 0;

		pMsgNumRsp->total_sim_num = storageInfo.totalSlot;
		pMsgNumRsp->unread_msg_num = storageInfo.unReadRecords;

		memset(&storageInfo, 0, sizeof(CFW_SMS_STORAGE_INFO));

#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		nResult = UT_CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME, adp_pSMSContext->nSimId);
#else
		nResult = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME, gCurrentSMSSimID);
#endif
#else
		nResult = CFW_CfgGetSmsStorageInfo(&storageInfo, CFW_SMS_STORAGE_ME);
#endif
		if (ERR_SUCCESS == nResult)
		{
			pMsgNumRsp->in_me_no = storageInfo.usedSlot - adp_pSMSContext->nSMSCountMe;
			pMsgNumRsp->out_me_no = 0;
			pMsgNumRsp->total_me_num = storageInfo.totalSlot;
			pMsgNumRsp->unread_msg_num += storageInfo.unReadRecords;

			pMsgNumRsp->result = TRUE;
		}
		else
		{
			pMsgNumRsp->result = FALSE;
		}
	}
	else
	{
		pMsgNumRsp->result = FALSE;
	}

	SendEventToMMI((local_para_struct*)pMsgNumRsp, PRT_MSG_ID_MMI_SMS_GET_MSG_NUM_RSP);
}

/**************************************************************

	FUNCTION NAME		: GetVoiceMailReq

  	PURPOSE				: Read voicemail
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void GetVoiceMailReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{

	INT32 ret1 = 0;
	INT32 ret2 = 0;
	UINT16 error = 0;
	l4c_sms_mailbox_info_struct pVm1;
	l4c_sms_mailbox_info_struct pVm2;
	ilm_struct  ilm_ptr;
	mmi_sms_get_mailbox_address_rsp_struct *pVm = OslMalloc(sizeof(mmi_sms_get_mailbox_address_rsp_struct));
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry GetVoiceMailReq");
	ASSERT(pVm != NULL);
	memset(pVm, 0x00, sizeof(mmi_sms_get_mailbox_address_rsp_struct));

	memset(&pVm1, 0x00, sizeof(l4c_sms_mailbox_info_struct));
	memset(&pVm2, 0x00, sizeof(l4c_sms_mailbox_info_struct));

#ifdef __ADP_MULTI_SIM__
	ret1 = ReadRecord(NVRAM_VOICEMAIL1_NVRAM_LID + adp_pSMSContext->nSimId, 1, (void *)&pVm1, sizeof(l4c_sms_mailbox_info_struct), &error); 
#else  /* No Defined __ADP_MULTI_SIM__ */
	ret1 = ReadRecord(NVRAM_VOICEMAIL1_NVRAM_LID, 1, (void *)&pVm1, sizeof(l4c_sms_mailbox_info_struct), &error); 
#endif /* __ADP_MULTI_SIM__ */
	if (ret1 > 0)
	{
		pVm->mailbox_num++;
		memcpy((void*)&pVm->mailbox_info[0], (void*)&pVm1, sizeof(l4c_sms_mailbox_info_struct));
	}
	else
	{
		pVm->mailbox_num++;
		memset((void*)&pVm->mailbox_info[0].name, 0, sizeof(l4_name_struct));
	}
#ifdef __ADP_MULTI_SIM__
	ret2 = ReadRecord(NVRAM_VOICEMAIL2_NVRAM_LID + adp_pSMSContext->nSimId, 1, (void *)&pVm2, sizeof(l4c_sms_mailbox_info_struct), &error); 
#else  /* No Defined __ADP_MULTI_SIM__ */
	ret2 = ReadRecord(NVRAM_VOICEMAIL2_NVRAM_LID , 1, (void *)&pVm2, sizeof(l4c_sms_mailbox_info_struct), &error); 
#endif /* __ADP_MULTI_SIM__ */
	if (ret2 > 0)
	{
		pVm->mailbox_num++;
		memcpy((void*)&pVm->mailbox_info[1], (void*)&pVm2, sizeof(l4c_sms_mailbox_info_struct));
	}
	else
	{
		pVm->mailbox_num++;
		memset((void*)&pVm->mailbox_info[1].name, 0, sizeof(l4_name_struct));
	}
	pVm->result = TRUE;
	pVm->alpha_length = 10;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
#ifdef __ADP_MULTI_SIM__
	ilm_ptr.src_mod_id += adp_pSMSContext->nSimId;
#endif /* __ADP_MULTI_SIM__ */
	ilm_ptr.msg_id = PRT_MSG_ID_MMI_SMS_GET_MAILBOX_ADDRESS_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)pVm;
	OslMsgSendExtQueue(&ilm_ptr);

}

/**************************************************************

	FUNCTION NAME		: SetVoiceMailReq

  	PURPOSE				: write voicemail
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void SetVoiceMailReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{

	INT32 ret1 = 0;
	UINT16 error = 0;
	l4c_sms_mailbox_info_struct pNvram;
	ilm_struct  ilm_ptr;
	mmi_sms_set_mailbox_address_req_struct *pVm = (mmi_sms_set_mailbox_address_req_struct *)(((ilm_struct*)nParam1)->local_para_ptr);
	mmi_sms_set_mailbox_address_rsp_struct *pVmrsp = OslMalloc(sizeof(mmi_sms_set_mailbox_address_rsp_struct));
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry SetVoiceMailReq");
	ASSERT(pVmrsp != NULL);

	memset(&pNvram, 0x00, sizeof(l4c_sms_mailbox_info_struct));
	memcpy(&(pNvram.name), &(pVm->name), sizeof(l4_name_struct));
	memcpy(&(pNvram.number_info), &(pVm->num_addr), sizeof(l4c_number_struct));

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: SetVoiceMailReq>>pVm->mailbox_num = %d",pVm->mailbox_num);

	if (pVm->mailbox_num == 0)
{
#ifdef __ADP_MULTI_SIM__
		ret1 = WriteRecord(NVRAM_VOICEMAIL1_NVRAM_LID + adp_pSMSContext->nSimId, 1, (void *)&pNvram, sizeof(l4c_sms_mailbox_info_struct), &error); 
#else  /* No Defined __ADP_MULTI_SIM__ */
		ret1 = WriteRecord(NVRAM_VOICEMAIL1_NVRAM_LID, 1, (void *)&pNvram, sizeof(l4c_sms_mailbox_info_struct), &error); 
#endif /* __ADP_MULTI_SIM__ */
}
	else if (pVm->mailbox_num == 1)
{
#ifdef __ADP_MULTI_SIM__
		ret1 = WriteRecord(NVRAM_VOICEMAIL2_NVRAM_LID + adp_pSMSContext->nSimId, 1, (void *)&pNvram, sizeof(l4c_sms_mailbox_info_struct), &error); 
#else  /* No Defined __ADP_MULTI_SIM__ */
		ret1 = WriteRecord(NVRAM_VOICEMAIL2_NVRAM_LID, 1, (void *)&pNvram, sizeof(l4c_sms_mailbox_info_struct), &error); 
#endif /* __ADP_MULTI_SIM__ */
}
	if (ret1 > 0)
		pVmrsp->result = TRUE;
	else
		pVmrsp->result = FALSE;
	pVmrsp->cause = 0;

	ilm_ptr.dest_mod_id = MOD_MMI;
#ifdef __ADP_MULTI_SIM__
	ilm_ptr.src_mod_id = MOD_L4C + adp_pSMSContext->nSimId;
#else  /* No Defined __ADP_MULTI_SIM__ */
	ilm_ptr.src_mod_id = MOD_L4C ;
#endif /* __ADP_MULTI_SIM__ */
	ilm_ptr.msg_id = PRT_MSG_ID_MMI_SMS_SET_MAILBOX_ADDRESS_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)pVmrsp;
	OslMsgSendExtQueue(&ilm_ptr);
}


/**************************************************************

	FUNCTION NAME		: GetMessageReq

  	PURPOSE				: Read message
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void GetMessageReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	mmi_sms_get_msg_req_struct *pMMISmsNode;
	UINT32 nResult;
	mmi_sms_get_msg_rsp_struct *pMMISmsRsp ;
	U8 Uti;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry GetMessageReq");
	ASSERT(NULL != (ilm_struct*)nParam1);
	pMMISmsNode = (mmi_sms_get_msg_req_struct *)(((ilm_struct*)nParam1)->local_para_ptr);

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: index = %d, storage = 0x%0x, phyindex = %d", 
	pMMISmsNode->index, indexMap[pMMISmsNode->index].nStorage, indexMap[pMMISmsNode->index].nPhyIndex);

	nResult = 0;
	Uti = FREE_UTI;
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	nResult = UT_CFW_SmsReadMessage(indexMap[pMMISmsNode->index].nStorage,  
								indexMap[pMMISmsNode->index].nPhyIndex, 
								Uti, adp_pSMSContext->nSimId);
#else
	#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
	Simu_Rsp_Marker_Struct simuRspMarker;
	simuRspMarker.msgID = EV_CFW_SMS_READ_MESSAGE_RSP;
	simuRspMarker.nSimID = adp_pSMSContext->nSimId;
	simuRspMarker.Uti = Uti;
	simuRspMarker.pNext = NULL;
	adp_simu_add_Rsp_Marker(&simuRspMarker);
	#endif
	nResult = CFW_SmsReadMessage(indexMap[pMMISmsNode->index].nStorage,  CFW_SMS_TYPE_PP,
								indexMap[pMMISmsNode->index].nPhyIndex, 
								Uti, gCurrentSMSSimID);
#endif
#else
	nResult = CFW_SmsReadMessage(indexMap[pMMISmsNode->index].nStorage,  
								indexMap[pMMISmsNode->index].nPhyIndex, 
								Uti);
#endif

	

	if (ERR_SUCCESS != nResult)
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_SmsReadMessage failed!!! nResult = 0x%x", nResult);
	
		pMMISmsRsp = NULL;
		pMMISmsRsp = OslMalloc(sizeof(mmi_sms_get_msg_rsp_struct));
		ASSERT(NULL != pMMISmsRsp);
		memset(pMMISmsRsp, 0, sizeof(mmi_sms_get_msg_rsp_struct));

		pMMISmsRsp->result = FALSE;
		CswError2CSDError(&pMMISmsRsp->cause, nResult);

		SendEventToMMI((local_para_struct *) pMMISmsRsp, PRT_MSG_ID_MMI_SMS_GET_MSG_RSP);
	}
	
}

/**************************************************************

	FUNCTION NAME		: DealGetDeliveredTextHeader

  	PURPOSE				: Read message when the message type is "delivered text"
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void DealGetDeliveredTextHeader(CFW_SMS_NODE *pSmsNode, mmi_sms_get_msg_rsp_struct** ppMMISmsRsp)
{
	UINT16 nDataLen;
	CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO *pTxtHrdNode;
	mmi_sms_get_msg_rsp_struct *pMMISmsRsp;
	UINT8 nStatus;
  	UINT8 *pData = NULL;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry DealGetDeliveredTextHeader");
	
	ASSERT(NULL != pSmsNode);
//	UINT16 TempLength = 0;
	
	pTxtHrdNode = 
				(CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)pSmsNode->pNode;

	 nDataLen = (pTxtHrdNode->length == 0 ? 1 : pTxtHrdNode->length) - 1;
	 *ppMMISmsRsp = OslMalloc(sizeof(mmi_sms_get_msg_rsp_struct) + nDataLen);

	 pMMISmsRsp = *ppMMISmsRsp;
	ASSERT(NULL != pMMISmsRsp);
	memset(pMMISmsRsp, 0, sizeof(mmi_sms_get_msg_rsp_struct) + nDataLen);

	#if 0
	pMMISmsRsp->addr_number.length = 
		SUL_GsmBcdToAscii(pTxtHrdNode->oa, pTxtHrdNode->oa_size, pMMISmsRsp->addr_number.number);
	pMMISmsRsp->addr_number.type = pTxtHrdNode->tooa;
	#endif

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: pTxtHrdNode->tooa is:%d, size is:%d",pTxtHrdNode->tooa, pTxtHrdNode->oa_size);
	switch(pTxtHrdNode->tooa)
	{
		case CFW_TELNUMBER_TYPE_INTERNATIONAL:
		case CFW_TELNUMBER_TYPE_NATIONAL:
			pMMISmsRsp->addr_number.length = SUL_GsmBcdToAscii(pTxtHrdNode->oa, pTxtHrdNode->oa_size, pMMISmsRsp->addr_number.number);
			break;
		case 0xd0:
			pMMISmsRsp->addr_number.length = SUL_Decode7Bit(pTxtHrdNode->oa, pMMISmsRsp->addr_number.number, pTxtHrdNode->oa_size);
			break;
		default:
			pMMISmsRsp->addr_number.length = SUL_GsmBcdToAscii(pTxtHrdNode->oa, pTxtHrdNode->oa_size, pMMISmsRsp->addr_number.number);
			break;
			
	}
	pMMISmsRsp->addr_number.type = pTxtHrdNode->tooa;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: addr_number The length = %d, number is: %s, type is:%d", 
								pMMISmsRsp->addr_number.length, pMMISmsRsp->addr_number.number,pMMISmsRsp->addr_number.type);
	
	pMMISmsRsp->cause = 0;

	CswDcs2CSDDcs(&pMMISmsRsp->dcs, pTxtHrdNode->dcs);
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: Get pTxtHrdNode->dcs = 0x%0x, mmi dcs = %d >>>>\n", pTxtHrdNode->dcs, pMMISmsRsp->dcs);

	memset(pMMISmsRsp->dt, 0, sizeof(pMMISmsRsp->dt));

	pMMISmsRsp->fo = pTxtHrdNode->fo;

	pMMISmsRsp->mr = 0;

//	pMMISmsRsp->no_msg_data = pTxtHrdNode->length;

	mmi_trace(g_sw_ADP_SMS,"ADP_SMS: Func: %s pTxtHrdNode->length=%d", __FUNCTION__,pTxtHrdNode->length);

	/*Guojian Add Start For 10629 Ver: GreenStone  on 2008-12-9 13:47 */
	mmi_trace(g_sw_SMS, "ADP_SMS: pTxtHrdNode->data = %c, pTxtHrdNode->lenth = %d, mmi dcs = %d", 
						pTxtHrdNode->data[0], pTxtHrdNode->length, pMMISmsRsp->dcs);
	pData = (UINT8*)CovertStringForGetSMS(pTxtHrdNode->data, pTxtHrdNode->length, pMMISmsRsp->dcs, &(pMMISmsRsp->no_msg_data));
	
	if(pData != NULL)
	{
		memcpy(pMMISmsRsp->msg_data, pData, pMMISmsRsp->no_msg_data);
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: pMMISmsRsp->msg_data[0] = %d, len = %d", pMMISmsRsp->msg_data[0], pMMISmsRsp->no_msg_data);
		OslMfree(pData);
		pData = NULL;
	}
	/*Guojian Add End  For 10629 Ver: GreenStone  on 2008-12-9 13:47 */

#if SMS_TEST_SWITCH
	int i;
	for(i=0; i<pMMISmsRsp->no_msg_data + 2; i++)
	{
		mmi_trace(g_sw_ADP_SMS,"ADP_SMS: Func: %s pMMISmsRsp->msg_data[%d]:0x%x", __FUNCTION__,i, pMMISmsRsp->msg_data[i]);
	}
#endif


	pMMISmsRsp->mti = SMSAL_MTI_DELIVER;

	pMMISmsRsp->ori_dcs = 0;

	pMMISmsRsp->pid = pTxtHrdNode->pid;

	pMMISmsRsp->result = TRUE;

	pMMISmsRsp->sca_number.length = SUL_GsmBcdToAscii(pTxtHrdNode->sca, pTxtHrdNode->sca_size, pMMISmsRsp->sca_number.number);
	pMMISmsRsp->sca_number.type = pTxtHrdNode->tosca;

	Cswtime2CSDtime(pMMISmsRsp->scts, &pTxtHrdNode->scts);

	pMMISmsRsp->st = 0;

	nStatus = 0;
	for(nStatus = 0; nStatus != SMSAL_INVALID_STATUS; ++nStatus)
	{
		if ((1 << nStatus) == pSmsNode->nStatus)
		{
			break;
		}
	}
	pMMISmsRsp->status = nStatus;

	CswStorage2CSDStorage(&pMMISmsRsp->storage_type, pSmsNode->nStorage);

	pMMISmsRsp->vp = 0;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS:  DealGetDeliveredTextHeader End");
}

/**************************************************************

	FUNCTION NAME		: DealGetSubmittedTextHeader

  	PURPOSE				: Read message when the message type is "submit text"
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void DealGetSubmittedTextHeader(CFW_SMS_NODE *pSmsNode, mmi_sms_get_msg_rsp_struct** ppMMISmsRsp)
{
	UINT16 nDataLen;
	mmi_sms_get_msg_rsp_struct *pMMISmsRsp;
	UINT8 nStatus;
	UINT8 *pData;
	CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO *pTxtHrdNode;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry DealGetSubmittedTextHeader");
	
	ASSERT(NULL != pSmsNode);
	
	 pTxtHrdNode = (CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO*)pSmsNode->pNode;

	nDataLen = (pTxtHrdNode->length == 0 ? 1 : pTxtHrdNode->length) - 1;
	*ppMMISmsRsp = OslMalloc(sizeof(mmi_sms_get_msg_rsp_struct) + nDataLen);

	pMMISmsRsp = *ppMMISmsRsp;
	ASSERT(NULL != pMMISmsRsp);
	memset(pMMISmsRsp, 0, sizeof(mmi_sms_get_msg_rsp_struct) + nDataLen);
	
	pMMISmsRsp->addr_number.length = SUL_GsmBcdToAscii(pTxtHrdNode->da, pTxtHrdNode->da_size, pMMISmsRsp->addr_number.number);
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The pMMISmsRsp length = %d, pTxtHrdNode da_size = %d", 
							pMMISmsRsp->addr_number.length, pTxtHrdNode->da_size);
	pMMISmsRsp->addr_number.type = pTxtHrdNode->toda;

	pMMISmsRsp->cause = 0;

	CswDcs2CSDDcs(&pMMISmsRsp->dcs, pTxtHrdNode->dcs);
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: Get Message pTxtHrdNode->dcs %0x, mmi dcs = %d", pTxtHrdNode->dcs, pMMISmsRsp->dcs);

	memset(pMMISmsRsp->dt, 0, sizeof(pMMISmsRsp->dt));

	pMMISmsRsp->fo = pTxtHrdNode->fo;

	pMMISmsRsp->mr = 0;
	
//	pMMISmsRsp->no_msg_data = pTxtHrdNode->length;

	/*Guojian Add Start For 10629 Ver: GreenStone  on 2008-12-9 13:45 */
	pData = (UINT8*)CovertStringForGetSMS(pTxtHrdNode->data, pTxtHrdNode->length, pMMISmsRsp->dcs, &(pMMISmsRsp->no_msg_data));
	if(pData != NULL)
	{
		memcpy(pMMISmsRsp->msg_data, pData, pMMISmsRsp->no_msg_data);

		OslMfree(pData);
		pData = NULL;
	}
	/*Guojian Add End  For 10629 Ver: GreenStone  on 2008-12-9 13:45 */

#if SMS_TEST_SWITCH
	int i;
	for(i=0; i<pMMISmsRsp->no_msg_data; i++)
	{
		mmi_trace(g_sw_ADP_SMS,"ADP_SMS: Func: %s pMMISmsRsp->msg_data[%d]:0x%x", __FUNCTION__,i, pMMISmsRsp->msg_data[i]);
		
	}
#endif
	pMMISmsRsp->mti = SMSAL_MTI_SUBMIT;

	pMMISmsRsp->ori_dcs = 0;

	pMMISmsRsp->pid = pTxtHrdNode->pid;

	pMMISmsRsp->result = TRUE;

	pMMISmsRsp->sca_number.length =
		SUL_GsmBcdToAscii(pTxtHrdNode->sca, pTxtHrdNode->sca_size, pMMISmsRsp->sca_number.number);
	pMMISmsRsp->sca_number.type = pTxtHrdNode->tosca;

	//I think this is CSW bug, there is no time for submit message.
	//Cswtime2CSDtime(pMMISmsRsp->scts, &pTxtHrdNode->scts);

	pMMISmsRsp->st = 0; 

	 nStatus = 0;
	for(nStatus = 0; nStatus != SMSAL_INVALID_STATUS; ++nStatus)
	{
		if ((1 << nStatus) == pSmsNode->nStatus)
		{
			break;
		}
	}
	pMMISmsRsp->status = nStatus;

	CswStorage2CSDStorage(&pMMISmsRsp->storage_type, pSmsNode->nStorage);

	pMMISmsRsp->vp = pTxtHrdNode->vp;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: DealGetSubmittedTextHeader End");
}

/**************************************************************

	FUNCTION NAME		: GetMessageRsp

  	PURPOSE				: Read message RSP processing
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void GetMessageRsp(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	mmi_sms_get_msg_rsp_struct* pMMISmsRsp = NULL;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry DealGetMessageRsp");
	
	if (0 == nType)
	{
		CFW_SMS_NODE *pSmsNode = (CFW_SMS_NODE*)nParam1;

		switch (pSmsNode->nType)
		{
		case ADP_CFW_SMS_TXT_DELIVERED_NO_HRD_INFO:
			break;
			
		case ADP_CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO:
			DealGetDeliveredTextHeader(pSmsNode, &pMMISmsRsp);
			break;

		case ADP_CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO:
			break;

		case ADP_CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO:
			DealGetSubmittedTextHeader(pSmsNode, &pMMISmsRsp);
			break;
			
		default:
			break;
		}
	}
	else
	{
		pMMISmsRsp = OslMalloc(sizeof(mmi_sms_get_msg_rsp_struct));
		ASSERT(NULL != pMMISmsRsp);
		memset(pMMISmsRsp, 0, sizeof(mmi_sms_get_msg_rsp_struct));

		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: Read Message RSP faile!!!ntype is: %d", nType);
		pMMISmsRsp->result = FALSE;
		CswError2CSDError(&pMMISmsRsp->cause, nParam1);
	}
	
	SendEventToMMI((local_para_struct *) pMMISmsRsp, PRT_MSG_ID_MMI_SMS_GET_MSG_RSP);
}

/**************************************************************

	FUNCTION NAME		: SendMessageReq

  	PURPOSE				: Deal "send message" req
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void SendMessageReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	mmi_sms_send_msg_req_struct* pMMISmsNode;
	CFW_DIALNUMBER dailNumber;
	UINT8 cBcd[21] = {0};
	CFW_SMS_PARAMETER smsInfo;
	UINT32 nResult = ERR_SUCCESS;
	UINT16 nMessageLength;
	UINT8 *pData ;
	mmi_sms_send_msg_rsp_struct *pMMISmsRSP = NULL;
	U8 Uti;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry SendMessageReq");
	ASSERT(NULL != (ilm_struct*)nParam1);
	 pMMISmsNode = (mmi_sms_send_msg_req_struct*)((ilm_struct*)nParam1)->local_para_ptr;
	ASSERT(NULL != pMMISmsNode);
	
	bAbortSendMessage = FALSE;
	
	memset(&dailNumber, 0, sizeof(CFW_DIALNUMBER));

	
	dailNumber.nDialNumberSize = SUL_AsciiToGsmBcd(pMMISmsNode->dest.number, pMMISmsNode->dest.length, cBcd);
	dailNumber.pDialNumber = cBcd;

	dailNumber.nType = pMMISmsNode->dest.type;
	
	dailNumber.nClir = 0;

	memset(&smsInfo, 0, sizeof(CFW_SMS_PARAMETER));

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_CfgGetSmsParam(&smsInfo, 0, adp_pSMSContext->nSimId);
#else
	CFW_CfgGetSmsParam(&smsInfo, 0, gCurrentSMSSimID);
#endif
#else
	CFW_CfgGetSmsParam(&smsInfo, 0);
#endif

	smsInfo.dcs = pMMISmsNode->dcs;
	smsInfo.nIndex = 0;
	smsInfo.bDefault = TRUE;
	smsInfo.nSaveToSimFlag = 0;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The send message req dcs = %0x", smsInfo.dcs);
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	nResult = UT_CFW_CfgSetSmsParam(&smsInfo, FREE_UTI, adp_pSMSContext->nSimId);
#else
	nResult = CFW_CfgSetSmsParam(&smsInfo, FREE_UTI, gCurrentSMSSimID);
#endif
#else
	nResult = CFW_CfgSetSmsParam(&smsInfo, FREE_UTI);
#endif
	if(ERR_SUCCESS != nResult)
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgSetSmsParam failed, nResult = 0x%x", nResult);
		
		pMMISmsRSP = OslMalloc(sizeof(mmi_sms_set_msg_rsp_struct));
		ASSERT(NULL != pMMISmsRSP);

		pMMISmsRSP->result = FALSE;
		CswError2CSDError(&pMMISmsRSP->cause, nResult);

		SendEventToMMI((local_para_struct*)pMMISmsRSP, PRT_MSG_ID_MMI_SMS_SEND_MSG_RSP);
		return;
	}

	/*Guojian Add Start For 10629 Ver: GreenStone  on 2008-12-9 13:48 */
	 nMessageLength = 0;
	pData = CovertStringForSetSMS(pMMISmsNode->msg, pMMISmsNode->length, pMMISmsNode->dcs, &nMessageLength);
	if (pData != NULL)
	{
		memcpy(pMMISmsNode->msg, pData, pMMISmsNode->length);
	
		OslMfree(pData);
		pData = NULL;
	}
	/*Guojian Add End  For 10629 Ver: GreenStone  on 2008-12-9 13:48 */
	
	mmi_trace(g_sw_ADP_SMS,"ADP_SMS: Func: %s pMMISmsNode->length=%d", __FUNCTION__,pMMISmsNode->length);

#if SMS_TEST_SWITCH
	int i;	
	for (i=0; i<pMMISmsNode->length; i++)
	{
		mmi_trace(g_sw_ADP_SMS,"ADP_SMS: Func: %s pMMISmsNode->msg[%d]:0x%x", __FUNCTION__,i,pMMISmsNode->msg[i]);	
	}
#endif 

	Uti = FREE_UTI;
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	nResult = UT_CFW_SmsSendMessage(&dailNumber, pMMISmsNode->msg, pMMISmsNode->length, Uti, adp_pSMSContext->nSimId);
#else
	#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
	Simu_Rsp_Marker_Struct simuRspMarker;
	simuRspMarker.msgID = EV_CFW_SMS_SEND_MESSAGE_RSP;
	simuRspMarker.nSimID = adp_pSMSContext->nSimId;
	simuRspMarker.Uti = Uti;
	simuRspMarker.pNext = NULL;
	adp_simu_add_Rsp_Marker(&simuRspMarker);
	#endif
	nResult = CFW_SmsSendMessage(&dailNumber, pMMISmsNode->msg, pMMISmsNode->length, Uti, gCurrentSMSSimID);
#endif
#else
	nResult = CFW_SmsSendMessage(&dailNumber, pMMISmsNode->msg, pMMISmsNode->length, Uti);
#endif

	
	if (ERR_SUCCESS != nResult)
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_SmsSendMessage failed! reason = %0x", nResult);

		pMMISmsRSP = OslMalloc(sizeof( mmi_sms_send_msg_rsp_struct));
	 	ASSERT(NULL != pMMISmsRSP);
	 	memset(pMMISmsRSP, 0, sizeof(mmi_sms_send_msg_rsp_struct));

		pMMISmsRSP->result = FALSE;
		CswError2CSDError(&pMMISmsRSP->cause, nResult);

		SendEventToMMI((local_para_struct*)pMMISmsRSP, PRT_MSG_ID_MMI_SMS_SEND_MSG_RSP);
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_SmsSendMessage OK!");
	}
}

/**************************************************************

	FUNCTION NAME		: SendMessageRsp

  	PURPOSE				: Deal "send message" rsp
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void SendMessageRsp(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	mmi_sms_send_msg_rsp_struct *pMMISmsNode;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry DealSendMessageRsp");
	
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	if (SimuGetSMSID() == ADP_SIM_1)
	{
		mmi_trace(g_sw_SMS, "ADP_SMS: send sim 1 id error!");
		adp_pSMSContext = &adp_gSMSContext[0];
	}
	else if(SimuGetSMSID() == ADP_SIM_2)
	{
		mmi_trace(g_sw_SMS, "ADP_SMS: send sim 2 id error!");
		adp_pSMSContext = &adp_gSMSContext[1];
	}
	else
	{
		mmi_trace(g_sw_SMS, "ADP_SMS: send sim id error!");
	}
#endif

	 pMMISmsNode = OslMalloc(sizeof( mmi_sms_send_msg_rsp_struct));
	 ASSERT(NULL != pMMISmsNode);
	 memset(pMMISmsNode, 0, sizeof(mmi_sms_send_msg_rsp_struct));
	 
	if (0 == nType)
	{
		TM_SYSTEMTIME systemTime;
		memset(&systemTime, 0, sizeof(TM_SYSTEMTIME));
		
		if (TM_GetSystemTime(&systemTime))
		{
			TM_SMS_TIME_STAMP timeStamp;
			memset(&timeStamp, 0, sizeof(TM_SMS_TIME_STAMP));
			
			timeStamp.iZone = 0;
			timeStamp.uDay = systemTime.uDay;
			timeStamp.uHour = systemTime.uHour;
			timeStamp.uMinute = systemTime.uMinute;
			timeStamp.uMonth = systemTime.uMonth;
			timeStamp.uSecond = systemTime.uSecond;
			timeStamp.uYear = systemTime.uYear;

			Cswtime2CSDtime(pMMISmsNode->scts, &timeStamp);

			if (nParam2 == 0)
			{
				pMMISmsNode->concat_info.total_seg = 0;
				pMMISmsNode->concat_info.seg = 0;
			}
			else
			{
				pMMISmsNode->concat_info.total_seg = (UINT8)((nParam2 & 0xffff0000)>> 16);
				pMMISmsNode->concat_info.seg = (UINT8)(nParam2 & 0x0000ffff);
			} //wangwb add @2007.06.28

			//memset(&pMMISmsNode->concat_info, 0, sizeof(smsal_concat_struct));      //wangwb close @2007.06.28
			
			pMMISmsNode->mr = 0;

			pMMISmsNode->result = TRUE;
			pMMISmsNode->cause = 0;
		}
		else
		{
			pMMISmsNode->result = FALSE;
			CswError2CSDError(&pMMISmsNode->cause, SUL_GetLastError());
		}
	}
	else
	{
		pMMISmsNode->result = FALSE;
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The send message RSP faill!!!, ntype is:%d", nType);
		CswError2CSDError(&pMMISmsNode->cause, nParam1);

		
	}
	if(bAbortSendMessage)
	{
		mmi_sms_send_abort_finish_ind_struct *msgRsp = NULL;
		msgRsp = (mmi_sms_send_abort_finish_ind_struct *)OslMalloc(sizeof(mmi_sms_send_abort_finish_ind_struct));
		ASSERT(NULL != msgRsp);
		memset(msgRsp, 0, sizeof(mmi_sms_send_abort_finish_ind_struct));

		msgRsp->result = TRUE;

		OslMfree(pMMISmsNode);
		bAbortSendMessage = FALSE;

		SendEventToMMI((local_para_struct *) msgRsp, PRT_MSG_ID_MMI_SMS_SEND_ABORT_FINISH_IND);
	}
	else
	{
		SendEventToMMI((local_para_struct*)pMMISmsNode, PRT_MSG_ID_MMI_SMS_SEND_MSG_RSP);
	}
}

U16 mmi_frm_sms_get_index (U16 index, U8 *index_bitmap);
void mmi_frm_sms_set_bitmap (U16 index, U8 *index_bitmap);

void SendDeleteFailToMMI(UINT32 nResult)
{
		mmi_sms_del_msg_rsp_struct *pDelRsp = NULL;
		pDelRsp = (mmi_sms_del_msg_rsp_struct*)OslMalloc(sizeof(mmi_sms_del_msg_rsp_struct));
		ASSERT(pDelRsp != NULL);
		memset(pDelRsp, 0, sizeof(mmi_sms_del_msg_rsp_struct));

		pDelRsp->result = FALSE;
		CswError2CSDError(&pDelRsp->cause, nResult);

		SendEventToMMI((local_para_struct*)pDelRsp, PRT_MSG_ID_MMI_SMS_DEL_MSG_RSP);
}
/**************************************************************

	FUNCTION NAME		: DeleteMessageReq

  	PURPOSE				: Deal "delete message" req
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
BOOL delete_inbox_sm = FALSE;
BOOL delete_outbox_sm = FALSE;
UINT8 delete_box_type = 0;
static void DeleteMessageReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	mmi_sms_del_msg_req_struct *pDelReq ;
	
	UINT32 nResult;
	UINT16 nIndex;
	U8 Uti;

	BOOL bALLFail; //for delete fail

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: Entry DeleteMessageReq");
	ASSERT(NULL != (ilm_struct*)nParam1);
	pDelReq =  (mmi_sms_del_msg_req_struct*)(((ilm_struct*)nParam1)->local_para_ptr);;
	ASSERT(NULL != pDelReq);

	memcpy(adp_pSMSContext->adpDelBitmap, pDelReq->del_bitmap, sizeof(UINT8)*480);

	 nResult = 0;
	nIndex = 0;

	bALLFail = TRUE; //for delete fail

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: pDelReq->del_flag  = %d", pDelReq->del_flag);
	if (pDelReq->del_flag == SMSAL_DEL_INBOX)
	{
		Uti = FREE_UTI;
		#ifdef __ADP_MULTI_SIM__
		#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		nResult = UT_CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNREAD|CFW_SMS_STORED_STATUS_READ, CFW_SMS_STORAGE_ME,CFW_SMS_TYPE_PP, Uti, adp_pSMSContext->nSimId);
		#else
		#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
		Simu_Rsp_Marker_Struct simuRspMarker;
		simuRspMarker.msgID = EV_CFW_SMS_DELETE_MESSAGE_RSP;
		simuRspMarker.nSimID = adp_pSMSContext->nSimId;
		simuRspMarker.Uti = Uti;
		simuRspMarker.pNext = NULL;
		adp_simu_add_Rsp_Marker(&simuRspMarker);
		#endif
		nResult = CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNREAD|CFW_SMS_STORED_STATUS_READ, CFW_SMS_STORAGE_ME,CFW_SMS_TYPE_PP, Uti, gCurrentSMSSimID);
		#endif
		#else
		nResult = CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNREAD|CFW_SMS_STORED_STATUS_READ, CFW_SMS_STORAGE_ME, Uti);
		#endif

		if (nResult == ERR_SUCCESS )
		{
			delete_inbox_sm = TRUE;
			delete_box_type = BOXTYPE_IN;
			bALLFail = FALSE;
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: all inbox has been deleted!");
		}
		else
			mmi_trace(g_sw_ADP_SMS,"ADP_SMS: call CFW_SmsDeleteMessage failed for delete all inbox message!!! nResult is:0x%x", nResult);
	
	}
	else if (pDelReq->del_flag == SMSAL_DEL_OUTBOX)
	{
		Uti = FREE_UTI;
		#ifdef __ADP_MULTI_SIM__
		#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		nResult = UT_CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNSENT|CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE|CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE, CFW_SMS_STORAGE_ME, CFW_SMS_TYPE_PP, Uti, adp_pSMSContext->nSimId);
		#else
		#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
		Simu_Rsp_Marker_Struct simuRspMarker;
		simuRspMarker.msgID = EV_CFW_SMS_DELETE_MESSAGE_RSP;
		simuRspMarker.nSimID = adp_pSMSContext->nSimId;
		simuRspMarker.Uti = Uti;
		simuRspMarker.pNext = NULL;
		adp_simu_add_Rsp_Marker(&simuRspMarker);
		#endif
		nResult = CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNSENT|CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE|CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE, CFW_SMS_STORAGE_ME, CFW_SMS_TYPE_PP, Uti, gCurrentSMSSimID);
		#endif
		#else
		nResult = CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNSENT|CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE|CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE, CFW_SMS_STORAGE_ME, Uti);
		#endif
		if (nResult == ERR_SUCCESS )
		{
			delete_outbox_sm = TRUE;
			delete_box_type = BOXTYPE_OUT;
			bALLFail = FALSE;
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: all outbox has been deleted!");
		}
		else
			mmi_trace(g_sw_ADP_SMS,"ADP_SMS: call CFW_SmsDeleteMessage failed for delete all outbox message!!! nResult is:0x%x", nResult);
	}
	else if (pDelReq->del_flag == SMSAL_DEL_BITMAP)
	{

		Uti = FREE_UTI;
		nIndex = mmi_frm_sms_get_index(pDelReq->index, pDelReq->del_bitmap);
		
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: pDelReq->index = %d, nIndex = %d", pDelReq->index, nIndex);
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: indexMap[nIndex].nPhyIndex = %d, indexMap[nIndex].bUsed = %d, indexMap[nIndex].nStorage=%d, FREE_UTI = %d",
					indexMap[nIndex].nPhyIndex, indexMap[nIndex].bUsed, indexMap[nIndex].nStorage, Uti);
		
		#ifdef __ADP_MULTI_SIM__
		#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		nResult = UT_CFW_SmsDeleteMessage(indexMap[nIndex].nPhyIndex, 0, indexMap[nIndex].nStorage,CFW_SMS_TYPE_PP, Uti, adp_pSMSContext->nSimId);
		#else
		#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
		Simu_Rsp_Marker_Struct simuRspMarker;
		simuRspMarker.msgID = EV_CFW_SMS_DELETE_MESSAGE_RSP;
		simuRspMarker.nSimID = adp_pSMSContext->nSimId;
		simuRspMarker.Uti = Uti;
		simuRspMarker.pNext = NULL;
		adp_simu_add_Rsp_Marker(&simuRspMarker);
		#endif
		nResult = CFW_SmsDeleteMessage(indexMap[nIndex].nPhyIndex, 0, indexMap[nIndex].nStorage,CFW_SMS_TYPE_PP, Uti, gCurrentSMSSimID);
		#endif
		#else
		nResult = CFW_SmsDeleteMessage(indexMap[nIndex].nPhyIndex, 0, indexMap[nIndex].nStorage, FREE_UTI);
		#endif
		
		if (ERR_SUCCESS == nResult)
		{
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The index = %d has been deleted!", nIndex);

			bALLFail = FALSE;
		}
		else
			mmi_trace(g_sw_ADP_SMS,"ADP_SMS: call CFW_SmsDeleteMessage failed for delete one message!!! nResult is:0x%x", nResult);
		
	}
	
	
	if (bALLFail)
	{
		#if 0
		mmi_sms_del_msg_rsp_struct *pDelRsp = NULL;
		pDelRsp = (mmi_sms_del_msg_rsp_struct*)OslMalloc(sizeof(mmi_sms_del_msg_rsp_struct));
		ASSERT(pDelRsp != NULL);
		memset(pDelRsp, 0, sizeof(mmi_sms_del_msg_rsp_struct));

		pDelRsp->result = FALSE;
		CswError2CSDError(&pDelRsp->cause, nResult);

		SendEventToMMI((local_para_struct*)pDelRsp, PRT_MSG_ID_MMI_SMS_DEL_MSG_RSP);
				
		mmi_trace(g_sw_ADP_SMS, ">>>> CFW_SmsDeleteMessage fail!! reason = %0x >>>>\n", nResult);
		#endif
		SendDeleteFailToMMI(nResult);
	}
}

/**************************************************************

	FUNCTION NAME		: DeleteMessageRsp

  	PURPOSE				: Deal "delete message" rsp
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void DeleteMessageRsp(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	UINT32	nResult;
	mmi_sms_del_msg_rsp_struct *pDelRsp;
	kal_uint16 storecount;
	U8 Uti;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: Entry DeleteMessageRsp");

	nResult = 0;
	//ASSERT(NULL != (ilm_struct*)nParam1); //nParam1 maybe null if delete req is not SMSAL_DEL_BITMAP
	pDelRsp = (mmi_sms_del_msg_rsp_struct*)OslMalloc(sizeof(mmi_sms_del_msg_rsp_struct));
	ASSERT(NULL != pDelRsp);		
	memset(pDelRsp, 0, sizeof(mmi_sms_del_msg_rsp_struct));

	//static UINT8 nBitMap[480] = {0};
	storecount = 0;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: nType = %d, ", nType);
	if (nType == 0)
	{
		pDelRsp->result  = TRUE;
		pDelRsp->cause = 0;
		
		CswStorage2CSDStorage(&pDelRsp->storage_type, LOUINT8(nParam2));

		if (adp_pSMSContext->bSIMFull && CFW_SMS_STORAGE_SM == LOUINT8(nParam2))
		{
			adp_pSMSContext->bSIMFull = FALSE;
		}

		if (adp_pSMSContext->bMEFull && CFW_SMS_STORAGE_ME == LOUINT8(nParam2))
		{
			adp_pSMSContext->bMEFull = FALSE;
		}

		SetCurrentStorage();

		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: After delete, the current storage = 0x%d LOUINT16(nParam1) = %d >>>>", 
			adp_pSMSContext->nCurrentStorage, LOUINT16(nParam1));
		
		if (LOUINT16(nParam1) != 0)
		{
			if(!GetLogicIndex(LOUINT16(nParam1), LOUINT8(nParam2), &pDelRsp->index))
			{
				OslMfree(pDelRsp);
				return;
			}

			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The phy index = %d, the logic index = %d", LOUINT16(nParam1), pDelRsp->index);
			
			indexMap[pDelRsp->index].bUsed = FALSE;
			pDelRsp->del_flag = SMSAL_DEL_INDEXED_MSG;

			if(pDelRsp->index != ADP_MMI_FRM_SMS_INVALID_INDEX)
			{
				//mmi_frm_sms_set_bitmap(pDelRsp->index, nBitMap);
			}
		}
		else 
		{
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: delete_inbox_sm = %d", delete_inbox_sm);
			if(delete_inbox_sm)
			{
				delete_inbox_sm = FALSE;
				Uti = FREE_UTI;
				#ifdef __ADP_MULTI_SIM__
				#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				nResult = UT_CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNREAD|CFW_SMS_STORED_STATUS_READ, CFW_SMS_STORAGE_SM,CFW_SMS_TYPE_PP, Uti, adp_pSMSContext->nSimId);
				#else
				#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
				Simu_Rsp_Marker_Struct simuRspMarker;
				simuRspMarker.msgID = EV_CFW_SMS_DELETE_MESSAGE_RSP;
				simuRspMarker.nSimID = adp_pSMSContext->nSimId;
				simuRspMarker.Uti = Uti;
				simuRspMarker.pNext = NULL;
				adp_simu_add_Rsp_Marker(&simuRspMarker);
				#endif
				nResult = CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNREAD|CFW_SMS_STORED_STATUS_READ, CFW_SMS_STORAGE_SM,CFW_SMS_TYPE_PP, Uti, gCurrentSMSSimID);
				#endif
				#else
				nResult = CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNREAD|CFW_SMS_STORED_STATUS_READ, CFW_SMS_STORAGE_SM, Uti);
				#endif
				if (nResult != ERR_SUCCESS)
				{
					SendDeleteFailToMMI(nResult);
				}
				OslMfree(pDelRsp);
				return;
			}
			else if(delete_outbox_sm)
			{
				delete_outbox_sm = FALSE;
				Uti = FREE_UTI;
				#ifdef __ADP_MULTI_SIM__
				#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				nResult = UT_CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNSENT|CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE|CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE, CFW_SMS_STORAGE_SM,CFW_SMS_TYPE_PP, Uti, adp_pSMSContext->nSimId);
				#else
				#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
				Simu_Rsp_Marker_Struct simuRspMarker;
				simuRspMarker.msgID = EV_CFW_SMS_DELETE_MESSAGE_RSP;
				simuRspMarker.nSimID = adp_pSMSContext->nSimId;
				simuRspMarker.Uti = Uti;
				simuRspMarker.pNext = NULL;
				adp_simu_add_Rsp_Marker(&simuRspMarker);
				#endif
				nResult = CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNSENT|CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE|CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE, CFW_SMS_STORAGE_SM,CFW_SMS_TYPE_PP, Uti, gCurrentSMSSimID);
				#endif
				#else
				nResult = CFW_SmsDeleteMessage(0, CFW_SMS_STORED_STATUS_UNSENT|CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV|CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE|CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE, CFW_SMS_STORAGE_SM, Uti);
				#endif
				if (nResult != ERR_SUCCESS)
				{
					SendDeleteFailToMMI(nResult);
				}
				OslMfree(pDelRsp);
				return;
			}
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: delete all Done.");
			pDelRsp->del_flag = SMSAL_DEL_INDEXED_MSG;
			#if 0
			for (storecount = 0; storecount < (PHONE_SMS_ENTRY_COUNT/ADP_SIM_NUMBER + SIM_SMS_ENTRY_COUNT); storecount++)
			{
				if (GetLogicIndex(LOUINT16(nParam1), LOUINT8(nParam2), &pDelRsp->index) == TRUE)
				{
					indexMap[pDelRsp->index].bUsed = FALSE;					
				}
				
			}
			#endif
			for(storecount = adp_pSMSContext->nStartIndexInMap;storecount<adp_pSMSContext->nStartIndexInMap+(PHONE_SMS_ENTRY_COUNT/ADP_SIM_NUMBER + SIM_SMS_ENTRY_COUNT);storecount++)
			{
				if(indexMap[storecount].nBoxType == delete_box_type)
				{
					indexMap[storecount].bUsed =FALSE;
				}
			}
			delete_box_type = 0;

			pDelRsp->index = 0xfff0; //wangwb @2007.06.16 close
		}	

	}
	else if (nType == 0xF0)
	{
		pDelRsp->result  = FALSE;
		CswError2CSDError(&pDelRsp->cause, nParam1);		
		delete_box_type = 0;
	}
	
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: delete rsp Send.");
	
	memcpy(pDelRsp->del_bitmap, adp_pSMSContext->adpDelBitmap, 480 * sizeof(UINT8));

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: pDelRsp->index = %d, result = %d", pDelRsp->index, pDelRsp->result);
	
	SendEventToMMI((local_para_struct*)pDelRsp, PRT_MSG_ID_MMI_SMS_DEL_MSG_RSP);

}

/**************************************************************

	FUNCTION NAME		: SetMessageReq

  	PURPOSE				: Deal "save message" req
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
#define __MMI_AUTO_CHANGE_STORAGE_FOR_LONG_SMS__
static void SetMessageReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	mmi_sms_set_msg_req_struct* pMMISmsReq;
	CFW_DIALNUMBER dialNumber;
	UINT8 destNumber[21];
	CFW_SMS_PARAMETER smsInfo;
	UINT16 nMessageLength;
	UINT8 *pData;
	UINT32 nResult = ERR_SUCCESS;
	UINT8 storage;
	mmi_sms_set_msg_rsp_struct *pMMISmsRsp = NULL;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: Enrty SetMessageReq");
	ASSERT(NULL != (ilm_struct*)nParam1);
	pMMISmsReq = (mmi_sms_set_msg_req_struct*)(((ilm_struct*)nParam1)->local_para_ptr);
	ASSERT(NULL != pMMISmsReq);

	memset(&dialNumber, 0, sizeof(CFW_DIALNUMBER));

	memset(destNumber, 0, 21);

	dialNumber.nDialNumberSize = 
		SUL_AsciiToGsmBcd(pMMISmsReq->dest.number, pMMISmsReq->dest.length, destNumber);

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: the dialnumber length = %d, pMMISmsReq number length = %d", dialNumber.nDialNumberSize, pMMISmsReq->dest.length);

	dialNumber.pDialNumber = destNumber;
	
	dialNumber.nType = pMMISmsReq->dest.type;

	adp_pSMSContext->nWriteMessageStatus = pMMISmsReq->status;

	memset(&smsInfo, 0, sizeof(CFW_SMS_PARAMETER));

 #ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_CfgGetSmsParam(&smsInfo, 0, adp_pSMSContext->nSimId);
#else
	CFW_CfgGetSmsParam(&smsInfo, 0, gCurrentSMSSimID);
#endif
#else
	CFW_CfgGetSmsParam(&smsInfo, 0);

#endif

	smsInfo.dcs = pMMISmsReq->dcs;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The set message req dcs = %0x >>>>", smsInfo.dcs);

	smsInfo.nIndex = 0;
	smsInfo.bDefault = TRUE;
	smsInfo.nSaveToSimFlag = 0;

 #ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	nResult = UT_CFW_CfgSetSmsParam(&smsInfo, FREE_UTI, adp_pSMSContext->nSimId);
#else
	nResult = CFW_CfgSetSmsParam(&smsInfo, FREE_UTI, gCurrentSMSSimID);
#endif
#else
	nResult = CFW_CfgSetSmsParam(&smsInfo, FREE_UTI);
#endif
	if(ERR_SUCCESS != nResult)
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_CfgSetSmsParam failed, nResult = 0x%x", nResult);
		
		pMMISmsRsp = OslMalloc(sizeof(mmi_sms_set_msg_rsp_struct));
		ASSERT(NULL != pMMISmsRsp);

		pMMISmsRsp->result = FALSE;
		CswError2CSDError(&pMMISmsRsp->cause, nResult);

		SendEventToMMI((local_para_struct*)pMMISmsRsp, PRT_MSG_ID_MMI_SMS_SET_MSG_RSP);
		return;
	}


	/*Guojian Add Start For 10629 Ver: GreenStone  on 2008-12-9 13:49 */
	 nMessageLength = 0;
	pData = CovertStringForSetSMS(pMMISmsReq->msg, pMMISmsReq->length, pMMISmsReq->dcs, &nMessageLength);
	if (pData != NULL)
	{
		memcpy(pMMISmsReq->msg, pData, pMMISmsReq->length);
	
		OslMfree(pData);
		pData = NULL;
	}
	/*Guojian Add End  For 10629 Ver: GreenStone  on 2008-12-9 13:49 */
#if 0
	if (0x08 == pMMISmsReq->dcs)
	{
		 nMessageLength = 0;
		UINT8 *pData = (UINT8*)CovertStringForPlatform(pMMISmsReq->msg, pMMISmsReq->length, pMMISmsReq->dcs, &nMessageLength);
	
		memcpy(pMMISmsReq->msg, pData, pMMISmsReq->length);
	
		OslMfree(pData);
		pData = NULL;
	}
#endif /* Guoj delete. It's not used now for bug 10629*/

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS:pMMISmsReq->length = %d ", pMMISmsReq->length);

	/*Wangzl Add Start For 6758 Ver: TBM780  on 2007-9-19 10:31 */
	 storage = adp_pSMSContext->nCurrentStorage;
	#ifdef __MMI_AUTO_CHANGE_STORAGE_FOR_LONG_SMS__
	 {
	CFW_SMS_STORAGE_INFO storageInfo;
	U8 nNeedSlot = 1;
	if(pMMISmsReq->dcs == SMSAL_DEFAULT_DCS && pMMISmsReq->length > CFW_SMS_LEN_7BIT_NOMAL)
	{
		nNeedSlot = pMMISmsReq->length/CFW_SMS_LEN_7BIT_LONG+1;
		if(pMMISmsReq->length%CFW_SMS_LEN_7BIT_LONG == 0)
			nNeedSlot--;
	}
	else if(pMMISmsReq->dcs == SMSAL_UCS2_DCS && pMMISmsReq->length > CFW_SMS_LEN_UCS2_NOMAL)
	{
		nNeedSlot = pMMISmsReq->length/CFW_SMS_LEN_UCS2_LONG+1;
		if(pMMISmsReq->length%CFW_SMS_LEN_UCS2_LONG == 0)
			nNeedSlot--;
	}

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	if (ERR_SUCCESS == UT_CFW_CfgGetSmsStorageInfo(&storageInfo, adp_pSMSContext->nCurrentStorage, adp_pSMSContext->nSimId))
#else
	if (ERR_SUCCESS == CFW_CfgGetSmsStorageInfo(&storageInfo, adp_pSMSContext->nCurrentStorage, gCurrentSMSSimID))
#endif
#else
	if (ERR_SUCCESS == CFW_CfgGetSmsStorageInfo(&storageInfo, adp_pSMSContext->nCurrentStorage))
#endif
	{
		if((storageInfo.totalSlot - storageInfo.usedSlot) < nNeedSlot)
		{
			storage = (adp_pSMSContext->nCurrentStorage == CFW_SMS_STORAGE_SM)?CFW_SMS_STORAGE_ME:CFW_SMS_STORAGE_SM;
		}
	}
	 }
	#endif
	/*Wangzl Add End  For 6758 Ver: TBM780  on 2007-9-19 10:31 */
	mmi_trace(g_sw_ADP_SMS,"ADP_SMS: SetMessageReq>> storage is:%d",storage);
	#ifdef __ADP_MULTI_SIM__
	#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
   {
	CFW_SMS_WRITE writeMessage;
	memcpy(&writeMessage.sNumber, &dialNumber, sizeof(CFW_DIALNUMBER));
	writeMessage.pData = pMMISmsReq->msg;
	writeMessage.nDataSize = pMMISmsReq->length;
	writeMessage.nStorage = storage;
	writeMessage.nType = CFW_SMS_TYPE_PP;
	writeMessage.nIndex = 0;
	writeMessage.nStatus = 1<<adp_pSMSContext->nWriteMessageStatus;
	nResult = UT_CFW_SmsWriteMessage(&writeMessage, FREE_UTI, adp_pSMSContext->nSimId);
   }
	#else /*ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
   {
	CFW_SMS_WRITE writeMessage;
	U8 Uti = FREE_UTI;
	memcpy(&writeMessage.sNumber, &dialNumber, sizeof(CFW_DIALNUMBER));
	writeMessage.pData = pMMISmsReq->msg;
	writeMessage.nDataSize = pMMISmsReq->length;
	writeMessage.nStorage = storage;
	writeMessage.nType = CFW_SMS_TYPE_PP;
	writeMessage.nIndex = 0;
	writeMessage.nStatus = 1<<adp_pSMSContext->nWriteMessageStatus;
	#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
	Simu_Rsp_Marker_Struct simuRspMarker;
	simuRspMarker.msgID = EV_CFW_SMS_WRITE_MESSAGE_RSP;
	simuRspMarker.nSimID = adp_pSMSContext->nSimId;
	simuRspMarker.Uti = Uti;
	simuRspMarker.pNext = NULL;
	adp_simu_add_Rsp_Marker(&simuRspMarker);
	#endif
	nResult = CFW_SmsWriteMessage(&writeMessage, Uti, gCurrentSMSSimID);
   }
	#endif
	#else /*__ADP_MULTI_SIM__*/
	nResult = CFW_SmsWriteMessage(&dialNumber, 
								     pMMISmsReq->msg, 
								     pMMISmsReq->length, 
								     storage, /*adp_pSMSContext->nCurrentStorage*/
								     0, 
								     1 << adp_pSMSContext->nWriteMessageStatus, 
								     FREE_UTI);
	#endif

	
	if (ERR_SUCCESS != nResult)
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: call CFW_SmsWriteMessage FAILED!!!nResult is: 0x%x",nResult);
		
		pMMISmsRsp = OslMalloc(sizeof(mmi_sms_set_msg_rsp_struct));
		ASSERT(NULL != pMMISmsRsp);

		pMMISmsRsp->result = FALSE;
		CswError2CSDError(&pMMISmsRsp->cause, nResult);

		SendEventToMMI((local_para_struct*)pMMISmsRsp, PRT_MSG_ID_MMI_SMS_SET_MSG_RSP);
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS:  call CFW_SmsWriteMessage OK!");
	}
}

/**************************************************************

	FUNCTION NAME		: WriteMessageRsp

  	PURPOSE				: Deal "save message" rsp
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void WriteMessageRsp(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	mmi_sms_set_msg_rsp_struct *pMMISmsRsp = NULL;
	UINT16 seginfo;
	UINT16 nFreeIndex;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: Enrty WriteMessageRsp");
	
	pMMISmsRsp = OslMalloc(sizeof(mmi_sms_set_msg_rsp_struct));
	ASSERT(NULL != pMMISmsRsp);
	memset(pMMISmsRsp, 0, sizeof(mmi_sms_set_msg_rsp_struct));

	
	if (0 == nType)
	{
		pMMISmsRsp->result = TRUE;

		pMMISmsRsp->cause = 0;

		seginfo = (UINT16)(nParam2 & 0x0000ffff);
		if (seginfo == 0)
		{
			pMMISmsRsp->concat_info.seg = 1;
			pMMISmsRsp->concat_info.total_seg = 1;
			pMMISmsRsp->concat_info.ref = 1;
		}
		else
		{
			pMMISmsRsp->concat_info.seg = (UINT8)(seginfo & 0x00f0) >> 4;
			pMMISmsRsp->concat_info.total_seg = (UINT8)(seginfo & 0x000f);
			pMMISmsRsp->concat_info.ref = (seginfo & 0xff00) >> 8;
		}      //wangwb add @2007.06.22
		mmi_trace(g_sw_ADP_SMS,"ADP_SMS: Func: %s pMMISmsRsp->concat_info.seg=%d,pMMISmsRsp->concat_info.total_seg=%d,pMMISmsRsp->concat_info.ref=%d", __FUNCTION__,pMMISmsRsp->concat_info.seg,pMMISmsRsp->concat_info.total_seg,pMMISmsRsp->concat_info.ref);
	
		nFreeIndex = 0;
		if (GetFreeIndex(&nFreeIndex))
		{
			pMMISmsRsp->index = nFreeIndex;
				
			indexMap[nFreeIndex].nPhyIndex = LOUINT16(nParam1);
			indexMap[nFreeIndex].nStorage = HIUINT16(nParam1);
			indexMap[nFreeIndex].bUsed = TRUE;
			indexMap[nFreeIndex].nBoxType = BOXTYPE_OUT;

			++adp_pSMSContext->nIndexCount;

			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: WriteMessageRsp index = %d, nPhyIndex is = %d, nStorage = %d", nFreeIndex, LOUINT16(nParam1), HIUINT16(nParam1));
			
			CswStorage2CSDStorage(&pMMISmsRsp->storage_type, HIUINT16(nParam1));
			
			pMMISmsRsp->status = adp_pSMSContext->nWriteMessageStatus;
		}
		else
		{
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS:  Can't get free index");
		}

		if (1 == HIUINT16(nParam2)) 
		{
			adp_pSMSContext->nCurrentStorage == CFW_SMS_STORAGE_SM ? (adp_pSMSContext->bSIMFull = TRUE) : (adp_pSMSContext->bMEFull = TRUE);
				
			SetCurrentStorage();

			if (adp_pSMSContext->bSIMFull && adp_pSMSContext->bMEFull)
			{
				mmi_sms_mem_full_ind_struct *pMemFull = OslMalloc(sizeof(mmi_sms_mem_full_ind_struct));
				ASSERT(NULL != pMemFull);
				memset(pMemFull, 0, sizeof(mmi_sms_mem_full_ind_struct));

				pMemFull->type = SMSAL_MEM_FULL_NORMAL;
				SendEventToMMI((local_para_struct*)pMemFull, PRT_MSG_ID_MMI_SMS_MEM_FULL_IND);
			}
			
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: HIUINT16(nParam2) = 1, The storage is FULL!");
		}
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: write message failed!!! ntype is:%d", nType);
		pMMISmsRsp->result = FALSE;
		CswError2CSDError(&pMMISmsRsp->cause, nParam1);
	}
	
	SendEventToMMI((local_para_struct*)pMMISmsRsp, PRT_MSG_ID_MMI_SMS_SET_MSG_RSP);
}

/**************************************************************

	FUNCTION NAME		: ListDeliveredTextHeadInfo

  	PURPOSE				: List "deliverd text" type message
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void ListDeliveredTextHeadInfo(CFW_SMS_NODE *pSmsNode)
{
	CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO *pDelivered;
	mmi_frm_sms_concat_struct concat; 
	mmi_sms_startup_read_msg_ind_struct *pMMISmsNode;

	UINT16 uDataLength;
	UINT8 uStatus;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry DealListDeliveredTextHeadInfo");
	ASSERT(NULL != pSmsNode);
	
	pDelivered = (CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO *)pSmsNode->pNode;
	ASSERT(NULL != pDelivered);


	pMMISmsNode = NULL;

	uDataLength = (pDelivered->length == 0 ? 1 : pDelivered->length) - 1;
	pMMISmsNode = OslMalloc(sizeof(mmi_sms_get_msg_rsp_struct) + uDataLength);
	ASSERT(NULL != pMMISmsNode); 
	TBM_ENTRY(0x29ea);
	//forget to initialize the pMMISmsNode
	memset(pMMISmsNode, 0, sizeof(mmi_sms_get_msg_rsp_struct) + uDataLength);

	#if 0
	pMMISmsNode->addr_number.length = SUL_GsmBcdToAscii(pDelivered->oa, pDelivered->oa_size, pMMISmsNode->addr_number.number);
	//mmi_trace(g_sw_ADP_SMS, ">>>> pMMISmsNode->addr_number.length = %d \n", pMMISmsNode->addr_number.length);
	
	//if (pMMISmsNode->addr_number.length != 0)                                     //add by yaosq 20061129 for deleted the sms which haven't number 
	//{
	pMMISmsNode->addr_number.type = pDelivered->tooa;
	#endif

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: pDelivered->tooa is:%d, size is:%d",pDelivered->tooa, pDelivered->oa_size);
	switch(pDelivered->tooa)
	{
		case CFW_TELNUMBER_TYPE_INTERNATIONAL:
		case CFW_TELNUMBER_TYPE_NATIONAL:
			pMMISmsNode->addr_number.length = SUL_GsmBcdToAscii(pDelivered->oa, pDelivered->oa_size, pMMISmsNode->addr_number.number);
			break;
		case 0xd0:
			pMMISmsNode->addr_number.length = SUL_Decode7Bit(pDelivered->oa, pMMISmsNode->addr_number.number, pDelivered->oa_size);
			break;
		default:
			pMMISmsNode->addr_number.length = SUL_GsmBcdToAscii(pDelivered->oa, pDelivered->oa_size, pMMISmsNode->addr_number.number);
			break;
			
	}
	pMMISmsNode->addr_number.type = pDelivered->tooa;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: oa_number The length = %d, number is: %s, type is:%d", 
								pMMISmsNode->addr_number.length, pMMISmsNode->addr_number.number,pMMISmsNode->addr_number.type);
	
	CswDcs2CSDDcs(&pMMISmsNode->dcs, pDelivered->dcs);
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: list message the delivered dcs = %0x, mmi dcs = %d", pDelivered->dcs, pMMISmsNode->dcs);

	//don't surpport
	memset(pMMISmsNode->dt, 0, sizeof(pMMISmsNode->dt));
	pMMISmsNode->mr = 0;
	pMMISmsNode->ori_dcs = 0;
	pMMISmsNode->vp = 0;
	//end
	
	pMMISmsNode->fo = pDelivered->fo;

	
	indexMap[adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount].nPhyIndex = pSmsNode->nConcatCurrentIndex;
	indexMap[adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount].nStorage = pSmsNode->nStorage;
	indexMap[adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount].bUsed = TRUE;
	indexMap[adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount].nBoxType = BOXTYPE_IN;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: indexMap[%d].nPhyIndex = %d , nStorage is = %d", adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount, pSmsNode->nConcatCurrentIndex,pSmsNode->nStorage);
	
	pMMISmsNode->index = adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount;

	++adp_pSMSContext->nIndexCount;
	
	//mmi_trace(g_sw_ADP_SMS, "The index is %d, the storage = 0x%0x \n", pMMISmsNode->index, pSmsNode->nStorage);
	
	pMMISmsNode->mti = SMSAL_MTI_DELIVER;
	pMMISmsNode->pid = pDelivered->pid;
	Cswtime2CSDtime(pMMISmsNode->scts, &pDelivered->scts);
	pMMISmsNode->st = ST_COMP_MSG_RECV_BY_SME;
	
	//pSmsNode->nConcatCurrentIndex < pSmsNode->nConcatNextIndex ? 
		//(pMMISmsNode->startup_seq_num = 0) : (pMMISmsNode->startup_seq_num = 1);       //wangwb @2007.06.16
	
	concat.seg = (pSmsNode->nConcatNextIndex & 0xff00)>>8;
	concat.total_seg = (pSmsNode->nConcatNextIndex & 0x00ff);

	//concat.seg  < concat.total_seg ? (pMMISmsNode->startup_seq_num = 0) : (pMMISmsNode->startup_seq_num = 1);    //wangwb @2007.06.16
	if(adp_pSMSContext->SMSListState == LIST_STATE_ME_START)
	{
		pMMISmsNode->startup_seq_num = (adp_pSMSContext->nListIndex < adp_pSMSContext->nSMSInME)? 0:1;
	}
	else if(adp_pSMSContext->SMSListState == LIST_STATE_SM_START)
	{
		pMMISmsNode->startup_seq_num = (adp_pSMSContext->nListIndex< adp_pSMSContext->nSMSInSM)?0:1;
	}
		

		
	//pMMISmsNode->concat_info = 0x01010001; //wangwb @2007.06.16      define the long message for mmi
	
	if (pSmsNode->nConcatPrevIndex == 0xffff)
		pMMISmsNode->concat_info = 0x01010001;
	else
	{
		pMMISmsNode->concat_info |= ((kal_uint32)pSmsNode->nConcatNextIndex & 0x0000ffff);
		pMMISmsNode->concat_info = pMMISmsNode->concat_info << 16;
		pMMISmsNode->concat_info |= ((kal_uint32)pSmsNode->nConcatPrevIndex & 0x0000ffff);//wangwb @2007.06.16
	}
	
	#if 0
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pDelivered->scts.uYear) = %x",pDelivered->scts.uYear);
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pDelivered->scts.uMonth = %x",pDelivered->scts.uMonth);
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pDelivered->scts.uDay = %x",pDelivered->scts.uDay);
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pDelivered->scts.uHour = %x",pDelivered->scts.uHour);
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pDelivered->scts.uMinute = %x",pDelivered->scts.uMinute);
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pDelivered->scts.uSecond = %x",pDelivered->scts.uSecond);
	
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pMMISmsNode->concat_info = %x",pMMISmsNode->concat_info);
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pSmsNode->nConcatNextIndex = %x",pSmsNode->nConcatNextIndex);
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pSmsNode->nConcatPrevIndex = %x",pSmsNode->nConcatPrevIndex);
	#endif


	uStatus = 0;
	for(uStatus = 0; uStatus != SMSAL_INVALID_STATUS; ++uStatus)
	{
		if ((1 << uStatus) == pSmsNode->nStatus)
		{
			break;
		}
	}
	pMMISmsNode->status = uStatus;
	
	CswStorage2CSDStorage(&pMMISmsNode->storage_type, pSmsNode->nStorage);

	memcpy(pMMISmsNode->msg_data, pDelivered->data, pDelivered->length);
	pMMISmsNode->no_msg_data = pDelivered->length;

	//SendEventToMMI((local_para_struct *)pMMISmsNode, PRT_MSG_ID_MMI_SMS_STARTUP_READ_MSG_IND);
	SendEventToMMI((local_para_struct *)pMMISmsNode, adp_pSMSContext->nStartupReadMsgId);

	TBM_EXIT(0x29ea);
}

/**************************************************************

	FUNCTION NAME		: ListSubmitedTextHeadInfo

  	PURPOSE				: List "submit text" type message
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void ListSubmitedTextHeadInfo(CFW_SMS_NODE *pSmsNode)
{	
	CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO *pSubmitNode;
	mmi_frm_sms_concat_struct concat;//wangwb @2007.06.16
	mmi_sms_startup_read_msg_ind_struct *pMMISmsNode;
	UINT16 nDataLen;
	UINT8 nStatus;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry DealListSubmitedTextHeadInfo");

	ASSERT(NULL != pSmsNode);

	pSubmitNode = (CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO*)pSmsNode->pNode;
	ASSERT(NULL != pSubmitNode);


	pMMISmsNode = NULL;

	nDataLen = (pSubmitNode->length == 0 ? 1 : pSubmitNode->length) - 1;

	//allocate the space and initialize the space allocated
	pMMISmsNode = OslMalloc(sizeof(mmi_sms_get_msg_rsp_struct) + nDataLen);
	ASSERT(NULL != pMMISmsNode);
	memset(pMMISmsNode, 0, sizeof(mmi_sms_get_msg_rsp_struct) + nDataLen);

	#if 0
	pMMISmsNode->addr_number.length = 
		SUL_GsmBcdToAscii(pSubmitNode->da, pSubmitNode->da_size, pMMISmsNode->addr_number.number);

	//mmi_trace(g_sw_ADP_SMS, ">>>> The pMMISmsNode = %d, pSubmitNode da_size = %d>>>> \n", 
		//pMMISmsNode->addr_number.length, pSubmitNode->da_size);
	
	pMMISmsNode->addr_number.type = pSubmitNode->toda;
	#endif
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: pSubmitNode->toda is:%d, length is:%d",pSubmitNode->toda, pSubmitNode->da_size);
	switch(pSubmitNode->toda)
	{
		case CFW_TELNUMBER_TYPE_INTERNATIONAL:
		case CFW_TELNUMBER_TYPE_NATIONAL:
			pMMISmsNode->addr_number.length = SUL_GsmBcdToAscii(pSubmitNode->da, pSubmitNode->da_size, pMMISmsNode->addr_number.number);
			break;
		case 0xd0:
			pMMISmsNode->addr_number.length = SUL_Decode7Bit(pSubmitNode->da, pMMISmsNode->addr_number.number, pSubmitNode->da_size);
			break;
		default:
			pMMISmsNode->addr_number.length = SUL_GsmBcdToAscii(pSubmitNode->da, pSubmitNode->da_size, pMMISmsNode->addr_number.number);
			break;
			
	}
	pMMISmsNode->addr_number.type = pSubmitNode->toda;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: addr_number The length = %d, number is: %s, type is:%d", 
								pMMISmsNode->addr_number.length, pMMISmsNode->addr_number.number,pMMISmsNode->addr_number.type);
	
	//pMMISmsNode->concat_info, csw can't surport this attribute

	CswDcs2CSDDcs(&pMMISmsNode->dcs, pSubmitNode->dcs);
	//mmi_trace(g_sw_ADP_SMS, ">>>> list submit dcs = 0x%0x, mmi dcs = %d >>>>\n", pSubmitNode->dcs, pMMISmsNode->dcs);
	
	memset(pMMISmsNode->dt, 0, sizeof(pMMISmsNode->dt));

	pMMISmsNode->fo = pSubmitNode->fo;

	indexMap[adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount].nPhyIndex = pSmsNode->nConcatCurrentIndex;
	indexMap[adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount].nStorage = pSmsNode->nStorage;
	indexMap[adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount].bUsed = TRUE;
	indexMap[adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount].nBoxType = BOXTYPE_OUT;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: indexMap[%d].nPhyIndex = %d , nStorage is = %d", adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount, pSmsNode->nConcatCurrentIndex,pSmsNode->nStorage);
	pMMISmsNode->index = adp_pSMSContext->nStartIndexInMap+adp_pSMSContext->nIndexCount;

	++adp_pSMSContext->nIndexCount;

	pMMISmsNode->mr = 0;

	memcpy(pMMISmsNode->msg_data, pSubmitNode->data, pSubmitNode->length);
	pMMISmsNode->no_msg_data = pSubmitNode->length;

	pMMISmsNode->mti = SMSAL_MTI_SUBMIT;
	pMMISmsNode->ori_dcs = 0;

	pMMISmsNode->pid = pSubmitNode->pid;
	
	pMMISmsNode->st = 0;

	pMMISmsNode->vp = pSubmitNode->vp;

	//pMMISmsNode->storage_type = pSmsNode->nStorage;
	CswStorage2CSDStorage(&pMMISmsNode->storage_type, pSmsNode->nStorage);

	nStatus = 0;

	concat.seg = (pSmsNode->nConcatNextIndex & 0xff00)>>8;
	concat.total_seg = (pSmsNode->nConcatNextIndex & 0x00ff);

	//concat.seg  < concat.total_seg ? (pMMISmsNode->startup_seq_num = 0) : (pMMISmsNode->startup_seq_num = 1);    //wangwb @2007.06.16
	if(adp_pSMSContext->SMSListState == LIST_STATE_ME_START)
	{
		pMMISmsNode->startup_seq_num = (adp_pSMSContext->nListIndex < adp_pSMSContext->nSMSInME)?0:1;
	}
	else if(adp_pSMSContext->SMSListState == LIST_STATE_SM_START)
	{
		pMMISmsNode->startup_seq_num = (adp_pSMSContext->nListIndex < adp_pSMSContext->nSMSInSM)?0:1;
	}

	
	//pMMISmsNode->concat_info = 0x01010001;	//wangwb @2007.06.16				//add by yaosq 20070124 for show the picture of OutBox

	if (pSmsNode->nConcatPrevIndex == 0xffff)
		pMMISmsNode->concat_info = 0x01010001;
	else
	{
		pMMISmsNode->concat_info |= ((kal_uint32)pSmsNode->nConcatNextIndex & 0x0000ffff);
		pMMISmsNode->concat_info = pMMISmsNode->concat_info << 16;
		pMMISmsNode->concat_info |= ((kal_uint32)pSmsNode->nConcatPrevIndex & 0x0000ffff);//wangwb @2007.06.16
	}

	#if 0
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pMMISmsNode->concat_info = %x",pMMISmsNode->concat_info);
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pSmsNode->nConcatNextIndex = %x",pSmsNode->nConcatNextIndex);
	mmi_trace(g_sw_ADP_SMS, "HHHHHHHHHHHHHHHHHH              pSmsNode->nConcatPrevIndex = %x",pSmsNode->nConcatPrevIndex);
	#endif
	
	for(nStatus = 0; nStatus != SMSAL_INVALID_STATUS; ++nStatus)
	{
		if ((1 << nStatus) == pSmsNode->nStatus)
		{
			break;
		}
	}
	pMMISmsNode->status = nStatus;

	//SendEventToMMI((local_para_struct*)pMMISmsNode, PRT_MSG_ID_MMI_SMS_STARTUP_READ_MSG_IND);
	SendEventToMMI((local_para_struct *)pMMISmsNode, adp_pSMSContext->nStartupReadMsgId);
}

/**************************************************************

	FUNCTION NAME		: DealNewMsgTxtHRDV1

  	PURPOSE				: deal "new message"
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
extern BOOL mmi_msg_is_black_number(U8* number);
static void DealNewMsgTxtHRDV1(CFW_NEW_SMS_NODE *pSmsNode,UINT32 seginfo)
{
	CFW_SMS_TXT_HRD_V1_IND *pTxtHrdNode;
	mmi_sms_deliver_msg_ind_struct *pDeliverNode;
	UINT16 nDataLen;
	UINT16 nFreeIndex;
	UINT8 *pData;
	CFW_EMS_INFO *pMsgInfo = (CFW_EMS_INFO *)seginfo;

	#if defined(__MMI_MESSAGES_BLACK_LIST__)
	U8 number_buffer[MAX_CC_ADDR_LEN*2];
	#endif

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry DealNewMsgTxtHRDV1");

	
	ASSERT(NULL != pSmsNode);

	pTxtHrdNode = (CFW_SMS_TXT_HRD_V1_IND*)pSmsNode->pNode;

	ASSERT(NULL != pTxtHrdNode);

	pDeliverNode = NULL;

	nDataLen = (pTxtHrdNode->nDataLen == 0 ? 1 : pTxtHrdNode->nDataLen) - 1;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: the len of the data = %d, before convert data len = %d", pTxtHrdNode->nDataLen, nDataLen);

	pDeliverNode = OslMalloc(sizeof(mmi_sms_deliver_msg_ind_struct) + nDataLen);
	memset(pDeliverNode, 0, sizeof(mmi_sms_deliver_msg_ind_struct) + nDataLen);

	pDeliverNode->fo = pTxtHrdNode->fo;

	Cswtime2CSDtime(pDeliverNode->scts, &pTxtHrdNode->scts);

	pDeliverNode->sca_number.length = SUL_GsmBcdToAscii(pTxtHrdNode->sca, pTxtHrdNode->sca_size, pDeliverNode->sca_number.number);
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: sca_number The length = %d", pDeliverNode->sca_number.length);
	pDeliverNode->sca_number.type = pTxtHrdNode->tosca;

	#if 0
	pDeliverNode->oa_number.length = SUL_GsmBcdToAscii(pTxtHrdNode->oa, pTxtHrdNode->oa_size, pDeliverNode->oa_number.number);
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: oa_number The length = %d", pDeliverNode->oa_number.length);
	pDeliverNode->oa_number.type = pTxtHrdNode->tooa;
	#endif

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: pTxtHrdNode->tooa is:%d, size is:%d",pTxtHrdNode->tooa, pTxtHrdNode->oa_size);
	switch(pTxtHrdNode->tooa)
	{
		case CFW_TELNUMBER_TYPE_INTERNATIONAL:
		case CFW_TELNUMBER_TYPE_NATIONAL:
			pDeliverNode->oa_number.length = SUL_GsmBcdToAscii(pTxtHrdNode->oa, pTxtHrdNode->oa_size, pDeliverNode->oa_number.number);
			break;
		case 0xd0:
			pDeliverNode->oa_number.length = SUL_Decode7Bit(pTxtHrdNode->oa, pDeliverNode->oa_number.number, pTxtHrdNode->oa_size);
			break;
		default:
			pDeliverNode->oa_number.length = SUL_GsmBcdToAscii(pTxtHrdNode->oa, pTxtHrdNode->oa_size, pDeliverNode->oa_number.number);
			break;
	}
	pDeliverNode->oa_number.type = pTxtHrdNode->tooa;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: oa_number The length = %d, number is: %s, type is:%d", 
								pDeliverNode->oa_number.length, pDeliverNode->oa_number.number,pDeliverNode->oa_number.type);

	#if defined __MMI_MESSAGES_BLACK_LIST__
	memset(number_buffer, 0, sizeof(number_buffer));
	AnsiiToUnicodeString(number_buffer, pDeliverNode->oa_number.number);
	if(mmi_msg_is_black_number(number_buffer))
	{
		#ifdef __ADP_MULTI_SIM__
		CFW_SmsDeleteMessage(pSmsNode->nConcatCurrentIndex,CFW_SMS_STORED_STATUS_STORED_ALL,pSmsNode->nStorage,CFW_SMS_TYPE_PP,FREE_UTI,adp_pSMSContext->nSimId);
		#else
		CFW_SmsDeleteMessage(pSmsNode->nConcatCurrentIndex,CFW_SMS_STORED_STATUS_STORED_ALL,pSmsNode->nStorage,FREE_UTI);
		#endif
		OslMfree(pDeliverNode);
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: number in blacklist, delete it directly");
		return;
	}
	#endif

	pDeliverNode->pid = pTxtHrdNode->pid;

	pDeliverNode->dcs = (pTxtHrdNode->dcs == 0 ? 0 : 0x08);

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: pSmsNode->nConcatPrevIndex  = %x",pSmsNode->nConcatPrevIndex );

	
	if (pSmsNode->nConcatPrevIndex == 0xffff)
		pDeliverNode->concat_info = 0;
	else
	{
		#if 0
		pDeliverNode->concat_info |= ( (seginfo << 8) & 0xff000000); 
		pDeliverNode->concat_info |= ( (seginfo << 16) & 0xffff0000);
		pDeliverNode->concat_info |= ((kal_uint32)pSmsNode->nConcatPrevIndex & 0x0000ffff);//wangwb @2007.06.16
		#else
       	ASSERT(NULL != pMsgInfo);
		pDeliverNode->concat_info |= (((U32)pMsgInfo->nCurrentNum<<24)&0xff000000);
		pDeliverNode->concat_info |=(((U32)pMsgInfo->nMaxNum<<16)&0x00ff0000);
		pDeliverNode->concat_info |=((U32)pMsgInfo->nRerNum&0x0000ffff);
        mmi_trace(g_sw_ADP_SMS,"pMsgInfo->current seg is:0x%x, total seg is:0x%x, ref is:0x%x", pMsgInfo->nCurrentNum,pMsgInfo->nMaxNum,pMsgInfo->nRerNum);
		#endif
	}
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS:  pDeliverNode->concat_info = %x",pDeliverNode->concat_info);

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS:  the new message dcs = 0x%0x >>>>\n", pTxtHrdNode->dcs);

	nFreeIndex = 0;
	if (GetFreeIndex(&nFreeIndex))
	{
		pDeliverNode->index = nFreeIndex;
	
		indexMap[nFreeIndex].nPhyIndex = pSmsNode->nConcatCurrentIndex;
		indexMap[nFreeIndex].bUsed = TRUE;
		indexMap[nFreeIndex].nStorage = pSmsNode->nStorage;//adp_pSMSContext->nCurrentStorage;
		indexMap[nFreeIndex].nBoxType = BOXTYPE_IN;

		mmi_trace(g_sw_ADP_SMS, "ADP_SMS:  New Message coming, the storage is 0x%0x, the index = %d", 
							pSmsNode->nStorage, pDeliverNode->index);

		++adp_pSMSContext->nIndexCount;

		SetMemFullStatus();
		SetCurrentStorage();
		
		if (adp_pSMSContext->bSIMFull && adp_pSMSContext->bMEFull)
		{
			mmi_sms_mem_full_ind_struct *pMemFull = OslMalloc(sizeof(mmi_sms_mem_full_ind_struct));
			ASSERT(NULL != pMemFull);
			memset(pMemFull, 0, sizeof(mmi_sms_mem_full_ind_struct));

			pMemFull->type = SMSAL_MEM_FULL_NORMAL;
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: DealNewMsgTxtHRDV1, send memory full ind to mmi");
			SendEventToMMI((local_para_struct*)pMemFull, PRT_MSG_ID_MMI_SMS_MEM_FULL_IND);
		}

		pDeliverNode->dest_port = 0;
		pDeliverNode->src_port = 0;

		pDeliverNode->mti = SMSAL_MTI_DELIVER;

		pDeliverNode->display_type = SMSAL_MSG_TYPE_NORMAL;

		CswStorage2CSDStorage(&pDeliverNode->storage_type, pSmsNode->nStorage);

		//pDeliverNode->concat_info = 0;      //wangwb @2007.06.16

		/*Guojian Add Start For 10629 Ver: GreenStone  on 2008-12-9 13:45 */
		pData = (UINT8*)CovertStringForGetSMS(pTxtHrdNode->pData, pTxtHrdNode->nDataLen, pDeliverNode->dcs, &(pDeliverNode->no_msg_data));
		if(pData != NULL)
		{
			memcpy(pDeliverNode->msg_data, pData, pDeliverNode->no_msg_data);

			OslMfree(pData);
			pData = NULL;
		}
		/*Guojian Add End  For 10629 Ver: GreenStone  on 2008-12-9 13:45 */
	

#if SMS_TEST_SWITCH
		int i;
		mmi_trace(g_sw_ADP_SMS,"ADP_SMS:  Func: %s pDeliverNode->no_msg_data=%d", __FUNCTION__,pDeliverNode->no_msg_data);
		for (i=0; i<pTxtHrdNode->nDataLen; i++)
		{
			mmi_trace(g_sw_ADP_SMS,"ADP_SMS:  Func: %s pDeliverNode->msg_data[%d]:0x%x", __FUNCTION__,i,pDeliverNode->msg_data[i]);	
		}
#endif
		SendEventToMMI((local_para_struct *)pDeliverNode, PRT_MSG_ID_MMI_SMS_DELIVER_MSG_IND);
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: Can't GetFreeIndex!!!");
	}

}

/**************************************************************

	FUNCTION NAME		: DealNewMsgTxtHRDV1

  	PURPOSE				: deal "status report"
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void DealStatusReport(CFW_NEW_SMS_NODE *pSmsNode)
{
	CFW_SMS_TXT_STATUS_IND  *pTxtStatusNode;	
	mmi_sms_status_report_ind_struct *pMMINode; 

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS:  entry DealStatusReport");
		
	ASSERT(NULL != pSmsNode);
	
	pTxtStatusNode = (CFW_SMS_TXT_STATUS_IND  *)pSmsNode->pNode;	
	ASSERT(NULL != pTxtStatusNode);
	
	pMMINode = 
		(mmi_sms_status_report_ind_struct *)OslMalloc(sizeof(mmi_sms_status_report_ind_struct));
	ASSERT(NULL != pMMINode);
	memset(pMMINode, 0, sizeof(mmi_sms_status_report_ind_struct));
	
	pMMINode->fo = pTxtStatusNode->fo;
	Cswtime2CSDtime(pMMINode->scts, &pTxtStatusNode->scts);
	Cswtime2CSDtime(pMMINode->dt, &pTxtStatusNode->dt);

	#if 0
	pMMINode->ra_number.type =  pTxtStatusNode->tora;
	pMMINode->ra_number.length = SUL_GsmBcdToAscii(pTxtStatusNode->ra, pTxtStatusNode->ra_size, pMMINode->ra_number.number);
	#endif
	
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: pTxtStatusNode->tora is:%d, size is:%d",pTxtStatusNode->tora, pTxtStatusNode->ra_size);
	switch(pTxtStatusNode->tora)
	{
		case CFW_TELNUMBER_TYPE_INTERNATIONAL:
		case CFW_TELNUMBER_TYPE_NATIONAL:
			pMMINode->ra_number.length = SUL_GsmBcdToAscii(pTxtStatusNode->ra, pTxtStatusNode->ra_size, pMMINode->ra_number.number);
			break;
		case 0xd0:
			pMMINode->ra_number.length = SUL_Decode7Bit(pTxtStatusNode->ra, pMMINode->ra_number.number, pTxtStatusNode->ra_size);
			break;
		default:
			pMMINode->ra_number.length = SUL_GsmBcdToAscii(pTxtStatusNode->ra, pTxtStatusNode->ra_size, pMMINode->ra_number.number);
			break;
	}
	pMMINode->ra_number.type = pTxtStatusNode->tora;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: ra_number The length = %d, number is: %s, type is:%d", 
								pMMINode->ra_number.length, pMMINode->ra_number.number,pMMINode->ra_number.type);
	pMMINode->st = pTxtStatusNode->st;
	pMMINode->mr = pTxtStatusNode->mr;

	mmi_trace(g_sw_ADP_SMS, "ADP_SMS:  the ra_number length = %d",pMMINode->ra_number.length);       

	/*CSW  暂不提供参数，所以赋值为0*/
	pMMINode->pid = 0;
	pMMINode->dcs = 0;
	pMMINode->no_msg_data = 0;
	pMMINode->msg_data[0] = 0;
	/*CSW  暂不提供参数，所以赋值为0*/

	SendEventToMMI((local_para_struct*)pMMINode, PRT_MSG_ID_MMI_SMS_STATUS_REPORT_IND);             
}

/**************************************************************

	FUNCTION NAME		: ListNextMessageREQ

  	PURPOSE				: list next message
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
void ListNextMessageREQ(void)
{
	mmi_trace(g_sw_ADP_SMS,"ADP_SMS: ListNextMessageREQ");

	if(adp_pSMSContext->SMSListState == LIST_STATE_ME_START)
	{
		UINT32 nResult = 0;
		#ifdef __ADP_MULTI_SIM__
		#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		CFW_SMS_LIST ListInfo;
		ListInfo.nCount = LIST_COUNT_ME;
		ListInfo.nStartIndex = adp_pSMSContext->nListIndex+1;
		ListInfo.nStorage = CFW_SMS_STORAGE_ME;
		ListInfo.nType = CFW_SMS_TYPE_PP;
		ListInfo.nStatus = CFW_SMS_STORED_STATUS_STORED_ALL;
		ListInfo.nOption = CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER;
		nResult = UT_CFW_SmsListMessages(&ListInfo,
									   FREE_UTI,
									   adp_pSMSContext->nSimId);
		#else/*no defined ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
		CFW_SMS_LIST ListInfo;
		U8 Uti = FREE_UTI;
		ListInfo.nCount = LIST_COUNT_ME;
		ListInfo.nStartIndex = adp_pSMSContext->nListIndex+1;
		ListInfo.nStorage = CFW_SMS_STORAGE_ME;
		ListInfo.nType = CFW_SMS_TYPE_PP;
		ListInfo.nStatus = CFW_SMS_STORED_STATUS_STORED_ALL;
		ListInfo.nOption = CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER;
		
		#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
		Simu_Rsp_Marker_Struct simuRspMarker;
		simuRspMarker.msgID = EV_CFW_SMS_LIST_MESSAGE_RSP;
		simuRspMarker.nSimID = adp_pSMSContext->nSimId;
		simuRspMarker.Uti = Uti;
		simuRspMarker.pNext = NULL;
		adp_simu_add_Rsp_Marker(&simuRspMarker);
		#endif
		
		nResult = CFW_SmsListMessages(&ListInfo, 
										Uti, 
										gCurrentSMSSimID);
		#endif /*ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
		#else /*no defined __ADP_MULTI_SIM__*/
		nResult = CFW_SmsListMessages(CFW_SMS_STORAGE_ME/*storage1*/, 
									    CFW_SMS_STORED_STATUS_STORED_ALL, 
									    LIST_COUNT_ME, 
									    adp_pSMSContext->nListIndex+1, 
									    CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER,
									   FREE_UTI);
		#endif /*__ADP_MULTI_SIM__*/

		if (nResult != ERR_SUCCESS)
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(g_sw_ADP_SMS,"ADP_SMS: call CFW_SmsListMessages for me(%d) error is:0x%x",adp_pSMSContext->nSimId ,nResult);
			#else
			mmi_trace(g_sw_ADP_SMS,"ADP_SMS: call CFW_SmsListMessages for me) error is:0x%x" ,nResult);
			#endif
			setActualSMSNumber();
			SendEventToMMI(NULL, adp_pSMSContext->nStartupFinishMsgId);
			SendEventToMMI(NULL, adp_pSMSContext->nStartupReadyMsgId);
		}
		else
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(g_sw_ADP_SMS,"ADP_SMS: list next me(%d) sms",adp_pSMSContext->nSimId);
			#else
			mmi_trace(g_sw_ADP_SMS,"ADP_SMS: list next me sms");
			#endif
		}
	}
	else if(adp_pSMSContext->SMSListState == LIST_STATE_SM_START)
	{
		UINT32 nResult = 0;
		#ifdef __ADP_MULTI_SIM__
		#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		CFW_SMS_LIST ListInfo;
		ListInfo.nCount = LIST_COUNT_SIM;
		ListInfo.nStartIndex = adp_pSMSContext->nListIndex+1;
		ListInfo.nStorage = CFW_SMS_STORAGE_SM;
		ListInfo.nType = CFW_SMS_TYPE_PP;
		ListInfo.nStatus = CFW_SMS_STORED_STATUS_STORED_ALL;
		ListInfo.nOption = CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER;
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: List SIM startIndex = %d",ListInfo.nStartIndex);
		nResult = UT_CFW_SmsListMessages(&ListInfo,
									   FREE_UTI,
									   adp_pSMSContext->nSimId);
		
		#else/*no defined ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
		
		CFW_SMS_LIST ListInfo;
		U8 Uti = FREE_UTI;
		ListInfo.nCount = LIST_COUNT_SIM;
		ListInfo.nStartIndex = adp_pSMSContext->nListIndex+1;
		ListInfo.nStorage = CFW_SMS_STORAGE_SM;
		ListInfo.nType = CFW_SMS_TYPE_PP;
		ListInfo.nStatus = CFW_SMS_STORED_STATUS_STORED_ALL;
		ListInfo.nOption = CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER;

		#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
		Simu_Rsp_Marker_Struct simuRspMarker;
		simuRspMarker.msgID = EV_CFW_SMS_LIST_MESSAGE_RSP;
		simuRspMarker.nSimID = adp_pSMSContext->nSimId;
		simuRspMarker.Uti = Uti;
		simuRspMarker.pNext = NULL;
		adp_simu_add_Rsp_Marker(&simuRspMarker);
		#endif
		nResult = CFW_SmsListMessages(&ListInfo, 
										Uti, 
										gCurrentSMSSimID);
		#endif /*ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
		#else /*no defined __ADP_MULTI_SIM__*/
		
		nResult = CFW_SmsListMessages(CFW_SMS_STORAGE_SM/*storage*/, 
								   	    CFW_SMS_STORED_STATUS_STORED_ALL, 
								    	    LIST_COUNT_SIM, 
								    	    adp_pSMSContext->nListIndex+1, 
								    	    CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER,
								    	    FREE_UTI);
		#endif /*__ADP_MULTI_SIM__*/

		
		if (nResult != ERR_SUCCESS)
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(g_sw_SMS, "ADP_SMS: call CFW_SmsListMessages for sm(%d) error is: 0x%x", adp_pSMSContext->nSimId, nResult);
			#else
			mmi_trace(g_sw_SMS, "ADP_SMS: call CFW_SmsListMessages for sm error is: 0x%x", nResult);
			#endif
			setActualSMSNumber();
			SendEventToMMI(NULL, adp_pSMSContext->nStartupFinishMsgId);
			SendEventToMMI(NULL, adp_pSMSContext->nStartupReadyMsgId);
		}
		else
		{
			#ifdef __ADP_MULTI_SIM__
			mmi_trace(g_sw_ADP_SMS,"ADP_SMS: list next sm(%d) sms",adp_pSMSContext->nSimId);
			#else
			mmi_trace(g_sw_ADP_SMS,"ADP_SMS: list next sm sms");
			#endif
		}
	}
}
/**************************************************************

	FUNCTION NAME		: ListMessageRsp

  	PURPOSE				: deal "list message" rsp
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void ListMessageRsp(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	
	//static BOOL bListDone = FALSE; //a flag, when the messages in me are listed, it will be true.
	UINT8 messageIndex;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: ListMessageRsp  the nParam2 = %d, HIUINT16(nParam2) = %d, LOUINT16(nParam2) = %d, nType = %d", 
		nParam2, HIUINT16(nParam2), LOUINT16(nParam2),nType);

	
	TBM_ENTRY(0x29e9);
	if (0 == nType)
	{
		CFW_SMS_NODE *pSmsNode = NULL;
		UINT32 nResult = 0;
		PVOID pListHead = (PVOID)nParam1;
		UINT8 uMessageCount = HIUINT16(nParam2);
		adp_pSMSContext->nMsgCount += uMessageCount;//= LOUINT16(nParam2);
		adp_pSMSContext->nListIndex = LOUINT16(nParam2);

		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: uMessageCount is %d, the nMsgCount = %d, nListIndex = %d", uMessageCount, adp_pSMSContext->nMsgCount, adp_pSMSContext->nListIndex);
		if(adp_pSMSContext->SMSListState == LIST_STATE_ME_START)
		{
			adp_pSMSContext->nUsedMessageME += uMessageCount;
		}
		else if(adp_pSMSContext->SMSListState == LIST_STATE_SM_START)
		{
			adp_pSMSContext->nUsedMessageSM += uMessageCount;
		}
		messageIndex = 0;	
		for(messageIndex  = 0; messageIndex < uMessageCount; ++messageIndex)
		{	
			#ifdef __ADP_MULTI_SIM__
			#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			UT_CFW_SmsGetMessageNode(pListHead, messageIndex, &pSmsNode, adp_pSMSContext->nSimId);
			#else /*no define ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
			CFW_SmsGetMessageNode(pListHead, messageIndex, &pSmsNode, gCurrentSMSSimID);
			#endif/*ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
			#else /*no define __ADP_MULTI_SIM__*/
			CFW_SmsGetMessageNode(pListHead, messageIndex, &pSmsNode/*pSmsNode*/);
			#endif/*__ADP_MULTI_SIM__*/

      ASSERT(NULL != pSmsNode);      
			switch (pSmsNode->nType)
			{
			case ADP_CFW_SMS_TXT_DELIVERED_NO_HRD_INFO:
				break;
				
			case ADP_CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO:
				ListDeliveredTextHeadInfo(pSmsNode);
				break;
				
			case ADP_CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO:
				break;
				
			case ADP_CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO:
				ListSubmitedTextHeadInfo(pSmsNode);
				break;
				
			default:
				break;
			}
			
			OslMfree(pSmsNode);
			pSmsNode = NULL;
		}
		if(adp_pSMSContext->SMSListState== LIST_STATE_ME_START&& adp_pSMSContext->nSMSInME <= adp_pSMSContext->nListIndex)
		{
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: LIST_STATE_ME_OVER adp_pSMSContext->nSMSInME is %d, adp_pSMSContext->nMsgCount is %d", adp_pSMSContext->nSMSInME, adp_pSMSContext->nMsgCount );
			//sms list in me over
			adp_pSMSContext->SMSListState = LIST_STATE_ME_OVER;
			adp_pSMSContext->nMsgCount = 0;
			adp_pSMSContext->nListIndex = 0;
			
		}
		if(adp_pSMSContext->SMSListState == LIST_STATE_ME_OVER)
		{
			if(adp_pSMSContext->nSMSInSM != 0)
			{
				//sms list in sm start
				#ifdef __ADP_MULTI_SIM__
				#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	      			{
				CFW_SMS_LIST ListInfo;
				ListInfo.nCount = LIST_COUNT_SIM;
				ListInfo.nStartIndex = adp_pSMSContext->nListIndex + 1;
				ListInfo.nStorage = CFW_SMS_STORAGE_SM;
				ListInfo.nType = CFW_SMS_TYPE_PP;
				ListInfo.nStatus = CFW_SMS_STORED_STATUS_STORED_ALL;
				ListInfo.nOption = CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER;
				nResult = UT_CFW_SmsListMessages(&ListInfo,
											   FREE_UTI,
											   adp_pSMSContext->nSimId);
	      			}
				#else/*no defined ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
	      			{
				CFW_SMS_LIST ListInfo;
				U8 Uti = FREE_UTI;
				ListInfo.nCount = LIST_COUNT_SIM;
				ListInfo.nStartIndex = adp_pSMSContext->nListIndex + 1;
				ListInfo.nStorage = CFW_SMS_STORAGE_SM;
				ListInfo.nType = CFW_SMS_TYPE_PP;
				ListInfo.nStatus = CFW_SMS_STORED_STATUS_STORED_ALL;
				ListInfo.nOption = CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER;

				#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
				Simu_Rsp_Marker_Struct simuRspMarker;
				simuRspMarker.msgID = EV_CFW_SMS_LIST_MESSAGE_RSP;
				simuRspMarker.nSimID = adp_pSMSContext->nSimId;
				simuRspMarker.Uti = Uti;
				simuRspMarker.pNext = NULL;
				adp_simu_add_Rsp_Marker(&simuRspMarker);
				#endif
				
				nResult = CFW_SmsListMessages(&ListInfo, 
												Uti, 
												gCurrentSMSSimID);
	      			}
				#endif /*ADP_CFW_DUALSIM_SIMULATOR_SUPPORT*/
				#else /*no defined __ADP_MULTI_SIM__*/
				
				nResult = CFW_SmsListMessages(CFW_SMS_STORAGE_SM/*storage*/, 
										   	    CFW_SMS_STORED_STATUS_STORED_ALL, 
										    	    LIST_COUNT_SIM, 
										    	    1, 
										    	    CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER,
										    	    FREE_UTI);
				#endif /*__ADP_MULTI_SIM__*/
				adp_pSMSContext->SMSListState = LIST_STATE_SM_START;
				if (nResult != ERR_SUCCESS)
				{
					mmi_trace(g_sw_ADP_SMS,"ADP_SMS: call CFW_SmsListMessages for SM error is:0x%x, directly sm over", nResult);
					adp_pSMSContext->SMSListState = LIST_STATE_SM_OVER;
				}
				else
					mmi_trace(g_sw_ADP_SMS, "ADP_SMS: List after me over, list sm");
			}
			else
			{
				mmi_trace(g_sw_ADP_SMS,"ADP_SMS: List after me over, directly sm over");
				adp_pSMSContext->SMSListState = LIST_STATE_SM_OVER;
			}
			
		}
		if(adp_pSMSContext->SMSListState== LIST_STATE_SM_START&& adp_pSMSContext->nSMSInSM <= adp_pSMSContext->nListIndex)
		{
			//sms list in sim over
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: LIST_STATE_SM_OVER adp_pSMSContext->nSMSInSM is %d, adp_pSMSContext->nMsgCount is %d", adp_pSMSContext->nSMSInSM, adp_pSMSContext->nMsgCount );
			adp_pSMSContext->SMSListState= LIST_STATE_SM_OVER;
		}
			
	}
	else
	{
		
		/*
		if (nParam1 == UDP error )
			*/
	
		{
			mmi_trace(g_sw_ADP_SMS, "ADP_SMS: UDP error: The UTI  = %0x, the nType = %0x! EV_CFW_SMS_LIST_MESSAGES_RSP: Failed, reason = %0x\n", nUTI, nType, nParam1);
			
			 adp_pSMSContext->nMsgCount++;
			 if(ERR_CMS_OPERATION_NOT_ALLOWED == nParam1)//means no more message to list, the error code name should be modify later
			 {
			 	if (adp_pSMSContext->SMSListState== LIST_STATE_ME_START)
			 	{
					adp_pSMSContext->nListIndex = adp_pSMSContext->nSMSInME;
			 	}
				else if(adp_pSMSContext->SMSListState== LIST_STATE_SM_START )
				{
					adp_pSMSContext->nListIndex = adp_pSMSContext->nSMSInSM;
				}
			 }
			 else
			 {
				 adp_pSMSContext->nListIndex++;
			 }

			 if (adp_pSMSContext->SMSListState== LIST_STATE_ME_START)
			 {

			 	if ( adp_pSMSContext->nSMSInME <= adp_pSMSContext->nListIndex)
			 	{
					adp_pSMSContext->SMSListState = LIST_STATE_ME_OVER;
					adp_pSMSContext->SMSListState = LIST_STATE_SM_START;
					adp_pSMSContext->nMsgCount = 0;
					adp_pSMSContext->nListIndex = 0;
			 	}

				ListNextMessageREQ();
			 	
			 }
			else if(adp_pSMSContext->SMSListState== LIST_STATE_SM_START )
			{
				if (adp_pSMSContext->nSMSInSM<= adp_pSMSContext->nListIndex)
				{
					adp_pSMSContext->SMSListState= LIST_STATE_SM_OVER;
				}
				else
				{
					ListNextMessageREQ();
				}
			}

		}
	}
	
	if (adp_pSMSContext->SMSListState == LIST_STATE_SM_OVER)
	{
		//SendEventToMMI(NULL, PRT_MSG_ID_MMI_SMS_STARTUP_FINISH_IND);
		//SendEventToMMI(NULL, PRT_MSG_ID_MMI_SMS_READY_IND);
		setActualSMSNumber();
		SendEventToMMI(NULL, adp_pSMSContext->nStartupFinishMsgId);
		SendEventToMMI(NULL, adp_pSMSContext->nStartupReadyMsgId);
#ifdef __ADP_MULTI_SIM__
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: sim%d initializing finished",adp_pSMSContext->nSimId);
#endif /* __ADP_MULTI_SIM__ */
	}	

	TBM_EXIT(0x29e9);
	
}

/**************************************************************

	FUNCTION NAME		: ListMessageRsp

  	PURPOSE				: deal "new message coming" 
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static BOOL NewMessage(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	UINT32 seg;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry DealNewMessage");

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
#if 0
	if (SimuGetSMSID() == ADP_SIM_1)
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: send sim 1 id error!");
		adp_pSMSContext = &adp_gSMSContext[0];
	}
	else if(SimuGetSMSID() == ADP_SIM_2)
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: send sim 2 id error!");
		adp_pSMSContext = &adp_gSMSContext[1];
	}
	else
	{
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: send sim id error!");
	}
#endif

	adp_pSMSContext = &adp_gSMSContext[1];


	extern CFW_SMS_NODE* simulator_write_sms_node(CFW_SMS_WRITE* pSmsInfo);

	CFW_SMS_WRITE writeMessage;
	
	writeMessage.nStorage = 2;
	writeMessage.nType = CFW_SMS_TYPE_PP;
	writeMessage.nIndex = 0;
	writeMessage.nStatus = CFW_SMS_STORED_STATUS_UNREAD;

	simulator_write_sms_node(&writeMessage);


#endif
	seg = nParam2;
	
	if (0 == nType)
	{
		CFW_NEW_SMS_NODE *pSmsNode = (CFW_NEW_SMS_NODE*)nParam1;

		ASSERT(NULL != pSmsNode);	

		switch (pSmsNode->nType)
		{
		case 0x20:
			break;
			
		case 0x21:
			DealNewMsgTxtHRDV1(pSmsNode,seg);
			break;
			
		case 0x22:
			break;
			
		case 0x41:
			DealStatusReport(pSmsNode);
			break;

		default:
			break;
		}
        
              OslMfree((PVOID)nParam2); //wangzl: 090624 add as CSW required
	}
            
	return TRUE;
}
//2009-5-6 for MMS push  begin
#ifdef JATAAYU_SUPPORT

#ifdef __ADP_MULTI_SIM__
extern void MMS_SendPushMsgToTransport (INT8 *pcBuffer, INT32 ulBuffrLen, UINT8 nSimID) ;
#else
extern void MMS_SendPushMsgToTransport (INT8 *pcBuffer, INT32 ulBuffrLen) ;
#endif

#define MMS_NOTIFICATION_REASSMBLE

static BOOL NewPushMessage(UINT32 nParam1, UINT32 nParam2, UINT32 nParam3, UINT16 nUTI)
{
	CFW_NEW_SMS_NODE *pSmsNode = (CFW_NEW_SMS_NODE*)nParam1;
	CFW_SMS_TXT_HRD_V1_IND *pTxtHrdNode = pSmsNode->pNode ;

	UINT8 nType = HIUINT8(nParam3);
#ifdef __ADP_MULTI_SIM__
    UINT8 nSimID = LOUINT8(nParam3);//nSimID is for jataayu
#endif
#ifdef MMS_NOTIFICATION_REASSMBLE	
		
	CFW_EMS_INFO *pstEmsInfo = (CFW_EMS_INFO *)nParam2 ;
	S8			 *ucPushBuff ;
	INT			 iPushLen ;
	UINT32       nRet;
#endif	
#ifdef __ADP_MULTI_SIM__
mmi_trace(1,"JDD_LOG: NewPushMessage  pSmsNode->nType=0x%x,nSimID=%d",pSmsNode->nType,nSimID);
#endif

	if (0 == nType)
		{
		
		switch (pSmsNode->nType)
		{
			/*Jataayu Push*/
			case 0x21:// 0x04:
			case 0x22://0x05:
#ifdef MMS_NOTIFICATION_REASSMBLE

			iPushLen = 12 + pTxtHrdNode->nDataLen ;
			ucPushBuff = OslMalloc (iPushLen) ;
			ucPushBuff [0] = 0x0B ; //Header Length
			ucPushBuff [1] = 0x05 ; //UDH IE Identifier
			ucPushBuff [2] = 0x04 ; //UDH IE Identifier length

                     ucPushBuff [3] = (S8)(pstEmsInfo->nDesPortNum);
                     ucPushBuff [4] = (S8)(pstEmsInfo->nDesPortNum>>8);
                     ucPushBuff [5] = (S8)(pstEmsInfo->nOriPortNum); 
		       ucPushBuff [6] = (S8)(pstEmsInfo->nOriPortNum>>8); 
			
			mmi_trace(1,"JDD_LOG: destport %d orignator port %d",pstEmsInfo->nDesPortNum , pstEmsInfo->nOriPortNum);
			ucPushBuff [7] = 0x00 ; //UDH IE Identifier SAR
			ucPushBuff [8] = 0x03 ; //UDH IE Identifier SAR Length
			ucPushBuff [9] = pstEmsInfo->nRerNum ; //Refrence Number
			ucPushBuff [10] = pstEmsInfo->nMaxNum ; //Total segment count
			ucPushBuff [11] = pstEmsInfo->nCurrentNum ; //Current segment count

			if(ucPushBuff [10] == 0 && ucPushBuff [11] == 0)
			{
				ucPushBuff [10] = 1;
				ucPushBuff [11] = 1;
				mmi_trace(1,"JDD_LOG: makethe currnum andmaxnum as 1");
			}

			mmi_trace(0,"JDD_LOG: new dataptr %d, datalen %d ",pTxtHrdNode->pData,pTxtHrdNode->nDataLen );

			if(pTxtHrdNode->pData)
			{
				mmi_trace(0,"JDD_LOG:commented SUL_MemCopy8 and delete");
				SUL_MemCopy8 ((void *)(ucPushBuff + 12), pTxtHrdNode->pData, \
					pTxtHrdNode->nDataLen) ;
			}
			COS_PageProtectSetup(5, 2,(UINT32)ucPushBuff -16 , (UINT32)ucPushBuff+((((iPushLen + 1 + (4 - (iPushLen & 3)) + 4) >> 2))<<2));
			COS_PageProtectEnable(5);
			mmi_trace(1,"JDD_LOG: push no is Refrence Number[%d], Total segment count [%d],Current [%d]", \
						pstEmsInfo->nRerNum, pstEmsInfo->nMaxNum, pstEmsInfo->nCurrentNum);

#ifdef __ADP_MULTI_SIM__
#ifdef JATAAYU_SUPPORT_MMS
			MMS_SendPushMsgToTransport ((S8*)ucPushBuff, iPushLen, nSimID) ;
            nRet = CFW_SmsDeleteMessage(pSmsNode->nConcatCurrentIndex,CFW_SMS_STORED_STATUS_STORED_ALL,pSmsNode->nStorage,CFW_SMS_TYPE_PP,FREE_UTI,nSimID);
#endif			
#else
#ifdef JATAAYU_SUPPORT_MMS
			MMS_SendPushMsgToTransport ((S8*)ucPushBuff, iPushLen) ;
			nRet = CFW_SmsDeleteMessage(pSmsNode->nConcatCurrentIndex,CFW_SMS_STORED_STATUS_STORED_ALL,pSmsNode->nStorage,FREE_UTI);
#endif			
#endif
			mmi_trace(1,"Delete the SMS Index 0x%x Storage %d The return value 0x%x",pSmsNode->nConcatCurrentIndex,pSmsNode->nStorage,nRet);
#else
#ifdef JATAAYU_SUPPORT_MMS
			MMS_SendPushMsgToTransport (pTxtHrdNode->pData, pTxtHrdNode->nDataLen) ;
#endif
#endif
			COS_PageProtectDisable(5);
			OslMfree(ucPushBuff);
				//DealPushMsgTxtHRDV1(pSmsNode,seg);//seg is port number
		break ;
		default:
			break ;
			}
        
                    OslMfree((PVOID)nParam2); 
		}

        return TRUE ;	
}
#else
static BOOL DeletePushMessage(UINT32 nParam1, UINT32 nParam2, UINT32 nParam3, UINT16 nUTI)
{
	UINT8 nType = HIUINT8(nParam3);
	UINT32 nRet= ERR_SUCCESS;
	CFW_NEW_SMS_NODE *pSmsNode = (CFW_NEW_SMS_NODE*)nParam1;
#ifdef __ADP_MULTI_SIM__
	UINT8 nSimID = LOUINT8(nParam3);//nSimID is for jataayu
#endif

	
	mmi_trace(g_sw_ADP_SMS, "DeletePushMessage");

	if (0 == nType)
	{
	#ifdef __ADP_MULTI_SIM__			
		nRet = CFW_SmsDeleteMessage(pSmsNode->nConcatCurrentIndex,CFW_SMS_STORED_STATUS_STORED_ALL,pSmsNode->nStorage,CFW_SMS_TYPE_PP,FREE_UTI,nSimID);
	#else 
		nRet = CFW_SmsDeleteMessage(pSmsNode->nConcatCurrentIndex,CFW_SMS_STORED_STATUS_STORED_ALL,pSmsNode->nStorage,FREE_UTI);
	#endif

		mmi_trace(g_sw_ADP_SMS, "Call CFW_SmsDeleteMessage return value 0x%x", nRet);
		OslMfree((PVOID)nParam2);
	}

        return TRUE ;	
	
}

#endif
//2009-5-6 MMS push end
/**************************************************************

	FUNCTION NAME		: DealSapceFullNoWaiting

  	PURPOSE				: storage is full and no waiting message
	INPUT PARAMETERS	: nParam2

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void DealSapceFullNoWaiting(UINT32 nParam2)
{	
	mmi_sms_mem_full_ind_struct *pMemFull;
	UINT8 nStorageType;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry DealSapceFullNoWaiting");
	
	pMemFull = OslMalloc(sizeof(mmi_sms_mem_full_ind_struct));
	ASSERT(NULL != pMemFull);
	memset(pMemFull, 0, sizeof(mmi_sms_mem_full_ind_struct));

	nStorageType = LOUINT8(nParam2);

	switch (nStorageType)
	{
	case CFW_SMS_STORAGE_ME:
		adp_pSMSContext->bMEFull = TRUE;
		adp_pSMSContext->nCurrentStorage = CFW_SMS_STORAGE_SM;

		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The ME has full, and the storage is SM");
		break;

	case CFW_SMS_STORAGE_SM:
		adp_pSMSContext->bSIMFull = TRUE;
		adp_pSMSContext->nCurrentStorage = CFW_SMS_STORAGE_ME;

		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The SM has full, and the storage is ME, the adp_pSMSContext->nCurrentStorage = 0x%0x", adp_pSMSContext->nCurrentStorage);
		break;

	default:
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The storage type can't be supported!");
		ASSERT(0);
		break;
	}
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_CfgSetNewSmsOption( CFW_SMS_MULTIPART_OPTION_ALL|CFW_SMS_ROUT_DETAIL_INFO, 
              UT_CFW_CFG_RECEIVE_STORAGE(adp_pSMSContext->nCurrentStorage, 0, 0), adp_pSMSContext->nSimId);
#else
	CFW_CfgSetNewSmsOption( CFW_SMS_MULTIPART_OPTION_ALL|CFW_SMS_ROUT_DETAIL_INFO, 
              CFW_CFG_RECEIVE_STORAGE(adp_pSMSContext->nCurrentStorage, 0, 0), gCurrentSMSSimID);
#endif
#else
	CFW_CfgSetNewSmsOption( CFW_SMS_MULTIPART_OPTION_ALL|CFW_SMS_ROUT_DETAIL_INFO, 
              CFW_CFG_RECEIVE_STORAGE(adp_pSMSContext->nCurrentStorage, 0, 0));
#endif
	
	if (adp_pSMSContext->bSIMFull && adp_pSMSContext->bMEFull)
	{
		pMemFull->type = SMSAL_MEM_FULL_NORMAL;
		SendEventToMMI((local_para_struct*)pMemFull, PRT_MSG_ID_MMI_SMS_MEM_FULL_IND);
	}
	
}

/**************************************************************

	FUNCTION NAME		: DealSapceFullNoWaiting

  	PURPOSE				: storage is full and exist waiting messages
	INPUT PARAMETERS	: nParam2

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void DealSapceFullWaiting(UINT32 nParam2)
{
	mmi_sms_mem_exceed_ind_struct *pMemExceed;
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry DealSapceFullWaiting");
	
	pMemExceed = 
		(mmi_sms_mem_exceed_ind_struct*)OslMalloc(sizeof(mmi_sms_mem_exceed_ind_struct));
	ASSERT(NULL != pMemExceed);
	memset(pMemExceed, 0, sizeof(mmi_sms_mem_exceed_ind_struct));
	
	pMemExceed->type = SMSAL_MEM_EXCEED_NORMAL;

	SendEventToMMI((local_para_struct*)pMemExceed, PRT_MSG_ID_MMI_SMS_MEM_EXCEED_IND);
}

/**************************************************************

	FUNCTION NAME		: SmsInfoInd

  	PURPOSE				: Deal memory case
	INPUT PARAMETERS	: nParam1, nParam2, nType, nUTI 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void SmsInfoInd(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: entry SmsInfoInd ");
	if (0 == nType)
	{
		switch (nParam1)
		{
			case MESSAGE_SPACE_AVAILABLE:        //当手机或SIM卡有存储空间时,图标不能闪烁,所以需要向MMI层发图标不闪消息
			{
				mmi_sms_mem_available_ind_struct *pMemFull = OslMalloc(sizeof(mmi_sms_mem_available_ind_struct));
				ASSERT(NULL != pMemFull);
				memset(pMemFull, 0, sizeof(mmi_sms_mem_available_ind_struct));
				pMemFull->type = SMSAL_MEM_AVAIL_NORMAL;
				SendEventToMMI((local_para_struct*)pMemFull, PRT_MSG_ID_MMI_SMS_MEM_AVAILABLE_IND);
			}
			break;

			case MESSAGE_SPACE_FULL_NO_WAITING:
				DealSapceFullNoWaiting(nParam2);
				break;

			case MESSAGE_SPACE_FULL_WAITING:
				DealSapceFullWaiting(nParam2);
				break;

			default:
				break;
		}
	}
	else
	{	
		mmi_trace(g_sw_ADP_SMS, "ADP_SMS: The EV_CFW_SMS_INFO_IND bug, the nType is %d" ,nType);
		ASSERT(0);
	}
	
}

/************************************************************
	FUNCTION NAME		: NotifySMSInitDone

  	PURPOSE				: Nofity SMS Init Done on MMI layer
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void NotifySMSInitDone(void)
{
	U8 i;
	mmi_trace(g_sw_ADP_SMS,"ADP_SMS: NotifySMSInitDone");
	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		#ifdef __ADP_MULTI_SIM__
		CFW_SmsInitComplete(FREE_UTI, adp_gSMSContext[i].nSimId);
		#else
		CFW_SmsInitComplete(FREE_UTI);
		#endif
	}
}
BOOL isOnsendToManyProcess = FALSE;
static void NotifySMSSendInfo(BOOL on_off)
{
	if(isOnsendToManyProcess == on_off)
	{
		return;
	}
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: NotifySMSSendInfo, begin_end is:%d",on_off);
	isOnsendToManyProcess = on_off;
	#ifdef __ADP_MULTI_SIM__
	CFW_SetSMSConcat(on_off, adp_pSMSContext->nSimId);
	#else
	CFW_SetSMSConcat(on_off);
	#endif
}
/**************************************************************

	FUNCTION NAME		: ADP_DispatchSmsIndMsg

  	PURPOSE				: Deal all the csw indication message
	INPUT PARAMETERS	: pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: BOOL

	REMARKS			: 

**************************************************************/
static BOOL ADP_DispatchSmsIndMsg(COS_EVENT *pCosEvent)
{
	UINT32 nParam1;
	UINT32 nParam2;
	UINT32 nParam3;
	UINT8 nType;
	UINT16 nUTI;

	ASSERT(NULL != pCosEvent);
	
	nParam1 = pCosEvent->nParam1;
	nParam2 = pCosEvent->nParam2;
	nParam3 = pCosEvent->nParam3;
	nType = HIUINT8(pCosEvent->nParam3);
	nUTI = HIUINT16(pCosEvent->nParam3);
	
	TBM_ENTRY(0x29e8);
#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_SMS, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: Indication, set context as SIM%d, nEventId is %d", adp_pSMSContext->nSimId, pCosEvent->nEventId);
#endif

	switch (pCosEvent->nEventId)
	{
		case EV_CFW_NEW_SMS_IND:
			NewMessage(nParam1, nParam2, nType, nUTI);
			break;
			
		case EV_CFW_SMS_INFO_IND: 
			SmsInfoInd(nParam1, nParam2, nType, nUTI);
			break;
            
#ifdef JATAAYU_SUPPORT            
//FIX ME:if no wap/mms then EMS ind will not support
		case EV_CFW_NEW_EMS_IND:
			NewPushMessage(nParam1, nParam2, nParam3, nUTI) ;
			break ;
#else
		case EV_CFW_NEW_EMS_IND:
			DeletePushMessage(nParam1, nParam2, nParam3, nUTI);
			break;
      
#endif            
		default:
			TBM_EXIT(0x29e8);
			return FALSE;
	}
	TBM_EXIT(0x29e8);
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchSmsIndMsg

  	PURPOSE				: Deal all the MMI req message
	INPUT PARAMETERS	: pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: BOOL

	REMARKS			: 

**************************************************************/
static BOOL ADP_DispatchSmsReqMsg(COS_EVENT *pCosEvent)
{	
	UINT32 nParam1;
	UINT32 nParam2;
	UINT8 nType;
	UINT16 nUTI;

	ASSERT(NULL != pCosEvent);
	
	nParam1 = pCosEvent->nParam1;
	nParam2 = pCosEvent->nParam2;
	nType = HIUINT8(pCosEvent->nParam3);
	nUTI = HIUINT16(pCosEvent->nParam3);

	TBM_ENTRY(0x29e7);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_SMS, adp_GetMMISimID(ADP_GET_MMI_DEST_MOD(pCosEvent->nParam3)));
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: Req, set context as SIM%d", adp_pSMSContext->nSimId);
#endif
	
	switch (pCosEvent->nEventId)
	{
		case PRT_MSG_ID_MMI_SMS_GET_MAILBOX_ADDRESS_REQ:
			GetVoiceMailReq(nParam1, nParam2, nType, nUTI);
		break;
			
		case PRT_MSG_ID_MMI_SMS_SET_MAILBOX_ADDRESS_REQ:
			SetVoiceMailReq(nParam1, nParam2, nType, nUTI);
		break;
		
		case PRT_MSG_ID_MMI_SMS_GET_MSG_REQ:
			GetMessageReq(nParam1, nParam2, nType, nUTI);
			break;

		case PRT_MSG_ID_MMI_SMS_SEND_MSG_REQ:
			SendMessageReq(nParam1, nParam2, nType, nUTI);
			break;

		case PRT_MSG_ID_MMI_SMS_SET_MSG_REQ:
			SetMessageReq(nParam1, nParam2, nType, nUTI);
			break;

		case PRT_MSG_ID_MMI_SMS_DEL_MSG_REQ:
			DeleteMessageReq(nParam1, nParam2, nType, nUTI);
			break;

		case PRT_MSG_ID_MMI_SMS_GET_PROFILE_NUM_REQ:
			GetProfileNumReq(nParam1, nParam2, nType, nUTI);
			break;
			
		case PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_REQ:
			GetProfileReq(nParam1, nParam2, nType, nUTI);
			break;

		case PRT_MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_REQ:
			SetProfileReq(nParam1, nParam2, nType, nUTI);
			break;

		case PRT_MSG_ID_MMI_SMS_GET_COMMON_PARAMS_REQ:
			GetCommonReq(nParam1, nParam2, nType, nUTI);
			break;

		case PRT_MSG_ID_MMI_SMS_SET_COMMON_PARAMS_REQ:
			SetCommonReq(nParam1, nParam2, nType, nUTI);
			break;

		case PRT_MSG_ID_MMI_SMS_GET_PREFERRED_STORAGE_REQ:
			GetPreferredStorageReq(nParam1, nParam2, nType, nUTI);
			break;

		case PRT_MSG_ID_MMI_SMS_SET_PREFERRED_STORAGE_REQ:
			SetPreferredStorageReq(nParam1, nParam2, nType, nUTI);
			break;

		case PRT_MSG_ID_MMI_SMS_GET_MSG_NUM_REQ:
			GetMessageNumReq(nParam1, nParam2, nType, nUTI);
			break;
			
		case PRT_MSG_ID_MMI_SMS_ABORT_REQ:
			AbortMessageREQ();
			break;

		case PRT_MSG_ID_MMI_SMS_STARTUP_READ_NEXT_REQ:
			ListNextMessageREQ();
			break;
		case PRT_MSG_ID_MMI_SMS_INIT_DONE:
			NotifySMSInitDone();
			break;
		case PRT_MSG_ID_MMI_SMS_SEND_BEGIN:
			NotifySMSSendInfo(TRUE);
			break;
		case PRT_MSG_ID_MMI_SMS_SEND_END:
			NotifySMSSendInfo(FALSE);
			break;
			
		default:
			TBM_EXIT(0x29e7);
			return FALSE;
	}
	TBM_EXIT(0x29e7);
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchSmsRspMsg

  	PURPOSE				: Deal all the csw rsp message
	INPUT PARAMETERS	: pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: BOOL

	REMARKS			: 

**************************************************************/
static BOOL ADP_DispatchSmsRspMsg(COS_EVENT *pCosEvent)
{
	UINT32 nParam1;
	UINT32 nParam2;
	UINT8 nType;
	UINT16 nUTI;

#ifdef __ADP_MULTI_SIM__
	U8 nSimID;
#endif

	ASSERT(NULL != pCosEvent);
	
	nParam1 = pCosEvent->nParam1;
	nParam2 = pCosEvent->nParam2;
	nType = HIUINT8(pCosEvent->nParam3);
	nUTI = HIUINT16(pCosEvent->nParam3);

	TBM_ENTRY(0x29e6);
#ifdef __ADP_MULTI_SIM__
	nSimID = adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3));
	#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
	nSimID = adp_simu_get_sim_id_by_uti_for_rsp_msg(pCosEvent->nEventId, nUTI, nSimID);
	#endif
	adp_ConfigureContext(ADP_MODULE_SMS, nSimID);
	mmi_trace(g_sw_ADP_SMS, "ADP_SMS: response, set context as SIM%d", adp_pSMSContext->nSimId);
#endif
	
	switch (pCosEvent->nEventId)
	{
		case EV_CFW_SMS_LIST_MESSAGE_RSP:
			ListMessageRsp(nParam1, nParam2, nType, nUTI);
			break;
		case EV_CFW_SMS_READ_MESSAGE_RSP:
			GetMessageRsp(nParam1, nParam2, nType, nUTI);
			break;

		case EV_CFW_SMS_SEND_MESSAGE_RSP:
			SendMessageRsp(nParam1, nParam2, nType, nUTI);
			break;

		case EV_CFW_SMS_WRITE_MESSAGE_RSP:
			WriteMessageRsp(nParam1, nParam2, nType, nUTI);
			break;

		case EV_CFW_SMS_DELETE_MESSAGE_RSP:
			DeleteMessageRsp(nParam1, nParam2, nType, nUTI);
			break;
		case EV_CFW_SMS_SET_PARAM_RSP:
			SendGetProfileRsp(nParam1, nParam2, nType, nUTI);
			break;
			
		default:
			TBM_EXIT(0x29e6);
			return FALSE;
	}
	TBM_EXIT(0x29e6);
	return TRUE;	
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchSmsMsg

  	PURPOSE				: All the messages entry, the function dispatches the message into different functions to be delt.
	INPUT PARAMETERS	: pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: BOOL

	REMARKS			: 

**************************************************************/
BOOL ADP_DispatchSmsMsg(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);
	if (pCosEvent->nEventId >= MSG_ID_MMI_SMS_SET_PROFILE_PARAMS_REQ && pCosEvent->nEventId <= MSG_ID_MMI_SMS_GET_MSG_LIST_REQ)
		return ADP_DispatchSmsReqMsg(pCosEvent);		
	
	else if (pCosEvent->nEventId >= EV_CFW_SMS_RSP_BASE && pCosEvent->nEventId <= EV_CFW_SMS_RSP_END_)
		return ADP_DispatchSmsRspMsg(pCosEvent);
	
	else if (pCosEvent->nEventId >= EV_CFW_IND_BASE && pCosEvent->nEventId <= EV_CFW_IND_END_)
		return ADP_DispatchSmsIndMsg(pCosEvent);
	
	else
		return FALSE;
}

