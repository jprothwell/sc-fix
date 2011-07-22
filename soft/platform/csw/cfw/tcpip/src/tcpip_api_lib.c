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

/* This is the part of the API that is linked with
   the application */


#ifdef CFW_TCPIP_SUPPORT
//#include "tcpip_opt.h"
#include "tcpip_memp.h"
//#include <cswtype.h>
#include <csw.h>
#include "tcpip_api.h"
#include "tcpip_api_msg.h"
#include "tcpip_socket_data.h"


struct netbuf *netbuf_new(VOID)
{
	struct netbuf *buf;

	buf = memp_malloc(MEMP_NETBUF);
	if (buf != NULL)
	{
		buf->p = NULL;
		buf->ptr = NULL;
		return buf;
	}
	else
	{
		return NULL;
	}
}

VOID netbuf_delete(struct netbuf *buf)
{
	if (buf != NULL)
	{
		if (buf->p != NULL)
		{
			pbuf_free(buf->p);
			buf->p 	= NULL;
			buf->ptr = NULL;
		}
		memp_free(MEMP_NETBUF, buf);
	}
}

VOID *netbuf_alloc(struct netbuf *buf, UINT16 size)
{
	/* Deallocate any previously allocated memory. */
	if (buf->p != NULL)
	{
		pbuf_free(buf->p);
	}
	buf->p = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
	if (buf->p == NULL)
	{
		return NULL;
	}
	buf->ptr = buf->p;
	return buf->p->payload;
}

VOID netbuf_free(struct netbuf *buf)
{
	if (buf->p != NULL)
	{
		pbuf_free(buf->p);
	}
	buf->p 	=  NULL;
	buf->ptr = NULL;
}

VOID netbuf_ref(struct netbuf *buf, VOID *dataptr, UINT16 size)
{
	if (buf->p != NULL)
	{
		pbuf_free(buf->p);
	}
	buf->p = pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_REF);
	buf->p->payload 	= dataptr;
	buf->p->len 		= size;
	buf->p->tot_len 	= size;
	buf->ptr 		= buf->p;
}

VOID netbuf_chain(struct netbuf *head, struct netbuf *tail)
{
	pbuf_chain(head->p, tail->p);
	head->ptr = head->p;
	memp_free(MEMP_NETBUF, tail);
}

UINT16 netbuf_len(struct netbuf *buf)
{
	return buf->p->tot_len;
}

err_t netbuf_data(struct netbuf * buf, VOID **dataptr, UINT16 * len)
{
	if (buf->ptr == NULL)
	{
		return ERR_TCPIP_BUF;
	}
	*dataptr = buf->ptr->payload;
	*len = buf->ptr->len;
	return ERR_TCPIP_OK;
}

INT8 netbuf_next(struct netbuf * buf)
{
	if (buf->ptr->next == NULL)
	{
		return -1;
	}
	buf->ptr = buf->ptr->next;
	if (buf->ptr->next == NULL)
	{
		return 1;
	}
	return 0;
}

VOID netbuf_first(struct netbuf *buf)
{
	buf->ptr = buf->p;
}

VOID netbuf_copy_partial(struct netbuf *buf, VOID *dataptr, UINT16 len, UINT16 offset)
{
	struct pbuf *p;
	UINT16 i, left;

	left = 0;

	if (buf == NULL || dataptr == NULL)
	{
		return;
	}

	/* This implementation is bad. It should use bcopy
	   instead. */
	for (p = buf->p; left < len && p != NULL; p = p->next)
	{
		if (offset != 0 && offset >= p->len)
		{
			offset -= p->len;
		}
		else
		{
			for (i = offset; i < p->len; ++i)
			{
				((UINT8 *) dataptr)[left] = ((UINT8 *) p->payload)[i];
				if (++left >= len)
				{
					return;
				}
			}
			offset = 0;
		}
	}
}

VOID netbuf_copy(struct netbuf *buf, VOID *dataptr, UINT16 len)
{
	netbuf_copy_partial(buf, dataptr, len, 0);
}

struct ip_addr *netbuf_fromaddr(struct netbuf *buf)
{
	return buf->fromaddr;
}

UINT16 netbuf_fromport(struct netbuf * buf)
{
	return buf->fromport;
}

struct netconn *netconn_new_with_proto_and_callback(UINT8 t, UINT16 proto, VOID (*callback) (struct netconn *, UINT8, UINT16 len))
{
	struct netconn *conn;
	struct api_msg *msg;

	conn = memp_malloc(MEMP_NETCONN);
	if (conn == NULL)
	{
	      CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp_malloc() == NULL !\n")); //zouyu
		return NULL;
	}

	conn->err = ERR_TCPIP_OK;
	conn->type = t;
	conn->pcb.tcp = NULL;
#if 0
	if ((conn->mbox = sys_mbox_new()) == SYS_MBOX_NULL)
	{
	      CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("sys_mbox_new() == NULL !")); //zouyu
		memp_free(MEMP_NETCONN, conn);
		return NULL;
	}
	conn->recvmbox = SYS_MBOX_NULL;
	conn->acceptmbox = SYS_MBOX_NULL;
	conn->sem = SYS_SEM_NULL;
#endif 	
	conn->state = NETCONN_NONE;
	conn->socket = 0;
	//conn->callback = callback;
	conn->recv_avail = 0;


	if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
	{
		memp_free(MEMP_NETCONN, conn);
		return NULL;
	}

	msg->type = API_MSG_NEWCONN;
	msg->msg.msg.bc.port = proto;	/* misusing the port field */
	msg->msg.conn = conn;
	api_msg_post(msg);
//	sys_mbox_fetch(conn->mbox, NULL);
//	memp_free(MEMP_API_MSG, msg);

	if (conn->err != ERR_TCPIP_OK)
	{
	      CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("conn->err != ERR_TCPIP_OK")); //zouyu
		memp_free(MEMP_NETCONN, conn);
		return NULL;
	}

	return conn;
}

struct netconn *netconn_new(UINT8 t)
{
	return netconn_new_with_proto_and_callback(t, 0, NULL);
}

struct netconn *netconn_new_with_callback(UINT8 t, VOID (*callback) (struct netconn *, UINT8, UINT16 len))
{
	return netconn_new_with_proto_and_callback(t, 0, callback);
}

err_t netconn_delete(struct netconn * conn)
{
	struct api_msg *msg;
//	VOID *mem;

	if (conn == NULL)
	{
		return ERR_TCPIP_OK;
	}

	if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
	{
		return ERR_TCPIP_MEM;
	}
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function netconn_delete\n"));
	msg->type = API_MSG_DELCONN;
	msg->msg.conn = conn;
	api_msg_post(msg);
	//sys_mbox_fetch(conn->mbox, NULL);
	//memp_free(MEMP_API_MSG, msg);

#if 0
	/* Drain the recvmbox. */
	if (conn->recvmbox != SYS_MBOX_NULL)
	{
		while (sys_arch_mbox_fetch(conn->recvmbox, &mem, 1) != SYS_ARCH_TIMEOUT)
		{
			if (conn->type == NETCONN_TCP)
			{
				if (mem != NULL)
					pbuf_free((struct pbuf *) mem);
			}
			else
			{
				netbuf_delete((struct netbuf *) mem);
			}
		}
		sys_mbox_free(conn->recvmbox);
		conn->recvmbox = SYS_MBOX_NULL;
	}

	/* Drain the acceptmbox. */ // should be deleted
	if (conn->acceptmbox != SYS_MBOX_NULL)
	{
		while (sys_arch_mbox_fetch(conn->acceptmbox, &mem, 1) != SYS_ARCH_TIMEOUT)
		{
			netconn_delete((struct netconn *) mem);
		}

		sys_mbox_free(conn->acceptmbox);
		conn->acceptmbox = SYS_MBOX_NULL;
	}

	sys_mbox_free(conn->mbox);
	conn->mbox = SYS_MBOX_NULL;
	if (conn->sem != SYS_SEM_NULL)
	{
		sys_sem_free(conn->sem);
	}
	/*  conn->sem = SYS_SEM_NULL; */
	memp_free(MEMP_NETCONN, conn);   /// need attation
#endif	
	return ERR_TCPIP_OK;
}

UINT8 netconn_type(struct netconn *conn)
{
	return conn->type;
}

err_t netconn_peer(struct netconn * conn, struct ip_addr * addr, UINT16 * port)
{
	switch (conn->type)
	{
	case NETCONN_RAW:
		/* return an error as connecting is only a helper for upper layers */
		return ERR_TCPIP_CONN;
	case NETCONN_UDPLITE:
	case NETCONN_UDPNOCHKSUM:
	case NETCONN_UDP:
		if (conn->pcb.udp == NULL || ((conn->pcb.udp->flags & UDP_FLAGS_CONNECTED) == 0))
			return ERR_TCPIP_CONN;
		*addr = (conn->pcb.udp->remote_ip);
		*port = conn->pcb.udp->remote_port;
		break;
	case NETCONN_TCP:
		if (conn->pcb.tcp == NULL)
			return ERR_TCPIP_CONN;
		*addr = (conn->pcb.tcp->remote_ip);
		*port = conn->pcb.tcp->remote_port;
		break;
	}
	return (conn->err = ERR_TCPIP_OK);
}

err_t netconn_addr(struct netconn * conn, struct ip_addr ** addr, UINT16 * port)
{
	switch (conn->type)
	{
	case NETCONN_RAW:
		*addr = &(conn->pcb.raw->local_ip);
		*port = conn->pcb.raw->protocol;
		break;
	case NETCONN_UDPLITE:
	case NETCONN_UDPNOCHKSUM:
	case NETCONN_UDP:
		*addr = &(conn->pcb.udp->local_ip);
		*port = conn->pcb.udp->local_port;
		break;
	case NETCONN_TCP:
		*addr = &(conn->pcb.tcp->local_ip);
		*port = conn->pcb.tcp->local_port;
		break;
	}
	return (conn->err = ERR_TCPIP_OK);
}

err_t netconn_bind(struct netconn * conn, struct ip_addr * addr, UINT16 port)
{
	struct api_msg *msg;

	if (conn == NULL)
	{
		return ERR_TCPIP_VAL;
	}

#if 0
	if (conn->type != NETCONN_TCP && conn->recvmbox == SYS_MBOX_NULL)
	{
		if ((conn->recvmbox = sys_mbox_new()) == SYS_MBOX_NULL)
		{
			return ERR_TCPIP_MEM;
		}
	}
#endif

	if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
	{
		return (conn->err = ERR_TCPIP_MEM);
	}
	msg->type = API_MSG_BIND;
	msg->msg.conn = conn;
	msg->msg.msg.bc.ipaddr = addr;
	msg->msg.msg.bc.port = port;
	api_msg_post(msg);
//	sys_mbox_fetch(conn->mbox, NULL);
//	memp_free(MEMP_API_MSG, msg);
	return (conn->err = ERR_TCPIP_OK);
}

err_t netconn_connect(struct netconn * conn, struct ip_addr * addr, UINT16 port)
{
	struct api_msg *msg;

	if (conn == NULL)
	{
		return ERR_TCPIP_VAL;
	}
	if(conn->state == NETCONN_NONE)
	{
	    conn->state = NETCONN_CONNECT;
	}
	else
	{
	    return ERR_TCPIP_ISCONN;
	}
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function netconn_connect\n"));

	if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
	{
		return ERR_TCPIP_MEM;
	}
	msg->type = API_MSG_CONNECT;
	msg->msg.conn = conn;
	msg->msg.msg.bc.ipaddr = addr;
	msg->msg.msg.bc.port = port;
       //socketdata_initial(conn->socket);//remove 2009-12-17
	
	api_msg_post(msg);
//	sys_mbox_fetch(conn->mbox, NULL);
//	memp_free(MEMP_API_MSG, msg);//COS_fREE??
	return (conn->err = ERR_TCPIP_OK);
}

err_t netconn_disconnect(struct netconn * conn)
{
	struct api_msg *msg;

	if (conn == NULL)
	{
		return ERR_TCPIP_VAL;
	}

	if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
	{
		return ERR_TCPIP_MEM;
	}
	msg->type = API_MSG_DISCONNECT;
	msg->msg.conn = conn;
	api_msg_post(msg);
//	sys_mbox_fetch(conn->mbox, NULL);
//	memp_free(MEMP_API_MSG, msg);
	return (conn->err = ERR_TCPIP_OK);
}

err_t netconn_listen(struct netconn * conn)
{
	struct api_msg *msg;

	if (conn == NULL)
	{
		return ERR_TCPIP_VAL;
	}
#if 0
	if (conn->acceptmbox == SYS_MBOX_NULL)
	{
		conn->acceptmbox = sys_mbox_new();
		if (conn->acceptmbox == SYS_MBOX_NULL)
		{
			return ERR_TCPIP_MEM;
		}
	}
#endif

	if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
	{
		return (conn->err = ERR_TCPIP_MEM);
	}
	msg->type = API_MSG_LISTEN;
	msg->msg.conn = conn;
	api_msg_post(msg);
//	sys_mbox_fetch(conn->mbox, NULL);
//	memp_free(MEMP_API_MSG, msg);
	return (conn->err = ERR_TCPIP_OK);
}

struct netconn *netconn_accept(struct netconn *conn)
{
#if 1
	struct netconn *newconn;

	if (conn == NULL)
	{
		return NULL;
	}

	//sys_mbox_fetch(conn->acceptmbox, (VOID **) &newconn); //comment by dupb
	newconn = socketdata_get(conn->socket);
	/* Register event with callback */
	//if (conn->callback)
	//	(*conn->callback) (conn, NETCONN_EVT_RCVMINUS, 0);

	return newconn;
#endif
    return NULL;
}
#if 1
struct netbuf *netconn_recv(struct netconn *conn)
{
//	struct api_msg *msg;
	struct netbuf *buf = NULL;
	struct pbuf *p = NULL;
  struct pbuf *tmp_p = NULL;
  struct pbuf *tmp_pr = NULL;
	UINT16 len;

	if (conn == NULL)
	{
		return NULL;
	}

	if ((conn->err != ERR_TCPIP_OK)&&(conn->err != ERR_TCPIP_CLSD))//add by wuys 2007-08-06, (conn->err != ERR_TCPIP_CLSD)
	{
		return NULL;
	}
	//CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function netconn_recv\n"));
	if (conn->type == NETCONN_TCP)
	{
    COS_WaitForSemaphore(Tcp_receive_buf_sem, COS_WAIT_FOREVER);
    
      tmp_p = socketdata_get(conn->socket);
      p = tmp_p;
      tmp_p = socketdata_get(conn->socket);
      while(tmp_p!=NULL)
      {
        if(p!=NULL)
        {
          tmp_pr = p;
          while(tmp_pr->next != NULL)
          {
            tmp_pr->tot_len += tmp_p->tot_len;
            tmp_pr = tmp_pr->next;
          }
          tmp_pr->tot_len += tmp_p->tot_len;
          tmp_pr->next = tmp_p;
        }
        tmp_p = socketdata_get(conn->socket);
        
      }
        
    COS_ReleaseSemaphore(Tcp_receive_buf_sem);
      
		if (conn->pcb.tcp->state == LISTEN)
		{
			conn->err = ERR_TCPIP_CONN;
			return NULL;
		}

		buf = memp_malloc(MEMP_NETBUF);

		if (buf == NULL)
		{
			conn->err = ERR_TCPIP_MEM;
			return NULL;
		}

//		sys_mbox_fetch(conn->recvmbox, (VOID **) &p);
        

		if (p != NULL)
		{
			len = p->tot_len;
			conn->recv_avail -= len;
		}
		else
			len = 0;

		/* Register event with callback */
//		if (conn->callback)
//			(*conn->callback) (conn, NETCONN_EVT_RCVMINUS, len);

		/* If we are closed, we indicate that we no longer wish to receive
		   data by setting conn->recvmbox to SYS_MBOX_NULL. */
		if (p == NULL)
		{
			memp_free(MEMP_NETBUF, buf);
//			sys_mbox_free(conn->recvmbox);
//			conn->recvmbox = SYS_MBOX_NULL;
			return NULL;
		}

		buf->p = p;
		buf->ptr = p;
		buf->fromport = 0;
		buf->fromaddr = NULL;

		/* Let the stack know that we have taken the data. */
#if 0
		if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
		{
			conn->err = ERR_TCPIP_MEM;
			return buf;
		}
		msg->type = API_MSG_RECV;
		msg->msg.conn = conn;
		if (buf != NULL)
		{
			msg->msg.msg.len = buf->p->tot_len;
		}
		else
		{
			msg->msg.msg.len = 1;
		}
		api_msg_post(msg);

		sys_mbox_fetch(conn->mbox, NULL);
		memp_free(MEMP_API_MSG, msg);
#endif
	}
	else
	{
		//sys_mbox_fetch(conn->recvmbox, (VOID **) &buf);  //comment by dupb
	 COS_WaitForSemaphore(Tcp_receive_buf_sem, COS_WAIT_FOREVER);//add by wuys 2008-09-09
		buf = socketdata_get(conn->socket);
        if(buf!=NULL)
		conn->recv_avail -= buf->p->tot_len;
        COS_ReleaseSemaphore(Tcp_receive_buf_sem);
		/* Register event with callback */
    //		if (conn->callback)
    //			(*conn->callback) (conn, NETCONN_EVT_RCVMINUS, buf->p->tot_len);
	
	}

 	//CSW_TRACE(CFW_TCPIP_TS_ID_SXS | TNB_ARG(2), TSTXT("netconn_recv: received %p (err %d)\n"), (VOID *) buf, conn->err);

	return buf;
}
#else
struct netbuf *netconn_recv(struct netconn *conn)
{
//	struct api_msg *msg;
	struct netbuf *buf = NULL;
	struct pbuf *p = NULL;
	UINT16 len;

	if (conn == NULL)
	{
		return NULL;
	}

	if ((conn->err != ERR_TCPIP_OK)&&(conn->err != ERR_TCPIP_CLSD))//add by wuys 2007-08-06, (conn->err != ERR_TCPIP_CLSD)
	{
		return NULL;
	}
	//CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("In function netconn_recv\n"));
	if (conn->type == NETCONN_TCP)
	{
	    p = socketdata_get(conn->socket);
		if (conn->pcb.tcp->state == LISTEN)
		{
			conn->err = ERR_TCPIP_CONN;
			return NULL;
		}

		buf = memp_malloc(MEMP_NETBUF);

		if (buf == NULL)
		{
			conn->err = ERR_TCPIP_MEM;
			return NULL;
		}

//		sys_mbox_fetch(conn->recvmbox, (VOID **) &p);
        

		if (p != NULL)
		{
			len = p->tot_len;
			conn->recv_avail -= len;
		}
		else
			len = 0;

		/* Register event with callback */
//		if (conn->callback)
//			(*conn->callback) (conn, NETCONN_EVT_RCVMINUS, len);

		/* If we are closed, we indicate that we no longer wish to receive
		   data by setting conn->recvmbox to SYS_MBOX_NULL. */
		if (p == NULL)
		{
			memp_free(MEMP_NETBUF, buf);
//			sys_mbox_free(conn->recvmbox);
//			conn->recvmbox = SYS_MBOX_NULL;
			return NULL;
		}

		buf->p = p;
		buf->ptr = p;
		buf->fromport = 0;
		buf->fromaddr = NULL;

		/* Let the stack know that we have taken the data. */
#if 0
		if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
		{
			conn->err = ERR_TCPIP_MEM;
			return buf;
		}
		msg->type = API_MSG_RECV;
		msg->msg.conn = conn;
		if (buf != NULL)
		{
			msg->msg.msg.len = buf->p->tot_len;
		}
		else
		{
			msg->msg.msg.len = 1;
		}
		api_msg_post(msg);

		sys_mbox_fetch(conn->mbox, NULL);
		memp_free(MEMP_API_MSG, msg);
#endif
	}
	else
	{
		//sys_mbox_fetch(conn->recvmbox, (VOID **) &buf);  //comment by dupb
		buf = socketdata_get(conn->socket);
		conn->recv_avail -= buf->p->tot_len;
		/* Register event with callback */
//		if (conn->callback)
//			(*conn->callback) (conn, NETCONN_EVT_RCVMINUS, buf->p->tot_len);
	
	}

 	CSW_TRACE(CFW_TCPIP_TS_ID_SXS | TNB_ARG(2), TSTXT("netconn_recv: received %p (err %d)\n"), (VOID *) buf, conn->err);

	return buf;
}
#endif
err_t netconn_send(struct netconn * conn, struct netbuf * buf)
{
	struct api_msg *msg;

	if (conn == NULL)
	{
		return ERR_TCPIP_VAL;
	}

	if (conn->err != ERR_TCPIP_OK)
	{
		return conn->err;
	}

	if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
	{
		return (conn->err = ERR_TCPIP_MEM);
	}

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("netconn_send: sending %d bytes\n"), buf->p->tot_len);
	msg->type = API_MSG_SEND;
	msg->msg.conn = conn;
	msg->msg.msg.p = buf->p;
	api_msg_post(msg);

//	sys_mbox_fetch(conn->mbox, NULL);
	//memp_free(MEMP_API_MSG, msg);
	return (conn->err = ERR_TCPIP_OK);
}

err_t netconn_write(struct netconn * conn, VOID *dataptr, UINT16 size, UINT8 copy)
{
	struct api_msg *msg;
	UINT16 len;
	CSW_TRACE(CFW_TCPIPT_TS_ID_SXS, TSTXT("In function netconn_write\n"));
	if (conn == NULL)
	{
		return ERR_TCPIP_VAL;
	}
	if (conn->err != ERR_TCPIP_OK)
	{
		return conn->err;
	}

#if 0
if (conn->sem == SYS_SEM_NULL)
	{
		conn->sem = sys_sem_new(0);
		if (conn->sem == SYS_SEM_NULL)
		{
			return ERR_TCPIP_MEM;
		}
	}
#endif
	if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
	{
		return (conn->err = ERR_TCPIP_MEM);
	}
 	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp_malloc api msg %p\n"), msg);   	
	msg->type = API_MSG_WRITE;
	msg->msg.conn = conn;

	conn->state = NETCONN_WRITE;
//	while (conn->err == ERR_TCPIP_OK && size > 0)
	if (conn->err == ERR_TCPIP_OK && size > 0)
	{
		msg->msg.msg.w.dataptr = dataptr;
		msg->msg.msg.w.copy = copy;
#if 0
		if (conn->type == NETCONN_TCP)
		{
			
            if (tcp_sndbuf(conn->pcb.tcp) == 0)
			{
				sys_sem_wait(conn->sem);
				if (conn->err != ERR_TCPIP_OK)
				{
					goto ret;
				}
			}

			if (size > tcp_sndbuf(conn->pcb.tcp))
			{
				/* We cannot send more than one send buffer's worth of data at a
				   time. */
				len = tcp_sndbuf(conn->pcb.tcp);
			}
			else
			{
				len = size;
			}
		}
       
		else
		{
			len = size;
		}
#endif
    len = size;
		//CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("netconn_write: writing 0x%x bytes (0x%x)\n"), len, copy);
		msg->msg.msg.w.len = len;
		api_msg_post(msg);
#if 0
		sys_mbox_fetch(conn->mbox, NULL);
		if (conn->err == ERR_TCPIP_OK)
		{
			dataptr = (VOID *) ((UINT8 *) dataptr + len);
			size -= len;
		}
		else if (conn->err == ERR_TCPIP_MEM)
		{
			conn->err = ERR_TCPIP_OK;
			sys_sem_wait(conn->sem);
		}
		else
		{
			goto ret;
		}
#endif
	}
#if 0
      ret:
	memp_free(MEMP_API_MSG, msg);
	conn->state = NETCONN_NONE;
	if (conn->sem != SYS_SEM_NULL)
	{
		sys_sem_free(conn->sem);
		conn->sem = SYS_SEM_NULL;
	}
#endif
	return (conn->err = ERR_TCPIP_OK);
}

err_t netconn_close(struct netconn * conn)
{
	struct api_msg *msg;

	if (conn == NULL)
	{
		return ERR_TCPIP_VAL;
	}
	if ((msg = memp_malloc(MEMP_API_MSG)) == NULL)
	{
		return (conn->err = ERR_TCPIP_MEM);
	}

	conn->state = NETCONN_CLOSE;
      again:
	msg->type = API_MSG_CLOSE;
	msg->msg.conn = conn;
	api_msg_post(msg);
//	sys_mbox_fetch(conn->mbox, NULL);
//	if (conn->err == ERR_TCPIP_MEM && conn->sem != SYS_SEM_NULL)
//	{
//		sys_sem_wait(conn->sem);
//		goto again;
//	}
	if (conn->err == ERR_TCPIP_MEM)
	{
		goto again;
	}
	conn->state = NETCONN_NONE;
	//memp_free(MEMP_API_MSG, msg);
	return (conn->err = ERR_TCPIP_OK);
}

err_t netconn_err(struct netconn * conn)
{
	return conn->err;
}
#endif

