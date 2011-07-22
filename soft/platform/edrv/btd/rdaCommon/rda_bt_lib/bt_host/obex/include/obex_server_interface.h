/****************************************************************
 *
 * MODULE NAME:    obex_server_interface.h
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_server_interface.h 1532 2010-08-23 05:47:34Z huazeng $
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

#ifndef OBEX_SERVER_INTERFACE
#include "host_config.h"

#if OBEX_SERVER
#define OBEX_SERVER_INTERFACE

#define RDA_FTPS_Server_Channel 0x03 /*  */
#define RDA_OPPS_Server_Channel 0x04 /*  */

#include "obex_server_types.h"

APIDECL1 t_api APIDECL2 OBEX_RegisterInbox(u_int32 tid_s,t_obex_service_handler handler, void *cref);
APIDECL1 t_api APIDECL2 OBEX_RegisterServerToInbox(u_int32 tid_s, u_int8 type, t_obex_service_handler handler);
APIDECL1 t_api APIDECL2 OBEX_RegisterService(u_int32 tid_s,t_obex_service_handler handler, u_int8 length, u_int8* target, void* cref);
APIDECL1 t_api APIDECL2 OBEX_Response(u_int32 tid_s, u_int8 response_code,u_int16 header_length, t_DataBuf* txbuffer);
APIDECL1 t_api APIDECL2 OBEX_ConnectResponse(u_int32 tid_s, u_int8 response_code, u_int8 version, u_int8 flags,u_int16 maxLen, u_int16 header_length, t_DataBuf* txbuffer);
APIDECL1 t_api APIDECL2 OBEX_GetCID(u_int32 tid_s,u_int8 target_length,u_int8* target,u_int32* cid);
APIDECL1 t_api APIDECL2 OBEX_ServerTransportDisconnect(u_int32 tid_s);

/* the listen routines must return back a transport identifier that can
be used in the service registration */
#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.2.20
APIDECL1 int APIDECL2 OBEX_TransportListen_RF(u_int8 server_channel,u_int32* tid_b,void (*disconnect)(u_int32 tid_s));
#else
APIDECL1 t_api APIDECL2 OBEX_TransportListen_RF(u_int8 server_channel,u_int32* tid_b, u_int16 max_obex,u_int8* rx_buffer,void (*disconnect)(u_int32 tid_s), void (*callbackFunc)(t_bdaddr address),void (*authorizecallbackFunc)(t_bdaddr address, u_int8 dlci, u_int8 schan, u_int8 tid_s));
#endif
APIDECL1 t_api APIDECL2 OBEX_GetConnectionInfo_RF(u_int32 tid_s,t_bdaddr *address,u_int8 *server_channel);




#endif /* OBEX_SERVER */
#endif /* OBEX_SERVER_INTERFACE*/
