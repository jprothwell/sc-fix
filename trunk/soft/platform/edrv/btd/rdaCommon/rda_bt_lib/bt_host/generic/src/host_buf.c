/***********************************************************************
 *
 * MODULE NAME:    host_buf.c
 * DESCRIPTION:    Host Side Buffers
 * MAINTAINER:     Dave Airlie
 *
 * SOURCE CONTROL: $Id: host_buf.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies.
 *     All rights reserved.
 *
 *    
 ***********************************************************************/

#include "host_config.h"

#include "host_types.h"

#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_papi.h"
#else
#include "papi.h"
#endif

#include "host_error.h"
#include "host_buf.h"

#if COMBINED_HOST==1
#include "tra_queue.h"
#include "lmp_acl_container.h"

#define MAX_HOST_BUFF_COUNT		10
struct host_buf _host_buffs[MAX_HOST_BUFF_COUNT];
#endif

/*
 * host_buf_alloc
 *
 * Parameters:
 *    size : size of buffer to allocate
 * Returns:
 *    host_buf ptr: a pointer to the allocated host buffer
 *
 * This function allocates a standard host buffer along with data of size
 * size. The host buffer returned can be used as a parent buffer.
 * modified to do a single malloc.. for use when been called in interrupt
 * context
 */
struct host_buf * host_buf_alloc(u_int32 size)
{
  struct host_buf *ret = NULL;
#if COMBINED_HOST==1
  int i;

  for(i=0; i<MAX_HOST_BUFF_COUNT; i++)
  {
  	if(_host_buffs[i].flags == 0)
  	{
            memset(&_host_buffs[i], 0, sizeof(_host_buffs[i]));
  		ret = _host_buffs+i;
        ret->len = size;
		ret->flags = 1;
		break;
  	}
  }
#else
  int length;
  u_int32  nCallerAdd = 0x00;
  COS_GET_RA(&nCallerAdd);

  length=size+sizeof(struct host_buf);
  ret=(struct host_buf *)pMalloc(length);
  if (ret==pNULL)
    return pNULL;
  COS_UpdateMemCallAdd(ret,nCallerAdd);
  ret->head=(((u_int8 *)ret)+sizeof(struct host_buf)); /* NB alignment */
  
  ret->len=0;
  ret->totalsize=size;
  ret->data=ret->head;
  ret->tail=ret->data+size;
  ret->child_count=1;
  ret->parent=NULL;
  ret->flags=PRH_HOST_BUF_HCI_HEADER_PRESENT;
  ret->next=NULL;
#endif  
  return ret;
}

/*
 * host_buf_child_alloc
 *
 * Parameters:
 *   parent: the host buffer whose parent this buffer is.
 *   new_data: a pointer to the start of the data in the parent that this buffer will contain
 *   length: length of the parent's data that this buffer contains
 * 
 * Returns:
 *   host_buf ptr: the child host buffer
 *
 * This function allocates a host buffer structure and points its pointers
 * into the data of another host buffer, it also increases the parent buffers
 * child count and sets the parent pointer in the child
 */
struct host_buf * host_buf_child_alloc(struct host_buf *parent, u_int8 *new_data, u_int32 length)
{
  struct host_buf *ret = NULL;
#if COMBINED_HOST==0
  
  ret=(struct host_buf *)pMalloc(sizeof(struct host_buf));
  if (ret==pNULL)
    return pNULL;
  
  ret->len=length;
  ret->parent=parent;
  ret->child_count=0;
  ret->parent->child_count++;
  ret->data=new_data;
  ret->head=ret->tail=NULL;
  ret->next=NULL;
  ret->flags=PRH_HOST_BUF_OUTGOING_PACKET;
  ret->totalsize=length;
#endif
 
  return ret;
}
/*
 * host_buf_free 
 * 
 * Parameters:
 *    buf: host buffer to free
 *
 * Returns:
 *    void
 *
 * This function releases a buffer from the system using the following method:
 *
 * If the buffer is a parent buffer:
 *     Decrease child count,
 *     If no children left
 *             Free the data pointer
 *     Free this buffer structure.
 * 
 * If the buffer has a parent:
 *     Decrease the parent's child counter
 *     If the Parent's child counter is zero:
 *             Free the data storage
 *             Free the parent structure
 *     Free this buffer.
 *
 */
void host_buf_free(struct host_buf *buf)
{
#if COMBINED_HOST==0
    if(buf==NULL)
        return;
  /* if the buffer is a parent */
  if (buf->parent==NULL)
    {
      buf->child_count--;
      if (buf->child_count==0)
    pFree(buf);
    }
  else 
    {
      /* if the buffer is a child */
      buf->parent->child_count--;
      if (buf->parent->child_count==0)
    {
      /* If the child count is zero... free the parent's 
         data buffers, and the parent buffer */
      pFree(buf->parent);
    }
      pFree(buf);
    }
#else
    buf->flags = 0;
#endif
}

/*
 * host_buf_reserve_header
 *
 * Parameters:
 *   buf : host buffer to reserve header in
 *   size: amount of header to reserve
 *
 * Returns
 *   t_api: E_SYS_BUF_ILLOP: Illegal operation performed on buffer
 * 
 * This function only works on parent buffers, 
 * The buffers data pointer is pushed into reserve space at the start of the
 * buffer, the buffer length is also modified.
 *
 */
t_api host_buf_reserve_header(struct host_buf *buf, u_int32 size)
{  
#if COMBINED_HOST==0
    if (buf->parent!=NULL) {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"host_buf_reserve_header: attempt to reserve %d bytes of header on a child host_buf",size));
        return BT_UNSUPPORTED;
    }
  
    if (buf->data+size > buf->tail) {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"host_buf_reserve_header: attempt to reserve %d bytes of header exceeds available space",size));
        return BT_UNSUPPORTED;
    }
#endif
  
    buf->data+=size;
    if (buf->len < size)
        buf->len=0;
    else
        buf->len-=size;
    return BT_NOERROR;
}

/*
 * host_buf_release_header
 * 
 * Parameters:
 *   buf: host buffer to release space in
 *   size: amount of space to release
 *
 * Returns:
 *   t_api: E_SYS_BUF_ILLOP: illegal usage of buffers.
 *
 * This function only work on parent buffers.
 * This function moves the buffer data pointer towards the head
 * of the buffer by size bytes, to release space that was previously
 * reserved for use as lower layer header space.
 * 
 */
t_api host_buf_release_header(struct host_buf *buf, u_int32 size)
{
#if COMBINED_HOST==0
    if (buf->parent!=NULL){
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"host_buf_release_header: attempt to release %d bytes of header on a child host_buf",size));
        return BT_UNSUPPORTED;
    }
    if (buf->data-size<buf->head){
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"host_buf_release_header: attempt to release %d bytes of header exceeds available space",size));
        return BT_UNSUPPORTED;
    }
#endif
    buf->data-=size;

    buf->len+=size;
    return BT_NOERROR;
}

#if COMBINED_HOST==1
t_api host_buf_GetWriteBuffer(u_int16 handle, u_int16 length, u_int32 flags, struct host_buf **p_buf)
{
    t_lmp_link* p_link;
    t_q_descr* qd;
    struct host_buf *buf;

    *p_buf = NULL;
    p_link = LMaclctr_Find_ACL_Handle(handle);
    do{
    qd = BTq_Enqueue(L2CAP_OUT_Q,  p_link->device_index, length);
    }while(qd==NULL);
    {
    	buf = host_buf_alloc(length);
    	if(buf)
    	{
    		qd->message_type = LCH_start;//sisi
    		buf->data = qd->data;
    		buf->len = length;
    		buf->link_handle = handle;
    		*p_buf = buf;
    	}
        else
            return BT_NORESOURCES;
    }
//    else
    //     return BT_NORESOURCES;

    return BT_NOERROR;
}

t_api host_buf_FreeWriteBuffer(struct host_buf *p_buf)
{
    t_lmp_link* p_link;
	t_q_descr* qd;
	if(p_buf->flags)
	{
		p_link = LMaclctr_Find_ACL_Handle(p_buf->link_handle);
		qd = BTq_Get_Last_Enqueue(L2CAP_OUT_Q, p_link->device_index);
		while(qd!= NULL && qd->data != p_buf->data)
		{
			qd = qd->next;
		}
		if(qd)
		{
			qd ->length = p_buf->len;
		}
        BTq_Commit(L2CAP_OUT_Q, p_link->device_index);
        host_buf_free(p_buf);
    }
    return BT_NOERROR;
}

t_api host_buf_CopyToA2dpBuffer(struct host_buf *p_buf)
{
#if(PRH_BS_CFG_SYS_A2DP_QUEUE_SUPPORT == 1)
    t_lmp_link* p_link;
    t_q_descr *l2cap_qd, *a2dp_qd;
    u_int8 *data;

    p_link = LMaclctr_Find_ACL_Handle(p_buf->link_handle);
    l2cap_qd = BTq_Get_Last_Dequeue(L2CAP_IN_Q, p_link->device_index);
    a2dp_qd = BTq_Enqueue(A2DP_IN_Q, 0, l2cap_qd->length);

    data = a2dp_qd->data;
    a2dp_qd->data = l2cap_qd->data;
    l2cap_qd->data = data;

    BTq_Commit(A2DP_IN_Q, 0);
    
#endif
    return BT_NOERROR;
}
#endif

