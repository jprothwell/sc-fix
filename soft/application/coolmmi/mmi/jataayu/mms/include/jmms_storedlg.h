 /***************************************************************************
 *
 * File Name : jmms_storedlg.h
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
 * @file		jmms_storedlg.h
 * @ingroup		jMMS
 * @brief		Describes the Store Dlg related API's.
 *
 **/ 
#ifndef __JMMS_STOREDLG
#define __JMMS_STOREDLG

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


/***************************************************************************
* Local Function Prototypes
***************************************************************************/
void jMms_StoreInboxMmsMsgHlthandler(int iSelectIndex) ;
void jMms_StoreInbox_handle_highlight_item();
void jMms_inbox_entry_handler(void);
void jMms_StoreSent_highlight_handler (int iSelectIndex) ;
void jMms_StoreSent_handle_highlight_item();
void jMms_sent_entry_handler(void);
void jMms_StoreOutBox_highlight_handler (int iSelectIndex) ;
void jMms_StoreOutbox_handle_highlight_item();
void jMms_outbox_entry_handler(void);
void jMms_StoreDraft_highlight_handler(int iSelectIndex) ;
void jMms_StoreDraft_handle_highlight_item();
void jMms_draft_entry_handler(void);
void jmms_DisplayMsgListInFolder(E_JMMS_FOLDER_ID eFolderID);
void jmms_DisplayMsgListExitHandler();
JC_RETCODE jmms_GetFolderMsgList(U8**pFolderMessageListItems, E_JMMS_FOLDER_ID eFolderID);
void jmms_InboxPrepareLSKHandler();
void jmms_SentPrepareLSKHandler();
void jmms_OutboxPrepareLSKHandler();
void jmms_DraftPrepareLSKHandler();
void jMms_Highlight_Resend_Send(void);
void jMms_Highlight_Resend_Send_And_Save(void);
#endif // __JATAAYU_APP__

#endif //__JMMS_STOREDLG
