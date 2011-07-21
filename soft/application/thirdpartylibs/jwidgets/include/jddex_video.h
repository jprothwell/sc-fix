/***************************************************************************
*
* File Name : jddex_video.h
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
* Purpose			:	class declaration for video control
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

#ifndef _JDDEX_VIDEO_
#define _JDDEX_VIDEO_

#include <jddex_widget.h>
#include <jddex_platform.h>

class jddex_video : public jddex_widget  
{

public:
	jddex_video();
	jddex_video(jddex_widget *poParentHwnd,JC_RECT *pstRect, 
				 JC_CONTROL_PROPERTIES* pstCtrlProperty);

	virtual ~jddex_video();
	void Show();
	void HandleAction(E_OBJECT_ACTION		eAction);
	void SetVideoPlayStatus(JC_BOOLEAN bIsPlaying);
	void RedrawVideoControl();
	void CreateVideoHandle();
	void RecreateVideoHandle();
	void DeleteVideoHandle();
	
protected:
	JC_OBJECT_INFO		m_stObjectInfo;

private:
	JC_BOOLEAN m_bIsVideoPlaying;// maintains state for videoplay ongoing or completed
	JC_BOOLEAN m_bIsVideoHandleDeleted; // maintains state of video if deleted 
	JC_BOOLEAN m_bIsVideoHandleCreated; // in case platform fails in video creation
};

#endif // _JDDEX_VIDEO_
