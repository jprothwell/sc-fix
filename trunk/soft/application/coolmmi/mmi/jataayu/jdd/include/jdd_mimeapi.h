/***************************************************************************
 * $Id: jdd_mimeapi.h,v 1.2 2006/05/02 11:19:21 kumardevjtbase Exp $
 * $Revision: 1.2 $
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
 * $Log: jdd_mimeapi.h,v $
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.4  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.2  2005/03/24 10:57:50  kumardevhtmlbrow
 * Changed as per the coding standard
 *
 *  
 ***************************************************************************/

#ifndef _JDD_MIME_H_
#define _JDD_MIME_H_

/**
 * @file jdd_mimeapi.h
 * @ingroup Mime
 * @brief Defines the function to handle the unknown scheme or content.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief This structure contains the mime information.
 */
typedef struct st_mime_info
{
	JC_INT8			*psContentType ; /**< Specifies the content type */
	JC_INT8			*pBuffer ; /**< Specifies the content that is associated with the given URL */
	JC_INT32		iBufferLen ; /**< Specifies the size of the buffer */
	JC_CHAR			*psUrl ; /**< Specifies the URL, from whether this content is downloaded */
	JC_CHAR			*psFileName ; /**< Specifies the filename, if available. If filename is not available, then application need to get the suitable filename either from the URL or from user. */
	JC_BOOLEAN		bFreeContent ; /**< Specifies the flag whether the memory that are referred in this structure should be freed */
} JC_MIME_INFO ;

/** 
 * @brief This function is used to pass the unknown content to the specific application.
 * @param[in] pstMimeInfo Specifies the mime information for the unknown content.
 *
 * When the application does not support the downloaded mime content, it needs to pass the 
 * content to the external mime handler. The external MIME handler can pass it on to the 
 * the specific application or inform the user for storing in the appropriate location.
 * The jdd_MimeHandler should handle the received content type and buffer to 
 * save/install/play/etc, according to the content type. 
 * This routine should not block the application from doing any other action. 
 * For example, jdd_MimeHandler should not block the application by displaying the modal 
 * dialog box. 
 */
JC_RETCODE jdd_MimeHandler (JC_MIME_INFO *pstMimeInfo) ;

/**
 * @brief This function is used to pass the unknown scheme handler. 
 * @param[in] pmGet Specifies the URL.
 * @param[in] pcPost Specifies the user data associated with the URL, if any.
 * @param[in] iPostLen Specifies the size of the post data.
 * This function is invoked by the application when it does not process the specific scheme. 
 * Example mailto:, tel:, so that this API can pass this information to the specific function.
 */
JC_RETCODE jdd_SchemeHandler (JC_CHAR *pmGet, JC_INT8 *pcPost, JC_INT32 iPostLen) ;

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


