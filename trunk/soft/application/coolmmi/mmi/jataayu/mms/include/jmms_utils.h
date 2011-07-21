 /***************************************************************************
 *
 * File Name : jmms_utils.h
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
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
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND 
 * AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
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
 ***************************************************************************/
 
 /** 
 * @file		jmms_utils.h
 * @ingroup		jMMS
 * @brief		Describes the MMS Utility related API's.
 *
 **/ 
#ifndef __JMMS_UTILS_INCLUDE
#define __JMMS_UTILS_INCLUDE

/***************************************************************************
 * System Include Files
 **************************************************************************/

/***************************************************************************
 * User Include Files
 **************************************************************************/
#include "jmms_inc.h"

/***************************************************************************
* Macros Definitions
**************************************************************************/

/***************************************************************************
* Global variables declarations
**************************************************************************/
#define JMMS_MAX_USE_BUFFER_SIZE	(100*1024)
#define JMMS_MAX_FOLDER_NAME_LEN	16
#define JMMS_MAX_MENU_ITEM			10

typedef enum e_jmms_folder_id
{
	E_JMMS_INBOX_FOLDER,
	E_JMMS_OUTBOX_FOLDER,
	E_JMMS_SENT_FOLDER,
	E_JMMS_DRAFT_FOLDER,
	E_JMMS_NONE_FOLDER
}  E_JMMS_FOLDER_ID ;


typedef enum e_jmms_compose_state
{
	E_JMMS_REPLY, 
	E_JMMS_REPLY_ALL, 
	E_JMMS_FORWARD, 
	E_JMMS_EDIT 
}  E_JMMS_COMPOSE_STATE ;

typedef enum e_jmms_app_event
{
	E_JMMS_EVENT_TIMER,
	E_JMMS_EVENT_PAGE_PRESENTATION,
	E_JMMS_EVENT_MSG_RETRIEVE_SUCCESS,	/* This event is sent from MMS to self indicating that one recv is completed/stopped */
	E_JMMS_EVENT_MSG_RETRIEVE_FAIL,	/* This event is sent from MMS to self indicating that one recv is completed/stopped */
	E_JMMS_EVENT_MSG_AUTO_RETRIEVE_SUCCESS,
	E_JMMS_EVENT_MSG_AUTO_RETRIEVE_FAIL,
	E_JMMS_EVENT_MSG_SEND_SUCCESS,
	E_JMMS_EVENT_MSG_SEND_FAIL,
	E_JMMS_EVENT_DATACONN_SUCCESS,
	E_JMMS_EVENT_DATACONN_FAIL
}  E_JMMS_APP_EVENT ;

typedef struct _jmms_app_event_data
{
	JC_UINT32					uiReferenceID ;
	JC_UINT32					uiMsgID ;
} ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA ;

/**
 * @brief Enumeration of media types
 *
 * Specifies the media categories based on content-types.
 */
typedef enum _mms_media_type
{
	E_MMS_MEDIA_TEXT = 0x00,	/**< Specifies media type as Inline-Text(Default)*/
	E_MMS_MEDIA_IMAGE,			/**< Specifies that media object is an image*/
	E_MMS_MEDIA_AUDIO,			/**< Specifies that media object is an audio*/
	E_MMS_MEDIA_VIDEO,			/**< Specifies that media object is a video*/
	E_MMS_MEDIA_OTHERS			/**< Specifies that media object is a attachment*/
}EMMSMediaType ;

typedef struct mms_media_info
{
	JC_CHAR				*pFileName ; 	/**< Specifies the file name (including complete path)  incase of image, video, attachment . Specifies the Text Buffer In case of Text Send as MMS*/
	EMMSMediaType		eMediaType ;	/**< Specifies the media type*/
	JC_CHAR				*pRecpt_To  ;	/**< Specifies the Reciepient List for To Field should be ';' seperated*/ 
	JC_CHAR				*pRecpt_CC  ;	/**< Specifies the Reciepient List for CC Field should be ';' seperated*/
	JC_CHAR				*pRecpt_BCC ;	/**< Specifies the Reciepient List for BCC Field should be ';' seperated*/
}MMS_MEDIA_INFO ;


/***************************************************************************
* Local Function Prototypes
***************************************************************************/

void	jmms_FreeContentData( CONTENT_DATA * pContentData, JC_RETCODE eRet );

JC_RETCODE jmms_MMISetMsgContentStream(E_JMMS_FOLDER_ID eFolderID,
												JC_UINT32 MsgId,CONTENT_DATA* pViewContent);

JC_RETCODE jmms_SetActiveFolder(E_JMMS_FOLDER_ID eFolderID);
JC_RETCODE jmms_GetMsgInfoByID(E_JMMS_FOLDER_ID eFolderID, JC_UINT32 msgId, MESSAGE_LIST** msgInfo);

JC_INT32	jmms_MMIGetMsgSize(E_JMMS_FOLDER_ID eFolderID,JC_UINT32 MsgIndex);
JC_UINT8* jmms_GetFolderName(E_JMMS_FOLDER_ID eFolderID);
JC_RETCODE jmms_MoveMmsByID(E_JMMS_FOLDER_ID eSrcFolderID, E_JMMS_FOLDER_ID eDesFolderID, 
				JC_UINT32 msgId);
JC_RETCODE jmms_GetMsgDetailsByID (STORE_HANDLE hStoreHandle, E_JMMS_FOLDER_ID eFolderID, JC_UINT32 ucMsgId, MESSAGE_LIST **ppstMsgDetail) ;
JC_RETCODE jMMS_GetMsgFromStoreByMsgID (STORE_HANDLE hStoreHandle, E_JMMS_FOLDER_ID eFolderID,
	JC_UINT32 uiMsgID, CONTENT_DATA *pstContentData) ;

JC_RETCODE jmms_GetMsgDetailsByIndex (STORE_HANDLE hStoreHandle, E_JMMS_FOLDER_ID eFolderID,
												JC_UINT32 ucMsgIndex, MESSAGE_LIST **ppstMsgDetail) ;
JC_RETCODE jmms_GetMsgList (STORE_HANDLE hStore, MESSAGE_LIST **ppMessageList, 
						   		RETRIEVE_FILTERS *pRetrieveFilters);
JC_BOOLEAN jmms_GetMsgID (E_JMMS_FOLDER_ID eFolderID, JC_UINT8 ucMsgIndex, JC_UINT32 *pucMsgId) ;

JC_RETCODE FileStreamReadCB (void *pStream, JC_INT32 uiOffset, JC_UINT32 uiSize, JC_UINT8 *pBuffer, JC_UINT32 *pSizeRead);

JC_RETCODE jmms_FileStreamWriteCB (void *pStream, JC_UINT8 *pBuffer, JC_UINT32 uiSize);
void jmms_UpdateMmsIcon();
void jmms_HideMmsIcon();
void jmms_RestoreMmsICon();


JC_RETCODE jmms_GetReadMsgStatus(JC_UINT32 *pucNUnreadMsg,JC_UINT32 *pucNTotMsg,
						E_JMMS_FOLDER_ID eFolderID);
#ifdef __MMI_MULTI_SIM__
JC_RETCODE jmms_GetReadMsgStatusBySimID(JC_UINT32 *pucNUnreadMsg,JC_UINT32 *pucNTotMsg,
						E_JMMS_FOLDER_ID eFolderID, UINT8 nSimID);
#endif
JC_RETCODE jMMS_SaveOrUpdateMsg (E_JMMS_FOLDER_ID eFolderID, JC_UINT32 *puiMessageID,
				CONTENT_DATA *pstContentData, MESSAGE_HEADERS *pMsgHeaders, 
				JC_UINT32 uiExtraBytes);
JC_RETCODE jMMS_DeleteMsg (E_JMMS_FOLDER_ID eFolderID, JC_UINT32 uiMessagID, JC_BOOLEAN bCheckNodeIsBusy) ;
JC_RETCODE jmms_GetMsgExtraBytes (STORE_HANDLE hStoreHandle, E_JMMS_FOLDER_ID eFolderID, JC_UINT32 uiMsgId, JC_UINT32 *uiExtraBytes) ;
void jMMS_FormDateTimeStringFromSecs (JC_UINT32 uiSeconds, JC_CHAR *pmcFormattedDateTimeString) ;
JC_UINT32 jMMS_GetTotalNumberOfMsgsInFolder (E_JMMS_FOLDER_ID eFolderID) ;
JC_UINT32 jMMS_GetTotalNumberOfMsgs (void) ;
void jMMS_ErrorHandler (JC_RETCODE rCode, FuncPtr pfCBFPtr) ;
void jMMS_NetErrorHandler (JC_RETCODE rCode, FuncPtr pfCBFPtr) ;
void jMMS_DataconnectionErrorHandler (JC_RETCODE rCode, FuncPtr pfCBFPtr) ;
JC_RETCODE jMMS_CreateReservedMMSFile (void) ;
JC_CHAR *jMMS_GetMMSFullFileName (const JC_INT8 *pcFileName) ;
JC_BOOLEAN jmms_IsFileNameAscii (JC_CHAR *pFileName);
void jmms_ShowNewMMSPopup(JC_UINT32 uiMsgID);
void jmms_DownloadCompletePopup();
JC_UINT32 jMMS_GetMMSFreeSpace (JC_BOOLEAN bIsTOConsiderThreshold) ;
JC_UINT32 jMMS_GetMMSTotalSpace (void) ;
BOOL jMMS_CheckJMMSFull();
void jMMS_NewMMSAutoRetiveFailPopup(JC_RETCODE rCode);
void jmms_prepareLaunch();
JC_RETCODE jmms_LaunchMMSFromApp (MMS_MEDIA_INFO *pstMMSMediaInfo);
char* jmms_UtilsUnicodeToUtf8( const JC_CHAR* wstr , int *outLen);

#endif // __JMMS_UTILS_INCLUDE

