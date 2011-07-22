/****************************************************************
 *
 * MODULE NAME:    goep_header_encoder.h
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

#ifndef GOEP_HEADER_ENCODER

u_int8* prh_obex_create_string_header(u_int8 identifier,u_int16 length,u_int8* pData);
u_int8* prh_obex_create_sequence_header(u_int8 identifier,u_int16 length,u_int8* pData);
u_int8* prh_obex_create_byte_header(u_int8 identifier,u_int8 data);
u_int8* prh_obex_create_int_header(u_int8 identifier,u_int32 data);

#endif /* goep header encoder */
