/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifdef CFW_TCPIP_SUPPORT
#ifndef __LWIP_API_MSG_H__
#define __LWIP_API_MSG_H__

#include "tcpip_opt.h"
#include "tcpip_pbuf.h"
#include "tcpip_sys.h"

#include "tcpip_ip.h"

#include "tcpip_udp.h"
#include "tcpip_tcp.h"

#include "tcpip_api.h"

#ifndef CII_VERSION
enum api_msg_type
{
	API_MSG_NEWCONN,
	API_MSG_DELCONN,

	API_MSG_BIND,
	API_MSG_CONNECT,
	API_MSG_DISCONNECT,

	API_MSG_LISTEN,
	API_MSG_ACCEPT,

	API_MSG_SEND,
	API_MSG_RECV,
	API_MSG_WRITE,

	API_MSG_CLOSE,

	API_MSG_MAX
};

#else


#define		API_MSG_NEWCONN		0
#define 		API_MSG_DELCONN		1

#define 		API_MSG_BIND			2
#define 		API_MSG_CONNECT		3
#define 		API_MSG_DISCONNECT	4

#define 		API_MSG_LISTEN		5
#define 		API_MSG_ACCEPT		6

#define 		API_MSG_SEND			7
#define 		API_MSG_RECV			8
#define 		API_MSG_WRITE			9

#define 		API_MSG_CLOSE			10

#define 		API_MSG_MAX			11

#endif

struct api_msg_msg
{
	struct netconn *conn;
//	enum netconn_type conntype;
	UINT8 conntype;
	union
	{
		struct pbuf *p;
		struct
		{
			struct ip_addr *ipaddr;
			UINT16 port;
		} bc;
		struct
		{
			VOID *dataptr;
			UINT16 len;
			UINT8 copy;
		} w;
		sys_mbox_t mbox;
		UINT16 len;
	} msg;
	BOOL   win_refreshflag;
	struct netbuf *pnetbuf;
};

struct api_msg
{
//	enum api_msg_type type;
	UINT8 type;
	struct api_msg_msg msg;
};
/****************************************/
/*
struct aps_msg_smtp
{
	CFW_TCPIP_SOCKET_ADDR smtpSvr_addr;
};
typedef struct aps_msg{
//	enum api_msg_type type;
	union{
		CFW_TCPIP_SOCKET_ADDR smtpSvr_addr;
		//others;
	}Msg_body;
}aps_msg_t;
*/

/***************************************/
#if 0  //defined in tcpip.h
struct tcpip_msg_t 
{
    sys_sem_t *sem;
    union 
    {
        struct api_msg *apimsg;
        struct 
        {
            struct pbuf *p;
            struct ip_addr *netif;
        } inp;
        struct 
        {
            VOID (*f)(void *ctx);
            VOID *ctx;
        } cb;
    } msg;
};
#endif


VOID api_msg_input(struct api_msg *msg);
VOID api_msg_post(struct api_msg *msg);

#endif /* __LWIP_API_MSG_H__ */
#endif

