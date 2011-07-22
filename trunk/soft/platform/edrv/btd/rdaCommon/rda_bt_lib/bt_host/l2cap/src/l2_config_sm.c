 /**************************************************************************
 * MODULE NAME:    l2_config_sm.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap channel configuration state machine
 * AUTHOR:         Gary Fleming
 * DATE:           02-06-1999
 *
 * SOURCE CONTROL: $Id: l2_config_sm.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Plc.
 *     All rights reserved.
 *
 *************************************************************************/

#include "host_config.h"
#include "host_types.h"
#include "host_error.h"

#include "l2_types.h"
#include "l2_const.h"
#include "l2_enc_dec.h"
#include "l2cap_primitives.h"
#include "l2_acl_link.h"
#include "l2_channel.h"
#include "l2_psm.h"
#include "l2_layer_config.h"
#include "l2cap_lower_layer.h"

void prh_l2_config_link_supervision_timeout_complete(tid_t transactionId, t_api status, u_int16 linkTO);
void prh_l2_config_flush_timeout_complete(tid_t transactionId, t_api status);

void prh_l2_config_build_config_req_pdu(prh_t_l2_channel* p_channel, prh_t_l2_pdu* p_pdu_info);
void prh_l2_config_send_config_req_pdu(prh_t_l2_channel* p_channel, prh_t_l2_pdu* pdu_info);

t_L2_ChannelFlowSpec *prh_l2_config_aggregate_qos(prh_t_l2_channel* p_channel);

/*********************************************************************
 *
 *  Function :-  L2_Configure
 *
 *  Input parameters
 *  u_int16 cid             The local identifier for the channel.
 *  u_int16 inMTU           Maximum transmission unit this channel can accept.
 *  u_int16 linkTimeout     Number of milliseconds to wait before terminating an
 *                          unresponsive link.
 *  u_int16 outFlushTimeout Number of milliseconds to wait before an L2CAP packet that
 *                          cannot be acknowledged at the physical layer is dropped.
 *
 *  t_L2_ChannelFlowSpec *outFlow  Quality of service parameters dealing with the traffic.
 *
 *  Returns
 *      t_api               BT_NOERROR
 *                          BT_INVALIDPARAM
 *                          BT_PENDING
 *                          
 *  Description :- 
 *  Handles an  L2 Configure Request message from the higher layers.
 *  This message is only valid in the CLOSED,CONFIG or OPEN states.
 *  It results in a L2CAP Config Request PDU being sent to the
 *  peer L2CAP layer and a state change to W4_L2CAP_CONFIG_RSP.
 *
 *********************************************************************/

APIDECL1 t_api APIDECL2 L2_Configure(u_int16 cid, u_int16 inMTU, u_int16 outFlushTimeout, u_int16 linkTimeout, t_L2_ChannelFlowSpec *outFlow)
{
    t_api status = BT_NOERROR;
    prh_t_l2_channel* p_channel;
    prh_t_l2_pdu pdu_info;

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    u_int8 temp[19];
    u_int8 temp_size=0;

    BT_WRITE_LE_UINT16(&(temp[0]),cid);
    BT_WRITE_LE_UINT16(&(temp[2]),inMTU);
    BT_WRITE_LE_UINT16(&(temp[4]),outFlushTimeout); 
    BT_WRITE_LE_UINT16(&(temp[6]),linkTimeout); 
    if(outFlow!=0)
    {
        temp[8]=LOG_EVENT_STRING_L2_CONFIG_SM_INDEX;//denote "specified"
    }
    else
    {
        temp[8]=LOG_EVENT_STRING_L2_CONFIG_SM_INDEX+1;//denote "unspecified"
    }
    temp_size=9;
#endif        
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_Configure called, cid 0x%02X, inMTU 0x%02X, outFlushTimeout 0x%02X, linkTimeout 0x%02X, outQoS %s\n",cid,inMTU,outFlushTimeout,linkTimeout,outFlow?"<specified>":"<unspecified>"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, LOG_L2CAP_L2_CONFIG_SM_INDEX, temp, temp_size,
                                                            (BT_CODE_WORD | (BT_CODE_WORD<<4) |(BT_CODE_WORD<<8)|(BT_CODE_WORD<<12) |(BT_CODE_EVENT_STRING<<16)));
#endif
    p_channel = prh_l2_chn_get_p_channel(cid);
    if (p_channel == 0)
        return BT_INVALIDPARAM;

	if (p_channel->config_req_path == PRH_L2_REQUEST_SENT)
		return BT_RETRY;

    pMemset(&pdu_info,0,sizeof(pdu_info));

    if ((p_channel->state == PRH_L2_CONFIG) || (p_channel->state == PRH_L2_OPEN))
    {
        if (inMTU != 0)
			p_channel->inMTU = inMTU;

		if (outFlushTimeout != 0)
			p_channel->outFlush = outFlushTimeout;
			
		if (outFlow != 0)
		{
			prh_l2_chn_set_qos(p_channel,outFlow);
#if COMBINED_HOST==0
            pdu_info.qos = p_channel->outQOS;
#else
            pdu_info.qos = &(p_channel->outQOS);
#endif
		}
        else
            pdu_info.qos = 0;

		pdu_info.mtu = inMTU;
		pdu_info.flush = outFlushTimeout;

        p_channel->flags = pdu_info.flags = 0; /* No Negotiation Required */

        pdu_info.length = 0x04;
        pdu_info.p_channel = p_channel;   
        
#if (L2CAP_CONFIG_VALUES_BELOW_HCI == 1)
        if ((linkTimeout != 0) && (linkTimeout != 0xFFFF))
        {
            status = prh_mgr_updateLinkSupervisionTimeout(p_channel->handle,&linkTimeout,prh_l2_config_link_supervision_timeout_complete,(tid_t)p_channel);
            if (!((status == BT_NOERROR) || (status == BT_PENDING)))
                return status;
        }
        if ((p_channel->outFlush != 0) && (status == BT_NOERROR))
        {
            status = prh_mgr_updateFlushTimeout(p_channel->handle,&p_channel->outFlush,prh_l2_config_flush_timeout_complete,(tid_t)p_channel);
            if (!((status == BT_NOERROR) || (status == BT_PENDING)))
                return status;
        }
        if (status == BT_NOERROR)
#endif
        {
            prh_l2_config_send_config_req_pdu(p_channel,&pdu_info);
        }
    }
    else
    {
        status = BT_INVALIDPARAM;
    }
    return status;
}


/*********************************************************************
 *
 *  Function :-  L2_ConfigureResponse
 *
 *  Input Parameters
 *  u_int16 cid             The local identifier for the channel.
 *  u_int16 inMTU           Maximum transmission unit this channel will send.
 *  t_L2_ChannelFlowSpec *inFlow    Quality of service parameters dealing with the traffic.
 *                                  characteristics of the incoming data,
 *  u_int16 result          Allows the upper layer to reject the configuration request,
 *
 *  Returns
 *      t_api               BT_NOERROR
 *                          BT_INVALIDPARAM
 *                          BT_NORESOURCES
 *                              
 *  Description :-
 *  Handles an  L2 Config Response message from the higher layers.
 *  This message is only valid in the CONFIG state.
 *  A positive results field cause a L2CAP Config Response PDU to be sent 
 *  to the peer L2CAP layer and a state change to CONFIG or OPEN.
 *
 *  A negative result triggers a L2CAP Config Response PDU being sent to 
 *  the peer L2CAP layer and no state change
 *
 *********************************************************************/

APIDECL1 t_api APIDECL2 L2_ConfigureResponse(u_int16 cid, u_int16 outMTU, u_int16 response, t_L2_ChannelFlowSpec *inFlow) 
{
    t_api status = BT_NOERROR;
    t_api enc_status;
    prh_t_l2_channel* p_channel; 
    prh_t_l2_pdu pdu_info;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_ConfigureResponse called, cid 0x%02X, outMTU 0x%02X, response 0x%02X, inQoS %s",cid,outMTU,response,inFlow?"<specified>":"<unspecified>"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)

    u_int8 temp[17];
    u_int8 temp_size=0;


    BT_WRITE_LE_UINT16(&(temp[0]),cid);
    BT_WRITE_LE_UINT16(&(temp[2]),outMTU);
    BT_WRITE_LE_UINT16(&(temp[4]),response); 
   
    if(inFlow!=NULL)
    {
        temp[6]=LOG_EVENT_STRING_L2_CONFIG_SM_INDEX;//denote "specified"
    }
    else
    {
        temp[6]=LOG_EVENT_STRING_L2_CONFIG_SM_INDEX+1;//denote "unspecified"
    }
    temp_size=7;
        
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONFIG_SM_INDEX+1), temp, temp_size,
                          (BT_CODE_WORD | (BT_CODE_WORD<<4) |(BT_CODE_WORD<<8)|(BT_CODE_EVENT_STRING<<12)));    
#endif
    p_channel = prh_l2_chn_get_p_channel(cid);
    if (p_channel == 0)
        return BT_INVALIDPARAM;

    pMemset(&pdu_info,0,sizeof(pdu_info));

    if(PRH_L2_CONFIG == p_channel->state) 
    {
        /***********************************/
        /* Send an Configure Response PDU  */
        /***********************************/

        pdu_info.mtu = outMTU;
        pdu_info.flush = 0;  /* Temp Only */
        pdu_info.qos = inFlow;
        pdu_info.length = 0x06;
        pdu_info.p_channel = p_channel;   
        pdu_info.result = response;
        if(p_channel->config_rsp_path != PRH_L2_REQUEST_RECEIVED)
            pdu_info.flags = PRH_L2_CONFIG_CONTINUE_ON; /* Continue Bit Set */
        else
            pdu_info.flags = PRH_L2_CONFIG_CONTINUE_OFF; /* Continue Bit Not Set */

        pdu_info.num_options = 0;
        if (outMTU != 0 )
        {
            pdu_info.length+=4;
            pdu_info.num_options++;
        }
        if (inFlow != 0 )
        {
            pdu_info.length+=24;
            pdu_info.num_options++;
#if COMBINED_HOST==0
            p_channel->inQOS = pMalloc(sizeof(t_L2_ChannelFlowSpec));
            if (p_channel->inQOS)
            {
                p_channel->inQOS->flags = inFlow->flags;
                p_channel->inQOS->bucketSize = inFlow->bucketSize;
                p_channel->inQOS->delayVariation = inFlow->delayVariation;
                p_channel->inQOS->latency = inFlow->latency;
                p_channel->inQOS->peakBandwidth = inFlow->peakBandwidth;
                p_channel->inQOS->serviceType = inFlow->serviceType;
                p_channel->inQOS->tokenRate = inFlow->tokenRate;
            }
			else
			{
				return BT_NORESOURCES;
			}
#else
            {
                p_channel->inQOS.flags = inFlow->flags;
                p_channel->inQOS.bucketSize = inFlow->bucketSize;
                p_channel->inQOS.delayVariation = inFlow->delayVariation;
                p_channel->inQOS.latency = inFlow->latency;
                p_channel->inQOS.peakBandwidth = inFlow->peakBandwidth;
                p_channel->inQOS.serviceType = inFlow->serviceType;
                p_channel->inQOS.tokenRate = inFlow->tokenRate;
            }
#endif

        }

        enc_status = prh_l2_encode_packet(p_channel->handle,L2CAP_CONFIGURE_RESPONSE, &pdu_info);            
#if 1
  		/* If memory was allocated for the incoming QoS free it here */
#if COMBINED_HOST==0
  		if ((inFlow != 0) && (p_channel->inQOS != 0))
		{
  			pFree(p_channel->inQOS);
            p_channel->inQOS = 0;
		}
#endif
#endif


        if (enc_status != BT_NOERROR)
            return BT_NORESOURCES;
        
        if ( L2CAP_CONFIG_SUCCESSFUL== response) 
        {
            if ( p_channel->config_rsp_path != PRH_L2_CONFIG_START)
            {
                p_channel->config_rsp_path = PRH_L2_CONFIG_END;
                if(outMTU == 0)
                    p_channel->outMTU = PRH_L2_DEFAULT_MTU;
                else
                    p_channel->outMTU = outMTU;

                if(PRH_L2_CONFIG_END == p_channel->config_req_path)  
                {
                    pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2_ConfigureResponse p_channel->state=PRH_L2_OPEN"));
                    prh_l2_chn_stop_cfg_timer(p_channel);
                    p_channel->state = PRH_L2_OPEN; 
                }
            }
            else  /* More Configuration Requests Expected */
            { 
                if(outMTU == 0)
                    p_channel->outMTU = PRH_L2_DEFAULT_MTU;
                else
                    p_channel->outMTU = outMTU;
            }
        } 
        else 
            p_channel->config_rsp_path = PRH_L2_CONFIG_START;
    } 
    else 
    {
        status = BT_INVALIDPARAM;
    }
    return status;
}


#if (L2CAP_CONFIG_VALUES_BELOW_HCI==1)
/*********************************************************************
 *
 *  Function :- prh_l2_config_link_supervision_timeout_complete
 *
 *  Input Parameters
 *      tid_t transactionId     A transaction identifier used to derive the p_channel.
 *      t_api status            The outcome of the operation perform in the manager.
 *      u_int16 linkTimeout     The current value of the link supervision timeout.
 *
 *  Returns
 *      void
 *
 *  Description :-
 *  This function is called back when the manager has completed the
 *  the operation to write the link supervision timeout.
 *
 *********************************************************************/

void prh_l2_config_link_supervision_timeout_complete(tid_t transactionId, t_api status,u_int16 linkTimeout)
{
    prh_t_l2_channel* p_channel;
    prh_t_l2_pdu pdu_info;
    t_L2_ControlCallbackArgs  eventinfo;
    u_int8 psm_index;

    p_channel = (prh_t_l2_channel*)transactionId;
    if (p_channel == 0)
    {
#if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGL2CAP," Invalid Transaction Id return in Callback - p_channel = NULL  \n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONFIG_SM_INDEX+2), NULL, 0,0);    
#endif
        return;
    }

    pMemset(&pdu_info,0,sizeof(pdu_info));
    pMemset(&eventinfo,0,sizeof(eventinfo));

    if (status != BT_NOERROR)
    {
        /* Callback the higher layer with an L2_Configure Cmf */
        eventinfo.result = L2CAP_CONFIG_FAILURE_UNACCEPTABLE_PARAMS;
        eventinfo.cid = p_channel->local_cid;
        psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
        if (psm_index != 0)
            prh_l2_psm_event(psm_index,L2CAP_evCONFIGURECONF,&eventinfo);
        return;
    }
    else if (p_channel->outFlush != 0)
    {
        status = prh_mgr_updateFlushTimeout(p_channel->handle,&p_channel->outFlush,prh_l2_config_flush_timeout_complete,(tid_t)p_channel);
        if ((status != BT_NOERROR) && (status != BT_PENDING))
        {
            /* Callback the higher layer with an L2_Configure Cmf */
            eventinfo.result = L2CAP_CONFIG_FAILURE_UNACCEPTABLE_PARAMS;
            eventinfo.cid = p_channel->local_cid;
            eventinfo.flushTimeout = p_channel->outFlush;
            psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
            if (psm_index != 0)
                prh_l2_psm_event(psm_index,L2CAP_evCONFIGURECONF,&eventinfo);
            return;
        }
    } 
    else if ((p_channel->outQOS != 0) && (status == BT_NOERROR))
    {
        /* No Devices Support This functionality Yet */
    }
    if (status == BT_NOERROR)
	{
		pdu_info.mtu = p_channel->inMTU;
        prh_l2_config_send_config_req_pdu(p_channel,&pdu_info);
	}
}


/*********************************************************************
 *
 *  Function :- prh_l2_config_flush_timeout_complete
 *
 *  Input Parameters
 *      tid_t transactionId     A transaction identifier used to derive the p_channel.
 *      t_api status            The outcome of the operation perform in the manager.
 *      u_int16 linkTimeout     The current value of the link supervision timeout.
 *
 *  Returns
 *      void
 *
 *  Description :-
 *  This function is called back when the manager has completed the
 *  the operation to write the flush timeout.
 *
 *********************************************************************/

void prh_l2_config_flush_timeout_complete(tid_t transactionId, t_api status)
{
    prh_t_l2_channel* p_channel;
    prh_t_l2_pdu pdu_info;
    t_L2_ChannelFlowSpec *aggregate_flow;
    t_L2_ControlCallbackArgs  eventinfo;
    u_int8 psm_index;


    p_channel = (prh_t_l2_channel*)transactionId;

    if (p_channel == 0)
    {
#if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGL2CAP," Invalid Transaction Id return in Callback - p_channel = NULL"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONFIG_SM_INDEX+2), NULL, 0,0);
#endif

        return;
    }

    pMemset(&pdu_info,0,sizeof(pdu_info));
    pMemset(&eventinfo,0,sizeof(eventinfo));

    if (status != BT_NOERROR)
    {
        /* Callback the higher layer with an L2_Configure Cmf */
        eventinfo.result = L2CAP_CONFIG_FAILURE_UNACCEPTABLE_PARAMS;
        eventinfo.cid = p_channel->local_cid;
        eventinfo.flushTimeout = p_channel->outFlush;
        psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
        if (psm_index != 0)
            prh_l2_psm_event(psm_index,L2CAP_evCONFIGURECONF,&eventinfo);
    }
    else if (p_channel->outQOS != 0)
    {
        /* Aggregate QoS accross all Channels on the Handle */

        aggregate_flow = prh_l2_config_aggregate_qos(p_channel);
		pdu_info.qos = p_channel->outQOS;

    }
    if (status == BT_NOERROR)
	{
		pdu_info.mtu = p_channel->inMTU;
		pdu_info.flush = p_channel->outFlush;
        prh_l2_config_send_config_req_pdu(p_channel,&pdu_info);
	}
}

#endif
/*********************************************************************
 *
 *  Function :- prh_l2_config_send_config_req_pdu
 *
 *  Input Parameters
 *      prh_t_l2_channel* p_channel :- A pointer to the channel.
 *      prh_t_l2_pdu* p_pdu_info    :- A pointer to a structure containing
 *                                     the elements of the PDU. 
 *  Returns
 *      void
 *
 *  Description :-
 *  This function builds a config request PDU and interacts with the 
 *  encode module to encode it into an L2CAP signalling packet.
 *
 *********************************************************************/

void prh_l2_config_send_config_req_pdu(prh_t_l2_channel* p_channel, prh_t_l2_pdu* p_pdu_info)
{

    #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    u_int8 temp[2];    
    #endif

    
    prh_l2_config_build_config_req_pdu(p_channel,p_pdu_info);
    p_channel->local_transaction_identifier = prh_l2_enc_derive_identifier();

         
    if (p_channel->state == PRH_L2_OPEN)
    {
        p_channel->state = PRH_L2_CONFIG;
    }

	if (((p_channel->config_req_path == PRH_L2_CONFIG_END) || (p_channel->config_req_path == PRH_L2_CONFIG_START)) &&
		((p_channel->config_rsp_path == PRH_L2_CONFIG_END) || (p_channel->config_rsp_path == PRH_L2_CONFIG_START)))
        prh_l2_chn_start_cfg_timer(p_channel);

    p_channel->config_req_path = PRH_L2_REQUEST_SENT;
    prh_l2_encode_packet(p_channel->handle,L2CAP_CONFIGURE_REQUEST, p_pdu_info);
#if pDEBUG
    pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"Channel %x -Config_Req Changed to Request_Sent", p_channel->local_cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    BT_WRITE_LE_UINT16(temp,p_channel->local_cid);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONFIG_SM_INDEX+3), temp, 2,BT_CODE_WORD);
#endif
}

/*********************************************************************
 *
 *  Function :- prh_l2_config_build_config_req_pdu
 *
 *  Input Parameters
 *      prh_t_l2_channel* p_channel :- A pointer to the channel.
 *      prh_t_l2_pdu* p_pdu_info    :- A pointer to a structure containing
 *                                     the elements of the PDU.         
 *  Returns
 *      void
 *
 *  Description :-
 *  This function builds a config request PDU.
 *
 *********************************************************************/


void prh_l2_config_build_config_req_pdu(prh_t_l2_channel* p_channel, prh_t_l2_pdu* p_pdu_info)
{
    p_pdu_info->flags = 0; /* No Negotiation Required */
    p_pdu_info->length = 0x04;
    p_pdu_info->p_channel = p_channel;   
        
    if (p_pdu_info->mtu != 0)
    {
        p_pdu_info->num_options++;
        p_pdu_info->length+=4;
        if ( PRH_L2_USE_C_BIT)
        {
           p_channel->inMTU = 0;
           return;
        }
    }
    if (p_pdu_info->flush != 0)
    {
        p_pdu_info->num_options++;
        p_pdu_info->length+=4;
        if (PRH_L2_USE_C_BIT)
        {
           p_channel->outFlush = 0;
           return;
        }
    }
    if (p_pdu_info->qos != 0)
    {
        p_pdu_info->num_options++;
        p_pdu_info->length+=24;
        if (PRH_L2_USE_C_BIT)
        {
#if COMBINED_HOST==0
           p_channel->outQOS = 0;
#endif
           return;
        }
    }
}

/*******************************************************************
 *
 * Function :- prh_l2_config_aggregate_qos
 *
 * Description
 * This function determines the required QoS on a handle to support
 * the QoS on a single channel.
 *******************************************************************/

t_L2_ChannelFlowSpec *prh_l2_config_aggregate_qos(prh_t_l2_channel* p_channel)
{
#if (L2CAP_AGGREGATE_QOS == 1)
    u_int16 handle;
    prh_t_l2_acl_link* p_link;
    t_L2_ChannelFlowSpec *flow;
#endif

#if (L2CAP_AGGREGATE_QOS == 1)
#if COMBINED_HOST==0
    flow = pMalloc(sizeof(t_L2_ChannelFlowSpec));
    if (!flow)
		return 0;
#endif
    handle = p_channel->handle;

    p_link = prh_l2_acl_find_handle(handle);
    if (p_channel->outQOS->serviceType > p_link->flow->serviceType)
        flow->serviceType = p_channel->outQOS->serviceType;

    flow->tokenRate = p_link->flow->tokenRate + p_channel->outQOS->tokenRate;
    flow->bucketSize = p_link->flow->bucketSize + p_channel->outQOS->bucketSize;
    flow->peakBandwidth =  p_link->flow->peakBandwidth + p_channel->outQOS->peakBandwidth;
    flow->latency =  (p_link->flow->latency * p_channel->outQOS->latency)/(p_link->flow->latency + p_channel->outQOS->latency);
    flow->delayVariation = (p_link->flow->delayVariation * p_channel->outQOS->delayVariation)/(p_link->flow->delayVariation + p_channel->outQOS->delayVariation);

    return flow;
#else
#if COMBINED_HOST==0
    return p_channel->outQOS;
#else
    return &(p_channel->outQOS);
#endif
#endif
}
