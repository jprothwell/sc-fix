/***************************************************************************
 * $Id: jdi_punycode.h,v 1.2 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.2 $
 * $Date: 2008/06/16 05:52:17 $
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
 * $Log: jdi_punycode.h,v $
 * Revision 1.2  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.2  2007/03/22 07:24:25  shreyasdevhtmlbrow
 * Updated the code review comments.
 *
 * Revision 1.1  2007/02/16 10:38:53  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

#ifndef _JDI_PUNYCODE_H
#define _JDI_PUNYCODE_H

#ifdef PUNYCODE_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief		Initializes the punycode converter (PC)
 *
 * @param[in]	vMemHandle		Specifies the memory handle that is to be used by the PC
 * @param[in]	uiMaxPCURI		Specifies the maximum number of converted URI that can be stored by the PC. 
 * @param[out]	pvHandle		The API returns the PC handle in this field. The application
 *								needs to use this handle to use the other PC APIs.
 * @retval		JC_OK if the PC is initialized successfully.
 * @retval		JC_ERR_MEMORY_ALLOCATION in case of memory allocation error
 * 
 * The application needs to use this API to initialize the PC. The application needs to 
 * pass the memory handle for the PC and the maximum number of converted URI's that can be 
 * stored by the PC. By storing the converted URI the PC can avoid encoding the URI's which are
 * accessed regularly.
 */
JC_RETCODE jdi_PCInitialize (JC_HANDLE		vMemHandle,
							 JC_UINT32		uiMaxPCURI,
							 JC_HANDLE		*pvHandle) ;

/**
 * @brief		DeInitializes the punycode converter (PC)
 *
 * @param[in]	vHandle	 Specifies the PC handle which was returned by the PC initialize API.
 * @retval		JC_OK if the PC is deinitialized successfully.
 *
 * The application needs to use this API to deinitialize the PC. 
 */
JC_RETCODE jdi_PCDeInitialize (JC_HANDLE		vHandle) ;

/**
 * @brief		Performs Punycode Conversion of the given URI
 *
 * @param[in]	vHandle			Specifies the PC handle that was returned by PC initialize API.
 * @param[in]	pmURI			Specifies the URI to be converted.
 * @param[out]	ppucEncodedURI	The function returns the encoded URI in this field.
 * @retval		JC_OK if the URI is encoded successfully.
 * @retval		JC_ERR_PC_INVALID_DATA if the URI to be converted is invalid.
 * @retval		JC_ERR_PC_NOT_REQUIRED if the URI does not need to be encoded and can be used as it
 *				is (ppucEncodedURI will be returned as JC_NULL).
 * @retval		JC_ERR_PC_BIG_OUTPUT if the URI to be converted is too big and will result into big output.
 * @retval		JC_ERR_PC_OVERFLOW if an overflow of the encoded buffer happening while performing punycode conversion
 * 
 * The application needs to use this API to perform punycode conversion of a given URI (only hostname).
 * The API will first check if any conversion of the URI hostname is required or not. If it not required it will return
 * an error and the application can use the same URI for its operations. Else the API will convert
 * the URI and send the encoded URI back to the application.
 */

JC_RETCODE jdi_PCEncodeURI (JC_HANDLE			vHandle,
							JC_CHAR				*pmURI,
							JC_UINT8			**ppucEncodedURI) ;

/**
 * @brief		Frees the Encoded URI
 *
 * @param[in]	vHandle			Specifies the PC handle that was returned by PC initialize API.
 * @param[in]	pucEncodedURI	Specifies the encoded URI to be freed.
 * @retval		void
 * 
 * The application needs to use this API to free the encoded URI which was previously returned
 * by the API jdi_PCEncodeURI.
 */
void jdi_PCFreeEncodedURI (JC_HANDLE		vHandle,
						   JC_UINT8			*pucEncodedURI) ;

/**
 * @brief The error code is returned if the URI to be converted does not have a proper host name
 */
#define JC_ERR_PC_INVALID_DATA			TERMINAL_ERR_CODE(PUNYCODE_MODULE, 0x01)

/**
 * @brief The error code is returned if the URI sent for punycode conversion does not need
 *		  any conversion. i.e. the URI can be used as it is for its operations.
 */
#define JC_ERR_PC_NOT_REQUIRED			TERMINAL_ERR_CODE(PUNYCODE_MODULE, 0x02)

/**
 * @brief The error code is returned if the punycode conversion of the URI will result into 
 *		  big output.
 */
#define JC_ERR_PC_BIG_OUTPUT			TERMINAL_ERR_CODE(PUNYCODE_MODULE, 0x03)

/**
 * @brief The error code is returned if while converting the output buffer has overflowed.
 */
#define JC_ERR_PC_OVERFLOW				TERMINAL_ERR_CODE(PUNYCODE_MODULE, 0x04)

#ifdef __cplusplus
}
#endif

#endif /** PUNYCODE_ENABLE **/

#endif

/* End of File */
