/******************************************************************
 * MODULE NAME:     l2_connect_sm.h
 * PROJECT CODE:
 * DESCRIPTION:     L2CAP Connection State Machine 
 *    
 * AUTHOR:          Gary Fleming
 * DATE:            21 July 1999 
 *
 * SOURCE CONTROL: $Id: l2_connect_sm.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies, Ltd.
 *     All rights reserved.
 *    
 ******************************************************************/

#ifndef _PARTHUS_L2CAP_CH_
#define _PARTHUS_L2CAP_CH_

#include "host_config.h"

#include "host_types.h"
#include "host_buf.h"

#include "l2_types.h"
#include "l2_const.h"
#include "l2cap_primitives.h"
#include "l2_layer_config.h"


/************************************************************
 * Functions for Test Interface, which determines the state of
 * an L2CAP Channel.
 ************************************************************/

t_api prh_l2_connect_event_handler(u_int8 *pdu);


/*****************************************
 * Handle the L2CAP channel state machine
 *****************************************/

t_api prh_l2_connect_incoming_pdu(u_int16 handle, u_int8 opcode,prh_t_l2_channel* p_l2ca_channel, prh_t_l2_pdu *p_in_pdu);

/*****************************************
 * Interface to the lower layer protocol
 *****************************************/

void prh_l2_connect_lp_connect_cfm(tid_t Transaction_Id,t_api result);
void prh_l2_connect_lp_connect_ind(t_bdaddr* p_bd_addr, u_int16 handle);

/*****************************************
 * Initialisation 
 *****************************************/

void prh_l2_connect_sm_initialise(void);

/*****************************************
 * Security Access Request callbacks
 *****************************************/

void  L2_IncomingAccessRequest_Callback(u_int32 transactionId, t_api status);
void  L2_OutgoingAccessRequest_Callback(u_int32 transactionId, t_api status);

#endif
