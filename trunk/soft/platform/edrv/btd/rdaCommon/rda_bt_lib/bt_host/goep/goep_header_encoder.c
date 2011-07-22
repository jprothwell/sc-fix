/***********************************************************************
 *
 * MODULE NAME:    goep_header_encoder.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Object Exchange Profile
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: goep_header_encoder.c 1532 2010-08-23 05:47:34Z huazeng $
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
#include "papi.h"

/***********************************************************************
 *
 * FUNCTION NAME:   prh_obex_create_string_header
 * DESCRIPTION:
 
 This function will allocate space for a header. It will then create the
header with the supplied data.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
u_int8* prh_obex_create_string_header(u_int8 identifier,u_int16 length,u_int8* pData)
{
  u_int8* pReturn;

  pReturn = 0x00;
  
  pReturn = pMalloc(length + 3);
  
  if(pReturn)
    {
      *pReturn = identifier;

      pReturn[1] = length >> 8;
      pReturn[2] = length & 0xff;
      
      pMemcpy(pReturn + 2,pData,length);

      *pReturn &= 0x3f; /* clear bits 7 and 8*/
    }
  
  return pReturn;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_obex_create_sequence_header
 * DESCRIPTION:
 
 This function will allocate space for a header. It will then create the
header with the supplied data.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
u_int8* prh_obex_create_sequence_header(u_int8 identifier,u_int16 length,u_int8* pData)
{
  u_int8* pReturn;

  pReturn = 0x00;
  
  pReturn = prh_obex_create_string_header(identifier,length,pData);
  
  if(pReturn)
    {
      
      *pReturn &= 0x7f;
    }
  
  return pReturn;
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_obex_create_byte_header
 * DESCRIPTION:
 
 This function will allocate space for a header. It will then create the
header with the supplied data.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
u_int8* prh_obex_create_byte_header(u_int8 identifier,u_int8 data)
{
  u_int8* pReturn;

  pReturn = 0x00;
  
  pReturn = pMalloc(2);
  
  if(pReturn)
    {
      *pReturn = identifier;
      pReturn[1] = data;

      *pReturn &= 0xbf;
    }
  
  return pReturn;

}





/***********************************************************************
 *
 * FUNCTION NAME:   prh_obex_create_int_header
 * DESCRIPTION:
 
 This function will allocate space for a header. It will then create the
header with the supplied data.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
u_int8* prh_obex_create_int_header(u_int8 identifier,u_int32 data)
{
  u_int8* pReturn;

  pReturn = 0x00;
  
  pReturn = pMalloc(5);
  
  if(pReturn)
    {
      *pReturn = identifier;

      pReturn[1] = (data >> 24);
      pReturn[2] = (data >> 16) & 0xff;
      pReturn[3] = (data >> 8) & 0xff;
      pReturn[4] = data & 0xff;

      *pReturn &= 0xff;
    }
  
  return pReturn;
}










