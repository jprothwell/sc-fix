/***************************************************************************
*
* File Name : jddex_combobox.h
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
* Purpose			:	class declaration for combobox control
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

#ifndef _JDDEX_COMBOBOX_
#define _JDDEX_COMBOBOX_


#include <jddex_widget.h>
#include <jddex_listbox.h>

class jddex_combobox : public jddex_widget  
{


public:
	jddex_combobox();
	jddex_combobox(jddex_widget *poParentHwnd,JC_RECT *pstRect, 
		JC_CONTROL_PROPERTIES* pstCtrlProperty);
	void SetSelectionStatus(JC_INT32 uiSelectedIndex);
	void GetSelectionStatus(JC_INT32 *puiSelectedIndex);
	void SetComboboxItem(JC_UINT32 uiIndexItem,JC_CHAR *pmText);
	JC_RETCODE EventHandler(JC_UINT32 hWnd, JC_INT32 eEventType, 
								   void	*pvEventInfo);
	
	virtual ~jddex_combobox();
	void Show();
private:
	void UpdateSelectionStatus();	
protected:
		JC_COMBO_BOX m_stComboBox ;
		JC_INT32 m_CurruiSelectedIndex;
		JDDEX_LIST_BOX m_stJddexComboBox ;
	



};

#endif // _JDDEX_COMBOBOX_
