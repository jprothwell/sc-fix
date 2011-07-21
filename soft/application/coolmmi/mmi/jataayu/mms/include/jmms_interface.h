 /***************************************************************************
 *
 * File Name : jmms_interface.h
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
 * @file		jmms_interface.h
 * @ingroup		jMMS
 * @brief		
 *
 **/
 
#ifndef __JMMS_INTERFACE
#define __JMMS_INTERFACE

/***************************************************************************
* System Include Files
**************************************************************************/
  
/***************************************************************************
* User Include Files
**************************************************************************/
#include "jmms_settings.h"

/***************************************************************************
* Macros Definitions
**************************************************************************/

#ifdef __JATAAYU_APP__


/***************************************************************************
* Global variables declarations
**************************************************************************/
#define	jMMS_MAX_INPUT_SIZE			(255)
#define	jMMS_MAX_SLIDE_NUMBER		20
#define	JMMS_MAX_TEXT_PER_SLIDE		(255)
#define JMMS_MAX_MEIDA_PER_SLIDE	5
#define JMMS_MAX_SCREEN_BUFFER		5*1024
/***************************************************************************
* Local Function Prototypes
***************************************************************************/

typedef struct _mms_Header
{
	JC_CHAR				*pTo ;
	JC_CHAR				*pCc ;
	JC_CHAR				*pBcc ;	
	JC_CHAR				*pSubject ;
}JMMS_HEADER;

typedef enum e_jmms_view_state
{
	E_JMMS_VIEW_NONE, 
	E_JMMS_VIEW_FOLDER_MSG, 
	E_JMMS_PRVIEW_COMPOSED_MSG 	
}  E_JMMS_VIEW_STATE ;

typedef enum e_jmms_preview_state
{
	E_JMMS_PREVIEW_NONE,
	E_JMMS_PREVIEW_IDLE,
	E_JMMS_PREVIEW_STARTED,
	E_JMMS_PREVIEW_PAUSED,
	E_JMMS_PREVIEW_RESUMED,
	E_JMMS_PREVIEW_STOPPED,
	E_JMMS_PREVIEW_STARTED_LEFTKEY,
	E_JMMS_PREVIEW_STARTED_RIGHTKEY
} E_JMMS_PREVIEW_STATE ;

typedef enum e_jmms_comm_state
{
	E_JMMS_COMM_IDLE,
	E_JMMS_COMM_CONNECTING,
	E_JMMS_COMM_CONNECTED,
	E_JMMS_COMM_DISCONNECTING,
	E_JMMS_COMM_DISCONNECTED
} E_JMMS_COMM_STATE ;

typedef struct _jmms_handle
{
	JC_HANDLE		hMsgStore;//hStore ;
	U8				*pcInputBuff ;
	JC_INT8*		pcMMSCUrl ;
	MSG_HANDLE		hCurrMsg ;
	JC_UINT32		uiViewWindow; //uiWinId ;
	JC_HANDLE		hCommunicator; //hComm ;
	JC_UINT8		ActivePageIndex ;
	JMMS_HEADER		stMmsHeader ; 
	JMMS_SETTINGS   stMMSSetting ;
	JC_BOOLEAN		bIsAutoSignatureAdded ;	
	JC_UINT32       ActivePageMediaIds[JMMS_MAX_MEIDA_PER_SLIDE];
	JC_CHAR         *pActiveTextBuffer;
	JC_INT32 		iFolderItemIndex ;
	E_JMMS_VIEW_STATE eViewState;// to maintain a state of view or preview
	JC_CHAR*		pszScreeBuff;		
	JC_INT32		iHilitedItem ;
	JC_UINT32       uiHighlightedMsgId;
	ST_REQUEST_LIST	*pReqRespList ;
	JC_BOOLEAN		bSendReadReport;
    JC_BOOLEAN		bIsViolatingContentClass ;	/* This is the state to indicate that the user
														is trying to violate the content class settings
														during the compose */
	E_JMMS_PREVIEW_STATE	ePreviewState ;
	E_JMMS_FOLDER_ID		eCurrentFolderID ;
	E_JMMS_COMM_STATE		eCommunicatorState ;
	JC_BOOLEAN				bEndKeyPressed ;
#ifdef __MMI_MULTI_SIM__
	U8     send_simid;
	U8     ph_edit_simid;
#endif		
	JC_UINT32 oldNumberOfMessages;//add by wangzl
} JMMS_APP_INFO ;

extern JMMS_APP_INFO *g_pstJMmsAppInfo ;
JC_RETCODE		jMms_init_engine () ;
void 	jMms_init_context () ;
#ifdef MMI_ON_HARDWARE_P /* zhuoxz,2009-4-29 */
U8*jMms_getInputBuff () ;
#else /* #ifdef MMI_ON_HARDWARE_P */
U8*jMms_getInputBuff () ;
#endif /* #ifdef MMI_ON_HARDWARE_P */
JC_RETCODE	jMms_msgComposeInit () ;
JC_RETCODE	jMms_createMmsViewWindow () ;
void	jMms_comm_init () ;
void	jMMS_updateDataConnSettings (ST_COMM_PROFILE *pstProxySett, ST_GENERIC_DATA_CONN *pstGenDataConn);
void 	jMMS_SetPrimaryMMIStatus(JC_BOOLEAN bIsActive);
JC_BOOLEAN 	jMMS_GetPrimaryMMIStatus();
JC_RETCODE 	jMms_setMsgProperties () ;
void 	jMMS_RegisterCallbacks();
JC_RETCODE 	jMMS_configMsgSmil (MSG_HANDLE hMsg) ;
void jMms_DestroyMmsViewWindow ();
JC_HANDLE  jmms_getWinHandle ();
JC_UINT32 jMMS_GetFontHeight(void);
JC_BOOLEAN jMMS_IsMMSAppOrDownloadActive (void) ;
void jmms_CreateMMSDir();

#endif //__JMms_INTERFACE	
#endif //__JATAAYU_APP__

