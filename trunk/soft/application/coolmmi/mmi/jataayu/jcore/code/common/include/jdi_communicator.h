/***************************************************************************
 * $Id: jdi_communicator.h,v 1.25 2009/06/02 04:44:44 browserdevjtbase Exp $
 * $Revision: 1.25 $
 * $Date: 2009/06/02 04:44:44 $
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
 * $Log: jdi_communicator.h,v $
 * Revision 1.25  2009/06/02 04:44:44  browserdevjtbase
 * Core Update
 *
 * Revision 1.24  2009/04/13 06:38:21  browserdevjtbase
 * Core Update
 *
 * Revision 1.64  2009/03/24 10:05:00  sriramdevhtmlbrow
 * When PUSH session ID is passed as zero all the push sessions will be deinitialized.
 *
 * Revision 1.63  2009/03/04 13:04:13  kavithadevhtmlbrow
 * Doxygen Comments Updated
 *
 * Revision 1.62  2009/02/12 14:17:38  arpitadevhtmlbrow
 * Network Confirm respose handling
 *
 * Revision 1.61  2009/01/06 07:28:50  sriramdevhtmlbrow
 * New member iMaxSocketAliveTime added in set option to configure the persistent socket alive time at run time.
 *
 *  
 ***************************************************************************/

/**
 * @mainpage
 *
 * @version 2.5
 * Interface Description
 *
 * @section Introduction
 * Communicator provides the interface for the application to send a request to the transport. i.e. the
 * communicator accepts the request from the application and forms an event and sends the same to 
 * the transport queue. Interface is provided to fetch a page, set the data connection and profile.
 * stop request, disconnet, suspend and resume, certificate handling, push handling, security indications
 * etc.  
 *
 * It Handles the responses and requests that are sent from the transport. The transport sends all the
 * response and requests to the application (which has initialized the session). The application can handle
 * the events that it wants to and the rest can be forwarded to the communicator. The communicator
 * will interpret the transport event and indicate the application about what needs to be done.
 *
 * This Documentation gives a detailed description of the communicator interface that has to be used
 * by an application to interact with the transport task. This documentation describes the usage of each function, 
 * the significance of each variable, usage of the data structures and enumerations.  The
 * document will ease the complexity faced while interfacing the application with the Communicator.
 *
 * @li @ref jcf
 * @li @ref flow
 * @li @ref security
 * @li @ref ipstack
 *
 *
 * @page flow Basic Flow
 * This section describes the basic API flow that the application needs to follow.
 * @li <b> Transport Task: </b> Transport task is the separate task which handles the data connection
 * and integration with HTTP and WSP Protocol layer. It accepts the request through events and sends
 * the response through events. Before invoking the communicator API's, the application should make 
 * sure that the transport task has been initialized/started.
 * 
 * @li <b> Communicator Initialize: </b> The communicator can be initialized by invoking the API
 * #jdi_CommunicationInitialize. The API will internally initialize the lower layer. While calling
 * this API the application needs to mention the application callback which should be used while
 * sending indications from the communicator to the application.
 *
 * @li <b> Set Data Connection: </b> The application needs to use the API #jdi_CommunicationDataConnection
 * to specify the data connection information (that should be used by the lower layer). The application
 * should call this API before calling the set Profile API else the data connection information will
 * not be updated to the lower layer.
 *
 * @li <b> Set Profile: </b> The application needs to use the API #jdi_CommunicationProfile to specify
 * the profile (IP, port etc). 
 *
 * @li <b> Fetch Request: </b> The application needs to use the API #jdi_CommunicationFetchURL to send 
 * fetch request. The response for this fetch request is provided by the communicator through the callback
 * function (#CB_COMM_GENERAL_INDICATION), which has been registered while initialization. Once the request 
 * has been processed the application needs to call #jdi_CommunicationFreeReference to free the memory
 * used by this particular request. 
 *
 * @li <b> Stop Request: </b> Communicator provides the following API to stop the requests that are
 * currently getting processed. <BR>
 *			(1) #jdi_CommunicationStopReq <BR>
 *			(2) #jdi_CommunicationStopAllReq <BR>
 * 
 * @li <b> Utility API's: </b> Communicator also provides the following utility API's. They are as follows: <BR>
 *			(1) #jdi_CommunicationSendSessionHeader <BR>
 *			(2) #jdi_CommunicationClearAuthInfo <BR>
 *			(3) #jdi_CommunicationGetFetchURL <BR>
 *			(4) #jdi_CommunicationSetOption <BR>
 *
 * @li <b> Suspend and Resume: </b> Communicator provides the following API's to allow the application
 * to suspend and resume a request or the entire communicator session. <BR>
 *			(1) #jdi_CommunicationSuspendFetchRequest <BR>
 *			(2) #jdi_CommunicationResumeFetchRequest <BR>
 *			(3) #jdi_CommunicationSuspendSessionRequest <BR>
 *			(4) #jdi_CommunicationResumeSessionRequest <BR>
 *
 * @li <b> Certificate handling: </b> Communicator provides the following API's to add and clear
 * the certificates. <BR>
 *			(1) #jdi_CommunicationAddRootCertificate <BR>
 *			(2) #jdi_CommunicationAddUserCertificate <BR>
 *			(3) #jdi_CommunicationClearRootCertificate <BR>
 *			(4) #jdi_CommunicationClearUserCertificate <BR>
 *
 * @li <b> Push Session Handling: </b> Communicator provides the following API's to allow the application
 * to initialize a push session (confirmed / HTTP push session) at lower layer and deintialize a push session. <BR>
 *			(1) #jdi_CommunicationSendPushInitReq  <BR>
 *			(2) #jdi_CommunicationSendPushDeinitReq <BR>
 *
 * @li <b> Security Indication Response: </b> The communicator sends security indication once security
 * is establised. The application needs to use the API #jdi_CommunicationSecurityIndResponse to send
 * its response (whether the security negotiated is accepted or not). Similarly for security failure 
 * indication the application needs to use the API #jdi_CommunicationSendSecurityFailureResponse to send its
 * response (whether to proceed with next class of security).
 *
 * @li <b> Event Handling: </b> As mentioned above the transport will send the response and request 
 * events to the application. When application receives any event and the destination task is of transport task, 
 * the application needs to send the event to the API #jdi_CommunicationEventHandler. This API
 * will interpret the transport event and if required updates the application through the callback function, which has 
 * been registered during the initialization. 
 *
 * @li <b> Disconnect: </b> The application needs to use the API #jdi_CommunicationDisconnect to disconnect
 * the data connection that is currently being used. The application then needs use the set profile
 * function (#jdi_CommunicationDataConnection followed by #jdi_CommunicationProfile) in order to set
 * the new data connection.
 *
 * @li <b> Deinitialize: </b> The application needs to use the API #jdi_CommunicationDeinitialize to deinitialize
 * the communicator. The communicator will in turn the deinitialze the transport session which 
 * was created for the application.
 *
 * The below diagram describes the basic flow for the communicator.
 * @image html Communicator.jpg
 * 
 * @page security Security Interface
 * 
 * Following section describes the way the Communicator and its application needs to communicate to 
 * handle the indications/contents that are received when security is enabled.
 *
 * Basically the security indications work in 'indication-response' mechanism. i.e. the Communicator will send the security
 * indications to its application. The Application needs to process the indications (user can be involved/prompted) and 
 * then send the response back to the Communicator. Once the Communicator receives the response then it will proceed/cancel
 * the request being processed. 
 *
 * @note Until the security indication response comes from the Application, the Communicator (or to be precise the lower layer)
 * suspends the request which is  being processed. So it is neccessary to handle the security interface.
 *
 * @section Security-Indication Security Indication
 *
 * @li Once the security is established successfully the Communicator will send the security indication
 * to application using the event type #E_SECURITY_INDICATION (sent to the applications general callback which
 * was registered while initializing the communicator). In this event type the application will receive the
 * security indication in the structure #ST_COMM_SECURITY_IND. This structure will contain the security details
 * such as the certificates, cipher suite, key exchange etc that is negotiated while establishing the security.
 *
 * @li The application needs to verify the security details (user can be invoved/prompted) and then decide whether
 * to proceed with the type of security that is negotiated. The application needs to send its response (OK / Cancel)
 * using the API #jdi_CommunicationSecurityIndResponse.
 *
 * @section Certificate Receiving Certificates
 *
 * After the security is established the lower layer will receive a certificate which specifies the details of the
 * security session that is currently established. There is a possibility that more than one certificate
 * is received by the lower layer. In the #ST_COMM_SECURITY_IND the communicator will pass only the first certificate
 * of the certificate list. In this struture the field 'uiTotalCertificate' will specify the actual number of certificates that
 * has been been received at lower layer. 
 *
 * @li To receive the other certificates the application has to use the API jdi_CommunicationGetCertInfo. The communicator
 * will send an event to retrieve the cert info.
 *
 * @li The communicator will send the certificate inforamtion back to application by using the callback
 * event type #E_CERT_RESP_INDICATION. 
 *
 * @note The application should retrieve the certificates before sending the security indication response
 * (i.e. before calling #jdi_CommunicationSecurityIndResponse). 
 *
 * @section Security-Failure Security Failure Indication
 * 
 * In case of WSP Secure mode (WTLS) there exists 3 classes of security:
 * 
 * @li Class III: Lower layer attempts this class if both root and user certificate has been added by the application.
 * @li Class II: Lower layer attempts this class if root certificate has been added by the application.
 * @li Class I: Lower layer attempts this without adding any certificates.
 *
 * If a higher class of security fails (Class III or class II) then the application should decide whether
 * to proceed with next lower class of security. In order to do that the following mechanism has to be followed.
 *
 * @li When the security fails the communicator will send the security failure indication to application
 * using the event type #E_SECURITY_FAILURE_INDICATION (sent to the appliction general callback which 
 * was registered while initializing the communicator). In this event type the application will get the 
 * security failure indication in the structure ST_COMM_SECURITY_FAILURE_IND. This structure will contain
 * the information about the class that has failed.
 *
 * @li The application needs to decide (user can be involved/propmted) whether to proceed with next lower layer of
 * security. The application needs to send its response (OK / Cancel) using the API #jdi_CommunicationSendSecurityFailureResponse.
 *
 * @note The Security failure indication is sent only for class III and class II. Class I failure is treated
 * as a general failure. Also the security failure indication will not be received in HTTP mode.
 *
 * @page ipstack IP Stack
 * Following section describes the various API's provided by the communciator to use the IP
 * stack functionalities provided by the lower layer.
 *
 * @li <b>Set IP Profile:</b> The application needs to use the API #jdi_CommunicationIPProfile to set the IP
 * profile. Before setting the profile the application should set the data connection information using
 * #jdi_CommunicationDataConnection.
 *
 * @li <b>Send IP Request:</b>The application needs to use the API #jdi_CommunicationIPSendReq to send an
 * IP request to the lower layer. The lower layer will send all the request to the server and then
 * start receiving the content. The content will be then forwarded to the application using #E_FETCH_INDICATION.
 * Since multiple IP requests can be proceessed at the same time, the lower layer will not know for which
 * request the response is being received. So the request ID while sending #E_FETCH_INDICATION will be 0.
 * If an error occurs for the request after it has started processing (due to data disconnetion etc) then
 * the communicator will send the error to the application using #E_ERROR_INDICATION and this time
 * the request ID will be present.
 *
 * @li <b>Stop IP request:</b> The application needs to use the API #jdi_CommunicationIPStopReq to stop the 
 * IP requests currently being processed. The lower layer will stop all the IP requests (sending and receiving)
 * and then send the response using #E_STOP_INDICATION. This indication will sent only once, with request ID as 0
 * (it wont be sent one by one for all the requests).
 *
 * @li <b>Change security mode:</b> The application needs to use the API #jdi_CommunicationIPChangeSecurityMode
 * to change the security mode. There is no response indication for this API. When this API is called
 * the lower layer will stop the current sending and receiving. After that all the fresh requests will
 * use the new security mode.
 */

/**
 * @file jdi_communicator.h
 * @ingroup Communicator
 * @brief Defines the exposed enumerations, structures and APIs of the communicator that
 *		  can be used by the browser application.
 */

#ifndef _JDI_COMMUNICATOR_H_
#define _JDI_COMMUNICATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @cond */
#include <jdi_commonmodule.h>
#include <jdi_mimeinfo.h>
#include <jdi_httpstatus.h>
#ifdef JCF_SSL
#include <jdi_ssl.h>
#else
#include <jdi_httpssl.h>
#endif

#ifdef JCF_WTLS
#include <jdi_wtls.h>
#else
#include <jdi_wspwtls.h>
#endif
/** @endcond */

/**
 * @brief Specifies the maximum size of the hostname for the profile
 */
#define MAX_COMM_HOSTNAME_SIZE			255
/**
 * @brief Specifies the maximum size of the charset for the header information
 */
#define MAX_CHARSET_SIZE				255
/**
 * @brief Specifies the maximum size of the header name for the header information
 */
#define MAX_HEADER_NAME_SIZE			255
/**
 * @brief Specifies the maximum size of the IP Address
 */
#define MAX_COMM_IP_SIZE				15
/**
 * @brief Specifies the maximum size of the content type
 */
#define MAX_CONTENT_TYPE_LEN	255

/**
 * @brief The enumeration contains the indication types that the communicator will be 
 *		  sending to the application.
 */
typedef enum _comm_indication_type
{
	E_PROGRESS_INDICATION, /**< Specifies to the application about the content received from the lower layers (Argument #ST_COMM_PROGRESS_IND) */
	E_STOP_INDICATION, /**< Specifies the response of a stop request that was previously sent from the application (Argument #ST_COMM_STOP_IND)  */
	E_STOP_ALL_INDICATION, /**< Specifies the response of stop all request that was previously sent from the application (Argument #ST_COMM_STOP_IND)  */
	E_PROFILE_CONFIRMATION_INDICATION, /**< Specifies the profile confirmation required for the lower layer to proceed for fetching operations (Argument #ST_COMM_PROFILE_CONFIRMATION_IND) */
	E_USER_DATA_INDICATION, /**< Specifies that the lower layer requires uses data (post data info) from application to proceed (Argument #ST_USER_DATA_IND)  */
	E_HEADER_INDICATION, /**< Specifies the header information for a fetch request (Argument #ST_COMM_HEADER_IND). */
	E_FETCH_INDICATION, /**< Specifies the fetch indication for a fetch request (Argument #ST_COMM_FETCH_IND). */
	E_ERROR_INDICATION, /**< Specifies to application the error (if any) for a fetch request (Argument #ST_COMM_ERR_IND). */
	E_PUSH_INDICATION, /**< Specifies the push data that is received by the lower layer (Argument #ST_COMM_PUSH_IND). */
	E_SECURITY_INDICATION, /**< Specifies details of the security that is negotiated by the lower layer (Argument #ST_COMM_SECURITY_IND). */
	E_SECURITY_FAILURE_INDICATION, /**< Specifies details of security failure (if any) (Argument #ST_COMM_SECURITY_FAILURE_IND) . */
	E_CERT_RESP_INDICATION, /**< Specifies the response of the certificate request which was previously sent by the application (Arguement #ST_COMM_CERT_RESP_IND) */
	E_COMM_MAX_TRANSPORT_REQUESTS /**< Specifies the maximum number of requests that can be handled in transport at a time. */
} E_COMM_INDICATION ;

/**
 * @brief The enumeration contains the error types that the communicator will send to
 *		  application error handler function.
 */
typedef enum _comm_err_type
{
	E_ERR_COMM_INITIALIZE, /**< Specifies error while initializating communicator. */
	E_ERR_COMM_DEINITIALIZE, /**< Specifies error while deinitializating communicator. */
	E_ERR_COMM_DISCONNECT, /**< Specifies error while disconnecting. */
	E_ERR_COMM_PROFILE, /**< Specifies error while setting profile. */
	E_ERR_COMM_REQUEST, /**< Specifies error while sending the fetch request. */
	E_ERR_COMM_CERTIFICATE, /**< Specifies error while perfomring certificate related operation. */
	E_ERR_COMM_LOW_MEMORY /**< Specifies low memory error. */
} E_COMM_ERR_TYPE ;

/**
 * @brief The enumeration contains the various types of headers.
 */
typedef enum comm_header_type
{
	E_COMM_BINARY_HEADER = 0, /**< Specifies that header is in binary format. */
	E_COMM_TEXT_HEADER /**< Specifies that header is in text format.*/ 
} E_COMM_HEADER_TYPE ;

/**
 * @brief The enumeration contains the various connection modes that can be used by the
 *		  application while setting up the connection.
 */
typedef enum comm_connection_mode
{
	E_COMM_WSP_CONNECTION_ORIENTED, /**< Specifies connection mode to be WSP connection oriented. */
	E_COMM_WSP_CONNECTION_ORIENTED_SECURE, /**< Specifies Connection mode to be WSP Secure connection oriented. */
	E_COMM_WPHTTP_PROXY, /**< Specifies connection mode to be HTTP Proxy. */
	E_COMM_WPHTTP_DIRECT /**< Specifies connection mode to be HTTP direct. */
} E_COMM_CONNECTION_MODE ;

/**
 * @brief The enumeration contains the various states of the communicator stack. The communicator
 *		  stack internally describes the status of the lower layer stacks. The 
 *		  communicator will send this value in the progress indication.
 */
typedef enum comm_stack_status
{
	E_COMM_STACK_NONE, /**< Specifies that communicator stack is in NIL state. */
	E_COMM_STACK_CONNECTING, /**< Specifies that communicator stack is in connecting state. */ 
	E_COMM_STACK_CONNECTED, /**< Specifies that communicator stack is in connected state. */
	E_COMM_STACK_SENDING, /**< Specifies that communicator stack is in sending state. */
	E_COMM_STACK_RECEIVING, /**< Specifies that communicator stack is in receiving state. */
	E_COMM_STACK_RETRYING, /**< Specifies that communicator stack is in retrying state. */
	E_COMM_STACK_DISCONNECTING, /**< Specifies that communicator stack is in disconnecting state. */
	E_COMM_STACK_DISCONNECTED /**< Specifies that communicator stack is in disconnected state. */
} E_COMM_STACK_STATUS ;

/**
 * @brief The enumneration contains the different types of fetch requests that the application
 *		  can give to the communicator.	
 */
typedef enum comm_fetch_type
{
	E_COMM_FETCH_GET, /**< Specifies the request given is fetch request. */
	E_COMM_FETCH_POST, /**< Specifies the request given is post request. */
	E_COMM_FETCH_HEAD /**< Specifies the request given is fetch header request. */
} E_COMM_FETCH_TYPE ;

/**
 * @brief The enumeration contains the different request types that can be given from the 
 *		  application.
 */
typedef enum comm_request_type
{
	E_COMM_MAIN_REQUEST, /**< Specifies that the request is to fetch a main page. */
	E_COMM_CSS_REQUEST, /**< Specifies that the request is to fetch a CSS content */
	E_COMM_IMAGE_REQUEST, /**< Specifies that the request is to fetch a image */
	E_COMM_AUDIO_REQUEST, /**< Specifies that the request is to fetch a audio */
	E_COMM_VIDEO_REQUEST, /**< Specifies that the request is to fetch a video */
	E_COMM_OTHER_REQUEST /**< Specifies that the request is. */
} E_COMM_REQUEST_TYPE ;

/**
 * @brief The application needs to use this enumeration to specify the type of authorization
 *		  list that is to be cleared from the lower layers.
 */
typedef enum comm_clear_auth_header
{
	E_COMM_CLEAR_URI_AUTHORIZATION, /**< Specifies that the URL authorization list is to be cleared */
	E_COMM_CLEAR_PROXY_AUTHORIZATION, /**< Specifies that the proxy authorization list is to be cleared */
	E_COMM_CLEAR_ALL /** Spcifies that both URI and proxy authorization list is to be cleared. */
} E_COMM_CLEAR_AUTH_HEADER ;

/**
 * @brief The enumeration contains the different security classes used in the lower layers. The
 *	      communicator sends this enumeration to the application when a particular class of 
 *        security has failed.
 */
typedef enum comm_security_class
{
	E_COMM_CLASS_II, /**< Specifies that the failure occured for class II level of security. */
	E_COMM_CLASS_III /**< Specifies that the failure occured for class III level of security. */
} E_COMM_SECURITY_CLASS ;

/**
 * @brief The enumeration contains the different types of push. The application needs to mention
 *		  this enum value when it sends the push initialization request to the communicator.
 */
typedef enum comm_push_type
{
	E_COMM_CONFIRMED_PUSH, /**< Specifies that push type is confirmed push */
	E_COMM_HTTP_PUSH /**< Specifies that push type is HTTP push */
} E_COMM_PUSH_TYPE ;

/**
 * @brief The enumneration defines the various profile confirmation types that can be sent to the
 *		  application 
 */
typedef enum comm_profile_confirmation_type
{
	E_COMM_USE_SECONDARY_CONNECTION_MODE, /**< Specifies that confirmation is required to use secondary connection mode. */
	E_COMM_USE_SECONDARY_DATA_CONNECTION, /**< Specifies that confirmation is required to use secondary data connection */
	E_COMM_USE_TERTIARY_DATA_CONNECTION, /**< Specifies that confirmation is required to use tertiary data connection */
	E_COMM_EX_PROFILE_RETRY /**< Specifies that confirmation is required to start the whole extended profile once again. */
} E_COMM_PROFILE_CONFIRMATION_TYPE ;

/**
 * @brief This structure describes the profile structure that the application needs to use to
 *		  send a set transport profile from communicator to transport.
 */
typedef struct comm_profile
{
	E_COMM_CONNECTION_MODE	eConnMode ; /**< Specifies the connection mode i.e. wsp, http or direct */
	JC_INT8					acHostName [MAX_COMM_HOSTNAME_SIZE + 1] ; /**< Specifies the host name */
	JC_UINT32				uiPortNumber ; /**< Specifies the port number */
	JC_UINT32				uiSecurePort ; /**< This is used only for proxy connection. This is used for socket connection instead of uiPortNum for secure requests */
	JC_BOOLEAN				bDeleteSessionHeaders ; /**< Specifies whether the existing session headers of 
														 the session has to be deleted when the profile is being changed*/
} ST_COMM_PROFILE ;

/**
 * @brief This structure describes the extended profile structure that the application needs to use to
 *		  send a set transport profile from communicator to transport.
 */
typedef struct comm_extended_profile
{
	JC_BOOLEAN					bDeleteSessionHeaders ; /**< Specifies whether the existing session headers of 
															 the session has to be deleted when the profile is being changed*/
	E_COMM_CONNECTION_MODE		ePrimaryConnectionMode ; /**< Specifies the primary connection mode */
	JC_INT8						acPrimaryHostName [MAX_COMM_HOSTNAME_SIZE + 1] ; /**< Specifies the primary host name */
	JC_UINT32					uiPrimaryPortNum ; /**< Specifies the primary port number */
	JC_BOOLEAN					bSecondaryUserConfRequired ; /**< Specifies whether, before using secondary connection user confirmation is required or not */
	E_COMM_CONNECTION_MODE		eSecondaryConnectionMode ; /**< Specifies the secondary connection mode */
	JC_INT8						acSecondaryHostName [MAX_COMM_HOSTNAME_SIZE + 1] ; /**< Specifies the secondary host name */
	JC_UINT32					uiSecondaryPortNum ; /**< Specifies the secondary port number */
	ST_GENERIC_DATA_CONN		stPrimaryDataConn ; /**< Specifies the Primary data connection */
	JC_BOOLEAN					bSecDataConnUserConfRequired ; /**< Specifies whether, before using secondary data connection user confirmation is required or not */
	ST_GENERIC_DATA_CONN		*pstSecondaryDataConn ; /**< Specifies the Secondary data connection */
	JC_BOOLEAN					bTertiaryDataConnUserConfRequired ; /**< Specifies whether, before using tertiary data connection user confirmation is required or not */
	ST_GENERIC_DATA_CONN		*pstTertiaryDataConn ; /**< Specifies the Tertiary data connection */
} ST_COMM_EXTENDED_PROFILE ;

/**
 * @brief This structure is used by the communicator to specify the profile confirmation indication
 *		  to the application.
 */
typedef struct comm_profile_confirmation_ind
{
	E_COMM_PROFILE_CONFIRMATION_TYPE eProfileConfType; /**< Specifies the type of profile confirmation required */
} ST_COMM_PROFILE_CONFIRMATION_IND ;

#ifdef IP_STACK_ENABLE

/**
 * @brief This enumeration specifies the type of IP connection modes.
 */
typedef enum comm_ip_connection_mode
{
	E_COMM_TCP_MODE, /**< Specifies TCP connection mode */
	E_COMM_UDP_MODE /**< Specifies UDP connection mode */
} E_COMM_IP_CONNECTION_MODE ;

/**
 * @brief This enumeration is to be used by the application to set the security mode.
 */
typedef enum comm_ip_security_mode
{
#ifdef JCF_SSL
	E_COMM_IP_SECURE, /** Specifies that secure mode is to be used. */
#endif
	 E_COMM_IP_NON_SECURE /** Specifies that non-secure mode is to be used. */
} E_COMM_IP_SECURITY_MODE ;

/**
 * @brief This structure is to be used by the application to set the IP profile.
 */
typedef struct comm_ip_profile
{
	E_COMM_IP_CONNECTION_MODE	eConnMode ; /**< Specifies the IP connection mode. */ 
	JC_INT8						acIPHostName [MAX_COMM_HOSTNAME_SIZE + 1] ; /**< Specifies the IP host name */
	JC_UINT32					uiIPPortNumber ; /**< Specifies the port number (for the host IP) */
	JC_INT8						acProxyHostName [MAX_COMM_HOSTNAME_SIZE + 1] ; /**< Specifies the proxy host name (can be empty string) */
	JC_UINT32					uiProxyPortNumber ; /**< Specifies the port number (for the proxy IP) */
} ST_COMM_IP_PROFILE ;

/**
 * @brief This structure is to be used by the application to send an IP request.
 */
typedef struct comm_ip_send_req
{
	JC_UINT8					*pucSendData ; /**< Specifies the IP request to be sent */
	JC_UINT32					uiSendDataLen ; /**< Specifies the length of the IP request */
} ST_COMM_IP_SEND_REQ ;

#endif

/**
 * @brief This structure describes the session header structure that the application needs to use to
 *		  send a Session header from communicator to transport.
 */
typedef struct comm_session_headers
{
	JC_BOOLEAN			bHasMoreHeader ; /**< Specifies whether the application will send more header information or not */
	JC_INT8				acCharset [MAX_CHARSET_SIZE + 1] ; /**< Specifies the charset information */
	JC_INT8				acName [MAX_HEADER_NAME_SIZE + 1] ; /**< Specifies the header name if any */
	JC_UINT32			uiHeaderLen ; /**< Specifies the length of the header */
	JC_UINT8			*pucHeader ; /**< Specifies the header */
} ST_COMM_SESSION_HEADER ;

/**
 * @brief This structure describes the push initialization structure that application needs to use
 *		  to send a initiailze push session request from communicator to transport.
 */
typedef struct comm_push_init
{
	E_COMM_PUSH_TYPE		eCommPushType ; /**< Specifies the Type of push to be initialized */
	JC_INT8					acIPAddress [MAX_COMM_IP_SIZE + 1] ; /**< Specifies the server address */
	JC_UINT32				uiPortNumber ; /**< Specifies the port number */
} ST_COMM_PUSH_INIT ;

/**
 * @brief This structure describes the push register structure that application needs to use
 *		  to register for receiving only specific type of push content.
 */
typedef struct comm_push_register_info
{
	JC_UINT32				uiPushAppID ; /**< Specifies the ID of the push content */
	JC_INT8					acContentType [MAX_CONTENT_TYPE_LEN + 1] ; /**< Specifies the content type of the push content */
} ST_COMM_PUSH_REGISTER_INFO ;

typedef ST_AUTH_HEADERS				ST_COMM_AUTH_HEADERS ;

/**
 * @brief This union is to be used by the application to specify whether the post data is sent
 *		  as buffer or is it stored in a local file. Incase post data is stored in file then the
 *		  communicator needs to read it from the file as and when required.
 */
typedef union _comm_user_data_mode
{
	JC_UINT8				*pucUserData ; /**< Specifies post data buffer */
	JC_CHAR					*pmFileName ; /**< Specifies the name of the file in which the post dat is stored */
} U_COMM_USER_DATA_MODE ;

/**
 * @brief This structure is to be used by the application when the fetch request is to be sent to
 *		  the communicator. It includes the URL, post data, headers, authorization info etc.
 */
typedef struct comm_fetch
{
	JC_CHAR					*pmURL ; /**< Specifies the URL to be fetched */
	JC_UINT32				uiUserDataLength ; /**< Specifies the length of the post data */
	JC_BOOLEAN				bIsFile ; /**< Specifies whether the application is passing post data buffer or post data file name */
	U_COMM_USER_DATA_MODE	uUserDataMode ; /**< Specifies the post data buffer or post data file */
	JC_INT8					*pcContentType ; /**< Specifies the content type of the fetch content */ 
	ST_MIME_HEADERS			*pstHeadersInfo ; /**< Specifies the headers to be sent along with the URL request */
	ST_COMM_AUTH_HEADERS	*pstAuthHeaders ; /**< Specifies the authorization headers to be sent along with the URL request */
	ST_COMM_AUTH_HEADERS	*pstProxyAuthHeaders ; /**< Specifies the proxy authorization headers to be sent along with the URL request */	
	JC_BOOLEAN				bUseMemory ; /**< Specifies whether all the memory pointers (URL, content type, headers, auth, post data) can be reused by the communicator */
	E_COMM_FETCH_TYPE		eCommFetchType ; /**< Specifies the Type of fetch (fetch, post or head) */
	E_COMM_REQUEST_TYPE		eCommRequestType ; /**< Specifies the type of request (main page, audio, video etc) */	
} ST_COMM_FETCH_URL ;

/**
 * @brief This structure is to be used by the application when it needs the communicator to send
 *		  an add root certificate request to the transport.
 */
typedef struct comm_root_cert
{
	JC_UINT8				*pucContent ; /**< Specifies the root certificate content */
	JC_UINT32				uiLength ; /**< Specifies the length of the root certificate content */
	JC_INT8					*pcContentType ; /**< Specifies the content type of the root certificate */
} ST_COMM_ROOT_CERT ;

/**
 * @brief This structure is to be used by the application when it needs the communicator to send
 *		  an add user certificate request to the transport.
 */
typedef struct comm_user_cert
{
	JC_UINT8				*pucUserCert ; /**< Specifies the user certificate content */
	JC_UINT32				uiCertLen ; /**< Specifies the length of the user certificate */
	JC_INT8					*pcContentType ; /**< Specifies the content type of the user certificate */
	JC_UINT8				*pucKey ; /**< Specifies the key associated with the user certificate */
	JC_UINT32				uiKeyLen ; /**< Specifies the length of the user certificate key */
	JC_INT8					*pcPassword ; /**< Specifies the password associated with the user certificate */
} ST_COMM_USER_CERT ;

/**
 * @brief This structure is to be used by the application when it needs the communicator to 
 *		  send an event which will set options at the lower layers. The options that can be set 
 *		  are the timeout value, setting the maximum number of retries in case of data failure,
 *		  maximum socket connections that can be used by the HTTP Stack and the maximum buffer
 *		  that can be received from socket layer at a time.
 */
typedef struct comm_set_option
{
	JC_INT32	iTimeOut ; /**< Specifies the time out value to be set at the lower layers */
	JC_INT32	iMaxRetries ; /**< Specifies the maximum number of connect retries that can be done at lower layers*/
	JC_INT32	iMaxSockets ; /**< Specifies the maximum number of socket connections (maximum socket pools in http stack) 
								   that can be used by HTTP Stack at a time. */
	JC_INT32	iMaxTcpRecvBuf ; /**< Specifies the maximum buffer that can be read from the socket layer at a time 
									  (this will be used for jdd_NetRecv function).*/
	JC_INT32	iMaxSocketAliveTime ; /**< Specifies the maximum time till which an idle persistent HTTP socket that can be kept alive without closing.*/
} ST_COMM_SET_OPTION ;

/**
 * @brief This structure is used by the communicator to specify the progress indication to the 
 *		  application.
 */	
typedef struct comm_prog_ind
{
	JC_UINT32					uiRequestID ; /**< Specifies the ID of request for which the progress ind is sent */
	JC_UINT32					uiBytesRecvd ; /**< Specifies the bytes received by the browser */
	JC_UINT32					uiBytesSent ; /**< Specifies the bytes sent from the browser */
	JC_UINT32					uiTotalBytesToRecv ; /**< Specifies the total bytes to be received by the browser */
	JC_UINT32					uiTotalBytesToSend ; /**< Specifies the total bytes to be sent by the browser */
	E_DATA_CONN_STATUS			eDataConnectionStatus ; /**< Specifies the data connection status */
	E_COMM_STACK_STATUS			eStackStatus ; /**< Specifies the lower layer stack status */
} ST_COMM_PROGRESS_IND ;

/**
 * @brief This structure is used by the communicator to specify the stop request's response 
 *		  to the application.
 */
typedef struct comm_stop_ind
{
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the stop ind is sent */
	JC_RETCODE			rCode ; /**< Specifies the error code of the stop operation */
} ST_COMM_STOP_IND ;

/**
 * @brief This structure is used by the communicator to get the user data indication from the
 *		  application. During the fetch request the application may specify the post data length
 *		  without the post data. In that case the communicator sends this structure to the 
 *		  application and expects the application to return the post data in the structure.
 */
typedef struct comm_user_data_ind
{
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the user data ind is sent */
	JC_UINT32			uiOffset ; /**< Specifies the offset of user data already read by the communicator */
	JC_UINT32			uiAllocLength ; /**< Specifies the length of user data expected by the communicator */ 
	JC_UINT8			*pucBuffer ; /**< Specifies the user data buffer to be filled by the application */
	JC_UINT32			uiUsedLength ; /**< The application returns the length of the user data buffer it actually filled */
} ST_USER_DATA_IND ;

/**
 * @brief This structure is used by the communicator to specify the header indication of a 
 *		  fetch request to the application.
 */
typedef struct comm_header_ind
{
	JC_UINT32				uiRequestID ; /**< Specifies the ID for which the header ind is sent */
	JC_UINT32				uiHeaderLength ; /**< Specifies the length of the header */
	JC_UINT32				uiContentLength ; /**< Specifies the length of the content (buffer) to be recevied */
	E_HTTP_STATUS			eHttpStatus; /**< Status of the lower layers (HTTP and WSP) */
	JC_BOOLEAN				bIsBinary ; /**< Specifies whether the content is binary or not. */
	JC_INT8					*pcContentType ; /**< Specifies the content type of the content to be received */
	JC_UINT8				*pucHeader ; /** Specifies the header */
	JC_CHAR					*pmUrl ; /** Specifies the URL for which the header is received. */
} ST_COMM_HEADER_IND ;

/**
 * @brief This structure is used by the communicator to specify the buffer indication of a 
 *		  fetch request to the application.
 */
typedef struct comm_fetch_ind
{
	JC_UINT32		uiRequestID ; /**< Specifies the ID for which the fetch (buffer) indication is sent */
	JC_UINT32		uiCurrentLength ; /**< Specifies the length of the buffer */
	JC_BOOLEAN		bHasMoreData ; /**< Specifies whether more data is expected from the lower layers */
	JC_UINT8		*pucBuffer ; /**< Specifies the buffer */
} ST_COMM_FETCH_IND ;

/**
 * @brief This structure is used by the communicator to specify the error indication of a 
 *		  fetch request to the application.
 */
typedef struct comm_error_ind
{
	E_COMM_ERR_TYPE		eErrType ; /**< Specifies the type of error */
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the error ind is sent */
	JC_RETCODE			rCode ; /**< Specifies the error code */
} ST_COMM_ERR_IND ;

/**
 * @brief This structure is used by the communicator to specify the Push indication to the 
 *		  application. Unlike the fetch request the push indication will have the header and
 *		  buffer in the same structure.
 */
typedef struct comm_push_ind
{
	E_HTTP_STATUS			eHttpStatus ; /**< Specifies the lower layer status (HTTP and WSP) */
	JC_UINT8				*pucBuffer ; /**< Specifies the push Buffer */
	JC_UINT32				uiLength ; /**< Specifies the push buffer length */
	JC_INT8					*pcContentType ; /**< Specifies the content type of the push message */
	JC_UINT8				*pucHeader ;/**< Specifies the push header. */
	JC_INT8					*pcPPGHostAddress ; /**< Specifies the Address of the PPG. */
	JC_INT8					*pcOrigAddress ; /**< Specifies the originating address (if the push is over SMS). */
	JC_UINT32				uiHeaderLength ; /**< Specifies the length of the push header */
	JC_BOOLEAN				bIsBinary ; /**< Specifies whether the push header is binary or not. */
	E_COMM_HEADER_TYPE		eHeaderType ; /**< Specifies whether the push header is binary or text. */
	JC_RETCODE				rCode ; /**< Specifies the error in push message processing, in this case push content will not be present. */
} ST_COMM_PUSH_IND ;

/**
 * @brief This structure is used by the communicator to specify the security indication 
 *		  to the application.
 */
typedef struct comm_security_ind
{
	JC_UINT32				uiRequestID ; /**< Specifies the request ID. Used internally for the browser engine */
	JC_BOOLEAN				bIsWTLS ; /**< Specifies whether the security ind is for WTLS or SSL */
#ifdef WAP_WITH_WTLS
	E_WTLS_CIPHER_SUITE		eWTLSCipherSuites ; /**< Specifies the WTLLS Cipher Suite */
	E_WTLS_KEY_EXCHANGE_ALG	eWTLSKeyExAlgs ; /**< Specifies the WTLS key exchange algorithm */
#endif
#ifdef HTTP_WITH_SSL
	E_SSL_CIPHER_SUITE		eSSLCipherSuites ; /**< Specifies the SSL Cipher Suite */
#endif
	JC_BOOLEAN				bIsValid ; /**< Reserved for future purpose */
	JC_UINT32				uiExtendedError ; /**< Specifies an extended error if any while establishing security */
	JC_UINT32				uiTotalCertificate ; /**< Specifies the total number of certificates received */
	JC_UINT32				uiLength ; /**< Specifies the length of the first certificate */
	JC_UINT8				*pucBuffer ; /**< Specifies the content of the first certificate */
} ST_COMM_SECURITY_IND ;

/**
 * @brief This structure is used by the communicator to specify the security class failure indication 
 *		  to the application.
 */
typedef struct comm_security_failure_ind
{
	JC_UINT32					uiRequestID ; /**< Specifies the request ID. Used internally for the browser engine */
	JC_UINT32					uiSecurityError ; /**< Specifies the security error sent from the security layer */
	E_COMM_SECURITY_CLASS		eSecurityClass ; /**< Specifies the security class which has failed */
} ST_COMM_SECURITY_FAILURE_IND ;

/**
 * @brief This structure is used by the communicator to specify the certificate response to the application
 */
typedef struct comm_cert_resp_ind
{
	JC_RETCODE					rCode ; /**< Specifies error if any while retrieving the certificates */
	JC_UINT32					uiRequestID ; /**< Specifies the request ID for which the security session is established and the certificates are in turn received */
	JC_UINT32					uiCertIndex ; /**< Specifies the index of the cert whose response is being sent */
	JC_UINT32					uiLength ; /**< Specifies the length of the certificate */
	JC_UINT8					*pucBuffer ; /**< Specifies the content of the certificate */
} ST_COMM_CERT_RESP_IND ;

/**
 * @brief		Prototype of application callback
 *
 * @param[in]	pvAppArg The application needs to specify the app arguement while initializing
 *				the communicator. The same argument will be passed while calling this application
 *				callback..
 * @param[in]	eCommIndicationType Specifies the callback type that the application needs to invoke or handle.
 * @param[in]	pvIndication Specifies the indication that will be required by the application to handle the callback type.
 * @retval		The callback function can return any application related error.
 *
 * The application needs to implement a callback with the following prototype and pass the address
 * of the function in jdi_CommunicationInitialize. From then on the communicator will call this
 * callback to send any information to the application.
 *
 * The communicator will send many indications to the application. Instead of
 * registering a separate application callback for each communicator indication, the application
 * needs to just register one function with the following prototype. Based on the
 * callback type the application needs to decide which app function needs to be invoked.
 */
 
typedef JC_RETCODE (* CB_COMM_GENERAL_INDICATION) (
							void					*pvAppArg,
							E_COMM_INDICATION		eCommIndicationType,
							void					*pvIndication) ;

/**
 * @brief		Initializes the communicator.
 *
 * @param[in]	pstAppTaskInfo Specifies the task information of the application.
 * @param[in]	pstTransportTaskInfo Specifies the task information of the transport. The 
 *			    communicator needs to use this task info while sending events to transport.
 * @param[in]	pvCallbackArg Specifies the argument that needs to be used when calling the 
 *				application general indication callback.
 * @param[in]	cbIndication Specifies the application general callback indication.
 * @param[in]	vMemHandle Specifies the memory handle to be used by the communicator.
 * @param[out]	pvHandle The communicator returns this handle. From then on the application should use
 *				this handle for using the communicator APIs until the communicator is deinitialized.
 * @retval		JC_OK if the communicator is sucessfully initialized.
 * @retval		JC_ERR_MEMORY_ALLOCATION if a memory error occurs.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the transport initialize request.
 * @see			CB_COMM_GENERAL_INDICATION callback prototype
 * @see			jdi_CommunicationDeinitialize
 *
 * The application needs to use this API to initialize the communicator and inturn initialize
 * the transport. 
 *
 * The application needs to specify its task information. The communicator will send
 * the same to the transport. The transport will send the response events to the queue
 * that is associated with this task information. So the application then needs to decide
 * which transport events it wants to handle (say push) and the ones it does not want to
 * handle can be passed to the communicator.
 *
 * The application can call the initialize the communicator multiple times. Each time
 * the communicator will initialize a separate instance of communicator and different
 * handle will be returned. So the communicator can be used by multiple applications.
 */

JC_RETCODE jdi_CommunicationInitialize (const JC_TASK_INFO 			*pstAppTaskInfo,
										const JC_TASK_INFO			*pstTransportTaskInfo,
										void 						*pvCallbackArg,
										CB_COMM_GENERAL_INDICATION	cbIndication,
										JC_HANDLE					vMemHandle,
										JC_HANDLE 					*pvHandle) ;

/**
 * @brief		Sets the accelerator URL
 * @param[in]	vHandle		Specifies the handle that was returned by the communicator initialize API.
 * @param[in]	pmAcceleratorURL	Specifies the accelerator URL. 
 *
 * If the pmAcceleratorURL is NULL, then the accelerator will be disabled.
 */
JC_RETCODE jdi_CommunicationSetAcceleratorURL (JC_HANDLE			vHandle,
											   JC_CHAR				*pmAcceleratorURL) ;

#ifdef PUNYCODE_ENABLE

/**
 * @brief		Initializes the Punycode converter.
 *
 * @param[in]	vHandle		Specifies the handle that was returned by the communicator initialize API.
 * @param[in]	uiMaxPCURI	Specifies the maximum number of URI that the punycode converter can store.
 * @retval		JC_OK if the Punycode converter is initialized successfully.
 * @retval		JC_ERR_MEMORY_ALLOCATION If memory error occurs.
 *
 * While browsing certain URL's the lower layer expects the URL to be punycode encoded so that the 
 * domain name can be resolved. Else the URL cannot be fetched Successfully. If the browser application is 
 * using the  communicator then it needs to use this API to initialize the punycode converter. 
 *
 * The punycode converter can store the converted URI in a list (to avoid repeated parsing of URI).
 * The application need to specify the limit of this list using the variable uiMaxPCURI.
 *
 * Once initialized the punycode converter will be deinitialized when the communicator is deinitialized.
 */
JC_RETCODE jdi_CommunicationInitPunycode (JC_HANDLE			vHandle,
										  JC_UINT32			uiMaxPCURI) ;

#endif

/**
 * @brief		Deinitializes the communicator.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the communicator initialize API
 * @retval		JC_OK if the communicator is sucessfully deinitialized.
 * @retval		JC_ERR_INVALID_PARAMETER if the handle passed to the API is invalid.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the transport deinitialize request.
 *
 * The application needs to use this API to deinitialize the communicator. The communicator
 * will in turn deinitialize the transport session used by the communicator instance. 
 */

JC_RETCODE jdi_CommunicationDeinitialize (JC_HANDLE		vHandle) ;

/**
 * @brief		Sets the data connection
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstDataConn Specifies the data connection information
 * @retval		JC_OK if the data connection information is successfully added to communicator.
 *
 * The application needs to use this API to set the data connection to be used for the 
 * transport layer operations. The communicator will copy the data connection information and send it when 
 * the set profile is sent to the transport.
 *
 * @note The application needs to use this API before calling the API jdi_CommunicationProfile
 * if it wants the data connection information to be sent to the transport.
 */

JC_RETCODE jdi_CommunicationDataConnection (JC_HANDLE					vHandle,
											const ST_GENERIC_DATA_CONN	*pstDataConn) ;
/**
 * @brief		Sets the profile
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstProfile Specifies the profile information.
 * @retval		JC_OK if the profile information is successfully added to communicator.
 * @see			jdi_CommunicationDisconnect
 *
 * The application needs to use this API to set the profile information to be used for the 
 * transport layer operations. The communicator will copy the profile information and 
 * send it to the transport once the transport is initialized.
 *
 * @note The application has to call the API #jdi_CommunicationDataConnection before setting
 * the profile.
 */

JC_RETCODE jdi_CommunicationProfile (JC_HANDLE 				vHandle,
									 const ST_COMM_PROFILE 	*pstProfile) ;

/**
 * @brief		Sets the extended profile
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstProfile Specifies the extended profile information.
 * @retval		JC_OK if the profile information is successfully added to communicator.
 * @see			jdi_CommunicationDisconnect
 *
 * The application needs to use this API to set the profile information to be used for the 
 * transport layer operations. The communicator will copy the profile information and 
 * send it to the transport once the transport is initialized.
 *
 * @note The data connection information is part of the extended profile information. So the application
 * need not call the function #jdi_CommunicationDataConnection before setting
 * the profile..
 */

JC_RETCODE jdi_CommunicationExtendedProfile (JC_HANDLE 							vHandle,
											 const ST_COMM_EXTENDED_PROFILE 	*pstProfile) ;

/**
 * @brief		Sends the profile response response.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	bProceed Specifies whether the application wants to proceed with the profile information.
 * @retval		JC_OK if the profile confirmation response is successfully sent.
 *
 * If a profile information specified in extended profile fails in the lower layer then it sends
 * profile confirmation request to the application (using #E_PROFILE_CONFIRMATION_INDICATION) to check
 * whether the application wants to proceed with the next option of the extended profile. In order to
 * do that the application should use this API.
 */
JC_RETCODE jdi_CommunicationSendProfileConfirmationResponse (JC_HANDLE			vHandle,
															 JC_BOOLEAN			bProceed) ;

#ifdef IP_STACK_ENABLE

/**
 * @brief		Sets the IP profile
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstIPProfile Specifies the IP profile information.
 * @retval		JC_OK if the profile information is successfully added to communicator.
 * @see			jdi_CommunicationDisconnect
 *
 * The application needs to use this API to set the IP profile information to be used for the 
 * transport layer operations. The communicator will copy the profile information and 
 * send it to the transport once the transport is initialized.
 *
 * @note The application has to call the API jdi_CommunicationDataConnection before setting
 * the profile.
 */
JC_RETCODE jdi_CommunicationIPProfile (JC_HANDLE				vHandle,
									   const ST_COMM_IP_PROFILE	*pstIPProfile) ;

/**
 * @brief		Sends an IP request.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstIPReq Specifies the IP request to be sent.
 * @param[out]	puiIPReqID	The API returns the request ID assigned to the IP request in this parameter.
 * @retval		JC_OK if the IP request is sent successfully.
 *
 * The application needs to use this API to send the IP request to the transport. The content 
 * for the IP request will be sent back in the callback function using #E_FETCH_INDICATION and error
 * if any will be sent to the callback function using #E_ERROR_INDICATION.
 */
JC_RETCODE jdi_CommunicationIPSendReq (JC_HANDLE					vHandle,
									   const ST_COMM_IP_SEND_REQ	*pstIPReq,
									   JC_UINT32					*puiIPReqID) ;

/**
 * @brief		Sends an stop request for the IP session.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @retval		JC_OK if the IP requests are stopped successfully.
 *
 * The application needs to use this API to stop the IP requests getting processed by the transport.
 * Once the requests are stopped, the response will be sent using the stop indication callback #E_STOP_INDICATION.
 */
JC_RETCODE jdi_CommunicationIPStopReq (JC_HANDLE				vHandle) ;

/**
 * @brief		Sends a request to change the security mode used by the IP stack.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	eSecurityMode Specifies the mode to be set.
 * @retval		JC_OK if the IP change security mode request is sent successfully.
 *
 * The application needs to use this API to change the security mode used in the
 * IP stack. The lower layer will stop the current requests and will start processing the
 * next request using the new security mode. By default the security mode is non-secure.
 */
JC_RETCODE jdi_CommunicationIPChangeSecurityMode (JC_HANDLE					vHandle,
												  E_COMM_IP_SECURITY_MODE	eSecurityMode) ;

#endif

/**
 * @brief		Sends the session header.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstSessionHeader Specifies the session header to be added.
 * @retval		JC_OK if the session header is sent to transport layer successfully.
 *
 * The application needs to use this API to send the session header information
 * to the transport layer.
 *
 * If at run time the application needs to add certain headers that should be sent
 * with all the requests, that are being processed by the transport session, then this
 * API is to be used. The session header pstSessionHeaders will be added to 
 * each request that is sent from the transport.
 */

JC_RETCODE jdi_CommunicationSendSessionHeader (JC_HANDLE						vHandle,
											   const ST_COMM_SESSION_HEADER		*pstSessionHeader) ;
/**
 * @brief		Sends the fetch request.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstFetchURL Specifies the fetch URL request structure.
 * @param[out]  puiRequestID Specifies the Unique ID for the request which will be returned by the API.
 * @retval		JC_OK if the fetch request is successfully processed by the communicator.
 * @retval		JC_ERR_MEMORY_ALLOCATION if a memory error occurs.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the fetch request to the transport.
 * @see			jdi_CommunicationGetFetchURL
 * @see			jdi_CommunicationFreeReference
 *
 * The application needs to use this API when it needs to give a fetch request to the communicator.
 * The application needs to specify the fetch request information (URL, post data, authorization)
 * etc in pstFetchURL.
 */

JC_RETCODE jdi_CommunicationFetchURL (JC_HANDLE 				vHandle,
									  const ST_COMM_FETCH_URL	*pstFetchURL,
									  JC_UINT32					*puiRequestID) ;

/**
 * @brief		Clears the authorization information
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	eCommClearAuthHeader Specifies the type of authorization list to be cleared.
 * @retval		JC_OK if the authorization information is cleared successfully.
 *
 * All the authorization information (URI and proxy) that are added by the application will be stored
 * at the lower layer for the lifetime of the communicator session. If the application wants to 
 * clear the authorization information maintained at the lower layers without deinitializing the 
 * session then it needs to use the following API.
 */
JC_RETCODE jdi_CommunicationClearAuthInfo (JC_HANDLE					vHandle,
										   E_COMM_CLEAR_AUTH_HEADER		eCommClearAuthHeader) ;

/**
 * @brief		Gets the fetch URL info of a fetch request.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]   uiRequestID Specifies the ID of the request for which the fetch url Info is to be returned.
 * @retval		ST_COMM_FETCH_URL specifying the fetch URL info of the given request.
 * @retval		JC_NULL if the communicator does not find any fetch URL info for the request ID.
 *
 * If the application only has the ID of a request and wants to know the request information
 * that was passed by the application to the communicator when the jdi_CommunicationFetchURL 
 * was called then this API has to be used. Using this
 * API the application can get information such as the URL, post data etc.
 */

ST_COMM_FETCH_URL * jdi_CommunicationGetFetchURL (JC_HANDLE		vHandle,
												  JC_UINT32		uiRequestID) ;

/**
 * @brief		Frees the reference of a request.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]   uiRequestID Specifies the ID of the request which needs to be freed.
 * @retval		JC_ERR_INVALID_PARAMETER if the request associated with the uiRequestID is not present.
 * @retval		JC_OK if the request reference is freed.
 *
 * For each request the communicator maintains a record in its context (information such
 * as the URL info). The application needs to use this API when it has finished
 * the processing of the request and wants to free the record maintained for the request.
 */

JC_RETCODE jdi_CommunicationFreeReference (JC_HANDLE 	vHandle,
										   JC_UINT32	uiRequestID) ;
/**
 * @brief		Stops a particular request.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]   uiRequestID Specifies the ID of the request which needs to be stopped.
 * @retval		JC_ERR_INVALID_PARAMETER if the request associated to the uiRequestID is not present.
 * @retval		JC_OK if the stop request is sent for the request.
 *
 * The application needs to use this API to stop a particular request. The API sends the 
 * stop request event to the transport which will in turn stop all the transactions related to that
 * request.
 *
 * @note The application cannot assume that once this API is called the request
 * is stopped. The application has to wait for the response of the transport to 
 * confirm whether the request is stopped or not. (Communicator will send 
 * E_STOP_INDICATION to the application callback for stop response).
 */

JC_RETCODE jdi_CommunicationStopReq (JC_HANDLE 			vHandle,
									 JC_UINT32			uiRequestID) ;

/**
 * @brief		Stops all the requests that are currently being processed by the 
 *				communicator session.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @retval		JC_ERR_INVALID_PARAMETER if the request associated to the uiRequestID is not present.
 * @retval		JC_OK if all the fetch requests are stopped successfully.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the stop request to the transport.
 *
 * The application needs to use this API to stop all the requests that are currently being
 * processed by the communicator session. The API will send the stop request to the transport for all the
 * pending request of the communicator.
 *
 * @note The application cannot assume that once this API is called all the requests
 * are stopped. The application has to wait for the response of the transport to 
 * confirm whether the requests are stopped or not. (Communicator will send 
 * E_STOP_INDICATION to the application callback for each request's stop response).
 */

JC_RETCODE jdi_CommunicationStopAllReq (JC_HANDLE 		vHandle) ;

/**
 * @brief		Disconnects the data connection that is currently used.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @retval		JC_OK if the disconnect request is sent.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the disconnect request to the transport.
 *
 * The application needs to use this API to disconnect the data connection that is currently used
 * by transport layer operations. The transport in turn deletes all the requests that are given for
 * processing in that particular data connection.
 */

JC_RETCODE jdi_CommunicationDisconnect (JC_HANDLE 		vHandle) ;

/**
 * @brief		Sets specific communicator options.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstCommSetOption Specifies the Structure which will specify the application defined options.
 * @retval		JC_OK if the set option event is sent
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the set option request to the transport.
 *
 
 * can be set by the application. The application needs to use this API to set
 * such parameters. The various options which can be set is specified in the
 * structure ST_COMM_SET_OPTION.
 */

JC_RETCODE jdi_CommunicationSetOption (JC_HANDLE				vHandle,
									   const ST_COMM_SET_OPTION	*pstCommSetOption) ;

/**
 * @brief		Suspends a fetch request operation.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	uiRequestID Specifies the ID of the request that needs to be suspended.
 * @retval		JC_OK if the suspend fetch request event is sent.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the suspend fetch request to the transport.
 * @see			jdi_CommunicationResumeFetchRequest
 *
 * The application needs to call this API for suspending a fetch request operation. 
 * The communicator will send the suspend request to the transport and transport will
 * in turn suspend all operations with respect to the request.
 *
 * The application needs to use this API when it wants a request to be stopped
 * temporarily. Example: During processing of a request if the application needs user confirmation 
 * then the application can use this API to suspend the request. And once the
 * user confirmation is received jdi_CommunicationResumeFetchRequest can be used
 * to proceed with the fetch.
 */

JC_RETCODE jdi_CommunicationSuspendFetchRequest (JC_HANDLE 			vHandle,
												 JC_UINT32			uiRequestID) ;

/**
 * @brief		Resumes a suspended fetch request operation.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	uiRequestID Specifies the ID of the request that needs to be resumed.
 * @retval		JC_OK if the resume fetch request event is sent.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the resume fetch request to the transport.
 * @see			jdi_CommunicationSuspendFetchRequest
 *
 * The application needs to call this API when it needs to resume a suspended fetch request operation. 
 * The communicator will send the resume request to the transport and transport will
 * in turn resume the suspended request.
 */

JC_RETCODE jdi_CommunicationResumeFetchRequest (JC_HANDLE 			vHandle,
												JC_UINT32			uiRequestID) ;

/**
 * @brief		Suspends the communicator session.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @retval		JC_OK if the suspend session event is sent.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the suspend session to the transport.
 * @see			jdi_CommunicationResumeSessionRequest
 *
 * The application needs to use this API for suspending a communicator session. After
 * suspending the session the lower layers will not stop the functioning
 * of the session until the resume session API is called.
 *
 * The application needs to call this API when it wants the functioning of the
 * communicator session to be stopped temporarily. Example: If a particular session 
 * has to be stopped coz another application needs to use the resources. Then the
 * API jdi_CommunicationResumeSessionRequest to resume the session.
 */

JC_RETCODE jdi_CommunicationSuspendSessionRequest (JC_HANDLE 			vHandle) ;

/**
 * @brief		Resumes a suspended communicator session.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @retval		JC_OK if the resume session event is sent.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the resume session to the transport.
 * @see			jdi_CommunicationSuspendSessionRequest
 *
 * The application needs to use this API to resume a suspended session. The communicator will
 * send a request to the transport and the transport will resume the session at transport and the lower layers.
 */

JC_RETCODE jdi_CommunicationResumeSessionRequest (JC_HANDLE 			vHandle) ;

/**
 * @brief		Adds a root certificate.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstRootCert Specifies the root certificate to be added.
 * @retval		JC_OK if the add root certificate is sent or the root certificate is added to communicator.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the add root certificate event to the transport.
 * @retval		JC_ERR_MEMORY_ALLOCATION if a memory allocation error occurs.
 * @note		This API should be called if the stack certificate and security is enable
 *				by the application.
 * @see			jdi_CommunicationClearRootCertificate
 *
 * The application needs to call this API to add the root certificate at the lower layers. The API
 * sends the add root certificate event to the transport if the communicator is idle. If not communicator
 * will store the certificate and add it when it becomes idle.
 */

JC_RETCODE jdi_CommunicationAddRootCertificate (JC_HANDLE				vHandle,
												const ST_COMM_ROOT_CERT	*pstRootCert) ;

/**
 * @brief		Adds an user certificate.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstUserCert Specifies the user certificate to be added.
 * @retval		JC_OK if the add user certificate is sent or the user certificate is added to communicator.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the add user certificate event to the transport.
 * @retval		JC_ERR_MEMORY_ALLOCATION if a memory allocation error occurs.
 * @note		This API should be called if the stack certificate and security is enabled
 *				by the application.
 * @see			jdi_CommunicationClearUserCertificate
 *
 * The application needs to call this API to add the user certificate at the lower layers. The API
 * sends the add user certificate event to the transport if the communicator is idle. If not communicator
 * will store the certificate and add it when it becomes idle.
 */

JC_RETCODE jdi_CommunicationAddUserCertificate (JC_HANDLE 					vHandle,
												const ST_COMM_USER_CERT		*pstUserCert) ;

/**
 * @brief		Clears root certificates.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @retval		JC_OK if the clear root certificate is sent.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the clear root certificate event to the transport.
 *
 * The application needs to call this API to clear root certificates. The API
 * sends the clear root certificate event to the transport.
 *
 * @note Once this API is called all the root certificates that are maintained in the
 * transport are cleared.
 */

JC_RETCODE jdi_CommunicationClearRootCertificate (JC_HANDLE 		vHandle) ;

/**
 * @brief		Clears user certificates.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @retval		JC_OK if the clear user certificate is sent.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the clear user certificate event to the transport.
 *
 * The application needs to call this API to clear user certificates. The API
 * sends the clear user certificate event to the transport.
 *
 * @note Once the API is called all the user certificates that are maintained in the 
 * transport are cleared.
 */

JC_RETCODE jdi_CommunicationClearUserCertificate (JC_HANDLE 		vHandle) ;

/**
 * @brief		Retrieves Certificate information
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	uiRequestID Specifies the request ID for which the security session is established and the certificates are in turn received (or to be received) 
 * @param[in]	uiCertIndex Specifies the index of the certificate in the cert list.
 * @retval		JC_OK if the request for the certificate is sent successfully.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending the certificate request.
 *
 * When security is established the lower layer may get a set of certificates (which gives details
 * about the security). Lower layer forms a cert list and keep these certificates in the list.
 * In the security indication (#ST_COMM_SECURITY_IND) the application will receive only the first 
 * certificate of the cert list and it will also specify the number of certificates that are present
 * in the cert list. The application can then use this API to retrieve the rest of the certificate
 * one by one.
 */
JC_RETCODE jdi_CommunicationGetCertInfo (JC_HANDLE					vHandle,
										 JC_UINT32					uiRequestID,
										 JC_UINT32					uiCertIndex) ;

#ifdef TR_PUSH

/**
 * @brief		Sends a push session initialize request.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstCommPushInit Specifies the Push Session Information (push type, Server Address, port).
 * @retval		JC_OK if the push initialize request is sent to transport.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending push initialize event to the transport.
 * @note		Define TR_PUSH to use jdi_BESendPushInitReq.
 * @see			jdi_CommunicationSendPushDeinitReq
 *
 * The application needs to use this API when it needs to initialize a push session. 
 * 
 * Example: If a SIA message is received the application will have to initialize 
 * a new push session. In that case this API used. The application can initialize
 * HTTP and Unconfirmed push session only.
 */

JC_RETCODE jdi_CommunicationSendPushInitReq (JC_HANDLE			vHandle,
											 ST_COMM_PUSH_INIT	*pstCommPushInit) ;

/**
 * @brief		Sends a push session deinitialize request.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	uiSessionID Specifies the ID of the push Session that needs to be deinitialized.
 * @retval		JC_OK if the push deinitialize request is sent to transport.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending push deinitialize event to the transport.
 * @note		Define TR_PUSH to use jdi_CommunicationSendPushDeinitReq.
 *
 * The application needs to use this API when it needs to deinitialize a push session. 
 * The application needs to specify the ID of the push session to be deinitialized. 
 * If the ID is zero then all the push sessions will be deinitialized.
 */

JC_RETCODE jdi_CommunicationSendPushDeinitReq (JC_HANDLE		vHandle,
											   JC_UINT32		uiSessionID) ;

/**
 * @brief		Sends a push register request.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstPushRegisterInfo Specifies the information about the type of push to be registered.
 * @retval		JC_OK if the push register request is sent successfully.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending push register request event to the transport.
 * @note		Define TR_PUSH to use jdi_CommunicationRegisterForPushContent.
 *
 * The application needs to use this API to receive only specific kind of push content.
 * 
 * That is the application can specify the ID or the content type of the push content that it wants to recieve.
 * Once registered the transport will send only registered push content to the applicaton.
 */

JC_RETCODE jdi_CommunicationRegisterForPushContent (JC_HANDLE					vHandle,
													ST_COMM_PUSH_REGISTER_INFO	*pstPushRegisterInfo) ;

#endif

/**
 * @brief		Sends the security indication response.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	bIsSecurityParamAccepted Specifies whether the application has accepted the security to be used.
 * @retval		JC_OK if the security indication response is sent to transport.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending security indication response event to the transport.
 *
 * Whenever the lower layers establishes security the transport sends the security parameters (certificates,
 * key exchange alg, cipher suites etc) to the communicator. The communicator in turn sends this information
 * to the application (using E_SECURITY_INDICATION in the application callback). 
 *
 * Once the application has verified the security parameters it needs to  use this API
 * to specify whether it wants to go ahead with the security provided. 
 *
 * @note Until the security indication response is sent the transport will not allow any operation
 * to be performed on the secured connection.
 */

JC_RETCODE jdi_CommunicationSecurityIndResponse (JC_HANDLE		vHandle,
												 JC_BOOLEAN		bIsSecurityParamAccepted) ;

/**
 * @brief		Sends the security failure response.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	bSetNextSecurityClass Specifies whether the application wants to proceed with the next class of security.
 * @retval		JC_OK if the security failure response is sent to transport.
 * @retval		JC_ERR_QUEUE_SEND if error occured while sending security failure response event to the transport.
 *
 * In the WSP secured mode there are 3 classes of security. Class III (if User and root certs present), 
 * class II (if root cert present) and class I (default).  Transport will try to set the security in 
 * descending order of class. 
 *
 * If class III or class II fails then transport sends failure indication (using 
 * E_SECURITY_FAILURE_INDICATION in the application callback) specifying the 
 * class for which the security has failed. 
 *
 * The application needs to use this API to specify whether it wants to go for the 
 * next class of security.
 *
 * @note Until this API is called the transport will not try to set the next level of security.
 * @note The communicator will send the security failure response only for class III
 * and class II. For class I error indication will be sent (E_ERROR_INDICATION).
 */

JC_RETCODE jdi_CommunicationSendSecurityFailureResponse (JC_HANDLE		vHandle,
														 JC_BOOLEAN		bSetNextSecurityClass) ;

/**
 * @brief		Used to handle the transport events.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstEventData Specifies the event data.
 * @retval		JC_OK if the transport event is handled sucessfully.
 *
 * The transport will send all the requests and responses to the application task info which is sepcified
 * during the communicator initialization. So when the application receives the transport it
 * needs to decide whether it wants the communicator to handle the event. If yes
 * the the application needs to call this API. 
 */

JC_RETCODE jdi_CommunicationEventHandler (JC_HANDLE			vHandle,
										  JC_EVENT_DATA		*pstEventData) ;

/**
 * @brief Suspends the communicator.
 * @param[in] vHandle	Specifies the handle returned by the jdi_CommunicationInitialize 
 *						function.
 * @retval	void
 *
 * For asynchronous callbacks to the application, while waiting for user's response, the
 * communicator can be suspended by using this API.
 */
void jdi_CommunicationSuspend (JC_HANDLE 		vHandle) ;

/**
 * @brief Resumes the communicator.
 * @param[in] vHandle	Specifies the handle returned by the jdi_CommunicationInitialize 
 *						function.
 * @param[in] rRespCode	Specifies the response returned from the Application.
 * @retval	JC_OK	If communicator is successfully resumed.
 * @retval	Others	Other errors occuring while resuming the communicatior.
 *
 * In case the communicator has been suspended, the same can be resumed using this API.
 */
JC_RETCODE jdi_CommunicationResume (JC_HANDLE 		vHandle,
									JC_RETCODE		rRespCode) ;

#ifdef IP_STACK_ENABLE

/**
 * @brief This error is returned to transport from the application if all the content for the 
 *		  current IP request is received successfully.
 *		  
 */
#define JC_ERR_COMM_IP_REQ_COMPLETED		TERMINAL_ERR_CODE(COMMUNICATOR_MODULE, 0x01)

#endif

#ifdef __cplusplus
}
#endif

#endif
/* End of File */


