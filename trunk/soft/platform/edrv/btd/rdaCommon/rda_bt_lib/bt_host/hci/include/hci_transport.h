/*
 * MODULE NAME:    hci_transport.h
 * DESCRIPTION:    HCI Transport Driver Interface Header
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_transport.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef prh_HCI_TRANSPORT_H
#    define prh_HCI_TRANSPORT_H

#include "host_config.h"
#include "host_types.h"

/* we need a forward declaration here for host_buf */
struct host_buf;
/* 
	the OS specific stuff is in hci_transport_impl. 
	The build mechanism on each OS must make sure 
	the correct one of these is found.
*/
#include "hci_transport_impl.h"


/* set up lengths for host_buf.h */
#define HEADERS_FOR_HCITRANSPORT	1  /* pdu type */

#define PRH_HCI_HEADER_SIZE 			HEADERS_FOR_HCITRANSPORT + 4 /* max header=ACL pdu header of 4 bytes */			
#define HEADERS_FOR_HCI		PRH_HCI_HEADER_SIZE

/*	HCI Transport Driver PDU types */
#define HCI_pduCOMMAND			0x01
#define HCI_pduACLDATA			0x02
#define HCI_pduSCODATA			0x03
#define HCI_pduEVENT			0x04


/* pdu header lengths */
#define HCI_pduCOMMANDHEADER	0x03
#define HCI_pduACLDATAHEADER	0x04
#define HCI_pduSCODATAHEADER	0x03
#define HCI_pduEVENTHEADER		0x02


/* interface to platform independent HCI Transport */
t_api HCI_Transport_Init(void *platformConfig);
t_api HCI_Transport_Shutdown(void);
/* The driver must not fail if the callback is set to NULL - just drop the packets */
void HCI_Transport_Set_Rx_CB(t_api (*callbackFunc)(struct host_buf *pdu, u_int8 type));
void HCI_Transport_Rx_Data(u_int8 *pdu, u_int32 length);
t_api HCI_Transport_PDU_Tx(struct host_buf *pdu, u_int8 type);
t_api HCI_Transport_Free_Buffer(struct host_buf *p_buf, u_int8 type);

#endif /* prh_HCI_TRANSPORT_H */

