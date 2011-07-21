/***************************************************************************
 * $Id: jdi_trfetch.h,v 1.12 2009/01/19 06:02:57 browserdevjtbase Exp $
 * $Revision: 1.12 $
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
 * $Log: jdi_trfetch.h,v $
 * Revision 1.12  2009/01/19 06:02:57  browserdevjtbase
 * Core update
 *
 * Revision 1.11  2008/11/18 07:44:09  browserdevjtbase
 * Core updates
 *
 * Revision 1.10  2008/02/13 12:56:50  browserdevjtbase
 * Updated the core code.
 *
 * Revision 1.9  2007/11/14 08:52:37  browserdevjtbase
 * core update.
 *
 * Revision 1.8  2007/09/27 14:52:23  browserdevjtbase
 * Core Update
 *
 * Revision 1.7  2007/04/10 15:31:01  browserdevjtbase
 * Update ...
 *
 * Revision 1.6  2006/12/21 07:24:39  browserdevjtbase
 * Update
 *
 * Revision 1.5  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.4  2006/07/03 07:26:11  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.3  2006/06/14 06:40:15  browserdevjtbase
 * Update ...
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.42  2006/04/28 07:36:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.41  2006/04/22 07:38:18  kumardevhtmlbrow
 * Transport logging updated
 *
 * Revision 1.40  2006/03/01 14:57:58  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.39  2006/01/09 14:45:52  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.38  2006/01/09 12:16:28  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.37  2006/01/09 07:00:54  kumardevhtmlbrow
 * Updated to HTTP push to handle listen has event based.
 *
 * Revision 1.36  2006/01/06 07:13:03  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.35  2005/12/31 06:46:08  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.34  2005/12/30 13:54:23  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.33  2005/12/29 07:41:11  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.32  2005/12/28 05:53:40  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.31  2005/12/23 11:42:11  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.30  2005/12/09 15:15:16  kumardevhtmlbrow
 * Added the suspend and resume request. Providing additional interface for starting confirmed & http push. Changes as per new JDD LOG abstraction
 *
 * Revision 1.29  2005/12/08 07:14:10  kumardevhtmlbrow
 * Changes for supporting different wsp mode and charset bug fixing
 *
 * Revision 1.28  2005/12/05 13:14:24  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.27  2005/12/03 08:05:37  kumardevhtmlbrow
 * Changes made in transport for handling security switch
 *
 * Revision 1.26  2005/12/02 11:27:06  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.25  2005/12/01 12:50:42  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.24  2005/11/29 15:18:32  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.23  2005/10/27 14:17:35  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.22  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.21  2005/08/12 12:11:08  shreyasdevhtmlbrow
 * no message
 *
 * Revision 1.20  2005/08/12 11:50:17  shreyasdevhtmlbrow
 * no message
 *
 * Revision 1.19  2005/08/02 12:06:45  kumardevhtmlbrow
 * no message
 *
 * Revision 1.18  2005/08/01 12:03:13  kumardevhtmlbrow
 * no message
 *
 * Revision 1.17  2005/07/22 14:31:03  kumardevhtmlbrow
 * no message
 *
 * Revision 1.16  2005/07/20 14:22:06  kumardevhtmlbrow
 * no message
 *
 * Revision 1.15  2005/07/02 14:56:53  kumardevhtmlbrow
 * no message
 *
 * Revision 1.14  2005/06/28 15:30:21  kumardevhtmlbrow
 * no message
 *
 * Revision 1.13  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.12  2005/06/21 12:04:00  kumardevhtmlbrow
 * no message
 *
 * Revision 1.11  2005/06/17 08:17:36  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.10  2005/06/07 15:26:45  kumardevhtmlbrow
 * no message
 *
 * Revision 1.9  2005/06/06 12:12:02  shreyasdevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/06/04 07:56:52  kumardevhtmlbrow
 * updated the transport task ...
 *
 * Revision 1.7  2005/05/30 13:03:21  kumardevhtmlbrow
 * Updated the event
 *
 * Revision 1.6  2005/05/27 13:26:32  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2005/05/27 07:24:05  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/05/21 12:09:26  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.3  2005/05/21 11:37:37  shreyasdevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/05/18 15:33:35  kumardevhtmlbrow
 * Update to transport task by adding task information and session id
 *
 * Revision 1.1  2005/05/17 06:25:14  kumardevhtmlbrow
 * Added the transport interface files
 *
 *  
 ***************************************************************************/

/*
 * @file jdi_trfetch.h
 * @ingroup Transport Task.
 * @brief Defines various enums and structures used for fetch related operations.
 *
 * This file defines all the enums and structures that are to be used for fetch related operations
 * by both transport and application. For Each fetch related event the application needs to specify
 * a structure which is defined here. In the same way the transport response structure is also 
 * defined here
 */

#ifndef _JDI_TRFETCH_H
#define _JDI_TRFETCH_H

#include <jdi_transport.h>
#include <jdi_wspdatatype.h>
#include <jdi_wsp.h>
#include <jdi_http.h>
#include <jdi_httpstatus.h>
#include <jdi_wspstatus.h>

/*** Macros				***/

#define	MAX_HEADER_LEN			255
#define MAX_CONTENT_TYPE_LEN	255
#define MAX_CERT_PASSWORD_LEN	255
#define MAX_SME_ADDR_LEN		32

/*** Type Definitions	***/

/**
 * @brief The application needs to use this enum to specify the type of authorization when 
 *		  sending the authorization header to the transport (ST_TR_FETCH_AUTH_HEADER_REQ).
 */
typedef enum _authorization_type
{
	E_AUTHORIZATION_BASIC, /**< Specifies that authorization is of type basic. */
	E_AUTHORIZATION_DIGEST /**< Specifies that authorization is of type digest */
} E_AUTHORIZATION_TYPE ;

/**
 * @brief The application needs to use this enum to specify the type of authorization header when 
 *		  sending the authorization header to the transport (ST_TR_FETCH_AUTH_HEADER_REQ).
 */
typedef enum _authorization_header
{
	E_URI_AUTHORIZATION = 0, /**< Specifies that authorization is URL authorization */
	E_PROXY_AUTHORIZATION /**< Specifies that authorization is proxy authorization */
} E_AUTHORIZATION_HEADER ;

/**
 * @brief The application needs to use this enum to specify the type of fetch request when 
 *		  giving a fetch URL request to the transport (ST_TR_FETCH_URL_REQ).
 */
typedef enum tr_fetch_type
{
	E_TR_FETCH_GET, /**< Specifies that request given is get request */
	E_TR_FETCH_HEAD, /**< Specifies that request given is fetch header request */
	E_TR_FETCH_DELETE, /**< Specifies that request given is delete request */
	E_TR_FETCH_POST, /**< Specifies that the request given is post request */
	E_TR_FETCH_PUT	/**< Specifies that the request given is put request */
} E_TR_FETCH_TYPE ;

/**
 * @brief The application needs to use this enum to specify the type of request when 
 *		  giving a fetch URL request to the transport (ST_TR_FETCH_URL_REQ).
 */
typedef enum tr_request_type
{
	E_TR_MAIN_REQUEST, /**< Specifies that request is to fetch a main page */
	E_TR_CSS_REQUEST, /**< Specifies that request is to fetch the CSS content */
	E_TR_IMAGE_REQUEST, /**< Specifies that request is to fetch a image */
	E_TR_AUDIO_REQUEST, /**< Specifies that request is to fetch an audio */
	E_TR_VIDEO_REQUEST /**< Specifies that request is to fetch a video */
} E_TR_REQUEST_TYPE ;

/**
 * @brief The application needs to use this structure while sending the fetch URL request event
 *		  E_TR_FETCH_URL_REQ_EVENT to the transport.
 */
typedef struct tr_fetch_url_req
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request */
	JC_UINT32			uiUserDataTotalLength ; /**< Specifies the length of the user data (post data) */
	E_TR_FETCH_TYPE		eTrFetchType ; /**< Specifies the type of fetch to be done */
	E_TR_REQUEST_TYPE	eTrRequestType ; /**< Specifies the type of request */
	JC_BOOLEAN			bHasHeader ; /**< Specifies whether the fetch request needs header */
	JC_BOOLEAN 			bIsHeaderRequired ; /**< Specifies whether the application required header indication */
	JC_BOOLEAN 			bIsProgressInfoRequired ; /**< Specifies whether the application required progress indication */
	JC_BOOLEAN			bUseDefaultAcceptHdr ; /**< Specifies whether application wants transport to use default accept header */
	JC_INT8				acContentType [TR_MAX_CONTENT_TYPE_SIZE + 1]; /**< Specifies the content type */
	JC_UINT8			ucURL [1] ; /**< Specifies the URL to be fetched */
} ST_TR_FETCH_URL_REQ ;

/**
 * @brief The transport uses this structure while sending the fetch URL request response event
 *		  E_TR_FETCH_URL_RESP_EVENT to the application. 
 */
typedef struct tr_fetch_url_resp
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the fetch URL resp is sent */
	JC_RETCODE			rCode ; /**< Specifies error if any while proceesing fetch URL request */
} ST_TR_FETCH_URL_RESP ;

/**
 * @brief The application needs to use this structure while sending the header request event E_TR_FETCH_HEADER_REQ_EVENT
 *		  to the transport. The header specified in the structure will be used while fetch request is sent to the
 *		  lower layers.
 */
typedef struct tr_fetch_header_req
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which header is sent */
	JC_BOOLEAN 			bHasMoreHeader ; /**< Specifies whether more header is to be sent from application */
	JC_INT8				acCharset [TR_MAX_CHARSET_SIZE + 1] ; /**< Specifies the charset information */
	JC_INT8				acName [TR_MAX_HEADER_NAME_SIZE + 1] ; /**< Specifies the header name if any */
	JC_UINT8			ucValue [1] ; /**< Specifies the header value */
} ST_TR_FETCH_HEADER_REQ ;

/**
 * @brief The transport uses this structure while sending the header response event E_TR_FETCH_HEADER_RESP_EVENT
 *		  to the application.
 */
typedef struct tr_fetch_header_resp
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the fetch header resp is sent */
	JC_RETCODE			rCode ; /**< Specifies error if any while processing fetch header request */
} ST_TR_FETCH_HEADER_RESP ;

/**
 * @brief The application needs to use this structure while sending the authroization header 
 *		  request event E_TR_FETCH_AUTH_HEADER_REQ_EVENT to the transport. The authorization 
 *		  header specified in the structure will be used while fetch request is sent to the
 *		  lower layers.
 */
typedef struct tr_fetch_auth_header_req
{
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32				uiRequestID ; /**< Specifies the ID of the request for which auth header is sent */
	JC_BOOLEAN 				bHasMoreHeader ; /**< Specifies whether more auth header is to be sent from application */
	E_AUTHORIZATION_TYPE	eAuthType ; /**< Specifies the auhorization type */
	E_AUTHORIZATION_HEADER	eAuthHeader ; /**< Specifies the authorization header type */
	JC_INT8					acUserName [TR_MAX_AUTH_USERNAME_SIZE + 1]; /**< Specifies the auth user name */
	JC_INT8					acPassword [TR_MAX_AUTH_PASSWORD_SIZE + 1] ; /**< Specifies the auth password */
	JC_INT8					acDomain [TR_MAX_AUTH_DOMAIN_SIZE + 1] ; /**< Specifies the auth domain information */
	JC_INT8					acRealm [TR_MAX_AUTH_REALM_SIZE + 1] ; /**< Specifies the auth realm info */
	JC_INT8					acNOnce [TR_MAX_AUTH_NONCE_SIZE + 1] ; /**< Specifies the auth nonce info */
	JC_INT8					acOpaque [TR_MAX_AUTH_OPAQUE_SIZE + 1] ;/**< This Specifies the Opaque information (in case of digest authorization).*/
	JC_INT8					acQOP [TR_MAX_AUTH_QOP_SIZE + 1] ;/**< This Specifies the QOP information (in case of digest authorization).*/
	JC_INT8					acAlgorithm [TR_MAX_AUTH_ALG_SIZE + 1] ;/**< This Specifies the algorithm information (in case of digest authorization).*/
} ST_TR_FETCH_AUTH_HEADER_REQ ;

/**
 * @brief The transport uses this structure while sending the authorization header response 
 *	      event E_TR_FETCH_AUTH_HEADER_RESP_EVENT to the application.
 */
typedef struct tr_fetch_auth_header_resp
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the fetch auth header resp is sent */
	JC_RETCODE			rCode ; /**< Specifies error if any while processing fetch auth header request */
} ST_TR_FETCH_AUTH_HEADER_RESP ;

/**
 * @brief The application needs to use this structure to clear the authorization information
 *		  that is stored in a transport task session.
 */
typedef struct tr_clear_auth_header_req
{
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	E_AUTHORIZATION_HEADER	eAuthHeader ; /**< Specifies the type of authorization list to be cleared. */
} ST_TR_CLEAR_AUTH_HEADER_REQ ;

/**
 * @brief The transport uses this structure while sending the clear authorization header response 
 *	      event E_TR_CLEAR_AUTH_HEADER_RESP_EVENT to the application.
 */
typedef struct tr_clear_auth_header_resp
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE			rCode ; /**< Specifies error if any while processing clear auth header request */
} ST_TR_CLEAR_AUTH_HEADER_RESP ;

/**
 * @brief The transport uses this structure while sending the user data request event E_TR_USER_DATA_REQ_EVENT 
 *		  to the application.
 */
typedef struct tr_user_data_req
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the user data is required */
	JC_UINT32			uiOffset ; /**< Specifies the user data already received by transport in previous chunks */
	JC_UINT32			uiUserDataLength ; /**< Specifies the user data expected in the current chunk */
} ST_TR_USER_DATA_REQ ;

/**
 * @brief The application uses this structure while sending the user data response event E_TR_USER_DATA_RESP_EVENT
 *		  to the transport. The structure contains the user data and user data length that is to be
 *        used by the transport.
 */
typedef struct tr_user_data_resp
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the user data is sent */
	JC_RETCODE			rCode ; /**< Specifies the error if any while application processed the user data request */
	JC_UINT32			uiUserDataLength ; /**< Specifies the user data length sent in the current chunk */
	JC_BOOLEAN			bIsUserMoreData ; /**< Specifies whether more data is to be sent */
	JC_UINT8			ucUserData [1] ; /**< Specifies the user data */
} ST_TR_USER_DATA_RESP ;

/**
 * @brief Once the transport receives the header for a fetch request it uses this structure while
 *		  sending the header indication event E_TR_FETCH_HEADER_IND_EVENT to the application.
 */
typedef struct tr_fetch_header_ind
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the fetch header indication is sent */
	JC_UINT32			uiTotalHeaderLength ; /**< Specifies the total header length */
	JC_UINT32			uiHeaderLength ; /**< Specifies the length of header sent in the current header indication */
	JC_UINT32			uiContentLength ; /**< Specifies the length of the content to be fetched */
	JC_BOOLEAN			bIsBinary ; /**< Specifies whether the content is binary or not */
	JC_BOOLEAN			bHasMoreHeader ; /**< Specifies whether more header indication is to be sent */
	E_HTTP_STATUS		eStatus ; /**< Specfies the lower layer status */
	JC_INT8				acContentType [MAX_CONTENT_TYPE_LEN + 1]; /**< Specifies the content type */
	JC_UINT8			ucHeader [1] ; /**< Specifies the header */
} ST_TR_FETCH_HEADER_IND ;

/**
 * @brief The application needs to use this structure while sending the header indication response
 *	      event E_TR_FETCH_HEADER_IND_RESP_EVENT to the transport.
 */
typedef struct tr_fetch_header_ind_resp
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the header indication response is sent */
	JC_RETCODE			rCode ; /**< Specifies error if any while processing fetch header indication */
} ST_TR_FETCH_HEADER_IND_RESP ;

/**
 * @brief Once the transport receives the buffer for a fetch request it uses this structure while
 *		  sending the buffer or URL indication event E_TR_FETCH_URL_IND_EVENT to the application.
 */
typedef struct tr_fetch_url_ind
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the buffer is sent */
	JC_RETCODE			rCode ; /**< Specifies error in transport if any */
	JC_UINT32			uiTotalLength ; /**< Specifies the total length of the buffer to be received */
	JC_UINT32			uiCurrLength ; /**< Specifies the length of the buffer sent in current chunk */
	JC_BOOLEAN			bHasMoreData ; /**< Specifies whether more buffer is to be sent from transport */
	E_HTTP_STATUS		eStatus ; /**< Specifies the lower layer status */
	JC_INT8				acContentType [MAX_CONTENT_TYPE_LEN + 1]; /**< Specifies the content type */
	JC_UINT8			ucBuffer [1] ; /**< Specifies the buffer */
} ST_TR_FETCH_URL_IND ;

#ifdef IP_STACK_ENABLE
/**
 * @brief The application needs to use this enumeration to set the security mode. By default the
 *		  security mode is non-secure.
 */
typedef enum tr_ip_security_type
{
#ifdef JCF_SSL
	E_TR_IP_SECURE, /** Specifies that secure mode is to be used. */
#endif
	E_TR_IP_NON_SECURE /** Specifies that non-secure mode is to be used. */
} E_TR_IP_SECURITY_TYPE ;

/**
 * @brief The application needs to use this structure while sending the IP request event
 *		  E_TR_IP_SEND_REQ_EVENT to the transport.
 */
typedef struct tr_ip_send_req
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiIPReqID ; /**< Specifies the ID of the IP request */
	JC_UINT32			uiSendDataLen ; /**< Specifies the length of the IP request. */
	JC_UINT8			ucSendData [1] ;  /**< Specifies the IP request */
} ST_TR_IP_SEND_REQ ;

/**
 * @brief Once the transport has processed (added) the IP request then it uses this structure to 
 *		  send the IP send request response E_TR_IP_SEND_REQ_RESP_EVENT to the application.
 */
typedef struct tr_ip_send_resp
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiIPReqID ; /**< Specifies the ID of the IP request */
	JC_RETCODE			rCode ; /**< Specifies error code if any while processing the IP request */
} ST_TR_IP_SEND_RESP ;

/**
 * @brief Once the transport has received content for a IP request then it uses this structure to
 *	      send the content indication event E_TR_IP_CONTENT_IND_EVENT to the application.
 */
typedef struct tr_ip_content_ind
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiIPReqID ; /**< Specifies the request ID. This will be specified only in case the content ind
									  	 is for an error that happened while sending a request else the value is 0
										 (for recv error or content the value will be 0) */
	JC_RETCODE			rCode ; /**< Specifies the error code if any while processing the IP requests */
	JC_UINT32			uiBufferLen ; /**< Specifies the length of the buffer (0 for error) */
	JC_UINT8			ucBuffer [1] ; /**< Specifies the buffer (NULL for error) */
} ST_TR_IP_CONTENT_IND ;

/**
 * @brief The application needs to use this structure while sending the content indication response event 
 *		  E_TR_IP_CONTENT_IND_RESP_EVENT to the transport.
 */
typedef struct tr_ip_content_ind_resp
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE			rCode ; /**< Specifies the return code for the content indication. If JC_OK it means continue receiving for the same request,
									 if JC_ERR_TR_IP_REQ_COMPLETED it means request is completed else it means application encountered error */
} ST_TR_IP_CONTENT_IND_RESP ;

/**
 * @brief The application needs to use this structure while sending the stop request event 
 *		  E_TR_IP_STOP_REQ_EVENT to the transport.
 */
typedef struct tr_ip_stop_req
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
} ST_TR_IP_STOP_REQ ;

/**
 * @brief Once the transport has processed the stop request then it uses this structure to
 *	      send the stop response event E_TR_IP_STOP_RESP_EVENT to the application.
 */
typedef struct tr_ip_stop_resp
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE			rCode ; /**< Specifies error if any while stopping the IP requests. */
} ST_TR_IP_STOP_RESP ;

/**
 * @brief The application needs to use this structure while sending the security change request event 
 *		  E_TR_IP_CHANGE_SECURITY_MODE_REQ_EVENT to the transport.
 */
typedef struct tr_ip_security_change_req
{
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	E_TR_IP_SECURITY_TYPE	eSecurityType ; /**< Specifies the type of security mode */
} ST_TR_IP_SECURITY_CHANGE_REQ ;

/**
 * @brief Once the transport has processed the security change request then it uses this structure to
 *	      send the stop response event E_TR_IP_CHANGE_SECURITY_MODE_RESP_EVENT to the application.
 */
typedef struct tr_ip_security_change_resp
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE			rCode ; /**< Specifies error if any while changing the security mode. */
} ST_TR_IP_SECURITY_CHANGE_RESP ;

#endif

/**
 * @brief The application needs to use this structure while sending the buffer or URL indication
 *		  response event E_TR_FETCH_URL_IND_RESP_EVENT to the transport.
 */
typedef struct tr_fetch_url_ind_resp
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the URL(buffer) indication is sent */
	JC_RETCODE			rCode ; /**< Specifies error if any while processing fetch url indication */
} ST_TR_FETCH_URL_IND_RESP ;

/**
 * @brief The application needs to use this structure while sending the stop request event
 *		  E_TR_FETCH_STOP_REQ_EVENT to the transport. The structure specifies the ID of the request
 *		  which needs to be stopped.
 */
typedef struct _tr_stop_req
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request which needs to be stopped */
} ST_TR_FETCH_STOP_REQ ;

/**
 * @brief The transport uses this structure while sending the stop response event E_TR_FETCH_STOP_RESP_EVENT
 *		  to the application.
 */
typedef struct _tr_stop_resp
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which  */
	JC_RETCODE			rCode ; /**< Specifies error in any while processing the stop request */
} ST_TR_FETCH_STOP_RESP ;

/**
 * @brief The application needs to use this structure while sending the stop all request event
 *		  E_TR_FETCH_STOP_ALL_REQ_EVENT to the transport. The structure specifies the ID of the transport
 *		  seesion whose requests needs to be stopped.
 */
typedef struct _tr_stop_all_req
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
} ST_TR_FETCH_STOP_ALL_REQ ;

/**
 * @brief The transport uses this structure while sending the stop all request response event 
 *		  E_TR_FETCH_STOP_ALL_RESP_EVENT to the application.
 */
typedef struct _tr_stop_all_resp
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE			rCode ; /**< Specifies error in any while processing the stop request */
} ST_TR_FETCH_STOP_ALL_RESP ;

#ifdef TR_PUSH

/**
 * @brief Once the transport receives push header it uses this structure while sending the
 *		  push header indication event E_TR_PUSH_HEADER_IND_EVENT to the application.
 */

typedef struct tr_push_header_ind
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiPushID ; /**< Specifies the ID of the push content */
	JC_UINT32			uiTotalHeaderLength ; /**< Specifies the total push header length */
	JC_UINT32			uiHeaderLength ; /**< Specifies the length of the push header sent in the current chunk */
	JC_UINT32			uiContentLength ; /**< Specifies the length of the push content */
	JC_BOOLEAN			bHasMoreHeader ; /**< Specifies whether more push header is to be sent */
	JC_BOOLEAN			bIsBinary ; /**< Specifies whether the push content is in binary or not */
	JC_BOOLEAN			bIsPPGAddress ; /**< Specifies whether the PPG address is mentioned in the push content */
	JC_INT8				acPPGHostAddress [TR_MAX_PPGHOST_SIZE + 1] ; /**< Specifies the push PPG if any */
	JC_INT8				acOriginatingAddress [TR_MAX_PPGHOST_SIZE + 1] ; /**< Specifies the originating address of the push */
	JC_UINT8			ucHeader [1] ; /**< Specifies the push header */
} ST_TR_PUSH_HEADER_IND ;

/**
 * @brief The application needs to use this structure to send the push header indication response
 *		  event E_TR_PUSH_HEADER_IND_RESP_EVENT to the transport.
 */
typedef struct tr_push_header_ind_resp
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiPushID ; /**< Specifies the ID of the push message */
	JC_RETCODE			rCode ; /**< Specifies error if any while processing push header indication */
} ST_TR_PUSH_HEADER_IND_RESP ;

/**
 * @brief Once the transport receives push content it uses this structure while sending the
 *	      push content indication event E_TR_PUSH_CONTENT_IND_EVENT to the application.
 */
typedef struct tr_push_content_ind
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiPushID ; /**< Specifies the ID of push message */
	JC_UINT32			uiTotalLength ; /**< Specifies the length of the push content */
	JC_UINT32			uiCurrLength ; /**< Specifies the length of content sent in current chunk */
	JC_BOOLEAN			bHasMoreData ; /**< Specifies whether more data is to be received */
	E_HTTP_STATUS		eStatus ; /**< Specifies the status of the lower layer */
	JC_RETCODE			rCode ; /**< Specifies the error if any in transport while processing the push */
	JC_INT8				acContentType [MAX_CONTENT_TYPE_LEN + 1]; /**< Specifies the content type of the push */
	JC_UINT8			ucBuffer [1] ; /**< Specifies the Push Buffer */
} ST_TR_PUSH_CONTENT_IND ;

/**
 * @brief The application uses this structure while sending the push content indication response
 *		  event E_TR_PUSH_CONTENT_IND_RESP_EVENT to the application.
 */
typedef struct tr_push_content_ind_resp
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiPushID ; /**< Specifies the ID of the push message */
	JC_RETCODE			rCode ; /**< Specifies the error if any while processing the push content */
} ST_TR_PUSH_CONTENT_IND_RESP ;

/**
 * @brief The application needs to use this structure while sending the handle push SMS event
 *		  E_TR_SMS_PUSH_MSG event to the transport. The structure specifies the SMS content and
 *	      all the relevant information required for the WAPGSM to handle the SMS.
 */
typedef struct tr_push_sms_content
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_INT8				acSmeAddr [MAX_SME_ADDR_LEN + 1] ; /**< Specifies the SME address */
	JC_INT8				acOrigAddr [MAX_SME_ADDR_LEN + 1] ; /**< Specifies the originator address */
	JC_INT32			iPushLen ; /**< Specifies the length of the SMS push */
	JC_UINT8			ucPushMsg[1] ; /**< Specifies the SMS content */
} ST_TR_PUSH_SMS_CONTENT ;

/**
 * @brief The application needs to use this structure while sending the handle wap push event
 *		  E_TR_WAP_PUSH_MSG event to the transport. The structure specifies the push message content
 *        and all relevant push message information.
 */
typedef struct tr_wap_push_content
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_INT8				acSmeAddr [MAX_SME_ADDR_LEN + 1] ; /**< Specifies the SME address */
	JC_INT8				acOrigAddr [MAX_SME_ADDR_LEN + 1] ; /**< Specifies the originator address */
	JC_UINT32			uiBufferLen ; /**< Specifies the length of the push buffer */
	JC_INT8				acHeader [MAX_HEADER_LEN + 1] ; /**< Specifies the push header */
	JC_UINT8			ucBuffer [1] ; /**< Specifies the push buffer */
} ST_TR_WAP_PUSH_CONTENT ;

/**
 * @brief The application uses this structure to send the push register request event 
 *		  E_TR_PUSH_REGISTER_REQ_EVENT to the transport. The transport will register the 
 *		  application for the Push ID and the content type.
 */
typedef struct tr_push_register_req
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiPushAppID ; /**< Specifies the ID of the application which needs to registered */
	JC_INT8				acContentType [MAX_CONTENT_TYPE_LEN + 1] ; /**< Specifies the content type content type of the push messasge */
} ST_TR_PUSH_REGISTER_REQ ;

/**
 * @brief The transport uses this structure while sending the push register confirmation event 
 *		  E_TR_PUSH_REGISTER_RESP_EVENT to the application.
 */
typedef struct tr_push_register_cnf
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_RETCODE			rCode ; /**< Specifies error if any while processing register request */
} ST_TR_PUSH_REGISTER_CNF ;

#endif

/**
 * @brief The application needs to use this structure while sending the add root certificate event
 *		  E_TR_ADD_ROOT_CERTIFICATE to the transport. The transport will in turn add the certificate at 
 *		  the lower layers
 */
typedef struct tr_add_root_certificate
{
	JC_TASK_INFO	stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session. If this field is 0 then the certificate will be added to certificate pool which will be used by all the transport sessions */
	JC_UINT32		uiLength ; /**< Specifies the Length of the root certificate */
	JC_INT8			acContentType [MAX_CONTENT_TYPE_LEN + 1] ; /**< Specifies the content type of the root certificate */
	JC_UINT8		ucBuffer [1] ; /**< Specifies the root certificate content */
} ST_TR_ADD_ROOT_CERTIFICATE ;

/**
 * @brief The transport uses this structure while sending the add root certificate response event
 *		  E_TR_ADD_ROOT_CERTIFICATE_IND to the application.
 */
typedef struct tr_add_root_certificate_ind
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE			rCode ; /**< Specifies error if any while processing root certificate */
} ST_TR_ADD_ROOT_CERTIFICATE_IND ;

/**
 * @brief The application needs to use this structure while sending the add user certificate event
 *		  E_TR_ADD_USER_CERTIFICATE to the transport. The transport will in turn add the certificate at 
 *		  the lower layers
 */
typedef struct tr_add_user_certificate
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session. If this field is 0 then the certificate will be added to certificate pool which will be used by all the transport sessions */
	JC_UINT32			uiCertLen ; /**< Specifies the Length of the user certificate */
	JC_UINT32			uiKeyLen ; /**< Specifies the length of the key */
	JC_INT8				acContentType [MAX_CONTENT_TYPE_LEN + 1] ; /**< Specifies content type of the user certificate */
	JC_INT8				acPassword [MAX_CERT_PASSWORD_LEN + 1] ; /**< Specifies the password of the user certificate */
	JC_UINT8			ucBuffer [1] ; /**< holds both certificate and key */
} ST_TR_ADD_USER_CERTIFICATE ;

/**
 * @brief The transport uses this structure while sending the add user certificate response event
 *		  E_TR_ADD_USER_CERTIFICATE_IND to the application.
 */
typedef struct tr_add_user_certificate_ind
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE			rCode ; /**< Specifies error if any while processing user certificate */
} ST_TR_ADD_USER_CERTIFICATE_IND ;

/**
 * @brief The application needs to use this structure while sending the clear root certificate event
 *		  E_TR_CLEAR_ROOT_CERTIFICATE to the transport. The transport will in turn clear the certificate at 
 *		  the lower layers
 */
typedef struct tr_clear_root_certificate
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session. If this field is 0 then all the root certificates maintained in the common certificate pool will be cleared */
} ST_TR_CLEAR_ROOT_CERTIFICATE ;

/**
 * @brief The transport uses this structure while sending the clear root certificate response event
 *		  E_TR_CLEAR_ROOT_CERTIFICATE_IND to the application.
 */
typedef struct tr_clear_root_certificate_ind
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE			rCode ; /**< Specifies error if any while processing clear root certificate */
} ST_TR_CLEAR_ROOT_CERTIFICATE_IND ;

/**
 * @brief The application needs to use this structure while sending the clear user certificate event
 *		  E_TR_CLEAR_USER_CERTIFICATE to the transport. The transport will in turn clear the certificate at 
 *		  the lower layers
 */
typedef struct tr_clear_user_certificate
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session. If this field is 0 then all the user certificates maintained in the common certificate pool will be cleared */
} ST_TR_CLEAR_USER_CERTIFICATE ;

/**
 * @brief The transport uses this structure while sending the clear user certificate response event
 *		  E_TR_CLEAR_USER_CERTIFICATE_IND to the application.
 */
typedef struct tr_clear_user_certificate_ind
{
	JC_TASK_INFO		stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE			rCode ; /**< Specifies error if any while processing clear user certificate */
} ST_TR_CLEAR_USER_CERTIFICATE_IND ;

/**
 * @brief The following structure specifies the various options that can be specified at the lower
 *		  layer by the transport. The application needs to use this structure while sending the
 *		  E_TR_SET_OPTION event to the transport.
 */
typedef struct tr_set_option
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_BOOLEAN			bIsSIAEnabled ; /**< Not used */
	JC_BOOLEAN			bIsSIEnabled ; /**< Not used */
	JC_BOOLEAN			bIsSLEnabled ; /**< Not used */
	JC_BOOLEAN			bIsCOEnabled ; /**< Not used */
	JC_BOOLEAN			bIsOTAEnabled ; /**< Not used */
	JC_BOOLEAN			bIsSICEnabled ; /**< Not used */
	JC_BOOLEAN			bIsSLCEnabled ; /**< Not used */
	JC_BOOLEAN			bIsCOCEnabled ; /**< Not used */
	JC_BOOLEAN			bIsOTACEnabled ; /**< Not used */
	JC_INT32			iTimeOut ; /**< Specifies the timeout value for the lower layers */
	JC_INT32			iMaxRetries ; /**< Specifies the maximum number of retries in case of connect failure */
	JC_INT32			iMaxSockets ; /**< Specifies the maximum number of socket connections (maximum socket pools in http stack) 
										   that can be used by HTTP Stack at a time. */
	JC_INT32			iMaxTcpRecvBuf ; /**< Specifies the maximum buffer that can be read from the socket layer at a time 
											  (this will be used for jdd_NetRecv function).*/
	JC_INT32			iMaxSocketAliveTime ; /**< Specifies the maximum time till which an idle persistent HTTP socket that can be kept alive without closing.*/
} ST_TR_SET_OPTION ;

/**
 * @brief The following specifies the various socket events that can be sent to the transport
 *		  while sending the socket event structure (ST_TR_SOCKET_EVENT).
 */
typedef enum _tr_socket_event_type
{
	E_TR_SOCKET_LISTEN, /**< Specifies listen socket event */
	E_TR_SOCKET_CONNECT, /**< Specifies connect socket event */
	E_TR_SOCKET_READ, /**< Specifies read socket event */
	E_TR_SOCKET_WRITE, /**< Specifies write socket event */
	E_TR_SOCKET_ACCEPT, /**< Specifies accept socket event  */
	E_TR_SOCKET_CLOSE /**< Specifies close socket event */
} E_TR_SOCKET_EVENT_TYPE ;

/**
 * @brief The following structure specifies the security class that has failed
 *		  while sending the transport sends the ST_TR_SECURITY_IND_RESP structure to the applicaiton.
 */
typedef enum _tr_security_class
{	
	E_TR_CLASS_II, /**< Specifies that security class is class II */
	E_TR_CLASS_III /**< Specifies that security class is class III */
} E_SECURITY_CLASS ;

/**
 * @brief If any socket layer agent or application needs to send the socket state to the transport
 *		  then the following structure needs to be sent while sending the socket request event
 *		  E_TR_SOCKET_EVENT.
 */
typedef struct _tr_set_socket_event
{
	JC_TASK_INFO			stIPTaskInfo ; /**< Specifies the IP task info */
	E_TR_SOCKET_EVENT_TYPE	eSocketEvent ; /**< Specifies the socket event type */
	JC_INT32				iSocketID ; /**< Specifies the socket ID */
	JC_RETCODE				rCode ; /**< Specifies the return code of the socket layer */
	JC_UINT32				uiDataAvailable ; /**< Specifies the length of the buffer that can be read from the socket (only for read event and only if the uiBufferLen is set to zero.) */
	JC_UINT32				uiBufferLen ; /**< Specifies the length of the buffer that is read from the socket (if ucBuffer is available) */
	JC_UINT8				ucBuffer [1] ;  /**< Specifies the data that is read from the socket (only for socket read event) */
} ST_TR_SOCKET_EVENT ;

/**
 * @brief If any DNS layer agent or application needs to send the DNS resolved statefs to the transport
 *		  then the following structure needs to be sent while sending the DNS event
 *		  E_TR_DNS_EVENT.
 */
typedef struct _tr_dns_event
{
	JC_TASK_INFO		stDNSTaskInfo ; /**< Specifies the DNS agent task info */
	JC_UINT8			aucHostName [TR_MAX_HOSTNAME_SIZE + 1] ; /**< Specifies the host address */
	JC_UINT8			aucIPAddress [TR_MAX_HOSTNAME_SIZE + 1] ; /**< Specifies the resolved IP address */
	JC_RETCODE			rCode ; /**< Specifies the error code from DNS agent */
} ST_TR_DNS_EVENT ;

/**
 * @brief If any data connection layer agent or application needs to send the data connection status
 *        in the form of events then the following structure needs to be sent while sending the 
 *		  handle data connection event E_TR_DC_EVENT.
 */
typedef struct _tr_dc_event
{
	JC_TASK_INFO			stDCTaskInfo ; /**< Specifies the DC task info */
	JC_UINT32				uiConnectionID ; /**< Specifies the connection ID */
	E_DATA_CONN_STATUS		eStatus ; /**< Specifies the status of the data connection layer */
	JC_RETCODE				rCode ; /**< Specifies the error code of the data connection layer */
} ST_TR_DC_EVENT ;

/**
 * @brief The transport uses this structure while sending the security indication event E_TR_SECURITY_IND_REQ_EVENT
 *		  to the application. The structure specifies the various security parameters such as the
 *		  cipher suites, key exchange and certificates.
 */
typedef struct _tr_security_indication_req
{
	JC_TASK_INFO			stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32				uiRequestID ; /**< Specifies the ID of the request for which the security is established */
	JC_BOOLEAN				bIsWTLS ; /**< Specifies whether the security indication is for WTLS or SSL */
#ifdef WAP_WITH_WTLS
	E_WTLS_CIPHER_SUITE		eWTLSCipherSuites ; /**< Specifies the WTLS cipher suite */
	E_WTLS_KEY_EXCHANGE_ALG	eWTLSKeyExAlgs ; /**< Specifies the WTLS key exchange algorithm */
#endif
#ifdef HTTP_WITH_SSL
	E_SSL_CIPHER_SUITE		eSSLCipherSuites ; /**< Specifies the SSL cipher suite */
#endif
	JC_BOOLEAN				bIsValid ; /**< Reserved for future purpose */
	JC_UINT32				uiExtendedError ; /**< Specifies an extended error if any while establishing security */
	JC_UINT32				uiTotalCertificate ; /**< Specifies the total number of certificates negotiated */
	JC_UINT32				uiLength ; /**< Specifies the length of the first certificate content */
	JC_UINT8				ucBuffer [1] ; /**< Specifies the first certificate content */
} ST_TR_SECURITY_IND_REQ ;

/**
 * @brief The application needs to use the following structure while sending the security indication 
 *        response E_TR_SECURITY_IND_RESP_EVENT to the transport. The field bIsProceed will specify
 *		  whether the fetch at the transport needs to be continued.
 */
typedef struct _tr_security_indication_response
{
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_BOOLEAN				bIsProceed ; /**< Specifies whether the application wants to proceed with the security */
} ST_TR_SECURITY_IND_RESP ;

/**
 * @brief The transport uses this structure while sending the security failure indication event
 *		  E_TR_SECURITY_FAILURE_IND_EVENT to the application. The structure specifies the 
 *		  class of security that has failed.
 */
typedef struct _tr_security_failure_ind
{
	JC_TASK_INFO			stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32				uiRequestID; /**< Specifies the ID of the request for which the security session was requested */
	E_SECURITY_CLASS		eSecurityClass ; /**< Specifies the class of security which has failed */
	JC_UINT32				uiSecurityError ; /**< Specifies the CERTICOM security error */
} ST_TR_SECURITY_FAILURE_IND ;

/**
 * @brief The application needs to use this structure while sending the security failure response
 *		  event E_TR_SECURITY_FAILURE_RESP_EVENT to the transport. The structure specifies whether
 *		  the application wants to proceed with the next class of security.
 */
typedef struct _tr_security_failure_resp
{	
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_BOOLEAN				bSetNextSecurityClass ; /**< Specifies whether the application wants to proceed with next class of security */
} ST_TR_SECURITY_FAILURE_RESP ;

/**
 * @brief The application needs to use this structure while sending the certificate request
 *		  event E_TR_CERTIFICATE_MORE_REQ_EVENT to the transport. The structure specifies the 
 *		  index of the certificate that the application needs.
 */
typedef struct _tr_certificate_req
{
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32				uiRequestID ; /**< Specifies the request ID for which the security session is established and the certificates are in turn received */
	JC_UINT32				uiIndex ; /**< Starts from zero **/ /**< Specifies the Index number of certificate that is required */
} ST_TR_CERTIFICATE_REQ ;

/**
 * @brief The transport uses this structure while sending the certificate request's response 
 *		  event E_TR_CERTIFICATE_MORE_RESP_EVENT to the application.
 */
typedef struct _tr_certificate_resp
{
	JC_TASK_INFO			stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
    JC_UINT32				uiRequestID ; /**< Specifies the request ID for which the security session is established and the certificates are in turn received */
	JC_UINT32				uiIndex ; /**< Specifies the index number of the certificate being sent */
	JC_UINT32				uiLength ; /**< Specifies the length of the certificate content */
	JC_RETCODE				rCode ; /**< Specifies error if any in handling the certificate request */
	JC_UINT8				ucBuffer [1] ; /**< Specifies the certificate content */	
} ST_TR_CERTIFICATE_RESP ;

/**
 * @brief The application needs to use this structure while sending the suspend fetch request
 *		  event E_TR_SUSPEND_FETCH_REQ_EVENT to the transport. The structure specifies the 
 *		  ID of the request to be suspended.
 */
typedef struct _tr_suspend_fetch_req
{
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32				uiRequestID ; /**< Specifies the ID of the request for which the suspend request is sent */
} ST_TR_SUSPEND_FETCH_REQ ;

/**
 * @brief The transport uses this structure while sending the E_TR_SUSPEND_FETCH_RESP_EVENT event 
 *		  to the application.
 */
typedef struct _tr_suspend_fetch_resp
{
	JC_TASK_INFO			stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task Session */
	JC_UINT32				uiRequestID ; /**< Specifies the ID of the request for which the suspend fetch response is sent */
	JC_RETCODE				rCode ; /**< Specifies error if any in handling the suspend fetch request */
} ST_TR_SUSPEND_FETCH_RESP ;

/**
 * @brief The application needs to use this structure while sending the resume fetch request
 *		  event E_TR_RESUME_FETCH_REQ_EVENT to the transport. The structure specifies the 
 *		  ID of the request to be resumed.
 */
typedef struct _tr_resume_fetch_req
{
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task information */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task Session */
	JC_UINT32				uiRequestID ; /**< Specifies the ID of the request which needs to be resumed */
} ST_TR_RESUME_FETCH_REQ ;

/**
 * @brief The transport uses this structure while sending the E_TR_RESUME_FETCH_RESP_EVENT event 
 *		  to the application.
 */
typedef struct _tr_resume_fetch_resp
{
	JC_TASK_INFO			stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task Session */
	JC_UINT32				uiRequestID ; /**< Specifies the ID of the request for which the resume fetch response is sent */
	JC_RETCODE				rCode ; /**< Specifies error if any in handling the resume fetch request */
} ST_TR_RESUME_FETCH_RESP ;

/**
 * @brief The application or any module which wants to invoke the transport timer using the E_TR_TIMER_EVENT
 *		  event needs to use this structrue.
 */
typedef struct _tr_timer_event
{
	JC_UINT32				uiTimerID ; /**< Specifies the timer value */
	void					*pvArg ; /**< Specifies the timer callback arguement */
} ST_TR_TIMER_EVENT ;

#endif

/* END OF FILE */


