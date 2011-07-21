/***************************************************************************
*
* File Name : jmms_communicator.h
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
* Created By		: 
* Created Date		: 
*
* 
*
* Current Revision	:
*
***************************************************************************
*
*
* Revision Details
* ----------------
* 
* 1. Modified By, Modified Date, Purpose
*
*
*
*
***************************************************************************/


/***************************************************************************
* System Include Files
**************************************************************************/
#ifndef MMS_COMMUNICATOR_H
#define MMS_COMMUNICATOR_H


/***************************************************************************
* User Include Files
**************************************************************************/
/***************************************************************************
* Macros Definitions
**************************************************************************/

/*****************************************************************************
 * Type Definations									                	     *
 *****************************************************************************/

/**
 * @brief Enumeration of content flags
 *
 * Specifies different types of message PDU's.
 */
typedef enum e_messsage_substate_type
{	
	E_JMMS_MSG_NIL = 0,
	E_JMMS_MSG_DEFERRED,
	E_JMMS_MSG_RETRIEVED,
	E_JMMS_MSG_REJECTED,
	E_JMMS_MSG_NEW_COMPOSE,
	E_JMMS_MSG_EDIT_FROM_DRAFTS,
	E_JMMS_MSG_EDIT_FROM_OUTBOX,
	E_JMMS_MSG_SEND_FRM_OUTBOX,
	E_JMMS_MSG_EDIT_FROM_SEND,
	E_JMMS_MSG_SEND_COPY,
	E_JMMS_MSG_EDIT_FROM_INBOX,
	E_JMMS_MSG_PUSH_MSG /* for notification push */
}E_JMMS_MSG_SUBSTATE ;

/**
 * @enum	e_compose_type
 * @brief	Queue Node State Enumerations
 */
typedef enum e_queue_node_state
{
	E_JMMS_NODE_IDLE,							/**< State Idle */
	E_JMMS_NODE_SEND,							/**< State while sending */
	E_JMMS_NODE_RETRIEVE,						/**< State while retrieving */
	E_JMMS_NODE_AUTO_RETRIEVE,				/**< State while retrieving in Autodownload mode */
	E_JMMS_NODE_NOTIFICATION_RESP_SEND,		/**< State while Sending Notification-resp */
	E_JMMS_NODE_ACK_SEND,						/**< State while Sending Acknowledge-Ind */
	E_JMMS_NODE_READREPORT_PDU_SEND,			/**< State while Sending ReadReport PDU (for MMS version 1.1 and 1.2) */
	E_JMMS_NODE_READREPORT_MSG_SEND			/**< State while Sending ReadReply Message (for MMS version 1.0) */
}E_JMMS_QUEUE_NODE_STATE ;

typedef struct _req_list
{
	JC_UINT32					uiReferenceID ;
	E_JMMS_QUEUE_NODE_STATE	eNodeState ;
	E_JMMS_MSG_SUBSTATE		eMsgSubState ;
	JC_INT8					*pszContentType ;
	JC_UINT32					eStatus ;
	JC_RETCODE					rCode ;
	JC_CHAR					*pFileName ;
	JDD_FILE					*hFile ;
	JC_UINT32					uiCurSize ;
	JC_UINT32					uiMsgID ;
	JC_UINT32					uiSimID ;
	JC_BOOLEAN					bIsUnderProgress ;
	JC_BOOLEAN					bIsToRemove ;
	JC_UINT32				    uiContentLength;
	struct _req_list			*pNext ;
} ST_REQUEST_LIST ;

#define MMS_MAX_PR0GRESS_PENCENT  98  //before success response reached, the progress bar keeps 98%

/***************************************************************************
* Global variables declarations
**************************************************************************/

/***************************************************************************
* Local Function Prototypes
***************************************************************************/

JC_RETCODE jMMS_CommCB (void *pvAppArg, E_COMM_INDICATION eCommIndicationType, void *pvIndication) ;
JC_UINT32 jMMS_FetchData (JC_CHAR *psURL, CONTENT_DATA *pEncBuffer) ;
JC_RETCODE jMMS_HandleHeaderIndication (ST_COMM_HEADER_IND *pstHeaderIndication) ;
JC_RETCODE jMMS_HandleFetchIndication (ST_COMM_FETCH_IND *pstFetchIndication) ;
JC_RETCODE jMMS_HandleStopIndication (ST_COMM_STOP_IND *pstStopIndication) ;
JC_RETCODE jMMS_HandleErrorIndication (ST_COMM_ERR_IND *pstErrorIndication) ;
JC_RETCODE jMMS_HandleProgressIndication (ST_COMM_PROGRESS_IND *pstProgressIndication) ;
E_JMMS_QUEUE_NODE_STATE jMMS_SendReadReport (const JC_INT8 *pcVersion) ;
JC_RETCODE jMMS_AddReqRespNode (E_JMMS_QUEUE_NODE_STATE eNodeState, JC_UINT32 uiRefID, JC_UINT32 uiMsgID, E_JMMS_MSG_SUBSTATE eMsgSubState) ;
void jMMS_RemoveReqRespNode(JC_UINT32 uiRefID) ;
ST_REQUEST_LIST *jMMS_GetReqRespNodeByReqID (JC_UINT32 uiRefID) ;
ST_REQUEST_LIST *jMMS_GetReqRespNodeByState (E_JMMS_QUEUE_NODE_STATE eNodeState) ;
ST_REQUEST_LIST *jMMS_GetReqRespNodeByMsgID (JC_UINT32 uiMsgID) ;
void jMMS_RemoveReqRespNodeByMsgID (JC_UINT32 uiMsgID) ;
JC_CHAR *jMMS_ExtractContentURLFromNotification (JC_UINT32 uiMessageID) ;
JC_RETCODE jMMS_GetMsgFromStore (E_JMMS_MSG_SUBSTATE eMsgState, JC_UINT32 uiMsgID, CONTENT_DATA *pstEncapsulatedData) ;
JC_RETCODE jMMS_GenerateAckData (E_JMMS_MSG_SUBSTATE eMsgState,	E_JMMS_QUEUE_NODE_STATE eNodeState, JC_UINT32 uiMessageID, CONTENT_DATA *pstEncapsulatedData) ;
JC_RETCODE jMMS_ProcessCommunicatorFetchRequest (JC_UINT32 uiMsgID, E_JMMS_QUEUE_NODE_STATE eNodeState, E_JMMS_MSG_SUBSTATE eMsgSubState) ;
JC_RETCODE jmms_AddTextToSlide(MSG_HANDLE hMsg, JC_CHAR *pmTextBuffer, JC_UINT32 *puiMediaID) ;
JC_BOOLEAN jMMS_FindMsgInGetReqRespNode (MESSAGE_LIST		*pMessageList);
JC_BOOLEAN jMMS_IsMsgIdInGetReqRespNode(JC_UINT32 uiMsgID);
JC_BOOLEAN jMMS_IsReqNodeUnderProgress(void);
void jMMS_RemAllReqRespNodeBySIM(UINT8 nSimID);



#endif

