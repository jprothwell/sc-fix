/***************************************************************************
 * $Id: jdi_wsp.h,v 1.7 2009/06/02 04:44:44 browserdevjtbase Exp $
 * $Revision: 1.7 $
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
 * $Log: jdi_wsp.h,v $
 * Revision 1.7  2009/06/02 04:44:44  browserdevjtbase
 * Core Update
 *
 * Revision 1.37  2009/05/13 12:34:08  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.36  2007/10/29 05:28:36  kumardevhtmlbrow
 * Provided the extended error code when the handshake is successful.
 *
 * Revision 1.35  2007/04/09 06:33:42  shreyasdevhtmlbrow
 * Updating handling of Push based on socket events
 *
 * Revision 1.34  2006/11/03 14:12:02  kumardevhtmlbrow
 * Update
 *
 * Revision 1.33  2006/10/19 06:13:21  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.32  2006/09/26 11:39:46  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.31  2006/08/30 04:54:53  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.30  2006/06/29 04:43:41  kumardevhtmlbrow
 * Update ....
 *
 * Revision 1.29  2006/06/15 05:58:02  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.28  2006/04/28 07:36:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.27  2006/03/04 11:52:30  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.26  2006/03/04 07:51:15  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.25  2006/03/04 05:48:35  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.24  2006/03/02 12:30:19  kumardevhtmlbrow
 * Transport updated for handling Data connection error code and warning fix with different profile
 *
 * Revision 1.23  2006/03/01 14:57:58  kumardevhtmlbrow
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
 * The Jataayu WAP Stack API is a simple but powerful means for application developers to build 
 * WAP client applications. These applications could be complete browsers or simpler ones like 
 * server monitors or data probes. The Jataayu WAP Stack API is available in both ANSI C and 
 * Java versions. The ANSI C version is available on a variety of platforms including desktop 
 * and embedded operating systems and can be ported to most environments supporting ANSI C. 
 * This help file describes the ANSI C version of the Jataayu WAP Stack API.
 * 
 * @section Interfaces Stack Interfaces
 * The stack has three types of interfaces Session creation method, Request methods, Miscellaneous methods
 * @li The session creation method sets up the WAP Stack to start communicating. 
 *	   It allocates and initializes local data structures in preparation for the actual 
 *     communication. The differentiation between connectionless and connection oriented 
 *	   sessions is made during session creation time.
 * @li There are miscellaneous methods where the WAP Stack parameters are set. There are a large
 *	   number of WAP Stack parameters that can be controlled by the application. These include 
 *     USER_AGENT information, timeout and retries, buffer sizes, capabilities and gateway parameters.
 * @li The application can call one or more request methods once the session is created. Request
 *     methods are present to send requests in both connection oriented and connectionless modes
 *     as well as to abort requests. Asynchronous mode of communication is also supported.
 * @li Data received from the server is made available to the application by the WAP Stack in the
 *	   form of a buffer
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
 * The Jataayu WAP Stack has built based on the Jataayu Client Framework (JCF), which abstract the 
 * platform specific API calls. The platform needs to implement the API's of the required abstraction 
 * layers. WAPStack uses the following abstraction layers:
 * @li Memory 
 * @li File
 * @li Network
 * @li Timer
 * @li Log
 * @li Misc
 * 
 * 
 * 
 * @defgroup WAPStack WAPStack Interface Layer
 * @defgroup Memory Memory Abstraction Layer
 * @defgroup File File Abstraction Layer
 * @defgroup Network Network Abstraction Layer
 * @defgroup Timer Timer Abstraction Layer
 * @defgroup Log Log Abstraction Layer
 * @defgroup MISC Miscellaneous Abstraction Layer 
 */

/**
 * @file jdi_wsp.h
 * @ingroup WAPStack
 * @brief Defines the the functions which are exposed to use the WSP functions
 *
 * This file defines the various functions which are exposed for using the WSP functions.
 * In this document this layer will be addressed as a wap stack.  
 */

#ifndef _JDI_WSP_H
#define _JDI_WSP_H

#include <jdi_transportmodule.h>
#include <jdi_stackheader.h>
#include <jdi_wspdatatype.h>

#ifdef CERTICOM_WTLS
#include <jdi_wspwtls.h>
#endif

#ifdef JCF_WTLS
#include <jdi_wtls.h> 
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief If the application requests the wap stack to receive a push message and if the wap stack finds that 
 *		  there is no message to be received in the wap connection then this error is returned.
 */
#define JC_ERR_WSP_NO_PUSH_MSG_RECEIVED			TERMINAL_ERR_CODE(WSP_MODULE, 0x01)
/**
 * @brief If the application passes an invalid WSP handle to any of the wap stack functions then this 
 *		  error is returned.
 */
#define JC_ERR_WSP_BAD_CONNECTION_HANDLE		TERMINAL_ERR_CODE(WSP_MODULE, 0x02)
/**
 * @brief If the gateway address (to which the WSP connect has to be done) is invalid then wap stack 
 *		  returns this error.
 */
#define JC_ERR_WSP_INVALID_GATEWAY				TERMINAL_ERR_CODE(WSP_MODULE, 0x03)
/**
 * @brief If the bearer specified by the application is not supported by the wap stack then this error is
 *		  is returned.
 */
#define JC_ERR_WSP_UNSUPPORTED_BEARER			TERMINAL_ERR_CODE(WSP_MODULE, 0x04)
/**
 * @brief If the WSP send operation is unsuccessful then this error is returned.
 */
#define JC_ERR_WSP_SEND_ERROR					TERMINAL_ERR_CODE(WSP_MODULE, 0x05)
/**
 * @brief If the WSP receive operation is unsuccessful then this error is returned.
 */
#define JC_ERR_WSP_RECV_ERROR					TERMINAL_ERR_CODE(WSP_MODULE, 0x06)
/**
 * @brief If a WSP queue transaction fails then this error is returned. 
 */
#define JC_ERR_WSP_QUEUE_ERROR					TERMINAL_ERR_CODE(WSP_MODULE, 0x07)
/**
 * @brief If the content to be sent is more than the maximum size allowed by the server
 *		  then the wap stack returns this error 
 */
#define JC_ERR_WSP_SIZE_ERROR					TERMINAL_ERR_CODE(WSP_MODULE, 0x08)
/**
 * @brief For a WSP request if no response is sent from the server for a specific time 
 *		  period then this error is returned.
 */
#define JC_ERR_WSP_TIMED_OUT					TERMINAL_ERR_CODE(WSP_MODULE, 0x09)
/**
 * @brief For a any request (send, receive etc) if the wap stack has no data to pass to the application
 *		  then this error is returned.
 */
#define JC_ERR_WSP_NO_DATA						TERMINAL_ERR_CODE(WSP_MODULE, 0x0A)
/**
 * @brief The wap stack returns this error to indicate to the application that more data is to be
 *		  received from the server.
 */
#define JC_ERR_WSP_MORE_DATA					TERMINAL_ERR_CODE(WSP_MODULE, 0x0B)
/**
 * @brief If the wap stack needs more data from the application for the send operation then this 
 *		  error is returned.
 */
#define JC_ERR_WSP_MORE_DATA_TO_SEND			TERMINAL_ERR_CODE(WSP_MODULE, 0x0C)
/**
 * @brief This error is returned if the WSP request is redirected.
 */
#define JC_ERR_WSP_WAP_REDIRECT					TERMINAL_ERR_CODE(WSP_MODULE, 0x0D)
/**
 * @brief If the size of the application buffer, to which the server reply is to be copied, 
 *		  is insufficient then this error is returned.
 */
#define JC_ERR_WSP_INSUFFICIENT_BUFFER			TERMINAL_ERR_CODE(WSP_MODULE, 0x0E)
/**
 * @brief This error code is used internally to the wap stack.
 */
#define JC_ERR_WSP_ENCODE_VERSION				TERMINAL_ERR_CODE(WSP_MODULE, 0x0F)
/**
 * @brief The wap stack returns this error if a protocol error occurs.
 */
#define JC_ERR_WSP_PROTO_ERROR					TERMINAL_ERR_CODE(WSP_MODULE, 0x10)
/**
 * @brief The content received from the server will be sent to the application callback (registered
 *		  at WSP initialize). The wap stack returns this error in case the application callback has
 *		  returned an error.
 */
#define JC_ERR_WSP_CALLBACK_HANDLE_ERROR		TERMINAL_ERR_CODE(WSP_MODULE, 0x11)
/**
 * @brief This error specifies that the WSP request is aborted because of a protocol error.
 */
#define JC_ERR_WSP_ABORT_PROTOERR				TERMINAL_ERR_CODE(WSP_MODULE, 0x12)
/**
 * @brief This error specifies that the WSP request is aborted because a disconnect request
 *		  was given to the wap stack.
 */
#define JC_ERR_WSP_ABORT_DISCONNECT				TERMINAL_ERR_CODE(WSP_MODULE, 0x13)
/**
 * @brief 
 */
#define JC_ERR_WSP_ABORT_SUSPEND				TERMINAL_ERR_CODE(WSP_MODULE, 0x14)
/**
 * @brief 
 */
#define JC_ERR_WSP_ABORT_RESUME					TERMINAL_ERR_CODE(WSP_MODULE, 0x15)
/**
 * @brief This error specifies that the WSP request is aborted because of network congestion. 
 */
#define JC_ERR_WSP_ABORT_CONGESTION				TERMINAL_ERR_CODE(WSP_MODULE, 0x16)
/**
 * @brief This error specifies that the WSP request is aborted because of connect error.
 */
#define JC_ERR_WSP_ABORT_CONNECTERR				TERMINAL_ERR_CODE(WSP_MODULE, 0x17)
/**
 * @brief This error sepcifies that the WSP request is aborted because the MRU size has exceeded.
 */
#define JC_ERR_WSP_ABORT_MRUEXCEEDED			TERMINAL_ERR_CODE(WSP_MODULE, 0x18)
/**
 * @brief This error specifies that the WSP Request is aborted becasue the number of requests
 *	      have exceeded the maximum outstanding requests that are allowed.
 */
#define JC_ERR_WSP_ABORT_MOREXCEEDED			TERMINAL_ERR_CODE(WSP_MODULE, 0x19)
/**
 * @brief This error specifies that the WSP request is abored because of a peer request.
 */
#define JC_ERR_WSP_ABORT_PEERREQ				TERMINAL_ERR_CODE(WSP_MODULE, 0x1A)
/**
 * @brief This error specifies that the WSP request is aborted because of a network error.
 */
#define JC_ERR_WSP_ABORT_NETERR					TERMINAL_ERR_CODE(WSP_MODULE, 0x1B)
/**
 * @brief This error specifies that the user has requested to abort the WSP request.
 */
#define JC_ERR_WSP_ABORT_USERREQ				TERMINAL_ERR_CODE(WSP_MODULE, 0x1C)
/**
 * @brief 
 */
#define JC_ERR_WSP_ABORT_USERRFS				TERMINAL_ERR_CODE(WSP_MODULE, 0x1D)
/**
 * @brief 
 */
#define JC_ERR_WSP_ABORT_USERPND				TERMINAL_ERR_CODE(WSP_MODULE, 0x1E)
/**
 * @brief 
 */
#define JC_ERR_WSP_ABORT_USERDCR				TERMINAL_ERR_CODE(WSP_MODULE, 0x1F)
/**
 * @brief 
 */
#define JC_ERR_WSP_ABORT_USERDCU				TERMINAL_ERR_CODE(WSP_MODULE, 0x20)
/**
 * @brief This error specifies that the WSP request is aborted because of a unknown error.
 */
#define JC_ERR_WSP_ABORT_UNKNOWN				TERMINAL_ERR_CODE(WSP_MODULE, 0x21)
/**
 * @brief This error specifies that the WSP request is aborted because an invalid TID value is 
 *		  encountered.
 */
#define JC_ERR_WSP_ABORT_INVALID_TID			TERMINAL_ERR_CODE(WSP_MODULE, 0x22)
/**
 * @brief This error specifies that the WSP request is aborted because class 2 layer of security
 *		  is not supported.
 */
#define JC_ERR_WSP_ABORT_CLASS_2_NOT_SUPPORTED	TERMINAL_ERR_CODE(WSP_MODULE, 0x23)
/**
 * @brief This error specifies that the WSP request is aborted because SAR is not supported.
 */
#define JC_ERR_WSP_ABORT_SAR_NOT_SUPPORTED		TERMINAL_ERR_CODE(WSP_MODULE, 0x24)
/**
 * @brief This error specifies that the WSP request is aborted because user acknowledgement is not supported.
 */
#define JC_ERR_WSP_ABORT_USER_ACK_NOT_SUPPORTED	TERMINAL_ERR_CODE(WSP_MODULE, 0x25)
/**
 * @brief This error specifies that the WSP request is aborted because of a version mismatch.
 */
#define JC_ERR_WSP_ABORT_VERSION_NOT_MATCHED	TERMINAL_ERR_CODE(WSP_MODULE, 0x26)
/**
 * @brief This error specifies that the WSP request is aborted because the wap stack is overloaded.
 */
#define JC_ERR_WSP_ABORT_OVERLOADED				TERMINAL_ERR_CODE(WSP_MODULE, 0x27)
/**
 * @brief This error specifies that the WSP request is aborted because the wap stack has 
 *		  got no response from the server end.
 */
#define JC_ERR_WSP_ABORT_NO_RESPONSE			TERMINAL_ERR_CODE(WSP_MODULE, 0x28)
/**
 * @brief This error specifies that the WSP request is aborted because the Message that is processed
 *		  is too large.
 */
#define JC_ERR_WSP_ABORT_MSG_TOO_LARGE			TERMINAL_ERR_CODE(WSP_MODULE, 0x29)
/**
 * @brief This error specifies that the WSP request is aborted because ESAR is not supported.
 */
#define JC_ERR_WSP_ABORT_ESAR_NOT_SUPPORTED		TERMINAL_ERR_CODE(WSP_MODULE, 0x2A)
/**
 * @brief 
 */
#define JC_ERR_WSP_ABORT_TRN					TERMINAL_ERR_CODE(WSP_MODULE, 0x2B)
/**
 * @brief This error specifies that the WSP option which is to be set is already set in the
 *		  wap stack
 */
#define	JC_ERR_WSP_OPT_ALREADY_SET				TERMINAL_ERR_CODE(WSP_MODULE, 0x2C)
/**
 * @brief This error specifies that the secured WSP operation could not be performed as the 
 *		  security libs are not avialable.
 */
#define JC_ERR_WSP_SECURITY_LIB_NOT_PRESENT		TERMINAL_ERR_CODE(WSP_MODULE, 0x2D)
/**
 * @brief This error specifies that the wap stack has requested the application for the post data
 *		  and is waiting for the application response.
 */
#define JC_ERR_WSP_AWAITING_USER_REQUEST		TERMINAL_ERR_CODE(WSP_MODULE, 0x2E)
/**
 * @brief If the WTLS security is not provided by the wap stack then this error will be returned.
 */
#define JC_ERR_WSP_WTLS_UNDEFINED				TERMINAL_ERR_CODE(WSP_MODULE, 0x2F)

/**
 * @brief The wap stack returns this error if the handshaking is done sucessfully.
 */
#define JC_ERR_WTLS_HANDSHAKING_DONE			TERMINAL_ERR_CODE(WSP_MODULE, 0x30)
/**
 * @brief The wap stack returns this error if the WTLS is not defined and a WTLS request
 *		  is sent by the application.
 */
#define JC_ERR_WTLS_UNDEFINED					TERMINAL_ERR_CODE(WSP_MODULE, 0x31)

/**
 * @brief The wap stack returns this error if the transaction id is not valid. 
 */
#define JC_ERR_WSP_INVALID_TRANSACTION			TERMINAL_ERR_CODE(WSP_MODULE, 0x32)

/**
 * @brief The wap stack is in invalid state. 
 */
#define JC_ERR_WSP_INVALID_STATE			TERMINAL_ERR_CODE(WSP_MODULE, 0x33)


/**
 * @brief	  The application needs to use this function to initialize the wap stack (WSP Session). 
 * @param[in]	pcServerAddress Specifies the server address
 * @param[in]	uiPort Specifies the port number.
 * @param[in]	bSecurity Specifies whether secured WSP Session is to be established.
 * @param[in]	uiConnID Specifies the ID of the data connection used.
 * @param[out]	pvHandle The initialize function returns this handle. The application needs to use
 *				this handle when it uses any wap stack functions.
 * @return		JC_RETCODE, Function returns JC_OK if the wap stack is initialized sucessfully, 
				else returns the specific error code.
 * @note		The parameters which should not be NULL are pvHandle.
 * @see			jdi_WSPDeInitialize
 *
 * The application needs to use this function to initialize the WAP stack (WSP Session). The function will return
 * the WSP handle to the application. The application needs to use this handle for the wap stack functions
 */

JC_RETCODE jdi_WSPInitialize (const JC_INT8				*pcServerAddress,
							  JC_INT32					uiPort,
							  JC_BOOLEAN				bSecurity, 
							  JC_UINT32					uiConnID,
							  JC_HANDLE					*pvHandle) ;

/**
 * @brief	Sets the IP Address of the hostname, if the platform posts the 
 */
JC_RETCODE jdi_WSPSetHostName (JC_HANDLE				wsp_info_handle, 
							   JC_INT8					*pcHostName, 
							   JC_UINT8					*pucAddr) ;

/**
 * @brief	  The application needs to use this function to set various WSP parameters such as
 *			  timeout value, maximum retries etc.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	uiParameterID Specifies the ID of the parameter to be changed
 * @param[in]	uiValue Specifies the new value of the WSP parameter.
 * @return		JC_RETCODE, Function returns JC_OK if the WSP Options are set, 
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle.
 *
 * There are some WSP parameters which the application can set. Such as the timeout value, maximum
 * retries and also to enable security, push etc. The uiParameterID is the ID of the WSP parameter
 * to be changed. Value will be the new value of the WSP paramter. 
 */

JC_RETCODE jdi_WSPSetOptions (JC_HANDLE					vWSPHandle,
							  JC_INT32					uiParameterID,
							  JC_INT32					uiValue) ;

/**
 * @brief	The application needs to use this function to get the extended information of a WSP error
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		JC_INT32 value specifying the specific WSP error.
 * @note		The parameter which should not be NULL is vWSPHandle.
 *
 * All the WSP function will return the error in the form of JC_RETCODE. If the application needs
 * the exact error code then it needs to use this function. For example it can be used to get the
 * exact security error.
 */

JC_INT32 jdi_WSPGetExtendedError (JC_HANDLE				vWSPHandle) ;

/**
 * @brief	The application needs to use this function to get the extended handshake error information.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		JC_INT32 value specifying the specific extended handshake error.
 * @note		The parameter which should not be NULL is vWSPHandle.
 *
 * All the WSP function will return the error in the form of JC_RETCODE. If the application needs
 * the exact error code then it needs to use this function. For example it can be used to get the
 * extended handshake error once handshaking is done successfully.
 */
JC_INT32 jdi_WSPGetHandshakeExtendedError (JC_HANDLE wsp_info_handle) ;

/**
 * @brief	The application needs to use this function to get the WSP connect status.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[out]	piStatus The function returns the WSP connect status in this parameter.
 * @return		JC_RETCODE, Function returns JC_OK if the WSP connect status is returned, 
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle, piStatus.
 *
 * The application needs to call this function to get the WSP connect status. Based on the connect
 * status the application can decide whether it wants to proceed with the connection.
 */

JC_RETCODE jdi_WSPGetStatus (JC_HANDLE					vWSPHandle,
							 JC_INT32					*piStatus) ;

/**
 * @brief	The application needs to use this function to set the request callback.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	cbRequestCallback Pointer to the application's request callback.
 * @param[in]	pvReference When the wap stack calls the request callback it will send 
 *				pvReference as the application arguement. 
 * @return		JC_RETCODE, Function returns JC_OK if request callback is set, 
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 * @see			JDI_STACK_REQUEST_CB
 *
 * The application needs to use this function to register the request callback. The wap stack will 
 * call this callback when it needs post data information. This scenario will be seen if the 
 * application specifies the post data length but not the post data in the WSP fetch request.
 * So when wap stack requires the post data it will call this callback and expects the application
 * to return the post data information.
 */

JC_RETCODE jdi_WSPSetRequestCallback (JC_HANDLE				vWSPHandle,
									  JDI_STACK_REQUEST_CB	cbRequestCallback,
									  void					*pvReference) ;

/**
 * @brief	The application needs to use this function to set the response callback.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	cbResponseCallback Pointer to the application's response callback.
 * @param[in]	pvReference When the wap stack calls the response callback it will send 
 *				pvReference as the application arguement. 
 * @return		JC_RETCODE, Function returns JC_OK if response callback is set, 
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 * @see			JDI_STACK_RESPONSE_CB
 *
 * The application needs to use this function to register the response callback. The wap stack will use
 * this callback to intimate the application about the content received from the server. 
 */

JC_RETCODE jdi_WSPSetResponseCallback (JC_HANDLE				vWSPHandle,
									   JDI_STACK_RESPONSE_CB	cbResponseCallback,
									   void						*pvReference) ;

/**
 * @brief	The application needs to use this function to set the client capability.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	pstClient Specifies the client capability information.
 * @return		JC_RETCODE, Function returns JC_OK if client capability is set, 
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle and pstClient
 *
 * The application needs to use this function to set the client capability. The Wap stack will 
 * store this information and all the WSP operations will be done based on the client's
 * capabilities.
 */

JC_RETCODE jdi_WSPSetClientCapability (JC_HANDLE				vWSPHandle,
									   const CLIENT_CAPABILITY	*pstClient) ;

/**
 * @brief	The application needs to use this function to get the client capability.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	pstCapability Specifies the client capability information.
 * @return		JC_RETCODE, Function returns JC_OK if client capability is returned sucessfully, 
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle and pstCapability
 *
 * The application needs to use this function to get the client capability. The application can
 * call this function to check its capability which was previously set.
 */

void jdi_WSPGetCapabilities (JC_HANDLE				vWSPHandle,
							 CLIENT_CAPABILITY		*pstCapability) ;

/**
 * @brief	The application needs to use this function to add a header data.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	pcHeaderName Specifies the header name
 * @param[in]	pcHeaderValue Specifies the header value
 * @return		JC_RETCODE, Function returns JC_OK if header is added sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle, pcHeaderName and pstHeaderValue
 * @see			jdi_WSPSetClientHeaders
 * @see			jdi_WSPFreeHeader
 *
 * The application needs to use this function to add a header data to the wap stack. The wap stack will
 * add this header information and will use it for the WSP operations.
 */

JC_RETCODE jdi_WSPAddHeader (JC_HANDLE					vWSPHandle,
							 const JC_INT8				*pcHeaderName,
							 const JC_INT8				*pcHeaderValue) ;

/**
 * @brief	The application needs to use this function form the client header.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		JC_RETCODE, Function returns JC_OK if client header is formed sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 *
 * The application can add many headers using the function jdi_WSPAddHeader. Once all the headers
 * are added the application needs to call this function. This function will form a single consolidated
 * header from all the client headers. This header will be used by the Wap stack while performing
 * the WSP operations.
 */

JC_RETCODE jdi_WSPSetClientHeaders (JC_HANDLE			vWSPHandle) ;

/**
 * @brief	The application needs to use this function to add the basic authorization information to the wap stack..
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	bIsProxy Specifies whether the information is for proxy authorization (gateway authorization) or not.
 * @param[in]	pcName Specifies the user name.
 * @param[in]	pcPassword Specifies the password.
 * @return		JC_RETCODE, Function returns JC_OK if basic authorization information is added to wap stack sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 *
 * The application needs to use this function to add the basic authorization information to the 
 * wap stack. When required the wap stack will pass the information to the server.
 */

JC_RETCODE jdi_WSPAddBasicAuthorization (JC_HANDLE		vWSPHandle,
										 JC_BOOLEAN		bIsProxy, 
										 const JC_INT8	*pcName,
										 const JC_INT8	*pcPassword) ;

/**
 * @brief	The application needs to use this function to add the digest authorization information to the wap stack.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	bIsProxy Specifies whether the information is for proxy authorization (gateway authorization) or not.
 * @param[in]	eRequestType Specifies the request type.
 * @param[in]	pucURL Specifies the URL for which the authorization is associated.
 * @param[in]	pstDigestAuth Specifies the digest authorization information.
 * @return		JC_RETCODE, Function returns JC_OK if digest authorization information is added to wap stack sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle and pstDigestAuth
 *
 * The application needs to use this function to add the digest authorization information to the 
 * wap stack. When required the wap stack will pass the information to the server.
 */

JC_RETCODE jdi_WSPAddDigestAuthorization (JC_HANDLE				vWSPHandle, 
										  JC_BOOLEAN			bIsProxy,
										  E_WSP_REQUEST_TYPE	eRequestType,
										  JC_UINT8				*pucURL,
										  WSP_DIGEST_AUTH		*pstDigestAuth) ;

/**
 * @brief	The application needs to use this function to free all the WSP headers that were added previously.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		JC_RETCODE, Function returns JC_OK if all the WSP headers are freed sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 *
 * The application needs to call this function to free all the WSP headers that were previously
 * added by using the function jdi_WSPAddHeader.
 */

JC_RETCODE jdi_WSPFreeHeader (JC_HANDLE vWSPHandle) ;

#ifdef WSP_SYNC_ENABLE

/**
 * @brief	The application needs to use this function in order to perform a blocking WSP connect.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		JC_RETCODE, Function returns JC_OK if WSP connect is done sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 * @note		This function can be used only if the flag WSP_SYNC_ENABLE is enabled.
 *
 * The application needs to use this function in order to perform a blocking WSP connect. i.e. the
 * function will return the control to the application afer receiving the response for the connect
 * from the server.
 */

JC_RETCODE jdi_WSPConnect (JC_HANDLE		vWSPHandle) ;

/**
 * @brief	The application needs to use this function in order to perform a blocking WSP send request.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	pstWSPRequest Specifies the WSP request to be sent to the server.
 * @return		JC_RETCODE, Function returns JC_OK if WSP send request is done sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle and pstWSPRequest.
 * @note		This function can be used only if the flag WSP_SYNC_ENABLE is enabled.
 *
 * The application needs to use this function in order to perform a blocking WSP send request. i.e. the
 * function will return the control to the application afer receiving the response for the send request
 * from the server. The response of the server will be returned in the WSPREQUEST structure.
 */

JC_RETCODE jdi_WSPSendRequest (JC_HANDLE	vWSPHandle,
							   WSPREQUEST	*pstWSPRequest) ;
#endif

/**
 * @brief	The application needs to use this function in order to perform a non-blocking WSP connect.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		JC_RETCODE, Function returns JC_OK if WSP connect is done sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 * @see			jdi_WSPConnectPoll
 *
 * The application needs to use this function in order to perform a non-blocking WSP connect. The function
 * will return the control to the application (with or without the server response). If the function
 * has returned without the server response, then later on the application should call the function
 * jdi_WSPConnectPoll to get the result of the connect request.  
 */

JC_RETCODE jdi_WSPConnectAsync (JC_HANDLE	vWSPHandle) ;

/**
 * @brief	The application needs to use this function to get the result of a WSP connect request.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		JC_RETCODE, Function returns JC_OK if result of WSP connect is received sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 *
 * When the function jdi_WSPConnectAsync if called for connect the wap stack may not return the
 * result of the connect immediately (since it is non-blocking). So at a later point of time the 
 * application needs to use this function to find the result of the connect. 
 */
JC_RETCODE jdi_WSPConnectPoll (JC_HANDLE	vWSPHandle) ;

/**
 * @brief	The application needs to use this function to check if the WSP connection 
 *			exists for the WSP Session.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		JC_BOOLEAN. Returns E_TRUE if a connection exists for the WSP Session else returns
 *				E_FALSE.
 * @note		The parameter which should not be NULL is vWSPHandle
 *
 * Before proceeding with any of the WSP functions if the application needs to find out if the
 * connection exists for the WSP Session then it needs to use the following function.
 */
JC_BOOLEAN jdi_WSPIsConnected (JC_HANDLE	vWSPHandle) ;
 
#ifdef WAPSTACK_QD_MOR

/**
 * @brief	The application needs to use this function to send a WSP request to the wap stack
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	eRequestType Specifies the request type.
 * @param[in]	pucRequestBuffer Specifies the request buffer
 * @param[in]	pucPostData Specifies the post data.
 * @param[in]	iRequestLen Specifies the length of the request buffer
 * @param[in]	iPostDataLen Specifies the post data length.
 * @param[out]	pvTransactionHandle If the send request is sucessful the application will create
 *				a separate transaction handle and return it in this field.
 * @return		JC_RETCODE, Function returns JC_OK if WSP send is performed sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle and pvTransactionHandle
 * @note		This function can be used only if the WAPSTACK_QD_MOR is enabled.
 *
 * The application needs to use this function to send a WSP request to the wap stack. This function
 * is asynchronous (non-blocking). If the wap stack is not able to return the result immediately
 * then the application should call the same function to get the result.
 */

JC_RETCODE jdi_WSPSendRequestAsync (JC_HANDLE			vWSPHandle,
									E_WSP_REQUEST_TYPE	eRequestType,
									const JC_UINT8		*pucRequestBuffer,
									JC_UINT8			*pucPostData,
									JC_INT32			iRequestLen,
									JC_INT32			iPostDataLen,
									JC_HANDLE			*pvTransactionHandle) ;

/**
 * @brief	The application needs to use this function to delete a transaction that was created 
 *			when a request was sent to the wap stack
 * @param[in]	vTransactionHandle Specifies the transaction handle which was returned by the jdi_WSPSendRequestAsync.
 * @return		void
 * @note		The parameter which should not be NULL is vTransactionHandle
 * @note		This function can be used only if the WAPSTACK_QD_MOR is enabled.
 *
 * The application needs to use this function to delete a transaction that was created 
 * when a request was sent to the wap stack
 */

void jdi_DeleteTransaction (JC_HANDLE					vTransactionHandle) ;
#else

/**
 * @brief	The application needs to use this function to send a WSP request to the wap stack
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	eRequestType Specifies the request type.
 * @param[in]	pucRequestBuffer Specifies the request buffer
 * @param[in]	pucPostData Specifies the post data.
 * @param[in]	iRequestLen Specifies the length of the request buffer
 * @param[in]	iPostDataLen Specifies the post data length.
 * @return		JC_RETCODE, Function returns JC_OK if WSP send is performed sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 *
 * The application needs to use this function to send a WSP request to the wap stack. This function
 * is asynchronous (non-blocking). If the wap stack is not able to return the result immediately
 * then the application should call the same function to get the result.
 */

JC_RETCODE jdi_WSPSendRequestAsync (JC_HANDLE			vWSPHandle,
									E_WSP_REQUEST_TYPE	eRequestType,
									const JC_UINT8		*pucRequestBuffer,
									JC_UINT8			*pucPostData,
									JC_INT32			iRequestLen,
									JC_INT32			iPostDataLen) ;
#endif

/**
 * @brief	The application needs to use this function to receive the content from the server.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[out]	ppucBuffer Specifies the content received from the server will be returned in this field. 
 * @param[out]	piBufferLen Specifies the length of the content received from the server.
 * @param[out]	puiStatus Specifies the status of the WSP request.
 * @param[out]	ppucContentType Specifies the content type of the content received.
 * @param[out]	ppucHeader Specifies the header information of the content received.
 * @param[out]	piHeaderLen Specifies the length of the header.
 * @return		JC_RETCODE, Function returns JC_OK if result of content is received sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle, ppucBuffer, piBufferLen,
				puiStatus, ppucContentType, ppucHeader, piHeaderLen.
 *
 * The application needs to call this function to receive the content from the server. This
 * is a non-blocking call (hence the name jdi_WSPPollRequest). If the function does not return
 * any content (and no server error) then the application needs to call the same function
 * at a later point of time.
 */

JC_RETCODE jdi_WSPPollRequest (JC_HANDLE				vWSPHandle,
							   JC_UINT8					**ppucBuffer,
							   JC_INT32					*piBufferLen,
							   JC_UINT8					*puiStatus,
							   JC_UINT8					**ppucContentType,
							   JC_UINT8					**ppucHeader,
							   JC_INT32					*piHeaderLen) ;

/**
 * @brief	The application needs to use this function to get the status of any WSP transaction (request).
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[out]	puiSentBytes Specifies the number of bytes sent from the wap stack to the server for the WSP transaction.
 * @param[out]	puiRecvBytes Specifies the number of bytes received from the server for the WSP transaction.
 * @return		JC_RETCODE, Function returns JC_OK if transaction status is returned sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle, puiSentBytes, puiRecvBytes
 *
 * The application can call this function to get the trasaction status of a WSP request. The application
 * will get the number of bytes sent and received for the request from this function.
 */

JC_RETCODE jdi_WSPGetTransactionStatus (JC_HANDLE		vWSPHandle,
										JC_UINT32		*puiSentBytes,
										JC_UINT32		*puiRecvBytes) ;

/**
 * @brief	The application needs to use this function to abort all the WSP transactions.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		JC_RETCODE, Function returns JC_OK if WSP client abort is performed sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 *
 * When the application calls this function the WSP will abort all the active WSP transactions.
 */

JC_RETCODE jdi_WSPClientAbort (JC_HANDLE				vWSPHandle) ;

/**
 * @brief	The application needs to use this function to disconnect the WSP connection used.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		JC_RETCODE, Function returns JC_OK if WSP connection is disconnected sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 *
 * The application needs to use this function to disconnect the WSP connection used.
 */

JC_RETCODE jdi_WSPDisconnect (JC_HANDLE					vWSPHandle) ;

/**
 * @brief	The application needs to use this function to suspend the WSP Session.
 * @param[in]	vWSPHandle Specifies the handle of the WSP session which is to be suspended. 
 * @param[out]	ppucBuffer Specifies the content which will be returned by the function and 
 *			    this content is to be used when the application resumes the WSP Session.
 * @param[out]	piBufferLen Specifies the length of the buffer.
 * @return		JC_RETCODE, Function returns JC_OK if WSP session is suspended sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle, ppucBuffer, piBufferLen
 * @see			jdi_WSPResume
 *
 * The application needs to use this function to suspend the WSP Session. The function will return
 * a buffer content to the application. This buffer has to be used when the WSP session is to be
 * resumed.
 */

JC_RETCODE jdi_WSPSuspend (JC_HANDLE					vWSPHandle,
						   JC_UINT8						**ppucBuffer,
						   JC_INT32						*piBufferLen) ;

/**
 * @brief	The application needs to use this function to resume a suspended WSP Session.
 * @param[in]	pucBuffer Specifies the buffer which was returned when the WSP session was suspended.
 * @param[in]	iBufferLen Specifies the length of the buffer.
 * @param[out]	pvWSPHandle The function will resume the WSP Session and return the handle
 *			    of the session in this field.
 * @return		JC_RETCODE, Function returns JC_OK if WSP session is resumed sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are pucBuffer, iBufferLen, pvWSPHandle
 * @see			jdi_WSPSuspend
 *
 * The application needs to use this function to suspend the WSP Session. The function will return
 * a buffer content to the application. This buffer has to be used when the WSP session is to be
 * resumed.
 */

JC_RETCODE jdi_WSPResume (JC_UINT8		*pucBuffer,
						  JC_INT32		iBufferLen,
						  JC_HANDLE		*pvWSPHandle) ;

/**
 * @brief	The application needs to use this function to deinitialize a WSP session
 * @param[in]	vWSPHandle Specfies the handle of the WSP session which is to be deinitialized
 * @return		JC_RETCODE, Function returns JC_OK if WSP session is deinitialized sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 *
 * The application needs to use this function to deinitialize a WSP session
 */

JC_RETCODE jdi_WSPDeInitialize (JC_HANDLE				vWSPHandle) ;

/**
 * @brief	The application needs to use this function to initialize the transaction state
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		void
 * @note		The parameter which should not be NULL is vWSPHandle
 *
 * The application needs to call this function to initialize the WSP transaction state. i.e. this
 * function resets all the WSP Session information. This should be called before the WSP connect
 * is called.
 */

void jdi_WSPInitTransactionState (JC_HANDLE				vWSPHandle) ;

/**
 * @brief	The application needs to use this function to find out if a particular socket is
 *			being used by a WSP Session or not.
 * @param[in]	vWSPHandle Specifies the handle of the WSP Session.
 * @param[in]	iSocketID Specifies the ID of the socket.
 * @return		E_TRUE if the WSP session is using the socket else E_FALSE
 * @note		The parameter which should not be NULL is vWSPHandle
 *
 * The application needs to use this function to find out if a particular socket is
 * being used by a WSP Session or not.
 */

JC_BOOLEAN jdi_WSPIsHandleForSocket (JC_HANDLE			vWSPHandle,
									 JC_INT32			iSocketID) ;

/**
 * @brief The application needs to invoke this function when there is any data available in 
 * the socket layer.
 * 
 * @param[in]	vWSPHandle Specifies the handle of the WSP Session.
 * @param[in]	iSocketId  Specifies the socket identifier,
 * @param[in]	pucReadBuffer Specifies the data when it is available from the socket layer as event.
 * @param[in]	uiDataLen Specifies the data size of the pucReadBuffer or the amount of data available at the socket layer.
 */
JC_RETCODE jdi_WSPReadData (JC_HANDLE			vWSPHandle,
							JC_INT32			iSocketId, 
							JC_UINT8			*pucReadBuffer,
							JC_UINT32			uiDataLen) ;

#ifdef WAP_WITH_WTLS

/**
 * @brief	The application needs to use this function to specify the set of cipher suite algorithms that
 *			it wants the WAP Stack to support.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	peCipherSuite Specifies the list of cipher suite algorithms 
 * @param[in]	uiCipherSuiteCount Specifies the number of cipher suite algorithms in peCipherSuite
 * @return		JC_RETCODE, Function returns JC_OK if cipher suites are set sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle and peCipherSuite
 * @note		This function can be used only if WAP_WITH_WTLS is enabled.
 *
 * The application needs to use this function to specify the set of cipher suite that
 * it wants the WAP Stack to support. The peCipherSuite will be a list of cipher suite
 * algorithms that the application wants the wap stack to support.
 */

JC_RETCODE jdi_WSPSetCipherSuites (JC_HANDLE				vWSPHandle,
								   E_WTLS_CIPHER_SUITE		*peCipherSuite,
								   JC_UINT32				uiCipherSuiteCount) ;

/**
 * @brief	The application needs to use this function to specify the set of key exchange algorithms
 *			that it wants the WAP Stack to support.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	peAlgorithms Specifies the list of key exchange algorithms
 * @param[in]	uiAlgsCount Specifies the number of key exchange algorithms present in peAlgorithms 
 * @return		JC_RETCODE, Function returns JC_OK if key exchange algorithms are set sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle and peAlgorithms
 * @note		This function can be used only if WAP_WITH_WTLS is enabled.
 *
 * The application needs to use this function to specify the set of key exchange algorithms that
 * it wants the WAP Stack to support. The peAlgorithms will be a list of key exchange algorithms 
 * that the application wants the wap stack to support.
 */

JC_RETCODE jdi_WSPSetKeyExAlgorithms (JC_HANDLE					vWSPHandle,
									  E_WTLS_KEY_EXCHANGE_ALG	*peAlgorithms,
									  JC_UINT32					uiAlgsCount) ;

/**
 * @brief	The application needs to use this function to set the WTLS class.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	eClassType Specifies the WTLS class.
 * @return		JC_RETCODE, Function returns JC_OK if WTLS class is set sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 * @note		This function can be used only if WAP_WITH_WTLS is enabled.
 *
 * Before establishing a secured connection the application needs to use this function to set the
 * WTLS class that it wants the wap stack to support.
 */

JC_RETCODE jdi_WSPSetWTLSClass (JC_HANDLE				vWSPHandle,
								E_WTLSClass				eClassType) ;

/**
 * @brief	The application needs to use this function to add an user certificate.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	pstClientCertInfo Specifies the user certificate information.
 * @return		JC_RETCODE, Function returns JC_OK if user certificate is added sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle and pstClientCertInfo
 * @note		This function can be used only if WAP_WITH_WTLS is enabled.
 *
 * The application needs to use this function to add an user certificate. The Wap stack will send
 * this certificate to the security layer and this certificate will be used when the secured
 * connection is being established.
 */

JC_RETCODE jdi_WSPAddUserCertificate (JC_HANDLE					vWSPHandle,
									  CLIENT_CERTIFICATE_INFO	*pstClientCertInfo) ;


/**
 * @brief	The application needs to use this function to add an root certificate.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	pucCertData Specifies the root certificate data.
 * @param[in]	uiCertLen Specifies the length of the root certificate data.
 * @return		JC_RETCODE, Function returns JC_OK if root certificate is added sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle and pucCertData
 * @note		This function can be used only if WAP_WITH_WTLS is enabled.
 *
 * The application needs to use this function to add an root certificate. The Wap stack will send
 * this certificate to the security layer and this certificate will be used when the secured
 * connection is being established.
 */

JC_RETCODE jdi_WSPAddRootCertificate (JC_HANDLE		vWSPHandle,
									  JC_UINT8		*pucCertData,
									  JC_UINT32		uiCertLen) ;

/**
 * @brief	The application needs to call this function to get the cipher suite which was negotiated 
 *			while establishing the secured connection.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	pNegotiatedCipherSuite Specifies the negotiated cipher suite.
 * @return		JC_RETCODE, Function returns JC_OK if negotiated cipher suite is returned sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle and pNegotiatedCipherSuite
 * @note		This function can be used only if WAP_WITH_WTLS is enabled.
 *
 * After establishing the secured connection if the application wants to know the exact cipher suite
 * that was negotiated between the client and server, then this function has to be used.
 */

JC_RETCODE jdi_WSPGetNegotiatedCipherSuites (JC_HANDLE				vWSPHandle, 
											 E_WTLS_CIPHER_SUITE	*pNegotiatedCipherSuite) ;

/**
 * @brief	The application needs to call this function to get the key exchange algorithm which was negotiated 
 *			while establishing the secured connection.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	pNegotiatedAlgorithm Specifies the negotiated key exchange algorithm.
 * @return		JC_RETCODE, Function returns JC_OK if negotiated key exchange algorithm is returned sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are vWSPHandle and pNegotiatedAlgorithm
 * @note		This function can be used only if WAP_WITH_WTLS is enabled.
 * 
 * After establishing the secured connection if the application wants to know the exact key exchange algorithm
 * that was negotiated between the client and server, then this function has to be used.
 */

JC_RETCODE jdi_WSPGetNegotiatedAlgorithm (JC_HANDLE					vWSPHandle, 
										  E_WTLS_KEY_EXCHANGE_ALG	*pNegotiatedAlgorithm) ;

/**
 * @brief	The application needs to call this function to register the certificate callback.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @param[in]	cbCertCallback Specifies the application's certificate callback
 * @param[in]	pvReference Specifies the application arguement.
 * @return		JC_RETCODE, Function returns JC_OK if certificaet callback is registered sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 * @note		This function can be used only if WAP_WITH_WTLS is enabled.
 * 
 * The application needs to call this function to register the certificate callback.The application
 * will call the certificate callback when any certificate is received from the security layer.
 */

JC_RETCODE jdi_WSPRegisterCertificateCallback (JC_HANDLE			vWSPHandle,
											  JL_WTLS_CERT_CALLBACK	cbCertCallback,
											  void					*pvReference) ;

/**
 * @brief	The application needs to call this function to delete the security context of the WSP Session.
 * @param[in]	vWSPHandle Specifies the WSP handle which was returned in the WSP initialize function.
 * @return		JC_RETCODE, Function returns JC_OK if security session is deleted sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPHandle
 * @note		This function can be used only if WAP_WITH_WTLS is enabled.
 * 
 * The application needs to call this function to delete the security context that is currently used 
 * in the WSP Session. All the security parameter such as the cipher suite and key exchange algorithms
 * will be deleted.
 */

JC_RETCODE jdi_WSPDeleteSecuritySession (JC_HANDLE			vWSPHandle) ;

#endif /* WAP_WITH_WTLS */


#ifdef WAPSTACK_PUSH

/**
 * @brief	The application needs to call this function to initialize the WSP push session
 * @param[out]	pvWSPPushHandle The initialize function returns this handle. The application needs to use
 *				this handle when it uses any WSP push functions.
 * @param[in]	uiConnID Specifies the ID of the connection to be used for the push session.
 * @return		JC_RETCODE, Function returns JC_OK if WSP push session is created sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is pvWSPPushHandle
 * @note		This function can be used only if WAPSTACK_PUSH is enabled.
 * 
 * The application needs to call this function to initialize the WSP push session.
 */

JC_RETCODE jdi_WSPPushInitialize (JC_HANDLE			*pvWSPPushHandle,
								  JC_UINT32			uiConnID) ;

/**
 * @brief	The application needs to use this function to start the WSP push
 * @param[in]	vWSPPushHandle Specifies the WSP push handle which was returned in the WSP push initialize function.
 * @param[in]	pcGateway Specifies the gateway address
 * @param[in]	uhPort Specifies the port number
 * @param[in]	bSecurity Specifies whether secured connection is to be established
 * @param[in]	ePushType Specifies the push type
 * @param[in]	piRef The reference to the push session will be returned in this field.
 * @return		JC_RETCODE, Function returns JC_OK if WSP push is started sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPPushHandle
 * @note		This function can be used only if WAPSTACK_PUSH is enabled.
 * 
 * The application needs to use this function to start a WSP push session. This is done after
 * the WSP push session is initialzed. The function start the transactions for the push session.
 */

JC_RETCODE jdi_WSPStartPush (JC_HANDLE				vWSPPushHandle, 
							 const JC_INT8			*pcGateway,
							 JC_UINT16				uhPort,
							 JC_BOOLEAN				bSecurity,
							 EPushType				ePushType,
							 JC_INT32				*piRef) ;

/**
 * @brief	The application needs to use this function to find out if a particular socket is
 *			being used by a WSP push Session or not.
 * @param[in]	vWSPPushHandle Specifies the handle of the WSP push Session.
 * @param[in]	iSocketID Specifies the ID of the socket.
 * @return		E_TRUE if the WSP push session is using the socket else E_FALSE
 * @note		The parameter which should not be NULL is vWSPPushHandle
 * @note		This function can be used only if WAPSTACK_PUSH is enabled.
 *
 * The application needs to use this function to find out if a particular socket is
 * being used by a WSP push  Session or not.
 */

JC_BOOLEAN jdi_WSPIsPushSocket (JC_HANDLE			vWSPPushHandle,
								JC_INT32			iSocketID) ;

/**
 * @brief	The application needs to invoke this function in case any push data is available in the 
 *			socket or if the socket layer has already read the push data from the socket.
 * @param[in]	vWSPPushHandle Specifies the handle of the WSP push Session.
 * @param[in]	iSocketID Specifies the ID of the socket.
 * @param[in]	pucBuffer Specifies the push buffer which is already read from the socket (if not read then 
 *				this value should be NULL).
 * @param[in]	uiBufferLen Specifies length of the push buffer (length of pucBuffer if pucBuffer is not NULL
 *				else the length of the push data that is currently available in the socket).
 * @return		JC_OK if the push data is read successfully.
 * @note		The parameter which should not be NULL is vWSPPushHandle
 * @note		This function can be used only if WAPSTACK_PUSH is enabled.
 *
 * The application needs to use this function to read the push data from the socket or to pass
 * the push data that is already read by the application's socket layer. 
 */

JC_RETCODE jdi_WSPPushReadData (JC_HANDLE		vWSPPushHandle,
								JC_INT32		iSocketID,
								JC_UINT8		*pucBuffer,
								JC_UINT32		uiBufferLen) ;

/**
 * @brief	The application needs to use this function to request the Wap stack to receive push messages.
 * @param[in]	vWSPPushHandle Specifies the WSP push handle which was returned in the WSP push initialize function.
 * @param[in]	iRef Specifies the reference of the push session
 * @param[out]	ppstWSPPushMsg if the message is received then it will be returned in this field.
 * @return		JC_RETCODE, Function returns JC_OK if push message is received sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPPushHandle and ppstWSPPushMsg
 * @note		This function can be used only if WAPSTACK_PUSH is enabled.
 * 
 * When the application wants the wap stack to receive push message this function is to be called.
 * The wap stack will connect (if connection oriented push) to the gateway and check if any 
 * push message has arrived.
 */

JC_RETCODE jdi_RecvPushMessage (JC_HANDLE			vWSPPushHandle,
								JC_INT32			iRef,
								PUSH_MSG			**ppstWSPPushMsg) ;

/**
 * @brief	The application needs to use this function to parse a WSP push buffer.
 * @param[in]	pucBuffer Specifies the WSP push buffer
 * @param[in]	uiBufferLen Specifies the push buffer length.
 * @param[out]	ppstWSPPushMsg The push message is parsed and returned in this field.
 * @return		JC_RETCODE, Function returns JC_OK if push message is parsed sucessfully
				else returns the specific error code.
 * @note		The parameters which should not be NULL are pucBuffer and ppstWSPPushMsg
 * @note		This function can be used only if WAPSTACK_PUSH is enabled.
 * @see			jdi_WSPFreePushMsg
 * 
 * The application needs to use this function to parse a WSP push buffer. The function will parse
 * the buffer and convert in into the PUSH_MSG format.
 */

JC_RETCODE jdi_ParsePushMessage (JC_UINT8			*pucBuffer,
								 JC_UINT32			uiBufferLen,
								 PUSH_MSG			**ppstWSPPushMsg) ;

/**
 * @brief	The application needs to use this function to free a WSP push message.
 * @param[in]	pstPushMsg Specifies the WSP push messages to be freed.
 * @return		void
 * @note		The parameter which should not be NULL is pstPushMsg
 * @note		This function can be used only if WAPSTACK_PUSH is enabled.
 * 
 * The application needs to use this function to free a WSP push message which was previously formed
 * by the jdi_ParsePushMessage function.
 */

void jdi_WSPFreePushMsg (PUSH_MSG					*pstPushMsg) ;

/**
 * @brief	The application needs to use this function to stop the WSP push
 * @param[in]	vWSPPushHandle Specifies the WSP push handle which was returned in the WSP push initialize function.
 * @param[in]	iPort Specifies the port number
 * @return		JC_RETCODE, Function returns JC_OK if WSP push is stopped sucessfully
				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPPushHandle
 * @note		This function can be used only if WAPSTACK_PUSH is enabled.
 * 
 * The application needs to call this function to stop a WSP push session. The wap stack will
 * search for the WSP push Session which is using the iPort and stop that push session
 */

JC_RETCODE jdi_WSPStopPush (JC_HANDLE				vWSPPushHandle, 
							JC_UINT16				iPort) ;

/**
 * @brief	The application needs to call this function to deinitialize the WSP push session
 * @param[out]	vWSPPushHandle Specifies the WSP push handle which was returned in the WSP push initialize function.
 * @return		JC_RETCODE, Function returns JC_OK if WSP push session is deinitialized sucessfully
 *				else returns the specific error code.
 * @note		The parameter which should not be NULL is vWSPPushHandle.
 * @note	 	This function can be used only if WAPSTACK_PUSH is enabled.
 * 
 * The application needs to call this function to deinitialize the WSP push session.
 */

JC_RETCODE jdi_WSPPushDeinitialize (JC_HANDLE		vWSPPushHandle) ;

#endif


#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


