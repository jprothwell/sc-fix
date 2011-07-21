/***************************************************************************
*
* File Name : jdd_combobox.cpp
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
* Purpose			:	Implimentation for combobox control
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

#include "jddex_combobox.h"
#include "jddex_platform.h"
#include "jddex_wnddefaults.h"
#include "jddex_int.h"
#include <ddl.h>
#include <jcal.h>
#include <jdi_browserengine.h>
#include "jddex_listbox.h"

//#include <app_mmi_inputbox.h>


#ifdef __cplusplus
    extern   "C"
    {
#endif
JC_HANDLE jddex_GetBrowserHandle(JC_HANDLE);
JC_HANDLE jddex_GetAppHandle() ;
#ifdef __cplusplus
	}
#endif

/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_combobox::jddex_combobox()
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
jddex_combobox::jddex_combobox(jddex_widget *poParentHwnd,JC_RECT *pstRect, JC_CONTROL_PROPERTIES* pstCtrlProperty)
				:jddex_widget(poParentHwnd,pstRect, pstCtrlProperty)
{

	m_eControlType = E_COMBO_BOX;
	m_stComboBox = pstCtrlProperty->typeOfControl.comboBox ;
	m_stJddexComboBox.uiItemCnt = m_stComboBox.uiNumberItems;
	m_stJddexComboBox.bIsCombo = E_TRUE;
	m_stJddexComboBox.listStyle = E_SINGLE_SELECTION;
	m_CurruiSelectedIndex = 0;
	

	m_stJddexComboBox.pstListItems = (JDDEX_LIST_ITEM**)jdd_MemAlloc(1, sizeof(JDDEX_LIST_ITEM*));
	*(m_stJddexComboBox.pstListItems) = JC_NULL;

	mmi_trace(1,(PCSTR)"no.of combo items[%d]",m_stComboBox.uiNumberItems);

    if(m_stComboBox.uiNumberItems > 0)
    {
		*(m_stJddexComboBox.pstListItems) = (JDDEX_LIST_ITEM*)jdd_MemAlloc(m_stComboBox.uiNumberItems, sizeof(JDDEX_LIST_ITEM));
    }
	m_stJddexComboBox.eRenderingStyle = pstCtrlProperty->eRenderingStyle;
}
/**
 * @brief 		sets selection status
 * @retval		None
 * frees all the resources allocated
 */
void jddex_combobox::SetSelectionStatus(JC_INT32 uiSelectedIndex)
{
	JDDEX_LIST_ITEM	  * pstListItems = * m_stJddexComboBox.pstListItems;

    if(m_stComboBox.uiNumberItems <= 0)
    {
		return;
	}
	pstListItems[uiSelectedIndex].IsSelected = E_TRUE ;
	m_CurruiSelectedIndex = uiSelectedIndex;
    
	
}

void jddex_combobox::GetSelectionStatus(JC_INT32 *puiSelectedIndex)
{
	*puiSelectedIndex = m_CurruiSelectedIndex;
}

void jddex_combobox::UpdateSelectionStatus()
{
	JDDEX_LIST_ITEM	  * pstListItems = * m_stJddexComboBox.pstListItems;
	JC_UINT32 uiCnt;	

	for ( uiCnt = 0 ; uiCnt < m_stJddexComboBox.uiItemCnt ; uiCnt++) 
	{
		if(pstListItems[uiCnt].IsSelected == E_TRUE )
		{
			m_CurruiSelectedIndex = uiCnt;
			break;
		}
	}
}

/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
void jddex_combobox::SetComboboxItem(JC_UINT32 uiIndexItem,JC_CHAR *pmText)
{
	JDDEX_LIST_ITEM	  * pstListItems = *m_stJddexComboBox.pstListItems;

     if(m_stComboBox.uiNumberItems <= 0)
    {
		return;
	}
	pstListItems[uiIndexItem].psText = jdi_CUtilsTcsDuplicate (JC_NULL, pmText);

}


jddex_combobox::~jddex_combobox()
{	
	if(m_stJddexComboBox.pstListItems)
	{
		if(*(m_stJddexComboBox.pstListItems))
		{
			JC_UINT32 uiItem;
		    for ( uiItem = 0 ; uiItem < m_stJddexComboBox.uiItemCnt ; uiItem++)
		    {				
				jdd_MemFree((*(m_stJddexComboBox.pstListItems))[uiItem].psText );
			   	(*(m_stJddexComboBox.pstListItems))[uiItem].psText = JC_NULL;			   
		    }
			jdd_MemFree(*(m_stJddexComboBox.pstListItems));
		}
		jdd_MemFree(m_stJddexComboBox.pstListItems);
	}
}

/**
 * @brief 		dispalys a combobox
 * @retval		None
 * displays the combobox based on its state
 */
void jddex_combobox::Show()
{
	JC_RECT stAbsRect, stTextRect, stIconRect;
	JC_INT32 iBorderColor ;
	JDDEX_LIST_ITEM	  * pstListItems = *m_stJddexComboBox.pstListItems;
	JC_UINT32 uiWidth = 0, uiHeight = 0 ;
	GetAbsRect(&stAbsRect);
	
	if(m_bIsFocussed)
	{
		iBorderColor = JDDEX_DEFAULT_BLUE;
	}
	else
	{
		iBorderColor = JDDEX_DEFAULT_BLACK ;
	}
	// draw the border
	jddex_DrawFormattedRect(&stAbsRect,iBorderColor, JDDEX_DEFAULT_WHITE, 
							JDDEX_DEFAULT_COMBOBOX_BORDER_WIDTH);
	jddex_GetListBoxIConDimensions(&uiWidth, &uiHeight);

	stTextRect = stAbsRect;
	stTextRect.uiWidth -= uiWidth;
 
	//draw the selected item
	 if(m_stComboBox.uiNumberItems > 0)
    {
	
		jddex_FormattedDrawTextEx(pstListItems[m_CurruiSelectedIndex].psText, stTextRect, m_stControlStyle, 
									E_JDDEX_LEFT_ALLIGN|E_JDDEX_MIDDLE_ALLIGN);
	 }

	stIconRect.uiWidth  = uiWidth; // width of the icon
	stIconRect.uiHeight = uiHeight; // height of the icon
	stIconRect.iLeft = stAbsRect.iLeft + stAbsRect.uiWidth - uiWidth;
    stIconRect.iTop = stAbsRect.iTop;

	//draw the icon at the end of the box
	jddex_DrawFormattedListBoxIcon(&stIconRect);
}


JC_RETCODE jddex_combobox::EventHandler(JC_UINT32 hWnd, JC_INT32 eEventType, 
								   void	*pvEventInfo)
{
	JC_RETCODE retCode = JC_ERR_NOT_IMPLEMENTED ;

	switch(eEventType)
	{
		case E_JDDEX_KEY_SELECT:
		{			
		    if(m_stComboBox.uiNumberItems > 0)
    		{ 
				jddex_PlatformCreateComboBox((JC_UINT32)this,&m_stJddexComboBox);		
				retCode = JC_OK ;
		    }
		}
		break;
	}
	return  retCode;
}

