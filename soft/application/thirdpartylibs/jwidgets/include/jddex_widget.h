/***************************************************************************
*
* File Name : jddex_widget.h
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
* Purpose			:	class declaration for widget
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

#ifndef _JDDEX_WIDGET_
#define _JDDEX_WIDGET_

#include <ddl.h>
#include <jcal.h>
#include <jdd_log.h>




class jddex_widget  
{

public:
	jddex_widget() ;
	jddex_widget(jddex_widget *poParentHwnd,JC_RECT *pstRect, 
		JC_CONTROL_PROPERTIES* pstCtrlProperty);
	jddex_widget(jddex_widget *poParentHwnd,JC_RECT *pstRect);
	virtual ~jddex_widget();

	virtual void Show() = 0;
	void SetColors(JC_INT32 iBgColor, JC_INT32 iFgColor);
	void GetColors(JC_INT32 *piBgColor, JC_INT32 *piFgColor);
	void SetDefaultColors();
	virtual JC_RETCODE EventHandler(JC_UINT32 hWnd, JC_INT32 eEventType, 
								   void	*pvEventInfo);
	void SetVisibilty(JC_BOOLEAN bVisible);
	void Disable(JC_BOOLEAN bDisabled);
	void SetFgColor(JC_INT32 iFgColor);
	void SetBgColor(JC_INT32 iBgColor);
	void GetFgColor(JC_INT32 *piFgColor);
	void GetBgColor(JC_INT32 *piBgColor);
	void SetRect(JC_RECT*	pstRect);
	void GetRect(JC_RECT*  pstRect);
	void SetText(JC_CHAR *pszText);
	void GetText(JC_CHAR **ppszText);
	void GetAbsRect(JC_RECT*  pstAbsRect);
	jddex_widget* GetParentHnd();
	void  GetParentRect(JC_RECT*  pstRect);
	JC_BOOLEAN GetVisibiltyStatus();
	virtual void SetDefaultBgColor();
	virtual void SetDefaultFgColor();
	void* operator new( JC_UINT32 uiSize );
	void operator delete(void *p);
	EControlType GetWidgetType();
	JC_BOOLEAN IsContaianer();
	JC_INT32 GetAbsTop();
	JC_INT32 GetAbsLeft();
	JC_INT32 GetLeft();
	JC_INT32 GetTop();
	void SetFocus();
	void ResetFocus();
	virtual void  HandleAction(E_OBJECT_ACTION		eAction);

protected:
	JC_BOOLEAN	m_bIsSelectable;
	JC_BOOLEAN	m_bIsDisabled ;
	JC_BOOLEAN	m_bIsFocussed;
	JC_BOOLEAN	m_bIsVisible;
	JC_BOOLEAN	m_bIsContainer;
	JC_STYLE	m_stControlStyle;
	JC_RECT		m_stRect;
	EControlType m_eControlType;
	JC_CHAR	*   m_pszText ; 
	jddex_widget*	m_poParentHwnd;
	ERenderingStyle m_eRenderingStyle;
	
};

#endif // _JDDEX_WIDGET_
