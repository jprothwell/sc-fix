/***************************************************************************
 * $Id: jdi_cutils_judge_content.h,v 1.1 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.1 $
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
 * $Log: jdi_cutils_judge_content.h,v $
 * Revision 1.1  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.1  2008/06/12 06:01:00  kavithadevhtmlbrow
 * Judgement of Content
 *
 *  
 ***************************************************************************/

/*
 * @file jdi_cutils_judge_content.h
 * @ingroup CommonUtils
 * @brief It contains declaration of utility functions used to handle judgment of content.
 *
 * The file contains the declarations of the utility functions that can be used to handle
 * the judgment of content. Judgment will be done based on the starting data in the content.
 */

#ifndef _JDI_CUTILS_JUDGE_CONTENT_H
#define _JDI_CUTILS_JUDGE_CONTENT_H

#ifdef __cplusplus
extern "C" {
#endif

/*** Function Prototypes			***/

/**
 * @brief This function judges the content based on the start of the content
 * @param[in]	vMemHandle		Handle used in memory operations
 * @param[in]	pcContentType	Content type received from the server
 * @param[in]	pstHeaders		MimeHeader list
 * @param[in]	pmURL			Request URL
 * @param[in]	pucContent		Content received from the server
 * @param[out]	ppcType			Content type found using the judgment method
 * @retval		JC_OK	Found the content type by checking the start of the content
 * @retval		JC_ERR_DE_UNKNOWN_CONTENT_TYPE	Unknown content
 *
 * This function checks the given content type and if it is not text/plain then it 
 * checks the starting of the content and determines the content type. If it is text/plain 
 * it checks the extension of the file in the content-disposition header or the extension 
 * of the URL and determines the content type. If the content type is not determined using 
 * the extension then it checks the starting of the data.
 */
JC_RETCODE jdi_CUtilsJudgeContent (JC_HANDLE		vMemHandle,
								   JC_INT8			*pcContentType,
								   ST_MIME_HEADERS	*pstHeaders,
								   JC_CHAR			*pmURL,
								   JC_UINT8			*pucContent,
								   JC_INT8			**ppcType) ;

/**
 * @brief This function handles the plain text by checking the extesion
 * @param[in]	vMemHandle	Handle used in memory operations
 * @param[in]	pstHeaders	MimeHeader list
 * @param[in]	pmURL		Request URL
 * @param[in]	pucContent	Content used in judging content type
 * @param[out]	ppcType		Content type found using the judgment method
 * @retval		JC_OK		Found the content type
 * @retval		JC_ERR_DE_UNKNOWN_CONTENT_TYPE	Not able to determine the content type
 */
JC_RETCODE jdi_CUtilsJudgePlainText (JC_HANDLE			vMemHandle,
									 ST_MIME_HEADERS	*pstHeaders,
									 JC_CHAR			*pmURL,
									 JC_UINT8			*pucContent,
									 JC_INT8			**ppcType) ;

/**
 * @brief This function judges the content type using the starting content
 * @param[in]	vMemHandle	Handle used in memory operations
 * @param[in]	pucContent	Content used in judging content type
 * @param[out]	ppcType		Content type found using the judgment method
 * @retval		JC_OK		Found the content type
 * @retval		JC_ERR_DE_UNKNOWN_CONTENT_TYPE	Not able to determine the content type
 */
JC_RETCODE jdi_CUtilsJudgeContentType (JC_HANDLE	vMemHandle,
									   JC_UINT8		*pucContent,
									   JC_INT8		**ppcType) ;


#ifdef __cplusplus
}
#endif

#endif

/* End of File */

