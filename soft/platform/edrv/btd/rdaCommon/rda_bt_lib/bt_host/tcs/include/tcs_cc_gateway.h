/*
 * MODULE NAME:    tcs_cc_gateway.h
 * DESCRIPTION:    TCS Call Control Gateway Header File
 * AUTHOR:         Thomas Howley
 * DATE:           03 April 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_gateway.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */


#ifndef __TCS_CC_GATEWAY_H
#define __TCS_CC_GATEWAY_H

#include "host_config.h"

#if PRH_TCS_GATEWAY

#include "host_types.h"
#include "tcs_cc_types.h"
#include "tcs_cc_upper.h"


APIDECL1 t_api APIDECL2 TCS_GWRegister(u_int8 flag, struct st_t_TCS_callbacks *callbacks);
APIDECL1 t_api APIDECL2 TCS_GWCordlessSetup(u_int16 *callHandle, t_bdaddr *TLbdAddress, struct st_t_TCS_callParams *callParams);
#if PRH_TCS_ENABLEGATEWAYINTERCOMCALLS
APIDECL1 t_api APIDECL2 TCS_GWIntercomSetup(u_int16 *callHandle, t_bdaddr TLbdAddress, struct st_t_TCS_callParams *callParams);
#endif
APIDECL1 t_api APIDECL2 TCS_GWConnect(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
#define TCS_GWSetupAcknowledge      TCS_SetupAcknowledge
#define TCS_GWAlerting              TCS_Alerting
#define TCS_GWInformation			TCS_Information
#define TCS_GWCallProceeding		TCS_CallProceeding
#define TCS_GWConnectAcknowledge	TCS_ConnectAcknowledge
#define TCS_GWDisconnect			TCS_Disconnect
#define TCS_GWRelease				TCS_Release
#define TCS_GWStartDTMF				TCS_StartDTMF
#define TCS_GWStartDTMFAcknowledge	TCS_StartDTMFAcknowledge
#define TCS_GWStartDTMFReject		TCS_StartDTMFReject
#define	TCS_GWStopDTMF				TCS_StopDTMF
#define TCS_GWStopDTMFAcknowledge	TCS_StopDTMFAcknowledge
#define TCS_GWProgress				TCS_Progress
APIDECL1 t_api APIDECL2 TCS_GWDisconnectFromTL(t_bdaddr TLAddress);
APIDECL1 t_api APIDECL2 TCS_GWReleaseComplete(u_int16 callHandle, struct st_t_TCS_callParams *callParams);


#endif /* __TCS_CC_GATEWAY_H */


#endif /* PRH_TCS_GATEWAY */
