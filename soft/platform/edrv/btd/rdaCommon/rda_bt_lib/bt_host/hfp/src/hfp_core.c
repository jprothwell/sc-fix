/* !=============================================================! */
/* !                M O D U L E     B O D Y                      ! */
/* !=============================================================! */
/* !                                                             ! */
/* ! Filename: hfp_core.c  (Nov, 2004)                           ! */
/* !                                                             ! */
/* ! Authors:  Daire McNamara, Gary Fleming.                     ! */
/* !                                                             ! */
/* !                                                             ! */
/* ! LICENSE:                                                    ! */
/* !   This source code is copyright (c) 2004 Ceva Technologies. ! */
/* !   All rights reserved.                                      ! */
/* !                                                             ! */
/* ! Description:                                                ! */
/* !  Hands Free Profile Core.				                     ! */
/* !                                                             ! */
/* ! History:                                                    ! */
/* !                                                             ! */
/* !=============================================================! */

#include <ctype.h>   //toupper
#include <stdio.h>   //sprintf
#include <string.h>

#include "papi.h"
#include "stack_ent_api.h"
#include "host_error.h"
#include "host_types.h"
#include "sdp_constants.h"
#include "rfcomm_types.h"
#include "host_config.h"
#include "host_buf.h"
#include "sdp_ue.h"
#include "stack_ent_core.h"

#if COMBINED_HOST==1
#include "lmp_scan.h"
#include "lmp_acl_container.h"
#include "lmp_config.h"
#include "lmp_ch.h"
#include "lmp_link_policy_protocol.h"
#include "lmp_sco_container.h"
#else
#include "hci_primitives.h"
#include "mgr_core.h"
#endif

#include "l2cap_primitives.h"
#include "sdp_server.h"

#include "hfp_types.h"
#include "hfp_core.h"
#include "hfp_const.h"
#include "host_msg.h"
#ifndef BD_ADDR_CMP /* An optimised version may be defined in platform_operations.h */
#define BD_ADDR_CMP(x,y) !pMemcmp(((x).bytes), ((y).bytes), 6)
#endif

/* API function return code */
//typedef int t_api;	

//???????????????????????????????????????????????
#define TEST_NO_SNIFF

/*************************************************************************/
/*********                    Definitions                       **********/
/*************************************************************************/
#define HFP_CORE_DEBUG 0

#define AUDIO_GATEWAY 0   // Only Turned on when acting as a test Audio Gateway
                          // The value "1" is currently not supported.
#define USE_SECURITY 0
#define HFP_VOICESETTING 0x0010
#define HFP_MULTIPOINT_SUPPORT 1
#define NUM_SCNS_SUPPORTED 1 //7
#define HFP_AG_NUMBER   1   //zhujinaguo added it. 

/*************************************************************************/
/*********                 Global Parameters                    **********/
/*************************************************************************/
//bool   hfp_allowIncomingSLC; //Control incoming SLC

//u_int8 g_SCO_AutoAccept=0;
//Rfcomm Callback structure

t_RSE_Callbacks rfcommCallbacks;

// Pointer to entry in AG container for
// device to which a call is currently
// being established 

//t_HFP_AG_Peer_Device* 	g_p_device_call_pending;

// Container to descrbe 6 Audio Gateways

t_HFP_AG_Peer_Device HFP_AG_Device[HFP_AG_NUMBER];

//Global information for DTMF string
 
u_int8 g_hfp_dtmf_len = 0 ;
u_int8 g_hfp_dtmf_offset = 0;
u_int8 g_num_mem_locs = 0;
char g_hfp_dtmf_string[PRH_HFP_MAX_DTMF_STRING_SIZE];


// SDP Reocord Handle -- Needs to be stored so record can be removed on SDP close.
u_int32 HFP_SDPrecordHandle;
t_SCN_values HF_Scn_Array[NUM_SCNS_SUPPORTED];
t_pTimer rda_sdp_timer = 0;//zhu jianguo added it. 2009.7.13 
/* Used to interface to Rfcomm flow control*/
#if (HFP_MULTIPOINT_SUPPORT==1)


#else
t_pEvent evHFP_TxOK = 0;
int HFP_TxOK=0;
#endif
static u_int8 HFP_SrvChan;
// Structure containing callback to the application
#if COMBINED_HOST==0
st_t_hf_Callbacks HF_callbacks;
#endif
t_bdaddr gCurrent_Active_Addr;

/* defines for flow indication */
#define RSE_FLOW_STOP 0
#define RSE_FLOW_GO 1

/* defines for connection confirmation */
#define RSE_CONNECT_REFUSE 0
#define RSE_CONNECT_ACCEPT 1

/* SDP Info */
#if SDP_CLIENT
static t_SDP_SearchPattern pattern; 
static t_SDP_AttributeIDs attribs;
static t_SDP_Results *pResults;
static t_SDP_Addresses devices;
#endif
u_int8 Bldn_Present_Flag=0;
u_int8 HFP_Browse_Pending=0;

u_int8  gCurrent_Active_Profile=ACTIVE_PROFILE_HANDSFREE_AG;
u_int8 gFirstFlag =0;
t_pTimer gSLCConnectTimer;

/*************************************************************************/
/*********                 Extern Parameters                    **********/
/*************************************************************************/
extern int prh_num;
extern int rse_num;
/*************************************************************************/
/*********                 Exported Functions                   **********/
/*************************************************************************/

/*************************************************************************/
/*********                  Local Functions                     **********/
/*************************************************************************/
// add by tianwq
TdbDeviceRecord DB_Device_Record;
extern u_int32 rdabt_put_message(u_int16 q, u_int16 mi, void *mv);

typedef enum CallState
{
    CM_IDLE_STATE = 0,
    CM_OUTGOING_STATE,
    CM_INCOMING_STATE,
    CM_ACTIVE_STATE,
    CM_HOLD_STATE,
    CM_DISCONNECTING_STATE, /* Start by Julia: Drop call refinement */
    CM_STATE_UNKNOWN
} CALL_STATE;
int g_call_answered_by_hf = FALSE;
int Release_SLC_Flag=0;//zhou siyou add 20070722
typedef int (*p_rda_GetCurrentCallStatus)();                   //get current call status 
p_rda_GetCurrentCallStatus rda_GetCurrentCallStatus = NULL;
extern t_RSE_Callbacks HS_rfcommCallbacks;
void rda_setCurrentCallStatusCB(void * func)
{
     rda_GetCurrentCallStatus = (p_rda_GetCurrentCallStatus)func;
}

typedef int (*p_rda_GetCallHoldCount)();                       //get hold call count num
p_rda_GetCallHoldCount rda_GetCallHoldCount = NULL;
void rda_setCallHoldCountCB(void * func)
{
     rda_GetCallHoldCount = (p_rda_GetCallHoldCount)func;
}

typedef int (*p_rda_GetSignalStrength)();                     //get signalStrength 
p_rda_GetSignalStrength rda_GetSignalStrength = NULL;
void rda_SetSignalStrengthCB(void * func)
{
     rda_GetSignalStrength = (p_rda_GetSignalStrength)func;
}

typedef int (*p_rda_GetRoamStatus)();                       //get roaming status
p_rda_GetRoamStatus rda_GetRoamStatus = NULL;
void rda_SetRoamStatusCB(void * func)
{
     rda_GetRoamStatus = (p_rda_GetRoamStatus)func;
}
TdbDeviceRecord *DB_GetDeviceRecordByBdAddress(t_bdaddr *address)
{
	int i=0,j=0;
	TdbDeviceRecord* peer_device;

	for(i=0;i<HFP_AG_NUMBER;i++)
	{
		//peer_device= &HFP_AG_Device[i];
		peer_device = &DB_Device_Record;
		if(BD_ADDR_CMP(*address, HFP_AG_Device[i].address))
		{
			for(j=0;j<6;j++)
				peer_device->bdAddress.bytes[j] = HFP_AG_Device[i].address.bytes[j];
			return peer_device;
		}
	}

	return NULL;
}
void *OS_HEAP_Alloc(size_t size)
{
	//return (void *)malloc(size);
    return (void *)pMalloc(size);
}
void OS_HEAP_Free(void *ptr)
{
	pFree(ptr);
}


static int validate_and_store_dtmf_string(char* dtmf_sequence);

/****************************************************************
 * Local Function Definitions for interfacing to SDP/RFCOMM/Mgr/HCI
 ****************************************************************/

//static int HF_SetupSDPRecord(u_int8 rf_serverChannel, char *serviceName, u_int16 serviceNameLen);
//static int HF_SetupHeadsetAG_SDPRecord(u_int8 rf_serverChannel, char *serviceName, u_int16 serviceNameLen);

static int sdpSearchCompleteCallback(u_int8 status);

static int HF_rfcommRxDataCallback(t_bdaddr address, u_int8 dlci, struct host_buf *rxData);
static int HF_rfcommConnectIndicationCallback(u_int8 schan, t_bdaddr address, u_int8 dlci, u_int16 max_frame_size);
static int HF_rfcommConnectConfirmCallback(u_int8 schan, t_bdaddr address, u_int8 status, u_int8 dlci, u_int16 max_frame_size);
static int HF_rfcommDisconnectIndicationCallback(t_bdaddr address, u_int8 dlci);	
static int HF_rfcommFlowCtrlCallback(t_bdaddr address, u_int8 dlci,  u_int8 flow_state);
static void HFP_RSE_Retransmit(void *arg);

#if COMBINED_HOST==0
static void HF_SCO_Setup_Mgr_Callback(u_int16 *scoHandle, t_api status/*, t_bdaddr *bdAddress*/);   //zhujianguo changed it. 2007.12.28
//static void HF_SCO_Setup_Mgr_Callback(u_int16 *scoHandle, t_api status);
static void HF_SCO_Release_Mgr_Callback(u_int16 scoHandle, t_api status);
#endif

u_int8 HF_GetAvailableSCN(void);
void HF_FreeSCN(u_int8 scn);
void HF_UpdateSDPRecord(u_int8 serverChannel);

u_int8 HF_Connect_As_Client(t_bdaddr ag_bdaddr);
void HFP_hciEventCallback(u_int8 *eventPdu);

/****************************************************************
 * Local Function Definitions for AT command generation/handling
 ****************************************************************/

static int HF_Enable_Called_Waiting(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI);
static int HF_Disable_Call_Waiting(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI);
static int HF_Activate_Indicators_Event_Reporting(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI);
//static int HF_Deactivate_Indicators_Event_Reporting(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI);
static int HF_Activate_Voice_Recognition(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI);
static int HF_DeActivate_Voice_Recognition(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI);
static int HF_Retrieve_Supported_Indicators(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI);
static int HF_DeActivate_NR_EC(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI);
static int HF_Respond_Support_Features(t_bdaddr *gatewayBDAddress, u_int8 gwDLCI);
static int HF_Respond_OK(t_bdaddr *gatewayBDAddress, u_int8 gwDLCI);
static int HF_Report_Mic_Gain_To_AG(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI, u_int8 gain);
static int HF_Report_Speaker_Gain_To_AG(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI, u_int8 gain);
static int HF_Set_Mic_Gain_To_HS(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI, u_int8 gain);
static int HF_Set_Speaker_Gain_To_HS(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI, u_int8 gain);

static int HF_Dial_Last_Number(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI);
static int HF_Activate_CLIP(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI);
static int HF_DeActivate_CLIP(t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI);
static int HF_SendToGateway(u_int8 *data, int dataLen, t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI);
static int HF_Send_Call_Setup(t_bdaddr ag_bdaddr, u_int8 call_type, char* called_party_num_string, u_int16 memory_id);
static int HF_Send_DTMF_Tone(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI,char dtmf);
static int HF_Send_Answer(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI);
static int HF_Send_Reject(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI);
static int HF_Send_BINP(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI);
//static int HF_Send_BVRA(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI,u_int8 ind_value);

static int HF_Retrieve_Support_Features(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI); // Only for HFP V1.0 devices.
static int HF_Send_Indicators(u_int8 indicator_type, u_int8 indicator_value,t_bdaddr *gatewayBDAddress, u_int16 gwDLCI);

//static int HF_Send_CLIP(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI);
static int HF_Send_RING(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI);
static int HF_Send_OK(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI);
//static int HF_Send_BVRA(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI,u_int8 ind_value);
static int HF_Send_Indicators_Supported(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI);
static int HF_Send_Indicators_Status(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI);

static void Handle_Rx_CIND(t_bdaddr* address,t_AT_Event* AT_Event);
static void Handle_Rx_CHLD(t_bdaddr* address,u_int8* p_call_handling_info);
static void Handle_Rx_BRSF(t_bdaddr* address, u_int32 supported_features);

static void Handle_Rx_CIEV(t_bdaddr* address,t_AT_Event* AT_Event);
static void Handle_OK_Command(t_bdaddr* address,u_int8 dlci);
static void Handle_Error_Command(t_bdaddr* address,u_int8 dlci);
static void Handle_RFCOMM_Disconnect(t_bdaddr* address);
static void Handle_Rx_RING(t_bdaddr* address);
static void Handle_Rx_CLIP(t_bdaddr* address,t_AT_Event* AT_Event);
static void Handle_Rx_CCWA(t_bdaddr* address,t_AT_Event* pAT_Event);
static void Handle_Rx_BSIR(t_bdaddr* address,t_AT_Event* pAT_Event);
static void Handle_Rx_BINP(t_bdaddr* address,t_AT_Event* pAT_Event);
static void Handle_Rx_BRVA(t_bdaddr* address,t_AT_Event* pAT_Event);
static void Handle_Rx_AT_CKPD(t_bdaddr* address,u_int16 dlci);
static void Handle_Rx_AT_CMER(t_bdaddr* address,u_int16 dlci,u_int8* p_u_int8);
static void Handle_Rx_ATA(t_bdaddr* address,u_int16 dlci);

static int HF_Decode_AT_Hayes(char* assemblyBuffer,t_bdaddr address, u_int8 dlci);
 int HF_Decode_AT_Command(char* assemblyBuffer,t_bdaddr address, u_int8 dlci);
 int HF_Decode_AT_USR(char* assemblyBuffer,t_bdaddr address, u_int8 dlci);
static int    HF_Update_Indicator_Status(u_int8* p_data,t_HFP_AG_Peer_Device* peer_device);
static int    HF_Find_Indicator_Value(u_int8* p_data,u_int8 ind_order,u_int8* p_ind_value,u_int8 max_value);
static void*  HF_Extract_Service_List(u_int8* p_data,t_HFP_AG_Peer_Device* peer_device);
static u_int8 HF_Extract_Call_Handling_Info(u_int8* p_data);
static u_int8* _Find_Numeric(u_int8* p_data,u_int8* numeric);

/*****************************************************
 * Local Functions for accessing AG container.
 *****************************************************/
static t_HFP_AG_Peer_Device* Find_Audio_GW_By_Address(t_bdaddr* address);
static t_HFP_AG_Peer_Device* Find_Free_Audio_GW(void);
static t_HFP_AG_Peer_Device* Find_Audio_GW_With_Audio_Setup_In_Progress(void);
static t_HFP_AG_Peer_Device* Find_Audio_GW_By_SCO_Handle(u_int16 sco_handle);
static t_HFP_AG_Peer_Device* Find_Audio_GW_By_ACL_Handle(u_int16 acl_handle);
//static t_HFP_AG_Peer_Device* Find_Audio_GW_Rejecting_SCO(void);


static void Free_Audio_GW(t_bdaddr *address);
static void HFP_Init_AG_Containers(void);
static int Is_Service_Browse_Ongoing(void);
static t_HFP_AG_Peer_Device* Find_Service_Browse_Ongoing(void);

/******************************************************
 * Local Functions for handling Sniff Mode
 ******************************************************/
void HF_Exit_Sniff_Mode(t_HFP_AG_Peer_Device* peer_device);
void HF_Enter_Sniff_Mode(void *arg);
t_HFP_AG_Peer_Device* HFP_AG_Pending_Entry_Into_Sniff(void);

/******************************************************
 * Local Functions for handling SLC Timeout
 ******************************************************/
void HF_SLC_Setup_Timeout(void *arg);  //Zhujianguo changed it. 2007.12.28


void HF_Ring_Timeout(void *arg);//Zhujianguo changed it. 2007.12.28

/***********************************************************************
* FUNCTION:  HF_Init
* DESCRIPTION:
*    Initialises the HFP layer. 
*       Initialises the local AG containers.
*       Sets up the callbacks.for comunication to the application.
*       Registers the RFCOMM callbacks for interfacing to RFCOMM.
*       Sets up the local SDP record.
*       Writes Voice-Settings, Class of Device, Local Name
**********************************************************************/
#if COMBINED_HOST==0
int HF_Init(u_int8 server_channel_number, st_t_hf_Callbacks* app_hf_callbacks)
#else
int HF_Init(void)
#endif
{
	int status = BT_NOERROR;
	char *LocalName;

#if (HFP_MULTIPOINT_SUPPORT==0)
	evHFP_TxOK = 0;
#endif
	//HFP_SrvChan= server_channel_number; // PRH_HANDS_FREE_PROFILE_SER_CHANNEL;

	HFP_SrvChan=PRH_HANDS_FREE_PROFILE_SER_CHANNEL;

	/* init the AG device Container */
	HFP_Init_AG_Containers();

#if COMBINED_HOST==0
	status=HCI_RegisterEventHandler(HFP_hciEventCallback);
#endif
	
	if (status!=BT_NOERROR)
		return status;

	/****************************************
	* Setup the HFP Callback Structure
	****************************************/
#if COMBINED_HOST==0	
	memset(&HF_callbacks,0,sizeof(HF_callbacks));
	HF_callbacks.HF_CB_ServiceDiscoveryResult = app_hf_callbacks->HF_CB_ServiceDiscoveryResult;
	HF_callbacks.HF_CB_SLC_Established_Ind = app_hf_callbacks->HF_CB_SLC_Established_Ind;
	HF_callbacks.HF_CB_SLC_Released_Ind = app_hf_callbacks->HF_CB_SLC_Released_Ind;
	HF_callbacks.HF_CB_SLC_Requested_Ind = app_hf_callbacks->HF_CB_SLC_Requested_Ind;
	HF_callbacks.HF_CB_Command_Complete = app_hf_callbacks->HF_CB_Command_Complete;
	HF_callbacks.HF_CB_Ring_Indication = app_hf_callbacks->HF_CB_Ring_Indication;
	HF_callbacks.HF_CB_Clip_Indication = app_hf_callbacks->HF_CB_Clip_Indication;
	HF_callbacks.HF_CB_Call_Waiting = app_hf_callbacks->HF_CB_Call_Waiting;
	HF_callbacks.HF_CB_Call_Alerting = app_hf_callbacks->HF_CB_Call_Alerting;
	HF_callbacks.HF_CB_Call_Release_Complete = app_hf_callbacks->HF_CB_Call_Release_Complete;
	HF_callbacks.HF_CB_Call_Active = app_hf_callbacks->HF_CB_Call_Active;
	HF_callbacks.HF_CB_Audio_Link_Released = app_hf_callbacks->HF_CB_Audio_Link_Released;
	HF_callbacks.HF_CB_Audio_Link_Established = app_hf_callbacks->HF_CB_Audio_Link_Established;
	HF_callbacks.HF_CB_Volume_Setting_Change_Req = app_hf_callbacks->HF_CB_Volume_Setting_Change_Req;
	HF_callbacks.HF_CB_Change_Volume_Complete = app_hf_callbacks->HF_CB_Change_Volume_Complete;
	HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update = app_hf_callbacks->HF_CB_Remote_Supplementary_Feature_Update;
	HF_callbacks.HF_CB_Send_DTMF_Complete = app_hf_callbacks->HF_CB_Send_DTMF_Complete;
	HF_callbacks.HF_CB_Phone_Status_Ind = app_hf_callbacks->HF_CB_Phone_Status_Ind;
	HF_callbacks.HF_CB_Audio_Request = app_hf_callbacks->HF_CB_Audio_Request;
#endif

#if COMBINED_HOST==1
	LC_Write_Local_Device_Class(HANDS_FREE_UNIT_COD/* HEADSET_AG_COD*/); 
#else
	//status=HCI_WriteClassOfDevice(HANDS_FREE_UNIT_COD/* HEADSET_AG_COD*/); 
#endif
	//if (status!=BT_NOERROR) 
	//	return status;

	/* register as a RFCOMM server */
	memset(&rfcommCallbacks,0,sizeof(rfcommCallbacks));
	rfcommCallbacks.connectionConfirmation = HF_rfcommConnectConfirmCallback;
	rfcommCallbacks.connectionIndication = HF_rfcommConnectIndicationCallback;
	rfcommCallbacks.dataIndication = HF_rfcommRxDataCallback;
	rfcommCallbacks.disconnectionIndication = HF_rfcommDisconnectIndicationCallback;
	rfcommCallbacks.flowIndication = HF_rfcommFlowCtrlCallback;
	{
		/* As the RFCOMM and Stack Entity model are designed on a P2P 
		* client server model the current Stack Entity supports only 
		* a single ACL link per registered server channel when acting
		* as a server. Thus most risk free approach to allow 6 peers 
		* connect into the system is to support 6 seperate servers.
		*
		* Register 6 server Ports from HFP_SrvChan to HFP_SrvChan +12
		* and Init the SCN allocation array 
		*/
	
		int i;
		u_int8 temp_scn;
#if COMBINED_HOST==1
		prh_num = 0;
		rse_num = 0;
#endif
		for (i=0;i < NUM_SCNS_SUPPORTED; i++)
		{
			//temp_scn = HFP_SrvChan + (i*2);
			temp_scn = i+1;

			HF_Scn_Array[i].number = temp_scn;
			HF_Scn_Array[i].used = 0;
			status = RSE_SrvRegisterPort(&temp_scn, &rfcommCallbacks);

			if (status!=BT_NOERROR) 
			{
				return status;
			}	
		}
	}
#if SDP_RECORD_GEN
	status=HF_SetupSDPRecord(/*HF_GetAvailableSCN()*/1, "RDA HandsFree", 13/*(u_int16)strlen("RDA HandsFree")*/);

	if (status!=BT_NOERROR)
		return status;
#endif

#if COMBINED_HOST==1
	//status=LMscan_Write_Scan_Enable(0x03);  //Inquiry scan enable, page scan enable 

	//if (status!=BT_NOERROR)
	//	return 1;
	
	//LocalName="RDA BT Unit ";
	//LMconfig_LM_Change_Local_Name((u_int8*)LocalName);
#else	
	status=HCI_WriteScanEnable(0x03);  //Inquiry scan enable, page scan enable 

	if (status!=BT_NOERROR)
		return 1;
	
	LocalName="RDA HandFree";
	HCI_ChangeLocalName((u_int8*)LocalName);
	/* enable authentication and encryption */
#if USE_SECURITY
    MGR_SetSecurityMode(MGR_LINKLEVEL,NULL);
    HCI_WriteEncryptionMode(0x01);
#endif
#endif
	HFP_Browse_Pending=0;

    return status;

}/*End of HF_Init()*/


/*int HF_Configure(u_int8 autoAcceptSCO)
{
	g_SCO_AutoAccept= autoAcceptSCO;

    return 0;
}*/
			

/***********************************************************************
* FUNCTION:  HF_Close
* DESCRIPTION:
*    Closes the HFP layer, by deregistering the RFCOMM server.
*    NOTE TBC :- Must also remove the SDP database for the profile
**********************************************************************/
int HF_Close(void)
/**********************************************************************/
{
	int status = BT_NOERROR;
    int i=0;

#if SDP_RECORD_GEN
    status = SDP_RemoveRecord(HFP_SDPrecordHandle);

    if (status != BT_NOERROR)
        return status;
#endif

#if COMBINED_HOST==0
	HCI_DeregisterEventHandler(HFP_hciEventCallback);
#endif

    for (i=0;i < NUM_SCNS_SUPPORTED; i++)
        status = RSE_SrvDeregisterPort(HF_Scn_Array[i].number);

	return status;

}/*End of HF_Close()*/


/***********************************************************************
* FUNCTION:  HF_Device_State
* DESCRIPTION:
*    For test purposes only, returns the state of an SLC.
*
**********************************************************************/

int HF_Device_State(t_bdaddr ag_bdaddr)
{
	t_HFP_AG_Peer_Device* peer_device;
	
	/* Ensure that an SLC does not exist already for this bdaddr */

	/* Call RFCOMM Establishment */
	
	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);

	if (peer_device)
		return peer_device->scm_state;
	else
		return 0xFF;
}

/***********************************************************************
* FUNCTION:  HF_ServiceDiscover_AG
* DESCRIPTION:
*    Initiates an SDP Search for the attributes of a device supporting the
*    HF profile. The Server Channel is the most important element to be 
*    retrieved. 
*
*    The following attributes need be retrieved by SDP
*
*               server channel number
*               version information
*               supported features
**********************************************************************/
#if 1
int HF_ServiceDiscover_AG(t_bdaddr address) 
/**********************************************************************/
{
    int status=BT_NOERROR;
#if SDP_CLIENT 
    u_int16 criteria;
    t_SDP_StopRule rule;

    if (HFP_Browse_Pending)
    {	
        return BT_RETRY;
    }

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_ServiceDiscover_AG\n"));
#endif  

    //	return HF_ServiceDiscover_Headset_AG(address);

    /************************************
    * Flag to indicate Browse is pending 
    ************************************/

    HFP_Browse_Pending=1;

    /******************************/
    /* address list to search for */
    /******************************/
    devices.numItems = 1;
    devices.addresses = OS_HEAP_Alloc(sizeof(t_bdaddr));
    if(!devices.addresses)
    {
        HFP_Browse_Pending=0;
        return BT_NORESOURCES; 
    }
    devices.addresses[0] = address;

    /* device relation criteria */
    criteria = 0;

    /******************************/
    /* search pattern             */
    /******************************/

    pattern.numItems = 1;
    pattern.patternUUIDs = OS_HEAP_Alloc(pattern.numItems*sizeof(u_int32));

    if(!pattern.patternUUIDs)
    {
        if(devices.addresses)
        {
            OS_HEAP_Free(devices.addresses);
            HFP_Browse_Pending=0;
        }
        return BT_NORESOURCES;   
    }
    // PROFILE DEPENDENT
    //  pattern.patternUUIDs[0] = SDP_SCLASS_UUID_HANDSFREE_AUDIOGATEWAY;
    pattern.patternUUIDs[0] = SDP_SCLASS_UUID_HANDSFREE;
#if 0 // Headset support 5 May
    pattern.patternUUIDs[1] = SDP_SCLASS_UUID_HEADSET_GATEWAY;
#endif
    /* attributes to retrieve */

#if 0 // Headset support 5 May
    attribs.numItems = 4;
#else
    attribs.numItems = 3;
#endif
    attribs.attributeIDs = OS_HEAP_Alloc(attribs.numItems*sizeof(u_int32));

    if(!attribs.attributeIDs)
    {
        if(devices.addresses)
            OS_HEAP_Free(devices.addresses);		
        if(pattern.patternUUIDs)
            OS_HEAP_Free(pattern.patternUUIDs);	

        HFP_Browse_Pending=0;
        return BT_NORESOURCES;
    }

    attribs.attributeIDs[0] = SDP_AID_PROTOCOL;
    attribs.attributeIDs[1] = SDP_AID_PROFILE_LIST;
    attribs.attributeIDs[2] = SDP_AID_SUPPORTED_FEATURES;
#if 0 // Headset support 5 May
    attribs.attributeIDs[3] = SDP_AID_AUDIO_VOLUME;
#endif
    /* stop rule */
    rule.maxItems = 10;
    rule.maxDuration = 40;
    rule.maxBytes = 250;

    /* call a service search  */
    status=SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,(sdp_api_callback)sdpSearchCompleteCallback);    
    if (status!=BT_PENDING)
    {
#if HFP_CORE_DEBUG	
        printf("\n Error in SDP browse ");
#endif
        if(devices.addresses)
            OS_HEAP_Free(devices.addresses);		
        if(pattern.patternUUIDs)
            OS_HEAP_Free(pattern.patternUUIDs);	
        if(attribs.attributeIDs)
            OS_HEAP_Free(attribs.attributeIDs);	

        HFP_Browse_Pending=0;
        return status;
    } 
    else
    {
        TdbDeviceRecord *deviceRecord;

        /* Update global DB. */
        deviceRecord = DB_GetDeviceRecordByBdAddress(&address);
        if (deviceRecord) 
        {
            deviceRecord->profile.hfp.sdp_state = DB_SDP_PROGRESS;
        }
    }
#endif

    return status;

}/*End of HF_ServiceDiscover_AG()*/
#else
int ServiceSearchCompleteCallback(u_int8 status);
int HF_ServiceDiscover_AG(t_bdaddr address) 
/**********************************************************************/
{
    int status=BT_NOERROR;
#if SDP_CLIENT 
    u_int16 criteria;
    t_SDP_StopRule rule;

    if (HFP_Browse_Pending)
    {	
        return BT_RETRY;
    }

    //	return HF_ServiceDiscover_Headset_AG(address);

    /************************************
    * Flag to indicate Browse is pending 
    ************************************/

    HFP_Browse_Pending=1;

    /******************************/
    /* address list to search for */
    /******************************/
    devices.numItems = 1;
    devices.addresses = OS_HEAP_Alloc(sizeof(t_bdaddr));
    if(!devices.addresses)
    {
        HFP_Browse_Pending=0;
        return BT_NORESOURCES; 
    }
    devices.addresses[0] = address;

    /* device relation criteria */
    criteria = 0;

    /******************************/
    /* search pattern             */
    /******************************/

    pattern.numItems = 1;
    pattern.patternUUIDs = OS_HEAP_Alloc(pattern.numItems*sizeof(u_int32));

    if(!pattern.patternUUIDs)
    {
        if(devices.addresses)
        {
            OS_HEAP_Free(devices.addresses);
            HFP_Browse_Pending=0;
        }
        return BT_NORESOURCES;   
    }
    // PROFILE DEPENDENT
    //  pattern.patternUUIDs[0] = SDP_SCLASS_UUID_HANDSFREE_AUDIOGATEWAY;
    pattern.patternUUIDs[0] = SDP_PROTOCOL_UUID_L2CAP;
#if 0 // Headset support 5 May
    pattern.patternUUIDs[1] = SDP_SCLASS_UUID_HEADSET_GATEWAY;
#endif
    /* attributes to retrieve */

#if 0 // Headset support 5 May
    attribs.numItems = 4;
#else
    attribs.numItems = 4;
#endif
    attribs.attributeIDs = OS_HEAP_Alloc(attribs.numItems*sizeof(u_int32));

    if(!attribs.attributeIDs)
    {
        if(devices.addresses)
            OS_HEAP_Free(devices.addresses);		
        if(pattern.patternUUIDs)
            OS_HEAP_Free(pattern.patternUUIDs);	

        HFP_Browse_Pending=0;
        return BT_NORESOURCES;
    }

    attribs.attributeIDs[0] = SDP_AID_SERVICE_CLASS;
    attribs.attributeIDs[1] = SDP_AID_PROTOCOL;
    attribs.attributeIDs[2] = SDP_AID_PROFILE_LIST;
    attribs.attributeIDs[3] = SDP_AID_SUPPORTED_FEATURES;
#if 0 // Headset support 5 May
    attribs.attributeIDs[3] = SDP_AID_AUDIO_VOLUME;
#endif
    /* stop rule */
    rule.maxItems = 10;
    rule.maxDuration = 40;
    rule.maxBytes = 1024;

    /* call a service search  */
    status=SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,(sdp_api_callback)sdpSearchCompleteCallback/*ServiceSearchCompleteCallback*/);    
    if (status!=BT_PENDING)
    {
#if HFP_CORE_DEBUG	
        printf("\n Error in SDP browse ");
#endif
        if(devices.addresses)
            OS_HEAP_Free(devices.addresses);		
        if(pattern.patternUUIDs)
            OS_HEAP_Free(pattern.patternUUIDs);	
        if(attribs.attributeIDs)
            OS_HEAP_Free(attribs.attributeIDs);	

        HFP_Browse_Pending=0;
        return status;
    } 
    else
    {
        TdbDeviceRecord *deviceRecord;

        /* Update global DB. */
        deviceRecord = DB_GetDeviceRecordByBdAddress(&address);
        if (deviceRecord) 
        {
            deviceRecord->profile.hfp.sdp_state = DB_SDP_PROGRESS;
        }
    }
#endif

    return status;

}/*End of HF_ServiceDiscover_AG()*/
#endif
#if 1
/***********************************************************************
* FUNCTION:  HF_ServiceDiscover_AG
* DESCRIPTION:
*    Initiates an SDP Search for the attributes of a device supporting the
*    Headsetprofile. The Server Channel is the most important element to be 
*    retrieved. 
*
*    The following attributes need be retrieved by SDP
*
*               server channel number
*               Volume Control Support
**********************************************************************/
int HF_ServiceDiscover_Headset_AG(t_bdaddr address) 
/**********************************************************************/
{
    int status=BT_NOERROR;
#if 0
    u_int16 criteria;
    t_SDP_StopRule rule;
    
    if (HFP_Browse_Pending)
	{	
		return BT_RETRY;
	}

	/************************************
	 * Flag to indicate Browse is pending 
	 ************************************/

	HFP_Browse_Pending=1;

	/******************************/
    /* address list to search for */
	/******************************/
    devices.numItems = 1;
	devices.addresses = OS_HEAP_Alloc(sizeof(t_bdaddr));
		if(!devices.addresses)
		{
			HFP_Browse_Pending=0;
			return BT_NORESOURCES; 
		}
		devices.addresses[0] = address;
    
    /* device relation criteria */
    criteria = 0;
    
	/******************************/
    /* search pattern             */
	/******************************/

    pattern.numItems = 1;

	pattern.patternUUIDs = OS_HEAP_Alloc(pattern.numItems*sizeof(u_int32));

	if(!pattern.patternUUIDs)
	{
		if(devices.addresses)
		{
			OS_HEAP_Free(devices.addresses);
			HFP_Browse_Pending=0;
		}
		return BT_NORESOURCES;   
	}

    pattern.patternUUIDs[0] = SDP_SCLASS_UUID_HEADSET;

    /* attributes to retrieve */
    
    attribs.numItems = 1;

	attribs.attributeIDs = OS_HEAP_Alloc(attribs.numItems*sizeof(u_int32));

	if(!attribs.attributeIDs)
	{
		if(devices.addresses)
			OS_HEAP_Free(devices.addresses);		
		if(pattern.patternUUIDs)
			OS_HEAP_Free(pattern.patternUUIDs);	
		
		HFP_Browse_Pending=0;
		return BT_NORESOURCES;
	}

  	attribs.attributeIDs[0] = SDP_AID_PROTOCOL;

    /* stop rule */
    rule.maxItems = 10;
    rule.maxDuration = 40;
    rule.maxBytes = 250;

    
    /* call a service search  */
    status=SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,(sdp_api_callback)sdpSearchCompleteCallback);    
	if (status!=BT_PENDING)
	{
#if HFP_CORE_DEBUG	
		printf("\n Error in SDP browse ");
#endif
		if(devices.addresses)
			OS_HEAP_Free(devices.addresses);		
		if(pattern.patternUUIDs)
			OS_HEAP_Free(pattern.patternUUIDs);	
		if(attribs.attributeIDs)
			OS_HEAP_Free(attribs.attributeIDs);	

		HFP_Browse_Pending=0;
		return status;
	} 
	else
	{
		TdbDeviceRecord *deviceRecord;

		/* Update global DB. */
		deviceRecord = DB_GetDeviceRecordByBdAddress(&address);
		if (deviceRecord) 
		{
			deviceRecord->profile.hfp.sdp_state = DB_SDP_PROGRESS;
		}
		
	}
#endif

	return status;

}/*End of HF_ServiceDiscover_AG()*/

#endif
/***********************************************************************
* FUNCTION:  HF_Setup_SLC
* DESCRIPTION:
*    Initiates a service link connection to a AG. 
*    Performs first step which is to setup an RFCOMM connection
**********************************************************************/
int HF_Setup_SLC(t_bdaddr ag_bdaddr)
/**********************************************************************/
{
// PROFILE DEPENDENT
	int active_profile = ACTIVE_PROFILE_HANDSFREE_AG;
	//int active_profile = ACTIVE_PROFILE_HANDSFREE;
	int status = BT_NOERROR;
	t_HFP_AG_Peer_Device* peer_device;
	
	/* Ensure that an SLC does not exist already for this bdaddr */

	/* Call RFCOMM Establishment */

	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);

#if pDEBUG
     if(peer_device)    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Setup_SLC peer_device=0x%x\n",peer_device));
#endif  

	// Only allow an SLC to be setup IF 
	//    1/ Device Does not exist already
	//    2/ Device is in closed state
	//    3/ Service Discovery is pending for the device.

	if ((peer_device == 0) || (peer_device->scm_state == SCM_CLOSED))
	{
		// Continue
	}
	else
	{
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Setup_SLC BT_DISALLOWED\n"));
#endif  
		return BT_DISALLOWED;
	}


	if ((peer_device != 0) && (peer_device->serverChannel != 0))
	{

		status = HF_Connect_As_Client(ag_bdaddr);
	}
	else
	{
		/* Ensure that an SDP Service Browse  is not already taking place.
		 * For correct operation of the sytem cannot have two SDP Service
		 * Browse occuring in the system
		 */

            if (Is_Service_Browse_Ongoing())
		{
		
			return BT_DISALLOWED;
		}
		else
		{
			/* Ensure that an SLC does not exist already for this bdaddr */
			if (!peer_device)
				peer_device = Find_Free_Audio_GW();
			
			if (peer_device)
			{
				peer_device->scm_state = SCM_SDP_DISCOVERY_PENDING;
//----------------------------------------------
// GF 
//  TEST FUNCTIONALITY FOR MUTIPLE PROFILES
//
//----------------------------------------------
				peer_device->active_profile = active_profile;
			}
			
			/* Init SDP Browse 
			*/

		  if (peer_device)
		    {
			{ 
				int i;
				
				for (i=0;i<6;i++)
					peer_device->address.bytes[i] = ag_bdaddr.bytes[i];
			}
			status = HF_ServiceDiscover_AG(peer_device->address);
			
		  }
			
		}
		
	}

	if (!((status == BT_NOERROR) || (status == BT_PENDING)))
	{
		Free_Audio_GW(&peer_device->address);
	}
	return status;
}/*End of HF_Setup_SLC()*/


/***********************************************************************
* FUNCTION:  HF_Release_SLC
* DESCRIPTION:
*    Releases the SLC link to an AG. Depending on the role of the device
*    as an RFCOMM Client or Server either RSE_SrvDisconnectPort or 
*    RSE_CliDisconnectPort is called.
*
*    The event used for flow control is also released.
**********************************************************************/
extern unsigned char is_a2dp_connect(void);
int HF_Release_SLC(t_bdaddr ag_bdaddr)
/**********************************************************************/
{
	int status = BT_NOERROR;
	t_HFP_AG_Peer_Device* peer_device;
	
	/* Ensure that an SLC does not exist already for this bdaddr */

	/* Call RFCOMM Establishment */
	
	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);
	if (peer_device != 0)
	{
             pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Release_SLC rf_role=%d\n", peer_device->rf_role));
		if (peer_device->rf_role == RFCOMM_SERVER)
		{

            status = RSE_SrvDisconnectPort(peer_device->local_serverChannel);

	//  Was Bug Here !! Should Not transition to SCM_CLOSED until I get the RFCOMM Callback for disconnect Indication.
		//	peer_device->scm_state = SCM_CLOSED;
			peer_device->scm_state = SCM_W4_RFCOMM_DISCONNECT_INDICATION;
#if (HFP_MULTIPOINT_SUPPORT==1)
			//if (peer_device->evHFP_TxOK)
			//	pEventFree(peer_device->evHFP_TxOK);
			peer_device->evHFP_TxOK = 0;
#else
			if (evHFP_TxOK)
				pEventFree(evHFP_TxOK);
			evHFP_TxOK = 0;
#endif
		}
		else if (peer_device->rf_role == RFCOMM_CLIENT)
		{
			status = RSE_CliDisconnectPort(peer_device->address,peer_device->serverChannel);
			//  Was Bug Here !! Should Not transition to SCM_CLOSED until I get the RFCOMM Callback for disconnect Indication.
			//	peer_device->scm_state = SCM_CLOSED;
			peer_device->scm_state = SCM_W4_RFCOMM_DISCONNECT_INDICATION;
#if (HFP_MULTIPOINT_SUPPORT==1)
			//if (peer_device->evHFP_TxOK)
			//	pEventFree(peer_device->evHFP_TxOK);
			
			peer_device->evHFP_TxOK = 0;
#else
			if (evHFP_TxOK)
				pEventFree(evHFP_TxOK);
			
			evHFP_TxOK = 0;
#endif
        }
            //prh_mgr_releaseACL(peer_device->acl_handle, PRH_USER_ENDED_CONNECTION,NULL);//zhou siyou remove it20070722
		if(is_a2dp_connect())
			Handle_RFCOMM_Disconnect(&peer_device->address);//zhou siyou remove it20070722
		else
		Release_SLC_Flag=1;
	}
	else
	{
		status = BT_INVALIDPARAM;
	}

	return status;
}/*End of HF_Release_SLC()*/


/***********************************************************************
* FUNCTION:  HF_Accept_SLC
* DESCRIPTION:
*    Called by the application to accept and incoming SLC. This triggers 
*    the completion of an incoming RFCOMM connection for the SLC.
**********************************************************************/
int HF_Accept_SLC(t_bdaddr ag_bdaddr)
/**********************************************************************/
{
	
	int status;
	t_HFP_AG_Peer_Device* peer_device;

	
    //printf("\n HF_Accept_SCL Called by Application ");

	/* Ensure that an SLC does not exist already for this bdaddr */

	/* Call RFCOMM Establishment */
	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);
	if ((peer_device != 0) && (peer_device->scm_state == SCM_W4_USER_ACCEPT))
	{	
		peer_device->rf_role = RFCOMM_SERVER;
		peer_device->scm_state = SCM_W4_RFCOMM_CONNECT;
		//peer_device->versionInfo = BT_HFP_PROTOCOL_VERSION_105;
		//peer_device->active_profile = ACTIVE_PROFILE_HANDSFREE_AG;
		// @@@@  SET SLC TIMER HERE 
#if COMBINED_HOST==1
		peer_device->slc_setup_timer_handle = host_timer_create((unsigned int)PRH_HFP_SLC_SETUP_TIMEOUT,
													HF_SLC_Setup_Timeout,(void*)peer_device,pTIMER_ONESHOT);
#endif

		status=RSE_SrvConnectResponse(peer_device->address, (u_int8)peer_device->dlci, RSE_CONNECT_ACCEPT);

		if (status == BT_NOERROR && peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE)
		{
			/* Only use this command for 1.01 devices */
			if (peer_device->versionInfo == BT_HFP_PROTOCOL_VERSION_100 || 
				peer_device->versionInfo == BT_HFP_PROTOCOL_VERSION_105)
			{
				status = HF_Retrieve_Support_Features(&peer_device->address,peer_device->dlci);
				peer_device->scm_state = SCM_W4_SUPPORTED_FEATURES;
			}
			else
			{
				peer_device->scm_state = SCM_W4_INDICATOR_CONTROL;
				status = HF_Retrieve_Supported_Indicators(&peer_device->address,peer_device->dlci);
			}
		}

	} 
	else
	{
		status = BT_INVALIDPARAM;
	}
	
	return status;

}/*End of HF_Accept_SLC()*/


int HF_Accept_And_Retrieve_Support_Features(t_bdaddr ag_bdaddr)
{
    int status=0;
    t_HFP_AG_Peer_Device* peer_device;
    peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);
    if ((peer_device != 0) && (peer_device->scm_state == SCM_W4_RFCOMM_CONNECT))
    {
        /* Only use this command for 1.01 devices */
        if (peer_device->versionInfo == BT_HFP_PROTOCOL_VERSION_100 || 
        	peer_device->versionInfo == BT_HFP_PROTOCOL_VERSION_105)
        {
        	status = HF_Retrieve_Support_Features(&peer_device->address,peer_device->dlci);
        	peer_device->scm_state = SCM_W4_SUPPORTED_FEATURES;
        }
        else
        {
        	peer_device->scm_state = SCM_W4_INDICATOR_CONTROL;
        	status = HF_Retrieve_Supported_Indicators(&peer_device->address,peer_device->dlci);
        }
     }
    return status;
}


/***********************************************************************
* FUNCTION:  HF_Reject_SLC
* DESCRIPTION:
*    Called by the application to reject and incoming SLC. This triggers 
*    the refusal of an incoming RFCOMM connection for the SLC.
**********************************************************************/
int HF_Reject_SLC(t_bdaddr ag_bdaddr)
/**********************************************************************/
{
	int status= BT_NOERROR;
	t_HFP_AG_Peer_Device* peer_device;
	
	/* Ensure that an SLC does not exist already for this bdaddr */

	/* Call RFCOMM Establishment */
    //printf("\n HF_Reject_SCL Called by Application ");

	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);
	if ((peer_device != 0) && (peer_device->scm_state == SCM_W4_USER_ACCEPT))
	{
		Free_Audio_GW(&peer_device->address);
        status=RSE_SrvConnectResponse(peer_device->address, (u_int8)peer_device->dlci, RSE_CONNECT_REFUSE);
    } 
	else
	{
		status = BT_INVALIDPARAM;
	}
	return status;

}/*End of HF_Reject_SLC()*/


/***********************************************************************
* FUNCTION:  HF_Call_Request
* DESCRIPTION:
*    Initiates the establishment of an outgoing call from the HF.
*    Three different calling Types are supported.
*                HF_CALL_TYPE_NORMAL
* 				 HF_CALL_TYPE_MEM
*                HF_CALL_TYPE_REDIAL
**********************************************************************/	
int HF_Call_Request(t_bdaddr ag_bdaddr, u_int8  call_type, char* string)
/**********************************************************************/
{

	int status = BT_NOERROR;
	t_HFP_AG_Peer_Device* peer_device;


	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);

	if (peer_device==0)
	{
		//printf("\n No Peer Device \n");
		return BT_INVALIDPARAM;
    }

	if ((!peer_device) || (peer_device->scm_state != SCM_OPEN))
	{
		//printf("\n Peer Device not in open state \n");
		return BT_INVALIDPARAM;
	}

	//printf("\n  In Call Request : Active Profile = %x\n",peer_device->active_profile);
#if 1
	if ((peer_device->active_profile == ACTIVE_PROFILE_HEADSET_AG) 
		||(peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE_AG))
	{
		if ((call_type==HF_CALL_TYPE_NORMAL) 
		 || (call_type == HF_CALL_TYPE_MEM) 
		 || (call_type==HF_CALL_TYPE_REDIAL))
		{
			status = HF_Send_Call_Setup(ag_bdaddr, call_type, string, 0 /* Deprecated */);
		}
		else
		{
			return BT_INVALIDPARAM;
		}
		
		peer_device->call_state = HFP_CALL_W4_ADT_OK;
	}
	else
	{
		
	}
#else
	if ((peer_device->active_profile == ACTIVE_PROFILE_HEADSET_AG) ||
		(peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE_AG))
	{

		/* For the Headset Profile we initiate the ringing so we
		 * periodically send a RING command until we get a response
		 * from the headset or a Call Release/Reject from the API.
		 * This is done via a time which is cancelled on any of the 
		 * exit condictions.
		 */
	     //printf("\n  In inner loop : Active Profile = %x\n",peer_device->active_profile);

		 if (peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE_AG)
		 {
             /* If Handsfree AG we send a CallSetup indicator prior to the first
			  * RING. This has the format "+CIEV:(callsetup=1)"
			  */

			 //printf("\n Sending CALL SETUP INDICATOR \n");
			 peer_device->call_state = HFP_CALL_ALERTING;
			 
		     // Test to see if this indictor is supported by Jabara 
	    
			 HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,1,&peer_device->address,peer_device->dlci);

			 // Initiate Audio Connection. Once the Audio Link is established 
			 // we initiate Ringing.

			 if (peer_device->inband_tone == 1)
			 {
				HF_Audio_Setup(peer_device->address);
			 }
			 else
			 {
				 
				 if ((peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE_AG) &&
					 ( peer_device->inband_tone == 0))
					 
				 {
					 HF_Send_RING(&peer_device->address,peer_device->dlci);
					 
					 //printf("\n Sending RING Indicator \n");
#if COMBINED_HOST==1
					 peer_device->ring_timer = host_timer_create((unsigned int)PRH_HFP_RING_TIMEOUT,
						 HF_Ring_Timeout,(void*)peer_device,pTIMER_ONESHOT);
#endif
				 }
				 

			 }


		 }

	}
	else
	{
		/******************************************************/
		/*  Check the call type of and parameter ranges       */
		/******************************************************/
		
		//printf("\n In Wrong Call setup code \n");
		if ((call_type==HF_CALL_TYPE_NORMAL) || (call_type == HF_CALL_TYPE_MEM) || 
			(call_type==HF_CALL_TYPE_REDIAL))
		{
			status = HF_Send_Call_Setup(ag_bdaddr, call_type, string, 0 /* Deprecated */);
		}
		else
		{
			return BT_INVALIDPARAM;
		}
		
		peer_device->call_state = HFP_CALL_W4_ADT_OK;
		
	}
#endif	
	return status;

}/*End of HF_Call_Request()*/


/***********************************************************************
* FUNCTION:  HF_Call_Accept
* DESCRIPTION:
*    Called by the Application to accept an incoming call from the AG.
**********************************************************************/
int HF_Call_Accept(t_bdaddr ag_bdaddr )
/**********************************************************************/
{
	t_HFP_AG_Peer_Device* peer_device;
	int status=BT_NOERROR;
	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);

	if (peer_device)
	{
		
		if (peer_device->call_state == HFP_CALL_W4_USER_ACCEPT)
		{
			peer_device->call_state = HFP_W4_PEER_CALL_CONNECT;
			status = HF_Send_Answer(&peer_device->address,peer_device->dlci);
		}
		else
		{
			status = BT_INVALIDPARAM;
		}
	}
	else
	{
		return BT_INVALIDPARAM;
	}

	return status;

}/*End of HF_Call_Accept()*/


/***********************************************************************
* FUNCTION:  HF_Call_Reject
* DESCRIPTION:
*    Called by the Application to reject an incoming call from the AG.
**********************************************************************/
int HF_Call_Reject (t_bdaddr ag_bdaddr )
/**********************************************************************/
{
	t_HFP_AG_Peer_Device* peer_device;
	int status = BT_NOERROR;
	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);

	if ((peer_device) && (peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE))
	{	
		if (peer_device->call_state == HFP_CALL_W4_USER_ACCEPT)
		{
			peer_device->call_state = HFP_LOCAL_USER_INIT_CALL_REJECT;
			status = HF_Send_Reject(&peer_device->address,peer_device->dlci);
		}
		else
		{
			status = BT_INVALIDPARAM;
		}
	}
	else if ((peer_device) && (peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE_AG))
	{
		if (peer_device->call_state == HFP_CALL_ACTIVE)
		{
			HF_Send_Indicators(HFP_CALL_INDICATOR,0,&peer_device->address,peer_device->dlci);
			peer_device->call_state = HFP_CALL_CLOSED;
			HF_Audio_Release(peer_device->address);

		}
		else if (peer_device->call_state != HFP_CALL_CLOSED)
		{
			if ((peer_device->call_state == HFP_CALL_ALERTING) && (peer_device->ring_timer))
			{
#if COMBINED_HOST==1
				host_timer_cancel(peer_device->ring_timer);
#endif
				peer_device->ring_timer = 0;
			}
			
			HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,0,&peer_device->address,peer_device->dlci);
			peer_device->call_state = HFP_CALL_CLOSED;
		}

	}
	else
	{
		return BT_INVALIDPARAM;
	}

	return status;

}/*End of HF_Call_Reject()*/


/***********************************************************************
* FUNCTION:  HF_Call_Release
* DESCRIPTION:
*    Called by the application to relase an active call.
**********************************************************************/
int HF_Call_Release (t_bdaddr ag_bdaddr  )
/**********************************************************************/
{
	int status=BT_NOERROR;
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);

	if ((peer_device) && (peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE))
	{
		peer_device->call_state = HFP_LOCAL_USER_INIT_CALL_RELEASE;
        status = HF_Send_Reject(&peer_device->address,peer_device->dlci);
	}
	else if ((peer_device) && (peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE_AG))
	{
		if (peer_device->call_state == HFP_CALL_ACTIVE)
		{
			HF_Send_Indicators(HFP_CALL_INDICATOR,0,&peer_device->address,peer_device->dlci);
			peer_device->call_state = HFP_CALL_CLOSED;
			HF_Audio_Release(peer_device->address);
		}
		else if (peer_device->call_state != HFP_CALL_CLOSED)
		{
			if ((peer_device->call_state == HFP_CALL_ALERTING) && (peer_device->ring_timer))
			{
#if COMBINED_HOST==1
				host_timer_cancel(peer_device->ring_timer);
#endif
				peer_device->ring_timer = 0;
			}

			HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,0,&peer_device->address,peer_device->dlci);
			peer_device->call_state = HFP_CALL_CLOSED;
		}

	}
	else
	{
		return BT_INVALIDPARAM;
	}
	return status;

}/*End of HF_Call_Release()*/


/***********************************************************************
* FUNCTION:  HF_Multiparty_Call_Handling
* DESCRIPTION:
*     Called by the application, to instruct the AG how to handle an
*     incoming call which is waiting.
**********************************************************************/
int HF_Multiparty_Call_Handling(t_bdaddr ag_bdaddr,  u_int8 action )
/**********************************************************************/
{
	t_HFP_AG_Peer_Device* peer_device;
	int status = BT_NOERROR;
	char AT_CHLD_Command[12] = "AT+CHLD= \r";
	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);

	
	if (action <= 4)
		AT_CHLD_Command[8] = (char)(action + '0');
	else
	{
		return BT_INVALIDPARAM;
	}
	
	if (peer_device)
	{
		peer_device->operation_pending = AT_CHLD;
		return HF_SendToGateway((u_int8*)AT_CHLD_Command,10,&ag_bdaddr,peer_device->dlci);
	}
	else
	{
		return BT_INVALIDPARAM;
	}

	return status;

}/*End of HF_Multiparty_Call_Handling()*/

/***********************************************************************
* FUNCTION:  HF_Audio_Setup
* DESCRIPTION:
*    Initiates the setup of a SCO link
**********************************************************************/
int HF_Audio_Setup(t_bdaddr ag_bdaddr)
/**********************************************************************/
{
	int status = BT_NOERROR;
	t_HFP_AG_Peer_Device* peer_device;
#if COMBINED_HOST==1    
	u_int16 voice_setting=0x60;//g_LM_config_info.default_voice_setting;  //???????????????
#endif

#if COMBINED_HOST==1
	t_bd_addr bdaddr;	
	pMemcpy(bdaddr.bytes,ag_bdaddr.bytes,sizeof(ag_bdaddr.bytes));
#endif

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Audio_Setup "));
#endif    

	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);
	
	// Advanced Audio setup mechanism
	//  
	// If SCO Open establish Audio immediately
	// If peer_device exists but no SLC, then setup SLC and then Audio.
	// If no peer device exists then setup do ServiceDiscover, SLC setup and Then Audio.
	//
#if pDEBUG  
     if(peer_device)
     	{
       pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"==HF_Audio_Setup peer_device->acl_state=%d, audio_state=%d\n",peer_device->acl_state,peer_device->audio_state));
	pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"==HF_Audio_Setup peer_device->scm_state=%d\n",peer_device->scm_state));
     	}	
#endif 
	if ((peer_device) && (peer_device->scm_state == SCM_OPEN))
	{
        
   
#if 0 // Disabled for now as this introduces problem with Exit Sniff
		// Ensure that we are not allowed to setup an audio link when one exists already
		if (peer_device->audio_state != HFP_AUDIO_CLOSED)
			return BT_DISALLOWED;
#endif
		if (peer_device->acl_state == HFP_DEVACTIVE)
		{
    			peer_device->audio_state = HFP_AUDIO_SETUP_IN_PROGRESS;
#if COMBINED_HOST==1
			t_lmp_link *p_link;
			p_link= LMaclctr_Find_Peer_Bd_Addr(&bdaddr);
			
			/* Open SCO link directly */
			LMconnection_LM_Add_SCO_Connection(p_link, 0x80/*HV3*/, voice_setting);
#else
			status = MGR_AddSCO(peer_device->acl_handle, &peer_device->sco_handle, 0x80, HF_SCO_Setup_Mgr_Callback);
#endif
		}
		else if ((peer_device->acl_state == HFP_DEVSNIFF) || (peer_device->acl_state == HFP_DEVPENDINGSNIFF))
		{
			/* in Sniff more or entering Sniff mode, call exit sniff mode and only initiate SCO
			 * once the device has successfully exited Sniff mode
			 */
			peer_device->audio_state = HFP_AUDIO_W4_EXIT_SNIFF;
			#if pDEBUG
			pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Exit_Sniff_Mode be called in hf_audio_setup\n"));
			#endif 
			HF_Exit_Sniff_Mode(peer_device);

		}
		else if(peer_device->acl_state == HFP_DEVPENDINGEXITSNIFF)//syzhou added
			peer_device->audio_state = HFP_AUDIO_W4_EXIT_SNIFF;//wait hci event callback status change
			

	}
	else if ((peer_device) && (peer_device->scm_state == SCM_CLOSED))
	{
		peer_device->audio_state = HFP_AUDIO_SETUP_PENDING;
		status = HF_Setup_SLC(ag_bdaddr);
	}

	return status;

}/*End of HF_Audio_Setup()*/

/***********************************************************************
* FUNCTION:  HF_SCO_Setup_Mgr_Callback
* DESCRIPTION:
*    Called back my the Mgr layer when a SCO setup is complete
**********************************************************************/
#if COMBINED_HOST==0
void HF_SCO_Setup_Mgr_Callback(u_int16 *scoHandle, t_api status/*, t_bdaddr *bdAddress*/)
{
	t_HFP_AG_Peer_Device* peer_device;

	//printf("\n\r Got Mgr SCO Callback with Status = %x", status);
	peer_device = Find_Audio_GW_With_Audio_Setup_In_Progress();

#if pDEBUG
     if(peer_device)
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_SCO_Setup_Mgr_Callback peer_device->scm_state=%d,status=%d",peer_device->scm_state,status));
#endif 
	if ((peer_device) && (peer_device->scm_state == SCM_OPEN))
	{
		if (status != BT_NOERROR)
		{
			peer_device->sco_handle = 0;
		}
		else
		{
			peer_device->audio_state = HFP_AUDIO_OPEN;
		}
		
		/* Generate Callback - to the app */
		if (HF_callbacks.HF_CB_Audio_Link_Established)
			(*HF_callbacks.HF_CB_Audio_Link_Established)(peer_device->address,status);

		peer_device->inband_tone = 0;    // added by cliff   03/20/2009
		if ((peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE_AG) &&
			(  (peer_device->inband_tone == 1) ))

		{
			HF_Send_RING(&peer_device->address,peer_device->dlci);
                    peer_device->ring_timer = pTimerCreate((unsigned int)PRH_HFP_RING_TIMEOUT,
											HF_Ring_Timeout,(void*)peer_device,pTIMER_ONESHOT);
			
#if COMBINED_HOST==1			
			peer_device->ring_timer = host_timer_create((unsigned int)PRH_HFP_RING_TIMEOUT,
											HF_Ring_Timeout,(void*)peer_device,pTIMER_ONESHOT);
#endif
		}
	}


}
#endif

/***********************************************************************
* FUNCTION:  HF_Audio_Release
* DESCRIPTION:
*    Initiates the release of a SCO link
**********************************************************************/
int HF_Audio_Release(t_bdaddr ag_bdaddr)
/**********************************************************************/
{
	int status;
	t_HFP_AG_Peer_Device* peer_device;
#if COMBINED_HOST==1
	t_connectionHandle connection_handle;
#endif

	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);
#if pDEBUG
    if(peer_device)
    {
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Audio_Release peer_device->audio_state=%d",peer_device->audio_state));
    }
#endif 
	
#if 0 // New Way
	if ((peer_device) && ((peer_device->audio_state == HFP_AUDIO_OPEN) /*||
		                  (peer_device->audio_state == HFP_REJECTING_AUDIO_W4_CONNECTION_COMPLETE)*/))
#else
	if ((peer_device) && ((peer_device->audio_state == HFP_AUDIO_OPEN) ||
		                  (peer_device->audio_state == HFP_REJECTING_AUDIO_W4_CONNECTION_COMPLETE)))
#endif
	{
		/* Call the manager to release the SCO link */
#if COMBINED_HOST==0
		status = MGR_ReleaseSCO(peer_device->sco_handle,HCI_errTERMUSER, HF_SCO_Release_Mgr_Callback);
#else
		connection_handle = peer_device->sco_handle;
		LMconnection_LM_Disconnect_SCO( LMscoctr_Find_SCO_By_Connection_Handle(connection_handle), LOCAL_TERMINATED_CONNECTION);
#endif
	}
	else
	{
		status = BT_INVALIDPARAM;
	}

	return status;

}/*End of HF_Audio_Release()*/

/***********************************************************************
* FUNCTION:  HF_Audio_Accept
* DESCRIPTION:
*    Called by the user to accept an incoming Audio Link.
**********************************************************************/

int HF_Audio_Accept(t_bdaddr ag_bdaddr, u_int8 packet_type)
{
	t_HFP_AG_Peer_Device* peer_device;
	u_int8 status;
#if COMBINED_HOST==1    
	u_int16 voice_setting=0x60;//g_LM_config_info.default_voice_setting;  //???????????????
#endif

#if COMBINED_HOST==1
	t_bd_addr bdaddr;
	pMemcpy(bdaddr.bytes,ag_bdaddr.bytes,sizeof(ag_bdaddr.bytes));
#endif

	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);

	if ((peer_device) && (( peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE) ||(peer_device->active_profile == ACTIVE_PROFILE_HEADSET)) )
	{
#if COMBINED_HOST==0
		//HCI_AcceptSyncronousConnection(ag_bdaddr, packet_type);  // tianwq
		return HCI_AcceptConnectionRequest(ag_bdaddr,0x00); /* role switch argument ignored in SCO case */      
#else
		return LMconnection_LM_Connection_Accept(&bdaddr, 0x00, voice_setting);
#endif
	}

	else if ((peer_device) && ((peer_device->active_profile == ACTIVE_PROFILE_HEADSET_AG)
		||(peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE_AG) ))
	{
		if (peer_device->acl_state == HFP_DEVACTIVE)
		{
#if COMBINED_HOST==1
			t_lmp_link *p_link;
			p_link= LMaclctr_Find_Peer_Bd_Addr(&bdaddr);
			/* Open SCO link directly */
			peer_device->audio_state = HFP_AUDIO_SETUP_IN_PROGRESS;
			//printf(" /n/r Calling Mgr Add SCO /n");
			LMconnection_LM_Add_SCO_Connection(p_link, 0x80/*HV3*/, voice_setting);
#else
			status = MGR_AddSCO(peer_device->acl_handle, &peer_device->sco_handle, 0x07, HF_SCO_Setup_Mgr_Callback);
#endif
		}
		else if ((peer_device->acl_state == HFP_DEVSNIFF) || (peer_device->acl_state == HFP_DEVPENDINGSNIFF))
		{
			/* in Sniff more or entering Sniff mode, call exit sniff mode and only initiate SCO
			* once the device has successfully exited Sniff mode
			*/
			peer_device->audio_state = HFP_AUDIO_W4_EXIT_SNIFF;
			HF_Exit_Sniff_Mode(peer_device);
		}	
	}

	return BT_NOERROR;
}

/***********************************************************************
* FUNCTION:  HF_Audio_Reject
* DESCRIPTION:
*    Called by the user to reject an incoming Audio Link.
**********************************************************************/

int HF_Audio_Reject(t_bdaddr ag_bdaddr)
{
#if COMBINED_HOST==1
	u_int16 voice_setting=0x60;//g_LM_config_info.default_voice_setting;  //???????????????
#endif	

#if COMBINED_HOST==1
	t_bd_addr bdaddr;
#endif

	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);


	if ((peer_device))
	{
#if 0// To overcome issues with the Audio links from AGs we accept the SCO and 
		// On the ConnectionComplete we Disconnect the SCO.
		HCI_RejectConnectionRequest(ag_bdaddr,0x0D/* Host rejects */);
#else
		peer_device->audio_state = HFP_REJECTING_AUDIO_W4_CONNECTION_COMPLETE;
#if COMBINED_HOST==0
		return HCI_AcceptConnectionRequest(ag_bdaddr,0x00);
#else
		pMemcpy(bdaddr.bytes,ag_bdaddr.bytes,sizeof(ag_bdaddr.bytes));
		return LMconnection_LM_Connection_Accept(&bdaddr, 0x00, voice_setting);
#endif
#endif
	}
	return BT_NOERROR; 


}


/***********************************************************************
* FUNCTION:  HF_SCO_Release_Mgr_Callback
* DESCRIPTION:
*    Called back my the Mgr layer when a SCO release is complete
**********************************************************************/
#if COMBINED_HOST==0
void HF_SCO_Release_Mgr_Callback(u_int16 scoHandle, t_api status)
{

	t_HFP_AG_Peer_Device* peer_device;
	u_int8 prev_audio_state;

	peer_device = Find_Audio_GW_By_SCO_Handle(scoHandle);
	
	if ((peer_device) && ((peer_device->audio_state == HFP_AUDIO_OPEN) ||
		                  (peer_device->audio_state == HFP_REJECTING_AUDIO_W4_CONNECTION_COMPLETE)))
	{

		/***************************************************
		 *
		 *  Set a timer to enter sniff after the SCO release
		 ***************************************************/

		/* Clear any pre-existing Low Power mode Timer */
		if(peer_device->enter_sniff_mode_timer_handle)
		{
#if COMBINED_HOST==1
			host_timer_cancel(peer_device->enter_sniff_mode_timer_handle);
#endif
			peer_device->enter_sniff_mode_timer_handle = 0;
		}
		
#ifndef TEST_NO_SNIFF
		/* Set a new low power mode timer */
#if COMBINED_HOST==1
		peer_device->enter_sniff_mode_timer_handle = host_timer_create((unsigned int)PRH_HFP_SNIFFMODE_TIMEOUT,
			HF_Enter_Sniff_Mode,(void*)peer_device,pTIMER_ONESHOT);
#endif
#endif
		prev_audio_state =peer_device->audio_state;
		if (status == BT_NOERROR)
		{
			peer_device->audio_state = HFP_AUDIO_CLOSED;
			peer_device->sco_handle = 0;
		}
#if 0 // NEW WAY
	//	if (prev_audio_state != HFP_REJECTING_AUDIO_W4_CONNECTION_COMPLETE)
#else
		if (prev_audio_state != HFP_REJECTING_AUDIO_W4_CONNECTION_COMPLETE)
#endif
		{
			/* Generate Callback - to the app */
#if COMBINED_HOST==0
			if (HF_callbacks.HF_CB_Audio_Link_Released)
				(*HF_callbacks.HF_CB_Audio_Link_Released)(peer_device->address,status);
#endif
		}

	}
}
#endif

/***********************************************************************
* FUNCTION:  HFP_hciEventCallback
* DESCRIPTION:
*    This handles HCI Events. For the purposes of the HFP the two most
*    critical HCI Events are :
*        HCI_evCONNECTIONCOMPLETE
*        HCI_evDISCONNECTIONCOMPLETE
*
*    These events indicate the addition or release of a SCO by the peer
**********************************************************************/
#define HCI_evSYNCHRONOUSCONNECTIONCOMPLETE         0x2C
void HFP_hciEventCallback(u_int8 *eventPdu) 
{
#if COMBINED_HOST==0
	u_int8 eventType=eventPdu[0];
	t_HFP_AG_Peer_Device* peer_device;
	u_int8 status=0;
	t_bdaddr ag_bdaddr;
	u_int16 linkType;
	u_int16 sco_handle;


    if(gCurrent_Active_Profile !=ACTIVE_PROFILE_HANDSFREE_AG)
        return;
	#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HFP_hciEventCallback eventType=0x%x,status=0x%x, gCurrent_Active_Profile=%x\n",eventType,eventPdu[2],gCurrent_Active_Profile));
#endif  
    switch (eventType) 
	{
		
	case HCI_evSYNCHRONOUSCONNECTIONCOMPLETE:
	case HCI_evCONNECTIONCOMPLETE:
		{
			linkType=eventPdu[11];
			status=eventPdu[2];
            
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HFP_hciEventCallback linkType=0x%x, status=%d\n",linkType,status));
#endif  
            
			if ((linkType==0x00) && (status==0x00))
			{
				pMemcpy(&ag_bdaddr.bytes,eventPdu+5,6);
				sco_handle=eventPdu[3]|(eventPdu[4]<<8);
				/* Find the peer_device */
				peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);
                
#if pDEBUG
    if(peer_device)
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HFP_hciEventCallback audio_state=0x%x\n",peer_device->audio_state));
#endif
                            if ((peer_device) && (peer_device->audio_state == HFP_AUDIO_CLOSED))
				{
					peer_device->sco_handle = sco_handle;
					peer_device->audio_state = HFP_AUDIO_OPEN;
					/* Generate Callback - to the app */
					if (HF_callbacks.HF_CB_Audio_Link_Established)
						(*HF_callbacks.HF_CB_Audio_Link_Established)(peer_device->address,status);
					
				}
#if 1 // OLD WAY NOW REDUNDANT
				if ((peer_device) && (peer_device->audio_state == HFP_REJECTING_AUDIO_W4_CONNECTION_COMPLETE))
				{
					/* Disconnect the SCO */
					peer_device->sco_handle = sco_handle;
        			        HF_Audio_Release(peer_device->address);				
				}
#endif

			}
			else if ((linkType==0x00) && (status!=0x00))
			{
#if 0 // NEW WAY
                // This condiction can be entered if we rejected the SCO setyp

				peer_device = Find_Audio_GW_Rejecting_SCO();
				if (peer_device)
				{
					peer_device->audio_state = HFP_AUDIO_CLOSED;
					if (HF_callbacks.HF_CB_Audio_Link_Released)
						(*HF_callbacks.HF_CB_Audio_Link_Released)(peer_device->address,status);
				}

		//		peer_device->audio_state = HFP_REJECTING_AUDIO_W4_CONNECTION_COMPLETE;
#endif
			}
		}
		break;
		

	case HCI_evDISCONNECTIONCOMPLETE:
		{
			u_int8 status_t=0;
			u_int16 handle=eventPdu[3]|(eventPdu[4]<<8);
			status_t=eventPdu[2];
			peer_device = Find_Audio_GW_By_SCO_Handle(handle);

			if (peer_device)
			{
				HF_SCO_Release_Mgr_Callback(handle,status_t);
			}
			else
			{   
#if 1//zhou siyou open 20070722
                            // Catch all case to ensure clean-up if the AG release the ACL
                            // Without releasing the RFCOMM or L2CAP channels.
                            // Not observed in any of the AGs tested to date.

				peer_device = Find_Audio_GW_By_ACL_Handle(handle);
				if(peer_device )
    					pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HCI_evDISCONNECTIONCOMPLETE peer_device->scm_state=%d\n",peer_device->scm_state));

				if ((peer_device) && (peer_device->scm_state != SCM_CLOSED)&&Release_SLC_Flag)
				{
					Handle_RFCOMM_Disconnect(&peer_device->address);//zhou siyou 090930
					Release_SLC_Flag=0;
				}
#endif
			}

		}
		break;


	case HCI_evCONNECTIONREQUEST :
//        printf("\n\rHFP: Debug: In SCO or ACL  Connection Request.");
		if (eventPdu[11]==0x00 /* SCO Link */)
		{
			t_bdaddr temp;
			u_int32 classOfDevice;

		    pMemcpy(temp.bytes,eventPdu+2,6);
			peer_device = Find_Audio_GW_By_Address(&temp);

			if ((peer_device) && (peer_device->scm_state == SCM_OPEN))
			{
				classOfDevice = eventPdu[8]|(eventPdu[9]<<8)|(eventPdu[10]<<16);
                            if (classOfDevice | 0x2004008)
				{
				//	if (!g_SCO_AutoAccept)
					{
						/* Call Back the Application for incoming SCO Link */
						if (HF_callbacks.HF_CB_Audio_Request)
							(*HF_callbacks.HF_CB_Audio_Request)(peer_device->address);
					}
#if 0 // OLD Way Redundant
					else
					{
                                        HCI_AcceptConnectionRequest(peer_device->address,0x00);
					}
#endif
				}

				else
				{
#if HFP_CORE_DEBUG	
					printf("\n\r Invalid Class of Device in SCO Link Request \n");
#endif
				}

			}
			else
			{
				return;
			}
        }
		break;

	case HCI_evMODECHANGE :
		{
			u_int8 status_t=0;
			u_int16 handle=eventPdu[3]|(eventPdu[4]<<8);
			u_int8 mode = eventPdu[5];
			status_t=eventPdu[2];

			peer_device = Find_Audio_GW_By_ACL_Handle(handle);

			if (!peer_device)
				return;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HFP_hciEventCallback status:%d, mode:%d,audio_status:%d",status_t,mode,peer_device->audio_state));
#endif    

			if (status_t == BT_NOERROR)
			{
                            switch(mode)
        			{
        			case HFP_DEVACTIVE :
                        // Find the AG corresponding to the handle and change its state.
        				peer_device->acl_state = HFP_DEVACTIVE;
        				if (peer_device->audio_state == HFP_AUDIO_W4_EXIT_SNIFF)
        				{
                                        HF_Audio_Setup(peer_device->address);
        				}
        				break;

        			case HFP_DEVHOLD :
                        // Not used in the HFP - No Explict support
        				peer_device->acl_state = HFP_DEVHOLD;

        				break;

        			case HFP_DEVSNIFF :
                        // Main low Power mode for the HFP
                        // Find the AG corresponding to the handle and change its state.
                        peer_device->acl_state = HFP_DEVSNIFF;
        				break;

        			case HFP_DEVPARK :
                        // May be used by 0.96 devices.
                        // To be completed 
        				peer_device->acl_state = HFP_DEVPARK;

        				break;

        			}
        		}
			else // Failed Sniff or Low Power Mode
			{
				if (peer_device->acl_state == HFP_DEVPENDINGSNIFF)
				{
#if HFP_CORE_DEBUG	
					printf("\n\r Entry into Sniff mode failed :- Status = %x",status_t);
#endif
					peer_device->acl_state = HFP_DEVACTIVE;

				}
				else if (peer_device->acl_state == HFP_DEVPENDINGEXITSNIFF)
				{
#if HFP_CORE_DEBUG	
					printf("\n\r Exit from Sniff mode failed :- Status = %x",status_t);
#endif
					peer_device->acl_state = HFP_DEVSNIFF;

					if (peer_device->audio_state == HFP_AUDIO_W4_EXIT_SNIFF)
					{
					   /* Change State on Audio to Closed 
					    * and Callback the Application indicating Failure on the Audio.
					    * Also print message indicating we have hit this condiction
						*/
						peer_device->audio_state = HFP_AUDIO_CLOSED;
						if (HF_callbacks.HF_CB_Audio_Link_Established)
							(*HF_callbacks.HF_CB_Audio_Link_Established)(peer_device->address,status_t);
					}
	 

				}
			}

		}
		break;

	case HCI_evCOMMANDSTATUS:
		{
			u_int16 cmdOpcode = BT_READ_LE_UINT16(eventPdu+4);
			status=eventPdu[2];
			#if pDEBUG
    			pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HCI_evCOMMANDSTATUS cmdOpcode=0x%x,status=0x%x\n",cmdOpcode,status));
			#endif  
			switch(cmdOpcode)
			{
				
			case HFP_HCI_cmdSNIFFMODE:
		//	case HFP_HCI_cmdEXITSNIFFMODE:  NOT SUPPORTED YET ! Dont know what to do if exit sniff fails, can it ?
				if(HCI_errNOERROR != status)
				{
					peer_device = HFP_AG_Pending_Entry_Into_Sniff();
                                  if(peer_device)
					{
					if (peer_device->acl_state == HFP_DEVPENDINGSNIFF)
					{
#if HFP_CORE_DEBUG	
						printf("\n\r Entry into Sniff mode failed :- Status = %x",status);
#endif
						peer_device->acl_state = HFP_DEVACTIVE;
						
					}
					else if (peer_device->acl_state == HFP_DEVPENDINGEXITSNIFF)
					{
#if HFP_CORE_DEBUG	
						printf("\n\r Exit from Sniff mode failed :- Status = %x",status);
#endif
						peer_device->acl_state = HFP_DEVSNIFF;
						
						if (peer_device->audio_state == HFP_AUDIO_W4_EXIT_SNIFF)
						{
						/* Change State on Audio to Closed 
						* and Callback the Application indicating Failure on the Audio.
						* Also print message indicating we have hit this condiction
							*/
							peer_device->audio_state = HFP_AUDIO_CLOSED;
							if (HF_callbacks.HF_CB_Audio_Link_Established)
								(*HF_callbacks.HF_CB_Audio_Link_Established)(peer_device->address,status);
						}
					   }
					}
				}
               break;
           }
			break;
        }

		case HCI_evCOMMANDCOMPLETE:
		{
			u_int16 commandOpcode;
			u_int8 status_t;
			commandOpcode=eventPdu[3]|(eventPdu[4]<<8);
			status_t=eventPdu[5];
		}
		break;

    }
#endif
}


/***********************************************************************
* FUNCTION:  HF_Change_Volume_Setting
* DESCRIPTION:
*    TO BE COMPLETED 
**********************************************************************/
int HF_Change_Volume_Setting(t_bdaddr ag_bdaddr , u_int8 target, u_int8 volume_setting )
/**********************************************************************/
{
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);

	if (peer_device)
	{
		if (volume_setting <= 15)
		{
			if (peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE)
			{
				if (target == HFP_SPEAKER)
				{
					HF_Report_Speaker_Gain_To_AG(&ag_bdaddr,peer_device->dlci,volume_setting);	
				}
				else if (target == HFP_MICROPHONE)
				{
					HF_Report_Mic_Gain_To_AG(&ag_bdaddr,peer_device->dlci,volume_setting);	
				}
				else
					return BT_INVALIDPARAM;
			}
			else if (peer_device->active_profile == ACTIVE_PROFILE_HEADSET_AG
				||(peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE_AG) )
			{
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Change_Volume_Setting target=%d, gain=%d,dlci=%d\n",target,volume_setting, peer_device->dlci));
#endif
				if (target == HFP_SPEAKER)
				{
					HF_Set_Speaker_Gain_To_HS(&ag_bdaddr,peer_device->dlci,volume_setting);		
				}
				else if (target == HFP_MICROPHONE)
				{
					HF_Set_Mic_Gain_To_HS(&ag_bdaddr,peer_device->dlci,volume_setting);	
				}
				else
					return BT_INVALIDPARAM;

			}
			return BT_NOERROR;
		}
		else
			return BT_INVALIDPARAM;
	}
	else
		return BT_INVALIDPARAM;


}/*End of HF_Change_Volume_Setting()*/


/***********************************************************************
* FUNCTION:  HF_Modify_Remote_Supplementary_Feature
* DESCRIPTION:
*    Called by the application to modify the setting of one of the 
*    supplementary features in the AG.
*    
**********************************************************************/

int HF_Modify_Remote_Supplementary_Feature(t_bdaddr ag_bdaddr,u_int8 feature_id , u_int8 enable)
{
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);

	if (peer_device)
	{
		switch(feature_id)
		{
		case 1 : // Call Waiting
			if (enable==1)
			{
				HF_Enable_Called_Waiting(&peer_device->address, peer_device->dlci);
			}
			else if (enable==0)
			{
				HF_Disable_Call_Waiting(&peer_device->address, peer_device->dlci);
			}
			break;

		case 2 : // Calling Line Id
			if (enable==1)
			{
				HF_Activate_CLIP(&peer_device->address, peer_device->dlci);
			}
			else if (enable==0)
			{
				HF_DeActivate_CLIP(&peer_device->address, peer_device->dlci);
			}
			break;

		case 4 : // Echo Cancellation 
			if (enable == 0)
			{
				HF_DeActivate_NR_EC(&peer_device->address, peer_device->dlci);
			}
			// Not possible for the HF to activate this facility in the AG as 
			// it is activated by default.
			break;


		case 8 : // Voice Recognition
			if (enable == 1)
			{
				HF_Activate_Voice_Recognition(&peer_device->address, peer_device->dlci);
			}
			else if (enable==0)
			{
				HF_DeActivate_Voice_Recognition(&peer_device->address, peer_device->dlci);
			}
			break;


		default:
			break;
		}

	}
	return BT_NOERROR;
}/*End of HF_Modify_Remote_Supplementary_Feature()*/


/***********************************************************************
* FUNCTION:  HF_Read_Remote_Supplementary_Feature
* DESCRIPTION:
*    Originated:
**********************************************************************/
int HF_Read_Remote_Supplementary_Feature(t_bdaddr ag_bdaddr)
/**********************************************************************/
{
      /*!!!!!!!!!!!  TO BE SUPPORTED !!!!!!!!!!!!!!!!!!! */
	return BT_NOERROR;
}/*End of HF_Read_Remote_Supplementary_Feature()*/


/***********************************************************************
* FUNCTION:  HF_Send_DTMF
* DESCRIPTION:
*    Called by the application to send a DTMF string form the HF to the 
*    AG. A sting of up to 20 dtmf chars is supported.
**********************************************************************/
int HF_Send_DTMF(t_bdaddr ag_bdaddr, char* dtmf_sequence )
/**********************************************************************/
{
	t_HFP_AG_Peer_Device* peer_device;
	int status = BT_NOERROR;
	g_hfp_dtmf_len = 0 ;
	g_hfp_dtmf_offset = 0;

	
	/* Ensure that an SLC does not exist already for this bdaddr */

	/* Call RFCOMM Establishment */
	
	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);
	if (peer_device != 0)
	{	        
		if (validate_and_store_dtmf_string(dtmf_sequence))
		{
			/* Send the first DTMF Character */ 
           	if (peer_device->scm_state == SCM_OPEN)
			{
				status = HF_Send_DTMF_Tone(&ag_bdaddr,peer_device->dlci,g_hfp_dtmf_string[g_hfp_dtmf_offset]);
			}
			else if (peer_device->scm_state == SCM_CLOSED)
			{
				/* Need to establish an SLC with the peer */
	
			}
		}
		else
		{
			status = BT_INVALIDPARAM;
		}
    }
	else
	{
		status = BT_INVALIDPARAM;
	}

	return status;
}/*End of HF_Send_DTMF()*/


/***********************************************************************
* FUNCTION:  HF_Read_Phone_Status_Information
* DESCRIPTION:
*    Originated:
**********************************************************************/
int HF_Read_Phone_Status_Information(t_bdaddr ag_bdaddr)
/**********************************************************************/
{
	t_HFP_AG_Peer_Device* peer_device;

	int phone_status=0;

	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);
	if (peer_device != 0)
	{
		/* 
		 * Bit  0  - Indicates the Service Status
		 * Bit  1  - Indicates the Call Status
		 * Bit 8,9 - Indicates the Call Setup Status
		 */

		phone_status |= (peer_device->service_ind);
		phone_status |= ((peer_device->call_ind) << 1);
		phone_status |= ((peer_device->call_setup_ind) << 8);

		return phone_status;
	}
	else
	{
		return 0;
	}

}/*End of HF_Read_Phone_Status_Information()*/

/***********************************************************************
* FUNCTION:  HF_Retrieve_Memory
* DESCRIPTION:
*    Solicits input from the AG. Usually a phone number stored in memory.
*    This is just an example of usage and pulls upto 20 numbers from the
*    AG.
**********************************************************************/

int HF_Retrieve_Memory(t_bdaddr ag_bdaddr,u_int8 num_mem_locs)
{
	t_HFP_AG_Peer_Device* peer_device;
	int status = BT_NOERROR;

	
	/* Ensure that an SLC does not exist already for this bdaddr */

	/* Call RFCOMM Establishment */
	
	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);
	if (peer_device != 0)
	{	        
		
		g_num_mem_locs = num_mem_locs;
		
		if (peer_device->scm_state == SCM_OPEN)
		{
			status = HF_Send_BINP(&ag_bdaddr,peer_device->dlci);
		}
		else if (peer_device->scm_state == SCM_CLOSED)
		{
			/* Need to establish an SLC with the peer */
			
		}
		
    }
	else
	{
		status = BT_INVALIDPARAM;
	}

	return status;
}

/**************************************************/
/*  AT COMMAND GENERATION FUNCTIONS               */
/*                                                */
/**************************************************/

/***********************************************************************
* FUNCTION:  HF_Enable_Called_Waiting
* DESCRIPTION:
*    Enables Call Waiting by sending AT+CCWA=1\r to the peer.
**********************************************************************/
int HF_Enable_Called_Waiting(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
   char CCWA_Command[12] = "AT+CCWA=1\r";
    t_HFP_AG_Peer_Device* peer_device;

   peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
   if(peer_device)
        peer_device->operation_pending = AT_CCWA_1;
 
   return HF_SendToGateway((u_int8*)CCWA_Command,10,gatewayBDAddress,gwDLCI);

}

/***********************************************************************
* FUNCTION:  HF_Disable_Called_Waiting
* DESCRIPTION:
*    Disables Call Waiting by sending AT+CCWA=0\r to the peer.
**********************************************************************/

int HF_Disable_Call_Waiting(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
    char CCWA_Command[12] = "AT+CCWA=0\r";
    t_HFP_AG_Peer_Device* peer_device;
   peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
   if(peer_device)
        peer_device->operation_pending = AT_CCWA_0;

   return HF_SendToGateway((u_int8*)CCWA_Command,10,gatewayBDAddress,gwDLCI);

}

/***********************************************************************
* FUNCTION:  HF_Activate_Indicators_Event_Reporting
* DESCRIPTION:
*    Acivates indicator event reporting by sending AT+CMER=3,0,0,1\r to the peer.
**********************************************************************/

int HF_Activate_Indicators_Event_Reporting(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
   char CMER_Command[17] = "AT+CMER=3,0,0,1\r";

   t_HFP_AG_Peer_Device* peer_device;

   peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
   if(peer_device)
        peer_device->operation_pending = AT_CMER;

   return HF_SendToGateway((u_int8*)CMER_Command,16,gatewayBDAddress,gwDLCI);

}
#if 0 // Currently not used
/***********************************************************************
* FUNCTION:  HF_Deactivate_Indicators_Event_Reporting
* DESCRIPTION:
*    Deactivates indicator event reporting by sending AT+CMER=3,0,0,0\r to 
*    the peer.
**********************************************************************/

int HF_Deactivate_Indicators_Event_Reporting(t_bdaddr *gatewayBDAddress, u_int16 dlci)
{
   char CMER_Command[17] = "AT+CMER=3,0,0,0\r";
   t_HFP_AG_Peer_Device* peer_device;
   peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
   peer_device->operation_pending = AT_CMER;

   return HF_SendToGateway((u_int8*)CMER_Command,16,gatewayBDAddress,dlci);
}

#endif

/***********************************************************************
* FUNCTION: HF_Activate_Voice_Recognition
* DESCRIPTION:
*    Activates voice recognition in the AG, by sending a ."AT+BRVA=1\r"
*    to the AG. This results in the AG setting up a SCO to the Hands Free.
**********************************************************************/

int HF_Activate_Voice_Recognition(t_bdaddr *gatewayBDAddress, u_int16 dlci)
{
   char BRVA_Command[11]="AT+BRVA=1\r";
    t_HFP_AG_Peer_Device* peer_device;
   peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
   if(peer_device)
        peer_device->operation_pending = AT_BRVA_1;

   return HF_SendToGateway((u_int8*)BRVA_Command,10,gatewayBDAddress,dlci);
}

/***********************************************************************
* FUNCTION: HF_DeActivate_Voice_Recognition
* DESCRIPTION:
*    Deactivates voice recognition in the AG, by sending a ."AT+BRVA=0\r"
*    to the AG. 
**********************************************************************/

int HF_DeActivate_Voice_Recognition(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
   char *BRVA_Command;
    t_HFP_AG_Peer_Device* peer_device;
   peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
   if(peer_device)
        peer_device->operation_pending = AT_BRVA_0;
   BRVA_Command ="AT+BRVA=0\r";

   return HF_SendToGateway((u_int8*)BRVA_Command,10,gatewayBDAddress,gwDLCI);
}
/***********************************************************************
* FUNCTION: HF_Retrieve_Support_Features
* DESCRIPTION:
*    Initiates feauture exchange with the AG. Results in "AT+BRSF=nnn\r"
*    being sent to the AG. 
**********************************************************************/
int HF_Retrieve_Support_Features(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
	t_HFP_AG_Peer_Device* peer_device;
	char BRSF_Command[16] = "AT+BRSF=";
	char num=0;
	int len =0;

	peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
        if(peer_device)
            peer_device->operation_pending = AT_BRSF;

	/* Create the AT cmd we're sending */
	//HFP_BT_WRITE_LE_UINT32((u_int8*)BRSF_Command[8],HF_UNIT_SUPPORTED_FEATURES_U_INT16);
	//sprintf(BRSF_Command, "AT+BRSF=%d\r", HF_UNIT_SUPPORTED_FEATURES_U_INT16);
	num = HF_UNIT_SUPPORTED_FEATURES_U_INT16/10;
	if (num != 0)
	{
		BRSF_Command[8] = '0'+num; 
		BRSF_Command[9] = '0'+HF_UNIT_SUPPORTED_FEATURES_U_INT16 -10*num;
		BRSF_Command[10] = '\r'; 
		len = 11;
	}
	else
	{
		BRSF_Command[8] = '0' + HF_UNIT_SUPPORTED_FEATURES_U_INT16;
		BRSF_Command[9] = '\r';
		len =10;
	}

	return HF_SendToGateway((u_int8*)BRSF_Command, len, gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_Respond_Support_Features
* DESCRIPTION:
*    Responds to a request for Bluetooth features from the peer by sending
*    the "\r\n+BRSF=nnn\r\n to the peer. 
* 
*    Functionality only required in the AG.
**********************************************************************/

int HF_Respond_Support_Features(t_bdaddr *gatewayBDAddress, u_int8 gwDLCI)
{
    t_HFP_AG_Peer_Device* peer_device;
	//char BRSF_Command[15] = "\r\n+BRSF:31\r\n";
	char BRSF_Command[15] = "\r\n+BRSF:481\r\n";//syzhou changed
   peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);


// GF  To Be completed 
//  HFP_BT_WRITE_LE_UINT16((u_int8*)BRSF_Command[8],HF_UNIT_SUPPORTED_FEATURES_U_INT16);

   return HF_SendToGateway((u_int8*)BRSF_Command,13,gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_Respond_OK
* DESCRIPTION:
*    Transmits an OK command to the peer    
*
**********************************************************************/
int HF_Respond_OK(t_bdaddr *gatewayBDAddress, u_int8 gwDLCI)
{
    t_HFP_AG_Peer_Device* peer_device;
	char OK_Command[7] = "\r\nOK\r\n";

   peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);

   return HF_SendToGateway((u_int8*)OK_Command,6,gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_DeActivate_NR_EC
* DESCRIPTION:
*    Sends AT+NREC=0\r to AG to deactivate Echo Cancellation
*
**********************************************************************/

int HF_DeActivate_NR_EC(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
   char NREC_Command[12] ="AT+NREC=0\r";
    t_HFP_AG_Peer_Device* peer_device;
   peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
   if(peer_device)
        peer_device->operation_pending = AT_NREC_0;

   return HF_SendToGateway((u_int8*)NREC_Command,10,gatewayBDAddress,gwDLCI);
}


/***********************************************************************
* FUNCTION: HF_Report_Mic_Gain_To_AG
* DESCRIPTION:
*    Sends AT+VGM to AG.
*
**********************************************************************/
int HF_Report_Mic_Gain_To_AG(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI, u_int8 gain)
{
	char VGM_Command[12]="AT+VGM=0 \r";
    t_HFP_AG_Peer_Device* peer_device;

   peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
   if(peer_device)
        peer_device->operation_pending = AT_VGM;

   if (gain > 9)
   {
	   VGM_Command[7] = '1'; 
	   VGM_Command[8] = '0' + (gain-10);
   }
   else
   {
	   VGM_Command[7] = '0' + gain;
	   VGM_Command[8] = ' ';
   }

    return HF_SendToGateway((u_int8*)VGM_Command,10,gatewayBDAddress,gwDLCI);

}


/***********************************************************************
* FUNCTION: HF_Report_Speaker_Gain_To_AG
* DESCRIPTION:
*    Sends AT+VGS to AG.
*
**********************************************************************/

int HF_Report_Speaker_Gain_To_AG(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI, u_int8 gain)
{
	char VGS_Command[12]="AT+VGS=0 \r";
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_VGS;

	if (gain > 9)
	{
		VGS_Command[7] = '1'; 
		VGS_Command[8] = '0' + (gain-10);
	}
	else
	{
		VGS_Command[7] = '0' + gain;
		VGS_Command[8] = ' ';
	}

    return HF_SendToGateway((u_int8*)VGS_Command,10,gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_Set_Mic_Gain_To_HS
* DESCRIPTION:
*    Sends +VGM to HS.
*
**********************************************************************/
int HF_Set_Mic_Gain_To_HS(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI, u_int8 gain)
{
	char VGM_Command[12]="\r\n+VGM=00\n\r";
    t_HFP_AG_Peer_Device* peer_device;

   peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
   if(peer_device)
        peer_device->operation_pending = AT_VGM;

   if (gain > 9)
   {
	   VGM_Command[7] = '1'; 
	   VGM_Command[8] = '0' + (gain-10);
   }
   else
   {
	   VGM_Command[7] = '0' + gain;
	   VGM_Command[8] = ' ';
   }

    return HF_SendToGateway((u_int8*)VGM_Command,11,gatewayBDAddress,gwDLCI);

}


/***********************************************************************
* FUNCTION: HF_Set_Speaker_Gain_To_HS
* DESCRIPTION:
*    Sends +VGS to Headset.
*
**********************************************************************/

int HF_Set_Speaker_Gain_To_HS(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI, u_int8 gain)
{
	char VGS_Command[12]="\r\n+VGS:00\r\n";
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_VGS;

	if (gain > 9)
	{
		VGS_Command[7] = '1'; 
		VGS_Command[8] = '0' + (gain-10);
	}
	else
	{
		VGS_Command[7] = ' ';
		VGS_Command[8] = '0' + gain;
	}

#if pDEBUG
		pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"--cliff-- VGS=%s\n",VGS_Command));
#endif

    return HF_SendToGateway((u_int8*)VGS_Command,11,gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_Dial_Last_Number
* DESCRIPTION:
*    Instructs the AG to re-dial the last number using AT+BLDN
*
**********************************************************************/

int HF_Dial_Last_Number(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
	char BLDN_Command[10]="AT+BLDN\r";
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_BLDN;

    return HF_SendToGateway((u_int8*)BLDN_Command,8,gatewayBDAddress,gwDLCI);
}	

/***********************************************************************
* FUNCTION: HF_Terminate_Call
* DESCRIPTION:
*    Terminates a call in progress using AT+CHUP
*
**********************************************************************/

int HF_Terminate_Call(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
	char *CHUP_Command;
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_CHUP;
    CHUP_Command = "AT+CHUP\r";

    return HF_SendToGateway((u_int8*)CHUP_Command,8,gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_Activate_CLIP
* DESCRIPTION:
*    Activates CLIP in the network using AT+CLIP=1
*
**********************************************************************/

int HF_Activate_CLIP(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
	char CLIP_Command[12]="AT+CLIP=1\r";
    t_HFP_AG_Peer_Device* peer_device;


    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_CLIP_1;

    return HF_SendToGateway((u_int8*)CLIP_Command,10,gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_DeActivate_CLIP
* DESCRIPTION:
*    DeActivates CLIP in the network using AT+CLIP=0
*
**********************************************************************/

int HF_DeActivate_CLIP(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
	char *CLIP_Command;
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_CLIP_0;
    CLIP_Command = "AT+CLIP=0\r";

    return HF_SendToGateway((u_int8*)CLIP_Command,10,gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_Retrieve_Supported_Indicators
* DESCRIPTION:
*    Retrieves the indicators supported in the AG by sending the AT+CIND=?.
*    This command is responeded to by +CIND : ("indicator_name"),("indicator_name"),("indicator_name"),
*
**********************************************************************/

int HF_Retrieve_Supported_Indicators(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
    t_HFP_AG_Peer_Device* peer_device;
    char CIND_Command[10] = "AT+CIND=?\r";

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_CIND_TEST;

    return HF_SendToGateway((u_int8*)CIND_Command,10,gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_Send_Answer
* DESCRIPTION:
*    Answers a call by sending ATA
*
**********************************************************************/

int HF_Send_Answer(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
    t_HFP_AG_Peer_Device* peer_device;
	char ATA_Command[6] = "ATA\r";

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_ANSWER;

    return HF_SendToGateway((u_int8*)ATA_Command,4,gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_Send_Reject
* DESCRIPTION:
*    Rejects a call by sending AT+CHUP
*
**********************************************************************/

int HF_Send_Reject(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
    t_HFP_AG_Peer_Device* peer_device;
	char CHUP_Command[10] = "AT+CHUP\r";

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_CHUP;

    return HF_SendToGateway((u_int8*)CHUP_Command,8,gatewayBDAddress,gwDLCI);


}

/***********************************************************************
* FUNCTION: HF_Send_OK
* DESCRIPTION:
*    Rejects a call by sending OK
*
**********************************************************************/

int HF_Send_OK(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
    t_HFP_AG_Peer_Device* peer_device;
	char OK_Command[7] = "\r\nOK\r\n";

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);

    return HF_SendToGateway((u_int8*)OK_Command,6,gatewayBDAddress,gwDLCI);


}

int HF_Send_ERROR(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
    t_HFP_AG_Peer_Device* peer_device;
	char ERROR_Command[10] = "\r\nERROR\r\n";

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);

    return HF_SendToGateway((u_int8*)ERROR_Command,9,gatewayBDAddress,gwDLCI);


}

/***********************************************************************
* FUNCTION: HF_Retrieve_Indicator_Status
* DESCRIPTION:
*    Retrieves the current status of the AG indicators using AT+CIND? read
*    command.
*    Response comes as +CIND: (x,y,x,.....) with the order previously determined
*    by the +CIND respones to the Test command AT+CIND=?
**********************************************************************/

int HF_Retrieve_Indicator_Status(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{

	char CIND_Command[10]="AT+CIND?\r";
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_CIND;

    return HF_SendToGateway((u_int8*)CIND_Command,9,gatewayBDAddress,gwDLCI);
}


/***********************************************************************
* FUNCTION: HF_Send_RING
* DESCRIPTION:
*    Sends a RING command to the peer. 
*    This is required for both Handsfree AG support and Headset support.
**********************************************************************/

int HF_Send_RING(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{

    char RING_Command[10]="\r\nRING\r\n";
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_CIND;
    if(peer_device->scm_state != SCM_OPEN)  //just send RING indicator after slc build
        return 0;
    return HF_SendToGateway((u_int8*)RING_Command,8,gatewayBDAddress,gwDLCI);
}

#if 0
int HF_Send_CLIP(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
//--------------------------------------------------------------
// GF NOTE THIS IS ONLY A PROTOTYPE FOR THIS FUNCTION 
//  Needs be update to get CLIP for Application or peer_device.
//--------------------------------------------------------------
	char CLIP_Command[24]="\r\n+CLIP: 0868549005\r\n";
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
 //   peer_device->operation_pending = AT_CIND;

    return HF_SendToGateway((u_int8*)CLIP_Command,21,gatewayBDAddress,gwDLCI);
}


static int HF_Send_BVRA(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI,u_int8 ind_value)
{
	char BVRA_Command[14]="\r\n+BVRA: 0\r\n";
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
 //   peer_device->operation_pending = AT_CIND;

    return HF_SendToGateway((u_int8*)BVRA_Command,12,gatewayBDAddress,gwDLCI);
}
#endif

int HF_Send_Indicators_Supported(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
    char Indicators_Supported_Command[]="\r\n+CIND:(\"service\",(0,1)),(\"call\",(0,1)),(\"callsetup\",(0,3)),(\"callheld\",(0,2)),(\"battchg\",(0,5)),(\"signal\",(0,5)),(\"roam\",(0,1))\r\n";
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_CIND;

    return HF_SendToGateway((u_int8*)Indicators_Supported_Command,strlen(&Indicators_Supported_Command[0]),gatewayBDAddress,gwDLCI);

}

int HF_Send_Indicators(u_int8 indicator_type, u_int8 indicator_value, t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
    char Indicator_Command[24]="\r\n+CIEV: 0,0\r\n";

	switch(indicator_type)
	{
	case HFP_CALLSETUP_INDICATOR :

		// ------------------------------------------------------------------
		// There seem to be some issues different interpretations of the 
		// call setup indicators. It can be either "call_setup" or "callsetup".
		// To cover both cases we inform the Handsfree of two seperate indicators
		// one for "call_setup" and one for "callsetup"
		// (This behaviour was observed on the Nokia 6600 interacting with the Jabara 
		// SP100 speaker handsfree unit.
		//
		// The when a call setup indicator needs to be sent we send both. 
		//-------------------------------------------------------------------
#if 1
		Indicator_Command[9] = '3';
        Indicator_Command[11] = indicator_value+'0';
		HF_SendToGateway((u_int8*)Indicator_Command,14,gatewayBDAddress,gwDLCI);

//just send call setup info modified by xiongzhichen
            //Indicator_Command[9] = '4';
            //Indicator_Command[11] = indicator_value+'0';
            //HF_SendToGateway((u_int8*)Indicator_Command,14,gatewayBDAddress,gwDLCI);
#endif

		break;

	case HFP_SERVICE_INDICATOR :
            Indicator_Command[9] = '1';
		//if(indicator_value==1)
        Indicator_Command[11] = indicator_value+'0';

		HF_SendToGateway((u_int8*)Indicator_Command,14,gatewayBDAddress,gwDLCI);
		break;

    case HFP_CALL_INDICATOR :
            Indicator_Command[9] = '2';		
        Indicator_Command[11] = indicator_value+'0';

		HF_SendToGateway((u_int8*)Indicator_Command,14,gatewayBDAddress,gwDLCI);
		break;

	default :
		break;
	}
#if 0
	switch(indicator_type)
	{
	case HFP_CALLSETUP_INDICATOR :

		// ------------------------------------------------------------------
		// There seem to be some issues different interpretations of the 
		// call setup indicators. It can be either "call_setup" or "callsetup".
		// To cover both cases we inform the Handsfree of two seperate indicators
		// one for "call_setup" and one for "callsetup"
		// (This behaviour was observed on the Nokia 6600 interacting with the Jabara 
		// SP100 speaker handsfree unit.
		//
		// The when a call setup indicator needs to be sent we send both. 
		//-------------------------------------------------------------------
#if 1
		Indicator_Command[9] = '3';
		//if(indicator_value==1)
       //     Indicator_Command[11] = '1';

		


		//Indicator_Command[9] = '4';
		//if(indicator_value==1)
        //    Indicator_Command[11] = '1';

		//HF_SendToGateway((u_int8*)Indicator_Command,14,gatewayBDAddress,gwDLCI);
#endif

		break;

	case HFP_CALL_INDICATOR :
 		Indicator_Command[9] = '2';
		break;

	case HFP_SERVICE_INDICATOR :
		Indicator_Command[9] = '1';
		break;


	default :
			break;
		}

	Indicator_Command[11] = indicator_value+ '0';
	#if pDEBUG
	{
	
		//pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"*********:indicator_type=%d;indicator_value=%d;Send CIEV COMMAND:%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",indicator_type,indicator_value,Indicator_Command[0],Indicator_Command[0],Indicator_Command[1],Indicator_Command[2],Indicator_Command[3],Indicator_Command[4],Indicator_Command[5],Indicator_Command[6],Indicator_Command[7],Indicator_Command[8],Indicator_Command[9],Indicator_Command[10],Indicator_Command[11],Indicator_Command[12],Indicator_Command[13]));
		pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"*********:indicator_type=%d;indicator_value=%d;Send CIEV COMMAND:%s\n",indicator_type,indicator_value,Indicator_Command));
		pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"*********:indicator_type=%d;indicator_value=%d;Send CIEV COMMAND:%s\n",indicator_type,indicator_value,Indicator_Command));

	}		
	#endif 
	HF_SendToGateway((u_int8*)Indicator_Command,14,gatewayBDAddress,gwDLCI);
#endif

	return 0;
}

int HF_Send_Indicators_Status(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
      int call_state = 0;
      int call_hold = 0;
      int signal_level = 5;
      int roam_flag = 0;
	char Indicators_Status_Command[]="\r\n+CIND:1,0,0,0,5,5,0\r\n";
    t_HFP_AG_Peer_Device* peer_device;
       if(rda_GetCurrentCallStatus)
        {
            call_state = rda_GetCurrentCallStatus();
        }
       if(call_state > 0 && call_state < 4)
           Indicators_Status_Command[10] = 0x30 + 1;   

       if(rda_GetCallHoldCount)
        {
            call_hold = rda_GetCallHoldCount();
        }
       if(call_hold > 0)
          Indicators_Status_Command[12] = 0x30 + 1;

       if(rda_GetSignalStrength)
        {
            signal_level = rda_GetSignalStrength();
        }
        Indicators_Status_Command[18] = 0x30 + signal_level;

        if(rda_GetRoamStatus)
        {
            roam_flag = rda_GetRoamStatus();
        }
        if(roam_flag == 1)
            Indicators_Status_Command[20] = 0x30 + 1;     
    //added by xiongzhichen to get phone status
    
    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_CIND;

    return HF_SendToGateway((u_int8*)Indicators_Status_Command,strlen(Indicators_Status_Command),gatewayBDAddress,gwDLCI);
}


int HF_Send_Call_Handling(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
	char Call_Handling_Status_Command[30]="\r\n+CHLD: (0,1,1x,2,2x,3,4)\r\n";
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    //peer_device->operation_pending = AT_CIND;

    return HF_SendToGateway((u_int8*)Call_Handling_Status_Command,28,gatewayBDAddress,gwDLCI);

}

/***********************************************************************
* FUNCTION: HF_Get_Indicator_Status
* DESCRIPTION:
*    Retrieves the current status of the AG indicators using AT+CIND? read
*    command.
*    Response comes as +CIND: (x,y,x,.....) with the order previously determined
*    by the +CIND respones to the Test command AT+CIND=?
**********************************************************************/

int HF_Get_Indicator_Status(t_bdaddr *gatewayBDAddress)
{
	char CIND_Command[10]="AT+CIND?\r";
    t_HFP_AG_Peer_Device* peer_device;
    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
	
 if(peer_device)
    return HF_SendToGateway((u_int8*)CIND_Command,9,gatewayBDAddress,peer_device->dlci);
 else
   return 0;
 
}

/***********************************************************************
* FUNCTION: HF_Send_Call_Hold_Test_Command
* DESCRIPTION:
*    Requests the AG to inform the HF of the mechanisms used to support 
*    multiparty call handling. 
*   
*    The response comes in the +CHLD command
* 
**********************************************************************/

int HF_Send_Call_Hold_Test_Command(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
    char CHLD_Command[12] = "AT+CHLD=?\r";
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_CHLD_TEST;

    return HF_SendToGateway((u_int8*)CHLD_Command,10,gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_Send_DTMF_Tone
* DESCRIPTION:
*    Requests the AG to inform the HF of the mechanisms used to support 
*    multiparty call handling. 
*   
*    The response comes in the +CHLD command
* 
**********************************************************************/

int HF_Send_DTMF_Tone(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI,char dtmf)
{
    char VTS_Command[12] = "AT+VTS=n\r";
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_VTS;
    VTS_Command[7] = dtmf;

    return HF_SendToGateway((u_int8*)VTS_Command,9,gatewayBDAddress,gwDLCI);
}

/***********************************************************************
* FUNCTION: HF_Send_BINP
* DESCRIPTION:
*    Sent to the AG to solicit input from the user. 
*    Response comes in +BINP:"nnnnnnn"
* 
**********************************************************************/

int HF_Send_BINP(t_bdaddr *gatewayBDAddress, u_int16 gwDLCI)
{
    char BINP_Command[12] = "AT+BINP=1\r";
    t_HFP_AG_Peer_Device* peer_device;

    g_num_mem_locs--;
    peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);
    if(peer_device)
        peer_device->operation_pending = AT_BINP;

    return HF_SendToGateway((u_int8*)BINP_Command,11,gatewayBDAddress,gwDLCI);
}

u_int8 HF_Connect_As_Client(t_bdaddr ag_bdaddr)
{
	int status = BT_NOERROR;
	t_HFP_AG_Peer_Device* peer_device;
	
	/* Ensure that an SLC does not exist already for this bdaddr */

	/* Call RFCOMM Establishment */

	peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);

	if (!peer_device)
		return BT_UNKNOWNERROR;

	peer_device->rf_role = RFCOMM_CLIENT;
	peer_device->scm_state = SCM_W4_RFCOMM_CONNECT;
	{ 
		int i;

		for (i=0;i<6;i++)
			peer_device->address.bytes[i] = ag_bdaddr.bytes[i];
	}
	if (!peer_device->evHFP_TxOK)
	{
		//peer_device->evHFP_TxOK = pEventCreate(0);
	}

	peer_device->HFP_TxOK = 1;
    // @@@@  SET SLC TIMER HERE 
#if COMBINED_HOST==1
	peer_device->slc_setup_timer_handle = host_timer_create((unsigned int)PRH_HFP_SLC_SETUP_TIMEOUT,
		                                               HF_SLC_Setup_Timeout,(void*)peer_device,pTIMER_ONESHOT);
#endif
	/* Note - Max frame size is set to 100 Bytes as we Tx a +CIND of 83 Bytes
	 */
	 
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Connect_As_Client chnl_num=%d\n",peer_device->serverChannel));
#endif 

	status=RSE_CliConnectPort(ag_bdaddr,(u_int8)peer_device->serverChannel,100,&rfcommCallbacks);
      gSLCConnectTimer = pTimerCreate(PRH_HFP_SLC_SETUP_TIMEOUT, HF_SLC_Setup_Timeout,(void*)peer_device,pTIMER_ONESHOT);
	return status;

}

/***********************************************************************
* FUNCTION: HF_Send_Call_Setup
* DESCRIPTION:
*    
* 
**********************************************************************/

int HF_Send_Call_Setup(t_bdaddr ag_bdaddr, u_int8 call_type, char* string, u_int16 memory_id)
{
    u_int8 ATD_Command[50];
    t_HFP_AG_Peer_Device* peer_device;
    u_int8 len;

    peer_device = Find_Audio_GW_By_Address(&ag_bdaddr);
    if(peer_device==NULL)
        return BT_INVALIDPARAM;

	switch (call_type)
	{

	case HF_CALL_TYPE_NORMAL :
        {
			int i = 0;

			strcpy((char*)ATD_Command,"ATD");
			{			
				while(string[i] != '\0')
				{
					ATD_Command[i+3] = string[i] ;
					i++;
				}
			}
			ATD_Command[i+3] = ';';
			i++;
			ATD_Command[i+3]= '\r';
			len=i+4;
			
		}
		peer_device->operation_pending = ATD;
		break;

	case HF_CALL_TYPE_MEM :
		 {
			int i = 0;

			strcpy((char*)ATD_Command,"ATD>");
			{			
				while(string[i] != '\0')
				{
					ATD_Command[i+4] = string[i] ;
					i++;
				}
			}
			ATD_Command[i+4] = ';';
			i++;
			ATD_Command[i+4]= '\r';
			len=i+5;
			
		}
		peer_device->operation_pending = ATD;
		break;

	case HF_CALL_TYPE_REDIAL :
		HF_Dial_Last_Number(&ag_bdaddr,peer_device->dlci);
		return 0;
		break;

	default :
		return 1;

	}
    return HF_SendToGateway((u_int8*)ATD_Command,len,&peer_device->address,peer_device->dlci);
}


int HF_SendToGateway(u_int8 *data, int dataLen, t_bdaddr *gatewayBDAddress, u_int16 gatewayDLCI) 
{
	int status;
	struct host_buf *txData;
      struct prh_rfcomm_se_user *se_info;
	t_HFP_AG_Peer_Device* peer_device;
      int sendCount = 0;
      int maxFrameSize = 0;
      int ret;
     
#if pDEBUG
	char Indicator_Command[24]="\r\n+CIEV: 0,0\r\n";

		pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"sendtogateway is called gatewayDLCI=%d\n",gatewayDLCI));
#endif 
	peer_device = Find_Audio_GW_By_Address(gatewayBDAddress);

	if (!peer_device)
	{
		return 0;
	}

  
      ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, *gatewayBDAddress, gatewayDLCI);
      if(ret != BT_NOERROR)
        {
            pDebugPrintfEX((pLOGNOTICE,pLOGHFP, "*** HF send data error return 1!!"));
            return 1;
        }
      maxFrameSize = se_info->dlc_params.max_frame_size;
      while(sendCount < dataLen) {
    	status = RSE_GetWriteBuffer(*gatewayBDAddress, gatewayDLCI, maxFrameSize+2, &txData);
    	if (status!=BT_NOERROR)
    	{
    	      pDebugPrintfEX((pLOGERROR,pLOGHFP, "*** HF send data error return 2!!"));
    		return 1;
    	}

      if((dataLen - sendCount) > maxFrameSize) {
    	    memcpy(txData->data, data + sendCount, maxFrameSize);
    	    txData->len = maxFrameSize;
           sendCount += maxFrameSize;
      }
      else
      {
           txData->len = dataLen - sendCount;
           memcpy(txData->data, data + sendCount, txData->len);
           sendCount = dataLen;
      }
	//if (peer_device->pending_data)
	{
		//pFree(peer_device->pending_data);
		//peer_device->pending_data = 0;
		memset(peer_device->pending_data,0,sizeof(peer_device->pending_data));
		peer_device->pending_data_len = 0;
	}

	/*****************************************
	 * Low Power mode handling 
	 ****************************************/

	/* Clear any pre-existing Low Power mode Timer */
	if(peer_device->enter_sniff_mode_timer_handle)
	{
#if COMBINED_HOST==1
		host_timer_cancel(peer_device->enter_sniff_mode_timer_handle);
#endif
		peer_device->enter_sniff_mode_timer_handle = 0;
	}

#ifndef TEST_NO_SNIFF
	/* Set a new low power mode timer */
#if COMBINED_HOST==1
	peer_device->enter_sniff_mode_timer_handle = host_timer_create((unsigned int)PRH_HFP_SNIFFMODE_TIMEOUT,
		                                               					HF_Enter_Sniff_Mode,(void*)peer_device,pTIMER_ONESHOT);
#endif
#endif
	/* If the device is already in low Power mode, exit it and then send data
	 * It is not important that the device exits sniff mode prior to sending data.
	 * As the actual data can be Txed during Sniff mode
	 */
	if (peer_device->acl_state == HFP_DEVSNIFF)
		HF_Exit_Sniff_Mode(peer_device);

	/*****************************************
	 * End Low Power Mode handling 
	 *****************************************/
#if (HFP_MULTIPOINT_SUPPORT==1)
	if (!peer_device->HFP_TxOK)
	{
		//pEventWait(peer_device->evHFP_TxOK,NULL,NULL);
	}
#else
	if (!HFP_TxOK)
	{
		pEventWait(evHFP_TxOK);
	}
#endif
#if pDEBUG
	if(0==pMemcmp(Indicator_Command,data,8))
        {
		pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"Send CIEV COMMAND:%s\n",data));
	}
#endif 
	status = RSE_Write(*gatewayBDAddress, (u_int8)gatewayDLCI, txData);
	pDebugPrintfEX((pLOGERROR,pLOGHFP,"sendtogateway is called,status=%d,send:%s\n",status,data));		

	/* Exception in the Case the Peer device allocates Zero Credits on the 
	 * PN negociation. As this call can be done in the context of the Callback
	 * for connection complete/indication then we can end up transmitting when
	 * there are 0 creits on the link to the peer and thus loose the RFCOMM Frame.
	 * 
	 * Instead if the lower layer report a flow control error we wait for more 
	 * a go indication from RFCOMM to indicate it now has credits from the peer.
	 *
	 * This problem occurs with Nokia devices (e.g 6230) and will probally also 
	 * occur in all other applications using RFCOMM, when communicating the Host
	 * layers used by Nokia.
	 */


	if (status==BT_FLOWCTRL)
	{
#if 1 // GF Hack for Flow Control  12 May 

		peer_device->HFP_TxOK = 0;
#endif
		//peer_device->pending_data = pMalloc(dataLen);
		peer_device->pending_data_len = dataLen;
		pMemcpy(peer_device->pending_data,data,dataLen);

		// NOTE :- Must handle condiction of below timer expiring after SLC release 
		//         i.e store handle and clear timer on release.
#if COMBINED_HOST==1
		host_timer_create((unsigned int)PRH_HFP_RSE_RETRANSMIT_TIMEOUT,HFP_RSE_Retransmit,&peer_device->address,pTIMER_ONESHOT);
#endif
             pTimerCreate((unsigned int)5, HFP_RSE_Retransmit,&peer_device->address, pTIMER_ONESHOT);
	}


	if (status!=BT_NOERROR) 
	{
		RSE_FreeWriteBuffer(txData);
		return 1;
	} 
    }   
    return 0;
}

void HFP_RSE_Retransmit(void *arg)
{
	t_bdaddr* p_pending_baddr = (t_bdaddr*)arg;
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(p_pending_baddr);

	if (!peer_device)
		return ;

	HF_SendToGateway(peer_device->pending_data,peer_device->pending_data_len,p_pending_baddr,peer_device->dlci);
}



/**************************************************/
/*  AT COMMAND HANDLER FUNCTIONS                  */
/*                                                */
/**************************************************/


/*************************************************************************
 * Function :-  HF_rfcommRxDataCallback(..)
 *
 * Description 
 * This function handles Rx callbacks from RFCOMM. This is mainly used for
 * the delivery of AT commands to the HF.
 *
 * The core component of this function is an assembler of RFCOMM data frames into a 
 * buffer. This specifically supports AT-Commands or Result codes which are 
 * segmented accross multiple RFCOMM frames.
 * 
 * Another critical aspect is to support an scenario where part of two or more AT-Commands
 * are present in the same RFCOMM frame.
 *
 *
 ****************************************************************************/

static int HF_rfcommRxDataCallback(t_bdaddr address, u_int8 dlci, struct host_buf *rxData) 
{
	unsigned int i=0;
	int finger = 0;
	int buffer_size = 0;
	int next_command_offset = 0;
	static char assemblyBuffer[300];
	static int assemblyOffset=0;

	/***********************************************************************
	 *                                                                     *
	 *  NOTE :- AT-Commands can be fragmented over multiple RFCOMM frames. *
	 *          In addition a single RFCOMM frame can contain multiple     *
	 *          AT-Commands.                                               *
	 *          The Ericsson T630 uses this format for +CIND               *
	 *          It will also place the end of +CIND in the same frame as   *
	 *          the "OK".                                                  *
	 *                                                                     *
	 *          So we build up an Assembly buffer over multiple RFCOMM     *
	 *          frames. We only terminate the                              *
	 ***********************************************************************/

	/***********************************************************************
	 *  Handle the Re-assembly of RFCOMM FRAMES
	 *
	 ***********************************************************************/

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_rfcommRxDataCallback dlci=%d\n",dlci));
#endif  

        memset(assemblyBuffer, 0, sizeof(assemblyBuffer));

	if (assemblyOffset==0)
	{
		if ( (rxData->data[i] == '\r') && (rxData->data[i+1] == '\n'))
		{
			assemblyBuffer[0]=rxData->data[0];
			assemblyBuffer[1]=rxData->data[1];
			i+=2;
			assemblyOffset+=2;
		}
	}

	for (; i<rxData->len; i++) 
	{
		assemblyBuffer[assemblyOffset]=rxData->data[i];

		if (assemblyOffset<300)
			assemblyOffset++;

		/****************************************************************
		*  Need to change this in the AG to allow AT commands to be Rxed 
		*****************************************************************/
		if ((rxData->data[i]=='\n') && (i == (rxData->len-1) ))
		{
			buffer_size = assemblyOffset;
			assemblyOffset=0;					
		}
#if 1
		else /* Support for Rx of AT command */
		{
			if ((rxData->data[i]=='\r') && (i == (rxData->len-1) ))
			{
				buffer_size = assemblyOffset;
				assemblyOffset=0;					
			}
		}
#endif
	}

	RSE_FreeWriteBuffer(rxData);
	if (assemblyOffset)
	{
		return 0;
	}

	/***********************************************************************
	 *  Parse the Assembled Buffer. As the buffer may contain more than one 
	 *  command - the buffer is scanned to find terminators for each command. 
	 *  Each command is then passed to the AT Command parser.
	 *
	 *  NOTE :- For now only Unsolicated Result Codes are conidered as the 
	 *  handsfree unit will only Rx Result codes. 
	 ***********************************************************************/

	i = 0;
	finger = 0;
	next_command_offset = 0;
	
	while(next_command_offset != buffer_size)
	{
		finger = next_command_offset;
		/******************************************************************
		* Check if we have initial sequence for an Result code  "\r\n"
		******************************************************************/
		if ((assemblyBuffer[finger]=='\r') && (assemblyBuffer[finger+1]=='\n'))
		{
			finger+=2;

			/******************************************************************
			* Determine the location of the Command termination  in the buffer  
			******************************************************************/
			while((finger < buffer_size) && (assemblyBuffer[finger] != '\r'))
				finger++;

			if( ((finger+1) < buffer_size) && 
				(assemblyBuffer[finger]=='\r') &&
				(assemblyBuffer[finger+1] == '\n'))
			{
#if 1
#if HFP_CORE_DEBUG	
				/*******************************
				* Found Terminator for Command 
				*******************************/
				{
					int i;

					printf("\n Rx :- ");
					for(i=0; ( (i<16) && (i<buffer_size) ); i++)
					{
						if ((assemblyBuffer[i] != '\r') && (assemblyBuffer[i] != '\n'))
							printf("%c",assemblyBuffer[i]);
					}

				}
#endif
#endif
				HF_Decode_AT_Hayes((assemblyBuffer+next_command_offset),address,dlci);
				next_command_offset =  (finger+2);

			}
			else
			{
				/********************************
				* Error in AT Command Handling 
				********************************/
				return 0;
			}
		}
		/*
		Support the Rx of "AT+XXXX" for headset and handsfree gateway 
		 */
		else if (   (assemblyBuffer[finger]=='A') && (assemblyBuffer[finger+1]=='T') &&
			     ((assemblyBuffer[finger+2]=='+') || (assemblyBuffer[finger+2]=='A')))
		{
			/******************************************************************
			* Determine the location of the Command termination  in the buffer  
			******************************************************************/

			while((finger < buffer_size) && (assemblyBuffer[finger] != '\r'))
				finger++;
			
			if((finger < buffer_size) && (assemblyBuffer[finger]=='\r'))
			{
#if 1
#if HFP_CORE_DEBUG	
				/*******************************
				* Found Terminator for Command 
				*******************************/
				{
					int i;

					printf("\n Rx :- ");
					for (i=0;((i<16)&&(i<buffer_size));i++)
					{
						if ((assemblyBuffer[i] != '\r') &&
							(assemblyBuffer[i] != '\n'))

						printf("%c",assemblyBuffer[i]);
					}

				}
#endif
#endif

				HF_Decode_AT_Hayes((assemblyBuffer+next_command_offset),address,dlci);
				next_command_offset =  (finger+1);

			}
			else
			{
				/********************************
				 * Error in AT Command Handling 
				 ********************************/
				return 0;
			}
		}
		else
		{
			/******************************************************************
			 * Nokia 6230 Seems to have addional byte @ start of CCWA.
			 ******************************************************************/
			next_command_offset++;
		}
    }
	return 1;
	
}


/*************************************************************************
 * Function :-  HF_Decode_AT_Hayes(..)
 *
 * Description 
 * This function Decodes atomic AT commands in to a more user friendly format.
 * It relies on seperate functions for decoding AT-Commands and Result Codes.
 *
 *
 ****************************************************************************/

int HF_Decode_AT_Hayes(char* assemblyBuffer,t_bdaddr address, u_int8 dlci)
{
	unsigned int i=0;
	int status=0;

	if (assemblyBuffer[i] == 'A')
	{
		status = HF_Decode_AT_Command(assemblyBuffer,address,dlci);
	}
	else
	{ 
		status = HF_Decode_AT_USR(assemblyBuffer,address,dlci);
	}

	return status;
}

/***********************************************************************
 *
 * Function : HF_Decode_AT_Command(..)
 *
 * Description
 * This function is responsible for decoding AT Hayes Commands. Normal commands
 * prefixed with "AT" are supported.
 *
 * NOTE :- This Function is not complete and not tested. As it is only required 
 * for the build of a HF Audio Gateway - it is not to be used in the early release
 * of the HFP. Also as this is not required in the final deployed solution it 
 * should be removed.
 *  
 *  Commands Support 
 *         AT+CCWA
 *         AT+CHLD
 *         AT+CHUP
 *         AT+CIND
 *         AT+CLIP
 *         AT+BINP
 *         AT+BLDN
 *         AT+BVRA
 *         AT+BRSF
 *         AT+VTS
 *         AT+VGM
 *         AT+VGS
 *         AT+NREC
 *         
 ****************************************************************************/

int HF_Decode_AT_Command(char* assemblyBuffer,t_bdaddr address, u_int8 dlci)
{
	int i=0;
	t_AT_Event AT_Event;
	
	AT_Event.cmd = HF_AT_UNKNOWN_COMMAND;
	AT_Event.dlci = dlci;
#if pDEBUG
			pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Decode_AT_Command AT command: %s\n",assemblyBuffer));
#endif  
	
	if((assemblyBuffer[i+1]=='T')&&(assemblyBuffer[i+2]=='+'))
	{
		// Valid "AT+" preceeding command
		switch(assemblyBuffer[i+3])
		{
			
		case 'C' : 
			{
				switch(assemblyBuffer[i+4])
				{
				case 'C' : /****** AT+CCWA **********/
					if (((assemblyBuffer[i+5])== 'W') &&
						((assemblyBuffer[i+6])== 'A'))
					{
						if ((assemblyBuffer[i+7] == '=') &&
							(assemblyBuffer[i+9] == '\r'))
						{
							AT_Event.n_value =  assemblyBuffer[i+8];   
							AT_Event.cmd = HF_AT_CMD_CALL_WAITING;
						}
                            HF_Send_OK(&address, dlci);
                        }
                        else
                            HF_Send_ERROR(&address, dlci);
					break; 
				case 'H' : /****** AT+CHLD or AT+CHUP **********/
                                if (((assemblyBuffer[i+5])== 'L') &&
                                    ((assemblyBuffer[i+6])== 'D'))
                                {
                                    if ((assemblyBuffer[i+7] == '=') &&
                                        (assemblyBuffer[i+9] == '\r'))
                                    {
                                        AT_Event.n_value = assemblyBuffer[i+8];
                                        AT_Event.cmd = HF_AT_CMD_CALL_HOLD;
                                        HF_Send_Call_Handling(&address, dlci);
                                        HF_Send_OK(&address, dlci);	
                                    }
                                else  // AT+CHLD=0 or 1, 2, 3,4 etc.
                                {
                                    HF_Send_OK(&address, dlci);
                                }
//                            else
//                            {
//                                HF_Send_OK(&address, dlci);	
//                            }
                                }
                                else if (((assemblyBuffer[i+5])== 'U') &&
                                        ((assemblyBuffer[i+6])== 'P'))
                                {
                                    //bt_hfg_sco_disconnect_req_struct  scoDisc;
                                    u_int8 connection_id=0;
                                    t_HFP_AG_Peer_Device* peer_device;
                                    peer_device = Find_Audio_GW_By_Address(&address);
                                    AT_Event.cmd = HF_AT_CMD_CALL_HANGUP;
                                    HF_Send_OK(&address, dlci);	

                                    if(peer_device)
                                    {
                                        if (peer_device->ring_timer)//syzhou added
                                        {
                                            //hfp_cb.hfpRejectIncomingCallConf();
                                            rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_REJECT_INCOMING_CALL_CNF,NULL);
                                            //HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,0,&address,dlci);
                                        }
                                        else
                                        {
                                            //scoDisc.connection_id = peer_device->acl_handle; 
                                            connection_id = peer_device->acl_handle;
                                            //hfp_cb.hfpSetDiscFromHF((void *)&scoDisc);
                                            rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_DISCONNECT_FROM_HF_IND,&connection_id);
                                        }
                                    }
                                }
                        else
                            HF_Send_ERROR(&address, dlci);
                                break;
                    
                            case 'I' : /****** AT+CIND **********/
                                if (((assemblyBuffer[i+5])== 'N') &&
                                    ((assemblyBuffer[i+6])== 'D'))
                                {
                                        // ADD SUPPORT FOR PARAMETER HANDLING
                                        AT_Event.cmd = HF_AT_CMD_CALL_INDICATORS;

                                    if ((assemblyBuffer[i+7]== '=') &&
                                        (assemblyBuffer[i+8]== '?'))
                                    {
                                        // AT+CIND=?
                                        HF_Send_Indicators_Supported(&address, dlci);
                                        HF_Send_OK(&address, dlci);
                                    }
                                    else if (assemblyBuffer[i+7]== '?') 
                                    { 
                                        // AT+CIND
                                        HF_Send_Indicators_Status(&address, dlci);
                                        HF_Send_OK(&address, dlci);
                                    }

                                }
                        else
                            HF_Send_ERROR(&address, dlci);
                            break;
                    
                            case 'L' : /****** AT+CLIP **********/
                                if (((assemblyBuffer[i+5])== 'I') &&
                                ((assemblyBuffer[i+6])== 'P'))
                                {
                                    if (assemblyBuffer[i+7] == '=')
                                    {
                                        AT_Event.p_service = assemblyBuffer+i+8;
                                        AT_Event.cmd = HF_AT_CMD_CALL_LINE_ID;
                                    }
                                }
                                HF_Send_OK(&address, dlci);
                            break;

					case 'M' : /******* AT+CMER **********/
						if (((assemblyBuffer[i+5])== 'E') &&
							((assemblyBuffer[i+6])== 'R'))
						{
							if (assemblyBuffer[i+7] == '=')
							{
								AT_Event.p_service = assemblyBuffer+i+8;
								AT_Event.cmd = HF_AT_CMD_CALL_LINE_ID;
								Handle_Rx_AT_CMER(&address, dlci,AT_Event.p_service);
								HF_Send_OK(&address, dlci);

								// Callback the application to indicate SLC establishment
								{
									t_HFP_AG_Peer_Device* peer_device;
									peer_device = Find_Audio_GW_By_Address(&address);
									if (peer_device)
									{
										peer_device->active_profile = ACTIVE_PROFILE_HANDSFREE_AG;
										peer_device->scm_state = SCM_OPEN;
#if COMBINED_HOST==1
										host_timer_cancel(peer_device->slc_setup_timer_handle);
#endif
										peer_device->slc_setup_timer_handle = 0;

										/* Callback the Application to indicate the SLC is complete */
#if COMBINED_HOST==0
                                                                        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Decode_AT_Command gFirstFlag=%d", gFirstFlag));
                                                                        if(gFirstFlag) //AG setup the ACL connect firstly.
                                                                        {
                                                                            if (HF_callbacks.HF_CB_SLC_Established_Ind)
                                                                                (*HF_callbacks.HF_CB_SLC_Established_Ind)(peer_device->address, BT_NOERROR);
                                                                        }
                                                                        else
                                                                        {
                                                                            if (HF_callbacks.HF_CB_SLC_Requested_Ind)
                                                                                (*HF_callbacks.HF_CB_SLC_Requested_Ind)(peer_device->address);
                                                                        }
#endif										
									}
								}
							}
						}
                                        else if (	((assemblyBuffer[i+5])== 'E') && ((assemblyBuffer[i+6])== 'E')) /**AT + CMEE**/
                                        {
                                            if (assemblyBuffer[i+7] == '=')
                                            {
                                                HF_Send_OK(&address, dlci);
                                            }
                                        }
                        else
                            HF_Send_ERROR(&address, dlci);
						break;

					case 'K' : /******** AT+CKPD=200 ******/
						if (((assemblyBuffer[i+5])== 'P') &&
							((assemblyBuffer[i+6])== 'D'))
						{
							if (assemblyBuffer[i+7] == '=')
							{
								if ((assemblyBuffer[i+8]  == '2') &&
									(assemblyBuffer[i+9]  == '0') &&
									(assemblyBuffer[i+10] == '0'))
								{
									AT_Event.cmd = HF_AT_CMD_CKPD;
									Handle_Rx_AT_CKPD(&address, dlci);
								}
							}
						}
                        else
                            HF_Send_ERROR(&address, dlci);
                        break;
                  default:
                    HF_Send_ERROR(&address, dlci);
                    break;
				}
			}
			break;

			case 'B' :
			{
				switch(assemblyBuffer[i+4])
				{
					case 'I' :  /****** AT+BINP **********/
						if (((assemblyBuffer[i+5])== 'N') &&
							((assemblyBuffer[i+6])== 'P') &&
							((assemblyBuffer[i+7])== '=') &&
							((assemblyBuffer[i+9])== '\r'))
						{
							AT_Event.n_value = assemblyBuffer[i+8];
							AT_Event.cmd = HF_AT_CMD_BT_REQ_INPUT;
						}
                        HF_Send_OK(&address, dlci);
						break;

					case 'L' : /****** AT+BLDN **********/
						if (((assemblyBuffer[i+5])== 'D') &&
							((assemblyBuffer[i+6])== 'N'))
						{
							//bt_hfg_sco_disconnect_req_struct  scoDisc;
						//	u_int8 connection_id=0;
							AT_Event.cmd = HF_AT_CMD_BT_LAST_DIALED_NUM;
							HF_Send_OK(&address,dlci);//syzhou added

							//HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,2,&address,dlci);

							
							Bldn_Present_Flag=1;
							// send +CIEV:3,2
							// send +CIEV:4,2
							HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,2,&address,dlci);//syzhou added
							//hfp_cb.hfpBldnFromHF((void *)&scoDisc);
                            rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_BLDN_FROM_HF_IND,NULL);

							
						}
                         else
                            HF_Send_ERROR(&address, dlci);
						break;

					case 'V' : /****** AT+BVRA **********/
						if (((assemblyBuffer[i+5])== 'R') &&
							((assemblyBuffer[i+6])== 'A') &&
							((assemblyBuffer[i+7])== '=') &&
							((assemblyBuffer[i+9])== '\r'))
						{
							//bt_hfg_sco_disconnect_req_struct  scoDisc;
							t_HFP_AG_Peer_Device* peer_device;
							peer_device = Find_Audio_GW_By_Address(&address);
							AT_Event.n_value = assemblyBuffer[i+8];
							AT_Event.cmd = HF_AT_CMD_BT_VOICE_REC_ACT;

							//----------------------------------------
							// NOTE :- May need to interact with the application 
							// here to control/ allow the setup of the Audio Channel
							//   ( for now bypass application )
							//------------------------------------------
							// Need to activate an audio link to the HandsFree Unit.

							HF_Send_OK(&address,dlci);

							//scoDisc.connection_id = peer_device->acl_handle; 
							//hfp_cb.hfpSetDiscFromHF((void *)&scoDisc);

							//HF_Send_Indicators(HFP_CALL_INDICATOR,0,&address,dlci); 

							
						}
                         else
                            HF_Send_ERROR(&address, dlci);
						break;

					case 'R' : /****** AT+BRSF **********/
					/*************************************************/
					/* From tests with the Jabara device this command 
					* Has the following format :
					*   "AT+BRSF=26\r"
					* The actual supported features are exchanged as 
					* a decimal number in ASCII format 
					*************************************************/
						if (((assemblyBuffer[i+5])== 'S') &&
							((assemblyBuffer[i+6])== 'F'))
						{
							if ((assemblyBuffer[i+7] == '='))
							{
								{
									t_HFP_AG_Peer_Device* peer_device;
									u_int16 features=0;

									peer_device = Find_Audio_GW_By_Address(&address);

									if (peer_device != 0)
									{
										if (assemblyBuffer[i+8] <= '9')
										{
											features = (assemblyBuffer[i+8] - '0');
										}
										if ((assemblyBuffer[i+9] != '\r') &&
											(assemblyBuffer[i+9] <= '9'))
										{
											features = (features*10)+(assemblyBuffer[i+9] - '0');
										}

										peer_device->supportedFeatures = features;
									}
								}
								HF_Respond_Support_Features(&address, dlci);
								HF_Respond_OK(&address, dlci);

							}
                         else
                            HF_Send_ERROR(&address, dlci);
						}
						break;
                    default:
                        HF_Send_ERROR(&address, dlci);
                        break;
				}
			}
			break;

			case 'V' : /****** AT+VTS/ AT+VGM / AT+VGS **********/
				switch(assemblyBuffer[i+4])
				{
					case 'T' :
						if (((assemblyBuffer[i+5])== 'S') &&
							((assemblyBuffer[i+6])== '='))
						{
							int gain;
							//AT_Event.n_value = assemblyBuffer[i+7]; 
							if ((assemblyBuffer[i+7] >= '0') &&
								(assemblyBuffer[i+7] <= '9'))
							{
								if ((assemblyBuffer[i+8] >= '0') &&
									(assemblyBuffer[i+8] <= '1'))
								{
									gain = ((assemblyBuffer[i+7] - '0') * 10) + (assemblyBuffer[i+8] - '0');
								}
								else
								{
									gain = ((assemblyBuffer[i+7] - '0') * 10);
								}
							}
#if HFP_CORE_DEBUG	
							printf("\n\r Got AT+VTS = %x  \n",gain);
#endif
						}
                         else
                            HF_Send_ERROR(&address, dlci);
						break;
					
					case 'G' :
						if (((assemblyBuffer[i+5])== 'S') &&
							((assemblyBuffer[i+6])== '='))
						{
							int gain=0;
                                                st_t_hfg_gain_ind_struct ret;
                                                t_HFP_AG_Peer_Device* peer_device;
                                                peer_device = Find_Audio_GW_By_Address(&address);
        						//AT_Event.n_value = assemblyBuffer[i+7]; 
							if ((assemblyBuffer[i+7] >= '0') &&
								(assemblyBuffer[i+7] <= '9'))
							{
								if ((assemblyBuffer[i+8] >= '0') &&
									(assemblyBuffer[i+8] <= '9'))
								{
									gain = ((assemblyBuffer[i+7] - '0') * 10) + (assemblyBuffer[i+8] - '0');
								}
								else
								{
									gain = (assemblyBuffer[i+7] - '0');
								}
							}
#if pDEBUG	
							 pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"Got AT+VGS = %d\n",gain));
#endif
							HF_Send_OK(&address,dlci);
                                                if(peer_device)
                                                {
                                                    ret.connection_id = peer_device->acl_handle;
                                                }
                                                else
                                                    ret.connection_id = 0;
                                                ret.gain = gain; 
                                                rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_SPEAKER_GAIN_IND,&ret);
						}
						else if (((assemblyBuffer[i+5])== 'M') &&
							((assemblyBuffer[i+6])== '='))
						{
                                                int gain=0;
                                                st_t_hfg_gain_ind_struct ret;
                                                t_HFP_AG_Peer_Device* peer_device;
                                                peer_device = Find_Audio_GW_By_Address(&address);
							//AT_Event.n_value = assemblyBuffer[i+7]; 
							if ((assemblyBuffer[i+7] >= '0') &&
								(assemblyBuffer[i+7] <= '9'))
							{
								if ((assemblyBuffer[i+8] >= '0') &&
									(assemblyBuffer[i+8] <= '1'))
								{
									gain = ((assemblyBuffer[i+7] - '0') * 10) + (assemblyBuffer[i+8] - '0');
								}
								else
								{
									gain = ((assemblyBuffer[i+7] - '0') * 10);
								}
							}
#if HFP_CORE_DEBUG	
        						 pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"Got AT+VGM = %d\n",gain));
#endif
							HF_Send_OK(&address,dlci);

                                                if(peer_device)
                                                {
                                                    ret.connection_id = peer_device->acl_handle;
                                                }
                                                else
                                                    ret.connection_id = 0;
                                                ret.gain = gain; 
                                                rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_MIC_GAIN_IND,&ret);
						}
                         else
                            HF_Send_ERROR(&address, dlci);
						break;
						
					default :
                        HF_Send_ERROR(&address, dlci);
						break;
				}
			break;

			case 'N' : /*********** AT+NREC *************************/
				if ((assemblyBuffer[i+4]=='R') &&
					(assemblyBuffer[i+5]=='E') &&
					(assemblyBuffer[i+6]=='C'))
				{
					if (((assemblyBuffer[i+7])== '=') &&
						((assemblyBuffer[i+9])== '\r'))
					{
						AT_Event.n_value = assemblyBuffer[i+8];
						AT_Event.cmd = HF_AT_CMD_BT_DISABLE_EC_RN;
					}
				}
                     else
                            HF_Send_ERROR(&address, dlci);
			break;
			
			default :
                HF_Send_ERROR(&address, dlci);
				break;
		}
		return 1;
	}
	else if((assemblyBuffer[i+1]=='T')&&(assemblyBuffer[i+2]=='A'))
	{

		/* Stop Alerting the HandsFree Unit */
		Handle_Rx_ATA(&address,dlci);
	}
	return 0;
}

/***********************************************************************
 *
 * Function : HF_Decode_AT_USR(..)
 *
 * Description
 * This function is responsible for decoding AT Hayes Unsolicitated Result Codes.
 * USRs are normally prefixed with "\r\n"
 *
 *  
 *  USRs Supported
 *         ERROR
 *         OK
 *         +CCWA:
 *         +CLIP:
 *         +CIEV:
 *         +CIND:
 *         +CHLD
 *         +BVRA:
 *         +BRSF:
 *         +BSIR:
 *         
 ****************************************************************************/

int HF_Decode_AT_USR(char* assemblyBuffer,t_bdaddr address, u_int8 dlci)
{
	int i=0;
	t_AT_Event AT_Event;
	
	AT_Event.cmd = HF_AT_UNKNOWN_COMMAND;
	AT_Event.dlci = dlci;
	
	// Unspecified Result code, Error or OK  
#if pDEBUG
		pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Decode_AT_USR --cliff--\n"));
#endif  

	
	if ((assemblyBuffer[i] == '\r') && (assemblyBuffer[i+1] == '\n'))
	{
		if ((assemblyBuffer[i+2] == 'E') &&
			(assemblyBuffer[i+3] == 'R') &&
			(assemblyBuffer[i+4] == 'R') &&
			(assemblyBuffer[i+5] == 'O') &&
			(assemblyBuffer[i+6] == 'R'))
		{
			if ((assemblyBuffer[i+7] == '\r') && (assemblyBuffer[i+8] == '\n'))
			{
				/****** ERROR **********/
				Handle_Error_Command(&address,dlci);
			}
		}
		else if ((assemblyBuffer[i+2] == 'O') &&
				(assemblyBuffer[i+3] == 'K'))
		{
			if ((assemblyBuffer[i+4] == '\r') && (assemblyBuffer[i+5] == '\n'))
			{
				/****** OK **********/
				AT_Event.cmd = HF_AT_OK;
				AT_Event.dlci = dlci;
				Handle_OK_Command(&address,dlci);
			}
		}
		else if (assemblyBuffer[i+2] == '+') // Unspecified Result code
		{
			switch (assemblyBuffer[i+3])
			{
			case 'C' :  // Standard Result Code
				switch(assemblyBuffer[i+4])
				{
				case 'C' : /****** +CCWA **********/
					if ((assemblyBuffer[i+5] == 'W') &&
						(assemblyBuffer[i+6] == 'A'))
					{
						if (assemblyBuffer[i+7] == ':')
						{
							AT_Event.cmd = HF_AT_USR_CALL_WAITING;
							AT_Event.p_service = assemblyBuffer+i+8;
							Handle_Rx_CCWA(&address,&AT_Event);
						}
					}
					break;
				case 'L' : /****** +CLIP **********/
					if ((assemblyBuffer[i+5] == 'I') &&
						(assemblyBuffer[i+6] == 'P'))
					{
						if (assemblyBuffer[i+7] == ':')
						{

							AT_Event.cmd = HF_AT_USR_CALLING_LINE_ID;
							AT_Event.p_service = assemblyBuffer+i+8;
							Handle_Rx_CLIP(&address,&AT_Event);
						}
					}
					break;
					
				case 'I' : /****** +CIEV **********/
					if ((assemblyBuffer[i+5] == 'E') &&
						(assemblyBuffer[i+6] == 'V') &&
						(assemblyBuffer[i+7] == ':'))
					{
						AT_Event.cmd = HF_AT_USR_CALL_INDICATORS;
						AT_Event.p_service = (assemblyBuffer+i+8);
						Handle_Rx_CIEV(&address,&AT_Event);
					}
					else if ((assemblyBuffer[i+5] == 'N') &&
						(assemblyBuffer[i+6] == 'D') &&
						(assemblyBuffer[i+7] == ':'))
					{
						AT_Event.cmd = HF_AT_USR_CALL_INDICATOR_CONTROL;
						AT_Event.p_service = (assemblyBuffer+i+8);
#if HFP_CORE_DEBUG	
						printf(" Handling Rx CIND ");
#endif
 						Handle_Rx_CIND(&address,&AT_Event);
					}	
					break;	
					
				case 'H' :
					if ((assemblyBuffer[i+5] == 'L') &&
						(assemblyBuffer[i+6] == 'D') &&
						(assemblyBuffer[i+7] == ':'))
					{
						AT_Event.cmd = HF_AT_USR_CALL_HOLD;
								Handle_Rx_CHLD(&address,(u_int8*)(assemblyBuffer+8));
					}
				}
				break;
				
			case 'B' : // Bluetooth Specific Result Code
				switch(assemblyBuffer[i+4])
				{
				case 'R' :/****** +BRVA/+BRSF ***********/
					if ((assemblyBuffer[i+5] == 'V') &&
						(assemblyBuffer[i+6] == 'A'))
					{
						if (((assemblyBuffer[i+7] == ':') &&
							(assemblyBuffer[i+9] == '\r') &&
							(assemblyBuffer[i+8] == '0')) ||
							(assemblyBuffer[i+8] == '1'))
						{
							AT_Event.cmd = HF_AT_USR_BT_VOICE_REC_ACT;
							AT_Event.n_value = (assemblyBuffer[i+8] - '0');
							Handle_Rx_BRVA(&address,&AT_Event);
						}
					}
					else if ((assemblyBuffer[i+5] == 'S') &&
							(assemblyBuffer[i+6] == 'F'))
					{
						if (assemblyBuffer[i+7] == ':')
						{	
							u_int32 value32bit;
							value32bit = HFP_BT_READ_LE_UINT32((assemblyBuffer+i+8));
							AT_Event.cmd = HF_AT_USR_BT_RET_SUPPORTED_FEATURES;
							Handle_Rx_BRSF(&address,value32bit );
						}
					}
					break;
						
				case 'S' : /****** +BSIR **********/
					if ((assemblyBuffer[i+5] == 'I') &&
						(assemblyBuffer[i+6] == 'R'))
					{
						if (((assemblyBuffer[i+7] == ':') &&
							(assemblyBuffer[i+9] == '\r') &&
							(assemblyBuffer[i+8] == '0') )||
							(assemblyBuffer[i+8] == '1'))
						{
								
							AT_Event.cmd = HF_AT_USR_INBAND_RING_TONE;
							AT_Event.n_value = (assemblyBuffer[i+8] - '0');
							Handle_Rx_BSIR(&address,&AT_Event);

						}
					}
					break;
						
				case 'I' : /******** +BINP *************/
					if ((assemblyBuffer[i+5] == 'N') &&
						(assemblyBuffer[i+6] == 'P'))
					{
						if ((assemblyBuffer[i+7] == ':') )
						{
							AT_Event.p_service = assemblyBuffer+i+8;
							Handle_Rx_BINP(&address,&AT_Event);

						}
					}
					break;
				default :
					AT_Event.cmd = HF_AT_UNKNOWN_COMMAND;
					break;
						
				}
				break;
					
			case 'V' :
				switch(assemblyBuffer[i+4])
				{
				case 'G' :
					if ((assemblyBuffer[i+5])== 'S')
					{
						if (((assemblyBuffer[i+6])== ':') ||
							((assemblyBuffer[i+6])== '='))
						{
							int volume_value = 0;
							AT_Event.n_value = assemblyBuffer[i+8];
							AT_Event.cmd = HF_AT_USR_BT_SPEAKER_GAIN;
									while(assemblyBuffer[i+7]<'0')
										i++;
										
									if ((assemblyBuffer[i+7] != ' ') && 
										(assemblyBuffer[i+7] >= '0') && 
										(assemblyBuffer[i+7] <= '9'))
									{
										volume_value = (assemblyBuffer[i+7]-'0');
									}
									else
									{
										return 0;
									}

									if ((assemblyBuffer[i+8] != '\r') && 
										((assemblyBuffer[i+8] != ' ') && (assemblyBuffer[i+8] >= '0') && (assemblyBuffer[i+8] <= '9')))
									{
										volume_value = (volume_value*10)+((assemblyBuffer[i+8]-'0') );
									}

#if HFP_CORE_DEBUG	
									if (volume_value > 15)
										printf("\n\r Illegal Volume Value Rxed = %x", volume_value);
#endif

#if COMBINED_HOST==0
									if (HF_callbacks.HF_CB_Volume_Setting_Change_Req)
										(*HF_callbacks.HF_CB_Volume_Setting_Change_Req)(address,HFP_SPEAKER,volume_value);
#endif   

								}
							}
							else if ((assemblyBuffer[i+5])== 'M')
							{
								if (((assemblyBuffer[i+6])== ':') ||
									((assemblyBuffer[i+6])== '='))
								{
									int volume_value = 0;

									AT_Event.n_value = assemblyBuffer[i+8];
									AT_Event.cmd = HF_AT_USR_BT_MICROPONE_GAIN;
									
									while(assemblyBuffer[i+7]<'0')
										i++;

									if ((assemblyBuffer[i+7] != ' ') && 
										(assemblyBuffer[i+7] >= '0') && 
										(assemblyBuffer[i+7] <= '9'))
									{
										volume_value = (assemblyBuffer[i+7]-'0');
									}
									else
									{
										return 0;
									}

									if ((assemblyBuffer[i+8] != '\r') && 
										((assemblyBuffer[i+8] != ' ') && (assemblyBuffer[i+8] >= '0') && (assemblyBuffer[i+8] <= '9')))
									{
										volume_value = (volume_value*10)+((assemblyBuffer[i+8]-'0') );
									}
#if HFP_CORE_DEBUG	
									if (volume_value > 15)
										printf("\n\r Illegal Volume Value Rxed = %x", volume_value);
#endif

#if COMBINED_HOST==0
                                                                if (HF_callbacks.HF_CB_Volume_Setting_Change_Req)
										(*HF_callbacks.HF_CB_Volume_Setting_Change_Req)(address,HFP_MICROPHONE,volume_value);
#endif
								}
							}
							break;
							
						default :
							break;
					}
					break;

				default :
					break;
			} // End of switch (assemblyBuffer[i+3]) 
		} //  End of "else if (assemblyBuffer[i+2] == '+')" 
		else if (assemblyBuffer[i+2]=='R')
		{
			if ((assemblyBuffer[i+3]=='I') &&
				(assemblyBuffer[i+4]=='N') &&
				(assemblyBuffer[i+5]=='G'))
			{
				Handle_Rx_RING(&address);
			}
		}

	} // End of "if ((assemblyBuffer[i] == '\r') && (assemblyBuffer[i+1] == '\n'))"
	
	return 1;
}


/******************************************************
 * Function :- Handle_Rx_BRSF
 *
 * Description 
 * Handles Receipt of a BRSF from the peer. Stores the 
 * supported features locally
 * 
 ******************************************************/

void Handle_Rx_BRSF(t_bdaddr* address, u_int32 supportedFeatures)
{
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(address);

	if (peer_device != 0)
	{
		if(peer_device->scm_state == SCM_W4_SUPPORTED_FEATURES)
		{
			peer_device->scm_state = SCM_W4_SUPPORTED_FEATURES_OK;
		}
		peer_device->supportedFeatures = supportedFeatures;
	}
	return;
}

/******************************************************
 * Function :- Handle_Rx_CIEV
 *
 * Description 
 * Handles Receipt of a CIEV. Calls back the application.
 * 
 ******************************************************/

void Handle_Rx_CIEV(t_bdaddr* address,t_AT_Event* pAT_Event)
{
	t_HFP_AG_Peer_Device* peer_device;
	u_int8 ind_value = 0;
	u_int8 ind_index = 0;
	u_int8 inds_updated = 0;
	u_int8 len = 0;
	u_int8 i=0;
	u_int8* p_data = pAT_Event->p_service;

	peer_device = Find_Audio_GW_By_Address(address);
	if (!peer_device)
	{
		return;
	}

	/*
	 *  First determine what the indicator is.
	 */ 

	while(p_data[len] != '\r')
		len++;


	while(((p_data[i] == ' ') || (p_data[i] == '(') || (p_data[i] == ')') || (p_data[i] == ':') ||  (p_data[i] == ',') ) && (i<len))
		i++;

	if ((p_data[i] >= '0') && (p_data[i] <= '9'))
	{
		ind_index = (u_int8)(p_data[i]-'0');
		i++;
	}
	else
	{
		return;
	}


	while(((p_data[i] == ' ') || (p_data[i] == '(') || (p_data[i] == ')') || (p_data[i] == ':') ||  (p_data[i] == ',') ) && (i<len))
		i++;

	if((p_data[i] >= '0') && (p_data[i] <= '9'))
	{
		ind_value = (u_int8)(p_data[i]-'0');
	}
	else
	{
		return;
	}

	if (ind_index == peer_device->service_ind_order)
	{
		inds_updated = 1;		
		peer_device->service_ind = ind_value;
       }
	else if (ind_index == peer_device->call_ind_order)
	{	
		peer_device->call_ind = ind_value;
		if (ind_value == 1)
		{
			inds_updated = 1;
			peer_device->call_state = HFP_CALL_ACTIVE;
			/* Callback the App with CB_Call_Active */  //after accept the call.
			/*
				Is ther anything need  to tell the APP? ??????????
			*/
#if COMBINED_HOST==0			
			if (HF_callbacks.HF_CB_Call_Active)
				(*HF_callbacks.HF_CB_Call_Active)(peer_device->address);
#endif            
		}
		else if (ind_value == 0)
		{
			u_int8 release_reason=0;

			inds_updated=1;
			{
				if (peer_device->call_state == HFP_LOCAL_USER_INIT_CALL_REJECT)
				{
					release_reason = 0; // User Init Call Reject 
				}
				else if (peer_device->call_state == HFP_LOCAL_USER_INIT_CALL_RELEASE)
				{
					release_reason = 1; // User Init Call Release 
				}
				else if (peer_device->call_state == HFP_CALL_ACTIVE)
				{
					release_reason = 3; // Peer Init Call Release
				}
				else if (peer_device->call_state != HFP_CALL_CLOSED)
				{
					release_reason = 2; // Peer Init Call Reject
				}
			}
			
			if (peer_device->call_state != HFP_CALL_CLOSED)
			{
				peer_device->call_state = HFP_CALL_CLOSED;
				/* Callback the App with CB_Release_Complete */
				/*
					Is there anything need  to tell the APP? ??????????
				*/
#if COMBINED_HOST==0
				if (HF_callbacks.HF_CB_Call_Release_Complete)
					(*HF_callbacks.HF_CB_Call_Release_Complete)(peer_device->address,release_reason,BT_NOERROR);
#endif
			}

		}

	}
	else if (ind_index == peer_device->call_setup_ind_order)  //incoming/outgoing call setup ongoing indicator 
	{
		inds_updated=1;
		peer_device->call_setup_ind = ind_value;
	}

	/* Callback the application to indicate that an indicator has been updated */
#if COMBINED_HOST==0    
	if ((inds_updated) && (HF_callbacks.HF_CB_Phone_Status_Ind))
		(*HF_callbacks.HF_CB_Phone_Status_Ind)(peer_device->address, peer_device->service_ind,
												peer_device->call_ind, peer_device->call_setup_ind);
#endif    
	/*if (inds_updated)
	{
		HF_CB_Phone_Status_Ind(peer_device->address, peer_device->service_ind, 
									peer_device->call_ind, peer_device->call_setup_ind);
	}*/

}

/******************************************************
 * Function :- Handle_Rx_BSIR
 *
 * Description 
 * Handles Receipt of a BSIR, which informs the HF unit of
 * the status of inband tone generation in the AG.
 * 
 ******************************************************/

static void Handle_Rx_BSIR(t_bdaddr* address,t_AT_Event* pAT_Event)
{
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(address);
	if (!peer_device)
	{
		return;
	}

	/* Callback the application */

	if (pAT_Event->n_value==1)
		peer_device->remote_feature_status |= HF_FEATURE_INBAND_TONE;
	else if (pAT_Event->n_value==0)
		peer_device->remote_feature_status &= ~HF_FEATURE_INBAND_TONE;

#if COMBINED_HOST==0
	if (HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)
	    (*HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)(peer_device->address,peer_device->remote_feature_status, BT_NOERROR);
#endif
    
	HF_CB_Remote_Supplementary_Feature_Update(peer_device->address,peer_device->remote_feature_status, BT_NOERROR);

}

/******************************************************
 * Function :- Handle_Rx_BRVA
 *
 * Description 
 * Handles Receipt of a +BRVA, which informs the HF unit of
 * the status of voice recognition in the AG.
 * 
 ******************************************************/

static void Handle_Rx_BRVA(t_bdaddr* address,t_AT_Event* pAT_Event)
{
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(address);
	if (!peer_device)
	{
		return;
	}

	/* Callback the application */

	if (pAT_Event->n_value==1)
		peer_device->remote_feature_status |= HF_FEATURE_VR;
	else if (pAT_Event->n_value==0)
		peer_device->remote_feature_status &= ~HF_FEATURE_VR;

#if COMBINED_HOST==0
	if (HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)
	    (*HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)(peer_device->address,peer_device->remote_feature_status, BT_NOERROR);
#endif    
	HF_CB_Remote_Supplementary_Feature_Update(peer_device->address,peer_device->remote_feature_status, BT_NOERROR);

}
/******************************************************
 * Function :- Handle_Rx_RING
 *
 * Description 
 * Handles Receipt of a RING. Calls back the application.
 ******************************************************/

void Handle_Rx_RING(t_bdaddr* bd_addr)
{
	t_HFP_AG_Peer_Device* peer_device;
	int i=0;
	
	peer_device = Find_Audio_GW_By_Address(bd_addr);
	if (peer_device)
	{
		/* Callback the application */
		if (peer_device->call_state != HFP_LOCAL_USER_INIT_CALL_REJECT)
		{
			peer_device->call_state = HFP_CALL_W4_USER_ACCEPT;
			for (i=0;i<6;i++) 
				gCurrent_Active_Addr.bytes[i]=peer_device->address.bytes[i];
#if COMBINED_HOST==0            
			if (HF_callbacks.HF_CB_Ring_Indication)
				(*HF_callbacks.HF_CB_Ring_Indication)(peer_device->address);
#endif            
			//HF_CB_Ring_Indication(peer_device->address);
			HF_Call_Accept(gCurrent_Active_Addr);
		}
	}
}

/******************************************************
 * Function :- Handle_Rx_CCWA
 *
 * Description 
 * Handles Receipt of a +CCWA. Extracts the number info
 * and calls back the application.
 *
 * For CCWA the format is +CCWA : "ddddddd", 129, 129
 ******************************************************/
void Handle_Rx_CCWA(t_bdaddr* address,t_AT_Event* pAT_Event)
{
	t_HFP_AG_Peer_Device* peer_device;
	int start_num_offset=0;
	int end_num_offset=0;
	int len=0;
	int i=0;
	char* p_data = pAT_Event->p_service;
	
	while(p_data[len] != '\r')
		len++;

	peer_device = Find_Audio_GW_By_Address(address);
	if (peer_device)
	{
		/* Skip until quote */
		while(((p_data[i] != '"') && (p_data[i] != '\r')) && (i<len))
			i++;

		/* increment over quote */
		i++;
        
		start_num_offset=i;

		while(((p_data[i] != '"') && (p_data[i] != '\r')) && (i<len))
			i++;

		end_num_offset=i;
#if COMBINED_HOST==0
		if (HF_callbacks.HF_CB_Call_Waiting)
			(*HF_callbacks.HF_CB_Call_Waiting)(peer_device->address,(p_data+start_num_offset),(end_num_offset-start_num_offset));
#endif
	}
}


/******************************************************
 * Function :- Handle_Rx_BINP
 *
 * Description 
 * Handles Receipt of a +BINP. Extracts the number info
 * and calls back the application.
 *
 * For +BINP the format is +BINP : "ddddddd"
 ******************************************************/
void Handle_Rx_BINP(t_bdaddr* address,t_AT_Event* pAT_Event)
{
	t_HFP_AG_Peer_Device* peer_device;
	int start_num_offset=0;
	int end_num_offset=0;
	int len=0;
	int i=0;
	char* p_data = pAT_Event->p_service;
	
	while(p_data[len] != '\r')
		len++;

	peer_device = Find_Audio_GW_By_Address(address);
	if (peer_device)
	{

		/* Skip until qoute */
		while(((p_data[i] != '"') && (p_data[i] != '\r')) && (i<len))
			i++;

		/* increment over quote */
		i++;
        
		start_num_offset=i;

		while(((p_data[i] != '"') && (p_data[i] != '\r')) && (i<len))
			i++;

		end_num_offset=i;

#if HFP_CORE_DEBUG	
		printf("\n Recieved Input from AG :- ");

		for(i=0;i<(end_num_offset-start_num_offset);i++)
			printf("%c",p_data[start_num_offset+i]);
 #endif       
	}
}


/******************************************************
 * Function :- Handle_Rx_CLIP
 *
 * Description 
 * Handles Receipt of a +CLIP. Extracts the CLIP info
 * and calls back the application.
 *
 * For Clip the format is +CLIP : "ddddddd", 129, 129
 ******************************************************/

void Handle_Rx_CLIP(t_bdaddr* address,t_AT_Event* pAT_Event)
{
	t_HFP_AG_Peer_Device* peer_device;
	int start_num_offset=0;
	int end_num_offset=0;
	int len=0;
	int i=0;
	char* p_data = pAT_Event->p_service;
	
	while(p_data[len] != '\r')
		len++;

	peer_device = Find_Audio_GW_By_Address(address);
	if (peer_device)
	{

		/* Skip until qoute */
		while(((p_data[i] != '"') && (p_data[i] != '\r')) && (i<len))
			i++;

		/* increment over quote */
		i++;
        
		start_num_offset=i;

		while(((p_data[i] != '"') && (p_data[i] != '\r')) && (i<len))
			i++;

		end_num_offset=i;
        
#if COMBINED_HOST==0
		if (HF_callbacks.HF_CB_Clip_Indication)
			(*HF_callbacks.HF_CB_Clip_Indication)(peer_device->address,(p_data+start_num_offset),(end_num_offset-start_num_offset));
#endif        
	}
}

			


/******************************************************
 * Function :- Handle_Rx_CIND
 *
 * Description 
 * Handles Receipt of a +CIND. The response to the Test and normal
 * command are treated differently. Both message types are only
 * recieved during call establishment.
 ******************************************************/

void Handle_Rx_CIND(t_bdaddr* address,t_AT_Event* pAT_Event)
{
	
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(address);
	if (peer_device)
	{
		switch(peer_device->operation_pending)
		{
			case AT_CIND_TEST :
				if (peer_device->scm_state == SCM_W4_INDICATOR_CONTROL)
				{
					peer_device->scm_state = SCM_W4_INDICATOR_CONTROL_OK;
					HF_Extract_Service_List(pAT_Event->p_service, peer_device);
				}
				break;
				
			case AT_CIND :
				if (peer_device->scm_state == SCM_W4_INDICATOR_STATUS)
				{
					peer_device->scm_state = SCM_W4_INDICATOR_STATUS_OK;
					HF_Update_Indicator_Status(pAT_Event->p_service,peer_device);	
				}
				break;
			
			default:
				HF_Update_Indicator_Status(pAT_Event->p_service,peer_device);	
				break;

		}
	}
}

/******************************************************
 * Function :- Handle_Rx_CHLD
 *
 * Description 
 * Handles Receipt of a +CHLD by extracting the call hold
 * information and transitioning state, if received in Call setup.
 ******************************************************/

void Handle_Rx_CHLD(t_bdaddr* address,u_int8* p_data)
{
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(address);
	if (peer_device)
	{
		peer_device->call_handling = HF_Extract_Call_Handling_Info(p_data);
		
		if (peer_device->scm_state == SCM_W4_CALL_HOLD_INFO)
			peer_device->scm_state = SCM_W4_CALL_HOLD_INFO_OK;
	}

}

/******************************************************
 * Function :- Handle_OK_Command
 *
 * Description 
 * Handles Receipt of a OK command. Action to be performed is
 * dependent on the operation pending and the current SCM state.
 ******************************************************/
void Handle_OK_Command(t_bdaddr* address,u_int8 dlci)
{
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(address);
    if (!peer_device)
        return;

    switch(peer_device->operation_pending)
    {

        case AT_BRSF : 
            if (peer_device->scm_state == SCM_W4_SUPPORTED_FEATURES_OK)
            {
                peer_device->scm_state = SCM_W4_INDICATOR_CONTROL;
                HF_Retrieve_Supported_Indicators(&peer_device->address,peer_device->dlci);
            }
        break;

        case AT_CIND_TEST :
            if (peer_device->scm_state == SCM_W4_INDICATOR_CONTROL_OK)
            {
                peer_device->scm_state = SCM_W4_INDICATOR_STATUS;
                HF_Retrieve_Indicator_Status(address,dlci);
            }
        break;

        case AT_CIND :
            if (peer_device->scm_state == SCM_W4_INDICATOR_STATUS_OK)
            {
                peer_device->scm_state = SCM_W4_ENABLE_INDICATOR_STATUS_UPDATE_OK;
                HF_Activate_Indicators_Event_Reporting(address,dlci);
            }
        break;

        case AT_CMER :
            if (peer_device->scm_state == SCM_W4_ENABLE_INDICATOR_STATUS_UPDATE_OK)
            {
#if 0 //(MULTIPARTY_CALLS_SUPPORTED==1)
                peer_device->scm_state = SCM_W4_CALL_HOLD_INFO;
                HF_Send_Call_Hold_Test_Command(&peer_device->address,peer_device->dlci);
#endif
            }
        break;

        case AT_CHLD_TEST :
            if (peer_device->scm_state == SCM_W4_CALL_HOLD_INFO_OK)
            {
                peer_device->scm_state = SCM_OPEN;
                HF_Activate_CLIP(&peer_device->address,peer_device->dlci);
                /* If an Audio Establishment is pending - then set it up */

                if (peer_device->audio_state == HFP_AUDIO_SETUP_PENDING)
                {
                    HF_Audio_Setup(peer_device->address);
                }

                // @@@ Cancel Timer
#if COMBINED_HOST==1
                host_timer_cancel(peer_device->slc_setup_timer_handle);
#endif
                peer_device->slc_setup_timer_handle = 0;

                /* Callback the Application to indicate the SLC is complete */
#if COMBINED_HOST==0    
                pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"Handle_OK_Command"));
                if (HF_callbacks.HF_CB_SLC_Established_Ind)
                	(*HF_callbacks.HF_CB_SLC_Established_Ind)(peer_device->address, BT_NOERROR);
#endif                
            }
        break;

        case AT_CHLD :
            if(peer_device->scm_state == SCM_OPEN)
            {
#if COMBINED_HOST==0                
                if (HF_callbacks.HF_CB_Command_Complete)
                	(*HF_callbacks.HF_CB_Command_Complete)(peer_device->address,HFP_MULTIP_PARTY_API,BT_NOERROR);
#endif
            }
        break;

        case ATD :
        case AT_BLDN :
            if (peer_device->call_state == HFP_CALL_W4_ADT_OK)
            {
#if 0 // Only if CallSetup Ind supported by peer !! Current 50% of devices dont support this indicator
                // so for now we are not dependent on it.
                peer_device->call_state = HFP_CALL_W4_CIEV_CALL_SETUP_2;
#else
                peer_device->call_state = HFP_CALL_W4_CIEV_CALL_1;
#endif

#if COMBINED_HOST==0
                if (HF_callbacks.HF_CB_Command_Complete)
                	(*HF_callbacks.HF_CB_Command_Complete)(peer_device->address,HFP_CALL_REQUEST_API, BT_NOERROR);
#endif                

                /* Inform the user that the peer is being alerted */
#if COMBINED_HOST==0
                if (HF_callbacks.HF_CB_Call_Alerting)
                	(*HF_callbacks.HF_CB_Call_Alerting)(peer_device->address);
#endif
                HF_CB_Call_Alerting(peer_device->address);
            }
        break;

        case AT_VTS :
            if ((peer_device->scm_state == SCM_OPEN) && (peer_device->call_state == HFP_CALL_ACTIVE))
            {
                /* If not at the end of a DTMF string, send next DTMF tone */
                if (g_hfp_dtmf_offset != (g_hfp_dtmf_len-1))
                {
                /* increment the offset and send the Tone */
                    g_hfp_dtmf_offset++;
                    HF_Send_DTMF_Tone(address, dlci,g_hfp_dtmf_string[g_hfp_dtmf_offset]);
                }
                else
                {
                    /* Callback the Application to indicate DTMF string complete */
#if COMBINED_HOST==0      
                    if (HF_callbacks.HF_CB_Send_DTMF_Complete)
                        (*HF_callbacks.HF_CB_Send_DTMF_Complete)(peer_device->address,BT_NOERROR);
#endif
                }
            }
        break;

        case AT_CLIP_1 :
            if (peer_device)
            {
                /* CLIP-Activated */
                peer_device->remote_feature_status |= HF_FEATURE_CLIP;
                
#if COMBINED_HOST==0
               if (HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)
                	(*HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)(peer_device->address,peer_device->remote_feature_status,BT_NOERROR);
#endif
                HF_CB_Remote_Supplementary_Feature_Update(peer_device->address,peer_device->remote_feature_status, BT_NOERROR);
            }
        break;


        case AT_CLIP_0 :
            if (peer_device)
            {
                /* CLIP-Activated */
                peer_device->remote_feature_status &= ~HF_FEATURE_CLIP;
#if COMBINED_HOST==0
                if (HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)
                    (*HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)(peer_device->address,peer_device->remote_feature_status,BT_NOERROR);
#endif
                HF_CB_Remote_Supplementary_Feature_Update(peer_device->address,peer_device->remote_feature_status, BT_NOERROR);
            }
        break;


        case AT_CCWA_1 :
            if (peer_device)
            {
                /* CCWA-Activated */
                peer_device->remote_feature_status |= HF_FEATURE_CALL_WAIT;
#if COMBINED_HOST==0
                if (HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)
                	(*HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)(peer_device->address,peer_device->remote_feature_status,BT_NOERROR);
#endif
                HF_CB_Remote_Supplementary_Feature_Update(peer_device->address,peer_device->remote_feature_status, BT_NOERROR);
            }
        break;


        case AT_CCWA_0 :
        if (peer_device)
            {
                /* CCWA-Activated */
                peer_device->remote_feature_status &= ~HF_FEATURE_CALL_WAIT;
#if COMBINED_HOST==0
                if (HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)
                	(*HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)(peer_device->address,peer_device->remote_feature_status,BT_NOERROR);
#endif
                HF_CB_Remote_Supplementary_Feature_Update(peer_device->address,peer_device->remote_feature_status, BT_NOERROR);
            }
        break;

        case AT_BRVA_1 :
            if (peer_device)
            {
                /* Voice Recognition-Activated */
                peer_device->remote_feature_status |= HF_FEATURE_VR; 
#if COMBINED_HOST==0    
                if (HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)
                	(*HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)(peer_device->address,peer_device->remote_feature_status,BT_NOERROR);
#endif
                HF_CB_Remote_Supplementary_Feature_Update(peer_device->address,peer_device->remote_feature_status, BT_NOERROR);
            }
        break;

        case AT_BRVA_0 :
            if (peer_device)
            {
                /* Voice Recognition-Activated */
                peer_device->remote_feature_status &= ~HF_FEATURE_VR; 
#if COMBINED_HOST==0
                if (HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)
                	(*HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)(peer_device->address,peer_device->remote_feature_status,BT_NOERROR);
#endif
                HF_CB_Remote_Supplementary_Feature_Update(peer_device->address,peer_device->remote_feature_status,BT_NOERROR);
            }
        break;


        case AT_NREC_0 :
            if (peer_device)
            {
                /* Noise Reduction and echo cancellation */
                peer_device->remote_feature_status &= HF_FEATURE_EC_NR; 
#if COMBINED_HOST==0
                if (HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)
                	(*HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)(peer_device->address,peer_device->remote_feature_status,BT_NOERROR);
#endif
                HF_CB_Remote_Supplementary_Feature_Update(peer_device->address,peer_device->remote_feature_status, BT_NOERROR);
            }
        break;

        case AT_CHUP :
            if (peer_device)
            {
                // GF :- 16 Jan 2005 was bug here
                //peer_device->call_state = HFP_CALL_CLOSED;

                //  Note : Call_Release_Complete will be called Rx of +CIEV with (call=0)
#if COMBINED_HOST==0
                if(HF_callbacks.HF_CB_Call_Release_Complete)
                		(*HF_callbacks.HF_CB_Call_Release_Complete)(peer_device->address,0,0x0);

                if (peer_device->call_state == HFP_LOCAL_USER_INIT_CALL_REJECT)
                {
                    if(HF_callbacks.HF_CB_Command_Complete)
                        (*HF_callbacks.HF_CB_Command_Complete)(peer_device->address,HFP_CALL_REJECT_API,BT_NOERROR);
                }
                else if (peer_device->call_state == HFP_LOCAL_USER_INIT_CALL_RELEASE)
                {
                    if(HF_callbacks.HF_CB_Command_Complete)
                        (*HF_callbacks.HF_CB_Command_Complete)(peer_device->address,HFP_CALL_RELEASE_API,BT_NOERROR);
                }
#endif
            }
        break;



        case AT_BINP :
            if (peer_device)
            {
                if (g_num_mem_locs)
                HF_Send_BINP(address, dlci);
            }
        break;

        case AT_ANSWER :
            if (peer_device->call_state == HFP_W4_PEER_CALL_CONNECT)
            {
                /* Send a command complete to the user for the call accept */
#if COMBINED_HOST==0
                if(HF_callbacks.HF_CB_Command_Complete)
                	(*HF_callbacks.HF_CB_Command_Complete)(peer_device->address,HFP_CALL_ACCEPT_API,BT_NOERROR);
#endif
            }
        break;

        case AT_VGM :
        case AT_VGS :
            if (peer_device)
            {
#if COMBINED_HOST==0
                if(HF_callbacks.HF_CB_Change_Volume_Complete)
                	(*HF_callbacks.HF_CB_Change_Volume_Complete)(peer_device->address /*BT_NOERROR*/);
#endif
            }
        break;

        default :
            break;

    }
      
}


void Handle_Rx_AT_CKPD(t_bdaddr* address,u_int16 dlci)
{
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(address);
#if HFP_CORE_DEBUG	
	printf("\n\r Printf Button press from Headset\n");
#endif

	/* The actions performed on a button press from the headset 
	 * are dependent on the current state of the link to the headset.
	 * if NO Audio link exists to the headset
	 *      then an Audio link is established. 
	 * if an Audio link exists to the headset
	 *      then it is released.
	 */

	if (peer_device)
	{		
		HF_Send_OK(address,dlci);
		/* Cancel the Ring timer 
		* and setup an Audio Link
		*/
		if (peer_device->ring_timer)
		{
#if COMBINED_HOST==1
			host_timer_cancel(peer_device->ring_timer);
#endif
			peer_device->ring_timer = 0;
		}

		if (peer_device->audio_state == HFP_AUDIO_CLOSED)
		{
#if COMBINED_HOST==0
			if (HF_callbacks.HF_CB_Audio_Request)
				(*HF_callbacks.HF_CB_Audio_Request)(peer_device->address);
#endif

//			HF_Audio_Setup((*address));
		}
		else if (peer_device->audio_state != HFP_AUDIO_CLOSED)
		{
			HF_Audio_Release((*address));
		}

	}

}

static void Handle_Rx_ATA(t_bdaddr* address,u_int16 dlci)
{
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(address);

	if (peer_device)
	{		

		peer_device->call_state = HFP_CALL_ANSWERED;

		/* TBD :- May need to interact with the Application
		 * or User prior to proceeding with sending the OK
		 * command in response to the ATA (ANSWER)
		 */

		HF_Send_OK(address,dlci);
		/* Cancel the Ring timer 
		* and setup an Audio Link
		*/
		if (peer_device->ring_timer)
		{
#if COMBINED_HOST==1
			host_timer_cancel(peer_device->ring_timer);
#else
			pTimerCancel(peer_device->ring_timer);   // added by cliff
			peer_device->ring_timer=0;
#endif
			peer_device->ring_timer = 0;
		}
        
		// Send +CIEV:(call=1)
		HF_Send_Indicators(HFP_CALL_INDICATOR,1,address,dlci);

		// Send +CIEV:(callsetup=0)
		HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,0,address,dlci);
             g_call_answered_by_hf = TRUE;
		peer_device->call_state = HFP_CALL_ACTIVE;

		 //((peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE_AG) &&*/
		//if	( peer_device->inband_tone == 0 )
		{

			//HF_Audio_Setup(peer_device->address);
			//hfp_cb.hfpSendRingConf();//syzhou added
			rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_RING_CNF,NULL);
		}
					 

	}
}

static void Handle_Rx_AT_CMER(t_bdaddr* address,u_int16 dlci,u_int8* buf_ptr)
{

	int i=0;
    /* The AT+CMER command can only have one of the following formats 
	 *    1/  AT+CMER=3,0,0,1   -- Activate event reporting
	 *    2/  AT+CMER=3,0,0,0   -- DeActivate event reporting
     */

	while((buf_ptr[i] != '3') && (buf_ptr[i] != '\r'))
		i++;

	if (buf_ptr[i] != '3')
		return;

	while((buf_ptr[i] != '0') && (buf_ptr[i] != '\r'))
		i++;
	
	if(buf_ptr[i] != '0')
		return;

	while((buf_ptr[i] != '0') && (buf_ptr[i] != '\r'))
		i++;
	
	if(buf_ptr[i] != '0')
		return;

	while((buf_ptr[i] != '0') && (buf_ptr[i] != '1') && (buf_ptr[i] != '\r'))
		i++;
	
	if(buf_ptr[i] == '0')
	{
        // Activate Indicators event reporting
	}
	else if (buf_ptr[i] == '1')
	{
        // DeActivate Indicators event reporting
	}
	
	return;
}

/******************************************************
 * Function :- Handle_Error_Command
 *
 * Description 
 * Handles Receipt of a Error command. Action to be performed is
 * dependent on the scm_state and the current SCM state.
 ******************************************************/
static void Handle_Error_Command(t_bdaddr* address,u_int8 dlci)
 {
	 t_HFP_AG_Peer_Device* peer_device;
	 
	 peer_device = Find_Audio_GW_By_Address(address);
	 if (!peer_device)
		 return;
	 
	 switch(peer_device->scm_state)
	 {
	 case SCM_W4_SUPPORTED_FEATURES_OK : 
	 case SCM_W4_SUPPORTED_FEATURES : 
		 // As Supported features are not essential assume peer does not support command
		 // and advance to the INDICATOR_CONTROL
		 
		 peer_device->scm_state = SCM_W4_INDICATOR_CONTROL;
		 HF_Retrieve_Supported_Indicators(&peer_device->address,peer_device->dlci);
		 
		 break;
		 
	 case SCM_W4_INDICATOR_CONTROL :
	 case SCM_W4_INDICATOR_CONTROL_OK :
	 case SCM_W4_INDICATOR_STATUS :
	 case SCM_W4_INDICATOR_STATUS_OK :
	 case SCM_W4_ENABLE_INDICATOR_STATUS_UPDATE_OK :
		 
		 // Fail the SLC Setup
		// @@@ Cancel SLC Timer
#if COMBINED_HOST==1
		host_timer_cancel(peer_device->slc_setup_timer_handle);
#endif
		peer_device->slc_setup_timer_handle = 0;
#if COMBINED_HOST==0
		if (HF_callbacks.HF_CB_SLC_Established_Ind)
			(*HF_callbacks.HF_CB_SLC_Established_Ind)(peer_device->address,BT_UNKNOWNERROR);		
#endif
		 break;
		 
	 case SCM_W4_CALL_HOLD_INFO :
	 case SCM_W4_CALL_HOLD_INFO_OK :
		 
		 // Device does not repond correctly to multiparty call handling 
		 // Ignore and proceed with CLIP activation 
		 
		 peer_device->scm_state = SCM_OPEN;
		 HF_Activate_CLIP(&peer_device->address,peer_device->dlci);
		 /* If an Audio Establishment is pending - then set it up */
		 
		 if (peer_device->audio_state == HFP_AUDIO_SETUP_PENDING)
		 {
			 HF_Audio_Setup(peer_device->address);
		 }
		 
		 /* Callback the Application to indicate the SLC is complete */
		// @@@ Cancel SLC Timer
#if COMBINED_HOST==1
		host_timer_cancel(peer_device->slc_setup_timer_handle);
#endif
		peer_device->slc_setup_timer_handle = 0;

#if COMBINED_HOST==0
                pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"Handle_Error_Command"));
		 if (HF_callbacks.HF_CB_SLC_Established_Ind)
			 (*HF_callbacks.HF_CB_SLC_Established_Ind)(peer_device->address,BT_NOERROR);
#endif

		 break;
		 
	 case SCM_OPEN :
		 {
			 switch(peer_device->operation_pending)
			 {
			 case AT_CHLD :
#if COMBINED_HOST==0
        			 if (HF_callbacks.HF_CB_Command_Complete)
					 (*HF_callbacks.HF_CB_Command_Complete)(peer_device->address,HFP_MULTIP_PARTY_API,BT_UNKNOWNERROR);
#endif
				 break;
				 
			 case ATD :
			 case AT_BLDN :
				 // Release any local information associated with the Call 
				 //
				 peer_device->call_state = HFP_CALL_CLOSED;
#if COMBINED_HOST==0
				 if (HF_callbacks.HF_CB_Command_Complete)
					 (*HF_callbacks.HF_CB_Command_Complete)(peer_device->address,HFP_CALL_REQUEST_API,BT_UNKNOWNERROR);
#endif

#if 0 // No need to send Call Release CB - as the Command Complete with error should inform user that call failed.
				 // Send Call Hung up Information 
		 
				 if(HF_callbacks.HF_CB_Call_Release_Complete)
					 (*HF_callbacks.HF_CB_Call_Release_Complete)(peer_device->address,0x02 /*release_reason :- Peer rejected */,BT_UNKNOWNERROR);				
#endif
				 break;
				 
			 case AT_VTS :
				 
				 /* Callback the Application to indicate DTMF string complete */
#if COMBINED_HOST==0
				 if (HF_callbacks.HF_CB_Send_DTMF_Complete)
					 (*HF_callbacks.HF_CB_Send_DTMF_Complete)(peer_device->address,BT_UNKNOWNERROR);
#endif
        			break;
				 
			 case AT_CLIP_1 :
			 case AT_CLIP_0 :
			 case AT_CCWA_1 :
			 case AT_CCWA_0 :
			 case AT_BRVA_1 :
			 case AT_BRVA_0 :
			 case AT_NREC_0 :
#if COMBINED_HOST==0
				 if (HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)
					 (*HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update)(peer_device->address, peer_device->remote_feature_status,BT_UNKNOWNERROR);
#endif
				 break;
				 
			 case AT_CHUP :
				 {
#if COMBINED_HOST==0                
					 u_int8 release_reason=0;
					 if (peer_device->call_state == HFP_LOCAL_USER_INIT_CALL_REJECT)
					 {
						 release_reason = 0; // User Init Call Reject 
						 if(HF_callbacks.HF_CB_Command_Complete)
							 (*HF_callbacks.HF_CB_Command_Complete)(peer_device->address,HFP_CALL_REJECT_API,BT_UNKNOWNERROR);
						 
					 }
					 else if (peer_device->call_state == HFP_LOCAL_USER_INIT_CALL_RELEASE)
					 {
						 release_reason = 1; // User Init Call Release 
						 if(HF_callbacks.HF_CB_Command_Complete)
							 (*HF_callbacks.HF_CB_Command_Complete)(peer_device->address,HFP_CALL_RELEASE_API,BT_NOERROR);
						 
					 }
					 
					 if(HF_callbacks.HF_CB_Call_Release_Complete)
						 (*HF_callbacks.HF_CB_Call_Release_Complete)(peer_device->address,release_reason,BT_UNKNOWNERROR);
#endif
				 }
				 break;
				 
			 case AT_VGM :
			 case AT_VGS :
				 if (peer_device)
				 {
#if COMBINED_HOST==0
					 if(HF_callbacks.HF_CB_Change_Volume_Complete)
						 (*HF_callbacks.HF_CB_Change_Volume_Complete)(peer_device->address/*BT_UNKNOWNERROR*/);
#endif
				 }
				 break;
			 default :
				 break;
				 
			 }
		 }	 
		 break;
		 
	 }
 }



/************************************************************************
 *
 *  RFCOMM Callback Functions 
 *  
 *************************************************************************/


 /************************************************************************
 *
 *  Function :- HF_rfcommConnectConfirmCallback
 *
 *  Description :- 
 *  Indicates the completion of a locally initiated RFOMM connection.
 *  Results in the SCM establishment and the initiation of RFCOMM flow control.
 *************************************************************************/

int HF_rfcommConnectConfirmCallback(u_int8 serverChannel, t_bdaddr address, u_int8 status, u_int8 dlci, u_int16 max_frame_size) 
{	
    u_int16 cid;
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(&address);
    if(!peer_device)
        return 1;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_rfcommConnectConfirmCallback peer_device=0x%x status=%d\n",peer_device,status));
#endif  
    
    if (status==1) /* no error */ 
	{
            /* Get the Link Structure Corresponding to this Address */
            //peer_device = Find_Audio_GW_By_Address(&address);
            peer_device->dlci = dlci;
            peer_device->acl_state = HFP_DEVACTIVE;

            RSE_GetCIDFromConnection(address, peer_device->dlci,&cid);
            if (cid)
            {
                L2_RetrieveHandle(cid,&peer_device->acl_handle);
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_rfcommConnectConfirmCallback peer_device->acl_handle:0x%x\n",peer_device->acl_handle));
#endif  
            }

		if (peer_device && (peer_device->scm_state == SCM_W4_RFCOMM_CONNECT))
		{
#if (HFP_MULTIPOINT_SUPPORT==1)
			if (!peer_device->evHFP_TxOK)
			{
				//peer_device->evHFP_TxOK = pEventCreate(0);
			}

			peer_device->HFP_TxOK = 1;
#else
			if (!evHFP_TxOK)
			evHFP_TxOK=pEventCreate(0);
			HFP_TxOK=1;
#endif		
			/* 
			 * If handsfree profile then exchange the AT commands
			 * for Supported Features and/or Indicator control.
			 */ 
//----------------------------------------------
// GF 
//  TEST FUNCTIONALITY FOR MUTIPLE PROFILES
//
//----------------------------------------------
// GF  Thur 12 May   14:05
// XXXXX
			//peer_device->active_profile = ACTIVE_PROFILE_HANDSFREE_AG;

// End Test Code ----------------------------------

			if (peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE)
			{
				/* Only use this command for 1.01 devices and later   ?????*/  
				if ( peer_device->versionInfo == BT_HFP_PROTOCOL_VERSION_100 || 
					peer_device->versionInfo == BT_HFP_PROTOCOL_VERSION_105)
				{
					HF_Retrieve_Support_Features(&address,dlci);
					peer_device->scm_state = SCM_W4_SUPPORTED_FEATURES;
				}
				else
				{
					peer_device->scm_state = SCM_W4_INDICATOR_CONTROL;
					//HF_Retrieve_Supported_Indicators(&address,dlci);//syzhou removed it
				}

			}
			else if (peer_device->active_profile == ACTIVE_PROFILE_HEADSET_AG) 
			{
			/* 
			 * If Headset Profile then Connection setup is complete.
			 * As the Headset profile does not have direct equivalent
			 * to an SLC, the connection establishment is complete
			 * once the RFCOMM link is established.
			 */
#if COMBINED_HOST==1
				host_timer_cancel(peer_device->slc_setup_timer_handle);
#endif
				peer_device->slc_setup_timer_handle = 0;
				
				peer_device->scm_state = SCM_OPEN;
                
#if COMBINED_HOST==0
                            pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_rfcommConnectConfirmCallback"));
				if (HF_callbacks.HF_CB_SLC_Established_Ind)
					(*HF_callbacks.HF_CB_SLC_Established_Ind)(peer_device->address,BT_NOERROR);
#endif
				
			}
			else if (peer_device->active_profile == ACTIVE_PROFILE_HANDSFREE_AG)
			{
				// If we are the HandsFree AG we wait for the peer headset to
				// initiate the setup of the SLC
				//
				peer_device->scm_state = SCM_OPEN;
			}
		}

	}
	else
	{	
		if(status==0)/*error*/
		{
			peer_device->scm_state = SCM_CLOSED;
#if (HFP_MULTIPOINT_SUPPORT==1)
			///if (peer_device->evHFP_TxOK)
			//	pEventFree(peer_device->evHFP_TxOK);
			peer_device->evHFP_TxOK = 0;
#else
			if (evHFP_TxOK)
				pEventFree(evHFP_TxOK);
			evHFP_TxOK = 0;
#endif

			// Callback Application indicating SLC setup failure
		    // Need to Add Status flag to indicate failure.
			Free_Audio_GW(&peer_device->address); 	

			// @@@ Cancel SLC Timer
#if COMBINED_HOST==1
			host_timer_cancel(peer_device->slc_setup_timer_handle);
#endif
			peer_device->slc_setup_timer_handle = 0;

#if COMBINED_HOST==0     
			if (HF_callbacks.HF_CB_SLC_Established_Ind)
				(*HF_callbacks.HF_CB_SLC_Established_Ind)(peer_device->address,BT_UNKNOWNERROR);
#endif
		}
	}
	return 0;
}



/************************************************************
 * Function :- HF_rfcommFlowCtrlCallback(..)                 
 *                                                           
 * Description :-                                            
 * This is the callback from the RFCOMM layer indication     
 * FLOW_STOP/FLOW_GO.                                        
 *
 * NOTE :- The handling of this, cannot be deffered. It is 
 * critical that it is handled in the context of the invoker.
 * OTHERWISE deffering this function could cause a DEADLOCK.
 ************************************************************/

int HF_rfcommFlowCtrlCallback(t_bdaddr address, u_int8 dlci,  u_int8 flow_state)
{
	t_HFP_AG_Peer_Device* peer_device;

	peer_device = Find_Audio_GW_By_Address(&address);
	if(peer_device)
	{

		if (flow_state==RSE_FLOW_STOP)
		{
	
#if (HFP_MULTIPOINT_SUPPORT==1)
			if (peer_device->HFP_TxOK==1)
			{
				peer_device->HFP_TxOK=0;	
			}
#else
			HFP_TxOK=0;
#endif
		}
		else
		{ 
#if (HFP_MULTIPOINT_SUPPORT==1)
			if (peer_device->HFP_TxOK==0)
			{
				//pEventSet(peer_device->evHFP_TxOK);
			peer_device->HFP_TxOK=1;
			}
#else
			HFP_TxOK=1;
			pEventSet(evHFP_TxOK);
#endif
		}
	}

	
	return 0;

}


 /************************************************************************
 *
 *  Function :- HF_rfcommConnectIndicationCallback
 *
 *  Description :- 
 *  Indicates a remotely initiated RFOMM connection, implying an SLC is about
 *  to be established. A callback is presented to the application to Accept
 *  or reject the SLC.
 *  
 *************************************************************************/


int HF_rfcommConnectIndicationCallback(u_int8 schan, t_bdaddr address, u_int8 dlci, u_int16 max_frame_size) 
{

	t_HFP_AG_Peer_Device* peer_device;

	u_int16 cid;

    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_rfcommConnectIndicationCallback \n"));

	peer_device = Find_Audio_GW_By_Address(&address);

	
	if (!peer_device)
	{
		peer_device = Find_Free_Audio_GW();
        }


#if (HFP_MULTIPOINT_SUPPORT==1)
	if (peer_device)
		peer_device->HFP_TxOK = 1;

#else
	evHFP_TxOK = pEventCreate(0);
	HFP_TxOK = 1;
#endif

#if pDEBUG
     if (peer_device)
       pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_rfcommConnectIndicationCallback peer_device->acl_handle:0x%x\n",peer_device->acl_handle));
#endif  

	if (peer_device != 0)
	{ 
		int i;
		u_int8 server_channel;

		for (i=0;i<6;i++)  //store the AG address to the HFP_AG_Device global variable
		{
			peer_device->address.bytes[i] = address.bytes[i];
			gCurrent_Active_Addr.bytes[i]= address.bytes[i];
		}

		peer_device->dlci = dlci;
		peer_device->local_serverChannel = schan;
		peer_device->acl_state = HFP_DEVACTIVE;
		
		peer_device->active_profile = ACTIVE_PROFILE_HANDSFREE_AG;

		/* Get next server channel number and update SDP record
		*/
		server_channel = HF_GetAvailableSCN();
                pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_rfcommConnectIndicationCallback server_channel:0x%x\n",server_channel));
		if (server_channel)
		{
			HF_UpdateSDPRecord(server_channel);
		}

		RSE_GetCIDFromConnection(address, (u_int8)peer_device->dlci, &cid);
		if (cid)
		{
			L2_RetrieveHandle(cid,&peer_device->acl_handle);
		}
		peer_device->scm_state = SCM_W4_USER_ACCEPT;
        
#if HFP_CORE_DEBUG			
		printf("\n CALL SLC_REQUESTED_IND ");
#endif

#if COMBINED_HOST==0
		//if (HF_callbacks.HF_CB_SLC_Requested_Ind)
		//	(*HF_callbacks.HF_CB_SLC_Requested_Ind)(address);
#endif        
		gFirstFlag = 0;
		HF_Accept_SLC(address);  //accept the SLC.

	}
	return 0;
}

/************************************************************************
 *
 *  Function :- HF_rfcommDisconnectIndicationCallback
 *
 *  Description :- 
 *  Indicates an RFCOMM connection has been released. This also is the implicit
 *  release of any SLC running over this connection.
 *  
 *************************************************************************/
extern u_int32 prh_mgr_linkFlags;
int HF_rfcommDisconnectIndicationCallback(t_bdaddr address, u_int8 dlci) 
{
#ifdef COOLSAND
    t_HFP_AG_Peer_Device* peer_device;
    st_t_hfg_connect_ind ret;
    u_int8 i=0;
    
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_rfcommDisconnectIndicationCallback \n"));

    peer_device = Find_Audio_GW_By_Address(&address);
    if(peer_device)
    {
        ret.connection_id = peer_device->acl_handle;

        for(i=0;i<6;i++)
            ret.bt_addr.bytes[i] = peer_device->address.bytes[i];    
    }
    else
    {
        ret.connection_id=0;
        for(i=0;i<6;i++)
            ret.bt_addr.bytes[i] = address.bytes[i];    
    }
    prh_mgr_linkFlags|=MGR_CONNREQ_ACCEPT_ALL;//zhou siyou add 20090817
    rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_ACL_DISCONNECT_IND,&ret);
    Free_Audio_GW(&address);
#else
	Handle_RFCOMM_Disconnect(&address);
#endif

    return (0);
}

/************************************************************************
 *
 *  Function :- Handle_RFCOMM_Disconnect
 *
 *  Description :- 
 *  Handler for RFCOMM disconnection. This function may be extended to support
 *  automatic re-atachment.
 *  
 *************************************************************************/

void Handle_RFCOMM_Disconnect(t_bdaddr* address)
{
	t_HFP_AG_Peer_Device* peer_device;
	t_bdaddr tmp_address = *address;
    u_int16 acl_handle;
    
    
	peer_device = Find_Audio_GW_By_Address(address);

#if pDEBUG
    if(peer_device)
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"Handle_RFCOMM_Disconnect peer_device->acl_handle:0x%x, scm_state=%d\n",peer_device->acl_handle,peer_device->scm_state));
#endif      

	if ((peer_device) && (peer_device->scm_state != SCM_OPEN))
	{
            acl_handle = peer_device->acl_handle;
		if (peer_device->scm_state == SCM_W4_RFCOMM_DISCONNECT_INDICATION)
		{
			Free_Audio_GW(address);

#if COMBINED_HOST==0
			if (HF_callbacks.HF_CB_SLC_Released_Ind)
				(*HF_callbacks.HF_CB_SLC_Released_Ind)(tmp_address, acl_handle /* Not re-attaching */);
#endif           
		}
		else
		{

			Free_Audio_GW(address);
			/* Call Back the Application and Free the device Structure */
		        // @@@ Cancel SLC Timer
#if COMBINED_HOST==1
			host_timer_cancel(peer_device->slc_setup_timer_handle);
#endif
			peer_device->slc_setup_timer_handle = 0;

#if COMBINED_HOST==0
			if (HF_callbacks.HF_CB_SLC_Established_Ind)
				(*HF_callbacks.HF_CB_SLC_Established_Ind)(tmp_address,BT_UNKNOWNERROR);
#endif
		}

	}
	else if (peer_device) // No Reattach 
	{
             acl_handle = peer_device->acl_handle;
		Free_Audio_GW(address);
		/* Call Back the Application and Free the device Structure */
#if COMBINED_HOST==0
		if (HF_callbacks.HF_CB_SLC_Released_Ind)
			(*HF_callbacks.HF_CB_SLC_Released_Ind)(tmp_address, acl_handle /* Not re-attaching */);
#endif
	}
	else
	{
		// In normal operation this should NOT OCCUR 
		// BUT
		// This is required as the Nokia 6230 sometimes releases by Disconnecting DLCI 0.
		// From RFCOMM we get two Callbacks - First for active DLCI and second for DLCI 0.
		// In some-tests the Callback was only for DLCI 0 
#if HFP_CORE_DEBUG			
		printf("\n\r Got RFCOMM Disconnect for Non Existing Device ");
#endif
		Free_Audio_GW(address);
	}
}



/************************************************************************
 *
 *  Helper Functions 
 *  The following functions are used to extract elements from AT-Commands
 *************************************************************************/


 
/***********************************************************************
 *
 * Function :- HF_Extract_Service_List
 *
 * Description
 * This is specific to the +CIND: USR where the indicators and there
 * ranges are returned. We are only interested in 3 indicators "service",
 * "call" and "callsetup". The valid ranges for these are defined in the 
 * BT spec and thus do not need to be extracted here.
 ************************************************************************/

void* HF_Extract_Service_List(u_int8* p_data,t_HFP_AG_Peer_Device* peer_device)
{
    int indicator_order = 0;
    int i=0;
    int len=0;

    /* First Find the length of the Data field */

    while(p_data[len]!='\r')
        len++;

    while (i < len)
    {
        /* Get next indicator */
        while((p_data[i] != '"') && (i < len))
        {
            i++;
        }
        /* Found indicator - skip over '"' */
        i++;
        indicator_order++;
        switch(p_data[i])
        {
            case 's' :
            if  ((p_data[i+1]=='e') &&
                (p_data[i+2]=='r') &&
                (p_data[i+3]=='v') &&
                (p_data[i+4]=='i') &&
                (p_data[i+5]=='c') &&
                (p_data[i+6]=='e'))
                {
                    peer_device->service_ind_order = indicator_order;
                }
            break;

            case 'c' :
                if  ((p_data[i+1]=='a') &&
                    (p_data[i+2]=='l') &&
                    (p_data[i+3]=='l'))
                {
                    if ((p_data[i+4]=='_') &&
						(p_data[i+5]=='s') &&
                        (p_data[i+6]=='e') &&
                        (p_data[i+7]=='t') &&
                        (p_data[i+8]=='u') &&
                        (p_data[i+9]=='p'))
                    {
                        peer_device->call_setup_ind_order = indicator_order;
                    }
                    else if (p_data[i+4]=='"')
                    {
                        peer_device->call_ind_order = indicator_order;
                    }
                }
            break;

        }
        /* Unknown indicator - Skip end of indicator */
        while((p_data[i] != '"') && (i < len))
            i++;
        i++;
    }

    return 0;
}

/***********************************************************************
 *
 * Function :- HF_Update_Indicator_Status
 *
 * Description
 * This is specific to the +CIND: USR where the indicators values are returned.
 * The order in which the indicators are presented is previous +CIND and 
 * determined by "HF_Extract_Service_List".
 *
 * Only concerned about 3 indicators : Call, CallSetup, Service
 * The order of each of these indicators is stored in the peer_device
 ************************************************************************/

int HF_Update_Indicator_Status(u_int8* p_data,t_HFP_AG_Peer_Device* peer_device)
{
    u_int8 temp_ind_value;
	u_int8 inds_updated=0;
	

	if (peer_device->service_ind_order)
	{
		if(HF_Find_Indicator_Value(p_data,peer_device->service_ind_order,&temp_ind_value,1))
		{
			peer_device->service_ind = temp_ind_value;
			inds_updated = 1;
		}
	}
	if (peer_device->call_ind_order)
	{
		if(HF_Find_Indicator_Value(p_data,peer_device->call_ind_order,&temp_ind_value,1))
		{
			peer_device->call_ind = temp_ind_value;
			inds_updated = 1;
		}
	}
	if (peer_device->call_setup_ind_order)
	{
		if(HF_Find_Indicator_Value(p_data,peer_device->call_setup_ind_order,&temp_ind_value,3))
		{
			peer_device->call_setup_ind = temp_ind_value;
			inds_updated = 1;
		}
	}

	/* Callback the application to indicate that an indicator has been updated */
#if COMBINED_HOST==0    
	if ((inds_updated) && (HF_callbacks.HF_CB_Phone_Status_Ind))
		(*HF_callbacks.HF_CB_Phone_Status_Ind)(peer_device->address, peer_device->service_ind,
												peer_device->call_ind, peer_device->call_setup_ind);
#endif    

	return 0;
}
/****************
 * Returns 1 if indicator found 
 */
int HF_Find_Indicator_Value(u_int8* p_data,u_int8 ind_order,u_int8* p_ind_value,u_int8 max_value)
{
	int i = 0;
    int num_commas = 0;
	if (ind_order)
	{
		while((p_data[i] != '\r') && (num_commas!=(ind_order-1)))
		{
			if (p_data[i] == ',')
				num_commas++;
			i++;
		}
		
		if (num_commas==(ind_order-1))
		{
			char temp;

			while(p_data[i]==' ')
				i++;

			temp = p_data[i] ;

			if ( (temp >='0') && (temp<=('0'+max_value))  )
			{
				*p_ind_value = (u_int8)(p_data[i] - '0') ;
				return 1;
			}
		}
	}
	return 0;
}

u_int8 HF_Extract_Call_Handling_Info(u_int8* p_data)
{

	int len=0;
	u_int8 number=0;
	u_int8 call_handling=0;

	while(p_data[len] != '\r')
		len++;
	
	while(p_data != 0)
	{
		p_data = _Find_Numeric(p_data,(u_int8*)&number);


		if (p_data)
		{
			switch(number)
			{
			case 0 :
				call_handling |= 1;
				
				break;
			case 1 :
				call_handling |= 2;
				break;
			case 2 :
				call_handling |= 4;
				break;
				
			case 3 :
				call_handling |= 8;
				break;
				
			case 4 :
				call_handling |= 16;
				break;
				
			default :
				break;
				
				
			}
		}
	}

	return call_handling;
}

u_int8* _Find_Numeric(u_int8* p_data,u_int8* numeric)
{
	int i=0;
	int len=0;

	while(p_data[len] != '\r')
		len++;

	while(((p_data[i] < '0') || (p_data[i] > '9')) && (i<len))
		i++;
	if (i == len)
		return 0;
	else
	{
		*numeric = (u_int8)((p_data[i]) - '0');
		i++;
		p_data = p_data+i;
		
		return p_data;
	}
}

int HF_extract_number(t_AT_Event* AT_Event,u_int8 event,u_int8* p_data)
{
	/* To be completed */

	return 0;
}


/***********************************************
 * Function :- validate_and_store_dtmf_string
 *
 * Description 
 * Validates a DTMF string and stores it in "g_hfp_dtmf_string".
 * The length is stored in "g_hfp_dtmf_len".
 ************************************************/ 


static int validate_and_store_dtmf_string(char* dtmf_sequence)
{
	int i;

	for(i=0;((i<PRH_HFP_MAX_DTMF_STRING_SIZE) && (dtmf_sequence[i]!='\0'));i++)
	{
		/* Validate that we have a valid DTMF caracter 
		 *  "0-9", "#", "*","A-D"
		 *
		 */
		if (((dtmf_sequence[i] >= '0') && (dtmf_sequence[i] <= '9')) || // "0-9"
		    ((dtmf_sequence[i] >= 'A') && (dtmf_sequence[i] <= 'D')) ||
			((dtmf_sequence[i] == '#') || (dtmf_sequence[i] == '*')))
		{
			/* Valid DTMF Caracter */
			g_hfp_dtmf_string[i] = dtmf_sequence[i];
			g_hfp_dtmf_len++;
		}
		else
		{
			return 0;
		}
	}
	return 1;
}


/****************************************************************
 * Functions for handling multiple devices
 * 
 * Hardcoded to a single device for the first release
 ****************************************************************/

/***********************************************
 * Function :- HFP_Init_AG_Containers
 *
 * Description 
 * Initialises the container of device entries.
 ************************************************/ 


void HFP_Init_AG_Containers(void)
{
    int i=0;
    t_HFP_AG_Peer_Device* peer_device;

    for(i=0;i<HFP_AG_NUMBER;i++)
    {
        peer_device = &HFP_AG_Device[i];
        peer_device->acl_state = HFP_DEVIDLE;
        peer_device->scm_state = SCM_CLOSED;
        peer_device->audio_state = HFP_AUDIO_CLOSED;
        peer_device->operation_pending = 0;
        peer_device->remote_feature_status = HF_FEATURE_INBAND_TONE; // inband tones supported
        peer_device->p_service = 0;
        peer_device->call_handling = 0xFF;
        peer_device->versionInfo = 0;
        peer_device->call_state = HFP_CALL_CLOSED;
        peer_device->service_ind_order = 0;
        peer_device->call_setup_ind_order = 0;
        peer_device->call_ind_order = 0;
        peer_device->service_ind = 0;
        peer_device->call_setup_ind = 0;
        peer_device->call_ind = 0;
        peer_device->rf_role = 0;
        peer_device->supportedFeatures = 0;
        memset(peer_device->pending_data,0,sizeof(peer_device->pending_data));
        //peer_device->pending_data = 0;
        peer_device->pending_data_len =0;
#if (HFP_MULTIPOINT_SUPPORT==1)
        peer_device->evHFP_TxOK=0;
        peer_device->HFP_TxOK=1;  // Init to GO
#endif

    }
}

/***********************************************
 * Function :- Find_Free_Audio_GW
 *
 * Description 
 * Finds the next un-used entry.
 ************************************************/ 

t_HFP_AG_Peer_Device* Find_Free_Audio_GW(void)
{
	t_HFP_AG_Peer_Device* peer_device;
	int i=0;

	for(i=0;i<HFP_AG_NUMBER;i++)
	{
		peer_device = &HFP_AG_Device[i];
		if(peer_device->scm_state == SCM_CLOSED)
		{
			return peer_device;
		}
	}

	return 0;
}

/***********************************************
 * Function :- Free_Audio_GW
 *
 * Description 
 * Set the state of a device back to SCM_Closed.
 ************************************************/ 
void Free_Audio_GW(t_bdaddr *address)
{
#if (HFP_MULTIPOINT_SUPPORT==1)
	int i=0;
	t_HFP_AG_Peer_Device* peer_device;

	for(i=0;i<HFP_AG_NUMBER;i++)
	{
		peer_device = &HFP_AG_Device[i];
		if(BD_ADDR_CMP(*address,peer_device->address))
		{

			peer_device->scm_state = SCM_CLOSED;
			peer_device->audio_state = HFP_AUDIO_CLOSED;
			peer_device->acl_state = HFP_DEVIDLE;
			if (peer_device->evHFP_TxOK)
			{
				//pEventFree(peer_device->evHFP_TxOK);
				peer_device->evHFP_TxOK=0;
			}
			{
				int j;

				for(j=0;j<6;j++)
					peer_device->address.bytes[j] = 0;
			}
            
			if (peer_device->rf_role == RFCOMM_SERVER)
			   HF_FreeSCN(peer_device->local_serverChannel);

                    if (peer_device->ring_timer)//syzhou added
                    {
                        pTimerCancel(peer_device->ring_timer);   
                        peer_device->ring_timer = 0;
                    }

			return;
		}
	}
	return;
#else
	HFP_AG_Device[0].scm_state = SCM_CLOSED;
	HFP_AG_Device[0].remote_feature_status = HF_FEATURE_INBAND_TONE;
	if (evHFP_TxOK)
		pEventFree(evHFP_TxOK);
	evHFP_TxOK = 0;
	return;
#endif
}



/***********************************************
 * Function :- Find_Audio_GW_By_Address
 *
 * Description 
 * Finds a structure describing an AG in the container
 * using its Bd_Addr as a key.
 ************************************************/ 

t_HFP_AG_Peer_Device* Find_Audio_GW_By_Address(t_bdaddr* address)
{
	int i=0;
	t_HFP_AG_Peer_Device* peer_device;

	for(i=0;i<HFP_AG_NUMBER;i++)
	{
		peer_device = &HFP_AG_Device[i];
		if(BD_ADDR_CMP(*address,peer_device->address))
		{
			return peer_device;
		}
	}
	
	return 0;
}


t_HFP_AG_Peer_Device* Find_Audio_GW_With_Audio_Setup_In_Progress(void)
{
	t_HFP_AG_Peer_Device* peer_device;

#if (HFP_MULTIPOINT_SUPPORT==1)
	int i;

	for(i=0;i<HFP_AG_NUMBER;i++)
	{
		peer_device = &HFP_AG_Device[i];
		if(peer_device->audio_state == HFP_AUDIO_SETUP_IN_PROGRESS)
		{
			return peer_device;
		}
	}
	
	return 0;
	
#else
	peer_device = &HFP_AG_Device[0];

	if (peer_device->audio_state == HFP_AUDIO_SETUP_IN_PROGRESS)
		return peer_device;
	else
		return  0;
#endif
}
/* NOW FOR SHLOMI */

t_HFP_AG_Peer_Device* Find_Audio_GW_Rejecting_SCO(void)
{
	t_HFP_AG_Peer_Device* peer_device;

	int i;

	for(i=0;i<HFP_AG_NUMBER;i++)
	{
		peer_device = &HFP_AG_Device[i];
		if(peer_device->audio_state == HFP_REJECTING_AUDIO_W4_CONNECTION_COMPLETE)
		{
			return peer_device;
		}
	}
	return 0;
}



t_HFP_AG_Peer_Device* Find_Audio_GW_By_SCO_Handle(u_int16 sco_handle)
{
	t_HFP_AG_Peer_Device* peer_device;

#if (HFP_MULTIPOINT_SUPPORT==1)

	int i;

	for(i=0;i<HFP_AG_NUMBER;i++)
	{
		peer_device = &HFP_AG_Device[i];
		if(peer_device->sco_handle == sco_handle)
		{
			return peer_device;
		}
	}

	return 0;
#else
	peer_device = &HFP_AG_Device[0];

	if (peer_device->sco_handle == sco_handle)
		return peer_device;
	else
		return  0;
#endif
}



t_HFP_AG_Peer_Device* Find_Audio_GW_By_ACL_Handle(u_int16 acl_handle)
{
	t_HFP_AG_Peer_Device* peer_device;

#if (HFP_MULTIPOINT_SUPPORT==1)

	int i;

	for(i=0;i<HFP_AG_NUMBER;i++)
	{
		peer_device = &HFP_AG_Device[i];
		if(peer_device->acl_handle == acl_handle)
		{
			return peer_device;
		}
	}

	return 0;
#else
	peer_device = &HFP_AG_Device[0];

	if (peer_device->acl_handle == acl_handle)
		return peer_device;
	else
		return  0;
#endif
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************
 *  
 *                             SDP DATA BASE HANDLING 
 ***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/

/***********************************************************************************
 ***********************************************************************************
 *  Set-up the HandsFree HF Unit SDP Record
 **********************************************************************************
 ***********************************************************************************/
 #if 0
int HF_SetupSDPRecord(u_int8 rf_serverChannel, char *serviceName, u_int16 serviceNameLen)
{
#if SDP_RECORD_GEN
	int status =0;
	t_SDP_serviceRecord *pRecord;

	status = SDP_CreateRecord(&HFP_SDPrecordHandle, &pRecord);
	if (status!=BT_NOERROR)
		return 1;

	{
		/********************************/
		/*** add the Service Class List */
		/********************************/

		u_int16 uuid;
		t_SDP_dataElement elem_handsfreeUUID;
		t_SDP_dataElement elem_genAudioUUID;
		t_SDP_dataElement *elem_serviceUUIDList[10];
		t_SDP_dataElement elem_ServiceClassIDList;

		/*********************************************/
		/* Encode the handsfree uuid to a data element */
		/*********************************************/
		uuid = SDP_SCLASS_UUID_HANDSFREE; 
		status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_handsfreeUUID);
		if (status!=BT_NOERROR)
			return 1;

		/*************************************************/
		/* Encode the generic audio uuid to an attribute */
		/*************************************************/
		uuid = SDP_SCLASS_UUID_GEN_AUDIO; 
		status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_genAudioUUID);
		if (status!=BT_NOERROR)
			return 1;

		/************************************/
		/* Add both encoded uuids to a list */
		/************************************/
		elem_serviceUUIDList[0] = &elem_handsfreeUUID;
		elem_serviceUUIDList[1] = &elem_genAudioUUID;
		status = SDP_CreateList(SDP_SEQUENCE, 2, elem_serviceUUIDList, &elem_ServiceClassIDList);
		if (status!=BT_NOERROR)
			return 1;
		status = SDP_AddAttribute(pRecord, SDP_AID_SERVICE_CLASS, &elem_ServiceClassIDList);
		if (status!=BT_NOERROR)
			return 1;
	}

	{
		/***********************************************************************/
		/* add the Protocol Descriptor List (PDL) 
		*   The PDL is a list containing two lists, one for each lower protocol 
		*   <PDL:  <L2CAP: UUID> <RFCOMM: UUID, serverChannel> >
		***********************************************************************/
		t_SDP_dataElement elem_l2capProtocolInfoList;
		t_SDP_dataElement elem_rfcommProtocolInfoList;
		t_SDP_dataElement elem_protocolDescriptorList;
		t_SDP_dataElement *elem_protocolDescriptorElements[10];

		{
			/*************************************************/
			/**** create the L2CAP protocol information list */
			/*************************************************/
			u_int16 uuid;
			t_SDP_dataElement elem_l2capUUID;
			t_SDP_dataElement *elem_l2capProtocolInfoElements[10];

			/**********************************/
			/* encode the l2cap protocol uuid */
			/**********************************/
			uuid = SDP_PROTOCOL_UUID_L2CAP;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_l2capUUID);
			if (status!=BT_NOERROR)
				return 1;

			/*****************************************/
			/* Add the l2cap protocol uuid to a list */
			/*****************************************/

			elem_l2capProtocolInfoElements[0] = &elem_l2capUUID;
			status = SDP_CreateList(SDP_SEQUENCE, 1, elem_l2capProtocolInfoElements, &elem_l2capProtocolInfoList);
			if (status!=BT_NOERROR)
				return 1;
		}

		{
			/**************************************************/
			/**** create the RFCOMM protocol information list */
			/**************************************************/
			u_int16 uuid;
			t_SDP_dataElement elem_rfcommUUID;
			t_SDP_dataElement elem_rfcommServerChannel;
			t_SDP_dataElement *elem_rfcommProtocolInfoElements[10];

			/*****************************************/
			/* encode the rfcomm protocol uuid       */
			/*****************************************/

			uuid = SDP_PROTOCOL_UUID_RFCOMM;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_rfcommUUID);
			if (status!=BT_NOERROR)
				return 1;

			/*******************************************/
			/* encode the rfcomm server channel number */
			/*******************************************/

			status = SDP_EncodeAttribute(SDP_UINT, 1, (u_int8 *)&rf_serverChannel, &elem_rfcommServerChannel);
			if (status!=BT_NOERROR)
				return 1;

			/*******************************************/
			/*  Add the both of the attributes to an   */
			/*  RFCOMM protocol information list       */
			/*******************************************/

			elem_rfcommProtocolInfoElements[0] = &elem_rfcommUUID;
			elem_rfcommProtocolInfoElements[1] = &elem_rfcommServerChannel;
			status = SDP_CreateList(SDP_SEQUENCE, 2, elem_rfcommProtocolInfoElements, &elem_rfcommProtocolInfoList);
			if (status!=BT_NOERROR)
				return 1;
		}

		/*******************************************/
		/* Enclose both L2CAP and RFCOMM protocol  */
		/* information lists in an outer protocol  */
		/* descriptor list                         */
		/*******************************************/
		elem_protocolDescriptorElements[0] = &elem_l2capProtocolInfoList;
		elem_protocolDescriptorElements[1] = &elem_rfcommProtocolInfoList;

		status = SDP_CreateList(SDP_SEQUENCE, 2, elem_protocolDescriptorElements, &elem_protocolDescriptorList);
		if (status!=BT_NOERROR)
			return 1;

		/*******************************************/
		/* Add the protocol descriptor list as a   */
		/* record attribute                        */
		/*******************************************/
		status = SDP_AddAttribute(pRecord, SDP_AID_PROTOCOL, &elem_protocolDescriptorList);
		if (status!=BT_NOERROR)
			return 1;
	}

	{
		/*******************************************/
		/**** add the Profile Descriptor List      */
		/*******************************************/

		t_SDP_dataElement elem_profile0DescriptorList;
		t_SDP_dataElement elem_ProfileDescriptorList;
		t_SDP_dataElement *elem_ProfileDescriptorElements[10];

		{
			/*******************************************/
			/**** create the Profile0 Descriptor List  */
			/*******************************************/
			u_int16 uuid;
			t_SDP_dataElement elem_profile0UUID;
			t_SDP_dataElement elem_profile0Version;
			t_SDP_dataElement *elem_profile0DescriptorElements[10];
			u_int16 profile0Version=BT_HFP_PROTOCOL_VERSION;

			/*******************************************/
			/* encode the profile0  uuid       */
			/*******************************************/
			uuid = SDP_SCLASS_UUID_HANDSFREE;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_profile0UUID);
			if (status!=BT_NOERROR)
				return 1;

			/*********************************************/
			/* encode the profile0 profile version       */
			/*********************************************/
			status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8 *)&profile0Version, &elem_profile0Version);
			if (status!=BT_NOERROR)
				return 1;

			/*********************************************/
			/*  Add the both of the attributes to an     */
			/*  profile0 protocol information list       */
			/*********************************************/
			elem_profile0DescriptorElements[0] = &elem_profile0UUID;
			elem_profile0DescriptorElements[1] = &elem_profile0Version;
			status = SDP_CreateList(SDP_SEQUENCE, 2, elem_profile0DescriptorElements, &elem_profile0DescriptorList);
			if (status!=BT_NOERROR)
				return 1;
		}  

		/*****************************************************************/
		/* Enclose the profile lists in an outer Profile Descriptor List */
		/*****************************************************************/
		elem_ProfileDescriptorElements[0] = &elem_profile0DescriptorList;
		status = SDP_CreateList(SDP_SEQUENCE, 1, elem_ProfileDescriptorElements, &elem_ProfileDescriptorList);
		if (status!=BT_NOERROR)
			return 1;

		/*****************************************************************/
		/* Add the the Profile0 Descriptor List to the record            */
		/*****************************************************************/
		status = SDP_AddAttribute(pRecord, SDP_AID_PROFILE_LIST, &elem_ProfileDescriptorList);
		if (status!=BT_NOERROR)
			return 1;
	}


	{
		/*****************************************************************/
		/**** add the Service Name                                       */
		/*****************************************************************/
		t_SDP_dataElement elem_serviceName;
		u_int16 nameAttributeId = SDP_AID_DEFAULT_LANGUAGE+SDP_AID_SERVICE_NAME_OFFSET; 

		/*****************************************************************/
		/*  Encode the name as a string attrbiute                        */
		/*****************************************************************/
		status = SDP_EncodeAttribute(SDP_TEXT, serviceNameLen, (u_int8*)serviceName, &elem_serviceName);
		if (status!=BT_NOERROR)
			return 1;

		/*****************************************************************/
		/* Add the Service Name to the record                            */
		/*****************************************************************/

		status = SDP_AddAttribute(pRecord, nameAttributeId, &elem_serviceName);
		if (status!=BT_NOERROR)
			return 1;
	}

	{
	/******************************************/
	/**** add the Supported Features          */
	/******************************************/
	t_SDP_dataElement elem_supportedFeatures;
	u_int16 supportedFeatures = PRH_HFP_SUPPORTED_FEATURES;

	/******************************************/
	/*  Encode the name as a string attrbiute */
	/******************************************/
	status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8*)&supportedFeatures, &elem_supportedFeatures);
	if (status!=BT_NOERROR)
	return 1;

	/**********************************************************/
	/* Add the the Volume Control Support flag to the record  */
	/**********************************************************/

        status = SDP_AddAttribute(pRecord, SDP_AID_SUPPORTED_FEATURES, &elem_supportedFeatures);
        if (status!=BT_NOERROR)
            return 1;
	}
#endif
	return 0; /* success */
}

/***********************************************************************************
 ***********************************************************************************
 *  Set-up the Headset AG SDP Record
 **********************************************************************************
 ***********************************************************************************/

int HF_SetupHeadsetAG_SDPRecord(u_int8 rf_serverChannel, char *serviceName, u_int16 serviceNameLen)
{
#if SDP_RECORD_GEN
	int status=0;
	t_SDP_serviceRecord *pRecord;
	u_int32 recordHandle;

	status = SDP_CreateRecord(&recordHandle, &pRecord);
    if (status!=BT_NOERROR)
        return 1;

    {
		/********************************/
	    /*** add the Service Class List */
		/********************************/

	    u_int16 uuid;
	    t_SDP_dataElement elem_headsetUUID;
	    t_SDP_dataElement elem_genAudioUUID;
	    t_SDP_dataElement *elem_serviceUUIDList[10];
	    t_SDP_dataElement elem_ServiceClassIDList;

        /* Encode the headset uuid to a data element */
	    uuid = SDP_SCLASS_UUID_HEADSET; 
	    status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_headsetUUID);
        if (status!=BT_NOERROR)
            return 1;
	    
        /* Encode the generic audio uuid to an attribute */
	    uuid = SDP_SCLASS_UUID_GEN_AUDIO; 
	    status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_genAudioUUID);
        if (status!=BT_NOERROR)
            return 1;

        /* Add both encoded uuids to a list */
	    elem_serviceUUIDList[0] = &elem_headsetUUID;
	    elem_serviceUUIDList[1] = &elem_genAudioUUID;
	    status = SDP_CreateList(SDP_SEQUENCE, 2, elem_serviceUUIDList, &elem_ServiceClassIDList);
        if (status!=BT_NOERROR)
            return 1;
	    status = SDP_AddAttribute(pRecord, SDP_AID_SERVICE_CLASS, &elem_ServiceClassIDList);
        if (status!=BT_NOERROR)
            return 1;

    }



    {
	    /******* add the Protocol Descriptor List (PDL) 
            The PDL is a list containing two lists, one for each lower protocol 
            <PDL:  <L2CAP: UUID> <RFCOMM: UUID, serverChannel> >
        */
        t_SDP_dataElement elem_l2capProtocolInfoList;
        t_SDP_dataElement elem_rfcommProtocolInfoList;
        t_SDP_dataElement elem_protocolDescriptorList;
        t_SDP_dataElement *elem_protocolDescriptorElements[10];

        {
            /**** create the L2CAP protocol information list */
            u_int16 uuid;
            t_SDP_dataElement elem_l2capUUID;
            t_SDP_dataElement *elem_l2capProtocolInfoElements[10];

	        /* encode the l2cap protocol uuid */
	        uuid = SDP_PROTOCOL_UUID_L2CAP;
	        status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_l2capUUID);
            if (status!=BT_NOERROR)
                return 1;

	        /* Add the l2cap protocol uuid to a list */
	        elem_l2capProtocolInfoElements[0] = &elem_l2capUUID;
	        status = SDP_CreateList(SDP_SEQUENCE, 1, elem_l2capProtocolInfoElements, &elem_l2capProtocolInfoList);
            if (status!=BT_NOERROR)
                return 1;
        }

        {
            /**** create the RFCOMM protocol information list */
            u_int16 uuid;
            t_SDP_dataElement elem_rfcommUUID;
            t_SDP_dataElement elem_rfcommServerChannel;
            t_SDP_dataElement *elem_rfcommProtocolInfoElements[10];

	        /* encode the rfcomm protocol uuid */
            uuid = SDP_PROTOCOL_UUID_RFCOMM;
            status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_rfcommUUID);
            if (status!=BT_NOERROR)
                return 1;

	        /* encode the rfcomm server channel number */
            status = SDP_EncodeAttribute(SDP_UINT, 1, (u_int8 *)&rf_serverChannel, &elem_rfcommServerChannel);
            if (status!=BT_NOERROR)
                return 1;

            /*  Add the both of the attributes to an RFCOMM protocol information list */
            elem_rfcommProtocolInfoElements[0] = &elem_rfcommUUID;
            elem_rfcommProtocolInfoElements[1] = &elem_rfcommServerChannel;
            status = SDP_CreateList(SDP_SEQUENCE, 2, elem_rfcommProtocolInfoElements, &elem_rfcommProtocolInfoList);
            if (status!=BT_NOERROR)
                return 1;
        }


        /* Enclose both L2CAP and RFCOMM protocol information lists in an outer protocol descriptor list */
        elem_protocolDescriptorElements[0] = &elem_l2capProtocolInfoList;
        elem_protocolDescriptorElements[1] = &elem_rfcommProtocolInfoList;

        status = SDP_CreateList(SDP_SEQUENCE, 2, elem_protocolDescriptorElements, &elem_protocolDescriptorList);
        if (status!=BT_NOERROR)
            return 1;

        /* Add the protocol descriptor list as a record attribute */
        status = SDP_AddAttribute(pRecord, SDP_AID_PROTOCOL, &elem_protocolDescriptorList);
				if (status!=BT_NOERROR)
					return 1;
    }


    {
        /**** add the Profile Descriptor List */
        t_SDP_dataElement elem_profile0DescriptorList;
	    t_SDP_dataElement elem_ProfileDescriptorList;
	    t_SDP_dataElement *elem_ProfileDescriptorElements[10];

        {
           /**** create the Profile0 Descriptor List */

            u_int16 uuid;
            t_SDP_dataElement elem_profile0UUID;
            t_SDP_dataElement elem_profile0Version;
            t_SDP_dataElement *elem_profile0DescriptorElements[10];
            u_int16 profile0Version=0x0100;

	        /* encode the profile0 protocol uuid */
            uuid = SDP_SCLASS_UUID_HEADSET;
            status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_profile0UUID);
            if (status!=BT_NOERROR)
                return 1;

	        /* encode the profile0 server channel number */
            status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8 *)&profile0Version, &elem_profile0Version);
            if (status!=BT_NOERROR)
                return 1;

            /*  Add the both of the attributes to an profile0 protocol information list */
            elem_profile0DescriptorElements[0] = &elem_profile0UUID;
            elem_profile0DescriptorElements[1] = &elem_profile0Version;
            status = SDP_CreateList(SDP_SEQUENCE, 2, elem_profile0DescriptorElements, &elem_profile0DescriptorList);
						if (status!=BT_NOERROR)
							return 1;
        }  
        
        /* Enclose the Profile0 Descriptor List in an outer Profile Descriptor List */
        elem_ProfileDescriptorElements[0] = &elem_profile0DescriptorList;

        status = SDP_CreateList(SDP_SEQUENCE, 1, elem_ProfileDescriptorElements, &elem_ProfileDescriptorList);
        if (status!=BT_NOERROR)
            return 1;

        /* Add the the Profile Descriptor List to the record  */
        status = SDP_AddAttribute(pRecord, SDP_AID_PROFILE_LIST, &elem_ProfileDescriptorList);
        if (status!=BT_NOERROR)
            return 1;

    }


    {
        /**** add the Service Name */
        t_SDP_dataElement elem_serviceName;
        u_int16 nameAttributeId = SDP_AID_DEFAULT_LANGUAGE+SDP_AID_SERVICE_NAME_OFFSET; 

		/*  Encode the name as a string attrbiute */
		status = SDP_EncodeAttribute(SDP_TEXT, serviceNameLen, (u_int8*)serviceName, &elem_serviceName);
		if (status!=BT_NOERROR)
			return 1;

		/* Add the the Profile0 Descriptor List to the record  */
		status = SDP_AddAttribute(pRecord, nameAttributeId, &elem_serviceName);
		if (status!=BT_NOERROR)
			return 1;
	}


	{
		/***** Add the Volume Control Support `***********/
		t_SDP_dataElement elem_volCtrlSupported;
		u_int8 volCtrlStatus = 1;

		status = SDP_EncodeAttribute(SDP_BOOLEAN, 1, &volCtrlStatus, &elem_volCtrlSupported);
		if (status!=BT_NOERROR)
			return 1;

		status = SDP_AddAttribute(pRecord, SDP_AID_AUDIO_VOLUME, &elem_volCtrlSupported);
		if (status!=BT_NOERROR)
			return 1;
	}
#endif

    return 0; /* success */
}
 #endif
static void ServiceSearchCompletCallback(void *pArgs)
{
    t_HFP_AG_Peer_Device* peer_device = (t_HFP_AG_Peer_Device *)pArgs;

     pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"ServiceSearchCompletCallback serverChannel=%d\n", peer_device->serverChannel));
    pTimerCancel(rda_sdp_timer);
    rda_sdp_timer=0;
    if(HF_callbacks.HF_CB_ServiceDiscoveryResult )
        HF_callbacks.HF_CB_ServiceDiscoveryResult (1,peer_device->address, peer_device->serverChannel);
}

extern u_int8 get_list_length(u_int8* data,u_int32 *len,u_int8* offset) ;
extern u_int8 read_uuid(u_int8* data,u_int32* uuid,u_int8* offset) ;
int sdpSearchCompleteCallback(u_int8 status)
{
#if SDP_CLIENT	
    t_HFP_AG_Peer_Device* peer_device;
    u_int32 uuid;
    u_int8 exit_condiction=0;
    u_int8 featuresFound = 0;
    u_int8 versionFound = 0;
    u_int8 scnFound = 0;
    u_int8* finger;
    u_int8* end_byte;
    u_int8 counter;
    u_int16 aid;
    u_int8 offset;
    u_int32 length;
    u_int32 outer;
    u_int32 inner;
    u_int8 search_fail=0;
    u_int8 incomplete_results=0;

	u_int32 total_len;


    u_int16 serverChannel=0;
    u_int16 versionInfo=0;
    u_int16 supportedFeatures=0; 
//    u_int8  volCtrlSupported=0;

	u_int32 uuid_finded[15];
   // t_SDP_service_Parse_Results uuid_result;
    
	int i=0;

    //sdp_service_parse(pResults, &uuid_result);

    if (pResults->status!=0x00)
    {
        if(devices.addresses)
            OS_HEAP_Free(devices.addresses);		
        if(pattern.patternUUIDs)
            OS_HEAP_Free(pattern.patternUUIDs);	
        if(attribs.attributeIDs)
            OS_HEAP_Free(attribs.attributeIDs);	

        status = BT_UNKNOWNERROR;
    }
    else if(!pResults->element.pData)
    {
        if(devices.addresses)
            OS_HEAP_Free(devices.addresses);		
        if(pattern.patternUUIDs)
            OS_HEAP_Free(pattern.patternUUIDs);	
        if(attribs.attributeIDs)
            OS_HEAP_Free(attribs.attributeIDs);	

        status = BT_UNKNOWNERROR;
    }

    if (status == BT_NOERROR)
    {
		total_len = pResults->element.dataLen;
        end_byte = pResults->element.pData + pResults->element.dataLen -1;

        finger = pResults->element.pData;

            get_list_length(finger,&length,&offset);
            finger+= offset;
            get_list_length(finger,&length,&offset);
            finger+= offset;
            counter = length;
            
            do 
            {
                counter--;
                finger++; /* bypass type code */
                aid = (finger[0] << 8) + finger[1];
                finger+=2;
                counter-=2;

                switch(aid) 
                {
                    case SDP_AID_PROTOCOL:
                        get_list_length(finger,&length,&offset);
                        finger += offset;
                        counter -= offset;
                        outer = length;
                        do 
                        {
                            do 
                            {
                                get_list_length(finger,&length,&offset);
                                finger +=offset;
                                counter -=offset;
                                outer -=offset;
                                inner = length;
                                read_uuid(finger,&uuid,&offset);
                                finger += offset;
                                counter -= offset;
                                outer -= offset;
                                inner -= offset;
                                if(uuid == SDP_PROTOCOL_UUID_RFCOMM && inner) 
                                {
                                    //peer_device->serverChannel = finger[1];
                                    serverChannel = finger[1];
                                    scnFound = 1;
                                    finger+=2;
                                    counter-=2;
                                    inner -= 2;
                                    outer-=2;
                                }
                                else
                                {
                                    finger += inner;
                                    counter -= inner;
                                    outer -= inner;
                                    inner = 0;
                                }					
                            }while(inner);
                        }while (outer);
                    break;

                    case SDP_AID_PROFILE_LIST :
                        get_list_length(finger,&length,&offset);
                        finger += offset;
                        counter -= offset;
                        outer = length;
                        do 
                        {
                            do 
                            {
                                get_list_length(finger,&length,&offset);
                                finger +=offset;
                                counter -=offset;
                                outer -=offset;
                                inner = length;
                                read_uuid(finger,&uuid,&offset);
                                finger += offset;
                                counter -= offset;
                                outer -= offset;
                                inner -= offset;
                                if(uuid == SDP_SCLASS_UUID_HANDSFREE && inner) 
                                {
                                    versionInfo = (finger[1] << 8) + finger[2];
                                    versionFound = 1;
                                    finger+=3;
                                    counter-=3;
                                    inner -= 3;
                                    outer-=3;
                                }
                                else
                                {
                                    finger += inner;
                                    counter -= inner;
                                    outer -= inner;
                                    inner = 0;
                                }
                                uuid_finded[i]=uuid;
                                i++;
                            } 	while(inner);
                        } while (outer);
                    break;

                    case SDP_AID_SUPPORTED_FEATURES :
                        counter--;
                        finger++; // Skip over Type code and len = 0x09
                        supportedFeatures = (finger[0] << 8) + finger[1];
                        featuresFound = 1;
                        finger+=2;
                        counter-=2;
                    break;

#if 0	
                    // For Headset only !
                    case SDP_AID_AUDIO_VOLUME : 
                    volCtrlSupported = finger[1];
                    finger +=2;
                    counter -= 2;
                    break;
#endif
                    default :
                        exit_condiction=1;
                    break;

                }
            }while (counter && !exit_condiction); 
    }
	else
	{
		// Search Complete returned an Error.
            search_fail = 1;
	}

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"sdpSearchCompleteCallback search_fail=%d, serverChannel=%d\n",search_fail, serverChannel));
#endif  

	/*****************************************************
	 *
	 * Interop fix for Motorola V525.
	 *
	 * As some devices/phone dont respond with features 
	 * or version information it is important that the SDP
	 * request is not failed due to lack of either of these 
	 * parameters. The critical parameter is the Server Channel 
	 * Number, once we have this we can proceed to SLC setup.
	 ********************************************************/

        if (!featuresFound)
        {
            supportedFeatures = HFP_DEFAULT_SUPPORTED_FEATURES;
        }
        if (!versionFound)
        {
            versionInfo = HFP_DEFAULT_VERSION_INFORMATION;
        }

	/***********************************************************
	 * End of interop fix for Motorola V525.
	 ***********************************************************/

	if (scnFound)
	{
		status = BT_NOERROR;
	}
	else
	{
#if HFP_CORE_DEBUG			
		printf("\n\rSearch Complete Did not find SCN ");
#endif
		status = BT_UNKNOWNERROR;
		incomplete_results = 1;
	}

	/********************************************************
	 * Following code has to change to support more than one 
	 * one AG attached to the Handsfree unit.
	 * Currently only support for one AG attached.
	 *********************************************************/
	
	if (status == BT_NOERROR)
	{
		peer_device = Find_Audio_GW_By_Address(devices.addresses);
		
#if (HFP_MULTIPOINT_SUPPORT==1)
		/* If device has not previously in use allocate a new element
		* for it.
		*/
		if (!peer_device)
		{
			peer_device = Find_Free_Audio_GW();
			if (peer_device)
			{ 
				int j;
				
				for (j=0;j<6;j++)
					peer_device->address.bytes[j] = devices.addresses->bytes[j];
			}
			else
			{
				status = BT_UNKNOWNERROR;
			}
			
		}
#else
		peer_device = Find_Free_Audio_GW();
             if (peer_device)
		{ 
			int i;
			
			for (i=0;i<6;i++)
				peer_device->address.bytes[i] = devices.addresses->bytes[i];
		}
#endif

		if (peer_device)
		{
                    peer_device->serverChannel = serverChannel;
                    peer_device->supportedFeatures = supportedFeatures;
                    peer_device->versionInfo = versionInfo;
                    peer_device->active_profile = ACTIVE_PROFILE_HANDSFREE_AG;//ACTIVE_PROFILE_HANDSFREE;
		}
		/*********************************************************
		* End of code to be modified for multipoint 
		*********************************************************/
		
		if(pResults)
		{
			SDP_FreeResults(pResults);
		}
#if 1
            //if (HF_callbacks.HF_CB_ServiceDiscoveryResult)
            {
                int outcome=0;
                if(status==BT_NOERROR)
                    outcome =1;
                
                HFP_Browse_Pending=0;
                //(*HF_callbacks.HF_CB_ServiceDiscoveryResult)(outcome,*(devices.addresses),serverChannel);
                 pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"sdpSearchCompleteCallback create the timer to call the ServiceSearchCompletCallback\n"));
				
                 if (peer_device) 
				 rda_sdp_timer=pTimerCreate(20, ServiceSearchCompletCallback,(void*)peer_device,pTIMER_ONESHOT);
				 
            }
#else
		{
			int outcome;
		    TdbDeviceRecord *deviceRecord;

			deviceRecord = DB_GetDeviceRecordByBdAddress(&(peer_device->address));
			if (status == BT_NOERROR)
			{	
				outcome = 1;
				if (deviceRecord) 
				{
					deviceRecord->profile.hfp.sdp_state = DB_SDP_TRUE;
				}
			}
			else
			{
				outcome = 0;
				if (deviceRecord) 
				{
					deviceRecord->profile.hfp.sdp_state = DB_SDP_FALSE;
				}
			}

			if (peer_device->scm_state == SCM_SDP_DISCOVERY_PENDING)
			{
				// HF_Setup_SLC(*(devices.addresses));
				status = HF_Connect_As_Client(peer_device->address);
				if (!((status == BT_NOERROR) || (status == BT_PENDING)))
				{
					int i;
					t_bdaddr  temp_addr;
					
					for (i=0;i<6;i++)
						temp_addr.bytes[i] = peer_device->address.bytes[i];
					
					Free_Audio_GW(&peer_device->address);
					HFP_Browse_Pending=0;
#if COMBINED_HOST==0                
					if (HF_callbacks.HF_CB_SLC_Established_Ind)
						(*HF_callbacks.HF_CB_SLC_Established_Ind)(temp_addr,status);
#endif                    
				}

			}
#if COMBINED_HOST==0                
			else if (HF_callbacks.HF_CB_ServiceDiscoveryResult)
			{
				HFP_Browse_Pending=0;
				(*HF_callbacks.HF_CB_ServiceDiscoveryResult)(outcome,*(devices.addresses),serverChannel);
			}
#endif            
		}
#endif
		HFP_Browse_Pending=0;
		return 0;
	
	}
	else // status != BT_NOERROR
	{
	//	peer_device = Find_Audio_GW_By_Address(devices.addresses);

		peer_device = Find_Service_Browse_Ongoing();

		if (peer_device && peer_device->scm_state == SCM_SDP_DISCOVERY_PENDING)
		{
			// Callback Application indicating SLC setup failure
		    // Need to Add Status flag to indicate failure.
			
			int j;
			t_bdaddr  temp_addr;
			
			for (j=0;j<6;j++)
				temp_addr.bytes[j] = peer_device->address.bytes[j];
			
			Free_Audio_GW(&peer_device->address); 
			HFP_Browse_Pending=0;
#if COMBINED_HOST==0                
			if (HF_callbacks.HF_CB_SLC_Established_Ind)
				(*HF_callbacks.HF_CB_SLC_Established_Ind)(temp_addr,status);
#endif
		}
		else
		{
			HFP_Browse_Pending=0;
		}
    }
#endif

	return 0;
}

int ServiceSearchCompleteCallback(u_int8 status)
{
  //  t_HFP_AG_Peer_Device* peer_device;
    u_int32 uuid;
    u_int8 exit_condiction=0;
    u_int8 featuresFound = 0;
    u_int8 versionFound = 0;
    u_int8* finger;
    u_int8* end_byte;
    u_int8 counter;
    u_int16 aid;
    u_int8 offset;
    u_int32 length;
    u_int32 outer;
    u_int32 inner;
    u_int8 search_fail=0;
   // u_int8 incomplete_results=0;

    u_int32 total_len;


    u_int16 serverChannel=0;
    u_int16 versionInfo=0;
    u_int16 supportedFeatures=0; 
  //  u_int8  volCtrlSupported=0;

    t_SDP_service_Results service_finded[MAX_SDP_UUID_NUM];
    int i=0;

    memset(service_finded,0,sizeof(t_SDP_service_Results)*MAX_SDP_UUID_NUM);

    if (pResults->status!=0x00)
    {
        if(devices.addresses)
            OS_HEAP_Free(devices.addresses);		
        if(pattern.patternUUIDs)
            OS_HEAP_Free(pattern.patternUUIDs);	
        if(attribs.attributeIDs)
            OS_HEAP_Free(attribs.attributeIDs);	

        status = BT_UNKNOWNERROR;
    }
    else if(!pResults->element.pData)
    {
        if(devices.addresses)
            OS_HEAP_Free(devices.addresses);		
        if(pattern.patternUUIDs)
            OS_HEAP_Free(pattern.patternUUIDs);	
        if(attribs.attributeIDs)
            OS_HEAP_Free(attribs.attributeIDs);	

        status = BT_UNKNOWNERROR;
    }

    if (status == BT_NOERROR)
    {
        total_len = pResults->element.dataLen;
        end_byte = pResults->element.pData + pResults->element.dataLen -1;

        finger = pResults->element.pData;
    		
        get_list_length(finger,&length,&offset);
        finger+= offset;
 	do 
	{           
            get_list_length(finger,&length,&offset);
            finger+= offset;
            counter = length;
            
            do 
            {
                counter--;
                finger++; /* bypass type code */
                aid = (finger[0] << 8) + finger[1];
                finger+=2;
                counter-=2;

                switch(aid) 
                {
                    case SDP_AID_SERVICE_CLASS:
                        get_list_length(finger,&length,&offset);
                        finger += offset;
                        counter -= offset;
                        outer = length;
        
                        read_uuid(finger,&uuid,&offset);
                        finger += offset;
                        counter -= offset;
                        service_finded[i].SC_uuid=uuid;
                        outer -= offset;
                        finger +=outer;
                        counter-=outer;
                    break;
                    
                    case SDP_AID_PROTOCOL:
                        get_list_length(finger,&length,&offset);
                        finger += offset;
                        counter -= offset;
                        outer = length;
                        do 
                        {
                            do 
                            {
                                get_list_length(finger,&length,&offset);
                                finger +=offset;
                                counter -=offset;
                                outer -=offset;
                                inner = length;
                                read_uuid(finger,&uuid,&offset);
                                finger += offset;
                                counter -= offset;
                                outer -= offset;
                                inner -= offset;
                                if(uuid == SDP_PROTOCOL_UUID_RFCOMM && inner) 
                                {
                                    serverChannel = finger[1];
                                    service_finded[i].chn_num = serverChannel;
                                    finger+=2;
                                    counter-=2;
                                    inner -= 2;
                                    outer-=2;
                                }
                                else
                                {
                                    finger += inner;
                                    counter -= inner;
                                    outer -= inner;
                                    inner = 0;
                                }					
                            }while(inner);
                        }while (outer);
                    break;

                    case SDP_AID_PROFILE_LIST :
                        get_list_length(finger,&length,&offset);
                        finger += offset;
                        counter -= offset;
                        outer = length;
                        do 
                        {
                            do 
                            {
                                get_list_length(finger,&length,&offset);
                                finger +=offset;
                                counter -=offset;
                                outer -=offset;
                                inner = length;
                                read_uuid(finger,&uuid,&offset);
                                finger += offset;
                                counter -= offset;
                                outer -= offset;
                                inner -= offset;
                                service_finded[i].PDL_uuid = uuid;
                                if(uuid == SDP_SCLASS_UUID_HANDSFREE && inner) 
                                {
                                    versionInfo = (finger[1] << 8) + finger[2];
                                    versionFound = 1;
                                    finger+=3;
                                    counter-=3;
                                    inner -= 3;
                                    outer-=3;
                                }
                                else
                                {
                                    finger += inner;
                                    counter -= inner;
                                    outer -= inner;
                                    inner = 0;
                                }

                            } 	while(inner);
                        } while (outer);
                    break;

                    case SDP_AID_SUPPORTED_FEATURES :
                        counter--;
                        finger++; // Skip over Type code and len = 0x09
                        supportedFeatures = (finger[0] << 8) + finger[1];
                        featuresFound = 1;
                        finger+=2;
                        counter-=2;
                    break;

                    default :
                        exit_condiction=1;
                    break;

                }
            }while (counter && !exit_condiction); 
            i++;
	}while(finger<end_byte);
    }
    else
    {
        // Search Complete returned an Error.
        search_fail = 1;
    }

	return search_fail;
}	

/****************************************************
 * Function :- Is_Service_Browse_Ongoing
 *
 * Description :- Determines if an SDP Browse is currently ongoing.
 ***************************************************/

static int Is_Service_Browse_Ongoing(void)
{

	t_HFP_AG_Peer_Device* peer_device;
	int i;

	for(i=0;i<HFP_AG_NUMBER;i++)
	{
		peer_device = &HFP_AG_Device[i];
		if(peer_device->scm_state == SCM_SDP_DISCOVERY_PENDING)
		{
			HFP_Browse_Pending = 1;
			//return 1; //return peer_device;  //Zhujianguo changed it. 2007.12.28
		}
	}
	
	if (HFP_Browse_Pending)
		return 1;

	return 0;
}

/****************************************************
 * Function :- Find_Service_Browse_Ongoing
 *
 * Description :- Determines if an SDP Browse is currently ongoing.
 ***************************************************/

t_HFP_AG_Peer_Device* Find_Service_Browse_Ongoing(void)
{

	t_HFP_AG_Peer_Device* peer_device;
	int i;

	for(i=0;i<HFP_AG_NUMBER;i++)
	{
		peer_device = &HFP_AG_Device[i];
		if(peer_device->scm_state == SCM_SDP_DISCOVERY_PENDING)
		{
			return peer_device;
		}
	}

	return 0;
}


/****************************************************
 * Function :- HF_GetAvailableSCN
 *
 * Description :- Allocates an SCN from a pool
 ***************************************************/

u_int8 HF_GetAvailableSCN(void)
{

	int i;

	for (i=0;i<6;i++)
	{
		if(HF_Scn_Array[i].used == 0)
		{
			HF_Scn_Array[i].used = 1;
			return HF_Scn_Array[i].number;
		}
	}
	return 0;
}

/****************************************************
 * Function :- HF_FreeSCN
 *
 * Description :- Free an SCN back into the pool
 ***************************************************/

void HF_FreeSCN(u_int8 scn)
{
	int i;

	for (i=0;i<6;i++)
	{
		if(HF_Scn_Array[i].number == scn)
		{
			HF_Scn_Array[i].used = 0;
			return;
		}
	}
	return;
}

void HF_UpdateSDPRecord(u_int8 serverChannel)
{
#if SDP_RECORD_GEN
    SDP_RemoveRecord(HFP_SDPrecordHandle);
    HF_SetupSDPRecord(serverChannel, "RDA HandsFree", 13/*(u_int16)strlen("RDA HandsFree")*/);
#endif
}



/********************************************************************************
 * Low Power Mode Handling
 *
 * If no  ACL Data sent to AG for 3 sec [ and flow = GO ] we enter low power mode
 * If new data to Tx to an AG we exit Low Power mode and Tx data.
 *
 * Low Power mode to use is SNIFF with Tsniff = 400, Nsniff=12, Nwindow=8  [ To make it 
 * distinuishable from AG initiated sniff ]
 *********************************************************************************/

/***********************************************************************************
 * Function :- HF_Exit_Sniff_Mode
 *
 * Description 
 * This funcion initiates the request for an AG to exit from sniff mode.
 *
 ***********************************************************************************/

 void HF_Exit_Sniff_Mode(t_HFP_AG_Peer_Device* peer_device)
 {
	 int status;
#if COMBINED_HOST==1
	 t_lmp_link *p_link;
	 p_link = LMaclctr_Find_Handle(peer_device->acl_handle);
#endif

	 /* Send HCI Exit Sniff Mode */
	 if (peer_device->acl_state == HFP_DEVSNIFF)
	 {
#if COMBINED_HOST==0
		 status = HCI_ExitSniffMode(peer_device->acl_handle);
#else
		status = LMpolicy_LM_Exit_Sniff_Mode(p_link);
#endif
		 peer_device->acl_state = HFP_DEVPENDINGEXITSNIFF;
	 }
 }

/***********************************************************************************
 * Function :- HF_Enter_Sniff_Mode
 *
 * Description 
 * This funcion initiates the request for an AG to enter sniff mode.
 *
 ***********************************************************************************/
 void HF_Enter_Sniff_Mode(void *arg)
 {
	 int status;
	 t_HFP_AG_Peer_Device* peer_device = (t_HFP_AG_Peer_Device*)arg;  //zhujianguo added it. 2007.12.28

#if COMBINED_HOST==1
 	 t_lmp_link *p_link;
 	 p_link = LMaclctr_Find_Handle(peer_device->acl_handle);
#endif

	 if (peer_device->acl_state == HFP_DEVACTIVE)
	 {
         /********************************************************************************************
		  * 
		  * To get maximum interoperability we should not place a device into sniff mode when there 
		  * is an audio link active. There is nothing in out BT implementation which limits this BUT
		  * from experimentation many AGs do not like being placed in Sniff Mode when there is a SCO
		  * active.
		  * 
		  * In addition to allow us to trap failed Sniff requests on the HCI (ie Command_Status(error)) 
		  * we can only allow one AG to be transitioning into sniff mode at a time. The function 
		  * "HFP_Is_AG_Pending_Entry_Into_Sniff" checks if we have an AG which we are currently transitioning
		  * into sniff mode.
		  ********************************************************************************************/

		 if ((peer_device->audio_state == HFP_AUDIO_CLOSED) && !(HFP_AG_Pending_Entry_Into_Sniff()))
		 {
#if COMBINED_HOST==1
		 	LMpolicy_Enter_Sniff_Mode(p_link->device_index);
#else
			status = HCI_SniffMode(peer_device->acl_handle,800,800,6,4);
#endif
			peer_device->acl_state = HFP_DEVPENDINGSNIFF;
#if HFP_CORE_DEBUG
			//printf("\n\r[%08ld] HCI_SniffMode [Stat=0x%X, Acl=0x%X]", cjtmtick(), status, peer_device->acl_handle);
#endif
		 }
		 else // Delay Entry into Sniff 
		 {
			 /* Clear any pre-existing Low Power mode Timer */
			 if(peer_device->enter_sniff_mode_timer_handle)
			 {
#if COMBINED_HOST==1
				 host_timer_cancel(peer_device->enter_sniff_mode_timer_handle);
#endif
				 peer_device->enter_sniff_mode_timer_handle = 0;
			 }
			 
			 /* Set a new low power mode timer */
#if COMBINED_HOST==1
			peer_device->enter_sniff_mode_timer_handle = host_timer_create((unsigned int)PRH_HFP_SNIFFMODE_TIMEOUT,
																HF_Enter_Sniff_Mode,(void*)peer_device,pTIMER_ONESHOT);
#endif
		 }
	 }
	 else
	 {
#if HFP_CORE_DEBUG			
		// printf("\n\rIn Enter Sniff Mode = But device not in Active State. \n");
#endif
	 }

 }

/***********************************************************************************
 * Function :- HFP_AG_Pending_Entry_Into_Sniff
 *
 * Description 
 * This funcion checks if there is an AG which we are currently transitioning into
 * sniff mode. 
 *
 *  Returns :  
 *      pointer to AG device structure if sniff pending
 *      0 if no device pending entry into sniff.
 ***********************************************************************************/

t_HFP_AG_Peer_Device* HFP_AG_Pending_Entry_Into_Sniff(void)
{
	t_HFP_AG_Peer_Device* peer_device;
	int i;

	for(i=0;i<HFP_AG_NUMBER;i++)
	{
		peer_device = &HFP_AG_Device[i];
		if(peer_device->acl_state == HFP_DEVPENDINGSNIFF)
		{
			return peer_device;
		}
	}

	return 0;
}


void HF_Ring_Timeout(void *arg)
{
    t_HFP_AG_Peer_Device* peer_device = (t_HFP_AG_Peer_Device*)arg;
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Ring_Timeout "));
    HF_Send_RING(&peer_device->address,peer_device->dlci);
    // GF Note Have to include CLIP info
    //	HF_Send_CLIP(&peer_device->address,peer_device->dlci);
    peer_device->ring_timer = pTimerCreate((unsigned int)PRH_HFP_RING_TIMEOUT,
                                            		HF_Ring_Timeout,(void*)peer_device,pTIMER_ONESHOT);
     pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Ring_Timeout peer_device->ring_timer=%d",peer_device->ring_timer));
}

void HF_SLC_Setup_Timeout(void *arg)
{
	t_bdaddr temp_address;
	int i;
	t_HFP_AG_Peer_Device *peer_device=(t_HFP_AG_Peer_Device*)arg;

#if HFP_CORE_DEBUG			
	printf("\n\r SLC_SETUP_TIMEOUT CALLED ");
#endif
	if ((peer_device) && (peer_device->scm_state != SCM_OPEN) && (peer_device->slc_setup_timer_handle != 0))
	{
		peer_device->slc_setup_timer_handle = 0;

		for (i=0;i<6;i++)
			temp_address.bytes[i] = peer_device->address.bytes[i];

		Free_Audio_GW(&peer_device->address);
		/* Callback the Application to indicate the SLC is complete */
#if HFP_CORE_DEBUG	
		printf("\n\r Sending Callback for SLC Timeout ");
#endif
#if COMBINED_HOST==0
		if (HF_callbacks.HF_CB_SLC_Established_Ind)
			(*HF_callbacks.HF_CB_SLC_Established_Ind)(temp_address, BT_TIMEOUT);
#endif
	}
	else
	{
#if HFP_CORE_DEBUG	
		if (!peer_device)
		{
			printf("\n\r In SLC Timeout - peer_device is NULL");
		}
		else if (peer_device->scm_state != SCM_OPEN)
		{
			printf("\n\r In SLC Timeout - scm_state is OPEN");
		}
		else if (peer_device->slc_setup_timer_handle == 0)
		{
			printf("\n\r In SLC Timeout - Timer Handle = 0");
		}
#endif
	}
}


/*
	The status of the AG has been changed, so do something according the changing.
*/
/***********************************************************************************
 * Function :- GET_HF_AG_Status_Ind
 *
 * Description 
 *        Get the status indicator of the call, service and call setup. we only concerned these three indicator,
 *        the other indicators are: callheld, signal, roam and battchg. 
 *
 *  Returns :  
 *    None
 ***********************************************************************************/
u_int16 GET_HF_AG_Status_Ind(void)
{
	t_HFP_AG_Peer_Device* peer_device;
	u_int16 ind_status=0;
	

	peer_device = Find_Audio_GW_By_Address(&gCurrent_Active_Addr);

	if(peer_device)
	{
		ind_status = (peer_device->call_ind) |(peer_device->service_ind<<1)|(peer_device->call_setup_ind<<2);
	}
	
	return ind_status;
}

/***********************************************************************************
 * Function :- Get_Current_HFP_Call_Status
 *
 * Description 
 * This funcion will return the cuurent hf call status, so the app can determine what to do
 *
 *  Returns :  
 *      call sates,like HFP_CALL_ACTIVE etc.
 ***********************************************************************************/
int Get_Current_HFP_Call_Status(void)
{
	t_HFP_AG_Peer_Device* peer_device;
	
	peer_device = Find_Audio_GW_By_Address(&gCurrent_Active_Addr);
	if (peer_device)
	{
		return peer_device->call_state;
	}
	
	return HFP_CALL_CLOSED;
}



void HF_CB_ServiceDiscoveryResult(u_int8 outcome, t_bdaddr bdaddr, u_int8 service_channel_number)
{
    t_HFP_AG_Peer_Device* peer_device;
    u_int8 status=BT_NOERROR;
    peer_device = Find_Audio_GW_By_Address(&bdaddr);

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_CB_ServiceDiscoveryResult chn_num=%d\n",service_channel_number));
#endif       

    if(!peer_device)
    {
        peer_device = Find_Free_Audio_GW();
        if (peer_device)
        { 
            int i;

        for (i=0;i<6;i++)
            peer_device->address.bytes[i] = bdaddr.bytes[i];
        }
    }

    if (peer_device)
	{
   	 peer_device->serverChannel = service_channel_number;
   	 //HFP_SrvChan = service_channel_number;
   	 peer_device->active_profile = ACTIVE_PROFILE_HANDSFREE_AG;
 	}


    if(peer_device->scm_state == SCM_SDP_DISCOVERY_PENDING)
    {
        status=HF_Connect_As_Client(bdaddr);

#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_CB_ServiceDiscoveryResult status=%d\n",status));
#endif 
    }
    
}

void  HF_CB_SLC_Released_Ind( t_bdaddr bdaddr, u_int16 acl_handle)
{
#ifdef COOLSAND
    st_t_hfp_connect_cnf ret;
    int i;

#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_CB_SLC_Released_Ind acl_handle:0x%x \n",acl_handle));
#endif    

    ret.result = BT_NOERROR;
    ret.connection_id = acl_handle;
    for(i=0;i<6;i++)
        ret.bt_addr.bytes[i] = bdaddr.bytes[i];
    
    //hfp_cb.hfpDisconConf(&ret);
    rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_ACL_DISCONNECT_CNF,&ret);
#endif
}

void HF_CB_SLC_Established_Ind(t_bdaddr bdaddr,u_int8 status)
{
#ifdef COOLSAND    
    st_t_hfp_connect_cnf ret;
    int i=0;
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(&bdaddr);

#if pDEBUG
    if(peer_device)
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_CB_SLC_Established_Ind acl_handle:0x%x\n",peer_device->acl_handle));
#endif     

    if(status==BT_NOERROR)
        {
            pTimerCancel(gSLCConnectTimer);
			gSLCConnectTimer=0;
            ret.result = BT_NOERROR;
        }
    else
        ret.result = 1;
    
    for(i=0;i<6;i++)
        ret.bt_addr.bytes[i] = bdaddr.bytes[i];

    if(peer_device)
        ret.connection_id = peer_device->acl_handle;
    else
        ret.connection_id=0;
    
     gCurrent_Active_Profile = ACTIVE_PROFILE_HANDSFREE_AG;
     rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_ACL_CONNECT_CNF,&ret);
#endif  
}

void  HF_CB_SLC_Requested_Ind(t_bdaddr bdaddr)
{
#ifdef COOLSAND
    st_t_hfg_connect_ind ret;
    t_HFP_AG_Peer_Device* peer_device;
    u_int8 i;

    peer_device = Find_Audio_GW_By_Address(&bdaddr);
    for(i=0;i<6;i++)
        ret.bt_addr.bytes[i] = bdaddr.bytes[i];

    if(peer_device)
        ret.connection_id = peer_device->acl_handle;
    else
        ret.connection_id = 0;
    
    gCurrent_Active_Profile = ACTIVE_PROFILE_HANDSFREE_AG; 
    rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_ACL_CONNECT_IND,&ret);
#endif     
}

void  HF_CB_Audio_Link_Released(t_bdaddr bdaddr , int status)
{
#ifdef COOLSAND
    st_t_hfg_sco_connect_cnf ret;
    t_HFP_AG_Peer_Device* peer_device;

    peer_device = Find_Audio_GW_By_Address(&bdaddr);

    if(status==BT_NOERROR)
        ret.result = BT_NOERROR;
    else
        ret.result = 1;
   if(peer_device)//syzhou added
        ret.connection_id = peer_device->sco_handle;
   else
        ret.connection_id = 0;
   
   rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_SCO_DISCONNECT_CNF,&ret);
#endif
}

void  HF_CB_Audio_Link_Established(t_bdaddr bdaddr , int status)
{
#ifdef COOLSAND
    st_t_hfg_sco_connect_cnf ret;
    t_HFP_AG_Peer_Device* peer_device;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_CB_Audio_Link_Established is called ,status=%d",status));
#endif 
    peer_device = Find_Audio_GW_By_Address(&bdaddr);

    if(status==BT_NOERROR)
        ret.result = BT_NOERROR;
    else
        ret.result = 1;

    if(peer_device)
        ret.connection_id = peer_device->sco_handle; 
    else
        ret.connection_id =0; 
    
    //hfp_cb.hfpScoConConf(&ret);
    rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_SCO_CONNECT_CNF,&ret);
#endif
}
void  HF_CB_Audio_Request(t_bdaddr bdaddr)
{

}

void  HF_CB_Ring_Indication(t_bdaddr bdaddr)
{

}

void  HF_CB_Clip_Indication(t_bdaddr bdaddr, char* Clip,u_int8 len)
{

}

void  HF_CB_Call_Waiting(t_bdaddr bdaddr,char* Clip, u_int8 len)
{

}

void  HF_CB_Call_Active(t_bdaddr bdaddr)
{

}

/*
     Indication that the remote party is being alerted, and waiting for the remote to accept.
     Maybe we need to issue tone to let the user know.
*/
void HF_CB_Call_Alerting(t_bdaddr address)
{
	
}

void HF_CB_Remote_Supplementary_Feature_Update(t_bdaddr address, u_int8 remote_feature_status, u_int8 status)
{
/*
	if(remote_feature_status&HF_FEATURE_CALL_WAIT ==1)
	{
	
	}
	
	if(remote_feature_status&HF_FEATURE_CLIP ==1)
	{
	
	}
	
	if(remote_feature_status&HF_FEATURE_EC_NR ==1)
	{
	
	}

	if(remote_feature_status&HF_FEATURE_VR ==1)
	{
	
	}
	
	if(remote_feature_status&HF_FEATURE_INBAND_TONE ==1)
	{
	
	}	
	*/
}

void  HF_CB_Call_Release_Complete(t_bdaddr bdaddr,u_int8 reason, u_int8 status)
{

}

void  HF_CB_Send_DTMF_Complete(t_bdaddr bdaddr,u_int8 status)
{

}

void  HF_CB_Phone_Status_Ind(t_bdaddr address, u_int8 service_status, u_int8 call_status, u_int8 callsetup_status)
{

}

void  HF_CB_Volume_Setting_Change_Req(t_bdaddr bdaddr, u_int8 target, u_int8 volume_setting )
{

}

void  HF_CB_Change_Volume_Complete(t_bdaddr bdaddr)
{

}

void  HF_CB_Multiparty_Call_Handling_Ind(t_bdaddr bdaddr,  u_int8 call_handling)
{

}

void  HF_CB_Command_Complete(t_bdaddr bdaddr,  u_int8 API_command_id, u_int8 status)
{

}


int Is_AG_Support_In_Band_Ring(void)
{
	t_HFP_AG_Peer_Device* peer_device;
	
	peer_device = Find_Audio_GW_By_Address(&gCurrent_Active_Addr);
	
	if(peer_device && (peer_device->remote_feature_status & HF_FEATURE_INBAND_TONE) ==1 )
		return TRUE;
	
	return FALSE;
}

int Hfp_Get_BdAddr(t_bdaddr *addr)
{
	if(addr)
	{
		memcpy(addr->bytes, gCurrent_Active_Addr.bytes,sizeof(addr->bytes));
		return TRUE;
	}

	return FALSE;
}

#ifdef COOLSAND
//activate
void rda_bt_hfg_send_activate_req(void /*(*callbackFunc)(void *pArgs) */ )
{
    u_int8 status = BT_NOERROR;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hfg_send_activate_req\n"));
#endif  

    /* init the AG device Container */
    HFP_Init_AG_Containers();

    //HCI_WriteClassOfDevice(HANDS_FREE_AG_COD); 

    status=HCI_RegisterEventHandler(HFP_hciEventCallback);
    memset(&HF_callbacks,0,sizeof(HF_callbacks));
    HF_callbacks.HF_CB_ServiceDiscoveryResult = HF_CB_ServiceDiscoveryResult;
    HF_callbacks.HF_CB_SLC_Established_Ind = HF_CB_SLC_Established_Ind;
    HF_callbacks.HF_CB_SLC_Released_Ind = HF_CB_SLC_Released_Ind;
    HF_callbacks.HF_CB_SLC_Requested_Ind = HF_CB_SLC_Requested_Ind;
    HF_callbacks.HF_CB_Command_Complete = HF_CB_Command_Complete;
    HF_callbacks.HF_CB_Ring_Indication = HF_CB_Ring_Indication;
    HF_callbacks.HF_CB_Clip_Indication = HF_CB_Clip_Indication;
    HF_callbacks.HF_CB_Call_Waiting = HF_CB_Call_Waiting;
    HF_callbacks.HF_CB_Call_Alerting = HF_CB_Call_Alerting;
    HF_callbacks.HF_CB_Call_Release_Complete = HF_CB_Call_Release_Complete;
    HF_callbacks.HF_CB_Call_Active = HF_CB_Call_Active;
    HF_callbacks.HF_CB_Audio_Link_Released = HF_CB_Audio_Link_Released;
    HF_callbacks.HF_CB_Audio_Link_Established = HF_CB_Audio_Link_Established;
    HF_callbacks.HF_CB_Volume_Setting_Change_Req = HF_CB_Volume_Setting_Change_Req;
    HF_callbacks.HF_CB_Change_Volume_Complete = HF_CB_Change_Volume_Complete;
    HF_callbacks.HF_CB_Remote_Supplementary_Feature_Update = HF_CB_Remote_Supplementary_Feature_Update;
    HF_callbacks.HF_CB_Send_DTMF_Complete = HF_CB_Send_DTMF_Complete;
    HF_callbacks.HF_CB_Phone_Status_Ind = HF_CB_Phone_Status_Ind;
    HF_callbacks.HF_CB_Audio_Request = HF_CB_Audio_Request;

    /* register as a RFCOMM server */
    memset(&rfcommCallbacks,0,sizeof(rfcommCallbacks));
    rfcommCallbacks.connectionConfirmation = HF_rfcommConnectConfirmCallback;
    rfcommCallbacks.connectionIndication = HF_rfcommConnectIndicationCallback;
    rfcommCallbacks.dataIndication = HF_rfcommRxDataCallback;
    rfcommCallbacks.disconnectionIndication = HF_rfcommDisconnectIndicationCallback;
    rfcommCallbacks.flowIndication = HF_rfcommFlowCtrlCallback;
        {
        /* As the RFCOMM and Stack Entity model are designed on a P2P 
        * client server model the current Stack Entity supports only 
        * a single ACL link per registered server channel when acting
        * as a server. Thus most risk free approach to allow 6 peers 
        * connect into the system is to support 6 seperate servers.
        *
        * Register 6 server Ports from HFP_SrvChan to HFP_SrvChan +12
        * and Init the SCN allocation array 
        */

        int i;
        u_int8 temp_scn;
            for (i=0;i < NUM_SCNS_SUPPORTED; i++)
            {
                temp_scn = i+1;

                HF_Scn_Array[i].number = temp_scn;
                HF_Scn_Array[i].used = 0;
                status = RSE_SrvRegisterPort(&temp_scn, &rfcommCallbacks);
            }
        }

    /* enable authentication and encryption */
#if USE_SECURITY
    MGR_SetSecurityMode(MGR_LINKLEVEL,NULL);
    HCI_WriteEncryptionMode(0x01);
#endif

    HFP_Browse_Pending=0;

    //callbackFunc(&status);
    rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_ACTIVATE_CNF,&status);
}
void rda_bt_hfg_send_CallSetup_1_ind(void)//zhou siyou add 20070730
{	
	if(!	Bldn_Present_Flag)
		HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,2,&HFP_AG_Device[0].address,HFP_AG_Device[0].dlci);
}
void rda_bt_hfg_send_BldnFail_ind(void)//syzhou added
{
	HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,0,&HFP_AG_Device[0].address,HFP_AG_Device[0].dlci);
	Bldn_Present_Flag=0;
}
extern void rda_bt_hsg_send_reject_cfm();
void rda_bt_hfg_send_reject_cfm()//syzhou added
{
    if(gCurrent_Active_Profile==ACTIVE_PROFILE_HANDSFREE_AG)
    {
	if (HFP_AG_Device[0].ring_timer)
	{
		#if COMBINED_HOST==1
			host_timer_cancel(HFP_AG_Device[0].ring_timer);
		#else
			pTimerCancel(HFP_AG_Device[0].ring_timer);   // added by cliff
		#endif
		HFP_AG_Device[0].ring_timer = 0;
	}	
	HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,0,&HFP_AG_Device[0].address,HFP_AG_Device[0].dlci);
    }
    else if(gCurrent_Active_Profile==ACTIVE_PROFILE_HEADSET_AG)
    {
        rda_bt_hsg_send_reject_cfm();
    }

}
// deactivate
void rda_bt_hfg_send_deactivate_req(void /*(*callbackFunc)(void *pArgs)*/ )
{
    u_int8 status = BT_NOERROR;
    
    status=HF_Close();

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hfg_send_deactivate_req status=%d\n",status));
#endif  

    rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_DEACTIVATE_CNF,&status);
}

// connect
void rda_bt_hfg_send_connect_req(u_int8 chn_num, t_bdaddr device_addr /*, void (*callbackFunc)(void *pArgs) */ )
{
    u_int8 result = BT_NOERROR;
    t_HFP_AG_Peer_Device* peer_device;
    
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hfg_send_connect_req chn_num=%d\n",chn_num));
#endif  
    gCurrent_Active_Profile = ACTIVE_PROFILE_HANDSFREE_AG;//zhou siyou 20090719
gFirstFlag = 1;
if(chn_num==0)
    result=HF_Setup_SLC(device_addr);
else
{
    peer_device = Find_Audio_GW_By_Address(&device_addr);
    if(!peer_device)
    {
        int i;
        peer_device = Find_Free_Audio_GW();

      if(peer_device)
      	{
      	
        for (i=0;i<6;i++)
            peer_device->address.bytes[i] = device_addr.bytes[i];

        peer_device->serverChannel = chn_num;  
      	}
		
        //HFP_SrvChan = chn_num;
    }

    if(peer_device)
    {
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hfg_send_connect_req chnl_num=%d\n",peer_device->serverChannel));
#endif 

       peer_device->active_profile = ACTIVE_PROFILE_HANDSFREE_AG;
        result = HF_Connect_As_Client(device_addr);
    }
}
        
}

// disconnect
void rda_bt_hfg_send_disconnect_req(u_int32 cid, t_bdaddr device_addr /*, void (*callbackFunc)(void *pArgs)*/ )
{
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hfg_send_disconnect_req\n"));
#endif      
    //hfp_Set_Disconnection_Cnf_callback(callbackFunc);
    HF_Release_SLC(device_addr);
}

//sco connect
extern int HS_Audio_Setup(u_int16 handle);
extern int HS_Audio_Release(void);
void rda_bt_hfg_send_sco_connect_req( u_int16 connect_id)
{
    if(gCurrent_Active_Profile==ACTIVE_PROFILE_HANDSFREE_AG)
    {
        t_HFP_AG_Peer_Device* peer_device; 

#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hfg_send_sco_connect_req connect_id=0x%x\n",connect_id));
#endif      
        // hfp_Set_Sco_Connection_Cnf_callback(CallbackFunc);

        peer_device=Find_Audio_GW_By_ACL_Handle(connect_id);

#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hfg_send_sco_connect_req peer_device=0x%x\n",peer_device));
#endif      

        if (peer_device && peer_device->ring_timer)//syzhou added
        {
#if COMBINED_HOST==1
            host_timer_cancel(peer_device->ring_timer);
#else
            pTimerCancel(peer_device->ring_timer);   
		peer_device->ring_timer=0;
#endif
            peer_device->ring_timer = 0;
        }

        if(peer_device)
        {
            HF_Audio_Setup(peer_device->address);
            if(Bldn_Present_Flag)
            {
                // send +CIEV:3,3
                // send +CIEV:4,3
                HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,3,&peer_device->address,peer_device->dlci);
                Bldn_Present_Flag=0;
            }//syzhou added ,only process the AT+BLDN command need send abrove
            if(!g_call_answered_by_hf) 
            {            
                // Send +CIEV:(call=1)
                HF_Send_Indicators(HFP_CALL_INDICATOR, 1, &peer_device->address, peer_device->dlci);

                // Send +CIEV:(callsetup=0)
                HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,0,&peer_device->address,peer_device->dlci);
            }
            else
                g_call_answered_by_hf = FALSE;
        }
    }
    else if(gCurrent_Active_Profile==ACTIVE_PROFILE_HEADSET_AG)
    {
        HS_Audio_Setup(connect_id);
    }
}


//sco disconnect
void rda_bt_hfg_send_sco_disconnect_req(u_int16 connect_id )
{
    int status = BT_NOERROR;
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hfg_send_sco_disconnect_req gCurrent_Active_Profile=%d\n", gCurrent_Active_Profile));
#endif  

    if(gCurrent_Active_Profile==ACTIVE_PROFILE_HANDSFREE_AG)
    {
        t_HFP_AG_Peer_Device* peer_device; 
        
       // hfp_Set_Sco_Disconnection_Cnf_callback(CallbackFunc);

        peer_device=&HFP_AG_Device[0];//Find_Audio_GW_By_ACL_Handle(connect_id);

        if(peer_device && peer_device->acl_handle== connect_id)
        {
            pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"peer_device->dlci=%d\n", peer_device->dlci));
            status=HF_Audio_Release(peer_device->address);
            pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"status=%d\n", status));
            // send +CIEV:1,1
            //HF_Send_Indicators(HFP_CALL_INDICATOR,0,&peer_device->address,connect_id);//syzhou added
            // send +CIEV:3,0
            // send +CIEV:4,0
            //HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,0,&peer_device->address,connect_id);//only send call=0
            HF_Send_Indicators(HFP_CALL_INDICATOR,0,&peer_device->address,peer_device->dlci);//syzhou added
        }
    }
    else if(gCurrent_Active_Profile==ACTIVE_PROFILE_HEADSET_AG)
    {
        HS_Audio_Release();
    }
}


void rda_bt_hfg_send_set_volume_req( u_int16 connect_id,u_int8 gain)
{
    t_HFP_AG_Peer_Device* peer_device;


    peer_device=Find_Audio_GW_By_ACL_Handle(connect_id);

    if(peer_device)
    {
        HF_Change_Volume_Setting(peer_device->address ,HFP_SPEAKER,gain);
    }
	
}

extern void rda_bt_hsg_send_ring(void);
void rda_bt_hfg_send_ring(void)
{
     pDebugPrintfEX((pLOGERROR,pLOGHFP,"rda_bt_hfg_send_ring gCurrent_Active_Profile=%d\n",gCurrent_Active_Profile));
    if(gCurrent_Active_Profile==ACTIVE_PROFILE_HANDSFREE_AG)
    {
	t_HFP_AG_Peer_Device* peer_device;
	
        peer_device = &HFP_AG_Device[0];
#if pDEBUG
      if( peer_device)
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HF_Send_RING dlci=%d\n",peer_device->dlci));
#endif 
        if(peer_device && peer_device->scm_state != SCM_OPEN)  //just send RING indicator after slc build
            return ;
		
		HF_Send_Indicators(HFP_CALL_INDICATOR,0,&HFP_AG_Device[0].address,HFP_AG_Device[0].dlci);
	HF_Send_Indicators(HFP_CALLSETUP_INDICATOR,1,&HFP_AG_Device[0].address,HFP_AG_Device[0].dlci);
	HF_Send_RING(&HFP_AG_Device[0].address,HFP_AG_Device[0].dlci);

	
	peer_device->ring_timer = pTimerCreate((unsigned int)PRH_HFP_RING_TIMEOUT,
											HF_Ring_Timeout,(void*)peer_device,pTIMER_ONESHOT);
	
		if(peer_device->ring_timer)
			     pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hfg_send_ring timer creat ok,peer_device->ring_timer=%d\n",peer_device->ring_timer));
		else
				 pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hfg_send_ring timer creat fail\n"));


			
    }
    else if(gCurrent_Active_Profile==ACTIVE_PROFILE_HEADSET_AG)
    {
        rda_bt_hsg_send_ring();
    }
}

void rda_bt_hfg_send_call_number(u_int8* callNum)  //send call line identification notification
{

    t_HFP_AG_Peer_Device* peer_device;	
    peer_device = &HFP_AG_Device[0];
    if(peer_device->scm_state != SCM_OPEN)  //just send call number after slc build
        return ;
    
    if(gCurrent_Active_Profile == ACTIVE_PROFILE_HANDSFREE_AG)
    {
        int offset = 0;
	  int len = strlen((char*)callNum);
        char * CLIP_Command = OS_HEAP_Alloc(30 + len);
        
        memset(CLIP_Command, 0, 25 + len);
        memcpy(CLIP_Command, "\r\n+CLIP: \"", 10);
        offset += 10;
        
        memcpy(CLIP_Command + offset, callNum, len);      //number
        offset += len; 
        
        memcpy(CLIP_Command + offset, "\",129,", 6);         //type
        offset += 6;
        
        memcpy(CLIP_Command + offset, "\"\",0,\"\",0", 9);   //other parameter will  ignored by HF but need send also
        offset += 9;  
        
        memcpy(CLIP_Command + offset, "\r\n", 2);
        offset += 2;   
        
        HF_SendToGateway((u_int8*)CLIP_Command, offset ,&HFP_AG_Device[0].address,HFP_AG_Device[0].dlci);
        OS_HEAP_Free(CLIP_Command);
    }
}//added by xiongzhi chen
#endif
