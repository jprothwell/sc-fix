/***********************************************************************
 *
 * MODULE NAME:    host_error.h
 * DESCRIPTION:    Error codes for Host Side Stack
 * MAINTAINER:     Dave Airlie
 *
 * SOURCE CONTROL: $Id: host_error.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies.
 *     All rights reserved.
 *
 ***********************************************************************/


#ifndef PRH_HOST_ERROR_H
#define PRH_HOST_ERROR_H

#include "host_config.h"

#define BT_NOERROR			    0x00 
#define BT_PENDING			    0x01 
#define BT_INVALIDPARAM		    0x02 
#define BT_SECFAIL			    0x03 
#define BT_PINFAIL			    0x04 
#define BT_FLOWCTRL			    0x05
#define BT_NORESOURCES		    0x06 
#define BT_UNSUPPORTED		    0x07 
#define BT_HCITERROR		    0x08
#define BT_NOPRIV			    0x09
#define BT_HWERROR			    0x0A
#define BT_HOSTERROR		    0x0B
#define BT_UNKNOWNERROR		    0x0C
#define BT_HOSTINACTIVE		    0x0D
#define BT_RETRY			    0x0E
#define BT_AUTHORISATIONFAIL    0x0F
#define BT_AUTHENTICATIONFAIL   0x10
#define BT_ENCRYPTFAIL		    0x11
#define BT_TIMEOUT			    0x12
#define BT_PROTOERROR		    0x13
#define BT_DISALLOWED           0x14
#define BT_BUFTOOSMALL		    0x15
#define BT_DISCONNECTED         0x16

#define BT_INVALIDERRORCODE 0xFE /* This will never be returned from any call */

#endif
