/***************************************************************************
*
* File Name : jdd_button.cpp
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
* Purpose			:	Implimentation for button control
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
#include "jddex_button.h"
#include "jddex_platform.h"
#include "jddex_wnddefaults.h"
#include "jddex_int.h"

/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_button::jddex_button()
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
jddex_button::jddex_button(jddex_widget *poParentHwnd,JC_RECT *pstRect, JC_CONTROL_PROPERTIES* pstCtrlProperty)
				:jddex_widget(poParentHwnd,pstRect, pstCtrlProperty)
{
	m_stControlStyle.uiFontSize = 10 ;

	m_eControlType = E_BUTTON;
	m_uiIsSoftKey = pstCtrlProperty->eEventCode;
}

/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
jddex_button::~jddex_button()
{

}

/**
 * @brief 		dispalys a button
 * @retval		None
 * displays the button based on its state
 */
void jddex_button::Show()
{
	JC_RECT stAbsRect;
//	JC_RECT stAllignRect;
	GetAbsRect(&stAbsRect);
	JC_INT32 iBorderColor = JDDEX_DEFAULT_BGCOLOR ;
	JC_INT32 iButtonColor = m_stControlStyle.iBackgroundColor;

	if( iButtonColor < 0)
		iButtonColor = JDDEX_DEFAULT_BUTTONCOLOR;



	if(m_bIsFocussed)
	{
		iBorderColor = JDDEX_DEFAULT_BLUE ;
	}

	mmi_trace(1,(PCSTR)"jddex_button::Show");
	//hack for soft key display
	if(m_uiIsSoftKey == 0xFFFF)
	{
		jddex_PlatformDrawRect(&stAbsRect, iBorderColor, JDDEX_DEFAULT_BUTTONCOLOR) ;
		jddex_PlatformDrawText(m_pszText, stAbsRect, m_stControlStyle, E_TRUE);
	
	}
	else
	{
	   mmi_trace(1,(PCSTR)"jddex_button::Show x[%d] y[%d] dx[%d] dy[%d]",
				stAbsRect.iLeft,stAbsRect.iTop,stAbsRect.uiWidth,stAbsRect.uiHeight);
		jddex_DrawFormattedRect(&stAbsRect, 
				 iBorderColor, iButtonColor, JDDEX_BUTTON_BORDER_WIDTH);

		jddex_FormattedDrawTextEx(m_pszText ,stAbsRect, 
				 m_stControlStyle, E_JDDEX_CENTRE_ALLIGN|E_JDDEX_MIDDLE_ALLIGN);
	}
}


