/**************************************************************************
 * MODULE NAME:    l2_sar.h
 * PROJECT CODE:
 * DESCRIPTION:    l2cap Segmentation and Re-assembly Function
 * MAINTAINER:         Gary Fleming
 * DATE:           02-12-1999
 *
 * SOURCE CONTROL: $Id: l2_sar.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 *
 * SOURCE: 
 *
 *************************************************************************/

#ifndef __PARTHUS_L2CAP_SAR
#define __PARTHUS_L2CAP_SAR

#include "host_config.h"

t_api prh_l2_sar_data_ind(u_int16 handle, struct host_buf* incoming_buffer, u_int8 flags);
t_api prh_l2_sar_data_req(u_int16 handle, u_int16 cid, struct host_buf* outgoing_buffer);


extern int g_l2cap_acl_size;

#define PRH_L2CAP_HEADER_SIZE                   4

#endif
