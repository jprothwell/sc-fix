/***************************************************************************
*
* File Name : jdd_scrollbar.cpp
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
* Purpose			:	Implimentation for scrollbar control
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
#include "jddex_scrollbar.h"
#include "jddex_platform.h"
#include "jddex_wnddefaults.h"
#include "jddex_int.h"


#define SCROLL_CAR_GAP 1
#define SCROLL_CAR_MIN_HEIGHT 4


void jddex_DrawScrollBarControl(JC_RECT stScrollBarRect, JC_RECT stScrollCarRect );

/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_scrollbar::jddex_scrollbar()
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
jddex_scrollbar::jddex_scrollbar(jddex_widget *poParentHwnd,JC_RECT *pstRect, JC_CONTROL_PROPERTIES* pstCtrlProperty)
				:jddex_widget(poParentHwnd,pstRect, pstCtrlProperty)
{

	m_eControlType = E_SCROLLBAR;
	mmi_trace(1,(PCSTR)"scroll bar x[%d] y [%d] width[%d] height[%d] ", pstRect->iLeft, pstRect->iTop, pstRect->uiWidth, pstRect->uiHeight);

	m_stScrollCar.uiWidth   = pstRect->uiWidth ;
}

/**
 * @brief 		sets scrollbar info
 * @retval		None
 * frees all the resources allocated
 */
void jddex_scrollbar::SetScrollInfo(SCROLL_INFO *pstScrollInfo)
{
	jc_memcpy(&m_stScrollInfo,pstScrollInfo,sizeof(SCROLL_INFO));
}

/**
 * @brief 		gets scrollbar info
 * @retval		None
 * frees all the resources allocated
 */
void jddex_scrollbar::GetScrollInfo(SCROLL_INFO *pstScrollInfo)
{
	jc_memcpy(pstScrollInfo,&m_stScrollInfo,sizeof(SCROLL_INFO));
}

/**
 * @brief 		gets scrollcar info
 * @retval		None
 */
void jddex_scrollbar::GetScrollCarInfo(JC_RECT *pstScrollcar)
{
	*pstScrollcar = m_stScrollCar ;
}

/**
 * @brief 		gets scrollcar info
 * @retval		None	
 */
void jddex_scrollbar::GetScrollBarInfo(JC_RECT *pstScrollBar)
{
	JC_RECT stAbsRect;
	GetAbsRect(&stAbsRect);
	
	*pstScrollBar = stAbsRect ;
}


/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
jddex_scrollbar::~jddex_scrollbar()
{
	
}

/**
 * @brief 		displays a scroll bar
 * @retval		None
 * displays a scroll bar
 */
void jddex_scrollbar::Show()
{
  	JC_RECT stAbsRect;
	GetAbsRect(&stAbsRect);
	JC_DOUBLE dScrollCarPos ;
	JC_DOUBLE dScrollCarHieght ;
	JC_DOUBLE uiActualheight;


	mmi_trace(1,(PCSTR)"jddex_scrollbar::Show");
	if(m_stScrollInfo.uiMaxPos == 0 || \
		m_stScrollInfo.uiMaxPos <= m_stScrollInfo.uiPageSize)
	{
	    JC_RECT stTempCarRect;
		mmi_trace(1,(PCSTR)"if condition [%d] [%d] ", m_stScrollInfo.uiMaxPos, m_stScrollInfo.uiPageSize);
		stTempCarRect = stAbsRect;
		stTempCarRect.iTop  =  jddex_GetDevicePropTop()+ jwap_GetScrollbarbarArrowHight() ;
		stTempCarRect.uiHeight = jddex_GetDeviceRenderHeight() - (2*jwap_GetScrollbarbarArrowHight());
		
	    jddex_DrawScrollBarControl(stAbsRect, stTempCarRect );
		return;
	}

	uiActualheight = m_stScrollInfo.uiPageSize - (2*jwap_GetScrollbarbarArrowHight());
	
	m_stScrollCar.iLeft		= stAbsRect.iLeft;	


	dScrollCarHieght = (JC_DOUBLE) (uiActualheight*m_stScrollInfo.uiPageSize) / (double) (m_stScrollInfo.uiMaxPos) ;
	m_stScrollCar.uiHeight = jdd_MiscCeil(dScrollCarHieght);	
	dScrollCarPos   =  ((JC_DOUBLE)uiActualheight/(JC_DOUBLE)m_stScrollInfo.uiMaxPos)*(double)m_stScrollInfo.uiCurrPos ;
	m_stScrollCar.iTop  =  dScrollCarPos + jddex_GetDevicePropTop()+ jwap_GetScrollbarbarArrowHight() ;

	if(m_stScrollCar.uiHeight < SCROLL_CAR_MIN_HEIGHT)
	{
		m_stScrollCar.uiHeight = SCROLL_CAR_MIN_HEIGHT;
		m_stScrollCar.iTop -= SCROLL_CAR_MIN_HEIGHT;

		if(m_stScrollCar.iTop < CST_TITLE_HEIGHT)
			m_stScrollCar.iTop = CST_TITLE_HEIGHT;

	}

	//error condition  shouldnt occur
	if((m_stScrollCar.uiHeight+m_stScrollCar.iTop) > \
		(jwap_GetScrollbarbarArrowHight()+uiActualheight+jddex_GetDevicePropTop()))
	{
		m_stScrollCar.uiHeight = (JC_UINT32)(jwap_GetScrollbarbarArrowHight()+uiActualheight + jddex_GetDevicePropTop()- m_stScrollCar.iTop );
	}

	jddex_DrawScrollBarControl(stAbsRect, m_stScrollCar );


}


JC_UINT32 jddex_scrollbar::GetScrollOffsetFactor()
{
	JC_DOUBLE uiActualheight;
	uiActualheight = m_stScrollInfo.uiPageSize - (2*jwap_GetScrollbarbarArrowHight());
	uiActualheight = m_stScrollInfo.uiPageSize - (2*jwap_GetScrollbarbarArrowHight());
 	return (uiActualheight/m_stScrollCar.uiHeight);
}

