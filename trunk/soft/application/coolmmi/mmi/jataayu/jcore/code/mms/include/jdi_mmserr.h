/**
 * @file    jdi_MMSErr.h
 * @brief   Declares the Messaging Module Error Codes.
 */
 /***************************************************************************
 *
 * File Name : jdi_MMSErr.h
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights in and to this Software 
 * or any part of this (including without limitation any images, photographs, animations, video, 
 * audio, music, text and/or "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the Software, are owned 
 * by Jataayu Software (P) Ltd., Bangalore ("Jataayu") or Jataayu's suppliers as the case may be. 
 * The Software is protected by copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and applicable laws in the country 
 * in which the Software is being used. You shall not modify, adapt or translate the Software, 
 * without prior express written consent from Jataayu. You shall not reverse engineer, decompile, 
 * disassemble or otherwise alter the Software, except and only to the extent that such activity is 
 * expressly permitted by applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe civil and criminal 
 * penalties and will be prosecuted to the maximum extent possible under the law. Jataayu reserves 
 * all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND AND 
 * ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY ACCURACY OF 
 * INFORMATIONAL CONTENT, AND/OR FITNESS FOR A PARTICULAR PURPOSE OR USE, TITLE 
 * OR INFRINGEMENT ARE EXPRESSLY DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. 
 * YOU ASSUME THE ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. 
 * JATAAYU SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, EXEMPLARY, 
 * SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION ANY LOSS OF DATA, OR; 
 * LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL OR OTHER SIMILAR LOSS RESULTING 
 * FROM OR OUT OF THE USE OR INABILITY TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************/
#ifndef	_JDI_MMS_ERR_H
#define	_JDI_MMS_ERR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <jdi_mmsmodule.h>

/**
 * @brief The error code is returned in case of failure while adding header.
 */
#define 	JC_ERR_MMS_ADD_HEADER						TERMINAL_ERR_CODE(MM1_MODULE, 0x01)

/**
 * @brief The error code is returned in case of failure while decoding text encoded message.
 */
#define 	JC_ERR_MMS_HTTP_MULTIPART_DECODE			TERMINAL_ERR_CODE(MM1_MODULE, 0x02)

/**
 * @brief The error code is returned if content-type header is missing.
 */
#define 	JC_ERR_MMS_INVALID_CONTENT_TYPE				TERMINAL_ERR_CODE(MM1_MODULE, 0x03)

/**
 * @brief The error code is returned in case failure in decoding mime headers.
 */
#define 	JC_ERR_MMS_HTTP_HEADERS_DECODE				TERMINAL_ERR_CODE(MM1_MODULE, 0x04)

/**
 * @brief The error code is returned if message header is not present (while decoding).
 */
#define 	JC_ERR_MMS_HTTP_INVALID_HEADER_LENGTH		TERMINAL_ERR_CODE(MM1_MODULE, 0x05)

/**
 * @brief The error code is returned if message encoding fails (binary).
 */
#define 	JC_ERR_MMS_WSP_MULTIPART_ENCAPSULATION		TERMINAL_ERR_CODE(MM1_MODULE, 0x06)

/**
 * @brief The error code is returned if message content type header formation fails.
 */
#define 	JC_ERR_MMS_WSP_CONTENT_HEADER				TERMINAL_ERR_CODE(MM1_MODULE, 0x07)

/**
 * @brief The error code is returned if message header formation fails (binary).
 */
#define 	JC_ERR_MMS_WSP_HEADERS_ENCAPSULATION		TERMINAL_ERR_CODE(MM1_MODULE, 0x08)

/**
 * @brief The error code is returned if message encoding fails (text format).
 */
#define 	JC_ERR_MMS_HTTP_MULTIPART_ENCAPSULATION		TERMINAL_ERR_CODE(MM1_MODULE, 0x09)

/**
 * @brief The error code is returned if message header formation fails (text).
 */
#define 	JC_ERR_MMS_HTTP_HEADER_ENCAPSULATION		TERMINAL_ERR_CODE(MM1_MODULE, 0x0A)

/**
 * @brief The error code is returned if MM1 Handle is NULL.
 */
#define 	JC_ERR_MMS_INVALID_HANDLE					TERMINAL_ERR_CODE(MM1_MODULE, 0x0B)

/**
 * @brief The error code is returned if message decoding fails.
 */
#define 	JC_ERR_MMS_DECODING							TERMINAL_ERR_CODE(MM1_MODULE, 0x0C)

/**
 * @brief The error code is returned if pdu type is invalid.
 */
#define 	JC_ERR_MMS_INVALID_MESSAGE_TYPE				TERMINAL_ERR_CODE(MM1_MODULE, 0x0D)

/**
 * @brief The error code is returned if header is not supported (get or set).
 */
#define 	JC_ERR_MMS_INVALID_HEADER					TERMINAL_ERR_CODE(MM1_MODULE, 0x0E)

/**
 * @brief The error code is returned if charset conversion fails.
 */
#define 	JC_ERR_MMS_CONVERSION						TERMINAL_ERR_CODE(MM1_MODULE, 0x0F)


/**
 * @brief The error code is returned if DRM Response callback from application passed an invalid DRM Reference ID (non-blocked DRM handling).
 */
#define 	JC_ERR_MSG_INVALID_DRM_ID					TERMINAL_ERR_CODE(MSG_GENERAL_MODULE, 0x01)

/**
 * @brief The error code is returned if core is waiting for the DRM Handler to 
 *			finish decoding DRM content (non-blocked DRM handling). 
 *
 * Note: This is an Informational error code and <I> requested operation is success </I>
 */
#define 	JC_ERR_AWAITING_DRM_RESPONSE_CB				TERMINAL_ERR_CODE(MSG_GENERAL_MODULE, 0x02)


/**
 * @brief The error code is returned if messaging type is not supported.
 */
#define 	JC_ERR_MSG_INVALID_MESSAGING_TYPE			TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x01)

/**
 * @brief The error code is returned if page/slide index is invalid.
 */
#define 	JC_ERR_MSG_INVALID_PAGE_INDEX				TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x02)

/**
 * @brief The error code is returned if media object reference-id is invalid.
 */
#define 	JC_ERR_MSG_INVALID_MEDIA_INDEX				TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x03)

/**
 * @brief The error code is returned if creation mode is set as restricted and the object 
 *			being added is non-conformant to the content-class.
 */
#define 	JC_ERR_MSG_MEDIA_RESTRICTED					TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x04)

/**
 * @brief The error code is returned if creation mode is set as warning and the object 
 *			being added is non-conformant to the content-class.
 *
 * This is an informational error code and <I> requested operation is success </I>.
 */
#define 	JC_ERR_MSG_MEDIA_WARNING					TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x05)

/**
 * @brief The error code is returned if message size exceeds the limit set.
 */
#define 	JC_ERR_MSG_MAX_REACHED						TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x06)

/**
 * @brief The error code is returned if presentation type is not set (on get/set of SMIL region/root).
 */
#define 	JC_ERR_MSG_PRESENTATION_NOT_SET				TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x07)

/**
 * @brief The error code is returned if feature is not supported.
 */
#define 	JC_ERR_UNSUPPORTED							TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x08)

/**
 * @brief The error code is returned if requested information is not found.
 */
#define 	JC_ERR_NOT_FOUND							TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x09)

/**
 * @brief The error code is returned if message header is not filled/set.
 */
#define 	JC_ERR_MSG_HEADERS_NOT_SET					TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x0A)

/**
 * @brief The error code is returned if media type is not supported (for SMIL).
 */
#define 	JC_ERR_MSG_MMS_UNSUPPORTED_TYPE				TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x0B)

/**
 * @brief The error code is returned if audio or video is added and current page/slide 
 *			already  has audio (for SMIL).
 */
#define 	JC_ERR_MSG_MMS_AUDIO_EXISTS					TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x0C)

/**
 * @brief The error code is returned if image or video is added and current page/slide 
 *			already has image (for SMIL).
 */
#define 	JC_ERR_MSG_MMS_IMAGE_EXISTS					TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x0D)

/**
 * @brief The error code is returned if image or audio or video is added and current page/slide 
 *			already has video (for SMIL).
 */
#define 	JC_ERR_MSG_MMS_VIDEO_EXISTS					TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x0E)

/**
 * @brief The error code is returned if text is added and current page/slide 
 *			already has text (for SMIL).
 */
#define 	JC_ERR_MSG_MMS_MEDIA_TYPE_EXISTS			TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x0F)

/**
 * @brief The error code is returned if media info is not valid (not filled).
 */
#define 	JC_ERR_MSG_INVALID_MEDIA_INFO				TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x10)

/**
 * @brief The error code is returned while forwarding MMS, if creation mode is 
 *			restricted and media(s) in forwarded message is non-conformant to the content-class.
 */
#define 	JC_ERR_MSG_MMS_CONTENT_RESTRICTED			TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x11)

/**
 * @brief The error code is returned while forwarding MMS, if creation mode is warning and 
 *			media(s) in forwarded message is non-conformant to the content-class.
 * 
 * Note: This is an informational error code and <I> the requested operation is success </I>.
 */
#define 	JC_ERR_MSG_MMS_CONTENT_WARNING				TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x12)

/**
 * @brief The error code is returned if media object is non-conformant to the content-class set.
 */
#define 	JC_ERR_MSG_MMS_CONTENT_CLASS				TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x13)

/**
 * @brief The error code is returned while forwarding a message if the message contains 
 *			forward protected DRM objects.
 */
#define 	JC_ERR_MSG_DRM_FORWARD_RESTRICTED			TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x14)


/**
 * @brief The error code is returned while forwarding a message if the message contains 
 *			forward protected DRM objects.
 */
#define 	JC_ERR_MSG_MULTIPLE_REFERENCE				TERMINAL_ERR_CODE(MSG_COMPOSE_MODULE, 0x15)


/**
 * @brief The error code is returned if SMIL decoding/parsing fails.
 */
#define 	JC_ERR_SMIL_PARSING							TERMINAL_ERR_CODE(MSG_PRESENTATION_MODULE, 0x01)

/**
 * @brief The error code is returned if SMIL region layout is empty/not set.
 */
#define 	JC_ERR_SMIL_REGION_NOT_SET					TERMINAL_ERR_CODE(MSG_PRESENTATION_MODULE, 0x02)

/**
 * @brief The error code is returned if presentation type is not set.
 */
#define	JC_ERR_PRESENTATION_NOT_SET						TERMINAL_ERR_CODE(MSG_PRESENTATION_MODULE, 0x03)


/**
 * @brief The error code is returned if folder name is invalid.
 */
#define 	JC_ERR_STORE_INVALID_FOLDER_NAME			TERMINAL_ERR_CODE(MSG_STORE_MODULE, 0x01)

/**
 * @brief The error code is returned if folder name already exists.
 */
#define 	JC_ERR_STORE_FOLDER_ALREADY_EXIST			TERMINAL_ERR_CODE(MSG_STORE_MODULE, 0x02)

/**
 * @brief The error code is returned if active folder is not set.
 */
#define 	JC_ERR_STORE_ACTIVE_FOLDER_NOT_SET			TERMINAL_ERR_CODE(MSG_STORE_MODULE, 0x03)

/**
 * @brief The error code is returned if folder name is not found.
 */
#define 	JC_ERR_STORE_FOLDER_NOT_FOUND				TERMINAL_ERR_CODE(MSG_STORE_MODULE, 0x04)

/**
 * @brief The error code is returned if active filter not set.
 */
#define 	JC_ERR_STORE_ACTIVE_FILTER_NOT_SET			TERMINAL_ERR_CODE(MSG_STORE_MODULE, 0x05)

/**
 * @brief The error code is returned if filter (name) is not found.
 */
#define 	JC_ERR_STORE_FILTER_NOT_FOUND				TERMINAL_ERR_CODE(MSG_STORE_MODULE, 0x06)

/**
 * @brief The error code is returned if message index is invalid/not found.
 */
#define 	JC_ERR_STORE_INVALID_MSG_INDEX				TERMINAL_ERR_CODE(MSG_STORE_MODULE, 0x07)

/**
 * @brief The error code is returned if filter with the same name already exists.
 */
#define 	JC_ERR_STORE_FILTER_ALREADY_EXIST			TERMINAL_ERR_CODE(MSG_STORE_MODULE, 0x08)

/**
 * @brief The error code is returned if the message is blocked.
 */
#define	JC_ERR_STORE_FILTER_MSG_BLOCKED					TERMINAL_ERR_CODE(MSG_STORE_MODULE, 0x09)

/**
 * @brief The error code is returned if store file is corrupted.
 */
#define	JC_ERR_STORE_FILE_CORRUPTED						TERMINAL_ERR_CODE(MSG_STORE_MODULE, 0x0A)


/**
 * @brief The error code is returned if message type is not supported.
 */
#define	JC_ERR_VIEW_UNSUPPORTED_MESSAGE_TYPE			TERMINAL_ERR_CODE(MSG_VIEW_MODULE, 0x01)

/**
 * @brief The error code is returned if page index is invalid.
 */
#define	JC_ERR_VIEW_INVALID_PAGE_INDEX					TERMINAL_ERR_CODE(MSG_VIEW_MODULE, 0x02)

/**
 * @brief The error code is returned if view info already exists.
 */
#define	JC_ERR_VIEW_INFO_ALREADY_EXISTS					TERMINAL_ERR_CODE(MSG_VIEW_MODULE, 0x03)

/**
 * @brief The error code is returned if content is corrupted (rendering audio/video/image).
 */
#define	JC_ERR_VIEW_CORRUPTED_CONTENT					TERMINAL_ERR_CODE(MSG_VIEW_MODULE, 0x04)

/**
 * @brief The error code is returned if content is not supported for rendering.
 */
#define	JC_ERR_VIEW_UNSUPPORTED_CONTENT					TERMINAL_ERR_CODE(MSG_VIEW_MODULE, 0x05)

/**
 * @brief The error code is returned if view info is not valid (jdi_MsgView is not called before).
 */
#define	JC_ERR_VIEW_INVALID_VIEW_INFO					TERMINAL_ERR_CODE(MSG_VIEW_MODULE, 0x06)

/**
 * @brief The error code is returned if no object found within the focus point.
 */
#define	JC_ERR_VIEW_FOCUS_CHANGE						TERMINAL_ERR_CODE(MSG_VIEW_MODULE, 0x07)

/**
 * @brief The error code is returned if focusing is disabled or no object is currently focused.
 */
#define	JC_ERR_VIEW_FOCUS_DISABLED						TERMINAL_ERR_CODE(MSG_VIEW_MODULE, 0x08)


/**
 * @brief The error code is returned if MMS Version is not supported/invalid.
 */
#define 	JC_ERR_MMS_INVALID_MMS_VERSION				TERMINAL_ERR_CODE(MSG_CONTROLLER_MODULE, 0x01)


#ifdef __cplusplus
}
#endif

#endif	//_JDI_MMS_ERR_H
