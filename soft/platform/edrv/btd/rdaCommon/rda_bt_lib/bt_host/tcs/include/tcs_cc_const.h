/*
 * MODULE NAME:    tcs_cc_const.h
 * DESCRIPTION:    TCS Call Control Constants
 * AUTHOR:         Thomas Howley
 * DATE:           09 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_const.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef __TCS_CC_CONST_H
#define __TCS_CC_CONST_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)


#include "host_types.h"
#include "mgr_core.h"

#define PRH_TCS_IDLE						0x00
#define PRH_TCS_W4UNPARK					0x01
#define PRH_TCS_SETUP_SIGCHANPENDING		0x02
#define PRH_TCS_SETUPACK_SIGCHANPENDING		0x03
#define PRH_TCS_ALERTING_SIGCHANPENDING		0x04
#define PRH_TCS_CONNECT_SIGCHANPENDING      0x05
#define PRH_TCS_CALLINITIATED				0x06
#define PRH_TCS_OVERLAPSENDING				0x07
#define PRH_TCS_OUTGOINGCALLPROCEEDING		0x08
#define PRH_TCS_CALLDELIVERED				0x09
#define PRH_TCS_CONNECTACKPENDING			0x0a
#define PRH_TCS_ORIGACTIVE					0x0b
#define PRH_TCS_CALLPRESENT					0x0c
#define PRH_TCS_OVERLAPRECEIVING			0x0d
#define PRH_TCS_INCOMINGCALLPROCEEDING		0x0e
#define PRH_TCS_CALLRECEIVED				0x0f
#define PRH_TCS_TERMBEARERPENDING			0x10
#define PRH_TCS_CONNECTREQUEST				0x11
#define PRH_TCS_TERMACTIVE					0x12
#define PRH_TCS_DISCONNECTREQUEST			0x13
#define PRH_TCS_DISCONNECTINDICATION		0x14
#define PRH_TCS_RELEASEREQUEST				0x15
#define PRH_TCS_RELEASECOMPLETEPENDING		0x16
#define PRH_TCS_ORIGRELEASEBEARERPENDING	0x17
#define PRH_TCS_TERMRELEASEBEARERPENDING	0x18
#define PRH_TCS_RELEASECOMPLETE_SIGCHANPENDING   0x19


#define PRH_TCS_DEFAULT_SCOPACKETTYPE MGR_HV3PACKETTYPE

/* Two Profiles call types - matches psm values used for profiles */
#define PRH_TCS_CALLTYPE_TCSBIN      PRH_TCS_PSM_TCSBIN
#define PRH_TCS_CALLTYPE_TCSCORDLESS PRH_TCS_PSM_TCSCORDLESS
/* used internally by stack only */
#define PRH_TCS_CALLTYPE_PENDINGBCAST PRH_TCS_PSM_TCSBIN+PRH_TCS_PSM_TCSCORDLESS

/* TCS TL Registration */
#define PRH_TCS_REGISTER_TCSBIN		 0x1
#define PRH_TCS_REGISTER_TCSCORDLESS 0x2

#define PRH_TCS_ENABLE_TCSBIN(X)		((X) & PRH_TCS_REGISTER_TCSBIN)
#define PRH_TCS_ENABLE_TCSCORDLESS(X)	((X) & PRH_TCS_REGISTER_TCSCORDLESS)

/* TCS (TL/GW) states */
#define PRH_TCS_INST_IDLE		0x00
#define PRH_TCS_INST_CONNECTED	0x01
#define PRH_TCS_INST_PARKED		0x02

/* for pending incoming external calls...waiting on response to a broadcast */
#define PRH_TCS_CALLHANDLEUNASSIGNED 0xFFFF




#if PRH_TCS_GATEWAY
/* Determines whether point-point or multipoing signalling is used for call setup */
#if PRH_TCS_SINGLEPOINTCONFIGURATION
#define PRH_TCS_MAXTERMINALSPERGATEWAY 1
#else
#define PRH_TCS_MAXTERMINALSPERGATEWAY  7
#endif


#endif /* PRH_TCS_GATEWAY */



#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/

#endif /* __TC_CC_CONST_H */
