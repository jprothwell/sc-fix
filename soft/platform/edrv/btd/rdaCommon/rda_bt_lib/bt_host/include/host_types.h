/***********************************************************************
 *
 * MODULE NAME:    host_types.h
 * DESCRIPTION:    Global types for Host Side Stack
 * MAINTAINER:     Dave Airlie
 *
 * SOURCE CONTROL: $Id: host_types.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies.
 *     All rights reserved.
 *
 ***********************************************************************/

#ifndef PRH_HOST_TYPES_H
#define PRH_HOST_TYPES_H

#include "host_config.h"

/* each platform should provide one of these for the build systen to find.....
	this should contain the basic X_intXX types */
#if COMBINED_HOST==1
#include "sys_hal_types.h"
#else
#include "platform_types.h"
#endif

/* API function return code */
typedef int t_api;	

typedef u_int16 t_channelId;
#if COMBINED_HOST==1
typedef u_int8 t_pTimer;
typedef u_int8 t_pMutex;
#endif
	
/* data transport buffer */
typedef struct st_t_dataBuf {
	u_int8 *buf;
	u_int32 dataLen;
#if COMBINED_HOST==0
	u_int32 bufLen;
#endif
} t_DataBuf;

/*
	NB:  The bytes in the bdaddr structure are ordered with the LSB at bytes[0].
	e.g. u_int8 bytes[] = { 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA } represents the
	device address 0xAABBCCDDEEFF.  This is the order which the HCI PDU parsing uses.
	Similar ordering is used for link keys.
*/
typedef struct {
	u_int8 bytes[6];
} t_bdaddr;

//zhu jianguo added it. 2009.1.7
typedef struct
{
	u_int32 lap; 
	u_int8  uap; 
	u_int16 nap; 
} bt_device_addr_struct;

#define SIZE_OF_BDADDR 6

#ifndef PRH_COMBINED_STACK_COMMON_TYPES
#define PRH_COMBINED_STACK_COMMON_TYPES 1
typedef u_int32 t_classDevice;
#endif

#ifndef BD_ADDR_CMP /* An optimised version may be defined in platform_operations.h */
#    define BD_ADDR_CMP(x,y) !pMemcmp(((x).bytes), ((y).bytes), SIZE_OF_BDADDR)
#endif


#ifndef BD_ADDR_TO_UINT8_CPY
#define  BD_ADDR_TO_UINT8_CPY(x,y) pMemcpy((x),((y).bytes),SIZE_OF_BDADDR)
#endif
/* 
	This is used as a callback identifier - it is passed into a service primitive, and the callback
	carries this value to identify the service primitive which it is paired with 
*/
typedef void *tid_t; 



/* Callback type for all service primitives which require callbacks */
typedef void (*callbackFuncPtr_t)(u_int32 cbHandle, t_api status);

typedef struct st_t_BT_Authorisation {
	u_int8 direction; /* INCOMING or OUTGOING */
	t_bdaddr address;
	u_int8 serviceType;
        union {
          char *serviceName;
          u_int16 protocolId;
        }service;
	u_int16 multiplexProtocolId;
	u_int32 multiplexChannelId;
} t_BT_Authorisation;


typedef struct st_BT_Authorisation_t BT_Authorisation_t;


#define L2CAP_PROTOCOLID  1
#define RFCOMM_PROTOCOLID 2
#define SDP_PROTOCOLID    3
#define TCSBIN_PROTOCOLID 4


/* Spec defined release reasons */

#define PRH_USER_ENDED_CONNECTION	0x13
#define PRH_LOW_RESOURCES			0x14
#define PRH_ABOUT_TO_POWER_OFF		0x15
#define PRH_TERMINATED_BY_LOCAL_HOST 0x16

#include "host_buf.h"
#endif /* HOST_TYPES_H */
