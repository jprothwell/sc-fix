/***************************************************************************
 * $Id: jdd_netdatatype.h,v 1.2 2006/05/02 11:19:21 kumardevjtbase Exp $
 * $Revision: 1.2 $
 * $DateTime: $
 * 
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdd_netdatatype.h,v $
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.5  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2006/01/21 15:54:46  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

#ifndef _JC_NET_
#define _JC_NET_

/** 
 * @file jdd_netdatatype.h
 * @ingroup Network
 * @brief Describes the basic data type that are getting used in the
 * network abstraction API's
 */

/**
 * @brief The enumeration lists the supported socket types that will be used in jdd_NetSocket function
 */
typedef enum e_socket_type
{
	E_SOCK_STREAM, /**< Provides sequenced reliable two way data connection based byte streams */
	E_SOCK_DGRAM /**< Provides connectionless unreliable transmission. */
} ESocketType ;

/**
 * @brief The enumeration lists the supported socket levels that will be used in jdd_NetSetSockOpt function
 */
typedef enum e_socket_level
{
	E_SOL_SOCKET, /**< Provides options at the socket level */
	E_IPPROTO_TCP, /**< Provides options at the TCP level. */
	E_IPPROTO_IP /**< Provides options at the IP level. */
} ESocketLevel ;

/**
 * @brief This enumeration specifies a communication domain. 
 * The enumeration lists the supported protocol, which shall be used in jdd_NetSocket function
 */
typedef enum e_socket_domain
{
	E_PF_INET_v4, /**< Specifies IPv4 internet protocols */
	E_PF_INET_v6  /**< Specifies IPv6 internet protocols */
} ESocketDomain ;

/**
 * The enumeration lists the supported option names, which will be used in jdd_NetSetSockOpt function
 */
typedef enum e_option_name
{
	E_SO_DONTLINGER, /**< Does not block jdd_NetClose waiting for unsent data to be sent */
	E_SO_LINGER, /**< Lingers on close if unsent data is present. */
	E_SO_RCVBUF, /**< Specifies the total per socket buffer reserved for receives */
	E_SO_REUSE_ADDR, /**< Allows socket to be bound on a particular address that is already in use. */
	E_SO_SENDBUF, /**< Specifies the total per socket buffer reserved for send. */
	E_SO_TCPNODELAY, /**< Disables the Nagle algorithm for send coalescing. */
	E_SO_SENDTIMEOUT, /**< Specifies the total per socket buffer reserved for send. */
	E_SO_NONBLOCKING, /**< Specifies the socket is of non-blocking type. */
	E_SO_RECV_TIMEOUT, /**< Specifies the timeout for receiving the data in milliseconds */
	E_SO_BROADCAST, /**< Allows transmission of broadcast messages on the socket. */
	E_SO_IP_ADD_MEMBERSHIP, /**< The membership request structure used to request a particular multicast group's membership. */
	E_SO_IP_DROP_MEMBERSHIP, /**< The group whose membership is to be used. */
	E_SO_IP_MULTICAST_TTL, /**< Time-to-live for a multicast packet. */
	E_SO_IP_HDRINCL, /**< Currently not defined */
	E_SO_TCP_NODELAY /**< Disables the Nagle algorithm for send coalescing. */
} EOptionName ;

/**
 * @brief The enumeration contains the flag that is used while reading from the socket.
 * @note Currently this field is not used.
 */
typedef enum e_recv_flags
{
	E_RECV0_NOT_USED, /**< Currently not defined */
	E_RECV1_NOT_USED  /**< Currently not defined */
} ERecvFlags ;

/**
 * @brief The enumeration contains the flag that is sent while writing to the socket.
 * @note Currently this field is not used.
 */
typedef enum e_send_flags
{
	E_SEND0_NOT_USED, /**< Currently not defined */
	E_SEND1_NOT_USED  /**< Currently not defined */
} ESendFlags ;

/**
 * @brief The enumeration contains the flag that is sent while creating the socket.
 * @note Currently this field is not used.
 */
typedef enum e_socket_protocol
{
	E_PROTOCOL0_NOT_USED, /**< Currently not defined */
	E_PROTOCOL1_NOT_USED  /**< Currently not defined */
} ESocketProtocol ;

/**
 * @brief This enumeration contains the socket descriptor types
 */
typedef enum e_socket_types
{
	E_WRITE_SOCKET = 1 , /**< Specifies the socket descriptor is for checking the writability */
	E_READ_SOCKET = 2 /**< Specifies the socket descriptor is for checking the readability */
} ESocketDesType ;

/**
 * @brief This structure specifies the network address.
 */ 
typedef struct st_net_addr
{
	JC_UINT32 iIP ; /**< Specifies the IP Address */
	JC_UINT16 hPortNo ; /**< Specifies the port number */
} JC_NET_ADDR ;

#endif

