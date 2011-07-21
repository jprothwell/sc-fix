/***************************************************************************
 * $Id: jdd_mmiapi.h,v 1.12 2008/06/30 06:09:28 browserdevjtbase Exp $
 * $Revision: 1.12 $
 * $DateTime: $
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
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdd_mmiapi.h,v $
 * Revision 1.12  2008/06/30 06:09:28  browserdevjtbase
 * Core Update
 *
 * Revision 1.11  2008/05/30 05:23:53  browserdevjtbase
 * Update.
 *
 * Revision 1.10  2007/12/14 06:16:02  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.9  2007/11/29 05:37:25  browserdevjtbase
 * Added the MMI for the poly and control caching when doing the re-draw.
 *
 * Revision 1.8  2007/11/20 14:45:23  browserdevjtbase
 * Updated.
 *
 * Revision 1.7  2007/04/03 07:13:39  browserdevjtbase
 * JCF Update
 *
 * Revision 1.6  2007/01/12 11:18:05  browserdevjtbase
 * Update from the main base
 *
 * Revision 1.5  2006/11/29 13:09:24  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.4  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.3  2006/06/20 13:28:37  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.23  2006/03/02 06:50:13  kavithadevhtmlbrow
 * jdd_MMIHightlighText updated
 *
 * Revision 1.22  2006/03/02 06:22:50  kavithadevhtmlbrow
 * jdd_MMIGetObectSize and jdd_MMIFreeObjectSize
 *
 * Revision 1.21  2006/02/28 10:59:56  kavithadevhtmlbrow
 * New Parameter added to jdd_MMIGetObjectSize
 *
 *
 *  
 ***************************************************************************/


#ifndef _JDD_MMI_API_H
#define _JDD_MMI_API_H


#ifdef __cplusplus
extern "C"
{
#endif

/** 
 * @file jdd_mmiapi.h
 * @ingroup MMI
 * @{
 * @brief Describes the API's that are available in the MMI abstraction layer. 
 */

/** @cond */
#include <jdd_mimeapi.h>
/** @endcond */


/**
 * @brief This function shall be called to initialize the MMI layer for the specific platform. 
 * @param[in] pvMMIReference Specifies platform specific argument which is required to initialize the MMI layer. 
 *							 It can be HINSTANCE in case of Win32.
 * This shall be the first call by the application before invoking any other MMI related API calls. 
 * In general, the application startup code needs to call this initialize function with the required 
 * initialization parameter that is specific to the platform. 
 * @see jdd_MMIDeInitialize
 */
JC_RETCODE jdd_MMIInitialize (void *pvMMIReference) ;

/**
 * @brief This function shall de-initialize the MMI platform specific layer. 
 * @param[in] pvMMIReference Specifies the platform specific argument that has been passed when the #jdd_MMIInitialize function is executed.
 * This shall be the last call by the application in invoking any MMI related API calls. This function shall free any global resources that are used by the MMI platform specific layer.
 * @see jdd_MMIInitialize
 */
JC_RETCODE jdd_MMIDeInitialize (void *pvMMIReference) ;

/**
 * @brief This function should return whether the graphics is supported by the specific platform. 
 */
JC_BOOLEAN jdd_MMIIsGraphicsSupported (void) ;

/**
 * @brief This function provides the device characteristics for the platform.
 */
JC_RETCODE jdd_MMIGetDeviceCharacteristics (DeviceCharacteristics *pstCharacteristics) ;

/**
 * @brief This function creates a window for the given rectangle position.
 * @param[in] pstWinProp Specifies the window properties.
 * @param[in] pstRect Specifies the initial horizontal and vertical position of the window. It also contains the width and height of the window in device units. If it is NULL then use the default width and height of the device capabilities.
 * @param[out] puiWindowID Returns the identifier for the newly created window. 
 * @see jdd_MMIDestroyWindow
 */
JC_RETCODE jdd_MMICreateWindow (JC_WINDOW_PROPERTIES	*pstWinProp,
								const JC_RECT			*pstRect,
								JC_UINT32				*puiWindowID) ;
/**
 * @brief This function allows to set the caption for the given window. 
 * @param[in] uiWindowID Specifies the window identifier.
 * @param[in] pmTitleValue Specifies the title for the window.
 * The application may free the value that has passed on to this function so MMI layer should have the copy of this content if required. 
 */
JC_RETCODE jdd_MMISetTitle (JC_UINT32		uiWindowID,
							const JC_CHAR	*pmTitleValue) ;
/**
 * @brief This function gets the background and foreground color for the window.
 * @param[in] uiWindowID Specifies the window identifier.
 * @param[in] piBackgroundColor Provides the background color that is set to the specific window.
 * @param[in] piForegroundColor Provides the foreground color that is set to the specific window.
 */
JC_RETCODE jdd_MMIGetColor (JC_UINT32	uiWindowID,
							JC_INT32	*piBackgroundColor,
							JC_INT32	*piForegroundColor) ;
/**
 * @brief This function sets the background and foreground color for the window.
 * @param[in] uiWindowID Specifies the window identifier.
 * @param[in] iBackgroundColor Specifies the background color.
 * @param[in] iForegroundColor Specifies the foreground color.
 */
JC_RETCODE jdd_MMISetColor (JC_UINT32	uiWindowID,
							JC_INT32	iBackgroundColor,
							JC_INT32	iForegroundColor) ;
/**
 * @brief This function sets the default color for the window.
 * @param[in] uiWindowID Specifies the window identifier.
 * @param[in] bIsBackgroundColor Specifies the whether the default background color need to be set.
 * @param[in] bIsForegroundColor Specifies the whether the default foreground color need to be set.
 */
JC_RETCODE jdd_MMISetDefaultColor (JC_UINT32	uiWindowID,
								   JC_BOOLEAN	bIsBackgroundColor,
								   JC_BOOLEAN	bIsForegroundColor) ;
/**
 * @brief This function sets the wallpaper for the window at the location specified in pRect.
 * @param[in] uiWindowID Specifies the window identifier.
 * @param[in] stObjectInfo Specifies the mime type, object content and size of the object.
 * @param[in] eProperty Specifies the background property for the object.
 * @param[in] pstRect Specifies the location where the object needs to be rendered.
 */
JC_RETCODE jdd_MMISetWallpaper (JC_UINT32			uiWindowID,
								JC_OBJECT_INFO		stObjectInfo,
								EWallpaperProperty	eProperty,
								JC_RECT				*pstRect) ;


/**
 * @brief This function sets the wallpaper for the window at the location specified in pRect.
 * @param[in] uiWindowID Specifies the window identifier.
 * @param[in] pstWallPaperInfo Specifies the wallpaper info which holds mime type, object content,
 *							   size of the object, background property  , location where the object needs
 *						   	   to be rendered and magnification factor.
 */
JC_RETCODE jdd_MMISetWallpaperEx (JC_UINT32				uiWindowID,
								  JC_WALLPAPER_INFO		*pstWallPaperInfo) ;



/**
 * @brief This function clears the wallpaper that was earlier set using the API jdd_MMISetWallpaper.
 * @param[in] uiWindowID Specifies the window identifier.
 * @param[in] uiWallpaperID Reserved, not used.
 */
JC_RETCODE jdd_MMIClearWallpaper (JC_UINT32 uiWindowID, JC_UINT32 uiWallpaperID) ;

/**
 * @brief This function sets the specified window's show state.
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] bIsVisible Specifies the state of the window state. If it is JC_TRUE the window state shall be set to visible otherwise it shall be set to invisible.
 */
JC_RETCODE jdd_MMIShowWindow (JC_UINT32 uiWindowID, JC_BOOLEAN bIsVisible) ;

/**
 * @brief This function changes the position of the specified window. 
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] pstRect Specifies the new co-ordinates for the window
 */
JC_RETCODE jdd_MMIMoveWindow (JC_UINT32 uiWindowID, const JC_RECT *pstRect) ;

/**
 * @brief This function gets the current position, width and height of the specified window. 
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] pstRect Provides the current horizontal position, vertical position, width and height of the control
 */
JC_RETCODE jdd_MMIGetWindowRect (JC_UINT32 uiWindowID, JC_RECT *pstRect) ;

/**
 * @brief This function is used to focused or defocus the window.
 * @param[in] uiWindowID Specifies the identifier for the window.
 * @param[in] bIsFocus Specifies whether the focus is to be provided or not. E_TRUE specifies the window has to be focused.
 */
JC_RETCODE jdd_MMISetFocusWindow (JC_UINT32 uiWindowID, JC_BOOLEAN bIsFocus) ;

/**
 * @brief This function destroys the window and frees any memory the window is occupied. 
 * @param[in] uiWindowID Specifies the identifier for the window. 
 * @see jdd_MMICreateWindow
 */
JC_RETCODE jdd_MMIDestroyWindow (JC_UINT32 uiWindowID) ;

/**
 * @brief This function shall be invoked by the application before drawing or redrawing the controls on the specified window. 
 * @param[in] uiWindowID Specifies the identifier for the window 
 */
void jdd_MMIBeginDisplay (JC_UINT32 uiWindowID) ;
/**
 * @brief This function shall be invoked by the application after drawing or redrawing the controls on the specified window. 
 * @param[in] uiWindowID Specifies the identifier for the window.
 * @param[in] bRedraw Specifies whether the display needs to be redrawn. 
 */
void jdd_MMIEndDisplay (JC_UINT32 uiWindowID, JC_BOOLEAN bRedraw) ;

/*** End of Window ***/
/**
 * @brief This function should return the color palette for the given RGB combination which is very close to the given environment. 
 * @param[in] ucRedColor Specifies the red color
 * @param[in] ucGreenColor Specifies the green color
 * @param[in] ucBlueColor Specifies the blue color
 */
JC_UINT32 jdd_MMIGetRGBPalette (JC_UINT8 ucRedColor, JC_UINT8 ucGreenColor, 
								JC_UINT8 ucBlueColor) ;

/**
 * @brief This function should return the foreground color that can be visible 
 *        with the background color passed
 * @param[in] uiBackgroundColor Specifies the background color
 * @param[in] uiForegroundColor Specifies the foreground color
 */
JC_UINT32 jdd_MMIGetForegroundColor (JC_UINT32		uiBackgroundColor,
									 JC_UINT32		uiForegroundColor) ;

/**
 * @brief This function draws the rectangle on the specified window. 
 * @param[in] uiWindowID Specifies the identifier for the window  
 * @param[in] pstRect Specifies the rectangle position for the rectangle
 * @param[in] pstBorderProps Specifies the Border Properties for the rectangle. 
 *							 If the border property is NULL, then there is no need to create the rectangle with border.
 * @param[in] iBackgroundColor Specifies the background color for the rectangle. 
 *							 If the background color is less than zero the rectangle need to be created with the default background color. 
 * @param[out] puiRectID Specifies the rectangle identifier for the given rectangle area. 
 *					     If the environment does not support the rectangle has control this value need to be assigned to zero.
 * Some environment allows the rectangle to be created as a control. In othercase, this function shall draw the rectangle to the specified region without creating any control information. 
 * The control identified need to be assigned to zero.
 * @see jdd_MMIClearRect
 */
JC_RETCODE jdd_MMIDrawRect (JC_UINT32 uiWindowID, JC_RECT *pstRect,
							JC_BORDER_PROPS *pstBorderProps, JC_INT32 iBackgroundColor,
							JC_UINT32 *puiRectID) ;
/**
 * @brief This function clears the particular rectangle area. 
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] pstRect Specifies the rectangle position for the rectangle to be cleared
 * This function shall be used only if the environment does not support the rectangle has a control. It needs to clear the particular area.
 * @see jdd_MMIDrawRect
 */
JC_RETCODE jdd_MMIClearRect (JC_UINT32 uiWindowID, JC_RECT *pstRect) ;

/**
 * @brief This function provides the text metrics for the given text for the specified style and width. 
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] pstStyle Specifies the style attribute which needs to be applied for the text
 * @param[in] pmTextValue Specifies the text string 
 * @param[in] uiWidth Specifies the width in device units
 * @param[out] pstTextMetrics Provides the text metrics for the given style, text and width.
 *
 * This function should provide the number of characters that can be rendered for the given width and 
 * also provides the width that is required to render the specified number of characters. 
 * It also provides the height that is required to render the text. Depending on the text metrics that 
 * has been provided by this function the text control shall be created and positioned using the #jdd_MMICreateControl function. 
 * In order to have the zero-pixel accuracy in rendering this function should provide the exact number of characters and width 
 * that is required. 
 *
 * When uiWidth is zero, this function returns the width and height of the full string passed 
 * along with the total number of character.
 *
 * When uiWidth > zero, this function returns number of character that can be displayed 
 * within the specified uiWidth, and exact width and height for those number of characters.
 */
JC_RETCODE jdd_MMIGetTextMetrics (JC_UINT32 uiWindowID, const JC_STYLE *pstStyle,
								  const JC_CHAR *pmTextValue, JC_UINT32 uiWidth,
								  JC_TEXTMETRICS *pstTextMetrics) ;

/**
 * @brief This function provides the text metrics for the given text for the specified style and
 *		  is also based on the specified control type. 
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] pstStyle Specifies the style attribute which needs to be applied for the text
 * @param[in] pmTextValue Specifies the text string 
 * @param[in] uiWidth Specifies the width in device units
 * @param[in] eControlType	Specifies the type of control.
 * @param[out] pstTextMetrics Provides the text metrics for the given style, text and width.
 *
 * This function should provide the number of characters that can be rendered for the given width and 
 * also provides the width that is required to render the specified number of characters. 
 * It also provides the height that is required to render the text. Depending on the text metrics that 
 * has been provided by this function the text control shall be created and positioned using the #jdd_MMICreateControl function. 
 * In order to have the zero-pixel accuracy in rendering this function should provide the exact number of characters and width 
 * that is required. 
 *
 * When uiWidth is zero, this function returns the width and height of the full string passed 
 * along with the total number of character.
 *
 * When uiWidth > zero, this function returns number of character that can be displayed 
 * within the specified uiWidth, and exact width and height for those number of characters.
 *
 * If the eControlType parameter specifies the control to be a multiline control, then the width and the height
 * of the full string would be paased along with the total number of characters, else, the height specfied corresponds to the
 * the height of a single line.
 */
JC_RETCODE jdd_MMIGetTextMetricsEx (JC_UINT32 uiWindowID, const JC_STYLE *pstStyle,
								    const JC_CHAR *pmTextValue, JC_UINT32 uiWidth,
									EControlType eControlType,
									JC_TEXTMETRICS *pstTextMetrics) ;

/**
 * @brief This function gets the maximum text control width
 * @param[in] uiWindowID Specifies the window identifier
 
 * 
 * This function should return the maximum width of a text control that can be
 * created.  This value is used while creating the text control.  This width is
 * mapped against the jdd_MMIGetTextMetrics width
 */
JC_UINT32 jdd_MMIGetMaxTextWidth (JC_UINT32			uiWindowID) ;

/**
 * @brief This function checks whether the specified font or font-family available. 
 * @param[in] uiWindowID Specifies the window identifier
 * @param[in] pcFontName Specifies the font name.
 * @param[in] pcLang Specifies the language attribute, this is an optional parameter.
 * @param[out] piFontID Returns the font identifier for the specified font and this will be
 *						refered in the #JC_STYLE.
 * 
 * This shall be used by the application to choose the appropriate font. If the language
 * attribute is specified this function should check whether the specific font's has the
 * required font to render the language specific character.
 */
JC_RETCODE jdd_MMIIsFontAvailable (JC_UINT32 uiWindowID, JC_INT8 *pcFontName,
								   JC_INT8 *pcLang, JC_INT32 *piFontID) ;

/** Control **/
/**
 * @brief This function creates the different control depending on the type of control. 
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] eControlType Specifies the control type which need to be created 
 * @param[in] pstControlProperty Specifies the attribute value for this control
 * @param[in, out] pstRect Specifies the initial horizontal and vertical positions to render the text on the specified window. 
 *				           The actual width and height that has been required for rendering the text
 * @param[out] puiControlID Provides the identifier for the created control
 * The control shall be positioned on the specified location on the given window. 
 * If the width and height is specified (that is greater than zero) then the control needs to be created 
 * for the specific width and height. If the width and height of the control is zero then depending 
 * on the control style and device characteristics, the control needs to be created with 
 * the default width and height. The position of the control (left and top) shall be changed 
 * later by the application if required. 
 */
JC_RETCODE jdd_MMICreateControl (JC_UINT32 uiWindowID, EControlType eControlType, 
								 JC_CONTROL_PROPERTIES *pstControlProperty,
								 JC_RECT *pstRect, JC_UINT32 *puiControlID) ;
/**
 * @brief This function sets the specified control's show state.
 * @param[in] uiWindowID Specifies the identifier for the parent of the label window 
 * @param[in] uiControlID Specifies the control identifie
 * @param[in] bIsVisible Specifies the state of the control visibility state. If it is JC_TRUE the control state shall be set to visible otherwise it shall be set to invisible.
 */
JC_RETCODE jdd_MMIShowControl (JC_UINT32 uiWindowID, JC_UINT32 uiControlID,
							   JC_BOOLEAN bIsVisible) ;
/**
 * @brief This function sets the control's enable state.
 * @param[in] uiWindowID Specifies the identifier for the parent of the label window 
 * @param[in] uiControlID Specifies the control identifier 
 * @param[in] bIsEnable Specifies the state of the control enable state. If it is JC_TRUE the enable state shall be set to enabled, where user can make action, otherwise it shall be set to disabled.
 */
JC_RETCODE jdd_MMIEnableControl (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
								 JC_BOOLEAN bIsEnable) ;
/**
 * @brief This function changes the position of the specified control. 
 * @param[in] uiWindowID Specifies the identifier for the parent of the label window
 * @param[in] uiControlID Specifies the control identifier 
 * @param[in] pstRect Specifies the new co-ordinates of the control on the specified window
 */
JC_RETCODE jdd_MMIMoveControl (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
							   const JC_RECT *pstRect) ;
/**
 * @brief This function gets the control identifier that has the focus on the specified window.
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[out] puiControlID Returns the control identifier, which is of currently focused.
 */
JC_RETCODE jdd_MMIGetFocusedControl (JC_UINT32 uiWindowID, JC_UINT32 *puiControlID) ;
/**
 * @brief This function sets the focus to the specified control identifier on the specified window. 
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] uiControlID Specifies the control identifier, if the value is zero then the current focused control needs to be unfocused. 
 * @param[in] bIsFocus Specifies whether focus should be set or not for the control specified using the other two parameters.
 */
JC_RETCODE jdd_MMISetFocusedControl (JC_UINT32 uiWindowID, JC_UINT32 uiControlID,
									 JC_BOOLEAN bIsFocus) ;
/**
 * @brief This function gets the background and foreground color for the control.
 * @param[in] uiWindowID Specifies the window ID.
 * @param[in] uiControlID Specifies the control ID. 
 * @param[out] piBackgroundColor Returns the background color that is set to the specific window.
 * @param[out] piForegroundColor Returns the foreground color that is set to the specific window.
 */
JC_RETCODE jdd_MMIGetControlColor (JC_UINT32 uiWindowID, JC_UINT32 uiControlID,
								   JC_INT32 *piBackgroundColor, JC_INT32 *piForegroundColor) ;
/**
 * @brief This function sets the background and foreground color for the control.
 * @param[in] uiWindowID Specifies the window ID
 * @param[in] uiControlID Specifies the control ID
 * @param[in] iBackgroundColor Specifies the background color
 * @param[in] iForegroundColor Specifies the foreground color
 */
JC_RETCODE jdd_MMISetControlColor (JC_UINT32 uiWindowID, 
								   JC_UINT32 uiControlID,
								   JC_INT32 iBackgroundColor, 
								   JC_INT32 iForegroundColor) ;
/**
 * @brief This function sets the default color for background and foreground for the control depending on the bIsBackgroundColor and bIsForegroundColor parameters.
 * @param[in] uiWindowID Specifies the window ID
 * @param[in] uiControlID ] Specifies the control ID
 * @param[in] bIsBackgroundColor Specifies the whether the default background color need to be set.
 * @param[in] bIsForegroundColor Specifies the whether the default foreground color need to be set.
 */
JC_RETCODE jdd_MMISetDefaultControlColor (JC_UINT32 uiWindowID,
										  JC_UINT32 uiControlID,
										  JC_BOOLEAN bIsBackgroundColor,
										  JC_BOOLEAN bIsForegroundColor) ;
/**
 * @brief This function destroys the specified control and frees any memory the control is occupied. 
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] uiControlID Specifies the control identifier
 */
JC_RETCODE jdd_MMIDeleteControl (JC_UINT32 uiWindowID, JC_UINT32 uiControlID) ;

/**
 * @brief This function unlinks the control from the window and not frees/deletes the controls
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] uiControlID Specifies the control identifier
 */
JC_RETCODE jdd_MMIUnlinkControl (JC_UINT32 uiWindowID, JC_UINT32 uiControlID) ;

/**
 * @brief This function gets the current position, width and height of the specified control. 
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] uiControlID Specifies the control identifier
 * @param[out] pstRect Provides the current horizontal position, vertical position, width and height of the control
 */
JC_RETCODE jdd_MMIGetControlRect (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
								  JC_RECT *pstRect) ;
/**
 * @brief This function sets the new position, width and height for the specified control. 
 * @param[in] uiWindowID Specifies the identifier for the window 
 * @param[in] uiControlID Specifies the control identifier
 * @param[in] pstRect Specifies the new horizontal position, vertical position, width and height of the control
 */
JC_RETCODE jdd_MMISetControlRect (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
								  const JC_RECT *pstRect) ;

/** End of Control **/

/** Text Box **/
/**
 * @brief This function sets the label value to the text box control. 
 * @param[in] uiWindowID Specifies the identifier for the window.
 * @param[in] uiControlID Specifies the Text Box identifier
 * @param[in] pmTextValue Specifies the text value that needs to be set to the Text box. 
 */
JC_RETCODE jdd_MMISetTextValue (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
								JC_CHAR *pmTextValue) ;
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
						   JC_UINT32		*puiID) ;
/** End of Text Box **/

/** Input Box **/

/**
 * @brief This function gets the cursor position from the input control that is identified by the control ID.
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] uiControlID Specifies the control identifier
 * @param[out] piTextLen Returns the current cursor position. 
 */
JC_RETCODE jdd_MMIGetInputCurPosition (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
									   JC_INT32 *piTextLen)  ;

/**
 * @brief This function sets the cursor position from the input control identified by the control ID.
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] uiControlID Specifies the control identifier
 * @param[in] iTextLen Specifies the value as to where the cursor should be placed in the Input or Password control.
 */
JC_RETCODE jdd_MMISetInputCurPosition (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
									   JC_INT32 iTextLen)   ;

/**
 * @brief This function sets the input value to the input box control. 
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] pmInputValue Specifies the input value that needs to be set to the input or password box. 
 * @see jdd_MMIGetInputValue
 */
JC_RETCODE jdd_MMISetInputValue (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
								 const JC_CHAR *pmInputValue) ;

/**
 * @brief This function gets the input value for the input box control. 
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] uiControlID Specifies the control identifier
 * @param[out] ppmInputValue Returns the input value for the input/password/multi-line box. 
 *			   The MMI abstraction layer needs to allocate the memory and provide the reference to the application. 
 *			   The application shall call the #jdd_MMIFreeInputValue function to free this memory reference.
 * @see jdd_MMISetInputValue
 * @see jdd_MMIFreeInputValue
 */
JC_RETCODE jdd_MMIGetInputValue (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
								 JC_CHAR **ppmInputValue) ;

/**
 * @brief This function frees the input value that has been allocated using #jdd_MMIGetInputValue function. 
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] uiControlID Specifies the control identifier
 * @param[in] pmInputValue Specifies the input value for the input or password box which needs to be freed 
 * @see jdd_MMIGetInputValue
 */
void jdd_MMIFreeInputValue (JC_UINT32	uiWindowID, JC_UINT32	uiControlID, 
							JC_CHAR		*pmInputValue) ;

/** End of Input Box **/

/** Check Box **/
/**
 * @brief This function provides the status of the checkbox. 
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] uiControlID Specifies the control identifier
 * @param[out] peCheckBoxStatus Provides the state of the checkbox control. 
 */
JC_RETCODE jdd_MMIGetCheckBoxStatus (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
									 E_OPTIONBOX_STATUS	*peCheckBoxStatus) ;
/**
 * @brief This function sets the status of the check box. 
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] eCheckBoxStatus Specifies the state of the check box control.
 */
JC_RETCODE jdd_MMISetCheckBoxStatus (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
									 E_OPTIONBOX_STATUS eCheckBoxStatus) ;
/** End of Check Box **/

/** Radio Box **/
/**
 * @brief This function creates the group identifier for the radio box, to group different radio box control.
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[out] puiGroupRadioID Returns the group identifier.
 * @see jdd_MMIDeleteRadioBoxGroupID
 */
JC_RETCODE jdd_MMICreateRadioBoxGroupID (JC_UINT32 uiWindowID,
										 JC_UINT32 *puiGroupRadioID) ;
/**
 * @brief This function deletes the radio box group identifier that has been created using #jdd_MMICreateRadioBoxGroupID.
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] uiGroupRadioID Specifies the group identifier which needs to be freed.
 * @see jdd_MMICreateRadioBoxGroupID
 */
void jdd_MMIDeleteRadioBoxGroupID (JC_UINT32 uiWindowID, JC_UINT32 uiGroupRadioID) ;

/**
 * @brief This function provides the status of the radio box.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[out] peRadioBoxStatus Provides the Radio Box status. 
 */
JC_RETCODE jdd_MMIGetRadioBoxStatus (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
									 E_OPTIONBOX_STATUS	*peRadioBoxStatus) ;
/**
 * @brief This function sets the status of the radio box. 
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] eRadioBoxStatus Specifies the radio box status.
 */
JC_RETCODE jdd_MMISetRadioBoxStatus (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
									 E_OPTIONBOX_STATUS eRadioBoxStatus) ;
/** End of Radio Box **/

/** List Box **/
/**
 * @brief This function creates option group ID for ListBox. 
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] pstListItem Specifies the List item information structure
 * @param[out] puiOptgroupID Returns the identifier for the created optgroup. This ID should be unique among the list box.
 * @see jdd_MMIListBoxDeleteOptgroup
 */
JC_RETCODE jdd_MMIListBoxCreateOptgroup (JC_UINT32 uiWindowID, 
										 JC_UINT32 uiControlID,
										 const JC_LIST_BOX_ITEM	*pstListItem,
										 JC_UINT32				*puiOptgroupID) ;
/**
 * @brief This function deletes an optional group box.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] uiOptgroupID Specifies the Optional group id that should be deleted.
 */
JC_RETCODE jdd_MMIListBoxDeleteOptgroup (JC_UINT32		uiWindowID,
										 JC_UINT32		uiControlID,
										 JC_UINT32		uiOptgroupID) ;
/**
 * @brief This function sets the text for the individual items of the list box.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] uiIndexItem Specifies the index of the list box item to set the text value.
 * @param[in] pmText Specifies the text of the list box item.
 * @param[in] pstImageInfo Specifies the image info (content type, buffer, buffer len) [optional].
 */
JC_RETCODE jdd_MMIListBoxSetTextItem (JC_UINT32 uiWindowID,
									  JC_UINT32 uiControlID,
									  JC_UINT32 uiIndexItem,
									  const JC_CHAR *pmText,
									  const JC_IMAGE_INFO *pstImageInfo) ;
/**
 * @brief This function sets the text for the individual items of the list box. 
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] pstListItem Specifies the List item information structure.
 * The item can be either option group or option item. 
 */
JC_RETCODE jdd_MMIListBoxSetTextItemOpt (JC_UINT32 uiWindowID,
										 JC_UINT32 uiControlID,
										 const JC_LIST_BOX_ITEM *pstListItem) ;
/**
 * @brief This function sets the item that needs to be selected from the list. 
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] puiSelectedIndex Specifies the index of the list box items that need to be selected.
 * @param[in] uiSelectedCounts Specifies the number of items in the puiSelectedIndex.
 */
JC_RETCODE jdd_MMIListBoxSetItem (JC_UINT32 uiWindowID,
								  JC_UINT32 uiControlID,
								  JC_INT32 *puiSelectedIndex,
								  JC_UINT32 uiSelectedCounts) ;
/**
 * @brief This function gets the items that have been selected in the list control.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[out] ppiSelectedIndex Provides the indexes of the list box items that are selected
 * @param[out] puiSelectedCounts Provides the number of items that are selected.
 * @see jdd_MMIListBoxFreeItem
 */
JC_RETCODE jdd_MMIListBoxGetItem (JC_UINT32 uiWindowID,
								  JC_UINT32 uiControlID,
								  JC_INT32 **ppiSelectedIndex,
								  JC_UINT32 *puiSelectedCounts) ;

void jdd_MMIFreeListBoxSelectItem (JC_UINT32	uiWindowID,
								   JC_UINT32	uiListBoxID,
								   JC_UINT32	uiSelectedCounts,
								   JC_INT32		*piSelectedIndex) ;

/**
 * @brief This function gets the focus item index among the selected items in the list box.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[out] piFocusedIndex Provides the index of the list box item that has focus. 
 */
JC_RETCODE jdd_MMIListBoxGetFocusedItem (JC_UINT32 uiWindowID, 
										 JC_UINT32 uiControlID,
										 JC_INT32 *piFocusedIndex) ;
/**
 * @brief This function is used to free the memory associated with the item in the list box, which was previously allocated using the API jdd_MMIListBoxGetItem.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] piSelectedIndex Specifies the pointer, which needs to be freed. 
 * @see jdd_MMIListBoxGetItem
 */
void jdd_MMIListBoxFreeItem (JC_UINT32		uiWindowID, 
							 JC_UINT32		uiControlID, 
							 JC_INT32		*piSelectedIndex) ;
/**
 * @brief This function is used to delete the specified index from the list box.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] puiSelectedIndex Specifies the list of index that needs to be removed from the list box.
 * @param[in] uiSelectedCounts Specifies the number of list index that needs to be removed.
 */
JC_RETCODE jdd_MMIListBoxDeleteItem (JC_UINT32		uiWindowID,
									 JC_UINT32		uiControlID,
									 JC_INT32		*puiSelectedIndex ,
									 JC_INT32		uiSelectedCounts) ; 

/** End of List Box **/

/** Combo Box **/
/**
 * @brief This function sets the text for the individual items for the combo box.
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] uiControlID Specifies the control identifier
 * @param[in] uiIndexItem Specifies the index of the combo box item to set the text value
 * @param[in] pmText Specifies the text of the combo box item
 */
JC_RETCODE jdd_MMIComboBoxSetTextItem (JC_UINT32 uiWindowID, JC_UINT32 uiControlID, 
									   JC_UINT32 uiIndexItem, const JC_CHAR *pmText) ;

/**
 * @brief This function creates the option group ID for ComboBox. 
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] pstComboItem Specifies the Combo item information.
 * @param[in] puiOptgroupID Holds the unique group id that is created.
 * @see jdd_MMIComboBoxDeleteOptgroup
 */
JC_RETCODE jdd_MMIComboBoxCreateOptgroup (JC_UINT32 uiWindowID, JC_UINT32 uiControlID,
										  JC_COMBO_BOX_ITEM		*pstComboItem,
										  JC_UINT32				*puiOptgroupID) ;
/**
 * @brief This function deletes an optional group box.
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] uiControlID Specifies the control identifier
 * @param[in] uiOptgroupID Specifies the optgroup identifier that needs to be freed.
 * @see jdd_MMIComboBoxCreateOptgroup
 */
JC_RETCODE jdd_MMIComboBoxDeleteOptgroup (JC_UINT32		uiWindowID,
										  JC_UINT32		uiControlID,
										  JC_UINT32		uiOptgroupID) ;
/**
 * @brief This function sets the text for the individual items of the combo box. 
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] pstComboItem Specifies the reference to the combo item information.
 * The item can be option group or option item. 
 */
JC_RETCODE jdd_MMIComboBoxSetTextItemOpt (JC_UINT32 uiWindowID, JC_UINT32 uiControlID,
										  const JC_COMBO_BOX_ITEM *pstComboItem) ;
/**
 * @brief This function sets the item that needs to be selected from the list. 
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] iSelectedIndex Specifies the index of the combo box that needs to be selected.
 */
JC_RETCODE jdd_MMIComboBoxSetItem (JC_UINT32 uiWindowID,
								   JC_UINT32 uiControlID,
								   JC_INT32 iSelectedIndex) ;
/**
 * @brief This function gets the index that is selected for the combo box.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[out] piSelectedIndex Provides the index of the combo box that is selected.
 */
JC_RETCODE jdd_MMIComboBoxGetItem (JC_UINT32 uiWindowID, JC_UINT32 uiControlID,
								   JC_INT32 *piSelectedIndex) ;
/** End of Combo Box **/

/** Scrollbar **/
/**
 * @brief This function sets the scrollbar information to the specific scrollbar.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] pstScrollInfo Specifies the scroll bar information.
 */
JC_RETCODE jdd_MMISetScrollInfo (JC_UINT32 uiWindowID,
								 JC_UINT32 uiControlID,
								 const SCROLL_INFO *pstScrollInfo) ;

/**
 * @brief This function gets the scrollbar information to the specific scrollbar.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[out] pstScrollInfo Provides the scroll bar information
 */
JC_RETCODE jdd_MMIGetScrollInfo (JC_UINT32		uiWindowID,
								 JC_UINT32		uiControlID,
								 SCROLL_INFO	*pstScrollInfo) ;
/*	COOLSAND specific api. Don't remove	*/
/**
 * @brief This function gets the scrollbar information to the specific scrollbar.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[out] pstScrollInfo Provides the scroll bar information
 */
JC_RETCODE jdd_MMIGetScrollCarInfo (JC_UINT32 uiWindowID,
		JC_UINT32 uiControlID, JC_RECT *pScrollInfo) ;
/*	COOLSAND specific api. Don't remove	*/

/** End of Scrollbar **/

/** Softkeys **/
/**
 * @brief This function returns the number of soft keys that are available in the platform.
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[out] puiNumSoftKeys Provides the number of soft key that are available in the platform for the specified window.
 */
JC_RETCODE jdd_MMIGetNumberOfSoftKeys (JC_UINT32 uiWindowID,
									   JC_UINT32 *puiNumSoftKeys) ;
/**
 * @brief This function sets the label for the soft key.
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] uiSoftKeyIndex Specifies the index of the soft key label. The Index starts from 0. 
 * @param[in] pmSoftKeyLabel Specifies the string for the soft key label.
 */
JC_RETCODE jdd_MMISetSoftKeyLabel (JC_UINT32 uiWindowID, 
								   JC_UINT32 uiSoftKeyIndex, 
								   const JC_CHAR *pmSoftKeyLabel) ;
/** End of Softkeys **/

/** Object **/
/**
 * @brief This function should return the width and height of the image without fully decoding the image buffer. 
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] pstImageInfo Specifies the mime type, content and size of the image.
 * @param[out] puiWidth Provides the width that is required to render this image.
 * @param[out] puiHeight Provides the height that is required to render this image.
 */
JC_RETCODE jdd_MMIGetImageSize (JC_UINT32		uiWindowID,
								JC_IMAGE_INFO	*pstImageInfo, 
								JC_UINT32		*puiWidth, 
								JC_UINT32		*puiHeight) ;
/**
 * @brief This function should return the width and height of the image where the image content resides in the file system. 
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] pmFileName Specifies the image filename.
 * @param[out] puiWidth Provides the width that is required to render this image.
 * @param[out] puiHeight Provides the height that is required to render this image.
 */
JC_RETCODE jdd_MMIGetFileStreamImageSize (JC_UINT32 uiWindowID,
										  JC_CHAR	*pmFileName, 
										  JC_UINT32 *puiWidth, 
										  JC_UINT32 *puiHeight) ;
/**
 * @brief This function checks if the given URL is present in the pictogram list. 
 * @param[in] pmPictURL Specifies the URL that should be checked for the pictogram
 * 
 * Returns E_TRUE if the given URL is present in the list and E_FALSE otherwise. 
 * If the device does not support pictogram then this function can return E_FALSE.
 */
JC_BOOLEAN jdd_MMIIsPictogram (JC_CHAR *pmPictURL) ;

/**
 * @brief This function obtains the width and height of the pictogram specified by the URL.
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] pmURI Specifies the pictogram URI.
 * @param[out] puiWidth Provides the width that is required to render this pictogram image.
 * @param[out] puiHeight Provides the height that is required to render this pictogram image.
 */
JC_RETCODE jdd_MMIGetPictogramSize (JC_UINT32  uiWindowID,
									JC_CHAR		*pmURI,
									JC_UINT32	*puiWidth,
									JC_UINT32	*puiHeight) ;
/**
 * @brief This function provides the count of the different icons that are being used.
 * 
 */
JC_UINT32 jdd_MMIGetIconCount (void) ;

/**
 * @brief This function provides the size of the given icon type.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] eType Specifies the icon type.
 * @param[out] puiWidth Provides the width that is required to render this icon image.
 * @param[out] puiHeight Provides the height that is required to render this icon image.
 */
JC_RETCODE jdd_MMIGetIconSize (JC_UINT32		uiWindowID, 
							   EIconType		eType,
							   JC_UINT32		*puiWidth, 
							   JC_UINT32		*puiHeight) ;

/**
 * @brief This function provides the size of the given icon type based upon the fontsize.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] eType Specifies the icon type.
 * @param[in] pstStyle Specifies the style associated with the icon.
 * @param[out] puiWidth Provides the width that is required to render this icon image.
 * @param[out] puiHeight Provides the height that is required to render this icon image.
 */
JC_RETCODE jdd_MMIGetIconSizeEx (JC_UINT32		uiWindowID, 
								 EIconType		eType,
								 const JC_STYLE	*pstStyle,
								 JC_UINT32		*puiWidth, 
								 JC_UINT32		*puiHeight) ;

/**
 * @brief This function gets the dimensions of the object specified.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] pstObjectInfo Specifies the information about the object 
 * @param[in] vObjectHandle Specifies the object handle associated to this object, should be used when 
 *							this API is implemented as a non-blocking call.
 * @param[out] puiWidth Provides the width that is required to render this object.
 * @param[out] puiHeight Provides the height that is required to render this object.
 * @param[in] puiObjectID Provides the object identifier, if it is implemented as non-blocking call.
 * @see jdd_MMIFreeObjectSize 
 */
JC_RETCODE jdd_MMIGetObjectSize (JC_UINT32			uiWindowID,
								 JC_OBJECT_INFO		*pstObjectInfo, 
								 JC_HANDLE			vObjectHandle,
								 JC_UINT32			*puiWidth,
								 JC_UINT32			*puiHeight,
								 JC_UINT32			*puiObjectID) ;
/**
 * @brief This function free's the reference that is allocated while calculating the object size.
 * @param[in] uiWindowID Specifies the Window identifier
 * @param[in] uiObjectID Specifies the object identifier
 * @see jdd_MMIGetObjectSize
 */
void jdd_MMIFreeObjectSize (JC_UINT32		uiWindowID,
							JC_UINT32		uiObjectID) ;
/**
 * @brief This function performs the specified action on the given object.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] eAction Specifies the action type.
 */
JC_RETCODE jdd_MMIObjectAction (JC_UINT32			uiWindowID,
								JC_UINT32			uiControlID,
								E_OBJECT_ACTION		eAction) ;

/**
 * @brief This function gets the duration of the object action.
 * @param[in] uiWindowID Specifies the Window identifier.
 * @param[in] uiControlID Specifies the control identifier.
 * @param[out] piDuration Returns duration value of the object action.
 */
JC_RETCODE jdd_MMIGetObjectDuration (JC_UINT32		uiWindowID,
									 JC_UINT32		uiControlID,
									 JC_INT32		*piDuration) ;

/**
 * @brief This function sets the object content more
 * @param[in] uiWindowID    Specifies the Window identifier
 * @param[in] uiControlID   Specifies the control identifier
 * @param[in] pstObjectMore Specifies the reference to the object more content
 */
JC_RETCODE jdd_MMIObjectContentMore (JC_UINT32		uiWindowID,
									 JC_UINT32		uiControlID,
									 JC_OBJECT_MORE	*pstObjectMore) ;

/**
 * @brief This function checks if the given classid value is supported or not
 * @param[in] uiWindowID		Specifies the Window identifier
 * @param[in,out] pstObjClass   Specifies the information about the object
 * @retval	JC_OK	The given classid value is supported
 * @retval	JC_ERR_NOT_SUPPORTED	The given classid is not supported
 *
 * This function should be called by the application to check whether the 
 * given classid value is supported or not.
 */
JC_RETCODE jdd_MMIIsObjectSupported (JC_UINT32			uiWindowID,
									 JC_OBJECT_CLASS	*pstObjClass) ;

/** end of object **/

/**
 * @brief This function sets the specified cursor type.
 * @param[in] uiWindowID Specifies the Window identifier. 
 * @param[in] uiControlID Specifies the control identifier.
 * @param[in] eType Specifies the type of cursor.
 *
 * This function when called should change the current cursor displayed on the screen 
 * to the cursor type passed to this function.
 */
JC_RETCODE jdd_MMISetCursorType (JC_UINT32 uiWindowID, JC_UINT32 uiControlID,
									ECursorType	eType) ;


/** Others **/
/**
 * @brief This function displays the notification message either information message or error message depending on the enumeration type. 
 * @param[in] iRCode Specifies the information message need to be displayed according to the return code.
 * @param[in] eType Specifies the type of message.
 * @param[in] cbInfoFnc Specifies the callback function.
 * @param[in] pvArg Specifies the callback argument.
 * 
 * The callback function should be used only when this function is implemented as a non-blocking call. 
 */
JC_RETCODE jdd_MMINotificationMessage (JC_INT32					iRCode, 
									   EMMINotificationMsgType	eType, 
									   JDD_MMI_INFORMATION_CB	cbInfoFnc, 
									   void						*pvArg) ;

/**
 * @brief This function displays the confirmation message either "Ok/Cancel" or "Yes/No" depending on the enumeration type. 
 * @param[in] pmChar Specifies the string.
 * @param[in] eType Specifies the type of message.
 * @param[in] cbConfFnc Specifies the callback function.
 * @param[in] pvArg Specifies the callback argument.
 * If the environment does not support the blocking information message to be rendered the MMI should invoke the callback function with the argument once the notification message is closed.
 */
JC_RETCODE jdd_MMIConfirmationMessage (JC_CHAR					*pmChar, 
									   EMMIConfirmationMsgType	eType, 
									   JDD_MMI_CONFIRMATION_CB	cbConfFnc, 
									   void						*pvArg) ;
/** End of Others **/

/**
 * @brief This function checks if the given pictograph character can be displayed or not
 * @param[in]  uhPictCode	Pictographic character code
 * @return	E_TRUE	The given pictograph character can be displayed
 * @return	E_FALSE The given pictograph character can not be displayed
 */
JC_BOOLEAN jdd_MMIIsPictograph (JC_UINT16	uhPictCode) ;

/**
 * @}
 */

JC_BOOLEAN jApp_isSecMmiActive () ;

JC_RETCODE jdd_GetScrollInfoForWindow (JC_UINT32 uiWindowID, JC_RECT *pstScrollCarRect,
	JC_RECT *pstScrollBarRect);
JC_UINT32 jdd_GetScrollOffsetFactor(JC_UINT32 uiWindowID);





#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


