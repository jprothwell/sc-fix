
/* @file
 *
 * This is the IP layer implementation for incoming and outgoing IP traffic.
 * 
 * @see ip_frag.c
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

#ifdef CFW_TCPIP_SUPPORT
#include "tcpip_opt.h"
#include <csw.h>

#include "tcpip_def.h"
#include "tcpip_mem.h"
#include "tcpip_ip.h"
#include "tcpip_ip_frag.h"
#include "tcpip_inet.h"
#include "tcpip_ip_addr.h"
#include "tcpip_icmp.h"
#include "tcpip_raw.h"
#include "tcpip_udp.h"
#include "tcpip_tcp.h"

#include "tcpip_stats.h"
#include "tcpip.h"

#include "tcpip_snmp.h"
#if LWIP_DHCP
#  include "dhcp.h"
#endif /* LWIP_DHCP */

/**
 * Initializes the IP layer.
 */
VOID ip_init(VOID)
{
	/* no initializations as of yet */
}

/**
 * Finds the appropriate network interface for a given IP address. It
 * searches the list of network interfaces linearly. A match is found
 * if the masked IP address of the network interface equals the masked
 * IP address given to the function.
 */

struct netif *ip_route(struct ip_addr *dest,struct ip_addr *src)
{
#if 1
  //      CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("ip_route: 1  .\n"));

	struct netif *netif;

	/* iterate through netifs */
	for (netif = netif_list; netif != NULL; netif = netif->next)
	{
		/* network mask matches? */
		if (ip_addr_netcmp(dest, &(netif->ip_addr), &(netif->netmask)))
		{
			/* return netif on which to forward IP packet */
			return netif;
		}
		
	}
	 //       CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("ip_route: 2  .\n"));

  /******************///test multi APN
  if(src !=NULL)
  	{
	for (netif = netif_list; netif != NULL; netif = netif->next)
	{
	//	 CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("!!!!!!!!!ip_route: src: 0x%x;   netif: 0x%x \n"),src->addr, netif->ip_addr.addr);

  		if (ip_addr_cmp(src, &(netif->ip_addr)))
  		{
  		
  			  return netif;
  		}
	}
  	}
	  //      CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("ip_route: 3  .\n"));

	/****************************/
	return netif_default;
#else
	
	if(g_ip_addr.addr == dest -> addr )
    {
        return &g_ip_addr;
    }
    else
    {
        return &g_ip_addr;
    }
#endif
}

#if IP_FORWARD

/**
 * Forwards an IP packet. It finds an appropriate route for the
 * packet, decrements the TTL value of the packet, adjusts the
 * checksum and outputs the packet on the appropriate interface.
 */

static struct ip_addr *ip_forward(struct pbuf *p, struct ip_hdr *iphdr, struct netif *inp)
{
	struct netif *netif;

	PERF_START;
	/* Find network interface where to forward this IP packet to. */
	netif = ip_route((struct ip_addr *) &(iphdr->dest));
	if (netif == NULL)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("ip_forward: no forwarding route for 0x%x found\n"), iphdr->dest.addr);
		snmp_inc_ipnoroutes();
		return (struct ip_addr *) NULL;
	}
	/* Do not forward packets onto the same network interface on which
	 * they arrived. */
	if (netif == inp)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("ip_forward: not bouncing packets back on incoming interface.\n"));
		snmp_inc_ipnoroutes();
		return (struct ip_addr *) NULL;
	}

	/* decrement TTL */
	IPH_TTL_SET(iphdr, IPH_TTL(iphdr) - 1);
	/* send ICMP if TTL == 0 */
	if (IPH_TTL(iphdr) == 0)
	{
		/* Don't send ICMP messages in response to ICMP messages */
		if (IPH_PROTO(iphdr) != IP_PROTO_ICMP)
		{
			icmp_time_exceeded(p, ICMP_TE_TTL);
			snmp_inc_icmpouttimeexcds();
		}
		return (struct ip_addr *) NULL;
	}

	/* Incrementally update the IP checksum. */
	if (IPH_CHKSUM(iphdr) >= htons(0xffff - 0x100))
	{
		IPH_CHKSUM_SET(iphdr, IPH_CHKSUM(iphdr) + htons(0x100) + 1);
	}
	else
	{
		IPH_CHKSUM_SET(iphdr, IPH_CHKSUM(iphdr) + htons(0x100));
	}

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("ip_forward: forwarding packet to 0x%x\n"), iphdr->dest.addr);

	IP_STATS_INC(ip.fw);
	IP_STATS_INC(ip.xmit);
	snmp_inc_ipforwdatagrams();

	PERF_STOP("ip_forward");
	/* transmit pbuf on chosen interface */
	netif->output(netif, p, (struct ip_addr *) &(iphdr->dest));
	return netif;
}
#endif /* IP_FORWARD */

/**
 * This function is called by the network interface device driver when
 * an IP packet is received. The function does the basic checks of the
 * IP header such as packet size being at least larger than the header
 * size etc. If the packet was not destined for us, the packet is
 * forwarded (using ip_forward). The IP checksum is always checked.
 *
 * Finally, the packet is sent to the upper layer protocol input function.
 * 
 * 
 * 
 */

err_t ip_input(struct pbuf * p, struct netif * inp)
{
	struct ip_hdr *iphdr;
	struct netif *netif=NULL;
	UINT16 iphdrlen;

	//IP_STATS_INC(ip.recv);
	//snmp_inc_ipinreceives();

	/* identify the IP header */
	iphdr = p->payload;
	if (IPH_V(iphdr) != 4)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("IP packet dropped due to bad version number 0x%x\n"), IPH_V(iphdr));
		ip_debug_print(p);
		pbuf_free(p);
		//IP_STATS_INC(ip.err);
		//IP_STATS_INC(ip.drop);
		snmp_inc_ipunknownprotos();
		return ERR_TCPIP_OK;
	}
	/* obtain IP header length in number of 32-bit words */
	iphdrlen = IPH_HL(iphdr);
	/* calculate IP header length in bytes */
	iphdrlen *= 4;

	/* header length exceeds first pbuf length? */
	if (iphdrlen > p->len)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("IP header (len 0x%x) does not fit in first pbuf (len 0x%x), IP packet droppped.\n"), iphdrlen, p->len);
		/* free (drop) packet pbufs */
		pbuf_free(p);
		//IP_STATS_INC(ip.lenerr);
		//IP_STATS_INC(ip.drop);
		snmp_inc_ipindiscards();
		return ERR_TCPIP_OK;
	}

	/* verify checksum */
#if CHECKSUM_CHECK_IP
	if (inet_chksum(iphdr, iphdrlen) != 0)
	{

		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("Checksum (0x%x) failed, IP packet dropped.\n"), inet_chksum(iphdr, iphdrlen));
		ip_debug_print(p);
		pbuf_free(p);
		//IP_STATS_INC(ip.chkerr);
		//IP_STATS_INC(ip.drop);
		snmp_inc_ipindiscards();
		return ERR_TCPIP_OK;
	}
#endif

	/* Trim pbuf. This should have been done at the netif layer,
	 * but we'll do it anyway just to be sure that its done. */
	pbuf_realloc(p, ntohs(IPH_LEN(iphdr)));

	/* match packet against an interface, i.e. is this packet for us? */

	for (netif = netif_list; netif != NULL; netif = netif->next)
	{

		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("ip_input: iphdr->dest 0x%xnetif->ip_addr 0x%x (0x%x, 0x%x, 0x%x)\n"),
				        iphdr->dest.addr, netif->ip_addr.addr, iphdr->dest.addr & netif->netmask.addr, netif->ip_addr.addr & netif->netmask.addr,  iphdr->dest.addr & ~(netif->netmask.addr));

		/* interface is up and configured? */
		if ((netif_is_up(netif)) && (!ip_addr_isany(&(netif->ip_addr))))
		{
			/* unicast to this interface address? */
			if (ip_addr_cmp(&(iphdr->dest), &(netif->ip_addr)) ||
			    /* or broadcast on this interface network address? */
			    ip_addr_isbroadcast(&(iphdr->dest), &(netif->ip_addr)))
			{
				CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("ip_input: packet accepted on interface %c%c\n"), netif->name[0], netif->name[1]);
				/* break out of for loop */
				break;
			}
		}
	}
#if 0
    if( inp == &g_ip_addr)
    {
        netif = &g_ip_addr;
    }
    else
    {
        netif = NULL ;
    }
#endif
#if LWIP_DHCP
	/* Pass DHCP messages regardless of destination address. DHCP traffic is addressed
	 * using link layer addressing (such as Ethernet MAC) so we must not filter on IP.
	 * According to RFC 1542 section 3.1.1, referred by RFC 2131).
	 */
	if (netif == NULL)
	{
		/* remote port is DHCP server? */
		if (IPH_PROTO(iphdr) == IP_PROTO_UDP)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ip_input: UDP packet to DHCP client port 0x%x\n"), ntohs(((struct udp_hdr *) ((UINT8 *) iphdr + iphdrlen))->dest));
			if (ntohs(((struct udp_hdr *) ((UINT8 *) iphdr + iphdrlen))->dest) == DHCP_CLIENT_PORT)
			{
				CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ip_input: DHCP packet accepted.\n"));
				netif = inp;
			}
		}
	}
#endif /* LWIP_DHCP */
	/* packet not for us? */
	if (netif == NULL)
	{
		/* packet not for us, route or discard */
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ip_input: packet not for us.\n"));
#if IP_FORWARD
		/* non-broadcast packet? */
		if (!ip_addr_isbroadcast(&(iphdr->dest), inp->ip_addr))
		{
			/* try to forward IP packet on (other) interfaces */
			ip_forward(p, iphdr, inp);
		}
		else
#endif /* IP_FORWARD */
		{
			snmp_inc_ipindiscards();
		}
		pbuf_free(p);
		return ERR_TCPIP_OK;
	}
	/* packet consists of multiple fragments? */
	if ((IPH_OFFSET(iphdr) & htons(IP_OFFMASK | IP_MF)) != 0)
	{
#if IP_REASSEMBLY		/* packet fragment reassembly code present? */
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("IP packet is a fragment (id=0x%04x tot_len=0x%x len=0x%x MF=0x%x offset=0x%x), calling ip_reass()\n"),
				       ntohs(IPH_ID(iphdr)), p->tot_len, ntohs(IPH_LEN(iphdr)), !!(IPH_OFFSET(iphdr) & htons(IP_MF)), (ntohs(IPH_OFFSET(iphdr)) & IP_OFFMASK) * 8);
		/* reassemble the packet */
		p = ip_reass(p);
		/* packet not fully reassembled yet? */
		if (p == NULL)
		{
			return ERR_TCPIP_OK;
		}
		iphdr = p->payload;
#else /* IP_REASSEMBLY == 0, no packet fragment reassembly code present */
		pbuf_free(p);
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("IP packet dropped since it was fragmented (0x%x) (while IP_REASSEMBLY == 0).\n"), ntohs(IPH_OFFSET(iphdr)));
		//IP_STATS_INC(ip.opterr);
		//IP_STATS_INC(ip.drop);
		snmp_inc_ipunknownprotos();
		return ERR_TCPIP_OK;
#endif /* IP_REASSEMBLY */
	}

#if IP_OPTIONS == 0		/* no support for IP options in the IP header? */
	if (iphdrlen > IP_HLEN)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("IP packet dropped since there were IP options (while IP_OPTIONS == 0).\n"));
		pbuf_free(p);
		//IP_STATS_INC(ip.opterr);
		//IP_STATS_INC(ip.drop);
		snmp_inc_ipunknownprotos();
		return ERR_TCPIP_OK;
	}
#endif /* IP_OPTIONS == 0 */

	/* send to upper layers */
//	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("ip_input: \n"));
//	ip_debug_print(p);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("ip_input: p->len 0x%x p->tot_len 0x%x\n"), p->len, p->tot_len);

#if LWIP_RAW
	/* raw input did not eat the packet? */
	if (raw_input(p, inp) == 0)
	{
#endif /* LWIP_RAW */

		switch (IPH_PROTO(iphdr))
		{
#if CII_UDP
		case IP_PROTO_UDP:
		case IP_PROTO_UDPLITE:
			snmp_inc_ipindelivers();
			udp_input(p, inp);
			break;
#endif /* CII_UDP */
#if CII_TCP
		case IP_PROTO_TCP:
			snmp_inc_ipindelivers();
			tcp_input(p, inp);
			break;
#endif /* CII_TCP */
		case IP_PROTO_ICMP:
			snmp_inc_ipindelivers();
			icmp_input(p, inp);
			break;
		default:
			/* send ICMP destination protocol unreachable unless is was a broadcast */
			if (!ip_addr_isbroadcast(&(iphdr->dest), &(inp->ip_addr)) && !ip_addr_ismulticast(&(iphdr->dest)))
			{
				p->payload = iphdr;
				icmp_dest_unreach(p, ICMP_DUR_PROTO);
			}
			pbuf_free(p);

			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("Unsupported transport protocol 0x%x\n"), IPH_PROTO(iphdr));

			IP_STATS_INC(ip.proterr);
			IP_STATS_INC(ip.drop);
			snmp_inc_ipunknownprotos();
		}
#if LWIP_RAW
	}			/* LWIP_RAW */
#endif
	return ERR_TCPIP_OK;
}

/**
 * Sends an IP packet on a network interface. This function constructs
 * the IP header and calculates the IP header checksum. If the source
 * IP address is NULL, the IP address of the outgoing network
 * interface is filled in as source address.
 */

err_t ip_output_if(struct pbuf * p, struct ip_addr * src, struct ip_addr * dest, UINT8 ttl, UINT8 tos, UINT8 proto, struct netif * netif)
{
	struct ip_hdr *iphdr;
	static UINT16 ip_id = 0;
CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ip_output_if begin\n"));

	snmp_inc_ipoutrequests();

	if (dest != IP_HDRINCL)
	{
		if (pbuf_header(p, IP_HLEN))
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ip_output: not enough room for IP header in pbuf\n"));

			IP_STATS_INC(ip.err);
			snmp_inc_ipoutdiscards();
			return ERR_TCPIP_BUF;
		}

		iphdr = p->payload;

		IPH_TTL_SET(iphdr, ttl);
		IPH_PROTO_SET(iphdr, proto);

		ip_addr_set(&(iphdr->dest), dest);

		IPH_VHLTOS_SET(iphdr, 4, IP_HLEN / 4, tos);
		IPH_LEN_SET(iphdr, htons(p->tot_len));
		IPH_OFFSET_SET(iphdr, htons(IP_DF));
		IPH_ID_SET(iphdr, htons(ip_id));
		++ip_id;
//modify by wuys 2010-05-13
#if 1
		if (src == NULL)
		{
			ip_addr_set(&(iphdr->src),&(netif->ip_addr));
		}
		else if(src->addr ==0)
		{
			ip_addr_set(&(iphdr->src),&(netif->ip_addr));

		}else
		{
			ip_addr_set(&(iphdr->src), src);
		}

#else
		if (ip_addr_isany(src))
		{
			ip_addr_set(&(iphdr->src),&(netif->ip_addr));
		}
		else
		{
			ip_addr_set(&(iphdr->src), src);
		}
#endif
		IPH_CHKSUM_SET(iphdr, 0);
#if CHECKSUM_GEN_IP
		IPH_CHKSUM_SET(iphdr, inet_chksum(iphdr, IP_HLEN));
#endif
	}
	else
	{
		iphdr = p->payload;
		dest = &(iphdr->dest);
	}

#if IP_FRAG
	/* don't fragment if interface has mtu set to 0 [loopif] */
//	if (netif->mtu && (p->tot_len > netif->mtu))
//		return ip_frag(p, netif, dest);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("########ip_output_if, netif->mtu:%d,  p->tot_len :%d"),netif->mtu,p->tot_len );

	if (netif->mtu && (p->tot_len > netif->mtu))
	{
		
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("########ip_output_if, need to fragment "));
		return ip_frag(p, netif, dest);
	}else{
		
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("########ip_output_if, need not fragment "));
	}
#endif

	IP_STATS_INC(ip.xmit);

	//CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("ip_output_if: %x%c0x%x\n"), netif->name[0], netif->name[1], netif->num);
	ip_debug_print(p);

	//return netif->output(netif, p, dest);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("ip_output_if, netif: 0x%x, p: 0x%x, dest:0x%x "),netif, p, dest);
	return data_output(netif, p, dest);
}

/**
 * Simple interface to ip_output_if. It finds the outgoing network
 * interface and calls upon ip_output_if to do the actual work.
 */

err_t ip_output(struct pbuf * p, struct ip_addr * src, struct ip_addr * dest, UINT8 ttl, UINT8 tos, UINT8 proto)
{
	if(src!=NULL&&dest!=NULL)
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ip_output: begin src IP: 0x%x; dest IP: 0x%x\n"),src->addr,dest->addr);
	else
	      	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ip_output: begin src IP: NULL or dest IP:NULL\n"));

	struct netif *netif;
	if ((netif = ip_route(dest,src)) == NULL)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ip_output: No route to 0x%x\n"), dest->addr);

		IP_STATS_INC(ip.rterr);
		snmp_inc_ipoutdiscards();
		return ERR_TCPIP_RTE;
	}
	if(netif !=NULL)
           CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("ip_output: cid 0x%x\n"), netif->lowaddr.cid);
      else
	    CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("netif: NULL\n"));

	return ip_output_if(p, src, dest, ttl, tos, proto, netif);
}

#if IP_DEBUG
VOID ip_debug_print(struct pbuf *p)
{
	struct ip_hdr *iphdr = p->payload;
	UINT8 *payload;

	payload = (UINT8 *) iphdr + IP_HLEN;

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("IP header:\n"));
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("+-------------------------------+\n"));
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("|0x%2x |%2x |  0x%02x |     0x%5x     | (v, hl, tos, len)\n"),
		IPH_V(iphdr), IPH_HL(iphdr), IPH_TOS(iphdr), ntohs(IPH_LEN(iphdr)));
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("+-------------------------------+\n"));
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("|    0x%5x      |0x%x0x%x0x%x|    0x%4x   | (id, flags, offset)\n"),
			       ntohs(IPH_ID(iphdr)), ntohs(IPH_OFFSET(iphdr)) >> 15 & 1, 
			       ntohs(IPH_OFFSET(iphdr)) >> 14 & 1, ntohs(IPH_OFFSET(iphdr)) >> 13 & 1,
			       ntohs(IPH_OFFSET(iphdr)) & IP_OFFMASK);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("+-------------------------------+\n"));
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("|  0x%3x  |  0x%3x  |    0x%04x     | (ttl, proto, chksum)\n"), 
		IPH_TTL(iphdr), IPH_PROTO(iphdr), ntohs(IPH_CHKSUM(iphdr)));
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("+-------------------------------+\n"));
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("|  0x%3x  |  0x%3x  |  0x%3x  |  0x%3x  | (src)\n"), 
		ip4_addr1(&iphdr->src), ip4_addr2(&iphdr->src), ip4_addr3(&iphdr->src), ip4_addr4(&iphdr->src));
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("+-------------------------------+\n"));
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("|  0x%3x  |  0x%3x  |  0x%3x  |  0x%3x  | (dest)\n"),
		ip4_addr1(&iphdr->dest), ip4_addr2(&iphdr->dest), ip4_addr3(&iphdr->dest), ip4_addr4(&iphdr->dest));
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("+-------------------------------+\n"));
}
#endif /* IP_DEBUG */
#endif

