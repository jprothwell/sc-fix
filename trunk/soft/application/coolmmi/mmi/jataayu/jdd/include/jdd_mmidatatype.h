/***************************************************************************
 * $Id: jdd_mmidatatype.h,v 1.25 2008/09/30 09:44:15 browserdevjtbase Exp $
 * $Revision: 1.25 $
 * $Date: 2008/09/30 09:44:15 $
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
 * $Log: jdd_mmidatatype.h,v $
 * Revision 1.25  2008/09/30 09:44:15  browserdevjtbase
 * Core Updates
 *
 * Revision 1.24  2008/08/14 08:25:27  browserdevjtbase
 * Core Updates
 *
 * Revision 1.23  2008/06/30 06:09:28  browserdevjtbase
 * Core Update
 *
 * Revision 1.22  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.21  2008/05/29 14:14:45  browserdevjtbase
 * Core Update
 *
 * Revision 1.20  2008/05/15 08:18:43  browserdevjtbase
 * Core Update
 *
 * Revision 1.19  2008/04/15 12:56:25  browserdevjtbase
 * Updated.
 *
 * Revision 1.18  2008/01/28 05:56:46  browserdevjtbase
 * Core Updated.
 *
 * Revision 1.17  2008/01/15 11:57:45  browserdevjtbase
 * Updated the jtbase.
 *
 * Revision 1.16  2007/12/14 06:16:02  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.15  2007/11/29 05:44:55  browserdevjtbase
 * New style added for marquee.
 *
 * Revision 1.14  2007/11/29 05:37:25  browserdevjtbase
 * Added the MMI for the poly and control caching when doing the re-draw.
 *
 * Revision 1.13  2007/11/20 14:45:23  browserdevjtbase
 * Updated.
 *
 * Revision 1.12  2007/10/15 07:16:46  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.11  2007/07/27 10:52:59  browserdevjtbase
 * Update
 *
 * Revision 1.10  2007/07/23 11:04:34  browserdevjtbase
 * Core update
 *
 * Revision 1.9  2007/04/10 14:23:22  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.8  2007/04/03 07:13:39  browserdevjtbase
 * JCF Update
 *
 * Revision 1.7  2007/01/12 11:18:05  browserdevjtbase
 * Update from the main base
 *
 * Revision 1.6  2006/11/29 13:09:24  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.5  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.4  2006/07/03 07:26:11  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.3  2006/06/20 13:28:37  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.31  2006/02/28 11:00:53  kavithadevhtmlbrow
 * JC_CONTROL_DIMENSION and E_ASYNC_CTRL_EVENT added
 *
 * Revision 1.30  2006/01/31 07:47:17  kavithadevhtmlbrow
 * Stylus Datatype Updated
 *
 *  
 ***************************************************************************/

#ifndef _JDD_MMI_DATATYPE_H
#define _JDD_MMI_DATATYPE_H

/** 
 * @file jdd_mmidatatype.h
 * @ingroup MMI
 * @brief Describes the basic data type that are getting used in the
 * MMI abstraction API's
 */

/**
 * @brief Specifies the maximum size for the font name.
 */
#define STYLE_FONT_NAME_SIZE			1

/**
 * @brief Specifies the key code values.
 * Note:- For alphabetic keys, key code will be ASCII key codes for the key. 
 *		  For Keyboard number keys ,ASCII code of number (48 + number), even if 
 *				modifiers are held down.
 *		  For Keypad number keys, 96 plus number.
 */
#define KEY_SPACE						32 /**< Space Key */
#if 0
#define KEY_ENTER						13 /**< Enter Key*/
#endif

#define KEY_TAB							9 /**< Tab Key */
#define KEY_BACKSPACE					8 /**< Backspace Key */
#define KEY_SHIFT						16 /**< Shift Key */
#define KEY_CTRL						17 /**< Ctrl key */
#define KEY_ALT							18 /**< Alt Key */
#define KEY_LARROW						37 /**< Left Arrow Key */
#define KEY_UPARROW						38 /**< Up Arrow Key */
#define KEY_RARROW						39 /**< Right Arrow Key */
#define KEY_DNARROW						40 /**< Down Arrow Key */
#define KEY_COLON						59 /**< Colon-Semicolon Key */
#define KEY_PLUSEQ						61 /**< Plus Equal Key */
#define KEY_COMMALSHIFT					188 /**< Comma-Left shift Key */
#define KEY_DASHUNDERSCORE				109 /**< Dash-Underscore Key */
#define KEY_DOTRSHIFT					190 /**< Dot-Right shift Key */
#define KEY_SLASHWHAT					191  
#define KEY_TILDE						192 /**< Tilde-Apostrophe Key  */
#define KEY_LSQUAREBRACKET				219 /**< Left Sqaure Bracket-Left Curly Brace Key */
#define KEY_BACKSLASH					220 /**< Back Slash-Pipe Key  */
#define KEY_RSQUAREBRACKET				221 /**< Right Sqaure Bracket-Right Curly Brace Key */
#define KEY_QUOTE						222 /**< Quote-Double Quote Key */
/** @cond */
#define TRANSPARENT_VALUE				"transparent"
#define MMI_DEFAULT_COLOR				-2				/**< Default Color value */
#define MMI_TRANSPARENT_COLOR			-1				/**< Transparent Color value */
#define HASH_ACCESS_KEY					10
#define STAR_ACCESS_KEY					11
/** @endcond */

/**
 * @brief This enumeration contains the different window style that can be created using the 
 * jdd_MMICreateWindow. 
 */
typedef enum e_window_style
{
	E_MAIN_WINDOW, /**< Specifies the window style where it has the option for the title bar with a thin-line border separating the title area and the actual window space */
	E_CHILD_WINDOW, /**< Specifies the window style which is the child of the other window, the parent window can be created either using E_MAIN_WINDOW or E_CHILD_WINDOW itself */
	E_POPUP_WINDOW, /**< Specifies the pop-up window style */
	E_APPLICATION_WINDOW /**< Specifies the application level window */
} EWindowStyle, E_WINDOW_STYLE ;

/**
 * @brief This enumeration contains the different controls that can be created using the 
 * MMI abstraction layer. 
 */
typedef enum e_control_type
{
	E_TEXT_BOX, /**< A control that renders the text in the MMI. */
	E_INPUT_BOX, /**< A control that accepts input from the user. */
	E_BUTTON, /**< A control which allows the user to turn either on or off by selecting it. This control is called as push button or command button. */
	E_BUTTON_MULTILINE, /**< A control similar to the push/command button but can span over multiple lines depending upon the length of the text */
	E_CHECK_BOX, /**< A control in the MMI to enable or disable one or more options from a set.  */
	E_RADIO_BOX, /**< A control in the MMI to enable or disable one option from a set. The enabling of one radio box will disable the already enabled radio box in the given set. */
	E_LIST_BOX, /**< A control that allows the user to choose more than one options from the list. */
	E_COMBO_BOX, /**< A control that allows the user to choose one options from the list. */
	E_SCROLLBAR, /**< A control that visually indicates that there is more data that is yet to be seen outside the visible area */
	E_IMAGE, /**< A control that allows creating an image. */
	E_VIDEO, /**< A control that specifies a video type */
	E_PICTOGRAM, /**< A control that allows creating a pictogram. */
	E_IMAGE_TEXT, /**< A control that allows creating an image with text. */
	E_ICON, /**< A control that allows creating an icon. */
	E_RECT, /**< A control that allows creating a rect. */
	E_POLY, /**< A control that allows creating a polygon. */
	E_OBJECT, /**< A control that is common for image, audio, video and any other unknown content type. */
	E_OBJECT_TEXT,  /**< A control that allows creating an object alternate with text */
	E_CURSOR, /**<A control that allows to create a cursor. */
	E_THUMBNAIL_CURSOR /**<A control that allows to create a thumbnail cursor. */
} EControlType, E_CONTROL_TYPE ;

/**
 * @brief This enumeration contains the style of rendering the content. It can be either left to right 
 * or right to left rendering. 
 */
typedef enum e_rendering_style
{
	E_LEFT_RIGHT, /**< Specifies the rendering style of the control, which starts from the left */
	E_RIGHT_LEFT, /**< Specifies the rendering style of the control, which starts from the right */
	E_DEFAULT_DIR /**< Specifies the default rendering style of the control. */
} ERenderingStyle, E_RENDERING_STYLE ;

/**
 * @brief This enumeration contains the different text style that can be created using the 
 * jdd_MMICreateControl for the text box (E_TEXT_BOX) controls.
 * 
 * Multi-line, Marquee and Blink are the styles that have been applied to the text control. 
 * If the environment does not support these styles at least it should display the text in the
 * normal style without the additional properties. 
 */
typedef enum e_text_style
{
	E_TEXT_NORMAL, /**< Specifies the normal rendering of the text. */
	E_TEXT_MULTILINE, /**< Specifies the multi-line rendering of the text, the text control should create the control for the specified width and it should wrap the content accordingly. */
	E_TEXT_MARQUEE, /**< Specifies the marquee text control, where the text should marquee as per the direction specified. */
	E_TEXT_BLINK, /**< Specifies the blink text control, where the text should blink */
	E_TEXT_JUSTIFICATION  /**< Specifies the normal text control, where the text should justified to the width specified */
} ETextStyle, E_TEXT_STYLE ;

#ifdef JCF_OPTIMIZE_ENUM
typedef JC_INT8			E_INPUTBOX_STYLE ;
typedef JC_INT8			EInputBoxStyle ;
#define E_INPUT_NORMAL			0
#define E_INPUT_MASK			1
#define E_INPUT_MULTILINE		2 
#else
/**
 * @brief This enumeration contains the different input style that can be created using the jdd_MMICreateControl for the input box controls. 
 */
typedef enum e_input_style
{
	E_INPUT_NORMAL, /**< Specifies the normal input box (edit box) */
	E_INPUT_MASK, /**< Specifies the password input box, where the character that are keyed in shall be masked. */
	E_INPUT_MULTILINE /**< Specifies the input box with the multiple line. */
} EInputBoxStyle, E_INPUTBOX_STYLE ;
#endif

#ifdef JCF_OPTIMIZE_ENUM
typedef JC_INT8			E_INPUT_TRANSFORM ;

#define E_INPUT_TRANSFORM_NONE				0
#define E_INPUT_TRANSFORM_CAPITALIZE		1
#define E_INPUT_TRANSFORM_UPPERCASE			2
#define E_INPUT_TRANSFORM_LOWERCASE			3

#else
/**
 * @brief This enumeration contains the different input text transformation that are associated with the input box.
 */
typedef enum e_input_transform
{
	E_INPUT_TRANSFORM_NONE,
	E_INPUT_TRANSFORM_CAPITALIZE,
	E_INPUT_TRANSFORM_UPPERCASE,
	E_INPUT_TRANSFORM_LOWERCASE
} E_INPUT_TRANSFORM ;
#endif
/**
 * @brief This enumeration contains the state of the check box or radio box.
 */
typedef enum e_optionbox_status
{
	E_UNCHECKED, /**< Specifies the value for the check box or radio box when the control is not selected. */
	E_CHECKED /**< Specifies the value for the check box or radio box when the control is selected. */
} E_OPTIONBOX_STATUS ;

/**
 * @brief This enumeration contains the style of selection of the list box properties.
 */
typedef enum e_list_style
{
	E_SINGLE_SELECTION = 0 , /**< Specifies the list box control property for allowing selection of only one item at a time. */
	E_MULTIPLE_SELECTION /**< Specifies the list box control property to select multiple items at a time. */
} E_LIST_STYLE ;

/**
 * @brief This enumeration contains style property for the marquee text control.
 */
typedef enum e_marquee_style
{
	E_MARQUEE_NONE, /**< Specifies that the marquee is not present. */
	E_MARQUEE_SLIDE, /**< Specifies the marquee need to scroll only once. */
	E_MARQUEE_ALTERNATE, /**< Specifies the marquee need to scroll in alternate direction. */
	E_MARQUEE_SCROLL, /**< Specifies the marquee need to scroll in the direction specified. */
	E_MARQUEE_SCROLL_ONFOCUS, /**< Specifies the marquee needs to be applied only if the control is in focus. */
	E_MARQUEE_ALTERNATE_ONFOCUS /**< Specifies the marquee needs to be applied only if the control is in focus and the marquee style should be alternate scrolling. */
} EMarqueeStyle, E_MARQUEE_STYLE ;

/**
 * @brief This enumeration contains style of marquee property for the marquee control.	
 */
typedef enum e_marquee_direction
{
	E_MARQUEE_RIGHT_TO_LEFT, /**< Specifies the marquee scrolling direction from left towards the right side. */
	E_MARQUEE_LEFT_TO_RIGHT, /**< Specifies the marquee scrolling direction from right side towards the left side. */
	E_MARQUEE_UP_DOWN, /**< Specifies the marquee scrolling direction from up side towards the down side */
	E_MARQUEE_DOWN_UP /**< Specifies the marquee scrolling direction from down side towards the up side */
} EMarqueeDirection, E_MARQUEE_DIRECTION ;

/**
 * @brief This enumeration contains speed of marquee property for the marquee control.
 */
typedef enum e_marquee_speed
{
	E_MARQUEE_SLOW, /**< Specifies the slow speed in the marquee control. */
	E_MARQUEE_NORMAL, /**< Specifies the normal speed in the marquee control. */
	E_MARQUEE_FAST /**< Specifies the fast speed in the marquee control. */
} EMarqueeSpeed, E_MARQUEE_SPEED ;

/**
 * @brief This enumeration contains different type of the scrollbar that can be created. 
 */
typedef enum e_scrollbar_type
{
	E_VERTICAL_SCROLLBAR, /**< Specifies the vertical scrollbar properties. */
	E_HORIZONTAL_SCROLLBAR /**< Specifies the horizontal scrollbar properties. */
} EScrollBarType, E_SCROLLBAR_TYPE ;

/**
 * @brief This enumeration contains different style property for setting the wallpaper image. 
 */
typedef enum e_wallpaper_image
{
	E_POSITION_POINT, /**< Specifies the wallpaper property for the image, which needs to be rendered on the specified location. */
	E_POSITION_CENTER, /**< Specifies the wallpaper property for the image, which needs to be rendered in the center of the window for the exact width and height of the given image. */
	E_POSITION_STRETCH, /**< Specifies the wallpaper property for the image, which needs to be rendered for the full screen of the window, the image need to be stretched/resized if the actual width and height of the image is not equal to the window's width and height. */
	E_POSITION_TILE, /**< Specifies the wallpaper property for the image which needs to be tiled across vertical and horizontal for the full width and height of the window. */
	E_POSITION_VERTICAL_TILE, /**< Specifies the wallpaper property for the image which needs to be tiled across vertical for the full height of the window. */
	E_POSITION_HORIZONTAL_TILE /**< Specifies the wallpaper property for the image which needs to be tiled across horizontal for the full width of the window. */
} EWallpaperProperty, E_WALLPAPER_PROPERTY ;

/**
 * @brief This enumeration describes the different event that are generated by the MMI layer.
 */
typedef enum e_mmi_event_type
{
	JC_MMI_LINK_EVENT, /**< Whenever the link is used, the MMI need to generate this event. */
	JC_MMI_INPUT_EVENT, /**< Any key event in the E_INPUT_BOX, the MMI need to generate this event. */
	JC_MMI_BUTTON_EVENT, /**< Any action event for the E_BUTTON, the MMI needs to generate this event. */
	JC_MMI_CHECK_EVENT, /**< When there is any change in the state of the check box, the MMI need to generate this event. */
	JC_MMI_RADIO_EVENT, /**< When there is any change in the state of the radio box, the MMI need to generate this event. */
	JC_MMI_LIST_EVENT, /**< When any of the list box item is selected or de-selected, the MMI need to generate this event. */
	JC_MMI_COMBO_EVENT, /**< When any of the combo box item is selected, the MI need to generate this event. */
	JC_MMI_IMAGE_EVENT, /**< Any action event in the E_IMAGE, the MMI needs to generate this event. */
	JC_MMI_ACCESSKEY_EVENT, /**< Whenever an access key is entered, the MMI needs to generate this event. */
	JC_MMI_SOFTKEY_EVENT /**< When there is action key event on the soft key on the device, this specific event needs to be generated by the MMI layer. */
} EMMIEventType, E_MMI_EVENT_TYPE ;

/**
 * @brief This enumeration contains different style property for setting the different message type. 
 */
typedef enum e_mmi_notification
{
	E_NOTIFICATION_ERR /**< Specifies the message box needs to be rendered as an error message. */
} EMMINotificationMsgType, E_MMI_NOTIFICATION_MSGTYPE ;

/**
 * @brief This enumeration contains different style property for setting the different confirmation message type. 
 */
typedef enum e_mmi_confirmatation
{
	E_CONFIRMATION_OK_CANCEL, /**< Specifies the confirmation box needs to provide two options such as "Ok" and "Cancel". */
	E_CONFIRMATION_YES_NO /**< Specifies the confirmation box needs to provide two options such as "Yes" and "No". */
} EMMIConfirmationMsgType, E_MMI_CONFIRMATION_MSGTYPE ;

/**
 * @brief This enumeration contains the different border style.
 */
typedef enum e_border_style
{
    E_NO_BORDER, /**< Specifies the border style where the border no needs to be drawn. */
    E_SOLID_BORDER, /**< Specifies the solid border style where the border needs to be continuously drawn. */
    E_DOTTED_BORDER, /**< Specifies the dotted border style where the border needs to be drawn continuously. */
    E_DASHED_BORDER, /**< Specifies dashed border. Can be Rendered as solid. */
    E_DOUBLE_BORDER, /**< Specifies two borders. The width of the two borders are the same as the border-width value. */
    E_GROOVE_BORDER, /**< Specifies a grooved border. The effect depends on the border-color value. */
    E_RIDGE_BORDER, /**< Specifies a ridge border. The effect depends on the border-color value. */
    E_INSET_BORDER, /**< Specifies a recessed border. The effect depends on the border-color value. */
    E_OUTSET_BORDER /**< Specifies a raised border. The effect depends on the border-color value. */
} E_BORDER_STYLE ;
 
/**
 * @brief This enumeration contains the different icon type.
 */
typedef enum e_icon_type
{
	E_DISC, /**< Specifies the icon type is disc. */
	E_CIRCLE, /**< Specifies the icon type is circle. */
	E_SQUARE /**< Specifies the icon type is square. */
} EIconType, E_ICON_TYPE ;

#ifdef JCF_OPTIMIZE_ENUM
typedef JC_UINT8	E_OBJECT_MIME_TYPE ;
#define E_OBJECT_IMAGE				0
#define E_OBJECT_AUDIO				1
#define E_OBJECT_VIDEO				2
#define E_OBJECT_OTHER				3
#else
/**
 * @brief This enumeration contains different object type that can be specified when creating the object control. 
 */
typedef enum e_object_mime_type
{
	E_OBJECT_IMAGE, /**< Specifies the object type is image. */
	E_OBJECT_AUDIO, /**< Specifies the object type is audio. */
	E_OBJECT_VIDEO, /**< Specifies the object type is video. */
	E_OBJECT_OTHER /**< Specifies the unknown object type. */
} E_OBJECT_MIME_TYPE ;
#endif


#ifdef JCF_OPTIMIZE_ENUM
#define E_FONT_NORMAL	0 
#define E_FONT_ITALIC	1
#define E_FONT_OBLIQUE	2

typedef JC_UINT8	E_FONT_STYLE ;
#else
/**
 * @brief This enumeration describes the different font style that can be applied when creating the control. 
 *
 * If any of the font styles are not supported, the best effort rendering 
 * should be supported by the platform.
 */
typedef enum e_mmi_font_style
{
	E_FONT_NORMAL, /**< Specifies the normal font style. */
	E_FONT_ITALIC, /**< Specifies the italic font style. */
	E_FONT_OBLIQUE /**< Specifies the oblique font style. */
} E_FONT_STYLE ;

#endif

/**
 * @brief This enumeration describes the different action associated with the object control. 
 * 
 * Not all the object type need to implement, only applicable object like audio and video may need
 * to implement these object action. 
 */
typedef enum e_object_action
{
	E_OBJECT_INSTANTIATE, /**< Specifies that object is instantiated; 
						       applicable only where object is only declared during creating */
	E_OBJECT_PLAY, /**< Specifies that the action has to be performed to play the object content. */
	E_OBJECT_PAUSE, /**< Specifies that the action has to be paused */
	E_OBJECT_RESUME, /**< Specifies that the action has to be resumed. */
	E_OBJECT_STOP /**< Specifies that the action has to be stopped. */
} E_OBJECT_ACTION ;

#ifdef JCF_OPTIMIZE_ENUM
#define E_DECN_NONE				0
#define E_DECN_UNDERLINE		1
#define E_DECN_OVERLINE			2
#define E_DECN_LINE_THROUGH		3

typedef JC_UINT8		E_TEXT_DECORATION ;

#else
/**
 * @brief This enumeration describes the different text decoration that can be applied.
 */
typedef enum e_mmi_test_decoration
{
	E_DECN_NONE, /**< Specifies the empty text decoration. */
	E_DECN_UNDERLINE, /**< Specifies the underline text decoration, where the font needs to created with the underlined text, when this property is specified. */
	E_DECN_OVERLINE, /**< Specifies the overline text decoration. */
	E_DECN_LINE_THROUGH /**< Specifies the linethrough text decoration. */
} E_TEXT_DECORATION ;
#endif

/**
 * @brief This enumeration describes the different type of cursors.
 */
typedef enum e_cursor_type
{
	E_CURSOR_NORMAL, /**< Specifies the normal cursor type. */
	E_CURSOR_ACTION,  /**< Specifies the action cursor type when focused on the selectable controls. */
	E_CURSOR_INPUT  /**< Specifies the action cursor type when focused on the input controls. */
} ECursorType, E_CURSOR_TYPE ;
/**
 * @brief This structure defines the style attribute that is specified when creating the control.
 *
 */
typedef struct st_jc_style
{
	JC_UINT32			uiFontSize ; /**< Specifies the size of the font which need to be applied for the style. */
	JC_INT32			iForegroundColor ; /**< Specifies the foreground color of the style, if it is less than zero then this value shall be ignored. */
	JC_INT32			iBackgroundColor ; /**< Specifies the background color of the style, if it is less than zero then this value shall be ignored. */
	JC_INT32			iFontID ; /**< Specifies the font ID, which has got from #jdd_MMIIsFontAvailable. */
	JC_UINT16			uhFontWeight ; /**< Specifies the font weight. */
	JC_UINT16			uhMagnification ; /**< Specifies the magnification factor. */
	E_FONT_STYLE		eFontStyle ; /**< Specifies the font style. */
	E_TEXT_DECORATION	eTextDecoration ; /**< Specifies the font decoration. */
#ifdef FUTURE_USAGE
	JC_UINT32			uiReserved ; /**< Reserved for future use. */
#endif
} JC_STYLE ;

/**
 * @brief This structure contains the text metrics information, where it is used to calculate the required metrics for displaying the specified text.
 */
typedef struct st_text_metrics
{
	JC_UINT32 uiWidth ; /**< Specifies the width of the number of characters that can be rendered. */
	JC_UINT32 uiHeight ; /**< Specifies the height (Ascent + Descent) of the number of characters that can be rendered. */
	JC_UINT32 uiDescent ; /**< Specifies the Descent (from the baseline) */
	JC_UINT32 uiNumCharacters ; /**< Specifies the number of characters that can be rendered for the specified width. */
} JC_TEXTMETRICS ;

/** 
 * @brief This structure defines the Cartesian co-ordinates of the point.
 */
typedef struct st_jc_point
{
	JC_UINT32 uiX ; /**< Specifies the x-coordinate of the point. */
	JC_UINT32 uiY ; /**< Specifies the y-coordinate of the point. */
} JC_POINT ;

/**
 * @brief This structure defines the co-ordinates of the upper-left, width and height of the rectangle. 
 */
typedef struct st_jc_rect
{
	JC_INT32 iLeft ; /**< Specifies the x-coordinate of the upper-left corner of the rectangle. */
	JC_INT32 iTop ; /**< Specifies the y-coordinate of the upper-left corner of the rectangle. */
	JC_UINT32 uiWidth ; /**< Specifies the width of the rectangle. */
	JC_UINT32 uiHeight ; /**< Specifies the height of the rectangle. */
} JC_RECT ;

/**
 * @brief This structure defines the co-ordinates and the number of points, required for creating the polygon control. 
 */
typedef struct st_jc_poly
{
	JC_INT32	iPoints ; /**< Specifies the number of points required for the creation of the polygon control. */
	JC_POINT	*pstPoint ; /**< Specifies the cartesian coordinates of the points required to create the polygon control. */
} JC_POLY ;

/**
 * @brief This structure contains the image information.
 */
typedef struct st_image_info
{
	JC_UINT8 *pImgBuffer ; /**< Specifies the content of the image. */
	JC_UINT32 uiImgSize ; /**< Specifies the size of the image content. */
	JC_INT8 *psMimeType ; /**< Specifies the mime type of the image content, in case of NULL, the device needs to parse the image content to find the mime type. */
	EWallpaperProperty eProperty ; /**< Specifies the wall paper property. */
} JC_IMAGE_INFO ;

/**
 * @brief This structure contains the video control information. 
 *
 * @deprecated This structure is deprecated and recommended to use JC_OBJECT_INFO. 
 */
typedef struct st_video_info
{
	JC_UINT8 *pVideoBuffer ; /**< Specifies the content of the video. */
	JC_UINT32 uiVideoSize ; /**< Specifies the size of the video content. */
	JC_INT8 *psMimeType ; /**< Specifies the content type of the video. */ 
} JC_VIDEOCONTROL_INFO ;

/**
 * @brief Contains the object data either as an memory reference or persistent storage reference.
 */ 
typedef union u_object_buffer
{
	JC_UINT8		*pucBuffer ; /**< Specifies the memory reference to the object content. */
	JC_CHAR			*pmFileName ; /**< Specifies the file reference to the object content. */
} U_OBJECT_BUFFER ;

/**
 * @brief This structure contains the information about the object control. 
 */
typedef struct st_object_info
{
	E_OBJECT_MIME_TYPE	eType ; /**< Specifies the object mime type. */
	JC_BOOLEAN			bHasMoreData ; /**< Specifies more data flag */
	JC_BOOLEAN			bIsFileContent ; /**< Specifies the object content available in memory or file.*/
	U_OBJECT_BUFFER		uBuffer ; /**< Specifies the mode of content (memory/file). */
	JC_CHAR				*pmClassId ; /**< Specifies the value of the classid attribute of the object.*/	
	JC_UINT32			uiObjectLen ; /**< Specifies the size of the object. */
	JC_UINT32			uiTotalLen ; /**< Specifies the total length of the object */
	JC_INT8				*psMimeType ; /**< Specifies the mime type of the object. */
	JC_UINT32			uiPrevCtrlId ; /**< Specifies the previous control ID unlinked from the page **/
	JC_HANDLE			vHandle ; /**< Specifies the handle associated with the object. Depending upon the application nature, this should be applied. */
} JC_OBJECT_INFO ;

/**
 * @brief This structure contains the information about the object classid. 
 */
typedef struct st_object_class
{
	JC_CHAR		*pmClassId ; /**< Specifies the value of the classid attribute of the object. */
	JC_CHAR		*pmURI ; /**< Holds the URL of the object to be downloaded. This has to be filled and send to the calling function */
	JC_HANDLE	vHandle ; /**< Specifies the object element infomation. */
} JC_OBJECT_CLASS ;

/**
 * @brief This structure contains the information about more object control
 */
typedef struct st_object_more
{
	JC_UINT8			*pucBuffer ; /**< Specifies the reference to the buffer */
	JC_UINT32			uiBufLen ;  /**< Specifies the current buffer length */
	JC_BOOLEAN			bHasMoreData ; /**< Specifies the  more data flag */
	JC_RETCODE			rCode ; /**< Specifies the error code if any */
} JC_OBJECT_MORE ;


/**
 * @brief This structure contains the information about the object text control . 
 */
typedef struct st_object_text
{
	JC_CHAR			*pmURI ;
	JC_CHAR			*pmType ;
} JC_OBJECT_TEXT ;

/**
 * @brief This structure contains the pictogram information.
 */
typedef struct st_pictogram_info
{
	JC_CHAR		*pmURL ; /**< Specifies the pictogram URL. */
	JC_UINT16   uhCode ; /**< Specifies the pictograph unicode value. This shall be used when the pictogram URL is NULL. */
} JC_PICTOGRAM_INFO ;

/**
 * @brief This structure contains the information about the icon information.
 */
typedef struct st_icon_info
{
	EIconType eType ; /**< Specifies the icon type. */
} JC_ICON_INFO ;

/**
 * @brief This structure contains the marquee text control properties.
 */
typedef struct st_marquee
{
	EMarqueeStyle eStyle ; /**< Specifies the marquee style. If the marquee style is E_MARQUEE_ALTERNATE, the alignment should be based on the number of iteration. The text should be aligned where the iteration stops irrespective of the eDirection. */
	JC_INT32 iCount ; /**< Specifies the number of times the marquee needs to be rendered. If the value is zero, then the marquee should be treated as normal label and as per the direction the text needs to be aligned. If the value is -1, then the marquee needs to be rendered continuously. If the direction is left to right, then the text should right aligned when the marquee stops. If the direction is right to left, then the text should be left aligned when the marquee stops. For on focus style, when the control gets the focus the marquee needs to be repeated. */
	EMarqueeDirection eDirection ; /**< Specifies the direction for the marquee. */
	EMarqueeSpeed eSpeed ; /**< Specifies the speed for the marquee control. */
	JC_UINT32 uiBorder ; /**< Specifies the border size for the marquee control, in case of zero, the marquee control shall not have any border. */
} JC_MARQUEE ;

/**
 * @brief This structure contains the blink text control properties.
 */
typedef struct st_blink
{
	JC_UINT32 uiSpeed ; /**< Specifies the speed for the blink control. */
} JC_BLINK ;

/**
 * @brief This structure contains the text control properties.
 */
typedef struct st_text
{
	ETextStyle	etextStyle ; /**< Specifies the text style for the text control. */
	JC_MARQUEE	marqueeAttribute ; /**< Specifies the marquee attribute if the text style is of type Marquee control. */
	JC_BLINK	blinkAttribute ; /**< Specifies the blink attribute if the text style is of type Blink control. */
	JC_UINT32	uiTextLen ;
} JC_TEXT_BOX ;

typedef struct st_input_box_value
{
	JC_CHAR						*pmValue ; /**< Specifies the input text. */
	struct st_input_box_value	*pstNext ; /** < Specifies the next element in the list. */
} JC_INPUT_BOX_VALUE ;

/**
 * @brief This structure contains the input box properties.
 */
typedef struct st_input_box
{
	E_INPUTBOX_STYLE	einputBoxStyle ; /**< Specifies the input style for the input box control. */
	E_INPUT_TRANSFORM	eInputTransform ; /**< Specifies the input transform for the text entries. */
	JC_BOOLEAN			bIsNotEditable ; /**< Specifies whether the input box is not editable. If it is E_TRUE, it should be displayed as disabled.*/
	JC_BOOLEAN			bIsNotEmpty ; /**< Specifies whether the input box can be empty or not. */
	JC_UINT32			uiRows ; /**< Specifies the number of rows if the input style is multiline input box. */
	JC_UINT32			uiColumns ; /**< Specifies the number of columns if the input style is multi-line input box. */
	JC_CHAR				*psInputFormat ; /**< Specifies the input format. */
	JC_CHAR				*psInputMode ;	/**< Specifies the input mode. */
	JC_INT32			iMaxLength ; /**< Specifies the maximum number of characters can be entered in the input box. */
	JC_CHAR				*pDefaultValue ; /**< Specifies the initial value that needs to be set to the control. */	
	JC_INPUT_BOX_VALUE	*pstValueList ;
} JC_INPUT_BOX ;

/**
 * @brief This structure contains the list box properties.
 */
typedef struct st_list_box
{
	JC_UINT32 uiNumberItems ; /**< Specifies the number of items in the list box. */
	JC_UINT32 uiOptgroupCount ; /**< Specifies the number of opt groups present in the list box. */
	JC_UINT32 uiVisibleRows ; /**< Specifies the no of visible rows in the list box. */
	E_LIST_STYLE listStyle ; /**< Specifies the style property for the list box. */
} JC_LIST_BOX ;

/**
 * @brief This structure contains the list box item properties.
 */
typedef struct st_list_box_item
{
	JC_UINT32		uiIndexItem ; /**< Specifies the index of the item in the opt group. */
	JC_CHAR			*psText ; /**< Specifies the text of the list item. */
	JC_UINT32		uiOptgroupId ; /**< Specifies the opt group id to which this item belongs. */
	JC_BOOLEAN		bIsEnabled ; /**< Specifies if the item is enabled or not. */
	JC_IMAGE_INFO	*pImageInfo ; /**< Specifies the image info if the item is an image. */
} JC_LIST_BOX_ITEM ;

/**
 * @brief This structure contains the radio box properties.
 */
typedef struct st_radio_box
{
	JC_UINT32 uiGroupID ; /**< Specifies the group identifier for the radio box. */
	E_OPTIONBOX_STATUS eStatus ; /**< Specifies the radio box status. */
} JC_RADIO_BOX ;

/**
 * @brief This structure contains the check box properties.
 */
typedef struct st_check_box
{
	E_OPTIONBOX_STATUS eStatus ; /**< Specifies the check box status. */
} JC_CHECK_BOX ;

/**
 * @brief This structure contains the combo box properties.
 */
typedef struct st_combo_box
{
	JC_UINT32 uiNumberItems ; /**< Specifies the number of items for the combo box. */
	JC_UINT32 uiOptgroupCount ; /**< Specifies the total number of opt groups. */
} JC_COMBO_BOX ;

/**
 * @brief This structure contains the combo box item properties.
 */
typedef struct st_combo_box_item
{
	JC_UINT32		uiIndexItem ; /**< Specifies the index of the item in the opt group. */
	JC_UINT32		uiOptgroupId ; /**< Specifies the opt group id to which this item belongs. */
	JC_BOOLEAN		bIsEnabled ; /**< Specifies if the item is enabled or not. */
	JC_CHAR			*psText ; /**< Specifies the text of the combo item. */
} JC_COMBO_BOX_ITEM ;

/**
 * @brief This structure contains the scrollbar properties.
 */
typedef struct st_scrollbar
{
	EScrollBarType eScrollBarType ; /**< Specifies the type of the scrollbar. */
} JC_SCROLL_BAR ;

/**
 * @brief This structure contains the border properties. 
 */
typedef struct st_border_props
{
    JC_UINT32 uiLeftBorderWidth ; /**< Specifies the width of the left side of the rectangle border. */
    JC_INT32 iLeftBorderColor ; /**< Specifies the color of the left side of the rectangle border. */
    E_BORDER_STYLE iLeftBorderStyle ; /**< Specifies the border style of the left side of the rectangle border. */
    JC_UINT32 uiRightBorderWidth ; /**< Specifies the width of the right side of the rectangle border. */
    JC_INT32 iRightBorderColor ; /**< Specifies the color of the right side of the rectangle border.*/
    E_BORDER_STYLE iRightBorderStyle ; /**< Specifies the border style of the right side of the rectangle border. */
    JC_UINT32 uiTopBorderWidth ; /**< Specifies the width of the top side of the rectangle border. */
    JC_INT32 iTopBorderColor ; /**< Specifies the color of the top side of the rectangle border. */
    E_BORDER_STYLE iTopBorderStyle ; /**< Specifies the border style of the top side of the rectangle border. */
    JC_UINT32 uiBottomBorderWidth ; /**< Specifies the width of the bottom side of the rectangle border. */
    JC_INT32 iBottomBorderColor ; /**< Specifies the color of the bottom side of the rectangle border. */
    E_BORDER_STYLE iBottomBorderStyle ; /**< Specifies the border style of the bottom side of the rectangle border. */
} JC_BORDER_PROPS ;

/**
 * @brief This structure contains the padding info. 
 */
typedef struct st_padding_info
{
 	JC_UINT32 			uiLeft ;
	JC_UINT32			uiRight ;
	JC_UINT32			uiTop ;
	JC_UINT32			uiBottom ;
} JC_PADDING_INFO ;


/**
 * @brief This structure contains the rectangle properties.
 */
typedef struct st_rect_info 
{
	JC_BORDER_PROPS		stBorderProp ; /**< Specifies the border properties for the rectangle. */
} JC_RECT_INFO ;

/**
 * @brief This structure contains the different cursor types
 */
typedef struct st_cursor_info 
{
	ECursorType		eType ; /**< Specifies the types of cursors to be displayed appropriately based on the type of the control. */ 
} JC_CURSOR_INFO ;

/** 
 * @brief This structure contains the wallpaper information for a window.
 */

typedef struct st_wallpaper_info
{
	JC_OBJECT_INFO		stObjectInfo ; /*Specifies the mime type, object content and size of the object.*/
	EWallpaperProperty	eProperty ; /*Specifies the background property for the object.*/
	JC_RECT				*pstRect ; /*Specifies the location where the object needs to be rendered.*/
	JC_UINT16			uhMagnification ; /**< Specifies the magnification factor. */
} JC_WALLPAPER_INFO ;


#ifdef JDD_RICH_CONTROL_ENABLE
/**
 * @brief This structure contains the rich control properties like padding, border, bgimage
 */
typedef struct st_rich_ctrl_property
{
	JC_BOOLEAN			bRichCtrl ; /** Specifies whether the rich control properties should be used or not*/
	JC_WALLPAPER_INFO	stBgImageInfo ; /**< Specifies the bgimage for the rectangle. */
	JC_BORDER_PROPS		*pstBorderProp ; /**< Specifies the border properties for the control. */
	JC_PADDING_INFO		*pstPaddingInfo ; /**< Specifies the padding information for the rectangle. */
} JC_RICH_CONTROL_PROPS ;
#endif /*JDD_RICH_CONTROL_ENABLE*/

/**
 * @brief This structure contains the generic control properties, which shall be used in the jdd_MMICreateControl function.
 */
typedef struct st_control_properties
{
	JC_UINT32				uiMasterWindowID ; /**< Specifies the ID of the master (main) window in which the control is created */
	JC_CHAR					*psCaptionValue ; /**< Specifies the caption for the specific control. */
	JC_CHAR					*pmLanguage ; /**< Specifies the language for the specific control. */
	JC_STYLE				controlStyle ; /**< Specifies the style attribute which needs to be applied for the control. */
	JC_BOOLEAN				bIsSelectable ; /**< Specifies whether the control is selectable while scrolling. */
	ERenderingStyle 		eRenderingStyle ; /**< Specifies the style of rendering. */
#ifdef JDD_RICH_CONTROL_ENABLE
	JC_RICH_CONTROL_PROPS	stRichCtrlProps ; /**< Specifies rich control properties. */
#endif
	JC_UINT32				eEventCode ; /**< Specifies the event code that can be used by the MMI to generate the event. If the value is zero, the MMI need to generate the default event value for the specific control. */
	JC_INT32				iMaxWidth ; /**< Specifies the maximum width that needs to be used by this control. */
	JC_BOOLEAN				bReturnDimension ; /**< Specifies whether only the dimension needs to be generated instead of creating the actual controls, when this flag is set to E_TRUE. */
	JC_BOOLEAN				bIsDisabled ; /**< Specifies whether the control needs to be disabled for selection. */
	union
	{
		JC_TEXT_BOX				textBox ; /**< Specifies the text box control properties. */
		JC_INPUT_BOX			inputBox ; /**< Specifies the input box control properties. */
		JC_LIST_BOX				listBox ; /**< Specifies the list box control properties. */
 		JC_RADIO_BOX			radioBox ; /**< Specifies the radio box control properties. */
		JC_CHECK_BOX			checkBox ; /**< Specifies the check box control properties. */
		JC_COMBO_BOX			comboBox ; /**< Specifies the combo box control properties. */
		JC_SCROLL_BAR			scrollBar ; /**< Specifies the scroll bar control properties. */
		JC_IMAGE_INFO			image ; /**< Specifies the image control properties. */
		JC_VIDEOCONTROL_INFO	video ; /**< Specifies the video control properties. */
		JC_OBJECT_INFO			object ; /**< Specifies the object control properties. */
		JC_OBJECT_TEXT			objectText ; /**< Specifies the object text control properties */
		JC_PICTOGRAM_INFO		pictogram ; /**< Specifies the pictogram control properties. */
		JC_ICON_INFO			icon ; /**< Specifies the icon control properties. */
		JC_RECT_INFO			rect ; /**< Specifies the rect control properties. */
		JC_CURSOR_INFO			cursor ; /**< Specifies the different cursor types. */
		JC_POLY					poly ; /**< Specifies the polygon control properties. */
	} typeOfControl ; /**< Specifies the individual control properties. */
	JC_UINT32 uiReserved ; /**< Reserved for future usage. */
} JC_CONTROL_PROPERTIES ;


/**
 * @brief This structure contains the property for the window style.
 */
typedef struct st_window_props
{
	EWindowStyle	eWindowStyle ; /**< Specifies the style of the window. */
	JC_CHAR 		*pszCaptionValue ; /**< Specifies title for the window. */
	JC_TASK_INFO	srcTaskInfo ; /**< Specifies the source task information. */
	JC_TASK_INFO	destTaskInfo ; /**< Specifies the destination task information. */
	JC_UINT32		uiMasterWindowID ; /**< Specifies the ID of the master (main) window in which the window is created.
										    If 0 it means that window being created is the main window*/
	JC_UINT32		uiParentID ; /**< Specifies the parent window id. */
	JC_BORDER_PROPS	stBorderProp ; /**< Specifies the border properties of the window. */
	void			*pApplicationData ; /**< Reserved for future usage. */
	JC_UINT32		uiResourceID ; /**< Specifies the resource id. */
	JC_UINT32 		uiReserved ; /**< Reserved for future usage. */
} JC_WINDOW_PROPERTIES ;

/**
 * @brief This structure contains the scrolling information. 
 */
typedef struct st_scroll_info
{
	JC_UINT32 uiMinPos ; /**< Specifies the minimum position of the scrollbar. */
	JC_UINT32 uiMaxPos ; /**< Specifies the maximum position of the scrollbar. */
	JC_UINT32 uiPageSize ; /**< Specifies the maximum size of the page. */
	JC_UINT32 uiCurrPos ; /**< Specifies the current position. */
	JC_BOOLEAN bIsScrollDown ; /**< Specifies whether the scroll down is possible or not. */
	JC_BOOLEAN bIsScrollUp ; /**< Specifies whether the scroll up is possible or not. */
} SCROLL_INFO ;

/**
 * @brief This structure contains the device characteristics information. 
 */
typedef struct st_device_characteristics
{
	JC_UINT32 uiActualWidth ; /**< Specifies the actual width of the device. */
	JC_UINT32 uiActualHeight ; /**< Specifies the actual height of the device. */
	JC_UINT32 uiRenderingWidth ; /**< Specifies the width which needs to be used for rendering. */
	JC_UINT32 uiRenderingHeight ; /**< Specifies the height which needs to be used for rendering. */
	JC_UINT32 uiScrollBarWidth ; /*< Specifies the scrollbar width. */
	JC_UINT32 uiScrollBarHeight ; /**< Specifies the scrollbar height. */
	JC_UINT32 uiLeftMargin ; /**< Specifies the left margin. */
	JC_UINT32 uiTopMargin ; /**< Specifies the top margin. */
	JC_UINT32 uiWindowX ; /**< Specifies the x axis. */
	JC_UINT32 uiWindowY ; /**< Specifies the y axis. */
	JC_BOOLEAN bIsChildWindowSupported ; /**< Specifies whether platform support child window. */
	JC_BOOLEAN bIsHorizontalScrollSupported ; /**< Specifies whether platform support horizontal scroll bar. */
	JC_BOOLEAN bIsSoftKeySupported ; /**< Specifies whether the platform support any soft key label. */
} DeviceCharacteristics, DEVICE_CHARACTERISTICS ;

/**
 * @brief This structure defines the input event structure when there is any event that needs to be generated for the E_INPUT_BOX control type. 
 * It contains the key event type and the ASCII value of the key that is pressed. 
 */
typedef struct st_input_event
{
	JC_UINT32 uiKeyValue ; /**< Specifies the ascii value that is specified. */
} JC_INPUT_EVENT ;

/**
 * @brief This structure defines the check event structure when there is any event that needs to be generated for the E_CHECK_BOX control type. 
 * The check box control generates event when there is any change in the selection status.
 */
typedef struct st_check_event
{
	E_OPTIONBOX_STATUS eStatus ; /**< Specifies the E_CHECK_BOX status. */
} JC_CHECK_BOX_EVENT ;

/** 
 * @brief This structure defines the radio event structure when there is any event that needs to be generated for the E_RADIO_BOX control type. 
 * When there is any change in the selection of the radio box this control should generate this event. 
 */
typedef struct st_radio_event
{
	E_OPTIONBOX_STATUS eStatus ; /**< Specifies the E_RADIO_BOX status. */ 
} JC_RADIO_BOX_EVENT ;

/** 
 * @brief This structure defines the list box event structure when there is any event that needs to be generated for the E_LIST_BOX control type. 
 * If any of the list box item is selected/deselected this event need to be generated with the specific index that has been modified. 
 */
typedef struct st_list_event
{
	JC_UINT32 uiIndex ; /**< Specifies the index in the list box, which got changed. */
	JC_BOOLEAN bIsSelected ; /**< Specifies whether the index has been selected or not. */
} JC_LIST_BOX_EVENT ;

/** 
 * @brief This structure defines the combo box event structure when there is any event that needs to be generated for the E_COMBO_BOX control type. 
 * If there is any change in the combo box state the control generates the event with the newly selected index and the previously selected index. 
 */
typedef struct st_combobox_event
{
	JC_UINT32 uiSelectedIndex ; /**< Specifies the index, which has been selected in the combo box. */
	JC_UINT32 uiDeSelectedIndex ; /**< Specifies the index, which has been deselected because of this combo box. */
} JC_COMBO_BOX_EVENT ;

/** 
 * @brief This structure defines the soft key event structure when the user has selected the soft key. 
 * The particular soft key index needs to be passed on to the application.
 */
typedef struct st_softkey_event 
{
	JC_UINT32 uiSoftKeyIndex ; /**< Specifies the soft key index.*/
} JC_SOFT_KEY_EVENT ;

/** 
 * @brief This structure defines the access key event.
 */
typedef struct st_accesskey_event 
{
	JC_UINT32 uiAccessKeyIndex ; /**< Specifies the access key index. */
} JC_ACCESS_KEY_EVENT ;

/** 
 * @brief This is the generic event structure that shall be generated by the MMI layer to the application layer for any event in the MMI.
 */
typedef struct st_mmi_event
{
	JC_UINT32 uiWindowID ; /**< Specifies the window's identifier. */
	JC_UINT32 uiControlID ; /**< Specifies the control identifier. */
	EMMIEventType eEventType ; /**< Specifies the event type. */
	union
	{
		JC_INPUT_EVENT 		inputEvent ; /**< Specifies the input event. */
		JC_CHECK_BOX_EVENT 	checkBoxEvent ; /**< Specifies the check box event. */
		JC_RADIO_BOX_EVENT 	radioBoxEvent ; /**< Specifies the radio box event. */
		JC_LIST_BOX_EVENT 	listBoxEvent ; /**< Specifies the list box event. */
		JC_COMBO_BOX_EVENT 	comboBoxEvent ; /**< Specifies the combox box event. */
		JC_SOFT_KEY_EVENT 	softKeyEvent ; /**< Specifies the soft key event. */
		JC_ACCESS_KEY_EVENT	accessKeyEvent ; /**< Specifies the access key event. */
	} eventData ; /**< Specifies the event data. */
} JC_MMI_EVENT ;


/**
 * @brief This enumeration describes the different key action.
 */
typedef enum e_key_action
{
	E_KEY_DOWN, /**< Specifies the key down action. */
	E_KEY_PRESS, /**< Specifies the key press action. */
	E_KEY_UP /**< Specifies the key up action. */	
} E_KEY_ACTION ;


/** 
 * @brief This structure defines the key event. 
 */
typedef struct st_key_event
{
	JC_INT32		iKeyCode ; /**< Specifies Key code in keyUp/keyDown events and zero in case of keyPress event. */
	JC_INT8			cCharCode ; /**< Specifies ASCII Key code in keyPress event and zero in case of keyUp/keyDown events. */
	E_KEY_ACTION	eKeyAction ; /**< Specifies the different key actions. */
} JC_KEY_EVENT ;

/**
 * @brief This enumeration describes the different stylus action.
 */
typedef enum e_stylus_action
{
	E_STYLUS_TAP, /**< Specifies the stylus tap. */
	E_STYLUS_DBL_TAP /**< Specifies the double tap. */
} E_STYLE_ACTION ;

/** 
 * @brief This structure defines the stylus event. 
 */
typedef struct st_stylus_event
{
	JC_UINT32			uiWindowID ; /**< Specifies the window identifier, where the stylus or mouse has been activated. */
	JC_UINT32			uiX ; /**< Specifies the x axis. */
	JC_UINT32			uiY ; /**< Specifies the y axis. */
	E_STYLE_ACTION		eStylusAction ;
} JC_STYLUS_EVENT ;


/**
 * @brief This enumeration describes the different mouse actions.
 */
typedef enum e_mouse_action
{
	E_MOUSE_CLICK, /**< Specifies the mouse click. */
	E_MOUSE_DBL_CLICK, /**< Specifies the mouse double click. */
	E_MOUSE_DOWN, /**< Specifies the mouse down. */
	E_MOUSE_UP, /**< Specifies the mouse up. */
	E_MOUSE_MOVE /**< Specifies the mouse move. */
} E_MOUSE_ACTION ;

/** 
 * @brief This structure defines the mouse event. 
 */

typedef struct st_mouse_event
{
	JC_UINT32			uiWindowID ; /**< Specifies the window identifier, where the stylus or mouse has been activated. */
	JC_UINT32			uiX ; /**< Specifies the x axis. */
	JC_UINT32			uiY ; /**< Specifies the y axis. */
	JC_BOOLEAN			bAltPressed ; /**< This flag specifies whether ALT key is pressed or not during the mouse action.*/
	JC_BOOLEAN			bCtrlPressed ;/**< This flag specifies whether CONTROL key is pressed or not during the mouse action.*/
	JC_BOOLEAN			bShiftPressed ;/**< This flag specifies whether SHIFT key is pressed or not during the mouse action.*/
	JC_INT8				cButton ; /**< Specifies which mouse button is pressed. 0 for left, 1 for middle and 2 for right.*/
	E_MOUSE_ACTION		eAction ; /**< Specifies the mouse action. */
} JC_MOUSE_EVENT ;

/** 
 * @brief When the computation of the control dimension is asynchronous then the application needs to post
 * the event with this structure.
 */
typedef struct st_ctrl_dimension
{
	JC_RETCODE			rCode ; /**< Specifies the status code for the computation of the control dimension. */
	JC_HANDLE			vObjectHandle ; /**< Specifies the reference that has been passed when creating the control. */
	JC_UINT32			uiWidth ; /**< Specifies the width of the control. */
	JC_UINT32			uiHeight ; /**< Specifies the height of the control. */
} JC_CONTROL_DIMENSION ;

/** 
 * @brief This callback function is registered with the #jdd_MMIConfirmationMessage. 
 * This function is called by the JDD layer when the confirmation message is implemented as a non-blocking call.
 * @param[in] eConfirmationStatus Specifies the confirmation status. 
 * @param[in] pvArg Specifies the argument that is passed when the confirmation message function is invoked. 
 */
typedef void (*JDD_MMI_CONFIRMATION_CB) (JC_BOOLEAN eConfirmationStatus, void *pvArg) ;

/** 
 * @brief This callback function is registered with the #jdd_MMINotificationMessage.
 * This function is called by the JDD layer when the notification message is implemented as a non-blocking call.
 * @param[in] pvArg Specifies the argument that is passed when the information message is invoked. 
 */
typedef void (*JDD_MMI_INFORMATION_CB) (void *pvArg) ;

/** 
 * @brief This specifies different enumeration that can be generated by the JDD layer.
 * These events vary from different to different. 
 */
typedef enum e_sec_ddl_event_type_
{
	E_F1_EVENT = 0x01, /**< This event occurs when "F1" key is pressed. */

	E_F2_EVENT, /**< This event occurs when "F2" key is pressed. */
	E_F3_EVENT, /**< This event occurs when "F3" key is pressed. */
	E_F4_EVENT, /**< This event occurs when "F4" key is pressed. */
	E_F6_EVENT, /**< This event occurs when "F6" key is pressed. */
	E_F7_EVENT, /**< This event occurs when "F7" key is pressed. */
	E_F8_EVENT, /**< This event occurs when "F8" key is pressed. */
	E_F9_EVENT, /**< This event occurs when "F9" key is pressed. */

	E_PRIMARY_MMI_EVENT, /**< This event is a "primary MMI" event. */
	E_F11_EVENT, /**< This event occurs when "F11" key is pressed. */

	E_VK_UP_EVENT, /**< This event occurs when the "up arrow" key is pressed. */
	E_VK_DOWN_EVENT, /**< This event occurs when the "down arrow" key is pressed. */
	E_VK_LEFT_EVENT, /**< This event occurs when the "left" key is pressed. */
	E_VK_RIGHT_EVENT, /**< This event occurs when the "right" key is pressed. */
	E_VK_PRIOR_EVENT, /**< This event occurs when the "page up" key is pressed. */
	E_VK_NEXT_EVENT, /**< This event occurs when the "page down" key is pressed. */
	E_VK_HOME_EVENT, /**< This event occurs when the "home" key is pressed. */
	E_VK_END_EVENT, /**< This event occurs when the "end" key is pressed. */
	E_VK_RETURN_EVENT , /**< This event occurs when the "enter" key is pressed. */
	E_VK_NUM0_EVENT, /**< This event occurs when the key '0' is pressed. */
	E_VK_NUM1_EVENT, /**< This event occurs when the key '1' is pressed. */
	E_VK_NUM2_EVENT, /**< This event occurs when the key '2' is pressed. */
	E_VK_NUM3_EVENT, /**< This event occurs when the key '3' is pressed. */
	E_VK_NUM4_EVENT, /**< This event occurs when the key '4' is pressed. */
	E_VK_NUM5_EVENT, /**< This event occurs when the key '5' is pressed. */
	E_VK_NUM6_EVENT, /**< This event occurs when the key '6' is pressed. */
	E_VK_NUM7_EVENT, /**< This event occurs when the key '7' is pressed. */
	E_VK_NUM8_EVENT, /**< This event occurs when the key '8' is pressed. */
	E_VK_NUM9_EVENT, /**< This event occurs when the key '9' is pressed. */
	E_VK_HASH_EVENT, /**< This event occurs when the key '#' is pressed. */
	E_VK_STAR_EVENT, /**< This event occurs when the key '*' is pressed. */
	
	E_STYLUS_EVENT, /**< This event occurs when an action is performed with the stylus. */
	E_MOUSE_EVENT, /**< This event occurs when an action is performed with the mouse. */
	E_KEY_EVENT,	/**< This event occurs when an action is performed with the key. */
	E_CTRL_ASYNC_EVENT, /**< This event occurs when an control async event is performed. */

	E_F12_EVENT, /**< This event occurs when "F12" key is pressed. */

	E_SHIFT_F3_EVENT, /**< This event occurs when the "shift" key is pressed along with the "F3" key. */
	E_SHIFT_F4_EVENT, /**< This event occurs when the "shift" key is pressed along with the "F4" key. */
	E_SHIFT_F6_EVENT, /**< This event occurs when the "shift" key is pressed along with the "F6" key. */

	E_CBN_SELCHANGE_EVENT, /**< This event is generated when change event occurs. */
	E_DISPLAY_REFRESH_EVENT, /**< This event occurs when a window is refreshed. */
	E_RESIZE_EVENT, /**< This event occurs when a window is resized. */
	E_CLOSE_EVENT, /**< This event occurs when a window is closed. */
	E_INPUT_SELECT_EVENT, /**< This event occurs when a part or full text in the input box get selected.*/
	E_CHANGE_EVENT, /**< This event occurs when the selection of an option get changes in a list/combo box.*/
	E_SHUTDOWN_EVENT, /**< This event occurs when the application is closed. */

	E_TOTAL_EVENTS /**< Not used for events. */

} ESecondaryDDLEventType, E_SECONDARY_DDL_EVENTTYPE ;

#endif

/* END OF FILE */
