/***************************************************************************
*
* File Name : jddex_inputbox.h
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
* Purpose			:	class declaration for inputbox control
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

#ifndef _JDDEX_INPUTBOX_
#define _JDDEX_INPUTBOX_

#include <jddex_widget.h>
#define JDDEX_MAX_ELLIPSES 3

class jddex_inputbox : public jddex_widget  
{

public:
	jddex_inputbox();
	jddex_inputbox(jddex_widget *poParentHwnd,JC_RECT *pstRect, JC_CONTROL_PROPERTIES* pstCtrlProperty);
	JC_RETCODE EventHandler(JC_UINT32 hWnd, JC_INT32 eEventType, 
								   void	*pvEventInfo);
	void SetInputText(JC_CHAR *pszText);
	void GetInputText(JC_CHAR **ppszText);

	virtual ~jddex_inputbox();
	void Show();
	void SetText();


protected:
	JC_INPUT_BOX			m_stInputBox;
	//JC_CHAR	*   m_pszInputText ; 
	JC_CHAR	   *m_szInputText ; 
private:
	void ShowInputChars(JC_RECT stAbsRect);
	void ShowMakedChars(JC_RECT stAbsRect);	
};

#endif // _JDDEX_INPUTBOX_
