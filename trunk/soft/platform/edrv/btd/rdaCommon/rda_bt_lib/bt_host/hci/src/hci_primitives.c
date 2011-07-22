/*
 * MODULE NAME:    hci_primitives.c
 * DESCRIPTION:    HCI Service primitives
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_primitives.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */


#include "host_config.h"


#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"
#include "hci_debug.h"
#include "hci_constants.h"
#include "hci_primitives.h"

#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_hcit.h"
#else
#include "hci_transport.h"
#endif

#include "hci_callbacks.h"
#include "hci_encoder.h"
#include "papi.h"
#include "host_ctrl.h"

#include "mgr_core.h"
#include "mgr_utils.h"
volatile u_int8 prh_hci_running=0;
u_int8 prh_hci_security_mode_pending;

#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_hci_fc.h"
#else
#include "hci_flow_ctrl.h"
#endif


#if pDEBUG
   char *prh_hci_eventStatus2string(u_int16 status); /* from hci_debug.c */
#endif

void prh_hci_reset_timer_callback(void *args);
volatile t_api prh_host_startup_complete=BT_INVALIDERRORCODE;
/*************************************************************************************************
 * prh_hci_layer_ctrl
 *
 ************************************************************************************************/
t_api prh_hci_layer_ctrl(u_int8 flag) {
    if (flag&PRH_HOST_STARTUP || flag&PRH_HOST_RESET) {
		if (flag&PRH_HOST_RESET) {
			prh_hci_running=0;
			prh_hci_fc_shutdown();   
		}
        prh_hci_manager_event_callback_func=NULL;
        prh_hci_l2cap_event_callback_func=NULL;
        prh_hci_l2cap_acl_callback_func=NULL;
		prh_host_startup_complete=BT_INVALIDERRORCODE;
        prh_hci_fc_init();
        pMemset(prh_hci_app_event_callback_func,0,MAX_OUTSTANDING_CALLBACKS*sizeof(void*));
		if(!(flag&PRH_HOST_RESET))
			pMemset(prh_hci_app_global_rx_callback_func,0,MAX_OUTSTANDING_CALLBACKS*sizeof(void*));
        prh_hci_running=1;
        HCI_Transport_Set_Rx_CB(prh_hci_recv_callback); /* setup callback from transport driver */   
    } else if (flag&PRH_HOST_SHUTDOWN) {
        prh_hci_running=0;
        prh_hci_fc_shutdown();  
    }
    return BT_NOERROR;
}

/*************************************************************************************************
 * HCI_RegisterEventHandler
 *
 * Registers a handler for events received from the HC.
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 HCI_RegisterEventHandler(void (*callbackFunc)(u_int8 *eventPdu)) {
    u_int8 i=0;
    while (prh_hci_app_event_callback_func[i] && i<MAX_OUTSTANDING_CALLBACKS)
        i++;
    if (i>=MAX_OUTSTANDING_CALLBACKS)
        return BT_NORESOURCES;
    prh_hci_app_event_callback_func[i]=callbackFunc;
    return BT_NOERROR;
}

/*************************************************************************************************
 * HCI_DeregisterEventHandler
 *
 * De-registers a handler for events received from the HC.
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 HCI_DeregisterEventHandler(void (*callbackFunc)(u_int8 *eventPdu)) {
    u_int8 i=0;
    while (prh_hci_app_event_callback_func[i]!=callbackFunc && i<MAX_OUTSTANDING_CALLBACKS)
        i++;
    if (i==MAX_OUTSTANDING_CALLBACKS)
        return BT_INVALIDPARAM;
    prh_hci_app_event_callback_func[i]=NULL;
    return BT_NOERROR;
}


/*************************************************************************************************
 * HCI_RegisterRxHandler
 *
 * Registers a handler for all pdus received from the HC.
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 HCI_RegisterRxHandler(void (*callbackFunc)(u_int8 type, u_int8 *pdu, u_int32 len)) {
    u_int8 i=0;
    while (prh_hci_app_global_rx_callback_func[i] && i<MAX_OUTSTANDING_CALLBACKS)
        i++;
    if (i==MAX_OUTSTANDING_CALLBACKS)
        return BT_NORESOURCES;
    prh_hci_app_global_rx_callback_func[i]=callbackFunc;
    return BT_NOERROR;
}

/*************************************************************************************************
 * HCI_DeregisterRxHandler
 *
 * Registers a handler for all pdus received from the HC.
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 HCI_DeregisterRxHandler(void (*callbackFunc)(u_int8 type, u_int8 *pdu, u_int32 len)) {
    u_int8 i=0;
    while (prh_hci_app_global_rx_callback_func[i]!=callbackFunc && i<MAX_OUTSTANDING_CALLBACKS)
        i++;
    if (i==MAX_OUTSTANDING_CALLBACKS)
        return BT_INVALIDPARAM;
    prh_hci_app_global_rx_callback_func[i]=NULL;
    return BT_NOERROR;
}


/*************************************************************************************************
 * HCI_GetWriteBuffer
 *
 * Allocates a data buffer suitable for passing to HCI primitives (i.e. HCI_SendRawPDU).
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 HCI_GetWriteBuffer(u_int16 handle, u_int16 len, struct st_t_dataBuf **writeBuffer) {
    if (handle == HCI_NO_HANDLE)
      return prh_hci_GetCommandBuffer(len, 0, (struct host_buf **)writeBuffer);

    if (handle == HCI_ACTIVE_BROADCAST_HANDLE)
	   handle = 0x4000;
    else if (handle == HCI_PICONET_BROADCAST_HANDLE)
	   handle = 0x8000;

    if (handle & HCI_SCO_HANDLE)
    {
        handle &= ~HCI_SCO_HANDLE;
	    return prh_hci_GetSCOBuffer(handle, len, 0, (struct host_buf **)writeBuffer); 
	}
	else
	    return prh_hci_GetWriteBuffer(handle, len, 0, (struct host_buf **)writeBuffer);

    return BT_INVALIDPARAM;
}

/*************************************************************************************************
 * HCI_FreeWriteBuffer
 *
 * Releases a HCI data buffer allocated by HCI_GetWriteBuffer.
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 HCI_FreeWriteBuffer(struct st_t_dataBuf *writeBuffer) {
    return prh_hci_FreeWriteBuffer((struct host_buf *)writeBuffer);
}

/*************************************************************************************************
 * HCI_SendRawPDU
 *
 * Sends a pdu constructed by the application.
 * Broadcast ACLs can be sent on any handle 
 * - this will be overwritten in hci_flowctrl.c by the correct HCI broadcast handle.
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 HCI_SendRawPDU(u_int8 type, struct st_t_dataBuf *pdu) {

    switch (type) {

        case HCI_pduCOMMAND:
            return prh_hci_fc_send_command_pdu((struct host_buf *)pdu);
            break;

        case HCI_pduACLDATA:
            return prh_hci_fc_send_data_pdu(HCI_pduACLDATA, (u_int16)(BT_READ_LE_UINT16(pdu->buf)&0xFFF),(struct host_buf *)pdu);
            break;
            
        case HCI_pduSCODATA:
            return prh_hci_fc_send_data_pdu(HCI_pduSCODATA, (u_int16)(BT_READ_LE_UINT16(pdu->buf)&0xFFF),(struct host_buf *)pdu);
            break;
    };
    
    return BT_INVALIDPARAM;
}


/*
    Callback setup functions 
*/


/*************************************************************************************************
 * prh_hci_register_manager_event_cb
 *
 * Registers a callback used to pass events data to the manager.
 ************************************************************************************************/
t_api prh_hci_register_manager_event_cb(t_api (*callbackfunc)(u_int8 *)) {
    prh_hci_manager_event_callback_func=callbackfunc;
    return BT_NOERROR;
}


/*************************************************************************************************
 * prh_hci_register_l2cap_event_cb
 *
 * Registers a callback used to pass events to L2CAP.
 ************************************************************************************************/
t_api prh_hci_register_l2cap_event_cb(t_api (*callbackfunc)(u_int8 *)) {
    prh_hci_l2cap_event_callback_func=callbackfunc;
    return BT_NOERROR;
}

/*************************************************************************************************
 * prh_hci_register_l2cap_acl_cb
 *
 * Registers a callback used to pass ACL data to L2CAP.
 ************************************************************************************************/
t_api prh_hci_register_l2cap_acl_cb(t_api (*callbackfunc)(u_int16 handle, struct host_buf *pdu, u_int8 flags)) {
    prh_hci_l2cap_acl_callback_func=callbackfunc;
    return BT_NOERROR;
}



/*
    HCI data primitives
*/

t_api prh_hci_send_acl_child_list(u_int16 handle, struct host_buf *bufferParent, u_int8 flags) {

    struct host_buf *current;
    u_int8 pb, bf;
    t_api status;
    u_int32 totalBytes;

    current=bufferParent;
    /* skip over the parent buffer if there are children */
    if (current->child_count>1)
      current=bufferParent->next;
    
    while (current) {

      /* Build ACL header on each segment */
      /* is this the first fragment? */
      if (current->data==bufferParent->data)
	pb=0x02;
      else
	pb=0x01;
      
      if (flags&prh_hci_ACL_ACTIVE_BROADCAST)
	bf=0x01;
      else if (flags&prh_hci_ACL_PICONET_BROADCAST)
	bf=0x02;
      else        
	bf=0x00;
      
      current->hci_handle_flags=handle;
      current->hci_handle_flags |= (pb<<12) | (bf<<14);
      
      current=current->next;
    }

    if (bufferParent->child_count>1)
      {
	totalBytes = bufferParent->len + (bufferParent->child_count-1)*sizeof(struct host_buf);
	status = prh_hci_fc_enqueue_child_list(handle, bufferParent->next, totalBytes);
      }
    else
      {
	totalBytes = bufferParent->totalsize + sizeof(struct host_buf);
	status = prh_hci_fc_enqueue_child_list(handle, bufferParent, totalBytes);
      }
    
    if (status) {
      pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"prh_hci_send_acl_list: prh_hci_fc_send_acl_list returned an error - cleaning up\n"));
    }
    return status;
}


/*************************************************************************************************
 * HCI Command Functions
 *
 * These functions implement the HCI command set. All but 4 of these function use the table
 * driven encoder HCI_GenerateCommand. The exceptions are constructed in the functions themselves.
 ************************************************************************************************/




/*
    HCI link control commands - Opcode Group Field = 0x01
*/

APIDECL1 t_api APIDECL2 HCI_Inquiry(u_int32 lap, u_int8 inquiryLength, u_int8 numResponses) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending inquiry command, LAP=0x%03X, inquiryLength=0x%01X, numResponses=0x%01X",lap,inquiryLength,numResponses));
    return HCI_GenerateCommand(HCI_cmdINQUIRY,lap,inquiryLength,numResponses);  
}

APIDECL1 t_api APIDECL2 HCI_InquiryCancel(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending inquiry_cancel command"));
    return HCI_GenerateCommand(HCI_cmdINQUIRYCANCEL);   
}

APIDECL1 t_api APIDECL2 HCI_PeriodicInquiryMode(u_int16 maxPeriodLength, u_int16 minPeriodLength, u_int32 lap, u_int8 inquiryLength, u_int8 numResponses) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending periodic_inquiry_mode command, maxPeriodLength=0x%02X, minPeriodLength=0x%02X, LAP=0x%03X, inquiryLength=0x%01X, numResponses=0x%01X"
        ,maxPeriodLength,minPeriodLength,lap,inquiryLength,numResponses));
    return HCI_GenerateCommand(HCI_cmdPERIODICINQUIRYMODE,maxPeriodLength,minPeriodLength,lap,inquiryLength,numResponses);
}

APIDECL1 t_api APIDECL2 HCI_ExitPeriodicInquiryMode(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending exit_periodic_inquiry_mode command"));
    return HCI_GenerateCommand(HCI_cmdEXITPERIODICINQUIRYMODE);
}


APIDECL1 t_api APIDECL2 HCI_CreateConnection(t_bdaddr bdaddr, u_int16 packetType, u_int8 pageScanRepet, u_int8 pageScanMode, u_int16 clockOffset, u_int8 role) {   

pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_CreateConnection, bdaddr=%s, packetType=0x%02X, pageScanRepet=0x%01X"
        ,prh_mgr_printBDAddress(bdaddr),packetType,pageScanRepet));
pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_CreateConnection, pageScanMode=0x%01X, clockOffset=0x%02X, role=0x%01X"
        ,pageScanMode,clockOffset, role));


#ifdef MGR_SECURITY_TEST
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: MGR_SECURITY_TEST is defined, sending artificial connection_complete events instead"));
    return prh_hci_debug_send_connection_complete_event(bdaddr,1);    
#endif

    return HCI_GenerateCommand(HCI_cmdCREATECONNECTION,bdaddr.bytes,packetType,pageScanRepet,pageScanMode,clockOffset, role);

}

APIDECL1 t_api APIDECL2 HCI_Disconnect(u_int16 handle, u_int8 reason) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending disconnect command, handle=0x%02X, reason=0x%01X (%s)",handle,reason,prh_hci_eventStatus2string(reason)));
    return HCI_GenerateCommand(HCI_cmdDISCONNECT,handle,reason);    
}


APIDECL1 t_api APIDECL2 HCI_AddSCOConnection(u_int16 handle, u_int16 packetType) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending add_sco_connection command, handle=0x%02X, packetType=0x%02X",handle,packetType));
    return HCI_GenerateCommand(HCI_cmdADDSCOCONNECTION,handle,packetType);
}

APIDECL1 t_api APIDECL2 HCI_AcceptConnectionRequest(t_bdaddr bdaddr, u_int8 role) {  
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending accept_connection_request command, bdaddr=%s, role=0x%01X",prh_mgr_printBDAddress(bdaddr),role));
    return HCI_GenerateCommand(HCI_cmdACCEPTCONNECTIONREQUEST,bdaddr.bytes,role);   
}

APIDECL1 t_api APIDECL2 HCI_RejectConnectionRequest(t_bdaddr bdaddr, u_int8 reason) {  
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending reject_connection_request command, bdaddr=%s, reason=0x%01X",prh_hci_debug_print_large_number(bdaddr.bytes,6),reason));
    return HCI_GenerateCommand(HCI_cmdREJECTCONNECTIONREQEUST,bdaddr.bytes,reason); 
}
APIDECL1 t_api APIDECL2 HCI_LinkKeyRequestReply(t_bdaddr bdaddr, u_int8 *linkKey) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending link_key_request_reply command, bdaddr=%s, linkkey=%s",prh_mgr_printBDAddress(bdaddr)
        ,prh_mgr_printLargeNumber(linkKey,16)));
    return HCI_GenerateCommand(HCI_cmdLINKKEYREQUESTREPLY,bdaddr.bytes,linkKey);
}
APIDECL1 t_api APIDECL2 HCI_LinkKeyRequestNegativeReply(t_bdaddr bdaddr) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending link_key_request_negative_reply command, bdaddress=%s",prh_mgr_printBDAddress(bdaddr)));
    return HCI_GenerateCommand(HCI_cmdLINKKEYREQUESTNEGATIVEREPLY,bdaddr.bytes);
}
APIDECL1 t_api APIDECL2 HCI_PINCodeRequestReply(t_bdaddr bdaddr, u_int8 pinCodeLen, u_int8 *pincode)
   {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_PINCodeRequestReply, bdaddress=%s, pincodeLen=0x%01X, pincode=%s"
        ,prh_mgr_printBDAddress(bdaddr),pinCodeLen,prh_mgr_printLargeNumber(pincode,pinCodeLen)));
    return HCI_GenerateCommand(HCI_cmdPINCODEREQUESTREPLY,bdaddr.bytes,pinCodeLen,pincode);
}
APIDECL1 t_api APIDECL2 HCI_PINCodeRequestNegativeReply(t_bdaddr bdaddr) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending pincode_request_negative_reply command, bdaddress=%s",prh_mgr_printBDAddress(bdaddr)));
    return HCI_GenerateCommand(HCI_cmdPINCODEREQUESTNEGATIVEREPLY,bdaddr.bytes);
}
APIDECL1 t_api APIDECL2 HCI_ChangeConnectionPacketType(u_int16 handle, u_int16 packetType) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending change_connection_packet_type command, handle=0x%02X, packetType=0x%02X",handle,packetType));
    return HCI_GenerateCommand(HCI_cmdCHANGECONNECTIONPACKETTYPE,handle,packetType);
}
APIDECL1 t_api APIDECL2 HCI_AuthenticationRequested(u_int16 handle) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending authentication_requested command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdAUTHENTICATIONREQUESTED,handle);
}

APIDECL1 t_api APIDECL2 HCI_SetConnectionEncryption(u_int16 handle, u_int8 encrypt) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending set_connection_encryption command, handle=0x%02X, encrypt=0x%01X",handle, encrypt));
    return HCI_GenerateCommand(HCI_cmdSETCONNECTIONENCRYPTION,handle,encrypt);

}
APIDECL1 t_api APIDECL2 HCI_ChangeConnectionLinkKey(u_int16 handle) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending change_connection_link_key command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdCHANGECONNECTIONLINKKEY,handle);
}
APIDECL1 t_api APIDECL2 HCI_MasterLinkKey(u_int8 keyFlag) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending master_link_key command, keyflag=0x%01X",keyFlag));
    return HCI_GenerateCommand(HCI_cmdMASTERLINKKEY,keyFlag);
}
APIDECL1 t_api APIDECL2 HCI_RemoteNameRequest(t_bdaddr bdaddr, u_int8 pageScanRepet, u_int8 pageScanMode, u_int16 clockOffset) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending remote_name_request command, bdaddr=%s, pageScanRepet=0x%01X, pageScanMode=0x%01X, clockOffset=0x%02X",prh_hci_debug_print_large_number(bdaddr.bytes,6),pageScanRepet,pageScanMode,clockOffset));
    return HCI_GenerateCommand(HCI_cmdREMOTENAMEREQUEST,bdaddr.bytes,pageScanRepet,pageScanMode,clockOffset);   
} 

APIDECL1 t_api APIDECL2 HCI_RemoteNameRequestCancel(t_bdaddr bdaddr)
{
     pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending HCI_RemoteNameRequestCancel command, bdaddr=%s",prh_hci_debug_print_large_number(bdaddr.bytes,6)));
     return HCI_GenerateCommand(HCI_cmdREMOTENAMEREQUESTCANCEL, bdaddr.bytes);
}


APIDECL1 t_api APIDECL2 HCI_ReadRemoteSupportedFeatures(u_int16 handle) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_remote_features command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdREADREMOTESUPPORTEDFEATURES,handle);  
}
APIDECL1 t_api APIDECL2 HCI_ReadRemoteVersionInformation(u_int16 handle) {      
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_remote_version_information command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdREADREMOTEVERSIONINFORMATION,handle);
}

APIDECL1 t_api APIDECL2 HCI_ReadClockOffset(u_int16 handle) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_clock_offset command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdREADCLOCKOFFSET,handle);
}



/*
    HCI link policy commands - Opcode Group Field = 0x02
*/
APIDECL1 t_api APIDECL2 HCI_HoldMode(u_int16 handle, u_int16 maxInterval, u_int16 minInterval) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending hold_mode command, handle=0x%03X, maxInterval=0x%02X, minInterval=0x%02X",handle,maxInterval,minInterval));
    return HCI_GenerateCommand(HCI_cmdHOLDMODE,handle,maxInterval,minInterval);
}   
APIDECL1 t_api APIDECL2 HCI_SniffMode(u_int16 handle, u_int16 maxInterval, u_int16 minInterval, u_int16 sniffAttempt, u_int16 sniffTimeout) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending sniff_mode command, handle=0x%03X, maxInterval=0x%02X, minInterval=0x%02X, sniffAttempt=0x%02X, sniffTimeout=0x%02X",handle,maxInterval,minInterval,sniffAttempt, sniffTimeout));
    return HCI_GenerateCommand(HCI_cmdSNIFFMODE,handle,maxInterval,minInterval,sniffAttempt,sniffTimeout);
}
APIDECL1 t_api APIDECL2 HCI_ExitSniffMode(u_int16 handle) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending exit_sniff_mode command, handle=0x%03X",handle));
    return HCI_GenerateCommand(HCI_cmdEXITSNIFFMODE,handle);
}
APIDECL1 t_api APIDECL2 HCI_ParkMode(u_int16 handle, u_int16 beaconMaxInterval, u_int16 beaconMinInterval) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending park_mode command, handle=0x%03X, beaconMaxInterval=0x%02X, beaconMinInterval=0x%02X",
        handle,beaconMaxInterval,beaconMinInterval));
    return HCI_GenerateCommand(HCI_cmdPARKMODE,handle,beaconMaxInterval,beaconMinInterval);
}
APIDECL1 t_api APIDECL2 HCI_ExitParkMode(u_int16 handle) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending exit_park_mode command, handle=0x%03X",handle));
    return HCI_GenerateCommand(HCI_cmdEXITPARKMODE,handle);
}
APIDECL1 t_api APIDECL2 HCI_QoSSetup(u_int16 handle, u_int8 flags, u_int8 serviceType, u_int32 tokenRate, u_int32 peakBandwidth, u_int32 latency, u_int32 delayVar) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending qos_setup command, handle=0x%02X, flags=0x%01X, serviceType=0x%01X, tokenRate=0x%04X, peakBandwidth=0x%04X, latency=0x%04X, delayVar=0x%04X"
        ,handle,flags,serviceType,tokenRate,peakBandwidth,latency,delayVar));
    return HCI_GenerateCommand(HCI_cmdQOSSETUP,handle,flags,serviceType,tokenRate,peakBandwidth,latency,delayVar);
}
APIDECL1 t_api APIDECL2 HCI_RoleDiscovery(u_int16 handle) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending role_discovery command, handle=0x%03X",handle));
    return HCI_GenerateCommand(HCI_cmdROLEDISCOVERY,handle);
}
APIDECL1 t_api APIDECL2 HCI_SwitchRole(t_bdaddr bdaddr, u_int8 role) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending switch_role command, bdaddr=%s, role=0x%01X",prh_hci_debug_print_large_number(bdaddr.bytes,6),role));
    return HCI_GenerateCommand(HCI_cmdSWITCHROLE,bdaddr.bytes,role);    
}
APIDECL1 t_api APIDECL2 HCI_ReadLinkPolicySettings(u_int16 handle) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_link_policy_settings command, handle=0x%03X",handle));
    return HCI_GenerateCommand(HCI_cmdREADLINKPOLICYSETTINGS,handle);
}
APIDECL1 t_api APIDECL2 HCI_WriteLinkPolicySettings(u_int16 handle, u_int16 settings) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_link_policy_settings command, handle=0x%03X, settings=0x%02X",handle,settings));
    return HCI_GenerateCommand(HCI_cmdWRITELINKPOLICYSETTINGS,handle,settings);
}


/*
    HCI host controller and baseband commands - Opcode Group Field = 0x03
*/

APIDECL1 t_api APIDECL2 HCI_SetEventMask(u_int8 *mask) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending set_event_mask command, mask=%s",prh_hci_debug_print_large_number(mask,8)));
    return HCI_GenerateCommand(HCI_cmdSETEVENTMASK,mask);
}
APIDECL1 t_api APIDECL2 HCI_Reset(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending reset command"));
    return HCI_GenerateCommand(HCI_cmdRESET);
}

APIDECL1 t_api APIDECL2 HCI_SetEventFilter(t_HCI_EventFilter *filter) {
    /* This command is an exception as it cannot be generated completely from the table */

    struct host_buf *pdu;
    t_api status;
    u_int8 parmlen;

    status=prh_hci_GetCommandBuffer(HCI_cmdSETEVENTFILTER_LEN+HEADERS_FOR_HCITRANSPORT, 0, &pdu);
    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_SetEventFilter failed to allocate command buffer for %d bytes\n",HCI_cmdSETEVENTFILTER_LEN));
        return BT_NORESOURCES;
    }

    pdu->data[0]=HCI_cmdSETEVENTFILTER&0xFF;
    pdu->data[1]=(HCI_cmdSETEVENTFILTER>>8)&0xFF;

    pdu->data[3]=filter->FilterType;
    pdu->data[4]=filter->ConditionType;
    
    parmlen=2;

    switch (filter->FilterType) {
        case 0x00:
            /* clear all filters */
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending set_event_filter command, filterType=0x00 (CLEARALLFILTERS)"));
            parmlen=1;
            break;
        case 0x01:
            /* inquiry result filter */
            switch (filter->ConditionType) {
                case 0x00:
                    /* don't filter */
                    break;
                case 0x01:
                    /* filter on COD */
                    pdu->data[5]=filter->Condition.DeviceClass&0xFF;
                    pdu->data[6]=(filter->Condition.DeviceClass>>8)&0xFF;
                    pdu->data[7]=(filter->Condition.DeviceClass>>16)&0xFF;
                    pdu->data[8]=filter->Condition.DeviceClassMask&0xFF;
                    pdu->data[9]=(filter->Condition.DeviceClassMask>>8)&0xFF;
                    pdu->data[10]=(filter->Condition.DeviceClassMask>>16)&0xFF;
                    parmlen+=6;
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending set_event_filter command, filterType=0x01 (INQUIRYFILTER), conditionType=0x01 (CODFILTER), deviceClass=0x%03X, deviceClassMask=0x%02X",
                        filter->Condition.DeviceClass,filter->Condition.DeviceClassMask));
                    break;
                case 0x02:
                    /* filter on address */
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending set_event_filter command, filterType=0x01 (INQUIRYFILTER), conditionType=0x02 (BDADDRFILTER), BDAddress=%s",
                        prh_mgr_printBDAddress(filter->Condition.BDAddress)));
                    pMemcpy(pdu->data+5,filter->Condition.BDAddress.bytes,6);
                    parmlen+=6;
                    break;
                default:
                    prh_hci_ReleaseCommandBuffer(pdu);
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_SetEventFilter: Invalid parameters, filterType=0x01 (INQUIRYFILTER), conditionType=0x%01X (UNKNOWN)",filter->ConditionType));
                    return BT_INVALIDPARAM;
            }
            break;
        case 0x02:
            /* connection setup filter */
            switch (filter->ConditionType) {
                case 0x00:
                    /* autoaccept all connections */
                    pdu->data[5]=filter->Condition.AutoAccept;
                    parmlen+=1;
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending set_event_filter command, filterType=0x02 (CONNSETUP), conditionType=0x00 (ALLDEVICES), autoAcceptFlag=0x%01X"
                        ,filter->Condition.AutoAccept));
                    break;
                case 0x01:
                    /* filter on COD */
                    pdu->data[5]=filter->Condition.DeviceClass&0xFF;
                    pdu->data[6]=(filter->Condition.DeviceClass>>8)&0xFF;
                    pdu->data[7]=(filter->Condition.DeviceClass>>16)&0xFF;
                    pdu->data[8]=filter->Condition.DeviceClassMask&0xFF;
                    pdu->data[9]=(filter->Condition.DeviceClassMask>>8)&0xFF;
                    pdu->data[10]=(filter->Condition.DeviceClassMask>>16)&0xFF;
                    pdu->data[11]=filter->Condition.AutoAccept;
                    parmlen+=7;
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending set_event_filter command, filterType=0x02 (CONNSETUP), conditionType=0x01 (CODFILTER), deviceClass=0x%03X, deviceClassMask=0x%02X, autoAcceptFlag=0x%01X"
                        ,filter->Condition.DeviceClass,filter->Condition.DeviceClassMask,filter->Condition.AutoAccept));
                    break;
                case 0x02:
                    /* filter on address */
                    pMemcpy(pdu->data+5,filter->Condition.BDAddress.bytes,6);
                    pdu->data[11]=filter->Condition.AutoAccept;
                    parmlen+=7;
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending set_event_filter command, filterType=0x02 (CONNSETUP), conditionType=0x02 (BDADDRFILTER), BDAddress=%s, autoAcceptFlag=0x%01X"
                        ,prh_mgr_printBDAddress(filter->Condition.BDAddress),filter->Condition.AutoAccept));
                    break;
                default:
                    prh_hci_ReleaseCommandBuffer(pdu);
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_SetEventFilter: Invalid parameters, filterType=0x02 (CONNSETUP), conditionType=0x%01X (UNKNOWN)",filter->ConditionType));
                    return BT_INVALIDPARAM;
            }
            break;
        default:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_SetEventFilter: Invalid filterType=0x%01X (UNKNOWN)",filter->FilterType));
            prh_hci_ReleaseCommandBuffer(pdu);
            return BT_INVALIDPARAM;
    }

    pdu->data[2]=parmlen;
    pdu->len=HCI_pduCOMMANDHEADER+parmlen;

    /* add pdu to flow control queue */
    status=prh_hci_fc_send_command_pdu(pdu); 

    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_SetEventFilter failed to queue Command PDU for sending - prh_hci_fc_send_command_pdu() returned error %d\n",status));
        prh_hci_ReleaseCommandBuffer(pdu);
    }
    return status;
}

APIDECL1 t_api APIDECL2 HCI_Flush(u_int16 handle) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending flush command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdFLUSH,handle);
}
APIDECL1 t_api APIDECL2 HCI_ReadPINType(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_pin_type command"));
    return HCI_GenerateCommand(HCI_cmdREADPINTYPE);
}
APIDECL1 t_api APIDECL2 HCI_WritePINType(u_int8 pinType) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_pin_type command, pinType=0x%01X",pinType));
    return HCI_GenerateCommand(HCI_cmdWRITEPINTYPE,pinType);
}
APIDECL1 t_api APIDECL2 HCI_CreateNewUnitKey(void) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending create_new_unit_key command"));
    return HCI_GenerateCommand(HCI_cmdCREATENEWUNITKEY);
}
APIDECL1 t_api APIDECL2 HCI_ReadStoredLinkKey(t_bdaddr bdaddr, u_int8 readFlag) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_stored_linkkey command, bdaddress=%s, readFlag=0x%01X",prh_mgr_printBDAddress(bdaddr),readFlag));
    return HCI_GenerateCommand(HCI_cmdREADSTOREDLINKKEY,bdaddr.bytes,readFlag);
}
APIDECL1 t_api APIDECL2 HCI_WriteStoredLinkKey(u_int8 numKeys, t_HCI_WriteLinkKey *links) {
/* This command is an exception as it cannot be generated completely from the table */

    struct host_buf *pdu;
    t_api status;
    u_int8 i;
     
    if (numKeys>0x0B)  /* max number of addr/key pairs which fit in this command */
        return BT_INVALIDPARAM;

    status=prh_hci_GetCommandBuffer((u_int16)(4+1+numKeys*22+HEADERS_FOR_HCITRANSPORT), 0, &pdu);
    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_WriteStoredLinkKey failed to allocate command buffer for %d bytes\n",4+1+numKeys*22+HEADERS_FOR_HCITRANSPORT));
        return BT_NORESOURCES;
    }
    
    pdu->data[0]=HCI_cmdWRITESTOREDLINKKEY&0xFF;
    pdu->data[1]=(HCI_cmdWRITESTOREDLINKKEY>>8)&0xFF;
    pdu->data[2]=1+numKeys*22; /* 22 = 6 bytes for address, 16 for link key */
    
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_stored_link_key command, numKeys=0x%02X",numKeys));

    pdu->data[3]=numKeys;
    for (i=0; i<numKeys; i++) {
        /* encode the address and key fields */
        pMemcpy(pdu->data+4+i*22,links[i].BDAddress.bytes,6);
        pMemcpy(pdu->data+4+i*22+6,links[i].linkKey,16);

        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: ... linkkey[%d]: bdaddress=%s, linkkey=%s", i ,prh_mgr_printBDAddress(links[i].BDAddress), prh_mgr_printLargeNumber(links[i].linkKey,16)));
    }

    pdu->len=HCI_pduCOMMANDHEADER+pdu->data[2];

    status=prh_hci_fc_send_command_pdu(pdu); 

    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_WriteStoredLinkKey failed send command PDU - prh_hci_fc_send_command_pdu() returned error %d\n",status));
        prh_hci_ReleaseCommandBuffer(pdu);
    }

    return status;
}
APIDECL1 t_api APIDECL2 HCI_DeleteStoredLinkKey(t_bdaddr bdaddr, u_int8 deleteFlag) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending delete_stored_linkkey command, bdaddress=%s, deleteFlag=0x%01X",prh_mgr_printBDAddress(bdaddr),deleteFlag));
    return HCI_GenerateCommand(HCI_cmdDELETESTOREDLINKKEY,bdaddr.bytes,deleteFlag);
}
APIDECL1 t_api APIDECL2 HCI_ChangeLocalName(u_int8 *name) {
    /* the name length here is always NUL terminated */
#if pDEBUG
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending change_local_name command, name=\"%s\"",name));
#endif
    return HCI_GenerateCommand(HCI_cmdCHANGELOCALNAME,name);
}
APIDECL1 t_api APIDECL2 HCI_ReadLocalName(void) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_local_name command"));
    return HCI_GenerateCommand(HCI_cmdREADLOCALNAME);
}
APIDECL1 t_api APIDECL2 HCI_ReadConnectionAcceptTimeout(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_connection_accept_timeout command"));
    return HCI_GenerateCommand(HCI_cmdREADCONNECTIONACCEPTTIMEOUT);
}
APIDECL1 t_api APIDECL2 HCI_WriteConnectionAcceptTimeout(u_int16 timeout) {  
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_connection_accept_timeout command, timeout=0x%02X",timeout));
    return HCI_GenerateCommand(HCI_cmdWRITECONNECTIONACCEPTTIMEOUT,timeout);
}
APIDECL1 t_api APIDECL2 HCI_ReadPageTimeout(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_page_timeout command"));
    return HCI_GenerateCommand(HCI_cmdREADPAGETIMEOUT);
}
APIDECL1 t_api APIDECL2 HCI_WritePageTimeout(u_int16 timeout) {  
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_page_timeout command, timeout=0x%02X",timeout));
    return HCI_GenerateCommand(HCI_cmdWRITEPAGETIMEOUT,timeout);
}
APIDECL1 t_api APIDECL2 HCI_ReadScanEnable(void) {      
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_scan_enable command"));
    return HCI_GenerateCommand(HCI_cmdREADSCANENABLE);
}
APIDECL1 t_api APIDECL2 HCI_WriteScanEnable(u_int8 newScanEnable) {    
/* scanEnable and modeFlag scanEnablePending are global variables */
    prh_mgr_scan_enable_pending = newScanEnable;
	prh_mgr_mode_flag |= PRH_MGR_SCANENABLEPENDING_BIT;
	pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_scan_enable command, scanEnable=0x%01X", newScanEnable));
    return HCI_GenerateCommand(HCI_cmdWRITESCANENABLE, newScanEnable);
}
APIDECL1 t_api APIDECL2 HCI_ReadPageScanActivity(void) {    
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_page_scan_activity command"));
    return HCI_GenerateCommand(HCI_cmdREADPAGESCANACTIVITY);
}
APIDECL1 t_api APIDECL2 HCI_WritePageScanActivity(u_int16 interval,u_int16 window) {  
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_page_scan_activity command, interval=0x%04X, window=0x%04X",interval,window));
    return HCI_GenerateCommand(HCI_cmdWRITEPAGESCANACTIVITY,interval,window);
}
APIDECL1 t_api APIDECL2 HCI_ReadInquiryScanActivity(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_inquity_scan_activity command"));
    return HCI_GenerateCommand(HCI_cmdREADINQUIRYSCANACTIVITY);
}
APIDECL1 t_api APIDECL2 HCI_WriteInquiryScanActivity(u_int16 interval, u_int16 window) {    
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_inquity_scan_activity command, interval=0x%02X, window=0x%02X",interval,window));
    return HCI_GenerateCommand(HCI_cmdWRITEINQUIRYSCANACTIVITY,interval,window);
}
APIDECL1 t_api APIDECL2 HCI_ReadAuthenticationEnable(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_authentication_enable command"));
    return HCI_GenerateCommand(HCI_cmdREADAUTHENTICATIONENABLE);
}
APIDECL1 t_api APIDECL2 HCI_WriteAuthenticationEnable(u_int8 authenticationEnable) {   
	
	if(authenticationEnable == 0x00)
        prh_hci_security_mode_pending = MGR_NOSECURITY;   
    else
        prh_hci_security_mode_pending = MGR_LINKLEVEL; 
	
	pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_authentication_enable command, authenticationEnable=0x%01X",authenticationEnable));
    return HCI_GenerateCommand(HCI_cmdWRITEAUTHENTICATIONENABLE,authenticationEnable);
}
APIDECL1 t_api APIDECL2 HCI_ReadEncryptionMode(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_encryption_mode command"));
    return HCI_GenerateCommand(HCI_cmdREADENCRYPTIONMODE);
}
APIDECL1 t_api APIDECL2 HCI_WriteEncryptionMode(u_int8 encryptionMode) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_encryption_mode command, encryptionMode=0x%02X",encryptionMode));
    return HCI_GenerateCommand(HCI_cmdWRITEENCRYPTIONMODE,encryptionMode);
}
APIDECL1 t_api APIDECL2 HCI_ReadClassOfDevice(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_class_of_device command"));
    return HCI_GenerateCommand(HCI_cmdREADCLASSOFDEVICE);
}
APIDECL1 t_api APIDECL2 HCI_WriteClassOfDevice(u_int32 cod) {   

    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_class_of_device command, cod=0x%04X",cod));
    return HCI_GenerateCommand(HCI_cmdWRITECLASSOFDEVICE, cod);

}
APIDECL1 t_api APIDECL2 HCI_ReadVoiceSetting(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_voice_setting command"));
    return HCI_GenerateCommand(HCI_cmdREADVOICESETTING);
}
APIDECL1 t_api APIDECL2 HCI_WriteVoiceSetting(u_int16 voiceSetting) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_voice_setting command, voiceSetting=0x%02X",voiceSetting));
    return HCI_GenerateCommand(HCI_cmdWRITEVOICESETTING,voiceSetting);
}
APIDECL1 t_api APIDECL2 HCI_ReadAutomaticFlushTimeout(u_int16 handle) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_automatic_flush_timeout command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdREADAUTOMATICFLUSHTIMEOUT,handle);
}
APIDECL1 t_api APIDECL2 HCI_WriteAutomaticFlushTimeout(u_int16 handle, u_int16 timeout) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_automatic_flush_timeout command, handle=0x%02X, timeout=0x%02X",handle,timeout));
    return HCI_GenerateCommand(HCI_cmdWRITEAUTOMATICFLUSHTIMEOUT,handle,timeout);
}
APIDECL1 t_api APIDECL2 HCI_ReadNumBroadcastRetransmissions(void) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_num_broadcast_transmissions command"));
    return HCI_GenerateCommand(HCI_cmdREADNUMBROADCASTRETRANSMISSIONS);
}
APIDECL1 t_api APIDECL2 HCI_WriteNumBroadcastRetransmissions(u_int8 number) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_num_broadcast_transmissions command, number=0x%01X",number));
    return HCI_GenerateCommand(HCI_cmdWRITENUMBROADCASTRETRANSMISSIONS,number);
}
APIDECL1 t_api APIDECL2 HCI_ReadHoldModeActivity(void) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_hold_mode_activity command"));
    return HCI_GenerateCommand(HCI_cmdREADHOLDMODEACTIVITY);
}
APIDECL1 t_api APIDECL2 HCI_WriteHoldModeActivity(u_int8 activity) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_hold_mode_activity command, number=0x%01X",activity));
    return HCI_GenerateCommand(HCI_cmdWRITEHOLDMODEACTIVITY,activity);
}
APIDECL1 t_api APIDECL2 HCI_ReadTransmitPowerLevel(u_int16 handle, u_int8 type) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_transmit_power_level command, handle=0x%02X, type=0x%X",handle,type));
    return HCI_GenerateCommand(HCI_cmdREADTRANSMITPOWERLEVEL, handle, type);
}
APIDECL1 t_api APIDECL2 HCI_ReadSCOFlowControlEnable(void) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_sc_flow_control_enable command"));
    return HCI_GenerateCommand(HCI_cmdREADSCOFLOWCONTROLENABLE);
}
APIDECL1 t_api APIDECL2 HCI_WriteSCOFlowControlEnable(u_int8 enable) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_sco_fc_enable command, number=0x%01X",enable));
    return HCI_GenerateCommand(HCI_cmdWRITESCOFLOWCONTROLENABLE,enable);
}
APIDECL1 t_api APIDECL2 HCI_SetHostControllerToHostFlowControl(u_int8 enable) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending host_controller_to_host_fc command, mode=0x%01X",enable));
    return HCI_GenerateCommand(HCI_cmdSETHOSTCONTROLLERTOHOSTFLOWCONTROL,enable);
}
APIDECL1 t_api APIDECL2 HCI_HostBufferSize(u_int16 ACLlength, u_int8 SCOlength, u_int16 totalACLpackets, u_int16 totalSCOpackets) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending host_buffer_size command, ACLlength=0x%02X, SCOlength=0x%01X, totalACLpackets=0x%02x, totalSCOpackets=0x%02x",ACLlength,SCOlength,totalACLpackets,totalSCOpackets));
    return HCI_GenerateCommand(HCI_cmdHOSTBUFFERSIZE,ACLlength,SCOlength,totalACLpackets,totalSCOpackets);
}
APIDECL1 t_api APIDECL2 HCI_HostNumberOfCompletedPackets(u_int8 numHandles, t_HCI_NumPackets *numPackets) {
    /* This command is an exception as it cannot be generated completely from the table */

    struct host_buf *pdu;
    t_api status;
    u_int8 i;

    if (numHandles>63) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_HostNumberOfCompletedPackets: PDU size to encode 0x%01X handles is too large -  returning BT_INVALIDPARAM\n",numHandles));
        return BT_INVALIDPARAM;
    }
    
    /* 4=cmd header len, 1=num handles, (numHandles<<2)=4 bytes per handle entry */
    status=prh_hci_GetCommandBuffer((u_int16)(4+1+(numHandles<<2)+HEADERS_FOR_HCITRANSPORT), 0, &pdu);
    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_HostNumberOfCompletedPackets failed to allocate command buffer for %d bytes\n",4+1+(numHandles<<2)+HEADERS_FOR_HCITRANSPORT));
        return BT_NORESOURCES;
    }

    pdu->data[0]=HCI_cmdHOSTNUMBEROFCOMPLETEDPACKETS&0xFF;
    pdu->data[1]=(HCI_cmdHOSTNUMBEROFCOMPLETEDPACKETS>>8)&0xFF;
    pdu->data[2]=1+(numHandles<<2);
    
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending host_num_completed_packets command, numHandles=0x%02X",numHandles));

    pdu->data[3]=numHandles;
    for (i=0; i<numHandles; i++) {
        pdu->data[4+(i<<2)]=numPackets[i].Handle&0xFF;
        pdu->data[4+(i<<2)+1]=(numPackets[i].Handle&0xFF00)>>8;
        pdu->data[4+(i<<2)+2]=numPackets[i].NumCompleted&0xFF;
        pdu->data[4+(i<<2)+3]=(numPackets[i].NumCompleted&0xFF00)>>8;

        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: ... 0x%02X packets on handle 0x%02X",numPackets[i].NumCompleted,numPackets[i].Handle));
    }

    pdu->len=HCI_pduCOMMANDHEADER+pdu->data[2];

    status=prh_hci_fc_send_command_pdu(pdu); 

    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_HostNumberOfCompletedPackets failed send command PDU - prh_hci_fc_send_command_pdu() returned error %d\n",status));
        prh_hci_ReleaseCommandBuffer(pdu);
    }

    return status;
}
APIDECL1 t_api APIDECL2 HCI_ReadLinkSupervisionTimeout(u_int16 handle) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_link_supervision_timeout command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdREADLINKSUPERVISIONTIMEOUT,handle);
}
APIDECL1 t_api APIDECL2 HCI_WriteLinkSupervisionTimeout(u_int16 handle, u_int16 timeout) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_link_supervision_timeout command, handle=0x%02X, timeout=0x%02X",handle,timeout));
    return HCI_GenerateCommand(HCI_cmdWRITELINKSUPERVISIONTIMEOUT,handle,timeout);
}
APIDECL1 t_api APIDECL2 HCI_ReadNumberOfSupportedIAC(void) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_number_of_supported_iac command"));
    return HCI_GenerateCommand(HCI_cmdREADNUMBEROFSUPPORTEDIAC);
}
APIDECL1 t_api APIDECL2 HCI_ReadCurrentIACLAP(void) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_current_iac_lap command"));
    return HCI_GenerateCommand(HCI_cmdREADCURRENTIACLAP);
}
APIDECL1 t_api APIDECL2 HCI_WriteCurrentIACLAP(u_int8 numLap, u_int32 *lap) {
    /* This command is an exception as it cannot be generated completely from the table */

    struct host_buf *pdu;
    t_api status;
    u_int8 i;

    if (numLap>63) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_WriteCurrentIACLAP: PDU size to encode 0x%01X LAPs is too large -  returning BT_INVALIDPARAM\n",numLap));
        return BT_INVALIDPARAM;
    }
    
    /* 4=cmd header len, 1=num LAPs, numLap*3)=3 bytes per LAP entry */
    status=prh_hci_GetCommandBuffer((u_int16)(4+1+numLap*3+HEADERS_FOR_HCITRANSPORT), 0, &pdu);
    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_WriteCurrentIACLAP failed to allocate command buffer for %d bytes\n",4+1+numLap*3+HEADERS_FOR_HCITRANSPORT));
        return BT_NORESOURCES;
    }
    pdu->data[0]=HCI_cmdWRITECURRENTIACLAP&0xFF;
    pdu->data[1]=(HCI_cmdWRITECURRENTIACLAP>>8)&0xFF;
    pdu->data[2]=1+numLap*3; /* parameter length = count field + 3 bytes per LAP */

    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_current_iac_lap command, numLap=0x%01X",numLap));

	prh_mgr_mode_flag |= PRH_MGR_IACLAPPENDING_BIT;
	prh_mgr_liac_present_pending = ((numLap > 1) && (!pMemcmp(lap, PRH_GIAC_PLUS_LIAC, 6)));
		

    pdu->data[3]=numLap;
    for (i=0; i<numLap; i++) {

        /* encode the lap fields */
        pdu->data[4+i*3]=lap[i]&0xFF;
        pdu->data[4+i*3+1]=(lap[i]&0xFF00)>>8;
        pdu->data[4+i*3+2]=(lap[i]&0xFF0000)>>16;

        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: ... lap[0x%01X]=0x%03X",i,lap[i]));
    }

    pdu->len=HCI_pduCOMMANDHEADER+pdu->data[2];

    status=prh_hci_fc_send_command_pdu(pdu); 

    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_WriteCurrentIACLAP failed to queue Command PDU for sending - HCI_Enqueue() returned error %d\n",status));
        prh_hci_ReleaseCommandBuffer(pdu);
    }

    return status;
}
APIDECL1 t_api APIDECL2 HCI_ReadPageScanPeriodMode(void) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_page_scan_period_mode command"));
    return HCI_GenerateCommand(HCI_cmdREADPAGESCANPERIODMODE);
}
APIDECL1 t_api APIDECL2 HCI_WritePageScanPeriodMode(u_int8 mode) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_page_scan_period_mode command, mode=0x%01X",mode));
    return HCI_GenerateCommand(HCI_cmdWRITEPAGESCANPERIODMODE,mode);
}
APIDECL1 t_api APIDECL2 HCI_ReadPageScanMode(void) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_page_scan_mode command"));
    return HCI_GenerateCommand(HCI_cmdREADPAGESCANMODE);
}
APIDECL1 t_api APIDECL2 HCI_WritePageScanMode(u_int8 mode) {
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_page_scan_mode command, mode=0x%01X",mode));
    return HCI_GenerateCommand(HCI_cmdWRITEPAGESCANMODE,mode);
}


/*
    HCI informational commands - Opcode Group Field = 0x04
*/

APIDECL1 t_api APIDECL2 HCI_ReadLocalVersionInformation(void) {  
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_local_version_information command"));
    return HCI_GenerateCommand(HCI_cmdREADLOCALVERSIONINFORMATION);
}

APIDECL1 t_api APIDECL2 HCI_ReadLocalSupportedFeatures(void) { 
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_local_features command"));
    return HCI_GenerateCommand(HCI_cmdREADLOCALSUPPORTEDFEATURES);
}
APIDECL1 t_api APIDECL2 HCI_ReadBufferSize(void) {      
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_buffer_size command"));
    return HCI_GenerateCommand(HCI_cmdREADBUFFERSIZE);
}
APIDECL1 t_api APIDECL2 HCI_ReadCountryCode(void) {     
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_country_code command"));
    return HCI_GenerateCommand(HCI_cmdREADCOUNTRYCODE);
}
APIDECL1 t_api APIDECL2 HCI_ReadBDAddress(void) {   
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_bdaddress command"));
    return HCI_GenerateCommand(HCI_cmdREADBDADDR);
}


/*
    HCI status commands - Opcode Group Field = 0x05
*/

APIDECL1 t_api APIDECL2 HCI_ReadFailedContactCounter(u_int16 handle) {      
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_failed_contact_counter command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdREADFAILEDCONTACTCOUNTER,handle);
}
APIDECL1 t_api APIDECL2 HCI_ResetFailedContactCounter(u_int16 handle) {     
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending reset_failed_contact_counter command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdRESETFAILEDCONTACTCOUNTER,handle);
}
APIDECL1 t_api APIDECL2 HCI_GetLinkQuality(u_int16 handle) {    
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending get_link_quality command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdGETLINKQUALITY,handle);
}
APIDECL1 t_api APIDECL2 HCI_ReadRSSI(u_int16 handle) {      
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_rssi command, handle=0x%02X",handle));
    return HCI_GenerateCommand(HCI_cmdREADRSSI,handle);
}


/*
    HCI test commands - Opcode Group Field = 0x06
*/

APIDECL1 t_api APIDECL2 HCI_ReadLoopbackMode(void) {    
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending read_loopback_mode command"));
    return HCI_GenerateCommand(HCI_cmdREADLOOPBACKMODE);
}
APIDECL1 t_api APIDECL2 HCI_WriteLoopbackMode(u_int8 mode) {    
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending write_loopback_mode command, mode=0x%01X",mode));
    return HCI_GenerateCommand(HCI_cmdWRITELOOPBACKMODE,mode);
}
APIDECL1 t_api APIDECL2 HCI_EnableDUT(void) { 
    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Sending enable_dut command"));
    return HCI_GenerateCommand(HCI_cmdENABLEDUT);
}




