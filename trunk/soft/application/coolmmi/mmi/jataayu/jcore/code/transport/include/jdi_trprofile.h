/***************************************************************************
 * $Id: jdi_trprofile.h,v 1.10 2008/08/14 08:25:27 browserdevjtbase Exp $
 * $Revision: 1.10 $
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
 * $Log: jdi_trprofile.h,v $
 * Revision 1.10  2008/08/14 08:25:27  browserdevjtbase
 * Core Updates
 *
 * Revision 1.9  2008/07/31 08:10:48  browserdevjtbase
 * Core Updates
 *
 * Revision 1.8  2007/11/14 08:52:37  browserdevjtbase
 * core update.
 *
 * Revision 1.7  2007/09/27 14:52:23  browserdevjtbase
 * Core Update
 *
 * Revision 1.6  2007/05/23 12:29:50  browserdevjtbase
 * Core update
 *
 * Revision 1.5  2007/04/10 15:31:02  browserdevjtbase
 * Update ...
 *
 * Revision 1.4  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.3  2006/06/16 06:58:58  browserdevjtbase
 * Updated with email protocol
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.7  2006/01/17 14:39:12  kumardevhtmlbrow
 * Updated the comments ...
 *
 * Revision 1.6  2006/01/09 14:45:52  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.4  2005/06/04 07:56:52  kumardevhtmlbrow
 * updated the transport task ...
 *
 * Revision 1.3  2005/05/21 11:37:31  shreyasdevhtmlbrow
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
 * @file jdi_trprofile.h
 * @ingroup Transport Task.
 * @brief Defines enums and structures used for profile handling.
 *
 * This file defines the enums and strcuture that are to be used to send a profile request
 * to the transport and receive the response of the profile request.
 */

#ifndef _JDI_TRPROFILE_H
#define _JDI_TRPROFILE_H

#include <jdi_transport.h>

/*** Type Definitions	***/

/**
 * @brief The enumneration defines the various connection modes the application can specify
 *		  to the transport while sending the profile request (ST_TR_PROFILE_REQ) .
 */
typedef enum tr_connection_mode
{
#ifdef WSP_STACK_ENABLE
	E_TR_WSP_CONNECTION_ORIENTED, /**< Specifies connection oriented mode */
	E_TR_WSP_CONNECTION_ORIENTED_SECURE, /**< Specifies secure connection oriented mode */
#endif
#ifdef HTTP_STACK_ENABLE
	E_TR_HTTP_PROXY, /**< Specifies HTTP proxy connection mode */
	E_TR_HTTP_DIRECT, /**< Specifies Direct Connection mode */
#endif
#ifdef EMAIL_PROTOCOL_ENABLE
	E_TR_DATA_CONN_MODE,
	E_TR_SMTP_DIRECT,
	E_TR_POP3_DIRECT,
	E_TR_IMAP4_DIRECT,
#endif
	E_TR_DUMMY_MODE
} E_TR_CONNECTION_MODE ;

/**
 * @brief The enumneration defines the various profile confirmation types that can be sent to the
 *		  application 
 */
typedef enum tr_profile_confirmation_type
{
	E_TR_USE_SECONDARY_CONNECTION_MODE, /**< Specifies that confirmation is required to use secondary connection mode. */
	E_TR_USE_SECONDARY_DATA_CONNECTION, /**< Specifies that confirmation is required to use secondary data connection */
	E_TR_USE_TERTIARY_DATA_CONNECTION, /**< Specifies that confirmation is required to use tertiary data connection */
	E_TR_EX_PROFILE_RETRY /**< Specifies that confirmation is required to start the whole extended profile once again. */
} E_TR_PROFILE_CONFIRMATION_TYPE ;

/**
 * @brief The enumeration defines the various value that can be specified for the duplicate profile information
 * in the extended profile. 
 */
typedef enum tr_profile_option
{
	E_TR_CONNECTION_NOT_AVAILABLE, /**< Specifies the connection information is not available. */
	E_TR_CONNECTION_USER_CONFIRMATION, /**< Specifies the connection information should be used only after the user confirmation. */
	E_TR_CONNECTION_PROCEED /**< Specifies the connection information can be used without the user confirmation. */
} E_TR_PROFILE_OPTION ;

/**
 * @brief The application needs to use this structure while sending the profile request 
 *		  E_TR_PROFILE_REQ_EVENT event to the transport. 
 */
typedef struct tr_profile_req
{
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_UINT32				uiPortNumber ; /**< Specifies the port number */
	JC_UINT32				uiSecurePort ; /**< This is used only for proxy connection. This is used for socket connection instead of uiPortNum for secure requests */
	E_TR_CONNECTION_MODE	eTrConnMode ; /**< Specifies the connection mode */
	JC_INT8					acHostName [TR_MAX_HOSTNAME_SIZE + 1]; /**< Specifies the host name */
	JC_BOOLEAN				bUseExistingBearer ; /**< Specifies whether an existing bearer can be used */
	JC_BOOLEAN				bDeleteSessionHeaders ; /**< Specifies whether the existing session headers of 
														 the session has to be deleted when the profile is being changed*/
	ST_GENERIC_DATA_CONN	stDataConnection ; /**< Specifies the data connection information */
} ST_TR_PROFILE_REQ ;

/**
 * @brief The application needs to use this structure while sending the profile request 
 *		  E_TR_EXTENDED_PROFILE_REQ_EVENT event to the transport.
 */
typedef struct tr_extended_profile_req
{
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_BOOLEAN				bDeleteSessionHeaders ; /**< Specifies whether the existing session headers of 
													 the session has to be deleted when the profile is being changed*/

	E_TR_CONNECTION_MODE	ePrimaryConnectionMode ; /**< Specifies the primary connection mode */
	JC_INT8					acPrimaryHostName [TR_MAX_HOSTNAME_SIZE + 1] ; /**< Specifies the primary host name */
	JC_UINT32				uiPrimaryPortNum ; /**< Specifies the primary port number */

	E_TR_PROFILE_OPTION		eSecondaryConnOption ; /**< Specifies whether the lower layer can use secondary host and port if primary fails.*/
	E_TR_CONNECTION_MODE	eSecondaryConnectionMode ; /**< Specifies the secondary connection mode */
	JC_INT8					acSecondaryHostName [TR_MAX_HOSTNAME_SIZE + 1] ; /**< Specifies the secondary host name */
	JC_UINT32				uiSecondaryPortNum ; /**< Specifies the secondary port number */

	ST_GENERIC_DATA_CONN	stPrimaryDataConn ; /**< Specifies the Primary data connection */

	E_TR_PROFILE_OPTION		eSecondaryDataConnOption ; /**< Specified whether the secondary data connection can be used without user intervention
														    if the primary data connection fails. */
	ST_GENERIC_DATA_CONN	stSecondaryDataConn ; /**< Specifies the Secondary data connection */

	E_TR_PROFILE_OPTION		eTertiaryDataConnOption ; /**< Specified whether the tertiary data connection can be used without user intervention
																	  if the secondary data connection fails. */
	ST_GENERIC_DATA_CONN	stTertiaryDataConn ; /**< Specifies the Tertiary data connection */
} ST_TR_EXTENDED_PROFILE_REQ ;

/**
 * @brief The transport uses this structure while sending the profile confirmation request event
 *		  E_TR_PROFILE_CONFIRMATION_REQ to the application.
 */
typedef struct tr_profile_confirmation_req
{
	JC_TASK_INFO					stTransportTaskInfo ; /**< Specifies the transport task info */
	JC_UINT32						uiSessionID ; /**< Specifies the ID of the transport task session */
	E_TR_PROFILE_CONFIRMATION_TYPE	eProfileConfirmationType ; /**<Specifies the type of profile confirmation requested */
} ST_TR_PROFILE_CONFIRMATION_REQ ;

/**
 * @brief The application uses this structure while sending the profile confirmation response event
 *		  E_TR_PROFILE_CONFIRMATION_RESP to the transport.
 */
typedef struct tr_profile_confirmation_resp
{
	JC_TASK_INFO		stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32			uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_BOOLEAN			bProceed ; /**< Specifies whether the application wants the transport to proceed with the profile option */
} ST_TR_PROFILE_CONFIRMATION_RESP ;


/**
 * @brief Once the transport has processed the profile request it uses the following structure to send
 *		  the profile request response event E_TR_PROFILE_RESP_EVENT to the application.
 */
typedef struct tr_profile_resp
{
	JC_TASK_INFO			stTransportTaskInfo ; /**< Specifies the transport task information */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	JC_RETCODE				rCode ; /**< Specifies error if any while processing the profile request */
} ST_TR_PROFILE_RESP ;

#ifdef IP_STACK_ENABLE

/**
 * @brief The application needs to use this enum to specify the IP connection mode.
 */
typedef enum tr_ip_connection_mode
{
	E_TR_TCP_MODE, /**< Specifies the TCP connection mode. */
	E_TR_UDP_MODE /**< Specifies the UDP connection mode. */
} E_TR_IP_CONNECTION_MODE ;

/**
 * @brief The application needs to use this structure to specify the IP profile.
 */
typedef struct tr_ip_profile_req
{
	JC_TASK_INFO			stClientTaskInfo ; /**< Specifies the client (application) task info */
	JC_UINT32				uiSessionID ; /**< Specifies the ID of the transport task session */
	E_TR_IP_CONNECTION_MODE	eTrIPConnMode ; /**< Specifies the connection mode */
	JC_INT8					acIPHostName [TR_MAX_HOSTNAME_SIZE + 1] ; /**< Specifies the host IP */
	JC_UINT32				uiIPPortNumber ; /**< Specifies the port number (for the Host IP). */
	JC_INT8					acProxyHostName [TR_MAX_HOSTNAME_SIZE + 1] ; /**< Specifies the proxy IP */
	JC_UINT32				uiProxyPortNumber ; /**< Specifies the port number (for the proxy) */
    JC_BOOLEAN				bUseExistingBearer ;  /**< Specifies whether to use existing bearer connection */
	ST_GENERIC_DATA_CONN	stDataConnection ; /**< Specifies the data connection information */
} ST_TR_IP_PROFILE_REQ ;	

#endif

#endif

/* END OF FILE */


