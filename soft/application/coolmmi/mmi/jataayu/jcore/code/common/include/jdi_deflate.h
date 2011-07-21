/***************************************************************************
 * $Id: jdi_deflate.h,v 1.6 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.6 $
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
 * $Log: jdi_deflate.h,v $
 * Revision 1.6  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.6  2007/07/04 12:16:02  kumardevhtmlbrow
 * Added the CPLUSPLUS header.
 *
 * Revision 1.5  2007/06/28 08:04:46  kumardevhtmlbrow
 * Deflate added under the common #ifdef DEFLATE_ENABLE.
 *
 * Revision 1.4  2007/01/17 13:01:56  kumardevhtmlbrow
 * Interface change in the deflate. Updated the browser engine and deflate module.
 *
 * Revision 1.3  2007/01/04 14:10:07  kumardevhtmlbrow
 * Changed the parameter for defalte initialize
 *
 * Revision 1.2  2007/01/02 13:08:26  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.1  2006/06/14 07:27:24  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

#ifndef _JDI_DEFLATE_H
#define _JDI_DEFLATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <jdi_commonmodule.h>

#ifdef DEFLATE_ENABLE

typedef struct _deflate_info
{
	JC_UINT8			*pucData ;
	JC_UINT32			uiDataLength ;
	JC_BOOLEAN			bHasMoreData ;
} ST_DEFLATE_INFO ;

/**
 * @brief Initializes the deflate engine
 *
 * @param[in] uiBufferLen	Specifies the size of the compressed buffer length which is going to be uncompressed. 
 *							If size is not known, it shall be specified as zero.
 * @param[in] cbFncPtr		Specifies the callback function pointer, which is used by the deflate engine for passing
 *							the uncompressed content to the application.
 * @param[in] pvCBArg		Specifies the callback argument which is passed back to the application when invoking the 
 *							the callback function.
 * @param[out] pvHandle		Returns the reference to the deflate engine, which needs to be passed while passing the compressed data.
 */
JC_RETCODE jdi_DeflateExInitialize (JC_UINT32				uiBufferLen,
									JC_INT8					*pcEncodingValue,
									JC_UINT32				uiMaxUnCompressLen,
									JC_HANDLE				*pvHandle) ;		

/**
 * @brief Sets the compressed data and returns the decompressed data.
 * 
 * @param[in] vHandle			Specifies the handle returned by the #jdi_DeflateInitialize
 * @param[in] pstDeflateIn		Specifies the compressed data, data length and also specifies whether any more data available.
 * @param[out] pstDeflateOut	Returns the uncompressed data, data length and also specifies whether more data available or not.
 * 
 * When partial data is uncompressed this function returns #JC_ERR_DEFLATE_MORE_DATA, the application needs to call #jdi_DeflateExContentMore 
 * to uncompress the remaining data.
 */
JC_RETCODE jdi_DeflateExContent (JC_HANDLE				vHandle,
								 ST_DEFLATE_INFO		*pstDeflateIn,
								 ST_DEFLATE_INFO		*pstDeflateOut) ;	  

/**
 * @brief Returns the decompressed data.
 * 
 * @param[in] vHandle			Specifies the handle returned by the #jdi_DeflateInitialize
 * @param[out] pstDeflateOut	Returns the uncompressed data, data length and also specifies whether more data available or not.
 */
JC_RETCODE jdi_DeflateExContentMore (JC_HANDLE				vHandle,
									 ST_DEFLATE_INFO		*pstDeflateOut) ;	  

/**
 * @brief Deinitialize the deflate engine
 *
 * @param[in] vHandle		Specifies the handle returned by the #jdi_DeflateInitialize
 */
JC_RETCODE jdi_DeflateExDeinitialize (JC_HANDLE			vHandle) ;	


#define JC_ERR_DEFLATE_METHOD		TERMINAL_ERR_CODE(DEFLATE_MODULE, 0x01)	  
#define JC_ERR_DEFLATE_ERROR		TERMINAL_ERR_CODE(DEFLATE_MODULE, 0x02)	  
#define JC_ERR_DEFLATE_EOF			TERMINAL_ERR_CODE(DEFLATE_MODULE, 0x03) 
#define JC_ERR_DEFLATE_MORE_DATA	TERMINAL_ERR_CODE(DEFLATE_MODULE, 0x04) 
#define JC_ERR_DEFLATE_NO_DATA		TERMINAL_ERR_CODE(DEFLATE_MODULE, 0x05) 

#endif

#ifdef __cplusplus
}
#endif

#endif


