/****************************************************************
 *
 * MODULE NAME:    obex_sec.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    MD5 Security algorithm implementation
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_sec.h 1532 2010-08-23 05:47:34Z huazeng $
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
#ifndef OBEX_SEC_LOWER
#include "host_config.h"

#if OBEX_SEC
#define OBEX_SEC_LOWER


#include "host_types.h"
#include "host_buf.h"
#include "papi.h"
#include "host_error.h"


/*typedef unsigned long word32;*/
/*typedef unsigned char byte; */

typedef u_int8 t_obex_nonce[18];
typedef u_int8 t_obex_digest[18];

#define PRH_MAX_PINCODE_SIZE 20


struct prh_MD5Context {
        u_int32 buf[4];
        u_int32 bytes[2];
        u_int32 in[16];
};


APIDECL1 t_api APIDECL2 OBEX_GenerateNonce(u_int8 pinLen, u_int8 *pinCode, u_int8 *nonce);
APIDECL1 t_api APIDECL2 OBEX_GenerateDigest(u_int8 pinLen, u_int8 *pinCode, u_int8 *nonce, u_int8 *digest);

void prh_MD5(void *dest, void *orig, int len);
void prh_MD5Init(struct prh_MD5Context *context);
void prh_MD5Update(struct prh_MD5Context *context, u_int8 *buf, int len);
void prh_MD5Final(u_int8 digest[16], struct prh_MD5Context *context);
void prh_MD5Transform(u_int32 buf[4], u_int32 const in[16]);


#endif /* OBEX_SEC */
#endif /* OBEX_SEC_LOWER */
