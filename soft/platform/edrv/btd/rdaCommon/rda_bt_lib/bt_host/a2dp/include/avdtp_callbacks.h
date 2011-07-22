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

#ifndef AVDTP_CALLBACKS
#define AVDTP_CALLBACKS

#include "host_config.h"
#include  "l2cap_primitives.h"

void Avdtp_L2cap_Connect_Ind_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs);

void Avdtp_L2cap_Disconnect_Ind_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs);

void Avdtp_L2cap_Data_Ind_Cb(u_int16 cid, struct host_buf * p_buf);

void Avdtp_L2cap_Disconnect_Cfm_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs);
void Avdtp_l2cap_config_ind_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs);

#if A2DP_SRC
void  Avdtp_L2cap_Connection_Cfm_Cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs);

void Avdtp_L2cap_Configure_Cfm_Cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs);

void Avdtp_L2cap_Qos_Violation_Ind_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs);
#ifdef COOLSAND
void Avdtp_Set_Signal_Connection_Ind_callback(void (*CallbackFunc)(void *pArgs));

//void Avdtp_Set_Signal_Connection_Cnf_callback(void (*CallbackFunc)(void *pArgs));

void Avdtp_Set_Signal_Disconnection_Ind_callback(void (*CallbackFunc)(void *pArgs));

//void Avdtp_Set_Signal_Disconnection_Cnf_callback(void (*CallbackFunc)(void *pArgs));
#endif
u_int8 Avdtp_Is_First_Conection(void);
void Avdtp_Set_SRC_First_Conection_Flag(u_int8 flag);

#endif

#endif

