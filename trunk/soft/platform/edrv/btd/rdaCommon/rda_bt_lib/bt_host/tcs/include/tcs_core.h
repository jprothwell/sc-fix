/***********************************************************************
 *
 * MODULE NAME:    tcs_core.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS L2CAP Channel Handler non-state machine related
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  06 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_core.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
#ifndef PRH_TCS_CORE_H
#define PRH_TCS_CORE_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "tcs_const.h"
#include "host_types.h"
#include "tcs_cc_types.h"



/* Enumerated Type to store L2CAP Channel State */
typedef enum {
    PRH_TCS_L2CAP_IDLE,
    PRH_TCS_L2CAP_W4CONN,
    PRH_TCS_L2CAP_OPEN,
    PRH_TCS_L2CAP_W4RL
} PRH_TCS_L2CAP_STATE;

struct prh_st_tcs_call
{
  u_int16 cid;
};

struct prh_st_tcs_acl_bearer 
{
  u_int16 cid;
};

#define PRH_TCS_L2CAP_CHAN_CALL 1
#define PRH_TCS_L2CAP_CHAN_BEARER 2

struct prh_st_tcs_l2conn
{
  t_bdaddr bd_addr;
  u_int16 psm;
  u_int16 cid;
  u_int8 clk_offset_pending;
  int in_mtu, out_mtu;
  PRH_TCS_L2CAP_STATE l2chan_state;

  u_int8 connector;
  t_api (*conn_callback)(t_api status, struct prh_st_tcs_l2conn *);
  u_int8 type;
  union {
    struct st_t_TCS_callInstance *call;
    struct prh_st_tcs_acl_bearer *bearer;
  } un;
};


#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/


#endif
