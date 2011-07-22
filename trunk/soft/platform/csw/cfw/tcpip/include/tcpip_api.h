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
#ifndef __LWIP_API_H__
#define __LWIP_API_H__

#include "tcpip_opt.h"
#include "tcpip_pbuf.h"
#include "tcpip_sys.h"

#include "tcpip_ip.h"

#include "tcpip_raw.h"
#include "tcpip_udp.h"
#include "tcpip_tcp.h"

#include "tcpip_err.h"


#define NETCONN_NOCOPY				0x00
#define NETCONN_COPY   				0x01

//enum netconn_type
#define		NETCONN_TCP				0
#define		NETCONN_UDP				1
#define		NETCONN_UDPLITE			2
#define		NETCONN_UDPNOCHKSUM		3
#define		NETCONN_RAW				4
#define		NETCONN_INVAID			255

//enum netconn_state
#define 		NETCONN_NONE				0
#define 		NETCONN_WRITE				1
#define 		NETCONN_ACCEPT			2
#define 		NETCONN_RECV				3
#define 		NETCONN_CONNECT			4
#define 		NETCONN_CLOSE				5

//enum netconn_evt
#define         NETCONN_EVT_RCVPLUS     0
#define         NETCONN_EVT_RCVMINUS    1
#define         NETCONN_EVT_SENDPLUS    2
#define         NETCONN_EVT_SENDMINUS   3



struct netbuf
{
	struct pbuf *p;
	struct pbuf *ptr;
	struct ip_addr *fromaddr;
	UINT16 fromport;
	err_t err;
	UINT8 nPad;
};

struct netconn
{
	UINT8  type;				//netconn_type
	UINT8  state;				//netconn_state
	UINT8 nPad[2];
	union
	{
		struct tcp_pcb *tcp;
		struct udp_pcb *udp;
		struct raw_pcb *raw;
	} pcb;
	
	UINT8 socket;

	err_t err;
	//sys_mbox_t mbox;
	//sys_mbox_t recvmbox;
	//sys_mbox_t acceptmbox;
	//sys_sem_t sem;
	UINT16 recv_avail;
	//VOID (*callback) (struct netconn *, UINT8, UINT16 len);
};

/* Network buffer functions: */
struct netbuf *netbuf_new(VOID);
VOID netbuf_delete(struct netbuf *buf);
VOID *netbuf_alloc(struct netbuf *buf, UINT16 size);
VOID netbuf_free(struct netbuf *buf);
VOID netbuf_ref(struct netbuf *buf, VOID *dataptr, UINT16 size);
VOID netbuf_chain(struct netbuf *head, struct netbuf *tail);

UINT16 netbuf_len(struct netbuf *buf);
err_t netbuf_data(struct netbuf *buf, VOID **dataptr, UINT16 * len);
INT8 netbuf_next(struct netbuf *buf);
VOID netbuf_first(struct netbuf *buf);

VOID netbuf_copy(struct netbuf *buf, VOID *dataptr, UINT16 len);
VOID netbuf_copy_partial(struct netbuf *buf, VOID *dataptr, UINT16 len, UINT16 offset);
struct ip_addr *netbuf_fromaddr(struct netbuf *buf);
UINT16 netbuf_fromport(struct netbuf *buf);

/* Network connection functions: */
struct netconn *netconn_new(UINT8 type);
struct netconn *netconn_new_with_callback(UINT8 t, VOID (*callback) (struct netconn *,UINT8, UINT16 len));
struct netconn *netconn_new_with_proto_and_callback(UINT8 t, UINT16 proto, VOID (*callback) (struct netconn *, UINT8, UINT16 len));
err_t netconn_delete(struct netconn *conn);
UINT8 netconn_type(struct netconn *conn);
err_t netconn_peer(struct netconn *conn, struct ip_addr *addr, UINT16 * port);
err_t netconn_addr(struct netconn *conn, struct ip_addr **addr, UINT16 * port);
err_t netconn_bind(struct netconn *conn, struct ip_addr *addr, UINT16 port);
err_t netconn_connect(struct netconn *conn, struct ip_addr *addr, UINT16 port);
err_t netconn_disconnect(struct netconn *conn);
err_t netconn_listen(struct netconn *conn);
struct netconn *netconn_accept(struct netconn *conn);
struct netbuf *netconn_recv(struct netconn *conn);
err_t netconn_send(struct netconn *conn, struct netbuf *buf);
err_t netconn_write(struct netconn *conn, VOID *dataptr, UINT16 size, UINT8 copy);
err_t netconn_close(struct netconn *conn);

err_t netconn_err(struct netconn *conn);


#ifndef BLOCK
#define TCP_RECVDATA_IND        0x1A
#define TCP_SNDDATACMP_IND      0x1B
#define TCP_RECVDATA_CMF        0x1C
#endif


#endif
#endif /* __LWIP_API_H__ */
