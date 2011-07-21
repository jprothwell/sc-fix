/***************************************************************************
 *
 * File Name : jwap_push.h
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
 * @file		jwap_push.h
 * @ingroup	jWAP
 * @brief
 *
 **/
 #ifndef __JWAP_PUSH_H
#define __JWAP_PUSH_H
 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

 /***************************************************************************
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					JWAP_APP
#else
#define __MODULE_ID__					0
#endif
#include "jdi_pushmsg.h"
#include "commonscreens.h"
#include "jdi_communicator.h"

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/

 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/
#ifdef JWAP_PUSH_ENABLE	
#ifdef __JATAAYU_APP__
	

typedef struct __wap_push_info__
{
	JC_HANDLE		vPushHandle ;
	ST_PUSH_INFO	*pstPushInfo ;
	JC_INT32		ucSelectedMsg ;
	JC_UINT32		uiNumPushMsgs ;
	JC_CHAR 		*amListItems [JWAP_MAX_PUSH_MSGS] ;
	JC_UINT16		uhIndexIconsImageList [JWAP_MAX_PUSH_MSGS] ;
	JC_INT32		bIsBookMarkAdded;
} ST_WAP_PUSH_INFO ;

void jWap_Push_Highlite_Handler (int iSelectIndex) ;
void jWap_Push_Option_Highlite_Handler (int iSelectIndex) ;
void jWap_PushOption() ;
void jWap_push_view_handler (void) ;
void jWap_push_go_handler() ;
void jWap_push_clear_handler() ;
void jWap_push_clearall_handler () ;
void jWap_push_entry_handler(void) ;
void HandleClearPushYes () ;
void HandleClearAllPushYes () ;
void HandleClearPushNo () ;
void GetPushClearAllUserConfirmation () ;
void GetPushClearUserConfirmation () ;
void jWap_PushFreeMemory (void) ;
void jWap_PushExit () ;
JC_UINT8 jWap_GetUniqueID (JC_INT32 iReservedField) ;
JC_INT32 jWap_SetUniqueID (JC_INT32 iReservedField, JC_UINT8 ucUniqueID) ;
JC_UINT8 jWap_CreateUniqueID (void) ;
void jWap_HandlePushMsg (ST_COMM_PUSH_IND *pstPushData) ;
void jWap_PushGetPushItemFromTheList (JC_UINT8 ucSelectedItem) ;
ST_PUSH_INFO *jWap_PushGetPushItemFromTheListByReservedId (JC_INT32 iReservedID) ;
JC_RETCODE jWap_ShowLatestItemDetails (void) ;
JC_RETCODE jWap_PushDeInitialise (void) ;
JC_RETCODE jWap_PushInitialise (void) ;
void jWap_push_add_to_bm_handler();
JC_UINT32 jwap_getUnreadPushMsg(JC_UINT32 *uiTotalMsg);
JC_UINT8 jWap_HandleServiceInboxEndKey(void* p);
#endif //JWAP_PUSH_ENABLE


#endif //__JATAAYU_APP__
#endif //__JWAP_PUSH_H

