/***********************************************************************
 * MODULE NAME:    l2_pkt_handler.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap packet handler
 * AUTHOR:         Gary Fleming
 * DATE:           07-08-1999
 *
 * SOURCE CONTROL: $Id: l2_pkt_handler.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 * Description
 * This module is responsible for handling L2CAP packets.       
 **********************************************************************/


#include "host_config.h"
#include "host_types.h"
#include "host_error.h"

#include "l2_connect_sm.h"
#include "l2_pkt_handler.h"
#include "l2_const.h"
#include "l2_enc_dec.h"
#include "l2_channel.h"
#include "l2_groups.h"
#include "l2_psm.h"

#include "papi.h"

/*******************************************************************************
 *
 * Function :- prh_l2_pkt_handler
 *
 * Parameters 
 *      u_int16 header_cid      The CID contained in the header of the L2CAP packet.    
 *      u_int16 handle          The handle for the ACL link the L2CAP packet was recieved on.
 *      struct host_buf* p_buf  A pointer to the host buf containing the L2CAP packet payload.
 *
 * Returns 
 *      t_api       
 *
 * Description 
 * This function is the main entry into this module. All incoming L2CAP packets
 * are passed through this handler. This function is responsible for determining 
 * the packet type.
 *******************************************************************************/

t_api prh_l2_pkt_handler(u_int16 header_cid,u_int16 handle,struct host_buf* p_buf)
{

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))

u_int8 temp[2];

#endif

    t_api status = BT_NOERROR;
    prh_t_l2_channel* p_channel;
    u_int8 psm_index;
    u_int8 opcode;
    u_int8* p_sig_start;
    u_int8* p_payload;
    t_L2_ControlCallbackArgs  eventinfo;

    if(p_buf->data !=0) 
    {
        switch(header_cid) 
        {
        case L2CAP_NULL_IDENTIFIER :
            status = BT_UNKNOWNERROR;
            host_buf_free(p_buf);
            break;

        case L2CAP_SIGNALLING_CID :
            /********************************************************************
            * This case is responsible for handling L2CAP signalling packets. 
            * As signalling packets may contain multiple signalling PDUs. This 
            * function pulls each signalling PDU in turn from the siganalling 
            * packet and invokes the L2CAP decode functions.
            *********************************************************************/
            p_payload = p_buf->data;
            p_sig_start = p_payload;
            while(p_payload < (p_buf->len + p_sig_start))
            {
                opcode = BT_READ_LE_UINT8(p_payload);
                p_payload++;
                p_payload = prh_l2_decode_packet(handle,opcode,p_payload);
                if (p_payload == 0)
				{
					host_buf_free(p_buf);
                    return BT_UNKNOWNERROR;
				}
            }
            host_buf_free(p_buf);
            break;
#if (L2CAP_CLS_TRAFFIC_SUPPORTED==1)
        case L2CAP_CONNECTIONLESS_DATA :
			{
				u_int16 psm;

				psm = BT_READ_LE_UINT16(p_buf->data);
				host_buf_reserve_header(p_buf,sizeof(u_int16));
				if (PRH_L2_CLT_ENABLED == prh_l2_psm_get_clt_state(psm))
				{
					eventinfo.cid = header_cid;
					eventinfo.p_buf = p_buf;
					
					psm_index = prh_l2_psm_get_psm_ident(psm);
					if (psm_index)
						prh_l2_psm_event(psm_index,L2CAP_evDATAIND, &eventinfo);
					else
						host_buf_free(p_buf);
				}
				else
					host_buf_free(p_buf);
			}
            break;
#endif

        default : 
            if (header_cid >= PRH_L2_MIN_USER_CID) 
            { 
                p_channel = prh_l2_chn_get_p_channel(header_cid);
                if (p_channel == 0)
                {
#if pDEBUG
                    pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Invalid User Data CID %d\n", header_cid));

#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                    BT_WRITE_LE_UINT16(temp,header_cid);
                    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_PKT_HANDLER_INDEX),temp,2,BT_CODE_WORD);
#endif
                    host_buf_free(p_buf);
                    status = BT_UNKNOWNERROR;
                }
                else
                {
                    eventinfo.cid = header_cid;
                    eventinfo.p_buf = p_buf;

                    psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
                    prh_l2_psm_event(psm_index,L2CAP_evDATAIND, &eventinfo);
                }
            }
            else
            {
#if pDEBUG
                pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: Invalid User Data CID %d\n",header_cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                BT_WRITE_LE_UINT16(temp,header_cid);
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_PKT_HANDLER_INDEX),temp,2,BT_CODE_WORD);
#endif
                host_buf_free(p_buf);    
                status = BT_UNKNOWNERROR;
            }                 
            break;
        }
    }
    else 
    {
        status = BT_NOERROR;
    }
    return status;
}





