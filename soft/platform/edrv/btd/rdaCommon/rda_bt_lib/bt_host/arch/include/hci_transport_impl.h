/***********************************************************************
 *
 * MODULE NAME:    hci_transport_impl.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    HCI Transport for eCos Serial port
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: hci_transport_impl.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    1.May.2000 - JS - first import
 *
 *
 * ISSUES:
 *       
 ***********************************************************************/
#ifndef HCI_TRANSPORT_IMPL_H
#define HCI_TRANSPORT_IMPL_H

#include "host_config.h"

#ifndef HCI_BAUDRATE
#define HCI_BAUDRATE CYGNUM_SERIAL_BAUD_38400
#endif

#define RS232_ERROR_MSG_LEN   4
#define RS232_NEGOT_MSG_LEN   8

/*  HCI RS232 Packet Types */

#define RS232_HCI_COMMAND 0x1
#define RS232_HCI_ACLDATA 0x2
#define RS232_HCI_SCODATA 0x3
#define RS232_HCI_EVENT   0x4
#define RS232_ERROR_MSG   0x5
#define RS232_NEGOT_MSG   0x6

#endif
