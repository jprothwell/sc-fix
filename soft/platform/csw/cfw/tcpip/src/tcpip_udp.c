/**
 * @file
 * User Datagram Protocol module
 *
 */
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

/* udp.c
 *
 * The code for the User Datagram Protocol UDP.
 *
 */


#ifdef CFW_TCPIP_SUPPORT
#include "tcpip_icmp.h"
#include "tcpip_def.h"
#include <csw.h>
#include "tcpip_udp.h"

#include "tcpip_stats.h"

#include "tcpip_snmp.h"

/* The list of UDP PCBs */
#if CII_UDP
/* was static, but we may want to access this from a socket layer */
struct udp_pcb *udp_pcbs = NULL;

static struct udp_pcb *pcb_cache = NULL;

VOID udp_init(VOID)
{
	udp_pcbs    = NULL;
	pcb_cache   = NULL;
}

/**
 * Process an incoming UDP datagram.
 *
 * Given an incoming UDP datagram (as a chain of pbufs) this function
 * finds a corresponding UDP PCB and
 *
 * @param pbuf pbuf to be demultiplexed to a UDP PCB.
 * @param netif network interface on which the datagram was received.
 *
 */

VOID udp_input(struct pbuf *p, struct netif * inp)
{
#if 1
    
    struct udp_hdr *udphdr;
    struct udp_pcb *pcb;
    struct udp_pcb *uncon_pcb;
    struct ip_hdr *iphdr;
    UINT16 src, dest;
    UINT8 local_match;
    
    PERF_START;
    
    UDP_STATS_INC(udp.recv);
    
    iphdr = p->payload;
    
    if (pbuf_header(p, -((INT16) (UDP_HLEN + IPH_HL(iphdr) * 4))))
    {
        /* drop short packets */
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_input: short UDP datagram (0x%x bytes) discarded\n"), p->tot_len);
       // UDP_STATS_INC(udp.lenerr);
      //  UDP_STATS_INC(udp.drop);
        snmp_inc_udpinerrors();
        pbuf_free(p);
        goto end;
    }
    
    udphdr = (struct udp_hdr *) ((UINT8 *) p->payload - UDP_HLEN);
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_input: received datagram of length 0x%x\n"), p->tot_len);
    
    src = ntohs(udphdr->src);
    dest = ntohs(udphdr->dest);
    
    udp_debug_print(udphdr);
    
    /* print the UDP source and destination */
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp (%d.%d.%d.%d, %d) <-- (%d.%d.%d.%d, %d)\n"),
        ip4_addr1(&iphdr->dest), ip4_addr2(&iphdr->dest),
        ip4_addr3(&iphdr->dest), ip4_addr4(&iphdr->dest), 
        ntohs(udphdr->dest), ip4_addr1(&iphdr->src),
        ip4_addr2(&iphdr->src), ip4_addr3(&iphdr->src),
        ip4_addr4(&iphdr->src), ntohs(udphdr->src));
    
    local_match = 0;
    uncon_pcb = NULL;
    /* Iterate through the UDP pcb list for a matching pcb */
    for (pcb = udp_pcbs; pcb != NULL; pcb = pcb->next)
    {
        /* print the PCB local and remote address */
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pcb (%d.%d.%d.%d, %d) --- (%d%d.%d.%d, %d)\n"),
            ip4_addr1(&pcb->local_ip), ip4_addr2(&pcb->local_ip),
            ip4_addr3(&pcb->local_ip), ip4_addr4(&pcb->local_ip), 
            pcb->local_port, ip4_addr1(&pcb->remote_ip), 
            ip4_addr2(&pcb->remote_ip), ip4_addr3(&pcb->remote_ip), 
            ip4_addr4(&pcb->remote_ip), pcb->remote_port);
        
        /* compare PCB local addr+port to UDP destination addr+port */
        if ((pcb->local_port == dest) && (ip_addr_isany(&pcb->local_ip) || ip_addr_cmp(&(pcb->local_ip), &(iphdr->dest))))
        {
            local_match = 1;
            if ((uncon_pcb == NULL) && ((pcb->flags & UDP_FLAGS_CONNECTED) == 0))
            {
                /* the first unconnected matching PCB */
                uncon_pcb = pcb;
            }
        }
        /* compare PCB remote addr+port to UDP source addr+port */
        if ((local_match != 0) && (pcb->remote_port == src) && (ip_addr_isany(&pcb->remote_ip) || ip_addr_cmp(&(pcb->remote_ip), &(iphdr->src))))
        {
            /* the first fully matching PCB */
            break;
        }
    }
    /* no fully matching pcb found? then look for an unconnected pcb */
    if (pcb == NULL)
    {
        pcb = uncon_pcb;
    }
    
    /* Check checksum if this is a match or if it was directed at us. */
    if (pcb != NULL || (inp->ip_addr.addr == iphdr->dest.addr))
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_input: calculating checksum\n"));
        pbuf_header(p, UDP_HLEN);
#ifdef IPv6
        if (iphdr->nexthdr == IP_PROTO_UDPLITE)
        {
#else
            if (IPH_PROTO(iphdr) == IP_PROTO_UDPLITE)
            {
#endif /* IPv4 */
                /* Do the UDP Lite checksum */
#if CHECKSUM_CHECK_UDP
                if (inet_chksum_pseudo(p, (struct ip_addr *) &(iphdr->src), (struct ip_addr *) &(iphdr->dest), IP_PROTO_UDPLITE, ntohs(udphdr->len)) != 0)
                {
                    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_input: UDP Lite datagram discarded due to failing checksum\n"));
                    UDP_STATS_INC(udp.chkerr);
                    UDP_STATS_INC(udp.drop);
                    snmp_inc_udpinerrors();
                    pbuf_free(p);
                    goto end;
                }
#endif
            }
            else
            {
#if CHECKSUM_CHECK_UDP
                if (udphdr->chksum != 0)
                {
                    if (inet_chksum_pseudo(p, (struct ip_addr *) &(iphdr->src), (struct ip_addr *) &(iphdr->dest), IP_PROTO_UDP, p->tot_len) != 0)
                    {
                        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_input: UDP datagram discarded due to failing checksum\n"));
                        
                        UDP_STATS_INC(udp.chkerr);
                        UDP_STATS_INC(udp.drop);
                        snmp_inc_udpinerrors();
                        pbuf_free(p);
                        goto end;
                    }
                }
#endif
            }
            pbuf_header(p, -UDP_HLEN);
            if (pcb != NULL)
            {
                snmp_inc_udpindatagrams();
                /* callback */
                if (pcb->recv != NULL)
                {
                    pcb->recv(pcb->recv_arg, pcb, p, &(iphdr->src), src);
                }
            }
            else
            {
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_input: not for us.\n"));
                
                /* No match was found, send ICMP destination port unreachable unless
                destination address was broadcast/multicast. */
                
                if (!ip_addr_isbroadcast(&iphdr->dest, &(inp->ip_addr)) && !ip_addr_ismulticast(&iphdr->dest))
                {
                    
                    /* adjust pbuf pointer */
                    p->payload = iphdr;
                    icmp_dest_unreach(p, ICMP_DUR_PORT);
                }
                UDP_STATS_INC(udp.proterr);
                UDP_STATS_INC(udp.drop);
                snmp_inc_udpnoports();
                pbuf_free(p);
            }
        }
        else
        {
            pbuf_free(p);
        }
end:
     PERF_STOP("udp_input");
    
   //}
#endif
}

/**
 * Send data to a specified address using UDP.
 *
 * @param pcb UDP PCB used to send the data.
 * @param pbuf chain of pbuf's to be sent.
 * @param dst_ip Destination IP address.
 * @param dst_port Destination UDP port.
 *
 * If the PCB already has a remote address association, it will
 * be restored after the data is sent.
 * 
 * @return lwIP error code.
 * - ERR_TCPIP_OK. Successful. No error occured.
 * - ERR_TCPIP_MEM. Out of memory.
 * - ERR_TCPIP_RTE. Could not find route to destination address.
 *
 * @see udp_disconnect() udp_send()
 */
err_t udp_sendto(struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *dst_ip, UINT16 dst_port)
{
	err_t err;
	/* temporary space for current PCB remote address */
	struct ip_addr pcb_remote_ip;
	UINT16 pcb_remote_port;
	/* remember current remote peer address of PCB */
	pcb_remote_ip.addr = pcb->remote_ip.addr;
	pcb_remote_port = pcb->remote_port;
	/* copy packet destination address to PCB remote peer address */
	pcb->remote_ip.addr = dst_ip->addr;
	pcb->remote_port = dst_port;
	/* send to the packet destination address */
	err = udp_send(pcb, p);
	/* restore PCB remote peer address */
	pcb->remote_ip.addr = pcb_remote_ip.addr;
	pcb->remote_port = pcb_remote_port;
	return err;
}

/**
 * Send data using UDP.
 *
 * @param pcb UDP PCB used to send the data.
 * @param pbuf chain of pbuf's to be sent.
 *
 * @return lwIP error code.
 * - ERR_TCPIP_OK. Successful. No error occured.
 * - ERR_TCPIP_MEM. Out of memory.
 * - ERR_TCPIP_RTE. Could not find route to destination address.
 *
 * @see udp_disconnect() udp_sendto()
 */
err_t udp_send(struct udp_pcb * pcb, struct pbuf * p)
{
	struct udp_hdr *udphdr;
	struct netif *netif;
	struct ip_addr *src_ip;
	err_t err;
	struct pbuf *q;		/* q will be sent down the stack */
	//CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send\n"));

	/* if the PCB is not yet bound to a port, bind it here */
	if (pcb->local_port == 0)
	{
		//CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: not yet bound to a port, binding now\n"));
		err = udp_bind(pcb, &pcb->local_ip, pcb->local_port);
		if (err != ERR_TCPIP_OK)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: forced port bind failed\n"));
			return err;
		}
	}
	/* find the outgoing network interface for this packet */
	netif = ip_route(&(pcb->remote_ip),&(pcb->local_ip));
	/* no outgoing network interface could be found? */
	if (netif == NULL)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: No route to 0x%x\n"), pcb->remote_ip.addr);
		UDP_STATS_INC(udp.rterr);
		return ERR_TCPIP_RTE;
	}

	/* not enough space to add an UDP header to first pbuf in given p chain? */
	if (pbuf_header(p, UDP_HLEN))
	{
		/* allocate header in a seperate new pbuf */
		q = pbuf_alloc(PBUF_IP, UDP_HLEN, PBUF_RAM);
		/* new header pbuf could not be allocated? */
		if (q == NULL)
		{
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: could not allocate header\n"));
			return ERR_TCPIP_MEM;
		}
		/* chain header q in front of given pbuf p */
		pbuf_chain(q, p);
		/* { first pbuf q points to header pbuf } */
	//	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: added header pbuf %p before given pbuf %p\n"), (VOID *) q, (VOID *) p);
		/* adding a header within p succeeded */
	}
	else
	{
		/* first pbuf q equals given pbuf */
		q = p;
		//CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: added header in given pbuf %p\n"), (VOID *) p);
	}
	/* { q now represents the packet to be sent } */
	udphdr = q->payload;
	udphdr->src = htons(pcb->local_port);
	udphdr->dest = htons(pcb->remote_port);
	/* in UDP, 0 checksum means 'no checksum' */
	udphdr->chksum = 0x0000;

	/* PCB local address is IP_ANY_ADDR? */
	if (ip_addr_isany(&pcb->local_ip))
	{
		/* use outgoing network interface IP address as source address */
		src_ip = &(netif->ip_addr);
	}
	else
	{
		/* use UDP PCB local IP address as source address */
		src_ip = &(pcb->local_ip);
	}

	//CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: sending datagram of length 0x%x\n"), q->tot_len);

	/* UDP Lite protocol? */
	if (pcb->flags & UDP_FLAGS_UDPLITE)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: UDP LITE packet length 0x%x\n"), q->tot_len);
		/* set UDP message length in UDP header */
		udphdr->len = htons(pcb->chksum_len);
		/* calculate checksum */
#if CHECKSUM_GEN_UDP
		udphdr->chksum = inet_chksum_pseudo(q, src_ip, &(pcb->remote_ip), IP_PROTO_UDP, pcb->chksum_len);
		/* chksum zero must become 0xffff, as zero means 'no checksum' */
		if (udphdr->chksum == 0x0000)
			udphdr->chksum = 0xffff;
#else
		udphdr->chksum = 0x0000;
#endif
		/* output to IP */
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: ip_output_if (,,,,IP_PROTO_UDPLITE,)\n"));
		err = ip_output_if(q, src_ip, &pcb->remote_ip, pcb->ttl, pcb->tos, IP_PROTO_UDPLITE, netif);
		/* UDP */
	}
	else
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: UDP packet length 0x%x\n"), q->tot_len);
		udphdr->len = htons(q->tot_len);
		/* calculate checksum */
#if CHECKSUM_GEN_UDP
		if ((pcb->flags & UDP_FLAGS_NOCHKSUM) == 0)
		{
			udphdr->chksum = inet_chksum_pseudo(q, src_ip, &pcb->remote_ip, IP_PROTO_UDP, q->tot_len);
			/* chksum zero must become 0xffff, as zero means 'no checksum' */
			if (udphdr->chksum == 0x0000)
				udphdr->chksum = 0xffff;
		}
#else
		udphdr->chksum = 0x0000;
#endif
		//CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: UDP checksum 0x%04x\n"), udphdr->chksum);

	//	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_send: ip_output_if (,,,,IP_PROTO_UDP,)\n"));
             
		/* output to IP */
		err = ip_output_if(q, src_ip, &pcb->remote_ip, pcb->ttl, pcb->tos, IP_PROTO_UDP, netif);
	}
	/* TODO: must this be increased even if error occured? */
	snmp_inc_udpoutdatagrams();

	/* did we chain a seperate header pbuf earlier? */
	if (q != p)
	{
		/* free the header pbuf */
		pbuf_free(q);
		q = NULL;
		/* { p is still referenced by the caller, and will live on } */
	}

	UDP_STATS_INC(udp.xmit);
	return err;
}

/**
 * Bind an UDP PCB.
 *
 * @param pcb UDP PCB to be bound with a local address ipaddr and port.
 * @param ipaddr local IP address to bind with. Use IP_ADDR_ANY to
 * bind to all local interfaces.
 * @param port local UDP port to bind with.
 *
 * @return lwIP error code.
 * - ERR_TCPIP_OK. Successful. No error occured.
 * - ERR_TCPIP_USE. The specified ipaddr and port are already bound to by
 * another UDP PCB.
 *
 * @see udp_disconnect()
 */
err_t udp_bind(struct udp_pcb * pcb, struct ip_addr * ipaddr, UINT16 port)
{
	struct udp_pcb *ipcb;
	UINT8 rebind;

      CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_bind(ipaddr = "));
	ip_addr_debug_print(CFW_TCPIP_TS_ID_SXS, ipaddr);
      CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(", port = %d)\n"), port);

	rebind = 0;
	/* Check for double bind and rebind of the same pcb */
	for (ipcb = udp_pcbs; ipcb != NULL; ipcb = ipcb->next)
	{
		/* is this UDP PCB already on active list? */
		if (pcb == ipcb)
		{
			/* pcb may occur at most once in active list */
//			LWIP_ASSERT("rebind == 0", rebind == 0);
			/* pcb already in list, just rebind */
			rebind = 1;
		}

/* this code does not allow upper layer to share a UDP port for
   listening to broadcast or multicast traffic (See SO_REUSE_ADDR and
   SO_REUSE_PORT under *BSD). TODO: See where it fits instead, OR
   combine with implementation of UDP PCB flags. Leon Woestenberg. */
#ifdef CII_UDP_TODO
		/* port matches that of PCB in list? */
		else if ((ipcb->local_port == port) &&
			 /* IP address matches, or one is IP_ADDR_ANY? */
			 (ip_addr_isany(&(ipcb->local_ip)) || ip_addr_isany(ipaddr) || ip_addr_cmp(&(ipcb->local_ip), ipaddr)))
		{
			/* other PCB already binds to this local IP and port */
                  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_bind: local port 0x%x already bound by another pcb\n"), port);
			return ERR_TCPIP_USE;
		}
#endif

	}

	ip_addr_set(&pcb->local_ip, ipaddr);
	/* no port specified? */
	if (port == 0)
	{
#ifndef UDP_LOCAL_PORT_RANGE_START
#define UDP_LOCAL_PORT_RANGE_START 4096
#define UDP_LOCAL_PORT_RANGE_END   0x7fff
#endif
		port = UDP_LOCAL_PORT_RANGE_START;
		ipcb = udp_pcbs;
		while ((ipcb != NULL) && (port != UDP_LOCAL_PORT_RANGE_END))
		{
			if (ipcb->local_port == port)
			{
				port++;
				ipcb = udp_pcbs;
			}
			else
				ipcb = ipcb->next;
		}
		if (ipcb != NULL)
		{
			/* no more ports available in local range */
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_bind: out of free UDP ports\n"));
			return ERR_TCPIP_USE;
		}
	}
	pcb->local_port = port;
	/* pcb not active yet? */
	if (rebind == 0)
	{
		/* place the PCB on the active list if not already there */
		pcb->next = udp_pcbs;
		udp_pcbs = pcb;
	}
CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_bind: bound to %d.%d.%d.%d, port %d\n"),
							(UINT16) (ntohl(pcb->local_ip.addr) >> 24 & 0xff),
						(UINT16) (ntohl(pcb->local_ip.addr) >> 16 & 0xff), 
						(UINT16) (ntohl(pcb->local_ip.addr) >> 8 & 0xff),
						(UINT16) (ntohl(pcb->local_ip.addr) & 0xff), pcb->local_port);
	return ERR_TCPIP_OK;
}

/**
 * Connect an UDP PCB.
 *
 * This will associate the UDP PCB with the remote address.
 *
 * @param pcb UDP PCB to be connected with remote address ipaddr and port.
 * @param ipaddr remote IP address to connect with.
 * @param port remote UDP port to connect with.
 *
 * @return lwIP error code
 *
 * @see udp_disconnect()
 */
err_t udp_connect(struct udp_pcb * pcb, struct ip_addr * ipaddr, UINT16 port)
{
	struct udp_pcb *ipcb;

	if (pcb->local_port == 0)
	{
		err_t err = udp_bind(pcb, &pcb->local_ip, pcb->local_port);
		if (err != ERR_TCPIP_OK)
			return err;
	}

	ip_addr_set(&pcb->remote_ip, ipaddr);
	pcb->remote_port = port;
	pcb->flags |= UDP_FLAGS_CONNECTED;
/** TODO: this functionality belongs in upper layers */
#ifdef CII_UDP_TODO
	/* Nail down local IP for netconn_addr()/getsockname() */
	if (ip_addr_isany(&pcb->local_ip) && !ip_addr_isany(&pcb->remote_ip))
	{
		struct netif *netif;

		if ((netif = ip_route(&(pcb->remote_ip))) == NULL)
		{
                   CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_connect: No route to 0x%lx\n"), pcb->remote_ip.addr);
			UDP_STATS_INC(udp.rterr);
			return ERR_TCPIP_RTE;
		}
    /** TODO: this will bind the udp pcb locally, to the interface which
        is used to route output packets to the remote address. However, we
        might want to accept incoming packets on any interface! */
		pcb->local_ip = netif->ip_addr;
	}
	else if (ip_addr_isany(&pcb->remote_ip))
	{
		pcb->local_ip.addr = 0;
	}
#endif
           CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("udp_connect: connected to %d.%d.%d.%d,port %d\n"),
						(UINT16) (ntohl(pcb->remote_ip.addr) >> 24 & 0xff),
						(UINT16) (ntohl(pcb->remote_ip.addr) >> 16 & 0xff),
						(UINT16) (ntohl(pcb->remote_ip.addr) >> 8 & 0xff),
				             (UINT16) (ntohl(pcb->remote_ip.addr) & 0xff), pcb->remote_port);

	/* Insert UDP PCB into the list of active UDP PCBs. */
	for (ipcb = udp_pcbs; ipcb != NULL; ipcb = ipcb->next)
	{
		if (pcb == ipcb)
		{
			/* already on the list, just return */
			return ERR_TCPIP_OK;
		}
	}
	/* PCB not yet on the list, add PCB now */
	pcb->next = udp_pcbs;
	udp_pcbs = pcb;
	return ERR_TCPIP_OK;
}

VOID udp_disconnect(struct udp_pcb *pcb)
{
	/* reset remote address association */
	ip_addr_set(&pcb->remote_ip, IP_ADDR_ANY);
	pcb->remote_port = 0;
	/* mark PCB as unconnected */
	pcb->flags &= ~UDP_FLAGS_CONNECTED;
}

VOID udp_recv(struct udp_pcb *pcb, VOID (*recv) (VOID *arg, struct udp_pcb * upcb, struct pbuf * p, struct ip_addr * addr, UINT16 port), VOID *recv_arg)
{
	/* remember recv() callback and user data */
	pcb->recv = recv;
	pcb->recv_arg = recv_arg;
}

/**
 * Remove an UDP PCB.
 *
 * @param pcb UDP PCB to be removed. The PCB is removed from the list of
 * UDP PCB's and the data structure is freed from memory.
 *
 * @see udp_new()
 */
VOID udp_remove(struct udp_pcb *pcb)
{
	struct udp_pcb *pcb2;
	/* pcb to be removed is first in list? */
	if (udp_pcbs == pcb)
	{
		/* make list start at 2nd pcb */
		udp_pcbs = udp_pcbs->next;
		/* pcb not 1st in list */
	}
	else
		for (pcb2 = udp_pcbs; pcb2 != NULL; pcb2 = pcb2->next)
		{
			/* find pcb in udp_pcbs list */
			if (pcb2->next != NULL && pcb2->next == pcb)
			{
				/* remove pcb from list */
				pcb2->next = pcb->next;
			}
		}
	memp_free(MEMP_UDP_PCB, pcb);
}

/**
 * Create a UDP PCB.
 *
 * @return The UDP PCB which was created. NULL if the PCB data structure
 * could not be allocated.
 *
 * @see udp_remove()
 */
struct udp_pcb *udp_new(VOID)
{
	struct udp_pcb *pcb;
	pcb = memp_malloc(MEMP_UDP_PCB);
	/* could allocate UDP PCB? */
	if (pcb != NULL)
	{
		/* initialize PCB to all zeroes */
		SUL_MemSet8(pcb, 0, sizeof(struct udp_pcb));
		pcb->ttl = UDP_TTL;
	}

	return pcb;
}

#if UDP_DEBUG
VOID udp_debug_print(struct udp_hdr *udphdr)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("UDP header:\n"));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("+-------------------------------+\n"));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("|     %5d     |     %5d     | (src port, dest port)\n"), ntohs(udphdr->src), ntohs(udphdr->dest));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("+-------------------------------+\n"));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("|     %5d     |     0x%04x    | (len, chksum)\n"), ntohs(udphdr->len), ntohs(udphdr->chksum));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("+-------------------------------+\n"));
}
#endif /* UDP_DEBUG */

#endif /* CII_UDP */
#endif

