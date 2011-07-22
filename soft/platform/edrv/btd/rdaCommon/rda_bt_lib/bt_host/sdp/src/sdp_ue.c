/***********************************************************************
 *
 * MODULE NAME:    sdp_ue.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery Test code
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_ue.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001-2001 Parthus Technologies
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

#include "sdp_ue.h"
#include "sdp_le.h"
#include "sdp_le_request_encoder.h"
#include "sdp_callbacks.h"
#include "sdp_state_machine.h"

extern struct prh_sdp_primitive_task* p_SDP_Primitive_Task_List;
extern u_int8 waiting_for_disconnect ;
extern struct prh_sdp_primitive_task* p_disconnect_waiting_task;

static struct prh_sdp_primitive_task* _sdp_create_primitive(sdp_api_callback callback,t_SDP_Results* pResults,t_classDevice cod,t_SDP_Addresses* p_devices,t_SDP_SearchPattern *p_pattern,t_SDP_AttributeIDs* p_attributes, u_int8 retrieveName, t_SDP_StopRule* rule,prh_sdp_task_function execute,u_int16 length);
static u_int32 prh_sdp_allocate_primitive_id(void);
static u_int8 prh_check_stop_rule(t_SDP_StopRule* pRule);


#if SDP_SDAP

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_EnumerateDevices
 * DESCRIPTION:
 
 This function will create a new task for enumerate and it will add it
to the list of tasks. if it is the only task then it will start the task
executing.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_EnumerateDevices(t_SDP_Addresses** devices,u_int32 cod,u_int32 iac,t_SDP_StopRule* pRule,sdp_api_callback callback, void *reserved)
{
	u_int8 ret_value;
	u_int32 prim_id;
	struct prh_sdp_primitive_task *p_task;
	
	ret_value = BT_PENDING;
	
	if(!prh_check_stop_rule(pRule))
		return BT_INVALIDPARAM;
	
	
	prim_id = prh_sdp_allocate_primitive_id();
	
	if(pRule->maxDuration > 0x30)
		pRule->maxDuration = 0x30;
	
	*devices = pMalloc(sizeof( t_SDP_Addresses));
	
	if(*devices)
	{
		
		pMemset(*devices,0,sizeof( t_SDP_Addresses));
		
		p_task = _sdp_create_primitive(callback,(t_SDP_Results*)*devices,0x00,0x00,0x00,0x00,0x00,pRule,prh_sdp_w4_enumerate,0);
		
		if(p_task)
		{
			p_task->cod = cod;
			if(!iac)
				iac = BT_GIAC_LAP;
			p_task->iac = iac;
			p_task->service_primitive_id = prim_id;
			
			prh_sdp_add_primitive_task(p_task);
		} else {
			ret_value = BT_NORESOURCES;
		}
	} else {
		ret_value = BT_NORESOURCES;
	}
    
	return ret_value;
}

#if SDP_SS_AND_SA_SUPPORT
/***********************************************************************
 *
 * FUNCTION NAME:   SDP_ServiceSearch
 * DESCRIPTION:
 
 This function will create a search task and add it to the list. 

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_ServiceSearch(t_SDP_Results** pResults,t_SDP_Addresses* pDevices, t_SDP_SearchPattern* pPattern, t_SDP_AttributeIDs* pAttributes, t_SDP_StopRule* pRule, u_int16 flags, sdp_api_callback callback)
{
	u_int8 ret_value;
	t_SDP_Addresses* allowed_devices;
	
	u_int8 retrieveName;
	
	u_int32 prim_id;
	struct prh_sdp_primitive_task *p_task;
	
	if(!prh_check_stop_rule(pRule))
		return BT_INVALIDPARAM;

	pRule->maxDuration *=10;
		
	retrieveName = 0;
	
	ret_value = BT_PENDING;
	
	allowed_devices = pDevices;
	
	prim_id = prh_sdp_allocate_primitive_id();
	
	*pResults = pMalloc(sizeof(t_SDP_Results));
	
	if(*pResults)
    {
		pMemset(*pResults,0,sizeof(t_SDP_Results));
		
		p_task = _sdp_create_primitive(callback,*pResults,0x00,allowed_devices,pPattern,pAttributes,(u_int8)(flags & SDP_REMOTE_NAME),pRule,prh_sdp_w4_connections,PRH_SDP_BASE_RX_BUFFER_SIZE);
		
		if(p_task)
		{
			p_task->service_primitive_id = prim_id;
			
			p_task->timer_handle=pTimerCreate(p_task->rule.maxDuration,prh_sdp_timer_handler,p_task,pTIMER_ONESHOT);
			
			/*
			now I will need to add the primitive task to the task list.
			*/
			
			prh_sdp_add_primitive_task(p_task);
			
		} else {
			pFree(*pResults);
			ret_value = BT_NORESOURCES;
		}
    } else {
		ret_value = BT_NORESOURCES;
    }
	
	return ret_value;
}

#endif /* SDP_SS_AND_SA_SUPPORT */



#if SDP_SSA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_ServiceBrowse
 * DESCRIPTION:
 
 This function will generate a task for the service browse and it will
add it to the list.

 * KNOWN ISSUES:
 
 it does not support the relation stuff.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_ServiceBrowse(t_SDP_Results** pResults,t_SDP_Addresses* pDevices, t_SDP_SearchPattern* pPattern, t_SDP_AttributeIDs* pAttributes, t_SDP_StopRule* pRule, u_int16 flags, sdp_api_callback callback)
{
	u_int8 ret_value;
	
	u_int32 prim_id;
	struct prh_sdp_primitive_task *p_task;  
	t_SDP_Addresses* allowed_devices;
	
	u_int8 retrieveName;
	
	if(!prh_check_stop_rule(pRule))
		return BT_INVALIDPARAM;
	
	pRule->maxDuration *=10;
		
	retrieveName = 0;
	
	ret_value = BT_PENDING;
	
	allowed_devices = pDevices;
	
	prim_id = prh_sdp_allocate_primitive_id();
	
	*pResults = pMalloc(sizeof(t_SDP_Results));
	
	if(*pResults)
    {
		pMemset(*pResults,0,sizeof(t_SDP_Results));
		p_task = _sdp_create_primitive(callback,*pResults,0x00,allowed_devices,pPattern,pAttributes,(u_int8)(flags & SDP_REMOTE_NAME),pRule,prh_sdp_w4_browse_connections, PRH_SDP_BASE_RX_BUFFER_SIZE);
		
		if(p_task)
		{
			p_task->service_primitive_id = prim_id;
			
			p_task->timer_handle=pTimerCreate(p_task->rule.maxDuration,prh_sdp_timer_handler,p_task,pTIMER_ONESHOT);
			
			prh_sdp_add_primitive_task(p_task);
			
		} else {
			ret_value =  BT_NORESOURCES;
		}
		
    } else {
		ret_value =  BT_NORESOURCES;
    } 
	
	return ret_value;
}

#endif /* SDP_SSA_SUPPORT */

#if SDP_GENERIC_BROWSE && SDP_SSA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_GenericBrowse
 * DESCRIPTION:
 
 This function will perform a generic browse as per sdp test spec
 test case TP/SERVER/BRW/BV-02-C

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_GenericBrowse(t_SDP_Results** pResults,t_SDP_Addresses* pDevices, t_SDP_AttributeIDs* pAttributes, t_SDP_StopRule* pRule, u_int16 flags, sdp_api_callback callback)
{
	u_int8 ret_value;
	u_int32 prim_id;
	struct prh_sdp_primitive_task *p_task;
	t_SDP_SearchPattern* pPattern;
	u_int8 retrieveName;
	
	ret_value = BT_PENDING;

	pPattern = 0;
	retrieveName = 0;
	
	pRule->maxItems = 1;
	
	if(!prh_check_stop_rule(pRule))
		return BT_INVALIDPARAM;
	
	pRule->maxDuration *=10;
	
	pPattern = pMalloc(sizeof(t_SDP_SearchPattern));
	
	if(pPattern){
		pPattern->numItems = 1;
		pPattern->patternUUIDs = pMalloc(4);

        if (pPattern->patternUUIDs) {
            
		    prim_id = prh_sdp_allocate_primitive_id();
		    
		    *pResults = pMalloc(sizeof(t_SDP_Results));
		    
		    if(*pResults)
		    {
			    pMemset(*pResults,0,sizeof(t_SDP_Results));
			    p_task = _sdp_create_primitive(callback,*pResults,0x00,pDevices,pPattern,pAttributes,(u_int8)(flags & SDP_REMOTE_NAME),pRule,prh_sdp_generic_browse, PRH_SDP_BASE_RX_BUFFER_SIZE);
			    
			    if(p_task)
			    {
				    p_task->timer_handle=pTimerCreate(p_task->rule.maxDuration,prh_sdp_timer_handler,p_task,pTIMER_ONESHOT);
				    
				    p_task->service_primitive_id = prim_id;
				    
				    prh_sdp_add_primitive_task(p_task);
			    } else {
                    pFree(pPattern->patternUUIDs);
				    pFree(pPattern);
				    pFree(*pResults);
				    ret_value = BT_NORESOURCES;
			    }
		    } else {
                pFree(pPattern->patternUUIDs);
			    pFree(pPattern);
			    ret_value = BT_NORESOURCES;
		    }
        } else {
			pFree(pPattern);
		    ret_value = BT_NORESOURCES;
        }
	} else {
		ret_value = BT_NORESOURCES;
	}
	return ret_value;
}

#endif /* SDP_GENERIC_BROWSE && SDP_SSA_SUPPORT */







#endif /* SDP_SDAP */

/***********************************************************************
 *
 * FUNCTION NAME:    SDP_TerminatePrimitive
 * DESCRIPTION:
 
 This function will convert remove the task from the list.

 * KNOWN ISSUES:
 
 none

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_TerminatePrimitive(t_SDP_Results* pResults)
{
/*
The primitive will need to be taken from the primitive task list. 
	*/
	struct prh_sdp_primitive_task *p_task,*p_previous;

    
	if(p_SDP_Primitive_Task_List == 0x00)
		return BT_UNKNOWNERROR;
	
	for(p_task = p_SDP_Primitive_Task_List, p_previous = 0x00;
	p_task->p_next !=0x00 && p_task->app_data != pResults;
	p_task = p_task->p_next)
    {
		p_previous = p_task;
    }
	
	if(p_task->app_data == pResults)
    { 
		/* remove it from the list */
		
		if(p_previous != 0x00)
		{
			p_previous->p_next = p_task->p_next;
		} else if (p_task->p_next == 0x00) { /* just one left */            
			p_SDP_Primitive_Task_List = 0x00;         
		} else { /* there others after it */
			p_SDP_Primitive_Task_List = p_task->p_next;
		}
		
		/* clean up states */
		waiting_for_disconnect = 0;
		p_disconnect_waiting_task = 0;
        if (p_task->timer_handle!=0)
        {
		    pTimerCancel(p_task->timer_handle);
			p_task->timer_handle = 0;
		}
		prh_sdp_free_primitive_task(p_task);
    }
	
	return BT_NOERROR;
}




/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_create_primitive
 * DESCRIPTION:
 
 This function will construct a sdp primitive task and it will initialiase
it with the supplied values.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
static struct prh_sdp_primitive_task* _sdp_create_primitive(sdp_api_callback callback,t_SDP_Results* pResults,t_classDevice cod,t_SDP_Addresses* p_devices, t_SDP_SearchPattern *p_pattern, t_SDP_AttributeIDs* p_attributes, u_int8 retrieveName, t_SDP_StopRule* rule,prh_sdp_task_function execute,u_int16 length)
{
	struct prh_sdp_primitive_task *p_task;
	u_int32  lcv;
	
	p_task = (struct prh_sdp_primitive_task*)pMalloc(sizeof(struct prh_sdp_primitive_task));
	
	
	if(p_task)
    {
		/* initialise */
		pMemset(p_task,0x00,sizeof(struct prh_sdp_primitive_task));
		
		p_task->app_callback = callback;
		p_task->app_data = pResults;
		
		p_task->status = BT_NOERROR;
		pDebugCheck();
		
		p_task->length = rule->maxBytes - sizeof(t_SDP_Results);
		pDebugCheck();
		
		p_task->rx_buffer = pMalloc(length);

		pDebugCheck();
		
		p_task->rx_size = length;
		pDebugCheck();
		
		
		p_task->mtu = MGR_HintMTU(SDP_PSM);
		pDebugCheck();
		
		
		if(p_task->rx_buffer || (!p_task->rx_buffer && !length) )
		{
			if(length) p_task->rx_buffer[0] = 0;
			
			pDebugCheck();
			
			p_task->cod = cod;
			p_task->rule = *rule;
			
			pDebugCheck();
			
			p_task->execute = execute;
						
			
			pDebugCheck();
			p_task->service_records = pMalloc( rule->maxItems * 4); /* allocate space for the record handles */
			
			pDebugCheck();
			
			if(p_task->service_records)
			{
				p_task->retrieveName = retrieveName;
				
				if(p_devices)
				{
					p_task->devices.numItems = p_devices->numItems;
					
					p_task->devices.addresses = pMalloc(sizeof(t_bdaddr) * p_devices->numItems);
					
					if(!p_task->devices.addresses)
					{
						pFree(p_task->service_records);
						pFree(p_task->rx_buffer);
						pFree(p_task);
						return 0x00;
					}
					
					for(lcv = 0;lcv < p_devices->numItems;lcv++)
					{
						p_task->devices.addresses[lcv] = p_devices->addresses[lcv];
					}
				} 
				
				if(p_pattern)
				{
					p_task->pattern.numItems = p_pattern->numItems;
					
					p_task->pattern.patternUUIDs = pMalloc(sizeof(u_int32) * p_pattern->numItems);
					
					if(!p_task->pattern.patternUUIDs)
					{
						pFree(p_task->service_records);
						pFree(p_task->rx_buffer);
						if(p_task->devices.addresses)
							pFree(p_task->devices.addresses);
						pFree(p_task);
						
						return 0x00;
					}
					
					for(lcv = 0;lcv < p_pattern->numItems;lcv++)
					{
						p_task->pattern.patternUUIDs[lcv] = p_pattern->patternUUIDs[lcv];
					}
				} 
				
				if(p_attributes)
				{
					p_task->attributes.numItems = p_attributes->numItems;
					
					p_task->attributes.attributeIDs = pMalloc(sizeof(u_int32) * p_attributes->numItems);
					
					if(!p_task->attributes.attributeIDs)
					{
						pFree(p_task->service_records);
						pFree(p_task->rx_buffer);
						if(p_task->devices.addresses)
							pFree(p_task->devices.addresses);
						if(p_task->pattern.patternUUIDs)
							pFree(p_task->pattern.patternUUIDs);
						pFree(p_task);
						
						return 0x0;
					}
					
					for(lcv = 0;lcv < p_attributes->numItems;lcv++)
					{
						p_task->attributes.attributeIDs[lcv] = p_attributes->attributeIDs[lcv];
					}
				} 
				
				
			} else {
				pFree(p_task->rx_buffer);
				pFree(p_task);
				p_task = 0x00;
			}
    } else {
		pFree(p_task);
		p_task = 0x00;
    }   
    }
	return p_task;
}

#if 0

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_ServiceSearch
 * DESCRIPTION:
 
 This function will create a search task and add it to the list. 

 * KNOWN ISSUES:
 
 it does not support the realation stuff yet

 *
 *
 ***********************************************************************/
 void _SDP_Check_Relation(SDPRelation relation,t_SDP_Addresses *pIn, t_SDP_Addresses* pOut)
 {
	 u_int8 counter;
	 u_int8 lcv;
	 u_int8 pass_trust;  
	 
	 counter = 0;
	 pass_trust = 0;
	 
	 pOut->addresses = pMalloc( sizeof(t_bdaddr) * pIn->numItems);
	 
	if(!pOut->addresses)
		return;

	 for(lcv = 0; lcv < pIn->numItems; lcv++)
	 {        
		 
		 pass_trust = 0;
		 
		 switch(relation.trust)
		 {
		 case SDP_Relation_TRUSTED:
			 if( MGR_GetDeviceTrustLevel(pIn->addresses[lcv]) == MGR_TRUSTED )
				 pass_trust = 1;
			 break;
		 case SDP_Relation_ZERO_PIN:
			 if( MGR_GetDeviceTrustLevel(pIn->addresses[lcv]) == MGR_TRUSTED || MGR_GetDevicePinType(pIn->addresses[lcv]) == MGR_ALLZEROPIN)
				 pass_trust = 1;  
			 break;
		 case SDP_Relation_NORMAL_PIN:
			 if( MGR_GetDeviceTrustLevel(pIn->addresses[lcv]) == MGR_TRUSTED || MGR_GetDevicePinType(pIn->addresses[lcv]) != MGR_ALLZEROPIN)
				 pass_trust = 1; 
			 break;
		 }
		 
		 if(pass_trust)
			 switch(relation.connection)
		 {
		 case SDP_Connected:
			 if(prh_mgr_isDeviceConnected(pIn->addresses[lcv]) == CONN_ACTIVE)
				 pOut->addresses[counter++] = pIn->addresses[lcv];
			 break;
		 case SDP_UnConnected:
			 if(prh_mgr_isDeviceConnected(pIn->addresses[lcv]) == CONN_CLOSED)
				 pOut->addresses[counter++] = pIn->addresses[lcv];
			 break;
		 case SDP_DontCare:
			 pOut->addresses[counter++] = pIn->addresses[lcv];
			 break;
		 }      
	 }
	 
	 pOut->numItems = counter;
 }

#endif

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_allocate_primitive_id
 * DESCRIPTION:
 
 This function will simply allocate a primitive id.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
 static u_int32 prh_sdp_allocate_primitive_id(void)
 {
	 static u_int32 primitive_id = 0;
	 
	 return primitive_id++;
 }

#if SDP_SDAP

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_FreeResults
 * DESCRIPTION:
 
 This function will free results structure.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
 APIDECL1 t_api APIDECL2 SDP_FreeResults(t_SDP_Results* pResults)
 {
	 struct st_t_SDP_Element *pElement,*pPrevious;
	 
	 pElement = pResults->element.pNext;
	 
	 while(pElement)
	 {
		 pPrevious = pElement;
		 pElement = pElement->pNext;
		 pFree(pPrevious->pData);
		 if(pPrevious->deviceName)
			 pFree(pPrevious->deviceName);
		 pFree(pPrevious);
	 }
	 
	 if (pResults->element.pData)
	 	pFree(pResults->element.pData);
	 if(pResults->element.deviceName)
		 pFree(pResults->element.deviceName);
	 pFree(pResults);
	 
	 return 0;
 }


/***********************************************************************
 *
 * FUNCTION NAME:   SDP_FreeAddressList
 * DESCRIPTION:
 
 This function will deallocate and address list.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
 APIDECL1 t_api APIDECL2 SDP_FreeAddressList(t_SDP_Addresses* devices)
 {
	 if(devices->addresses)
		 pFree(devices->addresses);
	 
	 pFree(devices);
	 
	 return 0;
 }


/***********************************************************************
 *
 * FUNCTION NAME:   SDP_EableCache
 * DESCRIPTION:
 
 This function will enable caching on the client side.

 * KNOWN ISSUES:
 
 unsupported.

 *
 *
 ***********************************************************************/
 APIDECL1 t_api APIDECL2 SDP_EnableCache(u_int32 maxBytes, u_int16 flags)
 {
	 return BT_UNSUPPORTED;
 }



/***********************************************************************
 *
 * FUNCTION NAME:   SDP_DisableCache
 * DESCRIPTION:
 
 This function will turn off support for caching.

 * KNOWN ISSUES:
 
 unsupported

 *
 *
 ***********************************************************************/
 APIDECL1 t_api APIDECL2 SDP_DisableCache(u_int16 flags)
 {
	 return BT_UNSUPPORTED;
 }
 


/***********************************************************************
 *
 * FUNCTION NAME:   prh_check_stop_rule
 * DESCRIPTION:
 
 This function will check the sized of the stop rule to see if they 
 are at a minimum

 * KNOWN ISSUES:
 
 unsupported

 *
 *
 ***********************************************************************/
 static u_int8 prh_check_stop_rule(t_SDP_StopRule* pRule)
 {
	 if(pRule->maxItems < 1 || pRule->maxDuration < 2 || pRule->maxBytes < 50)
		 return 0;
	 else
		 return 1;
 }
 


#endif /* SDP_SDAP */










/*
****************************************************************************************************************************************************************

SDP low level commands

****************************************************************************************************************************************************************
 */

#if SDP_INTERFACE


/***********************************************************************
 *
 * FUNCTION NAME:   SDP_ConnectTransport
 * DESCRIPTION:
 
 This function will bring up an l2cap connect for use by the interface
 function.

 * KNOWN ISSUES:
 
 unsupported

 *
 *
 ***********************************************************************/
 APIDECL1 t_api APIDECL2 SDP_ConnectTransport(t_bdaddr* address, u_int16 mtu,void (*configured)(u_int8,t_L2_ControlCallbackArgs *),void (*disconnect)(u_int8 type,t_L2_ControlCallbackArgs *))
 {
	 u_int8 ret_value;
	 
	 l2_callbacks cbs;
	 t_L2_ConfigReqParams req_params;
	 
	 
	 /* initialise the mtu */
	 p_SDP_Primitive_Task_List = 0x00;
	 
	 cbs.disconnectionIndication = disconnect;
	 cbs.qosviolationIndication = 0x00; /* null */
	 cbs.connectionIndication = prh_sdp_l2cap_connect_ind_cb;
	 cbs.disconnectionConfirmation = prh_sdp_l2cap_disconnect_cfm_cb;
	 cbs.dataIndication = prh_sdp_l2cap_data_ind_cb;
	 cbs.flushOccurred=pNULL;
	 cbs.bufferOverflow=pNULL;
	 cbs.timeoutIndication=pNULL;
	 
	 cbs.connectionConfirmation = pNULL;
	 cbs.configureIndication = pNULL;
	 cbs.configureConfirmation = pNULL;
	 cbs.connectionConfiguredConfirmation=configured;
	 
#if PRH_HOST_UNIT_TEST
	 L2_DeregisterPSM(SDP_PSM);
#endif 
	 L2_RegisterPSM(SDP_PSM,&cbs);	
	 
	 if(mtu)
	 {
		 if(mtu < PRH_L2CAP_MTU_MIN)
			 mtu = PRH_L2CAP_MTU_MIN;
	 } else {
		 mtu = MGR_HintMTU(SDP_PSM);
	 }
	 
	 req_params.inMTUmax=mtu;
	 req_params.outMTUmax=mtu;
	 req_params.inMTUmin=PRH_L2CAP_MTU_MIN;
	 req_params.outMTUmin=PRH_L2CAP_MTU_MIN;
	 req_params.flushTO=0xffff;
	 req_params.linkTO=0;
	 
	 ret_value = L2_ConnectandConfig(*address, SDP_PSM, &req_params);
	 
	 if(ret_value != BT_NOERROR && ret_value != BT_PENDING)
	 {
#if !PRH_HOST_UNIT_TEST
		 L2_DeregisterPSM(SDP_PSM);
#endif
	 }
	 
	 return ret_value;
 }

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_DisconnectTransport
 * DESCRIPTION:
 
 This function will disconnect the transport for SDP

 * KNOWN ISSUES:
 
 unsupported

 *
 *
 ***********************************************************************/
 APIDECL1 t_api APIDECL2 SDP_DisconnectTransport(t_channelId cid)
 {
	 u_int8 ret_value;
	 
	 ret_value = L2_Disconnect(cid);
	 
	 if(ret_value == BT_NOERROR)
	 {
#if !PRH_HOST_UNIT_TEST
		 L2_DeregisterPSM(SDP_PSM);
#endif
	 }
	 
	 return ret_value;
 }

#if SDP_SS_AND_SA_SUPPORT 

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_ServiceSearchRequest
 * DESCRIPTION:
 
 This function will check the sized of the stop rule to see if they 
 are at a minimum

 * KNOWN ISSUES:
 
 unsupported

 *
 *
 ***********************************************************************/
 APIDECL1 t_api APIDECL2 SDP_ServiceSearchRequest(t_SDP_LowLevelResults** pResults,t_channelId cid, t_SDP_SearchPattern* pPattern, u_int16 maxCount, u_int8* Continue, u_int8 timeout, sdp_api_callback callback)
 {
	 u_int8 ret_value;
	 t_SDP_Addresses* allowed_devices;
	 u_int32 prim_id;
	 struct prh_sdp_primitive_task *p_task;
	 t_SDP_StopRule rule;	
	 u_int8 counter;
	 
	 ret_value = BT_PENDING;
	 
	 prim_id = prh_sdp_allocate_primitive_id();
	 
	 *pResults = pMalloc(sizeof(t_SDP_LowLevelResults));
	 
	 if(*pResults)
	 {
		 pMemset(*pResults,0,sizeof(t_SDP_LowLevelResults));
		 rule.maxBytes = maxCount * 4;
		 rule.maxDuration = timeout * 10;
		 rule.maxItems = maxCount;
		 
		 allowed_devices = 0x00;
		 
		 p_task = _sdp_create_primitive(callback,(t_SDP_Results*)*pResults,0x00,allowed_devices,pPattern,0x00,0,&rule,prh_sdp_int_ss,PRH_SDP_BASE_RX_BUFFER_SIZE);
		 
		 if(p_task)
		 {
			 p_task->service_primitive_id = prim_id;
			 p_task->cid = cid;
			 p_task->keep_connection = 1;
			 p_task->w4_l2cap_connection = 1;
			 
			 if(Continue)
			 {
				 for(counter = 0; counter < 17; counter++)
				 {
					 p_task->rx_buffer[counter] = Continue[counter];
				 }
			 }
			 
			 p_task->timer_handle=pTimerCreate(p_task->rule.maxDuration,prh_sdp_timer_handler,p_task,pTIMER_ONESHOT);
			 prh_sdp_add_primitive_task(p_task);
			 
		 } else {
			 pFree(*pResults);
			 ret_value = BT_NORESOURCES;
		 }
	 } else {
		 ret_value = BT_NORESOURCES;
	 }
	 
	 return ret_value;
 }
 
 


/***********************************************************************
 *
 * FUNCTION NAME:   SDP_ServiceAttributeRequest
 * DESCRIPTION:
 
 This function will check the sized of the stop rule to see if they 
 are at a minimum

 * KNOWN ISSUES:
 
 unsupported

 *
 *
 ***********************************************************************/
 APIDECL1 t_api APIDECL2 SDP_ServiceAttributeRequest(t_SDP_LowLevelResults** pResults, t_channelId cid, u_int32 handle, t_SDP_AttributeIDs* pAttributes, u_int16 maxBytes, u_int8* Continue, u_int8 timeout, sdp_api_callback callback)
 {
	 u_int8 ret_value;
	 t_SDP_Addresses* allowed_devices;
	 u_int32 prim_id;
	 struct prh_sdp_primitive_task *p_task;
	 t_SDP_StopRule rule;	
	 u_int8 counter;
	 
	 ret_value = BT_PENDING;
	 
	 prim_id = prh_sdp_allocate_primitive_id();
	 
	 *pResults = pMalloc(sizeof(t_SDP_LowLevelResults));
	 
	 if(*pResults)
	 {
		 pMemset(*pResults,0,sizeof(t_SDP_LowLevelResults));
		 rule.maxBytes = maxBytes + sizeof(t_SDP_Results);
		 rule.maxDuration = timeout * 10;
		 rule.maxItems = 1;
		 
		 allowed_devices = 0x00;
		 
		 p_task = _sdp_create_primitive(callback,(t_SDP_Results*)*pResults,0x00,allowed_devices,0x00,pAttributes,0,&rule,prh_sdp_int_sa,PRH_SDP_BASE_RX_BUFFER_SIZE);
		 
		 if(p_task)
		 {
			 
			 p_task->service_primitive_id = prim_id;
			 p_task->record_handles_left = 1;
			 prh_sdp_write32(handle,(u_int8*)p_task->service_records); /* must be bigendian */
			 p_task->cid = cid;
			 p_task->keep_connection = 1;
			 p_task->w4_l2cap_connection = 1;
			 
			 if(Continue)
			 {
				 for(counter = 0; counter < 17; counter++)
				 {
					 p_task->rx_buffer[counter] = Continue[counter];
				 }
			 }
			 
			 p_task->timer_handle=pTimerCreate(p_task->rule.maxDuration,prh_sdp_timer_handler,p_task,pTIMER_ONESHOT);
			 prh_sdp_add_primitive_task(p_task);
			 
		 } else {
			 pFree(*pResults);
			 ret_value = BT_NORESOURCES;
		 }
	 } else {
		 ret_value = BT_NORESOURCES;
	 }
	 
	 return ret_value;
 }

#endif /* SDP_SS_AND_SA_SUPPORT */

#if SDP_SSA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_ServiceSearchAttributeRequest
 * DESCRIPTION:
 
 This function will check the sized of the stop rule to see if they 
 are at a minimum

 * KNOWN ISSUES:
 
 unsupported

 *
 *
 ***********************************************************************/
 APIDECL1 t_api APIDECL2 SDP_ServiceSearchAttributeRequest(t_SDP_LowLevelResults** pResults, t_channelId cid, t_SDP_SearchPattern* pPattern, t_SDP_AttributeIDs* pAttributes, u_int16 maxBytes, u_int8* Continue, u_int8 timeout, sdp_api_callback callback)
 {
	 u_int8 ret_value;
	 t_SDP_Addresses* allowed_devices;
	 u_int32 prim_id;
	 struct prh_sdp_primitive_task *p_task;
	 t_SDP_StopRule rule;	
	 u_int8 counter;
	 
	 ret_value = BT_PENDING;
	 
	 prim_id = prh_sdp_allocate_primitive_id();
	 
	 *pResults = pMalloc(sizeof(t_SDP_LowLevelResults));
	 
	 if(*pResults)
	 {
		 pMemset(*pResults,0,sizeof(t_SDP_LowLevelResults));
		 rule.maxBytes = maxBytes + sizeof(t_SDP_Results);
		 rule.maxDuration = timeout * 10;
		 rule.maxItems = 1;
		 
		 allowed_devices = 0x00;
		 
		 p_task = _sdp_create_primitive(callback,(t_SDP_Results*)*pResults,0x00,allowed_devices,pPattern,pAttributes,0,&rule,prh_sdp_int_ssa,PRH_SDP_BASE_RX_BUFFER_SIZE);
		 
		 if(p_task)
		 {
			 
			 p_task->service_primitive_id = prim_id;
			 p_task->cid = cid;
			 p_task->keep_connection = 1;
			 p_task->w4_l2cap_connection = 1;
			 
			 if(Continue)
			 {
				 for(counter = 0; counter < 17; counter++)
				 {
					 p_task->rx_buffer[counter] = Continue[counter];
				 }
			 }
			 
			 p_task->timer_handle=pTimerCreate(p_task->rule.maxDuration,prh_sdp_timer_handler,p_task,pTIMER_ONESHOT);
			 prh_sdp_add_primitive_task(p_task);
			 
		 } else {
			 pFree(*pResults);
			 ret_value = BT_NORESOURCES;
		 }
	 } else {
		 ret_value = BT_NORESOURCES;
	 }
	 
	 return ret_value;
 }

#endif /* SDP_SSA_SUPPORT */


#endif /* SDP_INTERFACE */


t_api prh_sdp_cli_layer_ctrl(u_int8 flags) {
    if (flags&PRH_HOST_STARTUP) {
        SDP_RegisterPSM();
    } else if (flags&PRH_HOST_RESET) {
        SDP_RegisterPSM();
    }

    return BT_NOERROR;
}

#if SDP_DB_TO_FILE

void sdp_db_to_file(u_int8* data, u_int32 length)
{
	FILE* file;
	u_int32 counter;
	
	file = fopen("sdp_generated_db.c","w");
	
	/*

	write in header information
	*/
	fprintf(file,"/* Automatically generated file */\n\n");
	
	fprintf(file,"#include \"host_config.h\"\n");
	fprintf(file,"#include \"sdp_db.h\"\n\n");
	
	
	fprintf(file,"extern struct st_t_SDP_serviceRecord* prh_sdp_database;\n\n");
	
	fprintf(file,"u_int8 prh_sdp_static_db[] = {\n");
	
	for(counter=0;counter < length; counter += 16)
	{
		if(counter + 16 > length)
		{
			for(;counter < length;counter++)
			{
				fprintf(file,"%d,",data[counter]);
			}
			fprintf(file,"\n");
		} else {
			fprintf(file,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n",
				data[counter],
				data[counter + 1],
				data[counter + 2] ,
				data[counter + 3],
				data[counter + 4],
				data[counter + 5],
				data[counter + 6],
				data[counter + 7],
				data[counter + 8],
				data[counter + 9],
				data[counter + 10],
				data[counter + 11],
				data[counter + 12],
				data[counter + 13],
				data[counter + 14],
				data[counter + 15]);
		}
	}
	
	fprintf(file,"};\n\n");
	
	fprintf(file,"void prh_load_rom_db(void)\n");
	fprintf(file,"{\n");
	fprintf(file,"\tsdp_database = prh_sdp_static_db;\n");
	fprintf(file,"}\n\n\n");
	
	fclose(file);
}

#endif /* SDP_DB_TO_FILE */

u_int8 get_list_length(u_int8* data,u_int32 *len,u_int8* offset) 
{
	u_int8* internal;

	if(!data)
	{
		return BT_INVALIDPARAM;
	}

	internal = data;

	switch(*internal) 
	{

		case SDP_SEQUENCE_SMALL:
		case SDP_UNION_SMALL:
		case SDP_URL_SMALL:
		case SDP_TEXT_SMALL:
			*len = internal[1];
			*offset = 2;
		break;

		case SDP_SEQUENCE_MEDIUM:
		case SDP_UNION_MEDIUM:
		case SDP_URL_MEDIUM:
		case SDP_TEXT_MEDIUM:
			*len = (internal[1] << 8) + internal[2];
			*offset = 3;
		break;

		case SDP_SEQUENCE_LARGE:
		case SDP_UNION_LARGE:
		case SDP_URL_LARGE:
		case SDP_TEXT_LARGE:
			*len = (internal[1] << 24) + (internal[2] << 16) + (internal[3] << 8) + internal[4];
			*offset = 5;
		break;

		default:
			return BT_INVALIDPARAM;
	}
	return BT_NOERROR;
}


u_int8 read_uuid(u_int8* data,u_int32* uuid,u_int8* offset) 
{
	switch(*data) 
	{
		case SDP_UUID_2:
			*uuid = (data[1] << 8) + data[2];
			*offset = 3;
		break;

		case SDP_UUID_4:
			*uuid = (data[1] << 24) + (data[2] << 16) + (data[3] << 8) + data[4];
			*offset = 5;
		break;

		case SDP_UUID_16:
			*uuid = (data[1] << 24) + (data[2] << 16) + (data[3] << 8) + data[4];
			*offset = 17;
		break;

		default:
			return BT_INVALIDPARAM;
	}
	return BT_NOERROR;
}

extern void * memset(void *,int,size_t);
 APIDECL1 t_api APIDECL2  sdp_service_parse(t_SDP_Results *pResults, t_SDP_service_Parse_Results *service_finded)
{
    u_int32 uuid;
    u_int8 exit_condiction=0;
    u_int8 featuresFound = 0;
    u_int8 versionFound = 0;
    u_int8* finger;
    u_int8* end_byte;
    u_int8 counter;
    u_int16 aid;
    u_int8 offset;
    u_int32 length;
    u_int32 outer;
    u_int32 inner;
    u_int8 search_fail=0;
//    u_int8 incomplete_results=0;

    u_int32 total_len;


    u_int16 serverChannel=0;
    u_int16 versionInfo=0;
    u_int16 supportedFeatures=0; 
 //   u_int8  volCtrlSupported=0;

    int status=BT_NOERROR;

    int i=0;

    memset(service_finded,0,sizeof(t_SDP_service_Parse_Results));

    if(pResults==NULL)
        return BT_UNKNOWNERROR;

    status = pResults->status;

    if (status == BT_NOERROR)
    {
        total_len = pResults->element.dataLen;
        end_byte = pResults->element.pData + pResults->element.dataLen -1;

        finger = pResults->element.pData;
    		
        get_list_length(finger,&length,&offset);
        finger+= offset;
 	do 
	{           
            get_list_length(finger,&length,&offset);
            finger+= offset;
            counter = length;
            
            do 
            {
                counter--;
                finger++; /* bypass type code */
                aid = (finger[0] << 8) + finger[1];
                finger+=2;
                counter-=2;

                switch(aid) 
                {
                    case SDP_AID_SERVICE_CLASS:
                        get_list_length(finger,&length,&offset);
                        finger += offset;
                        counter -= offset;
                        outer = length;
        
                        read_uuid(finger,&uuid,&offset);
                        finger += offset;
                        counter -= offset;
                        service_finded->service[i].SC_uuid=uuid;
                        //pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"sdp_service_parse SC_uuid=%d",service_finded[i].SC_uuid)); 
                        outer -= offset;
                        finger +=outer;
                        counter-=outer;
                    break;
                    
                    case SDP_AID_PROTOCOL:
                        get_list_length(finger,&length,&offset);
                        finger += offset;
                        counter -= offset;
                        outer = length;
                        do 
                        {
                            do 
                            {
                                get_list_length(finger,&length,&offset);
                                finger +=offset;
                                counter -=offset;
                                outer -=offset;
                                inner = length;
                                read_uuid(finger,&uuid,&offset);
                                finger += offset;
                                counter -= offset;
                                outer -= offset;
                                inner -= offset;
                                if(uuid == SDP_PROTOCOL_UUID_RFCOMM && inner) 
                                {
                                    serverChannel = finger[1];
                                    service_finded->service[i].chn_num =  finger[1];
                                    //pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"sdp_service_parse chn_num=%d", service_finded[i].chn_num)); 
                                    finger+=2;
                                    counter-=2;
                                    inner -= 2;
                                    outer-=2;
                                }
                                else
                                {
                                    finger += inner;
                                    counter -= inner;
                                    outer -= inner;
                                    inner = 0;
                                }					
                            }while(inner);
                        }while (outer);
                    break;

                    case SDP_AID_PROFILE_LIST :
                        get_list_length(finger,&length,&offset);
                        finger += offset;
                        counter -= offset;
                        outer = length;
                        do 
                        {
                            do 
                            {
                                get_list_length(finger,&length,&offset);
                                finger +=offset;
                                counter -=offset;
                                outer -=offset;
                                inner = length;
                                read_uuid(finger,&uuid,&offset);
                                finger += offset;
                                counter -= offset;
                                outer -= offset;
                                inner -= offset;
                                service_finded->service[i].PDL_uuid = uuid;
                                 //pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"sdp_service_parse PDL_uuid=%d",service_finded[i].PDL_uuid)); 
                                if(uuid == SDP_SCLASS_UUID_HANDSFREE && inner) 
                                {
                                    versionInfo = (finger[1] << 8) + finger[2];
                                    versionFound = 1;
                                    finger+=3;
                                    counter-=3;
                                    inner -= 3;
                                    outer-=3;
                                }
                                else
                                {
                                    finger += inner;
                                    counter -= inner;
                                    outer -= inner;
                                    inner = 0;
                                }

                            } 	while(inner);
                        } while (outer);
                    break;

                    case SDP_AID_SUPPORTED_FEATURES :
                        counter--;
                        finger++; // Skip over Type code and len = 0x09
                        supportedFeatures = (finger[0] << 8) + finger[1];
                        featuresFound = 1;
                        finger+=2;
                        counter-=2;
                    break;

                    default :
                        exit_condiction=1;
                    break;

                }
            }while (counter && !exit_condiction); 
            i++;
            service_finded->total_searched = i;
	}while(finger<end_byte && service_finded->total_searched<15 );
    }
    else
    {
        // Search Complete returned an Error.
        search_fail = 1;
    }

	return search_fail;
}

#endif /* SDP_CLIENT */
