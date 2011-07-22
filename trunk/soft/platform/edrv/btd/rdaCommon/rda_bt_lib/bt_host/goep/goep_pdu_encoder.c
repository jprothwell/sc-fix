/***********************************************************************
 *
 * MODULE NAME:    goep_pdu_encoder.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Object Exchange Profile
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: goep_pdu_encoder.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#include "host_config.h"
#include "host_types.h"
#include "goep_header_encoder.h"
#include "goep_obex_object.h"
#include "goep_pdu_encoder.h"
#include "papi.h"

/***********************************************************************
 *
 * FUNCTION NAME:   prh_goep_connect
 * DESCRIPTION:
 
 This function will allocate space for a header. It will then create the
header with the supplied data.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
u_int8 prh_goep_connect(struct prh_obex_object* pObject,u_int8 version,u_int8 flags,u_int16 maximum_length,u_int8* pTarget)
{
  u_int8* pTemp;
  pObject->opcode = OBEX_CONNECT;

  pObject->fields[2] = version;
  pObject->fields[3] = flags ? 1 : 0;
  pObject->fields[4] = maximum_length >> 8;
  pObject->fields[5] = maximum_length & 0xff;

  pObject->header_length += 6;

  if(pTarget)
    {
      pTemp = prh_obex_create_sequence_header(0x46,6,pTarget);
      if(pTemp)
    {
      prh_obex_add_header(pObject,pTemp);
    }
    }

  return 0;
}









