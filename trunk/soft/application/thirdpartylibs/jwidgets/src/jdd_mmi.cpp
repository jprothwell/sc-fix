#define MODULE_NAME						"jdd"
#define FILE_NAME						"jdd_mmi.c"
/***************************************************************************
 * $Id: jdd_mmi.c,v 1.97 2008/09/04 06:15:26 shajudevhtmlbrow Exp $
 * $Revision: 1.97 $
 * $Date: 2008/09/04 06:15:26 $
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
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT,
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION
 * ANY LOSS OF DATA, OR ; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdd_mmi.c,v $
 * Revision 1.97  2008/09/04 06:15:26  shajudevhtmlbrow
 * Changes for JDD without resource dependency.
 *
 * Revision 1.96  2008/06/25 07:27:36  kavithadevhtmlbrow
 * E_OBJECT_INSTANTIATE is added in JC_OBJECT_ACTION
 *
 * Revision 1.95  2008/06/20 13:41:15  sriramdevhtmlbrow
 * Added new API jdd_MMIIsPictograph for checking if the given pictograph character can be handled by JDD or not.
 *
 * Revision 1.94  2008/02/21 16:13:07  kumardevhtmlbrow
 * Klocwork updated.
 *
 * Revision 1.93  2008/02/06 07:51:53  shajudevhtmlbrow
 * New switch is added for hiding default cursor
 *
 * Revision 1.92  2008/01/23 10:39:05  shajudevhtmlbrow
 * Cursor hiding is done under VS2005 switch
 *
 * Revision 1.91  2008/01/21 13:44:17  shajudevhtmlbrow
 * Hiding default cursor when core creates a cursor.
 *
 * Revision 1.90  2008/01/07 12:34:31  shajudevhtmlbrow
 * Thumbnail cursor JDD implementation.
 *
 * Revision 1.89  2007/12/28 12:37:58  kumardevhtmlbrow
 * doxygen changes.
 *
 * Revision 1.88  2007/12/14 07:46:11  shajudevhtmlbrow
 * Setting the focus to its parent when a control losing its focus.
 *
 * Revision 1.87  2007/12/11 09:59:06  shajudevhtmlbrow
 * Memory leak avoided in cursor implementation
 *
 * Revision 1.86  2007/12/11 07:53:25  shajudevhtmlbrow
 * Window region is set for the cursor.
 *
 * Revision 1.85  2007/12/10 14:06:22  kavithadevhtmlbrow
 * Cursor data type added
 *
 * Revision 1.84  2007/11/22 06:32:22  kavithadevhtmlbrow
 * jdd_MMIUnlinkControl added
 *
 * Revision 1.83  2007/11/16 12:59:23  kavithadevhtmlbrow
 * E_POLY control type
 *
 * Revision 1.82  2007/11/16 11:04:51  kavithadevhtmlbrow
 * uiDescent in JC_TEXTMETRICS
 *
 * Revision 1.81  2007/11/12 07:59:29  kavithadevhtmlbrow
 * Font height for Input type
 *
 * Revision 1.80  2007/10/09 08:34:37  shreyasdevhtmlbrow
 * Restricting the max size of a text in get text metrics
 *
 *
 ***************************************************************************/



/**
 * @file	 jdd_mmi.cpp
 * @ingroup	 jdd files
 * @brief	 Implementation of mmi module
 *
 * This file implements jdd MMI api. It initializes and deinitializes the mmi
 * module and it defines various mmi apis like create mmi window, mmi control.
 */
 
#include <jddex_platformdefines.h>
#include "ddl.h"
#include "jcal.h"
#include <jddex_window.h>
#include <jddex_int.h>
#include <jddex_platform.h>
#include <jddex_mmieventhandler.h>
#include <jddex_int.h>
#include <jddex_audio.h>
#include <jddex_wnddefaults.h>
#include <jddex_listbox.h>
#include <jddex_wnddefaults.h>
#include "cs_types.h"
#include "mmi_trace.h"
#include "ts.h"

#if 0
JC_INT8 g_ControlType [][30] =
{
	"E_TEXT_BOX", /**< A control that renders the text in the MMI. */
	"E_INPUT_BOX", /**< A control that accepts input from the user. */
	"E_BUTTON", /**< A control which allows the user to turn either on or off by selecting it. This control is called as push button or command button. */
	"E_BUTTON_MULTILINE", /**< A control similar to the push/command button but can span over multiple lines depending upon the length of the text */
	"E_CHECK_BOX", /**< A control in the MMI to enable or disable one or more options from a set.  */
	"E_RADIO_BOX", /**< A control in the MMI to enable or disable one option from a set. The enabling of one radio box will disable the already enabled radio box in the given set. */
	"E_LIST_BOX", /**< A control that allows the user to choose more than one options from the list. */
	"E_COMBO_BOX", /**< A control that allows the user to choose one options from the list. */
	"E_SCROLLBAR", /**< A control that visually indicates that there is more data that is yet to be seen outside the visible area */
	"E_IMAGE", /**< A control that allows creating an image. */
	"E_VIDEO", /**< A control that specifies a video type */
	"E_PICTOGRAM", /**< A control that allows creating a pictogram. */
	"E_IMAGE_TEXT", /**< A control that allows creating an image with text. */
	"E_ICON", /**< A control that allows creating an icon. */
	"E_RECT", /**< A control that allows creating a rect. */
	"E_POLY", /**< A control that allows creating a polygon. */
	"E_OBJECT", /**< A control that is common for image", audio", video and any other unknown content type. */
	"E_OBJECT_TEXT",  /**< A control that allows creating an object alternate with text */
	"E_CURSOR", /**<A control that allows to create a cursor. */
	"E_THUMBNAIL_CURSOR" /**<A control that allows to create a thumbnail cursor. */
} ;
#endif

#ifdef __cplusplus
    extern   "C"
    {
#endif
	extern JC_BOOLEAN jMMS_GetMMSAppStatus();
#ifdef __cplusplus
    }
#endif

extern "C" void __cxa_pure_virtual()
{
	//mmi_trace (1, (PCSTR)"####### __cxa_pure_virtual  invoked ########") ;
	//Crashes....
	ASSERT (0) ;
}

/**
 * @brief		Initialize the mmi module.
 * @param[in]	pvMmiReference	handle to instance of the application.
 * @return		Appropriate error code.
 *
 * It initializes the mmi module by registering the window class for mmi
 * application window.
 */

JC_RETCODE jdd_MMIInitialize (void * pvMmiReference)
{
    JC_RETCODE rCode = JC_OK ;
	return rCode ;
}


/**
 * @brief		Deinitialize the mmi module.
 * @return		Approtriate error code.
 *
 * It deinitializes the mmi module by unregistering the window class for mmi
 * application window. It returns JC_OK for success and necessary error code
 * for failure.
 */

JC_RETCODE jdd_MMIDeInitialize (void *pvMmiReference)
{
	JC_RETCODE rCode = JC_OK ;

	return rCode ;
}


/**
 * @brief		Creates mmi application window.
 * @param[in]   pWinProp	Window properties.
 * @param[in]	pRect		Rectangle dimensions to draw the window.
 * @param[out]	puiWindowID Window information.
 * @return		Appropriate error code.
 *
 * This fuction contains the implementation of function which creates mmi
 * application window according to the properties we give.
 */

JC_RETCODE jdd_MMICreateWindow (JC_WINDOW_PROPERTIES *pWinProp,
								const JC_RECT *pRect,
								JC_UINT32 *puiWindowID)
{
	JC_RETCODE		 rCode = JC_OK ;



	if (E_MAIN_WINDOW == pWinProp->eWindowStyle || E_CHILD_WINDOW == pWinProp->eWindowStyle)
	{
		rCode = jddex_MMICreateWindow (pWinProp, pRect,puiWindowID);


	}
	else if (E_POPUP_WINDOW == pWinProp->eWindowStyle)
	{
        rCode = JC_ERR_NOT_IMPLEMENTED;
	}
	else if (E_APPLICATION_WINDOW == pWinProp->eWindowStyle)
	{

		jddex_MMICreateWindow (pWinProp, pRect,puiWindowID);
	}

	return rCode ;
}


/**
 * @brief		Creates mmi control.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]	eControlType		type of the control
 * @param[in]	pControlProperty	Control property.
 * @param[in]	pRect				Rectangle dimensions to draw the control
 * @param[out]	puiControlID		Control Info.
 * @return		Appropriate error code.
 *
 * This fuction contains the implementation of function which creates mmi
 * control according to the properties and type we give.
 */

JC_RETCODE jdd_MMICreateControl (JC_UINT32 uiWindowID, EControlType eControlType,
								 JC_CONTROL_PROPERTIES *pControlProperty,
								 JC_RECT *pRect, JC_UINT32 *puiControlID)
{
	JC_RETCODE rCode = JC_OK ;

	*puiControlID = 0 ;

	mmi_trace(0, (PCSTR)"MMICreateControl controltype[%d]", eControlType);
	switch (eControlType)
	{
		case E_TEXT_BOX:
			{
				rCode = jddex_MMICreateLabel((jddex_window * )uiWindowID,eControlType,pControlProperty,
								pRect,puiControlID);
			}
			break ;

		case E_INPUT_BOX:
			{
				rCode = jddex_MMICreateInputBox((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);
			}
			break ;

		case E_BUTTON:
			{
				rCode = jddex_MMICreateButton((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_CHECK_BOX:
			{
				rCode = jddex_MMICreateCheckBox((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_RADIO_BOX:
			{
				rCode = jddex_MMICreateRadioButton((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_LIST_BOX:
			{
				rCode = jddex_MMICreateListBox((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_COMBO_BOX:
			{
				rCode = jddex_MMICreateComboBox((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_SCROLLBAR:
			{
				rCode = jddex_MMICreateScrollBar((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_IMAGE:
			{
				rCode = jddex_MMICreateImage((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_OBJECT:
			{
				rCode = jddex_MMICreateObject((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_PICTOGRAM:
			{
				rCode = jddex_MMICreatePictogram((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_IMAGE_TEXT :
			{
				rCode = jddex_MMICreateObjectText((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);
			}
			break ;

		case E_OBJECT_TEXT :
			{
				rCode = jddex_MMICreateObjectText((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_ICON :
			{
				rCode = jddex_MMICreateIcon((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_RECT :
			{
				rCode = jddex_MMICreateInputBox((jddex_window * )uiWindowID,eControlType,pControlProperty,
					pRect,puiControlID);

			}
			break ;

		case E_CURSOR :
			rCode = JC_ERR_INVALID_PARAMETER ;
			break ;

		case E_THUMBNAIL_CURSOR :
			rCode = JC_ERR_INVALID_PARAMETER ;
			break ;

		case E_POLY :
			rCode = JC_ERR_INVALID_PARAMETER ;
			break ;

		default:
			rCode = JC_ERR_INVALID_PARAMETER ;
			break ;
	}


	return rCode ;
}


/**
 * @brief		Set the Window title.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]	pmTitleValue		Title value.
 * @return		Appropriate error code.
 *
 * This function sets title for a window.
 */

JC_RETCODE jdd_MMISetTitle (JC_UINT32 uiWindowID,
							const JC_CHAR *pmTitleValue)
{
	JC_RETCODE rCode = JC_OK ;

	//wingtech to set the title for the window here
	return rCode ;
}


/**
 * @brief		Set focus to a Window.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]	bIsFocus		Focus flag.
 * @return		Appropriate error code.
 *
 * If the focus flag is true then the focus set to the specified window.
 */

JC_RETCODE jdd_MMISetFocusWindow (JC_UINT32 uiWindowID,
								   JC_BOOLEAN bIsFocus)
{

	jddex_widget * pstWidget = (jddex_widget *) uiWindowID;

    if (JC_NULL != pstWidget)
	{
		if (bIsFocus)
	{
		pstWidget->SetFocus();
	}
	else
	{
		pstWidget->ResetFocus();
	}
	}


	return JC_OK ;
}


/**
 * @brief		Get the focused control
 * @param[in]   uiWindowID		Window Info.
 * @param[out]	uiControlID		Control Info.
 * @return		Appropriate error code.
 *
 * It return the current focused contfrol info.
 */

JC_RETCODE jdd_MMIGetFocusedControl (JC_UINT32 uiWindowID,
									 JC_UINT32 *puiControlID)
{
    JC_RETCODE rCode = JC_OK ;
	jddex_window * pstWindow = (jddex_window *) uiWindowID;
	jddex_widget * pstWidget = (jddex_widget *) puiControlID;

    if (JC_NULL != pstWindow)
    {
	pstWindow->GetCurrFocusWidget(&pstWidget);
    }

	return rCode ;
}


/**
 * @brief		Set focus to a Control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]	uiControlID		Control Info.
 * @param[in]	bIsFocus		Focus flag.
 * @return		Appropriate error code.
 *
 * This function sets the focus to the specified control identifier on the
 * specified window.If the focus flag is true then the focus set to the
 * specified window.
 */

JC_RETCODE jdd_MMISetFocusedControl (JC_UINT32 uiWindowID,
									 JC_UINT32 uiControlID, JC_BOOLEAN bIsFocus)
{

	jddex_window * pstWindow = (jddex_window *) uiWindowID;
	jddex_widget * pstWidget = (jddex_widget *) uiControlID;


    if (JC_NULL != pstWindow && JC_NULL != pstWidget)
	{
		if (bIsFocus)
	{
		pstWindow->SetCurrFocusWidget(pstWidget);
		pstWidget->SetFocus();
	}
	else
	{
		pstWindow->SetCurrFocusWidget(JC_NULL);
		pstWidget->ResetFocus();
	}
	}


	return JC_OK ;
}


/**
 * @brief		Change the visibilty of Window.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]	bIsVisible		Visibility flag.
 * @return		Appropriate error code.
 *
 * This function sets the specified window's display state according to
 * the visibilty flag.
 */

JC_RETCODE jdd_MMIShowWindow (JC_UINT32 uiWindowID, JC_BOOLEAN bIsVisible)
{
	JC_RETCODE rCode = JC_OK ;

	jddex_window * pstWindow = (jddex_window *) uiWindowID;

    if (JC_NULL != pstWindow)
    {
	pstWindow->SetVisibilty(bIsVisible);
    }

	return rCode ;
}


/**
 * @brief		Move or resize a window.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]	pRect			Window rect.
 * @return		Appropriate error code.
 *
 * This function move or resize the specified window as per the new rectangle.
 */

JC_RETCODE jdd_MMIMoveWindow (JC_UINT32 uiWindowID,	const JC_RECT *pRect)
{

	jddex_window *pstWindow = (jddex_window *)uiWindowID;

    if (JC_NULL != pstWindow)
    {
	pstWindow->SetRect((JC_RECT *)pRect);
    }

	return JC_OK ;
}


/**
 * @brief		Destroys a window.
 * @param[in]   uiWindowID		Window Info.
 * @return		Appropriate error code.
 *
 * This function destroys the specified window and frees the resources allocated
 * for the window.
 */

JC_RETCODE jdd_MMIDestroyWindow (JC_UINT32 uiWindowID)
{
	JC_RETCODE rCode = JC_OK ;
	jddex_window *pstWindow = (jddex_window *)uiWindowID;
	if (JC_NULL != pstWindow)
	{
		delete pstWindow;
	}

	return rCode ;
}


/**
 * @brief		Get the colors of a window.
 * @param[in]   uiWindowID			Window Info.
 * @param[out]  piBackgroundColor	background color.
 * @param[out]  piForegroundColor	forground color.
 * @return		Appropriate error code.
 *
 * This function gets the background and foreground color for the window.
 */

JC_RETCODE jdd_MMIGetColor (JC_UINT32 uiWindowID, JC_INT32 *piBackgroundColor,
							JC_INT32 *piForegroundColor)
{

	JC_RETCODE rCode = JC_OK ;

	jddex_widget * pstWidget = (jddex_widget *) uiWindowID;

    if (JC_NULL != pstWidget)
    {
	pstWidget->GetColors(piBackgroundColor, piForegroundColor);
    }

	return rCode ;
}


/**
 * @brief		Sets the colors for a window.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]	piBackgroundColor	background color.
 * @param[in]   piForegroundColor	forground color.
 * @return		Appropriate error code.
 *
 * This function sets the background and foreground color for the window..
 */

JC_RETCODE jdd_MMISetColor (JC_UINT32 uiWindowID,JC_INT32 iBackgroundColor,
							JC_INT32 iForegroundColor)
{
	//JC_RETCODE rCode = JC_OK ;
	jddex_widget * pstWidget = (jddex_widget *) uiWindowID;

    if (JC_NULL != pstWidget)
    {
	pstWidget->SetColors(iBackgroundColor, iForegroundColor);
    }

	return JC_OK ;
}


/**
 * @brief		Sets default color for a window.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]	piBackgroundColor	background color flag.
 * @param[in]   piForegroundColor	forground color flag.
 * @return		Appropriate error code.
 *
 * This function sets the default color for the window.  Here it is white as a
 * background and black as a foreground
 */

JC_RETCODE jdd_MMISetDefaultColor (JC_UINT32 uiWindowID,
								   JC_BOOLEAN bIsBackgroundColor,
								   JC_BOOLEAN bIsForegroundColor)
{
	JC_RETCODE rCode = JC_OK ;
	jddex_widget * pstWidget = (jddex_widget *) uiWindowID;

    if (JC_NULL != pstWidget)
    {
	if(bIsBackgroundColor)
		pstWidget->SetDefaultBgColor();
	if(bIsForegroundColor)
		pstWidget->SetDefaultFgColor();
    }

	return rCode ;
}


/**
 * @brief		Get the rectangle dimension of a window.
 * @param[in]   uiWindowID	Window Info.
 * @param[in]   pRect		Window rect.
 * @return		Appropriate error code.
 *
 * This function gets the current position, width and height of the specified
 * window.
 */

JC_RETCODE jdd_MMIGetWindowRect (JC_UINT32 uiWindowID,
									JC_RECT *pRect)
{
	JC_RETCODE rCode = JC_OK ;
	jddex_widget * pstWidget = (jddex_widget *) uiWindowID;

	if (JC_NULL != pstWidget)
	{
		pstWidget->GetRect(pRect);
    }
	mmi_trace(1,(PCSTR)"widget_log: get window rect w[%d],h[%d]",pRect->uiWidth,pRect->uiHeight);
	return rCode ;
}


/**
 * @brief		Get the rectangle dimension of a control.
 * @param[in]   uiWindowID	Window Info.
 * @param[in]   uiControlID	Control Info.
 * @param[in]   pRect		Window rect.
 * @return		Appropriate error code.
 *
 * This function gets the current position, width and height of the specified
 * control.
 */

JC_RETCODE jdd_MMIGetControlRect (JC_UINT32 uiWindowID,
								  JC_UINT32 uiControlID,
								  JC_RECT *pRect)
{
	JC_RETCODE rCode = JC_OK ;
	jddex_widget * pstWidget = (jddex_widget *) uiControlID;

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

    if (JC_NULL != pstWidget)
    {
	pstWidget->GetRect(pRect);
    }
	
	return rCode ;
}


/**
 * @brief		Set the rectangle dimensions of a control.
 * @param[in]   uiWindowID	Window Info.
 * @param[in]   uiControlID	Control Info.
 * @param[in]   pRect		Window rect.
 * @return		Appropriate error code.
 *
 * This function sets the new position, width and height for the specified control.
 */

JC_RETCODE jdd_MMISetControlRect (JC_UINT32 uiWindowID,
								  JC_UINT32 uiControlID,
								  const JC_RECT *pRect)
{
	JC_RETCODE rCode = JC_OK ;
	jddex_widget * pstWidget = (jddex_widget *) uiControlID;

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
    if (JC_NULL != pstWidget)
    {
	pstWidget->SetRect((JC_RECT *)pRect);
    }

	return rCode ;
}


/**
 * @brief		Change the visibilty of Control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Control Info.
 * @param[in]	bIsVisible		Visibility flag.
 * @return		Appropriate error code.
 *
 * This function sets the specified controls's display state according to
 * the visibilty flag.
 */

JC_RETCODE jdd_MMIShowControl (JC_UINT32 uiWindowID,
								JC_UINT32 uiControlID,
								JC_BOOLEAN bIsVisible)
{
	jddex_widget * pstWidget = (jddex_widget *) uiControlID;
	
    if (JC_NULL != pstWidget)
    {
	pstWidget->SetVisibilty(bIsVisible);
    }
	return JC_OK ;
}


/**
 * @brief		Move or resize a control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Control Info.
 * @param[in]	pRect			Control rect.
 * @return		Appropriate error code.
 *
 * This function move or resize the specified control as per the new rectangle.
 */

JC_RETCODE jdd_MMIMoveControl (JC_UINT32 uiWindowID,
								JC_UINT32 uiControlID,
								const JC_RECT *pRect)
{
	JC_RETCODE		rCode= JC_OK ;
	jddex_widget *pstWidget = (jddex_window *)uiControlID;

    if (JC_NULL != pstWidget)
    {
	pstWidget->SetRect((JC_RECT *)pRect);
    }
	return rCode ;
}


/**
 * @brief		Set the text in the text control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Control Info.
 * @param[in]	pmTextValue		Text value.
 * @return		Appropriate error code.
 *
 * This function sets the caption (label) value to the text box control.
 */

JC_RETCODE jdd_MMISetTextValue (JC_UINT32 uiWindowID, JC_UINT32 uiControlID,
								JC_CHAR *pmTextValue)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	jddex_widget *pstLabel = (jddex_widget *)uiControlID;
if (JC_NULL != pstLabel)
    {	
pstLabel->SetText(pmTextValue);
}
	return JC_OK;
}


/**
 * @brief		Get text value of the text control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Control Info.
 * @param[out]	pmTextValue		Text value.
 * @return		Appropriate error code.
 *
 * This function gets the caption (label) value of the text box control.
 */

JC_RETCODE jdd_MMIGetTextValue (JC_UINT32 uiWindowID,
								JC_UINT32 uiControlID,
								JC_CHAR **pmTextValue)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	jddex_widget *pstLabel = (jddex_widget *)uiControlID;

    if (JC_NULL != pstLabel)
    {
	pstLabel->GetText(pmTextValue);
    }

	return JC_OK ;
}


/**
 * @brief		Get the input cursor position in the input box.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Control Info.
 * @param[out]	piTextLen		Position.
 * @return		Appropriate error code.
 *
 * This function gets the input cursor position in the input box.
 */

JC_RETCODE jdd_MMIGetInputCurPosition (JC_UINT32 uiWindowID,
									   JC_UINT32 uiControlID,
									   JC_INT32 *piTextLen)
{

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;



	return JC_OK ;
}


/**
 * @brief		Set the input cursor position in the input box.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Control Info.
 * @param[in]	iTextLen		Position.
 * @return		Appropriate error code.
 *
 * This function sets the input cursor position in the input box.
 */

JC_RETCODE jdd_MMISetInputCurPosition (JC_UINT32 uiWindowID,
									   JC_UINT32 uiControlID,
									   JC_INT32 iTextLen)
{



	return JC_OK ;
}


/**
 * @brief		Set the value to the input box.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Control Info.
 * @param[in]	pmInputValue	Text value.
 * @return		Appropriate error code.
 *
 * This function sets the text value to the input box.
 */

JC_RETCODE jdd_MMISetInputValue (JC_UINT32 uiWindowID,
								 JC_UINT32 uiControlID,
								 const JC_CHAR *pmInputValue)

{

	jddex_inputbox * pst_InputBox = (jddex_inputbox *) uiControlID;

    if (JC_NULL != pst_InputBox)
    {
	pst_InputBox->SetInputText((JC_CHAR *)pmInputValue);
    }

	return JC_OK ;
}


/**
 * @brief		Get the value from the input box.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Control Info.
 * @param[out]	pmInputValue	Text value.
 * @return		Appropriate error code.
 *
 * This function gets the text value from the input box.
 */

JC_RETCODE jdd_MMIGetInputValue (JC_UINT32 uiWindowID,
								JC_UINT32 uiControlID,
								JC_CHAR **ppmInputValue)
{
	JC_RETCODE rCode = JC_OK ;	

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	jddex_inputbox * pst_InputBox = (jddex_inputbox *) uiControlID;

       if (JC_NULL != pst_InputBox && JC_NULL != ppmInputValue)
	   {
	   	 pst_InputBox->GetInputText(ppmInputValue);
       }
	return rCode ;
}


/**
 * @brief		Frees memory allocated for input value. .
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Control Info.
 * @param[in]	pmInputValue	Input value.
 * @return		Appropriate error code.
 *
 * This Function frees the input value that has been allocated in the
 * jdd_MMIGetInputValue function.
 */


void jdd_MMIFreeInputValue (JC_UINT32 uiWindowID,
							JC_UINT32 uiControlID,
							JC_CHAR *pmInputValue)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	PARAM_INTENTIONALLY_NOT_USED (uiControlID) ;

	jdd_AssertOnFail ((JC_NULL != pmInputValue),
		              "jdd_MMIFreeInputValue : Invalid parameter") ;

	jdd_MemFree (pmInputValue);

	return ;
}


/**
 * @brief		Get the checkbox status.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiControlID			Control Info.
 * @param[out]	pCheckBoxStatus		check status.
 * @return		Appropriate error code.
 *
 * This function gets the status of checkbox. It checks whether checkbox
 * is checked/unckecked.
 */

JC_RETCODE jdd_MMIGetCheckBoxStatus (JC_UINT32 uiWindowID,JC_UINT32 uiControlID,
									   E_OPTIONBOX_STATUS *pCheckBoxStatus)
{

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

	jddex_checkbox * pstCheckBox = (jddex_checkbox *) uiControlID;

    if (JC_NULL != pstCheckBox)
    {
	pstCheckBox->GetStatus(pCheckBoxStatus);
    }
	return JC_OK ;
}


/**
 * @brief		Change the checkbox status.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiControlID			Control Info.
 * @param[out]	pCheckBoxStatus		check status.
 * @return		Appropriate error code.
 *
 * This function change the status of checkbox. It will ckeck or uncheck
 * the checkbox ;
 */

JC_RETCODE jdd_MMISetCheckBoxStatus (JC_UINT32 uiWindowID, JC_UINT32 uiControlID,
									   E_OPTIONBOX_STATUS eCheckBoxStatus)
{

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	jddex_checkbox * pstCheckBox = (jddex_checkbox *) uiControlID;

    if (JC_NULL != pstCheckBox)
    {
	pstCheckBox->SetStatus(eCheckBoxStatus);
    }

	return JC_OK ;
}


/**
 * @brief		Change the radiobox status.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiControlID			Control Info.
 * @param[out]	pCheckBoxStatus		radiobox status.
 * @return		Appropriate error code.
 *
 * This function change the status of radiobox. It will set or unset
 * the radiobox ;
 */

JC_RETCODE jdd_MMISetRadioBoxStatus (JC_UINT32 uiWindowID, JC_UINT32 uiRadioBoxID,
										E_OPTIONBOX_STATUS eRadioBoxStatus)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	jddex_radiobutton * pstRadioButton = (jddex_radiobutton *) uiRadioBoxID;

    if (JC_NULL != pstRadioButton)
    {
	pstRadioButton->SetStatus(eRadioBoxStatus);
    }
	return JC_OK ;
}


/**
 * @brief		Get the radiobox status.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiControlID			Control Info.
 * @param[out]	pCheckBoxStatus		radiobox status.
 * @return		Appropriate error code.
 *
 * This function gets the status of radiobox. It checks whether radiobox
 * is set/unset.
 */

JC_RETCODE jdd_MMIGetRadioBoxStatus (JC_UINT32 uiWindowID,JC_UINT32 uiRadioBoxID,
									   E_OPTIONBOX_STATUS *pRadioBoxStatus)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	jddex_radiobutton * pstRadioButton = (jddex_radiobutton *) uiRadioBoxID;

    if (JC_NULL != pstRadioButton)
    {
	pstRadioButton->GetStatus(pRadioBoxStatus);
    }
	return JC_OK ;
}


/**
 * @brief		Get the colors of a control.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiControlID			Control Info.
 * @param[out]  	background color.
 * @param[out]  piForegroundColor	forground color.
 * @return		Appropriate error code.
 *
 * This function gets the background and foreground color for the control.
 */

JC_RETCODE jdd_MMIGetControlColor (JC_UINT32 uiWindowID, JC_UINT32 uiControlID,
									JC_INT32 *piBackgroundColor,
									JC_INT32 *piForegroundColor)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

	jddex_widget * pstWidget = (jddex_widget *) uiControlID;

    if (JC_NULL != pstWidget)
    {
	pstWidget->GetColors(piBackgroundColor,piForegroundColor);
    }

	return JC_OK ;
}



/**
 * @brief		Sets the colors for a control.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiControlID			Control Info.
 * @param[in]	piBackgroundColor	background color.
 * @param[in]   piForegroundColor	forground color.
 * @return		Appropriate error code.
 *
 * This function sets the background and foreground color for the control..
 */

JC_RETCODE jdd_MMISetControlColor (JC_UINT32 uiWindowID,JC_UINT32 uiControlID,
								   JC_INT32 iBackgroundColor,
								   JC_INT32 iForegroundColor)
{
	JC_RETCODE rCode = JC_OK ;

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

	jddex_widget * pstWidget = (jddex_widget *) uiControlID;

    if (JC_NULL != pstWidget)
    {
	pstWidget->SetColors(iBackgroundColor, iForegroundColor);
    }

	return rCode ;
}


/**
 * @brief		Sets default color for a control.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]	bIsBackgroundColor	background color flag.
 * @param[in]   bIsForegroundColor	forground color flag.
 * @return		Appropriate error code.
 *
 * This function sets the default color for the controls. Here it is white for
 * background and black for foreground color.
 */

JC_RETCODE jdd_MMISetDefaultControlColor (JC_UINT32 uiWindowID,
										  JC_UINT32 uiControlID,
										  JC_BOOLEAN bIsBackgroundColor,
										  JC_BOOLEAN bIsForegroundColor)
{
	JC_RETCODE rCode = JC_OK ;

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

	jddex_widget * pstWidget = (jddex_widget *) uiControlID;

    if (JC_NULL != pstWidget)
    {
	if(bIsBackgroundColor)
		pstWidget->SetDefaultBgColor();

	if(bIsForegroundColor)
		pstWidget->SetDefaultFgColor();
    }

	return rCode ;
}


/**
 * @brief		Deletes the control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]	uiControlID		Control Info.
 * @return		Appropriate error code.
 *
 * This function destroys the specified control and frees any memory the
 * control is occupied.
 */

JC_RETCODE jdd_MMIDeleteControl (JC_UINT32 uiWindowID,JC_UINT32 uiControlID)
{
	JC_RETCODE rCode = JC_OK ;

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

	jddex_window * pstWindow = (jddex_window *) uiWindowID;

	jddex_widget * pstWidget = (jddex_widget *) uiControlID;

    if (JC_NULL != pstWindow)
    {
	pstWindow->DeleteWidget(pstWidget);
    }

	return rCode ;
}


/**
 * @brief This function unlinks the control from the window and not frees/deletes the controls
 * @param[in] uiWindowID Specifies the identifier for the window
 * @param[in] uiControlID Specifies the control identifier
 */
JC_RETCODE jdd_MMIUnlinkControl (JC_UINT32	uiWindowID, JC_UINT32 uiControlID)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	PARAM_INTENTIONALLY_NOT_USED (uiControlID) ;

	return JC_OK ;
}


/**
 * @brief		Set the text item in the listbox control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiListBoxID		Listbox Info.
 * @param[in]	uiIndexItem		Item Index.
 * @param[in]	pmText			Text item.
 * @param[in]	pImageInfo		Image info.
 * @return		Appropriate error code.
 *
 * This function sets the text for the individual items of the list box.
 */

JC_RETCODE jdd_MMIListBoxSetTextItem (JC_UINT32 uiWindowID,
										JC_UINT32 uiListBoxID,
										JC_UINT32 uiIndexItem,
										const JC_CHAR *pmText,
										const JC_IMAGE_INFO *pImageInfo)
{
	JC_RETCODE rCode = JC_OK ;

	jddex_listbox * poListBox = (jddex_listbox *) uiListBoxID ;

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

    if (JC_NULL != poListBox)
    {
		poListBox->SetListboxItem(uiIndexItem, (JC_CHAR *)pmText, (JC_IMAGE_INFO *)pImageInfo);
    }
	return rCode ;
}


/**
 * @brief		Create the option group ID for ListBox.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Listbox Info.
 * @param[in]	pstListItem		pointer to List item information structure.
 * @param[out]	puiOptgroupID	Hold option group ID.
 * @return		Appropriate error code.
 *
 * This function creates option group ID for ListBox. This ID should be
 * unique among the list box.
 */

JC_RETCODE jdd_MMIListBoxCreateOptgroup (JC_UINT32 uiWindowID,
										 JC_UINT32 uiControlID,
										 const JC_LIST_BOX_ITEM	*pstListItem,
										 JC_UINT32 *puiOptgroupID)
{
	JC_RETCODE rCode= JC_OK ;



	return rCode ;
}


/**
 * @brief		Set the text item to the listbox control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiListBoxID		Listbox Info.
 * @param[in]	pstListItem		pointer to List item information structure.
 * @return		Appropriate error code.
 *
 * This function sets the text for the individual items of the list box.
 * The item can be option group or option item.
 */

JC_RETCODE jdd_MMIListBoxSetTextItemOpt (JC_UINT32 uiWindowID,
										 JC_UINT32 uiListBoxID,
										 const JC_LIST_BOX_ITEM *pstListItem)
{


	return JC_OK ;
}


/**
 * @brief		Gets the  selected item(s) in the listbox control.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiListBoxID			Listbox Info.
 * @param[out]	ppiSelectedIndex	Selected item index(es).
 * @param[out]	puiSelectedCounts	Number of selected items.
 * @return		Appropriate error code.
 *
 * This function gets the selected item index(es) in the listbox control.
 */

JC_RETCODE jdd_MMIListBoxGetItem (JC_UINT32 uiWindowID,
								  JC_UINT32 uiListBoxID,
								  JC_INT32 **ppiSelectedIndex,
								  JC_UINT32 *puiSelectedCounts)
{
	jddex_listbox * poListBox = (jddex_listbox *) uiListBoxID ;

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

    if (JC_NULL != poListBox)
    {
	poListBox->GetSelectionStatus(ppiSelectedIndex,puiSelectedCounts);
    }

	return JC_OK ;
}


/**
 * @brief		It set the items selected in the listbox control.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiListBoxID			Listbox Info.
 * @param[in]	puiSelectedIndex	Item index(es).
 * @param[in]	uiSelectedCounts	Item Count to be selected.
 * @return		Appropriate error code.
 *
 * This function sets the item(s) from the list selected. If uiSelectedCounts
 * is 0 than unselect all the text of the list box else set selected depending
 * on the index value provided by the puiSelectedIndex
 */

JC_RETCODE jdd_MMIListBoxSetItem (JC_UINT32 uiWindowID,
								  JC_UINT32 uiListBoxID,
								  JC_INT32 *puiSelectedIndex,
								  JC_UINT32 uiSelectedCounts)
{
	jddex_listbox * poListBox = (jddex_listbox *) uiListBoxID ;

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

    if (JC_NULL != poListBox)
    {
	poListBox->SetSelectionStatus(puiSelectedIndex,uiSelectedCounts);
    }

	return JC_OK ;
}


/**
 * @brief		Gets the focused item in the listbox control.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiListBoxID			Listbox Info.
 * @param[out]	piFocusedIndex		Focused item.
 * @return		Appropriate error code.
 *
 * This function gets the focus item index in the list box.
 */

JC_RETCODE jdd_MMIListBoxGetFocusedItem (JC_UINT32 uiWindowID,
										 JC_UINT32 uiListBoxID,
										 JC_INT32 *piFocusedIndex)
{
	JC_RETCODE rCode = JC_OK ;

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	return rCode ;
}


/**
 * @brief		Free memory allocated for listbox item.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiListBoxID			Listbox Info.
 * @param[out]	puiSelectedIndex	item.
 * @return		Appropriate error code.
 *
 * This function freeing the allocated memory by the jdd_MMIListBoxGetItem
 * function.
 */

void jdd_MMIListBoxFreeItem (JC_UINT32 uiWindowID, JC_UINT32 uiListBoxID,
							 JC_INT32 *puiSelectedIndex)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	PARAM_INTENTIONALLY_NOT_USED (uiListBoxID) ;

	jdd_MemFree(puiSelectedIndex);
	return ;
}


/**
 * @brief		Set the text item in the combo box control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiComboBoxID	Combobox Info.
 * @param[in]	uiIndexItem		Item Index.
 * @param[in]	pmText			Text item.
 * @return		Appropriate error code.
 *
 * This function sets the text for the individual items of combobox control.
 */

JC_RETCODE jdd_MMIComboBoxSetTextItem (JC_UINT32 uiWindowID,
									   JC_UINT32 uiComboBoxID,
									   JC_UINT32 uiIndexItem,
									   const JC_CHAR *pmText)
{
	JC_RETCODE rCode = JC_OK ;

	jddex_combobox * poComboBox = (jddex_combobox *)  uiComboBoxID;

    if (JC_NULL != poComboBox)
    {
	poComboBox->SetComboboxItem(uiIndexItem,(JC_CHAR *)pmText);
    }

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;


	return rCode ;

}


/**
 * @brief		Create the option group ID for ComboBox.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Listbox Info.
 * @param[in]	pstComboItem	pointer to Combo item information structure.
 * @param[out]	puiOptgroupID	Hold option group ID.
 * @return		Appropriate error code.
 *
 * This function creates option group ID for ComboBox. This ID should be
 * unique among the Combo box.
 */

JC_RETCODE jdd_MMIComboBoxCreateOptgroup (JC_UINT32 uiWindowID,
										  JC_UINT32 uiControlID,
										  JC_COMBO_BOX_ITEM	*pstComboItem,
										  JC_UINT32	*puiOptgroupID)
{
	JC_RETCODE rCode= JC_OK ;


	return rCode ;
}


/**
 * @brief		Set the text item to the Combobox control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiListBoxID		Combobox Info.
 * @param[in]	pstComboItem	pointer to Combo item information structure.
 * @return		Appropriate error code.
 *
 * This function sets the text for the individual items of the Combobox.
 * The item can be option group or option item.
 */

JC_RETCODE jdd_MMIComboBoxSetTextItemOpt (JC_UINT32 uiWindowID,
										  JC_UINT32 uiControlID,
										  const JC_COMBO_BOX_ITEM *pstComboItem)
{
	JC_RETCODE rCode = JC_OK ;




	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;


	return rCode ;
}


/**
 * @brief		Sets an item selected in combo box control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiComboBoxID	Combobox Info.
 * @param[in]	uiSelectedIndex	Item Index.
 * @return		Appropriate error code.
 *
 * This function sets the specified item selected from combobox list.
 */

JC_RETCODE jdd_MMIComboBoxSetItem (JC_UINT32 uiWindowID,
									JC_UINT32 uiComboBoxID,
									JC_INT32 uiSelectedIndex)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

	jddex_combobox * poComboBox = (jddex_combobox *)  uiComboBoxID;

    if (JC_NULL != poComboBox)
    {
	poComboBox->SetSelectionStatus(uiSelectedIndex);
    }

	return JC_OK ;
}


/**
 * @brief		Get the selected item index of combo box control.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiComboBoxID		Combobox Info.
 * @param[in]	puiSelectedIndex	selected item Index.
 * @return		Appropriate error code.
 *
 * This function gets the index of the item that is selected for the combo box.
 */

JC_RETCODE jdd_MMIComboBoxGetItem (JC_UINT32 uiWindowID,
									JC_UINT32 uiComboBoxID,
									JC_INT32 *puiSelectedIndex)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	jddex_combobox * poComboBox = (jddex_combobox *)  uiComboBoxID;

    if (JC_NULL != poComboBox)
    {
	poComboBox->GetSelectionStatus(puiSelectedIndex);
    }

	return JC_OK ;
}


/**
 * @brief		Draw rectangle on the specified window.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   pRect				Dimension for the rectangle.
 * @param[in]	pBorderProps		Border properties.
 * @param[in]	iBackgroundColor	Background color.
 * @param[out]	puiRectID			Rectangle Info.
 * @return		Appropriate error code.
 *
 * This function draws the rectangle on the specified window.
 */

JC_RETCODE jdd_MMIDrawRect (JC_UINT32 uiWindowID, JC_RECT *pRect,
							JC_BORDER_PROPS *pBorderProps,
							JC_INT32 iBackgroundColor,
							JC_UINT32 *puiRectID)
{

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	mmi_trace(1,(PCSTR)"widget_log: jdd_MMIDrawRect");
	jddex_DrawFormattedRectEx(pRect, pBorderProps, iBackgroundColor);

	return JC_OK ;
}


/**
 * @brief		Disable or enable a Control.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiControlID		Control Info.
 * @param[in]	bIsEnable		Enable flag.
 * @return		Appropriate error code.
 *
 * This function sets the control's enable(ENABLE/DISABLE) state.
 */


JC_RETCODE jdd_MMIEnableControl (JC_UINT32 uiWindowID,
								 JC_UINT32 uiControlID,
								 JC_BOOLEAN bIsEnable)
{
	jddex_widget *pstWidget = (jddex_window *)uiControlID;

    if (JC_NULL != pstWidget)
    {
	if(bIsEnable)
		pstWidget->Disable(E_FALSE);
	else
		pstWidget->Disable(E_TRUE);
    }

	return JC_OK ;
}


/**
 * @brief		Set a wallpaper.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   imageInfo		Image Info.
 * @param[in]	eProperty		Wallpaper propert.
 * @param[out]	pRect           Wallapaper Rect.
 * @return		Appropriate error code.
 *
 * This function set a wallpaper to a window.
 */

JC_RETCODE jdd_MMISetWallpaper (JC_UINT32 uiWindowID, JC_OBJECT_INFO imageInfo,
								EWallpaperProperty eProperty, JC_RECT *pRect)
{
	JC_BOOLEAN IsSucess;

	jddex_window *pstWindow = (jddex_window *)uiWindowID;
	pstWindow->SetWallPaper(&imageInfo, pRect,(EImageProperty) eProperty, &IsSucess);
	if( E_FALSE == IsSucess)
	{
		return  JC_ERR_GENERAL_FAILURE;
	}
	else
	{
		return JC_OK;
	}
	
}


/**
 * @brief		Clear the wallpaper.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   uiWallpaperID	Not used.
 * @return		Appropriate error code.
 *
 * This function clears a wallpaper from a window.
 */

JC_RETCODE jdd_MMIClearWallpaper (JC_UINT32 uiWindowID, JC_UINT32 uiWallpaperID)
{
	JC_RETCODE rCode = JC_OK ;
	PARAM_INTENTIONALLY_NOT_USED (uiWallpaperID) ;

	jddex_window *pstWindow = (jddex_window *)uiWindowID;
    if (JC_NULL != pstWindow)
    {
		pstWindow->ClearWallPaper();
    }

	return rCode ;
}


/**
 * @brief		Get the palette relative colour.
 * @param[in]   ucRedColor		Red color intensity.
 * @param[in]   ucGreenColor	Green color intensity.
 * @param[in]   ucBlueColor		Blue color intensity.
 * @return		Appropriate error code.
 *
 * This function returns the palette relative color specified through red,
 * green and blue.
 */

JC_UINT32 jdd_MMIGetRGBPalette (JC_UINT8 ucRedColor, JC_UINT8 ucGreenColor,
								JC_UINT8 ucBlueColor)
{
	JC_UINT32 uiRGB = 0;
	uiRGB = jddex_PlatformGetRGBPalette (ucRedColor,ucGreenColor, ucBlueColor);
	return uiRGB;
}


/**
 * @brief This function shoudl return the foreground color that can be visible
 *        with the background color passed
 * @param[in] uiBackgroundColor Specifies the background color
 * @param[in] uiForegroundColor Specifies the foreground color
 */
JC_UINT32 jdd_MMIGetForegroundColor (JC_UINT32		uiBackgroundColor,
									 JC_UINT32		uiForegroundColor)
{
#if 0
JC_UINT8		ucRed, ucGreen, ucBlue ;
	JC_BOOLEAN		bWhite ;

	bWhite = E_TRUE ;
	ucRed = GetRValue (uiBackgroundColor) ;
	ucGreen = GetGValue (uiBackgroundColor) ;
	ucBlue = GetBValue (uiBackgroundColor) ;
	if (ucRed > 150)
	{
		if ((ucGreen > 150) || (ucBlue > 150))
		{
			bWhite = E_FALSE ;
		}
	}
	else if ((ucGreen > 150) && (ucBlue > 150))
	{
		bWhite = E_FALSE ;
	}
	if (E_TRUE == bWhite)
	{

		uiForegroundColor = jddex_PlatformGetRGBPalette (255, 255, 255) ;

	}
	else
	{
		uiForegroundColor = jddex_PlatformGetRGBPalette (0, 0, 0) ;

	}

	return uiForegroundColor ;
#endif
	return 0;
}


/**
 * @brief		Get the size of the image.
 * @param[in]   uiWindowID		Window id
 * @param[in]   imageInfo		Image Info.
 * @param[out]  puiWidth		Width of the image.
 * @param[out]  puiHeight		Height of the image.
 * @return		Appropriate error code.
 *
 * This function returns the width and height of the image corresponding to
 * the image info.
 */

JC_RETCODE jdd_MMIGetImageSize (JC_UINT32			uiWindowID,
								JC_IMAGE_INFO		*imageInfo,
								JC_UINT32			*puiWidth,
								JC_UINT32			*puiHeight)
{
	JC_RETCODE rCode = JC_OK ;



	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;


	return rCode ;
}


/**
 * @brief		Gets the icon count
 * @return		Icon Count.
 *
 * This function returns the icon count what mmi supports.
 */

JC_UINT32 jdd_MMIGetIconCount ()
{
	return 0 ;
}


/**
 * @brief		Return the graphic supportability.
 * @return		True or False.
 *
 * This function returns whether graphics is supported or not in the current
 * platform.
 */

JC_BOOLEAN jdd_MMIIsGraphicsSupported (void)
{
	return E_TRUE;
}


/**
 * @brief		It begins the display of a window.
 * @param[in]   uiWindowID		Window Info.
 *
 * This function begins the display of a window. This function must be called
 * before any display should perform in a window.
 */

void jdd_MMIBeginDisplay (JC_UINT32 uiWindowID)
{
	mmi_trace(1,(PCSTR)"jdd_MMIBeginDisplay");
}



/**
 * @brief		It ends the display of a window.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]	bRedraw			Redraw flag
 *
 * This function ends the display of a window. This function should be called
 * whenever it finishes the display operations over that window.
 */

void jdd_MMIEndDisplay (JC_UINT32 uiWindowID, JC_BOOLEAN bRedraw)
{
	jddex_window * pst_Window = (jddex_window *) uiWindowID;
	JDDEX_DEVICE_CHAR stDeviceChar;
	JC_BOOLEAN		bIsSecondaryMMIUp ;
		
	jddex_GetDeviceProp((E_APP_TYPE) 0, &stDeviceChar);

	
	jddex_IsSecondaryMMIUp(&bIsSecondaryMMIUp);
	mmi_trace (1, (PCSTR)"widget_log: IsSecondaryMMIUp returned %d", bIsSecondaryMMIUp) ;
	//return;

	if (!bIsSecondaryMMIUp)
	{
		gui_lock_double_buffer();
		
		gui_reset_clip();
		gui_push_clip();	
		gui_set_clip(stDeviceChar.uiLeft, stDeviceChar.uiTop, 
			stDeviceChar.uiLeft+stDeviceChar.uiRenderingWidth , 
			stDeviceChar.uiTop+stDeviceChar.uiRenderingHeight);
		
		pst_Window->Show();
		// drawing the tiltle as the gui_set_clip is not clipping a pixel as expected below the ttile
		//draw_title();
		
		gui_pop_clip();		
		gui_unlock_double_buffer();
						
		gui_BLT_double_buffer(stDeviceChar.uiLeft, stDeviceChar.uiTop, 
			stDeviceChar.uiLeft+stDeviceChar.uiRenderingWidth , 
			stDeviceChar.uiTop+stDeviceChar.uiRenderingHeight);
#if 0		
		mmi_trace (1, (PCSTR)"$$$jdd_MMIEndDisplay Left[%d], Top[%d],\
				RenderingWidth[%d], uiRenderingHeight[%d] @@@",\
				stDeviceChar.uiLeft, stDeviceChar.uiTop,\
				stDeviceChar.uiRenderingWidth, stDeviceChar.uiRenderingHeight) ;
#endif		
	}
}


/**
 * @brief		It Get the textmetrics of a window.
 * @param[in]   uiWindowID		Window Info.
 * @param[in]   pStyle			pointer to font style information
 * @param[in]   psTextValue		pointer to text String.
 * @param[in]   uiWidth			width of the window.
 * @param[out]  pTextMetrics	pointer to text metric structure.
 * @returns     Appropriate error code. JC_OK if successful.
 *
 * This function gets the textmetrics of the device context of the specified
 * window, when we select a font with the style given.
 */


JC_RETCODE jdd_MMIGetTextMetrics (JC_UINT32 uiWindowID,
								  const JC_STYLE *pStyle,
								  const JC_CHAR *psTextValue,
								  JC_UINT32 uiWidth,
								  JC_TEXTMETRICS *pTextMetrics)
{
	JC_RETCODE		rCode = JC_OK ;
	jddex_PlatformGetTextMetrics (pStyle, psTextValue, uiWidth,pTextMetrics);
#if 0
	{
		JC_INT8  pcFName [100] = {0, } ;
			
		jdi_CUtilsStrTcsNCpy (psTextValue, 99, pcFName) ;
		mmi_trace (1, (PCSTR)"@@@ GetTxtMetrics Text: %s @@@", pcFName) ;
	}
	mmi_trace (1, (PCSTR)"@@@ GetTxtMetrics: uiWidth=%d, ret uiWidth=%d, no of chars=%d", \
		uiWidth, pTextMetrics->uiWidth, pTextMetrics->uiNumCharacters) ;
#endif
	return rCode ;
}

/**
 * @brief		This function provides the text metrics for the given text for the
 *				specified style and is also based upon the control type.
 * @param[in]   uiWindowID		Specifies the identifier for the window.
 * @param[in]   pstStyle		Pointer to font style information
 * @param[in]   pmTextValue		Pointer to text String.
 * @param[in]   uiWidth			Specifies the width of the window.
 * @param[in]	eControlType	Type of control
 * @param[out]  pstTextMetrics	Pointer to text metric structure.
 * @returns     Appropriate error code. JC_OK if successfull.
 *
 * This function gets the textmetrics of the device context of the specified
 * window, when we select a font with the given style. It also bases the textmetrics
 * calculation upon the specified style and the type of control.
 */
JC_RETCODE jdd_MMIGetTextMetricsEx (JC_UINT32 uiWindowID,
								    const JC_STYLE *pstStyle,
									const JC_CHAR *pmTextValue,
									JC_UINT32 uiWidth,
									EControlType eControlType,
									JC_TEXTMETRICS *pstTextMetrics)
{
	JC_RETCODE	rCode = JC_OK ;

	jddex_PlatformGetTextMetrics (pstStyle, pmTextValue, uiWidth,pstTextMetrics);
	mmi_trace(1,(PCSTR)"widget_log: jdd_MMIGetTextMetricsEx type [%d] w[%d] h[%d] ",eControlType,pstTextMetrics->uiWidth,pstTextMetrics->uiHeight);
	return rCode ;
}

/**
 * @brief This function gets the maximum text control width
 * @param[in] uiWindowID Specifies the window identifier
 * @retval  max width of the text control. zero for as per page instruction
 *
 * This function should return the maximum width of a text control that can be
 * created.  This value is used while creating the text control.  This width is
 * mapped against the jdd_MMIGetTextMetrics width
 */
JC_UINT32 jdd_MMIGetMaxTextWidth (JC_UINT32			uiWindowID)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

	return 0 ;
}


/**
 * @brief		Creates radiobox group ID.
 * @param[in]   uiWindowID			Window Info.
 * @param[out]  puiGroupRadioID		Radio group ID.
 * @return		Appropriate error code.
 *
 * This function returns a unique group ID for radio buttons. It is a linked list
 * starting address which holds all the radio buttun adresses coming under that
 * group.
 */

JC_RETCODE jdd_MMICreateRadioBoxGroupID (JC_UINT32 uiWindowID,
										JC_UINT32 *puiGroupRadioID)
{
	JC_RETCODE rCode = JC_OK ;

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	*puiGroupRadioID = (JC_UINT32)jdd_MemAlloc (sizeof(JC_UINT32), 1);

	return rCode ;
}


/**
 * @brief		Deletes radiobox group ID.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiGroupRadioID		Radio group ID.
 * @return		Appropriate error code.
 *
 * This function deletes the radio button group ID. It frees the linked list which
 * holds radio button addresses. If any radio button in that group is left undeleted
 * it sets the group ID of that control to 0.
 */

void jdd_MMIDeleteRadioBoxGroupID (JC_UINT32 uiWindowID, JC_UINT32 uiGroupRadioID)
{

	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
    jdd_MemFree((void*)uiGroupRadioID);

}


/**
 * @brief		Sets the scroll information to scroll bar control.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiControlID			Scrollbar ID.
 * @param[in]   pScrollInfo			pointer to Scroll Info.
 * @return		Appropriate error code.
 *
 * This function updates the position of scroll tab according to the
 * scroll information provided.
 */
 

JC_RETCODE jdd_MMISetScrollInfo (JC_UINT32 uiWindowID,
								JC_UINT32 uiControlID,
								const SCROLL_INFO *pScrollInfo)
{
    JC_RETCODE  rCode = JC_OK ;
	jddex_scrollbar *pst_ScrollBar = (jddex_scrollbar*)uiControlID;
	//mmi_trace(1, (PCSTR)"jdd_MMISetScrollInfo uiWindowID[%d] uiControlID[%d] ", uiWindowID,uiControlID);

	mmi_trace(1, (PCSTR)"scroll info max[%d] min[%d] pagesize[%d] uiCurrPos[%d]",  
			pScrollInfo->uiMinPos, pScrollInfo->uiMaxPos,pScrollInfo->uiPageSize,pScrollInfo->uiCurrPos);

	if(pst_ScrollBar)
		pst_ScrollBar->SetScrollInfo((SCROLL_INFO *)pScrollInfo);

	return rCode ;
}

/**
 * @brief		Gets the scroll car information from scroll bar control.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiControlID			Scrollbar ID.
 * @param[out]  pScrollInfo			pointer to Scroll Info.
 * @return		Appropriate error code.
 *
 * This function gets the current scroll car information.
 */

JC_RETCODE jdd_MMIGetScrollCarInfo (JC_UINT32 uiWindowID,
								JC_UINT32 uiControlID,
								JC_RECT *pScrollInfo)
{

	jddex_scrollbar *pst_ScrollBar = (jddex_scrollbar*)uiControlID;
	mmi_trace(1, (PCSTR)"jdd_MMIGetScrollCarInfo uiWindowID[%d] uiControlID[%d] ", uiWindowID,uiControlID);

	pst_ScrollBar->GetScrollCarInfo(pScrollInfo);
	return JC_OK ;
}

/**
 * @brief		Gets the scroll information from scroll bar control.
 * @param[in]   uiWindowID			Window Info.
 * @param[in]   uiControlID			Scrollbar ID.
 * @param[out]  pScrollInfo			pointer to Scroll Info.
 * @return		Appropriate error code.
 *
 * This function gets the current scroll information.
 */

JC_RETCODE jdd_MMIGetScrollInfo (JC_UINT32 uiWindowID,
								JC_UINT32 uiControlID,
								SCROLL_INFO *pScrollInfo)
{

	jddex_scrollbar *pst_ScrollBar = (jddex_scrollbar*)uiControlID;

	pst_ScrollBar->GetScrollInfo(pScrollInfo);
	return JC_OK ;
}


JC_RETCODE jdd_MMIGetIconSize (JC_UINT32 uiWindowID, EIconType eType,
							   JC_UINT32 *puiWidth, JC_UINT32 *puiHeight)
{
	JC_RETCODE rCode = JC_OK ;

	PARAM_INTENTIONALLY_NOT_USED (eType) ;

	*puiWidth = JDDEX_DEFAULT_ICON_WIDTH ;
	*puiHeight = JDDEX_DEFAULT_ICON_HEIGHT ;
	return rCode ;
}

/**
 * @brief This function provides the size of the given icon type based upon the fontsize.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] eType Specifies the icon type.
 * @param[in] pstStyle Specifies the style associated with the icon.
 * @param[out] puiWidth Provides the width that is required to render this icon image.
 * @param[out] puiHeight Provides the height that is required to render this icon image.
 */
JC_RETCODE jdd_MMIGetIconSizeEx (JC_UINT32 uiWindowID, EIconType eType,
							     const JC_STYLE *pstStyle,
							     JC_UINT32 *puiWidth, JC_UINT32 *puiHeight)
{
	JC_RETCODE			rCode = JC_OK;
	JC_TEXTMETRICS		stTextMetrics ;
	JC_CHAR				amStr [2] ;

	PARAM_INTENTIONALLY_NOT_USED (eType) ;
	amStr [0] = 'M' ;
	amStr [1] = '\0' ;

	rCode = jdd_MMIGetTextMetrics (uiWindowID, pstStyle, amStr, 0, &stTextMetrics) ;
	if ((JC_OK == rCode) && ((stTextMetrics.uiHeight / 2) > JDDEX_DEFAULT_ICON_HEIGHT))
	{
		*puiHeight = stTextMetrics.uiHeight / 2 ;
		*puiWidth = *puiHeight ;
	}
	else
	{
		*puiWidth =  JDDEX_DEFAULT_ICON_WIDTH ;
		*puiHeight = JDDEX_DEFAULT_ICON_HEIGHT ;

	}
	return  rCode ;
}

JC_RETCODE jdd_MMIGetObjectSize		(JC_UINT32		uiWindowID,
									 JC_OBJECT_INFO	*pstObjectInfo,
									 JC_HANDLE		vObjectHandle,
									 JC_UINT32		*puiWidth,
									 JC_UINT32		*puiHeight,
									 JC_UINT32		*puiObjectID)
{
	JC_RETCODE rCode = JC_OK ;

	rCode = jddex_PlatformGetObjectSize (pstObjectInfo, puiWidth, puiHeight);
	mmi_trace(0, (PCSTR)"jdd_log: MMIGetObjectSize wt[%d] ht[%d] rCode[%x]", *puiWidth, *puiHeight, -rCode);
	return rCode ;
}


void jdd_MMIFreeObjectSize (JC_UINT32			uiWindowID,
							JC_UINT32			uiObjectID)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	PARAM_INTENTIONALLY_NOT_USED (uiObjectID) ;

	return ;
}

/**
 * @brief This function performs the specified action on the given object.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] eAction Specifies the action type.
 */
JC_RETCODE jdd_MMIObjectAction (JC_UINT32			uiWindowID,
								JC_UINT32			uiControlID,
								E_OBJECT_ACTION		eAction)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	mmi_trace(1,(PCSTR)"jdd_MMIObjectAction called");

	jddex_widget*poWidget = (jddex_widget*)uiControlID;
	

	/*jddex_audio * poAudio = (jddex_audio *)uiControlID ;

    if (JC_NULL != poAudio)
    {
	poAudio->HandleAudioAction(eAction);
    }*/
	 if (JC_NULL != poWidget)
    {
		poWidget->HandleAction(eAction);
    }
	
	return JC_OK ;
}

JC_RETCODE jdd_MMINotificationMessage (JC_INT32 iRetCode,
									   EMMINotificationMsgType eType,
									   JDD_MMI_INFORMATION_CB inforamationCB,
									   void *pCBArg)
{
	PARAM_INTENTIONALLY_NOT_USED (iRetCode) ;
	PARAM_INTENTIONALLY_NOT_USED (eType) ;
	PARAM_INTENTIONALLY_NOT_USED (inforamationCB) ;
	PARAM_INTENTIONALLY_NOT_USED (pCBArg) ;

	return JC_ERR_NOT_IMPLEMENTED ;

}


JC_RETCODE jdd_MMIConfirmationMessage (JC_CHAR *psChar,
									   EMMIConfirmationMsgType eType,
									   JDD_MMI_CONFIRMATION_CB confirmationCB,
									   void *pCBArg)
{
	PARAM_INTENTIONALLY_NOT_USED (psChar) ;
	PARAM_INTENTIONALLY_NOT_USED (eType) ;
	PARAM_INTENTIONALLY_NOT_USED (confirmationCB) ;
	PARAM_INTENTIONALLY_NOT_USED (pCBArg) ;

	return JC_ERR_NOT_IMPLEMENTED ;
}



JC_RETCODE jdd_MMIGetFileStreamImageSize (JC_UINT32		uiWindowID,
										  JC_CHAR		*psFileName,
										  JC_UINT32		*puiWidth,
										  JC_UINT32		*puiHeight)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	PARAM_INTENTIONALLY_NOT_USED (psFileName) ;
	PARAM_INTENTIONALLY_NOT_USED (puiWidth) ;
	PARAM_INTENTIONALLY_NOT_USED (puiHeight) ;

	return JC_ERR_NOT_IMPLEMENTED ;
}


JC_RETCODE jdd_MMIGetNumberOfSoftKeys (JC_UINT32 uiWindowID,
										JC_UINT32 *puiNumSoftKeys)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	PARAM_INTENTIONALLY_NOT_USED (puiNumSoftKeys) ;

	return JC_ERR_NOT_IMPLEMENTED ;
}

JC_RETCODE jdd_MMISetSoftKeyLabel (JC_UINT32 uiWindowID,
									JC_UINT32 uiSoftKeyIndex,
									const JC_CHAR *psSoftKeyLabel)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	PARAM_INTENTIONALLY_NOT_USED (uiSoftKeyIndex) ;
	PARAM_INTENTIONALLY_NOT_USED (psSoftKeyLabel) ;

	return JC_ERR_NOT_IMPLEMENTED ;
}


JC_RETCODE jdd_MMIClearRect (JC_UINT32 uiWindowID, JC_RECT *pRect)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	
	jddex_PlatformEraseRect(pRect);

	return JC_OK ;
}


JC_RETCODE jdd_MMIGetDeviceCharacteristics (DeviceCharacteristics *pCharacteristics)
{
	JDDEX_DEVICE_CHAR pstDeviceChar ;
	PARAM_INTENTIONALLY_NOT_USED (pCharacteristics) ;


	jddex_GetDeviceProp(JDDEX_BROWSER_APP, &pstDeviceChar);

	pCharacteristics->uiActualWidth = pstDeviceChar.uiActualWidth;
	pCharacteristics->uiActualHeight = pstDeviceChar.uiActualHeight;
	pCharacteristics->uiRenderingWidth = pstDeviceChar.uiRenderingWidth ;
    pCharacteristics->uiRenderingHeight= pstDeviceChar.uiRenderingHeight;
	pCharacteristics->uiWindowX =  pstDeviceChar.uiLeft;
	pCharacteristics->uiWindowY =  pstDeviceChar.uiTop;
	pCharacteristics->uiScrollBarWidth = jwap_GetScrollbarWidth();
	pCharacteristics->uiScrollBarHeight = pstDeviceChar.uiRenderingHeight;

	return JC_OK ;
}


/**
 * @brief		Checks whether a font is available or not.
 * @param[in]   pcFontName		Reference to the fontname
 * @retval		E_TRUE if the font is present.
 */

JC_RETCODE jdd_MMIIsFontAvailable (JC_UINT32 uiWindowID, JC_INT8 *pcFontName,
								   JC_INT8 *pcLang, JC_INT32 *piFontID)
{
	JC_RETCODE		rCode = JC_OK;



	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	PARAM_INTENTIONALLY_NOT_USED (pcLang) ;


	return rCode ;
}


/**
 * @brief This function select or deselect the specific region in the specified control.
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] uiControlID Specifies the control identifier
 * @param[in] uiStrLen Specifies the length of the string that needs to be highlighted
 * @param[in] uiOffset Specifies the offset from the which the string needs to start highlighting.
 * @param[in] bIsHighlight Specifies whether this function is called for select or deselect.
 * @param[out] puiID Returns the unique identifier for selection.
 */

void jdd_MMIHighlightText (JC_UINT32		uiWindowID,
						   JC_UINT32		uiControlID,
						   JC_UINT32		uiStrLen,
						   JC_UINT32		uiOffset,
						   JC_BOOLEAN		bIsHighlight,
						   JC_UINT32		*puiID)
{


	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;

}


JC_RETCODE jdd_MMIObjectContentMore (JC_UINT32		uiWindowID,
									 JC_UINT32		uiControlID,
									 JC_OBJECT_MORE	*pstObjectMore)
{
	PARAM_INTENTIONALLY_NOT_USED (uiWindowID) ;
	PARAM_INTENTIONALLY_NOT_USED (uiControlID) ;
	PARAM_INTENTIONALLY_NOT_USED (pstObjectMore) ;

	return JC_OK ;
}


/**
 * @brief This function checks if the given classid value is supported or not
 * @param[in] uiWindowID		Specifies the Window identifier
 * @param[in,out] pstObjClass   Specifies the information about the object
 * @retval	JC_OK	The given classid value is supported
 * @retval	JC_ERR_NOT_SUPPORTED	The given classid is not supported
 */
JC_RETCODE jdd_MMIIsObjectSupported (JC_UINT32			uiWindowID,
									 JC_OBJECT_CLASS	*pstObjClass)
{
	return JC_OK ;

}


/**
 * @brief This function sets a different cursor type
 * @param[in]  uiWindowID	Window Info.
 * @param[in]  uiControlID	Cursor ID.
 * @param[in]  eType		cursor type.
 * @return	Appropriate error code.
 *
 * This function gets the current scroll information.
 */
JC_RETCODE jdd_MMISetCursorType (JC_UINT32		uiWindowID,
								 JC_UINT32		uiControlID,
								 ECursorType	eType)
{
	JC_RETCODE			rCode = JC_OK ;

	return rCode ;
}

/**
 * @brief This function checks if the given pictograph character can be displayed or not
 * @param[in]  uhPictCode	Pictographic character code
 * @return	E_TRUE	The given pictograph character can be displayed
 * @return	E_FALSE The given pictograph character can not be displayed
 */
JC_BOOLEAN jdd_MMIIsPictograph (JC_UINT16	uhPictCode)
{

	return  E_TRUE;
}

/*
 * @brief This function sets the wallpaper for the window at the location specified in pRect.
 * @param[in] uiWindowID Specifies the window identifier.
 * @param[in] pstWallPaperInfo Specifies the wallpaper info which holds mime type, object content,
 *							   size of the object, background property  , location where the object needs
 *						   	   to be rendered and magnification factor.
 */
JC_RETCODE jdd_MMISetWallpaperEx (JC_UINT32				uiWindowID,
								  JC_WALLPAPER_INFO		*pstWallPaperInfo)
{
	return (jdd_MMISetWallpaper (uiWindowID, pstWallPaperInfo->stObjectInfo, \
		pstWallPaperInfo->eProperty, pstWallPaperInfo->pstRect)) ;
}


// intorduced by application team . not part of original JDD
JC_RETCODE jdd_GetScrollInfoForWindow (JC_UINT32 uiWindowID, JC_RECT *pstScrollCarRect,
	JC_RECT *pstScrollBarRect)
{

	jddex_window *poWindow = (jddex_window*)uiWindowID;
	jddex_widget *poWidget = JC_NULL;
	jddex_scrollbar *poScrollBar ;
		
	poWindow->GetScrollBar(&poWidget);
	mmi_trace(1,(PCSTR)"scroll bar control id %d %d", (JC_UINT32)poWidget, uiWindowID);
	if(poWidget)
	{
		poScrollBar = (jddex_scrollbar *)poWidget;
		poScrollBar->GetScrollCarInfo(pstScrollCarRect);
		poScrollBar->GetScrollBarInfo(pstScrollBarRect);
	}
	return JC_OK ;
}
JC_UINT32 jdd_GetScrollOffsetFactor(JC_UINT32 uiWindowID)
{
	jddex_window *poWindow = (jddex_window*)uiWindowID;
	jddex_widget *poWidget = JC_NULL;
	jddex_scrollbar *poScrollBar ;
	//JC_RECT stScrollCarRect={0,}, stScrollBarRect={0,};
	JC_UINT32 uiFactor = 1;
	poWindow->GetScrollBar(&poWidget);
	if(poWidget)
	{
		poScrollBar = (jddex_scrollbar *)poWidget;
		uiFactor = poScrollBar->GetScrollOffsetFactor();
	}
}


/*
JC_RETCODE jdd_GetScrollOffset(JC_UINT32 uiWindowID, JC_UINT32 uiNewScrollCarPos)
{

	jddex_window *poWindow = (jddex_window*)uiWindowID;
	jddex_widget *poWidget = JC_NULL;
	jddex_scrollbar *poScrollBar ;
	JC_UINT32 uiScrollOffset = 0;
	JC_RECT stScrollCarRect={0,};
	JC_UINT32 uiActScrollCarPos =0, uiOldScrollCarCentre=0;
		
	poWindow->GetScrollBar(&poWidget);
	
	mmi_trace(1,(PCSTR)"scroll bar control id %d %d pos %d ", (JC_UINT32)poWidget, uiWindowID,uiNewScrollCarPos);
	if(poWidget)
	{
		poScrollBar = (jddex_scrollbar *)poWidget;
		poScrollBar->GetScrollCarInfo(&stScrollCarRect);
		
		
		uiOldScrollCarCentre = (stScrollCarRect.iTop + stScrollCarRect.uiHeight)/2 ;

				
		uiScrollOffset = poScrollBar->GetPageOffset(uiNewScrollCarPos);
	}
	return uiScrollOffset ;
}*/

