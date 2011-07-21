/***************************************************************************
*
* File Name : jdd_window.cpp
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
* Purpose			:	Implimentation for window 
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
#include "jddex_window.h"
#include <jddex_platform.h>
//#include <jddex_wingtech.h>
#include <jddex_int.h>



/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_window::jddex_window()
{

}
/**
 * @brief 		constructor
 * @param[in]	pst_ParentHwnd   Handle of parent window
 * @param[in]	pstRect          control co-ordinates
 * @param[in]	pstWinProp		 pointer to  JC_WINDOW_PROPERTIES
 * @retval		None
 *
 * Calls the base class (widget class) constructor and initliazes 
 * class members
 */
jddex_window::jddex_window(jddex_widget *poParentHwnd, JC_RECT *pstRect,
				JC_WINDOW_PROPERTIES *pstWinProp ):jddex_widget(poParentHwnd,pstRect)
{
	jc_memcpy (&m_stWinProp, pstWinProp, sizeof(JC_WINDOW_PROPERTIES)) ;
	SetDefaultColors();
	m_pstStart = JC_NULL;
	m_bIsContainer = E_TRUE;
    m_poCurrFocussedWidget = JC_NULL;
    m_poWallPaper = JC_NULL;
}

/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
jddex_window::~jddex_window()
{
	DeleteChildList();
	ClearWallPaper();

	jddex_window *pstParentWindow = JC_NULL;

	pstParentWindow = this->GetParentWindow();
	
	if(pstParentWindow)
	{
		pstParentWindow->DeleteChildEntry(this);
	}

}

/**
 * @brief 		Displays the window along with its controls
 * @retval		None
 *
 * Invokes ShowChildWindows()
 */
void jddex_window::SetWallPaper(JC_OBJECT_INFO* pstImageInfo, 
								JC_RECT *pstRect,EImageProperty  eImageProperty,JC_BOOLEAN* pIsSucess  )
{


	// store the wallpapaer rect in member variable 
	jc_memcpy (&m_poWallPaperRect, pstRect, sizeof(JC_RECT)) ;
	

	if(m_poWallPaperRect.iTop < 0)
		m_poWallPaperRect.iTop = 0;

	if(m_poWallPaperRect.iLeft < 0)
		m_poWallPaperRect.iLeft = 0;
	
	if(pstRect->uiHeight > m_stRect.uiHeight)
		m_poWallPaperRect.uiHeight = m_stRect.uiHeight;

	if(pstRect->uiWidth > m_stRect.uiWidth)
		m_poWallPaperRect.uiWidth = m_stRect.uiWidth;

	mmi_trace(1,(PCSTR)"widget_log: SetWallPaper  %d %d", 
		pstRect->uiHeight,pstRect->uiWidth);
	mmi_trace(1,(PCSTR)"widget_log: SetWallPaper %d %d %d %d", \
					m_poWallPaperRect.iLeft,m_poWallPaperRect.iTop, \
					m_poWallPaperRect.uiHeight, m_poWallPaperRect.uiWidth);
	
	if(pstImageInfo->bIsFileContent == E_FALSE)
	{
		JC_CHAR *pmTempImageFile = JC_NULL;
		JDD_FILE hTempFile  = JC_NULL;
		jdi_CUtilsCharToTchar (JC_NULL, JMMS_TEMP_IMAGEFILE, &pmTempImageFile) ;
		hTempFile = jdd_FSOpen (JC_NULL, pmTempImageFile, E_CREATE_WRITE_MODE) ;	
		jdd_FSWrite ((void *)(pstImageInfo->uBuffer.pucBuffer),
					pstImageInfo->uiObjectLen, 1, hTempFile);
		jdd_FSClose(hTempFile);
			
		m_poWallPaper = new jddex_image(this, pstRect, pstImageInfo, eImageProperty, pIsSucess, pmTempImageFile);
		//m_poWallPaper = new jddex_image(poWindow, pstRect, pstControlProperty, pIsSucess, JC_NULL);
	}
	else
	{
		m_poWallPaper = new jddex_image(this, pstRect, pstImageInfo, eImageProperty, pIsSucess, JC_NULL);
		//m_poWallPaper = new jddex_image(this,pstRect, pstImageInfo, eImageProperty, pIsSucess);
	}
	
}

/**
 * @brief 		Deletes the window wallpaper.
 * @retval		None
 *
 * Invokes ShowChildWindows()
 */
void jddex_window::ClearWallPaper( )
{
	if (JC_NULL != m_poWallPaper )
	{
		delete m_poWallPaper ;
		m_poWallPaper = JC_NULL;
	}
}
/**
 * @brief 		Displays the window along with its controls
 * @retval		None
 *
 * Invokes ShowChildWindows()
 */
void jddex_window::Show()
{
	ShowChildWindows();
}

/**
 * @brief 		Displays the window along with its controls
 * @retval		None
 *
 * Draws the window and invokes all the child control show function 
 */
void jddex_window::ShowChildWindows()
{
	CHILD_LIST * pstWidgetNode = m_pstStart;

	JDDEX_DEVICE_CHAR pstDeviceChar ;

	DrawWindowBorder();
	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &pstDeviceChar);

	while(pstWidgetNode != JC_NULL)
	{
		if (pstWidgetNode->poWidget)
		{
		if(pstWidgetNode->poWidget->IsContaianer() == E_TRUE)
		{
			jddex_window * poChildWnd = (jddex_window*)(pstWidgetNode->poWidget) ;
			poChildWnd->ShowChildWindows();
		}
		else
		{
			if(pstWidgetNode->poWidget->GetVisibiltyStatus() == E_TRUE)
			{
					pstWidgetNode->poWidget->Show();
			}
		}
		}
		pstWidgetNode = pstWidgetNode->pstNext;
	}
}

 
/**
 * @brief 		Displays the window along with its controls
 * @retval		None
 *
 * Draws the window borders 
 */
void jddex_window::DrawWindowBorder()
{
	JC_RECT stAbsRect;
	GetAbsRect(&stAbsRect);
	JC_INT32 iBgColor;
	JDDEX_DEVICE_CHAR stDeviceChar;
	JC_RECT stRenderingRect;

	GetBgColor(&iBgColor);

	if( E_APPLICATION_WINDOW == m_stWinProp.eWindowStyle)
	{
		//dex_PlatformEraseRect(&m_stRect);
		jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);

		stRenderingRect.iLeft = stDeviceChar.uiLeft;
		stRenderingRect.iTop = stDeviceChar.uiTop;
		stRenderingRect.uiWidth = stDeviceChar.uiRenderingWidth;
		stRenderingRect.uiHeight = stDeviceChar.uiRenderingHeight;
		
		jddex_PlatformEraseRect(&stRenderingRect);
#if 0
		mmi_trace(1,(PCSTR)"jddex_log: WindowBorder left[%d], Top[%d] wt[%d] HT[%d]", 
					stRenderingRect.iLeft, stRenderingRect.iTop, 
					stRenderingRect.uiWidth, stRenderingRect.uiHeight);
#endif		
		// pass absolute rect
		mmi_trace(1,(PCSTR)"widget_log: DrawWindowBorder 1");
		jddex_DrawFormattedRectEx(&m_stRect, &m_stWinProp.stBorderProp, iBgColor) ;
		ShowWallpaper();
	}
	else
	{
		mmi_trace(1,(PCSTR)"widget_log: DrawWindowBorder2 %d %d %d %d %d", \
					stAbsRect.iLeft,stAbsRect.iTop,stAbsRect.uiHeight,stAbsRect.uiWidth,iBgColor);
		jddex_DrawFormattedRectEx(&stAbsRect, &m_stWinProp.stBorderProp, iBgColor) ;
		ShowWallpaper();
	}
}


void jddex_window::ShowWallpaper()
{

	if(m_poWallPaper)
	{
		JC_RECT stAbsRect = {0,};
		//the wallpaper rectangle is w.r.t parent window, so adding parent co-ordinates to get the actaula location
		stAbsRect.iLeft = m_poWallPaperRect.iLeft + m_stRect.iLeft;
		stAbsRect.iTop = m_poWallPaperRect.iTop + m_stRect.iTop;
		stAbsRect.uiHeight = m_poWallPaperRect.uiHeight;
		stAbsRect.uiWidth = m_poWallPaperRect.uiWidth;
		
	
		mmi_trace(1,(PCSTR)"widget_log: wallpaper present");
		if( m_poWallPaper->GetImageProperty()== E_IMAGE_CENTER)
		{
			
			mmi_trace(1,(PCSTR)"widget_log: wallpaper E_IMAGE_CENTER");
			m_poWallPaper->SetRect(&stAbsRect);
			m_poWallPaper->Show();
		}
	}
}

/**
 * @brief 		Sets the current foccused widget in the window
 * @param[in]	pstWidget Handle of control
 * @retval		None
 *
 */
void jddex_window::SetCurrFocusWidget(jddex_widget*  poWidget)
{
	m_poCurrFocussedWidget = poWidget;
}

/**
 * @brief 		Gets the current foccused widget in the window
 * @param[in]	ppstWidget   pointer to control
 * @retval		None
 *
 */
void jddex_window::GetScrollBar(jddex_widget ** ppoWidget)
{
	CHILD_LIST* pstTemp = JC_NULL ;

	pstTemp = m_pstStart;
	if (pstTemp)
	{
		
		do
		{
			//jddex_widget * poWidget = (jddex_widget *) pstTemp;
			if( pstTemp->poWidget->GetWidgetType() == E_SCROLLBAR)
			{
				*ppoWidget = pstTemp->poWidget ;
				break;
			}
			pstTemp = pstTemp->pstNext;
		}
		while(pstTemp != JC_NULL);
	}
}


/**
 * @brief 		Gets the current foccused widget in the window
 * @param[in]	ppstWidget   pointer to control
 * @retval		None
 *
 */
void jddex_window::GetCurrFocusWidget(jddex_widget ** ppoWidget)
{
	*ppoWidget = m_poCurrFocussedWidget ;
}

/**
 * @brief 		Add a control to child list of window
 * @param[in]	pstWidget   Handle of control
 * @retval		None
 *
 */
void jddex_window::AddToChildList(jddex_widget* poWidget)
{
	CHILD_LIST* pstNewNode = JC_NULL;
	CHILD_LIST* pstTemp = JC_NULL ;

	if(m_pstStart == JC_NULL)
	{
		pstNewNode = (CHILD_LIST*)jdd_MemAlloc (1, sizeof(CHILD_LIST)) ;
		pstNewNode->poWidget = poWidget;
		pstNewNode->bIsVisible = 1;
		pstNewNode->pstNext = JC_NULL;
		m_pstStart = pstNewNode;
	}
	else
	{
		pstTemp = m_pstStart;
		while(pstTemp->pstNext != JC_NULL)
			pstTemp = pstTemp->pstNext;

		pstNewNode = (CHILD_LIST*)jdd_MemAlloc (1, sizeof(CHILD_LIST)) ;
		pstNewNode->poWidget = poWidget;
		pstNewNode->bIsVisible = 1;
		pstNewNode->pstNext = JC_NULL;
		pstTemp->pstNext = pstNewNode;	
	}

}


/**
 * @brief 		deletes the entire child list of window
 * @retval		None
 *
 */
void jddex_window::DeleteChildList()
{
	CHILD_LIST* pstTempNode, *pstPrevNode ;

	pstTempNode = m_pstStart ;

	while ( pstTempNode )
	{
		pstPrevNode = pstTempNode;
		pstTempNode = pstTempNode->pstNext ;
		if(pstPrevNode->poWidget)
		{
			if(pstPrevNode->poWidget->IsContaianer())
			{
				jddex_window*  poWindow = (jddex_window*) pstPrevNode->poWidget ;
				poWindow->DeleteChildList();

			}
			else
			{
				delete pstPrevNode->poWidget;
			}
			pstPrevNode->poWidget =  JC_NULL;
		}
		jdd_MemFree (pstPrevNode);
	}
	m_pstStart = JC_NULL;
}


void jddex_window::Delete()
{
	DeleteChildList();
}

jddex_window * jddex_window::GetParentWindow()
{
	return (jddex_window *)m_stWinProp.uiParentID;
}
/**
 * @brief 		retruns the window properties
 * @retval		pointer to JC_WINDOW_PROPERTIES
 *
 */
JC_WINDOW_PROPERTIES jddex_window::GetWindowProp()
{
	return m_stWinProp;
}

/**
 * @brief 		checks if the window is application window
 * @retval		TRUE  -  If applilaction window
 *              FALSE - If primary MMI window
 *
 */

JC_BOOLEAN jddex_window::IsApplicationWindow()
{
	if( m_stWinProp.eWindowStyle == E_APPLICATION_WINDOW)
		return E_TRUE;
}

/**
 * @brief 		deletes a control to child list of window
 * @param[in]	poWidget   Handle of control
 * @retval		None
 *
 */

void jddex_window::DeleteWidget(jddex_widget* poWidget)
{
	CHILD_LIST *pstPrevNode = JC_NULL, *pstTempNode ;  
	pstTempNode = m_pstStart;  
	while ( pstTempNode != JC_NULL )  
	{  

		if ( pstTempNode->poWidget == poWidget)  
		{  
				if ( pstTempNode == m_pstStart )  
				{  
					m_pstStart = pstTempNode -> pstNext ;  
					delete pstTempNode->poWidget;
					jdd_MemFree ( pstTempNode ) ;  
					return ;  
				}  
				else  
				{  
					pstPrevNode -> pstNext = pstTempNode -> pstNext ;  
					delete pstTempNode->poWidget;
					jdd_MemFree ( pstTempNode ) ;  
					return ;  
				}  

		}  
		else   
		{  
			pstPrevNode = pstTempNode ;  
			pstTempNode = pstTempNode -> pstNext ;  
		}  
	}
}

void jddex_window::DeleteChildEntry(jddex_widget* poWidget)
{
	CHILD_LIST *pstPrevNode = JC_NULL, *pstTempNode ;  
	pstTempNode = m_pstStart;  
	while ( pstTempNode != JC_NULL )  
	{
		if ( pstTempNode->poWidget == poWidget)  
		{  
			if ( pstTempNode == m_pstStart )  
			{  
				m_pstStart = pstTempNode -> pstNext ;  
				jdd_MemFree ( pstTempNode ) ;  
				return ;  
			}  
			else  
			{  
				pstPrevNode -> pstNext = pstTempNode -> pstNext ;  
				jdd_MemFree ( pstTempNode ) ;  
				return ;  
			}
		}  
		else   
		{  
			pstPrevNode = pstTempNode ;  
			pstTempNode = pstTempNode -> pstNext ;  
		}  
	}
}

JC_BOOLEAN jddex_window::IsValidWidget(jddex_widget* poWidget)
{
	CHILD_LIST* pstTempNode ;

	pstTempNode = m_pstStart ;

	while ( pstTempNode )
	{
		if(pstTempNode->poWidget == poWidget)
			return E_TRUE;
		
		if(pstTempNode->poWidget->IsContaianer())
		{
			jddex_window * poWindow = (jddex_window *) pstTempNode->poWidget;

			if(poWindow->IsValidWidget(poWidget) == E_TRUE)
				return E_TRUE;
		}
		pstTempNode = pstTempNode->pstNext ;
	}
	return E_FALSE;

}

void jddex_window::GetControlID(jddex_widget ** ppoWidget, EControlType eControlType)
{
	CHILD_LIST* pstTemp = JC_NULL ;
	pstTemp = m_pstStart;

	if (pstTemp)
	{		
		do
		{
			if(pstTemp->poWidget->GetWidgetType() == eControlType)
			{
				*ppoWidget = pstTemp->poWidget ;
				break;
			}
			pstTemp = pstTemp->pstNext;
		}
		while(pstTemp != JC_NULL);
	}
}

