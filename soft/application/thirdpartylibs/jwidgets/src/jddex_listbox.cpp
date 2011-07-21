/***************************************************************************
*
* File Name : jdd_listbox.cpp
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
* Purpose			:	Implimentation for list box control
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
#include <ddl.h>
#include <jcal.h>
#include "jddex_wnddefaults.h"
#include "jddex_int.h"
#include "jddex_listbox.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef __cplusplus
	}
#endif

/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_listbox::jddex_listbox()
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
jddex_listbox::jddex_listbox(jddex_widget *poParentHwnd,JC_RECT *pstRect, JC_CONTROL_PROPERTIES* pstCtrlProperty)
				:jddex_widget(poParentHwnd,pstRect, pstCtrlProperty)
{
	m_eControlType = E_LIST_BOX;
	m_stListBox = pstCtrlProperty->typeOfControl.listBox ;
	m_stJddexListBox.uiItemCnt = m_stListBox.uiNumberItems;
	m_stJddexListBox.bIsCombo = E_FALSE;
	m_stJddexListBox.listStyle = m_stListBox.listStyle;

	m_stJddexListBox.pstListItems = (JDDEX_LIST_ITEM**)jdd_MemAlloc(1, sizeof(JDDEX_LIST_ITEM*));

    *(m_stJddexListBox.pstListItems) = JC_NULL;
    if(m_stListBox.uiNumberItems > 0)
    {
		*(m_stJddexListBox.pstListItems) = (JDDEX_LIST_ITEM*)jdd_MemAlloc(m_stListBox.uiNumberItems, sizeof(JDDEX_LIST_ITEM));
    }
	m_stJddexListBox.eRenderingStyle = pstCtrlProperty->eRenderingStyle;

	
}
/**
 * @brief 		sets selection status
 * @retval		None
 * frees all the resources allocated
 */
void jddex_listbox::SetSelectionStatus(JC_INT32 *puiSelectedIndex,
								  JC_UINT32 uiSelectedCounts)
{
	JC_UINT32 uiCnt;
	JDDEX_LIST_ITEM	  * pstListItems = * m_stJddexListBox.pstListItems;

	if(m_stListBox.uiNumberItems <= 0)
	{
			return;
	}

	for ( uiCnt = 0 ; uiCnt < uiSelectedCounts ; uiCnt++)
			pstListItems[puiSelectedIndex[uiCnt]].IsSelected = E_TRUE ;

}

void jddex_listbox::GetSelectionStatus(JC_INT32 **ppiSelectedIndex,
								  JC_UINT32 *puiSelectedCounts)
{
	JDDEX_LIST_ITEM*	pstListItems = * m_stJddexListBox.pstListItems;
	JC_UINT32 			uiCnt,uiSelectedCounts = 0, uiIndex= 0;	

	if(m_stListBox.uiNumberItems <= 0)
	{
		*puiSelectedCounts = 0;
			return;
	}

	for ( uiCnt = 0 ; uiCnt < m_stJddexListBox.uiItemCnt ; uiCnt++) 
	{
		if(pstListItems[uiCnt].IsSelected == E_TRUE )
		{
			uiSelectedCounts++;
		}
	}
 
	*puiSelectedCounts = uiSelectedCounts;
	if (0 == uiSelectedCounts)
	{
		uiSelectedCounts += 1 ;
	}
	*ppiSelectedIndex = (JC_INT32 *) jdd_MemAlloc (uiSelectedCounts, sizeof (JC_INT32)) ;

	for ( uiCnt = 0 , uiIndex= 0 ; uiCnt < m_stJddexListBox.uiItemCnt ; uiCnt++) 
	{
		if(pstListItems[uiCnt].IsSelected == E_TRUE )
		{
			*(*ppiSelectedIndex + uiIndex) = uiCnt;
			uiIndex++;
		}
	}
}

JC_INT32 jddex_listbox::GetFirstSelection()
{
	JDDEX_LIST_ITEM	  * pstListItems = * m_stJddexListBox.pstListItems;
	JC_UINT32 uiCnt,iSelectedIndex = 0;

	for ( uiCnt = 0 ; uiCnt < m_stJddexListBox.uiItemCnt ; uiCnt++) 
	{
		if(pstListItems[uiCnt].IsSelected == E_TRUE )
		{
			iSelectedIndex = uiCnt;
			break;
		}
	}
 return iSelectedIndex;

}
/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
void jddex_listbox::SetListboxItem(JC_UINT32 uiIndexItem,JC_CHAR *pmText,
									JC_IMAGE_INFO *pImageInfo)
{
	JDDEX_LIST_ITEM	  * pstListItems = *m_stJddexListBox.pstListItems;
    if(m_stListBox.uiNumberItems > 0)
    {
		pstListItems[uiIndexItem].psText = jdi_CUtilsTcsDuplicate (JC_NULL, pmText);
    }
}


/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
jddex_listbox::~jddex_listbox()
{
	if(m_stJddexListBox.pstListItems)
	{
		if(*(m_stJddexListBox.pstListItems))
		{
			JC_UINT32 uiItem;
		    for ( uiItem = 0 ; uiItem < m_stJddexListBox.uiItemCnt ; uiItem++)
		    {
				jdd_MemFree((*(m_stJddexListBox.pstListItems))[uiItem].psText );
			   	(*(m_stJddexListBox.pstListItems))[uiItem].psText = JC_NULL;			   
		    }
			jdd_MemFree (*(m_stJddexListBox.pstListItems));
		}
		jdd_MemFree (m_stJddexListBox.pstListItems);
	}
}

/**
 * @brief 		dispalys a listbox
 * @retval		None
 * displays the listbox based on its state
 */
void jddex_listbox::Show()
{
	JC_RECT stAbsRect, stTextRect, stIconRect;
	JC_INT32 iBorderColor ;
	JC_UINT32 uiVisibleRows = 0;
	JC_UINT32 uiWidth = 0, uiHeight = 0 ;
	JDDEX_LIST_ITEM	  * pstListItems = *m_stJddexListBox.pstListItems;

	GetAbsRect(&stAbsRect);
	if(m_stListBox.uiVisibleRows == 0)
		uiVisibleRows = m_stListBox.uiNumberItems;
	else
		uiVisibleRows = m_stListBox.uiVisibleRows;


	if(m_bIsFocussed)
	{
		iBorderColor = JDDEX_DEFAULT_BLUE;
	}
	else
	{
		iBorderColor = JDDEX_DEFAULT_BLACK ;
	}
	jddex_DrawFormattedRect(&stAbsRect,iBorderColor, JDDEX_DEFAULT_WHITE, 
				JDDEX_DEFAULT_LISTBOX_BORDER_WIDTH);

	jddex_GetListBoxIConDimensions(&uiWidth, &uiHeight);

	stTextRect = stAbsRect;
	stTextRect.uiWidth -= uiWidth;

	 if(m_stListBox.uiNumberItems > 0)
    {
		//display the first selcted if any or dispaly the 1st list item
		jddex_FormattedDrawTextEx(pstListItems[GetFirstSelection()].psText, stTextRect, m_stControlStyle, 
										E_JDDEX_LEFT_ALLIGN|E_JDDEX_MIDDLE_ALLIGN);
	}

	stIconRect.uiWidth  = uiWidth; // width of the icon
	stIconRect.uiHeight = uiHeight; // height of the icon
	stIconRect.iLeft = stAbsRect.iLeft + stAbsRect.uiWidth - uiWidth;
    stIconRect.iTop = stAbsRect.iTop;

	//draw the icon at the end of the box
	jddex_DrawFormattedListBoxIcon(&stIconRect);
}


JC_RETCODE jddex_listbox::EventHandler(JC_UINT32 hWnd, JC_INT32 eEventType, 
								   void	*pvEventInfo)
{
	JC_RETCODE retCode = JC_ERR_NOT_IMPLEMENTED ;
	switch(eEventType)
	{
	case E_JDDEX_KEY_SELECT:
		{	
			if(m_stListBox.uiNumberItems > 0)
    		{
				jddex_PlatformCreateListBox((JC_UINT32)this, &m_stJddexListBox);
				retCode = JC_OK ;
			}
		}
		break;
	}
	return  retCode;
}
