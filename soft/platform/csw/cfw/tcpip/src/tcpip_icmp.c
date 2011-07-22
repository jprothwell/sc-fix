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

/* Some ICMP messages should be passed to the transport protocols. This
   is not implemented. */

//#include <string.h>

#ifdef CFW_TCPIP_SUPPORT
//#include "tcpip_opt.h"
#include "tcpip_stats.h"
#include <csw.h>
#include "tcpip_icmp.h"
#include "tcpip_inet.h"
#include "tcpip_ip.h"
//#include "tcpip_def.h"
#include "tcpip_snmp.h"
//#include "sul.h"
//#include "ts.h"
VOID icmp_input(struct pbuf *p, struct netif *inp)
{
    UINT8 type;
    UINT8 code;
    struct icmp_echo_hdr *iecho;
    struct ip_hdr *iphdr;
    struct ip_addr tmpaddr;
    UINT16 hlen;

    ICMP_STATS_INC(icmp.recv);
    snmp_inc_icmpinmsgs();

    iphdr = p->payload;
    hlen = IPH_HL(iphdr) * 4;
    if (pbuf_header(p, -((INT16) hlen)) || (p->tot_len < sizeof(UINT16) * 2))
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("icmp_input: short ICMP (0x%x bytes) received\n"), p->tot_len);
        pbuf_free(p);
        ICMP_STATS_INC(icmp.lenerr);
        snmp_inc_icmpinerrors();
        return;
    }

    type = *((UINT8 *) p->payload);
    code = *(((UINT8 *) p->payload) + 1);
    switch (type)
    {
    case ICMP_ECHO:
        /* broadcast or multicast destination address? */
        if (ip_addr_isbroadcast(&iphdr->dest, &(inp->ip_addr)) || ip_addr_ismulticast(&iphdr->dest))
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("icmp_input: Not echoing to multicast or broadcast pings\n"));
            ICMP_STATS_INC(icmp.err);
            pbuf_free(p);
            return;
        }
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("icmp_input: ping\n"));
        if (p->tot_len < sizeof(struct icmp_echo_hdr))
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("icmp_input: bad ICMP echo received\n"));
            pbuf_free(p);
            ICMP_STATS_INC(icmp.lenerr);
            snmp_inc_icmpinerrors();

            return;
        }
        iecho = p->payload;
        if (inet_chksum_pbuf(p) != 0)
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("icmp_input: checksum failed for received ICMP echo\n"));
            pbuf_free(p);
            ICMP_STATS_INC(icmp.chkerr);
            snmp_inc_icmpinerrors();
            return;
        }
        tmpaddr.addr = iphdr->src.addr;
        iphdr->src.addr = iphdr->dest.addr;
        iphdr->dest.addr = tmpaddr.addr;
        ICMPH_TYPE_SET(iecho, ICMP_ER);
        /* adjust the checksum */
        if (iecho->chksum >= htons(0xffff - (ICMP_ECHO << 8)))
        {
            iecho->chksum += htons(ICMP_ECHO << 8) + 1;
        }
        else
        {
            iecho->chksum += htons(ICMP_ECHO << 8);
        }
        ICMP_STATS_INC(icmp.xmit);
        /* increase number of messages attempted to send */
        snmp_inc_icmpoutmsgs();
        /* increase number of echo replies attempted to send */
        snmp_inc_icmpoutechoreps();

        pbuf_header(p, hlen);
        ip_output_if(p, &(iphdr->src), IP_HDRINCL, IPH_TTL(iphdr), 0, IP_PROTO_ICMP, inp);
        break;
    default:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("icmp_input: ICMP type 0x%x code 0x%x not supported.\n"), (INT16) type, (INT16) code);
        ICMP_STATS_INC(icmp.proterr);
        ICMP_STATS_INC(icmp.drop);
    }
    pbuf_free(p);
}

VOID icmp_dest_unreach(struct pbuf *p, UINT8 t)
{
    struct pbuf *q;
    struct ip_hdr *iphdr;
    struct icmp_dur_hdr *idur;

    q = pbuf_alloc(PBUF_IP, 8 + IP_HLEN + 8, PBUF_RAM);
    /* ICMP header + IP header + 8 bytes of data */

    iphdr = p->payload;

    idur = q->payload;
    ICMPH_TYPE_SET(idur, ICMP_DUR);
    ICMPH_CODE_SET(idur, t);

    SUL_MemCopy8((UINT8 *) q->payload + 8, p->payload, IP_HLEN + 8);

    /* calculate checksum */
    idur->chksum = 0;
    idur->chksum = inet_chksum(idur, q->len);
    ICMP_STATS_INC(icmp.xmit);
    /* increase number of messages attempted to send */
    snmp_inc_icmpoutmsgs();
    /* increase number of destination unreachable messages attempted to send */
    snmp_inc_icmpoutdestunreachs();

    ip_output(q, NULL, &(iphdr->src), ICMP_TTL, 0, IP_PROTO_ICMP);
    pbuf_free(q);
}

#if IP_FORWARD
VOID icmp_time_exceeded(struct pbuf *p, UINT8 t)
{
    struct pbuf *q;
    struct ip_hdr *iphdr;
    struct icmp_te_hdr *tehdr;

    q = pbuf_alloc(PBUF_IP, 8 + IP_HLEN + 8, PBUF_RAM);

    iphdr = p->payload;
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("icmp_time_exceeded from "));
    ip_addr_debug_print(ICMP_DEBUG, &(iphdr->src));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" to "));
    ip_addr_debug_print(ICMP_DEBUG, &(iphdr->dest));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("\n"));

    tehdr = q->payload;
    ICMPH_TYPE_SET(tehdr, ICMP_TE);
    ICMPH_CODE_SET(tehdr, t);

    /* copy fields from original packet */
    SUL_MemCopy8((UINT8 *) q->payload + 8, (UINT8 *) p->payload, IP_HLEN + 8);

    /* calculate checksum */
    tehdr->chksum = 0;
    tehdr->chksum = inet_chksum(tehdr, q->len);
    ICMP_STATS_INC(icmp.xmit);
    /* increase number of messages attempted to send */
    snmp_inc_icmpoutmsgs();
    /* increase number of destination unreachable messages attempted to send */
    snmp_inc_icmpouttimeexcds();
    ip_output(q, NULL, &(iphdr->src), ICMP_TTL, 0, IP_PROTO_ICMP);
    pbuf_free(q);
}

#endif /* IP_FORWARD */
#endif


