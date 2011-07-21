/***************************************************************************
*
* File Name : jdd_label.cpp
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
* Purpose			:	Implimentation for label control
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
#include "jddex_label.h"
#include <jddex_platform.h>

#include "jdd_logapi.h"

/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_label::jddex_label()
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
jddex_label::jddex_label(jddex_widget *poParentHwnd,JC_RECT *pstRect, JC_CONTROL_PROPERTIES* pstCtrlProperty)
				:jddex_widget(poParentHwnd,pstRect, pstCtrlProperty)
{
	m_eTextStyle = pstCtrlProperty->typeOfControl.textBox.etextStyle;	

	//this->pst_ParentHwnd = pst_ParentHwnd;
	m_eControlType = E_TEXT_BOX;
}

/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
jddex_label::~jddex_label()
{

}

/**
 * @brief 		dispalys a label
 * @retval		None
 * displays the label based on its state
 */
void jddex_label::Show()
{
	JC_RECT stAbsRect, stParentAbsRect= {0,};

	GetAbsRect(&stAbsRect);
	GetParentRect(&stParentAbsRect);
	
	jddex_FormattedDrawText(m_pszText ,stAbsRect, stParentAbsRect, m_stControlStyle);
	//jddex_FormattedDrawTextEx(m_pszText ,stAbsRect, m_stControlStyle,E_JDDEX_LEFT_ALLIGN);
	//jddex_LimitLabel(m_pszText, stAbsRect, stParentAbsRect, m_stControlStyle);
}



