/*
 * MODULE NAME:    hci_flow_ctrl.c
 * DESCRIPTION:    HCI Flow Control Implementation (Host to Host Controller) 
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_flow_ctrl.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

/*
    Flow control uses a linked list of handles each node being a linked list of pdus.  
    Each toplevel list is a list of type prh_st_t_hci_queue. The entries in these 
    lists are themselves lists of host_buf structures - one per handle (plus the
    broadcast handle) A single linked list queues command pdus. Each list is 
    protected by a semaphore. Each queue maintains the number of entries from that
    queue which are buffered in the HC, and also the number of entries that may
    be sent before filling the HC buffers. The latter counter is managed on a 'fair'
    allocation of HC buffer space by the UpdateQueue functions. All queues are 
    scanned periodically by Host_Execute(), and pdus are sent subject to flow control.
*/


#include "host_config.h"
#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"
#include "hci_constants.h"
#include "papi.h"
#include "hci_flow_ctrl.h"
#include "hci_primitives.h"
#include "host_ctrl.h"

#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_hcit.h"
#else
#include "hci_transport.h"
#endif

//extern u_int8 bcsp_recv_correct;
//extern BOOL use_bcsp;

struct prh_st_t_hci_queue prh_hci_fc_command_queue;


t_api prh_hci_fc_update_acl_queues(void);
u_int32 prh_hci_fc_acl_slots_empty_in_hc;
u_int32 prh_hci_fc_acl_slots_unearmarked_in_hc;
u_int16 prh_hci_fc_num_active_acl_handles;
u_int16 prh_hci_fc_total_acl_slots_in_hc=0;
struct prh_st_t_hci_queue *prh_hci_acl_queue_list_head;
struct prh_st_t_hci_queue *prh_hci_fc_next_acl_tx_queue, *prh_hci_fc_next_acl_update_queue;
u_int32 prh_hci_fc_total_queued_acl_pdus;
t_pMutex prh_hci_fc_acl_queue_lock;
u_int32 prh_hci_fc_total_queued_acl_bytes;
t_pTimer prh_hci_report_host_completed_packets_timerhandle;
u_int8 prh_hci_fc_hc_to_host_fc_enabled_acl;

u_int8 prh_hci_reset_in_progress;
t_pMutex prh_hci_fc_command_queue_lock;
u_int16 prh_active_broadcast_handle, prh_piconet_broadcast_handle;



void prh_hci_fc_free_queue(struct prh_st_t_hci_queue *queue);
void prh_hci_release_handle_queue(struct prh_st_t_hci_queue *head, t_pMutex lock);


void prh_hci_report_host_completed_packets_timer(void *dummy);
void prh_hci_report_host_completed_packets(void);


/*************************************************************************************************
 * prh_hci_fc_init
 *
 * Start flow control - initialise all file-scope variables and queues, create locking semaphores.
 ************************************************************************************************/
t_api prh_hci_fc_init(void) {

    prh_hci_acl_queue_list_head=NULL;   
    prh_hci_fc_next_acl_tx_queue=NULL;
    prh_hci_fc_next_acl_update_queue=NULL;
    prh_hci_fc_num_active_acl_handles=0;
    prh_hci_fc_total_queued_acl_pdus=0;
    prh_hci_fc_acl_queue_lock=pMutexCreate(0);

    prh_hci_fc_total_queued_acl_bytes=0;

    prh_hci_fc_command_queue.numCanSend=1;
    prh_hci_fc_command_queue.allocatedBytes=0;
    prh_hci_fc_command_queue.reservedBytes=0;
    prh_hci_fc_command_queue_lock=pMutexCreate(0);
    prh_hci_reset_in_progress=0;

    prh_hci_fc_hc_to_host_fc_enabled_acl=0;

    /* current values of the broadcast handles */
    prh_active_broadcast_handle=0xEF1;
    prh_piconet_broadcast_handle=0xEF2;

    /* at bootup, prh_hci_fc_total_acl_slots_in_hc is zero, but if we read the buffer size it will be set correctly */
	prh_hci_fc_acl_slots_empty_in_hc=prh_hci_fc_total_acl_slots_in_hc;
	prh_hci_fc_acl_slots_unearmarked_in_hc=prh_hci_fc_total_acl_slots_in_hc;

#if PRH_HOST_HC_TO_HOST_FLOW_CONTROL_REPORTING_FREQUENCY>0
    prh_hci_report_host_completed_packets_timerhandle=pTimerCreate(PRH_HOST_HC_TO_HOST_FLOW_CONTROL_REPORTING_FREQUENCY,prh_hci_report_host_completed_packets_timer,NULL,pTIMER_ONESHOT);
#else
    prh_hci_report_host_completed_packets_timerhandle=0;
#endif

    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI: Flow control initialisation complete\n"));

    return BT_NOERROR;
} 


/*************************************************************************************************
 * prh_hci_fc_shutdown
 *
 * Stop flow control - Free all queued pdus and the queues themselves.
 ************************************************************************************************/
t_api prh_hci_fc_shutdown(void) {

if(prh_hci_report_host_completed_packets_timerhandle)
{
    pTimerCancel(prh_hci_report_host_completed_packets_timerhandle);
	prh_hci_report_host_completed_packets_timerhandle = 0;
}
    prh_hci_release_handle_queue(prh_hci_acl_queue_list_head,prh_hci_fc_acl_queue_lock);
    pMutexLock(prh_hci_fc_command_queue_lock);
    prh_hci_fc_free_queue(&prh_hci_fc_command_queue);
    pMutexUnlock(prh_hci_fc_command_queue_lock);
    pMutexFree(prh_hci_fc_command_queue_lock);

    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI: Flow control shutdown complete\n"));

    return BT_NOERROR;
} 

/*************************************************************************************************
 * prh_hci_release_handle_queue
 *
 * As part of the shutdown proecdure, release all handles on a queue (acl or sco queue)
 ************************************************************************************************/
void prh_hci_release_handle_queue(struct prh_st_t_hci_queue *head, t_pMutex lock) {

    struct prh_st_t_hci_queue *currentQueue, *nextQueue;

    pMutexLock(lock);
    currentQueue=head;
    nextQueue=currentQueue;
    
    while (currentQueue) {
        nextQueue=currentQueue->next;
        prh_hci_fc_free_queue(currentQueue);
        pFree(currentQueue);
        currentQueue=nextQueue;
    }
    pMutexUnlock(lock);
    pMutexFree(lock);

}


/*************************************************************************************************
 * prh_hci_fc_find_data_queue
 *
 * Search a toplevel queue for the hostbuf list representing a particular handle. 
 * Mutex locking is the responsibility of the calling function.
 ************************************************************************************************/
struct prh_st_t_hci_queue *prh_hci_fc_find_data_queue(struct prh_st_t_hci_queue *queueListHead, u_int16 handle_t)
{

    struct prh_st_t_hci_queue *currentQueue;

    currentQueue=queueListHead;
    while (currentQueue)
    {            
        if (currentQueue->handle==handle_t) 
        {
            return currentQueue;
        }
        currentQueue=currentQueue->next;
    }
    return NULL;
}


/*************************************************************************************************
 * prh_hci_fc_enqueue_pdu
 *
 * Add a new host_buf structure to a queue. This performs low-level queue addition after
 * all lookups and checks have been completed. Mutex locking is performed by the caller.
 ************************************************************************************************/
void prh_hci_fc_enqueue_pdu(struct prh_st_t_hci_queue *targetQueue, struct host_buf *pdu) {
    //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI: enqueuing 0x%02X PDU bytes (footprint 0x%02X bytes),targetQueue->tail=%x\n", pdu->len,pdu->totalsize+sizeof(struct host_buf),targetQueue->tail));
    pdu->next=NULL;
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"targetQueue->tail=0x%x,targetQueue->head=0x%x",targetQueue->tail,targetQueue->head));
    if (targetQueue->tail) {
        targetQueue->tail->next=pdu;
        targetQueue->tail=pdu;
    } else {
        /* 
            The queue was empty, so kick off the scheduler to send this pdu.
            The outer mutex locking means the scheduler does not actually run immediately
        */
        targetQueue->head=pdu;
        targetQueue->tail=pdu;
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI: Enqueuing to an empty queue, so kicking off the scheduler\n"));
        prh_system_scheduler_execute();
    }
    ///targetQueue->tail=pdu;
}
/*************************************************************************************************
 * prh_hci_fc_send_command_pdu
 *
 * Adds a command pdu to the command queue. After queuing bounds are checked, the commandqueue 
 * is locked and prh_hci_fc_enqueue_pdu used to add the pdu to the queue. The queue is then unlocked.
 * Any combination of queue size limits may be defined from host_config.h so all must be checked.
 ************************************************************************************************/
t_api prh_hci_fc_send_command_pdu(struct host_buf *pdu) {

    //pDebugPrintfEX((pLOGERROR,pLOGHCI,"prh_hci_fc_send_command_pdu numcansend:%d\n", prh_hci_fc_command_queue.numCanSend));

    pMutexLock(prh_hci_fc_command_queue_lock);

#if 0
    if(prh_hci_fc_command_queue.numCanSend == 0)
    {
          struct host_buf *wake_pdu;
          prh_hci_GetCommandBuffer(4, 0, &wake_pdu);
         wake_pdu->len= 4;

         /* write opcode field */
         wake_pdu->data[0]=0xc1;
         wake_pdu->data[1]=0xfc;
         wake_pdu->data[2]=0;
         wake_pdu->data[3]=0;
         prh_hci_fc_enqueue_pdu(&prh_hci_fc_command_queue,wake_pdu);
    }
#endif
    
    if (BT_READ_LE_UINT16(pdu->data)==HCI_cmdHOSTNUMBEROFCOMPLETEDPACKETS) 
    {
        /* HostNumCompletedPackets commands should not be subject to flow control  */
#if HCI_HWRESET_HANDLING
        if (prh_hci_reset_in_progress) {
            /* All data will be flushed, so no need to send this command */
            pMutexUnlock(prh_hci_fc_command_queue_lock);
            return BT_NOERROR; 
        }
#endif
        /* pdu must be placed at the head of the queue - not the end */
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI: enqueuing 0x%02X PDU bytes (footprint 0x%02X bytes) of nost_num_completed_packets command\n", pdu->len,pdu->totalsize+sizeof(struct host_buf)));

        pdu->next=prh_hci_fc_command_queue.head;
        if (!prh_hci_fc_command_queue.tail)
            prh_hci_fc_command_queue.tail=pdu;
        prh_hci_fc_command_queue.head=pdu;    
        prh_hci_fc_command_queue.numCanSend++;    
        
        if (!pdu->next) 
        {
            /* 
                The queue was empty, so kick off the scheduler to send this pdu.
                The outer mutex locking means the scheduler does not actually run immediately
            */
            //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI: Enqueuing nost_num_completed_packets command to an empty queue, so kicking off the scheduler\n"));
            prh_system_scheduler_execute();
        }
    } 
    else 
    {
        prh_hci_fc_enqueue_pdu(&prh_hci_fc_command_queue,pdu);    
    }
        
    pMutexUnlock(prh_hci_fc_command_queue_lock);

    return BT_NOERROR;
}

/*************************************************************************************************
 * prh_hci_fc_send_data_pdu
 *
 * Adds a SCO or ACL pdu to a per-handle queue. After the queue is located the queue is locked and 
 * prh_hci_fc_enqueue_pdu used to add the pdu to the queue. The queue is then unlocked.
 ************************************************************************************************/
t_api prh_hci_fc_send_data_pdu(u_int8 type, u_int16 handle, struct host_buf *pdu)
{
    
    struct prh_st_t_hci_queue *targetQueue;

    switch (type)
    {
 
        case HCI_pduSCODATA:
            {
                /* Initial SCO pdu support -MM */
                t_api ret;
                pdu->flags |= PRH_HOST_BUF_HCI_HEADER_PRESENT;
                pMutexLock(prh_hci_fc_acl_queue_lock);
                ret=HCI_Transport_PDU_Tx(pdu, type);
                pMutexUnlock(prh_hci_fc_acl_queue_lock);
                return ret;
            }    
            
            /*
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: SCO pdus unsupported - discarding\n"));
            host_buf_free(pdu); ///????
            */
            break;


        case HCI_pduACLDATA:

             /* update broadcast pdus with the current broadcast handle */
            if (pdu->data[1]&0x40) { 
                BT_WRITE_LE_UINT16(pdu->data,prh_active_broadcast_handle|((pdu->data[1]&0xF0)<<8));
                handle=prh_active_broadcast_handle;
            }
            if (pdu->data[1]&0x80) {
                BT_WRITE_LE_UINT16(pdu->data,prh_piconet_broadcast_handle|((pdu->data[1]&0xF0)<<8));
                handle=prh_piconet_broadcast_handle;
            }

            pMutexLock(prh_hci_fc_acl_queue_lock);
            targetQueue=prh_hci_fc_find_data_queue(prh_hci_acl_queue_list_head, handle);

            if (!targetQueue && pdu->data[1]&0xc0) {
                /*                
                    buffer is broadcast data, so assign a queue for this handle 
                    this handle will be used for all further broadcast traffic 
                    this only happens the first time each type of broadcast pdu is sent 
                */
                //pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: assigning queue for broadcast ACL handle 0x%02X\n", handle));
    	        targetQueue=prh_hci_create_queue(handle,HCI_linkACL);
            }

            if (!targetQueue) {
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: cannot queue outgoing PDU for unknown handle 0x%02X - discarding\n",handle));
                pMutexUnlock(prh_hci_fc_acl_queue_lock);
                prh_hci_FreeWriteBuffer(pdu);///????
                return BT_INVALIDPARAM;
            } 
            else 
            {
                pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"prh_hci_fc_send_data_pdu total=%d\n",prh_hci_fc_total_queued_acl_pdus));
                prh_hci_fc_total_queued_acl_pdus++;
                prh_hci_fc_enqueue_pdu(targetQueue,pdu);   /* this will run the scheduler when complete */
                pMutexUnlock(prh_hci_fc_acl_queue_lock);
                return BT_NOERROR;
            }
            break;


        default:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: cannot queue unknown outgoing PDU of type 0x%02X - discarding\n", type));
            prh_hci_FreeWriteBuffer(pdu);  ///????
            return BT_INVALIDPARAM;
    }

    /* NOTREACHED */
    return BT_UNKNOWNERROR;
}


/*************************************************************************************************
 * prh_hci_fc_num_completed_packets
 *
 * Called when a flow control update is recieved from the HC. Each queue is locked in turn and
 * the per-handle counters updated. An iteration of the scheduler is then run. 
 ************************************************************************************************/
void prh_hci_fc_num_completed_packets(u_int8 *pdu)
{  
    int i; 
    u_int16 handle;
    struct prh_st_t_hci_queue *targetQueue;
    u_int8 numHandles;
    u_int16 numCompletedPkts;

    pMutexLock(prh_hci_fc_acl_queue_lock);

    numHandles = pdu[2];
    for (i=0; i<numHandles; i++)
    {
        handle=BT_READ_LE_UINT16(pdu+3+(i<<2));
        targetQueue=prh_hci_fc_find_data_queue(prh_hci_acl_queue_list_head,handle);
        if (targetQueue) 
        {
            numCompletedPkts = BT_READ_LE_UINT16(pdu+5+(i<<2));
            targetQueue->packetsInHC-=numCompletedPkts;

            //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: updating flow control for handle 0x%02X, 0x%02X chunks completed, 0x%02X left in HC,\n", handle, numCompletedPkts,targetQueue->packetsInHC));

            if (targetQueue->linkType==HCI_linkACL) {
              prh_hci_fc_acl_slots_empty_in_hc+=numCompletedPkts;
              prh_hci_fc_acl_slots_unearmarked_in_hc+=numCompletedPkts;
            } 
           
        } 
        else 
        {
            pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: recieved flow control information for inactive handle 0x%02X\n",handle));
        }
    }
    //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: total free acl chunks free in hc=0x%02X\n", prh_hci_fc_acl_slots_empty_in_hc));

    /* recompute what may be sent on each handle. This function will run the scheduler when complete */
    prh_hci_fc_update_acl_queues();   

    pMutexUnlock(prh_hci_fc_acl_queue_lock);

}


/*************************************************************************************************
 * prh_hci_fc_read_buf_size_event
 *
 * Called during initialisation when the HC buffer sizes are recieved from the HC (in response
 * to a HCI_ReadBufferSize command. These values are recorded for use in allocating HC buffer space.
 ************************************************************************************************/
void prh_hci_fc_read_buf_size_event(u_int16 maxACLs, u_int16 maxSCOs) 
{

  if (!prh_hci_acl_queue_list_head) 
  {
        /* no data is currently queued */
        //pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: got read_buffer_size, maxNumACLs = 0x%02X, maxNumSCOs = 0x%02X\n",maxACLs,maxSCOs));
    
        pMutexLock(prh_hci_fc_acl_queue_lock);
        prh_hci_fc_total_acl_slots_in_hc=maxACLs;
        prh_hci_fc_acl_slots_empty_in_hc=maxACLs;
        prh_hci_fc_acl_slots_unearmarked_in_hc=maxACLs;
        prh_hci_fc_rebalance_queues();
        pMutexUnlock(prh_hci_fc_acl_queue_lock);
    }
}

/*************************************************************************************************
 * prh_hci_fc_disconnection_complete
 *
 * Called when a link is disconected. The contents of the queue are released, and the 
 * queue information is rebalanced.
 ************************************************************************************************/

void prh_hci_fc_disconnection_complete(u_int16 handle) { 
    struct prh_st_t_hci_queue *previousQueue, *currentQueue;

    pMutexLock(prh_hci_fc_acl_queue_lock);

    previousQueue=currentQueue=prh_hci_acl_queue_list_head;
    while (currentQueue) 
    {            
        if (currentQueue->handle==handle) {
            break;
        }
        previousQueue=currentQueue;
        currentQueue=currentQueue->next;
    }
    if (!currentQueue) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI FC: Got disconnection complete for unknown (or sco) handle 0x%02X - ignoring\n",handle));
        pMutexUnlock(prh_hci_fc_acl_queue_lock);
        return;
    }

    if (currentQueue==prh_hci_fc_next_acl_update_queue)
        prh_hci_fc_next_acl_update_queue=currentQueue->next;
    if (currentQueue==prh_hci_fc_next_acl_tx_queue)
        prh_hci_fc_next_acl_tx_queue=currentQueue->next;


    if (currentQueue->active)
        prh_hci_fc_num_active_acl_handles--;

    prh_hci_fc_acl_slots_unearmarked_in_hc+=currentQueue->numCanSend+ currentQueue->packetsInHC;
    prh_hci_fc_acl_slots_empty_in_hc+=currentQueue->packetsInHC;

    /* free all queued pdus */
    prh_hci_fc_free_queue(currentQueue);

    /* remove the queue from the linked list and free it */
    if (currentQueue==prh_hci_acl_queue_list_head) 
    {
        prh_hci_acl_queue_list_head=currentQueue->next;
    } 
    else 
    {
        previousQueue->next=currentQueue->next;
    }

    pFree(currentQueue);

    prh_hci_fc_update_acl_queues();
    pMutexUnlock(prh_hci_fc_acl_queue_lock);
}



/*************************************************************************************************
 * prh_hci_fc_connection_complete
 *
 * Called when a link is created. 
 ************************************************************************************************/
void prh_hci_fc_connection_complete(u_int8 linkType, u_int16 handle)
{ 
 
    pMutexLock(prh_hci_fc_acl_queue_lock);
    if (linkType==HCI_linkACL)
      prh_hci_create_queue(handle, linkType);
    pMutexUnlock(prh_hci_fc_acl_queue_lock);
}

/*************************************************************************************************
 * prh_hci_fc_loopback_command
 *
 * Called to update the command flow control state when in loopback mode
 ************************************************************************************************/
void prh_hci_fc_loopback_command(void) 
{ 
    
    pMutexLock(prh_hci_fc_command_queue_lock);
    prh_hci_fc_command_queue.numCanSend++;
    prh_system_scheduler_execute();
    pMutexUnlock(prh_hci_fc_command_queue_lock);
}

/*************************************************************************************************
 * prh_hci_fc_num_command_packets
 *
 * Called to update the command flow control state (from commandcomplete and commandstatus)
 ************************************************************************************************/
void prh_hci_fc_num_command_packets(u_int8 numCommandPackets) 
{ 
    
    pMutexLock(prh_hci_fc_command_queue_lock);
    prh_hci_fc_command_queue.numCanSend = numCommandPackets;
    prh_system_scheduler_execute();
    pMutexUnlock(prh_hci_fc_command_queue_lock);
}

/*************************************************************************************************
 * prh_hci_create_queue
 *
 * Allocate and initialise a new queue structure for a new handle. Locking is the responsibility
 * of the caller.
 ************************************************************************************************/
struct prh_st_t_hci_queue *prh_hci_create_queue(u_int16 handle, u_int8 type) {
    struct prh_st_t_hci_queue *newQueue;

    newQueue=pMalloc(sizeof(struct prh_st_t_hci_queue));
    if (newQueue==NULL)
	return NULL;
    pMemset(newQueue,0,sizeof(struct prh_st_t_hci_queue)); /* zero all counters */

    newQueue->handle=handle;
    newQueue->linkType=type;
    newQueue->active=TRUE;

    if (type==HCI_linkACL) {
        prh_hci_fc_num_active_acl_handles++;
        newQueue->next=prh_hci_acl_queue_list_head;
        prh_hci_acl_queue_list_head=newQueue;
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: Queue for handle 0x%02X created",handle));
    } else
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI FC: Queue create for unknown handle type 0x%02X",type));

    prh_hci_fc_rebalance_queues();   
    return newQueue;
}

/*************************************************************************************************
 * prh_hci_fc_free_queue
 *
 * Empries a queue, freeing all data. Mutex locking is the responsibility of the caller.
 ************************************************************************************************/
void prh_hci_fc_free_queue(struct prh_st_t_hci_queue *queue) { 
 
    struct host_buf *current, *next;

    current=queue->head;
    next=current;

    while (current) {
        next=current->next;
        host_buf_free(current); //////????
        current=next;
	 if(prh_hci_fc_total_queued_acl_pdus)	
	       prh_hci_fc_total_queued_acl_pdus--; 	
    };
    queue->allocatedBytes=0;
    queue->reservedBytes=0;
    queue->head=queue->tail=0;
}

/*************************************************************************************************
 * prh_hci_fc_scan_command_queue
 *
 * Scans the command queue state and sends pdus to the HC, if possible.
 ************************************************************************************************/
t_api prh_hci_fc_scan_command_queue(u_int32 *numToSend) {

    struct host_buf *pduToSend;
    t_api status;
    u_int16 pduBytes;

    pMutexLock(prh_hci_fc_command_queue_lock);

    if (!prh_hci_fc_command_queue.head) {
        /* no commands are queued */
        pMutexUnlock(prh_hci_fc_command_queue_lock);
        return BT_NOERROR;
    }
    
    //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"prh_hci_fc_scan_acl_queues *numToSend=%d\n",(*numToSend)));
    
    while (*numToSend) {    
        
        if (!(prh_hci_fc_command_queue.head && prh_hci_fc_command_queue.numCanSend)) {
            break;
        }

        pduToSend=prh_hci_fc_command_queue.head;
        prh_hci_fc_command_queue.head=prh_hci_fc_command_queue.head->next;
        
        if (!prh_hci_fc_command_queue.head)
            prh_hci_fc_command_queue.tail=NULL;
        
        if (BT_READ_LE_UINT16(pduToSend->data)==HCI_cmdRESET) {
            /* Reset command requires that no other commands are sent before the CommandComplete is returned */
            prh_hci_reset_in_progress=1; 
        }

        pduBytes=pduToSend->totalsize+sizeof(struct host_buf);
        status = HCI_Transport_PDU_Tx(pduToSend,HCI_pduCOMMAND);        
        if (status) {
            //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"Error case scheduler execute by prh_hci_fc_scan_command_queue\n"));
            prh_system_scheduler_execute(); /* attempt to rerun the scheduler */
            pMutexUnlock(prh_hci_fc_command_queue_lock);
            return status;            
        } else {
            prh_hci_fc_command_queue.numCanSend--;
            (*numToSend)--;
        }
        if (prh_hci_reset_in_progress) {
            /* Reset command requires that no other commands are sent before CommandComplete */
            prh_hci_fc_command_queue.numCanSend=0;
        }
        
    }   

    if (prh_hci_fc_command_queue.head && prh_hci_fc_command_queue.numCanSend)
        prh_system_scheduler_execute();

    pMutexUnlock(prh_hci_fc_command_queue_lock);

    //if (prh_hci_fc_command_queue.head && prh_hci_fc_command_queue.numCanSend)
    //    return BT_RETRY; 
    //else
        return BT_NOERROR; 
}


/*************************************************************************************************
 * prh_hci_fc_scan_acl_queues
 *
 * Scans the ACL queues in a round-robin manner and sends pdus to the HC, if possible.
 ************************************************************************************************/
t_api prh_hci_fc_scan_acl_queues(u_int32 *maxNumToProcess) { 
    struct host_buf *newQueueHead;
    struct prh_st_t_hci_queue *origin, *current;
    t_api status=BT_NOERROR;


    pMutexLock(prh_hci_fc_acl_queue_lock);

    if (prh_hci_fc_num_active_acl_handles==0) {
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"prh_hci_fc_scan_acl_queues called and no handles open\n"));  
        pMutexUnlock(prh_hci_fc_acl_queue_lock);
        return BT_NOERROR;
    }


    if (!prh_hci_fc_total_queued_acl_pdus) {
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"prh_hci_fc_scan_acl_queues called and no queued pdus\n"));
        pMutexUnlock(prh_hci_fc_acl_queue_lock);
        return BT_NOERROR;
    }       

    //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"prh_hci_fc_scan_acl_queues called and processing - %d PDUs queued\n",prh_hci_fc_total_queued_acl_pdus));



    origin=prh_hci_fc_next_acl_tx_queue;
    if (!origin)
        origin=prh_hci_acl_queue_list_head;

    current=origin;

    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"numCanSend=%d,head=0x%x,active=%d, maxNumToProcess=%d, total_queued_acl_pdus=%d\n",current->numCanSend,current->head, current->active,*maxNumToProcess, prh_hci_fc_total_queued_acl_pdus));

    while (*maxNumToProcess && prh_hci_fc_total_queued_acl_pdus) {

        if (current->numCanSend && current->head && current->active) {

            newQueueHead=current->head->next;

            status = HCI_Transport_PDU_Tx(current->head,HCI_pduACLDATA);
            current->head=newQueueHead;    
            if (!newQueueHead)
                current->tail=NULL;

            current->numCanSend--;
            current->packetsInHC++;
            (*maxNumToProcess)--;
            prh_hci_fc_acl_slots_empty_in_hc--;
            prh_hci_fc_total_queued_acl_pdus--;

			if((!current->numCanSend) && prh_hci_fc_acl_slots_empty_in_hc && current->head)
			{
				prh_hci_fc_rebalance_queues();
			}
            pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"prh_hci_fc_acl_slots_empty_in_hc=0x%02X, handle:0x%02X ,numCanSend=0x%02X, packetsInHC=0x%02X\n",prh_hci_fc_acl_slots_empty_in_hc, current->handle,current->numCanSend,current->packetsInHC));
        }

			

        current=current->next;
        if (!current) 
            current=prh_hci_acl_queue_list_head;
        
        if (current==origin)
            break;

        if (status) {
            pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"Error from HCI transport during acl pdu tx, error=0x%02X\n",status));
            prh_system_scheduler_execute();
            pMutexUnlock(prh_hci_fc_acl_queue_lock);
            return status;
        }

    };

    prh_hci_fc_next_acl_tx_queue=current;

    //if (prh_hci_fc_total_queued_acl_pdus && prh_hci_fc_acl_slots_empty_in_hc)
    //    status=BT_RETRY; 
    //else
        status=BT_NOERROR; 

    pMutexUnlock(prh_hci_fc_acl_queue_lock);

    return status; 
}


/*************************************************************************************************
 * prh_hci_fc_process_outgoing
 *
 * Processes (i.e. sends to the HC) a given number of pdus from the queues in a fair manner.
 ************************************************************************************************/

t_api prh_hci_fc_process_outgoing(u_int32 *numToProcess) { 
    
    t_api status1, status2;

    //add the bcsp resend code here 
    //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"prh_hci_fc_process_outgoing *numToProcess=%d\n",(*numToProcess)));

    if (!(*numToProcess)) 
    {
        if (prh_hci_fc_command_queue.head || prh_hci_fc_total_queued_acl_bytes) { 
            //pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"we can't do anything, but there is data which must be dequeued\n"));
            /* we can't do anything, but there is data which must be dequeued */
            return BT_RETRY;
        }
        return BT_NOERROR;
    }


   // if((use_bcsp==TRUE)&&(bcsp_recv_correct==0))
   //     return BT_NOERROR;
   //rdabt_pageSpaySetup(5, 2, (UINT32)&prh_hci_fc_command_queue , (UINT32)&prh_hci_fc_command_queue+4);
   //rdabt_pageSpayEnable(5);
    status1 = prh_hci_fc_scan_command_queue(numToProcess);
   //rdabt_pageSpayDisable(5);
    if (status1 && status1!=BT_RETRY)
        return status1;
    //if (status1==BT_RETRY)
       //pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: there is command data which must be dequeued\n"));

    status2 = prh_hci_fc_scan_acl_queues(numToProcess);
    if (status2 && status2!=BT_RETRY)
        return status2;
   // if (status2==BT_RETRY)
       // pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: there is acl data which must be dequeued\n"));

    if (status1==BT_RETRY || status2==BT_RETRY)
        return BT_RETRY;
    else
        return BT_NOERROR; 

}

/*************************************************************************************************
 * prh_hci_fc_enqueue_child_list
 *
 * Enqueue a group of pdus described in a parent/child host_buf fashion. All data from L2CAP is
 * handled in this way, and represents a single segmented L2CAP MTU.
 ************************************************************************************************/
t_api prh_hci_fc_enqueue_child_list(u_int16 handle, struct host_buf *bufferParent, u_int32 totalBytes) { 

    struct host_buf *current;
    u_int16 numFragments;
    struct prh_st_t_hci_queue *targetQueue;

#if pDEBUG
    u_int32 totalListSize;
#endif


    pMutexLock(prh_hci_fc_acl_queue_lock);

     /* update broadcast pdus with the current broadcast handle */
    if (bufferParent->hci_handle_flags & 0x4000) {
      bufferParent->hci_handle_flags = prh_active_broadcast_handle | (bufferParent->hci_handle_flags & 0xF000);
      handle=prh_active_broadcast_handle;
    }
    if (bufferParent->hci_handle_flags&0x8000) {
      bufferParent->hci_handle_flags = prh_piconet_broadcast_handle | (bufferParent->hci_handle_flags & 0xF000);
      handle=prh_piconet_broadcast_handle;
    }

    targetQueue=prh_hci_fc_find_data_queue(prh_hci_acl_queue_list_head,handle);
    if (!targetQueue) {
      if (bufferParent->hci_handle_flags&0xc000) {
            /* buffer is broadcast data, so assign a queue for this handle */
            /* this handle will be used for all further broadcast traffic */
            //pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: assigning queue for broadcast ACL handle 0x%02X\n", handle));
    	    targetQueue=prh_hci_create_queue(handle,HCI_linkACL);
        } else {
            //pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: cannot find queue for ACL handle 0x%02X\n", handle));
            pMutexUnlock(prh_hci_fc_acl_queue_lock);
            return BT_FLOWCTRL;
        }
    }

    if (targetQueue->tail) {
        targetQueue->tail->next=bufferParent;
    } else {
        targetQueue->head=bufferParent;
    }

    current=bufferParent;
    if (current->child_count>1)
      current=bufferParent->next;
    
    numFragments=0;

#if pDEBUG
    totalListSize=0;
#endif
    do {
#if pDEBUG
      totalListSize += current->totalsize + sizeof (struct host_buf);
#endif
      
      /* update broadcast pdus with the current broadcast handle */
      if (current->hci_handle_flags & 0x4000) {
	    current->hci_handle_flags = prh_active_broadcast_handle | (current->hci_handle_flags & 0xF000);
	    handle=prh_active_broadcast_handle;
      }
      if (current->hci_handle_flags & 0x8000) {
	    current->hci_handle_flags = prh_piconet_broadcast_handle | (current->hci_handle_flags & 0xF000);
	    handle=prh_piconet_broadcast_handle;
      }
      
      numFragments++;
      if (!(current->next))
	    targetQueue->tail=current;
    } while ((current=current->next));
    

#if pDEBUG
    if (totalListSize!=totalBytes) {
      pDebugPrintfEX((pLOGERROR,pLOGHCI,"prh_hci_fc_send_acl_list(): L2CAP SAR error!!! - totalBytes argument is incorrect tList: %d tBytes: %d\n", totalListSize, totalBytes));
      
    }
#endif

    prh_hci_fc_total_queued_acl_pdus+=numFragments;

    /* We don't use EnqueuePDU() so do this manually */
    prh_system_scheduler_execute();

    pMutexUnlock(prh_hci_fc_acl_queue_lock);

    return BT_NOERROR; 
    
}
 


/*************************************************************************************************
 * prh_hci_fc_modechange
 *
 * called when a modechange event occurs to enable/disable sending data on a handle. 
 ************************************************************************************************/
void prh_hci_fc_modechange(u_int16 handle, u_int8 mode) {
    struct prh_st_t_hci_queue *targetQueue;

    pMutexLock(prh_hci_fc_acl_queue_lock);

    targetQueue=prh_hci_fc_find_data_queue(prh_hci_acl_queue_list_head,handle);
    if (targetQueue) {
        if (mode==0 || mode==0x02) { /* active or sniff */
            if (!targetQueue->active) {
                targetQueue->active=1;
                prh_hci_fc_num_active_acl_handles++;
            }
        } else {
            if (targetQueue->active) {
                targetQueue->active=0;
                prh_hci_fc_num_active_acl_handles--;
            }
        }
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"prh_hci_fc_modechange(): handle 0x%02X %s",handle,(mode==0x00||mode==0x02)?"active":"inactive"));
        if (targetQueue->active)
            prh_hci_fc_rebalance_queues();
    } else {
        //pDebugPrintfEX((pLOGERROR,pLOGHCI,"prh_hci_fc_modechange(): unknown handle 0x%02X",handle));
    }        
    pMutexUnlock(prh_hci_fc_acl_queue_lock);
}

/*************************************************************************************************
 * prh_hci_fc_update_acl_queues
 *
 * Rebalances the available HC buffer space between the open ACL handles. 
 * Queue mutex locking is the responsibility of the caller.
 ************************************************************************************************/
t_api prh_hci_fc_update_acl_queues(void) {

    struct prh_st_t_hci_queue *current, *origin;
    u_int16 fairSharePerHandlePerHandle, updateAmount; 
    u_int8 iteration=0;

#if pDEBUG
    struct prh_st_t_hci_queue *old_origin;
#endif

    if (prh_hci_fc_num_active_acl_handles==0) { /* must check this or we will loop forever trying to distribute the chunks */
        return BT_NOERROR; 
    }

    if (prh_hci_fc_acl_slots_unearmarked_in_hc==0) { /* nothing to distribute (why were we called?) */
        pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: prh_hci_fc_update_acl_queues called when no slots to distribute\n"));
        return BT_NOERROR;
    }

    fairSharePerHandlePerHandle = prh_hci_fc_total_acl_slots_in_hc / prh_hci_fc_num_active_acl_handles; 
    if (fairSharePerHandlePerHandle<1) {
        fairSharePerHandlePerHandle=1; /* we know there is always at least one slot to distribute becuase of check at entry */
   }

    //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: Updating queues, 0x%02X handles open, 0x%02X slots free in HC, 0x%02X slots to distribute",
                                                            //prh_hci_fc_num_active_acl_handles,prh_hci_fc_acl_slots_empty_in_hc,prh_hci_fc_acl_slots_unearmarked_in_hc));
    
#if pDEBUG
    origin=prh_hci_fc_next_acl_update_queue;
    if (!prh_hci_fc_next_acl_update_queue)
        origin=prh_hci_acl_queue_list_head;
    current=origin;
    old_origin=origin;
    do { /* do -> while is ok here since prh_hci_fc_num_active_acl_handles>0 */
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: Updating queues: record at 0x%p, handle=0x%02X, active=%d, packetsInHC=0x%02X, numCanSend=0x%02X, next record at 0x%p\n",
        //    current,current->handle, current->active, current->packetsInHC, current->numCanSend, current->next));
        
        current=current->next;
        if (!current) { /* wrap to start of list & keep going */
            current=prh_hci_acl_queue_list_head;
        }
    } while (current!=origin);
#endif


    origin=prh_hci_fc_next_acl_update_queue;
    if (!prh_hci_fc_next_acl_update_queue)
        origin=prh_hci_acl_queue_list_head;

    current=origin;
 

#define MIN_VAL(a,b)  (((a)<(b))?(a):(b))


    do { /* do -> while is ok here since prh_hci_fc_num_active_acl_handles>0 */
        if (current->active) {


            updateAmount=0;  
        
            //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: iteration=%d, handle=0x%02X with 0x%02X slots to distribute",iteration,current->handle,prh_hci_fc_acl_slots_unearmarked_in_hc));
        
            if (iteration==0) {               
                if ( (!current->packetsInHC==0) && current->numCanSend==0) {
                    updateAmount=MIN_VAL(fairSharePerHandlePerHandle,prh_hci_fc_acl_slots_unearmarked_in_hc);
                }
            } else if (iteration==1) {
                if (current->numCanSend==0) {
					if(fairSharePerHandlePerHandle > current->packetsInHC)
	                    updateAmount=MIN_VAL((unsigned)(fairSharePerHandlePerHandle-current->packetsInHC),prh_hci_fc_acl_slots_unearmarked_in_hc);
	            	else 
		                updateAmount=1;
                }
            } else if (iteration==2) {
                if (current->numCanSend<fairSharePerHandlePerHandle) {   
					if(fairSharePerHandlePerHandle > current->numCanSend)
	                    updateAmount=MIN_VAL((unsigned)(fairSharePerHandlePerHandle-current->numCanSend),prh_hci_fc_acl_slots_unearmarked_in_hc);
	            	else 
		                updateAmount=1;
                }
            } else { /* iteration==3 */
                updateAmount=1;
            }

            current->numCanSend += updateAmount;
            prh_hci_fc_acl_slots_unearmarked_in_hc -= updateAmount;

            //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: iteration %d: handle 0x%02X: adding 0x%02X to numCanSend, new numCanSend=0x%02X, 0x%02X slots left to distribute",
           //                                                iteration,current->handle,updateAmount,current->numCanSend,prh_hci_fc_acl_slots_unearmarked_in_hc));
        }

        current=current->next;
        if (!current) { /* wrap to start of list & keep going */
            current=prh_hci_acl_queue_list_head;
        }

        if (current==origin) { /* if we have scanned the entire queue, start a new iteration around the queue */
            iteration=(iteration+1)%4;
        }
    } while (prh_hci_fc_acl_slots_unearmarked_in_hc);

    prh_hci_fc_next_acl_update_queue=current;

#if pDEBUG
    origin=old_origin;
    current=origin;
    do {
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: Queued updated: record at 0x%p, handle=0x%02X, active=%d, packetsInHC=0x%02X, numCanSend=0x%02X, next record at 0x%p\n",
       //     current,current->handle, current->active, current->packetsInHC, current->numCanSend, current->next));
        
        current=current->next;
        if (!current) { /* wrap to start of list & keep going */
            current=prh_hci_acl_queue_list_head;
        }
    } while (current!=origin);
#endif

    prh_system_scheduler_execute();

    return BT_NOERROR;
}


/*************************************************************************************************
 * prh_hci_fc_rebalance_queues
 *
 * Clear the assigned flow control state for all handles to allow rebalancing to "start again". 
 * Called if a handle is created or becomes active (via link policy). 
 ************************************************************************************************/
void prh_hci_fc_rebalance_queues(void) {
    struct prh_st_t_hci_queue *current;

    //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: rebalancing queues, %d handles active\n",prh_hci_fc_num_active_acl_handles));

    current=prh_hci_acl_queue_list_head;
    while (current) {
        prh_hci_fc_acl_slots_unearmarked_in_hc+=current->numCanSend;
        current->numCanSend=0;

        current=current->next;
    }; 

    prh_hci_fc_update_acl_queues();

}
    
#if PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED==1


void prh_hci_report_host_completed_packets(void) {
    /* called from the timer and also optionally at a threshold value */
    struct prh_st_t_hci_queue *current;
    struct st_t_dataBuf *hciCommand;
    u_int8 i;
    u_int16 handlesToReport;
    t_api status;

    if (prh_hci_fc_hc_to_host_fc_enabled_acl) {

        pMutexLock(prh_hci_fc_acl_queue_lock);

        current=prh_hci_acl_queue_list_head;
        handlesToReport=0;
        while (current) { 
            if (current->numConsumedPackets)
                handlesToReport++;
            current=current->next;
        }

        do {
            current=prh_hci_acl_queue_list_head;

            /* max 63 handles may be reported in any one command */
            status=HCI_GetWriteBuffer(HCI_NO_HANDLE,(u_int16)(1+3+4*(handlesToReport>63?63:handlesToReport)),&hciCommand);
            if (status) {
                //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: Cannot allocate %d bytes to send HCI_HostNumCompletedPackets command\n",3+4*(handlesToReport>63?63:handlesToReport)));
				break;
            }
        
            handlesToReport-=handlesToReport>63?63:handlesToReport;
            BT_WRITE_LE_UINT16(hciCommand->buf,HCI_cmdHOSTNUMBEROFCOMPLETEDPACKETS);
            i=0;
            while (current) { 
                if (current->numConsumedPackets) {
                    BT_WRITE_LE_UINT16(hciCommand->buf+4+i*4,current->handle);
                    BT_WRITE_LE_UINT16(hciCommand->buf+6+i*4,current->numConsumedPackets);
                   // pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: reporting 0x%02X packets consumed on handle 0x%02X\n",current->numConsumedPackets,current->handle));
                    current->numConsumedPackets=0;
                    i++;
                } 
                current=current->next;
            }
            hciCommand->buf[2]=1+i*4;
            hciCommand->buf[3]=i;
            hciCommand->dataLen=1+3+i*4;
            status=HCI_SendRawPDU(HCI_pduCOMMAND,hciCommand);
            if (status) {
                //pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI FC: serious error - cannot send HCI_HostNumCompletedPackets command - error %d\n", status));
                /* the next time is called the ram may be available - if so, the system will recover */
                HCI_FreeWriteBuffer(hciCommand);
				break;
            }
        } while (handlesToReport);
        pMutexUnlock(prh_hci_fc_acl_queue_lock);
    } else {
        pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: prh_hci_report_host_completed_packets() called but HC->Host flow control not enabled\n"));
    }

    /* SCO data flow control currently not supported since SCO data over the HCI is not supported */
}

void prh_hci_report_host_completed_packets_timer(void *dummy) {
    static int lastReport=1;
    
    if (prh_hci_fc_total_queued_acl_bytes || lastReport)  {
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: prh_hci_report_host_completed_packets_timer() calling prh_hci_report_host_completed_packets()\n"));
        prh_hci_report_host_completed_packets();
        lastReport=1;
    } else {
        //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI FC: prh_hci_report_host_completed_packets_timer() NOT calling prh_hci_report_host_completed_packets()\n"));
    }

    if (!prh_hci_fc_total_queued_acl_bytes)
        lastReport=0;
    
    if (prh_hci_fc_total_queued_acl_bytes && !lastReport)
        lastReport=1;

    prh_hci_report_host_completed_packets_timerhandle=pTimerCreate(PRH_HOST_HC_TO_HOST_FLOW_CONTROL_REPORTING_FREQUENCY,prh_hci_report_host_completed_packets_timer,NULL,pTIMER_ONESHOT);
}

#endif /* PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED */




