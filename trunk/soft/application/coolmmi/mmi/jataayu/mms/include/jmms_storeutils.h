 /***************************************************************************
 *
 * File Name : jmms_storeutils.h
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
 * @file		jmms_storeutils.h
 * @ingroup	jMMS
 * @brief		Describes the Task related API's.
 *
 **/ 
#ifndef __JMMS_STOREUTILS
#define __JMMS_STOREUTILS

 /***************************************************************************
 * System Include Files
 **************************************************************************/


 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include "jmms_inc.h"

/***************************************************************************
 * Macros
 **************************************************************************/
#ifdef __JATAAYU_APP__

/***************************************************************************
* Macros Definitions
**************************************************************************/


/***************************************************************************
* Global variables declarations
**************************************************************************/
typedef void (*FPOINTER) (void) ;


/***************************************************************************
* Local Function Prototypes
***************************************************************************/
void jMms_view_msg_handler(void);
void jmms_InboxReplyHandler();
//void jMms_reply_by_sms_handler();
void jmms_InboxReplyAllHandler();
void jMms_delete_handler();
void jMMS_GetUserConfirmation(JC_UINT8 * pcMessage,FPOINTER fpLeft,FPOINTER fpRight,FPOINTER entryFun);
void jMms_deleteAll_Msg();
void jMms_delete_all_handler();
void get_highlited_id(int iSelectIndex);
void jmms_SentEditHandler();
void jmms_OutboxEditHandler();
void jmms_DraftEditHandler();
void jMms_ReSend_handler();
void jMms_Send_handler();
void jmms_InboxForwardHandler();
void jmms_SentForwardHandler();
void jmms_OutboxForwardHandler() ;
void jMms_GoBack_History();
JC_RETCODE AddTextMediaWithHeader(MSG_HANDLE	 phMsgHandle,JC_CHAR *psTextBuffer,JC_UINT8 ucSlideNo);
void jmms_SetReadFlag(MESSAGE_LIST*	pstMsgDetail);
void jMms_GoBack_History();
JC_RETCODE	jmms_CreateMsgHandleForFolderMsg(E_JMMS_FOLDER_ID eFolderID, JC_UINT32 uiMsgID, 
													E_JMMS_COMPOSE_STATE eComposeState);
#endif // __JATAAYU_APP__
#endif //__JMMS_STOREUTILS

