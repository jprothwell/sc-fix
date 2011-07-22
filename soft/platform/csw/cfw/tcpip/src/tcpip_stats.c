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

//#include <string.h>
//#include <cswtype.h>

//#include "tcpip_opt.h"
#ifdef CFW_TCPIP_SUPPORT
#include "tcpip_stats.h"
#include "tcpip_def.h"
#include <csw.h>
//#include "tcpip_mem.h"
//#include "sul.h"
#if LWIP_STATS
struct stats_ lwip_stats;

VOID stats_init(VOID)
{
	SUL_MemSet8(&lwip_stats, 0, sizeof(struct stats_));
}

#if LWIP_STATS_DISPLAY
VOID stats_display_proto(struct stats_proto *proto, INT8 *name)
{
#if 0
	LWIP_PLATFORM_DIAG(("\n%s\n\t", name));
	LWIP_PLATFORM_DIAG(("xmit: 0x%x\n\t", proto->xmit));
	LWIP_PLATFORM_DIAG(("rexmit: 0x%x\n\t", proto->rexmit));
	LWIP_PLATFORM_DIAG(("recv: 0x%x\n\t", proto->recv));
	LWIP_PLATFORM_DIAG(("fw: 0x%x\n\t", proto->fw));
	LWIP_PLATFORM_DIAG(("drop: 0x%x\n\t", proto->drop));
	LWIP_PLATFORM_DIAG(("chkerr: 0x%x\n\t", proto->chkerr));
	LWIP_PLATFORM_DIAG(("lenerr: 0x%x\n\t", proto->lenerr));
	LWIP_PLATFORM_DIAG(("memerr: 0x%x\n\t", proto->memerr));
	LWIP_PLATFORM_DIAG(("rterr: 0x%x\n\t", proto->rterr));
	LWIP_PLATFORM_DIAG(("proterr: 0x%x\n\t", proto->proterr));
	LWIP_PLATFORM_DIAG(("opterr: 0x%x\n\t", proto->opterr));
	LWIP_PLATFORM_DIAG(("err: 0x%x\n\t", proto->err));
	LWIP_PLATFORM_DIAG(("cachehit: 0x%x\n", proto->cachehit));
#endif	
}

VOID stats_display_pbuf(struct stats_pbuf *pbuf)
{
#if 0
	LWIP_PLATFORM_DIAG(("\nPBUF\n\t"));
	LWIP_PLATFORM_DIAG(("avail: 0x%x\n\t", pbuf->avail));
	LWIP_PLATFORM_DIAG(("used: 0x%x\n\t", pbuf->used));
	LWIP_PLATFORM_DIAG(("max: 0x%x\n\t", pbuf->max));
	LWIP_PLATFORM_DIAG(("err: 0x%x\n\t", pbuf->err));
	LWIP_PLATFORM_DIAG(("alloc_locked: 0x%x\n\t", pbuf->alloc_locked));
	LWIP_PLATFORM_DIAG(("refresh_locked: 0x%x\n", pbuf->refresh_locked));
#endif
}

VOID stats_display_mem(struct stats_mem *mem, INT8 *name)
{
#if 0
	LWIP_PLATFORM_DIAG(("\n MEM %s\n\t", name));
	LWIP_PLATFORM_DIAG(("avail: 0x%x\n\t", mem->avail));
	LWIP_PLATFORM_DIAG(("used: 0x%x\n\t", mem->used));
	LWIP_PLATFORM_DIAG(("max: 0x%x\n\t", mem->max));
	LWIP_PLATFORM_DIAG(("err: 0x%x\n", mem->err));
#endif
}

VOID stats_display(VOID)
{
#if 0
	s16_t i;
	INT8 *memp_names[] = { "PBUF", "RAW_PCB", "UDP_PCB", "TCP_PCB", "TCP_PCB_LISTEN",
		"TCP_SEG", "NETBUF", "NETCONN", "API_MSG", "TCP_MSG", "TIMEOUT"
	};
	stats_display_proto(&lwip_stats.link, "LINK");
	stats_display_proto(&lwip_stats.ip_frag, "IP_FRAG");
	stats_display_proto(&lwip_stats.ip, "IP");
	stats_display_proto(&lwip_stats.icmp, "ICMP");
	stats_display_proto(&lwip_stats.udp, "UDP");
	stats_display_proto(&lwip_stats.tcp, "TCP");
	stats_display_pbuf(&lwip_stats.pbuf);
	stats_display_mem(&lwip_stats.mem, "HEAP");
	for (i = 0; i < MEMP_MAX; i++)
	{
		stats_display_mem(&lwip_stats.memp[i], memp_names[i]);
	}
#endif
}
#endif /* LWIP_STATS_DISPLAY */
#endif /* LWIP_STATS */
#endif

