/***************************************************************************
 * $Id: jdi_drm.h,v 1.8 2009/03/04 13:04:13 kavithadevhtmlbrow Exp $
 * $Revision: 1.8 $
 * $Date: 2009/03/04 13:04:13 $
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
 * $Log: jdi_drm.h,v $
 * Revision 1.8  2009/03/04 13:04:13  kavithadevhtmlbrow
 * Doxygen Comments Updated
 *
 * Revision 1.7  2007/05/31 16:02:56  kumardevhtmlbrow
 * Doxygen comment update.
 *
 * Revision 1.6  2007/05/31 14:56:14  shreyasdevhtmlbrow
 * Doxygen Comment updated
 *
 * Revision 1.5  2007/05/31 12:05:22  shreyasdevhtmlbrow
 * Doxygen comment updated
 *
 * Revision 1.4  2006/07/17 12:53:20  kavithadevhtmlbrow
 * Updated For DRM
 *
 * Revision 1.3  2006/07/06 05:20:10  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.2  2006/07/05 08:53:44  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.1  2006/04/28 09:29:34  kavithadevhtmlbrow
 * DRM Initial Version
 *
 *  
 ***************************************************************************/

/** 
 * @page drm Integration with DRM Engine
 *
 * Digital rights management (DRM) is an umbrella term referring to technologies used by publishers
 * or copyright owners to control access to or usage of digital data or hardware, and to restrictions
 * associated with a specific instance of a digital work or device. The basic function is to 
 
 * when, how often, for how long, and under which conditions.
 *
 * OMA defines the OMA DRM, which provide mechanisms for secure authentication of trusted 
 * DRM Agents, and for secure packaging and transfer of usage rights and DRM Content to
 * trusted DRM Agents. 
 *
 * How the rights objects are obtained is not part of the scope of the browser, which needs to
 * be handled by the DRM engine. Jataayu does not provide the DRM engine, where as 
 * browser core provides mechanism to integrate with the DRM engine if it is available 
 * on the platform. 
 *
 * The browse is capable of handling the DRM protected inline content. The browser passes the DRM protected 
 * content to the application for parsing the DRM content. The application needs to integrate
 * with the native "DRM Engine" and provide the parsed content with the rights to the browser core.
 * @note If the main page is DRM protected then it will be given as unknown content handler to the
 * application and will not be handled internally by the browser engine.
 * 
 * The following are the data-types that are used for the DRM integration:
 * @li #ST_DRM_EXT_CONTENT
 * @li #ST_DRM_EXT_CONTENT_MORE
 * @li #ST_DRM_EXT_CANCEL
 * @li #ST_DRM_EXT_FREE
 * @li #ST_DRM_EXT_PARSED_CONTENT
 * @li #ST_DRM_EXT_PARSED_CONTENT_MORE
 * @li #ST_DRM_EXT_RIGHTS_UPDATE
 * 
 * The below diagram describes the basic flow diagram how DRM protected inline content is passed to the 
 * application and used. <BR><BR>
 * @image html drm1.jpg 
 * <BR><BR>
 * The below diagram describes the flow diagram how the DRM request is cancelled during the DRM parsing. <BR><BR>
 * @image html drm2.jpg
 * <BR><BR>
 * The below diagram describes the flow diagram how the application can update the DRM rights. <BR><BR>
 * @image html drm3.jpg
 * <BR>
 */ 


/**
 * @file jdi_drm.h
 * @ingroup BE
 * @brief Defines the exposed enumerations and structures for processing DRM Content.
 * 
 * This file defines the various structures and enumnerations that are used for the 
 * parsing DRM content.
 */

#ifndef _JDI_DRM_H
#define _JDI_DRM_H

#include <jdd_drmcb.h>

typedef struct st_drm_ext_mime_content
{
	JC_CHAR				*pmURI ; /**< Specifies the URL for the DRM Content object */
	JC_INT8				*pcContentType ; /**< Specifies the content type */	
	E_DRM_MIME_TYPE		eDataType ;  /**< Specifies the type in which content is sent */ 
	U_DRM_MIME_DATA		uData ; /**< Specifies whether content of the page is specified in buffer or in a file */
	JC_UINT32			uiBufTotalLen ; /**< Specifies the total buffer length (if known) for the DRM parsed content */
	JC_UINT32			uiBufCurrLen ; /**< Specifies the length for the current chunk of DRM parsed content buffer */
	ST_MIME_HEADERS		*pstHeaders ; /**< Specifies the headers for the object */	
	JC_BOOLEAN			bHasMoreData ; /**< Specifies whether more data is to be received */
	JC_BOOLEAN			bUseMemory ; /**< Specifies whether memory needs to be allocated or not while copying the parsed buffer */
} ST_DRM_EXT_MIME_CONTENT ;

/**
 * @brief The browser uses the following structure to send the DRM content 
 *        to the application.
 */
typedef struct st_drm_ext_content		
{
	ST_DRM_EXT_MIME_CONTENT		stContent ; /**< Specifies the page content */
	JC_UINT32					uiDRMRefID ; /**< Specifies the DRM reference ID returned by the application */
} ST_DRM_EXT_CONTENT ;

/**
 * @brief This structure defines the mime information
 */
typedef struct st_drm_ext_content_more
{
	JC_UINT32			uiDRMRefID ;		/**< Specifies the Reference ID for the DRM content */
	JC_UINT8			*pucBuffer ;		/**< Specifies the DRM current chunk */
	JC_UINT32			uiBufferLen ;		/**< Specifies the current DRM chunk length */
	JC_BOOLEAN			bHasMoreData ;		/**< Specifies whether more data is to be received */
} ST_DRM_EXT_CONTENT_MORE ;


/**
 * @brief This structure contains the parsed DRM content received from the application
 */
typedef struct st_drm_parsed_content
{
	JC_UINT32					uiDRMRefID ; /**< Specifies the Reference ID for the DRM content */
	JC_RETCODE					rCode ; /**< Specifies the return code for the DRM parsing by the application */
	JC_INT8						*pcContentType ;
	ST_DRM_RIGHTS_OBJECT		*pstRightsObject ; /**< Specifies the rights object information. */
	ST_DRM_MIME_CONTENT			*pstDRMContent ; /**< Specifies the output data after parsing the DRM message. */
} ST_DRM_EXT_PARSED_CONTENT ;


/**
 * @brief This structure contains the parsed DRM content received from the application
 */
typedef struct st_drm_ext_parsed_content_more
{
	JC_UINT32				uiDRMRefID ; /**< Specifies the Reference ID for the DRM content */
	JC_UINT8				*pucBuffer ;  /**< Specifies the buffer (current chunk) */
	JC_UINT32				uiBufCurrLen ;  /**< Specifies the buffer lenght */
	JC_BOOLEAN				bHasMoreData ;  /**< Specifies the end of content or more to be recieved */
	JC_RETCODE				rCode ; /**< Specifies the return code for the DRM parsing by the application */
} ST_DRM_EXT_PARSED_CONTENT_MORE ;

/**
 * @brief This structure contains the DRM information that needs to be cancelled.
 */
typedef struct st_drm_ext_cancel
{
	JC_UINT32				uiDRMRefID ; /**< Specifies the Reference ID for the DRM content, which needs to be cancelled from further processing. */
} ST_DRM_EXT_CANCEL ;

/**
 * @brief This structure contains the 
 */
typedef struct st_drm_ext_free_info
{
	JC_UINT32				uiDRMRefID ; /**< Specifies the reference ID for the DRM content, for which any memory that are associated should be freed. */
} ST_DRM_EXT_FREE ;

/**
 * @brief This structure contains the
 */
typedef struct st_drm_ext_rights_update
{
	JC_UINT32				uiDRMRefID ; /** Specifies the reference ID for the DRM content. */
	ST_DRM_RIGHTS_OBJECT	*pstRightsObject ; /**< Specifies the rights object that is associated with the given DRM reference ID. */
} ST_DRM_EXT_RIGHTS_UPDATE ;

#endif

