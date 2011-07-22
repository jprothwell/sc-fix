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
#ifndef __LWIP_ICMP_H__
#define __LWIP_ICMP_H__

#include "tcpip_arch.h"

#include "tcpip_opt.h"
#include "tcpip_pbuf.h"

#include "tcpip_ip_addr.h"
#include "tcpip_netif.h"

#define ICMP_ER 0        /* echo reply */
#define ICMP_DUR 3        /* destination unreachable */
#define ICMP_SQ 4        /* source quench */
#define ICMP_RD 5        /* redirect */
#define ICMP_ECHO 8        /* echo */
#define ICMP_TE 11        /* time exceeded */
#define ICMP_PP 12        /* parameter problem */
#define ICMP_TS 13        /* timestamp */
#define ICMP_TSR 14        /* timestamp reply */
#define ICMP_IRQ 15        /* information request */
#define ICMP_IR 16        /* information reply */


#define ICMP_DUR_NET 0    /* net unreachable */
#define ICMP_DUR_HOST 1    /* host unreachable */
#define ICMP_DUR_PROTO 2    /* protocol unreachable */
#define ICMP_DUR_PORT 3    /* port unreachable */
#define ICMP_DUR_FRAG 4    /* fragmentation needed and DF set */
#define ICMP_DUR_SR 5        /* source route failed */



#define ICMP_TE_TTL 0    /* time to live exceeded in transit */
#define ICMP_TE_FRAG 1    /* fragment reassembly time exceeded */


VOID icmp_input(struct pbuf *p, struct netif *inp);

VOID icmp_dest_unreach(struct pbuf *p, UINT8 t);
VOID icmp_time_exceeded(struct pbuf *p, UINT8 t);

#ifdef PACK_STRUCT_USE_INCLUDES
//#  include "arch/bpstruct.h"
#endif

#ifndef CII_VERSION
	PACK_STRUCT_BEGIN struct icmp_echo_hdr
	{
	    PACK_STRUCT_FIELD(UINT16 _type_code);
	    PACK_STRUCT_FIELD(UINT16 chksum);
	    PACK_STRUCT_FIELD(UINT16 id);
	    PACK_STRUCT_FIELD(UINT16 seqno);
	} PACK_STRUCT_STRUCT;
	PACK_STRUCT_END 
	PACK_STRUCT_BEGIN struct icmp_dur_hdr
	{
	    PACK_STRUCT_FIELD(UINT16 _type_code);
	    PACK_STRUCT_FIELD(UINT16 chksum);
	    PACK_STRUCT_FIELD(UINT32 unused);
	} PACK_STRUCT_STRUCT;
	PACK_STRUCT_END 
	PACK_STRUCT_BEGIN struct icmp_te_hdr
	{
	    PACK_STRUCT_FIELD(UINT16 _type_code);
	    PACK_STRUCT_FIELD(UINT16 chksum);
	    PACK_STRUCT_FIELD(UINT32 unused);
	} PACK_STRUCT_STRUCT;
	PACK_STRUCT_END

#else

	struct icmp_echo_hdr
	{
	    UINT16 _type_code;
	    UINT16 chksum;
	    UINT16 id;
	    UINT16 seqno;
	} ;
	 
	 struct icmp_dur_hdr
	{
	    UINT16 _type_code;
	    UINT16 chksum;
	   UINT32 unused;
	} ;
	 
	 struct icmp_te_hdr
	{
	    UINT16 _type_code;
	    UINT16 chksum;
	    UINT32 unused;
	} ;
#endif


#ifdef PACK_STRUCT_USE_INCLUDES
//#  include "arch/epstruct.h"
#endif
#define ICMPH_TYPE(hdr) (ntohs((hdr)->_type_code) >> 8)
#define ICMPH_CODE(hdr) (ntohs((hdr)->_type_code) & 0xff)
#define ICMPH_TYPE_SET(hdr, type) ((hdr)->_type_code = htons(ICMPH_CODE(hdr) | ((type) << 8)))
#define ICMPH_CODE_SET(hdr, code) ((hdr)->_type_code = htons((code) | (ICMPH_TYPE(hdr) << 8)))
#endif /* __LWIP_ICMP_H__ */

#endif

