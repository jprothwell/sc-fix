/***************************************************************************
 * $Id: jdi_wbxml.h,v 1.6 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.6 $
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
 * $Log: jdi_wbxml.h,v $
 * Revision 1.6  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.12  2007/04/19 12:59:12  kumardevhtmlbrow
 * Update the WBXML for taking the tag and attribute name from the calling function through callback.
 *
 * Revision 1.11  2006/01/09 09:58:05  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.10  2005/12/12 15:29:01  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.9  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/08/23 16:34:37  kumardevhtmlbrow
 * no message
 *
 * Revision 1.7  2005/08/17 06:22:00  kumardevhtmlbrow
 * Updated the jdd_MemAlloc to jdi_MemAlloc function.
 *
 * Revision 1.6  2005/08/12 10:57:42  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/06/23 14:31:05  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/06/20 10:44:26  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/06/08 15:57:35  kumardevhtmlbrow
 * WBXML Initialize Module ...
 *
 * Revision 1.1  2005/05/12 10:34:20  kumardevhtmlbrow
 * Initial version of WBXML Interface
 *
 *  
 ***************************************************************************/

#ifndef _JDI_WBXML_H
#define _JDI_WBXML_H

#include <jdi_xmldatatype.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This structure contains required parameter to decode the wbxml content.
 */
typedef struct wbxmlParam
{
	JC_UINT8				*pucBuffer ;	/**< Specifies reference to the wbxml content */
	JC_UINT32				uiBufferLen ;	/**< Specifies the passed wbxml content length*/
	JC_INT8					*pcContentType ;/**< Specifies the passed wbxml content's type*/
	JC_UINT32				uiBlockSize ;	/**< Specifies the block size, used for chunkbase parsing*/
	JC_BOOLEAN				bHasMoreData ;	/**< Specifies the flag to indicate any further data is there or not*/
	JC_HANDLE				vMemHandle ;	/**< Specifies the Memory Handle.*/
	CB_GET_TAG_ATTR_NAME	cbGetTagAttributeName ; /**< Specifies the callback function. */
	void					*pvCBArg ; /**< specifies the callback argument. */
} ST_WBXML_PARAM ;

/**
 * @brief Specifies the error code when block of content is yet to parsed. 
 */
#define JC_ERR_WBXML_CONTINUE 					TERMINAL_ERR_CODE(WBXML_MODULE, 0x01)
/**
 * @brief Specifies the error code when offset goes beyond boundary. 
 */
#define JC_ERR_WBXML_INVALID_OFFSET				TERMINAL_ERR_CODE(WBXML_MODULE, 0x02)
/**
 * @brief Specifies the error code when current id is wrong. 
 */
#define	JC_ERR_WBXML_INVALID_CURRENT_ID			TERMINAL_ERR_CODE(WBXML_MODULE, 0x03)
/**
 * @brief Specifies the error code when decoder could not able find out attribute value. 
 */
#define	JC_ERR_WBXML_INVALID_ATTRIBUTE_VALUE	TERMINAL_ERR_CODE(WBXML_MODULE, 0x04)
/**
 * @brief Specifies the error code when its not able to get the table reference value. 
 */
#define	JC_ERR_WBXML_INVALID_STRING_TABLE_REF	TERMINAL_ERR_CODE(WBXML_MODULE, 0x05)
/**
 * @brief Specifies the error code when parsed value is not valid UTF8. 
 */
#define	JC_ERR_WBXML_INVALID_UTF8_VALUE			TERMINAL_ERR_CODE(WBXML_MODULE, 0x06)
/**
 * @brief Specifies the error code when its not able find out tag value. 
 */
#define	JC_ERR_WBXML_INVALID_TAG_VALUE			TERMINAL_ERR_CODE(WBXML_MODULE, 0x07)


/**
 * @brief	This function decode the passed the wbxml buffer.
 * @param[in]		pstWbxmlParam	Specifies the WBXML data of parsing.
 * @param[in/out]	pvHandle		Holds the reference when parsing to be continued
 * @param[in/out]	pstXMLTree		Specifies the reference to ST_XML_TREE with
 * previous parsed content (if any), which gets updated in this function.
 * @retval		JC_ERR_WBXML_CONTINUE					Parsing of block is success
 * @retval		JC_ERR_MEMORY_ALLOCATION				Fails to provide memory block
 * @retval		JC_ERR_WBXML_INVALID_TAG_VALUE			Fails to provide respective tag value
 * @retval		JC_ERR_WBXML_INVALID_ATTRIBUTE_VALUE	Fails to provide attribute value
 * @retval		JC_OK									Parsing is complete and success
 * @retval		others									Error while parsing.
 * @note		The parameter which should not be NULL are pvHandle, pstWbxmlParam and pstXMLTree.
 * 
 * This function is called when application wants to decode binary encoded 
 * XML contents into well defined textual form. 
 * The function parsed the passed wbxml buffer and stores the decoded content into the 
 * well defined XML structure.  
 */
JC_RETCODE jdi_WBXMLDecode (const ST_WBXML_PARAM	*pstWbxmlParam, 
							JC_HANDLE				*pvHandle, 
							ST_XML_TREE				*pstXmlTree) ;

/**
 * @brief	This function decode the remaining block of wbxml content if any.
 * @param[in/out]	pstWbxmlParam	Specifies the WBXML data of parsing.
 * @param[in/out]	pvHandle		Holds the reference when parsing to be continued
 * @retval		JC_ERR_WBXML_CONTINUE					Parsing of block is success
 * @retval		JC_ERR_MEMORY_ALLOCATION				Fails to provide memory block
 * @retval		JC_ERR_WBXML_INVALID_TAG_VALUE			Fails to provide respective tag value
 * @retval		JC_ERR_WBXML_INVALID_ATTRIBUTE_VALUE	Fails to provide attribute value
 * @retval		JC_OK									Parsing is complete and success
 * @retval		others									Error while parsing.
 * @see			jdi_WBXMLDecode
 * @note		The parameter which should not be NULL are pvHandle, pstWbxmlParam.
 * 
 * This function is used to parse the remaining block of the wbxml content. 
 * The function parsed the passed wbxml buffer and stores the decoded content into the 
 * well defined XML structure. 
 */
JC_RETCODE jdi_WBXMLUpdate (JC_HANDLE				vHandle, 
							const ST_XML_TREE		*pstXMLTree) ;

#ifdef __cplusplus
}
#endif

#endif


