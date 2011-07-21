/******************************************************************************
*              Copyright (C) 2006, Coolsand Technologies, Inc.
*                            All Rights Reserved
*****************************************************************************
* Filename:    adp_SIM_Events.c
*
* Description:
*     change CSD Sim message into coolsand message .
*     @Jackiechen 2006-07-11.
******************************************************************************/

/************************************************************************
  Change Histroy
  Version      Date             Author    Description
  02.01.001  2007-07-11, Li Tao, 添加、维护映射表（与SIM卡中电话本的物理存储位置索引对应）
            向SIM卡添加电话本记录时，新记录的存储位置由MMI传给CSW
 ************************************************************************/
 
/**************************************************************
 * Include Files
**************************************************************/
#include "mmi_features.h"
#include "phonebooktypes.h"
#include "unit_test.h"
#define __NEWSIMULATOR
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

//zhoumn added for call cost @2006/11/27>>>start
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
//zhoumn added for call cost @2006/11/27<<<end

 
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

#ifdef __ADP_MULTI_SIM__
#include "dual_sim.h"
#endif

#include "device.h"
#include "stack_msgs.h"
#include <cswtype.h>
#include "mmi_msg_struct.h"
#include "sul.h"
#include "mmi_trace.h"
#include "simdetectionhwmapfile.h"
//#include "phb_utils.h"



/**************************************************************
 * Macros
**************************************************************/



/**************************************************************
 * Type Definitions
**************************************************************/

/**************************************************************
 * Manifest Constants
**************************************************************/
PUBLIC UINT16 nCurAccess = 1;
PUBLIC UINT8 SimStorageID = 0;
PUBLIC BOOL flag_init_ok = FALSE;

static UINT16 nMSISDNusedSlot = 0;
static UINT16 nMSISDNtotalSlot = 0;

//static UINT8 PIN1[MAX_SIM_PASSWD_LEN] = {0};
static UINT8 PIN2[MAX_SIM_PASSWD_LEN] = {0};

//add by panxu 2006-11-16
static UINT32 nLockType = 0;
static BOOL g_IsPinPassed = FALSE;
static BOOL g_IsPinChanged = FALSE;
static BOOL g_IsPukPassed = FALSE;

static UINT8 PrefOperList[384] = {0};
static UINT8 PrefOperListLen = 0;
static const UINT16 nPreNWUTI = 50;
extern void CswPLMN2CSDPLMN(UINT8 *pCSDPLMN,  UINT8 *pCswPLMN,  UINT8 nLength);
extern void CSDPLMN2CswPLMN(UINT8 *pCswPLMN,  UINT8 *pCSDPLMN,  UINT8 nLength);
extern void Msg_Adaption_PHB_startup_begin(void);
extern void  Msg_Adaption_PHB_Init_Ready(void);

static message_type msgType = PHB_MSG;

#ifdef __ADP_MULTI_SIM__

PUBLIC BOOL flag_is_fdn_state_on[ADP_SIM_NUMBER] = {FALSE,};
static BOOL g_b_fdn_state_req[ADP_SIM_NUMBER] = {FALSE,};
extern BOOL g_PHBSIMStartInd[];
extern BOOL g_PHBSIMStartFinish[];
extern adp_phb_context_t adp_gPHBContex[];
extern void ADP_PhbInitFinishOneCardStartAnother(ADP_SIM_ID_T SimId);
extern U8 gPHBCurrentSimID;
#else
extern adp_phb_context_t adp_gPHBContex;
#endif

#ifdef __ADP_MULTI_SIM__
adp_sim_context_t g_SIMContext[ADP_SIM_NUMBER];
adp_sim_context_t *g_pSIMContext = g_SIMContext;
#endif
extern adp_phb_context_t *adp_pPHBContext;
/**************************************************************
 * Local Functions Prototypes
**************************************************************/
extern void  Msg_Adaption_PHB_Init(void);
extern void adp_phb_del_entry_error_rsp(void);
extern void adp_phb_del_entry_ok_rsp(void);
extern void adp_phb_set_entry_error_rsp(void);

extern UINT8 AdaptNETSETGetPreferredNetworkReq(COS_EVENT *pCosEvent);
extern UINT8 DoNETSETGetPreferredNetworkRsp(COS_EVENT *pCosEvent);
extern UINT8 AdaptNETSETAddorDeletePrefNetworkReq(COS_EVENT *pCosEvent);
extern UINT8 DoNETSETAddorDeletePrefNetworkRsp(COS_EVENT *pCosEvent);
extern UINT8 DoNumberOfPLMNSuported(COS_EVENT *pCosEvent);


extern BOOL MMI_PHB_PHONEBOOK_TYPE;

/**************************************************************
 * Local Functions definitions
**************************************************************/

/**************************************************************
	FUNCTION NAME		: SetMsgFlag
	PURPOSE			    : set the message flag type to distinguish the different messages
	INPUT PARAMETERS	: msgFlag
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS			    :
**************************************************************/
void SetMsgFlag(message_type msgFlag)
{
	msgType = msgFlag;
}


/**************************************************************
	FUNCTION NAME		: GetMsgFlag
	PURPOSE			    : get the message flag type to distinguish the different messages
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS			    :
**************************************************************/
message_type GetMsgFlag()
{
	return msgType;
}

/**************************************************************
	FUNCTION NAME		: adp_phb_MSISDN_set_entry_ok_rsp
	PURPOSE			       : respond PRT_PHB_SET_ENTRY_RSP.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void adp_phb_MSISDN_set_entry_ok_rsp(void)
{
	ilm_struct  ilm_ptr;
	mmi_phb_set_entry_rsp_struct *phb = OslMalloc(sizeof(mmi_phb_set_entry_rsp_struct));
	ASSERT(phb != NULL);
	memset(phb, 0x00, sizeof(mmi_phb_set_entry_rsp_struct));
  //add by chenq for bug 5889 and 5800, 20070803 B 
  COS_KillTimerEX(GetMmiTaskHandle(MOD_ADP),PHBLIST_COPY_OR_DELETE_TIMEROUTID );
  //add by chenq for bug 5889 and 5800, 20070803 E
	phb->result.flag = L4C_OK;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = PRT_PHB_SET_ENTRY_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)phb;
	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************

	FUNCTION NAME		: adp_phb_get_extra_number_rsp

	PURPOSE				: set the rsp body for PRT_PHB_GET_ENTRY_BY_INDEX_RSP

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/
void  adp_phb_get_extra_number_rsp(UINT32 nParam1, UINT32 nParam2, UINT16 totalSlot)
{
	UINT16 i = 0;
	UINT16 j = 0;
	CFW_SIM_PBK_ENTRY_INFO *entryInfo = NULL;

	ilm_struct  ilm_ptr;
	mmi_phb_get_entry_by_index_rsp_struct *phb = OslMalloc(sizeof(mmi_phb_get_entry_by_index_rsp_struct));
	ASSERT(phb != NULL);
	memset(phb, 0x00, sizeof(mmi_phb_get_entry_by_index_rsp_struct));

	if (totalSlot > 0)
	{
		phb->result.flag = L4C_OK;
		phb->no_list = totalSlot;

		for (i = 0; i < totalSlot && i < MAX_SIM_ENTRIES; i++)
		{
			phb->list[i].storage = MMI_SIM;
			phb->list[i].index = i + 1;

			if (j < nParam2)
			{
				entryInfo = (CFW_SIM_PBK_ENTRY_INFO *)(nParam1 + (sizeof(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE) * j);
					
				if ((entryInfo->phoneIndex - 1) == i)
				{
					j++;

					phb->list[i].record_index = entryInfo->phoneIndex;

					if (entryInfo->pFullName[0] == 0x80)
					{
						phb->list[i].alpha_id.name_dcs = MMI_PHB_UCS2;
						memcpy(phb->list[i].alpha_id.name, entryInfo->pFullName + 1, entryInfo->iFullNameSize - 1);
						phb->list[i].alpha_id.name[entryInfo->iFullNameSize - 1] = '\0';
						phb->list[i].alpha_id.name[entryInfo->iFullNameSize] = '\0';
						phb->list[i].alpha_id.name_length = entryInfo->iFullNameSize - 1;
					}
					else
					{
						phb->list[i].alpha_id.name_dcs = MMI_PHB_ASCII;
						memcpy(phb->list[i].alpha_id.name, entryInfo->pFullName, entryInfo->iFullNameSize);
						phb->list[i].alpha_id.name[entryInfo->iFullNameSize] = '\0';
						phb->list[i].alpha_id.name_length = entryInfo->iFullNameSize;
					}

					if (entryInfo->nType == 129 || entryInfo->nType == 161)
						phb->list[i].tel.type = MMI_CSMCC_DEFAULT_ADDR_TYPE;
					else
						phb->list[i].tel.type = MMI_CSMCC_INTERNATIONAL_ADDR;
					phb->list[i].tel.length = SUL_GsmBcdToAscii(entryInfo->pNumber, entryInfo->nNumberSize, phb->list[i].tel.number);
				}
			}
		}
	}
	else
	{
		phb->result.flag = L4C_ERROR;
	}

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = ADP_PHB_MSG_SRC_MOD;
	ilm_ptr.msg_id = PRT_PHB_GET_ENTRY_BY_INDEX_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)phb;
	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************

	FUNCTION NAME		: adp_phb_get_FDN_number_rsp

	PURPOSE				: set the rsp body for PRT_PHB_GET_ENTRY_BY_INDEX_RSP

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/
void  adp_phb_get_FDN_number_rsp(UINT32 nParam1, UINT32 nParam2)
{
	UINT16 i = 0;
	CFW_SIM_PBK_ENTRY_INFO *entryInfo = NULL;

	ilm_struct  ilm_ptr;
	mmi_phb_get_entry_by_index_rsp_struct *phb = OslMalloc(sizeof(mmi_phb_get_entry_by_index_rsp_struct));
	ASSERT(phb != NULL);
	memset(phb, 0x00, sizeof(mmi_phb_get_entry_by_index_rsp_struct));

	phb->result.flag = L4C_OK;
	phb->no_list = nParam2;

	for (i = 0; i < nParam2 && i < MAX_SIM_ENTRIES; i++)
	{
		phb->list[i].storage = MMI_SIM;

		entryInfo = (CFW_SIM_PBK_ENTRY_INFO *)(nParam1 + (sizeof(CFW_SIM_PBK_ENTRY_INFO) + SIM_PBK_EXTR_SIZE) * i);

		phb->list[i].index = entryInfo->phoneIndex;
		phb->list[i].record_index = entryInfo->phoneIndex;

		if (entryInfo->pFullName[0] == 0x80)
		{
			phb->list[i].alpha_id.name_dcs = MMI_PHB_UCS2;
			memcpy(phb->list[i].alpha_id.name, entryInfo->pFullName + 1, entryInfo->iFullNameSize - 1);
			phb->list[i].alpha_id.name[entryInfo->iFullNameSize - 1] = '\0';
			phb->list[i].alpha_id.name[entryInfo->iFullNameSize] = '\0';
			phb->list[i].alpha_id.name_length = entryInfo->iFullNameSize - 1;
		}
		else
		{
			phb->list[i].alpha_id.name_dcs = MMI_PHB_ASCII;
			memcpy(phb->list[i].alpha_id.name, entryInfo->pFullName, entryInfo->iFullNameSize);
			phb->list[i].alpha_id.name[entryInfo->iFullNameSize] = '\0';
			phb->list[i].alpha_id.name_length = entryInfo->iFullNameSize;
		}

		if (entryInfo->nType == 129 || entryInfo->nType == 161)
			phb->list[i].tel.type = MMI_CSMCC_DEFAULT_ADDR_TYPE;
		else
			phb->list[i].tel.type = MMI_CSMCC_INTERNATIONAL_ADDR;
		phb->list[i].tel.length = SUL_GsmBcdToAscii(entryInfo->pNumber, entryInfo->nNumberSize, phb->list[i].tel.number);
	}

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = ADP_PHB_MSG_SRC_MOD;
	ilm_ptr.msg_id = PRT_PHB_GET_ENTRY_BY_INDEX_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)phb;
	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************
	FUNCTION NAME		: adp_phb_smu_read_file_info_rsp
	PURPOSE			       : respond PRT_MMI_SMU_READ_FILE_INFO_RSP.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void adp_phb_smu_read_file_info_rsp(UINT16 num_of_rec)
{
	ilm_struct  ilm_ptr;
	mmi_smu_read_file_info_rsp_struct *phb = OslMalloc(sizeof(mmi_smu_read_file_info_rsp_struct));
	ASSERT(phb != NULL);
	memset(phb, 0x00, sizeof(mmi_smu_read_file_info_rsp_struct));

	phb->num_of_rec = num_of_rec;
	if (num_of_rec > 0)
		phb->result.flag = L4C_OK;
	else
		phb->result.flag = L4C_ERROR;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = ADP_PHB_MSG_SRC_MOD;
	ilm_ptr.msg_id = PRT_MMI_SMU_READ_FILE_INFO_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)phb;
	OslMsgSendExtQueue(&ilm_ptr);
	mmi_trace(g_sw_ADP_PHB, TSTXT("adp_phb_smu_read_file_info_rsp!\n"));
}

/**************************************************************
	FUNCTION NAME		: adp_phb_verify_pin_ok_rsp
	PURPOSE			       : respond PRT_MMI_SMU_VERIFY_PIN_RSP.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void adp_phb_verify_pin_ok_rsp(void)
{
	ilm_struct  ilm_ptr;
	mmi_smu_verify_pin_rsp_struct *phb = OslMalloc(sizeof(mmi_smu_verify_pin_rsp_struct));
	ASSERT(phb != NULL);
	memset(phb, 0x00, sizeof(mmi_smu_verify_pin_rsp_struct));

	phb->next_type = MMI_SETTING_TYPE_NO_REQUIRED;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = PRT_MMI_SMU_VERIFY_PIN_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)phb;
	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************
	FUNCTION NAME		: adp_phb_verify_pin_error_rsp
	PURPOSE			       : respond PRT_MMI_SMU_VERIFY_PIN_RSP.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void adp_phb_verify_pin_error_rsp(UINT32 nParam1)
{
	ilm_struct  ilm_ptr;
	mmi_smu_verify_pin_rsp_struct *phb = OslMalloc(sizeof(mmi_smu_verify_pin_rsp_struct));
	ASSERT(phb != NULL);
	memset(phb, 0x00, sizeof(mmi_smu_verify_pin_rsp_struct));

	if (nParam1 == ERR_CME_SIM_PIN_REQUIRED)
		phb->next_type = MMI_SETTING_TYPE_CHV1;
	else if (nParam1 == ERR_CME_SIM_PUK_REQUIRED)
		phb->next_type = MMI_SETTING_TYPE_UBCHV1;
	else if (nParam1 == ERR_CME_SIM_PIN2_REQUIRED)
		phb->next_type = MMI_SETTING_TYPE_CHV2;
	else if (nParam1 == ERR_CME_SIM_PUK2_REQUIRED)
		phb->next_type = MMI_SETTING_TYPE_UBCHV2;
	else
		phb->next_type = MMI_SETTING_TYPE_UNSPECIFIED;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = PRT_MMI_SMU_VERIFY_PIN_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)phb;
	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************
	FUNCTION NAME		: adp_phb_verify_pin_ok_rsp
	PURPOSE			       : respond PRT_MMI_SMU_SET_DIAL_MODE_RSP.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void adp_phb_set_dial_mode_ok_rsp(void)
{
	ilm_struct  ilm_ptr;
	mmi_smu_set_dial_mode_rsp_struct *phb = OslMalloc(sizeof(mmi_smu_set_dial_mode_rsp_struct));
	ASSERT(phb != NULL);
	memset(phb, 0x00, sizeof(mmi_smu_set_dial_mode_rsp_struct));

	phb->result = 1;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = PRT_MMI_SMU_SET_DIAL_MODE_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)phb;
	OslMsgSendExtQueue(&ilm_ptr);	
}

/**************************************************************
	FUNCTION NAME		: adp_phb_set_dial_mode_error_rsp
	PURPOSE			       : respond PRT_MMI_SMU_SET_DIAL_MODE_RSP.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void adp_phb_set_dial_mode_error_rsp(void)
{
	ilm_struct  ilm_ptr;
	mmi_smu_set_dial_mode_rsp_struct *phb = OslMalloc(sizeof(mmi_smu_set_dial_mode_rsp_struct));
	ASSERT(phb != NULL);
	memset(phb, 0x00, sizeof(mmi_smu_set_dial_mode_rsp_struct));

	phb->result = 0;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = PRT_MMI_SMU_SET_DIAL_MODE_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)phb;
	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************
	FUNCTION NAME		: SendSmuEventToMMI
	PURPOSE			    : Send the smu event to mmi layer
	INPUT PARAMETERS	: pMessageStruct, messageID
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS			    :
**************************************************************/
static void SendSmuEventToMMI(local_para_struct *pMessageStruct, msg_type_t messageID)
{
	ilm_struct  ilm_ptr;

	ilm_ptr.local_para_ptr = pMessageStruct;
	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = ADP_SIM_MSG_SRC_MOD;
	ilm_ptr.msg_id = messageID;
	ilm_ptr.sap_id = MM_SIM_SAP;
	ilm_ptr.peer_buff_ptr = NULL;

	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************

	FUNCTION NAME		: AdaptNETSETGetPreferredNetworkReq

	PURPOSE				: get preferred network list

	PARAMETERS	              :

	REMARKS			:
**************************************************************/
UINT8 AdaptNETSETGetPreferredNetworkReq(COS_EVENT *pCosEvent)
{
	ASSERT(NULL != pCosEvent);


#ifdef __ADP_MULTI_SIM__
	mmi_trace(g_sw_ADP_NW, ">>>> entry AdaptNETSETGetPreferredNetworkReq >>>>for sim%d", g_pSIMContext->sim_id);

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	if (ERR_SUCCESS != UT_CFW_SimGetPrefOperatorList(FREE_UTI, g_pSIMContext->sim_id))
#else
	if (ERR_SUCCESS != CFW_SimGetPrefOperatorList(FREE_UTI, g_pSIMContext->sim_id))
#endif
#else
	if (ERR_SUCCESS != CFW_SimGetPrefOperatorList(FREE_UTI))
#endif
	{
		mmi_smu_get_preferred_oper_list_rsp_struct *GetPrefListRsp = NULL;
		GetPrefListRsp = (mmi_smu_get_preferred_oper_list_rsp_struct *)OslMalloc(sizeof(mmi_smu_get_preferred_oper_list_rsp_struct));
		ASSERT(NULL != GetPrefListRsp);
		memset(GetPrefListRsp, 0, sizeof(mmi_smu_get_preferred_oper_list_rsp_struct));
		GetPrefListRsp->result = MMI_FALSE;
		SendSmuEventToMMI((local_para_struct *)GetPrefListRsp, PRT_PREFERRED_NETWORKS_REQ_SUCCESS);
	}

	return 0;
}

/**************************************************************

	FUNCTION NAME		: DoNETSETGetPreferredNetworkRsp

	PURPOSE				: send message PRT_PREFERRED_NETWORKS_REQ_SUCCESS to mmi

	PARAMETERS	              :

	REMARKS			:
**************************************************************/

UINT8 DoNETSETGetPreferredNetworkRsp(COS_EVENT *pCosEvent)
{
	UINT8 nType;
	UINT8 num;
	UINT8 *CswPreNetwork = NULL;
	mmi_smu_get_preferred_oper_list_rsp_struct *GetPrefListRsp = NULL;

	ASSERT(NULL != pCosEvent);
#ifdef __ADP_MULTI_SIM__	
	mmi_trace(g_sw_ADP_NW, ">>>> entry DoNETSETGetPreferredNetworkRsp>>>>for sim%d", g_pSIMContext->sim_id);
#endif

	GetPrefListRsp = (mmi_smu_get_preferred_oper_list_rsp_struct *)OslMalloc(sizeof(mmi_smu_get_preferred_oper_list_rsp_struct));
	ASSERT(NULL != GetPrefListRsp);
	memset(GetPrefListRsp, 0, sizeof(mmi_smu_get_preferred_oper_list_rsp_struct));

	nType = HIUINT8(pCosEvent->nParam3);
	mmi_trace(g_sw_ADP_NW, ">>>> pCosEvent:%d,%d,%d,%d>>>>\r\n", pCosEvent->nEventId, pCosEvent->nParam1, pCosEvent->nParam2, pCosEvent->nParam3);

	if (0 == nType)
	{
		GetPrefListRsp->result = MMI_TRUE;
		GetPrefListRsp->retnum = (UINT16)pCosEvent->nParam2;   //csw have question man
		//GetPrefListRsp->retnum = PrefOperListLen; // man
		CswPreNetwork = (UINT8 *)pCosEvent->nParam1;
		PrefOperListLen = pCosEvent->nParam2;//for return csw value is error
		memcpy(PrefOperList, CswPreNetwork, pCosEvent->nParam2*MAX_PLMN_LEN);

		for (num = 0; num < pCosEvent->nParam2; num++)
		{
			CswPLMN2CSDPLMN(&(GetPrefListRsp->oper_list[num*7]), CswPreNetwork + num*MAX_PLMN_LEN, MAX_PLMN_LEN);
		}
	}
	else if (nType == 0xF0)
	{
		GetPrefListRsp->result = MMI_FALSE;
	}
	else
		ASSERT(0);
	SendSmuEventToMMI((local_para_struct *)GetPrefListRsp, PRT_PREFERRED_NETWORKS_REQ_SUCCESS);

	return 0;
}
/**************************************************************

	FUNCTION NAME		: AdaptNETSETDeletePrefNetworkReq

	PURPOSE				: delete the preffered PLMN

	PARAMETERS	              :

	REMARKS			:
**************************************************************/

UINT8 AdaptNETSETAddorDeletePrefNetworkReq(COS_EVENT* pCosEvent)
{
	ilm_struct *OslMsg = NULL;
	mmi_smu_set_preferred_oper_list_req_struct  *SetPreOperListReq = NULL;
	INT8 num = 0;//man
	UINT8 ToAddPLMN[MAX_PLMN_LEN] = {0};
	UINT8 ret;

	ASSERT(NULL != pCosEvent);
	mmi_trace(g_sw_ADP_NW, ">>>> entry AdaptNETSETAddorDeletePrefNetworkReq>>>>\r\n");
	TBM_ENTRY(0x29dd);
	OslMsg = (ilm_struct *)pCosEvent->nParam1;
	SetPreOperListReq = (mmi_smu_set_preferred_oper_list_req_struct *)OslMsg->local_para_ptr;
	mmi_trace(g_sw_ADP_NW, ">>>> SetPreOperListReq->opcode =%d>>>>\r\n", SetPreOperListReq->opcode);

	if (SetPreOperListReq->opcode == SMU_REMOVE_PLMNSEL)
	{
		mmi_trace(g_sw_ADP_NW, ">>>> meet SetPreOperListReq->opcode == SMU_REMOVE_PLMNSEL>>>>\r\n");

		for (num = SetPreOperListReq->index; num < PrefOperListLen - 1; num++)
		{
			memcpy(&(PrefOperList[num*MAX_PLMN_LEN]), &(PrefOperList[(num+1)*MAX_PLMN_LEN]), MAX_PLMN_LEN);
		}

		PrefOperListLen = PrefOperListLen - 1;
	}
	else if (SetPreOperListReq->opcode == SMU_ADD_PLMNSEL)
	{

		mmi_trace(g_sw_ADP_NW, ">>>> entry AdaptNETSETDeletePrefNetworkReq CSDPLMN2CswPLMN>>>>\r\n");

		CSDPLMN2CswPLMN(ToAddPLMN, SetPreOperListReq->oper, MAX_PLMN_LEN);
		mmi_trace(g_sw_ADP_NW, ">>>> PrefOperListLen =%d>>>>\r\n", PrefOperListLen);
		mmi_trace(g_sw_ADP_NW, ">>>> SetPreOperListReq->index =%d>>>>\r\n", SetPreOperListReq->index);

		if (PrefOperListLen == 0)
		{
			mmi_trace(g_sw_ADP_NW, ">>>> entry PrefOperListLen == 0>>>>\r\n");
			memcpy(&(PrefOperList[0]), ToAddPLMN, MAX_PLMN_LEN);
		}
		else
		{
			mmi_trace(g_sw_ADP_NW, ">>>> entry PrefOperListLen != 0>>>>\r\n");

			for (num = PrefOperListLen - 1; num  >= SetPreOperListReq->index; num--)
			{
				memcpy(&(PrefOperList[(num + 1)*MAX_PLMN_LEN]), &(PrefOperList[num*MAX_PLMN_LEN]), MAX_PLMN_LEN);
			}

			memcpy(&(PrefOperList[SetPreOperListReq->index*MAX_PLMN_LEN]), ToAddPLMN, MAX_PLMN_LEN);
		}

		PrefOperListLen = PrefOperListLen + 1;
	}

#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	ret = UT_CFW_SimSetPrefOperatorList(PrefOperList, PrefOperListLen * MAX_PLMN_LEN, FREE_UTI, g_pSIMContext->sim_id);

#else
	ret = CFW_SimSetPrefOperatorList(PrefOperList, PrefOperListLen * MAX_PLMN_LEN, FREE_UTI, g_pSIMContext->sim_id);

#endif
#else
	ret = CFW_SimSetPrefOperatorList(PrefOperList, PrefOperListLen * MAX_PLMN_LEN, FREE_UTI);

#endif

	if (ERR_SUCCESS != ret)
	{
		mmi_smu_set_preferred_oper_list_rsp_struct *SetPrefListRsp;
		SetPrefListRsp = (mmi_smu_set_preferred_oper_list_rsp_struct *)OslMalloc(sizeof(mmi_smu_set_preferred_oper_list_rsp_struct));
		ASSERT(NULL != SetPrefListRsp);
		memset(SetPrefListRsp, 0, sizeof(mmi_smu_set_preferred_oper_list_rsp_struct));
		SetPrefListRsp->result = MMI_FALSE;
		SendSmuEventToMMI((local_para_struct *)SetPrefListRsp, PRT_SET_PREFERRED_NETWORKS_RSP);
	}

	TBM_EXIT(0x29dd);

	return 0;
}

/**************************************************************

	FUNCTION NAME		: DoNETSETDeletePrefNetworkRsp

	PURPOSE				: send PRT_SET_PREFERRED_NETWORKS_RSP to mmi

	PARAMETERS	              :

	REMARKS			:
**************************************************************/
UINT8 DoNETSETAddorDeletePrefNetworkRsp(COS_EVENT* pCosEvent)
{
	UINT8 nType;
	mmi_smu_set_preferred_oper_list_rsp_struct *SetPrefListRsp;

	TBM_ENTRY(0x29de);
	ASSERT(NULL != pCosEvent);
	mmi_trace(g_sw_ADP_NW, ">>>> entry DoNETSETAddorDeletePrefNetworkRsp>>>>\r\n");
	SetPrefListRsp = (mmi_smu_set_preferred_oper_list_rsp_struct *)OslMalloc(sizeof(mmi_smu_set_preferred_oper_list_rsp_struct));
	ASSERT(NULL != SetPrefListRsp);
	memset(SetPrefListRsp, 0, sizeof(mmi_smu_set_preferred_oper_list_rsp_struct));

	nType = HIUINT8(pCosEvent->nParam3);

	if (0 == nType)
	{
		mmi_trace(g_sw_ADP_NW, ">>>> return  0 == nType in DoNETSETAddorDeletePrefNetworkRsp>>>>\r\n");
		SetPrefListRsp->result = MMI_TRUE;
	}
	else if (0xF0 == nType)
	{
		mmi_trace(g_sw_ADP_NW, ">>>> return  0xF0 == nType in DoNETSETAddorDeletePrefNetworkRsp>>>>\r\n");
		SetPrefListRsp->result = MMI_FALSE;
	}
	else
		ASSERT(0);
	
	SendSmuEventToMMI((local_para_struct *)SetPrefListRsp, PRT_SET_PREFERRED_NETWORKS_RSP);

	TBM_EXIT(0x29de);

	return 0;
}


/**************************************************************

	FUNCTION NAME		: DoNumberOfPLMNSuported

	PURPOSE				: send message PRT_PLMN_LIST_IND to mmi

	PARAMETERS	              :

	REMARKS			:display time when time zone changes

**************************************************************/

UINT8 DoNumberOfPLMNSuported(COS_EVENT *pCosEvent)
{
	MMISMUSUPPORTPLMNLISTIND *NoOfPLMNList;

	ASSERT(NULL != pCosEvent);
	mmi_trace(g_sw_ADP_NW, ">>>> entry DoNumberOfPLMNSuported >>>>\r\n");

	NoOfPLMNList = (MMISMUSUPPORTPLMNLISTIND *)OslMalloc(sizeof(MMISMUSUPPORTPLMNLISTIND));
	ASSERT(NULL != NoOfPLMNList);
	memset(NoOfPLMNList, 0, sizeof(MMISMUSUPPORTPLMNLISTIND));

	NoOfPLMNList->length = (UINT16)pCosEvent->nParam1;
	SendSmuEventToMMI((local_para_struct *)NoOfPLMNList, PRT_PLMN_LIST_IND);
	mmi_trace(g_sw_ADP_NW, ">>>> NoOfPLMNList->length = %d >>>>\r\n", NoOfPLMNList->length);

	return 0;
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchSimReqMsg

  	PURPOSE				: Message Adaption for SIM  REQ

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/
extern pwr_context_struct  g_pwr_context[] ;/*wangrui add 2008.11.27 for fixbug10388  */

// TODO: use the formal API provided by PX.
#define GET_TARGET_MOD() (MOD_L4C_2)

BOOL ADP_DispatchSimReqMsg(COS_EVENT *pCosEvent)
{
	ilm_struct *oslMessage = (ilm_struct *)pCosEvent->nParam1;
	UINT32 RtnValue = ERR_SUCCESS;
	UINT16 error = 0;
	UINT32 nResult = 0;
	
	ASSERT(pCosEvent != NULL);
	ASSERT(oslMessage != NULL);
	
	TBM_ENTRY(0x29e0);

#ifdef __ADP_MULTI_SIM__
	{
		U8 nReqSimID = adp_GetMMISimID(ADP_GET_MMI_DEST_MOD(pCosEvent->nParam3));
		adp_ConfigureContext(ADP_MODULE_SIM, nReqSimID);
	}
#endif

	switch (oslMessage->msg_id)
	{

	case PRT_MMI_SMU_GET_DIAL_MODE_REQ:
		{
		#ifdef __ADP_MULTI_SIM__
			#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_SimGetFacilityLock(CFW_STY_FAC_TYPE_FD, FREE_UTI, g_pSIMContext->sim_id);
			#else
			U8 Uti = FREE_UTI;
			U8 nSimId = g_pSIMContext->sim_id;
			#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
			Simu_Rsp_Marker_Struct simuRspMarker;
			simuRspMarker.msgID = EV_CFW_SIM_GET_FACILITY_LOCK_RSP;
			simuRspMarker.nSimID = nSimId;
			simuRspMarker.Uti = Uti;
			simuRspMarker.pNext = NULL;
			adp_simu_add_Rsp_Marker(&simuRspMarker);
			if(nSimId > ADP_SIM_2)
				nSimId -= 2;
			#endif
			g_b_fdn_state_req[g_pSIMContext->sim_id] = TRUE;
            
			RtnValue = CFW_SimGetFacilityLock(CFW_STY_FAC_TYPE_FD, Uti, nSimId);
			#endif
		#else
			RtnValue = CFW_SimGetFacilityLock(CFW_STY_FAC_TYPE_FD, FREE_UTI);
		#endif
			if (RtnValue != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_PHB, "PHB FDL PRT_MMI_SMU_GET_DIAL_MODE_REQ RtnValue = %x", RtnValue);
			}
		}

		break;

	case MSG_ID_MMI_SMU_VERIFY_PIN_REQ:
	{

		mmi_smu_verify_pin_req_struct *local_data = NULL;
		UINT8 nPinSize = 0;
		UINT8 nNewPinSize = 0;
		UINT8 nPostion = 0;
		UINT16 len;
		
		local_data = (mmi_smu_verify_pin_req_struct *)oslMessage->local_para_ptr;
		nPinSize = strlen(local_data->pin);
		nNewPinSize = strlen(local_data->new_pin);

			mmi_trace(g_sw_ADP_SMU, "MMI_SIM: MSG_ID_MMI_SMU_VERIFY_PIN_REQ, local_data->pin_type = %d",local_data->pin_type);

			if(local_data->pin_type == MMI_SETTING_TYPE_CHV1 || local_data->pin_type == MMI_SETTING_TYPE_UBCHV1) //verify the puk1 in the power on process
			{
				TRACE_SMU_FUNC("PUK1_LOCK");
				SetMsgFlag(PUK1_LOCK);
				nPostion = 1;
			}
			else if (local_data->pin_type == MMI_SETTING_TYPE_CHV2 || local_data->pin_type == MMI_SETTING_TYPE_UBCHV2) //verify the puk2 not the power on precess
			{
				SetMsgFlag(PUK2_LOCK);
				nPostion = 2;
			}
			else if (local_data->pin_type == MMI_SETTING_TYPE_PHONELOCK) //for phone lock not the power on process
			{
				char pTempBuff[MAX_SIM_SETTING_PASSWD_LEN + 1] ={0};
				mmi_smu_verify_pin_rsp_struct *VerifyPhoneRsp;

				//TBM780 hansl 2007/6/15 B
				//memset(pTempBuff, 0, sizeof(pTempBuff)/sizeof(char));	//man
				ReadRecord(NVRAM_PHONELOCK_NVRAM_LID, 1, (void *)pTempBuff, (MAX_SIM_SETTING_PASSWD_LEN + 1), &error);
				#if 0
				hFile = FS_Open("c:/phonelock.dat", FS_O_RDWR, 0);
				FS_Seek(hFile, 0, FS_SEEK_SET);
				FS_Read(hFile, pTempBuff, (MAX_SIM_SETTING_PASSWD_LEN + 1));
				#endif
				//TBM780 hansl 2007/6/15 E
				VerifyPhoneRsp = (mmi_smu_verify_pin_rsp_struct*)OslMalloc(sizeof(mmi_smu_verify_pin_rsp_struct));				   
				ASSERT(VerifyPhoneRsp != NULL);
				memset(VerifyPhoneRsp, 0, sizeof(mmi_smu_verify_pin_rsp_struct));	
				len = strlen(pTempBuff);
				if(len < strlen(local_data->pin))
				{
					len = strlen(local_data->pin);
				}
				if (memcmp(pTempBuff, local_data->pin, len) == 0)
				{
					mmi_trace(g_sw_ADP_SMU, "MMI_SIM: >>>> verify phonelock success>>>>\r\n");
					VerifyPhoneRsp->next_type = MMI_SETTING_TYPE_NO_REQUIRED;
					VerifyPhoneRsp->result = 1;
					g_pwr_context[0].PhoneLockProcesing = FALSE;/*wangrui add 2008.11.27 for fixbug10388  */
				}
				else
				{
					mmi_trace(g_sw_ADP_SMU, "MMI_SIM: >>>> verify phonelock fail>>>>\r\n");
					VerifyPhoneRsp->next_type = MMI_SETTING_TYPE_PHONELOCK;
					VerifyPhoneRsp->result = 0;
				}
				
				//FS_Close(hFile);
				SendSmuEventToMMI((local_para_struct*) VerifyPhoneRsp, PRT_MMI_SMU_VERIFY_PIN_RSP);
				TBM_EXIT(0x29e0);
				return TRUE;

		}
		else if (local_data->pin_type == MMI_TYPE_UNSPECIFIED) //verify the pin1 in the power on process
		{
			SetMsgFlag(VERIFY_PIN1);
			nPostion = 1;
			nNewPinSize = 0;
		#ifdef __ADP_MULTI_SIM__
			#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_SimEnterAuthentication(local_data->pin, nPinSize, NULL, 0, nPostion, FREE_UTI,
									   g_pSIMContext->sim_id);
			#else
			RtnValue = CFW_SimEnterAuthentication(local_data->pin, nPinSize, NULL, 0, nPostion, FREE_UTI,
									   g_pSIMContext->sim_id);		
			#endif
		#else
			RtnValue = CFW_SimEnterAuthentication(local_data->pin, nPinSize, NULL, 0, nPostion, FREE_UTI);
		#endif
			if(RtnValue != ERR_SUCCESS)
				mmi_trace(g_sw_ADP_SMU, "MMI_SIM: call CFW_SimEnterAuthentication to verify pin failed with ret: 0x%x", RtnValue);
			else
				mmi_trace(g_sw_ADP_SMU, "MMI_SIM: call CFW_SimEnterAuthentication to verify pin OK");
			TBM_EXIT(0x29e0);
			return TRUE;
		}
		else if (local_data->pin_type == MMI_TYPE_UBCHV1) //verify the pin1 or puk1 not the power on process
		{
			mmi_trace(g_sw_ADP_SMU,"MMI_SIM: local_data->pin_type == MMI_TYPE_UBCHV1");
			SetMsgFlag(VERIFY_PUK1);
			nPostion = 1;
		}

		if (local_data->pin_type == MMI_SETTING_TYPE_CHV2)
			memcpy(PIN2, local_data->pin, MAX_SIM_PASSWD_LEN);
		else if (local_data->pin_type == MMI_SETTING_TYPE_UBCHV2)
			memcpy(PIN2, local_data->new_pin, MAX_SIM_PASSWD_LEN);

		TRACE_EF(g_sw_ADP_SMU, "The nPosition = %d", nPostion);

// Call new CFW APIs for dual-sim.
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		UT_CFW_SimEnterAuthentication(local_data->pin, nPinSize, local_data->new_pin, nNewPinSize, nPostion, FREE_UTI, GET_TARGET_MOD());

#else

		RtnValue = CFW_SimEnterAuthentication(local_data->pin, nPinSize, local_data->new_pin, nNewPinSize, nPostion, FREE_UTI, g_pSIMContext->sim_id);
		
#endif
#else
		RtnValue = CFW_SimEnterAuthentication(local_data->pin, nPinSize, local_data->new_pin, nNewPinSize, nPostion, FREE_UTI);

#endif
		mmi_trace(g_sw_ADP_SMU, "MMI_SIM:(WARN)Call CFW_SimEnterAuthentication(), =%u", RtnValue);
	}

	break;

		case PRT_MMI_SMU_SET_DIAL_MODE_REQ:
		{
			mmi_smu_set_dial_mode_req_struct *local_data;
			UINT8 nPwdSize ;

			TRACE_SMU_FUNC("PRT_MMI_SMU_SET_DIAL_MODE_REQ");
			SetMsgFlag(PHB_DAIL_MODE);
			
			local_data = NULL;
			nPwdSize = 0;

		local_data = (mmi_smu_set_dial_mode_req_struct *)oslMessage->local_para_ptr;
		nPwdSize = strlen(PIN2);

		if (local_data->type == 0x4)//MMI_TYPE_FDN
		{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_SimSetFacilityLock(CFW_STY_FAC_TYPE_FD, PIN2, nPwdSize, local_data->on_off, FREE_UTI, g_pSIMContext->sim_id);
#else
			RtnValue = CFW_SimSetFacilityLock(CFW_STY_FAC_TYPE_FD, PIN2, nPwdSize, local_data->on_off, FREE_UTI, g_pSIMContext->sim_id);
#endif
#else
			RtnValue = CFW_SimSetFacilityLock(CFW_STY_FAC_TYPE_FD, PIN2, nPwdSize, local_data->on_off, FREE_UTI);
#endif
		}

		mmi_trace(g_sw_ADP_PHB, TSTXT("PRT_MMI_SMU_SET_DIAL_MODE_REQ RtnValue:%x !\n"), RtnValue);

		mmi_trace(g_sw_ADP_PHB, TSTXT("PRT_MMI_SMU_SET_DIAL_MODE_REQ pin:%s,mode:%d,on_off !\n"), PIN2, local_data->on_off);
	}

	break;

	case PRT_MMI_SMU_READ_FILE_INFO_REQ:
	{
    mmi_smu_read_file_info_req_struct *local_data = NULL;

#ifdef __ADP_MULTI_SIM__
		adp_ConfigureContext(ADP_MODULE_PHB, g_pSIMContext->sim_id);
#endif

		local_data = (mmi_smu_read_file_info_req_struct *)oslMessage->local_para_ptr;

		adp_pPHBContext->flag_list_FDN = FALSE;

		if (local_data->file_idx == 70)//fdn...FILE_FDN_IDX
		{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_SimGetPbkStorage(CFW_PBK_SIM_FIX_DIALLING, FREE_UTI,  adp_pPHBContext->nSimId);
#else
			RtnValue = CFW_SimGetPbkStorage(CFW_PBK_SIM_FIX_DIALLING, FREE_UTI,  adp_pPHBContext->nSimId);
#endif
#else
			RtnValue = CFW_SimGetPbkStorage(CFW_PBK_SIM_FIX_DIALLING, FREE_UTI);
#endif
		}
		else if (local_data->file_idx == 82)
		{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_SimGetPbkStorage(CFW_PBK_SIM_BARRED_DIALLING, FREE_UTI, adp_pPHBContext->nSimId);
#else
			RtnValue = CFW_SimGetPbkStorage(CFW_PBK_SIM_BARRED_DIALLING, FREE_UTI, adp_pPHBContext->nSimId);
#endif
#else
			RtnValue = CFW_SimGetPbkStorage(CFW_PBK_SIM_BARRED_DIALLING, FREE_UTI);
#endif
		}
		else
			ASSERT(0);

		mmi_trace(g_sw_ADP_PHB, "PHB FDN smu read req local_data->file_idx = %d, CFW_SimGetPbkStorage = %d", local_data->file_idx, RtnValue);
	}

	break;

	case PRT_MMI_SMU_CHECK_PIN_STATUS_REQ:
	{
		TRACE_SMU_FUNC("PRT_MMI_SMU_CHECK_PIN_STATUS_REQ");

		if (GetMsgFlag() == CHECK_PIN1)
		{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			UT_CFW_SimGetFacilityLock(CFW_STY_FAC_TYPE_SC, FREE_UTI, g_pSIMContext->sim_id);
#else
			CFW_SimGetFacilityLock(CFW_STY_FAC_TYPE_SC, FREE_UTI, g_pSIMContext->sim_id);
#endif
#else
			CFW_SimGetFacilityLock(CFW_STY_FAC_TYPE_SC, FREE_UTI);

#endif
		}
		else
		{
			SetMsgFlag(PHB_CHECK_PIN2); // this function should be moved to the MMI layer for perocessing

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			RtnValue = UT_CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
#else
			RtnValue = CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
#endif
#else
			RtnValue = CFW_SimGetAuthenticationStatus(FREE_UTI);

#endif
		}
	}

	break;

		case PRT_PREFERRED_NETWORKS_REQ:
			mmi_trace(g_sw_ADP_NW, ">>>> receive PRT_PREFERRED_NETWORKS_REQ>>>>\r\n");
			AdaptNETSETGetPreferredNetworkReq(pCosEvent);
			break;

		case PRT_SET_PREFERRED_NETWORKS_REQ:
			mmi_trace(g_sw_ADP_NW, ">>>> come here PRT_SET_PREFERRED_NETWORKS_REQ >>>>\n");
			AdaptNETSETAddorDeletePrefNetworkReq(pCosEvent);
			break;
			
		//add by panxu for the smu
		case PRT_GET_IMSI_REQ:
			{
			U32 r;
			mmi_smu_get_imsi_rsp_struct *pMMIRsp;
			TRACE_SMU_FUNC("PRT_GET_IMSI_REQ");
			//modified by zhoumn for bug 10437 @2007/04/06<<<start
			#if 0
			if (ERR_SUCCESS != CFW_SimGetProviderId(CFW_SIM_IND_UTI))
			{
				mmi_smu_get_imsi_rsp_struct *pMMIRsp =
					(mmi_smu_get_imsi_rsp_struct*)OslMalloc(sizeof(mmi_smu_get_imsi_rsp_struct));
				ASSERT(NULL != pMMIRsp);
				memset(pMMIRsp, 0, sizeof(mmi_smu_get_imsi_rsp_struct));

				pMMIRsp->result = KAL_FALSE;

			SendSmuEventToMMI((local_para_struct*)pMMIRsp, PRT_GET_IMSI_RSP);
		}

		break;

#else

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT

		r = UT_CFW_SimGetProviderId(FREE_UTI, g_pSIMContext->sim_id);
#else
		r = CFW_SimGetProviderId(FREE_UTI, g_pSIMContext->sim_id);
#endif
#else
		r = CFW_SimGetProviderId(FREE_UTI);

#endif
		mmi_trace(g_sw_ADP_SMU, ">>>>>>UTI parameter of CFW_SimGetProviderId() is 0x%x\n", FREE_UTI);

		if (ERR_SUCCESS != r)
		{
			mmi_trace(g_sw_ADP_SMU, ">>>>>>>>>>result of CFW_SimGetProviderId() is 0x%x\n", r);
			pMMIRsp =
				(mmi_smu_get_imsi_rsp_struct*)OslMalloc(sizeof(mmi_smu_get_imsi_rsp_struct));
			ASSERT(NULL != pMMIRsp);
			memset(pMMIRsp, 0, sizeof(mmi_smu_get_imsi_rsp_struct));
			pMMIRsp->result = KAL_FALSE;
			SendSmuEventToMMI((local_para_struct*)pMMIRsp, PRT_GET_IMSI_RSP);
		}
	}

	break;
#endif
		//modified by zhoumn for bug 10437 @2007/04/06>>>end

		case PRT_MMI_SMU_LOCK_REQ:
			{
			mmi_smu_lock_req_struct *pMMIReq;
			mmi_smu_lock_rsp_struct *pMMIRsp;
			UINT8 nMode;
			UINT16 len ;

			TRACE_SMU_FUNC("PRT_MMI_SMU_LOCK_REQ");
			
			pMMIReq = (mmi_smu_lock_req_struct*)oslMessage->local_para_ptr;
			ASSERT(NULL != pMMIReq);

			nMode = 0;

			if (MMI_SETTING_TYPE_PHONELOCK == pMMIReq->type)
			{
			 
				char pTempBuff[MAX_SIM_SETTING_PASSWD_LEN + 1] = {0};
				mmi_smu_lock_rsp_struct *PhonelockRsp;
				//memset(pTempBuff, 0, sizeof(pTempBuff)/sizeof(char));	//man
			 	ReadRecord(NVRAM_PHONELOCK_NVRAM_LID, 1, (void *)pTempBuff, (MAX_SIM_SETTING_PASSWD_LEN + 1), &error);
      				mmi_trace(1, ">>>>pTempBuff = %s>>>>\r\n",pTempBuff);
        #if 0
				hFile = FS_Open("c:/phonelock.dat", FS_O_RDWR, 0);
				FS_Seek(hFile, 0, FS_SEEK_SET);
				FS_Read(hFile, pTempBuff, (MAX_SIM_SETTING_PASSWD_LEN + 1));
	#endif
		        	PhonelockRsp = (mmi_smu_lock_rsp_struct*)OslMalloc(sizeof(mmi_smu_lock_rsp_struct));				   
				ASSERT(PhonelockRsp != NULL);
				memset(PhonelockRsp, 0, sizeof(mmi_smu_lock_rsp_struct));	

				TRACE_EF(g_sw_SMU, "the pTempBuff = %s, pMMIReq->passwd = %s", pTempBuff, pMMIReq->passwd);
				mmi_trace(1,"pMMIReq->passwd = %s",pMMIReq->passwd);
				len = strlen(pTempBuff);
				if(len < strlen(pMMIReq->passwd))
				{
					len = strlen(pMMIReq->passwd);
				}
				if (memcmp(pTempBuff, pMMIReq->passwd, len) == 0)
				{
					
					PhonelockRsp->result = 1;
					PhonelockRsp->cause = MMI_SETTING_TYPE_PHONELOCK;
				}
				else
				{
					TRACE_SMU_FUNC("comapre not ok??");
					PhonelockRsp->result = 0;
					PhonelockRsp->cause = MMI_SETTING_TYPE_PHONELOCK;
				}
				
			//	FS_Close(hFile);
				SendSmuEventToMMI((local_para_struct*)PhonelockRsp, PRT_MMI_SMU_LOCK_RSP);
				TBM_EXIT(0x29e0);
				return TRUE;//man
			}
			else if (MMI_SETTING_TYPE_CHV1 == pMMIReq->type)
			{
				SetMsgFlag(PIN1_LOCK);
				nLockType = CFW_STY_FAC_TYPE_SC;
			}
			
			if (pMMIReq->opcode == DISABLE) 
			{
				nMode = 0;
			}
			else
			{
				nMode = 1;
			} 

			TRACE_EF(g_sw_ADP_SMU, "The mode is %d, The password is %s, %d, %d, %d, %d, the length = %d", nMode, pMMIReq->passwd, pMMIReq->passwd[0], pMMIReq->passwd[1], pMMIReq->passwd[2], pMMIReq->passwd[3], strlen(pMMIReq->passwd));

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			nResult = UT_CFW_SimSetFacilityLock(nLockType, pMMIReq->passwd, strlen(pMMIReq->passwd), nMode, FREE_UTI,
											 g_pSIMContext->sim_id);
#else
			nResult = CFW_SimSetFacilityLock(nLockType, pMMIReq->passwd, strlen(pMMIReq->passwd), nMode, FREE_UTI,
											 g_pSIMContext->sim_id);
#endif
#else
			nResult = CFW_SimSetFacilityLock(nLockType, pMMIReq->passwd, strlen(pMMIReq->passwd), nMode, FREE_UTI);
#endif

			if (ERR_SUCCESS != nResult && CFW_STY_FAC_TYPE_SC == nLockType)
			{
				TRACE_EF(g_sw_ADP_SMU, "NO successful the reason is %x ", nResult);
					
				pMMIRsp =
					(mmi_smu_lock_rsp_struct*)OslMalloc(sizeof(mmi_smu_lock_rsp_struct));
				ASSERT(NULL != pMMIRsp);
				memset(pMMIRsp, 0, sizeof(mmi_smu_lock_rsp_struct));

				pMMIRsp->result = FALSE;

				SendSmuEventToMMI((local_para_struct*)pMMIRsp, PRT_MMI_SMU_LOCK_RSP);
			}
			}
			break;

		case PRT_MMI_SMU_CHANGE_PASSWORD_REQ:
		{
			mmi_smu_change_password_req_struct *pPwdReq;
			UINT16 nPinType;
			TRACE_SMU_FUNC("PRT_MMI_SMU_CHANGE_PASSWORD_REQ");
			pPwdReq = 
				(mmi_smu_change_password_req_struct*)oslMessage->local_para_ptr;
			ASSERT(NULL != pPwdReq);

			nPinType = 0;
			
			if (MMI_SETTING_TYPE_CHV1 == pPwdReq->type)
			{
				nPinType = CFW_STY_FAC_TYPE_SC;
				SetMsgFlag(CHANGE_PIN1_PWD);
			}
			else if (MMI_SETTING_TYPE_CHV2 == pPwdReq->type)
			{
				nPinType = CFW_STY_FAC_TYPE_P2;
				SetMsgFlag(CHANGE_PIN2_PWD);
			}
			else if (MMI_SETTING_TYPE_PHONELOCK == pPwdReq->type)//change the phone lock password
			{
				//HANDLE hFile = NULL;
				char pTempBuff[MAX_SIM_SETTING_PASSWD_LEN + 1] = {0};
				UINT16 len;
			    mmi_cc_set_max_acm_rsp_struct *RSP_localPtr = OslMalloc(sizeof(mmi_cc_set_max_acm_rsp_struct));				   
				ASSERT(RSP_localPtr != NULL);
				memset(RSP_localPtr, 0, sizeof(mmi_cc_set_max_acm_rsp_struct));
				//memset(pTempBuff, 0, sizeof(pTempBuff)/sizeof(char));	//man
			  ReadRecord(NVRAM_PHONELOCK_NVRAM_LID, 1, (void *)pTempBuff, (MAX_SIM_SETTING_PASSWD_LEN + 1), &error);
        #if 0
				hFile = FS_Open("c:/phonelock.dat", FS_O_RDWR, 0);
				FS_Seek(hFile, 0, FS_SEEK_SET);
				FS_Read(hFile, pTempBuff, (MAX_SIM_SETTING_PASSWD_LEN + 1));		
        #endif
    
				TRACE_EF(g_sw_SMU, "pTempBuff = %s, pPwdReq->old = %s, pPwdReq->new1 = %s", pTempBuff, pPwdReq->old, pPwdReq->new1);
				 len = strlen(pTempBuff);
				if(len < strlen(pPwdReq->old))
				{
					len = strlen(pPwdReq->old);
				}
				if (memcmp(pTempBuff, pPwdReq->old, len) == 0)//man
				{
					mmi_trace(g_sw_ADP_NW, ">>>> compare success>>>>\r\n");
				//	FS_Seek(hFile, 0, FS_SEEK_SET);
				//	FS_Write(hFile, (PSTR)pPwdReq->new1, (MAX_SIM_SETTING_PASSWD_LEN + 1));
					WriteRecord(NVRAM_PHONELOCK_NVRAM_LID, 1, (void *)pPwdReq->new1, strlen(pPwdReq->new1) + 1, &error);
					RSP_localPtr->result = 1;
				}
				else
				{
					mmi_trace(g_sw_ADP_NW, ">>>> compare fail>>>>\r\n");
					RSP_localPtr->result = 0;
				}					

				// FS_Close(hFile);					
				SendSmuEventToMMI((local_para_struct*) RSP_localPtr, PRT_MMI_SMU_CHANGE_PASSWORD_RSP);
				TBM_EXIT(0x29e0);
				return TRUE;
			}

			TRACE_EF(g_sw_ADP_SMU, "type = %d, csw type = %x, old pwd = %s, new1 = %s, new2 = %s", pPwdReq->type, nPinType, pPwdReq->old, pPwdReq->new1, pPwdReq->new2);
			TRACE_EF(g_sw_ADP_SMU, "old len = %d, new len = %d", strlen(pPwdReq->old), strlen(pPwdReq->new1));

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			nResult = UT_CFW_SimChangePassword(nPinType, pPwdReq->old, strlen(pPwdReq->old),
											pPwdReq->new1, strlen(pPwdReq->new1), FREE_UTI, g_pSIMContext->sim_id);
#else
			nResult = CFW_SimChangePassword(nPinType, pPwdReq->old, strlen(pPwdReq->old),
											pPwdReq->new1, strlen(pPwdReq->new1), FREE_UTI, g_pSIMContext->sim_id);
#endif
#else

			nResult = CFW_SimChangePassword(nPinType, pPwdReq->old, strlen(pPwdReq->old),
											pPwdReq->new1, strlen(pPwdReq->new1), FREE_UTI);
#endif

			if (ERR_SUCCESS != nResult)
			{
				TRACE_EF(g_sw_ADP_SMU, "change password not success, the reason  = %0x", nResult);	
			}
		}
			break;
#ifndef MMI_ON_WIN32
		case PRT_MSG_ID_MMI_RESET_SIM_REQ:
			mmi_trace(g_sw_ADP_SMU, "ADP_SIM: call sim reset");
			#ifdef __ADP_MULTI_SIM__
			CFW_SimReset(FREE_UTI,g_pSIMContext->sim_id);
			#else
			CFW_SimReset(FREE_UTI);
			#endif
			break;
#endif
		default:
			TBM_EXIT(0x29e0);
			return FALSE;
	}

	if (RtnValue != ERR_SUCCESS)
		mmi_trace(g_sw_ADP_PHB, TSTXT("SIM req Error code: 0x%x\n"), RtnValue);
	TBM_EXIT(0x29e0);
	return TRUE;
}

	/*****************************************************************************
	 * FUNCTION
	 *  do_cswmsg_EV_CFW_SIM_GET_AUTH_STATUS_RSP
	 * DESCRIPTION
	 *  Process message EV_CFW_SIM_GET_AUTH_STATUS_RSP comes from CSW layer.
	 * PARAMETERS
	 *  pCosEvent: CSW message data structure.
	 * RETURNS
	 *  Returns true when it process this message sucessfully, else return FALSE.
	 *  You need to process this message in other place when it returns FALSE.
	 *****************************************************************************/
	static BOOL do_cswmsg_EV_CFW_SIM_GET_AUTH_STATUS_RSP(COS_EVENT *pCosEvent)
	{
		/*----------------------------------------------------------------*/
		/* Local Variables                                                */
		/*----------------------------------------------------------------*/
		message_type op = GetMsgFlag();
		UINT8 type = adp_GetCswEvType(pCosEvent);
		UINT32 p1 = pCosEvent->nParam1, p2 = pCosEvent->nParam2;

		/*----------------------------------------------------------------*/
		/* Code Body                                                      */
		/*----------------------------------------------------------------*/

		if (type != 0)
		{
			mmi_trace(g_sw_ADP_SMU, "MMI_SIM: EV_CFW_SIM_GET_AUTH_STATUS_RSP returns failed result. type=%d", type);
			// TODO: need more process when type equals 0xF0.
			return TRUE;
		}

		switch (op)
		{

		case CHANGE_PIN1_PWD:
		{
			mmi_smu_change_password_rsp_struct *pPwdRsp;

		MMI_MALLOC(pPwdRsp, mmi_smu_change_password_rsp_struct);

			pPwdRsp->chv_info.chv1_count = LOUINT16(p2) & 0x0F;
			pPwdRsp->chv_info.chv2_count = (LOUINT16(p2) & 0xF00) >> 8;;
			pPwdRsp->chv_info.ubchv1_count = (LOUINT16(p2) & 0xf0) >> 4;//man
			pPwdRsp->chv_info.ubchv2_count = (LOUINT16(p2) & 0xF000) >> 12;

		if (g_IsPinChanged)
				pPwdRsp->result = 1;
			else
			{
				pPwdRsp->result = 0;

				if (0 == pPwdRsp->chv_info.chv1_count)
				{
					pPwdRsp->cause = MMI_SETTING_TYPE_UBCHV1;
				}

			}

		mmi_trace(g_sw_ADP_SMU, "MMI_SIM: ADP send MSG_ID_MMI_SMU_CHANGE_PASSWORD_RSP to MMI");

		SendSmuEventToMMI((local_para_struct*) pPwdRsp, MSG_ID_MMI_SMU_CHANGE_PASSWORD_RSP);
		}

		break;

	case CHANGE_PIN2_PWD:
	{
		mmi_smu_change_password_rsp_struct *pPwdRsp = NULL;

		MMI_MALLOC(pPwdRsp, mmi_smu_change_password_rsp_struct);

		pPwdRsp->chv_info.chv1_count = LOUINT16(p2) & 0x0F;
		pPwdRsp->chv_info.chv2_count = (LOUINT16(p2) & 0xF00) >> 8;;
		pPwdRsp->chv_info.ubchv1_count = (LOUINT16(p2) & 0xf0) >> 4;
		pPwdRsp->chv_info.ubchv2_count = (LOUINT16(p2) & 0xF000) >> 12;

		if (g_IsPinChanged)
			pPwdRsp->result = 1;
		else
		{
			pPwdRsp->result = 0;

				if (0 == pPwdRsp->chv_info.chv2_count)
				{
					pPwdRsp->cause = MMI_SETTING_TYPE_UBCHV2;
				}

			}

		mmi_trace(g_sw_ADP_SMU, "MMI_SIM: ADP send MSG_ID_MMI_SMU_CHANGE_PASSWORD_RSP to MMI");
		SendSmuEventToMMI((local_para_struct*) pPwdRsp, PRT_MMI_SMU_CHANGE_PASSWORD_RSP);
	}

		break;

		case PIN1_LOCK:
	{
		mmi_smu_lock_rsp_struct *pMMIStatusRsp = NULL;

		MMI_MALLOC(pMMIStatusRsp, mmi_smu_lock_rsp_struct);

		pMMIStatusRsp->result = g_IsPinPassed;
		pMMIStatusRsp->chv_info.chv1_count = (LOUINT16(p2) & 0x0F);
		pMMIStatusRsp->chv_info.chv2_count = ((LOUINT16(p2) & 0xF00) >> 8);
		pMMIStatusRsp->chv_info.ubchv1_count = ((LOUINT16(p2) & 0xF0) >> 4);
		pMMIStatusRsp->chv_info.ubchv2_count = ((LOUINT16(p2) & 0xF000) >> 12);

		if (0 == pMMIStatusRsp->chv_info.chv1_count)
			pMMIStatusRsp->cause = MMI_SETTING_TYPE_UBCHV1;
		
		mmi_trace(g_sw_ADP_SMU, "MMI_SIM: ADP send PRT_MMI_SMU_LOCK_RSP to MMI");
		SendSmuEventToMMI((local_para_struct*)pMMIStatusRsp, PRT_MMI_SMU_LOCK_RSP);
	}

		break;

	case PUK1_LOCK:
	{
		mmi_smu_verify_pin_rsp_struct *pPinRsp = NULL;

		MMI_MALLOC(pPinRsp, mmi_smu_verify_pin_rsp_struct);

		pPinRsp->result = g_IsPukPassed;
		pPinRsp->chv_info.chv1_count = (LOUINT16(p2) & 0x0F);
		pPinRsp->chv_info.chv2_count = ((LOUINT16(p2) & 0xF00) >> 8);
		pPinRsp->chv_info.ubchv1_count = ((LOUINT16(p2) & 0xF0) >> 4);
		pPinRsp->chv_info.ubchv2_count = ((LOUINT16(p2) & 0xF000) >> 12);

		if (g_IsPukPassed)
		{
			pPinRsp->result = TRUE;
			pPinRsp->next_type = MMI_SETTING_TYPE_NO_REQUIRED;
		}
		else
		{
			pPinRsp->result = FALSE;

			if (0 == pPinRsp->chv_info.ubchv1_count)
			{
				pPinRsp->next_type = MMI_SETTING_TYPE_SIM_CARD_BLOCKED;
			}
			else
			{
				pPinRsp->next_type = MMI_SETTING_TYPE_UBCHV1;
			}
		}

		mmi_trace(g_sw_ADP_SMU, "MMI_SIM: ADP send MSG_ID_MMI_SMU_VERIFY_PIN_RSP to MMI");
		SendSmuEventToMMI((local_para_struct*)pPinRsp, MSG_ID_MMI_SMU_VERIFY_PIN_RSP);
	}

		break;

	case PHB_CHECK_PIN2:
	{
		ilm_struct  ilm_ptr;
		mmi_smu_check_pin_status_rsp_struct *phb = NULL;

		MMI_MALLOC(phb, mmi_smu_check_pin_status_rsp_struct);

		//BUG:
		phb->result = L4C_OK; ///????????????????????result==L4C???
			phb->chv_info.chv1_count = LOUINT16(p2) & 0x0F;
			phb->chv_info.ubchv1_count = (LOUINT16(p2) & 0x0F) >> 4;
			phb->chv_info.chv2_count = (LOUINT16(p2) & 0xF00) >> 8;
			phb->chv_info.ubchv2_count = (LOUINT16(p2) & 0xF000) >> 12;

			if (phb->chv_info.chv1_count > 0)
				phb->chv_status.chv1_status = CHV_VERIFIED;
			else if (phb->chv_info.ubchv1_count > 0)
				phb->chv_status.chv1_status = CHV_BLOCKED;
			else
				phb->chv_status.chv1_status = CHV_DEAD;

			if (phb->chv_info.chv2_count > 0)
				phb->chv_status.chv2_status = CHV_VERIFIED;
			else if (phb->chv_info.ubchv2_count > 0)
				phb->chv_status.chv2_status = CHV_BLOCKED;
			else
				phb->chv_status.chv2_status = CHV_DEAD;

		ilm_ptr.dest_mod_id = MOD_MMI;
		ilm_ptr.src_mod_id = ADP_SIM_MSG_SRC_MOD;
		ilm_ptr.msg_id = MSG_ID_MMI_SMU_CHECK_PIN_STATUS_RSP;
		ilm_ptr.local_para_ptr = (local_para_struct *)phb;

		mmi_trace(g_sw_ADP_SMU, "MMI_SIM: ADP send MSG_ID_MMI_SMU_CHECK_PIN_STATUS_RSP to MMI");
		OslMsgSendExtQueue(&ilm_ptr);
	}

		break;

		case VERIFY_PIN1:
		case VERIFY_PUK1:
		{
		mmi_smu_password_required_ind_struct *pVerifyPinRsp = NULL;
		
		MMI_MALLOC(pVerifyPinRsp, mmi_smu_password_required_ind_struct);
		pVerifyPinRsp->chv_info.chv1_count = (LOUINT16(p2) & 0x0F);
		pVerifyPinRsp->chv_info.chv2_count = ((LOUINT16(p2) & 0xF00) >> 8);
		pVerifyPinRsp->chv_info.ubchv1_count = ((LOUINT16(p2) & 0xF0) >> 4);
		pVerifyPinRsp->chv_info.ubchv2_count = ((LOUINT16(p2) & 0xF000) >> 12);

		if (p1 == CFW_STY_AUTH_READY || p1 == CFW_STY_AUTH_PIN1_READY)
		{
			mmi_trace(g_sw_ADP_SMU, "MMI_SIM: pin passed");
			pVerifyPinRsp->type = MMI_TYPE_CHV_PASSED;
		}
		else
		{
			if (0 != pVerifyPinRsp->chv_info.chv1_count)
			{
				mmi_trace(g_sw_ADP_SMU, "MMI_SIM: pin1 needs to be entried, count is: %d", pVerifyPinRsp->chv_info.chv1_count);
				pVerifyPinRsp->type = MMI_TYPE_CHV1;
			}
			else
			{
				if (0 != pVerifyPinRsp->chv_info.ubchv1_count)
				{
					mmi_trace(g_sw_ADP_SMU, "MMI_SIM: puk1 needs to be entried, count is: %d",pVerifyPinRsp->chv_info.ubchv1_count);
					pVerifyPinRsp->type = MMI_TYPE_UBCHV1;
				}
				else
				{
					mmi_trace(g_sw_ADP_SMU, "MMI_SIM: the sim card is locked!");
					pVerifyPinRsp->type = MMI_TYPE_SIM_CARD_BLOCKED;
				}
			}
		}

		mmi_trace(g_sw_ADP_SMU, "MMI_SIM: ADP send MSG_ID_MMI_SMU_PASSWORD_REQUIRED_IND to MMI");
		SendSmuEventToMMI((local_para_struct*)pVerifyPinRsp, MSG_ID_MMI_SMU_PASSWORD_REQUIRED_IND);
	}

		break;

	case PUK2_LOCK://man
	{
		//adp_phb_verify_pin_ok_rsp();
		ilm_struct  ilm_ptr;
		mmi_smu_verify_pin_rsp_struct *phb = NULL;

		MMI_MALLOC(phb, mmi_smu_verify_pin_rsp_struct);

		phb->result = g_IsPukPassed;
		phb->chv_info.chv1_count = LOUINT16(p2) & 0x0F;
		phb->chv_info.ubchv1_count = (LOUINT16(p2) & 0x0F) >> 4;
		phb->chv_info.chv2_count = (LOUINT16(p2) & 0xF00) >> 8;
		phb->chv_info.ubchv2_count = (LOUINT16(p2) & 0xF000) >> 12;
		ilm_ptr.dest_mod_id = MOD_MMI;

		if (g_IsPukPassed)
			phb->next_type = MMI_SETTING_TYPE_NO_REQUIRED;
		else
		{
			if (phb->chv_info.chv2_count > 0)//man 2005-6-28
			{
				phb->next_type = MMI_SETTING_TYPE_CHV2;
			}
			else if (phb->chv_info.ubchv2_count > 0)
			{
				phb->next_type = MMI_SETTING_TYPE_UBCHV2;
			}
			else if (phb->chv_info.ubchv2_count <= 0)
			{
				phb->next_type = MMI_SETTING_TYPE_CHV2_BLOCKED;
			}
		}

		ilm_ptr.src_mod_id = ADP_SIM_MSG_SRC_MOD;
		ilm_ptr.msg_id = MSG_ID_MMI_SMU_VERIFY_PIN_RSP;
		ilm_ptr.local_para_ptr = (local_para_struct *)phb;

		mmi_trace(g_sw_ADP_SMU, "MMI_SIM: ADP send MSG_ID_MMI_SMU_VERIFY_PIN_RSP to MMI");
		OslMsgSendExtQueue(&ilm_ptr);
	}

		break;

		default:
			break;

		}

		return TRUE;
	}

	/**************************************************************

	FUNCTION NAME		: ADP_DispatchSimRspMsg

  	PURPOSE				: Message Adaption for SIM  RSP

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

	**************************************************************/
	extern BOOL gSIMIsNotExist[];
	static BOOL ADP_DispatchSimRspMsg(COS_EVENT *pCosEvent)
	{
		UINT8 nType = 0;
		UINT32 nParam1 = 0;
		UINT32 nParam2 = 0;
		UINT32 rtn = ERR_SUCCESS;
		#ifdef __ADP_MULTI_SIM__
		U16 nUTI;
		#endif

	#ifdef __ADP_MULTI_SIM__
		U8 nRspSimId = adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3));
	#endif

		TBM_ENTRY(0x29e1);
		ASSERT(pCosEvent != NULL);
		nParam1 = pCosEvent->nParam1;
		nParam2 = pCosEvent->nParam2;
		nType = HIUINT8(pCosEvent->nParam3);

	#ifdef __ADP_MULTI_SIM__
		#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
		nUTI = HIUINT16(pCosEvent->nParam3);
		nRspSimId = adp_simu_get_sim_id_by_uti_for_rsp_msg(pCosEvent->nEventId, nUTI, nRspSimId);
		#endif
		adp_ConfigureContext(ADP_MODULE_SIM, nRspSimId);
	#endif

		switch (pCosEvent->nEventId)
		{

		case EV_CFW_SIM_ADD_PBK_RSP:
		{
#ifdef __ADP_MULTI_SIM__
			adp_ConfigureContext(ADP_MODULE_PHB, g_pSIMContext->sim_id);
#endif

			if (nType == 0)
			{
				if (SimStorageID == CFW_PBK_SIM)
				{
				#ifdef __ADP_MULTI_SIM__
					#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					UT_CFW_SimGetPbkEntry(CFW_PBK_SIM, nParam1, FREE_UTI, adp_pPHBContext->nSimId);
					#else
					U8 Uti = FREE_UTI;
					#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
					Simu_Rsp_Marker_Struct simuRspMarker;
					simuRspMarker.msgID = EV_CFW_SIM_GET_PBK_ENTRY_RSP;
					simuRspMarker.nSimID = adp_pPHBContext->nSimId;
					simuRspMarker.Uti = Uti;
					simuRspMarker.pNext = NULL;
					adp_simu_add_Rsp_Marker(&simuRspMarker);
					#endif
					CFW_SimGetPbkEntry(CFW_PBK_SIM, nParam1, Uti, gPHBCurrentSimID);
					#endif
				#else
					CFW_SimGetPbkEntry(CFW_PBK_SIM, nParam1, FREE_UTI);
				#endif
					//2007-07-11, Li Tao, 添加成功后维护映射表,物理地址从1开始
					ASSERT((nParam1 > 0) && (nParam1 <= adp_pPHBContext->nSIMTotalSlot));
					adp_pPHBContext->PB_SIM_RECORDID[nParam1] = nParam1;
				}
				else
					adp_phb_MSISDN_set_entry_ok_rsp();
			}
			else if (nType == 0xF0)
			{
				switch (nParam1)
				{
					case ERR_CME_INVALID_INDEX:
						mmi_trace(g_sw_ADP_PHB, TSTXT("The input index is invalid  !\n"));
						break;

					case ERR_CME_MEMORY_FAILURE:
						mmi_trace(g_sw_ADP_PHB, TSTXT("SIM card is full.  !\n"));
						break;

					case ERR_CME_SIM_NOT_INSERTED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Not SIM Card Insert  !\n"));
						break;

					case ERR_CME_SIM_PIN_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PIN1  !\n"));
						break;

					case ERR_CME_SIM_PUK_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PUK1 !\n"));
						break;

					case ERR_CME_SIM_PIN2_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PIN2 !\n"));
						break;

					case ERR_CME_SIM_PUK2_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PUK2  !\n"));
						break;

					case ERR_CME_SIM_FAILURE:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Initialize SIM Fail  !\n"));
						break;

					case ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("SIM maybe is initializing  !\n"));
						break;

					case ERR_CME_MEMORY_FULL:
						mmi_trace(g_sw_ADP_PHB, TSTXT("No any more memory to allocate for SIM management !\n"));
						break;

					case ERR_CME_UNKNOWN:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Unknown error occur  !\n"));
						break;

					default:
						break;

				}

				/* send response to MMI:set entry fail */
				adp_phb_set_entry_error_rsp();
			}
			else
			{
				ASSERT(0);
			}

		}
			break;

		case EV_CFW_SIM_DELETE_PBK_ENTRY_RSP :
		{

      mmi_trace(g_sw_ADP_PHB, "EV_CFW_SIM_DELETE_PBK_ENTRY_RSP");
       //add by chenq for bug 5889 and 5800, 20070803 B 
      COS_KillTimerEX(GetMmiTaskHandle(MOD_ADP),PHBLIST_COPY_OR_DELETE_TIMEROUTID );
        //add by chenq for bug 5889 and 5800, 20070803 E
			if (nType == 0)
			{
				/* send msg to MMI: result = ok*/
				adp_phb_del_entry_ok_rsp();

				if (MMI_PHB_PHONEBOOK_TYPE)
				{
					//2007-07-11, Li Tao, 添加成功后维护映射表
					ASSERT((nParam1 > 0) && (nParam1 <= adp_pPHBContext->nSIMTotalSlot));
					adp_pPHBContext->PB_SIM_RECORDID[nParam1] = 0;
					MMI_PHB_PHONEBOOK_TYPE = FALSE;
				}
			}
			else if (nType == 0xF0)
			{
				switch (nParam1)
				{
					case ERR_CME_INVALID_INDEX:
						mmi_trace(g_sw_ADP_PHB, TSTXT("The input index is invalid  !\n"));
						break;

					case ERR_CME_MEMORY_FAILURE:
						mmi_trace(g_sw_ADP_PHB, TSTXT("SIM card is full.  !\n"));
						break;

					case ERR_CME_SIM_NOT_INSERTED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Not SIM Card Insert  !\n"));
						break;

					case ERR_CME_SIM_PIN_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PIN1  !\n"));
						break;

					case ERR_CME_SIM_PUK_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PUK1 !\n"));
						break;

					case ERR_CME_SIM_PIN2_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PIN2 !\n"));
						break;

					case ERR_CME_SIM_PUK2_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PUK2  !\n"));
						break;

					case ERR_CME_SIM_FAILURE:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Initialize SIM Fail  !\n"));
						break;

					case ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("SIM maybe is initializing  !\n"));
						break;

					case ERR_CME_MEMORY_FULL:
						mmi_trace(g_sw_ADP_PHB, TSTXT("No any more memory to allocate for SIM management !\n"));
						break;

					case ERR_CME_UNKNOWN:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Unknown error occur  !\n"));
						break;

					default:
						break;

				}

				/* send msg to MMI: result = error*/
				adp_phb_del_entry_error_rsp();
			}
			else
			{
				ASSERT(0);
			}
		}

		break;

		case EV_CFW_SIM_GET_PBK_ENTRY_RSP :
		{
		#ifdef __ADP_MULTI_SIM__
			U8 nSimID = g_pSIMContext->sim_id;
			#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
			nSimID= adp_simu_get_sim_id_by_uti_for_rsp_msg(EV_CFW_SIM_GET_PBK_ENTRY_RSP, nUTI, nSimID);
			#endif
			adp_ConfigureContext(ADP_MODULE_PHB, nSimID);
			mmi_trace(g_sw_PHB, "PHB GET ENTRY RSP nParam3simid = %d", adp_GetCSWSimID(pCosEvent->nParam3));
		#endif

			//add by chenq for bug 5889 and 5800, 20070803 B
			COS_KillTimerEX(GetMmiTaskHandle(MOD_ADP), PHBLIST_COPY_OR_DELETE_TIMEROUTID);
			//add by chenq for bug 5889 and 5800, 20070803 E

			if (flag_init_ok)
			{
				mmi_trace(g_sw_ADP_PHB, TSTXT("flag_init_ok\n"));
				if (nType == 0 && (CFW_SIM_PBK_ENTRY_INFO *)nParam1 != NULL)
				{
					ilm_struct  ilm_ptr;
					mmi_phb_set_entry_rsp_struct *phb = OslMalloc(sizeof(mmi_phb_set_entry_rsp_struct));
					CFW_SIM_PBK_ENTRY_INFO *entryInfo;
					mmi_trace(g_sw_ADP_PHB, TSTXT("if (nType == 0)\n"));
					ASSERT(phb != NULL);
					memset( phb, 0x00, sizeof(mmi_phb_set_entry_rsp_struct));

					entryInfo = (CFW_SIM_PBK_ENTRY_INFO *)nParam1;
					mmi_trace(g_sw_PHB, "PHB init ok get entry rsp phoneindex = %d", entryInfo->phoneIndex);

					phb->result.flag = 0;
					phb->no_list = 1;
				#ifdef __ADP_MULTI_SIM__
					phb->list[0].storage = MMI_SIM + adp_pPHBContext->nSimId;
				#else
					phb->list[0].storage = MMI_SIM;
				#endif
					phb->list[0].record_index = entryInfo->phoneIndex;

					//2007-07-11, Li Tao, 维护映射表,物理地址从1开始
					ASSERT((entryInfo->phoneIndex > 0) && (entryInfo->phoneIndex <= adp_pPHBContext->nSIMTotalSlot));

					adp_pPHBContext->PB_SIM_RECORDID[entryInfo->phoneIndex] = entryInfo->phoneIndex;

					mmi_trace(g_sw_PHB, "Init phb >> 0x%x,%d", entryInfo->pFullName[0], entryInfo->iFullNameSize);
					
					if (entryInfo->pFullName[0] == 0x80)
					{
						UINT8 len = 0;
						if (entryInfo->iFullNameSize > 0)
						{
							len = (UINT8)(entryInfo->iFullNameSize - 1) > (sizeof(phb->list[0].alpha_id.name) - 2) ? (sizeof(phb->list[0].alpha_id.name) - 2):(UINT8)(entryInfo->iFullNameSize - 1);
						}
						else
						{
							len = 0;
						}

						memcpy(phb->list[0].alpha_id.name, entryInfo->pFullName + 1, len);
						phb->list[0].alpha_id.name[len] = '\0';
						phb->list[0].alpha_id.name[len + 1] = '\0';
						phb->list[0].alpha_id.name_length = len;				
						phb->list[0].alpha_id.name_dcs = MMI_PHB_UCS2;
		
					}
					else
					{
						UINT8 len = 0;
						len = entryInfo->iFullNameSize < (sizeof(phb->list[0].alpha_id.name) - 1) ? entryInfo->iFullNameSize : (sizeof(phb->list[0].alpha_id.name) - 1);
						phb->list[0].alpha_id.name_dcs = MMI_PHB_ASCII;
						memcpy(phb->list[0].alpha_id.name, entryInfo->pFullName, len);
						phb->list[0].alpha_id.name[len] = '\0';
						phb->list[0].alpha_id.name_length = len;
					}

					if (entryInfo->nType == 129 || entryInfo->nType == 161)
						phb->list[0].tel.type = MMI_CSMCC_DEFAULT_ADDR_TYPE;
					else
						phb->list[0].tel.type = MMI_CSMCC_INTERNATIONAL_ADDR;

					phb->list[0].tel.length = SUL_GsmBcdToAscii(entryInfo->pNumber, entryInfo->nNumberSize, phb->list[0].tel.number);

					ilm_ptr.dest_mod_id = MOD_MMI;
					ilm_ptr.src_mod_id = MOD_L4C;
					ilm_ptr.msg_id = PRT_PHB_SET_ENTRY_RSP;
					ilm_ptr.local_para_ptr = (local_para_struct *)phb;
					OslMsgSendExtQueue(&ilm_ptr);
				}
				else if (nType == 0xF0 || (CFW_SIM_PBK_ENTRY_INFO *)nParam1 == NULL)
				{
					adp_phb_set_entry_error_rsp();
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				UINT32 ValueSimGetPbkEntry = 0;
				mmi_trace(g_sw_ADP_PHB, TSTXT("flag_init_ok is false"));
				adp_pPHBContext->nSimIndex++;

				if (nType == 0)
				{
					CFW_SIM_PBK_ENTRY_INFO *entryInfo = (CFW_SIM_PBK_ENTRY_INFO *)nParam1;
					ASSERT(adp_pPHBContext->nSimIndex <= adp_pPHBContext->nSIMTotalSlot + 1);
					mmi_trace(g_sw_ADP_PHB, "PHB INIT get entry rsp phoneIndex = %d, simindex = %d, nParam1=0x%x", LOINT16(nParam2), adp_pPHBContext->nSimIndex, nParam1);
					 
					if (/*entryInfo->phoneIndex*/nParam1 == 0) //guojian modified 20090416 for crash when nParam1 is null but entryInfo->phoneIndex is not null
					{
					#ifdef __ADP_MULTI_SIM__					
						nCurAccess++;
					#endif
						if(adp_pPHBContext->nSimIndex >= adp_pPHBContext->nSIMTotalSlot + 1)
						{
							mmi_trace(g_sw_ADP_PHB, "PHB Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);						
					#ifdef __ADP_MULTI_SIM__
							ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
					#else
							Msg_Adaption_PHB_Init();
					#endif
						}
						else
						{
						#ifdef __ADP_MULTI_SIM__
							#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
							ValueSimGetPbkEntry = UT_CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, FREE_UTI, adp_pPHBContext->nSimId);
							#else
							nUTI = FREE_UTI;
							#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
							Simu_Rsp_Marker_Struct simuRspMarker;
							simuRspMarker.msgID = EV_CFW_SIM_GET_PBK_ENTRY_RSP;
							simuRspMarker.nSimID = adp_pPHBContext->nSimId;
							simuRspMarker.Uti = nUTI;
							simuRspMarker.pNext = NULL;
							adp_simu_add_Rsp_Marker(&simuRspMarker);
							#endif
							ValueSimGetPbkEntry = CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, nUTI, gPHBCurrentSimID);
							if(ValueSimGetPbkEntry != ERR_SUCCESS)
							{
								mmi_trace(g_sw_ADP_PHB, "PHB Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
								ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
							}
							#endif
						#else
							ValueSimGetPbkEntry = CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, FREE_UTI);
						#endif
						}
					}
					else
					{	
						ilm_struct  ilm_ptr;
						mmi_phb_startup_read_ind_struct *phb = OslMalloc(sizeof(mmi_phb_startup_read_ind_struct));
						ASSERT(phb != NULL);
						memset(phb, 0x00, sizeof(mmi_phb_startup_read_ind_struct));
						phb->access_id = nCurAccess;
						nCurAccess++;
					#ifdef __ADP_MULTI_SIM__
						phb->phb_entry.storage = MMI_SIM + adp_pPHBContext->nSimId;
					#else
						phb->phb_entry.storage = MMI_SIM;

					#endif
						phb->phb_entry.record_index = entryInfo->phoneIndex;

						//2007-07-11, Li Tao, 维护映射表,物理地址从1开始
						ASSERT((entryInfo->phoneIndex > 0) && (entryInfo->phoneIndex <= adp_pPHBContext->nSIMTotalSlot));

						adp_pPHBContext->PB_SIM_RECORDID[entryInfo->phoneIndex] = entryInfo->phoneIndex;

						mmi_trace(g_sw_PHB, "PHB Init phb 0x%x,%d", entryInfo->pFullName[0], entryInfo->iFullNameSize);

						if (entryInfo->pFullName[0] == 0x80)
						{	
							UINT8 len = 0;
							if (entryInfo->iFullNameSize > 0)
							{
								len = (UINT8)(entryInfo->iFullNameSize - 1) > (sizeof(phb->phb_entry.alpha_id.name) - 2) ? (sizeof(phb->phb_entry.alpha_id.name) - 2):(UINT8)(entryInfo->iFullNameSize - 1);
							}
							else
							{
								len = 0;
							}

							memcpy(phb->phb_entry.alpha_id.name, entryInfo->pFullName + 1, len);
							phb->phb_entry.alpha_id.name[len] = '\0';
							phb->phb_entry.alpha_id.name[len + 1] = '\0';
							phb->phb_entry.alpha_id.name_length = len;					
							phb->phb_entry.alpha_id.name_dcs = MMI_PHB_UCS2;
	
						}
						else
						{
							UINT8 len = 0;
							len = entryInfo->iFullNameSize < (sizeof(phb->phb_entry.alpha_id.name) - 1) ? entryInfo->iFullNameSize : (sizeof(phb->phb_entry.alpha_id.name) - 1);
							phb->phb_entry.alpha_id.name_dcs = MMI_PHB_ASCII;
							memcpy(phb->phb_entry.alpha_id.name, entryInfo->pFullName, len);
							phb->phb_entry.alpha_id.name[len] = '\0';
							phb->phb_entry.alpha_id.name_length = len;
						}

						mmi_trace(g_sw_PHB, "PHB entryInfo->nType is 0x%x",entryInfo->nType);
						if(entryInfo->nType == 0x91)
						{
							phb->phb_entry.tel.addr_bcd[0] = MMI_CSMCC_INTERNATIONAL_ADDR;
						}
						else
						{
							phb->phb_entry.tel.addr_bcd[0] = MMI_CSMCC_DEFAULT_ADDR_TYPE;
						}
						/*
						if (entryInfo->nType == 129 || entryInfo->nType == 161)
							phb->phb_entry.tel.addr_bcd[0] = MMI_CSMCC_DEFAULT_ADDR_TYPE;
						else
							phb->phb_entry.tel.addr_bcd[0] = MMI_CSMCC_INTERNATIONAL_ADDR;
						*/
						memcpy(phb->phb_entry.tel.addr_bcd + 1, entryInfo->pNumber, entryInfo->nNumberSize);
						phb->phb_entry.tel.addr_length = entryInfo->nNumberSize + 1;
		
						ilm_ptr.dest_mod_id = MOD_MMI;
						ilm_ptr.src_mod_id = MOD_L4C;
						ilm_ptr.msg_id = MSG_ID_MMI_PHB_STARTUP_READ_IND;
						ilm_ptr.local_para_ptr = (local_para_struct *)phb;
						OslMsgSendExtQueue(&ilm_ptr);
					}
				}
				else if (nType == 0xF0)
				{
					switch (nParam1)
					{

					case ERR_CME_INVALID_INDEX:
						mmi_trace(g_sw_ADP_PHB, TSTXT("EV_CFW_SIM_GET_PBK_ENTRY_RSP: Not SIM Card Insert."));

						if (adp_pPHBContext->nSimIndex <= adp_pPHBContext->nSIMTotalSlot)
						{
						#ifdef __ADP_MULTI_SIM__
							#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
							UT_CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, FREE_UTI, adp_pPHBContext->nSimId);
							#else
							nUTI = FREE_UTI;
							#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
							Simu_Rsp_Marker_Struct simuRspMarker;
							simuRspMarker.msgID = EV_CFW_SIM_GET_PBK_ENTRY_RSP;
							simuRspMarker.nSimID = adp_pPHBContext->nSimId;
							simuRspMarker.Uti = nUTI;
							simuRspMarker.pNext = NULL;
							adp_simu_add_Rsp_Marker(&simuRspMarker);
							#endif
							ValueSimGetPbkEntry = CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, nUTI, gPHBCurrentSimID);
							if(ValueSimGetPbkEntry != ERR_SUCCESS)
							{
								mmi_trace(g_sw_ADP_SAT,"PHB Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
								ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
							}
							#endif
						#else
							CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, FREE_UTI);
						#endif
						}

						break;

						case ERR_CME_MEMORY_FAILURE:
							mmi_trace(g_sw_ADP_PHB, TSTXT("SIM card is full."));
							break;

						case ERR_CME_SIM_NOT_INSERTED:
							mmi_trace(g_sw_ADP_PHB, TSTXT("Not SIM Card Insert."));
							break;

						case ERR_CME_SIM_PIN_REQUIRED:
							mmi_trace(g_sw_ADP_PHB, TSTXT("Need PIN1"));
							break;

						case ERR_CME_SIM_PUK_REQUIRED:
							mmi_trace(g_sw_ADP_PHB, TSTXT("Need PUK1"));
							break;

						case ERR_CME_SIM_FAILURE:
							mmi_trace(g_sw_ADP_PHB, TSTXT("Initialize SIM Fail."));
							break;

						case ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED:
							mmi_trace(g_sw_ADP_PHB, TSTXT("SIM maybe is initializing."));
							break;

						case ERR_CME_MEMORY_FULL:
							mmi_trace(g_sw_ADP_PHB, TSTXT("No any more memory to allocate for SIM management."));
							break;

						case ERR_CME_UNKNOWN:
						default:
							mmi_trace(g_sw_ADP_PHB, TSTXT("ERR_CME_UNKNOWN or Defalut Error: nParam1=0x%x"));
							break;
					}
				}
				else
				{
					ASSERT(0);
				}
			}
		}
			break;

		case EV_CFW_SIM_LIST_PBK_ENTRY_RSP :
			if (nType == 0)
			{
				adp_phb_get_extra_number_rsp(nParam1, nParam2, nParam2);
			}
			else if (nType == 0xF0)
			{
				adp_phb_get_extra_number_rsp(0, 0, 0);
			}
		
			break;
		case EV_CFW_SIM_GET_PBK_STRORAGE_RSP:
		{
			//add by liuxn 060630 start
			CFW_PBK_STORAGE *pbStorage = NULL;
			UINT32 ValueSimGetPbkEntry = 0;
      
#ifdef __ADP_MULTI_SIM__
			adp_ConfigureContext(ADP_MODULE_PHB, g_pSIMContext->sim_id);
			mmi_trace(g_sw_ADP_PHB, "PHB get storage rsp nParam3simid = %x, nType = %d, SimStorageID = %d", adp_GetCSWSimID(pCosEvent->nParam3), nType, SimStorageID);
#endif
			if (nType == 0)
			{
				pbStorage = (CFW_PBK_STORAGE *)pCosEvent->nParam1;
				//mmi_trace(g_sw_ADP_PHB, TSTXT("EV_CFW_SIM_GET_PBK_STRORAGE_RSP:used=%d,total=%d,storageId=%d\n"),
				//pbStorage->usedSlot, pbStorage->totalSlot, pbStorage->storageId);
			#ifdef __ADP_MULTI_SIM__	
				mmi_trace(g_sw_ADP_PHB, "PHB Init get storage rsp used=%d,total=%d,storageId=%d", pbStorage->usedSlot, pbStorage->totalSlot, pbStorage->storageId);
			#endif
				SimStorageID = pbStorage->storageId;
				if (SimStorageID == CFW_PBK_SIM)
				{
					adp_pPHBContext->nSIMusedSlot = pbStorage->usedSlot;
					adp_pPHBContext->nSIMTotalSlot = pbStorage->totalSlot;
					mmi_trace(g_sw_ADP_PHB, TSTXT("CFW_PBK_SIM\n"));

					if (adp_pPHBContext->nSIMusedSlot > 0)		//init the SIM pbk
					{
					
					#ifdef __ADP_MULTI_SIM__
						U16 nSimUsedSlot = 0;
						U8 i;
						for(i = 0; i < ADP_SIM_NUMBER; i++)
						{
							nSimUsedSlot += adp_gPHBContex[i].nSIMusedSlot;
						}
				
						if (nCurAccess >  nSimUsedSlot)
						{
							nCurAccess = 1;
						}
					#else
						nCurAccess = 1;
					#endif
						adp_pPHBContext->nSimIndex = 1;


					#ifdef __ADP_MULTI_SIM__
						{
							#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
							ValueSimGetPbkEntry = UT_CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, FREE_UTI, adp_pPHBContext->nSimId);
							#else
							U8 Uti = FREE_UTI;
							#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
							Simu_Rsp_Marker_Struct simuRspMarker;
							simuRspMarker.msgID = EV_CFW_SIM_GET_PBK_ENTRY_RSP;
							simuRspMarker.nSimID = adp_pPHBContext->nSimId;
							simuRspMarker.Uti = Uti;
							simuRspMarker.pNext = NULL;
							adp_simu_add_Rsp_Marker(&simuRspMarker);
							#endif
							ValueSimGetPbkEntry = CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, Uti, gPHBCurrentSimID);
							if(ValueSimGetPbkEntry != ERR_SUCCESS)
							{
								mmi_trace(g_sw_ADP_PHB, "PHB Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
								ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
							}
							#endif
						}
					#else
						ValueSimGetPbkEntry = CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, FREE_UTI);
					#endif

						switch (ValueSimGetPbkEntry)
						{
							case ERR_SUCCESS:
								mmi_trace(g_sw_ADP_PHB, TSTXT("the function succeeds !\n"));
								break;

							case ERR_CFW_INVALID_PARAMETER:
								mmi_trace(g_sw_ADP_PHB, TSTXT("The input parameter(nIndex) is invalid !\n"));
								break;

							case ERR_CFW_NOT_EXIST_FREE_UTI:
								mmi_trace(g_sw_ADP_PHB, TSTXT("No any available free UTI in the system !\n"));
								break;

							case ERR_CFW_UTI_IS_BUSY:
								mmi_trace(g_sw_ADP_PHB, TSTXT("The UTI is busy, maybe the UTI is used by other instance !\n"));
								break;

							case ERR_CME_MEMORY_FULL:
								mmi_trace(g_sw_ADP_PHB, TSTXT("No any more memory to allocate !\n"));
								break;

							default:
					//			ASSERT(0);
								break;
						}
					}

#ifdef __ADP_MULTI_SIM__
					else
					{
						ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
					}
#else
					else		//init the ME pbk
					{
						Msg_Adaption_PHB_Init();
					}
#endif								
				}
				else if (SimStorageID == CFW_PBK_ON)
				{
					nMSISDNusedSlot = pbStorage->usedSlot;
					nMSISDNtotalSlot = pbStorage->totalSlot;

					if (nMSISDNusedSlot > 0)
					{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
						ValueSimGetPbkEntry = UT_CFW_SimListCountPbkEntries(SimStorageID, 1, nMSISDNusedSlot, FREE_UTI, adp_pPHBContext->nSimId);
#else
						ValueSimGetPbkEntry = CFW_SimListCountPbkEntries(SimStorageID, 1, nMSISDNusedSlot, FREE_UTI, adp_pPHBContext->nSimId);
#endif
#else
						ValueSimGetPbkEntry = CFW_SimListCountPbkEntries(SimStorageID, 1, nMSISDNusedSlot, FREE_UTI);
#endif
					}
					else	//rsp
						adp_phb_get_extra_number_rsp(0, 0, nMSISDNtotalSlot);
				}
				else if (SimStorageID == CFW_PBK_SIM_FIX_DIALLING
					|| SimStorageID == CFW_PBK_SIM_BARRED_DIALLING)
				{
					adp_pPHBContext->nFDNusedSlot = pbStorage->usedSlot;
					adp_pPHBContext->nFDNtotalSlot = pbStorage->totalSlot;

					if (adp_pPHBContext->flag_list_FDN)
					{
						if (pbStorage->usedSlot > 0)
						{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
							ValueSimGetPbkEntry = UT_CFW_SimListCountPbkEntries(SimStorageID, 1, pbStorage->usedSlot, FREE_UTI, adp_pPHBContext->nSimId);
#else
							ValueSimGetPbkEntry = CFW_SimListCountPbkEntries(SimStorageID, 1, pbStorage->usedSlot, FREE_UTI, adp_pPHBContext->nSimId);
#endif
#else
							ValueSimGetPbkEntry = CFW_SimListCountPbkEntries(SimStorageID, 1, pbStorage->usedSlot, FREE_UTI);
#endif
						}
						else	//rsp
						{
							adp_phb_get_FDN_number_rsp(0, 0);
						}
					}
					else	
					{
						adp_phb_smu_read_file_info_rsp(adp_pPHBContext->nFDNtotalSlot);
					}

					mmi_trace(g_sw_ADP_PHB, "PHB FDN get storage rsp flag_list_FDN = %d, CFW_SimListCountPbkEntries = %d", adp_pPHBContext->flag_list_FDN, ValueSimGetPbkEntry);

				}
				else if (SimStorageID == CFW_PBK_EN
					|| SimStorageID == CFW_PBK_SIM_SERVICE_DIALLING)
				{
					if (pbStorage->usedSlot > 0)
					{
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
						ValueSimGetPbkEntry = UT_CFW_SimListCountPbkEntries(SimStorageID, 1, pbStorage->usedSlot, FREE_UTI, adp_pPHBContext->nSimId);
#else
						ValueSimGetPbkEntry = CFW_SimListCountPbkEntries(SimStorageID, 1, pbStorage->usedSlot, FREE_UTI, adp_pPHBContext->nSimId);
#endif
#else
						ValueSimGetPbkEntry = CFW_SimListCountPbkEntries(SimStorageID, 1, pbStorage->usedSlot, FREE_UTI);
#endif
					}
					else	//rsp
						adp_phb_get_FDN_number_rsp(0, 0);
				}
				else
				{
					ASSERT(0);
				}
			}
			else if (nType == 0xF0)
			{
				mmi_trace(g_sw_ADP_PHB,"PHB STORGE RSP Line: %d+++++----- error code=%x", __LINE__,pCosEvent->nParam1);
				switch (pCosEvent->nParam1)
				{
					case ERR_CME_SIM_NOT_INSERTED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Not SIM Card Insert."));
						break;

					case ERR_CME_SIM_PIN_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PIN1"));
						break;

					case ERR_CME_SIM_PUK_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PUK1"));
						break;

					case ERR_CME_SIM_FAILURE:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Initialize SIM Fail."));
						break;

					case ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("SIM maybe is initializing."));
						break;

					case ERR_CME_MEMORY_FULL:
						mmi_trace(g_sw_ADP_PHB, TSTXT("No any more memory to allocate for SIM management."));
						break;

				case ERR_CME_UNKNOWN:
					mmi_trace(g_sw_ADP_PHB, TSTXT("ERR_CME_UNKNOWN"));
					break;

				default:
					break;
				}
				/*Guojian Modify Start For 9757 if sim is error,can't complete the init process for phb Ver: GreenStone  on 2008-10-19 11:37 */
				if(!flag_init_ok)
				{
#ifdef __ADP_MULTI_SIM__
					if(ValueSimGetPbkEntry != ERR_SUCCESS)
					{
						mmi_trace(g_sw_ADP_PHB,"PHB Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
						ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
					}	
#else	
					Msg_Adaption_PHB_Init();  //if SIM is error, it should continue to initialize ME PHB
#endif
				}
				else
				{
					adp_phb_smu_read_file_info_rsp(0);
					adp_phb_get_extra_number_rsp(0, 0, 0);
				}
				/*Guojian Modify End  For 9757 Ver: GreenStone  on 2008-10-19 11:37 */
			}
			else
			{
				ASSERT(0);
			}
			//add by liuxn 060630 end
		}
			break;

		case EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP:
		{
			CFW_PBK_STRORAGE_INFO *pbStorage = NULL;
			UINT32 ValueSimGetPbkEntry = 0;

			BOOL bOtherInitOnGoing = FALSE;
			U8 i;

		#ifdef __ADP_MULTI_SIM__
			U8 nSimID;
		#endif
			
		#ifdef __ADP_MULTI_SIM__
			nSimID = g_pSIMContext->sim_id;
			#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
			nSimID = adp_simu_get_sim_id_by_uti_for_rsp_msg(EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP, nUTI, nSimID);
			#endif
			adp_ConfigureContext(ADP_MODULE_PHB, nSimID);
			mmi_trace(g_sw_ADP_PHB, "PHB get storage rsp nParam3simid = %x, nType = %d, SimStorageID = %d", adp_GetCSWSimID(pCosEvent->nParam3), nType, SimStorageID);
		#endif
		
			if(nType == 0)
			{
				pbStorage = (CFW_PBK_STRORAGE_INFO *)pCosEvent->nParam1;

				mmi_trace(g_sw_ADP_PHB, "ADP_PHB Init get storage info rsp total=%d", pbStorage->index);
				
				SimStorageID = CFW_PBK_SIM;
			
				adp_pPHBContext->nSIMTotalSlot = pbStorage->index;

				#ifdef __ADP_MULTI_SIM__
				for(i = 0; i < ADP_SIM_NUMBER; i++)
				{
					if(i != adp_pPHBContext->nSimId && g_PHBSIMStartInd[i] && !g_PHBSIMStartFinish[i])
					{
						bOtherInitOnGoing = TRUE;
						break;
					}
				}
				
				if(flag_is_fdn_state_on[adp_pPHBContext->nSimId] && ! bOtherInitOnGoing)
				{
					g_PHBSIMStartInd[adp_pPHBContext->nSimId] = TRUE;
					ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
					return TRUE;
				}
				#endif
				
				adp_pPHBContext->nSIMusedSlot = pbStorage->index;
				if (adp_pPHBContext->nSIMusedSlot > 0)		//init the SIM pbk
				{
				#ifdef __ADP_MULTI_SIM__
					U16 nSIMUsedSlot = 0;
					for(i = 0; i < ADP_SIM_NUMBER; i++)
					{
						nSIMUsedSlot += adp_gPHBContex[i].nSIMusedSlot;
					}
					if (nCurAccess >  nSIMUsedSlot)
					{
						nCurAccess = 1;
					}
				#else
					nCurAccess = 1;
				#endif

				
					adp_pPHBContext->nSimIndex = 1;
				
				#ifdef __ADP_MULTI_SIM__
					#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					ValueSimGetPbkEntry = UT_CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, FREE_UTI, adp_pPHBContext->nSimId);
					#else
					
					nUTI = FREE_UTI;
					
					#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
					Simu_Rsp_Marker_Struct simuRspMarker;
					simuRspMarker.msgID = EV_CFW_SIM_GET_PBK_ENTRY_RSP;
					simuRspMarker.nSimID = adp_pPHBContext->nSimId;
					simuRspMarker.Uti = nUTI;
					simuRspMarker.pNext = NULL;
					adp_simu_add_Rsp_Marker(&simuRspMarker);
					#endif
					
					ValueSimGetPbkEntry = CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, nUTI, gPHBCurrentSimID);
					if(ValueSimGetPbkEntry != ERR_SUCCESS)
					{
						ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
					}
					#endif
				#else
					ValueSimGetPbkEntry = CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, FREE_UTI);
				#endif

					switch (ValueSimGetPbkEntry)
					{
						case ERR_SUCCESS:
							mmi_trace(g_sw_ADP_PHB, TSTXT("call CFW_SimGetPbkEntry succeed !\n"));
							break;

						case ERR_CFW_INVALID_PARAMETER:
							mmi_trace(g_sw_ADP_PHB, TSTXT("The input parameter(nIndex) is invalid !\n"));
							break;

						case ERR_CFW_NOT_EXIST_FREE_UTI:
							mmi_trace(g_sw_ADP_PHB, TSTXT("No any available free UTI in the system !\n"));
							break;

						case ERR_CFW_UTI_IS_BUSY:
							mmi_trace(g_sw_ADP_PHB, TSTXT("The UTI is busy, maybe the UTI is used by other instance !\n"));
							break;

						case ERR_CME_MEMORY_FULL:
							mmi_trace(g_sw_ADP_PHB, TSTXT("No any more memory to allocate !\n"));
							break;

						default:
							break;
					}
				}
				else
				{
				#ifdef __ADP_MULTI_SIM__
					ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
				#else
					Msg_Adaption_PHB_Init();
				#endif
				}							
			}
			else if(nType == 0xF0)
			{
				mmi_trace(g_sw_ADP_PHB,"ADP_PHB STORGE INFO RSP Line: %d+++++----- error code=%x", __LINE__,pCosEvent->nParam1);
				switch (pCosEvent->nParam1)
				{
					case ERR_CME_SIM_NOT_INSERTED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Not SIM Card Insert."));
						break;

					case ERR_CME_SIM_PIN_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PIN1"));
						break;

					case ERR_CME_SIM_PUK_REQUIRED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Need PUK1"));
						break;

					case ERR_CME_SIM_FAILURE:
						mmi_trace(g_sw_ADP_PHB, TSTXT("Initialize SIM Fail."));
						break;

					case ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED:
						mmi_trace(g_sw_ADP_PHB, TSTXT("SIM maybe is initializing."));
						break;

					case ERR_CME_MEMORY_FULL:
						mmi_trace(g_sw_ADP_PHB, TSTXT("No any more memory to allocate for SIM management."));
						break;

				case ERR_CME_UNKNOWN:
					mmi_trace(g_sw_ADP_PHB, TSTXT("ERR_CME_UNKNOWN"));
					break;

				default:
					break;
				}
				/*Guojian Modify Start For 9757 if sim is error,can't complete the init process for phb Ver: GreenStone  on 2008-10-19 11:37 */
				if(!flag_init_ok)
				{
				#ifdef __ADP_MULTI_SIM__
					if(ValueSimGetPbkEntry != ERR_SUCCESS)
					{
						mmi_trace(g_sw_ADP_PHB,"PHB Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
						ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
					}	
				#else
					Msg_Adaption_PHB_Init();  //if SIM is error, it should continue to initialize ME PHB
				#endif
				}
				else
				{
					adp_phb_smu_read_file_info_rsp(0);
					adp_phb_get_extra_number_rsp(0, 0, 0);
				}
				/*Guojian Modify End  For 9757 Ver: GreenStone  on 2008-10-19 11:37 */
			}				
		}			
			break;

		case EV_CFW_SIM_GET_PROVIDER_ID_RSP:
		{
			ilm_struct  ilm_ptr;
			mmi_smu_get_imsi_rsp_struct *imsi_ptr= OslMalloc(sizeof(mmi_smu_get_imsi_rsp_struct));
			ASSERT(imsi_ptr != NULL);
			memset(imsi_ptr, 0x00, sizeof(mmi_smu_get_imsi_rsp_struct));

			ilm_ptr.local_para_ptr = (local_para_struct *)imsi_ptr;
			if (nType == 0)
			{
				imsi_ptr->result = ST_SUCCESS;
				memcpy(imsi_ptr->imsi, (void *)pCosEvent->nParam1, pCosEvent->nParam2);
				imsi_ptr->imsi[pCosEvent->nParam2] = '\0';
				{
					int i;
					int len = strlen(imsi_ptr->imsi);
					mmi_trace(g_sw_ADP_SIM, TSTXT(" GET_PROVIDER_ID_RSP len =%d \n"), len);
					for (i = 0; i < len; i++)
						mmi_trace(g_sw_ADP_SIM, TSTXT(" imsi =[%d] \n"), imsi_ptr->imsi[i]);
				}
			}
			else
				imsi_ptr->result = ST_FAILURE;

			ilm_ptr.dest_mod_id = MOD_MMI;
			ilm_ptr.src_mod_id = ADP_SIM_MSG_SRC_MOD;
			ilm_ptr.msg_id = MSG_ID_MMI_SMU_GET_IMSI_RSP;
			OslMsgSendExtQueue(&ilm_ptr);
		}
			break;

		case EV_CFW_SIM_GET_AUTH_STATUS_RSP:

			if (!do_cswmsg_EV_CFW_SIM_GET_AUTH_STATUS_RSP(pCosEvent))
			{
				TBM_EXIT(0x29e1);
				return FALSE;
			}

			break;

	case EV_CFW_SIM_ENTER_AUTH_RSP:
	{
			message_type dispatch = GetMsgFlag();
			mmi_trace(g_sw_ADP_SMU, "MMI_SIM: EV_CFW_SIM_ENTER_AUTH_RSP, nType is: 0x%x, nParam1 is: 0x%x", nType, nParam1);
			if (nType == 0)
			{
			g_IsPukPassed = TRUE;//man

				switch (dispatch)
				{

				case PUK2_LOCK:
					//adp_phb_verify_pin_ok_rsp();
#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					UT_CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);//man
#else
					CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);//man
#endif
#else

					CFW_SimGetAuthenticationStatus(FREE_UTI);//man
#endif

					break;

				case PUK1_LOCK:
				{
#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					UT_CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
#else
					CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
#endif
#else
					CFW_SimGetAuthenticationStatus(FREE_UTI);
#endif
				}

				break;

				case VERIFY_PIN1:

				case VERIFY_PUK1:
				{
					mmi_trace(g_sw_ADP_SMU, "MMI_SIM: verify pin ok");
				#ifdef __ADP_MULTI_SIM__
					#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					rtn = UT_CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
					#else
					rtn = CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
					#endif
				#else
					rtn = CFW_SimGetAuthenticationStatus(FREE_UTI);
				#endif
					if (rtn != ERR_SUCCESS)
						mmi_trace(g_sw_ADP_SMU, "MMI_SIM: Call CFW_SimGetAuthenticationStatus failed, reutrn value is: 0x%x", rtn);
					else
						mmi_trace(g_sw_ADP_SMU, "MMI_SIM: Call CFW_SimGetAuthenticationStatus OK");
				}

				break;

				default:
					break;
				}
			}
			else if (nType == 0xF0)
			{
			g_IsPukPassed = FALSE;//man

			switch (dispatch)
			{

			case PUK2_LOCK:
				//adp_phb_verify_pin_error_rsp(nParam1);
	#ifdef __ADP_MULTI_SIM__

	#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				UT_CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
	#else
				CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
	#endif
	#else

				CFW_SimGetAuthenticationStatus(FREE_UTI);
	#endif
				break;

				case PUK1_LOCK:
#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					UT_CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
#else
					CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
#endif
#else

					CFW_SimGetAuthenticationStatus(FREE_UTI);
#endif
					break;

				case VERIFY_PIN1:

				case VERIFY_PUK1:
				{
					mmi_trace(g_sw_ADP_SMU, "MMI_SIM: verify pin failed");

				#ifdef __ADP_MULTI_SIM__
					#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					rtn = UT_CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
					#else
					rtn = CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
					#endif
				#else
					rtn = CFW_SimGetAuthenticationStatus(FREE_UTI);
				#endif
					if(rtn != ERR_SUCCESS)
						mmi_trace(g_sw_ADP_SMU, "MMI_SIM: call CFW_SimGetAuthenticationStatus failed, return value is: 0x%x", rtn);
					else
						mmi_trace(g_sw_ADP_SMU, "MMI_SIM: call CFW_SimGetAuthenticationStatus OK");
				}

				break;

				default:
					break;
				}
			}
			else
				ASSERT(0);
		}
			break;

		case EV_CFW_SIM_SET_FACILITY_LOCK_RSP:
		{
			message_type dispatch;
			UINT32 result;
			mmi_trace(g_sw_ADP_PHB, TSTXT("EV_CFW_SIM_SET_FACILITY_LOCK_RSP:ntype=%d,p1:0x%x,P2:0x%x\n"), nType, nParam1, nParam2);

			TRACE_EF(g_sw_ADP_SMU, " EV_CFW_SIM_SET_FACILITY_LOCK_RSP nType = %x, nPram1 = %x", nType, nParam1);
			 dispatch = GetMsgFlag();
			
			if (nType == 0)
			{
			g_IsPinPassed = TRUE;

			switch (dispatch)
			{

			case PHB_DAIL_MODE:
				adp_phb_set_dial_mode_ok_rsp();
			#ifdef __ADP_MULTI_SIM__
				{
					UINT32 nCFWRtn = 0;

					if(flag_init_ok)
					{
						U8 i;
						nCurAccess=1;
						Msg_Adaption_PHB_Init_Ready();
						Msg_Adaption_PHB_startup_begin();


						flag_is_fdn_state_on[g_pSIMContext->sim_id] = !flag_is_fdn_state_on[g_pSIMContext->sim_id];

						for(i = 0; i < MMI_SIM_NUMBER; i++)
						{
							if(!gSIMIsNotExist[i])
								g_PHBSIMStartInd[i] = TRUE;
						}
						if(flag_is_fdn_state_on[g_pSIMContext->sim_id])
						{
							ADP_PhbInitFinishOneCardStartAnother(g_pSIMContext->sim_id);
						}
						else
						{
							U8 Uti = FREE_UTI;
							U8 nSimID = g_pSIMContext->sim_id;
							#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
							Simu_Rsp_Marker_Struct simuRspMarker;
							simuRspMarker.msgID = EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP;
							simuRspMarker.nSimID = nSimID;
							simuRspMarker.Uti = Uti;
							simuRspMarker.pNext = NULL;
							adp_simu_add_Rsp_Marker(&simuRspMarker);
							if(nSimID > ADP_SIM_2)
								nSimID -= 2;
							#endif
							nCFWRtn = CFW_SimGetPbkStrorageInfo((UINT8)CFW_PBK_SIM, Uti, nSimID);
							if(nCFWRtn != ERR_SUCCESS)
							{
								mmi_trace(g_sw_ADP_PHB, "FDN  change state rsp CFW_SimGetPbkStrorageInfo = %x", nCFWRtn);
							}
						}
			
					}
				}
			#endif				
				break;

			case PIN1_LOCK:
				g_IsPinPassed = TRUE;
				TRACE_EF(g_sw_SMU, "g_IsPinPassed = TRUE;CFW_SimGetAuthenticationStatus(CFW_SIM_IND_UTI);");
	#ifdef __ADP_MULTI_SIM__

	#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				result = UT_CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
	#else
				result = CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
	#endif
	#else
				result = CFW_SimGetAuthenticationStatus(FREE_UTI);
	#endif

				if (ERR_SUCCESS != result)
				{
					TRACE_SMU_FUNC("(ERR_SUCCESS != CFW_SimGetAuthenticationStatus(CFW_SIM_IND_UTI))");
					TRACE_EF(g_sw_SMU, "reason = %d, reason = 0x%x", result, result);
				}
				else
				{
					TRACE_SMU_FUNC("(ERR_SUCCESS == CFW_SimGetAuthenticationStatus(CFW_SIM_IND_UTI))");
				}

				break;

			default:
				break;
			}

			//add by panxu for the pin 2006-11-16
		}
		else if (nType == 0xF0)
		{
			g_IsPinPassed = FALSE;

			switch (dispatch)
			{

			case PHB_DAIL_MODE:
				adp_phb_set_dial_mode_error_rsp();
				break;

			case PIN1_LOCK:
				g_IsPinPassed = FALSE;
	#ifdef __ADP_MULTI_SIM__

	#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				UT_CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
	#else
				CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
	#endif
	#else
				CFW_SimGetAuthenticationStatus(FREE_UTI);

	#endif
				break;

			default:
				break;
			}

			// add by panxu for the pin 2006-11-16
		}
		else
			ASSERT(0);
	}

		break;

		case EV_CFW_SIM_GET_FACILITY_LOCK_RSP:
			{
			#ifdef __ADP_MULTI_SIM__
				BOOL b_fdn_state_req = FALSE;
				U8 i = 0;
			#endif
			
				message_type dispatch = GetMsgFlag();
				mmi_trace(g_sw_ADP_PHB, TSTXT("EV_CFW_SIM_GET_FACILITY_LOCK_RSP: ntype=%d,p1:0x%x,P2:0x%x\n"), nType, nParam1, nParam2);
			#ifdef __ADP_MULTI_SIM__
				for(i = 0; i <ADP_SIM_NUMBER; i++)
				{
					if(g_b_fdn_state_req[i])
					{
						b_fdn_state_req = TRUE;
						g_b_fdn_state_req[i] = FALSE;
						break;
					}
				}
			#endif
			
				if (nType == 0)
				{
					if (CHECK_PIN1 == dispatch)
					{
						mmi_smu_check_pin_status_rsp_struct *pPinStatusRsp ;
						
						TRACE_SMU_FUNC("EV_CFW_SIM_GET_FACILITY_LOCK_RSP, OK PRT_MMI_SMU_CHECK_PIN_STATUS_RSP ok");
						pPinStatusRsp =
							(mmi_smu_check_pin_status_rsp_struct*)OslMalloc(sizeof(mmi_smu_check_pin_status_rsp_struct));
						ASSERT(NULL != pPinStatusRsp);
						memset(pPinStatusRsp, 0, sizeof(mmi_smu_check_pin_status_rsp_struct));

						if (0 == LOUINT8(nParam1))
						{
							pPinStatusRsp->chv_status.chv1_status = CHV_DISABLED;
						}
						else
						{
							pPinStatusRsp->chv_status.chv1_status = CHV_VERIFIED;
						}
						
						pPinStatusRsp->chv_info.chv1_count = (LOUINT16(nParam2)&0x0F);
						pPinStatusRsp->chv_info.chv2_count = ((LOUINT16(nParam2)&0xF00) >> 8);
						pPinStatusRsp->chv_info.ubchv1_count = ((LOUINT16(nParam2)&0xF0) >> 4);
						pPinStatusRsp->chv_info.ubchv2_count = ((LOUINT16(nParam2)&0xF000) >> 12);
						
						TRACE_EF(g_sw_ADP_SMU, "EV_CFW_SIM_GET_FACILITY_LOCK_RSP, chv1_status = %d", pPinStatusRsp->chv_status.chv1_status);
						SendSmuEventToMMI((local_para_struct*) pPinStatusRsp, PRT_MMI_SMU_CHECK_PIN_STATUS_RSP);
					}
				#ifdef __ADP_MULTI_SIM__	
					else if(b_fdn_state_req)
				#else
					else
				#endif
					{
						ilm_struct  ilm_ptr;
						module_type srcmodId = MOD_L4C;
						mmi_smu_get_dial_mode_rsp_struct *phb = OslMalloc(sizeof(mmi_smu_get_dial_mode_rsp_struct));
						ASSERT(phb != NULL);
						memset(phb, 0x00, sizeof(mmi_smu_get_dial_mode_rsp_struct));

						if (LOUINT8(nParam1) == 1)
						{
							phb->dial_mode = 0x04;//MMI_TYPE_FDN
						#ifdef __ADP_MULTI_SIM__
							flag_is_fdn_state_on[g_pSIMContext->sim_id] = TRUE;
						#endif	
						}
						else
							phb->dial_mode = 0x02;//MMI_TYPE_ADN;

						ilm_ptr.dest_mod_id = MOD_MMI;
						
					#ifdef __ADP_MULTI_SIM__
						srcmodId = MOD_L4C + g_pSIMContext->sim_id;
						flag_is_fdn_state_on[g_pSIMContext->sim_id] = FALSE;
					#endif
						ilm_ptr.src_mod_id = srcmodId;

						ilm_ptr.msg_id = PRT_MMI_SMU_GET_DIAL_MODE_RSP;
						ilm_ptr.local_para_ptr = (local_para_struct *)phb;
						OslMsgSendExtQueue(&ilm_ptr);
					}
				}
				else if (nType == 0xF0)
				{
				#ifdef __ADP_MULTI_SIM__
					if(!b_fdn_state_req)
				#endif				
					{
	         			 mmi_smu_check_pin_status_rsp_struct *pPinStatusRsp = NULL;

					mmi_trace(g_sw_ADP_PHB,"PHB Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
					
						//PHB needn't
					pPinStatusRsp = (mmi_smu_check_pin_status_rsp_struct*)OslMalloc(sizeof(mmi_smu_check_pin_status_rsp_struct));
					TRACE_SMU_FUNC("EV_CFW_SIM_GET_FACILITY_LOCK_RSP, FAIL PRT_MMI_SMU_CHECK_PIN_STATUS_RSP FAIL");
					ASSERT(NULL != pPinStatusRsp);
					memset(pPinStatusRsp, 0, sizeof(mmi_smu_check_pin_status_rsp_struct));

						pPinStatusRsp->result = FALSE;
						pPinStatusRsp->chv_status.chv1_status = CHV_DISABLED;

						SendSmuEventToMMI((local_para_struct*) pPinStatusRsp, PRT_MMI_SMU_CHECK_PIN_STATUS_RSP);
					}
				#ifdef __ADP_MULTI_SIM__
					else
					{
						flag_is_fdn_state_on[g_pSIMContext->sim_id] = FALSE;
					}
				#endif				
				}
				else
					ASSERT(0);
			}
			break;

		case EV_CFW_SIM_DELETE_MESSAGE_RSP:
			break;

		case EV_CFW_SIM_WRITE_MESSAGE_RSP:
			break;

		case EV_CFW_SIM_READ_MESSAGE_RSP:
			break;

		case EV_CFW_SIM_LIST_MESSAGE_RSP:
			break;

		case EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP:
			break;

		case EV_CFW_SIM_READ_BINARY_RSP:
			break;

		case EV_CFW_SIM_UPDATE_BINARY_RSP:
			break;

		case EV_CFW_SIM_GET_SMS_PARAMETERS_RSP:
			break;

		case EV_CFW_SIM_SET_SMS_PARAMETERS_RSP:
			break;

		case EV_CFW_SIM_GET_MR_RSP:
			break;

		case EV_CFW_SIM_SET_MR_RSP:
			break;

		case EV_CFW_SIM_COMPOSE_PDU_RSP:
			break;

		case EV_CFW_SAT_ACTIVATION_RSP:
			break;

		case EV_CFW_SAT_GET_STATUS_RSP:
			break;

		case EV_CFW_SAT_RESPONSE_RSP:
			break;

		case EV_CFW_SAT_GET_INFORMATION_RSP:
			break;

		case EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP:
			mmi_trace(g_sw_ADP_NW, ">>>> receive EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP>>>>\r\n");
			DoNETSETAddorDeletePrefNetworkRsp(pCosEvent);
			break;
			
		case EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP:
			mmi_trace(g_sw_ADP_NW, ">>>> receive EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP>>>>\r\n");
			DoNETSETGetPreferredNetworkRsp(pCosEvent);
			break;

		case EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP:
#ifdef __ADP_MULTI_SIM__		
			mmi_trace(g_sw_ADP_NW, ">>>> receive EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP>>>>for SIM%d",g_pSIMContext->sim_id);
#endif
			DoNumberOfPLMNSuported(pCosEvent);
			break;

		case EV_CFW_SIM_SET_PBKMODE_RSP:
			break;

		case EV_CFW_SIM_GET_PBKMODE_RSP:
			break;

		case EV_CFW_SIM_INIT_SMS_RSP:
			break;
		case EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP:
		{
			if (nType == 0)
			{
				if (SimStorageID == CFW_PBK_ON)
					adp_phb_get_extra_number_rsp(nParam1, nParam2, nMSISDNtotalSlot);
				else if (SimStorageID == CFW_PBK_EN
					|| SimStorageID == CFW_PBK_SIM_FIX_DIALLING
					|| SimStorageID == CFW_PBK_SIM_BARRED_DIALLING 
					|| SimStorageID == CFW_PBK_SIM_SERVICE_DIALLING)
				{
					adp_phb_get_FDN_number_rsp(nParam1, nParam2);
				}
			}
			else if (nType == 0xF0)
			{
				adp_phb_get_extra_number_rsp(0, 0, 0);
			}
			mmi_trace(g_sw_ADP_PHB, TSTXT("EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP: count=%d\n"), nParam2);
		}
			break;

	case EV_CFW_SIM_CHANGE_PWD_RSP:
	{
		UINT32 nResult = 0;
		
		if (0 == nType)
		{
			g_IsPinChanged = TRUE;
		}
		else
		{
			g_IsPinChanged = FALSE;
		}

		TRACE_EF(g_sw_ADP_SMU, "EV_CFW_SIM_CHANGE_PWD_RSP, g_IsPinChanged = %d, nType = %x, nParam1 = %0x", g_IsPinChanged, nType, nParam1);

#ifdef __ADP_MULTI_SIM__

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			nResult = UT_CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
#else
			nResult = CFW_SimGetAuthenticationStatus(FREE_UTI, g_pSIMContext->sim_id);
#endif
#else
			nResult = CFW_SimGetAuthenticationStatus(FREE_UTI);

#endif

			if (ERR_SUCCESS != nResult)
			{
				TRACE_EF(g_sw_ADP_SMU, "EV_CFW_SIM_CHANGE_PWD_RSP, CFW_SimGetAuthenticationStatus, fail, the reason = %0x", nResult);
			}
		}

		break;
	case EV_CFW_SIM_RESET_RSP:
#ifndef MMI_ON_WIN32	
		{
			U8 *pReult = NULL;
			pReult = (U8*)OslMalloc(sizeof(U8));
			MMI_ASSERT(pReult != NULL);
			
			if(nType == 0)
			{
				//reset success
				gSIMIsNotExist[g_pSIMContext->sim_id] = FALSE;
				*pReult = TRUE;
			}
			else
			{
				//reset failed
				*pReult = FALSE;	
			}
			mmi_trace(g_sw_ADP_SMU, "ADP_SIM: sim reset result is %d", *pReult);
			SendSmuEventToMMI((local_para_struct*) pReult, PRT_MSG_ID_MMI_RESET_SIM_RSP);
		}
#endif
		break;

	default:
		TBM_EXIT(0x29e1);
		return FALSE;

	}
	TBM_EXIT(0x29e1);
	return TRUE;
}


/**************************************************************

	FUNCTION NAME		: ADP_DispatchSimIndMsg

  	PURPOSE				: Message Adaption for SIM  IND

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

	**************************************************************/
	static BOOL ADP_DispatchSimIndMsg(COS_EVENT *pCosEvent)
	{
		ASSERT(pCosEvent != NULL);

#ifdef __ADP_MULTI_SIM__
		adp_ConfigureContext(ADP_MODULE_SIM, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif
		return FALSE;
	}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchSimMsg

  	PURPOSE				: Message Adaption for SIM

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/
BOOL ADP_DispatchSimMsg(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);

	if (pCosEvent->nEventId >= MSG_ID_MMI_SMU_GET_IMSI_REQ && pCosEvent->nEventId <= MSG_ID_MMI_SMU_CHECK_PIN_STATUS_REQ)
	{
		return ADP_DispatchSimReqMsg(pCosEvent);
	}
	else if (pCosEvent->nEventId >= EV_CFW_SIM_RSP_BASE && pCosEvent->nEventId <= EV_CFW_SIM_RSP_END_)
	{
		if (ADP_DispatchSimRspMsg(pCosEvent))
			return TRUE;
		else if (ADP_DispatchSimIndMsg(pCosEvent))
			return TRUE;
	}
	
	return FALSE;
}

