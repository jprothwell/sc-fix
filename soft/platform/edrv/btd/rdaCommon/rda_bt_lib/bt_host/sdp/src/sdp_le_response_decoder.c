/***********************************************************************
 *
 * MODULE NAME:    sdp_le_response_decoder.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery client
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_le_response_decoder.c 1532 2010-08-23 05:47:34Z huazeng $
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

#include "sdp_le_response_decoder.h"
#include "sdp_state_machine.h"
#include "sdp_constants.h"
#include "sdp_le.h"

extern struct prh_sdp_primitive_task*  p_SDP_Primitive_Task_List;

static void sdp_execute_sub_task(u_int8 level,u_int16 transaction_id,u_int32 length, u_int8* p_data, u_int8* pContinue);



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_response_dispatcher
 * DESCRIPTION:
 
 This function will look at the pdu and call the correct decoder for the
 recieved message.

 * KNOWN ISSUES:
 
none.

 *
 *
 ***********************************************************************/
void prh_sdp_response_dispatcher(u_int8* pdu,t_channelId cid)
{
    if (!p_SDP_Primitive_Task_List) {
#if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGSDP,"prh_sdp_response_dispatcher: received response PDU with no outstanding tasks.\n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,LOG_SDP_SDP_LE_RESPONSE_DECODER,NULL,0,0);
#endif
        return;
    }

	switch(*pdu++)
    {
    case PDU_ID_SDP_ERROR_RESPONSE:
		prh_sdp_error_decode(pdu);
		break;
#if SDP_SS_AND_SA_SUPPORT
    case PDU_ID_SDP_SERVICE_SEARCH_RESPONSE:
		prh_sdp_service_response_decode(pdu,cid);
		break;
    case PDU_ID_SDP_SERVICE_ATTRIBUTE_RESPONSE:
		prh_sdp_attribute_response_decode(pdu,cid); 
		break;
#endif /* SDP_SS_AND_SA_SUPPORT */
#if SDP_SSA_SUPPORT
    case PDU_ID_SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE:
		prh_sdp_service_attribute_response_decode(pdu,cid);
		break;      
#endif /* SDP_SSA_SUPPORT */
    default:
		break;
    }
}


#if SDP_SS_AND_SA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_service_response_decode
 * DESCRIPTION:
 
 This function will partially decode the pdu and search for a matching 
 level1 sub_task and it will execute that task passing it the pdu.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_service_response_decode(u_int8* pdu,t_channelId cid)
{
	
	u_int16 transaction_id;
	u_int16 length;
	u_int16 total_count;
	u_int16 current_count;
	u_int8* Service_Record_Handles;

     #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
  u_int8* temp;
  #endif
	
	/* read a u_int16 */
	
	transaction_id = prh_sdp_read16(pdu);
	pdu += 2;
	
	length = prh_sdp_read16(pdu);
	pdu += 2;
	
	total_count = prh_sdp_read16(pdu);
	pdu += 2;
	
	current_count = prh_sdp_read16(pdu);
	pdu += 2;
	
	/*
    Next to remove the service Record Handles.
	*/
#if pDEBUG
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"<<    SDP_ServiceSearchResponse(%d ,%d, %d, %d)",transaction_id,length,total_count,current_count));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       temp=pdu-8;
           SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_LE_RESPONSE_DECODER+1),temp,8,(BT_CODE_WORD |(BT_CODE_WORD<<4)|(BT_CODE_WORD<<8) |(BT_CODE_WORD<<12)));
 #endif
 
	if(p_SDP_Primitive_Task_List->sub_task_level_1.expects == PDU_ID_SDP_SERVICE_SEARCH_RESPONSE)
	{	
		p_SDP_Primitive_Task_List->sub_task_level_1.expects = 0;
		Service_Record_Handles = pdu;
	
		pdu += current_count << 2;	
		sdp_execute_sub_task(0,transaction_id,current_count,Service_Record_Handles,(*pdu)? pdu : 0x00);
	}
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_attribute_response_decode
 * DESCRIPTION:
 
 This function will decode the pdu and serach for a level 2 task that
 has the same transaction id. It will then execute that task.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_attribute_response_decode(u_int8* pdu,t_channelId cid)
{
	u_int16 transaction_id;
	u_int16 length;
	u_int16 count;
       #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))

    u_int8* temp;
    #endif
	
	/* read a u_int16 */
	
	transaction_id = prh_sdp_read16(pdu);
	pdu += 2;
	
	length = prh_sdp_read16(pdu);
	pdu += 2;
	
	count = prh_sdp_read16(pdu);
	pdu += 2;
#if pDEBUG	
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"<<    SDP_ServiceAttributeResponse(%d ,%d, %d)",transaction_id,length,count));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        temp=pdu-6;
        SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_LE_RESPONSE_DECODER+2),temp,6,(BT_CODE_WORD |(BT_CODE_WORD<<4)|(BT_CODE_WORD<<8)));
#endif
	if(p_SDP_Primitive_Task_List->sub_task_level_2.expects == PDU_ID_SDP_SERVICE_ATTRIBUTE_RESPONSE)
	{	
		p_SDP_Primitive_Task_List->sub_task_level_2.expects = 0;
		sdp_execute_sub_task(1,transaction_id,count,pdu, pdu[count] ? pdu + count : 0x00);    
	}
}

#endif /* SDP_SS_AND_SA */

#if SDP_SSA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_service_attribute_response_decode
 * DESCRIPTION:
 
 This function will decode the pdu and it will search for the level_1
 task that matches the transaction id. This will then be passed a pointer
 to the data a pointer to the continue information.

 * KNOWN ISSUES:

 none.

 *
 *
 ***********************************************************************/
void prh_sdp_service_attribute_response_decode(u_int8* pdu,t_channelId cid)
{
	
	u_int16 transaction_id;
	u_int16 length;
	u_int16 count;

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
  u_int8* temp;
  #endif
	
	transaction_id = prh_sdp_read16(pdu);
	pdu += 2;
	
	length = prh_sdp_read16(pdu);
	pdu += 2;
	
	count = prh_sdp_read16(pdu);
	pdu += 2;
#if pDEBUG	
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"<<    SDP_ServiceSearchAttributeResponse(%d ,%d, %d)",transaction_id,length,count));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        temp=pdu-6;
        SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_LE_RESPONSE_DECODER+3),temp,6,(BT_CODE_WORD |(BT_CODE_WORD<<4)|(BT_CODE_WORD<<8)));
#endif
	
	if(p_SDP_Primitive_Task_List->sub_task_level_1.expects == PDU_ID_SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE)
	{
		p_SDP_Primitive_Task_List->sub_task_level_1.expects = 0;
		sdp_execute_sub_task(0,transaction_id,count,pdu,pdu[count] ? pdu + count : 0x00);
	}
}

#endif /* SDP_SSA_SUPPORT */

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_error_decode
 * DESCRIPTION:
 
 This function will decode an erorr pdu and pass the results back up to
 the caller.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void prh_sdp_error_decode(u_int8* pdu)
{
	u_int16 transaction_id;
	u_int16 length;
	u_int16 error;
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))

       u_int8* temp;
#endif	
	
	/* read a u_int16 */
	
	transaction_id = prh_sdp_read16(pdu);
	pdu += 2;
	length = prh_sdp_read16(pdu);
	pdu += 2;
	
	error = prh_sdp_read16(pdu);
	pdu += 2;
#if pDEBUG	
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"<<    SDP_ErrorResponse(%d ,%d, %d)",transaction_id,length,error));  
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      temp=pdu-6;
       SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_LE_RESPONSE_DECODER+4),temp,6,(BT_CODE_WORD |(BT_CODE_WORD<<4)|(BT_CODE_WORD<<8)));

#endif

	

	if( (p_SDP_Primitive_Task_List->sub_task_level_1.expects && p_SDP_Primitive_Task_List->sub_task_level_1.transaction_id == transaction_id)
		|| (p_SDP_Primitive_Task_List->sub_task_level_2.expects && p_SDP_Primitive_Task_List->sub_task_level_2.transaction_id == transaction_id) )
	{
		p_SDP_Primitive_Task_List->sub_task_level_1.expects = 0;
		p_SDP_Primitive_Task_List->sub_task_level_2.expects = 0;
	} else {
		return;
	}

	p_SDP_Primitive_Task_List->app_data->status = (u_int8)error;
	
	p_SDP_Primitive_Task_List->status = BT_PROTOERROR;
    
	if(!p_SDP_Primitive_Task_List->keep_connection)
	{
		L2_Disconnect(p_SDP_Primitive_Task_List->cid);
	} 
	
	prh_sdp_finish_primitive_task(p_SDP_Primitive_Task_List);
	
}


/***********************************************************************
 *
 * FUNCTION NAME:   sdp_execute_sub_task
 * DESCRIPTION:
 
 This function will callback to the correct sub task function.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void sdp_execute_sub_task(u_int8 level,u_int16 transaction_id,u_int32 length, u_int8* p_data, u_int8* pContinue)
{
	struct prh_sdp_primitive_task *pTask;
	struct prh_sdp_task* p_sub_task;
	
	if(level>1)
		return;
	
	for(pTask =  p_SDP_Primitive_Task_List; pTask != 0x00; pTask = pTask->p_next)
    {
		p_sub_task = level ? &pTask->sub_task_level_2 : &pTask->sub_task_level_1;
		
		if(p_sub_task->transaction_id == transaction_id)
		{
			p_sub_task->waiter(pTask,length,p_data,pContinue);
			return;
		}
    }
}


#endif /* SDP_CLIENT */
