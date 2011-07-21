
/***************************************************************************
 * $Id: jdi_http.h,v 1.16 2009/01/19 06:02:57 browserdevjtbase Exp $
 * $Revision: 1.16 $
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
 * $Log: jdi_http.h,v $
 * Revision 1.16  2009/01/19 06:02:57  browserdevjtbase
 * Core update
 *
 * Revision 1.65  2009/01/06 07:31:52  sriramdevhtmlbrow
 * New member iMaxSocketAliveTime added in set option to configure the persistent socket alive time at run time.
 *
 * Revision 1.64  2008/11/07 10:35:37  sriramdevhtmlbrow
 
 *
 * Revision 1.63  2008/08/07 11:53:50  sriramdevhtmlbrow
 * Added uiSecurePort in profile which can be used by application incase of proxy connection for secure requests. If this is specified then socket connection will be made using this port for secure requests instead of the normal proxy port.(jtrack issue 2299 - imode requirement)
 *
 * Revision 1.62  2008/07/25 13:52:52  sriramdevhtmlbrow
 * Added new status code 452 for imode
 *
 * Revision 1.61  2008/06/16 11:34:32  sriramdevhtmlbrow
 * Added new http status codes defined by i-mode for MAIL and PUSH purposes under the macro IMODE_HTTP_EX_STATUS_CODE.
 *
 * Revision 1.60  2008/03/04 15:04:03  kumardevhtmlbrow
 * Update.
 *
 * Revision 1.59  2008/01/14 06:05:28  kumardevhtmlbrow
 * Updated the comments.
 *
 * Revision 1.58  2008/01/03 06:12:00  kumardevhtmlbrow
 * Doxygen warning fixed.
 *
 * Revision 1.57  2008/01/02 14:47:29  sriramdevhtmlbrow
 * Updated documentation of files
 *
 * Revision 1.56  2007/06/12 12:52:32  sriramdevhtmlbrow
 * Provided an API to the application to set the HTTP Protocol version.
 *
 * Revision 1.55  2007/04/05 12:18:15  kumardevhtmlbrow
 * Update http for handling error code.
 *
 * Revision 1.54  2007/01/12 06:18:21  kumardevhtmlbrow
 * Lint Warning Fix
 *
 * Revision 1.53  2006/12/20 09:01:30  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.52  2006/12/19 05:12:07  kumardevhtmlbrow
 * Update
 *
 * Revision 1.51  2006/11/24 07:28:54  kumardevhtmlbrow
 * Lint Warning fix
 *
 * Revision 1.50  2006/11/10 05:44:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.49  2006/11/08 12:45:22  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.48  2006/11/06 14:08:20  kumardevhtmlbrow
 * Update
 *
 * Revision 1.47  2006/10/19 06:13:21  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.46  2006/09/21 06:43:21  kumardevhtmlbrow
 * Update for generic SSL Layer
 *
 * Revision 1.45  2006/08/09 12:18:50  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.44  2006/06/15 15:10:59  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.43  2006/06/12 08:49:45  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.42  2006/06/07 15:24:36  kumardevhtmlbrow
 * LINT Warning
 *
 * Revision 1.41  2006/05/26 10:04:11  kumardevhtmlbrow
 * Updated....
 *
 * Revision 1.40  2006/05/04 05:15:19  kumardevhtmlbrow
 * updated for sending the progress indication total bytes to recv.
 *
 * Revision 1.39  2006/05/04 04:16:29  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.38  2006/04/28 07:36:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.37  2006/03/24 05:44:31  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.36  2006/03/11 12:59:09  sriramdevhtmlbrow
 * Documentation updated
 *
 * Revision 1.35  2006/02/13 06:11:03  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

/**
 * @mainpage
 * 
 * @version 5.0
 * Interface Description
 * 
 * @section Introduction
 * The Jataayu HTTP Stack API is a simple but powerful means for application developers to build 
 * HTTP client applications. These applications could be complete browsers or simpler ones like 
 * server monitors or data probes. The Jataayu HTTP Stack API is available in ANSI C version. 
 * The ANSI C version is available on a variety of platforms including desktop and 
 * embedded operating systems and can be ported to most environments supporting ANSI C. 
 * This help file describes the ANSI C version of the Jataayu HTTP Stack API.
 * 
 * @section Interfaces Stack Interfaces
 * The stack provides the following interface functionalities.
 * @li Initialization of stack using #jdi_WPHTTPInitialize
 * @li Timeout for socket operations used by the stack can be set using #jdi_WPHTTPSetTimeOut
 * @li Adding static headers which are common to every request in that session 
 *	   using #jdi_WPHTTPAddStaticHeader
 * @li Creating a request uisng #jdi_WPHTTPCreateTransaction
 * @li Adding dynamic headers which are particular to a request using #jdi_WPHTTPAddDynamicHeader
 * @li Adding basic authorization if required using #jdi_WPHTTPAddAuthorization
 * @li Adding digest authorization if required using #jdi_WPHTTPAddDigestAuthorization
 * @li Sending request to the server by establishing a socket connection using 
 *     network abstraction using #jdi_WPHTTPSendData
 * @li Sending the post data to the server if required using #jdi_WPHTTPSendMoreData
 * @li Receiving response from the server and parsing it and sending the response to the 
 *	   upper layers in the form of buffers using #jdi_WPHTTPDataPoll
 * @li Deleting the completed request using #jdi_WPHTTPDeleteTransaction
 * @li Deinitialization of stack using #jdi_WPHTTPDeInitialize
 * @li Provides interface for handling following socket events in case of event based platforms
 * @li DNS resolution event is handled using #jdi_WPHTTPSetHostName
 * @li Socket connection event is handled using #jdi_WPHTTPSetConnectionState
 * @li Socket read event is handled using #jdi_WPHTTPReadData
 * @li Socket close event is handled using #jdi_WPHTTPCloseConnection
 * 
 * @section datatype Data Types
 * The generic data types that are referred in this document are JC_INT8, JC_UINT8, 
 * JC_INT16, JC_UINT16, JC_INT32, JC_UINT32 and JC_BOOLEAN.
 * @li JC_INT8 refers to signed 8 bit data.
 * @li JC_UINT8 refers to unsigned 8 bit data.
 * @li JC_INT16 refers to signed 16 bit data.
 * @li JC_UINT16 refers to unsigned 16 bit data.
 * @li JC_INT32 refers to signed 32 bit data.
 * @li JC_UINT32 refers to usigned 32 bit data.
 * @li JC_BOOLEAN refers to the datatype where it holds only two values such as E_TRUE and E_FALSE.
 *
 * @section Abstraction Abstraction Layer
 * The Jataayu HTTP Stack has built based on the Jataayu Client Framework (JCF), which abstract the 
 * platform specific API calls. The platform needs to implement the API's of the required abstraction 
 * layers. HTTPStack uses the following abstraction layers:
 * @li Memory 
 * @li Network
 * @li Timer
 * @li Log
 * @li Misc
 * 
 * 
 */
/* 
 * @defgroup HTTPStack HTTPStack Interface Layer
 * @defgroup Memory Memory Abstraction Layer
 * @defgroup Network Network Abstraction Layer
 * @defgroup Timer Timer Abstraction Layer
 * @defgroup Log Log Abstraction Layer
 * @defgroup MISC Miscellaneous Abstraction Layer 
 */

/**
 * @file jdi_http.h
 * @ingroup HTTP
 * @brief Defines the exposed enumerations, structures and functions of 
 *		  the HTTP module that can be used by transporttask module.
 * 
 * This interface provides APIs for folliwing operations. Intialization of 
 * HTTP module, creating a transaction, deleting a transaction, sending a 
 * request to the server, receiving response from the server, setting timout 
 * values for different operations like connect, send, received and select, 
 * sending post data, adding static and dynamic headers, setting authorization 
 * information, setting the state of a socket connetion, reading data from 
 * a socket, closing socket connections, getting the status of a request, 
 * checkign data availability for a request in the data queue of socket pools,
 * some security related functions and deinitialization of HTTP module.
 */

#ifndef _JDI_HTTP_H
#define _JDI_HTTP_H

#include <jdi_transportmodule.h>
#include <jdi_mimeinfo.h>
#ifdef HTTP_WITH_SSL
#ifdef JCF_SSL
#include <jdi_ssl.h>
#elif defined SSL_4_5
#include <jdi_httpssl.h>
#endif
#endif

/**
 * @brief Specifies that some more response has to be received from 
 *		  the server for the current request.
 */ 
#define JC_ERR_WPHTTP_RECV_CONTINUE					TERMINAL_ERR_CODE(HTTP_MODULE, 0x01)
/**
 * @brief Specifies that the current request has received complete response 
 *		  and the next request has to start receiving response from the server.
 */ 
#define JC_ERR_WPHTTP_RECV_CONTINUE_NEXT_BUFFER		TERMINAL_ERR_CODE(HTTP_MODULE, 0x02)
/**
 * @brief Specifies that the given http method for a request is invalid.
 */ 
#define JC_ERR_WPHTTP_INVALID_HTTP_METHOD			TERMINAL_ERR_CODE(HTTP_MODULE, 0x03)
/**
 * @brief Specifies that the response received from the server is invalid.
 */ 
#define JC_ERR_WPHTTP_INVALID_RESPONSE				TERMINAL_ERR_CODE(HTTP_MODULE, 0x04)
/**
 * @brief Specifies that the given header is not found in the header list.
 */ 
#define JC_ERR_WPHTTP_HEADER_NOT_FOUND				TERMINAL_ERR_CODE(HTTP_MODULE, 0x05)
/**
 * @brief Specifies that a tunnel has to be established with a server.
 */ 
#define JC_ERR_WPHTTP_TUNNEL_ERR					TERMINAL_ERR_CODE(HTTP_MODULE, 0x06)
/**
 * @brief Specifies that the given URL is invalid.
 */ 
#define JC_ERR_WPHTTP_INVALID_URL					TERMINAL_ERR_CODE(HTTP_MODULE, 0x07)
/**
 * @brief Specifies that the port number in the given URL is invalid.
 */ 
#define JC_ERR_WPHTTP_INVALID_PORT_NUMBER			TERMINAL_ERR_CODE(HTTP_MODULE, 0x08)
/**
 * @brief Specifies that user data has to be sent to the server for the current request.
 */ 
#define JC_ERR_WPHTTP_MORE_DATA_TO_SEND				TERMINAL_ERR_CODE(HTTP_MODULE, 0x09)
/**
 * @brief Specifies that the connection operation was timedout.
 */ 
#define JC_ERR_WPHTTP_CONNECTION_TIMEOUT			TERMINAL_ERR_CODE(HTTP_MODULE, 0x0A)
/**
 * @brief Specifies that the send operation was timedout.
 */ 
#define JC_ERR_WPHTTP_SEND_TIMEOUT					TERMINAL_ERR_CODE(HTTP_MODULE, 0x0B)
/**
 * @brief Specifies that the receive operation was timedout.
 */ 
#define JC_ERR_WPHTTP_RECV_TIMEOUT					TERMINAL_ERR_CODE(HTTP_MODULE, 0x0C)
/**
 * @brief Specifies that the header data received from the server is partial.
 */ 
#define JC_ERR_WPHTTP_PARTIAL_HEADER				TERMINAL_ERR_CODE(HTTP_MODULE, 0x0D)
/**
 * @brief Specifies that a response has to be received from the server 
 *		  for the tunnel request.
 */ 
#define JC_ERR_WPHTTP_TUNNEL_RESPONSE				TERMINAL_ERR_CODE(HTTP_MODULE, 0x0E)
/**
 * @brief Specifies that the tunnel request has been sent to the server.
 */ 
#define JC_ERR_WPHTTP_TUNNEL_REQUEST_SENT			TERMINAL_ERR_CODE(HTTP_MODULE, 0x0F)
/**
 * @brief Specifies that the set of cipher suites used in the handshake process is invalid.
 */ 
#define JC_ERR_WPHTTP_INVALID_CIPHERSUITE_SET		TERMINAL_ERR_CODE(HTTP_MODULE, 0x10)
/**
 * @brief Specifies that the process of establishing a tunnel with the server is in progress.
 */ 
#define JC_ERR_WPHTTP_TUNNELLING					TERMINAL_ERR_CODE(HTTP_MODULE, 0x11)
/**
 * @brief Specifies that handshake has to be done with the server.
 */ 
#define JC_ERR_WPHTTP_HANDSHAKE                     TERMINAL_ERR_CODE(HTTP_MODULE, 0x12)
/**
 * @brief Specifies that the given userdata is being sent to the server.
 */ 
#define JC_ERR_WPHTTP_SENDING_USERDATA				TERMINAL_ERR_CODE(HTTP_MODULE, 0x13)
/**
 * @brief Specifies that the current request has to be sent again.
 */ 
#define JC_ERR_WPHTTP_RETRY_TRANSACTION				TERMINAL_ERR_CODE(HTTP_MODULE, 0x14)
/**
 
 */ 
#define JC_ERR_WPHTTP_MAX_REQUEST_COUNT_EXCEEDED	TERMINAL_ERR_CODE(HTTP_MODULE, 0x15)
/**
 * @brief Specifies that the handshake process with the server is successful.
 */ 
#define JC_ERR_WPHTTP_HANDSHAKE_SUCCESSFUL			TERMINAL_ERR_CODE(HTTP_MODULE, 0x16)
/**
 * @brief Specifies that security feature is not supported.
 */ 
#define JC_ERR_WPHTTP_SECURITY_NOT_SUPPORTED		TERMINAL_ERR_CODE(HTTP_MODULE, 0x17)

/**
 * @brief This enumeration contains different states used for socket pools 
 *		  and transactions to trace their status.
 */
typedef enum http_status
{
	E_HTTP_NULL = 0, /**< Initial state. */
	E_HTTP_DNS_RESOLVING, /**< DNS resolution process is in progress. */
	E_HTTP_DNS_FAILED, /**< DNS resolution has failed. */
	E_HTTP_CONNECTING, /**< Connection process with a server is in progress. */
	E_HTTP_CONNECTED, /**< Conection with a server is successful. */
	E_HTTP_TUNNEL, /**< Tunnel has to be established with a server. */
	E_HTTP_TUNNEL_RESPONSE, /**< Response has to be received for the tunnel request. */
	E_HTTP_HANDSHAKE, /**< Handshake has to be done with a server. */
	E_HTTP_HANDSHAKE_DONE, /**< Handshake process is successful. */
	E_HTTP_CONNECT_FAILED, /**< Conection to a server is failed. */
	E_HTTP_CLOSED, /**< Socket connection is closed. */
	E_HTTP_SENDING_USERDATA, /**< User data is being sent to the server. */
	E_HTTP_WAITING_USERDATA, /**< Waiting for the userdata from the application. */
	E_HTTP_SENT, /**< Request has been sent to the server. */
	E_HTTP_RECEIVING, /**< Response is being received from the server. */
	E_HTTP_RECEIVED, /**< Complete response if received from the server. */
	E_HTTP_FAILED /**< Request Failed without completion. */
} E_HTTP_STATE ;

/**
 * @brief This enumeration contains different http methods used for 
 *		  sending requests to servers.
 */
typedef enum http_method_type
{
	E_HTTP_METHOD_HEAD, /**< HTTP method HEAD. */
	E_HTTP_METHOD_GET, /**< HTTP method GET. */
	E_HTTP_METHOD_POST, /**< HTTP method POST. */
	E_HTTP_METHOD_PUT, /**< HTTP method PUT. */
	E_HTTP_METHOD_DELETE /**< HTTP method DELETE. */
} E_HTTP_METHOD_TYPE ;

/**
 * @brief This enumeration contains different types used for 
 *		  differentiating requests.
 */
typedef enum http_request_type
{
	E_HTTP_MAIN_REQUEST, /**< The reqeust is a main page request. */
	E_HTTP_CSS_REQUEST, /**< The request is for a CSS content. */
	E_HTTP_IMAGE_REQUEST, /**< The request if for an Image. */
	E_HTTP_AUDIO_REQUEST, /**< The request if for an Audio content. */
	E_HTTP_VIDEO_REQUEST, /**< The request if for a Video content. */
	E_HTTP_OTHER_REQUEST
} E_HTTP_REQUEST_TYPE ;

/**
 * @brief This enumeration contains timeouts for different operations.
 */
typedef enum _e_timeout_type
{
	E_CONNECT_TIMEOUT, /**< Timout type for connect operation. */
	E_SEND_TIME_OUT, /**< Timout type for send operation. */
	E_RECV_TIME_OUT, /**< Timout type for receive operation. */
	E_SOCKET_ALIVE_TIME_OUT /**< Timout type for socket alive time in case of persistent sockets. */
} E_TIMEOUT_TYPE ; 

/**
 * @brief This enumeration contains timeouts for different select operations.
 */
typedef enum _select_timeout_type
{
	E_SELECT_CONNECT_TIMEOUT, /**< Timout type for select operation for connect operation. */
	E_SELECT_READ_TIMEOUT, /**< Timout type for select operation for send operation. */
	E_SELECT_WRITE_TIMEOUT /**< Timout type for select operation for receive operation. */
} E_SELECT_TIMEOUT_TYPE ; 

/**
 * @brief This structure contains information about a request that wll be used 
 *		  while creating a transaction. Also this information will be used 
 *		  when forming the request that has to be sent to the server.
 */
typedef struct http_request
{
	JC_UINT8				*pucGet ; /**< URL of a request. */
	JC_UINT32				uiUserdataLen ; /**< User data length of a request. */
	JC_UINT32				uiReqId ; /**< Unique id of a Request. */
	E_HTTP_METHOD_TYPE		eHttpMethodType ; /**< HTTP method type such as GET, HEAD etc. */
	E_HTTP_REQUEST_TYPE		eHttpRequestType ; /**< Type of the request such as main page, image etc. */
} ST_HTTP_REQUEST ;

/**
 * @brief This structure contains information about the resonse received from 
 *		  the server. This response will be given to the upper layers.
 */
typedef struct http_response_tag
{
	JC_UINT8	*pucBuffer ; /**< Buffer received  from server for a request. */
	JC_UINT32	uiBufferLen ; /**< Length of pucBuffer. */
	JC_UINT32	uiReqId ; /**< Identification of a request. */
	JC_UINT8	*pucContentType ; /**< Content type of the buffer received from server. */
	JC_UINT8	*pucHeader ; /**< Header received  from server for a request. */
	JC_UINT32	uiHeaderLen ; /**< Length of pucHeader. */
	JC_INT32	iStatus ; /**< HTTP status of a response received from server. */
	JC_UINT32	uiTotalContentLength ; /**< Specifies the total response length. */
	JC_BOOLEAN	bFreeMemory ; /**< If this is true, pucBuffer must be freed. */
	JC_BOOLEAN  bFreeHeader ; /**< If this is true, pucHeader must be freed. */
	struct http_response_tag *pstNext ; /**< Not used. **/
} ST_HTTP_RESPONSE ;

/**
 * @brief This structure contains information about the digest autorization.
 *		  This information will be used in forming the digest response that 
 *		  has to be sent to the server with the request.
 */
typedef struct http_digest_authorization_tag
{
	JC_INT8 *pcDomain ; /**< Specifies the domain for which authorization is required. */
	JC_INT8 *pcUsername ; /**< User name. */
	JC_INT8 *pcPassword ; /**< password. */
	JC_INT8 *pcRealm ; /**< Specifies the realm parameter in the digest authorization. */
	JC_INT8 *pcNonce ; /**< Specifies the nonce parameter in the digest authorization. */
	JC_INT8 *pcOpaque ; /**< Specifies the opaque parameter in the digest authorization. */
	JC_INT8 *pcQOP ; /**< Specifies the qop parameter in the digest authorization. */
	JC_INT8 *pcAlg ; /**< Specifies the algorithm parameter in the digest authorization. */
} ST_HTTP_DIGEST_AUTHORIZATION ;

/**
 * @brief This enumeration contains options for different versions of HTTP protocol.
 */
typedef enum _e_http_verion_type
{
	E_HTTP_VERSION_1_0, /**< Specifies HTTP/1.0 verion. */
	E_HTTP_VERSION_1_1  /**< Specifies HTTP/1.1 verion. */
} E_HTTP_VERSION_TYPE ;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief	This function initializes HTTP module by initializing the HTTP_INFO structure.
 * @param[in] pucServer Specifies the server address to connect incase of proxy connection
 * @param[in] uhPort Specifies the port to connect incase of proxy connection
 * @param[in] vMemHandle Handle used in memory operations
 * @param[in] uiConnID	Specifies the connection ID, which is created using the data connection abstraction layer. 
 *						This ID shall be passed back to the socket layer while creating the new socket.
 * @param[in] iMaxSockets		Maximum number of socket connections that can be used at a time.
 * @param[in] iMaxTcpRecvBuf	Maximum data that can be polled from socket layer at a time.
 * @param[in,out] pvHttpHandle Stores the initialized structure HTT_INFO
 * @retval		JC_OK	Successfully initialized the HTTP module.
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @see			jdi_WPHTTPDeInitialize
 * @note		The parameter pvHttpHandle should not be NULL.
 * @note		The parameter pvHttpHandle should be passed to all 
 *				other jdi functions in HTTP module.
 * 
 * This function is called by transporttask module when it gets 
 * some request from the application.
 */
JC_RETCODE jdi_WPHTTPInitialize (const JC_UINT8		*pucServer, 
								 JC_UINT16			uhPort, 
								 JC_HANDLE			vMemHandle,
								 JC_UINT32			uiConnID,
								 JC_INT32			iMaxSockets,
								 JC_UINT32			iMaxTcpRecvBuf,
								 JC_HANDLE			*pvHttpHandle) ;

/**
 * @brief This function sets the port number to be used for socket connection for secure requests.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[in]	uhSecurePort	Port number to be used for tunneling process
 * @return		No Return value
 *
 * Using this function application can specify the alternative port number which should be used 
 * for socket connection instead of the normal proxy port number for secure requests. This is 
 * application only for the proxy connection.
 */
void jdi_WPHTTPSetSecurePortNumber (JC_HANDLE	*vHttpHandle,
									JC_UINT16	uhSecurePort) ;

/**
 * @brief	This function creates a transaction with the given request details.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[in]	pstHttpRequest	Contains information about the request
 * @retval		JC_ERR_WPHTTP_INVALID_URL The given URL is invalid
 * @retval		JC_ERR_WPHTTP_INVALID_PORT_NUMBER Port number in the given URL is invalid
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @see			jdi_WPHTTPDeleteTransaction
 * @note		The parameters vHttpHandle and pstHttpRequest should not be NULL.
 * 
 * This function is called by transporttask to create a transaction, 
 * when ever it gets a request from the application.
 */
JC_RETCODE jdi_WPHTTPCreateTransaction (JC_HANDLE				vHttpHandle, 
										const ST_HTTP_REQUEST	*pstHttpRequest) ;

/**
 * @brief	This function deletes a trasaction with the given request id.
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	uiReqId		Specifies the request id of a transaction
 * @retval		JC_ERR_INVALID_PARAMETER The given request id is invalid
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error
 * @retval		JC_OK On success.
 * @retval		Others. 
 * @note		The parameters vHttpHandle should not be NULL.
 * 
 * This function is called by transporttask to delete a transaction, 
 * when that request is completely processed.
 */
JC_RETCODE jdi_WPHTTPDeleteTransaction (JC_HANDLE		vHttpHandle, 
										JC_UINT32		uiReqId) ;

/**
 * @brief	This function establishes a socket connection and sends the request to the server.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[out]	puiReqId		Holds the Request ID for which userdata is needed
 * @retval		JC_ERR_NET_WOULDBLOCK A non blocking socket operation is going on
 * @retval		JC_ERR_WPHTTP_SENDING_USERDATA Userdata is being sent to the server
 * @retval		JC_ERR_WPHTTP_TUNNEL_RESPONSE Response for the tunnel request 
 *				has to be received from the server
 * @retval		JC_ERR_WPHTTP_HANDSHAKE Handshake has to be done with the server
 * @retval		JC_ERR_WPHTTP_HANDSHAKE_SUCCESSFUL Handshake process is successful
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error
 * @retval		JC_OK On success.
 * @retval		Others. 
 * @see			jdi_WPHTTPDataPoll
 * @note		The parameters vHttpHandle should not be NULL.
 * 
 * This function is called by transporttask when a request has to be sent to the server
 */
JC_RETCODE jdi_WPHTTPSendData (JC_HANDLE	vHttpHandle, 
							   JC_UINT32	*puiReqId) ;

/**
 * @brief	This function sends the given user data to the server.
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	pucBuffer	Contains the user data
 * @param[in]	uiBufferLen Specifies length of the user data
 * @param[in]	uiReqId		Request id for which the user data has to be sent
 * @retval		JC_ERR_INVALID_PARAMETER The given request id is invalid
 * @retval		JC_ERR_NET_WOULDBLOCK A non blocking socket operation is going on
 * @retval		JC_ERR_WPHTTP_SENDING_USERDATA Userdata is being sent to the server
 * @retval		JC_ERR_WPHTTP_MORE_DATA_TO_SEND Some more data has to be sent to the server
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error
 * @retval		JC_OK On success.
 * @retval		Others. 
 * @see			jdi_CacheInvalidateObject
 * @note		The parameters vHttpHandle and pucBuffer should not be NULL.
 * 
 * This function is called by transporttask when it gets user data for a 
 * request from the application which has to be sent to the server.
 */
JC_RETCODE jdi_WPHTTPSendMoreData (JC_HANDLE	vHttpHandle, 
								   JC_UINT8		*pucBuffer, 
								   JC_UINT32	uiBufferLen, 
								   JC_UINT32	uiReqId) ;

/**
 * @brief	This function receives response from the server and processes it.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[out]	pstHttpResponse Structure containing the processed response
 * @retval		JC_ERR_WPHTTP_RECV_CONTINUE Some more data has to be received 
 *				for the current request from the server
 * @retval		JC_ERR_HTTPDEC_PARTIAL_CONTENT The response received from 
 *				the server is partial.
 * @retval		JC_ERR_HTTPDEC_PARTIAL_HEADER The header data received from 
 *				the server is partial.
 * @retval		JC_ERR_WPHTTP_RECV_CONTINUE_NEXT_BUFFER The current request has 
 *				received complete response and the next request has to start 
 *				receiving response from the server.
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @retval		Others. 
 * @note		The parameters vHttpHandle and pstHttpResponse should not be NULL.
 * 
 * This function is called by transporttask when it has to receive response 
 * for a request. This function will be called once the function jdi_WPHTTPSendData 
 * is completed successfully.
 */
JC_RETCODE jdi_WPHTTPDataPoll (JC_HANDLE		vHttpHandle, 
							   ST_HTTP_RESPONSE	*pstHttpResponse) ;

/**
 * @brief	This function adds the digest authorization information to a request.
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	bIsProxy	Specifies whether it is a proxy server authorization
 * @param[in]	pstDigestAuth Structure containing information about digest authorization
 * @param[in]	pucUrl Specifies the URL for which digest authorization is required
 * @param[in]	uiReqId Identification of the request for which 
 *				digest authorization has to be added
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @retval		Others. 
 * @note		The parameters vHttpHandle and pstDigestAuth should not be NULL.
 * 
 * This function is called by transporttask when it gets a request from the application
 * which requires digest authentication.
 */
JC_RETCODE jdi_WPHTTPAddDigestAuthorization (JC_HANDLE								vHttpHandle, 
											 JC_BOOLEAN								bIsProxy, 
											 const ST_HTTP_DIGEST_AUTHORIZATION		*pstDigestAuth, 
											 const JC_UINT8							*pucUrl, 
											 JC_UINT32								uiReqId) ;

/**
 * @brief	This function adds the basic authorization information to a request.
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	bIsProxy	Specifies whether it is a proxy server authorization
 * @param[in]	pucName		Specifies the username
 * @param[in]	pucPassword Specifies the password
 * @param[in]	uiReqId Identification of the request for which 
 *				basic authorization has to be added
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @retval		Others. 
 * @note		The parameters vHttpHandle and pucName should not be NULL.
 * 
 * This function is called by transporttask when it gets a request from the application
 * which requires basic authentication.
 */
JC_RETCODE jdi_WPHTTPAddAuthorization (JC_HANDLE		vHttpHandle, 
									   JC_BOOLEAN		bIsProxy, 
									   const JC_UINT8	*pucName, 
									   const JC_UINT8	*pucPassword, 
									   JC_UINT32		uiReqId) ;

/**
 * @brief	This function adds the binding information to the HTTP_INFO structure.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[in]	pcBindAddress	Specifies the IP address to which the 
 *				opened sokcets has to be bound
 * @param[in]	uhBindPort		Specifes the biding port
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @retval		Others. 
 * @note		The parameters vHttpHandle and pcBindAddress should not be NULL.
 * 
 * This function is called by transporttask if the sockets opened in HTTP 
 * should be bound to a particular IP address.
 */
JC_RETCODE jdi_WPHTTPAddBindInfo (JC_HANDLE			vHttpHandle, 
								  const JC_INT8		*pcBindAddress, 
								  JC_UINT16			uhBindPort) ;

/**
 * @brief	This function sets the IP address for a given host name for making a connection.
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	pucHostName	Specifies the host name with which connection has to be made
 * @param[in]	pucAddress	Specifies IP address used for making a connection
 * @param[out]	puiReqID Specifies the Request id of the current request
 * @retval		JC_ERR_FILE_READ If fails during reading from cache file.
 * @retval		JC_ERR_FILE_WRITE If fails during writting to cache file.
 * @retval		JC_ERR_CACHE_HEADER_NOT_EXIST If header not found in cache file.
 * @retval		JC_ERR_CACHE_DOMAIN_MISMATCH If domain mismatch occurs.
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @retval		Others. 
 * @see			jdi_CacheInvalidateObject
 * @note		None of the parameters should be NULL.
 * 
 * This function is called by transporttask when it gets a DNS event.
 */
JC_RETCODE jdi_WPHTTPSetHostName (JC_HANDLE			vHttpHandle,
								  const JC_UINT8	*pucHostName,
								  JC_UINT8			*pucAddress,
								  JC_UINT32			*puiReqID) ;

/**
 * @brief	This function deinitializes the HTTP module.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle should not be NULL.
 * 
 * This function is called by transporttask when it gets a session destroy 
 * event from the application.
 */
JC_RETCODE jdi_WPHTTPDeInitialize (JC_HANDLE	vHttpHandle) ;

/**
 * @brief	This function sets the state of a socket connection.
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	iSocketId	Specifies the socket id
 * @param[in]	eHttpState	HTTP state
 * @param[out]	puiRequestID	Holds the request id which is completed
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle and puiRequestID should not be NULL.
 * 
 * This function is called by transporttask when it gets a socket event from the application
 */
JC_RETCODE jdi_WPHTTPSetConnectionState (JC_HANDLE		vHttpHandle, 
										 JC_INT32		iSocketId, 
										 E_HTTP_STATE	eHttpState,
										 JC_UINT32		*puiRequestID) ;

/**
 * @brief	This function sets the timeout for connect, send and receive operations.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[in]	eTimeOutType	Specifies the timeout type
 * @param[in]	uiTimeOut		Timeout value in seconds
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle should not be NULL.
 * 
 * This function is called by transporttask to set the timeout value for connect, 
 * send and receive operations.
 */
JC_RETCODE jdi_WPHTTPSetTimeOut (JC_HANDLE		vHttpHandle, 
								 E_TIMEOUT_TYPE	eTimeOutType, 
								 JC_UINT32		uiTimeOut) ;

/**
 * @brief	This function sets the timeout for select operations.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[in]	eTimeOutType	Specifies the timeout type
 * @param[in]	uiTimeout		Timeout value in seconds
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle should not be NULL.
 * 
 * This function is called by transporttask to set the timeout value for select 
 * operation for connect, send and receive operations.
 */
JC_RETCODE jdi_WPHTTPSetSocketSelectTimeout (JC_HANDLE				vHttpHandle, 
											 E_SELECT_TIMEOUT_TYPE	eTimeOutType, 
											 JC_UINT32				uiTimeout) ;

/**
 * @brief	This function closes all the socket connections
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle should not be NULL.
 * 
 * This function is called by transporttask when it gets a user cancel request 
 * from the application.
 */
JC_RETCODE jdi_WPHTTPCloseConnection (JC_HANDLE		vHttpHandle) ;

/**
 * @brief	This function gets the state of a request.
 * @param[in]	vHttpHandle			Pointer to the HTTP_INFO structure
 * @param[in]	uiReqId				Identification of a request
 * @param[out]	puiSentBytes		No of bytes sent to  the server
 * @param[out]	puiReceivedBytes	No of bytes received from the server
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle should not be NULL.
 * 
 * This function is called by transporttask when it has to send the status 
 * of a request to the application.
 */
JC_RETCODE jdi_WPHTTPGetTransactionStatus (JC_HANDLE	vHttpHandle, 
										   JC_UINT32	uiReqId, 
										   JC_UINT32	*puiSentBytes, 
										   JC_UINT32	*puiReceivedBytes) ;

/**
 * @brief	This function checks all the sockets and closes those sockets that are timedout.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[in]	bForceful		Specifies if the empty socketpools have 
 *								to be closed forcefully.
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle should not be NULL.
 * 
 * This function is called by transporttask to clean up the timedout sockets 
 * before creating a transaction in HTTP. If bForceful is E_TRUE then all the 
 * empty socketpools will be closed forcefully.
 */
JC_RETCODE jdi_WPHTTPSocketPoolCleanUp (JC_HANDLE	vHttpHandle, 
										JC_BOOLEAN	bForceful) ;

/**
 * @brief	This function adds a header to the HTTP_INFO structure which 
 *			will be added to all the requests.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[in]	pstMimeHeaders	List of headers
 * @param[in]	bIsAccept	Specifies if the header is an accept contenttype header
 * @param[in]	eHttpRequestType	Specifies the type of request
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle and pstMimeHeaders should not be NULL.
 * 
 * This function is called by transporttask to add different headers after 
 * initializing the HTTP module.
 */
JC_RETCODE jdi_WPHTTPAddStaticHeader (JC_HANDLE				vHttpHandle, 
									  ST_MIME_HEADERS		*pstMimeHeaders, 
									  JC_BOOLEAN			bIsAccept, 
									  E_HTTP_REQUEST_TYPE	eHttpRequestType) ;

/**
 * @brief	This function adds a header to a particular request.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[in]	uiRequestId		Reqest id for which the given headers have to be added
 * @param[in]	pstMimeHeaders	List of headers
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle and pstMimeHeaders should not be NULL.
 * 
 * This function is called by transporttask when it gets header information 
 * for a request from the application.
 */
JC_RETCODE jdi_WPHTTPAddDynamicHeader (JC_HANDLE		vHttpHandle, 
									   JC_UINT32		uiRequestId, 
									   ST_MIME_HEADERS	*pstMimeHeaders) ;

/**
 * @brief	This function reads data from the given socket.
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	iSocketId	Socket id from which data has to be read
 * @param[in]	pucReadBuffer	Specifies the data read from the socket (if available).
 * @param[in]	uiDataLen	No of bytes read, if the pucBuffer is not NULL. 
 *							If pucBuffer is NULL, this specifies the number of bytes that can be read from the socket.
 * @retval		JC_ERR_NET_WOULDBLOCK A non blocking socket operation is going on
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @note		The parameter vHttpHandle should not be NULL.
 * 
 * This function is called by transporttask when it gets socket read event 
 * from the application.
 */
JC_RETCODE jdi_WPHTTPReadData (JC_HANDLE			vHttpHandle,
							   JC_INT32				iSocketId, 
							   const JC_UINT8		*pucReadBuffer,
							   JC_UINT32			uiDataLen) ;

/**
 * @brief	This function checks the data queue in a socket pool if data is 
 *			available for a given request or not.
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	uiReqId		request for which data availability has to be checked
 * @retval		E_TRUE	Data is available for the given request
 * @retval		E_FALSE	Data is not available for the given request
 * @note		The parameter vHttpHandleshould not be NULL.
 * 
 * This function is called by transporttask to check for data for a particular 
 * request when the bearer connection is down.
 */
JC_BOOLEAN jdi_WPHTTPIsDataAvailable (JC_HANDLE		vHttpHandle, 
									  JC_UINT32		uiReqId) ;

/**
 * @brief	This function sets the offline mode in HTTP.
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	bIsOffLine	If it is true, it specifies that the data has to be 
 *				read from the socket pool data queue only
 * @retval		JC_OK	Success code
 * @note		The parameter vHttpHandle should not be NULL.
 * 
 * This function is called by transporttask to set offline mode in HTTP 
 * so that when bearer connection is down data will be read only from the 
 * socket pool data queue and sockets will not be polled for data.
 */
JC_RETCODE jdi_WPHTTPSetOffLineStatus (JC_HANDLE		vHttpHandle, 
									   JC_BOOLEAN		bIsOffLine) ;

/**
 
 * @param[in]	vHttpHandle		Handle that holds the HTTP_INFO structure.
 
  
 * @retval JC_OK	The process is successful.
 * @retval Others	Error in the process.
 * @note	The parameter vHttpHandle should not be NULL and uiMaxSockets should not be zero.
 * 
 
 * can be opened by HTTP Stack at any time to process the requests.
 */

JC_RETCODE jdi_WPHTTPSetMaxSockets (JC_HANDLE		vHttpHandle, 
									JC_UINT32		uiMaxSockets) ;

/**
 
 *		  http stack currently.
 * @param[in]	vHttpHandle		Handle that holds the HTTP_INFO structure.
 
 * @note		The parameters vHttpHandle and puiMaxRequests should not be NULL.
 * @retval JC_OK	Success code
 * 
 
 * can be processed by HTTP Stack currently.
 */
JC_RETCODE jdi_WPHTTPGetMaxRequestCount (JC_HANDLE		vHttpHandle, 
										 JC_UINT32		*puiMaxRequests) ;

/**
 * @brief This function sets the HTTP protocol version type.
 * @param[in]	vHttpHandle		Handle that holds the HTTP_INFO structure.
 * @param[in]	eVersion		Specifies the HTTP Protocol Version type.
 * @return No Return value
 *
 * This function is called by transporttask to set the protocol version. This value 
 * will be used while sending the requests to the server.
 */
void jdi_WPHTTPSetProtocolVersion (JC_HANDLE				vHttpHandle,
								   E_HTTP_VERSION_TYPE		eVersion) ;

#ifdef HTTP_DECODER_ENABLE

/**
 * @brief	This function parses the given header data and forms the header list.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[in]	pucHeader		Header data that has to be parsed
 * @param[in]	uiHeaderLength	Length of header data
 * @param[in]	ppstMimeHeaders Holds the list of headers that are formed
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle, pucHeader and ppstMimeHeaders should not 
 *				be NULL and uiHeaderlength should not be zero.
 * 
 * This function is called by transporttask when it wants to parse the given 
 * header data to form the header list.
 */
JC_RETCODE jdi_WPHTTPParseHeaders (JC_HANDLE		vHttpHandle, 
								   JC_UINT8			*pucHeader, 
								   JC_UINT32		uiHeaderlength, 
								   ST_MIME_HEADERS	**ppstMimeHeaders) ;
#endif

/**
 * @brief	This function checks the validity of an HTTP Handle for the given socket.
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[in]	iSocket Specifies the URL Domain which need to be invalidated.
 * @retval		E_TRUE  If the given socket belongs to the given HTTP handle
 * @retval		E_TRUE  If the given socket does not belong to the given HTTP handle 
 * @see			jdi_CacheInvalidateObject
 * @note		The parameter vHttpHandleshould not be NULL.
 * 
 * This function is called by transporttask to check the validity of a handle 
 * for a given socket.
 */
JC_BOOLEAN jdi_WPHTTPIsHandleForSocket (JC_HANDLE	vHttpHandle, 
										JC_INT32	iSocket) ;


#ifdef HTTP_WITH_SSL
/* Security Related */


/**
 * @brief This structure contains information about user certificates 
 *		  which will be used in the handshake process with a server.
 */
typedef struct ssl_user_certificate_info
{
	JC_UINT8			*pucCert ; /**< Cetificate buffer. */
	JC_UINT8			*pucPswd ; /**< Password used for decoding the certificate. */
	JC_UINT8			*pucKey ; /**< Key used for the decoding the certificate. */
	JC_UINT32			uiKeyLen ; /**< Length of the key. */
	JC_UINT32			uiLen ; /**< total lenght of the certificate. */
} ST_SSL_USER_CERTIFICATE_INFO ;

/* Security functions */

/**
 * @brief	This is the prototype of a callback which will be called by HTTP 
 *			to pass certificate information to the application.
 * @param[in]	vHandle		Application handle
 * @param[in]	puBuffer	Raw Certificate Data from the server
 * @param[in]	uiCertificate	Certificate length
 * @retval		The callback function can return any application related error.
 * @note		The parameters vHandle and pucBuffer should not be NULL and 
 *				uiCertificate should not be zero.
 *
 * The application needs to implement a callback with the following prototype 
 * in order to get the certificate information from HTTP.
 */
typedef JC_RETCODE (*CB_SSL_CERT_CALLBACK) (JC_HANDLE		vHandle, 
											const JC_UINT8	*pucBuffer, 
											JC_UINT32		uiCertificate) ;

/**
 * @brief	This is the prototype of a callback which will be called by HTTP 
 *			to intimate the upper layer that secure session will be started.
 * @param[in]	vHandle		Application handle
 * @retval		The callback function can return any application related error.
 * @note		The parameter vHandle should not be NULL.
 *
 * The application needs to implement a callback with the following prototype 
 * in order to set the security parameters.
 */
typedef JC_RETCODE (*HTTP_SECURE_SESSION_CALLBACK)(JC_HANDLE	vHandle) ;

/**
 * @brief	This function adds the list of cipher suites to HTTP_INFO structure, 
 *			which will be used during handshake process.
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	peCipherSuite	List of cipher suites
 * @param[in]	uiCipherSuiteCount Specifies the count of cipher suites
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle and phCipherSuite should not be NULL.
 * 
 * This function is called by transporttask to set the list of cipher suites 
 * in HTTP, which will be used in handshake process.
 */
JC_RETCODE jdi_WPHTTPSetCipherSuites (JC_HANDLE					vHttpHandle, 
									  const E_SSL_CIPHER_SUITE	*peCipherSuite, 
									  JC_UINT32					uiCipherSuiteCount) ;

/**
 * @brief	This function adds the user certificate information to HTTP_INFO structure, 
 * @param[in]	vHttpHandle		Pointer to the HTTP_INFO structure
 * @param[in]	pstUserCertInfo	Contains the user certificate information
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle and pstUserCertInfo should not be NULL.
 * 
 * This function is called by transporttask to set user certificate information 
 * in HTTP, which will be used in handshake process.
 */
JC_RETCODE jdi_WPHTTPAddUserCertificate (JC_HANDLE							vHttpHandle, 
										 const ST_SSL_USER_CERTIFICATE_INFO	*pstUserCertInfo) ;

/**
 * @brief	This function adds the root certificate information to HTTP_INFO structure, 
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	pucCert		Root certificate information
 * @param[in]	uiCertLen	Length of the certificate data
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle and pucCert should not be NULL and 
 *				uiCertLen should not be zero.
 * 
 * This function is called by transporttask to set root certificate information 
 * in HTTP, which will be used in handshake process.
 */
JC_RETCODE jdi_WPHTTPAddRootCertificate (JC_HANDLE			vHttpHandle, 
										 const JC_UINT8		*pucCert, 
										 JC_UINT32			uiCertLen) ;

/**
 * @brief	This function gets the cipher suite negotiated by the server 
 *			during handshake process.
 * @param[in]	vHttpHandle				Pointer to the HTTP_INFO structure
 * @param[in]	peNegotiatedCipherSuite	Holds the negotiated cipher suite
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle and peNegotiatedCipherSuite should not be NULL.
 * 
 * This function is called by transporttask when it has to send the details 
 * about the handshake process to the application.
 */
JC_RETCODE jdi_WPHTTPGetNegotiatedCipherSuites (JC_HANDLE				vHttpHandle, 
												E_SSL_CIPHER_SUITE		*peNegotiatedCipherSuite) ;

/**
 * @brief	This function registers a callback function to get the cartificate information.
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	cbCertCallBack	Specifies the callback
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle and cbCertCallBack should not be NULL.
 * 
 * This function is called by transporttask to register for the certificate information
 */
JC_RETCODE jdi_WPHTTPRegisterCertificateCallback (JC_HANDLE				vHttpHandle, 
												  CB_SSL_CERT_CALLBACK	cbCertCallBack) ;

/**
 * @brief	This function registers a callback function to get intimation of secure session
 * @param[in]	vHttpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	cbSecureSessionCallback	Specifies the callback
 * @param[in]	pvAppArg	Specifies the application argument that should be passed when 
 *				the callback is called
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle and cbCertCallBack should not be NULL.
 * 
 * This function is called by transporttask to register for the secure session intimation.
 */
JC_RETCODE jdi_WPHTTPRegisterSecureSessionIndicationCallback (JC_HANDLE						vHttpHandle,
															  HTTP_SECURE_SESSION_CALLBACK	cbSecureSessionCallback,
															  void							*pvAppArg) ;


#endif

#ifdef __cplusplus
}
#endif

/**
 * @brief Server is ready to accept the post data.
 */
#define HTTP_CONTINUE							100

/**
 * @brief Successfully processed the given request.
 */
#define HTTP_OK									200

/**
 * @brief Successfully created a file on the server with the given content.
 */
#define HTTP_CREATED							201

/** 
 * @brief The given request is accepted by the server for proessing.
 */
#define HTTP_ACCEPTED							202

/**
 * @brief The metainformation in the entity-header is not from the origin server.
 */
#define HTTP_NON_AUTHORITATIVE					203

/**
 * @brief The given request is processed but there is no message body in the response.
 */
#define HTTP_NO_CONTENT							204

/**
 * @brief The user agent should update the document view.
 */
#define HTTP_RESET_CONTENT						205

/**
 * @brief The given range request is processed successfully.
 */
#define HTTP_PARTIAL_CONTENT					206

/**
 * @brief There are different representations of the given request in different locations.
 */
#define HTTP_MULTIPLE_CHOICES					300

/**
 * @brief The requested URL is moved permanently to the given location.
 */
#define HTTP_MOVED_PERMANENTLY					301

/**
 * @brief The requested URL is moved temporarily to the given location.
 */
#define HTTP_MOVED_TEMPORARILY					302

/** 
 * @brief The response for the given request can be fetched from the given location.
 */
#define HTTP_SEE_OTHER							303

/**
 * @brief The requested data is not midified since the given time.
 */
#define HTTP_NOT_MODIFIED						304

/**
 * @brief The request has to be accessed through the given proxy.
 */
#define HTTP_USE_PROXY							305

/**
 * @brief The requested URL is moved temporarily to the given location.
 */
#define HTTP_TEMPORARY_REDIRECT					307

/**
 * @brief The request formation by the client is invalid.
 */
#define HTTP_BAD_REQUEST						400

/**
 * @brief Authorization is required to access the requested URL.
 */
#define HTTP_UNAUTHORIZED						401

/**
 * @brief Reserved for future use.
 */
#define HTTP_PAYMENT							402

/**
 * @brief The server understood the request but it is not willing to fulfill it.
 */
#define HTTP_FORBIDDEN							403

/**
 * @brief The requested URL is not found on the server.
 */
#define HTTP_NOT_FOUND							404

/**
 * @brief The server cannot apply the given HTTP method on the requested resource.
 */
#define HTTP_METHOD_NOT_ALLOWED					405

/**
 * @brief The response is of type not supported by the client.
 */
#define HTTP_NOT_ACCEPTABLE						406

/**
 * @brief Client has to authenticate itself with the Proxy to access the given resource.
 */
#define HTTP_PROXY_AUTHENTICATION_REQD			407

/**
 * @brief The client did not produce a request within the time that 
 *		  the server was prepared to wait.
 */
#define HTTP_REQUEST_TIMEOUT					408

/**
 * @brief The request could not be completed due to a conflict with 
 *		  the current state of the resource.
 */
#define HTTP_CONFLICT							409

/**
 * @brief The requested resource is no longer available at the server.
 */
#define HTTP_GONE								410

/**
 * @brief The server is expecting a content-length header field in the request.
 */
#define HTTP_LENGTH_REQD						411

/**
 * @brief The precondition given in the request is failed.
 */
#define HTTP_PRECONDITION_FAILED				412

/**
 * @brief The response for the requested entity exceeded the limit on the server.
 */
#define HTTP_REQUEST_ENTITY_TOO_LARGE			413

/**
 * @brief The length of the requested URL is too large for the server to process.
 */
#define HTTP_REQUEST_URI_TOO_LARGE				414

/**
 * @brief The entity of the request is not supported by the 
 *		  requested resource for the requested HTTP method.
 */
#define HTTP_UNSUPPORTED_MEDIA_TYPE				415

/**
 * @brief The given range request cannot be fulfilled by the server.
 */
#define HTTP_REQUEST_RANGE_NOT_SATISFIABLE		416

/**
 * @brief The expectation given in an Expect request-header field 
 *		  could not be met by the server.
 */
#define HTTP_EXPECTATION_FAILED					417

/**
 * @brief The server encountered an unexpected condition which prevented it 
 *		  from fulfilling the request.
 */
#define HTTP_INTERNAL_SERVER_ERROR				500

/**
 * @brief The server does not support the functionality required to fulfill the request.
 */
#define HTTP_NOT_IMPLEMENTED					501

/**
 * @brief The server, while acting as a gateway or proxy, received an invalid response 
 *		  from the upstream server it accessed in attempting to fulfill the request.
 */
#define HTTP_BAD_GATEWAY						502

/**
 * @brief The server is currently unable to handle the request due to a
 *		  temporary overloading or maintenance of the server.
 */
#define HTTP_SERVICE_UNAVAILABLE				503

/**
 * @brief The server, while acting as a gateway or proxy, did not receive a
 *		  timely response from the upstream server .
 */
#define HTTP_GATEWAY_TIMEOUT					504

/**
 * @brief The server does not support, or refuses to support, the HTTP protocol
 *		  version that was used in the request message.
 */
#define HTTP_BAD_VERSION						505

#ifdef IMODE_HTTP_EX_STATUS_CODE
/** HTTP Status codes for mail operations **/

/**
 * @brief It indicates that mail Get is completed and there are no accumulated mails in the server.
 */
#define HTTP_MAIL_FINISHED						251

/**
 * @brief It shows the server received the mail sent from the UE correctly.
 */
#define HTTP_MAIL_RECEIVED						252

/**
 * @brief With the response of "X-EID: 00000" from the UE, this code indicates 
 *		  there are no accumulated mails in the server.
 */
#define HTTP_MAIL_EMPTY							255

/**
 * @brief It indicates that the server could not receive the mail, because the 
 *		  corresponding mail address did not exist in the same domain as that of the user.
 */
#define HTTP_MAIL_ID_ERROR						451

/**
 * @brief It indicates that the size of the video (MP4) file for 
 *		  streaming play received in the i-mode server has exceeded 2MB.
 */
#define HTTP_MAX_SIZE_ERROR						452

/**
 * @brief It indicates the mail box for the destination in 
 *		  the server is full, and the mail was not accepted.
 */
#define HTTP_MAIL_INBOX_FULL					551

/**
 * @brief It indicates the server could not receive the Mail Sent Request 
 *		  sent from the UE due to server error.
 */
#define HTTP_MAIL_POST_ERROR					552

/**
 * @brief It indicates the server could not receive the Mail Text Request 
 *		  sent from the UE due to server error.
 */
#define HTTP_MAIL_GET_ERROR						553

/**
 * @brief It indicates the server could not receive the Request as the 
 *		  service order to the server was not completed.
 */
#define HTTP_MAIL_UNORDERED						554

/**
 * @brief It indicates that undelivered mail exists, due to a destination
 *		  error or other reasons, in the case of broadcast mail.
 */
#define HTTP_MAIL_UE_POST_ERROR					561

#endif


#endif /* _JDI_HTTP_H */


