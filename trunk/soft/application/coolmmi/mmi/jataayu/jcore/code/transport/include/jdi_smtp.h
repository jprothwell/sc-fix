/***************************************************************************
 * $Id: jdi_smtp.h,v 1.17 2009/05/29 12:00:18 browserdevjtbase Exp $
 * $Revision: 1.17 $
 * $Date: 2009/05/29 12:00:18 $
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
 * $Log: jdi_smtp.h,v $
 * Revision 1.17  2009/05/29 12:00:18  browserdevjtbase
 * jMail Core Update
 *
 * Revision 1.2  2009/05/15 12:20:00  basakccums
 * Chm document updated with new logo, header & footer.
 *
 * Revision 1.1  2009/05/14 07:22:48  basakccums
 * Messaging 5.0 from jEmulator to UMS
 *
 * Revision 1.21  2009/02/09 04:16:25  manikumardevjemulator
 * *** empty log message ***
 *
 * Revision 1.20  2008/06/16 13:24:10  manikumardevjemulator
 * Build 39 updates
 *
 * Revision 1.19  2008/04/09 11:49:53  manikumardevjemulator
 * *** empty log message ***
 *
 * Revision 1.18  2008/04/08 13:46:35  manikumardevjemulator
 * Code Review Updates
 *
 * Revision 1.17  2008/03/28 15:45:01  manikumardevjemulator
 * Build 35 updates
 *
 * Revision 1.16  2008/03/14 15:09:28  manikumardevjemulator
 * Build 34 updates
 *
 * Revision 1.15  2008/01/18 15:48:37  manikumardevjemulator
 * *** empty log message ***
 *
 * Revision 1.11  2007/11/14 08:52:37  browserdevjtbase
 * core update.
 *
 * Revision 1.10  2007/09/17 06:56:07  browserdevjtbase
 * Update.
 *
 * Revision 1.11  2007/07/31 14:06:01  manikumardevjemulator
 * jtbase updates
 *
 * Revision 1.10  2007/07/03 05:49:32  manikumardevjemulator
 * *** empty log message ***
 *
 * Revision 1.8  2007/05/14 09:25:57  browserdevjtbase
 * Mail Update.
 *
 * Revision 1.7  2007/04/03 06:43:27  browserdevjtbase
 * Mail Update
 *
 * Revision 1.6  2007/03/02 13:44:27  browserdevjtbase
 * Core Update
 *
 * Revision 1.5  2007/02/06 10:09:59  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.4  2007/01/16 12:45:20  browserdevjtbase
 * Mail source update
 *
 * Revision 1.3  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.2  2006/07/03 07:26:11  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.1  2006/06/20 13:28:37  browserdevjtbase
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_smtp.h
 * @ingroup SMTP 
 * @brief Defines the exposed enumerations, structures and APIs of the smtp that
 *		  can be used by the transport.
 */

#ifndef _JDI_SMTP_H
#define _JDI_SMTP_H

#ifdef __cplusplus
extern "C"
{
#endif



#include <ddl.h>

#ifdef SMTP_STACK_ENABLE
#include <jcal.h>
#include <jdi_transportmodule.h>

#ifdef SMTP_WITH_SSL
#include <jdi_ssl.h>
#endif

/**
 * @brief Specifies the maximum error array size
 */
#define MAX_ERROR_ARRAY_SIZE			30
/**
 * @brief Specifies the smtp callback.
 */
typedef JC_INT32 (*JL_SMTP_CALLBACK) (JC_INT32) ;

/**
 * @brief Specifies the error code to indicate that maximum pofiles added
 */
#define	JC_ERR_SMTP_MAX_PROFILES_ADDED		TERMINAL_ERR_CODE( SMTP_MODULE, 0x01)				
/**
 * @brief Specifies the error code to indicate that profile is already exists.
 */
#define JC_ERR_SMTP_PROFILE_ALREADY_EXISTS	TERMINAL_ERR_CODE( SMTP_MODULE, 0x02)
/**
 * @brief Specifies the error code to indicate that error in adding profile
 */
#define JC_ERR_SMTP_ADD_PROFILE				TERMINAL_ERR_CODE( SMTP_MODULE, 0x03)
/**
 * @brief Specifies the error code to indicate that error in deleting profile
 */
#define JC_ERR_SMTP_DELETE_PROFILE			TERMINAL_ERR_CODE( SMTP_MODULE, 0x04)				
/**
 * @brief Specifies the error code to indicate that error in getting profile
 */
#define JC_ERR_SMTP_GET_PROFILE				TERMINAL_ERR_CODE( SMTP_MODULE, 0x05)
/**
 * @brief Specifies the error code to indicate that smtp profile is not exists
 */
#define JC_ERR_SMTP_PROFILE_DOES_NOT_EXIST	TERMINAL_ERR_CODE( SMTP_MODULE, 0x06)				

/**
 * @brief Specifies the error code to indicate that smtp is busy.
 */
#define JC_ERR_SMTP_BUSY					TERMINAL_ERR_CODE( SMTP_MODULE, 0x07)
/**
 * @brief Specifies the error code to indicate that error in smtp connection 
 */
#define JC_ERR_SMTP_CONNECT					TERMINAL_ERR_CODE( SMTP_MODULE, 0x08)				
/**
 * @brief Specifies the error code to indicate that error in smtp disconnection
 */
#define JC_ERR_SMTP_DISCONNECT				TERMINAL_ERR_CODE( SMTP_MODULE, 0x09)				
/**
 * @brief Specifies the error code to indicate that smtp user authentication error.
 */
#define JC_ERR_SMTP_AUTHENTICATE_USER		TERMINAL_ERR_CODE( SMTP_MODULE, 0x0A)				
/**
 * @brief Specifies the error code to indicate that smtp pass error
 */
#define JC_ERR_SMTP_AUTHENTICATE_PASS		TERMINAL_ERR_CODE( SMTP_MODULE, 0x0B)				
/**
 * @brief Specifies the error code to indicate that authentication failure.
 */
#define JC_ERR_SMTP_AUTHENTICATION			TERMINAL_ERR_CODE( SMTP_MODULE, 0x0C)					
/**
 * @brief Specifies the error code to indicate that authentication is not supported.
 */
#define JC_ERR_SMTP_AUTHENTICATION_UNSUPPORTED 					TERMINAL_ERR_CODE( SMTP_MODULE, 0x0D)
/**
 * @brief Specifies the error code to indicate that authentication mechanism is not supported.
 */
#define JC_ERR_SMTP_AUTH_MECHANISM_UNSUPPORTED 					TERMINAL_ERR_CODE( SMTP_MODULE, 0x0E)
/**
 * @brief Specifies the error code to indicate that mail box is locked.
 */
#define JC_ERR_SMTP_MAILBOX_LOCKED			TERMINAL_ERR_CODE( SMTP_MODULE, 0x0F)			
/**
 * @brief Specifies the error code to indicate that error in sending a mail.
 */
#define JC_ERR_SMTP_SENDMAIL				TERMINAL_ERR_CODE( SMTP_MODULE, 0x10)				
/**
 * @brief Specifies the error code to indicate that smtp incomplete buffer.
 */			
#define JC_ERR_SMTP_INCOMPLETE_BUFFER		TERMINAL_ERR_CODE( SMTP_MODULE, 0x11)			
/**
 * @brief Specifies the error code to indicate that invalid user name  .
 */
#define JC_ERR_SMTP_INVALID_USERNAME		TERMINAL_ERR_CODE( SMTP_MODULE, 0x12)
/**
 * @brief Specifies the error code to indicate that invalid password  .
 */
#define JC_ERR_SMTP_INVALID_PASSWORD		TERMINAL_ERR_CODE( SMTP_MODULE, 0x13)
/**
 * @brief Specifies the error code to indicate that invalid port number.
 */
#define JC_ERR_SMTP_INVALID_PORT_NUMBER		TERMINAL_ERR_CODE( SMTP_MODULE, 0x14)
/**
 * @brief Specifies the error code to indicate that invalid server.
 */
#define JC_ERR_SMTP_INVALID_SERVER			TERMINAL_ERR_CODE( SMTP_MODULE, 0x15)
/**
 * @brief Specifies the error code to indicate that invalid response.
 */
#define JC_ERR_SMTP_INVALID_RESPONSE		TERMINAL_ERR_CODE( SMTP_MODULE, 0x16)
/**
 * @brief Specifies the error code to indicate that more data to be send.
 */
#define JC_ERR_SMTP_MORE_DATA_TO_SEND		TERMINAL_ERR_CODE( SMTP_MODULE, 0x17)
/**
 * @brief Specifies the error code to indicate that more data to be receive.
 */
#define JC_ERR_SMTP_MORE_DATA_TO_RECV		TERMINAL_ERR_CODE( SMTP_MODULE, 0x18)
/**
 * @brief Specifies the error code to indicate that send mail is in progress.
 */
#define JC_SMTP_SEND_MAIL_CONTINUE			TERMINAL_ERR_CODE( SMTP_MODULE, 0x19)
/**
 * @brief Specifies the error code to indicate that receiving data is in progress.
 */
#define JC_SMTP_RECV_CONTINUE				TERMINAL_ERR_CODE( SMTP_MODULE, 0x1A)
/**
 * @brief Specifies the error code to indicate that writing data is in progress to send mail.
 */
#define JC_SMTP_WRITE_CONTINUE				TERMINAL_ERR_CODE( SMTP_MODULE, 0x1B)
/**
 * @brief Specifies the error code to indicate that connection is timed out.
 */
#define JC_ERR_SMTP_CONNECTION_TIMEOUT		TERMINAL_ERR_CODE( SMTP_MODULE, 0x1C)
/**
 * @brief Specifies the error code to indicate that sending is timed out.
 */
#define JC_ERR_SMTP_SEND_TIMEOUT			TERMINAL_ERR_CODE( SMTP_MODULE, 0x1D)
/**
 * @brief Specifies the error code to indicate that receive is timed out.
 */
#define JC_ERR_SMTP_RECV_TIMEOUT			TERMINAL_ERR_CODE( SMTP_MODULE, 0x1E)
/**
 * @brief Specifies the error code to indicate that general failure.
 */
#define JC_ERR_SMTP_GENERAL_FAILURE			TERMINAL_ERR_CODE( SMTP_MODULE, 0x1F)
/**
 * @brief Specifies the error code to indicate that invalid stop request.
 */
#define JC_ERR_SMTP_INVALID_STOP_REQUEST	TERMINAL_ERR_CODE(SMTP_MODULE, 0x20)
/**
 * @brief Specifies the error code to indicate the user interrupt. 
 */
#define JC_ERR_SMTP_USER_INTERRUPT			TERMINAL_ERR_CODE(SMTP_MODULE, 0x21)
/**
 * @brief Specifies the error code to indicate that invalid delivery report request.
 */
#define JC_ERR_SMTP_INVALID_DELIVERY_REPORT_REQUEST TERMINAL_ERR_CODE(SMTP_MODULE, 0x22)
/**
 * @brief Specifies the error code to indicate that reset in smtp.
 */
#define JC_ERR_SMTP_RESET			TERMINAL_ERR_CODE(SMTP_MODULE, 0x23)
/**
 * @brief Specifies the error code to indicate that invalid mailbox in case of user id is not correct
 */
#define JC_ERR_SMTP_INVALID_MAILBOX		TERMINAL_ERR_CODE(SMTP_MODULE, 0x24)
/**
 * @brief Specifies the error code to indicate that transaction failed.
 */
#define JC_ERR_SMTP_TRANSACTION_FAILED 	TERMINAL_ERR_CODE(SMTP_MODULE, 0x25)
/**
 * @brief Specifies the error code to indicate that error in transport socket event.
 */
#define JC_SMTP_ERR_CHECK_TRANSPORT_SOCKETEVENT 	TERMINAL_ERR_CODE(SMTP_MODULE, 0x26)

#ifdef SMTP_WITH_SSL
/**
 * @brief Specifies the error code to indicate that smtp to give ehlo command.
 */
#define JC_ERR_SMTP_GIVE_EHLO	TERMINAL_ERR_CODE(SMTP_MODULE, 0x27)
/**
 * @brief Specifies the error code to indicate that ssl is required.
 */
#define JC_ERR_SMTP_SSL_530	TERMINAL_ERR_CODE(SMTP_MODULE, 0x28)
/**
 * @brief Specifies the error code to indicate that smtp security tls is failed.
 */
#define JC_ERR_SMTP_STARTTLS_FAILED		TERMINAL_ERR_CODE(SMTP_MODULE, 0x29)

#endif

#define JC_SMTP_AUTH_PROFILE_CONTINUE	        TERMINAL_ERR_CODE(SMTP_MODULE, 0x2A)
#define JC_ERR_SMTP_SERVER_CANNOT_VRFY_USER	    TERMINAL_ERR_CODE(SMTP_MODULE, 0x2B)
#define JC_ERR_SMTP_VRFY_DISABLED	            TERMINAL_ERR_CODE(SMTP_MODULE, 0x2C)

#define JC_ERR_SMTP_VRFY_MAILBOX_NOT_AVAILABLE	    TERMINAL_ERR_CODE(SMTP_MODULE, 0x2D)
#define JC_ERR_SMTP_VRFY_RCPT_NOT_LOCAL_TO_SERVER	TERMINAL_ERR_CODE(SMTP_MODULE, 0x2E)
#define JC_ERR_SMTP_VRFY_MAILBOXNAME_NOTALLOWED	    TERMINAL_ERR_CODE(SMTP_MODULE, 0x2F)

#ifdef SMTP_WITH_SSL

/**
 * @brief	This is the prototype of a callback which will be called by HTTP 
 *			to pass certificate information to the application.
 * @param[in]	vHandle		Application handle
 * @param[in]	ulCertificate	Certificate length
 * @param[in]	rCode			Status from HTTP
 * @retval		The callback function can return any application related error.
 * @note		The parameters vHandle should not be NULL.
 *
 * The application needs to implement a callback with the following prototype 
 * in order to get the certificate information from HTTP.
 */
typedef JC_RETCODE (*JL_MAIL_SSL_CERT_CALLBACK)(JC_HANDLE	vHandle, 
										   JC_UINT64	ulCertificate, 
										   JC_RETCODE	rCode) ;

/**
 * @brief	This function adds the root certificate information to SMTP_INFO structure, 
 * @param[in]	vSmtpHandle	Pointer to the SMTP_INFO structure
 * @param[in]	pucCert		Root certificate information
 * @param[in]	uiCertLen	Length of the certificate data
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle and pucCert should not be NULL.
 * 
 * This function is called by transporttask to set root certificate information 
 * in SMTP, which will be used in handshake process.
 */
JC_RETCODE jdi_SMTPAddRootCertificate (JC_HANDLE		vSmtpHandle, 
										 const JC_UINT8	*pucCert, 
										 JC_UINT32		uiCertLen ) ;

/**
 * @brief	This function registers a callback function to get the cartificate information.
 * @param[in]	vSmtpHandle	Pointer to the HTTP_INFO structure
 * @param[in]	cbCertCallBack	Specifies whether it is a proxy server authorization
 * @retval		JC_OK On success.
 * @note		The parameters vHttpHandle and cbCertCallBack should not be NULL.
 * 
 * This function is called by transporttask to register for the certificate information
 */
JC_RETCODE jdi_SMTPRegisterCertificateCallback (JC_HANDLE				vSmtpHandle, 
												  JL_MAIL_SSL_CERT_CALLBACK	cbCertCallBack) ;

#endif 

/**
 * @brief Enumeration values specifying the status for smtp profile.
 */
typedef enum smtp_status
{
	SMTP_UNCONNECTED = 0,	/**< Specifies smtp not connected */
	SMTP_INITIATED,			/**< Specifies smtp is initialized */
	SMTP_OK,				/**< Specifies smtp is ok */			
	SMTP_CONNECTING,		/**< Specifies smtp is connecting */
	SMTP_CONNECTED,			/**< Specifies smtp is connected with the server*/
	SMTP_CONNECT_FAILED,	/**< Specifies smtp connection failed*/

	SMTP_STARTING_SESSION,	/**< Specifies starting the session */
	SMTP_SESSION_STARTED,	/**< Specifies smtp session started */
	
	SMTP_AUTHENTICATING,	/**< Specifies smtp is authenticating */
	SMTP_AUTHENTICATION_FAILED,	/**< Specifies smtp authentication failure */
	SMTP_AUTHENTICATED,		/**< Specifies smtp authenticatd successfully */

	SMTP_MAIL_STARTED,		/**< Specifies smtp mail started */
	
	SMTP_SEND_MAIL_FROM_FAILED, /**< Specifies smtp send mail for this user is failed*/
	SMTP_SENDING_MAIL_FROM,		/**< Specifies smtp is authenticating */

	SMTP_SEND_RCPT_TO_FAILED,	/**< Specifies smtp sending mail receipent is failed*/
	SMTP_SENDING_RCPT_TO,		/**< Specifies smtp sending receipent*/

	SMTP_SEND_DATA_FAILED,		/**< Specifies smtp sending data is failed */
	SMTP_SENDING_DATA,			/**< Specifies smtp sending data */

	SMTP_SEND_USERDATA_FAILED,	/**< Specifies smtp sending user data is failed*/
	SMTP_SENDING_USERDATA,		/**< Specifies smtp sending user data*/

	SMTP_MAIL_QUEUED,			/**< Specifies mail is queue for sending */
	SMTP_SPAM_MAIL_QUEUED,		/**< Specifies smtp spam mail queued */
	SMTP_MAIL_SENT,				/**< Specifies smtp mail sent */
	SMTP_KEEPING_ALIVE,			/**< Specifies smtp sending NOOP command*/
	SMTP_CONNECTION_ALIVE,		/**< Specifies smtp Connection Alive*/
	SMTP_SENDING_RESET,			/**< Specifies smtp sending reset */
	SMTP_SENDING_VRFY, 			/**< Specifies smtp sending vrfy */
	SMTP_VRFY_SENT,             /**< Specifies smtp sent vrfy */
	SMTP_SEND_VRFY_FAILED,      /**< Specifies smtp sending VRFY has failed*/ 
	SMTP_CANCELED,				/**< Specifies smtp cancel */
	SMTP_SENDING_QUIT,			/**< Specifies smtp quit the sending*/
	SMTP_RESPONSE_RECEIVED,		/**< Received Response from the Server*/
	SMTP_CLOSED,				/**< Specifies smtp close the connection*/
	SMTP_SENDERID_SENT,			/**< Specifies smtp sender id is sent */
	SMTP_RCPTID_SENT,			/**< Specifies smtp receipent id is sent */
   	SMTP_RECEIVE_RESPONSE,		/**< Receiving Response from the Server*/
	SMTP_SEND_RESET_FAILED,		/**< specifies  smtp send reset failure*/
	SMTP_RECEIVE_RESPONSE_FAILED,/**< specifies receiving response from the server is failed*/
	SMTP_DISCONNECT_FAILED,		/**< specifies  smtp disconnect is failure*/
	SMTP_DNS_FAILED,			/**< specifies  DNS not found for the given host name*/
	SMTP_NO_CONTENT	,			/**< specifies  no content*/
	SMTP_BAD_REQUEST,			/**< specifies  smtp bad request*/				
	SMTP_NOT_FOUND	,			/**< specifies  smtp not found*/
	SMTP_COMMAND_NOT_SUPPORTED	,/**< specifies  smtp the command you specified is not supported*/		
	SMTP_METHOD_NOT_ALLOWED	,	/**< specifies  method not allowed*/	
	SMTP_USER_NOT_FOUND	,		/**< specifies  smtp user not found*/
	SMTP_NOT_ACCEPTABLE	,		/**< specifies  smtp not accepting*/				
	SMTP_REQUEST_TIMEOUT,		/**< specifies  smtp request timed out*/	
	SMTP_CONFLICT		,		/**< specifies  smtp conflict between servers*/	
	SMTP_LENGTH_REQD	,		/**< specifies  length required*/	
	SMTP_PRECONDITION_FAILED,	/**< specifies  smtp recondition failed*/	
	SMTP_REQUEST_ENTITY_TOO_LARGE	,/**< specifies  smtp request entity too large*/
	SMTP_REQUEST_ACTION_FAILURE	, /**< specifies  smtp request action failure*/
	SMTP_COMMAND_SYNTAX_ERROR,	/**< specifies  smtp command syntax error*/
	SMTP_ARGUMENT_SYNTAX_ERROR,	/**< specifies  smtp argument syntax error*/
	SMTP_BAD_SEQUENCE_OF_COMMAND,/**< specifies  smtp bad sequence of command*/
	SMTP_TRANSACTION_FAILED		,/**< specifies  smtp transaction failed*/
	SMTP_COMMAND_NOT_IMPLEMENTED,/**< specifies  smtp command not implemented*/			
	SMTP_SERVICE_UNAVAILABLE,	/**< specifies  smtp service unavailable*/	

	SMTP_PROXY_AUTHENTICATION_REQD	,/**< specifies smtp proxy authentication required*/
	SMTP_BAD_GATEWAY			,	/**< specifies smtp bad gateway */
	SMTP_INTERNAL_SERVER_ERROR	,/**< specifies  smtp internal server error*/
	SMTP_USER_INTERRUPT		/**< specifies  smtp user interrupt in between cleint/server transaction*/
#ifdef SMTP_WITH_SSL
	, SMTP_SSL_REQUIRED,		/**< when 530 is returned by the server*/
	SMTP_STARTING_SSL,			/**< when STARTTLS is sent*/
	SMTP_SSL_HANDSHAKE,			/**< when chipher suite negotiation is on*/
	SMTP_STARTTLS_OK,			/**< ssl negotiation is success*/
	SMTP_STARTTLS_FAILED,		/**< 501 returned by server*/
	SMTP_STARTTLS_TEMP_FAILED ,/**< temporary failure in ssl, 454 is sent by server*/
	SMTP_HANDSHAKE_FAILED	/**< specifies  smtp handshaking is failed*/
#endif
	, SMTP_PROFILE_DELETED	/**< specifies  smtp profile deleted */
} SMTP_STATUS ;

/**
 * @brief Enumeration values specifying the request types for smtp profile.
 */
typedef enum _request_type
{
	E_SMTP_NO_REQUEST       = 0,	/**< Specifies smtp No request */
	E_SMTP_CONNECT_REQUEST ,		/**< Specifies smtp connect request */
	E_SMTP_SEND_MAIL_REQUEST,		/**< Specifies smtp request for sending mail  */
	E_SMTP_RESET_REQUEST,			/**< Specifies smtp reset request */
	E_SMTP_KEEP_ALIVE_REQUEST,	/**< Specifies smtp	request for keep alive the connection */
	E_SMTP_DISCONNECT_REQUEST,	/**< Specifies smtp disconnect request */
	E_SMTP_DELETE_PROFILE_REQUEST,	/**< Specifies smtp request for deleting profile */
	E_SMTP_AUTH_PROFILE_REQUEST /**< Specifies smtp request for authenticating profile */
} ESMTPREQUEST_TYPE;

/**
 * @brief Enumeration values specifying the socket status.
 */
typedef enum _email_socket_status
{
	E_SMTP_SOCKET_UNCONNECTED = 0,	/**< Specifies smtp socket not connected */
	E_SMTP_SOCKET_DNS_RESOLVING,		/**< Specifies smtp dns resolving */
	E_SMTP_SOCKET_DNS_FAILED,			/**< Specifies smtp socket dns failed */
	E_SMTP_SOCKET_DNS_RESOLVED,		/**< Specifies smtp socket dns resolved */
	E_SMTP_SOCKET_CONNECTING,			/**< Specifies smtp socket is connecting */
	E_SMTP_SOCKET_CONNECTED,			/**< Specifies smtp socket connected successfully */
	E_SMTP_SOCKET_CONNECTION_FAILED,	/**< Specifies smtp socket connection failure */
	E_SMTP_SOCKET_CLOSED,				/**< Specifies smtp socket closed */
	E_SMTP_SOCKET_SENDING_DATA,		/**< Specifies smtp socket sending data is in progress*/
	E_SMTP_SOCKET_SENT_DATA,			/**< Specifies smtp socket data has been sent*/
	E_SMTP_SOCKET_RECEIVING_DATA,		/**< Specifies smtp socket receiving data is in progress */
	E_SMTP_SOCKET_RECEIVED_DATA		/**< Specifies smtp socket received data successfully */
#ifdef SMTP_WITH_SSL
	,	E_SMTP_SSL_HANDSHAKE_REQUIRED ,/**< Specifies smtp ssl handshake is required */
		E_SMTP_SSL_FAILED				/**< Specifies smtp ssl is failed */
#endif
} ESMTP_SOCKET_STATUS ; 

/**
 * @brief Enumeration values specifying the time out types for smtp.
 */
typedef enum e_smtp_timeout_type
{
	E_SMTP_CONNECT_TIMEOUT,		/**< Specifies smtp connection is timed out*/
	E_SMTP_SEND_TIME_OUT,		/**< Specifies smtp for sending data is timed out*/
	E_SMTP_RECV_TIME_OUT		/**< Specifies smtp for receiving data is timed out*/
} ESMTPTimeoutType ; 

/**
 * @brief Enumeration values specifies the Criteria for Delivery Report.
 */
typedef enum _smtp_deliver_report_criteria
{
	E_SMTP_SUCCESS = 0x01,      /**< Specifies the condition for recieving deleivery report is on Successful delivery*/
	E_SMTP_FAILURE = 0x02,      /**< Specifies the condition for recieving deleivery report is on Failure of Sending Mail */
	E_SMTP_DELAY   = 0x04,     /**< Specifies the condition for recieving deleivery report is on delay of delivery of the maail. */
	E_SMTP_NEVER   = 0x08      /**< Specifies that the delivery report is not to be sent in any case*/
}ESMTPDeliveryReportCriteria ;

/**
 * @brief Enumeration values specifies the types of secure connections supported.
 */
#if (defined SMTP_WITH_SSL )
	typedef enum _smtp_ssl_type
	{
		E_SMTP_SSL_NONE = 0 ,	 /**< Specifies that the Secure connection is not requried*/   
		E_SMTP_TLS_OR_NONE = 1 , /**< Specifies that the TLS Connection should be estabished if available or else normal connection*/
		E_SMTP_TLS = 2,          /**< Specifies that the TLS Connection is required*/
		E_SMTP_SSL = 3           /**< Specifies that the SSL Connection is required*/
	} ESMTPSSLType ;
#endif

/**
 * @brief this structure hold information about delivery report data for smtp. 
 */
typedef struct _smtp_deliver_report_data
{
	JC_BOOLEAN  bIsDelReportReq ;	/**< Specifies whether delivery report is required or not */
	JC_BOOLEAN  bRetHdrs ;          /**< if True then returns headers only otherwise entire mail*/
	JC_UINT32	uiDeliveryReportCriteria; /**< Specifies smtp delivery report criteria like success, failure */
}ST_SMTP_DELIVERY_REPORT_DATA ;

/**
 * @brief Defines structure to hold information sending mail buffer to send. 
 */
typedef struct _smtp_sendmail_buffer
{
	JC_INT8		*pszSenderMailID;     /**< Specifies sender mailid i.e from address */ 
	JC_INT8		*pszRecipientsIDList; /**< Specifies recipients mailids to be dispatch the mail*/
	ST_SMTP_DELIVERY_REPORT_DATA *pstSmtpDelReportData ;/**< structure contains information about delivery report data */
	JC_INT8		*pszMailBuffer;		/**< Specifies actual buffer contains body and all */
	JC_UINT32	uiTotalBufferLen;	/**< Specifies total buffer length of the mail */
	JC_UINT32   puiRcvdBufferSize;	/**< Specifies received buffer size */
	JC_UINT32	uiRemainChunkLen;	/**< Specifies chunking data length if any */
	JC_BOOLEAN  bHasMoreData;		/**< if TRUE then user has to send more data other wise total mail buffer he can send */
} ST_SMTP_SEND_MAILBUFFER;

/**
 * @brief Defines structure to hold information of sending more data. 
 */
typedef struct _smtp_buffer
{
	JC_INT8		*pszMailBuffer;		/**< Specifies mail buffer to be send more data */
	JC_UINT32	uiOffset;			/**< Specifies the user data length already received by transport in previous chunks */
	JC_UINT32   uiBufferLen;		/**< Specifies buffer length of the current chunking data */
	JC_UINT32   uiTotalSize;		/**< Specifies total size of the mail buffer*/
} ST_SMTP_BUFFER;

/**
 * @brief Defines structure to hold information of smtp server response. 
 */
typedef struct _smtp_response_tag
{
	SMTP_STATUS	eStatus;			/**< Specifies status of the smtp */
	ESMTPREQUEST_TYPE eRequestType ;		/**< Specifies request type like whether it is connected,disconnected */
	JC_RETCODE	eRetCode;			/**< Specifies return code value */
	JC_BOOLEAN	bIsResponseRcvd;		/**< if it is true then response received successfully  */
	JC_UINT32	uiSessionID;		/**< Specifies session id */
	JC_UINT32	uiProfileID;		/**< Specifies profile id */
	JC_UINT32	uiBytesRecvd ;		/**< Specifies no of bytes already received */
	JC_UINT32	uiBytesSent ;		/**< Specifies no of bytes already sent */
	JC_UINT32	uiTotalBytesToSend ;/**< Specifies total no of bytes has to be send */
	JC_UINT32	uiTotalBytesToRecv ;/**< Specifies total no of bytes has to be receive */
	JC_UINT32	uiBufferLen;		/**< Specifies length of the whole buffer */
	JC_INT8		*pucBuffer;			/**< Specifies sending or receiving buffer */
} ST_SMTP_RESPONSE ;

/**
 * @brief Defines structure to hold information of smtp profile.
 */
typedef struct _smtp_profile
{
	JC_UINT32	uiProfileID;		/**< Specifies profile id */
	JC_BOOLEAN	bIsAuthReqd ;		/**< if it is true then we should provide user id and password */
	JC_INT8		*pszUserID;			/**< Specifies user id to authenticate*/
	JC_INT8		*pszPassword;		/**< Specifies password to authenticate*/
	JC_INT8		*pszServerName;		/**< Specifies server name for this profile to send mails */
	JC_UINT16	 uhPortNum;			/**< Specifies port number*/
#ifdef SMTP_WITH_SSL
	ESMTPSSLType eSSLType ;			/**< Specifies security type (SSL type)*/
#endif
} ST_SMTP_PROFILE;

/**
 * @brief This function will initialize the smtp protocol and get the handle .
 * @param[out] pvSMTPHandle   Specifies the handle. 
 * @return  
 *		   - JC_RETCODE               Specifies smtp is initialized successfully.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.
 *		   - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE jdi_SMTPInitialize (JC_HANDLE *pvSMTPHandle) ;

/**
* @fn 		jdi_SMTPAuthenticateProfile
* @brief 	This function authenticates the new profile 
* @param	[in]  vSMTPHandle	 The Initialized SMTP Handle
* @param	[in]  uiProfileID    The profile's id that needs to be validated 
* @return  
* @retval	JC_OK	on success.  
*/
JC_RETCODE	jdi_SMTPAuthenticateProfile( JC_HANDLE	vSMTPHandle, JC_UINT32 uiProfileID ) ; 

/**
 * @brief This function adds the new profile to smtp handle.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] pstProfileData  Specifies the profile data details to be add.
 * @return  
 *		   - JC_RETCODE               Specifies added profile successfully.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE jdi_SMTPAddProfile (JC_HANDLE vSMTPHandle, ST_SMTP_PROFILE *pstProfileData) ;

/**
 * @brief This function updates the existing profile in smtp handle based on profile id.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] uiProfileID  Specifies the profile id to update the profile.
 * @param[in] pstProfileData Specifies the profile data details to update the profile based on profile id.
 * @return  
 *		   - JC_RETCODE               Specifies updated profile successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_SMTP_PROFILE_NOT_EXISTS Specifies that if you are trying to update the profile that is not exists in the handle.
 */
JC_RETCODE	jdi_SMTPUpdateProfile ( JC_HANDLE		vSMTPHandle, JC_UINT32 uiProfileID, 
								    ST_SMTP_PROFILE *pstProfileData ) ;

/**
 * @brief This function deletes the profile from smtp handle based on profile id.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] uiProfileID  Specifies the profile id to delete the profile.
 * @return  
 *		   - JC_RETCODE               Specifies deleted profile successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_SMTP_PROFILE_NOT_EXISTS Specifies that if you are trying to delete the profile that is not exists in the handle.
 */
JC_RETCODE jdi_SMTPDeleteProfile (JC_HANDLE vSMTPHandle, JC_UINT32 uiProfileID) ;

/**
 * @brief This function resets the profile in smtp handle.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @return  
 *		   - JC_RETCODE               Specifies reset the profile successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_SMTP_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 */
JC_RETCODE	jdi_SMTPReset ( JC_HANDLE  vSMTPHandle, JC_UINT32  uiProfileID ) ;

/**
 * @brief This function Connects establishes a socket connection to the required Host.
 * @param[in] hHandle        The Initialized SMTP Handle.
 * @param[in] uiProfileID     Specifies the profile id.
 * @return  
 *		   - JC_RETCODE               Specifies connection established successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE	jdi_SMTPKeepAlive ( JC_HANDLE	vHandle, JC_UINT32	uiProfileID ) ;

/**
 * @brief This function sends the mail to recipientlist.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @param[in] pstMailData Specifies the Email Details of the Mail to be sent.
 * @return  
 *		   - JC_RETCODE               Specifies sent mail successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.	
 *		   - JC_ERR_SMTP_PROFILE_NOT_EXISTS Specifies the profile is not exists in the handle for this profile id.
 *		   - JC_SMTP_SEND_MAIL_CONTINUE Specifies that sending mail is in progress.
 *		   - JC_ERR_SMTP_BUSY         Specifies smtp is busy with some other task.	
 */
JC_RETCODE	jdi_SMTPSendMail ( JC_HANDLE	vSMTPHandle, JC_UINT32 uiProfileID, 
							   ST_SMTP_SEND_MAILBUFFER	*pstSendMailReq ) ;

/**
 * @brief This function gets the remaining chunk of mail data to be sent.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @param[in] pstSmtpBuffer  Contains the next chunk of Data sent from application.
 * @return  
 *		   - JC_RETCODE               Specifies sent more data successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.	
 *		   - JC_ERR_SMTP_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 *		   - JC_SMTP_RECV_CONTINUE         Specifies smtp is receving data from server is in progress.	
 */
JC_RETCODE	jdi_SMTPSendMoreData ( JC_HANDLE	vSMTPHandle, JC_UINT32	uiProfileID, 
								   ST_SMTP_BUFFER	*pstSendMoreData ) ;

/**
 * @brief This function disconnects the Session with the server.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @return  
 *		   - JC_RETCODE               Specifies smtp diconnected successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_SMTP_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 */
JC_RETCODE	jdi_SMTPDisconnect ( JC_HANDLE	vSMTPHandle, JC_UINT32	uiProfileID ) ;

/**
 * @brief This function calls the smtphandleconnect and smtphandlesendmail based on the profile request type.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @param[in] pstSmtpResponse Specifies the response data.
 * @return  
 *		   - JC_RETCODE               Specifies get the response  successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_SMTP_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 *		   - JC_SMTP_RECV_CONTINUE         Specifies smtp is receving data from server is in progress.	
 */
JC_RETCODE	jdi_SMTPDataPoll ( JC_HANDLE	vSMTPHandle, JC_UINT32	uiProfileID, 
							   ST_SMTP_RESPONSE		*pstSmtpResponse ) ;

/**
 * @brief This function will deinitialize the smtp handle.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @return  
 *		   - JC_RETCODE               Specifies smtp deinitialized successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE	jdi_SMTPDeInitialize ( JC_HANDLE	*vSMTPHandle ) ;

/**
 * @brief This function Sets the Socket Status of Socket connection.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] iSocketID   Specfies that socket id to get the socket data infomation from handle .
 * @param[in] eSocketStatus Specifies that socket status to be set in socket data.
 * @return  
 *		   - JC_RETCODE               Specifies that set connection state successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE	jdi_SMTPSetConnectionState ( JC_HANDLE	vSMTPHandle, JC_INT32 iSocketID,
										 ESMTP_SOCKET_STATUS	 eSocketStatus ) ;

/**
* @brief This function Connects establishes a socket connection to the required Host.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] pucHostName     Specifies the host name to connect.
 * @param[in] pucAddress      Specifies the ip address of that host.
 * @return  
 *		   - JC_RETCODE               Specifies connection established successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_MEMORY_ALLOCATION	Specifies that memory allocation failure.
 *         - JC_SMTP_SEND_MAIL_CONTINUE Specifies that sending mail is in progress
 *         - JC_ERR_NET_DNS_RESOLVE    Specifies  that connection with host is fail.
 */
JC_RETCODE	jdi_SMTPConnect ( JC_HANDLE		vSMTPHandle, const JC_UINT8	*pucHostName,
						      JC_UINT8		*pucAddress ) ;

/**
 * @brief This function Sets the STOP request for the ongoing process.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] uiProfileID Specifies the profile id to get the profile from handle.
 * @return  
 *		   - JC_RETCODE               Specifies that stopped the smtp request successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_SMTP_INVALID_STOP_REQUEST Specifies that invalid stop request incase of req type is equals to SMTP_NO_REQUEST.
 *         - JC_ERR_SMTP_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 */
JC_RETCODE	jdi_SMTPSetStopRequest ( JC_HANDLE	vSMTPHandle, JC_UINT32  uiProfileID ) ;

/**
 * @brief This function Sets the the Timeout value in the Protocol Handle.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] eTimeOutType Specifies time out type whether it is send time out or recv time out.
 * @param[in] uiTimeOut	  Specifies some number to set the time out.
 * @return  
 *		   - JC_RETCODE               Specifies that set the smtp time out successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
void	 jdi_SMTPSetTimeOut ( JC_HANDLE vSMTPHandle , ESMTPTimeoutType eTimeOutType,
							  JC_UINT32 uiTimeOut ) ;

/**
 * @brief This function will set the chunk size in Protocol handle.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] uiChunkSize Specifies the chunk size, if the buffer length is greater than this size then we will chunk that data and send.
 * @return  
 *		   - JC_RETCODE               Specifies that chunking size is setting in ptotocol handle successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
void	jdi_SMTPSetChunkSize( JC_HANDLE		vSMTPHandle, JC_UINT32	uiChunkSize ) ;

/**
 * @brief This function will check whether the current session is valid or not.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] iSocketID   Specifies the socket id to get the socket data.
 * @return  
 *		   - E_TRUE     get the data successfully from socket handle 
 *         - E_FALSE    if socket data is NULL for corresponding socket id.
 */
JC_BOOLEAN	jdi_SMTPIsHandleForSocket ( JC_HANDLE	vSMTPHandle, JC_INT32	iSocketID ) ;

/**
 * @brief This function  reads the data from the socket and fills it in the socket queue.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] iSocketID   Specfies that socket id to get the socket data infomation from handle .
 * @param[in] pucReadBuffer Specifies the buffer.
 * @param[in] uiReadBufferLen Specifies the length of the buffer.
 * @return  
 *		   - JC_RETCODE               Specifies that read socket data successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies that memory allocation failure.
 */
JC_RETCODE	jdi_SMTPReadSocketData ( JC_HANDLE vSMTPHandle,    JC_INT32   iSocketID, 
									 JC_INT8   *pucReadBuffer, JC_UINT32  uiReadBufferLen ) ;

/**
 * @brief This function resets the socket profile.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] iSocketID   Specifies the socket id to get the socket data.
 * @return  
 *		   - JC_RETCODE               Specifies reset the socket profile successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE	jdi_SMTPResetSocketProfile ( JC_HANDLE	vSMTPHandle, JC_INT32	iSocketID ) ;

/**
 * @brief This function will set the data connection status to smtp handle.
 * @param[in] vSMTPHandle     The Initialized SMTP Handle.
 * @param[in] bDataConnStatus Specifies the connection status.
 * @param[in] uiConnID    specifies Connection id to store in handle.
 * @return  
 *		   - JC_RETCODE               Specifies that set data connection status in handle.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE	jdi_SMTPSetDataConnStatus ( JC_HANDLE  vSMTPHandle, JC_BOOLEAN  bDataConnStatus,
									    JC_UINT32  uiConnID ) ;

#endif

#ifdef __cplusplus
}
#endif


#endif
