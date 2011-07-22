/****************************************************************
 *
 * MODULE NAME:    obex_client_interface.c
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_client_interface.c 1532 2010-08-23 05:47:34Z huazeng $
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

#if OBEX_CLIENT

#include "host_types.h"
#include "host_error.h"
#include "papi.h"
#include "sdp_le.h"
#include "obex_defines.h"
#include "obex_client_types.h"
#include "obex_client_interface.h"
#include "obex_tal.h"
#include "obex_utils.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "stack_ent_api.h"


/* 
   Globals 
*/
 
u_int8 prh_w4_obex_connect_rsp = 0;
 
extern struct prh_obex_transport* prh_obex_transmit_transport;
extern struct prh_obex_command_callback* prh_obex_client_callbacks;
extern t_pEvent prh_obex_flow_lock  ;
extern t_pEvent prh_obex_memory_lock  ;
extern int CpTimerCreate ;
extern u_int8 prh_obex_memory ;



/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_Connect
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_Connect(u_int32 tid_s, u_int8 version, u_int8 flags, u_int16 max_length, 
								 u_int16 target_length, u_int8* target, u_int16 header_length, t_DataBuf *txbuffer, t_obex_command_handler handler)
{
	u_int16 offset_len;
	struct host_buf *buf;
	int ret_value = BT_NOERROR;

	buf = (struct host_buf*)txbuffer;

	/*
	if header_length set to a valid number (7+)- use it....otherwise set it to the min
	*/
	offset_len= (header_length>OBEX_CONNECT_HEADER) ? header_length : OBEX_CONNECT_HEADER;

	if(target) /* if target put it into the buffer. */
	{
		ret_value = OBEX_CreateSequenceHeader(OBEX_TARGET,target_length,target,txbuffer,&offset_len);
		
		if (ret_value)
			return BT_INVALIDPARAM;
	}

	buf->data[0] = OBEX_CONNECT;
	/*buf->data[1] = offset_len >> 8;
	buf->data[2] = offset_len & 0xff;
	*/
	prh_sdp_write16(offset_len,buf->data + 1);
	buf->data[3] = version;
	buf->data[4] = flags;
	prh_sdp_write16(max_length,buf->data + 5);

	buf->len = offset_len;
	
	ret_value = prh_obex_create_client_callback(tid_s,handler);
	if(ret_value != BT_NOERROR)
		return ret_value;

	
	prh_w4_obex_connect_rsp = 1;

	ret_value = prh_obex_transport_tx(tid_s,txbuffer);

	/* check result*/
  pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"OBEX_Connect:  END ret_value = %d\n",ret_value));

  return ret_value;
}




/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_Disconnect
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_Disconnect(u_int32 tid_s, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler)
{
	int ret;

	if(txBuffer && handler)
	{
		
		/*
		if valid header length->use it...otherwise set it to the min
		*/
		header_length = (header_length>OBEX_HEADER) ? header_length : OBEX_HEADER;


		if(txBuffer->bufLen < (u_int32) header_length)
			return BT_INVALIDPARAM;

		txBuffer->dataLen = (u_int32) header_length;
		txBuffer->buf[0] = OBEX_DISCONNECT;
		prh_sdp_write16((u_int16)txBuffer->dataLen,txBuffer->buf + 1);
	} else {
		return BT_INVALIDPARAM;
	}

	ret = prh_obex_create_client_callback(tid_s,handler);
	if(ret != BT_NOERROR)
		return ret;

	prh_obex_transport_tx(tid_s,txBuffer);

	return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_put_or_get
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
t_api prh_obex_put_or_get(u_int8 put_or_get,u_int32 tid, u_int8 final, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler)
{
	int ret;
	
	if(txBuffer && handler)
	{
	/*
	if valid header length->use it...otherwise set it to the min
		*/
		header_length = (header_length>OBEX_HEADER) ? header_length : OBEX_HEADER;
		
		if(txBuffer->bufLen < (u_int32) header_length)
			return BT_INVALIDPARAM;
		
		txBuffer->dataLen = (u_int32) header_length;
		txBuffer->buf[0] = (put_or_get) ? (final) ? OBEX_PUT_FINAL : OBEX_PUT : (final) ? OBEX_GET_FINAL : OBEX_GET;
		prh_sdp_write16((u_int16)txBuffer->dataLen,txBuffer->buf + 1);
	} else {
		return BT_INVALIDPARAM;
	}
	
	ret = prh_obex_create_client_callback(tid,handler);
	if(ret != BT_NOERROR)
		return ret;
	
	ret = prh_obex_transport_tx(tid,txBuffer);
	if(ret != 0)
		{
    		pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"prh_obex_put_or_get:  ret =%d\n",ret ));
	return ret;
	}
	return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_Put
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_Put(u_int32 tid_s, u_int8 final, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler)
{
	return prh_obex_put_or_get(0x01,tid_s,final,header_length,txBuffer,handler);
}


/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_Get
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_Get(u_int32 tid_s, u_int8 final, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler)
{
	return prh_obex_put_or_get(0x00,tid_s,final,header_length,txBuffer,handler);
}


/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_GetMTU
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_GetMTU(u_int32 tid_s,u_int16* mtu)
{
	struct prh_obex_transport* pTransport;
	
	for(pTransport = prh_obex_transmit_transport; pTransport; pTransport = pTransport->p_next)
	{
		if(pTransport->tid == tid_s)
			break;
	}

	if(!pTransport)
		return BT_INVALIDPARAM;

	*mtu = pTransport->max_obex;
	
	return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_SetPath
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_SetPath(u_int32 tid_s, u_int8 flags, u_int8 constants, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler)
{
	int ret;

	if(txBuffer && handler)
	{
		/*
		if valid header length->use it...otherwise set it to the min
		*/
		header_length = (header_length>OBEX_SETPATH_HEADER) ? header_length : OBEX_SETPATH_HEADER;

		if(txBuffer->bufLen < (u_int32) header_length)
			return BT_INVALIDPARAM;

		txBuffer->dataLen = (u_int32) header_length;
		txBuffer->buf[0] = OBEX_SETPATH;
		prh_sdp_write16((u_int16)txBuffer->dataLen,txBuffer->buf + 1);
		txBuffer->buf[3] = flags;
		txBuffer->buf[4] = constants;
	} else {
		return BT_INVALIDPARAM;
	}

	ret = prh_obex_create_client_callback(tid_s,handler);
	if(ret != BT_NOERROR)
		return ret;

	prh_obex_transport_tx(tid_s,txBuffer);

	return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_Abort
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_Abort(u_int32 tid_s, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler)
{
	int ret;

	if(txBuffer && handler)
	{
		/*
		if valid header length->use it...otherwise set it to the min
		*/
		header_length = (header_length>OBEX_HEADER) ? header_length : OBEX_HEADER;


		if(txBuffer->bufLen < (u_int32)header_length)
			return BT_INVALIDPARAM;

		txBuffer->dataLen = (u_int32) header_length;
		txBuffer->buf[0] = OBEX_ABORT;
		prh_sdp_write16((u_int16)txBuffer->dataLen,txBuffer->buf + 1);
	} else {
		return BT_INVALIDPARAM;
	}

	ret = prh_obex_create_client_callback(tid_s,handler);
	if(ret != BT_NOERROR)
		return ret;

	prh_obex_transport_tx(tid_s,txBuffer);

	return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_TransportDisconnect
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_TransportDisconnect(u_int32 tid_s)
{
	struct prh_obex_transport* pTransport;
//	struct prh_obex_transport* pTemp;
	
	for(pTransport = prh_obex_transmit_transport; pTransport; pTransport = pTransport->p_next)
	{
		if(pTransport->tid == tid_s)
			break;
	}

	if(!pTransport)
		return BT_INVALIDPARAM;

	/*
		disconnect
	*/

	switch(pTransport->type)
	{
		case PRH_OBEX_TYPE_RFCOMM:
			RSE_CliDisconnectPort(pTransport->use.rfcomm.address,pTransport->use.rfcomm.server_channel);
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
	
    if( prh_obex_flow_lock != 0)
      	{
			pEventFree(prh_obex_flow_lock);
			prh_obex_flow_lock = 0;
      	}
	if(CpTimerCreate != 0)
	{
		pTimerCancel(CpTimerCreate);
		CpTimerCreate =0;
	}
	if(prh_obex_memory_lock != 0)
	{
		pEventFree(prh_obex_memory_lock);
		prh_obex_memory_lock = 0;
	}
	prh_obex_memory = 0;

	return BT_NOERROR;
}


#if OBEX_RFCOMM

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_TransportConnect_RF
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.2.5
APIDECL1 t_api APIDECL2 OBEX_TransportConnect_RF(t_bdaddr address,u_int8 server_channel, u_int32* tid_b, u_int16 mtu,void (*callbackFunc)(u_int32 tid_s))
{ 
	struct prh_obex_transport* pTransport;
	u_int8 match = 0;
	int ret;
	
	pTransport = prh_obex_transmit_transport;
	
	if(!pTransport)
    {
		/* allocate */
		pTransport = pMalloc(sizeof(struct prh_obex_transport));
		
		if(!pTransport)
			return BT_NORESOURCES;
		
		pTransport->p_next = 0x00;
		prh_obex_transmit_transport = pTransport;
		goto end; 
    }
	
	/* search for entry */
	
	do {
		if(pTransport->type == PRH_OBEX_TYPE_RFCOMM && 
			pTransport->use.rfcomm.server_channel == 0x00 &&
			BD_ADDR_CMP(pTransport->use.rfcomm.address,address) )
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
		
		pTransport->type = 0; /* rfcomm */
		pTransport->tid = prh_obex_allocate_tid();
		pTransport->use.rfcomm.address = address;
		pTransport->use.rfcomm.flow_state = 0x01;
		pTransport->use.rfcomm.server_channel = server_channel;
    } 
	pTransport->connect_wait = pEventCreate(FALSE);
	
	ret = prh_obex_tal_rfcomm_connect_port(address,server_channel,mtu);
	
	if(ret != BT_NOERROR && ret != BT_PENDING)
	{
		prh_obex_tal_remove_transport(pTransport);
		return ret;
	}
	

	 	
	pEventWait(pTransport->connect_wait, NULL, NULL); // TODO: 

	pEventFree(pTransport->connect_wait);


	if(!pTransport->rx_buffer)
	{
		prh_obex_tal_remove_transport(pTransport);
		return BT_UNKNOWNERROR;
	} else {
		*tid_b = pTransport->tid;
	}

	return BT_NOERROR;
	
}
#else
void (*OBEX_TransportConnect_RF_callback)(u_int32 tid);

int OBEX_TransportConnect_RF2(void *msg_p)
{ 
	u_int32 *tidp = (u_int32*)msg_p;
	struct prh_obex_transport* pTransport;
	u_int32 tid ;
	pTransport = prh_obex_transmit_transport;
	if(pTransport)
    	{
		do {
			if(pTransport->tid == *tidp )
				break;
		pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"OBEX_TransportConnect_RF2 pTransport->tid =%d, *tidp=%d\n",pTransport->tid,*tidp));		
		} while((pTransport = pTransport->p_next));
    	}
	if (pTransport)
	{
		if(pTransport->connect_wait)
		{
			pEventFree(pTransport->connect_wait);
			pTransport->connect_wait = 0;	
		}

		if(!pTransport->rx_buffer)
		{
			prh_obex_tal_remove_transport(pTransport);
			if (OBEX_TransportConnect_RF_callback != NULL)
				OBEX_TransportConnect_RF_callback( 0xffff );
			return BT_UNKNOWNERROR;
		} else {
			tid = pTransport->tid;
			pDebugPrintfEX((pLOGERROR,pLOGOBEX,"OBEX_TransportConnect_RF2\n"));
		}
	}
	if (OBEX_TransportConnect_RF_callback != NULL)
		OBEX_TransportConnect_RF_callback( tid );

	pDebugPrintfEX((pLOGERROR,pLOGOBEX,"OBEX_TransportConnect_RF2 END\n"));
	return BT_NOERROR;
}
APIDECL1 t_api APIDECL2 OBEX_TransportConnect_RF(t_bdaddr address,u_int8 server_channel, u_int32* tid_b, u_int16 mtu,void (*callbackFunc)(u_int32 tid_s))
{ 
	struct prh_obex_transport* pTransport;
	u_int8 match = 0;
	int ret;
#if pDEBUG
    pDebugPrintfEX((pLOGERROR,pLOGOBEX,"OBEX_TransportConnect_RF\n"));
//    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX_TransportConnect_RF, address %s, mtu 0x%02X\n",prh_mgr_printBDAddress(address),mtu));
#endif
	pTransport = prh_obex_transmit_transport;
	OBEX_TransportConnect_RF_callback = callbackFunc;
	if(!pTransport)
    {

        prh_obex_clean_tid();
		
		pTransport = pMalloc(sizeof(struct prh_obex_transport));
	    pDebugPrintfEX((pLOGERROR,pLOGOBEX,"pTransport pMalloc 1%x\n",pTransport));
		if(!pTransport)
			return BT_NORESOURCES;
		pTransport->p_next = 0x00;
		prh_obex_transmit_transport = pTransport;
		goto end; 
    }
	do {
		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTransport->use.rfcomm.server_channel = %d pTransport->use.rfcomm.dlci =%d\n",pTransport->use.rfcomm.server_channel,pTransport->use.rfcomm.dlci));		
		if(pTransport->type == PRH_OBEX_TYPE_RFCOMM && 
			pTransport->use.rfcomm.server_channel == server_channel &&
			BD_ADDR_CMP(pTransport->use.rfcomm.address,address) )
		{
			match = 1;
			break;
		}
		if(!pTransport->p_next)
		{
			pTransport->p_next = pMalloc(sizeof(struct prh_obex_transport));

		    pDebugPrintfEX((pLOGERROR,pLOGOBEX,"pTransport->p_next 2 %x\n",pTransport->p_next));

			if(!pTransport->p_next)
				return BT_NORESOURCES;
			pTransport = pTransport->p_next;
			pTransport->p_next = 0x00;
			break;
		}    
	} while((pTransport = pTransport->p_next));
#if pDEBUG
    pDebugPrintfEX((pLOGERROR,pLOGOBEX,"match,%x\n",match));
#endif
end:
	if(!match)
    {
		pTransport->type = 0; /* rfcomm */
		pTransport->tid = prh_obex_allocate_tid();

	    pDebugPrintfEX((pLOGERROR,pLOGOBEX,"pTransport->tid 1,%x\n",pTransport->tid));

		pTransport->use.rfcomm.address = address;
		pTransport->use.rfcomm.flow_state = 0x01;
		pTransport->use.rfcomm.server_channel = server_channel;
		pTransport->rx_buffer = 0;// I will have to think about memory here...
//		pTransport->rx_buffer = prh_obex_server_transport->rx_buffer;// I will have to think about memory here...

    } 
	*tid_b = pTransport->tid;
    pDebugPrintfEX((pLOGERROR,pLOGOBEX,"OBEX_TransportConnect_RF,tid =%d\n",pTransport->tid));
	pTransport->connect_wait = pEventCreate(FALSE);
	ret = prh_obex_tal_rfcomm_connect_port(address,server_channel,mtu);
	if(ret != BT_NOERROR && ret != BT_PENDING)
	{
		pEventFree(pTransport->connect_wait);
		pTransport->connect_wait = 0;		
		prh_obex_tal_remove_transport(pTransport);
		return ret;
	}
	pEventWait(pTransport->connect_wait, OBEX_TransportConnect_RF2, tid_b); 
	//pEventWait(pTransport->connect_wait, OBEX_TransportConnect_RF2, &tid); 
	return BT_NOERROR;
}
#endif /* OBEX_RFCOMM */



#endif /* OBEX CLIENT */
#endif /* OBEX CLIENT */
