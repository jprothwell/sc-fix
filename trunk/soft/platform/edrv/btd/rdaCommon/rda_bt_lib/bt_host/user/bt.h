/***********************************************************************
 *
 * MODULE NAME:    bt.h
 * DESCRIPTION:    Bluetooth Host Stack API 
 * AUTHOR:		   Martin Mellody
 *
 * SOURCE CONTROL: $Id: bt.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *	   This source code is copyright (c) 2000-2002 Parthus Technologies.
 *	   All rights reserved.
 *
 ***********************************************************************/


/*
	CONFIGURATION OPTIONS:
	
	Before including this file, or in bt_sys.h....

	#define BLUETOOTH_NO_SDP to omit all of the sdp headers
	#define BLUETOOTH_NO_RFCOMMM to omit all of the rfcomm headers
	#define BLUETOOTH_NO_EXTAPI to omit all of the extended API headers
	#define BLUETOOTH_NO_SDP_SERVER to omit the sdp server headers
	#define BLUETOOTH_NO_SDP_CLIENT to omit the sdp client headers
	#define BLUETOOTH_NO_SECURITY to omit the host security headers
  
*/



#ifndef BLUETOOTH_HOST_DECLARED
#define BLUETOOTH_HOST_DECLARED

#ifdef __cplusplus
extern "C" {
#endif

#include "bt_sys.h"

/* The version number of the headers should match that of the core code */
#define BT_HOST_VERSION 	1 /* major software revision */
#define BT_HOST_REVISION	7 /* minor software revision */
#define BT_HOST_PATCHLEVEL	4 /* software patch level	 */


/* 
	Platform-independent types 
*/

/* bluetooth device address structure */
typedef struct st_t_bdaddr {
	u_int8 bytes[6];
} t_bdaddr;


/* data transport buffer */
typedef struct st_t_dataBuf {
	u_int8 *data;
	u_int32 dataLen;
	u_int32 bufLen;
} t_DataBuf;

typedef u_int16 t_channelId;

/* Error values returned by API functions */
#define BT_NOERROR			    0x00 
#define BT_PENDING			    0x01 
#define BT_INVALIDPARAM		    0x02 
#define BT_SECFAIL			    0x03 
#define BT_PINFAIL			    0x04 
#define BT_FLOWCTRL			    0x05
#define BT_NORESOURCES		    0x06 
#define BT_UNSUPPORTED		    0x07 
#define BT_HCITERROR		    0x08
#define BT_NOPRIV			    0x09
#define BT_HWERROR			    0x0A
#define BT_HOSTERROR		    0x0B
#define BT_UNKNOWNERROR		    0x0C
#define BT_HOSTINACTIVE		    0x0D
#define BT_RETRY			    0x0E
#define BT_AUTHORISATIONFAIL    0x0F
#define BT_AUTHENTICATIONFAIL   0x10
#define BT_ENCRYPTFAIL		    0x11
#define BT_TIMEOUT			    0x12
#define BT_PROTOERROR		    0x13
#define BT_DISALLOWED           0x14



#include "hci.h"

#include "l2cap.h"

#include "manager.h"


#ifndef BLUETOOTH_NO_SDP 
#	include "sdp.h"
#endif

#ifndef BLUETOOTH_NO_RFCOMMM 
#	include "rfcomm.h"
#endif

#ifndef BLUETOOTH_NO_OBEX 
#	include "obex.h"
#endif


#ifndef BLUETOOTH_NO_TCS
#	include "tcs.h"
#endif


/* Bluetooth Inquiry Access Codes for HCI_Inquiry/MGR_Inquiry/SDP_EnumerateDevices */
#define BT_GIAC  0x9e8b33	/* General/Unlimited IAC */
#define BT_LIAC  0x9e8b00	/* Limited IAC */
/*
//#ifdef printf
#undef printf
#define printf(...)   mmi_trace(1,...)
//#endif
*/



#endif /* BLUETOOTH_HOST_DECLARED */
