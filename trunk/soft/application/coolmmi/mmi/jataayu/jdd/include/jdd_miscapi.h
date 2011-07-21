/***************************************************************************
 * $Id: jdd_miscapi.h,v 1.7 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.7 $
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
 * $Log: jdd_miscapi.h,v $
 * Revision 1.7  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.6  2007/03/20 06:20:58  browserdevjtbase
 * Update to comments
 *
 * Revision 1.5  2007/01/12 11:18:05  browserdevjtbase
 * Update from the main base
 *
 * Revision 1.4  2006/12/21 07:24:39  browserdevjtbase
 * Update
 *
 * Revision 1.3  2006/11/29 13:09:24  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.6  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2005/11/25 13:17:50  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.4  2005/10/15 10:17:07  kumardevhtmlbrow
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


#ifndef _JDD_MISCAPI_H
#define _JDD_MISCAPI_H

/** 
 * @file jdd_miscapi.h
 * @ingroup MISC
 * @brief Defines common utility functions.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Defines the extension and its associated content type. It is used 
 * for determining content type based on extension and vice versa.
 */
typedef struct stMimeExtension
{
	const JC_INT8		*pcExtension ;
	const JC_INT8		*pcContentType ;
} ST_MIME_EXTENSION ;


/**
 * @brief Defines the string having few starting bytes of a content and 
 * its associated content type. It is used for content judgment.
 */
typedef struct stContentJudgment
{
	const JC_INT8		*pcStartBytes ;
	const JC_INT8		*pcContentType ;
} ST_CONTENT_JUDGMENT ;

/**
 * @brief This function gets the appropriate content type for 
 *		  given filename extension.
 * @param[in] pmFileName File name for which to get the content type
 * @param[out] ppcContentType The content type for the filename
 * @return void
 */
void	jdd_MiscGetContentTypeForExt (const JC_CHAR		*pmFileName, 
									  const JC_INT8		**ppcContentType) ;

/**
 * @brief This function gets the extension to apply to the filename
 *        for storing the data of given content type.
 * @param[in] pcContentType Type of the given content
 * @param[out] ppcExtension Extension to apply for the content
 * @return void
 */
void jdd_MiscGetExtForContentType	(const JC_INT8		*pcContentType, 
									 const JC_INT8		**ppcExtension) ;

/**
 * @brief Checks the few starting bytes of the given content and gets the content type
 * @param[in]	pucContent		Specifies the content to be judged
 * @param[out]	ppcContentType	Holds the output content type
 * @return void
 */
void jdd_MiscGetContentTypeForData (const JC_UINT8		*pucContent,
									const JC_INT8		**ppcContentType) ;

/** 
 * @brief This function returns floor of the double value.
 * @param[in] dValue	Input value
 */
JC_DOUBLE jdd_MiscFloor (JC_DOUBLE dValue) ;

/**
 * @brief This function return the ceil of the double value.
 * @param[in] dValue	Input value
 */
JC_DOUBLE jdd_MiscCeil (JC_DOUBLE dValue) ;

/**
 * @brief This function returns the pow value of dValueX over dValueY.
 * @param[in] dValueX	Input value
 * @param[in] dValueY	Input value
 */
JC_DOUBLE jdd_MiscPower (JC_DOUBLE dValueX, JC_DOUBLE dValueY) ;

/**
 * @brief This function returns the unique value.
 */
JC_UINT64 jdd_MiscGetUniqueValue (void) ;

/*
JC_RETCODE jddex_PlatformCreateInputBox(JC_INPUT_BOX  stInputBox, 
								   ERenderingStyle eRenderingStyle,JC_CHAR* pszInputText);


*/
	JC_INT8 * jdd_strncat(JC_INT8 * pStr1, const JC_INT8 *pStr2, JC_UINT32 Count);

/**
 * @brief This function resets the watchdog timer. 
 * This function is invoked by the application where it consumes the CPU for more time and needs to reset the watchdog timer in order to avoid the watchdog assertion.
 */
void jdd_MiscWatchDogReset (void) ;


JC_DOUBLE jc_atof (const JC_INT8 *pcStr1) ;
#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


