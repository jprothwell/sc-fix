/****************************************************************
 *
 * MODULE NAME:    obex_sync_client.h
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_sync_client.h 1532 2010-08-23 05:47:34Z huazeng $
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

#ifndef OBEX_SYNC_CLIENT_INTERFACE
#include "host_config.h"

#if OBEX_SYNC_CLIENT
#define OBEX_SYNC_CLIENT_INTERFACE

#include "l2cap_primitives.h"

APIDECL1 t_api APIDECL2 OBEX_RegisterSyncCommandServer(u_int8 channel);
APIDECL1 t_api APIDECL2 OBEX_FindSyncServer(t_bdaddr* address,u_int8* channel,u_int8* formats);
		
#endif /* OBEX_SYNC_CLIENT */
#endif /* OBEX_SYNC_CLIENT_INTERFACE */