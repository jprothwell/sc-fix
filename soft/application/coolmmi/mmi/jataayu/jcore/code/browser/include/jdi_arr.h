/***************************************************************************
 * $Id: jdi_arr.h,v 1.29 2009/04/16 05:03:40 kavithadevhtmlbrow Exp $
 * $Revision: 1.29 $
 * $Date: 2009/04/16 05:03:40 $
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
 * $Log: jdi_arr.h,v $
 * Revision 1.29  2009/04/16 05:03:40  kavithadevhtmlbrow
 * Overflow Page Instruction (E_ARR_OVERFLOW_PAGE_INST)
 *
 * Revision 1.28  2009/03/02 13:58:38  arpitadevhtmlbrow
 * Updated comments for ARR Rules
 *
 * Revision 1.27  2009/02/11 07:38:46  kavithadevhtmlbrow
 * New rule in image for Image Optimize
 *
 * Revision 1.26  2008/11/18 10:22:49  arpitadevhtmlbrow
 * Fixed Postion ARR addition
 *
 * Revision 1.25  2008/10/07 10:35:33  shajudevhtmlbrow
 * ARR added for mouse over.
 *
 * Revision 1.24  2008/07/23 04:32:00  kumardevhtmlbrow
 * Compilation Warning update
 *
 * Revision 1.23  2008/04/11 12:47:44  kavithadevhtmlbrow
 * ARR Hidden in DE handled
 *
 * Revision 1.22  2008/03/27 08:04:53  kavithadevhtmlbrow
 * E_ARR_HIDDEN
 *
 * Revision 1.21  2007/12/27 12:17:37  kavithadevhtmlbrow
 * Compilation error updated
 *
 * Revision 1.20  2007/12/27 11:08:29  sriramdevhtmlbrow
 * Added new ARR for handling frames side by side.
 *
 *  
 ***************************************************************************/

/**
 * @page arr ARR (Adaptive Rendering Rules)
 *
 * There are different rendering mechanisms available to adapt the HTML content that is 
 * designed for desktop to the mobile browser. But there is no unique algorithm available 
 * that can adapt billions of pages available in the WEB to the mobile screen. 
 * 
 * The main criteria for the rendering mechanism in the mobile phone are:
 * 
 * @li Avoid two-dimensional scrolling and restrict only to up-down movement of the page.
 * @li Wrap into multiple lines, if not shrink to fit width of the screen.
 * 
 * Jataayu provides flexible and adaptive rendering solution, which will provide better 
 * user experience and easy navigation for different types of environment. This rendering 
 * solution has been mainly designed with the consideration of the feature phone(s).
 *
 * Jataayu adaptive rendering solution is termed as ART™, Adaptive Rendering Technology.
 *
 * In order to be adaptive according to the device capability and the display mode is 
 * controlled by set of rules, which is termed as ARR (Adaptive Rendering Rules). It contains
 * @li Every spatially affected language element (tag or CSS property) has controlling rules.
 * @li Each rule defines behavior for that element.
 *
 * A set of rules covering all the spatial affected elements defines the new layout mode. 
 * Many such layout modes can be defined by defining more rules for the layout elements. 
 *
 * @section formelements Form Elements
 * 
 * Provides the rules for controlling the size of the form elements like input box, select
 * and combo box. Refer #E_ARR_FORM.  
 *
 * @section button Button
 * 
 * Used to control the width of the button using rules. Refer #E_ARR_BUTTON. 
 * 
 * @section image Image
 * 
 * Used to control the dimension of the inline image. Refer #E_ARR_IMAGE.
 * 
 * @section table Table
 * 
 * Controls the rendering of the table. Refer #E_ARR_TABLE.
 *
 * @section imagemap Image Map
 *
 * An image map is a list of coordinates relating to a specific image, created in order 
 * to hyperlink areas of the image to various destinations (as opposed to a normal image 
 * link, in which the entire area of the image links to a single destination). 
 * For example, a map of the world may have each country hyperlinked to further information 
 * about that country. The intention of an image map is to provide an easy way of linking 
 * various parts of an image without dividing the image into separate image files.
 * 
 * This rules controls how image map can be controlled. Refer #E_ARR_IMAGEMAP.
 *
 * @section frame Frame
 * 
 * Allows the authors to present documents in multiple view, which may be independent or sub-windows. 
 * This rules controls how the frame content can be rendered. Refer #E_ARR_FRAME. 
 *
 * @section noframe NoFrame
 *
 * No frame content are available when the browser does not support frame mode of display. 
 * This rules controls how the noframe content can be rendered. It is recommended to enable 
 * this option based on the frame rules chosen. Refer #E_ARR_NOFRAME.
 *
 * @section iframe IFrame
 *
 * The IFRAME element allows authors to insert a frame within a block of text. Inserting an 
 * inline frame within a section of text is much like inserting an object via the OBJECT 
 * element: they both allow you to insert an HTML document in the middle of another, 
 * they may both be aligned with surrounding text, etc.
 *
 * This rules provides the option how iframe content can be rendered. Refer #E_ARR_IFRAME.
 *
 * @section link Link
 *
 * It conveys relationship information that may be rendered by user agents in a variety of ways 
 * (e.g., a tool-bar with a drop-down menu of links). Browser internally handles the style 
 * sheet information if available as part of the link tag.
 *
 * This rules provides the option how link can be handled. Refer #E_ARR_LINK.
 *
 * @section fontsize Font Size
 *
 
 * platform, there is the restriction in the font size. This rules allows to control the 
 * font size for rendering the page. Refer #E_ARR_FONTSIZE.
 *
 * @section position Position
 *
 * This will be applied only when the position is specified for the block with the visibility 
 * property set to hidden. Refer #E_ARR_POSITION.
 *
 * @section margin Margin
 * 
 * The CSS margin properties define the space around elements. This rules provide how the margin 
 * space can be controlled. Refer #E_ARR_MARGIN. 
 *
 * @section padding Padding
 *
 * The CSS padding properties define the space between the element border and the element content.
 * This rule provide how the padding space can be controlled. Refer #E_ARR_PADDING.
 *
 * @section percentage Percentage
 *
 * Property value has been specified as the percentage, but generally authors have considered the 
 * percentage based on the desktop size. This rule provide how this can be handled. Refer #E_ARR_PERCENTAGE. 
 *
 * @section indentation Indentation
 *
 * Indentation are used for to place the text or element farther to the right to separate 
 * it from surrounding text. Refer #E_ARR_INDENTATION.
 *
 * @section wraptext Text Content
 *
 * This rule is applied only when the no-wrap/pre property is specified for the block. Refer #E_ARR_WRAPTEXT.
 * 
 * @section lineheight Line Height
 * 
 * Line height property is specified in the CSS. This rule allows how to process the line height. 
 * Refer #E_ARR_LINEHEIGHT.
 *
 * @section float Float
 *
 * Float property is specified in the CSS, where the text content needs to be rendered around the block or image. 
 * Refer #E_ARR_FLOATTEXT, which allows to control this property.
 *
 * @section bgimage Background Image
 * 
 * The background-image property sets an image as the background. This rule allows how 
 * the background-image can be rendered. Refer #E_ARR_BGIMAGE.
 *
 * @section text-indent Text Indent
 *
 * The text-indent CSS property indents the text based on the value specified. Rules allows
 * to control the text-indent value applied. Refer #E_ARR_TEXT_INDENTATION
 *
 * @section overflow Overflow
 *
 * The overflow CSS property specifies how to handle the content of the box that exceeds the
 * box height/width. The property value can be overriden by using the rules. Refer #E_ARR_OVERFLOW
 *
 * @section hidden Hidden Visibility
 *
 * The visible CSS property specifies the block to be visible or hidden, applying the hidden value 
 * can be controlled by rules. Refer #E_ARR_HIDDEN
 *
 * @section general General 
 *
 * This is generic rule which is applied for the other elements, which is not described 
 * above. Refer #E_ARR_GENERAL. 
 * 
 * @section fixed Fixed Position
 *
 * The Fixed Position CSS property states that an element for which it has been specified shall remain
 * fixed at the specified position regardless of scrolling. The property value can be overriden by using
 * the rules. Refer #E_ARR_FIXED_POSITION
 *
 * @section mouseover MouseOver
 *
 * The JavaScript onmouseover event occurs when a mouse pointer moves over a specified object. JavaScript
 * functionality may be specified for an element for which this event needs to be captured. Whether to execute
 * the give script functionality or whether to ignore this event can be controlled by the rules. Refer #E_ARR_MOUSEOVER
 */

/**
 * @file jdi_arr.h
 * @ingroup ARR
 * @brief This file contains the data-structures related to the ARR (Adaptive Rendering Rules).
 */

#ifndef JDI_ARR_H
#define JDI_ARR_H


/**
 * @brief Used to specify the arr settings based on which the browser display is set.
 */
typedef enum arrGeneral
{
	E_ARR_GENERAL_FIT_ON_EXCEED, /**< Fit the display case it exceeds maximum limit */
	E_ARR_GENERAL_ALWAYS_FIT, /**< Fit the display at all times */
	E_ARR_GENERAL_PAGE_INST /**< Display should be based on the page instruction */
} E_ARR_GENERAL ;

/**
 * @brief Used to specify the ARR settings based on which the form size is set for elements 
 * like input box, select and combo box.
 */
typedef enum arrForm
{
	E_ARR_FORM_FIT_ON_EXCEED, /**< If the specific form element exceeds the block size then it has been adjusted to the available size.*/
	E_ARR_FORM_ALWAYS_FIT, /**< If the specific form element exceeds the visible width, it will sized to the visible width.*/
	E_ARR_FORM_PAGE_INST /**< Displays the form control as per described in the page. */
} E_ARR_FORM ;


/**
 * @brief Used to specify the behaviour of button in case its size exceeds the screen width
 */
typedef enum arrButton
{
	E_ARR_BUTTON_TRUNCATE_TEXT, /**< Truncate the text displayed on the button, when it exceeds the parent block.*/
	E_ARR_BUTTON_MULTILINE, /**< Display the button in multiline, when it exceeds the parent block.*/
#ifdef UNSUPPORTED_ARR_RULES
	E_ARR_BUTTON_MARQUEE, /**< Marquee the button */
#endif
	E_ARR_BUTTON_PAGE_INST /**< Set the button based on the page instruction */
} E_ARR_BUTTON ;


/**
 * @brief Used to specify the behaviour of image in case its size exceeds the screen width
 */
typedef enum arrImage
{
	E_ARR_IMAGE_ASPECT_FIT, /**< If the image size exceeds the display area, the image will be fit using the aspect ratio. */
	E_ARR_IMAGE_PAGE_INST, /**< Set the size of the image based on the page instruction */
	E_ARR_IMAGE_OPTIMIZE   /**< Same like Image aspect fit; only that it ignores the display/fetching of 
								 - images that are less than 5x5
								 - images without alt attribute
								 - images inside display none block when background image is ignored */
} E_ARR_IMAGE ;


/**
 * @brief Used to specify the rules based on which the table is to be displayed.
 */
typedef enum arrTable
{
	E_ARR_TABLE_2D_HSCROLL_TABLE, /**< Displayed the content in the tabular format which may lead to horizontal scroll. */
	E_ARR_TABLE_2D_FIT_SCREEN, /**< Displays the table in tabular format, if it can be fit within the visible area. In case if it exceeds, the tabular format is ignored and the content are rendered.*/
#ifdef UNSUPPORTED_ARR_RULES
	E_ARR_TABLE_1D_COL_ORDER, /**< To be displayed in 1 dimensional column order format */
#endif
	E_ARR_TABLE_1D_ROW_ORDER /**< Ignores the table format and renders the content in row-order format.*/
} E_ARR_TABLE ;


/**
 * @brief Used to specify the rules based on which the Image map is to be displayed
 */
typedef enum arrImagemap
{
	E_ARR_IMAGEMAP_IGNORE, /**< Ignore the image map */	
#ifdef UNSUPPORTED_ARR_RULES
	E_ARR_IMAGEMAP_NEW_WINDOW, /**< Display image map in separate window. */
	E_ARR_IMAGEMAP_LINKS_NEW_WINDOW, /**< Specify links to image map in separate window */
#endif
	E_ARR_IMAGEMAP_SAME_WINDOW, /**< Display image map on the same window and provides the navigation within the image. E_RECT control is used to highlight the image portion and image is created as the background image. */
	E_ARR_IMAGEMAP_POLYLINE_SAME_WINDOW, /**< Display image map on the same window and provides the navigation within the image. Also, if polyline is supported, shall render all polygon area controls as polylines. */
	E_ARR_IMAGEMAP_LINKS_SAME_WINDOW /**< Displays the image as a control and provide list of links in the image map below the image. */
} E_ARR_IMAGEMAP ;


/**
 * @brief Used to specify the rules based on which the frame is to be displayed.
 */
typedef enum arrFrame
{
	E_ARR_FRAME_LINKS, /**< Displayed as a anchor, when activated it takes to the respective frame page. */
	E_ARR_FRAME_DIV, /**< Display the frame content as a new block level element. 
						  Recommended for F2S Mode. */
	E_ARR_FRAME_PAGE_INST, /**< Display the frames side by side when cols attribute is specified for frameset element. 
						        Recommended for natural Mode. */
	E_ARR_FRAME_IGNORE /**< Ignore the frames */
} E_ARR_FRAME ;


/**
 * @brief Used to specify the rules based on which the NoFrame tag is to be displayed.
 */
typedef enum arrNoframe
{
	E_ARR_NOFRAME_IGNORE, /**< Ignore NoFrame Tag */
	E_ARR_NOFRAME_RENDER /**< Render NoFrame Tag */
} E_ARR_NOFRAME ;


/**
 * @brief Used to specify the rules based on which the iFrame is to be displayed.
 */
typedef enum arrIFrame
{
	E_ARR_IFRAME_LINK, /**< Displayed as a anchor, when activiated it takes to the respective iFrame page.*/
	E_ARR_IFRAME_DIV, /**< Displayed the inline frame fetched content as the block element.*/
	E_ARR_IFRAME_CONTENT /**< Ignore the iFrame tag and displays the content inside the iFrame element. */
} E_ARR_IFRAME ;

/**
 * @brief Used to specify Link element rules
 */
typedef enum arrLink
{
	E_ARR_LINK_IGNORE,
	E_ARR_LINK_DISPLAY_TOP,
	E_ARR_LINK_DISPLAY_MENU
} E_ARR_LINK ;

/**
 * @brief Used to specify the behaviour of font in case the display exceeds screen width
 */
typedef enum arrFontSize
{
	E_ARR_FONTSIZE_PAGE_INST, /**< Use font size based on the page instruction */
	E_ARR_FONTSIZE_IGNORE /**< Ignore the font size */
} E_ARR_FONTSIZE ;


/**
 * @brief Used to specify margin rules when content width becomes small
 */
typedef enum arrMargin
{
	E_ARR_MARGIN_PAGE_INST, /**< Provides the margin as specified in the style sheet. */
	E_ARR_MARGIN_SCALE_RATIO,  
	E_ARR_MARGIN_IGNORE, /**< Margin specified in the style sheet are ignored completely. Default margin of 5px is applied. */
	E_ARR_MARGIN_SCALE_ON_EXCEED, /**< Margin will be scaled down only when the block width exceeds the configured displayable width. */
	E_ARR_MARGIN_IGNORE_ON_EXCEED /**< Margin will be ignored when the block width exceeds the configured displayable width. */
} E_ARR_MARGIN ;


/**
 * @brief Used to specify Padding rules when content width becomes small
 */
typedef enum arrPadding
{
	E_ARR_PADDING_PAGE_INST, /**< Provides the padding as specified in the style sheet. */
	E_ARR_PADDING_SCALE_RATIO,  
	E_ARR_PADDING_IGNORE, /**< Padding specified in the style sheet are ignored completely. Default padding of 5px is applied. */
	E_ARR_PADDING_SCALE_ON_EXCEED, /**< Padding will be scaled down only when the block width exceeds the configured displayable width. */
	E_ARR_PADDING_IGNORE_ON_EXCEED /**< Padding will be ignored when the block width exceeds the configured displayable width. */
} E_ARR_PADDING ;


/**
 * @brief Used to specify the position Style rules
 */
typedef enum arrPosition
{
	E_ARR_POSITION_PAGE_INST, /**< Set position style based on the page instruction */
	E_ARR_POSITION_IGNORE /**< Ignore position rules */
} E_ARR_POSITION ;

/**
 * @brief Used to specify the Fixed Position Style rules
 */
typedef enum arrFixedPosition
{
	E_ARR_FIXED_POSITION_PAGE_INST, /**< Set Fixed Position style based on the page instruction */
	E_ARR_FIXED_POSITION_IGNORE, /**< Ignore Fixed Position rules */
	E_ARR_FIXED_POSITION_HIDE /**< Does not display the Fixed Position content */
} E_ARR_FIXED_POSITION ;

/**
 * @brief Used to specify the percentage rules
 */
typedef enum arrPercentage
{
	E_ARR_PERCENTAGE_PAGE_INST, /**< Any property value specified as percentage will be applied accordingly. */
	E_ARR_PERCENTAGE_IGNORE /**< Any property value specified as percentage will be ignored.*/
} E_ARR_PERCENTAGE ;


/**
 * @brief Used to specify Indentation rules
 */
typedef enum arrIndentation
{
	E_ARR_INDENTATION_MAX_LEVEL, /**< Indentation of the list item is provided only to the configured maximum level. (Each level indentation can be configured)*/
	E_ARR_INDENTATION_ALL_LEVEL /**< There is no restriction for the indentation of the list item.*/
} E_ARR_INDENTATION ;


/**
 * @brief Used to specify the Wrapping text rule for no-wrap option 
 */
typedef enum arrWrapText
{
	E_ARR_WRAPTEXT_PAGE_INST, /**< Displayed the content as specified in the page.*/
	E_ARR_WRAPTEXT_WRAP, /**< Wraps the content to the block width. */
	E_ARR_WRAPTEXT_MARQUEE_ON_FOCUS /**< Displays the single line content with the block width, if single line exceeds the block width, the control is created as "Marquee on Focus". JDD needs to support the marquee property.*/
} E_ARR_WRAPTEXT ;


/**
 * @brief Used to specify the line height rule
 */
typedef enum arrLineHeight 
{
	E_ARR_LINEHEIGHT_IGNORE,  /**< Ignores Lineheight property */
	E_ARR_LINEHEIGHT_OVERFLOW, /**< Computed height overrides line height when Line height is lesser */
	E_ARR_LINEHEIGHT_PAGE_INST /**< Process line height as per the page instruction */
} E_ARR_LINEHEIGHT ;

/**
 * @brief Used to specify Floating text rule
 */
typedef enum arrFloatText
{
	E_ARR_FLOATTEXT_ALONGSIDE, /**< Displays the floated window along side based on available width */
	E_ARR_FLOATTEXT_MOVE_BEYOND /**< Ignores the float property and displays the window as block */
} E_ARR_FLOATTEXT ;

/**
 * @brief Used to specify Background Image rule
 */
typedef enum arrBgImage
{
	E_ARR_BGIMAGE_RENDER, /**< Renders/displayes the background images */
	E_ARR_BGIMAGE_IGNORE /**< Does not render the background images */
} E_ARR_BGIMAGE ;

/**
 * @brief Used to specify Text Indentation rules (Used for CSS Property text-indent)
 */
typedef enum arrTextIndentation
{
	E_ARR_TEXT_INDENTATION_IGNORE, /**< Indentation of the text is ignored.*/
	E_ARR_TEXT_INDENTATION_ASPECT_FIT, /**< Indentation of the text is provided aspect-fit.*/
	E_ARR_TEXT_INDENTATION_PAGE_INST /**< Indentation of the text is provided according to the specifications of the page.*/
} E_ARR_TEXT_INDENTATION ;

/**
 * @brief Used to specifies the overflow rules for the content that exceeds the specified height 
 */
typedef enum arrOverFlow
{
	E_ARR_OVERFLOW_PAGE_INST, /**< Set this when window has both clipping and non-clipping support 
								   Exceeded content gets displayed out of box if value is visible/scroll/auto - Sets the window to be non-clipping
								   Hides the exceeded content - Sets window to be clipping 
								   **Recomended for Normal mode for both clipping and non-clipping support */
	E_ARR_OVERFLOW_EXTEND, /**< The box get extended to the width/height of the overflowed content, if overflow is visible/scroll/auto
						        Hides the content that exceeds the box height, if overflow is hide 
								** Recomended for Normal mode that does not have non-clipping support */
	E_ARR_OVERFLOW_HIDE, /**< Hides the content that overflows the box height irrespective of the overflow property.  
						      This makes the height to be restricted as per the page instruction */
	E_ARR_OVERFLOW_IGNORE /**< Extends the block to the width/height of the overflowed content if overflow is visible
						        Ignores the overflow property and resets the width/height of the block if overflow is not visible
						        ** Recomended for F2S mode */
} E_ARR_OVERFLOW ;

/**
 * @brief Specifies the rules for visibile property that are set to hidden 
 */
typedef enum arrHidden
{
	E_ARR_HIDDEN_COLLAPSE,	/**< The hidden block are not display and the space for the hidden block 
								 is not reserved (Handles as display:none property), this rule does not
								 get applied for td/tr elements when the table is rendered as table
								 ** Recommended for F2S mode */
	E_ARR_HIDDEN_VISIBLE,  /**< The hidden property is ignored and the hidden block is made visible */						       
	E_ARR_HIDDEN_HIDE	   /**< The hidden property is applied as specified in the page, the space 
						        used by the hidden block is reserved 
								** Recommended for Normal mode */
} E_ARR_HIDDEN ;


/**
 * @brief Used to specify the Mouseover event rules
 */
typedef enum arrMouseOver
{
	E_ARR_MOUSEOVER_ASSOCIATE_ACTION, /**< */
	E_ARR_MOUSEOVER_IGNORE /**< Ignore the mouse over feature */
} E_ARR_MOUSEOVER ;

#ifdef UNSUPPORTED_ARR_RULES
/**
 * @brief Used to Specify the Mouse Click event rules
 */
typedef enum arrMouseClick
{
	E_ARR_MOUSECLICK_PAGE_INST, /**< */
	E_ARR_MOUSECLICK_MENU /**< */
} E_ARR_MOUSECLICK ;
#endif

/**
 * @brief This structure is to used to specify the various ARR rules. The structure
 *		  contains the various enums that can be used to set the rules.
 */
typedef struct arrRules
{
	E_ARR_GENERAL			eGeneral ; /**< Used to specify the arr settings based on which the browser display is set. */
	E_ARR_FORM				eForm ; /**< Used to specify the arr settings based on which the form size is set. */
	E_ARR_BUTTON			eButton ; /**< Used to specify the behaviour of button in case its size exceeds the screen width */
	E_ARR_IMAGE				eImage ; /**< Used to specify the behaviour of image in case its size exceeds the screen width */
	E_ARR_TABLE				eTable ; /**< Used to specify the rules based on which the table is to be displayed. */
	E_ARR_IMAGEMAP			eImageMap ; /**< Used to specify the rules based on which the Image map is to be displayed */
	E_ARR_FRAME				eFrame ; /**< Used to specify the rules based on which the frame is to be displayed. */
	E_ARR_NOFRAME			eNoframe ; /**< Used to specify the rules based on which the NoFrame tag is to be displayed. */
	E_ARR_IFRAME			eIFrame ; /**< Used to specify the rules based on which the iFrame is to be displayed. */
	E_ARR_LINK				eLink ; /**< Used to specify Link element rules */

	E_ARR_FONTSIZE			eFontSize ; /**< Used to specify the behaviour of font in case the display exceeds screen width */
	E_ARR_MARGIN			eMargin ; /**< Used to specify margin rules when content width becomes small */
	E_ARR_PADDING			ePadding ; /**< Used to specify Padding rules when content width becomes small */
	E_ARR_POSITION			ePosition ; /**< Used to specify the position Style rules */
	E_ARR_PERCENTAGE		ePercentage ; /**< Used to specify the percentage rules */

	E_ARR_INDENTATION		eIndentation ; /**< Used to specify Indentation rules */
	E_ARR_WRAPTEXT			eWrapText ; /**< Used to specify the Wrapping text rule for no-wrap option  */
	E_ARR_LINEHEIGHT		eLineHeight ; /**< Used to specify the Line Height rule */
	E_ARR_FLOATTEXT			eFloatText ; /**< Used to specify Floating text rule */

	E_ARR_BGIMAGE			eBgImage ; /**< Used to specify Background Image rule */
	E_ARR_TEXT_INDENTATION	eTextIndentation ; /**< Used to specify the text indentation rules */
	E_ARR_OVERFLOW			eOverflow ; /**< Used to specify the overflow rules when content exceeds the specified width/height */
	E_ARR_HIDDEN			eHidden ; /**< Used to specify the rules for handling hidden value in visibility property */

	E_ARR_MOUSEOVER			eMouseOver ; /**< Used to specify the Mouseover event rules */
	E_ARR_FIXED_POSITION	eFixedPosition ; /**< Used to specify the Fixed Position style rules */
#ifdef UNSUPPORTED_ARR_RULES
	E_ARR_MOUSECLICK		eMouseClick ; /**< Used to Specify the Mouse Click event rules */
#endif
} ST_ARR_RULES ;



#endif

