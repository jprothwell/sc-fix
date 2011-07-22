/*
 * MODULE NAME:    tcs_cc_callbacks.h
 * DESCRIPTION:    TCS Call Control Callbacks Header File
 * AUTHOR:         Thomas Howley
 * DATE:           13 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_callbacks.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef __TCS_CC_CALLBACKS_H
#define __TCS_CC_CALLBACKS_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

struct prh_st_tcs_l2conn;



/* For functions common to both TL & GW but with different definitions */
/* If TERMINAL and GATEWAY are both part of the build, "tcsMode" is used to choose definition at run-time */
/* "tcsMode" is set by the TCS_GWRegister or TCS_TLRegister functions */
#if (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY)
t_api prh_tcs_cc_callback_TLorGW_L2Data(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);
#define prh_tcs_cc_callback_L2Data prh_tcs_cc_callback_TLorGW_L2Data

#elif PRH_TCS_TERMINAL
t_api prh_tcs_cc_callback_TL_L2Data(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);
#define prh_tcs_cc_callback_L2Data prh_tcs_cc_callback_TL_L2Data

#elif PRH_TCS_GATEWAY
t_api prh_tcs_cc_callback_GW_L2Data(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);
#define prh_tcs_cc_callback_L2Data prh_tcs_cc_callback_GW_L2Data

#endif


t_api prh_tcs_cc_callback_L2ConnComplete(t_api status, struct prh_st_tcs_l2conn *l2_conn);

void prh_tcs_cc_callback_timeout(void *callbackParam);
void prh_tcs_cc_callback_bcast_setup_repeat_timeout(void *callbackParam);
void prh_tcs_cc_callback_bcast_rx_setup_tx_no_reply_timeout(void *callbackParam);


#if PRH_TCS_TERMINAL
void prh_tcs_cc_core_TLconnectIndCallback(struct prh_st_tcs_l2conn * l2conn);
void prh_tcs_cc_core_TLdisconnectIndCallback(struct prh_st_tcs_l2conn *l2conn);
#endif /* PRH_TCS_TERMINAL */

#if PRH_TCS_GATEWAY
void prh_tcs_cc_callback_broadcastSETUP_timeout(void *callbackParam);
void prh_tcs_cc_callback_unparkAllACLs(t_api status);
void prh_tcs_cc_core_GWconnectIndCallback(struct prh_st_tcs_l2conn *l2conn);
void prh_tcs_cc_core_GWdisconnectIndCallback(struct prh_st_tcs_l2conn *l2conn);
#endif /* PRH_TCS_GATEWAY */

#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/

#endif /* __TCS_CC_CALLBACKS_H */

