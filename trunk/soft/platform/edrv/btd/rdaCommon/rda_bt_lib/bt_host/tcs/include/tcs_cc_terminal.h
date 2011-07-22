/*
 * MODULE NAME:    tcs_cc_terminal.h
 * DESCRIPTION:    TCS Call Control Terminal Header File
 * AUTHOR:         Thomas Howley
 * DATE:           03 April 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_terminal.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */



#ifndef __TCS_CC_TERMINAL_H
#define __TCS_CC_TERMINAL_H

#include "host_config.h"

#if PRH_TCS_TERMINAL

#include "host_types.h"
#include "tcs_cc_types.h"
#include "tcs_cc_upper.h"

#define NO_SIGCHANNEL_EXISTS(callInstance) (callInstance->cid == 0)

extern void (*registerCompleteCB)(t_api status);
extern void (*connectToTLCompleteCB)(t_bdaddr, t_api status);
extern void (*connectToGWCompleteCB)(t_bdaddr, t_api status);

APIDECL1 t_api APIDECL2 TCS_TLRegister(u_int8 flag, struct st_t_TCS_callbacks *callbacks, t_bdaddr *GWBDAddress, u_int8 connectToGW, void (*callback)(t_api status));

APIDECL1 t_api APIDECL2 TCS_TLConnectToTL(t_bdaddr TLbdAddress, void (*callbackFunc)(t_bdaddr address, t_api status));
APIDECL1 t_api APIDECL2	TCS_TLConnectToGW(t_bdaddr GWbdAddress, void (*callbackFunc)(t_bdaddr address, t_api status));
APIDECL1 t_api APIDECL2 TCS_TLIntercomSetup(u_int16 *callHandle, t_bdaddr TLbdAddress, struct st_t_TCS_callParams *callParams); 
APIDECL1 t_api APIDECL2 TCS_TLCordlessSetup(u_int16 *callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_TLRegisterRecall(u_int16 callHandle);
APIDECL1 t_api APIDECL2 TCS_TLConnect(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_TLReleaseComplete(u_int16 callHandle, struct st_t_TCS_callParams *callParams);

#define TCS_TLSetupAcknowledge      TCS_SetupAcknowledge
#define TCS_TLAlerting              TCS_Alerting
#define TCS_TLInformation			TCS_Information
#define TCS_TLCallProceeding		TCS_CallProceeding
#define TCS_TLConnectAcknowledge	TCS_ConnectAcknowledge
#define TCS_TLDisconnect			TCS_Disconnect
#define TCS_TLRelease				TCS_Release
#define TCS_TLStartDTMF				TCS_StartDTMF
#define TCS_TLStartDTMFAcknowledge	TCS_StartDTMFAcknowledge
#define TCS_TLStartDTMFReject		TCS_StartDTMFReject
#define	TCS_TLStopDTMF				TCS_StopDTMF
#define TCS_TLStopDTMFAcknowledge	TCS_StopDTMFAcknowledge
#define TCS_TLProgress				TCS_Progress

APIDECL1 t_api APIDECL2 TCS_TLDisconnectFromGWorTL(void);
#define TCS_TLDisconnectFromTL TCS_TLDisconnectFromGWorTL
#define TCS_TLDisconnectFromGW TCS_TLDisconnectFromGWorTL


#endif /* __TCS_CC_TERMINAL_H */

#endif /* PRH_TCS_TERMINAL */

