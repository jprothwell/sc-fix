/***************************************************************************
*
* File Name : jdi_imap4.h
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
*
* File Description
* ---------------
*
* Purpose			:
*
*
*
* 
* Created By		:  Soumya M	
* Created Date		: 
*
* Current Revision	:  0.1
*
*
***************************************************************************
*
*
* Revision Details
* ----------------
* 
* 1. Modified By, Modified Date, Purpose
* 2. Modified By, Modified Date, Purpose
* 3. Modified By, Modified Date, Purpose
*
*
*
***************************************************************************/

/**
 * @file jdi_imap4.h
 * @ingroup imap4 
 * @brief Defines the exposed enumerations, structures and APIs of the imap4 that
 *		  can be used by the transport.
 */
#ifndef _JDI_IMAP4_H
#define _JDI_IMAP4_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <ddl.h>

#ifdef IMAP4_STACK_ENABLE
#include <jcal.h>
#include <jdi_transportmodule.h>
 
/**
 * @brief Specifies the maximum error array size
 */
 
#define MAX_ERROR_ARRAY_SIZE		30
		
/**
 * @brief Specifies the error code to indicate that imap profile is not exists
 */
#define JC_ERR_IMAP4_PROFILE_DOESNOT_EXISTS		TERMINAL_ERR_CODE(IMAP4_MODULE, 0x01)				
/**
 * @brief  Specifies the error code to indicate that IMAP4 Protocol Module is busy processing a IMAP4 Request.
 */
#define JC_ERR_IMAP4_BUSY					TERMINAL_ERR_CODE( IMAP4_MODULE, 0x02)
/**
 * @brief Specifies the error code to indicate that error in imap connection 
 */
#define JC_ERR_IMAP4_CONNECT				TERMINAL_ERR_CODE( IMAP4_MODULE, 0x03)				
/**
 * @brief Specifies the error code to indicate that error in imap disconnection
 */
#define JC_ERR_IMAP4_DISCONNECT				TERMINAL_ERR_CODE( IMAP4_MODULE, 0x04)				
/**
 * @brief Specifies the error code to indicate that authentication failure.
 */
#define JC_ERR_IMAP4_AUTHENTICATION			TERMINAL_ERR_CODE( IMAP4_MODULE, 0x05)				
/**
 * @brief Specifies the error code to indicate that mail box is locked.
 */
#define JC_ERR_IMAP4_MAILBOX_LOCKED			TERMINAL_ERR_CODE( IMAP4_MODULE, 0x06)				
/**
 * @brief Specifies the error code to indicate that general failure.
 */
#define JC_ERR_IMAP4_GENERAL_FAILURE		TERMINAL_ERR_CODE( IMAP4_MODULE, 0x07)				
/**
 * @brief Specifies the error code to indicate that message does not exists in server.
 */
#define JC_ERR_IMAP4_MSG_DOES_NOT_EXIST		TERMINAL_ERR_CODE( IMAP4_MODULE, 0x08)				
/**
 * @brief Specifies the error code to indicate that getting message header is failed.
 */
#define JC_ERR_IMAP4_GET_MSG_HDR				TERMINAL_ERR_CODE( IMAP4_MODULE, 0x09)				
/**
 * @brief Specifies the error code to indicate that getting message list for imap is failed.
 */
#define JC_ERR_IMAP4_GET_MSG_LIST			TERMINAL_ERR_CODE( IMAP4_MODULE, 0x0A)				
/**
 * @brief Specifies the error code to indicate that error in more data to send.
 */
#define JC_ERR_IMAP4_MORE_DATA_TO_SEND		TERMINAL_ERR_CODE( IMAP4_MODULE, 0x0B)
/**
 * @brief Specifies the error code to indicate that error in more data to receive.
 */
#define JC_ERR_IMAP4_MORE_DATA_TO_RECV		TERMINAL_ERR_CODE( IMAP4_MODULE, 0x0C)
/**
 * @brief Specifies the error code to indicate that receiving data is in progress.
 */
#define JC_ERR_IMAP4_RECV_CONTINUE			TERMINAL_ERR_CODE( IMAP4_MODULE, 0x0D)
/**
 * @brief Specifies the error code to indicate that sending data is in progress.
 */
#define JC_ERR_IMAP4_SEND_CONTINUE			TERMINAL_ERR_CODE( IMAP4_MODULE, 0x0E)
/**
 * @brief Specifies the error code to indicate that invalid stop request.
 */
#define JC_ERR_IMAP4_INVALID_STOP_REQUEST	TERMINAL_ERR_CODE( IMAP4_MODULE, 0x0F) 
/**
 * @brief Specifies the error code to indicate that error in selecting folder.
 */
#define JC_ERR_IMAP4_SELECT_FOLDER  	TERMINAL_ERR_CODE( IMAP4_MODULE, 0x10) 
/**
 * @brief Specifies the error code to indicate that error in receviving folder list.
 */
#define JC_ERR_IMAP4_FOLDER_LIST  	TERMINAL_ERR_CODE( IMAP4_MODULE, 0x11) 
/**
 * @brief Specifies the error code to indicate invalid flag settings.
 */
#define JC_ERR_IMAP4_INVALID_FLAG_SETTINGS	TERMINAL_ERR_CODE( IMAP4_MODULE, 0x12) 
/**
 * @brief Specifies the error code to indicate that error in receviving subscribe folder list.
 */
#define JC_ERR_IMAP4_SUBSCRIBED_FOLDER_LIST	TERMINAL_ERR_CODE( IMAP4_MODULE, 0x13) 
/**
 * @brief Specifies the error code to indicate that error in setting flag.
 */
#define JC_ERR_IMAP4_SET_FLAG			TERMINAL_ERR_CODE( IMAP4_MODULE, 0x14) 
/**
 * @brief Specifies the error code to indicate that error in purging the folder.
 */
#define JC_ERR_IMAP4_PURGE_FOLDER		TERMINAL_ERR_CODE( IMAP4_MODULE, 0x15) 
/**
 * @brief Specifies the error code to indicate that error in subscribing particular folder.
 */
#define JC_ERR_IMAP4_SUBSCRIBING_FOLDER 	TERMINAL_ERR_CODE( IMAP4_MODULE, 0x16) 
/**
 * @brief Specifies the error code to indicate that error in unsubscribing particular folder.
 */
#define JC_ERR_IMAP4_UNSUBSCRIBING_FOLDER	TERMINAL_ERR_CODE( IMAP4_MODULE, 0x17) 
/**
 * @brief Specifies the error code to indicate that error in creating folder.
 */
#define JC_ERR_IMAP4_CREATING_FOLDER		TERMINAL_ERR_CODE( IMAP4_MODULE, 0x18) 	
/**
 * @brief Specifies the error code to indicate that error in deleting the folder.
 */
#define JC_ERR_IMAP4_DELETING_FOLDER		TERMINAL_ERR_CODE( IMAP4_MODULE, 0x19)
/**
 * @brief Specifies the error code to indicate that error in renaming the folder.
 */
#define JC_ERR_IMAP4_RENAMING_FOLDER		TERMINAL_ERR_CODE( IMAP4_MODULE, 0x1A)
/**
 * @brief Specifies the error code to indicate that error in getting message body.
 */
#define	JC_ERR_IMAP4_GET_MSG				TERMINAL_ERR_CODE( IMAP4_MODULE, 0x1B)
/**
 * @brief Specifies the error code to indicate that error in receiving mail uid.
 */
#define	JC_ERR_IMAP4_SEARCH_MSG_UID			TERMINAL_ERR_CODE( IMAP4_MODULE, 0x1C) 
/**
 * @brief Specifies the error code to indicate that error in copying mail from one folder to another.
 */
#define JC_ERR_IMAP4_COPYING_MAIL           TERMINAL_ERR_CODE(IMAP4_MODULE,0x1D)
/**
 * @brief Specifies the error code to indicate that error in getting capability.
 */
#define JC_ERR_IMAP4_GET_CAPABILITY			TERMINAL_ERR_CODE(IMAP4_MODULE,0x1E)
#ifdef IMAP4_WITH_SSL
/**
 * @brief Specifies the error code to indicate that error in starttls command.
 */
#define JC_ERR_IMAP4_STARTTLS_FAILED TERMINAL_ERR_CODE(IMAP4_MODULE, 0x1F)
/**
 * @brief Specifies the error code to indicate that error in handshaking.
 */
#define JC_ERR_IMAP4_HANDSHAKE_FAILED TERMINAL_ERR_CODE(IMAP4_MODULE, 0x20)
#endif
/**
 * @brief Specifies the error code to indicate that error in receiving body structure.
 */
#define JC_ERR_IMAP4_RECV_BODYSTRUCTURE_FAILED     TERMINAL_ERR_CODE(IMAP4_MODULE, 0x21)
/**
 * @brief Specifies the error code to indicate that error in idling.
 */
#define JC_ERR_IMAP4_IDLE_FAILED				   TERMINAL_ERR_CODE(IMAP4_MODULE, 0x22)
/**
 * @brief Specifies the error code to indicate keep alive failed.
 */
#define JC_ERR_IMAP4_KEEPALIVE_FAILED				TERMINAL_ERR_CODE(IMAP4_MODULE, 0x23)
/**
 * @brief Specifies the error code to indicate parsing idle buffer is failed.
 */
#define JC_ERR_IMAP4_PARSE_IDLE_BUFFER             TERMINAL_ERR_CODE(IMAP4_MODULE, 0x24)
/**
 * @brief Specifies the error code to indicate the server does not support IDLE command
 */
#define JC_ERR_IMAP4_IDLE_UNSUPPORTED_BYSERVER     TERMINAL_ERR_CODE(IMAP4_MODULE, 0x25)
/**
 * @brief specifies to indicate connection is alive
 */
#define JC_IMAP4_CONNECTION_ALIVE					TERMINAL_ERR_CODE(IMAP4_MODULE, 0x26)

/**
  * @brief Enumeration values specifying the imap profile status.
  */
typedef enum _e_imap4_profile_status  
{
	E_IMAP4_UNCONNECTED =0,						/**< Specifies IMAP4 Protocol Module is NOT Connected to IMAP4 Server*/
	
	E_IMAP4_CONNECTING,							/**< Specifies imap is connecting */
	E_IMAP4_CONNECTED,							/**< Specifies imap connection is established with server*/
	E_IMAP4_CONNECT_FAILED,						/**< Specifies imap connection with the server is failed*/
	E_IMAP4_SENDING_USERDATA,					/**< Specifies imap sending user data*/
	E_IMAP4_DNS_FAILED,							/**< Specifies imap dns is failed */
	
	E_IMAP4_AUTHENTICATING,						/**< Specifies imap is authenticating */
	E_IMAP4_AUTHORIZED,							/**< Specifies imap is authenticated with server*/
	E_IMAP4_AUTHENTICATION_FAILED,				/**< Specifies imap authentication is failed*/

	E_IMAP4_CHECKING_CAPABILITY,				/**< Specifies imap status for checking capability*/
	
	E_IMAP4_RECEIVING_FOLDER_LIST,				/**< Specifies status for receiving imap folder list*/	
	E_IMAP4_FOLDER_LIST_RECVD,					/**< Specifies status for receiving imap folder list is completed*/	
	E_IMAP4_FOLDER_LIST_RECVD_FAILED,			/**< Specifies status for receiving imap folder list is failed*/	

	E_IMAP4_RECEIVING_SUBSCRIBED_FOLDER_LIST,	/**< Specifies status for receiving imap subscribe folder list*/	
	E_IMAP4_SUBSCRIBED_FOLDER_LIST_RECVD,		/**< Specifies status for receiving imap subscribe folder list is completed*/	
	E_IMAP4_SUBSCRIBED_FOLDER_LIST_RECVD_FAILED,/**< Specifies status for receiving imap folder list is failed*/	

	E_IMAP4_CREATING_FOLDER,					/**< Specifies status for creating imap folder*/					
	E_IMAP4_FOLDER_CREATED ,					/**< Specifies status for creating imap folder in server is successful*/				
	E_IMAP4_CREATE_FOLDER_FAILED ,				/**< Specifies status for creating imap folder is failed*/

	E_IMAP4_DELETING_FOLDER,					/**< Specifies status for deleting imap folder*/		
	E_IMAP4_FOLDER_DELETED ,					/**< Specifies status for deleting imap folder in server is successful*/
	E_IMAP4_FOLDER_DELETE_FAILED,				/**< Specifies status for deleting imap folder is failed*/

	E_IMAP4_RENAMING_FOLDER,					/**< Specifies status for renaming imap folder*/		
	E_IMAP4_FOLDER_RENAMED ,					/**< Specifies status for renaming imap folder is successful*/
	E_IMAP4_FOLDER_RENAME_FAILED,				/**< Specifies status for renaming imap folder is failed*/

	E_IMAP4_SELECTING_FOLDER,					/**< Specifies status for selecting imap folder*/
	E_IMAP4_FOLDER_SELECTED,					/**< Specifies status for selecting imap folder is successful*/
	E_IMAP4_SELECT_FOLDER_FAILED,				/**< Specifies status for selecting imap folder is failed*/
	
	E_IMAP4_SUBSCRIBING_FOLDER,					/**< Specifies status for subscribing imap folder*/
	E_IMAP4_FOLDER_SUBSCRIBED ,					/**< Specifies status for subscribing imap folder is successful*/
	E_IMAP4_FOLDER_SUBSCRIBED_FAILED ,			/**< Specifies status for subscribing imap folder is failed*/

	E_IMAP4_UNSUBSCRIBING_FOLDER,				/**< Specifies status for unsubscribing imap folder*/
	E_IMAP4_FOLDER_UNSUBSCRIBED ,				/**< Specifies status for unsubscribing imap folder is successful*/
	E_IMAP4_FOLDER_UNSUBSCRIBED_FAILED ,		/**< Specifies status for unsubscribing imap folder is failed*/
	
	E_IMAP4_RECEIVING_MSGLIST,					/**< Specifies status for receving message list*/
	E_IMAP4_MSGLIST_RECVD,						/**< Specifies status for receving message list is successful*/
	E_IMAP4_RECV_UIDL_FAILED,					/**< Specifies status for receving message list is failed*/

	E_IMAP4_SETTING_MAIL_FLAG,					/**< Specifies status for setting message flag*/
	E_IMAP4_MAIL_FLAG_SET,						/**< Specifies status for setting message flag is successful*/
	E_IMAP4_MAIL_FLAG_SET_FAILED,				/**< Specifies status for setting message flag is failed*/

	E_IMAP4_RECEIVING_BODYSTRUCTURE,			/**< Specifies status for receving body structure of the mail*/
	E_IMAP4_BODYSTRUCTURE_RECVD,				/**< Specifies status for receving body structure of the mail is successful*/
	E_IMAP4_RECV_BODYSTRUCTURE_FAILED,			/**< Specifies status for receving body structure of the mail is failed*/

	E_IMAP4_RECEIVING_BODY_PART,				/**< Specifies status for receving body part of the mail*/

	E_IMAP4_RECEIVING_MSGHDR,					/**< Specifies status for receving message header of the mail*/
	E_IMAP4_MSGHDR_RECVD,						/**< Specifies status for receving message header of the mail is successful*/
	E_IMAP4_RECV_MAILHDR_FAILED,				/**< Specifies status for receving message header of the mail is failed*/

	E_IMAP4_RECEIVING_MSG,						/**< Specifies status for receving message*/
	E_IMAP4_MSG_RECVD,							/**< Specifies status for receving message is successful*/				
	E_IMAP4_RECV_MAIL_FAILED ,					/**< Specifies status for receving message is failed*/

	E_IMAP4_PURGING_FOLDER,						/**< Specifies status for purging folder*/
	E_IMAP4_FOLDER_PURGED,						/**< Specifies status for purging folder is successful*/
	E_IMAP4_FOLDER_PURGED_FAILED,				/**< Specifies status for purging folder is failed */

    E_IMAP4_FILTERING_MAIL,						/**< Specifies status for filtering mail*/
	E_IMAP4_MAIL_FILTERED,						/**< Specifies status for filtering mail is successful*/
	
	E_IMAP4_COPING_MAIL,						/**< Specifies status for copying mail*/
	E_IMAP4_MAIL_COPIED,						/**< Specifies status for copying mail is successful*/
	E_IMAP4_COPING_MAIL_FAILED,					/**< Specifies status for copying mail is failed*/

	E_IMAP4_SAVING_MAIL,						/**< Specifies status for saving mail*/
	E_IMAP4_MAIL_SAVED,							/**< Specifies status for saving mail is successful*/
	E_IMAP4_SAVING_MAIL_FAILED,					/**< Specifies status for saving mail is failed*/

	E_IMAP4_APPENDING_MAIL,						/**< Specifies status for append mail*/
	E_IMAP4_MAIL_APPENDED,						/**< Specifies status for append mail is successful*/
	E_IMAP4_MAIL_APPENDED_FAILED,				/**< Specifies status for append mail is failed*/

	E_IMAP4_FOLDER_IDLING,						/**< specifies imap status for folder idling is in process*/
	E_IMAP4_FOLDER_IDLE_COMPLETED,				/**< specifies imap status for folder idled */
	E_IMAP4_IDLE_FAILED,						/**< specifies imap status for idle is failed */

	E_IMAP4_KEEPING_ALIVE,						/**< Specifies imap sending NOOP command*/
	E_IMAP4_IDLE_KEEPING_ALIVE,                 /**< Specifies imap idle keeping alive after sending noop*/
	E_IMAP4_CONNECTION_ALIVE,					/**< Specifies imap Connection Alive*/
	E_IMAP4_KEEPALIVE_FAILED,                   /**< Specifies imap keep alive fail*/
	E_IMAP4_DISCONNECTING,						/**< Specifies imap status for disconnecting*/
	E_IMAP4_DISCONNECTED,						/**< Specifies imap status for disconnected successfully*/
	E_IMAP4_DISCONNECT_FAILED,					/**< Specifies imap status for disconnect failure*/
#ifdef IMAP4_WITH_SSL
	E_IMAP4_STARTING_SSL,						/**< when STARTTLS is sent*/
	E_IMAP4_SSL_HANDSHAKE,						/**< when chipher suite negotiation is on*/
	E_IMAP4_SSL_OK,								/**< ssl negotiation is success*/
	E_IMAP4_STARTTLS_FAILED ,					/**< specifies imap status for stls failed*/
	E_IMAP4_HANDSHAKE_FAILED,					/**< specifies imap status for hansshake failed*/
#endif
	E_IMAP4_PROFILE_DELETED						/**< specifies imap status for profile deleted */
}EIMAP4_STATUS;

/**
 * @brief Enumeration values specifying the request types for imap profile.
 */
typedef enum _e_imap4_request_type
{
	 E_IMAP4_REQUEST_NONE = 0 ,					/**< Specifies no request is required */
	 E_IMAP4_CONNECT_REQUEST,					/**< Specifies the request to connect*/
	 E_IMAP4_DELETE_PROFILE_REQUEST,			/**< Specifies the request to delete the profile*/
	 E_IMAP4_RECV_FOLDER_LIST_REQUEST,			/**< Specifies the request for receiving folder list*/
     E_IMAP4_SUBSCRIBE_FOLDER_REQUEST,		    /**< Specifies the request for subscribe the folder*/
	 E_IMAP4_UNSUBSCRIBE_FOLDER_REQUEST,		/**< Specifies the request for unsubscribe the folder*/
	 E_IMAP4_RECV_MSGLIST_REQUEST,				/**< Specifies the request for receiving message list*/
	 E_IMAP4_RECV_MSG_HEADER_REQUEST,			/**< Specifies the request for receiving message header list*/
	 E_IMAP4_RECV_MSG_BODY_STRUCTURE_REQUEST,	/**< Specifies the request for receiving message body structure list*/
	 E_IMAP4_RECV_MAIL_REQUEST,					/**< Specifies the request for receiving mail*/
	 E_IMAP4_RECV_SPECIFIC_PART_MAIL_REQUEST,	/**< Specifies the request for receiving specific part of the mail*/
	 E_IMAP4_SET_FLAG_REQUEST,					/**< Specifies the request for setting flag*/
	 E_IMAP4_PURGE_REQUEST,						/**< Specifies the request for purging the folder*/
	 E_IMAP4_CREATE_FOLDER_REQUEST,				/**< Specifies the request for creating imap folder*/ 
	 E_IMAP4_DELETE_FOLDER_REQUEST,				/**< Specifies the request for deleting imap folder*/
	 E_IMAP4_RENAME_FOLDER_REQUEST,				/**< Specifies the request for renaming imap folder*/
     E_IMAP4_RECV_FILTER_MAIL_REQUEST,			/**< Specifies the request for receiving filter mail*/
	 E_IMAP4_RECV_SUBSCRIBED_FOLDER_LIST_REQUEST,/**< Specifies the request for receiving subscribe folder list*/
	 E_IMAP4_COPY_MOVE_MAIL_REQUEST ,			/**< Specifies the request for copy move message*/
	 E_IMAP4_APPEND_MAIL_REQUEST,				/**< Specifies the request for append the mail*/
	 E_IMAP4_KEEPALIVE_REQUEST,					/**< Specifies the request for keep alive list*/
	 E_IMAP4_IDLE_REQUEST,						/**< Specifies the request for set IDLE on the server*/
	 E_IMAP4_KEEP_ALIVE_REQUEST,				/**< Specifies the request for connection keep alive */				
	 E_IMAP4_AUTH_PROFILE_REQUEST,				/**< Specifies the request for authorization */				
	 E_IMAP4_DISCONNECT_REQUEST					/**< Specifies the request for disconnect*/
}EIMAP4_REQUEST_TYPE;

/**
 * @brief Enumeration values specifying the imap socket status.
 */
typedef enum _e_imap4_socket_status
{
	E_IMAP4_SOCKET_UNCONNECTED = 0,			/**< Specifies imap socket is not connected */
	E_IMAP4_SOCKET_DNS_RESOLVING,			/**< Specifies imap dns resolving the connection*/
	E_IMAP4_SOCKET_DNS_FAILED,				/**< Specifies imap socket status dns failed */
	E_IMAP4_SOCKET_CONNECTING,				/**< Specifies imap socket status conenction is in progress*/
	E_IMAP4_SOCKET_CONNECTED,				/**< Specifies imap socket status connected successfully*/
	E_IMAP4_SOCKET_CONNECTION_FAILED,		/**< Specifies imap socket status connection failure*/
	E_IMAP4_SOCKET_SENDING_DATA,			/**< Specifies imap socket status sending data is in progress*/
	E_IMAP4_SOCKET_SENT_DATA,				/**< Specifies imap socket data has been sent*/
	E_IMAP4_SOCKET_RECEIVING_DATA,			/**< Specifies imap socket receiving data is in progress */
	E_IMAP4_SOCKET_RECEIVED_DATA,			/**< Specifies imap socket received data successfully */
	E_IMAP4_SOCKET_IDLE_RECEIVING_DATA,		 /**< Specifies imap socket idle receiving for updates from server*/
	E_IMAP4_SOCKET_CLOSED					/**< Specifies imap socket status for closing the connection */
} EIMAP4_SOCKET_STATUS ; 

/**
 * @brief Enumeration values specifying the time out types for imap.
 */
typedef enum _e_imap4_timeout_type
{
	E_IMAP4_CONNECT_TIMEOUT = 0,		/**< Specifies imap connection is timed out*/
	E_IMAP4_SEND_TIME_OUT,				/**< Specifies imap for sending data is timed out*/
	E_IMAP4_RECV_TIME_OUT				/**< Specifies imap for receiving data is timed out*/
} EIMAP4TimeoutType ; 

/**
 * @brief Defines structure to hold information of imap profile.
 */
typedef struct _st_imap4_profile 
{
	JC_UINT32	uiProfileID ;			/**< Specifies profile id */
	JC_INT8 	*pszUserID ;			/**< Specifies user id to authenticate*/
	JC_INT8 	*pszPassword ;			/**< Specifies password to authenticate*/
	JC_INT8 	*pszServerName;			/**< Specifies server name for this profile to receive mails */
	JC_UINT16	uhPortNumber;			/**< Specifies port number*/
	JC_BOOLEAN  bIsLeaveCopyOnServer ;	/**< if it is true then we will not delete the mail from server while receving,if false we will delete mails on server*/
	JC_BOOLEAN  bIsAuthReqd;			/**< if it is true then we should provide user id and password otherwise not require*/
#ifdef IMAP4_WITH_SSL						
	JC_UINT8 ucSSLType ;				/**< Specifies security type (SSL type)*/
#endif	
}ST_IMAP4_PROFILE ;

/**
 * @brief Defines structure to hold information of message list details.
 */
typedef struct _st_imap4_msg_list_req
{
	JC_UINT32	uiProfileID;			/**< Specifies profile id */
	JC_BOOLEAN	bIsAllMessages ;		/**< if it is true then retreive all message otherwise retrieve only recent messages*/
#ifdef EMAIL_CORE_WITH_INDICATION_HANDLING
	JC_BOOLEAN	bIsExpunged ;			/**< Specifies true if client is sending request for recv msg list after folder expunged successfully */
#endif
	JC_INT8		*pszFolderName ;		/**< Specifies the folder name to get the messages from*/
}ST_IMAP4_MSG_LIST_REQ;

/**
 * @brief Defines structure to hold information of creating folder details.
 */
typedef struct _st_imap4_create_folder_req
{
	JC_UINT32	uiProfileID;			/**< Specifies profile id */
	JC_INT8		*pszFolderList ;		/**< Specifies folder name to create in server*/
}ST_IMAP4_CREATE_FOLDER_REQ ;

/**
 * @brief Defines structure to hold information of deleting folder details.
 */
typedef struct _st_imap4_delete_folder_req
{
	JC_UINT32	uiProfileID;			/**< Specifies profile id */
	JC_INT8		*pszFolderName ;		/**< Specifies folder name to delete in server*/
}ST_IMAP4_DELETE_FOLDER_REQ ;
/**
 * @brief Defines structure to hold information of renaming folder details.
 */
typedef struct _st_imap4_rename_folder_req
{
	JC_UINT32	uiProfileID;			/**< Specifies profile id */
	JC_INT8		*pszOldFolderName ;		/**< Specifies old folder name to replace*/
	JC_INT8		*pszNewFolderName ;		/**< Specifies new folder name */
}ST_IMAP4_RENAME_FOLDER_REQ ;
/**
 * @brief Defines structure to hold information of message header details.
 */
typedef struct _st_imap4_header_req
{
	JC_UINT32	uiProfileID;			/**< Specifies profile id */
	JC_INT8		*pszMsgID ;				/**< Specifies message id to get the headers*/
	JC_INT8		*pszFolderName;			/**< Specifies foldername*/
} ST_IMAP4_MSG_HEADER_REQ;
/**
 * @brief Defines structure to hold information of subscribe folder details.
 */
typedef struct _st_imap4_subscribe_folder_req
{
	JC_UINT32 uiProfileID ;				/**< Specifies profile id */
	JC_INT8   *pszFolderList ;			/**< Specifies the folder name to subscribe*/
}ST_IMAP4_SUBSCRIBE_FOLDER_REQ ;
/**
 * @brief Defines structure to hold information of unsubscribe folder details.
 */
typedef struct _st_imap4_unsubscribe_folder_req
{
	JC_UINT32 uiProfileID ;				/**< Specifies profile id */
	JC_INT8   *pszFolderList ;			/**< Specifies the folder name to unsubscribe*/
}ST_IMAP4_UNSUBSCRIBE_FOLDER_REQ ;
/**
 * @brief Defines structure to hold information of copy move mail details.
 */
typedef struct _st_imap4_copy_move_mail_req
{
	JC_UINT32 uiProfileID ;				/**< Specifies profile id */
	JC_INT8   *pszSourceFolderName ;	/**< Specifies source foldername*/
	JC_INT8   *pszDestFolderName ;		/**< Specifies destination foldername */
	JC_INT8   *pszMsgID ;				/**< Specifies messageid to copy/move the message*/
	JC_BOOLEAN bIsCopyOrMove ;			/**< if true then copy move the message otherwise dont do*/
}ST_IMAP4_COPY_MOVE_MAIL_REQ ;

/**
 * @brief Defines structure to hold information of save mail details.
 */
typedef struct _st_imap4_save_mail_req
{
	JC_UINT32 uiProfileID ;				/**< Specifies profile id */
	JC_INT8   *pszFolderName ;			/**< Specifies the folder name*/
	JC_INT8   *pBuffer ;				/**< Specifies the buffer that contains body and headers*/
    JC_UINT32  uiMsgSize ;				/**< Specifies message size*/
	JC_UINT32	uiTotalBufferLen;		/**< Specifies total buffer length*/
	JC_UINT32   puiRcvdBufferSize;		/**< Specifies received buffer length of current chunking data*/
	JC_UINT32	uiRemainChunkLen;		/**< Specifies remaining chuncking data to receive*/
	JC_BOOLEAN  bHasMoreData;			/**< if it is true then then recevive more data other wise not require*/
	JC_UINT16  Flag2Set ;				/**< Specifies flag to set*/

}ST_IMAP4_SAVE_MAIL_REQ ;

/**
 * @brief Defines structure to hold information of set flag details.
 */
typedef struct _st_imap4_set_flag_req
{
	JC_UINT32	uiProfileID;			/**< Specifies profile id */
	JC_INT8		*pszUID ;				/**< Specifies uid */
	JC_INT8		*pszFolderName;			/**< Specifies the folder name*/
	JC_UINT16	 uhflags2Set ;			/**< Specifies the flag to set for message in server*/
	JC_INT8     *pUserDefinedFlags;		/**< Specifies the user defined flags*/
} ST_IMAP4_SET_FLAG_REQ ;

/**
 * @brief Defines structure to hold information of purge folder details.
 */
typedef struct _st_imap4_purge_req
{
	JC_UINT32	uiProfileID ;			/**< Specifies profile id */
	JC_INT8		*pszFolderName ;		/**< Specifies the folder name to purge*/
} ST_IMAP4_PURGE_REQ ;
/**
 * @brief Defines structure to hold information of mail buffer. 
 */
typedef struct _st_imap4_mail_buffer_req
{
	JC_UINT32	uiProfileID;			/**< Specifies the profile id*/
	JC_UINT32	uiMsgIndex;				/**< Specifies the message index*/
	JC_UINT32	uiMailSize;				/**< Specifies the mail size*/
	JC_INT8		*pszMsgID ;				/**< Specifies the message id of the mail to receive*/
	JC_INT8		*pszFolderName;			/**< Specifies the folder name from where we are receiving mail*/
	JC_BOOLEAN  bWithAttachment ;		/**< if it is true then retrieve the full body other wise retrieve only body structure*/
} ST_IMAP4_MAIL_BUFFER_REQ;
/**
 * @brief Defines structure to hold information of mail body structure details. 
 */
typedef struct _st_imap4_mail_bodystructure_req
{
	JC_UINT32	uiProfileID;			/**< Specifies the profile id*/
	JC_INT8		*pszMsgID ;				/**< Specifies the message id*/
	JC_INT8		*pszFolderName;			/**< Specifies the folder name*/
} ST_IMAP4_MAIL_BODYSTRUCTURE_REQ;
/**
 * @brief Defines structure to hold information of specific mail buffer details. 
 */
typedef struct _st_imap4_specific_mail_buffer_req
{
	JC_UINT32	uiProfileID;			/**< Specifies the profile id*/
	JC_UINT32	uiMailSize;				/**< Specifies the mail size*/
	JC_INT8		*pszMsgID ;				/**< Specifies the message id*/
	JC_INT8		*pszFolderName;			/**< Specifies the folder name*/
	JC_INT8     *pSpecificpart;			/**< Specifies only specific part of the mail*/
} ST_IMAP4_SPECIFIC_MAIL_BUFFER_REQ ;
/**
 * @brief Defines structure to hold information of sending more pop3 data. 
 */
typedef struct _st_imap4_send_more_data
{
	JC_UINT32			uiProfileID;	/**< Specifies the profile id*/
	EIMAP4_REQUEST_TYPE	eRequestType;	/**< Specifies the request type*/
	JC_UINT32			uiOffset;		/**< Specifies the user data length already received by transport in previous chunks */
	JC_UINT32			uiBufferLen;	/**< Specifies buffer length of the current chunking data */
	JC_INT8				*pszBuffer;		/**< Specifies mail buffer to be send more data */		
} ST_IMAP4_SEND_MORE_DATA;
/**
 * @brief Enumeration values specifying the imap mail flag type.
 */
typedef enum _e_imap4_mail_flag_type
{
	E_IMAP_NONE			= 0x00 ,		/**< Specifies flag type is none*/
	E_IMAP_ANSWERED		= 0x01 ,		/**< Specifies flag type is answered*/
	E_IMAP_UNANSWERED	= 0x02 ,		/**< Specifies flag type is unanswered*/
	E_IMAP_FLAGGED		= 0x04 ,		/**< Specifies flag type is flagged*/
	E_IMAP_UNFLAGGED	= 0x08 ,		/**< Specifies flag type is unflagged*/
	E_IMAP_DELETED		= 0x10 ,		/**< Specifies flag type is deleted*/
	E_IMAP_UNDELETED    = 0x20 ,		/**< Specifies flag type is undeleted*/
	E_IMAP_SEEN			= 0x40 ,		/**< Specifies flag type is seen*/
	E_IMAP_UNSEEN		= 0x80 ,		/**< Specifies flag type is unseen*/
	E_IMAP_DRAFT		= 0x100 ,		/**< Specifies flag type is draft*/
	E_IMAP_UNDRAFT		= 0x200 ,		/**< Specifies flag type is undraft*/
	E_IMAP_RECENT		= 0x400 ,		/**< Specifies flag type is recent*/
	E_IMAP_OLD			= 0x800			/**< Specifies flag type is old*/
}EMailIMAP4FlagType ;

/**
 * @brief Defines structure to hold information of filter mails details. 
 */
typedef struct _st_imap4_filtermail_req
{
	JC_UINT32					uiSessionID ;				/**< Specifies the session id*/
	JC_UINT32					uiProfileID ;				/**< Specifies the profile id*/
	JC_INT8						*pszFolderName ;			/**< Specifies the folder name from where we are filtering mails*/
	JC_INT8						*pFromContains ;			/**< Specifies the from field buffer that contains*/
    JC_INT8						*pFromDoesNotContain ;		/**< Specifies the from field buffer that doesnot contains*/
	JC_INT8                     *pToContains ;				/**< Specifies the to field buffer that contains*/
    JC_INT8                     *pToDoesNotContain;			/**< Specifies the to field buffer that doesnot contains*/
	JC_INT8                     *pCcContains ;				/**< Specifies the cc field buffer that contains*/
    JC_INT8                     *pCcDoesNotContain ;		/**< Specifies the cc field buffer that doesnot contains*/
	JC_INT8                     *pBCcContains ;				/**< Specifies the bcc field buffer that contains*/
    JC_INT8                     *pBCcDoesNotContain ;		/**< Specifies the bcc field buffer that doesnot contains*/
	JC_INT8                     *pSubjectContains ;			/**< Specifies the subject field buffer that contains*/
    JC_INT8                     *pSubjectDoesNotContain ;	/**< Specifies the subject field buffer that doesnot contains*/
    JC_INT8                     *pContainsKeyWordFlag ;		/**< Specifies the key word flag field buffer that contains*/
    JC_INT8                     *pDoesNotContainKeyWordFlag ; /**< Specifies the key word flag field buffer that doesnot contains*/
    JC_INT8                     *pContainsKeyWord ;			/**< Specifies the key word field buffer that contains*/
    JC_INT8                     *pDoesNotContainKeyWord ;	/**< Specifies the key word field buffer that doesnot contains*/
	JC_UINT32					uiMinSize ;					/**< Specifies the minimum size*/
	JC_UINT32					uiMaxSize ;					/**< Specifies the maximum size*/
	JC_UINT16					eEmailFlag	;				/**< Specifies the mail flag*/
	JC_INT8						*pszStartDate ;				/**< Specifies the start date*/
	JC_UINT8                    uiDateOption;				/**< Specifies the date option*/
}ST_IMAP4_FILTERMAIL_REQ ; 

/**
 * @brief Defines structure to IMAP4 IDLe Request Inputs. 
 */
typedef struct _st_imap4_idle_req
{
	JC_UINT32					uiSessionID ;				/**< Specifies the session id*/
	JC_UINT32					uiProfileID ;				/**< Specifies the profile id*/
	JC_INT8						*pszFolderName ;			/**< Specifies the folder name from where we are filtering mails*/
}ST_IMAP4_IDLE_REQ ;
/**
 * @brief Defines structure to hold information of imap response. 
 */
typedef  struct _st_imap4_response_info
{
	JC_UINT32			uiProfileID ;			/**< Specifies the profile id*/
	JC_UINT32			uiSessionID ;			/**< Specifies the session id*/
	JC_INT8				*pszUserData ;			/**< Store the Folder name and other information respective to the request */
	JC_BOOLEAN			bIsResponseRcvd ;		/**< if true then response is received otherwise not received*/
	JC_RETCODE			eRetCode ;				/**< Specifies retcode*/
	EIMAP4_REQUEST_TYPE	eRequestType ;			/**< Specifies the enumaration of imap request type*/
	EIMAP4_STATUS		eStatus ;				/**< Specifies the profile status*/
	JC_BOOLEAN          bIsLastChunk ;			/**< if it is true then receiving the last chunk of data otherwise we have to chunk some more data*/
	JC_BOOLEAN			bIsFoundHeaderInBuffer ;/**< if 0 then not found header in buffer other wise found */
#ifdef EMAIL_CORE_WITH_INDICATION_HANDLING
	JC_BOOLEAN			bIsExpunged ;			/**< Specifies true if client is sending request for recv msg list after folder expunged successfully */
#endif 
	JC_UINT32			uiBytesRecvd ;			/**< Specifies the bytes of data is received at present transaction*/
	JC_UINT32			uiBytesSent ;			/**< Specifies the bytes of data is sent at present transaction*/
	JC_UINT32			uiTotalBytesToSend ;	/**< Specifies the total no of bytes has to send */
	JC_UINT32			uiTotalBytestoRecv ;	/**< Specifies the total no of bytes has to receive */
	JC_UINT32			uiBufferLen;			/**< Specifies the total buffer length*/
	JC_INT8				*pBuffer ;				/**< Specifies the response buffer*/
	JC_INT8				*pcFolderName ;			/**< Specifies the folder name*/
} ST_IMAP4_RESPONSE_INFO;

/**
 * @brief This function initialize the imap and get the handle .
 * @param[out] vImap4Handle     Specifies the handle. 
 * @return  
 *		   - JC_RETCODE               Specifies imap is initialized successfully.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.
 *		   - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE jdi_IMAP4Initialize (JC_HANDLE *vImap4Handle) ;

/**
 * @brief This function deinitializes the imap handle.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @return  
 *		   - JC_RETCODE               Specifies imap deinitialized successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE jdi_IMAP4DeInitialize (JC_HANDLE *vImap4Handle) ;

/**
 * @brief This function adds the new profile to imap handle.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] pstProfileInfo  Specifies the profile data details to be add.
 * @return  
 *		   - JC_RETCODE               Specifies added profile successfully.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_EXISTS Specifies profile already exists.	
 */
JC_RETCODE jdi_IMAP4AddProfile (JC_HANDLE vImap4Handle, ST_IMAP4_PROFILE *pstProfileInfo) ;

/**
 * @brief This function updates the existing profile in imap handle based on profile id.
 * @param[in] vImap4Handle       The Initialized IMAP Handle.
 * @param[in] uiProfileId  Specifies the profile id to update the profile.
 * @param[in] pstProfileInfo Specifies the profile data details to update the profile based on profile id.
 * @return  
 *		   - JC_RETCODE               Specifies updated profile successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies that if you are trying to update the profile that is not exists in the handle.
 */
JC_RETCODE jdi_IMAP4UpdateProfile (JC_HANDLE vImap4Handle, ST_IMAP4_PROFILE *pstProfileInfo,JC_UINT32 uiProfileId) ;

/**
 * @brief This function deletes the profile from imap handle based on profile id.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] uiProfileID  Specifies the profile id to delete the profile.
 * @return  
 *		   - JC_RETCODE               Specifies deleted profile successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies that if you are trying to delete the profile that is not exists in the handle.
 */
JC_RETCODE jdi_IMAP4DeleteProfile (JC_HANDLE vImap4Handle, JC_UINT32 uiProfileID) ;

/**
 * @brief This function delete all profiles from imap handle. 
 * @param[in] Handle     The Initialized IMAP Handle.
 * @return  
 *		   - JC_RETCODE               Specifies deleted all profiles successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE jdi_IMAP4DeleteAllProfile (JC_HANDLE vImap4Handle) ;

/**
 * @brief This function gets the remaining chunk of data to be sent.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @param[in] pstSendMoreData Specifies the send more data details.
 * @return  
 *		   - JC_RETCODE               Specifies sent more data successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.	
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 *		   - JC_IMAP4_RECV_CONTINUE         Specifies pop3 is receving data from server is in progress.	
 */
JC_RETCODE jdi_IMAP4SendMoreData (JC_HANDLE vImap4Handle, JC_UINT32 uiProfileID, ST_IMAP4_SEND_MORE_DATA *pstSendMoreData) ;

/**
 * @brief This function calls the Imap4HandleReceiveFolderList and Imap4HandleCreateFolder based on the profile request type.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @param[in] pstIMAP4ResponseInfo Specifies the response data.
 * @return  
 *		   - JC_RETCODE               Specifies get the response  successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 *		   - JC_IMAP4_RECV_CONTINUE         Specifies imap4 is receving data from server is in progress.	
 */
JC_RETCODE jdi_IMAP4DataPoll (JC_HANDLE vImap4Handle, JC_UINT32 uiProfileID, ST_IMAP4_RESPONSE_INFO *pstIMAP4ResponseInfo) ;

/**
 * @brief This function receives all the folders from server based on profile id.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] uiProfileID Specifes the profile id to receive all folders.
 * @return  
 *		   - JC_RETCODE               Specifies received folder list successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies that no imap4 profile is exists for this profile id 		
 */
JC_RETCODE jdi_IMAP4ReceiveFolderList(JC_HANDLE vImap4Handle, JC_UINT32 uiProfileID) ;

/**
 * @brief This function receives message list.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] pstMsgListReq Specifes the message list request details.
 * @return  
 *		   - JC_RETCODE               Specifies received message list successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies that no imap4 profile is exists for this profile id 		
 *         - JC_ERR_IMAP4_BUSY        Specifies that imap4 is busy with some other task.
 */
JC_RETCODE jdi_IMAP4ReceiveMsgList(JC_HANDLE vImap4Handle, ST_IMAP4_MSG_LIST_REQ *pstMsgListReq) ;

/**
 * @brief This function will receive the message headers.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] pstMsgHeaderReq Specifes the mail header request details to receive the mail headers.
 * @return  
 *		   - JC_RETCODE               Specifies received mail headers successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies that no imap4 profile is exists for this profile id 		
 *         - JC_ERR_IMAP4_BUSY        Specifies that imap4 is busy with some other task.
 */
JC_RETCODE jdi_IMAP4ReceiveMsgHeader(JC_HANDLE vImap4Handle, ST_IMAP4_MSG_HEADER_REQ *pstMsgHeaderReq) ;

/**
 * @brief This function receives mail based on mail buffer request details.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] pEmailBufferReq Specifes the mail buffer request to receive the mail.
 * @return  
 *		   - JC_RETCODE               Specifies received mail successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies that no imap4 profile is exists for this profile id 		
 */
JC_RETCODE jdi_IMAP4ReceiveMail(JC_HANDLE vImap4Handle, ST_IMAP4_MAIL_BUFFER_REQ *pEmailBufferReq) ;

/**
 * @brief This function resets the profile in imap handle.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @return  
 *		   - JC_RETCODE               Specifies reset the profile successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 */
JC_RETCODE jdi_IMAP4Reset (JC_HANDLE vImap4Handle, JC_UINT32 uiProfileID) ;

/**
 * @brief This function Sets the the Timeout value in the Protocol Handle.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] eTimeOutType  Specifies the time out type.
 * @param[in] TimeOut     Specifies after how much time it has to come out
 * @return  

 *		   - JC_RETCODE               Specifies set time out for imap successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE jdi_IMAP4SetTimeOut (JC_HANDLE vImap4Handle, EIMAP4TimeoutType eTimeOutType,JC_UINT32 TimeOut) ;

/**
 * @brief This function will set the data connection status to imap handle.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] isDataConnAvailable Specifies the connection status.
 * @param[in] uiConnID    specifies Connection id to store in handle.
 * @return  
 *		   - JC_RETCODE               Specifies that set data connection status in handle.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE	jdi_IMAP4SetDataConnStatus(JC_HANDLE vImap4Handle, JC_BOOLEAN isDataConnAvailable,JC_UINT32 uiConnID) ;

/**
 * @brief This function will set the mail flag on server.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] pstSetFlagReq  Specifies the set flag request structure.
 * @return  
 *		   - JC_RETCODE               Specifies set mail flag on server is sccessful.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 */
JC_RETCODE jdi_IMAP4SetMailFlag(JC_HANDLE vImap4Handle, ST_IMAP4_SET_FLAG_REQ *pstSetFlagReq) ;

/**
 * @brief This function Sets the STOP request for the ongoing process.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] uiProfileID Specifies the profile id to get the profile from handle.
 * @return  
 *		   - JC_RETCODE               Specifies that stopped the imap request successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_INVALID_STOP_REQUEST Specifies that invalid stop request incase of req type is equals to POP3_NONE.
 *         - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 */
JC_RETCODE jdi_IMAP4SetStopRequest(JC_HANDLE vImap4Handle, JC_UINT32 uiProfileID) ;

/**
 * @brief This function will set the chunk size in Protocol handle.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] uiChunkSize Specifies the chunk size, if the buffer length is greater than this size then we will chunk that data and send.
 * @return  
 *		   - JC_RETCODE               Specifies that chunking size is setting in ptotocol handle successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE	jdi_IMAP4SetChunkSize(JC_HANDLE vImap4Handle, JC_UINT32 uiChunkSize) ;

/**
 * @brief This function will purge the folder on server.
 * @param[in] vImap4Handle    The Initialized IMAP Handle.
 * @param[in] pstImapPurgeReq  Specifies the purge folder request details.
 * @return  
 *		   - JC_RETCODE               Specifies purged the folder successfully.
 *         - JC_ERR_IMAP4_BUSY        Specifies Imap is busy with some other task.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies profile not exists.	
 */
JC_RETCODE jdi_IMAP4PurgeFolder(JC_HANDLE vImap4Handle, ST_IMAP4_PURGE_REQ *pstImapPurgeReq) ;

/**
 * @brief This function receive all mails those supports filter criteria.
 * @param[in] vImap4Handle    The Initialized IMAP Handle.
 * @param[in] pstImapFilterMail Specifes the filter mail request details to receive the supported mail.
 * @return  
 *		   - JC_RETCODE               Specifies received filter mails successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *         - JC_ERR_IMAP4_BUSY        Specifies imap is busy.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies that no imap4 profile is exists for this profile id 		
 */
JC_RETCODE jdi_IMAP4RecvFilterMail(JC_HANDLE vImap4Handle, ST_IMAP4_FILTERMAIL_REQ *pstImapFilterMail) ;

/**
 * @brief This function will subscribe the unsubscribe folder.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] pstIMAP4SubscribeFolderReq  Specifies the subscribe folder request details.
 * @return  
 *		   - JC_RETCODE               Specifies subscribed folder successfully.
 *         - JC_ERR_IMAP4_BUSY        Specifies Imap is busy with some other task.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies profile not exists.	
 */
JC_RETCODE jdi_IMAP4SubscribeFolder(JC_HANDLE vImap4Handle, ST_IMAP4_SUBSCRIBE_FOLDER_REQ *pstIMAP4SubscribeFolderReq) ;

/**
 * @brief This function will unsubscribe the subscribe folder.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] pstIMAP4UnSubscribeFolderReq  Specifies unsubscribe folder request details.
 * @return  
 *		   - JC_RETCODE               Specifies unsubscribed folder successfully.
 *         - JC_ERR_IMAP4_BUSY        Specifies Imap is busy with some other task.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies profile not exists.	
 */
JC_RETCODE jdi_IMAP4UnSubscribeFolder(JC_HANDLE vImap4Handle, ST_IMAP4_UNSUBSCRIBE_FOLDER_REQ *pstIMAP4UnSubscribeFolderReq);

/**
 * @brief This function will do receiving of all subscribe folder list from server based on profile id.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] uiProfileID Specifes the profile id to receive only subscribe folders.
 * @return  
 *		   - JC_RETCODE               Specifies received subscribed folder list successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies that no imap4 profile is exists for this profile id 		
 *         - JC_ERR_IMAP4_BUSY        Specifies imap is busy with some other task.
 */
JC_RETCODE jdi_IMAP4ReceiveSubscribedFolderList(JC_HANDLE vImap4Handle, JC_UINT32 uiProfileID) ;

/**
 * @brief This function will create the folder on server based on profile id.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] pstCreateFolder  Specifies the create folder request details.
 * @return  
 *		   - JC_RETCODE               Specifies created folder successfully.
 *         - JC_ERR_IMAP4_BUSY        Specifies Imap is busy with some other task.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies profile not exists.	
 */
JC_RETCODE jdi_IMAP4CreateFolder(JC_HANDLE vImap4Handle, ST_IMAP4_CREATE_FOLDER_REQ *pstCreateFolder ) ;

/**
 * @brief This function will delete the folder on server based on profile id.
 * @param[in] vImap4Handle    The Initialized IMAP Handle.
 * @param[in] pstDeleteFolder  Specifies the delete folder request details.
 * @return  
 *		   - JC_RETCODE               Specifies deleted folder successfully.
 *         - JC_ERR_IMAP4_BUSY        Specifies Imap is busy with some other task.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies profile not exists.	
 */
JC_RETCODE jdi_IMAP4DeleteFolder(JC_HANDLE vImap4Handle, ST_IMAP4_DELETE_FOLDER_REQ *pstDeleteFolder ) ;

/**
 * @brief This function will renames the folder on server based on profile id.
 * @param[in] vImap4Handle    The Initialized IMAP Handle.
 * @param[in] pstRenameFolder  Specifies the renaming folder request details.
 * @return  
 *		   - JC_RETCODE               Specifies renamed folder successfully.
 *         - JC_ERR_IMAP4_BUSY        Specifies Imap is busy with some other task.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies profile not exists.	
 */
JC_RETCODE jdi_IMAP4RenameFolder(JC_HANDLE vImap4Handle, ST_IMAP4_RENAME_FOLDER_REQ *pstRenameFolder ) ;

/**
 * @brief This function will copy or move the mails from one folder to another.
 * @param[in] vImap4Handle    The Initialized IMAP Handle.
 * @param[in] pstIMAP4CopyMoveMailReq Specifes the copy move mail request details.
 * @return  
 *		   - JC_RETCODE               Specifies copy move(d) mail successfully.
 *         - JC_ERR_IMAP4_BUSY        Specifies Imap is busy with some other task.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies profile not exists.	
 */
JC_RETCODE jdi_IMAP4CopyMoveMail(JC_HANDLE vImap4Handle, ST_IMAP4_COPY_MOVE_MAIL_REQ *pstIMAP4CopyMoveMailReq) ;

/**
 * @brief This function will save the mail on given folder.
 * @param[in] vImap4Handle    The Initialized IMAP Handle.
 * @param[in] pstIMAP4SaveMailReq Specifes the save mail request details.
 * @return  
 *		   - JC_RETCODE               Specifies saved mail successfully.
 *         - JC_ERR_IMAP4_BUSY        Specifies Imap is busy with some other task.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies profile not exists.	
 */
JC_RETCODE jdi_IMAP4SaveMail(JC_HANDLE vImap4Handle, ST_IMAP4_SAVE_MAIL_REQ *pstIMAP4SaveMailReq) ;

/**
 * @brief This function will check whether the current session is valid or not.
 * @param[in] vImap4Handle  The Initialized IMAP Handle.
 * @param[in] iSocketID   Specifies the socket id to get the socket data.
 * @return  
 *		   - E_TRUE     get the data successfully from socket handle 
 *         - E_FALSE    if socket data is NULL for corresponding socket id.
 */
JC_BOOLEAN jdi_IMAP4IsHandleForSocket(JC_HANDLE vImap4Handle, JC_INT32 iSocketID);

/**
 * @brief This function Sets the Socket Status of Socket connection.
 * @param[in] vImap4Handle  The Initialized IMAP Handle.
 * @param[in] iSocketID   Specfies that socket id to get the socket data infomation from handle .
 * @param[in] eSocketStatus Specifies that socket status to be set in socket data.
 * @return  
 *		   - JC_RETCODE               Specifies that set connection state successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 */
JC_RETCODE jdi_IMAP4SetConnectionState (JC_HANDLE vImap4Handle, JC_INT32 iSocketID, EIMAP4_SOCKET_STATUS eSocketStatus) ;

/**
 * @brief This function reads the data from the socket and fills it in the socket queue.
 * @param[in] vImap4Handle  The Initialized IMAP Handle.
 * @param[in] iSocketID   Specfies that socket id to get the socket data infomation from handle .
 * @param[in] pucReadBuffer Specifies the buffer.
 * @param[in] uiReadBufferLen Specifies the length of the buffer.
 * @return  
 *		   - JC_RETCODE               Specifies that read socket data successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies that memory allocation failure.
 */
JC_RETCODE jdi_IMAP4ReadSocketData (JC_HANDLE vImap4Handle,JC_INT32 iSocketID, JC_INT8 *pucReadBuffer, JC_UINT32 uiReadBufferLen) ;

/**
 * @brief This function Connects establishes a socket connection to the required Host.
 * @param[in] vImap4Handle   The Initialized IMAP Handle.
 * @param[in] pucHostName     Specifies the host name to connect.
 * @param[in] pucAddress      Specifies the ip address of that host.
 * @return  
 *		   - JC_RETCODE               Specifies connection established successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_MEMORY_ALLOCATION	Specifies that memory allocation failure.
 *         - JC_ERR_NET_DNS_RESOLVE    Specifies  that connection with host is fail.
 */
JC_RETCODE jdi_IMAP4Connect(JC_HANDLE vImap4Handle, const JC_UINT8	*pucHostName, JC_UINT8	*pucAddress );

/**
 * @brief This function receives only body structure of the mail.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] pstMsgBodyStructureReq Specifes the message body structure request details.
 * @return  
 *		   - JC_RETCODE               Specifies received message body sructure successfully.
 *         - JC_ERR_IMAP4_BUSY        Specifies Imap is busy with some other task.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies profile not exists.	
 */
JC_RETCODE jdi_IMAP4ReceiveBodyStructureOfMail(JC_HANDLE vImap4Handle, ST_IMAP4_MAIL_BODYSTRUCTURE_REQ *pstMsgBodyStructureReq) ;

/**
 * @brief This function receives only specific part of mail from orginal message.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] pstMailBufferReq Specifes the mail buffer structure request details.
 * @return  
 *		   - JC_RETCODE               Specifies received specific part of mail successfully.
 *         - JC_ERR_IMAP4_BUSY        Specifies Imap is busy with some other task.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies profile not exists.	
 */
JC_RETCODE jdi_IMAP4ReceiveSpecificPartOfMail(JC_HANDLE vImap4Handle, ST_IMAP4_SPECIFIC_MAIL_BUFFER_REQ *pstMailBufferReq) ;

/**
 * @brief This function gets the remaining chunk of mail data to be sent.
 * @param[in] vImap4Handle     The Initialized IMAP Handle.
 * @param[in] uiProfileID  Specifies the profile id to get the profile.
 * @param[in] pSendMoreData Specifies the send more data details.
 * @return  
 *		   - JC_RETCODE               Specifies sent more data successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *		   - JC_ERR_MEMORY_ALLOCATION Specifies memory allocation failure.	
 *		   - JC_ERR_IMAP4_PROFILE_NOT_EXISTS Specifies if the profile is not exists in the handle for this profile id.
 *		   - JC_IMAP4_RECV_CONTINUE         Specifies imap4 is receving data from server is in progress.	
 */
JC_RETCODE jdi_IMAP4SendMoreUserData (JC_HANDLE vImap4Handle, JC_UINT32 uiProfileID, ST_IMAP4_SEND_MORE_DATA *pSendMoreData) ;

/**
 * @brief This function reads Sets IDLE State to the IMAP4 Client-Server Session.
 * @param[in] vImap4Handle  The Initialized IMAP Handle.
 * @param[in] pstImap4IdleReq Specifies the Input details for setting IDLE Command
 * @return  
 *		   - JC_RETCODE               Specifies that read socket data successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies that memory allocation failure.
 */
JC_RETCODE jdi_IMAP4SetIDLEState(JC_HANDLE vImap4Handle, ST_IMAP4_IDLE_REQ *pstImap4IdleReq) ; 

/**
 * @brief This function reads keeps the  IMAP4 Client-Server Session ALIVE by sending NOOP Command
 * @param[in] vImap4Handle  The Initialized IMAP Handle.
 * @param[in] uiProfileID Specifies the Profile ID of the session to be kept alive
 * @return  
 *		   - JC_RETCODE               Specifies that read socket data successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies that memory allocation failure.
 */
JC_RETCODE jdi_IMAP4KeepAlive(JC_HANDLE vImap4Handle, JC_UINT32 uiProfileID) ;


/**
 * @brief This function will authenticate profile    
 * @param[in] vImap4Handle  The Initialized IMAP Handle
 * @param[in] uiProfileID Specifies the Profile ID of the session to be kept alive
 * @return  
 *	   - JC_RETCODE               Specifies that read socket data successfully.
 *         - JC_ERR_INVALID_PARAMETER Specifies that the parameter passed was invalid.
 *         - JC_ERR_MEMORY_ALLOCATION Specifies that memory allocation failure.
 */
JC_RETCODE	jdi_IMAP4AuthenticateProfile( JC_HANDLE	vIMAP4Handle, JC_UINT32 uiProfileID ) ;

#ifdef __cplusplus
}
#endif

#endif

#endif
