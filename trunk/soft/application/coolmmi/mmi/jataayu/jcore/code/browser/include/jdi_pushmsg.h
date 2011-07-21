/***************************************************************************
 * $Id: jdi_pushmsg.h,v 1.30 2009/03/04 13:04:13 kavithadevhtmlbrow Exp $
 * $Revision: 1.30 $
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
 * $Log: jdi_pushmsg.h,v $
 * Revision 1.30  2009/03/04 13:04:13  kavithadevhtmlbrow
 * Doxygen Comments Updated
 *
 * Revision 1.29  2007/05/31 12:05:22  shreyasdevhtmlbrow
 * Doxygen comment updated
 *
 * Revision 1.28  2007/05/30 14:39:04  shreyasdevhtmlbrow
 * Updated the doxygen comments.
 *
 *  
 ***************************************************************************/

/**
 * @page push Push Handling
 * Push messages are those content which are not requested by the browser, while it is pushed either by the mobile operator or by some service provider. 
 * Transport sends the push message to the relevant application based on the content-type. If there is no registered application for the push message, 
 * then it shall be pushed to the Browser application, as it is the discovery application in the mobile phone. 
 * <BR><BR>
 * @image html push_manager.jpg
 *
 * Browser core can handle the following content-type
 * @li Service Indication (SI) (text/vnd.wap.si or application/vnd.wap.sic)
 * @li Service Loading (SL) (text/vnd.wap.sl or application/vnd.wap.slc)
 * @li Cache Operation (CO) (text/vnd.wap.co or application/vnd.wap.coc) 
 * 
 * Cache Operation is internally processed by the browser core, where as SI and SL are passed to 
 * the application. The application needs to be integrated with the push module to handle the SI, SL
 * messages.
 *
 * Following are the API's that are exposed for handling push message by the application:
 * @li #jdi_PushMsgInitialize
 * @li #jdi_PushMsgParse
 * @li #jdi_PushMsgAdd
 * @li #jdi_PushMsgDelete
 * @li #jdi_PushMsgDeleteAll
 * @li #jdi_PushMsgDeleteExpired
 * @li #jdi_PushMsgStatistics
 * @li #jdi_PushMsgSetReadStatus
 * @li #jdi_PushMsgGetList
 * @li #jdi_PushMsgSort
 * @li #jdi_PushMsgFreeInfo
 * @li #jdi_PushMsgDeInitialize
 *
 * All the push messages are routed to the application through the callback routine (#CB_BE_GENERAL_CALLBACK) which is 
 * registed during the initialize time. The callback type shall be #E_BE_PUSH_LISTENER and the information
 * passed #ST_BE_PUSH_IND.
 *
 * Browser engine also exposes additional API #jdi_BEInvalidateCache, for handling cache operation message, which will invalidate
 * the cache content.
 */

/**
 * @file jdi_pushmsg.h
 * @ingroup PushHandler
 * @brief Defines the exposed enumerations, structures and functions that are used to parse and
 *		  store the SI/SL push messages.
 * 
 * This file defines all the enumerations, structures and functions that the application need
 * to use in order to parse a SI/SL message and also to store & retrieve the push messages.
 */

#ifndef JDI_PUSHMSG_H
#define JDI_PUSHMSG_H

/*** Nested Include Files	***/

#include <jdi_browsermodule.h>
#include <jdi_xml.h>
#include <jdi_wbxml.h>

/*** Type Definitions		***/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The enumerations specifies the type of action (with the priority) to be performed 
 *		  on the push message
 */
typedef enum _push_action_value
{
	E_PUSH_ACTION_SIGNAL_NONE = 0, /**< Specifies that no specified action to be taken on the push message */ 	
	E_PUSH_ACTION_SIGNAL_LOW, /**< Specifies that the action priority for the push message is low */
	E_PUSH_ACTION_SIGNAL_MEDIUM, /**< Specifies that the action priority for the push message is medium */
	E_PUSH_ACTION_SIGNAL_HIGH, /**< Specifies that the action priority for the push message is high */
	E_PUSH_ACTION_EXECUTE_LOW, /**< Specifies that the action on the push message should be executed (with low priority) */
	E_PUSH_ACTION_EXECUTE_HIGH, /**< Specifies that the action on the push message should be executed (with high priority) */
	E_PUSH_ACTION_CACHE /** Specifies that the push message action is to cache the push content */
} E_PUSH_ACTION_VALUE ;

/**
 * @brief The enumerations specifies the types of push messages.
 */
typedef enum _pushmsg_type
{
	E_PUSH_SERVICE_INDICATION = 0, /**<Specifies that the push message contains service indication information (SI) */
	E_PUSH_SERVICE_LOADING /**< Specifies that the push message contains service loading information (SL) */
} E_PUSH_TYPE ;

/**
 * @brief The application needs to use this enum to specify the condition based on which the sorting has to be done
 *		  on the push message list.
 */
typedef enum _push_sort_item_list
{
	E_PUSH_SORT_TYPE, /**<Specifies that the sorting has to be done based on the push message type (SI or SL) */
	E_PUSH_SORT_URL, /**<Specifies that the sorting has to be done based on the URL of the push message */
	E_PUSH_SORT_MESSAGE, /**<Specifies that the sorting has to be done based on the message in the push content */
	E_PUSH_SORT_SENDER, /**<Specifies that the sorting has to be done based on the sender of the push message */
	E_PUSH_SORT_CREATED /**<Specifies that the sorting has to be done based on the time when the push message is created */
} E_PUSH_SORT_ITEM_LIST ;

/**
 * @brief The application needs to use this enum to specify the order in which the sorting has to be done
 *		  on the push message list.
 */
typedef enum _push_sort_order
{
	E_PUSH_ASCENDING_ORDER, /**< Specifies that the sorting has to be done in ascending order */
	E_PUSH_DESCENDING_ORDER /**< Specifies that the sorting has to be done in descending order */
} E_PUSH_SORT_ORDER ;

/**
 * @brief Specifies the structure which is used by the push handler to store the time value (created, expired)
 *		  of the push messages.
 */
typedef struct push_time
{
	JC_UINT16	uhYear ; /**< Specifies the year */
	JC_UINT8	ucMonth ; /**< Specifies the month */
	JC_UINT8	ucDay ; /**< Specifies the day */
	JC_UINT8	ucHour ; /**< Specifies the hour */
	JC_UINT8	ucMinute ; /**< Specifies the minute */
	JC_UINT8	ucSeconds ; /**< SPecifies the seconds */
} ST_PUSH_TIME ;

/**
 * @brief This push handler uses this structure to pass the details of the all the messages
 *		  (in the form of link list) that are stored by the push handler. 
 */
typedef struct push_info
{
	E_PUSH_TYPE				ePushType ;	/**< Specifies the push type (SI/SL) */
	JC_CHAR					*pmURL ; /**< Specifies the URL related to the push message. */					
	JC_INT8					*pcId ;	/**< Specifies the ID of the message */					
	ST_PUSH_TIME			stPushCreated ;	/**< Specifies the time when the push message is created */
	ST_PUSH_TIME			stPushExpires ;	/**< Specifies the time when the push message will get expired */
	E_PUSH_ACTION_VALUE	eAction ; /**< Specifies the type of action to be taken on the push message */					
	JC_CHAR					*pmFrom ; /**< Specifies the name of the message originator */ 
	JC_BOOLEAN				bIsRead ; /**< Specifies whether the message is already read */
	JC_INT32				iReserved ; /**< Reserved for future use */
	JC_BOOLEAN				bIsExpired ; /**< Specifies whether the message has expired */
	JC_CHAR					*pmMessage ; /**< Specifies the push message content (Only in case of SI) */
	JC_BOOLEAN				bIsCreatedTimeAvailable ; /**< Specifies whether the created time for the message is available */
	JC_BOOLEAN				bIsExpiredTimeAvailable ; /**< Specifies whether the expired time for the message is available */
	struct push_info		*pstNext ; /**< Specifies the linked list of push messages*/
	struct push_info		*pstOriginalListNext ;/**< Specifies the origianl linked list (internally used by push handler) */ 
} ST_PUSH_INFO ;

/*** Macros					***/

/**
 * @brief While parsing the push content if any error is encountered (ex: invalid tag) this 
 *		  error is returned to the application
 */
#define JC_ERR_INVALID_PUSH_REQUEST			TERMINAL_ERR_CODE(PUSH_MODULE, 0x01)
/**
 * @brief Push handler returns this error if the application tries to enter a push message into
 *		  the push list which is already full.
 */
#define JC_ERR_PUSH_SERVICE_NO_ENTRY		TERMINAL_ERR_CODE(PUSH_MODULE, 0x02)
/**
 * @brief After adding a messasge sucessfully if the push handler identifies that the push list is
 *		  is full then it returns the following error.
 */
#define JC_ERR_PUSH_LIST_FULL				TERMINAL_ERR_CODE(PUSH_MODULE, 0x03)
/**
 * @brief This error is returned if the message to be added is older than a message in the list and
 *		  both the messages have the same ID.
 */
#define JC_ERR_PUSH_SI_OLDER				TERMINAL_ERR_CODE(PUSH_MODULE, 0x04)
/**
 * @brief This error code is returned if the message to be added has replaced a message in the list
 */
#define JC_ERR_PUSH_MSG_REPLACED			TERMINAL_ERR_CODE(PUSH_MODULE, 0x05)
/**
 * @brief While the push message is parsed if the push handler finds that the push has already expired
 *		  then this error is returned.
 */
#define JC_ERR_PUSH_SI_EXPIRED				TERMINAL_ERR_CODE(PUSH_MODULE, 0x06)
/**
 * @brief If a push message action is 'delete' then the push handler perform the delete operation
 *		  and return this error to the application.
 */
#define JC_ERR_PUSH_SI_DELETED				TERMINAL_ERR_CODE(PUSH_MODULE, 0x07)
/**
 * @brief This error is returned if the push message to be added already exists in the push list.
 */
#define JC_ERR_PUSH_SL_EXISTS				TERMINAL_ERR_CODE(PUSH_MODULE, 0x08)
/**
 * @brief If the push handler requests the push handler for the push message list and the push handler
 *		  finds that there are no messages added then this message is returned.
 */
#define JC_ERR_PUSH_LIST_EMPTY				TERMINAL_ERR_CODE(PUSH_MODULE, 0x09)
/**
 * @brief This error is returned if the content type of the push messages is invalid.
 */
#define JC_ERR_PUSH_UNKNOWN_CONTENT_TYPE	TERMINAL_ERR_CODE(PUSH_MODULE, 0x0A)

#define TEXT_VND_WAP_SI				"text/vnd.wap.si"
#define APPLICATION_VND_WAP_SIC		"application/vnd.wap.sic"
#define TEXT_VND_WAP_SL				"text/vnd.wap.sl"
#define APPLICATION_VND_WAP_SLC		"application/vnd.wap.slc"

/*** Function Prototypes		***/

/**
 * @brief		Initializes the push message (SI/SL) handler
 *
 * @param[in]	pmFileName Specifies the file name where the previous messages are stored 
 *			    (during previous deinitializations).
 * @param[in]	uiMaxPushCount Specifies the maximum number of push messages that should be stored
 *			    by the push handler.
 * @param[in]	vMemHandle Specifies the memory handle that is to be used by the push handler.
 * @param[out]	pvHandle The push handler returns this handle. From then on the application should use
 *				this handle for using the push handler functions until the push handler is deinitialized.
 *
 * @retval		JC_ERR_MEMORY_ALLOCATION If a memory allocation error occurs
 * @retval		JC_ERR_INVALID_PUSH_REQUEST If the push request buffer is invalid
 * @retval		JC_ERR_INVALID_PARAMETER If the parameters passed to the initialize function is invalid
 * @retval		JC_OK if the push handler is initialized successfuly.
 * @retval		JC_ERR_FILE_SYS_INIT if the file system is not initialized.
 * @retval		JC_ERR_FILE_NOT_FOUND if the push file does not exist
 * @retval		JC_ERR_FILE_OPEN if an error occurs while opening the push file.
 * @retval		JC_ERR_FILE_READ if an error occurs while reading from the push file.
 * @note		The push initialize function initializes the push message handler which only handles
 *				the SI/SL messages. The push message handler need not be initialized for CO operations.
 * @see			jdi_PushMsgDeInitialize
 *
 * The application needs to use this API to initialize the push message handler which handles
 * the SI/SL messages. The function first reads the messages from the push file (Created in previous 
 * push handler instances) and then initializes the necessary parameters for the push handler to
 * handle the application requests.
 */

JC_RETCODE jdi_PushMsgInitialize (JC_CHAR		*pmFileName,
								  JC_UINT32		uiMaxPushCount,
								  JC_HANDLE		vMemHandle,
								  JC_HANDLE		*pvHandle) ;

/**
 * @brief		Parses a SI/SL message.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the push message initialize function.
 * @param[in]	pucBuffer Specifies the push message buffer.
 * @param[in]	iLength Specifies the length of the push message buffer.
 * @param[in]	pcContentType Specifies the content type of the push message.
 * @param[in]	pcServerAddress Specifies the address of the server from which the message is originated.
 * @param[in,out]	pstPushInfo The push message handler parses the push buffer and returns the
 *				message information in the this field.
 * @retval		JC_ERR_PUSH_UNKNOWN_CONTENT_TYPE If the content type of the message is unknown.
 * @retval		JC_ERR_MEMORY_ALLOCATION Speicifies memory allocation error.
 * @retval		JC_ERR_INVALID_PUSH_REQUEST If the push content is invalid.
 * @retval		JC_ERR_PUSH_SI_DELETED If the message has action value delete.
 * @retval		JC_ERR_PUSH_SI_OLDER If the messaged to be parsed is older than a message in list 
 *				(where both the messages has the same ID).
 * @retval		JC_ERR_PUSH_SI_EXPIRED If the message being parsed is already expired.
 * @retval		JC_OK if the push message is parsed sucessfully.
 * @see			jdi_PushMsgFreeInfo
 *
 * The application needs to use this API to parse a raw push message buffer. Once the parsing
 * is successful then the application can add the message to the message list.
 *
 * The push handler will only allocate memory for the members of the structure ST_PUSH_INFO.
 * The application needs to use the API jdi_PushMsgFreeInfo to free the memory 
 * that has been allocated by the push handler.
 */

JC_RETCODE jdi_PushMsgParse (JC_HANDLE		vHandle, 
							 JC_UINT8		*pucBuffer,
							 JC_INT32		iLength,
							 JC_INT8		*pcContentType,
							 const JC_INT8	*pcServerAddress,
							 ST_PUSH_INFO	*pstPushInfo) ;

/**
 * @brief		Adds a push message to the message list.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the push message initialize function.
 * @param[in]	pstPushInfo Specifies the information of the message to be added.
 * @retval		JC_OK If the message is sucessfully added to the push list.
 * @retval		JC_ERR_PUSH_SERVICE_NO_ENTRY If the message cannot be added because the list is full.
 * @retval	    JC_ERR_PUSH_LIST_FULL Signifies that the push list has become full after adding the given message.
 * @retval		JC_ERR_PUSH_SI_OLDER If the incoming message is older to any of the	matching(SI-ID) message in the list.
 * @see			jdi_PushMsgDelete
 *
 * The application needs to use this API to add a message to the push message list that is 
 * maintained in the push message handler.
 */

JC_RETCODE jdi_PushMsgAdd (JC_HANDLE			vHandle,
						   const ST_PUSH_INFO	*pstPushInfo) ;

/**
 * @brief		Deletes a push message from the push message list
 *
 * @param[in]	vHandle Specifies the handle that was returned by the push message initialize function.
 * @param[in]	pstPushInfo Specifies the information of the message to be deleted.
 * @retval		void
 *
 * The application needs to use this API to delete a push message which was previously
 * added to the push message list.
 */

void jdi_PushMsgDelete (JC_HANDLE				vHandle,
						const ST_PUSH_INFO		*pstPushInfo) ;

/**
 * @brief		Deletes all the push messages.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the push message initialize function.
 * @retval		void
 *
 * The application needs to use this API to delete all the messages that are stored in the
 * list maintained by the push message handler.
 */

void jdi_PushMsgDeleteAll (JC_HANDLE		vHandle) ;

/**
 * @brief		Deletes all the messages that are expired.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the push message initialize function.
 * @retval		JC_OK if all the expired messages are deleted.
 *
 * The application needs to call this API to delete all the messages that are expired.
 
 * The API will check the expiry data for all the messages in the list and delete the
 * messages which are expired.
 */

JC_RETCODE jdi_PushMsgDeleteExpired (JC_HANDLE	vHandle) ;

/**
 * @brief		Gets the statistical information of the push message list.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the push message initialize function.
 * @param[out]	puiTotalMsg The function returns the number of messages in the message list in this field.
 * @param[out]	puiUnreadMsg the function returns the number of unread messages in the message list in this field.
 * @retval		void
 *
 * The application needs to call this API to get the statistical information of the messages
 * that are stored in the messages list. 
 *
 * The API will return the total number of messages in the push message list
 * and the number of unread messages in the push message list.
 */

void jdi_PushMsgStatistics (JC_HANDLE		vHandle,
							JC_UINT32		*puiTotalMsg,
							JC_UINT32		*puiUnreadMsg) ;

/**
 * @brief		Sets the read status of a message.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the push message initialize function.
 * @param[in]	bIsRead Specifies the read status to be set.
 * @param[in]	pstPushInfo Pointer to the message whose read status is to be changed.
 * @retval		JC_OK if the message is found and the read status of the message is set.
 * @retval		JC_ERR_INVALID_PARAMETER If the message is not found in the list.
 *
 * The application needs to use this API to set the read status of a message. The application
 * needs to mention the message whose read status is to be changed and the changed read status.
 */

JC_RETCODE jdi_PushMsgSetReadStatus (JC_HANDLE				vHandle,
									 JC_BOOLEAN				bIsRead,
									 const ST_PUSH_INFO		*pstPushInfo) ;
/**
 * @brief		Gets the message list.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the push message initialize function.
 * @param[out]	ppstPushInfo The function will return the pointer of the first message of the link
 *				list of message in this field.
 * @retval		JC_ERR_PUSH_LIST_EMPTY if the push message list is empty
 * @retval		JC_OK if the push message list is returned successfully.
 *
 * At any point of time if the application wants to know the push messages that are currently added
 * to the push list then this API is to be used. The API will return the 
 * pointer to the first node of the push list in the param ppstPushInfo.
 *
 * The application should not free the memory associated with the push list that
 * is returned in the function.
 */

JC_RETCODE jdi_PushMsgGetList	(JC_HANDLE			vHandle,
								 ST_PUSH_INFO		**ppstPushInfo) ;

/**
 * @brief		Stores push messages to persistent storage.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the push message initialize function.
 * @retval		JC_OK if the push messages are stored to persistent storage successfully.
 *
 * In normal flow the push messages are stored to persistent storage (push file) only while
 * the push handler module is deinitialized. If the application wants to perform persistent
 * storage without deinitializing then this API can be used. 
 */

JC_RETCODE jdi_PushMsgStorePersistent (JC_HANDLE		vHandle) ;

/**
 * @brief		Sorts the message list.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the push message initialize function.
 * @param[in]	ePushSortItem Specifies the condition based on which the sorting has to be done.
 * @param[in]	ePushSortOrder Specifies the order (ascending or descending) in which the sorting has to be done.
 * @retval		Pointer to the first message in the sorted linked list of messages.
 *
 * The application needs to use this function to sort the push message list. The application needs
 * to mention the condition (URL, Message etc) based on which the sorting has to be done. The application
 * also needs to mention the order (ascending or descending) in which the sorting has to be done.
 */

const ST_PUSH_INFO * jdi_PushMsgSort (JC_HANDLE					vHandle,
									  E_PUSH_SORT_ITEM_LIST		ePushSortItem,
									  E_PUSH_SORT_ORDER			ePushSortOrder) ;

/**
 * @brief		Frees a push message.
 *
 * @param[in]	vMemHandle Specifies the memory handle used by the push handler.
 * @param[in]   pstPushInfo Specifies the message information to be freed.
 * @retval		void
 *
 * Once the push handler parses any push buffer it returns the push information in the ST_PUSH_INFO
 * structure. If the application does not want to add the message and wants to free the message information
 * then it needs to use the following function.
 */

void jdi_PushMsgFreeInfo (JC_HANDLE			vMemHandle, 
						  ST_PUSH_INFO		*pstPushInfo) ;

/**
 * @brief		Deinitializes the push message handler.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the push message handler initialize function
 * @retval		JC_OK if the push message handler is sucessfully deinitialized.
 * @retval		JC_ERR_INVALID_PARAMETER if the handle passed to the function is invalid or
 *			    if the push file is not specified.
 * @retval		JC_ERR_MEMORY_ALLOCATION if the memory error occurs
 * @retval		JC_ERR_FILE_SYS_INIT if the file system is not initialized.
 * @retval		JC_ERR_FILE_OPEN if an error occurs while opening the push file.
 *
 * The application needs to use this API to deinitialize the push messages handler. The function
 * will write the currently stored messages into the push file (persistent storage).
 *
 * The API will store the push message in the file name that is mentioned by the applicaiton
 * when it calls the jdi_PushMsgInitialize API.
 */

JC_RETCODE jdi_PushMsgDeInitialize (JC_HANDLE		vHandle) ;

#ifdef __cplusplus
}
#endif

#endif

/* End of File */


