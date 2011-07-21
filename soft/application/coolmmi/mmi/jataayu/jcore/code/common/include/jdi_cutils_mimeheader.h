/***************************************************************************
 * $Id: jdi_cutils_mimeheader.h,v 1.4 2008/06/16 05:52:17 browserdevjtbase Exp $
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
 * $Log: jdi_cutils_mimeheader.h,v $
 * Revision 1.4  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.1  2006/07/11 05:23:16  kumardevhtmlbrow
 * update the common function which does the header operation
 *
 *  
 ***************************************************************************/

/*
 * @file jdi_cutils_mimeheader.h
 * @ingroup CommonUtils
 * @brief It contains declaration of utility functions used to handle the mime headers.
 *
 * The file contains the declarations of the utility functions that can be used to handle
 * the mime header information. Such as copying and freeing the mime header information.
 */

#ifndef _JDI_CUTILS_MIMEHEADER_H
#define _JDI_CUTILS_MIMEHEADER_H

#ifdef __cplusplus
extern "C" {
#endif

/*** Function Prototypes			***/

/**
 * @brief	  Copies Mime header information
 *
 * @param[in] vMemHandle Specifies the memory handle to be used for copying the mime header
 *			  information.
 * @param[in] pstSrcMimeHeaders Specifies the mime header to be copied
 * @param[out] ppstDestMimeHeaders The function will copy the mime header and return 
 *			   it in this field.
 * @retval	  JC_OK if the mime header is copied successfully.
 * @retval	  JC_ERR_MEMORY_ALLOCATION if a memory error occurs.
 *
 * The application needs to use this function to copy a mime header information. Based on the
 * source mime header information the function will form a linked list of mime header and return
 * in the destination mime header.
 */

JC_RETCODE jdi_CUtilsCopyMimeHeaders (JC_HANDLE			vMemHandle,
									  ST_MIME_HEADERS	*pstSrcMimeHeaders,
									  ST_MIME_HEADERS	**ppstDestMimeHeaders) ;

/**
 * @brief	  Frees Mime header information
 *
 * @param[in] vMemHandle Specifies the memory handle which was used while forming the mime headers
 * @param[in] pstMimeHeaders Specifies the mime header to be freed
 * @retval	  void
 *
 * The application needs to use this function Free a mime header information.
 */

void jdi_CUtilsFreeMimeHeaders (JC_HANDLE		vMemHandle,
								ST_MIME_HEADERS *pstMimeHeaders) ;

#ifdef __cplusplus
}
#endif

#endif

/* End of File */
