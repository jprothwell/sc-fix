/****************************************************************
 *
 * MODULE NAME:    obex_server_types.h
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_server_types.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/


#ifndef OBEX_SERVER_TYPES
#include "host_config.h"

#if OBEX_SERVER
#define OBEX_SERVER_TYPES

#include "obex_utils.h"

typedef void (*t_obex_service_handler)(u_int32 tid, u_int8 messageType, t_obex_headers headers, u_int8 *data,u_int16 len,void* cbref);

/*
  I might move these to another header file layer.
 */

struct prh_obex_session_state
{
  u_int32 tid;
  u_int32 server_tid;
  u_int8 state;
  t_obex_service_handler handler;
  struct prh_obex_session_state* p_next;
};

struct prh_obex_inbox_registration
{
  u_int32 tid;
  t_obex_service_handler handler;
	struct prh_obex_inbox_registration * p_next;
};

struct prh_obex_service_registration
{
  u_int32 tid;
  t_obex_service_handler handler;
  u_int8* target;
  u_int8 target_length;
  u_int32 cid;
	u_int8 cid_length;
  struct prh_obex_service_registration* p_next;
};

#endif /* OBEX_SERVER */

#endif /* OBEX_SERVER_TYPES */
