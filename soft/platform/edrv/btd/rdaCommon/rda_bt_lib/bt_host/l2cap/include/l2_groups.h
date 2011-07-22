/**************************************************************************
 * MODULE NAME:    l2_groups.h
 * PROJECT CODE:
 * DESCRIPTION:    l2cap groups handler.
 * AUTHOR:         Gary Fleming
 * DATE:           02-07-2000
 *
 * SOURCE CONTROL:
 *
 * LICENSE
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Plc.
 *     All rights reserved.
 *
 * DESCRIPTION
 * This module is responsible for the support of L2CAP groups and the reception
 * of connection less traffic.
 *************************************************************************/
#ifndef __PARTHUS_L2_GROUPS_H_
#define __PARTHUS_L2_GROUPS_H_

#include "l2_types.h"

extern  prh_t_cls_psm prh_l2_cls_psm_list[];

t_api prh_l2_group_lp_add_complete(prh_t_l2_acl_link* p_link,t_api add_status);

#endif
