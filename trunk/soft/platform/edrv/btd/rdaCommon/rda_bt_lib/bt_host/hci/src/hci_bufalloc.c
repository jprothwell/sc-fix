/*
 * MODULE NAME:    hci_bufalloc c
 * DESCRIPTION:    HCI Buffer Allocation
 * AUTHOR:         Dave Airlie
 *
 * SOURCE CONTROL: $Id: hci_bufalloc.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */


#include "host_config.h"

#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"
#include "hci_constants.h"
#include "papi.h"
#include "hci_flow_ctrl.h"
#include "hci_transport.h"
#include "hci_primitives.h"

extern struct prh_st_t_hci_queue prh_hci_fc_command_queue;
extern t_pMutex prh_hci_fc_command_queue_lock;

extern t_pMutex prh_hci_fc_acl_queue_lock;
extern struct prh_st_t_hci_queue *prh_hci_acl_queue_list_head;

extern struct prh_st_t_hci_queue *prh_hci_fc_find_data_queue(struct prh_st_t_hci_queue *queueListHead, u_int16 handle_t);

extern u_int16 prh_active_broadcast_handle, prh_piconet_broadcast_handle;

t_api _prh_hci_GetQueueBuffer(struct prh_st_t_hci_queue *currentQueue, u_int16 length, u_int32 flags, struct host_buf **p_buf)
{
	int ret;
	struct host_buf *buf;
	u_int8 use_resv_mem=0;
	u_int32 max_bytes; 
	
	/* Need to deal with SCO here */
	if (currentQueue == &prh_hci_fc_command_queue)
	{
		max_bytes = STACK_MAX_OUTGOING_QUEUED_HCI_COMMAND_BYTES;
		flags &= ~PRH_HOST_BUF_USE_RESV_ACL_MEM; /* make sure flags doesn't try reserved memory for commands */
	}
	else
		max_bytes = STACK_MAX_OUTGOING_QUEUED_ACL_BYTES_PER_HANDLE;
	
	if (length + sizeof(struct host_buf) > max_bytes)
		return BT_NORESOURCES;
	
	/* Try to allocate space on the standard queue */
    //pDebugPrintfEX((pLOGERROR,pLOGHCI,"currentQueue->allocatedBytes =%d, max_bytes:%d\n",currentQueue->allocatedBytes,max_bytes ));
	if (currentQueue->allocatedBytes + length + sizeof(struct host_buf) > max_bytes)
	{
	/* if there is no space available and we are a reserved flagged request
		- use the reserved region */
		/* The reserved flag shouldn't be set for command queues */
		if (flags & PRH_HOST_BUF_USE_RESV_ACL_MEM)
		{
			if (currentQueue->reservedBytes + length + sizeof(struct host_buf) > STACK_MAX_OUTGOING_CONTROL_MESSAGE_RESERVED_BYTES)
			{
				pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Alloc: Resv Queue space filled for queue\n"));
				ret=BT_FLOWCTRL;
				goto error_return;
			}
			else
				use_resv_mem=1;
		}
		else
		{
			//pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI Alloc: Queue space filled for handle\n"));
			ret=BT_FLOWCTRL;
			goto error_return;
		}
	}
	  u_int32  nCallerAdd = 0x00;
	  COS_GET_RA(&nCallerAdd);
	//pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI_ALLOC: allocating %d length on queue %p with %d bytes allocated\n", length + sizeof(struct host_buf), currentQueue, currentQueue->allocatedBytes));
	buf=host_buf_alloc(length);
	
	if (buf==NULL)
    {
        pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"host_buf_alloc failed\n"));
		ret=BT_NORESOURCES;
		goto error_return;
    }
	 COS_UpdateMemCallAdd(buf,nCallerAdd);
	/* okay we have the buffer allocated */
	
	if (use_resv_mem)
    {
		buf->flags |= PRH_HOST_BUF_USE_RESV_ACL_MEM;
		currentQueue->reservedBytes += length + sizeof(struct host_buf);
    }
	else
		currentQueue->allocatedBytes += length + sizeof(struct host_buf);
	
	buf->flags |= PRH_HOST_BUF_OUTGOING_PACKET;
	*p_buf=buf;
	
	ret=BT_NOERROR;
error_return:
	return ret;
	
}

t_api prh_hci_GetCommandBuffer(u_int16 length, u_int32 flags, struct host_buf **p_buf)
{
	int ret;
	struct host_buf *buf;
	
	pMutexLock(prh_hci_fc_command_queue_lock);
	
	/* call buffer allocation routine */
	ret=_prh_hci_GetQueueBuffer(&prh_hci_fc_command_queue, length, flags, &buf);
	if (ret!=BT_NOERROR)
		goto error_return;
	
	*p_buf=buf;
	
	ret=BT_NOERROR;
error_return:
	pMutexUnlock(prh_hci_fc_command_queue_lock);
	return ret;
	
}

/* get a write buffer once we have a handle */
t_api prh_hci_GetWriteBuffer(u_int16 handle, u_int16 length, u_int32 flags, struct host_buf **p_buf)
{
	int ret;
	struct prh_st_t_hci_queue *currentQueue;
	struct host_buf *buf;
	u_int16 actual_handle; 
	u_int16 pbflags;
	
	pMutexLock(prh_hci_fc_acl_queue_lock);
	
	length+=PRH_HCI_HEADER_SIZE; 
	actual_handle = handle & 0xFFF;
	pbflags = handle & 0xF000;
	
	if (pbflags & 0x4000)
		actual_handle=prh_active_broadcast_handle;
	if (pbflags & 0x8000)
		actual_handle=prh_piconet_broadcast_handle;
	
	/* get queue for handle */
	currentQueue = prh_hci_fc_find_data_queue(prh_hci_acl_queue_list_head, actual_handle);
	
	if (!currentQueue) {
		if (pbflags & 0xc000) {			
			pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: assigning queue for broadcast ACL handle 0x%02X\n", actual_handle));
			currentQueue=prh_hci_create_queue(actual_handle,HCI_linkACL);
		} else {
			pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_ALLOC: cannot find queue for handle %d\n", actual_handle));
			ret=BT_INVALIDPARAM;
			goto error_return;
		}
    }
	
	
	/* call buffer allocation routine */
	ret=_prh_hci_GetQueueBuffer(currentQueue, length, flags, &buf);
	if (ret!=BT_NOERROR)
		goto error_return;
	
	host_buf_reserve_header(buf, PRH_HCI_HEADER_SIZE);
	buf->hci_handle_flags = actual_handle;
	
	*p_buf=buf;
	
	ret=BT_NOERROR;
error_return:
	pMutexUnlock(prh_hci_fc_acl_queue_lock);
    
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"prh_hci_GetWriteBuffer ret=%d\n",ret));
	return ret;
	
}

/* get a SCO buffer once we have a handle */
t_api prh_hci_GetSCOBuffer(u_int16 handle, u_int16 length, u_int32 flags, struct host_buf **p_buf)
{
    /* initial SCO packet support -MM */
    *p_buf=host_buf_alloc(length);
    if (*p_buf==NULL) {
        return BT_NORESOURCES;
    } else {
        (*p_buf)->flags=0;
        return BT_NOERROR;
    }

/*
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_ALLOC: SCO data over HCI surrently unsupported\n"));
    return BT_UNSUPPORTED;
*/
}

/* this is called when we have a lock already */
t_api _prh_hci_ReleaseWriteBuffer(struct prh_st_t_hci_queue *currentQueue, struct host_buf *p_buf)
{
	u_int32 length;
	/* release the buffer from the queue space if
	we have a queue and either
	
	  1. it is a parent buffer with no children.
	  2. it is a the last child buffer of a freed parent 
	*/
	
	if (currentQueue)
    {
            //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"p_buf->parent=0x%x,p_buf->child_count=%d\n",p_buf->parent,p_buf->child_count));
		if(p_buf->parent==NULL && p_buf->child_count==1)
		{
			length=p_buf->totalsize + sizeof(struct host_buf); 
			goto remove_queue;
		}
		
		
		if (p_buf->parent!=NULL) 
              {
                //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"p_buf->parent->child_count=%d\n",p_buf->parent->child_count));
			if (p_buf->parent->child_count==1)
			{
				length=p_buf->parent->totalsize + sizeof(struct host_buf);
				goto remove_queue;
			}
		}
		goto noqueue;
		
remove_queue:
		if (p_buf->flags & PRH_HOST_BUF_USE_RESV_ACL_MEM)
			currentQueue->reservedBytes -= length;
		else
			currentQueue->allocatedBytes -= length;
		//pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI_ALLOC: releasing %d length on queue %p with %d bytes left allocated\n", length, currentQueue, currentQueue->allocatedBytes));  
		
    }
noqueue:  
	host_buf_free(p_buf);
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"prh_hci_ReleaseWriteBuffer\n"));
	return BT_NOERROR;
}


t_api _prh_hci_ReleaseWriteBuffer_nolock(struct host_buf *p_buf)
{
	int ret;
	u_int16 handle;
	struct prh_st_t_hci_queue *currentQueue;
	
	handle=p_buf->hci_handle_flags & 0x0FFF;
	
	if (p_buf->flags & PRH_HOST_BUF_OUTGOING_PACKET)
    {
		currentQueue = prh_hci_fc_find_data_queue(prh_hci_acl_queue_list_head, handle);
		if (!currentQueue)
		{
			pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_ALLOC: cannot find queue for handle %d\n", handle));
			return BT_INVALIDPARAM;
		}
    }
	else
		currentQueue=NULL;
	
	ret=_prh_hci_ReleaseWriteBuffer(currentQueue, p_buf);
	return ret;
	
}

t_api _prh_hci_ReleaseCommandBuffer_nolock(struct host_buf *p_buf)
{
	int ret;
	ret=_prh_hci_ReleaseWriteBuffer(&prh_hci_fc_command_queue, p_buf);
	return ret;
	
}

t_api prh_hci_FreeWriteBuffer(struct host_buf *p_buf)
{
	int ret;
	pMutexLock(prh_hci_fc_acl_queue_lock);  
	ret=_prh_hci_ReleaseWriteBuffer_nolock(p_buf);
	pMutexUnlock(prh_hci_fc_acl_queue_lock);
	return ret;
}

t_api prh_hci_ReleaseCommandBuffer(struct host_buf *p_buf)
{
	int ret;
	pMutexLock(prh_hci_fc_command_queue_lock);  
	ret=_prh_hci_ReleaseCommandBuffer_nolock(p_buf);
	pMutexUnlock(prh_hci_fc_command_queue_lock);
	return ret;
}


t_api HCI_Transport_Free_Buffer(struct host_buf *p_buf, u_int8 type)
{
    extern volatile u_int8 prh_hci_running;
    if (prh_hci_running) {
	    int ret=0;
	    if (type==HCI_pduCOMMAND)
		    ret=_prh_hci_ReleaseCommandBuffer_nolock(p_buf);
	    else if (type==HCI_pduSCODATA)
            host_buf_free(p_buf);
        else
		    ret=_prh_hci_ReleaseWriteBuffer_nolock(p_buf);
	    return ret;
    } else {
        host_buf_free(p_buf);
        return BT_NOERROR;
    }
}
