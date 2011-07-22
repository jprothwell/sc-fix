/**************************************************************************
 * MODULE NAME:    l2_utils.h
 * PROJECT CODE:
 * DESCRIPTION:    
 * MAINTAINER:     Gary Fleming
 * DATE:           02-12-1999
 *
 * SOURCE CONTROL: $Id: l2_utils.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 *
 * SOURCE: 
 *
 *************************************************************************/

#ifndef __PARTHUS_L2CAP_UTILS
#define __PARTHUS_L2CAP_UTILS

u_int8 prh_l2_utils_addr_match(t_bdaddr* p_bd_addr1,t_bdaddr* p_bd_addr2);
void   prh_l2_utils_set_bd_addr(t_bdaddr* p_dest_bd_addr, t_bdaddr* p_src_bd_addr);


t_api prh_l2cap_layer_ctrl(u_int8 flags);

t_api prh_l2_utils_out_mtu(u_int16 cid,u_int16* mtu);

#endif
