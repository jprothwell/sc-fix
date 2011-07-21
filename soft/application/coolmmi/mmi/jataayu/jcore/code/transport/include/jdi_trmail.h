/***************************************************************************
 * $Id: jdi_trmail.h,v 1.23 2009/05/29 12:00:18 browserdevjtbase Exp $
 * $Revision: 1.23 $
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
 * $Log: jdi_trmail.h,v $
 * Revision 1.23  2009/05/29 12:00:18  browserdevjtbase
 * jMail Core Update
 *
 * Revision 1.2  2009/05/15 12:20:00  basakccums
 * Chm document updated with new logo, header & footer.
 *
 * Revision 1.1  2009/05/14 07:22:49  basakccums
 * Messaging 5.0 from jEmulator to UMS
 *
 * Revision 1.24  2009/03/05 06:45:40  manikumardevjemulator
 * *** empty log message ***
 *
 * Revision 1.23  2008/08/29 10:53:57  manikumardevjemulator
 * Indication handling and UIDL file handling are moved to the email communicator
 *
 * Revision 1.22  2008/07/16 13:34:02  manikumardevjemulator
 * Build 42 updates
 *
 * Revision 1.21  2008/06/16 13:24:10  manikumardevjemulator
 * Build 39 updates
 *
 * Revision 1.20  2008/04/09 11:49:53  manikumardevjemulator
 * *** empty log message ***
 *
 * Revision 1.19  2008/04/08 13:46:35  manikumardevjemulator
 * Code Review Updates
 *
 * Revision 1.18  2008/03/28 15:45:01  manikumardevjemulator
 * Build 35 updates
 *
 * Revision 1.17  2008/03/14 15:09:28  manikumardevjemulator
 * Build 34 updates
 *
 * Revision 1.16  2008/01/18 15:48:37  manikumardevjemulator
 * *** empty log message ***
 *
 * Revision 1.15  2007/11/14 08:52:37  browserdevjtbase
 * core update.
 *
 * Revision 1.14  2007/10/15 07:16:46  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.13  2007/09/17 06:56:07  browserdevjtbase
 * Update.
 *
 * Revision 1.11  2007/09/11 12:42:13  manikumardevjemulator
 * Updates of UIDL Handling in Core
 *
 * Revision 1.10  2007/08/13 15:17:47  manikumardevjemulator
 * jMail Build 16 QC Release Updates
 *
 * Revision 1.11  2007/08/01 11:54:20  kumardevjtbase
 * Email Updates
 *
 * Revision 1.10  2007/07/03 09:52:04  browserdevjtbase
 * Mail Update.
 *
 * Revision 1.9  2007/04/27 07:21:30  browserdevjtbase
 * Mail Update
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
 * Revision 1.4  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.3  2006/07/03 07:26:11  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.2  2006/06/20 13:28:37  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.1  2006/06/16 09:31:29  browserdevjtbase
 * For e-mail
 *
 *
 ***************************************************************************/

/**
 * @file jdi_trmail.h
 * @ingroup Transport
 * @brief Defines the macros and structures of transport. 
 */
#ifndef _JDI_TRMAIL_H
#define _JDI_TRMAIL_H

#ifdef EMAIL_PROTOCOL_ENABLE

#ifdef SMTP_STACK_ENABLE
#include <jdi_smtp.h>
#endif
#ifdef POP3_STACK_ENABLE
#include <jdi_pop3.h>
#endif
#ifdef IMAP4_STACK_ENABLE
#include <jdi_imap4.h>
#endif
#include <jdi_transport.h>
#include <jdi_trprofile.h>


/**
 * @brief Specifies the connection time out.
 */
#define TR_EMAIL_CONNECT_TIMEOUT 120
/**
 * @brief Specifies the recevive time out.
 */
#define TR_EMAIL_RECV_TIMEOUT 120
/**
 * @brief Specifies the sending time out.
 */
#define TR_EMAIL_SEND_TIMEOUT 120

#if (defined POP3_WITH_SSL || defined IMAP4_WITH_SSL || defined SMTP_WITH_SSL)
/**
 * @brief Enumeration values specifying the transport mail SSL types.
 */
typedef enum _tr_email_ssl_type
{
	E_SSL_NONE = 0 ,		/**< Specifies No Secure Connection Mode */
	E_TLS_OR_NONE = 1 ,		/**< Specifies TLS or If TLS is not Supported by Server No Secure Mode */
	E_TLS = 2,				/**< Specifies TLS Mode of Security */
	E_SSL = 3				/**< Specifies SSL Mode of Connection */

} E_TR_EMAIL_SSLType ;
#endif
/**
 * @brief Defines structure to hold information about transport add profile request details. 
 */
typedef struct tr_add_profile_req
{
	JC_TASK_INFO			stClientTaskInfo ;	/**< Specifies client task information*/
	ST_GENERIC_DATA_CONN	stdataConnection ;	/**< Specifies the generic data connection structure*/
	JC_BOOLEAN				bIsDataConnSent ;	/**< if true then data connection sent, otherwise not sent*/
	JC_UINT32				uiSessionID ;		/**< Specifies session id*/
	JC_UINT32				uiProfileID;		/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;		/**< Specifies the Connection Mode */
	JC_UINT16				uhPortNumber ;      /**< Specifies the Port Number on which the Mail Server is Listening*/
	JC_BOOLEAN				bEmailOption ;		/**< if true then dont delete mails from server (leave a copy on server) otherwise delete*/
	JC_BOOLEAN				bIsAuthReqd ;		/**< if true then provide authentication details like user name and password otherwise not required*/
#if (defined IMAP4_WITH_SSL || defined POP3_WITH_SSL || defined SMTP_WITH_SSL)
	E_TR_EMAIL_SSLType				eMailSSLType ;/**< Specifies type of SSL*/
#endif
	JC_INT8					pszBuffer[1];		/**< Specifies add profile buffer */
} ST_TR_ADD_PROFILE_REQ ;

/**
 * @brief Defines structure to hold information about transport update profile request details. 
 */
typedef struct tr_update_profile_req
{
	JC_TASK_INFO			stClientTaskInfo ;	/**< Specifies client task information*/
	ST_GENERIC_DATA_CONN	stdataConnection ;	/**< Specifies the generic data connection structure*/
	JC_BOOLEAN				bIsDataConnSent ;	/**< if true then data connection sent, otherwise not sent*/
	JC_UINT32				uiSessionID ;		/**< Specifies session id*/
	JC_UINT32				uiProfileID;		/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;		/**< Specifies the Connection Mode */
	JC_UINT16				uhPortNumber ;      /**< Specifies the Port Number on which the Mail Server is Listening*/
	JC_BOOLEAN				bEmailOption ;		/**< if true then dont delete mails from server (leave a copy on server) otherwise delete*/
	JC_BOOLEAN				bIsAuthReqd ;		/**< if true then provide authentication details like user name and password otherwise not required*/
#if (defined IMAP4_WITH_SSL || defined POP3_WITH_SSL || defined SMTP_WITH_SSL)
	JC_UINT8				ucMailSSLType ;		/**< Specifies type of SSL*/
#endif
	JC_INT8					pszBuffer[1];		/**< Specifies update profile buffer */
} ST_TR_UPDATE_PROFILE_REQ ;

/**
 * @brief Defines structure to hold information about transport delete profile request details. 
 */
typedef struct tr_delete_profile_req
{
	JC_TASK_INFO			stClientTaskInfo ;	/**< Specifies client task information*/
	JC_UINT32				uiSessionID ;		/**< Specifies session id*/
	JC_UINT32				uiProfileID;		/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;		/**< Specifies the Connection Mode */
}ST_TR_DELETE_PROFILE_REQ ;

/**
 * @brief Defines structure to hold information about transport authorise profile request details. 
 */
typedef struct tr_auth_req
{
	JC_TASK_INFO			stClientTaskInfo ;	/**< Specifies client task information*/
	JC_UINT32				uiSessionID ;		/**< Specifies session id*/
	JC_UINT32				uiProfileID;		/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;		/**< Specifies the Connection Mode */
}ST_TR_AUTH_PROFILE_REQ ;


/**
 * @brief Defines structure to hold information about transport email profile response details. 
 */
typedef struct tr_email_profile_resp
{
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileId;			/**< Specifies profile id*/
	JC_BOOLEAN				bIsAuthorisedUser ;     /**< Specifies whether user is valid, in case invalid then profile details will not be stored */
	JC_RETCODE				retCode ;				/**< Specifies retcode*/
} ST_TR_EMAIL_PROFILE_RESP ;

/**
 * @brief Defines structure to hold information about transport requesting data details. 
 */
typedef struct tr_data_request
{
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_UINT32				uiTotalBuffSize;		/**< Specifies total buffer size*/
	JC_UINT32				uiRcvdBufferLen;		/**< Specifies received buffer size in current transaction*/
} ST_TR_DATA_REQUEST;

/**
 * @brief Defines structure to hold information about transport sending mail request details. 
 */
typedef struct tr_send_mail_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the trasport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_BOOLEAN				bIsDeliveryRptRequested ;/**< Specifies whether delivery report is requested or not */
	JC_BOOLEAN				bDeliveryReturnHdrs;	/**< Specifies whether in delivery report full mail buffer should be returned or only headers */
	JC_UINT32				uiDelRptCriteria ;		/**< Specifies the criteria for which delivery report is required */
	JC_UINT32				uiSenderIDLen;			/**< Specifies the senderid length*/
	JC_UINT32				uiRecipientLen;			/**< Specifies the receipent length*/
	JC_UINT32				uiMailBufferLen;		/**< Specifies the full mail buffer length*/
	JC_UINT32				uiTotalChunkLen;		/**< Specifies the total chunk length of mail*/
	JC_UINT32				uiTotalMailSize;		/**< Specifies the total mail size*/
	JC_BOOLEAN				bHasMoreData;			/**< Specifies whether the buffer is complete or not.*/
	JC_INT8					szMailBuffer[1];		/**< Specifies send mail buffer */
} ST_TR_SEND_MAIL_REQ;

/**
 * @brief Defines structure to hold information about transport email user requesting data details. 
 */
typedef struct tr_email_user_data_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiOffset;				/**< Specifies the offset length after which the user data needs to be appended to the already received data */
	JC_UINT32				uiUserDataLength ;		/**< Specifies Length of the Userdata  buffer requested */
	JC_UINT32				uiUserDataInfoLength;	/**< Specifies Length of the Userdata  info buffer*/
	JC_INT8					pUserDataInfo[1];		/**< Specifies email user data Buffer */
} ST_TR_EMAIL_USER_DATA_REQ ;
/**
 * @brief Defines structure to hold information about transport mail stop request details. 
 */
typedef struct _tr_mail_stop_req
{
	JC_UINT32				uiSessionID ;		/**< Specifies session id*/
	JC_UINT32				uiProfileID ;		/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;		/**< Specifies the Connection Mode */
} ST_TR_MAIL_STOP_REQ ;
/**
 * @brief Defines structure to hold information about transport receive mail request details. 
 */
typedef struct _tr_recv_mail_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_UINT32				uiMsgIndex ;			/**< Specifies the message index*/
	JC_UINT32				uiMailSize ;			/**< Specifies total mail size*/
	JC_BOOLEAN				bLeaveCopyOnServer;		/**< if it is true then we will not delete the mail from server while receving,if false we will delete mails on server*/
	JC_BOOLEAN              bWithAttachment;		/**<  if true then mail is having attachment otherwise receive only body*/
	JC_INT8					ucUserData [1] ;		/**< Specifies mail Buffer */
}ST_TR_RECV_MAIL_REQ ;
/**
 * @brief Defines structure to hold information about transport receive specific part of mail request details. 
 */
typedef struct _tr_recv_specific_part_mail_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_UINT32				uiMailSize ;			/**< Specifies total mail size*/
	JC_INT8				    ucUserData [1] ;		/**< Specifies specific part of mail data Buffer */
}ST_TR_RECV_SPECIFIC_PART_MAIL_REQ ;
/**
 * @brief Defines structure to hold information about transport receive folder list request details. 
 */
typedef struct _tr_recv_folder_list_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
}ST_TR_RECV_FOLDER_LIST_REQ ;
/**
 * @brief Defines structure to hold information about transport receive message list request details. 
 */
typedef struct _tr_recv_msglist_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_BOOLEAN				bIsAllMessages ;		/**< if it is true then retreive all message otherwise retrieve only recent messages*/
#ifdef EMAIL_CORE_WITH_INDICATION_HANDLING
	JC_BOOLEAN				bIsExpunged ;			/**< Specifies true if client is sending request for recv msg list after folder expunged successfully */
#endif
	JC_INT8                 ucUserData[1] ;			/**< Specifies message list Buffer */
}ST_TR_RECV_MSGLIST_REQ ;
/**
 * @brief Defines structure to hold information about transport receive message header request details. 
 */
typedef struct _tr_recv_msg_header_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_INT32				uiMsgIndex ;			/**< Specifies the message index*/
	JC_INT8                 ucUserData[1] ;			/**< Specifies message header Buffer */
}ST_TR_RECV_MSG_HEADER_REQ ;
/**
 * @brief Defines structure to hold information about transport receive message body structure request details. 
 */
typedef struct _tr_recv_msg_body_structure_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/

	JC_INT8                 ucUserData[1] ;			/**< Specifies message body structure Buffer */
}ST_TR_RECV_MSG_BODY_STRUCTURE_REQ ;

/**
 * @brief Defines structure to hold information about transport set mail flag request details. 
 */
typedef struct _tr_set_mail_flag_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_UINT16				uhflags2Set ;				/**< Specifies flag to set on the server message*/
	JC_INT8                 ucUserData[1] ;			/**< Specifies set mail flag Buffer */
}ST_TR_SET_MAIL_FLAG_REQ ;

/**
 * @brief Defines structure to hold information about transport purge folder request details. 
 */
typedef struct _tr_purge_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_INT8                 ucUserData[1] ;			/**< Specifies purge folder Buffer */
} ST_TR_PURGE_REQ ;
/**
 * @brief Defines structure to hold information about transport imap idle request details. 
 */
typedef struct _tr_imap_idle_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_INT8                 ucUserData[1] ;			/**< Specifies imap folder buffer */
} ST_TR_IMAP_IDLE_REQ ;
/**
 * @brief Enumeration values specifying the transport mail flag types.
 */
typedef enum _tr_mail_flag_type
{
	E_IMAP_TR_NONE			= 0x00 ,		/**< Specifies flag type is none*/
	E_IMAP_TR_ANSWERED		= 0x01 ,		/**< Specifies flag type is answered*/
	E_IMAP_TR_UNANSWERED	= 0x02 ,		/**< Specifies flag type is unanswered*/
	E_IMAP_TR_FLAGGED		= 0x04 ,		/**< Specifies flag type is flagged*/
	E_IMAP_TR_UNFLAGGED		= 0x08 ,		/**< Specifies flag type is unflagged*/
	E_IMAP_TR_DELETED		= 0x10 ,		/**< Specifies flag type is deleted*/
	E_IMAP_TR_UNDELETED     = 0x20 ,		/**< Specifies flag type is undeleted*/
	E_IMAP_TR_SEEN			= 0x40 ,		/**< Specifies flag type is seen*/
	E_IMAP_TR_UNSEEN		= 0x80 ,		/**< Specifies flag type is unseen*/
	E_IMAP_TR_DRAFT			= 0x100 ,		/**< Specifies flag type is draft*/
	E_IMAP_TR_UNDRAFT		= 0x200			/**< Specifies flag type is undraft*/
} EMailTRFlagType ;
/**
 * @brief Defines structure to hold information about transport filter mail request details. 
 */
typedef struct _tr_filtermail_req
{
	JC_TASK_INFO				stClientTaskInfo ;	/**< Specifies client task information*/
	JC_UINT32					uiSessionID ;		/**< Specifies session id*/
	JC_UINT32					uiProfileID ;		/**< Specifies profile id*/
	E_TR_CONNECTION_MODE		eTrConnMode ;		/**< Specifies the transport Connection Mode */
	JC_UINT32					uiMinSize ;			/**< Specifies the minimum Size of the Mail */
	JC_UINT32					uiMaxSize ;			/**< Specifies the maximum Size of the Mail */
	JC_UINT16                   eEmailFlag ;		/**< Specifies the IMAP Flags that should be Set for the Filtered Mails */
	JC_UINT8                    uiDateOption;		/**< Specifies the Date Option One of the Values of EMAIL_COMM_IMAP_DATE_FILTER_CRITERIA type*/
	JC_INT8						ucUserData[1] ;		/**< Specifies the filter mail buffer*/
}ST_TR_FILTER_MAIL_REQ ;
/**
 * @brief Defines structure to hold information about transport delete mail request details. 
 */
typedef struct _tr_email_delete_mail_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_UINT32				uiUIDLBuffLen ;			/**< Specifies total buffer length */
	JC_UINT32				uiCurrBuffLen ;			/**< Specifies current buffer length */
	JC_BOOLEAN              bIsMsgindex ;			/**< Specifies whether Message index is passed by application */
	JC_BOOLEAN				bHasMoreData;			/**< Specifies whether the buffer is complete or not.*/
	JC_INT8					ucBuffer[1] ;			/**< Specifies delete mail buffer*/
}ST_TR_DELE_MAIL_REQ ;
/**
 * @brief Defines structure to hold information about transport create folder request details. 
 */
typedef struct _tr_email_create_folder_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_INT8				    ucUserData[1] ;		/**< Specifies create folder buffer*/
}ST_TR_CREATE_FOLDER_REQ ;
/**
 * @brief Defines structure to hold information about transport delete folder request details. 
 */
typedef struct _tr_email_delete_folder_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_INT8				    ucUserData[1] ;		/**< Specifies delete folder buffer*/
}ST_TR_DELETE_FOLDER_REQ ;	
/**
 * @brief Defines structure to hold information about transport rename folder request details. 
 */
typedef struct _tr_email_rename_folder_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_INT8				    ucUserData[1] ;			/**< Specifies rename folder buffer*/

}ST_TR_RENAME_FOLDER_REQ ;
/**
 * @brief Defines structure to hold information about transport copy move mail request details. 
 */
typedef struct _tr_email_copy_move_mail_req
{
	JC_TASK_INFO            stClientTaskInfo  ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE    eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32               uiSessionID ;			/**< Specifies session id*/
    JC_UINT32               uiprofileID ;			/**< Specifies profile id*/
	JC_BOOLEAN              bIsCopyOrMove ;			/**< Specifies whether the mail has to moved or copied if the value is set to E_TRUE => it species that the mail has to be MOVED */
	JC_INT8                 ucUserData [1] ;		/**< Specifies copy move mail buffer*/
}ST_TR_COPY_MOVE_MAIL_REQ ;
/**
 * @brief Defines structure to hold information about transport save mail request details. 
 */
typedef struct _tr_email_save_mail_req
{
	JC_TASK_INFO            stClientTaskInfo  ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE    eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32               uiSessionID ;			/**< Specifies session id*/
    JC_UINT32               uiprofileID ;			/**< Specifies profile id*/
	JC_UINT16               Flags2Set ;				/**< Specifies the flags to be set to the mail*/
	JC_UINT32               uiMsgSize ;				/**< Specifies message size*/
	JC_UINT32				uiTotalChunkLen;		/**< Specifies the size of the Current Chunk of Mail Buffer */
	JC_UINT32				uiTotalMailSize;		/**< Specifies the size of the mail*/
	JC_BOOLEAN				bHasMoreData;			/**< Specifies the whether complete Mail buffer has been sent*/
	JC_INT8                 ucUserData [1] ;		/**< Specifies save mail buffer*/
}ST_TR_SAVE_MAIL_REQ ;
/**
 * @brief Defines structure to hold information about transport subscribe folder request details. 
 */
typedef struct _tr_subscribe_folder_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_INT8					ucUserData [1] ;		/**< Specifies subscribe folder buffer*/
}ST_TR_SUBSCRIBE_FOLDER_REQ ;
/**
 * @brief Defines structure to hold information about transport unsubscribe folder request details. 
 */

typedef struct _tr_unsubscribe_folder_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_INT8					ucUserData [1] ;		/**< Specifies unsubscribe folder buffer*/
}ST_TR_UNSUBSCRIBE_FOLDER_REQ ;

/**
 * @brief Defines structure to hold information about IMAP4 IDLE request details. 
 */
typedef struct _tr_imap4_idle_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_INT8					*pszFolderName ;		/**<Specifies the Selected FolderName */
}ST_TR_IMAP4_IDLE_REQ ;

/**
 * @brief Defines structure to hold information about KEEP ALIVE request details. 
 */
typedef struct _tr_keep_alive_req
{
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/	
}ST_TR_KEEPALIVE_REQ ;

/**
 * @brief Defines structure to hold information about transport send mail response details. 
 */
typedef struct tr_send_mail_resp
{
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_RETCODE				eRetCode ;				/**< Specifies retcode that contains error code*/
} ST_TR_SEND_MAIL_RESP;
/**
 * @brief Defines structure to hold information about transport email user data response details. 
 */
typedef struct tr_email_user_data_resp
{
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_TASK_INFO			stClientTaskInfo ;		/**< Specifies client task information*/
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiUserDataInfoLength;	/**< Specifies user datainfo length */
	JC_UINT32				uiOffSet;				/**< Specifies the offset length after which the user data needs to be appended to the already received data */
	JC_UINT32				uiUserDataLength ;		/**< Specifies Length of the Userdata  buffer requested */
	JC_UINT32				uiTotalLength ;			/**< Specifies total length*/
	JC_UINT8				ucUserData [1] ;		/**< Specifies email user data response buffer*/
} ST_TR_EMAIL_USER_DATA_RESP ;
/**
 * @brief Defines structure to hold information about transport email event response details. 
 */
typedef struct _tr_email_event_resp
{
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID;			/**< Specifies profile id*/
	JC_RETCODE				eRetCode;				/**< Specifies retcode that contains error code*/
} ST_TR_EMAIL_EVENT_RESP ;
/**
 * @brief Defines structure to hold information about transport send mail indication details. 
 */
typedef struct tr_send_mail_ind
{
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	JC_RETCODE				retCode ;				/**< Specifies retcode that contains error code*/
#ifdef SMTP_STACK_ENABLE
	SMTP_STATUS				eStatus ;				/**< Specifies status based on SMTP_STATUS*/
#endif
	JC_UINT32				uiTotalLength ;			/**< Specifies total length*/
	JC_UINT8				ucBuffer [1] ;			/**< Specifies send mail indication buffer*/
} ST_TR_SEND_MAIL_IND ;
/**
 * @brief Defines structure to hold information about transport receive mail indication details. 
 */
typedef struct _tr_recv_mail_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_UINT8				eStatus ;				/**< Specifies the status generalized by core*/
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				isMsgBufferRecvd;		/**< if true message buffer received otherwise not received*/
	JC_BOOLEAN              bIsLastChunk;			/**< if true then this is chunk of data user if going to be received*/
	JC_BOOLEAN				bIsUIDLRxd ;			/**< if it is true then process uidl information*/
	JC_BOOLEAN				isUIDListRxd ;			/**< if it is true then update the uidl list*/
	JC_UINT32				uiTotalBufferLen;		/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;		/**< Specifies current buffer length*/
	JC_INT8					ucBuffer[1] ;			/**< Specifies recv mail indication buffer*/
}ST_TR_RECV_MAIL_IND ;
/**
 * @brief Defines structure to hold information about transport receive specific part of mail indication details. 
 */
typedef struct _tr_recv_specific_part_mail_ind
{
	JC_UINT32				uiSessionID ;		/**< Specifies session id*/
	JC_UINT32				uiProfileID ;		/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;		/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;/**< Specifies transport task information*/
	JC_UINT8				eStatus ;			/**< Specifies the status generalized by core*/
	JC_RETCODE				eRet ;				/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				isMsgBufferRecvd;	/**< if true message buffer received otherwise not received*/
	JC_UINT32				uiTotalBufferLen;	/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;	/**< Specifies current buffer length*/
	JC_INT8					ucBuffer[1] ;		/**< Specifies recv specific part of mail indication buffer*/

}ST_TR_RECV_SPECIFIC_PART_MAIL_IND ;

/**
 * @brief Defines structure to hold information about both transport receive folder list and subscribe folder list indication details. 
 */
typedef struct _tr_recv_folder_list_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_UINT8				eStatus ;				/**< Specifies the status generalized by core*/
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				isFolderListRecvd;		/**< if true folder list buffer received otherwise not received*/
	JC_BOOLEAN              bIsLastChunk;			/**< if true then this is chunk of data user if going to be received*/
	JC_UINT32				uiTotalBufferLen;		/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;		/**< Specifies current buffer length*/
	JC_INT8					ucBuffer[1] ;			/**< Specifies recv folder list indication buffer*/
}ST_TR_RECV_FOLDER_LIST_IND ;
/**
 * @brief Defines structure to hold information about transport receive message list indication details. 
 */
typedef struct _tr_recv_msglist_ind
{
	JC_UINT32		uiSessionID ;			/**< Specifies session id*/
	JC_UINT32		uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;	/**< Specifies the transport Connection Mode */
	JC_TASK_INFO	stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_UINT8		eStatus ;				/**< Specifies the status generalized by core*/
	JC_RETCODE		eRet ;					/**< Specifies retcode that contains error code*/
	JC_BOOLEAN		isMsgListRecvd;			/**< if true message list buffer received otherwise not received*/
	JC_BOOLEAN      bIsLastChunk;			/**< if true then this is chunk of data user if going to be received*/
#ifdef EMAIL_CORE_WITH_INDICATION_HANDLING
	JC_BOOLEAN				bIsExpunged ;			/**< Specifies true if client is sending request for recv msg list after folder expunged successfully */
#endif
	JC_UINT32		uiTotalBufferLen;		/**< Specifies total buffer length*/
	JC_UINT32		uiCurrBufferLen ;		/**< Specifies current buffer length*/
	JC_INT8			ucBuffer[1] ;			/**< Specifies recv message list indication buffer*/
}ST_TR_RECV_MSGLIST_IND ;
/**
 * @brief Defines structure to hold information about transport set mail flag indication details. 
 */
typedef struct _tr_set_flag_ind
{
	JC_UINT32				uiSessionID ;	/**< Specifies session id*/
	JC_UINT32				uiProfileID ;	/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;	/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;/**< Specifies transport task information*/
	JC_UINT8				eStatus ;		/**< Specifies the status generalized by core*/
	JC_RETCODE				eRet ;			/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				bIsMailFlagSet ;/**< if true mail flag set on server otherwise not*/
	JC_UINT8				flags ;			/**< Specifies which flag to set stores in this variable*/
	JC_UINT32				uiTotalBufferLen;/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;/**< Specifies current buffer length*/
	JC_INT8					ucBuffer[1] ;/**< Specifies set flag indication buffer*/
} ST_TR_SET_FLAG_IND ;

/**
 * @brief Defines structure to hold information about transport purge folder indication details. 
 */
typedef struct _tr_purge_ind
{
	JC_UINT32				uiSessionID ;		/**< Specifies session id*/
	JC_UINT32				uiProfileID ;		/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;		/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;/**< Specifies transport task information*/
	JC_UINT8				eStatus ;			/**< Specifies the status generalized by core*/
	JC_RETCODE				eRet ;				/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				bIsFolderPurged ;	/**< if true folder purged else return some error code*/
	JC_UINT32				uiTotalBufferLen;	/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;	/**< Specifies current buffer length*/
	JC_INT8					ucBuffer[1] ;		/**< Specifies purge folder indication buffer*/
} ST_TR_PURGE_IND ;
/**
 * @brief Defines structure to hold information about transport receive filter mail indication details. 
 */
typedef struct _tr_recv_filter_mail_ind
{
	JC_UINT32				uiSessionID ;		/**< Specifies session id*/
	JC_UINT32				uiProfileID ;		/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;		/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;/**< Specifies transport task information*/
	JC_UINT8	        	eStatus ;			/**< Specifies the status generalized by core*/
	JC_RETCODE				eRet ;				/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				bIsUIDListRecvd;	/**< if true then uidl list is received else return error*/
	JC_BOOLEAN              bIsLastChunk ;		/**< if true then this is chunk of data user if going to be received*/
	JC_UINT32				uiTotalBufferLen;	/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;	/**< Specifies current buffer length*/
	JC_INT8					ucBuffer[1] ;		/**< Specifies receive filter mail indication buffer*/
}ST_TR_RECV_FILTER_MAIL_IND ;
/**
 * @brief Defines structure to hold information about transport receive message header indication details. 
 */
typedef struct _tr_recv_msg_header_ind
{
	JC_UINT32				uiSessionID ;		/**< Specifies session id*/
	JC_UINT32				uiProfileID ;		/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;		/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;/**< Specifies transport task information*/
	JC_UINT8				eStatus ;			/**< Specifies the status generalized by core*/
	JC_RETCODE				eRet ;				/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				isMsgHeaderRecvd;	/**< if true then message header is received else return error*/
	JC_BOOLEAN              bIsLastChunk;		/**< if true then this is chunk of data user if going to be received*/
	JC_BOOLEAN				bIsUIDLRxd ;		/**< if it is true then process uidl information*/
	JC_BOOLEAN				isUIDListRxd ;		/**< if it is true then update the uidl list*/
	JC_UINT32				uiTotalBufferLen;	/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;	/**< Specifies current buffer length*/
	JC_INT8					ucBuffer[1] ;		/**< Specifies receive message header indication buffer*/
}ST_TR_RECV_MSG_HEADER_IND ;
/**
 * @brief Defines structure to hold information about transport receive message body structure indication details. 
 */
typedef struct _tr_recv_msg_bodystructure_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_UINT8				eStatus ;				/**< Specifies the status generalized by core*/
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				isMsgBodyStructureRecvd;/**< if true then message body structure is received else return error*/	
	JC_UINT32				uiTotalBufferLen;		/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;		/**< Specifies current buffer length*/
	JC_INT8					ucBuffer[1] ;			/**< Specifies receive message body structure indication buffer*/
}ST_TR_RECV_MSG_BODYSTRUCTURE_IND ;
/**
 * @brief Defines structure to hold information about transport subscribe folder indication details. 
 */
typedef struct _tr_subscribe_folder_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_UINT8				eStatus ;				/**< Specifies the status generalized by core*/
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				isFolderSubscribed;		/**< if true then folder subscribed*/	
	JC_UINT32				uiBufferLen ;			/**< Specifies buffer length*/
	JC_INT8					ucBuffer[1] ;			/**< Specifies subscribe folder indication buffer*/
}ST_TR_SUBSCRIBE_FOLDER_IND ;
/**
 * @brief Defines structure to hold information about transport unsubscribe folder indication details. 
 */
typedef struct _tr_unsubscribe_folder_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_UINT8				eStatus ;				/**< Specifies the status generalized by core*/
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				isFolderUnsubscribed;	/**< if true then folder unsubscribed*/	
	JC_UINT32				uiBufferLen ;			/**< Specifies buffer length*/
	JC_INT8					ucBuffer[1] ;			/**< Specifies unsubscribe folder indication buffer*/

}ST_TR_UNSUBSCRIBE_FOLDER_IND ;
/**
 * @brief Defines structure to hold information about transport delete mail indication details. 
 */
typedef struct _tr_email_delete_mail_ind
{
	JC_UINT32		uiSessionID ;			/**< Specifies session id*/
	JC_UINT32		uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;	/**< Specifies the transport Connection Mode */
	JC_TASK_INFO	stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_RETCODE		eRet ;					/**< Specifies retcode that contains error code*/
	JC_UINT8		eStatus ;				/**< Specifies the status generalized by core*/
	JC_BOOLEAN      bIsUIDLRxd ;			/**< if it is true then process uidl information*/
	JC_UINT32		uiBufferLen;			/**< Specifies buffer length*/
	JC_INT8			ucBuffer[1];			/**< Specifies delete mail indication buffer*/
}ST_TR_DELE_MAIL_IND ;
/**
 * @brief Defines structure to hold information about transport disconnect indication details. 
 */
typedef struct _tr_email_disconnect_ind
{
	JC_UINT32		uiSessionID ;			/**< Specifies session id*/
	JC_UINT32		uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;	/**< Specifies the transport Connection Mode */
	JC_TASK_INFO	stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_RETCODE		eRet ;					/**< Specifies retcode that contains error code*/
	JC_UINT8		eStatus ;				/**< Specifies the status generalized by core*/
	JC_BOOLEAN      bIsUIDLRxd ;			/**< if it is true then process uidl information*/
	JC_UINT32		uiBufferLen;			/**< Specifies buffer length*/
	JC_INT8			ucBuffer[1];			/**< Specifies disconnect indication buffer*/
}ST_TR_DISCONNECT_IND ;
/**
 * @brief Defines structure to hold information about transport mail progress indication details. 
 */
typedef struct tr_email_prog_ind
{
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	E_DATA_CONN_STATUS		eDataConnectionStatus ;	/**< Specifies the data connection status from enumaration  */
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	JC_UINT32				uiBytesRecvd ;			/**< Specifies total no of bytes received*/
	JC_UINT32				uiBytesSent ;			/**< Specifies total no of bytes sent*/
	JC_UINT32				uiTotalBytesToSend ;	/**< Specifies actual no of bytes has to send*/
	JC_UINT32				uiTotalBytesToRecv ;	/**< Specifies actual no of bytes has to receive*/
	JC_UINT32				eStatus ;				/**< Specifies the status generalized by core*/	
	JC_RETCODE				retCode ;				/**< Specifies retcode that contains error code*/
	JC_BOOLEAN              bIsUIDLRxd ;			/**< if it is true then process uidl information*/
	JC_UINT32				uiBufferLength ;		/**< Specifies buffer length*/
	JC_UINT8				ucBuffer [1] ;			/**< Specifies email progress indication buffer*/
} ST_TR_EMAIL_PROGRESS_IND ;
/**
 * @brief Defines structure to hold information about transport create folder indication details. 
 */
typedef struct _tr_email_create_folder_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_UINT8				eStatus ;				/**< Specifies the status generalized by core*/	
	JC_BOOLEAN				isFolderCreated ;		/**< if true then folder created on server*/	
	JC_UINT32				uiBufferLen;			/**< Specifies buffer length*/
	JC_INT8					ucBuffer[1];			/**< Specifies create folder indication buffer*/
}ST_TR_CREATE_FOLDER_IND ;

/**
 * @brief Defines structure to hold information about transport delete folder indication details. 
 */
typedef struct _tr_email_delete_folder_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_UINT8				eStatus ;				/**< Specifies the status generalized by core*/	
	JC_BOOLEAN				isFolderDeleted ;		/**< if true then folder deleted on server successfully*/	
	JC_UINT32				uiBufferLen;			/**< Specifies buffer length*/
	JC_INT8					ucBuffer[1];			/**< Specifies delete folder indication buffer*/
}ST_TR_DELETE_FOLDER_IND ;
/**
 * @brief Defines structure to hold information about transport rename folder indication details. 
 */
typedef struct _tr_email_rename_folder_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_UINT8				eStatus ;				/**< Specifies the status generalized by core*/	
	JC_BOOLEAN				isFolderRenamed ;		/**< if true then folder renamed on server successfully*/	
	JC_UINT32				uiBufferLen;			/**< Specifies buffer length*/
	JC_INT8					ucBuffer[1];			/**< Specifies rename folder indication buffer*/
}ST_TR_RENAME_FOLDER_IND ;
/**
 * @brief Defines structure to hold information about transport copy move mail indication details. 
 */
typedef struct _tr_copy_move_mail_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_UINT8			    eStatus ;				/**< Specifies the status generalized by core*/	
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				isMailCopied;			/**< if true then mail moved or copied successfully*/
	JC_UINT32				uiTotalBufferLen;		/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;		/**< Specifies current buffer length*/
	JC_BOOLEAN              bIsCopyOrMove ;			/**<if it is true then move the mail else copy the mail*/
	JC_INT8					ucBuffer[1] ;			/**< Specifies copy move mail indication buffer*/
}ST_TR_COPY_MOVE_MAIL_IND ;
/**
 * @brief Defines structure to hold information about transport save mail indication details. 
 */
typedef struct _tr_save_mail_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_UINT8			    eStatus ;				/**< Specifies the status generalized by core*/	
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				isMailSaved;			/**< if true then mail saved successfully*/
	JC_UINT32				uiTotalBufferLen;		/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;		/**< Specifies current buffer length*/
	JC_INT8					ucBuffer[1] ;			/**< Specifies save mail indication buffer*/

}ST_TR_SAVE_MAIL_IND ;
/**
 * @brief Defines structure to hold information about transport save mail indication details. 
 */

typedef struct _tr_imap4_idle_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**< Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_UINT8			    eStatus ;				/**< Specifies the status generalized by core*/	
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_BOOLEAN				isIdleUpdateComplete;			/**< if true then mail saved successfully*/
	JC_UINT32				uiTotalBufferLen;		/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;		/**< Specifies current buffer length*/
	JC_UINT32               uiBufferLen ;			/**< Specifies the buffer length */
	JC_INT8					ucBuffer[1] ;			/**< Specifies idle indication buffer*/
}ST_TR_IMAP4_IDLE_IND ;
/**
 *brief structure store info about keep alive indication details
 */
typedef struct _tr_email_keepalive_ind
{
	JC_UINT32				uiSessionID ;			/**< Specifies session id*/
	JC_UINT32				uiProfileID ;			/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;			/**<Specifies the transport Connection Mode */
	JC_TASK_INFO			stTransportTaskInfo ;	/**< Specifies transport task information*/
	JC_RETCODE				eRet ;					/**< Specifies retcode that contains error code*/
	JC_UINT8			    eStatus ;				/**< Specifies the status generalized by core*/	
	JC_BOOLEAN				isConnectionAlive;		/**< if true then mail saved successfully*/
	JC_UINT32				uiTotalBufferLen;		/**< Specifies total buffer length*/
	JC_UINT32				uiCurrBufferLen ;		/**< Specifies current buffer length*/
	JC_UINT32               uiBufferLen ;			/**< Specifies the buffer length */
	JC_INT8					ucBuffer[1] ;			/**< Specifies idle indication buffer*/
}ST_TR_EMAIL_KEEPALIVE_IND ;
/**
 * @brief Enumeration values specifying the transport mail callback status.
 */
typedef enum e_tr_mail_callback_status
{
	E_MAIL_TIMER_PENDING = 1,			/**< Specifies  status for timer pending*/
	E_MAIL_WAITING_FOR_DC_CONN ,		/**< Specifies  status for waiting for connection*/
	E_MAIL_WAITING_FOR_DC_DISCONNECT,	/**< Specifies  status for waiting for disconnection*/
	E_MAIL_TIMER_PROCESSING ,			/**< Specifies  status for timer processing*/
	E_MAIL_TIMER_COMPLETED,				/**< Specifies  status for timer completed*/
}E_MAIL_CALLBACK_STATUS ;

/**
 * @brief Defines structure to hold information about transport mail callback data details. 
 */
typedef struct tr_mail_cb_data
{

	JC_UINT32				uiSessionID ;		/**< Specifies session id*/
	JC_UINT32				uiProfileID;		/**< Specifies profile id*/
	E_TR_CONNECTION_MODE	eTrConnMode ;		/**< Specifies the transport Connection Mode */
	E_MAIL_CALLBACK_STATUS	eMailCbStatus ;		/**< Specifies the mail callback status*/
	struct tr_mail_cb_data			*pNext ;	/**< Specifies the next mail callback data in list*/

} ST_TR_MAIL_CB_DATA ;
/**
 * @brief This function converts normal buffer into base64 encoding buffer.
 * @param[in] pass     Specifies normal buffer.
 * @param[in] passlen  Specifies length of normal buffer.
 * @param[out] encpass    specifies output base64 encoded buffer.
 * @return  
 *		   - base64 encoded buffer lehgth.
 */
JC_INT32 convNormalToBase64 (JC_UINT8 *pass, JC_INT32 passlen, JC_UINT8 *encpass) ;
/**
 * @brief This function converts  base64 encoding buffer into normal buffer.
 * @param[in] encoded     Specifies encoded buffer.
 * @param[in] enc_len     Specifies length of encoded buffer.
 * @param[out] plain	  Specifies normal buffer.
 * @param[out] data_len   Specifies length of normal buffer.
 * @return  
 *		   - JC_OK  Specifies buffer converted successfully.
 */
JC_INT32 ConvertBase64Normal (JC_INT8 *encoded, JC_INT32 enc_len, JC_INT8 *plain, JC_INT32 *data_len) ;
/**
 * @brief This function will calculate length of encoded buffer. 
 * @param[in] in     Specifies encoded buffer.
 * @return  
 *		   - returns length of encoded buffer.
 */
JC_INT32 b64encode_len(JC_INT8 *in) ;
/**
 * @brief This function will calculate length of decoded buffer. 
 * @param[in] in     Specifies decoded buffer.
 * @return  
 *		   - returns length of decoded buffer.
 */
JC_INT32 b64decode_len(JC_INT8 *in) ;
/**
 * @brief This function will just allocate memory and send the buffer. 
 * @param[in] in     Specifies the normal buffer.
 * @return  
 *		   - returns encoded buffer.
 */
JC_INT8 *b64encode_alloc(JC_INT8 *in) ;
/**
 * @brief This function will just allocate memory and send the buffer.
 * @param[in] in     Specifies the normal buffer.
 * @return  
 *		   - returns decoded buffer.
 */
JC_INT8 *b64decode_alloc(JC_INT8 *in) ;
/**
 * @brief This function will just allocate memory and send the buffer.
 * @param[in] in	   Specifies the normal buffer.
 * @param[out] out     Specifies the digest encoded buffer.
 * @return  
 *		   - void.
 */
void b64encode(JC_UINT8 *in, JC_UINT8 *out) ;
/**
 * @brief This function will just allocate memory and send the buffer.
 * @param[in] in	   Specifies the normal buffer.
 * @param[in] inlen	   Specifies length buffer.
 * @param[out] out     Specifies digest encoded buffer.
 * @return  
 *		   - void.
 */
void b64enc(JC_UINT8 *in, JC_INT32 inlen, JC_UINT8 *out) ;
#endif /** EMAIL_PROTOCOL_ENABLE  **/

#endif


