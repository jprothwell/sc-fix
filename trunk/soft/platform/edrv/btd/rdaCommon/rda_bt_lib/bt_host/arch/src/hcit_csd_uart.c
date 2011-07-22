/***********************************************************************
 *
 * MODULE NAME:    hci_transport_ecos.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    HCI Transport for eCos Serial port
 * MAINTAINER:     John Sheehy
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: hcit_csd_uart.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    1.May.2000 - JS - first import
 *
 *
 * ISSUES:
 *       
 ***********************************************************************/

#include "host_config.h"


#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"

#include "papi.h"

#include "hci_transport.h"
#include "hal_timers.h"
#define PRH_BAUD_DIP 1

enum
{
    HCI_UART_STATE_RX_TYPE,
    HCI_UART_STATE_RX_HEADER,      
    HCI_UART_STATE_RX_DATA    
};

#if pDEBUG
static char *transportTypes[] = {        
    "HCI Command",
    "ACL Data",
    "SCO Data",
    "HCI Event",
    "RS232 Error",
    "RS232 Negotiation"
};
#endif

static u_int8 hci_uart_rx_state;
static struct host_buf *hci_uart_read_buffer=NULL;
static u_int16 hci_uart_pend_length;                  /* the length of data have received */
static u_int16 hci_uart_rx_length;                    /* the length of data need receive */
static u_int8* hci_uart_rx_buf=NULL;
static u_int8  hci_uart_rx_head_buf[4];
static u_int8  hci_uart_rx_type; 
static const u_int8 hci_uart_head_length[] = {4,3,2};

static  const u_int8 fc_wakeup_data[]={0x01,0xc0,0xfc,0x00};

u_int8 need_send_fc =0;
t_pTimer uart_timer = 0;
/*
 * Declare a global function pointer to set the receive PDU callback
 */

t_api (*HCI_Transport_RecvCallback)(struct host_buf *pdu, u_int8 type);
unsigned char rda5868_uart_tx(u_int8 *buf, u_int16 num_to_send, u_int16 *num_send);
typedef void (*UART_RX_CB_FUNC)(u_int8 *buff, u_int32 len); 
void rdabt_set_uart_rx_cb(UART_RX_CB_FUNC func);

t_api HCI_Transport_Init(void *platformConfig) 
{
    rdabt_set_uart_rx_cb(HCI_Transport_Rx_Data);
    hci_uart_rx_state = HCI_UART_STATE_RX_TYPE;
    hci_uart_pend_length = 0;
    hci_uart_rx_length = 1;
    hci_uart_rx_buf = hci_uart_rx_head_buf;
    return BT_NOERROR;
}

t_api HCI_Transport_Shutdown(void) 
{
    return BT_NOERROR;
}


void HCI_Transport_Set_Rx_CB(t_api (*callbackFunc)(struct host_buf *pdu, u_int8 type))
{
    HCI_Transport_RecvCallback = callbackFunc;
}
extern void * memset(void *,int,size_t);
UINT32 HCI_wrongDataCount=0;
void HCI_Transport_Rx_Data(u_int8 *pdu, u_int32 length)
{
   if (length == 0)
   	return ;
   /// pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"Receive hci data length = %d, pdu[0] = %d\n",length,pdu[0]));
    //pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"at %d tick,Receive state = %d, hci_uart_pend_length=%d,hci_uart_rx_length=%d\n", hal_TimGetUpTime(),hci_uart_rx_state,hci_uart_pend_length,hci_uart_rx_length));
    if(hci_uart_pend_length + length < hci_uart_rx_length)
    {
        pMemcpy(hci_uart_rx_buf + hci_uart_pend_length, pdu, length);
        hci_uart_pend_length += length;
        return;
    }

    while(hci_uart_pend_length + length >=  hci_uart_rx_length)    
    {
        pMemcpy(hci_uart_rx_buf + hci_uart_pend_length, pdu, hci_uart_rx_length - hci_uart_pend_length);
        pdu += hci_uart_rx_length - hci_uart_pend_length;
        length -= hci_uart_rx_length - hci_uart_pend_length;

        //pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"at %d tick, while loop Receive state = %d pdu_length=%d\n",hal_TimGetUpTime(), hci_uart_rx_state,length));
        switch(hci_uart_rx_state)
        {
        case HCI_UART_STATE_RX_TYPE:
            hci_uart_rx_type = hci_uart_rx_buf[0];
            pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"at %d tick,Receive hci packet of %s\n", hal_TimGetUpTime(),hci_uart_rx_type-1<6?transportTypes[hci_uart_rx_type-1]:"INVALID"));
            /* rx type error */
            if(hci_uart_rx_type < 2 || hci_uart_rx_type > 4)
            {
		  HCI_wrongDataCount++;
        	  pDebugPrintfEX((pLOGERROR,pLOGHCI,"Receive hci packet type wrong!!!  hci_uart_rx_type = %d\n", hci_uart_rx_type));				  
                break;
            }
            hci_uart_rx_length = hci_uart_head_length[hci_uart_rx_type - 2];
            hci_uart_rx_buf = hci_uart_rx_head_buf;
            hci_uart_rx_state = HCI_UART_STATE_RX_HEADER;
            break;
        case HCI_UART_STATE_RX_HEADER:

            hci_uart_pend_length = hci_uart_rx_length;
            
            if(hci_uart_rx_type == 2) /* ACL data */
            {
                hci_uart_rx_length = ((u_int16)hci_uart_rx_buf[3]<<8) |  hci_uart_rx_buf[2];
               // pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"Read 4 bytes of ACL Data PDU header: 0x%02X 0x%02X 0x%02X 0x%02X \n", hci_uart_rx_buf[0], hci_uart_rx_buf[1], hci_uart_rx_buf[2], hci_uart_rx_buf[3]));
            }
            else if(hci_uart_rx_type == 3) /* SCO data */
            {
                hci_uart_rx_length =  hci_uart_rx_buf[2];
                pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"Read 3 bytes of SCO Data PDU header: 0x%02X 0x%02X 0x%02X \n", hci_uart_rx_buf[0], hci_uart_rx_buf[1], hci_uart_rx_buf[2]));
            }
            else if (hci_uart_rx_type== 4)  /* Event */
            {
                hci_uart_rx_length =  hci_uart_rx_buf[1];
               // pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"Read 2 bytes of Event PDU header: 0x%02X 0x%02X\n", hci_uart_rx_buf[0], hci_uart_rx_buf[1]));
            }
            else
            	{
			asm("break 7");
		}
            /* allocate a buffer for the PDU */        
       		if (!(hci_uart_read_buffer=host_buf_alloc(hci_uart_pend_length + hci_uart_rx_length))) 
			{  
				pDebugPrintfEX((pLOGERROR,pLOGHCI,"Cannot allocate %d bytes of memory\n", hci_uart_pend_length + hci_uart_rx_length));
				return;
			}                             
      		pMemcpy(hci_uart_read_buffer->data, hci_uart_rx_buf, hci_uart_pend_length); /* copy header bytes into PDU buffer*/
        	hci_uart_read_buffer->len = hci_uart_pend_length;
            hci_uart_rx_buf = hci_uart_read_buffer->data + hci_uart_pend_length;
            
            if (hci_uart_read_buffer != NULL)
            {
                hci_uart_rx_state = HCI_UART_STATE_RX_DATA;
            }
            else
            {
                hci_uart_rx_state = HCI_UART_STATE_RX_TYPE;
            }
           // pDebugPrintfEX((pLOGNOTICE,pLOGHCI," HCI_UART_STATE_RX_HEADER at %d tick,next state = %d, hci_uart_pend_length=%d,hci_uart_rx_length=%d\n", hal_TimGetUpTime(),hci_uart_rx_state,hci_uart_pend_length,hci_uart_rx_length));
            if(hci_uart_rx_length != 0)
            {
                break;
            }
            /* for no payload, direct finish the packet */
        case HCI_UART_STATE_RX_DATA:
	     hci_uart_read_buffer->len += hci_uart_rx_length;
           // pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"Read total %d bytes of %s PDU \n", hci_uart_read_buffer->len,hci_uart_rx_type-1<6?transportTypes[hci_uart_rx_type-1]:"INVALID"));
      		if (HCI_Transport_RecvCallback) 
			{
				HCI_Transport_RecvCallback(hci_uart_read_buffer,hci_uart_rx_type);	      /* NB - The upper layers will free the buffer */      
       		} 
			else
				host_buf_free(hci_uart_read_buffer);        
            hci_uart_rx_state = HCI_UART_STATE_RX_TYPE;
            
            break;
        }

        if(hci_uart_rx_state == HCI_UART_STATE_RX_TYPE)
        {
            hci_uart_rx_buf = hci_uart_rx_head_buf;
	     memset(hci_uart_rx_buf,0,4);
            hci_uart_rx_length = 1;
        }
        hci_uart_pend_length = 0;

	//if (hci_uart_pend_length + length >=  hci_uart_rx_length)
	//	asm("break 1");
    //pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"Receive state = %d\n", hci_uart_rx_state));
    }
    /** save the last data **/
    if(length > 0)
        pMemcpy(hci_uart_rx_buf, pdu, length);
    hci_uart_pend_length = length;
   // pDebugPrintfEX((pLOGNOTICE,pLOGHCI," save the last data at %d tick,next state = %d, hci_uart_pend_length=%d,hci_uart_rx_length=%d\n", hal_TimGetUpTime(),hci_uart_rx_state,hci_uart_pend_length,hci_uart_rx_length));
}

void uart_timer_cb_fc(void *dummy)
{
        need_send_fc=1;
        if(uart_timer)
        {
            pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"uart_timeer_cb_fc uart_timer = %d\n", uart_timer));
            pTimerCancel(uart_timer);
            uart_timer = 0;
        }

      pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"uart_timeer_cb_fc  = %d\n", need_send_fc));

}
extern u_int8 Bt_init_Finished;
extern VOID sxr_Sleep (UINT32 Period);
t_api HCI_Transport_PDU_Tx(struct host_buf *pdu, u_int8 type) 
{
	u_int16 bytesWritten;
	u_int8 pkt_head[5];
    u_int16 pkt_head_len;
//	u_int8 rdabt_FF[1]={0xff};
//	u_int8 num_send;
//	u_int32 i =0;
	pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI_Transport_PDU_Tx is called need_send_fc =%d/n",need_send_fc));

	if(need_send_fc)
	{
	pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_Transport_PDU_Tx need_send_fc =1 ,send wakeup data/n"));

		rda5868_uart_tx((u_int8 *)fc_wakeup_data,sizeof(fc_wakeup_data),&bytesWritten);
  
		sxr_Sleep(210);
	}
	
    if (pdu->flags & PRH_HOST_BUF_HCI_HEADER_PRESENT)
		pkt_head_len=1;
	else
		pkt_head_len=5;
	
	pkt_head[0]=type;
	
	if (pkt_head_len==5)
	{
		pkt_head[1]=pdu->hci_handle_flags & 0xFF;
		pkt_head[2]=pdu->hci_handle_flags >> 8;
		pkt_head[3]=pdu->len & 0xFF;
		pkt_head[4]=pdu->len >> 8;
	}
//pDebugPrintfEX((pLOGNOTICE,pLOGHCI," before BT_TransData pkt_head 0x%x, pkt_head_len:%d\n",pkt_head, pkt_head_len));
    rda5868_uart_tx(pkt_head,pkt_head_len, &bytesWritten);
    if (bytesWritten<pkt_head_len) {
	//pDebugPrintfEX((pLOGNOTICE,pLOGHCI," after BT_TransData bytesWritten:%d\n", bytesWritten));
//asm("break 1");
        /* caller will free the buffer on an error return */
        return BT_HCITERROR;        
    }

	//pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"Wrote %d byte of packet header: 0x%02X (%s)\n", pkt_head_len,type,type-1<6?transportTypes[type-1]:"INVALID"));
	//pDebugDumpEX(pLOGNOTICE,pLOGHCI,pkt_head, pkt_head_len);


    rda5868_uart_tx(pdu->data,pdu->len, &bytesWritten);
    if (bytesWritten<pdu->len) {
        /* caller will free the buffer on an error return */
		pDebugPrintfEX((pLOGNOTICE,pLOGHCI," ERRRRRROOOOOOROR!! " ));
		//asm("break 1");

        return BT_HCITERROR;        
    }
    
   // pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"Wrote total %d bytes of %s PDU, dump: ", pdu->len,type-1<6?transportTypes[type-1]:"INVALID"));
  //  pDebugDumpEX(pLOGNOTICE,pLOGHCI,pdu->data,pdu->len);
//pDebugPrintfEX((pLOGNOTICE,pLOGHCI," HCI_Transport_PDU_Tx block here!! " ));
    HCI_Transport_Free_Buffer(pdu, type);
	
pDebugPrintfEX((pLOGNOTICE,pLOGHCI," finished function HCI_Transport_PDU_Tx " ));
if(Bt_init_Finished)
{
	if(uart_timer != 0)
	{
	  //  pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI_Transport_PDU_Tx  uart_timer canceled!!"));
	    pTimerCancel(uart_timer);
           uart_timer = 0;
		   pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"uart timer is canceled!!"));
	}
	need_send_fc = 0;
    uart_timer = pTimerCreate(60,uart_timer_cb_fc,NULL,pTIMER_ONESHOT);//60 is 6 secend
	if(uart_timer)
		;//pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"uart timer is created!!uart_timer=%d",uart_timer));
	else
		;//pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"uart timer is created FAIL!!"));
}

	return BT_NOERROR;
}

/*
 * This function runs as a seperate task reading from the 
 * serial. It calls the appropriate callback when a full
 * PDU is received.
 */

