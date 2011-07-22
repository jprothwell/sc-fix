/*
 * MODULE NAME:    hci_callbacks.c
 * DESCRIPTION:    HCI Callback Functions From HCI Transport Layer
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_callbacks.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */


/*
    HCIT Host Controller to Host flow control should be implemented here, when supported.
*/

#include "host_config.h"
#include "hci_callbacks.h"
#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"
#include "hci_constants.h"
#include "hci_debug.h"
#include "papi.h"
#include "mgr_utils.h"
#include "hci_primitives.h"
#include "hci_flow_ctrl.h"
#include "mgr_core.h"


/* Higher layer callback functions */
t_api (*prh_hci_manager_event_callback_func)(u_int8 *);
t_api (*prh_hci_l2cap_event_callback_func)(u_int8 *);
t_api (*prh_hci_l2cap_acl_callback_func)(u_int16 handle, struct host_buf *pdu, u_int8 flags);
void (*prh_hci_app_event_callback_func[MAX_OUTSTANDING_CALLBACKS])(u_int8 *event);
void (*prh_hci_app_global_rx_callback_func[MAX_OUTSTANDING_CALLBACKS])(u_int8 type, u_int8 *pdu, u_int32 len);

volatile u_int8 prh_host_reset_required;
volatile extern t_api prh_host_startup_complete;
volatile extern u_int8 prh_hci_expect_reset;
volatile u_int8 wait_for_flow_control_enable=0;

/*************************************************************************************************
 * prh_hci_recv_callback
 *
 * Called by the transport driver when a PDU is recieved. This callback is set up by prh_hci_init.
 ************************************************************************************************/
t_api prh_hci_recv_callback(struct host_buf *pdu, u_int8 type) {

    u_int8 flags;
    u_int16 handle;
    t_api retVal=BT_NOERROR;
    u_int8 i;
#if PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED==1 && PRH_HOST_HC_TO_HOST_FLOW_CONTROL_ACL_REPORTING_THRESHOLD>0
    static u_int32 consumedPacketCount=0; 
#endif

//pDebugPrintfEX((pLOGERROR,pLOGHCI,"prh_hci_recv_callback type=0x%x, data[0,1]=0x%x,0x%x\n, ",type,pdu->data[0], pdu->data[1]));

    /* First pass the pdu to the application 'sniffer' handler, if registered */
    for (i=0; i<MAX_OUTSTANDING_CALLBACKS; i++) 
        if (prh_hci_app_global_rx_callback_func[i]) 
            prh_hci_app_global_rx_callback_func[i](type, pdu->data, pdu->len); /* this does not free the pdu */
    
    switch (type) {
        case HCI_pduACLDATA:
#if PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED==1
            {
                extern struct prh_st_t_hci_queue *prh_hci_acl_queue_list_head;
                extern t_pMutex prh_hci_fc_acl_queue_lock;
                extern struct prh_st_t_hci_queue *prh_hci_fc_find_data_queue(struct prh_st_t_hci_queue *queueListHead, u_int16 handle_t);
                struct prh_st_t_hci_queue *current;
                handle=pdu->data[0]|((pdu->data[1]&0xF)<<8);
                pMutexLock(prh_hci_fc_acl_queue_lock);
                current=prh_hci_fc_find_data_queue(prh_hci_acl_queue_list_head,handle);
                if (current) {
                    current->numConsumedPackets++;
                }
#if pDEBUG
                else {
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: Cannot find data queue for handle=0x%02X\n",handle));
                }
#endif
                pMutexUnlock(prh_hci_fc_acl_queue_lock);
            }
#if PRH_HOST_HC_TO_HOST_FLOW_CONTROL_ACL_REPORTING_THRESHOLD>0
            consumedPacketCount++;
            if (consumedPacketCount>=PRH_HOST_HC_TO_HOST_FLOW_CONTROL_ACL_REPORTING_THRESHOLD) {
                void prh_hci_report_host_completed_packets(void);
                prh_hci_report_host_completed_packets();
                consumedPacketCount=0;
            }
#endif
#endif
            if (prh_hci_l2cap_acl_callback_func) {
                /* decode ACL header fields */
                handle=pdu->data[0]|((pdu->data[1]&0xF)<<8);

                flags=0;
                if((pdu->data[1]>>4)&0x01)
                    flags|=prh_hci_ACL_CONTINUE_FRAGMENT;
                else
                    flags|=prh_hci_ACL_START_FRAGMENT;

                if((pdu->data[1]>>6)&0x01)
                    flags|=prh_hci_ACL_ACTIVE_BROADCAST;  
                
                if((pdu->data[1]>>7)&0x01)
                    flags|=prh_hci_ACL_PICONET_BROADCAST;

#if 0//pDEBUG 
                {
                    char *pb_string, *bf_string;
                    pb_string=bf_string="INVALID";
                    if(((pdu->data[1]>>4)&0x03)==0x01)
                        pb_string="Continue";
                    if(((pdu->data[1]>>4)&0x03)==0x02)
                        pb_string="Start";
                    if(((pdu->data[1]>>6)&0x03)==0x00)
                        bf_string="None";
                    if(((pdu->data[1]>>6)&0x03)==0x01)
                        bf_string="Active";
                    if(((pdu->data[1]>>6)&0x03)==0x02)
                        bf_string="Piconet";

                    if (pb_string[0]=='I' || bf_string[0]=='I') /* a field was invalid */
                        pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: ACL Data PDU handle=0x%02X, length=0x%02X, segment=%s, broadcast=%s\n",pdu->data[0]|((pdu->data[1]&0x0F)<<8),pdu->data[2]|(pdu->data[3]<<8),pb_string,bf_string));
                    else
                        pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI Rx: ACL Data PDU handle=0x%02X, length=0x%02X, segment=%s, broadcast=%s\n",pdu->data[0]|((pdu->data[1]&0x0F)<<8),pdu->data[2]|(pdu->data[3]<<8),pb_string,bf_string));
                }
#endif
                /* remove ACL header */
                host_buf_reserve_header(pdu,4);

	        //pDebugPrintfEX((pLOGDEBUG, pLOGHCI, "prh_hci_recv_callback: after call to host_buf_reserve_header\n"));

                /* pass to L2CAP - it will free the pdu*/
                retVal = prh_hci_l2cap_acl_callback_func(handle, pdu, flags);
            } else {
                /* if L2CAP is present, it frees the pdu but now free here instead */
                host_buf_free(pdu);
            }

            break;

        case HCI_pduSCODATA:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: SCO Data PDU handle=0x%02X, length=0x%02X\n",pdu->data[0]|((pdu->data[1]&0x0F)<<8),pdu->data[2]));
                        
            host_buf_free(pdu);
            break;


        case HCI_pduEVENT:

#if pDEBUG
           // pDebugPrintfEX((pLOGERROR,pLOGHCI,"prh_hci_recv_callback pdu->data[0]=0x%02X\n",pdu->data[0]));
            prh_hci_debug_print_event(pdu); 
#endif

            /*  dispatch events which are used internally in the HCI */


            switch (pdu->data[0]) {
                case HCI_evCONNECTIONCOMPLETE:
#if pDEBUG
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"prh_hci_recv_callback CONNECTIONCOMPLETE status=0x%0X\n",!pdu->data[2]));
#endif
                    if (!pdu->data[2]) /* status == ok */
                        prh_hci_fc_connection_complete(pdu->data[11], BT_READ_LE_UINT16(pdu->data+3)); /* linktype, handle */
                    break;

                case HCI_evDISCONNECTIONCOMPLETE:
#if pDEBUG
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"prh_hci_recv_callback HCI_evDISCONNECTIONCOMPLETE status=0x%0X\n",!pdu->data[2]));
#endif
                    if (!pdu->data[2]) 
                    { /* status == ok */
#if PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED==1
                        {
                            extern struct prh_st_t_hci_queue *prh_hci_acl_queue_list_head;
                            extern t_pMutex prh_hci_fc_acl_queue_lock;
                            extern struct prh_st_t_hci_queue *prh_hci_fc_find_data_queue(struct prh_st_t_hci_queue *queueListHead, u_int16 handle_t);
                            struct prh_st_t_hci_queue *current;
                            handle=BT_READ_LE_UINT16(pdu->data+3);
                            pMutexLock(prh_hci_fc_acl_queue_lock);
                            current=prh_hci_fc_find_data_queue(prh_hci_acl_queue_list_head,handle);
                            if (current) {
                                consumedPacketCount-=current->numConsumedPackets;
                            }
                            pMutexUnlock(prh_hci_fc_acl_queue_lock);
                        }
#endif
                        prh_hci_fc_disconnection_complete(BT_READ_LE_UINT16(pdu->data+3)); /* handle */
                    }
                    break;

                case HCI_evLOOPBACKCOMMAND:
                    prh_hci_fc_loopback_command(); 
                    break;

               case HCI_evMODECHANGE:
                    if (!pdu->data[2]) {
                        prh_hci_fc_modechange(BT_READ_LE_UINT16(pdu->data+3),pdu->data[5]);
                    }
                    break;

                case HCI_evNUMCOMPLETEDPACKETS:
                    prh_hci_fc_num_completed_packets(pdu->data); 
                    break;

                case HCI_evCOMMANDSTATUS:
                    prh_hci_fc_num_command_packets(pdu->data[3]); 
                    break;

                case HCI_evHARDWAREERROR:
				    pDebugPrintfEX((pLOGERROR,pLOGSYSTEM,"Got HARDWARE ERROR event from Host Controller!!!\n"));
#if HCI_HWERROR_HANDLING==1 
                    prh_host_reset_required=1; /* this flag is checked in the scheduler */
#endif
                    break;

                case HCI_evCOMMANDCOMPLETE:
                    prh_hci_fc_num_command_packets(pdu->data[2]);

                    switch (BT_READ_LE_UINT16(pdu->data+3)) {

#if PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED==1
                        case HCI_cmdSETHOSTCONTROLLERTOHOSTFLOWCONTROL:
                            if (!pdu->data[5]) {
                                if (wait_for_flow_control_enable) {
                                    extern u_int8 prh_hci_fc_hc_to_host_fc_enabled_acl;
                                    wait_for_flow_control_enable=0;
                                    /* the bootup command sequence is still running */
                                    prh_hci_fc_hc_to_host_fc_enabled_acl=1;
                                    pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: HCI HC->Host Flow Control enabled\n"));
                                }
                            } else {
                                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: HCI Returned error 0x%02X (%s) for attempt to enable HC->Host flow control - data transfer may be comprimised\n",pdu->data[5],prh_hci_eventStatus2string(pdu->data[5])));
                            }

                            break;
#endif

                        case HCI_cmdREADBUFFERSIZE:
                            pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: HC Buffer sizes: maxACLnum=0x%02X, maxSCOnum=0x%02X, maxACLsize=0x%02X, maxSCOsize=0x%02X\n",BT_READ_LE_UINT16(pdu->data+9), BT_READ_LE_UINT16(pdu->data+11), BT_READ_LE_UINT16(pdu->data+6), pdu->data[8]));
                            prh_hci_fc_read_buf_size_event(BT_READ_LE_UINT16(pdu->data+9), BT_READ_LE_UINT16(pdu->data+11)); /* numACL, numSCO */
				            pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"*** stack initialisation complete ***\n"));                            
							prh_host_startup_complete=BT_NOERROR;
                            break;

                        case HCI_cmdREADLOCALVERSIONINFORMATION:                            
                            pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: Host Controller manufacturer ID 0x%04X\n",BT_READ_LE_UINT16(pdu->data+10)));                            
#if PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED==1
                            if (prh_host_startup_complete==BT_INVALIDERRORCODE) {
                                /* The bootup sequence is still running */
                                /* Do not enable HC->Host FC for Parthus revision 0x701 to workaround a HC bug */
                                if (BT_READ_LE_UINT16(pdu->data+10)!=PARTHUS_MANUFACTURERID || BT_READ_LE_UINT16(pdu->data+7)!=0x701) {
                                    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"Enabling HC->Host flow control\n"));                            
                                    wait_for_flow_control_enable=1;
                                    HCI_SetHostControllerToHostFlowControl(0x01 /* ACL only */); 
                                } else {
                                    pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"Detected Parthus HC revision 0x701 - Not enabling HC->Host flow control\n"));                            
                                }
                            }
#endif

#if pDEBUG && QUERY_PARTHUS_EXTENDED_INFO
                            if (BT_READ_LE_UINT16(pdu->data+10)==PARTHUS_MANUFACTURERID) {
                                pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"HCI: Identified a Parthus host controller - attempting proprietary TCI commands\n"));
				                {
                                    /* Send some proprietary HCI (TCI) commands to read HC properties */
                                    struct st_t_dataBuf *writeBuffer;
                                    t_api status_s=0;   

                                    /* Send Read_Hardware_Version */
                                    status_s=HCI_GetWriteBuffer(HCI_NO_HANDLE,3,&writeBuffer);                                   
                                    if (status_s)
                                        break;
                                    writeBuffer->buf[0]=0x06;
                                    writeBuffer->buf[1]=0xFC;
                                    writeBuffer->buf[2]=0x00;
                                    writeBuffer->dataLen=3;
                                    status_s=HCI_SendRawPDU(HCI_pduCOMMAND,writeBuffer);
                                    if (status_s) {
                                        HCI_FreeWriteBuffer(writeBuffer);
                                        break;
                                    }
                                    /* Send Read_Extended_Features */
                                    status_s=HCI_GetWriteBuffer(HCI_NO_HANDLE,3,&writeBuffer);                                   
                                    if (status_s)
                                        break;
                                    writeBuffer->buf[0]=0x1D;
                                    writeBuffer->buf[1]=0xFC;
                                    writeBuffer->buf[2]=0x00;
                                    writeBuffer->dataLen=3;
                                    status_s=HCI_SendRawPDU(HCI_pduCOMMAND,writeBuffer);
                                    if (status_s) 
                                     {
                                        HCI_FreeWriteBuffer(writeBuffer);
                                        break;
                                    }
				                }
                            } 
#endif /* pDEBUG && QUERY_PARTHUS_EXTENDED_INFO */
                            break;

                        case HCI_cmdRESET:
                            wait_for_flow_control_enable=0;
#if HCI_HWRESET_HANDLING
                            {
                                extern u_int8 prh_hci_reset_in_progress;
                                prh_hci_reset_in_progress=0; 
                            }    
							if (!prh_hci_expect_reset) 
								prh_host_reset_required=1; /* this flag is checked in the scheduler */
                            else
                                prh_hci_expect_reset=0;

#endif /* HCI_HWRESET_HANDLING */
                            break;
                    }

                    break;
            }

            /* Now deliver the event to the higher layers */


            /* 
                These internal L2CAP and Manager event functions 
                are exceptions - they do not free the pdu 
            */
            if (prh_hci_l2cap_event_callback_func) 
                prh_hci_l2cap_event_callback_func(pdu->data);

            if (prh_hci_manager_event_callback_func) 
                prh_hci_manager_event_callback_func(pdu->data);

            /* 
                now call the application event callback, if registered.
            */
            for (i=0; i<MAX_OUTSTANDING_CALLBACKS; i++) 
                if (prh_hci_app_event_callback_func[i]) 
                    prh_hci_app_event_callback_func[i](pdu->data);  /* this does not free the pdu */

            /* now we can free the pdu */
            host_buf_free(pdu);
            break;



        default:
            /* this can't normally happen because the transport driver has already recieved the pdu ok */
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"prh_hci_recv_callback received unknown PDU type %d, len %d from transport driver\n",type, pdu->len));

            /* free the pdu */
            host_buf_free(pdu);

            retVal=BT_UNKNOWNERROR; 
    }

    return retVal;
}





