/**
 * @file
 *
 * Transmission Control Protocol for IP
 *
 * This file contains common functions for the TCP implementation, such as functinos
 * for manipulating the data structures and the TCP timer functions. TCP functions
 * related to input and output is found in tcp_in.c and tcp_out.c respectively.
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

//#include <string.h>
#ifdef CFW_TCPIP_SUPPORT
#include "tcpip_opt.h"
#include <csw.h>
#include "tcpip_def.h"
#include "tcpip_mem.h"
#include "tcpip_memp.h"

#include "tcpip_tcp.h"
#if CII_TCP

/* Incremented every coarse grained timer shot
   (typically every 500 ms, determined by TCP_COARSE_TIMEOUT). */
UINT32 tcp_ticks;
const UINT8 tcp_backoff[13] = { 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7 };

/* The TCP PCB lists. */
/** List of all TCP PCBs bound but not yet (connected || listening) */
struct tcp_pcb *tcp_bound_pcbs;  

/** List of all TCP PCBs in LISTEN state */
union tcp_listen_pcbs_t tcp_listen_pcbs;
/** List of all TCP PCBs that are in a state in which
 * they accept or send data. */
struct tcp_pcb *tcp_active_pcbs;
/** List of all TCP PCBs in TIME-WAIT state */
struct tcp_pcb *tcp_tw_pcbs;

struct tcp_pcb *tcp_tmp_pcb;

static UINT8 tcp_timer;
static UINT16 tcp_new_port(VOID);

/**
 * Initializes the TCP layer.
 */
VOID tcp_init(VOID)
{
    /* Clear globals. */
    tcp_bound_pcbs = NULL;	
    tcp_listen_pcbs.listen_pcbs = NULL;
    tcp_active_pcbs = NULL;
    tcp_tw_pcbs = NULL;
    tcp_tmp_pcb = NULL;

    /* initialize timer */
    tcp_ticks = 0;
    tcp_timer = 0;

}

/**
 * Called periodically to dispatch TCP timers.
 *
 */
VOID tcp_tmr(VOID)
{
    /* Call tcp_fasttmr() every 250 ms */
    tcp_fasttmr();

    if (++tcp_timer & 1)
    {
        /* Call tcp_tmr() every 500 ms, i.e., every other timer
           tcp_tmr() is called. */
        tcp_slowtmr();
    }
}

/**
 * Closes the connection held by the PCB.
 *
 */
err_t tcp_close(struct tcp_pcb *pcb)
{
    err_t err;

#if TCP_DEBUG
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_close: closing in state "));
    tcp_debug_print_state(pcb->state);
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("\n"));
#endif /* TCP_DEBUG */
CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_close: closing in state , pcb->state, 0x%x"),pcb->state);
    switch (pcb->state)
    {
    case CLOSED:
        /* Closing a pcb in the CLOSED state might seem erroneous,
         * however, it is in this state once allocated and as yet unused
         * and the user needs some way to free it should the need arise.
         * Calling tcp_close() with a pcb that has already been closed, (i.e. twice)
         * or for a pcb that has been used and then entered the CLOSED state 
         * is erroneous, but this should never happen as the pcb has in those cases
         * been freed, and so any remaining handles are bogus. */
        err = ERR_TCPIP_OK;
	// tcp_pcb_remove(&tcp_active_pcbs, pcb);//add for test 2009-12-15
	 //tcp_pcb_remove(&tcp_tw_pcbs, pcb);//add for test 2009-12-15
         // tcp_pcb_remove_all(pcb,0);//2009-12-16, 
        //TCP_RMV(&tcp_bound_pcbs, pcb);	
        //       if(tcp_pcb_remove_all(pcb,0))//remove 2010-02-23
        tcp_pcb_remove_all(pcb,0);//add 2010-02-23
        memp_free(MEMP_TCP_PCB, pcb);
        pcb = NULL;
        break;
    case LISTEN:
        err = ERR_TCPIP_OK;
        tcp_pcb_remove((struct tcp_pcb **) &tcp_listen_pcbs.pcbs, pcb);
        memp_free(MEMP_TCP_PCB_LISTEN, pcb);
        pcb = NULL;
        break;
    case SYN_SENT:
        err = ERR_TCPIP_OK;
        //tcp_pcb_remove_all(pcb,1);//2009-12-16, tcp_pcb_remove(&tcp_active_pcbs, pcb);
            if(tcp_pcb_remove_all(pcb,1))
        memp_free(MEMP_TCP_PCB, pcb);
        pcb = NULL;
        break;
    case SYN_RCVD:
    case ESTABLISHED:
        err = tcp_send_ctrl(pcb, TCP_FIN);
        if (err == ERR_TCPIP_OK)
        {
            pcb->state = FIN_WAIT_1;
        }
        break;
    case CLOSE_WAIT:
        err = tcp_send_ctrl(pcb, TCP_FIN);
        if (err == ERR_TCPIP_OK)
        {
            pcb->state = LAST_ACK;
        }
        break;
    default:
        /* Has already been closed, do nothing. */
        err = ERR_TCPIP_OK;
        pcb = NULL;
        break;
    }

    if (pcb != NULL && err == ERR_TCPIP_OK)
    {
        err = tcp_output(pcb);
    }
    return err;
}

/**
 * Aborts a connection by sending a RST to the remote host and deletes
 * the local protocol control block. This is done when a connection is
 * killed because of shortage of memory.
 *
 */
VOID tcp_abort(struct tcp_pcb *pcb)
{
    UINT32 seqno, ackno;
    UINT16 remote_port, local_port;
    struct ip_addr remote_ip, local_ip;
#if LWIP_CALLBACK_API
    VOID (*errf) (VOID *arg, err_t err);
#endif /* LWIP_CALLBACK_API */
    VOID *errf_arg;

    /* Figure out on which TCP PCB list we are, and remove us. If we
       are in an active state, call the receive function associated with
       the PCB with a NULL argument, and send an RST to the remote end. */
    if (pcb->state == TIME_WAIT)
    {
      //  tcp_pcb_remove_all(pcb,2);//2009-12-16, tcp_pcb_remove(&tcp_tw_pcbs, pcb);
            if(tcp_pcb_remove_all(pcb,2))
        memp_free(MEMP_TCP_PCB, pcb);
    }
    else
    {
        seqno = pcb->snd_nxt;
        ackno = pcb->rcv_nxt;
        ip_addr_set(&local_ip, &(pcb->local_ip));
        ip_addr_set(&remote_ip, &(pcb->remote_ip));
        local_port = pcb->local_port;
        remote_port = pcb->remote_port;
#if LWIP_CALLBACK_API
        errf = pcb->errf;
#endif /* LWIP_CALLBACK_API */
        errf_arg = pcb->callback_arg;
       // tcp_pcb_remove_all(pcb,1);//2009-12-16, tcp_pcb_remove(&tcp_active_pcbs, pcb);
        if (pcb->unacked != NULL)
        {
            tcp_segs_free(pcb->unacked);
        }
        if (pcb->unsent != NULL)
        {
            tcp_segs_free(pcb->unsent);
        }
#if TCP_QUEUE_OOSEQ
        if (pcb->ooseq != NULL)
        {
            tcp_segs_free(pcb->ooseq);
        }
#endif /* TCP_QUEUE_OOSEQ */
    //     tcp_pcb_remove(&tcp_tw_pcbs, pcb);
        if(tcp_pcb_remove_all(pcb,1))
           memp_free(MEMP_TCP_PCB, pcb);
        TCP_EVENT_ERR(errf, errf_arg, ERR_TCPIP_ABRT);
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_abort: sending RST\n"));
        tcp_rst(seqno, ackno, &local_ip, &remote_ip, local_port, remote_port);
    }
}

/**
 * Binds the connection to a local portnumber and IP address. If the
 * IP address is not given (i.e., ipaddr == NULL), the IP address of
 * the outgoing network interface is used instead.
 *
 */

err_t tcp_bind(struct tcp_pcb *pcb, struct ip_addr *ipaddr, UINT16 port)
{
    struct tcp_pcb *cpcb;

    if (port == 0)
    {
        port = tcp_new_port();
    }
    /* Check if the address already is in use. */
    for (cpcb = (struct tcp_pcb *) tcp_listen_pcbs.pcbs; cpcb != NULL; cpcb = cpcb->next)
    {
       if(cpcb == pcb)
       	{
       	     return ERR_TCPIP_USE;

       	}
        if (cpcb->local_port == port)
        {
            if (ip_addr_isany(&(cpcb->local_ip)) || ip_addr_isany(ipaddr) || ip_addr_cmp(&(cpcb->local_ip), ipaddr))
            {
                return ERR_TCPIP_USE;
            }
        }
    }
    for (cpcb = tcp_active_pcbs; cpcb != NULL; cpcb = cpcb->next)
    {
           if(cpcb == pcb)
       	{
       	     return ERR_TCPIP_USE;

       	}

        if (cpcb->local_port == port)
        {
            if (ip_addr_isany(&(cpcb->local_ip)) || ip_addr_isany(ipaddr) || ip_addr_cmp(&(cpcb->local_ip), ipaddr))
            {
                return ERR_TCPIP_USE;
            }
        }
    }
	 for (cpcb = tcp_tw_pcbs; cpcb != NULL; cpcb = cpcb->next)
    {
           if(cpcb == pcb)
       	{
       	     return ERR_TCPIP_USE;

       	}

        if (cpcb->local_port == port)
        {
            if (ip_addr_isany(&(cpcb->local_ip)) || ip_addr_isany(ipaddr) || ip_addr_cmp(&(cpcb->local_ip), ipaddr))
            {
                return ERR_TCPIP_USE;
            }
        }
    }
  for(cpcb = tcp_bound_pcbs; cpcb != NULL; cpcb = cpcb->next) {
  	       if(cpcb == pcb)
       	{
       	     return ERR_TCPIP_USE;

       	}

    if (cpcb->local_port == port) {
      if (ip_addr_isany(&(cpcb->local_ip)) ||
          ip_addr_isany(ipaddr) ||
          ip_addr_cmp(&(cpcb->local_ip), ipaddr)) {
        return ERR_TCPIP_USE;
      }
    }
	/*
    if(cpcb == pcb)
    {
    	if (!ip_addr_isany(ipaddr))
      {
        cpcb->local_ip = *ipaddr;
      }
      cpcb->local_port = port;
	      CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_bind: bind to port 0x%x\n"), port);

	   return ERR_TCPIP_OK;
    }
    */
  }

    if (!ip_addr_isany(ipaddr))
    {
        pcb->local_ip = *ipaddr;
    }
    pcb->local_port = port;
   TCP_REG(&tcp_bound_pcbs, pcb);
	
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_bind: bind to port 0x%x\n"), port);
    return ERR_TCPIP_OK;
}

#if LWIP_CALLBACK_API
static err_t tcp_accept_null(VOID *arg, struct tcp_pcb *pcb, err_t err)
{
    (VOID) arg;
    (VOID) pcb;
    (VOID) err;

    return ERR_TCPIP_ABRT;
}
#endif /* LWIP_CALLBACK_API */

/**
 * Set the state of the connection to be LISTEN, which means that it
 * is able to accept incoming connections. The protocol control block
 * is reallocated in order to consume less memory. Setting the
 * connection to LISTEN is an irreversible process.
 *
 */
struct tcp_pcb *tcp_listen(struct tcp_pcb *pcb)
{

    struct tcp_pcb_listen *lpcb;

    /* already listening? */
    if (pcb->state == LISTEN)
    {
        return pcb;
    }
    lpcb = memp_malloc(MEMP_TCP_PCB_LISTEN);
    if (lpcb == NULL)
    {
        return NULL;
    }
    lpcb->callback_arg = pcb->callback_arg;
    lpcb->local_port = pcb->local_port;
    lpcb->state = LISTEN;
    lpcb->so_options = pcb->so_options;
    lpcb->so_options |= SOF_ACCEPTCONN;
    lpcb->ttl = pcb->ttl;
    lpcb->tos = pcb->tos;
    ip_addr_set(&lpcb->local_ip, &pcb->local_ip);
    TCP_RMV(&tcp_bound_pcbs, pcb);
	
    memp_free(MEMP_TCP_PCB, pcb);
#if LWIP_CALLBACK_API
    lpcb->accept = tcp_accept_null;
#endif /* LWIP_CALLBACK_API */
    TCP_REG(&tcp_listen_pcbs.listen_pcbs, lpcb);
    return (struct tcp_pcb *) lpcb;
}

/**
 * This function should be called by the application when it has
 * processed the data. The purpose is to advertise a larger window
 * when the data has been processed.
 *
 */
VOID tcp_recved(struct tcp_pcb *pcb, UINT16 len)
{
    if ((UINT32) pcb->rcv_wnd + len > TCP_WND)
    {
        pcb->rcv_wnd = TCP_WND;
    }
    else
    {
        pcb->rcv_wnd += len;
    }
    if (!(pcb->flags & TF_ACK_DELAY) && !(pcb->flags & TF_ACK_NOW))
    {
        /*
         * We send an ACK here (if one is not already pending, hence
         * the above tests) as tcp_recved() implies that the application
         * has processed some data, and so we can open the receiver's
         * window to allow more to be transmitted.  This could result in
         * two ACKs being sent for each received packet in some limited cases
         * (where the application is only receiving data, and is slow to
         * process it) but it is necessary to guarantee that the sender can
         * continue to transmit.
         */
//        CSW_TRACE(CFW_TCPIP_TS_ID_SXS | TNB_ARG(1), TSTXT("tcp_recved: send ack here pcb->flags = 0x%x\n"), pcb->flags);
         
        tcp_ack(pcb);
    }
    else if (pcb->flags & TF_ACK_DELAY && pcb->rcv_wnd >= TCP_WND / 2)
    {
        /* If we can send a window update such that there is a full
         * segment available in the window, do so now.  This is sort of
         * nagle-like in its goals, and tries to hit a compromise between
         * sending acks each time the window is updated, and only sending
         * window updates when a timer expires.  The "threshold" used
         * above (currently TCP_WND/2) can be tuned to be more or less
         * aggressive  */
       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_recved: send ack now window update pcb->flags = 0x%x\n"), pcb->flags);
         
        tcp_ack_now(pcb);
    }

    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_recved: recveived 0x%x bytes, wnd 0x%x (window used 0x%x).\n"), len, pcb->rcv_wnd, TCP_WND - pcb->rcv_wnd);
}

/**
 * A nastly hack featuring 'goto' statements that allocates a
 * new TCP local port.
 */
static UINT16 tcp_new_port(VOID)
{
    struct tcp_pcb *pcb;
#ifndef TCP_LOCAL_PORT_RANGE_START
#define TCP_LOCAL_PORT_RANGE_START 4096
#define TCP_LOCAL_PORT_RANGE_END   0x7fff
#endif
    static UINT16 port = TCP_LOCAL_PORT_RANGE_START;

      again:
    if (++port > TCP_LOCAL_PORT_RANGE_END)
    {
        port = TCP_LOCAL_PORT_RANGE_START;
    }

    for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next)
    {
        if (pcb->local_port == port)
        {
            goto again;
        }
    }
    for (pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next)
    {
        if (pcb->local_port == port)
        {
            goto again;
        }
    }
    for (pcb = (struct tcp_pcb *) tcp_listen_pcbs.pcbs; pcb != NULL; pcb = pcb->next)
    {
        if (pcb->local_port == port)
        {
            goto again;
        }
    }
    return port;
}

/**
 * Connects to another host. The function given as the "connected"
 * argument will be called when the connection has been established.
 *
 */
err_t tcp_connect(struct tcp_pcb * pcb, struct ip_addr * ipaddr, UINT16 port, err_t(*connected) (VOID *arg, struct tcp_pcb * tpcb, err_t err))
{
    UINT32 optdata;
    err_t ret;
    UINT32 iss;

    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_connect to port 0x%x\n"), port);
    if (ipaddr != NULL)
    {
        pcb->remote_ip = *ipaddr;
    }
    else
    {
        return ERR_TCPIP_VAL;
    }
    pcb->remote_port = port;
    if (pcb->local_port == 0)
    {
        pcb->local_port = tcp_new_port();
    }
    iss = tcp_next_iss();
    pcb->rcv_nxt = 0;
    pcb->snd_nxt = iss;
    pcb->lastack = iss - 1;
    pcb->snd_lbb = iss - 1;
    pcb->rcv_wnd = TCP_WND;
    pcb->snd_wnd = TCP_WND;
    pcb->mss = TCP_MSS;
    pcb->cwnd = 1;
    pcb->ssthresh = pcb->mss * 10;
    pcb->state = SYN_SENT;
#if LWIP_CALLBACK_API
    pcb->connected = connected;
#endif /* LWIP_CALLBACK_API */
    if(tcp_bound_pcbs!=NULL)
    	{
    	        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_connect  tcp_bound_pcbs:0x%x\n"),tcp_bound_pcbs);

               TCP_RMV(&tcp_bound_pcbs, pcb);
    	}else{
    	        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_connect  tcp_bound_pcbs: NULL\n"));

    	}
    TCP_REG(&tcp_active_pcbs, pcb);

    /* Build an MSS option */
    optdata = htonl(((UINT32) 2 << 24) | ((UINT32) 4 << 16) | (((UINT32) pcb->mss / 256) << 8) | (pcb->mss & 255));

    ret = tcp_enqueue(pcb, NULL, 0, TCP_SYN, 0, (UINT8 *) & optdata, 4);
    if (ret == ERR_TCPIP_OK)
    {
        tcp_output(pcb);
    }
    return ret;
}

/**
 * Called every 500 ms and implements the retransmission timer and the timer that
 * removes PCBs that have been in TIME-WAIT for enough time. It also increments
 * various timers such as the inactivity timer in each PCB.
 */
VOID tcp_slowtmr(VOID)
{
    struct tcp_pcb *pcb, *pcb2, *prev;
    UINT32 eff_wnd;
    UINT8 pcb_remove;    /* flag if a PCB should be removed */
    err_t err;

    err = ERR_TCPIP_OK;

    ++tcp_ticks;

    /* Steps through all of the active PCBs. */
    prev = NULL;
    pcb = tcp_active_pcbs;
    BOOL connect_timeout = FALSE;
    if (pcb == NULL)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_slowtmr: no active pcbs\n"));
    }
    while (pcb != NULL)
    {
        //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_slowtmr: processing active pcb\n"));
        LWIP_ASSERT("tcp_slowtmr: active pcb->state != CLOSED\n", pcb->state != CLOSED);
        LWIP_ASSERT("tcp_slowtmr: active pcb->state != LISTEN\n", pcb->state != LISTEN);
        LWIP_ASSERT("tcp_slowtmr: active pcb->state != TIME-WAIT\n", pcb->state != TIME_WAIT);

        pcb_remove = 0;

        if (pcb->state == SYN_SENT && pcb->nrtx == TCP_SYNMAXRTX)
        {
            ++pcb_remove;
			connect_timeout = TRUE;//add wuys 2009-12-01
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_slowtmr: max SYN retries reached\n"));
        }
        else if (pcb->nrtx == TCP_MAXRTX)
        {
            ++pcb_remove;
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_slowtmr: max DATA retries reached\n"));
        }
        else
        {
            ++pcb->rtime;
            if (pcb->unacked != NULL && pcb->rtime >= pcb->rto)
            {

                /* Time for a retransmission. */
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("tcp_slowtmr: Time for a retransmission: rtime 0x%x; pcb->rto 0x%x; pcb->state: 0x%x\n"), pcb->rtime, pcb->rto,pcb->state);

                /* Double retransmission time-out unless we are trying to
                 * connect to somebody (i.e., we are in SYN_SENT). */
                if (pcb->state != SYN_SENT)
                {
                    pcb->rto = ((pcb->sa >> 3) + pcb->sv) << tcp_backoff[pcb->nrtx];
                }
                /* Reduce congestion window and ssthresh. */
                eff_wnd = LWIP_MIN(pcb->cwnd, pcb->snd_wnd);
                pcb->ssthresh = eff_wnd >> 1;
                if (pcb->ssthresh < pcb->mss)
                {
                    pcb->ssthresh = pcb->mss * 2;
                }
                pcb->cwnd = pcb->mss;
      //          CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("tcp_slowtmr: cwnd 0x%x ssthresh 0x%x\n"), pcb->cwnd, pcb->ssthresh);

                /* The following needs to be called AFTER cwnd is set to one mss - STJ */
                tcp_rexmit_rto(pcb);
            }
        }
        /* Check if this PCB has stayed too long in FIN-WAIT-2 */
        if (pcb->state == FIN_WAIT_2)
        {
            if ((UINT32) (tcp_ticks - pcb->tmr) > TCP_FIN_WAIT_TIMEOUT / TCP_SLOW_INTERVAL)
            {
                ++pcb_remove;
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_slowtmr: removing pcb stuck in FIN-WAIT-2\n"));
            }
        }

        /* Check if KEEPALIVE should be sent */
        if ((pcb->so_options & SOF_KEEPALIVE) && ((pcb->state == ESTABLISHED) || (pcb->state == CLOSE_WAIT)))
        {
            if ((UINT32) (tcp_ticks - pcb->tmr) > (pcb->keepalive + TCP_MAXIDLE) / TCP_SLOW_INTERVAL)
            {
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_slowtmr: KEEPALIVE timeout. Aborting connection to 0x%x.0x%x.0x%x.0x%x.\n"),
                            ip4_addr1(&pcb->remote_ip), ip4_addr2(&pcb->remote_ip), ip4_addr3(&pcb->remote_ip), ip4_addr4(&pcb->remote_ip));

                tcp_abort(pcb);
            }
            else if ((UINT32) (tcp_ticks - pcb->tmr) > (pcb->keepalive + pcb->keep_cnt * TCP_KEEPINTVL) / TCP_SLOW_INTERVAL)
            {
                tcp_keepalive(pcb);
                pcb->keep_cnt++;
            }
        }

        /* If this PCB has queued out of sequence data, but has been
           inactive for too long, will drop the data (it will eventually
           be retransmitted). */
#if TCP_QUEUE_OOSEQ
        if (pcb->ooseq != NULL && (UINT32) tcp_ticks - pcb->tmr >= pcb->rto * TCP_OOSEQ_TIMEOUT)
        {
            tcp_segs_free(pcb->ooseq);
            pcb->ooseq = NULL;
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_slowtmr: dropping OOSEQ queued data\n"));
        }
#endif /* TCP_QUEUE_OOSEQ */

        /* Check if this PCB has stayed too long in SYN-RCVD */
        if (pcb->state == SYN_RCVD)
        {
            if ((UINT32) (tcp_ticks - pcb->tmr) > TCP_SYN_RCVD_TIMEOUT / TCP_SLOW_INTERVAL)
            {
                ++pcb_remove;
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_slowtmr: removing pcb stuck in SYN-RCVD\n"));
            }
        }

        /* Check if this PCB has stayed too long in LAST-ACK */
        if (pcb->state == LAST_ACK)
        {
            if ((UINT32) (tcp_ticks - pcb->tmr) > 2 * TCP_MSL / TCP_SLOW_INTERVAL)
            {
                ++pcb_remove;
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_slowtmr: removing pcb stuck in LAST-ACK\n"));
            }
        }

        /* If the PCB should be removed, do it. */
        if (pcb_remove)
        {
            tcp_pcb_purge(pcb);
            /* Remove PCB from tcp_active_pcbs list. */
            if (prev != NULL)
            {
                LWIP_ASSERT("tcp_slowtmr: middle tcp != tcp_active_pcbs", pcb != tcp_active_pcbs);
                prev->next = pcb->next;
            }
            else
            {
                /* This PCB was the first. */
                LWIP_ASSERT("tcp_slowtmr: first pcb == tcp_active_pcbs", tcp_active_pcbs == pcb);
                tcp_active_pcbs = pcb->next;
            }
       if(connect_timeout)
       	{
	   TCP_EVENT_ERR(pcb->errf, pcb->callback_arg, ERR_TCPIP_CONN_TIMEOUT);
       	}
	else
            TCP_EVENT_ERR(pcb->errf, pcb->callback_arg, ERR_TCPIP_ABRT);

            pcb2 = pcb->next;
            memp_free(MEMP_TCP_PCB, pcb);
            pcb = pcb2;
        }
        else
        {

            /* We check if we should poll the connection. */
            ++pcb->polltmr;
            if (pcb->polltmr >= pcb->pollinterval)
            {
                pcb->polltmr = 0;
                //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_slowtmr: polling application\n"));
                TCP_EVENT_POLL(pcb, err);
                if (err == ERR_TCPIP_OK)
                {
                    tcp_output(pcb);
                }
            }

            prev = pcb;
            pcb = pcb->next;
        }
    }

    /* Steps through all of the TIME-WAIT PCBs. */
    prev = NULL;
    pcb = tcp_tw_pcbs;
    while (pcb != NULL)
    {
        LWIP_ASSERT("tcp_slowtmr: TIME-WAIT pcb->state == TIME-WAIT", pcb->state == TIME_WAIT);
        pcb_remove = 0;

        /* Check if this PCB has stayed long enough in TIME-WAIT */
        if ((UINT32) (tcp_ticks - pcb->tmr) > 2 * TCP_MSL / TCP_SLOW_INTERVAL)
        {
            ++pcb_remove;
        }

        /* If the PCB should be removed, do it. */
        if (pcb_remove)
        {
            tcp_pcb_purge(pcb);
            /* Remove PCB from tcp_tw_pcbs list. */
            if (prev != NULL)
            {
                LWIP_ASSERT("tcp_slowtmr: middle tcp != tcp_tw_pcbs", pcb != tcp_tw_pcbs);
                prev->next = pcb->next;
            }
            else
            {
                /* This PCB was the first. */
                LWIP_ASSERT("tcp_slowtmr: first pcb == tcp_tw_pcbs", tcp_tw_pcbs == pcb);
                tcp_tw_pcbs = pcb->next;
            }
            pcb2 = pcb->next;
            memp_free(MEMP_TCP_PCB, pcb);
            pcb = pcb2;
        }
        else
        {
            prev = pcb;
            pcb = pcb->next;
        }
    }
}

/**
 * Is called every TCP_FAST_INTERVAL (250 ms) and sends delayed ACKs.
 */
VOID tcp_fasttmr(VOID)
{
    struct tcp_pcb *pcb;

    /* send delayed ACKs */
    for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next)
    {
    TCP_ASSERT(pcb!=pcb->next);
        if (pcb->flags & TF_ACK_DELAY)
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_fasttmr: delayed ACK\n"));
            tcp_ack_now(pcb);
            pcb->flags &= ~(TF_ACK_DELAY | TF_ACK_NOW);
        }
    }
}

/**
 * Deallocates a list of TCP segments (tcp_seg structures).
 *
 */
UINT8 tcp_segs_free(struct tcp_seg *seg)
{
    UINT8 count = 0;
    struct tcp_seg *next;
    while (seg != NULL)
    {
        next = seg->next;
        count += tcp_seg_free(seg);
        seg = next;
    }
    return count;
}

/**
 * Frees a TCP segment.
 *
 */
UINT8 tcp_seg_free(struct tcp_seg * seg)
{
    UINT8 count = 0;

    if (seg != NULL)
    {
        if (seg->p != NULL)
        {
            count = pbuf_free(seg->p);
#if TCP_DEBUG
            seg->p = NULL;
#endif /* TCP_DEBUG */
        }
        memp_free(MEMP_TCP_SEG, seg);
    }
    return count;
}

/**
 * Sets the priority of a connection.
 *
 */
VOID tcp_setprio(struct tcp_pcb *pcb, UINT8 prio)
{
    pcb->prio = prio;
}

#if TCP_QUEUE_OOSEQ

/**
 * Returns a copy of the given TCP segment.
 *
 */
struct tcp_seg *tcp_seg_copy(struct tcp_seg *seg)
{
    struct tcp_seg *cseg;

    cseg = memp_malloc(MEMP_TCP_SEG);
    if (cseg == NULL)
    {
        return NULL;
    }
    SUL_MemCopy8((UINT8 *) cseg, (const UINT8 *) seg, sizeof(struct tcp_seg));
    pbuf_ref(cseg->p);
    return cseg;
}
#endif

#if LWIP_CALLBACK_API
static err_t tcp_recv_null(VOID *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    arg = arg;
    if (p != NULL)
    {
        pbuf_free(p);
    }
    else if (err == ERR_TCPIP_OK)
    {
        return tcp_close(pcb);
    }
    return ERR_TCPIP_OK;
}
#endif /* LWIP_CALLBACK_API */

static VOID tcp_kill_prio(UINT8 prio)
{
    struct tcp_pcb *pcb, *inactive;
    UINT32 inactivity;
    UINT8 mprio;

    mprio = TCP_PRIO_MAX;

    /* We kill the oldest active connection that has lower priority than
       prio. */
    inactivity = 0;
    inactive = NULL;
    for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next)
    {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_kill_prio: pcb->prio 0x%x; tcp_ticks:0x%x; pcb->tmr:0x%x\n"),pcb->prio, tcp_ticks, pcb->tmr);

        if (pcb->prio <= prio && pcb->prio <= mprio && (UINT32) (tcp_ticks - pcb->tmr) >= inactivity)
        {
            inactivity = tcp_ticks - pcb->tmr;
            inactive = pcb;
            mprio = pcb->prio;
        }
    }
    if (inactive != NULL)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_kill_prio: killing oldest PCB %p (0x%x)\n"), (VOID *) inactive, inactivity);
        tcp_abort(inactive);
    }
}

static VOID tcp_kill_timewait(VOID)
{
    struct tcp_pcb *pcb, *inactive;
    UINT32 inactivity;

    inactivity = 0;
    inactive = NULL;
    for (pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next)
    {
        if ((UINT32) (tcp_ticks - pcb->tmr) >= inactivity)
        {
            inactivity = tcp_ticks - pcb->tmr;
            inactive = pcb;
        }
    }
    if (inactive != NULL)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_kill_timewait: killing oldest TIME-WAIT PCB %p (0x%x)\n"), (VOID *) inactive, inactivity);
        tcp_abort(inactive);
    }
}

struct tcp_pcb *tcp_alloc(UINT8 prio)
{
    struct tcp_pcb *pcb;
    UINT32 iss;
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function tcp_alloc prio = 0x%x)\n"), prio);

    pcb = memp_malloc(MEMP_TCP_PCB);
    if (pcb == NULL)
    {
        /* Try killing oldest connection in TIME-WAIT. */
      CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_alloc: killing off oldest TIME-WAIT connection\n"));
      tcp_kill_timewait();
      pcb = memp_malloc(MEMP_TCP_PCB);
      if (pcb == NULL)
      {
        tcp_kill_prio(prio);
        pcb = memp_malloc(MEMP_TCP_PCB);
      }
    }
    if (pcb != NULL)
    {
        SUL_MemSet8(pcb, 0, sizeof(struct tcp_pcb));
        pcb->prio = TCP_PRIO_NORMAL;
        pcb->snd_buf = TCP_SND_BUF;
        pcb->snd_queuelen = 0;
        pcb->rcv_wnd = TCP_WND;
        pcb->tos = 0;
        pcb->ttl = TCP_TTL;
        pcb->mss = TCP_MSS;
        pcb->rto = 3000 / TCP_SLOW_INTERVAL;
        pcb->sa = 0;
        pcb->sv = 3000 / TCP_SLOW_INTERVAL;
        pcb->rtime = 0;
        pcb->cwnd = 1;
        iss = tcp_next_iss();
        pcb->snd_wl2 = iss;
        pcb->snd_nxt = iss;
        pcb->snd_max = iss;
        pcb->lastack = iss;
        pcb->snd_lbb = iss;
        pcb->tmr = tcp_ticks;

        pcb->polltmr = 0;
	 	    pcb->flags = TF_NODELAY;

#if LWIP_CALLBACK_API
        pcb->recv = tcp_recv_null;
#endif /* LWIP_CALLBACK_API */

        /* Init KEEPALIVE timer */
        pcb->keepalive = TCP_KEEPDEFAULT;
        pcb->keep_cnt = 0;
	 	    pcb->so_options = SOF_KEEPALIVE;   //added by dupb
    }
    return pcb;
}

/**
 * Creates a new TCP protocol control block but doesn't place it on
 * any of the TCP PCB lists.
 *
 * @internal: Maybe there should be a idle TCP PCB list where these
 * PCBs are put on. We can then implement port reservation using
 * tcp_bind(). Currently, we lack this (BSD socket type of) feature.
 */

struct tcp_pcb *tcp_new(VOID)
{
    return tcp_alloc(TCP_PRIO_NORMAL);
}

/*
 * tcp_arg():
 *
 * Used to specify the argument that should be passed callback
 * functions.
 *
 */

VOID tcp_arg(struct tcp_pcb *pcb, VOID *arg)
{
    pcb->callback_arg = arg;
}

#if LWIP_CALLBACK_API

/**
 * Used to specify the function that should be called when a TCP
 * connection receives data.
 *
 */
VOID tcp_recv(struct tcp_pcb *pcb, err_t(*recv) (VOID *arg, struct tcp_pcb * tpcb, struct pbuf * p, err_t err))
{
    pcb->recv = recv;
}

/**
 * Used to specify the function that should be called when TCP data
 * has been successfully delivered to the remote host.
 *
 */

VOID tcp_sent(struct tcp_pcb *pcb, err_t(*sent) (VOID *arg, struct tcp_pcb * tpcb, UINT16 len))
{
    pcb->sent = sent;
}

/**
 * Used to specify the function that should be called when a fatal error
 * has occured on the connection.
 *
 */
VOID tcp_err(struct tcp_pcb *pcb, VOID (*errf) (VOID *arg, err_t err))
{
    pcb->errf = errf;
}

/**
 * Used for specifying the function that should be called when a
 * LISTENing connection has been connected to another host.
 *
 */
VOID tcp_accept(struct tcp_pcb *pcb, err_t(*accept) (VOID *arg, struct tcp_pcb * newpcb, err_t err))
{
    ((struct tcp_pcb_listen *) pcb)->accept = accept;
}
#endif /* LWIP_CALLBACK_API */

/**
 * Used to specify the function that should be called periodically
 * from TCP. The interval is specified in terms of the TCP coarse
 * timer interval, which is called twice a second.
 *
 */
VOID tcp_poll(struct tcp_pcb *pcb, err_t(*poll) (VOID *arg, struct tcp_pcb * tpcb), UINT8 interval)
{
#if LWIP_CALLBACK_API
    pcb->poll = poll;
#endif /* LWIP_CALLBACK_API */
    pcb->pollinterval = interval;
}

/**
 * Purges a TCP PCB. Removes any buffered data and frees the buffer memory.
 *
 */
VOID tcp_pcb_purge(struct tcp_pcb *pcb)
{
    if (pcb->state != CLOSED && pcb->state != TIME_WAIT && pcb->state != LISTEN)
    {

        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_pcb_purge\n"));

        if (pcb->unsent != NULL)
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_pcb_purge: not all data sent\n"));
        }
        if (pcb->unacked != NULL)
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_pcb_purge: data left on ->unacked\n"));
        }
#if TCP_QUEUE_OOSEQ        /* LW */
        if (pcb->ooseq != NULL)
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_pcb_purge: data left on ->ooseq\n"));
        }

        tcp_segs_free(pcb->ooseq);
        pcb->ooseq = NULL;
#endif /* TCP_QUEUE_OOSEQ */
        tcp_segs_free(pcb->unsent);
        tcp_segs_free(pcb->unacked);
        pcb->unacked = pcb->unsent = NULL;
    }
}

/**
 * Purges the PCB and removes it from a PCB list. Any delayed ACKs are sent first.
 *
 */
BOOL tcp_pcb_remove(struct tcp_pcb **pcblist, struct tcp_pcb *pcb)
{
    struct tcp_pcb *tmp_pcb;
	tmp_pcb = *pcblist;
	for(tmp_pcb = *pcblist; tmp_pcb!= NULL; tmp_pcb=tmp_pcb->next)
		{
		  if(pcb==tmp_pcb)
		  	break;
		}
	if(tmp_pcb==NULL)
		return FALSE;
	
    TCP_RMV(pcblist, pcb);

    tcp_pcb_purge(pcb);

    /* if there is an outstanding delayed ACKs, send it */
    if (pcb->state != TIME_WAIT && pcb->state != LISTEN && pcb->flags & TF_ACK_DELAY)
    {
        pcb->flags |= TF_ACK_NOW;
        tcp_output(pcb);
    }
    pcb->state = CLOSED;

    LWIP_ASSERT("tcp_pcb_remove: tcp_pcbs_sane()", tcp_pcbs_sane());
	return TRUE;
}

BOOL tcp_pcb_remove_all(struct tcp_pcb *pcb,UINT8 flag)
{
      BOOL ret = FALSE;
ret = tcp_pcb_remove(&tcp_active_pcbs, pcb);
if(ret)
{
       if(flag!=1)
	    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_pcb_remove from  tcp_active_pcbs, expect in  : 0x%x \n"),flag);

       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_pcb_remove from  tcp_active_pcbs, TCP state: 0x%x \n"),pcb->state);

	return TRUE;
}
ret =  tcp_pcb_remove(&tcp_tw_pcbs, pcb);
if(ret)
{
       if(flag!=2)
	      CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_pcb_remove from  tcp_active_pcbs, expect in  : 0x%x \n"),flag);

       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_pcb_remove from  tcp_tw_pcbs, TCP state: 0x%x \n"),pcb->state);

	return TRUE;
}
if(tcp_bound_pcbs!=NULL){
	 struct tcp_pcb *tmp_pcb;
	
	for(tmp_pcb = tcp_bound_pcbs; tmp_pcb!= NULL; tmp_pcb=tmp_pcb->next)
	{
	  if(pcb==tmp_pcb)
	  	break;
	}
	if(tmp_pcb==NULL)
		return FALSE;
      TCP_RMV(&tcp_bound_pcbs, pcb);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_pcb_remove from  tcp_bound_pcbs , TCP state: 0x%x , flag: 0x%x\n"),pcb->state,flag);

	return TRUE;
}else{

       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp_pcb_remove from  no list , TCP state: 0x%x , flag: 0x%x\n"),pcb->state,flag);

}
 return FALSE;
}





/**
 * Calculates a new initial sequence number for new connections.
 *
 */
UINT32 tcp_next_iss(VOID)
{
    static UINT32 iss = 6510;

    iss += tcp_ticks;    /* XXX */
    return iss;
}

#if TCP_DEBUG || TCP_INPUT_DEBUG || TCP_OUTPUT_DEBUG
//#define CFW_TCPIP_TS_ID_SXS (_MMI |TLEVEL(4))
VOID tcp_debug_print(struct tcp_hdr *tcphdr)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("TCP header:\n"));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("+-------------------------------+\n"));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("|    0x%5x      |    0x%5x      | (src port, dest port)\n"), ntohs(tcphdr->src), ntohs(tcphdr->dest));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("+-------------------------------+\n"));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("|           0x%010x          | (seq no)\n"), ntohl(tcphdr->seqno));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("+-------------------------------+\n"));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("|           0x%010x          | (ack no)\n"), ntohl(tcphdr->ackno));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("+-------------------------------+\n"));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("| 0x%2x |   |0x%x 0x%x 0x%x 0x%x 0x%x 0x%x|     0x%5x     | (hdrlen, flags ("),
                TCPH_HDRLEN(tcphdr), TCPH_FLAGS(tcphdr) >> 5 & 1, TCPH_FLAGS(tcphdr) >> 4 & 1, TCPH_FLAGS(tcphdr) >> 3 & 1, TCPH_FLAGS(tcphdr) >> 2 & 1, TCPH_FLAGS(tcphdr) >> 1 & 1, TCPH_FLAGS(tcphdr) & 1, ntohs(tcphdr->wnd));
    tcp_debug_print_flags(TCPH_FLAGS(tcphdr));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("), win)\n"));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("+-------------------------------+\n"));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("|    0x%04x     |     0x%5x     | (chksum, urgp)\n"), ntohs(tcphdr->chksum), ntohs(tcphdr->urgp));
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("+-------------------------------+\n"));
}

VOID tcp_debug_print_state(UINT8 s)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("State: "));
    switch (s)
    {
    case CLOSED:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CLOSED\n"));
        break;
    case LISTEN:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("LISTEN\n"));
        break;
    case SYN_SENT:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("SYN_SENT\n"));
        break;
    case SYN_RCVD:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("SYN_RCVD\n"));
        break;
    case ESTABLISHED:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ESTABLISHED\n"));
        break;
    case FIN_WAIT_1:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("FIN_WAIT_1\n"));
        break;
    case FIN_WAIT_2:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("FIN_WAIT_2\n"));
        break;
    case CLOSE_WAIT:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CLOSE_WAIT\n"));
        break;
    case CLOSING:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CLOSING\n"));
        break;
    case LAST_ACK:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("LAST_ACK\n"));
        break;
    case TIME_WAIT:
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("TIME_WAIT\n"));
        break;
    }
}

VOID tcp_debug_print_flags(UINT8 flags)
{
    if (flags & TCP_FIN)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("FIN "));
    }
    if (flags & TCP_SYN)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("SYN "));
    }
    if (flags & TCP_RST)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("RST "));
    }
    if (flags & TCP_PSH)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("PSH "));
    }
    if (flags & TCP_ACK)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ACK "));
    }
    if (flags & TCP_URG)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("URG "));
    }
    if (flags & TCP_ECE)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ECE "));
    }
    if (flags & TCP_CWR)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CWR "));
    }
}

VOID tcp_debug_print_pcbs(VOID)
{
    struct tcp_pcb *pcb;
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("Active PCB states:\n"));
    for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("Local port 0x%x, foreign port 0x%x snd_nxt 0x%x rcv_nxt 0x%x "), pcb->local_port, pcb->remote_port, pcb->snd_nxt, pcb->rcv_nxt);
        tcp_debug_print_state(pcb->state);
    }
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("Listen PCB states:\n"));
    for (pcb = (struct tcp_pcb *) tcp_listen_pcbs.pcbs; pcb != NULL; pcb = pcb->next)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("Local port 0x%x, foreign port 0x%x snd_nxt 0x%x rcv_nxt 0x%x "), pcb->local_port, pcb->remote_port, pcb->snd_nxt, pcb->rcv_nxt);
        tcp_debug_print_state(pcb->state);
    }
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("TIME-WAIT PCB states:\n"));
    for (pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("Local port 0x%x, foreign port 0x%x snd_nxt 0x%x rcv_nxt 0x%x "), pcb->local_port, pcb->remote_port, pcb->snd_nxt, pcb->rcv_nxt);
        tcp_debug_print_state(pcb->state);
    }
}

INT16 tcp_pcbs_sane(VOID)
{
    struct tcp_pcb *pcb;
    for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next)
    {
        LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != CLOSED", pcb->state != CLOSED);
        LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != LISTEN", pcb->state != LISTEN);
        LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != TIME-WAIT", pcb->state != TIME_WAIT);
    }
    for (pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next)
    {
        LWIP_ASSERT("tcp_pcbs_sane: tw pcb->state == TIME-WAIT", pcb->state == TIME_WAIT);
    }
    return 1;
}
#endif /* TCP_DEBUG */
#endif /* CII_TCP */
#endif

