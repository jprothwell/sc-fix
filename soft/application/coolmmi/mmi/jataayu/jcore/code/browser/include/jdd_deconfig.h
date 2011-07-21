/***************************************************************************
 * $Id: jdd_deconfig.h,v 1.78 2009/08/05 11:17:54 shajudevhtmlbrow Exp $
 * $Revision: 1.78 $
 * $Date: 2009/08/05 11:17:54 $
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
 * $Log: jdd_deconfig.h,v $
 * Revision 1.78  2009/08/05 11:17:54  shajudevhtmlbrow
 * new config function jdd_DEConfigListBoxForSelect added.
 *
 * Revision 1.77  2009/05/27 06:59:17  kavithadevhtmlbrow
 * Doxygen comments updated
 *
 * Revision 1.76  2009/05/11 10:09:29  kavithadevhtmlbrow
 * Config for creating full window when background property is specified
 *
 * Revision 1.75  2009/04/29 07:16:40  kavithadevhtmlbrow
 * Doxygen comment updated
 *
 * Revision 1.74  2009/03/23 12:59:56  kavithadevhtmlbrow
 * jdd_DEConfigPageSize to jdd_DEConfigPageSizeByMime
 *
 * Revision 1.73  2009/03/09 12:27:28  kavithadevhtmlbrow
 * Update Indentation configuration based on new LI handling
 *
 * Revision 1.72  2009/03/04 13:04:12  kavithadevhtmlbrow
 * Doxygen Comments Updated
 *
 * Revision 1.71  2009/01/27 10:43:57  kavithadevhtmlbrow
 * Comment Updated
 *
 * Revision 1.70  2009/01/22 13:08:32  kavithadevhtmlbrow
 * JDD_RICH_CONTROL_ENABLE - Handling Border/Padding/Bgimage at JDD for form controls
 *
 *  
 ***************************************************************************/

/**
 * @page displayconfig Display Engine (DE) Configuration
 * 
 * @section supptypes DE Supported Media Types
 * @li Set the supported mime types using #jdd_DEConfigMimeTypes.
 * @li Set the supported image types using #jdd_DEConfigImageTypes.
 * @li Set the supported icon types using #jdd_DEConfigIconTypes.
 * @li Set the supported object types using #jdd_DEConfigObjectTypes.
 * @li Set the supported link types using #jdd_DEConfigLinkTypes.
 * @li Set the supported DRM types using #jdd_DEConfigDRMTypes.
 *
 * @section suppext DE Supported Extenstions
 * @li Set the supported image extensions using #jdd_DEConfigImageExt.
 * @li Set the supported text extensions using #jdd_DEConfigTextExt.
 * @li Set the supported object extensions using #jdd_DEConfigObjectExt.
 
 *
 * @section contentsize DE Content Size Configurations
 * @li Set the maximum size of a page using #jdd_DEConfigPageSizeByMime.
 * @li Set the maximum size of an image using #jdd_DEConfigImageSize.
 * @li Set the maximum size of an object using #jdd_DEConfigObjectSize.
 * @li Set the maximum size of a DRM content using #jdd_DEConfigDRMSize.
 *
 * @section watchdogop DE Watchdog and Asynchronous Configurations
 * @li Set the controls points for each controls using #jdd_DEConfigGetCtrlPoints.
 * @li Set the maximum control point after which the DE should break using #jdd_DEConfigGetMaxPoints.
 * @li Set the rendering timeout using #jdd_DEConfigGetRenderingTimeout.
 * @li Set the DE timer value using #jdd_DEConfigGetLayoutTimer.
 *
 * @section detimerop DE Timer Configurations
 * @li Set the timer when refresh happens using #jdd_DEConfigTimerOnRefresh.
 * @li Set the timeout value for save page functionality using #jdd_DEConfigGetSavePageTimeout
 *
 * @section descrollop DE Scroll Configurations
 * @li Set the speed at which slow scroll has to be done using #jdd_DEConfigGetSlowScroll.
 * @li Set the speed at which medium scroll has to be done using #jdd_DEConfigGetMediumScroll.
 * @li Set the speed at which fast scroll has to be done using #jdd_DEConfigGetFastScroll.
 *
 * @section dedisplayop DE Display Related Configurations
 * @subsection despacingop DE Spacing Configurations
 * @li Set the number of spacing between lines using #jdd_DEConfigGetLineSpacing.
 * @li Set the number of spaces which constitute a 'tab' using #jdd_DEConfigGetTabSpace.
 *
 * @subsection demarginop DE Margin Configurations
 * @li Set the left margin value for rendering window using #jdd_DEConfigGetLeftMargin.
 * @li Set the right margin value for rendering window using #jdd_DEConfigGetRightMargin.
 * @li Set the top margin value for rendering window using #jdd_DEConfigGetTopMargin.
 *
 * @subsection decellop DE Cell Configurations
 * @li Set the default cell padding for a cell using #jdd_DEConfigGetCellPadding.
 * @li Set the default cell spacing (between cells) using #jdd_DEConfigGetCellSpacing.
 *
 * @subsection demarqueeop DE Marquee Configurations
 
 * @li Set the timer value for a marquee using #jdd_DEConfigGetMarqueeTimer.
 * @li Set the slow marquee pixel movement value using #jdd_DEConfigGetSlowMarquee.
 * @li Set the normal marquee pixel movement value using #jdd_DEConfigGetNormalMarquee.
 * @li Set the fast marquee pixel movement value using #jdd_DEConfigGetFastMarquee.
 *
 * @subsection deborderop DE Border Configurations
 * @li Set the value of the thin border width using #jdd_DEConfigGetThinBorderWidth.
 * @li Set the value of the medium border width using #jdd_DEConfigGetMediumBorderWidth.
 * @li Set the value of the thick border width using #jdd_DEConfigGetThickBorderWidth.
 * @li Set the number of pixcel between the layout window and the internal controls using #jdd_DEConfigGetBorderSpacing.
 *
 * @subsection dedisareaop DE Display Area Options
 * @li Set the minimum display area width #jdd_DEConfigGetMinDisplayWidth.
 * @li Set the maximum value that can be accepted in the width attribute using #jdd_DEConfigGetStyleMaxWidth.
 * @li Set the maximum value that can be accepted in the height attribute using #jdd_DEConfigGetStyleMaxHeight.
 * @li Specify whether the height has to be adjusted based on the text area using #jdd_DEConfigAdjustTextareaHeight.
 * @li Specify whether to scale images while it is getting created using #jdd_DEConfigScaleImage.
 * @li Specify whether to scale objects while it is getting created using #jdd_DEConfigScaleObject.
 *
 * @section deothers DE Other Configurations
 * @li Specify whether to redraw the background image while redrawing using #jdd_DEConfigRedrawBgImage.
 * @li Sets the extra sizes for the form controls using #jdd_DEConfigGetDefaultSize
 * @li Set the PPI value for the current window using #jdd_DEConfigGetPPI.
 * @li Set the maximum number of redirections in a page using #jdd_DEConfigMaxRedirection.
 * @li Set the maximum number of request sent at a time using #jdd_DEConfigMaxRequestCount.
 * @li Set the maximum number of interpage that can be done using #jdd_DEConfigMaxInterpage.
 * @li Set the maximum navigation count using #jdd_DEConfigGetNavigationCount.
 * @li Specify the default CSS buffer using #jdd_DEConfigGetCSSBuffer.
 * @li Set the number of bytes that can be written to file at a time using #jdd_DEConfigGetFileWriteLength.
 * @li Set the maximum buffer length (beyond which content is written to file) using #jdd_DEConfigGetMaxBufferLength.
 * @li Set the path where the temporary files are stored using #jdd_DEConfigGetConfigPath.
 * @li Set the path separtor for file and directory names using #jdd_DEConfigGetPathSeparator.
 * @li Set the password character using #jdi_DEConfigGetPasswordChar.
 * @li Specify whether default access key should be used for invalid access keys using #jdd_DEConfigIgnoreInvalidAccesskey.
 * @li Specify whether non-cachable content can be saved (while performing save page opeartion) using #jdd_DEConfigSaveNonCachableContent.
 * @li Specify whether zero dimension property can be ignored for a image or object using #jdd_DEConfigIgnoreNegativeZeroDimension.
 * @li Set maximum number of post data bytes to be read from file (in case of file upload) using #jdd_DEConfigGetMaxPostDataLength.
 */

/**
 * @file jdd_deconfig.h
 * @ingroup DeConfig
 * @brief This file contains the prototypes for configuration functions used by DE
 */

#ifndef _JDD_DECONFIG_H
#define _JDD_DECONFIG_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Specifies the value for the maximum indentation level to be applied for list elements.
 */
#define CFG_MAX_INDENT_LEVEL		4


/**
 * @brief This enumeration specifies the various css types specified in the configuration. It
 *		  is used for reading the defualt css values for the tags from the corresponding files.
 */
typedef enum deConfigCSSFile
{
	E_DE_WML1_CSS, /**< Used to specify the default css file for wml1 tags. */
	E_DE_WML2_CSS, /**< Used to specify the default css file for wml2 tags. */
	E_DE_WML1_COMMON_CSS, /**< Specifies the WML1 common CSS type. */
	E_DE_WML2_COMMON_CSS, /**< Specifies the WML2 common CSS type. */
	E_DE_COMMON_CSS, /**< Specifies the common CSS type. */
	E_DE_CONFIG_CSS  /**< Specifies the config CSS type. Used only when JDD_RICH_CONTROL_ENABLE is defined **/
} E_DE_CFG_CSS_TYPE ;

/**
 * @brief This enumeration specifies the various control types.
 */
typedef enum deConfigCtrlType
{
	E_DE_CFG_TEXT_BOX = 0, /**< Specifies a text box control. */
	E_DE_CFG_INPUT_BOX, /**< Specifies an input box control. */
	E_DE_CFG_BUTTON, /**< Specifies a button control. */
	E_DE_CFG_CHECK_BOX, /**< Specifies a check box control. */
	E_DE_CFG_RADIO_BOX, /**< Specifies a radio box control. */
	E_DE_CFG_LIST_BOX,  /**< Specifies a list box control. */
	E_DE_CFG_COMBO_BOX, /**< Specifies a combo box control. */
	E_DE_CFG_IMAGE, /**< Specifies an image control. */
	E_DE_CFG_PICTOGRAM, /**< Specifies a pictogram control. */
	E_DE_CFG_IMAGE_TEXT, /**< Specifies an image text control. */
	E_DE_CFG_ICON, /**< Specifies an icon control. */
	E_DE_CFG_OBJECT, /**< Specifies an object control. */
	E_DE_CFG_SUBWINDOW, /**< Specifies a subwindow control. */
	E_DE_CFG_WND_BORDER,  /**< Specifies the window border. */
	E_DE_CFG_WALLPAPER,	  /**< Specifies the window wallpaper */
	E_DE_CFG_TOTAL_CTRL /**< Specifies the total number of controls. */
} E_DE_CFG_CTRL_TYPE ;

/**
 * @brief This enumerations specifies the various scaling option for objects/images
 */
typedef enum deConfigScaleType
{
	E_SCALE_NONE,  /**< Specifies that no scaling would be applied. Uses the specified/calculated width/height */
	E_SCALE_NODIM, /**< Specifies that scaling gets applied when dimensions are not specified. */
	E_SCALE_ALL    /**< Specifies that scaling gets applied even for a specified width and height. */
} E_DE_CFG_SCALE_TYPE ;

/**
 * @brief The following structure stores the default dimensions and other required
 *		  values for the various controls.
 */
typedef struct deConfigSize
{
	JC_UINT32		uiDefInputSize ; /**< Specifies the default number of characters visible inside an input box control. This value helps in calculating the default size for the input box control. */
	JC_UINT32		uiDefRadioWidth ; /**< Specifies the defualt width for the radio button. */
	JC_UINT32		uiDefRadioHeight ; /**< Specifies the default height for the radio button. */
	JC_UINT32		uiDefCheckWidth ; /**< Specifies the default width for a check box. */
	JC_UINT32		uiDefCheckHeight ; /**< Specifies the default height for a check box. */

	JC_UINT32		uiExtraInputWidth ; /**< Specifies the extra width for the proper display of input control. */
	JC_UINT32		uiExtraInputHeight ; /**< Specifies the extra height for the proper display of input control. */
	JC_UINT32		uiExtraListWidth ; /**< Specifies the extra width for the proper display of list box control. */
	JC_UINT32		uiExtraListHeight ; /**< Specifies the extra height for the proper display of list box control. */
	JC_UINT32		uiExtraListItemHeight ; /**< Specifies the extra height for individual list box items. */
	JC_UINT32		uiExtraComboWidth ;  /**< Specifies the extra width for the proper display of combo box control. */
	JC_UINT32		uiExtraComboHeight ; /**< Specifies the extra height for the proper display of combo box control. */
	JC_UINT32		uiExtraOptionSpace ; /**< Specifies the extra space for options, while calculating the width in case of optgroup. */
	JC_UINT32		uiExtraButtonWidth ; /**< Specifies the extra width for the proper display of button control. */
	JC_UINT32		uiExtraButtonHeight ; /**< Specifies the extra height for the proper display of button control. */
	JC_UINT32		uiExtraAltImageWidth ; /**< Specifies the extra width for the proper display of the alt image control. */
	JC_UINT32		uiExtraAltImageHeight ; /**< Specifies the extra height for the proper display of alt image control. */
	JC_UINT32		uiExtraTextareaWidth ; /**< Specifies the extra width for the proper display of textarea control. */
	JC_UINT32		uiExtraTextareaHeight ; /**< Specifies the extra height for the proper display of textarea control. */
	JC_UINT32		uiTextareaLineSpacing ; /**< Specifies the value for spacing between lines within a textarea control. */

	JC_UINT32		uiDefInputRows ; /**< Specifies the default number of rows to be displayed for a textarea control. This value helps in calculating the default height for the textarea control. */
	JC_UINT32		uiMaxInputRows ; /**< Specifies the maximum number of rows to be displayed for a textarea control. 
										  This value is checked agains the rows specified in the page, If this value 
										  is zero, then the rows specified in the page is used. if page doesnt
										  specify the row, uiDefInputRows value is used. uiMaxInputRows should be either zero
										  or a value greater than or equal to uiDefInputRows  */
	JC_UINT32		uiDefInputCols ; /**< Specifies the default number of columns to be displayed for a textarea control. This value helps in calculating the default width for the textarea control. */
	JC_UINT32		uiMaxInputCols ; /**< Specifies the maximum number of columns to be displayed for a textarea control. 
										  This value is checked against the columns specified in the page, If this value 
										  is zero, then the columns specified in the page is used. if page doesnt
										  specify the column, uiDefInputCols value is used. uiMaxInputCols should be either zero
										  or a value greater than or equal to uiDefInputCols   */
	JC_UINT32		uiDefListRows ; /**< Specifies the default number of rows for calculating the default height of a list box control */
	JC_UINT32		uiMaxListRows ; /**< Specifies the maximum number of rows for calculating height of a list box control.
										 This value is checked against the size specified in the page. If this value
										 is zero, then the size specified in the page is used. It page doesnt specify
										 the size, then uiDefListRows value is used. uiMaxListRows should be either zero
										 or a value greater than or equal to uiDefListRows */

#ifdef DE_OLD_LIST_LAYOUT
	JC_UINT32		uiListIndentSpace[CFG_MAX_INDENT_LEVEL] ; /**< Specifies the number of spaces depending upon the indentation level, used for the display of a list item. */
#else
	JC_UINT32		uiListIndentSpace[CFG_MAX_INDENT_LEVEL] ; /**< Specifies the percentage of padding value(configured in common.css) depending upon the indentation level, used for the display of a list item. */
#endif
	JC_UINT32		uiListTabSpace ; /**< Specifies the number of spaces signifying a tab for a list box control. */ 
	JC_UINT32		uiListMaxCharAllInd ; /**< Specifies the maximum character to be used for list margin - When ARR for indentation is All Indent */
	JC_UINT32		uiListMaxCharMaxInd ;  
	JC_UINT32		uiInputBorderSpace ; /**< Specifies the space to be left for drawing the border of the input controls. */
	JC_UINT32		uiObjectBorderSpace ; /**< Specifies the space to be left for drawing the border of the image/object controls. */
	JC_UINT32		uiLinkBorderSpace ; /**< Specifies the space to be left for drawing the border of the link text controls. */

	JC_UINT32		uiInputVerticalLineSpace ;  /**< Specifies the value for spacing between the form element controls and other content */
	JC_UINT32		uiObjectVerticalLineSpace ; /**< Specifies the value for spacing between the object controls and other content */
	JC_UINT32		uiInlineVerticalLineSpace ; /**< Specifies the value for spacing between the inline controls */

	JC_UINT32		uiMinTblInlineChar ; /**< Specifies the minimum number of characters which can be displayed inside a table cell. */
	JC_UINT32		uiMinTblInputCols ; /**< Specifies the minimum number of columns which can be displayed in a table. */
} ST_DE_CFG_SIZE ;



/**
 * @brief	This function returns a list of the supported MIME types to the Display Engine.
 * @return	Returns the list of supported MIME types.
 *
 * This function returns a list of the supported MIME types to the Display Engine. Verification of
 * the type of transmitted data, received through some communication protocol, with this list, ascertains
 * whether or not the Display Engine is capable of handling the received data.
 */
const JC_INT8 ** jdd_DEConfigMimeTypes (void) ;

/**
 * @brief	This function returns a list of the supported image types to the Display Engine.
 * @return	Returns the list of supported image types.
 *
 * This function returns a list of the supported image types to the Display Engine. After verification
 * with this list it is ascertained whether or not the received image can be rendered/displayed by the browser.
 */
const JC_INT8 ** jdd_DEConfigImageTypes (void) ;

/**
 * @brief	This function returns a list of the supported icon types to the Display Engine. This List
 *			should be increasing order of preference of the icon types.
 * @return	Returns the list of supported icon types.
 *
 * This function returns a list of the supported icon types to the Display Engine. After verification
 * with this list it is ascertained whether or not the received image can be processed by the browser.
 *
 * The list should also be increasing order of preference. i.e. the first item of the list should be
 * least prefered icon type and the last one should be the most prefered icon. This is to handle icon
 * types in case multiple icons are received in the content.
 */
const JC_INT8 ** jdd_DEConfigIconTypes (void) ;

/**
 * @brief	This function returns a list of the supported object types to the Display Engine.
 * @return	Returns the list of supported object types.
 *
 * This function returns a list of the supported object types to the Display Engine. After verification
 * with this list it is ascertained whether or not the received object can be rendered/displayed by the browser.
 */
const JC_INT8 ** jdd_DEConfigObjectTypes (void) ;

/**
 * @brief	This function returns a list of the supported link element relationship types to the Display Engine.
 * @return	Returns the list of supported link element relationship types.
 *
 * This function returns a list of the supported link element relationship types. After verification with this
 * list it is ascertained whether that particular link element would be handled by Display Engine and/or be
 * made available/displayed to the user.
 */
const JC_INT8 ** jdd_DEConfigLinkTypes (void) ;

/**
 * @brief	This function returns a list of the various supported DRM content types.
 * @return	Returns the list of the various supported DRM content types.
 *
 * This function returns a list of the various supported DRM content types. 
 */
const JC_INT8 ** jdd_DEConfigDRMTypes (void) ;

/**
 * @brief	This function returns a list of the supported extensions for an image, to the Display Engine.
 * @return	Returns the list of supported extensions for an image.
 *
 * This function returns a list of the supported extensions for an image, to the Display Engine.
 * If the type attribute for an object or embeded element has not been specified, then, the verification
 * of the object's extension with this list ascertains whether or not the received object/embeded
 * element is of a supported image type, thus enabling the approprate processing of the element.
 */
const JC_INT8 ** jdd_DEConfigImageExt (void) ;

/**
 * @brief	This function returns a list of the supported extensions for text objects, to the Display Engine.
 * @return	Returns the list of supported extensions for text objects.
 *
 * This function returns a list of the supported extensions for text objects, to the Display Engine.
 * If the type attribute for an object or embeded element has not been specified, then, the verification
 * of the object's extension with this list ascertains whether or not the received object/embeded
 * element is of a supported text object type, thus enabling the approprate processing of the element.
 */
const JC_INT8 ** jdd_DEConfigTextExt (void) ;

/**
 * @brief	This function returns a list of the supported extensions for objects, to the Display Engine.
 * @return	Returns the list of supported extensions for objects.
 *
 * This function returns a list of the supported extensions for objects, to the Display Engine.
 * If the type attribute for an object or embeded element has not been specified, then, the verification
 * of the object's extension with this list ascertains whether or not the received object/embeded
 * element is of a supported object type, thus enabling the approprate processing of the element.
 */
const JC_INT8 ** jdd_DEConfigObjectExt (void) ;

/**
 
  
 *
 
 * If the type attribute for an object or embeded element has not been specified, then, the verification
 * of the object's extension with this list ascertains whether or not the received object/embeded
 
 */
const JC_INT8 ** jdd_DEConfigMarkupExt (void) ;

/**
 * @brief	This function gives the maximum page size allowed depending upon the mime type.
 * @return	Returns the page size. Incase of no limit, returns zero.
 *
 * This function returns the maximum allowed page size depending upon the mime type. Incase of no limit,
 * zero is returned. Before processing the received page data, a comparision of the page's size with this value
 * ascertains whether or not the page data is to be processed.
 */

JC_INT32 jdd_DEConfigPageSizeByMime (JC_INT8	*pcMimeType) ;

/**
 * @brief	This function gives the maximum CSS content size allowed.
 * @return	Returns the CSS size. Incase of no limit, returns zero.
 *
 * This function returns the maximum allowed CSS content size. Incase of no limit, zero is returned.
 * Before processing the received CSS data, a comparision of the CSS's size with this value
 * ascertains whether or not the CSS data is to be processed.
 */
JC_INT32 jdd_DEConfigCSSSize (void) ;

/**
 * @brief	This function gives the maximum image size allowed.
 * @return	Returns the image size. Incase of no limit, returns zero.
 *
 * This function returns the maximum allowed image size. Incase of no limit, zero is returned.
 * If the size of the received image exceeds the value returned by this function,
 * the image would not be processed by the Display Engine.
 */
JC_INT32 jdd_DEConfigImageSize (void) ;

/**
 * @brief	This function gives the maximum object size allowed.
 * @return	Returns the object size. Incase of no limit, returns zero.
 *
 * This function returns the maximum allowed object size. Incase of no limit, zero is returned.
 * If the size of the received object exceeds the value returned by this function,
 * the object would not be processed by the Display Engine.
 */
JC_INT32 jdd_DEConfigObjectSize (void) ;

/**
 * @brief	This function gives the maximum DRM size allowed.
 * @return	Returns the DRM size. Incase of no limit, returns zero.
 *
 * This function returns the maximum allowed DRM size. Incase of no limit, zero is returned.
 * If the size of the received DRM object exceeds the value returned by this function,
 * the DRM object would not be processed by the Display Engine.
 */
JC_INT32 jdd_DEConfigDRMSize (void) ;

/**
 * @brief		This function gets points for every control
 * @param[in]	uiCtrlCnt		Specifies the count for the control.
 * @param[out]	puiCtrlPoints	Reference to hold the points
 * @return		No return value
 *
 * This function gets the points for each control. Each control has been alloted a certain point
 * value based upon the complexity of rendering that control (or the amount of time required for
 * rendering that control). This enables the Display Engine to keep a track of the current control
 * count. Whenever the maximum control count is reached, and the rendering timout is reached,
 * a break is made from processing and the control is given back to the
 * system. This avoids "WatchDog" type of issues.
 */
void jdd_DEConfigGetCtrlPoints (JC_UINT32		uiCtrlCnt,
								JC_UINT32		*puiCtrlPoints) ;

/**
 * @brief	This function gives the maximum number of points for the controls, to be rendered
 *			at any given time.
 * @return	Returns the maximum number of points. In case of no limit, returns zero.
 *
 * At any given moment, this function returns the maximum number of control points after which,
 * if the Display Engine timeout has occured, the control is given back to the system.
 * A control point value is allotted to each control based upon the complexity of rendering the same
 * (time taken by the system to render that control). While adding controls to layout, a track is
 * maintained of the total control count. If, at any time, this value exceeds the maximum number of points
 * allowed, a verification is made with the rendering timeout. If that too has expired, a break is made
 * from processing.
 */
JC_UINT32 jdd_DEConfigGetMaxPoints (void) ;

/*
 * @brief	This function gives the timeout value for rendering
 * @return	Returns the timeout value. In case of no limit, returns zero.
 *
 * This function returns the timeout value for rendering. Incase of no limit, zero is returned.
 * While processing the elements, if this value is exceeded, a break is made from processing and 
 * control is given back to the system.
 */
JC_UINT32 jdd_DEConfigGetRenderingTimeout (void) ;

/*
 
 * @return Timeout count, zero if no limit
 */
JC_UINT32 jdd_DEConfigGetRenderingTimeoutCount (void) ;

/**
 * @brief	This function gives the timer value for Layout
 * @return	Returns the Layout timer value, which should be in millisecs
 *
 * When timeout occurs during rendering, a timer is set for the value specified by this function. 
 * Once the time is expired, procesing of content continues.
 * 
 */
JC_UINT32 jdd_DEConfigGetLayoutTimer (void) ;

/**
 * @brief	This function gives the value for the spacing in between lines.
 * @return	Returns the value for the spacing between lines.
 *
 * This function returns the value for spacing in between lines. It is used for adding a vertical gap
 * in between lines and is used at the time of rendering the controls.
 */
JC_UINT32 jdd_DEConfigGetLineSpacing (void) ;

/**
 * @brief	This function gives the count for the number of spaces which constitute a tab
 * @return	Returns the count for number of spaces constituting a tab.
 *
 * This function returns the count for the number of spaces constituting a tab. This value is used
 * while processing the pre element and is used for counting the total length of a pre element's
 * inline content.
 */
JC_UINT32 jdd_DEConfigGetTabSpace (void) ;

/**
 * @brief	This function gives the value for the slow scroll level
 * @return	Returns the slow scroll level value
 *
 * This function returns the value for the slow scroll level. An option may be provided to the user
 * for setting the scroll level. If the user choses the slow scroll level, the scroll speed value for
 * the same is then obtained by using this function and is set as the Display Engine's scroll level value.
 */
JC_UINT32 jdd_DEConfigGetSlowScroll (void) ;

/**
 * @brief	This function gives the value for the medium scroll level
 * @return	Returns the medium scroll level value
 *
 * This function returns the value for the mediun scroll level. An option may be provided to the user
 * for setting the scroll level. If the user choses the medium scroll level, the scroll speed value for
 * the same is then obtained by using this function and is set as the Display Engine's scroll level value.
 */
JC_UINT32 jdd_DEConfigGetMediumScroll (void) ;

/**
 * @brief	This function gives the value for the fast scroll level
 * @return	Returns the fast scroll level value
 *
 * This function returns the value for the fast scroll level. An option may be provided to the user
 * for setting the scroll level. If the user choses the fast scroll level, the scroll speed value for
 * the same is then obtained by using this function and is set as the Display Engine's scroll level value.
 */
JC_UINT32 jdd_DEConfigGetFastScroll (void) ;

/**
 * @brief	This function gives the left margin value for the rendering window
 * @return	Returns the value for the left margin
 *
 * This function returns the left margin value for the rendering window. It specifies the
 * amount of space to be left between the left border of the window and the beginning of rendering
 * of the controls.
 */
JC_UINT32 jdd_DEConfigGetLeftMargin (void) ;

/**
 * @brief	This function gives the right margin value for the rendering window
 * @return	Returns the right margin value
 *
 * This function returns the right margin value for the rendering window. It specifies the
 * amount of space to be left between the right border of the window and the ending of rendering
 * of the controls.
 */
JC_UINT32 jdd_DEConfigGetRightMargin (void) ;

/**
 * @brief	This function gives the top margin value for the rendering window
 * @return	Returns the top margin value
 *
 * This function returns the top margin value for the rendering window. It specifies the
 * amount of space to be left between the top border of the window and the top position for
 * starting the rendering of controls.
 */
JC_UINT32 jdd_DEConfigGetTopMargin (void) ;

/**
 * @brief		This function gets the default/extra config sizes for the form controls
 * @param[out]	pstCfgSize	Reference to config size to be filled
 * @return		No return value
 *
 * This function fills the config size structure with the default/extra size values. The default
 * dimensions for the form controls are required for creating the form controls.
 */
void jdd_DEConfigGetDefaultSize (ST_DE_CFG_SIZE		*pstCfgSize) ;

/**
 * @brief	This function gets the PPI for the current window
 * @return	Returns the PPI value
 *
 * This function returns the PPI value for the current window. This value is used for proper calculation
 * of the display units.
 */
JC_UINT32 jdd_DEConfigGetPPI (void) ;

/**
 * @brief	This function gives the maximum number of redirections allowed for any page.
 * @return	Returns the maximum redirection count
 *
 * This function gives the maximum number of redirections allowed for any page, thus
 * avoiding cases of infinite redirections. At the time of making a page URI redirection or
 * an inline URI redirection, a verification is made against the value returned by this function.
 * If the number of redirections ecxeeds the maximum limit, an error would be returned by the
 * Display Engine.
 */
JC_UINT32 jdd_DEConfigMaxRedirection (void) ;

/**
 * @brief	This function gets the maximum request count that can be sent at any given time.
 * @return  Returns the maximum request count
 *
 * This function gets the maximum request count that can be sent at any given time.
 */
JC_UINT32 jdd_DEConfigMaxRequestCount (void) ;

/**
 * @brief	This function gives the maximum number of interpage count.
 * @return	Returns the maximum interpage count
 *
 * This function returns the maximum number of interpage count. At the time of displaying an interpage
 * URL, a verification with the value returned by this function results in either the interpage fragment
 * being displayed, or, an error being returned.
 */
JC_UINT32 jdd_DEConfigMaxInterpage (void) ;

/**
 * @brief	This function gets the maximum navigation count
 * @return	Returns the maximum navigation count
 *
 * This function returns the maximum navigation count. This specifies the count for the maximum
 * number of previously fetched pages that would be maintained for navigation (backward navigation)
 * purpose.
 */
JC_UINT32 jdd_DEConfigGetNavigationCount (void) ;

/**
 * @brief	This function gets the default cell padding value
 * @return	Returns the default cell padding value
 *
 * This function returns the default cell padding value to be used for a cell. This is the 
 * space between the cell border and cell content and would be applied on all the four sides of the
 * cell.
 */
JC_UINT32 jdd_DEConfigGetCellPadding (void) ;

/**
 * @brief	This function gets the default cell spacing value
 * @return	Returns the cell spacing value
 *
 * This function returns the default cell spacing value which specifies the default value to
 * be left in between two cells.
 */
JC_UINT32 jdd_DEConfigGetCellSpacing (void) ;

/**
 * @brief	This function gets the value for infinite looping of marquee
 * @return	Returns the value for the infinite looping of the marquee.
 *
 * If the infinite attribute has been specified for a marquee control, the looping/scrolling
 * of the marquee would be stopped once the value, returned by this function, is reached.
 */
JC_INT32 jdd_DEConfigGetMarqueeInfinite (void) ;

/**
 * @brief This function gets the default loop count for wap-marquee
 * @return marquee default loop count
 */
JC_INT32 jdd_DEConfigGetDefaultWAPMarqueeLoopCount (void) ;

/**
 * @brief	This function gets the thin border width value
 * @return	Returns the thin border width value
 *
 * This function returns the thin border width value. While creating the borders for a box/cell,
 * if the thin attribute for borders has been speicfied then this function is used for
 * obtaining the value of the width of a thin border.
 */
JC_UINT32 jdd_DEConfigGetThinBorderWidth (void) ;

/**
 * @brief	This function gets the medium border width value
 * @return	Returns the medium border width value
 *
 * This function returns the medium border width value. While creating the borders for a box/cell,
 * if the medium attribute for borders has been speicfied then this function is used for
 * obtaining the value of the width of a meduium thickness border.
 */
JC_UINT32 jdd_DEConfigGetMediumBorderWidth (void) ;

/**
 * @brief	This function gets the thick border width value
 * @return	Returns the thick border width value
 *
 * This function returns the thick border width value. While creating the borders for a box/cell,
 * if the thick attribute for borders has been speicfied then this function is used for
 * obtaining the value of the width of a thick border.
 */
JC_UINT32 jdd_DEConfigGetThickBorderWidth (void) ;

/**
 * @brief	This function gives the marquee timer value 
 * @return	Returns the timer value for marquee. The value should be specified in millisecs
 *
 * This function returns the timer value for the marquee.
 */
JC_UINT32 jdd_DEConfigGetMarqueeTimer (void) ;

/**
 * @brief	This function gets the value for slow marquee pixel movement
 * @return	Returns the value for slow marquee pixel movement.
 *
 * This function returns the value for slow marquee pixel movement. It specifies the speed for
 * slow marquee movement (the marquee for which slow scroll level has been set).
 */
JC_UINT32 jdd_DEConfigGetSlowMarquee (void) ;

/**
 * @brief	This function gets the value for normal marquee pixel movement
 * @return	Returns the value for normal marquee pixel movement.
 *
 * This function returns the value for normal marquee pixel movement. It specifies the speed for
 * normal marquee movement (the marquee for which medium scroll level has been set).
 */
JC_UINT32 jdd_DEConfigGetNormalMarquee (void) ;

/**
 * @brief	This function gets the value for fast marquee pixel movement
 * @return	Returns the value for fast marquee pixel movement.
 *
 * This function returns the value for fast marquee pixel movement. It specifies the speed for
 * fast marquee movement (the marquee for which fast scroll level has been set).
 */
JC_UINT32 jdd_DEConfigGetFastMarquee (void) ;

/**
 * @brief	This function gets the number of pixels that need to be provided
 *			between the border and the internal controls.
 * @return	Returns the number of pixels which need to be provided between
 *			the border and the internal controls.
 *
 * This function returns the number of pixels that need to be provided between the layout window
 * border and the internal controls.
 */ 
JC_UINT32 jdd_DEConfigGetBorderSpacing (void) ;

/**
 * @brief		This function gets the CSS Buffer, from the file, based on the type
 * @param[in]	eType			Specifies the CSS default file type
 * @param[out]	ppcBuffer		Reference to hold the css buffer
 * @param[out]	puiLength		Reference to hold the buffer length
 * @param[out]	pbFreeMemory	Reference to hold the flag for freeing the memory
 * @retval		JC_OK			Success on getting the buffer
 * @retval		Others			Error on getting the buffer
 *
 * Based upon the type of CSS specified, this function reads the corresponding CSS files, that
 * contain the CSS details for the specified CSS type, and returns the CSS buffer. This
 * function is called while reading the default CSS.
 */
JC_RETCODE jdd_DEConfigGetCSSBuffer (E_DE_CFG_CSS_TYPE		eType,
									 JC_INT8				**ppcBuffer,
									 JC_UINT32				*puiLength,
									 JC_BOOLEAN				*pbFreeMemory) ;

/**
 * @brief	This function gets the number of bytes that can be written 
 *			into a file at a time. If it is 0 then it is assumed that 
 *			there is no limit on the file write operation
 * @return	Returns the number of bytes that can be written into a file.
 *
 * This function returns the number of bytes that can be written into a file at any
 * given time. For specifying a limitless number of bytes that can be written, zero is 
 * returned. This function is used at the time of saving a fetched page when the
 * fetched page's data is written into files.
 */
JC_UINT32 jdd_DEConfigGetFileWriteLength (void) ;

/**
 * @brief	This function is used to obtain the maximum buffer length, based upon which 
 *			it decides when to write the content into the file.
 * @retval	-1	Specifies that the content buffer should never be written into the file.
 * @retval  0	Specifies that the content buffer needs to be written each time into the file.
 * @retval  > 0	Specifies that the content buffer needs to be concatenated until the limit is
 *				reached, and then, written into the file.
 *
 * This function gets the buffer length on which it decides when to write the content
 * in to the file. If it is -1 never write the content buffer to file. If it is 0
 * each time content need to write. If it is > 0  then append the content to the
 * buffer until it reaches the limit, and once it reaches it needs to write in the file.
 */
JC_INT32 jdd_DEConfigGetMaxBufferLength (void) ;

/**
 * @brief			This function gets config path to store the temporary content files
 * @param[in,out]	pmPath		The configuration path
 * @param[in]		puiSize		The size of the configuration path
 * @retval			JC_OK		When the path for storing the temporary content files has
 *								been created successfully.
 * @retval			Others		In case sufficient memory is unavaliable at the
 *								specified path.
 *
 * This function gets the config path for storing the temporary content files.
 */
JC_RETCODE jdd_DEConfigGetConfigPath (JC_CHAR		*pmPath,
									  JC_UINT32		*puiSize) ;

/**
 * @brief			This function gets the path separator to be used in files and 
 *					directory names.
 * @param[in,out]	pcSlash		Specifies the path separator
 * @return			No return value
 *
 * This function is used to get the path separator to be used for file or directory names.
 */
void jdd_DEConfigGetPathSeparator (JC_INT8	*pcSlash) ;

/**
 * @brief			This function gets the flag for redrawing when the backgroung image
 *					is set for a sub window.
 * @retval E_TRUE	Indicates that redraw is required when background image is set.
 * @retval E_FALSE	Indicates that redraw is not required when background image is set.
 *
 * This function gets the flag for redrawing when the background image is set for a sub window.
 */
JC_BOOLEAN jdd_DEConfigRedrawBgImage (void) ;

/**
 * @brief	This function gets the password character used to find the size of input box
 * @return	Returns the password character
 *
 * This function is used to obtain the password character for calculating the width/height of the 
 * password input box.
 */
JC_CHAR jdi_DEConfigGetPasswordChar (void) ;

/**
 * @brief	This function gets the flag for setting timer when refresh event 
 *			occurs through refresh tag.
 * @retval	E_TRUE	Set Timer on refresh (if exists)
 * @retval	E_FALSE	Ignore Timer on refresh 
 *
 * This function is used to get the flag for setting the timer whenever a refresh event
 * occurs through the refresh tag.
 */
JC_BOOLEAN jdd_DEConfigTimerOnRefresh (void) ;

/**
 * @brief	This function specifies whether or not to apply image scaling  
 * @returns	Scaling type (#E_DE_CFG_SCALE_TYPE)
 *
 * This function is used to specify whether or not to apply image scaling and is used while
 * creating images. If no scaling is specified and the image's dimensions too have been specified,
 * the image would not be scaled. But if scaling is specified then irrespective of the image's
 * dimensions, scaling would be applied to the image based upon the current width and height.
 */
E_DE_CFG_SCALE_TYPE jdd_DEConfigScaleImage (void) ;

/**
 * @brief	This function specifies whether or not to apply object scaling  
 * @returns	Scaling type (#E_DE_CFG_SCALE_TYPE)
 *
 * This function is used to specify whether or not to apply object scaling and is used while
 * creating objects. If no scaling is specified and the object's dimensions too have been specified,
 * the object would not be scaled. But if scaling is specified then irrespective of the object's
 * dimensions, scaling would be applied to the object based upon the current width and height.
 */
E_DE_CFG_SCALE_TYPE jdd_DEConfigScaleObject (void) ;

/**
 * @brief	This function returns a flag indicating whether the default accesskey should
 *			be displayed in the place of invalid accesskeys.
 * @retval	E_TRUE  Ignore invalid accesskey
 * @retval	E_FALSE Display default accesskey (if available)
 *
 * This function returns a flag indicating whether the default accesskeys should be displayed
 * in place of an invalid accesskey. 
 */
JC_BOOLEAN jdd_DEConfigIgnoreInvalidAccesskey (void) ;

/**
 * @brief  This function returns the minimum visible display area width
 * @return Returns the visible minimum display area width
 *
 * This function returns the minimum display area width. When control/content width, which is to be
 * rendered is less than this width, then few CSS properties like margin/padding etc. will be
 * ignored/scaled. A -1 value suggests four way navigation is active, or that no scaling/ignoring of
 * any of the CSS properties is required.
 */
JC_INT32 jdd_DEConfigGetMinDisplayWidth (void) ;

/**
 * @brief  This function returns the maximum value that can be specified in the width attribute,
 *		   or, which can be specified in the style, for a block control.
 * @return Returns the maximum object width
 *
 * This function returns the maximum value that can be specified in the width attribute,
 * or, which can be specified in the style, for a block control. If the specified value exceeds this
 * value, the block control would then be rendered with the default width specified for that
 * particular control.
 */
JC_INT32 jdd_DEConfigGetStyleMaxWidth (void) ;

/**
 * @brief  This function returns the maximum value that can be specified in the height attribute,
 *		   or, which can be specified in the style, for a block control.
 * @return Returns the maximum object height
 *
 * This function returns the maximum value that can be specified in the height attribute,
 * or, which can be specified in the style, for a block control. If the specified value exceeds this
 * value, the block control would then be rendered with the default height specified for that
 * particular control.
 */
JC_INT32 jdd_DEConfigGetStyleMaxHeight (void) ;

/**
 * @brief	This function returns the flag specifying whether or not non-cachable content
 *			can be saved when trying to save the page.
 * @retval E_TRUE  Save non-cachable content also
 * @retval E_FALSE Save only cachable content
 *
 * This function returns a boolean value that specifies whether or not non-cachable content
 * (no-cache or no-store cache attributes specified) can be saved when trying to save the page.
 */
JC_BOOLEAN jdd_DEConfigSaveNonCachableContent (void) ;

/**
 * @brief This function ignores the negative or zero dimension attribute for 
 *        image and object 
 * @retval E_TRUE  Ignores and uses the image/object's dimension
 * @retval E_FALSE Creates the image/object control with zero dimension
 *
 * This function returns a boolean value that specifies whether the negative or 
 * zero dimension attribute specified in the page for image/object can be ignored.
 * Recommended to use E_FALSE to clear the test suits.
 */
JC_BOOLEAN  jdd_DEConfigIgnoreNegativeZeroDimension (void) ;

/**
 * @brief This function returns a flag for adjusting textarea height
 * @retval E_TRUE  Adjust the textarea height as per the rows
 * @retval E_FALSE Use the specified height
 *
 * This function returns a boolean value that specifies whether the textarea
 * height has to be adjusted based on the number of rows of the textarea, linespacing and extra
 * height, or uses the specified height, if any.
 */
JC_BOOLEAN jdd_DEConfigAdjustTextareaHeight (void) ;

/*
 * @brief	This function gives the timeout value for savepage functionality
 * @return	Returns the timeout value. In case of no limit, returns zero.
 *
 * This function returns the timeout value for savepage functionality. 
 * Incase of no limit,zero is returned.
 */
JC_UINT32 jdd_DEConfigGetSavePageTimeout (void) ;

/**
 * @brief This function gets the number of bytes of post data that can be read 
 *		  from an input file incase of file upload.
 * @return	Returns the length of post data that can be read from the input file.
 *
 * If returns the length of post data that can be read from an input file 
 * incase of file upload.If it is 0 then it specifies that there is no limit 
 * on the file read operation for post data file.
 */
JC_UINT32 jdd_DEConfigGetMaxPostDataLength (void) ;

/**
 * @brief	This function gives the maximum number of embed page count.
 * @return	Returns the maximum embed page count
 *
 * This function returns the maximum number of embed page count. At the time of processing an embed page
 * a verification with the value returned by this function results in either the rendering embed page or 
 * rendering alternate to it.
 */
JC_UINT32 jdd_DEConfigMaxEmbedPage (void) ;

/**
 * @brief	This function gives the maximum number of nested child XML tree can hold.
 * @return	Returns the maximum nested child count
 *
 * This function returns the maximum number of nested child XML tree can hold. At the time of forming 
 * the tree this value used to restrict the nesting level. If Zero, there will be no restriction.
 */
JC_UINT32 jdd_DEConfigMaxNestedChild (void)  ;

/**
 * @brief This function gets the percentage value by which UL/OL lists should be compacted
 *		  when the compact attribute has been specified.
 * @return percentage value by which the lists should be compacted
 *
 * This function returns the percentage value by which a UL/OL list should be compacted when
 * the compact attribute has been specified for either of the UL/OL elements.
 */
JC_UINT32 jdd_DEConfigCompactPercentage (void)  ;

/**
 * @brief This function obtains the default URI for the browser windows.
 * @retval	BROWSER_DEFAULT_URI	Specifies the default browser URI.
 *
 * This function obtains the default URI for the browser windows.
 */
JC_INT8 * jdd_DEConfigGetDefaultURI (void) ;

/**
 * @brief This function specifies the flag for creating the main window
 
 * @retval E_FALSE If window with background property to be created with the visible width and height
 *
 * This function used to get a flag for dimension to be used for creating the main window when 
 * its background property are specified like background color.
 */
JC_BOOLEAN jdd_DEConfigCreateFullWindow (void) ;

/**
 * @breif This function returns true if the select tag always need to be trated as listbox. The "size" 
 * attribute should be ignored and all items should be visible.
 * @param     No parameter
 * @retval E_TRUE	If the select tag should be treated as listbox always and size attribute should 
 *					be ignored
 * @retval E_FALSE	The decision whether select tag should be treated as combo box or list box will be
 *					taken by DE considering "size" attribute.
 *
 */
JC_BOOLEAN jdd_DEConfigListBoxForSelect (void) ;

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


