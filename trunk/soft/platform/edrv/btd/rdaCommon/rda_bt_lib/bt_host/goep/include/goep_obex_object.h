/****************************************************************
 *
 * MODULE NAME:    goep_obex_object.h
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

#ifndef GOEP_OBEX_HEADER

#define PRH_OBEX_MAX_FIELDS_LENGTH 6

struct prh_obex_object
{
  u_int8 opcode;
  u_int32 header_length;
  u_int32 body_length;
  u_int8 fields[PRH_OBEX_MAX_FIELDS_LENGTH];
  
  u_int8* pBody;
};

struct prh_obex_object* prh_obex_create_object(void);

u_int8 prh_obex_delete_object(struct prh_obex_object* this);

u_int8 prh_obex_add_header(struct prh_obex_object* this,u_int8* pHeader);

/*
u_int8 prh_obex_send(struct prh_obex_object* pObject);
*/


#endif /* GOEP OBEX HEADER */







