/***************************************************************************
*
* File Name : jdd_image.cpp
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
* Purpose			:	Implimentation for image control
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

#include <jddex_animationinterface.h>
#include "jddex_wingtech.h"


/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_image::jddex_image()
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
jddex_image::jddex_image(jddex_widget *poParentHwnd,JC_RECT *pstRect, 
				 JC_CONTROL_PROPERTIES* pstCtrlProperty,JC_BOOLEAN* pIsSucess, 
				 JC_CHAR* pmTempImageFile)
				:jddex_widget(poParentHwnd,pstRect, pstCtrlProperty)				
{
	m_eControlType = E_IMAGE;
	m_uiAnimHnd = 0;
	m_pvGifFrameBuff = NULL;
	m_uiAnimPlayInitiated = E_FALSE;
	JC_RECT stAbsRect;
	JC_RETCODE rCode = JC_OK;

	GetAbsRect(&stAbsRect);
	jc_memcpy(&m_stObjectInfo, &pstCtrlProperty->typeOfControl.object,sizeof(JC_OBJECT_INFO));

	m_pvDecodedBuff = NULL;
	
	m_eImageProperty = E_IMAGE_NORMAL;
	if(rCode == JC_OK)
	{
		*pIsSucess = E_TRUE;
	}
	else
	{
		*pIsSucess = E_FALSE;
	}
	m_pmTempImageFile = pmTempImageFile;
}

/**
 * @brief 		constructor
 * @param[in]	poParentHwnd   Handle of parent window
 * @param[in]	pstRect          control co-ordinates
 * @param[in]	pstImageInfo     pointer to object
 * @retval		None
 *
 * Calls the base class (widget class) constructor and initliazes 
 * class members
 */
jddex_image::jddex_image(jddex_widget *poParentHwnd,JC_RECT *pstRect, 
				 JC_OBJECT_INFO* pstImageInfo, EImageProperty  eImageProperty, 
				 JC_BOOLEAN* pIsSucess,JC_CHAR* pmTempImageFile )
				:jddex_widget(poParentHwnd,pstRect)				
{
	JC_RECT stAbsRect;
	JC_RETCODE rCode = JC_OK;
	m_eControlType = E_IMAGE;
	jc_memcpy(&m_stObjectInfo, pstImageInfo ,sizeof(JC_OBJECT_INFO));
	m_pvDecodedBuff = NULL;
	m_uiAnimHnd = 0;
	m_pvGifFrameBuff = NULL;
	m_uiAnimPlayInitiated = E_FALSE;
	GetAbsRect(&stAbsRect);
	m_eImageProperty = eImageProperty;

	if(rCode == JC_OK)
	{
		*pIsSucess = E_TRUE;
	}
	else
	{
		*pIsSucess = E_FALSE;
	}
	m_pmTempImageFile = pmTempImageFile;
}


/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
jddex_image::~jddex_image()
{
	if ( JC_NULL != m_pvDecodedBuff )
	{
		jdd_MemFree(m_pvDecodedBuff) ;
		m_pvDecodedBuff = JC_NULL;
	}
	if(m_pmTempImageFile)
	{
		jdd_FSDelete(NULL,  m_pmTempImageFile);
		jdd_MemFree(m_pmTempImageFile);
	}
	if(m_uiAnimHnd)
	{
		gdi_image_stop_animation((gdi_handle)m_uiAnimHnd);
		m_uiAnimHnd = 0;
	}
}

/**
 * @brief 		dispalys a image control
 * @retval		None
 * displays the image based on its state
 */
void jddex_image::Show()
{
 	JC_RECT stAbsRect;
	JC_RECT stTempRect;

	GetAbsRect(&stAbsRect);
	mmi_trace(1,(PCSTR)"jddex_image::Show x[%d]y[%d]w[%d]h[%d] ", \
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
	jddex_DrawFormattedImage(m_stObjectInfo, &stAbsRect, E_IMAGE_NORMAL, \
							m_pmTempImageFile, &m_uiAnimHnd);
}

/**
 * @brief 		returns the image property
 * @retval		None
 * returns the image property set while creating image control
 */
EImageProperty jddex_image::GetImageProperty()
{
	return m_eImageProperty;
}


