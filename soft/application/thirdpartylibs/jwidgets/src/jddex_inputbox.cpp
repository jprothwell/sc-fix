/***************************************************************************
*
* File Name : jdd_inputbox.cpp
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
* Purpose			:	Implimentation for inputbox control
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
#include "wgui_categories.h"

#include "jddex_inputbox.h"
#include "jddex_platform.h"

#include "jddex_int.h"
#include <ddl.h>
#include <jcal.h>
#include "jddex_wnddefaults.h"

//#include <jdi_browserengine.h>





/*
#ifdef __cplusplus
    extern   "C"
    {
#endif
JC_HANDLE jddex_GetBrowserHandle(JC_HANDLE);
JC_HANDLE jddex_GetAppHandle() ;
#ifdef __cplusplus
	}
#endif
*/
//#define INPUTTEXTMARGIN 2



/**
 * @brief 		default constructor
 * @retval		None
 * initliazes the class members
 */
jddex_inputbox::jddex_inputbox()
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
jddex_inputbox::jddex_inputbox(jddex_widget *poParentHwnd,JC_RECT *pstRect, JC_CONTROL_PROPERTIES* pstCtrlProperty)
				:jddex_widget(poParentHwnd,pstRect, pstCtrlProperty)
{
  
	m_stInputBox = pstCtrlProperty->typeOfControl.inputBox ;

	m_szInputText = (JC_CHAR*)jdd_MemAlloc(JDDEX_MAX_INPUT_CHAR+1,2);
	
	if(m_stInputBox.pDefaultValue)
		{
			jc_wstrncpy(m_szInputText,m_stInputBox.pDefaultValue,JDDEX_MAX_INPUT_CHAR);
		}
	m_eControlType = E_INPUT_BOX;

}

/**
 * @brief 		destructor
 * @retval		None
 * frees all the resources allocated
 */
jddex_inputbox::~jddex_inputbox()
{
 if(m_szInputText)
 	{
 		jdd_MemFree(m_szInputText);
 	}

}

/**
 * @brief 		dispalys a input box
 * @retval		None
 * displays the input box based on its state
 */
void jddex_inputbox::Show()
{
	JC_RECT stAbsRect;
	JC_INT32 iBorderColor = JDDEX_DEFAULT_BLACK ;

	GetAbsRect(&stAbsRect);

	if(m_bIsFocussed)
	{
		iBorderColor = JDDEX_DEFAULT_BLUE;
	}
	mmi_trace(1,(PCSTR)"jddex_inputbox::Show");
	jddex_DrawFormattedRect(&stAbsRect,iBorderColor, JDDEX_DEFAULT_WHITE, 
							JDDEX_DEFAULT_INPUT_BORDER_WIDTH);
	
	if(jc_wstrlen(m_szInputText)==0)
	{
		return ;
	}


	if( m_stInputBox.einputBoxStyle == E_INPUT_MASK)
	{
		ShowMakedChars(stAbsRect) ;
	}
	else
	{
		ShowInputChars(stAbsRect) ;
	}

} 

void jddex_inputbox::ShowInputChars(JC_RECT stAbsRect)
{
	//JC_UINT32 uiLen;
	JC_CHAR sz_DisplayBuff[JDDEX_MAXCHAR_LINE+1]= {0,};
	//JC_TEXTMETRICS stTextMetrics;

	/*JC_CHAR	amTemp [] = {'.', '.', '.', '.', 0} ;	
	uiLen = jc_tcslen(m_szInputText);

	jddex_PlatformGetTextMetrics ( &m_stControlStyle, 
								  m_szInputText, 
								  stAbsRect.uiWidth,
								  &stTextMetrics);
	
	if( uiLen > stTextMetrics.uiNumCharacters )
	{
		uiLen = stTextMetrics.uiNumCharacters ;
		if ( uiLen - JDDEX_MAX_ELLIPSES > 0 )
		{
			jc_tcsncpy(sz_DisplayBuff,m_szInputText,uiLen - JDDEX_MAX_ELLIPSES) ;
			jc_tcscat(sz_DisplayBuff,amTemp);
		}
	}
	else
	{
		jc_tcsncpy(sz_DisplayBuff,m_szInputText,uiLen ) ;
	}*/

	jc_tcsncpy(sz_DisplayBuff,m_szInputText,JDDEX_MAXCHAR_LINE ) ;
	jddex_FormattedDrawTextEx(sz_DisplayBuff ,stAbsRect, 
				 m_stControlStyle, E_JDDEX_LEFT_ALLIGN|E_JDDEX_MIDDLE_ALLIGN);

}

void jddex_inputbox::ShowMakedChars(JC_RECT stAbsRect)
{
	JC_UINT32 uiLen;
	JC_CHAR sz_DisplayBuff[JDDEX_MAXCHAR_LINE+1]= {0,};
	JC_TEXTMETRICS stTextMetrics;
	//JC_CHAR	amTemp [] = {'.', '.', '.', 0} ;

	uiLen = jc_tcslen(m_szInputText);

	jddex_PlatformGetTextMetrics ( &m_stControlStyle, 
								  m_szInputText, 
								  stAbsRect.uiWidth,
								  &stTextMetrics);
	
	/*if( uiLen > stTextMetrics.uiNumCharacters )
	{
		uiLen = stTextMetrics.uiNumCharacters ;
		if ( uiLen - JDDEX_MAX_ELLIPSES > 0 )
		{
			jddex_FillChar(sz_DisplayBuff, '*' , uiLen - JDDEX_MAX_ELLIPSES);
			jc_tcscat(sz_DisplayBuff, amTemp);
		}
	}
	else
	{
		jddex_FillChar(sz_DisplayBuff, '*' , uiLen);
	}
	*/
	if( uiLen > stTextMetrics.uiNumCharacters )
	{
		jddex_FillChar(sz_DisplayBuff, '*' , stTextMetrics.uiNumCharacters);
	}
	else
	{
		jddex_FillChar(sz_DisplayBuff, '*' , uiLen);
	}
	jddex_FormattedDrawTextEx(sz_DisplayBuff ,stAbsRect, 
				 m_stControlStyle, E_JDDEX_LEFT_ALLIGN|E_JDDEX_MIDDLE_ALLIGN);

}



void jddex_inputbox::SetInputText(JC_CHAR *pszText)
{
	if(pszText)
		{
			jc_wstrncpy(m_szInputText,pszText,JDDEX_MAX_INPUT_CHAR);
		}

}

void jddex_inputbox::GetInputText(JC_CHAR **ppszText)
{
	if(jc_wstrlen(m_szInputText)!=0)
		*ppszText = jdi_CUtilsTcsDuplicate (JC_NULL, m_szInputText);
	else
		*ppszText = JC_NULL ;
}

JC_RETCODE jddex_inputbox::EventHandler(JC_UINT32 hWnd, JC_INT32 eEventType, void	*pvEventInfo)
{
	JC_RETCODE retCode = JC_ERR_NOT_IMPLEMENTED ;

	switch(eEventType)
	{
	case E_JDDEX_KEY_SELECT:
		{
			jddex_PlatformCreateInputBox((JC_UINT32)this, m_stInputBox, m_eRenderingStyle, m_szInputText);
			retCode = JC_OK ;
		}
		break;
	}
	return  retCode;
}


