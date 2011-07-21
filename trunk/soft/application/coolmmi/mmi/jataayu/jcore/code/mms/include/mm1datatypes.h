/**
 * @file    MM1DataTypes.h
 * @brief   Declares the MM1 specific data types.
 */
 /***************************************************************************
 *
 * File Name : MM1DataTypes.h
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
#ifndef _MM1_DATATYPE_H
#define _MM1_DATATYPE_H

typedef void * 		MM1_HANDLE ; /**< MM1 Handle */

/**
 * @brief Enumeration of PDU Types
 *
 * Specifies the supported PDU types
 */
typedef enum  _mm1_message_type
{
	E_MMS_SEND_REQUEST_VALUE = 0x00,/**< Specifies the message pdu as send request*/
	E_MMS_SEND_CONFIRMATION_VALUE,/**< Specifies the message pdu as response to the send request*/
	E_MMS_NOTIFICATION_INDICATION_VALUE,/**< Specifies that message pdu is indication of a new message (from the server)*/
	E_MMS_NOTIFICATION_RESPONSE_INDICATION_VALUE,/** Specifies that message pdu is a response to the new message indication (to the server)*/
	E_MMS_RETRIEVE_CONFIRMATION_VALUE,/**< Specifies the message pdu as received message*/
	E_MMS_ACKNOWLEDGE_INDICATION_VALUE,/**< Specifies the message pdu as acknowledgement to the server after retrieving message*/
	E_MMS_DELIVERY_INDICATION_VALUE,/**< Specifies the message pdu as delivery report from the server*/
	/* MMS Stack 1.1 message type*/
	E_MMS_READ_REC_IND_VALUE,/**< Specifies the message pdu as read report to the sender (For sending Read Report)*/
	E_MMS_READ_ORG_IND_VALUE,/**< Specifies the message pdu as read report from the recipient (received from the server)*/
	E_MMS_FORWARD_REQ_VALUE,/**< Specifies the message pdu as a request to forward a message from the MBox (Only for MMS with MBox enabled)*/
	E_MMS_FORWARD_CONFIRM_VALUE,/**< Specifies the message pdu as response to the MBox forward request (Only for MMS with MBox enabled)*/
	/* MMS Stack 1.2 message types*/
	E_MMBOX_STORE_REQ_VALUE,/**<Specifies the message pdu as request to store the new message or update an existing message in the MBox (Only for MMS with MBox enabled)*/
	E_MMBOX_STORE_CONF_VALUE,/** Specifies the message pdu as response to the MBox store or update request (Only for MMS with MBox enabled)*/
	E_MMBOX_VIEW_REQ_VALUE,/**< Specifies the message pdu as request to retrieve messages) from the MBox (Only for MMS with MBox enabled)*/
	E_MMBOX_VIEW_CONF_VALUE,/**< Specifies the message pdu as response to MBox view request (Only for MMS with MBox enabled)*/
	E_MMBOX_UPLOAD_REQ_VALUE,/**< Specifies the message pdu as request to upload a composed message to the MBox (Only for MMS with MBox enabled)*/
	E_MMBOX_UPLOAD_CONF_VALUE,/**< Specifies the message pdu as response to the MBox upload request (Only for MMS with MBox enabled)*/
	E_MMBOX_DELETE_REQ_VALUE,/**< Specifies the message pdu as request to delete messages) from the MBox (Only for MMS with MBox enabled)*/
	E_MMBOX_DELETE_CONF_VALUE,/**< Specifies the message pdu as response to the MBox message delete request (Only for MMS with MBox enabled)*/
	E_MMBOX_DESCR_VALUE /**< Specifies the message pdu as MBox Descriptor*/
} EMM1MessageType ;


/**
 * @brief Enumeration of Charsets
 *
 * Specifies the supported Charsets
 *
 * Note: Currently Not Used
 */
typedef enum _mm1_charset
{
	E_NONE = 0x00,/**< Specifies no charset*/
	E_ISO_8859_1,/**< Specifies charset as ISO-8859-1*/
	E_UTF_8,/**< Specifies charset as UTF-8*/
	E_UTF_16,/**< Specifies charset as UTF-16*/
	E_UCS_2/**< Specifies charset as UCS2*/
} EMM1Charset ;

/**
 * @brief Enumeration of Header Flag (Type)
 *
 * Specifies the type of header, used while adding headers
 *
 */
typedef enum _MMSFlags
{
	ADD_TOKEN = 0x01, /**< Specifies header as Add Token (MMS 1.2)*/
	REMOVE_TOKEN, /**< Specifies header type as Remove Token (MMS 1.2)*/
	FILTER_TOKEN, /**< Specifies header type as Filter Token (MMS 1.2)*/
	INSERT_ADDRESS_TOKEN, /**< Specifies header type as Insert Address indication*/
	ADDRESS_TYPE, /**< Specifies header as Address Type (format), To, CC, BCC, FROM*/
	DATE_TYPE, /**< Specifies header as Date Type (format), Relative Value*/
	DATE_ABSOLUTE /**< Specifies header as Date Type (format), Absolute Value*/
} MMSFlags ;

/**
 * @brief Enumeration of Reply Charging Types
 *
 * Specifies the supported reply charging types
 *
 */
typedef enum _mm1_reply_charging_types
{
	E_REPLY_CHARGING_REQUESTED=0x00,/**< Specifies that the sender is willing to pay for the Reply-MM of the recipients)*/
	E_REPLY_CHARGING_REQUESTED_TEXT_ONLY,/**< Specifies that the sender is willing to pay for the Reply-MM of the recipients) provided that the MM contains only text media.*/
	E_REPLY_CHARGING_ACCEPTED,/**< Specifies that the sender is willing to pay for the Reply-MM*/
	E_REPLY_CHARGING_ACCEPTED_TEXT_ONLY,/**< Specifies that the sender is willing to pay for the Reply-MM provided that the MM has only text media.*/
	E_REPLY_CHARGING_NOT_SUPPORTED	/**< To be used for unsetting replycharging. This value should be set if this message is a reply to a message with reply charging*/
} EMM1ReplyCharging ;


/**
 * @brief Enumeration of Message class values
 *
 * Specifies the supported message class types
 *
 */
typedef enum _mm1_message_class
{
	E_PERSONAL_MESSAGE_CLASS = 0x00,/**< Specifies the message class as <I>Personal</I>*/
	E_ADVERTISEMENT_MESSAGE_CLASS,/**< Specifies the message class as <I>Advertisement</I>*/
	E_INFORMATIONAL_MESSAGE_CLASS,/**< Specifies the message class as <I>Informational</I>*/
	E_AUTO_MESSAGE_CLASS/**< Specifies the message class as <I>Automatic</I>*/
} EMM1MessageClass ;


/**
 * @brief Enumeration of Message priority values
 *
 * Specifies the supported message priority types
 *
 */
typedef enum _mm1_message_priority
{
	E_LOW_MESSAGE_PRIORITY = 0x00, /**< Specifies as low priority message*/
	E_NORMAL_MESSAGE_PRIORITY,/**< Specifies as normal priority message*/
	E_HIGH_MESSAGE_PRIORITY/**< Specifies as high priority message*/
} EMM1MessagePriority ;


/**
 * @brief Enumeration of read report status
 *
 * Specifies the read report status types
 *
 */
typedef enum _mm1_read_status
{
	READ = 0x00, /**< Specifies as message read by the recipient*/
	DELETED_WITHOUT_READ /**< Specifies as message deleted by the recipient without reading*/

}EMM1ReadStatus ;


/**
 * @brief Enumeration of status 
 *
 * Specifies the MMS Status types (X-Mms-Status). Used with M-NotifyResp.ind and M-Delivery.ind PDU's
 *
 */
typedef enum _mm1_status
{
	E_EXPIRED_STATUS = 0x00, /**< Specifies that the recipient did not retrieve the message before expiration.*/
	E_RETRIEVED_STATUS, /**< Specifies that the message has been successfully retrieved by the recipient*/
	E_REJECTED_STATUS, /**< Specifies that the message has been rejected by the recipient*/
	E_DEFERRED_STATUS, /**< Specifies that the message client decided to retrieve the message later */
	E_UNRECOGNISED_STATUS, /**< Specifies that message <B>PDU</B> is unrecognized*/
	E_INDETERMINATE_STATUS, /**< Specifies that it's not able to determine whether the message reached the recipient.*/
	E_FORWARDED_STATUS, /**< Specifies that the recipient without viewing has forwarded the message. Specific to MBox.*/
	E_UNREACHABLE_STATUS /**< Specifies that the recipient was not able to reach due to network or other failures*/
} EMM1Status ;


/**
 * @brief Enumeration of MBox Folders
 *
 * Specifies the MBox folders types (X-Mms-MM-State).
 *
 */
typedef enum _mm1_store_states
{
		E_DRAFT = 0x00, /**< Specifies MBox <I>Draft</I> folder*/
		E_SENT, /**< Specifies MBox <I>Sent</I> folder*/
		E_NEW, /**< Specifies MBox <I>New</I> folder*/
		E_RETRIEVED, /**< Specifies MBox <I>Retrieved</I> folder*/
		E_FORWARDED /**< Specifies MBox <I>Forwarded</I> folder*/
} EMM1StoreStates ;


/**
 * @brief Enumeration of MBox flags
 *
 * Specifies the MBox flag token types (X-Mms-MM-Flags).
 *
 */
typedef enum _mm1_store_flags
{
		E_ADD = 0x01, /**< Specifies the keyword to be added to MBox */
		E_REMOVE, /**< Specifies the keyword to be deleted from MBox */
		E_FILTER /**< Specifies the keyword to be used for filtering. Should be used only with <B>E_MSG_TYPE_MMBOX_VIEW</B> */
} EMM1StoreFlags ;


/**
 * @brief Enumeration of MBox Attributes
 *
 * Specifies MBox attribute types (X-Mms-Attributes). Used with M-Mbox-View.req, to indicate
 * the attributes to be returned (headers)
 *
 */
typedef enum _mm1_attrib_type_
{
		E_BCC_ATTRIB=0x01, /**< Specifies <I>BCC</I> attribute*/
		E_CC_ATTRIB, /**< Specifies <I>CC</I> attribute*/
		E_CONTENT_LOCATION_ATTRIB, /**< Specifies <I>Content Location</I> attribute*/
		E_CONTENT_TYPE_ATTRIB, /**< Specifies <I>Content Type</I> attribute*/
		E_DATE_ATTRIB, /**< Specifies <I>Date</I> attribute*/
		E_DELIVERYREPORT_ATTRIB, /**< Specifies <I>Delivery Report</I> attribute*/
		E_DELIVERYTIME_ATTRIB, /**< Specifies <I>Delivery Time</I> attribute*/
		E_EXPIRY_ATTRIB, /**< Specifies <I>Expiry</I> attribute*/
		E_FROM_ATTRIB, /**< Specifies <I>From</I> attribute*/
		E_MESSAGECLASS_ATTRIB, /**< Specifies <I>Message Class</I> attribute*/
		E_MESSAGEID_ATTRIB = 0x0C, /**< Specifies <I>Message Id</I> attribute*/
		E_MESSAGESIZE_ATTRIB=0x0E, /**< Specifies <I>Message Size</I> attribute*/
		E_PRIORITY_ATTRIB, /**< Specifies <I>Priority</I> attribute*/
		E_READREPORT_ATTRIB=0x10, /**< Specifies <I>Read Report</I> attribute*/
		E_SUBJECT_ATTRIB=0x16, /**< Specifies <I>Subject</I> attribute*/
		E_TO_ATTRIB, /**< Specifies <I>To</I> attribute*/
		E_REPLYCHARGING_ATTRIB=0x1c, /**< Specifies <I>Reply Charging</I> attribute*/
		E_REPLYCHARGINDEADLINE_ATTRIB, /**< Specifies <I>Reply Charging Deadline</I> attribute*/
		E_REPLYCHARGINGID_ATTRIB=0x1e, /**< Specifies <I>Reply Charging Id</I> attribute*/
		E_REPLYCHARGINGSIZE_ATTRIB, /**< Specifies <I>Reply Charging Size</I> attribute*/
		E_PREVIOUSLYSENTBY_ATTRIB, /**< Specifies <I>Previously Sent By</I> attribute*/
		E_PREVIOUSLYSENTDATE_ATTRIB, /**< Specifies <I>Previously Sent Date</I> attribute*/
		E_CONTENT = 0x2E /**< Specifies <I>Content</I> attribute*/

} EAttributeType ;


/**
 * @brief Enumeration of response status
 *
 * Specifies response status list (X-Mms-Response-Status).
 *
 */
typedef enum _mm1_response_status 
{
	E_RESP_OK =0, /**< Specifies status as <I>Ok</I> (success) */
	E_RESP_ERR_UNSPECIFIED, /**< Specifies status as <I>Error-unspecified</I> */
	E_RESP_ERR_SERVICE_DENIED, /**< Specifies status as <I>Error-service-denied</I> */
	E_RESP_ERR_MSG_FORMAT_CORRUPT, /**< Specifies status as <I>Error-message-format-corrupt</I> */
	E_RESP_ERR_SENDING_ADDR_UNRESOLVED, /**< Specifies status as <I>Error-sending-address-unresolved</I> */
	E_RESP_ERR_MESG_NOT_FOUND, /**< Specifies status as <I>Error-message-not-found</I> */
	E_RESP_ERR_NETWORK_PROBLEM, /**< Specifies status as <I>Error-network-problem</I> */
	E_RESP_ERR_CONTENT_NOT_ACCEPTED, /**< Specifies status as <I>Error-content-not-accepted</I> */
	E_RESP_ERR_UNSUPPORTED_MSG, /**< Specifies status as <I>Error-unsupported-message</I> */
	E_RESP_TRANSIENT_FAILURE =0x40, /**< Specifies status as <I>Error-transient-failure</I> */
	E_RESP_TRANSIENT_SENDING_ADDR_UNRESOLVED, /**< Specifies status as <I>Error-transient-sending-address-unresolved</I> */
	E_RESP_TRANSIENT_MSG_NOT_FOUND, /**< Specifies status as <I>Error-transient-message-not-found</I> */
	E_RESP_TRANSIENT_NETWORK_PROBLEM, /**< Specifies status as <I>Error-transient-network-problem</I> */
	E_RESP_TRANSIENT_PARTIAL_SUCCESS, /**< Specifies status as <I>Error-transient-partial-success</I> */
	E_RESP_PERMANENT_FAILURE =0x60, /**< Specifies status as <I>Error-permanent-failure</I> */
	E_RESP_PERMANENT_SERVICE_DENIED, /**< Specifies status as <I>Error-permanent-service-denied</I> */
	E_RESP_PERMANENT_MSG_FORMAT_CORRUPT, /**< Specifies status as <I>Error-permanent-message-formatcorrupt</I> */
	E_RESP_PERMANENT_SENDING_ADDRESS_UNRESOLVED, /**< Specifies status as <I>Error-permanent-sending-address-unresolved</I> */
	E_RESP_PERMANENT_MSG_NOT_FOUND, /**< Specifies status as <I>Error-permanent-message-not-found</I> */
	E_RESP_PERMANENT_CONTENT_NOT_ACCEPTED, /**< Specifies status as <I>Error-permanent-content-not-accepted</I> */
	E_RESP_PERMANENT_REPLY_CHARGING_LIMITATIONS_NOT_SET, /**< Specifies status as <I>Error-permanent-reply-charginglimitations-not-met</I> */
	E_RESP_PERMANENT_REPLY_CHARGING_NOT_ACCEPTED, /**< Specifies status as <I>Error-permanent-reply-chargingrequest-not-accepted</I> */
	E_RESP_PERMANENT_REPLY_CHARGING_FORWARD_DENIED, /**< Specifies status as <I>Error-permanent-reply-chargingforwarding-denied</I> */
	E_RESP_PERMANET_REPLY_CHARGING_NOT_SUPPORTED, /**< Specifies status as <I>Error-permanent-reply-charging-not-supported</I> */
	E_RESP_PERMANENT_ADDRESS_HIDING_NOT_SUPPORTED /**< Specifies status as <I>Error-permanent-address-hiding-not-supported</I> */

} EMM1ResponseStatus ;


/**
 * @brief Enumeration of MBox store status
 *
 * Specifies store status list (X-Mms-Store-Status).
 *
 */
typedef enum _mm1_store_status
{
	E_STORE_SUCESS = 0x00, /**< Specifies status as <I>Success</I> */
	E_STORE_TRANSIENT_FAILURE = 0x40, /**< Specifies status as <I>Error-transient-failure</I> */
	E_STORE_TRANSIENT_NETWORK_PROBLEM, /**< Specifies status as <I>Error-transient-network-problem</I> */
	E_STORE_PERMANENT_FAILURE = 0x60, /**< Specifies status as <I>Error-permanent-failure</I> */
	E_STORE_PERMANENT_SERVICE_DENIED, /**< Specifies status as <I>Error-permanent-service-denied</I> */
	E_STORE_PERMANENT_MSG_FORAMT_CORRUPT, /**< Specifies status as <I>Error-permanent-message-format-corrupt</I> */
	E_STORE_PERMANENT_MSG_NOT_FOUND, /**< Specifies status as <I>Error-permanent-message-not-found</I> */
	E_STORE_PERMANENT_MMBOX_FULL /**< Specifies status as <I>Error-permanent-mmbox-full</I> */

} EMM1StoreStatus ;


/**
 * @brief Enumeration of MM retrieve status
 *
 * Specifies retrieve status list (X-Mms-Retrieve-Status).
 *
 */
typedef enum _mm1_retrieve_status
{
	E_RETRIEVE_OK = 0x00, /**< Specifies status as <I>Ok</I> */
	E_RETRIEVE_TRANSIENT_FAILURE = 0x40, /**< Specifies status as <I>Error-transient-failure</I> */
	E_RETRIEVE_TRANSIENT_MSG_NOT_FOUND, /**< Specifies status as <I>Error-transient-message-not-found</I> */
	E_RETRIEVE_TRANSIENT_NETWORK_PROBLEM, /**< Specifies status as <I>Error-transient-network-problem</I> */
	E_RETRIEVE_PERMANENT_FAILURE =0x60, /**< Specifies status as <I>Error-permanent-failure</I> */
	E_RETRIEVE_PERMANENT_SERVICE_DENIED, /**< Specifies status as <I>Error-permanent-service-denied</I> */
	E_RETRIEVE_PERMANENT_MESSAGE_NOT_FOUND, /**< Specifies status as <I>Error-permanent-message-not-found</I> */
	E_RETRIEVE_PERMANENT_CONTENT_UNSUPPORTED /**< Specifies status as <I>Error-permanent-content-unsupported</I> */

} EMM1RetrieveStatus ;


/**
 * @brief Enumeration of Address types
 *
 * Specifies types of address formats
 *
 */
typedef enum _MMSAddressTypes
{
	E_PLMN = 0x00, /**< Specifies address type as <I>Mobile Number</I> */
	E_IPV4, /**< Specifies address type as <I>IP V4</I> */
	E_IPV6, /**< Specifies address type as <I>IP V6</I> */
	E_MAIL /**< Specifies address type as <I>Email Address</I> */

} MMSAddressTypes ;


/**
 * @brief Enumeration of MIME Content format
 *
 */
typedef enum _MIME_Content_Format
{
	E_MIME_FORMAT_TEXT = 1, /**< Specifies MIME Contents format as <I>Text</I> */
	E_MIME_FORMAT_BINARY /**< Specifies MIME Contents format as <I>Binary</I> */

} EMIMEContentFormat ;

/**
 * @brief Enumeration of Message Formats
 *
 * Specifies the message formats
 */
typedef enum _mm1_encoding_types
{
	E_ENCODING_MMS_BINARY = 0, /**< Specifies message format as Binary (MMS) */
	E_ENCODING_MMS_TEXT, /**< Specifies message format as Text (MMS) */
	E_ENCODING_EMAIL_TEXT /**< Specifies message format as Binary (Email) */
	
} EMM1EncodingType ;

/**
 * @brief Enumeration of parsing options
 *
 * Specifies the parsing options to be used.
 */
typedef enum _mm1_parse_option
{
	E_PARSE_HEADERS_MIMEPARTS = 0, /**< Specifies to parse both header and mime parts */
	E_PARSE_HEADERS_ONLY, /**< Specifies to parse only header */
	E_PARSE_MIMEPARTS_ONLY /**< Specifies to only mime parts */
	
} EMM1ParseOption ;


/**
 * @brief Defines the reply charging information
 *
 * Specifies the reply charging header information.
 */
typedef struct _mm1_reply_charging
{
	EMM1ReplyCharging 	eReplyCharging ; /**< Specifies reply charging type */
	JC_UINT32			uiRelativeTime ; /**< Specifies reply charging validity period */
	JC_UINT32			uiSize ; /**< Specifies reply charging accepted message size limit */
	JC_INT8				*pszReplyId ; /**< Specifies the message-id of original message */

} MM1_REPLY_CHARGING ;


/**
 * @brief Prototype for callback function to create file stream
 *
 * This function will be implemented by the MM1 caller. MM1 will invoke this function to create file stream.
 *
 * Note: Currently Not Supported
 */
typedef JC_RETCODE (* CREATE_STREAM_CB) 
					(void **ppStream, JC_INT32 uiTotalSize, JC_INT8 *pszObjectName, JC_INT8 *pszMimeType) ;

/**
 * @brief Prototype for callback function to close file stream
 *
 * This function will be implemented by the MM1 caller. MM1 will invoke this function to close file stream.
 *
 * Note: Currently Not Supported
 */
typedef JC_RETCODE (* CLOSE_STREAM_CB) 
					(void *pStream) ;


/**
 * @brief Prototype for callback function to read data from file stream
 *
 * This function will be implemented by the MM1 caller. MM1 will invoke this function to read data from file stream.
 *
 */
typedef JC_RETCODE (* CONTENT_READ_CB) 
					(void *pStream, JC_INT32 uiOffset, JC_UINT32 uiSize, JC_UINT8 *pBuffer, JC_UINT32 *uiSizeRead) ;


/**
 * @brief Prototype for callback function to writing data to file stream
 *
 * This function will be implemented by the MM1 caller. MM1 will invoke this function to write data to file stream.
 *
 */
typedef JC_RETCODE (* CONTENT_WRITE_CB) 
					(void *pStream, JC_UINT8 *pBuffer, JC_UINT32 uiSize) ;


/**
 * @brief Defines content data structure
 *
 * Data is passed from and to the caller using this structure.
 */
typedef struct _content_data
{
	JC_UINT32			uiTotalSize ; /**< Specifies the total size of buffer */
	JC_UINT32			uiCurSize ; /**< Specifies the current size of buffer. Note: If bIsStream = E_FALSE, must set same as uiTotalSize */
	JC_BOOLEAN			bIsStream ; /**< Specifies whether the input is in File or Inline Buffer */

	/**** Buffer specific data structures ****/
	JC_UINT8			*pBuffer ; /**< Specifies the Inline Buffer */
	JC_BOOLEAN			eCopyMemory ; /**< Specifies whether the called function needs to make a copy before using. Note: Currently not supported */

	/**** File stream specific data structures ****/
	void				*pStream ; /**< Specifies the user define file stream information*/
	JC_CHAR				*pFileName ; /**< Specifies the file name (including complete path) */
	JC_UINT32			uiStartOffset ; /**< Specifies the starting offset. Only for file stream */
	CONTENT_READ_CB		cbReadCallback ; /**< Specifies the file stream read callback function pointer */
	CONTENT_WRITE_CB	cbWriteCallback ; /**< Specifies the file stream write callback function pointer */

} CONTENT_DATA ;

/**
 * @brief Prototype for callback function to create file stream 
 *
 * This function will be implemented by the MM1 caller. MM1 will invoke this function to create and write data to file stream.
 *
 */
typedef JC_RETCODE (* MSG_CREATE_STREAM_CB)
					(CONTENT_DATA *pInputData, void *pBuffer, JC_CHAR *pFileName, JC_UINT32 uiReadSize, JC_BOOLEAN bIsAppend) ;

/**
 * @brief Defines decode mode
 *
 * NOT USED
 */
typedef enum _decode_mode
{
	E_CONTENT_INLINE,
	E_CONTENT_STREAM,
	E_CONTENT_USE_STREAM,
	E_CONTENT_CREATE_STREAM
} EMM1DecodeMode ;


/**
 * @brief Defines Stream Information
 *
 * NOT USED
 */
typedef struct _mm1_use_stream
{
	void 				*pStream ;/**< Specifies the stream data */
	JC_UINT32			uiOffset ;/**< Specifies the offset */
	JC_UINT32			uiSize ;/**< Specifies the stream size*/
} MM1_CONTENT_USE_STREAM ;


/**
 * @brief Defines Information for creating stream
 *
 * NOT USED
 */
typedef struct _mm1_new_stream
{
	void 				*pStream ; /**< Specifies the stream data */

} MM1_CONTENT_NEW_STREAM ;


/**
 * @brief Defines MM1 Content (mime part).
 *
 * Specifies MM1 content structure details.
 */
typedef struct _mm1_content
{
	JC_INT8 			*pszMimeType ; /**< Specifies the Content-Type of the mime part */
	JC_INT8             *pszNameParam ; /**< Specifies the Name Param for mime part */
	const JC_INT8       *pszContentID ; /**< Specifies the Content-ID for mime part*/
	const JC_INT8		*pszObjectName ; /**< Specifies the Content-Location for mime part */
	EMM1Charset			eCharset ; /**< Specifies the Charset for the mime part. Note: Currently not supported */
	const JC_INT8		*pszDRMSepDelTime ;/**< Specifies DRM Separate Delivery Time - X-OMA-DRM-SEPARATE-DELIVERY header value in Seconds*/
	const JC_INT8		*pszDisposition ; /**< Specifies the Content-Disposition for mime part */

	JC_BOOLEAN			bIsAttachment ; /**< Specifies the Content-Disposition, i.e., whether mime part is attachment or inline */
	EMIMEContentFormat	eMIMEFormat ; /**< Specifies the mime part format (text or binary) */

	CONTENT_DATA		MimeData ;	/**< Specifies the mime part's content data (buffer) */
	JC_BOOLEAN			bIsMemoryAllocated ; /**< Specifies whether memory is allocated for this mime-part */
} MM1_CONTENT ;


/**
 * @brief Defines MM1 Header
 *
 * Specifies mime header structure detail.
 */
typedef struct mm1_header
{
	JC_INT8 			*pszName ; /**< Specifies the header name */
	JC_CHAR				*pszValue ; /**< Specifies the header value */
	struct mm1_header	*next ; /**< Pointer to next header node */

} MM1_HEADER ;


/**
 * @brief Defines Message Content Type Parameter
 *
 * Specifies Message content type header parameter list
 */
typedef struct mm1_hdr_param
{
	JC_INT8 				*pszName ; /**< Specifies the param name */
	JC_INT8					*pszValue ; /**< Specifies the param value */
	struct mm1_hdr_param	*pNext ; /**< Specifies the next parameter node */
} MM1_HDR_PARAM ;


/**
 * @brief Defines the address list
 *
 * Specifies address (to, cc, bcc) value list
 */
typedef struct mms_address_list
{
	MMSAddressTypes		eAddressType ; /**< Specifies the address format type */
	JC_CHAR				*pAddress ; /**< Points to the Address */
	EMM1Charset			eMMM1Charset ; /**< Specified the address charset */
	struct mms_address_list *pNext ; /**< Points to next address node */

}MMS_ADDRESS_LIST ;

#endif
