/***************************************************************************
 * $Id: jdi_httpdecoder.h,v 1.3 2008/01/15 11:57:45 browserdevjtbase Exp $
 * $Revision: 1.3 $
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
 * $Log: jdi_httpdecoder.h,v $
 * Revision 1.3  2008/01/15 11:57:45  browserdevjtbase
 * Updated the jtbase.
 *
 * Revision 1.18  2008/01/02 14:47:29  sriramdevhtmlbrow
 * Updated documentation of files
 *
 * Revision 1.17  2007/12/28 12:29:14  kumardevhtmlbrow
 * Doxygen comment updated.
 *
 * Revision 1.16  2006/04/28 07:36:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.15  2006/04/22 07:38:18  kumardevhtmlbrow
 * Transport logging updated
 *
 * Revision 1.14  2006/01/17 15:33:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.13  2005/12/12 15:29:02  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.12  2005/09/17 11:58:39  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.11  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.10  2005/08/24 15:25:57  kumardevhtmlbrow
 * no message
 *
 * Revision 1.9  2005/08/17 08:21:45  kumardevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/08/17 06:22:05  kumardevhtmlbrow
 * Updated the jdd_MemAlloc to jdi_MemAlloc function.
 *
 * Revision 1.7  2005/07/29 15:31:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/07/28 14:39:42  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/07/02 14:56:53  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/05/21 11:20:49  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.3  2005/05/18 10:26:02  kumardevhtmlbrow
 * HTTP Utility function updated ...
 *
 * Revision 1.2  2005/05/13 06:22:53  kumardevhtmlbrow
 * Added the decoder info header
 *
 * Revision 1.1  2005/05/10 07:56:42  kumardevhtmlbrow
 * Added HTTP Utils function
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_httpdecoder.h
 * @ingroup HTTP Encoder Decoder
 * @brief Defines the exposed fucntion prototypes for HTTP Decoder.
 *
 * Different APIs have been provided that can be used to initialize the 
 * decoder module, parse all headers or a particular header, search for a 
 * given header, separate the given http response into headers and buffer,
 * parse the given chunked response and deinitialize the decoder module.
 */


#ifndef _JDI_HTTP_DECODER_H
#define _JDI_HTTP_DECODER_H

#include <jdi_mimeinfo.h>
#include <jdi_transportmodule.h>

#ifdef __cplusplus
extern "C" {
#endif

/***	Macros			***/

/**
 * @brief Specifies that some more data needs to be received from the server.
 */
#define JC_ERR_HTTPDEC_PARTIAL_CONTENT		TERMINAL_ERR_CODE(HTTP_UTILS_MODULE, 0x01)

/** 
 * @brief Specifies that the buffer received from the server is invalid.
 */
#define JC_ERR_HTTPDEC_INVALID_CONTENT		TERMINAL_ERR_CODE(HTTP_UTILS_MODULE, 0x02)

/**
 * @brief Specifies that the header data received from the server is partial.
 */
#define JC_ERR_HTTPDEC_PARTIAL_HEADER		TERMINAL_ERR_CODE(HTTP_UTILS_MODULE, 0x03)

#ifdef HTTP_DECODER_ENABLE
/**
 * @brief Specifies that the given header is not found in the list.
 */
#define JC_ERR_HTTPDEC_HEADER_NOT_FOUND		TERMINAL_ERR_CODE(HTTP_UTILS_MODULE, 0x04)

/*** Function Prototypes			***/

/**
 * @brief This function initializes the http decoder module.
 * @param[in]	pucBuffer	Buffer that will be used in the decoding process
 * @param[in]	uiLength	Length of the buffer
 * @param[in]	bUseMemory	Specifies whether to use the same buffer memory 
 *				or to allcate memory.
 * @param[in]	vMemHandle	Handle that will be used in the memory operations
 * @param[out]	pvHandleFF	Contains the ST_DECODER_INFO structure inforamtion 
 *				which will be used by other decoder module functions.
 * @retval		JC_OK	Specifies that the initialization process was successful
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error
 * @see			jdi_HTTPDecDeInitialize
 * @note		The parameters pucBuffer and pvHandleFF should not be NULL.
 * @note		The parameter pvHandleFF should be passed to all 
 *				other jdi functions of HTTP Decoder module.
 *
 * This function will be called to initialize the http decoder module. It 
 * stores the given buffer in ST_HTTPUTILS_DECODER_INFO which will be used 
 * in the decoding process.
 */
JC_RETCODE jdi_HTTPDecInitialize (JC_UINT8		*pucBuffer, 
								  JC_UINT32		uiLength, 
								  JC_BOOLEAN	bUseMemory, 
								  JC_HANDLE		vMemHandle, 
								  JC_HANDLE		*pvHandleFF) ;
/**
 * @brief This function parsed the given buffer and forms mime header list
 * @param[in]	vHandle		Handle to the decoder module, it contains the 
 *				buffer that has to be parsed
 * @param[in]	bParseParameter		Specifies whether to parse the 
 *				parameter list also or not.
 * @param[out]	ppstMimeHeaders	Holds the mime header structure that is formed.
 * @retval		JC_OK	On success
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error
 * @note		The parameters vHandle and ppstMimeHeaders should not be NULL.
 * @note		The calling function should not free the header list ppstMimeHeaders,
 *				it will be freed during deinitialization
 *
 * This function will be called to decode header data and form the header list.
 * This function uses the buffer stored in vHandle and parses it. If the variable 
 * bParseParameter is true then parameter list of a header also will be parsed.
 */
JC_RETCODE jdi_HTTPDecAllHeaders (JC_HANDLE			vHandle, 
								  JC_BOOLEAN		bParseParameter,
								  ST_MIME_HEADERS	**ppstMimeHeaders) ;

/**
 * @brief This function is used to search for a header in the header list
 * @param[in]	vHandle		Handle to the decoder module, 
 *				it contains the mime header list
 * @param[in]	pcHeaderName	Specifies the header name which has to be 
 *				searched in the given mime header list structure
 * @param[out]  ppstMimeHeader	Holds the reference to the mime header 
 *				that is found in the search
 * @retval		JC_OK On Success
 * @note		The parameters vHandle, pcHeaderName and ppstMimeHeaders should not be NULL.
 * @note		The mime header list need not be passed to this function as 
 *				it is already present in the vHandle.
 * This function will be called to search for a header. It uses the 
 * header list stored in vHanlde.
 */
JC_RETCODE jdi_HTTPDecSearchHeader (JC_HANDLE			vHandle, 
									const JC_INT8		*pcHeaderName, 
									ST_MIME_HEADERS		**ppstMimeHeader) ;

/**
 * @brief This function deinitializes the decoding process.
 * @param[in]	vHandle	Contains the ST_DECODER_INFO structure information 
 *				which should be deleted.
 * @return	The status of the deinitialization process. 
 * @retval	JC_OK	Specifies that the deinitialization process is successful.
 *
 * This function deletes the structure ST_DECODER_INFO and its members.
 * If the member variable bFreeBuffer is E_TRUE then it will delete 
 * the buffer, otherwise it will not delete the buffer.It will also delete the 
 * chunked and dynamic memory.
 */
JC_RETCODE jdi_HTTPDecDeInitialize (JC_HANDLE vHandle) ;
#endif

/**
 * @brief This function decodes the buffer on chunk basis.
 * @param[in]		pucBuffer	Specifies the buffer that has to be decoded.	   
 * @param[in]		uiLength	Length of the buffer that has to be decoded.
 * @param[in]	vMemHandle	Handle that will be used in the memory allocation
 * @param[in,out]	puiRemChunkLength	Specifies the length of the remaining 
 *					buffer that needs to be decoded.
 * @param[out]		ppucDecodedBufF	Holds the decoded buffer.
 * @param[out]		puiDecodedBufLength	Holds the size of the decoded buffer.		
 * @param[out]		ppcTempBuf				Stores the additional buffer that is 
	received from the server.
 * @param[out]		piTempBufLen			Length of the additional buffer.
 * @return The status of the decoding.
 * @retval	JC_OK	Specifies that the chunked decoding is successful.
 * @retval	JC_ERR_HTTPDEC_PARTIAL_CONTENT	Specifies that some more buffer is 
 *			there to be received.
 * @retval  JC_ERR_HTTPDEC_INVALID_CONTENT Specifies that the given buffer is 
 *			not a valid one.
 *
 *	This function parses the buffer pucBuffer on a chunk basis.Here uiLength 
 *	specifies the length of pucBuffer.The value *puiRemChunkLength specifies 
 * the length of the buffer that needs to be decoded.If *puiRemChunkLength is 
 * greater than uiLength, it specifies that some more buffer is there to be 
 * received.Every time the *puiRemChunkLength will be updated with the length 
 * of the remaining buffer that needs to be processed.
 */
JC_RETCODE jdi_HTTPDecChunkedBuf (JC_UINT8		*pucBuffer, 
								  JC_UINT32		uiLength, 
								  JC_HANDLE		vMemHandle,
								  JC_UINT32		*puiRemChunkLength, 
								  JC_UINT8		**ppucDecodedBufF, 
								  JC_UINT32		*puiDecodedBufLength, 
								  JC_INT8		**ppcTempBuf, 
								  JC_INT32		*piTempBufLen) ;

/**
 * @brief This function separates the header and buffer information from the 
 *		  given buffer.
 * @param[in]	pucHTTPBuffer	Buffer that needs to be processed.
 * @param[in]	uiHTTPBufLength	Length of the buffer pucHTTPBuffer.
 * @param[in]	vMemHandle	Handle that will be used in the memory allocation
 * @param[out]	ppucHeaders		Holds the header information.
 * @param[out]  puiHeaderLength	Holds the length of ppucHeaders
 * @param[out]  ppucBuffer		Holds the buffer information.
 * @param[out]  puiBufferLength	Holds the length of ppucBuffer.
 * @param[out]  piStatus		Holds the status of the received response 
 *								pucHTTPBuffer.
 * @return The status of the parsing. 
 * @retval	JC_OK	Specifies that the given buffer is parsed properly.
 * @retval	JC_ERR_HTTPDEC_INVALID_CONTENT	Specifies that the given buffer 
 *			is an invalid one.
 * @retval	JC_ERR_HTTPDEC_PARTIAL_HEADER	Specifies that the header 
 *			information in the given buffer is incomplete.
 *
 * This function parses the given buffer pucHTTPBuffer and separates the 
 * header and buffer information which will be stored in ppucHeaders and 
 * ppucBuffer respectively.Their lengths will be stored in *puiHeaderLength 
 * and *puiBufferLength respectively.The status of the given buffer 
 * pucHTTPBuffer will be stored in *piStatus.
 */
JC_RETCODE jdi_HTTPDecHTTPBuffer (JC_UINT8		*pucHTTPBuffer, 
								  JC_UINT32		uiHTTPBufLength, 
								  JC_HANDLE		vMemHandle,
								  JC_UINT8		**ppucHeaders, 
								  JC_UINT32		*puiHeaderLength, 
								  JC_UINT8		**ppucBuffer, 
								  JC_UINT32		*puiBufferLength, 
								  JC_INT32		*piStatus) ;

/**
 * @brief	This function is used to parse the given buffer for a particular header.
 * @param[in]	vMemHandle	Handle that will be used in the memory operations
 * @param[in]	pucBuffer	Contains the Header information
 * @param[in]	uiLength	Length of the Header information
 * @param[in]	pcName		Specific header name that should be parsed
 * @param[out]	ppcValue	Holds the string of header values
 * @retval		JC_OK	Specifies that the given specific header is parsed 
 *				successfully and the header values are formed
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error
 * @note		None of the parameters except vMemHandle should be NULL.
 * @note		To use this function, decoder module need to be initilaized
 * @note		The calling function should free the variable ppcValue
 *
 * This function will be called to parse the given header data for a 
 * particular header.
 */
JC_RETCODE jdi_HTTPDecSpecificHeader (JC_HANDLE		vMemHandle, 
									  JC_UINT8		*pucBuffer, 
									  JC_UINT32		uiLength,
									  const JC_INT8	*pcName, 
									  JC_INT8		**ppcValue) ;


#ifdef __cplusplus
}
#endif

#endif

/* End of File */

