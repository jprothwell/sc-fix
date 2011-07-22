/*
 * MODULE NAME:    host_ctrl.h
 * DESCRIPTION:    Host system scheduler interface header
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: host_ctrl.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies.
 *     All rights reserved.
 *
 */



#ifndef PRH_HOST_CTRL_H
#define PRH_HOST_CTRL_H

#include "host_types.h"

#if COMBINED_HOST==1
void  BThost_Initialise(void);
#endif
APIDECL1 t_api APIDECL2 BT_Execute(u_int32 maxIterations);
APIDECL1 t_api APIDECL2 BT_Init(void *stackConfig, void *platformConfig);
APIDECL1 t_api APIDECL2 BT_Main(void * reservedFlags);
APIDECL1 t_api APIDECL2 BT_Terminate(u_int32 reservedFlags);


APIDECL1 t_api APIDECL2 BT_Reset(u_int32 reservedFlags);
/* 
    These flags will be passed to the per-layer startup/shutdown functions. 
*/
#define PRH_HOST_STARTUP            0x01
#define PRH_HOST_SHUTDOWN           0x02
#define PRH_HOST_RESET              0x04

/* These flags may be passed to BT_Reset */
#define PRH_HOST_NOHCRESET          0x01
#define PRH_HOST_NOLINKDISCONNECT   0x02

void prh_system_scheduler_execute(void);


/* this structure is currently ignored - currently combined stack only */
struct st_BluestreamStackConfig {
	u_int16 length;
    t_bdaddr deviceAddress; 
    u_int8 countryCode;    
};

/* layer control function prototypes */
#if HCI_PRESENT 
    t_api prh_hci_layer_ctrl(u_int8 layerFlags);
#endif

#if MANAGER_PRESENT
    t_api prh_manager_layer_ctrl(u_int8 layerFlags);
#endif

#if L2CAP_PRESENT
    t_api prh_l2cap_layer_ctrl(u_int8 layerFlags);
#endif

#if SDP_CLIENT
    t_api prh_sdp_cli_layer_ctrl(u_int8 layerFlags);
#endif

#if SDP_SERVER
    t_api prh_sdp_srv_layer_ctrl(u_int8 layerFlags);
#endif

#if RFCOMM_PRESENT
    t_api prh_rfcomm_layer_ctrl(u_int8 layerFlags);
#endif

#if OBEX_PRESENT
    t_api prh_obex_layer_ctrl(u_int8 layerFlags);
#endif


/* macros to allow reading the value of preprocessor definitions */
#ifndef BT_STRINGIZE
#define BT_STRINGIZE(x) #x
#endif
#ifndef BT_VALUEOF
#define BT_VALUEOF(x) BT_STRINGIZE(x)
#endif

#endif /* PRH_HOST_CTRL_H */
