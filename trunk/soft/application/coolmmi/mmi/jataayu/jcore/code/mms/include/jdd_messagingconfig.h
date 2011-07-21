/**
 * @file  jdd_MessagingConfig.h
 * @brief Contains prototypes of message config APIs and declarations 
 *        of config Data structures/enumerations.
 */
/***************************************************************************
*
* File Name		: jdd_MessagingConfig.h
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
*
* File Description
* ----------------
*
* Purpose			:
*
*
*
* 
* Created By		: 
* Created Date		: 
*
* Current Revision	:
*
*
***************************************************************************
*
*
* Revision Details
* ----------------
* 
* 1. Modified By, Modified Date, Purpose
* 2. Modified By, Modified Date, Purpose
* 3. Modified By, Modified Date, Purpose
*
*
*
***************************************************************************/


#ifndef _JDD_MESSAGING_CONFIG_H
#define _JDD_MESSAGING_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup config_module Configuration Module
 *  @{
 */

/***************************************************************************
* Type Definitions
**************************************************************************/

/**
 * @brief Enumeration of presentation types
 */
typedef enum _Config_Presentation_Type
{
	E_CONFIG_PRESENTATION_SMIL = 0,/**< Specifies SMIL type presentation*/
	E_CONFIG_PRESENTATION_XHTML,/**< Specifies XHTML type presentation*/
	E_CONFIG_PRESENTATION_NONE/**< Specifies that presentation type is NONE*/

}E_PRESENTATION_TYPE;

/**
 * @brief Enumeration of messaging types
 */
typedef enum _Config_Messaging_Type
{
	E_CONFIG_MSG_MMS = 0, /**< Specifies Messaging type as MMS */
	E_CONFIG_MSG_EMAIL, /**< Specifies Messaging type as EMail */
	E_CONFIG_MSG_EMS /**< Specifies Messaging type as EMS */

}E_CONFIG_MSG_TYPE;

/**
 * @brief Enumeration of alignment type
 *
 * This enumeration can be used to specify alignment of
 * text/media object
 */
typedef enum _Config_Alignment
{
	E_CONFIG_RIGHT_ALIGNMENT = 0,/**< Specifies right alignment*/
	E_CONFIG_LEFT_ALIGNMENT,/**< Specifies left alignment*/
	E_CONFIG_CENTRE_ALIGNMENT/**< Specifies center alignment*/

}E_MSG_CONFIG_ALIGNMENT;

/**
 * @brief Enumeration of icon types
 *
 * Core will pass the icon types in case of unsupported/corrupted/DRM rights expired contents by invoking jdd_MsgConfigGetDefaultErrorInfo().
 */
typedef enum _Config_Icon_Type
{
	E_CONFIG_UNSUPPORTED_ICON = 1, /**< Specifies the type as unsupported content*/
	E_CONFIG_CORRUPTED_ICON, /**< Specifies the type as corrupted content*/
	E_CONFIG_DRM_ICON, /**< Specifies the type as DRM content*/
	E_CONFIG_AUDIO_ICON,/**< Specifies the type as Audio*/
	E_CONFIG_VIDEO_ICON /**< Specifies the type as Video*/
}EMsgConfigIconType;

/**
 * @brief Enumeration of DRM errors
 *
 * Core will pass this DRM error type while invoking jdd_MsgConfigGetDefaultErrorInfo().
 */
typedef enum _Config_DRM_Error
{
	E_CONFIG_DRM_NO_ERROR = 1, /**< Specifies no error*/
	E_CONFIG_DRM_DURATION_EXPIRED, /**< Specifies DRM duration expired*/
	E_CONFIG_DRM_COUNT_EXCEEDED, /**< Specifies DRM play count expired*/
	E_CONFIG_DRM_START_TIME_NOT_REACHED, /**< Specifies permitted DRM play period not reached*/
	E_CONFIG_DRM_END_TIME_EXPIRED /**< Specifies permitted DRM play period expired*/

}EMsgConfigDRMError;

/**
 * @brief Enumeration of layout options
 *
 * Specifies the various layout types used for rendering
 */
typedef enum _Layout
{
	E_IMAGE_TOP_TEXT_BOTTOM = 0,/**< Specifies to render Image on top followed by text*/
	E_TEXT_TOP_IMAGE_BOTTOM, /**< Specifies to render Text on top followed by image*/
	E_IMAGE_LEFT_TEXT_RIGHT, /**< Specifies to render Image on left and Text on right (Landscape)*/
	E_TEXT_LEFT_IMAGE_RIGHT, /**< Specifies to render Text on left and Image on right (Landscape)*/
	E_COMPOSE_ORDER_HORZ, /**< Specifies to render objects left to right*/
	E_COMPOSE_ORDER_VERT /**< Specifies to render objects top to bottom*/

}E_LAYOUT;

/**
 * @brief Enumeration of region fit attributes
 */
typedef enum  _Config_region_fit
{
	E_CONFIG_REGION_NONE = 0,/**< Specifies that region fit attribute is not set*/
	E_CONFIG_REGION_HIDDEN,/**< Specifies that region fit attribute is <I>hidden</I>*/
	E_CONFIG_REGION_FILL,/**< Specifies that region fit attribute is <I>fill</I>*/
	E_CONFIG_REGION_MEET,/**< Specifies that region fit attribute is <I>meet</I>*/
	E_CONFIG_REGION_SCROLL,/**< Specifies that region fit attribute is <I>scroll</I>*/
	E_CONFIG_REGION_SLICE/**< Specifies that region fit attribute is <I>slice</I>*/
} E_CONFIG_REGION_FIT ;

/**
 * @brief Enumeration of media types
 */
typedef enum _config_media_type
{
	E_CONFIG_MEDIA_TEXT = 0x00, /**< Specifies media type as Inline-Text*/
	E_CONFIG_MEDIA_IMAGE, /**< Specifies that media object is an image*/
	E_CONFIG_MEDIA_AUDIO, /**< Specifies that media object is an audio*/
	E_CONFIG_MEDIA_VIDEO, /**< Specifies that media object is a video*/
	E_CONFIG_MEDIA_OTHERS /**< Specifies that media object is something other than the above media types.*/
}E_CONFIG_MEDIA_TYPE ;

#ifdef ENABLE_MSG_HTML_DISPLAY

/**
 * @brief Enumeration of DE Engine settings type (REquired only for EMail with HTML support)
 */
typedef enum _config_de_settings
{
	E_CONFIG_ARR_RULES = 0x00, /**< Specifies setting for ARR Rules*/
	E_CONFIG_DISPLAY_MODE_INFO, /**< Specifies setting for Display Mode options*/
	E_CONFIG_OPTION_MAGNIFICATION, /**< Specifies setting for Magnification*/
	E_CONFIG_SCROLL_MODE /**< Specifies setting for Scrolling*/
}E_CONFIG_DE_SETTING ;

#endif	//ENABLE_MSG_HTML_DISPLAY

/**
 * @brief Defines default Presentation Information
 *
 * This structure contains default Presentation Information
 * used while rendering a Message. All the attributes in this
 * structure are mandatory.  Application must set appropriate
 * values.
 */
typedef struct _msg_config_presentation_info
{
	E_PRESENTATION_TYPE		eDefaultPresentation;/**< Specifies Default Presentation Type*/
	JC_UINT32				uiDefaultSMILPageTime;/**< Specifies Default SMIL page Time*/
	JC_UINT32				uiDefaultMultipartPageTime;/**< Specifies Default page Time when Multipart Mixed content is displayed as SMIL*/
	JC_BOOLEAN				bTimerUnit;/**< Specifies the timer unit to be used if not specified by the SMIL attribute.<B>E_TRUE</B> - Use milliseconds, <B>E_FALSE</B> - Use seconds*/

		
}MSG_CONFIG_PRESENTATION_INFO;

/**
 * @brief Defines default SMIL Information
 *
 * This structure contains default SMIL Information used
 * while Rendering MMS. All the attributes in this structure
 * are mandatory.  Application must set appropriate values.
 */
typedef struct _msg_config_smil_info
{
	JC_UINT32			uiRootLayoutWidth;/**< Specifies Default Root Layout Width*/
	JC_UINT32			uiRootLayoutHeight;/**< Specifies Default Root Layout Height*/
	JC_INT8				pBGColor[25] ;/**< Specifies Default BG Color*/

	JC_CHAR				pRegion_1_Name[10];/**< Specifies Region 1 Name*/
	JC_UINT32			uiRegion_1_Top;/**< Specifies Region 1 Top.*/
	JC_UINT32			uiRegion_1_Left;/**< Specifies Region 1 Left*/
	JC_UINT32			uiRegion_1_Width;/**< Specifies Region 1 Width.*/
	JC_UINT32			uiRegion_1_Height;/**< Specifies Region 1 Height*/
	E_CONFIG_REGION_FIT	eRegion_1_Fit;/**< Specifies Region 1 Fit Attribute (Currently Ignoring the value while rendering).*/

	JC_CHAR				pRegion_2_Name[10];/**< Specifies Region 2 Name*/
	JC_UINT32			uiRegion_2_Top;/**< Specifies Region 2 Top.*/
	JC_UINT32			uiRegion_2_Left;/**< Specifies Region 2 Left*/
	JC_UINT32			uiRegion_2_Width;/**< Specifies Region 2 Width.*/
	JC_UINT32			uiRegion_2_Height;/**< Specifies Region 2 Height*/
	E_CONFIG_REGION_FIT	eRegion_2_Fit;/**< Specifies Region 2 Fit Attribute (Currently Ignoring the value while rendering).*/

	JC_CHAR				pRegion_3_Name[10];/**< Specifies Region 3 Name*/
	JC_UINT32			uiRegion_3_Top;/**< Specifies Region 3 Top.*/
	JC_UINT32			uiRegion_3_Left;/**< Specifies Region 3 Left*/
	JC_UINT32			uiRegion_3_Width;/**< Specifies Region 3 Width.*/
	JC_UINT32			uiRegion_3_Height;/**< Specifies Region 3 Height*/
	E_CONFIG_REGION_FIT	eRegion_3_Fit;/**< Specifies Region 3 Fit Attribute (Currently Ignoring the value while rendering).*/

}MSG_CONFIG_SMIL_INFO;

/**
 * @brief Specifies default information to be used for rendering text media
 *
 * This structure contains default Information used while
 * Rendering Text Media. All the attributes in this structure
 * are mandatory.  Application must set appropriate values.
 */
typedef struct _msg_config_text_info
{
	JC_INT8				pTextColor[25] ;/**< Specifies Default Text Color.*/
	JC_UINT32			uiTextSize;/**< Specifies Default Text Font size*/
	JC_INT8				pFontName[25];/**< Specifies Default Text Font Name*/
	JC_INT8				pInactiveLinkColor[25] ;/**< Specifies Color for Inactive Link.*/
	JC_INT8				pActiveLinkColor[25] ;/**< Specifies Color for Active Link.*/
		
}MSG_CONFIG_TEXT_INFO;

/**
 * @brief Defines Focus Information used while Focusing Media
 *
 * This structure contains default Focus Information used while
 * Focusing Media. All the attributes in this structure
 * are mandatory.  Application must set appropriate values.
 */
typedef struct _msg_config_focus_info
{
	JC_UINT32				uiFocusLineWidth;/**< Specifies Focus Line Width*/
	JC_INT8					pFocusLineColor[25];/**< Specifies Focus Line Color*/
	E_BORDER_STYLE			eFocusLineStyle;/**< Specifies Focus Line Style.(Dotted, solid, etc)*/
		
}MSG_CONFIG_FOCUS_INFO;

/**
 * @brief Defines Spacing Info used while rendering MMS
 *
 * This structure contains Spacing Info used while
 * rendering MMS. All the attributes in this structure
 * are mandatory.  Application must set appropriate values.
 */
typedef struct _msg_config_spacing_info
{
	JC_UINT8				uiTabLength; /**< Specifies the number of spaces to be used for replacing TAB occurrence (text rendering)*/
	JC_UINT8				uiScrollBarWidth; /**< Specifies the scrollbar width in pixels (rendering)*/
	JC_UINT8				uiScrollBarSpacing; /**< Specifies the spacing for scrollbar in pixels (rendering)*/
	JC_UINT8				uiMediaSpacing; /**< Specifies the spacing (pixels) between objects*/
	JC_UINT8				uiRenderingRectSpacing; /**< Specifies the spacing (pixels) for rendering rectangle*/
	JC_UINT8				uiRenderingRectTopSpacing;  /**< Specifies the top spacing (pixels) for rendering rectangle*/
	JC_UINT8				uiRenderingRectLeftSpacing;  /**< Specifies the left spacing (pixels) for rendering rectangle*/
	JC_UINT8				uiRenderingRectRightSpacing;  /**< Specifies the right spacing (pixels) for rendering rectangle*/
	JC_UINT8				uiRenderingRectBottomSpacing;  /**< Specifies the bottom spacing (pixels) for rendering rectangle*/
	JC_UINT8				uiFocusLineSpacing; /**< Specifies the spacing (pixels) for object focus rectangle*/
	JC_UINT8				uiControlSpacing; /**< Specifies the spacing (pixels) between each line of text*/
}MSG_CONFIG_SPACING_INFO;

/**
 * @brief Defines string for pre pending the subject field while replying and forwarding
 *
 * This structure contains the string to be used for pre pending
 * the subject field while replying and forwarding (Example: Fw:, Re:).
 * All the attributes in this structure are mandatory.
 * Application must set appropriate values.
 */
typedef struct _msg_config_text_value_info
{
	JC_CHAR				pForwardString[10]; /**< Specifies the string to be used for pre pending the subject filed while forwarding*/
	JC_CHAR				pReplyString[10]; /**< Specifies the string to be used for pre pending the subject filed while replying*/
	JC_BOOLEAN			bReplace ;	/**< Specifies whether to replace reply/forward substring if exists*/
		
}MSG_CONFIG_TEXT_VALUE_INFO;

/**
 * @brief Defines Information used to handle corrupted, DRM and unknown content
 *
 * This structure contains the Information used to handle corrupted
 * and unknown content. All the attributes in this structure
 * are mandatory.  Application must set appropriate values.
 */
typedef struct _msg_config_error_info
{
	JC_OBJECT_INFO				*pImageInfo; /**< Contains alternative Image Buffer for Corrupted, unsupported content.*/
	EMsgConfigIconType			eIconType;/**< Specifies the icon type*/
	EMsgConfigDRMError			eDRMError;/**< Specifies the DRM error, incase of DRM content */
}MSG_CONFIG_ERROR_INFO;

/**
 * @brief Defines Information used to display audio & video icon
 *
 * This structure contains the Information used to display audio
 * and video icon. All the attributes in this structure
 * are mandatory.  Application must set appropriate values.
 */
typedef struct _msg_config_icon_info
{
	JC_OBJECT_INFO				*pstImageInfo; /**< Contains Image Buffer to be displayed for audio/video*/
	EMsgConfigIconType			eIconType;/**< Specifies the icon type*/
} MSG_CONFIG_ICON_INFO;

/**
 * @brief Defines settings Info to view a message
 *
 * This structure contains the settings Info to view a message.
 * All the attributes in this structure are mandatory.  Application
 * must set appropriate values.
 */
typedef struct _msg_config_view_settings_info
{
	JC_BOOLEAN			bUseConfigSMIL;		/**< Specifies whether to use original SMIL or configuration SMIL info*/
	E_LAYOUT			eLayout;			/**< Specifies the layout type to be used*/
	JC_BOOLEAN			bPreferMediaTime;	/**< Specifies whether to prefer page or media duration (if media duration > page, E_TRUE enables to play the page till media duration*/
	JC_BOOLEAN			bValidateLayout;	/**< Specifies whether to validate the region layout*/
	JC_BOOLEAN			bValidateTimer;		/**< Specifies whether to validate the region layout*/
	E_MSG_CONFIG_ALIGNMENT		eImageAlign;/**< Specifies the alignment for image content*/
	E_MSG_CONFIG_ALIGNMENT		eTextAlign; /**< Specifies the alignment for text rendering*/
	JC_UINT32			uiCacheSize ;		/**< Specifies the Number of pages to be cached. Core process Text contents only upto this size. As user scrolls down further processing will be done. Scroll bar info will be updated as it processes rest of data */
}MSG_CONFIG_VIEW_SETTINGS_INFO;

/**
 * @brief Defines information about store settings
 */
typedef struct _store_config_settings
{
	JC_CHAR		*pRootFile; /**< Specifies Main File Name (holds all folder info)*/
	JC_CHAR		*pRootPath ; /**< Specifies location for keeping all storage specific files*/
	JC_UINT32	uiMsgType ; /**< Specifies user defined value*/

}STORE_CONFIG_SETTINGS;



/**
 * @ingroup    config_module
 * @brief      Retrieves default presentation information.
 * @param[out] pPresentationInfo Specifies presentation information
 * @param[in]  uiConfigArg Points to user-defined information
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_INVALID_PARAMETER if pPresentationInfo is NULL
 *
 * Application can set its own presentation information
 * using this API implementation.
 *
 * @note
 *    - Memory for pPresentationInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultPresentationInfo 
				(MSG_CONFIG_PRESENTATION_INFO *pPresentationInfo, JC_UINT32 uiConfigArg) ;

/**
 * @ingroup    config_module
 * @brief      Retrieves default text information.
 * @param[out] pTextInfo Specifies default text information
 * @param[in]  uiConfigArg Points to user-defined information
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_INVALID_PARAMETER if pTextInfo is NULL
 *
 * Application can set its own text information
 * using this API implementation.
 *
 * @note
 *    - Memory for pTextInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultTextInfo
				(MSG_CONFIG_TEXT_INFO *pTextInfo, JC_UINT32 uiConfigArg) ;

/**
 * @ingroup    config_module
 * @brief      Retrieves default focus information.
 * @param[out] pFocusInfo Specifies focusing information
 * @param[in]  uiConfigArg Points to user-defined information
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_INVALID_PARAMETER if pFocusInfo is NULL
 *
 * Application can set its own focus information
 * for focused media object, using this API implementation.
 *
 * @note
 *    - Memory for pFocusInfo is allocated and freed by the Core Module
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultFocusingInfo 
				(MSG_CONFIG_FOCUS_INFO *pFocusInfo, JC_UINT32 uiConfigArg) ;

/**
 * @ingroup    config_module
 * @brief      Retrieves default view setting information.
 * @param[out] pViewSettingsInfo Specifies view settings information
 * @param[in]  uiConfigArg Points to user-defined information
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_INVALID_PARAMETER If pViewSettingsInfo is passed as NULL
 *
 * Application can set its own view settings
 * for rendering the message, using this API implementation.
 *
 * @note
 *    - Memory for pViewSettingsInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultViewSettingsInfo 
				(MSG_CONFIG_VIEW_SETTINGS_INFO *pViewSettingsInfo, JC_UINT32 uiConfigArg) ;

/**
 * @ingroup    config_module
 * @brief      Retrieves default error information.
 * @param[out] pErrorInfo Specifies default error information
 * @param[in]  uiConfigArg Points to user-defined information
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_INVALID_PARAMETER If pErrorInfo is passed as NULL
 *    - JC_ERR_MEMORY_ALLOCATION If memory allocation fails
 *    - File handling errors.
 *
 * Application can set its own error info to be used for handling
 * corrupted or unknown content, using this API implementation.
 *
 * @note
 *    - Memory for pErrorInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultErrorInfo 
				(MSG_CONFIG_ERROR_INFO *pErrorInfo, JC_UINT32 uiConfigArg) ;

/**
 * @ingroup    config_module
 * @brief      Retrieves default spacing information.
 * @param[out] pSpacingInfo Specifies default spacing information
 * @param[in]  uiConfigArg Points to user-defined information
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_INVALID_PARAMETER If pSpacingInfo is passed as NULL
 *
 * Application can set its own spacing information, for media objects,
 * for rendering the message, using this API implementation.
 *
 * @note
 *    - Memory for pSpacingInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultSpacingInfo 
				(MSG_CONFIG_SPACING_INFO *pSpacingInfo, JC_UINT32 uiConfigArg) ;

/**
 * @ingroup    config_module
 * @brief      Retrieves default text value
 * @param[out] pTextValueInfo Specifies default text value
 * @param[in]  uiConfigArg Points to user-defined information
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_INVALID_PARAMETER If pTextValueInfo is passed as NULL
 *
 * Application can set its own texts for pre pending to reply
 * or forward subject fields, using this API implementation.
 *
 * @note
 *    - Memory for pTextValueInfo is allocated and freed by the Core Module.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultTextValueInfo 
				(MSG_CONFIG_TEXT_VALUE_INFO *pTextValueInfo, JC_UINT32 uiConfigArg) ;

/**
 * @ingroup    config_module
 * @brief      Retrieves default SMIL information for rendering the message
 * @param[out] pSMILInfo Specifies view settings information
 * @param[in]  uiRootLayoutHeight Specifies root layout height
 * @param[in]  uiRootLayoutWidth Specifies root layout width
 * @param[in]  uiConfigArg Points to user-defined information
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_INVALID_PARAMETER If pSMILInfo is passed as NULL
 *
 * Application can set its own SMIL information for rendering the message,
 * using this API implementation.
 *
 * @note
 *    - Memory for pSMILInfo is allocated and freed by the Core Module.
 *    - uiRootLayoutHeight and uiRootLayoutWidth are the root layout height
 *      and width passed by the core and to be assigned to the pSMILInfo
 *      structure. If any one of these are zero then need to get these
 *      values from the device characteristics and assigned to the pSMILInfo
 *      structure.
 *    - This API is applicable only to MMS
 */
JC_RETCODE jdd_MsgConfigGetDefaultSMILInfo 
				(MSG_CONFIG_SMIL_INFO *pSMILInfo, JC_UINT32 uiRootLayoutHeight, JC_UINT32 uiRootLayoutWidth, JC_UINT32 uiConfigArg) ;

/**
 * @ingroup        config_module
 * @brief          Retrieves filename with extension
 * @param[in,out]  ppFileName Specifies the file name
 * @param[in]      pContentType Specifies the content type
 * @param[in]      uiConfigArg	Points to user defined information
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_INVALID_PARAMETER If either ppFileName or pContentType is
 *                               passed as NULL
 *
 * Core will invoke this function while parsing. Application can check the file name:
 * - If *ppFileName == JC_NULL : 
 *		Create a new file name (with extension based on content-type), and pass back to core. 
 * - If *ppFileName != JC_NULL : 
 *		Check the name and if required modify the name or extension and pass back the modified name i.e., assign to *ppFileName.
 *
 * @note
 *	ppFileName - If modified, application should free  and allocate new resource which will be freed by core
 *
 */
JC_RETCODE jdd_MsgConfigGetFileNameWithExt 
				(JC_CHAR **ppFileName, const JC_INT8 *pContentType, JC_UINT32 uiConfigArg) ;


/**
 * @ingroup        config_module
 * @brief          Gets the list of supported URL scheme strings
 * @return
 *    - Pointer to URL scheme strings list, terminated with last element as JC_NULL.
 *
 * Core will use this list of schemes while parsing text contents to display contents 
 * matching this string as links.
 * @note
 *  - Pass the last element as NULL.
 *
 */
const JC_INT8 ** jdd_MsgConfigGetURLSchemes (void) ;

/**
 * @ingroup    config_module
 * @brief      Retrieves the storage settings
 * @param[out] pStoreSettings	On success, filled with the settings information
 * @return
 *	- JC_OK On Success
 *	- JC_ERR_INVALID_PARAMETER If pStoreSettings is NULL
 *
 * Application can return default settings for the storage. These information will be used by the messaging core
 * for maintaining the storage related files.
 *
 * @note
 *		- Caller should allocate and free pStoreSettings
 *		- On success, caller should free resources for pStoreSettings members
 */
JC_RETCODE jdd_MsgConfigGetDefaultStoreSettings 
				(STORE_CONFIG_SETTINGS *pStoreSettings) ;

/**
 * @ingroup    config_module
 * @brief      Checks if the content type is supported by the application
 * @param[in] pszContentType Points to the content type to be checked
 * @return
 *	- E_TRUE If content type is supported
 *	- E_FALSE If content type is not supported
 *
 * Messaging core invokes this function to validate whether the content is supported (rendering)
 *
 */
JC_BOOLEAN jdd_MsgConfigIsSupportedContentType
                                (JC_INT8 *pszContentType);

/**
 * @ingroup    config_module
 * @brief      Retrieves the media type from the content type
 * @param[in] pszContentType Points to the content type to be checked
 * @return
 *	- E_CONFIG_MEDIA_TYPE associated with the content type
 *
 * Messaging core invokes this function to get the media type from the content type (rendering)
 *
 */
E_CONFIG_MEDIA_TYPE   jdd_MsgConfigGetMediaType 
				(JC_INT8 *pszContentType) ;

/**
 * @ingroup    config_module
 * @brief      Retrieves the region to be used for the media type.
 * @param[in] eMediaType Specifies the media type to be checked
 * @return
 *	- Region Name associated with the media type.
 *
 * Messaging core invokes this function to get the region to be used while converting a multipart/mixed content to smil (rendering)
 *
 * @note
 *	Application should allocate the resource returned and caller will free the resource.
 *
 */
JC_CHAR	* jdd_MsgConfigGetSMILRegion
				(E_CONFIG_MEDIA_TYPE eMediaType) ;

/**
 * @ingroup    config_module
 * @brief      Retrieves the icon to be displayed for audio/video control.
 * @param[in]  pstIconInfo Specifies the pointer to MSG_CONFIG_ICON_INFO structure
 * @param[in]  uiConfigArg Points to user defined information
 * @return
 *		- JC_OK on success
 *
 * Messaging core invokes this function to get the icon buffer to be displayed for audio/video controls.
 * This icon will be displayed only when presentation is set to E_PRESENTATION_NONE.
 *
 */
JC_RETCODE jdd_MsgConfigGetDefaultIcon( MSG_CONFIG_ICON_INFO *pstIconInfo,
									    JC_UINT32			 uiConfigArg ) ;
/**
 * @ingroup    config_module
 * @brief      Initializes the File System Handle
 * @return
 *	- JC_OK	On success
 *	- Otherwise File System specific error
 *
 */
JC_RETCODE jdd_MsgConfigFSHandleInit
				(void);

/**
 * @ingroup    config_module
 * @brief      Deinitializes the File System Handle
 * @return
 *	- JC_OK	On success
 *	- Otherwise File System specific error
 *
 */
JC_RETCODE jdd_MsgConfigFSHandleDeInit
				(void);

/**
 * @ingroup    config_module
 * @brief      Retrieves File System Handle
 * @return
 *	- File System Handle
 *
 * File system handle can be used by the caller for performing any file stream related operations.
 *
 */
JDD_FSHANDLE jdd_MsgConfigGetFSHandle
				(void);

/**
 * @ingroup    config_module
 * @brief      Retrieves Folder Name (with Path) for keeping temporary files created by core (Parsing)
 * @param[in] eMessagingType Specifies messaging type
 * @return
 *	- Folder Name
 *
 * Messaging core will generate all the temporary files (parsed mime objects) in the specified folder. Application can provide different paths based on eMessagingType.
 *
 */
JC_CHAR	*	jdd_MsgConfigGetTempFolder 
				(E_CONFIG_MSG_TYPE eMessagingType) ;

/**
 * @ingroup    config_module
 * @brief      Specifies the default data size (chunk size) that MM1 can read from file stream 
 *			   during Decode operation.
 * @return
 *	Chunk Size
 *
 * Messaging core will do file read of size returned from this function while decoding in chunks.
 *
 * Note: 
 * Applicable to File stream based decoding (i.e., application passes message data in file).
 *
 */
JC_UINT32	jdd_MsgConfigGetFileChunkSize
				(void) ;

/**
 * @ingroup    config_module
 * @brief      Checks whether application wishes to save a received email message in a file
 * @return
 *	- E_TRUE If received message is to be saved to a file 
 *	- E_FALSE If received message is to be saved to buffer 
 *
 * Messaging core invokes this function to find the means of storage for a received message. 
 *
 * Note:
 * Application should delete the file after reading the data from the file.
 */
JC_BOOLEAN jdd_MsgConfigSaveReceivedMailInFile (void) ;


/**
 * @ingroup    config_module
 * @brief      Checks whether the DRM content type is supported by application
 * @param[in] pContentType Points to the content type to be checked
 * @return
 *	- E_TRUE If content type is supported
 *	- E_FALSE If content type is not supported
 *
 * Messaging core invokes this function to validate if the DRM content type is supported by the application.
 *
 */
JC_BOOLEAN jdd_MsgConfigIsDRMTypeSupported
					(JC_INT8 *pContentType);

/**
 * @ingroup    config_module
 * @brief      Cleans up the temporary files directory.
 * @param[in]  uiConfigArg	Points to user defined information
 * @return
 *	- JC_OK On Success
 *
 * Core calls this function to clear the temporary file generated.
 *
 */
JC_RETCODE jdd_MsgConfigCleanUpTempFiles
				(JC_UINT32 uiConfigArg) ;


JC_INT8 *jdd_MsgConfigGetContentTypeFromFileName (JC_INT8 *pcFileName);


#ifdef ENABLE_MSG_HTML_DISPLAY

/**
 * @ingroup    config_module
 * @brief      Checks whether to display as Rich Text or Plain Text (text/html or text/plain)
 * @param[in]  uiConfigArg	Points to user defined information
 * @param[out]  bIsRichText	Specifies whether to display Rich Text (E_TRUE)
 * @return
 *	- JC_OK On Success
 *
 * Core calls this function if message is multipart/alternate to check whether to render the message as Plain Text or Rich Text
 */
JC_RETCODE jdd_MsgConfigGetRichTextInfo 
				(JC_BOOLEAN *bIsRichText, JC_UINT32 uiConfigArg) ;

/**
 * @ingroup    config_module
 * @brief      To get timer value to be used in Display Engine Transport Listener callback (to set content)
 * @param[in]  uiConfigArg	Points to user defined information
 * @return
 *	Returns the timer value (milliseconds)
 *
 */
JC_UINT32	jdd_MsgConfigGetDEListenerTimer 
						(JC_UINT32 uiConfigArg) ;

/**
 * @ingroup    config_module
 * @brief      Gets DE Settings
 * @param[in]  eDESetting	Specifies the settings type for which request comes.
 * @param[in,out]  pDESettingInfo	Points to settings information. On success information will be filled.
 * @param[in]  uiConfigArg	Points to user defined information
 * @return
 *	- JC_OK On Success
 *
 * Core calls this function to get the HTML Display Engine Settings.
 */
JC_RETCODE	jdd_MsgConfigGetDESettings 
					(E_CONFIG_DE_SETTING eDESetting, void *pDESettingInfo, JC_UINT32 uiConfigArg) ;

#endif	//ENABLE_MSG_HTML_DISPLAY

#ifdef EMAIL_CORE_WITH_INDICATION_HANDLING
/**
 * @ingroup    config_module
 * @brief      GetUIDLFileSizeLimit
 * @param[out]  puiFileSize	points to the Size Limit Configured on the UIDL File Size
 * @return
 *	- JC_OK On Success
 *
 * Core calls this function to get the configured Size limit for the UIDL File 
*  IF the Configured File Size is Set to zero No limitation would set to the File Size
*
 */
 JC_RETCODE jdd_EmailConfigGetUIDLFileSizeLimit(JC_UINT32 *puiFileSize) ;
#endif

/** @} */ // End of config_module

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//_JDD_MESSAGING_CONFIG_H

/* END OF FILE */
