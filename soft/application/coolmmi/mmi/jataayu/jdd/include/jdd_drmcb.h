/***************************************************************************
 * $Id: jdd_drmcb.h,v 1.1 2006/11/16 11:32:26 browserdevjtbase Exp $
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
 * $Log: jdd_drmcb.h,v $
 * Revision 1.1  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.1  2006/07/06 07:34:20  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2006/05/04 04:05:30  kumarcccjemulator
 * *** empty log message ***
 *
 * Revision 1.5  2006/04/26 07:25:58  kumarcccjemulator
 * Updated comments.
 *
 * Revision 1.4  2006/04/26 06:35:46  kumarcccjemulator
 * for handling push inbox full.
 *
 * Revision 1.3  2006/03/18 08:00:47  kumarcccjemulator
 * *** empty log message ***
 *
 * Revision 1.2  2005/12/19 14:44:11  kumarcccjemulator
 * *** empty log message ***
 *
 * Revision 1.1  2005/12/17 15:06:47  kumarcccjemulator
 * DRM Callback interface update
 *
 *  
 ***************************************************************************/

/**
 * @file jdd_drmcb.h
 * @ingroup DRM 
 * @brief Provides the DRM interface layer for the terminal application for supporting
 * DRM content.
 * @{
 * 
 */

#ifndef _JDD_DRMCB_H
#define _JDD_DRMCB_H

/**
 * @brief This enumeration contains the status code for the DRM message.
 */
typedef enum _drm_status_code
{
	E_DRM_STATUS_OK, /**< Specifies the DRM message can be used for the further processing. */
	E_DRM_STATUS_RIGHT_DURATION_EXPIRED, /**< Specifies the interval in which the privilege should be exercised is expired. */
	E_DRM_STATUS_COUNT_EXCEEDED, /**< Specifies the number of times the privilege has been exercised and now it has exceeded the limit. */
	E_DRM_STATUS_START_TIME_NOT_EXPIRED, /**< Specifies the start time beyond which privilege should be exercised has not reached. */
	E_DRM_STATUS_END_TIME_EXPIRED, /**< Specifies the end time up to which the privilege should be exercised has expired. */
	E_DRM_STATUS_UNKNOWN	/** Specifies the generic DRM error, which is not specified above. */
} E_DRM_STATUS_CODE ;

/**
 * @brief This structure describes the DRM Rights object information.
 */
typedef struct _drm_rights_object
{
	JC_INT8						*pcContentID ; /**< Specifies the unique content id with respect to the message. */
	E_DRM_STATUS_CODE			eStatusCode ; /**< Specifies the status of the specific DRM object. */
	JC_BOOLEAN					bIsRender ; /**< Specifies whether the object can be rendered (played in case of audio/viewed in case of image) */
	JC_BOOLEAN					bIsForwardLock ; /**< Specifies whether this object is protected through forward lock */
	JC_UINT64					uiStartTime ; /**< Specifies the object rights after which it can be rendered. */
	JC_UINT64					uiDuration ; /**< Specifies the duration in which the objects can be rendered. The units specified are in milliseconds. */
	struct _drm_rights_object	*pstNext ; /**< Specifies the next right object node. */				
} ST_DRM_RIGHTS_OBJECT ;

/**
 * @brief This enumeration defines different means of providing the DRM content. 
 */
typedef enum _mime_type
{
	E_DRM_MIME_BUFFER, /**< Specifies the content available as inline memory. */
	E_DRM_MIME_FILENAME /**< Specifies the content available in file. */
} E_DRM_MIME_TYPE ;

/** 
 * @brief This union is used to refer the DRM content either in file or inline memory. 
 */
typedef union _mime_data
{
	JC_UINT8		*pucBuffer ; /**< Specifies the reference to the inline memory content. */
	JC_CHAR			*pmFileName ; /**< Specifies the reference to the file system. */
} U_DRM_MIME_DATA ;

/**
 * @brief This structure defines the mime information.
 */
typedef struct _mime_content
{
	JC_UINT8			*pucHeader ; /**< Specifies the header content (if available) associated with the DRM content. */
	JC_UINT32			uiHeaderLen ; /**< Specifies the header length. */
	JC_BOOLEAN			bIsBinaryHeader ; /**< Specifies the type of the header. E_TRUE specifies the header has been encoded using WSP. */
	E_DRM_MIME_TYPE		eDataType ; /**< Specifies how the content has been stored either in memory or file system. */
	U_DRM_MIME_DATA		uData ;	/**< Specifies the reference to the actual content in memory or file system. */
	JC_UINT32			uiBufferLen ; /**< Specifies the current buffer length. */
	JC_UINT32			uiTotalLen ; /**< Specifies the total size of the content length. */
	JC_BOOLEAN			bHasMoreData ; /**< Specifies whether any more data is pending or completed. */
	JC_BOOLEAN			bUseMemory ; /**< Specifies whether the DRM engine can use the same reference or not when the content is passed in memory. */
} ST_DRM_MIME_CONTENT ;

/**
 * @brief This structure describes the DRM information.
 */
typedef struct _drm_info
{
	ST_DRM_RIGHTS_OBJECT	*pstRightsObject ; /**< Specifies the rights object information. */
	JC_INT8					*pcContentType ; /**< Specifies the DRM mime type */
	ST_DRM_MIME_CONTENT		stMimeContent ; /**< Specifies the mime information after parsing the DRM message. */
} ST_DRM_PARSED_INFO ;

/**
 * @brief The DRM Engine uses this callback to update the DRM related information and 
 * its content to the application after parsing the message. 
 * @param[in] pvAppArg Specifies the application argument passed which passing the indication callback.
 * @param[in] uiDRMRefID Specifies the reference ID associated to the DRM Message.
 * @param[in] pstDRMInfo Specifies the DRM Information.
 */
typedef JC_RETCODE (* CB_DRM_RESPONSE_HANDLER) (void					*pvAppArg,
												JC_UINT32				uiDRMRefID,
												ST_DRM_PARSED_INFO		*pstDRMInfo) ;

/** 
 * @brief The DRM Engine can pass the content to the application as chunk by chunk. 
 * In order to pass the more content the DRM engine shall use this callback.
 * @param[in] pvAppArg Specifies the application argument passed which passing the indication callback.
 * @param[in] uiDRMRefID Specifies the reference ID associated to the DRM Message.
 * @param[in] pucBuffer Specifies the mime content buffer associated to the DRM content.
 * @param[in] uiBufferLen Specifies the length of the mime content
 * @param[in] bHasMoreData Specifies whether more data is available from DRM Engine for the current reference ID. 
 */
typedef JC_RETCODE (* CB_DRM_MORE_CONTENT_HANDLER) (void			*pvAppArg,
													JC_UINT32		uiDRMRefID,
													JC_UINT8		*pucBuffer,
													JC_UINT32		uiBufferLen,
													JC_BOOLEAN		bHasMoreData) ;

/**
 * @brief The application need to register this callback function with the terminal component
 * for parsing the DRM content. 
 * @param[in] pvDRMArg Specifies the DRM argument that has been registered by the DRM Engine 
 *			  with the application while registering this callback.
 * @param[in] pcMimeType Specifies the mime type of the DRM content.
 * @param[in] pstMimeContent Specifies the DRM content information.
 * @param[in] cbResponseHandler Specifies the function reference in the application to the 
 *			  DRM Engine for updating the DRM response if the DRM Engine implements this 
 *			  function as non-blocking call.
 * @param[in] cbMoreContentHandler Specifies the function reference in the application to 
 *			  the DRM Engine for updating the mime content as chunk by chunk to the application. 
 *			  The DRM Engine uses this callback function only if the DRM Indication is implemented 
 *            as non-blocking call.
 * @param[in] pvAppArg Specifies the application argument, which needs to be used by the 
 *			  DRM Engine for the callback function.
 * @param[out] puiDRMRefID Returns the reference identifier for the DRM Messsage. 
 *			   This is applicable only if the DRM Engine implements this function has non-blocking call.
 * @param[out] pstDRMInfo Returns the DRM information for the given DRM Messsage. 
 *			   This is applicable only if the DRM Engine implements this function has blocking call.
 */
typedef JC_RETCODE (* CB_DRM_INDICATION_HANDLER) (void						*pvDRMArg,
												  JC_INT8					*pcMimeType,
												  ST_DRM_MIME_CONTENT		*pstMimeContent,
												  CB_DRM_RESPONSE_HANDLER 	cbResponseHandler,
												  CB_DRM_MORE_CONTENT_HANDLER 	cbMoreContentHandler,
												  void						*pvAppArg,
												  JC_UINT32					*puiDRMRefID,
												  ST_DRM_PARSED_INFO		*pstDRMInfo) ;

/**
 * @brief This function updates the DRM Rights object property by the application.
 * If there are multiple rights object need to be updated, the application need to call 
 * this callback multiple times.
 * @param[in] uiDRMRefID Specifies the reference ID for which the right objects need to be updated.
 * @param[in] pstRightsObject Specifies the rights object information.
 * @param[in] bIsRendered Specifies whether the particular rights object has been rendered or not.
 */

typedef void (* CB_DRM_RIGHTS_UPDATE_HANDLER) (JC_UINT32				uiDRMRefID,
											   ST_DRM_RIGHTS_OBJECT		*pstRightsObject,
											   JC_BOOLEAN				bIsRendered) ;

/**
 * @brief This function cancels the specified DRM reference identifier from parsing. 
 * @param[in] uiDRMRefID Specifies the reference ID for which the DRM reference need to be freed.
 * Once this function is called by the application, the DRM engine should not send any response for the specified DRM reference ID.
 */
typedef void (* CB_DRM_INDICATION_CANCEL_HANDLER) (JC_UINT32			uiDRMRefID) ;

#endif

/** @}  **/


/* End of File */

