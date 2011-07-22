/*
 * MODULE NAME:    tcs_cc_upper.c
 * DESCRIPTION:    TCS Call Control Upper Header File
 * AUTHOR:         Thomas Howley
 * DATE:           09 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_upper.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef __TCS_CC_UPPER_H
#define __TCS_CC_UPPER_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "host_types.h"
#include "tcs_cc_types.h"

#define NO_SIGCHANNEL_EXISTS(callInstance) (callInstance->cid == 0)

t_api TCS_Setup(struct st_t_TCS_callInstance *callInstance);
APIDECL1 t_api APIDECL2 TCS_SetupAcknowledge(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_Information(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_CallProceeding(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_Alerting(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
/*APIDECL1 t_api APIDECL2 TCS_Connect(u_int16 callHandle, struct st_t_TCS_callParams *callParams);*/
APIDECL1 t_api APIDECL2 TCS_Connect(struct st_t_TCS_callInstance *callInstance);
APIDECL1 t_api APIDECL2 TCS_ConnectAcknowledge(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_Disconnect(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_Release(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_ReleaseComplete(struct st_t_TCS_callInstance *callInstance);


APIDECL1 t_api APIDECL2 TCS_StartDTMF(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_StartDTMFAcknowledge(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_StartDTMFReject(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_StopDTMF(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_StopDTMFAcknowledge(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
APIDECL1 t_api APIDECL2 TCS_Progress(u_int16 callHandle, struct st_t_TCS_callParams *callParams);


#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/

#endif /* __TCS_CC_UPPER_H  */

