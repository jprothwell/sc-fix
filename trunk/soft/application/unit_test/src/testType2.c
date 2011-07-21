#ifndef __TEST_TYPE_C__
#define __TEST_TYPE_C__

#include "unit_test.h"

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
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

#else

#ifdef CFW_SIMU_WINDOW
#include "header.h" //for windows simu
#endif

#endif

#include "testtype2.h"

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
extern UINT8* itoa( INT32 value, UINT8* string, UINT32 radix );
#else

#endif

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
simulator_sms_sim2_storage_t g_simulator_sms_sim2;


static CFW_DELIVERED_DATA cswSim2DeliverData[SIMULATOR_SMS_SIM2_DELIVER];
static CFW_SUBMIT_DATA cswSIM2SubmitData[SIMULATOR_SMS_SIM2_SUBMIT];


simulator_sms_me2_storage_t g_simulator_sms_me2;
static CFW_DELIVERED_DATA cswME2DeliverData[SIMULATOR_SMS_ME2_DELIVER];
static CFW_SUBMIT_DATA cswME2SubmitData[SIMULATOR_SMS_ME2_SUBMIT];

simulator_phb_sim2_storage_t g_simulator_phb_sim2;

static simulator_phb_name_t simu_pbh_name[SIMULATOR_MAX_PHB_SIM2_STORAGE];
static simulator_phb_number_t simu_phb_number[SIMULATOR_MAX_PHB_SIM2_STORAGE];



/**********************************
AddSMSDeliverNode:
The function add SMS deliver node into g_simulator_sms_sim2.
index is the index in the g_simulator_sms_sim2; 
type is ADP_CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO and
ADP_CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO.

**********************************/
void AddSim2SMSDeliverNode(INT32 index)
{
	CFW_SMS_NODE* pSMSNode = &g_simulator_sms_sim2.sms_node[index];
	
	INT8* telNum = "13902145319";
	INT8 str[8];
	
	CFW_DELIVERED_DATA* pData = &cswSim2DeliverData[index];

	
	if ((index % 3) == 0)
	{
		pSMSNode->nStatus = CFW_SMS_STORED_STATUS_UNREAD;
	}
	else
	{
		pSMSNode->nStatus = CFW_SMS_STORED_STATUS_READ;
	}
	pSMSNode->nType = ADP_CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO;

	pSMSNode->nConcatCurrentIndex = index;
	pSMSNode->nConcatNextIndex = 1;
	pSMSNode->nConcatPrevIndex = 0xffff;

	pSMSNode->pNode = pData;

	pData->info.dcs = 0x01;
	
	pData->info.oa_size = (UINT8)SUL_AsciiToGsmBcd(telNum, (UINT8)strlen(telNum), pData->info.oa);

	itoa(index, str, 10);
	 memcpy(pData->info.data, str, 1);
	pData->info.length = 1;

	pData->info.tooa = CFW_TELNUMBER_TYPE_NATIONAL;

	pData->info.tosca = CFW_TELNUMBER_TYPE_NATIONAL;

	pData->info.stat = (U8)pSMSNode->nStatus;

	pData->info.scts.uYear = 2009;
	pData->info.scts.uMonth = index % 12;
	pData->info.scts.uDay = index % 9;
	pData->info.scts.uHour = 12;
	pData->info.scts.uMinute = 23;
	pData->info.scts.uSecond = 15;
	pData->info.scts.iZone = 8;

}

void AddSim2SMSSubmitNode(INT32 index)
{
	CFW_SMS_NODE* pSMSNode = &g_simulator_sms_sim2.sms_node[index];
	
	INT8* telNum = "13902145319";
	INT8 str[8];
	
	CFW_SUBMIT_DATA* pData = &cswSIM2SubmitData[index - SIMULATOR_SMS_SIM2_DELIVER];

	

	pSMSNode->nStatus = CFW_SMS_STORED_STATUS_UNSENT;
	
	pSMSNode->nType = ADP_CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO;

	pSMSNode->nConcatCurrentIndex = index;
	pSMSNode->nConcatNextIndex = 1;
	pSMSNode->nConcatPrevIndex = 0xffff;

	pSMSNode->pNode = pData;

	pData->info.dcs = 0x01;
	
	pData->info.da_size = (U8)SUL_AsciiToGsmBcd(telNum, (UINT8)strlen(telNum), pData->info.da);

	itoa(index, str, 10);
	memcpy(pData->info.data, str, 1);
	pData->info.length = 1;

	pData->info.toda = CFW_TELNUMBER_TYPE_NATIONAL;

	pData->info.tosca = CFW_TELNUMBER_TYPE_NATIONAL;

	pData->info.stat = (U8)pSMSNode->nStatus;

}

void AddME2SMSDeliverNode(INT32 index)
{
	CFW_SMS_NODE* pSMSNode = &g_simulator_sms_me2.sms_node[index];
	
	INT8* telNum = "13902145319";
	
	CFW_DELIVERED_DATA* pData = &cswME2DeliverData[index];
	INT8 str[8];

	
	if ((index % 3) == 0)
	{
		pSMSNode->nStatus = CFW_SMS_STORED_STATUS_UNREAD;
	}
	else
	{
		pSMSNode->nStatus = CFW_SMS_STORED_STATUS_READ;
	}
	pSMSNode->nType = ADP_CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO;

	pSMSNode->nConcatCurrentIndex = index;
	pSMSNode->nConcatNextIndex = 1;
	pSMSNode->nConcatPrevIndex = 0xffff;

	pSMSNode->pNode = pData;

	pData->info.dcs = 0x01;
	
	pData->info.oa_size = (U8)SUL_AsciiToGsmBcd(telNum, (UINT8)strlen(telNum), pData->info.oa);

	itoa(index, str, 10);
	 memcpy(pData->info.data, str, 1);
	pData->info.length = 1;

	pData->info.tooa = CFW_TELNUMBER_TYPE_NATIONAL;

	pData->info.tosca = CFW_TELNUMBER_TYPE_NATIONAL;

	pData->info.stat = (U8)pSMSNode->nStatus;

	pData->info.scts.uYear = 2009;
	pData->info.scts.uMonth = index % 12;
	pData->info.scts.uDay = index % 9;
	pData->info.scts.uHour = 12;
	pData->info.scts.uMinute = 23;
	pData->info.scts.uSecond = 15;
	pData->info.scts.iZone = 8;

}

void AddMe2SMSSubmitNode(INT32 index)
{
	CFW_SMS_NODE* pSMSNode = &g_simulator_sms_me2.sms_node[index];
	
	INT8* telNum = "13902145319";
	char str[8];
	
	CFW_SUBMIT_DATA* pData = &cswME2SubmitData[index - SIMULATOR_SMS_ME2_DELIVER];

	pSMSNode->nStatus = CFW_SMS_STORED_STATUS_UNSENT;
	
	pSMSNode->nType = ADP_CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO;
				
	pSMSNode->nConcatCurrentIndex = index;
	pSMSNode->nConcatNextIndex = 1;
	pSMSNode->nConcatPrevIndex = 0xffff;

	pSMSNode->pNode = pData;

	pData->info.dcs = 0x01;
	
	pData->info.da_size = (U8)SUL_AsciiToGsmBcd(telNum, (UINT8)strlen(telNum), pData->info.da);

	itoa(index, str, 10);
	 memcpy(pData->info.data, str, 1);
	pData->info.length = 1;

	pData->info.toda = CFW_TELNUMBER_TYPE_NATIONAL;

	pData->info.tosca = CFW_TELNUMBER_TYPE_NATIONAL;

	pData->info.stat = (U8)pSMSNode->nStatus;

}


void simulator_init_sms_sim2_storage()
{
	INT32 i;


	for (i = 0; i < SIMULATOR_SMS_SIM2_DELIVER; i++)
	{
		AddSim2SMSDeliverNode(i);
	}

	for (i = SIMULATOR_SMS_SIM2_DELIVER; i < SIMULATOR_SMS_SIM2_DELIVER + SIMULATOR_SMS_SIM2_SUBMIT; i++)
	{
		AddSim2SMSSubmitNode(i);
	}	




	g_simulator_sms_sim2.length = SIMULATOR_MAX_SMS_SIM2_STORAGE;

}
void simulator_init_sms_me2_storage()
{
	INT32 i;

	for (i = 0; i < SIMULATOR_SMS_ME2_DELIVER; i++)
	{
		AddME2SMSDeliverNode(i);
	}

	for (i = SIMULATOR_SMS_ME2_DELIVER; i < SIMULATOR_SMS_ME2_DELIVER + SIMULATOR_SMS_ME2_SUBMIT; i++)
	{
		AddMe2SMSSubmitNode(i);
	}

	g_simulator_sms_me2.length = SIMULAOTR_MAX_SMS_ME2_STORAGE;
}

CFW_SMS_NODE* simulator_get_sms_sim2_storage_node(UINT8 nStorageIndex)
{
	return &g_simulator_sms_sim2.sms_node[nStorageIndex];
}
CFW_SMS_NODE* simulator_get_sms_me2_storage_node(UINT8 nStorageIndex)
{
	return &g_simulator_sms_me2.sms_node[nStorageIndex];
}


CFW_SIM_PBK_ENTRY_INFO* AddSim2PhbNode(INT32 index)
{
	INT8* telNum = "13508881234";
	CFW_SIM_PBK_ENTRY_INFO* pPBKEntry = &g_simulator_phb_sim2.phb_node[index];

	pPBKEntry->pNumber = simu_phb_number[index].number;

	pPBKEntry->nNumberSize = (UINT8)SUL_AsciiToGsmBcd(telNum, strlen(telNum), pPBKEntry->pNumber);

	pPBKEntry->nType = MMI_CSMCC_DEFAULT_ADDR_TYPE;

	itoa(index, simu_pbh_name[index].name, 10);
	
	pPBKEntry->pFullName = simu_pbh_name[index].name;
	pPBKEntry->iFullNameSize = strlen(pPBKEntry->pFullName);

	pPBKEntry->phoneIndex = index + 1;

	return pPBKEntry;

}

void simulator_init_phb_sim2_storage()
{
	INT32 i;

	for (i = 0; i < SIMULATOR_MAX_PHB_SIM2_STORAGE; i++)
	{
		AddSim2PhbNode(i);
	}

	g_simulator_phb_sim2.length = SIMULATOR_MAX_PHB_SIM2_STORAGE;
}
#endif

#endif
