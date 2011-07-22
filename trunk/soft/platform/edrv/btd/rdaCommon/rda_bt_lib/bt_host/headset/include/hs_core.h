/*
 * MODULE NAME:    hs_core.h
 * DESCRIPTION:    Headset Profile Core Headset Application interface header
 * AUTHOR:         Jenny Murphy
 *
 * SOURCE CONTROL: $Id: hs_core.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2003 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef PRH_HS_CORE_H
#define PRH_HS_CORE_H

#define PARKIDLEACL     0
#define INBANDRINGING   0
#define HEADSETCOD      0x200404

/* see HCI spec for other bit encodings - 0x60 is the default*/
/*#define VOICESETTING 0x60 */
#define VOICESETTING 0xac 



#define PKT_HV1     0x20
#define PKT_HV2     0x40
#define PKT_HV3     0x80

#define HS_ATCKPDSTRING     "AT+CKPD=200\r"
#define HS_ATSPKRVOLSTRING  "AT+VGS="
#define HS_ATMICVOLSTRING   "AT+VGM="
#define HS_SPKRVOLSTRING    "\r\n+VGS="
#define HS_MICVOLSTRING     "\r\n+VGM="
#define HS_OKSTRING         "\r\nOK\r\n"
#define HS_ERRORSTRING      "\r\nOK\r\n"
#define HS_RINGSTRING       "\r\nRING\r\n"
#define HS_ATCKPD     "AT+CKPD\r"

#define PARKMINBEACON 0x1000
#define PARKMAXBEACON 0x4000

enum linkState {CONNECTED, PENDING, PARKED, DISCONNECTED};

int HS_Init(void);
int HS_Shutdown(void);
int HS_ConnectToGateway(void);
void HS_SetupSCO(void);
void HS_ScoSetupCompleteCallback (u_int16 *scoHandle, int status);

int HS_SetupSDPRecord(u_int8 rf_serverChannel, u_int8 volCtrlStatus, char *serviceName, u_int16 serviceNameLen);
int HS_RetrieveSDPParms(t_bdaddr address, u_int8 *rf_serverChannel);
int HS_SendToGateway(u_int8 *data, int dataLen);
void HS_HandleRxATCommand(u_int8 *data, int dataLen);
int HS_HeadsetButtonPressed(void);
int HS_CreateAudioLink(void);
void HS_SetHeadsetMicVol(int volumeSetting);
void HS_SetHeadsetSpkrVol(int volumeSetting);
int HS_rfcommRxDataCallback(t_bdaddr address, u_int8 dlci, struct host_buf * rxData);
int HS_rfcommConnectIndicationCallback(u_int8 schan, t_bdaddr address, u_int8 dlci, u_int16 max_frame_size);
int HS_rfcommConnectConfirmCallback(u_int8 schan, t_bdaddr address, u_int8 status, u_int8 dlci, u_int16 max_frame_size);
int HS_rfcommDisconnectIndicationCallback(t_bdaddr address, u_int8 dlci);	
int HS_rfcommFlowCtrlCallback(t_bdaddr address, u_int8 dlci,  u_int8 flow_state);
void scoEstablishedCallback(u_int16 handle, int status);
void hciEventCallback(u_int8 *eventPdu);
int HS_ReleaseAudioLink(void);
void HS_MicVolChanged(int volumeSetting);
void HS_SpkrVolChanged(int volumeSetting);

#ifdef COOLSAND
typedef struct
{
    u_int8         result;
    t_bdaddr       bt_addr;
    u_int16         connection_id;
} st_t_hsp_connect_cnf;

typedef struct
{
    t_bdaddr bt_addr;
    u_int16   connection_id;    
} st_t_hsg_connect_ind;

typedef struct
{
    t_bdaddr bt_addr;
    u_int16   connection_id;    
} st_t_hsg_disconnect_ind;

typedef struct
{
    u_int8 result;
    u_int16 connection_id;
} st_t_hsg_sco_connect_cnf;

typedef struct
{
    u_int8 gain;
    u_int16 connection_id;
} st_t_hsg_gain_ind;

int HS_CreateACLConnect(u_int8 chn_num, t_bdaddr address) ;
int HS_Send_Ring();
int HS_Set_Speaker_Gain_To_HS(u_int8 gain);
void HS_SCO_Release_Mgr_Callback(u_int16 scoHandle, t_api status);
#endif

#endif
