/******************************************************************************
*              Copyright (C) 2006, Coolsand Technologies, Inc.
*                            All Rights Reserved
******************************************************************************
* Filename:    adp_PHB_Events.c
*
* Description:
*     change CSD phonebook message into coolsand message .
*     @Jackiechen 2006-07-11.
******************************************************************************/

/************************************************************************
  Change Histroy
  Version      Date             Author    Description
  02.01.001  2007-06-20, Han ShiLin, add handler for offset when write record
  02.01.002  2007-07-11, Li Tao, 添加、维护映射表（与SIM卡中电话本的物理存储位置索引对应）
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
 
#include <csw.h>
#include "mmi.h"
#include "uim.h"
#include "adp_events.h"
#include "adp_eventsredef.h"
#include "cfw.h"   
#include "phonebooktypes.h"  
#include "nvramenum.h"

 
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
extern BOOL flag_is_fdn_state_on[];
extern BOOL gSIMIsNotExist[];// indicate the sim is not inserted
#endif

#include "device.h"
#include "stack_msgs.h"
#include <cswtype.h>
#include "mmi_msg_struct.h"
#include "sul.h"
#include "fs.h"
#include "mmi_trace.h"

#ifdef MMI_ON_WIN32 /* zhuoxz,2009-4-27 */
#include "mmi_data_types.h"
#include "nvram_data_items.h"
#endif

//#include "phb_utils.h"
//#include "custom_mmi_default_value.h"

BOOL MMI_PHB_PHONEBOOK_TYPE = FALSE;

/**************************************************************
 * Macros
**************************************************************/
#define MAX_ABBREVIATED_DIAL_NUMBER_LEN		14
#define MAX_FIX_DIALLING_NUMBER_LEN			14
#define MAX_BARRED_DIALLING_NUMBER_LEN		14
#define MAX_OWNER_NUMBER_LEN				14


#define PHB_TIMEROUT_HANDWRITING_CONFIRM 10000
#define PAL_TICK1S      16384       // 16384 ticks/s
#define MILLI_SECOND    * PAL_TICK1S/1000



/**************************************************************
 * Manifest Constants
**************************************************************/
/* total item's infomation */
PUBLIC CFW_PBK_ENTRY_INFO  pb_coolsand[MAX_PB_PHONE_ENTRIES + 1];
//2007-07-11, Li Tao
//此数组存储SIM卡上每条电话本记录的记录号（物理索引），0表示该位置空白，可以被写入新记录。
//一定要注意MAX_PB_SIM_ENTRIES的值，数组的数据类型不要改成UINT8
//该数组尾部会有一部分垃圾数据，具体应用时，数组的最大下标为nSIMTotalSlot。
//UINT16 PB_SIM_RECORDID[MAX_PB_SIM_ENTRIES] = {0};

/* used physical index*/
static UINT32 MEUsedIndex[MAX_PB_PHONE_ENTRIES] = {0};
/* the total count of me used item*/
PUBLIC UINT32 nMEusedSlot = 0;
extern UINT16 nCurAccess;
extern UINT8 SimStorageID;
extern BOOL flag_init_ok;
extern BOOL flag_list_FDN;

#ifdef __ADP_MULTI_SIM__
PUBLIC BOOL g_PHBSIMStartInd[ADP_SIM_NUMBER] = {FALSE,};
PUBLIC BOOL g_PHBSIMStartFinish[ADP_SIM_NUMBER] = {FALSE,};
U8 gPHBCurrentSimID = 0;
#endif

adp_phb_context_t adp_gPHBContex[ADP_SIM_NUMBER];
adp_phb_context_t *adp_pPHBContext = adp_gPHBContex;


#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
extern PHB_OPTIONAL_FIELDS_STRUCT	PhoneBookOptionalFields;
#endif 
extern void  adp_phb_get_extra_number_rsp(UINT32 nParam1, UINT32 nParam2, UINT16 totalSlot);

/**************************************************************
* Local Functions Prototypes
**************************************************************/
void (*PHBListCopyOrDeleteTimerOutErrorFUN)(void);

/**************************************************************
	FUNCTION NAME		: PhbSetSimStorageID
	PURPOSE			       : send startup begin ind
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void
	RETURNS			       : void
	REMARKS			:
**************************************************************/
static void SetSimStorageID(MMI_PHB_TYPE  type)
{
	switch (type)
	{
		case  MMI_PHB_PHONEBOOK:
			SimStorageID = CFW_PBK_SIM;
			break;
		case MMI_PHB_FDN:
			SimStorageID = CFW_PBK_SIM_FIX_DIALLING;
			break;
		case MMI_PHB_MSISDN:
			SimStorageID = CFW_PBK_ON;
			break;
		case MMI_PHB_BDN:
			SimStorageID = CFW_PBK_SIM_BARRED_DIALLING;
			break;
		case MMI_PHB_SDN:
			SimStorageID = CFW_PBK_SIM_SERVICE_DIALLING;
			break;
		case MMI_PHB_ECC:
			SimStorageID = CFW_PBK_EN;
			break;
		case MMI_PHB_NONE:
		default:
			SimStorageID = CFW_PBK_ALL;
			break;
	}
}
	
/**************************************************************
* Local Functions definitions
**************************************************************/

/**************************************************************
	FUNCTION NAME		: Msg_Adaption_PHB_startup_begin
	PURPOSE			       : send startup begin ind
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void Msg_Adaption_PHB_startup_begin(void)
{
  ilm_struct ilm_ptr;

#ifdef __ADP_MULTI_SIM__
	U8 i;
	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		g_PHBSIMStartInd[i] = FALSE;
		g_PHBSIMStartFinish[i] = FALSE;
		adp_gPHBContex[i].nSIMusedSlot = 0;
	}
#endif


	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;

	ilm_ptr.msg_id = MSG_ID_MMI_PHB_STARTUP_BEGIN_IND;
	/*wangrui add for prevent free error*/
	ilm_ptr.local_para_ptr = NULL;
	/*wangrui add end*/
	OslMsgSendExtQueue(&ilm_ptr);
	mmi_trace(g_sw_ADP_PHB, TSTXT("PHB Init MSG_ID_MMI_PHB_STARTUP_BEGIN_IND.  \n"));

	flag_init_ok = FALSE;
}

/**************************************************************

	FUNCTION NAME		: Msg_Adaption_PHB_Init_Ready

	PURPOSE				: Init First about the PHB of Adaption

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/
void  Msg_Adaption_PHB_Init_Ready(void)
{
  
	UINT16 error = 0;
	INT32 index = 0;
	INT32 ret = 0;

	TBM_ENTRY(0x29e5);

	nMEusedSlot = 0;
	memset(pb_coolsand, 0x00, sizeof(CFW_PBK_ENTRY_INFO) * MAX_PB_PHONE_ENTRIES);
	
	ret = ReadRecord(NVRAM_PHB_NVRAM_LID, 1, 
		(void *)&pb_coolsand[1], sizeof(CFW_PBK_ENTRY_INFO) * MAX_PB_PHONE_ENTRIES, &error); 

	if (ret < 0)
	{
		mmi_trace(g_sw_ADP_PHB, TSTXT("PHB Init Msg_Adaption_PHB_Init_Ready FS_read error!\n"));
	}
	else
	{
		for (index = 1; index <= MAX_PB_PHONE_ENTRIES; index++)
		{
			if (pb_coolsand[index].nAppIndex != 0)
			{
				/* record total used index */
				MEUsedIndex[nMEusedSlot] = index;

				/* record total count */
				nMEusedSlot++;
			}
		}
		mmi_trace(g_sw_ADP_PHB, TSTXT("PHB Init Msg_Adaption_PHB_Init_Ready()***PhoneUsed:%d!\n"), nMEusedSlot);

	}

	TBM_EXIT(0x29e5);
}

/**************************************************************
	FUNCTION NAME		: Msg_Adaption_PHB_Finish_Init
	PURPOSE			       : Msg_Adaption_PHB_Finish_Init.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
extern BOOL bNeedInit[];
void Msg_Adaption_PHB_Finish_Init(void)
{
	ilm_struct ilm_ptr;
	U8 i;
	mmi_phb_startup_finish_ind_struct *localPtr = OslMalloc(sizeof(mmi_phb_startup_finish_ind_struct));
	ASSERT(localPtr != NULL);
	memset(localPtr, 0, sizeof(mmi_phb_startup_finish_ind_struct));

	localPtr->phb_len = MAX_ABBREVIATED_DIAL_NUMBER_LEN;
	localPtr->fdn_len = MAX_FIX_DIALLING_NUMBER_LEN;
	localPtr->bdn_len = MAX_ABBREVIATED_DIAL_NUMBER_LEN;
	localPtr->owner_len = MAX_OWNER_NUMBER_LEN;
	localPtr->phb_max_num = MAX_PB_PHONE_ENTRIES;

	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		localPtr->sim_max_num[i] = adp_gPHBContex[i].nSIMTotalSlot;
	}
		
	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = MSG_ID_MMI_PHB_STARTUP_FINISH_IND;
	ilm_ptr.local_para_ptr = (local_para_struct *)localPtr;
	OslMsgSendExtQueue(&ilm_ptr);
	mmi_trace(g_sw_ADP_PHB, TSTXT("PHB MSG_ID_MMI_PHB_STARTUP_READ_NEXT_REQ STARTUP_FINISH_IND.  \n"));

	flag_init_ok = TRUE;

	{
		U8 nSimIndex;
		for(nSimIndex = 0; nSimIndex< ADP_SIM_NUMBER;nSimIndex++)
		{
			if(bNeedInit[nSimIndex])
			{
				#ifdef __ADP_MULTI_SIM__
				mmi_trace(g_sw_ADP_SS, "SS Init Call Forwarding Start for sim%d", nSimIndex);
				AdpInitCallFwowardingStart(nSimIndex);
				#else
				AdpInitCallFwowardingStart();
				#endif
				break;
			}
		}
	}

#ifdef ADP_DUALSIM_MODE_CLOSE_NOSIM
	ADP_CloseNoSIM();
#endif
	
}

/**************************************************************

	FUNCTION NAME		: Msg_Adaption_PHB_Init

	PURPOSE				: Init about the PHB of Adaption

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/
void  Msg_Adaption_PHB_Init(void)
{
    U8 i = 0;
    
	if (nMEusedSlot > 0)
	{

		/* induct init procedure */
		ilm_struct  ilm_ptr;
		CFW_PBK_ENTRY_INFO *entryInfo = NULL;
		mmi_phb_startup_read_ind_struct *phb = OslMalloc(sizeof(mmi_phb_startup_read_ind_struct));
		ASSERT(phb != NULL);
		memset(phb, 0x00, sizeof(mmi_phb_startup_read_ind_struct));

		/* start from the first index */
		entryInfo = &(pb_coolsand[MEUsedIndex[0]]);
        #ifdef __ADP_MULTI_SIM__
		phb->access_id = 1;
		for(i = 0; i < ADP_SIM_NUMBER; i++)
		{
			if(g_PHBSIMStartFinish[i])
			{
				phb->access_id += adp_gPHBContex[i].nSIMTotalSlot;
			}
		}
        #else
		phb->access_id = nCurAccess;  //entryInfo->nAppIndex + 1;// 1;
        #endif	
		phb->phb_entry.storage = MMI_NVRAM;
		phb->phb_entry.record_index = entryInfo->nAppIndex;

		memcpy(&(phb->phb_entry.alpha_id), &(entryInfo->alpha_id), sizeof(l4_name_struct));
		memcpy(&(phb->phb_entry.tel), &(entryInfo->tel), sizeof(l4_addr_bcd_struct));

		//vivian add for bug 10769
		phb->phb_entry.tel.addr_length = entryInfo->tel.addr_length + 1;
		ilm_ptr.dest_mod_id = MOD_MMI;
		ilm_ptr.src_mod_id = MOD_L4C;
		ilm_ptr.msg_id = MSG_ID_MMI_PHB_STARTUP_READ_IND;
		ilm_ptr.local_para_ptr = (local_para_struct *)phb;
		OslMsgSendExtQueue(&ilm_ptr);
		mmi_trace(g_sw_ADP_PHB, TSTXT("PHB Init Func: %s***access_id:%d******nAppIndex:%d*** MEUsedIndex[0] = %d"), __FUNCTION__, phb->access_id, entryInfo->nAppIndex, MEUsedIndex[0]);
	}
	else
	{
		Msg_Adaption_PHB_Finish_Init();
	}
}

/**************************************************************
	FUNCTION NAME		: adp_phb_del_entry_error_rsp
	PURPOSE			       : respond PRT_PHB_DEL_ENTRY_REQ.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void adp_phb_del_entry_error_rsp(void)
{
	ilm_struct ilm_ptr;
	mmi_phb_del_entry_rsp_struct *localPtr = OslMalloc(sizeof(mmi_phb_del_entry_rsp_struct));
	ASSERT(localPtr != NULL);
	memset(localPtr, 0, sizeof(mmi_phb_del_entry_rsp_struct));
  mmi_trace(g_sw_ADP_PHB, "adp_phb_del_entry_error_rsp");
 //add by chenq for bug 5889 and 5800, 20070803 B  
  COS_KillTimerEX(GetMmiTaskHandle(MOD_ADP), PHBLIST_COPY_OR_DELETE_TIMEROUTID );
 //add by chenq for bug 5889 and 5800, 20070803 E  
	localPtr->result.flag = L4C_ERROR;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = PRT_PHB_DEL_ENTRY_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)localPtr;
	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************
	FUNCTION NAME		: adp_phb_del_entry_ok_rsp
	PURPOSE			       : respond PRT_PHB_DEL_ENTRY_REQ.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void adp_phb_del_entry_ok_rsp(void)
{
	ilm_struct ilm_ptr;
	mmi_phb_del_entry_rsp_struct *localPtr = OslMalloc(sizeof(mmi_phb_del_entry_rsp_struct));
	ASSERT(localPtr != NULL);
	memset(localPtr, 0, sizeof(mmi_phb_del_entry_rsp_struct));
  mmi_trace(g_sw_ADP_PHB, "adp_phb_del_entry_ok_rsp");

	localPtr->result.flag = L4C_OK;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = PRT_PHB_DEL_ENTRY_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)localPtr;
	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************
	FUNCTION NAME		: adp_phb_set_entry_error_rsp
	PURPOSE			       : respond PRT_PHB_SET_ENTRY_REQ.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void adp_phb_set_entry_error_rsp(void)
{
	ilm_struct  ilm_ptr;
	mmi_phb_set_entry_rsp_struct *phb = OslMalloc(sizeof(mmi_phb_set_entry_rsp_struct));

  mmi_trace(g_sw_PHB,"PHB Func: %s PRT_PHB_SET_ENTRY_RSP", __FUNCTION__);
	ASSERT(phb != NULL);
	memset(phb, 0x00, sizeof(mmi_phb_set_entry_rsp_struct));

 //add by chenq for bug 5889 and 5800, 20070803 B  
  COS_KillTimerEX(GetMmiTaskHandle(MOD_ADP),PHBLIST_COPY_OR_DELETE_TIMEROUTID );
 //add by chenq for bug 5889 and 5800, 20070803 E  
	phb->result.flag = L4C_ERROR;
	phb->result.cause = 0;
	phb->no_list = 1;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = PRT_PHB_SET_ENTRY_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)phb;
	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************
	FUNCTION NAME		: adp_phb_set_entry_capacity_exceeded_rsp
	PURPOSE			       : respond PRT_PHB_SET_ENTRY_REQ.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void adp_phb_set_entry_capacity_exceeded_rsp(void)
{
	ilm_struct  ilm_ptr;
	mmi_phb_set_entry_rsp_struct *phb = OslMalloc(sizeof(mmi_phb_set_entry_rsp_struct));

	mmi_trace(g_sw_PHB,"PHB Func: %s PRT_PHB_SET_ENTRY_RSP", __FUNCTION__);
	ASSERT(phb != NULL);
	memset(phb, 0x00, sizeof(mmi_phb_set_entry_rsp_struct));

	//add by chenq for bug 5889 and 5800, 20070803 B  
	COS_KillTimerEX(GetMmiTaskHandle(MOD_ADP),PHBLIST_COPY_OR_DELETE_TIMEROUTID );
	//add by chenq for bug 5889 and 5800, 20070803 E  
	phb->result.flag = L4C_ERROR;
	phb->result.cause = MMI_PHB_CAUSE_CAPACITY_EXCEEDED;
	phb->no_list = 1;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = PRT_PHB_SET_ENTRY_RSP;
	ilm_ptr.local_para_ptr = (local_para_struct *)phb;
	OslMsgSendExtQueue(&ilm_ptr);
}

/**************************************************************
	FUNCTION NAME		: ADP_DispatchPhbReqMsg
	PURPOSE			       : Message Adaption for Phonebook REQ.
	INPUT PARAMETERS	: COS_EVENT *pCosEvent
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
static BOOL ADP_DispatchPhbReqMsg(COS_EVENT *pCosEvent)
{

	UINT32  ValueSimGetPbkEntry = ERR_SUCCESS;
 	 UINT16 error = 0;
	ASSERT(pCosEvent != NULL);
 	TBM_ENTRY(0x29e3);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_PHB, adp_GetMMISimID(ADP_GET_MMI_DEST_MOD(pCosEvent->nParam3)));
#endif
	
	switch (pCosEvent->nEventId)
	{
		/* init item by item */
		case MSG_ID_MMI_PHB_STARTUP_READ_NEXT_REQ:
		{
			CFW_PBK_ENTRY_INFO *entryInfo = NULL;
			U16 nSimTotalSlot = 0;
			U8 i;
			ilm_struct *oslMessage = (ilm_struct *) pCosEvent->nParam1;
			mmi_phb_startup_read_next_req_struct *local_para_ptr = (mmi_phb_startup_read_next_req_struct *)oslMessage->local_para_ptr;

		#ifdef __ADP_MULTI_SIM__
			mmi_trace(g_sw_ADP_PHB,"PHB Init read req , simid = %d, access_id = %d, simindex = %d", adp_pPHBContext->nSimId, local_para_ptr->access_id, adp_pPHBContext->nSimIndex);

			for(i = 0; i < ADP_SIM_NUMBER; i++)
			{
				mmi_trace(g_sw_ADP_PHB, "PHB g_PHBSIMStartFinish[%d] is %d,adp_gPHBContex->nSimId is %d,adp_gPHBContex[%d].nSIMTotalSlot is %d", i, g_PHBSIMStartFinish[i],adp_pPHBContext->nSimId, i, adp_gPHBContex[i].nSIMTotalSlot);
				if(i != adp_pPHBContext->nSimId && g_PHBSIMStartFinish[i])
				{
					mmi_trace(g_sw_ADP_PHB, "PHB nSimTotalSlot before = %d", nSimTotalSlot);
					mmi_trace(g_sw_ADP_PHB, "PHB adp_pPHBContext[i].nSIMTotalSlot = %d", adp_gPHBContex[i].nSIMTotalSlot);
					nSimTotalSlot += adp_gPHBContex[i].nSIMTotalSlot;
					mmi_trace(g_sw_ADP_PHB, "PHB nSimTotalSlot after = %d", nSimTotalSlot);
				}
			}
			nSimTotalSlot += adp_pPHBContext->nSIMTotalSlot;
			
			mmi_trace(g_sw_ADP_PHB, "PHB nSimTotalSlot = %d",nSimTotalSlot);
			
			if(local_para_ptr->access_id == nSimTotalSlot)
			{
				ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
				return TRUE;
			}
		#endif 
			
		#ifdef __ADP_MULTI_SIM__
			if (local_para_ptr->access_id < nSimTotalSlot)
		#else
			if (local_para_ptr->access_id < adp_pPHBContext->nSIMusedSlot)
		#endif
			{	
				//nCurAccess = local_para_ptr->access_id + 1;
		#ifdef __ADP_MULTI_SIM__
			#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				ValueSimGetPbkEntry = UT_CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, FREE_UTI, adp_pPHBContext->nSimId);
			#else
				U8 nUTI = FREE_UTI;
				#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
				Simu_Rsp_Marker_Struct simuRspMarker;
				simuRspMarker.msgID = EV_CFW_SIM_GET_PBK_ENTRY_RSP;
				simuRspMarker.nSimID = adp_pPHBContext->nSimId;
				simuRspMarker.Uti = nUTI;
				simuRspMarker.pNext = NULL;
				adp_simu_add_Rsp_Marker(&simuRspMarker);
				#endif
				ValueSimGetPbkEntry = CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, nUTI, gPHBCurrentSimID);
			#endif
		#else
				ValueSimGetPbkEntry = CFW_SimGetPbkEntry(SimStorageID, adp_pPHBContext->nSimIndex, FREE_UTI);
		#endif
				mmi_trace(g_sw_ADP_PHB,"PHB Init Func: %s ValueSimGetPbkEntry=0x%x", __FUNCTION__, ValueSimGetPbkEntry);
				if(ValueSimGetPbkEntry != ERR_SUCCESS)
				{
				#ifdef __ADP_MULTI_SIM__
					ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
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
						break;
				}

				}
			}
		#ifdef __ADP_MULTI_SIM__
			else if(local_para_ptr->access_id < nSimTotalSlot + nMEusedSlot)
		#else
			else if (local_para_ptr->access_id < adp_pPHBContext->nSIMusedSlot + nMEusedSlot)
		#endif
			{			
				ilm_struct  ilm_ptr;
				mmi_phb_startup_read_ind_struct *phb = OslMalloc(sizeof(mmi_phb_startup_read_ind_struct));
				ASSERT(phb != NULL);
				memset(phb, 0x00, sizeof(mmi_phb_startup_read_ind_struct));
 		#ifdef __ADP_MULTI_SIM__
				entryInfo = &(pb_coolsand[MEUsedIndex[local_para_ptr->access_id - nSimTotalSlot]]);
		#else
				entryInfo = &(pb_coolsand[MEUsedIndex[local_para_ptr->access_id - adp_pPHBContext->nSIMusedSlot]]);
		#endif
				/* init next assess_id */
				phb->access_id = local_para_ptr->access_id + 1;
				phb->phb_entry.storage = MMI_NVRAM;

				/* physical index */
				phb->phb_entry.record_index = entryInfo->nAppIndex;

				memcpy(&(phb->phb_entry.alpha_id), &(entryInfo->alpha_id), sizeof(l4_name_struct));
				memcpy(&(phb->phb_entry.tel), &(entryInfo->tel), sizeof(l4_addr_bcd_struct));

				//vivian add for bug 10769
				phb->phb_entry.tel.addr_length = entryInfo->tel.addr_length + 1;
				ilm_ptr.dest_mod_id = MOD_MMI;
				ilm_ptr.src_mod_id = MOD_L4C;
				ilm_ptr.msg_id = MSG_ID_MMI_PHB_STARTUP_READ_IND;
				ilm_ptr.local_para_ptr = (local_para_struct *)phb;
				OslMsgSendExtQueue(&ilm_ptr);
				mmi_trace(g_sw_ADP_PHB, TSTXT("PHB Init startup_read_next_req, new access_id:%d, record_index:%d\n"), phb->access_id, phb->phb_entry.record_index);
			}
			else
			{		

			#ifdef __ADP_MULTI_SIM__
				BOOL bAllInitDone = TRUE;
				for(i = 0; i < ADP_SIM_NUMBER; i++)
				{
					if(!gSIMIsNotExist[i] && !g_PHBSIMStartFinish[i])
					{
						bAllInitDone = FALSE;
					}
				}
				if(bAllInitDone)
			#endif		
				{
					Msg_Adaption_PHB_Finish_Init();
				}
			}
		}

			break;

		case MSG_ID_MMI_PHB_DEL_ENTRY_REQ:
		{
			UINT32 ValueSimDelPbkEntry;
			ilm_struct *oslMessage = (ilm_struct *) pCosEvent->nParam1;
			mmi_phb_del_entry_req_struct *local_para_ptr = (mmi_phb_del_entry_req_struct *)oslMessage->local_para_ptr;
      		mmi_trace(g_sw_ADP_PHB, "PHB Delete MSG_ID_MMI_PHB_DEL_ENTRY_REQ");
			if (local_para_ptr->storage == MMI_NVRAM)
			{
				if (local_para_ptr->type == MMI_PHB_PHONEBOOK)
				{
					/* check if me used index exist */
					ASSERT(nMEusedSlot > 0);
					ASSERT(local_para_ptr->index <= MAX_PB_PHONE_ENTRIES);
						/* set nAppIndex as 0 value when delete item */
						pb_coolsand[local_para_ptr->index].nAppIndex = 0;
	  					memset(&pb_coolsand[local_para_ptr->index], 0, sizeof(CFW_PBK_ENTRY_INFO));
	  					//2007-06-20, Han ShiLin, add handler for offset when write record
 						//if (WriteRecord(NVRAM_PHB_NVRAM_LID, 1, (void *)&(pb_coolsand[local_para_ptr->index]), sizeof(CFW_PBK_ENTRY_INFO), &error) == sizeof(CFW_PBK_ENTRY_INFO))
 						if (WriteRecord(NVRAM_PHB_NVRAM_LID, 1, (void *)&(pb_coolsand[local_para_ptr->index]), (local_para_ptr->index - 1) * sizeof(CFW_PBK_ENTRY_INFO), &error) == sizeof(CFW_PBK_ENTRY_INFO))
						{
							/* decrease one from total count */
	 						nMEusedSlot--;
 							adp_phb_del_entry_ok_rsp();
						}
						else
							adp_phb_del_entry_error_rsp();
				}
			}
			else
			{
				SetSimStorageID(local_para_ptr->type);
		#ifdef __ADP_MULTI_SIM__
			#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				if (local_para_ptr->type == MMI_PHB_PHONEBOOK)
					ValueSimDelPbkEntry = UT_CFW_SimDeletePbkEntry(CFW_PBK_SIM, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_MSISDN)
					ValueSimDelPbkEntry = UT_CFW_SimDeletePbkEntry(CFW_PBK_ON, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_FDN)
					ValueSimDelPbkEntry = UT_CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_BDN)
					ValueSimDelPbkEntry = UT_CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_SDN)
					ValueSimDelPbkEntry = UT_CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_ECC)
					ValueSimDelPbkEntry = UT_CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else	
					ValueSimDelPbkEntry = ERR_CME_UNKNOWN;
			#else//ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				if (local_para_ptr->type == MMI_PHB_PHONEBOOK)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_SIM, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_MSISDN)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_ON, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_FDN)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_BDN)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_SDN)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_ECC)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI, adp_pPHBContext->nSimId);
				else	
					ValueSimDelPbkEntry = ERR_CME_UNKNOWN;
			#endif//ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		#else//__ADP_MULTI_SIM__
				if (local_para_ptr->type == MMI_PHB_PHONEBOOK)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_SIM, local_para_ptr->index, FREE_UTI);
				else if (local_para_ptr->type == MMI_PHB_MSISDN)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_ON, local_para_ptr->index, FREE_UTI);
				else if (local_para_ptr->type == MMI_PHB_FDN)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI);
				else if (local_para_ptr->type == MMI_PHB_BDN)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI);
				else if (local_para_ptr->type == MMI_PHB_SDN)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI);
				else if (local_para_ptr->type == MMI_PHB_ECC)
					ValueSimDelPbkEntry = CFW_SimDeletePbkEntry(CFW_PBK_SIM_FIX_DIALLING, local_para_ptr->index, FREE_UTI);
				else	
					ValueSimDelPbkEntry = ERR_CME_UNKNOWN;			
		#endif//__ADP_MULTI_SIM__
		mmi_trace(g_sw_ADP_PHB,"PHB del req  CFW_SimDeletePbkEntry = %d",ValueSimGetPbkEntry);
//add by chenq for bug 5889 and 5800, 20070803 B        
       switch (ValueSimDelPbkEntry)
				{
					case ERR_SUCCESS:
			            COS_KillTimerEX(GetMmiTaskHandle(MOD_ADP),PHBLIST_COPY_OR_DELETE_TIMEROUTID );
			            COS_SetTimerEX(GetMmiTaskHandle(MOD_ADP), PHBLIST_COPY_OR_DELETE_TIMEROUTID, 0, (PHB_TIMEROUT_HANDWRITING_CONFIRM)MILLI_SECOND);
			            PHBListCopyOrDeleteTimerOutErrorFUN = adp_phb_del_entry_error_rsp;
			      				
			            if (local_para_ptr->type == MMI_PHB_PHONEBOOK)
			            {
			              MMI_PHB_PHONEBOOK_TYPE = TRUE;
			            }
			              mmi_trace(g_sw_ADP_PHB, "local_para_ptr->index = %d",local_para_ptr->index);
						mmi_trace(g_sw_ADP_PHB, "the function succeeds !\n");
						break;

					case ERR_CFW_INVALID_PARAMETER:
						mmi_trace(g_sw_ADP_PHB, "The input parameter(nIndex) is invalid !\n");
						break;

					case ERR_CFW_NOT_EXIST_FREE_UTI:
						mmi_trace(g_sw_ADP_PHB, "No any available free UTI in the system !\n");
						break;

					case ERR_CFW_UTI_IS_BUSY:
						mmi_trace(g_sw_ADP_PHB, "The UTI is busy, maybe the UTI is used by other instance !\n");
						break;

					case ERR_CME_MEMORY_FULL:
						mmi_trace(g_sw_ADP_PHB, "No any more memory to allocate !\n");
						break;

					default:
						break;
				}
//add by chenq for bug 5889 and 5800, 20070803 E

				/* send msg to MMI: result = error */
				if (ValueSimDelPbkEntry != ERR_SUCCESS)
					adp_phb_del_entry_error_rsp();
			}
		}
			break;

		case MSG_ID_MMI_PHB_SET_ENTRY_REQ:
		{
			UINT32 ValueSimAddPbkEntry = 0;
			U8  length = 0;
			S32 ret = 0;
			
			ilm_struct *oslMessage = (ilm_struct *) pCosEvent->nParam1;
			mmi_phb_set_entry_req_struct *local_para_ptr = (mmi_phb_set_entry_req_struct *)oslMessage->local_para_ptr;
			mmi_trace(g_sw_ADP_PHB, "PHB SET ENTRY REQ storage = %d\n", local_para_ptr->list.storage);

			if (local_para_ptr->list.storage == MMI_NVRAM)
			{
				if (local_para_ptr->type == MMI_PHB_PHONEBOOK)
				{
					/* the flag of for circle sentence */
					INT32 index = 1;

					/* add new item */
					if (local_para_ptr->list.record_index == 0xFFFF)
					{
						if(nMEusedSlot >= MAX_PB_PHONE_ENTRIES)
						{
							adp_phb_set_entry_capacity_exceeded_rsp();
							TBM_EXIT(0x29e3);
							return TRUE;
						}
						
						for (index = 1; index <= MAX_PB_PHONE_ENTRIES; index++)
						{
							/* search the unused phisical index */
							if (pb_coolsand[index].nAppIndex == 0)
								break;
						}
						nMEusedSlot++;
					}
					else
					{
						ASSERT(local_para_ptr->list.record_index <= MAX_PB_PHONE_ENTRIES);
						index = local_para_ptr->list.record_index;
					}

					pb_coolsand[index].nAppIndex = index;
					mmi_trace(g_sw_ADP_PHB, "PHB FUNC:%s MSG_ID_MMI_PHB_SET_ENTRY_REQ, nAppIndex = %d", __FUNCTION__, index);
					memcpy(&(pb_coolsand[index].alpha_id), &(local_para_ptr->list.alpha_id), sizeof(l4_name_struct));

					/* check if international number and so on */
					pb_coolsand[index].tel.addr_bcd[0] = local_para_ptr->list.tel.type;
					

					/* save as bcd format */
         				 {
					  	extern U8 mmi_phb_convert_to_bcd (U8 *dest, U8 *source,  U8 max_dest_len );
					 	length = mmi_phb_convert_to_bcd(&(pb_coolsand[index].tel.addr_bcd[1]), local_para_ptr->list.tel.number, (MAX_PB_NUMBER_LENGTH + 1) / 2);
         				 }
					pb_coolsand[index].tel.addr_length = length;
					//hFile = pfopen("c:/phonebook.dat", FS_O_CREAT|FS_O_RDWR, 0);

					//if (hFile != NULL)
				//	{
						/* orient to be write index */
						ret = WriteRecord(NVRAM_PHB_NVRAM_LID, 1, (void *)&(pb_coolsand[index]), (index-1)*sizeof(CFW_PBK_ENTRY_INFO), &error);
						mmi_trace(g_sw_ADP_PHB, "ADP_PHB: FUNC:%s,MMI_PHB_SET_ENTRY_REQ WriteRecord ret = %x", __FUNCTION__, ret);
					
						if (ret == sizeof(CFW_PBK_ENTRY_INFO))
						{
							ilm_struct  ilm_ptr;
							mmi_phb_set_entry_rsp_struct *phb = OslMalloc(sizeof(mmi_phb_set_entry_rsp_struct));
							ASSERT(phb != NULL);
							memset(phb, 0x00, sizeof(mmi_phb_set_entry_rsp_struct));

							phb->result.flag = L4C_OK;
							phb->no_list = 1;
							phb->list[0].storage = MMI_NVRAM;
							phb->list[0].record_index = index;

							memcpy(&(phb->list[0].tel), &(local_para_ptr->list.tel), sizeof(l4c_number_struct));
							memcpy(&(phb->list[0].alpha_id), &(local_para_ptr->list.alpha_id), sizeof(l4_name_struct));

							ilm_ptr.dest_mod_id = MOD_MMI;
							ilm_ptr.src_mod_id = MOD_L4C;
							ilm_ptr.msg_id = PRT_PHB_SET_ENTRY_RSP;
							ilm_ptr.local_para_ptr = (local_para_struct *)phb;
							OslMsgSendExtQueue(&ilm_ptr);
						}
						else if(ret == ERR_FS_DISK_FULL)
						{
							adp_phb_set_entry_capacity_exceeded_rsp();				
						}
						else
						{
							adp_phb_set_entry_error_rsp();
            					}

					//	pfclose(hFile);
					}
				//	else
				//	{
						/* send response to MMI: set entry error*/
				//		adp_phb_set_entry_error_rsp();
				//	}
				//	mmi_trace(g_sw_ADP_PHB, TSTXT("mmi_phb_set_entry_rsp***index:%d******************************\n"), index);
			}
			else
			{
				INT32 index = 1;	//2007-07-11, Li Tao
				UINT8 number_bcd[32] = {0};
				UINT8 name_ucs2[(MAX_PB_NAME_LENGTH + 1) * ENCODING_LENGTH] = {0};

				CFW_SIM_PBK_ENTRY_INFO local_para_ptr2;
				memset(&local_para_ptr2, 0, sizeof(CFW_SIM_PBK_ENTRY_INFO));

				if (local_para_ptr->list.alpha_id.name_dcs == MMI_PHB_ASCII)
				{
					memcpy(name_ucs2, local_para_ptr->list.alpha_id.name, local_para_ptr->list.alpha_id.name_length);
					name_ucs2[local_para_ptr->list.alpha_id.name_length] = '\0';
					local_para_ptr2.iFullNameSize = local_para_ptr->list.alpha_id.name_length;
				}
				else
				{
					name_ucs2[0] = 0x80;
					memcpy(name_ucs2 + 1, local_para_ptr->list.alpha_id.name, local_para_ptr->list.alpha_id.name_length);
					local_para_ptr2.iFullNameSize = local_para_ptr->list.alpha_id.name_length + 1;
					name_ucs2[local_para_ptr2.iFullNameSize] = '\0';
					name_ucs2[local_para_ptr2.iFullNameSize + 1] = '\0';
				}
				local_para_ptr2.pFullName = name_ucs2;
				local_para_ptr2.nNumberSize = SUL_AsciiToGsmBcd(local_para_ptr->list.tel.number, local_para_ptr->list.tel.length, number_bcd);
				local_para_ptr2.pNumber = number_bcd;

				if (local_para_ptr->list.tel.type == MMI_CSMCC_INTERNATIONAL_ADDR)
					local_para_ptr2.nType = CFW_TELNUMBER_TYPE_INTERNATIONAL;
				else
					local_para_ptr2.nType = CFW_TELNUMBER_TYPE_NATIONAL;

// modify by chenq for PBK, 20070807 B
        		SetSimStorageID(local_para_ptr->type);
        
				if (local_para_ptr->list.record_index == 0xFFFF)
				{
					//2007-07-11, Li Tao，查找映射表，物理地址从1开始
					//local_para_ptr2->phoneIndex = 0;
					if (local_para_ptr->type == MMI_PHB_PHONEBOOK) 
					{
					for (index = 1; index <= adp_pPHBContext->nSIMTotalSlot; index++)
					{
						/* search the unused phisical index in SIM*/
						if (adp_pPHBContext->PB_SIM_RECORDID[index] == 0)
						{
							local_para_ptr2.phoneIndex = index;
							break;
						}
					}
					}
				}
				else
				{
					local_para_ptr2.phoneIndex = LOUINT8(local_para_ptr->list.record_index);
				}
// modify by chenq for PBK, 20070807 E
		#ifdef __ADP_MULTI_SIM__
		        mmi_trace(g_sw_ADP_PHB, "PHB SET ENTRY REQ nParamsimid = %d, phoneIndex = %d reqtype = %d",ADP_GET_MMI_DEST_MOD(pCosEvent->nParam3), local_para_ptr2.phoneIndex, local_para_ptr->type);
			#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				if (local_para_ptr->type == MMI_PHB_PHONEBOOK)
					ValueSimAddPbkEntry = UT_CFW_SimAddPbkEntry(CFW_PBK_SIM, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_FDN)
					ValueSimAddPbkEntry = UT_CFW_SimAddPbkEntry(CFW_PBK_SIM_FIX_DIALLING, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_MSISDN)
					ValueSimAddPbkEntry = UT_CFW_SimAddPbkEntry(CFW_PBK_ON, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_BDN)
					ValueSimAddPbkEntry = UT_CFW_SimAddPbkEntry(CFW_PBK_SIM_BARRED_DIALLING, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_SDN)
					ValueSimAddPbkEntry = UT_CFW_SimAddPbkEntry(CFW_PBK_SIM_SERVICE_DIALLING, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_ECC)
					ValueSimAddPbkEntry = UT_CFW_SimAddPbkEntry(CFW_PBK_EN, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else	
					ValueSimAddPbkEntry = ERR_CME_UNKNOWN;
			#else//ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				if (local_para_ptr->type == MMI_PHB_PHONEBOOK)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_SIM, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_FDN)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_SIM_FIX_DIALLING, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_MSISDN)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_ON, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_BDN)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_SIM_BARRED_DIALLING, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_SDN)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_SIM_SERVICE_DIALLING, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else if (local_para_ptr->type == MMI_PHB_ECC)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_EN, &local_para_ptr2, FREE_UTI, adp_pPHBContext->nSimId);
				else	
					ValueSimAddPbkEntry = ERR_CME_UNKNOWN;
			#endif //ADP_CFW_DUALSIM_SIMULATOR_SUPPORT	
		#else//__ADP_MULTI_SIM__
				if (local_para_ptr->type == MMI_PHB_PHONEBOOK)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_SIM, &local_para_ptr2, FREE_UTI);
				else if (local_para_ptr->type == MMI_PHB_FDN)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_SIM_FIX_DIALLING, &local_para_ptr2, FREE_UTI);
				else if (local_para_ptr->type == MMI_PHB_MSISDN)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_ON, &local_para_ptr2, FREE_UTI);
				else if (local_para_ptr->type == MMI_PHB_BDN)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_SIM_BARRED_DIALLING, &local_para_ptr2, FREE_UTI);
				else if (local_para_ptr->type == MMI_PHB_SDN)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_SIM_SERVICE_DIALLING, &local_para_ptr2, FREE_UTI);
				else if (local_para_ptr->type == MMI_PHB_ECC)
					ValueSimAddPbkEntry = CFW_SimAddPbkEntry(CFW_PBK_EN, &local_para_ptr2, FREE_UTI);
				else	
					ValueSimAddPbkEntry = ERR_CME_UNKNOWN;		
		#endif//__ADP_MULTI_SIM__
 //add by chenq for bug 5889 and 5800, 20070803 B      
 		mmi_trace(g_sw_ADP_PHB,"PHB set entry req  CFW_SimAddPbkEntry = %d",ValueSimGetPbkEntry);
        switch (ValueSimAddPbkEntry)
				{
					case ERR_SUCCESS:
            COS_KillTimerEX(GetMmiTaskHandle(MOD_ADP),PHBLIST_COPY_OR_DELETE_TIMEROUTID );
            COS_SetTimerEX(GetMmiTaskHandle(MOD_ADP), PHBLIST_COPY_OR_DELETE_TIMEROUTID, 0, (PHB_TIMEROUT_HANDWRITING_CONFIRM)MILLI_SECOND); 
            PHBListCopyOrDeleteTimerOutErrorFUN = adp_phb_set_entry_error_rsp;
						mmi_trace(g_sw_ADP_PHB, "the function succeeds !\n");
						break;

					case ERR_CFW_INVALID_PARAMETER:
						mmi_trace(g_sw_ADP_PHB, "The input parameter(nIndex) is invalid !\n");
						break;

					case ERR_CFW_NOT_EXIST_FREE_UTI:
						mmi_trace(g_sw_ADP_PHB, "No any available free UTI in the system !\n");
						break;

					case ERR_CFW_UTI_IS_BUSY:
						mmi_trace(g_sw_ADP_PHB, "The UTI is busy, maybe the UTI is used by other instance !\n");
						break;

					case ERR_CME_MEMORY_FULL:
						mmi_trace(g_sw_ADP_PHB, "No any more memory to allocate !\n");
						break;

					default:
						break;
				}
 //add by chenq for bug 5889 and 5800, 20070803 E
 
				if (ERR_SUCCESS != ValueSimAddPbkEntry)
				{
					adp_phb_set_entry_error_rsp();
				}
			}
		}
			break;

		case MSG_ID_MMI_PHB_GET_ENTRY_BY_INDEX_REQ:
		{
			ilm_struct *oslMessage = (ilm_struct *) pCosEvent->nParam1;
			mmi_phb_get_entry_by_index_req_struct *local_para_ptr = (mmi_phb_get_entry_by_index_req_struct *)oslMessage->local_para_ptr;

#ifdef __ADP_MULTI_SIM__
			mmi_trace(g_sw_ADP_PHB, "PHB GET ENTRY by index REQ simid = %d, local_para_ptr->type = %d", adp_pPHBContext->nSimId, local_para_ptr->type);
#endif /* __ADP_MULTI_SIM__ */

			switch (local_para_ptr->type)
			{
				case MMI_PHB_ECC:
			#ifdef __ADP_MULTI_SIM__
				#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					ValueSimGetPbkEntry = UT_CFW_SimGetPbkStorage(CFW_PBK_EN, FREE_UTI, adp_pPHBContext->nSimId);
				#else
					ValueSimGetPbkEntry = CFW_SimGetPbkStorage(CFW_PBK_EN, FREE_UTI, adp_pPHBContext->nSimId);
				#endif
			#else
					ValueSimGetPbkEntry = CFW_SimGetPbkStorage(CFW_PBK_EN, FREE_UTI);
			#endif					
					break;

				case MMI_PHB_FDN:
					adp_pPHBContext->flag_list_FDN = TRUE;
			#ifdef __ADP_MULTI_SIM__
				#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					ValueSimGetPbkEntry = UT_CFW_SimGetPbkStorage(CFW_PBK_SIM_FIX_DIALLING, FREE_UTI, adp_pPHBContext->nSimId);
				#else
					ValueSimGetPbkEntry = CFW_SimGetPbkStorage(CFW_PBK_SIM_FIX_DIALLING, FREE_UTI, adp_pPHBContext->nSimId);
				#endif
			#else
					ValueSimGetPbkEntry = CFW_SimGetPbkStorage(CFW_PBK_SIM_FIX_DIALLING, FREE_UTI);
			#endif
					break;

				case MMI_PHB_BDN:
			#ifdef __ADP_MULTI_SIM__
				#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					ValueSimGetPbkEntry = UT_CFW_SimGetPbkStorage(CFW_PBK_SIM_BARRED_DIALLING, FREE_UTI, adp_pPHBContext->nSimId);
				#else
					ValueSimGetPbkEntry = CFW_SimGetPbkStorage(CFW_PBK_SIM_BARRED_DIALLING, FREE_UTI, adp_pPHBContext->nSimId);		
				#endif
			#else
					ValueSimGetPbkEntry = CFW_SimGetPbkStorage(CFW_PBK_SIM_BARRED_DIALLING, FREE_UTI);
			#endif
					break;

				case MMI_PHB_MSISDN:
			#ifdef __ADP_MULTI_SIM__		
				#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					ValueSimGetPbkEntry = UT_CFW_SimGetPbkStorage(CFW_PBK_ON, FREE_UTI, adp_pPHBContext->nSimId);
				#else
					ValueSimGetPbkEntry = CFW_SimGetPbkStorage(CFW_PBK_ON, FREE_UTI, adp_pPHBContext->nSimId);				
				#endif
			#else
					ValueSimGetPbkEntry = CFW_SimGetPbkStorage(CFW_PBK_ON, FREE_UTI);
			#endif
					break;

				case MMI_PHB_SDN:
			#ifdef __ADP_MULTI_SIM__		
				#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
					ValueSimGetPbkEntry = UT_CFW_SimGetPbkStorage(CFW_PBK_SIM_SERVICE_DIALLING, FREE_UTI, adp_pPHBContext->nSimId);
				#else
					ValueSimGetPbkEntry = CFW_SimGetPbkStorage(CFW_PBK_SIM_SERVICE_DIALLING, FREE_UTI, adp_pPHBContext->nSimId);
				#endif
			#else
					ValueSimGetPbkEntry = CFW_SimGetPbkStorage(CFW_PBK_SIM_SERVICE_DIALLING, FREE_UTI);
			#endif					
					break;

				default:
					break;
			}
			 mmi_trace(g_sw_ADP_PHB,"PHB get entry by index req  CFW_SimGetPbkStorage = %d",ValueSimGetPbkEntry);
			if (ValueSimGetPbkEntry != ERR_SUCCESS)
				adp_phb_get_extra_number_rsp(0, 0, 0);
		}
			break;

		default:
			TBM_EXIT(0x29e3);
			return FALSE;
	}
	
	TBM_EXIT(0x29e3);
	return TRUE;
}

/**************************************************************
	FUNCTION NAME		: ADP_DispatchPhbRspMsg
	PURPOSE				: Message Adaption for PhoneBook RSP.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
	REMARKS			:
**************************************************************/
static BOOL ADP_DispatchPhbRspMsg(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);
	TBM_ENTRY(0x29e4);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_PHB, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif

	switch (pCosEvent->nEventId)
	{
		case EV_CFW_PBK_LIST_CALLLOG_ENTRIES_RSP:
			break;

		default:
			TBM_EXIT(0x29e4);
			return FALSE;
	}
	TBM_EXIT(0x29e4);
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchPhbIndMsg

	PURPOSE				: Message Adaption for PHB

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/
static BOOL ADP_DispatchPhbIndMsg(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_PHB, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif
	
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchPhbMsg

	PURPOSE				: Message Adaption for PHB

	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/

BOOL ADP_DispatchPhbMsg(COS_EVENT *p)
{
	if (p->nEventId >= MSG_ID_MMI_PHB_GET_ENTRY_BY_NAME_REQ && p->nEventId <= MSG_ID_MMI_PHB_STARTUP_READ_NEXT_REQ)
	{
		return ADP_DispatchPhbReqMsg(p);
	}
	else if (p->nEventId >= EV_CFW_PBK_RSP_BASE && p->nEventId <= EV_CFW_PBK_RSP_END_)
	{
		if (ADP_DispatchPhbRspMsg(p))
			return TRUE;
		else if (ADP_DispatchPhbIndMsg(p))
			return TRUE;
	}
	
	return FALSE;
}

#ifdef __ADP_MULTI_SIM__
extern BOOL flag_is_fdn_state_on[];
void ADP_PhbInitFinishOneCardStartAnother(ADP_SIM_ID_T SimId)
{	
	UINT32 ValueSimGetPbkEntry = 2;
	U8 i;
	U8 nDoneSim = 0;
	U8 nSimID =0;


	g_PHBSIMStartFinish[SimId] = TRUE;
	mmi_trace(g_sw_ADP_PHB, "PHB ADP_PhbInitFinishOneCardStartAnother(%d)",SimId);
	//searh for another card to start
	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		mmi_trace(g_sw_ADP_PHB, "PHB %d g_PHBSIMStartInd is %d,g_PHBSIMStartFinish is %d, gSIMIsNotExist is %d, flag_is_fdn_state_on is %d",i,g_PHBSIMStartInd[i],g_PHBSIMStartFinish[i], gSIMIsNotExist[i], flag_is_fdn_state_on[i]);
		if(g_PHBSIMStartInd[i] && !g_PHBSIMStartFinish[i] && !gSIMIsNotExist[i] && !flag_is_fdn_state_on[i])
		{
		#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
			ValueSimGetPbkEntry = UT_CFW_SimGetPbkStorage(CFW_PBK_SIM, FREE_UTI, i);
		#else
			U8 Uti = FREE_UTI;
			nSimID = i;
			mmi_trace(g_sw_ADP_PHB, "PHB ADP_PhbInitFinishOneCardStartAnother, start %d",nSimID);
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
			ValueSimGetPbkEntry = CFW_SimGetPbkStrorageInfo(CFW_PBK_SIM, Uti, nSimID);		
		#endif
		
			if(ValueSimGetPbkEntry != ERR_SUCCESS)
			{
				mmi_trace(g_sw_ADP_PHB, "PHB ADP_PhbInitFinishOneCardStartAnother call CFW_SimGetPbkStrorageInfo fail, 0x%x",ValueSimGetPbkEntry);
				g_PHBSIMStartFinish[i] = TRUE;
				ADP_PhbInitFinishOneCardStartAnother(i);
			}
			else
			{
				return;
			}
		}
	}

	//searh if all sim is done
	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		if(gSIMIsNotExist[i] || g_PHBSIMStartFinish[i] || (flag_is_fdn_state_on[i] && g_PHBSIMStartInd[i]))
			nDoneSim++;
	}
	mmi_trace(g_sw_ADP_PHB, "PHB ADP_PhbInitFinishOneCardStartAnother nDoneSim is %d", nDoneSim);
	if(ADP_SIM_NUMBER == nDoneSim)
	{
		Msg_Adaption_PHB_Init();
	}
}
#endif

#ifdef __ADP_MULTI_SIM__
void Adp_Get_FDN_State(ADP_SIM_ID_T SimId)
{
	ilm_struct ilm_ptr;
	module_type srcmod = MOD_L4C + SimId;
	
	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = srcmod;
	ilm_ptr.msg_id = MSG_ID_MMI_FDN_GET_STATE_IND;
	ilm_ptr.local_para_ptr = NULL;
	OslMsgSendExtQueue(&ilm_ptr);
}
#endif

