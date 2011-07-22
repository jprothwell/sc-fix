/****************************************************************
 *
 * MODULE NAME:    obex_server_tal.c
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    transport abstraction layer
 * MAINTAINER:     Mark Donnelly, kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_tal.c 1532 2010-08-23 05:47:34Z huazeng $
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
#include "obex_defines.h"
#include "obex_server_types.h"
#include "obex_client_types.h"
#include "obex_server_interface.h"
#include "obex_tal.h"
#include "obex_utils.h"

#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "stack_ent_api.h"

/* 
   Globals  
*/
struct prh_obex_transport  server_transport[2] ;
struct prh_obex_transport* prh_obex_server_transport = server_transport;
struct prh_obex_transport* prh_obex_transmit_transport = 0x00;
struct prh_obex_command_callback* prh_obex_client_callbacks = 0x00;
extern struct prh_obex_session_state* prh_obex_state;
extern struct prh_obex_service_registration *obex_registered_services;
extern struct prh_obex_inbox_registration *obex_registered_inboxes;
void rdabt_send_notify(void);

extern u_int8 prh_w4_obex_connect_rsp;

u_int8 prh_obex_connecting = 0;

t_pEvent prh_obex_flow_lock =0 ;
t_pEvent prh_obex_memory_lock =0 ;
u_int8 prh_obex_flow;
u_int8 prh_obex_memory = 0;

#if (SINGLE_THREAD_FLAG!=0)  // jiancui added it. 2008.3.11
void (*OBEX_TransportListen_AUTHORIZE_callback)(t_bdaddr address, u_int8 dlci, u_int8 schan,u_int8 tid_s);
static u_int32 OBEX_tid = 0;
int CpTimerCreate =0;

int OBEX_remove_all_server_transport(void )
{
  struct prh_obex_transport *trav, *prev;

  trav=prev=prh_obex_server_transport;

  if (trav==pNULL)
    return BT_NOERROR;

  do
    {
      prev=trav;
      trav=trav->p_next;

      pFree(prev);

    } while(trav!=pNULL);
  
  return 0;
}

int OBEX_remove_all_transmit_transport(void)
{
  struct prh_obex_transport *trav, *prev;

  trav=prev=prh_obex_transmit_transport;

  if (trav==pNULL)
    return BT_NOERROR;

  do
    {
      prev=trav;
      trav=trav->p_next;

      pFree(prev);

    } while(trav!=pNULL);
  
  return 0;
}


int OBEX_remove_all_client_callbacks(void )
{
  struct prh_obex_command_callback *trav, *prev;

  trav=prev=prh_obex_client_callbacks;

  if (trav==pNULL)
    return BT_NOERROR;

  do
    {
      prev=trav;
      trav=trav->p_next;

      pFree(prev);

    } while(trav!=pNULL);
  
  return 0;
}


int OBEX_remove_all_state(void )
{
  struct prh_obex_session_state *trav, *prev;

  trav=prev=prh_obex_state;

  if (trav==pNULL)
    return BT_NOERROR;

  do
    {
      prev=trav;
      trav=trav->p_next;

      pFree(prev);

    } while(trav!=pNULL);
  
  return 0;
}


int OBEX_remove_all_registered_services(void )
{
  struct prh_obex_service_registration *trav, *prev;

  trav=prev=obex_registered_services;

  if (trav==pNULL)
    return BT_NOERROR;

  do
    {
      prev=trav;
      trav=trav->p_next;
      if(prev->target)
	     pFree(prev->target);
      pFree(prev);

    } while(trav!=pNULL);
  
  return 0;
}


int OBEX_remove_all_registered_inboxes(void )
{
  struct prh_obex_inbox_registration *trav, *prev;

  trav=prev=obex_registered_inboxes;

  if (trav==pNULL)
    return BT_NOERROR;

  do
    {
      prev=trav;
      trav=trav->p_next;

      pFree(prev);

    } while(trav!=pNULL);
  
  return 0;
}

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_Init_Variable
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 At the moment I do not deal with the target information. I will have to
 deal with this when I am dealing with more than one service.
 
 *
 *
 ***********************************************************************/
void OBEX_Init_Variable(void)
{
//	OBEX_remove_all_server_transport();
//	prh_obex_server_transport = 0x00;
	OBEX_remove_all_transmit_transport();
	prh_obex_transmit_transport = 0x00;
	OBEX_remove_all_client_callbacks();
	prh_obex_client_callbacks = 0x00;
	OBEX_remove_all_state();
	prh_obex_state= 0x00;
	OBEX_remove_all_registered_services();
	obex_registered_services= 0x00;
	OBEX_remove_all_registered_inboxes();
	 obex_registered_inboxes= 0x00;
	 
	 prh_w4_obex_connect_rsp= 0x00;
	 prh_obex_connecting = 0;
     if( prh_obex_flow_lock != 0)
       pEventFree(prh_obex_flow_lock);
	 prh_obex_flow_lock =0 ;
     if( prh_obex_memory_lock != 0)
       pEventFree(prh_obex_memory_lock);
       prh_obex_memory_lock = 0;
	 CpTimerCreate =0;
	 prh_obex_flow=0;
	 OBEX_tid = 0;
	 OBEX_TransportListen_AUTHORIZE_callback = 0x00;
}

#endif

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_setup_association
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 At the moment I do not deal with the target information. I will have to
 deal with this when I am dealing with more than one service.
 
 *
 *
 ***********************************************************************/
void prh_obex_setup_association(u_int8* pdu, u_int16 len,  struct prh_obex_session_state* p_state)
{
	struct prh_obex_service_registration *servers;
	struct prh_obex_inbox_registration *inboxes;
	u_int16 offset_len = 0;
	t_obex_header_offsets headers;
	
	servers = obex_registered_services;
	    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_setup_association,len=%d\n",len));

	
	if(servers)
	{
		do {
			if(servers ==0 || (p_state->server_tid == servers->tid))
				break;
		} while((servers = servers->p_next));
	} 
	
	if(servers)
	{
		    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX servers->tid, =%x\n",servers->tid));
		if(pdu == NULL)
		{
			p_state->handler = servers->handler;
			return;
		}

		switch(*pdu)
		{
			case OBEX_CONNECT:
				offset_len = OBEX_CONNECT_HEADER;
				break;
			case OBEX_SETPATH:
				offset_len = OBEX_SETPATH_HEADER;
				break;
			default:
				offset_len = OBEX_HEADER;
				break;
		}

		OBEX_ExtractHeaders(&headers,&offset_len,pdu,len);
		if(headers.tar)
		{
			if(servers->target_length == ((pdu + headers.tar)[0] << 8) + (pdu + headers.tar)[1] -3)
			{
				if(!pMemcmp(pdu + headers.tar + 2,servers->target,servers->target_length))
				{
					p_state->handler = servers->handler;
	    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_setup_association 1, =%x\n",p_state->handler));
					return;
				}
			}
		} 
        else if(headers.cid) 
        {
			if( (u_int32)(((pdu + headers.cid)[0] << 24) + ((pdu + headers.cid)[1] << 16) + ((pdu + headers.cid)[2] << 8) + (pdu + headers.cid)[3] ) == servers->cid)
			{
					p_state->handler = servers->handler;
	    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_setup_association 2, =%x\n",p_state->handler));
					return;
			}
		}
	}
	
	inboxes = obex_registered_inboxes;
	
	if(inboxes)
	{
		do {
			if(inboxes ==0 || p_state->server_tid == inboxes->tid)
				break;
		} while((inboxes = inboxes->p_next));
	}
	    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_setup_association 3, =%x\n",p_state->handler));

	if(inboxes)
	{
		p_state->handler = inboxes->handler;
	    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_setup_association 4, =%x\n",p_state->handler));
	}
	    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_setup_association END 5, =%x\n",p_state->handler));	
}


/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_from_transport
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 This will need to first look at the headers. If there are target field
then it will need to find a match else it should go to the registered inbox 
for that connection.
 
 *
 *
 ***********************************************************************/
void prh_obex_from_transport(struct prh_obex_session_state* p_state, u_int8* data, u_int16 length)
{
	t_obex_headers headers;
	u_int8 opcode;
	u_int8 final_bit_set;
	t_DataBuf *buffer;
	opcode = 1;
	final_bit_set = 1;
	
	/*
    this will need to look at the databuffer and decide what to do
	*/
	
	/* check is it a valid opcode */
	    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,1,state=%d\n",p_state->state));

	headers.nameOffset = 10;
	
	if(data == NULL)
	{
		if(length == 0)
		{
			if(!p_state->handler)
			{
				prh_obex_setup_association(data,length,p_state);
			}
			p_state->handler(p_state->tid,0x00,headers,NULL,0x00,0x00);
			pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,1,p_state->handler=0x%x\n",p_state->handler));
			return;
		}
	}
	
	
	opcode = data[0];
	
	final_bit_set = opcode & 0x80;

	    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,2,opcode=%d\n",opcode));
	
	if(opcode)
    {		
		switch(p_state->state)
		{
		case PRH_OBEX_STATE_START:
			
			prh_obex_setup_association(data,length,p_state);

		    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,3 \n"));
			
			if(p_state->handler)
			{
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,4 \n"));

				if(final_bit_set)
				{				
				    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,5 \n"));

					if(opcode == OBEX_GET_FINAL)
					{
						p_state->state = PRH_OBEX_STATE_CONTINUED_GET;
					    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,6 \n"));
					}
					
					if(opcode == OBEX_CONNECT)
						prh_obex_connecting = 1;
		    			pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,7 \n"));
					
					p_state->handler(p_state->tid,opcode,headers,data,length,0x00);					
				} else {

				    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,8 \n"));
					p_state->state = PRH_OBEX_STATE_CONTINUED;					
					p_state->handler(p_state->tid,opcode,headers,data,length,0x00);
				}				
			} else {
				/* I should be calling the buffer allocator here */
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,9 \n"));
				OBEX_GetWriteBuffer(0,&buffer);
				OBEX_Response(p_state->tid,OBEX_NOT_IMPLEMENTED,0,buffer);
			}
			break;
		case PRH_OBEX_STATE_CONTINUED:
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,10 \n"));
			if(final_bit_set)
			{
				if(opcode == OBEX_GET_FINAL)
				{
					p_state->state = PRH_OBEX_STATE_CONTINUED_GET;
					p_state->handler(p_state->tid,opcode,headers,data,length,0x00);
				} else {
					/* clear association */
					p_state->state = PRH_OBEX_STATE_START;
					p_state->handler(p_state->tid,opcode,headers,data,length,0x00);
				}
			} else {
				p_state->handler(p_state->tid,opcode,headers,data,length,0x00);
			}
			break;
		case PRH_OBEX_STATE_CONTINUED_GET:
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,11 \n"));
			if(final_bit_set)
			{
				if(opcode == OBEX_GET_FINAL)
				{
					p_state->handler(p_state->tid,opcode,headers,data,length,0x00);
				} else {
					/* clear association */
					p_state->state = PRH_OBEX_STATE_START;
					p_state->handler(p_state->tid,opcode,headers,data,length,0x00);
				}
			} else {
				p_state->handler(p_state->tid,opcode,headers,data,length,0x00);
			}
			break;
		default:
			p_state->state = PRH_OBEX_STATE_START;
			p_state->handler = 0x00;
			break;
		}      
    } else {
		p_state->state = 0;
		/* generate a response */
		/* allocate buffer */
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,12 \n"));
		
		OBEX_GetWriteBuffer(0,&buffer);
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,13 \n"));
		
		/* should I call the app */
		if(p_state->handler)
		{
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_from_transport,14 \n"));

			p_state->handler(p_state->tid,opcode,headers,data,length,0x00);
		}
    }
}


#endif /* OBEX SERVER */

#if OBEX_CLIENT

int prh_obex_create_client_callback(u_int32 tid, t_obex_command_handler handler)
{
	struct prh_obex_command_callback* pCallback;
	
	pCallback = prh_obex_client_callbacks;
	
	if(pCallback)
    {
		do{
			if(pCallback->tid == tid)	  
				break;
			
			if(!pCallback->p_next)
			{
    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"create_client_callback: pMalloc 1 \n"));
				struct prh_obex_command_callback* pCallback_tmp = pCallback;	
				pCallback = pMalloc(sizeof(struct prh_obex_command_callback));
				pCallback->p_next = 0x00;
				pCallback_tmp->p_next = pCallback;
				break;
			}
		} while ((pCallback = pCallback->p_next));      
    } else {
    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"create_client_callback: pMalloc 2 \n"));
		pCallback = pMalloc(sizeof(struct prh_obex_command_callback));
		pCallback->p_next = 0x00;
		prh_obex_client_callbacks = pCallback;
    }
	
	if(pCallback)
    {
		pCallback->tid = tid;
		pCallback->callback = handler;      
    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_create_client_callback: pCallback->tid %x \n",pCallback->tid));
    } else {
		return BT_NORESOURCES;
    } 	

	return BT_NOERROR;
}


int prh_obex_free_client_callback(u_int32 tid)
{
	struct prh_obex_command_callback* pCallback ,*target;
	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_free_client_callback: tid %x \n",tid));
	pCallback = prh_obex_client_callbacks;
	target = 0x00;

	if(pCallback)
       {
		if(pCallback->tid == tid)
		{
			prh_obex_client_callbacks = pCallback->p_next;
			target = pCallback;
		} else {
			while(pCallback)
			{
				if((pCallback->p_next != NULL) && (pCallback->p_next->tid  == tid))
				{
					target = pCallback->p_next;
					pCallback->p_next = pCallback->p_next->p_next;
					break;
				}
				pCallback = pCallback->p_next;
			}
		}
		
		if(target)
	       {

			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX prh_obex_free_client_callback: pCallback->tid %x \n",pCallback->tid));
				pFree(target);
		} else {
			return BT_NORESOURCES;
		} 	
       }	
	
	return BT_NOERROR;
}


#endif /* OBEX CLIENT */

#if (OBEX_SERVER || OBEX_CLIENT)
/*
  common code
*/

struct prh_obex_retransmit_info { 
	struct prh_obex_transport* pTransport;
	u_int16 lcv;
	t_DataBuf* txbuffer;
//	timer;
};

#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.2.5
int CpTimerCreate =0,cpEventWait=0;
void prh_obex_retransmit_handler(void* arg)
{
	struct prh_obex_transport* pTransport;
	u_int8 status;
	t_DataBuf *buffer;
	u_int16 lcv;
	t_DataBuf* txbuffer;		
	int temp;
	u_int16 data_len;
	struct prh_obex_retransmit_info* pInfo;
	if(!arg) return;
	pInfo = (struct prh_obex_retransmit_info*)arg;
	lcv = pInfo->lcv;
	pTransport = pInfo->pTransport;
	txbuffer = pInfo->txbuffer;
	data_len = txbuffer->dataLen;
	for(;lcv < data_len ; lcv += pTransport->max_transport)
	{
		status=RSE_GetWriteBuffer(pTransport->use.rfcomm.address, pTransport->use.rfcomm.dlci,
			pTransport->max_transport,(struct host_buf**)&buffer);
		if(status!=BT_NOERROR)
		{
			pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - allocate got status of %x.", status));
			if(status == BT_RETRY || status == BT_FLOWCTRL)
			{
				pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
				pInfo->lcv = lcv;
				pInfo->pTransport = pTransport;
				pInfo->txbuffer = txbuffer;
				CpTimerCreate++;
				temp=pTimerCreate(20,prh_obex_retransmit_handler,pInfo,pTIMER_ONESHOT);
			}			
			break;
		}
		if(data_len - lcv < pTransport->max_transport)
		{
			pMemcpy(buffer->buf,txbuffer->buf + lcv,data_len - lcv);
			buffer->dataLen = data_len - lcv;
			if(prh_obex_flow == 0)
				{
				cpEventWait++;
				pEventWait(prh_obex_flow_lock, NULL, NULL); // TODO: 
				}
			status=RSE_Write(pTransport->use.rfcomm.address,
				pTransport->use.rfcomm.dlci,(struct host_buf*)buffer);
			if(status!=BT_NOERROR)
			{
				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of %x.", status));
				if(status == BT_RETRY || status == BT_FLOWCTRL)
				{
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;
					CpTimerCreate++;
					temp=pTimerCreate(20,prh_obex_retransmit_handler,pInfo,pTIMER_ONESHOT);
				}
				break;
			} else {
				OBEX_FreeWriteBuffer(txbuffer);
			}
		} else {
			pMemcpy(buffer->buf,txbuffer->buf + lcv,pTransport->max_transport);
			buffer->dataLen = pTransport->max_transport;
			if(prh_obex_flow == 0)
				{
				cpEventWait++;
				pEventWait(prh_obex_flow_lock, NULL, NULL); // TODO: 
				}
			status=RSE_Write(pTransport->use.rfcomm.address,
				pTransport->use.rfcomm.dlci,(struct host_buf*)buffer);
			if(status!=BT_NOERROR)
			{
				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of %x.", status));
				if(status == BT_RETRY || status == BT_FLOWCTRL )
				{
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					CpTimerCreate++;
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;
					temp=pTimerCreate(20,prh_obex_retransmit_handler,pInfo,pTIMER_ONESHOT);
				}				
				break;
			}
		}
	}
	if(arg)
		pFree(arg);
}


/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_transport_tx
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

  I might put code in here for blocking to prevent data being sent on the 
  same channel while a response is pending.
 
 *
 *
 ***********************************************************************/
int prh_obex_transport_tx(u_int32 tid, t_DataBuf* txbuffer)
{
	struct prh_obex_transport* pTransport;
	struct prh_obex_retransmit_info* pInfo;
	
	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL sending to peer:\n"));
    pDebugDumpEX(pLOGNOTICE,pLOGOBEX,txbuffer->buf,txbuffer->dataLen);
	
	pTransport = prh_obex_transmit_transport;
	
	if(pTransport)
    {
		do {
			if(pTransport->tid == tid)
				break;
			
		} while((pTransport = pTransport->p_next));
    }
	
	if(pTransport)
    {

	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTransport->max_obex: %x\n",pTransport->max_obex));
		
		if(txbuffer->buf[0] == OBEX_OK && prh_obex_connecting)
		{
			u_int16 temp_length;
			
			prh_obex_connecting = 0;
			
			temp_length = (txbuffer->buf[5] << 8) + txbuffer->buf[6];
	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"temp_length: %x\n",temp_length));
			if(pTransport->max_obex < temp_length)
			{
				pFree(pTransport->rx_buffer);
				pTransport->rx_buffer = pMalloc(temp_length);
				pTransport->max_obex = temp_length;
			} else {
				// there is no need to reallocate.
				pTransport->max_obex = temp_length;
			}
		}
		    		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_transport_tx:  pTransport->type ==%d\n",pTransport->type));
		    		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTransport->max_obex=%d\n",pTransport->max_obex));
		    		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"txbuffer->dataLen=%d\n",txbuffer->dataLen));

		switch(pTransport->type)
		{
		case PRH_OBEX_TYPE_RFCOMM:
		/*
		Check the length characteristics 
			*/
			if(pTransport->max_obex < txbuffer->dataLen)
				return BT_INVALIDPARAM;
			
			/* set up retransmit and send */
			
			pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));

			pInfo->lcv = 0;
			pInfo->pTransport = pTransport;
			pInfo->txbuffer = txbuffer;
/*
			prh_obex_retransmit_info.lcv = 0;
			prh_obex_retransmit_info.pTransport = pTransport;
			prh_obex_retransmit_info.txbuffer = txbuffer;
*/
			/* call the handler let it do all the work */
			prh_obex_retransmit_handler(pInfo);
			
			break;
		case PRH_OBEX_TYPE_SERIAL:
			break;
		case PRH_OBEX_TYPE_SOCKET:
			break;
		case PRH_OBEX_TYPE_GENERIC:
			break;
		default:
			return BT_INVALIDPARAM;
		}
    } else
    	{
    		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_transport_tx:  pTransport ==0\n"));

		return BT_INVALIDPARAM;
    	}	
	return BT_NOERROR;
}

#else
int prh_obex_retransmit_handler2(void* arg);

void prh_obex_retransmit_handler(void* arg)
{
	struct prh_obex_transport* pTransport;
	u_int8 status;
	t_DataBuf *buffer;
	u_int16 lcv;
	t_DataBuf* txbuffer;		
	int temp;
	u_int16 data_len;
	struct prh_obex_retransmit_info* pInfo;

if(prh_obex_flow_lock == 0)
	prh_obex_flow_lock = pEventCreate(0);
if(CpTimerCreate != 0)
{
	pTimerCancel(CpTimerCreate);
	CpTimerCreate = 0;
}
prh_obex_memory = 0;
	    		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_retransmit_handler\n" ));


	// setup phase 
	/*
	lcv = prh_obex_retransmit_info.lcv;
	pTransport = prh_obex_retransmit_info.pTransport;
	txbuffer = prh_obex_retransmit_info.txbuffer;
	data_len = txbuffer->dataLen;
	*/
	if(!arg) return;
		    		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"NOT REturn?\n" ));

	pInfo = (struct prh_obex_retransmit_info*)arg;
	
	lcv = pInfo->lcv;
	pTransport = pInfo->pTransport;
	txbuffer = pInfo->txbuffer;
	data_len = txbuffer->dataLen;
	

	for(;lcv < data_len ; lcv += pTransport->max_transport)
	{
			if(prh_obex_flow == 0)
			{
//				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of prh_obex_flow %x.", prh_obex_flow));
				{
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;
					pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL -  pEventWait b."));
					pEventWait(prh_obex_flow_lock, prh_obex_retransmit_handler2, pInfo); // TODO: 
				}
				break;
			}
		
		//OBEX_GetWriteBuffer(pTransport->max_transport,&buffer);
if( data_len > pTransport->max_transport )
		status=RSE_GetWriteBuffer(pTransport->use.rfcomm.address, pTransport->use.rfcomm.dlci,
			pTransport->max_transport,(struct host_buf**)&buffer);
else
		status=RSE_GetWriteBuffer(pTransport->use.rfcomm.address, pTransport->use.rfcomm.dlci,
			data_len,(struct host_buf**)&buffer);
		
		if(status!=BT_NOERROR)
		{
			pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - allocate got status of %x.", status));
			if(status == BT_RETRY || status == BT_FLOWCTRL)
			{
			/*
			prh_obex_retransmit_info.lcv = lcv;
			prh_obex_retransmit_info.pTransport = pTransport;
			prh_obex_retransmit_info.txbuffer = txbuffer;
				*/
				pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
				
				pInfo->lcv = lcv;
				pInfo->pTransport = pTransport;
				pInfo->txbuffer = txbuffer;
				
				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTimerCreate 1."));
				//CpTimerCreate=pTimerCreate((unsigned int)10,prh_obex_retransmit_handler,(void*)pInfo,pTIMER_ONESHOT);
				if(prh_obex_memory_lock == 0)
					prh_obex_memory_lock = pEventCreate(0);
				pEventWait(prh_obex_memory_lock, prh_obex_retransmit_handler2, pInfo); // TODO: 
				pEventSet(prh_obex_memory_lock);
				prh_obex_memory = 1;
			}			
			break;
		}
		
		if(data_len - lcv < pTransport->max_transport)
		{
			pMemcpy(buffer->buf,txbuffer->buf + lcv,data_len - lcv);
			buffer->dataLen = data_len - lcv;
//			if(prh_obex_flow == 0)
//				pEventWait(prh_obex_flow_lock, NULL, NULL); // TODO: 
			if(prh_obex_flow == 0)
			{

//				lcv += pTransport->max_transport;
				
				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of prh_obex_flow %x.", prh_obex_flow));
				{
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;

					pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL -  pEventWait 7."));
					pEventWait(prh_obex_flow_lock, prh_obex_retransmit_handler2, pInfo); // //TODO: 
				}

				break;
			}
			status=RSE_Write(pTransport->use.rfcomm.address,
				pTransport->use.rfcomm.dlci,(struct host_buf*)buffer);
			
			if(status!=BT_NOERROR)
			{
				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of %x.", status));
				if(status == BT_RETRY || status == BT_FLOWCTRL)
				{
				/*
				prh_obex_retransmit_info.lcv = lcv;
				prh_obex_retransmit_info.pTransport = pTransport;
				prh_obex_retransmit_info.txbuffer = txbuffer;
					*/
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;
	     				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTimerCreate 2."));
					temp=pTimerCreate(1,prh_obex_retransmit_handler,(void*)pInfo,pTIMER_ONESHOT);
//					pEventWait(prh_obex_flow_lock, prh_obex_retransmit_handler2, pInfo); // //TODO: 

				}
				break;
			} else {
				/* free the memory */
				/*
				OBEX_FreeWriteBuffer(prh_obex_retransmit_info.txbuffer);
				prh_obex_retransmit_info.txbuffer = 0x00;
				*/
				OBEX_FreeWriteBuffer(txbuffer);
			}
			
		} else {
			pMemcpy(buffer->buf,txbuffer->buf + lcv,pTransport->max_transport);
			buffer->dataLen = pTransport->max_transport;

			if(prh_obex_flow == 0)
			{

//				lcv += pTransport->max_transport;
				
				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of prh_obex_flow %x.", prh_obex_flow));
				{
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;
					pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL -  pEventWait 8."));
					pEventWait(prh_obex_flow_lock, prh_obex_retransmit_handler2, pInfo); // TODO: 
				}
				break;
			}
			
			status=RSE_Write(pTransport->use.rfcomm.address,
				pTransport->use.rfcomm.dlci,(struct host_buf*)buffer);
			if(status!=BT_NOERROR)
			{
				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of %x.", status));
				if(status == BT_RETRY || status == BT_FLOWCTRL )
				{
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;

					pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTimerCreate 3."));
					temp=pTimerCreate((unsigned int)1,prh_obex_retransmit_handler,(void*)pInfo,pTIMER_ONESHOT);
//					pEventWait(prh_obex_flow_lock, prh_obex_retransmit_handler2, pInfo); // //TODO: 
				}				
				break;
			}
			else
			{
			  //pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"################################lcv = %d data_len=%d\n",lcv,data_len));
			  if (lcv >=data_len)
				OBEX_FreeWriteBuffer(txbuffer);
			}
		}
	}

  		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"lcv = %x\n",lcv ));
  		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"data_len = %x\n",data_len ));
  		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTransport->max_transport = %x\n",pTransport->max_transport ));

	if(arg)
		pFree(arg);

if(lcv >= data_len)
{
      if( prh_obex_flow_lock != 0)
		pEventFree(prh_obex_flow_lock);
       prh_obex_flow_lock = 0;
		if(prh_obex_memory_lock != 0)
		{
			pEventFree(prh_obex_memory_lock);
			prh_obex_memory_lock = 0;
		}
		prh_obex_memory = 0;
}
    		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_retransmit_handler END \n"));
	
}
int prh_obex_retransmit_handler2(void* arg)
{
    if(!arg) return 0 ;
	struct prh_obex_transport* pTransport;
	u_int8 status=0;
	t_DataBuf *buffer;
	u_int16 lcv;
	t_DataBuf* txbuffer;		
	int temp;
	u_int16 data_len;
	struct prh_obex_retransmit_info* pInfo;
	

if(prh_obex_flow_lock == 0)
	prh_obex_flow_lock = pEventCreate(0);
if(CpTimerCreate != 0)
{
	pTimerCancel(CpTimerCreate);
	CpTimerCreate = 0;
}
    if( prh_obex_memory_lock != 0)
       pEventFree(prh_obex_memory_lock);
    prh_obex_memory_lock = 0;
	prh_obex_memory = 0;
	
	pInfo = (struct prh_obex_retransmit_info*)arg;
	lcv = pInfo->lcv;
	pTransport = pInfo->pTransport;
	txbuffer = pInfo->txbuffer;
	data_len = txbuffer->dataLen;
	for(;lcv < data_len ; lcv += pTransport->max_transport)
	{

			if(prh_obex_flow == 0)
			{
//				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of prh_obex_flow %x.", prh_obex_flow));
				{
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;
					pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL -  pEventWait c."));
					pEventWait(prh_obex_flow_lock, prh_obex_retransmit_handler2, pInfo); // TODO: 
				}
				break;
			}
		status=RSE_GetWriteBuffer(pTransport->use.rfcomm.address, pTransport->use.rfcomm.dlci,
			pTransport->max_transport,(struct host_buf**)&buffer);
		if(status!=BT_NOERROR)
		{
			pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - allocate got status of %x.", status));
			if(status == BT_RETRY || status == BT_FLOWCTRL)
			{
				pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
				pInfo->lcv = lcv;
				pInfo->pTransport = pTransport;
				pInfo->txbuffer = txbuffer;

				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTimerCreate 4."));
				//CpTimerCreate=pTimerCreate((unsigned int)10,prh_obex_retransmit_handler,(void*)pInfo,pTIMER_ONESHOT);
				if(prh_obex_memory_lock == 0)
					prh_obex_memory_lock = pEventCreate(0);
				pEventWait(prh_obex_memory_lock, prh_obex_retransmit_handler2, pInfo); // TODO: 
				pEventSet(prh_obex_memory_lock);
				prh_obex_memory = 1;
//					pEventWait(prh_obex_flow_lock, prh_obex_retransmit_handler2, pInfo); // //TODO: 
			}			
			break;
		}
		if(data_len - lcv < pTransport->max_transport)
		{
			pMemcpy(buffer->buf,txbuffer->buf + lcv,data_len - lcv);
			buffer->dataLen = data_len - lcv;
//				pEventWait(prh_obex_flow_lock, NULL, NULL); // TODO: 

			if(prh_obex_flow == 0)
			{


//				lcv += pTransport->max_transport;
				

				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of prh_obex_flow %x.", prh_obex_flow));
				{
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;
					pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL -  pEventWait 9."));
					pEventWait(prh_obex_flow_lock, prh_obex_retransmit_handler2, pInfo); // //TODO: 
				}
			
				break;
			}

			status=RSE_Write(pTransport->use.rfcomm.address,
				pTransport->use.rfcomm.dlci,(struct host_buf*)buffer);
			
			if(status!=BT_NOERROR)
			{
				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of %x.", status));
				if(status == BT_RETRY || status == BT_FLOWCTRL)
				{
				/*
				prh_obex_retransmit_info.lcv = lcv;
				prh_obex_retransmit_info.pTransport = pTransport;
				prh_obex_retransmit_info.txbuffer = txbuffer;
					*/
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;

					pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTimerCreate 5."));					
					temp=pTimerCreate((unsigned int)1,prh_obex_retransmit_handler,(void*)pInfo,pTIMER_ONESHOT);
//					pEventWait(prh_obex_flow_lock, prh_obex_retransmit_handler2, pInfo); // //TODO: 

				//OBEX_FreeWriteBuffer(txbuffer);

				}				
				break;
			} else {
				OBEX_FreeWriteBuffer(txbuffer);
			}
		} else {
			pMemcpy(buffer->buf,txbuffer->buf + lcv,pTransport->max_transport);
			buffer->dataLen = pTransport->max_transport;

			if(prh_obex_flow == 0)
			{


//				lcv += pTransport->max_transport;
				

				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of prh_obex_flow %x.", prh_obex_flow));
				{
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;
					pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL -  pEventWait a."));
					pEventWait(prh_obex_flow_lock, prh_obex_retransmit_handler2, pInfo); // TODO: 
				}
				break;
			}

			status=RSE_Write(pTransport->use.rfcomm.address,
				pTransport->use.rfcomm.dlci,(struct host_buf*)buffer);
			if(status!=BT_NOERROR)
			{
				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"TAL - transmit got status of %x.", status));
				if(status == BT_RETRY || status == BT_FLOWCTRL )
				{
					pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));
					pInfo->lcv = lcv;
					pInfo->pTransport = pTransport;
					pInfo->txbuffer = txbuffer;
					pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTimerCreate 6."));
					temp=pTimerCreate((unsigned int)1,prh_obex_retransmit_handler,(void*)pInfo,pTIMER_ONESHOT);
//					pEventWait(prh_obex_flow_lock, prh_obex_retransmit_handler2, pInfo); // //TODO: 
				//OBEX_FreeWriteBuffer(txbuffer);
				}				
				break;
			} else {
			  if (lcv >=data_len)
				OBEX_FreeWriteBuffer(txbuffer);
			}
		}
	}
  		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"lcv = %x\n",lcv ));
  		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"data_len = %x\n",data_len ));
  		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTransport->max_transport = %x\n",pTransport->max_transport ));
	if(arg)
		pFree(arg);

if(lcv >= data_len)
{
		pEventFree(prh_obex_flow_lock);
       prh_obex_flow_lock = 0;
	    if( prh_obex_memory_lock != 0)
	       pEventFree(prh_obex_memory_lock);
	    prh_obex_memory_lock = 0;
		prh_obex_memory = 0;
}

      return status;

}

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_transport_tx
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

  I might put code in here for blocking to prevent data being sent on the 
  same channel while a response is pending.
 
 *
 *
 ***********************************************************************/
int prh_obex_transport_tx(u_int32 tid, t_DataBuf* txbuffer)
{
	struct prh_obex_transport* pTransport;
	struct prh_obex_retransmit_info* pInfo;
	
	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL sending to peer:\n"));
    pDebugDumpEX(pLOGNOTICE,pLOGOBEX,txbuffer->buf,txbuffer->dataLen);
	
	pTransport = prh_obex_transmit_transport;
	
	if(pTransport)
    {
		do {
			if(pTransport->tid == tid)
				break;
			
		} while((pTransport = pTransport->p_next));
    }
	
	if(pTransport)
    {

	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTransport->max_obex: %x\n",pTransport->max_obex));
		
		if(txbuffer->buf[0] == OBEX_OK && prh_obex_connecting)
		{
			u_int16 temp_length;
			
			prh_obex_connecting = 0;
			
			temp_length = (txbuffer->buf[5] << 8) + txbuffer->buf[6];
	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"temp_length: %x\n",temp_length));
			if(pTransport->max_obex < temp_length)
			{

	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTransport->rx_buffer: %x\n",pTransport->rx_buffer));

	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"temp_length: %x\n",temp_length));

//				pTransport->rx_buffer = prh_obex_server_transport->rx_buffer;
				pTransport->max_obex = temp_length;
			} else {

				pTransport->max_obex = temp_length;
//				pTransport->max_obex = (temp_length > 256) ? temp_length : 256;
			}
		}
		    		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_transport_tx:  pTransport->type ==%d\n",pTransport->type));
		    		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTransport->max_obex=%d\n",pTransport->max_obex));
		    		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"txbuffer->dataLen=%d\n",txbuffer->dataLen));
	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTransport->rx_buffer: %x\n",pTransport->rx_buffer));

		switch(pTransport->type)
		{
		case PRH_OBEX_TYPE_RFCOMM:
		/*
		Check the length characteristics 
			*/
			if(pTransport->max_obex < txbuffer->dataLen)
				{
				return BT_INVALIDPARAM;
				}
			
			/* set up retransmit and send */
			
			pInfo = pMalloc(sizeof (struct prh_obex_retransmit_info));

			pInfo->lcv = 0;
			pInfo->pTransport = pTransport;
			pInfo->txbuffer = txbuffer;
/*
			prh_obex_retransmit_info.lcv = 0;
			prh_obex_retransmit_info.pTransport = pTransport;
			prh_obex_retransmit_info.txbuffer = txbuffer;
*/
			/* call the handler let it do all the work */
			prh_obex_retransmit_handler(pInfo);
			
			break;
		case PRH_OBEX_TYPE_SERIAL:
			break;
		case PRH_OBEX_TYPE_SOCKET:
			break;
		case PRH_OBEX_TYPE_GENERIC:
			break;
		default:
			return BT_INVALIDPARAM;
		}
    } else
    	{
    		pDebugPrintfEX((pLOGERROR,pLOGOBEX,"prh_obex_transport_tx:  pTransport ==0\n"));

		return BT_INVALIDPARAM;
    	}	

  pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_transport_tx:  END\n"));

	return BT_NOERROR;
}

#endif

#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.3.13

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_allocate_tid
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int32 prh_obex_allocate_tid(void)
{
  static u_int32 tid = 0;
  
  return tid++;
}


/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_remove_transport
 * DESCRIPTION:
 
 This function will remove a transport structure from the active list
 that is being used in the system.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_obex_tal_remove_transport(struct prh_obex_transport* target)
{
	struct prh_obex_transport* pTemp;

	pTemp = prh_obex_transmit_transport;

	if(pTemp)
	{
		if(target == pTemp)
		{
			prh_obex_transmit_transport = pTemp->p_next;
		} else {
			while(pTemp)
			{
				if(pTemp->p_next == target)
				{
					pTemp->p_next = target->p_next;
					pTemp = target;
					break;
				}

				pTemp = pTemp->p_next;
			}
		}


		if(pTemp)
		{
			if(pTemp->rx_buffer)
				pFree(pTemp->rx_buffer);
			pFree(pTemp);
		}
	}
}

#else

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_allocate_tid
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int32 prh_obex_allocate_tid(void)
{
  
  return OBEX_tid++;
}

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_allocate_tid
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_obex_clean_tid(void)
{
  
  OBEX_tid = 0;
}

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_remove_transport
 * DESCRIPTION:
 
 This function will remove a transport structure from the active list
 that is being used in the system.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_obex_tal_remove_transport(struct prh_obex_transport* target)
{
	struct prh_obex_transport* pTemp;

	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL: remove_transport\n"));    

	pTemp = prh_obex_transmit_transport;

	if(pTemp)
	{
		if(target == pTemp)
		{
			prh_obex_transmit_transport = pTemp->p_next;
		} else {
			while(pTemp)
			{
				if(pTemp->p_next == target)
				{
					pTemp->p_next = target->p_next;
					pTemp = target;
					break;
				}

				pTemp = pTemp->p_next;
			}
		}


		if(pTemp)
		{
			if(pTemp->rx_buffer)
				pTemp->rx_buffer = NULL;
				prh_obex_free_client_callback(pTemp->tid);
			pFree(pTemp);
		}
	}
}
#endif
#endif /* (OBEX_SERVER || OBEX_CLIENT) */

#if OBEX_RFCOMM
#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.3.13

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_connect_conf
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 I will need to handle the status information here at some stage but for 
 the moment I will just take care of the packet size.
 
 *
 *
***********************************************************************/
t_api prh_obex_tal_rfcomm_connect_conf(prh_t_rfcomm_schan schan, t_bdaddr addr, u_int8 status, prh_t_rfcomm_dlci dlci, u_int16 frame_size)
{
	/* client side */
	struct prh_obex_transport* pTransport;
	
	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL: Got Connect Conf,frame_size = %d\n",frame_size));    
	
	pTransport = prh_obex_transmit_transport;
	
	if(pTransport)
    {
		do {
			if(pTransport->type == PRH_OBEX_TYPE_RFCOMM && 
				pTransport->use.rfcomm.server_channel == schan &&
				BD_ADDR_CMP(pTransport->use.rfcomm.address,addr) )
				break;
		} while((pTransport = pTransport->p_next));
    }
	
	if(pTransport)
    {
		if(status == BT_NOERROR || status == BT_PENDING)
		{
			pTransport->type = PRH_OBEX_TYPE_RFCOMM;
			pTransport->use.rfcomm.dlci = dlci;
			pTransport->use.rfcomm.flow_state = 0;
			pTransport->max_transport = frame_size;
			pTransport->length = 0;
			pTransport->max_obex = PRH_OBEX_MIN_MAX; // this will not change until after the connect
			pTransport->rx_buffer = pMalloc(pTransport->max_obex); // I will have to think about memory here...
		} else {
			pTransport->rx_buffer = 0x00;
		}
		pEventSet(pTransport->connect_wait);
    }
	
	return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_connect_ind
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
t_api prh_obex_tal_rfcomm_connect_ind(prh_t_rfcomm_schan schan, t_bdaddr addr, prh_t_rfcomm_dlci dlci, u_int16 frame_size)
{
	/* server side */
	struct prh_obex_transport* pTransport;
	struct prh_obex_session_state* state;
	u_int8 match = 0;
	int ret=BT_NOERROR;

if(prh_obex_flow_lock == 0)
	prh_obex_flow_lock = pEventCreate(0);
	prh_obex_flow = 1;

	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "prh_obex_tal_rfcomm_connect_ind \n"));    

	pTransport = prh_obex_server_transport;
	
	if(!pTransport)
		return 1;
	/* search for entry */
	
	do {
		if(pTransport->type == PRH_OBEX_TYPE_RFCOMM &&
			pTransport->use.rfcomm.server_channel == schan)
			break;     
		
	} while((pTransport = pTransport->p_next));
	
	if(pTransport)
    {
		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL : Got Connect Ind\n"));
		
		/* create session state */
		
		state = prh_obex_state;
		
		/* place at the end of the list */

		/* can I simply place it at the head of the list */
		
		if(state)
		{
			do{
				if(!state->p_next)
				{
					state->p_next = pMalloc(sizeof(struct prh_obex_session_state));
					state = state->p_next;
					state->p_next = 0x00;
					break;
				}				
			} while((state = state->p_next));
		} else {
			state = pMalloc(sizeof(struct prh_obex_session_state));
			state->p_next = 0x00;
			
			prh_obex_state = state;
		}
		
		/* associate the state with a server */
		state->server_tid = pTransport->tid;
		state->state = 0x00;
		state->handler = 0x00;
		
		pTransport = prh_obex_transmit_transport;
		
		if(pTransport)
		{
			/* search for entry */
			do {
				if(pTransport->type == PRH_OBEX_TYPE_RFCOMM && 
					pTransport->use.rfcomm.server_channel == schan &&
					BD_ADDR_CMP(pTransport->use.rfcomm.address,addr) )
				{
					match = 1;
					break;
				}				
			} while((pTransport = pTransport->p_next));			
		}

		if(!pTransport)
		{
			/* allocate */
			pTransport = pMalloc(sizeof(struct prh_obex_transport));
			
			if(!pTransport)
				return BT_NORESOURCES;
			
			pTransport->p_next = prh_obex_transmit_transport;
			pTransport->max_transport = frame_size;
			pTransport->max_obex = PRH_OBEX_MIN_MAX; // this will change after a connect
			pTransport->rx_buffer = pMalloc(pTransport->max_obex);
			pTransport->length = 0x00;
			prh_obex_transmit_transport = pTransport;			
		}		
				
		pTransport->tid = prh_obex_allocate_tid();
		pTransport->type = PRH_OBEX_TYPE_RFCOMM;
		pTransport->use.rfcomm.server_channel = schan;
		pTransport->use.rfcomm.flow_state = PRH_RFCOMM_FLOW_DATA_STOP;
		pTransport->use.rfcomm.dlci = dlci;
		pTransport->use.rfcomm.address = addr;
		
		/* associate the state with a client transport */
		state->tid = pTransport->tid;
		ret=RSE_SrvConnectResponse(addr, dlci, 1);
    } 

	return ret;
}
#else

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_connect_conf
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 I will need to handle the status information here at some stage but for 
 the moment I will just take care of the packet size.
 
 *
 *
***********************************************************************/
t_api prh_obex_tal_rfcomm_connect_conf(prh_t_rfcomm_schan schan, t_bdaddr addr, u_int8 status, prh_t_rfcomm_dlci dlci, u_int16 frame_size)
{
	/* client side */
	struct prh_obex_transport* pTransport;
	
	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL: Got Connect Conf,frame_size = %d schan=%d\n",frame_size,schan));    
	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL: Got Connect Conf,status = %x\n",status));    
	
	pTransport = prh_obex_transmit_transport;
	
	if(pTransport)
    {
		do {
			if(pTransport->type == PRH_OBEX_TYPE_RFCOMM && 
				pTransport->use.rfcomm.server_channel == schan &&
				BD_ADDR_CMP(pTransport->use.rfcomm.address,addr) )
				break;
		} while((pTransport = pTransport->p_next));
    }
	
	if(pTransport)
    {
		if(status == BT_NOERROR || status == BT_PENDING)
		{
			pTransport->type = PRH_OBEX_TYPE_RFCOMM;
			pTransport->use.rfcomm.dlci = dlci;
			pTransport->use.rfcomm.flow_state = 0;

	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL: pTransport->use.rfcomm.dlci = %x\n",pTransport->use.rfcomm.dlci));    


//JianCui add for test 2009/02/17
//if(frame_size > 328)
//			pTransport->max_transport = 328;
//else
			pTransport->max_transport = frame_size;

			pTransport->length = 0;
			pTransport->max_obex = PRH_OBEX_MIN_MAX; // this will not change until after the connect
			pTransport->rx_buffer = server_transport[1].rx_buffer;// I will have to think about memory here...


		} else {
			pTransport->rx_buffer = 0x00;
		}
if(status == 0)
			pTransport->rx_buffer = 0x00;

		pEventSet(pTransport->connect_wait);

    }
	
	return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_connect_ind
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
t_api prh_obex_tal_rfcomm_connect_ind(prh_t_rfcomm_schan schan, t_bdaddr addr, prh_t_rfcomm_dlci dlci, u_int16 frame_size)
{
	/* server side */
	struct prh_obex_transport* pTransport;
	struct prh_obex_session_state* state;
	u_int8 match = 0;
	int ret=BT_NOERROR;
//    u_int8* server_rx_buffer;

if(prh_obex_flow_lock == 0)
	prh_obex_flow_lock = pEventCreate(0);
	prh_obex_flow = 1;

	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "prh_obex_tal_rfcomm_connect_ind schan =%d,dlci =%d,frame_size=%d\n",schan,dlci,frame_size));    

	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "prh_obex_tal_rfcomm_connect_ind schan =%d,dlci =%d,frame_size=%d\n",schan,dlci,frame_size));    

	pTransport = prh_obex_server_transport;
	
	if(!pTransport)
		return 1;
	/* search for entry */
	
	do {
		if(pTransport->type == PRH_OBEX_TYPE_RFCOMM &&
			pTransport->use.rfcomm.server_channel == schan)
			break;     
		
	} while((pTransport = pTransport->p_next));

		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL : Got Connect Ind 0\n"));
	
	if(pTransport)
    {
		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL : Got Connect Ind\n"));
		
		/* create session state */
		
		state = prh_obex_state;
		
		/* place at the end of the list */

		/* can I simply place it at the head of the list */
		
		if(state)
		{
			do{
				if(!state->p_next)
				{
					state->p_next = pMalloc(sizeof(struct prh_obex_session_state));
					state = state->p_next;
					state->p_next = 0x00;
					break;
				}				
			} while((state = state->p_next));
		} else {
			state = pMalloc(sizeof(struct prh_obex_session_state));
			state->p_next = 0x00;
			
			prh_obex_state = state;
		}

		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL : Got Connect Ind 1\n"));
		
		/* associate the state with a server */
		state->server_tid = pTransport->tid;
		state->state = 0x00;
		state->handler = 0x00;
		
		pTransport = prh_obex_transmit_transport;
		
		if(pTransport)
		{
			/* search for entry */
			do {
				pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "match = 0 pTransport->use.rfcomm.dlci = %d\n",pTransport->use.rfcomm.dlci));
				if(pTransport->type == PRH_OBEX_TYPE_RFCOMM && 
					pTransport->use.rfcomm.dlci == dlci &&
					BD_ADDR_CMP(pTransport->use.rfcomm.address,addr) )
				{
					match = 1;
					pTransport->use.rfcomm.dlci = dlci;
					pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "match = 1 pTransport->use.rfcomm.dlci = %d\n",pTransport->use.rfcomm.dlci));
					break;
				}				
			} while((pTransport = pTransport->p_next));			
		}

		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL : Got Connect Ind 2\n"));

		if(!pTransport)
		{
			/* allocate */
			pTransport = pMalloc(sizeof(struct prh_obex_transport));
			
			if(!pTransport)
				return BT_NORESOURCES;
			
			pTransport->p_next = prh_obex_transmit_transport;
			pTransport->max_transport = frame_size;
			pTransport->max_obex = PRH_OBEX_MIN_MAX; // this will change after a connect
//			pTransport->rx_buffer = pMalloc(pTransport->max_obex);
if(schan == RDA_FTPS_Server_Channel)
			pTransport->rx_buffer = prh_obex_server_transport->rx_buffer;// I will have to think about memory here...
else
			pTransport->rx_buffer = server_transport[1].rx_buffer;// I will have to think about memory here...
	
			pTransport->length = 0x00;
			prh_obex_transmit_transport = pTransport;			
		}		

		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL : Got Connect Ind 3 math = %d\n",match));
	if(!match)
	{
		pTransport->tid = prh_obex_allocate_tid();
		pTransport->type = PRH_OBEX_TYPE_RFCOMM;
		pTransport->use.rfcomm.server_channel = schan;
		pTransport->use.rfcomm.flow_state = PRH_RFCOMM_FLOW_DATA_STOP;
		pTransport->use.rfcomm.dlci = dlci;
		pTransport->use.rfcomm.address = addr;
	}		
		/* associate the state with a client transport */
		state->tid = pTransport->tid;
//JianCui test
		if((OBEX_TransportListen_AUTHORIZE_callback) && (PRH_RFCOMM_DLCI0 != dlci))
			OBEX_TransportListen_AUTHORIZE_callback(addr, (u_int8)dlci, (u_int8)schan,(u_int8)pTransport->tid);
		else
			ret=RSE_SrvConnectResponse(addr, dlci, 1);
    } 
		pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "OBEX-TAL : Got Connect Ind 4\n"));

	return ret;
}
#endif

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_flow_info_ind
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
t_api prh_obex_tal_rfcomm_flow_info_ind( t_bdaddr addr,prh_t_rfcomm_dlci dlci, u_int8 flow_state)
{
  /* search for the entry and set the flow state */

  pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "SE: Got Flow %d\n", flow_state));

  if(flow_state == RSE_FLOW_STOP)
  {
	  prh_obex_flow = 0;
  } else {
	  	  prh_obex_flow = 1;

if(prh_obex_flow_lock == 0 )
	  pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "prh_obex_flow_lock ==0\n" ));
else
	{
	  pEventSet(prh_obex_flow_lock);
      rdabt_send_notify();   
	}
  }
  
  return BT_NOERROR;

}

#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.3.13
/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_client_data
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
t_api prh_obex_tal_rfcomm_client_data(t_bdaddr addr,prh_t_rfcomm_dlci dlci, struct host_buf *p_buf)
{
	struct prh_obex_transport* pTransport;
	struct prh_obex_command_callback* pCallback;


    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL Client received from peer:\n"));

	pTransport = prh_obex_transmit_transport;
	
	if(!pTransport)
		return 1;
	
	/* search for entry */
	
	do {
		if(pTransport->type == PRH_OBEX_TYPE_RFCOMM && 
			pTransport->use.rfcomm.dlci == dlci &&
			BD_ADDR_CMP(pTransport->use.rfcomm.address,addr) )
			break;
	} while((pTransport = pTransport->p_next));
	if(pTransport)
    {
		if(p_buf->data[0] == OBEX_OK && prh_w4_obex_connect_rsp)
		{
			u_int16 temp_length;

			prh_w4_obex_connect_rsp = 0;
			
			temp_length = (p_buf->data[5] << 8) + p_buf->data[6];


			
			if(pTransport->max_obex < temp_length)
			{
				pFree(pTransport->rx_buffer);


			//CuiJian JianCui add for test 2009/02/26
				pTransport->rx_buffer = pMalloc(temp_length);
				pTransport->max_obex = temp_length;
			} else {
				// there is no need to reallocate.
				pTransport->max_obex = (temp_length > 256) ? temp_length : 256;
			}
		}


		/* search for callback */
		
		pCallback = prh_obex_client_callbacks;
		if(pCallback)
		{

			do{
				if(pCallback->tid == pTransport->tid)
					break;
			} while ((pCallback = pCallback->p_next));
			
			if(pCallback)
			{

				/*
					I will have to store the data here
				*/
				pMemcpy(pTransport->rx_buffer + pTransport->length,p_buf->data,p_buf->len);
				pTransport->length += p_buf->len;

				if(pTransport->length == (pTransport->rx_buffer[1] << 8) + pTransport->rx_buffer[2])
				{

					pCallback->callback(pTransport->rx_buffer[0],pTransport->rx_buffer,pTransport->length);

					pTransport->length = 0;
				}
			} else {
				/* nothing I can do */
			}
		} else {
			/* nothing I can do */
		}           
    }
	
	host_buf_free(p_buf);

	return BT_NOERROR;
}

#else
/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_client_data
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
t_api prh_obex_tal_rfcomm_client_data(t_bdaddr addr,prh_t_rfcomm_dlci dlci, struct host_buf *p_buf)
{
	struct prh_obex_transport* pTransport;
	struct prh_obex_command_callback* pCallback;


    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL Client received from peer: dlci =%d\n",dlci));
    pDebugDumpEX(pLOGNOTICE,pLOGOBEX,p_buf->data,p_buf->len);

	pTransport = prh_obex_transmit_transport;

    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_transmit_transport = 0x%x\n",prh_obex_transmit_transport));
	if(!pTransport)
		return 1;
	
	/* search for entry */
	
	do {

		   pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTransport = %x\n",pTransport));

		if(pTransport->type == PRH_OBEX_TYPE_RFCOMM && 
			pTransport->use.rfcomm.dlci == dlci &&
			BD_ADDR_CMP(pTransport->use.rfcomm.address,addr) )
			break;
	} while((pTransport = pTransport->p_next));

    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"pTransport 2 = %x\n",pTransport));

	if(pTransport)
    {
    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL Client received from peer: b\n"));
		if(p_buf->data[0] == OBEX_OK && prh_w4_obex_connect_rsp)
		{
			u_int16 temp_length;

			prh_w4_obex_connect_rsp = 0;
			
			temp_length = (p_buf->data[5] << 8) + p_buf->data[6];
/*
			if(pTransport->max_obex < temp_length)
			{
				pTransport->max_obex = temp_length;
			} else {
				// there is no need to reallocate.
				pTransport->max_obex = (temp_length > 256) ? temp_length : 256;
			}
*/
			pTransport->max_obex = temp_length;

		}
		
		pTransport->rx_buffer = server_transport[1].rx_buffer;// I will have to think about memory here...

		/* search for callback */
		
		pCallback = prh_obex_client_callbacks;
		    	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_tal_rfcomm_client_data :pCallback d= %x 0\n",pCallback));
		if(pCallback)
		{
		    	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_tal_rfcomm_client_data :pCallback e\n"));

			do{
		    	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"obex :pCallback->tid %x\n",pCallback->tid));
		    	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"obex :pTransport->tid %x\n",pTransport->tid));
				
				if(pCallback->tid == pTransport->tid)
					break;
			} while ((pCallback = pCallback->p_next));
			
			if(pCallback)
			{
    		        pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_tal_rfcomm_client_data :pCallback f\n"));

				/*
					I will have to store the data here
				*/
				pMemcpy(pTransport->rx_buffer + pTransport->length,p_buf->data,p_buf->len);
				pTransport->length += p_buf->len;

				if(pTransport->length == (pTransport->rx_buffer[1] << 8) + pTransport->rx_buffer[2])
				{
				        pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"prh_obex_tal_rfcomm_client_data :pCallback ->callback g \n"));

					pCallback->callback(pTransport->rx_buffer[0],pTransport->rx_buffer,pTransport->length);

					pTransport->length = 0;
				}
			} else {
				/* nothing I can do */
			}
		} else {
			/* nothing I can do */
		}           
    }
	
	host_buf_free(p_buf);

	return BT_NOERROR;
}
#endif

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_server_data
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
 extern u_int32 HCI_wrongDataCount;
t_api prh_obex_tal_rfcomm_server_data(t_bdaddr addr,prh_t_rfcomm_dlci dlci, struct host_buf *p_buf)
{
	struct prh_obex_transport* pTransport;
	struct prh_obex_session_state* state;
	
    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL Server received from peer: dlci =%d,p_buf = 0x%x\n",dlci,p_buf));
    pDebugDumpEX(pLOGNOTICE,pLOGOBEX,p_buf->data,p_buf->len);
	pTransport = prh_obex_transmit_transport;
	
	if(!pTransport)
		return 1;
	
	/* search for entry */
	
	do {
		if(pTransport->type == PRH_OBEX_TYPE_RFCOMM && 
			pTransport->use.rfcomm.dlci == dlci &&
			BD_ADDR_CMP(pTransport->use.rfcomm.address,addr) )
			break;
	} while((pTransport = pTransport->p_next));
	
	if(pTransport)
    {
		/* search for callback */
		
		state = prh_obex_state;
		
		if(state)
		{
			do{
				if(state->tid == pTransport->tid)
					break;	    
			} while((state = state->p_next));
			
			if(state)
			{
				if(pTransport->length + p_buf->len > pTransport->max_transport)
				{

				}

			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL : pTransport->rx_buffer = %x\n",pTransport->rx_buffer));
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL : pTransport->length = %d\n",pTransport->length));
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL : p_buf->len = %d\n",p_buf->len));

				pMemcpy(pTransport->rx_buffer + pTransport->length,p_buf->data,p_buf->len);
				pTransport->length += p_buf->len;

			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL : pTransport->length = %x\n",pTransport->length));
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL : rx_buffer[1] = %x\n",pTransport->rx_buffer[1]));
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL : rx_buffer[2] = %x\n",pTransport->rx_buffer[2]));
			    pDebugPrintfEX((pLOGNOTICE,pLOGOBEX,"OBEX-TAL :NEEDED DATA LEN IS %x ,LOSSED DATA LEN IS %x\n",(pTransport->rx_buffer[1] << 8) + pTransport->rx_buffer[2]-pTransport->length,HCI_wrongDataCount));
				if(pTransport->length == (pTransport->rx_buffer[1] << 8) + pTransport->rx_buffer[2] || pTransport->length == (pTransport->rx_buffer[1] << 8) + pTransport->rx_buffer[2] + HCI_wrongDataCount )
				{
					prh_obex_from_transport(state,pTransport->rx_buffer,pTransport->length);

					HCI_wrongDataCount = 0;
					pTransport->length = 0;
				}				
			}
		}
    }
	
	host_buf_free(p_buf);
	return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_disconnection_ind
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
t_api prh_obex_tal_rfcomm_disconnect_ind(t_bdaddr addr,prh_t_rfcomm_dlci dlci)
{
	struct prh_obex_session_state* state,*prev_state;
	struct prh_obex_transport* pTransport,*marker;


	pDebugPrintfEX((pLOGERROR,pLOGOBEX, "prh_obex_tal_rfcomm_disconnect_ind dlci == %d\n",dlci));    

if(prh_obex_flow_lock != 0)
{
	pEventFree(prh_obex_flow_lock);
	prh_obex_flow_lock = 0;
}
if(prh_obex_memory_lock != 0)
{
	pEventFree(prh_obex_memory_lock);
	prh_obex_memory_lock = 0;
}
	pTransport = prh_obex_transmit_transport;
	
	if(!pTransport)
		return 1;
	
	/* search for entry */
	
	do {
		if(pTransport->type == PRH_OBEX_TYPE_RFCOMM && 
			pTransport->use.rfcomm.dlci == dlci &&
			BD_ADDR_CMP(pTransport->use.rfcomm.address,addr) )
			break;
	} while((pTransport = pTransport->p_next));


#if (1)  // jiancui added it. 2008.3.13
{
	struct prh_obex_command_callback* pCallback;

		pCallback = prh_obex_client_callbacks;
		if(pCallback)
		{
	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "prh_obex_tal_rfcomm_disconnect_ind pCallback == %x\n",pCallback));    
		
			do{
				
				if( pTransport && (pCallback->tid == pTransport->tid))
					break;
			} while ((pCallback = pCallback->p_next));
			
			if(pCallback)
			{

				/*
					I will have to store the data here
				*/
				if(pCallback->callback)
					pCallback->callback(OBEX_DISCONNECT,NULL,0);
//				prh_obex_free_client_callback(pCallback->tid);
			} else {
				/* nothing I can do */
			}
		} else {
			/* nothing I can do */
		}           
}
#endif
	
	
	state = prh_obex_state;
	prev_state = state;

	if(state)
	{
		do{
			if(pTransport && (state->tid == pTransport->tid))
				break;
			prev_state = state;
		} while((state = state->p_next));
	}
	
	if(state)
	{
		prh_obex_from_transport(state,NULL,0x00);
		if(prev_state == state)
		{
			prh_obex_state = state->p_next;
		} else {
			prev_state->p_next = state->p_next;
		}
		pFree(state);
	}
	
	marker = pTransport;

	pTransport = prh_obex_transmit_transport;
	
	if(marker == pTransport)
	{
		if (pTransport)
			prh_obex_transmit_transport = pTransport->p_next;
	}
	else if (pTransport) 
	{
		do {
			if(pTransport->p_next == marker)
				break;
		} while((pTransport = pTransport->p_next));
		if (pTransport  && marker)
			pTransport->p_next = marker->p_next;
	}
	if (marker)
		pFree(marker);

  return BT_NOERROR;
}


#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.3.13

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_reg_port
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
t_api prh_obex_tal_rfcomm_reg_port(u_int8 server_channel)
{
  prh_t_rfcomm_schan schan=server_channel;
  t_RSE_Callbacks cbs;
  int ret;

  cbs.connectionConfirmation=prh_obex_tal_rfcomm_connect_conf;
  cbs.connectionIndication=prh_obex_tal_rfcomm_connect_ind;
  cbs.dataIndication=prh_obex_tal_rfcomm_server_data;
  cbs.flowIndication=prh_obex_tal_rfcomm_flow_info_ind;
  cbs.disconnectionIndication=prh_obex_tal_rfcomm_disconnect_ind;
  cbs.framesizeIndication = 0x00;
  
  
  ret=RSE_SrvRegisterPort(&schan, &cbs);
  
  return ret;
}
#else

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_reg_port
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
t_api prh_obex_tal_rfcomm_reg_port(u_int8 server_channel,u_int16 max_obex,u_int8* rx_buffer,void (*authorizecallbackFunc)(t_bdaddr address, u_int8 dlci, u_int8 schan, u_int8 tid_s))
{
  prh_t_rfcomm_schan schan=server_channel;
  t_RSE_Callbacks cbs;
  int ret;
  struct prh_obex_transport* pTransport;

  pTransport = prh_obex_server_transport;
  OBEX_TransportListen_AUTHORIZE_callback = authorizecallbackFunc;
	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "prh_obex_tal_rfcomm_reg_port %x\n",rx_buffer));    

  cbs.connectionConfirmation=prh_obex_tal_rfcomm_connect_conf;
  cbs.connectionIndication=prh_obex_tal_rfcomm_connect_ind;
  cbs.dataIndication=prh_obex_tal_rfcomm_server_data;
  cbs.flowIndication=prh_obex_tal_rfcomm_flow_info_ind;
  cbs.disconnectionIndication=prh_obex_tal_rfcomm_disconnect_ind;
  cbs.framesizeIndication = 0x00;
  
//  pTransport->max_obex = max_obex; // this will change after a connect
//  pTransport->rx_buffer = (u_int8*)rx_buffer;

  ret=RSE_SrvRegisterPort(&schan, &cbs);

	pDebugPrintfEX((pLOGNOTICE,pLOGOBEX, "prh_obex_tal_rfcomm_reg_port 2 %x\n",pTransport->rx_buffer));    
  return ret;
}

#endif
/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_tal_rfcomm_connect_port
 * DESCRIPTION:
 
 This function should be generic

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
t_api prh_obex_tal_rfcomm_connect_port(t_bdaddr bdaddr, u_int8 schan,u_int16 mtu)
{
  t_RSE_Callbacks cbs;
  int ret;

if(prh_obex_flow_lock == 0)
	prh_obex_flow_lock = pEventCreate(0);
	prh_obex_flow = 1;


  cbs.connectionConfirmation=prh_obex_tal_rfcomm_connect_conf;
  cbs.connectionIndication=prh_obex_tal_rfcomm_connect_ind;
  cbs.dataIndication=prh_obex_tal_rfcomm_client_data;
  cbs.flowIndication=prh_obex_tal_rfcomm_flow_info_ind;
  cbs.disconnectionIndication=prh_obex_tal_rfcomm_disconnect_ind;
  cbs.framesizeIndication = 0x00;

  ret=RSE_CliConnectPort(bdaddr, schan, mtu, &cbs);
  
  return ret;
  
}

#endif /* OBEX_RFCOMM*/
