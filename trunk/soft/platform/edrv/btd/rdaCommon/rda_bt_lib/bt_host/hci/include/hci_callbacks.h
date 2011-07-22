/*
 * MODULE NAME:    hci_callbacks.h
 * DESCRIPTION:    HCI Callback Declarations
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_callbacks.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *

 */

#ifndef prh_HCI_CALLBACKS_H
#    define prh_HCI_CALLBACKS_H


#include "host_config.h"
#include "host_types.h"
#include "hci_transport.h"
#include "papi.h"

/* Higher layer callback functions */
extern t_api (*prh_hci_manager_event_callback_func)(u_int8 *);
extern t_api (*prh_hci_l2cap_event_callback_func)(u_int8 *);
extern t_api (*prh_hci_l2cap_acl_callback_func)(u_int16 handle, struct host_buf *pdu, u_int8 flags);
extern void (*prh_hci_app_event_callback_func[MAX_OUTSTANDING_CALLBACKS])(u_int8 *event);
extern void (*prh_hci_app_global_rx_callback_func[MAX_OUTSTANDING_CALLBACKS])(u_int8 type, u_int8 *pdu, u_int32 len);

t_api prh_hci_recv_callback(struct host_buf *pdu, u_int8 type);


#endif /* prh_HCI_CALLBACKS_H */

