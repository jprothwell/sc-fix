/***********************************************************************
 *
 * MODULE NAME:    avdtp_request_decoder.h
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
#ifndef AVRCP_CORE
#define AVRCP_CORE

#include "host_config.h"
#include "avrcp.h"
#include "host_types.h"


void Avrcp_Init(void);
void AVRCP_RegisterPSM(void);
t_api Avctp_Layer_Ctrl(u_int8 flags) ;
//int sdp_init_AVRCP(void);
void Avrcp_Set_State(t_AVRCP_STATE state);
void Avrcp_Connect_Req(t_bdaddr bdaddr);
t_AVRCP_STATE Get_Avrcp_State(void);
void Headset_Send_Pause(void);
void Headset_Send_Play(void);
void Headset_Send_Stop(void);
void Headset_Send_Forward(void);
void Headset_Send_Backward(void);
void Headset_Send_Fast_Forward(void);
void Headset_Send_Rewind(void);
void Avrcp_Handle_Received_Data(u_int8* ptr,t_channelId cid, u_int16 packet_size);

#endif /* AVRCP_CORE*/

