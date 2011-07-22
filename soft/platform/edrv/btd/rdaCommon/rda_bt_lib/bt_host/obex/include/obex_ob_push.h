/****************************************************************
 *
 * MODULE NAME:    obex_ob_push.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Object Push Profile
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_ob_push.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#ifndef OBEX_PUSH_CLIENT_INTERFACE
#include "host_config.h"

#if OBEX_PUSH_CLIENT
#define OBEX_PUSH_CLIENT_INTERFACE



#include "host_types.h"
#include "host_buf.h"
#include "papi.h"
#include "host_error.h"
#include "obex_client_types.h"
#include "l2cap_primitives.h"

#define OBEX_VCARD 1
#define OBEX_VCAL  2
#define OBEX_VNOTE 3
#define OBEX_VMESS 4
#define OBEX_VOTHER 5


APIDECL1 t_api APIDECL2 OBEX_ObjectPush(u_int32 tid_s, u_int8 type, u_int16 *name, u_int16 nameLen, u_int8 *data, u_int16 len, u_int32 totalLen,u_int8 final,t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_ObjectPull(u_int32 tid_s, t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_ObjectExchange(u_int32 tid_s, u_int16 *name, u_int16 nameLen, u_int8 *data, u_int16 len, u_int32 totalLen,t_obex_command_handler handler);

#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.2.20
APIDECL1 t_api APIDECL2 OBEX_FindObjectPushServer(t_bdaddr* address,u_int8* channel,u_int8* formats);
#else
APIDECL1 t_api APIDECL2 OBEX_FindObjectPushServer(t_bdaddr* address,u_int8* channel,u_int8* formats, void (*callbackFunc)(t_bdaddr address,u_int8 channel,u_int8* formats));
#endif

#endif /* OBEX_PUSH_CLIENT */
#endif /* OBEX_PUSH_CLIENT_INTERFACE */
