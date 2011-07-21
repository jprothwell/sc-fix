/***************************************************************************
*
* File Name : jdd_audio.cpp
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
* Purpose			:	Implimentation for audio control
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
#include "jddex_int.h"
#include "jddex_audio.h"
#include "jddex_platform.h"
#include "jddex_wnddefaults.h"
#include "jddex_audiointerfaces.h"

#define ALTOBJECT_RECT_WIDTH  10
#define ALTOBJECT_RECT_HEIGHT 10
#define ALTOBJECT_RECT_MARGIN  5


/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */

/**
 * @brief 		constructor
 * @param[in]	pst_ParentHwnd   Handle of parent window
 * @param[in]	pstRect          control co-ordinates
 * @param[in]	pstCtrlProperty  pointer to  JC_CONTROL_PROPERTIES
 * @retval		None
 *
 * Calls the base class (widget class) constructor and initliazes 
 * class members
 */
jddex_audio::jddex_audio(jddex_widget *poParentHwnd,JC_RECT *pstRect, 
				JC_CONTROL_PROPERTIES* pstCtrlProperty, JC_CHAR* pmTempFile)
				:jddex_widget(poParentHwnd,pstRect, pstCtrlProperty)
{

	m_eControlType = E_OBJECT;
	m_pmTempFile = pmTempFile;
	jc_memcpy(&m_stObjectInfo, &pstCtrlProperty->typeOfControl.object,sizeof(JC_OBJECT_INFO));
}

/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
jddex_audio::~jddex_audio()
{
	if(m_pmTempFile)
	{
		jdd_FSDelete(NULL,  m_pmTempFile);
		jdd_MemFree(m_pmTempFile);
	}
}



void jddex_audio::HandleAction(E_OBJECT_ACTION		eAction)
{
	switch(eAction)
	{
	 case E_OBJECT_PLAY:
		 {
			 m_pvAudioHandle = jddex_PlatformPlayAudio(&m_stObjectInfo, m_pmTempFile);
		 break;
		 }
 
	 case E_OBJECT_STOP:
		 {
			// if(m_pvAudioHandle)
			   jddex_PlatformStopAudio(&m_stObjectInfo, m_pvAudioHandle,m_pmTempFile);
		 }
	 break;
 
	 case E_OBJECT_PAUSE:
		 {
			jddex_PlatformPauseAudio(&m_stObjectInfo,m_pvAudioHandle,m_pmTempFile);
		 }
	 break;

	  case E_OBJECT_RESUME:
		  {
			jddex_PlatformResumeAudio(&m_stObjectInfo, m_pvAudioHandle,m_pmTempFile);
		  }
	 break;
	 default:
	 	break ;
		
	}

}

/**
 * @brief 		dispalys a object text
 * @retval		None
 * displays the object text
 */
void jddex_audio::Show()
{
	JC_RECT stAbsRect;
	GetAbsRect(&stAbsRect);

	if(stAbsRect.uiWidth == 0 || stAbsRect.uiHeight== 0)
	{
	  return;
	}


	if(ALTOBJECT_RECT_WIDTH < stAbsRect.uiWidth && ALTOBJECT_RECT_HEIGHT < stAbsRect.uiHeight )
	{
		jddex_DrawFormattedRect(&stAbsRect,JDDEX_DEFAULT_BLACK, JDDEX_DEFAULT_RED, JDDEX_IMAGE_BORDER_WIDTH);
	}
	stAbsRect.iLeft += ALTOBJECT_RECT_WIDTH+ALTOBJECT_RECT_MARGIN;
//	jddex_FormattedDrawText(m_pszText, stAbsRect, m_stControlStyle);

}


