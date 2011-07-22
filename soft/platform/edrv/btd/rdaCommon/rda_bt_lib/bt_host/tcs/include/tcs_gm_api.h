/*
 * MODULE NAME:    tci_primitives.h
 * DESCRIPTION:    TCI Service Primitive Declarations
 * AUTHOR:         David Airlie
 *
 * SOURCE CONTROL: $Id: tcs_gm_api.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */             

#ifndef PRH_TCI_PRIMITIVES_H
#define PRH_TCI_PRIMITIVES_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "tcs_cc_types.h"

/* obtain access rights*/
APIDECL1 t_api APIDECL2 TCS_ObtainWUGAccessRights(t_bdaddr gw_addr);

APIDECL1 t_api APIDECL2 TCS_AccessRightsResponse(t_TCS_wugMember *wugMember, u_int8 accept);


/* config distribution */
APIDECL1 t_api APIDECL2 TCS_DistributeWUGConfigInfo(void);

APIDECL1 t_api APIDECL2 TCS_TLConfigAcceptMode(int mode);



/* fast inter-member access */
APIDECL1 t_api APIDECL2 TCS_ListenRequest(char *int_num);

APIDECL1 t_api APIDECL2 TCS_ListenResponse(u_int8 accept, u_int8 cause);

#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/


#endif
