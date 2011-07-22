/**************************************************************************
 * MODULE NAME:    l2_types.h
 * PROJECT CODE:
 * DESCRIPTION:    L2CAP Types
 * AUTHOR:         Gary Fleming
 * DATE:           02-08-1999
 *
 * SOURCE CONTROL: $Id: l2_types.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 **************************************************************************/

#ifndef __PARTHUS_L2CAP_TYPES
#define __PARTHUS_L2CAP_TYPES

#include "host_config.h"


#include "host_buf.h"
#include "l2cap_primitives.h"
#include "bd_addr_list.h"


/****************************************
 * Main States of the L2CAP State Machine
 *****************************************/

typedef enum {
    PRH_L2_CLOSED,
    PRH_L2_OPEN,
    PRH_L2_CONFIG,
    PRH_L2_SUSPENDED_TX,
    PRH_L2_W4_LP_CONNECT_CFM,
    PRH_L2_W4_L2CA_CONNECT_RSP,
    PRH_L2_W4_L2CAP_CONNECT_RSP,
    PRH_L2_W4_L2CA_DISCON_RSP,
    PRH_L2_W4_L2CAP_DISCON_RSP
    } prh_t_l2_channel_states;


/****************************************
 * Config States of the L2CAP State Machine
 *****************************************/

typedef enum {
    PRH_L2_CONFIG_START,
    PRH_L2_REQUEST_RECEIVED,
    PRH_L2_REQUEST_SENT,
    PRH_L2_CONFIG_END
    } prh_t_l2_config_states;

#if COMBINED_HOST==1
/* flags for HCI_SendACL() and L2capACLCallback */
#define prh_hci_ACL_CONTINUE_FRAGMENT       1
#define prh_hci_ACL_START_FRAGMENT          2
#define prh_hci_ACL_ACTIVE_BROADCAST        4
#define prh_hci_ACL_PICONET_BROADCAST       8
#endif

/***************************************************
 *  Structure describing an L2CAP channel
 ***************************************************/

typedef struct {
#if COMBINED_HOST==1
	t_L2_ChannelFlowSpec    inQOS;
    t_L2_ChannelFlowSpec   outQOS;
#else
	t_L2_ChannelFlowSpec*    inQOS;
    t_L2_ChannelFlowSpec*   outQOS;
#endif
	u_int16      flags;			/* Currently not used in the Bluetooth Ver 1.1 specification	*/
    u_int16      local_cid;     /* Identifer for local end of Channel			*/
    u_int16      remote_cid;    /* Identifer for remote end of Channel			*/
    u_int16      psm;           /* Identifer for Protocol using channel			*/
    u_int16      handle;		/* Handle for the ACL link the channel is using */
    u_int16      inMTU;			/* The size of the max allowed incoming MTU		*/
    u_int16      outMTU;		/* The size of the max allowed outgoing MTU		*/
    u_int16      inFlush;		/* The required incoming flush timeout			*/
    u_int16      outFlush;		/* The required outgoing flush timeout			*/
    void*        p_pdu;         /* Pointer to the current PDU on the channel	*/
    t_pTimer     timer_handle;	/* The timer handle								*/
    t_pTimer     config_timer_handle; /* The config timer handle                 */
	t_pMutex chn_lock;
	u_int8		 timer_active;	/* Identifies RTX or ERTX timer is active or not */
								/* 0x00  NO_TIMER								*/
								/* 0x01	 RTX_TIMER								*/
								/* 0x02  ERTX_TIMER								*/

    u_int8       remote_transaction_identifier;
    u_int8       local_transaction_identifier;
	u_int8       type;			/* Type of the channel -- Connection Oriented / Connectionless	*/
    prh_t_l2_config_states config_req_path; /* Current State of the config request path for the channel	*/
    prh_t_l2_config_states config_rsp_path; /* Current State of the config response path for the channel*/
    prh_t_l2_channel_states state;    /* Current State of the Channel         */
	List     *bda_list;
#if COMBINED_HOST==1
	u_int8 flag;
#endif
} prh_t_l2_channel;


/************************************************
 * Structure Define an underlying ACL link
 * already established by the Link Manager 
 ************************************************/

typedef struct {
    t_bdaddr	bd_addr;	/* The Bluetooth device address of peer on the link		*/
#if COMBINED_HOST==1
    t_L2_ChannelFlowSpec    flow;
#else
	t_L2_ChannelFlowSpec*    flow;
#endif
    void (*ping_callback)(t_bdaddr, u_int16, u_int8*, u_int16 );
    void (*info_callback)(t_bdaddr, u_int16, u_int8* ,u_int16 );
    u_int8*		cur_pos;
	u_int8*		echo_data;  /* pointer to echo data which transfered on the link	*/
    struct host_buf* cur_buf;
    t_pTimer    timer_handle; /* The timer handle								*/
    u_int16		handle;		/* The HCI connection handle used to identify the link	*/
	u_int16		echo_len;	/* The length of the echo data to be transfered on the link		*/
	u_int16		info_type;	/* The type of information to be requested/recieved on an L2CAP information req/res */
    u_int16		cur_len;
    u_int16		pending_cid;
    u_int16		length;
	u_int8		timer_active;	/* Identifies if a timer is active or not		*/
    u_int8		op_pending;
    u_int8		identifier;
	u_int8      initiator;
	t_pMutex    acl_lock;
    u_int8      mtu_complete; /* used for error handling in L2CAP sar */
#if COMBINED_HOST==1
	u_int8 flag;
#endif
} prh_t_l2_acl_link;

/*************************************************
 * Structure describing the information required
 * to construct an L2CAP PDU. 
 * The opcode is contained and a pointer to the 
 * L2CAP channel sending the PDU.
 *************************************************/

typedef struct 
{
	t_L2_ChannelFlowSpec* qos;		/* A Quality of service field */
	prh_t_l2_channel* p_channel;	/* A pointer to a channel     */
	u_int8* p_data;					/* A pointer to a data field  */
    u_int16 length;					/* Length of the PDU          */
    u_int16 result;
    u_int16 status;
    u_int16 src_cid;
    u_int16 dest_cid; 
    u_int16 reason;
    u_int16 psm;
    u_int16 info_type;
    u_int16 mtu;
    u_int16 flush;
    u_int16 flags;
    u_int16 config_continue; 
    u_int8  opcode;
    u_int8  identifier;
    u_int8  num_options;
	u_int8  offset;
} prh_t_l2_pdu;


typedef struct {
    u_int16  psm;
    void (*addMemberCfm)(u_int16 cid,  t_bdaddr BDAddress, u_int16 status);
} prh_t_cls_psm;

typedef u_int16 t_psm;

struct l2cap_psm 
{
  struct l2cap_cbs psm_cbs;
  t_psm psm;
  u_int8 state;
};

#endif
