/***************************************************************************
*
* File Name : jdd_video.cpp
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
* THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND
* AND ANY AND ALL REPRESENTATION
*
***************************************************************************
*
*
* File Description
* ----------------
*
* Purpose			:	Implimentation for video control
*						
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
* 1. Modified By	:
*    Modified Date	:
*	 Purpose		:
*
*
*
*
***************************************************************************/

#include "jddex_platformdefines.h"
#include "jddex_image.h"
#include "jddex_platform.h"
#include "jddex_int.h"
#include "jddex_wnddefaults.h"
#include "jddex_platform.h"
#include "jddex_video.h"
#include <jddex_animationinterface.h>
#include "jddex_wingtech.h"
#include "jwap_interface.h"


#ifdef __cplusplus
extern "C" {
#endif
JC_UINT32 jwap_GetPrimaryWindow();
void PlayFinishCallBackFunc(void* dummy);
JC_RETCODE cs_Deletevideo ();
JC_RETCODE cs_Stopvideo ();
JC_RETCODE cs_Playvideo ( int x , int y , int dx, int dy, PsFuncPtr PlayFinishCallBackFunc);
JC_RETCODE  CS_CreateVideoHandleFromFile(JC_CHAR *pmPath, JC_UINT16 *dx, JC_UINT16 *dy);
JC_RETCODE  CS_CreateVideoHandleFromBuffer(JC_INT8*szbuffer, int len, JC_INT8* szContentType);
JC_RETCODE jddex_PlatformVideo(JC_OBJECT_INFO		m_stObjectInfo,JC_RECT * pstDispRect, 
									JC_RECT * pstClippedRect,JC_BOOLEAN bClip);
JC_RETCODE cs_GetCurrentTime(U64 *time);
JC_RETCODE cs_RelocateAndDisplay ( int x , int y , int dx, int dy, U64 time);
JC_RETCODE cs_Relocate ( int x , int y , int dx, int dy);


#ifdef __cplusplus
}
#endif

/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_video::jddex_video()
{

}

/**
 * @brief 		constructor
 * @param[in]	poParentHwnd   Handle of parent window
 * @param[in]	pstRect          control co-ordinates
 * @param[in]	pstCtrlProperty  pointer to  JC_CONTROL_PROPERTIES
 * @retval		None
 *
 * Calls the base class (widget class) constructor and initliazes 
 * class members
 */
jddex_video::jddex_video(jddex_widget *poParentHwnd,JC_RECT *pstRect, 
				 JC_CONTROL_PROPERTIES* pstCtrlProperty)
				:jddex_widget(poParentHwnd,pstRect, pstCtrlProperty)				
{
	m_eControlType = E_VIDEO;
	JC_RECT stAbsRect;
	U16 width = 0, height = 0;	
	JC_RETCODE rCode = JC_OK;
	m_bIsVideoPlaying = E_FALSE;
	m_bIsVideoHandleDeleted = E_FALSE;
	
	mmi_trace(1,(PCSTR)"jddex_video: jddex_video called ht[%d] wt[%d] type[%d]", \
				pstRect->uiHeight, pstRect->uiWidth, m_stObjectInfo.bIsFileContent);
	GetAbsRect(&stAbsRect);
	jc_memcpy(&m_stObjectInfo, &pstCtrlProperty->typeOfControl.object,sizeof(JC_OBJECT_INFO));	

	{
		JC_INT8	pcFName [64] = {0, } ;		
		jdi_CUtilsStrTcsNCpy (m_stObjectInfo.uBuffer.pmFileName, 63, pcFName) ;
		mmi_trace(1, (PCSTR)"jddex_video: jddex_video file name[%s]", pcFName);
	}	
	
	CreateVideoHandle();
	
	mmi_trace(1,(PCSTR)"jddex_video: jddex_video handle rCode[%d] len[%d] type[%s]", \
				rCode, m_stObjectInfo.uiObjectLen, m_stObjectInfo.psMimeType);
}


/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
jddex_video::~jddex_video()
{	
	DeleteVideoHandle();
}

/**
 * @brief 		dispalys a image control
 * @retval		None
 * displays the image based on its state
 */
void jddex_video::Show()
{
 	JC_RECT stAbsRect;
	JC_RECT stTempRect;
	U64 time =0;
	GetAbsRect(&stAbsRect);
	mmi_trace(1,(PCSTR)"jddex_video::Show x[%d]y[%d]w[%d]h[%d] ", \
		stAbsRect.iLeft,stAbsRect.iTop,
		stAbsRect.uiWidth,stAbsRect.uiHeight);
	
	if(m_bIsSelectable && m_bIsFocussed)
	{
		stTempRect = stAbsRect ;
		stTempRect.iLeft -= 1 ;
		stTempRect.iTop -= 1 ;
		stTempRect.uiWidth += 2 ;
		stTempRect.uiHeight += 2 ;

		jddex_DrawFormattedRect(&stTempRect,JDDEX_DEFAULT_BLUE, 
								JDDEX_DEFAULT_WHITE,JDDEX_IMAGE_BORDER_WIDTH );
	}
	// in case video is deleted in exit handler of PMMI screen re-create it
	RecreateVideoHandle();
	
    mmi_trace(1,(PCSTR)"jddex_video::m_bIsVideoHandleCreated %d", m_bIsVideoHandleCreated);
	if(E_FALSE == m_bIsVideoHandleCreated)
	{
		U8 * imgptr = JC_NULL;		
		imgptr = (U8*)GetImage(IMG_MMS_ALT_ICON);		
		gdi_image_draw_resized(stAbsRect.iLeft,stAbsRect.iTop, \
						stAbsRect.uiWidth, stAbsRect.uiHeight,imgptr);
		return;
	}
		
	
	if(m_bIsVideoPlaying == E_FALSE)
	{
		cs_GetCurrentTime(&time);
		mmi_trace(1,(PCSTR)"jddex_video::relocate and display x[%d]y[%d]w[%d]h[%d] ", \
		stAbsRect.iLeft,stAbsRect.iTop,
		stAbsRect.uiWidth,stAbsRect.uiHeight);
		
		cs_RelocateAndDisplay ( stAbsRect.iLeft, stAbsRect.iTop, stAbsRect.uiWidth, stAbsRect.uiHeight, time);		
	}
	else
	{	
	   mmi_trace(1,(PCSTR)"jddex_video::relocate  x[%d]y[%d]w[%d]h[%d] ", \
		stAbsRect.iLeft,stAbsRect.iTop,
		stAbsRect.uiWidth,stAbsRect.uiHeight);
		cs_Relocate( stAbsRect.iLeft, stAbsRect.iTop, stAbsRect.uiWidth, stAbsRect.uiHeight);
	}
}

void jddex_video::HandleAction(E_OBJECT_ACTION eAction)
{
	mmi_trace(0, (PCSTR)"jddex_video: Handleaction type[%d]", eAction);
	switch(eAction)
	{
	 case E_OBJECT_PLAY:
	// case E_OBJECT_RESUME:
		 {
			JC_RETCODE rCode;
			JC_RECT stAbsRect;
			JDDEX_DEVICE_CHAR stDeviceChar;
			JC_RECT stRenderingRect;
			GetAbsRect(&stAbsRect);
			m_bIsVideoPlaying = E_TRUE;
			
			jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);

			stRenderingRect.iLeft = stDeviceChar.uiLeft;
			stRenderingRect.iTop = stDeviceChar.uiTop;
			stRenderingRect.uiWidth = stDeviceChar.uiRenderingWidth;
			stRenderingRect.uiHeight = stDeviceChar.uiRenderingHeight;
		
			jddex_PlatformEraseRect(&stRenderingRect);
			rCode = jddex_DrawFormattedVideo(m_stObjectInfo, &stAbsRect, m_bIsVideoHandleCreated );
		}
 	break;

	//case E_OBJECT_PAUSE:
	case E_OBJECT_STOP:
		 {
			JDDEX_DEVICE_CHAR stDeviceChar;
			JC_BOOLEAN		bIsSecondaryMMIUp ;
			m_bIsVideoPlaying = E_FALSE;

			if(E_FALSE == m_bIsVideoHandleCreated)
			return;
			
			cs_Stopvideo();
			/*
			jddex_GetDeviceProp((E_APP_TYPE) 0, &stDeviceChar);
			jddex_IsSecondaryMMIUp(&bIsSecondaryMMIUp);
			mmi_trace (1, (PCSTR)"widget_log: IsSecondaryMMIUp returned %d", bIsSecondaryMMIUp) ;

			if (!bIsSecondaryMMIUp)
			{
				gui_lock_double_buffer();

				gui_reset_clip();
				gui_push_clip();	
				gui_set_clip(stDeviceChar.uiLeft, stDeviceChar.uiTop, 
					stDeviceChar.uiLeft+stDeviceChar.uiRenderingWidth , 
					stDeviceChar.uiTop+stDeviceChar.uiRenderingHeight);

				Show();


				gui_pop_clip();		
				gui_unlock_double_buffer();
								
				gui_BLT_double_buffer(stDeviceChar.uiLeft, stDeviceChar.uiTop, 
					stDeviceChar.uiLeft+stDeviceChar.uiRenderingWidth , 
					stDeviceChar.uiTop+stDeviceChar.uiRenderingHeight);
			}*/
		
	     }
	 break;
 
	case E_OBJECT_PAUSE:
		 {
			DeleteVideoHandle();
		 }
	 break;

	 case E_OBJECT_RESUME:
		  {
			RedrawVideoControl();
		  }
	 break;
	 default:
	 	break ;
		
	}

}

void jddex_video::RecreateVideoHandle()
{
	mmi_trace(0, (PCSTR)"jddex_video: RecreateVideoHandle");
	if(m_bIsVideoHandleDeleted == E_TRUE)
	{
		mmi_trace(0, (PCSTR)"jddex_video: CreateVideoHandle invoked");
		CreateVideoHandle();
		m_bIsVideoHandleDeleted = E_FALSE;
	}

}

void jddex_video::CreateVideoHandle()
{
	JC_RETCODE rCode = JC_OK;
	U16 width = 0, height = 0;	
	
	if(m_stObjectInfo.bIsFileContent == E_TRUE)
		rCode = CS_CreateVideoHandleFromFile(m_stObjectInfo.uBuffer.pmFileName, &width, &height);
	else
		rCode = CS_CreateVideoHandleFromBuffer((JC_INT8*)m_stObjectInfo.uBuffer.pucBuffer, \
						m_stObjectInfo.uiObjectLen, m_stObjectInfo.psMimeType);
	if(JC_OK == rCode)
		m_bIsVideoHandleCreated = E_TRUE;	
	else
		m_bIsVideoHandleCreated = E_FALSE;

}



void jddex_video::DeleteVideoHandle()
{
	mmi_trace(0, (PCSTR)"jddex_video: DeleteVideoHandle");
	if(m_bIsVideoHandleDeleted == E_FALSE)
	{
		  m_bIsVideoHandleDeleted = E_TRUE;
		  cs_Deletevideo();
		  mmi_trace(0, (PCSTR)"jddex_video: Deleted Video");
	}
}


void jddex_video::RedrawVideoControl()
{
	JDDEX_DEVICE_CHAR stDeviceChar;
	JC_BOOLEAN		bIsSecondaryMMIUp ;

	jddex_GetDeviceProp((E_APP_TYPE) 0, &stDeviceChar);
	jddex_IsSecondaryMMIUp(&bIsSecondaryMMIUp);
	mmi_trace (1, (PCSTR)"jddex_video:RedrawVideoControl IsSecondaryMMIUp returned %d ", bIsSecondaryMMIUp) ;

	if (!bIsSecondaryMMIUp)
	{
	gui_lock_double_buffer();

	gui_reset_clip();
	gui_push_clip();	
	gui_set_clip(stDeviceChar.uiLeft, stDeviceChar.uiTop, 
		stDeviceChar.uiLeft+stDeviceChar.uiRenderingWidth , 
		stDeviceChar.uiTop+stDeviceChar.uiRenderingHeight);

	Show();


	gui_pop_clip(); 	
	gui_unlock_double_buffer();
					
	gui_BLT_double_buffer(stDeviceChar.uiLeft, stDeviceChar.uiTop, 
		stDeviceChar.uiLeft+stDeviceChar.uiRenderingWidth , 
		stDeviceChar.uiTop+stDeviceChar.uiRenderingHeight);
	}

}


void jddex_video::SetVideoPlayStatus(JC_BOOLEAN bIsPlaying)
{
	m_bIsVideoPlaying = bIsPlaying;
}

#ifdef __cplusplus
extern "C" {
#endif

void DestroyVideoHandle()
{
	jddex_widget * poWidget = JC_NULL;
		jddex_video * poVideo = JC_NULL ;
	
	mmi_trace(TRUE, (PCSTR)"jddex_video: Func: %s ", __FUNCTION__);
	jddex_window * poWindow = (jddex_window *)jwap_GetPrimaryWindow();

   mmi_trace(0, (PCSTR)"jddex_video: destroy handle window %d ", poWindow );
	if(poWindow)
	{
		poWindow->GetControlID(&poWidget, E_VIDEO);

		poVideo = (jddex_video *)poWidget;
		if(poVideo)
		{
			poVideo->DeleteVideoHandle();
		}
	}

}
#ifdef __cplusplus
}
#endif


void PlayFinishCallBackFunc(void* dummy)
{
	jddex_widget * poWidget = JC_NULL;
	jddex_video * poVideo = JC_NULL ;

	mmi_trace(TRUE, (PCSTR)"jddex_video: Func: %s ", __FUNCTION__);
	jddex_window * poWindow = (jddex_window *)jwap_GetPrimaryWindow();


	cs_Stopvideo();
	poWindow->GetControlID(&poWidget, E_VIDEO);

	poVideo = (jddex_video *)poWidget;
	if(poVideo)
	{
		mmi_trace(TRUE, (PCSTR)"jddex_video:calling  RedrawVideoControl ");
		poVideo->SetVideoPlayStatus(E_FALSE);
		poVideo->RedrawVideoControl();
	}
	else
	{
		mmi_trace(TRUE, (PCSTR)"jddex_video:NULL ");
	}
}

JC_RETCODE jddex_PlatformDrawVideo(JC_OBJECT_INFO		m_stObjectInfo,JC_RECT * pstDispRect, 
									JC_RECT * pstClippedRect, JC_BOOLEAN bIsVideoHandleCreated)
{
	JC_RETCODE rCode;
	JDDEX_DEVICE_CHAR stDeviceChar;
	JC_BOOLEAN		bIsSecondaryMMIUp ;
	
	jddex_GetDeviceProp((E_APP_TYPE) 0, &stDeviceChar);
	jddex_IsSecondaryMMIUp(&bIsSecondaryMMIUp);
	mmi_trace (1, (PCSTR)"jddex_video: IsSecondaryMMIUp returned %d", bIsSecondaryMMIUp) ;
 
	if (!bIsSecondaryMMIUp)
	{
		gui_lock_double_buffer();
		
		gui_reset_clip();
		gui_push_clip();	
		gui_set_clip(stDeviceChar.uiLeft, stDeviceChar.uiTop, 
			stDeviceChar.uiLeft+stDeviceChar.uiRenderingWidth , 
			stDeviceChar.uiTop+stDeviceChar.uiRenderingHeight);

		if(bIsVideoHandleCreated)
		{
			rCode = cs_Playvideo (pstDispRect->iLeft, pstDispRect->iTop, pstDispRect->uiWidth, \
								pstDispRect->uiHeight, PlayFinishCallBackFunc);
		}
		else
		{
			U8 * imgptr = JC_NULL;		
			imgptr = (U8*)GetImage(IMG_MMS_ALT_ICON);		
			gdi_image_draw_resized(pstDispRect->iLeft, pstDispRect->iTop, \
							pstDispRect->uiWidth, pstDispRect->uiHeight,imgptr);
		}		
		gui_pop_clip();		
		gui_unlock_double_buffer();
						
		gui_BLT_double_buffer(stDeviceChar.uiLeft, stDeviceChar.uiTop, 
			stDeviceChar.uiLeft+stDeviceChar.uiRenderingWidth , 
			stDeviceChar.uiTop+stDeviceChar.uiRenderingHeight); 		
	}	
	return rCode;
}

