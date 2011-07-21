/***************************************************************************
 * $Id: jdi_trinit.h,v 1.8 2008/11/18 07:44:09 browserdevjtbase Exp $
 * $Revision: 1.8 $
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
 * $Log: jdi_trinit.h,v $
 * Revision 1.8  2008/11/18 07:44:09  browserdevjtbase
 * Core updates
 *
 * Revision 1.7  2007/06/26 12:43:33  browserdevjtbase
 * Updated
 *
 * Revision 1.6  2007/04/10 15:31:02  browserdevjtbase
 * Update ...
 *
 * Revision 1.5  2006/11/22 10:53:10  browserdevjtbase
 * Update
 *
 * Revision 1.4  2006/06/20 13:28:37  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.3  2006/06/16 06:58:58  browserdevjtbase
 * Updated with email protocol
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.11  2006/03/16 07:35:41  kumardevhtmlbrow
 * Added the application to add the session header
 *
 * Revision 1.10  2006/01/09 14:45:52  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.9  2005/12/31 12:15:36  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2005/12/30 13:54:23  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2005/10/27 15:55:20  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2005/10/27 14:17:35  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.4  2005/08/09 12:18:43  kavithadevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/07/22 14:31:03  kumardevhtmlbrow
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
 * @file jdi_trinit.h
 * @ingroup Transport Task.
 * @brief Defines datatypes used for transport.
 *
 * Defines the structures that are used to initialize and deinitialize
 * the transport.
 */

#ifndef _JDI_TRINIT_H
#define _JDI_TRINIT_H

#include <jdi_transport.h>
#include <jdi_trprofile.h> 

typedef enum _tr_protocol_type
{
	E_HTTP_WSP_PROTOCOL_STACK,
	E_MAIL_PROTOCOL_STACK
} E_PROTOCOL_TYPE ;


/**
 * @brief The application needs to use this structure while initializing the transport
 *		  task Session (E_TR_INITIALIZE_REQ_EVENT).
 */
typedef struct tr_initialize_req
{
	JC_INT32			iMaxRequests ; /**< Specfies the maximum requests that can be handles in transport at a time */
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
#ifdef TR_MULTIPLE_PROTOCOL
	E_PROTOCOL_TYPE		eProtocolType ;
#endif
} ST_TR_INITIALIZE_REQ ; 

/**
 * @brief The transport uses this structure while sending the response of the transport initialize function 
 *		  (E_TR_INITIALIZE_RESP_EVENT) to the application. The application will receive the session ID
 *		  of the transport task session that is initialized. The application needs to use this ID
 *		  while sending other events to this transport.
 */
typedef struct tr_initialize_resp
{
	JC_TASK_INFO	stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_TASK_INFO	stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session which is initialized */
	JC_RETCODE		rCode ;	 /**< Specifies error if any while initializing the transport task session */
} ST_TR_INITIALIZE_RESP ;

/**
 * @brief The application needs to use this structure while sending the tranport task session deinitilaize
 *		  event E_TR_DEINITIALIZE_REQ_EVENT to the transport.
 */
typedef struct tr_deinitialize_req
{
	JC_TASK_INFO	stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session which needs to deinitialized */
	JC_BOOLEAN		bIsResponseRequired ; /**< Specifies whether the application needs the response for deinitialize request */
} ST_TR_DEINITIALIZE_REQ ; 

/**
 * @brief The transport uses this structure while sending the transport task session deinitialize response
 *		  event E_TR_DEINITIALIZE_RESP_EVENT to the application. 
 */
typedef struct tr_deinitialize_resp
{
	JC_TASK_INFO	stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE		rCode ; /**< Specifies error if any while deintializing the transport task session */
} ST_TR_DEINITIALIZE_RESP ;

/**
 * @brief The application needs to use this structure while sending the disconnect data connection
 *	      request event E_TR_DISCONNECT_REQ_EVENT to the transport. The transport will disconnect
 *		  the data connection used by the task session with ID uiSessionID
 */
typedef struct tr_disconnect_req
{
	JC_TASK_INFO	stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session whose data connection is to be disconnected */
#ifdef EMAIL_PROTOCOL_ENABLE
	JC_UINT32				uiProfileID ;
	E_TR_CONNECTION_MODE	eTrConnMode ;
#endif /* EMAIL_PROTOCOL_ENABLE **/

} ST_TR_DISCONNECT_REQ ;

/**
 * @brief The transport uses this structure while sending the disconnect response event E_TR_DISCONNECT_RESP_EVENT
 *		  to the application.
 */
typedef struct tr_disconnect_resp
{
	JC_TASK_INFO	stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE		rCode ; /**< Specifies error if any while processing the disconnect request */
#ifdef EMAIL_PROTOCOL_ENABLE
	JC_TASK_INFO			stClientTaskInfo ;
	JC_UINT32				uiProfileID ;
	E_TR_CONNECTION_MODE	eTrConnMode ;
#endif /** EMAIL_PROTOCOL_ENABLE **/

} ST_TR_DISCONNECT_RESP ;

/**
 * @brief The application has to use this structure to send session header to the transport. The transport
 *		  will send the session header while initializing the lower layers.
 */
typedef struct tr_session_header_req
{
	JC_TASK_INFO	stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_BOOLEAN		bHasMoreHeader ; /**< Specifies whether more header is to be sent from the client */
	JC_INT8			acCharset [TR_MAX_CHARSET_SIZE + 1] ; /**< Specifies the charset information */
	JC_INT8			acName [TR_MAX_HEADER_NAME_SIZE + 1] ; /**< Specifies the header name if any */
	JC_UINT8		ucValue [1] ; /**< Specifies the header value */
} ST_TR_SESSION_HEADER_REQ ;

/**
 * @brief The transport uses this structure while sending the session header response event E_TR_SESSION_HEADER_RESP_EVENT
 *		  to the application.
 */
typedef struct tr_session_header_resp
{
	JC_TASK_INFO	stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE		rCode ; /**< Specifies error if any while processing the session header request */
} ST_TR_SESSION_HEADER_RESP ;

/**
 * @brief The application needs to use this structure while sending the suspend transport task session
 *		  event E_TR_SUSPEND_SESSION_REQ_EVENT to the transport. The transport will suspend all the
 *		  operations of the transport task session whose ID is uiSessionID.
 */
typedef struct tr_suspend_session_req
{
	JC_TASK_INFO	stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session that needs to be suspended */
} ST_TR_SUSPEND_SESSION_REQ ;

/**
 * @brief The transport uses this structure while sending the suspend transport task session response
 *		  event E_TR_SUSPEND_SESSION_RESP_EVENT to the application.
 */
typedef struct tr_suspend_session_resp
{
	JC_TASK_INFO	stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE		rCode ; /**< Specifies error if any while processing suspend session request */
} ST_TR_SUSPEND_SESSION_RESP ;

/**
 * @brief The application needs to use this structure while sending the resume transport task session
 *		  event E_TR_RESUME_SESSION_REQ_EVENT to the transport. The transport will resume all the
 *		  operations of the transport task session whose ID is uiSessionID provided the task session
 *		  is currently in suspended state
 */
typedef struct tr_resume_session_req
{
	JC_TASK_INFO	stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session that needs to be resumed */
} ST_TR_RESUME_SESSION_REQ ; 

/**
 * @brief The transport uses this structure while sending the resume transport task session response
 *		  event E_TR_RESUME_SESSION_RESP_EVENT to the application.
 */
typedef struct tr_resume_session_resp
{
	JC_TASK_INFO	stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE		rCode ; /**< Specifies error if any while processing the resume session request */
} ST_TR_RESUME_SESSION_RESP ;

#ifndef TRANSPORT_DISABLE_LOW_MEM_EVENT
/**
 * @brief The transport uses this structure while sending low memory indication event 
 *		  E_TR_LOW_MEMORY_IND_EVENT to the application.
 */
typedef struct tr_lowmem_ind
{
	JC_TASK_INFO	stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE		rCode ; /**< Specifies Specific memory related error if any */
} ST_TR_LOWMEM_IND ;
#endif

/**
 * @brief While sending the push session initialize request (ST_TR_PUSH_INITIALIZE_REQ) the
 *		  application needs to specify the type of push session to be initialized. The following 
 *		  enum defines the various push types supported at the transport layer.
 */
typedef enum _tr_push_type
{
	E_TR_PUSH_OVER_SMS, /**< Specifies that push session is of type SMS Push */
	E_TR_PUSH_UNCONFIRMED, /**< Specifies that push session is of type Unconfirmed push */
	E_TR_PUSH_CONFIRMED, /**< Specifies that push session is of type confirmed push */
	E_TR_HTTP_PUSH /**< Specifies that push session is of type HTTP push */
} E_TR_PUSH_TYPE ; 

/**
 * @brief The application needs to use this structure while sending the push initiaze request event
 *		  E_TR_PUSH_INITIALIZE_REQ_EVENT to the transport. The structure will specify the type of 
 *		  push session and various other details such as the port number, host name etc.
 */
typedef struct tr_push_initialize_req
{
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task info */
	E_TR_PUSH_TYPE			ePushType ; /**< Specifies the type of push session to be initialized */
	JC_INT8					acHostName [TR_MAX_HOSTNAME_SIZE + 1]; /**< Specifies the host name to which connect should be done */
	JC_UINT16				uhPortNumber ; /**< Specifies the port number to which the transport needs to listen */
	JC_BOOLEAN				bUseExistingBearer ; /**< Specifies whether an existing bearer (if any) should be used for push */
	ST_GENERIC_DATA_CONN	stDataConnection ; /**< Specifies the data connection to be used for push connection */
} ST_TR_PUSH_INITIALIZE_REQ ;

/**
 * @brief The transport uses this structure while sending the push initialize response event 
 *		  E_TR_PUSH_INITIALIZE_RESP_EVENT to the application. The structure will specify the 
 *	      ID of the push session created. The application needs to use this session ID while
 *		  processing push related operations.
 */
typedef struct tr_push_initialize_resp
{
	JC_TASK_INFO	stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_TASK_INFO	stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the push session */
	JC_RETCODE		rCode ;	 /**< Specifies error if any while processing the push initialize request */
} ST_TR_PUSH_INITIALIZE_RESP ;

/**
 * @brief The application needs to use this structure while sending the push deinitiaze request event
 *		  E_TR_PUSH_DEINITIALIZE_REQ_EVENT to the transport. The structure will specify the ID
 *		  of push session to be deinitialized.
 */
typedef struct tr_push_deinitialize_req
{
	JC_TASK_INFO	stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the push session to be deintialized */
	JC_BOOLEAN		bIsResponseRequired ; /**< Specifies whether the application requires a response for the deinitialize request */
} ST_TR_PUSH_DEINITIALIZE_REQ ;

/**
 * @brief The transport uses this structure while sending the push deinitialize response event 
 *		  E_TR_PUSH_DEINITIALIZE_RESP_EVENT to the application.
 */
typedef struct tr_push_deinitialize_resp
{
	JC_TASK_INFO	stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32		uiSessionID ; /**< Specifies the ID of the push session */
	JC_RETCODE		rCode ; /**< Specifies error if any while processing the deinitialize request */
} ST_TR_PUSH_DEINITIALIZE_RESP ;

#endif

/* END OF FILE */

