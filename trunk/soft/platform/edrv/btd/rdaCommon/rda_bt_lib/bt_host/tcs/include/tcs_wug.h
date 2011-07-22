/***********************************************************************
 *
 * MODULE NAME:    tcs_wug.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS L2CAP Channel Handler non-state machine related
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  06 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_wug.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
#ifndef PRH_TCS_WUG_H
#define PRH_TCS_WUG_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

struct prh_st_tcs_wug_listen_info 
{
  struct prh_st_tcs_wug_member *requester;
  struct prh_st_tcs_wug_member *terminator;  
  t_pTimer prh_tcs_t405_handle;
};

extern struct prh_host_gen_ll *prh_tcs_wug_listen_list;
extern t_pMutex prh_tcs_wug_listen_lock;

struct prh_st_tcs_wug_member_link_key
{
  t_bdaddr peer;
  u_int8 link_key[16];
};

/* NOTE: ordering must be the same as in t_TCS_wugMember in tcs_cc_types.h */
struct prh_st_tcs_wug_member 
{
  t_bdaddr addr;
  u_int8 wug_num[2];
  
  
  union {
    struct prh_host_gen_ll *member_link_keys; /* used only on the gateway */
    u_int8 link_key[16];                      /* used only in the terminal */
  } lk;
  u_int16 clock_offset;
  u_int16 needs_update; /* used by MASTER to note whether a node has been updated */
  struct prh_st_tcs_l2conn *l2_conn;
  t_pTimer t403_handle;
};

extern struct prh_host_gen_ll *prh_tcs_wug_list;
extern t_pMutex prh_tcs_wug_lock;
extern unsigned int prh_tcs_wug_num_members;

#define PRH_TCS_WUG_MASTER 1
#define PRH_TCS_WUG_MEMBER 2

/* WUG Device ROLE - WUG MASTER or WUG MEMBER */
extern u_int8 prh_tcs_wug_role;

#define PRH_TCS_WUG_REFUSE_CONFIG 0
#define PRH_TCS_WUG_ACCEPT_CONFIG 1

/* Should a terminal accept the config info */
extern u_int8 prh_tcs_wug_config_accept_mode;


#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/

#endif
