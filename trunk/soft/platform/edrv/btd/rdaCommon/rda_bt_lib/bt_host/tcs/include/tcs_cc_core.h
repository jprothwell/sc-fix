/*
 * MODULE NAME:    tcs_cc_core.h
 * DESCRIPTION:    TCS Call Control Core Header File
 * AUTHOR:         Thomas Howley
 * DATE:           09 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_core.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */


#ifndef __TCS_CC_CORE_H
#define __TCS_CC_CORE_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "tcs_cc_types.h"

#if (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY)
extern u_int8 tcsMode; 

#define PRH_TCS_MODE_TERMINAL 1
#define PRH_TCS_MODE_GATEWAY  2

#endif /* (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY) */


#if (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY)
extern struct st_t_TCS_TLorGWInstance TCSInstance;
#elif PRH_TCS_TERMINAL
extern struct st_t_TCS_TLInstance TCSInstance;
#elif PRH_TCS_GATEWAY
extern struct st_t_TCS_GWInstance TCSInstance;
#endif 

/* For functions common to both TL & GW but with different definitions */
/* If TERMINAL and GATEWAY are both part of the build, "tcsMode" is used to choose definition at run-time */
/* "tcsMode" is set by the TCS_GWRegister or TCS_TLRegister functions */
#if (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY)
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLorGWgetCallInstanceByHandle(u_int16 callHandle);
#define prh_tcs_cc_core_getCallInstanceByHandle prh_tcs_cc_core_TLorGWgetCallInstanceByHandle
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLorGWgetCallInstanceByCID(u_int16 cid);
#define prh_tcs_cc_core_getCallInstanceByCID prh_tcs_cc_core_TLorGWgetCallInstanceByCID
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLorGWgetCallInstanceByBearerHandle(u_int16 bearerHandle);
#define prh_tcs_cc_core_getCallInstanceByBearerHandle prh_tcs_cc_core_TLorGWgetCallInstanceByBearerHandle
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLorGWgetCallInstanceByBearerRef(u_int16 *bearerRef);
#define prh_tcs_cc_core_getCallInstanceByBearerRef prh_tcs_cc_core_TLorGWgetCallInstanceByBearerRef
void prh_tcs_cc_core_TLorGWfreeCallInstance(struct st_t_TCS_callInstance *callInstance);
#define prh_tcs_cc_core_freeCallInstance prh_tcs_cc_core_TLorGWfreeCallInstance
t_bdaddr *prh_tcs_cc_core_TLorGWgetBdAddressByCallInstance(struct st_t_TCS_callInstance *callInstance);
#define prh_tcs_cc_core_getBdAddressByCallInstance prh_tcs_cc_core_TLorGWgetBdAddressByCallInstance
u_int8 prh_tcs_cc_core_TLorGW_L2ReleaseRequired(struct st_t_TCS_callInstance *callInstance);
#define prh_tcs_cc_core_L2ReleaseRequired prh_tcs_cc_core_TLorGW_L2ReleaseRequired
u_int8 prh_tcs_cc_core_freeTLPeerByBdAddress(t_bdaddr address);
void prh_tcs_cc_core_freeUnassignedBcastCallInstance(struct st_t_TCS_callInstance *callInstance);

#elif PRH_TCS_TERMINAL
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLgetCallInstanceByHandle(u_int16 callHandle);
#define prh_tcs_cc_core_getCallInstanceByHandle prh_tcs_cc_core_TLgetCallInstanceByHandle
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLgetCallInstanceByCID(u_int16 cid);
#define prh_tcs_cc_core_getCallInstanceByCID prh_tcs_cc_core_TLgetCallInstanceByCID
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLgetCallInstanceByBearerHandle(u_int16 bearerHandle);
#define prh_tcs_cc_core_getCallInstanceByBearerHandle prh_tcs_cc_core_TLgetCallInstanceByBearerHandle
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLgetCallInstanceByBearerRef(u_int16 *bearerRef);
#define prh_tcs_cc_core_getCallInstanceByBearerRef prh_tcs_cc_core_TLgetCallInstanceByBearerRef
void prh_tcs_cc_core_TLfreeCallInstance(struct st_t_TCS_callInstance *callInstance);
#define prh_tcs_cc_core_freeCallInstance prh_tcs_cc_core_TLfreeCallInstance
t_bdaddr *prh_tcs_cc_core_TLgetBdAddressByCallInstance(struct st_t_TCS_callInstance *callInstance);
#define prh_tcs_cc_core_getBdAddressByCallInstance prh_tcs_cc_core_TLgetBdAddressByCallInstance
u_int8 prh_tcs_cc_core_TL_L2ReleaseRequired(struct st_t_TCS_callInstance *callInstance);
#define prh_tcs_cc_core_L2ReleaseRequired prh_tcs_cc_core_TL_L2ReleaseRequired
void prh_tcs_cc_core_freeUnassignedBcastCallInstance(struct st_t_TCS_callInstance *callInstance);

#elif PRH_TCS_GATEWAY
struct st_t_TCS_callInstance *prh_tcs_cc_core_GWgetCallInstanceByHandle(u_int16 callHandle);
#define prh_tcs_cc_core_getCallInstanceByHandle prh_tcs_cc_core_GWgetCallInstanceByHandle
struct st_t_TCS_callInstance *prh_tcs_cc_core_GWgetCallInstanceByCID(u_int16 cid);
#define prh_tcs_cc_core_getCallInstanceByCID prh_tcs_cc_core_GWgetCallInstanceByCID
struct st_t_TCS_callInstance *prh_tcs_cc_core_GWgetCallInstanceByBearerHandle(u_int16 bearerHandle);
#define prh_tcs_cc_core_getCallInstanceByBearerHandle prh_tcs_cc_core_GWgetCallInstanceByBearerHandle
struct st_t_TCS_callInstance *prh_tcs_cc_core_GWgetCallInstanceByBearerRef(u_int16 *bearerRef);
#define prh_tcs_cc_core_getCallInstanceByBearerRef prh_tcs_cc_core_GWgetCallInstanceByBearerRef
void prh_tcs_cc_core_GWfreeCallInstance(struct st_t_TCS_callInstance *callInstance);
#define prh_tcs_cc_core_freeCallInstance prh_tcs_cc_core_GWfreeCallInstance
t_bdaddr *prh_tcs_cc_core_GWgetBdAddressByCallInstance(struct st_t_TCS_callInstance *callInstance);
#define prh_tcs_cc_core_getBdAddressByCallInstance prh_tcs_cc_core_GWgetBdAddressByCallInstance
u_int8 prh_tcs_cc_core_GW_L2ReleaseRequired(struct st_t_TCS_callInstance *callInstance);
#define prh_tcs_cc_core_L2ReleaseRequired prh_tcs_cc_core_GW_L2ReleaseRequired
u_int8 prh_tcs_cc_core_freeTLPeerByBdAddress(t_bdaddr address);

#endif 


/* Function definitions unique to TERMINAL */
#if PRH_TCS_TERMINAL 
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLgetNewCallInstance(u_int16 cid, u_int8 callType);

#endif /* PRH_TCS_TERMINAL */

/* Function definitions unique to GATEWAY */
#if PRH_TCS_GATEWAY 
struct st_t_TCS_callInstance *prh_tcs_cc_core_GWgetNewCallInstance(struct st_t_TCS_TLPeer *TLPeer, u_int16 cid, u_int16 callType);
struct st_t_TCS_TLPeer *prh_tcs_cc_core_getNewTLPeer(t_bdaddr address, /*u_int8 initState,*/ u_int16 cid);
struct st_t_TCS_TLPeer *prh_tcs_cc_core_getTLPeerByCallInstance(struct st_t_TCS_callInstance *callInstance);
struct st_t_TCS_TLPeer *prh_tcs_cc_core_getTLPeerByBdAddress(t_bdaddr address);
t_api prh_tcs_cc_core_broadcastIncomingSETUP(struct st_t_TCS_callParams *callParams);
void prh_tcs_cc_core_GWfreeCallHandle(u_int16 callHandle);
struct st_t_TCS_callInstance *prh_tcs_cc_core_GWaddBCastCallInstance(struct st_t_TCS_TLPeer *TLPeer, u_int16 cid);
t_api prh_tcs_cc_core_GW_releaseNonSelectedBCastCalls(void);
struct st_t_TCS_TLPeer *prh_tcs_cc_core_GWgetNumConnectedTLPeers(u_int8 *numTLPeers);
#endif /* PRH_TCS_GATEWAY */

/* Common Definitions for TL and GW */
void prh_tcs_cc_core_initCallInstance(struct st_t_TCS_callInstance *callInstance);
void prh_tcs_cc_core_initCallParams(struct st_t_TCS_callParams *callParams);
void prh_tcs_cc_core_freeCallParamMembers(struct st_t_TCS_callParams *callParams);
void prh_tcs_cc_core_freeCallParams(struct st_t_TCS_callParams *callParams);
t_api prh_tcs_cc_core_storeCallParams(struct st_t_TCS_callParams **dest, struct st_t_TCS_callParams *src);

#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/

#endif /* __TCS_CC_CORE_H */
