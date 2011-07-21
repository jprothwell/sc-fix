/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 * soc_win32.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Socket APIs on WIN32 environment.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#if (defined(MMI_ON_HARDWARE_P) && defined(__TCPIP__))


#include "kal_non_specific_general_types.h"

#include "mmi_features.h"
#include "mmi_data_types.h"
#include "cfw.h"
#include <string.h>
#include "sockets.h"
#include "tcpip_inet.h"
#include "soc_api.h"
#include "adp_events.h"
#include "ctype.h" //wufasong added 2006.11.23 for update csw lib to csw v1.01.20
#include "stdlib.h"

HostName_RequestID_struct g_struRequest[20];
UINT32   g_nRequestCount = 0;

extern BOOL app_SocketEventcb (COS_EVENT *pstCosEv);
extern UINT8 g_nSocCid;
UINT8 g_nSimCard =-1;
UINT8 g_nCid = 255;

#ifdef __ADP_MULTI_SIM__

//when call soc_gethostbyname, soc_getsockaddr, soc_getlocaip before, must call soc_setActiveSimAndCID
// cid is active gprs  user cid.
void soc_setActiveSimAndCID(CFW_SIM_ID nSimID,UINT8 nCid )
{
	g_nSimCard = nSimID;
	g_nCid = nCid;
}
#endif


/*****************************************************************************
 * FUNCTION
 *  soc_create
 * DESCRIPTION
 *  This function get a free socket slot from socket pool.
 * PARAMETERS
 *  domain              [IN]        
 *  type                [IN]        
 *  protocol            [IN]        
 *  mod_id              [IN]        
 *  nwk_account_id      [IN]        
 *  a(?)                [IN]        Domain
 *  d(?)                [IN]        Mod_id
 *  e(?)                [IN]        Nwk_account_id
 *  c(?)                [IN]        Protocol
 *  b(?)                [IN]        Type
 * RETURNS
 *  non-negative: sokcet id
 *  negative: fail
 *****************************************************************************/
kal_int8 soc_create(kal_uint8 domain, kal_uint8 type, kal_uint8 protocol, kal_uint16 mod_id, kal_uint32 nwk_account_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	SOCKET	sockId;
	
	 sockId =   CFW_TcpipSocket(domain, type, protocol);
	if (-1 != sockId)
	 {
	 	
		CFW_SetTCPIPCallBackEx (app_SocketEventcb,sockId);
	}
     return sockId;


}   /* end of soc_create */


/*****************************************************************************
 * FUNCTION
 *  soc_close
 * DESCRIPTION
 *  This function free a free socket slot from socket pool and
 *  trigger ABM to close the corresponding bearer if required.
 * PARAMETERS
 *  s           [IN]        
 *  a(?)        [IN]        S, socket id
 * RETURNS
 *  SOC_SUCCESS: success
 *  negative: fail
 *****************************************************************************/
kal_int8 soc_close(kal_int8 s)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return CFW_TcpipSocketClose(s);

}   /* end of soc_close */


/*****************************************************************************
 * FUNCTION
 *  soc_bind
 * DESCRIPTION
 *  This function binds a local address which contain IP and port to a socket.
 * PARAMETERS
 *  s           [IN]        
 *  addr        [?]         
 *  a(?)        [IN]        S, socket id
 *  c(?)        [IN]        Namelen
 *  b(?)        [IN]        *name
 * RETURNS
 *  SOC_SUCCESS: success
 *  negative: fail
 *****************************************************************************/
kal_int8 soc_bind(kal_int8 s, sockaddr_struct *addr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   CFW_TCPIP_SOCKET_ADDR  ipAddr;
   UINT32 ret = ERR_SUCCESS;


    ipAddr.sin_len = 0;	// set 0 as usual
    ipAddr.sin_family = CFW_TCPIP_AF_INET;   
    ipAddr.sin_port = htons(addr->port);			// 8341
    ipAddr.sin_addr.s_addr = htonl(addr->addr[0]<<24 | addr->addr[1]<<16 | addr->addr[2]<<8 | addr->addr[3]);
    

	ret=  CFW_TcpipSocketBind(s, &ipAddr,sizeof(CFW_TCPIP_SOCKET_ADDR));

	if( ERR_SUCCESS !=  ret)
	{
		TS_OutputText(CSW_TS_ID, ("Error CFW_TcpipSocketBind!\n") );
	}		
    return ret;
}   /* end of soc_bind */


/*****************************************************************************
 * FUNCTION
 *  soc_listen
 * DESCRIPTION
 *  This function makes socket to a server for waiting connections.
 * PARAMETERS
 *  s           [IN]        
 *  backlog     [IN]        
 *  a(?)        [IN]        S, socket id
 *  b(?)        [IN]        Backlog, max no. of connections
 * RETURNS
 *  SOC_SUCCESS: success
 *  negative: fail
 *****************************************************************************/
kal_int8 soc_listen(kal_int8 s, kal_uint8 backlog)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return CFW_TcpipSocketListen(s, backlog);

}   /* end of soc_listen */


/*****************************************************************************
 * FUNCTION
 *  soc_accept
 * DESCRIPTION
 *  This function waits the incoming connections and return a socket id
 *  of new connection.
 * PARAMETERS
 *  s           [IN]            
 *  addr        [?]             
 *  a(?)        [IN]            S, socket id
 *  b(?)        [IN/OUT]        Addr, address of foreign host
 * RETURNS
 *  non-negative: socket id of new connection
 *  negative: fail
 *****************************************************************************/
kal_int8 soc_accept(kal_int8 s, sockaddr_struct *addr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     CFW_TCPIP_SOCKET_ADDR serIP;
      UINT32  addrlen;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	serIP.sin_len = 0;	// set 0 as usual
	serIP.sin_family = CFW_TCPIP_AF_INET;   
	serIP.sin_port = htons(addr->port);			// 8341
	serIP.sin_addr.s_addr = htonl(addr->addr[0]<<24 | addr->addr[1]<<16 | addr->addr[2]<<8 | addr->addr[3]);
	addrlen = sizeof(CFW_TCPIP_SOCKET_ADDR);
    return CFW_TcpipSocketAccept(s, &serIP, &addrlen);

}   /* end of soc_accept */


/*****************************************************************************
 * FUNCTION
 *  soc_connect
 * DESCRIPTION
 *  This function connects to the server and
 *  trigger ABM to establish the corresponding bearer if the bearer is
 *  not established.
 * PARAMETERS
 *  s           [IN]            
 *  addr        [?]             
 *  a(?)        [IN]            S, socket id
 *  b(?)        [IN/OUT]        Addr, pointer of server address
 * RETURNS
 *  non-negative: socket id of new connection
 *  negative: fail
 *****************************************************************************/
kal_int8 soc_connect(kal_int8 s, sockaddr_struct *addr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	CFW_TCPIP_SOCKET_ADDR serIP;
   UINT32 ret = ERR_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	serIP.sin_len = 0;	// set 0 as usual
	serIP.sin_family = CFW_TCPIP_AF_INET;   
	serIP.sin_port = htons(addr->port);			// 8341
	serIP.sin_addr.s_addr = htonl(addr->addr[0]<<24 | addr->addr[1]<<16 | addr->addr[2]<<8 | addr->addr[3]);

	ret =  CFW_TcpipSocketConnect(s, (CFW_TCPIP_SOCKET_ADDR *)&serIP, sizeof(CFW_TCPIP_SOCKET_ADDR));
	if( ERR_SUCCESS !=  ret)
	{
    		TS_OutputText(CSW_TS_ID, ("Error CFW_TcpipSocketConnect\n") );
	}
	return ret;

}   /* end of soc_connect */


/*****************************************************************************
 * FUNCTION
 *  soc_shutdown
 * DESCRIPTION
 *  This function shutdowns the connection in aspects of reading, writing
 *  or both.
 * PARAMETERS
 *  s           [IN]        
 *  how         [IN]        
 *  a(?)        [IN]        S, socket id
 *  b(?)        [IN]        How, specify shutdown read/write/both
 * RETURNS
 *  SOC_SUCCESS: success
 *  negative: fail
 *****************************************************************************/
kal_int8 soc_shutdown(kal_int8 s, kal_uint8 how)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return CFW_TcpipSocketShutdown(s, how);   /* not support */

}   /* end of soc_shutdown */


/*****************************************************************************
 * FUNCTION
 *  soc_sendto
 * DESCRIPTION
 *  
 * PARAMETERS
 *  s           [IN]        
 *  buf         [?]         
 *  len         [IN]        
 *  flags       [IN]        
 *  addr        [?]         
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 soc_sendto(kal_int8 s, kal_uint8 *buf, kal_int32 len, kal_uint8 flags, sockaddr_struct *addr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
		CFW_TCPIP_SOCKET_ADDR serIP;
     UINT16 FreeBufSize;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	serIP.sin_len = 0;	// set 0 as usual
	serIP.sin_family = CFW_TCPIP_AF_INET;   
	serIP.sin_port = htons(addr->port);			// 8341
	serIP.sin_addr.s_addr = htonl(addr->addr[0]<<24 | addr->addr[1]<<16 | addr->addr[2]<<8 | addr->addr[3]);
       FreeBufSize = CFW_TcpipAvailableBuffer(s);
	   	TS_OutputText(CSW_TS_ID, ("soc_send::FreeBufSize=%d  len = %d"), FreeBufSize, len );

       if(FreeBufSize>len)
              return CFW_TcpipSocketSendto(s, buf, len, flags, &serIP, sizeof(CFW_TCPIP_SOCKET_ADDR));
      else 
       	return SOC_LIMIT_RESOURCE;
}


/*****************************************************************************
 * FUNCTION
 *  soc_send
 * DESCRIPTION
 *  This function sends data to a host which already connected previously.
 * PARAMETERS
 *  s           [IN]        
 *  buf         [?]         
 *  len         [IN]        
 *  flags       [IN]        
 *  a(?)        [IN]        S, socket id
 *  d(?)        [IN]        Flags
 *  c(?)        [IN]        Len, data length
 *  b(?)        [IN]        *buf, hold the data
 * RETURNS
 *  non-negative: no. of bytes sent
 *  negative: fail
 *****************************************************************************/
kal_int32 soc_send(kal_int8 s, kal_uint8 *buf, kal_int32 len, kal_uint8 flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	     UINT16 FreeBufSize;

       FreeBufSize = CFW_TcpipAvailableBuffer(s);
	TS_OutputText(CSW_TS_ID, ("soc_send::FreeBufSize=%d  len = %d"), FreeBufSize, len );

       if(FreeBufSize>len)
	    return CFW_TcpipSocketSend(s, buf, len, 0);
      else
	    return SOC_LIMIT_RESOURCE;
}


/*****************************************************************************
 * FUNCTION
 *  soc_recvfrom
 * DESCRIPTION
 *  
 * PARAMETERS
 *  s               [IN]        
 *  buf             [?]         
 *  len             [IN]        
 *  flags           [IN]        
 *  fromaddr        [?]         
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 soc_recvfrom(kal_int8 s, kal_uint8 *buf, kal_int32 len, kal_uint8 flags, sockaddr_struct *fromaddr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	    /*----------------------------------------------------------------*/
		CFW_TCPIP_SOCKET_ADDR serIP;
    UINT32 	addrlen ;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	serIP.sin_len = 0;	// set 0 as usual
	serIP.sin_family = CFW_TCPIP_AF_INET;   
	serIP.sin_port = htons(fromaddr->port);			// 8341
	serIP.sin_addr.s_addr = htonl(fromaddr->addr[0]<<24 | fromaddr->addr[1]<<16 | fromaddr->addr[2]<<8 | fromaddr->addr[3]);
	addrlen = sizeof(CFW_TCPIP_SOCKET_ADDR);

    return CFW_TcpipSocketRecvfrom(s, buf, len, flags, &serIP, &addrlen);
}


/*****************************************************************************
 * FUNCTION
 *  soc_recv
 * DESCRIPTION
 *  This function receives data from a host connected previously.
 * PARAMETERS
 *  s           [IN]            
 *  buf         [?]             
 *  len         [IN]            
 *  flags       [IN]            
 *  a(?)        [IN]            S, socket id
 *  d(?)        [IN]            Flags
 *  c(?)        [IN]            Buffer length for receiving
 *  b(?)        [IN/OUT]        *buf, hold the data
 * RETURNS
 *  non-negative: no. of bytes received
 *  negative: fail
 *****************************************************************************/
kal_int32 soc_recv(kal_int8 s, kal_uint8 *buf, kal_int32 len, kal_uint8 flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return CFW_TcpipSocketRecv(s, buf, len, flags);
}   /* end of soc_recv */

#ifndef SOC_NOT_SUPPORT_SELECT


/*****************************************************************************
 * FUNCTION
 *  soc_select
 * DESCRIPTION
 *  This function receives data from a host connected previously.
 * PARAMETERS
 *  ndesc       [IN]            
 *  in          [?]             
 *  out         [?]             
 *  ex          [?]             
 *  tv          [?]             
 *  a(?)        [IN]            Ndesc, no. of descriptors
 *  d(?)        [IN/OUT]        *ex, exception fd_set
 *  e(?)        [IN]            *tv, specify the timeout value
 *  c(?)        [IN/OUT]        *out, output fd_set
 *  b(?)        [IN/OUT]        *in, input fd_set
 * RETURNS
 *  non-negative: no. of socket be set
 *  negative: fail
 *****************************************************************************/
kal_int8 soc_select(kal_uint8 ndesc, fd_set *in, fd_set *out, fd_set *ex, soc_timeval_struct *tv)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	    return SOC_ERROR;

}                       /* end of soc_select */

#endif /* SOC_NOT_SUPPORT_SELECT */ 


/*****************************************************************************
 * FUNCTION
 *  soc_setsockopt
 * DESCRIPTION
 *  This function sets the socket options.
 * PARAMETERS
 *  s               [IN]        
 *  option          [IN]        
 *  val             [?]         
 *  val_size        [IN]        
 *  a(?)            [IN]        S, socket id
 *  d(?)            [IN]        Val_size
 *  c(?)            [IN]        *val
 *  b(?)            [IN]        Option
 * RETURNS
 *  SOC_SUCCESS: success
 *  negative: fail
 *****************************************************************************/
kal_int8 soc_setsockopt(kal_int8 s, kal_uint16 option, kal_uint8 *val, kal_uint8 val_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	#if 1
	return 0; //chenhe ,for CFW_TcpipSocketSetsockopt is not exact
	#else
    return CFW_TcpipSocketSetsockopt(s, SOL_SOCKET, option, val, val_size);
	#endif
}   /* end of soc_setsockopt */


/*****************************************************************************
 * FUNCTION
 *  soc_getsockopt
 * DESCRIPTION
 *  This function gets the socket options.
 * PARAMETERS
 *  s               [IN]        
 *  option          [IN]        
 *  val             [?]         
 *  val_size        [IN]        
 *  a(?)            [IN]        S, socket id
 *  d(?)            [IN]        Val_size
 *  c(?)            [IN]        *val
 *  b(?)            [IN]        Option
 * RETURNS
 *  SOC_SUCCESS: success
 *  negative: fail
 *****************************************************************************/
kal_int8 soc_getsockopt(kal_int8 s, kal_uint16 option, kal_uint8 *val, kal_uint8 val_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	INT32 val_len = val_size;
	
    return CFW_TcpipSocketGetsockopt(s, SOL_SOCKET,option, val, &val_len);

}   /* end of soc_getsockopt */


/*****************************************************************************
 * FUNCTION
 *  soc_gethostbyname
 * DESCRIPTION
 *  This function gets the IP of the given domain name.
 * PARAMETERS
 *  is_blocking         [IN]            
 *  mod_id              [IN]            
 *  request_id          [IN]            
 *  domain_name         [IN]            
 *  addr                [?]             
 *  addr_len            [?]             
 *  access_id           [IN]            
 *  nwk_account_id      [IN]            
 *  a(?)                [IN]            Is_blocking, indicate whether block or non-block mode are used, currently only non-block mode support
 *  d(?)                [IN]            Domain_name
 *  h(?)                [IN]            Nwk_account_id
 *  g(?)                [IN]            Access_id, embedded in response message
 *  f(?)                [IN/OUT]        Addr_len
 *  e(?)                [IN/OUT]        Addr
 *  c(?)                [IN]            Request_id, embedded in response message
 *  b(?)                [IN]            Mod_id
 * RETURNS
 *  SOC_SUCCESS: success, return ip address in 'addr'
 *  negative: fail excpet for
 *  SOC_WOULDBLOCK is for waiting DNS reply from network
 *****************************************************************************/
kal_int8 soc_gethostbyname(
            kal_bool is_blocking,
            kal_uint16 mod_id,
            kal_int32 request_id,
            const kal_char *domain_name,
            kal_uint8 *addr,
            kal_uint8 *addr_len,
            kal_uint8 access_id,
            kal_uint32 nwk_account_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	UINT32 rest;
	struct ip_addr  naddr;

	if(g_nRequestCount >= 20) 
		return SOC_LIMIT_RESOURCE;
	
	#ifdef __ADP_MULTI_SIM__
	rest = CFW_Gethostbyname((char *)domain_name, &naddr,g_nCid,g_nSimCard);
	#else
	rest = CFW_Gethostbyname((char *)domain_name, &naddr,g_nSocCid);
	#endif
	if(RESOLV_COMPLETE == rest)
	{
		TS_OutputText(CSW_TS_ID, TSTXT(" DNS IP :0x%x\n"), naddr.addr);
		#if 0
	      addr[0] = (naddr.addr)&0xff000000 >>24;
	      addr[1] = (naddr.addr)&0x00ff0000 >>16;
	      addr[2] = (naddr.addr)&0x0000ff00 >>8;	
	      addr[3] = (naddr.addr)&0x000000ff;	
		  #endif
		addr[0] = ntohl(naddr.addr) >>24&0xff;	
		addr[1] = ntohl(naddr.addr) >>16&0xff;	
		addr[2] = ntohl(naddr.addr) >>8&0xff;		
		addr[3] = ntohl(naddr.addr)&0xff;	
	      *addr_len = 4;
		  return SOC_SUCCESS;
	}	  
       else if(RESOLV_QUERY_QUEUED == rest)
       {
		g_struRequest[g_nRequestCount].request_id = request_id;
		g_struRequest[g_nRequestCount].access_id  = access_id;
		g_struRequest[g_nRequestCount].nsimID = g_nSimCard;
		g_struRequest[g_nRequestCount].ncid       =  g_nCid;
		memcpy(g_struRequest[g_nRequestCount].domain_name,domain_name, strlen(domain_name));
		g_nRequestCount++;

		 CFW_SetTCPIPCallBackEx(app_SocketEventcb,4);

		TS_OutputText(CSW_TS_ID, TSTXT(" DNS test ,waiting.......later send event:EV_CFW_DNS_RESOLV_SUC_IND\n"));
		return SOC_WOULDBLOCK;
       }
 
       return rest;

}   /* end of soc_gethostbyname */


/*****************************************************************************
 * FUNCTION
 *  soc_gethostbyaddr
 * DESCRIPTION
 *  This function gets the domain name of the given IP.
 * PARAMETERS
 *  is_blocking         [IN]            
 *  mod_id              [IN]            
 *  request_id          [IN]            
 *  domain_name         [?]             
 *  addr                [IN]            
 *  addr_len            [IN]            
 *  access_id           [IN]            
 *  nwk_account_id      [IN]            
 *  a(?)                [IN]            Is_blocking, indicate whether block or non-block mode are used, currently only non-block mode support
 *  d(?)                [IN/OUT]        Domain_name
 *  h(?)                [IN]            Nwk_account_id
 *  g(?)                [IN]            Access_id, embedded in response message
 *  f(?)                [IN]            Addr_len, must be 4.
 *  e(?)                [IN]            Addr
 *  c(?)                [IN]            Request_id, embedded in response message
 *  b(?)                [IN]            Mod_id
 * RETURNS
 *  SOC_SUCCESS: success, return domain name in 'domain_name' which
 *  terminated by null character
 *  negative: fail excpet for
 *  SOC_WOULDBLOCK is for waiting DNS reply from network
 *****************************************************************************/
kal_int8 soc_gethostbyaddr(
            kal_bool is_blocking,
            kal_uint16 mod_id,
            kal_int32 request_id,
            kal_char *domain_name,
            const kal_uint8 *addr,
            kal_uint8 addr_len,
            kal_uint8 access_id,
            kal_uint32 nwk_account_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return SOC_ERROR;

}   /* end of soc_gethostbyaddr */


/*****************************************************************************
 * FUNCTION
 *  soc_close_nwk_account
 * DESCRIPTION
 *  This function closes the network account opened by that module.
 *  If the corresponding account is closed, socket will send a
 *  MSG_ID_APP_SOC_DEACTIVATE_CNF to application.
 * PARAMETERS
 *  mod_id      [IN]        
 *  a(?)        [IN]        Mod_id
 * RETURNS
 *  void
 *****************************************************************************/
void soc_close_nwk_account(kal_uint16 mod_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return; /* NOT SUPPORT */

}   /* end of soc_close_nwk_account */


/*****************************************************************************
 * FUNCTION
 *  soc_getlocalip
 * DESCRIPTION
 *  This function returns the local IP address.
 * PARAMETERS
 *  local_ip        [?]             
 *  a(?)            [IN/OUT]        *local_ip, copy the IP address, used as return
 * RETURNS
 *  SOC_SUCCESS: success
 *  negative: bearer is not activated, no IP address info
 *****************************************************************************/
kal_int8 soc_getlocalip(kal_uint8 *local_ip)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UINT32 ret = ERR_SUCCESS;

		UINT8 leng;
#ifdef __ADP_MULTI_SIM__
		ret=  CFW_GprsGetPdpAddr(1, &leng, local_ip, g_nSimCard);
#else
		ret=  CFW_GprsGetPdpAddr(1, &leng, local_ip);
#endif
		if( ERR_SUCCESS != ret )
		{
			TS_OutputText(CSW_TS_ID, ("Error CFW_GprsGetPdpAddr!\n") );
		}
	
    return ret;   /* not support */

}   /* end of soc_getlocalip */


/*****************************************************************************
 * FUNCTION
 *  soc_getsockaddr
 * DESCRIPTION
 *  This function returns the remote or local address assoicated
 *  with given socket.
 * PARAMETERS
 *  s               [IN]            
 *  is_local        [IN]            
 *  addr            [?]             
 *  a(?)            [IN]            S, socket id
 *  c(?)            [IN/OUT]        *addr, copy the address, used as return
 *  b(?)            [IN]            Is_local, true: local
 * RETURNS
 *  SOC_SUCCESS: success
 *  negative: fail
 *****************************************************************************/
kal_int8 soc_getsockaddr(kal_int8 s, kal_bool is_local, sockaddr_struct *addr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	    UINT32 ret = ERR_SUCCESS;

		UINT8 leng;

	if(is_local)
	{
		#ifdef __ADP_MULTI_SIM__
		ret=  CFW_GprsGetPdpAddr(1, &leng, addr->addr, g_nSimCard);
		#else
		ret=  CFW_GprsGetPdpAddr(1, &leng, addr->addr);
		#endif
		if( ERR_SUCCESS != ret )
		{
			TS_OutputText(CSW_TS_ID, ("Error CFW_GprsGetPdpAddr!\n") );
		}
		addr->addr_len = leng;
		addr->port = 0;
             return ret;
	}
	else
	{
	    return SOC_ERROR;	  
	}


}   /* end of soc_getsockaddr */


/*****************************************************************************
 * FUNCTION
 *  soc_get_last_error
 * DESCRIPTION
 *  This function is used to get the last network error.
 *  A successful function call, or a call to soc_get_last_error,
 *  does not reset the error cause.
 * PARAMETERS
 *  s                   [IN]            
 *  error               [?]             Cause
 *  detail_cause        [?]             
 *  a(?)                [IN]            S, socket id
 *  c(?)                [IN/OUT]        *detail_cause, minor error cause
 *  b(?)                [IN/OUT]        *error, major error cause
 * RETURNS
 *  SOC_SUCCESS: success
 *  SOC_INVALID_SOCKET: invalid socket id
 *****************************************************************************/
kal_int8 soc_get_last_error(kal_int8 s, kal_int8 *error, kal_int32 *detail_cause)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return SOC_ERROR;   /* NOT SUPPORT */

}   /* end of soc_get_last_error */


/*****************************************************************************
 * FUNCTION
 *  soc_set_last_error
 * DESCRIPTION
 *  This function is used to set the last network error.
 * PARAMETERS
 *  s                   [IN]        
 *  error               [IN]        
 *  detail_cause        [IN]        
 *  a(?)                [IN]        S, socket id
 *  c(?)                [IN]        Detail_cause, minor error cause
 *  b(?)                [IN]        Error, major error cause
 * RETURNS
 *  SOC_SUCCESS: success
 *  SOC_INVALID_SOCKET: invalid socket id
 *****************************************************************************/
kal_int8 soc_set_last_error(kal_int8 s, kal_int8 error, kal_int32 detail_cause)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return SOC_ERROR;   /* NOT SUPPORT */

}   /* end of soc_set_last_error */

kal_bool soc_ip_check(kal_char *asci_addr,
                      kal_uint8 *ip_addr,
                      kal_bool *ip_validity)
{
    kal_uint8 len;
    kal_uint8 i,octet_cnt;
    kal_int32 ip_digit;

    len = strlen(asci_addr);

    for (i = 0 ; i < len ; i++)
    {
        if (!isdigit((int)*(asci_addr+i)) && *(asci_addr+i) != '.' )
            return KAL_FALSE;
    }

    *ip_validity = KAL_TRUE;

    /* Extract the IP adress from character array */
    for (octet_cnt = 0 ; octet_cnt < 4 ; octet_cnt++)
    {
        if (*asci_addr == '\0') /* in case of "1.2.3." */
        {
            *ip_validity = KAL_FALSE;
            return KAL_TRUE;
        }

        ip_digit = atoi(asci_addr);

        if (ip_digit < 0 || ip_digit > 255)
        {
            *ip_validity = KAL_FALSE;
            return KAL_TRUE;
        }

        ip_addr[octet_cnt] = (kal_uint8)ip_digit;

        if (octet_cnt == 3)
            continue;

        asci_addr = strstr(asci_addr,".");
        if (asci_addr)
            asci_addr++;
        else
            break;
    }

    if (octet_cnt != 4)
    {
        return KAL_FALSE;
    }

    if (ip_addr[0] == 0 && ip_addr[1] == 0 && ip_addr[2] == 0 && ip_addr[3] == 0)
    {
        *ip_validity = KAL_FALSE;
    }

    return KAL_TRUE;
}
#endif /* defined(WIN32) && defined(WIN_SOC) && defined(__TCPIP__) */ 

