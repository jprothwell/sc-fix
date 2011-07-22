/***********************************************************************
 *
 * MODULE NAME:    sdp_callbacks.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery callback code
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_callbacks.c 1532 2010-08-23 05:47:34Z huazeng $
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

#if SDP_CLIENT||SDP_SERVER

#include "sdp_stack_access.h"


#include "sdp_callbacks.h"
#include "sdp_state_machine.h"
#include "sdp_error.h"
#include "sdp_le_request_decoder.h"
#include "sdp_le_response_decoder.h"
#include "sdp_le_response_encoder.h"
#include "sdp_le.h"
#include "host_buf.h"
#if COMBINED_HOST==0
#include "mgr_inqdb.h"
#endif


extern struct prh_sdp_primitive_task* p_SDP_Primitive_Task_List;

#if PRH_L2CAP_AUTO_CONFIG==0
u_int8 waiting_for_configure = 0;
u_int8 waiting_to_configure = 0;
#endif
u_int8 waiting_for_disconnect = 0;
struct prh_sdp_primitive_task* p_disconnect_waiting_task = 0x00;


extern tid_t inquiryCallbackTid[];

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_l2cap_connect_channel
 * DESCRIPTION:
 
 This function is used by the state machine code to bring up a l2cap
 channel.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
int prh_sdp_l2cap_connect_channel(t_bdaddr remote_address, u_int16 remote_psm)
{
#if PRH_L2CAP_AUTO_CONFIG==0
	int ret;
	
	waiting_for_configure=0x01;
	waiting_to_configure=0x01;
	
	ret=L2_Connect(remote_address,remote_psm);
	if (ret!=BT_NOERROR && ret!=BT_PENDING)
    {
		waiting_for_configure=0x00;
		waiting_to_configure=0x00;
    }
	return ret;
#else
	t_L2_ConfigReqParams req_params;
	
	
#if COMBINED_HOST==0
	req_params.inMTUmax=MGR_HintMTU(SDP_PSM);
	req_params.outMTUmax=MGR_HintMTU(SDP_PSM);
#endif
	req_params.inMTUmin=PRH_L2CAP_MTU_MIN;
	req_params.outMTUmin=PRH_L2CAP_MTU_MIN;
	req_params.flushTO=0xffff;
	req_params.linkTO=0;
	
	return L2_ConnectandConfig(remote_address, remote_psm, &req_params);
	
#endif
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_l2cap_connect_ind_cb
 * DESCRIPTION:
 
 This function will handle the callback for a connection indication. It
 will send back the response to the caller. This will also initiate the
 configuration of the link from this side.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void prh_sdp_l2cap_connect_ind_cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs)
{
	u_int16 response;
	u_int8 error_code;
#if PRH_L2CAP_AUTO_CONFIG==0
	u_int16 mtu;
#endif
	
#if PRH_L2CAP_AUTO_CONFIG==1
	t_L2_ConfigReqParams req_params;
	
#if COMBINED_HOST==0
	req_params.inMTUmax=MGR_HintMTU(SDP_PSM);
	req_params.outMTUmax=MGR_HintMTU(SDP_PSM);
#else
	req_params.inMTUmax=PRH_L2CAP_MTU_MIN;
	req_params.outMTUmax=PRH_L2CAP_MTU_MIN;
#endif
	req_params.inMTUmin=PRH_L2CAP_MTU_MIN;
	req_params.outMTUmin=PRH_L2CAP_MTU_MIN;
	req_params.flushTO=0xffff;
	req_params.linkTO=0;
#else
	mtu = MGR_HintMTU(SDP_PSM);
#endif
	
	response = 0; /* accept connection */
#if pDEBUG
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"SDP - Connection ind")); 
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,LOG_SDP_SDP_CALLBACKS_INDEX,NULL,0,0);
#endif

#if PRH_L2CAP_AUTO_CONFIG == 0
	error_code = L2_ConnectResponse(pArgs->BDAddress,pArgs->cid,response,0);
	
	if(error_code == BT_NOERROR)
    {
		error_code = L2_Configure(pArgs->cid,mtu,0x00,0x00,0x00);
		
		if(error_code == BT_NOERROR)
		{
			
		} else if (error_code == BT_RETRY) {
			/* call back another time */
		} else {
			/* error nothing I can do */
		}
    } else if (error_code == BT_RETRY) {
		/* call back another time */
    } else {
		/* error nothing I can do */
    }
#else
	error_code = L2_ConnectResponseandConfig(pArgs->BDAddress, pArgs->cid, response, 0, &req_params);
	if (error_code!=BT_NOERROR)
    {
		/* what can I do? */
    }
	
#endif
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_l2cap_data_ind_cb
 * DESCRIPTION:
 
 This function will firstly print out the contents of the recieved buffer.
 It is the callback for the reception of data for the sdp psm. It will
 then call the sdp_request dispatcher followed by the sdp_response_dispatcher.
 A pointer to the buffer the channel id are passed to the dispatchers.

 * KNOWN ISSUES:
 
 If the message is invalid it will do nothing. The flows it seems to just
 show an error being sent from the master to the slave.

 *
 *
 ***********************************************************************/
void prh_sdp_l2cap_data_ind_cb(u_int16 cid,struct host_buf * p_buf)
{  
	u_int32 len;
	u_int32 len_remaining;
	u_int8* pData;
	u_int16 mtu;
	struct host_buf* h_buf;
	
	//pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"SDP -- Received data \n"));
		
	//pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"<<< SDP Revieved from peer :\n"));
#if pDEBUG
       pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"SDP - Received data from peer :p_buf->len=%d\n", p_buf->len));	
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_CALLBACKS_INDEX+1),NULL,0,0);  
#endif
       pDebugDumpEX(pLOGDEBUG,pLOGSDP,p_buf->data,p_buf->len);

	
	prh_l2_utils_out_mtu(cid,&mtu);
	
	len = (p_buf->data[3] << 8) + p_buf->data[4] + 5;
	pData = p_buf->data;
	len_remaining = p_buf->len;

     pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"prh_sdp_l2cap_data_ind_cb len=%d, len_remaining=%d\n", len, len_remaining));	
	
	while(len <= len_remaining)
    {
#if SDP_SERVER
		prh_sdp_request_dispatcher(pData,cid);
#endif /* SDP_SERVER */
		
#if SDP_CLIENT
		prh_sdp_response_dispatcher(pData,cid);
#endif /* SDP_CLIENT*/
		
		if(len + 5 < len_remaining) 
		{
			pData +=len;
			len_remaining -=len;
			len = (pData[3] << 8) + pData[4] +5;    
		} else {
			break;
		}
    }
	
	if(len > len_remaining) /* Incorrect size */
    {
		if(*pData == 0x02 || *pData == 0x04 || *pData == 0x06)
		{
			
#if SDP_SERVER
			
			h_buf = prh_sdp_allocate_tx_buffer(mtu, cid);
			
			prh_sdp_generate_error_response(h_buf->data,(u_int16)SDP_InvalidSize,(u_int16)0,0x00,(u_int16)((pData[1] << 8) + pData[2]));
			
			prh_sdp_transfer_data(cid,h_buf);
#endif /* SDP_SERVER */
		}
    }
	
	host_buf_free(p_buf);
}


#if PRH_L2CAP_AUTO_CONFIG==0
/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_l2cap_connect_cfm_cb
 * DESCRIPTION:
 
 This function handles the connection confirm callback. It sets
 the channel id for a task and it will kick the state machine for that 
 task.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
void prh_sdp_l2cap_connect_cfm_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
#if SDP_CLIENT
	struct prh_sdp_primitive_task* p_task;
#endif /* SDP_CLIENT */
	
	u_int8 error_code;
#if pDEBUG
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"SDP - Connection conf"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_CALLBACKS_INDEX+2),NULL,0,0);
#endif
	
	error_code = (u_int8)pArgs->result;
	
#if SDP_CLIENT
	p_task = p_SDP_Primitive_Task_List;
	
	if(!p_task) return;
#endif 
	
	if(error_code == BT_NOERROR)
    { 
		error_code = L2_Configure(pArgs->cid,pTask->mtu,0x00,0x00,0x00); 
		
		if(error_code == BT_NOERROR)			
		{
			
#if SDP_CLIENT						
			p_task->cid = pArgs->cid; 
#endif /* SDP_CLIENT */
			
		} else if (error_code == BT_RETRY) {
			/* call back another time */
		} else {
			
			/* this is an error code */
			
		}            
    } else if(error_code == BT_PENDING) {      
		/* fall through as it will be called back */
    } 
#if SDP_CLIENT
	else {
		p_task->w4_l2cap_connection = 0x00;
		p_task->current_bd++;
		p_task->execute(p_task,0,NULL,NULL);
    }
#endif 
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_l2cap_config_ind_cb
 * DESCRIPTION:
 
 This function handles the config indication callback. It sends
 back a cofig response and it will also initiate a config request from 
 this side. If the requested mtu is of a smaller size then the SDP mtu will
 be changed for this session.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void prh_sdp_l2cap_config_ind_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
#if SDP_CLIENT
	struct prh_sdp_primitive_task* p_task;
#endif /* SDP_CLIENT */
	u_int16 response;
	u_int8 error_code;
	
	response = 0;
	
#if SDP_CLIENT
	p_task = p_SDP_Primitive_Task_List;
#endif /* SDP_CLIENT */

#if pDEBUG
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"SDP - config ind"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_CALLBACKS_INDEX+1),NULL,0,0);
#endif
	
	if(pArgs->mtu < p_task->mtu)
    {
		p_task->mtu = pArgs->mtu;
    }
	
	error_code = L2_ConfigureResponse(pArgs->cid,ptask->mtu,response,0x00);
	
	if(error_code == BT_NOERROR)
    {
#if SDP_CLIENT
		if(p_task != NULL && waiting_for_configure)
		{
			waiting_for_configure = 0x00;
			
			if(!waiting_to_configure)
			{
				p_task->execute(p_task,0,NULL,NULL);
			}
			
		}
#endif /* SDP_CLIENT */      
		
    } else if (error_code == BT_RETRY) {
		/* call back another time */
    } else {
		/* this is an error code */
    }  
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_l2cap_config_cfm_cb
 * DESCRIPTION:
 
 This function handles a connection confirm callback. 

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void prh_sdp_l2cap_config_cfm_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
#if SDP_CLIENT
	struct prh_sdp_primitive_task* p_task;
#endif /* SDP_CLIENT */

#if pDEBUG	
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"SDP - config conf"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_CALLBACKS_INDEX+2),NULL,0,0);
#endif
	
#if SDP_CLIENT
	p_task = p_SDP_Primitive_Task_List;
	
	if(p_task != NULL && waiting_to_configure)
    {      
		if(pArgs->mtu < ptask->mtu)
		{
			ptask->mtu = pArgs->mtu;
		}
		
		waiting_to_configure = 0;
		if(!waiting_for_configure)
		{
			p_task->execute(p_task,0,NULL,NULL);
		}    
		
    }
#endif /* SDP_CLIENT*/
}
#else /* PRH_L2CAP_AUTO_CONFIG == 1 */
/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_l2cap_connect_configured_cfm_cb
 * DESCRIPTION:
 
 This function will handle the auto configuration connect callback

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
void prh_sdp_l2cap_connect_configured_cfm_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
#if SDP_CLIENT
	struct prh_sdp_primitive_task* p_task;
#endif /* SDP_CLIENT */
#if pDEBUG
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"SDP - connected and configured cfm"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_CALLBACKS_INDEX+3),NULL,0,0);
#endif

#if SDP_CLIENT
	
	/* if we get a connection pending ignore it */
	if (pArgs->result==1)
		return;
	
	p_task = p_SDP_Primitive_Task_List;
	
	if(p_task != NULL)
    {      
		p_task->cid = pArgs->cid;  
		if(pArgs->result!=BT_NOERROR)
		{
			p_task->w4_l2cap_connection = 0x00;
			p_task->current_bd++;
			p_task->execute(p_task,0,NULL,NULL);
			return;
		}
		
		if(pArgs->mtu < p_task->mtu)
		{
			p_task->mtu = pArgs->mtu;
		}
		
		p_task->execute(p_task,0,NULL,NULL);
    }
#endif /* SDP_CLIENT*/
}
#endif

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_l2cap_disconnect_ind_cb
 * DESCRIPTION:
 
 This function will handle the disconnection indication callback.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
void prh_sdp_l2cap_disconnect_ind_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
#if SDP_CLIENT
	struct prh_sdp_primitive_task* p_task;
#endif

#if pDEBUG
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"SDP - disconnect ind"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_CALLBACKS_INDEX+4),NULL,0,0); 
#endif

#if SDP_CLIENT
	p_task = p_SDP_Primitive_Task_List;
	
	if(p_task != NULL)
	{
		p_task->w4_l2cap_connection = 0x00;
		p_task->current_bd++; /* try the next address */
		p_task->execute(p_task,0,NULL,NULL);
    }

#endif
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_l2cap_disconnect_cfm_cb
 * DESCRIPTION:
 
 This function handles the callback for a disconnection confirm 
 event.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void prh_sdp_l2cap_disconnect_cfm_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
#if pDEBUG
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"SDP - disconnect conf"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_CALLBACKS_INDEX+5),NULL,0,0);
#endif
	
#if SDP_CLIENT
	
	if(waiting_for_disconnect && p_disconnect_waiting_task)
    {
		
		p_disconnect_waiting_task->execute(p_disconnect_waiting_task,0,0x00,0x00);
		waiting_for_disconnect = 0x00;
		p_disconnect_waiting_task = 0x00;
		
    }
#endif /* SDP_CLIENT */
}



#if SDP_CLIENT


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_manager_inquiry_cb
 * DESCRIPTION:
 
 This function handles the inquiry result/complete callback. 

 * KNOWN ISSUES:

 None.
 
 *
 *
 ***********************************************************************/
void prh_sdp_manager_inquiry_cb(struct st_t_BT_DeviceEntry *deviceInfo, t_api status)
{
	struct prh_sdp_primitive_task* pTask;
	t_SDP_Addresses* pDevices;
	
	pTask = p_SDP_Primitive_Task_List;
	
	
	if(pTask)
    {
		pDevices = (t_SDP_Addresses*)pTask->app_data;
		
		if(deviceInfo)
		{
			if(deviceInfo->classOfDevice!=pTask->cod && pTask->cod)
				return;
			
			pDevices->numItems++;
			pMemcpy(&pDevices->addresses[pDevices->numItems -1],deviceInfo->address.bytes,sizeof(t_bdaddr));			
		} else {
			pTask->execute(pTask,0x00,0x00,0x00);
		}
    }  
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_manager_name_cb
 * DESCRIPTION:
 
 This function handles the RemoteName request callabck

 * KNOWN ISSUES:

 None.
 
 *
 *
 ***********************************************************************/
void prh_sdp_manager_name_cb(t_bdaddr address, t_api status)
{
	struct prh_sdp_primitive_task* pTask;
	
	pTask = p_SDP_Primitive_Task_List;
	
	if(pTask)
    {   
		if(pTask->sub_task_level_1.execute)
		{
			pTask->sub_task_level_1.state 
				++
				;
			
			pTask->sub_task_level_1.execute(pTask,0x00);
		}
    }
}

#endif /* SDP_CLIENT */


#endif /* SDP_CLIENT||SDP_SERVER */
