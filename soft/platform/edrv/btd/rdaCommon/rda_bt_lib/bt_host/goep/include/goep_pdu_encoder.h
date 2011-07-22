/****************************************************************
 *
 * MODULE NAME:    goep_pdu_encoder
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Object Exchange Profile
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id $
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

#ifndef GOEP_PDU_ENCODER

#define OBEX_CONNECT 0x80
#define OBEX_DISCONNECT 0x81
#define OBEX_PUT 0x02
#define OBEX_GET 0x03
#define OBEX_SET_PATH 0x85
#define OBEX_ABORT 0xff

u_int8 prh_goep_connect(struct prh_obex_object* pObject,u_int8 version,u_int8 flages,u_int16 maximum_length,u_int8* pTarget);

u_int8 prh_goep_put(struct prh_obex_object* pObject,u_int8* pName);

u_int8 prh_goep_get(struct prh_obex_object* pObject,u_int8* pName,u_int8* pType);


#endif /* GOEP PDU ENCODER */
