/**************************************************************************
 * MODULE NAME:    l2_sar.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap Segmentation and Re-assembly Function
 * MAINTAINER:         Gary Fleming
 * DATE:           02-12-1999
 *
 * SOURCE CONTROL: $Id: l2_sar.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 *
 * SOURCE: 
 *
 *************************************************************************/

#include "host_config.h"

#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"
#include "l2cap_lower_layer.h"
#include "l2_connect_sm.h"
#include "l2_const.h"
#include "l2_pkt_handler.h"
#include "l2_enc_dec.h"
#include "l2_sar.h"
#include "l2_acl_link.h"
#include "l2_groups.h"
#include "l2_channel.h"
#include "l2_psm.h"
#include "l2_layer_config.h"

#include "papi.h"

/*
 * The manager startup will send a readbuffersize command and the response 
 * is handled in l2cap_ch.c where this variable is set to the chunk size 
 * -MM
 */
int g_l2cap_acl_size= 0;

/*************************************************************************************
 * Function L2_Sar_Data_Ind
 *
 * Description 
 * This function is responsible for the receipt of incoming data from the lower layer
 * and re-assemblying it into L2CAP packets. These are then delivered to the L2CAP 
 * packet handler.
 *
 *************************************************************************************/   

t_api  prh_l2_sar_data_ind(u_int16 handle, struct host_buf* incoming_buffer, u_int8 flags)
{
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    u_int8 temp[4];
#endif

    prh_t_l2_channel* p_channel;
    prh_t_l2_acl_link* p_link;
    u_int8* p_data;
    u_int32 length;
    u_int8 psm_index;
    t_L2_ControlCallbackArgs  eventinfo;
	t_api ret;
 
    p_link = prh_l2_acl_find_handle(handle);
    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_l2_sar_data_ind handle=%d,incoming_buffer=0x%x,flags=%d,p_link=0x%x\n",handle,incoming_buffer,flags,p_link));
    if (p_link == 0)
	{
		ret = BT_UNKNOWNERROR;
        goto SAR_IND_ERROR_HANDLER;
	}

    p_data = incoming_buffer->data;
    length = incoming_buffer->len;

    pMemset(&eventinfo,0,sizeof(t_L2_ControlCallbackArgs));

#if pDEBUG
    //pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP: L2_Sar_Data_Ind len=%d flag=%d\n", length,flags));
   {
        pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"len:%d, data[0-15]:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",
                                                                          length, p_data[0],p_data[1],p_data[2],p_data[3],p_data[4],p_data[5],p_data[6],p_data[7],p_data[8], p_data[9],p_data[10],p_data[11],p_data[12],p_data[13],p_data[14], p_data[15] ));
    }
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    BT_WRITE_LE_UINT32(temp,length);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_SAR_INDEX),temp,4,BT_CODE_DWORD);
#endif

    if (flags&prh_hci_ACL_START_FRAGMENT)
    { 
#if COMBINED_HOST==0
		/* Note for Point to Multipoint mtu_complete length has to be assoiciated with the p_link */
        if (!p_link->mtu_complete) 
		{
            pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP SAR: Recieved START fragment before previous MTU reassembly complete - dropping previous MTU!\n"));

			if (p_link->pending_cid != L2CAP_CONNECTIONLESS_DATA)
			{
				p_channel  = prh_l2_chn_get_p_channel(p_link->pending_cid);
				if (p_channel->outFlush == 0xFFFF)
				{
					psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
					eventinfo.cid=p_channel->local_cid;
					prh_l2_psm_event(psm_index,L2CAP_evQOSVIOLATIONIND,&eventinfo);
				}
			}
        }
#endif
        p_link->mtu_complete=0;

        /* Initialise elements of p_link                */
        
	    p_link->length = BT_READ_LE_UINT16(p_data);         /* Total L2CAP Packet Length            */
        p_link->pending_cid = BT_READ_LE_UINT16(p_data+2);  /* Get the L2CAP CID of the information */
        p_link->cur_len = 0;

        /* Sanity check on the packet length */
#if COMBINED_HOST==0
		if (p_link->length < length-4)
		{
		        pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Length field mismatch - Expected Packet Length =%d , Total Length %d\n",p_link->length, length-4));
                p_channel  = prh_l2_chn_get_p_channel(p_link->pending_cid);
                if (p_channel && p_channel->outFlush == 0xFFFF)
                {
                    psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
                    eventinfo.cid=p_channel->local_cid;
                    prh_l2_psm_event(psm_index,L2CAP_evQOSVIOLATIONIND,&eventinfo);
                }
				p_link->mtu_complete=1;
				ret = BT_PROTOERROR;
                goto SAR_IND_ERROR_HANDLER; /* Incoming Data above MTU size */
        }
#endif
        switch(p_link->pending_cid)
        {

        case L2CAP_SIGNALLING_CID :
            if (p_link->length > (PRH_L2_MAX_SIGNALLING_MTU + 4))
            {
            #if pDEBUG
               // pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Incoming Signalling Packet Above MTU Size =%d\n",p_link->length));
           #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                BT_WRITE_LE_UINT16(temp,p_link->length);
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_SAR_INDEX+1),temp,2,BT_CODE_WORD);
            #endif
				p_link->mtu_complete=1;
				ret = BT_PROTOERROR;
                goto SAR_IND_ERROR_HANDLER;
            }
            break;

        case L2CAP_CONNECTIONLESS_DATA :
            if (p_link->length > (PRH_L2_MAX_CLS_MTU + 4))
            {
            #if pDEBUG
                pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Incoming Connectionless Packet Above MTU Size =%d\n",p_link->length));
            #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                BT_WRITE_LE_UINT16(temp,p_link->length);
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_SAR_INDEX+2),temp,2,BT_CODE_WORD);
            #endif
				p_link->mtu_complete=1;
				ret = BT_PROTOERROR;
                goto SAR_IND_ERROR_HANDLER;
            }
            break;

        default :
            p_channel =prh_l2_chn_get_p_channel(p_link->pending_cid); 

            if(p_channel == 0)
            {
            #if pDEBUG
                pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Incoming Data on Invalid CID %d,handle %d,p_link 0x%x \n",p_link->pending_cid,handle,p_link));
            #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_SAR_INDEX+3),NULL,0,0);
            #endif
				p_link->mtu_complete=1;
				ret = BT_INVALIDPARAM;
                goto SAR_IND_ERROR_HANDLER;
            }
             if(p_link->length > p_channel->inMTU)
            { 
             #if pDEBUG
                pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Incoming Fragment Exceeds MTU Packet Length - Expected Packet Length =%d , Total Length %d\n",p_link->length,p_channel->inMTU));
             #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                BT_WRITE_LE_UINT16(temp,p_link->length);
                BT_WRITE_LE_UINT16(temp,p_channel->inMTU);
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_SAR_INDEX+4),temp,4,(BT_CODE_WORD|(BT_CODE_WORD<<4)));   

             #endif
             
                p_channel  = prh_l2_chn_get_p_channel(p_link->pending_cid);
                if (p_channel->outFlush == 0xFFFF)
                {
                    psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
                    eventinfo.cid=p_channel->local_cid;
                    prh_l2_psm_event(psm_index,L2CAP_evQOSVIOLATIONIND,&eventinfo);
                }
				p_link->mtu_complete=1;
				ret = BT_PROTOERROR;
                goto SAR_IND_ERROR_HANDLER; /* Incoming Data above MTU size */
            }
            break;
        }
        
         /* Allocate & Init Host Buffer                  */

#if COMBINED_HOST==0
         p_link->cur_buf = host_buf_alloc(p_link->length+UL_HEADER_SIZE);
         if (p_link->cur_buf==0)
		 {
			 ret = BT_UNKNOWNERROR;
			 goto SAR_IND_ERROR_HANDLER;
		 }
#else
 	  p_link->cur_buf = incoming_buffer;
         host_buf_reserve_header(p_link->cur_buf, 4);
#endif
         /* Length of Data = Length - Header Size        */
     
         length = length-4;
         p_data += 4;  
        
         /* Only reserve header spaced if it is a User Data PDU    */

         if(p_link->pending_cid != L2CAP_SIGNALLING_CID)
             host_buf_reserve_header(p_link->cur_buf,UL_HEADER_SIZE);
		 
		 p_link->cur_buf->len = p_link->length;
         p_link->cur_pos = p_link->cur_buf->data;

         /* Copy the Data into the Buffer */
#if COMBINED_HOST==0

         pMemcpy(p_link->cur_pos,p_data,length);
         host_buf_free(incoming_buffer);
#endif

         p_data-=4;
         
         /* Advance the current position  */

         p_link->cur_pos+=length;

         p_link->cur_len = p_link->cur_len+length;

         if(p_link->length == length) /* One Segment Packet */
         {
         #if pDEBUG
                //pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Single Segment Data Packet: handle=0x%x,len=%d,hci_length=%d\n", handle, p_link->cur_buf->len,p_link->cur_buf->hci_length));
         #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
             SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_SAR_INDEX+5),NULL,0,0);
         #endif
			 p_link->cur_pos=NULL;
             p_link->mtu_complete=1;
             prh_l2_pkt_handler(p_link->pending_cid,handle,p_link->cur_buf);
	     
         }
         
     }
#if COMBINED_HOST==0
     else if (flags&prh_hci_ACL_CONTINUE_FRAGMENT)
     {
		 /* UPF5 - Check are we meant to be getting a CONTINUE */
		 if (p_link->cur_pos==NULL)
		 {
			 //pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Unexpected Continue Fragment - junking\n"));
			 host_buf_free(incoming_buffer);
			 p_link->mtu_complete=1;
			 
			 /* don't know any CID so can't do anything else */
			 return BT_NOERROR;
		 }

         if ((p_link->cur_len+length) > p_link->length)
         {
             host_buf_free(p_link->cur_buf);
		 	 p_link->cur_pos=NULL;
             pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Incoming Fragment Exceeds Packet Length - Expected Packet Length =%d , Total Length \n",p_link->length,p_link->cur_len+length));
             p_channel  = prh_l2_chn_get_p_channel(p_link->pending_cid);
             if (p_channel->outFlush == 0xFFFF)
             {
                psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
                eventinfo.cid=p_channel->local_cid;
                prh_l2_psm_event(psm_index,L2CAP_evQOSVIOLATIONIND,&eventinfo);
             }
			 p_link->mtu_complete=1;
			 /* Was Bug here - the incoming buffer was not released */
			 host_buf_free(incoming_buffer);
	         return BT_NOERROR;
         }
         else
         { 
             /* Copy the Data into the Buffer */

             pMemcpy(p_link->cur_pos,p_data,length);
             host_buf_free(incoming_buffer);
         }
 
         p_link->cur_len += length;

         /* Is this the last segment      */
            
         if (p_link->cur_len == p_link->length)
         {
             pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP: End of Multi-Segment Data Packet %d\n",p_link->cur_buf->len));
	         p_link->cur_pos=NULL;         
             p_link->mtu_complete=1;
	         prh_l2_pkt_handler(p_link->pending_cid,handle,p_link->cur_buf);
	 
         }
         else
         {         
             /* Advance the current position by length */
             p_link->cur_pos+=length; 
         }            
     }
     return BT_NOERROR;
#endif	 
SAR_IND_ERROR_HANDLER :
	   host_buf_free(incoming_buffer);
          pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"SAR_IND_ERROR_HANDLER,handle %d,p_link 0x%x \n",handle,p_link));
	   if (ret != BT_PROTOERROR)
	   {
	   	struct st_t_BT_ConnectionInfo *dbEntry;
		u_int8 linkType;
		u_int8 i;
	    	dbEntry=prh_mgr_get_deviceEntryByHandle(handle, &linkType);
		if (dbEntry !=NULL)
	   	{
			void (*tempFunc_t)(struct st_t_BT_ConnectionInfo *dbEntry_t, t_api status_t);
			pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"COMPLETE aclReleaseCallbackFunc will be called\n"));

			do 
			{
				prh_mgr_get_callback(&i,(void **)dbEntry->aclReleaseCallbackFunc,(void **)&tempFunc_t,dbEntry);
				pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"aclReleaseCallbackFunc tempFunc_t = 0x%x\n",tempFunc_t));
				if (tempFunc_t)
					tempFunc_t(dbEntry, ret); 
			} while (tempFunc_t);				
		}
		else
		{
	   		pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"prh_l2_sar_data_ind error dbEntry is NULL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"));
		}
	    }
	   pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"prh_l2_sar_data_ind error !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"));
	   return ret;
}

/*************************************************************************************
 * Function prh_l2_sar_data_req
 *
 * Description 
 * This function is responsible for the transmission of outgoing data to the lower layer
 * and segmenting it into L2CAP packets. These are then delivered to the L2CAP 
 * packet handler.
 *
 *************************************************************************************/
t_api prh_l2_sar_data_req(u_int16 handle, u_int16 cid, struct host_buf* outgoing_buffer)
{

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    u_int8 temp[6];
#endif
    t_api status = BT_NOERROR;
    prh_t_l2_channel* p_channel=NULL;
    u_int32 remaining_length;
#if COMBINED_HOST==0
    u_int32 hc_chunk_size = g_l2cap_acl_size;
#endif
    u_int8* cur_ptr;
    struct host_buf *p_buf;
    u_int16 payload_len;
    struct host_buf *fragment;
    struct host_buf *last_frag;

    u_int8 aclflags=0;

    p_buf = outgoing_buffer;

#if COMBINED_HOST==0
    if (hc_chunk_size==0)
    {
        pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2_Sar_Data_Req(): HCI chunk size not initialised - aborting\n"));
        return BT_PROTOERROR; 
    }
#endif	 

    if (cid != L2CAP_SIGNALLING_CID)
    {
        /* Determine the connection handle and remote CID from
           the local CID */
            
        p_channel =prh_l2_chn_get_p_channel(cid);
        if(p_channel != 0x0 )
        {
            if (p_channel->type == PRH_L2_CO_CHANNEL)
            {
                if(p_channel->state != PRH_L2_OPEN ) 
                {   /* Is data Tx suspended on the link */
                #if pDEBUG
                    pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Attempt to send on channel 0x%02X when data Tx suspended\n", cid));
                #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                    BT_WRITE_LE_UINT16(temp,cid);
                    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_SAR_INDEX+6),temp,2,BT_CODE_WORD);
               #endif
                    return BT_RETRY;
                } 
                else
                    handle = p_channel->handle;

                if(outgoing_buffer->len > p_channel->outMTU)
                {
                #if pDEBUG
                    pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Tx CO data of 0x%02X bytes is above MTU of 0x%02X bytes\n", outgoing_buffer->len,p_channel->outMTU));
                 #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                    BT_WRITE_LE_UINT32(temp,outgoing_buffer->len);
                    BT_WRITE_LE_UINT16(temp+4,p_channel->outMTU);
                    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_SAR_INDEX+7),temp,6,(BT_CODE_DWORD|(BT_CODE_WORD<<4)));
                #endif
                    return BT_INVALIDPARAM; 
                }
            }
            else if(p_channel->type == PRH_L2_CLS_CHANNEL)
            {
                if (outgoing_buffer->len > PRH_L2_MAX_CLS_MTU)
                {
                #if pDEBUG
                    pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Tx CLS data of 0x%02X bytes is above MTU of 0x%02X bytes\n", outgoing_buffer->len,PRH_L2_MAX_CLS_MTU));
                #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                    BT_WRITE_LE_UINT32(temp,outgoing_buffer->len);
                    BT_WRITE_LE_UINT16(temp+4,PRH_L2_MAX_CLS_MTU);
                    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_SAR_INDEX+8),temp,6,(BT_CODE_DWORD|(BT_CODE_WORD<<4))); 
                #endif
                    return BT_INVALIDPARAM;
                }
#if L2CAP_USE_PICONET_BROADCAST
                aclflags=prh_hci_ACL_PICONET_BROADCAST;
#else
                aclflags=prh_hci_ACL_ACTIVE_BROADCAST; 
#endif
                handle= PRH_L2_DEFAULT_CONNECTIONLESS_HANDLE ;
            }
        } 
        else 
        {
        #if pDEBUG
            pDebugPrintfEX((pLOGERROR,pLOGL2CAP," L2CAP:prh_l2_chn_get_p_channel returned NULL for cid 0x%02X\n",cid));
        #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
            BT_WRITE_LE_UINT16(temp,cid);
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_SAR_INDEX+9),temp,2,BT_CODE_WORD);                  
        #endif
            return BT_INVALIDPARAM; 
        }

    }
    /* Adjust the payload position to accomodate the L2CAP header */

     if(cid == L2CAP_SIGNALLING_CID)
     {
        payload_len = p_buf->len;
        host_buf_release_header(p_buf,L2CAP_PDU_HEADER_LENGTH);  /* Header size is 4 bytes*/

        remaining_length = p_buf->len;
        cur_ptr = p_buf->data;

        /* Set up the Header */

        BT_WRITE_LE_UINT16(cur_ptr,payload_len);
        BT_WRITE_LE_UINT16((cur_ptr+2),cid);
     }
     else
     {
         if (p_channel->type == PRH_L2_CLS_CHANNEL)
         {
            payload_len = p_buf->len + 2;
            host_buf_release_header(p_buf,L2CAP_PDU_HEADER_LENGTH + 2);

            remaining_length = p_buf->len;
            cur_ptr = p_buf->data;

            /* Set up the Header */

            BT_WRITE_LE_UINT16(cur_ptr,payload_len);
            BT_WRITE_LE_UINT16((cur_ptr+2),L2CAP_CONNECTIONLESS_DATA);
            BT_WRITE_LE_UINT16((cur_ptr+4),p_channel->psm);
         }
         else
         {
            payload_len = p_buf->len;
            host_buf_release_header(p_buf,L2CAP_PDU_HEADER_LENGTH);  /* Header size is 4 bytes*/

            remaining_length = p_buf->len;
            cur_ptr = p_buf->data;

            /* Set up the Header  */

            BT_WRITE_LE_UINT16(cur_ptr,payload_len);
            BT_WRITE_LE_UINT16((cur_ptr+2),p_channel->remote_cid);
         }
     }
#if COMBINED_HOST==1
    host_buf_FreeWriteBuffer(p_buf);
#else
     /* Below this point is SARing - D.A. */
	 
     if(remaining_length <= hc_chunk_size) /* Check if segmentation is needed */
	 {
		 /* no need to set up fragment */
		 /* write the PDU to the HCI straight off */
		 p_buf->flags &= ~PRH_HOST_BUF_HCI_HEADER_PRESENT;
		 status=prh_hci_send_acl_child_list(handle, p_buf, aclflags);
		 
	 }
     else /* Segmentation Required */
	 {
		 /* need to loop through pdu and set up the child buffers */
		 /* then enqueue the list to the HCI in one swoop */
		 /* really need to request queue space from HCI - and then enqueue into it if available */
		 last_frag = p_buf;
		 do 
		 {
			 u_int32 length_to_send;
			 
			 
			 length_to_send = hc_chunk_size;
			 if (remaining_length<length_to_send)
				 length_to_send=remaining_length;
			 
			 remaining_length -= length_to_send;
			 
			 fragment=host_buf_child_alloc(p_buf, cur_ptr, length_to_send);
			 last_frag->next=fragment;
			 
			 cur_ptr += length_to_send;
			 last_frag=fragment;
		 } while (remaining_length>0);
		 
		 /* send list to HCI */
		 status=prh_hci_send_acl_child_list(handle, p_buf, aclflags);
		 if (status!=BT_NOERROR)
		 {
			 /* set fragment to first child buffer */
			 fragment=p_buf->next;
			 do 
			 {
				 /* set last frag to current */
				 last_frag=fragment;
				 /* set current to next */
				 fragment=last_frag->next;
				 /* free last fragment */
				 host_buf_free(last_frag);
			 }
			 while (fragment);
			 host_buf_reserve_header(p_buf,L2CAP_PDU_HEADER_LENGTH);
		 }
		 else
		 {
			 prh_hci_FreeWriteBuffer(p_buf);
			 
		 }
     }
#endif     
     return status;
}

