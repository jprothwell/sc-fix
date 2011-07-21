/**
 * @file  MessagingAPI.h
 * @brief Contains the prototypes of messaging APIs
 */
/***************************************************************************
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
#ifndef _MESSAGING_API_H_
	#define _MESSAGING_API_H_

#include 	<jdi_mmserr.h>


#ifdef	__cplusplus
	extern "C"
	{
#endif

/** @mainpage
 *
 *
 * @section intro_section Introduction
 *
 * This Document gives a detailed description of the interfaces that has to be used by an application. The documentation clearly describes the usage of each function, the significance of each variable, usage of the data structures and enumerations etc. The document will ease the complexity faced while interfacing the application and the MMS core. <BR>
 *
 * @section startup_code Startup Code
 *
 *    - The application has to initialize all the JDD modules (Net, timer, MMI etc) that are going to be used by the MMS application.
 *    - Then the application needs to start the transport task. This will ensure that the queue for the transport is created.
 *    - After that, the queue for the application has to be created.
 *
 * @section basic_flow Basic Flow
 *
 *    - This section describes the basic flow that has to be used in order to use the Messaging Core and Communicator for Sending/receiving MMS messages.
 *    - The initialize function <B><I>jdi_MsgInit()</I></B> has to be called before calling any Messaging related functions.
 *    - Then the application has to call the other Messaging API's as required.
 *    - Finally application has to call deinitialize function <B><I>jdi_MsgDeInit(). </I></B>
 *    - The application has to initialize the communicator, specify the "data connection profile" and the "transport profile" information using Communicator before giving any send/receive request.
 *    - Then application can then give the send/receive request for MMS message.
 *    - Finally application has to Deinitialize the communicator.
 */

/** @defgroup Compose_Module Compose module
 *  @{
 */

/**
 * @ingroup    Compose_Module
 *
 * @brief      Initializes the message handle
 *
 * @param[out] phMsg  Specifies messaging handle, which has to be initialized
 * @param[in]  pCallbackArg Specifies user defined arguments that will be
 *                          passed to application's callback functions and
 *                          DDL's messaging config APIs.
 * @return
 *    - JC_OK on success
 *    - JC_ERR_MEMORY_ALLOCATION If Memory allocation fails
 *
 * This API is used to initialize the messaging handle. This handle is
 * valid per message basis.
 *
 * @note This messaging handle should be used in subsequent API calls.
 *       Each of jdi_MsgInit() should be accompanied by respective call
 *       to jdi_MsgDeInit()
 *
 * @sa jdi_MsgDeInit()
*/
extern JC_RETCODE 	jdi_MsgInit (MSG_HANDLE *phMsg, void *pCallbackArg);


/**
 * @ingroup    Compose_Module
 *
 * @brief      Deinitializes the message handle and releases all allocated resources
 *
 * @param[in]  hMsg  Specifies message handle
 *
 * @return     JC_OK on success
*/
extern JC_RETCODE 	jdi_MsgDeInit (MSG_HANDLE hMsg);


/**
 * @ingroup    Compose_Module
 *
 * @brief      Sets the message type
 *
 * @param[in]  hMsg          Specifies message handle
 * @param[in]  eMessagingType	Specifies the message type to be set
 * @return
 *    - JC_OK on success
 *
 * This API sets the type of message to be composed.
 * The message type can be either of MMS, E-Mail, EMS.
*/
extern JC_RETCODE	jdi_MsgSetMessagingType (MSG_HANDLE hMsg, EMessagingType eMessagingType);


/**
 * @ingroup    Compose_Module
 *
 * @brief      Retrieves the message type
 *
 * @param[in]  hMsg          Specifies message handle
 * @param[out] pMessagingType	On success points to the messaging type
 *
 * @return
 *    - JC_OK on success
 *
 * This API is used to get the messaging type of the handle [MMS, E-Mail or EMS].
 *
 * @note  Resources for <I>pMessagingType</I> should be allocated by
 *        the caller.
*/
extern JC_RETCODE	jdi_MsgGetMessagingType (MSG_HANDLE hMsg, EMessagingType *pMessagingType);


/**
 * @ingroup    Compose_Module
 * @brief      Adds media object to the message.
 *
 * @param[in]  hMsg         Specifies handle to message
 * @param[in]  pMediaInfo   Species the pointer to MEDIA_INFO to be added
 * @param[in]  ucPageIndex  Specifies Page index. Required if bIsAttachment = E_FALSE (index starts with 1)
 * @param[out] pMediaID	    Specifies pointer that will hold the unique media-id on success
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_INVALID_STATE	If this API is called while Preview/View is active
 *    - JC_ERR_MEMORY_ALLOCATION	If memory allocation fails
 *    - JC_ERR_MSG_MAX_REACHED	If the message size exceeds the maximum limit
 *      (or) If the total media exceeds the maximum limit.
 *    - JC_ERR_MSG_INVALID_MEDIA_INFO	If Media information is incomplete
 *      or invalid (content type missing, null buffer, filename not
 *      specified for media other than inline text, media size is 0)
 *    - JC_ERR_MSG_MMS_MEDIA_TYPE_EXISTS	This error code is applicable
 *      to MMS Messaging type with SMIL as presentation type.If the passed
 *      media type is already present in the selected page
 *    - JC_ERR_MSG_MMS_AUDIO_EXISTS	This error code is applicable to MMS
 *      Messaging type with SMIL as presentation type.If the passed media
 *      type is video and the selected page has an audio object.
 *    - JC_ERR_MSG_MMS_VIDEO_EXISTS	This error code is applicable to MMS
 *      Messaging type with SMIL as presentation type.If the passed media
 *      type is audio and the selected page has a video object.
 *    - JC_ERR_MSG_MMS_UNSUPPORTED_TYPE	This error code is applicable to
 *      MMS Messaging type with SMIL as presentation type.If the passed
 *      media is of type E_MEDIA_OTHERS
 *    - JC_ERR_MSG_MEDIA_RESTRICTED	This error code is applicable to MMS
 *      Messaging type with Creation mode set as E_CREATION_MODE_RESTRICTED.
 *      If the media object violates the content-class set for the message.
 *    - JC_ERR_MSG_MEDIA_WARNING	Applicable to MMS Messaging type with Creation mode set as
 *      E_CREATION_MODE_WARNING.If the media object violates the content-class
 *      set for the message.
 *	  - JC_ERR_MSG_MULTIPLE_REFERENCE	Informational error code to indicate previous media info is used (multiple reference), 
 *										instead of duplicating. Application should free the pMediaInfo->MEDIA_MIME_INFO.
 *	  - JC_ERR_ASYNC_CALL	If application wants to handle the error indication callback response <I>asynchronously</I>. This is an informational error code.
 *
 * This API is used to add a media to the message. pMediaID returned from this API can be
 * used for subsequent reference to the object.
 *
 * @note
 *    - Caller should allocate the resources for the <I>pMediaInfo</I> member variables.
 *    - <I>JC_ERR_MSG_MULTIPLE_REFERENCE</I> is an informational error code. When this error code is returned, 
 *		application should free resources allocated for pMediaInfo->MEDIA_MIME_INFO (and its members).
 *    - This API uses the same resources as passed in the members of <I>pMediaInfo</I>
 *    - Resources will be freed internally on calling <I>jdi_MsgDeinit()</I> or <I>jdi_MsgDeleteMedia()</I>
 *	  - If error code is JC_ERR_ASYNC_CALL, core will internally store the media being added and 
 *	    will process it on invoking jdi_MsgAsyncErrorIndCBHandler(). Application should not free the resources allocated for MEDIA_INFO members.
 *
 */
extern JC_RETCODE	jdi_MsgAddMedia (MSG_HANDLE hMsg, MEDIA_INFO *pMediaInfo,
									JC_UINT8 ucPageIndex, JC_UINT32 *pMediaID);

/**
 * @ingroup    Compose_Module
 * @brief      Updates inline text media object.
 * @param[in]  hMsg         Specifies handle to message
 * @param[in]  pTextBuffer	Specifies pointer to inline text buffer to be updated
 * @param[in]  uiBufferLen	Specifies buffer length( <I>jc_tcslen</I> of pTextBuffer)
 * @param[in]  ucPageIndex	Specifies Page index in which media is present
 * @param[in]  uiMediaID    Specifies Media id that has to be updated
 *
 * @return
 *    - JC_OK    On Success
 *    - JC_ERR_INVALID_STATE		If this API is called while Preview/View is active
 *    - JC_ERR_MSG_INVALID_MEDIA_INDEX	If the media not found (invalid uiMediaID)
 *    - JC_ERR_MSG_MAX_REACHED		If the message size exceeds the maximum limit
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX	If the page/slide not found (ucPageIndex)
 *
 * This API is used to update an already added inline text object's buffer.
 *
 * @note
 *    - Caller should not free the resource allocated for pTextBuffer.
 *    - pTextBuffer will be freed internally on calling jdi_MsgDeleteMedia or jdi_MsgDeinit
 *
 */
extern JC_RETCODE	jdi_MsgUpdateInlineText (MSG_HANDLE hMsg, JC_UINT8 *pTextBuffer,
										JC_UINT32 uiBufferLen, JC_UINT8 ucPageIndex,
										JC_UINT32 uiMediaID);


/**
 * @ingroup    Compose_Module
 * @brief      Asynchronous response to Error indication callback
 *
 * @param[in]  hMsg				Specifies handle to message
 * @param[in]  eErrorType		Specifies Error Indication type (Callback function which was initiated by core)
 * @param[in]  eAsyncResponse	Specifies the response for the callback function.
 * @param[in]  pReturnInfo		void pointer which will be filled with callback specific information (from core)
 *
 * @return
 *    - JC_OK for +ve user response (OK).
 *	  - JC_ERR_USER_CANCELLED on -ve user respone (Cancel).
 *
 * Platform on which application can not get synchronous user reponses (blocking) for callback functions can use 
 * this Interface.
 * Eg: For Creation mode - Warning, core invokes callback function if object added is non-conformant to the 
 *	   content class set. Application which can display MMI (modal window) can return the user confirmation in 
 *     immediately. For platforms which does not support modal windows can return JC_ERR_ASYNC_CALL immediately 
 *     and pass the user reponse using this API.
 *
 * Note: 
 *	- Currently E_WARNING_CREATION_MODE_ADDMEDIA callback alone supported.
 *	- If eAsyncResponse is other than JC_OK or any error while adding the media, core will free the resource internally.
 *
 */
extern JC_RETCODE	jdi_MsgAsyncErrorIndCBHandler (MSG_HANDLE		hMsg, 
												   ErrorCode		eErrorType, 
												   JC_RETCODE		eAsyncResponse, 
												   void				*pReturnInfo) ;

/**
 * @ingroup    Compose_Module
 * @brief      Deletes specified media from a message
 * @param[in]  hMsg         Specifies handle to message
 * @param[in]  ucPageIndex	Specifies page index in which
 *                          media is present. Not required if bIsAttachment = E_TRUE
 * @param[in]  uiMediaID	Specifies unique id of the media which
 *                          needs to be deleted
 * @param[in]  bIsAttachment E_TRUE for Attachment
 *
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_INVALID_STATE	If this API is called while Preview/View is active
 *    - JC_ERR_MSG_INVALID_MEDIA_INDEX	If the media not found (invalid uiMediaID)
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX	If the page/slide not found (ucPageIndex)
 *
 * This API is used to delete media object from the message handle.
*/
extern JC_RETCODE	jdi_MsgDeleteMedia (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
											JC_UINT32 uiMediaID, JC_BOOLEAN bIsAttachment);


/**
 * @ingroup    Compose_Module
 *
 * @brief      Creates message handle from the passed buffer
 * @param[out] phMsg            Specifies handle to message
 * @param[in]  eMessagingType   Specifies the message type
 * @param[in]  pEncapsulatedContent Specifies the message buffer
 *
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_NOT_IMPLEMENTED	If pEncapsulatedContent->bIsStream is E_TRUE
 *
 * This API is used to creates message handle from buffer (eg: Editing a draft message).
*/
extern JC_RETCODE	jdi_MsgCompose (MSG_HANDLE *phMsg, EMessagingType eMessagingType,
									CONTENT_DATA *pEncapsulatedContent);

/**
 * @ingroup    Compose_Module
 *
 * @brief      Retrieves the list of media objects present in the requested page or attachments
 * @param[in]  hMsg         Specifies handle to message
 * @param[in]  ucPageIndex	Specifies page index
 * @param[out] ppMediaInfo	On success, points to the media list
 * @param[out] pMediaCount	On success, points to the media count
 * @param[in]  bIsAttachment	Pass E_FALSE for getting inline objects, E_TRUE for attachments
 *
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX	If the page/slide not found (if bIsAttachment is E_FALSE)
 *
 * This API is used to get list of media objects for a particular page or list of attachments
 *
 * @note
 *    - Application should not directly alter the media list (ppMediaInfo) returned by the API. 
 *      Appropriate APIs should be called to alter the media information
 *
 * @sa jdi_MsgGetMediaAttributes(), jdi_MsgGetMediaCount().
*/
extern JC_RETCODE	jdi_MsgGetMediaList (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
									MEDIA_INFO **ppMediaInfo, JC_UINT8 *pMediaCount,
									JC_BOOLEAN bIsAttachment);

/**
 * @ingroup    Compose_Module
 *
 * @brief      Retrieves the PresentationInfo for the requested media object.
 * @param[in]  hMsg                 Specifies handle to message
 * @param[in]  ucPageIndex          Specifies the page index
 * @param[in]  uiMediaID	        Specifies media ID for requested media
 * @param[out] pPresentationInfo    On success, holds the media presentation information
 *
 * @return
 *    - JC_OK on success
 *    - JC_ERR_MSG_INVALID_MEDIA_INDEX If invalid media ID is passed
 *
 * This API is used to get the presentation information for a specific media object
 *
 * @note
 *    - Application should not directly modify the media presentation info returned by the API.
 *
 * @sa jdi_MsgSetMediaPresentationInfo(), jdi_MsgGetMediaMimeInfo()
*/
extern JC_RETCODE	jdi_MsgGetMediaPresentationInfo (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
									JC_UINT32 uiMediaID, MEDIA_PRESENTATION_INFO *pPresentationInfo);

/**
 * @ingroup    Compose_Module
 *
 * @brief      Sets the PresentationInfo for the requested media object.
 * @param[in]  hMsg                 Specifies handle to message
 * @param[in]  ucPageIndex			Specifies the page index
 * @param[in]  uiMediaID	        Specifies media ID for requested media
 * @param[in]  pPresentationInfo    Holds the media PresentationInfo to be set
 *
 * @return
 *    - JC_OK on success
 *    - JC_ERR_MSG_INVALID_MEDIA_INDEX If invalid media ID is passed
 *
 *
 * This API is used to set the presentation information for a specific media object
 *
 * @note
 *    - Caller should not free the resources allocated for pPresentationInfo members
 *
 * @sa jdi_MsgGetMediaPresentationInfo(), jdi_MsgGetMediaMimeInfo()
*/
extern JC_RETCODE	jdi_MsgSetMediaPresentationInfo (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
									JC_UINT32 uiMediaID, MEDIA_PRESENTATION_INFO *pPresentationInfo);

/**
 * @ingroup    Compose_Module
 *
 * @brief      Sets the DRM type for the requested media object.
 * @param[in]  hMsg	Specifies handle to message
 * @param[in]  ucPageIndex	Specifies the page index
 * @param[in]  uiMediaID        Specifies media ID for requested media
 * @param[in]  eDRMType		Specifies DRM Type
 * @param[in]  bIsAttachment    Specifies whether media is inline or attachment
 *
 * @return
 *    - JC_OK on success
 *    - JC_ERR_MSG_INVALID_MEDIA_INDEX If invalid media ID is passed
 *
 *
 * This API is used to set the DRM type for the requested media object
 *
 * @sa jdi_MsgGetMediaDRMType()
*/
extern JC_RETCODE	jdi_MsgSetMediaDRMType (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
									JC_UINT32 uiMediaID, E_DRM_TYPE eDRMType ,
									JC_BOOLEAN bIsAttachment);
/**
 * @ingroup    Compose_Module
 *
 * @brief      Gets DRM Type of the requested media object.
 * @param[in]  hMsg	Specifies handle to message
 * @param[in]  ucPageIndex	Specifies the page index
 * @param[in]  uiMediaID        Specifies media ID for requested media
 * @param[out]  peDRMType	On success, filled with DRM Type of the media
 * @param[in]  bIsAttachment    Specifies whether media is inline or attachment
 *
 * @return
 *    - JC_OK on success
 *    - JC_ERR_MSG_INVALID_MEDIA_INDEX If invalid media ID is passed
 *
 *
 * This API is used to get the DRM type for the requested media object
 *
 * @sa jdi_MsgSetMediaDRMType()
*/

extern JC_RETCODE	jdi_MsgGetMediaDRMType (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
									JC_UINT32 uiMediaID, E_DRM_TYPE *peDRMType ,
									JC_BOOLEAN bIsAttachment);

/**
 * @ingroup    Compose_Module
 *
 * @brief      Retrieves the MIME Info for the requested media object.
 * @param[in]  hMsg                 Specifies handle to message
 * @param[in]  ucPageIndex		    Specifies Page index
 * @param[in]  uiMediaID		    Specifies media id for the requested media
 * @param[out] pMediaMimeInfo		On success, holds the media MIME Info
 * @param[in]  bIsAttachment	    E_TRUE for attachment
 *
 * @return
 *    - JC_OK on success
 *    - JC_ERR_MSG_INVALID_MEDIA_INDEX If invalid media ID is passed
 *
 * This API is used to get the mime info for a specific media object
 *
 * @note
 *    - Caller should not modify pMediaMimeInfo returned by this API
*/
extern JC_RETCODE	jdi_MsgGetMediaMimeInfo (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
									JC_UINT32 uiMediaID, MEDIA_MIME_INFO *pMediaMimeInfo,
									JC_BOOLEAN bIsAttachment);


/**
 * @ingroup    Compose_Module
 *
 * @brief      Retrieves number of media objects in the requested page or attachment list
 *
 * @param[in]  hMsg         Specifies handle to message
 * @param[in]  ucPageIndex	Specifies page index. Not required if bIsAttachment = E_TRUE
 * @param[out] pMediaCount	Specifies pointer to hold the media count on success
 * @param[in]  bIsAttachment	 E_FALSE for inline and E_TRUE for attachment
 *
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX	If bIsAttachment is E_FALSE and ucPageIndex is not valid
 *
 * This API is used to get the number of media objects in the requested page or attachment list
 *
 * @sa jdi_MsgGetMediaList().
*/
extern JC_RETCODE	jdi_MsgGetMediaCount (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
											JC_UINT8 *pMediaCount, JC_BOOLEAN bIsAttachment);


/**
 * @ingroup    Compose_Module
 *
 * @brief      Sets the creation mode for message.
 * @param[in]  hMsg          Specifies handle to message
 * @param[in]  eCreationMode Specifies creation mode to be set
 *
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_MSG_MMS_CONTENT_RESTRICTED    If the new creation mode (E_CREATION_MODE_RESTRICTED) can not be set (existing media list violates)
 *    - JC_ERR_MSG_MMS_CONTENT_WARNING    If the new creation mode (E_CREATION_MODE_WARNING) can not be set (existing media list violates)
 *
 * This API is used to set the creation mode for the message (while composing)
 *
 * @note
 *    - Valid only for MMS.
 *
 * @sa jdi_MsgGetCreationMode().
*/
extern JC_RETCODE	jdi_MsgSetCreationMode (MSG_HANDLE hMsg, ECreationMode eCreationMode);


/**
 * @ingroup    Compose_Module
 * @brief      Retrieves the creation mode of message.
 * @param[in]  hMsg          Specifies handle to message
 * @param[out] pCreationMode Specifies pointer to hold the creation mode
 * @return
 *    - JC_OK	On success
 *
 * This API is used to get the creation mode of the message
 *
 * @note
 *    - This API applies only to MMS.
 *
 * @sa jdi_SetCreationMode().
*/
extern JC_RETCODE	jdi_MsgGetCreationMode (MSG_HANDLE hMsg, ECreationMode *pCreationMode);


/**
 * @ingroup    Compose_Module
 * @brief      Sets the content class for message.
 * @param[in]  hMsg          Specifies handle to message
 * @param[in]  eContentClass Specifies content class to be set
 *
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_MSG_MMS_CONTENT_CLASS	If the new content class violates the existing media list
 *    - JC_ERR_MSG_MMS_CONTENT_RESTRICTED    If the new content class can not be set (existing media list violates)
 *    - JC_ERR_MSG_MMS_CONTENT_WARNING    If the new content class can not be set (existing media list violates)
 *
 * This API is used to set the content class for the message.
 *
 * @note
 *    - This API applies only to MMS.
 *
 * @sa jdi_MsgGetContentClass()
*/
extern JC_RETCODE	jdi_MsgSetContentClass (MSG_HANDLE hMsg, EContentClass eContentClass) ;


/**
 * @ingroup    Compose_Module
 * @brief      Retrieves the content class of message.
 * @param[in]  hMsg          Specifies handle to message
 * @param[out] pContentClass On success points to the content class
 * @return
 *    - JC_OK	Success
 *
 * @note
 *    - This API applies only to MMS.
 *
*/
extern JC_RETCODE	jdi_MsgGetContentClass (MSG_HANDLE hMsg, EContentClass *pContentClass) ;


/**
 * @ingroup    Compose_Module
 * @brief      Sets maximum message size for message composition.
 * @param[in]  hMsg          Specifies handle to message
 * @param[in]  uiMaxMsgSize	 Specifies size in bytes[Pass 0 to reset the limit]
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_MSG_MAX_REACHED	If current size of the message is greater than the new limit (uiMaxMsgSize).
 *    - JC_ERR_MEMORY_ALLOCATION	If memory allocation failed.
 *
 * This API is used to set the limit for message size (composing)
 *
 * @note
 *		Pass uiMaxSize = 0 to set the limit to maximum
 *
 * @sa jdi_MsgGetMaxSize()
*/
extern JC_RETCODE	jdi_MsgSetMaxSize (MSG_HANDLE hMsg, JC_UINT32 uiMaxMsgSize);


/**
 * @ingroup    Compose_Module
 * @brief      Retrieves maximum message size for composing.
 * @param[in]  hMsg          Specifies handle to message
 * @param[out] pMaxMsgSize	 Specifies pointer to hold the size[if 0, limit is not set]
 * @return
 *    - JC_OK	Success
 *
 * This API is used to get the limit for message size (composing)
 *
 * @note
 *		*pMaxMsgSize = 0 specifies no limit set (maximum)
 *
 * @sa jdi_MsgSetMaxSize()
*/
extern JC_RETCODE	jdi_MsgGetMaxSize (MSG_HANDLE hMsg, JC_UINT32 *pMaxMsgSize);


/**
 * @ingroup    Compose_Module
 
 * @param[in]  hMsg         Specifies handle to message
 * @param[in]  ucMaxPages	Specifies the limit value[Pass 0 to reset the limit]
 * @return
 *    - JC_OK on success
 *    - JC_ERR_MSG_MAX_REACHED If the new limit is less than the number of pages currently exists
 *							   (or) If the messaging type is not MMS
 *							   (or) If the messaging type is MMS and Presentation is not SMIL.
 *    - JC_ERR_MEMORY_ALLOCATION If memory allocation fails
 *
 * This API is used to set the limit for number of pages/slides (composing)
 *
 * @note
 *		Pass ucMaxPages = 0 to set the limit to maximum
 *
 * @sa jdi_MsgGetMaxPages()
*/
extern JC_RETCODE	jdi_MsgSetMaxPages (MSG_HANDLE hMsg, JC_UINT8 ucMaxPages);


/**
 * @ingroup    Compose_Module
 
 * @param[in]  hMsg       Specifies handle to message
 * @param[out] pMaxPages  Specifies	pointer to hold the limit[if 0, limit is not set]
 * @return
 *    - JC_OK on success
 *
 *
 * This API is used to get the maximum pages supported for the message (composing)
 *
 * @note
 *		*pMaxPages = 0 specifies no limit set (maximum)
 *
 * @sa jdi_MsgSetMaxPages()
*/
extern JC_RETCODE	jdi_MsgGetMaxPages (MSG_HANDLE hMsg, JC_UINT8 *pMaxPages);

/**
 * @ingroup    Compose_Module
 * @brief      Adds a new page to the message.
 * @param[in]  hMsg        Specifies handle to message
 * @param[in]  ucPageIndex Specifies page index
 * @return
 *    - JC_OK on success.
 *    - JC_ERR_MSG_MAX_REACHED If current page count of the message
 *                             is greater than earlier set page limit.
 *    - JC_ERR_UNSUPPORTED If the messaging type is not MMS OR
 *                         If the messaging type is MMS but Presentation is not SMIL
 *    - JC_ERR_MEMORY_ALLOCATION If memory allocation failed.
 *
 *
 * This API is used to add/insert a new page to the message (composing)
 *
 * @note
 *		If <I>ucPageIndex</I> = 0 or greater than the current page count, then page will be appended at the end of message
 *
 * @sa jdi_MsgSetMaxPages(), jdi_MsgGetMaxPages()
*/
extern JC_RETCODE	jdi_MsgAddPage (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex);

/**
 * @ingroup    Compose_Module
 * @brief      Swaps pages in a message.
 * @param[in]  hMsg                 Specifies handle to message
 * @param[in]  ucSrcPageIndex       Specifies source page index
 * @param[in]  ucTargetPageIndex    Specifies destination page index
 * @return
 *    - JC_OK on success
 *    - JC_ERR_INVALID_STATE If view/preview is already running
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX If ucSourceIndex or/and ucTargetPageIndex is invalid
 *
 * This API is used to swap 2 slides/pages (composing)
*/
extern JC_RETCODE	jdi_MsgSwapPage (MSG_HANDLE hMsg, JC_UINT8 ucSrcPageIndex,
									JC_UINT8 ucTargetPageIndex) ;

/**
 * @ingroup    Compose_Module
 * @brief      Moves a page/slide to new position in the message.
 * @param[in]  hMsg        Specifies handle to message
 * @param[in]  ucSrcIndex  Specifies source slide/page index
 * @param[in]  ucDestIndex Specifies destination slide/page index
 * @return
 *    - JC_OK on success
 *    - JC_ERR_INVALID_STATE If view/preview is running
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX If ucSrcIndex or ucDestIndex is invalid.
 *
 * This API is used to move a specified slide/page to new location (re-arranges the slides/pages)
*/
extern JC_RETCODE	jdi_MsgMovePage (MSG_HANDLE hMsg, JC_UINT8 ucSrcIndex,
									JC_UINT8 ucDestIndex) ;

/**
 * @ingroup    Compose_Module
 * @brief      Deletes a page from the message.
 * @param[in]  hMsg        Specifies handle to message
 * @param[in]  ucPageIndex Specifies page index to be deleted
 * @return
 *    - JC_OK on success
 *    - JC_ERR_INVALID_STATE If view/preview is running
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX	If ucPageIndex is invalid index
 *
 * This API is used to delete the specified slide/page in the message
*/
extern JC_RETCODE	jdi_MsgDeletePage (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex);


/**
 * @ingroup    Compose_Module
 * @brief      Sets the play duration (time) for a page.
 * @param[in]  hMsg        Specifies handle to the message
 * @param[in]  ucPageIndex Specifies page index
 * @param[in]  uiDuration  Specifies play duration in milliseconds
 * @return
 *    - JC_OK on success
 *    - JC_ERR_INVALID_STATE If view/preview is running
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX If ucPageIndex is invalid page index.
 *    - JC_ERR_MEMORY_ALLOCATION If memory allocation fails
 *
 * This API is used to set the play duration for the specified slide/page
 *
 * @note
 *    - This API is applicable only to MMS with SMIL presentation
 *
 * @sa jdi_MsgGetPageTimer()
*/
extern JC_RETCODE	jdi_MsgSetPageTimer (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
											JC_UINT32 uiDuration);


/**
 * @ingroup    Compose_Module
 * @brief      Gets the play duration (time) set for a page.
 * @param[in]  hMsg        Specifies handle to message
 * @param[in]  ucPageIndex Specifies page index
 * @param[out] pDuration   Specifies pointer to hold the play duration [milliseconds]
 * @return
 *    - JC_OK on success
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX If ucPageIndex is invalid index
 *
 * This API is used to get the play duration for the specified slide/page
 *
 * @note
 *    - The resources for <I>pDuration</I> should be allocated and freed by caller.
 *    - This API is applicable only to MMS with SMIL presentation.
 *
 * @sa jdi_MsgSetPageTimer()
*/
extern JC_RETCODE	jdi_MsgGetPageTimer (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
											JC_UINT32 *pDuration);


/**
 * @ingroup    Compose_Module
 * @brief      Gets the total number of pages in a message
 * @param[in]  hMsg        Specifies handle to message
 * @param[out] pPageCount  Specifies pointer to hold total page count
 * @return
 *    - JC_OK on success
 *
 *
 * This API is used to get the total number of pages in the message
 *
 * @note
 *	- The resource for <I>pPageCount</I> should be allocated and freed by the caller.
*/
extern JC_RETCODE	jdi_MsgGetTotalPages (MSG_HANDLE hMsg, JC_UINT8 *pPageCount);


/**
 * @ingroup    Compose_Module
 * @brief      Gets the currently active page index
 * @param[in]  hMsg        Specifies handle to message
 * @param[out] pPageIndex  Specifies pointer to hold the page index [valid
 *                         only when View is running]
 * @return
 *    - JC_OK on success
 *    - JC_ERR_NULL_POINTER If hMsg or pPageIndex param is NULL
 *
 * This API is used to get the active page index
 *
 * @note
 *		- Valid only when view/preview is active
 *		- The resource for <I>pPageIndex</I> should be allocated and freed by the caller.
*/
extern JC_RETCODE	jdi_MsgGetCurrentPageIndex (MSG_HANDLE hMsg, JC_UINT8 *pPageIndex);


/**
 * @ingroup    Compose_Module
 * @brief      Gets properties of a page.
 * @param[in]  hMsg        Specifies handle to message
 * @param[in]  ucPageIndex Specifies page index
 * @param[out] ppPageInfo  Specifies pointer to the requested page properties [on success]
 * @return
 *    - JC_OK on success
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX If the page index passed as ucPageIndex is invalid
 *
 * This API is used to get the page/slide properties
 *
 * @note
 *		- Caller should not modify the information in ppPageInfo returned by this API
*/
extern JC_RETCODE	jdi_MsgGetPageInfo (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
											PAGE_INFO **ppPageInfo);

/**
 * @ingroup    Compose_Module
 * @brief      Sets the timer for a media object.
 * @param[in]  hMsg        Specifies handle to message
 * @param[in]  ucPageIndex Specifies requested page index
 * @param[in]  uiMediaID   Specifies requested media object Id
 * @param[in]  MediaTimer  Specifies timer information to be set
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_INVALID_STATE	If this API is called while Preview/View is active
 *    - JC_ERR_MSG_INVALID_MEDIA_INDEX	If uiMediaID is not found in the page
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX	If ucPageIndex is invalid (not found)
 *    - JC_ERR_MEMORY_ALLOCATION	If memory allocation fails.
 *
 * This API is used to set the media timer for a specific object
 *
 * @note
 *    - This API is applicable only for MMS with SMIL presentation
*/
extern JC_RETCODE	jdi_MsgSetMediaTimer (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
										JC_UINT32 uiMediaID, MEDIA_TIMER MediaTimer);


/**
 * @ingroup    Compose_Module
 * @brief      Retrieves a media object's timer information.
 * @param[in]  hMsg        Specifies handle to message
 * @param[in]  ucPageIndex Specifies requested page index
 * @param[in]  uiMediaID   Specifies requested media object Id
 * @param[out] pMediaTimer Specifies pointer to hold the timer information
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_MSG_INVALID_MEDIA_INDEX	If uiMediaID is not found in the page
 *
 * This API is used to get the media timer for a specific object
 *
 * @note
 *		- The resource for <I>pMediaTimer</I> should be allocated and freed by the caller.
*/
extern JC_RETCODE	jdi_MsgGetMediaTimer (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
										JC_UINT32 uiMediaID, MEDIA_TIMER *pMediaTimer);


/**
 * @ingroup    Compose_Module
 * @brief      Retrieves the size of a page or a message.
 * @param[in]  hMsg        Specifies handle to message
 * @param[in]  ucPageIndex Specifies requested page index [Pass 0 to get message size]
 * @param[out] pSize	   Specifies pointer to hold the size
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_MSG_INVALID_PAGE_INDEX	If ucPageIndex is invalid (not found)
 *
 * This API is used to get the size of message or a page (in bytes)
 *
 * @note
 *		- Pass ucPageIndex = 0 to get message size
 *		- The resource for <I>pSize</I> should be allocated and freed by the caller.
*/
extern JC_RETCODE	jdi_MsgGetSize (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex, JC_UINT32 *pSize);

/**
 * @ingroup    Compose_Module
 * @brief      Retrieves approximate size of message buffer (encoding).
 * @param[in]  hMsg        Specifies handle to message
 * @param[out] pSize	   Specifies pointer to hold the size
 * @return
 *    - JC_OK	Success
 *
 * This API is used to get the approximate size of message encoding(in bytes)
 *
 * @note
 *		- The resource for <I>pSize</I> should be allocated and freed by the caller.
*/
extern JC_RETCODE	jdi_MsgGetMsgEncodeSize (MSG_HANDLE hMsg, JC_UINT32 *pSize) ;

/**
 * @ingroup    Compose_Module
 * @brief      Sets the presentation type for a message
 * @param[in]  hMsg              Specifies handle to message
 * @param[in]  ePresentationType Specifies presentation type to be set
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_MEMORY_ALLOCATION	If memory allocation failed.
 *
 * This API is used to set the presentation type for the message
 *
*/
extern JC_RETCODE	jdi_MsgSetPresentationType (MSG_HANDLE hMsg,
													EPresentationType ePresentationType);

/**
 * @ingroup    Compose_Module
 * @brief      Retrieves the presentation type of a message
 * @param[in]  hMsg              Specifies handle to message
 * @param[out] pPresentationType Specifies pointer to hold presentation type
 * @return
 *    - JC_OK	Success
 *
 * This API is used to get the presentation type of the message
*/
extern JC_RETCODE	jdi_MsgGetPresentationType (MSG_HANDLE			hMsg,
												EPresentationType	*pPresentationType);

/**
 * @brief This function gets the original presentation type based on the message content type
 * @param[in]  hMsg				   Message Handle
 * @param[out] pPrensentationType  Reference to hold the presentation type
 * @return No return value 
 */
extern void jdi_MsgGetOrigPresentationType (MSG_HANDLE			hMsg, 
											EPresentationType	*pPresentationType) ;

/** @} */ // end of Compose_Module


/** @defgroup SMIL_Module  SMIL Module
 *  @{
 */

/**
 * @ingroup    SMIL_Module
 * @brief      Sets the root layout for the presentation.
 * @param[in]  hMsg     Specifies handle to message
 * @param[in]  uiHeight	Specifies the root-layout height
 * @param[in]  uiWidth	Specifies the root-layout width
 * @param[in]  pBGColor	Specifies the SMIL background color
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_MSG_PRESENTATION_NOT_SET	If presentation type
 *							is not set prior to this call (jdi_MsgSetPresentationType)
 *    - JC_ERR_UNSUPPORTED	If current presentation type is not E_PRESENTATION_SMIL
 *
 * This API is used to set the SMIL root layout information (height, width and background color)
 *
 * @note
 *	- Caller should allocate and free the resource for <I>pBGColor</I>.
 *
 * @sa jdi_MsgSetPresentationType()
*/
extern JC_RETCODE	jdi_MsgSetSMILRootLayout (MSG_HANDLE hMsg, JC_UINT32 uiHeight,
												JC_UINT32 uiWidth, JC_INT8 *pBGColor);


/**
 * @ingroup    SMIL_Module
 * @brief      Retrieves the root layout for the presentation.
 * @param[in]  hMsg     Specifies handle to message
 * @param[out] pHeight	Specifies the returned root-layout height
 * @param[out] pWidth	Specifies the returned root-layout width
 * @param[out] ppBGColor Specifies the returned root-layout background color
 * @return
 *    - JC_OK	On success
 *    - JC_ERR_MSG_PRESENTATION_NOT_SET	If presentation type is not set prior to this call (jdi_MsgSetPresentationType)
 *    - JC_ERR_UNSUPPORTED	If presentation type is not E_PRESENTATION_SMIL
 *
 * This API is used to get the SMIL root layout information (height, width and background color)
 *
 * @note
 *    - Caller should not alter <I>ppBGColor</I> returned by this API.
*/
extern JC_RETCODE	jdi_MsgGetSMILRootLayout (MSG_HANDLE hMsg, JC_UINT32 *pHeight,
												JC_UINT32 *pWidth, JC_INT8 **ppBGColor);

/**
 * @ingroup    SMIL_Module
 * @brief      Adds a region layout information for the presentation.
 * @param[in]  hMsg          Specifies handle to message
 * @param[in]  pRegionLayout Specifies a region-layout information.
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_MSG_PRESENTATION_NOT_SET	If presentation type is not set prior to this call (jdi_MsgSetPresentationType)
 *    - JC_ERR_UNSUPPORTED	If presentation type is not E_PRESENTATION_SMIL
 *    - JC_ERR_MEMORY_ALLOCATION	If memory allocation fails
 *
 * This API is used to add a SMIL region layout information
 *
 * @note
 *    - Only one region is added per call
 *	  - Caller should allocate and free the resources for <I>pRegionLayout</I>
*/
extern JC_RETCODE	jdi_MsgAddSMILRegionLayout (MSG_HANDLE hMsg,
														REGION_LAYOUT *pRegionLayout) ;


/**
 * @ingroup    SMIL_Module
 * @brief      Retrieves the region layout information for the presentation.
 * @param[in]  hMsg         Specifies handle to message
 * @param[out] ppRegionLayout	Specifies the region list
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_MSG_PRESENTATION_NOT_SET	If presentation type is not set prior to this call (jdi_MsgSetPresentationType)
 *    - JC_ERR_UNSUPPORTED	If presentation type is not E_PRESENTATION_SMIL
 *    - JC_ERR_SMIL_REGION_NOT_SET	If SMIL regions are not set
 *
 * This API is used to get the SMIL regions list
 *
 * @note
 *    - Caller should not alter <I>ppRegionLayout</I> returned by this API.
 *
 * @sa jdi_MsgDeleteSMILRegionLayout()
*/
extern JC_RETCODE	jdi_MsgGetSMILRegionLayout (MSG_HANDLE hMsg,
														REGION_LAYOUT **ppRegionLayout);

/**
 * @ingroup    SMIL_Module
 * @brief      Deletes a region layout information from the presentation information.
 * @param[in]  hMsg             Specifies handle to message
 * @param[in]  pRegionName  Specifies the region name to be deleted.
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_MSG_PRESENTATION_NOT_SET	If presentation type is not set prior to this call (jdi_MsgSetPresentationType)
 *    - JC_ERR_UNSUPPORTED	If presentation type is not E_PRESENTATION_SMIL
 *    - JC_ERR_NOT_FOUND	If the specified region not found
 *
 * This API is used to delete a SMIL region from the list
 *
 * @note
 *	  - Caller should allocate and free the resources for <I>pRegionName</I>
*/
extern JC_RETCODE	jdi_MsgDeleteSMILRegionLayout (MSG_HANDLE hMsg, JC_CHAR *pRegionName) ;


/** @} */ // end of SMIL_Module


/** @defgroup View_Module View module
 *  @{
 */

/**
 * @ingroup    View_Module
 * @brief      Fills the information needed to render the message
 * @param[in]  hMsg     Specifies handle to message
 * @param[in]  pContent Specifies the message content (buffer). Should be NULL in case of preview.
 * @return
 *	- JC_OK on success
 *    - JC_ERR_MEMORY_ALLOCATION	If memory allocation fails
 *	- JC_ERR_MSG_INVALID_MESSAGING_TYPE If messaging type is not supported
 *	- JC_ERR_AWAITING_DRM_RESPONSE_CB Informational error code for non-blocking DRM parsing (waiting for the DRM Engine to complete the parsing)
 *
 * This API is used to initialize rendering. Rendering can be started by calling jdi_MsgViewHandleEvent() with E_VIEW_START event.
 * - If pContent is not NULL, then pContent will be used to parse and fill the information needed to render.
 * - If pContent is NULL (Preview), then hMsg information will be used to fill the information needed to render.
*
 * @note
 *	  - Caller should allocate and free the resources for <I>pContent</I>
 *
 * @sa jdi_MsgParseContinue(), jdi_MsgViewHandleEvent()
*/
extern JC_RETCODE	jdi_MsgView (MSG_HANDLE hMsg, CONTENT_DATA *pContent) ;


/**
 * @ingroup    View_Module
 * @brief      Prepares the new handle with the forward message in compose format
 * @param[in]  hMsg             Specifies the handle to message.
 * @param[in]  pContent         Specifies the message content. Pass NULL if hMsg already has parsed contents (jdi_MsgParseEncapsulatedBuffer)
 * @param[out] phNewMsgHandle	On success forwarded content will be filled in this handle
 *
 * @return
 *	- JC_OK on success
 *    - JC_ERR_MEMORY_ALLOCATION	If memory allocation fails
 *	- JC_ERR_MSG_INVALID_MESSAGING_TYPE If messaging type is not supported
 *
 * @note
 *    	- If pContent is not NULL, caller should set the messaging type (<I>jdi_MsgSetMessagingType()</I>)
 *      in hMsg before calling this API.
 *	- Callback functions can be set in hMsg before calling this API (ERROR_IND_CB, which will be used in case of content class violation while forwarding)
 *    	- Caller is responsible for allocating and freeing the resource for <I>pContent</I>
 *	- jdd_MsgConfigGetDefaultTextValueInfo() [jdd_MessagingConfig.h] will be invoked by the core to get the string to prepend with the subject field (eg: Fwd:)
 *
 * @sa jdi_MsgViewHandleEvent()
*/
extern JC_RETCODE	jdi_MsgForward (MSG_HANDLE hMsg, CONTENT_DATA *pContent,
										MSG_HANDLE *phNewMsgHandle);


/**
 * @ingroup    View_Module
 * @brief      Prepares the new handle with the reply message in compose format
 * @param[in]  hMsg             Specifies handle to message
 * @param[in]  pContent         Specifies the message content. Pass NULL if hMsg already has parsed contents (jdi_MsgParseEncapsulatedBuffer)
 * @param[in]  bReplyAll        Specifies whether reply (E_FALSE) or reply-all (E_TRUE)
 * @param[out] phNewMsgHandle	On success replied content will be filled in this handle
 *
 * @return
 *	- JC_OK on success
 *	- JC_ERR_MEMORY_ALLOCATION	If memory allocation fails
 *	- JC_ERR_MSG_INVALID_MESSAGING_TYPE If messaging type is not supported
 *
 * This API is used to reply a message.
 *
 * @note
 *	- If pContent is not NULL, caller should set the messaging type (<I>jdi_MsgSetMessagingType()</I>)
 *      in hMsg before calling this API.
 *	- Callback functions can be set in hMsg before calling this API (REPLY_CHARGING_IND_CB, which will be used if the message has reply-charging)
 *	- Caller is responsible for allocating and freeing the resource for <I>pContent</I>
 *	- jdd_MsgConfigGetDefaultTextValueInfo() [jdd_MessagingConfig.h] will be invoked by the core to get the string to prepend with the subject field (eg: Re:)
*/
extern JC_RETCODE	jdi_MsgReply (MSG_HANDLE hMsg, CONTENT_DATA *pContent,
									JC_BOOLEAN bReplyAll, MSG_HANDLE *phNewMsgHandle);


/**
 * @ingroup    View_Module
 * @brief      Handles the events generated during message display.
 * @param[in]  hMsg             Specifies handle to message
 * @param[in]  eEventType   	Specifies event type
 * @param[in]  pEventData	    Specifies event specific Data
 *
 * @return
 *	- JC_OK on success
 *	- JC_ERR_MEMORY_ALLOCATION	If memory allocation fails
 *	- Other error codes refer jdi_MMSErr.h
 *
 * This API is used to interact/perform actions or events with rendering engine.
*/
extern JC_RETCODE	jdi_MsgViewHandleEvent (MSG_HANDLE hMsg, EViewEventType eEventType,
												void *pEventData);


/**
 * @ingroup    View_Module
 * @brief      Retrieves information of active/focused media.
 * @param[in]  hMsg        Specifies handle to message
 * @param[out] pActiveMediaInfo  On success, holds the Active Media Information
 *
 * @return
 *	- JC_OK on success
 *	- JC_ERR_MEMORY_ALLOCATION	If memory allocation fails
 *
 * This API is used to get the information about the focused media object (while rendering)
 *
 * @note
 *    - Caller should not free the resource pActiveMediaInfo->pMediaInfo (and its members) returned by this API
*/
extern JC_RETCODE	jdi_MsgGetActiveMediaInfo (MSG_HANDLE hMsg, ACTIVE_MEDIA_INFO *pActiveMediaInfo);


/**
 * @ingroup    View_Module
 * @brief      Registers view specific callback function
 * @param[in]  hMsg             Specifies handle to message
 * @param[in]  eCallbackType    Specifies CallBack Function Type
 * @param[in]  pCallbackFunction Specifies CallBack Function Pointer
 *
 * @return
 *	  - JC_OK on success
 *	  - JC_ERR_INVALID_PARAMETER If eCallbackType is not supported
 *
 * This API is used to register various callbacks (see <I>EMsgCallbackTypes</I>), such as :
 *		- indicating scrollbar display, presence of attachments/audio/video/DRM contents
 *		- page change, start and end of presentation
 *		- Object focus change (rendering)
 *		- error Indication
 *		- DRM content parser
*/
extern JC_RETCODE 	jdi_MsgRegisterCallbacks (MSG_HANDLE hMsg,
												EMsgCallbackTypes eCallbackType, void *pCallbackFunction);

/**
 * @ingroup    View_Module
 * @brief      Retrieves the list of Link information from the text media of a page/slide.
 * @param[in]  hMsg             Specifies handle to message
 * @param[in]  ucPageIndex    Specifies the page index
 * @param[out]  ppLinkInfo On success points to the link info list
 *
 * @return
 *	  - JC_OK on success
 *	  - JC_ERR_MEMORY_ALLOCATION If memory allocation fails
 *
 * This API is used to get the Link information (URL's, Email ID's and Phone Numbers) from Text content.
 *
 * @note
 *    - Caller should free the resource allocated for ppLinkInfo .
*/
extern JC_RETCODE	jdi_MsgGetLinkInfo (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex, LINK_INFO **ppLinkInfo) ;

/** @} */// end of View_Module


/** @defgroup Msg_Controller_Module Message Controller Module
 *  @{
 */

/**
 * @ingroup    Msg_Controller_Module
 * @brief      Forms the encapsulated message buffer
 * @param[in]  hMsg Specifies handle to message
 * @param[out] pEncapsulatedContent	On success points to the encapsulated message content
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_MSG_HEADERS_NOT_SET	If Message headers are not filled
 *	- JC_ERR_UNSUPPORTED If PDU type is not supported *
 *	- JC_ERR_NULL_POINTER If PDU type is E_MSG_TYPE_MMBOX_UPLOAD and MMDESCR is not filled or mandatory headers not filled
 *	- JC_ERR_MEMORY_ALLOCATION If memory allocation fails.
 *	- Other error codes refer jdi_MMSErr.h
 *
 * This API is used to create the encapsulated buffer in the format specific to messaging type. Buffer is created based on the
 * message and header information filled by the caller.
 *
 * @note
 *	- On success, caller is responsible for freeing the message buffer returned in pEncapsulatedContent->pBuffer
 */
extern JC_RETCODE	jdi_MsgGetEncapsulatedBuffer (MSG_HANDLE hMsg,
								CONTENT_DATA *pEncapsulatedContent);


/**
 * @ingroup    Msg_Controller_Module
 * @brief      Parses encapsulated buffer and creates a message handle
 * @param[in]  phMsg                Specifies handle to message
 * @param[in]  pEncapsulatedContent	Specifies message content information to be parsed
 * @param[in]  eMessagingType       Specifies messaging type of the buffer
 * @param[in]  bWithMedia           Specifies whether only headers or header and media are needed
 *
 * @return
 *    - JC_OK	On Success
 *    - JC_ERR_MSG_INVALID_MESSAGING_TYPE	If invalid messaging type is passed
 *	- JC_ERR_UNSUPPORTED  If PDU type is not supported
 *    - JC_ERR_MEMORY_ALLOCATION	If memory allocation failed
 *	- Other error codes refer jdi_MMSErr.h
 *
 * This API is used to parse the encapsulated message buffer and form the handle.
 *
 * @note
 *    - Core will parse the message, initialize the new handle and fill the information.
 *    - Caller can get the message headers, media list, etc by calling appropriate API's.
 *    - Caller is responsible for allocating and freeing the resource
 *      for <I>pEncapsulatedContent</I> and its members
*/
extern JC_RETCODE	jdi_MsgParseEncapsulatedBuffer (MSG_HANDLE *phMsg,
						CONTENT_DATA *pEncapsulatedContent,
						EMessagingType eMessagingType, JC_BOOLEAN bWithMedia);

/**
 * @ingroup    Msg_Controller_Module
 * @brief      Sets the message headers.
 * @param[in]  hMsg Specifies handle to the message
 * @param[in]  pMsgHeaders	Specifies message header information
 *
 * @return
 *    - JC_OK	Success
 *
 * This API is used to set the message headers to be used for forming the encapsulated buffer
 *
 * @note
 *    - Caller should allocate the resource for <I>pMsgHeaders</I> and its members
 *    - Messaging core will use the same resource and will free the resource on calling <I>jdi_MsgDeinit()</I>.
 *    - Caller should not free the resource allocated for <I>pMsgHeaders</I> (members can be altered by the caller).
*/
extern JC_RETCODE	jdi_MsgSetMessageHeaders (MSG_HANDLE hMsg,
													MESSAGE_HEADERS *pMsgHeaders);


/**
 * @ingroup    Msg_Controller_Module
 * @brief      Retrieves message header information
 * @param[in]  hMsg Specifies handle to message
 * @param[out] ppMsgHeaders	Specifies returned message header information on success
 * @return
 *    - JC_OK	Success
 *
 * This API is used to get the message headers set by the caller (jdi_MsgSetMessageHeaders).
 *
 * @note
 *    - Caller should not free the resource returned by <I>ppMsgHeaders</I> (members can be altered by the caller).
 *	    Messaging library uses the same resource passed by the caller (jdi_MsgSetMessageHeaders)
 *
 * @sa jdi_MsgSetMessageHeaders()
*/
extern JC_RETCODE	jdi_MsgGetMessageHeaders (MSG_HANDLE hMsg,
													MESSAGE_HEADERS **ppMsgHeaders);


/**
 * @ingroup    Msg_Controller_Module
 * @brief      Sets encoding type for the message
 * @param[in]  hMsg Specifies handle to message
 * @param[in]  bIsTextEncoding	E_FALSE = binary encoding, E_TRUE = text encoding
 *
 * @return
 *    - JC_OK	Success
 *
 * This function sets the encoding type, whether Text or Binary
 * encoding to be used for creating the encapsulated buffer.
 * Based on the encoding type specified, the Messaging layer performs the encapsulation.
*/
extern JC_RETCODE	jdi_MsgSetEncodingType (MSG_HANDLE hMsg, JC_BOOLEAN bIsTextEncoding);


/**
 * @ingroup    Msg_Controller_Module
 * @brief      Sets the user defined parameters along with the Content-Type header.
 * @param[in]  hMsg     Specifies handle to message
 * @param[in]  pParams	User defined parameter list
 *
 * @return
 *    - JC_OK	Success
 *
 * This API is used to add user defined parameters along with the Content-Type header.
 *
 * @note
 *    - Caller should allocate the resources for pParams.
 *    - Caller should not free the resources after calling this API (core will use the same resource passed by the caller),
 *    - Core will free the resources on deinitialization of <I>hMsg</I>.
*/
extern JC_RETCODE	jdi_MsgSetContentTypeParams(MSG_HANDLE hMsg, MM1_HDR_PARAM *pParams) ;


/**
 * @ingroup    Msg_Controller_Module
 * @brief      This API retrieves the user defined parameters which have been set along with the
 *			   Content-Type header.
 * @param[in]  hMsg     Specifies handle to message
 * @param[out] ppParams	On success, points to the content type parameter list
 *
 * @return
 *    - JC_OK	Success
 *
 * This API is used to get the user defined parameters set by a previous call to jdi_MsgSetContentTypeParams()
 *
 * @note
 *    - Caller should not alter the resource ppParams returned by the API.
 *
 * @sa jdi_MsgSetContentTypeParams()
*/
extern JC_RETCODE	jdi_MsgGetContentTypeParams (MSG_HANDLE hMsg, MM1_HDR_PARAM **ppParams) ;

/**
 * @ingroup    Msg_Controller_Module
 * @brief      Sets the configuration identifier to be used in subsequent call to messaging configuration APIs
 * @param[in]  hMsg Specifies handle to message
 * @param[in]  uiConfigArg	Specifies the configuration value
 *
 * @return
 *    - JC_OK	Success
 *
 * This API is used to set the configuration identifier to be used in subsequent call to messaging configuration APIs
*/
extern JC_RETCODE 	jdi_MsgSetConfigIdentifier (MSG_HANDLE hMsg, JC_UINT32 uiConfigArg);

/** @} */ // End of Msg_Controller_Module

/** @defgroup Storage_Module Storage Module
 *    - For all folder related operations, jdi_StoreSetActiveFolder()
 *      should be called first to set the active folder.
 *    - For all filter related operations, jdi_StoreSetActiveFilter()
 *      should be  called first to set the active filter.
 *    - The starting index for messages in the message list is 1.
 *    - For operations related to messages (delete, move, swap).
 *      jdi_StoreGetMsgList() should be called to get the message list
 *      of active folder.
 *  @{
 */

/**
 * @ingroup    Storage_Module
 * @brief      Initializes the store module.
 * @param[out] phStore      Specifies the Store handle
 * @param[in]  uiMsgType    Points to user defined information
 *
 * @return
 *    - JC_OK	Success
 *	- JC_ERR_MEMORY_ALLOCATION	If memory allocation failed
 *	- File system specific errors
 *
 *	This API initializes the store module and fills the folder list.
 *  Initialized handle shall be used by the application for invoking
 *  the other <I>storage</I> module API calls.
*/
extern JC_RETCODE	jdi_StoreInit (STORE_HANDLE *phStore,JC_UINT32 uiMsgType);


/**
 * @ingroup    Storage_Module
 * @brief      This API is used to get the list of all the folders.
 * @param[in]  hStore       Specifies the Store handle
 * @param[out] ppFolderList	Specifies the returned folders list on success.
 *
 * @return     JC_OK on success
 *
 *	This API is used to get the list of all folders that are currently
 *	in MMS_FOLDER.bin file.
 *
 * @note
 *		- Caller should not free the resource ppFolderList returned by the API
*/
extern JC_RETCODE	jdi_StoreGetFolders (STORE_HANDLE hStore, FOLDER_INFO **ppFolderList);


/**
 * @ingroup    Storage_Module
 * @brief      Creates new folder
 * @param[in]  hStore       Specifies the Store handle
 * @param[in]  pFolderName  Specifies the name of folder to be created
 * @param[in]  uhCategory   [Not Used]
 * @return     JC_OK on success, error code on failure
 *
 * This API creates a new folder and adds it to the folder's list
 * A new persistent file is created with the same name as the folder
 * name passed to the function and MMS_FOLDER.bin file is updated
 * for the same.
 *
 * @note
 *    - Caller should allocate the resource for <I>pNewFolderName</I>.
 *    - Store will use the same resource and free it on jdi_StoreDeinit().
 *    - Caller should not free the resource on SUCCESS.
*/
extern JC_RETCODE 	jdi_StoreCreateFolder (STORE_HANDLE hStore, JC_CHAR *pFolderName,
										JC_UINT16 *uhCategory);


/**
 * @ingroup    Storage_Module
 * @brief      Renames a folder.
 * @param[in]  hStore Specifies the Store handle
 * @param[in]  pSrcFolderName Specifies old folder name
 * @param[in]  pDestFolderName Specifies new folder name
 * @param[in]  uhCategory [Not Used]
 * @return     JC_OK on success, error code on failure
 *
 * @note
 *	- <b>Precondition :</b> Before calling this API, pSrcFolderName should be set as active folder 
 *	- Caller should allocate the resource for pDestFolderName. On success, caller should 
 *		not free the resource allocated for <I>pDestFolderName</I>
 *    - Resource will be freed on calling jdi_StoreDeinit().
*/
extern JC_RETCODE 	jdi_StoreRenameFolder (STORE_HANDLE hStore, JC_CHAR *pSrcFolderName,
												JC_CHAR *pDestFolderName, JC_UINT16 uhCategory);


/**
 * @ingroup    Storage_Module
 * @brief      Deletes a folder
 * @param[in]  hStore Specifies the Store handle
 * @param[in]  uhCategory [Not Used]
 * @return
 *    - JC_OK on success
 *
 * This API deletes the active folder.
 *
 * @note Folder to be deleted should be set as active folder before calling this API.
*/
extern JC_RETCODE	jdi_StoreDeleteFolder (STORE_HANDLE hStore, JC_UINT16 uhCategory);


/**
 * @ingroup    Storage_Module
 * @brief      Sets the active folder for subsequent operations.
 * @param[in]  hStore Specifies the Store handle
 * @param[in]  pFolderName	Specifies the folder name to be set as active.
 * @return
 *    - JC_OK on success
 *    - JC_ERR_STORE_FOLDER_NOT_FOUND if <I>pFolderName</I> is not found
 *
 * @sa jdi_StoreGetActiveFolder()
*/
extern JC_RETCODE 	jdi_StoreSetActiveFolder (STORE_HANDLE hStore, JC_CHAR *pFolderName);


/**
 * @ingroup    Storage_Module
 * @brief      Retrieves the active folder name
 * @param[in]  hStore Specifies the Store handle
 * @param[out] ppcFolder On success points to the active folder name
 * @param[in]  puhCategory [Not Used]
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_ACTIVE_FOLDER_NOT_SET	If active folder is not set
 *
 * @note
 *    - Caller should not free resource returned by this api (ppcFolder).
 *    - jdi_StoreDeinit() will release the resource.
 *
 * @sa jdi_StoreSetActiveFolder()
*/
extern JC_RETCODE 	jdi_StoreGetActiveFolder (STORE_HANDLE hStore, JC_CHAR **ppcFolder,
										JC_UINT16 *puhCategory);


/**
 * @ingroup    Storage_Module
 * @brief      Retrieves message list
 * @param[in]  hStore Specifies the Store handle
 * @param[out] ppMessageList On success points to the message list
 *             NOTE : Caller should not modify this resource
 * @param[in]  pRetrieveFilters	Specifies filters for retrieving the list.
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_STORE_ACTIVE_FOLDER_NOT_SET	If active folder is not set (jdi_StoreSetActiveFolder)
 *    - File module specific errors	If file operation fails
 *
 * This API shall be called to get the list of message stored in particular folder.
 * Active folder should be set before calling this API
 * 
*/
extern JC_RETCODE 	jdi_StoreGetMsgList (STORE_HANDLE hStore, MESSAGE_LIST**ppMessageList,
									RETRIEVE_FILTERS *pRetrieveFilters);


/**
 * @ingroup    Storage_Module
 * @brief      Sorts the messages in the active folder
 * @param[in]  hStore       Specifies the Store handle
 * @param[in]  pSortInfo	    Specifies options to be used for sorting
 * @return
 *    - JC_OK On Success
 *
 * This API sorts the messages in the active folder, as specified by the sortinfo structure.
 * This API sorts only the messages that are retrieved by a previous call to jdi_StoreGetMsgList().
*/
extern JC_RETCODE 	jdi_StoreSortMsgList (STORE_HANDLE hStore, SORT_INFO *pSortInfo);


/**
 * @ingroup    Storage_Module
 * @brief      Deletes the message specified by the message index
 * @param[in]  hStore       Specifies the Store handle
 * @param[in]  uiMessageID	Specifies unique ID of the message to  be deleted
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_ACTIVE_FOLDER_NOT_SET	If active folder is not set
 *    - JC_ERR_STORE_INVALID_MSG_INDEX	If <I>uiMsgIndex</I> is invalid (not found)
 *
 * @note
 *  - jdi_StoreGetMsgList() should have been called prior to calling this API.
 *    - After calling this api, application should call jdi_StoreUpdateMsgList()
 *      to commit the changes.
*/
extern JC_RETCODE 	jdi_StoreDeleteMsg (STORE_HANDLE hStore, JC_UINT32 uiMessageID);

/**
 * @ingroup    Storage_Module
 * @brief      Moves the message to specified folder
 * @param[in]  hStore Specifies the Store handle
 * @param[in]  uiMessageID	Specifies unique ID of the message to be moved
 * @param[in]  pDestFolder		Specifies the folder name to which the message has
 *								to be moved
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_ACTIVE_FOLDER_NOT_SET	If active folder is not set
 *    - JC_ERR_STORE_FOLDER_NOT_FOUND	If pDestFolder is invalid (not found)
 *    - File module specific errors	If file operation fails
 *
 * @note
 *  - jdi_StoreGetMsgList() should have been called prior to calling this API.
 *    - After calling this api, application should call jdi_StoreUpdateMsgList()
 *      to commit the changes.
*/
extern JC_RETCODE 	jdi_StoreMoveMsg (STORE_HANDLE hStore, JC_UINT32 uiMessageID,
								JC_CHAR *pDestFolder);


/**
 * @ingroup    Storage_Module
 * @brief      Saves the message to the folder name passed by the application
 * @param[in]  hStore Specifies the Store handle
 * @param[in]  pFolderName	Specifies the folder name in which user want to save
 *							the message.
 * @param[in]  uhCategory	NOT USED
 * @param[in]  pMsgHeaders	Specifies the message headers
 * @param[in]  pMsgContent	Specifies the buffer details. Input can be file stream
 *							or inline buffer.
 * @param[out] pMsgIndex	On success, returns the index of the saved message.
 * @param[in]  uiExtraBytes	Specifies any user-defined information that needs to
 *							be saved along with the message.
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_FOLDER_NOT_FOUND	If pFolderName is not valid (not found)
 *    - File module specific errors	If file operation fails
*/
extern JC_RETCODE 	jdi_StoreSaveMsg (STORE_HANDLE hStore, JC_CHAR *pFolderName, JC_UINT16 uhCategory,
						MESSAGE_HEADERS *pMsgHeaders, CONTENT_DATA *pMsgContent,
						JC_UINT32 *pMsgIndex, JC_UINT32 uiExtraBytes) ;

/**
 * @ingroup    Storage_Module
 * @brief      Retrieves the entire message buffer for the specified message index
 * @param[in]  hStore Specifies the Store handle
 * @param[in]  uiMessageID	Specifies unique ID of message to be retrieved.
 * @param[in,out] pMsgContent	On success points to the message buffer details.
 *								Output can be file stream or inline buffer
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_ACTIVE_FOLDER_NOT_SET	If active folder is not set.
 *    - JC_ERR_STORE_INVALID_MSG_INDEX	If the message index is not valid (not found)
 *    - JC_ERR_FILE_OPEN If message content file is not able to open
 *    - JC_ERR_MEMORY_ALLOCATION	If memory allocation failed
 *    - JC_ERR_FILE_READ	If failed to read the message content from file
 *
 * @note
 *    - jdi_StoreGetMsgList() should have been called before calling this api.
 *    - uiMsgIndex (starts with 1) specifies the position of the message in
 *      the message list (returned by jdi_StoreGetMsgList())
 *    - Caller is responsible for freeing pMsgContent->pBuffer, pFileName
*/
extern JC_RETCODE 	jdi_StoreGetMsgFromStore (STORE_HANDLE hStore, JC_UINT32 uiMessageID, CONTENT_DATA *pMsgContent) ;


/**
 * @ingroup    Storage_Module
 * @brief      Updates the entire message with the specified buffer.
 * @param[in]  hStore       Specifies the Store handle
 * @param[in]  uiMessageID	Specifies unique message ID to be updated
 * @param[in]  pMsgHeaders	Specifies the message header information
 * @param[in]  pMsgContent	Specifies the new Message buffer information.
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_ACTIVE_FOLDER_NOT_SET	If active folder is not set.
 *    - JC_ERR_STORE_INVALID_MSG_INDEX	If uiMsgIndex is invalid (not found)
 *    - File module specific errors, if file operation fails
 *
*/
extern JC_RETCODE 	jdi_StoreUpdateMsg (STORE_HANDLE hStore, JC_UINT32 uiMessageID,
										MESSAGE_HEADERS *pMsgHeaders, CONTENT_DATA *pMsgContent) ;

/**
 * @ingroup    Storage_Module
 * @brief      Updates all the messages in the message list
 * @param[in]  hStore Specifies the Store handle
 * @param[in]  bflag  Specifies the need to update the message (E_TRUE)
 *                    or only headers (E_FALSE)
 * @return
 *    - JC_OK	Success
 *    - JC_ERR_STORE_ACTIVE_FOLDER_NOT_SET	If active folder is not set
 *    - File module specific errors	If file operation fails
 *
 * This function will update all the messages in the message list.
 * <I>bflag</I> specifies whether only the headers needs to be modified
 * or entire message needs to be modified.
 *
 * @note
 *    - For committing any change to message list nodes), application should
 *      set the eUpdateFlag to E_STORE_MODIFY (MESSAGE_LIST member)
 */
extern JC_RETCODE 	jdi_StoreUpdateMsgList (STORE_HANDLE hStore, JC_BOOLEAN bflag);


/**
 * @ingroup    Storage_Module
 * @brief      Deletes messages specified in <I>pMsgIndexArr</I>.
 * @param[in]  hStore       Specifies the store handle
 * @param[in]  pMessageIDArr	Specifies the array of message ID's to be deleted.
 * @param[in]  uiMsgCount	Specifies the number of messages to be deleted
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_ACTIVE_FOLDER_NOT_SET	If active folder is not set.
 *    - JC_ERR_STORE_INVALID_MSG_INDEX	If any of pMsgIndexArr is invalid (not found)
 *
 * @note
 *    - jdi_StoreDeleteMultipleMsgs will set the delete flag (eUpdateFlag) in
 *      the message list and not actually remove the message.
 *    - Application should call jdi_StoreUpdateMsgList()
 *      after calling jdi_StoreDeleteMultipleMsgs() for removing the message from the store.
*/
extern JC_RETCODE 	jdi_StoreDeleteMultipleMsgs (STORE_HANDLE hStore, JC_UINT32 *pMessageIDArr,
											JC_UINT32 uiMsgCount);


/**
 * @ingroup    Storage_Module
 * @brief      Deletes all the messages in the active folder
 * @param[in]  hStore Specifies the Store handle
 * @return
 *    - JC_OK on Success
 *    - JC_ERR_STORE_ACTIVE_FOLDER_NOT_SET	If active folder is not set.
 *    - JC_ERR_STORE_INVALID_MSG_INDEX	If any of pMsgIndexArr is invalid (not found)
 *
 * @note
 *    - jdi_StoreDeleteAllMsgs() will set the delete flag (eUpdateFlag)
 *      in the message list and not actually remove the message.
 *    - Application should call jdi_StoreUpdateMsgList()
 *      after calling jdi_StoreDeleteAllMsgs() for removing the message from the store.
*/
extern JC_RETCODE 	jdi_StoreDeleteAllMsgs (STORE_HANDLE hStore);

/**
 * @ingroup		Storage_Module
 * @brief		This function searches the messages in the active folder for the specified search info. 
 *				This function searches only the messages that are retrieved by a previous call to jdi_StoreGetMsgList.
 *
 * @param[in]	hStore Specifies the Store handle
 * @param[in]	pstSearchInfo	Specifies the information required for search
 *
 * @return
 *    - JC_OK on Success
 *    - JC_ERR_MEMORY_ALLOCATION	If memory allocation fails.
 *
 * @note
 *	- Core will allocate the resource for pstSearchInfo->pMessageIDList. 
 *		Application is responsible for freeing the resource.
 *
 */
extern JC_RETCODE jdi_StoreSearch (STORE_HANDLE hStore, ST_STORE_SEARCH_INFO *pstSearchInfo) ;

/**
 * @ingroup    Storage_Module
 * @brief      Stores the filters for receiving messages.
 * @param[in]  hStore Specifies the Store handle
 * @param[in]  pRecvFilters	Specifies the new filter information to be added.
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_FILTER_ALREADY_EXIST	If pRecvFilters name already exists
 *    - File module specific errors	If file operation fails
 *
 * This function shall store the filters for receiving the Message.
 * This applies only to MMS.
 *
 * @note
 *    - Caller should allocate the resource for <I>pRecvFilters</I>.
 *    - On success, caller should not free the resource (freed internally)
*/
extern JC_RETCODE 	jdi_StoreAddReceiveFilters (STORE_HANDLE hStore, RECV_FILTERS *pRecvFilters);


/**
 * @ingroup    Storage_Module
 * @brief      Retrieves all the receive filters.
 * @param[in]  hStore         Specifies the Store handle
 * @param[out] pRecvFilters   On success, pRecvFilters will point to the receive filters.
 * @return
 *    - JC_OK On Success
 *
 * @note
 *    - Caller should not free the resource returned by this API
*/
extern JC_RETCODE 	jdi_StoreGetReceiveFilters (STORE_HANDLE hStore, RECV_FILTERS **pRecvFilters);


/**
 * @ingroup    Storage_Module
 * @brief      Deletes Active Filter
 * @param[in]  hStore Specifies the Store handle
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_ACTIVE_FILTER_NOT_SET	If active filter is not set (jdi_StoreSetActiveFilter)
 *    - File module specific errors	If file operation fails
 *
 * @note
 *    - jdi_StoreSetActiveFilter() Should be called before calling this API
*/
extern JC_RETCODE 	jdi_StoreDeleteFilter (STORE_HANDLE hStore);


/**
 * @ingroup    Storage_Module
 * @brief      Sets the active filter for subsequent operations
 * @param[in]  hStore       Specifies the Store handle
 * @param[in]  pFilterName  Specifies the Receive filter name, to be set as active filter.
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_FILTER_NOT_FOUND	If pFilterName is invalid (not found)
*/
extern JC_RETCODE 	jdi_StoreSetActiveFilter (STORE_HANDLE hStore, JC_CHAR *pFilterName);


/**
 * @ingroup		Storage_Module
 * @brief      Retrieves active filter name.
 * @param[in]  hStore Specifies the Store handle
 * @param[out] ppFilterName On success specifies the currently active filter name
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_FILTER_NOT_FOUND	If <I>ppFilterName</I> is invalid (not found)
 *
 * @note
 *    - Caller should not free or alter <I>ppFilterName</I> returned by this API
*/
extern JC_RETCODE 	jdi_StoreGetActiveFilter (STORE_HANDLE hStore, JC_CHAR **ppFilterName);

/**
 * @ingroup    Storage_Module
 * @brief      Validates received message with active filter
 * @param[in]  hStore Specifies the Store handle
 * @param[in]  pMsgHeaders	Specifies the message headers to be validated
 * @return
 *    - JC_OK On Success
 *    - JC_ERR_STORE_FILTER_NOT_FOUND	If filter is invalid (not found)
 *    - JC_ERR_STORE_FILTER_MSG_BLOCKED	If the message needs be blocked (filter blocked list)
*
 * This function shall validate the received message with the active filter. After
 * receiving a new message, application can call this API to filter the message
 * before storing.
*/
extern JC_RETCODE 	jdi_MsgValidateFilters (STORE_HANDLE hStore, MESSAGE_HEADERS *pMsgHeaders);

/**
 * @ingroup		Storage_Module
 * @brief		This API edits the existing filter
 * @param[in]	hStore Specifies the storage handle
 * @param[in]	pRecvFilters Specifies the receive filter to be edited
 * @return
 *    - JC_OK on success
 *    - JC_ERR_STORE_FILTER_NOT_FOUND If pRecvFilter name doesn't exist
 *    - Other file operation related errors.
 *
 */
extern JC_RETCODE jdi_StoreEditReceiveFilters (STORE_HANDLE hStore, RECV_FILTERS *pRecvFilters) ;

/**
 * @ingroup		Storage_Module
 * @brief		This API rename existing filter
 * @param[in]	hStore Specifies the storage handle
 * @param[in]	pSrcFilterName Specifies the filter to be renamed
 * @param[in]   pNewFilterName Specifies the new name
 *
 * @return
 *    - JC_OK on success
 *    - JC_ERR_STORE_FILTER_NOT_FOUND If pSrcFilterName name doesn't exist
 *    - Other file operation related errors.
 *
 */
extern JC_RETCODE jdi_StoreRenameFilter(STORE_HANDLE hStore, JC_CHAR *pSrcFilterName, JC_CHAR *pNewFilterName) ;

/**
 * @ingroup    Storage_Module
 * @brief      Restores the folder information.
 * @param[in]  hStore Specifies the Store handle
 * @return
 *    - JC_OK On Success
 *    - File module specific errors	If file operation fails
 */
extern JC_RETCODE jdi_StoreRestoreFolderInfo (STORE_HANDLE hStore);

/**
 * @ingroup    Storage_Module
 * @brief      De - initializes the store module
 * @param[in]  hStore Specifies the store handle
 * @return
 *    - JC_OK on success
 *
 * This function de-initializes the storage module and
 * frees all the resources that are used by the store module.
 * This is the last call that needs to be called by the
 * application. After this call the Store handle becomes invalid.
*/
extern JC_RETCODE 	jdi_StoreDeinit (STORE_HANDLE hStore);

/** @} */ // End of Storage_Module
#ifdef	__cplusplus
	}
#endif

#endif	//_MESSAGING_API_H_

