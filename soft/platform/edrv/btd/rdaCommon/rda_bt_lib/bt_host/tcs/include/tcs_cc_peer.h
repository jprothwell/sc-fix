/*
 * MODULE NAME:    tcs_cc_upper.h
 * DESCRIPTION:    TCS Call Control Upper Header File
 * AUTHOR:         Thomas Howley
 * DATE:           09 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_peer.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef __TCS_CC_PEER_H
#define __TCS_CC_PEER_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

struct st_t_TCS_callInstance;
t_api prh_tcs_peer_processEvent(u_int8 tcsMsg, t_bdaddr address, struct st_t_TCS_callInstance *callInstance);



#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/


#endif /* __TCS_CC_PEER_H */
