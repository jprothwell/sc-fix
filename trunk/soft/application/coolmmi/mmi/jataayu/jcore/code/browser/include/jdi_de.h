/***************************************************************************
 * $Id: jdi_de.h,v 1.184 2009/05/27 08:09:57 sriramdevhtmlbrow Exp $
 * $Revision: 1.184 $
 * $Date: 2009/05/27 08:09:57 $
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
 * $Log: jdi_de.h,v $
 * Revision 1.184  2009/05/27 08:09:57  sriramdevhtmlbrow
 * Comma should not be there for the last member of the enum.
 *
 * Revision 1.183  2009/05/27 06:59:17  kavithadevhtmlbrow
 * Doxygen comments updated
 *
 * Revision 1.182  2009/04/09 14:17:42  sriramdevhtmlbrow
 * Added new callback type E_DE_IS_COOKIE_ENABLED to check whether the cookies are enabled or not.
 *
 * Revision 1.181  2009/03/04 13:04:12  kavithadevhtmlbrow
 * Doxygen Comments Updated
 *
 * Revision 1.180  2009/01/21 14:32:13  kavithadevhtmlbrow
 * Popup Blocker for Window.Open
 *
 * Revision 1.179  2009/01/20 07:45:54  kavithadevhtmlbrow
 * Get and Free Absolute URI
 *
 *
 ***************************************************************************/

/**
 * @mainpage
 *
 * @version 1.3
 * Display Engine Interface Description
 *
 * This Documentation gives a detailed description of the interface that has to be used
 
 * describes the usage of each function, the significance of each variable, usage of the data 
 * structures and enumerations.  The document will ease the complexity faced while 
 * interfacing the application and the Browser.
 *
 *
 * @li @ref jcf
 * @li @ref mmi
 * @li @ref arr
 * @li @ref drm
 * @li @ref displayconfig
 *
 */

/**
 * @defgroup DEEngine			Display Engine
 * @defgroup ARR				ARR
 * @defgroup Plugins			Plugins
 * @defgroup DeConfig			Display Engine Configuration
 */

/**
 * @file jdi_de.h
 * @ingroup DEEngine
 * @brief Defines the exposed data types for display engine
 *
 * Defines various data types, function prototype and error codes that can be 
 * used by the application that uses this module
 */

#ifndef _JDI_DE_H
#define _JDI_DE_H

/** @cond */
#include <jdi_browsermodule.h>
#include <jdi_mimeinfo.h>
#include <jdi_displayinfo.h>
#include <jdi_ppp.h>
#include <jdi_upp.h>
#ifdef BROWSER_WINDOW_OPEN
#include <jdi_scriptinfo.h>
#endif
/** @endcond */

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @brief This enumeration specifies the set of callback types that are invoked by the display 
 *		  engine to the application. Depending upon the application requirement, it needs to handle it
 */
typedef enum _de_callback_type
{
	E_DE_TRANSPORT_LISTENER, /**< Specified when the Display Engine needs to send a request to the lower layers */	
	E_DE_FOCUS_LISTENER, /**< Intimates the application about a change in focus. (Argument passed #ST_DE_CONTROL_INFO) */
	E_DE_TIMER_LISTENER, /**< Intimates the application to handle the timer related operation like creating timer..(Argument passed #ST_DE_TIMER_INFO) */
	E_DE_DO_HANDLER_LISTENER, /**< Intimates the application to handle the DO element. (Argument passed #ST_DE_PAGE_DO_LIST) */
	E_DE_HISTORY_LISTENER, /**< Intimates the application to handle the history operation like adding a url, title.. (Argument passed #ST_DE_HISTORY_INFO) */
	E_DE_MULTIMEDIA_CONTENT, /**< Intimates the application to handle the Multimedia content (Argument passed #ST_DE_MULTIMEDIA_CONTENT) */
	E_DE_FREE_MULTIMEDIA_CONTENT, /**< Intimates the end of Multimedia content */
#ifdef DE_DRM_ENABLE
	E_DE_DRM_CONTENT_LISTENER, /**< Intimates the application to handle the DRM content. (Argument passed #ST_DRM_EXT_CONTENT) */
	E_DE_DRM_CONTENT_MORE_LISTENER, /**< Intimates the application to handle the more DRM content. (Argument passed #ST_DRM_EXT_CONTENT_MORE) */
	E_DE_DRM_CANCEL_LISTENER, /**< Intimates the application that DRM content need not be sent further. (Argument passed #ST_DRM_EXT_CANCEL) **/	
	E_DE_DRM_FREE_LISTENER, /**< Intimates the application to free the DRM right objects. (Argument passed #ST_DRM_EXT_FREE) **/			
#endif
	E_DE_VSCROLL_LISTENER, /**< Intimates the application that a vertical scroll is requested. (Argument Passed #ST_DE_SCROLL_INFO) */
	E_DE_HSCROLL_LISTENER, /**< Intimates the application that a horizontal scroll is requested (Argument Passed #ST_DE_SCROLL_INFO) */
	E_DE_TITLE_LISTENER, /**< Intimates the application about the title of the page being fetched (Argument passed is the reference to JC_CHAR title string)*/
	E_DE_ICON_LISTENER, /**< Intimates the application about the icon URI of the page being fetched (Argument passed is the reference to JC_CHAR icon URI string) */
	E_DE_AUTHORIZATION_LISTENER, /**< Intimates the application to handle the authorization (Argument passed #ST_DE_PAGE_INFO)*/
	E_DE_PROXYAUTHORIZATION_LISTENER, /**< Intimates the application to handle the proxy authorization (Argument passed #ST_DE_PAGE_INFO) */
#ifdef BROWSER_AJAX_ENABLE
	E_DE_AJAX_AUTH_LISTENER, /**< Intimates the application to handle the authorization for an ajax request (Argument passed #ST_DE_PAGE_INFO)*/
	E_DE_AJAX_PROXYAUTH_LISTENER, /**< Intimates the application to handle the proxy authorization for an ajax request (Argument passed #ST_DE_PAGE_INFO) */
#endif

	E_DE_SCRIPT_EXEC_CONFIRMATION, /**< Intimates the application to get the user confirmation for executing the script (Argument passed #ST_DE_SCRIPT_EXEC_CONFIRMATION) */
	E_DE_WMLS_ALERT_LISTENER, /**< Intimates the application to handle the WMLS alert (Argument passed #ST_DE_WMLS_ALERT) */				
	E_DE_WMLS_CONFIRM_LISTENER,	/**< Intimates the application to handle the WMLS confirm (Argument passed #ST_DE_WMLS_CONFIRM) */
	E_DE_WMLS_PROMPT_LISTENER, /**< Intimates the application to handle the WMLS prompt (Argument passed #ST_DE_WMLS_PROMPT) */
	E_DE_WTAI_MAKE_CALL_LISTENER, /**< Intimates the application to handle WTAI Make call (Argument Passed #ST_DE_WMLS_WTAI_MAKECALL) */
	E_DE_WTAI_SEND_DTMF_LISTENER, /**< Intimates the application to handle Send DTMF request (Argument Passed #ST_DE_WMLS_WTAI_SENDDTMF) */
	E_DE_WTAI_PHONE_BOOK_LISTENER, /**< Intimates the application to handle the phonebook related operation (Argument Passed #ST_DE_WMLS_WTAI_PHONEBOOK) */
	
	E_DE_BEGIN_PAGE_LISTENER, /**< Intimates the application that DE has started displaying a page (Argument passed #ST_DE_BEGIN_PAGE_INFO) */
	E_DE_END_PAGE_LISTENER, /**< Intimates the application that Display engine has finished displaying a page (Argument passed #ST_DE_END_PAGE_INFO) */
	E_DE_BEGIN_FETCH_LISTENER, /**< Intimates the application that browser has started processing a fetch request (Argument passed #ST_DE_BEGIN_FETCH_INFO) */
	E_DE_END_FETCH_LISTENER, /**< Intimates the application that browser has finished processing a fetch request (Argument passed #ST_DE_END_FETCH_INFO) */
	E_DE_BEGIN_DISPLAY_LISTENER, /**< Intimates the application that Display engine has started displaying content  */
	E_DE_END_DISPLAY_LISTENER, /**< Intimates the application that Display Engine has finished displaying content */

	E_DE_PAGE_PROGRESS_LISTENER, /**< Intimates the application about the contents of the page request received (Argument #ST_DE_PAGE_PROGRESS) */
	E_DE_STOPALL_REQUEST_LISTENER, /**< Intimates the application to stop all the requests sent from the display engine */
	E_DE_STOP_REQUEST_LISTENER, /**< Intimates the application to stop the request sent (Argument passed #ST_DE_STOP_REQUEST) */
	E_DE_STOP_REQUEST_URI_LISTENER, /**< Intimates the application to stop the request by URI (Argument passes JC_CHAR URI value) */
	E_DE_FREE_HEADER_LISTENER, /** < Intimates the application to free all the header (Argument passed #ST_DE_FREE_HEADER_INFO) */
	E_DE_URL_SUBMIT_WIZ_LISTENER, /**< Intimates the application about post data info */
	E_DE_META_HEADER_LISTENER, /**< Intimates the application about the meta header information (Argument passed #ST_DE_META_HEADER) */
#ifdef BROWSER_WEB_SPIDER_ON
	E_DE_BROWSER_WEB_SPIDER_LISTENER, /**< Intimates the application to add the testing URL to the list */
#endif
#ifdef DE_SCRIPT_ENABLE
	E_DE_BOM_COOKIE_SET, /**< Intimates the application to add the cookie string to cookie list (Argument passed (#ST_DE_BOM_COOKIE_INFO) cookie information) */
	E_DE_BOM_COOKIE_GET, /**< Intimates the application to get the cookie string from cookie list (Argument passed (#ST_DE_BOM_COOKIE_INFO) output parameter) */
	E_DE_BOM_COOKIE_FREE, /**< Intimates the application to free the cookie string which was previosly recieved using #E_DE_BOM_COOKIE_GET (Argument passed (JC_INT8 *) cookie information */
	E_DE_BOM_ALERT_LISTENER, /**< Intimates the application to handle the BOM alert (Argument passed #ST_BOM_ALERT) */				
	E_DE_BOM_CONFIRM_LISTENER,	/**< Intimates the application to handle the BOM confirm (Argument passed #ST_BOM_CONFIRM) */
	E_DE_BOM_PROMPT_LISTENER, /**< Intimates the application to handle the BOM prompt (Argument passed #ST_BOM_PROMPT) */
	E_DE_BOM_EXEC_CONFIRM, /**< Intimates the application to get the user confirmation for executing the script */
	E_DE_FREE_PROMPT_STRING, /**< Intimates the application to free the prompt string. This callback will be used only for blocking call.(Argument passed #JC_CHAR) */
	E_DE_BOM_ABORT_CONFIRM, /**< Intimates the application to get the user confirmation for aborting the script (Argument passed #ST_DE_BOM_ABORT_CONFIRM) */
	E_DE_BOM_GET_SCREEN_INFO, /**<Intimates the application to get the screen information (Argument passed #ST_BOM_SCREEN_INFO) */
#endif
#ifdef DE_PPP_READWRITE_WIDGET
	E_DE_WIDGET_UPDATE_MENU, /**< Initimates the application about the updation of widget menus. (Argument passed #ST_PPP_WIDGET_MENU) */
#endif 
#ifdef DE_FILE_UPLOAD_ENABLE
	E_DE_FILE_BROWSE_NAME_LISTENER,  /**< Intimates the application for file browse display name (Argument passed #ST_DE_FILE_BROWSE_NAME) */
	E_DE_FILE_BROWSE_LISTENER,  /**< Intimates the application to get the file name using browser operation (Argument passed #ST_DE_FILE_BROWSE_INFO) */
#endif
#ifdef BROWSER_EXTENDED_SUPPORT
	E_DE_ANCHOR_CONTENT_SUPPORT, /**< Intimates the application to check with the user whether to download the link or not.
									  The given content type is the destination content type of the anchor element selected by the user. (Argument passed #ST_DE_ANCHOR_PARAM_SUPPORT).
									  If it is implemented as non blocking then the response has to be sent using the event #E_DE_ANCHOR_CONTENT_SUPPORT_RESP. */
	E_DE_ANCHOR_CHARSET_SUPPORT, /**< Intimates the application to check with the user whether to download the link or not. 
									  The given charset type is the destination charset type of the anchor element selected by the user. (Argument passed #ST_DE_ANCHOR_PARAM_SUPPORT).
									  If it is implemented as non blocking then the response has to be sent using the event #E_DE_ANCHOR_CHARSET_SUPPORT_RESP. */
#endif
#ifdef BROWSER_IMODE_ATTRIBUTES
	E_DE_PAGE_PERM_REDIRECTION, /**< This callback intimates the application that the page is permanently redirecting to some other URL. (Argument passed #JC_NULL). */
#endif
#ifdef BROWSER_WINDOW_OPEN
	E_DE_GET_BOM_INFO, /**< Intimates the application to get the BOM Context and the BOM session handles. The same shall be returned through the callback itself. (Argument passed #ST_DE_BOM_INFO) */
	E_DE_WINDOW_OPEN, /**< Intimates the application to verify and create a new window. (Argument passed #ST_DE_WINDOW_OPEN) */
	E_DE_WINDOW_RESET, /**< Intimates the appliation to verify other DE contexts for resetting the opener before closing a particular page context. (Argument passed #ST_DE_WINDOW_RESET) */
	E_DE_WINDOW_CLOSE, /**< Intimates the application to search for the page context to be deleted whenever window.close is called from script and hence only the BOM session is known. (Argument passed #ST_DE_WINDOW_CLOSE) */
	E_DE_SET_ACTIVE_WINDOW, /**< Intimates the application to set the active window. */
	E_DE_GET_WINDOW_CLOSED, /**< Intimates the application to verify the existence of the passed page context against all the DE Contexts */
#endif
	E_DE_ERROR_HANDLER, /**< Intimates the application an error in display engine operation (Argument Passed #ST_DE_ERROR_LISTENER) */
	E_DE_PPP_READWRITE_LISTENER, /**< Intimates the application PPP related info like confirmation/prompt/etc. (Argument passed #ST_PPP_READWRITE_REQUEST) */
	E_DE_IS_COOKIE_ENABLED /**< Requests application to check whether the cookie functionality is enabled or not. (Argument passed JC_BOOLEAN) */
} E_DE_CALLBACK_TYPE ;


/**
 * @brief Specifies the set of browser options or features that the application wants to enable or
 *		  set. The application will specify this option value. The display engine needs to call the
 *		  appropriate function to enable or set the option.
 */
typedef enum _de_set_options
{
	E_DE_SET_DISPLAY_MODE, /**< Used to set the display mode (normal, F2S etc) */
	E_DE_SET_VISIBILITY, /**< Used to set the visibilty mode (normal, full screen etc) */
	E_DE_SET_GRAPHICS, /**< Used to set the graphics options  */
	E_DE_SET_AUDIO, /**< Used to set the audio options */
	E_DE_SET_IMAGE_SELECTION, /**< Used to specify whether the image can be selected or not */
	E_DE_SET_REFERER_HEADER_MODE, /** Used to specify the referer header mode */
	E_DE_SET_MAGNIFICATION, /**< Used to specify the magnification */
	E_DE_SET_SCROLL_MODE, /**< Used to specify the scrolling mode */
	E_DE_SET_DO_DISPLAY, /**< Used to specify whether the browser needs to display the DO functionality */
	E_DE_SET_STYLE, /**< Used to set the basic style */
	E_DE_SET_ARR_RULE, /**< Used to set the ARR rules */
	E_DE_SET_ENCODING, /**< Used to set the charset encoding, which will override the charset encoding specified by the page. Arguent passed #ST_DE_OPTION_ENCODING */
	E_DE_SET_DEFAULT_ENCODING, /**< Used to set the default charset encoding, which will be used incase the page doesn't specify any charset encoding. 
									This again will be overwritten by the encoding set using #E_DE_SET_ENCODING. Arguent passed #ST_DE_OPTION_ENCODING */
	E_DE_SET_CSS_PROPERTIES, /**< Used to set the CSS Properties */
	E_DE_SET_DEFAULT_CSS, /**< Used to set the default CSS for the specified type */
	E_DE_SET_PERFORMANCE,	/**< Used to set the browser performace properties (Argument passed #ST_DE_PERFORMANCE) */
#ifdef DE_SCRIPT_ENABLE
	E_DE_SET_SCRIPT, /**< Used to enable/disable script handling. uses ST_DE_OPTION_SCRIPT*/
#endif
	E_DE_SET_APPLICATION_INFO, /**< Used to get the application info. Argument passed to this property is #ST_DE_APPLICATION_INFO*/
	E_DE_SET_URL_SUBMIT_WIZ /**< */
} E_DE_SET_OPTIONS ;


typedef E_BE_DISPLAY_MODE		E_DE_DISPLAY_MODE ;

typedef E_BE_ACCESSKEY_DISPLAY	E_DE_ACCESSKEY_DISPLAY  ;

typedef E_BE_SCROLL_MODE		E_DE_SCROLL_MODE ;

typedef E_BE_SCROLL_DIRECTION	E_DE_SCROLL_DIRECTION ;

/**
 * @brief The enumeration specifies the type of events that will be sent from the application 
 *		  to the display engine. The display engine will call the appropriate function which 
 *		  will handle the event.
 */
typedef enum _de_event_type
{
	E_DE_SCROLL_UP, /**< Used when scroll up needs to be done.*/
	E_DE_SCROLL_DOWN, /**< Used when scroll down needs to be done */
	E_DE_SCROLL_LEFT, /**< Used when scroll left needs to be done */
	E_DE_SCROLL_RIGHT, /**< Used when scroll right needs to be done */
	E_DE_SCROLL_NO_FOCUS,	/**< Used for smooth scrollign witout setting focus */
	E_DE_SCROLL_NEXT_FOCUS, /**< Used when focus needs to move to the next selectable control. Applicable only in F2S mode */ 
	E_DE_SCROLL_PREV_FOCUS, /**< Used when focus needs to move to the previous selectable control. Applicable only in F2S mode */ 
	E_DE_SCROLL_PREV_BLOCK, /**< Used when focus needs to move to the previous block */ 
	E_DE_SCROLL_NEXT_BLOCK, /**< Used when focus needs to move to the next block */ 
	E_DE_SCROLL_NEXT_FORM_ELEMENT, /**< Used when focus needs to move to the next form element */
	E_DE_SCROLL_PREV_FORM_ELEMENT, /**< Used when focus needs to move to the previous form element */
	E_DE_PAGE_UP, /**< Used when page up needs to be done */
	E_DE_PAGE_DOWN, /**< Used when page down needs to be done */
	E_DE_PAGE_LEFT, /**< Used when page left needs to be done */
	E_DE_PAGE_RIGHT, /**< Used when page right needs to be done */
	E_DE_PAGE_TOP_LEFT, /**< Used when focus needs to be shifted to top left of the page */
	E_DE_PAGE_TOP_CENTER, /**< Used when focus needs to be shifted to top center of the page */
	E_DE_PAGE_TOP_RIGHT, /**< Used when focus needs to be shifted to top right of the page */
	E_DE_PAGE_CENTER_LEFT, /**< Used when focus needs to be shifted to center left of the page */
	E_DE_PAGE_CENTER_CENTER, /**< Used when focus needs to be shifted to the center of the page */
	E_DE_PAGE_CENTER_RIGHT, /**< Used when focus needs to be shifted to center right of the page */
	E_DE_PAGE_BOTTOM_LEFT, /**< Used when focus needs to be shifter to bottom left of the page */
	E_DE_PAGE_BOTTOM_CENTER, /**< Used when focus needs to be shifted to bottom center of the page */
	E_DE_PAGE_BOTTOM_RIGHT, /**< Used when focus needs to be shifted to bottom right of the page */
	E_DE_ACTION, /**< Used when an action element is to be handled (Argument to be passed : Event data with the action information) */
	E_DE_TIMER, /**< Used when the timer needs to be handled  */
	E_DE_DO_EVENT, /**< Used when the DO functionality needs to be handled (Argument to be passed #ST_DE_OPTION_DO_DISPLAY) */
#ifndef BROWSER_MOUSE_ENABLE
	E_DE_STYLUS, /**< Used when the stylus operation needs to be handled (Argument to be passed #ST_DE_STYLUS_INFO) */
#else
	E_DE_MOUSE_EVENT, /**< Used when the mouse event operation needs to be handled (Argument to be passed #ST_DE_STYLUS_INFO) */
#endif
	E_DE_KEY_EVENT, /**< Used when the key event needs to be handled (Argument to be passed #ST_DE_STYLUS_INFO) */
	E_DE_SELECT_EVENT, /**< Used when the input select event needs to be handled. */
	E_DE_CHANGE_EVENT, /**< Used when the change event needs to be handled. */
	E_DE_ASYNC_EVENT, /**< Used when the asynchronous event needs to be handled (Argument to be passed #ST_DE_ASYNC_EVENT_DATA) */
	E_DE_ASYNC_REQUEST, /**< Used when the asynchronous resquest needs to be handled */
#ifdef DE_FILE_UPLOAD_ENABLE
	E_DE_FILE_BROWSE_RESP,  /**< Used when application needs to send the file name information for browse operation (Argument to be passed #ST_DE_FILE_BROWSE_INFO) */
#endif
	E_DE_NAVIGATION_CLEAR,  /** < Used when application needs to clear the navigation list */
#ifdef DE_DRM_ENABLE
	E_DE_DRM_RESPONSE, /**< Used when the application need to send the parsed DRM content to the browser (Argument to be passed #ST_DRM_EXT_PARSED_CONTENT) */
	E_DE_DRM_RESPONSE_MORE, /** < Used when the application need to send more of the parsed DRM content to the browser (Argument to be passed #ST_DRM_EXT_PARSED_CONTENT_MORE) */
	E_DE_DRM_RIGHTS_UPDATE, /**< Used when application needs to update the rights object. (Argument passed #ST_DRM_EXT_RIGHTS_UPDATE). */
#endif	
	E_DE_SCRIPT_EXEC_CONFIRMATION_EVENT, /**< #ST_DE_SCRIPT_EXEC_CONFIRMATION_RESP */
	E_DE_WMLS_ALERT_RESP, /**< Used when application needs to send the WMLS alert response to the browser (Argument to be passed #ST_DE_WMLS_ALERT_RESP) */	
	E_DE_WMLS_CONFIRM_RESP,	/**< Used when application needs to send the WMLS confirm response to the browser (Argument to be passed #ST_DE_WMLS_CONFIRM_RESP) */
	E_DE_WMLS_PROMPT_RESP, /**< Used when application needs to send the WMLS prompt response to the browser (Argument to be passed #ST_DE_WMLS_PROMPT_RESP) */
#ifdef DE_SCRIPT_ENABLE
	E_DE_BOM_ALERT_RESP, /**< Used when application needs to send the BOM alert response to the browser (Argument to be passed #ST_BOM_ALERT_RESP) */
	E_DE_BOM_CONFIRM_RESP,	/**< Used when applicaiton needs to send the BOM confirm response to the browser (Argument to be passed #ST_BOM_CONFIRM_RESP) */
	E_DE_BOM_PROMPT_RESP, /**< Used when application needs to send the BOM prompt response to the browser (Argument to be passed #ST_BOM_PROMPT_RESP) */
	E_DE_BOM_EXEC_CONFIRM_RESP, /**< #ST_DE_BOM_EXEC_CONFIRM_RESP*/
	E_DE_BOM_ABORT_CONFIRM_RESP, /**< Used when application needs to send the abort confirm response to the browser #ST_DE_BOM_ABORT_CONFIRM_RESP*/
#endif
#ifdef BROWSER_EXTENDED_SUPPORT
	E_DE_ANCHOR_CONTENT_SUPPORT_RESP, /**< Used when the application needs to send the user response for the event #E_DE_ANCHOR_CONTENT_SUPPORT incase of non blocking implementation. (Argment passed #ST_DE_ANCHOR_PARAM_SUPPORT) */
	E_DE_ANCHOR_CHARSET_SUPPORT_RESP, /**< Used when the application needs to send the user response for the event #E_DE_ANCHOR_CHARSET_SUPPORT incase of non blocking implementation. (Argment passed #ST_DE_ANCHOR_PARAM_SUPPORT) */
#endif
#ifdef BROWSER_WINDOW_OPEN
	E_DE_WINDOW_OPEN_CONFIRM_RESP, /**< Used when the application needs to send the user response for a window.open call for a pop-up window (Argument passed #ST_DE_WINDOW_OPEN_RESP) */
#endif
	E_DE_WTAI_MAKE_CALL_RESP, /**< Used when application needs to send WTAI make call response to the browser (Argument to be passed #ST_DE_WMLS_WTAI_MAKECALL_RESP) */
	E_DE_WTAI_SEND_DTMF_RESP, /**< Used when application needs to send WTAI send DTM response (Argument to be passed #ST_DE_WMLS_WTAI_SEND_DTMF_RESP) */
	E_DE_WTAI_PHONE_BOOK_RESP, /**< Used when application needs to send WTAI phone book response to the browser (Argument to be passed #ST_DE_WMLS_WTAI_PHONEBOOK_RESP) */	
	E_DE_LOW_MEMORY_INDICATION /** Used when application needs to indicate the DE on low memory (Argument to be passed #ST_DE_LOW_MEMORY_INFO) */
} E_DE_EVENT_TYPE ;

#ifdef BROWSER_MOUSE_ENABLE
typedef E_BE_MOUSE_TYPE			E_DE_MOUSE_TYPE ;
#else
typedef E_BE_STYLUS_TYPE		E_DE_STYLUS_TYPE ;
#endif

typedef E_BE_ASYNC_EVENT_TYPE	E_DE_ASYNC_EVENT_TYPE ;

typedef E_BE_MIME_TYPE			E_DE_MIME_TYPE ;

typedef E_BE_CONTROL_ACTION		E_DE_CONTROL_ACTION ;


/**
 * @brief Specifies the enumeration for navigation type
 */
typedef enum _de_navigation_type
{
	E_DE_NAVIGATION_BACK, /**< Specifies that backward navigatation is to be done */
	E_DE_NAVIGATION_FORWARD /**< Specifies that forward navigation is to be done */
} E_DE_NAVIGATION_TYPE ;


/**
 * @brief Specifies the enumeration for the various request types, which are set 
 *		  when that particular request is been sent for fetching.
 */
typedef enum _de_request_type
{
	E_DE_PAGE_REQUEST, /**< Specifies that a request is been sent for fetching a Page */
	E_DE_CSS_REQUEST, /**< Specifies that a request is been sent for fetching a CSS file */
	E_DE_IMAGE_REQUEST, /**< Specifies that a request is been sent for fetching an Image */
	E_DE_AUDIO_REQUEST, /**< Specifies that a request is been sent for fetching an Audio */
	E_DE_VIDEO_REQUEST, /**< Specifies that a request is been sent for fetching a Video */
	E_DE_MIME_REQUEST, /**< Specifies that a request is been sent for fetching the MIME */
	E_DE_EMBED_PAGE_REQUEST, /**< Specifies that a request is been sent for fetching an embeded page */
	E_DE_FRAME_PAGE_REQUEST, /**< Specifies that a request is been sent for fetching an frame page */
	E_DE_NONE_REQUEST /**< Specifies that a no request is been sent */
} E_DE_REQUEST_TYPE ;


/**
 * @brief Specifies the enumeration when a page is to be added to history
 */
typedef enum _de_history_event
{
	E_DE_HISTORY_ADD, /**< Specifies when a page is to be added to History */
	E_DE_HISTORY_ADD_TITLE, /**< Specifies when a page title is to be added to the page which is in the 
								 history and whose title does not exist */
	E_DE_HISTORY_EXISTS, /**< Specifies when a page already exists in History and try to add it again */ 
	E_DE_HISTORY_RESOLVE_FIND  /**< Specifies whether a page already exists in history based on its base and current URI  */
} E_DE_HISTORY_EVENT ;


/**
 * @brief Specifies the enumeration for timer type
 */
typedef enum _de_timer_type
{
	E_DE_LAYOUT_TIMER, /**< Specifies the display engine the timer set for layout continue */
	E_DE_SCRIPT_TIMER, /**< Specifies the display engine the timer set for script continue */
	E_DE_PAGE_TIMER, /**< Specifies the display engine if any timer is specified for the page being fetched */
	E_DE_OTHER_TIMER /**< Specifies the display engine the various other timers (like marquee..) set for a page */
} E_DE_TIMER_TYPE ;

/**
 * @brief Specifies the enumeration for meta header type
 */
typedef enum _de_meta_header_type
{
	E_DE_META_CACHE, /**< Specifies the header type as Cache */
	E_DE_META_COOKIE /**< Specifies the header type as Cookie */
} E_DE_META_HEADER_TYPE ; 


#ifdef BROWSER_WINDOW_OPEN

/**
 * @brief Specifies the type of the window to be opened.
 */
typedef enum _de_window_type
{
	E_DE_WINDOW_NAME, /**< Specifies that a name has been given for the new window that is to be opened */
	E_DE_WINDOW_BLANK, /**< Specifies that a new window with no name has to be opened */
	E_DE_WINDOW_PARENT, /**< Specifies that the new window to be opened should replace the parent window/frame */
	E_DE_WINDOW_TOP, /**< Specifies that the new window to be opened should replace the topmost frameset */
	E_DE_WINDOW_SELF /**< Specifies that the opening window itself should be replaced by the new one. */
} E_DE_WINDOW_TYPE ;

/**
 * @brief Specifies the parameters to be passed at the time of DE Initialize
 *		  Extended call.
 */
typedef struct _de_init_ex_params
{
	JC_HANDLE		vOpenerHandle ; /**< Specifies the opening page context handle **/
	JC_CHAR			*pmConfigPath ; /**< Specifies the config path for the temp files **/	
	JC_UINT32		uiWindowCnt ; /** specifies the no of new windows opened **/
} ST_DE_INIT_EX_PARAMS ;

#endif /** BROWSER_WINDOW_OPEN **/

typedef ST_BE_DISPLAY_MODE_INFO				ST_DE_DISPLAY_MODE_INFO ;

typedef ST_BE_OPTION_VISIBLE				ST_DE_OPTION_VISIBLE ;

typedef ST_BE_OPTION_GRAPHICS				ST_DE_OPTION_GRAPHICS ;

typedef ST_BE_OPTION_AUDIO					ST_DE_OPTION_AUDIO ;

typedef ST_BE_OPTION_IMAGE_SELECTION		ST_DE_OPTION_IMAGE_SELECTION ;

typedef ST_BE_OPTION_REFERER_HEADER_MODE	ST_DE_OPTION_REFERER_HEADER_MODE ;

typedef ST_BE_OPTION_MAGNIFICATION			ST_DE_OPTION_MAGNIFICATION ;

typedef ST_BE_OPTION_SCROLL_MODE			ST_DE_OPTION_SCROLL_MODE ;

typedef ST_BE_SCROLL_NO_FOCUS				ST_DE_SCROLL_NO_FOCUS ;

typedef ST_BE_OPTION_DO_DISPLAY				ST_DE_OPTION_DO_DISPLAY ;

typedef ST_BE_OPTION_STYLE					ST_DE_OPTION_STYLE ;

typedef ST_BE_OPTION_ENCODING				ST_DE_OPTION_ENCODING ;

typedef ST_BE_OPTION_CSS					ST_DE_OPTION_CSS ;

typedef ST_BE_POST_DATA						ST_DE_POST_DATA ;

typedef ST_BE_URL_SUBMIT_WIZ				ST_DE_URL_SUBMIT_WIZ ;

typedef ST_BE_PERFORMANCE					ST_DE_PERFORMANCE ;

typedef ST_BE_LOW_MEMORY_INFO				ST_DE_LOW_MEMORY_INFO ;

typedef ST_BE_APPLICATION_INFO				ST_DE_APPLICATION_INFO ;

typedef ST_BE_DEFAULT_CSS					ST_DE_DEFAULT_CSS ;

/**
 * @brief Specifies information about the header received from the lower layers
 *		  for the request given from the display engine.
 */
typedef struct _de_headers_info
{
	JC_CHAR				*pmURI ; /**< Specifies the URL which is sent to the lower layers for fetching */
	ST_MIME_HEADERS		*pstHeaders ; /**< Specifies the headers of the content received form the fetch request */
} ST_DE_HEADER_INFO ;


/**
 * @brief Specifies information about the content received from the lower layers
 *		  for the request given from the display engine. 
 */
typedef struct _de_content_info
{
	JC_UINT32			uiRequestID ; /**< Specifies the request ID set for every fetch request sent to the lower layers */
	JC_CHAR				*pmURI ; /**< Specifies the URL which is sent to the lower layers for fetching */
	ST_DE_POST_DATA		*pstPostData ; /**< Specifies the post data information */
	JC_RETCODE			rCode ; /**< Specifies the return code for the fetch request given */
	JC_INT8				*pcContentType ; /**< Specifies the content type of the content received form the fetch request */
	ST_MIME_HEADERS		*pstHeaders ; /**< Specifies the headers of the content received form the fetch request */
	JC_UINT32			uiBufTotalLen ;	/**< Specifies the total buffer's (content) length received (stored only if known) */
	JC_UINT8			*pucBuffer ; /**< Specifies the buffer fetched for the request given */
	JC_UINT32			uiBufCurrLen ; /**< Specifies the length of the current buffer received (mainly chunk wise) */
	JC_CHAR				*pmFileName ; /**< Specifies the name of the file. */
	JC_BOOLEAN			bHasMoreData ; /**< Specifies whether more data has to be got from the lower layers */
	E_HTTP_STATUS		eHttpStatus ; /**< Specifies the HTTP status code got for the fetch request given */
	JC_BOOLEAN			bIsCachedContent ; /**< Specifies whether the content received is cachable */
	JC_BOOLEAN			bUseMemory ; /**< Specifies whether memory has to be allocated or use the previously used memory */
} ST_DE_CONTENT_INFO ;


/**
 * @brief Specifies about the more content information received from the lower layers
 *		  for the request given from the display engine
 */
typedef struct _de_content_info_more
{
	JC_UINT32			uiRequestID ; /**< Specifies the request ID set for every fetch request sent to the lower layers */
	JC_UINT8			*pucBuffer ; /**< Specifies the buffer fetched for the request given */
	JC_UINT32			uiBufCurrLen ; /**< Specifies the length of the current buffer received (mainly chunk wise) */
	JC_BOOLEAN			bHasMoreData ; /**< Specifies whether more data has to be got from the lower layers */
	JC_RETCODE			rCode ; /**< Specifies the return code for the fetch request given */
} ST_DE_CONTENT_INFO_MORE ;

typedef ST_BE_TIMER_RESP			ST_DE_TIMER_RESP ;

typedef ST_BE_PAGE_PROGRESS			ST_DE_PAGE_PROGRESS ;

typedef ST_BE_PAGE_INFO				ST_DE_PAGE_INFO ;

typedef U_BE_PAGE_BUFFER			U_DE_PAGE_BUFFER ;

typedef ST_BE_PAGE_CONTENT			ST_DE_PAGE_CONTENT ;

typedef ST_BE_CONTROL_INFO			ST_DE_CONTROL_INFO ; /*** E_DE_FOCUS_LISTENER  **/

typedef ST_BE_PAGE_LINK_LIST		ST_DE_PAGE_LINK_LIST ; 

typedef ST_BE_ANCHOR_LIST			ST_DE_ANCHOR_LIST ;

typedef ST_BE_ANCHOR_DATA			ST_DE_ANCHOR_DATA ;

typedef ST_BE_URL_CONTENT			ST_DE_URL_CONTENT ;

typedef ST_BE_AUTH_INFO				ST_DE_AUTH_INFO ;

typedef ST_BE_FETCH_REQ				ST_DE_FETCH_REQ ;

typedef ST_BE_SCROLL_INFO			ST_DE_SCROLL_INFO ;

#ifdef BROWSER_MOUSE_ENABLE
typedef ST_BE_MOUSE_INFO			ST_DE_MOUSE_INFO ;
#else
typedef ST_BE_STYLUS_INFO			ST_DE_STYLUS_INFO ;
#endif

typedef ST_BE_ASYNC_EVENT_DATA		ST_DE_ASYNC_EVENT_DATA ;

typedef ST_BE_MULTIMEDIA_CONTENT	ST_DE_MULTIMEDIA_CONTENT ;

typedef ST_BE_BUTTON_TYPE			ST_DE_BUTTON_TYPE ;

/**
 * @brief This structure is used by the display engine which specifies the information about 
 *		  the URI request which is given to the lower layers for fetching.
 */
typedef struct _de_uri_request_info			
{
	JC_CHAR					*pmURI ; /**< Specifies the URI of the request to be fetched */
	JC_CHAR					*pmRefererURI ; /**< Specifies the Referer Header value to be sent with the request */
	ST_DE_POST_DATA			*pstPostData ; /**< Specifies the post data information */
	ST_DE_AUTH_INFO			*pstAuthInfo ; /**< Specifies the authorization information */
	ST_DE_AUTH_INFO			*pstProxyAuthInfo ; /**< Specifies the proxy authorization information */
	ST_MIME_HEADERS			*pstHeaders ; /**< Specifies the headers of the content received for the fetch request */
	JC_BOOLEAN				bIsReloadPage ; /**< Specifies whether the page needs to be reloaded */
	JC_BOOLEAN				bStopAllPrevRequest ; /**< Specifies whether Stop all */
	E_DE_REQUEST_TYPE		eRequestType ; /**< Specifies the enumeration for the various request types */
	JC_HANDLE				vSchemeElement ; /**< Specifies the handle to the various scheme */
	JC_CHAR					*pmReqFragment ; /**< Specifies the reference to hold the fragment (if any) when request is given */
	JC_BOOLEAN				bNavigation ; /**< Specifies that the request is generated through navigation action (back or forward) */
#ifdef BROWSER_IMODE_ATTRIBUTES
	JC_BOOLEAN				bSendID ; /** Secified whether the IMEI and ICC ID numbers have to be sent to the server with User-Agent header */
#endif
} ST_DE_REQUEST_INFO ;


/**
 * @brief This structure is used by the application to stop the particular 
 *		  request by specifying the request ID.
 */
typedef struct _de_stop_request
{
	JC_UINT32				uiRequestID ; /**< Specifies the request ID set for every fetch request given */
} ST_DE_STOP_REQUEST ;


/**
 * @brief Specifies the history information
 */
typedef struct _de_history_info				
{
	E_DE_HISTORY_EVENT		eHistoryEvent ; /**< Specifies the enumeration when a page is to be added to history */
	E_HTTP_STATUS			eHttpStatus ; /**< Specifies the enumeration for the various HTTP Status retuen codes */
	ST_DE_PAGE_INFO			stPageInfo ; /**< Specifies the page information */
	JC_CHAR					*pmBaseURI ; /**< Specifies the base URI */
} ST_DE_HISTORY_INFO ;


/**
 * @brief This structure is used by the application which free's the URL from the list whose 
 *		  request ID and headers are provided.
 */
typedef struct _de_free_header_info		
{
	JC_UINT32			uiRequestID ; /**< Specifies the request ID set for every fetch request given */
	ST_MIME_HEADERS		*pstHeaders ; /**< Specifies the headers of the content received for the fetch request */
} ST_DE_FREE_HEADER_INFO ;


/**
 * @brief This structure is used internally by the display engine for handling of the timer information
 *		  Specifies information about the timer to be created/stopped
 */
typedef struct _de_timer_info			
{
	JC_UINT32			uiTimerID ; /**< Specifies the timer ID set for every timer occured in the page */
	JC_UINT32			uiValue ; /**< Specifies the timer value in millisecond */
	E_DE_TIMER_TYPE		eType ; /**< Specifies the enumeration for timer types */
	JC_BOOLEAN			bStopTimer ; /**< Specifies the status of the timer which occured in the page */
	JC_HANDLE			vTimerHandle ; /**< Specifies the handle for all the timer related operation */
} ST_DE_TIMER_INFO ;

/**
 * @brief This structure is used by the application to handle the different meta header types
 *		  Specifies the meta header information
 */
typedef struct _de_meta_header
{
	JC_CHAR			*pmName ; /**< Specifies the name of meta header specified in http-equiv */
	JC_CHAR			*pmValue ; /**< Specifies the content attribute value for the Meta element */
	JC_CHAR			*pmRequestURI ; /**< Specifies the current URI being fetched */
	JC_INT8			*pcPostData ; /**< Specifies the current page post data */
	JC_UINT32		uiPostLen ;   /**< Specifies the page post data length */
	JC_UINT32		uiRequestID ; /**< Specifies the page request ID */
} ST_DE_META_HEADER ;


#if (defined DE_SCRIPT_ENABLE) && (defined BROWSER_COOKIE_ENABLE)
/**
 * @brief Specifies the cookie information for BOM cookie events.
 */
typedef struct _de_bom_cookie_info
{
	JC_CHAR		*pmURL ;	/**< Specifies the URI for which the cookie should be added.*/
	JC_INT8		*pcCookie ; /**< Specifies the cookie string.*/
} ST_DE_BOM_COOKIE_INFO ;

#endif

typedef ST_BE_PAGE_DO_LIST			ST_DE_PAGE_DO_LIST ; 

typedef ST_BE_NAVIGATION_INFO		ST_DE_NAVIGATION_INFO ; 

typedef ST_BE_BEGIN_PAGE_INFO		ST_DE_BEGIN_PAGE_INFO ;

typedef ST_BE_END_PAGE_INFO			ST_DE_END_PAGE_INFO ;

typedef ST_BE_END_DISPLAY_INFO		ST_DE_END_DISPLAY_INFO ;

typedef ST_BE_BEGIN_FETCH_INFO		ST_DE_BEGIN_FETCH_INFO ;

typedef ST_BE_END_FETCH_INFO		ST_DE_END_FETCH_INFO ;

#ifdef DE_FILE_UPLOAD_ENABLE
typedef ST_BE_FILE_BROWSE_NAME		ST_DE_FILE_BROWSE_NAME ;

typedef ST_BE_FILE_BROWSE_INFO		ST_DE_FILE_BROWSE_INFO ;
#endif

#ifdef BROWSER_EXTENDED_SUPPORT
typedef ST_BE_ANCHOR_PARAM_SUPPORT	ST_DE_ANCHOR_PARAM_SUPPORT ;
#endif

/*** E_DE_WTAI_MAKE_CALL_LISTENER ***/
typedef ST_BE_WTAI_MAKECALL			ST_DE_WMLS_WTAI_MAKECALL ;
typedef ST_BE_WTAI_MAKECALL_RESP	ST_DE_WMLS_WTAI_MAKECALL_RESP ;

/*** E_DE_WTAI_SEND_DTMF_LISTENER ***/
typedef ST_BE_WTAI_SENDDTMF			ST_DE_WMLS_WTAI_SENDDTMF ;

/*** E_DE_WTAI_PHONE_BOOK_LISTENER ***/
typedef ST_BE_WTAI_PHONEBOOK		ST_DE_WMLS_WTAI_PHONEBOOK ;
typedef ST_BE_WTAI_PHONEBOOK_RESP	ST_DE_WMLS_WTAI_PHONEBOOK_RESP ;
typedef ST_BE_WTAI_SEND_DTMF_RESP	ST_DE_WMLS_WTAI_SEND_DTMF_RESP ;

/*** E_DE_WMLS_ALERT_LISTENER **/
typedef ST_BE_WMLS_ALERT			ST_DE_WMLS_ALERT ;
typedef ST_BE_WMLS_ALERT_RESP		ST_DE_WMLS_ALERT_RESP ;
/*** E_DE_WMLS_CONFIRM_LISTENER **/
typedef ST_BE_WMLS_CONFIRM			ST_DE_WMLS_CONFIRM ;
typedef ST_BE_WMLS_CONFIRM_RESP		ST_DE_WMLS_CONFIRM_RESP ;
/*** E_DE_WMLS_PROMPT_LISTENER **/
typedef ST_BE_WMLS_PROMPT			ST_DE_WMLS_PROMPT ;
typedef ST_BE_WMLS_PROMPT_RESP		ST_DE_WMLS_PROMPT_RESP ;

/*** E_DE_SCRIPT_EXECUTE_CONFIRMATION **/
typedef ST_BE_SCRIPT_EXEC_CONFIRMATION  ST_DE_SCRIPT_EXEC_CONFIRMATION ;

/*** E_DE_SCRIPT_EXECUTION_CONFIRMATION_EVENT **/
typedef ST_BE_SCRIPT_EXEC_CONFIRMATION_RESP ST_DE_SCRIPT_EXEC_CONFIRMATION_RESP ;

/********** DE_SCRIPT_ENABLE  **********/
#ifdef DE_SCRIPT_ENABLE

typedef ST_BE_OPTION_SCRIPT			ST_DE_OPTION_SCRIPT ;

/*** E_DE_BOM_EXECUTION_CONFIRMATION_EVENT **/
typedef ST_BE_BOM_EXEC_CONFIRM_RESP ST_DE_BOM_EXEC_CONFIRM_RESP ;

/*** E_DE_BOM_EXECUTION_CONFIRMATION **/
typedef ST_BE_BOM_EXEC_CONFIRM ST_DE_BOM_EXEC_CONFIRM ;

/*** E_DE_BOM_ABORT_CONFIRMATION **/
typedef ST_BE_BOM_ABORT_CONFIRM ST_DE_BOM_ABORT_CONFIRM ;

/*** E_DE_BOM_ABORT_CONFIRMATION_EVENT **/
typedef ST_BE_BOM_ABORT_CONFIRM_RESP ST_DE_BOM_ABORT_CONFIRM_RESP ;

#endif

/** 
 * @brief The structure is used by the display engine to send the error information to
 *		  the application error handler.
 */
typedef struct st_de_error_listener
{
	JC_RETCODE					rCode ; /**< Specifies the display engine error code.*/
} ST_DE_ERROR_LISTENER ;

#ifdef BROWSER_WINDOW_OPEN

/**
 * @brief This structure is used to contain the BOM Context and the BOM parent session handles
 *		  from BE.
 */
typedef struct st_de_bom_info
{
	JC_HANDLE		vBOMCtxHandle ; /**< Specifies the BOM Context handle **/
	JC_HANDLE		vBOMParentSessionHandle ; /**< Specifies the BOM Parent Session handle **/
} ST_DE_BOM_INFO ;

/**
 * @brief This structure is used to contain the window related information such as the opener
 *		  and other window specific information so that the same may be passed onto BE
 */
typedef struct st_de_window_open
{
	JC_HANDLE			vOpenerHandle ; /**< Specifies the page context handle for the opening window. **/
	E_DE_WINDOW_TYPE	eWinType ; /**< Specifies the type of the window to be opened. **/
	JC_HANDLE			vPageHandle ; /**< Specifies the page context that shall be replaced */
	JC_HANDLE			vDEHandle ; /**< Specifies the new DE Context for which the name needs to be set */
	JC_BOOLEAN			bAdvertisement ; /**< Specifies whether the new window being opened is an advertisement window or not */
	ST_BOM_WINDOW_OPEN	*pstBOMWinOpen ; /** Specifies the stucture containing information on the new window to be opened **/
} ST_DE_WINDOW_OPEN ;

/**
 * @brief This structure is used to contain the information required by DE for resetting the opener
 *		  before a particular page context is deleted.
 */
typedef struct st_de_window_reset
{
	JC_HANDLE		vPageContext ; /**< Specifies the page context being deleted **/
	JC_HANDLE		vDEContext ; /**< Specifies the DE context that contains the page context being deleted **/
} ST_DE_WINDOW_RESET ;

/**
 * @brief This structure is used to contain the information required by DE for closing a particular
 *		  window/page context when window.close call is made from script.
 */
typedef struct st_de_window_close
{
	JC_HANDLE		vBOMSessionHandle ; /**< Specifies the BOM Session handle that needs to be deleted or verified for **/
	JC_BOOLEAN		bWinExists ; /**< Specifies whether the window / DE Context exists for the passed BOM Session **/
} ST_DE_WINDOW_CLOSE ;

/** window.open call response from application **/
typedef ST_BE_WINDOW_OPEN_RESP	ST_DE_WINDOW_OPEN_RESP ;

#endif

/********* End of DE_SCRIPT_ENABLE *********/

/**
 * @brief		Prototype of application callback.
 *
 * @param[in]	pvArg The application needs to specify the app argument while initializing
 *				the browser. The same argument will be passed while calling this application
 *				callback.
 * @param[in]	uiWindowID Specifies the ID of the browser window ID for which the application
 *				callback is called (since the browser supports multiple browser window).
 * @param[in]	eCallbackType Specifies the callback type that the application needs to invoke or handle.
 * @param[in]	pvCallbackInfo Specifies the indication that will be required by the apllication to handle the callback type.
 * @retval		The callback function can return any application related error.
 *
 * The application needs to implement a callback with the following prototype and pass the address
 * of the function in jdi_DEInitialize. From then on the display engine will call this
 * callback to send any information to the application.
 *
 * The display engine will send many indications to the application. Instead of
 * registering a separate application callback for each display engine indication, the 
 * application needs to just register one function with the following prototype. Based on the
 * callback type the application needs to decide which app function needs to be invoked.
 *
 */
typedef JC_RETCODE (* CB_DE_GENERAL_CALLBACK) (void					*pvArg,
											   JC_UINT32			uiWindowID,
											   E_DE_CALLBACK_TYPE 	eCallbackType,
											   void 				*pvCallbackInfo) ;
/** 
 * @brief		Initilizes the display engine
 *
 * @param[in]	uiWindowID Specifies the ID of the browser window ID for which the application
 *				callback is called.
 * @param[in]	cbGeneral Specifies the applications general callback which will handle
 *			    all the callback indications from the display engine.
 * @param[in]	pvCallbackArg Specifies the indication that will be required by the display engine. 
 * @param[in]	vMemHandle Specifies the memory handle to use the internal memory pool for small memory chunk.
 * @param[out]	pvHandle Specifies display engine handle which will be returned by this API.
 *				The application needs to use this handle to call all the other display engine functions.
 * @retval		JC_OK if the display engine is sucessfully initialized.
 * @retval		JC_ERR_MEMORY_ALLOCATION if a memory error occurs.
 * @retval		Other appropriate errors
 * @see			jdi_DEDeinitialize
 *
 * The application needs to call the API to initialize the display engine. 
 * This API will only initialize the display engine module.
 */
JC_RETCODE jdi_DEInitialize (JC_UINT32					uiWindowID, 
							 CB_DE_GENERAL_CALLBACK		cbGeneral,
							 void						*pvCallbackArg,
							 JC_HANDLE					vMemHandle,
							 JC_HANDLE					*pvHandle) ;
#ifdef BROWSER_WINDOW_OPEN
/**
 * @brief Initializes the BOM context  and Parent Session and returns the corresponding handle.
 * @param[in]  vDEContext    handle for the DE context
 * @param[in]  pstBOMInfo    reference to structure holding the handle for BOMContext and parent session
 * @retval  JC_OK	Success on Initializing
 * @retval  Others  Error on Initializing
 **/
JC_RETCODE jdi_DEInitBOM (JC_HANDLE			vDEContext, 
						  ST_DE_BOM_INFO	*pstBOMInfo) ;

/**
 * @brief gets the window with window name specified,.If not present will return null. 
 * @param[in]  vDEContext		handle for the DE context.
 * @param[in]  vOpenerHandle    handle for the opener page context.
 * @param[in]  eWinType			enum for window name .
 * @param[in]  pmWindowName		reference to window name specified.
 * @param[out] pvPageHandle		handle for the pagecontext of window searched according to name .
 * @retval  no return values.
 **/
void jdi_DEGetWindow (JC_HANDLE				vDEContext, 
					  JC_HANDLE				vOpenerHandle,
					  E_DE_WINDOW_TYPE		eWinType,
					  JC_CHAR				*pmWindowName,
					  JC_HANDLE				*pvPageHandle) ;

/** 
 * @brief		Initilizes the display engine
 *
 * @param[in]	uiWindowID			Specifies the ID of the browser window ID for which the application
 *				callback is called.
 * @param[in]	cbGeneral			Specifies the applications general callback which will handle
 *			    all the callback indications from the display engine.
 * @param[in]	pvCallbackArg		Specifies the indication that will be required by the display engine. 
 * @param[in]	vMemHandle			Specifies the memory handle to use the internal memory pool for small memory chunk.
 * @param[in]	pstDEInitExParams	Reference to the params passed from BE at the time of initialization ;
 * @param[out]	pvHandle			Specifies display engine handle which will be returned by this API.
 *				The application needs to use this handle to call all the other display engine functions.
 * @retval		JC_OK if the display engine is sucessfully initialized.
 * @retval		JC_ERR_MEMORY_ALLOCATION if a memory error occurs.
 * @retval		Other appropriate errors
 *
 * The application needs to call the API to initialize the display engine. 
 * This API will only initialize the display engine module.
 */
JC_RETCODE jdi_DEInitializeEx (JC_UINT32				uiWindowID, 
							   CB_DE_GENERAL_CALLBACK	cbGeneral,
							   void						*pvCallbackArg,
							   JC_HANDLE				vMemHandle,
							   ST_DE_INIT_EX_PARAMS		*pstDEInitExParams, 
							   JC_HANDLE				*pvHandle) ;


/**
 * @brief sets the opener handle for the passed page context
 * @param[in]  vOpenerHandle    handle for the opener page context.
 * @param[out] pvPageHandle		handle for the pagecontext of window searched according to name .
 * @retval  no return values.
 **/
void jdi_DEReplaceWindow (JC_HANDLE		vOpenerHandle,
						  JC_HANDLE		vPageHandle) ;


/**
 * @brief gets the window with window name specified.
 * @param[in]  vDEContext			handle for the DE context.
 * @param[in]  vBOMSessionHandle    handle for the BOM Session.
 * @param[in/out]  pbPageFound		Boolean value to represent whether page context exist or not.
 * @param[in/out]  pbIsMainPage		Boolean value to represent whether page context is page context head.
 * @retval  no return values.
 **/
void jdi_DEGetPageContext (JC_HANDLE		vDEContext, 
						   JC_HANDLE		vBOMSessionHandle,
						   JC_BOOLEAN		*pbPageFound,
						   JC_BOOLEAN		*pbIsMainPage) ;

/**
 * @brief Resets the opener handle for all the page contexts which are linked with the opener handle passed.
 * @param[in]  vDEContext		handle for the DE context.
 * @param[in]  vOpenerHandle    handle for the opener page context.
 * @retval  no return values.
 **/
void jdi_DEResetOpener (JC_HANDLE				vDEContext,
						JC_HANDLE				vOpenerHandle) ;

/**
 * @brief deinitializes the BOMcontext and BOMParentSession. 
 * @param[in]  vBOMCtxHandle				handle for BOM context.
 * @param[in]  vBOMParentSessionHandle		handle for BOM parent session.
 * @retval  no return values.
 **/
void jdi_DEDeInitBOM (JC_HANDLE		vBOMCtxHandle,
					  JC_HANDLE		vBOMParentSessionHandle) ;

/**
 * @brief Verifies whether the passed window exists or not.
 * @param[in]  vDEContext		Specifies the DE Context against which the passed window
 *								needs to be checked.
 * @param[in]  pstWinClose		Specifies the window information.
 * @retval  no return values.
 **/
void jdi_DEGetWindowClosed (JC_HANDLE			vDEContext,
							ST_DE_WINDOW_CLOSE	*pstWinClose) ;

#endif /** BROWSER_WINDOW_OPEN **/

/** 
 * @brief		Gets the application callback argument.
 *
 * @param[in]	vHandle			Specifies the handle returned by DE initialize.
 * @retval		void
 *
 * The application needs to call the API to get the application callback argument that was passed
 * when the API #jdi_DEInitialize was called.
 */
void * jdi_DEGetAppCallbackArg (JC_HANDLE		vHandle) ;

/**
 * @brief		Sets the browser properties
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	eSetOptions Specifies the browser property which has to be set.
 * @param[in]	pvOptionInfo Specifies the information related to the browser property.
 * @retval		JC_OK if the browser sets the property sucessfully.
 * @retval		JC_ERR_INVALID_PARAMETER if a invalid DE property is given.
 *
 * The application needs to use this API to set the various browser properties such
 * as display mode, graphics mode, audio, image selection etc. 
 *
 * This API need not be called in any specific sequence. It can be called at any 
 * point to reset the browser properties.
 */
JC_RETCODE jdi_DESetProperties (JC_HANDLE 				vHandle,
								E_DE_SET_OPTIONS 		eSetOptions,
								void					*pvOptionInfo) ;

/**
 * @brief		Sends the fetch request to the lower layers.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	pstFetchReq Specifies the fetch request information.
 * @retval		JC_OK if the fetch request is processed sucessfully.
 * @note		The application need not stop all the requests that are currently being fetched
 *				before calling the fetch request API. The browser will internally stop all 
 *				the currently fetched requests and then proceed with the fetch request.
 *
 * The application needs to use this API to send the fetch request to the lower layers . The 
 * application needs to specify the fetch request information (URL, post data, authorization)
 * etc in pstFetchReq.		
 */
JC_RETCODE jdi_DEFetchRequest (JC_HANDLE			vHandle,
							   ST_DE_FETCH_REQ		*pstFetchReq) ;

/**
 * @brief		Forms the mime header for the application.
 *
 * @param[in]	vHandle			Specifies the handle returned by DE initialize.
 * @param[in]	pcName			Specifies the header name.
 * @param[in]	pcValue			Specifies the header value.
 * @param[out]	ppstMimeHeaders	The API returns the mime header in this param.
 * @retval		JC_OK if the Mime header is created successfully.
 *
 * When the DE sends the request info to the application it sends the headers related to the 
 * DE only. Before sending the request to the lower layers the application may want to add its own
 * headers (such as cookie, cache, referer etc). 
 *
 * At the same time the application may want the DE to hold and free the memory related to the DE request
 * info. This can be done if the DE allocates all the headers using its memory handle. So the application 
 * can use this API to form the mime header and then add the same to the header list. DE in the end will
 * free the entire header list (headers created by DE and the application).
 */
JC_RETCODE jdi_DEFormHeader (JC_HANDLE				vHandle,
							 JC_INT8				*pcName,
							 JC_INT8				*pcValue,
							 ST_MIME_HEADERS		**ppstMimeHeaders) ;

/**
 * @brief		Stop the requests currently being processed.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @retval		JC_OK if the stop request is processed sucessfully.
 *
 * The application needs to use this API to stop the specific request that the browser
 * is currently processing. The API will send stop request for the request (which is
 * yet to be completed) to the lower layer. 
 *
 * Once a request is stopped successfully then the end fetch listener of the application
 * (E_DE_END_FETCH_LISTENER) will be called and once the request for the page is 
 * stopped then the end page listener of the application (E_DE_END_PAGE_LISTENER)
 * will be called.
 */
JC_RETCODE jdi_DEStopRequest (JC_HANDLE			vHandle) ;

/**
 * @brief		Handles the stop all indication
 *
 * @param[in]	vHandle    Specifies the handle returned by DE initialize.
 * @param[in]   pmPageURI  Specifies the Page URI when header is not sent for the page request
 * @param[in]	rErrorCode Specifies the error code of the stop all response.
 * @retval		JC_OK if the stop all indication is processed sucessfully.
 *
 * The application needs to use this API to handle the stop all response that is sent from 
 * the lower layer. The lower layer will send a single response for all the requests that are
 * stopped. The API will delete all the requests that are stopped from the request list and also
 * send the end page listener to the application to specify that the stop all request has been
 * processed successfully.
 */
JC_RETCODE jdi_DEHandleStopAllIndication (JC_HANDLE		vHandle,
										  JC_CHAR		*pmPageURI,
										  JC_RETCODE	rErrorCode) ;

/**
 * @brief		Updates the DE status before giving any new page request.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @retval		JC_OK if pre-processing is successfull.
 *
 * The application needs to use this API before processing the request 
 * to update the Main context structure. It also stops all the timers.
 */
JC_RETCODE jdi_DEPreProcessRequest (JC_HANDLE			vHandle) ;


/**
 * @brief Gets the request fragment if any and update the url 
 *
 * @param[in]     vHandle	 Display Engine Handle
 * @param[in,out] pmURI      Reference to the URI
 * @param[out]    ppmReqFrag Reference to hold the fragment (if any)
 * @retval JC_OK  Success on getting the fragment
 * @retval Others Error on getting the fragment
 *
 * This function used to get the fragment from the URL passed. This can be used
 * by the application before giving the request to Transport.  The same URI memory
 * is used for fragment
 * For eg: pmURI -> http://www.test.com/index.wml#part1
 * After processing: pmURI-> http://www.test.com/index.wml
 *                   *ppmReqFrag-> part1
 * where the # in the URL will be replace with null character and ppmReqFrag points to next index.
 */
JC_RETCODE jdi_DEGetRequestFragment (JC_HANDLE			vHandle,
									 JC_CHAR			*pmURI, 
									 JC_CHAR			**ppmReqFrag) ;

/**
 * @brief		Sets the reponse content received.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	pstContentInfo Reference to the content received.
 * @retval		JC_OK Success on receiving the content.
 * @retval		Other appropriate error codes for the further functions called.
 *
 * Once a request is given for fetching, the page progress listener of the application 
 * (E_DE_PAGE_PROGRESS_LISTENER) is called. Once the content is received, based on the 
 * HTTP status codes appropriate functions like handling the headers, authorization, redirection
 * etc. are called to handle the content received.
 */
JC_RETCODE	jdi_DESetContent (JC_HANDLE					vHandle,
							  ST_DE_CONTENT_INFO		*pstContentInfo) ;

/**
 * @brief		Sets the more response content received.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	pstContentMore Reference to the more content received.
 * @retval		JC_OK Success on receiving the content.
 * @retval		Other appropriate error codes for the further functions called.
 *
 * Once a request is given for fetching, the page progress listener of the application 
 * (E_DE_PAGE_PROGRESS_LISTENER) is called. The more content received for the request given 
 * is handled and various functions are called to handle the same.
 */
JC_RETCODE jdi_DESetContentMore (JC_HANDLE					vHandle,
								 ST_DE_CONTENT_INFO_MORE	*pstContentMore) ;

/**
 * @brief		Sets a browser event. 
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	eEventType Specifies the browser event type
 * @param[in]	pvEventInfo Specifies the information related to the browser event.
 * @retval		JC_OK if the browser handles the event sucessfully.
 * @retval		JC_ERR_INVALID_PARAMETER if the event specified is invalid.
 *
 * The application needs to use this API when it comes across any event or operation that is to 
 * be handled by the display engine. 
 */
JC_RETCODE jdi_DESetEvent (JC_HANDLE					vHandle,
						   E_DE_EVENT_TYPE				eEventType,
						   void 						*pvEventInfo) ;

/**
 * @brief		Gets the page information.
 * 
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[out]  pstPageInfo The API will return the page information in this field.
 * @retval		JC_OK if the browser returns the page info sucessfully
 * @retval		JC_ERR_INVALID_PARAMETER Specifies that no page is currently fetched in the browser.
 *
 * The application needs to call this API to get the information of the page that is 
 * currently displayed on the browser. The page information will not specify the content
 * of the page. To view the content of the page the application needs to call the
 * API jdi_DEGetPageContent
 *
 * @note The page information will be invalid once page display is cleared (say if another page
 * is fetched). The application can call this API at any point of time (page is fetching or fetched). 
 */
JC_RETCODE jdi_DEGetPageInfo (JC_HANDLE				vHandle,
							  ST_DE_PAGE_INFO		*pstPageInfo) ;

/**
 * @brief This function gives the navigation info.
 * @param[in] vHandle		Display engine handle
 * @param[out] pstNavInfo	Reference to the navigation information structure
 * 
 */
void jdi_DEGetNavigationInfo (JC_HANDLE				vHandle,
							  ST_DE_NAVIGATION_INFO	*pstNavInfo) ;

/**
 * @brief		Gets the content of the page.
 * 
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[out]  pstPageContent The API will return the page content in this field.
 * @retval		JC_OK if the browser returns the page content sucessfully
 * @retval		JC_ERR_INVALID_PARAMETER Specifies that no page is currently fetched.
 *
 * The application needs to call this API to get the content of the page that is currently
 * displayed on the browser. The page content will specify the content of the page (source)
 * and will also specify the page information.
 *
 * @note The page content will be invalid once page display is cleared (say if another page
 * is fetched). The application can call this API at any point of time (page is fetching or fetched). 
 */
JC_RETCODE jdi_DEGetPageContent (JC_HANDLE				vHandle,
								 ST_DE_PAGE_CONTENT		*pstPageContent) ;

/**
 * @brief		Gets the information of the focussed control
 * 
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[out]  pstCtrlInfo The API will return the focussed controls info in this field.
 * @retval		JC_OK if the browser returns the information of the focussed control
 *				sucessfully
 * @retval		JC_ERR_INVALID_PARAMETER if none of the controls are currently focussed.
 *
 * The application needs to call this API to get the information of the currently
 * focussed control. The API will return the control information only if control 
 * is in the visible area.
 *
 * @note The control info will be invalid once focus shifts from the control.
 */
JC_RETCODE jdi_DEGetFocusedControlInfo (JC_HANDLE				vHandle,
										ST_DE_CONTROL_INFO		*pstCtrlInfo) ;

/**
 * @brief		Gets mime information of the focussed control.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[out]  pstCtrlMimeInfo The API will return the mime info in this field.
 * @retval		JC_OK if the browser returns the mime information of the focussed control
 *				sucessfully
 * @retval		JC_ERR_INVALID_PARAMETER if none of the controls are currently focussed
 *				or the focussed control does not have mime information (only link and 
 *			    object will have associated mime information)
 *
 * The application needs to call this API to get the mime information of the currently
 * focussed control. 
 *
 * Browser will return the mime information only if the control is in the visible area
 * and the control is either an image or an object.
 *
 * @note The control mime info will be invalid once focus shifts from the control. 
 *
 */
JC_RETCODE jdi_DEGetFocusedControlMimeInfo (JC_HANDLE			vHandle,
											ST_DE_URL_CONTENT	*pstCtrlMimeInfo) ;

/**
 * @brief		Gets the type for the focused button control.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[out]	pstButtonType The API will return the type of the button control in this field.
 * @retval		JC_OK  If the browser returns the type of the focused button successfully.
 * @retval		JC_ERR_INVALID_PARAMETER If the button control is not currently focused.
 *
 * The application needs to call this API to get the type of the button control which is
 * currently focused.
 */
JC_RETCODE jdi_DEGetFocusedButtonType (JC_HANDLE			vHandle,
									   ST_DE_BUTTON_TYPE	*pstButtonType) ;

/**
 * @brief	    Gets focussed link info.
 * 
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[out]  ppmURI The API will return the URL in this field.
 * @retval		JC_OK if the browser returns the Focussed link info sucessfully
 * @see			jdi_DEFreeFocusedLinkInfo
 *
 * The application needs to call this API to get the URL associated with the currently focussed link. 
 * The API will return the URL only if the focussed link is in the visible area.
 *
 * @note The application needs to free the memory (using jdi_DEFreeFocusedLinkInfo)
 * associated with pmURL. 
 *
 */
JC_RETCODE jdi_DEGetFocusedLinkInfo (JC_HANDLE			vHandle,
									 JC_CHAR			**ppmURI) ;

/**
 * @brief	    Frees focused link info.
 * 
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]   pmURI Specifies the focused link info which is to be freed.
 * @retval		void
 *
 * The application needs to use this API to free the focused control info which was
 * obtained previously, using the API jdi_DEGetFocusedLinkInfo.
 *
 */
void jdi_DEFreeFocusedLinkInfo (JC_HANDLE		vHandle,
								JC_CHAR			*pmURI) ;

/**
 * @brief		Gets focussed object's type.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[out]  peType The API specifies the type of object in this parameter.
 * @retval		JC_OK if the current focussed control is an object 
 * @retval      JC_ERR_INVALID_PARAMETER if the current focussed control is not in the
 *				visible area or if the current focussed control is not an object
 *
 * The application needs to call this API to get the object type of the 
 * currently focussed object.
 *
 * Browser will identify the object type only if the focussed control is in the visible area and
 * the control is an object.
 */
JC_RETCODE jdi_DEGetFocusedObjectType (JC_HANDLE			vHandle,
									   E_OBJECT_MIME_TYPE	*peType) ;

/**
 * @brief		Refreshes the current display of the active browser.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @retval		JC_OK if the browser display is refreshed successfully.
 * @retval		JC_ERR_INVALID_PARAMETER if there are no controls in the visible area.
 *
 * The application needs to call this API to refresh the content displayed on the 
 * active browser. The browser will redraw only the visible area of the browser 
 * (and not the entire page).
 */
JC_RETCODE jdi_DERefreshDisplay (JC_HANDLE		vHandle) ;

/**
 * @brief		Gets the list of Link elements
 *
 * @param[in]	vHandle	Specifies the handle returned by DE initialize.
 * @param[out]	ppstPageLinkList Specifies the list of link elements of the page
 * @retval		void
 *
 * The application needs to use this API to get the list of Link elements,
 * if any, in the downloaded page. The link elements would then be displayed as menu
 * items for the Links menu. 
 */
void jdi_DEGetPageLinkList (JC_HANDLE					vHandle,
							ST_DE_PAGE_LINK_LIST		**ppstPageLinkList) ;

/**
 * @brief		Activates a link
 *
 * @param[in]	vHandle		Specifies the handle returned by DE initialize.
 * @param[out]	vTagHandle	Reference to the activated link element
 * @retval		void
 *
 * When a link, from the links displayed as menu options, is selected, the application
 * needs to use this API to pass information about the selected link to the
 * browser, so that appropriate action can be taken
 */
JC_RETCODE jdi_DEActivateLinkElement (JC_HANDLE		vHandle,
									  JC_HANDLE		vTagHandle) ;

/**
 * @brief		Activates an anchor element.
 *
 * @param[in]	vHandle		Specifies the handle returned by DE initialize.
 * @param[out]	vTagHandle	Reference to the activated anchor element
 * @retval		JC_OK		Success on activation of the selected anchor.
 * @retval		Others		Failure on activation of the selected anchor.
 *
 * When a particular anchor is selected from the displayed list, the application
 * needs to use this API to pass information about the selected anchor to the
 * browser, so that appropriate action can be taken
 */
JC_RETCODE jdi_DEActivateAnchorElement (JC_HANDLE		vHandle,
										JC_HANDLE		vTagHandle) ;

/**
 * @brief		Redraws the entire page that is currently displayed
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @retval		JC_OK if the redraw is done successfully.
 * @retval		JC_ERR_INVALID_PARAMETER if no page is currently displayed on the browser.
 *
 * The application needs to call this API to redraw the page displayed on the 
 * browser. The browser will redraw the visible and the non-visible area of the page 
 * (for example when the display mode is changed).
 */
JC_RETCODE jdi_DERedrawPage (JC_HANDLE		vHandle) ;

/**
 * @brief		Sets the focus on the current focusable control in the displayed page.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @retval		JC_OK if the focus is set successfully.
 * @retval		JC_ERR_INVALID_PARAMETER if no page is currently displayed on the browser.
 *
 * The application needs to call this API to set focus on the current focusable control.
 * This function is called at the time of alert/confirmation/prompt messages which need to
 * set the focus on the focusable control after executing some script related functionality.
 */
JC_RETCODE jdi_DESetCurrentFocus (JC_HANDLE	vHandle) ;

/**
 * @brief	    Reloads a page.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @retval		JC_OK if the page is reloaded successfully.
 * @retval		JC_ERR_INVALID_PARAMETER if currenly no page is displayed on the browser.
 *
 * The application needs to use this API to reload the page that is currently displayed or
 * currently being displayed. The browser will give a fresh fetch request while reloading 
 * (instead of just performing a redraw operation).
 */
JC_RETCODE jdi_DEReloadPage (JC_HANDLE		vHandle) ;

/**
 * @brief		Handles the navigation between pages.
 * 
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	eType Specifies the type of navigation (forward or backward)
 * @retval		JC_OK if navigation is done successfully
 * @retval		JC_ERR_INVALID_PARAMETER if invalid navigation type is specified.
 *
 * The application needs to call this API when it needs to navigate between pages. The 
 * eNavType will specify whether the forward or backward navigation has to be done.
 */
JC_RETCODE jdi_DEHandleNavigation (JC_HANDLE				vHandle, 
								   E_DE_NAVIGATION_TYPE		eType) ;

/**
 * @brief		Gets the Anchor Count
 * 
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[out]	puiCount Returns the number of anchor node in the displayed page.
 * @retval		JC_OK If the anchor count is returned successfully.
 * @see			jdi_DEGetAnchorList
 *
 * The application needs to call this API to get the count of anchor elements
 * in the page that is currently displayed on the browser.
 */
JC_RETCODE jdi_DEGetAnchorCount (JC_HANDLE				vHandle,
								 JC_UINT32				*puiCount) ;

/**
 * @brief		Gets the list of anchor elements
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	uiStartIndex Specifies the index from which the anchor information is needed.
 * @param[in]	uiCount Specifies the number of anchor element whose information is to be 
 *				returned in the API.
 * @param[out]	pstAnchorData Specifies the field in which the anchor information is returned.
 * @retval		JC_OK if anchor data list is returned successfully.
 * @see			jdi_DEFreeAnchorList
 *
 * @note		This API can be called while the page is getting downloaded or after the 
 *			    page is completely downloaded.
 *
 * The application needs to call this API to get the list of anchor elements that are
 * displayed on the browser. The API will return the anchor list in ppstAnchorData.
 *
 * The application has to first get the total count of anchor elements in the page using
 * the API jdi_DEGetAnchorCount. Then based on the total count the application needs
 * to decide the index from which the anchor elements needs to be retrieved. The number of
 * anchor elements that needs to be returned in pstAnchorData is to be specified in 
 * uiCount.
 *
 */
JC_RETCODE jdi_DEGetAnchorList (JC_HANDLE				vHandle,
								JC_UINT32				uiStartIndex,
								JC_UINT32				uiCount,
								ST_DE_ANCHOR_DATA		*pstAnchorData) ;

/**
 * @brief		Frees the anchor list
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	pstAnchorData Specifies the anchor list to be freed. 
 * @retval		void
 *
 * The application needs to use this API to free the anchor list which was
 * obtained previously, using the API jdi_DEGetAnchorList.
 */
void jdi_DEFreeAnchorList (JC_HANDLE				vHandle,
						   ST_DE_ANCHOR_DATA		*pstAnchorData) ;

/**
 * @brief		Zooms the content that is currently displayed.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	uhZoomValue Specifies the zoom value (percentage of zoom to be done)
 * @retval		JC_OK if the content displayed is zoomed.
 *
 * The application needs to zoom the contents that are displayed on the browser window.
 */
JC_RETCODE jdi_DEZoomTo (JC_HANDLE			vHandle,
						 JC_UINT16			uhZoomValue) ;


/**
 * @brief		Toggles the display of the current page between thumbnail view and normal view
 *
 * @param[in]	vHandle		 Specifies the handle returned by DE initialize.
 * @param[in]	uhZoomValue	 Specifies Zoom factor to be applied for thumbnail mode.
 * @retval		JC_OK if the page view is set properly.
 *
 * The application can set the zoom factor for thumbnail mode. If zoom factor is zero, this function
 * calculates the zoom factor based on page size and visible size so that the content gets fit 
 * within visible area for thumbnail view.
 */
JC_RETCODE jdi_DESetThumbnailView (JC_HANDLE	vHandle,
								   JC_UINT16	uhZoomValue) ;

/**
 * @brief		Resizes all the browser windows.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	uiWidth Specifies the width of the resized browser window
 * @param[in]	uiHeight Specifies the height of the resized browser window.
 * @retval		JC_OK if all the browser windows are resized successully.
 *
 * The application needs to use this API to resize all the browser windows.
 * 
 * @note The application needs to move the window to the specific dimension 
 *		 before calling this fucntion.
 */
JC_RETCODE jdi_DESizeTo (JC_HANDLE		vHandle,
						 JC_UINT32		uiWidth,
						 JC_UINT32		uiHeight) ;

/** 
 * @brief		Checks for the mime type whether supported or not.
 *
 * @param[in]	vHandle    Specifies the handle returned by DE initialize.
 * @param[in]	pcMimeType Specifies the Mime type to be checked.
 * @param[in]	uiSize     Specifies the size of the media whose Mime type to be verified.
 * @param[in]   eRequest   Specifies the request type
 * @param[out]	peType     Specifies the reference to the mime types.
 * @retval		JC_OK if the mime type of the media is supported
 * 
 * The application needs to call this API when ever the Mime type of the media object
 * needs to be verified whether it is been supported by the browser or not.
 */
JC_RETCODE jdi_DEIsMimeTypeSupported (JC_HANDLE				vHandle,
									  JC_INT8				*pcMimeType,
									  JC_UINT32				uiSize,
									  E_DE_REQUEST_TYPE		eRequest,
									  E_DE_MIME_TYPE		*peType) ;

/**
 * @brief		Gets the content for the passed URI
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	pmURI Specifies the URI whose content to be got.
 * @param[out]	pstContent Specifies the reference to the URI content.
 * @retval		JC_OK Success on getting the content
 * @retval		JC_ERR_DE_REDIRECTED When the redirection flag is set.
 * 
 * The application needs to call this API when the content for a parsed URI is got 
 * and is stored in the reference pointer which is larer on made use.
 */
JC_RETCODE jdi_DEGetContent (JC_HANDLE				vHandle,
							 JC_CHAR				*pmURI,
							 ST_DE_URL_CONTENT		*pstContent) ;

/**
 * @brief		Sets the focus to next visible control.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @retval		JC_OK always
 *
 * The application needs to call this API to set the focus to next visible &
 * selectable control (if any) in the current view.
 */
JC_RETCODE jdi_DESetNextVisibleFocus (JC_HANDLE				vHandle) ;

#ifdef BROWSER_AJAX_ENABLE

/**
 * @brief		Sets the reponse headers received.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	pstHeaderInfo Reference to the header received.
 * @retval		JC_OK Success on receiving the headers.
 * @retval		Other appropriate error codes for the further functions called.
 *
 * Once a request is given for fetching, the page progress listener of the application 
 * (E_DE_PAGE_PROGRESS_LISTENER) is called. Once the content is received, if the request 
 * is an Ajax request then the status will be intimated to BOM layer.
 */
JC_RETCODE	jdi_DESetHeaderIndication (JC_HANDLE			vHandle,
									   ST_DE_HEADER_INFO	*pstHeaderInfo) ;

/**
 * @brief This function sets the focus to the next visible selectable control 
 * @param[in]  vHandle    Display engine handle
 * @return Returns JC_OK always
 */
JC_RETCODE jdi_DEAjaxSetAuthInfo (JC_HANDLE				vHandle, 
								  ST_DE_AUTH_INFO		*pstAuthInfo,
								  E_DE_CALLBACK_TYPE	eCallBackType) ;

#endif

/**
 * @brief		The application needs to call this API to deinitialize the display engine.
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @retval		void
 *
 * The application needs to call this API to deinitialize the display engine.
 */
JC_RETCODE jdi_DEDeinitialize (JC_HANDLE 				vHandle) ;

/*** Saving of Background Image - Need to work on this **/

/**
 * @brief		Executes a read PPP. 
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	eType Specifies the non-context read PPP type
 * @param[in]	vPPPArg Specifies the PPP argument.
 * @retval		JC_OK if non-context read PPP is executed successfully
 * @retval		JC_ERR_DE_PLUGIN_NOT_FOUND if the plug in to execute the read PPP is not found.
 *
 * The application needs to use this API to execute a read PPP. The API first 
 * searches for the plug in function for read PPP using the plug in type eType. If the plug
 * in function is found then it is called using the PPP argument vPPPArg
 */
JC_RETCODE jdi_DEExecuteReadPPP (JC_HANDLE				vHandle,
							     E_PPP_READ_PLUGIN		eType,
								 JC_HANDLE				vPPPArg) ;

/**
 * @brief		Executes a read write PPP.
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @param[in]	eType Specifies the context read write PPP type
 * @param[in]	vPPPArg Specifies the PPP argument.
 * @retval		JC_OK if context read write PPP is executed successfully
 * @retval		JC_ERR_DE_PLUGIN_NOT_FOUND if the plug in to execute the read write PPP is not found.
 *
 * The application needs to use this API to execute a read write PPP. The API first 
 * searches for the plug in function for read write PPP using the plug in type eType. If the plug
 * in function is found then it is called using the PPP argument vPPPArg
 */
JC_RETCODE jdi_DEExecuteReadWritePPP (JC_HANDLE						vHandle,
								      E_PPP_READWRITE_PLUGIN		eType,
									  JC_HANDLE						vPPPArg) ;

/** 
 * @brief This function executes the read write upp based on its type
 * @param[in] vHandle  Display engine handle
 * @param[in] eType    Context Read Write UPP Type
 * @param[in] vUPPArg  Reference to the UPP Argument
 * @retval JC_OK  Success on executing 
 * @retval JC_ERR_DE_PLUGIN_NOT_FOUND Plugin is not registered for this type
 * @retval Others Error on executing
 */
JC_RETCODE jdi_DEExecuteReadWriteUPP (JC_HANDLE						vHandle,
									  E_UPP_READWRITE_PLUGIN		eType,
									  JC_HANDLE						vUPPArg) ;

/**
 * @brief		Suspends the display engine
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @retval		JC_OK if the suspend operation is successful.
 * @retval		Other On error.
 *
 * The application needs to call this API to suspend the display engine. 
 * After this no requests will be processed and user actions will not be 
 * processed until it is resumed again using #jdi_DEResume is called.
 */
JC_RETCODE jdi_DESuspend (JC_HANDLE		vHandle) ;

/**
 * @brief		Resumes the display engine
 *
 * @param[in]	vHandle Specifies the handle returned by DE initialize.
 * @retval		JC_OK if the resume operation is successful.
 * @retval		Others On error.
 *
 * The application needs to call this API to resume the display engine which is 
 * already suspended. After this display engine will start its normal processing.
 */
JC_RETCODE jdi_DEResume (JC_HANDLE		vHandle) ;

/**
 * @brief This function takes the index as input parameter and moves back or front in navigation depending on 
 *								index value i.e., back if index negative and front if index is positive
* @param[in]	vHandle			Specifies the handle returned by DE initialize.
 * @param[in]	hIndex			Index value for navigation
 * retVal		returns JC_OK if successs else appropriate error.
**/
JC_RETCODE	jdi_DENavigateByIndex (JC_HANDLE					vHandle,
								   JC_INT16						hIndex) ;

/**
 * @brief This function gets the navigation handle
 * @param[in]	vHandle				Specifies the handle returned by DE initialize.
 * @param[in, out]	pvNavgHandle	Specifies the handle to hold the navigation handle.
 * retVal		returns JC_OK if successs else appropriate error.
**/
JC_RETCODE	jdi_DEGetNavigationHandle (JC_HANDLE		vHandle,
									   JC_HANDLE		*pvNavgHandle) ;

/**
 * @brief This function forms and returns the absolute URI from the relative URI passed
 * @param[in]  vHandle        Display Engine Handle
 * @param[in]  pmRelativeURI  Reference to relative URI
 * @param[out] ppmAbsoluteURI Reference to hold the absolute URI
 * @retval JC_OK  Success on getting the absolute URI
 * @retval Others Error on getting the absolute URI
 */
JC_RETCODE jdi_DEGetAbsoluteURI (JC_HANDLE		vHandle,
								 JC_CHAR		*pmRelativeURI,
								 JC_CHAR		**ppmAbsoluteURI) ;

/**
 * @brief This function frees the absolute URI returned by jdi_DEGetAbsoluteURI 
 * @param[in] vHandle       Display Engine Handle
 * @param[in] pmAbsoluteURI Reference to absolute URI to be freed
 * @return No return value
 */
void jdi_DEFreeAbsoluteURI (JC_HANDLE		vHandle,
							JC_CHAR			*pmAbsoluteURI) ;

/**
 * @brief The error code is returned when chunk wise parsing of the content is finished.	
 */
#define JC_ERR_DE_OK					TERMINAL_ERR_CODE(DE_MODULE, 0x01)
/**
 * @brief The error code is returned when time out is happened and still needs to be continued. 		
 */
#define JC_ERR_DE_CONTINUE				TERMINAL_ERR_CODE(DE_MODULE, 0x02)
/**
 * @brief The error code is returned when the browser is denied access to fetch 
 *		  or show a particular content.	
 */
#define JC_ERR_DE_ACCESS_DENIED			TERMINAL_ERR_CODE(DE_MODULE, 0x03)
/**
 * @brief The error code is returned when the browser comes across a content type which 
 *		  is not supported.
 */
#define JC_ERR_DE_UNKNOWN_CONTENT_TYPE	TERMINAL_ERR_CODE(DE_MODULE, 0x04)
/**
 * @brief The error code is returned when the size of the page to be displayed exceeds the
 *		  maximum size that can be displayed on the browser.		
 */
#define JC_ERR_DE_PAGE_SIZE_EXCEEDS		TERMINAL_ERR_CODE(DE_MODULE, 0x05)
/**
 * @brief The error code is returned when the input provided to the browser is not valid.		
 */
#define JC_ERR_DE_INVALID_INPUT			TERMINAL_ERR_CODE(DE_MODULE, 0x06)
/**
 * @brief The error code is returned when an invalid event is returned by the display engine.
 */
#define JC_ERR_DE_INVALID_EVENT			TERMINAL_ERR_CODE(DE_MODULE, 0x07)
/**
 * @brief The error code is returned when the cache gets expired.		
 */
#define JC_ERR_DE_CACHE_EXPIRED			TERMINAL_ERR_CODE(DE_MODULE, 0x08)
/**
 * @brief The error code is returned when the display property for any element is set as none
 *		  then the style info is freed, later on setting this error code
 */
#define JC_ERR_DE_NONE_DISPLAY			TERMINAL_ERR_CODE(DE_MODULE, 0x09)
/**
 * @brief The error is returned when the information required by the application to display the 
 *		  page is not found.
 */
#define JC_ERR_DE_NOT_FOUND				TERMINAL_ERR_CODE(DE_MODULE, 0x0A)
/**
 * @brief The error code is returned when no action is handled.		
 */
#define JC_ERR_DE_NO_ACTION				TERMINAL_ERR_CODE(DE_MODULE, 0x0B)
/**
 * @brief The error is returned when a redirection flag for a particulat URI is set, then the 
 *		  redirected URI is considered and fetched.
 */
#define JC_ERR_DE_REDIRECTED			TERMINAL_ERR_CODE(DE_MODULE, 0x0C)
/**
 * @brief The error code is returned when a particular URL is getting redirected more than 
 *		  the maximum of redirections allowed by the browser.		
 */
#define JC_ERR_DE_EXCEEDS_MAX_REDIRECT  TERMINAL_ERR_CODE(DE_MODULE, 0x0D)
/**
 * @brief The error specifies the HTTP status.		
 */
#define JC_ERR_DE_HTTP_STATUS			TERMINAL_ERR_CODE(DE_MODULE, 0x0E)
/**
 * @brief The error specifes that the page recieved is an empty page		
 */
#define JC_ERR_DE_EMPTY_PAGE			TERMINAL_ERR_CODE(DE_MODULE, 0x0F)
/**
 * @brief The error code is returned when nested tables (table within table) occures in 
 *		  any page.
 */
#define JC_ERR_DE_NESTED_TABLE			TERMINAL_ERR_CODE(DE_MODULE, 0x10)
/**
 * @brief The error code is returned when the maximum number of interpage is reached for any page.
 */
#define JC_ERR_DE_MAX_INTERPAGE			TERMINAL_ERR_CODE(DE_MODULE, 0x11)
/**
 * @brief The error is returned when the scheme is not supported.		
 */
#define JC_ERR_DE_SCHEME_NOT_SUPPORTED	TERMINAL_ERR_CODE(DE_MODULE, 0x12)
/**
 * @brief The error code is returned when the input field in empty in such a case where 
 *		  input field has to have some input.
 */
#define JC_ERR_DE_INPUT_REQUIRED		TERMINAL_ERR_CODE(DE_MODULE, 0x13)
/**
 * @brief The error code is returned when the form element format, handling of the data,
 *		  forming the query is invalid.	
 */
#define JC_ERR_DE_INVALID_FORM			TERMINAL_ERR_CODE(DE_MODULE, 0x14)
/**
 * @brief The error code is returned when the page is neither in any of the states of 
 *		  refreshing, reloading or redrawing.	
 */
#define JC_ERR_DE_INVALID_STATE			TERMINAL_ERR_CODE(DE_MODULE, 0x15)
/**
 * @brief The error code is returned when an header of any page has non-cachable content.
 */
#define JC_ERR_DE_SAVE_NOT_ALLOWED		TERMINAL_ERR_CODE(DE_MODULE, 0x16)
/**
 * @brief The error code is returned if the application reqeusts for a particular history record
 *		  and the record is not found.		
 */
#define JC_ERR_DE_HISTORY_NOT_FOUND		TERMINAL_ERR_CODE(DE_MODULE, 0x17)
/**
 * @brief The error code is returned if the history is full.		
 */
#define JC_ERR_DE_HISTORY_FULL			TERMINAL_ERR_CODE(DE_MODULE, 0x18)
/**
 * @brief The error code is returned when Plugin is not registered for this type.	
 */
#define JC_ERR_DE_PLUGIN_NOT_FOUND		TERMINAL_ERR_CODE(DE_MODULE, 0x19)
/**
 * @brief The error is returned when the displayed page cannot be saved 
 *       (When displayed page is an error page).
 */
#define JC_ERR_DE_SAVE_INVALID_PAGE		TERMINAL_ERR_CODE(DE_MODULE, 0x1A)
/**
 * @brief The error is returned when there is no memory to copy the content
 *        passed through jdi_DESetContent. 
 */
#define JC_ERR_DE_NO_MEMORY				TERMINAL_ERR_CODE(DE_MODULE, 0x1B)
/**
 * @brief The error is returned when there is not pending request to be stopped
 */
#define JC_ERR_DE_NO_REQUEST_TO_STOP	TERMINAL_ERR_CODE(DE_MODULE, 0x1C)
/**
 * @brief The error is returned if an API is not processed because the content is still
 * being rendered and without rendering fully the API cannot be executed.
 */
#define JC_ERR_DE_RENDERING_IN_PROGRESS TERMINAL_ERR_CODE(DE_MODULE, 0x1D)
/**
 * @brief The error is returned if an event is passed from application 
 * to DE when it is in suspended state.
 */
#define JC_ERR_DE_IN_SUSPENDED_STATE	TERMINAL_ERR_CODE(DE_MODULE, 0x1E)
/**
 * @brief The error is returned if the page preview (thumbnail mode) cannot be set 
 * based upon the ratio factor calculated
 */
#define JC_ERR_DE_NO_PAGE_PREVIEW		TERMINAL_ERR_CODE(DE_MODULE, 0x1F)

#ifdef DE_SCRIPT_ENABLE
/**
 * @brief The error is returned when the script is expecting a user action. 
 */
#define JC_ERR_DE_SCRIPT_CONT			TERMINAL_ERR_CODE(DE_MODULE, 0x30)
/**
 * @brief The error is returned when the page is not fully fetched and
 *		  try to trigger any event or select an anchor with javascript scheme.
 */
#define JC_ERR_DE_SCRIPT_IN_PROCESS		TERMINAL_ERR_CODE(DE_MODULE, 0x31)
#endif
/**
 * @brief The error is returned when ever stck overflow happens
 */

#define JC_ERR_DE_STACK_OVERFLOW		TERMINAL_ERR_CODE(DE_MODULE, 0x32)	

#ifdef BROWSER_WINDOW_OPEN
/**
 * @brief The error code specifies that the passed window has been processed.
 */
#define JC_ERR_DE_WINDOW_PROCESSED		JC_ERR_BE_WINDOW_PROCESSED

/**
 * @brief The error code specifies that the passed window has been closed.
 */
#define JC_ERR_DE_WINDOW_CLOSED			TERMINAL_ERR_CODE(DE_MODULE, 0x41)

/**
 * @brief The error code specifies that the application has already reached the maximum window
 * count and that it cannot create any more new windows.
 */
#define JC_ERR_DE_MAX_WINDOW_COUNT		JC_ERR_BE_MAX_WINDOW_COUNT 

#endif

/**
 * @brief The error code (JC_ERR_DE_WTAI_INVALID_PB_NAME) specifies that wtai add phone 
 * book failed because the name parameter is unacceptable or too long.
 */
#define JC_ERR_DE_WTAI_INVALID_PB_NAME			JC_ERR_BE_WTAI_INVALID_PB_NAME
/**
 * @brief The error code (JC_ERR_DE_WTAI_INVALID_PB_NUMBER) specifies that wtai add phone 
 * book failed because the number parameter is too long.
 */
#define JC_ERR_DE_WTAI_INVALID_PB_NUMBER		JC_ERR_BE_WTAI_INVALID_PB_NUMBER
/**
 * @brief The error code (JC_ERR_DE_WTAI_INVALID_PB_ENTRY) specifies that wtai add phone 
 * book failed because the phone book name is invalid.
 */
#define JC_ERR_DE_WTAI_INVALID_PB_ENTRY			JC_ERR_BE_WTAI_INVALID_PB_ENTRY
/**
 * @brief The error code (JC_ERR_DE_WTAI_INVALID_PB_FULL) specifies that wtai add phone book 
 * failed because the phone book entry could not be written.
 */
#define JC_ERR_DE_WTAI_INVALID_PB_FULL			JC_ERR_BE_WTAI_INVALID_PB_FULL
/**
 * @brief The error code (JC_ERR_DE_WTAI_UNSPECIFIED) specifies that wtai add phone book failed 
 * because of some unspecified error.
 */
#define JC_ERR_DE_WTAI_UNSPECIFIED				JC_ERR_BE_WTAI_UNSPECIFIED
/**
 * @brief The error code (JC_ERR_DE_WMLS_NETWORK_NOT_AVAILABLE) specifies that wmls make call
*  failed because the network is not available.
 */
#define JC_ERR_DE_WMLS_NETWORK_NOT_AVAILABLE	JC_ERR_BE_WMLS_NETWORK_NOT_AVAILABLE
/**
 * @brief The error code (JC_ERR_DE_WMLS_CALLED_PARTY_NOT_ANSWER) specifies that wmls make 
 * call failed because the party (the person being called) is not answering.
 */
#define JC_ERR_DE_WMLS_CALLED_PARTY_NOT_ANSWER	JC_ERR_BE_WMLS_CALLED_PARTY_NOT_ANSWER
/**
 * @brief The error code (JC_ERR_DE_WMLS_CALLED_PARTY_BUSY) specifies that wmls make call 
 * failed because the party (the person being called) is busy.
 */
#define JC_ERR_DE_WMLS_CALLED_PARTY_BUSY		JC_ERR_BE_WMLS_CALLED_PARTY_BUSY

#ifdef __cplusplus
}
#endif

#endif


