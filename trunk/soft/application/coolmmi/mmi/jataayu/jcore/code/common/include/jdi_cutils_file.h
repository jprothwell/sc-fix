/***************************************************************************
 * $Id: jdi_cutils_file.h,v 1.4 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.4 $
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
 * $Log: jdi_cutils_file.h,v $
 * Revision 1.4  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.2  2007/01/29 14:14:21  kumardevhtmlbrow
 * Symbian compilation update
 *
 * Revision 1.1  2006/04/19 07:22:11  kavithadevhtmlbrow
 * Initial Version
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_cutils_file.h
 * @ingroup commonutils
 * @brief Provides the set of functions for file name utility functions.
 * 
 * Functions for different operations such as getting the file name, getting the
 * extension based on content type and forming the file name with the path.
 */

#ifndef _JDI_CUTILS_FILE_H
#define _JDI_CUTILS_FILE_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief This function gets the file name based on disposition name or URL.
 * @param[in]  vMemHandle	 Handle used in memory operations
 * @param[in]  pmURI		 Specifies the URI
 * @param[in]  pcContentType Specifies the content type to get the extension
 * @param[in]  pcDispsnName  Specifies the Content-Disposition name
 * @param[in]  pcDefFileName Specifies the Default filename. 
 * @param[out] ppcName		 Reference to hold the file name 
 * @param[out] puiNameLen	 Reference to hold the file name length
 * @retval     JC_OK	                Success on getting the file name
 * @retval     JC_ERR_MEMORY_ALLOCATION	On memory allocation error
 * @note       Any one of pmURI, pcDispsnName and pcDefFileName should not be NULL
 * @note       Both output parameter ppcName and puiNameLen should not be NULL
 * @note       Caller function has to free the memory allocated for ppcName
 *
 * This function gets the file name when found in content disposition, if not 
 * parses the URL to get the file name and gets the extension based on content type
 * if URL is NULL or not able to get the file name from URL, then uses the default
 * name and extension to get the file name.
 */
JC_RETCODE jdi_CUtilsFileGetName (JC_HANDLE			vMemHandle, 
								  JC_CHAR			*pmURI,
								  JC_INT8			*pcContentType,
								  JC_INT8			*pcDispsnName,
								  JC_INT8			*pcDefFileName,
								  JC_INT8			**ppcName, 
								  JC_UINT32			*puiNameLen) ;


/**
 * @brief This function finds the extension based on the content type
 * @param[in]  pcContentType Specifies the Content type 
 * @param[out] pcExt		 Reference to hold the extension
 * @param[out] puiExtLen	 Reference to hold the extension length
 * @return     No return value
 * @note       All parameter should not be NULL.
 * 
 * This function finds the extension by parsing the content type passed.
 */
void jdi_CUtilsFileFindExtension (const JC_INT8		*pcContentType, 
								  JC_INT8		*pcExt, 
								  JC_UINT32		*puiExtLen) ;


/**
 * @brief This function forms the full file name based on the path and file name.
 * @param[in]  vMemHandle 	   Handle used for memory operations.
 * @param[in]  pcPathSeparator Specifies the path separator.
 * @param[in]  pmPath	       Specifies the path
 * @param[in]  pmFileName      Specifies the file name
 * @param[out] ppmFullName	   Reference to hold the full filename..
 * @retval JC_OK                        Success on forming 
 * @retval JC_ERR_FILE_SYS_INIT         Error initializing file handle
 * @retval JC_ERR_MEMORY_ALLOCATION     Memory Allocation error
 
 * @note   All parameter except vMemHandle should not be NULL
 * @note   Caller function should free the memory allocated for ppmFullName
 * 
 * This function forms full file name with path and filename and checks for the 
 * existence. If exists, adds a number to the file name and again forms full name 
 * Now checks with the new file name. Repeats the step until the file does not
 
 */
JC_RETCODE jdi_CUtilsFileFormName (JC_HANDLE		vMemHandle, 
								   JC_INT8			*pcPathSeparator,
								   JC_CHAR			*pmConfigPath,
								   JC_CHAR			*pmReqFile, 
								   JC_CHAR			**ppmFileName) ;


#ifdef __cplusplus
}
#endif

#endif


