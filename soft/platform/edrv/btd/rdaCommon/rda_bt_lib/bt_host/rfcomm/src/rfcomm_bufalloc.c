/***********************************************************************
 *
 * MODULE NAME:    rfcomm_bufalloc.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM Buffer Allocation
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  Sept 2001
 *
 * SOURCE CONTROL: $Id: rfcomm_bufalloc.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    15.Mar.2000 -   DA - first import
 *
 *
 ***********************************************************************/

#include "rfcomm_lower_layer.h"
#include "rfcomm_frame.h"

#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_proto.h"
#include "rfcomm_debug.h"

t_api PRH_RFCOMM_UE_Get_Test_Buffer(t_bdaddr bd_addr, u_int16 frame_size, struct host_buf **frame)
{
  int ret;
  ret=PRH_RFCOMM_UE_Get_Write_Buffer(bd_addr, 0, (u_int16)(frame_size+4), 0, frame);
  host_buf_reserve_header(*frame, 4);
  return ret;
}

t_api PRH_RFCOMM_UE_Get_Write_Buffer(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, u_int16 frame_size, u_int32 flags, struct host_buf **frame)
{
  int ret;
  u_int16 length=frame_size;
  struct prh_rfcomm_l2cap_inst *l2cap_inst_ptr;
  struct host_buf *buf;
  
  length += PRH_RFCOMM_BUF_UIH_LONG+PRH_RFCOMM_BUF_FCS;
  
  ret=prh_rfcomm_lookup_l2cap_inst_by_bd_addr(bd_addr, &l2cap_inst_ptr);
  if (ret!=BT_NOERROR)
    return ret;
  
  ret=prh_l2_GetWriteBuffer(l2cap_inst_ptr->cid, length, flags, &buf);
  
  if (ret!=BT_NOERROR)
    return ret;
  
  host_buf_reserve_header(buf,PRH_RFCOMM_BUF_UIH_LONG);

  *frame=buf;
  
  return BT_NOERROR;
}

t_api PRH_RFCOMM_UE_Release_Write_Buffer(struct host_buf *frame)
{
  prh_l2_FreeWriteBuffer(frame);
  return BT_NOERROR;
}

