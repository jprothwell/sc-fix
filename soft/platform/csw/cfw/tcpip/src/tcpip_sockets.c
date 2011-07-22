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
 * Improved by Marc Boucher <marc@mbsi.ca> and David Haas <dhaas@alum.rpi.edu>
 *
 */
#ifdef CFW_TCPIP_SUPPORT
#include "tcpip_api.h"
#include <csw.h> 
#include "tcpip_arch.h"

#include "sockets.h"
#include "tcpip_api_msg.h"

#define NUM_SOCKETS MEMP_NUM_NETCONN

struct socket_dsc
{
	struct netconn *conn;
	struct netbuf *lastdata;
	UINT16 lastoffset;
	UINT16 rcvevent;
	UINT16 sendevent;
	UINT16 flags;
	INT32 err;
};

struct lwip_select_cb
{
	struct lwip_select_cb *next;
	fd_set *readset;
	fd_set *writeset;
	fd_set *exceptset;
	INT32 sem_signalled;
	sys_sem_t sem;
	UINT8 padding[3];
};

static struct socket_dsc sockets[NUM_SOCKETS];
static struct lwip_select_cb *select_cb_list = 0;
UINT16 nUTI;

static sys_sem_t socksem = 0;
static sys_sem_t selectsem = 0;
static UINT32 errno;
#define ERRNO

static VOID event_callback(struct netconn *conn, UINT8 evt, UINT16 nLen);

static INT32 err_to_errno_table[11] = {
	0,			/* ERR_OK    0      No error, everything OK. */
	ERR_TCPIP_ENOMEM,			/* ERR_MEM  -1      Out of memory error.     */
	ERR_TCPIP_ENOBUFS,		/* ERR_BUF  -2      Buffer error.            */
	ERR_TCPIP_ECONNABORTED,		/* ERR_ABRT -3      Connection aborted.      */
	ERR_TCPIP_ECONNRESET,		/* ERR_RST  -4      Connection reset.        */
	ERR_TCPIP_ESHUTDOWN,		/* ERR_CLSD -5      Connection closed.       */
	ERR_TCPIP_ENOTCONN,		/* ERR_CONN -6      Not connected.           */
	ERR_TCPIP_EINVAL,			/* ERR_VAL  -7      Illegal value.           */
	ERR_TCPIP_EIO,			/* ERR_ARG  -8      Illegal argument.        */
	ERR_TCPIP_EHOSTUNREACH,		/* ERR_RTE  -9      Routing problem.         */
	ERR_TCPIP_EADDRINUSE		/* ERR_USE  -10     Address in use.          */
};

#define ERR_TO_ERRNO_TABLE_SIZE \
  (sizeof(err_to_errno_table)/sizeof(err_to_errno_table[0]))

#define err_to_errno(err) \
    (-(err) >= 0 && -(err) < (INT32)ERR_TO_ERRNO_TABLE_SIZE ?             \
    err_to_errno_table[-(err)] : (INT32)ERR_TCPIP_EIO)

#ifdef ERRNO
#define set_errno(err) errno = (err)
#else
#define set_errno(err)
#endif

#define sock_set_errno(sk, e) do { \
      sk->err = (e); \
      set_errno(sk->err); \
} while (0)

static struct socket_dsc *get_socket(UINT8 nSocket)
{
	struct socket_dsc *sock;

	//if (nSocket > NUM_SOCKETS)
  if (nSocket >= NUM_SOCKETS) // Modify by Wuys for bug 13078.13079.13082
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("get_socket(%d): invalid\n"),nSocket);
		set_errno(ERR_TCPIP_EBADF);
		return NULL;
	}

	sock = &sockets[nSocket];

	if ((!sock->conn)||(!sock->conn->pcb.tcp))
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("get_socket(%d): not active\n"),nSocket);
		set_errno(ERR_TCPIP_EBADF);
		return NULL;
	}

	return sock;
}
static struct socket_dsc *close_get_socket(UINT8 nSocket)
{
	struct socket_dsc *sock;

	//if (nSocket > NUM_SOCKETS)
  if (nSocket >= NUM_SOCKETS) // Modify by Wuys for bug 13078.13079.13082
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS,TSTXT("get_socket(%d): invalid\n"),nSocket);
		set_errno(ERR_TCPIP_EBADF);
		return NULL;
	}

	sock = &sockets[nSocket];

	if (!sock->conn)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("get_socket(%d): not active\n"),nSocket);
		set_errno(ERR_TCPIP_EBADF);
		return NULL;
	}

	return sock;
}
static UINT8 alloc_socket(struct netconn *newconn)
{
	UINT8 i;

//	if (!socksem)
//		socksem = sys_sem_new(1);

	/* Protect socket array */
//	sys_sem_wait(socksem);

	/* allocate a new socket identifier */
	for (i = 0; i < NUM_SOCKETS; ++i)
	{
		if (!sockets[i].conn)
		{
			sockets[i].conn = newconn;
			sockets[i].lastdata = NULL;
			sockets[i].lastoffset = 0;
			sockets[i].rcvevent = 0;
			sockets[i].sendevent = 1;	/* TCP send buf is empty */
			sockets[i].flags = 0;
			sockets[i].err = 0;
//			sys_sem_signal(socksem);
			return i;
		}
	}
//	sys_sem_signal(socksem);
	return -1;
}


UINT16 CFW_TcpipAvailableBuffer(SOCKET nSocket)
{
    struct socket_dsc *sock;
    //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipAvailableBuffer(%d)...\n"), nSocket);
    sock = get_socket(nSocket);
    if (!sock)
    {
        set_errno(ERR_TCPIP_EBADF);
        return -1;
    }
	
    if(sock->conn->pcb.tcp==NULL)
		return 0;
    return sock->conn->pcb.tcp->snd_buf;
}

UINT16 CFW_TcpipGetRecvAvailable(SOCKET nSocket)
{
    struct socket_dsc *sock;
    //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipGetRecvAvailable(%d)...\n"), nSocket);
    sock = get_socket(nSocket);
    if (!sock)
    {
        set_errno(ERR_TCPIP_EBADF);
        return -1;
    }
    return sock->conn->recv_avail;
}

UINT32 CFW_TcpipGetLastError(VOID)
{
    return errno; 

}


UINT32 CFW_TcpipSocketAccept(SOCKET nSocket,  CFW_TCPIP_SOCKET_ADDR *addr, UINT32 * addrlen)
{
	struct socket_dsc *sock;
	struct netconn *newconn;
	struct ip_addr naddr;
	UINT16 port;
	INT32 newsock;
	 CFW_TCPIP_SOCKET_ADDR sin;

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketAccept(%d)...\n"), nSocket);
	sock = get_socket(nSocket);
	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}

	newconn = netconn_accept(sock->conn);

	/* get the IP address and port of the remote host */
	netconn_peer(newconn, &naddr, &port);

	SUL_MemSet8(&sin, 0, sizeof(sin));
	sin.sin_len = sizeof(sin);
	sin.sin_family = CFW_TCPIP_AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = naddr.addr;

	if (*addrlen > sizeof(sin))
		*addrlen = sizeof(sin);

	SUL_MemCopy8(addr, &sin, *addrlen);

	newsock = alloc_socket(newconn);
	if (newsock == -1)
	{
		netconn_delete(newconn);
		sock_set_errno(sock,  ERR_TCPIP_ENOBUFS);
		return -1;
	}
//	newconn->callback = event_callback;
	sock = get_socket(newsock);

	sys_sem_wait(socksem);
	sock->rcvevent += -1 - newconn->socket;
	newconn->socket = newsock;
	sys_sem_signal(socksem);

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketAccept(%d) returning new sock=%d addr="), nSocket, newsock);
	ip_addr_debug_print(CFW_TCPIP_TS_ID_SXS, &naddr);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" port=%u\n"), port);

	sock_set_errno(sock, 0);
	return newsock;
}

UINT32 CFW_TcpipSocketBind(SOCKET nSocket,  CFW_TCPIP_SOCKET_ADDR *pName, UINT8 nNameLen)
{
	struct socket_dsc *sock;
	struct ip_addr local_addr;
	UINT16 local_port;
	err_t err;

	sock = get_socket(nSocket);
	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}

	local_addr.addr = (( CFW_TCPIP_SOCKET_ADDR *) pName)->sin_addr.s_addr;
	local_port = (( CFW_TCPIP_SOCKET_ADDR *) pName)->sin_port;

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketBind(%d, addr="), nSocket);
	ip_addr_debug_print(CFW_TCPIP_TS_ID_SXS, &local_addr);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" port=%u)\n"), ntohs(local_port));

	err = netconn_bind(sock->conn, &local_addr, ntohs(local_port));

	if (err != ERR_TCPIP_OK)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketBind(%d) failed, err=%d\n"), nSocket, err);
		sock_set_errno(sock, err_to_errno(err));
		return -1;
	}

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketBind(%d) succeeded\n"), nSocket);
	sock_set_errno(sock, 0);
	return 0;
}

UINT32 CFW_TcpipSocketClose(SOCKET nSocket)
{
	struct socket_dsc *sock;

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketClose(%d)\n"), nSocket);
	if (!socksem)
		socksem = sys_sem_new(1);

	/* We cannot allow multiple closes of the same socket. */
	sys_sem_wait(socksem);

	sock = close_get_socket(nSocket);
	if (!sock)
	{
		sys_sem_signal(socksem);
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}

	netconn_delete(sock->conn);
	if (sock->lastdata)
	{
		netbuf_delete(sock->lastdata);
	}
	sock->lastdata = NULL;
	sock->lastoffset = 0;
	sock->conn = NULL;
	sys_sem_signal(socksem);

         
	sock_set_errno(sock, 0);
	return 0;
}

UINT32 CFW_TcpipSocketConnect(SOCKET nSocket,  CFW_TCPIP_SOCKET_ADDR *pName, UINT8 nNameLen)
{
	struct socket_dsc *sock;
	err_t err;

	sock = get_socket(nSocket);
	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}
       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("conn->state = %x\n"), sock->conn->state);

       if(sock->conn->state == NETCONN_CONNECT)
       {
		set_errno(ERR_TCPIP_EISCONN);
		return -1;
       }

	if (((CFW_TCPIP_SOCKET_ADDR *) pName)->sin_family == CFW_TCPIP_AF_UNSPEC)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketConnect(%d, CFW_TCPIP_AF_UNSPEC)\n"), nSocket);
		//err = netconn_disconnect(sock->conn);
		set_errno(ERR_TCPIP_EPERM);
		return -1;
	}
	else if(((CFW_TCPIP_SOCKET_ADDR *) pName)->sin_family == CFW_TCPIP_AF_INET)
	{
		struct ip_addr remote_addr;
		UINT16 remote_port;

		remote_addr.addr = ((CFW_TCPIP_SOCKET_ADDR *) pName)->sin_addr.s_addr;
		remote_port = ((CFW_TCPIP_SOCKET_ADDR *) pName)->sin_port;

		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketConnect(%d, addr="), nSocket);
		ip_addr_debug_print(CFW_TCPIP_TS_ID_SXS, &remote_addr);
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" port=%u)\n"), ntohs(remote_port));

		err = netconn_connect(sock->conn, &remote_addr, ntohs(remote_port));
	}
	else
	{
		set_errno(ERR_TCPIP_EINVAL);
		return -1;	
	}

	if (err != ERR_TCPIP_OK)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketConnect(%d) failed, err=%d\n"), nSocket, err);
		sock_set_errno(sock, err_to_errno(err));
		return -1;
	}

	sock_set_errno(sock, 0);
	return 0;
}

UINT32 CFW_TcpipSocketListen(SOCKET nSocket, UINT32 backlog)
{
	struct socket_dsc *sock;
	err_t err;

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketListen(%d, backlog=%d)\n"), nSocket, backlog);
	sock = get_socket(nSocket);
	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}

	err = netconn_listen(sock->conn);

	if (err != ERR_TCPIP_OK)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_TcpipSocketListen(%d) failed, err=%d\n"), nSocket, err);
		sock_set_errno(sock, err_to_errno(err));
		return -1;
	}

	sock_set_errno(sock, 0);
	return 0;
}

UINT32 CFW_TcpipSocketRecvfrom(SOCKET nSocket, VOID *pMem, INT32 nLen, UINT32 nFlags, CFW_TCPIP_SOCKET_ADDR *from, INT32 * fromlen)
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

		if (*fromlen > (INT32)sizeof(sin))
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
	       api_msg_post(msg);
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

        do_recv_test(msg->msg);

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
  //add end
UINT32 CFW_TcpipSocketRead(SOCKET nSocket, VOID *pMem, INT32 nLen)
{
	return CFW_TcpipSocketRecvfrom(nSocket, pMem, nLen, 0, NULL, NULL);
}

UINT32 CFW_TcpipSocketRecv(SOCKET nSocket, UINT8* pData, UINT16 nDataSize, UINT32 nFlags)
{
	return CFW_TcpipSocketRecvfrom(nSocket, pData, nDataSize, nFlags, NULL, NULL);
}

UINT32 CFW_TcpipSocketSend(SOCKET nSocket, UINT8 *pData, UINT16 nDataSize, UINT32 nFlags)
{
	struct socket_dsc *sock;
	struct netbuf *buf;
	err_t err;

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSend(%d),  nSize=%d, flags=0x%x)\n"), nSocket, nDataSize, nFlags);

	sock = get_socket(nSocket);
	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}

	switch (netconn_type(sock->conn))
	{
	case NETCONN_RAW:
	case NETCONN_UDP:
	case NETCONN_UDPLITE:
	case NETCONN_UDPNOCHKSUM:
		/* create a buffer */
		buf = netbuf_new();

		if (!buf)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSend(%d) ENOBUFS\n"), nSocket);
			sock_set_errno(sock,  ERR_TCPIP_ENOBUFS);
			return -1;
		}
 		/* make the buffer point to the data that should
		   be sent */
		netbuf_ref(buf, pData, nDataSize);

		/* send the data */
		err = netconn_send(sock->conn, buf);

		/* deallocated the buffer */
		netbuf_delete(buf);
		break;
	case NETCONN_TCP:
		    if(nDataSize > sock->conn->pcb.tcp->snd_buf)
		    {
		       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSend  tcp size > buf \n"));       
		       set_errno(ERR_TCPIP_ENOBUFS);
		       return -1;
		    }		
		err = netconn_write(sock->conn, pData, nDataSize, NETCONN_COPY);
		break;
	default:
		err = ERR_TCPIP_ARG;
		break;
	}
	if (err != ERR_TCPIP_OK)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSend(%d) err=%d\n"), nSocket, err);
		sock_set_errno(sock, err_to_errno(err));
		return -1;
	}

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSend(%d) ok nSize=%d\n"), nSocket, nDataSize);
	sock_set_errno(sock, 0);
	return nDataSize;
}

UINT32 CFW_TcpipSocketSendto(SOCKET nSocket, VOID *pData, UINT16 nDataSize, UINT32 nFlags, CFW_TCPIP_SOCKET_ADDR *to, INT32 tolen)
{
	struct socket_dsc *sock;
	struct ip_addr remote_addr, addr;
	UINT16 remote_port, port;
	INT32 ret, connected;

	sock = get_socket(nSocket);
	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}

	/* get the peer if currently connected */
	connected = (netconn_peer(sock->conn, &addr, &port) == ERR_TCPIP_OK);

	remote_addr.addr = ((CFW_TCPIP_SOCKET_ADDR *) to)->sin_addr.s_addr;
	remote_port = ((CFW_TCPIP_SOCKET_ADDR *) to)->sin_port;

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSendto(%d, pData=%p, nSize=%d, flags=0x%x to="), nSocket, pData, nDataSize, nFlags);
	ip_addr_debug_print(CFW_TCPIP_TS_ID, &remote_addr);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT (" port=%u\n"), ntohs(remote_port));

	sock->conn->state = NETCONN_NONE;//add by wuys 2007-11-16 
	netconn_connect(sock->conn, &remote_addr, ntohs(remote_port));

	ret = CFW_TcpipSocketSend(nSocket, pData, nDataSize, nFlags);

	/* reset the remote address and port number
	   of the connection */
	sock->conn->state = NETCONN_NONE;//add by wuys 2007-11-16 
	if (connected)
		netconn_connect(sock->conn, &addr, port);
	else
		netconn_disconnect(sock->conn);
	return ret;
}

SOCKET CFW_TcpipSocket(UINT8 nDomain, UINT8 nType, UINT8 nProtocol)
{
	struct netconn *conn;
	INT32 i;

    switch(nDomain)
    {
      case CFW_TCPIP_AF_UNSPEC:
			  break;
		  case CFW_TCPIP_AF_INET:
			  break;
	    default:
		    set_errno(ERR_TCPIP_EINVAL);
		    return INVALID_SOCKET;
    }
    switch(nProtocol)
    {
      case CFW_TCPIP_IPPROTO_IP:
  			break;
  		case CFW_TCPIP_IPPROTO_TCP:
  			break;
  		case CFW_TCPIP_IPPROTO_UDP:
  			break;			
	    default:
		    set_errno(ERR_TCPIP_EINVAL);
		    return INVALID_SOCKET;
    }	
	/* create a netconn */
	switch (nType)
	{
  	case CFW_TCPIP_SOCK_RAW:
  		conn = netconn_new_with_proto_and_callback(NETCONN_RAW, nProtocol, event_callback);
  		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocket(%d, CFW_TCPIP_SOCK_RAW, %d) = "), nDomain, nProtocol);
  		break;
  	case CFW_TCPIP_SOCK_DGRAM:
  		conn = netconn_new_with_callback(NETCONN_UDP, event_callback);
  		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocket(%d, CFW_TCPIP_SOCK_DGRAM, %d) = "), nDomain , nProtocol);
  		break;
  	case CFW_TCPIP_SOCK_STREAM:
  		conn = netconn_new_with_callback(NETCONN_TCP, event_callback);
  		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocket(%d, CFW_TCPIP_SOCK_STREAM, %d) = "), nDomain, nProtocol);
  		break;
  	default:
  		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocket(%d, %d/UNKNOWN, %d) = -1\n"), nDomain, nType, nProtocol);
  		set_errno(ERR_TCPIP_EINVAL);
  		return INVALID_SOCKET;
	}

	if (!conn)
	{
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("-1 / ENOBUFS (could not create netconn)\n"));
		set_errno(ERR_TCPIP_ENOBUFS);
		return INVALID_SOCKET;
	}

	i = alloc_socket(conn);

	if (i == -1)
	{
		netconn_delete(conn);
		set_errno(ERR_TCPIP_ENOBUFS);
		return INVALID_SOCKET;
	}
	conn->socket = i;
	//add 2009-12-17
	socketdata_initial(conn->socket, conn->type);
	//add end
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("socket id = %d\n"), i);
	set_errno(0);
	return i;
}

UINT32 CFW_TcpipSocketWrite(SOCKET nSocket, VOID *pData, UINT16 nDataSize)
{
	return CFW_TcpipSocketSend(nSocket, pData, nDataSize, 0);
}

static UINT32 CFW_TcpipSocketSelscan(INT32 maxfdp1, fd_set * readset, fd_set * writeset, fd_set * exceptset)
{
	INT32 i, nready = 0;
	fd_set lreadset, lwriteset, lexceptset;
	struct socket_dsc *p_sock;

	FD_ZERO(&lreadset);
	FD_ZERO(&lwriteset);
	FD_ZERO(&lexceptset);

	/* Go through each socket in each list to count number of sockets which
	   currently match */
	for (i = 0; i < maxfdp1; i++)
	{
		if (FD_ISSET(i, readset))
		{
			/* See if netconn of this socket is ready for read */
			p_sock = get_socket(i);
			if (p_sock && (p_sock->lastdata || p_sock->rcvevent))
			{
				FD_SET(i, &lreadset);
				CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSelscan: fd=%d ready for reading\n"), i);
				nready++;
			}
		}
		if (FD_ISSET(i, writeset))
		{
			/* See if netconn of this socket is ready for write */
			p_sock = get_socket(i);
			if (p_sock && p_sock->sendevent)
			{
				FD_SET(i, &lwriteset);
				CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSelscan: fd=%d ready for writing\n"), i);
				nready++;
			}
		}
	}
	*readset = lreadset;
	*writeset = lwriteset;
	FD_ZERO(exceptset);

	return nready;
}

UINT32 CFW_TcpipSocketSelect(INT32 maxfdp1, fd_set * readset, fd_set * writeset, fd_set * exceptset, struct timeval *timeout)
{
	INT32 i;
	INT32 nready;
	fd_set lreadset, lwriteset, lexceptset;
	UINT32 msectimeout;
	struct lwip_select_cb select_cb;
	struct lwip_select_cb *p_selcb;

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSelect(%d, %p, %p, %p, tvsec=%ld tvusec=%ld)\n"), 
		maxfdp1, (VOID *) readset, 
		(VOID *) writeset, 
		(VOID *) exceptset, 
		timeout ? timeout->tv_sec : -1L, 
		timeout ? timeout->tv_usec : -1L);

	select_cb.next = 0;
	select_cb.readset = readset;
	select_cb.writeset = writeset;
	select_cb.exceptset = exceptset;
	select_cb.sem_signalled = 0;

	/* Protect ourselves searching through the list */
	if (!selectsem)
		selectsem = sys_sem_new(1);
	sys_sem_wait(selectsem);

	if (readset)
		lreadset = *readset;
	else
		FD_ZERO(&lreadset);
	if (writeset)
		lwriteset = *writeset;
	else
		FD_ZERO(&lwriteset);
	if (exceptset)
		lexceptset = *exceptset;
	else
		FD_ZERO(&lexceptset);

	/* Go through each socket in each list to count number of sockets which
	   currently match */
	nready = CFW_TcpipSocketSelscan(maxfdp1, &lreadset, &lwriteset, &lexceptset);

	/* If we don't have any current events, then suspend if we are supposed to */
	if (!nready)
	{
		if (timeout && timeout->tv_sec == 0 && timeout->tv_usec == 0)
		{
			sys_sem_signal(selectsem);
			if (readset)
				FD_ZERO(readset);
			if (writeset)
				FD_ZERO(writeset);
			if (exceptset)
				FD_ZERO(exceptset);

			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSelect: no timeout, returning 0\n"));
			set_errno(0);

			return 0;
		}

		/* add our semaphore to list */
		/* We don't actually need any dynamic memory. Our entry on the
		 * list is only valid while we are in this function, so it's ok
		 * to use local variables */

		select_cb.sem = sys_sem_new(0);
		/* Note that we are still protected */
		/* Put this select_cb on top of list */
		select_cb.next = select_cb_list;
		select_cb_list = &select_cb;

		/* Now we can safely unprotect */
		sys_sem_signal(selectsem);

		/* Now just wait to be woken */
		if (timeout == 0)
			/* Wait forever */
			msectimeout = 0;
		else
			msectimeout = ((timeout->tv_sec * 1000) + ((timeout->tv_usec + 500) / 1000));

		i = sys_sem_wait_timeout(select_cb.sem, msectimeout);

		/* Take us off the list */
		sys_sem_wait(selectsem);
		if (select_cb_list == &select_cb)
			select_cb_list = select_cb.next;
		else
			for (p_selcb = select_cb_list; p_selcb; p_selcb = p_selcb->next)
				if (p_selcb->next == &select_cb)
				{
					p_selcb->next = select_cb.next;
					break;
				}

		sys_sem_signal(selectsem);

		sys_sem_free(select_cb.sem);
		if (i == 0)	/* Timeout */
		{
			if (readset)
				FD_ZERO(readset);
			if (writeset)
				FD_ZERO(writeset);
			if (exceptset)
				FD_ZERO(exceptset);

			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSelect: timeout expired\n"));
			set_errno(0);

			return 0;
		}

		if (readset)
			lreadset = *readset;
		else
			FD_ZERO(&lreadset);
		if (writeset)
			lwriteset = *writeset;
		else
			FD_ZERO(&lwriteset);
		if (exceptset)
			lexceptset = *exceptset;
		else
			FD_ZERO(&lexceptset);

		/* See what's set */
		nready = CFW_TcpipSocketSelscan(maxfdp1, &lreadset, &lwriteset, &lexceptset);
	}
	else
		sys_sem_signal(selectsem);

	if (readset)
		*readset = lreadset;
	if (writeset)
		*writeset = lwriteset;
	if (exceptset)
		*exceptset = lexceptset;

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSelect: nready=%d\n"), nready);
	set_errno(0);

	return nready;
}

static VOID event_callback(struct netconn *conn, UINT8 evt, UINT16 nLen)
{
	UINT8 nSocket;
	struct socket_dsc *sock;
	struct lwip_select_cb *scb;

	/* Get socket */
	if (conn)
	{
		nSocket = conn->socket;
		//if (nSocket < 0)
		//{
			/* Data comes in right away after an accept, even though
			 * the server task might not have created a new socket yet.
			 * Just count down (or up) if that's the case and we
			 * will use the data later. Note that only receive events
			 * can happen before the new socket is set up. */
			//if (evt == NETCONN_EVT_RCVPLUS)
			//	conn->socket--;
			//return;
		//}

		sock = get_socket(nSocket);
		if (!sock)
			return;
	}
	else
		return;

	if (!selectsem)
		selectsem = sys_sem_new(1);

	sys_sem_wait(selectsem);
	/* Set event as required */
	switch (evt)
	{
	case NETCONN_EVT_RCVPLUS:
		sock->rcvevent++;
		break;
	case NETCONN_EVT_RCVMINUS:
		sock->rcvevent--;
		break;
	case NETCONN_EVT_SENDPLUS:
		sock->sendevent = 1;
		break;
	case NETCONN_EVT_SENDMINUS:
		sock->sendevent = 0;
		break;
	}
	sys_sem_signal(selectsem);

	/* Now decide if anyone is waiting for this socket */
	/* NOTE: This code is written this way to protect the select link list
	   but to avoid a deadlock situation by releasing socksem before
	   signalling for the select. This means we need to go through the list
	   multiple times ONLY IF a select was actually waiting. We go through
	   the list the number of waiting select calls + 1. This list is
	   expected to be small. */
	while (1)
	{
		sys_sem_wait(selectsem);
		for (scb = select_cb_list; scb; scb = scb->next)
		{
			if (scb->sem_signalled == 0)
			{
				/* Test this select call for our socket */
				if (scb->readset && FD_ISSET(nSocket, scb->readset))
					if (sock->rcvevent)
						break;
				if (scb->writeset && FD_ISSET(nSocket, scb->writeset))
					if (sock->sendevent)
						break;
			}
		}
		if (scb)
		{
			scb->sem_signalled = 1;
			sys_sem_signal(selectsem);
			sys_sem_signal(scb->sem);
		}
		else
		{
			sys_sem_signal(selectsem);
			break;
		}
	}

}

UINT32 CFW_TcpipSocketShutdown(SOCKET nSocket, INT32 how)
{
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketShutdown(%d, how=%d)\n"), nSocket, how);
	return CFW_TcpipSocketClose(nSocket);	/* XXX temporary hack until proper implementation */
}

UINT32 CFW_TcpipSocketGetpeername(SOCKET nSocket, CFW_TCPIP_SOCKET_ADDR *pName, INT32 * pNameLen)
{
	struct socket_dsc *sock;
	CFW_TCPIP_SOCKET_ADDR sin;
	struct ip_addr naddr;

	sock = get_socket(nSocket);
	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}

	SUL_MemSet8(&sin, 0, sizeof(sin));
	sin.sin_len = sizeof(sin);
	sin.sin_family = CFW_TCPIP_AF_INET;

	/* get the IP address and port of the remote host */
	netconn_peer(sock->conn, &naddr, &sin.sin_port);

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetpeername(%d, addr="), nSocket);
	ip_addr_debug_print(CFW_TCPIP_TS_ID, &naddr);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT (" port=%d)\n"), sin.sin_port);

	sin.sin_port = htons(sin.sin_port);
	sin.sin_addr.s_addr = naddr.addr;

	if (*pNameLen > (INT32)sizeof(sin))
		*pNameLen = sizeof(sin);

	SUL_MemCopy8(pName, &sin, *pNameLen);
	sock_set_errno(sock, 0);
	return 0;
}

UINT32 CFW_TcpipSocketGetsockname(SOCKET nSocket, CFW_TCPIP_SOCKET_ADDR *pName, INT32 * pNameLen)
{
	struct socket_dsc *sock;
	CFW_TCPIP_SOCKET_ADDR sin;
	struct ip_addr *naddr;

	sock = get_socket(nSocket);
	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}

	SUL_MemSet8(&sin, 0, sizeof(sin));
	sin.sin_len = sizeof(sin);
	sin.sin_family = CFW_TCPIP_AF_INET;

	/* get the IP address and port of the remote host */
	netconn_addr(sock->conn, &naddr, &sin.sin_port);

	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockname(%d, addr="), nSocket);
	ip_addr_debug_print(CFW_TCPIP_TS_ID_SXS, naddr);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT (" port=%d)\n"), sin.sin_port);

	sin.sin_port = htons(sin.sin_port);
	sin.sin_addr.s_addr = naddr->addr;

	if (*pNameLen > (INT32)sizeof(sin))
		*pNameLen = sizeof(sin);

	SUL_MemCopy8(pName, &sin, *pNameLen);
	sock_set_errno(sock, 0);
	return 0;
}

UINT32 CFW_TcpipSocketGetsockopt(SOCKET nSocket, INT32 level, INT32 optname, VOID *optval, INT32 * optlen)
{
	INT32 err = 0;
	struct socket_dsc *sock = get_socket(nSocket);

	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}

	if (NULL == optval || NULL == optlen)
	{
		sock_set_errno(sock,  ERR_TCPIP_EFAULT);
		return -1;
	}

	/* Do length and type checks for the various options first, to keep it readable. */
	switch (level)
	{

/* Level: SOL_SOCKET */
	case SOL_SOCKET:
		switch (optname)
		{

		case SO_ACCEPTCONN:
		case SO_BROADCAST:
			/* UNIMPL case SO_DEBUG: */
			/* UNIMPL case SO_DONTROUTE: */
		case SO_ERROR:
		case SO_KEEPALIVE:
			/* UNIMPL case SO_OOBINLINE: */
			/* UNIMPL case SO_RCVBUF: */
			/* UNIMPL case SO_SNDBUF: */
			/* UNIMPL case SO_RCVLOWAT: */
			/* UNIMPL case SO_SNDLOWAT: */
#if SO_REUSE
		case SO_REUSEADDR:
		case SO_REUSEPORT:
#endif /* SO_REUSE */
		case SO_TYPE:
			/* UNIMPL case SO_USELOOPBACK: */
			if (*optlen < (INT32)sizeof(INT32))
			{
				err = ERR_TCPIP_EINVAL;
			}
			break;

		default:
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, SOL_SOCKET, UNIMPL: optname=0x%x, ..)\n"), nSocket, optname);
			err = ERR_TCPIP_ENOPROTOOPT;
		}		/* switch */
		break;

/* Level: CFW_TCPIP_IPPROTO_IP */
	case CFW_TCPIP_IPPROTO_IP:
		switch (optname)
		{
			/* UNIMPL case IP_HDRINCL: */
			/* UNIMPL case IP_RCVDSTADDR: */
			/* UNIMPL case IP_RCVIF: */
		case IP_TTL:
		case IP_TOS:
			if (*optlen < (INT32)sizeof(INT32))
			{
				err = ERR_TCPIP_EINVAL;
			}
			break;

		default:
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, CFW_TCPIP_IPPROTO_IP, UNIMPL: optname=0x%x, ..)\n"), nSocket, optname);
			err = ERR_TCPIP_ENOPROTOOPT;
		}		/* switch */
		break;

/* Level: CFW_TCPIP_IPPROTO_TCP */
	case CFW_TCPIP_IPPROTO_TCP:
		if (*optlen < (INT32)sizeof(INT32))
		{
			err = ERR_TCPIP_EINVAL;
			break;
		}

		/* If this is no TCP socket, ignore any options. */
		if (sock->conn->type != NETCONN_TCP)
			return 0;

		switch (optname)
		{
		case TCP_NODELAY:
		case TCP_KEEPALIVE:
			break;

		default:
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, CFW_TCPIP_IPPROTO_TCP, UNIMPL: optname=0x%x, ..)\n"), nSocket, optname);
			err = ERR_TCPIP_ENOPROTOOPT;
		}		/* switch */
		break;

/* UNDEFINED LEVEL */
	default:
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, level=0x%x, UNIMPL: optname=0x%x, ..)\n"), nSocket, level, optname);
		err = ERR_TCPIP_ENOPROTOOPT;
	}			/* switch */

	if (0 != err)
	{
		sock_set_errno(sock, err);
		return -1;
	}

	/* Now do the actual option processing */

	switch (level)
	{

/* Level: SOL_SOCKET */
	case SOL_SOCKET:
		switch (optname)
		{

			/* The option flags */
		case SO_ACCEPTCONN:
		case SO_BROADCAST:
			/* UNIMPL case SO_DEBUG: */
			/* UNIMPL case SO_DONTROUTE: */
		case SO_KEEPALIVE:
			/* UNIMPL case SO_OOBINCLUDE: */
#if SO_REUSE
		case SO_REUSEADDR:
		case SO_REUSEPORT:
#endif /* SO_REUSE */
			/*case SO_USELOOPBACK: UNIMPL */
			*(INT32 *) optval = sock->conn->pcb.tcp->so_options & optname;
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, SOL_SOCKET, optname=0x%x, ..) = %x\n"),
				nSocket, optname, (*(INT32 *) optval ));
			break;

		case SO_TYPE:
			switch (sock->conn->type)
			{
			case NETCONN_RAW:
				*(INT32 *) optval = CFW_TCPIP_SOCK_RAW;
				break;
			case NETCONN_TCP:
				*(INT32 *) optval = CFW_TCPIP_SOCK_STREAM;
				break;
			case NETCONN_UDP:
			case NETCONN_UDPLITE:
			case NETCONN_UDPNOCHKSUM:
				*(INT32 *) optval = CFW_TCPIP_SOCK_DGRAM;
				break;
			default:	/* unrecognized socket type */
				*(INT32 *) optval = sock->conn->type;
				CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, SOL_SOCKET, SO_TYPE): unrecognized socket type %d\n"), nSocket, *(INT32 *) optval);
			}	/* switch */
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, SOL_SOCKET, SO_TYPE) = %d\n"), nSocket, *(INT32 *) optval);
			break;

		case SO_ERROR:
			*(INT32 *) optval = sock->err;
			sock->err = 0;
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, SOL_SOCKET, SO_ERROR) = %d\n"), nSocket, *(INT32 *) optval);
			break;
		}		/* switch */
		break;

/* Level: CFW_TCPIP_IPPROTO_IP */
	case CFW_TCPIP_IPPROTO_IP:
		switch (optname)
		{
		case IP_TTL:
			*(INT32 *) optval = sock->conn->pcb.tcp->ttl;
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, CFW_TCPIP_IPPROTO_IP, IP_TTL) = %d\n"), nSocket, *(INT32 *) optval);
			break;
		case IP_TOS:
			*(INT32 *) optval = sock->conn->pcb.tcp->tos;
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, CFW_TCPIP_IPPROTO_IP, IP_TOS) = %d\n"), nSocket, *(INT32 *) optval);
			break;
		}		/* switch */
		break;

/* Level: CFW_TCPIP_IPPROTO_TCP */
	case CFW_TCPIP_IPPROTO_TCP:
		switch (optname)
		{
		case TCP_NODELAY:
			*(INT32 *) optval = (sock->conn->pcb.tcp->flags & TF_NODELAY);
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, CFW_TCPIP_IPPROTO_TCP, TCP_NODELAY) = %d\n"), nSocket, (*(INT32 *) optval) );
			break;
		case TCP_KEEPALIVE:
			*(INT32 *) optval = (INT32) sock->conn->pcb.tcp->keepalive;
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketGetsockopt(%d, CFW_TCPIP_IPPROTO_IP, TCP_KEEPALIVE) = %d\n"), nSocket, *(INT32 *) optval);
			break;
		}		/* switch */
		break;
	}

	sock_set_errno(sock, err);
	return err ? -1 : 0;
}

UINT32 CFW_TcpipSocketSetsockopt(SOCKET nSocket, INT32 level, INT32 optname, const VOID *optval, INT32 optlen)
{
	struct socket_dsc *sock = get_socket(nSocket);
	INT32 err = 0;

	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}

	if (NULL == optval)
	{
		sock_set_errno(sock,  ERR_TCPIP_EFAULT);
		return -1;
	}

	/* Do length and type checks for the various options first, to keep it readable. */
	switch (level)
	{

/* Level: SOL_SOCKET */
	case SOL_SOCKET:
		switch (optname)
		{

		case SO_BROADCAST:
			/* UNIMPL case SO_DEBUG: */
			/* UNIMPL case SO_DONTROUTE: */
		case SO_KEEPALIVE:
			/* UNIMPL case SO_OOBINLINE: */
			/* UNIMPL case SO_RCVBUF: */
			/* UNIMPL case SO_SNDBUF: */
			/* UNIMPL case SO_RCVLOWAT: */
			/* UNIMPL case SO_SNDLOWAT: */
#if SO_REUSE
		case SO_REUSEADDR:
		case SO_REUSEPORT:
#endif /* SO_REUSE */
			/* UNIMPL case SO_USELOOPBACK: */
			if (optlen < (INT32)sizeof(INT32))
			{
				err = ERR_TCPIP_EINVAL;
			}
			break;
		default:
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSetsockopt(%d, SOL_SOCKET, UNIMPL: optname=0x%x, ..)\n"), nSocket, optname);
			err = ERR_TCPIP_ENOPROTOOPT;
		}		/* switch */
		break;

/* Level: CFW_TCPIP_IPPROTO_IP */
	case CFW_TCPIP_IPPROTO_IP:
		switch (optname)
		{
			/* UNIMPL case IP_HDRINCL: */
			/* UNIMPL case IP_RCVDSTADDR: */
			/* UNIMPL case IP_RCVIF: */
		case IP_TTL:
		case IP_TOS:
			if (optlen < (INT32)sizeof(INT32))
			{
				err = ERR_TCPIP_EINVAL;
			}
			break;
		default:
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSetsockopt(%d, CFW_TCPIP_IPPROTO_IP, UNIMPL: optname=0x%x, ..)\n"), nSocket, optname);
			err = ERR_TCPIP_ENOPROTOOPT;
		}		/* switch */
		break;

/* Level: CFW_TCPIP_IPPROTO_TCP */
	case CFW_TCPIP_IPPROTO_TCP:
		if (optlen < (INT32)sizeof(INT32))
		{
			err = ERR_TCPIP_EINVAL;
			break;
		}

		/* If this is no TCP socket, ignore any options. */
		if (sock->conn->type != NETCONN_TCP)
			return 0;

		switch (optname)
		{
		case TCP_NODELAY:
		case TCP_KEEPALIVE:
			break;

		default:
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSetsockopt(%d, CFW_TCPIP_IPPROTO_TCP, UNIMPL: optname=0x%x, ..)\n"), nSocket, optname);
			err = ERR_TCPIP_ENOPROTOOPT;
		}		/* switch */
		break;

/* UNDEFINED LEVEL */
	default:
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSetsockopt(%d, level=0x%x, UNIMPL: optname=0x%x, ..)\n"), nSocket, level, optname);
		err = ERR_TCPIP_ENOPROTOOPT;
	}			/* switch */

	if (0 != err)
	{
		sock_set_errno(sock, err);
		return -1;
	}

	/* Now do the actual option processing */

	switch (level)
	{

/* Level: SOL_SOCKET */
	case SOL_SOCKET:
		switch (optname)
		{

			/* The option flags */
		case SO_BROADCAST:
			/* UNIMPL case SO_DEBUG: */
			/* UNIMPL case SO_DONTROUTE: */
		case SO_KEEPALIVE:
			/* UNIMPL case SO_OOBINCLUDE: */
#if SO_REUSE
		case SO_REUSEADDR:
		case SO_REUSEPORT:
#endif /* SO_REUSE */
			/* UNIMPL case SO_USELOOPBACK: */
			if (*(INT32 *) optval)
			{
				sock->conn->pcb.tcp->so_options |= optname;
			}
			else
			{
				sock->conn->pcb.tcp->so_options &= ~optname;
			}
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSetsockopt(%d, SOL_SOCKET, optname=0x%x, ..) -> %x\n"), 
				nSocket, optname, (*(INT32 *) optval ));
			break;
		}		/* switch */
		break;

/* Level: CFW_TCPIP_IPPROTO_IP */
	case CFW_TCPIP_IPPROTO_IP:
		switch (optname)
		{
		case IP_TTL:
			sock->conn->pcb.tcp->ttl = (UINT8) (*(INT32 *) optval);
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSetsockopt(%d, CFW_TCPIP_IPPROTO_IP, IP_TTL, ..) -> %u\n"), nSocket, sock->conn->pcb.tcp->ttl);
			break;
		case IP_TOS:
			sock->conn->pcb.tcp->tos = (UINT8) (*(INT32 *) optval);
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSetsockopt(%d, CFW_TCPIP_IPPROTO_IP, IP_TOS, ..)-> %u\n"), nSocket, sock->conn->pcb.tcp->tos);
			break;
		}		/* switch */
		break;

/* Level: CFW_TCPIP_IPPROTO_TCP */
	case CFW_TCPIP_IPPROTO_TCP:
		switch (optname)
		{
		case TCP_NODELAY:
			if (*(INT32 *) optval)
			{
				sock->conn->pcb.tcp->flags |= TF_NODELAY;
			}
			else
			{
				sock->conn->pcb.tcp->flags &= ~TF_NODELAY;
			}
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSetsockopt(%d, CFW_TCPIP_IPPROTO_TCP, TCP_NODELAY) -> %x\n"), nSocket, (*(INT32 *) optval) );
			break;
		case TCP_KEEPALIVE:
			sock->conn->pcb.tcp->keepalive = (UINT32) (*(INT32 *) optval);
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketSetsockopt(%d, CFW_TCPIP_IPPROTO_TCP, TCP_KEEPALIVE) -> %lu\n"), nSocket, sock->conn->pcb.tcp->keepalive);
			break;
		}		/* switch */
		break;
	}			/* switch */

	sock_set_errno(sock, err);
	return err ? -1 : 0;
}

UINT32 CFW_TcpipSocketIoctl(SOCKET nSocket, INT32 cmd, VOID *argp)
{
	struct socket_dsc *sock = get_socket(nSocket);

	if (!sock)
	{
		set_errno(ERR_TCPIP_EBADF);
		return -1;
	}

	switch (cmd)
	{
	case FIONREAD:
		if (!argp)
		{
			sock_set_errno(sock,  ERR_TCPIP_EINVAL);
			return -1;
		}

		*((UINT16 *) argp) = sock->conn->recv_avail;

		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketIoctl(%d, FIONREAD, %p) = %u\n"), nSocket, argp, *((UINT16 *) argp));
		sock_set_errno(sock, 0);
		return 0;

	case FIONBIO:
		if (argp && *(UINT32 *) argp)
			sock->flags |= O_NONBLOCK;
		else
			sock->flags &= ~O_NONBLOCK;
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketIoctl(%d, FIONBIO, %d)\n"), nSocket, !!(sock->flags & O_NONBLOCK));
		sock_set_errno(sock, 0);
		return 0;

	default:
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT ("CFW_TcpipSocketIoctl(%d, UNIMPL: 0x%lx, %p)\n"), nSocket, cmd, argp);
		sock_set_errno(sock,  ERR_TCPIP_ENOSYS);	/* not yet implemented */
		return -1;
	}
}



void free_sock_sem()
{
#if 0
#ifdef REMAIN_SEM
    if(socksem)
    {
        sys_sem_free(socksem);
        socksem = SYS_SEM_NULL;
    }
#endif
    if(selectsem)
    {
        sys_sem_free(selectsem);
        selectsem = SYS_SEM_NULL;
    }
#endif
}

#endif



