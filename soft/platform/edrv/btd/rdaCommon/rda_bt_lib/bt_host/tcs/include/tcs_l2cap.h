/***********************************************************************
 *
 * MODULE NAME:    tcs_l2cap.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS L2CAP Channel Handler Header File
 * MAINTAINER:     Thomas Howley
 * CREATION DATE:  22 March 2000
 *
 * SOURCE CONTROL: $Id: tcs_l2cap.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

#ifndef __TCS_L2CAP_H
#define __TCS_L2CAP_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

int prh_tcs_init(int flags);
int prh_tcs_register_connectionCallbacks( void (*connectIndCallback)(struct prh_st_tcs_l2conn * l2conn), void (*disconnectIndCallback)(struct prh_st_tcs_l2conn * l2conn));
int prh_tcs_add_l2cap_inst(struct prh_st_tcs_l2conn *new_ent);
int prh_tcs_connect_device_control(t_bdaddr bd_addr, u_int16 psm, struct st_t_TCS_callInstance *call, t_api(*callback_func_ptr)(t_api status, struct prh_st_tcs_l2conn *l2_conn));
int prh_tcs_lookup_l2cap_inst_by_cid(u_int16 cid, struct prh_st_tcs_l2conn **l2cap_inst_ptr);
int prh_tcs_lookup_l2cap_inst_by_bd_addr(t_bdaddr bd_addr, struct prh_st_tcs_l2conn **l2cap_inst_ptr);
int prh_tcs_lookup_free_l2cap_inst(t_bdaddr bd_addr, u_int16 psm, struct prh_st_tcs_l2conn **l2cap_inst_ptr);

#if PRH_TCS_GATEWAY
t_bdaddr *prh_tcs_l2cap_findConnectedTL(void);
#endif


#endif/* (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/

#endif /* __TCS_L2CAP_H */
