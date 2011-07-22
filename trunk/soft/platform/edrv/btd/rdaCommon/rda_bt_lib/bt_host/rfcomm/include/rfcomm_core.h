/***********************************************************************
 *
 * MODULE NAME:    rfcomm_core.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM Core Structs type + defines
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  15 March 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_core.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#ifndef PRH_RFCOMM_CORE_H
#define PRH_RFCOMM_CORE_H

#include "host_config.h"
#include "host_ll.h"

/* Enumerated Type to store L2CAP Channel State */
typedef enum {
    PRH_RFCOMM_L2CAP_IDLE,
    PRH_RFCOMM_L2CAP_W4CONN,
    PRH_RFCOMM_L2CAP_CONN,
    PRH_RFCOMM_L2CAP_W4RL
} PRH_RFCOMM_L2CAP_STATE;

/* Enumerated Type to store DLC State */
typedef enum {
    PRH_RFCOMM_DLC_SENDPN,
    PRH_RFCOMM_DLC_CLOSED,
    PRH_RFCOMM_DLC_W4UA,
    PRH_RFCOMM_DLC_W4PE,
    PRH_RFCOMM_DLC_CONN,
    PRH_RFCOMM_DLC_W4RL
} PRH_RFCOMM_DLC_STATE;

/* Enumerated type containing all allowed events in the system */
typedef enum {
    PRH_RFCOMM_L2CAP_CONNECT_IND,
    PRH_RFCOMM_L2CAP_CONNECT_CFM,
    PRH_RFCOMM_L2CAP_DISC_REQ,
    PRH_RFCOMM_L2CAP_DISC_CFM,
    PRH_RFCOMM_L2CAP_DISC_RESP,
    PRH_RFCOMM_L2CAP_DISC_IND,
    PRH_RFCOMM_L2CAP_DATA_IND,
    PRH_RFCOMM_L2CAP_CONFIG_IND,
    PRH_RFCOMM_L2CAP_CONFIG_CFM,
    PRH_RFCOMM_L2CAP_CONNECT_CONFIG_CFM,
    PRH_RFCOMM_RFSTART_REQ,
    PRH_RFCOMM_RFSTART_RESP,
    PRH_RFCOMM_RFCLOSE_REQ,
    PRH_RFCOMM_DLC_ESTAB_REQ,
    PRH_RFCOMM_DLC_ESTAB_CONF,
    PRH_RFCOMM_DLC_ESTAB_RESP,
    PRH_RFCOMM_DLC_RELEASE_REQ,
    PRH_RFCOMM_DLC_RELEASE_RESP,
    PRH_RFCOMM_DLC_FLOW_REQ,
    PRH_RFCOMM_DATA_REQ,
    PRH_RFCOMM_DLC_DATA_REQ,
    PRH_RFCOMM_DLC_SABM,
    PRH_RFCOMM_DLC_UA,
    PRH_RFCOMM_DLC_DISC,
    PRH_RFCOMM_DLC_UIH,
    PRH_RFCOMM_DLC_DM,
    PRH_RFCOMM_DLC_PN,
    PRH_RFCOMM_TIMEOUT,
    PRH_RFCOMM_DLC_ESTAB_SEC,
    PRH_RFCOMM_DLC_CLOSE_NOW /* used when L2CAP channel is disconnected */
} PRH_RFCOMM_EVENT;

/* Credit state, nothing yet, switched off and switched on */
typedef enum {
  CREDIT_NOTSELECTED,
  CREDIT_REQUESTED,
  CREDIT_OFF,
  CREDIT_ON
} prh_rfcomm_credit_state;

/* Event information structure - passed between upper edge + state machines */
struct prh_rfcomm_event_info 
{
  t_bdaddr bd_addr;
  prh_t_rfcomm_cid cid;
  prh_t_rfcomm_dlci dlci;
  prh_t_rfcomm_flow_state flow;
  struct host_buf *buffer;
  prh_t_rfcomm_accept accept;
  union {
    struct prh_rfcomm_multi_cb multi;
    struct prh_rfcomm_dlc_cb dlc;
  } cb;
  u_int8 cb_valid;
  void *priv;
};

/* 
 * RFCOMM Registered Service
 *
 */
struct prh_rfcomm_server_inst {
  t_pMutex lock;
  u_int8 server_chan;
  struct prh_rfcomm_dlc_cb dlc_cbs;
};

/* 
 * RFCOMM Core instance structure , stores list of multiplexers and has a 
 * callback to indicate a peer wishes to connect a multiplexer 
 * One per system.
 */
struct prh_rfcomm_inst {
  t_pMutex lock;
  
  struct prh_host_gen_ll *head;
  struct prh_host_gen_ll *server_list;

  int (*rfstart_ind_cb)(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, void *rfcomm_inst);
  u_int32 channel_allocation_state; /* one bit per server channel - is the channel allocated */
  u_int32 channel_registered_state; /* one bit per server channel - has the channel been registered on */
  u_int8 initial_credit_state;
};

/* Only one instance of above per system */
extern struct prh_rfcomm_inst *prh_rfcomm_core;
extern t_pMutex prh_rfcomm_core_lock;
/*
 * RFCOMM L2CAP Channel instance structure
 * One per multiplexer
 */
struct prh_rfcomm_l2cap_inst {
  /* indexed by bd_addr */
  t_bdaddr bd_addr;

  /* Back pointer to manager */
  struct prh_rfcomm_inst *rfcomm_ptr;

  /* pointer to first dlc instance associated with this L2CAP instance */
  struct prh_host_gen_ll *dlc_head;

  prh_t_rfcomm_cid cid;

  u_int8 connector;
  u_int8 l2cap_state;
  u_int8 dlc_count;
  /* Am I the Multiplexer Initiator? - TS 5.2.1.2 */
  u_int8 initiating;
  struct prh_rfcomm_multi_cb cb;

  prh_t_rfcomm_start_sys_params sys_params;
  u_int16 in_mtu;
  u_int16 out_mtu;
  prh_rfcomm_credit_state credit_state;
  t_pMutex lock;
  
};

/*
 * DLC Instance Structure
 * One per opened DLC or DLC in opening
 */
struct prh_rfcomm_dlc_inst {
  prh_t_rfcomm_dlci dlci;
  
  int dlc_state;

  prh_t_rfcomm_flow_state flow;

  struct prh_rfcomm_l2cap_inst *rfcomm_l2cap_ptr;

  struct prh_rfcomm_dlc_cb cb;

  t_pTimer pfbit_timer;
  /* count of the number of response expected on the control channel */
  u_int32 resp_count;
  prh_t_rfcomm_dlc_params dlc_params;

  prh_t_rfcomm_port_params port_params;
  struct host_buf *flow_head;
  u_int8 port_params_valid;
  /* Local credits are our buffers locally - i.e. what the peer can send */
  /* Peer credits are the remote buffers - i.e. what we can send peer */
  u_int8 max_local_credits;
  u_int8 max_peer_credits;

  u_int8 cur_local_credits;
  u_int8 cur_peer_credits;  
  t_pMutex credit_lock;
  t_pMutex lock;
  u_int8 auto_credit_issue;

#if PRH_RFCOMM_SM2==1
  u_int8 outgoing_sec_state;
  struct prh_rfcomm_event_info outgoing_sec_info;
  prh_t_rfcomm_dlc_params outgoing_sec_params;
#endif

};

/* System Parameters */
/* Timeout for frames sent with P/F-bit set, SABM or DISC */
#define TIMER_ACK_T1 600
#define TIMER_wait_dis_dlc0_T2 5
/* Timeout for commands sent in UIH frames on DLCI 0 */
#define TIMER_RESP_T2 600

#define PRH_RFCOMM_PSM 3

#define PRH_RFCOMM_DEFAULT_MTU 672
#define PRH_RFCOMM_DLCI0 0

#define PRH_RFCOMM_CR_UNSET 0
#define PRH_RFCOMM_CR_SET 1

#define PRH_RFCOMM_PF_UNSET 0
#define PRH_RFCOMM_PF_SET 1

#define PRH_RFCOMM_MAX_SCHAN 30
#include "rfcomm_buffer.h"

#endif
