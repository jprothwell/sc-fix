/****************************************************************
 *
 * MODULE NAME:    obex_sec_wrap.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Object Exchange Profile
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_sec_wrap.h 1532 2010-08-23 05:47:34Z huazeng $
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

#ifndef OBEX_SEC_INTERFACE
#include "host_config.h"

#if OBEX_SEC
#define OBEX_SEC_INTERFACE

#include "host_types.h"
#include "papi.h"
#include "obex_sec.h"


APIDECL1 t_api APIDECL2 OBEX_AuthRequest(u_int8 *localPinCode, u_int8 localPinCodeLen, t_obex_nonce nonce, u_int16 *offset_len, t_DataBuf *txBuffer);
APIDECL1 t_api APIDECL2 OBEX_AuthResponse(u_int8 *remotePinCode, u_int8 remotePinCodeLen, u_int8 *remoteNonceHeader, u_int16 *offset_len, t_DataBuf *txBuffer);
APIDECL1 t_api APIDECL2 OBEX_AuthCheck(u_int8 *localPinCode, u_int8 localPinCodeLen, t_obex_nonce localNonce, u_int8 *digestHeader);



#endif /* OBEX_SEC */
#endif /* OBEX_SEC_INTERFACE */
