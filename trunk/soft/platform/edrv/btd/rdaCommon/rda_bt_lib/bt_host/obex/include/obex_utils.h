/****************************************************************
 *
 * MODULE NAME:    obex_utils.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Object Exchange Profile
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_utils.h 1532 2010-08-23 05:47:34Z huazeng $
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

#ifndef OBEX_UTILS
#include "host_config.h"

#define OBEX_UTILS

#include "host_types.h"
#include "host_buf.h"
#include "papi.h"
#include "host_error.h"


typedef struct st_t_obex_header_offsets
{
	u_int16 count,
					name,
					type,
					len,
					time_iso,
					time_32,
					des,
					tar,
					http,
					body,
					eob,
					who,
					cid,
					app,
					au_cal,
					au_rsp,
					cla;
}t_obex_header_offsets;

typedef struct{
	
	u_int16 nameOffset;
	u_int16 typeOffset;
	u_int16 bodyOffset;

} t_obex_headers;

/* Header encoding functions*/
APIDECL1 t_api APIDECL2 OBEX_CreateStringHeader(u_int8 id, u_int16 length, u_int16* pData, t_DataBuf *txBuffer, u_int16* pOffset);
APIDECL1 t_api APIDECL2 OBEX_CreateSequenceHeader(u_int8 id, u_int16 length, u_int8* pData, t_DataBuf *txBuffer, u_int16* pOffset);
APIDECL1 t_api APIDECL2 OBEX_CreateByteHeader(u_int8 id, u_int8 data, t_DataBuf* txBuffer, u_int16* pOffset);
APIDECL1 t_api APIDECL2 OBEX_Create4ByteHeader(u_int8 id, u_int32 data, t_DataBuf *txBuffer, u_int16* pOffset);
APIDECL1 t_api APIDECL2 OBEX_ExtractHeaders(t_obex_header_offsets* headers,u_int16* offset_len,u_int8* finger, u_int16 len);


/* Write Buffer functions*/
APIDECL1 t_api APIDECL2 OBEX_GetWriteBuffer(u_int16 len, t_DataBuf **writeBuffer);
APIDECL1 t_api APIDECL2 OBEX_FreeWriteBuffer(t_DataBuf *writeBuffer);

#endif /* obex_utils */
