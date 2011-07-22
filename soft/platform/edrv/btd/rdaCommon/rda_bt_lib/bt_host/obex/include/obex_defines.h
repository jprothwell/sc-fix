/****************************************************************
 *
 * MODULE NAME:    obex_defines.h
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_defines.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/



/*
  response codes
 */

#define OBEX_CONTINUE 0x90

#define OBEX_OK 0xa0
#define OBEX_CREATED 0xa1
#define OBEX_ACCEPTED 0xa2
#define OBEX_NON_AUTH_INFO 0xa3
#define OBEX_NO_CONTENT 0xa4
#define OBEX_RESET_CONTENT 0xa5
#define OBEX_PARTIAL_RESET 0xa6

#define OBEX_MULTIPLE_CHOICE 0xb0
#define OBEX_MOVED_PERM 0xb1
#define OBEX_MOVED_TEMP 0xb2
#define OBEX_SEE_OTHER 0xb3
#define OBEX_NOT_MODIFIED 0xb4
#define OBEX_USE_PROXY 0xb5

#define OBEX_BAD_REQUEST 0xc0
#define OBEX_UNAUTHORIZED 0xc1
#define OBEX_PAYMENT_REQUIRED 0xc2
#define OBEX_FORBIDDEN 0xc3
#define OBEX_NOT_FOUND 0xc4
#define OBEX_METHOD_NOT_ALLOWED 0xc5
#define OBEX_NOT_ACCEPTABLE 0xc6
#define OBEX_PROXY_AUTH_REQUIRED 0xc7
#define OBEX_REQUEST_TIME_OUT 0xc8
#define OBEX_CONFLICT 0xc9
#define OBEX_GONE 0xca
#define OBEX_LENGTH_REQUIRED 0xcb
#define OBEX_PRECONDITION_FAIL 0xcc
#define OBEX_REQUEST_ENTITY_TOO_LARGE 0xcd
#define OBEX_REQUEST_URL_TOO_LARGE 0xce
#define OBEX_UNSUPPORTED_MEDIA_TYPE 0xcf

#define OBEX_SERVER_ERROR 0xd0
#define OBEX_NOT_IMPLEMENTED 0xd1
#define OBEX_BAD_GATEWAY 0xd2
#define OBEX_SERVICE_UNAVAILABLE 0xd3
#define OBEX_GATEWAY_TIMEOUT 0xd4
#define OBEX_HTTP_VER_NOT_SUPPORTED 0xd5

#define OBEX_DATABASE_FULL 0xe0
#define OBEX_DATABASE_LOCKED 0xe1

/*
  Operation Opcodes
 */

#define OBEX_CONNECT 0x80
#define OBEX_DISCONNECT 0x81
#define OBEX_PUT_FINAL 0x82
#define OBEX_PUT 0X02
#define OBEX_GET 0x03
#define OBEX_GET_FINAL 0x83
#define OBEX_SETPATH 0x85
#define OBEX_RESERVED 0x04
#define OBEX_RESERVED_FINAL 0x84
#define OBEX_SETPATH 0x85
#define OBEX_ABORT 0xff
#define OBEX_OP_RESERVED_LOWER 0x06
#define OBEX_OP_RESERVER_UPPER 0x0f
#define OBEX_OP_USER_LOWER 0x10
#define OBEX_OP_USER_UPPER 0x1f

/* header codes */

#define OBEX_COUNT 0xc0
#define OBEX_NAME 0x01
#define OBEX_TYPE 0x42
#define OBEX_LENGTH 0xc3
#define OBEX_TIME_ISO 0x44
#define OBEX_TIME_32 0xc4
#define OBEX_DESCRIPTION 0x05
#define OBEX_TARGET 0x46
#define OBEX_HTTP 0x47
#define OBEX_BODY 0x48
#define OBEX_END_BODY 0x49
#define OBEX_WHO 0x4a
#define OBEX_CID 0xcb
#define OBEX_APP_PARAM 0x4c
#define OBEX_AUTH_CHALLANGE 0x4d
#define OBEX_AUTH_RESPONSE 0x4e
#define OBEX_CLASS 0x4f
#define OBEX_HD_RESERVED_LOWER 0x10
#define OBEX_HD_RESERVER_UPPER 0x2f
#define OBEX_HD_USER_LOWER 0x30
#define OBEX_HD_USER_UPPER 0x3f

#define OBEX_HEADER 3
#define OBEX_SETPATH_HEADER 5
#define OBEX_CONNECT_HEADER 7

/* supported types */

#define OBEX_PUSH_VCARD_21 1
#define OBEX_PUSH_VCARD_30 2
#define OBEX_PUSH_VCAL_10 4
#define OBEX_PUSH_ICAL_20 8
#define OBEX_PUSH_VNOTE : 16
#define OBEX_PUSH_VMESSAGE : 32
#define OBEX_PUSH_ANY : 64

/* supported datastores */

#define OBEX_SYNC_PB 1
#define OBEX_SYNC_CAL 2
#define OBEX_SYNC_NT 4
#define OBEX_SYNC_MSG 8

/* sync type */

#define OBEX_SC_DEFAULT 0
#define OBEX_SC_PB 1
#define OBEX_SC_CAL 2
#define OBEX_SC_NOTE 4
#define OBEX_SC_INMEG 8
#define OBEX_SC_OUTMSG 16
#define OBEX_SC_SENTMSG 32
#define OBEX_SC_RTC 64 

/* service type */

#define OBEX_INBOX_SERVICE
#define OBEX_CAPABILITY_SERVICE
#define OBEX_IRMC_SERVCE

/* internal defines */

#define PRH_OBEX_TYPE_RFCOMM 0
#define PRH_OBEX_TYPE_SERIAL 1
#define PRH_OBEX_TYPE_SOCKET 2
#define PRH_OBEX_TYPE_GENERIC 3

#define PRH_OBEX_STATE_START 0
#define PRH_OBEX_STATE_CONTINUED 1
#define PRH_OBEX_STATE_CONTINUED_GET 2


#define PRH_OBEX_MIN_MAX 255









