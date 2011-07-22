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

//#include <cswtype.h>

//#include <cfw.h>
//#include "tcpip_opt.h"
#include "tcpip_memp.h"
#include "tcpip_socket_data.h"
#include <csw.h>
#include "tcpip_arch.h"
#include "tcpip_api_msg.h"
#include "tcpip_sys.h"
#include "tcpip.h"
//#include "cos.h"
//#include "tcpip_sys.h"
#if LWIP_RAW
static UINT8 recv_raw(VOID *arg, struct raw_pcb *pcb, struct pbuf *p, struct ip_addr *addr)
{
    struct netbuf *buf;
    struct netconn *conn;
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function recv_raw\n"));
    
    conn = arg;
    if (!conn)
        return 0;
    
//    if (conn->recvmbox != SYS_MBOX_NULL)
    {
        if (!(buf = memp_malloc(MEMP_NETBUF)))
        {
            return 0;
        }
        pbuf_ref(p);
        buf->p = p;
        buf->ptr = p;
        buf->fromaddr = addr;
        buf->fromport = pcb->protocol;
        
        conn->recv_avail += p->tot_len;
        /* Register event with callback */
   //     if (conn->callback)
  //          (*conn->callback) (conn, NETCONN_EVT_RCVPLUS, p->tot_len);
  //      sys_mbox_post(conn->recvmbox, buf);
    }
    
    return 0;		/* do not eat the packet */
}
#endif
#if CII_UDP
static VOID recv_udp(VOID *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, UINT16 port)
{
    struct netbuf *buf;
    struct netconn *conn;
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function recv_udp\n"));    
    conn = arg;
    
    if (conn == NULL)
    {
        pbuf_free(p);
        return;
    }
    //if (conn->recvmbox != SYS_MBOX_NULL)
    {
        buf = memp_malloc(MEMP_NETBUF);
        if (buf == NULL)
        {
            pbuf_free(p);
            return;
        }
        else
        {
            buf->p = p;
            buf->ptr = p;
            buf->fromaddr = addr;
            buf->fromport = port;
        }
        
        conn->recv_avail += p->tot_len;
        /* Register event with callback */
     //   if (conn->callback)
   //         (*conn->callback) (conn, NETCONN_EVT_RCVPLUS, p->tot_len);
        //sys_mbox_post(conn->recvmbox, buf);
	// post the p to up layer
        // store the recevie data in the buffer 
        // the socket id is the conn->socket 
        socketdata_store(conn->socket, buf);
        // post the event to app or mmi
        // 
        sys_post_event_to_APP(EV_CFW_TCPIP_REV_DATA_IND, conn->socket, p->tot_len, 1, 0, 0);	
    }
}
#endif /* CII_UDP */
#if CII_TCP
const UINT8 test_RecBuf[2048]={0,};
UINT32 TcpipSocketRecvfrom_test(SOCKET nSocket, VOID *pMem, INT32 nLen, UINT32 nFlags, CFW_TCPIP_SOCKET_ADDR *from, INT32 * fromlen);

static err_t recv_tcp(VOID *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
#if 0
    struct netconn *conn;
    UINT16 len;
    
    conn = arg;
    
    if (conn == NULL)
    {
        pbuf_free(p);
        return ERR_TCPIP_VAL;
    }
    
    if (conn->recvmbox != SYS_MBOX_NULL)
    {
        
        conn->err = err;
        if (p != NULL)
        {
            len = p->tot_len;
            conn->recv_avail += len;
        }
        else
            len = 0;
        /* Register event with callback */
        if (conn->callback)
            (*conn->callback) (conn, NETCONN_EVT_RCVPLUS, len);
        sys_mbox_post(conn->recvmbox, p);
    }
    return ERR_TCPIP_OK;
#endif
    struct netconn *conn;
    UINT16 len;
    conn = arg;
    
    //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function recv_tcp\n"));        
    if (conn == NULL)
    {
        pbuf_free(p);
        return ERR_TCPIP_VAL;
    }        
    conn->err = err;
    if (p != NULL)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function recv_tcp p != NULL send data ind to uplayer\n"));            
        len = p->tot_len;
        conn->recv_avail += len;
        
        // store the recevie data in the buffer 
        // the socket id is the conn->socket the data address is p
        socketdata_store(conn->socket, p);//remove by wuys 2008-09-24
        // post the event to app or mmi
        // 

        // modify bu wuys 2008-09-05 for test gprs
        #if 0
        UINT32 rev_num =0;




		if (pcb != NULL)
    {
            pbuf_free(p);
       
            tcp_recved(pcb, p->tot_len);
        
    }
        //rev_num = TcpipSocketRecvfrom_test((INT8 )conn->socket, (VOID*)test_RecBuf, 2048, 0, NULL, NULL);
        if(rev_num >0)
           CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("TcpipSocketRecvfrom_test receive: %d bytes\n"),rev_num);            
#else
        sys_post_event_to_APP(EV_CFW_TCPIP_REV_DATA_IND, conn->socket, p->tot_len, 0, 0, 0);
#endif
        //modify end
        
        
    }
    else
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function recv_tcp p == NULL send close ind to uplayer\n"));            
        len = 0;
	      //conn->err = ERR_TCPIP_CLSD;// dupb added   //wuys modify 2009-03-26
	      conn->state = NETCONN_NONE;
        sys_post_event_to_APP(EV_CFW_TCPIP_CLOSE_IND, conn->socket, conn->err, 0, 0, 0);
        
    }
    
    
    return ERR_TCPIP_OK;
}

static err_t poll_tcp(VOID *arg, struct tcp_pcb *pcb)
{
    struct netconn *conn;
//    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function poll_tcp\n"));    
    conn = arg;
//    if (conn != NULL && (conn->state == NETCONN_WRITE || conn->state == NETCONN_CLOSE) && conn->sem != SYS_SEM_NULL)
//    {
//        sys_sem_signal(conn->sem);
 //   }
    return ERR_TCPIP_OK;
}

static err_t sent_tcp(VOID *arg, struct tcp_pcb *pcb, UINT16 len)
{
    struct netconn *conn;
    //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function sent_tcp\n"));    
    conn = arg;
//  if (conn != NULL && conn->sem != SYS_SEM_NULL)
    {
        //sys_sem_signal(conn->sem);
      //  if(conn->pcb.tcp->snd_buf == TCP_SND_BUF)
        {
            sys_post_event_to_APP( EV_CFW_TCPIP_SOCKET_SEND_RSP, conn->socket, 0, 0, 0, 0);        	
        }
    }
//    if (conn && conn->callback)
//        if (tcp_sndbuf(conn->pcb.tcp) > TCP_SNDLOWAT)
//            (*conn->callback) (conn, NETCONN_EVT_SENDPLUS, len);
        
        return ERR_TCPIP_OK;
}

static VOID err_tcp(VOID *arg, err_t err)
{
    struct netconn *conn;
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function err_tcp\n"));    
    conn = arg;
    
    conn->pcb.tcp = NULL;
    
    conn->err = err;
    conn->state = NETCONN_NONE;
    UINT32 errcode =0;
	errcode = (UINT32)(0-conn->err);
 //   sys_post_event_to_APP( EV_CFW_TCPIP_ERR_IND, conn->socket, conn->err, 0, 0, 0);
 sys_post_event_to_APP( EV_CFW_TCPIP_ERR_IND, conn->socket, errcode, 0, 0, 0);
#if 0	
  //  if (conn->recvmbox != SYS_MBOX_NULL)
    {
        /* Register event with callback */
        if (conn->callback)
            (*conn->callback) (conn, NETCONN_EVT_RCVPLUS, 0);
//        sys_mbox_post(conn->recvmbox, NULL);
    }
//    if (conn->mbox != SYS_MBOX_NULL)
    {
//        sys_mbox_post(conn->mbox, NULL);
    }
//    if (conn->acceptmbox != SYS_MBOX_NULL)
    {
        /* Register event with callback */
        if (conn->callback)
            (*conn->callback) (conn, NETCONN_EVT_RCVPLUS, 0);
//        sys_mbox_post(conn->acceptmbox, NULL);
    }
    if (conn->sem != SYS_SEM_NULL)
    {
        sys_sem_signal(conn->sem);
    }
	#endif
}

static VOID setup_tcp(struct netconn *conn)
{
    struct tcp_pcb *pcb;
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function setup_tcp\n"));    
    pcb = conn->pcb.tcp;
    tcp_arg(pcb, conn);
    tcp_recv(pcb, recv_tcp);
    tcp_sent(pcb, sent_tcp);
    tcp_poll(pcb, poll_tcp, 4);
    tcp_err(pcb, err_tcp);
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function setup_tcp over\n"));    	
}

static err_t accept_function(VOID *arg, struct tcp_pcb *newpcb, err_t err)
{
   // sys_mbox_t mbox;
    struct netconn *newconn;
    struct netconn *conn;
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function accept_function\n"));    
#if API_MSG_DEBUG
#if TCP_DEBUG
    tcp_debug_print_state(newpcb->state);
#endif /* TCP_DEBUG */
#endif /* API_MSG_DEBUG */
    conn = (struct netconn *) arg;
 //   mbox = conn->acceptmbox;
    newconn = memp_malloc(MEMP_NETCONN);
    if (newconn == NULL)
    {
        return ERR_TCPIP_MEM;
    }
    newconn->type = NETCONN_TCP;
    newconn->pcb.tcp = newpcb;
    setup_tcp(newconn);
#if 0
    newconn->recvmbox = sys_mbox_new();
  //  if (newconn->recvmbox == SYS_MBOX_NULL)
    {
        memp_free(MEMP_NETCONN, newconn);
        return ERR_TCPIP_MEM;
    }
    newconn->mbox = sys_mbox_new();
    if (newconn->mbox == SYS_MBOX_NULL)
    {
        sys_mbox_free(newconn->recvmbox);
        memp_free(MEMP_NETCONN, newconn);
        return ERR_TCPIP_MEM;
    }
    newconn->sem = sys_sem_new(0);
    if (newconn->sem == SYS_SEM_NULL)
    {
        sys_mbox_free(newconn->recvmbox);
        sys_mbox_free(newconn->mbox);
        memp_free(MEMP_NETCONN, newconn);
        return ERR_TCPIP_MEM;
    }
    newconn->acceptmbox = SYS_MBOX_NULL;
    newconn->err = err;
    /* Register event with callback */
    if (conn->callback)
    {
        (*conn->callback) (conn, NETCONN_EVT_RCVPLUS, 0);
        /* We have to set the callback here even though
        * the new socket is unknown. Mark the socket as -1. */
        newconn->callback = conn->callback;
        newconn->socket = -1;
    }
	#endif
    
//    sys_mbox_post(mbox, newconn);
    socketdata_store(conn->socket, newconn);
    sys_post_event_to_APP(EV_CFW_TCPIP_ACCEPT_IND, conn->socket, 0, 0, 0, 0);	
    return ERR_TCPIP_OK;
}
#endif /* CII_TCP */

static VOID do_newconn(struct api_msg_msg *msg)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_newconn\n"));
    if (msg->conn->pcb.tcp != NULL)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("This new connection already has a PCB allocated. \n"));    
        /* This "new" connection already has a PCB allocated. */
        /* Is this an error condition? Should it be deleted? 
        We currently just are happy and return. */
//        sys_mbox_post(msg->conn->mbox, NULL);
        return;
    }
    
    msg->conn->err = ERR_TCPIP_OK;
    /* Allocate a PCB for this connection */
    switch (msg->conn->type)
    {
#if LWIP_RAW
    case NETCONN_RAW:
        msg->conn->pcb.raw = raw_new(msg->msg.bc.port);	/* misusing the port field */
        raw_recv(msg->conn->pcb.raw, recv_raw, msg->conn);
        break;
#endif
#if CII_UDP
    case NETCONN_UDPLITE:
        msg->conn->pcb.udp = udp_new();
        if (msg->conn->pcb.udp == NULL)
        {
            msg->conn->err = ERR_TCPIP_MEM;
            break;
        }
        udp_setflags(msg->conn->pcb.udp, UDP_FLAGS_UDPLITE);
        udp_recv(msg->conn->pcb.udp, recv_udp, msg->conn);
        break;
    case NETCONN_UDPNOCHKSUM:
        msg->conn->pcb.udp = udp_new();
        if (msg->conn->pcb.udp == NULL)
        {
            msg->conn->err = ERR_TCPIP_MEM;
            break;
        }
        udp_setflags(msg->conn->pcb.udp, UDP_FLAGS_NOCHKSUM);
        udp_recv(msg->conn->pcb.udp, recv_udp, msg->conn);
        break;
    case NETCONN_UDP:
        msg->conn->pcb.udp = udp_new();
        if (msg->conn->pcb.udp == NULL)
        {
            msg->conn->err = ERR_TCPIP_MEM;
            break;
        }
        udp_recv(msg->conn->pcb.udp, recv_udp, msg->conn);
        break;
#endif /* CII_UDP */
#if CII_TCP
    case NETCONN_TCP:
        msg->conn->pcb.tcp = tcp_new();
        if (msg->conn->pcb.tcp == NULL)
        {
            msg->conn->err = ERR_TCPIP_MEM;
            break;
        }
        setup_tcp(msg->conn);
        break;
#endif
    }
   // msg->conn->pcb.tcp->local_ip.addr = g_ip_addr.addr;
//    sys_mbox_post(msg->conn->mbox, NULL);
}

static VOID do_delconn(struct api_msg_msg *msg)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_delconn\n"));
    if (msg->conn->pcb.tcp != NULL)
    {
        switch (msg->conn->type)
        {
#if LWIP_RAW
        case NETCONN_RAW:
            raw_remove(msg->conn->pcb.raw);
            break;
#endif
#if CII_UDP
        case NETCONN_UDPLITE:
            /* FALLTHROUGH */
        case NETCONN_UDPNOCHKSUM:
            /* FALLTHROUGH */
        case NETCONN_UDP:
            msg->conn->pcb.udp->recv_arg = NULL;
            udp_remove(msg->conn->pcb.udp);
            break;
#endif /* CII_UDP */
#if CII_TCP
        case NETCONN_TCP:
            if (msg->conn->pcb.tcp->state == LISTEN)
            {
                tcp_arg(msg->conn->pcb.tcp, NULL);
                tcp_accept(msg->conn->pcb.tcp, NULL);
                tcp_close(msg->conn->pcb.tcp);
            }
            else
            {
                tcp_arg(msg->conn->pcb.tcp, NULL);
                tcp_sent(msg->conn->pcb.tcp, NULL);
                tcp_recv(msg->conn->pcb.tcp, NULL);
                tcp_poll(msg->conn->pcb.tcp, NULL, 0);
                tcp_err(msg->conn->pcb.tcp, NULL);
                if (tcp_close(msg->conn->pcb.tcp) != ERR_TCPIP_OK)
                {
                    tcp_abort(msg->conn->pcb.tcp);
                }
            }
#endif
        default:
            break;
        }
    }
    /* Trigger select() in socket layer */
//    if (msg->conn->callback)
 //   {
//        (*msg->conn->callback) (msg->conn, NETCONN_EVT_RCVPLUS, 0);
 //       (*msg->conn->callback) (msg->conn, NETCONN_EVT_SENDPLUS, 0);
 //   }
    
//    if (msg->conn->mbox != SYS_MBOX_NULL)
    {
//        sys_mbox_post(msg->conn->mbox, NULL);
    }
	
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("post close RSP to MMI \n"));	
    sys_post_event_to_APP(EV_CFW_TCPIP_SOCKET_CLOSE_RSP, msg->conn->socket, 0, 0, 0, 0);	
    memp_free(MEMP_NETCONN, msg->conn);   /// need attation dupb added
}

static VOID do_bind(struct api_msg_msg *msg)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_bind\n"));
    if (msg->conn->pcb.tcp == NULL)
    {
        switch (msg->conn->type)
        {
#if LWIP_RAW
        case NETCONN_RAW:
            msg->conn->pcb.raw = raw_new(msg->msg.bc.port);	/* misusing the port field as protocol */
            raw_recv(msg->conn->pcb.raw, recv_raw, msg->conn);
            break;
#endif
#if CII_UDP
        case NETCONN_UDPLITE:
            msg->conn->pcb.udp = udp_new();
            udp_setflags(msg->conn->pcb.udp, UDP_FLAGS_UDPLITE);
            udp_recv(msg->conn->pcb.udp, recv_udp, msg->conn);
            break;
        case NETCONN_UDPNOCHKSUM:
            msg->conn->pcb.udp = udp_new();
            udp_setflags(msg->conn->pcb.udp, UDP_FLAGS_NOCHKSUM);
            udp_recv(msg->conn->pcb.udp, recv_udp, msg->conn);
            break;
        case NETCONN_UDP:
            msg->conn->pcb.udp = udp_new();
            udp_recv(msg->conn->pcb.udp, recv_udp, msg->conn);
            break;
#endif /* CII_UDP */
#if CII_TCP
        case NETCONN_TCP:
            msg->conn->pcb.tcp = tcp_new();
            setup_tcp(msg->conn);
#endif /* CII_TCP */
        default:
            break;
        }
    }
    switch (msg->conn->type)
    {
#if LWIP_RAW
    case NETCONN_RAW:
        msg->conn->err = raw_bind(msg->conn->pcb.raw, msg->msg.bc.ipaddr);
        break;
#endif
#if CII_UDP
    case NETCONN_UDPLITE:
        /* FALLTHROUGH */
    case NETCONN_UDPNOCHKSUM:
        /* FALLTHROUGH */
    case NETCONN_UDP:
        msg->conn->err = udp_bind(msg->conn->pcb.udp, msg->msg.bc.ipaddr, msg->msg.bc.port);
        break;
#endif /* CII_UDP */
#if CII_TCP
    case NETCONN_TCP:
        msg->conn->err = tcp_bind(msg->conn->pcb.tcp, msg->msg.bc.ipaddr, msg->msg.bc.port);
#endif /* CII_TCP */
    default:
        break;
    }
//    sys_mbox_post(msg->conn->mbox, NULL);
}

#if CII_TCP

static err_t do_connected(VOID *arg, struct tcp_pcb *pcb, err_t err)
{
    struct netconn *conn;
    
    conn = arg;
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_connected\n"));
    
    if (conn == NULL)
    {
        return ERR_TCPIP_VAL;
    }
    
    conn->err = err;
    if (conn->type == NETCONN_TCP && err == ERR_TCPIP_OK)
    {
        setup_tcp(conn);
    }
    sys_post_event_to_APP(EV_CFW_TCPIP_SOCKET_CONNECT_RSP, conn->socket, 0, 0, 0, 0);
        
    return ERR_TCPIP_OK;
}
#endif

static VOID do_connect(struct api_msg_msg *msg)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_connect\n"));
    if (msg->conn->pcb.tcp == NULL)
    {
        switch (msg->conn->type)
        {
#if LWIP_RAW
        case NETCONN_RAW:
            msg->conn->pcb.raw = raw_new(msg->msg.bc.port);	/* misusing the port field as protocol */
            raw_recv(msg->conn->pcb.raw, recv_raw, msg->conn);
            break;
#endif
#if CII_UDP
        case NETCONN_UDPLITE:
            msg->conn->pcb.udp = udp_new();
            if (msg->conn->pcb.udp == NULL)
            {
                msg->conn->err = ERR_TCPIP_MEM;
//        sys_mbox_post(msg->conn->mbox, NULL);
                return;
            }
            udp_setflags(msg->conn->pcb.udp, UDP_FLAGS_UDPLITE);
            udp_recv(msg->conn->pcb.udp, recv_udp, msg->conn);
            break;
        case NETCONN_UDPNOCHKSUM:
            msg->conn->pcb.udp = udp_new();
            if (msg->conn->pcb.udp == NULL)
            {
                msg->conn->err = ERR_TCPIP_MEM;
//       sys_mbox_post(msg->conn->mbox, NULL);
                return;
            }
            udp_setflags(msg->conn->pcb.udp, UDP_FLAGS_NOCHKSUM);
            udp_recv(msg->conn->pcb.udp, recv_udp, msg->conn);
            break;
        case NETCONN_UDP:
            msg->conn->pcb.udp = udp_new();
            if (msg->conn->pcb.udp == NULL)
            {
                msg->conn->err = ERR_TCPIP_MEM;
            //    sys_mbox_post(msg->conn->mbox, NULL);
                return;
            }
            udp_recv(msg->conn->pcb.udp, recv_udp, msg->conn);
            break;
#endif /* CII_UDP */
#if CII_TCP
        case NETCONN_TCP:
            msg->conn->pcb.tcp = tcp_new();
            if (msg->conn->pcb.tcp == NULL)
            {
                msg->conn->err = ERR_TCPIP_MEM;
                //	sys_sem_signal(msg->conn->sem);//CtrSem
                //	sys_mbox_post(msg->conn->mbox, NULL);
                return;
            }
#endif
        default:
            break;
        }
    }
    switch (msg->conn->type)
    {
#if LWIP_RAW
    case NETCONN_RAW:
        raw_connect(msg->conn->pcb.raw, msg->msg.bc.ipaddr);
//        sys_mbox_post(msg->conn->mbox, NULL);
        break;
#endif
#if CII_UDP
    case NETCONN_UDPLITE:
        /* FALLTHROUGH */
    case NETCONN_UDPNOCHKSUM:
        /* FALLTHROUGH */
    case NETCONN_UDP:
        udp_connect(msg->conn->pcb.udp, msg->msg.bc.ipaddr, msg->msg.bc.port);
//        sys_mbox_post(msg->conn->mbox, NULL);
        break;
#endif
#if CII_TCP
    case NETCONN_TCP:
        /*    tcp_arg(msg->conn->pcb.tcp, msg->conn); */
        setup_tcp(msg->conn);
        tcp_connect(msg->conn->pcb.tcp, msg->msg.bc.ipaddr, msg->msg.bc.port, do_connected);
        /*tcp_output(msg->conn->pcb.tcp); */
#endif
        
    default:
        break;
    }
}

static VOID do_disconnect(struct api_msg_msg *msg)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_disconnect\n"));    
    switch (msg->conn->type)
    {
#if LWIP_RAW
    case NETCONN_RAW:
        /* Do nothing as connecting is only a helper for upper lwip layers */
        break;
#endif
#if CII_UDP
    case NETCONN_UDPLITE:
        /* FALLTHROUGH */
    case NETCONN_UDPNOCHKSUM:
        /* FALLTHROUGH */
    case NETCONN_UDP:
        udp_disconnect(msg->conn->pcb.udp);
        break;
#endif
    case NETCONN_TCP:
        break;
    }
 //   sys_mbox_post(msg->conn->mbox, NULL);
}

static VOID do_listen(struct api_msg_msg *msg)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_listen\n"));
    if (msg->conn->pcb.tcp != NULL)
    {
        switch (msg->conn->type)
        {
#if LWIP_RAW
        case NETCONN_RAW:
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("api_msg: listen RAW: cannot listen for RAW.\n"));
            break;
#endif
#if CII_UDP
        case NETCONN_UDPLITE:
            /* FALLTHROUGH */
        case NETCONN_UDPNOCHKSUM:
            /* FALLTHROUGH */
        case NETCONN_UDP:
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("api_msg: listen UDP: cannot listen for UDP.\n"));
            break;
#endif /* CII_UDP */
#if CII_TCP
        case NETCONN_TCP:
            msg->conn->pcb.tcp = tcp_listen(msg->conn->pcb.tcp);
            if (msg->conn->pcb.tcp == NULL)
            {
                msg->conn->err = ERR_TCPIP_MEM;
            }
            else
            {
    //            if (msg->conn->acceptmbox == SYS_MBOX_NULL)
                {
  //                  msg->conn->acceptmbox = sys_mbox_new();
    //                if (msg->conn->acceptmbox == SYS_MBOX_NULL)
                    {
                //        msg->conn->err = ERR_TCPIP_MEM;
               //         break;
                    }
                }
                tcp_arg(msg->conn->pcb.tcp, msg->conn);
                tcp_accept(msg->conn->pcb.tcp, accept_function);
            }
#endif
        default:
            break;
        }
    }
//    sys_mbox_post(msg->conn->mbox, NULL);
}

static VOID do_accept(struct api_msg_msg *msg)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_accept\n"));
    if (msg->conn->pcb.tcp != NULL)
    {
        switch (msg->conn->type)
        {
#if LWIP_RAW
        case NETCONN_RAW:
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("api_msg: accept RAW: cannot accept for RAW.\n"));
            break;
#endif
#if CII_UDP
        case NETCONN_UDPLITE:
            /* FALLTHROUGH */
        case NETCONN_UDPNOCHKSUM:
            /* FALLTHROUGH */
        case NETCONN_UDP:
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("api_msg: accept UDP: cannot accept for UDP.\n"));
            break;
#endif /* CII_UDP */
        case NETCONN_TCP:
            break;
        }
    }
}

static VOID do_send(struct api_msg_msg *msg)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_send\n"));
    if (msg->conn->pcb.tcp != NULL)
    {
        switch (msg->conn->type)
        {
#if LWIP_RAW
        case NETCONN_RAW:
            raw_send(msg->conn->pcb.raw, msg->msg.p);
            break;
#endif
#if CII_UDP
        case NETCONN_UDPLITE:
            /* FALLTHROUGH */
        case NETCONN_UDPNOCHKSUM:
            /* FALLTHROUGH */
        case NETCONN_UDP:
            udp_send(msg->conn->pcb.udp, msg->msg.p);
            break;
#endif /* CII_UDP */
        case NETCONN_TCP:
            break;
        }
    }
//    sys_mbox_post(msg->conn->mbox, NULL);
}

static VOID do_recv(struct api_msg_msg *msg)
{
#if 0
#if LWIP_TCP
    if (msg->conn->pcb.tcp != NULL) {
        if (msg->conn->type == NETCONN_TCP) {
            tcp_recved(msg->conn->pcb.tcp, msg->msg.len);
        }
    }
#endif  
    sys_mbox_post(msg->conn->mbox, NULL);
#endif
    //modified by zhangzd
    //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_recv\n"));
    
#if CII_TCP
    if (msg->conn->pcb.tcp != NULL)
    {
        if (msg->conn->type == NETCONN_TCP )
        {
            tcp_recved(msg->conn->pcb.tcp, msg->msg.len);
        }
    }
#endif
//    sys_mbox_post(msg->conn->mbox, NULL);
}

static VOID do_write(struct api_msg_msg *msg)
{
#if CII_TCP
    err_t err;
#endif
    CSW_TRACE(CFW_TCPIPT_TS_ID_SXS, TSTXT("In function do_write\n"));

    if (msg->conn->pcb.tcp != NULL)
    {
        switch (msg->conn->type)
        {
#if LWIP_RAW
        case NETCONN_RAW:
            msg->conn->err = ERR_TCPIP_VAL;
            break;
#endif
#if CII_UDP
        case NETCONN_UDPLITE:
            /* FALLTHROUGH */
        case NETCONN_UDPNOCHKSUM:
            /* FALLTHROUGH */
        case NETCONN_UDP:
            msg->conn->err = ERR_TCPIP_VAL;
            break;
#endif /* CII_UDP */
#if CII_TCP
        case NETCONN_TCP:
            err = tcp_write(msg->conn->pcb.tcp, msg->msg.w.dataptr, msg->msg.w.len, msg->msg.w.copy);
            /* This is the Nagle algorithm: inhibit the sending of new TCP
            segments when new outgoing data arrives from the user if any
            previously transmitted data on the connection remains
            unacknowledged. */
            if (err == ERR_TCPIP_OK && (msg->conn->pcb.tcp->unacked == NULL || (msg->conn->pcb.tcp->flags & TF_NODELAY)))
            {
                tcp_output(msg->conn->pcb.tcp);
            }
	     else
	     {
	  //       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_write case NETCONN_TCP: err = 0x%x, 
		//	 	unacked = 0x%x, flags = 0x%x\n"), err, msg->conn->pcb.tcp->unacked , msg->conn->pcb.tcp->flags);
	     }
            msg->conn->err = err;
//            if (msg->conn->callback)
//                if (err == ERR_TCPIP_OK)
//                {
//                    if (tcp_sndbuf(msg->conn->pcb.tcp) <= TCP_SNDLOWAT)
//                        (*msg->conn->callback) (msg->conn, NETCONN_EVT_SENDMINUS, msg->msg.w.len);
//                }

#endif
        default:
            break;
        }
    }
//    sys_mbox_post(msg->conn->mbox, NULL);
}

static VOID do_close(struct api_msg_msg *msg)
{
    err_t err;
    
    err = ERR_TCPIP_OK;
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function do_close\n"));    
    if (msg->conn->pcb.tcp != NULL)
    {
        switch (msg->conn->type)
        {
#if LWIP_RAW
        case NETCONN_RAW:
            break;
#endif
#if CII_UDP
        case NETCONN_UDPLITE:
            /* FALLTHROUGH */
        case NETCONN_UDPNOCHKSUM:
            /* FALLTHROUGH */
        case NETCONN_UDP:
            break;
#endif /* CII_UDP */
#if CII_TCP
        case NETCONN_TCP:
            if (msg->conn->pcb.tcp->state == LISTEN)
            {
                err = tcp_close(msg->conn->pcb.tcp);
            }
            msg->conn->err = err;
#endif
        default:
            break;
        }
    }
//    sys_mbox_post(msg->conn->mbox, NULL);
}

typedef VOID (*api_msg_decode) (struct api_msg_msg * msg);
static api_msg_decode decode[API_MSG_MAX] = {
        do_newconn,
        do_delconn,
        do_bind,
        do_connect,
        do_disconnect,
        do_listen,
        do_accept,
        do_send,
        do_recv,
        do_write,
        do_close
};
VOID api_msg_input(struct api_msg *msg)
{
    decode[msg->type] (&(msg->msg));
    //add by wuys 2008-08-14   get rid of by wuys 2009-03-26
/*    
    if(msg->msg.conn->err != ERR_TCPIP_OK)
    {
      sys_post_event_to_APP( EV_CFW_TCPIP_ERR_IND, msg->msg.conn->socket, msg->msg.conn->err, 0, 0, 0);
    }
*/    
   //add end
    
}

VOID api_msg_post(struct api_msg *msg)
{
    tcpip_apimsg(msg);
}


//add by wuys for gprs test 2008-09-05
#if 0
UINT32 TcpipSocketRecvfrom_test(SOCKET nSocket, VOID *pMem, INT32 nLen, UINT32 nFlags, CFW_TCPIP_SOCKET_ADDR *from, INT32 * fromlen)
{
	struct socket_dsc *sock;
	struct netbuf *buf;
	UINT16 buflen, copylen;
	struct ip_addr *addr;
	UINT16 port;
	struct api_msg *msg = NULL;  //use for notify downlayer.(added by zhangzd)

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketRecvfrom(%d, %p, len = %d, flag = 0x%x, pfromadd = 0x%x,  pfromlen = 0x%x)\n"), nSocket, pMem, nLen, nFlags, from, fromlen);
	sock = get_socket(nSocket);
	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}
	if(pMem == NULL)
	{
		set_errno(ERR_TCPIP_EINVAL);
		return -1;
	}
	
	/* Check if there is data left from the last recv operation. */
	if (sock->lastdata)
	{
		buf = sock->lastdata;
	}
	else
	{
		/* If this is non-blocking call, then check first */
		if (((nFlags & MSG_DONTWAIT) || (sock->flags & O_NONBLOCK)) && !sock->rcvevent)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketRecvfrom(%d): returning EWOULDBLOCK\n"), nSocket);
			sock_set_errno(sock,  ERR_TCPIP_EWOULDBLOCK);
			return -1;
		}

		/* No data was left from the previous operation, so we try to get
		   some from the network. */
		buf = netconn_recv(sock->conn);

		if (!buf)
		{
			/* We should really do some error checking here. */
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketRecvfrom(%d): buf == NULL!\n"), nSocket);
			sock_set_errno(sock, 0);
			return 0;
		}
	}

	buflen = netbuf_len(buf);

	buflen -= sock->lastoffset;

	if (nLen > buflen)
	{
		copylen = buflen;
	}
	else
	{
		copylen = nLen;
	}

	/* copy the contents of the received buffer into
	   the supplied memory pointer mem */
	netbuf_copy_partial(buf, pMem, copylen, sock->lastoffset);

	/* Check to see from where the data was. */
	if (from && fromlen)
	{
		CFW_TCPIP_SOCKET_ADDR sin;

		addr = netbuf_fromaddr(buf);
		port = netbuf_fromport(buf);

		SUL_MemSet8(&sin, 0, sizeof(sin));
		sin.sin_len = sizeof(sin);
		sin.sin_family = CFW_TCPIP_AF_INET;
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = addr->addr;

		if (*fromlen > sizeof(sin))
			*fromlen = sizeof(sin);

		SUL_MemCopy8(from, &sin, *fromlen);

		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketRecvfrom(%d): addr="), nSocket);
		ip_addr_debug_print(CFW_TCPIP_TS_ID_SXS, addr);
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT (" port=%u nLen=%u\n"), port, copylen);
	}
	else
	{
#if SOCKETS_DEBUG
		addr = netbuf_fromaddr(buf);
		port = netbuf_fromport(buf);

		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketRecvfrom(%d): addr="), nSocket);
		ip_addr_debug_print(CFW_TCPIP_TS_ID_SXS, addr);
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT (" port=%u len=%u\n"), port, copylen);
#endif

	}
	

	/* If this is a TCP socket, check if there is data left in the
	   buffer. If so, it should be saved in the sock structure for next
	   time around. */
	if (netconn_type(sock->conn) == NETCONN_TCP && buflen - copylen > 0)
	{
		sock->lastdata = buf;
		sock->lastoffset += copylen;
	}
	else if(netconn_type(sock->conn) == NETCONN_TCP )
	{
		sock->lastdata = NULL;
		sock->lastoffset = 0;
		if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
	       {
		    sock->conn->err = ERR_TCPIP_MEM;
		    return -1;
	       }
	       msg->msg.msg.len = buf->p->tot_len;
              netbuf_delete(buf);

	       msg->type = API_MSG_RECV;
	       msg->msg.conn = sock->conn;
         //add 

        do_recv(msg->msg);

        //end
	      // api_msg_post(msg);//remove
	}
	else
	{
            sock->lastdata = NULL;
            sock->lastoffset = 0;
             netbuf_delete(buf);	
	}

	sock_set_errno(sock, 0);
	return copylen;
}
#endif
void do_recv_test(struct api_msg_msg * msg)
{
  do_recv(msg);
}
//add end
#endif

