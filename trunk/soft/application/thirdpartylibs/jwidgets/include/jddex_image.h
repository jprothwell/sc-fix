/***************************************************************************
*
* File Name : jddex_image.h
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
* Purpose			:	class declaration for image control
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

#ifndef _JDDEX_IMAGE_
#define _JDDEX_IMAGE_

#include <jddex_widget.h>
#include <jddex_platform.h>

class jddex_image : public jddex_widget  
{

public:
	jddex_image();
	jddex_image(jddex_widget *poParentHwnd,JC_RECT *pstRect, 
				 JC_CONTROL_PROPERTIES* pstCtrlProperty,JC_BOOLEAN* pIsSucess,
				 JC_CHAR* pmTempImageFile);

	jddex_image(jddex_widget *poParentHwnd,JC_RECT *pstRect, 
				 JC_OBJECT_INFO* pstImageInfo, EImageProperty  eImageProperty, 
				 JC_BOOLEAN* pIsSucess,JC_CHAR* pmTempImageFile);
	virtual ~jddex_image();
	void Show();
	EImageProperty GetImageProperty();
	
protected:
	JC_OBJECT_INFO		m_stObjectInfo;
	void				*m_pvDecodedBuff;
	void				*m_pvGifFrameBuff;
	EImageProperty      m_eImageProperty;
	JC_UINT32 			m_uiDecodedWidth ;
	JC_UINT32 			m_uiDecodedHeight;
	JC_BOOLEAN 			m_uiAnimPlayInitiated;
	JC_UINT32			m_uiAnimHnd;
	JC_CHAR				*m_pmTempImageFile;
};

#endif // _JDDEX_IMAGE_
