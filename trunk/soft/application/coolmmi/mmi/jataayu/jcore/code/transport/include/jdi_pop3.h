/***************************************************************************
 * $Id: jdi_pop3.h,v 1.18 2009/05/29 12:00:18 browserdevjtbase Exp $
 * $Revision: 1.18 $
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
 * $Log: jdi_pop3.h,v $
 * Revision 1.18  2009/05/29 12:00:18  browserdevjtbase
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
 * Revision 1.12  2007/11/14 08:52:37  browserdevjtbase
 * core update.
 *
 * Revision 1.11  2007/09/17 06:56:07  browserdevjtbase
 * Update.
 *
 * Revision 1.11  2007/09/11 12:42:13  manikumardevjemulator
 * Updates of UIDL Handling in Core
 *
 * Revision 1.10  2007/08/13 15:17:47  manikumardevjemulator
 * jMail Build 16 QC Release Updates
 *
 * Revision 1.9  2007/08/01 11:54:20  kumardevjtbase
 * Email Updates
 *
 * Revision 1.8  2007/04/12 14:59:24  browserdevjtbase
 * Mail core update
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
 * @file jdi_pop3.h
 * @ingroup transport 
 * @brief Defines the exposed enumerations, structures and APIs of the pop3 that
 *		  can be used by the transport.
 */

#ifndef _JDI_POP3_H
#define _JDI_POP3_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <ddl.h>

#ifdef POP3_STACK_ENABLE
#include <jcal.h>
#include <jdi_transportmodule.h>


/**
 * @brief Specifies the error code to indicate that profile already exists.
 */
#define JC_ERR_POP3_PROFILE_EXISTS			TERMINAL_ERR_CODE(POP3_MODULE, 0x01)				
/**
 * @brief Specifies the error code to indicate that profile does not exists in POP3 Handle.
 */
#define JC_ERR_POP3_PROFILE_DOES_NOT_EXIST  TERMINAL_ERR_CODE(POP3_MODULE, 0x02)				
/**
 * @brief Specifies the error code to indicate Error in adding profile
 */
#define JC_ERR_POP3_ADD_PROFILE				TERMINAL_ERR_CODE(POP3_MODULE, 0x03)				
/**
 * @brief Specifies the error code to indicate Error in deleting profile
 */
#define JC_ERR_POP3_DELETE_PROFILE			TERMINAL_ERR_CODE(POP3_MODULE, 0x04)				
/**
 * @brief Specifies the error code to indicate Error in updating profile
 */
#define JC_ERR_POP3_UPDATE_PROFILE			TERMINAL_ERR_CODE( POP3_MODULE, 0x05)
/**
 * @brief Specifies the error code to indicate that POP3 Protocol Module is busy processing previous request.
 */
#define JC_ERR_POP3_BUSY					TERMINAL_ERR_CODE( POP3_MODULE, 0x06)
/**
 * @brief Specifies the error code to indicate Error in pop3 connection 
 */
#define JC_ERR_POP3_CONNECT					TERMINAL_ERR_CODE( POP3_MODULE, 0x07)				
/**
 * @brief Specifies the error code to indicate Error in pop3 disconnection
 */
#define JC_ERR_POP3_DISCONNECT				TERMINAL_ERR_CODE( POP3_MODULE, 0x08)				
/**
 * @brief Specifies the error code to indicate that the POP3 Server has Timed Out the connection(AUTO LOGOUT).
 */
#define JC_ERR_POP3_AUTOLOGOUT				TERMINAL_ERR_CODE( POP3_MODULE, 0x09)				
/**
 * @brief Specifies the error code to indicate Error in authenticating username for pop3.
 */
#define JC_ERR_POP3_AUTHENTICATE_USER		TERMINAL_ERR_CODE( POP3_MODULE, 0x0A)				
/**
 * @brief Specifies the error code to indicate authentication failure.
 */
#define JC_ERR_POP3_AUTHENTICATION			TERMINAL_ERR_CODE( POP3_MODULE, 0x0B)				
/**
 * @brief Specifies the error code to indicate that mail box is locked or the MailBox is used by another Client.
 */
#define JC_ERR_POP3_MAILBOX_LOCKED			TERMINAL_ERR_CODE( POP3_MODULE, 0x0C)				
/**
 * @brief Specifies the error code to indicate General failure.
 */
#define JC_ERR_POP3_GENERAL_FAILURE			TERMINAL_ERR_CODE( POP3_MODULE, 0x0D)				
/**
 * @brief Specifies the error code to indicate Error in getting message index.
 */
#define JC_ERR_POP3_GET_MSGINDEX			TERMINAL_ERR_CODE( POP3_MODULE, 0x0E)				
/**
 * @brief Specifies the error code to indicate that message does not exists in server.
 */
#define JC_ERR_POP3_MSG_DOES_NOT_EXIST		TERMINAL_ERR_CODE( POP3_MODULE, 0x0F)				
/**
 * @brief Specifies the error code to indicate that retrieving mail size Failed.
 */
#define JC_ERR_POP3_GET_MAIL_SIZE			TERMINAL_ERR_CODE( POP3_MODULE, 0x10)				
/**
 * @brief Specifies the error code to indicate that retrieving message header failed.
 */
#define JC_ERR_POP3_GET_MSGHDR				TERMINAL_ERR_CODE( POP3_MODULE, 0x11)				
/**
 * @brief Specifies the error code to indicate that Protocol Module has Some More data to be Sent to the server.
 */
#define JC_ERR_POP3_MORE_DATA_TO_SEND		TERMINAL_ERR_CODE( POP3_MODULE, 0x12)
/**
 * @brief Specifies the error code to indicate that Protocol Module has some More Data to be received
 */
#define JC_ERR_POP3_MORE_DATA_TO_RECV		TERMINAL_ERR_CODE( POP3_MODULE, 0x13)
/**
 * @brief Specifies the error code to indicate that the requested process is in progress
 */
#define JC_ERR_POP3_RECV_CONTINUE			TERMINAL_ERR_CODE( POP3_MODULE, 0x14)
/**
 * @brief Specifies the error code to indicate that sending data is in progress.
 */
#define JC_ERR_POP3_SEND_CONTINUE			TERMINAL_ERR_CODE( POP3_MODULE, 0x15)
/**
 * @brief Specifies the error code to indicate invalid stop request.
 */
#define JC_ERR_POP3_INVALID_STOP_REQUEST	TERMINAL_ERR_CODE( POP3_MODULE, 0x16)
/**
 * @brief Specifies the error code to indicate the user interrupt. 
 */
#define JC_ERR_POP3_USER_INTERRUPT			TERMINAL_ERR_CODE( POP3_MODULE, 0x17)
/**
 * @brief Specifies the error code to indicate the error while getting uid list from server. 
 */
#define JC_ERR_POP3_GETUIDLIST				TERMINAL_ERR_CODE( POP3_MODULE, 0x18)
/**
 * @brief Specifies the error code to indicate the error while getting mail buffer from server. 
 */
#define JC_ERR_POP3_GETMAILBUFFER			TERMINAL_ERR_CODE( POP3_MODULE, 0x19)
/**
 * @brief Specifies the error code to indicate the error while deleting mail in server. 
 */
#define JC_ERR_POP3_DELETE_MAIL				TERMINAL_ERR_CODE( POP3_MODULE, 0x1A)
/**
 * @brief Specifies the error code to indicate the error while resetting the profile.
 */
#define JC_ERR_POP3_UNDELETE_MAIL			TERMINAL_ERR_CODE( POP3_MODULE, 0x1B)
/**
 * @brief Specifies the error code to indicate that apop command is not supported by the server.
 */
#define JC_ERR_POP3_APOP_UNSUPPORTED		TERMINAL_ERR_CODE( POP3_MODULE, 0x1C)
/**
/**
 * @brief Specifies the error code to indicate that invalid state for message index to be retrived. 
 */
#define JC_ERR_POP3_INVALID_STATE_FOR_MSGINDEX     	TERMINAL_ERR_CODE(POP3_MODULE, 0x1D)





/**
  * @brief Enumeration values specifying the pop3 profile status.
  */
typedef enum _pop3_profile_status  
{
	POP3_UNCONNECTED =0,		/**< Specifies pop3 not connected */
	
	POP3_CONNECTING,			/**< Specifies pop3 is connecting */
	POP3_CONNECTED,				/**< Specifies pop3 is connected with the server*/

#ifdef POP3_WITH_SSL
	POP3_STARTING_SSL, /* when STARTTLS is sent*/
	POP3_SSL_HANDSHAKE,/* when chipher suite negotiation is on*/
	POP3_STLS_OK,/* ssl negotiation is success*/
#endif

	POP3_SENDING_USERDATA,		/**< Specifies pop3 is sending user data*/
	POP3_AUTHENTICATING,		/**< Specifies pop3 is authenticating */
	POP3_AUTHENTICATING_USER,	/**< Specifies pop3 autenticating with userid*/
	POP3_AUTHORIZED_USER,		/**< Specifies pop3 authorized user*/

	POP3_AUTHENTICATING_PASS,	/**< Specifies pop3 authenticating with password*/
	POP3_AUTHORIZED_PASS,		/**< Specifies pop3 authenticating user*/
	
	POP3_RECEIVING_MSGLIST,		/**< Specifies pop3 status for receiving message list*/
	POP3_MSGLIST_RECVD,			/**< Specifies Message list has been received*/
	POP3_RECEIVING_MSGINDEX ,	/**< Specifies pop3 status for receiving message index*/
	POP3_MSGINDEX_RECVD,		/**< Specifies pop3 status  to indicate that Message index has been received*/

	POP3_RECEIVING_MSGHDR,		/**< Specifies pop3 status for receiving message header*/
	POP3_MSGHDR_RECVD,			/**< Specifies pop3 status that the Message header has been received*/

	POP3_RECEIVING_MSG,			/**< Specifies pop3 status for receiving message*/
	POP3_MSG_RECVD,			    /**< Specifies pop3 status to indicate that Message has been received*/

	POP3_RECEIVING_SIZE,		/**< Specifies pop3 status for receiving message size*/
	POP3_SIZE_RECVD,			 /**< Specifies pop3 status to indicate that Message size is received*/

	POP3_DELETING_MSG,			/**< Specifies pop3 status for deleting message size*/
	POP3_MSG_DELETED,			 /**< Specifies pop3 status to indicate Message has been deleted*/

	POP3_RESETTING,				/**< Specifies pop3 status for resetting the profile*/
	POP3_RESET,				    /**< Specifies pop3 status to indicate that the profile has been reset*/

	POP3_KEEPING_ALIVE,			/**< Specifies pop3 command which indicates receiving keeping alive response from server*/
	POP3_CONNECTION_ALIVE,      /**< Specifies pop3 commnad which indiactes connection is alive */
	POP3_DISCONNECTING,			 /**< Specifies pop3 Module is diconnecting the Profile's connection to the server*/
	POP3_DISCONNECTED,			/**< Specifies pop3 status for disconnected the pop3*/
	
	POP3_AUTOLOGOUT,			/**< Specifies pop3 auto logout command*/
	POP3_CONNECT_FAILED,		/**< Specifies pop3 status for connection failed*/
	POP3_MAILBOX_LOCKED ,		/**< Specifies pop3 status for mailbox locked*/
	POP3_AUTHENTICATING_USER_FAILED,/**< Specifies pop3 status for authentication is failed for this userid*/
	POP3_AUTHENTICATING_PASS_FAILED,/**< Specifies pop3 status for authentication is failed for this password*/
    POP3_AUTHENTICATING_APOP_FAILED,/**< Specifies pop3 status for authentication is failed for apop command*/
	POP3_RECV_UIDL_FAILED,		 /**< Specifies receving uidl list failed*/
	POP3_GET_MSGINDEX_FAILED ,	/**< Specifies pop3 status for getting message index is failed*/
	POP3_RECEIVING_SIZE_FAILED ,/**< Specifies pop3 status which indicates receiving size has failed*/
	POP3_RECV_MAIL_FAILED ,		/**< Specifies pop3 status which indicates receiving mail has failed*/
	POP3_RECV_MAILHDR_FAILED,	/**< Specifies pop3 status which indicates receiving mail header has failed*/
	POP3_DELETE_FAILED,			/**< Specifies pop3 status which indicates deleting a mail has failed*/
	POP3_RESET_FAILED ,			/**< Specifies pop3 status which indicates resetting profile has failed*/
	POP3_DISCONNECT_FAILED,		/**< Specifies pop3 status which indicates disconnect failed*/
	POP3_DNS_FAILED,			/**< Specifies pop3 status which indicates dnslookup  failed*/
	POP3_CAPA_SENDING ,			/**< Specifies pop3 command which indicates receiving capability response from server has failed*/
	POP3_CAPA_OK ,				/**< Specifies pop3 command which indicates receiving capability response from server was successful*/
	
	POP3_USER_INTERRUPT,		/**< Specifies pop3 status which indicates POP3 Module has received a user interrupt*/
#ifdef POP3_WITH_SSL
	POP3_STLS_FAILED,			/**< specifies pop3 status which indicates that starting TLS Session has failed*/
#endif
	POP3_PROFILE_DELETED		/**< specifies  pop3 profile has been deleted */

}POP3_STATUS;

/**
  * @brief Enumeration values specifying the pop3 socket status.
  */
typedef enum _pop3_socket_status
{
	E_POP3_SOCKET_UNCONNECTED = 0,	/**< Specifies pop3 socket is not connected */
	E_POP3_SOCKET_DNS_RESOLVING,		/**< Specifies pop3 dns resolving the connection*/
	E_POP3_SOCKET_DNS_RESOLVED,		/**< Specifies pop3 socket dns resolved */
	E_POP3_SOCKET_DNS_FAILED,			/**< Specifies pop3 socket dns failed */
	E_POP3_SOCKET_CONNECTING,			/**< Specifies pop3 socket conenction is in progress*/
	E_POP3_SOCKET_CONNECTED,			/**< Specifies pop3 socket connected successfully */
	E_POP3_SOCKET_CONNECTION_FAILED,	/**< Specifies pop3 socket connection failure */
	E_POP3_SOCKET_SENDING_DATA,		/**< Specifies pop3 socket sending data is in progress*/
	E_POP3_SOCKET_SENT_DATA,			/**< Specifies pop3 socket data has been sent*/
	E_POP3_SOCKET_RECEIVING_DATA,		/**< Specifies pop3 socket receiving data is in progress */
	E_POP3_SOCKET_RECEIVED_DATA,		/**< Specifies pop3 socket received data successfully */
	E_POP3_SOCKET_CLOSED				/**< Specifies pop3 socket status for closing the connection */
#ifdef POP3_WITH_SSL
	,E_POP3_SSL_HANDSHAKE_REQUIRED ,	/**< Specifies pop3 ssl handshake is required */
	E_POP3_SSL_FAILED					/**< Specifies pop3 ssl is failed */
#endif
} EPOP3_SOCKET_STATUS ; 

/**
 * @brief Enumeration values specifying the request types for pop3 profile.
 */
typedef enum _pop3_request_type
{
	 E_POP3_REQUEST_NONE = 0 ,				/**< Specifies no request is required */
	 E_POP3_RECV_MSGLIST_REQUEST,		/**< Specifies the request for receving message list from server*/
	 E_POP3_RECV_MSG_HEADER_REQUEST,	/**< Specifies the request for receving message header from server*/
	 E_POP3_RECV_MAIL_REQUEST,		/**< Specifies the request for receving message from server*/
	 E_POP3_DELETE_MAIL_REQUEST,		/**< Specifies the request for deleting message on server*/
	 E_POP3_CANCEL_REQUEST,			/**< Specifies the request for cancel*/
	 E_POP3_KEEPALIVE_REQUEST,		/**< Specifies the request for keep alive the pop3 handle*/
	 E_POP3_ADD_PROFILE_REQUEST,		/**< Specifies the request for adding  profile in handle*/
	 E_POP3_AUTH_PROFILE_REQUEST,   /**< Specifies the request for authorising  profile in handle*/
	 E_POP3_UPDATE_PROFILE_REQUEST,	/**< Specifies the request for updating  profile in handle*/
	 E_POP3_DELETE_PROFILE_REQUEST,	/**< Specifies the request for deleting  profile from handle*/
	 E_POP3_DELETE_ALL_PROFILE_REQUEST,/**< Specifies the request for deleting all profiles from handle*/
	 E_POP3_DISCONNECT_REQUEST		/**< Specifies the request to disconnect */
}EPOP3_REQUEST_TYPE;

/**
 * @brief Enumeration values specifying the types of SSL Connection Suipported.
 */
#if (defined POP3_WITH_SSL )
	typedef enum _pop3_ssl_type
	{
		E_POP3_SSL_NONE = 0 ,      /**< Specifies that Secure Connection is not required */
		E_POP3_TLS_OR_NONE = 1 ,  /**< Specifies that TLS Connection is to be used If Available Else Connect without secure connection */
		E_POP3_TLS = 2,			 /**< Specifies that TLS Connection is required */
		E_POP3_SSL = 3            /**< Specifies that SSL Connection is required */
	} E_POP3_SSLType ;
#endif

/**
 * @brief Defines structure to hold information of pop3 profile.
 */
typedef struct pop3_profile 
{
	JC_UINT32	uiProfileID ;			/**< Specifies profile id */
	JC_INT8 	*pszUserID ;			/**< Specifies user id to authenticate*/
	JC_INT8 	*pszPassword ;			/**< Specifies password to authenticate*/
	JC_INT8 	*pszServerName;			/**< Specifies server name for this profile to receive mails */
	JC_UINT16	uhPortNum;				/**< Specifies port number*/
	JC_BOOLEAN  bLeaveCopyOnServer ;	/**< if it is true then we will not delete the mail from server while receving,if false we will delete mails on server*/
	JC_BOOLEAN  bIsAuthReqd ;			/**< if it is true then profile should be authenticated if valid then profile should be added */
#ifdef	POP3_WITH_SSL
	E_POP3_SSLType	eSSLType ;				/**< Specifies security type (SSL type)*/
#endif
}ST_POP3_PROFILE ;

/**
 * @brief Defines structure to hold information of pop3 uid list details.
 */
typedef struct _pop3_uid_list
{
	JC_INT8		*pszBuffer ;		/**< Specifies the buffer contains all the uid's that received from server*/
	JC_UINT32	uiTotalBufferLen;	/**< Specifies the total buffer length */
	JC_UINT32   puiRcvdBufferSize;	/**< Specifies the recevied buffer size that is received in one chunk */
	JC_UINT32	uiRemainChunkLen;   /**< Specifies the remaining chunk length of data to receive*/
	JC_BOOLEAN  bIsMsgindex ;		/**< Specifies whether Message index is passed by application */
	JC_BOOLEAN  bHasMoreData ;		/**< Specifies whether the buffer is complete or not.*/
	
}ST_POP3_UID_LIST ;
	
/**
 * @brief Defines structure to hold information about profile id.
 */
typedef struct _pop3_msglist_req
{	
	JC_UINT32	uiProfileID;		/**< Specifies the profile id*/
	
}ST_POP3_MSG_LIST_REQ;
/**
 * @brief Defines structure to hold information of message header .
 */
typedef struct _pop3_header_req
{
	JC_UINT32	uiProfileID;	/**< Specifies the profile id*/
	JC_UINT32	uiMsgIndex;		/**< Specifies the message index of the mail */ 
	JC_INT8		*pszMsgID ;		/**< Specifies the message id of the pop3 mail to receive headers*/
	
} 
ST_POP3_MSG_HEADER_REQ;

/**
 * @brief Defines structure to hold information of sending more pop3 data. 
 */	
typedef struct _pop3_buffer
{
	JC_UINT32			uiOffset;	    /**< Specifies the user data length already received by transport in previous chunks */
	JC_UINT32			uiBufferLen;	/**< Specifies buffer length of the current chunking data */
	JC_INT8				*pszBuffer;	    /**< Specifies mail buffer to be send more data */		
	JC_UINT32            uiTotalSize;	/**< Specifies total size of the mail buffer*/
} ST_POP3_BUFFER;

/**
 * @brief Defines structure to hold information of mail buffer. 
 */
typedef struct _pop3_mail_buffer_req
{
	JC_UINT32	uiProfileID;			/**< Specifies the profile id*/
	JC_UINT32	uiMsgIndex;				/**< Specifies the message index*/
	JC_UINT32	uiMailSize;				/**< Specifies the mail size*/
	JC_INT8		*pszMsgID ;				/**< Specifies the message id of the mail to receive*/
	JC_BOOLEAN  bLeaveCopyOnServer;		/**< if it is true then we will not delete the mail from server while receving,if false we will delete mails on server*/
} ST_POP3_MAIL_BUFFER_REQ;
/**
 * @brief Defines structure to hold information of pop3 response. 
 */
typedef  struct _pop3_response_info
{
	JC_UINT32			uiProfileID ;	/**< Specifies the profile id*/
	JC_UINT32			uiSessionID;	/**< Specifies the session id*/
	JC_INT8				*pszMailID;		/**< Specifies the message id*/
	JC_BOOLEAN			bIsResponseRcvd; /**< if true then response is received otherwise not received*/
	JC_BOOLEAN			bIsUIDListRxd ;	/**< if it is true then process the uidl list data*/
	JC_BOOLEAN			bIsUIDLRxd ;	/**< if it is true then process uidl information*/
		JC_BOOLEAN          bIsLastChunk;  /**< if it is true then receiving the last chunk of data otherwise we have to chunk some more data*/
	JC_RETCODE			eRetCode;		/**< Specifies retcode*/
	EPOP3_REQUEST_TYPE	eRequestType;	/**< Specifies the enumaration of pop3 request types*/
	POP3_STATUS			eStatus ;		/**< Specifies the profile status*/
	JC_UINT32			uiBytesRecvd ;	/**< Specifies the bytes of data is received at present transaction*/
	JC_UINT32			uiBytesSent ;	/**< Specifies the bytes of data is sent at present transaction*/
	JC_UINT32			uiTotalBytesToSend ;/**< Specifies the total no of bytes has to send */
	JC_UINT32			uiTotalBytestoRecv ;/**< Specifies the total no of bytes has to receive */
	JC_UINT32			uiBufferLen;	/**< Specifies the total buffer length*/
	JC_INT8				*pcBuffer ;		/**< Specifies the response buffer*/

} ST_POP3_RESPONSE_INFO;

/**
 * @brief This function initialize the pop3 and get the handle .
 * @param[out] vPop3Handle     Specifies the handle. 
 * @return  
 *		   - JC_RETCODE               Specifies pop3 is initialized successfully.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.
 *		   - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
	JC_RETCODE jdi_POP3Initialize (JC_HANDLE *vPop3Handle) ;

/**
 * @brief This function deinitialize the pop3 handle.
 * @param[in] vPop3Handle     Specifies the initialized pop3 Handle.
 * @return  
 *		   - JC_RETCODE               Specifies pop3 deinitialized successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
	JC_RETCODE jdi_POP3DeInitialize (JC_HANDLE *vPop3Handle) ;

/**
 * @brief This function adds the new profile to pop3 handle.
 * @param[in] vPop3Handle     Specifies the initialized pop3 Handle.
 * @param[in] pstProfileInfo  Specifies the profile data details to be add.
 * @return  
 *		   - JC_RETCODE               Specifies added profile successfully.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_POP3_PROFILE_EXISTS Specifies profile already exists.	
 */
	JC_RETCODE jdi_POP3AddProfile (JC_HANDLE vPop3Handle, ST_POP3_PROFILE *pstProfileInfo) ;

/**
 * @brief This function updates the existing profile in pop3 handle based on profile id.
 * @param[in] vPop3Handle       Specifies the initialized pop3 Handle.
 * @param[in] uiProfileId  Specifies the profile id to update the profile.
 * @param[in] pstProfileInfo Specifies the profile data details to update the profile based on profile id.
 * @return  
 *		   - JC_RETCODE               Specifies updated profile successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies that if you are trying to update the profile that is not exists in the handle.
 */
	JC_RETCODE jdi_POP3UpdateProfile (JC_HANDLE 	vPop3Handle,ST_POP3_PROFILE *pstProfileInfo,JC_UINT32 	uiProfileId) ;


/**
 * @brief This function authenticate logged in user i.e, active profile
 * @param[in] vPop3Handle  Specifies the initialized pop3 Handle.
 * @param[in] uiProfileId  Specifies the profile id to update the profile.
 * @return  
 *		   - JC_OK				Specifies valid user
 *		   - JC_ERR_POP3_BUSY	Specifies last process is going on, POP3 busy. 
 */
JC_RETCODE	jdi_POP3AuthenticateProfile( JC_HANDLE	vPOP3Handle, JC_UINT32 uiProfileID ) ;

/**
 * @brief This function deletes the profile from pop3 handle based on profile id.
 * @param[in] vPop3Handle     Specifies the initialized pop3 Handle.
 * @param[in] uiProfileID  Specifies the profile id to delete the profile.
 * @return  
 *		   - JC_RETCODE               Specifies deleted profile successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies that if you are trying to delete the profile that is not exists in the handle.
 */
	JC_RETCODE jdi_POP3DeleteProfile (JC_HANDLE vPop3Handle, JC_UINT32 uiProfileID) ;

/**
 * @brief This function delete all profiles from pop3 handle. 
 * @param[in] vPop3Handle     Specifies the initialized pop3 Handle.
 * @return  
 *		   - JC_RETCODE               Specifies deleted all profiles successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE jdi_POP3DeleteAllProfile (JC_HANDLE vPop3Handle) ;
/**
 * @brief This function gets the remaining chunk of data to be sent.
 * @param[in] Handle     Specifies the initialized pop3 Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @param[in] pSendMoreData Specifies the send more data details.
 * @return  
 *		   - JC_RETCODE               Specifies sent more data successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.	
 *		   - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 *		   - JC_POP3_RECV_CONTINUE         Specifies pop3 is receving data from server is in progress.	
 */
JC_RETCODE jdi_POP3SendMoreData ( JC_HANDLE vPop3Handle, JC_UINT32 uiProfileID, 
								  ST_POP3_BUFFER *pstSendMoreData) ;


/**
 * @brief This function set the connection alive.
 * @param[in] vPop3Handle   Specifies the initialized pop3 Handle.
 * @param[in] uiProfileID	Specifies the profile id to get the profile.
 * @return  
 *		   - JC_ERR_POP3_BUSY        Specifies that pop3 is busy with some other task.
 */

	JC_RETCODE jdi_POP3KeepAlive(JC_HANDLE vPop3Handle, JC_UINT32 uiProfileID) ;
/**
 * @brief This function calls like HandleReceiveMessageList based on the profile request type.
 * @param[in] vPop3Handle     Specifies the initialized pop3 Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @param[in] pstPOP3ResponseInfo Specifies the response data.
 * @return  
 *		   - JC_RETCODE               Specifies get the response  successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 *		   - JC_POP3_RECV_CONTINUE         Specifies pop3 is receving data from server is in progress.	
 */
	JC_RETCODE jdi_POP3DataPoll ( JC_HANDLE vPop3Handle, JC_UINT32 uiProfileID, 
		                          ST_POP3_RESPONSE_INFO *pstPOP3ResponseInfo) ;

/**
 * @brief This function receives mail based on mail buffer request details.
 * @param[in] vPop3Handle     Specifies the initialized pop3 Handle.
 * @param[in] pstEmailBufferReq Specifes the mail buffer request to receive the mail.
 * @return  
 *		   - JC_RETCODE               Specifies received mail successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies that no pop3 profile is exists for this profile id 		
 */
	JC_RETCODE jdi_POP3ReceiveMail(JC_HANDLE vPop3Handle, ST_POP3_MAIL_BUFFER_REQ *pstEmailBufferReq) ;

/**
 * @brief This function receives message list.
 * @param[in] vPop3Handle    Specifies the initialized pop3 Handle.
 * @param[in] uiProfileID Specifes the profile id to receive whole message list.
 * @return  
 *		   - JC_RETCODE               Specifies received message list successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies that no pop3 profile is exists for this profile id 		
 *         - JC_ERR_POP3_BUSY        Specifies that pop3 is busy with some other task.
 */
	JC_RETCODE jdi_POP3ReceiveMsgList(JC_HANDLE 	vPop3Handle,JC_UINT32 uiProfileID) ; 

/**
 * @brief This function will receive the message headers.
 * @param[in] vPop3Handle    Specifies the initialized pop3 Handle.
 * @param[in] pstMsgHeaderReq Specifes the mail header request details to receive the mail headers.
 * @return  
 *		   - JC_RETCODE               Specifies received mail headers successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies that no pop3 profile is exists for this profile id 		
 *         - JC_ERR_POP3_BUSY        Specifies that pop3 is busy with some other task.
 */
	JC_RETCODE jdi_POP3ReceiveMsgHeader(JC_HANDLE 	vPop3Handle, ST_POP3_MSG_HEADER_REQ *pstMsgHeaderReq) ;

/**
 * @brief This function deletes the mail.
 * @param[in] vPop3Handle    Specifies the initialized pop3 Handle.
 * @param[in] pstUIDList   Specifes the uid list to delete the mail.
 * @param[in] uiProfileID Specifies that peofile id to delete the mail.
 * @return  
 *		   - JC_RETCODE               Specifies deleted mail successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies that no pop3 profile is exists for this profile id 		
 *         - JC_ERR_POP3_BUSY        Specifies that pop3 is busy with some other task.
 *         - JC_ERR_POP3_INVALID_STATE_FOR_MSGINDEX specifies that pop3 is in invalid state.
 */
	JC_RETCODE jdi_POP3DeleteMail ( JC_HANDLE vPop3Handle, JC_UINT32 uiProfileID,ST_POP3_UID_LIST *pstUIDList) ;

/**
 * @brief This function resets the profile in pop3 handle.
 * @param[in] vPop3Handle    Specifies the initialized pop3 Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @return  
 *		   - JC_RETCODE               Specifies reset the profile successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 */
	JC_RETCODE jdi_POP3Reset ( JC_HANDLE vPop3Handle, JC_UINT32 uiProfileID) ;

/**
 * @brief This function disconnects the Session with the server.
 * @param[in] vPop3Handle    Specifies the initialized pop3 Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @return  
 *		   - JC_RETCODE               Specifies pop3 diconnected successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 */
	JC_RETCODE jdi_POP3Disconnect(JC_HANDLE 	vPop3Handle , JC_UINT32 uiProfileID);

/**
 * @brief This function will set the chunk size in Protocol handle.
 * @param[in] vPop3Handle     Specifies the initialized pop3 Handle.
 * @param[in] uiChunkSize Specifies the chunk size, if the buffer length is greater than this size then we will chunk that data and send.
 * @return  
 *		   - JC_RETCODE               Specifies that chunking size is setting in ptotocol handle successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
	JC_RETCODE	jdi_POP3SetChunkSize(JC_HANDLE vPop3Handle,JC_UINT32 uiChunkSize) ;

/**
 * @brief This function will set the data connection status to pop3 handle.
 * @param[in] vPop3Handle     Specifies the initialized pop3 Handle.
 * @param[in] isDataConnAvailable Specifies the connection status.
 * @param[in] uiConnID    specifies Connection id to store in handle.
 * @return  
 *		   - JC_RETCODE               Specifies that set data connection status in handle.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
	JC_RETCODE	jdi_POP3SetDataConnStatus(JC_HANDLE vPop3Handle,JC_BOOLEAN isDataConnAvailable ,JC_UINT32 uiConnID) ;

/**
 * @brief This function Sets the STOP request for the ongoing process.
 * @param[in] vPop3Handle     Specifies the initialized pop3 Handle.
 * @param[in] uiProfileID Specifies the profile id to get the profile from handle.
 * @return  
 *		   - JC_RETCODE               Specifies that stopped the pop3 request successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_POP3_INVALID_STOP_REQUEST Specifies that invalid stop request incase of req type is equals to POP3_NONE.
 *         - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 */
	JC_RETCODE jdi_POP3SetStopRequest(JC_HANDLE vPop3Handle, JC_UINT32 uiProfileID)  ;

/**
 * @brief This function Sets the Socket Status of Socket connection.
 * @param[in] vPop3Handle   Specifies the initialized pop3 Handle.
 * @param[in] iSocketID   Specfies that socket id to get the socket data infomation from handle .
 * @param[in] eSocketStatus Specifies that socket status to be set in socket data.
 * @return  
 *		   - JC_RETCODE               Specifies that set connection state successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
	JC_RETCODE jdi_POP3SetConnectionState (JC_HANDLE vPop3Handle,JC_INT32 iSocketID, EPOP3_SOCKET_STATUS eSocketStatus)  ; 

/**
 * @brief This function reads the data from the socket and fills it in the socket queue.
 * @param[in] vPop3Handle   Specifies the initialized pop3 Handle.
 * @param[in] iSocketID   Specfies that socket id to get the socket data infomation from handle .
 * @param[in] pucReadBuffer Specifies the buffer.
 * @param[in] uiReadBufferLen Specifies the length of the buffer.
 * @return  
 *		   - JC_RETCODE               Specifies that read socket data successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies that memory allocation failure.
 */
	JC_RETCODE jdi_POP3ReadSocketData (JC_HANDLE vPop3Handle,JC_INT32 iSocketID, JC_INT8 *pucReadBuffer, JC_UINT32 uiReadBufferLen)  ; 

/**
 * @brief This function resets the socket profile.
 * @param[in] vPop3Handle   Specifies the initialized pop3 Handle.
 * @param[in] iSocketID   Specifies the socket id to get the socket data.
 * @return  
 *		   - JC_RETCODE               Specifies reset the socket profile successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
	JC_RETCODE jdi_POP3ResetSocketProfile(JC_HANDLE vPop3Handle,JC_INT32 iSocketID) ;

/**
 * @brief This function will check whether the input socket id corresponds to 
 * any socket used to POP3 Client Server Session.
 * @param[in] vPop3Handle   Specifies the initialized pop3 Handle.
 * @param[in] iSocketID   Specifies the socket id to get the socket data.
 * @return  
 *		   - E_TRUE     get the data successfully from socket handle 
 *         - E_FALSE    if socket data is NULL for corresponding socket id.
 */
	JC_BOOLEAN jdi_POP3IsSocket4POP3Session(JC_HANDLE vPop3Handle,JC_INT32 iSocketID);

/**
 * @brief This function connects establishes a socket connection to the required Host.
 * @param[in] vPop3Handle   Specifies the initialized pop3 Handle.
 * @param[in] pucHostName     Specifies the host name to connect.
 * @param[in] pucAddress      Specifies the ip address of that host.
 * @return  
 *		   - JC_RETCODE               Specifies connection established successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_MEMORY_ALLOCATION	Specifies that memory allocation failure.
 *         - JC_ERR_NET_DNS_RESOLVE    Specifies  that connection with host is fail.
 */
	JC_RETCODE jdi_POP3Connect(JC_HANDLE vPop3Handle ,const JC_UINT8	*pucHostName, JC_UINT8  *pucAddress) ;

/**
 * @brief This function gets the remaining chunk of mail data to be sent.
 * @param[in] vPop3Handle     Specifies the initialized pop3 Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @param[in] pstSendMoreData Specifies the send more data details.
 * @return  
 *		   - JC_RETCODE               Specifies sent more data successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.	
 *		   - JC_ERR_POP3_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 *		   - JC_IMAP4_RECV_CONTINUE         Specifies pop3 is receving data from server is in progress.	
 */
	JC_RETCODE jdi_POP3SendMoreUserData ( JC_HANDLE vPop3Handle,JC_UINT32 uiProfileID, ST_POP3_BUFFER *pstSendMoreData) ;
#endif	/*POP3_STACK_ENABLE*/

#ifdef __cplusplus
}
#endif

#endif
