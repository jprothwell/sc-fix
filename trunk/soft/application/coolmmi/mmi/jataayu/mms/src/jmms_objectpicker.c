#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_objectpicker.c"
 /***************************************************************************
 *
 * File Name : jmms_objectpicker.c
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
 * @file		jmms_objectpicker.c
 * @ingroup		jMMS
 * @brief	
 *
 **/

#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_objectpicker.c"
#ifdef JATAAYU_SUPPORT 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include "jmms_inc.h"


#include "filemgr.h"
#include "fmt_def.h"
#include "jmms_composeutils.h"
#include "jmms_objectpicker.h"


/***************************************************************************
* All Function Definitions
***************************************************************************/

void MediaPickerImageCallback(void*pFileName,int is_short)
{
	JC_RETCODE rCode = JC_OK; 
	JC_CHAR * pmStr = JC_NULL;
	
	mmi_trace(1,"jdd_log: ImageCallback[%s]",pFileName) ;	
	if( pFileName == NULL )
	{
		GoBackHistory();
		return;
	}	
	pmStr = jdi_CUtilsTcsDuplicate(JC_NULL,pFileName) ;
	if(pmStr)
	{
		if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[1])
		{
			rCode = jdi_MsgDeleteMedia(g_pstJMmsAppInfo->hCurrMsg,g_pstJMmsAppInfo->ActivePageIndex,
									g_pstJMmsAppInfo->ActivePageMediaIds[1],0) ;
			g_pstJMmsAppInfo->ActivePageMediaIds[1] = 0;
		}
		else if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[3])
		{
			rCode = jdi_MsgDeleteMedia(g_pstJMmsAppInfo->hCurrMsg,g_pstJMmsAppInfo->ActivePageIndex,
									g_pstJMmsAppInfo->ActivePageMediaIds[3],0) ;
			g_pstJMmsAppInfo->ActivePageMediaIds[3] = 0;
		}
		rCode = jMMS_AddObjectMedia(pmStr,E_MEDIA_IMAGE, 0);
		mmi_trace(1,"*****MediaPickerImageCallback AddObjectMedia[%d]****",rCode) ;
	}
	jMMS_HandleMediaError(rCode);
	
}

void MediaPickerAudioCallback(void*pFileName,int is_short)
{
	JC_RETCODE rCode = JC_OK; 
	JC_CHAR * pmStr = JC_NULL;
	
	mmi_trace(1,"jdd_log: AudioCallback[%s]",pFileName) ;	
	if( pFileName == NULL )
	{		
		GoBackHistory();
		return;
	}
	pmStr = jdi_CUtilsTcsDuplicate(JC_NULL,pFileName) ;
	if(pmStr)
	{
		if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[3])
		{
			rCode = jdi_MsgDeleteMedia(g_pstJMmsAppInfo->hCurrMsg,g_pstJMmsAppInfo->ActivePageIndex,
								g_pstJMmsAppInfo->ActivePageMediaIds[3],0);
			g_pstJMmsAppInfo->ActivePageMediaIds[3] = 0;
		}
		else if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[2])
		{
			rCode = jdi_MsgDeleteMedia(g_pstJMmsAppInfo->hCurrMsg,g_pstJMmsAppInfo->ActivePageIndex,
								g_pstJMmsAppInfo->ActivePageMediaIds[2],0);
			g_pstJMmsAppInfo->ActivePageMediaIds[2] = 0;
		}
		rCode = jMMS_AddObjectMedia(pmStr,E_MEDIA_AUDIO,0);
	}
	mmi_trace(1,"*****MediaPickerAudioCallback AddObjectMedia[%d]****",rCode) ;	
	jMMS_HandleMediaError(rCode);
}

void MediaPickerVideoCallback(void*pFileName,int is_short)
{
	JC_RETCODE rCode = JC_OK; 
	JC_CHAR * pmStr = JC_NULL;
	
	mmi_trace(1,"jdd_log: VideoCallback[%s]",pFileName) ;	
	if( pFileName == NULL )
	{		
		GoBackHistory();
		return;
	}
	pmStr = jdi_CUtilsTcsDuplicate(JC_NULL,pFileName) ;
	if(pmStr)
	{
		if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[1])
		{
			rCode = jdi_MsgDeleteMedia(g_pstJMmsAppInfo->hCurrMsg,g_pstJMmsAppInfo->ActivePageIndex,
									g_pstJMmsAppInfo->ActivePageMediaIds[1],0) ;
			g_pstJMmsAppInfo->ActivePageMediaIds[1] = 0;
		}
		if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[2])
		{
			rCode = jdi_MsgDeleteMedia(g_pstJMmsAppInfo->hCurrMsg,g_pstJMmsAppInfo->ActivePageIndex,
								g_pstJMmsAppInfo->ActivePageMediaIds[2],0);
			g_pstJMmsAppInfo->ActivePageMediaIds[2] = 0;
		}
		if(0 != g_pstJMmsAppInfo->ActivePageMediaIds[3])
		{
			rCode = jdi_MsgDeleteMedia(g_pstJMmsAppInfo->hCurrMsg,g_pstJMmsAppInfo->ActivePageIndex,
									g_pstJMmsAppInfo->ActivePageMediaIds[3],0) ;
			g_pstJMmsAppInfo->ActivePageMediaIds[3] = 0;
		}	
		rCode = jMMS_AddObjectMedia(pmStr,E_MEDIA_VIDEO,0);
	}
	mmi_trace(1,"jdd_log: VideoCallback AddObjectMedia rCode[%d]",rCode) ;	
	jMMS_HandleMediaError(rCode);
}


void MediaPickerAttachmentCallback(void*pFileName,int is_short)
{
	JC_RETCODE rCode = JC_OK;
	JC_CHAR * pmStr = JC_NULL;
	mmi_trace(1,"*****MediaPickerAttachCallback[%s]****",pFileName) ;
	
	if( pFileName == NULL )
	{
		GoBackHistory();
		return;
	}
	pmStr = jdi_CUtilsTcsDuplicate(JC_NULL,pFileName) ;
	if(pmStr)
	{
	rCode = jMMS_AddObjectMedia(pmStr,E_MEDIA_OTHERS,1);
	}
	mmi_trace(1,"*****MediaPickerAttachCallback AddObjectMedia[%d]****",rCode) ;	
	jMMS_HandleMediaError(rCode);
}

JC_RETCODE jMMS_AddObjectMedia(JC_CHAR *pFileName,EMediaType eMediaType,JC_BOOLEAN bIsAttachment)
{
	JC_RETCODE rCode = JC_OK;	
	rCode = AddObjectMedia(pFileName,eMediaType,g_pstJMmsAppInfo->ActivePageIndex,bIsAttachment);
	return rCode;
}

void ImagePicker(EMediaType eMediaType)
{
	FMGR_FILTER filter;
	FMGR_FILTER_INIT(&filter);	
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER_DOT);
	
	if(E_MEDIA_IMAGE == eMediaType)
	{
		FMGR_FILTER_SET_IMAGE(&filter);
		FMGR_FILTER_CLEAR(&filter,FMGR_TYPE_PNG);
		mmi_fmgr_select_path_and_enter (APP_JATAAYU_MMS, FMGR_SELECT_FILE|FMGR_SELECT_REPEAT, 
									filter,(S8*) L"root", MediaPickerImageCallback) ;
	}
	else if(E_MEDIA_AUDIO == eMediaType)
	{
		FMGR_FILTER_SET_AUDIO(&filter);		
		FMGR_FILTER_CLEAR(&filter,FMGR_TYPE_M4A);
		FMGR_FILTER_CLEAR(&filter,FMGR_TYPE_WMA);		
		
		mmi_fmgr_select_path_and_enter (APP_JATAAYU_MMS, FMGR_SELECT_FILE|FMGR_SELECT_REPEAT, 
										filter,(S8*) L"root", MediaPickerAudioCallback) ;
	}
	else if(E_MEDIA_VIDEO== eMediaType)
	{
		FMGR_FILTER_SET_VIDEO(&filter);		
				
		mmi_fmgr_select_path_and_enter (APP_JATAAYU_MMS, FMGR_SELECT_FILE|FMGR_SELECT_REPEAT, 
										filter,(S8*) L"root", MediaPickerVideoCallback) ;
	}
	else if(E_MEDIA_OTHERS == eMediaType)
	{
		//FMGR_FILTER_SET_ALL(&filter);	
		mmi_fmgr_set_mms_attachment_filter(&filter);

	mmi_fmgr_select_path_and_enter (APP_JATAAYU_MMS, FMGR_SELECT_FILE|FMGR_SELECT_REPEAT, 
										filter,(S8*) L"root", MediaPickerAttachmentCallback) ;
	}
}

#endif //__JATAAYU_APP__
