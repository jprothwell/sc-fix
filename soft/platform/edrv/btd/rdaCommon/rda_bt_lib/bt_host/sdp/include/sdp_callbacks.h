/***********************************************************************
 *
 * MODULE NAME:    sdp_callbacks.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery 
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_callbacks.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#ifndef SDP_CALLBACKS
#define SDP_CALLBACKS

#include "host_config.h"

#include "sdp_stack_access.h"

#define SDP_PSM 0x0001

void prh_sdp_l2cap_connect_ind_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs);

#if PRH_L2CAP_AUTO_CONFIG==0
void prh_sdp_l2cap_connect_cfm_cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs);
void prh_sdp_l2cap_config_ind_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs);
void prh_sdp_l2cap_config_cfm_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs);
#else
void prh_sdp_l2cap_connect_configured_cfm_cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs);
#endif

void prh_sdp_l2cap_disconnect_ind_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs);

void prh_sdp_l2cap_disconnect_cfm_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs);


void prh_sdp_l2cap_data_ind_cb(u_int16 cid,struct host_buf * p_buf);

void prh_sdp_manager_inquiry_cb(struct st_t_BT_DeviceEntry *deviceInfo, t_api status);
/* void prh_sdp_manager_inquiry_cb(tid_t transactionId, struct st_t_BT_ConnectionInfo *deviceInfo);*/

void prh_sdp_manager_name_cb(t_bdaddr address, t_api status);
/* void prh_sdp_manager_name_cb(tid_t transactionId, error_t status); */

#endif
