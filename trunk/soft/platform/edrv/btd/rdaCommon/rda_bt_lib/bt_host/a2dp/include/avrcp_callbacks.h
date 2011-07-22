/***********************************************************************
 *
 * MODULE NAME:    avdtp_callbacks.h
 * DESCRIPTION:     Audio/Video Distribution Profile callback header
 * MAINTAINER:     Mark Donnelly
 *
 *
 * LICENSE:
 *     This source code is copyright (c) 2008  RDA Ltd.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#ifndef AVRCP_CALLBACKS
#define AVRCP_CALLBACKS

#include "host_config.h"
#include  "l2cap_primitives.h"

void Avrcp_L2cap_Connect_Ind_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs);

void Avrcp_L2cap_Disconnect_Ind_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs);

void Avrcp_L2cap_Data_Ind_Cb(u_int16 cid, struct host_buf * p_buf);

void Avrcp_L2cap_Disconnect_Cfm_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs);

void Avrcp_L2cap_Connect_Cfm_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs);
void Avrcp_L2cap_Config_Ind_Cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs);
void Avrcp_L2cap_Configure_Cfm_Cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs);
void Avrcp_L2cap_Qos_Violation_Ind_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs);

#if 0//def COOLSAND
void Avrcp_Set_Connect_Cfm_Callback(u_int8 chnl_num, u_int8 local_role);
void Avrcp_Set_Connect_Ind_Callback(void (*CallbackFunc)(void *pArgs));
void Avrcp_Set_Disconnect_Cfm_Callback(void (*CallbackFunc)(void *pArgs));
void Avrcp_Set_Disconnect_Ind_Callback(void (*CallbackFunc)(void *pArgs));
#endif

#endif

