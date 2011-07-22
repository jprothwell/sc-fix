/**
 * @file
 *
 * Transmission Control Protocol, outgoing traffic
 *
 * The output functions of TCP.
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
//#include <cswtype.h>
//#include <string.h>
#ifdef CFW_TCPIP_SUPPORT
#include "tcpip_tcp.h"
#include "tcpip_def.h"
#include "tcpip_memp.h"
#include "tcpip_netif.h"
#include "tcpip_stats.h"

//#include <csw.h>
//#include "tcpip_sys.h"
 
//#include "tcpip_opt.h"
//#include "tcpip_mem.h"
//#include "tcpip_ip_addr.h"
//#include "tcpip_inet.h"

#if CII_TCP

/* Forward declarations.*/
static VOID tcp_output_segment(struct tcp_seg *seg, struct tcp_pcb *pcb);

err_t tcp_send_ctrl(struct tcp_pcb *pcb, UINT8 flags)
{
    /* no data, no length, flags, copy=1, no optdata, no optdatalen */
    return tcp_enqueue(pcb, NULL, 0, flags, 1, NULL, 0);
}

/**
 * Write data for sending (but does not send it immediately).
 *
 * It waits in the expectation of more data being sent soon (as
 * it can send them more efficiently by combining them together).
 * To prompt the system to send data now, call tcp_output() after
 * calling tcp_write().
 * 
 * @arg pcb Protocol control block of the TCP connection to enqueue data for. 
 * 
 * @see tcp_write()
 */

err_t tcp_write(struct tcp_pcb * pcb, const VOID *arg, UINT16 len, UINT8 copy)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_write(pcb=%p, arg=%p, len=0x%x, copy=0x%x)\n"), (VOID *) pcb, arg, len, (UINT16) copy);
    /* connection is in valid state for data transmission? */
    if (pcb->state == ESTABLISHED || pcb->state == CLOSE_WAIT || pcb->state == SYN_SENT || pcb->state == SYN_RCVD)
    {
        if (len > 0)
        {
            return tcp_enqueue(pcb, (VOID *) arg, len, 0, copy, NULL, 0);
        }
        return ERR_TCPIP_OK;
    }
    else
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_write() called in invalid state\n"));
        return ERR_TCPIP_CONN;
    }
}

/**
 * Enqueue either data or TCP options (but not both) for tranmission
 * 
 * 
 * 
 * @arg pcb Protocol control block for the TCP connection to enqueue data for.
 * @arg arg Pointer to the data to be enqueued for sending.
 * @arg len Data length in bytes
 * @arg flags
 * @arg copy 1 if data must be copied, 0 if data is non-volatile and can be
 * referenced.
 * @arg optdata
 * @arg optlen
 */
err_t tcp_enqueue(struct tcp_pcb * pcb, VOID *arg, UINT16 len, UINT8 flags, UINT8 copy, UINT8 * optdata, UINT8 optlen)
{
    struct pbuf *p;
    struct tcp_seg *seg, *useg, *queue;
    UINT32 left, seqno;
    UINT16 seglen;
    VOID *ptr;
    UINT8 queuelen;

    //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue(pcb=%p, arg=%p, len=0x%x, flags=0x%x, copy=0x%x)\n"), (VOID *) pcb, arg, len, (UINT16) flags, (UINT16) copy);
    LWIP_ASSERT("tcp_enqueue: len == 0 || optlen == 0 (programmer violates API)", len == 0 || optlen == 0);
    LWIP_ASSERT("tcp_enqueue: arg == NULL || optdata == NULL (programmer violates API)", arg == NULL || optdata == NULL);
    /* fail on too much data */
    if (len > pcb->snd_buf)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: too much data (len=0x%x > snd_buf=0x%x)\n"), len, pcb->snd_buf);
        return ERR_TCPIP_MEM;
    }
    left = len;
    ptr = arg;

    /* seqno will be the sequence number of the first segment enqueued
     * by the call to this function. */
    seqno = pcb->snd_lbb;

   // CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: queuelen: 0x%x\n"), (UINT16) pcb->snd_queuelen);

    /* If total number of pbufs on the unsent/unacked queues exceeds the
     * configured maximum, return an error */
    queuelen = pcb->snd_queuelen;
    if (queuelen >= TCP_SND_QUEUELEN)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: too long queue 0x%x (max 0x%x)\n"), queuelen, TCP_SND_QUEUELEN);
        TCP_STATS_INC(tcp.memerr);
        return ERR_TCPIP_MEM;
    }
    if (queuelen != 0)
    {
        LWIP_ASSERT("tcp_enqueue: pbufs on queue => at least one queue non-empty", pcb->unacked != NULL || pcb->unsent != NULL);
    }
    else
    {
        LWIP_ASSERT("tcp_enqueue: no pbufs on queue => both queues empty", pcb->unacked == NULL && pcb->unsent == NULL);
    }

    /* First, break up the data into segments and tuck them together in
     * the local "queue" variable. */
    useg = queue = seg = NULL;
    seglen = 0;
    while (queue == NULL || left > 0)
    {

        /* The segment length should be the MSS if the data to be enqueued
         * is larger than the MSS. */
        seglen = left > pcb->mss ? pcb->mss : left;

        /* Allocate memory for tcp_seg, and fill in fields. */
        seg = memp_malloc(MEMP_TCP_SEG);
        if (seg == NULL)
        {          
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: could not allocate memory for tcp_seg\n"));
            
            goto memerr;
        }
        seg->next = NULL;
        seg->p = NULL;
		    seg->dataptr = NULL;
        /* first segment of to-be-queued data? */
        if (queue == NULL)
        {
            queue = seg;
        }
        /* subsequent segments of to-be-queued data */
        else
        {
            /* Attach the segment to the end of the queued segments */
            LWIP_ASSERT("useg != NULL", useg != NULL);
            useg->next = seg;
        }
        /* remember last segment of to-be-queued data for next iteration */
        useg = seg;

        /* If copy is set, memory should be allocated
         * and data copied into pbuf, otherwise data comes from
         * ROM or other static memory, and need not be copied. If
         * optdata is != NULL, we have options instead of data. */

        /* options? */
        if (optdata != NULL)
        {
            if ((seg->p = pbuf_alloc(PBUF_TRANSPORT, optlen, PBUF_RAM)) == NULL)
            {
            	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("goto memerr"));
                goto memerr;
            }			
			      ++queuelen;
            seg->dataptr = seg->p->payload;
        }
        /* copy from volatile memory? */
        else if (copy)
        {
            if ((seg->p = pbuf_alloc(PBUF_TRANSPORT, seglen, PBUF_RAM)) == NULL)
            {
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue : could not allocate memory for pbuf copy size 0x%x\n"), seglen);
                goto memerr;
            }
            ++queuelen;
            if (arg != NULL)
            {
                SUL_MemCopy8(seg->p->payload, ptr, seglen);
            }
            seg->dataptr = seg->p->payload;
        }
        /* do not copy data */
        else
        {
            /* First, allocate a pbuf for holding the data.
             * since the referenced data is available at least until it is sent out on the
             * link (as it has to be ACKed by the remote party) we can safely use PBUF_ROM
             * instead of PBUF_REF here.
             */
            if ((p = pbuf_alloc(PBUF_TRANSPORT, seglen, PBUF_ROM)) == NULL)
            {
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: could not allocate memory for zero-copy pbuf\n"));
                goto memerr;
            }
            ++queuelen;
            /* reference the non-volatile payload data */
            p->payload = ptr;
            seg->dataptr = ptr;

            /* Second, allocate a pbuf for the headers. */
            if ((seg->p = pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_RAM)) == NULL)
            {
                /* If allocation fails, we have to deallocate the data pbuf as
                 * well. */
                pbuf_free(p);
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: could not allocate memory for header pbuf\n"));
                goto memerr;
            }
            ++queuelen;

            /* Concatenate the headers and data pbufs together. */
            pbuf_cat(seg->p /*header */ , p /*data */ );
            p = NULL;
        }

        /* Now that there are more segments queued, we check again if the
           length of the queue exceeds the configured maximum. */
        if (queuelen > TCP_SND_QUEUELEN)
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: queue too long 0x%x (0x%x)\n"), queuelen, TCP_SND_QUEUELEN);
            goto memerr;
        }
        seg->len = seglen;

        /* build TCP header */
        if (pbuf_header(seg->p, TCP_HLEN))
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: no room for TCP header in pbuf.\n"));
            TCP_STATS_INC(tcp.err);
            goto memerr;
        }
        seg->tcphdr = seg->p->payload;
        seg->tcphdr->src = htons(pcb->local_port);
        seg->tcphdr->dest = htons(pcb->remote_port);
        seg->tcphdr->seqno = htonl(seqno);
        seg->tcphdr->urgp = 0;
        TCPH_FLAGS_SET(seg->tcphdr, flags);
        /* don't fill in tcphdr->ackno and tcphdr->wnd until later */

        /* Copy the options into the header, if they are present. */
        if (optdata == NULL)
        {
            TCPH_HDRLEN_SET(seg->tcphdr, 5);
        }
        else
        {
            TCPH_HDRLEN_SET(seg->tcphdr, (5 + optlen / 4));
            /* Copy options into data portion of segment.
               Options can thus only be sent in non data carrying
               segments such as SYN|ACK. */
            SUL_MemCopy8(seg->dataptr, optdata, optlen);
        }
       // CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: queueing 0x%x:0x%x (0x0x%x)\n"), ntohl(seg->tcphdr->seqno), ntohl(seg->tcphdr->seqno) + TCP_TCPLEN(seg), (UINT16) flags);

        left -= seglen;
        seqno += seglen;
        ptr = (VOID *) ((UINT8 *) ptr + seglen);
    }

    /* Now that the data to be enqueued has been broken up into TCP
       segments in the queue variable, we add them to the end of the
       pcb->unsent queue. */
    if (pcb->unsent == NULL)
    {
        useg = NULL;
    }
    else
    {
        for (useg = pcb->unsent; useg->next != NULL; useg = useg->next);
    }
    /* { useg is last segment on the unsent queue, NULL if list is empty } */

    /* If there is room in the last pbuf on the unsent queue,
       chain the first pbuf on the queue together with that. */
    if (useg != NULL && TCP_TCPLEN(useg) != 0 && !(TCPH_FLAGS(useg->tcphdr) & (TCP_SYN | TCP_FIN)) && !(flags & (TCP_SYN | TCP_FIN)) &&
        /* fit within max seg size */
        useg->len + queue->len <= pcb->mss)
    {
        /* Remove TCP header from first segment of our to-be-queued list */
        pbuf_header(queue->p, -TCP_HLEN);
        pbuf_cat(useg->p, queue->p);
        useg->len += queue->len;
        useg->next = queue->next;

        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: chaining segments, new len 0x%x\n"), useg->len);
        if (seg == queue)
        {
            seg = NULL;
        }
        memp_free(MEMP_TCP_SEG, queue);
    }
    else
    {
        /* empty list */
        if (useg == NULL)
        {
            /* initialize list with this segment */
            pcb->unsent = queue;
        }
        /* enqueue segment */
        else
        {
            useg->next = queue;
        }
    }
    if ((flags & TCP_SYN) || (flags & TCP_FIN))
    {
        ++len;
    }
    pcb->snd_lbb += len;

    pcb->snd_buf -= len;

    /* update number of segments on the queues */
    pcb->snd_queuelen = queuelen;
 //   CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: 0x%x (after enqueued)\n"), pcb->snd_queuelen);
    if (pcb->snd_queuelen != 0)
    {
        LWIP_ASSERT("tcp_enqueue: valid queue length", pcb->unacked != NULL || pcb->unsent != NULL);
    }

    /* Set the PSH flag in the last segment that we enqueued, but only
       if the segment has data (indicated by seglen > 0). */
    if (seg != NULL && seglen > 0 && seg->tcphdr != NULL)
    {
        TCPH_SET_FLAG(seg->tcphdr, TCP_PSH);
    }

    return ERR_TCPIP_OK;
      memerr:
    TCP_STATS_INC(tcp.memerr);

    if (queue != NULL)
    {
        tcp_segs_free(queue);
    }
    if (pcb->snd_queuelen != 0)
    {
        LWIP_ASSERT("tcp_enqueue: valid queue length", pcb->unacked != NULL || pcb->unsent != NULL);
    }
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_enqueue: 0x%x (with mem err)\n"), pcb->snd_queuelen);
    return ERR_TCPIP_MEM;
}

/* find out what we can send and send it */
err_t tcp_output(struct tcp_pcb * pcb)
{
    struct pbuf *p;
    struct tcp_hdr *tcphdr;
    struct tcp_seg *seg, *useg;
    UINT32 wnd;

   //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function : tcp_output:\n"));

    /* First, check if we are invoked by the TCP input processing
       code. If so, we do not output anything. Instead, we rely on the
       input processing code to call us when input processing is done
       with. */
    if (tcp_input_pcb == pcb)
    {
      //  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_output: tcp_input_pcb == pcb return;\n"));    
        return ERR_TCPIP_OK;
    }

    wnd = LWIP_MIN(pcb->snd_wnd, pcb->cwnd);

    seg = pcb->unsent;
//DM_BuzzerVibratingOn();
    /* useg should point to last segment on unacked queue */
    useg = pcb->unacked;
    if (useg != NULL)
    {
        for (; useg->next != NULL; useg = useg->next);
    }
//   CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_output: pcb->flags = 0x%x\n"), pcb->flags);

    /* If the TF_ACK_NOW flag is set and no data will be sent (either
     * because the ->unsent queue is empty or because the window does
     * not allow it), construct an empty ACK segment and send it.
     *
     * If data is to be sent, we will just piggyback the ACK (see below).
     */
//    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_output: pcb->flags = 0x%0x\n"), pcb->flags);
    if (pcb->flags & TF_ACK_NOW && (seg == NULL || ntohl(seg->tcphdr->seqno) - pcb->lastack + seg->len > wnd))
    {
        p = pbuf_alloc(PBUF_IP, TCP_HLEN, PBUF_RAM);
        if (p == NULL)
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_output: (ACK) could not allocate pbuf\n"));
            return ERR_TCPIP_BUF;
        }
     //   CSW_TRACE(CFW_TCPIP_TS_ID, TSTXT("tcp_output: empty ACK next seqno expected 0x%x, recv wnd = 0x%x\n"), pcb->rcv_nxt, pcb->rcv_wnd);
	//    CSW_TRACE(CFW_TCPIP_TS_ID, TSTXT("tcp_output: empty ACK next seqno to be sent 0x%x, pcb->local_port\n"), pcb->snd_nxt,pcb->local_port,pcb->remote_port);
        /* remove ACK flags from the PCB, as we send an empty ACK now */
        pcb->flags &= ~(TF_ACK_DELAY | TF_ACK_NOW);

        tcphdr = p->payload;
        tcphdr->src = htons(pcb->local_port);
        tcphdr->dest = htons(pcb->remote_port);
        tcphdr->seqno = htonl(pcb->snd_nxt);
        tcphdr->ackno = htonl(pcb->rcv_nxt);
        TCPH_FLAGS_SET(tcphdr, TCP_ACK); 
        tcphdr->wnd = htons(pcb->rcv_wnd);
        tcphdr->urgp = 0;
        TCPH_HDRLEN_SET(tcphdr, 5);

        tcphdr->chksum = 0;
#if CHECKSUM_GEN_TCP
        tcphdr->chksum = inet_chksum_pseudo(p, &(pcb->local_ip), &(pcb->remote_ip), IP_PROTO_TCP, p->tot_len);
#endif
        ip_output(p, &(pcb->local_ip), &(pcb->remote_ip), pcb->ttl, pcb->tos, IP_PROTO_TCP);
        pbuf_free(p);
        //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_output:  data end\n"));
        return ERR_TCPIP_OK;
    }

#if TCP_OUTPUT_DEBUG
    if (seg == NULL)
    {
        //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_output: nothing to send (%p)\n"), (VOID *) pcb->unsent);
    }
#endif /* TCP_OUTPUT_DEBUG */
#if TCP_CWND_DEBUG
/*
    if (seg == NULL)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_output: snd_wnd 0x%x, cwnd 0x%x, wnd 0x%x, seg == NULL, ack 0x%x\n"), pcb->snd_wnd, pcb->cwnd, wnd, pcb->lastack);
    }
    else
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_output: snd_wnd 0x%x, cwnd 0x%x, wnd 0x%x, effwnd 0x%x, seq 0x%x, ack 0x%x\n"),
                         pcb->snd_wnd, pcb->cwnd, wnd, ntohl(seg->tcphdr->seqno) - pcb->lastack + seg->len, ntohl(seg->tcphdr->seqno), pcb->lastack);
    }
    */
#endif /* TCP_CWND_DEBUG */
    /* data available and window allows it to be sent? */
    while (seg != NULL && ntohl(seg->tcphdr->seqno) - pcb->lastack + seg->len <= wnd)
    {
#if 0
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_output: snd_wnd 0x%x, cwnd 0x%x, wnd 0x%x, effwnd 0x%x, seq 0x%x, ack 0x%x, i 0x%x\n"),
                         pcb->snd_wnd, pcb->cwnd, wnd, ntohl(seg->tcphdr->seqno) + seg->len - pcb->lastack, ntohl(seg->tcphdr->seqno), pcb->lastack, i);
        ++i;
#endif /* TCP_CWND_DEBUG */

        pcb->unsent = seg->next;

        if (pcb->state != SYN_SENT)
        {
            TCPH_SET_FLAG(seg->tcphdr, TCP_ACK);
            pcb->flags &= ~(TF_ACK_DELAY | TF_ACK_NOW);
        }
		
     //   CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("tcp_output: window allows, next seqno expected 0x%x, recv wnd = 0x%x\n"), pcb->rcv_nxt, pcb->rcv_wnd);
	//    CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("tcp_output: window allows, next seqno to be sent 0x%x\n"), ntohl(seg->tcphdr->seqno));
	 
        tcp_output_segment(seg, pcb);
        pcb->snd_nxt = ntohl(seg->tcphdr->seqno) + TCP_TCPLEN(seg);
       //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pcb->snd_max = 0x%x, pcb->snd_nxt = 0x%x\n"), pcb->snd_max, pcb->snd_nxt);		
        if (TCP_SEQ_LT(pcb->snd_max, pcb->snd_nxt))
        {             
            pcb->snd_max = pcb->snd_nxt;
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("update the snd_max = 0x%x\n"), pcb->snd_max);       			
        }
        /* put segment on unacknowledged list if length > 0 */
        if (TCP_TCPLEN(seg) > 0)
        {
            seg->next = NULL;
            /* unacked list is empty? */
            if (pcb->unacked == NULL)
            {
                pcb->unacked = seg;
                useg = seg;
                /* unacked list is not empty? */
            }
            else
            {
                /* In the case of fast retransmit, the packet should not go to the tail
                 * of the unacked queue, but rather at the head. We need to check for
                 * this case. -STJ Jul 27, 2004 */
                if (TCP_SEQ_LT(ntohl(seg->tcphdr->seqno), ntohl(useg->tcphdr->seqno)))
                {
                    /* add segment to head of unacked list */
                    seg->next = pcb->unacked;
                    pcb->unacked = seg;
                }
                else
                {
                    /* add segment to tail of unacked list */
                    useg->next = seg;
                    useg = useg->next;
                }
            }
            /* do not queue empty segments on the unacked list */
        }
        else
        {
            tcp_seg_free(seg);
        }
        seg = pcb->unsent;
    }
//	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_output: end\n"));
    return ERR_TCPIP_OK;
}

/**
 * Actually send a TCP segment over IP
 */
static VOID tcp_output_segment(struct tcp_seg *seg, struct tcp_pcb *pcb)
{
 //       CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("tcp_output_segment: begin \n"));

    UINT16 len;
    struct netif *netif;
    //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function tcp_output_segment\n"));
    /* The TCP header has already been constructed, but the ackno and
       wnd fields remain. */
    seg->tcphdr->ackno = htonl(pcb->rcv_nxt);

    /* silly window avoidance */
    if (pcb->rcv_wnd < pcb->mss)
    {
        seg->tcphdr->wnd = 0;
    }
    else
    {
        /* advertise our receive window size in this TCP segment */
        seg->tcphdr->wnd = htons(pcb->rcv_wnd);
    }
//        CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("tcp_output_segment: 1  .\n"));

    /* If we don't have a local IP address, we get one by
       calling ip_route(). */
    if (ip_addr_isany(&(pcb->local_ip)))
    {
        netif = ip_route(&(pcb->remote_ip),NULL);
        if (netif == NULL)
        {
             CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("tcp_output_segment: no netif  .\n"));

            return;
        }
        ip_addr_set(&(pcb->local_ip), &(netif->ip_addr));
    }
      //  CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("tcp_output_segment: 2  .\n"));

    pcb->rtime = 0;

    if (pcb->rttest == 0)
    {
        pcb->rttest = tcp_ticks;
        pcb->rtseq = ntohl(seg->tcphdr->seqno);

    //    CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("tcp_output_segment: rtseq 0x%x\n"), pcb->rtseq);
    }
   // CSW_TRACE(CFW_TCPIP_TS_ID_SXS | TNB_ARG(3), TSTXT("tcp_output_segment: 0x%x:0x%x , data len = 0x%x\n"), htonl(seg->tcphdr->seqno), htonl(seg->tcphdr->seqno) + seg->len, seg->len);

    len = (UINT16) ((UINT8 *) seg->tcphdr - (UINT8 *) seg->p->payload);

    seg->p->len -= len;
    seg->p->tot_len -= len;

    seg->p->payload = seg->tcphdr;

    seg->tcphdr->chksum = 0;
#if CHECKSUM_GEN_TCP
    seg->tcphdr->chksum = inet_chksum_pseudo(seg->p, &(pcb->local_ip), &(pcb->remote_ip), IP_PROTO_TCP, seg->p->tot_len);
#endif

    TCP_STATS_INC(tcp.xmit);
    //    CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("tcp_output_segment: 3  .\n"));

    ip_output(seg->p, &(pcb->local_ip), &(pcb->remote_ip), pcb->ttl, pcb->tos, IP_PROTO_TCP);
}

VOID tcp_rst(UINT32 seqno, UINT32 ackno, struct ip_addr *local_ip, struct ip_addr *remote_ip, UINT16 local_port, UINT16 remote_port)
{
    struct pbuf *p;
    struct tcp_hdr *tcphdr;
    p = pbuf_alloc(PBUF_IP, TCP_HLEN, PBUF_RAM);
    if (p == NULL)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_rst: could not allocate memory for pbuf\n"));
        return;
    }

    tcphdr = p->payload;
    tcphdr->src = htons(local_port);
    tcphdr->dest = htons(remote_port);
    tcphdr->seqno = htonl(seqno);
    tcphdr->ackno = htonl(ackno);
    TCPH_FLAGS_SET(tcphdr, TCP_RST | TCP_ACK);
    tcphdr->wnd = htons(TCP_WND);
    tcphdr->urgp = 0;
    TCPH_HDRLEN_SET(tcphdr, 5);

    tcphdr->chksum = 0;
#if CHECKSUM_GEN_TCP
    tcphdr->chksum = inet_chksum_pseudo(p, local_ip, remote_ip, IP_PROTO_TCP, p->tot_len);
#endif
    TCP_STATS_INC(tcp.xmit);
    /* Send output with hardcoded TTL since we have no access to the pcb */
    ip_output(p, local_ip, remote_ip, TCP_TTL, 0, IP_PROTO_TCP);
    pbuf_free(p);
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_rst: seqno 0x%x ackno 0x%x.\n"), seqno, ackno);
}

/* requeue all unacked segments for retransmission */
VOID tcp_rexmit_rto(struct tcp_pcb *pcb)
{
    struct tcp_seg *seg;

    if (pcb->unacked == NULL)
    {
        return;
    }

    /* Move all unacked segments to the head of the unsent queue */
    for (seg = pcb->unacked; seg->next != NULL; seg = seg->next);
    /* concatenate unsent queue after unacked queue */
    seg->next = pcb->unsent;
    /* unsent queue is the concatenated queue (of unacked, unsent) */
    pcb->unsent = pcb->unacked;
    /* unacked queue is now empty */
    pcb->unacked = NULL;

    pcb->snd_nxt = ntohl(pcb->unsent->tcphdr->seqno);
    /* increment number of retransmissions */
    ++pcb->nrtx;

    /* Don't take any RTT measurements after retransmitting. */
    pcb->rttest = 0;
	
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("Do the actual retransmission, 0x%x times\n"),pcb->nrtx);

    /* Do the actual retransmission */
    tcp_output(pcb);
}

VOID tcp_rexmit(struct tcp_pcb *pcb)
{
    struct tcp_seg *seg;

    if (pcb->unacked == NULL)
    {
        return;
    }

    /* Move the first unacked segment to the unsent queue */
    seg = pcb->unacked->next;
    pcb->unacked->next = pcb->unsent;
    pcb->unsent = pcb->unacked;
    pcb->unacked = seg;

    pcb->snd_nxt = ntohl(pcb->unsent->tcphdr->seqno);

    ++pcb->nrtx;

    /* Don't take any rtt measurements after retransmitting. */
    pcb->rttest = 0;

    /* Do the actual retransmission. */
    tcp_output(pcb);

}

VOID tcp_keepalive(struct tcp_pcb *pcb)
{
    struct pbuf *p;
    struct tcp_hdr *tcphdr;

    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_keepalive: sending KEEPALIVE probe to 0x%x.0x%x.0x%x.0x%x\n"), ip4_addr1(&pcb->remote_ip), ip4_addr2(&pcb->remote_ip), ip4_addr3(&pcb->remote_ip), ip4_addr4(&pcb->remote_ip));

    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_keepalive: tcp_ticks 0x%x   pcb->tmr 0x%x  pcb->keep_cnt 0x%x\n"), tcp_ticks, pcb->tmr, pcb->keep_cnt);

    p = pbuf_alloc(PBUF_IP, TCP_HLEN, PBUF_RAM);

    if (p == NULL)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_keepalive: could not allocate memory for pbuf\n"));
        return;
    }

    tcphdr = p->payload;
    tcphdr->src = htons(pcb->local_port);
    tcphdr->dest = htons(pcb->remote_port);
    tcphdr->seqno = htonl(pcb->snd_nxt - 1);
    tcphdr->ackno = htonl(pcb->rcv_nxt);
    tcphdr->wnd = htons(pcb->rcv_wnd);
    tcphdr->urgp = 0;
    TCPH_HDRLEN_SET(tcphdr, 5);
    TCPH_FLAGS_SET( tcphdr, TCP_ACK);

    tcphdr->chksum = 0;
#if CHECKSUM_GEN_TCP
    tcphdr->chksum = inet_chksum_pseudo(p, &pcb->local_ip, &pcb->remote_ip, IP_PROTO_TCP, p->tot_len);
#endif
    TCP_STATS_INC(tcp.xmit);

    /* Send output to IP */
    ip_output(p, &pcb->local_ip, &pcb->remote_ip, pcb->ttl, 0, IP_PROTO_TCP);

    pbuf_free(p);

    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_keepalive: seqno 0x%x ackno 0x%x.\n"), pcb->snd_nxt - 1, pcb->rcv_nxt);
}

#endif /* CII_TCP */
#endif

