/******************************************************************
 * MODULE NAME:    l2_const.h
 * PROJECT CODE:
 * DESCRIPTION:    L2CAP Constants, Opcodes, PDU lengths etc..
 * AUTHOR:         Gary Fleming
 * DATE:           21 July 1999
 *
 * SOURCE CONTROL: $Id: l2_const.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 ******************************************************************/

#ifndef _HOST_L2CAP_CONST
#define _HOST_L2CAP_CONST

#include "host_config.h"

/****************************************************************
 * L2CAP CONSTANTS DEFINED IN THE BLUETOOTH L2CAP SPECIFICATION
 ****************************************************************/

#define MAX_L2CAP_OPCODES 0x0C

#define L2CAP_RESERVED              0x00
#define L2CAP_COMMAND_REJECT        0x01
#define L2CAP_CONNECTION_REQUEST    0x02 
#define L2CAP_CONNECTION_RESPONSE   0x03 
#define L2CAP_CONFIGURE_REQUEST     0x04
#define L2CAP_CONFIGURE_RESPONSE    0x05 
#define L2CAP_DISCONNECTION_REQUEST    0x06 
#define L2CAP_DISCONNECTION_RESPONSE   0x07  
#define L2CAP_ECHO_REQUEST          0x08  
#define L2CAP_ECHO_RESPONSE         0x09  
#define L2CAP_INFORMATION_REQUEST   0x0a  
#define L2CAP_INFORMATION_RESPONSE  0x0b  

#define L2CAP_COMMAND_REJECT_LENGTH             0x06  /* Without optional data field */ 
#define L2CAP_CONNECTION_REQ_PARAM_LENGTH       0x0004
#define L2CAP_CONNECTION_RSP_PARAM_LENGTH       0x0008
#define L2CAP_CONFIGURE_REQ_PARAM_LENGTH        0x0004  /* Without options field */
#define L2CAP_CONFIGURE_RSP_PARAM_LENGTH        0x0008
#define L2CAP_DISCONNECTION_REQ_PARAM_LENGTH    0x0004
#define L2CAP_DISCONNECTION_RSP_PARAM_LENGTH    0x0004
#define L2CAP_ECHO_REQ_PARAM_LENGTH             0x0004  /* Without optional data field */
#define L2CAP_ECHO_RSP_PARAM_LENGTH             0x0004  /* Without optional data field */
#define L2CAP_INFO_REQUEST_LENGTH              0x06
#define L2CAP_INFO_RESPONSE_LENGTH             0x08  /* Without optional data field */

/************************************/
/* Result Values for L2CAP messages */
/************************************/

/* Results Values of Connection Response */

#define L2CAP_CONNECTION_SUCCESSFUL                 0x0000
#define L2CAP_CONNECTION_PENDING                    0x0001
#define L2CAP_CONNECTION_REFUSED_PSM_NOT_SUPPORTED  0x0002
#define L2CAP_CONNECTION_REFUSED_SECURITY_BLOCK     0x0003
#define L2CAP_CONNECTION_REFUSED_NO_RESOURCES_AVAIL 0x0004

#define L2CAP_CONNECTION_TIMEOUT 0xEEEE

/* Results Values of Configuration Response */

#define L2CAP_CONFIG_SUCCESSFUL                  0x0000
#define L2CAP_CONFIG_FAILURE_UNACCEPTABLE_PARAMS 0x0001
#define L2CAP_CONFIG_FAILURE_REJECTED            0x0002
#define L2CAP_CONFIG_FAILURE_UNKNOWN_OPTIONS     0x0003

/* Result Values for the Information Response */
#define L2CAP_SUCCESS                0x0000
#define L2CAP_INFO_SUCCESS           0x0000
#define L2CAP_INFO_NOT_SUPPORTED     0x0001
#define L2CAP_INFO_PDU_REJECTED      0x0002
#define L2CAP_INFO_TIMEOUT_OCCURED   0x0003

/* Result Values for the Ping Response */

#define L2CAP_PING_SUCCESS           0x0000
#define L2CAP_PING_TIMEOUT_OCCURED   0x0001

#define L2CAP_REJECT_COMMAND_NOT_UNDERSTOOD  0x0000
#define L2CAP_REJECT_SIGNALLING_MTU_EXCEEDED 0x0001
#define L2CAP_REJECT_INVALID_CID_IN_REQUEST  0x0002

/* Pre-defined channel identifiers */

#define L2CAP_NULL_IDENTIFIER 0x00
#define L2CAP_SIGNALLING_CID  0x01
#define L2CAP_CONNECTIONLESS_DATA 0x02

/* Header lengths */

#define L2CAP_PDU_HEADER_LENGTH 0x04
#define L2CAP_SIG_HEADER_LENGTH 0x04

/* Spec-defined QoS Values */

#define PRH_L2_DEFAULT_SERVICE  PRH_L2_BEST_EFFORT
#define PRH_L2_TOKEN_RATE_WILD_CARD 0xFFFFFFFF
#define PRH_L2_TOKEN_RATE_DONT_CARE 0x00000000 
#define PRH_L2_TOKEN_BUCKET_WILD_CARD 0xFFFFFFFF
#define PRH_L2_TOKEN_BUCKET_DONT_CARE 0x00000000 
#define PRH_L2_PEAK_BANDWDTH_UNKNOWN 0x00000000
#define PRH_L2_LATENCY_DONT_CARE 0xFFFFFFFF
#define PRH_L2_DELAY_DONT_CARE   0xFFFFFFFF

/* Spec Defaults */

#define PRH_L2_DEFAULT_MTU  896//672    /* ref Page 289 of Ver 1.0 */
#define PRH_L2_DEFAULT_FLUSH_TO 0xFFFF /* Infinite retransmissions - Reliable channel -see pg 289 */
#define PRH_L2_DEFAULT_LINK_TO 0xFFFF  

#define PRH_L2_MIN_USER_CID 0x40
#define PRH_L2_MAX_SIGNALLING_MTU 0x048

/* Spec specific Identifier for Information Type Field */

#define PRH_L2_CONNECTIONLESS_MTU_INFOTYPE 0x0001

/* The different option type supported in the Config PDUS */

#define PRH_L2_MTU_OPTION    0x01 
#define PRH_L2_FLUSH_OPTION  0x02
#define PRH_L2_QOS_OPTION    0x03
#define PRH_L2_RETRANSMISSION_AND_FLOW_CONTROL_OPTION 0x04

/************************************
 * PENDING OPERATIONS ON AN ACL LINK
 * DURING ACL ESTABLISHMENT
 ************************************/

#define L2_CHANNEL_SETUP  0xC0
#define L2_ECHO_REQ       0xC1
#define L2_INFO_REQ	      0xC2
#define L2_GROUP_ADD      0xC3

/********************************
 * L2CAP STATE MACHINE TIMEOUT VALUES
 ********************************/

#define PRH_L2_RTX_TIMEOUT_VALUE   320	/* 32 Seconds */
#define PRH_L2_ERTX_TIMEOUT_VALUE  1280	/* 128 Seconds */
#define PRH_L2_CFG_TIMEOUT_VALUE 1200 /* 120 Seconds  */

#define START 1
#define CONTINUE 0

#define UL_HEADER_SIZE 0

#define PRH_L2_CLT_NONE    00
#define PRH_L2_CLT_ENABLED 01
#define PRH_L2_CLT_DISABLED 02

/* Parthus Specific flags indicating if an ACL link should be released
 * on channel release - overloaded to specify if config info should also
 * be released - D.A. 
 */

#define PRH_L2_DONT_RELEASE_ACL 0
#define PRH_L2_RELEASE_ACL 1
#define PRH_L2_DONT_RELEASE_CONFIG 2

/* Parthus Specific values for the Channel Type */

#define PRH_L2_CLS_CHANNEL  0x01
#define PRH_L2_CO_CHANNEL  0x02

/* Parthus Secific flag to determine of the C-Bit should be used 
 * in L2CAP CONFIGURATION PDUs
 */

#define PRH_L2_USE_C_BIT 0x00

/* Configigure Continue Values */

#define PRH_L2_CONFIG_CONTINUE_ON  0x0001
#define PRH_L2_CONFIG_CONTINUE_OFF 0x0000

#define PRH_L2_DEFAULT_CONNECTIONLESS_HANDLE 0x0012

#endif
