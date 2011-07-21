/***************************************************************************
 * $Id: jdd_netapi.h,v 1.3 2006/07/03 07:26:11 browserdevjtbase Exp $
 * $Revision: 1.3 $
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
 * $Log: jdd_netapi.h,v $
 * Revision 1.3  2006/07/03 07:26:11  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.7  2006/03/04 13:04:31  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2006/03/01 14:57:58  kumardevhtmlbrow
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

#ifndef _JDD_NETAPI_H
#define _JDD_NETAPI_H

#ifdef __cplusplus
extern "C"
{
#endif

/** 
 * @file jdd_netapi.h
 * @ingroup Network
 * @brief Defines the network interface abstract calls.
 *
 */

/**
 * @brief This function initializes the network layer, if required by the platform 
 * before using the network socket calls. 
 */
JC_RETCODE jdd_NetInitialize (void) ;

/**
 * @brief This function creates an endpoint for communication.
 * @param[in] eSocketDomain Specifies the socket domain for the communication
 * @param[in] eSocketType Specify the type of the socket whether the TCP (E_TCP_SOCKET) 
 *						  connection oriented type of socket or UDP (E_UDP_SOCKET) 
 *						  connectionless type of socket.
 * @param[in] eProtocol Specifies the protocol type. This flag is currently not used.
 * @param[in] uiConnID Specifies the data connection identifier for which the 
 *					   the created socket needs to be bounded with the specific 
 *					   transport service provided. 
 * @param[in] piSocketID Returns the socket identifier associated with the created socket.
 * @retval JC_ERR_NET_SOCKET Returns this error code when there is any error in 
 *							 creating the socket.
 *
 * This function creates a socket identifier and any related resources to be allocated 
 * and bound to a specific transport service provider specified by the uiConnID.
 * If the uiConnID has zero then it needs to bounded with the default service provider.
 * @see jdd_NetClose
 */

JC_RETCODE jdd_NetSocket (ESocketDomain eSocketDomain, ESocketType eSocketType, 
						  ESocketProtocol eProtocol, JC_UINT32 uiConnID,
						  JC_INT32 *piSocketID) ;

/**
 * @brief This function binds the specfied socket with the associated local address.
 * @param[in] uiSocketID Specifies the socket descriptor, which is returned by #jdd_NetSocket API.
 * @param[in] pcBindAddress Specifies the address to which the socket needs to be binded. 
 *							If the value is NULL it needs to be binded to the default service 
 *							provider or the provider which is already binded during the socket creation.
 * @param[in] uhPortNo Specifies the local port number to which the socket to be binded.
 * 
 * This function is used on an unconnected socket before subsequent calls to the
 * connect or listen functions. It is used to bind to either connection-oriented 
 * (stream) or connectionless (datagram) sockets. The socket is binded to
 * specific data connection when it is created, this function is used to 
 * bind to the specific port number. 
 */
JC_RETCODE jdd_NetBind (JC_UINT32 uiSocketID, JC_INT8 *pcBindAddress, JC_UINT16 uhPortNo) ;

/**
 * @brief This function establishes the connection to the specified address.
 * @param[in] uiSocketID Specifies the socket descriptor which needs to be connected
 * @param[in] pcIpAddress Specifies the IP Address of the socket to which the connection should be established.
 * @param[in] uhPortNo Specifies the destination port number to which the socket should be connected.
 * @param[in] pvApplicationData This flag is not used.
 * @retval JC_ERR_NET_CONNECT Returns when the connection request to the peer was not successful.
 
 * 
 * In case of nonblocking sockets this function will return JC_ERR_WOULDBLOCK. It 
 * should call jdd_NetSelect function to determine the completion of the connection.
 * In case of the blocking sockets, return value JC_OK indicates that socket is 
 * successfully connected. This returns only after the socket is accepted or 
 * the JC_ERR_NET_CONNECT is returned.
 */
JC_RETCODE jdd_NetConnect (JC_UINT32 uiSocketID, JC_INT8 *pcIpAddress, JC_UINT16 uhPortNo, 
						   void *pvApplicationData) ;

/**
 * @brief This function gets the status of the socket waiting to perform send/receive operations.
 * @param[in] uiSocketID Specifies the socket descriptor
 * @param[in] eSocketDescriptor Specifies the type of the socket descriptor.
 * @param[in] uiTimeInMilliSeconds Specifies the time period in milliseconds to wait.
 * @retval JC_ERR_NET_SOCKET_NOT_READY Specifies that the socket is not ready for the specified descriptor
 * @retval JC_EER_NET_SOCKET Specifies that there was an error in the socket.
 */
JC_RETCODE jdd_NetSelect (JC_UINT32 uiSocketID, ESocketDesType eSocketDescriptor, 
						  JC_UINT32 uiTimeInMilliSeconds) ;

/**
 * @brief ??
 */
JC_RETCODE jdd_NetAccept (JC_UINT32 uiSocketID, JC_INT8 **ppcAcceptAddress,
						  JC_UINT16 *puhPortNo, JC_UINT32   *puiNewSocketID) ;
/**
 * @brief This function places a socket a state where it is listening for an incoming connection.
 * @param[in] uiSocketID Specifies the socket descriptor.
 * @param[in] uiTotalConnections Specifies the maximum number of the incoming connections
 *					that are accepted.
 * @see jdd_NetBind
 */
JC_RETCODE jdd_NetListen (JC_UINT32 uiSocketID, JC_UINT32 uiTotalConnections) ;

/**
 * @brief This function sends the data to the specific destination. 
 * @param[in] uiSocketID Specifies the socket descriptor
 * @param[in] pucSendBuffer Specifies the byte stream which needs to be transmitted.
 * @param[in] uiSendBufferLen Specifies the length of the content (pucSendBuffer) that needs to be transmitted.
 * @param[in] eFlags Currently this flag is not used
 * @param[in] pstDestAddr Specifes the information about the destination address
 * @param[in] uiDestAddrLen Specifies the address of #JC_NET_ADDR.
 * @param[out] puiSentBytes Returns the actual number of bytes that has been transmitted from the given byte stream.
 * 
 * @retval JC_ERR_NET_SEND Specifies that the data could not be sent on the socket.
 * @retval JC_ERR_NET_SOCKET Specifies that there was an error in the socket.
 
 *
 * This function shall be used to send a datagram to a specified peer socket identified by the destination address.
 * @see jdd_NetRecvFrom
 */
JC_RETCODE jdd_NetSendTo (JC_UINT32 uiSocketID, JC_UINT8 *pucSendBuffer, 
						  JC_UINT32 uiSendBufferLen, ESendFlags eFlags, 
						  JC_NET_ADDR *pstDestAddr, JC_UINT32 uiDestAddrLen, 
						  JC_UINT32 *puiSentBytes) ;

/**
 * @brief This function receives the data from the socket and stores the source address. 
 * @param[in] uiSocketID Specifies the socket descriptor
 * @param[in] eFlags This flag is not used.
 * @param[out] pucRecvBuffer Specifies the buffer which will received the data from the socket.
 * @param[in] uiBufferLength Specifies the size of the pucRecvBufer.
 * @param[out] puiReceivedBytes Returns the actual number of bytes received
 * @param[out] pstSourceAddress Returns the information about the source address. This information is available only if this is not NULL.
 * @param[in] uiSourceAddressLen Specifies the size of the pstSourceAddress.
 * @retval JC_ERR_NET_CONNECT	Specifies that the connection request to peer was not successful
 * @retval JC_ERR_NET_RECV		Specifies that the data could not be received from the socket.
 * @retval JC_ERR_NET_SOCKET	Specifies that there was an error in the socket
 
 * 
 * This function shall be used only for datagram sockets.
 * @see jdd_NetSendTo
 */
JC_RETCODE jdd_NetRecvFrom (JC_UINT32 uiSocketID, ERecvFlags eFlags, JC_UINT8 *pucRecvBuffer, 
							JC_UINT32 uiBufferLength, JC_UINT32 *puiReceivedBytes,
							JC_NET_ADDR *pstSourceAddress, JC_UINT32 uiSourceAddressLen) ;

/**
 * @brief This function is invoked to receive the data on the connected socket.
 * @param[in] uiSocketID Specifies the socket descriptor
 * @param[out] pucRecvBuffer Specifies the buffer for the incoming data.
 * @param[in] uiBufferSize Specifies the size of the pucRecvBuffer. The network layer should not copy more than this value to the pucRecvBuffer.
 * @param[out] puiReceivedBytes Returns the number of bytes that is copied to the pucRecvBuffer.
 * @param[in] eFlags Currently this is not used.
 * 
 * @see jdd_NetSend
 */
JC_RETCODE jdd_NetRecv (JC_UINT32 uiSocketID, JC_UINT8 *pucRecvBuffer, JC_UINT32 uiBufferSize,
						JC_UINT32 *puiReceivedBytes, ERecvFlags eFlags) ;

/**
 * @brief This function is invoked to send the data on the connected socket.
 * @param[in] uiSocketID Specifies the socket descriptor.
 * @param[in] pucSendBuffer Specifies the byte stream which needs to be transmitted.
 * @param[in] uiSendBufferLen Specifies the length of the content (pucSendBuffer) that needs to be transmitted.
 * @param[out] puiSentBytes Returns the number of bytes that was transmitted.
 * 
 * @retval JC_ERR_NET_CONNECT Specifies that the connection request to peer was not successful.
 * @retval JC_ERR_NET_TIMEOUT Specifies that the time has expired before the network operation took place.
 * @retval JC_ERR_NET_SEND	Specifies that the data could not be sent on the socket.
 * @retval JC_ERR_NET_SOCKET	Specifies that there was an error in the socket.
 
 *
 * If this function returns JC_ERR_NET_WOULDBLOCK, it needs to resend the data for transmission.
 * @see jdd_NetRecv
 */
JC_RETCODE jdd_NetSend (JC_UINT32 uiSocketID, JC_UINT8 *pucSendBuffer, JC_UINT32 uiSendBufferLen, 
						JC_UINT32 *puiSentBytes) ;

/**
 * @brief ??
 */
JC_RETCODE jdd_NetGetSockName (JC_UINT32 uiSocketID, JC_INT8 *pcIpAddress,
							   JC_UINT16 *puhPortNo) ;

/**
 * @brief This function retrieves the host IP address from the host name.
 * @param[in] uiConnID	Specifies the data connection ID
 * @param[in] pcHostName Specifies the host name, where IP address needs to be resolved.
 * @param[in] pvApplicationData This flag is not used
 * @param[in] ppucIpAddresses ??
 * @retval JC_ERR_NET_DNS_WOULDBLOCK Specifies that DNS could not be resolved as the environment does not blocking call.
 *
 * When the platform does not support the blocking call interface, the network layer
 * should post the result to the application task.
 */
JC_RETCODE jdd_NetGetHostByName (JC_UINT32 uiConnID, const JC_INT8 *pcHostName, 
								 void *pvApplicationData, JC_UINT8 **ppucIpAddresses) ;

/**
 * @brief This function closes an existing socket.
 * @param[in] uiSocketID Specifies the socket descriptor
 * This function is used to release the socket descriptor so that further references 
 * to this socket will fail. 
 * @see jdd_NetSocket
 */
JC_RETCODE jdd_NetClose (JC_UINT32 uiSocketID) ;

/**
 * @brief This function sets the socket option. 
 * @param[in] uiSocketID Specifies the socket descriptor.
 * @param[in] eSocketLevel Specifies the socket level.
 * @param[in] eOptName Specifies the name of the option which needs to be set.
 * @param[in] pvOptValue Specifies the value which needs to be set.
 * @param[in] uiOptLen Specifies the size of the pvOptValue.
 *
 * Specifies the different socket options for different socket level. 
 * For boolean option in order to enable the pvOptValue should be nonzero. 
 * To enable the option set the pvOptValue parameter to Zero. 
 * For Integer and structure options the pvOptValue hold the actual value.
 */
JC_RETCODE jdd_NetSetSockOpt (JC_UINT32 uiSocketID, ESocketLevel eSocketLevel, 
							  EOptionName  eOptName, void *pvOptValue, JC_UINT32 uiOptLen) ;

/**
 * @brief This function sets the socket option. 
 * @param[in] uiSocketID Specifies the socket descriptor.
 * @param[in] eSocketLevel Specifies the socket socket level.
 * @param[in] eOptName Specifies the name of the option which needs to be get.
 * @param[out] pvOptValue Specifies the option value which needs to be retrieved.
 * @param[in,out] puiOptLen Specifies the size of the pvOptValue. Returns the actual size that is used.
 *
 */
JC_RETCODE jdd_NetGetSockOpt (JC_UINT32 uiSocketID, ESocketLevel eSocketLevel,
							  EOptionName  eOptName, void *pvOptValue, JC_UINT32 *puiOptLen) ;

/**
 * @brief
 */
JC_RETCODE jdd_NetAbortConnection (JC_UINT32 uiSocketID) ;

/**
 * @brief
 */
JC_BOOLEAN jdd_NetInetAddress (const JC_INT8 *pcIPAddress, JC_UINT32 uiConnID, JC_UINT32 *puiIP) ;


/**
 * @brief
 */
JC_UINT16 jdd_NetHtons (JC_UINT16 uhVal) ;

/**
 * @brief
 */
JC_UINT16 jdd_NetNtohs (JC_UINT16 uhVal) ;

/**
 * @brief
 */
JC_UINT32 jdd_NetHtonl (JC_UINT32 uiVal) ;

/**
 * @brief
 */
JC_UINT32 jdd_NetNtohl (JC_UINT32 uiVal) ;

JC_RETCODE jdd_NetNtoA (JC_UINT32 uiAddr, JC_UINT32 *puiSize, JC_INT8 *pcIPAddress) ;

#ifdef __cplusplus
}
#endif


#endif

/* END OF FILE */


