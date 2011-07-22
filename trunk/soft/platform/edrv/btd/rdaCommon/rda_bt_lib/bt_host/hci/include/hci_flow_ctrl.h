/*
 * MODULE NAME:    hci_flow_ctrl.h
 * DESCRIPTION:    HCI Flow Control Header (Host to Host Controller) 
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_flow_ctrl.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef prh_HCI_FLOWCTRL_H
#	define prh_HCI_FLOWCTRL_H

struct prh_st_t_hci_queue {
	struct host_buf *head, *tail;
	struct prh_st_t_hci_queue *next;
	u_int16 handle;
	u_int16 numCanSend;
	u_int16 packetsInHC;
	u_int8 linkType;
	u_int8 active;
    u_int32 allocatedBytes;
    u_int32 reservedBytes;
#if PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED==1
    u_int32 numConsumedPackets; /* This is in the Rx direction for this handle */
#endif

};

t_api prh_hci_fc_init(void); 
t_api prh_hci_fc_shutdown(void);

t_api prh_hci_fc_send_command_pdu(struct host_buf *pdu);
t_api prh_hci_fc_send_data_pdu(u_int8 type, u_int16 handle, struct host_buf *pdu);


void prh_hci_fc_num_completed_packets(u_int8 *pdu);
void prh_hci_fc_read_buf_size_event(u_int16 maxACLs, u_int16 maxSCOs);
void prh_hci_fc_disconnection_complete(u_int16 handle);
void  prh_hci_fc_connection_complete(u_int8 eventType, u_int16 handle);
void prh_hci_fc_num_command_packets(u_int8 numCommandPackets);
void prh_hci_fc_rebalance_queues(void);
void prh_hci_fc_modechange(u_int16 handle, u_int8 mode);
void prh_hci_fc_loopback_command(void); 
t_api prh_hci_fc_enqueue_child_list(u_int16 handle, struct host_buf *bufferParent, u_int32 totalBytes);
struct prh_st_t_hci_queue *prh_hci_CreateBroadcastQueue(u_int16 handle);
t_api prh_hci_GetCommandBuffer(u_int16 length, u_int32 flags, struct host_buf **p_buf);
struct prh_st_t_hci_queue *prh_hci_create_queue(u_int16 handle, u_int8 type);
t_api prh_hci_fc_process_outgoing(u_int32 *maxNumToProcess);
t_api prh_hci_fc_process_incoming(u_int32 *maxNumToProcess);

#endif /* prh_HCI_FLOWCTRL_H */

