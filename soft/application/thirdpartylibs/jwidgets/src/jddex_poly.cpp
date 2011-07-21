/***************************************************************************
*
* File Name : jdd_poly.cpp
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
* Purpose			:	Implimentation for polygon control
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


#include "jddex_poly.h"

/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_poly::jddex_poly()
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
jddex_poly::jddex_poly(jddex_widget *poParentHwnd,JC_RECT *pstRect, JC_CONTROL_PROPERTIES* pstCtrlProperty)
				:jddex_widget(poParentHwnd,pstRect, pstCtrlProperty)
{

	m_eControlType = E_POLY;
}

jddex_poly::~jddex_poly()
{

}
/**
 * @brief 		dispalys a polygon
 * @retval		None
 * displays a polygon
 */
void jddex_poly::Show()
{

}
