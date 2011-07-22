/***********************************************************************
 *
 * MODULE NAME:    sdp_state_machine.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery Test code
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_state_machine.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#include "host_config.h"

#if SDP_CLIENT

#include "sdp_stack_access.h"

#include "sdp_state_machine.h"
#include "sdp_le_request_encoder.h"
#include "sdp_callbacks.h"
#include "sdp_constants.h"
#include "sdp_le.h"

struct prh_sdp_primitive_task* p_SDP_Primitive_Task_List;


extern u_int8 waiting_for_disconnect;
extern struct prh_sdp_primitive_task* p_disconnect_waiting_task;

static u_int8 prh_sdp_browse_state = 0;

static u_int16 prh_sdp_allocate_transaction_id(void);

#if PRH_HOST_TCI_ENABLE
extern void* prh_sdp_tci_lower_result;
#endif /* PRH_HOST_TCI_ENABLE */

/* local types */

struct prh_sdp_list_type
{
  u_int32 browse_group;
  u_int32 length;
  u_int8* pData;
  struct prh_sdp_list_type* pNext;
};

static struct prh_sdp_list_type* p_browse_stage_one = 0x00;
static struct prh_sdp_list_type* p_browse_stage_two = 0x00;
static struct prh_sdp_list_type* p_browse_stage_three = 0x00;
static t_SDP_AttributeIDs prh_browse_attributes;


	
static u_int8 prh_sdp_browse_extract_public(struct prh_sdp_primitive_task* pTask);
static u_int8 prh_sdp_browse_extract_groupIDs(struct prh_sdp_primitive_task* pTask);
static u_int8 prh_sdp_browse_extract_attribs_for_group(struct prh_sdp_primitive_task* pTask);
static void prh_sdp_browse_free(struct prh_sdp_list_type* this);
static void prh_sdp_browse_store_results(struct prh_sdp_primitive_task* pTask);


int prh_sdp_l2cap_connect_channel(t_bdaddr remote_address, u_int16 remote_psm);

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_add_primitive_task
 * DESCRIPTION:
 
 This function will place a task at the end of the task list.

 * KNOWN ISSUES:
 
 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_add_primitive_task(struct prh_sdp_primitive_task *p_task)
{
	struct prh_sdp_primitive_task* p_temp;
	
	if(p_SDP_Primitive_Task_List)
    {
		p_temp = p_SDP_Primitive_Task_List;
		
		while(p_temp->p_next != 0x00) p_temp = p_temp->p_next;
		
		p_temp->p_next = p_task;
		
    } 
    else 
    {
		p_SDP_Primitive_Task_List = p_task;
    }
	
	p_task->p_next = 0x00;
	
	if(p_SDP_Primitive_Task_List == p_task)
    {
		p_task->execute(p_task,0,0x00,p_task->rx_buffer[0] ? p_task->rx_buffer : 0x00);
    } 
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_free_primitive_task
 * DESCRIPTION:
 
 This function will free the allocated memory associated with a sdp
 primitive task structure.

 * KNOWN ISSUES:
 
 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_free_primitive_task(struct prh_sdp_primitive_task *pTask)
{
	
	if(pTask)
    {
		if(pTask->rx_buffer)
		{
			pFree(pTask->rx_buffer);
		}
		if(pTask->service_records)
		{
			pFree(pTask->service_records);
		}                 
		if(pTask->devices.addresses)
		{
			pFree(pTask->devices.addresses);
		}
		if(pTask->pattern.patternUUIDs)
		{
			pFree(pTask->pattern.patternUUIDs);
		}
		if(pTask->attributes.attributeIDs)
		{
			pFree(pTask->attributes.attributeIDs);
		}
		
		pFree(pTask);
    }
}



/***********************************************************************
 *
 * FUNCTION NAME:    prh_sdp_allocate_transaction_id
 * DESCRIPTION:
 
 This function will allocate a transaction id by incrementing a value.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
static u_int16 prh_sdp_allocate_transaction_id(void)
{
	static u_int16 transaction_id = 0;
	
	return transaction_id++;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_timer_handler
 * DESCRIPTION:
 
 This function will clear the timer an instruct the primitive task to 
 finish.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_timer_handler(void *task)
{
	struct prh_sdp_primitive_task* pTask;
	pTask = (struct prh_sdp_primitive_task*)task;
	
	    pTask->timer_handle=0;
	
	prh_sdp_finish_primitive_task(pTask);
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_finish_primitive_task
 * DESCRIPTION:
 
 This function will finish a task by: a) calling the application 
 callback. b) removing the task from the list. c) execution the next task
 that is contained on the task list.

 * KNOWN ISSUES:
 
 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_finish_primitive_task(struct prh_sdp_primitive_task *pTask)
{
    sdp_api_callback callbackFunc;
    u_int8 callbackStatus;

#if PRH_HOST_TCI_ENABLE
	prh_sdp_tci_lower_result = pTask->app_data;
#endif /* PRH_HOST_TCI_ENABLE */
	
    if (pTask->timer_handle!=0) {
	    pTimerCancel(pTask->timer_handle);
        pTask->timer_handle=0;
    }

    callbackStatus=pTask->status;
    callbackFunc=pTask->app_callback;
	
	SDP_TerminatePrimitive(pTask->app_data); /* remove it from the list */
	
	if(p_SDP_Primitive_Task_List != 0x00)
    {
		p_SDP_Primitive_Task_List->execute(p_SDP_Primitive_Task_List,0,0x00,p_SDP_Primitive_Task_List->rx_buffer);
    }        

    if(callbackFunc)   
        callbackFunc(callbackStatus);
}

#if SDP_SS_AND_SA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_w4_service
 * DESCRIPTION: 
 
 This function will handle the callback for the service search. It will
 store the results and then issue the attribute requests.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void prh_sdp_w4_service(struct prh_sdp_primitive_task *pTask,u_int32 length, u_int8* pData, u_int8* pContinue)
{	
	/* store data */
	
	if(length)
	{
		
		pMemcpy(&pTask->rx_buffer[pTask->offset],pData,length << 2);
		
		pTask->offset += length << 2;
		
		pTask->record_handles_left += length;
		
		if(!pContinue)
		{ 
			/* modify state */
			pTask->sub_task_level_1.state = 0x01;
		}
		
		/* call execute */
	} else {
		pTask->sub_task_level_1.state = 0x03;
	}
	
	
	pTask->sub_task_level_1.execute(pTask,pContinue);
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_w4_attributes
 * DESCRIPTION:
 
 This function will store the partial results of the request. 

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void prh_sdp_w4_attributes(struct prh_sdp_primitive_task *pTask,u_int32 length, u_int8* pData,u_int8* pContinue)
{
	u_int8* pTemp;
	
	/* store data */
	
	if(pTask->offset + length > pTask->rx_size)
    {      
		pTemp = pMalloc(pTask->rx_size <<=1);
		
		if(pTemp)
		{
			pMemcpy(pTemp,pTask->rx_buffer,pTask->offset);
			pFree(pTask->rx_buffer);
			
			pTask->rx_buffer = pTemp;
		} else {
			pTask->status = BT_NORESOURCES;
			
			L2_Disconnect(pTask->cid);
			
			prh_sdp_finish_primitive_task(pTask);
			return;
		}      
    }
	
	
	
	pMemcpy(&pTask->rx_buffer[pTask->offset],pData,length);
	
	
	pTask->offset += length;
	
	if( !pContinue )
    {
		/* modify state */
		pTask->sub_task_level_2.state = 1; /* finished */
		
    }
	
	/* call the sub task */
	
	
	
	pTask->sub_task_level_2.execute(pTask,pContinue);	
}

#endif /* SDP_SS_AND_SA_SUPPORT */

#if SDP_SSA_SUPPORT 

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_w4_service_attributes
 * DESCRIPTION:
 
 This function will store the returned data and it will reissue the request
 if the continue is set.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_w4_service_attributes(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue)
{
	
	u_int8* pTemp;
	
	
	
	/* store data */
	
	if(pTask->offset + length > pTask->rx_size)
    {
		pTemp = pMalloc(pTask->rx_size <<=1);
		
		if(pTemp)
		{
			pMemcpy(pTemp,pTask->rx_buffer,pTask->offset);
			pFree(pTask->rx_buffer);
			
			pTask->rx_buffer = pTemp;
		} else {
			pTask->status = BT_NORESOURCES;
			
			L2_Disconnect(pTask->cid);
			
			prh_sdp_finish_primitive_task(pTask);
			return;
		}      
    }
	
	pMemcpy(&(pTask->rx_buffer[pTask->offset]),pData,length);
	
	pTask->offset += length;
	
	if( !pContinue )
    {
		pTask->sub_task_level_1.state = 1;
    }
	
	pTask->sub_task_level_1.execute(pTask,pContinue);
}

#endif /* SDP_SSA_SUPPORT */


#if SDP_SDAP

#if SDP_SS_AND_SA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_w4_connections
 * DESCRIPTION:
 
 This function is called to start of the state machine as well a 
 connection is establised. And when the searches for a bd_address are 
 finished.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void prh_sdp_w4_connections(struct prh_sdp_primitive_task *pTask,u_int32 length, u_int8* pData, u_int8* pContinue)
{
	u_int8 error_code;
	
	error_code = 0;
	
	if(pTask->current_bd < pTask->devices.numItems)
    {
        
		if(pTask->w4_l2cap_connection)
		{
			pTask->w4_l2cap_connection = 0x00;
			/*
			I was called from the response
			so I will need to call the find services for the bd_address that just came back
			*/
			pTask->current_bd++;
			pTask->w4_Service_Search = 1;
			
			pTask->sub_task_level_1.state = 00;
			pTask->sub_task_level_1.execute = prh_sdp_find_services;
			pTask->sub_task_level_1.waiter = prh_sdp_w4_service;
			
			
			prh_sdp_find_services(pTask,0x00);
            
		} else {
            
			pTask->mtu = MGR_HintMTU(SDP_PSM);
            
			pTask->w4_l2cap_connection = 0x01;
            
			pTask->app_data->status = 0;
			pTask->app_data->resultType = SDP_SEARCH_RESULT;
            
			error_code = prh_sdp_l2cap_connect_channel(pTask->devices.addresses[pTask->current_bd], SDP_PSM);
			if(error_code != BT_NOERROR && error_code != BT_PENDING)
			{
				pTask->w4_l2cap_connection = 0x00;
				pTask->current_bd++;
				pTask->execute(pTask,0,0,0);
			}
		}        
    } else {
		prh_sdp_finish_primitive_task(pTask);
    }
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_find_services
 * DESCRIPTION:
 
 This function will create a service search pdu and send it. It will 
 create a level_1 task to handle the response.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_find_services(struct prh_sdp_primitive_task* pTask,u_int8* pContinue)
{
	struct st_t_SDP_Element *pAttribs;
	struct host_buf* h_buff;
	
	
	switch(pTask->sub_task_level_1.state)
	{
	case 0: /* searching for handles */
		
		
		/* generate message and send it */
		
		h_buff = prh_sdp_allocate_tx_buffer(pTask->mtu, pTask->cid);
		
		
		if(h_buff)
		{
			pTask->sub_task_level_1.transaction_id = prh_sdp_allocate_transaction_id();
			
			prh_sdp_generate_search_request(h_buff->data,pTask,pContinue);
			
			prh_sdp_transfer_data(pTask->cid,h_buff);
		} else { /* no resources */
			
			pTask->status = BT_NORESOURCES;
			
			L2_Disconnect(pTask->cid);
			
			prh_sdp_finish_primitive_task(pTask);
		}
		
		break;
	case 1: /* found all handles */
		
		
		/* store handles */
		pMemcpy(pTask->service_records,pTask->rx_buffer,pTask->offset);
		pTask->record_handles_left = pTask->offset >> 2;
		pTask->offset = 0;
		
		/* set up sub task */
		
		pTask->sub_task_level_2.state = 0;
		pTask->sub_task_level_2.execute = prh_sdp_search_attributes;
		pTask->sub_task_level_2.waiter = prh_sdp_w4_attributes;
		
		/* allocate space */
		
		pTask->app_data->numElements++;
		pAttribs = &pTask->app_data->element;
		
		while(pAttribs->pNext) pAttribs = pAttribs->pNext;
		if(pTask->app_data->numElements > 1)
		{
			pAttribs->pNext = pMalloc( sizeof(t_SDP_Element));
			
			if(!pAttribs->pNext)
			{ /* no resources */
				pTask->status = BT_NORESOURCES;
				
				L2_Disconnect(pTask->cid);
				
				prh_sdp_finish_primitive_task(pTask);
				return;
			}
			
			pAttribs = pAttribs->pNext;
			pAttribs->pNext = 0x00;
			
			pTask->length -= sizeof(t_SDP_Element);
		}
		
		pAttribs->deviceAddress = pTask->devices.addresses[pTask->current_bd -1];
		pAttribs->pData = 0x00;
		pAttribs->deviceName = 0x00;
		pAttribs->numMatchingRecords = 0x00;
		
		/* run sub task */
		
		prh_sdp_search_attributes(pTask,0x00);
		break;
		
	case 2: /* finished sub task */
		
		
		
			/* 
			I should look at the getting the remote name here and
			have an extra state to handle that 
		*/
		
		if(pTask->record_handles_left)
		{
			/* start next sub task */
			pTask->sub_task_level_2.state = 0; /* reset sub task */
			prh_sdp_search_attributes(pTask,0x00);
		} else {
			
			/* set state and call to the top */
			
			if(pTask->retrieveName)
			{
				pAttribs = &pTask->app_data->element;
				
				while(pAttribs->pNext) pAttribs = pAttribs->pNext;
				
				pAttribs->deviceName = pMalloc(41);

				if(!pAttribs->deviceName)
				{
					pTask->status = BT_NORESOURCES;
					L2_Disconnect(pTask->cid);

					prh_sdp_finish_primitive_task(pTask);

					return;
				}

				pAttribs->deviceName[0] = 0x00;
				MGR_GetRemoteName(pTask->devices.addresses[pTask->current_bd -1],pAttribs->deviceName,41,0x00,prh_sdp_manager_name_cb);
				break;
			}
			
			
			L2_Disconnect(pTask->cid);
			
			pTask->execute(pTask,0x00,0x00,0x00);
		}
		
		break;
	case 3: /* no handles */
		L2_Disconnect(pTask->cid);
		
		pTask->execute(pTask,0x00,0x00,0x00);
		break;
	}
	
	
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_search_attributes
 * DESCRIPTION:
 
 This function will generate a level_2 task and add it to the end of the
 list. It will then generate pdu and send it. When there are no more records
 left the it will call back up the Find_Services function.

 * KNOWN ISSUES:

 contains a hard coded malloc.
 
 *
 *
 ***********************************************************************/
 void prh_sdp_search_attributes(struct prh_sdp_primitive_task *pTask,u_int8* pContinue)
 {
	 struct host_buf* h_buff;
	 
	 struct st_t_SDP_Element *pAttribs;
	 
	 if(pTask->sub_task_level_2.state == 1 )
	 {
		 
		 /* store result */
		 pAttribs = &pTask->app_data->element;
		 
		 while(pAttribs->pNext) pAttribs = pAttribs->pNext;
		 
		 pAttribs->numMatchingRecords++;
		 
		 pTask->record_handles_left--;
		 
		 if(pTask->record_handles_left == 0)
		 {
			 pAttribs->pData = pMalloc(pTask->offset);
			 
			 pTask->length -= pTask->offset;
			 
			 if(!pAttribs->pData)
			 { /* no resources */
				 pTask->status = BT_NORESOURCES;
				 L2_Disconnect(pTask->cid);
				 
				 prh_sdp_finish_primitive_task(pTask);
				 return;
			 }
			 
			 if( (s_int32) pTask->length - (s_int32)pTask->offset < 0)
			 { /* hit the rails on the max byte limit */
				 L2_Disconnect(pTask->cid);
				 
				 prh_sdp_finish_primitive_task(pTask);
				 return;
			 } else {
				 pTask->length -= pTask->offset;
			 }
			 
			 
			 pMemcpy(pAttribs->pData,pTask->rx_buffer,pTask->offset);
			 pAttribs->dataLen = pTask->offset;
			 
			 pTask->offset = 0;
		 }
		 
		 
		 /* set state */
		 
		 pTask->sub_task_level_1.state = 2;
		 
		 /* call upper */
		 pTask->sub_task_level_1.execute(pTask,0x00);
		 
	 } else {
		 
		 
		 /* generate message */
		 
		 pTask->sub_task_level_2.transaction_id =  prh_sdp_allocate_transaction_id();
		 
        	h_buff = prh_sdp_allocate_tx_buffer(pTask->mtu, pTask->cid);
			
			if(h_buff)
			{
				prh_sdp_generate_attribute_request(h_buff->data,pTask,pContinue);      
				
				/* transmit */    
				prh_sdp_transfer_data(pTask->cid,h_buff);
			} else { /* no resources */
				
				pTask->status = BT_NORESOURCES;
				
				L2_Disconnect(pTask->cid);
				
				prh_sdp_finish_primitive_task(pTask);
			}
			
	 }	 
 }

#endif /* SDP_SS_AND_SA_SUPPORT */

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_w4_enumerate
 * DESCRIPTION:
 
 This function will handle the callback from the inquery I should send
 the results back and remove itself fromt the list and execute the next
 task if there is one.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
 void prh_sdp_w4_enumerate(struct prh_sdp_primitive_task *pTask,u_int32 length, u_int8* pData, u_int8* pContinue)
 {
	 u_int8 result;
	 t_SDP_Addresses *pDevices;
	 if(pTask->w4_l2cap_connection)
	 {
		 prh_sdp_finish_primitive_task(pTask);
		 
	 } else {
		 pDevices = (t_SDP_Addresses*)pTask->app_data;
		 
		 pDevices->addresses = pMalloc(sizeof(t_bdaddr)*pTask->rule.maxItems);
		 
		 if(pDevices->addresses)
		 {
			 result = MGR_Inquiry((u_int8)pTask->rule.maxDuration,pTask->iac,(u_int8)pTask->rule.maxItems,prh_sdp_manager_inquiry_cb);
			 pTask->w4_l2cap_connection = 0x01;
			 
			 if(result != BT_PENDING && result != BT_NOERROR)
			 {
				 pTask->status = result;
				 
				 prh_sdp_finish_primitive_task(pTask);
			 }
		 } else {
			 pTask->status = BT_NORESOURCES;
			 prh_sdp_finish_primitive_task(pTask);
		 }
	 }
 }
 
 

#if SDP_SSA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_w4_browse_connections
 * DESCRIPTION:
 
 This function will handle the callback for connection establishment for
 the browse commands.

 * KNOWN ISSUES: 
 None.

 *
 *
 ***********************************************************************/
 void prh_sdp_w4_browse_connections(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue)
 {
	 u_int8 error_code;
	 
	 if(pTask->current_bd < pTask->devices.numItems)
	 {
		 
		 if(pTask->w4_l2cap_connection)
		 {
			 pTask->w4_l2cap_connection = 0x00;
			 
			 pTask->current_bd++;
			 
			 pTask->sub_task_level_1.transaction_id = prh_sdp_allocate_transaction_id();
			 pTask->sub_task_level_1.state = 0x00;
			 pTask->sub_task_level_1.execute = prh_sdp_find_services_attributes;
			 pTask->sub_task_level_1.waiter = prh_sdp_w4_service_attributes;
			 
			 prh_sdp_find_services_attributes(pTask,0x00);
			 
		 } else {
			 pTask->mtu = MGR_HintMTU(SDP_PSM);
			 
			 pTask->app_data->status = 0;
			 pTask->app_data->resultType = SDP_BROWSE_RESULT;
			 
			 pTask->w4_l2cap_connection = 0x01;
			 error_code = prh_sdp_l2cap_connect_channel(pTask->devices.addresses[pTask->current_bd], SDP_PSM);
			 
			 if(error_code != BT_NOERROR && error_code != BT_PENDING)
			 {
				 pTask->w4_l2cap_connection = 0x00;
				 pTask->current_bd++;
				 pTask->execute(pTask,0,0,0);
			 }
		 }        
	 } else {
		 prh_sdp_finish_primitive_task(pTask);
	 }
 }
 
 
 
/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_find_services_attributes
 * DESCRIPTION:
 
 This function will create a level_1 task to handle the results of a 
 service attribute request.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
 void prh_sdp_find_services_attributes(struct prh_sdp_primitive_task* pTask,u_int8* pContinue)
 {
	 struct st_t_SDP_Element *pAttribs;    
	 struct host_buf* h_buff;
	 
	 
	 switch(pTask->sub_task_level_1.state)
	 {
	 case 0x00:
		 /* send request */
		 pTask->sub_task_level_1.transaction_id = prh_sdp_allocate_transaction_id();
		 
		 h_buff = prh_sdp_allocate_tx_buffer(pTask->mtu, pTask->cid);
       		if(h_buff)
			{  
				prh_sdp_genetate_service_attribute_request(h_buff->data,pTask,pContinue);
				
				prh_sdp_transfer_data(pTask->cid,h_buff);
			} else {
				pTask->status = BT_NORESOURCES;
				
				L2_Disconnect(pTask->cid);
				prh_sdp_finish_primitive_task(pTask);		
			}
			
			break;
	 case 0x01:
		 /* store the results */
		 
		 /*
		 copy the data into the the output
		 */
		 
		 pTask->app_data->numElements++;
		 
		 pAttribs = &pTask->app_data->element;
		 
		 while(pAttribs->pNext) pAttribs = pAttribs->pNext;
		 
		 if(pTask->app_data->numElements > 1)
		 {
			 if(prh_sdp_browse_state == 0x01 || prh_sdp_browse_state == 0x00) /* the start of a browse */
			 {
				 pAttribs->pNext = pMalloc(sizeof(t_SDP_Element));
				 
				 if(!pAttribs->pNext)
				 { /* no resources */
					 pTask->status = BT_NORESOURCES;
					 
					 L2_Disconnect(pTask->cid);
					 
					 prh_sdp_finish_primitive_task(pTask);
					 return;
				 }
				 
				 pAttribs = pAttribs->pNext;
				 pAttribs->pNext = 0x00;
				 
				 pTask->length -= sizeof(t_SDP_Element);
				 
			 } else if(prh_sdp_browse_state > 0x01){
				 pTask->app_data->numElements--;
			 }
		 }
		 
		 if(prh_sdp_browse_state > 0x01)
		 {
			 pFree(pAttribs->pData);
			 pTask->length += pTask->offset;
		 }
		 
		 pAttribs->pData = pMalloc(pTask->offset);
		 
		 if(!pAttribs->pData)
		 { /* no resources */
			 pTask->status = BT_NORESOURCES;
			 
			 L2_Disconnect(pTask->cid);
			 
			 prh_sdp_finish_primitive_task(pTask);
			 return;
		 }
		 
		 if( (s_int32)pTask->length - (s_int32)pTask->offset < 0)
		 {
			 if(prh_sdp_browse_state > 0x01)
			 {
				 pTask->status = BT_NORESOURCES;
			 }
			 
			 L2_Disconnect(pTask->cid);
			 
			 prh_sdp_finish_primitive_task(pTask);
			 return;
		 } else {
			 pTask->length -= pTask->offset;
		 }
		 
		 pAttribs->dataLen = pTask->offset;
		 pAttribs->deviceName = 0x00;
		 pAttribs->numMatchingRecords = 1;
		 pAttribs->deviceAddress = pTask->devices.addresses[pTask->current_bd -1];
		 pMemcpy(pAttribs->pData,pTask->rx_buffer,pTask->offset);
		 
		 pTask->offset = 0;
		 
		 if(pTask->retrieveName)
		 {
			 pAttribs->deviceName = pMalloc(41);

			 if(!pAttribs->deviceName)
			 {
				 pTask->status = BT_NORESOURCES;

				 L2_Disconnect(pTask->cid);
				 prh_sdp_finish_primitive_task(pTask);
				 return;				
			 }

			 pAttribs->deviceName[0] = 0x00;
			 MGR_GetRemoteName(pTask->devices.addresses[pTask->current_bd -1],pAttribs->deviceName,41,0x00,prh_sdp_manager_name_cb);
			 break;
		 }
		 
		 if(prh_sdp_browse_state)
		 {
			 pTask->execute(pTask,0x00,0x00,0x00);
		 } else {
			 /* close the connection */
			 
			 L2_Disconnect(pTask->cid);
			 
			 
			 waiting_for_disconnect = 0x01;
			 p_disconnect_waiting_task = pTask;
			 
		 }
		 
		 break;
	case 0x02:
		
		if(prh_sdp_browse_state)
		{
			pTask->execute(pTask,0x00,0x00,0x00);
		} else {
			/* close the connection */
			
			L2_Disconnect(pTask->cid);
			
			
			waiting_for_disconnect = 0x01;
			p_disconnect_waiting_task = pTask;
			
		}
		
		break;
	}
}

#endif /* SDP_SSA_SUPPORT */

#endif /* SDP SDAP */



#if SDP_GENERIC_BROWSE 


/***********************************************************************
 *
 * FUNCTION NAME:    prh_sdp_generic_browse
 * DESCRIPTION:
 
 This function will perform a generic browse. That is it will firstly open
 a connection. Then it will construct a request to retrieve the service class 
 list and attributes of the services that are members of the public browse
 group. Next it will constuct a message to retrieve the group ids of
 service that are member of the BrowseGroupDescriptor class. For each of 
 the group id that is retrieves it will construct a message to retrieve the 
 service class list and attributes of services that are members of that 
 browse group. The results are gathered and sent back to the caller.
 NOTE : a service search attribute request is used in each case.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 void prh_sdp_generic_browse(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue)
 {
	 u_int8 error_code;
	 
	 if(pTask->current_bd < pTask->devices.numItems)
	 {
		 
		 if(pTask->w4_l2cap_connection)
		 {
			 
			 pTask->w4_l2cap_connection = 0x00;
			 
			 prh_sdp_browse_state = 0x01;
			 
			 pTask->pattern.numItems = 1;
			 pTask->pattern.patternUUIDs[0] = SDP_SCLASS_UUID_PUBLIC;
			 
			 
			 /* set ids in the connection */
			 pTask->attributes.attributeIDs[0] = SDP_AID_SERVICE_CLASS;
			 
			 pTask->sub_task_level_1.state = 00;
			 pTask->sub_task_level_1.execute = prh_sdp_find_services_attributes;
			 pTask->sub_task_level_1.waiter = prh_sdp_w4_service_attributes;
			 
			 
			 prh_sdp_find_services_attributes(pTask,0x00);
			 
			 
		 } else {
			 
			 switch(prh_sdp_browse_state)
			 {
			 case 0x00: 
				 
				 pTask->app_data->status = 0;
				 pTask->app_data->resultType = SDP_GENERIC_BROWSE_RESULT;
				 
				 
				 pTask->w4_l2cap_connection = 0x01;
				 
				 /* store the attributes */
				 
				 prh_browse_attributes = pTask->attributes; /* copy over the ids */
				 pTask->attributes.numItems++;
				 pTask->attributes.attributeIDs = pMalloc(pTask->attributes.numItems * sizeof(u_int32) );
				 
				 if(!pTask->attributes.attributeIDs)
				 { /* no resources */
					 pTask->status = BT_NORESOURCES;
					 pFree(prh_browse_attributes.attributeIDs);
					 
					 prh_sdp_finish_primitive_task(pTask);
					 return;
				 }
				 
				 pMemcpy(&pTask->attributes.attributeIDs[1],prh_browse_attributes.attributeIDs,prh_browse_attributes.numItems << 2);
				 /* the other list will be deallocated when the primitive task is freed */
				 error_code = prh_sdp_l2cap_connect_channel(pTask->devices.addresses[pTask->current_bd], SDP_PSM);
				 
				 if(error_code != BT_NOERROR && error_code != BT_PENDING)
				 {
					 pTask->w4_l2cap_connection = 0x00;
					 pTask->current_bd++;
					 prh_sdp_browse_state = 0;
					 
					 pTask->execute(pTask,0,0,0);
				 }
				 
				 break;
			 case 0x01:
				 
				 prh_sdp_browse_state++;
				 
				 /*
				 Finished Public Browse Group
				 
				   Start The Browse Group Descriptor
				 */
				 
				 prh_sdp_browse_extract_public(pTask);
				 
				 
				 pTask->pattern.patternUUIDs[0] = SDP_SCLASS_UUID_BROWSE;
				 
				 pTask->attributes.numItems = 1;
				 pTask->attributes.attributeIDs[0] = SDP_AID_GROUP_ID;
				 
				 pTask->sub_task_level_1.state = 00;
				 
				 prh_sdp_find_services_attributes(pTask,0x00);
				 
				 pTask->record_handles_left = 0;
				 
				 
				 break;
			 case 0x02:
			 /*
			 Finished obtaining the group descriptor
			 for each group descriptor get the service class and the attributes
				 */
				 
				 prh_sdp_browse_extract_groupIDs(pTask);
				 
				 prh_sdp_browse_state++;
				 pTask->record_handles_left = p_browse_stage_two->length;
				 
				 pTask->pattern.numItems = 1;
				 pTask->pattern.patternUUIDs[0] = (p_browse_stage_two->pData[((pTask->record_handles_left-1)<<1)] << 8) +
					 p_browse_stage_two->pData[1 + ((pTask->record_handles_left-1)<<1)];
				 
				 /* put the service class attribute back in */
				 
				 pTask->attributes.numItems = prh_browse_attributes.numItems + 1; /* one extra to contain the service class id */
				 pTask->attributes.attributeIDs[0] = SDP_AID_SERVICE_CLASS;
				 
				 
				 pTask->record_handles_left--;
				 pTask->sub_task_level_1.state = 00;
				 
				 prh_sdp_find_services_attributes(pTask,0x00);
				 
				 break;
			 case 0x03:
				 
				 prh_sdp_browse_extract_attribs_for_group(pTask);
				 
				 if(pTask->record_handles_left)
				 {         
					 pTask->pattern.numItems = 1;
					 pTask->pattern.patternUUIDs[0] = (p_browse_stage_two->pData[(pTask->record_handles_left-1)<<1] << 8) +
						 p_browse_stage_two->pData[1 + ((pTask->record_handles_left-1)<<1)];
					 
					 
					 /* the attributes are unchanged */
					 
					 pTask->sub_task_level_1.state = 00;
					 prh_sdp_find_services_attributes(pTask,0x00);
					 
					 pTask->record_handles_left--;
				 } else {
				 /*
				 store results
					 */
					 
					 prh_sdp_browse_store_results(pTask);
					 
					 /*
					 clean up
					 */
					 
					 prh_sdp_browse_free(p_browse_stage_one);
					 p_browse_stage_one = 0x00;
					 prh_sdp_browse_free(p_browse_stage_two);
					 p_browse_stage_two = 0x00;
					 prh_sdp_browse_free(p_browse_stage_three);
					 p_browse_stage_three = 0x00;
					 
					 /*
					 Disconnect
					 */
					 pTask->current_bd++;
					 L2_Disconnect(pTask->cid);
					 waiting_for_disconnect = 0x01;
					 p_disconnect_waiting_task = pTask; 
					 prh_sdp_browse_state = 0x00;
				 }
				 
				 break;
        }
    }        
    } else {
		prh_sdp_finish_primitive_task(pTask);
    }
}

/***********************************************************************
 *
 * FUNCTION NAME:    prh_sdp_browse_extract_public
 * DESCRIPTION:
 
 This function will extract the results from the public browse group and
 store them in the p_browse_stage_one structure.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 u_int8 prh_sdp_browse_extract_public(struct prh_sdp_primitive_task* pTask)
 {
	 u_int8 ret_value = 1;
	 
	 if(pTask->app_data->element.dataLen)
	 {
		 p_browse_stage_one = pMalloc(sizeof(struct prh_sdp_list_type));
		 if(p_browse_stage_one)
		 {
			 p_browse_stage_one->pData = pMalloc(pTask->app_data->element.dataLen);
			 if(p_browse_stage_one->pData)
			 {
				 pMemcpy(p_browse_stage_one->pData,pTask->app_data->element.pData,pTask->app_data->element.dataLen);
				 p_browse_stage_one->length = pTask->app_data->element.dataLen;
				 p_browse_stage_one->pNext = 0x00;
				 p_browse_stage_one->browse_group = 0x00;
				 
				 pTask->app_data->element.dataLen = 0x00;
			 } else {
				 pTask->status = BT_NORESOURCES;
				 
				 L2_Disconnect(pTask->cid);
				 
				 prh_sdp_finish_primitive_task(pTask);
				 
				 pFree(p_browse_stage_one);         
				 ret_value = 0;
			 }
		 } else {
			 pTask->status = BT_NORESOURCES;
			 
			 L2_Disconnect(pTask->cid);
			 
			 prh_sdp_finish_primitive_task(pTask);
			 
			 ret_value = 0;
		 }
	 } else {
		 ret_value = 0;
	 }
	 
	 return ret_value;
 }

/***********************************************************************
 *
 * FUNCTION NAME:    prh_sdp_browse_extract_groupIDs
 * DESCRIPTION:
 
 This function will 

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 u_int8 prh_sdp_browse_extract_groupIDs(struct prh_sdp_primitive_task* pTask)
 {
	 u_int8 ret_value = 1;
	 u_int8* pFinger;
	 u_int32 counter = 0;
	 struct st_t_SDP_Element *pAttribs;
	 
	 pAttribs = &pTask->app_data->element;
	 while(pAttribs->pNext) pAttribs = pAttribs->pNext;
	 
	 if(pAttribs->dataLen)
	 {
		 p_browse_stage_two = pMalloc(sizeof(struct prh_sdp_list_type));
		 if(p_browse_stage_two)
		 {
			 p_browse_stage_two->length = 0;
			 
			 p_browse_stage_two->pData = pMalloc(10 * 2);
			 if(p_browse_stage_two->pData)
			 {
				 pFinger = pAttribs->pData;
				 /* extract the total size */
				 switch(*pFinger++)
				 {
				 case SDP_SEQUENCE_SMALL:
					 counter = *pFinger++;
					 break;
				 case SDP_SEQUENCE_MEDIUM:
					 counter = prh_sdp_read16(pFinger);
					 pFinger += 2;
					 break;
				 case SDP_SEQUENCE_LARGE:
					 counter = prh_sdp_read32(pFinger);
					 pFinger +=4;
					 break;
				 default:
					 ret_value = 0;
					 break;
				 }
				 
				 while(counter)
				 {         
					 switch(*pFinger++)
					 {
					 case SDP_SEQUENCE_SMALL:
						 pFinger++;
						 counter -=2;
						 break;
					 case SDP_SEQUENCE_MEDIUM:
						 pFinger +=2;
						 counter -=3;
						 break;
					 case SDP_SEQUENCE_LARGE:
						 pFinger +=4;
						 counter -=5;
						 break;
					 default:
						 ret_value = 0;
						 break;
					 }
					 
					 if(pFinger[0] == SDP_UINT_2 && pFinger[1] == 0x02 && pFinger[2] == 0x00)
					 {          
						 pFinger +=3;
						 counter -=3;
						 
						 switch(*pFinger++)
						 {
						 case SDP_UUID_2:
							 p_browse_stage_two->pData[p_browse_stage_two->length << 1] = pFinger[0];
							 p_browse_stage_two->pData[1 + (p_browse_stage_two->length << 1)] = pFinger[1];
							 pFinger +=2;
							 counter -=3;
							 break;
						 case SDP_UUID_4:
							 ((u_int16*)(p_browse_stage_two->pData))[p_browse_stage_two->length] = prh_sdp_read32(pFinger);
							 pFinger +=4;
							 counter -=5;
							 break;
						 case SDP_UUID_16:
							 ((u_int16*)(p_browse_stage_two->pData))[p_browse_stage_two->length] = prh_sdp_read32(pFinger);
							 pFinger +=16;
							 counter -=17;
							 break;
						 default:
							 /* this is an error case I will need a goto*/
							 break;
						 }
						 p_browse_stage_two->length++;
					 } else {
						 ret_value = 0;
					 }
				 }
				 
				 p_browse_stage_two->pNext = 0x00;
				 p_browse_stage_two->browse_group = 0x00;
				 
				 pTask->app_data->element.dataLen = 0x00;
			 } else {          
				 ret_value = 0;
			 }
		 } else {
			 ret_value = 0;
		 }
	 } else {
		 ret_value = 0;
	 }
	 
	 return ret_value;
	 
}



/***********************************************************************
 *
 * FUNCTION NAME:    prh_sdp_allocate_transaction_id
 * DESCRIPTION:
 
 This function will allocate a transaction id by incrementing a value.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 u_int8 prh_sdp_browse_extract_attribs_for_group(struct prh_sdp_primitive_task* pTask)
 {
	 u_int8 ret_value = 1;
	 struct st_t_SDP_Element *pAttribs;
	 struct prh_sdp_list_type *pBrowse_info;
	 
	 pAttribs = &pTask->app_data->element;
	 while(pAttribs->pNext) pAttribs = pAttribs->pNext;
	 
	 
	 if(pAttribs->dataLen)
	 {
		 if(p_browse_stage_three == 0x00)
		 {
			 p_browse_stage_three = pMalloc(sizeof(struct prh_sdp_list_type));
			 pBrowse_info = p_browse_stage_three;
		 } else {
			 pBrowse_info = p_browse_stage_three;
			 while(pBrowse_info->pNext) pBrowse_info = pBrowse_info->pNext;
			 pBrowse_info->pNext = pMalloc(sizeof(struct prh_sdp_list_type));
			 pBrowse_info = pBrowse_info->pNext;
		 }
		 
		 if(pBrowse_info)
		 {
			 pBrowse_info->pData = pMalloc(pAttribs->dataLen);
			 if(pBrowse_info->pData)
			 {
				 pMemcpy(pBrowse_info->pData,pAttribs->pData,pAttribs->dataLen);
				 pBrowse_info->length = pAttribs->dataLen;
				 pBrowse_info->pNext = 0x00;
				 pBrowse_info->browse_group = (p_browse_stage_two->pData[(pTask->record_handles_left)*2] << 8) +
					 p_browse_stage_two->pData[1 + ((pTask->record_handles_left)*2)];;
				 
				 pTask->app_data->element.dataLen = 0x00;
			 } else {          
				 ret_value = 0;
			 }
		 } else {
			 ret_value = 0;
		 }
	 } else {
		 ret_value = 0;
	 }
	 
	 return ret_value;  
 }


/***********************************************************************
 *
 * FUNCTION NAME:    prh_sdp_allocate_transaction_id
 * DESCRIPTION:
 
 This function will allocate a transaction id by incrementing a value.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 void prh_sdp_browse_free(struct prh_sdp_list_type* this)
 {
	 pFree(this->pData);
	 if(this->pNext)
		 prh_sdp_browse_free(this->pNext);
	 pFree(this);
 }

/***********************************************************************
 *
 * FUNCTION NAME:    prh_sdp_allocate_transaction_id
 * DESCRIPTION:
 
 This function will allocate a transaction id by incrementing a value.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 static void prh_sdp_browse_store_results(struct prh_sdp_primitive_task* pTask)
 {
	 struct st_t_SDP_Element *pAttribs;
	 struct prh_sdp_list_type *pBrowse_info;
	 u_int8* pFinger;
	 
	 pAttribs = &pTask->app_data->element;
	 while(pAttribs->pNext) pAttribs = pAttribs->pNext;
	 
	 pFree(pAttribs->pData); /* clear out the data */
	 
	 pAttribs->dataLen = p_browse_stage_one->length;
	 
	 pAttribs->numMatchingRecords = p_browse_stage_two->length;
	 
	 pBrowse_info = p_browse_stage_three;
	 
	 while(pBrowse_info)
	 {
		 pAttribs->dataLen += 2 + pBrowse_info->length;
		 
		 pBrowse_info = pBrowse_info->pNext;
	 }
	 
	 pAttribs->pData = pMalloc(pAttribs->dataLen);
	 
	if(!pAttribs->pData)
	{
		pTask->status = BT_NORESOURCES;
		L2_Disconnect(pTask->cid);
		prh_sdp_finish_primitive_task(pTask);
		return;
	}

	 pFinger = pAttribs->pData;
	 
	 pMemcpy(pFinger,p_browse_stage_one->pData,p_browse_stage_one->length);
	 
	 pFinger += p_browse_stage_one->length;
	 
	 pBrowse_info = p_browse_stage_three;
	 
	 while(pBrowse_info)
	 {
		 *pFinger++ = pBrowse_info->browse_group >> 8;
		 *pFinger++ = pBrowse_info->browse_group & 0xff;
		 
		 pMemcpy(pFinger,pBrowse_info->pData,pBrowse_info->length);
		 pFinger += pBrowse_info->length;
		 
		 pBrowse_info = pBrowse_info->pNext;
	 }
 }

#endif /* SDP_GENERIC_BROWSE */

#if SDP_INTERFACE

#if SDP_SS_AND_SA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_int_ss
 * DESCRIPTION:
 
 This function will add a level 1 task to do a service search request.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 void prh_sdp_int_ss(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue)
 {
	 
	 if(!pTask->w4_l2cap_connection)
	 {
		 pTask->status = 0x00;
		 prh_sdp_finish_primitive_task(pTask);	
	 } else {
		 /* construct sub task */
		 pTask->status = 0xff;
		 pTask->sub_task_level_1.state = 00;
		 pTask->sub_task_level_1.execute = prh_sdp_int_find_services;
		 pTask->sub_task_level_1.waiter = prh_sdp_w4_service;
		 
		 prh_sdp_int_find_services(pTask,pContinue);		
	 }
 }
 
/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_int_find_services
 * DESCRIPTION:
 
 This function will sent a service search request.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 void prh_sdp_int_find_services(struct prh_sdp_primitive_task* pTask,u_int8* pContinue)
 {	
	 struct host_buf* h_buff;
	 
	 switch(pTask->sub_task_level_1.state)
	 {
	 case 0: /* searching for handles */
		 
		 /* generate message and send it */
		 
		 
		 h_buff = prh_sdp_allocate_tx_buffer(pTask->mtu, pTask->cid);
		 
		 if(h_buff)
		 {
			 pTask->sub_task_level_1.transaction_id = prh_sdp_allocate_transaction_id();			
			 prh_sdp_generate_search_request(h_buff->data,pTask,pContinue);			
			 prh_sdp_transfer_data(pTask->cid,h_buff);
		 } else { /* no resources */
			 
			 pTask->status = BT_NORESOURCES;		
			 prh_sdp_finish_primitive_task(pTask);
		 }
		 
		 pTask->sub_task_level_1.state = 0x01;
		 pTask->w4_l2cap_connection = 0; 
		 
		 break;
	 case 1: /* found all handles */
		 
		 /* store handles */
		 
		 prh_sdp_int_store_results(pTask,pContinue,1,1);

		 pTask->execute(pTask,0x00,0x00,0x00);
		 break;
	 }		
 }
 
 
/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_int_sa
 * DESCRIPTION:
 
 This function will create a level 1 task to do a service attributes 
 request.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 void prh_sdp_int_sa(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue)
 {
	 
	 if(!pTask->w4_l2cap_connection)
	 {
		 pTask->status = 0x00;
		 prh_sdp_finish_primitive_task(pTask);	
	 } else {
		 /* construct sub task */
		 pTask->status = 0xff;
		 pTask->sub_task_level_2.state = 00;
		 pTask->sub_task_level_2.execute = prh_sdp_int_search_attributes;
		 pTask->sub_task_level_2.waiter = prh_sdp_w4_attributes;
		 
		 prh_sdp_int_search_attributes(pTask,pContinue);		
	 }
 }
 
 
/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_int_search_attributes
 * DESCRIPTION:
 
 This function will send a service attribute request.
 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 void prh_sdp_int_search_attributes(struct prh_sdp_primitive_task* pTask,u_int8* pContinue)
 {	
	 struct host_buf* h_buff;
	 
	 switch(pTask->sub_task_level_2.state)
	 {
	 case 0:		
		 /* generate message and send it */
		 
		 h_buff = prh_sdp_allocate_tx_buffer(pTask->mtu, pTask->cid);
		 
		 if(h_buff)
		 {
			 pTask->sub_task_level_2.transaction_id = prh_sdp_allocate_transaction_id();
			 prh_sdp_generate_attribute_request(h_buff->data,pTask,pContinue);
			 prh_sdp_transfer_data(pTask->cid,h_buff);
		 } else { /* no resources */
			 pTask->status = BT_NORESOURCES;			
			 prh_sdp_finish_primitive_task(pTask);
		 }
		 
		 pTask->sub_task_level_2.state = 0x01;
		 pTask->w4_l2cap_connection = 0; 
		 		 
		 break;
	 case 1:
		 
		 /* store results */
		 
		 prh_sdp_int_store_results(pTask,pContinue,1,2);
		 
		 /* change state */

		 pTask->execute(pTask,0x00,0x00,0x00);
		 break;
	 }		
 }
 
#endif /*SDP_SS_AND_SA_SUPPORT */
 
#if SDP_SSA_SUPPORT 
 
/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_int_ssa
 * DESCRIPTION:
 
 This function will create a level 1 task to do a service search 
 attribute request.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 void prh_sdp_int_ssa(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue)
 {
	 
	 if(!pTask->w4_l2cap_connection)
	 {
		 pTask->status = 0x00;
		 prh_sdp_finish_primitive_task(pTask);	
	 } else {
		 /* construct sub task */
		 pTask->status = 0xff;
		 pTask->sub_task_level_1.state = 00;
		 pTask->sub_task_level_1.execute = prh_sdp_int_find_service_attributes;
		 pTask->sub_task_level_1.waiter = prh_sdp_w4_service_attributes;
		 
		 prh_sdp_int_find_service_attributes(pTask,pContinue);		
	 }
 }
 
 
/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_int_find_service_attributes
 * DESCRIPTION:
 
 This function will send a service search attribute request.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 void prh_sdp_int_find_service_attributes(struct prh_sdp_primitive_task* pTask,u_int8* pContinue)
 {	
	 struct host_buf* h_buff;
	 
	 switch(pTask->sub_task_level_1.state)
	 {
	 case 0:
		 
		 /* generate message and send it */
		 
		 h_buff = prh_sdp_allocate_tx_buffer(pTask->mtu, pTask->cid);
		 
		 if(h_buff)
		 {
			 pTask->sub_task_level_1.transaction_id = prh_sdp_allocate_transaction_id();
			 prh_sdp_genetate_service_attribute_request(h_buff->data,pTask,pContinue);				
			 prh_sdp_transfer_data(pTask->cid,h_buff);
		 } else { /* no resources */
			 
			 pTask->status = BT_NORESOURCES;
			 prh_sdp_finish_primitive_task(pTask);
		 }
		 
		 pTask->sub_task_level_1.state = 0x01;
		 pTask->w4_l2cap_connection = 0; 

		 
		 break;
	 case 1:
		 
		 /* store results */
		 prh_sdp_int_store_results(pTask,pContinue,1,3);
		 
		 /* change state */
		 
		 pTask->execute(pTask,0x00,0x00,0x00);
		 break;
	 }		
 }
 
#endif /* SDP_SSA_SUPPORT */
 
/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_int_store_results
 * DESCRIPTION:
 
 This function will store results for the interface commands the low level
 results structure.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 void prh_sdp_int_store_results(struct prh_sdp_primitive_task* pTask, u_int8* pContinue, u_int16 status, u_int8 type)
 {
	 t_SDP_LowLevelResults* pResults;
	 u_int8 counter;
	 
	 pResults = (t_SDP_LowLevelResults*)pTask->app_data;
	 /*
		check the max bytes
	 */
	 if(pTask->offset > pTask->rule.maxBytes)
		 pTask->offset = pTask->rule.maxBytes;

	 pResults->pData = pMalloc(pTask->offset);

	 if(!pResults->pData)
	 {
		 pResults->status = BT_NORESOURCES;

		 return;
	 }


	 pMemcpy(pResults->pData,pTask->rx_buffer,pTask->offset);		
	 pResults->length = pTask->offset;
	 pResults->type = type;
	 pResults->status = status;
	 if(pContinue)
	 {
		 for(counter = 0; counter < pContinue[0] + 1; counter++)
			 pResults->Continue[counter] = pContinue[counter];
	 } else {
		 pResults->Continue[0] = 0;	
	 }
 }

#endif /* SDP_INTERFACE */

#endif /* SDP_CLIENT */



