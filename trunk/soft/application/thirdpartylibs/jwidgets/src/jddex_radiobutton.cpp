/***************************************************************************
*
* File Name : jdd_radiobutton.cpp
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
* Purpose			:	Implimentation for radiobutton control
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

#include "mmi_trace.h"

#include "jddex_platformdefines.h"
#include "jddex_radiobutton.h"
#include "jddex_platform.h"
#include "jddex_wnddefaults.h"
#include "jddex_int.h"

/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_radiobutton::jddex_radiobutton()
{

}
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
jddex_radiobutton::jddex_radiobutton(jddex_widget *poParentHwnd,JC_RECT *pstRect, JC_CONTROL_PROPERTIES* pstCtrlProperty)
				:jddex_widget(poParentHwnd,pstRect, pstCtrlProperty)
{
	m_eControlType = E_RADIO_BOX;
	m_stRadioBox.uiGroupID = pstCtrlProperty->typeOfControl.radioBox.uiGroupID;
	m_stRadioBox.eStatus = pstCtrlProperty->typeOfControl.radioBox.eStatus;
}

/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
jddex_radiobutton::~jddex_radiobutton()
{

}
/**
 * @brief 		sets the radio button status
 * @param[in]	E_UNCHECKED/E_UNCHECKED
 * 
 */
void jddex_radiobutton:: SetStatus(E_OPTIONBOX_STATUS eOptionStatus)
{
	m_stRadioBox.eStatus = eOptionStatus;
	Show();
	jddex_PlatformUpdateScreen();
}

/**
 * @brief 		gets the radio button status
 * @param[out]	E_UNCHECKED/E_UNCHECKED
 * 
 */
void jddex_radiobutton::GetStatus(E_OPTIONBOX_STATUS*  eOptionStatus)
{
	 *eOptionStatus = m_stRadioBox.eStatus;
}

/**
 * @brief 		dispalys a radio button
 * @retval		None
 * displays a polygon
 */
void jddex_radiobutton::Show()
{
	JC_RECT stAbsRect,stRect;
	//U8 * imgptr = JC_NULL;
	GetAbsRect(&stAbsRect);	
	
	JC_INT32 iBorderColor ;//,iFillColor = JDDEX_DEFAULT_BLUE;
	if(m_bIsFocussed)
	{
		iBorderColor = JDDEX_DEFAULT_BLUE;		
		jddex_DrawFormattedRect(&stAbsRect, iBorderColor, JDDEX_DEFAULT_WHITE, 1);
	}
	stRect = stAbsRect;
	stRect.iLeft += 2;
	stRect.iTop += 2;
	stRect.uiWidth -= 4;
	stRect.uiHeight -= 4;

	if(m_stRadioBox.eStatus)	
	{
		//imgptr = (U8*)GetImage(IMG_ID_JATAAYU_SELECTED_RADIOBUTTON_GIF);
		//rCode = gdi_image_draw(stAbsRect.iLeft,stAbsRect.iTop,imgptr);	
		jddex_DrawFormattedRadioIcon(&stRect,IMG_ID_JATAAYU_SELECTED_RADIOBUTTON_GIF);
	}
	else
	{
		//imgptr = (U8*)GetImage(IMG_ID_JATAAYU_UNSELECTED_RADIOBUTTON_GIF);
		//rCode = gdi_image_draw(stAbsRect.iLeft,stAbsRect.iTop,imgptr);	
		jddex_DrawFormattedRadioIcon(&stRect,IMG_ID_JATAAYU_UNSELECTED_RADIOBUTTON_GIF);		
	}	
}

