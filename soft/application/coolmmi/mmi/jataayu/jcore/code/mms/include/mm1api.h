/**
 * @file  MM1Api.h
 * @brief Contains the prototypes for MM1 APIs
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
#ifndef _MM1_API_H
#define _MM1_API_H

#ifdef __cplusplus
extern "C"
{	
#endif

/* 	
	Initialize the MM1 layer and returns the necessary mm1 handle that
	should be used passed for the subsequent mm1 interface calls
*/
JC_RETCODE	jdi_MM1Initialize 
				(MM1_HANDLE *phMM1) ;

/* 
	Sets mms encoding type either text or binary, default is binary encoding
*/
JC_RETCODE	jdi_MM1SetEncodingType 
				(MM1_HANDLE hMM1, EMM1EncodingType eEncodingType) ;

JC_RETCODE	jdi_MM1SetMessageType 
				(MM1_HANDLE hMM1 , EMM1MessageType eMsgType);

JC_RETCODE	jdi_MM1SetToAddress 
				(MM1_HANDLE hMM1, JC_CHAR *pszToAddr, EMM1Charset eCharset) ;

JC_RETCODE	jdi_MM1SetCCAddress 
				(MM1_HANDLE hMM1, JC_CHAR *pszCcAddr, EMM1Charset eCharset) ;

JC_RETCODE	jdi_MM1SetBCCAddress 
				(MM1_HANDLE hMM1, JC_CHAR *pszBccAddr, EMM1Charset eCharset) ;

JC_RETCODE	jdi_MM1SetSubject 
				(MM1_HANDLE hMM1, JC_CHAR *pszSubject, EMM1Charset eCharset) ;

/*
	Sets the mms delivery report required option, the mmsc will send a delivery report notification
	to the sender when recipient successfully retrieved the message
*/
JC_RETCODE jdi_MM1SetDeliveryReport 
				(MM1_HANDLE hMM1, JC_BOOLEAN bFlag) ;

/*
	Sets the mms read-reply report required option, the recipient shall be notified about the 
	read-receipt requested by the sender when he has viewed the mms
*/
JC_RETCODE jdi_MM1SetReadReplyReport 
				(MM1_HANDLE hMM1, JC_BOOLEAN bFlag) ;

/*
	Sets the mms version supported by the mms client
*/
JC_RETCODE jdi_MM1SetVersion 
				(MM1_HANDLE hMM1, JC_UINT8 cMajorVersion, JC_UINT8 cMinorVersion) ;

/*
	Specifies where the mms message is located in the mmsc (MM1_HANDLE hMM1i.e., URL)
*/
JC_RETCODE jdi_MM1SetLocation 
				(MM1_HANDLE hMM1, JC_INT8 *pszLocation) ;

/*
	Sets the message id for eg: when we mmsclient sends M-MBox-Upload.req PDU
	this message id would have been obtained from the previous transaction
*/
JC_RETCODE jdi_MM1SetMessageId 
				(MM1_HANDLE hMM1, JC_INT8 *pszMsgId) ;

/*
	Specifies till what time (MM1_HANDLE hMM1date+time) the message is valid in the mmsc
	if the recipient has not downloaded this mms 
*/
JC_RETCODE jdi_MM1SetExpiryTime 
				(MM1_HANDLE hMM1, JC_UINT32 uiExpiryTime, JC_BOOLEAN bIsAbsolute) ;	// 3 2 06

/*
	Sets message class (MM1_HANDLE hMM1auto, personal, advertisement,Informational)
*/
JC_RETCODE jdi_MM1SetMessageClass 
				(MM1_HANDLE hMM1, EMM1MessageClass eMsgClass) ;

/*
	Sets the priority - high/medium/low
*/
JC_RETCODE jdi_MM1SetPriority 
				(MM1_HANDLE hMM1, EMM1MessagePriority eMsgPriority) ;

/*
	Sets the date and time of the submission
*/
JC_RETCODE jdi_MM1SetDate 
				(MM1_HANDLE hMM1, JC_UINT32 uiDateValue) ;

/*
	Indicates the desired delivery time (MM1_HANDLE hMM1earliest possible time)
*/
JC_RETCODE jdi_MM1SetDeliveryTime 
				(MM1_HANDLE hMM1, JC_UINT32 uiDeliveryTime, JC_BOOLEAN bIsAbsolute) ;	// 3 2 06

/*
	Sets the Transaction id
*/
JC_RETCODE jdi_MM1SetTransactionId 
				(MM1_HANDLE hMM1, JC_INT8 *pszTransactionID) ;

/*
	Sets the hide sender or visible sender option for the current message
*/
JC_RETCODE jdi_MM1SetSenderVisibility 
				(MM1_HANDLE hMM1, JC_BOOLEAN bFlag) ;

/*
	Sets the read report flag
*/
JC_RETCODE jdi_MM1SetReportAllowed 
				(MM1_HANDLE hMM1, JC_BOOLEAN bFlag) ;

/*
	Sets the Read Status (MM1_HANDLE hMM1Retrieved, Deferred, Rejected.. etc.)
*/
JC_RETCODE jdi_MM1SetReadStatus 
				(MM1_HANDLE hMM1, EMM1ReadStatus eStatus) ;

JC_RETCODE jdi_MM1SetMessageStatus 
				(MM1_HANDLE hMM1, EMM1Status eStatus) ;

JC_RETCODE jdi_MM1SetRetrieveStatus 
				(MM1_HANDLE hMM1,EMM1RetrieveStatus eMM1RetrieveStatus) ;

JC_RETCODE jdi_MM1SetRetrieveText 
				(MM1_HANDLE hMM1,JC_CHAR *psText,EMM1Charset eMM1Charset) ;

/*
	Sets the senders address
*/
JC_RETCODE	jdi_MM1SetFromAddress 
				(MM1_HANDLE hMM1, JC_BOOLEAN bInsertAddressToken, 
				JC_CHAR *pszFromAddr, EMM1Charset eCharset) ;

JC_RETCODE  jdi_MM1SetResponseStatus 
				(MM1_HANDLE hMM1, EMM1ResponseStatus eRespStatus) ;

JC_RETCODE jdi_MM1SetResponseText 
				(MM1_HANDLE hMM1, JC_CHAR *psResponseText, EMM1Charset eMM1Charset) ;

JC_RETCODE jdi_MM1SetMessageSize 
				(MM1_HANDLE hMM1, JC_UINT32 uiMsgSize) ;

/*
	Any headers the application willing to send with the mms
*/
JC_RETCODE jdi_MM1AddUserHeader 
				(MM1_HANDLE hMM1, JC_INT8 *pszName, JC_INT8 *pszValue) ;

JC_RETCODE jdi_MM1AddHeader 
				(MM1_HANDLE hMM1, const JC_INT8 *psHeaderString, const JC_INT8 *psValue, JC_INT32 iFlags) ;

JC_RETCODE jdi_MM1EncodeMultipartContent 
				(MM1_HANDLE hMM1, JC_INT8 **ppszContentType, 
				CONTENT_DATA *pContentEncode)  ;

// adds the content (MM1_HANDLE hMM1smil, wml, image, audio, text)
JC_RETCODE jdi_MM1AddContent 
				(MM1_HANDLE hMM1, MM1_CONTENT *pContent) ;

JC_RETCODE jdi_MM1GetEncodedStream 
				(MM1_HANDLE hMM1, CONTENT_DATA *pContentEncode) ;

JC_RETCODE jdi_MM1SetDecodeMode 
				(MM1_HANDLE hMM1, EMM1DecodeMode eDecodeMode,
				CREATE_STREAM_CB createCb, CONTENT_WRITE_CB writeCb, CONTENT_READ_CB readCb) ;

JC_RETCODE jdi_MM1Decode 
				(MM1_HANDLE hMM1, CONTENT_DATA *pDecode, EMM1ParseOption eParseOption, MSG_CREATE_STREAM_CB fpStreamCreateCB, JC_UINT32 *puiNumObjects) ;

JC_RETCODE jdi_MM1GetEncodingType 
				(MM1_HANDLE hMM1, EMM1EncodingType *peEncodingType) ;

JC_RETCODE jdi_MM1DecodeMultipartContent 
				(MM1_HANDLE hMM1, CONTENT_DATA *pEncapsulatedContent,
				JC_INT8* pszContentType, EMM1EncodingType eEncodingType,
				MSG_CREATE_STREAM_CB fpStreamCreateCB, JC_UINT32 *puiNumObjects) ;

/*
	Returns the message type of the mms buffer(MM1_HANDLE hMM1PDU)
*/
JC_RETCODE jdi_MM1GetMessageType
				(MM1_HANDLE hMM1, EMM1MessageType *peMsgType) ;

JC_RETCODE jdi_MM1GetSenderVisibility 
				(MM1_HANDLE hMM1, JC_BOOLEAN *bIsVisible) ;

JC_RETCODE jdi_MM1GetFromAddress 
				(MM1_HANDLE hMM1, MMS_ADDRESS_LIST **ppFromAddr) ;

JC_RETCODE jdi_MM1GetToAddress
				(MM1_HANDLE hMM1, MMS_ADDRESS_LIST **ppToAddr)  ;

JC_RETCODE jdi_MM1GetCCAddress 
				(MM1_HANDLE hMM1, MMS_ADDRESS_LIST **ppCCAddr) ;

JC_RETCODE jdi_MM1GetBCCAddress 
				(MM1_HANDLE hMM1, MMS_ADDRESS_LIST **ppBCCAddr) ;

JC_RETCODE jdi_MM1GetSubject 
				(MM1_HANDLE hMM1, JC_CHAR **ppSubject, EMM1Charset *peCharset) ;

/*
	Checks if the message contains the delivery report option set 
	Eg: the MMS notification can contain such option indicating the sender has requested
	Delivery report from the MMSC
*/
JC_RETCODE jdi_MM1GetDeliveryReport 
				(MM1_HANDLE hMM1, JC_BOOLEAN *pbFlag) ;

/*
	Checks if the message contains the Read-Reply report option set
	Eg: the MMS downloaded message can contain such option indicating the sender has
	requested Read-Reply report from the MMSC
*/
JC_RETCODE jdi_MM1GetReadReplyReport 
				(MM1_HANDLE hMM1, JC_BOOLEAN *pbFlag) ;

/*
	Gets the mms version of the current message
*/
JC_INT8 *jdi_MM1GetVersion 
				(MM1_HANDLE hMM1) ;

/*
	Gets the mms location (MM1_HANDLE hMM1URL) of the current message in the mmsc
*/
JC_INT8 *jdi_MM1GetLocation 
				(MM1_HANDLE hMM1) ;

JC_INT8 *jdi_MM1GetMessageId 
				(MM1_HANDLE hMM1) ;

JC_UINT32 jdi_MM1GetExpiryTime 
				(MM1_HANDLE hMM1, JC_BOOLEAN *bIsAbsolute) ;	//3 2 06

JC_UINT32 jdi_MM1GetDate 
				(MM1_HANDLE hMM1) ;

JC_UINT32 jdi_MM1GetDeliveryTime 
				(MM1_HANDLE hMM1, JC_BOOLEAN *bIsAbsolute) ;	// 3 2 06

JC_RETCODE jdi_MM1GetMessageClass 
				(MM1_HANDLE hMM1, EMM1MessageClass *eMsgClass)  ;

JC_RETCODE jdi_MM1GetPriority 
				(MM1_HANDLE hMM1, EMM1MessagePriority *eMsgPriority)  ;

/*
	For M-NotifyResp.ind PDU recipient specifies the mmsc whether the delivery report is allowed to
	to the sender whether in notifyResp.Ind or Ack.Ind PDU's
*/
JC_RETCODE	jdi_MM1GetReportAllowed 
				(MM1_HANDLE hMM1, JC_BOOLEAN *pbFlag) ;

JC_CHAR *	jdi_MM1GetResponseText 
				(MM1_HANDLE hMM1) ;

JC_RETCODE jdi_MM1GetMessageSize 
				(MM1_HANDLE hMM1, JC_UINT32 *puiMsgSize) ;

/*
	Sets a unique identifier for the current transaction
*/
JC_INT8 *jdi_MM1GetTransactionId 
				(MM1_HANDLE hMM1) ;

/*
	Set Message Status
*/
JC_RETCODE jdi_MM1GetMessageStatus 
				(MM1_HANDLE hMM1, EMM1Status *peStatus) ;

/*
	Gets the sent message response status from the send-conf message
*/
JC_RETCODE jdi_MM1GetResponseStatus 
				(MM1_HANDLE hMM1, EMM1ResponseStatus *peMM1RespStatus) ;

/*
	Gets the retrieval response status (MM1_HANDLE hMM1mmsc response status) from the 
	retrieve-conf message
*/
JC_RETCODE	jdi_MM1GetRetrieveStatus 
				(MM1_HANDLE hMM1, EMM1RetrieveStatus *eMM1RetrieveStatus) ;

JC_RETCODE	jdi_MM1GetRetrieveText 
				(MM1_HANDLE hMM1, JC_CHAR **ppsText, EMM1Charset *peMM1Charset) ;

/*
	Gets the recipient read status from the delivery-report indication message
*/
JC_RETCODE	jdi_MM1GetReadStatus 
				(MM1_HANDLE hMM1, EMM1ReadStatus *peReadStatus) ;

/*
	Retrieves if any user headers available in the message
*/
JC_INT8 *	jdi_MM1GetUserHeader 
				(MM1_HANDLE hMM1, JC_INT8 *pszName) ;

/*
	Sets the content-type and its parameters for the message
*/
JC_RETCODE	jdi_MM1SetContentType 
				(MM1_HANDLE hMM1, JC_INT8 *pContentType, JC_INT8 *pType, JC_INT8 *pStartID, 
					MM1_HDR_PARAM *pParams) ;

/*
	Return the content-type of the message body when mmbox view - conf PDU - message
*/
JC_INT8 *	jdi_MM1GetContentType 
				(MM1_HANDLE hMM1) ;

/*
	Retrieves all the header (MM1_HANDLE hMM1name-value) pairs of the current message
*/
MM1_HEADER *	jdi_MM1GetAllHeaders 
					(MM1_HANDLE hMM1) ;

/*
	Retrieves the content of the decoded message (MM1_HANDLE hMM1image, audio, text etc)
*/
JC_RETCODE	jdi_MM1GetContent 
				(MM1_HANDLE hMM1, MM1_CONTENT *pMM1Content, JC_UINT32 uiIndex) ;

/*
	This function shall reset all the mms options to their default values so that when the application 
	does not call the appropriate function for setting an option, the transaction options would still 
	contain the default values.
*/
JC_RETCODE	jdi_MM1Cleanup 
				(MM1_HANDLE hMM1, JC_BOOLEAN bKeepAllocatedMIME) ;

/* 
	DeInitialize the MM1 layer for the necessary mm1 handle MM1_HANDLE that was previously 
	initialized
*/
JC_RETCODE	jdi_MM1DeInitialize 
				(MM1_HANDLE hMM1) ;

#ifdef	MSG_MMS_V_1_1_ENABLE
/*******************************************************************************
		MMS Version 1.1 and above specific Functions
*******************************************************************************/

/*
	Sets the reply charging information
*/
JC_RETCODE jdi_MM1SetReplyCharging 
				(MM1_HANDLE hMM1, MM1_REPLY_CHARGING *pReplyCharging) ;

/*
	Sets the Address of the MMS Client that forwarded or originally sent the message.
*/
JC_RETCODE jdi_MM1SetPreviousSentBy 
				(MM1_HANDLE hMM1,JC_CHAR *pFromAddr, EMM1Charset peCharset) ;

/*
	Sets the Date and time of a forwarding or original send transaction of the message
*/
JC_RETCODE jdi_MM1SetPreviousSentDate 
				(MM1_HANDLE hMM1 , JC_INT64 iDate) ;

/*
	Decoding of MMS Message
*/
JC_RETCODE jdi_MM1GetReplyCharging 
				(MM1_HANDLE hMM1, MM1_REPLY_CHARGING *pReplyCharging) ;

JC_RETCODE	jdi_MM1GetPreviousSentBy 
				(MM1_HANDLE hMM1, JC_CHAR **pFromAddr, EMM1Charset *peCharset) ;

/*
	Gets the Date and time of a forwarding or original send transaction of the message
*/
JC_UINT32	jdi_MM1GetPreviousSentDate 
				(MM1_HANDLE hMM1)  ;
#endif
/*******************************************************************************/

#ifdef ENABLE_MMS_MMBOX
/*******************************************************************************
		MMS Version 1.2 and above specific Functions
*******************************************************************************/

/*
	Specifies what identifier/keyword should be assigned to the mms being stored in the mmbox 
	(MM1_HANDLE hMM1used for filtering mms msgs during viewing of the mm box list msgs)
*/
JC_RETCODE jdi_MM1SetMMFlags 
				(MM1_HANDLE hMM1, EMM1StoreFlags eOperation,JC_CHAR *pszToken, 
				EMM1Charset eCharset) ;

JC_INT8 *	jdi_MM1GetElementDescriptor 
				(MM1_HANDLE hMM1) ;

JC_RETCODE jdi_MM1SetMMStore 
				(MM1_HANDLE hMM1 , JC_BOOLEAN bFlag) ;

/*
	Specifies what state should be assigned to the mms being stored in the mmbox 
	the values are -> Draft, Sent, New, Retrieved, Forwarded. 
*/
JC_RETCODE jdi_MM1SetMMState 
				(MM1_HANDLE hMM1 , EMM1StoreStates eState) ;

/* 
	Sets the index value, indicating the index of the first MM of those selected to have information 
	returned in the response. If this is absent, the first item selected SHALL be returned.
	(MM1_HANDLE hMM1during view mmbox list Request PDU)
*/
JC_RETCODE jdi_MM1SetMMStart 
				(MM1_HANDLE hMM1, JC_UINT32 uiStart) ;

/*
	Sets the index value. indicating the maximum number of selected MMs whose information 
	SHALL be returned in the response. If this is absent, information elements from all remaining 
	MMs SHALL be returned. If this is zero then no MM-related information SHALL be returned.
*/
JC_RETCODE jdi_MM1SetMMLimit 
				(MM1_HANDLE hMM1, JC_UINT32 uiLimit) ;

/*
	Returns whether the sender allows this message to be forwarded 
*/
JC_RETCODE jdi_MM1SetDistributionIndicator 
				(MM1_HANDLE hMM1, JC_BOOLEAN bFlag) ;

/*
	Sets the quota option during view-req pdu
*/
JC_RETCODE jdi_MM1SetMMQuotas 
				(MM1_HANDLE hMM1, JC_BOOLEAN bFlag) ;

/*
	Specifies the list of fields that should be sent by the mmsc for this request. Useful during View 
	mmbox list request as the client can request for certain number of fields only. The default 
	behaviour will be to get  the subset of the fields available in notification message
*/
JC_RETCODE jdi_MM1SetMMAttributes 
				(MM1_HANDLE hMM1, EAttributeType eAttribList) ;

/*
	Sets the total option during veiw-req pdu
*/
JC_RETCODE jdi_MM1SetMMTotal 
				(MM1_HANDLE hMM1 , JC_BOOLEAN bFlag) ;

JC_RETCODE jdi_MM1SetStoreStatus 
				(MM1_HANDLE hMM1, EMM1StoreStatus eMM1StoreStatus) ;

JC_RETCODE jdi_MM1SetStoreStatusText 
				(MM1_HANDLE hMM1, JC_CHAR *pStoreStatus, EMM1Charset eMM1Charset) ;

JC_RETCODE jdi_MM1SetMMBoxStored 
				(MM1_HANDLE hMM1, JC_BOOLEAN bFlag) ;

/*
	Checks for the mm box store status when sender has sent a message with store option the 
	response shall contain the status of the store operation. This function shall return that status 
	from the send-conf message
*/
JC_RETCODE	jdi_MM1GetStoreStatus 
				(MM1_HANDLE hMM1, EMM1StoreStatus *peMM1StoreStatus) ;

/*
	This routine returns whether the MM was stored in the user’s MMBox
	eg: this value will be available in the notification indication
*/
JC_RETCODE	jdi_MM1GetMMStore 
				(MM1_HANDLE hMM1, JC_BOOLEAN *pbFlag) ;

/*
	Returns the state of the MM as one of the following: Draft, Sent, New, Retrieved, Forwarded
*/
JC_RETCODE	jdi_MM1GetMMState 
				(MM1_HANDLE hMM1, EMM1StoreStates *peMM1StoreStates) ;

/*
	Retrieves the list of identifier/keywords associated with the mms being stored in the mmbox 
	(MM1_HANDLE hMM1used for filtering mms msgs during viewing of the mm box list msgs)
*/
JC_RETCODE	jdi_MM1GetMMFlags 
				(MM1_HANDLE hMM1, JC_CHAR **ppszToken, EMM1Charset *eCharset) ;

JC_RETCODE jdi_MM1GetMMCount 
				(MM1_HANDLE hMM1, JC_UINT32 *puiMsgCount) ;

JC_RETCODE	jdi_MM1GetDistributionIndicator 
				(MM1_HANDLE hMM1, JC_BOOLEAN *pbFlag)  ;

/*
	Returns the index value, indicating the index of the first MM of those selected to have information 
	returned in the response.
*/
JC_RETCODE	jdi_MM1GetMMStart 
				(MM1_HANDLE hMM1, JC_UINT32 *puiStart) ;

/*
	Returns a index value, indicating the number of MM selected that Client requested to be displayed.
*/
JC_RETCODE	jdi_MM1GetMMLimit 
				(MM1_HANDLE hMM1, JC_UINT32 *puiLimit) ;

/*
	Retrieves the list of fields that are requested during View mmbox list request as the client can 
	request for certain number of fields only. The default behavior will be to get  the subset of the
	fields available in notification message
*/
JC_RETCODE	jdi_MM1GetMMAttributes 
				(MM1_HANDLE hMM1, EAttributeType *peAttributeType) ;

/*
	Returns the total number of messages.. (MM1_HANDLE hMM1what is the bytes ??)
*/
JC_RETCODE	jdi_MM1GetMMTotal 
				(MM1_HANDLE hMM1, JC_UINT32 *puiTotal) ;

JC_RETCODE	jdi_MM1GetMMQuotas 
				(MM1_HANDLE hMM1, JC_UINT32 *puiQuotas) ;

JC_RETCODE	jdi_MM1GetStoreStatusText 
				(MM1_HANDLE hMM1, JC_CHAR **ppsStoreStatusText, EMM1Charset *peMM1Charset) ;

JC_RETCODE	jdi_MM1GetMMBoxStored 
				(MM1_HANDLE hMM1, JC_BOOLEAN *pbFlag) ;

/*******************************************************************************/
#endif	//ENABLE_MMS_MMBOX

#ifdef __cplusplus
}	
#endif

#endif
