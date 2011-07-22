/****************************************************************
 *
 * MODULE NAME:    obex_server_interface.c
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_server_interface.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/
 
#include "host_config.h"

#if OBEX_SERVER

#include "host_types.h"
#include "host_error.h"
#include "papi.h"
#include "sdp_le.h"
#include "obex_defines.h"
#include "obex_server_types.h"
#include "obex_server_interface.h"
#include "obex_client_types.h"
#include "obex_tal.h"

#include "rfcomm_types.h"


/* 
   Globals 
*/
struct prh_obex_session_state* prh_obex_state;
extern struct prh_obex_transport  server_transport[2] ;
extern struct prh_obex_transport* prh_obex_server_transport;
extern struct prh_obex_transport* prh_obex_transmit_transport;
struct prh_obex_service_registration *obex_registered_services = 0x00;
struct prh_obex_inbox_registration *obex_registered_inboxes = 0x00;
extern u_int8 prh_obex_connecting;


u_int8 prh_obex_compair_registration(struct prh_obex_service_registration* a, u_int8 target_length, u_int8* b);
int prh_obex_allocate_cid(struct prh_obex_service_registration* registration);
APIDECL1 t_api APIDECL2 RSE_SrvDisconnectPort(prh_t_rfcomm_schan schan);




/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_RegisterInbox
 * DESCRIPTION:
 
 This function will register the inbox handler for a transport. There
 can only be one inbox handler per transport.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_RegisterInbox(u_int32 tid_s,t_obex_service_handler handler, void *cref)
{
	struct prh_obex_inbox_registration *pRegistration;
	struct prh_obex_transport* pTransport;
	
	pTransport = prh_obex_server_transport;


	/* check parameters */

	/* does the transport exist */

	if(pTransport)
	{
		do {
			if(pTransport->tid == tid_s)
				break;
		} while((pTransport = pTransport->p_next));
	}
 
	if(!pTransport)
		return BT_INVALIDPARAM;

	pRegistration = obex_registered_inboxes;

	if(pRegistration)
	{
		do {
			if(pRegistration->tid == tid_s)
				break;
		} while ((pRegistration = pRegistration->p_next));
	}

	if(!pRegistration)
	{
		pRegistration = pMalloc(sizeof(struct prh_obex_inbox_registration));

		if(!pRegistration)
			return BT_NORESOURCES;	
	}

	pRegistration->handler = handler;
	pRegistration->tid = tid_s;
	pRegistration->p_next = obex_registered_inboxes;
	obex_registered_inboxes = pRegistration;


	return BT_NOERROR;
}


APIDECL1 t_api APIDECL2 OBEX_RegisterServerToInbox(u_int32 tid_s, u_int8 type, t_obex_service_handler handler)
{
	return BT_UNSUPPORTED;
}

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_RegisterService
 * DESCRIPTION:
 
 This function 

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_RegisterService(u_int32 tid_s,t_obex_service_handler handler, u_int8 length, u_int8* target, void* cref)
{
  struct prh_obex_service_registration *p_temp, *p_new;
	
  /* check arguments */
	
  p_temp = obex_registered_services;
	
  if(p_temp)
	{
tim1:
	if(prh_obex_compair_registration(p_temp,length,target) == BT_NOERROR)
	{
		return BT_HOSTERROR;
	} else {
		if(p_temp->p_next)
		{
			p_temp = p_temp->p_next;
			goto tim1;	 
		} else {
			p_new = pMalloc(sizeof(struct prh_obex_service_registration));
			
			if(!p_new)
				return BT_NORESOURCES;
			
			p_new->handler = handler;
			p_new->tid = tid_s;
			p_new->target = pMalloc(length);
			pMemcpy(p_new->target,target,length);
			p_new->target_length = length;
			p_new->cid = 0x00;
			p_new->p_next = 0x00;
			
			p_temp->p_next = p_new;
		}
	}
	} else {
		p_temp = pMalloc(sizeof(struct prh_obex_service_registration));
		
		if(!p_temp)
			return BT_NORESOURCES;
		
		p_temp->handler = handler;
		p_temp->tid = tid_s;
		//p_temp->target = target;
		p_temp->target = pMalloc(length);
		pMemcpy(p_temp->target,target,length);
		p_temp->target_length = length;
		p_temp->cid = 0x00;
		p_temp->p_next = 0x00;
		
		obex_registered_services = p_temp;
	}
	
  return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_Response
 * DESCRIPTION:
 
 This function 

 * KNOWN ISSUES:

 it does not support any additional headers. Which I think that it 
 should for completeness.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_Response(u_int32 tid_s, u_int8 response_code, u_int16 header_length, t_DataBuf* txbuffer)
{
	
	/* check parameters */
	
	struct  prh_obex_session_state *pState;
	
	pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"OBEX_Response : header_length == %x\n",header_length));
	
	if(!txbuffer)
		{
		pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"OBEX_Response :txbuffer == 0\n"));
		return BT_INVALIDPARAM;
		}
	pState = prh_obex_state;
	
	if(!pState)
		{
		pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"OBEX_Response : 1 prh_obex_state == 0\n"));
		return BT_INVALIDPARAM;
		}
	/* search for the state that matches with the tid */
	
	do{
		if(pState->tid == tid_s)
			break;
	} while( (pState = pState->p_next) );
		
	if(!pState)
		{
		pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"OBEX_Response : 2 prh_obex_state == 0\n"));
		return BT_INVALIDPARAM;
		}


	if(txbuffer->bufLen < (u_int32) header_length)
		{
		pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"OBEX_Response :txbuffer->bufLen == %x\n",txbuffer->bufLen));
		pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"OBEX_Response :header_length == %x\n",header_length));
		return BT_INVALIDPARAM;
		}
	
	txbuffer->dataLen = (header_length>OBEX_HEADER) ? header_length : OBEX_HEADER;

		pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"OBEX_Response :txbuffer->dataLen == %x\n",txbuffer->dataLen));
	
	txbuffer->buf[0] = response_code;
	prh_sdp_write16((u_int16)txbuffer->dataLen,txbuffer->buf + 1);	
	
	switch(pState->state)
	{
	case PRH_OBEX_STATE_START:
		/* send message */
		pState->handler = 0x00;
		break;
	case PRH_OBEX_STATE_CONTINUED:
		if(response_code != OBEX_CONTINUE)
		{
			pState->state = PRH_OBEX_STATE_START;
			pState->handler = 0x00;
		} 
		break;
	case PRH_OBEX_STATE_CONTINUED_GET:
		if(response_code != OBEX_CONTINUE)
		{
			pState->state = PRH_OBEX_STATE_START;
			pState->handler = 0x00;
		}
		break;
	default:
		/* error reset state*/
		
		pState->state = PRH_OBEX_STATE_START;
		pState->handler = 0;
		break;
	}
	
	return prh_obex_transport_tx(tid_s,txbuffer);	
}

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_ConnectResponse
 * DESCRIPTION:
 
 This function 

 * KNOWN ISSUES:

	none.
  
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_ConnectResponse(u_int32 tid_s, u_int8 response_code, u_int8 version, u_int8 flags,u_int16 maxLen, u_int16 header_length, t_DataBuf* txbuffer)
{
	u_int8 status;

	txbuffer->buf[3] = version;
	txbuffer->buf[4] = flags;
	prh_sdp_write16(maxLen,txbuffer->buf + 5);

	if(header_length < OBEX_CONNECT_HEADER)
		header_length=OBEX_CONNECT_HEADER;


	status=OBEX_Response(tid_s, response_code, header_length, txbuffer);

	return status;
}


/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_GetCID
 * DESCRIPTION:
 
 This function 

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_GetCID(u_int32 tid_s,u_int8 target_length,u_int8* target,u_int32* cid)
{
	struct prh_obex_service_registration *p_temp;
	
	/* check arguments */
	
	p_temp = obex_registered_services;
	
	if(p_temp)
	{
tim1:
	if(!prh_obex_compair_registration(p_temp,target_length,target))
	{
		if(p_temp->cid)
		{
			*cid = p_temp->cid;
		} else {
			if(prh_obex_allocate_cid(p_temp) == BT_NOERROR)
			{
				*cid = p_temp->cid;
			} else return BT_NORESOURCES;
		}
	} else {
		if(p_temp->p_next)
		{
			p_temp = p_temp->p_next;
			goto tim1;
		}
	}
	} else {
		return BT_UNKNOWNERROR;
	}
	
	return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_ServerTransportDisconnect
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_ServerTransportDisconnect(u_int32 tid_s)
{
	struct prh_obex_transport* pTransport;
//	struct prh_obex_transport* pTemp;
	
	for(pTransport = prh_obex_transmit_transport; pTransport; pTransport = pTransport->p_next)
	{
		if(pTransport->tid == tid_s)
			break;
	}
    pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"OBEX_ServerTransportDisconnect:\n"));

	if(!pTransport)
		return BT_INVALIDPARAM;
    pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"OBEX_ServerTransportDisconnect  NOT  BT_INVALIDPARAM:\n"));

	/*
		disconnect
	*/

	switch(pTransport->type)
	{
		case PRH_OBEX_TYPE_RFCOMM:
			pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"RSE_SrvDisconnectPort  server_channel == %x:\n",pTransport->use.rfcomm.server_channel));
			
			RSE_SrvDisconnectPort(pTransport->use.rfcomm.server_channel);
			break;
	}

	/*
		remove from the list
	*/
/*
		if(pTransport == prh_obex_transmit_transport)
		{
			prh_obex_transmit_transport = pTransport->p_next;			
		} else { 
			for(pTransport = prh_obex_transmit_transport; pTransport; pTransport = pTransport->p_next)
			{
				if(pTransport->p_next->tid == tid)
				{
					pTemp = pTransport;
					pTemp->p_next = pTransport->p_next->p_next;
					pTransport = pTransport->p_next;
				}
			}
		}
		
		pFree(pTransport);
*/
	prh_obex_tal_remove_transport(pTransport);

	return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_allocate_cid
 * DESCRIPTION:
 
 This function 

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
int prh_obex_allocate_cid(struct prh_obex_service_registration* registration)
{
  static u_int32 cid=1;  

  registration->cid = cid++;

  return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_compair_registration
 * DESCRIPTION:
 
 This function 

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int8 prh_obex_compair_registration(struct prh_obex_service_registration* a, u_int8 target_length, u_int8* b)
{	
	if(a)
    {
		if(a->target_length == target_length)
		{
			if(pMemcmp(a->target,b,a->target_length))
				return BT_UNKNOWNERROR;

			return BT_NOERROR;
		} else {
			return BT_UNKNOWNERROR;
		}
    } else {
		return BT_UNKNOWNERROR;
    }
}

#if OBEX_RFCOMM

#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.3.11
/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_TransportListen_RF
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_TransportListen_RF(u_int8 server_channel,u_int32* tid_b, void (*disconnect)(u_int32 tid_s))
{
	struct prh_obex_transport* pTransport;
	u_int8 match = 0;
	
	pTransport = prh_obex_server_transport;
	
	if(!pTransport)
    {
		/* allocate */
		pTransport = pMalloc(sizeof(struct prh_obex_transport));
		
		if(!pTransport)
			return BT_NORESOURCES;
		pTransport->p_next = 0x00;
		prh_obex_server_transport  = pTransport;
		
		goto end; 
    }
	
	/* search for entry */
	
	do {
		if(pTransport->type == 0 && pTransport->use.rfcomm.server_channel == server_channel)
		{
			match = 1;
			break;
		}
		
		if(!pTransport->p_next)
		{
			/* end of list */
			pTransport->p_next = pMalloc(sizeof(struct prh_obex_transport));
			
			if(!pTransport->p_next)
				return BT_NORESOURCES;
			
			pTransport = pTransport->p_next;
			pTransport->p_next = 0x00;
			
			break;
		}    
	} while((pTransport = pTransport->p_next));
	
end:
	
	if(!match)
    {
		/* a new one */      
		pTransport->type = 0;
		pTransport->tid = prh_obex_allocate_tid();
		/*  prh_obex_t_list.rf.address*/
		pTransport->use.rfcomm.server_channel = server_channel;
    } 
	
	pTransport->use.rfcomm.flow_state = PRH_RFCOMM_FLOW_DATA_GO; /* it might not be needed here */
	pTransport->use.rfcomm.disconnect = disconnect;
	prh_obex_tal_rfcomm_reg_port(server_channel);
	
	*tid_b = pTransport->tid;
	
	return BT_NOERROR;
}


#else
			
/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_TransportListen_RF
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_TransportListen_RF(u_int8 server_channel,u_int32* tid_b, u_int16 max_obex,u_int8* rx_buffer,void (*disconnect)(u_int32 tid_s), void (*callbackFunc)(t_bdaddr address),void (*authorizecallbackFunc)(t_bdaddr address, u_int8 dlci, u_int8 schan, u_int8 tid_s))
{
	struct prh_obex_transport* pTransport;
//	u_int8 match = 0;
	
	pTransport = prh_obex_server_transport;

	pDebugPrintfEX((pLOGDEBUG,pLOGOBEX, "OBEX_TransportListen_RF  server_channel =%x\n",server_channel));    


	if(RDA_FTPS_Server_Channel == server_channel)
	{
		pTransport = prh_obex_server_transport;
		pTransport->p_next = server_transport+1;
	}
	if(RDA_OPPS_Server_Channel == server_channel)
	{
		pTransport = server_transport+1;
		pTransport->p_next = 0;
	}

/*
	if(!pTransport)
    {
	pDebugPrintfEX((pLOGDEBUG,pLOGOBEX, "OBEX_TransportListen_RF 1 server_channel =%x\n",server_channel));    
		pTransport = pMalloc(sizeof(struct prh_obex_transport));
		
		if(!pTransport)
			return BT_NORESOURCES;
		pTransport->p_next = 0x00;
		prh_obex_server_transport  = pTransport;
		
		goto end; 
    }

	pDebugPrintfEX((pLOGDEBUG,pLOGOBEX, "OBEX_TransportListen_RF 2 server_channel =%x\n",server_channel));    

	
	do {
		if(pTransport->type == 0 && pTransport->use.rfcomm.server_channel == server_channel)
		{
			match = 1;
			break;
		}
		
		if(!pTransport->p_next)
		{
	pDebugPrintfEX((pLOGDEBUG,pLOGOBEX, "OBEX_TransportListen_RF 3 server_channel =%x\n",server_channel));    
			pTransport->p_next = pMalloc(sizeof(struct prh_obex_transport));
			
			if(!pTransport->p_next)
				return BT_NORESOURCES;
			
			pTransport = pTransport->p_next;
			pTransport->p_next = 0x00;
			
			break;
		}    
	} while((pTransport = pTransport->p_next));
	
end:
*/
	pDebugPrintfEX((pLOGDEBUG,pLOGOBEX, "OBEX_TransportListen_RF 4 server_channel =%x\n",server_channel));    

//	if(!match)
    {
		/* a new one */      
		pTransport->type = 0;
		pTransport->tid = prh_obex_allocate_tid();
		/*  prh_obex_t_list.rf.address*/
		pTransport->use.rfcomm.server_channel = server_channel;
    } 
	pDebugPrintfEX((pLOGDEBUG,pLOGOBEX, "OBEX_TransportListen_RF 5 server_channel =%x\n",server_channel));    

	pTransport->use.rfcomm.flow_state = PRH_RFCOMM_FLOW_DATA_GO; /* it might not be needed here */
	pTransport->use.rfcomm.disconnect = disconnect;
	prh_obex_tal_rfcomm_reg_port(server_channel,max_obex,rx_buffer,authorizecallbackFunc);
	
	*tid_b = pTransport->tid;
	pTransport->max_obex = max_obex; // this will change after a connect
	pTransport->rx_buffer = (u_int8*)rx_buffer;
	
	return BT_NOERROR;
}

#endif

#endif /* OBEX RFCOMM */

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_GetConnectionInfo_RF
 * DESCRIPTION:
 
 This function 

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_GetConnectionInfo_RF(u_int32 tid_s,t_bdaddr *address,u_int8 *server_channel)
{
	int ret_value;

	struct prh_obex_transport* pTransport;

	ret_value = BT_NOERROR;

	pTransport = prh_obex_transmit_transport;
	
	if(pTransport)
    {
		do {
			if(pTransport->tid == tid_s)
				break;
			
		} while((pTransport = pTransport->p_next));
    }
	
	if(pTransport)
	{
		if(pTransport->type == PRH_OBEX_TYPE_RFCOMM)
		{
			*address = pTransport->use.rfcomm.address;
			*server_channel = pTransport->use.rfcomm.server_channel;
		} else {
			ret_value = BT_INVALIDPARAM;			
		}
	} else {
		ret_value = BT_INVALIDPARAM;
	}

	return ret_value;
}


#endif /* OBEX SERVER */
