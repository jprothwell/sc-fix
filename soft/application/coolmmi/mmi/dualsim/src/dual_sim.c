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
/*       dual_sim.c                                                           */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   MTPNP layer soruce file, includes the implementation of MTPNP APIs.      */
/*                                                                            */
/******************************************************************************/

#include "mmi_features.h"

#ifdef __MMI_MULTI_SIM__


#ifdef JATAAYU_SUPPORT
#include "jmms_inc.h"
#include "jwap_inc.h"
#include "messagingapi.h"
#include "jdi_communicator.h"
#include "jmms_mmi_cb.h"
#include "jmms_settings.h"
#include "phonebookgprot.h"
#include "jmms_struct_list.h"
#include "custom_nvram_editor_data_item.h"
#include "jprofilehandlertypes.h"
#endif

#include "globaldefs.h"
#include "globalscrenum.h"
#include "frameworkstruct.h"
#include "gui_data_types.h"
#include "eventsgprot.h"
#include "mmi_msg_struct.h"
#include "simdetectiondef.h"
#include "simdetectionhwmapfile.h"
#include "debuginitdef.h"
#include "wgui_status_icons.h"

#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalconstants.h"
#include "debuginitdef.h"	/*debug info*/
#include "protocolevents.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_categories_cm.h"
#include "wgui_categories_inputs.h"
#include "wgui_status_icons.h"
#include "allappgprot.h"
#include "phonebookdef.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "phonebooktypes.h"

#include "callsdefs.h"
#include "settingstructs.h"
#include "settinggprots.h"
#include "simdetectiongprot.h"
#include "protocolevents.h"
#include "callfixeddial.h"
#include "barreddiallist.h"
#include "settingprofile.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "commonscreens.h"
#include "mmi_features.h"
#include "callsetupenum.h"
#include "settingprot.h"
#include "callmanagementiddef.h"
#include "networksetupgprot.h"
#include "organizergprot.h"
#include "idleappdef.h"
#include "idleappprot.h"
#include "simdetectiongexdcl.h"
#include "settingsgexdcl.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "securitysetup.h"
#include "callmanagementgprot.h"
#include "protocolevents.h"
#include "smsapi.h"
#include "smsstruct.h"
#include "wgui_status_icons.h"
#include "smsguiinterfacetype.h"
#include "mmi_msg_context.h"
#include "historygprot.h"
#include "smsguiinterfaceprot.h"
#include "smsfunc.h"
#include "phonebookgprot.h"
#include "commonscreens.h"
#include "settingprofile.h"

#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"

#include "mtpnp_ad_resdef.h"
#include "messagesresourcedata.h"
#include "messagesexdcl.h"
#include "cphsprotocolhandlers.h"
#include "incomingcallmanagementprot.h"
#include "mainmenudef.h"
#include "statusiconres.h"
#include "dual_sim_call.h"
#include "dual_sim.h"
#include "unit_test.h"
#include "callstructuremanagementprot.h"
#include "csp.h"
#include "gbhmi.h"
#include "alarmframeworkprot.h"
#include "callsetup.h"
#include "callhistorygprots.h"
#include "wgui_categories_idlescreen.h"
#include "networksetupdefs.h"
#include "preferrednwksstruct.h"
#include "prefnwksgprot.h"
#include "custresdef.h"
#include "engineermodedef.h"
#include "gpioinc.h"
#include "plmnenum.h"

#include "dual_sim_sms_chat.h"
#include "commonscreens.h"
#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
#endif

#ifdef MMI_ON_WIN32 /* zhuoxz,2009-5-18 */
#include "nvram_data_items.h"
#endif

U16 Slave_g_phb_fdn_count = 0;
extern FDLBDLContext gFDLBDLContext[];
extern FDLBDLContext *gpFDLBDLContext;


static MTPNP_BOOL g_avtive_sms_is_slave = MTPNP_FALSE;

E_MTPNP_AD_CHANNELS gMtpnpChannel[MTPNP_AD_CHANNEL_TOTAL];

void SetReqSrcModeIndex(U8 index, U8 mode_index);
extern void mmi_show_qq_select_confirm(void (*default_entry) (void));

//////////////////////////////////////////////////////////////////////////////////

void MTPNP_AD_Set_CardServiceType(E_MTPNP_AD_SERVICE_TYPE svr_type, U8 nSimID)
{
	g_pwr_context[nSimID].CurrentServiceType = svr_type;
}

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////
// Public APIs
extern void mmi_frm_sms_read_sms_rsp(void* inMsg);
extern void mmi_frm_sms_delete_sms_rsp(void* inMsg);
extern void mmi_frm_sms_new_sms_ind(void* inMsg);
extern void ClearInputEventHandler(U16 device);

static E_MTPNP_AD_NVRAM_STATUS dm_status = MTPNP_AD_NVRAM_CARD1|MTPNP_AD_NVRAM_CARD2|MTPNP_AD_NVRAM_CARD3|MTPNP_AD_NVRAM_CARD4;
static void MTPNP_PFAL_DM_Setup_Init(void);
extern BOOL SSCStringParsing2(void);

static void MTPNP_PFAL_Network_Setup_Init(void);
#ifdef JATAAYU_SUPPORT
JC_BOOLEAN jMMS_IsMMSAutoDownloading(U8 nSIMId);
#endif

void MTPNP_PFAL_InitVoiceMail(void);
void DualModeSwitchCompleteUpdateStatus(void);
void MTPNP_AD_Init(void)
{
	MTPNP_PFAL_InitMessageSetup();
	return;
}

void MTPNP_PFAL_Init(void)
{

	MTPNP_PFAL_DM_Setup_Init();
	MTPNP_AD_InitChannel();
	MTPNP_PFAL_Network_Setup_Init();
	MTPNP_PFAL_InitVoiceMail();
}

E_MTPNP_AD_NVRAM_STATUS MTPNP_AD_Get_Startup_Mode(void)
{
	return dm_status;
}

BOOL MTPNP_AD_Is_Flight_Mode(void)
{
	if(0 == dm_status)
		return TRUE;
	return FALSE;
}
void MTPNP_AD_Init_BootupDM(E_MTPNP_AD_NVRAM_STATUS dm)
{
	U16 i;
	dm_status = dm;

	if (dm_status == MTPNP_AD_NVRAM_FLIGHT)
	{
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			g_pwr_context[i].CurrentServiceType = NO_SERVICE;
			g_pwr_context[i].NetWorkFlag = 1;
		}
	}
}

E_MTPNP_AD_CHANNELS MTPNP_AD_Set_Channel(E_MTPNP_AD_CHANNELS_OWNER owner, E_MTPNP_AD_CHANNELS channel)
{
	if (owner >= MTPNP_AD_CHANNEL_TOTAL)
	{
		return MTPNP_AD_CHANNEL_NONE;
	}

	gMtpnpChannel[owner] = channel;

	if(MTPNP_AD_SMS_CHANNEL == owner)
		mmi_msg_set_msg_need_select_sim_when_send(FALSE);

	return channel;
}

E_MTPNP_AD_CHANNELS MTPNP_AD_Get_Channel(E_MTPNP_AD_CHANNELS_OWNER owner)
{
	if (owner >= MTPNP_AD_CHANNEL_TOTAL)
	{
		return MTPNP_AD_CHANNEL_NONE;
	}

	return gMtpnpChannel[owner];

}

void MTPNP_AD_InitChannel()
{
	int i;

	for (i = 0; i < MTPNP_AD_CHANNEL_TOTAL; i++)
	{
		gMtpnpChannel[i] = MTPNP_AD_CHANNEL_NONE;
	}
}

BOOL MTPNP_AD_IsMasterChannel(E_MTPNP_AD_CHANNELS_OWNER owner)
{
	switch (MTPNP_AD_Get_Channel(owner))
	{

	case MTPNP_AD_CHANNEL_MASTER:

	case MTPNP_AD_CHANNEL_DEFAULT_MASTER:
	{
		return TRUE;
	}
	default:
	{
		return FALSE;
	}

	}

}
BOOL MTPNP_AD_IsSlaveChannel(E_MTPNP_AD_CHANNELS_OWNER owner)
{
	switch (MTPNP_AD_Get_Channel(owner))
	{

        	case MTPNP_AD_CHANNEL_SLAVE:

        	case MTPNP_AD_CHANNEL_DEFAULT_SLAVE:
        	{
        		return TRUE;
        	}

        	default:
        	{
        		return FALSE;
        	}

	}

}

BOOL MTPNP_AD_IsThirdChannel(E_MTPNP_AD_CHANNELS_OWNER owner)
{
	switch (MTPNP_AD_Get_Channel(owner))
	{

        	case MTPNP_AD_CHANNEL_SIM3:

        	case MTPNP_AD_CHANNEL_DEFAULT_SIM3:
        	{
        		return TRUE;
        	}
        	default:
        	{
        		return FALSE;
        	}

	}

}

void MTPNP_AD_SetProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID, E_MTPNP_AD_CHANNELS_OWNER owner)
{
	U8 nSimID;

	nSimID = MTPNP_PFAL_Get_Channel_SIMID(owner);

	SetProtocolEventHandlerExt((PsFuncPtr)funcPtr, eventID,nSimID);
}

void MTPNP_AD_Free_Channel(E_MTPNP_AD_CHANNELS_OWNER owner)
{
	if (owner >= MTPNP_AD_CHANNEL_TOTAL)
	{
		return ;
	}

	gMtpnpChannel[owner] = MTPNP_AD_CHANNEL_NONE;
}

void MTPNP_PFAL_Register_Event_Handler(MTPNP_UINT32 eventID, MTPNP_EVENTHANDLER_FuncPtr funcptr)
{
	SetSlaveProtocolEventHandler(funcptr, eventID);
}

void MTPNP_PFAL_Remove_Event_Handler(MTPNP_UINT32 eventID)
{
	ClearSlaveProtocolEventHandler(eventID);
}

void *MTPNP_OSAL_malloc(const MTPNP_UINT size)
{
	return OslMalloc(size);
	//return NULL;
}

void MTPNP_OSAL_memset(void *dest, MTPNP_UINT8 value, MTPNP_UINT len)
{
	memset(dest, value, len);
	//return;
}

void MTPNP_OSAL_memcpy(void *dest, const void *src, const MTPNP_UINT len)
{
	return;
}

void MTPNP_OSAL_memfree(void *ptrBlock)
{
	OslMfree(ptrBlock);
	//return;
}

MTPNP_INT MTPNP_PFAL_Unicode_String_Length(const MTPNP_CHAR * str)
{
	return pfnUnicodeStrlen(str);
}

MTPNP_CHAR *MTPNP_PFAL_Unicode_String_Copy(MTPNP_CHAR * dest, const MTPNP_CHAR * src)
{
	return pfnUnicodeStrcpy(dest, src);
	//return NULL;
}

MTPNP_CHAR *MTPNP_PFAL_Unicode_Strcat(MTPNP_CHAR * dest, const MTPNP_CHAR * src)
{
	return NULL;
}

MTPNP_UINT16 MTPNP_PFAL_Ansii_To_Unicode_String(MTPNP_CHAR * dest, MTPNP_CHAR * src)
{
	return 0;
}

MTPNP_UINT16 MTPNP_PFAL_Unicode_To_Ansii_String(MTPNP_CHAR * dest, MTPNP_CHAR * src)
{
	return 0;
}

void MTPNP_PFAL_Hide_StatusICON(MTPNP_INT16 icon_id)
{
	HideStatusIcon(icon_id);
}

void MTPNP_PFAL_Show_StatusICON(MTPNP_INT16 icon_id)
{
	ShowStatusIcon(icon_id);
}

void MTPNP_PFAL_Replace_StatusICON(MTPNP_INT16 icon_id, MTPNP_UINT16 image_id)
{
	ChangeStatusIconImage(icon_id, image_id);
}

void MTPNP_PFAL_Blink_StatusICON(MTPNP_UINT16 icon_id)
{
	blink_status_icon(icon_id);
}

extern U8 IsReArrangeNeeded;

void MTPNP_PFAL_Refresh_StatusBar(void)
{
	if (IsReArrangeNeeded)
	{
		arrange_status_icons();
		IsReArrangeNeeded = 0;
	}

	refresh_status_icon_display();
}



////////////////////////////////////////
// Bootup APIs

void MTPNP_AD_Set_PowerOn_Mode(E_MTPNP_AD_POWERON_MODE mode)
{
	return;
}

void MTPNP_PFAL_SIM2_ECCInfoSave(void *inMsg)
{
	return;
}


////////////////////////////////////////
// Mode Switch APIs

typedef struct
{
	E_MTPNP_AD_NVRAM_STATUS temp_mode;
} DM_CONTEXT_T;

static DM_CONTEXT_T g_dm_context = {0};

typedef enum
{
	MTPNP_NO_SIM_INSERTED    = 0,
	MTPNP_ONLY_SIM1_INSERTED = 1,
	MTPNP_ONLY_SIM2_INSERTED = 2,
	MTPNP_DUAL_SIM_INSERTED  = 3,
	MTPNP_SIM_INSERT_STATUS_NONE = 0xFF
} dualmode_sim_insert_status_enum;

typedef enum
{
	MTPNP_SIM1_ONLY_CONFIG  = 0,
	MTPNP_SIM2_ONLY_CONFIG  = 1,
	MTPNP_DAUL_SIM_CONFIG   = 2,
	MTPNP_FLIGHTMODE_CONFIG = 3,
	MTPNP_SIM_CONFIG_STATUS_NONE  = 0xFF
} dualmode_sim_config_status_enum;

typedef struct
{
	dualmode_sim_insert_status_enum sim_insert;
	dualmode_sim_config_status_enum src_status;
	dualmode_sim_config_status_enum dest_status;
} DUALMODE_SWITCH_STATUS;

//static DUALMODE_SWITCH_STATUS dm_switch_status;
static MTPNP_BOOL dm_is_during_switch = MTPNP_FALSE;
static MTPNP_BOOL dm_is_enable_mode_switch = MTPNP_TRUE;
static E_MTPNP_AD_SIMCARD_STATUS dm_card_status[MMI_SIM_NUMBER] = {MTPNP_AD_SIMCARD_NOT_INIT,};


MTPNP_BOOL MTPNP_PFAL_Has_CardValid(void)
{
	U16 i;
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(dm_card_status[i] == MTPNP_AD_SIMCARD_VALID)
			return MTPNP_TRUE;
	}
	return MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_PFAL_Is_All_SIM_Init(void)
{
	U8 i;

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(MTPNP_AD_SIMCARD_NOT_INIT == dm_card_status[i])
			return MTPNP_FALSE;
	}
	return MTPNP_TRUE;
}


static E_MTPNP_AD_NVRAM_STATUS MTPNP_AD_Get_DM_Switch_Tmp(void)
{
	return g_dm_context.temp_mode;
}

static void MTPNP_AD_Set_DM_Switch_Tmp(E_MTPNP_AD_NVRAM_STATUS tmp)
{
	g_dm_context.temp_mode = tmp;
}

void EntryDualModeSwitchProgressing(void)
{
	EntryNewScreen(SCR_DM_SWITCH_PROGRESS, NULL, EntryDualModeSwitchProgressing, NULL);
	SetKeyHandler(NULL, KEY_END, KEY_EVENT_DOWN);
	SetKeyHandler(NULL, KEY_END, KEY_EVENT_UP);
	ShowCategory66Screen(STRING_MTPNP_DM_SETTING,
						 GetRootTitleIcon(MENU_MTPNP_DM_SETTING), 0, 0, 0, 0,
						 (MTPNP_UINT8 *) GetString(STRING_MTPNP_DM_SWITCH), IMG_DM_SWITCH_PROGRESS, NULL);
}

void EntryDualModeSwitchProgress(void)
{
#if 0
	MTPNP_AD_During_Mode_Switch();
#endif

	EntryDualModeSwitchProgressing();

#if 0
	DualModeSwitchStartCallBack();
#endif

	/*
	MTPNP_PFAL_Stop_Timer(MTPNP_MODESWITCH_TIMER);
	MTPNP_PFAL_Start_Timer(MTPNP_MODESWITCH_TIMER, DUALSIM_MODESWITCH_TIMEOUT, DualModeSwitchFailed);
	*/
}

void MTPNP_AD_Set_CardStatus(E_MTPNP_AD_SIMCARD_STATUS card_status)
{
	U8 nSimID; 

	mmi_trace(g_sw_DUALSIM, "MTPNP_AD_Set_CardStatus card_status is %d", card_status);
	
	nSimID = MTPNP_AD_get_protocol_event_src_mode();
	
	dm_card_status[nSimID] = card_status;
}

E_MTPNP_AD_SIMCARD_STATUS MTPNP_AD_Get_CardStatus(U8 nSimID)
{
	return dm_card_status[nSimID];
}

MTPNP_BOOL MTPNP_PFAL_Is_CardAbsent(U8 nSimID)
{
	return MTPNP_AD_Get_CardStatus(nSimID) == MTPNP_AD_SIMCARD_ABSENT?MTPNP_TRUE : MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_PFAL_Is_CardValid(U8 nSimID)
{
	return MTPNP_AD_Get_CardStatus(nSimID) == MTPNP_AD_SIMCARD_VALID ? MTPNP_TRUE : MTPNP_FALSE;
}

U8 MTPNP_AD_Get_UsableSide_Number(void)
{
	U8 number =0;
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if((dm_status&(DM_STATUS_MASK<<i))&&MTPNP_PFAL_Is_CardValid(i))
			number++;
	}
	return number;
}
/*****************************************************************
return the firset usable side index
******************************************************************/
S8 MTPNP_AD_GET_UsableSide_Index(void)
{
	S8 index = -1;
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if((dm_status&(DM_STATUS_MASK<<i))&&MTPNP_PFAL_Is_CardValid(i))
		{
			index = i;
			break;
		}
	}
	return index;
}

MTPNP_BOOL MTPNP_PFAL_Is_Card_Usable(U8 nSimID)
{
	return ((dm_status&(DM_STATUS_MASK<<nSimID))&&MTPNP_PFAL_Is_CardValid(nSimID));
}

U8 MTPNP_PFAL_Get_Channel_SIMID(E_MTPNP_AD_CHANNELS_OWNER owner)
{
	switch (MTPNP_AD_Get_Channel(owner))
	{

		case MTPNP_AD_CHANNEL_MASTER:
		case MTPNP_AD_CHANNEL_DEFAULT_MASTER:
			return 0;

		case MTPNP_AD_CHANNEL_SLAVE:
		case MTPNP_AD_CHANNEL_DEFAULT_SLAVE:
			return 1;
		case MTPNP_AD_CHANNEL_SIM3:
		case MTPNP_AD_CHANNEL_DEFAULT_SIM3:
			return 2;
		case MTPNP_AD_CHANNEL_SIM4:
		case MTPNP_AD_CHANNEL_DEFAULT_SIM4:
			return 3;
		default:
			return 0;
	}
}

void MTPNP_AD_SetMenuHideUnhide(U32 menuID)
{
	U8 i;
	U8 j = 0;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		mmi_frm_hide_menu_item(menuID + i);
		if(MTPNP_PFAL_Is_Card_Usable(i))
		{
			mmi_frm_unhide_menu_item(menuID + i);
			SetReqSrcModeIndex(j++, i);
		}
	}
}

//delete deplicated Functions
/*
E_MTPNP_AD_SIMCARD_STATUS MTPNP_AD_Get_Card1Status(void)
{
	return MTPNP_AD_Get_CardStatus(0);
}

E_MTPNP_AD_SIMCARD_STATUS MTPNP_AD_Get_Card2Status(void)
{
	return MTPNP_AD_Get_CardStatus(1);
}
*/

//((MTPNP_AD_CARD1 | MTPNP_AD_SIMCARD_VALID) + (MTPNP_AD_CARD2 | (MTPNP_AD_SIMCARD_VALID << 1)))
#if (MMI_SIM_NUMBER == 2)

E_MTPNP_AD_STATUS MTPNP_AD_Get_Status(void)
{
	switch (dm_status)
	{

	case MTPNP_AD_NVRAM_CARD1|MTPNP_AD_NVRAM_CARD2:
		return MTPNP_AD_DUALCARD;

	case MTPNP_AD_NVRAM_CARD1:
		return MTPNP_AD_CARD1;

	case MTPNP_AD_NVRAM_CARD2:
		return MTPNP_AD_CARD2;

	case MTPNP_AD_NVRAM_FLIGHT:
		return MTPNP_AD_FLIGHT;

	default:
		return MTPNP_AD_DUALCARD;
	}
}

E_MTPNP_AD_SIMCARD_USABLE_SIDE MTPNP_AD_Get_UsableSide(void)
{
	E_MTPNP_AD_SIMCARD_USABLE_SIDE side = 0;
	U8 validcheck, modecheck;

	/* SIM1 open: 0x01 */
	/* SIM2 open: 0x02 */
	/* SIM1 valid: 0x10 */	
	/* SIM2 valid: 0x20 */				
	switch (MTPNP_AD_Get_Status())
	{

	/* 0011 0011 dualsim usable */
	/* 0010 0011 SIM2 usable */
	/* 0000 0011 No SIM usable */
	/* 0001	0011 SIM1 usable */
	case MTPNP_AD_DUALCARD:
		side = (MTPNP_PFAL_Is_CardValid(0)? MTPNP_AD_SIMCARD_VALID : 0) | (MTPNP_PFAL_Is_CardValid(1) ? (MTPNP_AD_SIMCARD_VALID<<1) : 0)
			   | MTPNP_AD_DUALCARD;
		break;
	
	/* 0000 0000 Flight Mode */
	case MTPNP_AD_FLIGHT:
		side =  MTPNP_AD_DUALSIM_UNUSABLE;
		break;

	/* 0001 0001 SIM1 usable */
	/* 0000 0001 No SIM usable */
	case MTPNP_AD_CARD1:
		side = (MTPNP_PFAL_Is_CardValid(0)? MTPNP_AD_SIMCARD_VALID : 0)
			   | MTPNP_AD_CARD1;
		break;

	/* 0010 0010 SIM2 usable */
	/* 0000 0010 No SIM usable */
	case MTPNP_AD_CARD2:
		side = (MTPNP_PFAL_Is_CardValid(1) ? (MTPNP_AD_SIMCARD_VALID<<1) : 0)
			   | MTPNP_AD_CARD2;

		break;

	default:
		break;
	}

	validcheck = (side & 0xf0) >> 4;
	modecheck = side & 0x0f;
	if (validcheck == 0 )
	{
		if (modecheck == MTPNP_AD_CARD1) //mode: only sim1, no sim valid
		{
			return MTPNP_AD_SIMCARD1_NOSIM_USABLE;
		}
		else if (modecheck == MTPNP_AD_CARD2) //mode: only sim2, no sim valid
		{
			return MTPNP_AD_SIMCARD2_NOSIM_USABLE;
		}
		else
		{
			return MTPNP_AD_SIMCARD_NOSIM_USABLE;
		}
	}
	else if (validcheck == 0x1)
	{
		return MTPNP_AD_SIMCARD1_USABLE;
	}
	else if (validcheck == 0x2)
	{
		return MTPNP_AD_SIMCARD2_USABLE;
	}
	else if (validcheck == 0x3)
	{
		return MTPNP_AD_DUALSIM_USABLE;
	}

	return MTPNP_AD_SIMCARD_NOSIM_USABLE;
}
#endif

BOOL mmi_bootup_is_network_searching_complete(U8 nSimID)
{
	return g_pwr_context[nSimID].NetWorkFlag == 1 ? TRUE : FALSE;
}

MTPNP_BOOL MTPNP_AD_Is_During_Mode_Switch(void)
{
	return dm_is_during_switch;
}

MTPNP_BOOL MTPNP_AD_Is_Enable_Mode_Switch(void)
{
	return dm_is_enable_mode_switch;
}

MTPNP_BOOL MTPNP_PFAL_Is_CardCHVBlocked(U8 nSimID)
{
	return MTPNP_AD_Get_CardStatus(nSimID) == MTPNP_AD_SIMCARD_CHVBLOCKED ? MTPNP_TRUE : MTPNP_FALSE;
}

//delete duplicated function
/*
MTPNP_BOOL MTPNP_PFAL_Is_Card1Absent(void)
{
	return MTPNP_AD_Get_CardStatus(0) == MTPNP_AD_SIMCARD_ABSENT ? MTPNP_TRUE : MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_PFAL_Is_Card2Absent(void)
{
	return MTPNP_AD_Get_CardStatus(1) == MTPNP_AD_SIMCARD_ABSENT ? MTPNP_TRUE : MTPNP_FALSE;
}
MTPNP_BOOL MTPNP_PFAL_Is_Card1Valid(void)
{
	return MTPNP_AD_Get_CardStatus(0) == MTPNP_AD_SIMCARD_VALID ? MTPNP_TRUE : MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_PFAL_Is_Card2Valid(void)
{
	return MTPNP_AD_Get_CardStatus(1) == MTPNP_AD_SIMCARD_VALID ? MTPNP_TRUE : MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_PFAL_Is_Card1CHVBlocked(void)
{
	return MTPNP_AD_Get_CardStatus(0) == MTPNP_AD_SIMCARD_CHVBLOCKED ? MTPNP_TRUE : MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_PFAL_Is_Card2CHVBlocked(void)
{
	return MTPNP_AD_Get_CardStatus(1) == MTPNP_AD_SIMCARD_CHVBLOCKED ? MTPNP_TRUE : MTPNP_FALSE;
}
*/

MTPNP_UINT8 MTPNP_AD_Update_Switch_Table(void)
{
	return 1;
}

MTPNP_BOOL MTPNP_AD_Get_Mode_Item(E_MTPNP_AD_NVRAM_STATUS option, MTPNP_INT8 * index)
{
	if (option >= 5)
		return MTPNP_FALSE;
	else
	{
		*index = option;
		return MTPNP_TRUE;
	}
}
#if 0
static void FillDualModeSwitchStruct(E_MTPNP_AD_NVRAM_STATUS src_status, E_MTPNP_AD_NVRAM_STATUS dest_status)
{
	if (MTPNP_PFAL_Is_Card1Absent())			/* Not Insert SIM1 */
	{
		if (MTPNP_PFAL_Is_Card2Absent())		/* Not Insert SIM2 */
		{
			dm_switch_status.sim_insert = MTPNP_NO_SIM_INSERTED;
		}
		else	/* Insert SIM2 */
		{
			dm_switch_status.sim_insert = MTPNP_ONLY_SIM2_INSERTED;
		}
	}
	else	/* Insert SIM1 */
	{
		if (MTPNP_PFAL_Is_Card2Absent())		/* Not Insert SIM2 */
		{
			dm_switch_status.sim_insert = MTPNP_ONLY_SIM1_INSERTED;
		}
		else	/* Insert SIM2 */
		{
			dm_switch_status.sim_insert = MTPNP_DUAL_SIM_INSERTED;
		}
	}

	switch (src_status)
	{

	case MTPNP_AD_NVRAM_DUALCARD:
		dm_switch_status.src_status = MTPNP_DAUL_SIM_CONFIG;
		break;

	case MTPNP_AD_NVRAM_CARD1:
		dm_switch_status.src_status = MTPNP_SIM1_ONLY_CONFIG;
		break;

	case MTPNP_AD_NVRAM_CARD2:
		dm_switch_status.src_status = MTPNP_SIM2_ONLY_CONFIG;
		break;

	case MTPNP_AD_NVRAM_FLIGHT:
		dm_switch_status.src_status = MTPNP_FLIGHTMODE_CONFIG;
		break;

	default:
		MMI_ASSERT(0);
		break;
	}

	switch (dest_status)
	{

	case MTPNP_AD_NVRAM_DUALCARD:
		dm_switch_status.dest_status = MTPNP_DAUL_SIM_CONFIG;
		break;

	case MTPNP_AD_NVRAM_CARD1:
		dm_switch_status.dest_status = MTPNP_SIM1_ONLY_CONFIG;
		break;

	case MTPNP_AD_NVRAM_CARD2:
		dm_switch_status.dest_status = MTPNP_SIM2_ONLY_CONFIG;
		break;

	case MTPNP_AD_NVRAM_FLIGHT:
		dm_switch_status.dest_status = MTPNP_FLIGHTMODE_CONFIG;
		break;

	default:
		MMI_ASSERT(0);
		break;
	}
}
#endif
static void Terminate_Mode_Switch(void)
{
	if (GetActiveScreenId() != SCR_FLIGHT_MODE_PROGRESS)
	{
		DeleteScreenIfPresent(SCR_DM_SWITCH_PROGRESS);      /* progressing */
		DeleteScreenIfPresent(SCR_STARTUPMODESET);          /* mode switch menu */
		DeleteScreenIfPresent(SCR9003_INITIALSETUP);        /* phone setup */
		DeleteScreenIfPresent(SCR9000_SETTINGS);            /* setting */
		DeleteScreenIfPresent(MAIN_MENU_SCREENID);          /* main menu */
		DeleteScreenIfPresent(SCR_SIM_OPTION);              /* SIM option menu */
	}
	else
	{
		if (IsScreenPresent(SCR_SIM_INSERTION))
		{
			DeleteUptoScrID(SCR_SIM_INSERTION);
		}
		else if (IsScreenPresent(SCR_SIM_BLOCK))
		{
			DeleteUptoScrID(SCR_SIM_BLOCK);
		}
		else
		{
			DeleteUptoScrID(IDLE_SCREEN_ID);
		}
	}

#if 0
	AlmEnableExpiryHandler();

#endif

#if 0
	MTPNP_AD_terminate_Mode_Switch();

#endif
}

void DualModeSwitchCompleteUpdateStatus(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	DisplayPopup((MTPNP_UINT8 *) GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	Terminate_Mode_Switch();
}


static MTPNP_BOOL MTPNP_AD_Update_DM_Setup(void)
{
	U16 i;
	TRC_ENTER_FUNC(g_sw_NW);
	mmi_trace(g_sw_NW, "[221]Old dual-mode is %d,new is %d", dm_status, g_dm_context.temp_mode);
	#if 0
	switch ( g_dm_context.temp_mode)
	{
	case MTPNP_AD_NVRAM_DUALCARD:

		if (dm_status != MTPNP_AD_NVRAM_CARD1) //card1 disable -> card1 enable
		{
			MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[0], (MTPNP_CHAR *) GetString(STRING_MTPNP_SEARCHING_NW));

		}
		
		if (dm_status != MTPNP_AD_NVRAM_CARD2) //card2 disable -> card2 enable
		{
			MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[1], (MTPNP_CHAR *) GetString(STRING_MTPNP_SEARCHING_NW));

		}
		
		break;
	case MTPNP_AD_NVRAM_CARD1:

		if (dm_status != MTPNP_AD_NVRAM_DUALCARD) //card1 disable -> card1 enable
		{
			MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[0], (MTPNP_CHAR *) GetString(STRING_MTPNP_SEARCHING_NW));
		}
		break;
	case MTPNP_AD_NVRAM_CARD2:
		if (dm_status != MTPNP_AD_NVRAM_DUALCARD) //card2 disable -> card2 enable
		{
			MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[1], (MTPNP_CHAR *) GetString(STRING_MTPNP_SEARCHING_NW));
		}
		break;
	case MTPNP_AD_NVRAM_FLIGHT:
		memset(gNWProviderName[1], 0, MAX_LENGTH_DISPLAY_NAME * ENCODING_LENGTH);
		break;
	default:
		break;
	}
	#endif
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if((dm_status&(DM_STATUS_MASK<<i))!=(g_dm_context.temp_mode&(DM_STATUS_MASK<<i)))
		{
			if(g_dm_context.temp_mode&(DM_STATUS_MASK<<i))
			{
				MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[i], (MTPNP_CHAR *) GetString(STRING_MTPNP_SEARCHING_NW));
			}
			else
			{
				MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) gNWProviderName[i], (MTPNP_CHAR *) GetString(STRING_MTPNP_SEARCHING_NW));
			}
		}
	}
	
	dm_status = g_dm_context.temp_mode;

	
	if (dm_status == MTPNP_AD_NVRAM_FLIGHT)
	{
		g_pwr_context[0].actFlightMode = 1;
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			g_pwr_context[i].CurrentServiceType = MTPNP_AD_NO_SERVICE;
		}
	}
	else
	{
		g_pwr_context[0].actFlightMode = 0;
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			g_pwr_context[i].CurrentServiceType = MTPNP_AD_FULL_SERVICE;
		}
	}

	return MTPNP_TRUE;
}

extern U8 gSignalStrengthLevel[];

MTPNP_BOOL MTPNP_AD_MMS_Is_Avail(void)
{
	if (MTPNP_AD_Get_UsableSide_Number() > 0)
		return MTPNP_TRUE;
	return MTPNP_FALSE;
}
extern void jwap_UpdatePushIcon();
void MTPNP_PFAL_Rearrange_StatusBar(void)
{
	E_MTPNP_AD_STATUS status;
	U16 line_id;
	U8 simID;

	status = MTPNP_AD_Get_Startup_Mode();
    
#ifdef JATAAYU_SUPPORT
       HideStatusIcon(STATUS_ICON_WAP);
#ifdef JATAAYU_SUPPORT_MMS
       jmms_HideMmsIcon();
#endif      
#endif       
	mmi_trace(g_sw_IDLE,"MMI_Idle: Func: %s status=%d", __FUNCTION__, status);
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	for(simID = 0; simID < 1; simID++)
#else
	for(simID = 0; simID < MMI_SIM_NUMBER; simID++)
#endif
	{
		line_id = GetCallSetLine(simID);
		if(status&(0x01<<simID))
		{
		#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_SIM_ID__
			#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__
			if(line_id == LINE1)
			{
				MTPNP_PFAL_Replace_StatusICON(STATUS_ICON_SIGNAL_STRENGTH + simID, IMG_SI_MASTER_SIGNAL_STRENGTH_L1 + simID);
			}
			else if(line_id == LINE2)
			{
				MTPNP_PFAL_Replace_StatusICON(STATUS_ICON_SIGNAL_STRENGTH + simID, IMG_SI_MASTER_SIGNAL_STRENGTH_L2 + simID);
			}
			else
			#endif/*__MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__*/
			{
				MTPNP_PFAL_Replace_StatusICON(STATUS_ICON_SIGNAL_STRENGTH + simID, IMG_SI_MASTER_SIGNAL_STRENGTH + simID);
			}
		#else/*__MMI_STATUS_ICON_SIGNAL_WITH_SIM_ID__*/
			#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__
			if(line_id == LINE1)
			{
				MTPNP_PFAL_Replace_StatusICON(STATUS_ICON_SIGNAL_STRENGTH + simID, IMG_SI_SIGNAL_STRENGTH_L1);
			}
			else if(line_id == LINE2)
			{
				MTPNP_PFAL_Replace_StatusICON(STATUS_ICON_SIGNAL_STRENGTH + simID, IMG_SI_SIGNAL_STRENGTH_L2);
			}
			#endif/*__MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__*/
			else
			{
				MTPNP_PFAL_Replace_StatusICON(STATUS_ICON_SIGNAL_STRENGTH + simID, IMG_SI_SIGNAL_STRENGTH);
			}
		#endif/*__MMI_STATUS_ICON_SIGNAL_WITH_SIM_ID__*/
			ChangeStatusIconLevel(STATUS_ICON_SIGNAL_STRENGTH + simID, gSignalStrengthLevel[simID]);
		}
		else
		{
			gSignalStrengthLevel[simID] = 0;
			ChangeStatusIconLevel(STATUS_ICON_SIGNAL_STRENGTH+ simID, gSignalStrengthLevel[simID]);
			MTPNP_PFAL_Hide_StatusICON(STATUS_ICON_GPRS_SERVICE+simID);
			MTPNP_PFAL_Hide_StatusICON(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR+simID);
			MTPNP_PFAL_Hide_StatusICON(STATUS_ICON_ROAMING+simID);
		}

	}

	if(status == 0)
	{//flight mode
		g_pwr_context[simID].IsCampOnTonePlayed = 0;
	}

	/* CFU icon */
	MTPNP_CPHSDisplayCFU();

	//SMS icon
	for(simID = 0; simID < MMI_SIM_NUMBER; simID++)
	{
		if(mmi_msg_is_mem_full_for_simid(simID))
		{
			MTPNP_PFAL_Refresh_StatusBar_SMS_Display(MTPNP_TRUE, MTPNP_FALSE, simID);
		}
		else
		{
			MTPNP_PFAL_Refresh_StatusBar_SMS_Display(MTPNP_FALSE, MTPNP_FALSE, simID);
		}
	}

#ifdef JATAAYU_SUPPORT
#ifdef JATAAYU_SUPPORT_MMS
	if(MTPNP_AD_MMS_Is_Avail())
	{
		jmms_RestoreMmsICon();
	}
#endif	
#endif

	/* handle missed and rejected call icon except flight mode */
	mmi_chist_rearrange_missed_call_status_icon();
	mmi_chist_rearrange_rejected_call_status_icon();


#ifdef __MMI_MESSAGES_CHAT__
	MTPNP_PFAL_Refresh_StatusBar_Chat_Display(status);

#endif

#ifdef __MMI_EMAIL__
	mmi_email_bearer_status_show_icon();

#endif

	MTPNP_PFAL_Refresh_StatusBar();


	MTPNP_PFAL_SMS_Init_Show_Style();
	
#if 0
	MTPNP_PFAL_check_SMS_Waiting_Icon();
#endif

}


#if 0
static void MTPNP_PFAL_Rearrange_RootScreen(DUALMODE_SWITCH_STATUS *dm_switch)
{
	extern MTPNP_UINT16 simErrorDisplayString;
	DUALMODE_SWITCH_STATUS tmp_dm_switch;

	// ÏÈË¢ÐÂslave¿¨µÄservice area.
	IdleRefreshSlaveServiceIndicationArea();

	memcpy(&tmp_dm_switch, dm_switch, sizeof(DUALMODE_SWITCH_STATUS));

	// ÓÐ×¢Íø²Ù×÷²¥·ÅÁåÉù.
	#if 0 //wangzl: 090708 del for bug 12563
	g_pwr_context.IsCampOnTonePlayed = 0;
	g_pwr_context_2.IsCampOnTonePlayed = 0;
	#endif
	if (g_idle_context.IsOnIdleScreen || g_idle_context.IsOnSimErrorScreen)
	{
		/* force to add root screen to history if current is on root screen */
		EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
	}

	// ¿¨1²å.
	if (MTPNP_AD_Get_Card1Status() != MTPNP_AD_SIMCARD_ABSENT)
	{
		// ¿¨2²å.
		if (MTPNP_AD_Get_Card2Status() != MTPNP_AD_SIMCARD_ABSENT)
		{
			// ÓÐÒ»¸ö¿¨±»ËøÏÔÊ¾"¿¨Ëø".
			if ((MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_CARD1 && MTPNP_PFAL_Is_Card1CHVBlocked()) ||
					(MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_CARD2 && MTPNP_PFAL_Is_Card2CHVBlocked()))
			{
				simErrorDisplayString = STR_SIM_BLOCK_MSG;
				HistoryReplace(IDLE_SCREEN_ID, SCR_SIM_BLOCK, EntryScrnSimBlockRsp);
			}
			else
			{
				HistoryReplace(SCR_SIM_BLOCK, IDLE_SCREEN_ID, EntryIdleScreen);
			}
		}

		// Ë«¿¨²å.
		else
		{
			// case 2, 6 ,10
			if (MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_CARD2)
			{
				simErrorDisplayString = STR_SIM_INSERTION_MSG;
				HistoryReplace(IDLE_SCREEN_ID, SCR_SIM_INSERTION, EntryScrnSimErrorScreen);
				HistoryReplace(SCR_SIM_BLOCK, SCR_SIM_INSERTION, EntryScrnSimErrorScreen);
			}
			else
			{
				if (MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_CARD1 && MTPNP_PFAL_Is_Card1CHVBlocked())
				{
					simErrorDisplayString = STR_SIM_BLOCK_MSG;
					HistoryReplace(IDLE_SCREEN_ID, SCR_SIM_BLOCK, EntryScrnSimBlockRsp);
					HistoryReplace(SCR_SIM_INSERTION, SCR_SIM_BLOCK, EntryScrnSimBlockRsp);
				}
				else
				{
					HistoryReplace(SCR_SIM_INSERTION, IDLE_SCREEN_ID, EntryIdleScreen);
					HistoryReplace(SCR_SIM_BLOCK, IDLE_SCREEN_ID, EntryIdleScreen);
				}
			}
		}
	}

	// ¿¨1Î´²å.
	else
	{
		if (MTPNP_AD_Get_Card2Status() != MTPNP_AD_SIMCARD_ABSENT)
		{
			// case 3, 7 ,11
			if (MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_CARD1)
			{
				simErrorDisplayString = STR_SIM_INSERTION_MSG;
				HistoryReplace(IDLE_SCREEN_ID, SCR_SIM_INSERTION, EntryScrnSimErrorScreen);
				HistoryReplace(SCR_SIM_BLOCK, SCR_SIM_INSERTION, EntryScrnSimErrorScreen);
			}
			else
			{
				if (MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_CARD2 && MTPNP_PFAL_Is_Card2CHVBlocked())
				{
					simErrorDisplayString = STR_SIM_BLOCK_MSG;
					HistoryReplace(IDLE_SCREEN_ID, SCR_SIM_BLOCK, EntryScrnSimBlockRsp);
					HistoryReplace(SCR_SIM_INSERTION, SCR_SIM_BLOCK, EntryScrnSimBlockRsp);
				}
				else
				{
					HistoryReplace(SCR_SIM_INSERTION, IDLE_SCREEN_ID, EntryIdleScreen);
					HistoryReplace(SCR_SIM_BLOCK, IDLE_SCREEN_ID, EntryIdleScreen);
				}
			}
		}
		else
		{
			// case 1, 5 ,7
			// still insert sim screen
			if (MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_CARD1 || MTPNP_AD_Get_Startup_Mode() == MTPNP_AD_NVRAM_CARD2)
			{
				simErrorDisplayString = STR_SIM_INSERTION_MSG;
				HistoryReplace(IDLE_SCREEN_ID, SCR_SIM_INSERTION, EntryScrnSimErrorScreen);
			}
			else
			{
				HistoryReplace(SCR_SIM_INSERTION, IDLE_SCREEN_ID, EntryIdleScreen);
			}
		}
	}

	/* reset all channels because the previous channel may no longer available after mode switch */
	MTPNP_AD_Free_Channel(MTPNP_AD_CALL_CHANNEL);

	MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);

	MTPNP_AD_Free_Channel(MTPNP_AD_DATA_SERVICE_CHANNEL);

	MTPNP_AD_Free_Channel(MTPNP_AD_AUDIO_RECORD);

	/* sim error menu */
#if 0
	if (IsScreenPresent(IDLE_SCREEN_ID))
	{
		mmi_sim_error_unhide_menu_items();
	}
	else
	{
		mmi_sim_error_hide_menu_items();
	}

#endif
}
#endif /* Guoj delete. It's not used now at 2009-8-20 */

#if 0
static void MTPNP_PFAL_Rearrange_NetworkIcons(DUALMODE_SWITCH_STATUS *dm_switch)
{

	DUALMODE_SWITCH_STATUS tmp_dm_switch;

	memcpy(&tmp_dm_switch, dm_switch, sizeof(DUALMODE_SWITCH_STATUS));

	if (tmp_dm_switch.src_status == MTPNP_FLIGHTMODE_CONFIG)
	{
		mmi_flight_mode_update_status_icon();
		mmi_flight_mode_unhide_menu_items();
	}

	if (tmp_dm_switch.dest_status == MTPNP_FLIGHTMODE_CONFIG)
	{
		mmi_flight_mode_update_status_icon();
		mmi_flight_mode_hide_menu_items();
	}


}
#endif

static void DualModeSwitchUpdateMenuItem(void)
{
#if 0

	if ((dm_switch_status.dest_status != MTPNP_FLIGHTMODE_CONFIG) && (dm_switch_status.src_status == MTPNP_FLIGHTMODE_CONFIG))
	{
		kal_prompt_trace(MOD_MMI, "flight mode -> normal mode");
#ifdef __MMI_BT_SUPPORT__
		kal_prompt_trace(MOD_MMI, "flight mode -> normal mode : unhide MENU_CONN_BT_MAIN");
		mmi_frm_unhide_menu_item(MENU_CONN_BT_MAIN);
#endif /* __MMI_BT_SUPPORT__ */
#ifdef __MMI_IRDA_SUPPORT__
		kal_prompt_trace(MOD_MMI, "flight mode -> normal mode : unhide MENU_CONN_IRDA_MAIN");
		mmi_frm_unhide_menu_item(MENU_CONN_IRDA_MAIN);
#endif /* __MMI_IRDA_SUPPORT__ */
	}
	else if ((dm_switch_status.dest_status == MTPNP_FLIGHTMODE_CONFIG) && (dm_switch_status.src_status != MTPNP_FLIGHTMODE_CONFIG))
	{
		kal_prompt_trace(MOD_MMI, "normal mode -> flight mode");
#ifdef __MMI_BT_SUPPORT__
		kal_prompt_trace(MOD_MMI, "flight mode -> normal mode : hide MENU_CONN_BT_MAIN");
		mmi_frm_hide_menu_item(MENU_CONN_BT_MAIN);
#endif /* __MMI_BT_SUPPORT__ */
#ifdef __MMI_IRDA_SUPPORT__
		kal_prompt_trace(MOD_MMI, "flight mode -> normal mode : hide MENU_CONN_IRDA_MAIN");
		mmi_frm_hide_menu_item(MENU_CONN_IRDA_MAIN);
#endif /* __MMI_IRDA_SUPPORT__ */
	}
	else
	{
		kal_prompt_trace(MOD_MMI, "sim_insert=%d,src_status=%d,dest_status=%d",
						 dm_switch_status.sim_insert, dm_switch_status.src_status, dm_switch_status.dest_status);
	}

#endif
}


static void DualModeSwitchCompleteCallBack(void)
{
#if 0
	mmi_idle_notify_dual_sim_status_to_wap();
#endif

//	MTPNP_PFAL_Rearrange_RootScreen(&dm_switch_status);
	//MTPNP_PFAL_Rearrange_NetworkIcons(&dm_switch_status);
	DualModeSwitchUpdateMenuItem();
}


#if 0
MTPNP_BOOL MTPNP_AD_Get_Network_ID(MTPNP_UCHAR * plmn)
{
	return MTPNP_FALSE;
}
#endif /* Guoj delete. It's not used now at 2009-9-1 */

void MTPNP_PFAL_RefreshNwProviderName(MTPNP_UCHAR *plmn)
{
	S8 *Temp=NULL;
	S8 pOutBuffer[(MAX_PLMN_LEN+2)*ENCODING_LENGTH];
	Temp = GetCurrentPLMNName((S8*) (plmn));
	memset(gNWProviderName[1],0,MAX_LENGTH_DISPLAY_NAME * ENCODING_LENGTH);
	if(Temp== NULL) 
	{
		AnsiiToUnicodeString(pOutBuffer,(PS8)plmn );
		pfnUnicodeStrcpy((PS8)gNWProviderName[1],(PS8)pOutBuffer);
	}
	else 
	{
		pfnUnicodeStrcpy((PS8)gNWProviderName[1],Temp);
	}
}

#if 0
static void MTPNP_PFAL_Refresh_Card2PLMN(void)
{
	MTPNP_UCHAR card2_plmn[SLAVE_NETWORK_ID_LEN];

	MTPNP_OSAL_memset(card2_plmn, 0, SLAVE_NETWORK_ID_LEN);
	MTPNP_AD_Get_Network_ID(card2_plmn);
	MTPNP_PFAL_RefreshNwProviderName(card2_plmn);
}
#endif 
/*
#if (defined (__NATIVE_JAVA__))    
void gcj_NativeJavaQuit(unsigned char isQuit);
#endif
*/
void DualModeSwitchComplete(void)
{
	/*
	MTPNP_PFAL_Stop_Timer(MTPNP_MODESWITCH_TIMER);
	*/
	MTPNP_AD_Update_DM_Setup();

	/* complete callback shall be executed before MTPNP rearrange status bar and menu items */
	DualModeSwitchCompleteCallBack();

	MTPNP_PFAL_Rearrange_StatusBar();

	mmi_trace(g_sw_BT, "DualModeSwitchComplete::dm_status=%d", dm_status);

#if defined(__MMI_BT_SUPPORT__)
	if(mmi_bootup_get_active_flight_mode() == FLIGHTMODE_OFF)
	{
		mmi_trace(g_sw_BT, "DualModeSwitchComplete::mmi_bt_reset_hw_on");
		if(!mmi_bt_is_power_on(FALSE))
			mmi_bt_reset_hw_on();
	}
	else
	{
		mmi_trace(g_sw_BT, "DualModeSwitchComplete::mmi_bt_reset_hw_off");
		mmi_bt_reset_hw_off();
	}
#endif
/*
#if (defined (__NATIVE_JAVA__))    
	gcj_NativeJavaQuit(mmi_bootup_get_active_flight_mode());
#endif
*/
	DualModeSwitchCompleteUpdateStatus();
}

void DualModeSwitchFailed(void)
{
	/*
	    MTPNP_PFAL_Stop_Timer(MTPNP_MODESWITCH_TIMER);
	    MTPNP_AD_Update_DM_Setup();

	    MTPNP_PFAL_Update_FlightMode_Status();

	    MTPNP_PFAL_Refresh_Slave_Idle_ServiceArea();
	    MTPNP_PFAL_Rearrange_StatusBar();
	    MTPNP_PFAL_Rearrange_MenuItem();
	*/

#if 0
	DualModeSwitchFailCallBack();
#endif

	DisplayPopup((MTPNP_UINT8 *) GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, ST_NOTIFYDURATION, ERROR_TONE);

	Terminate_Mode_Switch();
}

U8 activeDMModeIndex;
U8 newDMMode = 0;
U8 currentDMMode = 0;
U8 DMModeEnabled[MMI_SIM_NUMBER];

void EntryStartupModeSetScreen(void);
void SaveDmSet(void);
void CheckDmMode(void);

void SetDMModeIndex(S32 nIndex)
{
	activeDMModeIndex = (U8) nIndex;
}

void EnableDisableDMMode(void)
{
	newDMMode = 0x01<<activeDMModeIndex;

	if(DMModeEnabled[activeDMModeIndex])
	{
		DMModeEnabled[activeDMModeIndex] = 0;
		currentDMMode&=~newDMMode;
	}
	else
	{
		DMModeEnabled[activeDMModeIndex] = 1;
		currentDMMode|=newDMMode;
	}
	mmi_trace(g_sw_NW, "EnableDisableDMMode, currentDMMode is 0x%x",currentDMMode);
	standard_check_list_handle_item_select(activeDMModeIndex);

	ChangeRightSoftkey(STR_GLOBAL_SAVE, 0);
	SetRightSoftkeyFunction(CheckDmMode, KEY_EVENT_UP);
	
}

void SaveDmSet(void)
{
	E_MTPNP_AD_NVRAM_STATUS curr_dm_setting;
	U8 i;
	U8 mask = 0x01;



	curr_dm_setting = MTPNP_AD_Get_Startup_Mode();

	newDMMode = 0;

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(DMModeEnabled[i])
		{
			newDMMode|= mask;
		}
		mask=mask<<1;
	}
	
	mmi_trace(g_sw_NW, "SaveDmSet, curr_dm_setting is 0x%x,newDMMode is 0x%x", curr_dm_setting, newDMMode);
	
	if (curr_dm_setting == newDMMode)
	{
		DisplayPopup((MTPNP_UINT8 *) GetString(STRING_MTPNP_SWITCH_SAME_MODE),
					 IMG_GLOBAL_INFO, 0, ST_NOTIFYDURATION, WARNING_TONE);
		return;
	}

	//FillDualModeSwitchStruct(curr_dm_setting, newDMMode);

	MTPNP_AD_Set_DM_Switch_Tmp(newDMMode);

	/* entry switch progress screen */
	EntryDualModeSwitchProgress();

	MTPNP_PFAL_Switch_Mode(curr_dm_setting);
}
void GoBackDMSetHistory(void)
{
	GoBacknHistory(1);
}
void CheckDmMode(void)
{

	/*
	ShowCategory2Screen (STRING_MTPNP_DM_SETTING, 0,
								STR_GLOBAL_YES, 0,
								STR_GLOBAL_NO, 0, 
								STR_GLOBAL_SAVE, NULL);
	*/
	DisplayConfirm(STR_GLOBAL_YES, 0, STR_GLOBAL_NO, 0, get_string(STR_GLOBAL_SAVE), IMG_GLOBAL_QUESTION, 0);

	SetLeftSoftkeyFunction (SaveDmSet, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackDMSetHistory, KEY_EVENT_UP);
   
}
void PreEntryStartupModeSetScreen(void)
{
	E_MTPNP_AD_NVRAM_STATUS status;
	U16 nNumofItem;
	U16 i;
	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(PreEntryStartupModeSetScreen);
		return;
	}
	status = MTPNP_AD_Get_Startup_Mode();
	
	nNumofItem = MMI_SIM_NUMBER;
	
	for(i = 0; i < nNumofItem; i++)
	{		
		DMModeEnabled[i] = status & (0x01<<i);
	}
	
	currentDMMode = status;
	
	EntryStartupModeSetScreen();
	
}
void EntryStartupModeSetScreen(void)
{
    	U16 nStrItemList[MAX_SUB_MENUS]; 

	U16 nNumofItem;
	U8* guiBuffer;
	U16 i;
	
	if (MTPNP_AD_Is_During_Mode_Switch())
	{
		return;
	}

	/* judge whether enable switch mode */
	if (!MTPNP_AD_Is_Enable_Mode_Switch())
	{
		DisplayPopup((MTPNP_UINT8 *) GetString(STRING_MTPNP_DM_DISABLESWTH),
	             			IMG_GLOBAL_UNFINISHED, 1, CS_NOTIFYDURATION, ERROR_TONE);
		return;
	}
	
	EntryNewScreen(SCR_STARTUPMODESET, NULL, EntryStartupModeSetScreen, NULL);
    
	guiBuffer = GetCurrGuiBuffer(SCR_STARTUPMODESET);/* Getting the Index from history  */

	nNumofItem = MMI_SIM_NUMBER;
	
	mmi_trace(g_sw_NW, "EntryStartupModeSetScreen, currentDMMode is 0x%x, nNumofItem is %d", currentDMMode, nNumofItem);
	
	for(i = 0; i < nNumofItem;)
	{
		nStrItemList[i++] = STRING_MTPNP_DM_CARD1_OPEN + i;
	}
	
	RegisterHighlightHandler(SetDMModeIndex);

	ShowCategory13Screen(STRING_MTPNP_DM_SETTING, GetRootTitleIcon(MENU_MTPNP_DM_SETTING),
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  nNumofItem, nStrItemList, DMModeEnabled, 0, guiBuffer);

	SetLeftSoftkeyFunction(EnableDisableDMMode,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


void HighlightStartupModeSetMain(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(PreEntryStartupModeSetScreen, KEY_EVENT_UP);
	SetKeyHandler(PreEntryStartupModeSetScreen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

static void MTPNP_PFAL_DM_Setup_Init(void)
{
	/* hilite */
#if 0
	SetHiliteHandler(MENU_MTPNP_SETTING, HighlightDMSetMain);
#endif

	SetHiliteHandler(MENU_MTPNP_DM_SETTING, HighlightStartupModeSetMain);

#if 0
	/* help information */
	SetHiliteHandler(MENU_MTPNP_HELPINFO, HightlightShowMTPNPHelpInfo);
#endif

	/* hint */
#if 0
	SetHintHandler(MENU_MTPNP_DM_SETTING, HintStartupModeSet);
#endif

#if 0
	MTPNP_PFAL_Register_Event_Handler(MTPNP_PFAL_MSG_VERIFY_SIM1, MTPNP_PFAL_Entry_SIM1CHV_Verify);
	MTPNP_PFAL_Register_Event_Handler(MTPNP_PFAL_MSG_VERIFY_SIM2, MTPNP_PFAL_Entry_SIM2CHV_Verify);
#endif
}

static void mmi_dm_set_rsp_hdlr(void *msg_ptr)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	mmi_nw_cfun_state_rsp_struct *p = (mmi_nw_cfun_state_rsp_struct *)msg_ptr;
	U8 i;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	mmi_trace(g_sw_NW, "[221]MMI got PRT_NW_CFUN_STATE_RSP from ADP. ->result=%d", p->result);

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		ClearProtocolEventHandlerExt(PRT_NW_CFUN_STATE_RSP,i);
	}

	if (p->result == KAL_TRUE)
		DualModeSwitchComplete();
	else
		DualModeSwitchFailed();
}


/*
0001 0001	17		dual sim card
0100 0001	65		sim card1 only
0001 0100	20		sim card2 only
0100 0100	68		flight mode
**/
#define CFUN_BITS_DUAL_SIM			17
#define CFUN_BITS_SIM_1				65
#define CFUN_BITS_SIM_2				20
#define CFUN_BITS_FLIGHT_MODE		68
#ifdef JATAAYU_SUPPORT
extern void jmms_abort_auto_downloading(UINT8 nSimID);
#endif
E_MTPNP_AD_MODE_SWITCH_STATE MTPNP_PFAL_Switch_Mode(E_MTPNP_AD_NVRAM_STATUS curr_status)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	E_MTPNP_AD_NVRAM_STATUS switch_status_tmp;
	mmi_nw_cfun_state_req_struct *p = NULL;
	ilm_struct ilm = {0};
	U16 i;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	switch_status_tmp = MTPNP_AD_Get_DM_Switch_Tmp();

	p = (mmi_nw_cfun_state_req_struct *)OslMalloc(sizeof(mmi_nw_cfun_state_req_struct));
	MMI_ASSERT(p != NULL);
	memset(p, 0, sizeof(mmi_nw_cfun_state_req_struct));
#if 0
	switch (switch_status_tmp)
	{

	case MTPNP_AD_NVRAM_DUALCARD:
		p->cfun_state = CFUN_BITS_DUAL_SIM;
		break;

	case MTPNP_AD_NVRAM_CARD1:
		p->cfun_state = CFUN_BITS_SIM_1;
		break;

	case MTPNP_AD_NVRAM_CARD2:
		p->cfun_state = CFUN_BITS_SIM_2;
		break;

	case MTPNP_AD_NVRAM_FLIGHT:
		p->cfun_state = CFUN_BITS_FLIGHT_MODE;
		break;

	default:
		MMI_ASSERT(0);
		break;
	}
#endif
	p->cfun_state = switch_status_tmp;

	memset(&ilm, 0, sizeof(ilm_struct));

	ilm.dest_mod_id = MOD_L4C;
	ilm.src_mod_id = MOD_MMI;
	ilm.sap_id = CSM_L4C_SAP;
	ilm.local_para_ptr = (local_para_struct *)p;
	ilm.msg_id = PRT_NW_CFUN_STATE_REQ;

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetProtocolEventHandlerExt(mmi_dm_set_rsp_hdlr, PRT_NW_CFUN_STATE_RSP,i);
	}

	mmi_trace(g_sw_NW, "[221]MMI send PRT_NW_CFUN_STATE_REQ to ADP");
	OslMsgSendExtQueue(&ilm);

	return MTPNP_AD_SWITCH_SUCCEED;
}


////////////////////////////////////////
// Setting-CHV APIs

void MTPNP_AD_Get_SIM_Status_For_Lock(void)
{
	return;
}

MTPNP_UINT8 MTPNP_AD_Get_CHV1_Count(void)
{
	return 0;
}

MTPNP_UINT8 MTPNP_AD_Get_UBCHV1_Count(void)
{
	return 0;
}

void MTPNP_AD_SetSIMLockReq(void)
{
	return;
}

MTPNP_BOOL MTPNP_AD_SIM_CHV1_Is_Enable(void)
{
	return MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_AD_SIM_CHV1_Is_Disable(void)
{
	return MTPNP_FALSE;
}

void MTPNP_AD_Get_SIM_Status_For_CHV2(void)
{
	return;
}

MTPNP_UINT8 MTPNP_AD_Get_CHV2_Count(void)
{
	return 0;
}

MTPNP_UINT8 MTPNP_AD_Get_UBCHV2_Count(void)
{
	return 0;
}

void MTPNP_AD_Validate_CHV(void)
{
	return;
}

MTPNP_BOOL MTPNP_AD_CHV_Operation_Rsp(MTPNP_BOOL result)
{
	return MTPNP_FALSE;
}

void MTPNP_AD_ADN_SetLoadFinish(MTPNP_BOOL value)
{
	return;
}


////////////////////////////////////////
// Network Setup APIs

static void EntryDMNETSETMain(void);

static void HighlightDMNETSETMain(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryDMNETSETMain, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(EntryDMNETSETMain, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

static void EntryDMNETSETMain(void)
{
	MTPNP_AD_SetMenuHideUnhide(MENU9185_NETWORK_SETUP);
	mmi_mtpnp_entry_menu(SCR_MTPNP_DM_NETSET_MAIN, NULL, EntryDMNETSETMain,
							MENU_MTPNP_DM_NETWORK_SETUP, STR_ID_NETSET_MITEM9185, MAIN_MENU_TITLE_SETTINGS_ICON);
}


void MTPNP_PFAL_Network_Setup_Init(void)
{
	SetHiliteHandler(MENU_MTPNP_DM_NETWORK_SETUP, HighlightDMNETSETMain);	
}
////////////////////////////////////////
// Call APIs


////////////////////////////////////////
// Phonebook APIs
extern mmi_phb_context_struct g_phb_cntx;
extern UINT32 copyEntry;
extern UINT32 moveEntry;
extern UINT32 duplicateEntry;
extern MMI_PHB_STORAGE_LOCATION phbcopymovedes;
extern MMI_PHB_STORAGE_LOCATION phbcopymovesrc;

extern U8 g_phb_temp_storage;
#if (MAX_PB_ENTRIES)
extern U16 g_phb_name_index[MAX_PB_ENTRIES];	/*Sorting Order by Name Field.*/
#else
extern U16 g_phb_name_index[1];	/*Sorting Order by Name Field.*/
#endif /* MAX_PB_ENTRIES */
/****************************************************************
**
**
**
**
**
*****************************************************************/
void MTPNP_PFAL_Slave_PHB_Init(void)
{
	/* regester menu hightlight and hint */

	SetHiliteHandler(MENU_ID_PHB_COPY_SIM2_TO_PHONE, HighlightSlaveCopylEntrySIM2Phone);
	SetHiliteHandler(MENU_ID_PHB_MOVE_SIM2_TO_PHONE, HighlightSlaveMovelEntrySIM2Phone);
	SetHiliteHandler(MENU_ID_PHB_COPY_PHONE_TO_SIM2, HighlightSlaveCopylEntrySIM2Phone);
	SetHiliteHandler(MENU_ID_PHB_MOVE_PHONE_TO_SIM2, HighlightSlaveMovelEntrySIM2Phone);

	SetHiliteHandler(MENU_ID_PHB_COPY_SIM1_TO_SIM2, HighlightSlaveCopylEntrySIM2SIM);
	SetHiliteHandler(MENU_ID_PHB_MOVE_SIM1_TO_SIM2, HighlightSlaveMovelEntrySIM2SIM);
	SetHiliteHandler(MENU_ID_PHB_COPY_SIM2_TO_SIM1, HighlightSlaveCopylEntrySIM2SIM);
	SetHiliteHandler(MENU_ID_PHB_MOVE_SIM2_TO_SIM1, HighlightSlaveMovelEntrySIM2SIM);

	SetHiliteHandler(MENU_ID_PHB_COPY_SIM1_TO_PHONE, mmi_phb_highlight_op_copy_sim1_phone);
	SetHiliteHandler(MENU_ID_PHB_MOVE_SIM1_TO_PHONE, mmi_phb_highlight_op_move_sim1_phone);
	SetHiliteHandler(MENU_ID_PHB_COPY_PHONE_TO_SIM1, mmi_phb_highlight_op_copy_sim1_phone);
	SetHiliteHandler(MENU_ID_PHB_MOVE_PHONE_TO_SIM1, mmi_phb_highlight_op_move_sim1_phone);

	SetHiliteHandler(MENU_ID_PHB_COPY_ALL_SIM2_TO_PHONE, HighlightSlaveCopyAllSIM2toPhone);
	SetHiliteHandler(MENU_ID_PHB_COPY_ALL_PHONE_TO_SIM2, HighlightSlaveCopyAllPhonetoSIM2);
	SetHintHandler(MENU_ID_PHB_COPY_ALL_SIM2_TO_PHONE, HintSlaveAddEntry);
	SetHintHandler(MENU_ID_PHB_COPY_ALL_PHONE_TO_SIM2, mmi_phb_hint_copy_all_from_phone);

	SetHiliteHandler(MENU_ID_PHB_COPY_ALL_SIM1_TO_SIM2, HighlightSlaveCopyAllSIM1toSIM2);
	SetHiliteHandler(MENU_ID_PHB_COPY_ALL_SIM2_TO_SIM1, HighlightSlaveCopyAllSIM2toSIM1);
	SetHintHandler(MENU_ID_PHB_COPY_ALL_SIM1_TO_SIM2, mmi_phb_hint_copy_all_from_sim);
	SetHintHandler(MENU_ID_PHB_COPY_ALL_SIM2_TO_SIM1, HintSlaveAddEntry);

	SetHiliteHandler(MENU_ID_PHB_COPY_ALL_SIM1_TO_PHONE		, mmi_phb_highlight_copy_all_from_sim);
	SetHiliteHandler(MENU_ID_PHB_COPY_ALL_PHONE_TO_SIM1		, mmi_phb_highlight_copy_all_from_phone);
	SetHintHandler(MENU_ID_PHB_COPY_ALL_SIM1_TO_PHONE		, mmi_phb_hint_copy_all_from_sim);
	SetHintHandler(MENU_ID_PHB_COPY_ALL_PHONE_TO_SIM1		, mmi_phb_hint_copy_all_from_phone);


}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void HintSlaveAddEntry(MTPNP_UINT16 index)
{
	MTPNP_CHAR simStatus[40];
	MTPNP_CHAR ascii_status[20];

	memset((MTPNP_CHAR *) hintData[index], 0, sizeof(hintData[index]));

	sprintf(ascii_status, "%d/%d",  g_phb_cntx.sim_used[1], g_phb_cntx.sim_total[1]);
	AnsiiToUnicodeString(simStatus, ascii_status);

	pfnUnicodeStrcpy((S8*) hintData[index], simStatus);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void HighlightSlaveCopylEntrySIM2Phone(void)
{
	if (g_phb_name_index[g_phb_cntx.active_index] < MAX_PB_PHONE_ENTRIES)
	{
		phbcopymovesrc = MMI_NVRAM;
		phbcopymovedes = MMI_SIM2;
	}
	else
	{
		phbcopymovesrc = MMI_SIM2;
		phbcopymovedes = MMI_NVRAM;
	}

	SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_confirm, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_entry_op_copy_confirm,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);		
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void HighlightSlaveMovelEntrySIM2Phone(void)
{
	if (g_phb_name_index[g_phb_cntx.active_index] < MAX_PB_PHONE_ENTRIES)
	{
		phbcopymovesrc = MMI_NVRAM;
		phbcopymovedes = MMI_SIM2;
	}
	else
	{
		phbcopymovesrc = MMI_SIM2;
		phbcopymovedes = MMI_NVRAM;
	}

	SetLeftSoftkeyFunction(mmi_phb_entry_op_move_confirm, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_entry_op_move_confirm,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);		
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void HighlightSlaveCopylEntrySIM2SIM(void)
{
	if (g_phb_name_index[g_phb_cntx.active_index] >= MAX_PB_PHONE_ENTRIES && g_phb_name_index[g_phb_cntx.active_index] < MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES)
	{
		phbcopymovesrc = MMI_SIM;
		phbcopymovedes = MMI_SIM2;
	}
	else
	{
		phbcopymovesrc = MMI_SIM2;
		phbcopymovedes = MMI_SIM;
	}

	SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_confirm, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_entry_op_copy_confirm,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);		
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void HighlightSlaveMovelEntrySIM2SIM(void)
{
	if (g_phb_name_index[g_phb_cntx.active_index] >= MAX_PB_PHONE_ENTRIES && g_phb_name_index[g_phb_cntx.active_index] < MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES)
	{
		phbcopymovesrc = MMI_SIM;
		phbcopymovedes = MMI_SIM2;
	}
	else
	{
		phbcopymovesrc = MMI_SIM2;
		phbcopymovedes = MMI_SIM;
	}

	SetLeftSoftkeyFunction(mmi_phb_entry_op_move_confirm, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

	SetKeyHandler(mmi_phb_entry_op_move_confirm,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);		
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void HighlightSlaveCopyAllSIM2toPhone(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	g_phb_cntx.selected_storage = MMI_SIM2;

	phbcopymovesrc = MMI_SIM2;
	phbcopymovedes = MMI_NVRAM;

	if (g_phb_cntx.sim_used[1])
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_all_confirm, KEY_EVENT_UP);
	}
	else
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void HighlightSlaveCopyAllPhonetoSIM2(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	g_phb_cntx.selected_storage = MMI_NVRAM;

	phbcopymovesrc = MMI_NVRAM;
	phbcopymovedes = MMI_SIM2;

	if (g_phb_cntx.phone_used)
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_all_confirm, KEY_EVENT_UP);
	}
	else
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void HighlightSlaveCopyAllSIM1toSIM2(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	g_phb_cntx.selected_storage = MMI_SIM;

	phbcopymovesrc = MMI_SIM;
	phbcopymovedes = MMI_SIM2;

	if (g_phb_cntx.sim_used)
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_all_confirm, KEY_EVENT_UP);
	}
	else
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void HighlightSlaveCopyAllSIM2toSIM1(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	g_phb_cntx.selected_storage = MMI_SIM2;

	phbcopymovesrc = MMI_SIM2;
	phbcopymovedes = MMI_SIM;

	if (g_phb_cntx.sim_used[1])
	{
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_phb_entry_op_copy_all_confirm, KEY_EVENT_UP);
	}
	else
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
}



/****************************************************************
**
**
**
**
**
*****************************************************************/
void MTPNP_AD_ADN_Startup_Read(ST_MTPNP_DATA_PB_CAPACITY *info)
{
	return;
}

MTPNP_BOOL MTPNP_AD_ADN_READALL_FINISH(MTPNP_UINT8 max_name_len, MTPNP_UINT16 capacity, MTPNP_UINT8 max_number_len)
{
	return MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_AD_ADN_SIM2_Is_LoadFinished(void)
{
	return MTPNP_FALSE;
}

MTPNP_UINT8 MTPNP_AD_ADN_SIM2GetTagLen(void)
{
	return 0;
}

void MTPNP_AD_ADN_SIM2SetTagSupport(MTPNP_UINT8 len)
{
	return;
}

MTPNP_UINT8 MTPNP_AD_ADN_SIM2GetNumLen(void)
{
	return 0;
}

void MTPNP_AD_ADN_SIM2SetNumberSupport(MTPNP_UINT16 len)
{
	return;
}

MTPNP_UINT16 MTPNP_AD_ADN_SIM2GetUsedNumber(void)
{
	return 0;
}

MTPNP_UINT16 MTPNP_AD_ADN_SIM2GetCapacity(void)
{
	return 0;
}

MTPNP_UINT16 MTPNP_AD_FDN_GetUsedNumber(void)
{
	return 0;
}

MTPNP_UINT8 MTPNP_AD_FDN_GetReady(void)
{
	return 0;
}

MTPNP_BOOL MTPNP_AD_Add_ADN_Record(E_MTPNP_CODING_SCHEME coding_scheme, MTPNP_CHAR * name, MTPNP_CHAR * number, MTPNP_UINT8 TON_NPI)
{
	return MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_AD_Edit_ADN_Record(E_MTPNP_CODING_SCHEME coding_scheme, MTPNP_CHAR * name, MTPNP_CHAR * number, MTPNP_UINT8 TON_NPI, MTPNP_UINT16 index)
{
	return MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_AD_ADN_Record_ADD_Succeed(MTPNP_UINT16 index, MTPNP_UINT16 name_length, E_MTPNP_STORAGE_TYPE storage)
{
	return MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_AD_ADN_Record_ADD_Failed(MTPNP_UINT16 fail_cause, E_MTPNP_STORAGE_TYPE storage)
{
	return MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_AD_ADN_Record_EDIT_Succeed(MTPNP_UINT16 index, MTPNP_UINT16 name_length, E_MTPNP_STORAGE_TYPE storage)
{
	return MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_AD_ADN_Record_EDIT_Failed(MTPNP_UINT16 fail_cause, E_MTPNP_STORAGE_TYPE storage)
{
	return MTPNP_FALSE;
}

MTPNP_UINT8 MTPNP_AD_ADN_SIM2GetTagSupport(void)
{
	return 0;
}

MTPNP_UINT8 MTPNP_AD_ADN_SIM2GetNumberSupport(void)
{
	return 0;
}

MTPNP_BOOL MTPNP_AD_DELETE_ADN_Record(MTPNP_UINT16 index)
{
	return MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_AD_ADN_Record_DEL_Succeed(MTPNP_UINT16 index, E_MTPNP_STORAGE_TYPE storage)
{
	return MTPNP_FALSE;
}

MTPNP_BOOL MTPNP_AD_ADN_Record_DEL_Failed(MTPNP_UINT16 index, E_MTPNP_STORAGE_TYPE storage)
{
	return MTPNP_FALSE;
}

void MTPNP_AD_ADN_DelAllOneAfterOneReq(void *pData)
{
	return;
}

MTPNP_UINT16 MTPNP_AD_ADN_GetNextValidIndex(MTPNP_UINT16 Begin)
{
	return 0;
}

void MTPNP_AD_ADN_DelAllFinish(void)
{
	return;
}

////////////////////////////////////////
// fdn APIs


////////////////////////////////////////
// CHIS APIs


////////////////////////////////////////
// SS APIs


////////////////////////////////////////
// SAT APIs


////////////////////////////////////////
// SMS APIs
static MTPNP_BOOL g_mmi_magbox_init = MTPNP_FALSE;
static E_MTPNP_AD_FILTER_RULE g_filter_rule = MTPNP_AD_FILTER_ALL;
static E_MTPNP_AD_SHOW_STYLE g_show_style = MTPNP_AD_SHOW_MIXED;
static S32 sms_show_list_currIndex = 0;
static MTPNP_BOOL g_dual_sim_reply_sms_support = MTPNP_TRUE;

extern mmi_frm_sms_context_struct g_frm_sms_cntx;
extern msg_context_struct g_msg_cntx;
extern const U16 gIndexIconsImageList[];
extern U16 mmi_frm_sms_show_list_index[];
extern mmi_frm_sms_send_struct* sendSMS;
extern void mmi_frm_sms_init_msgbox(void);
extern void mmi_frm_sms_get_sms_index(mmi_frm_sms_msgbox_enum type, U16 index, U16* l4_index);
extern void mmi_frm_sms_handle_sms_refresh(U16 index);
extern void mmi_frm_sms_startup_read_ind(void* inMsg);
extern void mmi_frm_sms_interrupt_callback(module_type mod, U32 msgid, void *inMsg, U16 result);
extern U8 *CovertStringForPlatform(U8 *data, U16 len, U8 format, U16 *outLen);
extern S32 InputboxConvertDeviceEncodingToGSM(U8 *inBuffer,U8 *outBuffer,S32 outBufferSize,S32 UCS2_count,S32 GSM_ext_count,byte flag);


static void mmi_msg_highlight_dm_msg_settings(void);
static void mmi_msg_entry_dm_msg_settings(void);
static void mmi_msg_save_show_style(void);
static void mmi_msg_entry_show_style(void);
static void mmi_msg_highlight_show_style(void);
static void mmi_msg_hint_show_style(U16 index);
static void HighlightMultiInboxHandler(void);
static void HintMultiInboxHandler(U16 index);
static void HighlightSignalInboxHandler(void);
static void HintSignalInboxHandler(U16 index);
static void HighlightSignalOutboxHandler(void);
static void HintSignalOutboxHandler(U16 index);
static void HighlightMultiOutboxHandler(void);
static void HintMultiOutboxHandler(U16 index);
#ifdef __MMI_MESSAGES_DRAFT_BOX__    
static void HighlightMasterDraftboxHandler(void);
static void HintMasterDraftboxHandler(U16 index);
static void HighlightSlaveDraftboxHandler(void);
static void HintSlaveDraftboxHandler(U16 index);
#endif
/*****************************************************************************
 * FUNCTION
 *  mmi_frm_sms_is_hide_send_sms_menu_item
 * DESCRIPTION
 *  enter sms option screen
 * PARAMETERS
 *  void
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL mmi_frm_sms_is_hide_send_sms_menu_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
 
    return (!mmi_frm_sms_is_sms_valid());
}

BOOL mmi_frm_sms_is_sms_valid(void)
{
	if(MTPNP_AD_Get_UsableSide_Number() > 0)
		return MTPNP_TRUE;
	return MTPNP_FALSE;
}


/////////////////////////////////////////////////////////////////////////
/****************************************************************
**
**
**
**
**
*****************************************************************/
void MTPNP_PFAL_InitMessageSetup(void)
{
	U8 i;
	// TODO: all these menu should be done
	SetHiliteHandler(MENU_MTPNP_DM_MSG_SETTINGS, mmi_msg_highlight_dm_msg_settings);

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MENU_MTPNP_SMS_MASTER_INBOX + i, HighlightMultiInboxHandler);
		SetHintHandler(MENU_MTPNP_SMS_MASTER_INBOX + i, HintMultiInboxHandler);
		SetHiliteHandler(MENU_MTPNP_SMS_MASTER_OUTBOX + i, HighlightMultiOutboxHandler);
		SetHintHandler(MENU_MTPNP_SMS_MASTER_OUTBOX + i, HintMultiOutboxHandler);
		SetHiliteHandler(MENU_MTPNP_SMS_SELECT_SIM1_OPTION + i, mmi_msg_highlight_send_from_SIM_Select);
		SetHiliteHandler(MENU_MTPNP_SMS_SELECT_SAVE_SIM1_OPTION + i, mmi_msg_highlight_save_from_SIM_select);
	}
#ifdef __MMI_MESSAGES_DRAFT_BOX__    
	SetHiliteHandler(MENU_MTPNP_SMS_MASTER_DRAFTBOX, HighlightMasterDraftboxHandler);
	SetHintHandler(MENU_MTPNP_SMS_MASTER_DRAFTBOX, HintMasterDraftboxHandler);
	SetHiliteHandler(MENU_MTPNP_SMS_SLAVE_DRAFTBOX, HighlightSlaveDraftboxHandler);
	SetHintHandler(MENU_MTPNP_SMS_SLAVE_DRAFTBOX, HintSlaveDraftboxHandler);
#endif 	
	SetHiliteHandler(MENU_MTPNP_SMS_SHOW_STYLE, mmi_msg_highlight_show_style);
	SetHintHandler(MENU_MTPNP_SMS_SHOW_STYLE, mmi_msg_hint_show_style);
	
	memset((S8 *) mmi_frm_sms_show_list_index, MMI_FRM_SMS_INVALID_NUM, sizeof(U16) * g_frm_sms_cntx.mmi_frm_sms_msg_box_size);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
MTPNP_BOOL MTPNP_AD_Msgbox_Is_Init(void)
{
	return g_mmi_magbox_init;
}

void MTPNP_AD_Msgbox_HasInit(void)
{
	g_mmi_magbox_init = MTPNP_TRUE;
}

MTPNP_UINT16 MTPNP_AD_Get_StartIdx_In_Msgbox(U8 nSimID)
{
	return ( PHONE_SMS_ENTRY_COUNT/MMI_SIM_NUMBER + SIM_SMS_ENTRY_COUNT )*nSimID;
}

MTPNP_UINT16 MTPNP_AD_Get_EndIdx_In_Msgbox(U8 nSimID)
{
	return ( PHONE_SMS_ENTRY_COUNT/MMI_SIM_NUMBER + SIM_SMS_ENTRY_COUNT )*(nSimID+1) -1;
}






/****************************************************************
**
**
**
**
**
*****************************************************************/

MTPNP_UINT16 MTPNP_PFAL_Msg_Get_valid_L4index(MTPNP_UINT16 *l4_index)
{
	MTPNP_UINT16 i;

	for (i = 0; i < g_frm_sms_cntx.mmi_frm_sms_msg_seg_size; i++)
	{
		if (l4_index[i] != MMI_FRM_SMS_INVALID_INDEX)
		{
			return l4_index[i];
		}
	}

	return MMI_FRM_SMS_INVALID_INDEX;

}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void MTPNP_PFAL_Refresh_StatusBar_SMS_Display(MTPNP_BOOL blink, MTPNP_BOOL unblink, U8 nSimID)
{
	MTPNP_UINT16 i = 0;
	MTPNP_UINT16 totalinbox = mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX);
#ifdef __UNIFIED_MESSAGE_SIMBOX_SUPPORT__
	MTPNP_UINT16 totalsimbox = mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_SIM);
#endif /* __UNIFIED_MESSAGE_SIMBOX_SUPPORT__ */
	MTPNP_UINT16 *l4_index = MTPNP_OSAL_malloc(sizeof(MTPNP_UINT16) * (g_frm_sms_cntx.mmi_frm_sms_msg_seg_size + 1));

	if (!MTPNP_PFAL_Is_Card_Usable(nSimID))
	{
		MTPNP_PFAL_Hide_StatusICON(STATUS_ICON_INCOMING_SMS + nSimID);

		mmi_trace(g_sw_SMS,"MMI_SMS: MTPNP_PFAL_Refresh_StatusBar_SMS_Display, card%d sms not valid", nSimID);

		MTPNP_OSAL_memfree(l4_index);
		return;
	}

	if (blink == MTPNP_TRUE)
	{
		MTPNP_PFAL_Hide_StatusICON(STATUS_ICON_INCOMING_SMS + nSimID);
		//MTPNP_PFAL_Hide_StatusICON(STATUS_ICON_SUBLCD_SMS);
		MTPNP_PFAL_Blink_StatusICON(STATUS_ICON_INCOMING_SMS + nSimID);
		//MTPNP_PFAL_Blink_StatusICON(STATUS_ICON_SUBLCD_SMS);
		MTPNP_PFAL_Refresh_StatusBar();


		MTPNP_OSAL_memfree(l4_index);
		l4_index = MTPNP_NULL;
		mmi_trace(g_sw_SMS,"MMI_SMS: MTPNP_PFAL_Refresh_StatusBar_SMS_Display, blink");
		return;
	}

	if (unblink == MTPNP_TRUE)
	{
		MTPNP_PFAL_Hide_StatusICON(STATUS_ICON_INCOMING_SMS + nSimID);
		//MTPNP_PFAL_Hide_StatusICON(STATUS_ICON_SUBLCD_SMS);
	}


	if (MTPNP_PFAL_Is_Card_Usable(nSimID) && g_msg_cntx.msg_full_ind[nSimID] == FALSE)
	{
		if (totalinbox != MMI_FRM_SMS_INVALID_INDEX)
		{
			while (i < totalinbox)
			{
				mmi_frm_sms_get_sms_index(MMI_FRM_SMS_APP_INBOX, i, l4_index);
				*l4_index = MTPNP_PFAL_Msg_Get_valid_L4index(l4_index);

				if ((mmi_frm_sms_get_status(MMI_FRM_SMS_APP_INBOX, i) & 0x0f) == MMI_FRM_SMS_APP_UNREAD)
				{

					if (mmi_frm_msg_get_simid_by_l4_index(*l4_index) == nSimID)
					{
						MTPNP_PFAL_Show_StatusICON(STATUS_ICON_INCOMING_SMS + nSimID);
						//MTPNP_PFAL_Show_StatusICON(STATUS_ICON_SUBLCD_SMS);
						MTPNP_PFAL_Refresh_StatusBar();

						MTPNP_OSAL_memfree(l4_index);
						l4_index = MTPNP_NULL;
						mmi_trace(g_sw_SMS,"MMI_SMS: MTPNP_PFAL_Refresh_StatusBar_SMS_Display, show new SMS icon");
						return;
					}
				}
				i++;
			}
		}

		MTPNP_PFAL_Hide_StatusICON(STATUS_ICON_INCOMING_SMS + nSimID);
		//MTPNP_PFAL_Hide_StatusICON(STATUS_ICON_SUBLCD_SMS);
	}

	MTPNP_PFAL_Refresh_StatusBar();

	MTPNP_OSAL_memfree(l4_index);
	l4_index = MTPNP_NULL;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/

void MTPNP_PFAL_SMS_Unload(U8 nSimID)
{
	MTPNP_UINT16 i;
	MTPNP_UINT16 idx_start;
	MTPNP_UINT16 idx_end;

	idx_start = MTPNP_AD_Get_StartIdx_In_Msgbox(nSimID);
	idx_end = MTPNP_AD_Get_EndIdx_In_Msgbox(nSimID);

	for (i = 0; i <= idx_end; i++)
	{
		mmi_frm_sms_handle_sms_refresh(i);
	}

	MTPNP_PFAL_Refresh_StatusBar_SMS_Display(MTPNP_FALSE, MTPNP_FALSE, nSimID);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/

E_MTPNP_SLAVE_MODULE_TYPE MTPNP_AD_Get_Slave_Type(void)
{
	return MTPNP_CM_GSM_MODULE;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static U16 init_sms_show_list_index(U8 box_type, E_MTPNP_AD_FILTER_RULE rule)
{
	U16 box_size;
	U16 show_size = 0;
	U16 i = 0;

	box_size = mmi_frm_sms_get_sms_list_size(box_type);

	if (box_size == MMI_FRM_SMS_INVALID_INDEX)
	{
		return MMI_FRM_SMS_INVALID_INDEX;
	}

	memset((S8 *) mmi_frm_sms_show_list_index, MMI_FRM_SMS_INVALID_NUM, sizeof(U16) * g_frm_sms_cntx.mmi_frm_sms_msg_box_size);

	switch (rule)
	{

	case MTPNP_AD_FILTER_ALL:
		show_size = box_size;
		break;

	case MTPNP_AD_FILTER_MASTER:

		for (i = 0; i < box_size; i++)
		{
			if (0 == mmi_frm_sms_record_get_sim_id(box_type, i))
			{
				mmi_frm_sms_show_list_index[show_size++] = i;
			}
		}

		break;

	case MTPNP_AD_FILTER_SLAVE:

		for (i = 0; i < box_size; i++)
		{
			if (1 == mmi_frm_sms_record_get_sim_id(box_type, i))
			{
				mmi_frm_sms_show_list_index[show_size++] = i;
			}
		}

		break;

	case MTPNP_AD_FILTER_SIM3:

		for (i = 0; i < box_size; i++)
		{
			if (2 == mmi_frm_sms_record_get_sim_id(box_type, i))
			{
				mmi_frm_sms_show_list_index[show_size++] = i;
			}
		}

		break;
	case MTPNP_AD_FILTER_SIM4:

		for (i = 0; i < box_size; i++)
		{
			if (3 == mmi_frm_sms_record_get_sim_id(box_type, i))
			{
				mmi_frm_sms_show_list_index[show_size++] = i;
			}
		}

		break;

	default:
		break;
	}

	mmi_trace(g_sw_SMS, "MMI_SMS: init_sms_show_list_index, box_type=%d, box_size=%d, rule=%d, show_size=%d", box_type, box_size, rule, show_size);

	return show_size;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/

U16 MTPNP_PFAL_SMS_Get_SMS_List_Size(U8 type)
{
	U16 result = 0;

	result = init_sms_show_list_index(type, MTPNP_AD_SMS_Get_Filter_Rule());
	return result;
}

void MTPNP_PFAL_SMS_Set_Current_Index(S32 nIndex)
{
	if (MTPNP_AD_SMS_Get_Filter_Rule() == MTPNP_AD_FILTER_ALL)
	{
		g_msg_cntx.currBoxIndex = nIndex;
	}
	else
	{
		g_msg_cntx.currBoxIndex = mmi_frm_sms_show_list_index[nIndex];
	}

	sms_show_list_currIndex = nIndex;
}

void MTPNP_PFAL_SMS_Modify_Index(U8 type)
{
	U16 totalNumber = MTPNP_PFAL_SMS_Get_SMS_List_Size(type);
	if(sms_show_list_currIndex >= totalNumber)
		MTPNP_PFAL_SMS_Set_Current_Index(totalNumber - 1);
}
/****************************************************************
**
**
**
**
**
*****************************************************************/

S32 MTPNP_PFAL_SMS_Get_Current_Index(void)
{
	U16 i = 0;

	if (g_msg_cntx.currBoxIndex != MMI_FRM_SMS_INVALID_INDEX &&
			g_msg_cntx.currBoxIndex != mmi_frm_sms_show_list_index[sms_show_list_currIndex])
	{
		for (i = 0; i < MMI_SMS_MAX_MSG_NUM; i++)
		{
			if (g_msg_cntx.currBoxIndex == mmi_frm_sms_show_list_index[i])
			{
				sms_show_list_currIndex = i;
				break;
			}
		}
	}

	return sms_show_list_currIndex;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
MTPNP_BOOL MTPNP_PFAL_Msg_Inbox_List_Get_Item(MTPNP_INT32 item_index, UI_string_type str_buff, MTPNP_UINT8 ** img_buff_p, MTPNP_UINT8 str_img_mask)
{
	MTPNP_CHAR temp[(MAX_DIGITS_SMS + 1) * ENCODING_LENGTH];
	MTPNP_CHAR *name;
	MTPNP_UINT16 *l4_index = MTPNP_OSAL_malloc(sizeof(MTPNP_UINT16) * (g_frm_sms_cntx.mmi_frm_sms_msg_seg_size + 1));
	S32 m_item_index = item_index;
	S32 list_index;

	#ifdef __MMI_VIP_FUNCTION__
	m_item_index = g_vip_context.SMSIndexMap[item_index];
	#endif

	list_index = m_item_index;

	if (MTPNP_AD_SMS_Get_Filter_Rule() != MTPNP_AD_FILTER_ALL)
	{
		list_index = mmi_frm_sms_show_list_index[m_item_index];
	}

	if (((mmi_frm_sms_get_status(MMI_FRM_SMS_APP_INBOX, (U16) list_index) & 0xf0) >> 4) == SMSAL_MTI_STATUS_REPORT)
	{
		MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) str_buff, (S8 *) GetString(STR_INBOX_REPORT_ID));
	}
	else
	{
		MTPNP_OSAL_memset(temp, 0, (MAX_DIGITS_SMS + 1) * ENCODING_LENGTH);

		AnsiiToUnicodeString((S8*)temp, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_INBOX, (MTPNP_UINT16)list_index));

		name = lookUpNumber((PS8)temp);

		if (MTPNP_PFAL_Unicode_String_Length(name))
		{
			MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) str_buff, name);
		}
		else
		{
			MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) str_buff, temp);
		}
	}

	mmi_frm_sms_get_sms_index(MMI_FRM_SMS_APP_INBOX, (MTPNP_UINT16) list_index, l4_index);

	*l4_index = MTPNP_PFAL_Msg_Get_valid_L4index(l4_index);

	if ((mmi_frm_sms_get_status(MMI_FRM_SMS_APP_INBOX, (MTPNP_UINT16) list_index) & 0x0f) == MMI_FRM_SMS_APP_UNREAD)
	{
		if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_INBOX, (U16)list_index)&0x0f) == 0)
		{
			*img_buff_p = get_image(IMG_SMS_INBOX_UNREAD_SM_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
		}
		else
		{
			*img_buff_p = get_image(IMG_SMS_INBOX_UNREAD_ME_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
		}
	}
	else //read message
	{
		if (mmi_frm_sms_check_complete(MMI_FRM_SMS_APP_INBOX, (MTPNP_UINT16) list_index) == MTPNP_TRUE)
		{
			if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_INBOX, (U16)list_index)&0x0f) == 0)
			{
				*img_buff_p = get_image(IMG_SMS_INBOX_READ_SM_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
			}
			else
			{
				*img_buff_p = get_image(IMG_SMS_INBOX_READ_ME_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
			}
		}
		else
		{
			if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_INBOX, (U16)list_index)&0x0f) == 0)
			{
				*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_SM_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
			}
			else
			{
				*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_ME_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
			}
		}
	}

	MTPNP_OSAL_memfree(l4_index);

	l4_index = MTPNP_NULL;

	return MTPNP_TRUE;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/

MTPNP_BOOL MTPNP_PFAL_Msg_Outbox_List_Get_Item(MTPNP_INT32 item_index, UI_string_type str_buff, MTPNP_UINT8 ** img_buff_p, MTPNP_UINT8 str_img_mask)
{
	MTPNP_CHAR temp[(MAX_DIGITS_SMS + 1) * ENCODING_LENGTH];
	MTPNP_CHAR *name;
	MTPNP_UINT16 *l4_index = MTPNP_OSAL_malloc(sizeof(MTPNP_UINT16) * (g_frm_sms_cntx.mmi_frm_sms_msg_seg_size + 1));
	S32 m_item_index = item_index;
	S32 list_index;

	#ifdef __MMI_VIP_FUNCTION__
	m_item_index = g_vip_context.SMSIndexMap[item_index];
	#endif

	list_index = m_item_index;

	if (MTPNP_AD_SMS_Get_Filter_Rule() != MTPNP_AD_FILTER_ALL)
	{
		list_index = mmi_frm_sms_show_list_index[m_item_index];
	}

	if (strlen((MTPNP_CHAR *) mmi_frm_sms_get_address(MMI_FRM_SMS_APP_OUTBOX, (MTPNP_UINT16) list_index)))
	{
		MTPNP_OSAL_memset(temp, 0, (MAX_DIGITS_SMS + 1) * ENCODING_LENGTH);
		AnsiiToUnicodeString((S8*)temp, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_OUTBOX, (MTPNP_UINT16)list_index));

		name = lookUpNumber(temp);

		if (MTPNP_PFAL_Unicode_String_Length(name))
		{
			MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) str_buff, name);
		}
		else
		{
			MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) str_buff, temp);
		}
	}
	else
	{
		MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) str_buff, (MTPNP_CHAR *) GetString(STR_OUTBOX_LIST_MESSAGE));
	}

	mmi_frm_sms_get_sms_index(MMI_FRM_SMS_APP_OUTBOX, (MTPNP_UINT16) list_index, l4_index);

	if (mmi_frm_sms_check_complete(MMI_FRM_SMS_APP_OUTBOX, (MTPNP_UINT16) list_index) == MTPNP_FALSE)
	{
		if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_OUTBOX, (U16)list_index)&0x0f) == 0)
		{
			*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_SM_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
		}
		else
		{
			*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_ME_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
		}
	}
	else
	{
		if ((mmi_frm_sms_get_status(MMI_FRM_SMS_APP_OUTBOX, (MTPNP_UINT16) list_index) & 0x0f) == MMI_FRM_SMS_APP_OUTBOX)
		{
			if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_OUTBOX, (U16)list_index)&0x0f) == 0)
			{
				*img_buff_p = get_image(IMG_SMS_OUTBOX_SEND_SM_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
			}
			else
			{
				*img_buff_p = get_image(IMG_SMS_OUTBOX_SEND_ME_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
			}
		}
		else
		{
		
			if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_OUTBOX, (U16)list_index)&0x0f) == 0)
			{
				*img_buff_p = get_image(IMG_SMS_OUTBOX_UNSEND_SM_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
			}
			else
			{
				*img_buff_p = get_image(IMG_SMS_OUTBOX_UNSEND_ME_M + mmi_frm_msg_get_simid_by_l4_index(*l4_index));
			}
		}
	}

	MTPNP_OSAL_memfree(l4_index);

	l4_index = MTPNP_NULL;

	return MTPNP_TRUE;
}
#ifdef __MMI_MESSAGES_DRAFT_BOX__

/****************************************************************
**
**
**
**
**
*****************************************************************/

MTPNP_BOOL MTPNP_PFAL_Msg_Draftbox_List_Get_Item(MTPNP_INT32 item_index, UI_string_type str_buff, MTPNP_UINT8 ** img_buff_p, MTPNP_UINT8 str_img_mask)
{
	MTPNP_CHAR temp[(MAX_DIGITS_SMS + 1) * ENCODING_LENGTH];
	MTPNP_CHAR *name;
	MTPNP_UINT16 *l4_index = MTPNP_OSAL_malloc(sizeof(MTPNP_UINT16) * (g_frm_sms_cntx.mmi_frm_sms_msg_seg_size + 1));
	S32 list_index = item_index;

	if (MTPNP_AD_SMS_Get_Filter_Rule() != MTPNP_AD_FILTER_ALL)
	{
		list_index = mmi_frm_sms_show_list_index[item_index];
	}

	if (strlen((MTPNP_CHAR *) mmi_frm_sms_get_address(MMI_FRM_SMS_APP_DRAFTS, (MTPNP_UINT16) list_index)))
	{
		MTPNP_OSAL_memset(temp, 0, (MAX_DIGITS_SMS + 1) * ENCODING_LENGTH);
		AnsiiToUnicodeString((S8*)temp, (S8*)mmi_frm_sms_get_address(MMI_FRM_SMS_APP_DRAFTS, (MTPNP_UINT16)list_index));

		name = lookUpNumber(temp);

		if (MTPNP_PFAL_Unicode_String_Length(name))
		{
			MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) str_buff, name);
		}
		else
		{
			MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) str_buff, temp);
		}
	}
	else
	{
		MTPNP_PFAL_Unicode_String_Copy((MTPNP_CHAR *) str_buff, (MTPNP_CHAR *) GetString(STR_OUTBOX_LIST_MESSAGE));
	}

	mmi_frm_sms_get_sms_index(MMI_FRM_SMS_APP_DRAFTS, (MTPNP_UINT16) list_index, l4_index);

	if (mmi_frm_sms_check_complete(MMI_FRM_SMS_APP_DRAFTS, (MTPNP_UINT16) list_index) == MTPNP_FALSE)
	{
		if (*l4_index < MTPNP_AD_Get_Card2_StartIdx_In_Msgbox())
		{
			if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_DRAFTS, (U16)list_index)&0x0f) == 0)
			{
				*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_SM_M);
			}
			else
			{
				*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_ME_M);
			}
		}
		else
		{
			if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_DRAFTS, (U16)list_index)&0x0f) == 0)
			{
				*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_SM_S);
			}
			else
			{
				*img_buff_p = get_image(IMG_MESSAGE_SS_NCOMP_ME_S);
			}
		}
	}
	else
	{
		if ((mmi_frm_sms_get_status(MMI_FRM_SMS_APP_DRAFTS, (MTPNP_UINT16) list_index) & 0x0f) == MMI_FRM_SMS_APP_DRAFTS)
		{
			if (*l4_index < MTPNP_AD_Get_Card2_StartIdx_In_Msgbox())
			{
				if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_DRAFTS, (U16)list_index)&0x0f) == 0)
				{
					*img_buff_p = get_image(IMG_SMS_OUTBOX_SEND_SM_M);
				}
				else
				{
					*img_buff_p = get_image(IMG_SMS_OUTBOX_SEND_ME_M);
				}
			}
			else
			{
				if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_DRAFTS, (U16)list_index)&0x0f) == 0)
				{
					*img_buff_p = get_image(IMG_SMS_OUTBOX_SEND_SM_S);
				}
				else
				{
					*img_buff_p = get_image(IMG_SMS_OUTBOX_SEND_ME_S);
				}
			}
		}
		else
		{
		
			if (*l4_index < MTPNP_AD_Get_Card2_StartIdx_In_Msgbox())
			{
				if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_DRAFTS, (U16)list_index)&0x0f) == 0)
				{
					*img_buff_p = get_image(IMG_SMS_OUTBOX_UNSEND_SM_M);
				}
				else
				{
					*img_buff_p = get_image(IMG_SMS_OUTBOX_UNSEND_ME_M);
				}
			}
			else
			{
				if((mmi_frm_sms_get_storage(MMI_FRM_SMS_APP_DRAFTS, (U16)list_index)&0x0f) == 0)
				{
					*img_buff_p = get_image(IMG_SMS_OUTBOX_UNSEND_SM_S);
				}
				else
				{
					*img_buff_p = get_image(IMG_SMS_OUTBOX_UNSEND_ME_S);
				}
			}
		}
	}

	MTPNP_OSAL_memfree(l4_index);

	l4_index = MTPNP_NULL;

	return MTPNP_TRUE;
}
#endif
/****************************************************************
**
**
**
**
**
*****************************************************************/

S32 MTPNP_PFAL_Msg_Inbox_List_Get_Hint(S32 item_index, UI_string_type * hint_array)
{
	S32 m_item_index = item_index;
	S32 list_index;

	#ifdef __MMI_VIP_FUNCTION__
	m_item_index = g_vip_context.SMSIndexMap[item_index];
	#endif

	list_index = m_item_index;

	if (MTPNP_AD_SMS_Get_Filter_Rule() != MTPNP_AD_FILTER_ALL)
	{
		list_index = mmi_frm_sms_show_list_index[m_item_index];
	}

	return mmi_msg_inbox_list_get_hint(list_index, hint_array);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
S32 gMultiSimHighLightIndex = 0;
U8 gReqSrcModIndex[MMI_SIM_NUMBER]={0,1,};
void SetReqSrcModeIndex(U8 index, U8 mode_index)
{
	gReqSrcModIndex[index] = mode_index;
}

/*this function used for highlight item not changed, for example, when generate hint for menu*/
U8 GetReqSrcModeIndexByIndex(U8 index)
{
	mmi_trace(1, "GetReqSrcModeIndexByIndex, return %d",gReqSrcModIndex[index]);
	return gReqSrcModIndex[index];
}

/*this function used for higlight item already changed*/
U8 GetReqSrcModIndex(void)
{
	mmi_trace(1, "GetReqSrcModIndex, return %d",gReqSrcModIndex[gMultiSimHighLightIndex]);
	return gReqSrcModIndex[gMultiSimHighLightIndex];
}
U32 GetMulitSimHighLightIndex(void)
{
	return gMultiSimHighLightIndex;
}
/*********************************************************************
multi sim menu should at first position if you want use this function
*********************************************************************/
void MultiSimMenuHighLightHandler(S32 hiliteid)
{
	gMultiSimHighLightIndex = hiliteid;
	ExecuteCurrHiliteHandler(hiliteid);
}
void mmi_mtpnp_entry_menu(U16 scr_id, FuncPtr exit_func, FuncPtr entry_func, U16 menu_id, U16 str_id, U16 img_id)
{
	U8 *guiBuffer;
	U16 nStrItemList[MAX_SUB_MENUS];
	U16 nImgIltemList[MAX_SUB_MENUS];
	U16 nNumofItem;
	U8 *hintList[MAX_SUB_MENUS];

	EntryNewScreen(scr_id, exit_func, entry_func, NULL);
	guiBuffer = GetCurrGuiBuffer(scr_id);

	nNumofItem = GetNumOfChild_Ext(menu_id);
	GetSequenceStringIds_Ext(menu_id, nStrItemList);
	GetSequenceImageIds_Ext(menu_id, nImgIltemList);
	ConstructHintsList(menu_id, hintList);
	SetParentHandler(menu_id);
	RegisterHighlightHandler(MultiSimMenuHighLightHandler);

	ShowCategory52Screen(str_id, img_id,
						 STR_GLOBAL_OK, IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 nNumofItem, nStrItemList, (U16 *)gIndexIconsImageList, hintList, 0, 0, guiBuffer); /* maybe use nImgIltemList */

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/

 void mmi_msg_entry_divided_inbox_menu(void)
{
#ifdef __FLIGHT_MODE_SUPPORT__ 
	if(MTPNP_AD_Is_Flight_Mode())
	{
		DisplayPopup((PU8)GetString(STRING_MTPNP_DM_FLIGHT) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		return;
	}
#endif
	MTPNP_AD_SetMenuHideUnhide(MENU_MTPNP_SMS_MASTER_INBOX);
	mmi_mtpnp_entry_menu(SCR_MTPNP_DM_MSG_INBOX, NULL, mmi_msg_entry_divided_inbox_menu,
						 MESSAGES_MENU_INBOX_MENUID, STR_INBOX_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION);
	SetMessagesCurrScrnID(SCR_MTPNP_DM_MSG_INBOX);
}

void mmi_msg_entry_inbox_menu(void)
{
	mmi_msg_entry_divided_inbox_menu();
}

/****************************************************************
**
**
**
**
**
*****************************************************************/

static void mmi_msg_entry_divided_outbox_menu(void)
{
	MTPNP_AD_SetMenuHideUnhide(MENU_MTPNP_SMS_MASTER_OUTBOX);
	
	mmi_mtpnp_entry_menu(SCR_MTPNP_DM_MSG_OUTBOX, NULL, mmi_msg_entry_divided_outbox_menu,
						 MESSAGES_MENU_OUTBOX_MENUID, STR_OUTBOX_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION);
	SetMessagesCurrScrnID(SCR_MTPNP_DM_MSG_OUTBOX);
}


/****************************************************************
**
**
**
**
**
*****************************************************************/
#ifdef __MMI_MESSAGES_DRAFT_BOX__

static void mmi_msg_entry_divided_draftbox_menu(void)
{
	E_MTPNP_AD_SIMCARD_USABLE_SIDE usableSide = MTPNP_AD_Get_UsableSide();
	if(MTPNP_PFAL_Is_CardAbsent(0))
	{
		mmi_frm_hide_menu_item(MENU_MTPNP_SMS_MASTER_DRAFTBOX);
	}
	else
	{
		if(usableSide == MTPNP_AD_DUALSIM_USABLE || usableSide == MTPNP_AD_SIMCARD1_USABLE)
			mmi_frm_unhide_menu_item(MENU_MTPNP_SMS_MASTER_DRAFTBOX);
		else
			mmi_frm_hide_menu_item(MENU_MTPNP_SMS_MASTER_DRAFTBOX);
	}
	if(MTPNP_PFAL_Is_CardAbsent(1))
	{
		mmi_frm_hide_menu_item(MENU_MTPNP_SMS_SLAVE_DRAFTBOX);
	}
	else
	{
		if(usableSide == MTPNP_AD_DUALSIM_USABLE || usableSide == MTPNP_AD_SIMCARD2_USABLE)
			mmi_frm_unhide_menu_item(MENU_MTPNP_SMS_SLAVE_DRAFTBOX);
		else
			mmi_frm_hide_menu_item(MENU_MTPNP_SMS_SLAVE_DRAFTBOX);
	}
	mmi_mtpnp_entry_menu(SCR_MTPNP_DM_MSG_DRAFTBOX, NULL, mmi_msg_entry_divided_draftbox_menu,
						 MESSAGES_MENU_DRAFTBOX_MENUID, STR_DRAFTBOX_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION);
	SetMessagesCurrScrnID(SCR_MTPNP_DM_MSG_DRAFTBOX);
}
#endif
/****************************************************************
**
**
**
**
**
*****************************************************************/

static void HighlightDividedInboxHandler(void)
{
	mmi_msg_highlight_generic(STR_GLOBAL_OK, IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  mmi_msg_entry_divided_inbox_menu, GoBackHistory);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/

static void HighlightDividedOutboxHandler(void)
{
	mmi_msg_highlight_generic(STR_GLOBAL_OK, IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  mmi_msg_entry_divided_outbox_menu, GoBackHistory);
}

#ifdef __MMI_MESSAGES_DRAFT_BOX__

/****************************************************************
**
**
**
**
**
*****************************************************************/

static void HighlightDividedDraftboxHandler(void)
{
	mmi_msg_highlight_generic(STR_GLOBAL_OK, IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  mmi_msg_entry_divided_draftbox_menu, GoBackHistory);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HighlightSignalDraftboxHandler(void)
{
	U8 nUsableSide = MTPNP_AD_GET_UsableSide_Index();
	
	MTPNP_AD_SMS_Set_Filter_Rule(nUsableSide);
	
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction(mmi_msg_pre_entry_draftbox_list, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_msg_pre_entry_draftbox_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HintSignalDroftboxHandler(U16 index)
{
	S8 LocalData[10];
	U8 nUsableSide = MTPNP_AD_GET_UsableSide_Index();
	
	U16 size = init_sms_show_list_index(MMI_FRM_SMS_DRAFTS, nUsableSide);

	memset((PS8) hintData[index], 0, sizeof(hintData[index]));

	if (MMI_FRM_SMS_INVALID_INDEX != size)
	{
		sprintf(LocalData, "%d ", size);
		AnsiiToUnicodeString((S8*) hintData[index], (S8*)LocalData);
		UCS2Strcat((PS8) hintData[index], GetString(STR_SMS_COUNT_POPUP_SUFFIX));
	}
}

#endif
/****************************************************************
**
**
**
**
**
*****************************************************************/

static void MTPNP_AD_SMS_Set_Show_Style(E_MTPNP_AD_SHOW_STYLE style)
{
	g_show_style = style;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/

E_MTPNP_AD_SHOW_STYLE MTPNP_AD_SMS_Get_Show_Style(void)
{
	return g_show_style;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void MTPNP_PFAL_SMS_Init_Show_Style(void)
{

	U8 style;
	S16 error;

	ReadValue(NVRAM_MTPNP_SMS_SHOW_STYLE, &style, DS_BYTE, &error);

	if (style != MTPNP_AD_SHOW_DIVIDED && style != MTPNP_AD_SHOW_MIXED)
	{
		style = MTPNP_AD_SHOW_DIVIDED;
		WriteValue(NVRAM_MTPNP_SMS_SHOW_STYLE, &style, DS_BYTE, &error);
	}

	MTPNP_AD_SMS_Set_Show_Style(style);

	switch (style)
	{

	case MTPNP_AD_SHOW_MIXED:
		SetHiliteHandler(MESSAGES_MENU_INBOX_MENUID, HighlightInboxHandler);
		SetHintHandler(MESSAGES_MENU_INBOX_MENUID, HintInboxHandler);
		SetHiliteHandler(MESSAGES_MENU_OUTBOX_MENUID, HighlightOutboxHandler);
		SetHintHandler(MESSAGES_MENU_OUTBOX_MENUID, HintOutboxHandler);
        #ifdef __MMI_MESSAGES_DRAFT_BOX__
		SetHiliteHandler(MESSAGES_MENU_DRAFTBOX_MENUID, HighlightDraftboxHandler);
		SetHintHandler(MESSAGES_MENU_DRAFTBOX_MENUID, HintDraftboxHandler);     
        #endif
		break;

	case MTPNP_AD_SHOW_DIVIDED:
		if(MTPNP_AD_Get_UsableSide_Number() > 1)
		{
			SetHiliteHandler(MESSAGES_MENU_INBOX_MENUID, HighlightDividedInboxHandler);
			SetHintHandler(MESSAGES_MENU_INBOX_MENUID, NULL);
			SetHiliteHandler(MESSAGES_MENU_OUTBOX_MENUID, HighlightDividedOutboxHandler);
			SetHintHandler(MESSAGES_MENU_OUTBOX_MENUID, NULL);
            #ifdef __MMI_MESSAGES_DRAFT_BOX__
         		SetHiliteHandler(MESSAGES_MENU_DRAFTBOX_MENUID, HighlightDividedDraftboxHandler);
        		SetHintHandler(MESSAGES_MENU_DRAFTBOX_MENUID, NULL);        
            #endif
		}
		else
		{
			SetHiliteHandler(MESSAGES_MENU_INBOX_MENUID, HighlightSignalInboxHandler);
			SetHintHandler(MESSAGES_MENU_INBOX_MENUID, HintSignalInboxHandler);
			SetHiliteHandler(MESSAGES_MENU_OUTBOX_MENUID, HighlightSignalOutboxHandler);
			SetHintHandler(MESSAGES_MENU_OUTBOX_MENUID, HintSignalOutboxHandler);
            #ifdef __MMI_MESSAGES_DRAFT_BOX__
         		SetHiliteHandler(MESSAGES_MENU_DRAFTBOX_MENUID, HighlightSignalDraftboxHandler);
        		SetHintHandler(MESSAGES_MENU_DRAFTBOX_MENUID, HintSignalDroftboxHandler);   
            #endif			
		}
		break;

	default:
		break;
	}

}

/****************************************************************
**
**
**
**
**
*****************************************************************/

void MTPNP_AD_SMS_Set_Filter_Rule(E_MTPNP_AD_FILTER_RULE rule)
{
	g_filter_rule = rule;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/

E_MTPNP_AD_FILTER_RULE MTPNP_AD_SMS_Get_Filter_Rule(void)
{
	return g_filter_rule;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/

void mmi_msg_pre_entry_multi_inbox_list(void)
{
	if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX) == 0)
	{
		DisplayPopup((U8 *) GetString(STR_GLOBAL_EMPTY),
					 IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8) EMPTY_LIST_TONE);
	}
	else if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_INBOX) == MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_msg_set_processing_screen(STR_MESSAGE_MAIN_MENU_CAPTION,
									  STR_LOADING_SMS, IMG_GLOBAL_PROGRESS, STR_LOADING_INBOX_BACK);
		mmi_msg_entry_processing_generic();

	}

	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	else if (mmi_frm_sms_check_action_pending())
	{
		DisplayPopup((PU8) GetString(STR_SMS_MSG_NOT_READY_YET),
					 IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8) ERROR_TONE);
	}
	else
	{
		mmi_msg_entry_inbox_list();
	}
}

/****************************************************************
**
**
**
**
**
*****************************************************************/

void mmi_msg_pre_entry_multi_outbox_list(void)
{

	if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX) == 0)
	{
		DisplayPopup((U8 *) GetString(STR_GLOBAL_EMPTY),
					 IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8) EMPTY_LIST_TONE);
	}
	else if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_OUTBOX) == MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_msg_set_processing_screen(STR_MESSAGE_MAIN_MENU_CAPTION,
									  STR_LOADING_SMS, IMG_GLOBAL_PROGRESS, STR_LOADING_OUTBOX_BACK);
		mmi_msg_entry_processing_generic();
	}

	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	else if (mmi_frm_sms_check_action_pending())
	{
		DisplayPopup((PU8) GetString(STR_SMS_MSG_NOT_READY_YET),
					 IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8) ERROR_TONE);
	}
	else
	{
		mmi_msg_entry_outbox_list();
	}

}


#ifdef __MMI_MESSAGES_DRAFT_BOX__

/****************************************************************
**
**
**
**
**
*****************************************************************/

void mmi_msg_pre_entry_master_draftbox_list(void)
{
	MTPNP_AD_SMS_Set_Filter_Rule(MTPNP_AD_FILTER_MASTER);

	if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS) == 0)
	{
		DisplayPopup((U8 *) GetString(STR_GLOBAL_EMPTY),
					 IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8) EMPTY_LIST_TONE);
	}
	else if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS) == MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_msg_set_processing_screen(STR_MESSAGE_MAIN_MENU_CAPTION,
									  STR_LOADING_SMS, IMG_GLOBAL_PROGRESS, STR_LOADING_OUTBOX_BACK);
		mmi_msg_entry_processing_generic();
	}

	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	else if (mmi_frm_sms_check_action_pending())
	{
		DisplayPopup((PU8) GetString(STR_SMS_MSG_NOT_READY_YET),
					 IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8) ERROR_TONE);
	}
	else
	{
		mmi_msg_entry_draftbox_list();
	}

}

/****************************************************************
**
**
**
**
**
*****************************************************************/

void mmi_msg_pre_entry_slave_draftbox_list(void)
{
	MTPNP_AD_SMS_Set_Filter_Rule(MTPNP_AD_FILTER_SLAVE);

	if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS) == 0)
	{
		DisplayPopup((U8 *) GetString(STR_GLOBAL_EMPTY),
					 IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8) EMPTY_LIST_TONE);
	}
	else if (mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_APP_DRAFTS) == MMI_FRM_SMS_INVALID_INDEX)
	{
		mmi_msg_set_processing_screen(STR_MESSAGE_MAIN_MENU_CAPTION,
									  STR_LOADING_SMS, IMG_GLOBAL_PROGRESS, STR_LOADING_OUTBOX_BACK);
		mmi_msg_entry_processing_generic();
	}

	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	else if (mmi_frm_sms_check_action_pending())
	{
		DisplayPopup((PU8) GetString(STR_SMS_MSG_NOT_READY_YET),
					 IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8) ERROR_TONE);
	}
	else
	{
		mmi_msg_entry_draftbox_list();
	}
}
#endif

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void mmi_msg_highlight_dm_msg_settings(void)
{
	mmi_msg_highlight_generic(STR_GLOBAL_OK, IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  mmi_msg_entry_dm_msg_settings, GoBackHistory);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void mmi_msg_entry_dm_msg_settings(void)
{
	MTPNP_AD_SetMenuHideUnhide(MESSAGES_MENU_MSG_SETTINGS_MENUID);
	
	mmi_mtpnp_entry_menu(SCR_MTPNP_DM_MSG_SETTINGS, NULL, mmi_msg_entry_dm_msg_settings,
						 MENU_MTPNP_DM_MSG_SETTINGS, STR_MESSAGESETUP_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION);
}


/****************************************************************
**
**
**
**
**
*****************************************************************/
static void mmi_msg_save_show_style(void)
{
	U8 style = g_msg_cntx.currHiliteIndex;
	S16 error;

	if (g_msg_cntx.currHiliteIndex != MTPNP_AD_SMS_Get_Show_Style())
	{
		WriteValue(NVRAM_MTPNP_SMS_SHOW_STYLE, &style, DS_BYTE, &error);
		MTPNP_PFAL_SMS_Init_Show_Style();
	}

	DisplayPopup((PU8) GetString(STR_GLOBAL_SAVED), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8) SUCCESS_TONE);

	DeleteUptoScrID(SCR_MTPNP_DM_MSG_SETTINGS);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void mmi_msg_entry_show_style(void)
{
	U8 *guiBuffer;
	U16 nStrItemList[MAX_SUB_MENUS];
	U16 nNumofItem;
	S32 hiliteitem;

	EntryNewScreen(SCR_MTPNP_SMS_SHOW_STYLE, mmi_msg_exit_generic, mmi_msg_entry_show_style, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_MTPNP_SMS_SHOW_STYLE);
	nNumofItem = GetNumOfChild(MENU_MTPNP_SMS_SHOW_STYLE);
	GetSequenceStringIds(MENU_MTPNP_SMS_SHOW_STYLE, nStrItemList);
	SetParentHandler(MENU_MTPNP_SMS_SHOW_STYLE);

	if (guiBuffer != NULL)
	{
		hiliteitem = g_msg_cntx.currHiliteIndex;
	}
	else
	{
		hiliteitem = (S32) MTPNP_AD_SMS_Get_Show_Style();
	}

	RegisterHighlightHandler(GetHiliteIndex);

	SetMessagesCurrScrnID(SCR_MTPNP_SMS_SHOW_STYLE);
	SetMessagesScrnIdToDelHistoryNodes(SCR_MTPNP_SMS_SHOW_STYLE);

	ShowCategory11Screen(STRING_MTPNP_SMS_SHOW_STYLE, IMG_SMS_ENTRY_SCRN_CAPTION,
						 STR_GLOBAL_OK, IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 nNumofItem, nStrItemList, (U16) hiliteitem, guiBuffer);

	SetLeftSoftkeyFunction(mmi_msg_save_show_style, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void mmi_msg_highlight_show_style(void)
{
	mmi_msg_highlight_generic(STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  mmi_msg_entry_show_style, GoBackHistory);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void mmi_msg_hint_show_style(U16 index)
{
	PU8 ShowStr;

	if (MTPNP_AD_SMS_Get_Show_Style() == MTPNP_AD_SHOW_MIXED)
	{
		ShowStr = (PU8) GetString((U16) STRING_MTPNP_SMS_SHOW_MIXED);
	}
	else
	{
		ShowStr = (PU8) GetString((U16) STRING_MTPNP_SMS_SHOW_DIVIDED);
	}

	memset((PS8) hintData[index], 0, sizeof(hintData[index]));

	MTPNP_PFAL_Unicode_String_Copy((PS8) hintData[index], (PS8) ShowStr);
}

void MTPNP_PFAL_Active_Slave_SMS(MTPNP_BOOL flag)
{
	g_avtive_sms_is_slave = flag;
}

MTPNP_BOOL MTPNP_PFAL_Active_SMS_Is_Slave(void)
{
	return g_avtive_sms_is_slave;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HighlightMultiInboxHandler(void)
{
	U8 reqSrcMod = GetReqSrcModIndex();
	MTPNP_AD_SMS_Set_Filter_Rule(reqSrcMod);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction(mmi_msg_pre_entry_multi_inbox_list, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_msg_pre_entry_multi_inbox_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HighlightSignalInboxHandler(void)
{
	U8 nUsableSide = MTPNP_AD_GET_UsableSide_Index();
	
	MTPNP_AD_SMS_Set_Filter_Rule(nUsableSide);
	
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction(mmi_msg_pre_entry_multi_inbox_list, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_msg_pre_entry_multi_inbox_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HintSignalInboxHandler(U16 index)
{
	S8 LocalData[10];
	U8 nUsableSide = MTPNP_AD_GET_UsableSide_Index();
	
	U16 size = init_sms_show_list_index(MMI_FRM_SMS_INBOX, nUsableSide);

	memset((PS8) hintData[index], 0, sizeof(hintData[index]));

	if (MMI_FRM_SMS_INVALID_INDEX != size)
	{
		sprintf(LocalData, "%d ", size);
		AnsiiToUnicodeString((S8*) hintData[index], (S8*)LocalData);
		UCS2Strcat((PS8) hintData[index], GetString(STR_SMS_COUNT_POPUP_SUFFIX));
	}
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HintMultiInboxHandler(U16 index)
{
	S8 LocalData[10];
	U8 reqSrcMod = GetReqSrcModeIndexByIndex((U8)index);
	
	U16 size = init_sms_show_list_index(MMI_FRM_SMS_INBOX, reqSrcMod);

	memset((PS8) hintData[index], 0, sizeof(hintData[index]));

	if (MMI_FRM_SMS_INVALID_INDEX != size)
	{
		sprintf(LocalData, "%d ", size);
		AnsiiToUnicodeString((S8*) hintData[index], (S8*)LocalData);
		UCS2Strcat((PS8) hintData[index], GetString(STR_SMS_COUNT_POPUP_SUFFIX));
	}
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HighlightMultiOutboxHandler(void)
{
	U8 reqSrcMod = GetReqSrcModIndex();
	MTPNP_AD_SMS_Set_Filter_Rule(reqSrcMod);
	
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction(mmi_msg_pre_entry_multi_outbox_list, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_msg_pre_entry_multi_outbox_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HighlightSignalOutboxHandler(void)
{
	U8 nUsableSide = MTPNP_AD_GET_UsableSide_Index();
	
	MTPNP_AD_SMS_Set_Filter_Rule(nUsableSide);
	
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction(mmi_msg_pre_entry_multi_outbox_list, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_msg_pre_entry_multi_outbox_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HintMultiOutboxHandler(U16 index)
{
	S8 LocalData[10];
	
	U8 reqSrcMod = GetReqSrcModeIndexByIndex((U8)index);
	
	U16 size = init_sms_show_list_index(MMI_FRM_SMS_OUTBOX, reqSrcMod);

	memset((PS8) hintData[index], 0, sizeof(hintData[index]));

	if (MMI_FRM_SMS_INVALID_INDEX != size)
	{
		sprintf(LocalData, "%d ", size);
		AnsiiToUnicodeString((PS8) hintData[index], LocalData);
		UCS2Strcat((PS8) hintData[index], GetString(STR_SMS_COUNT_POPUP_SUFFIX));
	}
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HintSignalOutboxHandler(U16 index)
{
	S8 LocalData[10];
	
	U8 nUsableSide = MTPNP_AD_GET_UsableSide_Index();
	
	U16 size = init_sms_show_list_index(MMI_FRM_SMS_OUTBOX, nUsableSide);

	memset((PS8) hintData[index], 0, sizeof(hintData[index]));

	if (MMI_FRM_SMS_INVALID_INDEX != size)
	{
		sprintf(LocalData, "%d ", size);
		AnsiiToUnicodeString((PS8) hintData[index], LocalData);
		UCS2Strcat((PS8) hintData[index], GetString(STR_SMS_COUNT_POPUP_SUFFIX));
	}
}
#ifdef __MMI_MESSAGES_DRAFT_BOX__

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HighlightMasterDraftboxHandler(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction(mmi_msg_pre_entry_master_draftbox_list, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_msg_pre_entry_master_draftbox_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HintMasterDraftboxHandler(U16 index)
{
	S8 LocalData[10];
	U16 size = init_sms_show_list_index(MMI_FRM_SMS_DRAFTS, MTPNP_AD_FILTER_MASTER);

	memset((PS8) hintData[index], 0, sizeof(hintData[index]));

	if (MMI_FRM_SMS_INVALID_INDEX != size)
	{
		sprintf(LocalData, "%d ", size);
		AnsiiToUnicodeString((PS8) hintData[index], LocalData);
		UCS2Strcat((PS8) hintData[index], GetString(STR_SMS_COUNT_POPUP_SUFFIX));
	}
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HighlightSlaveDraftboxHandler(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction(mmi_msg_pre_entry_slave_draftbox_list, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_msg_pre_entry_slave_draftbox_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
static void HintSlaveDraftboxHandler(U16 index)
{
	S8 LocalData[10];
	U16 size = init_sms_show_list_index(MMI_FRM_SMS_DRAFTS, MTPNP_AD_FILTER_SLAVE);

	memset((PS8) hintData[index], 0, sizeof(hintData[index]));

	if (MMI_FRM_SMS_INVALID_INDEX != size)
	{
		sprintf(LocalData, "%d ", size);
		AnsiiToUnicodeString((PS8) hintData[index], LocalData);
		UCS2Strcat((PS8) hintData[index], GetString(STR_SMS_COUNT_POPUP_SUFFIX));
	}
}
#endif
/****************************************************************
**
**
**
**
**
*****************************************************************/
MTPNP_BOOL MTPNP_AD_Dual_Card_Reply_SMS(void)
{
	/*
	 * If you don't want to support enable dual card reply SMS,
	 * please change return value to MTPNP_FALSE.
	 */
	return g_dual_sim_reply_sms_support;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_msg_dial_use_number
 * DESCRIPTION
 *  Dial use number
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_msg_dial_use_number_ext(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if (mmi_ucs2strlen(g_msg_cntx.numbersList[g_msg_cntx.currHiliteIndex]))
	{
		mmi_ucm_app_entry_dial_option((U8 *)g_msg_cntx.numbersList[g_msg_cntx.currHiliteIndex], NULL, NULL);
	}
	else
	{
		DisplayPopup(
			(PU8) GetString(STR_GLOBAL_UNFINISHED),
			IMG_GLOBAL_UNFINISHED,
			1,
			MESSAGES_POPUP_TIME_OUT,
			(U8) ERROR_TONE);
	}
}

////////////////////////////////////////
//voice mail
#ifndef __MMI_DUAL_SIM_SINGLE_OPENED_
static void HighlightDMVoiceMailHandler(void);
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
static void mmi_msg_entry_dm_voice_mail_server(void);

extern MMI_BOOL mmi_frm_test_menu_item_hide(U16 menu_item_id);

void MTPNP_PFAL_InitVoiceMail(void)
{
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	SetHiliteHandler(MENU_MTPNP_DM_VMAIL, HighlightVoiceMailHandler);
#else
	SetHiliteHandler(MENU_MTPNP_DM_VMAIL, HighlightDMVoiceMailHandler);
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
}

#ifndef __MMI_DUAL_SIM_SINGLE_OPENED_
static void HighlightDMVoiceMailHandler(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(mmi_msg_entry_dm_voice_mail_server, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_entry_dm_voice_mail_server, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */

static void mmi_msg_entry_dm_voice_mail_server(void)
{
	g_msg_cntx.is_voicemail_from_idle = FALSE;
	MTPNP_AD_SetMenuHideUnhide(MESSAGES_MENU_VMAIL_MENUID);
	mmi_mtpnp_entry_menu(SCR_MTPNP_DM_MSG_VOICE_MAIL, NULL, mmi_msg_entry_dm_voice_mail_server,
							MENU_MTPNP_DM_VMAIL, STR_VMAIL_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION);
}

////////////////////////////////////////
// Setting-Profile APIs

#ifdef JATAAYU_SUPPORT
//WAP
#ifdef JATAAYU_SUPPORT_WAP
extern JWAP_APP_INFO *g_pstJWapAppInfo ;
/****************************************************************
**
**
**
**
**
*****************************************************************/
extern void jWap_dataConn_entry_handler ();
void MTPNP_mmi_wap_highlight_edit_ph_sim_selct(void)
{
	jWap_dataConn_entry_handler();
}
void MTPNP_jwap_edit_ph_select_sim_Hlt_handler(int iSelectIndex) 
{	
	U8 nSimID = GetReqSrcModeIndexByIndex((U8)iSelectIndex);
	g_pstJWapAppInfo->ph_edit_simid = nSimID;
	SetLeftSoftkeyFunction (MTPNP_mmi_wap_highlight_edit_ph_sim_selct ,KEY_EVENT_UP) ;
}

void MTPNP_jwap_edit_ph_select_sim_entry_handler(void)
{
	U8* guiBuffer;
	U16 nStrItemList[MMI_SIM_NUMBER];
	U16 nImgIltemList[MMI_SIM_NUMBER];
	U16 nNumofItem;

	EntryNewScreen(SCR_MTPNP_SELECT_SIM_MENU, NULL, MTPNP_jwap_edit_ph_select_sim_entry_handler, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_MTPNP_SELECT_SIM_MENU);
	MTPNP_AD_SetMenuHideUnhide(MENU_ID_MTPNP_SELECT_SIM_SIM1);

	nNumofItem = GetNumOfChild_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU);
	GetSequenceStringIds_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU, nStrItemList);
	GetSequenceImageIds_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU, nImgIltemList);
	RegisterHighlightHandler(MTPNP_jwap_edit_ph_select_sim_Hlt_handler);

	ShowCategory52Screen(STR_GLOBAL_OPTIONS, IMG_ID_JATAAYU_WAP_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, nImgIltemList,
						  0, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void MTPNP_jwap_always_ask_select_sim_Hlt_handler(int iSelectIndex) 
{	
	U8 nSimID = GetReqSrcModeIndexByIndex((U8)iSelectIndex);
	g_pstJWapAppInfo->simid = nSimID;
	SetLeftSoftkeyFunction (jWap_mainMenu_entry_handler ,KEY_EVENT_UP) ;
}

void MTPNP_jwap_always_ask_select_sim_entry_handler(void)
{
	U8* guiBuffer;
	U16 nStrItemList[MMI_SIM_NUMBER];
	U16 nImgIltemList[MMI_SIM_NUMBER];
	U16 nNumofItem;

	EntryNewScreen(SCR_MTPNP_SELECT_SIM_MENU, NULL, MTPNP_jwap_always_ask_select_sim_entry_handler, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_MTPNP_SELECT_SIM_MENU);

	MTPNP_AD_SetMenuHideUnhide(MENU_ID_MTPNP_SELECT_SIM_SIM1);
	
	nNumofItem = GetNumOfChild_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU);
	GetSequenceStringIds_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU, nStrItemList);
	GetSequenceImageIds_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU, nImgIltemList);
	SetParentHandler(MENU_ID_MTPNP_SELECT_SIM_MENU);
	RegisterHighlightHandler(MTPNP_jwap_always_ask_select_sim_Hlt_handler);

	ShowCategory52Screen(STR_GLOBAL_OPTIONS, IMG_ID_JATAAYU_WAP_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, nImgIltemList,
						  0, 0, 0, guiBuffer);

      SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void MTPNP_jWap_mainMenu_entry_handler (void)
{ 
  	S16 error = 0;

	switch(g_pstJWapAppInfo->simType)
	{
		case WAP_SIMTYPE_ALWAYSASK:
			if(MTPNP_AD_Get_UsableSide_Number() == 1)
			{
				g_pstJWapAppInfo->simid = MTPNP_AD_GET_UsableSide_Index();
				g_pstJWapAppInfo->simType = E_MTPNP_SIM_SELECT_SIM1 + MTPNP_AD_GET_UsableSide_Index();
				WriteValue(NVRAM_MTPNP_WAP_PROFILE_SELECT_SIMTYPE, &g_pstJWapAppInfo->simType, DS_BYTE, &error);

				jWap_mainMenu_entry_handler();
			}
			else
			{
				MTPNP_jwap_always_ask_select_sim_entry_handler();
			}
			break;
		case WAP_SIMTYPE_SIM1:
		case WAP_SIMTYPE_SIM2:
		case WAP_SIMTYPE_SIM3:
		case WAP_SIMTYPE_SIM4:
			g_pstJWapAppInfo->simid = g_pstJWapAppInfo->simType - WAP_SIMTYPE_SIM1;

			if(!MTPNP_PFAL_Is_Card_Usable(g_pstJWapAppInfo->simid))
			{
				g_pstJWapAppInfo->simid = MTPNP_AD_GET_UsableSide_Index();	
				g_pstJWapAppInfo->simType = E_MTPNP_SIM_SELECT_SIM1 + MTPNP_AD_GET_UsableSide_Index();
				WriteValue(NVRAM_MTPNP_WAP_PROFILE_SELECT_SIMTYPE, &g_pstJWapAppInfo->simType, DS_BYTE, &error);	
			}
			
			jWap_mainMenu_entry_handler();
			break;

		default:
			if(MTPNP_AD_Get_UsableSide_Number() == 1)
			{
				g_pstJWapAppInfo->simid = MTPNP_AD_GET_UsableSide_Index();
				g_pstJWapAppInfo->simType = E_MTPNP_SIM_SELECT_SIM1 + MTPNP_AD_GET_UsableSide_Index();
				WriteValue(NVRAM_MTPNP_WAP_PROFILE_SELECT_SIMTYPE, &g_pstJWapAppInfo->simType, DS_BYTE, &error);

				jWap_mainMenu_entry_handler();
			}
			else
			{
				g_pstJWapAppInfo->simType = E_MTPNP_SIM_SELECT_ALWAYS_ASK;
				WriteValue(NVRAM_MTPNP_WAP_PROFILE_SELECT_SIMTYPE, &g_pstJWapAppInfo->simType, DS_BYTE, &error);				
				MTPNP_jwap_always_ask_select_sim_entry_handler();
			}			
			break;
	}
	mmi_trace(g_sw_NW, "WAP Func: %s simtype = %d, simid = %d, error = %x", __FUNCTION__, g_pstJWapAppInfo->simType, g_pstJWapAppInfo->simid, error);
			
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void MTPNP_jWap_select_sim_entry_handler (void)
{
	U8* guiBuffer; 
	U16 item_texts[] = {
		STR_ID_WAP_SIMTYPE_ALWAYS_ASK,
		MULTI_STRING(STRING_MTPNP_SIM1),
		} ;

	mmi_trace (1, "@@@ MTPNP_jWap_selectsim_entry_handler @@@") ;

	EntryNewScreen (SCR_ID_MTPNP_WAP_SELECT_SIM, NULL, MTPNP_jWap_select_sim_entry_handler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MTPNP_WAP_SELECT_SIM) ;	
	RegisterHighlightHandler (MTPNP_jWap_select_sim_entry_highlight_handler) ;
	ShowCategory15Screen (STR_GLOBAL_OPTIONS, IMG_ID_JATAAYU_WAP_ICON, 
		STR_GLOBAL_OK, 0, 
		STR_GLOBAL_BACK, 0,
		E_MTPNP_SIM_SELECT_MENU_COUNT, item_texts, (PU16)gIndexIconsImageList, 
		LIST_MENU, g_pstJWapAppInfo->simType, guiBuffer);

	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	//SetKeyHandler(jwap_DualsimSelectionEndKayHandler, KEY_END, KEY_EVENT_DOWN) ;
}
/*
void jwap_DualsimSelectionEndKayHandler()
{
	jWap_deInitProfiles();
	jWap_keyEndHandler();
}

void jwap_ProfileEditEndKayHandler()
{
	jWap_keyEndHandler();
}
*/

/****************************************************************
**
**
**
**
**
*****************************************************************/
void MTPNP_mmi_wap_select_always_ask (void)
{
  	S16 error = 0;
	
	if(MTPNP_AD_Get_UsableSide_Number() > 1)
	{
		g_pstJWapAppInfo->simType = E_MTPNP_SIM_SELECT_ALWAYS_ASK;
		WriteValue(NVRAM_MTPNP_WAP_PROFILE_SELECT_SIMTYPE, &g_pstJWapAppInfo->simType, DS_BYTE, &error);
		DisplayPopup((PU8)GetString(STR_ID_WAP_SIMTYPE_ALWAYS_ASK), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);		
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);		
	}
	DeleteUptoScrID(SCR_ID_WAP_SETTINGS);
}
U8 nSimSelect = 0;
void MTPNP_mmi_wap_select_sim(void)
{
  	S16 error = 0;
	
	if(MTPNP_PFAL_Is_Card_Usable(nSimSelect))
	{
		g_pstJWapAppInfo->simid = nSimSelect;
		g_pstJWapAppInfo->simType = E_MTPNP_SIM_SELECT_SIM1 + nSimSelect;
		WriteValue(NVRAM_MTPNP_WAP_PROFILE_SELECT_SIMTYPE, &g_pstJWapAppInfo->simType, DS_BYTE, &error);
		DisplayPopup((PU8)GetString(STRING_MTPNP_SIM1 + nSimSelect), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);		
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);		
	}
	DeleteUptoScrID(SCR_ID_WAP_SETTINGS);
}

void MTPNP_jWap_select_sim_entry_highlight_handler(int iSelectIndex)
{	
	switch (iSelectIndex)
	{
		case E_MTPNP_SIM_SELECT_ALWAYS_ASK:
			SetLeftSoftkeyFunction (MTPNP_mmi_wap_select_always_ask ,KEY_EVENT_UP) ;
			break;
		case E_MTPNP_SIM_SELECT_SIM1:
		case E_MTPNP_SIM_SELECT_SIM2:
		case E_MTPNP_SIM_SELECT_SIM3:
		case E_MTPNP_SIM_SELECT_SIM4:
			nSimSelect = iSelectIndex - E_MTPNP_SIM_SELECT_SIM1;
			SetLeftSoftkeyFunction (MTPNP_mmi_wap_select_sim ,KEY_EVENT_UP) ;
			break ;
		default:
			break ;		
	}
}
#endif

//MMS
#ifdef JATAAYU_SUPPORT_MMS
extern JMMS_APP_INFO *g_pstJMmsAppInfo ;
/****************************************************************
**
**
**
**
**
*****************************************************************/
void MTPNP_mmi_mms_highlight_setting_edit_sim_select (void)
{
	jMms_server_profile_entry_handler();
}
void MTPNP_mms_setting_select_sim_Hlt_handler(int iSelectIndex) 
{	
	U8 nSimID = GetReqSrcModeIndexByIndex((U8)iSelectIndex);
	g_pstJMmsAppInfo->ph_edit_simid= nSimID;
	SetLeftSoftkeyFunction (MTPNP_mmi_mms_highlight_setting_edit_sim_select ,KEY_EVENT_UP) ;

}

void MTPNP_mms_setting_select_sim_entry_handler(void)
{
	U8* guiBuffer;
	U16 nStrItemList[MMI_SIM_NUMBER];
	U16 nImgIltemList[MMI_SIM_NUMBER];
	U16 nNumofItem;

	EntryNewScreen(SCR_MTPNP_SELECT_SIM_MENU, NULL, MTPNP_mms_setting_select_sim_entry_handler, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_MTPNP_SELECT_SIM_MENU);
	MTPNP_AD_SetMenuHideUnhide(MENU_ID_MTPNP_SELECT_SIM_SIM1);
	
	nNumofItem = GetNumOfChild_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU);
	GetSequenceStringIds_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU, nStrItemList);
	GetSequenceImageIds_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU, nImgIltemList);

	SetParentHandler(MENU_ID_MTPNP_SELECT_SIM_MENU);
	RegisterHighlightHandler(MTPNP_mms_setting_select_sim_Hlt_handler);

	ShowCategory52Screen(STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, nImgIltemList,
						  0, 0, 0, guiBuffer);

    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP); // wangbei add 20060619
    //SetKeyHandler(jmms_SettingsEndKeyHandler, KEY_END, KEY_EVENT_DOWN) ;
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
void MTPNP_mms_send_msg_select_sim_Hlt_handler(int iSelectIndex) 
{	
	U8 nSimID = GetReqSrcModeIndexByIndex((U8)iSelectIndex);
	g_pstJMmsAppInfo->send_simid= nSimID;
	SetLeftSoftkeyFunction(jMms_SendMsgEntryHandler, KEY_EVENT_UP);
}

void MTPNP_mms_send_msg_select_sim_entry_handler(void)
{
	U8* guiBuffer;
	U16 nStrItemList[MMI_SIM_NUMBER];
	U16 nImgIltemList[MMI_SIM_NUMBER];
	U16 nNumofItem;
	U8 usableSide = 0;

	if(MTPNP_AD_Get_UsableSide_Number() == 1)
	{
		usableSide = MTPNP_AD_GET_UsableSide_Index();
		g_pstJMmsAppInfo->send_simid = usableSide;
		jMms_SendMsgEntryHandler();
	}
	else
	{
		EntryNewScreen(SCR_ID_MMS_SELECT_SIM_MENU, NULL, MTPNP_mms_send_msg_select_sim_entry_handler, NULL);
		guiBuffer = GetCurrGuiBuffer(SCR_ID_MMS_SELECT_SIM_MENU);

		MTPNP_AD_SetMenuHideUnhide(MENU_ID_MTPNP_SELECT_SIM_SIM1);
		
		nNumofItem = GetNumOfChild_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU);
		GetSequenceStringIds_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU, nStrItemList);
		GetSequenceImageIds_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU, nImgIltemList);
		SetParentHandler(MENU_ID_MTPNP_SELECT_SIM_MENU);
		RegisterHighlightHandler(MTPNP_mms_send_msg_select_sim_Hlt_handler);

		ShowCategory52Screen(STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION,
							  STR_GLOBAL_OK, IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  nNumofItem, nStrItemList, nImgIltemList,
							  0, 0, 0, guiBuffer);

	      SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP); // wangbei add 20060619	
	}
}

void MTPNP_mms_resend_select_sim_Hlt_handler(int iSelectIndex) 
{	
	U8 nSimID = GetReqSrcModeIndexByIndex((U8)iSelectIndex);
	g_pstJMmsAppInfo->send_simid = nSimID;
	SetLeftSoftkeyFunction (jMms_ReSend_handler ,KEY_EVENT_UP) ;
}

void MTPNP_mms_resend_select_sim_entry_handler(void)
{
	U8* guiBuffer;
	U16 nStrItemList[MMI_SIM_NUMBER];
	U16 nImgIltemList[MMI_SIM_NUMBER];
	U16 nNumofItem;

	if(MTPNP_AD_Get_UsableSide_Number() == 1)
	{
		g_pstJMmsAppInfo->send_simid = MTPNP_AD_GET_UsableSide_Index();
		jMms_ReSend_handler();
	}
	else
	{
		EntryNewScreen(SCR_MTPNP_SELECT_SIM_MENU, NULL, MTPNP_mms_resend_select_sim_entry_handler, NULL);
		guiBuffer = GetCurrGuiBuffer(SCR_MTPNP_SELECT_SIM_MENU);

		MTPNP_AD_SetMenuHideUnhide(MENU_ID_MTPNP_SELECT_SIM_SIM1);
		
		nNumofItem = GetNumOfChild_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU);
		GetSequenceStringIds_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU, nStrItemList);
		GetSequenceImageIds_Ext(MENU_ID_MTPNP_SELECT_SIM_MENU, nImgIltemList);
		SetParentHandler(MENU_ID_MTPNP_SELECT_SIM_MENU);
		RegisterHighlightHandler(MTPNP_mms_resend_select_sim_Hlt_handler);

		ShowCategory52Screen(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
							  STR_GLOBAL_OK, IMG_GLOBAL_OK,
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  nNumofItem, nStrItemList, nImgIltemList,
							  0, 0, 0, guiBuffer);

		  SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP); // wangbei add 20060619	
	}
}

UINT8 gJddWapMMSSimId = 0;
/****************************************************************
**
**
**
**
**
*****************************************************************/
void wap_mms_set_simid(UINT32 uiConnID )
{
	if(uiConnID & WAP_MMS_UICONNID_SIM1_FLAG)
	{
		gJddWapMMSSimId = MMI_SIM_1;
	}
	else if(uiConnID & WAP_MMS_UICONNID_SIM2_FLAG)
	{
		gJddWapMMSSimId = MMI_SIM_2;
	}
}

/****************************************************************
**
**
**
**
**
*****************************************************************/
UINT8 wap_mms_get_simid()
{
	return gJddWapMMSSimId;
}
#endif  //MMS

#endif
/////////////////////////////////////////////////////////////////////////////////////
//gsm pin
/*****************************************************************************
 * FUNCTION
 *  mmi_entry_gsm_change_pin1_select_sim
 * DESCRIPTION
 *  Shows the call type selection screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void SendReqPIN1Change(void);
void mmi_entry_gsm_change_pin1_select_sim_Hlt_handler(S32 iSelectIndex) 
{	
	U8 reqSrcMod = GetReqSrcModeIndexByIndex((U8) iSelectIndex);

	SECSETSetSIMIdforPIN(reqSrcMod);
	
	SetLeftSoftkeyFunction (SendReqPIN1Change ,KEY_EVENT_UP) ;
}

void mmi_entry_gsm_change_pin1_select_sim(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 *guiBuffer = NULL;
	U16 numItems = 0;
	U16 nStrItemList[MAX_SUB_MENUS];

	if(MTPNP_AD_Get_UsableSide_Number() > 1)
	{
		/*----------------------------------------------------------------*/
		/* Code Body                                                      */
		/*----------------------------------------------------------------*/
		EntryNewScreen(SCR_GSM_CHANGE_PIN1_SELECT_SIM, NULL, mmi_entry_gsm_change_pin1_select_sim, NULL);

		guiBuffer = GetCurrGuiBuffer(SCR_GSM_CHANGE_PIN1_SELECT_SIM);

		MTPNP_AD_SetMenuHideUnhide(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM1);
		numItems = GetNumOfChild_Ext(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM);
		GetSequenceStringIds_Ext(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM, nStrItemList);    
		SetParentHandler(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM);

		RegisterHighlightHandler(mmi_entry_gsm_change_pin1_select_sim_Hlt_handler);

		ShowCategory52Screen(
		    STRING_MTPNP_SELECTION_MODE,
		    0,
		    STR_GLOBAL_OK,
		    IMG_GLOBAL_OK,
		    STR_GLOBAL_BACK,
		    IMG_GLOBAL_BACK,
		    numItems,
		    nStrItemList,
		    (U16*) gIndexIconsImageList,
		    NULL,
		    0,
		    0,
		    guiBuffer);

		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	}
	else if(MTPNP_AD_GET_UsableSide_Index() >=0)
	{
		SECSETSetSIMIdforPIN(MTPNP_AD_GET_UsableSide_Index());
		SendReqPIN1Change();
	}
}

extern void SendReqPIN2Change(void);
 void mmi_entry_gsm_change_pin2_select_sim_Hlt_handler(int iSelectIndex) 
{	
	U8 reqSrcMod = GetReqSrcModeIndexByIndex((U8) iSelectIndex);

	SECSETSetSIMIdforPIN(reqSrcMod);
	
	SetLeftSoftkeyFunction (SendReqPIN2Change ,KEY_EVENT_UP) ;
}

void mmi_entry_gsm_change_pin2_select_sim(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 *guiBuffer = NULL;
	U16 numItems = 0;
	U16 nStrItemList[MAX_SUB_MENUS];

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if(MTPNP_AD_Get_UsableSide_Number() > 1)
	{	
		EntryNewScreen(SCR_GSM_CHANGE_PIN2_SELECT_SIM, NULL, mmi_entry_gsm_change_pin2_select_sim, NULL);

		guiBuffer = GetCurrGuiBuffer(SCR_GSM_CHANGE_PIN2_SELECT_SIM);

		MTPNP_AD_SetMenuHideUnhide(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM1);
		
		numItems = GetNumOfChild_Ext(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM);
		GetSequenceStringIds_Ext(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM, nStrItemList);    
		SetParentHandler(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM);

		RegisterHighlightHandler(mmi_entry_gsm_change_pin2_select_sim_Hlt_handler);

		ShowCategory52Screen(
		    STRING_MTPNP_SELECTION_MODE,
		    0,
		    STR_GLOBAL_OK,
		    IMG_GLOBAL_OK,
		    STR_GLOBAL_BACK,
		    IMG_GLOBAL_BACK,
		    numItems,
		    nStrItemList,
		    (U16*) gIndexIconsImageList,
		    NULL,
		    0,
		    0,
		    guiBuffer);

		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	}
	else if(MTPNP_AD_GET_UsableSide_Index() >=0)
	{
		SECSETSetSIMIdforPIN(MTPNP_AD_GET_UsableSide_Index());
		SendReqPIN2Change();
	}	
}


extern void SSCEntryPUK1(void);
void mmi_entry_puk_change_pin1_select_sim_Hlt_handler(int iSelectIndex) 
{	
	U8 reqSrcMod = GetReqSrcModeIndexByIndex((U8) iSelectIndex);

	SECSETSetSIMIdforPIN(reqSrcMod);
	
	SetLeftSoftkeyFunction (SSCEntryPUK1 ,KEY_EVENT_UP) ;

}
 
/*****************************************************************************
 * FUNCTION
 *  mmi_ssc_entry_puk1_select_sim
 * DESCRIPTION
 *  Dual SIM: SSC handler for **05*
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ssc_entry_puk1_select_sim(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 *guiBuffer = NULL;
	U16 numItems = 0;
	U16 nStrItemList[MAX_SUB_MENUS];

	if(MTPNP_AD_Get_UsableSide_Number() > 1)
	{
		/*----------------------------------------------------------------*/
		/* Code Body                                                      */
		/*----------------------------------------------------------------*/
		EntryNewScreen(SCR_GSM_CHANGE_PIN1_SELECT_SIM, NULL, mmi_ssc_entry_puk1_select_sim, NULL);

		guiBuffer = GetCurrGuiBuffer(SCR_GSM_CHANGE_PIN1_SELECT_SIM);
		
		MTPNP_AD_SetMenuHideUnhide(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM1);
		
		numItems = GetNumOfChild_Ext(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM);
		GetSequenceStringIds_Ext(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM, nStrItemList);    
		SetParentHandler(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM);

		RegisterHighlightHandler(mmi_entry_puk_change_pin1_select_sim_Hlt_handler);

		ShowCategory52Screen(
		    STRING_MTPNP_SELECTION_MODE,
		    0,
		    STR_GLOBAL_OK,
		    IMG_GLOBAL_OK,
		    STR_GLOBAL_BACK,
		    IMG_GLOBAL_BACK,
		    numItems,
		    nStrItemList,
		    (U16*) gIndexIconsImageList,
		    NULL,
		    0,
		    0,
		    guiBuffer);

		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	}
	else if(MTPNP_AD_GET_UsableSide_Index() >=0)
	{
		SECSETSetSIMIdforPIN(MTPNP_AD_GET_UsableSide_Index());
		SSCEntryPUK1();
	}	
}

extern void SSCEntryPUK2(void);
void mmi_entry_puk_change_pin2_select_sim_Hlt_handler(int iSelectIndex) 
{	
	U8 reqSrcMod = GetReqSrcModeIndexByIndex((U8) iSelectIndex);

	SECSETSetSIMIdforPIN(reqSrcMod);
	
	SetLeftSoftkeyFunction (SSCEntryPUK2 ,KEY_EVENT_UP) ;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ssc_entry_puk2_select_sim
 * DESCRIPTION
 *  Dual SIM: SSC handler for **052*
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ssc_entry_puk2_select_sim(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 *guiBuffer = NULL;
	U16 numItems = 0;
	U16 nStrItemList[MAX_SUB_MENUS];

	if(MTPNP_AD_Get_UsableSide_Number() > 1)
	{
		/*----------------------------------------------------------------*/
		/* Code Body                                                      */
		/*----------------------------------------------------------------*/
		EntryNewScreen(SCR_GSM_CHANGE_PIN1_SELECT_SIM, NULL, mmi_ssc_entry_puk2_select_sim, NULL);

		guiBuffer = GetCurrGuiBuffer(SCR_GSM_CHANGE_PIN1_SELECT_SIM);
		
		MTPNP_AD_SetMenuHideUnhide(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM1);
		
		numItems = GetNumOfChild_Ext(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM);
		GetSequenceStringIds_Ext(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM, nStrItemList);    
		SetParentHandler(MENU_ID_GSM_CHANGE_PIN_SELECT_SIM);

		RegisterHighlightHandler(mmi_entry_puk_change_pin2_select_sim_Hlt_handler);

		ShowCategory52Screen(
		    STRING_MTPNP_SELECTION_MODE,
		    0,
		    STR_GLOBAL_OK,
		    IMG_GLOBAL_OK,
		    STR_GLOBAL_BACK,
		    IMG_GLOBAL_BACK,
		    numItems,
		    nStrItemList,
		    (U16*) gIndexIconsImageList,
		    NULL,
		    0,
		    0,
		    guiBuffer);

		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	}
	else if(MTPNP_AD_GET_UsableSide_Index() >=0)
	{
		SECSETSetSIMIdforPIN(MTPNP_AD_GET_UsableSide_Index());
		SSCEntryPUK2();
	}
}

void MTPNP_CPHSDisplayCFU()
{
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		HideStatusIcon(STATUS_ICON_CALL_DIVERT_L1L2+i);
		if(MTPNP_PFAL_Is_Card_Usable(i)&&b_CFU_Eable[i])
		{
			ShowStatusIcon(STATUS_ICON_CALL_DIVERT_L1L2 + i);
		}
	}	
}





//wuys add for QQ,2010-08-31
#ifdef __MMI_SUPPORT_JME__
 #ifdef MMI_ON_HARDWARE_P
extern UINT8 jmetoc_SIM;
 #else
 	UINT8 jmetoc_SIM;
 #endif
UINT8 simUsableindextoId[5] = {0x00,};
/*
void VM_always_ask_sim1 (void)
{
	jmetoc_SIM = MMI_SIM_1;
	mmi_trace(1,"MMI_SMS: VM_always_ask_sim1");

	GoBackHistory();
}
void VM_always_ask_sim2 (void)
{
	jmetoc_SIM = MMI_SIM_2;
	mmi_trace(1,"MMI_SMS: VM_always_ask_sim2");

	GoBackHistory();
}*/
INT32 index_sim = 0;
void VM_select_sim_Hlt_handler(int iSelectIndex) 
{	
		mmi_trace(1,"MMI_SMS: VM_select_sim_Hlt_handler, index: %d",iSelectIndex);

index_sim = iSelectIndex;
return;
/*
	switch (iSelectIndex)
	{
	
		case 0:
			SetLeftSoftkeyFunction (VM_always_ask_sim1 ,KEY_EVENT_UP) ;
//			SetKeyHandler(VM_always_ask_sim1, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

			break ;
		case 1:
			SetLeftSoftkeyFunction (VM_always_ask_sim2 ,KEY_EVENT_UP) ;
		//	SetKeyHandler(VM_always_ask_sim2, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

			break ; 
			
		default:
			break ; 
	}
	*/
}
extern void VM_writeSimNum(U8 simNum );
void VM_select_sim_handler() 
{
//jmetoc_SIM = index_sim;
jmetoc_SIM = simUsableindextoId[index_sim];
 #ifdef MMI_ON_HARDWARE_P
VM_writeSimNum(jmetoc_SIM);
 #endif
mmi_trace(1,"VM_select_sim_handler jmetoc_SIM = %d",jmetoc_SIM);

/*
	   switch (index_sim)
	{
#ifdef MMI_ON_HARDWARE_P
	
		case 0:

	jmetoc_SIM = MMI_SIM_1;
	mmi_trace(1,"MMI_SMS: VM_always_ask_sim1");

			break ;
		case 1:

	jmetoc_SIM = MMI_SIM_2;
	mmi_trace(1,"MMI_SMS: VM_always_ask_sim2");

			break ; 
			
		default:
			break ; 
#endif

	}
*/
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_OK, 5, 1000, SUCCESS_TONE);
	DeleteNHistory(1);
}
U8* nosimcard = "\x4e\x00\x4f\x00\x20\x00\x73\x00\x69\x00\x6d\x00\x20\x00\x63\x00\x61\x00\x72\x00\x64\x00\x00";
U8* Onesimcard = "\x4f\x00\x6e\x00\x6c\x00\x79\x00\x20\x00\x6f\x00\x6e\x00\x65\x00\x20\x00\x73\x00\x69\x00\x6d\x00\x20\x00\x63\x00\x61\x00\x72\x00\x64\x00\x00";
#include "mmi_features_camera.h"
#include "cameraresdefs.h"
VOID mmi_display_noSimCard(void) 
{
	U16 string_id=0;	
	ClearAllKeyHandler();
	string_id=STR_ID_CAMERA_NOTIFY_ON_SIMCARD;
	DisplayPopup((PU8)GetString(string_id), IMG_GLOBAL_WARNING, 5, 1000, WARNING_TONE);
	//DisplayPopup(nosimcard, 26051, 5, 1000, 4);

}
VOID mmi_display_OneSimCard(void) 
{
	U16 string_id=0;	
	ClearAllKeyHandler();
	string_id=STR_ID_CAMERA_NOTIFY_ONLY_ONE_SIM;
	DisplayPopup((PU8)GetString(string_id), IMG_GLOBAL_WARNING, 5, 1000, WARNING_TONE);
	//DisplayPopup(Onesimcard, 26051, 5, 1000, 4);

}
VOID mmi_display_InFlightMode(void) 
{
	U16 string_id=0;	
	ClearAllKeyHandler();
		DisplayPopup((PU8)GetString(STRING_MTPNP_DM_FLIGHT) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	//DisplayPopup(Onesimcard, 26051, 5, 1000, 4);

}
UINT8 checksim(void)
{
	U8 sim_num = 0;
	U8 total_num = 0;
	U8 first_sim =0xff;
	U8 tmp_sim;
 #ifdef MMI_ON_HARDWARE_P
	tmp_sim = VM_readSimNum();
#endif
	for(sim_num=0;sim_num<MMI_SIM_NUMBER;sim_num++)
		if((dm_card_status[sim_num]&0x10)==0x10)
		{
			simUsableindextoId[total_num] = sim_num;
			
			if( sim_num == tmp_sim)
			    index_sim = total_num;
			total_num++;
		}
		
simUsableindextoId[4] = total_num;
return total_num;
/*

      
#ifdef MMI_ON_HARDWARE_P
	E_MTPNP_AD_SIMCARD_USABLE_SIDE usableSide = MTPNP_AD_Get_UsableSide();
	switch(usableSide)
	{
		case MTPNP_AD_DUALSIM_USABLE:
			if(jmetoc_SIM ==0xff)
			      jmetoc_SIM= CFW_SIM_0;
		      return 2;
		case MTPNP_AD_SIMCARD1_USABLE:
			 jmetoc_SIM= CFW_SIM_0;
			 return 1;
		case MTPNP_AD_SIMCARD2_USABLE:
			jmetoc_SIM= CFW_SIM_1;
			return 1;
		default:
			jmetoc_SIM= 0xff;
//			mmi_display_noSimCard();
			return 0;
	}
	#endif
	*/
}


UINT8 vm_setSmsChannel(void)
{
      U8 sim_num = 0;
     U8 first_sim =0xff;
	 U8 tmp_sim;
 #ifdef MMI_ON_HARDWARE_P
	tmp_sim = VM_readSimNum();
 #endif

	for(sim_num=0;sim_num<MMI_SIM_NUMBER;sim_num++)
		if((dm_card_status[sim_num]&0x10)==0x10)
		{
		  MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);

			MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_MASTER+sim_num);
                     if( sim_num == tmp_sim)
			    return sim_num;
		       else if(0xff ==first_sim)
			   	first_sim =sim_num;
		}
	return first_sim;	
}

void RegisterInputMethodScreenCloseFunction(void (*f)(void));

void VM_select_sim(void)
{

//    SetKeyHandler(fixed_list_goto_previous_item, KEY_UP_ARROW, KEY_EVENT_DOWN);
//    SetKeyHandler(fixed_list_goto_next_item, KEY_DOWN_ARROW, KEY_EVENT_DOWN);

  //    MTPNP_jwap_always_ask_select_sim_entry_handler();
//	return;
		
	U8* guiBuffer;
	U16 nStrItemList[MMI_SIM_NUMBER];
	U16 nImgIltemList[MMI_SIM_NUMBER];
	U16 nStrItemListEx[MMI_SIM_NUMBER];
	U16 nImgIltemListEx[MMI_SIM_NUMBER];

	U16 nNumofItem;
	U8 indexnum = 0;

	mmi_trace(1,"MMI_SMS: VM_select_sim");

      RegisterInputMethodScreenCloseFunction(NULL);
	EntryNewScreen(SCR_MTPNP_SELECT_SIM_MENU, NULL, VM_select_sim, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_MTPNP_SELECT_SIM_MENU);

	//nNumofItem = GetNumOfChild(MENU_ID_MTPNP_SELECT_SIM_MENU);
	nNumofItem = simUsableindextoId[4];
	
	GetSequenceStringIds(MENU_ID_MTPNP_SELECT_SIM_MENU, nStrItemList);

	for(indexnum=0;indexnum<simUsableindextoId[4];indexnum++)
      {
          nStrItemListEx[indexnum] = nStrItemList[simUsableindextoId[indexnum]];
      }
	
	GetSequenceImageIds(MENU_ID_MTPNP_SELECT_SIM_MENU, nImgIltemList);
	for(indexnum=0;indexnum<simUsableindextoId[4];indexnum++)
      {
          nImgIltemListEx[indexnum] = nImgIltemList[simUsableindextoId[indexnum]];
      }

	
	
	SetParentHandler(MENU_ID_MTPNP_SELECT_SIM_MENU);
	RegisterHighlightHandler(VM_select_sim_Hlt_handler);

	ShowCategory52Screen(STR_GLOBAL_OPTIONS, 
		#if defined(JATAAYU_SUPPORT)
		IMG_ID_JATAAYU_WAP_ICON,
		#else
		0,
		#endif
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemListEx, nImgIltemListEx,
						  0, 0, index_sim, guiBuffer);
      SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP); 
	SetLeftSoftkeyFunction (VM_select_sim_handler ,KEY_EVENT_UP) ;
//	SetKeyHandler(fixed_list_goto_previous_item, KEY_UP_ARROW, KEY_EVENT_DOWN);
//   SetKeyHandler(fixed_list_goto_next_item, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	//   	mmi_trace(1,"MMI_SMS: VM_select_sim 2");



}
#else
//qiff WAP need
#include "mmi_features_camera.h"
#include "cameraresdefs.h"

VOID mmi_display_noSimCard(void) 
{
	U16 string_id=0;	
	ClearAllKeyHandler();
	string_id=STR_ID_CAMERA_NOTIFY_ON_SIMCARD;
	DisplayPopup((PU8)GetString(string_id), IMG_GLOBAL_WARNING, 5, 1000, WARNING_TONE);
	//DisplayPopup(nosimcard, 26051, 5, 1000, 4);

}

#endif
//add end
#endif

