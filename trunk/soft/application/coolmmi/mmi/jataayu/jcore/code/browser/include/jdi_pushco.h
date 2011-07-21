 /***************************************************************************
 * $Id: jdi_pushco.h,v 1.12 2006/05/26 10:04:11 kumardevhtmlbrow Exp $
 * $Revision: 1.12 $
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
 * $Log: jdi_pushco.h,v $
 * Revision 1.12  2006/05/26 10:04:11  kumardevhtmlbrow
 * Updated....
 *
 * Revision 1.11  2006/05/11 07:20:10  kumardevhtmlbrow
 * Updated the interface documentation ...
 *
 * Revision 1.10  2006/03/08 12:29:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.9  2006/03/01 14:57:58  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2005/12/12 15:29:01  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.7  2005/09/12 12:20:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/08/12 10:57:42  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/06/24 10:40:50  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/06/23 09:49:45  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/06/15 11:25:42  kumardevhtmlbrow
 * no message
 *
 * Revision 1.1  2005/06/11 08:02:31  kumardevhtmlbrow
 * Added the push handler prototype
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_pushco.h
 * @ingroup PushHandler
 * @brief Defines the exposed enumerations, structures and APIs of the Push CO handler
 * that can be used by the browser application.
 * 
 * This file defines all the enumerations, structures and APIs that the application need
 * to use in order to parse a CO buffer. The API will parse the raw CO buffers and return
 * the CO in the form of a structure to the application.
 */

#ifndef _JDI_PUSHCO_H
#define _JDI_PUSHCO_H


#include <jdi_browsermodule.h>
#include <jdi_xml.h>
#include <jdi_wbxml.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The enumeration contains the types of cache operations.
 */
typedef enum _push_co_type
{
	E_PUSH_CO_INVALIDATE_OBJECT, /**< Specifies that the cache operation is to invalidate an object */
	E_PUSH_CO_INVALIDATE_SERVICE /**< Specifies that the cache operation is to invalidate a service */
} E_PUSH_CO_TYPE ;

/**
 * @brief The raw CO buffer is parsed by the push handler module and returned to the application
 *		  in the this structure.
 */
typedef struct _push_co_info
{
	JC_INT8					*pcURI ; /**< Specifies the URL of the Cache operation */
	E_PUSH_CO_TYPE			eCoType ; /**< Specifies the type of cache operation */
	struct _push_co_info	*pstNext ; /**< Specifies the pointer to the next CO information */
} ST_PUSH_CO_INFO ;

/**
 * @brief The error code is returned if the message to be parsed is invalid.
 */
#define JC_ERR_PUSH_CO_INVALID_DOCUMENT		TERMINAL_ERR_CODE(PUSH_MODULE, 0x10)

#define TEXT_VND_WAP_CO				"text/vnd.wap.co"
#define APPLICATION_VND_WAP_COC		"application/vnd.wap.coc"

/**
 * @brief		Parses the CO buffer. 
 *
 * @param[in]	pucBuffer Specifies the CO buffer to be parsed.
 * @param[in]	iLength Specifies the length of the CO buffer.
 * @param[in]	pcContentType Specifies the content type of the CO buffer.
 * @param[in]	vMemHandle Specifies the memory handle to be used by the push handler module.
 * @param[out]	ppstPushCOInfo Once the API parses the buffer it returns the parsed content in this structure.
 * @retval		JC_OK if the message is parsed sucessfully.
 * @retval		JC_ERR_INVALID_PARAMETER if invalid content type is specified.
 * @retval		JC_ERR_MEMORY_ALLOCATION if a memory error occurs.
 * @retval		JC_ERR_PUSH_CO_INVALID_DOCUMENT if the buffer to be parsed is invalid.
 * @see			jdi_PushCOFree
 *
 * The application needs to use this API to parse the CO buffer. 
 * The API will parse the content and return the CO information in the form of a 
 * structure (ST_PUSH_CO_INFO) to the application.
 *
 * The appplication needs to use the API jdi_PushCOFree to free the ST_PUSH_CO_INFO
 * which is retunred by this API.
 */

JC_RETCODE jdi_PushCOParseMessage (JC_UINT8			*pucBuffer,
								   JC_INT32			iLength,
								   JC_INT8			*pcContentType,
								   JC_HANDLE		vMemHandle,
								   ST_PUSH_CO_INFO	**ppstPushCOInfo) ;

/**
 * @brief		Frees a cache operation structure
 *
 * @param[in]	pstPushCOInfo Specifies the CO information to be freed.
 * @param[in]	vMemHandle Specifies the memory handle used by the push handler
 * @retval		void
 *
 * The API jdi_PushCOParseMessage returns the ST_PUSH_CO_INFO structure. 
 * Once the application has finished using the structure it needs to use this 
 * API to free the same.
 */

void jdi_PushCOFree (ST_PUSH_CO_INFO *pstPushCOInfo, JC_HANDLE vMemHandle) ;

#ifdef __cplusplus
}
#endif

#endif


