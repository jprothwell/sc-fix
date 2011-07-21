/***************************************************************************
 * $Id: jdi_transport.h,v 1.17 2009/05/29 12:00:18 browserdevjtbase Exp $
 * $Revision: 1.17 $
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
 * $Log: jdi_transport.h,v $
 * Revision 1.17  2009/05/29 12:00:18  browserdevjtbase
 * jMail Core Update
 *
 * Revision 1.16  2008/06/16 13:15:49  browserdevjtbase
 * E-Mail Core update.
 *
 * Revision 1.15  2008/03/17 11:42:00  browserdevjtbase
 * Core team update.
 *
 * Revision 1.14  2007/12/14 06:16:02  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.13  2007/09/27 14:52:23  browserdevjtbase
 * Core Update
 *
 * Revision 1.12  2007/06/26 12:43:33  browserdevjtbase
 * Updated
 *
 * Revision 1.11  2007/04/27 07:21:30  browserdevjtbase
 * Mail Update
 *
 * Revision 1.10  2007/04/10 15:31:01  browserdevjtbase
 * Update ...
 *
 * Revision 1.9  2007/04/03 07:13:39  browserdevjtbase
 * JCF Update
 *
 * Revision 1.8  2007/02/06 10:09:59  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.7  2006/11/29 11:51:16  browserdevjtbase
 * General Update
 *
 * Revision 1.6  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.5  2006/06/20 13:28:37  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.4  2006/06/16 06:58:58  browserdevjtbase
 * Updated with email protocol
 *
 * Revision 1.3  2006/06/14 06:40:15  browserdevjtbase
 * Update ...
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.28  2006/04/28 07:36:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.27  2006/03/17 13:32:54  kumardevhtmlbrow
 * WSP Optimization and code clean up
 *
 * Revision 1.26  2006/03/16 07:35:41  kumardevhtmlbrow
 * Added the application to add the session header
 *
 * Revision 1.25  2006/03/01 14:57:58  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.24  2006/01/09 14:45:52  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.23  2005/12/31 12:15:36  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.22  2005/12/31 06:46:08  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.21  2005/12/30 13:54:23  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.20  2005/12/12 15:29:02  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.19  2005/12/09 15:15:16  kumardevhtmlbrow
 * Added the suspend and resume request. Providing additional interface for starting confirmed & http push. Changes as per new JDD LOG abstraction
 *
 * Revision 1.18  2005/12/01 12:50:42  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.17  2005/10/27 15:55:20  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.16  2005/10/20 06:58:39  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.15  2005/09/27 15:35:45  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.14  2005/09/26 14:10:35  shreyasdevhtmlbrow
 * Transport Log Message Updated
 *
 * Revision 1.13  2005/09/22 16:13:43  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.12  2005/09/13 07:43:19  kumardevhtmlbrow
 * Updated the push for SMS & Unconfirmed Push
 *
 * Revision 1.11  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.10  2005/08/30 14:24:20  kumardevhtmlbrow
 * no message
 *
 * Revision 1.9  2005/08/01 06:23:03  kumardevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/07/22 14:31:03  kumardevhtmlbrow
 * no message
 *
 * Revision 1.7  2005/07/08 13:22:33  shreyasdevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/06/13 10:07:36  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/06/04 07:56:52  kumardevhtmlbrow
 * updated the transport task ...
 *
 * Revision 1.3  2005/05/30 13:03:21  kumardevhtmlbrow
 * Updated the event
 *
 * Revision 1.2  2005/05/21 11:37:48  shreyasdevhtmlbrow
 * no message
 *
 * Revision 1.1  2005/05/17 06:25:14  kumardevhtmlbrow
 * Added the transport interface files
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_transport.h
 * @ingroup Transport
 * @brief Defines the events, macros and exposed calls of transport. 
 *
 * The primary purpose is to provide an interface between the application and the lower layers. 
 * The transport waits for an event from the application and then processes it. The transport 
 * starts from handling the data connection and then sending request to lower layers. Then receving
 * response and seding the same to application. The transport also handles the various types of
 * push and send the push content to the registered application.
 */

#ifndef _JDI_TRANSPORT_H
#define _JDI_TRANSPORT_H

/** @cond */
/*** Macros					***/

#if	(!defined WSP_STACK_ENABLE && !defined HTTP_STACK_ENABLE) 
	#error "Atleast one of the stack option WSP_STACK_ENABLE or HTTP_STACK_ENABLE should be defined"
#endif

#if (!defined HTTP_STACK_ENABLE && (defined HTTP_PIPELINE_ENABLE || defined HTTP_WITH_SSL || defined CERTICOM_SSL || defined SSL_4_5))
	#error "HTTP_PIPELINE_ENABLE or HTTP_WITH_SSL or CERTICOM_SSL or SSL_4_5 is enabled without enabling HTTP_STACK_ENABLE."
#elif (!defined HTTP_WITH_SSL && (defined CERTICOM_SSL || defined SSL_4_5))
	#error "CERTICOM_SSL or SSL_4_5 is enabled without enabling HTTP_WITH_SSL"
#elif (!defined CERTICOM_SSL && defined SSL_4_5)
	#error "SSL_4_5 enabled without enabling CERTICOM_SSL"
#endif 

#if (!defined HTTP_DECODER_ENABLE && defined TR_HTTP_PUSH)
	#error "TR_HTTP_PUSH is enabled without enabling HTTP_DECODER_ENABLE"
#endif

#if (!defined WSP_STACK_ENABLE && (defined WAPSTACK_PUSH || defined WAP_WITH_WTLS || defined CERTICOM_WTLS || defined WTLS_2_4))
	#error "WAPSTACK_PUSH or WAP_WITH_WTLS or CERTICOM_WTLS or WTLS_2_4 is enabled without enabling WSP_STACK_ENABLE."
#elif (!defined WAP_WITH_WTLS && (defined CERTICOM_WTLS || defined WTLS_2_4))
	#error "CERTICOM_WTLS or WTLS_2_4 is enabled without enabling WAP_WITH_WTLS"
#elif (!defined CERTICOM_WTLS && defined WTLS_2_4)
	#error "WTLS_2_4 enabled without enabling CERTICOM_WTLS"
#elif (defined CERTICOM_WTLS && !defined WTLS_2_4)
	#error "CERTICOM_WTLS enabled without enabling WTLS_2_4"
#endif 

#if (!defined TR_PUSH && (defined TR_WAP_UNCONFIRMED_PUSH || defined TR_WAP_CONFIRMED_PUSH || defined TR_HTTP_PUSH || defined TR_WAP_GSM_REASSEMBLE))
	#error "TR_WAP_UNCONFIRMED_PUSH or TR_WAP_CONFIRMED_PUSH or TR_HTTP_PUSH or TR_WAP_GSM_REASSEMBLE enabled without enabling TR_PUSH"
#elif (!defined WAPSTACK_PUSH && (defined TR_WAP_UNCONFIRMED_PUSH_AT_STARTUP || defined TR_WAP_UNCONFIRMED_PUSH || defined TR_WAP_CONFIRMED_PUSH))
	#error "TR_WAP_UNCONFIRMED_PUSH_AT_STARTUP or TR_WAP_UNCONFIRMED_PUSH or TR_WAP_CONFIRMED_PUSH enabled without enabling WAPSTACK_PUSH"
#elif (!defined TR_WAP_UNCONFIRMED_PUSH && defined TR_WAP_UNCONFIRMED_PUSH_AT_STARTUP)
	#error "TR_WAP_UNCONFIRMED_PUSH_AT_STARTUP enabled without enabling TR_WAP_UNCONFIRMED_PUSH"
#endif 

#include <jdi_transportmodule.h>

#define TR_FETCH_TIMER_NAME				"trFetchTimer"
#define TR_PUSH_TIMER_NAME				"trPushTimer"
#define TR_MAIL_TIMER_NAME				"trMailTimer"

/**
 * @brief Specifies the maximum size of the phone number
 */ 
#define TR_MAX_PHONE_NUMBER_SIZE		255
/**
 * @brief Specifies the maximum size of APN
 */
#define TR_MAX_APN_SIZE					255
/**
 * @brief Specifies the maximum size of the username
 */
#define TR_MAX_USERNAME_SIZE			255
/**
 * @brief Specifies the maximum size of the password
 */
#define TR_MAX_PASSWORD_SIZE			255
/**
 * @brief Specifies the maximum size of the hostname
 */
#define TR_MAX_HOSTNAME_SIZE			255
/**
 * @brief Specifies the maximum size of the PPG host name
 */
#define TR_MAX_PPGHOST_SIZE				255
/**
 * @brief Specifies the maximum size of content type
 */
#define TR_MAX_CONTENT_TYPE_SIZE		255
/**
 * @brief Specifies the maximum size of the charset value
 */
#define TR_MAX_CHARSET_SIZE				255
/**
 * @brief Specifies the maximum size of the header name
 */
#define TR_MAX_HEADER_NAME_SIZE			255
/**
 * @brief Specifies the maximum size of the username used in authorization
 */
#define TR_MAX_AUTH_USERNAME_SIZE		255
/**
 * @brief Specifies the maximum size of the password used in authorization
 */
#define TR_MAX_AUTH_PASSWORD_SIZE		255
/**
 * @brief Specifies the maximum size of the domain name used in authorization
 */
#define TR_MAX_AUTH_DOMAIN_SIZE			255
/**
 * @brief Specifies the maximum size of the realm information used in authorization
 */
#define TR_MAX_AUTH_REALM_SIZE			255
/**
 * @brief Specifies the maximum size of the Nonce information used in authorization
 */
#define TR_MAX_AUTH_NONCE_SIZE			255
/**
 * @brief Specifies the maximum authentication opaque size.
 */
#define TR_MAX_AUTH_OPAQUE_SIZE			255

/**
 * @brief Specifies the maximum authentication qop size.
 */
#define TR_MAX_AUTH_QOP_SIZE				255

/**
 * @brief Specifies the maximum authentication algorithm size.
 */
#define TR_MAX_AUTH_ALG_SIZE				255

/**
 * @brief Specifies the transport module name
 */
#define TRANSPORT_MODULE_NAME			"transport"

/**
 * @brief The error code is returned in case of invalid data (from application).
 */
#define JC_ERR_TR_INVALID_DATA					TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x01)
/**
 * @brief The error code is returned in case invalid session ID is specified to transport.
 */
#define JC_ERR_TR_INVALID_SESSION				TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x02)
/**
 * @brief The error code is returned in case invalid request ID is specified to transport.
 */
#define JC_ERR_TR_INVALID_REQUEST				TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x03)
/**
 * @brief The error code is returned in case invalid connection mode is specified to transport
 */
#define JC_ERR_TR_INVALID_CONNECTION_MODE		TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x04)
/**
 * @brief The error code is returned in case invalid User data is passed to transport (i.e.
 *		  for example sending more user data than expected)
 */
#define JC_ERR_TR_WSP_INVALID_USER_DATA			TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x05)
/**
 * @brief The error code is returned when WSP stack has sucessfully processed the user data request.
 */
#define JC_ERR_TR_WSP_PROCESSED					TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x06)
/**
 * @brief The error code is returned when WSP stack is busy processing other request(s).
 */
#define JC_ERR_TR_WSP_IN_PROCESS				TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x07)
/**
 * @brief The error code is returned when HTTP stack is busy processing other request(s).
 */
#define JC_ERR_TR_WPHTTP_IN_PROCESS				TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x08)
/**
 * @brief The error code is returned in case invalid push ID is specified (by application) to transport.
 */
#define JC_ERR_TR_INVALID_PUSH_ID				TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x09)
/**
 * @brief The error code is returned when the application has given a SMS handle request but
 *		  the sms session is not initialized.
 */
#define JC_ERR_TR_SMS_NOT_INITIALIZED			TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x0A)
/**
 * @brief When the Data connection layer sends disconnected GPRS to the transport this error
 *		  is passed to the application.
 */
#define JC_ERR_TR_DATA_CONN_DISCONNECTED_GPRS	TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x0B)
/**
 * @brief When the Data connection layer sends disconnected CSD to the transport this error
 *		  is passed to the application.
 */
#define JC_ERR_TR_DATA_CONN_DISCONNECTED_CSD	TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x0C)
/**
 * @brief When the Data connection layer sends disconnected indication for other bearer type
 *		  to the transport this error is passed to the application.
 */
#define JC_ERR_TR_DATA_CONN_DISCONNECTED_OTHER	TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x0D)
/**
 * @brief When the Data connection layer sends failed GPRS to the transport this error
 *		  is passed to the application.
 */
#define JC_ERR_TR_DATA_CONN_FAIL_GPRS			TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x0E)
/**
 * @brief When the Data connection layer sends failed CSD to the transport this error
 *		  is passed to the application.
 */
#define JC_ERR_TR_DATA_CONN_FAIL_CSD			TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x0F)
/**
 * @brief When the Data connection layer sends failed indication for other bearer type
 *		  to the transport this error is passed to the application.
 */
#define JC_ERR_TR_DATA_CONN_FAIL_OTHER			TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x10)
/**
 * @brief If the transport is unable to set up a secure data connection then this error is
 *		  returned to the application.
 */
#define JC_ERR_TR_SECURE_CONNECT_FAILED			TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x11)
/**
 * @brief This error is returned by the transport if the application tried to resume a 
 *		  request which is not suspended.
 */
#define JC_ERR_TR_REQUEST_NOT_SUSPENDED			TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x12)
/**
 * @brief This error is returned by the transport if the session to be suspended is invalid
 */
#define JC_ERR_TR_INVALID_SUSPEND_SESSION_REQ	TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x13)
/**
 * @brief This error is returned by the transport if the session to be resumed is invalid
 */
#define JC_ERR_TR_INVALID_RESUME_SESSION_REQ	TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x14)
/**
 * @brief This error is returned by the transport if the lower layer sends multiple headers.
 */
#define JC_ERR_TR_DUPLICATE_HEADER_RECEIVED		TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x15)
/**
 * @brief This error is returned by the transport if the stop all request is received but there
 *		  are currently no request present in the transport session
 */
#define JC_ERR_TR_NO_REQUEST_TO_STOP			TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x16)

#ifdef IP_STACK_ENABLE
/**
 * @brief This error is returned to transport from the application if all the content for the 
 *		  current IP request is received successfully.
 *		  
 */
#define JC_ERR_TR_IP_REQ_COMPLETED				TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x17)
/**
 * @brief This error is returned to application from the transport if an error has occured
 *		  while performing tunnel operation.
 */
#define JC_ERR_TR_IP_TUNNEL_ERR					TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x18)

#endif

#ifdef EMAIL_PROTOCOL_ENABLE 

#define JC_ERR_TR_SMTP_IN_PROCESS				TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x31)
#define JC_ERR_TR_POP3_IN_PROCESS				TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x32)
#define JC_ERR_TR_IMAP4_IN_PROCESS				TERMINAL_ERR_CODE(TRANSPORT_MODULE, 0x33)

#endif	/** EMAIL_PROTOCOL_ENABLE **/

#ifdef LOG_TRANSPORT_ENABLED
#define TRANSPORT_LOGMSG						jdd_LogMsg
#define TRANSPORT_LOGPRINTF						jdd_LogPrintf
#else
#define TRANSPORT_LOGMSG						
#define TRANSPORT_LOGPRINTF						
#endif

/*** Type Definitions		***/

/**
 * @brief This enum specifies the events handled by the transport and the events sent from transport
 *		  to the application.
 */

typedef enum tr_event_type
{
	E_TR_INITIALIZE_REQ_EVENT = 900, /**< Event sent from application to initialize the transport session (Argument Passed #ST_TR_INITIALIZE_REQ) */
	E_TR_INITIALIZE_RESP_EVENT, /**< Event used by transport to send the Initialize response to application (Argument Passed #ST_TR_INITIALIZE_RESP)*/
	E_TR_DEINITIALIZE_REQ_EVENT, /**< Event sent from application to Deinitialize the transport session (Argument Passed #ST_TR_DEINITIALIZE_REQ)*/
	E_TR_DEINITIALIZE_RESP_EVENT, /**< Event used by transport to send the Deinitialize response to application (Argument Passed #ST_TR_DEINITIALIZE_RESP)*/
	E_TR_DISCONNECT_REQ_EVENT, /**< Event sent from application to disconnect the data connection used by the transport session (Argument Passed #ST_TR_DISCONNECT_REQ) */
	E_TR_DISCONNECT_RESP_EVENT, /**<  Event used by transport to send the Disconnect response to application (Argument Passed #ST_TR_DISCONNECT_RESP)*/
	E_TR_PROFILE_REQ_EVENT, /**< Event sent from application to specify the profile that is to be used by transport (Argument Passed #ST_TR_PROFILE_REQ) */
	E_TR_PROFILE_RESP_EVENT, /**< Event used by transport to send the profile response to application (Argument Passed #ST_TR_PROFILE_RESP) */
	E_TR_EXTENDED_PROFILE_REQ_EVENT, /**< Event sent from application to specify the extended profile that is to be used by transport (Argument Passed #ST_TR_EXTENDED_PROFILE_REQ) */
	E_TR_PROFILE_CONFIRMATION_REQ_EVENT, /**< Event sent from transport to get the confirmation for proceeding with other options of the extended profile (Argument Passed #ST_TR_PROFILE_CONFIRMATION_REQ) */
	E_TR_PROFILE_CONFIRMATION_RESP_EVENT, /**< Event sent from application to specify whether the transport can go ahead with other options of the extended profile (Argument Passed #ST_TR_PROFILE_CONFIRMATION_RESP) */
#ifdef IP_STACK_ENABLE
	E_TR_IP_PROFILE_REQ_EVENT, /**< Event sent from application to specify the IP (TCP) profile that is to be used by transport (Argument Passed #ST_TR_IP_PROFILE_REQ) */
	E_TR_IP_PROFILE_RESP_EVENT, /**< Event used by transport to send the profile response to application (Argument Passed #ST_TR_PROFILE_RESP) */
	E_TR_IP_SEND_REQ_EVENT, /**< Event used by application to send a IP request to the transport. (Argument used #ST_TR_IP_REQ) */
	E_TR_IP_SEND_REQ_RESP_EVENT, /**< Event used by transnport to send the IP request's response to application (Argument passed #ST_TR_IP_REQ_RESP) */ 
	E_TR_IP_CONTENT_IND_EVENT, /**< Event used by the transport to send the content indication to the applciation (Argument used #ST_TR_IP_CONTENT_IND) */
	E_TR_IP_CONTENT_IND_RESP_EVENT, /**< Event used by the application to send the content indication response to the transport (Argument used #ST_TR_IP_CONTENT_IND_RESP) */
	E_TR_IP_STOP_REQ_EVENT, /**< Event used by the application to send the stop IP requests to the transport (Argument used #ST_TR_IP_STOP_REQ) */
	E_TR_IP_STOP_RESP_EVENT, /**< Event used by the transport to send the stop response to the application (Argument used #ST_TR_IP_STOP_RESP) */
	E_TR_IP_CHANGE_SECURITY_MODE_REQ_EVENT, /**< Event used by the application to send the change in security mode to transport (Argument used #ST_TR_IP_SECURITY_CHANGE_REQ) */
	E_TR_IP_CHANGE_SECURITY_MODE_RESP_EVENT, /**< Event used by the transport to send the change security response to the application (Argument used #ST_TR_IP_SECURITY_CHANGE_RESP) */
#endif
	E_TR_SESSION_HEADER_REQ_EVENT, /**< Event sent from application to add a header which should be used for the entire transport session operations (Argument Passed #ST_TR_SESSION_HEADER_REQ)*/
	E_TR_SESSION_HEADER_RESP_EVENT, /**< Event sent by transport to send the session header response to application (Argument Passed #ST_TR_SESSION_HEADER_RESP) */
	E_TR_FETCH_URL_REQ_EVENT, /**< Event sent from application to transport to fetch a URL. (Argument Passed #ST_TR_FETCH_URL_REQ) */
	E_TR_FETCH_URL_RESP_EVENT, /**< Event used by transport to send the fetch URL response to application (Argument Passed #ST_TR_FETCH_URL_RESP) */
	E_TR_FETCH_HEADER_REQ_EVENT, /**< Event used by application to send the header info to transport (Argument Passed #ST_TR_FETCH_HEADER_REQ) */
	E_TR_FETCH_HEADER_RESP_EVENT, /**< Event used by transport to send the header request response to application (Argument Passed #ST_TR_FETCH_HEADER_RESP) */
	E_TR_FETCH_AUTH_HEADER_REQ_EVENT, /**< Event used by application to send the authorization header info to transport (Argument Passed #ST_TR_FETCH_AUTH_HEADER_REQ) */
	E_TR_FETCH_AUTH_HEADER_RESP_EVENT, /**< Event used by transport to send the authorization header request response to application (Argument Passed #ST_TR_FETCH_AUTH_HEADER_RESP) */
	E_TR_CLEAR_AUTH_HEADER_REQ_EVENT, /**< Event used by application to send the clear authorization request to transport (Argument Passed # ST_TR_CLEAR_AUTH_HEADER_REQ) */
	E_TR_CLEAR_AUTH_HEADER_RESP_EVENT, /**< Event used by transport to send the clear authorization response to transport (Argument Passed # ST_TR_CLEAR_AUTH_HEADER_RESP) */
	E_TR_USER_DATA_REQ_EVENT, /**< Event used by transport to request the application for user data (Argument Passed # ST_TR_USER_DATA_REQ)*/
	E_TR_USER_DATA_RESP_EVENT, /**< Event used by application to send the user data to the transport (Argument Passed # ST_TR_USER_DATA_RESP)*/
	E_TR_FETCH_HEADER_IND_EVENT, /**< Event used by transport to send header indication of fetch request to application (Argument Passed # ST_TR_FETCH_HEADER_IND)*/
	E_TR_FETCH_HEADER_IND_RESP_EVENT, /**< Event used by application to send the response of header indication to transport (Argument Passed # ST_TR_FETCH_HEADER_IND_RESP)*/
	E_TR_FETCH_URL_IND_EVENT, /**< Event used by transport to send URL indication (buffer) of fetch request to application (Argument Passed # ST_TR_FETCH_URL_IND)*/
	E_TR_FETCH_URL_IND_RESP_EVENT, /**< Event used by application to send the response of URL indication to transport (Argument Passed # ST_TR_FETCH_URL_IND_RESP)*/
	E_TR_FETCH_STOP_REQ_EVENT, /**< Event sent from application to transport to stop aparticular request (Argument Passed # ST_TR_FETCH_STOP_REQ)*/
	E_TR_FETCH_STOP_RESP_EVENT, /**< Event used by transport to send the response of stop request to application (Argument Passed # ST_TR_FETCH_STOP_RESP)*/
	E_TR_FETCH_STOP_ALL_REQ_EVENT, /**< Event sent from application to transport to stop all the requests that are currently processed by the transport (Argument Passed # ST_TR_FETCH_STOP_ALL_REQ)*/
	E_TR_FETCH_STOP_ALL_RESP_EVENT, /**< Event used by transport to send the response of stop all request to application (Argument Passed # ST_TR_FETCH_STOP_ALL_RESP)*/
	E_TR_PROGRESS_IND_EVENT, /**< Event used by transport to send the progress indication to application (Argument Passed # ST_TR_PROGRESS_IND)*/
#ifndef TRANSPORT_DISABLE_LOW_MEM_EVENT
	E_TR_LOW_MEMORY_IND_EVENT, /**< Event used by transport to send the low memory indication to application (Argument Passed # ST_TR_LOWMEM_IND)*/
#endif
	E_TR_ADD_ROOT_CERTIFICATE, /**< Event sent from application to add a root certificate at transport. (Argument Passed # ST_TR_ADD_ROOT_CERTIFICATE)*/
	E_TR_ADD_ROOT_CERTIFICATE_IND, /**< Event used by transport to send the add root certificate indication to transport (Argument Passed # ST_TR_ADD_ROOT_CERTIFICATE_IND)*/
	E_TR_ADD_USER_CERTIFICATE, /**< Event sent from application to add a user certificate at transport (Argument Passed # ST_TR_ADD_USER_CERTIFICATE)*/
	E_TR_ADD_USER_CERTIFICATE_IND, /**< Event used by transport to send the add user certificate indication to transport (Argument Passed # ST_TR_ADD_USER_CERTIFICATE_IND)*/
	E_TR_CLEAR_ROOT_CERTIFICATE, /**< Event sent from application to transport to clear a root certificate (Argument Passed # ST_TR_CLEAR_ROOT_CERTIFICATE)*/
	E_TR_CLEAR_ROOT_CERTIFICATE_IND, /**< Event used by transport to send the clear root certificate indication to transport (Argument Passed # ST_TR_CLEAR_ROOT_CERTIFICATE_IND)*/
	E_TR_CLEAR_USER_CERTIFICATE, /**< Event sent from application to transoprt to clear a user certificate (Argument Passed # ST_TR_CLEAR_USER_CERTIFICATE)*/
	E_TR_CLEAR_USER_CERTIFICATE_IND, /**< Event used by transport to send the clear user certificate indication to transport (Argument Passed # ST_TR_CLEAR_USER_CERTIFICATE_IND)*/
	E_TR_SET_OPTION, /**< Event sent from application to transport to set user specific values at lower layers. (Argument Passed # ST_TR_SET_OPTION)*/
#ifdef TR_PUSH
	E_TR_PUSH_REGISTER_REQ_EVENT, /**< Event sent from application to register itself with transport for receiving push (Argument Passed # ST_TR_PUSH_REGISTER_REQ)*/
	E_TR_PUSH_REGISTER_RESP_EVENT, /**< Event used by transport to send the push register response (Argument Passed # ST_TR_PUSH_REGISTER_CNF)*/
	E_TR_PUSH_HEADER_IND_EVENT, /**< Event used by transport to send the push header indication to application (Argument Passed # ST_TR_PUSH_HEADER_IND)*/
	E_TR_PUSH_HEADER_IND_RESP_EVENT, /**< Event used by application to send the push header indication to transport (Argument Passed # ST_TR_PUSH_HEADER_IND_RESP)*/
	E_TR_PUSH_CONTENT_IND_EVENT, /**< Event used by transport to send the push content indication to application (Argument Passed # ST_TR_PUSH_CONTENT_IND)*/
	E_TR_PUSH_CONTENT_IND_RESP_EVENT, /**< Event used by application to send the push content indication to transport (Argument Passed # ST_TR_PUSH_CONTENT_IND_RESP)*/
	E_TR_PUSH_INITIALIZE_REQ_EVENT, /**< Event sent from application to transport to initialize a push session (Argument Passed # ST_TR_PUSH_INITIALIZE_REQ)*/
	E_TR_PUSH_INITIALIZE_RESP_EVENT, /**< Event used by transport to send the push initialize response to application (Argument Passed # ST_TR_PUSH_INITIALIZE_RESP)*/
	E_TR_PUSH_DEINITIALIZE_REQ_EVENT, /**< Event sent from application to transport to deinitialize a push session (Argument Passed # ST_TR_PUSH_DEINITIALIZE_REQ)*/
	E_TR_PUSH_DEINITIALIZE_RESP_EVENT, /**< Event used by transport to send the push deinitialize reponse to application (Argument Passed # ST_TR_PUSH_DEINITIALIZE_RESP)*/
#endif
#ifdef TR_PUSH
	E_TR_PUSH_RECEIVE_EVENT, /**< Event used by application for requesting transport to poll for a push message. */
	E_TR_SMS_PUSH_MSG, /**< Event sent from the application requesting transport to handle a SMS content. (Argument Passed # ST_TR_PUSH_SMS_CONTENT)*/
	E_TR_WAP_PUSH_MSG, /**< Event sent from the application requesting transport to handle a Push content (Argument Passed # ST_TR_WAP_PUSH_CONTENT)*/ 
	E_TR_PARSED_WAP_PUSH_MSG, /**< Event sent from the application requesting transport to handle a parsed push content (Argument Passed # ST_TR_WAP_PUSH_CONTENT)*/
#endif
	E_TR_SECURITY_IND_REQ_EVENT, /**< Event used by transport to sent the security indicaiton to application (Argument Passed # ST_TR_SECURITY_IND_REQ)*/
	E_TR_SECURITY_IND_RESP_EVENT, /**< Event used by application to send the security indication response to transport (Argument Passed # ST_TR_SECURITY_IND_RESP)*/
	E_TR_CERTIFICATE_MORE_REQ_EVENT, /**< Event sent by application requesting transport for negotiated certificates (Argument Passed # ST_TR_CERTIFICATE_REQ)*/
	E_TR_CERTIFICATE_MORE_RESP_EVENT, /**< Event used by transport to send the certificate content to the application (Argument Passed # ST_TR_CERTIFICATE_RESP)*/
	E_TR_SECURITY_FAILURE_IND_EVENT, /**< Event used by transport to send the security failure indication to application (Argument Passed # ST_TR_SECURITY_FAILURE_IND)*/
	E_TR_SECURITY_FAILURE_RESP_EVENT, /**< Event used by application to send the seucrity failure indication response to transport (Argument Passed # ST_TR_SECURITY_FAILURE_RESP)*/
	E_TR_SOCKET_EVENT, /**< Event used by application or socket layer to intimate transport of a socket event (Argument Passed # ST_TR_SOCKET_EVENT)*/
	E_TR_DNS_EVENT, /**< Event used by application or DNS agent to intimate transport of resolved DNS. (Argument Passed # ST_TR_DNS_EVENT)*/
	E_TR_DC_EVENT, /**< Event used by application or Data connection agent to intimate transport of change in data connection status (Argument Passed # ST_TR_DC_EVENT)*/
	E_TR_SUSPEND_SESSION_REQ_EVENT, /**< Event sent from application to suspend a transport session (Argument Passed # ST_TR_SUSPEND_SESSION_REQ)*/
	E_TR_SUSPEND_SESSION_RESP_EVENT, /**< Event used by transport to send the suspend session response to application (Argument Passed # ST_TR_SUSPEND_SESSION_RESP)*/
	E_TR_RESUME_SESSION_REQ_EVENT, /**< Event sent from application to resume a suspended transport session (Argument Passed # ST_TR_RESUME_SESSION_REQ)*/
	E_TR_RESUME_SESSION_RESP_EVENT, /**< Event used by transport to send the resume session response to application (Argument Passed # ST_TR_RESUME_SESSION_RESP)*/
	E_TR_SUSPEND_FETCH_REQ_EVENT, /**< Event sent from application to transport to suspend a fetch request. (Argument Passed # ST_TR_SUSPEND_FETCH_REQ)*/
	E_TR_SUSPEND_FETCH_RESP_EVENT, /**< Event used by transport to send the suspend fetch request response to application (Argument Passed # ST_TR_SUSPEND_FETCH_RESP)*/
	E_TR_RESUME_FETCH_REQ_EVENT, /**< Event used by application to transport to resume a suspended fetch request (Argument Passed # ST_TR_RESUME_FETCH_REQ)*/
	E_TR_RESUME_FETCH_RESP_EVENT, /**< Event used by transport to send the resume fetch request response to application (Argument Passed # ST_TR_RESUME_FETCH_RESP)*/
	E_TR_TIMER_EVENT, /**< Event used by transport to invoke the transport timer. (Argument Passed # )*/
	E_TR_CALLBACK_EVENT, /**< Event used by transport to invoke the transport callback (Argument Passed # )*/
	E_TR_PUSH_TIMER_EVENT, /**< Event used to invoke the transport push timer. (Argument Passed # )*/
#ifdef EMAIL_PROTOCOL_ENABLE  /** E-Mail Events **/
	E_TR_AUTH_REQ_EVENT,/**< Event sent from email communicator to transport requesting to authorize user*/
	E_TR_AUTH_RESP_EVENT,/**< Event sent by transport to send authorize response to the email communicator*/
	E_TR_ADD_PROFILE_REQ_EVENT,	/**< Event sent from email communicator to transport to request add profile*/
	E_TR_ADD_PROFILE_RESP_EVENT,/**< Event sent by transport to send add profile response to the email communicator*/
	E_TR_DELETE_PROFILE_REQ_EVENT,/**< Event sent from email communicator to transport to request delete profile*/
	E_TR_DELETE_PROFILE_RESP_EVENT,/**< Event sent by transport to send delete profile response to the email communicator*/
	E_TR_UPDATE_PROFILE_REQ_EVENT,/**< Event sent from email communicator to transport to request update profile*/
	E_TR_UPDATE_PROFILE_RESP_EVENT,/**< Event sent by transport to send update profile response to the email communicator*/
	E_TR_SEND_MAIL_REQ_EVENT,/**< Event sent from email communicator to transport to request send mail*/
	E_TR_SEND_MAIL_RESP_EVENT,/**< Event sent by transport to send send mail response to the email communicator*/
	E_TR_RECV_MAIL_REQ_EVENT,/**< Event sent from email communicator to transport to request receiving a mail*/
	E_TR_RECV_MAIL_RESP_EVENT,/**< Event sent by transport to send recv mail response to the email communicator*/
	E_TR_RECV_SPECIFIC_PART_MAIL_REQ_EVENT,/**< Event sent from email communicator to transport to request recv specific part of mail*/
	E_TR_RECV_SPECIFIC_PART_MAIL_RESP_EVENT,/**< Event sent by transport to send recv specific part of mail response to the email communicator*/
	E_TR_RECV_MAIL_LIST_REQ_EVENT,/**< Event sent from email communicator to transport to request recv mail list*/
	E_TR_RECV_MAIL_LIST_RESP_EVENT,/**< Event sent by transport to send receive mail list response to the email communicator*/
	E_TR_RECV_MAIL_HEADER_REQ_EVENT,/**< Event sent from email communicator to transport to request receive mail headers*/
	E_TR_RECV_MAIL_HEADER_RESP_EVENT,/**< Event sent by transport to send receive mail header response to the email communicator*/
	E_TR_RECV_MAIL_BODY_STRUCTURE_REQ_EVENT,/**< Event sent from email communicator to transport to request receive mail body structure*/
	E_TR_RECV_MAIL_BODY_STRUCTURE_RESP_EVENT,/**< Event sent by transport to send receive mail body structure response to the email communicator*/
	E_TR_SET_MAIL_FLAG_REQ_EVENT,/**< Event sent from email communicator to transport to request set mail flag*/			
	E_TR_SET_MAIL_FLAG_RESP_EVENT,/**< Event sent by transport to send set mail flag response to the email communicator*/
	E_TR_RECV_FOLDER_LIST_REQ_EVENT,/**< Event sent from email communicator to transport to request receive folder list*/
	E_TR_RECV_FOLDER_LIST_RESP_EVENT,/**< Event sent by transport to send recv folder list response to the email communicator*/
	E_TR_SUBSCRIBE_FOLDER_REQ_EVENT,/**< Event sent from email communicator to transport to request subscribe the particular folder*/
	E_TR_SUBSCRIBE_FOLDER_RESP_EVENT,/**< Event sent by transport to send subscribe folder response to the email communicator*/

	E_TR_UNSUBSCRIBE_FOLDER_REQ_EVENT,/**< Event sent from email communicator to transport to request unsubscribe the particular folder*/
	E_TR_UNSUBSCRIBE_FOLDER_RESP_EVENT,/**< Event sent by transport to send unsubscribe folder response to the email communicator*/
	
	E_TR_FILTER_MAIL_IND_EVENT,/**< Event sent by transport to send filter mail indication to the email communicator*/
	E_TR_FILTER_MAIL_REQ_EVENT,/**< Event sent from email communicator to transport to request receiving a filter mail*/
	E_TR_FILTER_MAIL_RESP_EVENT,/**< Event sent by transport to send recv filter mail response to the email communicator*/
	E_TR_DELETE_MAIL_REQ_EVENT,/**< Event sent from email communicator to transport to request receiving a delete mail*/
	E_TR_DELETE_MAIL_RESP_EVENT,/**< Event sent by transport to send delete mail response to the email communicator*/
	E_TR_STOP_MAIL_REQ_EVENT,/**< Event sent from email communicator to transport to request stop mail*/
	E_TR_STOP_MAIL_RESP_EVENT,/**< Event sent by transport to send stop mail response to the email communicator*/
	E_TR_PURGE_REQ_EVENT,/**< Event sent from email communicator to transport to request purge the folder*/
	E_TR_PURGE_RESP_EVENT,/**< Event sent by transport to send purge folder response to the email communicator*/
	E_TR_RECV_SUBCRIBED_FOLDER_LIST_REQ_EVENT,/**< Event sent from email communicator to transport to request receive subscribe folder list*/		
	E_TR_RECV_SUBCRIBED_FOLDER_LIST_RESP_EVENT,/**< Event sent by transport to send recv subscribe folder list response to the email communicator*/

	E_TR_SEND_MAIL_IND_EVENT,/**< Event sent by transport to send send mail indication to the email communicator*/
	E_TR_RECV_MAIL_IND_EVENT,/**< Event sent by transport to send receive mail indication to the email communicator*/
	/*E_TR_RECV_SPECIFIC_PART_MAIL_IND_EVENT,*/
	E_TR_RECV_MAIL_BODY_STRUCTURE_IND_EVENT,/**< Event sent by transport to send recv mail body structure to the email communicator*/
	E_TR_RECV_MAIL_LIST_IND_EVENT,/**< Event sent by transport to send recv mail list indication to the email communicator*/
	E_TR_RECV_MAIL_HEADER_IND_EVENT,/**< Event sent by transport to send recv mail header indication to the email communicator*/
	E_TR_RECV_FOLDER_LIST_IND_EVENT,/**< Event sent by transport to send recv folder list indication to the email communicator*/
	E_TR_SUBSCRIBE_FOLDER_IND_EVENT,/**< Event sent by transport to send recv subscribe folder indication to the email communicator*/
	E_TR_UNSUBSCRIBE_FOLDER_IND_EVENT,/**< Event sent by transport to send recv unsubscribe folder indication to the email communicator*/
	E_TR_DELETE_MAIL_IND_EVENT,/**< Event sent by transport to send delete mail indication to the email communicator*/
	/*E_TR_STOP_MAIL_IND_EVENT,*/
	E_TR_RECVMAIL_ERR_IND_EVENT,/**< Event sent by transport to send receive mail error indication to the email communicator*/
	E_TR_SET_MAIL_FLAG_IND_EVENT,/**< Event sent by transport to send set mail flag indication to the email communicator*/
	E_TR_PURGE_IND_EVENT,/**< Event sent by transport to send delete purge folder indication to the email communicator*/
	E_TR_RECV_SUBSCRIBED_FOLDER_LIST_IND_EVENT,	/**< Event sent by transport to send receive subscribe folder list indication to the email communicator*/	
	E_TR_CREATE_FOLDER_REQ_EVENT,/**< Event sent from email communicator to transport to request create folder on server*/
	E_TR_CREATE_FOLDER_RESP_EVENT,/**< Event sent by transport to send create folder response to the email communicator*/
	E_TR_CREATE_FOLDER_IND_EVENT,/**< Event sent by transport to send create folder indication to the email communicator*/
	E_TR_DELETE_FOLDER_REQ_EVENT,/**< Event sent from email communicator to transport to request delete folder on server*/
	E_TR_DELETE_FOLDER_IND_EVENT,/**< Event sent by transport to send delete folder indication to the email communicator*/
	E_TR_DELETE_FOLDER_RESP_EVENT,/**< Event sent by transport to send delete folder response to the email communicator*/
	E_TR_RENAME_FOLDER_REQ_EVENT ,/**< Event sent from email communicator to transport to request renaming folder on server*/
	E_TR_RENAME_FOLDER_RESP_EVENT ,/**< Event sent by transport to send renaming folder response to the email communicator*/
	E_TR_RENAME_FOLDER_IND_EVENT ,/**< Event sent by transport to send rename folder indication to the email communicator*/
    E_TR_COPY_MOVE_MAIL_REQ_EVENT ,/**< Event sent from email communicator to transport to copy/move mail one folder to another*/
	E_TR_COPY_MOVE_MAIL_RESP_EVENT ,/**< Event sent by transport to send copy/move mail response to the email communicator*/
	E_TR_COPY_MOVE_MAIL_IND_EVENT ,/**< Event sent by transport to send copy/move mail indication to the email communicator*/

	E_TR_SAVE_MAIL_REQ_EVENT ,/**< Event sent from email communicator to transport to save mail*/
	E_TR_SAVE_MAIL_RESP_EVENT ,/**< Event sent by transport to send save mail response to the email communicator*/
	E_TR_SAVE_MAIL_IND_EVENT ,/**< Event sent by transport to send save mail indication to the email communicator*/
	E_TR_EMAIL_KEEPALIVE_IND_EVENT, /**Event sent by transport to send keep alive indication to the email communicator*/
	E_TR_DISCONNECT_IND_EVENT,/**< Event sent by transport to send disconnect indication to the email communicator*/
	E_TR_MAIL_CALLBACK_EVENT,/**< Event used by transport to invoke the transport email callback */
	E_TR_MAIL_USER_DATA_REQ_EVENT,/**< Event sent from email communicator to transport to get user data mail details*/
	E_TR_MAIL_USER_DATA_RESP_EVENT,/**< Event sent by transport to user data mail response to the email communicator*/
	E_TR_IMAP_IDLE_REQ_EVENT, /**< Event sent from email communicator to transport to get the new mail details from the server */
	E_TR_IMAP_IDLE_RESP_EVENT, /**< Event sent by transport to send imap idle response to the email communicator*/
	E_TR_IMAP_IDLE_IND_EVENT,  /**< Event sent by transport to send imap idle indication to the email communicator */	
	E_TR_KEEPALIVE_REQ_EVENT, /**< Event sent from email communicator to transport to set the connection alive */
	E_TR_KEEPALIVE_RESP_EVENT, /**< Event sent by transport to send Keep Alive response to the email communicator */
#endif
	E_TR_SHUTDOWN_REQ_EVENT, /**< Event sent from application to shutdown the transport session (Argument Passed # )*/
	E_TR_SHUTDOWN_RESP_EVENT /**< Event used by transport to send the shutdown indication to application (Argument Passed # )*/
} E_TR_EVENT_TYPE ;

/** @endcond */

/*** Function Prototypes	***/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	This function is used to initialize the transport module.
 * @param[out]	pvHandle The function will return the transport handle in this field. The 
 *				application needs to use this handle to call any of the transport functions 
 * @retval		JC_OK if the transport module is initialized successfully
 * @retval		JC_ERR_MEMORY_ALLOCATION in case of memory allocation
 * @note		The parameter which should not be NULL is vHandle.
 * @see			jdi_TransportDeInitialize.
 * 
 * The application needs to call this function to initialize the transport module.
 */
JC_RETCODE jdi_TransportInitialize (JC_HANDLE *pvHandle) ;

/**
 * @brief	This function is used to deinitialize the transport module.
 * @param[in]	vHandle Specifies the handle which was returned when the transport was initialized.
 * @note		The parameter which should not be NULL is vHandle.
 * 
 * The application needs to call this function to deinitialize the transport module.
 */
JC_RETCODE jdi_TransportDeInitialize (JC_HANDLE vHandle) ;

/**
 * @brief	This function is used to initialize the transport task.
 * @param[in]	ulParams This parameter is currently not used.
 * @param[in]	pParams This parameter is currently not used.
 * @retval		JC_OK if the transport task is created sucessfully.
 * @retval		JC_ERR_QUEUE_EXISTS if the transport queue already exist
 * @retval		JC_ERR_QUEUE_CREATE error while creating queue.
 * @retval		JC_ERR_QUEUE_RECV error while receiving events in transport queue
 * @see			jdi_TransportInitialize
 * @see			jdi_TransportHandleRequestEvent
 * 
 * The application needs to use this function to set up the transport task. The function creates
 * the transport queue and then transport waits for application events. If some event come then
 * the transport handler function is called to handle the event.
 */
void jdi_TransportTask (JC_UINT64 ulParams, void *pParams) ;

/**
 * @brief	This function is used by transport to handle the events sent from application
 * @param[in]	pEventData Specifies the event data information
 * @param[in]	vTrHandle Specifies the transport handle.
 * @retval		JC_OK if the event is successfully handled.
 * @note The  parameter which should not be NULL are pEventData and vTrHandle.
 * 
 * This function is used to handle all the events sent from the application. For each event a 
 * function is defined in transport. So the appropriate function is called.fs
 */
void jdi_TransportHandleRequestEvent (JC_EVENT_DATA *pEventData,
									  JC_HANDLE		vTrHandle) ;

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


