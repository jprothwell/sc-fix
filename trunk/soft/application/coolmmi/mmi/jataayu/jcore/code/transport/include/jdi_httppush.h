/***************************************************************************
 * $Id: jdi_httppush.h,v 1.5 2009/06/02 04:44:44 browserdevjtbase Exp $
 * $Revision: 1.5 $
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
 * $Log: jdi_httppush.h,v $
 * Revision 1.5  2009/06/02 04:44:44  browserdevjtbase
 * Core Update
 *
 * Revision 1.19  2009/05/18 09:44:06  sriramdevhtmlbrow
 * Handling error conditions in http push. When error comes, http push indication will be sent with error code set.
 *
 * Revision 1.18  2008/09/17 11:59:04  sriramdevhtmlbrow
 * In case of socket events if PUSH buffer is sent in the socket read event itself then that buffer is processed instead of polling for the buffer from the socket. Fix for jtrack issue 2391.
 *
 * Revision 1.17  2006/05/09 10:28:36  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.16  2006/04/28 07:36:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.15  2006/01/25 07:22:17  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.14  2006/01/18 10:36:40  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.13  2006/01/11 12:48:32  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.12  2006/01/09 07:00:54  kumardevhtmlbrow
 * Updated to HTTP push to handle listen has event based.
 *
 * Revision 1.11  2006/01/03 14:29:26  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.10  2005/12/29 14:36:37  kumardevhtmlbrow
 * Integrated the HTTP Push
 *
 * Revision 1.9  2005/12/29 11:38:30  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2005/12/12 15:29:02  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.7  2005/09/19 14:01:38  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2005/09/17 11:58:39  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.4  2005/09/12 07:25:40  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/09/08 15:05:25  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.2  2005/09/08 11:45:16  kumardevhtmlbrow
 * Updated the interface
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_httppush.h
 * @ingroup transport
 * @brief Provides the set of interface for the transport application for recieving the wap push
 * content from the server.
 * 
 * This set of Interface is used for Initializing the http push module, once it is been initialised
 * successfully then transport is able to perform accept, read, write and close opertions on that socket.
 * For all these an event based api is provided "jdi_HTTPPushTOEvent". For validating whether the socket 
 * is valid or not an api is provided. After calling the callback function as api is provided to free
 * the memory allocated to http push message structure and its elements. In last api is provided for 
 * deinitialize the handle returns during Initialize fucntion and it free all the memory allocated and
 * all the handles.
 */

#ifndef _JDI_HTTPPUSH_H
#define _JDI_HTTPPUSH_H

/**
 * @brief This enumeration contains the different type of push events.
 */
typedef enum _http_push_event
{
	E_HTTP_PUSH_CONNECT, /**< Specifies the Connect push event. */ 
	E_HTTP_PUSH_LISTEN, /**< Specifies the Listen push event. */
	E_HTTP_PUSH_ACCEPT, /**< Specifies the Accept push event. */
	E_HTTP_PUSH_READ, /**< Specifies the Read push event. */
	E_HTTP_PUSH_WRITE, /**< Specifies the Write push event. */
	E_HTTP_PUSH_CLOSE, /**< Specifies the Close push event. */
	E_HTTP_PUSH_NONE /**< Specifies the Unknown push event. */
} E_HTTP_PUSH_EVENT ;

/**
 * @brief This structure describes the push message structure.
 */
typedef struct _http_push_msg
{
	JC_INT8		*pcHeader ; /**< Specifies the header of push messsage. */
	JC_INT32	iHeaderLen ; /**< Specifies the header length of push message. */
	JC_INT8		*pcContentType ; /**< Specifies the content type of push message. */
	JC_UINT8	*pucBuffer ; /**< Specifies the actual content of push message. */
	JC_UINT32	uiBufferLen ; /**< Specifies the actual content length of push message. */
	JC_RETCODE	rCode ; /**< If any error comes at any point push content indication will be 
							 sent with this rCode and without any data. */
} ST_HTTP_PUSH_MSG ;

/**
 * @brief This callback function used after getting the push content.
 * @param[in]	pvArg Specifies the Memory Handle.
 * @param[in]	pstPushMsg Specifies the structure that contains the push content information.
 * @retval		JC_OK On successfull.
 * @retval		Others 
 *
 * This function is called by transport layer after parsing the push content
 * and filling the http push message structure.
 */
typedef JC_RETCODE (* CB_HTTP_PUSH_MSG) (void				*pvArg,
										 const ST_HTTP_PUSH_MSG 	*pstPushMsg) ;

/**
 * @brief This function initializes the PPG Orignated http push. 
 * @param[in]	vMemHandle Specifies the Memory Handle.
 * @param[in]	uhPortNum Specifies the port number on which content need to be recieved.
 * @param[in]	pvArg Specifies the callback function arguments.
 * @param[in]	cbPushMsg Specifies the callback function name.
 * @param[out]	pvPushHandle Specifies a pointer to JC_HANDLE, used as out parameter.
 * @retval		JC_OK On successfull initialize of the http push
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		Others 
 * @see			jdi_HTTPPushDeinitialize
 * @note		The parameter which should not be NULL are pvPushHandle.
 * @note		The caller function should pass pvPushHandle with all the other cache API.
 *
 * This function is used for creating the active TCP/IP connectin between PPG/Server
 * and Terminal. This function listen of specific port as specified as argument and
 * return the Push Handle.
 */
JC_RETCODE jdi_HTTPPushPOInitialize (JC_HANDLE			vMemHandle,
									 JC_UINT16			uhPortNum,
									 JC_UINT32			uiConnID, 
									 void				*pvArg,
									 CB_HTTP_PUSH_MSG	cbPushMsg,
									 JC_HANDLE			*pvPushHandle) ;

/**
 * @brief This function initializes the Terminal Orginated http push. 
 * @param[in]	vMemHandle Specifies the Memory Handle.
 * @param[in]	pcIpAddress Specifies the IP address of PPG where connection need to be established.
 * @param[in]	uhPortNum Specifies the port number on which connection need to be established.
 * @param[in]	pvArg Specifies the callback function arguments.
 * @param[in]	cbPushMsg Specifies the callback function name.
 * @param[out]	pvPushHandle Specifies a pointer to JC_HANDLE, used as out parameter.
 * @retval		JC_OK On successfull initialize of the http push
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		Others 
 * @see			jdi_HTTPPushDeinitialize
 * @note		The parameter which should not be NULL are pvPushHandle.
 * @note		The caller function should pass pvPushHandle with all the other cache API.
 *
 * This function is used for creating the active TCP/IP connection between PPG/Server
 * and Terminal. This function connect to the IP Address and port number, specified 
 * as argument and return the Push Handle.
 */
JC_RETCODE jdi_HTTPPushTOInitialize (JC_HANDLE			vMemHandle,
									 JC_INT8			*pcIpAddress,
									 JC_UINT16			uhPortNum,
									 JC_UINT32			uiConnID,
									 void				*pvArg,
									 CB_HTTP_PUSH_MSG	cbPushMsg,
									 JC_HANDLE			*pvPushHandle) ;

 
/**
 * @brief This function Checkes whether the Socket Id is Valid or not.
 * @param[in]	vPushHandle Specifies the Push Handle.
 * @param[in]	iSocketID Specifies the Socket Id.
 * @retval		TRUE On Success.
 * @retval		FALSE On Failure.
 * @note		The parameter which should not be NULL are vPushHandle.
 * 
 * This function is used for Checking whether the Socket Id is Valid or not.
 * It checks the Specified socket Id in the Transaction List and validates 
 * whether it exists in the list or not.
 */
JC_BOOLEAN jdi_HTTPPushIsValidSocket (JC_HANDLE	vPushHandle,
									  JC_INT32	iSocketID) ;

/**
 * @brief This function is used for sending the HttpPush Event and performing the event based functions.
 * @param[in]	vPushHandle Specifies the Push Handle.
 * @param[in]	ePushEvent Specifies the Push Event type.
 * @param[in]	iSocketID Specifies the Socket Id.
 * @param[in]	pucBuffer Specifies the buffer if present.
 * @param[in]	uiBufLen  Specifies the length of pucBuffer it is not NULL otherwise 
 *						  it specifies the number of bytes to read from socket layer.
 * @retval		JC_OK On success.
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_ERR_HTTPPUSH_RECV_CONTINUE if still more content is to be recv.
 * @retval		JC_ERR_HTTPPUSH_RECV_PARTIAL_HEADER if recieved only partial content.
 * @retval		Others 
 * @note		The parameter which should not be NULL are vPushHandle.
 * 
 * This function is used for performing the Event based HttpPush functions.
 * Event Based Function can be of type Accept, Read, Write, Close and Unknown type.
 * And also a socket Id is specified that is used for only reading, writing and closing 
 * purpose not for accept and unknown/none event type.
 */
JC_RETCODE jdi_HTTPPushEvent (JC_HANDLE				vPushHandle,
								E_HTTP_PUSH_EVENT	ePushEvent,
								JC_INT32			iSocketID,
								JC_UINT8			*pucBuffer,
								JC_UINT32			uiBufLen) ;

/**
 * @brief This function is used for freeing the Push Message Structure.
 * @param[in]	vPushHandle Specifies the Push Handle.
 * @param[in]	pstPushMsg Specifies the Http Push Message Structure.
 * @note		The parameter which should not be NULL are vPushHandle.
 * 
 * This function is used for freeing the Push Message Structure.
 * This Structure internaly contains Header, Buffer and content type,
 * which also gets free after calling this fucntion.
 */
void jdi_HTTPPushFreeMsg (JC_HANDLE				vPushHandle,
						  ST_HTTP_PUSH_MSG		*pstPushMsg) ;

/**
 * @brief This function is used for DeInitializing the Http Push Handle.
 * @param[in]	vPushHandle, Specifies the Push Handle.
 * @retval		JC_OK On success.
 * @retval		Others
 * @see			jdi_HTTPPushTOInitialize
 * @note		The parameter which should not be NULL are vPushHandle.
 * 
 * This function is used for DeIntializing the Push Handle.
 * It Frees the Transaction List, Decoding Process, CPI tags list and
 * every memory which ever is allocated during the function call.
 */
JC_RETCODE jdi_HTTPPushDeinitialize (JC_HANDLE	vPushHandle) ;

/**
 * @brief Specifies the error code if there is mismatch in the CPI tag of deivce and recieved from PPG/Server.
 */ 
#define JC_ERR_HTTPPUSH_CPI_MISMATCH			TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x01)
/**
 * @brief Specifies the error code if buffer doesn't contain \wappush.
 */ 
#define JC_ERR_HTTPPUSH_NOT_WAPPUSH				TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x02)
/**
 * @brief Specifies the error code if request type is other than POST or OPTIONS.
 */ 
#define JC_ERR_HTTPPUSH_UNKNOWN_REQUEST			TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x03)
/**
 * @brief Specifies the error code if not enough memory is available.
 */ 
#define JC_ERR_HTTPPUSH_INSUFFICIENT_MEMORY		TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x04)
/**
 * @brief Specifies the error code if buffer is not valid and it doesn't contain the required values for parsing.
 */ 
#define JC_ERR_HTTPPUSH_BUFFER_NOT_VALID		TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x05)
/**
 * @brief Specifies the error code if still more content is to be recieved.
 */ 
#define JC_ERR_HTTPPUSH_RECV_CONTINUE			TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x06)
/**
 * @brief Specifies the error code if still more content is left for sending. 
 */ 
#define JC_ERR_HTTPPUSH_SEND_CONTINUE			TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x07)
/**
 * @brief Specifies the error code if recieved only partial buffer. 
 */ 
#define JC_ERR_HTTPPUSH_RECV_PARTIAL_HEADER		TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x08)
/**
 * @brief Specifies the error code if there is no transaction node correspoding to socket id. 
 */ 
#define JC_ERR_HTTPPUSH_TRANSACTION_NOT_FOUND	TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x09)
/**
 * @brief Specifies the error code if content buffer is not valid and it doesn't contain the required values for parsing.. 
 */ 
#define JC_ERR_HTTPPUSH_INVALID_CONTENT			TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x0A)
/**
 * @brief Specifies the error code if recieve buffer is of less than or equal to Zero. 
 */ 
#define JC_ERR_HTTPPUSH_BUFFER_INSUFFICIENT		TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x0B)
/**
 * @brief Specifies the error code if after recieving actual content recieve is Zero.
 */ 
#define JC_ERR_HTTPPUSH_NO_RECV_CONTENT			TERMINAL_ERR_CODE(HTTPPUSH_MODULE, 0x0C)

#endif


/* End of File */


