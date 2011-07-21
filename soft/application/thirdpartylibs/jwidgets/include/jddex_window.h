/***************************************************************************
*
* File Name : jddex_window.h
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
* Purpose			:	class declaration for window
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

#ifndef _JDDEX_WINDOW_
#define _JDDEX_WINDOW_


#include <jddex_widget.h>
#include <jddex_image.h>
#include <jddex_wndutils.h>


class jddex_window : public jddex_widget  
{

public:
	jddex_window();
	virtual ~jddex_window();
	jddex_window(jddex_widget *poParentHwnd, JC_RECT *pstRect,
				JC_WINDOW_PROPERTIES *pstWinProp );
	void SetCurrFocusWidget(jddex_widget*  poWidget);
	void GetCurrFocusWidget(jddex_widget** ppoWidget);
	void DeleteWidget(jddex_widget* powidget);
	void AddToChildList(jddex_widget* poWidget);
	void SetWallPaper(JC_OBJECT_INFO* pstImageInfo, 
	JC_RECT *pstRect,EImageProperty  eImageProperty,JC_BOOLEAN* pIsSucess );
	JC_WINDOW_PROPERTIES GetWindowProp();
	JC_BOOLEAN IsApplicationWindow();
	void DeleteChildEntry(jddex_widget* poWidget);
	void GetScrollBar(jddex_widget ** ppoWidget);
	JC_BOOLEAN IsValidWidget(jddex_widget* poWidget);
	void Delete();
	void DeleteChildList();
    void Show();
	void ClearWallPaper() ;
	jddex_window * GetParentWindow();
	void ShowWallpaper();
	void GetControlID(jddex_widget ** ppoWidget, EControlType m_eControlType);

private:
	JC_WINDOW_PROPERTIES m_stWinProp;
	CHILD_LIST * m_pstStart;
	jddex_image * m_poWallPaper;
	JC_RECT  m_poWallPaperRect;
	jddex_widget * m_poCurrFocussedWidget;
	void DrawWindowBorder();
	void ShowChildWindows();
	

};

#endif // _JDDEX_WINDOW_
