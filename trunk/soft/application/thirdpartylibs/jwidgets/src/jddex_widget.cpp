/***************************************************************************
*
* File Name : jdd_widget.cpp
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
* Purpose			:	Implimentation for widget class
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
#include <jddex_widget.h>
#include <jddex_int.h>
#include <jddex_platform.h>
#include <jddex_wnddefaults.h>


/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_widget::jddex_widget()
{

}
/**
 * @brief 		constructor
 * @param[in]	pst_ParentHwnd   Handle of parent window
 * @param[in]	pstRect          control co-ordinates
 * @param[in]	pstCtrlProperty  pointer to  JC_CONTROL_PROPERTIES
 * @retval		None
 *
 * initliazes class members. invoked by all inherited class constructors
 */
jddex_widget::jddex_widget(jddex_widget *poParentHwnd,JC_RECT *pstRect,
						   JC_CONTROL_PROPERTIES* pstCtrlProperty)
{
	jc_memcpy (&m_stRect, pstRect, sizeof(JC_RECT)) ;
	m_bIsSelectable = pstCtrlProperty->bIsSelectable ;
	m_bIsDisabled = pstCtrlProperty->bIsDisabled;
	m_stControlStyle = pstCtrlProperty->controlStyle ;
	m_poParentHwnd = poParentHwnd;
	m_pszText = JC_NULL;
	if(pstCtrlProperty->psCaptionValue)
	{
		m_pszText = jdi_CUtilsTcsDuplicate (JC_NULL,
							pstCtrlProperty->psCaptionValue);
	}
	m_bIsVisible = E_TRUE;
	m_bIsContainer = E_FALSE;
	m_bIsFocussed = E_FALSE;

	m_bIsVisible = E_TRUE;
//	m_eControlType = (EControlType)0xFF;


	//m_stControlStyle.iForegroundColor = JDDEX_DEFAULT_COLOR;

	//m_stControlStyle.iBackgroundColor = JDDEX_DEFAULT_COLOR;
}

/**
 * @brief 		constructor
 * @param[in]	pst_ParentHwnd   Handle of parent window
 * @param[in]	pstRect          control co-ordinates
 * @retval		None
 *
 * initliazes class members. invoked by all inherited class constructors
 */
jddex_widget::jddex_widget(jddex_widget *poParentHwnd, JC_RECT *pstRect)
{
	jc_memcpy (&m_stRect, pstRect, sizeof(JC_RECT)) ;
	m_poParentHwnd = poParentHwnd;
	m_pszText = JC_NULL;
	m_bIsVisible = E_TRUE;
	m_bIsContainer = E_FALSE;
	m_bIsFocussed = E_FALSE;

	m_bIsVisible = E_TRUE;
//	eControlType = (EControlType)0xFF;


	//m_stControlStyle.iForegroundColor = JDDEX_DEFAULT_COLOR;

	//m_stControlStyle.iBackgroundColor = JDDEX_DEFAULT_COLOR;
}

/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
jddex_widget::~jddex_widget()
{
	if(m_pszText)
	{
		jdd_MemFree (m_pszText);
		m_pszText = JC_NULL;
	}
}

void jddex_widget::Show()
{

}

/**
 * @brief 		sets the control colors
 * @param[in]	iBgColor   background color
 * @param[in]	iFgColor   foreground color
 * @retval		None
 *
 * sets the control background/foreground colors for a control
 */
void jddex_widget::SetColors(JC_INT32 iBgColor, JC_INT32 iFgColor)
{
	m_stControlStyle.iForegroundColor = iFgColor;
	m_stControlStyle.iBackgroundColor = iBgColor;
}

void jddex_widget::SetFgColor(JC_INT32 iFgColor)
{
	m_stControlStyle.iForegroundColor = iFgColor;
}

void jddex_widget::SetBgColor(JC_INT32 iBgColor)
{
	m_stControlStyle.iBackgroundColor = iBgColor;
}

/**
 * @brief 		Gets the control colors
 * @param[out]	piBgColor   background color
 * @param[out]	piFgColor   foreground color
 * @retval		None
 *
 * sets the control background/foreground colors for a control
 */
void jddex_widget::GetColors(JC_INT32 *piBgColor, JC_INT32 *piFgColor)
{
	*piFgColor = m_stControlStyle.iForegroundColor ;
	*piBgColor = m_stControlStyle.iBackgroundColor ;
}

/**
 * @brief 		Gets the control foreground color
 * @param[out]	piBgColor   background color
 * @retval		None
 *
 * Gets the control foreground for a control
 */
void jddex_widget::GetFgColor(JC_INT32 *piFgColor)
{
	*piFgColor = m_stControlStyle.iForegroundColor ;
}

/**
 * @brief 		Gets the control background color
 * @param[out]	piBgColor   background color
 * @retval		None
 *
 * sets the control background color for a control
 */
void jddex_widget::GetBgColor(JC_INT32 *piBgColor)
{
	*piBgColor = m_stControlStyle.iBackgroundColor ;
}

/**
 * @brief 		Sets the default background color
 * @retval		None
 *
 * sets the default background color for a control
 */
void jddex_widget::SetDefaultBgColor()
{
	m_stControlStyle.iForegroundColor = JDDEX_DEFAULT_BLACK;
}

/**
 * @brief 		Sets the default foreground color
 * @retval		None
 *
 * sets the default foreground color for a control
 */
void jddex_widget::SetDefaultFgColor()
{
	m_stControlStyle.iBackgroundColor = JDDEX_DEFAULT_WHITE;
}

/**
 * @brief 		sets the default control colors
 * @retval		None
 *
 * sets the deafult background/foreground colors for a control
 */
void jddex_widget::SetDefaultColors()
{
	m_stControlStyle.iForegroundColor = JDDEX_DEFAULT_BLACK;
	m_stControlStyle.iBackgroundColor = JDDEX_DEFAULT_WHITE;
}

/**
 * @brief 		sets focus to a control
 * @retval		None
 *
 * sets focus to a control and control acts accordingly when dispalyed
 */
void jddex_widget::SetFocus()
{
	m_bIsFocussed = E_TRUE;
}

/**
 * @brief 		resets focus to a control
 * @retval		None
 *
 * resets focus to a control and control acts accordingly when dispalyed
 */
void jddex_widget::ResetFocus()
{
	m_bIsFocussed = E_FALSE;
}

/**
 * @brief 		sets recatngle co-ordiantes to a control
 * @param[in]	pstRect   pointer to JC_RECT
 * @retval		None
 *
 * sets recatngle co-ordiantes to a control
 */
void jddex_widget::SetRect(JC_RECT*	pstRect)
{
	jc_memcpy (&m_stRect, pstRect, sizeof(JC_RECT)) ;
}

/**
 * @brief 		gets recatngle co-ordiantes to a control
 * @param[out]	pstRect   pointer to JC_RECT
 * @retval		None
 *
 * gets recatngle co-ordiantes to a control
 */
void jddex_widget::GetRect(JC_RECT*  pstRect)
{
	jc_memcpy (pstRect, &m_stRect, sizeof(JC_RECT)) ;
}

/**
 * @brief 		sets visibility sattus to a control
 * @param[in]	bVisible   visible/not-visible
 * @retval		None
 *
 * sets visibility of a control
 */
void jddex_widget::SetVisibilty(JC_BOOLEAN bVisible)
{
	m_bIsVisible = bVisible;
}


/**
 * @brief 		gets visibility sattus of a control
 * @param[in]	bVisible   visible/not-visible
 * @retval		None
 *
 * sets visibility of a control
 */
JC_BOOLEAN jddex_widget::GetVisibiltyStatus()
{
	return m_bIsVisible ;
}
/**
 * @brief 		enables/disables  a control
 * @param[in]	bDisabled   enable/disable
 * @retval		None
 *
 * sets visibility of a control
 */
void jddex_widget::Disable(JC_BOOLEAN bDisabled)
{
	m_bIsDisabled = bDisabled;
}

/**
 * @brief 		virtaul event handler function to be overloaded by derived classes
 * @param[in]	hWnd		control handle
 * @param[in]	eEventType   event type
 * @param[in]	pvEventInfo   event data
 * @retval		E_FALSE
 *
 * sets visibility of a control
 */

JC_RETCODE jddex_widget::EventHandler(JC_UINT32 hWnd, JC_INT32 eEventType,
								   void	*pvEventInfo)
{
	return JC_ERR_NOT_IMPLEMENTED ;
}

void jddex_widget::HandleAction(E_OBJECT_ACTION		eAction)
{
	return  ;
}


/**
 * @brief 		sets text to a control
 * @param[in]	pszText   text to be set
 * @retval		None
 *
 * sets text to a control
 */
void jddex_widget::SetText(JC_CHAR *pszText)
{
	if(m_pszText)
	{
		jdd_MemFree (m_pszText);
		m_pszText = JC_NULL;
	}
	m_pszText = jdi_CUtilsTcsDuplicate (JC_NULL, pszText);

}

/**
 * @brief 		gets text to a control
 * @param[in]	ppszText   text of the control
 * @retval		None
 *
 * gets text to a control
 */
void jddex_widget::GetText(JC_CHAR **ppszText)
{
	if(m_pszText)
		*ppszText = jdi_CUtilsTcsDuplicate (JC_NULL, m_pszText);
	else
		*ppszText = JC_NULL ;
}

/**
 * @brief 		gets left co-ordiante of control
 * @retval		left co-ordinate of control
 *
 * gets left co-ordiante of control
 */
JC_INT32 jddex_widget::GetLeft()
{
	return m_stRect.iLeft ;
}

/**
 * @brief 		gets top co-ordiante of control
 * @retval		top co-ordinate of control
 *
 * gets top of control co-ordiante
 */
JC_INT32 jddex_widget::GetTop()
{
	return m_stRect.iTop ;
}

/**
 * @brief 		gets absloute left co-ordiante  of control
 * @retval		left co-ordinate
 *
 * gets absloute left co-ordiante of control w.r.t LCD dispaly
 */
JC_INT32 jddex_widget ::GetAbsLeft ( )
{
	JC_INT32 Left = 0;
	Left = m_stRect.iLeft + jddex_GetDevicePropLeft();
	return Left ;
}

/**
 * @brief 		gets absloute top co-ordiante of control
 * @retval		top co-ordinate
 *
 * gets absloute top co-ordiante of control w.r.t LCD dispaly
 */
JC_INT32 jddex_widget ::GetAbsTop()
{
	JC_INT32 Top = 0;
	Top = m_stRect.iTop ;
	Top += jddex_GetDevicePropTop();
	return Top ;
}

/**
 * @brief 		gets absloute recatngle co-ordiantes of control
 * @param[out]	pstAbsRect   pointer to JC_RECT
 * @retval		None
 *
 * gets absloute recatngle co-ordiantes of control w.r.t LCD dispaly
 */
void jddex_widget ::GetAbsRect(JC_RECT*  pstAbsRect)
{
	//JC_INT32 Left = 0,Top = 0;
	jc_memcpy (pstAbsRect, &m_stRect, sizeof(JC_RECT)) ;

	pstAbsRect->iLeft += jddex_GetDevicePropLeft() ;
	pstAbsRect->iTop += jddex_GetDevicePropTop() ; //
}

/**
 * @brief 		checks wether the widget is a container
 * @retval		E_FALSE/E_TRUE
 *
 */

JC_BOOLEAN jddex_widget ::IsContaianer()
{
  return m_bIsContainer;
}



/**
 * @brief 		returns the type of widget
 * @retval		enumeration of type EControlType
 *
 */

EControlType jddex_widget ::GetWidgetType()
{
  return m_eControlType;
}

/**
 * @brief 		overloaded new operator
 * @param[in]	uiSize   size to be allocated
 * @retval		pointer to memory allocated
 *
 */
void* jddex_widget::operator new( JC_UINT32 uiSize )
{
	return jdd_MemAlloc (1, uiSize);
}

/**
 * @brief 		overloaded delete operator
 * @param[in]	p   pointer to be freed
 * @retval		None
 *
 */
void jddex_widget::operator delete(void *p)
{
  if( p != JC_NULL )
	  jdd_MemFree (p);
}

/**
 * @brief 		returns parent window handle
 * @retval		parent handle
 *
 */
jddex_widget* jddex_widget::GetParentHnd()
{
  return m_poParentHwnd;
}


/**
 * @brief 		gets parent window rectangle
 * @param[out]	pstRect   rectangle co-ordiantes of parent window
 * @retval		None
 *
 */
void  jddex_widget::GetParentRect(JC_RECT*  pstRect)
{
	if(m_poParentHwnd)
	{
		m_poParentHwnd->GetAbsRect(pstRect);
	}
}
