#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_cb.c"
 /***************************************************************************
 *
 * File Name : jdd_taskapi.c
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
 * @file		jmms_cb.c
 * @ingroup		jMMS
 * @brief		Describes the Task related API's.
 *
 **/
 
 #ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

#include "jmms_inc.h"
#include "jdi_communicator.h"


#include "jmms_composeutils.h"
#include "jmms_cb.h"
#include "jmms_composedlg.h"
#include "jmms_storedlg.h"
#include <messagesresourcedata.h>//for mms notification image 
#include "jmms_communicator.h"

 /***************************************************************************
 * Macros
 **************************************************************************/

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

JC_RETCODE jMms_cbError (MSG_HANDLE hMsg, void *vCallbackArg, void *vCBStruct, ErrorCode eErrorCode )
{
	JC_RETCODE	rCode = JC_OK ;
	if(JC_NULL != hMsg)
	{
		if(E_MEDIA_ALREADY_EXISTS == eErrorCode)
		{		
			
		}
		else if(E_WARNING_CREATION_MODE_ADDMEDIA == eErrorCode)
		{			
			rCode = JC_ERR_ASYNC_CALL ;
			g_pstJMmsAppInfo->bIsViolatingContentClass = E_TRUE ;
			jmms_ShowConfirmationForRestrictedMediaAdd () ;
		}
		else if(E_WARNING_CREATION_MODE_SETMODE == eErrorCode || E_WARNING_CREATION_MODE_FORWARD == eErrorCode)
		{	
			DisplayPopup(GetString(STR_ID_MMS_WARNING_CONTENT_CLASS), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
	}

	JDD_LOG_INFO0("Exiting COMPOSE Module:ComposeModeErrorCB. rCode : ");
	return rCode ;
}
 
JC_RETCODE jmms_AttachmentMiscIndicatorCB(MSG_HANDLE hMsg, void *vCallbackArg,
							   						ATTACHMENT_MISC_IND *pstAttachmentMiscInfo)
{
	JDD_LOG_INFO0("IndicateAttachment Callback : ");
	return JC_OK ;
}

 JC_RETCODE jmms_AudioVideoUnknownIndicatorCB(MSG_HANDLE hMsg, void *vCallbackArg,
							  AUDIO_VIDEO_UNKNOWN_IND *pstAudioVideoUnknownInfo)
 {
	 JC_RETCODE rStatus = JC_OK;
 
	 PARAM_INTENTIONALLY_NOT_USED(hMsg);
	 PARAM_INTENTIONALLY_NOT_USED(vCallbackArg);
 	 mmi_trace(1, "jdd_log: AudioVideoUnknownIndicatorCB type[%d]", pstAudioVideoUnknownInfo->eContent);
	 switch (pstAudioVideoUnknownInfo->eContent)
	 {
		 case E_CONTENT_AUDIO:
			 {
				//Show Audio Icon on screen
				 break;
			 }
		 case E_CONTENT_VIDEO:
			 {
			 	//Show Video Icon on screen
				 break;
			 }
		 case E_CONTENT_UNKNOWN:
			 {
			 	//DisplayPopup(GetString(STR_ID_MMS_MEDIA_CONTENT_UNS), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
				break;
			 }
		 case E_CONTENT_CORRUPTED:
			 {			 	
			 	//DisplayPopup(GetString(STR_ID_MMS_MEDIA_CONTENT_CORRUPT), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
				break;
			 }
		 default:
			 break;
	 }
	 return rStatus;
 }

JC_RETCODE jmms_DRMMediaListIndicatorCB(MSG_HANDLE hMsg, void *vCallbackArg,
						   DRM_MEDIA_LIST *pstDRMInfoList)
{	
	DisplayPopup((PU8)GetString(STR_ID_MMS_DRM_NOT_SUPPORTED), IMG_GLOBAL_ERROR, 1, 2000, 1);
	return JC_OK ;
}

JC_RETCODE jmms_TimerListenerCB (MSG_HANDLE hMsg, void *pCallbackArg, void *pvTimerInfo)
{
	JC_RETCODE rCode = JC_OK ;
	JC_EVENT_DATA stTimerEvent ;

	PARAM_INTENTIONALLY_NOT_USED(pCallbackArg);
	
	memset( &stTimerEvent, 0, sizeof( JC_EVENT_DATA ) ) ;
	
	stTimerEvent.srcTaskInfo.iTaskID   = 
	stTimerEvent.srcTaskInfo.iAppID    = 
	stTimerEvent.destTaskInfo.iTaskID  = 
	stTimerEvent.destTaskInfo.iAppID   = E_TASK_MMS_CORE ;
	stTimerEvent.iEventType			= E_JMMS_EVENT_TIMER ;
	
	if ( JC_OK == jdd_QueueSend( &stTimerEvent ) )
	{
		rCode = JC_ERR_ASYNC_CALL ;
	}
	else
	{
		jdd_QueueFree( &stTimerEvent ) ;
	}
	return rCode ;
}

JC_RETCODE	jmms_PagePresentationIndicatorCB(MSG_HANDLE hMsgHandle, void *pCallbackArg, PAGE_PRESENTATION_IND *pPagePresentationInfo)
{    
	JC_RETCODE rCode = JC_OK ;
	JC_EVENT_DATA stEvent ;
	PARAM_INTENTIONALLY_NOT_USED(pCallbackArg);

	memset( &stEvent, 0, sizeof( JC_EVENT_DATA ) ) ;

	stEvent.srcTaskInfo.iTaskID   = 
	stEvent.srcTaskInfo.iAppID    = 
	stEvent.destTaskInfo.iTaskID  = 
	stEvent.destTaskInfo.iAppID   = E_TASK_MMS_CORE ;
	stEvent.iEventType 		= E_JMMS_EVENT_PAGE_PRESENTATION ;

	stEvent.uiEventInfoSize = sizeof (PAGE_PRESENTATION_IND) ;
	rCode = jdd_QueueAlloc (&stEvent) ;
	if (JC_OK == rCode)
	{
		jc_memcpy ((PAGE_PRESENTATION_IND *) stEvent.pEventInfo, 
				(PAGE_PRESENTATION_IND *) pPagePresentationInfo, sizeof (PAGE_PRESENTATION_IND)) ;
		jdd_QueueSend (&stEvent) ;
		rCode = JC_ERR_ASYNC_CALL ;
	}
	return rCode ;
}

#endif //__JATAAYU_APP__

