/**************************************************************************
 * MODULE NAME:    l2_bufalloc.h
 * PROJECT CODE:
 * DESCRIPTION:    l2cap Buffer allocation
 * MAINTAINER:         Dave Airlie
 *
 * SOURCE CONTROL: $Id: l2_bufalloc.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001-2002 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 *
 * SOURCE: 
 *
 *************************************************************************/

#ifndef PRH_L2CAP_BUFALLOC_H
#define PRH_L2CAP_BUFALLOC_H

t_api prh_l2_GetWriteBuffer(u_int16 cid, u_int16 len, u_int32 flags, struct host_buf **writeBuffer);
t_api prh_l2_FreeWriteBuffer(struct host_buf *writeBuffer);

#endif
