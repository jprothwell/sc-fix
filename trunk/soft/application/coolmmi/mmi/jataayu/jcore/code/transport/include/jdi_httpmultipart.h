/***************************************************************************
 * $Id: jdi_httpmultipart.h,v 1.7 2009/02/27 14:06:24 browserdevjtbase Exp $
 * $Revision: 1.7 $
 * $Date: 2009/02/27 14:06:24 $
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
 * $Log: jdi_httpmultipart.h,v $
 * Revision 1.7  2009/02/27 14:06:24  browserdevjtbase
 * Core Updates
 *
 * Revision 1.14  2009/02/25 12:29:24  sriramdevhtmlbrow
 * Changed the error code values as the same values are used for http encoder utils.
 *
 * Revision 1.13  2008/05/20 09:17:53  kumardevhtmlbrow
 * Added the transport module header file.
 *
 * Revision 1.12  2008/01/03 11:22:40  sriramdevhtmlbrow
 * Updated documentation and removed macros which are not used
 *
 * Revision 1.11  2008/01/02 14:47:29  sriramdevhtmlbrow
 * Updated documentation of files
 *
 * Revision 1.10  2007/07/13 10:11:18  kavithadevhtmlbrow
 * Multipart As and When Decoding
 *
 * Revision 1.9  2006/06/28 07:56:00  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2006/01/17 15:33:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2005/12/12 15:29:02  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.6  2005/09/26 15:57:29  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.4  2005/09/08 15:05:25  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/08/31 08:11:53  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/08/30 13:17:03  kumardevhtmlbrow
 * no message
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_httpmultipart.h
 * @ingroup HTTP
 * @brief Defines the macros and exposed funciton prototypes 
 *	for HTTP Multipart.
 *
 * APIs are provided to parse the given multipart response to form the 
 * multipart buffers with their header list and free the given multipart 
 * structure.
 */

#ifndef _JDI_HTTP_MULTIPART_H
#define _JDI_HTTP_MULTIPART_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HTTP_MULTIPART_ENABLE

#include <jdi_transportmodule.h>
#include <jdi_mimeinfo.h>

/**
 * @brief	Specifies the multipart information structure that will be used as input parameter 
 *			for multipart decode APIs. This will contain the multipart buffer details.
 */
typedef struct _http_multipart_in
{
	JC_UINT8			*pucBuffer ; /**< Specifies the multipart buffer */
	JC_UINT32			uiLength ; /**< Specifies the length of the multipart buffer */
	JC_INT8				*pcContentType ; /**< Specifies the content type of the multipart page */
	JC_BOOLEAN			bHasMoreData ; /**< Specifies whether more data has to be received from server */
} ST_HTTP_MULTIPART_IN ;

/**
 * @brief	Specifies the multipart information structure that will be used as out parameter 
 *			for multipart decode APIs. This will contain the parsed multipart details which 
 *			will be given to application.
 */
typedef struct _http_mp_out
{
	JC_UINT8			*pucBuffer ; /**< Specifies the output buffer */
	JC_UINT32			uiLength ; /**< Specifies the length of the output buffer */
	JC_INT8				*pcContentType ; /**< Specifies the content type of the output buffer */
	ST_MIME_HEADERS		*pstHeaders ; /**< Specifies the mime headers associated with the output buffer */
	JC_BOOLEAN			bHasMoreResp ; /**< Specifies whether more response is there for this same multipart node */
	struct _http_mp_out	*pstNext ; /**< Specifies the next node */
} ST_HTTP_MULTIPART_OUT ;

/**
 * @brief Specifies that the given multipart data is not valid.
 */
#define JC_ERR_HTTP_MULTIPART_INVALID_DATA		TERMINAL_ERR_CODE(HTTP_UTILS_MODULE, 0x20)

/**
 * @brief Specifies that there is more multipart response for the given chunk.
 */
#define JC_ERR_HTTP_MULTIPART_MORE_DATA			TERMINAL_ERR_CODE(HTTP_UTILS_MODULE, 0x21)

/**
 * @brief Specifies that there is no more multipart response for the given chunk.
 */
#define JC_ERR_HTTP_MULTIPART_OK				TERMINAL_ERR_CODE(HTTP_UTILS_MODULE, 0x22)

/*** Global Function Prototypes	***/

/**
 * @brief This function parses the given x-multipart buffer, which is an mrt page.
 * @param[in]	vMemHandle		Handle used in the memory operations
 * @param[in]	pucBuffer		Multipart buffer that should be parsed
 * @param[in]	uiLength		Length of the buffer
 * @param[out]	ppstMultipart	Holds the parsed multipart buffers and headers
 * @retval		JC_OK	Specifies that the given buffer is parsed successfully
 * @retval		JC_ERR_HTTP_MULTIPART_INVALID_DATA	Specifies that the given 
 *				multipart data is invalid.
 * @see			jdi_HTTPMultipartFree
 * @note		The parameters pucBuffer and ppstMultipart should not be NULL.
 *
 * This function will be called to parse the given multipart data which is.
 * a buffer of an mrt page. This function parses the given data and forms 
 * the multipart buffers with their header list.
 */
JC_RETCODE jdi_HTTPMultipartDecode (JC_HANDLE			vMemHandle,
									JC_UINT8			*pucBuffer,
									JC_UINT32			uiLength,
									ST_MULTIPART_INFO	**ppstMultipart) ;

/**
 * @brief This function parses the given multipart buffer.
 * @param[in]	vMemHandle		Handle used in the memory operations
 * @param[in]	pcBuffer		Multipart buffer that should be parsed
 * @param[in]	iLength			Length of the buffer
 * @param[in]	pcContentType	Content type of the multipart buffer.
 * @param[out]	ppstMultipart	Holds the parsed multipart buffers and headers
 * @retval		JC_OK	Specifies that the given buffer is parsed successfully
 * @retval		JC_ERR_HTTP_MULTIPART_INVALID_DATA	Specifies that the given 
 *				multipart data is invalid.
 * @see			jdi_HTTPMultipartFree
 * @note		The parameters pucBuffer and ppstMultipart should not be NULL.
 *
 * This function will be called to parse the given multipart data.
 * This function parses the given data and forms the multipart buffers
 * with their header list.
 */
JC_RETCODE jdi_HTTPMultipartDecodeContent (JC_HANDLE			vMemHandle,
										   JC_INT8				*pcBuffer,
										   JC_INT32				iLength,
										   JC_INT8				*pcContentType, 
										   ST_MULTIPART_INFO	**ppstMultipart) ;

/**
 * @brief This function frees the given multipart structure information
 * @param[in]	vMemHandle		Handle used in the memory operations
 * @param[in]	pstMultipart	Structure containing the multipart buffers and headers.
 * @return		This function does not return any value.
 * @note		The parameter pstMultipart should not be NULL.
 *
 * This function will be called to free the given multipart structure.
 */
void jdi_HTTPMultipartFree (JC_HANDLE			vMemHandle,
							ST_MULTIPART_INFO	*pstMultipart) ;

/**
 * @brief This function initializes the multipart parsing process.
 * @param[in]	vMemHandle			Handle used in the memory operations
 * @param[out]	pvMultipartHandle	Holds the MultipartHandle,which has 
 *				to be passed to other multipart APIs.
 * @retval		JC_OK	On Success
 * @retval		JC_ERR_MEMORY_ALLOCATION	On memory allocation failure.
 * @see			#jdi_HTTPMultipartDeinitializeEx
 *
 * This function will be called to initialize the multipart parsing process.
 */
JC_RETCODE jdi_HTTPMultipartInitializeEx (JC_HANDLE		vMemHandle,
										  JC_HANDLE		*pvMultipartHandle) ;

/**
 * @brief This function parses the given multipart buffer, which can be partial.
 * @param[in]	vMultipartHandle	Multipart Handle.
 * @param[in]	pstMPIn				Contains the multipart buffer information.
 * @param[out]	pstMPOut			Holds the parsed multipart information.
 * @retval		JC_OK	Specifies that there is no more response to be received, 
 *						i.e. end of the whole multipart response.
 * @retval		JC_ERR_HTTP_MULTIPART_INVALID_DATA	Specifies that the given 
 *				multipart data is invalid.
 * @retval		JC_ERR_HTTP_MULTIPART_MORE_DATA	Specifies that there is more 
 *				multipart response to be received.
 * @retval		JC_ERR_HTTP_MULTIPART_OK	Specifies that there is no  
 *				more multipart response to be received for the current buffer.
 * @see			#jdi_HTTPMultipartDecodeContentMoreEx
 *
 * This function will be called to parse multipart buffer, and this buffer can be partial.
 * The multipart buffer information will be given in the structure pstMultipartIn and 
 * the parsed output will be filled in the structure pstMultipartOut. If there is more 
 * response to be given to the application for a particular multipart node then 
 * bHasMoreResp will be set in the structure pstMultipartOut. If there are more 
 * multipart nodes to be given to application or more response is present for the same 
 * multipart node then JC_ERR_HTTP_MULTIPART_MORE_DATA will be returned. If there are no 
 * more multipart nodes to be given to application for the given buffer then 
 * JC_ERR_HTTP_MULTIPART_OK will be returned. To receive more data in any case 
 * the application has to call #jdi_HTTPMultipartDecodeContentMoreEx. If there is no 
 * more response to be received for the whole multipart then JC_OK will be returned.
 */
JC_RETCODE jdi_HTTPMultipartDecodeContentEx (JC_HANDLE				vMultipartHandle,
											 ST_HTTP_MULTIPART_IN	*pstMPIn,
											 ST_HTTP_MULTIPART_OUT	*pstMPOut) ;

/**
 * @brief This function gives the more multipart response to the application.
 * @param[in]	vMultipartHandle	Multipart Handle.
 * @param[out]	pstMPOut			Holds the Multipart response
 * @retval		JC_OK	Specifies that there is no more response to be received, 
 *						i.e. end of the whole multipart response.
 * @retval		JC_ERR_HTTP_MULTIPART_MORE_DATA	Specifies that there is more 
 *				multipart response to be received.
 * @retval		JC_ERR_HTTP_MULTIPART_OK	Specifies that there is no  
 *				more multipart response to be received for the current buffer.
 * @see			#jdi_HTTPMultipartDecodeContentEx
 *
 * This function will be called to receive more multipart response.
 * If there is more response to be given to the application for a particular 
 * multipart node then bHasMoreResp will be set in the structure pstMultipartOut. 
 * If there are more multipart nodes to be given to application or more response 
 * is present for the same multipart node then JC_ERR_HTTP_MULTIPART_MORE_DATA
 *  will be returned. If there are no more multipart nodes to be given to 
 * application for the given buffer then JC_ERR_HTTP_MULTIPART_OK will be returned. 
 * To receive more data in any case the application has to call this function again.
 * If there is no more response to be received for the whole multipart 
 * then JC_OK will be returned.
 */
JC_RETCODE jdi_HTTPMultipartDecodeContentMoreEx (JC_HANDLE				vMultipartHandle,
												 ST_HTTP_MULTIPART_OUT	*pstMultipartOut) ;

/**
 * @brief This function deinitializes the multipart parsing process.
 * @param[in]	vMultipartHandle	Multipart Handle
 * @retval		JC_OK	On Success
 * @see			#jdi_HTTPMultipartInitializeEx
 *
 * This function will be called to deinitialize the multipart parsing process, 
 * once multipart parsing process is completed..
 */
JC_RETCODE jdi_HTTPMultipartDeinitializeEx (JC_HANDLE	vMultipartHandle) ;

#endif

#ifdef __cplusplus
}
#endif

#endif

/* End of File */



