/**************************************************************************
 * MODULE NAME:    l2_connect_sm.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap connection state machine
 * AUTHOR:         Gary Fleming
 * DATE:           02-06-1999
 *
 * SOURCE CONTROL: $Id: l2_connect_sm.c 2720 2010-10-11 07:15:34Z jiancui $
 *
 * LICENSE
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Plc.
 *     All rights reserved.
 *
 *************************************************************************/

#include "host_config.h"
#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"

#include "l2_types.h"
#include "l2_connect_sm.h"
#include "l2_const.h"
#include "l2_enc_dec.h"
#include "l2_sar.h"
#include "l2_groups.h"
#include "l2cap_primitives.h"
#include "l2_acl_link.h"
#include "l2_channel.h"
#include "l2_psm.h"

#include "papi.h"
#include "l2cap_lower_layer.h"

u_int16 pending_cid; /* Is this used?? -MM */
u_int8 L2_Send_Disconnect_Response = 0x1;
u_int8 L2_Send_Echo_Response = 0x1;

u_int8 tester_config=0x00;

/*********************************
 *       API FUNCTION CALLS 
 *********************************/
void L2_Set_Test_Mode(u_int8 mode)
{
	tester_config=mode;
}

void L2_Dont_Send_Disconnect_Rsp()
{
    L2_Send_Disconnect_Response = 0x00;
}

void L2_Dont_Send_Echo_Response()
{
    L2_Send_Echo_Response = 0x00;
}

#if COMBINED_HOST==1
u_int8 *prh_mgr_printBDAddress(t_bdaddr address) {


#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    static u_int8 outBuffer[24];
#else
    static u_int8 outBuffer[18];
#endif
	int i;

    for (i=5; i>=0; i--) {
        /* upper nybble */
        outBuffer[15-i*3]=(((address.bytes[i])&0xF0)>>4)+48;
        outBuffer[15-i*3]+=outBuffer[15-i*3]>57?7:0;
        /* lower nybble */
        outBuffer[15+1-i*3]=((address.bytes[i])&0x0F)+48;
        outBuffer[15+1-i*3]+=outBuffer[15+1-i*3]>57?7:0;
        /* delimiter */
        outBuffer[15+2-i*3]=':';
    }
    outBuffer[17]='\0';

    return outBuffer;
}
#endif

/********************************************************************
 * FUNCTION :-  L2_Connect
 *
 * Input Parameters
 *  t_bdaddr bdaddr  The Bluetooth address of the device to which 
 *                   the Channel is to be established.   
 *  u_int16 psm      The Protocol/Service Multiplexer of the higher
 *                   layer protocol. 
 *
 * DESCRIPTION :- 
 * Handles an L2 Connect Req from the higher layers. This message
 * is only valid in the CLOSED state. 
 *
 * Given the BD_ADDR determine if a connection exists to this device                             
 * 
 * Invoke the Lower Protocol to establish a connection to the 
 * peer. The interaction to the lower protocol is performed via
 * the Bluetooth Manager to give the L2CAP protocol a higher degree
 * of location independance.
 *
 * The cid_index is used as an identifer to map req's to resp's. 
 * 
 * According to the spec (pg 298) the use of this primitive blocks
 * until the L2CAP connect Cfm is received.
 *         
 *  Valid State Transitions :-  CLOSED -> W4_LP_CONNECT_RSP
 *
 *  Actions :- Allocate a CID
 *             Send LP_Connect_Req to the lower layers
 *
 * NOTE :- The psm parameter has to be the PSM of a previously registered
 * protocol.
 *
 ********************************************************************/
#if PRH_L2CAP_AUTO_CONFIG==1
t_api L2_Connect_Ex(t_bdaddr bdaddr, u_int16 psm, u_int16 *cid_p);

APIDECL1 t_api APIDECL2 L2_Connect(t_bdaddr bdaddr, u_int16 psm)
{
  return L2_Connect_Ex(bdaddr,psm, NULL);
} 

t_api L2_Connect_Ex(t_bdaddr bdaddr, u_int16 psm, u_int16 *cid_p) 
#else
APIDECL1 t_api APIDECL2 L2_Connect(t_bdaddr bdaddr, u_int16 psm)
#endif
{
    #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    u_int8 temp[8];
    u_int8 i=0;    
    #endif

    t_api outcome = BT_PENDING;
    prh_t_l2_channel* p_channel;
    prh_t_l2_acl_link* p_link;
    
    u_int16 handle;
    u_int16 psm_ident;
    prh_t_l2_pdu pdu_info;
#if HOST_SECURITY
    t_api security_status;
#endif

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_Connect called, address %s, psm 0x%02X\n",prh_mgr_printBDAddress(bdaddr),psm));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
     
    BD_ADDR_TO_UINT8_CPY(temp,bdaddr);

    BT_WRITE_LE_UINT16(&(temp[6]),psm);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGSYSTEM, LOG_L2CAP_L2_CONNCECT_SM_INDEX, temp, 8, (BT_CODE_BDADDR | (BT_CODE_WORD)));

#endif

//JianCui add 2009 08 22
#ifndef  MGR_MULTI_CONNECTION 

	struct st_t_BT_ConnectionInfo *currentDevice=prh_device_list_head;	
      /* check that no ACLS are up */
	while(currentDevice) {
	pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_Connect called aclState=%d,addr=%s,cod=0x%x",currentDevice->aclState,prh_mgr_printBDAddress(currentDevice->bdAddress),currentDevice->cod));
	if(currentDevice->aclState==CONN_CLOSED)
		currentDevice = currentDevice->nextDevice;
	else if( (BD_ADDR_CMP(bdaddr,currentDevice->bdAddress)) &&  ((currentDevice->cod&0x500000)!=0x500000)   )
				currentDevice = currentDevice->nextDevice;
	else
		break;
		/*//if((currentDevice->aclState!=CONN_CLOSED)&&(!BD_ADDR_CMP(bdaddr,currentDevice->bdAddress))) //zhou siyou add cmp addr
		if((currentDevice->aclState!=CONN_CLOSED)&&(!BD_ADDR_CMP(bdaddr,currentDevice->bdAddress)))
			break;
		else if(((currentDevice->cod&0x500000)==0x500000)&&(currentDevice->aclState!=CONN_CLOSED)&&(BD_ADDR_CMP(bdaddr,currentDevice->bdAddress)))
			break;
		else
			currentDevice = currentDevice->nextDevice;*/
	}
        if (currentDevice) {
			return BT_NORESOURCES;
        	}

#endif

	/* Firstly Validate that the PSM is valid and is registed */
 
	if (psm & 0x0001) /* Check is the PSM value odd :- BT Spec requirement */
    {
 		psm_ident=prh_l2_psm_get_psm_ident(psm);
 		if (psm_ident==0)
 			return BT_INVALIDPARAM;
 	}
 	else
 		return BT_INVALIDPARAM;
    
    pMemset(&pdu_info,0,sizeof(pdu_info));
    
    p_channel = prh_l2_chn_alloc_channel();
    if(p_channel)
    {
        /* Essential to Reset these paths states in the channel */
        
        p_channel->config_rsp_path = PRH_L2_CONFIG_START;
        p_channel->config_req_path = PRH_L2_CONFIG_START;
        p_channel->psm = psm;
        pending_cid = p_channel->local_cid;
#if PRH_L2CAP_AUTO_CONFIG==1
	if (cid_p)
	  *cid_p=p_channel->local_cid;
#endif
        p_link = prh_l2_acl_exists(&bdaddr);
        if (p_link)
        { 
            handle = p_link->handle;
            p_channel->local_transaction_identifier = prh_l2_enc_derive_identifier();
            p_channel->state = PRH_L2_W4_L2CAP_CONNECT_RSP;
            p_channel->handle = handle;
#if HOST_SECURITY
            security_status = MGR_MultiplexOutgoingAccessRequest(L2CAP_PROTOCOLID, (u_int32)psm,
                bdaddr, (u_int32)p_channel->local_cid, L2_OutgoingAccessRequest_Callback);
            if(BT_PENDING == security_status)
                return BT_PENDING;
            
            if(BT_NOERROR != security_status)
            {
                prh_l2_chn_free_channel(p_channel, PRH_L2_DONT_RELEASE_ACL);
				/* Only release the channel as the ACL was set up by a different
				 * service request
				 */

                return security_status; 
            }
            /* If MGR returned BT_NOERROR, L2CAP can continue immediately with channel setup */
#endif
            pdu_info.p_channel = p_channel;            
            pdu_info.length = 0x04;
            
            outcome = prh_l2_encode_packet(handle,L2CAP_CONNECTION_REQUEST, &pdu_info);
            if(BT_NOERROR != outcome)
            {
                prh_l2_chn_free_channel(p_channel, PRH_L2_DONT_RELEASE_ACL);

                /* Note "outcome" should be returned to let HCI errors 
                 * propegate to the application 
                 */

                return outcome /* BT_NORESOURCES */;
            }
            else
            {
                prh_l2_chn_start_RTX_timer(p_channel); 
            }
        }
        else /* p_link == 0 :- No ACL link exists */
        {
            /*
            * Need to establish an LM link to the Bd_Addr. This is done by interacting 
            * with the Bluetooth Manager. The prh_mgr_createACL function is passed a pointer
            * to a callback function which is invoked when the manager has extablished
            * the LM ACL link.
            * The op_pending state is important as it allows the callback function to 
            * determine the actions to be performed following ACL link establishment
            */
            
            p_link = prh_l2_acl_alloc_link(&bdaddr,pending_cid);
             
            if(p_link)
			{
                p_link->op_pending = L2_CHANNEL_SETUP;
			}
			else
				return BT_NORESOURCES;

            p_channel->state = PRH_L2_W4_LP_CONNECT_CFM;
#if COMBINED_HOST==0
            outcome = prh_mgr_createACL(bdaddr, psm, &p_channel->handle,prh_l2_connect_lp_connect_cfm, (tid_t)p_link);
            if (outcome != BT_PENDING)
			{ 
			    struct st_t_BT_ConnectionInfo *dbEntry = prh_mgr_find_deviceEntryByAddress(bdaddr);
				/* Check to protect against Mgr thinking ACL exists and L2CAP doesnt
				 */
                prh_l2_acl_free_link(p_link);
                if(dbEntry && dbEntry->aclState == CONN_RELEASEPENDING)
                {
                }
                else
                    prh_l2_chn_free_channel(p_channel, PRH_L2_DONT_RELEASE_ACL);
				/* Over-ride error message */
				if (outcome == BT_NOERROR)
					outcome = BT_UNKNOWNERROR;
            }
#else
	{
	    t_pageParams params;
	    params.p_bd_addr =(t_bd_addr*)&bdaddr;
		params.packet_types = 0xcc18;
		params.allow_role_switch = FALSE;
		params.clockOffset = 0;
		params.pageScanMode = 0;
		params.srMode = 1;
	    LMconnection_LM_Connection_Req(&params);
            }
#endif
        }
    }
    else /* p_channel == 0 :- Could not alloc memory for the channel */
    {
        outcome =  BT_NORESOURCES;
    }
    return outcome;
}


/****************************************************
 *
 *  Function :-  L2_ConnectResponse
 *
 *  Input Parameters
 *      t_bdaddr bdaddr The device address of the originating device.
 *      u_int16 cid     The Channel IDentifier
 *      u_int16 result  The outcome ofthe Connection Request
 *                          
 *      u_int16 status  Used to give further information if the result
 *                      indicates PENDING.
 *  Returns
 *      t_api
 *
 *  Description :- 
 *  Handles an  L2 Connect Response message from the upper layer. This message is
 *  used by the upper layer to accept or reject an incoming L2CAP connection and 
 *  is only valid in the W4_L2CA_CONNECT_RSP state. A positive result causes an 
 *  L2CAP Connect Rsp PDU  to the peer layer and the state changing to CONFIG.
 *
 *  A negative results cause an L2CAP Connect Rsp PDU  to be sent to the
 *  peer layer and the state changing to CLOSED. In addition the CID 
 *  is freed.
 *
 *  Optionally an L2CAP Config_Req PDU can also be sent to the Peer.
 *
 *  Valid State Transitions :-  W4_L2CA_CONNECT_RSP -> CONFIG
 *                              W4_L2CA_CONNECT_RSP -> CLOSED
 *
 ************************************************************/

APIDECL1 t_api APIDECL2 L2_ConnectResponse(t_bdaddr bdaddr, u_int16 cid, u_int16 response, u_int16 status)           
{
    t_api enc_status;
    prh_t_l2_channel* p_channel;
    prh_t_l2_pdu pdu_info;
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
u_int8 temp[12];
u_int8 i=0;
#endif
    
#if pDEBUG    
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_ConnectResponse called, address %s, cid 0x%02X, response 0x%02X, status 0x%02X\n",prh_mgr_printBDAddress(bdaddr),cid,response,status));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    //temp=prh_mgr_printBDAddress(bdaddr);
    BD_ADDR_TO_UINT8_CPY(temp,bdaddr);
    BT_WRITE_LE_UINT16((temp+6),cid);
    BT_WRITE_LE_UINT16((temp+8),response);
    BT_WRITE_LE_UINT16((temp+10),status);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONNCECT_SM_INDEX+1), temp, 24,
         ((BT_CODE_BDADDR) | (BT_CODE_WORD<<4) | (BT_CODE_WORD<<8) |( BT_CODE_WORD<<12)));
#endif



    /* Find the p_channel from the CID */
    
    p_channel = prh_l2_chn_get_p_channel(cid);
    if (p_channel == 0)
        return BT_INVALIDPARAM;
    pMemset(&pdu_info,0,sizeof(pdu_info));
    
    /* Check Current State and Check if the Identifiers match */
    
    if(PRH_L2_W4_L2CA_CONNECT_RSP == p_channel->state)
    {
        pdu_info.p_channel = p_channel;    
        pdu_info.result = response;
        pdu_info.status = status;
        pdu_info.length = 0x08;        
        enc_status = prh_l2_encode_packet(p_channel->handle,L2CAP_CONNECTION_RESPONSE, &pdu_info);
        if (enc_status != BT_NOERROR)
            return BT_NORESOURCES;
        
        if(L2CAP_CONNECTION_SUCCESSFUL == response)
            p_channel->state = PRH_L2_CONFIG;
        else if (L2CAP_CONNECTION_PENDING == response)
        {
            /* No state change */
        }
        else 
        {
            prh_l2_chn_free_channel(p_channel, PRH_L2_DONT_RELEASE_ACL);
        }       
    } 
    else 
    {
        return BT_INVALIDPARAM;
    } 
    return BT_NOERROR;
}
     
/*********************************************************************
 *  Function :-  L2_Disconnect
 *
 *  Input Parameters
 *      cid     The local identifier of the channel to be released.
 *
 *  Returns
 *      t_api       BT_NOERROR
 *                  BT_INVALIDPARAM
 *                  BT_NORESOURCES
 *      
 *  Description :- 
 *  Handles an  L2 Disconnect Request message from the higher layers.
 *  Assumed that this message is only valid in the OPEN state.
 *
 *  If in the OPEN state an L2CAP_Disconnect_Req PDU is sent to the peer.
 *  RTX timer is started for the PDU and state becomes W4_L2CAP_DISCON_RSP.
 *
 *********************************************************************/

APIDECL1 t_api APIDECL2 L2_Disconnect(u_int16 cid)
{
    prh_t_l2_channel* p_channel;
    prh_t_l2_pdu pdu_info;
    t_api enc_status;

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    u_int8 temp[2];   
    #endif
    
#if pDEBUG
    pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: L2_Disconnect called, cid 0x%02X\n",cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      BT_WRITE_LE_UINT16(temp,cid);
     SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONNCECT_SM_INDEX+2), temp, 2,BT_CODE_WORD);
#endif

    p_channel = prh_l2_chn_get_p_channel(cid);

    if (p_channel == 0)
        return BT_INVALIDPARAM;
    pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"L2CAP: L2_Disconnect p_channel 0x%x,p_channel->state=%d\n",p_channel,p_channel->state));
	
    pMemset(&pdu_info,0,sizeof(pdu_info));

    if((PRH_L2_OPEN == p_channel->state) || (PRH_L2_CONFIG == p_channel->state))
    {
        p_channel->local_transaction_identifier = prh_l2_enc_derive_identifier();
		
        /* Send an Disconnect Response PDU */
        pdu_info.length = 0x04;
        pdu_info.p_channel = p_channel;   
		prh_l2_chn_start_RTX_timer(p_channel);
		p_channel->state = PRH_L2_W4_L2CAP_DISCON_RSP; 
		
		enc_status = prh_l2_encode_packet(p_channel->handle,L2CAP_DISCONNECTION_REQUEST, &pdu_info);    
		if (enc_status != BT_NOERROR)
		{
			prh_l2_chn_disable_RTX_timer(p_channel);
			return BT_NORESOURCES;
		}
		else
		{
			;
		}
		
    }
	else
	{
		;
	}
    return BT_NOERROR;
}

#if (L2CAP_PING_SUPPORTED==1)
/*********************************************************************
 * FUNCTION :-  L2_Ping
 *
 * t_bdaddr bdaddr      The Bluetooth address of the device to be Pinged
 * u_int8 *echo_data    A pointer to the data to be sent to the target device.
 * u_int16 length       The length of the data to be sent to the target device.
 * callbackFunc         The function to be invoked once the peer responds to the
 *                      the ping. The following are the parameters
 *                          t_bdaddr bdaddr
 *                          u_int16 result
 *                          u_int8 *echoData
 *                          u_int16 length
 * Returns 
 *      t_api
 *
 * DESCRIPTION :- 
     * Handles an Ping from the higher layers. Determines the p_link from the given BD Address.
     * If a valid BD Address is found, an Echo Req PDU is sent to the peer. 
     * Otherwise an ACL connection is established, and a callback function
     * is passed to the manager.
 *
 *******************************************************/

APIDECL1 t_api APIDECL2 L2_Ping(t_bdaddr bdaddr, u_int8 *echo_data, u_int16 length,
                                void (*callbackFunc)(t_bdaddr bdaddr, u_int16 result,
                                u_int8 *echoData, u_int16 length))
{ 

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
u_int8* temp;
#endif

    t_api status = BT_NOERROR;   
    prh_t_l2_acl_link* p_link;
    prh_t_l2_pdu pdu_info;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_Ping called, address %s, dataLen=0x%02X\n",prh_mgr_printBDAddress(bdaddr),length));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    //temp=prh_mgr_printBDAddress(bdaddr);
    BD_ADDR_TO_UINT8_CPY(temp,bdaddr);
    BT_WRITE_LE_UINT16((temp+6),length);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONNCECT_SM_INDEX+3), temp, 8, (BT_CODE_BDADDR | (BT_CODE_WORD<<4)));
#endif
		if (callbackFunc == NULL)
			return BT_INVALIDPARAM;
    p_link = prh_l2_acl_exists(&bdaddr);
    pMemset(&pdu_info,0,sizeof(pdu_info));
    if(p_link) 
    {
        pMemset(&pdu_info,0,sizeof(pdu_info));
        p_link->ping_callback = callbackFunc;
        p_link->op_pending = L2CAP_ECHO_REQUEST;
        p_link->identifier = prh_l2_enc_derive_identifier();

        /* Send an Echo Request PDU        */
        pdu_info.identifier = p_link->identifier;
        pdu_info.length = length;
        pdu_info.p_data = echo_data;
        prh_l2_encode_packet(p_link->handle,L2CAP_ECHO_REQUEST, &pdu_info);   
    } 
    else 
    {
        /* Set up the ACL link             */
        p_link = prh_l2_acl_alloc_link(&bdaddr,pending_cid);
        if (p_link == 0)
            return BT_NORESOURCES;

        p_link->ping_callback = callbackFunc;
        p_link->identifier = prh_l2_enc_derive_identifier();
        p_link->op_pending = L2_ECHO_REQ;
        p_link->echo_len = length;
        p_link->echo_data = echo_data;

#if COMBINED_HOST==0
        status = prh_mgr_createACL(bdaddr,0,&p_link->handle,prh_l2_connect_lp_connect_cfm, (tid_t)p_link);
#endif
    }
    return status;
}

#endif

#if (L2CAP_INFO_SUPPORTED==1)
/********************************************************
 *
 * FUNCTION :-  L2_GetInfo
 *
 * t_bdaddr bdaddr      The Bluetooth address of the target device
 * u_int16  info_type   The Information Type to be retrieved from the target device.
 * callbackFunc         The function to be invoked once the peer responds to the
 *                      the information request. The following are the parameters
 *                          t_bdaddr bdaddr
 *                          u_int16 result
 *                          u_int8 *infoData
 *                          u_int16 infolength
 *
 * Returns 
 *      t_api
 *
 * DESCRIPTION :- 
 * This primitive initiates the retrieval of remote information from a device.
 * Currently the only defined information type in the Bluetooth Ver 1.1 spec
 * is a the connectionless MTU. 
 * If an ACL link does not exist to the remote device one is created.
 *
 *******************************************************/

APIDECL1 t_api APIDECL2 L2_GetInfo(t_bdaddr bdaddr, u_int16 info_type,
                                   void (*callbackFunc)(t_bdaddr bdaddr,u_int16 result,
                                   u_int8* infoData,u_int16 infoLength))
{
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))

    u_int8 temp[8];
    u_int8 i;
#endif

    t_api status = BT_NOERROR;   
    prh_t_l2_acl_link* p_link;
    prh_t_l2_pdu pdu_info;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_GetInfo called, address %s, infoType=0x%02X\n",prh_mgr_printBDAddress(bdaddr),info_type));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    //temp=prh_mgr_printBDAddress(bdaddr);
    BD_ADDR_TO_UINT8_CPY(temp,bdaddr); 
    BT_WRITE_LE_UINT16((temp+6),info_type);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONNCECT_SM_INDEX+4), temp, 8,(BT_CODE_BDADDR | (BT_CODE_WORD<<4)));

#endif


		if (callbackFunc == NULL)
			return BT_INVALIDPARAM;

    p_link = prh_l2_acl_exists(&bdaddr);
    pMemset(&pdu_info,0,sizeof(pdu_info));
    if(p_link) 
    {
        p_link->info_callback = callbackFunc;
        p_link->identifier = prh_l2_enc_derive_identifier();
        p_link->op_pending = L2CAP_INFORMATION_REQUEST;


        /* Send an Get Info Request PDU    */

        pdu_info.identifier = p_link->identifier;
        pdu_info.length = 0x02;
        pdu_info.info_type = info_type;
        prh_l2_encode_packet(p_link->handle,L2CAP_INFORMATION_REQUEST, &pdu_info);   
    } 
    else 
    {
        p_link = prh_l2_acl_alloc_link(&bdaddr,pending_cid);
        if (p_link == 0)
            return BT_NORESOURCES;

        p_link->info_callback = callbackFunc;
        p_link->identifier = prh_l2_enc_derive_identifier();
        p_link->op_pending = L2_INFO_REQ;
        p_link->info_type = info_type;

#if COMBINED_HOST==0
        status = prh_mgr_createACL(bdaddr,0,&p_link->handle,prh_l2_connect_lp_connect_cfm, (tid_t)p_link);
#endif
    }
    return status;
}

#endif

#if HOST_SECURITY

/*******************************************************************************
 * Function :- L2_IncomingAccessRequest_Callback
 *
 * Input Parameters
 *      u_int32 cbHandle    The transaction handle which was used in the the previous
 *                          access request.
 *      t_api status        Outcome of the Access Request
 *
 * Returns
 *      void
 *   
 * Description
 *  This is the callback function that is invoked by the manager whan a security access
 *  request has been processed for an incoming L2CAP connection. If the Access request
 *  succeeded a Connection Indication event is raised to the higher layer.  
 *  If there was a failure a L2CAP CONNECT RESPONSE PDU is sent to the initiator with
 *  the result field indicating an error (L2CAP_CONNECTION_REFUSED_SECURITY_BLOCK). 
 *******************************************************************************/

void L2_IncomingAccessRequest_Callback(u_int32 cbHandle, t_api status)
{
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    u_int8 temp[2];
#endif

    prh_t_l2_channel* p_channel;
    t_bdaddr* p_src_bd_addr;
    t_L2_ControlCallbackArgs  eventinfo;
    prh_t_l2_pdu out_pdu_info;
    u_int8 psm_index;

    p_channel = prh_l2_chn_get_p_channel((u_int16)cbHandle);

    if(p_channel)
    {
        psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
        p_src_bd_addr = prh_l2_acl_get_bd_addr(p_channel->handle);

		pMemset(&eventinfo,0,sizeof(eventinfo));
        p_channel->state = PRH_L2_W4_L2CA_CONNECT_RSP;
        p_channel->config_rsp_path = PRH_L2_CONFIG_START;
        p_channel->config_req_path = PRH_L2_CONFIG_START;
#if pDEBUG
        pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"\n Channel %x -Config_Req Changed to Config Start (IAR_Callback) Loc #4\n", p_channel->local_cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        BT_WRITE_LE_UINT16(temp,p_channel->local_cid);
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONNCECT_SM_INDEX+5), temp, 2,BT_CODE_WORD);

#endif
        /*
        * Build up the event structure and send L2_Connect_Ind 
        * to the higher layers.
        */

        eventinfo.BDAddress=*p_src_bd_addr;
        eventinfo.cid=p_channel->local_cid;
        eventinfo.psm=p_channel->psm; /* Not really required */
      
        /*
        * If the Security Access Request failed, need to send back 
        * negative response and free the cid.
        */

        if(BT_NOERROR != status)
        {
			pMemset(&out_pdu_info,0,sizeof(out_pdu_info));
            out_pdu_info.p_channel = p_channel;
            out_pdu_info.result = L2CAP_CONNECTION_REFUSED_SECURITY_BLOCK;
            out_pdu_info.length = 0x08;
            prh_l2_encode_packet(p_channel->handle, L2CAP_CONNECTION_RESPONSE, &out_pdu_info);
            prh_l2_chn_free_channel(p_channel, PRH_L2_DONT_RELEASE_ACL);
        }
        else
        {
            prh_l2_psm_event(psm_index, L2CAP_evCONNECTIND,&eventinfo);
                  
        }
    }
}

/*******************************************************************************
 * Function :- L2_OutgoingAccessRequest_Callback
 *
 * Input Parameters
 *      u_int32 cbHandle    The transaction handle which was used in the the previous
 *                          access request.
 *      t_api status        Outcome of the Access Request
 *
 * Returns
 *      void
 *   
 * Description
 *  This is the callback function that is invoked by the manager whan a security access 
 *  request has  been processed for an outgoing L2CAP connection. If the Access request
 *  succeeded the L2CAP connection setup is continued by sending an L2CAP_CONNECTION_REQUEST. 
 *  If there was a failure a Connect_Confirm event is raised to the higher layer
 *  indicating an error.
 *******************************************************************************/


void L2_OutgoingAccessRequest_Callback(u_int32 cbHandle, t_api status)
{
    prh_t_l2_channel* p_channel;
    prh_t_l2_pdu pdu_info;
    u_int8 psm_index;
    t_L2_ControlCallbackArgs  eventinfo;

    p_channel = prh_l2_chn_get_p_channel((u_int16)cbHandle);
    if (p_channel == 0)
        return;
    if (status == BT_NOERROR)
    {
        p_channel->state = PRH_L2_W4_L2CAP_CONNECT_RSP;
  
        pdu_info.p_channel = p_channel;            
        pdu_info.length = 0x04;
        prh_l2_encode_packet(p_channel->handle,L2CAP_CONNECTION_REQUEST, &pdu_info);
        prh_l2_chn_start_RTX_timer(p_channel); 
    }
    else
    {
		pMemset(&eventinfo,0,sizeof(eventinfo));
        eventinfo.cid = p_channel->local_cid;
        eventinfo.result = L2CAP_CONNECTION_REFUSED_SECURITY_BLOCK;
        eventinfo.status = 0x00;

        /* Are the bdaddr & psm parameters seem unnecessary */

        eventinfo.psm = p_channel->psm;

        eventinfo.BDAddress = *prh_l2_acl_get_bd_addr(p_channel->handle);

        psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
        prh_l2_psm_event(psm_index, L2CAP_evCONNECTCONF,&eventinfo);
        //prh_l2_chn_free_channel(p_channel, PRH_L2_DONT_RELEASE_ACL);
	    prh_l2_chn_free_channel(p_channel, PRH_L2_RELEASE_ACL);
    }
}

#endif /* HOST_SECURITY */

/***************************
 * STACK INTERNAL FUNCTIONS
 ***************************/
/********************************************************************************
 * Function :- prh_l2_connect_incoming_pdu
 *
 * Inout parameters
 *      u_int16 handle          The connection handle of the ACL link the 
 *                              signalling PDU was recieved on.
 *      u_int8 opcode           The opcode of the incoming signalling PDU.
 *      t_l2_channel* p_channel A pointer to the L2CAP channel which the PDU
 *                              is associated with.
 *      prh_t_l2_pdu *p_in_pdu  A pointer to the first byte of the incoming PDU.
 *
 * Description
 *  This function handles ALL incoming signalling PDUs. Depending on the PDU 
 *  recieved a state transition may occur on the Channel/Link and/or a response
 *  PDU sent to the peer. In addition, where necessary events to the higher
 *  layer are raised.
 *  Much of the error checking on the PDU is performed by the decode routines 
 *  prior to the invokation of this function.
 *
 * NOTE :- The input p_channel parameter is NOT used in the decoding of the 
 * L2CAP_CONNECT_REQUEST PDU. Instead it is over-ridden by a newly allocated
 * p_channel. This occurs as a channel is only established on receipt of its
 * CONNECT_REQUEST message.
 *******************************************************************************/

t_api prh_l2_connect_incoming_pdu(u_int16 handle, u_int8 opcode,prh_t_l2_channel* p_channel, prh_t_l2_pdu *p_in_pdu)
{

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
u_int8 temp[2];
#endif

    t_api status = BT_NOERROR;
    u_int8 psm_index=0;
    u_int8 identifier;
    u_int8 result;
    prh_t_l2_pdu out_pdu_info;
#if (HOST_LOWER_TESTER == 1)
    prh_t_l2_pdu pdu1_info, pdu2_info;
#endif

    u_int16 length;
    prh_t_l2_acl_link* p_link;
    t_bdaddr* p_bd_addr;
    t_L2_ControlCallbackArgs  eventinfo;
    t_L2_ChannelFlowSpec* p_chn_qos=NULL;
    t_L2_ChannelFlowSpec* p_pdu_qos=NULL;

        
    /* Get the information fields from the PDU 
    */
    
    pMemset(&out_pdu_info,0,sizeof(out_pdu_info));
    pMemset(&eventinfo,0,sizeof(eventinfo));
    identifier = p_in_pdu->identifier;
    p_bd_addr = prh_l2_acl_get_bd_addr(handle);
    
    if (opcode!=L2CAP_CONNECTION_REQUEST && opcode < L2CAP_ECHO_REQUEST) 
        psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
    
    switch(opcode)
    {
    case L2CAP_CONNECTION_REQUEST :

        /* For this PDU the p_channel value passed into the function is overriden.
         * This happens as this PDU requests the establishment of a channel. In this 
         * case the p_channel passed in was NULL (0)).
         */
        p_channel = prh_l2_chn_alloc_channel();
        out_pdu_info.status = 0;
        if(p_channel) 
        {
            /* Build up the local state information */
            p_channel->remote_cid = p_in_pdu->src_cid;
            p_channel->remote_transaction_identifier = p_in_pdu->identifier;
            p_channel->psm = p_in_pdu->psm;
            p_channel->handle = handle;
            
            psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
            if (psm_index == 0)    /* PSM of unregistered protocol */
            {
                out_pdu_info.p_channel = p_channel;
                out_pdu_info.result = L2CAP_CONNECTION_REFUSED_PSM_NOT_SUPPORTED;
                out_pdu_info.length = 0x08;
                prh_l2_encode_packet(handle,L2CAP_CONNECTION_RESPONSE, &out_pdu_info);
                prh_l2_chn_free_channel(p_channel,PRH_L2_DONT_RELEASE_ACL);
                return 0;
            }
            else
            {
#if HOST_SECURITY               
                t_api security_status;
#if pDEBUG
               // pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"\n Calling MGR_IncomingAccessRequest\n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONNCECT_SM_INDEX+6), NULL,0,0);
#endif
                /*
                * If Mgr returns with immediate access granted (status = 0),
                * L2CAP continues immediately with channel setup 
                */
                security_status = MGR_IncomingAccessRequest(L2CAP_PROTOCOLID, p_channel->psm, *p_bd_addr, (u_int32)p_channel->local_cid, L2_IncomingAccessRequest_Callback);
                if(BT_PENDING == security_status)
                {
                    /* Dependent on the length of time taken to perform security access 
                    * It may be required to send an L2CAP_CONNECT_RESPONSE with L2CAP_CONNECTION_PENDING
                    * to the peer. This will allow the peer to restart its timers.
                    */

                    out_pdu_info.p_channel = p_channel;
                    out_pdu_info.result = L2CAP_CONNECTION_PENDING;
                    out_pdu_info.length = 0x08;
                    prh_l2_encode_packet(handle, L2CAP_CONNECTION_RESPONSE, &out_pdu_info);
                    return BT_NOERROR;
                }
                else if( BT_NOERROR != security_status)
                {
                    /* Send an L2CAP_CONNECTION_RESPONSE PDU */
                    out_pdu_info.p_channel = p_channel;
                    out_pdu_info.result = L2CAP_CONNECTION_REFUSED_SECURITY_BLOCK;
                    out_pdu_info.length = 0x08;
                    prh_l2_encode_packet(handle, L2CAP_CONNECTION_RESPONSE, &out_pdu_info);
                    prh_l2_chn_free_channel(p_channel, PRH_L2_DONT_RELEASE_ACL);
                    return BT_NOERROR;
                }       
#endif 
                
                /* Update the L2CAP state information */
                
                p_channel->state = PRH_L2_W4_L2CA_CONNECT_RSP;
                p_channel->config_rsp_path = PRH_L2_CONFIG_START;
                p_channel->config_req_path = PRH_L2_CONFIG_START;
#if pDEBUG                
                pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"\n Channel %x -Config_Req Changed to Config Start Loc #1\n", p_channel->local_cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                BT_WRITE_LE_UINT16(temp,p_channel->local_cid);
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONNCECT_SM_INDEX+7), temp,2,BT_CODE_WORD);
#endif                
                /* Build up the event structure and send L2CA_Connect_Ind to the higher layers. */
                eventinfo.BDAddress=*p_bd_addr;
                eventinfo.cid=p_channel->local_cid;
                eventinfo.psm=p_channel->psm;
                eventinfo.identifier = p_channel->remote_transaction_identifier;
                
                prh_l2_psm_event(psm_index, L2CAP_evCONNECTIND,&eventinfo);
            }
        } 
        else /* p_channel == 0 */
        {
			/* Is this worth while coding as when the malloc for the channel fails 
			 * we are not going to be able to allocate memory for the PDU either
			 */

            /* Send an L2CAP_CONNECTION_RESPONSE PDU */
            out_pdu_info.identifier = p_in_pdu->identifier;
            out_pdu_info.result = L2CAP_CONNECTION_REFUSED_NO_RESOURCES_AVAIL;
            out_pdu_info.length = 0x08;
            prh_l2_encode_packet(handle,L2CAP_CONNECTION_RESPONSE, &out_pdu_info);
            
        }
        break;
    case L2CAP_CONNECTION_RESPONSE :
        /* p_channel used is the parameter passed into the function */
        if(PRH_L2_W4_L2CAP_CONNECT_RSP == p_channel->state) 
        {
            p_channel->remote_cid = p_in_pdu->dest_cid;
            
            /*
            * Build up the event structure for an L2CA_Connect_Cfm 
            * to the higher layers.
            */

			eventinfo.cid = p_channel->local_cid;
            eventinfo.result = p_in_pdu->result;
            eventinfo.status = p_in_pdu->status;
            /* PSM & BDAddress Not required */
            eventinfo.psm = p_channel->psm;
            eventinfo.BDAddress = *p_bd_addr;
            
            switch(p_in_pdu->result) 
            {
            case L2CAP_CONNECTION_SUCCESSFUL : 
                prh_l2_chn_disable_RTX_timer(p_channel);
                p_channel->state = PRH_L2_CONFIG;
                break;
                
            case L2CAP_CONNECTION_PENDING :
                prh_l2_chn_disable_RTX_timer(p_channel);
                prh_l2_chn_start_ERTX_timer(p_channel);
                break;
                
            case L2CAP_CONNECTION_REFUSED_PSM_NOT_SUPPORTED : 
            case L2CAP_CONNECTION_REFUSED_SECURITY_BLOCK :
            case L2CAP_CONNECTION_REFUSED_NO_RESOURCES_AVAIL :
			default :
                prh_l2_chn_disable_RTX_timer(p_channel);
                prh_l2_chn_free_channel(p_channel, PRH_L2_RELEASE_ACL);
                break;
            }
            prh_l2_psm_event(psm_index,L2CAP_evCONNECTCONF,&eventinfo);
			
        }
        else
            return BT_UNKNOWNERROR;
        
        break;
    case L2CAP_CONFIGURE_REQUEST :
        /*
        * Build up the event structure for an L2CA_Connect_Ind
        * to the higher layers.
        */
#if (HOST_LOWER_TESTER==1)  /* LOWER CONFORMANCE TESTER FUNCTIONALITY ONLY !! */		
		if(tester_config)
		{
			if (tester_config == 9)
			{
				
				pdu1_info.mtu = 0;
				pdu1_info.flush = 0;
				pdu1_info.qos = 0;
				pdu1_info.flags = 0; /* No Negotiation Required */
				pdu1_info.length = 0x06;
				pdu1_info.p_channel = p_channel;   
				pdu1_info.result = 0;
				pdu1_info.num_options = 0;
				
				pdu2_info.mtu = PRH_L2_DEFAULT_MTU;
				pdu2_info.flush = 0;
				pdu2_info.qos = 0;
				pdu2_info.flags = 0; /* No Negotiation Required */
				pdu2_info.length = 0x04;
				pdu2_info.p_channel = p_channel;  
				pdu2_info.num_options = 0;
				
				
				/* Construct 2 Signalling PDUs in a single L2CAP packet */
				prh_l2_enc_2_pdus(handle,L2CAP_CONFIGURE_RESPONSE,L2CAP_CONFIGURE_REQUEST,
					&pdu1_info,&pdu2_info );
			}
			else
			{
				eventinfo.cid=p_in_pdu->dest_cid;
				eventinfo.psm=p_channel->psm;
				
				if (p_in_pdu->num_options) 
				{
					eventinfo.mtu = p_in_pdu->mtu;
					eventinfo.flushTimeout = p_in_pdu->flush;
					eventinfo.flow = p_in_pdu->qos;
				} 
				else 
				{
					eventinfo.mtu  = PRH_L2_DEFAULT_MTU;
					eventinfo.flushTimeout = PRH_L2_DEFAULT_FLUSH_TO;
					eventinfo.flow = 0;
				}
				/* The actions to be performed are dependent on the current
				* state of the given channel
				*/
				if (p_channel->state == PRH_L2_CLOSED)
				{
					out_pdu_info.p_channel = p_channel;
					out_pdu_info.result = L2CAP_CONFIG_FAILURE_REJECTED;
					out_pdu_info.status = 0;
					
					prh_l2_encode_packet(handle,L2CAP_CONFIGURE_RESPONSE, &out_pdu_info);
				}     
				else if ((p_channel->state == PRH_L2_CONFIG) || (p_channel->state == PRH_L2_OPEN))
				{
					if(p_in_pdu->config_continue == 0)
						p_channel->config_rsp_path = PRH_L2_REQUEST_RECEIVED;
					
					if (p_channel->state == PRH_L2_OPEN)
					{
						p_channel->state = PRH_L2_CONFIG;
						prh_l2_chn_start_cfg_timer(p_channel);
						p_channel->config_req_path = PRH_L2_CONFIG_START;
#if pDEBUG
						pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"\n Channel %x -Config_Req Changed to Config Start Loc #2\n", p_channel->local_cid));

#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                                          BT_WRITE_LE_UINT16(temp,p_channel->local_cid);
                                          SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONNCECT_SM_INDEX+8), temp,2,BT_CODE_WORD);
#endif
                                   }
					prh_l2_chn_disable_RTX_timer(p_channel);
					prh_l2_psm_event(psm_index,L2CAP_evCONFIGUREIND,&eventinfo);
				} 
				else
					status = BT_UNKNOWNERROR;
			}
		}
		else /*if(!tester_config)*/
#endif
     /* End of lower conformance tester functionality */
		{
			
			eventinfo.cid=p_in_pdu->dest_cid;
			eventinfo.psm=p_channel->psm;
			
			if (p_in_pdu->num_options) 
			{
				if (p_in_pdu->mtu)
				{
					p_channel->outMTU = p_in_pdu->mtu;
				}
				if (p_in_pdu->flush)
				{
					p_channel->inFlush = p_in_pdu->flush;
				}
				if (p_in_pdu->qos)
				{
#if COMBINED_HOST==0
					p_channel->inQOS = p_in_pdu->qos;
#else
					p_channel->inQOS = *(p_in_pdu->qos);
#endif
				}
			}
			eventinfo.mtu  = p_channel->outMTU;
			eventinfo.flushTimeout = p_channel->inFlush;
#if COMBINED_HOST==0
			eventinfo.flow = p_channel->inQOS;
#else
			eventinfo.flow = &(p_channel->inQOS);
#endif

			/* The actions to be performed are dependent on the current
			* state of the given channel
			*/
			if (p_channel->state == PRH_L2_CLOSED)
			{
				out_pdu_info.p_channel = p_channel;
				out_pdu_info.result = L2CAP_CONFIG_FAILURE_REJECTED;
				out_pdu_info.status = 0;
				out_pdu_info.length = 6;
				
				prh_l2_encode_packet(handle,L2CAP_CONFIGURE_RESPONSE, &out_pdu_info);
			}     
			else if ((p_channel->state == PRH_L2_CONFIG) || (p_channel->state == PRH_L2_OPEN))
			{
				if(p_in_pdu->config_continue == 0)
					p_channel->config_rsp_path = PRH_L2_REQUEST_RECEIVED;
				
				prh_l2_chn_start_cfg_timer(p_channel);
				if (p_channel->state == PRH_L2_OPEN)
				{
					//prh_l2_chn_start_cfg_timer(p_channel);
					p_channel->state = PRH_L2_CONFIG;
				}
				prh_l2_chn_disable_RTX_timer(p_channel);
				prh_l2_psm_event(psm_index,L2CAP_evCONFIGUREIND,&eventinfo);
			} 
			else
				status = BT_UNKNOWNERROR;

			/* If memory was allocated for the incoming QoS in the PDU, ensure it is freed */
#if COMBINED_HOST==0
			if ((p_in_pdu->num_options) && (p_in_pdu->qos))
			{
				p_channel->inQOS=0;
				pFree(p_in_pdu->qos);
			}		
#endif
		}
        break;
        
    case L2CAP_CONFIGURE_RESPONSE :
        if (PRH_L2_CONFIG == p_channel->state) 
        {
            eventinfo.cid = p_in_pdu->src_cid;  
            eventinfo.psm = p_channel->psm;  
            eventinfo.result = p_in_pdu->result;
            
            switch(p_in_pdu->result) 
            {
            case L2CAP_CONFIG_SUCCESSFUL :
                p_channel->config_req_path = PRH_L2_CONFIG_END;
#if pDEBUG
                //pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"\n Channel %x - Config_Req Changed to Config End \n", p_channel->local_cid));

#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                BT_WRITE_LE_UINT16(temp,p_channel->local_cid);
                SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONNCECT_SM_INDEX+9), temp,2,BT_CODE_WORD);
#endif
                /* Set up the values in the p_channel structure */
                
                if (PRH_L2_CONFIG_END == p_channel->config_rsp_path)
				{
					prh_l2_chn_stop_cfg_timer(p_channel);
                    p_channel->state = PRH_L2_OPEN; 
				}
                else
                    p_channel->state = PRH_L2_CONFIG;
                
                /* If the configuration was successfull then then new values should be
                * stored in the channel and event raised to the higher layer with the event
                * containing the agreed values.
                */
                
#if COMBINED_HOST==0
                if ((p_in_pdu->qos) && (p_channel->outQOS == 0))
                {
                    prh_l2_chn_set_qos(p_channel,p_in_pdu->qos);
                    eventinfo.flow = p_channel->outQOS;                  
                }
                /* End New Code */
                else if ((p_in_pdu->qos) && (p_channel->outQOS))
                {
                    p_chn_qos = p_channel->outQOS;
#else
                if ((p_in_pdu->qos) /*&& (p_channel->outQOS == 0)*/)
                {
                    prh_l2_chn_set_qos(p_channel,p_in_pdu->qos);
                    eventinfo.flow = &(p_channel->outQOS);                  
                }
                /* End New Code */
                else if ((p_in_pdu->qos)/* && (p_channel->outQOS)*/)
                {
                    p_chn_qos = &(p_channel->outQOS);
#endif
                    p_pdu_qos = p_in_pdu->qos;

                    if (((p_chn_qos->tokenRate == PRH_L2_TOKEN_RATE_WILD_CARD ) ||
                        (p_chn_qos->tokenRate ==  PRH_L2_TOKEN_RATE_DONT_CARE)) &&
                        (p_pdu_qos->tokenRate !=  PRH_L2_TOKEN_RATE_DONT_CARE ))
                    {
                        p_chn_qos->tokenRate = p_pdu_qos->tokenRate;
                    }
                    if (((p_chn_qos->bucketSize == PRH_L2_TOKEN_BUCKET_WILD_CARD ) ||
                        (p_chn_qos->bucketSize ==  PRH_L2_TOKEN_BUCKET_DONT_CARE )) &&
                        (p_pdu_qos->bucketSize !=  PRH_L2_TOKEN_BUCKET_DONT_CARE ))
                    {
                        p_chn_qos->bucketSize = p_pdu_qos->bucketSize;
                    }
                    
                    if ((p_chn_qos->latency ==  PRH_L2_LATENCY_DONT_CARE ) &&
                        (p_pdu_qos->latency != PRH_L2_LATENCY_DONT_CARE ))
                    {
                        p_chn_qos->latency = p_pdu_qos->latency;
                    }
                    if ((p_chn_qos->peakBandwidth == PRH_L2_PEAK_BANDWDTH_UNKNOWN) &&
                        (p_pdu_qos->peakBandwidth != PRH_L2_PEAK_BANDWDTH_UNKNOWN ))
                    {
                        p_chn_qos->peakBandwidth = p_pdu_qos->peakBandwidth;
                    }
                    p_chn_qos->delayVariation = p_pdu_qos->delayVariation;
                    eventinfo.flow = p_chn_qos ;
                }
                else
                    eventinfo.flow = 0;
                
                if ((p_in_pdu->flush) /*&& (p_in_pdu->flush < p_channel->outFlush)*/)
                {
                    p_channel->outFlush = p_in_pdu->flush;
                }
                eventinfo.flushTimeout = p_channel->outFlush;

                if ((p_in_pdu->mtu) /*&& (p_in_pdu->mtu < p_channel->inMTU)*/)
                {
                    p_channel->inMTU = p_in_pdu->mtu;
                }
                eventinfo.mtu = p_channel->inMTU;

                break;
                
                
            case L2CAP_CONFIG_FAILURE_UNACCEPTABLE_PARAMS :
                /* In this case the acceptable parameters are contained in the 
                * incoming PDU and are reported to the higher layers -- but not
                * assigned to the channel.
                */
				p_channel->config_req_path = PRH_L2_CONFIG_START;
                
                if(p_in_pdu->mtu)
                    eventinfo.mtu = p_in_pdu->mtu;
                else
                    eventinfo.mtu = p_channel->inMTU;
               
                if(p_in_pdu->flush)
                    eventinfo.flushTimeout = p_in_pdu->flush;
                else
                    eventinfo.flushTimeout = p_channel->outFlush;
                
                if(p_in_pdu->qos)
                    eventinfo.flow = p_in_pdu->qos;
                else
#if COMBINED_HOST==0
                    eventinfo.flow = p_channel->outQOS;
#else
                    eventinfo.flow = &(p_channel->outQOS);
#endif
                break;
                
            case L2CAP_CONFIG_FAILURE_REJECTED :
            case L2CAP_CONFIG_FAILURE_UNKNOWN_OPTIONS :
                /* No additional actions required here */
                break;
            default : 
                status = BT_UNKNOWNERROR;
                break;
            }
            if (status != BT_UNKNOWNERROR)
            {
                prh_l2_chn_disable_RTX_timer(p_channel);
                prh_l2_psm_event(psm_index,L2CAP_evCONFIGURECONF,&eventinfo);
            }
        } 
#if COMBINED_HOST==0
		if ((p_in_pdu->qos)&&(p_channel->outQOS))
			pFree(p_in_pdu->qos);
#endif
        break;
    case L2CAP_DISCONNECTION_REQUEST :
#if (PRH_L2CAP_INTERNAL_HOST_TESTER==1) /* Conformance Tester Functionality only */
        if (L2_Send_Disconnect_Response) /* Test Condiction only */
        {
            out_pdu_info.p_channel = p_channel;   
            out_pdu_info.length = L2CAP_DISCONNECTION_RSP_PARAM_LENGTH;
            out_pdu_info.identifier = p_in_pdu->identifier;
            prh_l2_encode_packet(handle,L2CAP_DISCONNECTION_RESPONSE, &out_pdu_info);
        }
        else
            L2_Send_Disconnect_Response = 0x01;
#else /* End of Conformance Tester Functionality  */

		out_pdu_info.p_channel = p_channel;   
		out_pdu_info.length = L2CAP_DISCONNECTION_RSP_PARAM_LENGTH;
		out_pdu_info.identifier = p_in_pdu->identifier;
		prh_l2_encode_packet(handle,L2CAP_DISCONNECTION_RESPONSE, &out_pdu_info);
#endif
        eventinfo.cid=p_in_pdu->dest_cid;
        eventinfo.result = 0x00;
        eventinfo.psm=p_channel->psm;
        /* Note it is important that the Channel Free is done after the event */
        prh_l2_psm_event(psm_index,L2CAP_evDISCONNECTIND,&eventinfo);
        prh_l2_chn_free_channel(p_channel,PRH_L2_DONT_RELEASE_ACL);
        break;
        
    case L2CAP_DISCONNECTION_RESPONSE :
        if ( PRH_L2_W4_L2CAP_DISCON_RSP == p_channel->state) 
        { 
            eventinfo.cid=p_in_pdu->src_cid;
            eventinfo.psm=p_channel->psm;
            eventinfo.result = 0x00;
            prh_l2_chn_disable_RTX_timer(p_channel);   
            /* Note it is important that the Channel Free is done after the event */
			/* in the case of a disconnection response.. the app requested the disconnect so it should stop data transfer */
            prh_l2_chn_free_channel(p_channel, PRH_L2_RELEASE_ACL);
            prh_l2_psm_event(psm_index,L2CAP_evDISCONNECTCONF,&eventinfo);

         
        } 
        break;
        
    case L2CAP_ECHO_REQUEST :
        length = p_in_pdu->length; 
        out_pdu_info.identifier =  p_in_pdu->identifier; 
        
        if (length <= PRH_L2_MAX_ECHO_DATA_SIZE) 
        {
#if (PRH_L2CAP_INTERNAL_HOST_TESTER==1)
            if (L2_Send_Echo_Response == 0x01) /* Test Condiction only */
            {
#endif
                /* Send an Echo Response PDU */            
            
                out_pdu_info.p_data = p_in_pdu->p_data;
                out_pdu_info.length = length;
                prh_l2_encode_packet(handle,L2CAP_ECHO_RESPONSE,&out_pdu_info);
#if (PRH_L2CAP_INTERNAL_HOST_TESTER==1)
            }
            else
                L2_Send_Echo_Response = 0x01;
#endif
        } 
        break;
        
    case L2CAP_ECHO_RESPONSE :
    case L2CAP_INFORMATION_RESPONSE :
        result = L2CAP_SUCCESS;  /* Successfull Result */ 
        p_link = prh_l2_acl_find_handle(handle);
        p_bd_addr = prh_l2_acl_get_bd_addr(handle);
        if (opcode == L2CAP_ECHO_RESPONSE)
        {
            if(p_link != 0)
            {
                prh_l2_acl_disable_RTX_timer(p_link);
				p_link->op_pending = 0;
                p_link->ping_callback(*p_bd_addr,result,p_in_pdu->p_data, p_in_pdu->length);
            }
        }
        else /* opcode == L2CAP_INFORMATION_RESPONSE */ 
        {
            result = p_in_pdu->result;
            if (p_in_pdu->info_type == PRH_L2_CONNECTIONLESS_MTU_INFOTYPE)
            {   
                if(p_link != 0)
                {
                    prh_l2_acl_disable_RTX_timer(p_link);
					p_link->op_pending = 0;
                    p_link->info_callback(*p_bd_addr, result, p_in_pdu->p_data,0x02 /* MTU ONLY*/);
                }
            }
            else
            {
				if(p_link != 0)
				{
					/* Other Values of info_type currently unsupported by Bluetooth Specification */
					prh_l2_acl_disable_RTX_timer(p_link);
					p_link->info_callback(*p_bd_addr, result, p_in_pdu->p_data,L2CAP_INFO_NOT_SUPPORTED );
				}
            }
        }
        /* Release the ACL link if no Channels on Link and we initiated the ACL link*/
        

		if(!prh_l2_chn_handle_in_use(p_link->handle) && p_link->initiator)
        {
#if COMBINED_HOST==1
	     LMconnection_LM_Disconnect_Req(p_link->handle,PRH_USER_ENDED_CONNECTION);
#else
            prh_mgr_releaseACL(p_link->handle,PRH_USER_ENDED_CONNECTION, NULL);
#endif
            prh_l2_acl_free_link(p_link);
        }
        break;
        
    case L2CAP_INFORMATION_REQUEST :
        out_pdu_info.identifier =  p_in_pdu->identifier; 
        out_pdu_info.info_type = p_in_pdu->info_type;
        if (p_in_pdu->info_type == PRH_L2_CONNECTIONLESS_MTU_INFOTYPE)
        {
            out_pdu_info.length = 0x06;
            out_pdu_info.mtu = PRH_L2_MAX_CLS_MTU; /* Max Connectionless MTU */
            out_pdu_info.result = L2CAP_INFO_SUCCESS;
        }
        else
        {
            out_pdu_info.length = 0x04;
            out_pdu_info.result = L2CAP_INFO_NOT_SUPPORTED;
        }
        prh_l2_encode_packet(handle,L2CAP_INFORMATION_RESPONSE,&out_pdu_info);
        break;
        
    default :
        break;
    }
    return BT_NOERROR;
}

/*
 * There are two different stratagies for decoding HCI events. In the first 
 * approach the HCI decodes all events into a union. However this is very
 * wastefull of ROM. (code size). As many events will not be ignored by
 * most stack configurations.
 * 
 * An alternative approach is to let the HCI deliver events in "raw" format
 * Each layer can then decode only the events it is interested in.
 */

/**************************************************************************
 *
 * FUNCTION :-  prh_l2_connect_event_handler
 *
 * Input Parameters
 *      u_int8 *pdu     A pointer to the HCI PDU
 *
 * Returns 
 *      t_api
 *
 * DESCRIPTION :- 
 * Handles an Event from the HCI. Any events of interest to L2CAP are decoded from
 * the raw PDU format in this function.
 *
 *******************************************************/

t_api prh_l2_connect_event_handler(u_int8 *pdu)
{
#if COMBINED_HOST==0
    u_int16 handle;
    prh_t_l2_acl_link* p_link;
    u_int8 eventType;
    t_bdaddr bdaddr;
    u_int8 result;
    eventType = pdu[0];
  
	/*
	 * pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP -- Got event of type %d\n", eventType));
     */
    switch (eventType) 
    { 
    case HCI_evCONNECTIONCOMPLETE:
		if ((pdu[11]==0x01) && (!pdu[2]))
		{
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP -- acting on HCI_evCONNECTIONCOMPLETE event from HCI\n"));
			pMemcpy(&bdaddr.bytes, pdu+5, 6);
			handle = BT_READ_LE_UINT16(pdu+3);
			prh_l2_connect_lp_connect_ind(&bdaddr, handle);
        }
        break;
        
    case HCI_evCONNECTIONREQUEST:

        break;
        
    case HCI_evDISCONNECTIONCOMPLETE:
		result = pdu[2];
        if (!result) 
        {
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP -- acting on HCI_evDISCONNECTIONCOMPLETE event from HCI\n"));
            handle = BT_READ_LE_UINT16(pdu+3);
            prh_l2_chn_free_channels_on_handle(handle);
            p_link =  prh_l2_acl_find_handle(handle);
			if (p_link==0)
				return BT_NOERROR;
			
			switch (p_link->op_pending)
			{
			case L2CAP_ECHO_REQUEST :
				{
					/* Callback the Upper layer and return the link to Idle. */
					result = 0x01; /* Timeout Occured */
					p_link->ping_callback(p_link->bd_addr,(u_int16)result,p_link->echo_data, p_link->length);
					prh_l2_acl_free_link(p_link);
				}
				break;
				
#if (L2CAP_INFO_SUPPORTED==1)
			case L2CAP_INFORMATION_REQUEST :
				{
				/* Callback the Upper layer with the L2_Info_Callback
				* and return the link to Idle.
					*/
					result = 0x01; /* Timeout Occured */
					p_link->info_callback(p_link->bd_addr,(u_int16)result, NULL,0 /* Zero Length */);
					prh_l2_acl_free_link(p_link);
				}
				break;
#endif
			default :
				prh_l2_acl_free_link(p_link);
				break;
			}
		}
		break;
		
    case HCI_evFLUSHOCCURRED :
		handle = BT_READ_LE_UINT16(pdu+2);
		prh_l2_chn_data_error_for_channels_on_handle(handle,L2CAP_evFLUSHOCCURRED);
		break;

    case HCI_evDATABUFFEROVERFLOW:
		handle = BT_READ_LE_UINT16(pdu+2);
		prh_l2_chn_data_error_for_channels_on_handle(handle,L2CAP_evBUFFEROVERFLOW);
		break;

    case HCI_evQOSVIOLATION:
		handle = BT_READ_LE_UINT16(pdu+2);
        prh_l2_chn_data_error_for_channels_on_handle(handle,L2CAP_evQOSVIOLATIONIND);
        break;
        
    case HCI_evCOMMANDCOMPLETE:
        if (BT_READ_LE_UINT16(pdu+3) == HCI_cmdREADBUFFERSIZE) 
		{
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"L2CAP -- acting on HCI_cmdREADBUFFERSIZE HCI_evCOMMANDCOMPLETE event from HCI\n"));
            g_l2cap_acl_size = BT_READ_LE_UINT16(pdu+6);
        }
        break;
    }
#endif	
    return BT_NOERROR;
}

/**************************************************************************************
 *
 *  FUNCTION :-  prh_l2_connect_lp_connect_cfm
 *
 *  Input Parameters
 *      tid_t Transaction_Id    An identifer for the Link Establishment operation
 *      t_api result            Indicates if the connection succeded or failed.
 *
 *  Returns
 *      t_api
 *
 *  Description
 *  This function is called back when the ACL link establishment operation is
 *  complete. The operation to be performed are dependent on the "op_pending"
 *  field of the link structure.
 *****************************************************************************/

void prh_l2_connect_lp_connect_cfm(tid_t Transaction_Id,t_api result)
{
    t_api status = BT_NOERROR;
    prh_t_l2_channel   * p_channel=NULL;
    prh_t_l2_acl_link  * p_link=NULL;
    prh_t_l2_pdu pdu_info;

    t_L2_ControlCallbackArgs  eventinfo;
    u_int8 psm_index=0;

    /* First get the p_link from the Transation Id */

    pMemset(&pdu_info,0,sizeof(pdu_info));
	pMemset(&eventinfo,0,sizeof(eventinfo));
	
    /* The Transaction Id is a void* used as a unique identifier */
    p_link = (prh_t_l2_acl_link*)Transaction_Id;
    
     pDebugPrintfEX((pLOGERROR,pLOGL2CAP," prh_l2_connect_lp_connect_cfm p_link=0x%x\n",p_link));
     
    if (p_link == 0)
    {
#if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGL2CAP," Invalid Transaction Id received in lp_connect_cfm \n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_CONNCECT_SM_INDEX+10),NULL,0,0);
#endif
        return;
    }
    switch (p_link->op_pending)
    {

    case L2_CHANNEL_SETUP :
         /* Use pending_cid field to derive channel */
        p_channel = prh_l2_chn_get_p_channel(p_link->pending_cid);
        if (p_channel == 0)
            return;

        if ((PRH_L2_W4_LP_CONNECT_CFM == p_channel->state) &&
           (BT_NOERROR == result))
        {   
            p_channel->local_transaction_identifier = prh_l2_enc_derive_identifier();
            p_link->handle = p_channel->handle;
            p_link->initiator = 0x01;
#if HOST_SECURITY
            {
                t_bdaddr *p_dest_bd_addr;
                t_api security_status;
                
                p_dest_bd_addr = prh_l2_acl_get_bd_addr(p_channel->handle);
                
                security_status = MGR_MultiplexOutgoingAccessRequest(L2CAP_PROTOCOLID,
                    (u_int32)p_channel->psm, *p_dest_bd_addr, 
                    (u_int32)p_channel->local_cid,L2_OutgoingAccessRequest_Callback);
                
                if(BT_PENDING == security_status)
                    return;
                
                if(BT_NOERROR != security_status)
                {
                    eventinfo.cid = p_channel->local_cid;
                    eventinfo.result = L2CAP_CONNECTION_REFUSED_SECURITY_BLOCK;
                    eventinfo.status = 0;

                    /* PSM & BDADDR seem un-necessary */
                    eventinfo.psm = p_channel->psm;
                    
                    eventinfo.BDAddress = p_link->bd_addr;
                    
                    psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
                    prh_l2_chn_free_channel(p_channel, PRH_L2_RELEASE_ACL);
                    prh_l2_psm_event(psm_index, L2CAP_evCONNECTCONF,&eventinfo);
				
                    return;
                }
                /* If MGR returned BT_NOERROR, L2CAP can continue immediately with channel setup */
            }
#endif
            p_channel->state = PRH_L2_W4_L2CAP_CONNECT_RSP;
            pdu_info.length = 0x04;
            pdu_info.p_channel = p_channel;            
            status = prh_l2_encode_packet(p_link->handle,L2CAP_CONNECTION_REQUEST, &pdu_info);
            if(BT_NOERROR == status)
                prh_l2_chn_start_RTX_timer(p_channel);   
        } 
        else if ((PRH_L2_W4_LP_CONNECT_CFM == p_channel->state) && (BT_NOERROR != result))
        {
            /* Release the link & inform the Higer Protocol using a callback */
            eventinfo.cid = p_channel->local_cid;
            eventinfo.result = L2CAP_CONNECTION_TIMEOUT;
            eventinfo.status = 0;

            /* PSM & BDADDR seem uncecessary */
            eventinfo.psm = p_channel->psm;
            eventinfo.BDAddress = p_link->bd_addr;

            psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);

            prh_l2_chn_free_channel(p_channel,PRH_L2_DONT_RELEASE_ACL|PRH_L2_DONT_RELEASE_CONFIG);
            prh_l2_acl_free_link(p_link);

            prh_l2_psm_event(psm_index, L2CAP_evCONNECTCONF,&eventinfo);
			
        }
        break;
#if (L2CAP_PING_SUPPORTED==1)
    case L2_ECHO_REQ :
        if(BT_NOERROR == result) 
        {
			p_link->initiator = 0x01;
            pMemset(&pdu_info,0,sizeof(pdu_info));
            p_link->op_pending = L2CAP_ECHO_REQUEST;

            /* Send an Echo Request PDU        */

            pdu_info.identifier = p_link->identifier;
            pdu_info.length = p_link->echo_len;
            pdu_info.p_data = p_link->echo_data;
            prh_l2_encode_packet(p_link->handle,L2CAP_ECHO_REQUEST, &pdu_info);   
        } 
        else
        {
            /* Callback the Upper layer and return the link to Idle. */
            result = 0x01; /* Timeout Occured */
            p_link->ping_callback(p_link->bd_addr,(u_int16)result,p_link->echo_data, p_link->length);
            prh_l2_acl_free_link(p_link);
        }
        break;
#endif

#if (L2CAP_INFO_SUPPORTED==1)
    case L2_INFO_REQ :
        if ((p_link) && (result==0))
        {
			p_link->initiator = 0x01;
			pMemset(&pdu_info,0,sizeof(pdu_info));
            p_link->op_pending = L2CAP_INFORMATION_REQUEST;

            /* Send an Get Info Request PDU    */
            pdu_info.identifier = p_link->identifier;
            pdu_info.length = 0x02;
            pdu_info.info_type = p_link->info_type;
            prh_l2_encode_packet(p_link->handle,L2CAP_INFORMATION_REQUEST, &pdu_info); 
        }
        else
        {
            /* Callback the Upper layer with the L2_Info_Callback
            * and return the link to Idle.
            */
			 result = 0x01; /* Timeout Occured */
             p_link->info_callback(p_link->bd_addr,(u_int16)result, NULL,0 /* Zero Length */);
             prh_l2_acl_free_link(p_link);
        }
        break;
#endif

#if (L2CAP_GROUPS_SUPPORTED==1)
    case L2_GROUP_ADD :
        if (p_link) {
            if (result!=0)
                result=1; /* Connection Failure */
            prh_l2_group_lp_add_complete(p_link,result);
        }
        break;
#endif
    }
}

/********************************************************************
 *  FUNCTION :-  prh_l2_connect_lp_connect_ind
 *
 *  Input Parameters
 *      t_bdaddr* p_bd_addr     A pointer to the Bluetooth device address of 
 *                              of the device which initiated the connection.
 *      u_int16 handle          The connection handle allocated by the lower
 *                              stack to the ACL link
 *
 *  Returns 
 *      void
 *
 *  DESCRIPTION :- 
 *  Handles a connection indication from the lower stack layers. A new entry for the
 *  bd_addr is inserted into the Link Container. If memory cannot be allocated
 *  for the link - it is released us Low_Resources as the release reason.
 *
 ********************************************************************/

void prh_l2_connect_lp_connect_ind(t_bdaddr* p_bd_addr, u_int16 handle)
{
    prh_t_l2_acl_link* p_link;

    p_link = prh_l2_acl_exists(p_bd_addr);

	if(p_link != 0)
    {
		p_link->initiator = 0x01;
    }
    if(p_link == 0)
        p_link = prh_l2_acl_alloc_link(p_bd_addr,0);
    
    if(p_link != 0)
    {
        p_link->handle = handle;
    }
    else
    {
#if COMBINED_HOST==1
	 LMconnection_LM_Disconnect_Req(p_link->handle,PRH_USER_ENDED_CONNECTION);
#else
        prh_mgr_releaseACL(handle,PRH_LOW_RESOURCES, NULL);
#endif
    }

}

