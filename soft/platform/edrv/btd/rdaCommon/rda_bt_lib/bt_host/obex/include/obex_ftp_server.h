/****************************************************************
 *
 * MODULE NAME:    obex_ftp_server.h
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_ftp_server.h 1532 2010-08-23 05:47:34Z huazeng $
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
#ifndef OBEX_FTP_SERVER_INTERFACE
#include "host_config.h"

#if OBEX_FTP_SERVER
#define OBEX_FTP_SERVER_INTERFACE

#include "l2cap_primitives.h"	
#if 0
u_int8 OBEX_FTP_UUID	[16] = {
	0xf9,0xec,0x7b,0xc4,0x95,0x3c,0x11,0xd2,0x98,0x4e,0x52,0x54,0x00,0xdc,0x9e,0x09};
#endif

APIDECL1 t_api APIDECL2 OBEX_RegisterFTPServer(u_int8 channel);
/*APIDECL1 t_api APIDECL2 OBEX_FTPConnectResponse(u_int32 tid, u_int16 maxLength,u_int8 responseCode);*/
APIDECL1 t_api APIDECL2 OBEX_ChooseCurrentFolderResponse(u_int32 tid_s, u_int8 responseCode, u_int8 *folderListingOb, u_int8 floLen, u_int16 totalLen);
APIDECL1 t_api APIDECL2 OBEX_FTPPullResponse(u_int32 tid_s, u_int8 responseCode, u_int16 *name, u_int8 nameLen, u_int16 totalLen, u_int8* data, u_int16 dataLen);


#endif /* OBEX FTP SERVER */
#endif /* OBEX_FTP_SERVER_INTERFACE*/
