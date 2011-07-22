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
#ifndef __CII_UDP_H__
#define __CII_UDP_H__

#include "tcpip_arch.h"

#include "tcpip_pbuf.h"
#include "tcpip_inet.h"
#include "tcpip_ip.h"

#define UDP_HLEN							8
#ifndef CII_VERSION

struct udp_hdr
{
	PACK_STRUCT_FIELD(UINT16 src);
	PACK_STRUCT_FIELD(UINT16 dest);				/* src/dest UDP ports */
	PACK_STRUCT_FIELD(UINT16 len);
	PACK_STRUCT_FIELD(UINT16 chksum);
} PACK_STRUCT_STRUCT;
#else
struct udp_hdr
{
	UINT16 src;
	UINT16 dest;				/* src/dest UDP ports */
	UINT16 len;
	UINT16 chksum;
} ;
#endif

#define UDP_FLAGS_NOCHKSUM				0x01U
#define UDP_FLAGS_UDPLITE					0x02U
#define UDP_FLAGS_CONNECTED				0x04U

struct udp_pcb
{
/* Common members of all PCB types */
	IP_PCB;

/* Protocol specific PCB members */

	struct udp_pcb *next;

	UINT8 flags;
	UINT16 local_port, remote_port;

	UINT16 chksum_len;

	VOID (*recv) (VOID *arg, struct udp_pcb * pcb, struct pbuf * p, struct ip_addr * addr, UINT16 port);
	VOID *recv_arg;
};

/* The following functions is the application layer interface to the  UDP code. */
struct udp_pcb *udp_new(VOID);
VOID udp_remove(struct udp_pcb *pcb);
err_t udp_bind(struct udp_pcb *pcb, struct ip_addr *ipaddr, UINT16 port);
err_t udp_connect(struct udp_pcb *pcb, struct ip_addr *ipaddr, UINT16 port);
VOID udp_disconnect(struct udp_pcb *pcb);
VOID udp_recv(struct udp_pcb *pcb, VOID (*recv) (VOID *arg, struct udp_pcb * upcb, struct pbuf * p, struct ip_addr * addr, UINT16 port), VOID *recv_arg);
err_t udp_sendto(struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *dst_ip, UINT16 dst_port);
err_t udp_send(struct udp_pcb *pcb, struct pbuf *p);

#define  udp_flags(pcb)			((pcb)->flags)
#define  udp_setflags(pcb, f)		((pcb)->flags = (f))

/* The following functions are the lower layer interface to UDP. */
VOID udp_input(struct pbuf *p, struct netif *inp);
VOID udp_init(VOID);

#if UDP_DEBUG
VOID udp_debug_print(struct udp_hdr *udphdr);
#else
#define udp_debug_print(udphdr)
#endif
#endif /* __CII_UDP_H__ */
#endif


