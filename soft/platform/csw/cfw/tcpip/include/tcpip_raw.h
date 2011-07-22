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
#ifndef __LWIP_RAW_H__
#define __LWIP_RAW_H__

#include "tcpip_arch.h"

#include "tcpip_pbuf.h"
#include "tcpip_inet.h"
#include "tcpip_ip.h"

struct raw_pcb
{
/* Common members of all PCB types */
	IP_PCB;

	struct raw_pcb *next;

	UINT16 protocol;

	  UINT8(*recv) (VOID *arg, struct raw_pcb * pcb, struct pbuf * p, struct ip_addr * addr);
	VOID *recv_arg;
};

/* The following functions is the application layer interface to the
   RAW code. */
struct raw_pcb *raw_new(UINT16 proto);
VOID raw_remove(struct raw_pcb *pcb);
err_t raw_bind(struct raw_pcb *pcb, struct ip_addr *ipaddr);
err_t raw_connect(struct raw_pcb *pcb, struct ip_addr *ipaddr);

VOID raw_recv(struct raw_pcb *pcb, UINT8(*recv) (VOID *arg, struct raw_pcb * pcb, struct pbuf * p, struct ip_addr * addr), VOID *recv_arg);
err_t raw_sendto(struct raw_pcb *pcb, struct pbuf *p, struct ip_addr *ipaddr);
err_t raw_send(struct raw_pcb *pcb, struct pbuf *p);

/* The following functions are the lower layer interface to RAW. */
UINT8 raw_input(struct pbuf *p, struct netif *inp);
VOID raw_init(VOID);

#endif /* __LWIP_RAW_H__ */
#endif

