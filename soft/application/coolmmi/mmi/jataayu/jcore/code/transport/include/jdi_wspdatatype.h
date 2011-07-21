/***************************************************************************
 * $Id: jdi_wspdatatype.h,v 1.4 2006/12/21 07:24:39 browserdevjtbase Exp $
 * $Revision: 1.4 $
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
 * $Log: jdi_wspdatatype.h,v $
 * Revision 1.4  2006/12/21 07:24:39  browserdevjtbase
 * Update
 *
 * Revision 1.3  2006/06/14 06:40:15  browserdevjtbase
 * Update ...
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.9  2006/03/04 11:52:30  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2006/03/04 07:51:15  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2006/03/01 14:57:58  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_wspdatatype.h
 * @ingroup WAPStack
 * @brief Defines the error codes, enums, structures which are exposed to use
 *		  the WSP functions
 *
 * This file defines the various enums and structures which are exposed 
 * for using the WSP functions. 
 */

#ifndef _STACK_INTERFACE_H
#define _STACK_INTERFACE_H


/*		Client Options		*/

/**
 * @brief This parameter name is to be used by the application (while using jdi_WSPSetOptions)
 *		  for setting the WSP time out value
 */
#define JWS_CLIENT_OPT_MAXTIMEOUT 			1

/**
 * @brief This parameter name is to be used by the application (while using jdi_WSPSetOptions)
 *		  for setting the maximum retries value
 */
#define JWS_CLIENT_OPT_MAXRETRIES 			2

/**
 * @brief This parameter name is to be used by the application (while using jdi_WSPSetOptions)
 *		  for setting the maximum outstanding methods or requests that should be processed by the WSP 
 *		  at any time
 */
#define JWS_CLIENT_OPT_MOM 					3	

/**
 * @brief This parameter name is to be used by the application (while using jdi_WSPSetOptions)
 *		  to specify the current bearer information 
 */
#define JWS_CLIENT_OPT_CURRENTBEARER 		4

/**
 * @brief This parameter name is to be used by the application (while using jdi_WSPSetOptions)
 *        to specify its preferred bearer information.
 */
#define JWS_CLIENT_OPT_PREFERREDBEARER 		5

/**
 * @brief This parameter name is to be used by the application (while using jdi_WSPSetOptions)
 *		  to start the WSP push.
 */
#define JWS_CLIENT_OPT_ACCEPTPUSH 			6

/**
 * @brief This parameter name is to be used by the application (while using jdi_WSPSetOptions)
 *		  to specify that it wants wap stack to use secured connection */
#define JWS_CLIENT_OPT_USESECURITY 			7

/**
 * @brief This parameter name is to be used by the application (while using jdi_WSPSetOptions)
 *		  for setting the WSP version
 */
#define JWS_CLIENT_OPT_VERSION 				8

/**
 * @brief This parameter name is  to be used by the application (while using jdi_WSPSetOptions)
 *		  for setting the client capabilities
 */
#define JWS_CLIENT_OPT_CAPABILITIES 		9

/**
 * @brief This parameter name is  to be used by the application (while using jdi_WSPSetOptions)
 *		  for setting the client authorization information.
 */
#define JWS_CLIENT_OPT_USEAUTHORIZATION		10


/** 
 * @brief Value used to set the WSP Protocol options for confirmed push. 
 */
#define PROTO_CONFIRM_PUSH					0x80

/** 
 * @brief Value used to set the WSP Protocol options for unconfirmed push. 
 */
#define PROTO_PUSH							0x40

/** 
 * @brief Value used to set the WSP Protocol options for session-resume feature. 
 */
#define PROTO_SESSION_RESUME				0x20

/** 
 * @brief Value used to set the WSP Protocol options for ack header. 
 */
#define PROTO_ACK_HEADER					0x10

/** 
 * @brief Value used to set the WSP Protocol options for large data transfer. 
 */
#define PROTO_LARGE_DATA					0x08


/**
 * @brief	Specifies the prototype of the request callback function that is to be implemented
 *			by the applicaiton.
 * @param[in]	pucBuffer The user data is to be returned in this field.
 * @param[in]	uiBufferLen Specifies the length of user data that is expected by the wap stack
 * @param[in]	vReqHandle Specifies the handle of the request for which the response is sent.
 * @param[in]	pvReference Specifies the application arguement.
 * @param[out]	puiActualLen Specifies the length of the user data that was finally returned by the application.
 * @see			jdi_WSPSetRequestCallback
 *
 * When the wap stack requires a user data from the application it will call an application function
 * which has the following prototype.
 */

typedef JC_RETCODE (* JDI_STACK_REQUEST_CB) (JC_UINT8			*pucBuffer,
											 JC_UINT32			uiBufferLen,
											 JC_HANDLE			vReqHandle,
											 void				*pvReference,
											 JC_UINT32			*puiActualLen) ;

/**
 * @brief	Specifies the prototype of the response callback function that is to be implemented
 *			by the applicaiton.
 * @param[in]	pucBuffer Specifies the wap stack response
 * @param[in]	uiBufferLen Specifies the length of the wap stack response
 * @param[in]	uiStatus Specifies the request status
 * @param[in]	pcContentType Specifies the content type of the wap stack response
 * @param[in]	pucHeader Specifies the header information
 * @param[in]	uiHeaderLen Specifies the header length
 * @param[in]	bIsDataEnd Specifies whether more responses have to sent from the wap stack to the application.
 * @param[in]	vReqHandle Specifies the handle of the request for which the response is sent.
 * @param[in]	pvReference Specifies the application arguement.
 * @see			jdi_WSPSetResponseCallback
 *
 * The application needs to implement a function with the following prototype in order to 
 * receive response (content) from the wap stack. 
 */

typedef JC_RETCODE (* JDI_STACK_RESPONSE_CB) (JC_UINT8			*pucBuffer,
											  JC_UINT32			uiBufferLen,
											  JC_UINT8			uiStatus,
											  JC_INT8			*pcContentType,
											  JC_UINT8			*pucHeader,
											  JC_UINT32			uiHeaderLen,
											  JC_BOOLEAN		bIsDataEnd,
											  JC_HANDLE			vReqHandle,
											  void				*pvReference) ;

/**
 * @brief The application needs to use this enum to specify the type of request sent to the WSP.
 */
typedef enum _wsp_request_type
{
	E_WSP_GET, /**< Specifies that the request given is a get request */
	E_WSP_HEAD, /**< Specifies that the request given is a fetch header request */
	E_WSP_DELETE, /**< Specifies that the request given is a delete request */
	E_WSP_POST, /**< Specifies that the request given is a post request */
	E_WSP_PUT /**< Specifies that the request given is a put request */
} E_WSP_REQUEST_TYPE ;

/**
 * @brief The application needs to use this structure to either set or get the client capability
 */
typedef struct client_capability
{
	JC_INT32		client_sdu_size ;  
	JC_INT32		server_sdu_size ;  
	JC_INT32		client_msg_size ;  
	JC_INT32		server_msg_size ;  
	JC_INT32		method_MOR ; /**< Specifies the maximum requests that can be processed by Wap stack in case the WSP MOR is used. */			
	JC_INT32		push_MOR ; /**< Specifies the maximum push requests that can be processed by the Wap stack in case the WSP MOR is used. */				
	JC_INT32		protocol_options ; /**< The application uses this to set the protocol options. */	
} WSP_CLIENT_CAPABILITY, CLIENT_CAPABILITY ;

#ifdef WSP_SYNC_ENABLE
/**
 * @brief If the application requests the wap stack to perform a sync WSP send (blocking) then
 *		  this structure is to be used. Since it is sync function the the wap stack will return the
 *        server response in the same structure.
 */
typedef struct wsp_request
{
	JC_INT8					*pszURL ; /**< Specifies the URL to be fetched */
	JC_INT8					*pPostData ; /**< Specifies the post data */
	JC_INT32				iPostLen ; /**< Specifies the post data length */
	E_WSP_REQUEST_TYPE		eRequestType ; /**< Specifies the request type */
	JC_UINT8				*pucBuffer ; /**< The wap stack returns the server response of the WSP send in this field */
	JC_INT32				iBufferSize ; /**< Specifies the size of the server response */
	JC_INT8					*pszContentType ; /**< Specifies the content type */
	JC_UINT8				*pucHeader ; /**< Specifies the request header */
	JC_INT32				iHeaderSize ; /**< Specifies the header length */
	JC_INT32				iStatus ; /** Specifies the status of the request */
} WSPREQUEST ;
#endif

/**
 * @brief The application needs to use this structure to specify the digest authorization
 *		  information.
 */
typedef struct wsp_digest
{
	JC_INT8		*pcUserName ; /**< Specifies the user name */
	JC_INT8		*pcPassword ; /**< Specifies the password */
	JC_INT8		*pcDomain ; /**< Specifies the domain name */
	JC_INT8		*pcRealm ; /**< Specifies the realm information  */
	JC_INT8		*pcNonce ; /**< Specifies the NOnce information */
	JC_INT8		*pcOpaque ; /**< Specifies the opaque parameter in the digest authorization. */
	JC_INT8		*pcQOP ; /**< Specifies the qop parameter in the digest authorization. */
	JC_INT8		*pcAlg ; /**< Specifies the algorithm parameter in the digest authorization. */	
} WSP_DIGEST_AUTH ;

#ifdef WAPSTACK_PUSH

/**
 * @brief The application needs to use this enum to specify the type of push session to be started.
 *		  This enum can be used only if WAPSTACK_PUSH is enabled
 */
typedef enum _push_type
{
	E_WSP_CONNECTION_ORIENTED_PUSH, /**< Specifies that connection oriented push session */
	E_WSP_CONNECTION_LESS_PUSH /**< Specifies connection less push session */
} EPushType ;

/**
 * @brief The wap stack returns a parsed WSP push message to the application by using the following
 *		  structure. 
 */

typedef struct push_msg
{
	JC_INT32			buflen ; /**< Specifies the length of the push message */
	JC_INT32			header_len ; /**< Specifies the length of the push header information */
	JC_UINT8			*buf ; /**< Specifies the push message */
	JC_INT8				*push_server ; /**< Specifies the name of the server from which the push message is originated */
	JC_UINT8			*header ; /**< Specifies the header information of the push message */ 
	JC_UINT8			push_type ; /**< Specifies the type of push message */
	JC_UINT8			*content_type ; /**< Specifies the content type of the push message */
	JC_UINT16			push_id ; /**< Specifies the ID of the push message */
	struct push_msg		*next ; /**< Pointer to the next push message information */
} PUSH_MSG ;

#endif

#ifdef WAP_WITH_WTLS

/**
 * @brief The application needs to use this enum to specify the WTLS class
 */

typedef enum wtls_class 
{
	E_NO_WTLS = 0,
	E_CLASS_I = 1, /**< Specifies class I level of security */
	E_CLASS_II, /**< Specifies class II level of security */
	E_CLASS_III /**< Specifies class III level of security */
} E_WTLSClass ;

/**
 * @brief	Specifies the prototype of the certificate callback function that is to be implemented
 *			by the applicaiton.
 * @param[in]	pvHandle Specifies the application handle.
 * @param[in]	uhCertLength Specifies the length of the certificate data.
 * @param[in]	pucCertData Specifies the certificate data.
 * @note		This prototype can be used only if WAP_WITH_WTLS is enabled.
 *
 * The application needs to implement a function with the following prototype in order to receive 
 * any certificate data from the Wap stack. 
 */

typedef JC_RETCODE (*JL_WTLS_CERT_CALLBACK) (void						*pvHandle,
											 JC_UINT16					uhCertLength,
											 const JC_UINT8				*pucCertData) ;

/**
 * @brief The application needs to use this structure to add a user certificate to the Wap stack
 */

typedef struct client_certificate_info 
{
	JC_UINT8	*cert ; /**< Specifies the certificate data */
	JC_UINT32	cert_size ; /**< Specifies the size of the certificate data */
	JC_UINT8	*password ; /**< Specifies the password */
	JC_UINT8	*key ; /**< Specifies the key information */
	JC_UINT32	key_size ; /**< Specifies the length of the key information */
} CLIENT_CERTIFICATE_INFO ;

#endif 

#endif



