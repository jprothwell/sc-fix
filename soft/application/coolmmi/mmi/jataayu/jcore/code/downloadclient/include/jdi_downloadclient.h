/***************************************************************************
 * $Id: jdi_downloadclient.h,v 1.24 2009/03/04 13:04:13 kavithadevhtmlbrow Exp $
 * $Revision: 1.24 $
 * $Date: 2009/03/04 13:04:13 $
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
 * $Log: jdi_downloadclient.h,v $
 * Revision 1.24  2009/03/04 13:04:13  kavithadevhtmlbrow
 * Doxygen Comments Updated
 *
 * Revision 1.23  2008/01/31 11:22:54  arpitadevhtmlbrow
 * Added error code for HTTP errors
 *
 * Revision 1.22  2007/11/23 07:13:01  kumardevhtmlbrow
 * Lint & Documentation update.
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_downloadclient.h
 * @ingroup DC
 * @brief Defines the exposed enumerations, structures and functions of the download client that
 *		  can be used by the application.
 * 
 * The Download Client module is used by the application for providing a consistent interface for
 * content download functionality. An interface function is required for any of the modules to 
 * communicate with the Download Client. 
 */

/**
 * @mainpage
 *
 * @version 2.1
 * Download Client Description
 *
 * @section Introduction Introduction to Download Client
 * <p>The <i>Open Mobile Alliance</i> has defined a set of application level protocols and behaviors needed
 * for providing both powerful as well as reliable content download functionality. </p><p> The Download Client
 * follows these specifications for providing a consistent download interface and functionality for all
 * generic content types, by addressing requirements such as Delivery Negotiation, Content Delivery and
 * Delivery Confirmation.</p>
 *
 * @section Need Need for Download Client
 * <p>With the advent of e-commerce there arose a need for Internet style, media-type independent,
 * download of <i>Media Object</i>. The Download Client aims at addressing this requirement.</p>
 * <p>The provision of such a component allows for various billing transactions, thus making premium
 * priced content available to the mobile users.</p>
 * <p>Download Client addresses the need for an implementation that enables confirmed and reliable, thus
 * billable transactions between a server entity and a client device.</p>
 *
 * @section Objectives Objectives of Download Client
 * @li Provide support for various types of payment models.
 * @li Avoid fragmentation of content space within the mobile device.
 * @li Have a similarity between the download process of all types of media.
 * @li Have client driven capability negotiation.
 * @li Provide a means for confirmed download for entertainment or business based applications as well as
 *	   objects to be rendered onto a mobile phone.
 *
 * @section Definitions Definitions and Terminology
 * @li <b>Content Delivery</b> - The actual delivery of the <i>Media Object</i>, to the client device.
 * @li <b>Content Download</b> - The whole transaction including discovery, delivery of content and confirmation of
 *	   download.
 * @li <b>Content Handler</b> - An entity in the mobile device responsible for the processing of a particular media type.
 * @li <b>Content Storage</b> - The physical location of the media object to be downloaded.
 * @li <b>Download Descriptor</b> - Metadata about a <i>media object</i> and instructions to the <i>download agent</i>
 *	   on how to download it. The object triggers the <i>Download Agent</i> on the client device. It describes the <i>media
 *	   object</i> to be downloaded and allows the client device to decide if it has the capabilities to install and render/
 *	   execute the <i>Media Object</i>.
 * @li <b>Download Server</b> - A Web server hosting <i>media objects</i> available for download.
 * @li <b>Download User Agent</b> - A user agent in the device responsible for downloading a <i>media object</i> described
 *	   by a <i>download descriptor</i>. It is triggered by the reception or activation of a <i>download descriptor</i>.
 * @li <b>Installation Notification</b> - A <i>Status Report</i> message from the client to the server. It indicates
 *	   to the server that the <i>download agent</i> has successfully installed the <i>media object</i>, and that the content
 *	   (to the best knowledge of the <i>download agent</i>) would be made available to the user.
 * @li <b>Media Object</b> - A resource on the Web server that can be downloaded. It may be a single object, or a container
 *	   consisting of multiple objects. The download of a <i>media object</i> is the ultimate goal of each transaction
 *	   undertaken with the defined protocol.
 * @li <b>Media Type</b> - A MIME media type [RFC2046].
 * @li <b>Status Report</b> - A message sent from the mobile device to a server to indicate the positive or negative
 *	   outcome of a download transaction. In the context of Content Download, the <i>status report</i> terminates the
 *	   "download session" (or "download transaction").
 *
 * @section Download OMA Download
 * The generic OMA Download, based upon the concept of a <i>download descriptor</i>, includes to basic scenarios :
 * @li OMA Download with Separate Delivery of <i>download descriptor</i> and <i>media object</i>.
 * @li OMA Download with Combined Delivery of <i>download descriptor</i> and <i>media object</i>.
 * In the two scenarios there may or may not be an <i>installation notification</i>, depending upon what was requested
 * in the <i>download descriptor</i>.
 * Few points to be noted in the OMA Download are :
 * @li During the download and installation process, the user should be given opportunities to control the download.
 * @li The user should be informed of the progress of the download transaction and be given an opportunity to
 *	   cancel the activity.
 * @li When the <i>Media Object</i> is installed, and if <i>Installation Notification</i> has been requested in the
 *	   <i>Download Descriptor</i>, a confirmation must be sent to the server to indicate that the installation has been
 *	   completed.
 * @li If an <i>installNotifyURI</i> has been defined in the <i>download descriptor</i>, then errors during the
 *	   download process must be reported using the <i>status report</i> mechanism.
 *
 * @section Process OMA Download Process
 * <p>Object Discovery Process :
 * @li <b>Step 1 - </b>The <i>download descriptor</i> is transferred to the device.</p>
 * <p>Object Installation Process :
 * @li <b>Step 2 - </b>The <i>download user agent</i> is launched, the <i>download descriptor</i> is processed.
 * @li <b>Step 3 - </b>Memory Check is performed for verifying whether sufficient memory is available on the
 * the device for the storage of the <i>media object</i>.
 * @li <b>Step 4 - </b>Capability Check is performed for ascertaining whether the device is capable of using and/or
 *	   rendering the <i>media object</i>. This check is based upon the MIME type of the <i>media object</i> received
 *	   in the <i>download descriptor</i>.
 * @li <b>Step 5 - </b>User Confirmation. Using the information provided in the <i>download descriptor</i>, the
 *	   user is given a chance to confirm whether they want to install the <i>media object</i>.
 * @li <b>Step 6 - </b>The retrieval of the <i>media object</i> is typically performed using HTTP (or HTTPS)
 *	   but always according to the scheme in the <i>objectURI</i> attribute of the <i>download descriptor</i>. In case
 *	   of <i>Combined Delivery</i> mode, the <i>media object</i> is retrieved from the buffer containing the
 *	   <i>download descriptor</i>.
 * @li <b>Step 7 - </b>Installation. It is media type and implementation specific. This mechanism prepares the
 *	   <i>media object</i> for rendering/execution on the device. It is dependant upon whether the <i>Installation
 *	   Notification</i> has been requested or not.
 * @li <b>Step 8 - </b>Sending Installation Notification. Valid only if the <i>installNotifyURI</i> attribute
 *	   is present in the <i>download descriptor</i>. It provides the <i>download server</i> with an indication that
 *	   the <i>media object</i> has been properly received and installed.
 * @li <b>Step 9 - </b>Download Confirmation and next step. When the device has sent the <i>status report</i> or,
 *	   the <i>download transaction</i> has completed without a status report, then the <i>download agent</i> should
 *	   indicate the result of the transaction to the user. Also, the <i>download agent</i> should invoke the URL
 *	   defined in the <i>nextURL</i> attribute if it is present in the <i>download descriptor</i>.</p>
 *
 * @section Codes Installation Status Codes
 * The <i>Status Report</i> must contain the Status Code in a decimal format, followed by one or more space
 * characters, followed by the textual representation of the error code.
 *
 * @section Descriptor Download Descriptor
 * <p>The <i>download descriptor</i> is a collection of attributes, used to describe a <i>media object</i>.
 * These allow the <i>download agent</i> to identify, retrieve and install the <i>media object</i>.</p>
 * <p>It can also be used by the content handler for the actual processing of the <i>media object</i>, the
 * <i>download descriptor</i> may contain media specific attributes. Thus all attributes appearing in the
 * <i>download descriptor</i> should be made available to the content handler of the media type that the
 * <i>download descriptor</i> references.</p>
 * <p>A predefined set of attributes is specified to allow the device to verify whether the desired <i>media
 * object</i> is suitable for the device before being loaded.</p>
 * <p>The <i>download descriptor</i> content types can be of the following types :
 * @li <i>application/vnd.oma.dd+xml</i> (Separate delivery)
 * @li <i>application/vnd.wap.multipart.related</i> (Combined delivery)
 * @li All the <i>multipart</i> content types with the type specified as <i>application/vnd.oma.dd+xml</i>
 * (Combined delivery)
 *
 * @section Attributes Download Descriptor Attributes
 * @li <b>type - </b>Indicates the MIME media type of the object to be installed. It may occur multiple times.
 * It is used to indicate to the client if the media object to be downloaded has a media type that is supported
 * by the client. It is mandatory both for the download descriptor, as well as the download agent to have
 * and process this attribute.
 * @li <b>size - </b>Specifies the number of bytes to be downloaded from the URI. The size can be used to allocate
 * sufficiently large buffers for downloading in the client. It is a mandatory attribute to have and process for the
 * download descriptor and the download agent respectively.
 * @li <b>objectURI - </b>The URI from which the <i>media object</i> is to be downloaded. Again this is a
 * mandatory attribute for both the download descriptor and the download agent.
 * @li <b>installNotifyURI - </b>The URI to which the installation status report needs to be sent, either in case
 * of success or in the case of failure. Its presence in the <i>download descriptor</i> is optional, but, if
 * present, then its mandatory for the <i>download agent</i> to send the status report to this URI. If this URI
 * is not present, then no status report needs to be sent, either in case of success or in case of failure. The
 * <i>download agent</i> posts a status report to this URI.
 * @li <b>nextURL - </b>This specifies the URL to which the client should navigate to, once the download is
 * through. Its presence in the <i>download descriptor</i> and its processing by the <i>download agent</i>
 * are both optional.
 * @li <b>DDVersion - </b>Indicates the version of the <i>download descriptor</i> technology. The format of the
 * version is "major.minor". A <i>download agent</i> not supporting the major version must send an "Invalid DD
 * version" status report to the <i>installNotifyURI</i> and abort the download operation. The default DD version
 * when the attribute is missing from the <i>download descriptor</i> is "1.0". Its presence in the <i>download
 * descriptor</i> and its processing by the <i>download agent</i> are both optional.
 * @li <b>name - </b>Specifies a user readable name for the <i>media object</i> that identifies the object to the
 * user. The client may use that name as the default storage name. Its presence in the <i>download descriptor</i>
 * and its processing by the <i>download agent</i> are both optional.
 * @li <b>description - </b>Specifies a short textual description for the <i>media object</i>. It should be displayed
 * to the end user before the download of the <i>media object</i> is accepted by the user. Its presence in the
 * <i>download descriptor</i> and its processing by the <i>download agent</i> are both optional.
 
 * be displayed to the end user before the download. The attribute may be used by the <i>download agent</i> to
 * create a unique name for the <i>media object</i> when it is stored. Its presence in the <i>download descriptor</i>
 * and its processing by the <i>download agent</i> are both optional.
 * @li <b>infoURL - </b>The <i>infoURL</i> is used for obtaining further information about the media object which may
 * be displayed to the end user. Its presence in the <i>download descriptor</i> and its processing by the
 * <i>download agent</i> are both optional.
 * @li <b>iconURI - </b>This may be used by the client to retrieve an icon for representing the <i>media object</i>
 * in the client's user interface. Its presence in the <i>download descriptor</i> and its processing by
 * the <i>download agent</i> are both optional.
 * @li <b>installParam - </b>Indicates an installation parameter associated with the download of the <i>media
 * object</i>. The value is handed by the <i>download agent</i> to the media object installer. Its presence in
 * the <i>download descriptor</i> and its processing by the <i>download agent</i> are both optional.
 *
 * @section Interface Download Client Interface description
 *
 * This documentation gives a detailed description of the interface that has to be used
 * by the application. The documentation clearly describes the usage of each function, the 
 * significance of each variable, usage of the data structures and enumerations etc. The
 * document will ease the complexity faced while interfacing the application and the Download Client.
 *
 * @section Conventions Naming Conventions
 * Following specifies the naming convention used in the code: 
 * @li Names of all the functions, exposed to the application, starts with "jdi_". 
 * @li Names of all the structures starts with "ST_". 
 * @li Names of all the enumerations and its members starts with "E_". 
 * @li Names of all the unions starts with "U_". 
 * @li Names of all the error code that are specific to Download client starts with "JC_ERR_DC_". 
 *
 * @section Startup Startup Code
 * @li The application has to initialize all the JDD modules (Net, timer, MMI etc.) 
 *	   that are going to be used by the download client.
 * @li Then the application needs to start the transport task. This will ensure that the queue
 *	   for the transport is created. 
 * @li Then the queue for the application has to be created. 
 *
 * @section Flow Basic Flow
 * This section describes the basic flow that has to be used in order to download a Media Object.
 * @li The download client @link jdi_downloadclient.h.jdi_DCInitialize initialize function @endlink
 *	   has to be called before calling any download client related functions. 
 * @li Upon receiving a content of download descriptor type, the application sends this data for 
 *	   @link jdi_downloadclient.h.jdi_DCProcessContent further processing @endlink to the download client.
 
 *	   combined or separate delivery, depending upon the content type of the download descriptor. A multipart
 *	   content type (<i>application/vnd.wap.multipart.related</i> or any of the <i>multipart/</i> content types
 *	   with type specified as <i>application/vnd.oma.dd+xml</i>) indicates that the Media Object is present along with the
 *	   download descriptor buffer. Whereas, a general download descriptor content type (<i>application/vnd.oma.dd+xml</i>)
 *	   indicates that a fetch request would have to be made for obtaining the Media Object as the Media
 *	   Object is not contained along with the received download descriptor buffer.
 * @li After successfully parsing the download descriptor data, the above mentioned #jdi_DCProcessContent function,
 *	   makes calls to the application for checking the memory and capability of the device, for downloading the
 *	   Media Object. In case sufficient memory is present for storing the Media Object on the device, and,
 *	   the device is also capable to handle/render the Media Object to be downloaded, the Download Client then
 *	   sends a fetch request to the application for the Media Object. The handle for that particular download session
 *	   is also returned to the application. The application then uses this particular handle for making further 
 *	   calls to the Download Client related to that particular download transaction.
 * @li Depending upon the buffer received by the application, in response to a previously made fetch
 *	   request for downloading the Media Object, the application then makes appropriate calls to
 *	   the various download client functions for processing the received response,@link jdi_downloadclient.h.jdi_DCSetEvent
 *	   by sending the appropriate event type @endlink to the download client. Thus the header
 *	   and the response buffer received during the download transaction are processed by the download client.
 * @li Finally, the application has to @link jdi_downloadclient.h.jdi_DCDeInitialize deinitialize @endlink
 *	   the download client by making a call for the same. 
 *
 * The below diagram describes the basic successful flow diagram:
 * @image html DL-Flow.jpg
 *
 * @section Integration Integration with jBrowser5
 * The Download Client module is an independent and generic module that can be integrated with any
 * application for providing a consistent interface for content download functionality. 
 * This section provides the required information for integrating the Download Client module with
 * jBrowser5.
 * @li The Application needs to implement a @link jdi_downloadclient.h.CB_DC_CALLBACK callback @endlink,
 *     for the Download Client to pass any indication to the application. This callback is registered at the time
 *     of initialization of Download Client itself. The various indication types that can be sent to the 
 *	   Application, through the callback, have been enumerated in #E_DC_CALLBACK_TYPE.
 * @li Upon receiving an indication from the Download Client, the Application then processes the data accordingly
 *	   and sends the response back to the Download Client by calling the function @link 
 *	   jdi_downloadclient.h.jdi_DCSetEvent jdi_DCSetEvent @endlink, with an appropriate event type. The event types
 *	   have been enumerated in #E_DC_EVENT_TYPE.
 * @li For sending the header data, in a download transaction, the Application sets the E_DC_FETCH_HEADER_INFO event
 *	   and sends the header data in the #ST_DC_HEADER_IND structure, by making a call to the #jdi_DCSetEvent function.
 * @li For sending the response information, the Application sets the E_DC_FETCH_INDICATION_INFO event and sends the 
 *	   response buffer in the #ST_DC_FETCH_IND structure, by making a call to the #jdi_DCSetEvent function.
 * @li In case an error occurs during a download transaction, the application can send the error information to the
 *	   Download Client by making a call to the #jdi_DCSetEvent function with the evet type set to E_DC_FETCH_ERROR_INFO.
 *	   The error information can be sent through the structure #ST_DC_ERR_IND.
 * @li When the Application needs to intimate the Download Client to send the status of the download transaction to the
 *	   server, it sets the event E_DC_APPLICATION_RESPONSE and sends the status to the Download Client through the
 *	   #ST_DC_APPLICATION_RESP structure.
 * @li If the download of some Media Object requires authorization details, the Application sends the authorization details
 *	   through the structure #ST_DC_AUTHORIZATION_HANDLER. The event type set is E_DC_AUTHORIZATION_RESPONSE.
 * @li For sending the memory status of the device, for the download of the Media Object, the event type set is
 *	   E_DC_MEMORY_INDICATION. The status is returned through the #ST_DC_MEMORY_RESP structure.
 * @li The device's capability, to handle/render the Media Object, is intimidated to the Download Client by setting the
 *	   event E_DC_CAPABILITY_INDICATION. The return code for the capability indication is sent through the structure
 *	   #ST_DC_CAPABILITY_RESP.
 * @li To send the User's response, for either downloading or canceling the download of the Media Object, the Application
 *	   can set the event as E_DC_USER_INDICATION and send the response information through the #ST_DC_USER_RESP structure.
 * @li When the Download Client wishes to send a fetch request for an object, it makes a callback to the application
 *	   with the type set as E_DC_FETCH_REQUEST. The data required for the fetch request is sent through the structure
 *	   #ST_DC_FETCH_REQUEST. The Application in turn makes a call to the Browser Engine through the jdi_BEDownload function for
 *	   downloading the Media Object.
 * @li To intimate the Application to obtain the User's response for either proceeding with the download of the Media Object,
 *	   or canceling the transaction, the Download Client makes a callback to the Application with the type set as E_DC_USER_CONFIRMATION.
 *	   The information required for obtaining the response from the User is sent through the structure #ST_DC_USER_CONFIRMATION.
 * @li To intimate the Application to notify the user about the status of the download transaction, the Download Client calls the 
 *	   Application callback with the type set as E_DC_NOTIFY_USER. The information is sent through the structure #ST_DC_NOTIFY_USER.
 * @li When authorization data is requested before proceeding with the download of some Media Object, E_DC_AUTHORIZATION_HANDLER
 *	   event type is set in the Application callback.
 * @li For obtaining the memory status of the device to store the Media Object to be downloaded, the Download Client calls the
 *	   Application callback with the event set to E_DC_MEMORY_CHECK. The size of the Media Object to be downloaded is sent through the
 *	   structure #ST_DC_MEMORY_IND.
 * @li For ascertaining the capability of the device to be able to render the Media Object, E_DC_CAPABILITY_CHECK is set while making
 *	   the callback. The MIME type of the Media Object to be downloaded is sent to the Application through the structure #ST_DC_CAPABILITY_IND.
 * @li E_DC_HEADER_INDICATION callback type is set to send the header data of the downloaded Media Object to the Application for further
 *	   processing of the same by the Application. The data is sent through the structure #ST_DC_HEADER_IND.
 * @li E_DC_RESPONSE_INDICATION callback type is set to send the response buffer for the downloaded Media Object to the Application.
 *	   The data is sent through the structure #ST_DC_FETCH_IND.
 * @li E_DC_ERROR_INDICATION callback type is set to send any error information to the Application, in case of an error occurring during
 *	   a download transaction. The error information is sent through the structure #ST_DC_FETCH_ERROR_INFO.
 * @li The Media Object's installation status code is sent by the Download Client to the application by setting the event as 
 *	   E_DC_OBJECT_INSTALLATION_STATUS. The status of the installation is sent through the structure #ST_DC_INSTALLATION_STATUS.
 *
 */


#ifndef _JDI_DOWNLOADCLIENT_H
#define _JDI_DOWNLOADCLIENT_H


#ifdef __cplusplus
extern "C" {
#endif

#ifdef DOWNLOAD_CLIENT_ENABLE

//#pragma message ("Download Client V1 Enabled")

/** @cond */
#include <jdi_dcmodule.h>
#include <jdi_mimeinfo.h>
#include <jdi_httpstatus.h>
/** @endcond */

/**
 * @brief Specifies error code for an unsupported download descriptor type 
 */
#define JC_ERR_DC_UNSUPPORTED_TYPE			TERMINAL_ERR_CODE(DOWNLOAD_CLIENT_MODULE, 0x01)

/**
 * @brief Specifies the error code in case an invalid download descriptor is received
 */
#define JC_ERR_DC_INVALID_DD				TERMINAL_ERR_CODE(DOWNLOAD_CLIENT_MODULE, 0x02)

/**
 * @brief Specifies the error code in case the application is not capable to 
 *		  handle/render the type of the media object to be downloaded
 */
#define JC_ERR_DC_CAPABILITY_ERROR			TERMINAL_ERR_CODE(DOWNLOAD_CLIENT_MODULE, 0x03)

/**
 * @brief Specifies the code which can be returned by the application in case of
 *		  a non-blocking call.
 */
#define JC_ERR_DC_CONTINUE					TERMINAL_ERR_CODE(DOWNLOAD_CLIENT_MODULE, 0x04)

/**
 * @brief Depending upon the status code of the download transaction, a corresponding
 *		  error message is generated. In case the received status code is unknown, this
 *		  error code is rturned
 */
#define JC_ERR_DC_INVALID_STATUS_CODE		TERMINAL_ERR_CODE(DOWNLOAD_CLIENT_MODULE, 0x05)

/**
 * @brief Error code signifying low memory on the device to carry out the download of the
 *		  Media Object
 */
#define JC_ERR_DC_LOW_MEMORY				TERMINAL_ERR_CODE(DOWNLOAD_CLIENT_MODULE, 0x06)

/**
 * @brief This error code signifies that the user authentication, required for fetching
 *		  the media object, has failed, even after the specified number of tries
 */
#define JC_ERR_DC_AUTHENTICATION			TERMINAL_ERR_CODE(DOWNLOAD_CLIENT_MODULE, 0x07)

/**
 * @brief Signifies an error occuring for sending a notification to the download server
 */
#define JC_ERR_DC_NOTIFICATION_FAILURE		TERMINAL_ERR_CODE(DOWNLOAD_CLIENT_MODULE, 0x08)

/**
 * @brief Specifies the error code in case an invalid download descriptor version is present
 */
#define JC_ERR_DC_UNSUPPORTED_DD_VERSION	TERMINAL_ERR_CODE(DOWNLOAD_CLIENT_MODULE, 0x09)

/**
 * @brief Specifies the error code in case of an HTTP error during download
 */
#define JC_ERR_DC_HTTP_STATUS	TERMINAL_ERR_CODE(DOWNLOAD_CLIENT_MODULE, 0x0A)

/**
 * @brief Specifies the content type for download descriptor 
 */
#define APPLICATION_VND_OMA_DD		"application/vnd.oma.dd+xml"

/**
 * @brief Specifies the maximum size for the name used for authorization
 */
#define MAX_DC_AUTH_NAME_SIZE		255

/**
 * @brief Specifies the maximum size for the domain name used for authorization
 */
#define MAX_DC_AUTH_DOMAIN_SIZE		255

/**
 * @brief Specifies the maximum size for the realm used for authorization
 */
#define MAX_DC_AUTH_REALM_SIZE		255

/**
 * @brief Specifies the maximum size for the nonce used for authorization
 */
#define MAX_DC_AUTH_NONCE_SIZE		255

/**
 * @brief Specifies the maximum size for the password used for authorization
 */
#define MAX_DC_AUTH_PASSWORD_SIZE	255

/** 
 * @brief This enumeration specifies the responses which can be returned by the
 *		  user/application to the Download Client. Based upon the response, the 
 *		  Download Client module either proceeds with the download of the Media
 *		  Object or sends a cancellation notification.
 */
typedef enum _dc_user_response
{
	E_DC_PROCEED, /**< Signifies that the user has sent an affirmative response for downloading the Media Object */
	E_DC_CANCELED /**< Signifies that the user has cancelled the download of the Media Object */
} E_DC_USER_RESPONSE ;

/** 
 * @brief This enumeration specifies the types of notification messages that
 *		  can be sent to the application from the Download Client module. 
 */
typedef enum _dc_msg_type
{
	E_DC_INFORMATION, /**< Indicates that the message being sent to the application is of an informative nature */
	E_DC_ERROR /**< Indicates that the message sent to the application is an error message */
} E_DC_MESSAGE_TYPE ;

/**
 * @brief The enumeration contains the different types of authorization that the application 
 *		  needs to specify to the download client when sending any authorization information.
 */
typedef enum _dc_auth_type
{
	E_DC_AUTHORIZATION_BASIC, /**< Specifies that the authorization is of type basic */
	E_DC_AUTHORIZATION_DIGEST /**< Specifies that the authorization is of type digest */
} E_DC_AUTHORIZATION_TYPE ;

/**
 * @brief The enumeration contains the error types that the application will send to
 *		  Download Client error handler function.
 */
typedef enum _dc_err_type
{
	E_DC_ERR_DISCONNECT, /**< Specifies error while disconnecting. */
	E_DC_ERR_REQUEST, /**< Specifies error while sending the fetch request. */
	E_DC_ERR_USER_CANCELLED, /**< Specifies the error in case the user cancels the download transaction */
	E_DC_ERR_LOW_MEMORY /**< Specifies low memory error. */
} E_DC_ERR_TYPE ;

/** 
 * @brief This enumeration specifies the various event types which the application
 *		  can send to the Download Client module while communicating with it. Based
 *		  upon the received type, appropriate action would be carried out by the
 *		  Download Client.
 */
typedef enum _dc_event_type
{
	E_DC_APPLICATION_RESPONSE, /**< Depending upon the status of the download of the Media Object, the application uses this type to intimate the download client to send a notification to the download server accordingly. (Argument passed #ST_DC_APPLICATION_RESP) */
	E_DC_AUTHORIZATION_RESPONSE, /**< Intimates the Download Client module to resend the request for download, along with the authorization details. (Argument passed #ST_DC_AUTHORIZATION_HANDLER) */
	E_DC_MEMORY_INDICATION, /**< Makes the Download Client aware of whether sufficient memory is available on the device to proceed with the download of the Media Object. (Argument passed #ST_DC_MEMORY_RESP) */
	E_DC_CAPABILITY_INDICATION, /**< Makes the Download Client aware of whether the device is capable to handle the type of the Media Object being downloaded. (Argument passed #ST_DC_CAPABILITY_RESP) */
	E_DC_USER_INDICATION, /**< Used to send the User's response to the Download Client, to either proceed with the download of the Media Object, or, to abort the same. (Argument passed #ST_DC_USER_RESP) */
	E_DC_FETCH_HEADER_INFO, /**< Used to send the header indication, received for the fetch request from the application, to Download Client. (Argument passed #ST_DC_HEADER_IND) */
	E_DC_FETCH_INDICATION_INFO, /**< Used to send the response indication received from application, to Download Client. (Argument passed #ST_DC_FETCH_IND) */
	E_DC_FETCH_ERROR_INFO /**< In case of an error occuring during a download, this type is used to send the error indication, received from application, to Download Client. (Argument passed #ST_DC_ERR_IND) */
} E_DC_EVENT_TYPE ;

/** 
 * @brief This enumeration specifies the various event types which the Download Client uses to
 *		  to interact with the application. The type is passed along to the application, through
 *		  the callback, to initmate the application for carrying out the specified action.
 */
typedef enum _dc_callback_type
{
	E_DC_FETCH_REQUEST, /**< Intimates the application to send a fetch request for the specified URL. (Argument passed #ST_DC_FETCH_REQUEST) */
	E_DC_USER_CONFIRMATION, /**< Intimates the application to obtain the user's confirmation for either proceeding with the download of the Media Object or cancelling the same. (Argument passed #ST_DC_USER_CONFIRMATION) */
	E_DC_NOTIFY_USER, /**< Intimates the application to notify the user of the status of the download. (Argument passed #ST_DC_NOTIFY_USER) */
	E_DC_NEXT_URL, /**< Intimates the application to send a fetch request for the specified NextURL. (Argument passed #ST_DC_NEXT_URL) */
	E_DC_AUTHORIZATION_HANDLER, /**< Intimates the application to send the authorization information required to proceed for some particular download. (No argument is passed) */ 
	E_DC_MEMORY_CHECK, /**< Initmates the application to perform a memory check for ascertaining whether the device has sufficient memory for storing the Media Object to be downloaded. (Argument passed #ST_DC_MEMORY_IND) */
	E_DC_CAPABILITY_CHECK, /**< Intimates the application to perform a capability check for ascertaining whether the device is capable of handling/rendering the Media Object to be downloaded. (Argument passed #ST_DC_CAPABILITY_IND) */
	E_DC_HEADER_INDICATION, /**< Sends the received header for the Media Object to the application for further processing of the header content by the application. (Argument passed #ST_DC_HEADER_IND) */
	E_DC_RESPONSE_INDICATION, /**< Sends the received response buffer for the Media Object to the application for further processing of the response buffer by the application. (Argument passed #ST_DC_FETCH_IND) */
	E_DC_ERROR_INDICATION, /**< In case of an error occuring during the download of the Media Object, this type is used to send the error indication to the application. (Argument passed #ST_DC_FETCH_ERROR_INFO) */
	E_DC_OBJECT_INSTALLATION_STATUS /**< Used to send the Media Object's installation status to the application. (Argument passed is #ST_DC_INSTALLATION_STATUS) */
} E_DC_CALLBACK_TYPE ;

/** 
 * @brief The following structure is a list of all the MIME media types to be
 *		  supported for the successfull display/rendering of the Media Object
 */
typedef struct _dc_media_type_list
{
	JC_INT8						*pcMediaType ; /**< Specifies the media type */
	struct _dc_media_type_list	*pstNext ; /**< Pointer to the next media type in the list */
} ST_MEDIA_TYPE_LIST ;

/**
 * @brief The following structure is used to store the Download Descriptor details.
 *		  The download descriptor contains the various attributes used to describe 
 *		  the Media Object. These attributes are required for identifying, retrieving
 *		  and installing the Media Objects. These may also be used by the application 
 *		  for actual processing of the Media Object.
 */
typedef struct _dc_download_descriptor
{
	JC_CHAR				*pmMediaUrl; /**< Specifies the URL to be fetched for downloading the Media Object */
	JC_CHAR				*pmNotifyUrl ; /**< Specifies the URL the URL to which the object installation status report is to be sent, either incase of a successful completion of a download, or in case of failure */
	JC_CHAR				*pmNextUrl ; /**< Specifies the URL to which the application should navigate to in case the end user selects to invoke a browsing action after the download transaction has completed with either a success or a failure */
	JC_CHAR				*pmDDVersion ; /**< Indicates the version of the Download Descriptor technology */
	JC_CHAR				*pmName ; /**< Signifies a name for the media object by which the user can identify it */
	JC_CHAR				*pmDescription ; /**< Signifies a short textual description of the media object. It should be displayed to the user before the download of the media object */
	JC_CHAR				*pmVendor ; /**< Signifies the organization that provides the media object */
	JC_CHAR				*pmInfoUrl ; /**< Indicates a URL for obtaining further information describing the media object */
	JC_CHAR				*pmIconUrl ; /**< Indicates the URL for obtaining an icon which maybe used by the application to represent the media object in the user interface */
	JC_CHAR				*pmInstallParam ; /**< Signifies an installation parameter associated with the downloaded media object */
	JC_INT32			iSize ; /**< Stores the number of bytes to be downloaded from the specified URL. The size of the Media Object to be downloaded */
	ST_MEDIA_TYPE_LIST	*pstTypeList ; /**< Pointer to the ST_MEDIA_TYPE_LIST structure used for storing the MIME media types of the media object */
} ST_DOWNLOAD_DESCRIPTOR ;

/**
 * @brief The following structure is used to send the download descriptor data 
 *		  to the application for obtaining the user's confirmation for the 
 *		  download of the Media Object
 */
typedef struct _dc_user_confirmation
{
	ST_DOWNLOAD_DESCRIPTOR	*pstDD ; /**< Pointer to the structure holding the download descriptor's attributes.*/          
	JC_INT8					*pcBuffer ; /**< Specifies the icon buffer associated to the download descriptor's.*/
	JC_INT32				iBufferLen ; /**< Specifies the icon size.*/
	JC_INT8					*pcType ; /**< Specifies the icon type.*/
	E_DC_USER_RESPONSE		eResponse ; /**< Specifies the user's response received from the application */
} ST_DC_USER_CONFIRMATION ;

/**
 * @brief The following structure is used to send the status of the download transaction
 *		  to the application for intimating the user of the same. In case a download transaction fails due to a
 *		  HTTP related error while downloading, the rRetCode parameter would be set to JC_OK but the iStatus parameter
 *		  shall indicate the HTTP Status for the error. Otherwise, for all device dependent errors, the rRetCode parameter
 *		  shall hold the error code.
 */		  
typedef struct _dc_notify_user
{
	JC_RETCODE			rRetCode ; /**< Specifies the return code for the download transaction (JC_OK in case of success).*/
	JC_INT32			iStatus ; /**< Specifies the HTTP status for the download transaction.*/
	E_DC_MESSAGE_TYPE	eType ; /**< Specifies the type of the message being sent to the application, whether it is an information or an error message */
} ST_DC_NOTIFY_USER ;

/**
 * @brief The following structure is used to send the NextURL attribute value to the application in case
 *		  the attribute is present in the received download descriptor. The URL specified in the NextURL
 *		  attribute would then have to be fetched by the application.
 */		  
typedef struct _dc_next_url
{
	JC_CHAR		*pmNextUrl ; /**< Specifies the NextURL attribute value in case it is present in the received download descriptor. */
} ST_DC_NEXT_URL ;

/**
 * @brief The following structure is used by the application to send the authorization 
 *		  related details, such as, the username and the password, to the
 *		  download client. The download client module uses this information to
 *		  resend the fetch request along with the authorization details.
 */
typedef struct _dc_authorization_handler
{
	const JC_CHAR	*pmUserName ; /**< Specifies the username required for authenticating the user */
	const JC_CHAR	*pmPassword ; /**< Specifies the password required for authenticating the user */
	JC_RETCODE		rRetCode ; /**< Specifies the return value returned by the application for authentication. Determines whether to proceed with the authentication information or to cancel the download transaction */
} ST_DC_AUTHORIZATION_HANDLER ;

/**
 * @brief This structure describes the authorization structure that the application uses to
 *		  set the authorization information for the Download Client. The Download Client sends this
 *		  information while fetching a Media Object, whenever required.
 */
typedef struct _dc_auth_headers
{
	E_DC_AUTHORIZATION_TYPE		eAuthType ; /**< Specifies the type of authorization (basic or digest) */
	JC_CHAR						amName [MAX_DC_AUTH_NAME_SIZE + 1] ; /**< Specifies the user name */
	JC_CHAR						amPassword [MAX_DC_AUTH_PASSWORD_SIZE + 1] ; /**< Specifies the password */
	JC_INT8						acDomain [MAX_DC_AUTH_DOMAIN_SIZE + 1] ; /**< Specifies the domain information (in case of digest) */
	JC_INT8						acRealm [MAX_DC_AUTH_REALM_SIZE + 1] ; /**< Specifies the realm information (in case of digest) */
	JC_INT8						acNonce [MAX_DC_AUTH_NONCE_SIZE + 1] ; /**< Specifies the nonce information (in case of digest) */
} ST_DC_AUTH_HEADERS ;

/**
 * @brief The following structure is used to send information to the application
 *		  making a fetch request.
 */
typedef struct _dc_fetch_request
{
	JC_CHAR					*pmUrl ; /**< Specifies the URL to be fetched */
	JC_INT8					*pcPostData ; /**< In case of a POST request, this specifies the data to be posted to the specified URL */
	JC_UINT32				uiPostDataLen ; /**< In case of a POST request, this specifies the size of the post data's size */
	ST_MIME_HEADERS			*pstHeaders ; /**< Specifies the MIME request headers to be sent along with the request */
	ST_DC_AUTH_HEADERS		*pstAuthHeaders ;  /**< Specifies the authorization headers to be sent along with the request */
} ST_DC_FETCH_REQUEST ;

/**
 * @brief The following structure is used to store the information sent to the
 *		  application in case of an error occuring during the download transaction
 */
typedef struct _dc_fetch_error_info
{
	JC_RETCODE	rRetCode ; /**< Specifies the error code */
	JC_UINT32	uiReferenceID ; /**< Specifies the reference ID of the fetch request */
} ST_DC_FETCH_ERROR_INFO ;

/**
 * @brief This structure is used by the download client to specify the header indication of a 
 *		  fetch request to the application.
 */
typedef struct _dc_header_indication
{
	JC_UINT32				uiRequestID ; /**< Specifies the ID for which the header ind is sent */
	JC_UINT32				uiHeaderLength ; /**< Specifies the length of the header */
	JC_UINT32				uiContentLength ; /**< Specifies the length of the content (buffer) to be downloaded */
	E_HTTP_STATUS			eHttpStatus; /**< Status of the lower layers (HTTP and WSP) */
	JC_BOOLEAN				bIsBinary ; /**< Specifies whether the header information is in binary format or not */
	JC_INT8					*pcContentType ; /**< Specifies the content type of the content to be downloaded */
	JC_UINT8				*pucHeader ; /** Specifies the header */
	JC_CHAR					*pmUrl ; /** Specifies the URL for which the header is received. */
} ST_DC_HEADER_IND ;

/**
 * @brief This structure is used by the download client to specify the buffer indication of a 
 *		  fetch request to the application.
 */
typedef struct _dc_fetch_indication
{
	JC_UINT32		uiRequestID ; /**< Specifies the ID for which the fetch (buffer) indication is sent */
	JC_UINT32		uiCurrentLength ; /**< Specifies the length of the buffer to be downloaded */
	JC_BOOLEAN		bHasMoreData ; /**< Specifies whether more data is expected from the lower layers */
	JC_UINT8		*pucBuffer ; /**< Specifies the content buffer which is to be downloaded */
} ST_DC_FETCH_IND ;

/**
 * @brief This structure is used by the application to send the error indication of a 
 *		  fetch request to the Download Client.
 */
typedef struct _dc_error_ind
{
	E_DC_ERR_TYPE		eErrType ; /**< Specifies the type of error */
	JC_INT32			uiRequestID ; /**< Specifies the ID of the request for which the error indication is sent */
	JC_RETCODE			rCode ; /**< Specifies the error code */
} ST_DC_ERR_IND ;

/**
 * @brief For sending the return code of the download transaction to the Download Client, once the Media Object
 *		  has been received and rendered by the application, this structure is used.
 */
typedef struct _st_dc_application_resp
{
	JC_RETCODE		rRetCode ; /**< Specifies the return code for the status of the download of the Media Object (JC_OK in case of success).*/
} ST_DC_APPLICATION_RESP ;

/**
 * @brief The Download Client uses this structure to send the size of the Media Object
 *		  which is to be downloaded, to the application. Based upon this size value, the application
 *		  verifies whether the sufficient memory is available on the device to proceed with the download
 *		  of the Media Object.
 */
typedef struct _st_dc_memory_ind
{
	JC_INT32	iSize ; /**< Specifies the size of the Media Object to be downloaded */
} ST_DC_MEMORY_IND ;

/**
 * @brief The Download Client uses this structure to pass on the Media type/MIME type information
 *		  of the Media Object to be downloaded, to the application. 
 */
typedef struct _st_dc_capability_ind
{
	JC_INT8		*pcMediaType ; /**< Specifies the Media type/MIME type of the Media Object to be downloaded */
} ST_DC_CAPABILITY_IND ;

/**
 * @brief The Download Client uses this structure to inform the application whether the object has been successfully 
 *		  installed/rendered onto the device.
 */
typedef struct _st_dc_installation_status
{
	JC_RETCODE	rRetCode ; /**< Specifies the return code indicating whether the Media Object has been successfully installed/rendered on the device or not (JC_OK in case of success).*/
} ST_DC_INSTALLATION_STATUS ;

/**
 * @brief For sending the return code of the memory check to the Download Client, specifying whether the Media Object can be stored by
 *		  by the application, this structure is used.
 */
typedef struct _st_dc_memory_resp
{
	JC_RETCODE		rRetCode ; /**< Specifies the return code for the memory check indicating whether sufficient memory is available for the download of the Media Object (JC_OK in case of success and JC_ERR_DC_LOW_MEMORY in case sufficient memory is not available).*/
} ST_DC_MEMORY_RESP ;

/**
 * @brief For sending the return code of the capability check to the Download Client, specifying whether the 
 *		  device is capable of rendering the specified type of the Media Object.
 */
typedef struct _st_dc_capability_resp
{
	JC_RETCODE		rRetCode ; /**< Specifies the return code for the capability check indicating whether the device is capable of rendering specified type of the Media Object (JC_OK in case of success and JC_ERR_DC_CAPABILITY_ERROR in case the device is not capable of handling the Media Object). */
} ST_DC_CAPABILITY_RESP ;

/**
 * @brief For sending the user's response to the Download Client, to either proceed with the download,
 *		  or, to cancel the same, the application uses this structure.
 */
typedef struct _st_dc_user_resp
{
	E_DC_USER_RESPONSE	eResponse ; /**< Specifies the response sent by the user, to either proceed with the download of the Media Object, or, to cancel the same. */
} ST_DC_USER_RESP ;

/**
 * @brief This structure is used by the application for sending the download descriptor information to the
 *		  Download Client. The URL, buffer, buffer length and the content type for the download descriptor are passed onto the 
 *		  Download Client through this structure.
 */
typedef struct _st_dc_dd_info
{
	const JC_CHAR	*pmUrl ; /**< Specifies the URL for the download descriptor */
	JC_UINT8		*pucBuffer ; /**< Specifies the download descriptor buffer */
	JC_UINT32		uiBufferLen ; /**< Specifies the size of the download descriptor buffer */
	JC_INT8			*pcContentType ; /**< Specifies the content type of the download descriptor */
} ST_DC_DD_INFO ;

/**
 * @brief		Prototype of the callback which will be called by the
 *				download client to pass any indication to the application
 * @param[in]	pvArg			Specifies the application argument, which is passed to the download client
 *								during #jdi_DCInitialize. 
 * @param[in]	pvDCArg			Specifies the DD reference. This is useful when multiple DD are in processing
 *								and application can associate this event for the particular DD. This reference
 *								is provided to the application using #jdi_DCProcessContent.
 * @param[in]	eType			Specifies the callback type that the application needs to process.
 * @param[in]	pvCallbackInfo	Specifies the reference to the callback type. Application needs to typecast this 
 *								reference to the appropriate data-type. 
 * @retval		The callback function can return any application related error.
 *
 * The application needs to implement a callback with the following prototype in order
 * to get the various indications from the download client. The type of application
 * callback to be invoked is specified in the eType parameter
 */
typedef JC_RETCODE (* CB_DC_CALLBACK) (void						*pvArg, 
									   void						*pvDCArg,
									   E_DC_CALLBACK_TYPE		eType,
									   void						*pvCallbackInfo) ;
/**
 * @brief		The application needs to call this function to initialize the download client
 * @param[in]	cbGeneral		Specifies the application's general callback which will handle
 *								all the callback indications from the download client
 * @param[in]	pvCallbackArg	Specifies the application argument, which shall be used as the 
 *								first argument while invoking the callback function. 
 * @param[in]	vMemHandle		Pointer to the memory handle for download client
 * @param[out]	pvDCHandle		Specifies the download client handle which will be returned by
 *								this function. The application needs to use this handle to
 *								call all the other download client functions
 * @retval		JC_OK			If the download client is sucessfully initialized
 * @retval		JC_ERR_MEMORY_ALLOCATION	If a memory error occurs
 * @see			jdi_DCDeInitialize
 *
 * The application needs to call this function to initialize the download client.
 */
JC_RETCODE jdi_DCInitialize (CB_DC_CALLBACK		cbGeneral,
							 void				*pvCallbackArg,
							 JC_HANDLE			vMemHandle,
							 JC_HANDLE			*pvDCHandle) ;
/**
 * @brief		This function processes the download descriptor data
 * @param[in]	vDCHandle			Specifies the handle returned by #jdi_DCInitialize function. 
 * @param[in]	pstDDInfo			Specifies the information regarding the download
 *									descriptor URL, buffer, buffer length and the
 *									content type. 
 * @param[out]	pvDCSessionHandle	Specifies the reference for the given download description. Application
 *									needs to pass this reference to #jdi_DCSetEvent while updating.
 * @retval		JC_OK						If the received download descriptor is successfully
 *											processed
 * @retval		JC_ERR_NOT_IMPLEMENTED		In case an error occurs while processing the
 *											download descriptor of multipart type
 * @retval		JC_ERR_DC_UNSUPPORTED_TYPE	If the content type of the received download
 *											descriptor is not supported
 * @retval		JC_ERR_MEMORY_ALLOCATION	In case a memory error occurs during the 
 *											processing of the received download descriptor
 * @retval		JC_ERR_DC_CONTINUE			The return code specifying a non-blocking call.
 * @retval		JC_ERR_DC_INVALID_DD		If the download descriptor is not valid (like not adhering to XML Standards).
 * @retval		JC_ERR_DC_UNSUPPORTED_DD_VERSION	If the major version of the download descriptor is not supported
 *													(supported major version is 1).
 * @note		The parameters which should not be NULL are vDCHandle, pmUrl, pucBuffer and
 *				pcContentType
 *
 * This function is called by the application when it recieves content either of type
 * "application/vnd.oma.dd + xml" or of "application/vnd.wap.multipart.related" type,
 * implying that the received content is of download descriptor type. It then makes
 * various calls to obtain and store the information from the rceived download descriptor
 * and make appropriate calls for downlaoding the Media Object. After successfully parsing the download
 * descriptor, the handle to that particular download session is returned to the application.
 */
JC_RETCODE jdi_DCProcessContent (JC_HANDLE		vDCHandle,
								 ST_DC_DD_INFO	*pstDDInfo,
								 JC_HANDLE		*pvDCSessionHandle) ;

/**
 * @brief		The application needs to use this function to send an event to the download
 *				client module
 * @param[in]	vHandle		Specifies the handle returned by #jdi_DCInitialize function. 
 * @param[in]	eEventType	Specifies the event type
 * @param[in]	vDCArg		Specifies the download client reference which has been passed to the 
 *							application using #jdi_DCProcessContent. This should be a valid reference.
 * @param[in]	pvEventInfo	Specifies the event information
 * @retval		none
 *
 * The application needs to use this when it comes across any event or operation that is to 
 * be handled by the download client. The function in turn makes appropriate calls depending
 * upon the passed event type.
 */
void jdi_DCSetEvent (JC_HANDLE			vHandle,
					 E_DC_EVENT_TYPE	eEventType,
					 JC_HANDLE			vDCArg,
					 void 				*pvEventInfo) ;

/**
 * @brief		Deinitialize the download client.
 * @param[in]	vDCHandle	Specifies the handle returned by jdi_DCInitialize function. This reference 
 *							is not valid after invoking this API.
 * @retval		JC_OK		If the download client is deinitialized successfully
 * @retval		JC_ERR_INVALID_PARAMETER	If the download client handle received is null
 * @note		The parameter which should not be NULL is vDCHandle.
 *
 * The application needs to call this function to deinitialize the download client.
 */
JC_RETCODE jdi_DCDeInitialize (JC_HANDLE vDCHandle) ;

#endif /* DOWNLOAD_CLIENT_ENABLE */

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */
