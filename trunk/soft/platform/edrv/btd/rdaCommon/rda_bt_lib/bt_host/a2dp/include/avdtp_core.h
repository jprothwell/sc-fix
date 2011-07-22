/***********************************************************************
 *
 * MODULE NAME:    avdtp_core.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Audio/video distribution transport profile
 * MAINTAINER:     Mark Donnelly
 *
 *
 * LICENSE:
 *     This source code is copyright (c) 2008 RDA Ltd.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/
#ifndef AVDTP_CORE
#define AVDTP_CORE

#if COMBINED_HOST==1
#include "sys_types.h"
#endif

#include "host_config.h"
#include "avdtp.h"
#include "host_types.h"
#include "host_ctrl.h"

#include "a2dp.h"

void AVDTP_RegisterPSM(void);
void Avdtp_Register_Sep(t_A2DP_SEP_TYPE sep_type);
t_api Avdtp_Layer_Ctrl(u_int8 flags);
void Avdtp_Request_Dispatcher(u_int8* pdu,t_channelId cid, u_int16 packet_size, t_AVDTP_TYPE type, t_bdaddr bdaddr);
t_SEP_STATE Get_Sep_State(void);

boolean Avdtp_Get_BdAddr(t_bdaddr *addr);
void Avdtp_Handle_Suspend_Req(void);
void Avdtp_Handle_Start_Req(void);
void Avdtp_Handle_Close_Req(void);

boolean Avdtp_Send_Discover(sep* sep_entry, u_int16 cid);
boolean Aavdp_Send_GetCapabilities(sep* sep_entry, u_int8 seid, u_int16 cid);

#ifdef COOLSAND
boolean Avdtp_Send_Reconfigure(u_int16 connect_id, u_int8 acp_seid, u_int8 codec_type,bt_a2dp_audio_codec_cap_struct* audio_cap);
boolean Avdtp_Send_SetConfiguration(u_int16 connect_id, u_int8 acp_seid, u_int8 int_seid, u_int8 codec_type, bt_a2dp_audio_codec_cap_struct* audio_cap);
boolean Avdtp_Send_Open(u_int8 acp_seid);
#else
boolean Avdtp_Send_Reconfigure(sep* sep_entry, u_int8* config, u_int16 config_size);
boolean Avdtp_Send_SetConfiguration(sep* sep_entry);
boolean Avdtp_Send_Open(sep* sep_entry);
#endif
boolean Avdtp_Send_Start(sep* sep_entry);
boolean Avdtp_Send_Suspend(sep* sep_entry);
boolean Avdtp_Send_Close(sep* sep_entry);
boolean Avdtp_Send_Abort(sep* sep_entry);

void Avdtp_Start_Discover(u_int16 cid,u_int8 seid);
void Avdtp_Start_Start(void);
int Avdtp_Send_Data(u_int32 length, u_int8* p_data, u_int32 timestamp,u_int16 seq_num, u_int8 payload_type, u_int8 marker );

#ifdef COOLSAND
void Avdtp_Set_Sep_Discover_Conf_callback(void (*CallbackFunc)(void *pArgs));

void Avdtp_Set_Sep_Get_Capabilities_Conf_callback(void (*CallbackFunc)(void *pArgs));

void Avdtp_Set_Sep_Get_Config_Conf_callback(void (*CallbackFunc)(void *pArgs));

void Avdtp_Set_Sep_Set_Config_Conf_callback(void (*CallbackFunc)(void *pArgs));

void Avdtp_Set_Sep_Reconfig_Conf_callback(void (*CallbackFunc)(void *pArgs));

void Avdtp_Set_Sep_Open_Conf_callback(void (*CallbackFunc)(void *pArgs));

void Avdtp_Set_Sep_Start_Conf_callback(void (*CallbackFunc)(void *pArgs));

void Avdtp_Set_Sep_Close_Conf_callback(void (*CallbackFunc)(void *pArgs));

void Avdtp_Set_Sep_Suspend_Conf_callback(void (*CallbackFunc)(void *pArgs));

void Avdtp_Set_Sep_Abort_Conf_callback(void (*CallbackFunc)(void *pArgs));

void AVDTP_Start_Media_Channel_Connection(void);

u_int8 Avdtp_Get_Codec_Type(void);
void Avdtp_Send_Sep_Stream_Config_Res(u_int16 result, u_int8 stream_handle);
void Avdtp_Send_Sep_Stream_Open_Res(u_int16 result, u_int8 stream_handle);
void Avdtp_Send_Sep_Stream_Abort_Res( u_int8 stream_handle);
void Avdtp_Send_Sep_Discover_Res(u_int16 cid, bt_sep_info_struct* sep_list);
void Avdtp_Send_Sep_Capabilities_Get_Res(u_int16 cid, u_int8 codec_type, bt_a2dp_audio_codec_cap_struct* audio_cap);
void Avdtp_Send_Sep_Stream_Start_Res(u_int16 result, u_int8 stream_handle);
void Avdtp_Send_Sep_Stream_Suspend_Res(u_int16 result, u_int8 stream_handle);
void Avdtp_Send_Sep_Stream_Close_Res(u_int16 result, u_int8 stream_handle);
#endif
#endif /* AVDTP_CORE */

