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
#ifndef __LWIP_IP_H__
#define __LWIP_IP_H__

#include "tcpip_arch.h"

#include "tcpip_def.h"
#include "tcpip_pbuf.h"
#include "tcpip_ip_addr.h"

#include "tcpip_err.h"

VOID ip_init(VOID);
struct netif *ip_route(struct ip_addr *dest,struct ip_addr *src);
err_t ip_input(struct pbuf *p, struct netif *inp);
err_t ip_output(struct pbuf *p, struct ip_addr *src, struct ip_addr *dest, UINT8 ttl, UINT8 tos, UINT8 proto);
err_t ip_output_if(struct pbuf *p, struct ip_addr *src, struct ip_addr *dest, UINT8 ttl, UINT8 tos, UINT8 proto, struct netif *netif);

#define IP_HLEN 20

#define IP_PROTO_ICMP 1
#define IP_PROTO_UDP 17
#define IP_PROTO_UDPLITE 136
#define IP_PROTO_TCP 6

/* This is passed as the destination address to ip_output_if (not
   to ip_output), meaning that an IP header already is constructed
   in the pbuf. This is used when TCP retransmits. */
#ifdef IP_HDRINCL
#undef IP_HDRINCL
#endif /* IP_HDRINCL */
#define IP_HDRINCL  NULL

/* This is the common part of all PCB types. It needs to be at the
   beginning of a PCB type definition. It is located here so that
   changes to this common part are made in one location instead of
   having to change all PCB structs. */
#define IP_PCB struct ip_addr local_ip; \
  struct ip_addr remote_ip; \
   /* Socket options */  \
  UINT16 so_options;      \
   /* Type Of Service */ \
  UINT8 tos;              \
  /* Time To Live */     \
  UINT8 ttl

/*
 * Option flags per-socket. These are the same like SO_XXX.
 */
#define	SOF_DEBUG	    (UINT16)0x0001U	/* turn on debugging info recording */
#define	SOF_ACCEPTCONN	(UINT16)0x0002U	/* socket has had listen() */
#define	SOF_REUSEADDR	(UINT16)0x0004U	/* allow local address reuse */
#define	SOF_KEEPALIVE	(UINT16)0x0008U	/* keep connections alive */
#define	SOF_DONTROUTE	(UINT16)0x0010U	/* just use interface addresses */
#define	SOF_BROADCAST	(UINT16)0x0020U	/* permit sending of broadcast msgs */
#define	SOF_USELOOPBACK	(UINT16)0x0040U	/* bypass hardware when possible */
#define	SOF_LINGER	    (UINT16)0x0080U	/* linger on close if data present */
#define	SOF_OOBINLINE	(UINT16)0x0100U	/* leave received OOB data in line */
#define	SOF_REUSEPORT	(UINT16)0x0200U	/* allow local address & port reuse */

#ifdef PACK_STRUCT_USE_INCLUDES
//#  include "arch/bpstruct.h"
#endif


#ifndef 	CII_VERSION

PACK_STRUCT_BEGIN struct ip_hdr
{
	/* version / header length / type of service */
	PACK_STRUCT_FIELD(UINT16 _v_hl_tos);
	/* total length */
	PACK_STRUCT_FIELD(UINT16 _len);
	/* identification */
	PACK_STRUCT_FIELD(UINT16 _id);
	/* fragment offset field */
	PACK_STRUCT_FIELD(UINT16 _offset);
#define IP_RF 0x8000		/* reserved fragment flag */
#define IP_DF 0x4000		/* dont fragment flag */
#define IP_MF 0x2000		/* more fragments flag */
#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
	/* time to live / protocol */
	PACK_STRUCT_FIELD(UINT16 _ttl_proto);
	/* checksum */
	PACK_STRUCT_FIELD(UINT16 _chksum);
	/* source and destination IP addresses */
	PACK_STRUCT_FIELD(struct ip_addr src);
	  PACK_STRUCT_FIELD(struct ip_addr dest);
} PACK_STRUCT_STRUCT;

#else
struct ip_hdr
{
	/* version / header length / type of service */
	UINT16 _v_hl_tos;
	/* total length */
	UINT16 _len;
	/* identification */
	UINT16 _id;
	/* fragment offset field */
	UINT16 _offset;
#define IP_RF 0x8000		/* reserved fragment flag */
#define IP_DF 0x4000		/* dont fragment flag */
#define IP_MF 0x2000		/* more fragments flag */
#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
	/* time to live / protocol */
	UINT16 _ttl_proto;
	/* checksum */
	UINT16 _chksum;
	/* source and destination IP addresses */
	struct ip_addr src;
struct ip_addr dest;
} ;
#endif

#ifndef 	CII_VERSION
PACK_STRUCT_END
#endif

#ifdef PACK_STRUCT_USE_INCLUDES
//#  include "arch/epstruct.h"
#endif
#define IPH_V(hdr)  (ntohs((hdr)->_v_hl_tos) >> 12)
#define IPH_HL(hdr) ((ntohs((hdr)->_v_hl_tos) >> 8) & 0x0f)
#define IPH_TOS(hdr) (ntohs((hdr)->_v_hl_tos) & 0xff)
#define IPH_LEN(hdr) ((hdr)->_len)
#define IPH_ID(hdr) ((hdr)->_id)
#define IPH_OFFSET(hdr) ((hdr)->_offset)
#define IPH_TTL(hdr) (ntohs((hdr)->_ttl_proto) >> 8)
#define IPH_PROTO(hdr) (ntohs((hdr)->_ttl_proto) & 0xff)
#define IPH_CHKSUM(hdr) ((hdr)->_chksum)
#define IPH_VHLTOS_SET(hdr, v, hl, tos) (hdr)->_v_hl_tos = (htons(((v) << 12) | ((hl) << 8) | (tos)))
#define IPH_LEN_SET(hdr, len) (hdr)->_len = (len)
#define IPH_ID_SET(hdr, id) (hdr)->_id = (id)
#define IPH_OFFSET_SET(hdr, off) (hdr)->_offset = (off)
#define IPH_TTL_SET(hdr, ttl) (hdr)->_ttl_proto = (htons(IPH_PROTO(hdr) | ((ttl) << 8)))
#define IPH_PROTO_SET(hdr, proto) (hdr)->_ttl_proto = (htons((proto) | (IPH_TTL(hdr) << 8)))
#define IPH_CHKSUM_SET(hdr, chksum) (hdr)->_chksum = (chksum)
#if IP_DEBUG
VOID ip_debug_print(struct pbuf *p);
#else
#define ip_debug_print(p)
#endif /* IP_DEBUG */
#endif /* __LWIP_IP_H__ */
#endif

