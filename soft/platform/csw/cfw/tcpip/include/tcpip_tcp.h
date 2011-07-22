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
#ifndef __CII_TCP_H__
#define __CII_TCP_H__
//#include "tcpip_opt.h"
#include "tcpip_sys.h"
#include "tcpip_ip.h"
#include "tcpip_icmp.h"
//#include <csw.h>
#include "tcpip_mem.h"

//struct tcp_pcb;




struct tcp_pcb
{
/** common PCB members */
    IP_PCB;
/** protocol specific PCB members */
    struct tcp_pcb *next;    /* for the linked list */

    VOID *callback_arg;

#define TF_ACK_DELAY (UINT8)0x01U    /* Delayed ACK. */
#define TF_ACK_NOW   (UINT8)0x02U    /* Immediate ACK. */
#define TF_INFR      (UINT8)0x04U    /* In fast recovery. */
#define TF_RESET     (UINT8)0x08U    /* Connection was reset. */
#define TF_CLOSED    (UINT8)0x10U    /* Connection was sucessfully closed. */
#define TF_GOT_FIN   (UINT8)0x20U    /* Connection was closed by the remote end. */
#define TF_NODELAY   (UINT8)0x40U    /* Disable Nagle algorithm */

    /* receiver variables */
    UINT32 rcv_nxt;        /* next seqno expected */

    /* Timers */
    UINT32 tmr;

    /* RTT (round trip time) estimation variables */
    UINT32 rttest;        /* RTT estimate in 500ms ticks */
    UINT32 rtseq;        /* sequence number being timed */

    /* fast retransmit/recovery */
    UINT32 lastack;        /* Highest acknowledged seqno. */

    /* sender variables */
    UINT32 snd_nxt,        /* next seqno to be sent */
      snd_max,        /* Highest seqno sent. */
      snd_wnd,        /* sender window */
      snd_wl1, snd_wl2,    /* Sequence and acknowledgement numbers of last
                   window update. */
      snd_lbb;        /* Sequence number of next byte to be buffered. */

    /* These are ordered by sequence number: */
    struct tcp_seg *unsent;    /* Unsent (queued) segments. */
    struct tcp_seg *unacked;    /* Sent but unacknowledged segments. */
#if TCP_QUEUE_OOSEQ
    struct tcp_seg *ooseq;    /* Received out of sequence segments. */
#endif                /* TCP_QUEUE_OOSEQ */

#if LWIP_CALLBACK_API
    /* Function to be called when more send buffer space is available. */
      err_t(*sent) (VOID *arg, struct tcp_pcb * pcb, UINT16 space);

    /* Function to be called when (in-sequence) data has arrived. */
      err_t(*recv) (VOID *arg, struct tcp_pcb * pcb, struct pbuf * p, err_t err);

    /* Function to be called when a connection has been set up. */
      err_t(*connected) (VOID *arg, struct tcp_pcb * pcb, err_t err);

    /* Function to call when a listener has been connected. */
      err_t(*accept) (VOID *arg, struct tcp_pcb * newpcb, err_t err);

    /* Function which is called periodically. */
      err_t(*poll) (VOID *arg, struct tcp_pcb * pcb);

    /* Function to be called whenever a fatal error occurs. */
    VOID (*errf) (VOID *arg, err_t err);
#endif                /* LWIP_CALLBACK_API */

    /* idle time before KEEPALIVE is sent */
    UINT32 keepalive;
    UINT16 local_port;
    UINT16 remote_port;
    UINT16 rcv_wnd;        /* receiver window */
    /* Retransmission timer. */
    UINT16 rtime;
    UINT16 mss;        /* maximum segment size */
    UINT16 sa, sv;        /* @todo document this */
    UINT16 rto;        /* retransmission time-out */
    /* congestion avoidance/control variables */
    UINT16 cwnd;
    UINT16 ssthresh; 
    UINT16 acked;
    UINT16 snd_buf;        /* Available buffer space for sending (in bytes). */ 
    UINT8 state;    /* TCP state */      
    UINT8 prio;
    UINT8 flags;
    UINT8 polltmr, pollinterval;
    UINT8 nrtx;        /* number of retransmissions */
    UINT8 snd_queuelen;    /* Available buffer space for sending (in tcp_segs). */
    /* KEEPALIVE counter */
    UINT8 keep_cnt;
    UINT8 dupacks;
    UINT8 padding[2];
    
};

/* Functions for interfacing with TCP: */

/* Lower layer interface to TCP: */
VOID tcp_init(VOID);        /* Must be called first to
                   initialize TCP. */
VOID tcp_tmr(VOID);        /* Must be called every
                   TCP_TMR_INTERVAL
                   ms. (Typically 250 ms). */
/* Application program's interface: */
struct tcp_pcb *tcp_new(VOID);
struct tcp_pcb *tcp_alloc(UINT8 prio);

VOID tcp_arg(struct tcp_pcb *pcb, VOID *arg);
VOID tcp_accept(struct tcp_pcb *pcb, err_t(*accept) (VOID *arg, struct tcp_pcb * newpcb, err_t err));
VOID tcp_recv(struct tcp_pcb *pcb, err_t(*recv) (VOID *arg, struct tcp_pcb * tpcb, struct pbuf * p, err_t err));
VOID tcp_sent(struct tcp_pcb *pcb, err_t(*sent) (VOID *arg, struct tcp_pcb * tpcb, UINT16 len));
VOID tcp_poll(struct tcp_pcb *pcb, err_t(*poll) (VOID *arg, struct tcp_pcb * tpcb), UINT8 interval);
VOID tcp_err(struct tcp_pcb *pcb, VOID (*err) (VOID *arg, err_t err));

#define          tcp_mss(pcb)      ((pcb)->mss)
#define          tcp_sndbuf(pcb)   ((pcb)->snd_buf)

VOID tcp_recved(struct tcp_pcb *pcb, UINT16 len);
err_t tcp_bind(struct tcp_pcb *pcb, struct ip_addr *ipaddr, UINT16 port);
err_t tcp_connect(struct tcp_pcb *pcb, struct ip_addr *ipaddr, UINT16 port, err_t(*connected) (VOID *arg, struct tcp_pcb * tpcb, err_t err));
struct tcp_pcb *tcp_listen(struct tcp_pcb *pcb);
VOID tcp_abort(struct tcp_pcb *pcb);
err_t tcp_close(struct tcp_pcb *pcb);
err_t tcp_write(struct tcp_pcb *pcb, const VOID *dataptr, UINT16 len, UINT8 copy);

VOID tcp_setprio(struct tcp_pcb *pcb, UINT8 prio);

#define TCP_PRIO_MIN    1
#define TCP_PRIO_NORMAL 64
#define TCP_PRIO_MAX    127

/* It is also possible to call these two functions at the right
   intervals (instead of calling tcp_tmr()). */
VOID tcp_slowtmr(VOID);
VOID tcp_fasttmr(VOID);

/* Only used by IP to pass a TCP segment to TCP: */
VOID tcp_input(struct pbuf *p, struct netif *inp);
/* Used within the TCP code only: */
err_t tcp_output(struct tcp_pcb *pcb);
VOID tcp_rexmit(struct tcp_pcb *pcb);
VOID tcp_rexmit_rto(struct tcp_pcb *pcb);

#define TCP_SEQ_LT(a,b)     ((INT32)((a)-(b)) < 0)
#define TCP_SEQ_LEQ(a,b)    ((INT32)((a)-(b)) <= 0)
#define TCP_SEQ_GT(a,b)     ((INT32)((a)-(b)) > 0)
#define TCP_SEQ_GEQ(a,b)    ((INT32)((a)-(b)) >= 0)
/* is b<=a<=c? */
#if 0                /* see bug #10548 */
#define TCP_SEQ_BETWEEN(a,b,c) ((c)-(b) >= (a)-(b))
#endif
#define TCP_SEQ_BETWEEN(a,b,c) (TCP_SEQ_GEQ(a,b) && TCP_SEQ_LEQ(a,c))
#define TCP_FIN 0x01U
#define TCP_SYN 0x02U
#define TCP_RST 0x04U
#define TCP_PSH 0x08U
#define TCP_ACK 0x10U
#define TCP_URG 0x20U
#define TCP_ECE 0x40U
#define TCP_CWR 0x80U

#define TCP_FLAGS 0x3fU

/* Length of the TCP header, excluding options. */
#define TCP_HLEN 20

#ifndef TCP_TMR_INTERVAL
#define TCP_TMR_INTERVAL       250    /* The TCP timer interval in
                       milliseconds. */
#endif /* TCP_TMR_INTERVAL */

#ifndef TCP_FAST_INTERVAL
#define TCP_FAST_INTERVAL      TCP_TMR_INTERVAL    /* the fine grained timeout in
                           milliseconds */
#endif /* TCP_FAST_INTERVAL */

#ifndef TCP_SLOW_INTERVAL
#define TCP_SLOW_INTERVAL      (2*TCP_TMR_INTERVAL)    /* the coarse grained timeout in
                               milliseconds */
#endif /* TCP_SLOW_INTERVAL */

#define TCP_FIN_WAIT_TIMEOUT 20000    /* milliseconds */
#define TCP_SYN_RCVD_TIMEOUT 20000    /* milliseconds */

#define TCP_OOSEQ_TIMEOUT        6U    /* x RTO */

#define TCP_MSL 60000        /* The maximum segment lifetime in microseconds */

/*
 * User-settable options (used with setsockopt).
 */
#define    TCP_NODELAY       0x01    /* don't delay send to coalesce packets */
#define TCP_KEEPALIVE  0x02    /* send KEEPALIVE probes when idle for pcb->keepalive miliseconds */

/* Keepalive values */
#define  TCP_KEEPDEFAULT   7200000    /* KEEPALIVE timer in miliseconds */
#define  TCP_KEEPINTVL     75000    /* Time between KEEPALIVE probes in miliseconds */
#define  TCP_KEEPCNT       9    /* Counter for KEEPALIVE probes */
#define  TCP_MAXIDLE       TCP_KEEPCNT * TCP_KEEPINTVL    /* Maximum KEEPALIVE probe time */

#ifdef PACK_STRUCT_USE_INCLUDES
//#  include "arch/bpstruct.h"
#endif

#ifndef CII_VERSION
PACK_STRUCT_BEGIN struct tcp_hdr
{
    PACK_STRUCT_FIELD(UINT16 src);
    PACK_STRUCT_FIELD(UINT16 dest);
    PACK_STRUCT_FIELD(UINT32 seqno);
    PACK_STRUCT_FIELD(UINT32 ackno);
    PACK_STRUCT_FIELD(UINT16 _hdrlen_rsvd_flags);
    PACK_STRUCT_FIELD(UINT16 wnd);
    PACK_STRUCT_FIELD(UINT16 chksum);
    PACK_STRUCT_FIELD(UINT16 urgp);
} PACK_STRUCT_STRUCT;
#else
//remove by wuys 2008-11-26

struct tcp_hdr
{
    UINT16 src;
    UINT16 dest;
    UINT32 seqno;
    UINT32 ackno;
    UINT16 _hdrlen_rsvd_flags;
   UINT16 wnd;
   UINT16 chksum;
   UINT16 urgp;
} ;//PACK_STRUCT_STRUCT;

#endif

#ifndef 	CII_VERSION
PACK_STRUCT_END
#endif

#ifdef PACK_STRUCT_USE_INCLUDES
//#  include "arch/epstruct.h"
#endif
#define TCPH_OFFSET(phdr) (ntohs((phdr)->_hdrlen_rsvd_flags) >> 8)
#define TCPH_HDRLEN(phdr) (ntohs((phdr)->_hdrlen_rsvd_flags) >> 12)
#define TCPH_FLAGS(phdr)  (ntohs((phdr)->_hdrlen_rsvd_flags) & TCP_FLAGS)
#define TCPH_OFFSET_SET(phdr, offset) (phdr)->_hdrlen_rsvd_flags = htons(((offset) << 8) | TCPH_FLAGS(phdr))
#define TCPH_HDRLEN_SET(phdr, len) (phdr)->_hdrlen_rsvd_flags = htons(((len) << 12) | TCPH_FLAGS(phdr))
#define TCPH_FLAGS_SET(phdr, flags) (phdr)->_hdrlen_rsvd_flags = htons((ntohs((phdr)->_hdrlen_rsvd_flags) & ~TCP_FLAGS) | (flags))
#define TCPH_SET_FLAG(phdr, flags ) (phdr)->_hdrlen_rsvd_flags = htons(ntohs((phdr)->_hdrlen_rsvd_flags) | (flags))
#define TCPH_UNSET_FLAG(phdr, flags) (phdr)->_hdrlen_rsvd_flags = htons(ntohs((phdr)->_hdrlen_rsvd_flags) | (TCPH_FLAGS(phdr) & ~(flags)) )
#define TCP_TCPLEN(seg) ((seg)->len + ((TCPH_FLAGS((seg)->tcphdr) & TCP_FIN || \
          TCPH_FLAGS((seg)->tcphdr) & TCP_SYN)? 1: 0))

#define CLOSED 0
#define LISTEN 1
#define SYN_SENT 2
#define SYN_RCVD 3
#define ESTABLISHED 4
#define FIN_WAIT_1 5
#define FIN_WAIT_2 6
#define CLOSE_WAIT 7
#define CLOSING 8
#define LAST_ACK 9
#define TIME_WAIT 10


/* the TCP protocol control block */

struct tcp_pcb_listen
{
/* Common members of all PCB types */
    IP_PCB;

/* Protocol specific PCB members */
    struct tcp_pcb_listen *next;    /* for the linked list */

    /* Even if state is obviously LISTEN this is here for
     * field compatibility with tpc_pcb to which it is cast sometimes
     * Until a cleaner solution emerges this is here.FIXME
     */
    VOID *callback_arg;
    UINT16 local_port;
    UINT8 prio;
    UINT8 state;    /* TCP state */

#if LWIP_CALLBACK_API
    /* Function to call when a listener has been connected. */
      err_t(*accept) (VOID *arg, struct tcp_pcb * newpcb, err_t err);
#endif                /* LWIP_CALLBACK_API */
};

#if LWIP_EVENT_API



#define LWIP_EVENT_ACCEPT 0
#define LWIP_EVENT_SENT 1
#define LWIP_EVENT_RECV 2
#define LWIP_EVENT_CONNECTED 3
#define LWIP_EVENT_POLL 4
#define LWIP_EVENT_ERR 5


err_t lwip_tcp_event(VOID *arg, struct tcp_pcb *pcb, UINT8, struct pbuf *p, UINT16 size, err_t err);

#define TCP_EVENT_ACCEPT(pcb,err,ret)    ret = lwip_tcp_event((pcb)->callback_arg, (pcb),\
                LWIP_EVENT_ACCEPT, NULL, 0, err)
#define TCP_EVENT_SENT(pcb,space,ret) ret = lwip_tcp_event((pcb)->callback_arg, (pcb),\
                   LWIP_EVENT_SENT, NULL, space, ERR_OK)
#define TCP_EVENT_RECV(pcb,p,err,ret) ret = lwip_tcp_event((pcb)->callback_arg, (pcb),\
                LWIP_EVENT_RECV, (p), 0, (err))
#define TCP_EVENT_CONNECTED(pcb,err,ret) ret = lwip_tcp_event((pcb)->callback_arg, (pcb),\
                LWIP_EVENT_CONNECTED, NULL, 0, (err))
#define TCP_EVENT_POLL(pcb,ret)       ret = lwip_tcp_event((pcb)->callback_arg, (pcb),\
                LWIP_EVENT_POLL, NULL, 0, ERR_OK)
#define TCP_EVENT_ERR(errf,arg,err)  lwip_tcp_event((arg), NULL, \
                LWIP_EVENT_ERR, NULL, 0, (err))
#else /* LWIP_EVENT_API */
#define TCP_EVENT_ACCEPT(pcb,err,ret)     \
                        if((pcb)->accept != NULL) \
                        (ret = (pcb)->accept((pcb)->callback_arg,(pcb),(err)))
#define TCP_EVENT_SENT(pcb,space,ret) \
                        if((pcb)->sent != NULL) \
                        (ret = (pcb)->sent((pcb)->callback_arg,(pcb),(space)))
#define TCP_EVENT_RECV(pcb,p,err,ret) \
                        if((pcb)->recv != NULL) \
                        { ret = (pcb)->recv((pcb)->callback_arg,(pcb),(p),(err)); } else { \
                          if (p) pbuf_free(p); }
#define TCP_EVENT_CONNECTED(pcb,err,ret) \
                        if((pcb)->connected != NULL) \
                        (ret = (pcb)->connected((pcb)->callback_arg,(pcb),(err)))
#define TCP_EVENT_POLL(pcb,ret) \
                        if((pcb)->poll != NULL) \
                        (ret = (pcb)->poll((pcb)->callback_arg,(pcb)))
#define TCP_EVENT_ERR(errf,arg,err) \
                        if((errf) != NULL) \
                        (errf)((arg),(err))
#endif /* LWIP_EVENT_API */

/* This structure represents a TCP segment on the unsent and unacked queues */
struct tcp_seg
{
    struct tcp_seg *next;    /* used when putting segements on a queue */
    struct pbuf *p;        /* buffer containing data + TCP header */
    VOID *dataptr;        /* pointer to the TCP data in the pbuf */
    struct tcp_hdr *tcphdr;    /* the TCP header */
    UINT16 len;        /* the TCP length of this segment */
    UINT8 padding[2];
};

/* Internal functions and global variables: */
struct tcp_pcb *tcp_pcb_copy(struct tcp_pcb *pcb);
VOID tcp_pcb_purge(struct tcp_pcb *pcb);
BOOL tcp_pcb_remove(struct tcp_pcb **pcblist, struct tcp_pcb *pcb);
BOOL tcp_pcb_remove_all(struct tcp_pcb *pcb,UINT8 flag);//2009-12-16, 
UINT8 tcp_segs_free(struct tcp_seg *seg);
UINT8 tcp_seg_free(struct tcp_seg *seg);
struct tcp_seg *tcp_seg_copy(struct tcp_seg *seg);

#define tcp_ack(pcb)     if((pcb)->flags & TF_ACK_DELAY) { \
                            (pcb)->flags &= ~TF_ACK_DELAY; \
                            (pcb)->flags |= TF_ACK_NOW; \
                            tcp_output(pcb); \
                         } else { \
                            (pcb)->flags |= TF_ACK_DELAY; \
                         }

#define tcp_ack_now(pcb) (pcb)->flags |= TF_ACK_NOW; \
                         tcp_output(pcb)

err_t tcp_send_ctrl(struct tcp_pcb *pcb, UINT8 flags);
err_t tcp_enqueue(struct tcp_pcb *pcb, VOID *dataptr, UINT16 len, UINT8 flags, UINT8 copy, UINT8 * optdata, UINT8 optlen);

VOID tcp_rexmit_seg(struct tcp_pcb *pcb, struct tcp_seg *seg);

VOID tcp_rst(UINT32 seqno, UINT32 ackno, struct ip_addr *local_ip, struct ip_addr *remote_ip, UINT16 local_port, UINT16 remote_port);

UINT32 tcp_next_iss(VOID);

VOID tcp_keepalive(struct tcp_pcb *pcb);

extern struct tcp_pcb *tcp_input_pcb;
extern UINT32 tcp_ticks;

#if TCP_DEBUG || TCP_INPUT_DEBUG || TCP_OUTPUT_DEBUG
VOID tcp_debug_print(struct tcp_hdr *tcphdr);
VOID tcp_debug_print_flags(UINT8 flags);
VOID tcp_debug_print_state(UINT8 s);
VOID tcp_debug_print_pcbs(VOID);
INT16 tcp_pcbs_sane(VOID);
#else
#  define tcp_debug_print(tcphdr)
#  define tcp_debug_print_flags(flags)
#  define tcp_debug_print_state(s)
#  define tcp_debug_print_pcbs()
#  define tcp_pcbs_sane() 1
#endif /* TCP_DEBUG */

#if NO_SYS
#define tcp_timer_needed()
#else
VOID tcp_timer_needed(VOID);
#endif

/* The TCP PCB lists. */
union tcp_listen_pcbs_t
{                /* List of all TCP PCBs in LISTEN state. */
    struct tcp_pcb_listen *listen_pcbs;
    struct tcp_pcb *pcbs;
};
extern union tcp_listen_pcbs_t tcp_listen_pcbs;
extern struct tcp_pcb *tcp_active_pcbs;    /* List of all TCP PCBs that are in a
                       state in which they accept or send
                       data. */
extern struct tcp_pcb *tcp_tw_pcbs;    /* List of all TCP PCBs in TIME-WAIT. */

extern struct tcp_pcb *tcp_tmp_pcb;    /* Only used for temporary storage. */

/* Axioms about the above lists:   
   1) Every TCP PCB that is not CLOSED is in one of the lists.
   2) A PCB is only in one of the lists.
   3) All PCBs in the tcp_listen_pcbs list is in LISTEN state.
   4) All PCBs in the tcp_tw_pcbs list is in TIME-WAIT state.
*/

/* Define two macros, TCP_REG and TCP_RMV that registers a TCP PCB
   with a PCB list or removes a PCB from a list, respectively. */
#if 0
#define TCP_REG(pcbs, npcb) do {\
                            SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("TCP_REG %p local port %d\n"), npcb, npcb->local_port); \
                            for(tcp_tmp_pcb = *pcbs; \
          tcp_tmp_pcb != NULL; \
        tcp_tmp_pcb = tcp_tmp_pcb->next) { \
                                LWIP_ASSERT("TCP_REG: already registered\n", tcp_tmp_pcb != npcb); \
                            } \
                            LWIP_ASSERT("TCP_REG: pcb->state != CLOSED", npcb->state != CLOSED); \
                            npcb->next = *pcbs; \
                            LWIP_ASSERT("TCP_REG: npcb->next != npcb", npcb->next != npcb); \
                            *(pcbs) = npcb; \
                            LWIP_ASSERT("TCP_RMV: tcp_pcbs sane", tcp_pcbs_sane()); \
              tcp_timer_needed(); \
                            } while(0)
#define TCP_RMV(pcbs, npcb) do { \
                            LWIP_ASSERT("TCP_RMV: pcbs != NULL", *pcbs != NULL); \
                            SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("TCP_RMV: removing %p from %p\n"), npcb, *pcbs); \
                            if(*pcbs == npcb) { \
                               *pcbs = (*pcbs)->next; \
                            } else for(tcp_tmp_pcb = *pcbs; tcp_tmp_pcb != NULL; tcp_tmp_pcb = tcp_tmp_pcb->next) { \
                               if(tcp_tmp_pcb->next != NULL && tcp_tmp_pcb->next == npcb) { \
                                  tcp_tmp_pcb->next = npcb->next; \
                                  break; \
                               } \
                            } \
                            npcb->next = NULL; \
                            LWIP_ASSERT("TCP_RMV: tcp_pcbs sane", tcp_pcbs_sane()); \
                            SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("TCP_RMV: removed %p from %p\n"), npcb, *pcbs); \
                            } while(0)

#else /* LWIP_DEBUG */
#if 0//modify wuys 2009-12-10

#define TCP_REG(pcbs, npcb) do { \
	                     ASSERT(npcb != *pcbs)\
                            npcb->next = *pcbs; \
                            *(pcbs) = npcb; \
                                  tcp_timer_needed(); \
                            } while(0)
#define TCP_RMV(pcbs, npcb) do { \
                            if(*(pcbs) == npcb) { \
                               (*(pcbs)) = (*pcbs)->next; \
                            } else for(tcp_tmp_pcb = *pcbs; tcp_tmp_pcb != NULL; tcp_tmp_pcb = tcp_tmp_pcb->next) { \
                               if(tcp_tmp_pcb->next != NULL && tcp_tmp_pcb->next == npcb) { \
                                  tcp_tmp_pcb->next = npcb->next; \
                                  break; \
                               } \
                            } \
                            npcb->next = NULL; \
                            } while(0)
#else
#define TCP_REG(pcbs, npcb) do { \
	                      npcb->next = *pcbs; \
                            *(pcbs) = npcb; \
                             TCP_ASSERT(npcb != npcb->next );\
                                  tcp_timer_needed(); \
                            } while(0)
#define TCP_RMV(pcbs, npcb) do { \
                            if(*(pcbs) == npcb) { \
                               (*(pcbs)) = (*pcbs)->next; \
                              if( (*pcbs)!=NULL)\
                               TCP_ASSERT((*(pcbs)) != (*pcbs)->next );\
                            } else {\
                            for(tcp_tmp_pcb = *pcbs; tcp_tmp_pcb != NULL; tcp_tmp_pcb = tcp_tmp_pcb->next) { \
                               if(tcp_tmp_pcb->next != NULL && tcp_tmp_pcb->next == npcb) { \
                                  tcp_tmp_pcb->next = npcb->next; \
                                  TCP_ASSERT(tcp_tmp_pcb != tcp_tmp_pcb->next );\
                                  break; \
                               } \
                            } \
                              	}\
                            npcb->next = NULL; \
                            } while(0)
#endif
#endif /* __CII_TCP_H__ */
#endif
#endif

