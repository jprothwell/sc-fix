/****************************************************************
 *
 * MODULE NAME:    obex_sync_server.h
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_sync_server.h 1532 2010-08-23 05:47:34Z huazeng $
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

#ifndef OBEX_SYNC_SERVER_INTERFACE
#include "host_config.h"

#if OBEX_SYNC_SERVER
#define OBEX_SYNC_SERVER_INTERFACE

#include "l2cap_primitives.h"

APIDECL1 t_api APIDECL2 OBEX_SuggestSync(u_int32 tid_s, u_int8 type, char* pAny, u_int8 length,t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_RegisterSyncServer(u_int8 channel, u_int8 supported_types);
APIDECL1 t_api APIDECL2 OBEX_FindSyncCommandServer(t_bdaddr* address,u_int8* channel);

#endif /* OBEX_SYNC_SERVER */
#endif /* OBEX_SYNC_SERVER_INTERFACE */