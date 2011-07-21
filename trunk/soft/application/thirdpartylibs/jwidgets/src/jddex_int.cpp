/***************************************************************************
*
* File Name : jdd_int.cpp
*
* IMPORTANT NOTICE
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
* Purpose			:	Interfaces of JDD MMI
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

#include <jddex_platformdefines.h>


#include <jddex_window.h>
#include <jddex_platform.h>
#include <jddex_int.h>
#include <jddex_wndutils.h>
#include <ddl.h>
#include <jcal.h>
#include "jdd_logapi.h"


#include "jwap_interface.h"

#include <jdi_browserengine.h>
#include <jdi_xmldatatype.h>
#include <jdi_history.h>
#include <jdi_cutils_time.h>
#include <jdi_de.h>
#include <jddex_animationinterface.h>
#include <jddex_image.h>
#include <jddex_audio.h>
#include <jddex_video.h>
#include "jddex_wnddefaults.h"
/* Following #define is a Temporary Fix to make widgets compile using jwap include files */
//#define __JATAAYU_APP__
#ifdef __cplusplus
    extern   "C"
    {
#endif
JC_UINT32 jwap_GetPrimaryWindow();
void* 	jWap_Get_Brw_Handle();

#ifdef __cplusplus
	}
#endif
#include "jwap_interface.h"

#define JDDEX_LISTBOX_ICON_HT_WT 16
#define JDDEX_RADIO_ICON_HT_WT 10

extern JC_UINT32 g_uiControlId;
extern U8* g_field_state;
extern JDDEX_LIST_BOX	   g_pstListBox;
extern U8**  g_TempListItems;
extern JC_CHAR* g_pszInputText ;
extern JC_CHAR* g_pszInputOrigText ;
extern JC_UINT32 g_uiMaxlen ;



static E_LINE_STYLE GetMappedBorderEnum(E_BORDER_STYLE eBorderStyle);

void jddex_GetWidgetRect(JC_UINT32 uicontrolId, JC_RECT* pstRect )
{
	jddex_window * poWindow = (jddex_window *)jwap_GetPrimaryWindow();
	if (poWindow && poWindow->IsValidWidget((jddex_widget*)uicontrolId))
	{
		jddex_widget * poWidget = (jddex_widget*)uicontrolId;	
		poWidget->GetRect (pstRect) ;
	}

}

#ifdef JATAAYU_SUPPORT_WAP

JC_BOOLEAN jddex_WidgetHandleEvent(JC_HANDLE	vHandle,
				JC_UINT32 eEventType, void	*pvEventInfo, JC_RECT *pStylusPosition)
{


	ST_BE_CONTROL_INFO		stCtrlInfo = {E_TEXT_BOX, } ;
	jc_memset(&stCtrlInfo, 0, sizeof(ST_BE_CONTROL_INFO));
	JC_RETCODE retCode ;
	JC_BOOLEAN	bIsHandled = E_FALSE ;
	jddex_widget * poWidget ;
	JC_RECT			stFocusedCtrlRect = {0, } ;
		
	retCode = jdi_BEGetFocusedControlInfo (vHandle, &stCtrlInfo) ;

	poWidget = (jddex_widget *) stCtrlInfo.uiCtrlID ;
	//JDD_LOG_INFO1("jddex_WidgetHandleEvent poWidget %d",poWidget);
	//TBD remove typecast
	if (poWidget)
	{
		JDDEX_DEVICE_CHAR	stDeviceChar = {0, } ;
		jddex_GetDeviceProp (JDDEX_BROWSER_APP, &stDeviceChar) ;
		poWidget->GetRect (&stFocusedCtrlRect) ;
		stFocusedCtrlRect.iLeft += stDeviceChar.uiLeft ;
		stFocusedCtrlRect.iTop += stDeviceChar.uiTop ;
		if (JC_NULL == pStylusPosition ||
			(pStylusPosition->iLeft >= stFocusedCtrlRect.iLeft &&
			pStylusPosition->iLeft < stFocusedCtrlRect.iLeft + (JC_INT32)stFocusedCtrlRect.uiWidth &&
			pStylusPosition->iTop >= stFocusedCtrlRect.iTop &&
			pStylusPosition->iTop < stFocusedCtrlRect.iTop + (JC_INT32)stFocusedCtrlRect.uiHeight))
		{
			retCode = poWidget->EventHandler((JC_UINT32)vHandle,eEventType,pvEventInfo);
			if (JC_OK == retCode)
			{
				bIsHandled = E_TRUE ;
			}
		}
	}
	//TBD.
	return bIsHandled ;
	
}
#endif
/**
 * @brief 		interface to create a label control 
 * @param[in]	poWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for label/mrquee class
 */
JC_RETCODE jddex_MMICreateLabel(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	if (E_TEXT_NORMAL == pstControlProperty->typeOfControl.textBox.etextStyle)
	{
			jddex_label *poLabel;
			//JDD_LOG_INFO1((PCSTR)">>>>>>>>>>jddex_MMICreateLabel%d",pstControlProperty->controlStyle.iForegroundColor);
			//JDD_LOG_INFO1((PCSTR)">>>>>>>>>>jddex_MMICreateLabelr%d",pstControlProperty->controlStyle.iBackgroundColor);
			poLabel = new jddex_label(poWindow,pstRect,pstControlProperty);
				
			*puiControlID = (JC_UINT32) poLabel ;
			poWindow->AddToChildList(poLabel);
	}
	else if (E_TEXT_MARQUEE == pstControlProperty->typeOfControl.textBox.etextStyle||
				E_TEXT_BLINK == pstControlProperty->typeOfControl.textBox.etextStyle)
	{
			jddex_marquee *poMarquee;

			poMarquee = new jddex_marquee(poWindow,pstRect,pstControlProperty);
					
			*puiControlID = (JC_UINT32) poMarquee ;
			poWindow->AddToChildList(poMarquee);
	}
	else 
	{
		jdd_AssertOnFail(E_FALSE, "Not Implimented") ;
	}
	return JC_OK;
}

/**
 * @brief 		interface to create a button control 
 * @param[in]	poWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for button class
 */
JC_RETCODE jddex_MMICreateButton(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_button *poButton;

	  mmi_trace(1,(PCSTR)"jddex_MMICreateButton x[%d] y[%d] width[%d] height[%d]",pstRect->iLeft,
		pstRect->iTop,pstRect->uiWidth,pstRect->uiHeight);

	poButton = new jddex_button(poWindow,pstRect,pstControlProperty);
	
		
	*puiControlID = (JC_UINT32) poButton ;
	poWindow->AddToChildList(poButton);

	return JC_OK;

}
/**
 * @brief 		interface to create a radio button control 
 * @param[in]	poWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for radio button class
 */

JC_RETCODE jddex_MMICreateRadioButton(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_radiobutton *poRadioButton;



	poRadioButton = new jddex_radiobutton(poWindow,pstRect,pstControlProperty);
			
	*puiControlID = (JC_UINT32) poRadioButton ;
	poWindow->AddToChildList(poRadioButton);

	return JC_OK;

}

/**
 * @brief 		interface to create a check box control 
 * @param[in]	pstWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for check box class
 */
JC_RETCODE jddex_MMICreateCheckBox(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_checkbox *poCheckBox;

	poCheckBox = new jddex_checkbox(poWindow,pstRect,pstControlProperty);
	
		
	*puiControlID = (JC_UINT32) poCheckBox ;
	poWindow->AddToChildList(poCheckBox);

	return JC_OK;

}

/**
 * @brief 		interface to create a label control 
 * @param[in]	pstWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for iamge class
 */
JC_RETCODE jddex_MMICreateImage(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_image *poImage;
	JC_BOOLEAN IsSucess;


	//poImage = new jddex_image(poWindow,pstRect,pstControlProperty, &IsSucess);
	if(pstControlProperty->typeOfControl.object.bIsFileContent == E_FALSE)
	{
#ifdef USE_FILE_FOR_IMGBUFF_DRAW
		JC_CHAR *pmTempImageFile = JC_NULL;
		JDD_FILE hTempFile  = JC_NULL;
		jdi_CUtilsCharToTchar (JC_NULL, JMMS_TEMP_IMAGEFILE, &pmTempImageFile) ;
		hTempFile = jdd_FSOpen (JC_NULL, pmTempImageFile, E_CREATE_WRITE_MODE) ;	
		jdd_FSWrite ((void *)(pstControlProperty->typeOfControl.object.uBuffer.pucBuffer),
					pstControlProperty->typeOfControl.object.uiObjectLen, 1, hTempFile);
		jdd_FSClose(hTempFile);
			
		poImage = new jddex_image(poWindow, pstRect, pstControlProperty, &IsSucess, pmTempImageFile);
#else	
		poImage = new jddex_image(poWindow, pstRect, pstControlProperty, &IsSucess, JC_NULL);
#endif
	}
	else
	{
		poImage = new jddex_image(poWindow, pstRect, pstControlProperty, &IsSucess, JC_NULL);
	}
		
	if( E_FALSE == IsSucess)
	{
		delete poImage;
		return  JC_ERR_GENERAL_FAILURE;
	}
	else
	{
	*puiControlID = (JC_UINT32) poImage ;
	poWindow->AddToChildList(poImage);

	return JC_OK;
	}

}

//static JC_CHAR* jddex_PlatformGetAudioType(JC_INT8 *pcAudioMimeType);
/**
 * @brief 		interface to create a object control 
 * @param[in]	pstWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for object class
 */
JC_RETCODE jddex_MMICreateObject(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	JC_RETCODE rCode = JC_OK;

	mmi_trace(0 , (PCSTR)"jwidget_log: MMICreateObject type[%d]", \
				pstControlProperty->typeOfControl.object.eType );
	if (E_OBJECT_IMAGE == pstControlProperty->typeOfControl.object.eType)
	{
		jddex_image *poImage;
		JC_BOOLEAN IsSucess;
		if (E_TRUE == pstControlProperty->bReturnDimension)			
		{
		
			jdd_MMIGetObjectSize((JC_UINT32)poWindow,&pstControlProperty->typeOfControl.object,
				JC_NULL,&pstRect->uiWidth,&pstRect->uiHeight,JC_NULL);
		}

		if(!pstRect->uiWidth || !pstRect->uiHeight)
		{
			rCode = JC_ERR_GENERAL_FAILURE;
			goto EXIT;
		}
		
		if(pstControlProperty->typeOfControl.object.bIsFileContent == E_FALSE)
		{
#ifdef USE_FILE_FOR_IMGBUFF_DRAW
			JC_CHAR *pmTempImageFile = JC_NULL;
			JDD_FILE hTempFile  = JC_NULL;
			jdi_CUtilsCharToTchar (JC_NULL, JMMS_TEMP_IMAGEFILE, &pmTempImageFile) ;
			hTempFile = jdd_FSOpen (JC_NULL, pmTempImageFile, E_CREATE_WRITE_MODE) ;	
			jdd_FSWrite ((void *)(pstControlProperty->typeOfControl.object.uBuffer.pucBuffer),
						pstControlProperty->typeOfControl.object.uiObjectLen, 1, hTempFile);
			jdd_FSClose(hTempFile);
			
				
			poImage = new jddex_image(poWindow, pstRect, pstControlProperty, &IsSucess, pmTempImageFile);
#else
			poImage = new jddex_image(poWindow, pstRect, pstControlProperty, &IsSucess, JC_NULL);
#endif
		}
		else
		{
			poImage = new jddex_image(poWindow, pstRect, pstControlProperty, &IsSucess, JC_NULL);
		}		

		if( E_FALSE == IsSucess)
		{
			delete poImage;
			rCode = JC_ERR_GENERAL_FAILURE;
		}
		else
		{
			*puiControlID = (JC_UINT32) poImage ;
			poWindow->AddToChildList(poImage);
		}
	}
	else if (E_OBJECT_AUDIO == pstControlProperty->typeOfControl.object.eType)
	{
		jddex_audio *poAudio;

		if(pstControlProperty->typeOfControl.object.bIsFileContent == E_FALSE)
		{
#ifdef USE_FILE_FOR_AUDIOBUFF_PLAY			
			JC_CHAR *pmAudioTempFile;
			JDD_FILE hTempFile  = JC_NULL;
			pmAudioTempFile = jddex_PlatformGetAudioType(pstControlProperty->typeOfControl.object.psMimeType);
			hTempFile = jdd_FSOpen (JC_NULL, pmAudioTempFile, E_CREATE_WRITE_MODE) ;	
			jdd_FSWrite ((void *)(pstControlProperty->typeOfControl.object.uBuffer.pucBuffer),
									pstControlProperty->typeOfControl.object.uiObjectLen, 
							1, hTempFile);
			jdd_FSClose(hTempFile);
			poAudio = new jddex_audio(poWindow,pstRect,pstControlProperty, pmAudioTempFile);
#else
		   poAudio = new jddex_audio(poWindow,pstRect,pstControlProperty, JC_NULL);
#endif
		}
		else
		{
			poAudio = new jddex_audio(poWindow,pstRect,pstControlProperty, JC_NULL);
		}
				
		*puiControlID = (JC_UINT32) poAudio ;
		poWindow->AddToChildList(poAudio);

	}
	else if (E_OBJECT_VIDEO == pstControlProperty->typeOfControl.object.eType)
	{
		jddex_video *poVideo;

		poVideo = new jddex_video(poWindow,pstRect,pstControlProperty);

			
		*puiControlID = (JC_UINT32) poVideo ;
		poWindow->AddToChildList(poVideo);


	}
	else if (E_OBJECT_OTHER == pstControlProperty->typeOfControl.object.eType)
	{
		jddex_objecttext *pstObjectText;

		pstObjectText = new jddex_objecttext(poWindow,pstRect,pstControlProperty);

			
		*puiControlID = (JC_UINT32) pstObjectText ;
		poWindow->AddToChildList(pstObjectText);

	}
EXIT:
	return rCode;
}

JC_CHAR* jddex_PlatformGetAudioType(JC_INT8 *pcAudioMimeType)
{
	JC_CHAR * pmFName = JC_NULL;
	mmi_trace(1,(PCSTR)"jddex_PlatformGetAudioType called");
   	if(jc_strstr(pcAudioMimeType , "audio/mid") ||
		jc_strstr(pcAudioMimeType,"audio/midi")||
		jc_strstr(pcAudioMimeType,"audio/sp-midi")||
        jc_strstr(pcAudioMimeType,"audio/imy") ||
		jc_strstr(pcAudioMimeType,"audio/x-imy") )
	{
         jdi_CUtilsCharToTchar (JC_NULL, JMMS_TEMP_AUDIOFILE_MIDI, &pmFName) ;
	}
	else if(jc_strstr(pcAudioMimeType , "audio/wav")||
		jc_strstr(pcAudioMimeType,"audio/x-wav"))
	{
         jdi_CUtilsCharToTchar (JC_NULL, JMMS_TEMP_AUDIOFILE_WAV, &pmFName) ;
	}
	else if(jc_strstr(pcAudioMimeType, "audio/mp3") ||
            jc_strstr(pcAudioMimeType, "audio/mpeg")||
            jc_strstr(pcAudioMimeType, "audio/x-mpeg-3"))
	{
		jdi_CUtilsCharToTchar (JC_NULL, JMMS_TEMP_AUDIOFILE_MP3, &pmFName) ;
	}
	else if(jc_strstr(pcAudioMimeType, "audio/amr"))
	{
		jdi_CUtilsCharToTchar (JC_NULL, JMMS_TEMP_AUDIOFILE_AMR, &pmFName) ;
	}
	else if(jc_strstr(pcAudioMimeType, "audio/aac"))
	{
		jdi_CUtilsCharToTchar (JC_NULL, JMMS_TEMP_AUDIOFILE_AAC, &pmFName) ;
	}
	else
	{
		mmi_trace(1,(PCSTR)"Un supported audio");
		jdi_CUtilsCharToTchar (JC_NULL, JMMS_TEMP_AUDIOFILE_MIDI, &pmFName) ;
	}
	return pmFName ;
}


/**
 * @brief 		interface to create a icon control 
 * @param[in]	pstWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for icon class
 */
JC_RETCODE jddex_MMICreateIcon(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_icon *poIcon;
	poIcon = new jddex_icon(poWindow,pstRect,pstControlProperty);
		
	*puiControlID = (JC_UINT32) poIcon ;
	poWindow->AddToChildList(poIcon);

	return JC_OK;
}

/**
 * @brief 		interface to create a pictogram control 
 * @param[in]	pstWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for pictogram class
 */
JC_RETCODE jddex_MMICreatePictogram(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_pictogram *poPictogram;

	poPictogram = new jddex_pictogram(poWindow,pstRect,pstControlProperty);
		
	*puiControlID = (JC_UINT32) poPictogram ;
	poWindow->AddToChildList(poPictogram);

	return JC_OK;

}

/**
 * @brief 		interface to create a marquee control 
 * @param[in]	poWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for marquee class
 */

JC_RETCODE jddex_MMICreateMarquee(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_marquee *poMarquee;

	poMarquee = new jddex_marquee(poWindow,pstRect,pstControlProperty);

		
	*puiControlID = (JC_UINT32) poMarquee ;
	poWindow->AddToChildList(poMarquee);

	return JC_OK;

}

/**
 * @brief 		interface to create a ObjectText control 
 * @param[in]	pstWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for ObjectText class
 */
JC_RETCODE jddex_MMICreateObjectText(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_objecttext *poObjectText;

	poObjectText = new jddex_objecttext(poWindow,pstRect,pstControlProperty);

		
	*puiControlID = (JC_UINT32) poObjectText ;
	poWindow->AddToChildList(poObjectText);

	return JC_OK;

}

/**
 * @brief 		interface to create a ScrollBar control 
 * @param[in]	poWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for ScrollBar class
 */
JC_RETCODE jddex_MMICreateScrollBar( void* poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_window *poScrollwindow = (jddex_window *)poWindow;
	jddex_scrollbar *poScrollbar;
	mmi_trace(1,(PCSTR)"jddex_MMICreateScrollBar called");
	poScrollbar = new jddex_scrollbar(poScrollwindow,pstRect,pstControlProperty);

		
	*puiControlID = (JC_UINT32) poScrollbar ;
	poScrollwindow->AddToChildList(poScrollbar);

	return JC_OK;

}
/**
 * @brief 		interface to create a ComboBox control 
 * @param[in]	poWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for ComboBox class
 */
JC_RETCODE jddex_MMICreateComboBox(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_combobox *poComboBox;
	poComboBox = new jddex_combobox(poWindow,pstRect,pstControlProperty);

		
	*puiControlID = (JC_UINT32) poComboBox ;
	poWindow->AddToChildList(poComboBox);

	return JC_OK;

}

/**
 * @brief 		interface to create a ListBox control 
 * @param[in]	poWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for ListBox class
 */
JC_RETCODE jddex_MMICreateListBox(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_listbox *poListBox;
	poListBox = new jddex_listbox(poWindow,pstRect,pstControlProperty);
		
	*puiControlID = (JC_UINT32) poListBox ;
	poWindow->AddToChildList(poListBox);

	return JC_OK;

}


/**
 * @brief 		interface to create a InputBox control 
 * @param[in]	poWindow - handle to window
 * @param[in]	eControlType -  control type
 * @param[in]	pstControlProperty - control properties
 * @param[in]	pstRect control co-ordiantes
 * @param[OUT]	puiControlID control handle
 * @retval		JC_OK
 * creates a object for InputBox class
 */
JC_RETCODE jddex_MMICreateInputBox(jddex_window * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
	jddex_inputbox *poInputBox;
	poInputBox = new jddex_inputbox(poWindow,pstRect,pstControlProperty);
		  mmi_trace(1,(PCSTR)"widget_log: jddex_MMICreateInputBox x[%d] y[%d] width[%d] height[%d]",pstRect->iLeft,
		pstRect->iTop,pstRect->uiWidth,pstRect->uiHeight);

		
	*puiControlID = (JC_UINT32) poInputBox ;
	poWindow->AddToChildList(poInputBox);

	return JC_OK;

}

/**
 * @brief 		interface to create a window 
 * @param[in]	pWinProp - window properties
 * @param[in]	pRect  - window co-ordiantes
 * @param[OUT]	puiWindowID window handle
 * @retval		JC_OK
 * creates a object for window class
 */
JC_RETCODE jddex_MMICreateWindow (JC_WINDOW_PROPERTIES *pWinProp,
								const JC_RECT *pRect,
								JC_UINT32 *puiWindowID)
{
	jddex_window * poWindow;
	jddex_window * poParentWindow = (jddex_window *)pWinProp->uiParentID;

	
	poWindow = new jddex_window(poParentWindow, (JC_RECT *)pRect, pWinProp);
	*puiWindowID = (JC_UINT32) poWindow ;


	if(poParentWindow)
		poParentWindow->AddToChildList(poWindow);
	return JC_OK;
}




/**
 * @brief 		formats the text w.r.t LCD display
 * @param[in]	pszText - text to be dispalyed
 * @param[in]	stRect  - dispaly co-ordiantes
 * @param[OUT]	stTextStyle text style
 * @retval		JC_OK
 * draws the text based on the co-ordiantes of the LCD
 */
JC_RETCODE jddex_FormattedDrawText(const JC_CHAR* pszText , JC_RECT stRect, 
					JC_RECT stParentRect, JC_STYLE stTextStyle)
{
	JDDEX_DEVICE_CHAR stDeviceChar ;
	//JC_RECT stFormattedRect = {0,};
	JC_UINT32 uiHeight;

	if(!pszText)
	{
		return JC_OK;
	}

	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);


#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( stRect.iTop < 0 )
	{
		return JC_OK;
	}

#else
	if( stRect.iTop < (JC_INT32)stDeviceChar.uiTop )
	{
		return JC_OK;
	}
#endif

#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( (JC_INT32)(stRect.iTop + stRect.uiHeight) >
			(JC_INT32)(stDeviceChar.uiActualHeight ))
	{
		return JC_OK;
	}
#else
	if( (JC_INT32)(stRect.iTop + stRect.uiHeight) >
			(JC_INT32)(stDeviceChar.uiTop + stDeviceChar.uiRenderingHeight - 1))
	{
		return JC_OK;
	}
#endif

    uiHeight =  jddex_PlatformGetTextHeight(&stTextStyle, pszText);
	if(stRect.uiHeight < uiHeight )
	{

		return JC_OK;
	}

	jddex_PlatformDrawLabel( pszText , stRect, stParentRect, stTextStyle, E_FALSE);
	return JC_OK;
}

/**
 * @brief 		formats the text w.r.t LCD display
 * @param[in]	pszText - text to be dispalyed
 * @param[in]	stRect  - dispaly co-ordiantes
 * @param[OUT]	stTextStyle text style
 * @retval		JC_OK
 * draws the text based on the co-ordiantes of the LCD
 */


JC_RETCODE jddex_FormattedDrawTextEx(const JC_CHAR* pszText ,JC_RECT stRect, 
					 JC_STYLE stTextStyle, JC_UINT32 uiAllignFlags)
{
	JDDEX_DEVICE_CHAR stDeviceChar ;
	//JC_RECT stFormattedRect = {0,};
	JC_CHAR sz_DisplayBuff[JDDEX_MAXCHAR_LINE+1]= {0,};
	JC_TEXTMETRICS stTextMetrics;
	JC_RECT stDispRect={0,} ;
	JC_UINT32 uiLen;

	if(!pszText)
	{
		return JC_OK;
	}

	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);


#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( stRect.iTop < 0 )
	{
		return JC_OK;
	}

#else
	if( stRect.iTop < (JC_INT32)stDeviceChar.uiTop )
	{
		return JC_OK;
	}
#endif

#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( (JC_INT32)(stRect.iTop + stRect.uiHeight) >
			(JC_INT32)(stDeviceChar.uiActualHeight))
	{
		return JC_OK;
	}
#else
	if( (JC_INT32)(stRect.iTop + stRect.uiHeight) >
			(JC_INT32)(stDeviceChar.uiTop + stDeviceChar.uiRenderingHeight - 1))
	{
		return JC_OK;
	}
#endif
	

    jddex_PlatformGetTextMetrics ( &stTextStyle, 
								  pszText, 
								  stRect.uiWidth,
								  &stTextMetrics);

	if(stRect.uiHeight < stTextMetrics.uiHeight )
	{

		return JC_OK;
	}

	uiLen = jc_tcslen(pszText);

	if( uiLen > stTextMetrics.uiNumCharacters )
		uiLen = stTextMetrics.uiNumCharacters ;


	jc_tcsncpy(sz_DisplayBuff,pszText,uiLen) ;
	stDispRect = stRect;
	if( uiAllignFlags & E_JDDEX_CENTRE_ALLIGN)
	{
		JC_INT32 iPixel = ( stRect.uiWidth - stTextMetrics.uiWidth )/2 + 1;
		stDispRect.iLeft += iPixel;
		stDispRect.uiWidth -= iPixel;

	}
    else if( uiAllignFlags & E_JDDEX_LEFT_ALLIGN)
	{
		stDispRect.iLeft   += 2;
		stDispRect.uiWidth -= 2;
	}
	
	if( uiAllignFlags & E_JDDEX_MIDDLE_ALLIGN)
	{
		stDispRect.iTop += ( stRect.uiHeight - stTextMetrics.uiHeight )/2;
	}
	else
	{
		stDispRect.iTop += 2;
	}
	
	jddex_PlatformDrawText( sz_DisplayBuff , stDispRect, stTextStyle, E_TRUE);


	return JC_OK;
}


JC_RETCODE	jddex_FormattedDrawCircle(JC_POINT* pstCentre, JC_INT32 iRadius,
									JC_INT32 iBorderColor, JC_INT32 thickness )
{
	JDDEX_DEVICE_CHAR stDeviceChar ;


	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);

#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( pstCentre->uiY - iRadius <  0)
	{
		return JC_OK;
	}
#else
	if( pstCentre->uiY - iRadius < (JC_INT32)stDeviceChar.uiTop )
	{
		return JC_OK;
	}

#endif


#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( (JC_INT32)(pstCentre->uiY + iRadius) >
			(JC_INT32)(stDeviceChar.uiActualHeight))
	{
		return JC_OK;
	}

#else
	if( (JC_INT32)(pstCentre->uiY + iRadius) >
			(JC_INT32)(stDeviceChar.uiTop + stDeviceChar.uiRenderingHeight))
	{
		return JC_OK;
	}
#endif

	jddex_PlatformDrawCircle(pstCentre, iRadius,
									iBorderColor, thickness );
	return JC_OK;
}



/**
 * @brief 		Gets the formatted rectangle which is in visible area
 * @param[in]	pstRect - actual rectangle
 * @param[in]	pstFormattedRectProp  - visible rectangle
 * @retval		JC_OK
 * 
 */
JC_RETCODE jddex_GetFormattedRect(const JC_RECT * pstRect, 
			 JDDEX_FORMATTED_RECT_PROPS *pstFormattedRectProp)
{
	JDDEX_DEVICE_CHAR stDeviceChar ;
	JC_INT32 uiExcessTopHeight, uiExcessBottomHeight ;
	JC_POINT stPoint1, stPoint2 ,stPoint3, stPoint4 ;
	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);
	JC_RETCODE retCode;
   
	jc_memset(pstFormattedRectProp, 0 , sizeof(JDDEX_FORMATTED_RECT_PROPS));

	uiExcessBottomHeight = (pstRect->iTop + pstRect->uiHeight) - (stDeviceChar.uiTop + 
															stDeviceChar.uiRenderingHeight -1);
	uiExcessTopHeight =  stDeviceChar.uiTop - pstRect->iTop;

	if( (JC_INT32)(pstRect->iTop + pstRect->uiHeight)  < stDeviceChar.uiTop)
	{
		retCode = E_TRUE;
		goto EXIT_LABEL;
	}

	if( pstRect->iTop >= (JC_INT32)(stDeviceChar.uiTop + stDeviceChar.uiRenderingHeight))
	{
		retCode = E_TRUE;
		goto EXIT_LABEL;
	}

	// Full rectangle to be drawn
	if( uiExcessBottomHeight <= 0 && uiExcessTopHeight <= 0)
	{
		pstFormattedRectProp->bDrawBottomBorder = E_TRUE;
		pstFormattedRectProp->bDrawLeftBorder = E_TRUE;
		pstFormattedRectProp->bDrawTopBorder = E_TRUE;
		pstFormattedRectProp->bDrawRightBorder = E_TRUE;


		stPoint1.uiX = pstRect->iLeft ;
		stPoint1.uiY = pstRect->iTop ;

		// draw second vertical line ....
		stPoint2.uiX = pstRect->iLeft + pstRect->uiWidth ;
		stPoint2.uiY = pstRect->iTop;

		// draw second vertical line ....
		stPoint3.uiX = pstRect->iLeft;
		stPoint3.uiY = pstRect->iTop + pstRect->uiHeight ;

		stPoint4.uiX = pstRect->iLeft + pstRect->uiWidth  ;
		stPoint4.uiY = pstRect->iTop + pstRect->uiHeight;
		//jc_memcpy (&pstFormattedRectProp->stRect, &pstRect, sizeof(JC_RECT)) ;

		retCode= E_TRUE;
	}
	// both top and bottom are cut
	else if ( uiExcessBottomHeight > 0 && uiExcessTopHeight > 0)
	{
		// draw first vertical line ....
		stPoint1.uiX = pstRect->iLeft ;
		stPoint1.uiY = stDeviceChar.uiTop ;

		stPoint3.uiX = pstRect->iLeft ;
		stPoint3.uiY = stDeviceChar.uiTop + pstRect->uiHeight - uiExcessTopHeight - uiExcessBottomHeight;
     
		// draw second vertical line ....
		stPoint2.uiX = pstRect->iLeft + pstRect->uiWidth ;
		stPoint2.uiY = stDeviceChar.uiTop ;

		stPoint4.uiX = pstRect->iLeft + pstRect->uiWidth  ;
		stPoint4.uiY = stDeviceChar.uiTop + pstRect->uiHeight - uiExcessTopHeight- uiExcessBottomHeight;



		pstFormattedRectProp->bDrawLeftBorder = E_TRUE;
		pstFormattedRectProp->bDrawRightBorder = E_TRUE;

		retCode= E_TRUE;
	}
	//cut at bottom
	else if( uiExcessBottomHeight > 0 )
	{

		// draw horizontal line ....
		stPoint1.uiX = pstRect->iLeft ;
		stPoint1.uiY = pstRect->iTop ;

		stPoint2.uiX = pstRect->iLeft + pstRect->uiWidth ;
		stPoint2.uiY = pstRect->iTop ;

		// draw first vertical line ....
		stPoint1.uiX = pstRect->iLeft ;
		stPoint1.uiY = pstRect->iTop ;

		stPoint3.uiX = pstRect->iLeft ;
		stPoint3.uiY = pstRect->iTop + pstRect->uiHeight - uiExcessBottomHeight;

 		// draw second vertical line ....
 
		stPoint2.uiX = pstRect->iLeft + pstRect->uiWidth ;
		stPoint2.uiY = pstRect->iTop ;

		stPoint4.uiX = pstRect->iLeft + pstRect->uiWidth ;
		stPoint4.uiY = pstRect->iTop + pstRect->uiHeight - uiExcessBottomHeight;


		
		pstFormattedRectProp->bDrawLeftBorder = E_TRUE;
		pstFormattedRectProp->bDrawTopBorder = E_TRUE;
		pstFormattedRectProp->bDrawRightBorder = E_TRUE;
		retCode = E_TRUE;
	}	
	// cut on top
	else if( uiExcessTopHeight > 0)
	{
		// draw first vertical line ....
		stPoint1.uiX = pstRect->iLeft ;
		stPoint1.uiY = stDeviceChar.uiTop ;

		stPoint3.uiX = pstRect->iLeft ;
		stPoint3.uiY = stDeviceChar.uiTop + pstRect->uiHeight - uiExcessTopHeight;

 		// draw second vertical line ....
		stPoint2.uiX = pstRect->iLeft + pstRect->uiWidth ;
		stPoint2.uiY = stDeviceChar.uiTop ;

		stPoint4.uiX = pstRect->iLeft + pstRect->uiWidth  ;
		stPoint4.uiY = stDeviceChar.uiTop + pstRect->uiHeight - uiExcessTopHeight;

	   
		 // draw horizontal line ....
		stPoint3.uiX = pstRect->iLeft  ;
		stPoint3.uiY = stDeviceChar.uiTop + pstRect->uiHeight - uiExcessTopHeight; ;

		stPoint4.uiX = pstRect->iLeft + pstRect->uiWidth ;
		stPoint4.uiY = stDeviceChar.uiTop + pstRect->uiHeight - uiExcessTopHeight;

		pstFormattedRectProp->bDrawBottomBorder = E_TRUE;
		pstFormattedRectProp->bDrawLeftBorder = E_TRUE;
		pstFormattedRectProp->bDrawRightBorder = E_TRUE;

		retCode= E_TRUE;
 	}
	// reduce the pixel in height and width to accomidate the border thickness
	stPoint4.uiX -= 1 ;
	stPoint2.uiX -= 1 ;

	stPoint3.uiY -= 1 ;
	stPoint4.uiY -= 1 ;

	pstFormattedRectProp->stPoint1 = stPoint1;
	pstFormattedRectProp->stPoint2 = stPoint2;
	pstFormattedRectProp->stPoint3 = stPoint3;
	pstFormattedRectProp->stPoint4 = stPoint4;

EXIT_LABEL:
	return retCode;
}

/**
 * @brief 		Draw the rectangle which is in visible area
 * @param[in]	pstChildRect - actual rectangle co-ordiantes
 * @param[in]	iBorderColor  - rectangle border color
 * @param[in]	iFillColor  - rectangle fill color
 * @retval		none
 * 
 */
void jddex_DrawFormattedRect(JC_RECT * pstChildRect,JC_INT32 iBorderColor, JC_INT32 iFillColor, JC_INT32 iWidth)
{
	JDDEX_FORMATTED_RECT_PROPS stFormattedRectProp;
	JC_RECT  stFormattedRect ;

    mmi_trace(1,(PCSTR)"widget_log: DrawFormattedRect x[%d] y[%d] width[%d] height[%d]",pstChildRect->iLeft,
	 	pstChildRect->iTop,pstChildRect->uiWidth,pstChildRect->uiHeight);
	if(jddex_GetFormattedRect(pstChildRect, &stFormattedRectProp) == E_FALSE)
	   return;

	stFormattedRect.iLeft		= stFormattedRectProp.stPoint1.uiX;
	stFormattedRect.iTop		= stFormattedRectProp.stPoint1.uiY;
	// when stFormattedRect is used we call the jddex_PlatformDrawRect, the width has to eb corrected back
	// which was modified in jddex_GetFormattedRect for border thickness.
	stFormattedRect.uiWidth		= stFormattedRectProp.stPoint2.uiX - stFormattedRectProp.stPoint1.uiX + 1;
	stFormattedRect.uiHeight	= stFormattedRectProp.stPoint3.uiY - stFormattedRectProp.stPoint1.uiY + 1;
	 //mmi_trace(1,(PCSTR)"jddex_DrawFormattedRect x[%d] y[%d] width[%d] height[%d]",stFormattedRect.iLeft,
	 //	stFormattedRect.iTop,stFormattedRect.uiWidth,stFormattedRect.uiHeight);
 	
	
	if(stFormattedRectProp.bDrawTopBorder == E_TRUE &&
		stFormattedRectProp.bDrawBottomBorder == E_TRUE &&
		stFormattedRectProp.bDrawLeftBorder == E_TRUE &&
		stFormattedRectProp.bDrawRightBorder == E_TRUE)
	{
		//mmi_trace(1,(PCSTR)"all borders true");
		jddex_PlatformDrawRect(&stFormattedRect, iBorderColor, iFillColor);
		return;
	}

	jddex_PlatformFillRect(&stFormattedRect, iFillColor);
	if(stFormattedRectProp.bDrawTopBorder == E_TRUE)
	{
		//mmi_trace(1,(PCSTR)"bDrawTopBorder true");
		jddex_PlatformDrawLineEx(&stFormattedRectProp.stPoint1, 
					&stFormattedRectProp.stPoint2, iBorderColor, iWidth);
	}
	if(stFormattedRectProp.bDrawBottomBorder == E_TRUE)
	{
		//mmi_trace(1,(PCSTR)"bDrawBottomBorder true");
		jddex_PlatformDrawLineEx(&stFormattedRectProp.stPoint3, 
					&stFormattedRectProp.stPoint4, iBorderColor, iWidth);
	}
	if(stFormattedRectProp.bDrawLeftBorder == E_TRUE)
	{
		//mmi_trace(1,(PCSTR)"bDrawLeftBorder true");
		jddex_PlatformDrawLineEx(&stFormattedRectProp.stPoint1, 
					&stFormattedRectProp.stPoint3, iBorderColor, iWidth);
	}
	if(stFormattedRectProp.bDrawRightBorder == E_TRUE)
	{
		//mmi_trace(1,(PCSTR)"bDrawRightBorder true");
		jddex_PlatformDrawLineEx(&stFormattedRectProp.stPoint2, 
					&stFormattedRectProp.stPoint4, iBorderColor, iWidth);
	}

}




/**
 * @brief 		Draw the rectangle which is in visible area with border properties
 * @param[in]	pstRect - actual rectangle co-ordiantes
 * @param[in]	pstBorderProp  - rectangle border properties
 * @param[in]	iFillColor  - rectangle fill color
 * @retval		none
 * 
 */
JC_RETCODE jddex_DrawFormattedRectEx(const JC_RECT * pstRect, 
			 JC_BORDER_PROPS *pstBorderProp, JC_INT32 iFillColor)
{
	JDDEX_FORMATTED_RECT_PROPS stFormattedRectProp;
	JC_RECT  stFormattedRect ;
	//JC_BOOLEAN bAllBorderVisible = E_FALSE;

#if 0	
	mmi_trace(1,(PCSTR)"widget_log: jddex_DrawFormattedRectEx x[%d] y[%d] dx[%d] dy[%d] [%d] [%d]",
				pstRect->iLeft,pstRect->iTop,pstRect->uiWidth,pstRect->uiHeight,
				pstBorderProp->uiLeftBorderWidth,pstBorderProp->uiRightBorderWidth );
#endif

	if(jddex_GetFormattedRect(pstRect, &stFormattedRectProp) == E_FALSE)
	   return JC_OK;

#if 0
	if( (pstBorderProp->iBottomBorderStyle != E_NO_BORDER) &&
		(pstBorderProp->iLeftBorderStyle != E_NO_BORDER) &&
		(pstBorderProp->iTopBorderStyle != E_NO_BORDER) &&
		(pstBorderProp->iRightBorderStyle != E_NO_BORDER))
	{
		bAllBorderVisible = E_TRUE;
	}
#endif

	stFormattedRect.iLeft		= stFormattedRectProp.stPoint1.uiX;
	stFormattedRect.iTop		= stFormattedRectProp.stPoint1.uiY;
	stFormattedRect.uiWidth		= stFormattedRectProp.stPoint2.uiX - stFormattedRectProp.stPoint1.uiX;
	stFormattedRect.uiHeight	= stFormattedRectProp.stPoint3.uiY - stFormattedRectProp.stPoint1.uiY;
#if 0
	if( bAllBorderVisible == E_TRUE&&
		stFormattedRectProp.bDrawTopBorder == E_TRUE &&
		stFormattedRectProp.bDrawBottomBorder == E_TRUE &&
		stFormattedRectProp.bDrawLeftBorder == E_TRUE &&
		stFormattedRectProp.bDrawRightBorder == E_TRUE)
	{

		jddex_PlatformDrawRect(&stFormattedRect, pstBorderProp->iTopBorderColor, iFillColor);
		return 0;
	}
#endif
   #if 0
	mmi_trace(1,(PCSTR)"jddex_DrawFormattedRectEx x[%d] y[%d] dx[%d] dy[%d]",
		stFormattedRect.iLeft,stFormattedRect.iTop,stFormattedRect.uiWidth,stFormattedRect.uiHeight);
   #endif
       
	jddex_PlatformFillRect(&stFormattedRect, iFillColor);
	if(pstBorderProp->iBottomBorderStyle != E_NO_BORDER &&
		stFormattedRectProp.bDrawBottomBorder == E_TRUE)
	{
		jddex_PlatformDrawLine(&stFormattedRectProp.stPoint3, 
					&stFormattedRectProp.stPoint4, 
					 GetMappedBorderEnum(pstBorderProp->iBottomBorderStyle), 
					 pstBorderProp->iBottomBorderColor, pstBorderProp->uiBottomBorderWidth,
					 E_JDDEX_BOTTOM_BORDER );
	}
	if(pstBorderProp->iLeftBorderStyle != E_NO_BORDER &&
		stFormattedRectProp.bDrawLeftBorder == E_TRUE)
	{
		jddex_PlatformDrawLine(&stFormattedRectProp.stPoint1, 
					&stFormattedRectProp.stPoint3, 
					 GetMappedBorderEnum(pstBorderProp->iLeftBorderStyle), 
					 pstBorderProp->iLeftBorderColor, pstBorderProp->uiLeftBorderWidth,
					 E_JDDEX_LEFT_BORDER);
	}
	if(pstBorderProp->iTopBorderStyle != E_NO_BORDER &&
		stFormattedRectProp.bDrawTopBorder == E_TRUE)
	{
		jddex_PlatformDrawLine(&stFormattedRectProp.stPoint1, 
					&stFormattedRectProp.stPoint2, 
					 GetMappedBorderEnum(pstBorderProp->iTopBorderStyle), 
					 pstBorderProp->iTopBorderColor,pstBorderProp->uiTopBorderWidth, 
					 E_JDDEX_TOP_BORDER );
	}
	if(pstBorderProp->iRightBorderStyle != E_NO_BORDER &&
		stFormattedRectProp.bDrawRightBorder == E_TRUE)
	{
		jddex_PlatformDrawLine(&stFormattedRectProp.stPoint2, 
					&stFormattedRectProp.stPoint4, 
					 GetMappedBorderEnum(pstBorderProp->iRightBorderStyle), 
					 pstBorderProp->iRightBorderColor,pstBorderProp->uiRightBorderWidth, 
					 E_JDDEX_RIGHT_BORDER);
	
	}
	return JC_OK;
}

/**
 * @brief 		maps the jataayu broder style to widgets line style
 * @param[in]	eBorderStyle - border style
 * @retval		mapped line style
 * 
 */
static E_LINE_STYLE  GetMappedBorderEnum(E_BORDER_STYLE eBorderStyle)
{
	E_LINE_STYLE eStyle;

	switch(eBorderStyle)
	{
		case E_SOLID_BORDER:
			eStyle = E_SOLID_LINE;
			break;

		case E_DOTTED_BORDER:
			eStyle = E_DOTTED_LINE;
			break;
		case E_DASHED_BORDER:
			eStyle = E_DASHED_LINE;
			break;
		case E_DOUBLE_BORDER:
			eStyle = E_DOUBLE_LINE;
			break;
		case E_GROOVE_BORDER:
			eStyle = E_GROOVE_LINE;
			break;
		case E_RIDGE_BORDER:
			eStyle = E_RIDGE_LINE;
			break;
		case E_INSET_BORDER:
			eStyle = E_INSET_LINE;
			break;
		case E_OUTSET_BORDER:
			eStyle = E_OUTSET_LINE;
			break;
		default:
			eStyle = E_SOLID_LINE;
			break;
	}

	return eStyle;
}

/**
 * @brief 		gets the rectangle which is in visible area of image
 * @param[in]	pstRect  - image rectangle
 * @param[out]	pstBorderProp  - part of image rectangle that is in visible area
 * @retval		JC_OK
 * 
 */

JC_RETCODE jddex_GetClippedRect(const JC_RECT * pstRect, 
			 JC_RECT * pstClippedRect)
{
	JDDEX_DEVICE_CHAR stDeviceChar ;
	JC_INT32 uiExcessTopHeight, uiExcessBottomHeight ;
	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);

   

	uiExcessBottomHeight = (pstRect->iTop + pstRect->uiHeight) - (stDeviceChar.uiTop + 
															stDeviceChar.uiRenderingHeight);
	uiExcessTopHeight =  stDeviceChar.uiTop - pstRect->iTop;
	
	pstClippedRect->iLeft = 0;
	pstClippedRect->uiWidth = pstRect->uiWidth;

	// Full rectangle to be drawn
	if( uiExcessBottomHeight <= 0 && uiExcessTopHeight <= 0)
	{
		pstClippedRect->iTop = 0 ;
		pstClippedRect->uiHeight = pstRect->uiHeight;
	}
	// both top and bottom are cut
	else if ( uiExcessBottomHeight > 0 && uiExcessTopHeight > 0)
	{
		pstClippedRect->iTop = uiExcessTopHeight;
		pstClippedRect->uiHeight = pstRect->uiHeight - uiExcessTopHeight- uiExcessBottomHeight;
	}
	//cut at bottom
	else if( uiExcessBottomHeight > 0 )
	{
		pstClippedRect->iTop = 0 ;
		pstClippedRect->uiHeight = pstRect->uiHeight - uiExcessBottomHeight;
	}	
	// cut on top
	else if( uiExcessTopHeight > 0)
	{
		pstClippedRect->iTop = uiExcessTopHeight;
		pstClippedRect->uiHeight = pstRect->uiHeight - uiExcessTopHeight;
 	}

	return JC_OK;

}

/**
 * @brief 		Draw the rectangle which is in visible area with border properties
 * @param[in]	pvDecodedBuff - decode buffer
 * @param[in]	pstBorderProp  - image co-ordiantes
 * @param[in]	eImageProperty  - image properties
 * @retval		none
 * 
 */
JC_RETCODE jddex_DrawFormattedImage (JC_OBJECT_INFO		m_stObjectInfo,
											JC_RECT * pstRect, 
									 		EImageProperty eImageProperty,
									 		JC_CHAR* m_pmTempImageFile,
									 		JC_UINT32* puiAnimHnd)
{	
	JC_RECT stDisplayRect;
	JDDEX_DEVICE_CHAR stDeviceChar ;
	JDDEX_FORMATTED_RECT_PROPS stFormattedRectProp;
	JC_RECT  stFormattedRect ;
	JC_RETCODE rCode;

	stDisplayRect.iTop = pstRect->iTop;
	stDisplayRect.iLeft = pstRect->iLeft;	
	stDisplayRect.uiHeight = pstRect->uiHeight;
	stDisplayRect.uiWidth = pstRect->uiWidth;

	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);
	
	if(jddex_GetFormattedRect(pstRect, &stFormattedRectProp) == E_FALSE)
	   return JC_OK;

	
	stFormattedRect.iLeft		= stFormattedRectProp.stPoint1.uiX;
	stFormattedRect.iTop		= stFormattedRectProp.stPoint1.uiY;
	stFormattedRect.uiWidth		= stFormattedRectProp.stPoint2.uiX - stFormattedRectProp.stPoint1.uiX;
	stFormattedRect.uiHeight	= stFormattedRectProp.stPoint3.uiY - stFormattedRectProp.stPoint1.uiY;
#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if((JC_INT32)( pstRect->iTop + pstRect->uiHeight) < 0   )
		return JC_OK;
#else
	if((JC_INT32)( pstRect->iTop + pstRect->uiHeight) < stDeviceChar.uiTop  )
		return JC_OK
#endif


#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( pstRect->iTop >= (JC_INT32) (stDeviceChar.uiActualHeight))
		return JC_OK;
#else
	if( pstRect->iTop >= (JC_INT32) (stDeviceChar.uiTop + stDeviceChar.uiRenderingHeight))
		return JC_OK;
#endif
	
	rCode = jddex_PlatformDrawImage (m_stObjectInfo, &stDisplayRect, &stFormattedRect, 
							eImageProperty, m_pmTempImageFile, puiAnimHnd);
								
	return rCode;
}

JC_RETCODE jddex_PlatformDrawVideo(JC_OBJECT_INFO		m_stObjectInfo,JC_RECT * pstDispRect, 
									JC_RECT * pstClippedRect, JC_BOOLEAN bIsVideoHandleCreated);

JC_RETCODE jddex_DrawFormattedVideo(JC_OBJECT_INFO		m_stObjectInfo,
											JC_RECT * pstRect, JC_BOOLEAN bIsVideoHandleCreated)
{
									 	
	JC_RECT stDisplayRect;
	JDDEX_DEVICE_CHAR stDeviceChar ;
	JDDEX_FORMATTED_RECT_PROPS stFormattedRectProp;
	JC_RECT  stFormattedRect ;
	JC_RETCODE rCode;

	stDisplayRect.iTop = pstRect->iTop;
	stDisplayRect.iLeft = pstRect->iLeft;	
	stDisplayRect.uiHeight = pstRect->uiHeight;
	stDisplayRect.uiWidth = pstRect->uiWidth;

	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);
	
	if(jddex_GetFormattedRect(pstRect, &stFormattedRectProp) == E_FALSE)
	   return JC_OK;

	
	stFormattedRect.iLeft		= stFormattedRectProp.stPoint1.uiX;
	stFormattedRect.iTop		= stFormattedRectProp.stPoint1.uiY;
	stFormattedRect.uiWidth		= stFormattedRectProp.stPoint2.uiX - stFormattedRectProp.stPoint1.uiX;
	stFormattedRect.uiHeight	= stFormattedRectProp.stPoint3.uiY - stFormattedRectProp.stPoint1.uiY;
#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if((JC_INT32)( pstRect->iTop + pstRect->uiHeight) < 0   )
		return JC_OK;
#else
	if((JC_INT32)( pstRect->iTop + pstRect->uiHeight) < stDeviceChar.uiTop  )
		return JC_OK
#endif


#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( pstRect->iTop >= (JC_INT32) (stDeviceChar.uiActualHeight))
		return JC_OK;
#else
	if( pstRect->iTop >= (JC_INT32) (stDeviceChar.uiTop + stDeviceChar.uiRenderingHeight))
		return JC_OK;
#endif
	
	rCode = jddex_PlatformDrawVideo(m_stObjectInfo, &stDisplayRect, &stFormattedRect, bIsVideoHandleCreated );
								
	return rCode;
}



JC_RETCODE jddex_DrawImage(JC_OBJECT_INFO		m_stObjectInfo,
											JC_RECT * pstRect, 
									 		EImageProperty eImageProperty,
									 		JC_CHAR* m_pmTempImageFile,
									 		JC_UINT32* puiAnimHnd)
{	
	JC_RECT stDisplayRect;
	JDDEX_DEVICE_CHAR stDeviceChar ;
	JDDEX_FORMATTED_RECT_PROPS stFormattedRectProp;
	JC_RECT  stFormattedRect ;
	JC_RETCODE rCode;

	stDisplayRect.iTop = pstRect->iTop;
	stDisplayRect.iLeft = pstRect->iLeft;	
	stDisplayRect.uiHeight = pstRect->uiHeight;
	stDisplayRect.uiWidth = pstRect->uiWidth;

	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);
	
	if(jddex_GetFormattedRect(pstRect, &stFormattedRectProp) == E_FALSE)
	   return JC_OK;

	
	stFormattedRect.iLeft		= stFormattedRectProp.stPoint1.uiX;
	stFormattedRect.iTop		= stFormattedRectProp.stPoint1.uiY;
	stFormattedRect.uiWidth		= stFormattedRectProp.stPoint2.uiX - stFormattedRectProp.stPoint1.uiX;
	stFormattedRect.uiHeight	= stFormattedRectProp.stPoint3.uiY - stFormattedRectProp.stPoint1.uiY;
#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if((JC_INT32)( pstRect->iTop + pstRect->uiHeight) < 0   )
		return JC_OK;
#else
	if((JC_INT32)( pstRect->iTop + pstRect->uiHeight) < stDeviceChar.uiTop  )
		return JC_OK
#endif


#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( pstRect->iTop >= (JC_INT32) (stDeviceChar.uiActualHeight))
		return JC_OK;
#else
	if( pstRect->iTop >= (JC_INT32) (stDeviceChar.uiTop + stDeviceChar.uiRenderingHeight))
		return JC_OK;
#endif
	
	rCode = jddex_PlatformDrawImage (m_stObjectInfo, &stDisplayRect, &stFormattedRect, 
							eImageProperty, m_pmTempImageFile, puiAnimHnd);
								
	return rCode;
}

void jddex_FillChar(JC_CHAR *pszText, JC_CHAR ch , JC_UINT32 uiCnt)
{
	JC_UINT32 uiIndex ;
	for ( uiIndex = 0 ; uiIndex < uiCnt; uiIndex++)
	{
		pszText[uiIndex] = ch;
	}	
}

JC_RETCODE jddex_DrawFormattedAltImageIcon(JC_RECT* pstRect, JC_RECT* pParentRect )
{
	JDDEX_DEVICE_CHAR stDeviceChar ;
    //JC_UINT32 uiWidth = 0, uiHeight = 0 ;
	JC_UINT8* uiAltimgptr = JC_NULL;

	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);


	if(pParentRect->uiWidth < pstRect->uiWidth || 
		pParentRect->uiHeight < pstRect->uiHeight )
	{

	    jddex_DrawFormattedRect(pParentRect,JDDEX_DEFAULT_BLACK, JDDEX_DEFAULT_WHITE, 
							JDDEX_DEFAULT_INPUT_BORDER_WIDTH);
		return JC_OK;
	}

#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( pstRect->iTop  < 0 )
	{
		return JC_OK;
	}
#else
	if( pstRect->iTop  < (JC_INT32) (stDeviceChar.uiTop) )
	{
		return JC_OK;
	}
#endif

#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( pstRect->iTop + pstRect->uiHeight  > 
		(JC_INT32) stDeviceChar.uiActualHeight)
	{
		return JC_OK;
	}
#else
	if( pstRect->iTop + pstRect->uiHeight  > 
		(JC_INT32) (stDeviceChar.uiTop + stDeviceChar.uiRenderingHeight))
	{
		return JC_OK;
	}
#endif

	if( pstRect->iLeft < (JC_INT32) (stDeviceChar.uiLeft) )
		return JC_OK;

	if( pstRect->iLeft + pstRect->uiWidth  > (JC_INT32) (stDeviceChar.uiLeft + stDeviceChar.uiRenderingWidth))
		return JC_OK;

	// currently using some exiting icons . replace with new icon
	uiAltimgptr = (U8*)GetImage(IMG_ID_JATAAYU_ALTERNATE_IMAGE_GIF);
    gdi_image_draw(pstRect->iLeft, pstRect->iTop, uiAltimgptr);

	return (JC_OK) ;
}

void jddex_GetAltImgIConDimensions(JC_UINT32 *puiWidth, JC_UINT32 *puiHeight)
{
	*puiWidth = 14;
	*puiHeight = 14; 
}

JC_RETCODE jddex_DrawFormattedListBoxIcon(JC_RECT* pstRect)
{
	JDDEX_DEVICE_CHAR stDeviceChar ;
	JC_UINT8* uiListBoximgptr = JC_NULL;	

	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);

#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( pstRect->iTop  < 0 )
	{
		return JC_OK;
	}
#else
	if( pstRect->iTop  < (JC_INT32)stDeviceChar.uiTop )
	{
		return JC_OK;
	}
#endif

#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( pstRect->iTop + pstRect->uiHeight  > (JC_INT32) stDeviceChar.uiActualHeight)
	{
		return JC_OK;
	}
#else
	if( pstRect->iTop + pstRect->uiHeight  > (JC_INT32) (stDeviceChar.uiTop + stDeviceChar.uiRenderingHeight))
	{
		return JC_OK;
	}
#endif
	uiListBoximgptr = (U8*)GetImage(IMG_ID_JATAAYU_SMALL_ARROW_DOWN_GIF);
    gdi_image_draw_resized(pstRect->iLeft, pstRect->iTop, \
			JDDEX_LISTBOX_ICON_HT_WT, JDDEX_LISTBOX_ICON_HT_WT, uiListBoximgptr);
}

JC_RETCODE jddex_DrawFormattedRadioIcon(JC_RECT* pstRect,U16 ImageId)
{
	JDDEX_DEVICE_CHAR stDeviceChar ;
	JC_UINT8* uiRadioButtonimgptr = JC_NULL;	

	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &stDeviceChar);

#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( pstRect->iTop  < 0 )
	{
		return JC_OK;
	}
#else
	if( pstRect->iTop  < (JC_INT32)stDeviceChar.uiTop )
	{
		return JC_OK;
	}
#endif

#ifdef JDDEX_INVALIDATERECT_AVAIALABLE
	if( pstRect->iTop + pstRect->uiHeight  > (JC_INT32) stDeviceChar.uiActualHeight)
	{
		return JC_OK;
	}
#else
	if( pstRect->iTop + pstRect->uiHeight  > (JC_INT32) (stDeviceChar.uiTop + stDeviceChar.uiRenderingHeight))
	{
		return JC_OK;
	}
#endif
	uiRadioButtonimgptr = (U8*)GetImage(ImageId);
    gdi_image_draw_resized(pstRect->iLeft, pstRect->iTop, \
			JDDEX_RADIO_ICON_HT_WT, JDDEX_RADIO_ICON_HT_WT, uiRadioButtonimgptr);
}


void jddex_GetListBoxIConDimensions(JC_UINT32 *puiWidth, JC_UINT32 *puiHeight)
{
	*puiWidth = JDDEX_LISTBOX_ICON_HT_WT;
	*puiHeight= JDDEX_LISTBOX_ICON_HT_WT; // coolsand to use an icon preferably with size <=14
}

void jddex_ListBoxLSKHandler()
{	
#ifdef JATAAYU_SUPPORT_WAP
	JC_UINT32 selected_index = 0;	  
	JDDEX_LIST_ITEM *pstListItems;
	U8 temp_index;

	jddex_window * poWindow = (jddex_window *)jwap_GetPrimaryWindow();
	if (poWindow && poWindow->IsValidWidget((jddex_widget*)g_uiControlId))
	{
	
		pstListItems = *g_pstListBox.pstListItems;	
		selected_index = GetCurrHiliteID();
		
	
		for ( temp_index = 0; temp_index < g_pstListBox.uiItemCnt ; temp_index++)
		{
			pstListItems[temp_index].IsSelected = E_FALSE;
		}
		pstListItems[selected_index].IsSelected = E_TRUE;	
	}
	else
	{
	 mmi_trace(1,(PCSTR)"invalid list box");
	}
		
	jddex_LSKDoneHandler();
#endif	
}

void jddex_ComboBoxLSKHandler()
{    
#ifdef	JATAAYU_SUPPORT_WAP

	U8 selected_index = 0;    
	//ST_BE_CONTROL_INFO stCtrlInfo;
	jddex_combobox *poComboBox ;
	jddex_window * poWindow = (jddex_window *)jwap_GetPrimaryWindow();
	

	selected_index = (U8)GetCurrHiliteID();
	//mmi_trace(1, (PCSTR)"selction index %d", selected_index);
	//jdi_BEGetFocusedControlInfo ( jWap_Get_Brw_Handle(), &stCtrlInfo) ;
	if (poWindow && poWindow->IsValidWidget((jddex_widget*)g_uiControlId))
	{
		//poComboBox = (jddex_combobox *) stCtrlInfo.uiCtrlID;
		poComboBox = (jddex_combobox *) g_uiControlId;
		poComboBox->SetSelectionStatus(selected_index);
	}
	//mmi_trace(1, (PCSTR)"combo box controlid %d", poComboBox);
	jddex_LSKDoneHandler(); 
#endif	
}

void Donegotoprimarymmi()
{	
#ifdef	JATAAYU_SUPPORT_WAP

	void* vHandle;
	JC_RETCODE retCode = JC_OK;
	jddex_window * poWindow = (jddex_window *)jwap_GetPrimaryWindow();

	g_uiMaxlen = 0 ;
	
	/*if (g_pszInputOrigText)
	{
		jdd_MemFree (g_pszInputOrigText) ;
		g_pszInputOrigText = 0 ;
	}*/
	//mmi_trace(1,(PCSTR)"jdd_log: After Done Widget Control ID %d",g_uiControlId);
	if (poWindow && poWindow->IsValidWidget((jddex_widget*)g_uiControlId))
	{
	mmi_trace(1,(PCSTR)"jdd_log Donegotoprimarymmi  1 ");
		if(g_pszInputText)
		{
		mmi_trace(1,(PCSTR)"jdd_log Donegotoprimarymmi  2 ");
	  		jc_wstrcpy(g_pszInputOrigText,g_pszInputText );
		}
	}
	else
	{
		mmi_trace(1,(PCSTR)"jdd_log Invalid Widget");
	}
	if(g_pszInputText)
	{
	mmi_trace(1,(PCSTR)"jdd_log Donegotoprimarymmi  3 ");
		jdd_MemFree (g_pszInputText) ;
		g_pszInputText = JC_NULL;
	}
	jmms_goto_primaryMMI();
	vHandle = jWap_Get_Brw_Handle();	
	if(vHandle)
	{
	mmi_trace(1,(PCSTR)"jdd_log Donegotoprimarymmi  4");
		retCode = jdi_BERefreshDisplay (vHandle);
		jdi_BESetEvent(vHandle, E_BE_ACTION, JC_NULL);
	}
#endif	
}


void jddex_MMIBrwSetInfo(JC_UINT32  uiWindowID, SCROLL_INFO * pstVScrollInfo)
{
	jddex_window *poWindow = (jddex_window *) uiWindowID;
	jddex_widget * poWidget = JC_NULL;
	jddex_scrollbar * poScrollBar = JC_NULL ;
	poWindow->GetScrollBar(&poWidget);
	poScrollBar = (jddex_scrollbar *)poWidget;
	if(poScrollBar)
	{
		jdd_MMISetScrollInfo (uiWindowID, (JC_UINT32)poScrollBar, pstVScrollInfo);
	}
}

void mmi_field_save(void)
{

#ifdef	JATAAYU_SUPPORT_WAP

	int temp_index;
	JDDEX_LIST_ITEM *pstListItems;
	jddex_window * poWindow = (jddex_window *)jwap_GetPrimaryWindow();
   
	if (poWindow && poWindow->IsValidWidget((jddex_widget*)g_uiControlId))
	{
		
		pstListItems = *g_pstListBox.pstListItems;	

		for ( temp_index = 0; temp_index < g_pstListBox.uiItemCnt ; temp_index++)
		{						
			pstListItems[temp_index].IsSelected = g_field_state[temp_index] ;						
		}	
		if(g_field_state)
		{
			jdd_MemFree(g_field_state);
		}

		if(g_TempListItems)
		{
			for (temp_index = 0; temp_index< g_pstListBox.uiItemCnt; temp_index++)
			{
				jdd_MemFree(g_TempListItems[temp_index]) ;
			
			}	
			jdd_MemFree(g_TempListItems);
			g_TempListItems = JC_NULL;
			
		}
	}
	//Donegotoprimarymmi();
	jddex_LSKDoneHandler();
#endif	
}

#ifdef	JATAAYU_SUPPORT_WAP

void jddex_LSKDoneHandler()
{
	void* vHandle;
	JC_RETCODE retCode = JC_OK;
	jmms_goto_primaryMMI();
	vHandle = jWap_Get_Brw_Handle();	
	if(vHandle)
	{
		retCode = jdi_BERefreshDisplay (vHandle);
		jdi_BESetEvent(vHandle, E_BE_ACTION, JC_NULL);
	}
}
#endif

