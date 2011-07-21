/***************************************************************************
 * $Id: jdi_sia.h,v 1.3 2008/01/15 11:57:45 browserdevjtbase Exp $
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
 * $Log: jdi_sia.h,v $
 * Revision 1.3  2008/01/15 11:57:45  browserdevjtbase
 * Updated the jtbase.
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.8  2006/03/08 12:29:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2005/12/31 06:46:08  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2005/12/12 15:29:02  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.5  2005/12/09 15:15:16  kumardevhtmlbrow
 * Added the suspend and resume request. Providing additional interface for starting confirmed & http push. Changes as per new JDD LOG abstraction
 *
 * Revision 1.4  2005/10/27 15:55:20  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.2  2005/08/30 05:59:54  kumardevhtmlbrow
 * Added the SIA Module
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_sia.h
 * @ingroup SIA.
 * @brief Defines the enums, structures and the exposed functions of SIA. 
 * 
 * The file defines the functions, enums and structures that the application has to use in order
 * to parse a SIA content. 
 */


#ifndef _JDI_SIA_H
#define _JDI_SIA_H

/*** Nested Include Files			***/

#include <jdi_transportmodule.h>

/*** Macros							***/

#define SIA_MAX_HOSTNAME			255
#define SIA_MAX_IP_NAME				15
#define SIA_MAX_APPLICATION_ID		20

/*** Type definitions				***/

/**
 * @brief The SIA module specifies the WSP and HTTP contact point information in this structure
 */

typedef struct _sia_contact_points_
{
	E_DATA_CONN_BEARER_TYPE				eDataConnBearerType ; /**< Specifies the bearer type */
	JC_INT8								acIPAddress [SIA_MAX_IP_NAME + 1] ; /**< Specifies the IP address */
	JC_UINT32							uiPortNumber ; /**< Specifies the port number */
	/** Have to add another field for protocol options. **/
	struct _sia_contact_points_			*pstNext ; /**< Specifies the pointer to the next Contact point node */
} ST_SIA_CONTACT_POINTS ;

/**
 * @brief The SIA module parse the raw SIA buffer and returns the parsed information in the following
 *		  structure.
 */

typedef struct _sia_info_
{
	JC_UINT32						auiApplicationIdList [SIA_MAX_APPLICATION_ID + 1] ; /**< Specifies the List of Application which has registered for the SIA */
	ST_SIA_CONTACT_POINTS			*pstSIAWspContactPoints ; /**< Specifies the WSP contact points */
	ST_SIA_CONTACT_POINTS			*pstSIAContactPoints ; /**< Specifies the HTTP contact points */
	JC_UINT32						uiProvUrlLen ; /**< Specifies the length of the Prov URL */
	JC_INT8							*pcProvUrl ; /**< Specifies the Prov URL */
	JC_UINT32						uiCPITagLen ; /**< Specifies the length of the CPI tag */
	JC_INT8							*pcCPITagLen ; /**< Specifies the CPI tag */ 
} ST_SIA_INFO ;

/*** Function Prototypes			***/


/**
 * @brief	  The application needs to use this function to parse a SIA content
 * @param[in]	pucBuffer Specifies the SIA content to be parsed.
 * @param[in]	uiBufferLen Specifies the length of the SIA buffer.
 * @param[out]	pstSIAInfo The parsed SIA information will be returned in this field.
 * @param[out]	puiNoOfAppIds The number of applications who have registered for the SIA.
 * @retval		JC_OK if the SIA content is parsed successfully.
 * @retval		JC_ERR_MEMORY_ALLOCATION Speicifies memory allocation error.
 * @note		The parameter pucBuffer, pstSIAInfo, puiNoOfAppIds should not be NULL
 * @see			jdi_SIAFreeContent
 *
 * The application needs to use this function to parse a raw SIA content. The function
 * will return the parsed SIA content and the number of applications which have registered 
 * for the SIA. If no application has registered for the SIA content then this function will
 * specify a default application ID in the application list.
 */

JC_RETCODE	jdi_SIAParseContent (JC_UINT8					*pucBuffer,
								 JC_UINT32					uiBufferLen,
								 ST_SIA_INFO				*pstSIAInfo,
								 JC_UINT32					*puiNoOfAppIds) ;

/**
 * @brief	  The application needs to use this function to free a parsed SIA information.
 * @param[in,out]	Specifies the SIA content to be freed.
 * @retval		void
 * @note		The parameter pstSiaInfo should not be NULL
 *
 * When the jdi_SIAParseContent  parses any SIA buffer it returns the SIA information in the ST_SIA_INFO
 * structure. The application needs to use this function to free the SIA information.
 */

void jdi_SIAFreeContent (ST_SIA_INFO			*pstSiaInfo) ;

#endif

/* End of File */


