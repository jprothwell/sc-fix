/****************************************************************
 *
 * MODULE NAME:    obex_ob_push_server.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Object Push Profile
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_ob_push_server.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#ifndef OBEX_PUSH_SERVER_INTERFACE

#include "host_config.h"

#if OBEX_PUSH_SERVER
#define OBEX_PUSH_SERVER_INTERFACE

#include "host_types.h"
#include "host_buf.h"
#include "papi.h"
#include "host_error.h"
#include "obex_client_types.h"
#include "l2cap_primitives.h"

APIDECL1 t_api APIDECL2 OBEX_PushConnectResponse(u_int32 tid, u_int16 maxLength, u_int8 responseCode);
APIDECL1 t_api APIDECL2 OBEX_PullResponse(u_int32 tid_s, u_int8 responseCode, u_int16 *name, u_int8 nameLen, u_int8* data, u_int16 dataLen, u_int16 totalLen);
APIDECL1 t_api APIDECL2 OBEX_RegisterObjectPushServer(u_int8 channel, u_int8 supported_types);




#endif /*OBEX_PUSH_SERVER*/
#endif /*OBEX_PUSH_SERVER_INTERFACE*/
