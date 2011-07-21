/***************************************************************************
 * $Id: jdd_configapi.h,v 1.10 2009/04/13 06:38:21 browserdevjtbase Exp $
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
 * $Log: jdd_configapi.h,v $
 * Revision 1.10  2009/04/13 06:38:21  browserdevjtbase
 * Core Update
 *
 * Revision 1.9  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.8  2008/01/28 05:56:46  browserdevjtbase
 * Core Updated.
 *
 * Revision 1.7  2008/01/15 11:57:45  browserdevjtbase
 * Updated the jtbase.
 *
 * Revision 1.6  2007/09/27 14:52:23  browserdevjtbase
 * Core Update
 *
 * Revision 1.5  2007/06/05 10:50:07  browserdevjtbase
 * Core update.
 *
 * Revision 1.4  2007/04/27 07:21:30  browserdevjtbase
 * Mail Update
 *
 * Revision 1.3  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.19  2006/03/16 07:35:41  kumardevhtmlbrow
 * Added the application to add the session header
 *
 * Revision 1.18  2006/03/04 05:48:35  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.17  2006/01/19 15:33:04  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.16  2006/01/17 14:39:12  kumardevhtmlbrow
 * Updated the comments ...
 *
 * Revision 1.15  2006/01/11 12:48:32  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.14  2006/01/09 12:16:28  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.13  2006/01/06 07:13:03  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.12  2006/01/02 09:05:55  kumardevhtmlbrow
 * Changed to configure the TCP buffer as configurable
 *
 * Revision 1.11  2005/12/31 12:15:36  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

/**
 * @page protocolconfig Protocol Configuration
 * 
 * @section rdf RDF Location
 * @li Set the RDF location using #jdd_CFGGetRDFLocation.
 * 
 * @section useragent User Agent
 * @li Set the User agent for the browser application using #jdd_CFGGetUserAgent.
 *
 * @section accept Accept Headers
 * @li Set the charset types that are supported using #jdd_CFGGetAcceptCharset.
 * @li Set the content types that are supported using #jdd_CFGGetAcceptContentType.
 * @li Set the css content types that are supported using #jdd_CFGGetAcceptContentTypeForCSSRequest.
 * @li Set the image content types that are supported using #jdd_CFGGetAcceptContentTypeForImageRequest.
 * @li Set the audio content types that are supported using #jdd_CFGGetAcceptContentTypeForAudioRequest.
 * @li Set the video content types that are supported using #jdd_CFGGetAcceptContentTypeForVideoRequest.
 * @li Set the languages that are supported using #jdd_CFGGetAcceptLanguage.
 * @li Set the encoding types that are supported using #jdd_CFGGetAcceptEncoding. 
 *
 * @section timerop Transport Timers
 * @li Set the transport timer value using #jdd_CFGGetTransportTimerValue.
 * @li Set the transport push timer value using #jdd_CFGGetPushTimerValue.
 *
 * @section httpconfig HTTP Configurations
 * @li Set the maximum number of sockets that can be used at a time using #jdd_CFGGetHTTPMaxConnections.
 * @li Set the maximum length of TCP buffers (for socket recieve operations) using #jdd_CFGGetTCPRecvBuffer.
 * @li Set the maximum time a socket can be kept alive using #jdd_CFGGetMaxSocketAliveTime.
 *
 * @section wspconfig WSP Configurations
 * @li Specify whether continous polling is enabled in WSP mode using #jdd_CFGIsWSPContinuousPollingEnabled.
 * @li Set the maximum WTP group size while recieving using #jdd_CFGGetWSPMaxWTPGroupSize.
 * @li Specify the maximum time the transport can poll within a single timer function #jdd_CFGGetMaxTransportPollingTime. 
 * @li Set the maximum size of the PDU for any send operation using #jdd_CFGWSPGetSendPDUSize.
 * @li Set the maximum size of the PDU for any recieve operation using #jdd_CFGWSPGetRecvPDUSize.
 *
 * @section clientcap WSP Client Capabilies
 * @li Set the maximum client SDU size using #jdd_CFGGetClientSDUSize.
 * @li Set the maximum server SDU size using #jdd_CFGGetServerSDUSize.
 * @li Set the maximum size of the message that can be sent from the client using #jdd_CFGGetClientMsgSize.
 * @li Set the maximum size of the message that can be accepted from the server using #jdd_CFGGetServerMsgSize.
 * @li Set the maximum OS requests that can be handled using #jdd_CFGGetMaximumOSRequest.
 * @li Set the maximum OS push requests that be handled using #jdd_CFGGetPushMaxOSRequest.
 * @li Specify whether confirmed push is supported by client using #jdd_CFGIsConfirmedPushSupported.
 * @li Specify whether unconfirmed push is supported by client using #jdd_CFGIsPushSupported.
 * @li Specify whether WSP session suspend/resume is supported using #jdd_CFGIsSessionResumeSupported.
 * @li Specify whether acknowlegdement header is supported by the client using #jdd_CFGIsAckHeadersSupported.
 * @li Specify whether large data chunks can be accepted from server using #jdd_CFGIsLargeDataTransferSupported. 
 *
 * @section wspwtls WTLS Options
 * @li Set the number of cipher suite supported using #jdd_CFGGetCipherSuiteCount.
 * @li Set the number of key exchange supported using #jdd_CFGGetKeyExAlgCount.
 * @li Set the cipher suite that are supported using #jdd_CFGGetCipherSuite.
 * @li Set the key exchange algorithms supported using #jdd_CFGGetKeyExAlgorithm.
 * @li Set the file name where the WTLS context is to be used using #jdd_CFGGetWTLSContextName.
 *
 * @section httpssl SSL Options
 * @li Set the number of cipher suite supported using #jdd_CFGGetSSLCipherSuiteCount.
 * @li Set the cipher suites supported using #jdd_CFGGetSSLCipherSuite.
 *
 * @section otherconfig Other Configurations
 * @li Set the generic application header that is to be used for all the requests using #jdd_CFGGetGenericHeader.
 * @li Set the task information of the default application that will handle push messages (not registered) #jdd_CFGGetMimeHandlerTaskInfo.
 * @li Set the default root certificate using #jdd_CFGGetRootCertificate.
 * @li Set the maximum size of a user data chunk that can be requested by the transport to application using #jdd_CFGGetUserDataSize
 */

/**
 * @file jdd_configapi.h
 * @ingroup jB5Config
 * @brief Defines the functions that can be configured by an application 
 *		  for transport
 * 
 * The file defines functions for configuring different parameters in 
 * transport. These functions can be configured according to the application 
 * requirements. The parameters that can be configured with these functions 
 * are AcceptCharsets, AcceptContenttypes, AcceptLanguage, AcceptEncoding, 
 * RDFLocation, UserAgent, SDU, PDU and Message size for both client 
 
  
 */

#ifndef _JDD_CONFIG_H_
#define _JDD_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WAP_WITH_WTLS
#ifdef CERTICOM_WTLS
#include <jdi_wspwtls.h>
#else
#include <jdi_wtls.h>
#endif
#endif

#ifdef HTTP_WITH_SSL
#ifdef JCF_SSL
#include <jdi_ssl.h>
#elif defined SSL_4_5
#include <jdi_httpssl.h>
#endif
#endif
/**
 * @brief	  This function initializes the CFG module.
 * @retval		JC_OK	Initialization process is successful
 * @see			jdd_CFGDeInitialize
 *
 * The function is not used currently
 */
JC_RETCODE jdd_CFGInitialize (void) ;

/**
 * @brief	  This function deinitializes the CFG module.
 * @retval		JC_OK	deInitialization process is successful
 *
 * The function is not used currently
 */
JC_RETCODE jdd_CFGDeInitialize (void) ;

/**
 * @brief	Provides the Location where RDF information will be found
 * @param[in]		pstTaskInfo	Not used currently
 * @param[in,out]	pcRDFLocation	Holds the Location of RDF
 * @param[in,out]	puiLength	Specifies the memory length of pcRDFLocation
 * @retval		JC_OK	The Location of RDF is got successfully
 * @retval		JC_ERR_INVALID_PARAMETER	The parameters pcRDFLocation or/and puiLength are NULL
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY	The given memory for pcRDFLocation is not sufficient
 * @ retval		JC_ERR_CFG_NOT_FOUND if the RDF location is not specified
 * @note		The parameters pcRDFLocation and puiLength should not be NULL.
 *
 * This function will be called in transport to get the location of RDF.
 * This information will be sent to the server in the request buffer. If the 
 * given memory for pcRDFLocation is not sufficient then it stores the required 
 * memory length in puiLength and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetRDFLocation (JC_TASK_INFO *pstTaskInfo, JC_INT8 *pcRDFLocation, JC_UINT32 *puiLength) ;

/**
 * @brief	Provides the User agent information
 * @param[in]		pstTaskInfo	Not used currently
 * @param[in,out]	pcUserAgent	Holds the user agent information
 * @param[in,out]	puiLength	Specifies the memory length of pcUserAgent
 * @retval		JC_OK	The user agent information is got successfully
 * @retval		JC_ERR_INVALID_PARAMETER	The parameters pcUserAgent or/and puiLength are NULL
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY	The given memory for pcUserAgent is not sufficient
 * @note		The parameters pcUserAgent and puiLength should not be NULL.
 *
 * This function will be called in transport to get the user agent information.
 * This information will be sent to the server in the request buffer. If the 
 * given memory for pcUserAgent is not sufficient then it stores the required 
 * memory length in puiLength and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetUserAgent (JC_TASK_INFO *pstTaskInfo, JC_INT8 *pcUserAgent, JC_UINT32 *puiLength) ;

/**
 * @brief	Provides the Accept charsets supported by the application.
 * @param[in]		pstTaskInfo	Not used currently
 * @param[in,out]	pcCharset	Holds the accept charset
 * @param[in,out]	puiLength	Specifies the memory length of pcCharset
 * @param[in]		uiCharSetIndex	Specifies the index of the accept charset in the list
 * @retval		JC_OK	The accept charset is got successfully
 * @retval		JC_ERR_INVALID_PARAMETER	The parameters pcCharset or/and puiLength are NULL
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY	The given memory for pcCharset is not sufficient
 * @retval		JC_ERR_CFG_NOT_FOUND	Accept charset is not found in the list 
 *				for the given index	
 * @note		The parameters pcCharset and puiLength should not be NULL.
 *
 * This function will be called in transport to get the accept charsets
 * supported by the application. This information will be sent to the server 
 * in the request buffer. If the given memory for pcCharset is not sufficient 
 * then it stores the required memory length in puiLength and returns 
 * JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptCharset (JC_TASK_INFO *pstTaskInfo, JC_INT8 *pcCharset, JC_UINT32 *puiLength, JC_UINT32 uiCharSetIndex) ;

/**
 * @brief	Provides the Accept contenttypes supported by the application.
 * @param[in]		pstTaskInfo	Not used currently
 * @param[in,out]	pcContentType	Holds the accept contenttype
 * @param[in,out]	puiLength	Specifies the memory length of pcContentType
 * @param[in]		uiContentTypeIndex	Specifies the index of the accept contenttype in the list
 * @retval		JC_OK	The accept contenttype is got successfully
 * @retval		JC_ERR_INVALID_PARAMETER	The parameters pcContentType or/and puiLength are NULL
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY	The given memory for pcContentType is not sufficient
 * @retval		JC_ERR_CFG_NOT_FOUND	Accept contenttype is not found in the list 
 *				for the given index	
 * @note		The parameters pcContentType and puiLength should not be NULL.
 *
 * This function will be called in transport to get the accept contenttypes
 * supported by the application. This information will be sent to the server 
 * in the request buffer. If the given memory for pcContentType is not 
 * sufficient then it stores the required memory length in puiLength and 
 * returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptContentType (JC_TASK_INFO *pstTaskInfo, JC_INT8 *pcContentType, JC_UINT32 *puiLength, JC_UINT32 uiContentTypeIndex) ;

/**
 * @brief	Provides the Accept contenttypes supported by the application 
 *			for CSS content.
 * @param[in]		pstTaskInfo	Not used currently
 * @param[in,out]	pcContentType	Holds the accept contenttype
 * @param[in,out]	puiLength	Specifies the memory length of pcContentType
 * @param[in]		uiContentTypeIndex	Specifies the index of the accept contenttype in the list
 * @retval		JC_OK	The accept contenttype is got successfully
 * @retval		JC_ERR_INVALID_PARAMETER	The parameters pcContentType or/and puiLength are NULL
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY	The given memory for pcContentType is not sufficient
 * @retval		JC_ERR_CFG_NOT_FOUND	Accept contenttype is not found in the list 
 *				for the given index	
 * @note		The parameters pcContentType and puiLength should not be NULL.
 *
 * This function will be called in transport to get the accept contenttypes
 * supported by the application for CSS content. This information will be 
 * sent to the server in the request buffer. If the given memory for 
 * pcContentType is not sufficient then it stores the required memory length 
 * in puiLength and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptContentTypeForCSSRequest (JC_TASK_INFO *pstTaskInfo, JC_INT8 *pcContentType, 
													 JC_UINT32 *puiLength, JC_UINT32 uiContentTypeIndex) ;

/**
 * @brief	Provides the Accept contenttypes supported by the application 
 *			for image content.
 * @param[in]		pstTaskInfo	Not used currently
 * @param[in,out]	pcContentType	Holds the accept contenttype
 * @param[in,out]	puiLength	Specifies the memory length of pcContentType
 * @param[in]		uiContentTypeIndex	Specifies the index of the accept contenttype in the list
 * @retval		JC_OK	The accept contenttype is got successfully
 * @retval		JC_ERR_INVALID_PARAMETER	The parameters pcContentType or/and puiLength are NULL
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY	The given memory for pcContentType is not sufficient
 * @retval		JC_ERR_CFG_NOT_FOUND	Accept contenttype is not found in the list 
 *				for the given index	
 * @note		The parameters pcContentType and puiLength should not be NULL.
 *
 * This function will be called in transport to get the accept contenttypes
 * supported by the application for image content. This information will be 
 * sent to the server in the request buffer. If the given memory for 
 * pcContentType is not sufficient then it stores the required memory length 
 * in puiLength and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptContentTypeForImageRequest (JC_TASK_INFO *pstTaskInfo, JC_INT8 *pcContentType, JC_UINT32 *puiLength, JC_UINT32 uiContentTypeIndex) ;

/**
 * @brief	Provides the Accept contenttypes supported by the application 
 *			for audio content.
 * @param[in]		pstTaskInfo	Not used currently
 * @param[in,out]	pcContentType	Holds the accept contenttype
 * @param[in,out]	puiLength	Specifies the memory length of pcContentType
 * @param[in]		uiContentTypeIndex	Specifies the index of the accept contenttype in the list
 * @retval		JC_OK	The accept contenttype is got successfully
 * @retval		JC_ERR_INVALID_PARAMETER	The parameters pcContentType or/and puiLength are NULL
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY	The given memory for pcContentType is not sufficient
 * @retval		JC_ERR_CFG_NOT_FOUND	Accept contenttype is not found in the list 
 *				for the given index	
 * @note		The parameters pcContentType and puiLength should not be NULL.
 *
 * This function will be called in transport to get the accept contenttypes
 * supported by the application for audio content. This information will be 
 * sent to the server in the request buffer. If the given memory for 
 * pcContentType is not sufficient then it stores the required memory length 
 * in puiLength and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptContentTypeForAudioRequest (JC_TASK_INFO *pstTaskInfo, JC_INT8 *pcContentType, JC_UINT32 *puiLength, JC_UINT32 uiContentTypeIndex) ;

/**
 * @brief	Provides the Accept contenttypes supported by the application 
 *			for video content.
 * @param[in]		pstTaskInfo	Not used currently
 * @param[in,out]	pcContentType	Holds the accept contenttype
 * @param[in,out]	puiLength	Specifies the memory length of pcContentType
 * @param[in]		uiContentTypeIndex	Specifies the index of the accept contenttype in the list
 * @retval		JC_OK	The accept contenttype is got successfully
 * @retval		JC_ERR_INVALID_PARAMETER	The parameters pcContentType or/and puiLength are NULL
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY	The given memory for pcContentType is not sufficient
 * @retval		JC_ERR_CFG_NOT_FOUND	Accept contenttype is not found in the list 
 *				for the given index	
 * @note		The parameters pcContentType and puiLength should not be NULL.
 *
 * This function will be called in transport to get the accept contenttypes
 * supported by the application for video content. This information will be 
 * sent to the server in the request buffer. If the given memory for 
 * pcContentType is not sufficient then it stores the required memory length 
 * in puiLength and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptContentTypeForVideoRequest (JC_TASK_INFO *pstTaskInfo, JC_INT8 *pcContentType, JC_UINT32 *puiLength, JC_UINT32 uiContentTypeIndex) ;

/**
 * @brief	  Provides the Accept languages supported by the application.
 * @param[in]		pstTaskInfo	Not used currently
 * @param[in,out]	pcLanguage	Holds the accept language
 * @param[in,out]	puiLangLength	Specifies the memory length of pcLanguage
 * @param[in]		uiLangTypeIndex		Specifies the index of the accept language in the list
 * @retval		JC_OK	The accept language is got successfully
 * @retval		JC_ERR_INVALID_PARAMETER	The parameters pcLanguage or/and puiLangLength are NULL
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY	The given memory for pcLanguage is not sufficient
 * @retval		JC_ERR_CFG_NOT_FOUND	Accept language is not found in the list 
 *				for the given index	
 * @note		The parameters pcLanguage and puiLangLength should not be NULL.
 *
 * This function will be called in transport to get the accept languages
 * supported by the application. This information will be sent to the server 
 * in the request buffer. If the given memory for pcLanguage is not sufficient 
 * then it stores the required memory length in puiLangLength and returns 
 * JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptLanguage (JC_TASK_INFO *pstTaskInfo, JC_INT8 *pcLanguage, JC_UINT32 *puiLangLength, JC_UINT32 uiLangTypeIndex) ;

/**
 * @brief	  Provides the Accept encodings supported by the application.
 * @param[in]		pstTaskInfo	Not used currently
 * @param[in,out]	pcEncoding	Holds the accept encoding
 * @param[in,out]	puiLength	Specifies the memory length of pcEncoding
 * @param[in]		uiIndex		Specifies the index of the accept encoding in the list
 * @retval		JC_OK	The accept encoding is got successfully
 * @retval		JC_ERR_INVALID_PARAMETER	The parameters pcEncoding or/and puiLength are NULL
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY	The given memory for pcEncoding is not sufficient
 * @retval		JC_ERR_CFG_NOT_FOUND	Accept encoding is not found in the list 
 *				for the given index	
 * @note		The parameters pcEncoding and puiLength should not be NULL.
 *
 * This function will be called in transport to get the accept encodings 
 * supported by the application. This information will be sent to the server 
 * in the request buffer. If the given memory for pcEncoding is not sufficient 
 * then it stores the required memory length in puiLength and returns 
 * JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptEncoding (JC_TASK_INFO *pstTaskInfo, JC_INT8 *pcEncoding, JC_UINT32 *puiLength, JC_UINT32 uiIndex) ;

/**
 * @brief	  Provides the generic data that the application wants to send to the lower layers.
 * @param[in]		pstTaskInfo	Not used currently
 * @param[in,out]	pcHeaderName The name of the generic header should be returned in this parameter
 * @param[in,out]	uiHeaderNameLen Specifies the length of the header name.
 * @param[in,out]	pcHeaderValue The header value should be returned in this parameter.
 * @param[in,out]	puiHeaderValueLen Specifies the length of the header value
 * @param[in]		uiGenericHeaderIndex Specifies the index of the generic header
 * @retval		JC_OK	If the generic header is sent successfully.
 * @retval		JC_ERR_INVALID_PARAMETER	If invalid input parameters are specified
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY	The memory given for the header value is insufficient
 * @retval		JC_ERR_CFG_NOT_FOUND	If the header related to the generic header index is not found.
 * @note		The parameters pcEncoding and puiLength should not be NULL.
 *
 * This function will be called in transport to get the generic header that the application wants 
 * to send to the lower layers (HTTP, WSP).  If the given memory for pcHeaderValue is not sufficient 
 * then it stores the required memory length in puiLength and returns 
 * JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetGenericHeader (JC_TASK_INFO *pstTaskInfo, JC_INT8 *pcHeaderName, JC_INT32	uiHeaderNameLen,
									JC_INT8	*pcHeaderValue, JC_UINT32 *puiHeaderValueLen, JC_UINT32 uiGenericHeaderIndex) ;

/**
 * @brief	  Provides the task information that handles the push messages.
 * @param[in]		pcContentType	Not used currently
 * @param[in,out]	pstTaskInfo		Stores the task information, that will 
 *					handle the push messages
 * @retval		JC_OK	The required information is got successfully
 * @retval		JC_ERR_INVALID_PARAMETER	The parameter pstTaskInfo is NULL
 * @note		The parameter pstTaskInfo should not be NULL.
 *
 * This function will be called by transport when it receives a push message 
 * to get the task information that handles the push messages
 */
JC_RETCODE jdd_CFGGetMimeHandlerTaskInfo (const JC_INT8 *pcContentType, JC_TASK_INFO *pstTaskInfo) ;

/**
 * @brief	 Provides the client SDU size.
 * @retval		JC_INT32 Returns the size of the client SDU size
 *
 * This function will be called in transport while setting the client capabilities.
 */
JC_INT32 jdd_CFGGetClientSDUSize (void) ;

/**
 * @brief	 Provides the server SDU size.
 * @retval		JC_INT32 Returns the size of the server SDU size
 *
 * This function will be called in transport while setting the client capabilities.
 */
JC_INT32 jdd_CFGGetServerSDUSize (void) ;

/**
 * @brief	Defines the client capability for the size of a message that it 
 *			can send to the server.
 * @retval		JC_INT32	Returns the size of the client messages
 *
 * This function will be called in transport while setting the client capabilities.
 */
JC_INT32 jdd_CFGGetClientMsgSize (void) ;

/**
 * @brief	Defines the client capability for the size of a message that it 
 *			can accept from the server.
 * @retval		JC_INT32	Returns the size of the server messages
 *
 * This function will be called in transport while setting the client capabilities.
 */
JC_INT32 jdd_CFGGetServerMsgSize (void) ;

/**
 
  
 *
 * This function will be called in transport while setting the client capabilities.
 */
JC_UINT8 jdd_CFGGetMaximumOSRequest (void) ;

/**
 
  
 *
 * This function will be called in transport while setting the client capabilities.
 */
JC_INT32 jdd_CFGGetPushMaxOSRequest (void) ;

/**
 * @brief	Defines the client capability for confirmed push operations.
 * @retval		E_TRUE	Client supports confirmed push operations
 * @retval		E_FALSE	Client doest not support confirmed push operations
 *
 * This function will be called in transport while setting the client capabilities.
 */
JC_BOOLEAN jdd_CFGIsConfirmedPushSupported (void) ;

/**
 * @brief	Defines the client capability for push operations.
 * @retval		E_TRUE	Client supports push operations
 * @retval		E_FALSE	Client doest not support push operations
 *
 * This function will be called in transport while setting the client capabilities.
 */
JC_BOOLEAN jdd_CFGIsPushSupported (void) ;

/**
 * @brief	Defines the client capability for resuming a session.
 * @retval		E_TRUE	Client supports resuming a session
 * @retval		E_FALSE	Client doest not support resuming a session
 *
 * This function will be called in transport while setting the client capabilities.
 */
JC_BOOLEAN jdd_CFGIsSessionResumeSupported (void) ;

/**
 * @brief	Defines the client capability for acknowledgement headers.
 * @retval		E_TRUE	Client supports acknowledgement headers
 * @retval		E_FALSE	Client doest not support acknowledgement headers
 *
 * This function will be called in transport while setting the client capabilities.
 */
JC_BOOLEAN jdd_CFGIsAckHeadersSupported (void) ;

/**
 * @brief	Defines the client capability for large data transfer.
 * @retval		E_TRUE	Client supports large data transfer
 * @retval		E_FALSE	Client doest not support large data transfer
 *
 * This function will be called in transport while setting the client capabilities.
 */
JC_BOOLEAN jdd_CFGIsLargeDataTransferSupported (void) ;

/**
 * @brief	Provides the PDU size used in send operations in WSP.
 * @retval		JC_UINT16 Returns the size of send PDU size
 *
 * This function should be called during initialization of WSP module.
 */
JC_UINT16 jdd_CFGWSPGetSendPDUSize (void) ;

/**
 * @brief	Provides the PDU size used in receive operations in WSP.
 * @retval		JC_UINT16 Returns the size of receive PDU size
 *
 * This function should be called during initialization of WSP module.
 */
JC_UINT16 jdd_CFGWSPGetRecvPDUSize (void) ;

#ifdef WAP_WITH_WTLS

/**
 * @brief	Provides the Cipher suite count
 * @param[in]	pstTaskInfo Not used currently
 * @param[out]	puiCipherSuiteCount Holds the Cipher suite count
 * @retval		JC_OK Successfully got the Cipher suite count
 * @note		The parameter puiCipherSuiteCount should not be NULL.
 *
 * This function will be called by transport to get the Cipher suite count.
 */
JC_RETCODE jdd_CFGGetCipherSuiteCount (JC_TASK_INFO *pstTaskInfo, JC_UINT32 *puiCipherSuiteCount) ;

/**
 * @brief	Provides the key exchange algorithm count
 * @param[in]	pstTaskInfo Not used currently
 * @param[in]	uiClass Specifies the class of algorithm
 * @param[out]	puiKeyExAlgCount Holds the algorithm count
 * @retval		JC_OK Successfully got the algorithm count
 * @note		The parameter puiKeyExAlgCount should not be NULL.
 *
 * This function will be called by transport to get the key exchange algorithm count.
 */
JC_RETCODE jdd_CFGGetKeyExAlgCount (JC_TASK_INFO *pstTaskInfo, JC_UINT32 uiClass, JC_UINT32 *puiKeyExAlgCount) ;

/**
 * @brief	Provides the Cipher suites to be used during secure session
 * @param[in]	pstTaskInfo Not used currently
 * @param[in]	uiIndex Specifies the index of the algorithm in the list
 * @param[out]	phCipherSuite Holds the Cipher suite
 * @retval		JC_OK Successfully got the Cipher suite
 * @retval		JC_ERR_CFG_NOT_FOUND No Cipher suite found in the list with the given index
 * @note		The parameter phCipherSuite should not be NULL.
 *
 * This function will be called by transport to get the Cipher suites.
 * One of them will be selected in handshake process that will be used during 
 * the secure session.
 */
JC_RETCODE jdd_CFGGetCipherSuite (JC_TASK_INFO *pstTaskInfo, JC_UINT32 uiIndex, E_WTLS_CIPHER_SUITE *phCipherSuite) ;

/**
 * @brief	Provides the key exchange algorithms to be used during secure session
 * @param[in]	pstTaskInfo Not used currently
 * @param[in]	uiIndex Specifies the index of the algorithm in the list
 * @param[in]	uiClass Specifies the class of algorithm
 * @param[out]	phKeyExAlgorithm Holds the algorithm
 * @retval		JC_OK Successfully got the algorithm
 * @retval		JC_ERR_CFG_NOT_FOUND No algorithm found in the list with the given index
 * @note		The parameter phKeyExAlgorithm should not be NULL.
 *
 * This function will be called by transport to get the key exchange algorithms.
 * One of them will be selected in handshake process that will be used during 
 * the secure session.
 */
JC_RETCODE jdd_CFGGetKeyExAlgorithm (JC_TASK_INFO *pstTaskInfo, JC_UINT32 uiIndex, JC_UINT32 uiClass, E_WTLS_KEY_EXCHANGE_ALG *phKeyExAlgorithm) ;
#endif

#ifdef HTTP_WITH_SSL

/**
 * @brief		Provides the SSL Cipher suite count
 * @param[in]	pstTaskInfo Not used currently
 * @param[out]	puiCipherSuiteCount Holds the SSL Cipher suite count
 * @retval		JC_OK Successfully got the SSL Cipher suite count
 * @note		The parameter puiCipherSuiteCount should not be NULL.
 *
 * This function will be called by transport to get the SSL Cipher suite count.
 */
JC_RETCODE jdd_CFGGetSSLCipherSuiteCount (JC_TASK_INFO *pstTaskInfo, JC_UINT32 *puiCipherSuiteCount) ;

/**
 * @brief		Provides the SSL Cipher suites to be used during secure session
 * @param[in]	pstTaskInfo Not used currently
 * @param[in]	uiIndex Specifies the index of the cipher suite in the list
 * @param[out]	phCipherSuite Holds the Cipher suite
 * @retval		JC_OK Successfully got the SSL Cipher suite
 * @retval		JC_ERR_CFG_NOT_FOUND No Cipher suite found in the list with the given index
 * @note		The parameter phCipherSuite should not be NULL.
 *
 * This function will be called by transport to get the SSL Cipher suites.
 * One of them will be selected in handshake process that will be used during 
 * the secure session.
 */
JC_RETCODE jdd_CFGGetSSLCipherSuite (JC_TASK_INFO *pstTaskInfo, JC_UINT32 uiIndex, E_SSL_CIPHER_SUITE *phCipherSuite) ;

#endif

/**
 * @brief	Provides the root certificates to be used during secure session
 * @param[in]	pstTaskInfo Not used currently
 * @param[in]	uiRootIndex Specifies the index of the certificate in the list
 * @param[in,out]	puiRootLength specifeis the length of the variable pmFileName
 * @param[in,out]	pmFileName Holds the file name which has the required 
 *					certificate information
 * @retval		JC_OK	Successfully got the required information
 * @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY The given memory for the variable 
 *				pmFileName is not sufficient
 * @note		The parameters puiRootLength and pmFileName should not be NULL.
 *
 * This function will be called by transport to get the root certificates, 
 * which will be used during the secure session establishment.
 */
JC_RETCODE jdd_CFGGetRootCertificate (JC_TASK_INFO *pstTaskInfo, JC_UINT32 uiRootIndex,
									  JC_UINT32	   *puiRootLength, JC_CHAR *pmFileName) ;

/**
 * @brief	Specifies the file name to be used to store and retrieve the 
 *			WTLS Context information.
 * @param[in]	uiSize			Not used currently
 * @param[in,out]	pcName		Holds the WTLS Context name
 * @param[in,out]	puiLength	Not used currently
 * @retval		JC_OK			Successfully got the file name
 * @note		The parameter pcName should not be NULL
 *
 * This function will be called to store and retrieve the WTLS Context 
 * information that will be used during secure session.
 */
JC_RETCODE jdd_CFGGetWTLSContextName (JC_UINT32 uiSize, JC_INT8 *pcName, JC_UINT32 *puiLength) ;

/**
 * @brief	 Defines the maximum no of connections to be used in HTTP.
 
 * @note		The return value should not be zero
 *
 * The function will be called during initialization of HTTP module to 
 * set the maximum no of socket connections that can be opened at any time.
 */
JC_UINT32 jdd_CFGGetHTTPMaxConnections (void) ;

/**
 * @brief	 Defines the TCP Receive buffer length to be used in HTTP.
 * @retval		JC_UINT32	TCP Receive buffer length in HTTP
 *
 * The function will be called during initialization of HTTP module to set 
 * the length of the TCP receive buffer to be used in socket receive operations.
 */
JC_UINT32 jdd_CFGGetTCPRecvBuffer (void) ;

/**
 * @brief	 Defines the user data size that the transport can request to the application.
 * @retval		JC_UINT32	User data Size
 *
 * The function will be called by transport to get the user data size that the transport can
 * request to the application in one chunk.
 */
JC_UINT32 jdd_CFGGetUserDataSize (void) ;

/**
 * @brief	 Defines the minimum timer value of the transport timer
 * @retval		JC_UINT32	Transport Timer value
 *
 * The function will be called to find out the transport timer value.
 */

JC_UINT32 jdd_CFGGetTransportTimerValue (void) ;

/**
 * @brief	 Defines the minimum timer value of the push timer
 * @retval		JC_UINT32	Push Timer value
 *
 * The function will be called to find out the push timer value.
 */

JC_UINT32 jdd_CFGGetPushTimerValue (void) ;

/**
 * @brief	 Specifies whether the transport can perform continuous polling in case of WSP
 *			 connection mode or should it wait for the application response before polling.
 * @retval		Boolean value specifing whether continuous polling is enabled or not.
 *
 * The function will be called to find out whether continuous polling is enabled if WSP connection
 * mode is used.
 */
JC_BOOLEAN jdd_CFGIsWSPContinuousPollingEnabled (void) ;

/**
 * @brief Specifies the maximum WTP Group size that can be processed by the WAPStack during
 * receiving.
 * @return Any value greater than zero, specifies the WTP Group Size. If it is zero, then it
 * is the default value of the gateway is used.
 * @note We recommend this API should be configured for those platform where the dynamic memory
 * is very limited.
 */
JC_UINT16 jdd_CFGGetWSPMaxWTPGroupSize (void) ;

/**
 * @brief	Specifies the maximum time for a socket to be kept alive in HTTP 
 * @return returns the maximum alive time for a socket in seconds.
 *		   
 */
JC_UINT32 jdd_CFGGetMaxSocketAliveTime (void) ;

/**
 * @brief Specifies whether the idle socket (no request is pending) in the HTTP layer 
 * needs to be closed, in case if the connection is persistent.
 */
JC_BOOLEAN jdd_CFGCloseIdleSocket (void) ;

/**
 * @brief	Specifies the maximum time the transport task can poll when the data is 
 *			available in the socket. 
 * @return Returns the maximum time that transport can poll continuously. If it
 *			returns zero, the transport poll until the lower layer returns would block.
 */
JC_UINT32 jdd_CFGGetMaxTransportPollingTime (void) ;

/**
 * @brief Specifies the maximum number of requests that can be pipelined on a 
 * pipeline supported socket connection at a time.
 * @return	Number of requests that can be pipelined.
 * @return	0	There is no limit on the number of requests that can be pipelined.
 */
JC_UINT16 jdd_CFGHTTPGetMaxPipelineReqCount (void) ;

/**
 * @brief Specifies the maximum amount of data that can be queued at HTTP
 * @return	0	Specifies that there is no limit on the amount of data that can be queued.
 * @return	Others	Maximum amount of data that can be queued at HTTP layer.
 *
 * This function is used for event based sockets. When receive events comes from platform 
 * HTTP layer will read the data and put it in queue. So if application wants to put a limit
 * on the amount of data that can be queued at HTTP layer, it can be configured here. Data 
 * will be queued in HTTP layer until it reaches this limit, once it reaches limit, the 
 * receive events will be ignored and count will be maintained  for each receive event.
 * Once the data in the queue goes below this limit HTTP once again starts reading data.
 * If 0 is returned here, it is assumed that there is no limit on the queue.
 */
JC_UINT32 jdd_CFGGetHTTPMaxRecvQueueSize (void) ;


#ifdef	EMAIL_PROTOCOL_ENABLE
/**
 * @brief Specifies the maximum time for a socket to be kept alive in Email Protocols.
 * @return returns the maximum alive time for a socket in seconds.
 *
 */
JC_UINT32 jdd_CFGGetMaxEmailSocketAliveTime (void) ;

/**
 * @brief Specifies the maximum time value for socket select call in Email Protocols.
 * @return returns the maximum alive time for a socket in milliseconds.
 *
 */
JC_UINT32 jdd_CFGGetMaxEmailSocketSelectTimeout (void) ;

#endif	/** EMAIL_PROTOCOL_ENABLE **/

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


