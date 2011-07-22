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
#ifndef __LWIP_STATS_H__
#define __LWIP_STATS_H__

#include "tcpip_opt.h"
//#include "cc.h"

#include "tcpip_mem.h"
#include "tcpip_memp.h"

#if LWIP_STATS

struct stats_proto
{
	UINT16 xmit;		/* Transmitted packets. */
	UINT16 rexmit;		/* Retransmitted packets. */
	UINT16 recv;		/* Received packets. */
	UINT16 fw;			/* Forwarded packets. */
	UINT16 drop;		/* Dropped packets. */
	UINT16 chkerr;		/* Checksum error. */
	UINT16 lenerr;		/* Invalid length error. */
	UINT16 memerr;		/* Out of memory error. */
	UINT16 rterr;		/* Routing error. */
	UINT16 proterr;		/* Protocol error. */
	UINT16 opterr;		/* Error in options. */
	UINT16 err;			/* Misc error. */
	UINT16 cachehit;
};

struct stats_mem
{
	mem_size_t avail;
	mem_size_t used;
	mem_size_t max;
	mem_size_t err;
};

struct stats_pbuf
{
	UINT16 avail;
	UINT16 used;
	UINT16 max;
	UINT16 err;

	UINT16 alloc_locked;
	UINT16 refresh_locked;
};

struct stats_syselem
{
	UINT16 used;
	UINT16 max;
	UINT16 err;
};

struct stats_sys
{
	struct stats_syselem sem;
	struct stats_syselem mbox;
};

struct stats_
{
	struct stats_proto link;
	struct stats_proto ip_frag;
	struct stats_proto ip;
	struct stats_proto icmp;
	struct stats_proto udp;
	struct stats_proto tcp;
	struct stats_pbuf pbuf;
	struct stats_mem mem;
	struct stats_mem memp[MEMP_MAX];
	struct stats_sys sys;
};

extern struct stats_ lwip_stats;

VOID stats_init(VOID);

#define STATS_INC(x) ++lwip_stats.x
#else
#define stats_init()
#define STATS_INC(x)
#endif /* LWIP_STATS */

#if TCP_STATS
#define TCP_STATS_INC(x) STATS_INC(x)
#else
#define TCP_STATS_INC(x)
#endif

#if UDP_STATS
#define UDP_STATS_INC(x) STATS_INC(x)
#else
#define UDP_STATS_INC(x)
#endif

#if ICMP_STATS
#define ICMP_STATS_INC(x) STATS_INC(x)
#else
#define ICMP_STATS_INC(x)
#endif

#if IP_STATS
#define IP_STATS_INC(x) STATS_INC(x)
#else
#define IP_STATS_INC(x)
#endif

#if IPFRAG_STATS
#define IPFRAG_STATS_INC(x) STATS_INC(x)
#else
#define IPFRAG_STATS_INC(x)
#endif

#if LINK_STATS
#define LINK_STATS_INC(x) STATS_INC(x)
#else
#define LINK_STATS_INC(x)
#endif

/* Display of statistics */
#if LWIP_STATS_DISPLAY
VOID stats_display(VOID);
#else
#define stats_display()
#endif

#endif /* __LWIP_STATS_H__ */

#endif

