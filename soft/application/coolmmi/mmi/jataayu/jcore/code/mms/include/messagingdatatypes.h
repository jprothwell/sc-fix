/**
 * @file    MessagingDataTypes.h
 * @brief   Declares the messaging specific data types.
 */
 /***************************************************************************
 *
 * File Name : MessagingDataTypes.h
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

#ifndef _MESSAGING_DATATYPES_H_
	#define _MESSAGING_DATATYPES_H_

#include <mm1datatypes.h>

typedef void * 		MSG_HANDLE ;	/**< Messaging Handle */
typedef void * 		STORE_HANDLE ;	/**< Store Handle */


/**
 * @brief Enumeration of message types
 *
 * Specifies the supported messaging types
 */
typedef enum _messaging_type
{
	E_TYPE_MMS = 0x00,	/**< For MMS Type. This type is default */
	E_TYPE_EMAIL,		/**< For Email Type */
	E_TYPE_EMS			/**< For EMS Type */
} EMessagingType ;


/**
 * @brief Enumeration of Creation modes
 *
 * Specifies the supported creation modes (applicable only for MMS).
 */
typedef enum _creation_mode
{
	E_CREATION_MODE_FREE,		/**< Specifies <B>Free</B> creation mode */
	E_CREATION_MODE_WARNING,	/**< Specifies <B>Warning</B> creation mode */
	E_CREATION_MODE_RESTRICTED	/**< Specifies <B>Restricted</B> creation mode*/
} ECreationMode ;


/**
 * @brief Enumeration of Content Class
 *
 * Specifies the supported content classes (applicable only for MMS).
 */
typedef enum _content_class
{
	E_MMS_CONTENT_CLASS_TEXT,		/**< Specifies <I>Text</I> content class */
	E_MMS_CONTENT_CLASS_IMAGE_BASIC,/**< Specifies <I>Image Basic</I> content class*/
	E_MMS_CONTENT_CLASS_IMAGE_RICH,	/**< Specifies <I>Image Rich</I> content class*/
	E_MMS_CONTENT_CLASS_VIDEO_BASIC,/**< Specifies <I>Video Basic</I> content class*/
	E_MMS_CONTENT_CLASS_VIDEO_RICH	/**< Specifies <I>Video Rich</I> content class*/
} EContentClass ;


/**
 * @brief Enumeration of presentation type
 *
 * Specifies the supported presentation types
 */
typedef enum _presentation_type
{
	E_PRESENTATION_NONE = 0x00,	/**< Specifies that presentation type is not set*/
	E_PRESENTATION_SMIL			/**< Specifies that presentation type is set as SMIL*/
} EPresentationType ;


/**
 * @brief Enumeration of media types
 *
 * Specifies the media categories based on content-types.
 */
typedef enum _media_type
{
	E_MEDIA_TEXT = 0x00,	/**< Specifies media type as Inline-Text(Default)*/
	E_MEDIA_IMAGE,			/**< Specifies that media object is an image*/
	E_MEDIA_AUDIO,			/**< Specifies that media object is an audio*/
	E_MEDIA_VIDEO,			/**< Specifies that media object is a video*/
	E_MEDIA_OTHERS			/**< Specifies that media object is something other than the above media types.*/
}EMediaType ;


/**
 * @brief Specifies value type of headers.
 */
typedef enum _ValueType
{
	E_INT_VALUE = 0x00,	/**< Specifies integer type value*/
	E_STRING_VALUE,		/**< Specifies string type value*/
	E_TSTRING_VALUE		/**< Specifies wide char type value*/
} EValueType;


/**
 * @brief Enumeration of alignments
 *
 * Specifies the alignment types for media rendering.
 */
typedef enum _Alignment
{
	E_LEFT_ALIGNMENT = 0x00,/**< Specifies left alignment*/
	E_CENTER_ALIGNMENT,		/**< Specifies center alignment*/
	E_RIGHT_ALIGNMENT		/**< Specifies right alignment*/
} EAlignment;


/**
 * @brief Enumeration of link types
 *
 * Specifies the supported embedded link types while rendering text content.
 */
typedef enum _link_type
{
	E_LINK_URL = 0,		/**< Specifies link to URL*/
	E_LINK_EMAIL_ID,	/**< Specifies link to Email ID*/
	E_LINK_PHONE_NUMBER	/**< Specifies link to Phone number*/
} E_LINK_TYPE ;


/**
 * @brief Enumeration of error codes
 *
 * Specifies the error codes passed to the ERROR_IND_CB callback function (applicable only for MMS).
 *
 * <br>
 * <table border="1">
 * <tr><th colspan="2" scope="colgroup">Error Type - Callback Data Mapping Table</th></tr>
 * <tr><td><b>error</b></td>						<td><b>pCBStruct</b></td></tr>
 * <tr><td>E_WARNING_CREATION_MODE_ADDMEDIA</td>	<td>MEDIA_INFO *</td></tr>
 * <tr><td>E_WARNING_CREATION_MODE_SETMODE</td>		<td>NONE (JC_NULL)</td></tr>
 * <tr><td>E_WARNING_CREATION_MODE_FORWARD</td>		<td>NONE (JC_NULL)</td></tr>
 * <tr><td>E_WARNING_DRM_FORWARD_RESTRICTED</td>	<td>NONE (JC_NULL)</td></tr>
 * <tr><td>E_MEDIA_ALREADY_EXISTS</td>				<td>MEDIA_INFO *</td></tr>
 * </table>
 * <br>
 *
 */
typedef enum _error_code
{
	 E_WARNING_CREATION_MODE_ADDMEDIA = 0x00,	/**< Specifies that error has occurred due to content class during jdi_MsgAddMedia/jdi_MsgUpdateInlineText*/
	 E_WARNING_CREATION_MODE_SETMODE,			/**< Specifies that error has occurred due to content class during jdi_MsgSetContentClass/jdi_MsgSetCreationMode*/
	 E_WARNING_CREATION_MODE_FORWARD,			/**< Specifies that error has occurred due to content class during jdi_MsgForward*/
	 E_WARNING_DRM_FORWARD_RESTRICTED,			/**< Specifies that media has been restricted due to DRM */
	 E_MEDIA_ALREADY_EXISTS						/**< Specifies the error that media already exists. This error occurs when media already exists and an attempt to add media is done*/

}ErrorCode ;


/**
 * @brief Enumeration of view event types
 *
 * Specifies different event types while rendering the message
 *
 * <br>
 * <table border="1">
 * <tr><th colspan="2" scope="colgroup">Event Type - Event Data Mapping Table</th></tr>
 * <tr><td><b>EViewEventType</b></td>	<td><b>Event Data</b></td></tr>
 * <tr><td>E_VIEW_START</td>			<td>EVENT_START_VIEW *</td></tr>
 * <tr><td>E_VIEW_PLAY</td>				<td>EVENT_PLAY *</td></tr>
 * <tr><td>E_VIEW_PAUSE</td>			<td>NONE</td></tr>
 * <tr><td>E_VIEW_RESUME</td>			<td>NONE</td></tr>
 * <tr><td>E_VIEW_STOP</td>				<td>EVENT_STOP *</td></tr>
 * <tr><td>E_VIEW_PLAY_NEXT_SLIDE</td>	<td>EVENT_NEXT_SLIDE *</td></tr>
 * <tr><td>E_VIEW_PLAY_PREV_SLIDE</td>	<td>EVENT_PREV_SLIDE *</td></tr>
 * <tr><td>E_VIEW_SET_ACTIVE_SLIDE</td>	<td>EVENT_SET_ACTIVE_SLIDE *</td></tr>
 * <tr><td>E_VIEW_SCROLL</td>			<td>EVENT_SCROLL *</td></tr>
 * <tr><td>E_VIEW_SET_FOCUS</td>		<td>EVENT_SET_FOCUS *</td></tr>
 * <tr><td>E_VIEW_REFRESH</td>			<td>NONE</td></tr>
 * <tr><td>E_VIEW_REPLAY</td>			<td>NONE</td></tr>
 * <tr><td>E_VIEW_TIMER</td>			<td>NONE</td></tr>
 * <tr><td>E_VIEW_CHANGE_VIEW_AREA</td>	<td>EVENT_CHANGE_VIEW_AREA *</td></tr>
 * <tr><td>E_VIEW_CLOSE</td>			<td>NONE</td></tr>
 * <tr><td>E_VIEW_END</td>				<td>NONE</td></tr>
 * </table>
 * <br>
 *
 */
typedef enum _view_event
{
	E_VIEW_START = 0x00,		/**< Specifies <I> Start </I> event */
	E_VIEW_PLAY,				/**< Specifies <I> Play </I> event*/
	E_VIEW_PAUSE,				/**< Specifies <I> Pause </I> event*/
	E_VIEW_RESUME,				/**< Specifies <I> Resume </I> event for paused presentation*/
	E_VIEW_STOP,				/**< Specifies <I> Stop </I> event for playing presentation*/
	E_VIEW_PLAY_NEXT_SLIDE,		/**< Specifies <I> Next Slide </I> event. This is applicable only when more than one slides/pages exist */
	E_VIEW_PLAY_PREV_SLIDE,		/**< Specifies <I> Previous Slide </I> event. This is applicable only when more than one slides/pages exist */
	E_VIEW_SET_ACTIVE_SLIDE,	/**< Specifies <I> setting Active </I> event for playing presentation*/
	E_VIEW_SCROLL,				/**< Specifies <I> Scroll </I> event*/
	E_VIEW_SET_FOCUS,			/**< Specifies <I> Set focus</I> event */
	E_VIEW_REFRESH,				/**< Specifies <I> Refresh display </I> event */
	E_VIEW_REPLAY,				/**< Specifies <I> Replay </I> event*/
	E_VIEW_TIMER,				/**< Specifies <I> Timer Triggered </I> event */
	E_VIEW_CHANGE_VIEW_AREA,	/**< Specifies <I> Change View Area</I> event to change the area of display*/
	E_VIEW_CLOSE,				/**< Specifies <I> Close </I> event for presentation*/
	E_VIEW_END					/**< Specifies <I> End </I> event */
} EViewEventType ;


/**
 * @brief Enumeration of direction of movement
 *
 * Specifies scroll direction movement
 */
typedef enum _direction
{
	E_DIRECTION_LEFT = 0x00,	/**< Specifies <I>left</I> direction of movement*/
	E_DIRECTION_RIGHT,			/**< Specifies <I>right</I> direction of movement*/
	E_DIRECTION_UP,				/**< Specifies <I>upward</I> direction of movement*/
	E_DIRECTION_DOWN			/**< Specifies <I>downward</I> direction of movement*/
} EDirection ;


/**
 * @brief Enumeration for view mode
 */
typedef enum _view_mode
{
	E_PORTRAIT_MODE = 0x00,	/**< Specifies portrait mode */
	E_LANDSCAPE_MODE		/**< Specifies landscape mode*/
} E_VIEW_MODE ;


/**
 * @brief Enumeration of focused objects
 *
 * Specifies the type of object in focus (rendering)
 */
typedef enum focused_object
{
	E_MSG_FOCUS_NONE = 0x00,	/**< Informs no media is focused.*/
	E_MSG_FOCUS_IMAGE,			/**< Informs image is focused*/
	E_MSG_FOCUS_TEXT,			/**< Informs text is focused*/
	E_MSG_FOCUS_AUDIO,			/**< Informs audio is focused*/
	E_MSG_FOCUS_VIDEO,			/**< Informs video is focused*/
	E_MSG_FOCUS_URL,			/**< Informs URL is focused*/
	E_MSG_FOCUS_EMAIL_ID,		/**< Informs Email-ID is focused*/
	E_MSG_FOCUS_PHONE_NUMBER	/**< Informs Phone number is focused*/
} FOCUSED_OBJECT ;


/**
 * @brief Enumeration of callback types
 *
 * Specifies different types of callbacks.
 *
 * <br>
 * <table border="1">
 * <tr><th colspan="2" scope="colgroup">Callback Type - Callback Function Mapping Table</th></tr>
 * <tr><td><b>EMsgCallbackTypes</b></td>			<td><b>Callback Function</b></td></tr>
 * <tr><td>E_CB_ERROR_IND</td>						<td>ERROR_IND_CB</td></tr>
 * <tr><td>E_CB_ATTACHMENT_MISC_IND</td>			<td>VIEW_ATTACHMENTS_MISC_IND_CB</td></tr>
 * <tr><td>E_CB_AUDIO_VIDEO_UNKNOWN_IND</td>		<td>VIEW_AUDIO_VIDEO_UNKNOWN_IND_CB</td></tr>
 * <tr><td>E_CB_FOCUS_CHANGED_IND</td>				<td>VIEW_FOCUS_CHANGE_CB</td></tr>
 * <tr><td>E_CB_PAGE_PRESENTATION_IND</td>			<td>VIEW_PAGE_PRESENTATION_IND_CB</td></tr>
 * <tr><td>E_CB_DRM_IND</td>						<td>DRM_IND_CB</td></tr>
 * <tr><td>E_MSG_DRM_UPDATE_RIGHTS</td>				<td>DRM_UPDATE_RIGHTS_CB</td></tr>
 * <tr><td>E_CB_DRM_CANCEL</td>						<td>DRM_CANCEL_CB</td></tr>
 * <tr><td>E_CB_DRM_COMPOSE</td>					<td>DRM_COMPOSE_CB</td></tr>
 * <tr><td>E_CB_DRM_INFO_LIST</td>					<td>VIEW_DRM_LIST_IND_CB</td></tr>
 * <tr><td>E_CB_REPLY_CHARGING</td>					<td>REPLY_CHARGING_IND_CB</td></tr>
 * <tr><td>E_CB_TIMER_LISTENER</td>					<td>MSG_TIMER_LISTENER_CB</td></tr>
 * </table>
 * <br>
 */
typedef enum _msg_callback_types
{
	E_CB_ERROR_IND = 0x00,			/**< Invoked when creation mode conflicts occur or duplicate mime content added while composing.*/
	E_CB_ATTACHMENT_MISC_IND,		/**< Invoked when attachment is present in the message*/
	E_CB_AUDIO_VIDEO_UNKNOWN_IND,	/**< Invoked when audio or video or unsupported content or corrupted content is present in the message.*/
	E_CB_FOCUS_CHANGED_IND,			/**< Invoked when focus changes (manual or automatic)*/
   	E_CB_PAGE_PRESENTATION_IND,		/**< Invoked when there is change in the state of Page of Presentation (Page start, Page End, Page Pause, Presentation End).*/
	E_CB_DRM_IND,					/**< Invoked to parse DRM content while viewing a message.*/
	E_CB_DRM_UPDATE_RIGHTS,			/**< Invoked to inform the application that the DRM object has been rendered or not so that the rights associated with that DRM content can be updated.*/
	E_CB_DRM_CANCEL,				/**< Invoked to inform the application to abandon parsing DRM object.*/
	E_CB_DRM_COMPOSE,				/**< Invoked to get DRM Encoded buffer for media from application*/
	E_CB_DRM_INFO_LIST,				/**< Invoked to inform list of DRM contents (if any) present in the message.*/
	E_CB_REPLY_CHARGING,			/**< Invoked to inform reply charging details on replying a MMS with reply charging accepted*/
	E_CB_TIMER_LISTENER				/**< Invoked on timer callback to decide on sync or async timer handling.*/
} EMsgCallbackTypes ;


/**
 * @brief Enumeration of page states
 */
typedef enum _page_state
{
	E_STATE_PAGE_START = 0x00,	/**< Specifies start of a page*/
	E_STATE_PAGE_END,			/**< Specifies end of a page*/
	E_STATE_PAGE_PAUSE ,		/**< Specifies that page has been paused*/
	E_STATE_PRESENTATION_END	/**< Specifies end of presentation*/

}E_PAGE_STATE;


/**
 * @brief Enumeration of content flags
 *
 * Specifies type of content passed in AUDIO_VIDEO_UNKNOWN_IND
 */
typedef enum _content_flag
{
	E_CONTENT_AUDIO = 0x00,	/**< Specifies audio content*/
	E_CONTENT_VIDEO,		/**< Specifies video content*/
	E_CONTENT_UNKNOWN,		/**< Specifies that the content object is unknown*/
	E_CONTENT_CORRUPTED		/**< Specifies that the content object is corrupted*/
}E_CONTENT_FLAG;


/**
 * @brief Enumeration of content flags
 *
 * Specifies different types of message PDU's.
 */
typedef enum _msg_type
{
	E_MSG_TYPE_SEND = 0x00,				/**< Specifies the message pdu as send request*/
	E_MSG_TYPE_SEND_RESPONSE,			/**< Specifies the message pdu as response to the send request*/
	E_MSG_TYPE_NOTIFICATION,			/**< Specifies that message pdu is indication of a new message (from the server)*/
	E_MSG_TYPE_NOTIFICATION_RESPONSE,	/**< Specifies that message pdu is a response to the new message indication (to the server)*/
	E_MSG_TYPE_RECEIVE,					/**< Specifies the message pdu as received message*/
	E_MSG_TYPE_ACKNOWLEDGE,				/**< Specifies the message pdu as acknowledgement to the server after retrieving message*/
	E_MSG_TYPE_DELIVERY_REPORT,			/**< Specifies the message pdu as delivery report from the server*/
	/* MMS 1.1 and above */
	E_MSG_TYPE_READ_REPORT,				/**< Specifies the message pdu as read report to the sender */
	E_MSG_TYPE_READ_REPORT_RESPONSE,	/**< Specifies the message pdu as read report from the recipient (received from the server)*/
	E_MSG_TYPE_FORWARD,					/**< Specifies the message pdu as a request to forward a message from the MBox (Only for MMS with MBox enabled)*/
	E_MSG_TYPE_FORWARD_RESPONSE,		/**< Specifies the message pdu as response to the MBox forward request (Only for MMS with MBox enabled)*/
	/* MMS 1.2 and above */
	E_MSG_TYPE_MMBOX_STORE,				/**< Specifies the message pdu as request to store the new message or update an existing message in the MBox (Only for MMS with MBox enabled)*/
	E_MSG_TYPE_MMBOX_STORE_RESPONSE,	/**< Specifies the message pdu as response to the MBox store or update request (Only for MMS with MBox enabled)*/
	E_MSG_TYPE_MMBOX_VIEW,				/**< Specifies the message pdu as request to retrieve messages) from the MBox (Only for MMS with MBox enabled)*/
	E_MSG_TYPE_MMBOX_VIEW_RESPONSE,		/**< Specifies the message pdu as response to MBox view request (Only for MMS with MBox enabled)*/
	E_MSG_TYPE_MMBOX_UPLOAD,			/**< Specifies the message pdu as request to upload a composed message to the MBox (Only for MMS with MBox enabled)*/
	E_MSG_TYPE_MMBOX_UPLOAD_RESPONSE,	/**< Specifies the message pdu as response to the MBox upload request (Only for MMS with MBox enabled)*/
	E_MSG_TYPE_MMBOX_DELETE,			/**< Specifies the message pdu as request to delete messages) from the MBox (Only for MMS with MBox enabled)*/
	E_MSG_TYPE_MMBOX_DELETE_RESPONSE	/**< Specifies the message pdu as response to the MBox message delete request (Only for MMS with MBox enabled)*/
}EMsgType ;


/**
 * @brief Enumeration of supported message class
 *
 * Specifies types of message classes.
 */
typedef enum _message_class
{
	E_MSG_PERSONAL_CLASS = 0x00,/**< Specifies the message class as <I>Personal</I>*/
	E_MSG_ADVERTISEMENT_CLASS,	/**< Specifies the message class as <I>Advertisement</I>*/
	E_MSG_INFORMATION_CLASS,	/**< Specifies the message class as <I>Informational</I>*/
	E_MSG_AUTO_CLASS			/**< Specifies the message class as <I>Automatic</I>*/
} EMessageClass ;


/**
 * @brief Enumeration of message priority
 *
 * Specifies the types of message classes.
 */
typedef enum _message_priority
{
	E_MSG_PRIORITY_LOW = 0x01,	/**< Specifies as low priority message*/
	E_MSG_PRIORITY_NORMAL,		/**< Specifies as normal priority message*/
	E_MSG_PRIORITY_HIGH			/**< Specifies as high priority message*/
} EMessagePriority;


/**
 * @brief Enumeration of message status
 *
 * @note
 *    - This is applicable only to MMS.
 *    - It is used with notification - E_MSG_TYPE_NOTIFICATION and
 *      with delivery report E_MSG_TYPE_DELIVERY_REPORT
 */
typedef enum _message_status
{
	E_MSG_EXPIRED = 0x00,	/**< Specifies that the recipient did not retrieve the message before expiration.*/
	E_MSG_RETRIEVED,		/**< Specifies that the message has been successfully retrieved by the recipient*/
	E_MSG_REJECTED,			/**< Specifies that the message has been rejected by the recipient*/
	E_MSG_DEFERRED,			/**< Specifies that the message client decided to retrieve the message later */
	E_MSG_UNRECOGNIZED,		/**< Specifies that message <B>PDU</B> is unknown*/
	E_MSG_INDETERMINATE,	/**< Specifies that it's not able to determine whether the message reached the recipient.*/
	E_MSG_FORWARDED,		/**< Specifies that the recipient without viewing has forwarded the message. Specific to MBox.*/
	E_MSG_UNREACHABLE		/**< Specifies that the recipient was not able to reach due to network or other failures*/
}EMessageStatus ;


/**
 * @brief Enumeration of available MMBox status
 *
 * Specifies the types of MMBox folders.
 *
 * @note
 *    - This is applicable only to MMS with MMBox enabled
 */
typedef enum _mbox_states
{
	E_MBOX_STATE_DRAFT = 0x00,	/**< Specifies MBox <I>Draft</I> folder*/
	E_MBOX_STATE_SENT,			/**< Specifies MBox <I>Sent</I> folder*/
	E_MBOX_STATE_NEW,			/**< Specifies MBox <I>New</I> folder*/
	E_MBOX_STATE_RETRIEVED,		/**< Specifies MBox <I>Retrieved</I> folder*/
	E_MBOX_STATE_FORWARDED		/**< Specifies MBox <I>Forwarded</I> folder*/
} EMBoxState ;


/**
 * @brief Enumeration of available MMBox attributes
 *
 * @note
 *    This applies only to MMS. It is used to specify the list of information
 *    elements that should appear in MMBox view request
 */
typedef enum _mbox_attrib_type_
{
	E_MBOX_BCC_ATTRIB = 0x01,			/**< Specifies <I>BCC</I> attribute*/
	E_MBOX_CC_ATTRIB,					/**< Specifies <I>CC</I> attribute*/
	E_MBOX_CONTENT_LOCATION_ATTRIB,		/**< Specifies <I>Content Location</I> attribute*/
	E_MBOX_CONTENT_TYPE_ATTRIB,			/**< Specifies <I>Content Type</I> attribute*/
	E_MBOX_DATE_ATTRIB,					/**< Specifies <I>Date</I> attribute*/
	E_MBOX_DELIVERYREPORT_ATTRIB,		/**< Specifies <I>Delivery Report</I> attribute*/
	E_MBOX_DELIVERYTIME_ATTRIB,			/**< Specifies <I>Delivery Time</I> attribute*/
	E_MBOX_EXPIRY_ATTRIB,				/**< Specifies <I>Expiry</I> attribute*/
	E_MBOX_FROM_ATTRIB,					/**< Specifies <I>From</I> attribute*/
	E_MBOX_MESSAGECLASS_ATTRIB,			/**< Specifies <I>Message Class</I> attribute*/
	E_MBOX_MESSAGEID_ATTRIB = 0x0C,		/**< Specifies <I>Message Id</I> attribute*/
	E_MBOX_MESSAGESIZE_ATTRIB=0x0E,		/**< Specifies <I>Message Size</I> attribute*/
	E_MBOX_PRIORITY_ATTRIB,				/**< Specifies <I>Priority</I> attribute*/
	E_MBOX_READREPORT_ATTRIB=0x10,		/**< Specifies <I>Read Report</I> attribute*/
	E_MBOX_SUBJECT_ATTRIB=0x16,			/**< Specifies <I>Subject</I> attribute*/
	E_MBOX_TO_ATTRIB,					/**< Specifies <I>To</I> attribute*/
	E_MBOX_REPLYCHARGING_ATTRIB=0x1c,	/**< Specifies <I>Reply Charging</I> attribute*/
	E_MBOX_REPLYCHARGINDEADLINE_ATTRIB,	/**< Specifies <I>Reply Charging Deadline</I> attribute*/
	E_MBOX_REPLYCHARGINGID_ATTRIB=0x1e,	/**< Specifies <I>Reply Charging Id</I> attribute*/
	E_MBOX_REPLYCHARGINGSIZE_ATTRIB,	/**< Specifies <I>Reply Charging Size</I> attribute*/
	E_MBOX_PREVIOUSLYSENTBY_ATTRIB,		/**< Specifies <I>Previously Sent By</I> attribute*/
	E_MBOX_PREVIOUSLYSENTDATE_ATTRIB,	/**< Specifies <I>Previously Sent Date</I> attribute*/
	E_MBOX_CONTENT = 0x2E				/**< Specifies <I>Content</I> attribute*/
} EMBoxAttributeType ;


/**
 * @brief Enumeration of reply charging types
 *
 * @note
 *     - This is applicable only to MMS(MM Version 1.1 and above).
 */
typedef enum _reply_charging_types
{
	E_REQUEST_REPLY_CHARGING = 0x00,	/**< This value should be set only for E_MSG_TYPE_SEND. Specifies that the sender is willing to pay for the Reply-MM of the recipients)*/
	E_REQUEST_TEXT_ONLY_REPLY_CHARGING,	/**< This value should be set only for E_MSG_TYPE_SEND. Specifies that the sender is willing to pay for the Reply-MM of the recipients) provided that the MM contains only text media.*/
	E_ACCEPTED_REPLY_CHARGING,			/**< This value should be present only in E_MSG_TYPE_NOTIFICATION or E_MSG_TYPE_RECEIVE. Specifies that the sender is willing to pay for the Reply-MM*/
	E_ACCEPTED_TEXT_ONLY_REPLY_CHARGING,/**< This value should be present only in E_MSG_TYPE_NOTIFICATION or E_MSG_TYPE_RECEIVE. Specifies that the sender is willing to pay for the Reply-MM provided that the MM has only text media.*/
	E_RESET_REPLY_CHARGING				/**< To be used for unsetting replycharging. This value should be set if this message is a reply to a message with reply charging*/
} EReplyCharging ;

/**
 * @brief Enumeration of read report PDU status
 */
typedef enum _message_read_status
{
	E_STATUS_READ = 0x00,	/**< Specifies that the message has been read by the recipient*/
	E_STATUS_DELETED ,		/**< Specifies that the recipient had deleted the message without reading*/
	E_STATUS_DISPATCHED,	/**< Species that the message has been dispatched (only for EMAIL)*/
	E_STATUS_PROCESSED,		/**< Specifies that message has been processed (only for EMAIL)*/
	E_STATUS_DENIED,		/**< check (only for EMAIL)*/
	E_STATUS_FAILED			/**< check (only for EMAIL)*/

} EMessageReadStatus ;


/**
 * @brief Enumeration of read receipt settings
 *
 * This is applicable only to Email type messages
 */
typedef enum _message_disposition_action_mode
{
	E_MANUAL_ACTION =0x00,		/**< Specifies that every time a message is read, a manual selection should be made to send read receipt*/
	E_AUTOMATIC_ACTION			/**< Specifies that a read receipt is automatically sent when any message is read*/
}EMessageDispositionActionMode ; 


/**
 * @brief Enumeration of reply type
 *
 * Specifies the type of reply to be used when calling jdi_MsgReply()
 */
typedef enum _reply_type
{
	E_TYPE_REPLY = 0x00,	/**< Specifies normal reply*/
	E_TYPE_REPLYALL			/**< Specifies reply to all*/
} EReplyType ;


/***************** STORE MODULE ENUMS ****************/

/**
 * @brief Enumeration of different sorting fields to be used while sorting
 */
typedef enum _sort_by
{
	E_SORT_BY_TO = 0x00,		/**< Specifies the sort field as <I>To</I> Address*/
	E_SORT_BY_FROM,				/**< Specifies the sort field as <I>From</I> Address*/
	E_SORT_BY_SUBJECT,			/**< Specifies the sort field as <I>Subject</I> of the message*/
	E_SORT_BY_RECVTIME,			/**< Specifies the sort field as <I>Receive time</I> of the message.*/
	E_SORT_BY_MESSAGE_CLASS,	/**< Specifies the sort field as <I>Message Class</I>.*/
	E_SORT_BY_MESSAGE_SIZE,		/**< Specifies the sort field as <I>Message Size</I>.*/
	E_SORT_BY_EXPIRY,			/**< Specifies the sort field as <I>Expiry Time</I>.*/
	E_SORT_BY_STATUS			/**< Specifies the sort field as <I>Read/Unread Status</I>.*/
} ESortBy;


/**
 * @brief Enumeration of sorting order
 */
typedef enum _sort_order
{
	E_SORT_ORDER_ASCENDING = 0x00,	/**< Specifies the sorting order as <I>ascending</I>*/
	E_SORT_ORDER_DESCENDING			/**< Specifies the sorting order is <I>descending</I>*/
} ESortOrder;


/**
 * @brief Enumeration of update types
 *
 * Specifies the operation to be performed while committing the changes (jdi_StoreUpdateMsgList)
 */
typedef enum _update_flag
{
	E_STORE_NONE = 0x00,	/**< Specifies that no change has been done*/
	E_STORE_DELETE,			/**< Specifies that msg be flagged for deleting*/
	E_STORE_MOVE,			/**< Specifies that msg be flagged for moving*/
	E_STORE_MODIFY			/**< Specifies that msg header has been modified and needs to be updated*/
} EUpdateFlag;

/**
 * @brief Specifies the enum for filter 
 *
 * Specifies the filtering to be used while getting the message list
 */
typedef enum _retrieve_filter
{
	E_RETRIEVE_BY_INDEX = 0x01	/**< Specifies filter by index */

} ERetrieveFilter;


/***************** SMIL MODULE ENUMS ****************/

/**
 * @brief Enumeration of SMIL Region Fit Attributes
 *
 * This specifies how media elements are cropped or scaled if they are not of the same dimension as the region
 *
 * Note: Currently FIT attributes are not supported while rendering.
 */
typedef enum _region_fit
{
	E_REGION_NONE = 0x00,	/**< Specifies that SMIL fit attribute is not set*/
	E_REGION_HIDDEN,		/**< Specifies that media larger than region will be cropped and media smaller will leave area of region empty*/
	E_REGION_FILL,			/**< Specifies that media will be scaled to the size of region, can distort aspect ratio*/
	E_REGION_MEET,			/**< Specifies that media will be scaled while preserving aspect ratio of image*/
	E_REGION_SCROLL,		/**< Specifies that media will be rendered as normal size, adding horizontal or vertical scroll bars if the clip extends beyond the region's boundaries*/
	E_REGION_SLICE			/**< Specifies that media will be scaled to fill region while preserving aspect ratio, part cropped off*/
} E_SMIL_REGION_FIT ;

/**************** DRM ***************/

/**
 * @brief	Specifies the DRM Error Codes.
 */
typedef enum _eDRMErrorCode
{
	E_DRM_ERR_RIGHT_DURATION_EXPIRED = 0x01,	/**< Specifies that rights duration has expired*/
	E_DRM_ERR_RIGHT_COUNT_EXCEEDED,				/**< Specifies that number of times the media object has been rendered, has exceed permissible number*/
	E_DRM_ERR_START_TIME_NOT_REACHED,			/**< Specifies that start time has not reached*/
	E_DRM_ERR_END_TIME_EXPIRED					/**< Specifies that end time has expired*/

}E_DRM_ERRORCODE;

/**
 * @brief	Specifies the DRM Types.
 */
typedef enum _eDRMType
{
	E_DRM_NONE = 0,			/**< Specifies as no DRM Protection*/
	E_DRM_FWD_LOCK,			/**< Specifies as DRM Forward Locked type*/
	E_DRM_COMBINED_DELIVERY,/**< Specifies as DRM Combined Delivery*/
	E_DRM_SEPARATE_DELIVERY /**< Specifies as DRM Separate Delivery*/

}E_DRM_TYPE;



/***************************************************************************
*						STRUCTURE DECLARATIONS
***************************************************************************/

/**
 * @brief Contains the timer information for media or page in milliseconds(for SMIL)
 */
typedef struct media_timer
{
	JC_UINT32		uiBegin ;		/**< Specifies the start time (in milliseconds)*/
	JC_UINT32		uiDuration ;	/**< Specifies the play duration (in milliseconds)*/
} MEDIA_TIMER ;


/**
 * @brief Contains page or slide properties
 */
typedef struct page_info
{
	JC_UINT32		uiDuration ;	/**< Specifies the duration. Valid only for SMIL presentation(in milliseconds)*/
	JC_UINT8		ucNumMedia ;	/**< Specifies the media count for the page*/
	JC_UINT32		uiPageSize ;	/**< Specifies the size of the page (in bytes)*/
} PAGE_INFO ;


/**
 * @brief Defines value for headers.
 */
typedef  union _value
{
	JC_INT32				IntVal ;	/**< Specifies integer value*/
	JC_INT8					*pStrVal ;	/**< Specifies string value*/
	JC_CHAR					*pTcharVal ;/**< Specifies widechar value*/
}VALUE;


/**
 * @brief Specifies the attributes information.
 */
typedef struct misc_headers
{
	JC_INT8					*pAttrName ;	/**< Specifies the Header name*/
	EValueType				eAttrValue;		/**< Specifies the Header value type */
	VALUE					AttrValue;		/**< Specifies the Header value*/
	struct misc_headers		*pNext ;		/**< Pointer to the next Header*/
} MISC_HEADERS ;

/**
 * @brief Defines inline text properties/styles
 */
typedef struct text_attributes
{
	EAlignment		eAlignment;			/**< Specifies the text alignment*/		
	JC_BOOLEAN		bIsBold ;			/**< Specifies normal or bold text*/
	JC_BOOLEAN		bIsItalic ;			/**< Specifies normal or italic text*/
	JC_BOOLEAN		bIsUnderLine ;		/**< Specifies text underline property*/
	JC_BOOLEAN		bIsStrikeThrough ;	/**< Specifies text strikethough property*/
	JC_UINT32		uiFontSize ;		/**< Specifies the font size*/ 
	JC_INT8			*pFontColor ;		/**< Specifies the font color*/
	JC_INT8			*psFontName ;		/**< Specifies the font name*/
	
}TEXT_ATTRIBUTES ;

/**
 * @brief Callback function prototype for Error indication. 
 *
 * @param hMsg[in]			Points to Message Handle
 * @param pCallbackArg [in]	Points to callback argument passed while registering callback.
 * @param pCBStruct[in]		Void pointer to callback data. Refer ErrorCode for Error Type - Callback Data mapping table.
 * @param error [in]		Specifies the error type for which callback is invoked.
 *
 * This function shall be implemented by application and registered with core. Core will invoke 
 * this function to indicate the application about the error conditon and gets back the option to follow.
 *
 * Following are the conditions on which core will invoke this function :
 *	- On Creation Mode non-conformance while adding/updating media: To decide whether to proceed add/update media
 *	- On setting new content class : To decide whether to proceed in setting if current message has non-conformant media
 *	- On forwarding message with DRM : To decide whether to remove DRM objects and continue forward or abort forwarding.
 *	- On adding media, if similar object already exists (only if SMIL presentation) : To decide whether to reuse existing media or add new.
 *
 * Note: 
 *	- Caller should not free the resource passed in pCallbackArg and pCBStruct. 
 *
 */
typedef JC_RETCODE	(* ERROR_IND_CB) 
								(MSG_HANDLE			hMsg, 
								 void				*pCallbackArg, 
								 void				*pCBStruct, 
								 ErrorCode			error) ;


/**
 * @brief Specifies the MIME header details
 */
typedef struct _media_mime_headers
{
	JC_INT8				*pContentType ;			/**< Specifies the content-type of the mime object*/
	JC_INT8				*pContentTypeParam ;	/**< Specifies content-type parameters for mime object. (Note: For internal use)*/
	JC_CHAR				*pObjectName ;			/**< Specifies the name of the mime object*/
	JC_INT8				*pContentID ;			/**< Specifies the Content-id of the mime object. Not to be filled by caller (Core will generate this internally while composing)*/
	JC_BOOLEAN			bIsAttachment;			/**< Specifies the content disposition (inline or attachment)*/
	MISC_HEADERS		*pOtherHeaders ;		/**< Points to other mime headers. NOT USED*/
}MEDIA_MIME_HEADERS;


/**
 * @brief Specifies information related to DRM object.
 */
typedef struct DRM_Info
{
	JC_INT8				*pRefId ;		/**< Specifies the reference Id*/
	JC_BOOLEAN			bIsRender;		/**< Specifies whether to render/play the media or not*/
	E_DRM_TYPE			eDRMType ;		/**< Specifies the DRM Type*/
	E_DRM_ERRORCODE		eErrorCode ;	/**< Specifies the error code if the media cannot be rendered.*/
} DRM_INFO ;


/**
 * @brief Specifies presentation details
 */
typedef struct _media_presentation_info
{
	JC_CHAR				*pRegionName ;		/**< Specifies the SMIL region name for the object*/
	MEDIA_TIMER			*pMediaTimer ;		/**< Specifies the media timer for the object*/
	JC_CHAR				*pszAlt ;			/**< Specifies the alternative name for the object*/
	MISC_HEADERS		*pOtherAttributes ;	/**< Specifies other presentation attributes*/
	TEXT_ATTRIBUTES		*pTextAttributes ;	/**< Specifies the text properties*/
	JC_UINT8			ucPredefinedNumber;	/**< Specifies Predefined Number of Object (For EMS)*/
	JC_UINT32			uiHeight;			/**< Specifies the height of object (For EMS)*/
	JC_UINT32			uiWidth;			/**< Specifies the width of object (For EMS)*/
}MEDIA_PRESENTATION_INFO;


/**
 * @brief Defines mime object headers and data
 */
typedef struct _media_mime_info
{
	MEDIA_MIME_HEADERS 	MimeHeaders;	/**< Contains Mime Headers of the Mime Object*/
	CONTENT_DATA		ContentData;	/**< Contains Mime Content of the Mime Object*/
	DRM_INFO			*pDRMInfo;		/**< Contains DRM Information for the Mime Object*/
} MEDIA_MIME_INFO;

/**
 * @brief Defines complete media object information
 */
typedef struct media_info
{
	MEDIA_MIME_INFO		*pMimeInfo;				/**< Specifies mime headers and data part*/
	MEDIA_PRESENTATION_INFO	PresentationInfo;	/**< Specifies the presentation part*/
	EMediaType			eMediaType ;			/**< Specifies the media type*/
	JC_UINT32			uiMediaID ;				/**< Unique reference id for the object, generated by the core on adding*/
	struct media_info	*pAlternate ;			/**< Points to the alternate object. (Presently used for DRM Objects)*/
	struct media_info	*pNext ;				/**< Points to the next object in the list*/
}MEDIA_INFO ;

/*************** VIEW MODULE STRUCTURES & CALLBACKS ****************/

/**
 * @brief Defines event data specific to E_VIEW_PLAY_NEXT_SLIDE event
 */
typedef struct event_next_slide
{
	JC_BOOLEAN		bIsTimerInvoked ;	/**< Always pass E_FALSE*/
} EVENT_NEXT_SLIDE ;


/**
 * @brief Defines event data specific to E_VIEW_PLAY_PREV_SLIDE event
 *
 * This event renders the previous slide
 */
typedef struct event_prev_slide
{
	JC_BOOLEAN		bIsTimerInvoked ;	/**< Always pass E_FALSE*/
} EVENT_PREV_SLIDE ;

/** 
 * @brief Defines event data specific to E_VIEW_SET_ACTIVE_SLIDE event
 *
 * This event sets the specified page/slide as active.
 */
typedef struct event_set_active_slide
{
	JC_UINT8		ucPageIndex;		/**< Specifies the index of page/slide to be displayed*/
	JC_BOOLEAN		bPlayPresentation;	/**< Specifies whether to play the presentation from that page or not*/
} EVENT_SET_ACTIVE_SLIDE ;


/**
 * @brief Defines event data specific to E_VIEW_PLAY event
 */
typedef struct event_play
{
	JC_BOOLEAN		bPlayAudioVideo;	/**< Specifies whether to play audio/video media object or to play entire presentation from that page*/
} EVENT_PLAY ;


/**
 * @brief Defines event data specific to E_VIEW_STOP event
 */
typedef struct event_stop
{
	JC_BOOLEAN		bStopAudioVideo;	/**< Specifies whether to stop the playing audio/video media or to stop entire presentation.*/
} EVENT_STOP;


/** 
 * @brief Defines event data specific to E_VIEW_SCROLL event
 *
 * This Event either scrolls the page contents in the specified direction or changes the focus (if enabled) to a different link in the specified direction if present
 */
typedef struct event_scroll
{
	EDirection			eDirection ;	/**< Informs the direction of scrolling (left, right, up, down). It can take any of above values.*/
	JC_BOOLEAN			bScrollPage;	/**< Informs whether Page up or Page down event if the Scrolling direction is up or down*/
	JC_BOOLEAN			bScrollMedia;	/**< Informs whether to scroll Page content or Media Content. Presently not used.*/ 
	JC_UINT32			uiScrollOffset;	/**< Scrolling Offset (in Pixels)*/
	JC_UINT32			uiScrollCtrlID ;/**< Specifies Scrollbar Control ID. Used for Stylus Scrolling. Core will use this ID to identify Vertical or Horizontal scrollbar */
	JC_BOOLEAN			bNewPosition ;	/**< Indicates whether uiScrollOffset specifies the new position of Scrollbar or the offset from previous position. This shall be used in case of application not able to provide previous position and offset */
	JC_BOOLEAN			bStylusEvent ;	/**< Specifies whether scrolling is done using Stylus (E_TRUE) or Key (E_FALSE). This will indicate that Scroll bar already navigated and core needs to update the controls (scroll) alone */
} EVENT_SCROLL ;


/**
 * @brief This Event changes the dimensions of rendering rectangle (SIP Invoke) or changes the origin and dimensions of the rendering rectangle (Portrait <-> Landscape)
 */
typedef struct event_change_view_area
{
	JC_RECT 			Rect;		/**< New Dimensions and origin of the Rendering Rectangle*/
	E_VIEW_MODE			eViewMode;	/**< Specifies new mode (portrait or landscape). Presently not used.*/
} EVENT_CHANGE_VIEW_AREA ;


/**
 * @brief Defines event data specific to E_VIEW_SET_FOCUS event
 *
 * This Event changes the focus to the Link or Media if any containing the Focus point
 */
typedef struct event_set_focus
{
	JC_POINT		FocusPoint ;	/**< Point in Focusable Media or Link*/
} EVENT_SET_FOCUS ;


/**
 * @brief Defines information to start display
 */
typedef struct event_start_view
{
	JC_RECT			Rect;			/**< Rectangle in which the Message should be rendered (Mandatory).If Invalid Rect is passed, Error is thrown by the View Module*/
	JC_UINT32		uiWindowID;		/**< ID of the Window in which message should be rendered. (Mandatory)*/
	JC_UINT8		ucPageIndex;	/**< This parameter specifies the page Index from which message should be rendered.(Only for Preview)For View it can be always 1*/
	E_VIEW_MODE 	eViewMode;		/**< Specifies the mode (Landscape or portrait) in which the message should be rendered. (Presently Portrait mode is alone supported)*/
	JC_BOOLEAN		bViewPageOnly;	/**< Specifies whether to render only a page or all pages.(Only for Preview)*/
	JC_BOOLEAN		bStaticView ;	/**< E_TRUE : To display the content without presentation (Manual Navigation). E_FALSE : To display with presentation (auto navigation)*/
} EVENT_START_VIEW ;


/**
 * @brief Defines information to end presentation
 */ 
typedef struct event_end_view
{
	JC_BOOLEAN		bModeChange ;	/**< This parameter is meant for internal use (Core Module, used during Mode Change). Always pass E_FALSE.*/
} EVENT_END_VIEW ;


/**
 * @brief Defines link information
 */
typedef struct link_info
{
	E_LINK_TYPE			eLinkType ;	/**< Specifies the link type. It can be either Phone number, URL or Email Id*/
	JC_CHAR				*pLinkText;	/**< Contains text of focused link, if any*/
	struct link_info	*pNext ;	/**< Points to next link*/
} LINK_INFO ;


/**
 * @brief Defines list of DRM media
 *
 * This information will be passed to VIEW_DRM_LIST_IND_CB callback
 */
typedef struct drm_media_List
{
	JC_UINT32				uiMediaID ;		/**< Specifies DRM Media Id*/
	JC_UINT8				ucPageIndex ;	/**< Specifies page index in which DRM media exists*/
	JC_BOOLEAN				bIsAttachment;	/**< Specifies whether media is an attachment or not*/
	struct drm_media_List	*pNext ;		/**< Specifies next DMR media list */
} DRM_MEDIA_LIST ;

/**
 * @brief Defines active/focused media information
 */
typedef struct active_media_info
{
	MEDIA_INFO			*pMediaInfo ;	/**< Specifies details of  Focused Media*/
	LINK_INFO			LinkInfo ;		/**< Specifies Details of Focused Link (if any) if Text is Focused*/
} ACTIVE_MEDIA_INFO ;

/**
 * @brief Defines information about unknown audio/video media
 */
typedef struct audio_video_unknown_ind
{
	E_CONTENT_FLAG					eContent ;		/**< Specifies whether Audio or Video or Corrupted or Unknown*/
	JC_UINT32						uiMediaID ;		/**< Specifies Media ID of  respective content*/
	JC_UINT32						uiControlID ;	/**< Specifies Control ID (Presently used only for Video if first frame is displayed as image)*/
	struct audio_video_unknown_ind	*pNext;			/**< Specifies Pointer to Next Node if any.*/

} AUDIO_VIDEO_UNKNOWN_IND ;

/**
 * @brief Defines information about state of page or presentation
 */
typedef struct page_presentation_ind
{
	E_PAGE_STATE		eState ;	/**< Specifies State of Page and Presentation*/
	JC_UINT32			uiPageDur;	/**< Specifies presentation time for the active page*/
} PAGE_PRESENTATION_IND ;


/**
 * @brief Contains information to indicate presence of attachment
 */
typedef struct attachment_misc_ind
{
	JC_BOOLEAN			bIsAttachment ;	/**< <B>E_TRUE</B> Specifies that attachment is present*/
} ATTACHMENT_MISC_IND ;


/**
 * @brief Defines reply charging limit information
 */
typedef struct reply_charge_limits
{
	JC_UINT32		uiRCTime ;		/**< Specifies the reply charging time limit set.Absolute time from 1 - 1 - 1970*/
	JC_UINT32		uiRCSize ;		/**< Specifies the reply message size limit*/
	EReplyCharging	eRCType ;		/**< Specifies the type of reply charging*/
	JC_INT8			*pReplyMsgID ;	/**< Specifies the message id of the message being replied to. Application (caller) should set this message id to the reply charging of the information of the replied message.*/
} REPLY_CHARGE_LIMITS ;


/**
 * @brief Callback function prototype to indicate presence of attachments 
 *
 * @param hMsg[in]					Points to Message Handle
 * @param pCallbackArg [in]			Points to callback argument passed while registering callback.
 * @param pAttachmentMiscInfo [in]	Points to attachment information.
 *
 * This function shall be implemented by application and registered with core before viewing a message (jdi_MsgView()). 
 * Core will invoke this function to indicate the presence of attachments (before rendering). Application
 * shall use this information to indicate the present of attachments to user (eg: Icons)
 *
 * Note: 
 *	Caller should not free the resource passed in pAttachmentMiscInfo & pCallbackArg.
 *
 */
typedef JC_RETCODE	(* VIEW_ATTACHMENTS_MISC_IND_CB)
													(MSG_HANDLE				hMsg,
													 void					*pCallbackArg, 
													 ATTACHMENT_MISC_IND	*pAttachmentMiscInfo) ;


/**
 * @brief Callback function prototype to indicate audio or video or unknown contents
 *
 * @param hMsg[in]						Points to Message Handle
 * @param pCallbackArg [in]				Points to callback argument passed while registering callback.
 * @param pAudioVideoUnknownInfo [in]	Points to Audio, video or unknown content information.
 *
 * This function shall be implemented by application and registered with core before viewing a message (jdi_MsgView()). 
 * Core will invoke this function to indicate the presence of audio/video/unknown contents (For each slide). Application
 * shall use this information to indicate the present of audio/video/unknown contents to user (eg: Icons)
 *
 * Note: 
 *	Caller should not free the resource passed in pAudioVideoUnknownInfo & pCallbackArg.
 *
 */
typedef JC_RETCODE	(* VIEW_AUDIO_VIDEO_UNKNOWN_IND_CB) 
													(MSG_HANDLE					hMsg,
													 void						*pCallbackArg, 
													 AUDIO_VIDEO_UNKNOWN_IND	*pAudioVideoUnknownInfo) ;


/**
 * @brief Callback function prototype to indicate media focus change
 *
 * @param hMsg[in]				Points to Message Handle
 * @param pCallbackArg [in]		Points to callback argument passed while registering callback.
 * @param eFocussedObject [in]	Specifies the focused object type.
 *
 * This function shall be implemented by application and registered with core before viewing a message (jdi_MsgView()). 
 * Core will invoke this function to indicate object focus change. Application shall use this information to provide 
 * custom context menu based on focused object type.
 *
 * Note: 
 *	Caller should not free the resource passed in pCallbackArg.
 *
 */
typedef JC_RETCODE	(* VIEW_FOCUS_CHANGE_CB) 
										(MSG_HANDLE			hMsg,
										 void				*pCallbackArg, 
										 FOCUSED_OBJECT		eFocussedObject) ;


/**
 * @brief Callback function prototype to indicate Presentation states (Presentation or Slide Start, End, Pause)
 *
 * @param hMsg[in]						Points to Message Handle
 * @param pCallbackArg [in]				Points to callback argument passed while registering callback.
 * @param pPagePresentationInfo [in]	Points to presentation state information.
 *
 * This function shall be implemented by application and registered with core before viewing a message (jdi_MsgView()). 
 * Core will invoke this function to indicate presentation states 
 *	- Page Start
 *	- Page Time/Duration
 *	- Page Pause
 *	- Page End
 *	- Presentation End. 
 * Application shall use this information to provide custom MMI to indicate the Presentation states to user.
 *
 * Note: 
 *	Caller should not free the resource passed in pPagePresentationInfo & pCallbackArg.
 *
 */
typedef JC_RETCODE	(* VIEW_PAGE_PRESENTATION_IND_CB) 
													(MSG_HANDLE				hMsg,
													 void					*pCallbackArg, 
													 PAGE_PRESENTATION_IND	*pPagePresentationInfo) ;


/**
 * @brief Callback function prototype to indicate DRM Object List
 *
 * @param hMsg[in]				Points to Message Handle
 * @param pCallbackArg [in]		Points to callback argument passed while registering callback.
 * @param pDRMInfoList [in]		Points to DRM Object Information List.
 *
 * This function shall be implemented by application and registered with core before viewing a message (jdi_MsgView()). 
 * Core will invoke this function to indicate the list of DRM objects present in the message. Application shall use this 
 * information to indicate the user about presence of DRM contents (eg: Icons).
 *
 * Note: 
 *	Caller should not free the resource passed in pDRMInfoList (shall copy the information) & pCallbackArg.
 *
 */
typedef JC_RETCODE	(* VIEW_DRM_LIST_IND_CB) 
											(MSG_HANDLE			hMsg,
											 void				*pCallbackArg, 
											 DRM_MEDIA_LIST		*pDRMInfoList) ;

/**
 * @brief Callback function prototype to indicate the presence of replycharging limit (Only for MMS, verisons > 1.0).
 *
 * @param hMsg[in]				Points to Message Handle
 * @param pCallbackArg [in]		Points to callback argument passed while registering callback.
 * @param pRCLimit [in]			Points to Reply Charging information.
 *
 * This function shall be implemented by application and registered with core before replying a message (jdi_MsgReply()). 
 * On message reply, core will invoke this function to indicate the Reply Charging Information (if present). 
 * Application shall use this information to set the reply charging detail in the message header.
 *
 * @note 
 *	Caller should not free the resource passed in pRCLimit (shall copy the information) & pCallbackArg.
 *
 */
typedef JC_RETCODE	(* REPLY_CHARGING_IND_CB) 
											(MSG_HANDLE				hMsg, 
											 void					*pCallbackArg, 
											 REPLY_CHARGE_LIMITS	*pRCLimit) ;

/**
 * @brief Specifies the prototype for timer listener callback (only for MMS with SMIL)
 *
 * @param hMsg[in]				Points to Message Handle
 * @param pCallbackArg [in]		Points to callback argument passed while registering callback.
 * @param pvTimerInfo [in]		Not used, reserved for future usage.
 *
 * This function shall be implemented by application and registered with core before viewing a message (jdi_MsgView()). 
 * If registered, core will invoke this callback on timer callback (trigger) to check whether to handle processing in timer callback (synchronous) or asynchronously.
 * Application shall :
 *	- Return JC_OK to indicate core to handle processing in timer callback.
 *	- Return JC_ERR_ASYNC_CALL to indicate the core to handle asynchronously i.e., wait for application to indicate core to process. 
 *	  Application shall indicate the core to process by calling jdi_MsgViewHandleEvent with eEventType E_VIEW_TIMER
 *
 * @note 
 *	- If this callback is not registered, core will follow synchrous timer callback handling.
 *
 */
typedef JC_RETCODE	(* MSG_TIMER_LISTENER_CB) 
											(MSG_HANDLE			hMsg, 
											 void				*pCallbackArg, 
											 void				*pvTimerInfo) ;


/**
 * @brief Defines the MBox Flags types for setting tokens.
 *
 * This is applicable only to MMS with MMBox support.
 */
typedef struct mbox_flags
{
	JC_CHAR			*pAddTokens ;		/**< Specifies the keyword to be added to MBox (comma separated)*/
	JC_CHAR			*pRemoveTokens ;	/**< Specifies the keyword to be deleted from MBox (comma separated)*/
	JC_CHAR			*pFilterTokens ;	/**< Specifies the keyword to be used for filtering. Should be used only with <B>E_MSG_TYPE_MMBOX_VIEW</B> (comma separated)*/
} MBOX_FLAGS ;


/**
 * @brief Defines the MMBox information 
 * 
 * This is applicable only to MMS with MMBox support.
 */
typedef struct mbox_info
{
	JC_BOOLEAN		bMBoxStore;			/**< E_TRUE Specifies that sender wants the MMS to be saved in the MBox in addition to sending it*/
	EMBoxState		eMBoxState ;		/**< Specifies the MBox folder to be used for saving the sent message. Can be either of <I>Draft, Sent, New, Retrieved, Forwarded</I>*/
	MBOX_FLAGS   	*pMBoxFlagsType ;	/**< Specifies the keyword to be added/deleted from MBox*/
} MBOX_INFO ;


/**
 * @brief Defines the previous senders information.
 */
typedef struct prev_sent_by
{
	JC_CHAR				*pSenderAddress ;	/**< Specifies the previous senders address (;) semicolon separated*/
	JC_UINT32			uiSentDate ;		/**< Specifies the date on which the previous sender forwarded this message*/
} PREV_SENT_BY ;


/**
 * @brief Defines the custom header information to be added to the messages. 
 */
typedef struct user_header
{
	JC_INT8 			*pszName ;	/**< Specifies the header name string.*/
	JC_CHAR 			*pszValue ;	/**< Specifies the header value string*/
	struct user_header	*pNext ;	/**< Specifies Pointer to next header information*/
} USER_HEADERS ;


/**
 * @brief Defines list of message fields that will appear in E_MSG_TYPE_MMBOX_VIEW_RESPONSE 
 */
typedef struct mbox_attributes
{
	EMBoxAttributeType	eMBoxAttribute ;	/**< Specifies the MBox attribute*/
	struct mbox_attributes	*pNext ;		/**< Specifies next <B>MBOX_ATTRIBUTES</B>information*/
}MBOX_ATTRIBUTES ;


/**
 * @brief  Defines reply charging information
 */
typedef struct reply_charging
{
	EReplyCharging	eReplyCharging ;	/**< Specifies the type of reply charging.For send request ONLY <B>E_REQUEST_REPLY_CHARGING</B> or <B>E_REQUEST_TEXT_ONLY_REPLY_CHARGING</B> should be used*/
	JC_UINT32		uiRelativeTime ;	/**<  Specifies the latest time of the recipients) to submit the Reply-MM. After this time the originator of the Original-MM will not pay for the Reply-MM any more. (Relative from the timer received - in seconds)*/
	JC_UINT32		uiSize ;			/**< Specifies the maximum size (number of octets) for the Reply-MM*/
	JC_INT8			*pszReplyId ;		/**< Specifies the Message-ID of the original Message that is replied to.This should be set ONLY if it's a reply message (i.e. reply message with reply-charging).This will be filled by core.*/
} REPLY_CHARGING ;


/**
 * @brief  Defines the content location strings list
 */
typedef struct location_list		
{
	JC_INT8				*pLocation ;		/**< Specifies the location/path on server where information is stored*/
	struct location_list		*pNext ;	/**< Specifies next content location*/
} CONTENT_LOCATION_LIST ;


/**
 * @brief  Defines the response status list.
 */
typedef struct response_status_list	
{
	EMM1ResponseStatus		eStatus ;		/**< Specifies response status*/
	JC_CHAR					*pStatusText;	/**< Contains the response status text received*/
	struct response_status_list	*pNext ;	/**< Specifies the next response status*/
} RESP_STATUS_LIST ;


/**
 * @brief  Defines the header information for E_MSG_TYPE_SEND PDU
 */
typedef struct message_send
{
	JC_INT8				*pTransactionID ;	/**< <B>[OPTIONAL]</B> Only valid for MMS. Specifies unique identifier string for the message PDU. This transaction ID identifies the M-Send.req (E_MSG_TYPE_SEND) and the corresponding reply (E_MSG_TYPE_SEND_RESPONSE) only.Note : MMS core will fill this value if the client leaves this field blank*/
	JC_INT8				*pVersion ;			/**< <B>[OPTIONAL]</B> Specifies the Messaging types version. For MMS currently valid values are "1.0", "1.1" and "1.2".Default value is "1.2" for MMS*/
	JC_UINT32			uiDate ;			/**< <B>[OPTIONAL]</B> Only for MMS. Specifies the date of message submit. Time in seconds from 1970-01-01, 00:00:00 GMT. If the client does not fill this field, MMS-relay server will fill*/
	JC_INT8				*pszDate ;			/**< Date in string format (For EMail)*/
	JC_CHAR				*pFrom ;			/**< Specifies the sender's address*/
	JC_CHAR				*pReplyTo ;			/**< For Email - Specifies Address to which reply is received*/
	JC_CHAR				*pTo ;				/**< <B>[OPTIONAL]</B> Specifies the recipient's addresses. Multiple addresses should be separated by semi-colon (;)*/
	JC_CHAR				*pCc ;				/**< <B>[OPTIONAL]</B> Specifies the recipient's addresses. Multiple addresses should be separated by semi-colon (;)*/
	JC_CHAR				*pBcc ;				/**< <B>[OPTIONAL]</B> Specifies the recipient's addresses. Multiple addresses should be separated by semi-colon (;)*/
	JC_CHAR				*pSubject ;			/**< <B>[OPTIONAL]</B> Specifies the subject for the message*/
	EMessageClass		eMsgClass ;			/**< <B>[OPTIONAL]</B> Specifies the class of the message. For MMS default value is <I>Personal</I> class*/
	JC_BOOLEAN			bExpiryAbs ;		/**< Specifies whether expiry time is absolute or not */
	JC_UINT32			uiExpiryTime ;		/**< <B>[OPTIONAL]</B> Only for MMS. Specifies the length of time (Relative time in seconds) the MM will be stored in MMS Proxy-Relay or time to delete the MM. Default is maximum time*/
	JC_BOOLEAN			bDeliveryAbs ;		/**< Specifies whether delivery time is absolute or not*/
	JC_UINT32			uiDeliveryTime ;	/**< <B>[OPTIONAL]</B> Only for MMS. Specifies the earliest possible delivery time (Relative time in seconds) of the MM to the recipient. Default is immediate delivery*/
	EMessagePriority	eMsgPriority ;		/**< <B>[OPTIONAL]</B> Specifies the message priority. For MMS default value is <I>Normal</I> priority.*/
	JC_BOOLEAN			bHideSender ;		/**< <B>[OPTIONAL]</B> Specifies whether the originator of the message wish to show or hide her address*/
	JC_BOOLEAN			bDeliveryReport ;	/**< <B>[OPTIONAL]</B> Specifies whether the originator client requests a delivery report from each recipient*/
	JC_BOOLEAN			bReadReport ;		/**< <B>[OPTIONAL]</B> Specifies whether the originator wants a read report from each recipient*/
	MBOX_INFO			*pMBoxInfo ;		/**< <B>[OPTIONAL]</B> Valid only for MMS with MBox support. Specifies MBox details*/
	REPLY_CHARGING		*pReplyChargingInfo ;	/**< <B>[OPTIONAL]</B> Valid only for MMS version 1.1 and above. Specifies the reply charging details*/
	USER_HEADERS 		*pUserHeaders ;			/**< <B>[OPTIONAL]</B> Specifies the additional header information set by the user*/
} MESSAGE_SEND ;


/**
 * @brief  Defines the header information for E_MSG_TYPE_SEND_RESPONSE PDU.
 *
 * This PDU is only for MMS. This will be received as response for E_MSG_TYPE_SEND (from server)
 */
typedef struct send_response
{
	JC_INT8				*pTransactionID ;	/**< <B>[MANDATORY]</B> Only valid for MMS. Unique identifier string for the message PDU. This transaction ID identifies the reply (E_MSG_TYPE_SEND_RESPONSE) and the corresponding M-Send.req (E_MSG_TYPE_SEND) only*/
	JC_INT8				*pVersion ;			/**< <B>[MANDATORY for MMS]</B> Specifies the Messaging types version. For MMS currently valid values are "1.0", "1.1" and "1.2". Default value is "1.2" for MMS*/
	EMM1ResponseStatus	eStatus ;			/**< Status of the send request*/
	JC_CHAR				*pStatusText;		/**< Specifies the status string corresponding to M-Send.req request*/
	JC_INT8				*pMessageID ;		/**< <B>[OPTIONAL]</B> Only for MMS. Unique reference assigned to the message on successful submission. This ID enables a MMS client to match delivery reportsor read-report PDU's with previously sent MM.*/
	JC_INT8				*pContentLocation ;	/**< Specifies a reference to the stored version of the MM (MMBox)*/
	EMM1StoreStatus		eStoreStatus ;		/**< Specifies the status for MM Box store request*/
	JC_CHAR				*pStoreStatusText ;	/**< <B>[OPTIONAL]</B>  Only for MMS with MBox support. Indicates if the submitted MM was successfully stored into the MMBox*/
	USER_HEADERS		*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Message type specific information (Header Name - Value pairs) that is not defined in the structure*/
} MESSAGE_SEND_RESPONSE ;


/**
 * @brief  Defines the header information for E_MSG_TYPE_NOTIFICATION PDU
 *
 * This PDU informs about the presence of new MMS on the server
 */
typedef struct  message_notification
{
	JC_INT8				*pTransactionID ;		/**< <B>[MANDATORY]</B> Only valid for MMS. This transaction ID identifies the M-Notification.ind (E_MSG_TYPE_NOTIFICATION) and the corresponding M-NotifyResp.ind (E_MSG_TYPE_NOTIFICATION_RESPONSE)*/
	JC_INT8				*pVersion ;				/**< <B>[MANDATORY]</B> Specifies the Messaging types version. For MMS currently valid values are "1.0", "1.1" and "1.2"*/
	JC_UINT32			uiReceivedTime ;		/**< In seconds from 1970-01-01, 00:00:00 GMT. Core Module (Storage Module) will fill this value.*/
	JC_CHAR				*pFrom ;				/**< <B>[OPTIONAL]</B> Specifies the sender's address.*/
	JC_CHAR				*pSubject ;				/**< <B>[Optional]</B> Specifies the subject of the message.*/
	JC_BOOLEAN			bDeliveryReport ;		/**< <B>[OPTIONAL]</B> Specifies whether the originator client wants a delivery report from each recipient.*/
	JC_BOOLEAN			bMMBoxStored;			/**< <B>[OPTIONAL]</B> Only for MMS with MBox support. Indicates that the MM was stored to the user's MMBox.*/
	EMessageClass		eMsgClass ;				/**< <B>[MANDATORY for MMS]</B> Specifies the class of the message*/
	EMessagePriority	eMsgPriority ;			/**< <B>[OPTIONAL]</B> Specifies the message priority*/
	JC_UINT32			uiMessageSize ;			/**< <B>[MANDATORY for MMS]</B> Specifies the size of the message in bytes*/
	JC_BOOLEAN			bExpiryAbs ;			/**< Specifies whether expiry time is absolute or not*/
	JC_UINT32			uiExpiryTime ;			/**< <B>[OPTIONAL]</B> Only for MMS. Specifies the length of time the message will be available*/
	REPLY_CHARGING		*pReplyChargingInfo ;	/**< <B>[OPTIONAL]</B> Valid only for MMS version 1.1 and above. Specifies the reply charging details*/
	JC_BOOLEAN			bDistribution ;			/**< <B>[OPTIONAL]</B> Only for MMS. Indicates to the recipient whether the content of the MM can be distributed*/
	JC_INT8				*pMMSElementDescriptor ;/**< <B>[OPTIONAL]</B> Only for MMS. Contains the Content-Reference associated with the corresponding top level message content of the MM waiting for retrieval*/
	JC_INT8				*pContentLocation ;		/**< <B>[MANDATORY]</B> Only for MMS. Specifies the location of the MM to be retrieved*/
	USER_HEADERS		*pUserHeaders ;			/**< <B>[OPTIONAL]</B> Contains additional header information*/
} MESSAGE_NOTIFICATION ;


/**
 * @brief  Defines the header information for E_MSG_TYPE_NOTIFICATION_RESPONSE PDU.
 *
 * This PDU is used to sent an acknowledgement for MESSAGE_NOTIFICATION received from
 * the server (in case of Deferred download).
 */
typedef struct notification_response
{
	JC_INT8			*pTransactionID ;	/**< <B>[MANDATORY]</B> Only valid for MMS. This transaction ID Identifies the transaction started by notification indication (E_MSG_TYPE_NOTIFICATION)*/
	JC_INT8			*pVersion ;			/**< <B>[MANDATORY]</B> Specifies the Messaging types version. For MMS currently valid values are "1.0", "1.1" and "1.2". Default value is "1.2" for MMS*/
	EMessageStatus	eMessageStatus ;	/**< <B>[OPTIONAL]</B> Only for MMS. Specifies the status of the message i.e. whether retrieved/rejected/deferred*/
	JC_BOOLEAN		bDeliveryReport ;	/**< <B>[OPTIONAL]</B> Only for MMS. Indication whether or not the sending of delivery report is allowed by the recipient MMS Client*/
	USER_HEADERS	*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Used by messaging types other than MMS for sending headers that are not defined in this structure*/
} NOTIFICATION_RESPONSE;


/**
 * @brief Defines the header information for E_MSG_TYPE_RECEIVE PDU.
 */
typedef struct message_retrieval
{
	JC_INT8				*pTransactionID ;		/**< <B>[OPTIONAL]</B> Only valid for MMS. Identifies either the transaction that has been started by M-Notification.ind PDU (E_MSG_TYPE_NOTIFICATION) without M-NotifyResp.ind PDU (immediate retrieval) or a new transaction when deferred retrieval was requested*/
	JC_INT8				*pVersion ;				/**< <B>[MANDATORY]</B> Specifies the Messaging types version. For MMS currently valid values are "1.0", "1.1" and "1.2"*/
	JC_INT8				*pMessageID ;			/**< Specifies an unique reference assigned to the message*/
	JC_UINT32			uiDate ;				/**< <B>[MANDATORY for MMS]</B> Specifies the date and time of latest submission of the message by sender (relative seconds).*/
	JC_INT8				*pszDate ;				/**< Date in string format should be Day,<I>Date</I> <I>Month</I> <I>Year</I> <I>UTC</I> +-<I>TimeDifference</I>*/
	JC_CHAR				*pFrom ;				/**< <B>[OPTIONAL]</B> Specifies the sender's address*/
	PREV_SENT_BY		*pPrevSentBy ;			/**< <B>[OPTIONAL]</B> Specifies the details about the previous sender*/
	JC_CHAR				*pTo ;					/**< <B>[OPTIONAL]</B> Specifies recipients' addresses. Multiple address separated by semi-colon (;)*/
	JC_CHAR				*pCc ;					/**< <B>[OPTIONAL]</B> Specifies recipients' addresses. Multiple address separated by semi-colon (;)*/
	JC_CHAR				*pSubject ;				/**< <B>[OPTIONAL]</B> Specifies the subject of the message.*/
	EMBoxState			eMBoxState ;			/**< <B>[OPTIONAL]</B> Only for MMS with MBox support. This field is valid only for the messages that are retrieved from MBox*/
	MBOX_FLAGS			*pMBoxFlagsType ;		/**< <B>[OPTIONAL]</B> Only for MMS with MBox support. This filed will appear only for messages that are retrieved from MBox*/
	EMessageClass		eMsgClass ;				/**< <B>[OPTIONAL]</B> Specifies the class of the message. For MMS default is <I>Personal</I> class.*/
	EMessagePriority	eMsgPriority ;			/**< <B>[OPTIONAL]</B> Specifies the message priority. For MMS default is <I>Normal</I> priority.*/
	JC_BOOLEAN			bDeliveryReport ;		/**< <B>[OPTIONAL]</B> Specifies whether the originator client wants a delivery report from each recipient*/
	JC_BOOLEAN			bReadReport ;			/**< <B>[OPTIONAL]</B> Specifies whether the originator wants a read report from each recipient as a new message. For MMS default is <I>No</I>*/
	REPLY_CHARGING		*pReplyChargingInfo ;	/**< <B>[OPTIONAL]</B> Valid only for MMS version 1.1 and above. Specifies the reply charging details*/
	EMM1RetrieveStatus	eStatus ;				/**< Specifies the retrieval status*/
	JC_CHAR				*pStatusText ;			/**< <B>[OPTIONAL]</B> Description, which qualifies the retrieve status value (sent by server)*/
	JC_BOOLEAN			bDistribution ;			/**< <B>[OPTIONAL]</B> Only for MMS. Indicates to the recipient whether the content of the MM can be distributed*/
	USER_HEADERS		*pUserHeaders ;			/**< <B>[OPTIONAL]</B> Contains additional header information. Used by messaging types other than MMS for sending headers that are not defined in this structure*/
} MESSAGE_RETRIEVAL ;


/**
 * @brief Defines the header information for E_MSG_TYPE_ACKNOWLEDGE PDU.
 *
 * This PDU is used to sent an acknowledgement after successfully downloading the message (only for deferred retrieval)
 */
typedef struct message_ack
{
	JC_INT8			*pTransactionID ;	/**< <B>[MANDATORY]</B> Only valid for MMS. This transaction ID identifies the M-Acknowledge.indPDU and the corresponding M-Retrieve.conf PDU [E_MSG_TYPE_RECEIVE]*/
	JC_INT8			*pVersion ;			/**< <B>[MANDATORY]</B> Specifies the Messaging types version. For MMS currently valid values are "1.0", "1.1" and "1.2". Default value is "1.2" for MMS*/
	JC_BOOLEAN		bDeliveryReport ;	/**< <B>[OPTIONAL]</B> Specifies whether sending of delivery report to the originator is allowed. For MMS default is <I>Yes</I>*/
	USER_HEADERS	*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Contains additional header information. Used by messaging types other than MMS for sending headers that are not defined in this structure*/
}MESSAGE_ACK ;


/**
 * @brief Defines the header information for E_MSG_TYPE_FORWARD PDU
 *
 * This PDU will be used for forwarding a message without downloading from server
 *
 * @note
 *    Valid only for MMS versions 1.1 and above
 */
typedef struct forward_message
{
	JC_INT8			*pTransactionID ;	/**< <B>[OPTIONAL]</B> Specifies a unique identifier for the forward transaction that provides linkage between the M-Forward.req (E_MSG_TYPE_FORWARD) and corresponding M-Forward.conf message (E_MSG_TYPE_FORWARD_RESPONSE).Note : MMS core will fill this value if the client leaves this field blank*/
	JC_INT8			*pVersion ;			/**< <B>[OPTIONAL]</B> Specifies the Messaging types version. Currently valid values are "1.1" and "1.2".Default value is "1.2" for MMS*/
	JC_UINT32		uiDate ;			/**< <B>[OPTIONAL]</B> Specifies the date of message submit. Absolute time in seconds from 1970-01-01, 00:00:00 GMT. If the client does not fill this field, MMS-relay server will fill.*/
	JC_CHAR			*pFrom ;			/**< Specifies the sender's address.*/
	JC_CHAR			*pTo ;				/**< <B>[OPTIONAL]</B> Specifies the recipient's addresses. Multiple addresses should be separated by semi-colon (;)*/
	JC_CHAR			*pCc ;				/**< <B>[OPTIONAL]</B>  Specifies the recipient's addresses. Multiple addresses should be separated by semi-colon (;)*/
	JC_CHAR			*pBcc ;				/**< <B>[OPTIONAL]</B> Specifies the recipient's addresses. Multiple addresses should be separated by semi-colon (;)*/
	JC_BOOLEAN		bExpiryAbs ;		/**< Specifies whether expiry time is absolute or not*/
	JC_UINT32		uiExpiryTime ;		/**< <B>[OPTIONAL]</B> Specifies the length of time (Relative time in seconds) the MM will be stored in MMS Proxy-Relay or time to delete the MM. Default is maximum time*/
	JC_BOOLEAN		bDeliveryAbs ;		/**< Specifies whether delivery time is absolute or not*/
	JC_UINT32		uiDeliveryTime ;	/**< <B>[OPTIONAL]</B>  Specifies the earliest possible delivery time (Relative time in seconds) of the MM to the recipient. Default is immediate delivery*/
	JC_BOOLEAN		bReportPrevSender ;	/**< <B>[OPTIONAL]</B>  Specifies whether sending of delivery report allowed to the previous sender of the message to be forwarded*/
	JC_BOOLEAN		bDeliveryReport ;	/**< <B>[OPTIONAL]</B> Specifies whether the originator client requests a delivery report from each recipient*/
	JC_BOOLEAN		bReadReport ;		/**< <B>[OPTIONAL]</B> Specifies whether the originator wants a read report from each recipient*/
	MBOX_INFO		*pMBoxInfo ;		/**< <B>[OPTIONAL]</B> Specifies MBox details*/
	JC_INT8			*pContentLocation ;	/**< <B>[MANDATORY]</B> This field specifies the location of the message to be forwarded, as received in the M-Notification.ind message. sender of the message*/
	USER_HEADERS	*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Specifies the additional header information set by the user*/
} FORWARD_MESSAGE ;


/**
 * @brief Defines the header information for E_MSG_TYPE_FORWARD_RESPONSE PDU
 *
 * This PDU will be received from the server in response to E_MSG_TYPE_FORWARD
 *
 * @note
 *    Valid only for MMS versions 1.1 and above
 */
typedef struct forward_response
{
	JC_INT8				*pTransactionID ;	/**< <B>[OPTIONAL]</B> Specifies a unique identifier for the forward transaction that provides linkage between the M-Forward.req (E_MSG_TYPE_FORWARD) and corresponding M-Forward.conf PDU (E_MSG_TYPE_FORWARD_RESPONSE). It originates from the M-Forward.req PDU*/
	JC_INT8				*pVersion ;			/**< <B>[OPTIONAL]</B> Specifies the Messaging types version. Currently valid values are "1.1" and "1.2".Default is "1.2"*/
	EMM1ResponseStatus	eStatus ;			/**< Specifies the status of forward request*/
	JC_CHAR				*pStatusText ;		/**< <B>[MANDATORY]</B> Description, which qualifies the response status value*/
	JC_INT8				*pMessageID ;		/**< <B>[OPTIONAL]</B> A unique reference assigned to message.*/
	JC_INT8				*pContentLocation ;	/**< <B>[OPTIONAL]</B> This field appears if the corresponding E_MSG_TYPE_FORWARD request has enabled store option. Specifies the location of the stored version of the MM*/
	EMM1StoreStatus		eStoreStatus ;		/**< Specifies the status if the submitted MM was successfully stored into the MMBox*/
	JC_CHAR				*pStoreStatusText ;	/**< <B>[OPTIONAL]</B> This field appears if the corresponding E_MSG_TYPE_FORWARD request has enabled store option. Indicates if the submitted MM was successfully stored into the MMBox*/
	USER_HEADERS		*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Contains additional header information sent by the server*/
} FORWARD_RESPONSE ;


/**
 * @brief Defines the header information for E_MSG_TYPE_DELIVERY_REPORT PDU.
 *
 * This PDU will be received from the server to indicate the delivery status of the
 * message to the recipient
 */
typedef struct delivery_report
{
	JC_INT8			*pVersion ;			/**< <B>[OPTIONAL]</B> Specifies the Messaging types version. Currently valid values for MMS are "1.0", "1.1" and "1.2"*/
	JC_INT8			*pMessageID ;		/**< <B>[OPTIONAL]</B> For MMS, this is the reference that was originally assigned to the MM by the MMS Proxy-Relay and included in the corresponding E_MSG_TYPE_SEND_RESPONSE or E_MSG_TYPE_FORWARD_RESPONSE PDU*/
	JC_CHAR			*pTo ;				/**< <B>[MANDATORY]</B> Specifies the recipient for whom the message has been delivered*/
	JC_UINT32		uiDate ;			/**< <B>[MANDATORY]</B> Specifies the date on which the message has been delivered*/
	JC_INT8			*pszDate ;			/**< Date in string format should be Day,<I>Date</I> <I>Month</I> <I>Year</I> <I>UTC</I> +-<I>TimeDifference</I>*/
	EMessageStatus	eDeliveryStatus ;	/**< <B>[MANDATORY]</B>  Status of the message*/
	USER_HEADERS	*pUserHeaders ;		/**< <B>[OPTIONAL]</B>Headers sent by the server that are not listed in this structure (name - value pairs)*/
} DELIVERY_REPORT ;


/**
 * @brief Defines the header information for E_MSG_TYPE_READ_REPORT PDU
 *
 * Defines the header information for sending
 * read report PDU (E_MSG_TYPE_READ_REPORT) to
 * the sender. For MMS, Version 1.1 and above
 * supports sending read report PDU (m-read-rec-ind).
 * For MMS version 1.0 read report should be sent
 * as a new MMS Message. 
 */
typedef struct read_report
{
	JC_INT8				*pVersion ;					/**< <B>[OPTIONAL]</B> Specifies the Messaging types version. Currently valid values for MMS are "1.1" and "1.2"*/
	JC_INT8				*pMessageID ;				/**< <B>[MANDATORY]</B> For MMS, This is the reference that was originally assigned to the MM by the MMS Proxy-Relay and included in the corresponding E_MSG_TYPE_RECEIVE PDU*/
	JC_CHAR				*pTo ;						/**< <B>[MANDATORY]</B> Specifies the address of the recipient of the Read Report, i.e. the originator of the original message*/	
	JC_CHAR				*pFrom ;					/**< <B>[MANDATORY]</B> Specifies the sender's address*/
	JC_UINT32			uiDate ;					/**< <B>[MANDATORY]</B> Specifies the date on which the message has been read (absolute). For MMS, if this field is blank, Proxy-relay server will fill*/
	JC_INT8				*pszDate ;					/**< Date in string format should be Day,<I>Date</I> <I>Month</I> <I>Year</I> <I>UTC</I> +-<I>TimeDifference</I>*/
	EMessageReadStatus	eReadStatus ;			/**< <B>[MANDATORY]</B> The status of the message*/
	JC_CHAR				*pReplyTo ;					/**< For Email - Specifies Address to which reply is received*/
	JC_CHAR				*pSubject ;					/**< Specifies subject field. Used only for Email*/
	EMessageDispositionActionMode  	eActionMode ;	/**< Specifies setting action for sending read receipt*/
	USER_HEADERS		*pUserHeaders ;				/**< <B>[OPTIONAL]</B> Additional headers that are not listed in this structure (name - value pairs)*/
} READ_REPORT ;


/**
 * @brief Defines the header information for E_MSG_TYPE_READ_REPORT_RESPONSE PDU.
 *
 * This PDU is used to send read report to the original sender (if requested)
 *
 * @note
 *    - Valid only for MMS versions 1.1 and above
 */
typedef struct read_report_response
{
	JC_INT8				*pVersion ;		/**< <B>[OPTIONAL]</B> Specifies the Messaging types version. Currently valid values for MMS are "1.1" and "1.2"*/
	JC_INT8				*pMessageID ;	/**< <B>[MANDATORY]</B> For MMS, This is the reference that was originally assigned to the MM by the MMS Proxy-Relay and included in the corresponding E_MSG_TYPE_SEND_RESPONSE or E_MSG_TYPE_FORWARD_RESPONSE PDU*/
	JC_CHAR				*pTo ;			/**< <B>[MANDATORY]</B> Specifies the address of the recipient of the Read Report, i.e. the originator of the original message*/
	JC_CHAR				*pFrom ;		/**< <B>[MANDATORY]</B> Specifies the sender's address*/
	JC_UINT32			uiReceivedTime ;/**< <B>[MANDATORY]</B> Specifies the date on which the message has been read. For MMS, if this field is blank, Proxy-relay server will fill*/
	EMessageReadStatus	eReadStatus ;	/**< <B>[MANDATORY]</B> The status of the message*/
	USER_HEADERS		*pUserHeaders ;	/**< <B>[OPTIONAL]</B> Additional headers that are not listed in this structure (name - value pairs)*/
}READ_REPORT_RESPONSE ;


/**
 * @brief  Defines the header information for E_MSG_TYPE_MMBOX_STORE PDU
 *
 * This PDU is used to save a MMS (not been retrieved as yet) to MMBox (or) to update
 * the Status or MM Flags associated with an MM that already resides in the MMBox.
 *
 * @note
 *    - Valid only for MMS versions 1.2 and above with MMBox support
 */
typedef struct mbox_store
{
	JC_INT8			*pTransactionID ;	/**< <B>[OPTIONAL]</B> A unique identifier for the store/update transaction that provides linkage between the E_MSG_TYPE_MMSTORE_REQUEST and corresponding E_MSG_TYPE_MMSTORE_RESPONSE PDUs. If this field is blank, MMS core will fill with a unique id*/
	JC_INT8			*pVersion ;			/**< <B>[OPTIONAL]</B> Specifies the Messaging types version. Valid version is "1.2"*/
	JC_INT8			*pContentLocation ;	/**< <B>[MANDATORY]</B> This field specifies the location of the message to be stored or updated. This reference is obtained from any transaction that stored a message to the user's MMBox*/
	MBOX_INFO 		*pMBoxInfo ;		/**< <B>[OPTIONAL]</B> Specifies the MBox details*/
	USER_HEADERS	*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Specifies additional headers*/
}MBOX_STORE_REQUEST ;


/**
 * @brief Defines the header information for E_MSG_TYPE_MMBOX_STORE_RESPONSE PDU
 *
 * This PDU is sent by server in response to E_MSG_TYPE_MMBOX_STORE
 *
 * @note
 *    - Valid only for MMS versions 1.2 and above with MMBox support
 */
typedef struct mbox_store_response
{
	JC_INT8			*pTransactionID ;	/**< <B>[MANDATORY]</B> A unique identifier for the store/update transaction that provides linkage between the E_MSG_TYPE_MMSTORE_REQUEST and corresponding E_MSG_TYPE_MMSTORE_RESPONSE PDU*/
	JC_INT8			*pVersion ;			/**< <B>[MANDATORY]</B> Specifies the Messaging types version. Valid version is "1.2"*/
	JC_INT8			*pContentLocation ;	/**< <B>[OPTIONAL]</B> A reference to the stored version of the MM that can be retrieved later. This field will appear only if the E_MSG_TYPE_MMSTORE_REQUEST is success*/
	EMM1StoreStatus	eStoreStatus ;		/**< Specifies the status of the store request*/
	JC_CHAR			*pStoreStatusText ;	/**< <B>[MANDATORY]</B> Description, which qualifies store request*/
	USER_HEADERS	*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Contains the headers that are not listed in this structure (name - value pairs)*/
}MBOX_STORE_RESPONSE ;


/**
 * @brief Defines the header information for MMDESCR PDU
 *
 * Specifies the details for the MMBox Description PDU that will be used with
 * E_MSG_TYPE_MMBOX_VIEW_RESPONSE and E_MSG_TYPE_MMBOX_UPLOAD.
 *
 * @note
 *    - Valid only for MMS versions 1.2 and above with MMBox support
 */
typedef struct mbox_descr
{
	JC_INT8				*pContentLocation ;	/**< [CONDITIONAL] This field shall only be used in conjunction with the E_MSG_TYPE_MMBOX_VIEW_RESPONSE PDU. Specifies the location URI that may be used to retrieve this MM in a future operation*/
	JC_INT8				*pMessageID ;		/**< [CONDITIONAL] This field shall only be used in conjunction with the E_MSG_TYPE_MMBOX_VIEW_RESPONSE PDU. This field SHALL be present when included in the E_MSG_TYPE_MMBOX_UPLOAD if the MM being uploaded has been assigned a message-id by a previous transaction*/
	MBOX_INFO			*pMBoxInfo ;		/**< <B>[OPTIONAL]</B> Specifies the MBox information*/
	JC_UINT32			uiDate ;			/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the attributes list in the request and is present in the associated MM (Absolute time in seconds from 1-1-1970 00:00:00 GMT)*/
	JC_CHAR				*pFrom ;			/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the attributes list in the request and is present in the associated MM (Absolute time in seconds from 1-1-1970 00:00:00 GMT)*/
	JC_CHAR				*pTo ;				/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM. Multiple values will be separated by semi-colon (;)*/
	JC_CHAR				*pCc ;				/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM. Multiple values will be separated by semi-colon (;)*/
	JC_CHAR				*pBCc ;				/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM. Multiple values will be separated by semi-colon (;)*/
	JC_CHAR				*pSubject ;			/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM*/
	EMessageClass		eMsgClass ;			/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM*/
	EMessagePriority	eMsgPriority ;	/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM*/
	JC_BOOLEAN			bDeliveryAbs ;		/**< Specifies whether delivery time is absolute or not*/
	JC_UINT32			uiDeliveryTime ;	/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM*/
	JC_BOOLEAN			bExpiryAbs ;		/**< specifies whether expiry time is absolute or not*/
	JC_UINT32			uiExpiryTime ;		/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM*/
	JC_BOOLEAN			bDeliveryReport ;	/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM*/
	JC_BOOLEAN			bReadReport ;		/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM*/
	JC_UINT32			uiMessageSize ;		/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request*/
	REPLY_CHARGING		*pReplyCharging ;	/**< <B>[OPTIONAL]</B> This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM*/
	JC_CHAR				*pPrevSentBy ;		/**< This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM*/
	JC_UINT32			uiPrevSentDate ;	/**< This value SHALL appear only if requested as part of the Attributes-list in the request and is present in the associated MM*/
	USER_HEADERS		*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Specifies the additional header information set by the user*/
	MSG_HANDLE			hMBoxMsgHandle ;	/**< <B>[OPTIONAL]</B> If the "Content" was requested as part of the X-Mms-Attributes field and the associated MM includes a body then the body SHALL appear as the Data of the Multipart Entry and will be parsed and filled by the MMS Core.If Content has been requested as a part of MM View Request, "application/vnd.wap.mms-message" will be filled*/
	struct mbox_descr	*pNext ;			/**< <B>[OPTIONAL]</B> Points to the next MMDESCR associated with the request*/

}MMDESCR ;


/**
 * @brief Defines the header information for E_MSG_TYPE_MMBOX_VIEW PDU.
 *
 * This PDU is used to retrieve information of messages in MMBox
 *
 * @note
 *    - Valid only for MMS versions 1.2 and above with MMBox support
 */
typedef struct mbox_view
{
	JC_INT8					*pTransactionID ;	/**< <B>[OPTIONAL]</B> A unique identifier for the viewing transaction that provides linkage between the E_MSG_TYPE_MMBOX_VIEW and corresponding E_MSG_TYPE_MMBOX_VIEW_RESPONSE PDU.Note: MMS Core will fill this field with a unique-id if its left blank by the client*/
	JC_INT8					*pVersion ;			/**< <B>[OPTIONAL]</B> Specifies the Messaging types version. Valid version is "1.2". If not filled, MMS Core will set the version to "1.2"*/
	CONTENT_LOCATION_LIST	*pContentLocation ;	/**< <B>[OPTIONAL]</B> A message reference obtained from any transaction that stored a message to the MMBox or a previous result from the E_MSG_TYPE_MMBOX_VIEW_RESPONSE. Attributes of the specific message that is referenced should be returned*/
	MBOX_INFO 				*pMBoxInfo ;		/**< <B>[OPTIONAL]</B> Specifies MBox information for the request*/
	JC_UINT32				uiStartIndex ;		/**< <B>[OPTIONAL]</B> Indicating the index of the first MM of those selected to have information returned in the response*/
	JC_UINT32				uiTotalItems ;		/**< <B>[OPTIONAL]</B> A number indicating the maximum number of selected MMs whose information SHALL be returned in the response*/
	MBOX_ATTRIBUTES			*pMBoxAttributes ;	/**< <B>[OPTIONAL]</B> A list of information elements that should appear in the view for each selected message. If this filed is not set then MMS Proxy-Relay SHOULD use a system default*/
	JC_BOOLEAN				bMBoxTotal ;		/**< <B>[OPTIONAL]</B> Indicates that MMS Client is requesting a total count of messages currently stored in MMBox. Default is <I>No</I>*/
	JC_BOOLEAN				bMBoxQuota ;		/**< <B>[OPTIONAL]</B> Indicates that MMS Client is requesting an indication of the quotas defined for user's MMBox. Default is <I>No</I>*/
	USER_HEADERS			*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Contains the headers that are not listed in this structure (name - value pairs)*/
} MBOX_VIEW ;


/**
 * @brief Defines the header information for E_MSG_TYPE_MMBOX_VIEW_RESPONSE PDU
 *
 * This PDU is sent by server in response to E_MSG_TYPE_MMBOX_VIEW
 *
 * @note
 *    - Valid only for MMS versions 1.2 and above with MMBox support
 */
typedef struct  mbox_view_response
{
	JC_INT8					*pTransactionID ;	/**< <B>[MANDATORY]</B> A unique identifier for the viewing transaction that provides linkage between the E_MSG_TYPE_MMBOX_VIEW and corresponding E_MSG_TYPE_MMBOX_VIEW_RESPONSE PDU*/
	JC_INT8					*pVersion ;			/**< <B>[MANDATORY]</B> Specifies the Messaging types version. Valid version is "1.2"*/
	EMM1ResponseStatus		eStatus ;			/**< <B>[MANDATORY]</B> Specifies the status of the view request*/
	JC_CHAR					*pRespStatusText;	/**< <B>[MANDATORY]</B> Description that qualifies the Response-status-value*/
	CONTENT_LOCATION_LIST	*pContentLocation ;	/**< <B>[OPTIONAL]</B> The list of MM that were specified in the corresponding request. If there were no content location specified in the request this SHALL not appear in reply*/
	MBOX_INFO				*pMBoxInfo ;		/**< <B>[OPTIONAL]</B> Specifies the MBox information*/
	JC_UINT32				uiStartIndex ;		/**< <B>[OPTIONAL]</B> A number, indicating the index of the first MM of those selected to have information returned in the response. If no start field appeared in the request this SHALL not appear in the reply*/
	JC_UINT32				uiLimit ;			/**< <B>[OPTIONAL]</B> A number, indicating the number of MM selected that the client requested to be displayed. If no limit field appeared in the request this SHALL not appear in the reply*/
	MBOX_ATTRIBUTES			*pMBoxAttributes ;	/**< <B>[OPTIONAL]</B> The list of information elements those are included for each selected message. If the list was not specified in the request this MAY not appear in the response PDU*/
	JC_UINT32				uiMBoxTotal ;		/**< <B>[OPTIONAL]</B> Gives the total number of messages or bytes in the user's MMBox. This field MUST appear if the totals field was "Yes" in the corresponding request*/
	JC_UINT32				uiMBoxQuota ;		/**< <B>[OPTIONAL]</B> Gives the quotas defined for the user's MMBox in messages and/or bytes*/
	JC_UINT32				uiTotalReturned ;	/**< <B>[OPTIONAL]</B> Number of messages listed in content of the PDU*/
	USER_HEADERS			*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Contains the headers that are not listed in this structure (name - value pairs)*/
	MMDESCR					*pMBoxDescInfo ;	/**< <B>[OPTIONAL]</B> List of MMDESCR information related to each of the messages*/
} MBOX_VIEW_RESPONSE;


/**
 * @brief Defines the header information for E_MSG_TYPE_MMBOX_UPLOAD PDU.
 *
 * This PDU is used to save a MMS that is on the MMS Terminal to the user’s MMBox on server.
 *
 * @note
 *    - Valid only for MMS versions 1.2 and above with MMBox support
 */
typedef struct mbox_upload
{
	JC_INT8			*pTransactionID ;	/**< <B>[OPTIONAL]</B> A unique identifier for the upload transaction that provides linkage between the E_MSG_TYPE_MMBOX_UPLOAD and corresponding E_MSG_TYPE_MMBOX_UPLOAD_RESPONSE PDU.MMS Core will fill this field with a unique-id if its left blank by the client*/
	JC_INT8			*pVersion ;			/**< <B>[OPTIONAL]</B> Specifies the Messaging types version. Valid version is "1.2". If not filled, MMS Core will set the version to "1.2"*/
	MBOX_INFO		*pMBoxInfo ;		/**< <B>[OPTIONAL]</B> Specifies MBox information for the request*/
	USER_HEADERS	*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Additional header information that is not present in this structure (name - value pairs)*/
	MMDESCR         *pMMDescriptor ;	/**< <B>[MANDATORY]</B> Contains description about the MMS PDU that is being uploaded*/
} MBOX_UPLOAD ;


/**
 * @brief  Defines the header information for E_MSG_TYPE_MMBOX_UPLOAD_RESPONSE PDU
 *
 * This PDU is sent by server in response to E_MSG_TYPE_MMBOX_UPLOAD
 *
 * @note
 *    - Valid only for MMS versions 1.2 and above with MMBox support
 */
typedef struct mbox_upload_response
{
	JC_INT8			*pTransactionID ;	/**< <B>[MANDATORY]</B> A unique identifier for the upload transaction that provides linkage between the E_MSG_TYPE_MMBOX_UPLOAD and corresponding E_MSG_TYPE_MMBOX_UPLOAD_RESPONSE PDU*/
	JC_INT8			*pVersion ;			/**< <B>[MANDATORY]</B> Specifies the Messaging types version. Valid version is "1.2*/
	JC_INT8			*pContentLocation ;	/**< <B>[OPTIONAL]</B> Specifies a reference to the stored version of the MM that can be retrieved later*/
	EMM1StoreStatus	eStoreStatus ;		/**< Specifies the status of the upload request*/
	JC_CHAR			*pStoreStatusText ;	/**< Description that qualifies the store status*/
	USER_HEADERS	*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Additional header information those are not present in this structure (name - value pairs)*/
} MBOX_UPLOAD_RESPONSE ;


/**
 * @brief Defines the header information for E_MSG_TYPE_MMBOX_DELETE PDU
 *
 * This PDU is used to delete one or more MMS from the user’s MMBox
 *
 * @note
 *    - Valid only for MMS versions 1.2 and above with MMBox support
 */
typedef struct  mbox_delete
{
	JC_INT8					*pTransactionID ;	/**< <B>[OPTIONAL]</B> A unique identifier for the deleting transaction that provides linkage between the E_MSG_TYPE_MMBOX_DELETE and corresponding E_MSG_TYPE_MMBOX_DELETE_RESPONSE PDUs.Note: MMS Core will fill this field with a unique-id if its left blank by the client*/
	JC_INT8					*pVersion ;			/**< <B>[OPTIONAL]</B> Specifies the Messaging types version. Valid version is "1.2". If not filled, MMS Core will set the version to "1.2"*/
	CONTENT_LOCATION_LIST	*pContentLocation ;	/**< <B>[MANDATORY]</B> Specifies a reference to the stored MM that was obtained from any of the storing PDU (from previously transactions)*/
	USER_HEADERS			*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Additional header information those are not present in this structure (name - value pairs)*/
}MBOX_DELETE ;


/**
 * @brief Defines the header information for E_MSG_TYPE_MMBOX_DELETE_RESPONSE PDU
 *
 * This PDU is sent by server in response to E_MSG_TYPE_MMBOX_DELETE
 *
 * @note
 *    - Valid only for MMS versions 1.2 and above with MMBox support
 */
typedef struct mbox_delete_response
{
	JC_INT8					*pTransactionID ;	/**< <B>[MANDATORY]</B> A unique identifier for the deleting transaction that provides linkage between the E_MSG_TYPE_MMBOX_DELETE and corresponding E_MSG_TYPE_MMBOX_DELETE_RESPONSE PDUs*/
	JC_INT8					*pVersion ;			/**< <B>[MANDATORY]</B> Specifies the Messaging types version. Valid version is "1.2"*/
	CONTENT_LOCATION_LIST	*pContentLocation ;	/**< <B>[OPTIONAL]</B> Specifies a reference to the MM that was not successfully deleted*/
	RESP_STATUS_LIST		*pStatus ;			/**< <B>[MANDATORY]</B> Specifying the status of the delete request*/
	USER_HEADERS			*pUserHeaders ;		/**< <B>[OPTIONAL]</B> Additional header information those are not present in this structure (name - value pairs)*/
}MBOX_DELETE_RESPONSE ;


/**
 * @brief Defines the structure containing the header information for all types of message PDU's.
 */
typedef struct message_headers
{
	EMsgType				eMsgType;	/**< <B>[MANDATORY]</B> Specifies the message PDU type of the associated pHeaders.*/
	void					*pHeaders ;	/**< <B>[MANDATORY]</B> void pointer to hold any of the message PDU header information*/
} MESSAGE_HEADERS ;


/*********** STORAGE MODULE STRUCTURES & CALLBACKS ************/

/**
 * @brief Defines information for sorting the messages.
 */
typedef struct sort_info
{
	ESortBy 		eSortBy ;		/**< Specifies the sorting field information*/
	ESortOrder 		eSortOrder ;	/**< Specifies the sorting order information*/
} SORT_INFO;


/**
 * @brief Defines information specific to retrieve filters
 */
typedef struct retrieve_filters
{
	JC_BOOLEAN			bIsBufRequired;	/**< NOT Used */ 	
	ERetrieveFilter		eRetrieve;		/**< Specifies the option/filter to be used for retrieving the list*/
	JC_UINT32			uiStartIndex;	/**< Specifies the starting index from which the message list has to the retrieved (base index 1)*/		
	JC_UINT32			uiNumMessage;	/**< Specifies the number of messages to be retrieved. Set 0 for retrieving the all the messages*/			
} RETRIEVE_FILTERS;


/**
 * @brief Defines information specific to a folder
 */
typedef struct folder_list
{
	JC_CHAR 			*pFolderName ;			/**< Specifies the folder name*/
	JC_UINT32			uiTotalMessages;		/**< Specifies the total number of messages present in the folder*/
	JC_UINT32			uiTotalReadMessages;	/**< Specifies the total number of read messages present in the folder*/
	JC_UINT32			uiFolderSize ;			/**< Specifies the folder size in bytes*/
	JC_UINT16 			uhCategory ;			/**< Reserved for future use*/
	struct folder_list	*pNext ;				/**< Points to the next folder in the list*/
} FOLDER_LIST;


/**
 * @brief Defines information specific to folder list
 */
typedef struct folder_info
{
	JC_UINT16 		uhTotalFolders ;	/**< Specifies the total number of folders present in the folder list*/
	FOLDER_LIST		*pFolderList ;		/**< Specifies the folder list*/
} FOLDER_INFO ;


/**
 * @brief Defines information specific to message list
 */
typedef struct message_list
{
	MESSAGE_HEADERS 		*pMessageHeaders;	/**< Specifies the Message headers*/
	JC_UINT32				uiMessageID ;		/**< Specifies the Store UniqueId for the Message*/
	JC_UINT8 				*pBuffer ;			/**< Reserved for future use.Specifies the encapsulated buffer dump.*/
	JC_UINT32 				uiBufferLen	;		/**< Reserved or future use.Specifies the encapsulated buffer length*/
	JC_BOOLEAN				bDownLoadFlag;		/**< Specifies whether the message is downloaded or not*/
	JC_BOOLEAN				bReadFlag;			/**< Specifies whether the message is read or not*/
	EUpdateFlag				eUpdateFlag;		/**< Specifies the message status i.e. move, delete or modify*/
	JC_UINT32				uiExtraBytes;		/**< Specifies the user defined information (application can use this for storing custom information)*/
	JC_UINT32				uiMsgSaveDate ;		/**< Specifies the message creation time*/
	struct message_list 	*pNext ;			/**< Specifies the next message in the list*/
}MESSAGE_LIST ;


/**
 * @brief Defines receive filter information
 */
typedef struct recv_filters
{
	JC_CHAR				*pFilterName ;	/**< Specifies the filter name*/
	JC_CHAR				*pBlockList ;	/**< Specifies the list of from addresses (comma separated) that user want to block. */
	JC_CHAR				*pSubject ;		/**< Specifies the subject field that user want to filter*/
	JC_UINT8			ucMsgClass ;	/**< Specifies the message Class that user want to filter.
											0x01 - E_MSG_PERSONAL_CLASS, 0x02 - E_MSG_ADVERTISEMENT_CLASS, 0x04 - E_MSG_INFORMATION_CLASS, 0x80 - E_MSG_AUTO_CLASS*/
	JC_UINT8			ucPriority ;	/**< Specifies the message Priority that user want to filter.
											0x01 - E_MSG_PRIORITY_LOW, 0x02 - E_MSG_PRIORITY_NORMAL, 0x04 - E_MSG_PRIORITY_HIGH*/
	JC_UINT32			uiMsgSize ;		/**< Specifies the maximum message size (in bytes) that user want to allow. Set as 0 for no limit*/
	struct recv_filters *pNext ;		/**< Specifies the pointer to the next filter in the list*/
}RECV_FILTERS ;

/**
 * @brief Defines search information
 */
typedef struct st_store_search_info
{
	JC_CHAR				*pmSearchString ;	/**< Specifies the search string*/
	JC_UINT32			*pMessageIDList ;	/**< Contains the search result as an array of message ids. 
											Resource allocated by core, freed by application
											Note : Index starts from 1 */
	JC_UINT32			uiMsgCount ;		/**< Specifies the number of elements in result array - pMessageIDList*/

} ST_STORE_SEARCH_INFO ;

/*********** SMIL MODULE STRUCTURES & CALLBACKS ************/


/**
 * @brief Defines SMIL region layout information.
 */
typedef struct region_layout
{
	JC_CHAR					*pRegionName ;	/**< Specifies the region name*/
	JC_UINT32				uiWidth ;		/**< Specifies the width of region layout*/
	JC_UINT32				uiHeight ;		/**< Specifies the height of region layout*/
	JC_UINT32				uiLeft ;		/**< Specifies the left most position from root layout*/
	JC_UINT32				uiTop ;			/**< Specifies the top most position from root layout*/
	E_SMIL_REGION_FIT		eFit ;			/**< Specifies the region fit information*/
	struct region_layout	*pNext ;		/**< Points to the next region details (node)*/
} REGION_LAYOUT ;


/************ DRM *******************/


/**
 * @brief	Specifies the structure for rights object.
 */
typedef struct _st_rights_object
{
	JC_INT8						*pContentId ;	/**< Specifies content ID of the media*/
	JC_BOOLEAN					bIsRender;		/**< Specifies whether to play the media or not*/
	E_DRM_TYPE					eDRMType ;		/**< Specifies the DRM type for media object*/
	E_DRM_ERRORCODE				eErrorCode ;	/**< Specifies the error code if the media object cannot be rendered*/
	struct _st_rights_object	*pNext ;		/**< Specifies the next rights object structure*/

}ST_RIGHTS_OBJECT ;


/**
 * @brief	Specifies the structure for DRM Info.
 */
typedef struct _st_drm_info
{
	ST_RIGHTS_OBJECT	*pstRightsObject ;	/**< Specifies the rights object structure*/
	MEDIA_MIME_HEADERS	MimeHeaders ;		/**< Specifies the associated mime headers*/
	CONTENT_DATA		ContentData ;		/**< Specifies the data information*/
}ST_DRM_INFO ;


/**
 * @brief	Specifies the callback function prototype for DRM Indication Response (only in case of non blocked DRM parsing).
 *
 * @param hMsg[in]			Points to Message Handle
 * @param psRefID [in]		Specifies the DRM reference ID (refer DRM_IND_CB).
 * @param ppDRMInfo [in]	On success, points to parsed DRM objects.
 *
 * This function shall be implemented by <b>core (passed to application in DRM_IND_CB)</b>. 
 * Application shall invoke this function to pass parsed DRM contents to core.
 *
 * Note: 
 *	- Caller should not free the resource passed in psRefID and ppDRMInfo. Core will free the resources internally.
 *
 */
typedef JC_RETCODE	(* DRM_IND_RESPONSE_CB) 
										(MSG_HANDLE			hMsg, 
										 JC_INT8			*psRefID, 
										 ST_DRM_INFO		**ppDRMInfo) ;


/**
 * @brief	Specifies the callback function prototype for DRM Indication.
 *
 * @param hMsg[in]				Points to Message Handle
 * @param pAppCBArg [in]		Points to callback argument passed while registering callback.
 * @param pDRMContent [in]		Points to unparsed DRM buffer.
 * @param psContentType [in]	Points to content-type of DRM buffer.
 * @param ppsRefID [out]		On success, fill with unique reference ID for the DRM object. This ID will be used by core for future reference (updating rights, canceling parse request)
 * @param ppstDRMInfo [out]		On success, fill points to parsed DRM objects.
 * @param DRMIndResponseCB [in]	Function pointer to DRM response callback function (implemeted by core). This shall be used in case of <B>Non Blocked DRM Parsing</B>, where in application shall perform 
 *								asynchronous DRM parsing and invoke this callback function on completing parsing and pass the parsed objects to core.
 *
 * This function shall be implemented by application and registered with core before parsing a message i.e., before calling
 *	- jdi_MsgView()
 *	- jdi_MsgParseEncapsulatedBuffer()
 *	- jdi_MsgForward() (forwarding directly from buffer)
 *	- jdi_MsgCompose()
 *
 * Core will invoke this function to parse the DRM Buffer. Application shall interact with DRM parser/engine to get the parsed DRM objects (and rights) and pass to core.
 *
 * Note: 
 *	- Caller should not free the resource passed in pAppCBArg, pDRMContent, psContentType. 
 *	- Caller should not free the resources returned in ppsRefID, ppstDRMInfo (and members). Core will free the resource internally.
 *
 */
typedef JC_RETCODE	(* DRM_IND_CB) 
								(MSG_HANDLE				hMsg, 
								 void					*pAppCBArg, 
								 CONTENT_DATA			*pDRMContent, 
								 JC_INT8				*psContentType, 
								 JC_INT8				**ppsRefID, 
								 ST_DRM_INFO			**ppstDRMInfo, 
								 DRM_IND_RESPONSE_CB	DRMIndResponseCB) ;


/**
 * @brief	Specifies the callback function prototype for updating DRM Rights.
 *
 * @param hMsg[in]			Points to Message Handle
 * @param pAppCBArg [in]	Points to callback argument passed while registering callback.
 * @param psRefID [in]		Specifies the DRM reference ID (refer DRM_IND_CB).
 * @param bRender [in]		Specified whether the object has been rendered. 
 *							If object has Rights and bRender = E_TRUE, then application shall decrement the rights count.
 *
 * This function shall be implemented by application and registered with core. Core will invoke 
 * this function to indicate the application to update the rights.
 *
 * Note: 
 *	- Caller should not free the resource passed in pAppCBArg and psRefID. 
 *
 */
typedef JC_RETCODE	(* DRM_UPDATE_RIGHTS_CB) 
											(MSG_HANDLE			hMsg, 
											 void				*pAppCBArg, 
											 JC_INT8			*psRefID, 
											 JC_BOOLEAN			bRender) ;


/**
 * @brief	Specifies the callback function prototype for canceling DRM parsing (only in case of non blocked DRM parsing).
 *
 * @param hMsg[in]			Points to Message Handle
 * @param pAppCBArg [in]	Points to callback argument passed while registering callback.
 *
 * This function shall be implemented by application and registered with core. Core will invoke 
 * this function to indicate the application to cancel DRM parsing.
 *
 * Note: 
 *	- Caller should not free the resource passed in pAppCBArg. 
 *
 */
typedef JC_RETCODE	(* DRM_CANCEL_CB) 
									(MSG_HANDLE		hMsg, 
									 void			*pAppCBArg) ;


/**
 * @brief	Specifies the prototype for DRM compose Callback. Required only for applications that supports sending messages with DRM objects.
 *
 * @param hMsg[in]			Points to Message Handle
 * @param pAppCBArg [in]	Points to callback argument passed while registering callback.
 * @param pMediaMime[in]	Points to MIME Object for which DRM Encoded buffer has to be formed
 * @param ppDRMMime [out]	On success, points to DRM Encoded buffer.
 *
 * This function shall be implemented by application and registered with core. Core will invoke 
 * this function to indicate the application to form DRM encoded buffer for the protected media.
 *
 * Note: 
 *	- Caller should not free the resource passed in pAppCBArg and pMediaMime. 
 *	- Caller should not free the resources allocated for ppDRMMime. Resource will be freed internally by core.
 *
 */
typedef JC_RETCODE	(* DRM_COMPOSE_CB) 
									(MSG_HANDLE			hMsg, 
									 void				*pAppCBArg, 
									 MEDIA_MIME_INFO	*pMediaMime, 
									 MEDIA_MIME_INFO	**ppDRMMime) ;


#endif	// _MESSAGING_DATATYPES_H_


