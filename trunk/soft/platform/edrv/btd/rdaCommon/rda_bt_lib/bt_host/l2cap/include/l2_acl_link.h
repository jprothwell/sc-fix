/**************************************************************************
 * MODULE NAME:    l2_acl_link.h
 * PROJECT CODE:
 * DESCRIPTION:    L2CAP ACL link managment functions
 * AUTHOR:         Gary Fleming
 * DATE:           20-08-2000
 *
 * SOURCE CONTROL: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 **************************************************************************/

#ifndef _PARTHUS_L2_ACL_LINK
#define _PARTHUS_L2_ACL_LINK


#include "host_config.h"

#if COMBINED_HOST==0
extern prh_t_l2_acl_link* prh_l2_acl_links[PRH_L2_MAX_NUM_LINKS];
#else
extern prh_t_l2_acl_link prh_l2_acl_links[PRH_L2_MAX_NUM_LINKS];
#endif

prh_t_l2_acl_link* prh_l2_acl_exists(t_bdaddr* p_bd_addr);
prh_t_l2_acl_link* prh_l2_acl_find_handle(u_int16 handle);
prh_t_l2_acl_link* prh_l2_acl_alloc_link(t_bdaddr* p_bd_addr, u_int16 pending_cid);

void prh_l2_acl_free_link(prh_t_l2_acl_link* p_link);
void prh_l2_acl_shutdown_all(u_int32 flags);
void prh_l2_acl_start_RTX_timer(u_int16 handle);
void prh_l2_acl_disable_RTX_timer(prh_t_l2_acl_link* p_link);

t_bdaddr* prh_l2_acl_get_bd_addr(u_int16 handle);

#endif
