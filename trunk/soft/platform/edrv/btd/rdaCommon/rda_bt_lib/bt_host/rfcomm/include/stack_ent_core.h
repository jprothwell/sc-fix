/***********************************************************************
 *
 * MODULE NAME:    stack_ent_core.h
 * PROJECT CODE:   Host Side Stack
 * DESCRIPTION:    Stack Entity core
 * MAINTAINER:     David Airlie
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: stack_ent_core.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    01.May.2000 -   DA - first revision
 *
 *
 * ISSUES:
 *
 ***********************************************************************/
#ifndef PRH_STACK_ENT_CORE_H
#define PRH_STACK_ENT_CORE_H

#include "stack_ent_api.h"

#if PRH_RFCOMM_RSE_SUPPORT==1

struct prh_rfcomm_se_user
{
  t_bdaddr remote_device_addr;

  u_int32 portnum;

  void *multi_inst;
  
  prh_t_rfcomm_dlci dlci;

  prh_t_rfcomm_flow_state local_flow_state;
  prh_t_rfcomm_flow_state agg_flow_state;

  prh_t_rfcomm_port_params port_params;
  prh_t_rfcomm_dlc_params dlc_params;
  
  struct st_t_RSE_Callbacks cb;
#if PRH_RFCOMM_RSE_DATA_PUMP_ENABLE==1
  u_int8 tx_pump_enabled, rx_pump_enabled;
  t_pMutex rx_stats_lock;
  struct st_t_RSE_Callbacks pre_data_pump_cbs;
  u_int32 rx_traffic;
  u_int32 rx_pkt_count;
  u_int32 rx_time_first;
  u_int32 rx_time_last;
#endif
};


int prh_rfcomm_se_lookup_stack_entity_by_portnum(struct prh_rfcomm_se_user **port, u_int32 portnum);
int prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(struct prh_rfcomm_se_user **port, t_bdaddr bdaddr, u_int8 dlci);
int prh_rfcomm_se_remove_stack_entity_by_portnum(struct prh_rfcomm_se_user **port, u_int32 portnum);
int prh_rfcomm_se_remove_stack_entity_by_addr_and_dlci(struct prh_rfcomm_se_user **port, t_bdaddr bdaddr, u_int8 dlci);
int prh_rfcomm_se_add_stack_entity(struct prh_rfcomm_se_user *self);
void prh_rfcomm_init_se_struct(struct prh_rfcomm_se_user *se_info);
int prh_rfcomm_se_remove_all_stack_entity(void);

#endif

#endif
