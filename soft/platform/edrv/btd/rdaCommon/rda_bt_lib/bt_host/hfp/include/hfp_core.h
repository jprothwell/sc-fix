/**************************************************************************
 * MODULE NAME:    hfp_core.h
 * PROJECT CODE:
 * DESCRIPTION:    HFP Constants
 * AUTHOR:         Gary Fleming, Daire McNamara
 * DATE:           15-10-2004
 *
 * SOURCE CONTROL: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2004 Ceva Inc.
 *     All rights reserved.
 *
 * DESCRIPTION
 *  Public header file for HFP module	
 *
 **************************************************************************/

#ifndef __HFP_CORE_H
#define __HFP_CORE_H

/*#include "basic_types.h"
#include "typedefs.h"*/

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
	void  (*HF_CB_SLC_Released_Ind)( t_bdaddr ag_bdaddr, u_int16 acl_handle);
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

#ifdef COOLSAND
struct hfp_cbs 
{

    void (*hfpConConf)( void *pArgs);
    void (*hfpDisconConf)( void *pArgs);
    void (*hfpScoConConf)( void *pArgs);
    void (*hfpScoDisconConf)( void *pArgs);
};

typedef struct hfp_cbs hfp_callbacks;

typedef struct
{
    u_int8         result;
    t_bdaddr       bt_addr;
    u_int16         connection_id;
} st_t_hfp_connect_cnf;

typedef struct
{
    t_bdaddr bt_addr;
    u_int16   connection_id;    
} st_t_hfg_connect_ind;


typedef struct
{
	u_int8	result;
	u_int16 	connection_id;
} st_t_hfg_sco_connect_cnf;

typedef struct
{
    u_int8    gain;
    u_int16  connection_id;
} st_t_hfg_gain_ind_struct;

#endif

/*******************************************************************/
/*                        Exported Vars                            */
/*******************************************************************/
//extern bool hfp_allowIncomingSLC; //Control incoming SLC
//extern u_int8 hfp_RingingCallId; //Call-Id of Ringing call
//extern u_int8 hfp_ActiveCallId;  //Call-Id of Active call
//extern u_int8 hfp_channel_server; //Passed by Application

/*******************************************************************/
/*                        Prototypes                               */
/*******************************************************************/
#if COMBINED_HOST==0
int HF_Init(u_int8 server_channel_number, st_t_hf_Callbacks* app_hf_callbacks);
#else
int HF_Init(void);
#endif
int HF_Close(void);
int HF_ServiceDiscover_AG(t_bdaddr address);

int HF_Setup_SLC(t_bdaddr ag_bdaddr);
int HF_Release_SLC(t_bdaddr ag_bdaddr);
int HF_Accept_SLC(t_bdaddr ag_bdaddr);
int HF_Reject_SLC(t_bdaddr ag_bdaddr);

int HF_Call_Request(t_bdaddr ag_bdaddr, u_int8  Type, char* string);
int HF_Call_Accept(t_bdaddr ag_bdaddr);
int HF_Call_Reject (t_bdaddr ag_bdaddr);
int HF_Call_Release (t_bdaddr ag_bdaddr);
int HF_Multiparty_Call_Handling(t_bdaddr ag_bdaddr,  u_int8 Action );

int HF_Audio_Setup(t_bdaddr ag_bdaddr);
int HF_Audio_Release(t_bdaddr ag_bdaddr);
int HF_Audio_Accept(t_bdaddr ag_bdaddr, u_int8 packet_type);
int HF_Audio_Reject(t_bdaddr ag_bdaddr);

int HF_Change_Volume_Setting(t_bdaddr ag_bdaddr , u_int8 target, u_int8 volume_setting );
int HF_Read_Remote_Supplementary_Feature(t_bdaddr ag_bdaddr);
int HF_Send_DTMF(t_bdaddr ag_bdaddr, char* dtmf_sequence );
int HF_Read_Phone_Status_Information(t_bdaddr ag_bdaddr);
int HF_Modify_Remote_Supplementary_Feature(t_bdaddr ag_bdaddr,u_int8 feature_id , u_int8 enable);
int HF_Retrieve_Multiparty_Call_Handling(t_bdaddr ag_bdaddr);       // New API
int HF_Retrieve_Memory(t_bdaddr ag_bdaddr,u_int8 num_mem_locs);

void HFP_CliMenu (void);

int HF_Device_State(t_bdaddr ag_bdaddr);
//int HF_Configure(u_int8 connectable);
u_int16 GET_HF_AG_Status_Ind(void);
int Get_Current_HFP_Call_Status(void);
void HF_CB_Remote_Supplementary_Feature_Update(t_bdaddr address, u_int8 remote_feature_status, u_int8 status);
int Is_AG_Support_In_Band_Ring(void);
void HF_CB_Call_Alerting(t_bdaddr address);
int Hfp_Get_BdAddr(t_bdaddr *addr);
void rda_bt_hfg_send_CallSetup_1_ind(void);//zhou siyou add 20070730

#endif  //__CTP_CORE_H
