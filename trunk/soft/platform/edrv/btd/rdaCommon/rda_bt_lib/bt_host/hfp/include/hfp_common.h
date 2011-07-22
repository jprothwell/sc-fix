/**************************************************************************
 * MODULE NAME:    hfp_common.h
 * PROJECT CODE:
 * DESCRIPTION:    HFP common header
 * AUTHOR:         Gary Fleming, Daire McNamara
 * DATE:           15-10-2004
 *
 * SOURCE CONTROL: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2008 RDA Inc.
 *     All rights reserved.
 *
 * DESCRIPTION
 *  Public header file for HFP module	
 *
 **************************************************************************/

#ifndef __HFP_COMMON_H
#define __HFP_COMMON_H

#ifndef BD_ADDR_CMP
#define BD_ADDR_CMP(x,y) !pMemcmp(((x).bytes), ((y).bytes), 6)
#endif

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
#define NUM_SCNS_SUPPORTED 7

/* defines for flow indication */
#define RSE_FLOW_STOP 0
#define RSE_FLOW_GO 1

/* defines for connection confirmation */
#define RSE_CONNECT_REFUSE 0
#define RSE_CONNECT_ACCEPT 1

/*****************************************************************************/
/*                        Stuctures Definitions                              */
/*****************************************************************************/
//AG supported features bit masks .
#define AG_THREE_WAY_CALLING         (1)
#define AG_NREC_FUNCTION                 (1<<1)
#define AG_VOICE_RECOGNITION          (1<<2)
#define AG_IN_BAND_RING                    (1<<3)
#define AG_VOICE_TAG                          (1<<4)
#define AG_REJECT_CALL                       (1<<5)
#define AG_ENHANCED_CALL_STATUS   (1<<6)
#define AG_ENHANCED_CALL_CONTROL (1<<7)
#define AG_EXTENDED_ERROR_CODES   (1<<8)

/*Bits for HF_Modify_Remote_Supplementary_Feature(). */
typedef enum{
	HF_FEATURE_CALL_WAIT	= 0x01,
	HF_FEATURE_CLIP			= 0x02,
	HF_FEATURE_EC_NR		= 0x04,
	HF_FEATURE_VR			= 0x08,
	HF_FEATURE_INBAND_TONE	= 0x10
} HF_REMOTE_FEATURE;

/*Bits for HF_Call_xxxxx(), etc */
typedef enum{
	HF_CALL_TYPE_NULL,
	HF_CALL_TYPE_NORMAL,
	HF_CALL_TYPE_MEM,
	HF_CALL_TYPE_REDIAL
} HF_CALL_TYPE;


/*Bits for HF_Multiparty_Call_Handling() */
typedef enum{
	PARTY_CURR_ACTIVE_OTHER_DISMISS	= 0,
	PARTY_CURR_RELEASE_OTHER_ACCEPT,
	PARTY_CURR_HOLD_OTHER_ACCEPT,
	PARTY_JOIN_3WAY,
	PARTY_LEAVE_3WAY
} HF_PART_CALL_TYPE;

typedef struct st_t_hf_Callbacks
{
	void  (*HF_CB_SLC_Established_Ind)(t_bdaddr ag_bdaddr,u_int8 status);
	void  (*HF_CB_SLC_Released_Ind)( t_bdaddr ag_bdaddr, u_int8 reattaching);
	void  (*HF_CB_SLC_Requested_Ind)(t_bdaddr ag_bdaddr);
	void  (*HF_CB_ServiceDiscoveryResult)  (u_int8 outcome, t_bdaddr ag_bdaddr, u_int8 service_channel_number);

	void  (*HF_CB_Audio_Link_Released)(t_bdaddr ag_bdaddr , int status);
	void  (*HF_CB_Audio_Link_Established)(t_bdaddr ag_bdaddr , int status);
	void  (*HF_CB_Audio_Request)(t_bdaddr ag_bdaddr);

	void  (*HF_CB_Ring_Indication)(t_bdaddr ag_bdaddr);
	void  (*HF_CB_Clip_Indication)(t_bdaddr ag_bdaddr, char* Clip,u_int8 len);
	void  (*HF_CB_Call_Waiting)(t_bdaddr ag_bdaddr,char* Clip, u_int8 len);
	void  (*HF_CB_Call_Active)(t_bdaddr ag_bdaddr);
	void  (*HF_CB_Call_Alerting)(t_bdaddr ag_bdaddr);
	void  (*HF_CB_Call_Release_Complete)(t_bdaddr ag_bdaddr,u_int8 reason, u_int8 status);

	void  (*HF_CB_Remote_Supplementary_Feature_Update) (t_bdaddr ag_bdaddr, u_int8 status_byte, u_int8 status);
	void  (*HF_CB_Send_DTMF_Complete)(t_bdaddr ag_bdaddr,u_int8 status);
	void  (*HF_CB_Phone_Status_Ind)(t_bdaddr address, u_int8 service_status, u_int8 call_status, u_int8 callsetup_status);
	void  (*HF_CB_Volume_Setting_Change_Req)(t_bdaddr ag_bdaddr, u_int8 target, u_int8 volume_setting );
	void  (*HF_CB_Change_Volume_Complete)(t_bdaddr ag_bdaddr);
	void  (*HF_CB_Multiparty_Call_Handling_Ind)(t_bdaddr ag_bdaddr,  u_int8 call_handling); 
	void  (*HF_CB_Command_Complete)(t_bdaddr ag_bdaddr,  u_int8 API_command_id, u_int8 status);
} st_t_hf_Callbacks;


#endif  //__HFP_COMMON_H
