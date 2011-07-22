/***********************************************************************
 *
 * MODULE NAME:    goep_obex_object.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Object Exchange Profile
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: goep_obex_object.c 1532 2010-08-23 05:47:34Z huazeng $
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
#include "goep_obex_object.h"
#include "papi.h"

/***********************************************************************
 *
 * FUNCTION NAME:   prh_obex_create_object
 * DESCRIPTION:
 
 This function will allocate space for a header. It will then create the
header with the supplied data.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
struct prh_obex_object* prh_obex_create_object(void)
{
  struct prh_obex_object* pObject;

  pObject = pMalloc(sizeof(struct prh_obex_object));

  if(pObject)
    {
      pMemset(pObject,sizeof(struct prh_obex_object),0x00);
    }

  return pObject;
}


u_int8 prh_obex_delete_object(struct prh_obex_object* this)
{
  if(this->pBody)
    {
      pFree(this->pBody);
    }

  /*
    free headers.
   */

  pFree(this);
  return 0;
}

u_int8 prh_obex_add_header(struct prh_obex_object* pObject,u_int8* pHeader)
{
  return 0;
}
