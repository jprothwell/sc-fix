/****************************************************************
 *
 * MODULE NAME:    obex_client_interface.h
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_client_interface.h 1532 2010-08-23 05:47:34Z huazeng $
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
#ifndef OBEX_CLIENT_INTERFACE

#include "host_config.h"

#if OBEX_CLIENT
#define OBEX_CLIENT_INTERFACE

/* command functions */
#include "obex_client_types.h"

APIDECL1 t_api APIDECL2 OBEX_Connect(u_int32 tid_s, u_int8 version, u_int8 flags, u_int16 max_length, u_int16 target_length, u_int8* target, u_int16 header_length, t_DataBuf *txbuffer, t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_Disconnect(u_int32 tid_s, u_int16 header_length, t_DataBuf *tx_buffer, t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_Put(u_int32 tid_s, u_int8 final, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_Get(u_int32 tid_s, u_int8 final, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_SetPath(u_int32 tid_s, u_int8 flags, u_int8 constants, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_Abort(u_int32 tid_s, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler);

APIDECL1 t_api APIDECL2 OBEX_GetMTU(u_int32 tid_s,u_int16* mtu);
/* the connection routines */
APIDECL1 t_api APIDECL2 OBEX_TransportConnect_RF(t_bdaddr address,u_int8 server_channel, u_int32* tid_b, u_int16 mtu,void (*callbackFunc)(u_int32 tid_s));
//APIDECL1 t_api APIDECL2 OBEX_TransportConnect_RF(t_bdaddr address,u_int8 server_channel, u_int32* tid, u_int16 mtu,void (*disconnect)(u_int32 tid));

/*int OBEX_TransportConnect_Serial(port);

int OBEX_TransportConnect_Socket(address,port);*/


APIDECL1 t_api APIDECL2 OBEX_TransportDisconnect(u_int32 tid_s); /* have a special case of fffffffff 
					      that will flush all transports */

#endif /* OBEX CLIENT */

#endif /* OBEX CLIENT INTERFACE */
