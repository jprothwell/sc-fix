/**************************************************************************
 * MODULE NAME:    l2_bufalloc.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap buffer allocation
 * MAINTAINER:     Dave Airlie
 * DATE:           Sep 2001
 *
 * SOURCE CONTROL: $Id: l2_bufalloc.c 2719 2010-10-11 07:02:08Z shenh $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 *
 *************************************************************************/


#include "host_config.h"
#include "host_types.h"
#include "l2cap_lower_layer.h"
#include "l2_types.h"
#include "l2_channel.h"


#include "l2_connect_sm.h"
#include "l2_sar.h"
#include "l2cap_primitives.h"
#include "papi.h"
#include "host_types.h"
#include "host_error.h"

#include "l2_bufalloc.h"
#include "hci_flow_ctrl.h"

#define PRH_L2_BUFALLOC_ACTIVE_BROADCAST 0x4000
#define PRH_L2_BUFALLOC_PICONET_BROADCAST 0x8000
extern u_int16 prh_active_broadcast_handle, prh_piconet_broadcast_handle;
extern struct prh_st_t_hci_queue *prh_hci_acl_queue_list_head;
u_int8 get_a2dp_havebuffer(u_int16 cid,u_int16 len)//zhou siyou091228
{
    struct prh_st_t_hci_queue *currentQueue;
    u_int16 handle=0;
    u_int16 actual_handle;
    u_int16 pbflags;
    prh_t_l2_channel*p_channel=prh_l2_chn_get_p_channel(cid);
    if(!p_channel)
        return 0;
    if (p_channel->type==PRH_L2_CLS_CHANNEL)
    {
#if L2CAP_USE_PICONET_BROADCAST
        handle = PRH_L2_BUFALLOC_PICONET_BROADCAST;
#else
        handle = PRH_L2_BUFALLOC_ACTIVE_BROADCAST;
#endif
        handle |= PRH_L2_DEFAULT_CONNECTIONLESS_HANDLE;
    }
    else 
    {
        handle=p_channel->handle;
    }
    actual_handle = handle & 0xFFF;
    pbflags = handle & 0xF000;
    if (pbflags & 0x4000)
        actual_handle=prh_active_broadcast_handle;
    if (pbflags & 0x8000)
        actual_handle=prh_piconet_broadcast_handle;
    currentQueue = (struct prh_st_t_hci_queue *)prh_hci_fc_find_data_queue(prh_hci_acl_queue_list_head, actual_handle);
#if pDEBUG
#endif
    if(currentQueue)
        {
            if((currentQueue->allocatedBytes + len + sizeof(struct host_buf)+800)>STACK_MAX_OUTGOING_QUEUED_ACL_BYTES_PER_HANDLE)
                return 0;
        }
    return 1;
}

t_api prh_l2_GetWriteBuffer(u_int16 cid, u_int16 len, u_int32 flags, struct host_buf **writeBuffer) 
{
    int ret;
    struct host_buf *buf;
    prh_t_l2_channel* p_channel;
    u_int16 handle;
	
    len+=PRH_L2CAP_HEADER_SIZE;
    p_channel=prh_l2_chn_get_p_channel(cid);
    if (!p_channel)
   {
   #if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGA2DP,"prh_l2_chn_get_p_channel failure \n" ));
   #endif
        return BT_INVALIDPARAM;
    }
	
    if (p_channel->type==PRH_L2_CLS_CHANNEL)
	{
#if L2CAP_USE_PICONET_BROADCAST
		handle = PRH_L2_BUFALLOC_PICONET_BROADCAST;
#else
		handle = PRH_L2_BUFALLOC_ACTIVE_BROADCAST;
#endif
		handle |= PRH_L2_DEFAULT_CONNECTIONLESS_HANDLE;
	}
    else 
	{
		handle=p_channel->handle;
	}

    ret=prh_hci_GetWriteBuffer(handle, len, flags, &buf);
    if (ret!=BT_NOERROR)
		return ret;
    
    host_buf_reserve_header(buf,PRH_L2CAP_HEADER_SIZE);
    
    *writeBuffer=buf;
    return BT_NOERROR;
}

t_api prh_l2_FreeWriteBuffer(struct host_buf *writeBuffer)
{
  return prh_hci_FreeWriteBuffer(writeBuffer);

}


