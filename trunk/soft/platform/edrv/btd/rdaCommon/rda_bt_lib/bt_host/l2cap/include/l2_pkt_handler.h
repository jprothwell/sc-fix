/******************************************************************
 * MODULE NAME:    l2_pkt_handler.h
 * PROJECT CODE:
 * DESCRIPTION:    L2CAP Packet Handler
 * MAINTAINER:     Gary Fleming
 * DATE:           21 July 1999
 *
 * SOURCE CONTROL: $Id: l2_pkt_handler.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 *    
 ******************************************************************/


#ifndef _PARTHUS_L2CAP_PKT_HANDLER_
#define _PARTHUS_L2CAP_PKT_HANDLER_

#include "host_config.h"
#include "host_types.h"

t_api prh_l2_pkt_handler(u_int16 header_cid,u_int16 handle,struct host_buf* p_buf);

#endif
