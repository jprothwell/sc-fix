/*
 * MODULE NAME:    hci_debug.h
 * DESCRIPTION:    HCI Debugging-only Functions
 * AUTHOR:         Martin Mellody
 * DATE:           20 March 2000
 *
 * SOURCE CONTROL: $Id: hci_debug.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef prh_HCI_INTERNALS_H
#    define prh_HCI_INTERNALS_H

#include "host_config.h"



#include "host_types.h"
#include "hci_primitives.h"
#include "hci_transport.h"




char *prh_hci_hciOpcode2string(u_int16 opcode);
char *prh_hci_eventCode2string(u_int16 eventCode);
char *prh_hci_eventStatus2string(u_int16 status);
char *prh_hci_lmpOpcode2string(u_int16 opcode);
char *prh_hci_linkPolicyMode2string(u_int16 mode);

u_int8 *prh_hci_debug_print_large_number(u_int8 *number, u_int8 length);
void prh_hci_debug_print_event(struct host_buf *rxbuf);
void prh_hci_debug_print_command_complete_event(u_int8 *pdu);


#endif

