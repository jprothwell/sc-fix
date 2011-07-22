/****************************************************************
 *
 * MODULE NAME:    obex_client_types.h
 * PROJECT CODE:   Bluetooth hsot
 * DESCRIPTION:    GOEP layer code.
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_client_types.h 1532 2010-08-23 05:47:34Z huazeng $
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

#ifndef OBEX_CLIENT_TYPES

#include "host_config.h"
	
#if OBEX_CLIENT

#define OBEX_CLIENT_TYPES

typedef void (*t_obex_command_handler)(u_int8 status, u_int8* data, u_int16 length);





struct prh_obex_command_callback {
  u_int32 tid;
  t_obex_command_handler callback;
  struct prh_obex_command_callback* p_next;
};	

#endif /* OBEX CLIENT */

#endif /*OBEX_CLIENT_TYPES*/
 
