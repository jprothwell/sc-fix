/**************************************************************************
 * MODULE NAME:    l2_enc_dec.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap Encode and Decode routines
 * MAINTAINER:     Gary Fleming
 * DATE:           11-08-1999
 *
 * SOURCE CONTROL: $Id: l2_enc_dec.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 *
 * Description :
 * This module is responsible for encoding and decoding L2CAP PDUs.
 *
 * The PDU information is decoded into both the p_channel (if applicable) and
 * a pdu_info structure.
 *
 * PDU informaiton is encoded into the host buffer using the informaiton contained
 * in the pdu_info structure and the p_channel.
 **************************************************************************/

#include "host_config.h"
#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"

#include "papi.h"
#include "l2cap_lower_layer.h"
#include "l2_types.h"
#include "l2_connect_sm.h"
#include "l2_const.h"
#include "l2_enc_dec.h"
#include "l2_sar.h"
#include "l2_acl_link.h"
#include "l2_channel.h"
#include <assert.h>          
#include <string.h>


u_int8 l2_cur_ident= 0x44;

u_int8 prh_l2_enc_derive_identifier(void);
static u_int8* prh_l2_enc_insert_options(u_int8* p_pdu,prh_t_l2_pdu* p_pdu_info);
static t_api prh_l2_enc_get_options(u_int8* p_pdu,prh_t_l2_pdu* p_pdu_info);
t_api prh_l2_enc_single_pdu(u_int8* p_pdu,u_int8 opcode,prh_t_l2_pdu* p_pdu_info);
static u_int8 L2_Use_C_Bit = 0x00;

void L2_Set_C_Bit(void)
{
    L2_Use_C_Bit = 0x01;
}

/********************************************************
 * Function : prh_l2_encode_packet
 *
 * Parameters : 
 *     handle :- ACL handle for the link for Tx
 *     opcode :- The opcode of the L2CAP PDU to be Tx
 *     p_pdu_info :- A pointer to a structure containing the 
 *                   parameters of the PDU to be Tx-ed.
 *
 * Returns
 *      t_api       BT_NOERROR
 *                  BT_UNKNOWNERROR
 * Description
 * This function is used to encode L2CAP PDUs for Tx to the 
 * peer. The PDU is encoded into a host_buf which is then 
 * sent to the SAR using L2_Sar_Data_Req
 *
 ********************************************************/

t_api prh_l2_encode_packet(u_int16 handle,u_int8 opcode, prh_t_l2_pdu* p_pdu_info)
{

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
u_int8 temp[4];
#endif

    u_int8* p_pdu;
    u_int8* end_p_pdu;
    t_api status = BT_NOERROR;
    u_int16 buf_len;
    u_int16 payload_len;
    struct host_buf *p_buf;
    int ret;
    prh_t_l2_channel* p_channel;

    if(opcode > MAX_L2CAP_OPCODES)
    {
    #if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP Enc_Dec : Error - Encode of PDU with invalid opcode \n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
         SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX),NULL,0,0);
    #endif
        return BT_UNKNOWNERROR; 
    }
    p_channel = p_pdu_info->p_channel;

    payload_len = p_pdu_info->length;
    buf_len = payload_len+L2CAP_SIG_HEADER_LENGTH;

    ret=prh_hci_GetWriteBuffer(handle, (u_int16)(buf_len+L2CAP_PDU_HEADER_LENGTH), 0, &p_buf);

    if (ret!=BT_NOERROR)
    {
        #if pDEBUG
	    pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP Enc_Dec : Error - Memory could not be allocated for Signalling Packet \n"));
        #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
           SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+1),NULL,0,0);
        #endif
        return ret; 
    }
    host_buf_reserve_header(p_buf,L2CAP_PDU_HEADER_LENGTH);

    p_pdu = p_buf->data;

    if (p_pdu) 
    {
        BT_WRITE_LE_UINT8(p_pdu,opcode);
        BT_WRITE_LE_UINT16((p_pdu+2),payload_len);

        switch(opcode)
        {
        case L2CAP_COMMAND_REJECT :
            /******************************************************
             *  Code       - 1 Byte
             *  Identifier - 1 Byte
             *  Length     - 2 Bytes
             *  PSM        - 2 Bytes
             *  Source CID - 2 Bytes
             ******************************************************/
#if pDEBUG
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP - Sent L2CAP COMMAND REJECT, reason: 0x%02X",p_pdu_info->reason));
#endif

            BT_WRITE_LE_UINT8((p_pdu+1),p_pdu_info->identifier);
            BT_WRITE_LE_UINT16((p_pdu+4),p_pdu_info->reason);
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))            
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+2),(p_pdu+4),2,BT_CODE_WORD);
 #endif
            /* Note :- The previously written length field (@ p_pdu+2) is overwritten here */
            switch(p_pdu_info->reason)
            {
            case L2CAP_REJECT_SIGNALLING_MTU_EXCEEDED :
                BT_WRITE_LE_UINT16((p_pdu+2), 0x04);
                BT_WRITE_LE_UINT16((p_pdu+6),p_pdu_info->mtu);
#if pDEBUG
                pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP - SIGNALLING_MTU_EXCEEDED"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+3),NULL,0,0);

#endif
                break;
            case L2CAP_REJECT_INVALID_CID_IN_REQUEST :
                BT_WRITE_LE_UINT16((p_pdu+2), 0x06);
                BT_WRITE_LE_UINT16((p_pdu+6),p_pdu_info->src_cid);
                BT_WRITE_LE_UINT16((p_pdu+8),p_pdu_info->dest_cid);
#if pDEBUG
                pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP - INVALID_CID_IN_REQUEST"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+4),NULL,0,0);
#endif
                break;
            default :
                BT_WRITE_LE_UINT16((p_pdu+2), 0x02);
#if pDEBUG
                pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP - COMMAND REJECT"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+5),NULL,0,0);
#endif
                break;
            }
            break;
        case L2CAP_CONNECTION_REQUEST :
            /*********************************************************
             *  Code       - 1 Byte
             *  Identifier - 1 Byte
             *  Length     - 2 Bytes
             *  PSM        - 2 Bytes
             *  Source CID - 2 Bytes
             ******************************************************/
#if pDEBUG
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONNECTION_REQUEST - psm=0x%02X, local cid=0x%02X\n",p_channel->psm,p_channel->local_cid));
#endif
            BT_WRITE_LE_UINT8((p_pdu+1), p_channel->local_transaction_identifier);
            BT_WRITE_LE_UINT16((p_pdu+4),p_channel->psm);
            BT_WRITE_LE_UINT16((p_pdu+6),(p_channel->local_cid));

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
          
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+6),(p_pdu+4),4,(BT_CODE_WORD |( BT_CODE_WORD<<4)));
#endif
            break;

        case L2CAP_CONNECTION_RESPONSE :
            /******************************************************  
             *  Code       - 1 Byte
             *  Identifier - 1 Byte
             *  Length     - 2 Bytes
             *  Dest   CID - 2 Bytes
             *  Source CID - 2 Bytes
             *  Result     - 2 Bytes
             *  Status     - 2 Bytes
             ******************************************************/
#if pDEBUG
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONNECTION_RESPONSE - local cid=0x%02X, remote cid=0x%02X, result=0x%02X, status=0x%02X\n",p_channel->local_cid,p_channel->remote_cid,p_pdu_info->result,p_pdu_info->status));
           
#endif
            BT_WRITE_LE_UINT8((p_pdu+1), p_channel->remote_transaction_identifier);
            BT_WRITE_LE_UINT16((p_pdu+4),p_channel->local_cid);
            BT_WRITE_LE_UINT16((p_pdu+6),p_channel->remote_cid);
            BT_WRITE_LE_UINT16((p_pdu+8),p_pdu_info->result);
            BT_WRITE_LE_UINT16((p_pdu+10),p_pdu_info->status);

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+7),(p_pdu+4),8,((BT_CODE_WORD) |(BT_CODE_WORD<<4) |(BT_CODE_WORD<<8) |(BT_CODE_WORD<<12)));
#endif
            break;

        case L2CAP_CONFIGURE_REQUEST :
            /*******************************
             *  Code       - 1 Byte
             *  Identifier - 1 Byte
             *  Length     - 2 Bytes
             *  Dest   CID - 2 Bytes
             *  Flags      - 2 Bytes
             *  Optional   - 2 Bytes
             *******************************/
#if pDEBUG
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONFIGURE_REQUEST - remote cid=0x%02X\n",p_channel->remote_cid));
#endif
            BT_WRITE_LE_UINT8((p_pdu+1), p_channel->local_transaction_identifier);
            BT_WRITE_LE_UINT16((p_pdu+4),p_channel->remote_cid);

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+8),(p_pdu+4),2,BT_CODE_WORD );
#endif
            if ( L2_Use_C_Bit && (p_pdu_info->num_options >= 1))
            {
                BT_WRITE_LE_UINT16((p_pdu+6), PRH_L2_CONFIG_CONTINUE_ON); 
                L2_Use_C_Bit = 0x00;
            }
            else
            {
                BT_WRITE_LE_UINT16((p_pdu+6),0 /* Non-Segmented configuration */ ); 
            }
            if(p_pdu_info->num_options)
            {
                end_p_pdu = prh_l2_enc_insert_options(p_pdu+8,p_pdu_info);
                BT_WRITE_LE_UINT16((p_pdu+2),(end_p_pdu - (p_pdu+4)));
            }
            prh_l2_chn_start_RTX_timer(p_channel);
            break;

        case L2CAP_CONFIGURE_RESPONSE :

            /******************************************************
             *  Code       - 1 Byte
             *  Identifier - 1 Byte
             *  Length     - 2 Bytes
             *  Source CID - 2 Bytes
             *  Flags      - 2 Bytes
             *  Results    - 2 Bytes
             *  Optional   - 2 Bytes
             ******************************************************/
#if pDEBUG
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONFIGURE_RESPONSE - remote cid=0x%02X\n",p_channel->remote_cid));
#endif
            BT_WRITE_LE_UINT8((p_pdu+1), p_channel->remote_transaction_identifier);
            BT_WRITE_LE_UINT16((p_pdu+4),p_channel->remote_cid);   
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+9),(p_pdu+4),2,BT_CODE_WORD);
#endif
            if (p_pdu_info->flags)
            {
                BT_WRITE_LE_UINT16((p_pdu+6),PRH_L2_CONFIG_CONTINUE_ON /* Segmented configuration */ ); 
            }
            else
            {
                BT_WRITE_LE_UINT16((p_pdu+6),PRH_L2_CONFIG_CONTINUE_OFF /* Non-Segmented configuration */ );
            }
            BT_WRITE_LE_UINT16((p_pdu+8),p_pdu_info->result); 

            if(p_pdu_info->num_options)
            {
                end_p_pdu = prh_l2_enc_insert_options(p_pdu+10,p_pdu_info);
                BT_WRITE_LE_UINT16((p_pdu+2),(end_p_pdu - (p_pdu+4)));
            }
            break;

        case L2CAP_DISCONNECTION_REQUEST :
            /******************************************************
             *  Code       - 1 Byte
             *  Identifier - 1 Byte
             *  Length     - 2 Bytes
             *  Dest CID   - 2 Bytes
             *  Source CID - 2 Bytes
             ******************************************************/
#if pDEBUG
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_DISCONNECTION_REQUEST - local cid=0x%02X, remote cid=0x%02X\n",p_channel->local_cid,p_channel->remote_cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
            BT_WRITE_LE_UINT16(temp,p_channel->local_cid);
            BT_WRITE_LE_UINT16((temp+2),p_channel->remote_cid);
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+10),temp,4,(BT_CODE_WORD |( BT_CODE_WORD<<4)));
#endif


            BT_WRITE_LE_UINT8((p_pdu+1), p_channel->local_transaction_identifier);
            BT_WRITE_LE_UINT16((p_pdu+4),p_channel->remote_cid);
            BT_WRITE_LE_UINT16((p_pdu+6),p_channel->local_cid); 

            break;

        case L2CAP_DISCONNECTION_RESPONSE :
            /******************************************************
             *  Code       - 1 Byte
             *  Identifier - 1 Byte
             *  Length     - 2 Bytes
             *  Dest CID   - 2 Bytes
             *  Source CID - 2 Bytes
             ******************************************************/
#if pDEBUG
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_DISCONNECTION_RESPONSE - local cid=0x%02X, remote cid=0x%02X\n",p_channel->local_cid,p_channel->remote_cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
            BT_WRITE_LE_UINT16(temp,p_channel->local_cid);
            BT_WRITE_LE_UINT16((temp+2),p_channel->remote_cid);
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+11),temp,4,(BT_CODE_WORD|(BT_CODE_WORD<<4)));
#endif
            BT_WRITE_LE_UINT8((p_pdu+1), p_channel->remote_transaction_identifier);
            BT_WRITE_LE_UINT16((p_pdu+4),p_channel->local_cid);
            BT_WRITE_LE_UINT16((p_pdu+6),p_channel->remote_cid);  
            break;

        case L2CAP_ECHO_REQUEST :
        case L2CAP_ECHO_RESPONSE :
            /********************************************
             *  Code       - 1 Byte
             *  Identifier - 1 Byte
             *  Length     - 2 Bytes
             *  Data       - X Bytes [ Optional ]
             **********************************************/
            if (opcode == L2CAP_ECHO_REQUEST)
            {
#if pDEBUG
                pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP - Send ECHO_REQUEST"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+12),NULL,0,0);
#endif
                prh_l2_acl_start_RTX_timer(handle);
            }
            else
#if pDEBUG
                pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP - Send ECHO_RESPONSE, length: 0x%02X",p_pdu_info->length));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                BT_WRITE_LE_UINT16(temp,p_pdu_info->length);
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+13),temp,2,BT_CODE_WORD);
#endif
            BT_WRITE_LE_UINT8((p_pdu+1),p_pdu_info->identifier);
            pMemcpy((p_pdu+4),p_pdu_info->p_data,p_pdu_info->length);
            break;

#if (L2CAP_INFO_SUPPORTED==1)
        case L2CAP_INFORMATION_REQUEST :
#if pDEBUG
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_INFORMATION_REQUEST \n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+14),NULL,0,0);
#endif
            BT_WRITE_LE_UINT8((p_pdu+1),p_pdu_info->identifier);
            if(p_pdu_info->info_type == PRH_L2_CONNECTIONLESS_MTU_INFOTYPE)
            {
                BT_WRITE_LE_UINT16((p_pdu + 4),PRH_L2_CONNECTIONLESS_MTU_INFOTYPE);
#if pDEBUG
                pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Info Type Requested = CONNECTIONLESS MTU \n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+15),NULL,0,0);

#endif                
            }
            else
            {
				BT_WRITE_LE_UINT16((p_pdu + 4),p_pdu_info->info_type);
#if pDEBUG
                pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Info Type Requested = UNKNOWN \n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+16),NULL,0,0);
#endif
            }
            prh_l2_acl_start_RTX_timer(handle);
            break;

        case L2CAP_INFORMATION_RESPONSE :
#if pDEBUG            
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_INFORMATION_RESPONSE - type=0x%02X, result=0x%02X\n",p_pdu_info->info_type,p_pdu_info->result));
#endif
            BT_WRITE_LE_UINT8((p_pdu+1),p_pdu_info->identifier);
            BT_WRITE_LE_UINT16((p_pdu+4),p_pdu_info->info_type);
            BT_WRITE_LE_UINT16((p_pdu+6),p_pdu_info->result); 

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
             SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+17),(p_pdu+4),4,(BT_CODE_WORD|(BT_CODE_WORD<<4)));
 #endif
            if (p_pdu_info->info_type == PRH_L2_CONNECTIONLESS_MTU_INFOTYPE)
            {
                BT_WRITE_LE_UINT16((p_pdu + 8),p_pdu_info->mtu);
#if pDEBUG
                pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Info Type Recieved = CONNECTIONLESS MTU \n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+18),NULL,0,0);
#endif
            }
            else
            {
#if pDEBUG
                pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP Enc_Dec : Info Type Recieved = UNKNOWN \n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
               SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+19),NULL,0,0);
#endif
            }
            break;
#endif
        default:
			prh_hci_FreeWriteBuffer(p_buf);
			
            return BT_UNKNOWNERROR;
            break;
        }
        p_buf->len = buf_len;
    }
    status = prh_l2_sar_data_req(handle,L2CAP_SIGNALLING_CID,p_buf);
    if (status != BT_NOERROR)
    {
#if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP Enc_Dec : SAR Returned Error \n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+20),NULL,0,0);
#endif
		prh_hci_FreeWriteBuffer(p_buf);
    }
    
    return status;
}

/********************************************************
 * Function : prh_l2_decode_packet
 *
 * Parameters : 
 *      handle :-   ACL handle for the link for Tx
 *      opcode :-   The opcode of the L2CAP PDU to be Tx
 *      p_pdu  :-   A pointer to a PDU Rx-ed.
 *
 * Returns
 *      u_int8* :-  A pointer to the next byte after the end of the 
 *                  current PDU. This is needed to handle multiple signalling
 *                  PDUs in a single L2CAP packet.
 *
 * Description
 * This function is used to decode L2CAP PDUs Rx-ed from the 
 * peer. The PDU is decoded into a pdu_info structure and then
 * sent to the L2CAP_CH using the L2CAP_Incoming_PDU
 *
 * The error checking of the incoming PDU is also performed here.
 * The following checks are performed.
 ********************************************************/

u_int8* prh_l2_decode_packet(u_int16 handle,u_int8 opcode,u_int8* p_pdu)
{
    prh_t_l2_pdu pdu_info; 
    prh_t_l2_channel *p_channel=NULL;
    prh_t_l2_acl_link* p_link;

    pMemset(&pdu_info,0,sizeof(pdu_info));
    pdu_info.identifier = BT_READ_LE_UINT8(p_pdu);
    pdu_info.length = BT_READ_LE_UINT16(p_pdu+1);
    
    switch(opcode)
    {
    case L2CAP_COMMAND_REJECT :          
        //pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP Enc_Dec : Got L2CAP COMMAND REJECT - Discarding \n"));  
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP - Receive COMMAND REJECT"));  
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+21),NULL,0,0);
#endif
        break;

    case L2CAP_CONNECTION_REQUEST :
        /********************************************
        *  Code       - 1 Byte
        *  Identifier - 1 Byte
        *  Length     - 2 Bytes
        *  PSM        - 2 Bytes
        *  SCID       - 2 Bytes
        **********************************************/  
        //pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Got L2CAP CONNECT REQUEST \n"));  
 #if pDEBUG
        pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP - Receive CONNECTION_REQUEST"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+22),NULL,0,0);
 #endif
        pdu_info.psm = BT_READ_LE_UINT16(p_pdu+3);
        pdu_info.src_cid = BT_READ_LE_UINT16(p_pdu+5);
        p_pdu+=7;
        break;

    case L2CAP_CONNECTION_RESPONSE : 
        /********************************************
        *  Code       - 1 Byte
        *  Identifier - 1 Byte
        *  Length     - 2 Bytes
        *  DCID       - 2 Bytes
        *  SCID       - 2 Bytes
        *  Result     - 2 Bytes
        *  Status     - 2 Bytes
        **********************************************/
        //pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Got L2CAP CONNECT RESPONSE \n"));
 #if pDEBUG
        pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP - Receive CONNECT RESPONSE"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+23),NULL,0,0);
 #endif
        
        pdu_info.dest_cid = BT_READ_LE_UINT16(p_pdu+3);
        pdu_info.src_cid = BT_READ_LE_UINT16(p_pdu+5);
        pdu_info.result = BT_READ_LE_UINT16(p_pdu + 7);
        pdu_info.status = (t_api) BT_READ_LE_UINT16(p_pdu + 9);
        p_pdu+=11;
        break;

    case L2CAP_CONFIGURE_REQUEST :
        /********************************************
        *  Code       - 1 Byte
        *  Identifier - 1 Byte
        *  Length     - 2 Bytes
        *  DCID       - 2 Bytes
        *  Flags      - 2 Bytes
        *  Options    - 2-24 Bytes
        **********************************************/
        //pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Got L2CAP CONFIG REQUEST \n"));
  #if pDEBUG
        pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP - Receive CONFIG REQUEST"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
          
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+24),NULL,0,0);
  #endif
        pdu_info.dest_cid = BT_READ_LE_UINT16(p_pdu+3);
        pdu_info.config_continue = ( BT_READ_LE_UINT16(p_pdu+5) & PRH_L2_CONFIG_CONTINUE_ON );

        /*
        * An options field is only present if the length
        * field is greater than four 
        */
		pdu_info.offset = 4;
        if (pdu_info.length > 7)
        {   
			
            if (BT_NOERROR != prh_l2_enc_get_options(p_pdu + 7,&pdu_info))
			{
				/* Invalid Options Field */
                /* Send a command Reject and return 0 */
				pdu_info.reason = L2CAP_REJECT_COMMAND_NOT_UNDERSTOOD;
                pdu_info.length = 0x02;
                prh_l2_encode_packet(handle,L2CAP_COMMAND_REJECT, &pdu_info);
				return 0;
			}
        } 
        else
            pdu_info.num_options = 0;

        p_pdu += (pdu_info.offset+3);
        break;

    case L2CAP_CONFIGURE_RESPONSE :
        /********************************************
        *  Code       - 1 Byte
        *  Identifier - 1 Byte
        *  Length     - 2 Bytes
        *  SCID       - 2 Bytes
        *  Flags      - 2 Bytes
        *  Result     - 2 Bytes
        *  Options    - 2-24 Bytes
        **********************************************/
        pdu_info.src_cid = BT_READ_LE_UINT16((p_pdu+3));
        pdu_info.flags = BT_READ_LE_UINT16((p_pdu+5));
        pdu_info.result = BT_READ_LE_UINT16((p_pdu + 7));

        /* An options field is only present if the length
        * field is greater than six 
        */
		pdu_info.offset = 6;
        if (pdu_info.length > 9) 
        {
			/* Need to check for BT_PROTOERROR here */

            if (BT_NOERROR != prh_l2_enc_get_options(p_pdu + 9,&pdu_info))
			{
				/* Invalid Options Field */
                /* Send a command Reject and return 0 */
				pdu_info.reason = L2CAP_REJECT_COMMAND_NOT_UNDERSTOOD;
                pdu_info.length = 0x02;
                prh_l2_encode_packet(handle,L2CAP_COMMAND_REJECT, &pdu_info);
				return 0;
			}
        }
        else
            pdu_info.num_options = 00;

        p_pdu = p_pdu+3+pdu_info.offset;
        break;

    case L2CAP_DISCONNECTION_REQUEST :
    case L2CAP_DISCONNECTION_RESPONSE :
        /********************************************
        *  Code       - 1 Byte
        *  Identifier - 1 Byte
        *  Length     - 2 Bytes
        *  DCID       - 2 Bytes
        *  SCID       - 2 Bytes
        **********************************************/  
        pdu_info.dest_cid = BT_READ_LE_UINT16(p_pdu+3);
        pdu_info.src_cid = BT_READ_LE_UINT16(p_pdu+5);
        p_pdu+=7;
        break;

    case L2CAP_ECHO_RESPONSE :
    case L2CAP_ECHO_REQUEST :
        /********************************************
        *  Code       - 1 Byte
        *  Identifier - 1 Byte
        *  Length     - 2 Bytes
        *  Data       - 
        **********************************************/ 
        pdu_info.length = BT_READ_LE_UINT16(p_pdu+1);
        if (pdu_info.length != 0)
        {   
            pdu_info.p_data = p_pdu+3;
        }
        p_pdu+= (4 + pdu_info.length);
        break;

#if (L2CAP_INFO_SUPPORTED==1)
    case L2CAP_INFORMATION_REQUEST :
        pdu_info.length = BT_READ_LE_UINT16(p_pdu+1);
        pdu_info.info_type = BT_READ_LE_UINT16(p_pdu+3);
        p_pdu+=5;
        break;

    case L2CAP_INFORMATION_RESPONSE :
        pdu_info.length = BT_READ_LE_UINT16(p_pdu+1);
        /******************************************************************************
        * Note ! Currently in the bluetooth spec (Ver 1.1) there is only one 
        * information type defined. This is the Connectionless MTU (info type = 0x01)
        ******************************************************************************/
        pdu_info.info_type = BT_READ_LE_UINT16(p_pdu+3);
        pdu_info.result = BT_READ_LE_UINT16(p_pdu+5);
        if(pdu_info.length > 4)
        {
            pdu_info.p_data = p_pdu + 7;
        }
        p_pdu += pdu_info.length + 3;
        break;
#endif

    default :
        /* Send a command Reject - With Reason as COMMAND_NOT_UNDERSTOOD */
        pdu_info.reason = L2CAP_REJECT_COMMAND_NOT_UNDERSTOOD;
        pdu_info.length = 0x02;
        prh_l2_encode_packet(handle,L2CAP_COMMAND_REJECT, &pdu_info);
        return 0;
        break;
    }

    /*****************************************************************************
     * The following code preforms most the the error checking for incoming 
     * L2CAP PDUs. 
     * The following cases are currently considered.
     *
     *    1/ L2CAP_CONNECTION_RESPONSE with non zero result field.
     *       In this case the SCID and DCID fields can be null. So the the CID cannot
     *       be used to cannot be determined form the PDU. The transaction identifier
     *       needs to be used to derive the p_channel. And then the PDU fields + p_channel
     *       is passed to the L2CAP connection handler (l2_connect_sm.c).
     *
     *    2/ L2CAP_CONNECTION_REQUEST
     *       In this case there has been no local CID allocated for the link yet so the
     *       so the p_channel cannot be derived. Instead, the L2CAP connection handler is
     *       passed a null p_channel.
     *       
     * 
     * For all other opcodes of a value below or equal to  L2CAP_DISCONNECTION_RESPONSE 
     * the p_channel can be obtained directly, from either the SCID or DCID.
     * The fact that all REQUESTs are even opcodes and responses are odd opcodes is used to 
     * derive if the SCID or DCID should be used to obtain the p_channel.
     *
     * In these cases if the p_channel is NULL an L2CAP command reject is sent to the peer.
     * For the DISCONNECT REQ/RESP a check is made on the Cids and if they do not match the
     * expected CIDs the PDU is silently discarded.
     *
     * In addition, for all response PDUs the transaction identifiers are checked and if 
     * they dont match the expected the PDU is silently discarded.
     *
     * Otherwise for L2CAP_ECHO_REQ/RESP and L2CAP_INFORMATION_REQ/RESP the p_channel is not
     * relevant and a pointer to the ACL link container ( p_link ) is used. 
     *
     *************************************************************************************/
    if (opcode == L2CAP_COMMAND_REJECT) 
    {
        //pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"Received L2CAP REJECT pdu - ignoring\n"));
#if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP - Received COMMAND_REJECT"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+21),NULL,0,0);
#endif
        return NULL;
    }

    if ((opcode == L2CAP_CONNECTION_RESPONSE) && ( pdu_info.result != 0))
    {
        p_channel = prh_l2_chn_tid2channel(pdu_info.identifier);
        pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP_CONNECTION_RESPONSE local_cid:%x,src_cid,result",p_channel->local_cid,pdu_info.src_cid,pdu_info.result));

        if (p_channel != 0) 
        {
            if ((pdu_info.result == 0x01) && (p_channel->local_cid != pdu_info.src_cid))
            {
                pdu_info.reason = L2CAP_REJECT_INVALID_CID_IN_REQUEST;
                pdu_info.length = 0x06;
                prh_l2_encode_packet(handle,L2CAP_COMMAND_REJECT, &pdu_info);
            }
            else
                prh_l2_connect_incoming_pdu(handle,opcode,p_channel,&pdu_info);
        }
        else
        {
            /* Response with invalid Identifier is silently discarded ( pg 256 of ver 1.1) */
            //pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Invalid Identifier in L2CAP_CONNECTION_RESPONSE Discarding \n")); 
#if pDEBUG
            pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP - Received L2CAP_CONNECTION_RESPONSE with invalid Indentifier, discarding "));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)         
            SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+25),NULL,0,0);
#endif
        }
        return p_pdu; 
    }
    else if(opcode <= L2CAP_DISCONNECTION_RESPONSE)
    {
        if ( opcode == L2CAP_CONNECTION_REQUEST )
        {
            p_channel = 0;          
            prh_l2_connect_incoming_pdu(handle, opcode, p_channel, &pdu_info);
            return p_pdu;
        }
        if (opcode & 0x01) /* Response */
            p_channel = prh_l2_chn_get_p_channel(pdu_info.src_cid);        
        else
        {/* Request */
            p_channel = prh_l2_chn_get_p_channel(pdu_info.dest_cid);
            if (p_channel)
                p_channel->remote_transaction_identifier = pdu_info.identifier;
            else
                return 0;
        }
        if ((p_channel == 0) && (opcode != L2CAP_CONNECTION_REQUEST) && (opcode != L2CAP_DISCONNECTION_RESPONSE))
        {
            pdu_info.reason = L2CAP_REJECT_INVALID_CID_IN_REQUEST;
            pdu_info.length = 0x06;
            prh_l2_encode_packet(handle,L2CAP_COMMAND_REJECT, &pdu_info);
            return 0;
        }
        if (opcode == L2CAP_DISCONNECTION_REQUEST)
        {
            if (p_channel->remote_cid != pdu_info.src_cid)
                return p_pdu;
        }
        else if(opcode == L2CAP_DISCONNECTION_RESPONSE)
        {
            if (p_channel == 0)
                return p_pdu;
            if (p_channel->remote_cid != pdu_info.dest_cid)
                return p_pdu;
        }
        if ((opcode & 0x01) && (p_channel->local_transaction_identifier != pdu_info.identifier))
            return 0;    
    }
    else if (opcode <= L2CAP_INFORMATION_RESPONSE)
    {
        p_link = prh_l2_acl_find_handle(handle);
        if (opcode & 0x01 )  /* L2CAP_ECHO_RESPONSE or L2CAP_INFORMATION_RESPONSE */
        {
            if (p_link->identifier != pdu_info.identifier)
            {
                return p_pdu;
            }
        }
        else /* L2CAP_ECHO_REQUEST or L2CAP_INFORMATION_REQUEST */
        {
            p_link->identifier = pdu_info.identifier;
        }
    }
    prh_l2_connect_incoming_pdu(handle, opcode, p_channel, &pdu_info);
    return p_pdu;
}


/********************************************************
 * Function : prh_l2_enc_insert_options
 *
 * Parameters : 
 *     p_pdu      :- A pointer to the current location in the PDU.
 *     p_pdu_info :- A pointer to the structure containing the fields
 *                   of the PDU.
 *
 * Returns 
 *     u_int8*    :- A pointer to the next location in the PDU to be written to.
 *
 * Description
 * Encodes the options field of an L2CAP Config Req/Resp PDU.
 * using the fields of the PDU Structure.
 *
 * Takes the option information in the p_pdu_info and builds 
 * the options field of an L2CAP PDU.
 * 
 ********************************************************/

u_int8* prh_l2_enc_insert_options(u_int8* p_pdu,prh_t_l2_pdu* p_pdu_info)
{
    t_L2_ChannelFlowSpec* p_pdu_qos;

    if (p_pdu_info->mtu != 0)
    {
        BT_WRITE_LE_UINT8(p_pdu,PRH_L2_MTU_OPTION);
        BT_WRITE_LE_UINT8((p_pdu+1),02);
        BT_WRITE_LE_UINT16((p_pdu+2),p_pdu_info->mtu);
        p_pdu+=4;
        if (PRH_L2_USE_C_BIT)
        {
            p_pdu_info->mtu = 0;
            p_pdu_info->num_options--;
            return p_pdu;
        }
    }
    if (p_pdu_info->flush != 0)
    {
        BT_WRITE_LE_UINT8(p_pdu,PRH_L2_FLUSH_OPTION);
        BT_WRITE_LE_UINT8((p_pdu+1),02);
        BT_WRITE_LE_UINT16((p_pdu+2),p_pdu_info->flush);
        p_pdu+=4;
        if (PRH_L2_USE_C_BIT)
        {
            p_pdu_info->flush = 0;
            p_pdu_info->num_options--;
            return p_pdu;
        }
    }
    if (p_pdu_info->qos != 0)
    {
        BT_WRITE_LE_UINT8(p_pdu,PRH_L2_QOS_OPTION);
        BT_WRITE_LE_UINT8((p_pdu+1),22);
        BT_WRITE_LE_UINT8((p_pdu+2),00); /* flags is reserved and must be set to zero */

        p_pdu_qos = p_pdu_info->qos;

        BT_WRITE_LE_UINT8((p_pdu+3),p_pdu_qos->serviceType);
        BT_WRITE_LE_UINT32((p_pdu+4),p_pdu_qos->tokenRate);
        BT_WRITE_LE_UINT32((p_pdu+8),p_pdu_qos->bucketSize);
        BT_WRITE_LE_UINT32((p_pdu+12),p_pdu_qos->peakBandwidth);
        BT_WRITE_LE_UINT32((p_pdu+16),p_pdu_qos->latency);
        BT_WRITE_LE_UINT32((p_pdu+20),p_pdu_qos->delayVariation);
        p_pdu+=24;
        if (PRH_L2_USE_C_BIT)
        {
            p_pdu_info->qos = 0;
            p_pdu_info->num_options--;
            return p_pdu;
        }
    }
    return p_pdu;
}
    

/********************************************************
 * Function : prh_l2_enc_get_options
 *
 * Parameters : 
 *      p_pdu      :-   A pointer to the current location in the PDU.
 *      p_pdu_info :-   A pointer to the structure containing the fields
 *                      of the PDU.
 *
 * Returns
 *      t_api           BT_NOERROR
 *                      BT_PROTOERROR
 * Description
 * Decodes the options field of an L2CAP Config Req/Resp PDU.
 * into the fields of the PDU Structure.
 *
 * 
 ********************************************************/
 

t_api prh_l2_enc_get_options(u_int8* p_pdu,prh_t_l2_pdu* p_pdu_info)
{
    u_int8 option_type;
    t_L2_ChannelFlowSpec* p_pdu_qos;
	u_int8 option_len = 0;
#if COMBINED_HOST==1
    static t_L2_ChannelFlowSpec qos;
    p_pdu_qos = &qos;
#endif

    option_len = p_pdu[1];

    while ( (p_pdu_info->offset+2+option_len) <= p_pdu_info->length)
    {
        option_type = BT_READ_LE_UINT8(p_pdu);
        

		/* NOTE the Top Bit of the Option field indiates it is a hint
		 */

        switch(option_type & 0x7F /* Mask off the hint bit */)
        {
        case PRH_L2_MTU_OPTION :
            if(02 != option_len)
                return BT_PROTOERROR;
            else
            {
                p_pdu_info->mtu = BT_READ_LE_UINT16(p_pdu+2);
                p_pdu += 4;
                p_pdu_info->num_options++;
            }
            break;

        case PRH_L2_FLUSH_OPTION :
            if(02 != option_len)
                return BT_PROTOERROR;
            else
            {
                p_pdu_info->flush = BT_READ_LE_UINT16(p_pdu+2);
                p_pdu_info->num_options++;
                p_pdu += 4;
            }
            break;

        case PRH_L2_QOS_OPTION :
            if(22 != option_len)
                return BT_PROTOERROR;
            else
            {
#if COMBINED_HOST==0
                p_pdu_qos  =pMalloc(sizeof(t_L2_ChannelFlowSpec));
                if (p_pdu_qos == 0)
					return BT_NORESOURCES;
#endif
                p_pdu_qos->flags = BT_READ_LE_UINT8(p_pdu+2);
                p_pdu_qos->serviceType =BT_READ_LE_UINT8(p_pdu+3);
                p_pdu_qos->tokenRate = BT_READ_LE_UINT32(p_pdu+4);
                p_pdu_qos->bucketSize = BT_READ_LE_UINT32(p_pdu+8);   
                p_pdu_qos->peakBandwidth = BT_READ_LE_UINT32(p_pdu+12);  
                p_pdu_qos->latency = BT_READ_LE_UINT32(p_pdu+16);
                p_pdu_qos->delayVariation = BT_READ_LE_UINT32(p_pdu+20); 

                p_pdu_info->qos = p_pdu_qos;

                p_pdu_info->num_options++;
                p_pdu += 24;
            }       
            break;
        case PRH_L2_RETRANSMISSION_AND_FLOW_CONTROL_OPTION:
            if(9 != option_len)
                return BT_PROTOERROR;
            else
            {
                ; // todo
                p_pdu_info->num_options++;
                p_pdu += 11;
            }
            break;
        default :
            if (option_type & 0x80) /* Option is a hint; skip the option and continue processing. */
            {
				/* 
				 * Increment the p_pdu by the length of the option field 
				*/
                p_pdu += (2 + p_pdu[1]); 
                return BT_NOERROR;
            }
            else
            {
                return BT_PROTOERROR; /* Unknown Option */
            }
            break;
        }
		if ((p_pdu_info->offset + 2 + option_len) == p_pdu_info->length)
		{
			p_pdu_info->offset += (2+option_len);
			return BT_NOERROR;
		}
		else if ((p_pdu_info->offset + option_len +4) <= p_pdu_info->length)
		{
			p_pdu_info->offset += (2 + option_len);
			option_len = p_pdu[1];
		}
		else
			return BT_PROTOERROR;
    }
    return BT_NOERROR;
}

/**************************************************************************
 *
 * Function :- Derive an identfier for an L2CAP Transaction
 *
 * Parameters :
 *      None
 *
 * Returns
 *      u_int8 :- Transaction Identifier for an L2CAP Signalling Operation.
 *
 * Description 
 * Allocates a Transaction Identifier for an L2CAP Signalling Operation. For test
 * purposes the Transaction identifiers begin at 0x45.
 **************************************************************************/

u_int8 prh_l2_enc_derive_identifier()
{  
    l2_cur_ident += 1;

    if (l2_cur_ident == 0xFF)
       l2_cur_ident = 0;

    return l2_cur_ident;
}

/**************************************************************************
 *
 * Function :- Sets the an identfier for an L2CAP Transaction
 *
 * Parameters :
 *      None
 *
 * Returns
 *      u_int8 :- Transaction Identifier for an L2CAP Signalling Operation.
 *
 * Description 
 * Used to set the Tid of the L2CAP signalling to a given value. This is the 
 * starting Tid value from which all further Tids are determined by incrementing 
 * this value.
 **************************************************************************/

void prh_l2_enc_set_cur_ident(u_int8 Tid)
{
	l2_cur_ident = Tid;
}

#if (PRH_L2CAP_INTERNAL_HOST_TESTER == 1)
/**************************************************************************
 *
 * Function :- l2_enc_2_pdus
 *
 * Parameters :
 *      None
 *
 *
 * Description 
 * Used by the Tester to encode 2 PDUs into a single L2CAP packet.
 **************************************************************************/

t_api prh_l2_enc_2_pdus(u_int16 handle,u_int8 opcode1, u_int8 opcode2, prh_t_l2_pdu* p_pdu1_info,prh_t_l2_pdu* p_pdu2_info )
{
    u_int8* p_pdu;

    t_api status = BT_NOERROR;
    u_int16 buf_len;
    u_int16 payload_len;
    struct host_buf *p_buf;
    int ret;


    if((opcode1 > MAX_L2CAP_OPCODES) || (opcode2 > MAX_L2CAP_OPCODES))
    {
    #if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP Enc_Dec : Error - Encode of PDU with invalid opcode \n"));
    #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+26),NULL,0,0);
    #endif
        return BT_UNKNOWNERROR; 
    }

    payload_len = p_pdu1_info->length + p_pdu2_info->length;
    buf_len = payload_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_SIG_HEADER_LENGTH;

    //ret=prh_hci_GetWriteBuffer(handle, (u_int16)(buf_len+L2CAP_PDU_HEADER_LENGTH), 0, &p_buf);
    if (ret!=BT_NOERROR)
      {
      #if pDEBUG
	pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP Enc_Dec : Error - Memory could not be allocated for Signalling Packet \n"));
  #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+27),NULL,0,0);
      #endif
	return ret;
      }
    //host_buf_reserve_header(p_buf,L2CAP_PDU_HEADER_LENGTH);

    p_pdu = p_buf->data;

    if (p_pdu) 
    {
        if (BT_NOERROR == prh_l2_enc_single_pdu(p_pdu,opcode1,p_pdu1_info))
        {
            /* Advance the PDU */
            p_pdu += (L2CAP_SIG_HEADER_LENGTH + p_pdu1_info->length);
            if (!(BT_NOERROR == prh_l2_enc_single_pdu(p_pdu,opcode2,p_pdu2_info)))
                status = BT_UNKNOWNERROR;
        }
        else
            status = BT_UNKNOWNERROR;
           
        p_buf->len = buf_len;
    }
    if (status == BT_NOERROR)
        status = prh_l2_sar_data_req(handle,L2CAP_SIGNALLING_CID,p_buf);
    else
        return status;

    if (status != BT_NOERROR)
    {
    #if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP Enc_Dec : SAR Returned Error \n"));
      #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+20),NULL,0,0);
    
    #endif

	  prh_hci_FreeWriteBuffer(p_buf);
    }
    
    return status;
}

t_api prh_l2_enc_single_pdu(u_int8* p_pdu,u_int8 opcode,prh_t_l2_pdu* p_pdu_info)
{
    prh_t_l2_channel* p_channel;
    u_int8* end_p_pdu;
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    u_int8 temp[4];
#endif

    p_channel = p_pdu_info->p_channel;

    BT_WRITE_LE_UINT8(p_pdu,opcode);
    BT_WRITE_LE_UINT16((p_pdu+2),p_pdu_info->length);

    switch(opcode)
    {
    case L2CAP_COMMAND_REJECT :
        /******************************************************
         *  Code       - 1 Byte
         *  Identifier - 1 Byte
         *  Length     - 2 Bytes
         *  PSM        - 2 Bytes
         *  Source CID - 2 Bytes
         ******************************************************/
        #if pDEBUG
        pDebugPrintfEX(pLOGDEBUG,pLOGNOTICE,"L2CAP Enc_Dec : Sent L2CAP COMMAND REJECT ,reason: 0x%02X",
                        p_pdu_info->reason);        
        #endif
        BT_WRITE_LE_UINT8((p_pdu+1),p_pdu_info->identifier);         
        BT_WRITE_LE_UINT16((p_pdu+4),p_pdu_info->reason);

        #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
        
         SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+28),(p_pdu+4),2,BT_CODE_WORD);  
        #endif
        /* Note :- The previously written length field (@ p_pdu+2) is overwritten here */
        switch(p_pdu_info->reason)
        {
        case L2CAP_REJECT_SIGNALLING_MTU_EXCEEDED :
            BT_WRITE_LE_UINT16((p_pdu+2), 0x04);
            BT_WRITE_LE_UINT16((p_pdu+6),p_pdu_info->mtu);
            #if pDEBUG
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP," SIGNALLING_MTU_EXCEEDED \n"));
            #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
             SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+3),NULL,0,0);
            #endif
            break;
        case L2CAP_REJECT_INVALID_CID_IN_REQUEST :
            BT_WRITE_LE_UINT16((p_pdu+2), 0x06);
            BT_WRITE_LE_UINT16((p_pdu+6),p_pdu_info->src_cid);
            BT_WRITE_LE_UINT16((p_pdu+8),p_pdu_info->dest_cid);
            #if pDEBUG
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP," INVALID_CID_IN_REQUEST \n"));
            #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
             SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+4),NULL,0,0);
            #endif
            break;

        default :
            BT_WRITE_LE_UINT16((p_pdu+2), 0x02);
            #if pDEBUG
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP," COMMAND REJECT \n"));
            #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
             SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+5),NULL,0,0);
            #endif
            break;
        }
        break;
    case L2CAP_CONNECTION_REQUEST :
        /*********************************************************
         *  Code       - 1 Byte
         *  Identifier - 1 Byte
         *  Length     - 2 Bytes
         *  PSM        - 2 Bytes
         *  Source CID - 2 Bytes
         ******************************************************/
        //pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONNECTION_REQUEST \n"));
        #if pDEBUG
        pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONNECTION_REQUEST,psm: 0x%02X, local cid: 0x%02X",p_channel->psm,p_channel->local_cid));
       #endif
        BT_WRITE_LE_UINT8((p_pdu+1), p_channel->local_transaction_identifier);
        BT_WRITE_LE_UINT16((p_pdu+4),p_channel->psm);
        BT_WRITE_LE_UINT16((p_pdu+6),(p_channel->local_cid));

        #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+6),(p_pdu+4),4,(BT_CODE_WORD|(BT_CODE_WORD<<4)));
        #endif
        break;

    case L2CAP_CONNECTION_RESPONSE :
        /******************************************************  
         *  Code       - 1 Byte
         *  Identifier - 1 Byte
         *  Length     - 2 Bytes
         *  Dest   CID - 2 Bytes
         *  Source CID - 2 Bytes
         *  Result     - 2 Bytes
         *  Status     - 2 Bytes
         ******************************************************/
        //pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONNECTION_RESPONSE \n"));
        #if pDEBUG
        pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONNECTION_RESPONSE, local cid: 0x%02X, remote cid: 0x%02X, result: 0x%02X, status: 0x%02X",
            p_channel->local_cid, p_channel->remote_cid, p_pdu_info->result, p_pdu_info->status));
        #endif
        BT_WRITE_LE_UINT8((p_pdu+1), p_channel->remote_transaction_identifier);
        BT_WRITE_LE_UINT16((p_pdu+4),p_channel->local_cid);
        BT_WRITE_LE_UINT16((p_pdu+6),p_channel->remote_cid);
        BT_WRITE_LE_UINT16((p_pdu+8),p_pdu_info->result);
        BT_WRITE_LE_UINT16((p_pdu+10),p_pdu_info->status);


        #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+7),(p_pdu+4),8,(BT_CODE_WORD |(BT_CODE_WORD<<4)|(BT_CODE_WORD<<8) |(BT_CODE_WORD<<12)));
        #endif
        break;

    case L2CAP_CONFIGURE_REQUEST :
        /*******************************
         *  Code       - 1 Byte
         *  Identifier - 1 Byte
         *  Length     - 2 Bytes
         *  Dest   CID - 2 Bytes
         *  Flags      - 2 Bytes
         *  Optional   - 2 Bytes
         *******************************/
        //pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONFIGURE_REQUEST \n"));
        #if pDEBUG
        pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONFIGURE_REQUEST, remote cid: 0x%02X", p_channel->remote_cid ));
        #endif
        BT_WRITE_LE_UINT8((p_pdu+1), p_channel->local_transaction_identifier);
        BT_WRITE_LE_UINT16((p_pdu+4),p_channel->remote_cid);

        #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+8),(p_pdu+4),2,BT_CODE_WORD);
        #endif
        if ( PRH_L2_USE_C_BIT && (p_pdu_info->num_options > 1))
        {
            BT_WRITE_LE_UINT16((p_pdu+6), PRH_L2_CONFIG_CONTINUE_ON); 
        }
        else
        {
            BT_WRITE_LE_UINT16((p_pdu+6),0 /* Non-Segmented configuration */ ); 
        }
        if(p_pdu_info->num_options)
        {
            end_p_pdu = prh_l2_enc_insert_options(p_pdu+8,p_pdu_info);
            BT_WRITE_LE_UINT16((p_pdu+2),(end_p_pdu - (p_pdu+4)));
        }
        prh_l2_chn_start_RTX_timer(p_channel);
        break;

    case L2CAP_CONFIGURE_RESPONSE :

        /******************************************************
         *  Code       - 1 Byte
         *  Identifier - 1 Byte
         *  Length     - 2 Bytes
         *  Source CID - 2 Bytes
         *  Flags      - 2 Bytes
         *  Results    - 2 Bytes
         *  Optional   - 2 Bytes
         ******************************************************/
        //pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONFIGURE_RESPONSE \n"));
        #if pDEBUG
        pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_CONFIGURE_RESPONSE, remote cid: 0x%02X", p_channel->remote_cid));
#endif
        BT_WRITE_LE_UINT8((p_pdu+1), p_channel->remote_transaction_identifier);
        BT_WRITE_LE_UINT16((p_pdu+4),p_channel->remote_cid);   
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+9),(p_pdu+4),2,BT_CODE_WORD);
#endif
        if (PRH_L2_USE_C_BIT && (p_pdu_info->num_options > 1))
        {
            BT_WRITE_LE_UINT16((p_pdu+6),PRH_L2_CONFIG_CONTINUE_ON /* Segmented configuration */ ); 
        }
        else
        {
            BT_WRITE_LE_UINT16((p_pdu+6),PRH_L2_CONFIG_CONTINUE_OFF /* Non-Segmented configuration */ );
        }
        BT_WRITE_LE_UINT16((p_pdu+8),p_pdu_info->result); 

        if(p_pdu_info->num_options)
        {
            end_p_pdu = prh_l2_enc_insert_options(p_pdu+10,p_pdu_info);
            BT_WRITE_LE_UINT16((p_pdu+2),(end_p_pdu - (p_pdu+4)));
        }
        break;

    case L2CAP_DISCONNECTION_REQUEST :
        /******************************************************
         *  Code       - 1 Byte
         *  Identifier - 1 Byte
         *  Length     - 2 Bytes
         *  Dest CID   - 2 Bytes
         *  Source CID - 2 Bytes
         ******************************************************/
#if pDEBUG
        pDebugPrintfEX(pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_DISCONNECTION_REQUEST, local cid: 0x%02X, remote cid: 0x%02X", p_channel->local_cid, p_channel->remote_cid);

#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        BT_WRITE_LE_UINT16(temp,p_channel->remote_cid);
        BT_WRITE_LE_UINT16((temp+2),p_channel->local_cid); 
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+10),temp,4,(BT_CODE_WORD|(BT_CODE_WORD<<4)));
#endif
        BT_WRITE_LE_UINT8((p_pdu+1), p_channel->local_transaction_identifier);
        BT_WRITE_LE_UINT16((p_pdu+4),p_channel->remote_cid);
        BT_WRITE_LE_UINT16((p_pdu+6),p_channel->local_cid); 

        break;

    case L2CAP_DISCONNECTION_RESPONSE :
        /******************************************************
         *  Code       - 1 Byte
         *  Identifier - 1 Byte
         *  Length     - 2 Bytes
         *  Dest CID   - 2 Bytes
         *  Source CID - 2 Bytes
         ******************************************************/
#if pDEBUG
        pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP Enc_Dec : Sent  - L2CAP_DISCONNECTION_RESPONSE, local cid: 0x%02X, remote cid: 0x%02X",p_channel->local_cid, p_channel->remote_cid ));
            
#endif
        BT_WRITE_LE_UINT8((p_pdu+1), p_channel->remote_transaction_identifier);
        BT_WRITE_LE_UINT16((p_pdu+4),p_channel->local_cid);
        BT_WRITE_LE_UINT16((p_pdu+6),p_channel->remote_cid);  

#if pDEBUG==0
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_ENC_DEC_INDEX+11),(p_pdu+4),4,(BT_CODE_WORD|(BT_CODE_WORD<<4)));
#endif
        break;

    default:
        return BT_UNKNOWNERROR;
        break;
    }
    return BT_NOERROR;
}

#endif  /* End of functionality required specifically for the Lower Host Conformance Tester */
