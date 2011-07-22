/****************************************************************
 *
 * MODULE NAME:    obex_ftp_client.h
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_ftp_client.h 1532 2010-08-23 05:47:34Z huazeng $
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

#ifndef OBEX_FTP_CLIENT_INTERFACE
#include "host_config.h"

#if OBEX_FTP_CLIENT
#define OBEX_FTP_CLIENT_INTERFACE

#include "l2cap_primitives.h"

#if (SINGLE_THREAD_FLAG==0) 
APIDECL1 int APIDECL2 OBEX_FindFTPServer(t_bdaddr* address,u_int8* channel);
#else
APIDECL1 int APIDECL2 OBEX_FindFTPServer(t_bdaddr* address,u_int8* channel,u_int8* formats, void (*callbackFunc)(t_bdaddr address,u_int8 channel,u_int8* formats));
#endif
APIDECL1 t_api APIDECL2 OBEX_FTPConnect(u_int32 tid_s, u_int16 mtu, t_DataBuf *txBuffer,u_int16 headerLength, t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_Delete(u_int32 tid_s, u_int32 cid, u_int16* name, u_int8 length, t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_CreateFolder(u_int32 tid_s,u_int32 cid, u_int16* name,u_int8 length, t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_ChooseCurrentFolder(u_int32 tid_s, u_int32 cid, u_int16* name, u_int8 length,t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_FTPPushFile(u_int32 tid_s, u_int32 cid, u_int16 *name, u_int16 nameLen, u_int8 *data, u_int16 dataLen, u_int32 totalLen,u_int8 final,t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_FTPPullFile(u_int32 tid_s, u_int32 cid, u_int16 *name, u_int16 nameLen, t_obex_command_handler handler);
APIDECL1 t_api APIDECL2 OBEX_GetFolderListingObject(u_int32 tid_s, u_int32 cid, t_obex_command_handler handler);

#endif /* OBEX CLIENT */

#endif /* OBEX_FTP_CLIENT_INTERFACE */
