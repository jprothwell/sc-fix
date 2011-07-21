/***************************************************************************
 * $Id: jdi_httpencoder.h,v 1.3 2008/01/15 11:57:45 browserdevjtbase Exp $
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
 * $Log: jdi_httpencoder.h,v $
 * Revision 1.3  2008/01/15 11:57:45  browserdevjtbase
 * Updated the jtbase.
 *
 * Revision 1.10  2008/01/02 14:47:29  sriramdevhtmlbrow
 * Updated documentation of files
 *
 * Revision 1.9  2007/12/28 12:29:14  kumardevhtmlbrow
 * Doxygen comment updated.
 *
 * Revision 1.8  2006/04/28 07:36:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2006/01/17 15:33:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2005/12/12 15:29:02  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.5  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.4  2005/08/24 15:25:57  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/07/02 14:56:53  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/05/21 11:20:49  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.1  2005/05/10 07:56:42  kumardevhtmlbrow
 * Added HTTP Utils function
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_httpencoder.h
 * @ingroup HTTP Encoder Decoder
 * @brief Defines the exposed funciton prototypes for HTTP Encoder.
 *
 * It provides an API that can be used to parse the given mime headers and 
 * form a buffer.
 */


#ifndef _JDI_HTTP_ENCODER_H
#define _JDI_HTTP_ENCODER_H

#include <jdi_mimeinfo.h> 
#include <jdi_transportmodule.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HTTP_ENCODER_ENABLE
/*** Function Prototypes			***/

/**
 * @brief This function encodes the given data.
 * @param[in]		pstMimeHeaders	Contains information which 
 *					should be decoded.
 * @param[in,out]	pucBuffer	This will store the encoded data.
 * @param[in,out]	puiBufLen	Specifies the buffer length.
 * @return The status of the encoding process.
 * @retval	JC_OK	Specifies that the encoding operation is successful.
 * @retval	JC_ERR_CFG_INSUFFICIENT_MEMORY	Specifies that the buffer memory 
 *			is insufficient to store the data.
 *
 * This function parses the given structure and stores it in the variabe 
 * pucBuffer if the given buffer length is greater than or equal to the size 
 * of the given structure data.It also stores the size of the written buffer 
 * in puiBufLen.If pucBuffer is NULL then it stores the required memory size 
 * in the variable puiBufLen and returns JC_ERR_CFG_INSUFFICIENT_MEMORY 
 * as the error code.
 */
JC_RETCODE jdi_HTTPEncHeaders (const ST_MIME_HEADERS	*pstMimeHeaders,
							   JC_UINT8					*pucBuffer,
							   JC_UINT32				*puiBufLen) ;

/** 
 * @brief Specifies that the given memory is not sufficient.
 */
#define JC_ERR_HTTPENC_INSUFFICIENT_MEMORY		TERMINAL_ERR_CODE(HTTP_UTILS_MODULE, 0x11)

/**
 * @brief Specifies that the given mime headers are not valid.
 */
#define JC_ERR_HTTPENC_INVALID_HEADERS			TERMINAL_ERR_CODE(HTTP_UTILS_MODULE, 0x12)

#endif

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

