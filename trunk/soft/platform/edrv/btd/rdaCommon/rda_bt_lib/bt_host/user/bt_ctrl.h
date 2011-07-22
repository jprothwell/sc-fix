/*
 * MODULE NAME:    bt_ctrl.h
 * DESCRIPTION:    Bluestream control API interface header
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: bt_ctrl.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef BLUETOOTH_HOST_CTRL_DECLARED
#define BLUETOOTH_HOST_CTRL_DECLARED

#ifdef __cplusplus
extern "C" {
#endif

/* Stack configuration via BT_Init is currently unsupported */
struct st_BluestreamStackConfig {
	u_int16 length;
    t_bdaddr deviceAddress; 
    u_int8 countryCode;    
};

APIDECL1 int APIDECL2 BT_Execute(u_int32 maxIterations);
APIDECL1 int APIDECL2 BT_Init(void *stackConfig, void *platformConfig);
APIDECL1 int APIDECL2 BT_Main(void * reservedFlags);
APIDECL1 int APIDECL2 BT_Terminate(u_int32 reservedFlags);
APIDECL1 int APIDECL2 BT_Reset(u_int32 reservedFlags);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BLUETOOTH_HOST_CTRL_DECLARED */
