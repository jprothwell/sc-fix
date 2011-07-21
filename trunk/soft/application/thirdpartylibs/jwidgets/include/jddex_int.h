
/***************************************************************************
*
* File Name : jddex_int.h
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
* Purpose			:	Delacrations for interfaces used by jdd_mmi.cpp
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
#ifndef _JDDEX_INT_
#define _JDDEX_INT_
#ifdef __cplusplus
    extern   "C"
    {
#endif
#include <ddl.h>
#include <jcal.h>
#include <jddex_window.h>
#include <jddex_label.h>
#include <jddex_button.h>
#include <jddex_radiobutton.h>
#include <jddex_checkbox.h>
#include <jddex_inputbox.h>
#include <jddex_listbox.h>
#include <jddex_combobox.h>
#include <jddex_marquee.h>
#include <jddex_rect.h>
#include <jddex_scrollbar.h>
#include <jddex_image.h>
#include <jddex_icon.h>
#include <jddex_pictogram.h>
#include <jddex_objecttext.h>
#include <jdi_cutils.h>
#include <jddex_platform.h>

#define JDDEX_MAXCHAR_LINE 255

#define jddex_min(a,b)            (((a) < (b)) ? (a) : (b))
#define JMMS_TEMP_IMAGEFILE				"/_wap_mms/mms_tempfiles/jmms_tempimage"
#define JMMS_TEMP_AUDIOFILE				"/_wap_mms/mms_tempfiles/jmms_tempaudio"
#define JMMS_TEMP_AUDIOFILE_MIDI		JMMS_TEMP_AUDIOFILE ".mid"
#define JMMS_TEMP_AUDIOFILE_WAV			JMMS_TEMP_AUDIOFILE ".wav"
#define JMMS_TEMP_AUDIOFILE_MP3			JMMS_TEMP_AUDIOFILE ".mp3"
#define JMMS_TEMP_AUDIOFILE_AMR			JMMS_TEMP_AUDIOFILE ".amr"
#define JMMS_TEMP_AUDIOFILE_AAC			JMMS_TEMP_AUDIOFILE ".aac"

typedef enum e_jddex_EventTypes
{

// short press key events
	E_JDDEX_KEY_UP,
	E_JDDEX_KEY_DOWN,
	E_JDDEX_KEY_RIGHT,
	E_JDDEX_KEY_LEFT,
    E_JDDEX_KEY_SELECT,
    E_JDDEX_KEY_CANCEL,
	E_JDDEX_KEY_CALL,
	E_JDDEX_KEY_1,
	E_JDDEX_KEY_2,
	E_JDDEX_KEY_3,
	E_JDDEX_KEY_4,
	E_JDDEX_KEY_5,
	E_JDDEX_KEY_6,
	E_JDDEX_KEY_7,
	E_JDDEX_KEY_8,
	E_JDDEX_KEY_9,
	E_JDDEX_KEY_0,
	E_JDDEX_KEY_STAR,
	E_JDDEX_KEY_HASH,
	E_JDDEX_KEY_SOFT1,
	E_JDDEX_KEY_SOFT2,

// long press key events
    E_JDDEX_KEY_LP_UP,
	E_JDDEX_KEY_LP_DOWN,
	E_JDDEX_KEY_LP_RIGHT,
	E_JDDEX_KEY_LP_LEFT,
    E_JDDEX_KEY_LP_SELECT,
    E_JDDEX_KEY_LP_CANCEL,
	E_JDDEX_KEY_LP_CALL,
	E_JDDEX_KEY_LP_END,
	E_JDDEX_KEY_LP_1,
	E_JDDEX_KEY_LP_2,
	E_JDDEX_KEY_LP_3,
	E_JDDEX_KEY_LP_4,
	E_JDDEX_KEY_LP_5,
	E_JDDEX_KEY_LP_6,
	E_JDDEX_KEY_LP_7,
	E_JDDEX_KEY_LP_8,
	E_JDDEX_KEY_LP_9,
	E_JDDEX_KEY_LP_0,
	E_JDDEX_KEY_LP_STAR,
	E_JDDEX_KEY_LP_HASH,
	E_JDDEX_KEY_LP_SOFT1,
	E_JDDEX_KEY_LP_SOFT2,
	E_JDDEX_KEY_END = E_JDDEX_KEY_LP_SOFT2

}EJddExEventTypes, E_JDDEX_EVENT_TYPES ;

typedef enum e_jddex_allign
{
	E_JDDEX_CENTRE_ALLIGN = 0x1,
	E_JDDEX_RIGHT_ALLIGN =0x2,
	E_JDDEX_LEFT_ALLIGN =0x4,
	E_JDDEX_MIDDLE_ALLIGN =0x8,//0x10,
}EJddExAllign, E_JDDEX_ALLIGN ;

typedef struct st_formatted_rect_props
{
    JC_POINT   stPoint1;
	JC_POINT   stPoint2;
	JC_POINT   stPoint3;
	JC_POINT   stPoint4;
    JC_BOOLEAN bDrawLeftBorder ;
    JC_BOOLEAN bDrawRightBorder ;
    JC_BOOLEAN bDrawTopBorder ;
    JC_BOOLEAN bDrawBottomBorder ;
} JDDEX_FORMATTED_RECT_PROPS ;

/**
 * @brief		Sets a widget event.
 *
 * @param[in]	Not Used
 * @param[in]	eEventType Specifies the browser event type
 * @param[in]	pvEventInfo Specifies the information related to the browser event.
 * @retval		E_TRUE if widget library consumes the event.
 * @retval		E_FALSE if widget library wants the core to handle
 *
 * The application needs to use this API when it comes across any event or operation that is to
 * be handled by jataayu primary MMI and based on the return value application should call the
 * the respective JDI interface of that particular application for further processing.
 */

JC_BOOLEAN jddex_WidgetHandleEvent(JC_HANDLE	vHandle,
			JC_UINT32 eEventType, void	*pvEventInfo, JC_RECT *pStylusPosition) ;



JC_RETCODE jddex_MMICreateLabel(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);


JC_RETCODE jddex_MMICreateButton(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);


JC_RETCODE jddex_MMICreateRadioButton(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);


JC_RETCODE jddex_MMICreateCheckBox(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);

JC_RETCODE jddex_MMICreateImage(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);


JC_RETCODE jddex_MMICreateIcon(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);


JC_RETCODE jddex_MMICreatePictogram(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);


JC_RETCODE jddex_MMICreateMarquee(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);


JC_RETCODE jddex_MMICreateObjectText(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);



JC_RETCODE jddex_MMICreateScrollBar(void * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);


JC_RETCODE jddex_MMICreateComboBox(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);


JC_RETCODE jddex_MMICreateListBox(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);

JC_RETCODE jddex_MMICreateInputBox(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);


JC_RETCODE jddex_MMICreateWindow (JC_WINDOW_PROPERTIES *pWinProp,
								const JC_RECT *pRect,
								JC_UINT32 *puiWindowID);

JC_RETCODE jddex_FormattedDrawText(const JC_CHAR* pszText , JC_RECT stRect, 
					JC_RECT stParentRect, JC_STYLE stTextStyle);

JC_RETCODE jddex_DrawFormattedRectEx(const JC_RECT * pstRect,
			 JC_BORDER_PROPS* stBorderProp, JC_INT32 iFillColor);


JC_RETCODE jddex_GetFormattedRect(const JC_RECT * pstRect,
			 JDDEX_FORMATTED_RECT_PROPS *pstFormattedRectProp);

JC_RETCODE jddex_MMICreateObject(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);

JC_RETCODE jddex_GetClippedRect(const JC_RECT * pstRect,
			 JC_RECT * pstClippedRect);
JC_RETCODE jddex_DrawFormattedImage (JC_OBJECT_INFO		m_stObjectInfo,
									 JC_RECT* pstRect, EImageProperty eImageProperty,
									 JC_CHAR* m_pmTempImageFile, JC_UINT32* puiAnimHnd);

JC_RETCODE	jddex_FormattedDrawCircle(JC_POINT* pstCentre, JC_INT32 iRadius,
									JC_INT32 iBorderColor, JC_INT32 thickness );
void jddex_FillChar(JC_CHAR *pszText, JC_CHAR ch , JC_UINT32 uiCnt);
JC_RETCODE jddex_FormattedDrawTextEx(const JC_CHAR* pszText ,JC_RECT stRect,
					 JC_STYLE stTextStyle, JC_UINT32 uiAllignFlags);
void jddex_DrawScrollBarControl(JC_RECT stScrollBarRect, JC_RECT stScrollCarRect );

JC_BOOLEAN jddex_IsAnimationSupprted();
void jddex_GetListBoxIConDimensions(JC_UINT32 *puiWidth, JC_UINT32 *puiHeight);
JC_RETCODE jddex_DrawFormattedListBoxIcon(JC_RECT* pstRect);
void jddex_GetAltImgIConDimensions(JC_UINT32 *puiWidth, JC_UINT32 *puiHeight);
JC_RETCODE jddex_DrawFormattedAltImageIcon(JC_RECT* pstRect, JC_RECT* pParentRect);
JC_CHAR* jddex_PlatformGetAudioType(JC_INT8 *pcAudioMimeType);

JC_RETCODE jddex_DrawFormattedRadioIcon(JC_RECT* pstRect,U16 ImageId);
JC_RETCODE jddex_DrawFormattedVideo(JC_OBJECT_INFO		m_stObjectInfo,
											JC_RECT * pstRect, JC_BOOLEAN bIsVideoHandleCreated);

#ifdef __cplusplus
    }
#endif

#endif// _JDDEX_INT_

