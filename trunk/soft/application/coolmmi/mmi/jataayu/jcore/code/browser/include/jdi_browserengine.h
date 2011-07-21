/***************************************************************************
 * $Id: jdi_browserengine.h,v 1.208 2009/05/27 06:59:17 kavithadevhtmlbrow Exp $
 * $Revision: 1.208 $
 * $Date: 2009/05/27 06:59:17 $
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
 * $Log: jdi_browserengine.h,v $
 * Revision 1.208  2009/05/27 06:59:17  kavithadevhtmlbrow
 * Doxygen comments updated
 *
 * Revision 1.207  2009/04/29 07:16:40  kavithadevhtmlbrow
 * Doxygen comment updated
 *
 * Revision 1.206  2009/03/04 13:04:12  kavithadevhtmlbrow
 * Doxygen Comments Updated
 *
 * Revision 1.205  2009/02/26 05:14:39  sudhadevhtmlbrow
 
 *
 * Revision 1.204  2009/02/12 14:15:06  arpitadevhtmlbrow
 * Network Confirm respose handling
 *
 * Revision 1.203  2009/01/21 14:32:13  kavithadevhtmlbrow
 * Popup Blocker for Window.Open
 *
 * Revision 1.202  2009/01/20 07:45:54  kavithadevhtmlbrow
 * Get and Free Absolute URI
 *
 * Revision 1.201  2009/01/06 07:27:27  sriramdevhtmlbrow
 * New member iMaxSocketAliveTime added in set option to configure the persistent socket alive time at run time.
 *
 * Revision 1.200  2008/12/19 07:50:27  prabhatdevhtmlbrow
 
 *
 * Revision 1.199  2008/12/17 12:00:24  kavithadevhtmlbrow
 * window.open - updated with window count and window.closed and other issues
 *
 * Revision 1.198  2008/12/16 07:23:53  kavithadevhtmlbrow
 * DE_WINDOW_OPEN_ENABLE to BROWSER_WINDOW_OPEN
 *
 * Revision 1.197  2008/12/12 09:14:30  sriramdevhtmlbrow
 * Type error in comment fixed.
 *
 * Revision 1.196  2008/12/10 14:46:16  kavithadevhtmlbrow
 * window.open Support - DE_WINDOW_OPEN_ENABLE
 *
 * Revision 1.195  2008/11/18 12:28:40  kavithadevhtmlbrow
 * Setting Default Encoding from Property
 *
 * Revision 1.194  2008/11/13 07:27:35  sudhadevhtmlbrow
 * GetNavigationHandle updated.
 *
 *
 ***************************************************************************/

/**
 * @mainpage
 *
 * @version 1.13
 * jB5 Interface Description
 *
 * @section introduction Introduction
 *
 * jB5, Jataayu's HTML browser product is architected to extend the excellence of its earlier 
 * WAP 1.x and WAP 2.0 browsers to the HTML world. jB5 can be delivered to meet, where required, 
 * WAP 1.x only, WAP 2.0, HTML or full Web browser functionality in increasing level of sophistication.
 * It meets requirements from leading operators around the world, while continuing to deliver 
 * powerful web browsing functions. This lets handset manufacturers use a single browser to meet all 
 * their requirements. 
 *
 * Jataayu has defined a rendering mechanism, <b>Adaptive Rendering Technology - ART®</b>, which provides
 * the most usable adaptation of web pages for mobile phones. ART® provides the flexibility to select
 * a rendering mechanism best suited for the variety of device capabilities in use today and also 
 * provide for a continuous enhancement of adaptation algorithms.
 *
 * External content handlers can be easily integrated to quickly add support for additional content 
 * types including video, PDF's and Flash. jB5 is architected to meet demanding platform requirements
 * of co-operative multi-tasking from low-mid end processor speeds.
 *
 * @subsection features jB5 Key Features
 *
 * @li <b>Multiple browsers in one:</b> 
 * jB5 packs the operator requirements of full WAP support and consumer needs for HTML support, 
 * in one efficient package. Extensive support for both OMA and W3C standards makes jB5 the ideal
 * Web browser for Mobile devices.
 *
 * @li <b>Extensible, powerful plugin support:</b>
 * jB5 supports multiple levels of plugins to let platform providers, ODMs, OEMs and handset 
 * manufacturers to harness the power of the browser to other applications as well as extend the
 * browser to include additional features. 
 * 
 * @li <b>Secure:</b>
 * Support for full transport level security algorithms WTLS, SSL and TLS, as well as support for
 * class 1, 2 and 3 secure authentication mechanisms, deliver maximum security to applications.
 * 
 * @li <b>Multi-mode adaptation:</b>
 * jB5 provides ART based, very usable, fit-to-display-width rendering which allows page viewing 
 * without horizontal scrolling at all. It also provides Natural mode rendering to see screens in 
 * a Virtual Desktop view. jB5 also has a Text Only mode which allows some very quick browsing without
 * taking either the time or the bandwidth to download images, when they are not required by the user. 
 * ART provides full flexibility to add more rendering modes, based on the device characteristics.
 * 
 * @li <b>Touch screen support:</b>
 * Full support for touch screen based devices as well as keypad only devices makes jB5 effective on both
 * mobile-phone and PDA form factor devices.
 *
 * @li <b>Operator specifications support:</b>
 * jB5 is built on the JCF® which is the foundation for all Jataayu’s handset products. Apart from
 * basic platform functionality, JCF® enables customers to implement Jataayu’s products on their 
 * handsets without having to worry about meeting standards, clearing conformance tests, and 
 * operator requirements. jB5 has successfully met operator specific requirements of Tier1 
 * carriers in a number of countries in Europe and Asia, including China.
 *
 * @li <b>Supported Media Types:</b><BR><BR>
 * <table border="1">
 * <tr align="center"><td><b>Media</b></td><td><b>Media types</b></td><td><b>Reference</b></td></tr>
 * <tr><td>WML 1.3</td><td><div>text/vnd.wap.wml</div><div>application/vnd.wap.wmlc</div></td><td>WAP-191-WML-20000219-a.pdf</td></tr>
 * <tr><td>WML 2.0</td><td>application/wml+xml</td><td>WAP-238-WML-20010911-a.pdf</td></tr>
 * <tr><td>XHTML MP</td><td><div>application/vnd.wap.xhtml+xm</div><div>application/xhtml+xml</div></td><td>OMA-TS-XHTMLMP-V1_2-20050118-C.pdf</td></tr>
 * <tr><td><div>XHTML 1.1</div><div>HTML 4.01</div><div>cHTML </div></td><td>text/html</td><td><div>REC-xhtml1-20020801</div><div>REC-html401-19991224</div>NOTE-compactHTML-19980209<div></div></td></tr>
 * <tr><td>Multipart Mixed</td><td>application/vnd.wap.multipart.mixed</td><td>RFC 2046</td></tr>
 * <tr><td>Multipart Related</td><td>application/vnd.wap.multipart.related</td><td>RFC 2046</td></tr>
 * <tr><td><div>CSS 1</div><div>CSS 2</div><div>CSS MP</div><div>OMA WCSS</div></td><td>text/css</td><td><div>REC-CSS1-19990111</div><div>REC-CSS2-19980512</div><div>CR-css-mobile-20020725</div><div>OMA-WAP-WCSS-V1_1-20040609-C.pdf</div></td></tr>
 * <tr><td>WMLScript</td><td>application/vnd.wap.wmlscriptc</td><td><div>WAP-193-WMLScript-20001025-a.pdf</div><div>WAP-194-WMLScriptLibraries-20000925 a.pdf</div></td></tr>
 * <tr><td><div>ECMAScript Mobile Profile</div><div>ECMAScript</div></td><td><div>application/ecmascript</div><div>application/x-ecmascript</div></td><td><div>OMA-WAP-ESMP-V1_0-20050614-C.pdf</div><div>Standard ECMA - 262 3rd Edition</div></td></tr>
 * <tr><td>JavaScript</td><td><div>application/javascript</div><div>application/x-javascript</div></td><td>--</td></tr>
 * </table>
 *
 * @li <b>Protocol's supported:</b><BR><BR>
 * The following protocols are supported for communicating to the origin server. <BR>
 * <table border="1">
 * <tr> <td>HTTP 1.0</td> <td>RFC 1945</td></tr>
 * <tr> <td>HTTP 1.1</td> <td>RFC 2616<BR>RFC 2617<BR></td></tr>
 * <tr> <td>OMA WSP 1.2</td> <td>OMA-WAP-TS-WSP-V1_0-20020920-C.pdf</td></tr>
 * </table>
 *
 * @section scope Scope
 * This Documentation gives a detailed description of the interface that has to be used
 * by an application. The documentation clearly describes the usage of each function, the 
 * significance of each variable, usage of the data structures and enumerations.  The
 * document will ease the complexity faced while interfacing the application and the Browser.
 *
 * @section start Getting Started
 * @li @ref jcf
 * @li @ref mmi
 * @li @ref task
 * @li @ref conventions
 * @li @ref integration
 *
 * @section appendix Appendix
 *
 * @li @ref faq
 * @li @ref revhis
 *
 * 
 */

/**
 * @defgroup BrEngine			Browser Engine
 * @defgroup ARR				ARR
 * @defgroup History			History
 * @defgroup PushHandler		Push Routines
 * @defgroup Plugins			Plugins
 * @defgroup BrConfig			Browser Configuration
 * @defgroup DeConfig			Display Engine Configuration
 
 * @defgroup Transport			Transport Task
 */

/**
 * @file jdi_browserengine.h
 * @ingroup BrEngine
 * @brief Defines the exposed enumerations, structures and APIs of the browser engine that
 *		  can be used by the communicator, application and display engine modules.
 * 
 */

#ifndef _JDI_BROWSER_ENGINE_H
#define _JDI_BROWSER_ENGINE_H

/** @cond */
#include <jdi_displayinfo.h>
#include <jdi_mimeinfo.h>
#include <jdi_ppp.h>
#include <jdi_upp.h>
#ifdef JCF_SSL
#include <jdi_ssl.h>
#else
#include <jdi_httpssl.h>
#endif
#ifdef JCF_WTLS
#include <jdi_wtls.h>
#else
#include <jdi_wspwtls.h>
#endif
/** @endcond */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Specifies the maximum host name size 
 */
#define BE_MAX_HOST_SIZE			255

/**
 * @brief Specifies the maximum size of IP address
 */
#define BE_MAX_IP_SIZE				15 

/**
 * @brief Specifies the maximum size of a charset
 */
#define BE_MAX_CHARSET_SIZE				255

/**
 * @brief Specifies the maximum header name size 
 */
#define BE_MAX_HEADER_NAME_SIZE			255

/**
 * @brief Specifies the maximum size of the content type
 */
#define BE_MAX_CONTENT_TYPE_LEN	255

/**
 * @brief Specifies the various browser components
 */

typedef enum e_be_component
{
#ifdef BROWSER_CACHE_ENABLE	
	E_BE_CACHE, /**< Specifies the cache component. Argument to be passed for this component is #ST_BE_CACHE_COMPONENT. */
#endif
#ifdef BROWSER_COOKIE_ENABLE
	E_BE_COOKIE, /**< Specifies the cookie component. Argument to be passed for this component is #ST_BE_COOKIE_COMPONENT */
#endif
#ifdef BROWSER_HISTORY_ENABLE
	E_BE_HISTORY, /**< Specifies the history component. Argument to be passed for this component is #ST_BE_HISTORY_COMPONENT */
#endif
	E_BE_TOTAL_COMPONENT
} E_BE_COMPONENT ;

/** 
 * @brief This enumeration specifies the set of callback types that are invoked by the browser 
 *		  engine to the application. Depending upon the application requirement, it needs to handle it
 */
typedef enum e_be_callback_type
{
	E_BE_FOCUS_LISTENER, /**< Intimates the application about a change in focus. (Argument passed #ST_BE_CONTROL_INFO) */
	E_BE_DO_HANDLER_CALLBACK, /**< Intimates the application to handle the DO element. (Argument passed #ST_BE_PAGE_DO_LIST) */
	E_BE_MULTIMEDIA_CONTENT, /**< Intimates the application to handle the Multimedia content. (Argument passed #ST_BE_MULTIMEDIA_CONTENT) */
	E_BE_FREE_MULTIMEDIA_CONTENT,	/**  Intimates the end of Multimedia content. (No argument passed). **/
#ifdef DE_DRM_ENABLE
	E_BE_DRM_CONTENT_LISTENER, /**< Intimates the application to handle the DRM content. (Argument passed #ST_DRM_EXT_CONTENT) */
	E_BE_DRM_CONTENT_MORE_LISTENER, /**< Intimates the application to handle the more DRM content. (Argument passed #ST_DRM_EXT_CONTENT_MORE) */
	E_BE_DRM_CANCEL_LISTENER, /**< Intimates the application that DRM content need not be sent further. (Argument passed #ST_DRM_EXT_CANCEL) **/
	E_BE_DRM_FREE_LISTENER,	  /**< Intimates the application to free the DRM right objects. (Argument passed #ST_DRM_EXT_FREE) **/
#endif
	E_BE_VSCROLL_LISTENER, /**< Intimates the application that a vertical scroll is requested. (Argument Passed #ST_BE_SCROLL_INFO) */
	E_BE_HSCROLL_LISTENER, /**< Intimates the application that a horizontal scroll is requested (Argument Passed #ST_BE_SCROLL_INFO) */
	E_BE_BEGIN_PAGE_LISTENER, /**< Intimates the application that DE has started displaying a page */
	E_BE_END_PAGE_LISTENER, /**< Intimates the application that Display engine has finished displaying a page (Argument passed #ST_BE_END_PAGE_INFO) */
	E_BE_BEGIN_FETCH_LISTENER, /**< Intimates the application that browser has started processing a fetch request */
	E_BE_END_FETCH_LISTENER, /**< Intimates the application that browser has finished processing a fetch request. */
	E_BE_BEGIN_DISPLAY_LISTENER, /**< Intimates the application that Display engine has started displaying content  */
	E_BE_END_DISPLAY_LISTENER, /**< Intimates the application that Display Engine has finished displaying content */
	E_BE_TITLE_LISTENER, /**< Intimates the application about the title of the page being fetched. (Argument passed is the reference to "JC_CHAR" string) */
	E_BE_ICON_LISTENER, /**< Intimates the application about the icon URI that it needs to download to get the icon of the page being downloaded (Argument passed is the reference to "JC_CHAR" icon URI string)*/
	E_BE_AUTHORIZATION_LISTENER, /**< Intimates the application to handle the authorization (Argument passed #ST_BE_PAGE_INFO)*/
	E_BE_PROXYAUTHORIZATION_LISTENER, /**< Intimates the application to handle the proxy authorization (Argument passed #ST_BE_PAGE_INFO) */
#ifdef BROWSER_AJAX_ENABLE
	E_BE_AJAX_AUTH_LISTENER, /**< Intimates the application to handle the authorization for an ajax request (Argument passed #ST_BE_PAGE_INFO)*/
	E_BE_AJAX_PROXYAUTH_LISTENER, /**< Intimates the application to handle the proxy authorization for an ajax request (Argument passed #ST_BE_PAGE_INFO) */
#endif
	E_BE_PAGE_PROGRESS_LISTENER, /**< Intimates the application about the contents of the page request received (Argument #ST_BE_PAGE_PROGRESS) */
	E_BE_PROGRESS_LISTENER, /**< Intimates the application about the content received from the lower layers (Argument #ST_BE_PROGRESS_IND) */
	E_BE_TIMER_LISTENER, /**< Intimates the application about timer event. From this callback type the application can return JC_OK, which specifies that the browser engine can proceed with the 
						      timer handling else return JC_ERR_ASYNC_CALL which specifies that the application wants to release the timer thread and at a later point of time the application
						      will use jdi_BESetEvent (event type #E_BE_TIMER) to continue with the timer (Argument passed #ST_BE_TIMER_RESP). */
#ifdef DE_WMLSCRIPT_ENABLE
	E_BE_SCRIPT_EXEC_CONFIRMATION, /**< Intimates the application to get the user confirmation for executing the WMLscript content. (Argument passed #ST_BE_SCRIPT_EXEC_CONFIRMATION) */
	E_BE_WMLS_ALERT_LISTENER, /**< Intimates the application to handle the WMLS alert (Argument passed #ST_BE_WMLS_ALERT) */		
	E_BE_WMLS_CONFIRM_LISTENER, /**< Intimates the application to handle the WMLS confirm (Argument passed #ST_BE_WMLS_CONFIRM) */
	E_BE_WMLS_PROMPT_LISTENER, /**< Intimates the application to handle the WMLS prompt (Argument passed #ST_BE_WMLS_PROMPT) */
	E_BE_WTAI_MAKE_CALL_LISTENER, /**< Intimates the application to handle WTAI Make call (Argument Passed #ST_BE_WTAI_MAKECALL) */
	E_BE_WTAI_SEND_DTMF_LISTENER, /**< Intimates the application to handle Send DTMF request (Argument Passed #ST_BE_WTAI_SENDDTMF) */
	E_BE_WTAI_PHONE_BOOK_LISTENER, /**< Intimates the application to handle the phonebook related operation (Argument Passed #ST_BE_WTAI_PHONEBOOK) */
#endif
	E_BE_ERROR_LISTENER, /**< Intimates the application about an error in browser operation (Argument Passed #ST_BE_ERROR_LISTENER) */
#ifdef BROWSER_PUSH_ENABLE 
	E_BE_PUSH_LISTENER, /**< Intimates the application about arrival of a push message (Argument Passed #ST_BE_PUSH_IND) */
#endif
	E_BE_PROFILE_CONFIRMATION_IND_LISTENER, /**< Intimates the application about profile confirmation indication (Argument passed #ST_BE_PROFILE_CONFIRMATION_IND) */
	E_BE_SECURITY_IND_LISTENER, /**< Intimates the application about security indication (security switch, success and failure indication etc) (Argument Passed #ST_BE_SECURITY_INDICATION) */
	E_BE_SCHEME_LISTENER, /**< Intimates the application to handle unsupported scheme (Argument Passed #ST_BE_SCHEME_LISTENER)  */
	E_BE_CONTENT_LISTENER, /**< Intimates the application to handle the unsupported content type (Argument Passed #ST_BE_URL_IND) */
	E_BE_DOWNLOAD_LISTENER, /**< Intimates the application to handle the content received for a downalod request (Argument Passed #ST_BE_URL_IND) */
	E_BE_DOWNLOAD_TO_CACHE_LISTENER, /**< Intimates the application about the result of a download to cache request (success or failure) (Argument Passed #ST_BE_DOWNLOAD_TO_CACHE_LISTENER) */
	E_BE_NETWORK_CONFIRM_LISTENER, /**< Intimates the application to get the user confirmation for network (data) connection. This is the blocking call. */
#ifdef BROWSER_SELF_EVENT_DISABLE 
	E_BE_SELF_RETRIEVE_LISTENER, /**< Intimates the application to handle the retrieve cache or file content */	
#endif
#ifdef BROWSER_WEB_SPIDER_ON
	E_BE_BROWSER_WEB_SPIDER_LISTENER, /**< Intimates the application to add the testing URL to the list**/
#endif
#ifdef DE_SCRIPT_ENABLE
	E_BE_BOM_ALERT_LISTENER, /**< Intimates the application to handle the BOM alert (Argument passed #ST_BOM_ALERT) */				
	E_BE_BOM_CONFIRM_LISTENER,	/**< Intimates the application to handle the BOM confirm (Argument passed #ST_BOM_CONFIRM) */
	E_BE_BOM_PROMPT_LISTENER, /**< Intimates the application to handle the BOM prompt (Argument passed #ST_BOM_PROMPT) */
	E_BE_BOM_EXEC_CONFIRM,	/**<Intimates the application to get the user confirmation for executing the ECMAScript or JavaScript (Argument passed #ST_BE_BOM_EXEC_CONFIRM) */
	E_BE_BOM_ABORT_CONFIRM,	/**<Intimates the application to get the user confirmation for aborting the current script (Argument passed #ST_BE_BOM_ABORT_CONFIRM) */
	E_BE_FREE_PROMPT_STRING, /**<Intimates the application to free the prompt string. This callback will be used only for blocking call.(Argument passed is the reference to "JC_CHAR" string)*/
	E_BE_BOM_GET_SCREEN_INFO, /**<Intimates the application to get the screen information (Argument passed #ST_BOM_SCREEN_INFO) */
#endif
#ifdef DE_PPP_READWRITE_WIDGET
	E_BE_WIDGET_UPDATE_MENU, /**< Initimates the application about the updation of widget menus. (Argument passed #ST_PPP_WIDGET_MENU) */
#endif 
#ifdef DE_FILE_UPLOAD_ENABLE
	E_BE_FILE_BROWSE_NAME_LISTENER, /**< Intimates the application for file browse display name (Argument passed #ST_BE_FILE_BROWSE_NAME) */
	E_BE_FILE_BROWSE_LISTENER, /**< Intimates the application to get the file name using browser operation (Argument passed #ST_BE_FILE_BROWSE_INFO) */
#endif
#ifdef BROWSER_IMODE_ATTRIBUTES
	E_BE_UTN_ID_NUMBER_LISTENER, /**< Intimates the application to get the confirmation from the user whether to send the IMEI and ICCID numbers to the server. 
									  If it is implemnted as nonblocking appliaction should send the response using the function #jdi_BESendIDNumberResponse.
									  Application should send the user response using #ST_BE_ID_RESP. Same structure is used for both blocking and nonblocking implementations. */
    E_BE_UTN_ID_VALUE_FREE, /**< Intimates the application to free the user-agent header value that it has returned with the callback #E_BE_UTN_ID_NUMBER_LISTENER. 
								 This callback is used only if the callback #E_BE_UTN_ID_NUMBER_LISTENER is implemented as blocking. (Argument passed #JC_CHAR). */
#endif
#ifdef BROWSER_EXTENDED_SUPPORT
	E_BE_ANCHOR_CONTENT_SUPPORT, /**< Intimates the application to check with the user whether to download the link or not.
									  The given content type is the destination content type of the anchor element selected by the user. (Argument passed #ST_BE_ANCHOR_PARAM_SUPPORT).
									  If it is implemented as non blocking then the response has to be sent using the event #E_BE_ANCHOR_CONTENT_SUPPORT_RESP. */
	E_BE_ANCHOR_CHARSET_SUPPORT, /**< Intimates the application to check with the user whether to download the link or not. 
									  The given charset type is the destination charset type of the anchor element selected by the user. (Argument passed #ST_BE_ANCHOR_PARAM_SUPPORT).
									  If it is implemented as non blocking then the response has to be sent using the event #E_BE_ANCHOR_CHARSET_SUPPORT_RESP. */
#endif
#ifdef BROWSER_IMODE_ATTRIBUTES
	E_BE_PAGE_PERM_REDIRECTION, /**< This callback intimates the application that the page is permanently redirecting to some other URL. */
#endif
#ifdef BROWSER_WINDOW_OPEN
	E_BE_WINDOW_OPEN, /**< Intimates the application to open a new window and create a DE context for the same. This callback shall be made only once BE ascertains that a new window needs to be created. (Argument passed #ST_BE_WINDOW_INFO) **/
	E_BE_WINDOW_CLOSE, /**< Intimates the application to close the window pointed to by the passed windowID. **/
	E_BE_SET_WINDOW_PROPERTIES, /**< Intimates the application to set the display and ARR settings for the new window. **/
	E_BE_SET_ACTIVE_WINDOW, /**< Intimates the application to set the specified window as active window. **/
#endif
	E_BE_URL_SUBMIT_WIZ_LISTENER,   
	E_BE_PPP_READWRITE_LISTENER,  /**< Intimates the application regarding the Page Processor Plugin (PPP) related information like confirmation/prompt/etc. (Argument passed #ST_PPP_READWRITE_REQUEST) */
	E_BE_COOKIE_NOT_ENABLED,	  /**< Intimates the application that there is a cookie header but the cookie is not set because cookie is disabled. */
	E_BE_MAX_TRANSPORT_REQUESTS	   

} E_BE_CALLBACK_TYPE ;

/**
 * @brief Specifies the set of browser options or features that the application wants to enable or
 *		  set. The application will specify this option value. The browser engine needs to call the
 *		  appropriate function to enable or set the option.
 */
typedef enum e_be_set_options
{
	E_BE_SET_DISPLAY_MODE, /**< Used to set the display mode (normal, F2S etc). Argument to passed for this option #ST_BE_DISPLAY_MODE_INFO. */
	E_BE_SET_VISIBILITY, /**< Used to set the visibilty mode (normal, full screen etc). Argument passed for this property is #ST_BE_OPTION_VISIBLE. */
	E_BE_SET_GRAPHICS, /**< Used to set the graphics options. Argument passed to this property is #ST_BE_OPTION_GRAPHICS. */
	E_BE_SET_AUDIO, /**< Used to set the audio options. Argument passed to this property is #ST_BE_OPTION_AUDIO */
	E_BE_SET_IMAGE_SELECTION, /**< Used to specify whether the image can be selected or not. Argument passed to this property is #ST_BE_OPTION_IMAGE_SELECTION.*/
	E_BE_SET_REFERER_HEADER_MODE, /**< Used to specify the way the referer header is to be used for the browser requests. Argument passed to this property is #ST_BE_OPTION_REFERER_HEADER_MODE */
	E_BE_SET_MAGNIFICATION, /**< Used to specify the magnification. Argument passed to this property is #ST_BE_OPTION_MAGNIFICATION */
	E_BE_SET_SCROLL_MODE, /**< Used to specify the scrolling mode. Argument passed to this property is #ST_BE_OPTION_SCROLL_MODE */
	E_BE_SET_DO_DISPLAY, /**< Used to specify whether the browser needs to display the DO functionality. Argument passed to this property is #ST_BE_OPTION_DO_DISPLAY.*/
	E_BE_SET_STYLE,  /**< Used to set the basic style. Argument passed to this property is #ST_BE_OPTION_STYLE */
	E_BE_SET_ARR_RULE, /**< Used to set the ARR rules. Argument passed to this property is #ST_ARR_RULES */
	E_BE_SET_ENCODING,	/**< Used to set the charset encoding, which will override the charset encoding specified by the page . Argument passed to this property is #ST_BE_OPTION_ENCODING */
	E_BE_SET_DEFAULT_ENCODING, /**< Used to set the default charset encoding, which will be used incase the page doesn't specify any charset encoding. 
									This again will be overwritten by the encoding set using #E_BE_SET_ENCODING. Argument passed to this property is #ST_BE_OPTION_ENCODING */
	E_BE_SET_CACHE, /**< Used to specify whether the cache has to be enabled. (#ST_BE_OPTION_CACHE) */
	E_BE_SET_COOKIE, /**< Used to specify whether the cookie has to be enabled. (#ST_BE_OPTION_COOKIE) */
	E_BE_SET_CSS_PROPERTIES, /**< Used to set the CSS Properties. The Author CSS enable, Embedded Style handling and the Supported Media Types can be set through this. The CSS properties set would affect the display from the next URL fetched. */
	E_BE_SET_DEFAULT_CSS, /**< Used to set the default CSS from the application and set it based on the CSS type selected. (#ST_BE_DEFAULT_CSS) */
	E_BE_SET_PERFORMANCE, /**< Used to set the browser performance properites. Argument passed to this property is #ST_BE_PERFORMANCE */
#ifdef DE_SCRIPT_ENABLE
	E_BE_SET_SCRIPT, /**< Used to enable/disable script handling. Argument passed to this property is #ST_BE_OPTION_SCRIPT*/
#endif
	E_BE_SET_APPLICATION_INFO, /**< Used to get the application info. Argument passed to this property is #ST_BE_APPLICATION_INFO*/
	E_BE_SET_URL_SUBMIT_WIZ
} E_BE_SET_OPTIONS ;


/**
 * @brief The enumeration specifies the type of events that will be sent from the application
 *		  to the browser engine. The browser engine will call the appropriate function which 
 *		  will handle the event.
 */
typedef enum e_be_event_type
{
	E_BE_SCROLL_UP = 1100, /**< Used when the page needs to be scrolled up.*/
	E_BE_SCROLL_DOWN, /**< Used when the page needs to be scrolled down. */
	E_BE_SCROLL_LEFT, /**< Used when scroll left needs to be done */
	E_BE_SCROLL_RIGHT, /**< Used when scroll right needs to be done */
	E_BE_SCROLL_NO_FOCUS,	/**< Used for smooth scrollign witout setting focus. (Argument to be passed: #ST_BE_SCROLL_NO_FOCUS) */
	E_BE_SCROLL_NEXT_FOCUS, /**< Used when focus needs to move to the next selectable control. Applicable only in F2S mode */ 
	E_BE_SCROLL_PREV_FOCUS, /**< Used when focus needs to move to the previous selectable control. Applicable only in F2S mode */ 
	E_BE_SCROLL_PREV_BLOCK, /**< Used when focus needs to move to the previous block */ 
	E_BE_SCROLL_NEXT_BLOCK, /**< Used when focus needs to move to the next block */ 
	E_BE_SCROLL_NEXT_FORM_ELEMENT, /**< Used when focus needs to move to the next form element */ 
	E_BE_SCROLL_PREV_FORM_ELEMENT, /**< Used when focus needs to move to the previous form element */ 
	E_BE_PAGE_UP, /**< Used when page up needs to be done */
	E_BE_PAGE_DOWN, /**< Used when page down needs to be done */
	E_BE_PAGE_LEFT, /**< Used when page left needs to be done */
	E_BE_PAGE_RIGHT, /**< Used when page right needs to be done */
	E_BE_PAGE_TOP_LEFT, /**< Used when focus needs to be shifted to top left of the page */
	E_BE_PAGE_TOP_CENTER, /**< Used when focus needs to be shifted to top center of the page */
	E_BE_PAGE_TOP_RIGHT, /**< Used when focus needs to be shifted to top right of the page */
	E_BE_PAGE_CENTER_LEFT, /**< Used when focus needs to be shifted to center left of the page */
	E_BE_PAGE_CENTER_CENTER, /**< Used when focus needs to be shifted to the center of the page */
	E_BE_PAGE_CENTER_RIGHT, /**< Used when focus needs to be shifted to center right of the page */
	E_BE_PAGE_BOTTOM_LEFT, /**< Used when focus needs to be shifter to bottom left of the page */
	E_BE_PAGE_BOTTOM_CENTER, /**< Used when focus needs to be shifted to bottom center of the page */
	E_BE_PAGE_BOTTOM_RIGHT, /**< Used when focus needs to be shifted to bottom right of the page */
	E_BE_ACTION, /**< Used when an action element is to be handled (Argument to be passed : Event data with the action information) */
	E_BE_TIMER, /**< Used when the timer needs to be handled (Argument to be passed : #ST_BE_TIMER_RESP */
	E_BE_DO_EVENT, /**< Used when the DO functionality needs to be handled. */
#ifndef BROWSER_MOUSE_ENABLE
	E_BE_STYLUS, /**< Used when the stylus operation needs to be handled (Argument to be passed #ST_BE_STYLUS_INFO) */
#else
	E_BE_MOUSE_EVENT, /**< Used when the mouse event operation needs to be handled (Argument to be passed #ST_BE_STYLUS_INFO) */
#endif
	E_BE_KEY_EVENT, /**< Used when the key event needs to be handled (Argument to be passed JC_KEY_EVENT, which is defined as part of JCF) */
	E_BE_SELECT_EVENT, /**< Used when the input select event needs to be handled. */
	E_BE_CHANGE_EVENT, /**< Used when the input change event needs to be handled. */
	E_BE_ASYNC_EVENT, /**< Used when the asynchronous event needs to be handled (Argumen to be passed #ST_BE_ASYNC_EVENT_DATA) */
	E_BE_CACHE_CLEAR, /**< Used when the cache needs to be cleared (Argument passed #ST_BE_OPTION_CACHE_CLEAR). */
	E_BE_COOKIE_CLEAR, /**< Used when the cookie needs to be cleared */
	E_BE_HISTORY_CLEAR, /**< Used when the history needs to be cleared */
	E_BE_TRANSPORT_EVENT, /**< Used when the application wants the communicator to handle a event sent from transport (Argument to be passed : Event data containing the information of the transport event to be handled) */
	E_BE_CACHE_RETRIEVE, /**< Used when application needs to retrieve cached content (Argument to be passed : Event Specifying the cache retrieve information) */
	E_BE_CACHE_HEADER_RETRIEVE, /**< Used when application needs to retrieve cached header (Argument to be passed : Event Specifying the cache header retrieve information) */
	E_BE_FILE_RETRIEVE, /**< Used when applicaion needs to retrieve file content (Argument to be passed : Event Specifying the file retrieve information) */
#ifdef DE_FILE_UPLOAD_ENABLE
	E_BE_FILE_BROWSE_RESP,  /**< Used when application needs to send the file name information for browse operation (Arugment to be passed #ST_BE_FILE_BROWSE_INFO) */
#endif
	E_BE_NAVIGATION_CLEAR, /**< Used when application needs to clear the navigation list */
	E_BE_SCRIPT_EXEC_CONFIRM_RESP, /**< #ST_BE_SCRIPT_EXEC_CONFIRMATION_RESP */
#ifdef DE_DRM_ENABLE
	E_BE_DRM_RESPONSE, /** < Used when the application need to send the parsed DRM content to the browser. (Argument to be passed #ST_DRM_EXT_PARSED_CONTENT) */
	E_BE_DRM_RESPONSE_MORE, /** < Used when the application need to send more of the parsed DRM content to the browser. (Argument to be passed #ST_DRM_EXT_PARSED_CONTENT_MORE) */
	E_BE_DRM_RIGHTS_UPDATE, /**< Used when application needs to update the rights object. (Argument passed #ST_DRM_EXT_RIGHTS_UPDATE). */
#endif
#ifdef DE_SCRIPT_ENABLE
	E_BE_BOM_ALERT_RESP, /**< Used when application needs to send the BOM alert response to the browser (Argument to be passed #ST_BOM_ALERT_RESP) */
	E_BE_BOM_CONFIRM_RESP,	/**< Used when applicaiton needs to send the BOM confirm response to the browser (Argument to be passed #ST_BOM_CONFIRM_RESP) */
	E_BE_BOM_PROMPT_RESP, /**< Used when application needs to send the BOM prompt response to the browser (Argument to be passed #ST_BOM_PROMPT_RESP) */
	E_BE_BOM_EXEC_CONFIRM_RESP, /**< Used when the application needs to send the ecmascript ececution confirmation response to the browser (Argument passed #ST_BE_BOM_EXEC_CONFIRM_RESP) */
	E_BE_BOM_ABORT_CONFIRM_RESP, /**< Used when the application needs to send the abort script confirmation response to the browser (Argument passed #ST_BE_BOM_ABORT_CONFIRM_RESP) */
#endif
#ifdef BROWSER_EXTENDED_SUPPORT
	E_BE_ANCHOR_CONTENT_SUPPORT_RESP, /**< Used when the application needs to send the user response for the event #E_BE_ANCHOR_CONTENT_SUPPORT incase of non blocking implementation. (Argment passed #ST_BE_ANCHOR_PARAM_SUPPORT) */
	E_BE_ANCHOR_CHARSET_SUPPORT_RESP, /**< Used when the application needs to send the user response for the event #E_BE_ANCHOR_CHARSET_SUPPORT incase of non blocking implementation. (Argment passed #ST_BE_ANCHOR_PARAM_SUPPORT) */
#endif
#ifdef BROWSER_WINDOW_OPEN
	E_BE_WINDOW_OPEN_CONFIRM_RESP, /**< Used when the application needs to send the user response for a window.open call for a pop-up window (Argument to be passed #ST_BE_WINDOW_OPEN_RESP) */
#endif
	E_BE_WMLS_ALERT_RESP, /**< Used when application needs to send the WMLS alert response to the browser (Argument to be passed #ST_BE_WMLS_ALERT_RESP) */
	E_BE_WMLS_CONFIRM_RESP,	/**< Used when applicaiton needs to send the WMLS confirm response to the browser (Argument to be passed #ST_BE_WMLS_CONFIRM_RESP) */
	E_BE_WMLS_PROMPT_RESP, /**< Used when application needs to send the WMLS prompt response to the browser (Argument to be passed #ST_BE_WMLS_PROMPT_RESP) */
	E_BE_WTAI_MAKE_CALL_RESP, /**< Used when application needs to send WTAI make call response to the browser (Argument to be passed #ST_BE_WTAI_MAKECALL_RESP) */	
	E_BE_WTAI_SEND_DTMF_RESP, /**< Used when application needs to send WTAI make call response to the browser (Argument to be passed #ST_BE_WTAI_SEND_DTMF_RESP) */	
	E_BE_WTAI_PHONE_BOOK_RESP, /**< Used when application needs to send WTAI phone book response to the browser (Argument to be passed #ST_BE_WTAI_PHONEBOOK_RESP) */

	E_BE_NETWORK_CONFIRM_RESP, /**< Used when the application needs to send the response for the network confirmation indication (Argument to be passed #ST_BE_NETWORK_CONFIRM_RESP) */

	E_BE_LOW_MEMORY_INDICATION /**< Used when application needs to indicate the browser on low memory (Argument to be passed #ST_BE_LOW_MEMORY_INFO)*/
} E_BE_EVENT_TYPE ;

/**
 * @brief Specifies the type of navigation.
 */
typedef enum e_be_navigation_type
{
	E_BE_NAVIGATION_BACK, /**< Specifies that backward navigatation is to be done */
	E_BE_NAVIGATION_FORWARD /**< Specifies that forward navigation is to be done */
} E_BE_NAVIGATION_TYPE ; 

/** 
 * @brief Specifies the type of connection mode
 */
typedef enum e_be_connection_mode
{
	E_BE_WSP_CONNECTION_ORIENTED, /**< Specifies WSP connection oriented mode */
	E_BE_WSP_CONNECTION_ORIENTED_SECURE, /**< Specifies secured WSP connection oriented mode */
	E_BE_WPHTTP_PROXY, /**< Specifies HTTP proxy mode */
	E_BE_WPHTTP_DIRECT /**< Specifies HTTP direct mode */
} E_BE_CONNECTION_MODE ;

/**
 * @brief The enumneration defines the various profile confirmation types that can be sent to the
 *		  application 
 */
typedef enum e_be_profile_confirmation_type
{
	E_BE_USE_SECONDARY_CONNECTION_MODE, /**< Specifies that confirmation is required to use secondary connection mode. */
	E_BE_USE_SECONDARY_DATA_CONNECTION, /**< Specifies that confirmation is required to use secondary data connection */
	E_BE_USE_TERTIARY_DATA_CONNECTION, /**< Specifies that confirmation is required to use tertiary data connection */
	E_BE_EX_PROFILE_RETRY /**< Specifies that confirmation is required to start the whole extended profile once again. */
} E_BE_PROFILE_CONFIRMATION_TYPE ;


/** 
 * @brief The application needs to use this enumeration to specify the type of authorization
 *		  list that is to be cleared from the lower layers.
 */
typedef enum e_be_clear_auth_header
{
	E_BE_CLEAR_URI_AUTHORIZATION, /**< Specifies that the URL authorization list is to be cleared */
	E_BE_CLEAR_PROXY_AUTHORIZATION, /**< Specifies that the proxy authorization list is to be cleared */
	E_BE_CLEAR_ALL /** Spcifies that both URI and proxy authorization list is to be cleared. */
} E_BE_CLEAR_AUTH_HEADER ;

/** 
 * @brief Specifies the type of security classes used while setting up WSP Secured connection.
 */

typedef enum e_be_security_class
{
	E_BE_CLASS_II, /**< Specifies that class II security is used */
	E_BE_CLASS_III /**< Specifies that class III security is used */
} E_BE_SECURITY_CLASS ; 

/** 
 * @brief The enumneration specifies the type of push that the application can specify
 *		  while sending the initialize push session request to the communicator.
 */
typedef enum e_be_push_type
{
	E_BE_CONFIRMED_PUSH, /**< Specifies that confirmed push */
	E_BE_HTTP_PUSH /**< Specifies HTTP push */
} E_BE_PUSH_TYPE ;

/** 
 * @brief The browser uses this enumeration to indicate the stack status to the application.
 *		  This stack status actually depicts the status of the lower layer stack.
 */
typedef enum e_be_stack_status
{
	E_BE_STACK_NONE, /**< Specifies that the stack is in NIL state */
	E_BE_STACK_CONNECTING, /**< Specifies that the stack is in connecting state */
	E_BE_STACK_CONNECTED, /**< Specifies that the stack is in connected state */
	E_BE_STACK_SENDING, /**< Specifies that the stack is in sending state */
	E_BE_STACK_RECEIVING, /**< Specifies that the stack is in receiving state */
	E_BE_STACK_RETRYING, /**< Specifies that the stack is in retrying state */
	E_BE_STACK_DISCONNECTING, /**< Specifies that the stack is in disconnecting state */
	E_BE_STACK_DISCONNECTED /**< Specifies that the stack is in disconnected state */
} E_BE_STACK_STATUS ;

/** 
 * @brief This enum is used by the browser while sending the URL indication to the application. The
 *		  enum specifies whether header , buffer or error information is specified in the indication.
 */
typedef enum e_be_ind_type
{
	E_BE_HEADER_INDICATION, /**< Specifies that indication contains header information */
	E_BE_HEADER_INDICATION_ONLY, /**< Specifies that indication contains header information and 
									  also specifies that there won't be buffer indication following this */
	E_BE_BUFFER_INDICATION, /**< Specifies that indication contains buffer information */
#ifdef BROWSER_JUDGMENT_OF_CONTENT
	E_BE_CONTENT_CAPABILITY_CHECK_INDICATION, /** Application should check whether the given content can be handled 
												  or not. If it can be handled it should return JC_OK and if not 
												  return JC_ERR_BE_UNSUPPORTED_CONTENT_TYPE. */
#endif
	E_BE_ERROR_INDICATION /**< Specifies that indication contains error indication */
} E_BE_IND_TYPE ;

#ifdef BROWSER_IMODE_ATTRIBUTES

/**
 * @brief This enum is used by the application to send the user response 
 *		  about sending ID number along with the User-Agent header.
 */
typedef enum e_be_utn_id_resp
{
	E_BE_UTN_ID_SEND, /**< Specifies that ID number can be sent with the User-Agent header */
	E_BE_UTN_ID_DONT_SEND, /**< Specifies that ID number should not be sent with the User-Agent header */
	E_BE_UTN_ID_CANCEL_REQ /**< Specifies that request itself should not be sent */
} E_BE_UTN_ID_RESP ;

/**
 * @brief This structure is used by the application to send the user response 
 *		  about sending ID number along with the User-Agent header.
 */
typedef struct st_be_utn_id_resp
{
	JC_INT8				*pcUserAgent ; /**< Specifies the User-Agent header value incase user agrees for sending the ID number to the server. */
	E_BE_UTN_ID_RESP	eIDResp ; /**< Specifies the user response whether to send the ID number or not. */
} ST_BE_UTN_ID_RESP ;

#endif

/** 
 * @brief The structure is used by browser engine to send the progress indication
 *		  details to the application.
 */

typedef struct st_be_prog_ind
{
	E_DATA_CONN_STATUS			eDataConnectionStatus ; /**< Specifies the data connection status */
	E_BE_STACK_STATUS			eStackStatus ; /**< Specifies the browser stack status */
} ST_BE_PROGRESS_IND ;

/** 
 * @brief The following structure is used by the application to set the profile that is to be used
 *		  for the lower layer operations.
 */
typedef struct st_be_profile
{
	E_BE_CONNECTION_MODE		eMode ; /**< Specifies the connection mode */
	JC_INT8						acHostName [BE_MAX_HOST_SIZE + 1] ; /**< Specifies the host name */
	JC_UINT32					uiPortNum ; /**< Specifies the port number */
	JC_UINT32					uiSecurePort ; /**< This is used only for proxy connection. This is used for socket connection instead of uiPortNum for secure requests */
	JC_BOOLEAN					bDeleteSessionHeaders ; /**< Specifies whether the existing session headers of 
															 the session has to be deleted when the profile is being changed*/
} ST_BE_PROFILE ;

/** 
 * @brief The following structure is used by the application to set the extended profile that is to be used
 *		  for the lower layer operations.
 */
typedef struct st_be_extended_profile
{
	JC_BOOLEAN					bDeleteSessionHeaders ; /**< Specifies whether the existing session headers of 
															 the session has to be deleted when the profile is being changed*/
	E_BE_CONNECTION_MODE		ePrimaryConnectionMode ; /**< Specifies the primary connection mode */
	JC_INT8						acPrimaryHostName [BE_MAX_HOST_SIZE + 1] ; /**< Specifies the primary host name */
	JC_UINT32					uiPrimaryPortNum ; /**< Specifies the primary port number */
	JC_BOOLEAN					bSecondaryUserConfRequired ; /**< Specifies whether, before using secondary connection user confirmation is required or not */
	E_BE_CONNECTION_MODE		eSecondaryConnectionMode ; /**< Specifies the secondary connection mode */
	JC_INT8						acSecondaryHostName [BE_MAX_HOST_SIZE + 1] ; /**< Specifies the secondary host name */
	JC_UINT32					uiSecondaryPortNum ; /**< Specifies the secondary port number */
	ST_GENERIC_DATA_CONN		stPrimaryDataConn ; /**< Specifies the Primary data connection */
	JC_BOOLEAN					bSecDataConnUserConfRequired ; /**< Specifies whether, before using secondary data connection user confirmation is required or not */
	ST_GENERIC_DATA_CONN		*pstSecondaryDataConn ; /**< Specifies the Secondary data connection */
	JC_BOOLEAN					bTertiaryDataConnUserConfRequired ; /**< Specifies whether, before using tertiary data connection user confirmation is required or not */
	ST_GENERIC_DATA_CONN		*pstTertiaryDataConn ; /**< Specifies the Tertiary data connection */
} ST_BE_EXTENDED_PROFILE ;

/**
 * @brief This structure is used by the browser engine to specify the profile confirmation indication
 *		  to the application.
 */
typedef struct st_be_profile_confirmation_ind
{
	E_BE_PROFILE_CONFIRMATION_TYPE eProfileConfType; /**< Specifies the type of profile confirmation required */
} ST_BE_PROFILE_CONFIRMATION_IND ;

/**
 * @brief This following structure is to be used by the application when it needs the BE to 
 *		  send an event which will set options at the lower layers. The options that can be set 
 *		  are the timeout value, setting the maximum number of retries in case of data failure, 
 *		  maximum socket connections that can be used by the HTTP Stack and the maximum buffer
 *		  that can be received from socket layer at a time.
 */
typedef struct st_be_set_option
{
	JC_INT32	iTimeOut ; /**< Specifies the time out value to be set at the lower layers */
	JC_INT32	iMaxRetries ; /**< Specifies the maximum number of connect retries that can be done at lower layers*/
	JC_INT32	iMaxSockets ; /**< Specifies the maximum number of socket connections (maximum SocketPools in http stack) 
								   that can be used by HTTP Stack at a time. */
	JC_INT32	iMaxTcpRecvBuf ; /**< Specifies the maximum buffer that can be read from the socket layer at a time 
									  (this will be used for jdd_NetRecv function).*/
	JC_INT32	iMaxSocketAliveTime ; /**< Specifies the maximum time till which an idle persistent HTTP socket that can be kept alive without closing.*/
} ST_BE_SET_OPTION ;

/**
 * @brief The following structure is used by the application to initialize the cache component.
 */
typedef struct st_be_cache_component
{
	JC_UINT32					uiGeneralCacheSize ; /**< Specifies the maximum size of the general cache. This value cannot be 0. */
	JC_UINT32					uiOperatorCacheSize ; /**< Specifies the maximum size of operator cache. This value can be 0. */
	JC_CHAR						*pmGeneralStorage ; /**< Specifies the filename where the general cache content is to be stored. This value cannot be NULL. */
    JC_CHAR						*pmOperatorStorage ; /**< Specifies the filename where the operator cache content is to be stored. This value can be NULL. */
	JC_CHAR						*pmDomainInfoFile ; /**< Specifies the filename where the opertor specific domain Info will be stored. This value can be NULL. */
} ST_BE_CACHE_COMPONENT ;

/**
 * @brief The following structure is used by the application to set the cache options
 */
typedef struct st_be_option_cache
{
	JC_BOOLEAN					bIsGeneralCacheEnabled ; /**< Specifies whether the general caching is enabled or not */
	JC_BOOLEAN					bIsOperatorCacheEnabled ; /**< Specifies whether the operator caching is enabled or not */
	JC_UINT32					uiGeneralCacheSize ; /**< Specifies the general cache size that is to be used by the browser */
	JC_UINT32					uiOperatorCacheSize ; /**< Specifies the operator cache size that is to be used by the browser */
} ST_BE_OPTION_CACHE ;

/**
 * @brief The following structure is used by the application to clear the cache
 */
typedef struct st_be_option_cache_clear
{
	JC_BOOLEAN					bClearGeneralCache ; /**< Specifies whether the general cache has to be cleared */
	JC_BOOLEAN					bClearOpeartorCache ; /**< Specifies whether the Operator cache has to be cleared */
} ST_BE_OPTION_CACHE_CLEAR ;

/**
 * @brief The following structure is used by the application to initialize the cookie component.
 */
typedef struct st_be_cookie_component
{
	JC_CHAR						*pmCookieFileName ; /**< Specifies the file where the cookie information is to be stored. */
	JC_UINT32					uiMaxNoOfCookies ; /**< Specifies the maximum number of cookies that can be stored in the browser */
	JC_UINT32					uiMaxCookieSize ; /**< Specifies the maximum size of a cookie that can be stored */ 
	JC_UINT32					uiMaxNoDomain ;	/**< Specifies the maximum number domains that can be added if "0" no restriction on the number of doamins */
} ST_BE_COOKIE_COMPONENT ;

/**
 * @brief The following structure is used by the application to set the cookie options 
 */
typedef struct st_be_option_cookie
{
	JC_BOOLEAN					bIsCookieEnabled ; /**< Specifies whether the cookie is enabled or not. */
	JC_BOOLEAN					bIsSessionCookieEnabled ; /**< Specifies whether the session cookie needs to be saved to persistent storage or not. */
} ST_BE_OPTION_COOKIE ;

/**
 * @brief The following structure is used by the application to initialize the history component.
 */
typedef struct st_be_history_component
{
	JC_CHAR						*pmHistoryFileName ; /**< Specifies the file where the history information is to be stored. */
	JC_INT32					iNoOfDomains ; /**< Specifies the maximum number of history domains that can be maintained in the browser */
	JC_INT32					iNoOfHosts ; /**< Specifies the maximum number of hosts/domain */ 
} ST_BE_HISTORY_COMPONENT ;


/** 
 * @brief The following structure is used by the application to specify the session header
 *		  that is to be sent to the lower layers
 */
typedef struct st_be_session_header
{
	JC_BOOLEAN			bHasMoreHeader ; /**< Specifies whether the application will send more header information or not */
	JC_INT8				acCharset [BE_MAX_CHARSET_SIZE + 1] ; /**< Specifies the charset information */
	JC_INT8				acName [BE_MAX_HEADER_NAME_SIZE + 1] ; /**< Specifies the header name if any */
	JC_UINT32			uiHeaderLen ; /**< Specifies the length of the header */
	JC_UINT8			*pucHeader ; /**< Specifies the header */
} ST_BE_SESSION_HEADER ;

/** 
 * @brief The structure is used by the browser engine to send the error information to
 *		  the application error handler.
 */
typedef struct st_be_error_listener
{
	JC_RETCODE					rCode ; /**< Specifies the browser error code.*/
	E_HTTP_STATUS				eHttpStatus ; /**< Specifies the lower layer (HTTP/WSP) status code. The browser can also be configured
												   to display the error page. If the browser has been enabled to display the content other than HTTP "OK" status,
												   then display of status error message will be called only if the content is not received. */
} ST_BE_ERROR_LISTENER ;


/** 
 * @brief The structure is used by the browser engine to specify to the application 
 *		  about an unsupported scheme while fetching a particular URL
 */
typedef struct st_be_scheme_listener
{
	JC_CHAR					*pmURI ; /**< Specifies the URL whose fetching resulted in unsupported scheme error */
	JC_UINT32				uiRequestID ; /**< Request ID shall be added only in case the content is to be handled by the application and sent back to BE */
	JC_HANDLE				vSchemeElement ; /**< Specifies the handle to the scheme element.*/
} ST_BE_SCHEME_LISTENER ;

/** 
 * @brief The structure is used by the browser engine to send the result of a download to cache
 *		  request.
 */
typedef struct st_be_download_to_cache_listener
{
	JC_RETCODE				rCode ; /** Specifies the result of the download to cache request */
	JC_UINT32				uiDownloadID ; /** Specifies the ID of the download to cache request for which the information is being sent */
} ST_BE_DOWNLOAD_TO_CACHE_LISTENER ;

/**
 * @brief The structure is used by the browser engine to send the push indication
 *		  to the application. i.e. whenever a push message is received by the browser the 
 *		  push information will be passed in this structure.
 */
typedef struct st_be_push_ind
{
	E_HTTP_STATUS				eHttpStatus ; /**< Specifies the lower layer status */
	JC_UINT8					*pucBuffer ; /**< Specifies the push buffer */
	JC_UINT32					uiLength ; /**< Specifies the push buffer length */
	JC_INT8						*pcContentType ; /**< Specifies the content type of the push content */
	JC_UINT8					*pucHeader ; /**< Specifies the push header */
	JC_UINT32					uiHeaderLength ; /**< Specifies the push header length */
	JC_INT8						*pcPPGAddress ; /**< Specifies PPG address if present */
	JC_INT8						*pcOrigAddress ; /**< Specifies the originating address (if the push is over SMS). */
	JC_BOOLEAN					bIsBinary ; /**< Specifies whether Push content is in binary format */
} ST_BE_PUSH_IND ;

#ifdef BROWSER_CACHE_ENABLE
/**
 * @brief The enum is used by the application to specify the type of cache invalidation 
 *		  operation that it wants the browser to execute.
 *
 * @note Define BROWSER_CACHE_ENABLE to use E_BE_INV_CACHE
 */
typedef enum e_be_inv_cache
{
	E_INVALIDATE_SERVICE, /**< Specifies that a cache service is to be invalidated. */
	E_INVALIDATE_OBJECT /**< Specifies that a cache object is to be invalidated. */
} E_BE_INV_CACHE ;

#endif

#ifdef BROWSER_PUSH_ENABLE
/**
 * @brief The structure is used by the application to send the push session initialize request
 *		  to the browser engine.
 *
 * @note Define BROWSER_PUSH_ENABLE to use ST_BE_PUSH_INIT
 */
typedef struct st_be_push_init
{
	E_BE_PUSH_TYPE				ePushType ; /**< Specifies the type of push session */
	JC_INT8						acIPAddress [BE_MAX_IP_SIZE + 1] ; /**< Specifies the Host name */
	JC_UINT32					uiPortNum ; /**< Specifies the port number */
} ST_BE_PUSH_INIT ;

/**
 * @brief This structure describes the push register structure that application needs to use
 *		  to register for receiving only specific type of push content.
 */
typedef struct st_be_push_register_info
{
	JC_UINT32				uiPushAppID ; /**< Specifies the ID of the push content */
	JC_INT8					acContentType [BE_MAX_CONTENT_TYPE_LEN + 1] ; /**< Specifies the content type of the push content */
} ST_BE_PUSH_REGISTER_INFO ;

#endif

/**
 * @brief The structure is used by the application when it needs to add a root certificate
 *		  to the browser. 
 */
typedef struct st_be_root_cert
{
	JC_UINT8				*pucContent ; /**< Specifies the certificate content */
	JC_UINT32				uiLength ; /**< Specifies the length of the certificate content */
	JC_INT8					*pcContentType ; /**< Specifies the content type of the certificate content */
} ST_BE_ROOT_CERT ;

/**
 * @brief The structure is used by the application when it needs to add a user certificate
 *		  to the browser. 
 */
typedef struct st_be_user_cert
{
	JC_UINT8				*pucUserCert ; /**< Specifies the user certificate content */
	JC_UINT32				uiCertLen ; /**< Specifies the length of the user certificate */
	JC_INT8					*pcContentType ; /**< Specifies the content type of the user certificate */
	JC_UINT8				*pucKey ; /**< Specifies the key associated with the user certificate */
	JC_UINT32				uiKeyLen ; /**< Specifies the length of the user certificate key */
	JC_INT8					*pcPassword ; /**< Specifies the password associated with the user certificate */
} ST_BE_USER_CERT ;

/**
 * @brief The enum indicates the type of security switch.
 */
typedef enum e_be_security_switch_to
{
	E_BE_SWITCH_TO_SECURE, /**< Switch from non-secure to secure mode */
	E_BE_SWITCH_TO_NONSECURE /**< Switch from secure to non-secure mode */
} E_BE_SECURITY_SWITCH_TO ;

/**
 * @brief The structure specifies the details of the switch in security mode.
 */
typedef struct st_be_security_switch
{
	E_BE_SECURITY_SWITCH_TO	eSecuritySwitch ; /**< Specifies the security mode to which the switch is attempted */
} ST_BE_SECURITY_SWITCH ;

/*
typedef struct st_be_non_security_ind
{
		
} ST_BE_NON_SECURITY_IND ;

typedef struct st_be_non_secure_post
{

} ST_BE_NON_SECURE_POST ;
*/

/**
 * @brief The structure is used by the browser engine to send the security indication to the 
 *		  application. The security indication specifies the various security parameters that
 *		  are negotiated by the lower layers while setting up secured connection.
 */
typedef struct st_be_security_ind
{
	JC_UINT32				uiSecurityReferenceID; /**< Specifies a unique security related reference ID. This ID is to be used if
														the application wants to use any other API (like getting cert) before sending 
														response of the security indication */
	JC_CHAR					*pmURI ; /**< Specifies the URI for which the indication is done */
	JC_BOOLEAN				bIsWTLS ; /**< Specifies the security indication is for WTLS or SSL */
#ifdef WAP_WITH_WTLS
	E_WTLS_CIPHER_SUITE		eWTLSCipherSuites ; /**< Specifies the WTLS cipher suite */
	E_WTLS_KEY_EXCHANGE_ALG	eWTLSKeyExAlgs ; /**< Specifies the WTLS key exchange algorithm */
#endif
#ifdef HTTP_WITH_SSL
	E_SSL_CIPHER_SUITE		eSSLCipherSuites ; /**< Specifies the SSL cipher suite */
#endif
	JC_BOOLEAN				bIsValid ; /**< Reserved for future purpose */
	JC_UINT32				uiExtendedError ; /**< Specifies an extended error if any while establishing security */
	JC_UINT32				uiTotalCertificate ; /**< Specifies the total number of certificates received by the lower layer */
	JC_UINT32				uiLength ; /**< Specifies the length of the first certificate */
	JC_UINT8				*pucBuffer ; /**< Specifies the content of the first certificate */
} ST_BE_SECURITY_IND ;

/**
 * @brief The structure is used by the browser engine to send the security failure indication
 *		  to the application. The structure specifies the class of security which has failed.
 */
typedef struct st_be_security_failure_ind
{
	JC_UINT32				uiSecurityError ; /**< Specifies the security error specifies by the security layer */
	E_BE_SECURITY_CLASS		eSecurityClass ; /**< Specifies class of security */
} ST_BE_SECURITY_FAILURE_IND ;

/**
 * @brief The structure is used by the browser engine to send the certificate details. After security is established
 * the application may request for certificates (if certificate is more than 1). Then the response of
 * the certificate is returned in this structure.
 */
typedef struct st_be_cert_resp_ind
{
	JC_RETCODE				rCode ; /**< Specifies error if any while retrieving the certificates */
	JC_UINT32				uiCertIndex ; /**< Specifies the index of the cert in the cert list (which is maintained in the lower layer) */
	JC_UINT32				uiLength ; /**< Specifies the length of the certificate */
	JC_UINT8				*pucBuffer ; /**< Specifies the content of the certificate */
} ST_BE_CERT_RESP_IND ;

/**
 * @brief The union is used to specify the security indication information to the application.
 *		  For each security enum (E_BE_SECURITY_INDICATION) a corresponding member in this union
 *		  will be specified by the browser engine.
 */
typedef union u_be_security_indication
{
	ST_BE_SECURITY_SWITCH			*pstSecuritySwitch ; /**< Specifies the security switch details */
	ST_BE_SECURITY_IND				*pstSecurityInd ; /**< Specifies the security indication details */
	ST_BE_SECURITY_FAILURE_IND		*pstSecurityFailureInd ; /**< Specifies the security failure indication details */
	ST_BE_CERT_RESP_IND				*pstCertRespInd ; /**< Specifies the certificate details */
	/** 
	ST_BE_NON_SECURITY_IND			*pstNonSecurityInd ;
	ST_BE_NON_SECURE_POST			*pstNonSecurePost ;	
	**/
} U_BE_SECURITY_INDICATION ;

/**
 * @brief The enum is used to specify the type of security indication to the application.
 *		  For each security enum a corresponding member in this union (U_BE_SECURITY_INDICATION)
 *		  will be specified by the browser engine.
 */
typedef enum e_be_security_indication
{
	E_BE_SECURITY_SWITCH, /**< Switch of security mode. (Argument #ST_BE_SECURITY_SWITCH)*/
	E_BE_SECURITY_CONFIRM, /**< Security mode confirmation. (Argument #ST_BE_SECURITY_IND)*/
	E_BE_SECURITY_FAILURE, /**< Security failure indication. (Argument #ST_BE_SECURITY_FAILURE_IND) */
	E_BE_CERT_RESP, /**< Certificate Information. (Argument #ST_BE_CERT_RESP_IND) */
	E_BE_SECUREINLINE_IN_NONSECURE, /**< Secure inline content in non-secure mode warning */
	E_BE_NONSECUREINLINE_IN_SECURE, /**< Non-secure inline content in secure mode warning */
	E_BE_NON_SECURITY_CONFIRM, /**< Non-Security mode confirmation */
	E_BE_NON_SECURE_POST /**< Post data in secure mode warning */
} E_BE_SECURITY_INDICATION ;

/**
 * @brief The structure is used by the browser engine to specify the security indication to the application
 *		  The application needs to use the details in this structure to display various options 
 *		  (such as getting user confirmation, showing security icon etc)
 */
typedef struct st_be_security_indication
{
	E_BE_SECURITY_INDICATION		eSecurityIndication ; /**< Sepcifies the type of security indication */
	U_BE_SECURITY_INDICATION		uSecurityIndication ; /**< Specifies the details of the security indication */
} ST_BE_SECURITY_INDICATION ; 

/**
 * @brief The structure is used by the browser engine to send the header information to the 
 *		  application
 */
typedef struct st_be_header_ind
{
	JC_UINT32			uiHeaderLength ; /**< Specifies the length of the header */
	JC_UINT32			uiContentLength ; /**< Specifies the length of the content to be downloaded */
	E_HTTP_STATUS		eHTTPStatus ; /**< Specifies the lower layer status */
	JC_BOOLEAN			bIsBinary ; /**< Specifies whether the header information is in binary format or not */
	JC_INT8				*pcContentType ; /**< Specifies the content type */
	JC_UINT8			*pucHeader ; /**<Specifies the header */
	JC_CHAR				*pmURL ; /**< Specfifies the URL for which the header indication is sent */
} ST_BE_HEADER_IND ;

/**
 * @brief The structure is used by the browser engine to send the buffer information to the 
 *		  application
 */
typedef struct st_be_fetch_ind
{
	JC_UINT32			uiCurrentLength ; /**< Specifies the length of the buffer sent in the current indication */
	JC_BOOLEAN			bHasMoreData ; /**< Specifies whether more indication are to be received */
	JC_UINT8			*pucBuffer ; /**< Specifies the fetch indication (buffer) */
} ST_BE_FETCH_IND ;

/**
 * @brief The union specifies the header or buffer indication based on the Indication type 
 *		  (header or buffer).
 */
typedef union u_be_url_ind
{
	ST_BE_HEADER_IND	stBeHeaderInd ; /**< Specifies the header information */
	ST_BE_FETCH_IND		stBeFetchInd ; /**< Specifies the buffer information */
	JC_INT8				*pcContentType ; /**< Specifies the content type */
} U_BE_URL_IND ;

/**
 * @brief The structure is used by the browser engine to send the fetch indication to the application.
 *		  The browser engine can use the structure to specify header, buffer or error indication
 *		  to the application.
 */
typedef struct st_be_url_ind
{
	JC_UINT32			uiRequestID ; /**< Specifies the ID of the request for which the indication is sent */
	E_BE_IND_TYPE		eIndType ; /**< Specifies the type of indication (header, buffer or error) */
	JC_RETCODE			rErrorCode ; /**< Specifies the error in case the indication is an error indication */
	U_BE_URL_IND		uBeInd ; /**< Specifies either the buffer or header indication (based on the eIndType) */
} ST_BE_URL_IND ;

/**
 * @brief In case the application is using non-blocking mechanism for sending the network confirmation
 *			response, it needs to use this structure.
 */
typedef struct st_be_network_confirm_resp
{
	JC_RETCODE		rRespCode ; /**< Specifies the return code based no the user action **/
} ST_BE_NETWORK_CONFIRM_RESP ;

/**
 * @brief	    Prototype of Application callback.
 *
 * @param[in]	pvArg The application needs to specify the app arguement while initializing
 *				the browser. The same argument will be passed while calling this application
 *				callback.
 * @param[in]	uiWindowID Specifies the ID of the browser window ID for which the application
 *				callback is called (since the browser supports multiple browser window).
 * @param[in]	eCallbackType Specifies the callback type that the application needs to invoke or handle.
 * @param[in]	pvCallbackInfo Specifies the indication that will be required by the application to handle the callback type.
 * @return		The callback function can return any application related error.
 *
 * The application needs to implement a callback with the following prototype and pass the address
 * of the function in jdi_BEInitialize. From then on the browser engine will call this
 * callback to send any information to the application.
 *
 * The browser engine will send many indications to the application. Instead of
 * registering a separate application callback for each browser engine indication, the application
 * needs to just register one function with the following prototype. Based on the
 * callback type the application needs to decide which app function needs to be invoked.
 * 
 */
typedef JC_RETCODE (* CB_BE_GENERAL_CALLBACK) (void					*pvArg,
											   JC_UINT32			uiWindowID,
											   E_BE_CALLBACK_TYPE 	eCallbackType,
											   void 				*pvCallbackInfo) ;
/**
 * @brief	    Initializes the browser engine
 * 
 * @param[in]	pvArg Specifies the Application argument. The browser engine will use this 
 *				argument while calling the application callback.
 * @param[in]	cbCallbackFnc Specifies the applications general callback which will handle
 *			    all the callback indications from the browser engine.
 * @param[in]	stBrowserAppTaskInfo Specifies the application task information. 
 * @param[in]	stTransportTaskInfo Specifies the transport task information.
 * @param[out]	pvHandle Specifies browser engine handle which will be returned by this API.
 *				The application needs to use this handle to call all the other browser engine functions.
 * @retval		JC_OK if the browser engine is sucessfully initialized.
 * @retval		JC_ERR_MEMORY_ALLOCATION if a memory error occurs.
 * @see			jdi_BEDeinitialize
 * @see			jdi_BEInitComponent
 *
 * The application needs to call the API to initialize the browser engine. 
 * This API will only initialize the browser engine module, communicator module and
 * the lower layer modules. 
 *
 * The application needs to use the API jdi_BEInitComponent to initialize the other
 * browser components and jdi_BEOpen to initialize the display engine. 
 */
JC_RETCODE jdi_BEInitialize (void						*pvArg,
							 CB_BE_GENERAL_CALLBACK		cbCallbackFnc,
							 JC_TASK_INFO				stBrowserAppTaskInfo,
							 JC_TASK_INFO				stTransportTaskInfo,
							 JC_HANDLE					*pvHandle) ;

/**
 * @brief	    Initializes browser component.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	eBeComponent Specifies the browser component that is to be initialized.
 * @param[in]	pvComponentInfo Specifies the information related to the component being initialized.
 * @retval		JC_OK if the browser component is sucessfully initialized.
 * @retval		JC_ERR_ASYNC_CALL Specifies that the cache initialization has to be continued
 *				at a later point of time by using the API #jdi_BEContinueCacheInit.
 * @see			jdi_BEDeinitComponent
 *
 * When the API jdi_BEInitialize is called only the browser engine is initialized. The applicaion
 * needs to use this API to initialize the various browser engine components such
 * as cache, cookie and history.
 */
JC_RETCODE jdi_BEInitComponent (JC_HANDLE				vHandle,
								E_BE_COMPONENT			eBeComponent,
								void					*pvComponentInfo) ;

#ifdef BROWSER_CACHE_ENABLE
/**
 * @brief		This API is used to continue with cache initialization
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK On successful initialize of the cache
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_ERR_INVALID_PARAMETER On passing Invalid Handle.
 * @retval		JC_ERR_ASYNC_CALL Specifies that the cache initialization has to be continued
 *				at a later point of time by using the API #jdi_BEContinueCacheInit.
 *
 * During initialization the cache files are created. If the application wants to create one file at
 * a time and then later continue with next file creation then the cache will return the error JC_ERR_ASYNC_CALL
 * during init. The application then needs to call this API at a later point of time. This API may also
 * return the same error and the application will have to call this API until it returns JC_OK.
 *
 * If this API returns any other error then the application should explicitly call the
 * cache deinit component.
 */
JC_RETCODE jdi_BEContinueCacheInit (JC_HANDLE			vHandle) ;
#endif

/**
 * @brief	    Activates a browser window.
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiWindowID Specifies the ID of the browser window to be activated.
 * @retval		JC_OK if the browser window is activated successfully.
 *
 * The browser supports multiple window concept wherein the application can open multiple browser
 * windows. The application needs to use this API to activate a particular browser
 * window.
 *
 * The application needs to specify the ID of the browser window which is to be activated.
 *
 * @note If only one browser window is opened then the browser engine will actiavte it
 * by default and if the activated window is closed then the browser will another browser
 * window by default.
 */
JC_RETCODE jdi_BESetActive (JC_HANDLE		vHandle,
							JC_UINT32		uiWindowID) ;

/**
 * @brief	    Opens a browser window.
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiWindowID The application needs to specify the ID of the browser window in this parameter.
 * @retval		JC_OK if the browser is opened sucessfully.
 * @see			jdi_BEClose
 *
 * The application needs to use this API to open a browser window. The API
 * will initialize the display engine for the window.
 */
JC_RETCODE jdi_BEOpen (JC_HANDLE		vHandle,
					   JC_UINT32		uiWindowID) ;

/**
 * @brief	    Sets the browser properties
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	eSetOptions Specifies the browser property which has to be set.
 * @param[in]	pvOptionInfo Specifies the information related to the browser property.
 * @retval		JC_OK if the browser sets the property sucessfully.
 * @retval		JC_ERR_INVALID_PARAMETER if a invalid DE property is given.
 *
 * The application needs to use this API to set the various browser properties such
 * as cache enable, cookie enable, display mode, graphics mode etc. 
 *
 * This API need not be called in any specific sequence. It can be called at any 
 * point to reset the browser properties.
 */
JC_RETCODE jdi_BESetProperties (JC_HANDLE 			vHandle,
								E_BE_SET_OPTIONS 	eSetOptions,
								void				*pvOptionInfo) ;

/**
 * @brief		Sets a browser event. 
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	eEventType Specifies the browser event type
 * @param[in]	pvEventInfo Specifies the information related to the browser event.
 * @retval		JC_OK if the browser handles the event sucessfully.
 * @retval		JC_ERR_INVALID_PARAMETER if the event specified is invalid.
 *
 * The application needs to use this API when it comes across any event or operation that is to 
 * be handled by the browser engine. 
 */
JC_RETCODE jdi_BESetEvent (JC_HANDLE			vHandle,
						   E_BE_EVENT_TYPE		eEventType,
						   void					*pvEventInfo) ;

/**
 * @brief	    Gets the history context.
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[out]	pvHistoryCtxt The API returns the history context in this field.
 * @retval		JC_OK if the history context is returned sucessfully
 * @see			jdi_history.h
 *
 * The application needs to use this API to get the history context. The API
 * will return the handle of the history module. The application can then use this 
 * handle to call the Exposed APIs of the history module.
 */
JC_RETCODE jdi_BEGetHistoryContext (JC_HANDLE			vHandle,
									JC_HANDLE			*pvHistoryCtxt) ;

#ifdef BROWSER_WINDOW_OPEN
/**
 * @brief		Gets the page information.
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiWindowID Specifies the window ID for the page.
 * @param[out]  pstPageInfo The API will return the page information in this field.
 * @retval		JC_OK if the browser returns the page info sucessfully
 * @retval		JC_ERR_INVALID_PARAMETER Specifies that no page is currently fetched in the browser.
 *
 * The application needs to call this API to get the information of the page that is 
 * currently displayed on the browser. The page information will not specify the content
 * of the page. To view the content of the page the application needs to call the
 * API jdi_BEGetPageContent
 *
 * @note The application should not free the memory associated with pstPageInfo 
 * @note The page information will be invalid once page display is cleared (say if another page
 * is fetched). The application can call this API at any point of time (page is fetching or fetched). 
 */
JC_RETCODE jdi_BEGetPageInfoWindow (JC_HANDLE			vHandle,
									JC_UINT32			uiWindowID,
									ST_BE_PAGE_INFO		*pstPageInfo) ;
#endif
/**
 * @brief		Gets the page information.
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[out]  pstPageInfo The API will return the page information in this field.
 * @retval		JC_OK if the browser returns the page info sucessfully
 * @retval		JC_ERR_INVALID_PARAMETER Specifies that no page is currently fetched in the browser.
 *
 * The application needs to call this API to get the information of the page that is 
 * currently displayed on the browser. The page information will not specify the content
 * of the page. To view the content of the page the application needs to call the
 * API jdi_BEGetPageContent
 *
 * @note The application should not free the memory associated with pstPageInfo 
 * @note The page information will be invalid once page display is cleared (say if another page
 * is fetched). The application can call this API at any point of time (page is fetching or fetched). 
 */
JC_RETCODE jdi_BEGetPageInfo (JC_HANDLE				vHandle,
							  ST_BE_PAGE_INFO		*pstPageInfo) ;

/**
 * @brief		Gets the Navigation information for the current display window. 
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[out]  pstNavInfo The API will return the navigation informatiion in this field.
 * 
 * The application needs to call this API to get the navigation status whether the back & 
 * forward is allowed for the currently active window. 
 */
void jdi_BEGetNavigationInfo (JC_HANDLE				vHandle,
							  ST_BE_NAVIGATION_INFO	*pstNavInfo) ;

/**
 * @brief		Gets the content of the page.
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[out]  pstPageContent The API will return the page content in this field.
 * @retval		JC_OK if the browser returns the page content sucessfully
 * @retval		JC_ERR_INVALID_PARAMETER Specifies that no page is currently fetched.
 *
 * The application needs to call this API to get the content of the page that is currently
 * displayed on the browser. The page content will specify the content of the page (source)
 * and will also specify the page information.
 *
 * @note The appliction should not free the memory associated with pstPageContent.
 * @note The page content will be invalid once page display is cleared (say if another page
 * is fetched). The application can call this API at any point of time (page is fetching or fetched). 
 */
JC_RETCODE jdi_BEGetPageContent (JC_HANDLE				vHandle,
								 ST_BE_PAGE_CONTENT		*pstPageContent) ;

/**
 * @brief		Gets the information of the focussed control
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[out]  pstCtrlInfo The API will return the focussed controls info in this field.
 * @retval		JC_OK if the browser returns the information of the focussed control
 *				sucessfully
 * @retval		JC_ERR_INVALID_PARAMETER if none of the controls are currently focussed.
 *
 * The application needs to call this API to get the information of the currently
 * focussed control. The API will return the control information only if control 
 * is in the visible area.
 *
 * @note The application should not free the memory associated with pstCtrlInfo.
 * @note The control info will be invalid once focus shifts from the control.
 */
JC_RETCODE jdi_BEGetFocusedControlInfo (JC_HANDLE				vHandle,
										ST_BE_CONTROL_INFO		*pstCtrlInfo) ;

/**
 * @brief		Gets mime information of the focussed control.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
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
 * @note The application should not free the memory associated with pstCtrlInfo.
 * @note The control mime info will be invalid once focus shifts from the control. 
 *
 */
JC_RETCODE jdi_BEGetFocusedControlMimeInfo (JC_HANDLE			vHandle,
											ST_BE_URL_CONTENT	*pstCtrlMimeInfo) ;

/**
 * @brief		Gets the type of the focused button control
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[out]  pstButtonType The API will return the focused button control's type.
 * @retval		JC_OK if the browser returns the type of the button control
 *				sucessfully
 * @retval		JC_ERR_INVALID_PARAMETER if no button control is currently focussed.
 *
 * The application needs to call this API to get the type of the currently focused
 * button control.
 */
JC_RETCODE jdi_BEGetFocusedButtonType (JC_HANDLE				vHandle,
									   ST_BE_BUTTON_TYPE		*pstButtonType) ;

/**
 * @brief	    Gets focussed link info.
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[out]  ppmURI The API will return the URL in this field.
 * @retval		JC_OK if the browser returns the Focussed link info sucessfully
 * @see			jdi_BEFreeFocusedLinkInfo
 *
 * The application needs to call this API to get the URL associated with the currently focussed link. 
 * The API will return the URL only if the focussed link is in the visible area.
 *
 * @note The application needs to free the memory (using jdi_BEFreeFocusedLinkInfo)
 * associated with pmURL. 
 *
 */
JC_RETCODE jdi_BEGetFocusedLinkInfo (JC_HANDLE			vHandle,
									 JC_CHAR			**ppmURI) ;

/**
 * @brief	    Frees focused link info.
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]   pmURI Specifies the focused link info which is to be freed.
 *
 * The application needs to use this API to free the focused control info which was
 * obtained previously, using the API jdi_BEGetFocusedLinkInfo.
 *
 */
void jdi_BEFreeFocusedLinkInfo (JC_HANDLE				vHandle,
								JC_CHAR					*pmURI) ;


/**
 * @brief		Gets focussed object's type.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
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
JC_RETCODE jdi_BEGetFocusedObjectType (JC_HANDLE			vHandle,
									   E_OBJECT_MIME_TYPE	*peType) ;

/**
 * @brief		Refreshes the current display of the active browser.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK if the browser display is refreshed successfully.
 * @retval		JC_ERR_INVALID_PARAMETER if there are no controls in the visible area.
 *
 * The application needs to call this API to refresh the content displayed on the 
 * active browser. The browser will redraw only the visible area of the browser 
 * (and not the entire page).
 */
JC_RETCODE jdi_BERefreshDisplay (JC_HANDLE		vHandle) ;

/**
 * @brief		Redraws the entire page that is currently displayed
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK if the redraw is done successfully.
 * @retval		JC_ERR_INVALID_PARAMETER if no page is currently displayed on the browser.
 *
 * The application needs to call this API to redraw the page displayed on the 
 * browser. The browser will redraw the visible and the non-visible area of the page 
 * (for example when the display mode is changed).
 */
JC_RETCODE jdi_BERedrawPage (JC_HANDLE			vHandle) ;

/**
 * @brief		Sets the focus on the current focusable control in the displayed page
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK if the focus is set successfully.
 * @retval		JC_ERR_INVALID_PARAMETER if no page is currently displayed on the browser.
 *
 * The application needs to call this API to set the focus on the current focusable control.
 * This function is called at the time of alert/confirmation/prompt messages which need to
 * set the focus on the focusable control after executing some script related functionality.
 */
JC_RETCODE jdi_BESetCurrentFocus (JC_HANDLE	vHandle) ;

/**
 * @brief		Handles the navigation between pages.
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	eNavType Specifies the type of navigation (forward or backward)
 * @retval		JC_OK if navigation is done successfully
 * @retval		JC_ERR_INVALID_PARAMETER if invalid navigation type is specified.
 * @retval		JC_ERR_NAVIGATION_NO_PAGE_FOUND if the navigation list is empty.
 * @retval		JC_ERR_NAVIGATION_PREV_PAGE_NOT_FOUND if there is no previous element in the navigation list.
 * @retval		JC_ERR_NAVIGATION_NEXT_PAGE_NOT_FOUND if the next item in the navigation list is empty.
 *
 * The application needs to call this API when it needs to navigate between pages. The 
 * eNavType will specify the whether foward or backward navigation has to be done.
 */
JC_RETCODE jdi_BEHandleNavigation (JC_HANDLE				vHandle, 
								   E_BE_NAVIGATION_TYPE		eNavType) ;

/**
 * @brief		Gets Anchor Count
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[out]	puiCount Returns the number of anchor node in the displayed page.
 * @retval		JC_OK If the anchor count is returned successfully.
 * @see			jdi_BEGetAnchorList
 *
 * The application needs to call this API to get the count of anchor elements
 * in the page that is currently displayed on the browser.
 */
JC_RETCODE jdi_BEGetAnchorCount (JC_HANDLE				vHandle, 
								JC_UINT32				*puiCount) ;

/**
 * @brief		Gets the list of anchor elements
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiStartIndex Specifies the index from which the anchor information is needed.
 * @param[in]	uiCount Specifies the number of anchor element whose information is to be 
 *				returned in the API.
 * @param[out]	pstAnchorData Specifies the field in which the anchor information is returned.
 * @retval		JC_OK if anchor data list is returned successfully.
 * @note		This API can be called while the page is getting downloaded or after the 
 *			    page is completely downloaded.
 *
 * The application needs to call this API to get the list of anchor elements that are
 * displayed on the browser. The API will return the anchor list in ppstAnchorData.
 *
 * The application has to first get the total count of anchor elements in the page using
 * the API jdi_BEGetAnchorCount. Then based on the total count the application needs
 * to decide the index from which the anchor elements needs to be retrieved. The number of
 * anchor elements that needs to be returned in pstAnchorData is to be specified in 
 * uiCount.
 *
 * Each time the get anchor list is called the browser will allocate memory for
 * the anchor list. So the application needs to free the list using the API jdi_BEFreeAnchorList.
 */
JC_RETCODE jdi_BEGetAnchorList (JC_HANDLE				vHandle,
								JC_UINT32				uiStartIndex,
								JC_UINT32				uiCount,
								ST_BE_ANCHOR_DATA		*pstAnchorData) ;

/**
 * @brief		Frees the anchor list
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstAnchorData Specifies the anchor list to be freed. 
 *
 * The application needs to use this API to free the anchor list which was
 * obtained previously, using the API jdi_BEGetAnchorList.
 */
void jdi_BEFreeAnchorList (JC_HANDLE				vHandle,
						   ST_BE_ANCHOR_DATA		*pstAnchorData) ;

/**
 * @brief		Sends the fetch request to the browser.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstFetchReq Specifies the fetch request information.
 * @retval		JC_OK if the fetch request is processed sucessfully.
 * @note		The application need not stop all the requests that are currently being fetched
 *				before calling the fetch request API. The browser will internally stop all 
 *				the currently fetched requests and then proceed with the fetch request.
 *
 * The application needs to use this API to send the fetch request to the browser. The 
 * application needs to specify the fetch request information (URL, post data, authorization)
 * etc in pstFetchReq.
 */
JC_RETCODE jdi_BEFetchRequest (JC_HANDLE				vHandle,
							   const ST_BE_FETCH_REQ	*pstFetchReq) ;

/**
 * @brief		Clears the authorization information 
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	eBeClearAuthHeader Specifies the type of authorization list to be cleared.
 * @retval		JC_OK if the auth information is cleared successfully.
 *
 * All the authorization information (URI and proxy) that are added by the application will be stored
 * at the lower layer for the lifetime of the browser session. If the application wants to 
 * clear the authorization information maintained at the lower layers without deinitializing the 
 * session then it needs to use this API.
 */

JC_RETCODE jdi_BEClearAuthInfo (JC_HANDLE				vHandle,
								E_BE_CLEAR_AUTH_HEADER	eBeClearAuthHeader) ;

/**
 * @brief	  Stop the requests currently being processed.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK if the stop request is processed sucessfully.
 *
 * The application needs to use this APi to stop all the requests that the browser
 * is currently processing. The API will send stop request for each request (which is
 * yet to be completed) to the lower layer. 
 *
 * Once a request is stopped successfully then the end fetch listener of the application
 * (E_BE_END_FETCH_LISTENER) will be called and once all the requests of the page is 
 * stopped then the end page listener of the application (E_BE_END_PAGE_LISTENER)
 * will be called.
 */
JC_RETCODE jdi_BEStopRequest (JC_HANDLE	vHandle) ;

/**
 * @brief	  Stop the request with given request ID.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiReqID Specifies the id of the request to be stopped.
 * @retval		JC_OK if the stop request is processed sucessfully.
 * @retval		JC_ERR_INVALID_PARAMETER if the given request id is not valid.
 *
 * The application needs to use this APi to stop a given request using 
 * the request ID. The API will send stop request to file or cache or 
 * lower layer based on the request source.
 */
JC_RETCODE jdi_BEStopRequestWithID (JC_HANDLE	vHandle,
									JC_UINT32	uiReqID) ;

/**
 * @brief		Sends a download request to the browser
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstDownloadReq Specifies the download request
 * @param[out]	puiDownloadID The function returns the ID of the download request in this parameter.
 * @retval		JC_OK if the download request is processed sucessfully.
 *
 * The application needs to use this API to send the download request to the browser. The 
 * application needs to specify the download request information (URL, post data, authorization)
 * etc in pstDownloadReq.
 *
 * The browser will send all the information (header, buffer etc) related to the download
 * request to the E_BE_DOWNLOAD_LISTENER callback of the application.
 */
JC_RETCODE jdi_BEDownload (JC_HANDLE				vHandle,
						   const ST_BE_FETCH_REQ	*pstDownloadReq,
						   JC_UINT32				*puiDownloadID) ;

/**
 * @brief	    Stops all the download request which are being processed
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK if all the download requests are stopped successfully.
 *
 * The application needs to use this API to stop all the download requests that the browser
 * is currently processing. The API will send stop request for each download request (which is
 * yet to be completed) to the lower layer.
 */
JC_RETCODE jdi_BEStopAllDownload (JC_HANDLE			vHandle) ;

/**
 * @brief		Stop a particular download request
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize. 
 * @param[in]	uiDownloadID Specifies the id of the download request to be stopped
 * @retval		JC_OK if the download request is stopped successfully.
 *
 * The application needs to use this API to stop a particular download request that the browser
 * is currently processing. The application needs to specify the ID of the download request to be 
 * stopped.
 */
JC_RETCODE jdi_BEStopDownload (JC_HANDLE			vHandle,
							   JC_UINT32			uiDownloadID) ;

/**
 * @brief	    Reloads a page.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK if the page is reloaded successfully.
 * @retval		JC_ERR_INVALID_PARAMETER if currenly no page is displayed on the browser.
 *
 * The application needs to use this API to reload the page that is currently displayed or
 * currently being displayed. The browser will give a fresh fetch request while reloading 
 * (instead of just performing a redraw operation).
 */
JC_RETCODE jdi_BEReloadPage (JC_HANDLE				vHandle) ;

/**
 * @brief		Sets a connection profile.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstProfile Specifies the profile information.
 * @retval		JC_OK if the profile is set successfully.
 * @see			jdi_BESetDataConn
 *
 * The application needs to used this API to set the communication profile that should be
 * used for all lower layer fetch operations. The API sends the profile information to the
 * lower layers.
 *
 * Before setting the profile the application needs to specify the data connection
 * information to be used for the fetch operations (using jdi_BESetDataConn).
 */
JC_RETCODE jdi_BESetProfile (JC_HANDLE				vHandle,
							 const ST_BE_PROFILE	*pstProfile) ;

/**
 * @brief		Sets a extended connection profile.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstProfile Specifies the extended profile information.
 * @retval		JC_OK if the profile is set successfully.
 *
 * The application needs to used this API to set the communication (extended) profile that should be
 * used for all lower layer fetch operations. The API sends the profile information to the
 * lower layers.
 *
 * All the data connection information will be part of the #ST_BE_EXTENDED_PROFILE and hence the
 * API #jdi_BESetDataConn need not be used before calling this function.
 */
JC_RETCODE jdi_BESetExtendedProfile (JC_HANDLE						vHandle,
									 const ST_BE_EXTENDED_PROFILE	*pstProfile) ;

/**
 * @brief		Sends the profile response response.
 *
 * @param[in]	vHandle Specifies the handle that was returned by BE initialize API
 * @param[in]	bProceed Specifies whether the application wants to proceed with the profile information.
 * @retval		JC_OK if the profile confirmation response is successfully sent.
 *
 * If a profile information specified in extended profile fails in the lower layer then it sends
 * profile confirmation request to the application (using #E_BE_PROFILE_CONFIRMATION_IND_LISTENER) to check
 * whether the application wants to proceed with the next option of the extended profile. In order to
 * do that the application should use this API.
 */
JC_RETCODE jdi_BESendProfileConfirmationResponse (JC_HANDLE				vHandle,
												  JC_BOOLEAN			bProceed) ;

/**
 * @brief		Sets specific browser (lower layer specific) options.
 *
 * @param[in]	vHandle Specifies the handle that was returned by communicator initialize API
 * @param[in]	pstSetOption Specifies the Structure which will specify the application defined options.
 * @retval		JC_OK if the set option event is sent
 *
 
 * can be set by the application. The application needs to use this API to set
 * such parameters. The various options which can be set is specified in the
 * structure ST_BE_SET_OPTION.
 */
JC_RETCODE jdi_BESetOption (JC_HANDLE				vHandle,
							ST_BE_SET_OPTION		*pstSetOption) ;

/**
 * @brief		Sends the session header
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstSessionHeader Specifies the session header that is to be sent.
 * @retval		JC_OK if the session header is sent successfully.
 *
 * The application needs to used this API to send the session header to the browser.
 * The browser will in turn send the session headers to the lower layers. 
 *
 * As the name suggests the session header will be added for the whole lower layer session. 
 * This header will be part of the all the requests that are being processed in that particular
 * lower layer session.
 *
 * For example all the content (like accept, RDF etc) which are added from the config file can be added using
 * session headers API. In that case from the config file JC_ERR_CFG_NOT_FOUND can be returned.
 */
JC_RETCODE jdi_BESendSessionHeaders (JC_HANDLE						vHandle,
									 const ST_BE_SESSION_HEADER		*pstSessionHeader) ;

/**
 * @brief		Sets the data connection.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstDataConn Specifies the data connection information.
 * @retval		JC_OK if the data connection is set successfully.
 * @see			jdi_BEDisconnect
 *
 * The application needs to used this API to disconnect the data connection that is
 * used for all lower layer fetch operations. The API sends the data connection information
 * to the lower layers. This API is to be called before setting the profile using
 * jdi_BESetProfile.
 */
JC_RETCODE jdi_BESetDataConn (JC_HANDLE						vHandle,
							  const ST_GENERIC_DATA_CONN	*pstDataConn) ;

/**
 * @brief		Handles the content sent from application.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiRequestID Specifies the request for which the content is being sent.
 * @param[in]	pstBeFetchInd Specifies the content.
 * @param[in]	rErrorCode The application can specify error for the request (if any) in this parameter.
 * @retval		JC_OK if the content is handled successfully.
 *
 * The application needs to use this API to sent the content for a particular request. In certain
 * cases the browser engine will not handle the content (for unknown content type). The content in that
 * case will be handled by the application. After receiving the content if the application wants 
 * to display it on the display screen then this API needs to be used.
 */
JC_RETCODE jdi_BEHandleContentIndication (JC_HANDLE			vHandle,
										  JC_UINT32			uiRequestID,
										  ST_BE_FETCH_IND	*pstBeFetchInd,
										  JC_RETCODE		rErrorCode) ;

/**
 * @brief		Parses header.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pucHeader Specifies the header to be parsed.
 * @param[in]	uiHeaderLength Specifies the length of the header.
 * @param[in]	bIsBinary Specifies whether the header is in binary format or not.
 * @param[out]	pvParseHandle API returns the handle to the parsed content. The application needs
 *				to use this handle when it wants to free the parsed header.
 * @param[out]	ppstMimeHeaders API returns the parsed header in this parameter.
 * @retval		JC_OK if the header is parsed successfully.
 * @see			jdi_BEFreeParsedHeader
 *
 * The application needs to used this API to parse a header. 
 *
 * If this API returns JC_OK but does not return the parsed header or the parsed header handle (both are NULL)
 * then it indicates that the decoder (for binary or non-binary) is not enabled.
 *
 * The application should use the API #jdi_BEFreeParsedHeader to free the parsed header.
 * Application needs to only pass the handle (*pvParseHandle) to #jdi_BEFreeParsedHeader, the Freeing
 * of mime headers (*ppstMimeHeaders) will be taken care by the API.
 */
JC_RETCODE jdi_BEParseHeader (JC_HANDLE					vHandle,
							  JC_UINT8					*pucHeader,
							  JC_UINT32					uiHeaderLength,
							  JC_BOOLEAN				bIsBinary,
							  JC_HANDLE					*pvParseHandle,
							  ST_MIME_HEADERS			**ppstMimeHeaders) ;

/**
 * @brief		Free a parsed header.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	vParseHandle Specifies the parsed header handle returned by #jdi_BEParseHeader
 * @retval		void
 *
 * The application needs to used this API to free a parsed header.
 */
void jdi_BEFreeParsedHeader (JC_HANDLE			vHandle,
							 JC_HANDLE			vParseHandle) ;

/**
 * @brief		Disconnects the data connection
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK if the data connection is diconnected successfully.
 *
 * The application needs to used this API to disconnect the data connection that is
 * used for all lower layer fetch operations. 
 */
JC_RETCODE jdi_BEDisconnect (JC_HANDLE				vHandle) ;

/**
 * @brief		Adds a root certificate.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstRootCert Specifies the root certificate to be added.
 * @retval		JC_OK if the root certificate is added successfully.
 *
 * The application needs to used this API to add a root certificate. The API will 
 * send the add root certificate request to the lower layer.
 */
JC_RETCODE jdi_BEAddRootCertificate (JC_HANDLE				vHandle,
									 const ST_BE_ROOT_CERT	*pstRootCert) ;

/**
 * @brief		Adds a user certificate.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstUserCert Specifies the user certificate to be added.
 * @retval		JC_OK if the user certificate is added successfully.
 *
 * The application needs to used this API to add a user certificate. The API will 
 * send the add user certificate request to the lower layer.
 */

JC_RETCODE jdi_BEAddUserCertificate (JC_HANDLE				vHandle,
									 const ST_BE_USER_CERT	*pstUserCert) ;

/**
 * @brief		Clears all the root certificates that are maintained in the browser.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK if all the root certificates are cleared successfully.
 *
 * The application needs to used this API to clear all the root certificates that are maintained
 * in the browser.
 */
JC_RETCODE jdi_BEClearRootCertificate (JC_HANDLE				vHandle) ;

/**
 * @brief		Clears all the user certificates that are maintained in the browser.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK if all the user certificates are cleared successfully.
 *
 * The application needs to used this API to clear all the user certificates that are maintained
 * in the browser.
 */
JC_RETCODE jdi_BEClearUserCertificate (JC_HANDLE				vHandle) ;

#ifdef BROWSER_COOKIE_ENABLE

/**
 * @brief		Stores cookie to persistent storage
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK if the cookies is stored to persistent storage sucessfully.
 * @retval		JC_ERR_MEMORY_ALLOCATION If a memory error occurs.
 * @note		Define BROWSER_COOKIE_ENABLE to use jdi_BEStorePersistentCookie.
 *
 * The application needs to use this API to store the cookie information to the persistent
 * storage. The API will retrieve the currently stored cookie information and store in to the
 * cookie file. 
 */

JC_RETCODE jdi_BEStorePersistentCookie (JC_HANDLE				vHandle) ;

#endif

#ifdef BROWSER_CACHE_ENABLE

/**
 * @brief		Downloads content and adds it to cache.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstDownloadReq Specifies the download request.
 * @param[in]	puiDownloadID Specifies the ID of the download (if sent successfully).
 * @retval		JC_OK if the download request is sent succesfully.
 * @retval		JC_ERR_BE_INVALID_INPUT If the post data value is sent by the application. 
 *				BE returns this error because the content which has post data cannot be stored within
 *				the cache.
 * @note		Define BROWSER_CACHE_ENABLE to use jdi_BEDownloadToCache.
 *
 * The application needs to use this API when it wants to download a content and add it to 
 * cache (ex: SL cache). The API will initially return JC_OK if the download request is sent
 * to the lower layer successfully. Once the content is downloaded and added to the cache
 * successfully the BE will send the result (success / failure) to the
 * callback type E_BE_DOWNLOAD_TO_CACHE_LISTENER.
 */
JC_RETCODE jdi_BEDownloadToCache (JC_HANDLE				vHandle,
								  const ST_BE_FETCH_REQ	*pstDownloadReq,
								  JC_UINT32				*puiDownloadID) ;

/**
 * @brief		Invalidates a cache record
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pcURI Specifies the URI of the cache record to be invalidated
 * @param[in]	eCacheType Specifies the type of cache invalidation to be executed 
 * @retval		JC_OK if the cache record is invalidated sucessfully
 * @retval		JC_ERR_INVALID_PARAMETER If an invalid value is specified in the field eCacheType
 * @note		Define BROWSER_CACHE_ENABLE to use jdi_BEInvalidateCache.
 *
 * The application needs to use this API to invalidate a cache record. The application 
 * needs to specify type of cache invalidation (whether the cache object or cache service
 * is to be invalidated) and the URI of the cache record which is to be invalidated.
 *
 * If the application receives a push message of type cache operation then this API 
 * should be used. Using this API the cache record, stored in the browser, can be
 * invalidated.
 */

JC_RETCODE jdi_BEInvalidateCache (JC_HANDLE			vHandle,
								  E_BE_INV_CACHE	eCacheType,
								  const JC_INT8		*pcURI) ;

/**
 * @brief		Retrieves requests which can be fetched offline
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	vURLHandle Specifies the URL handle which was previously returned with the same
 *				API
 * @param[out]	ppmURL The API will return the offline URL in this parameter. 
 * @param[out]	pvNextURLHandle The API will return the handle to the next Offline URL in this 
 *				parameter.
 * @retval		JC_OK if the offline URL is retrieved successfully.
 * @retval		JC_ERR_CACHE_EMPTY If the cache is empty.
 * @note		Define BROWSER_CACHE_ENABLE to use jdi_BEGetOfflineURL.
 * @see			jdi_BEFreeOfflineURL
 *
 * The application needs to use this API to get the list of URL's that can be fetched offline.
 * The API is capable of returning only one URL when it is called (not a list of URL). The application
 * has to call this API again to get the next URL in the cache (to avoid high memory usage).
 *
 * The first time this function is called the value of the URL handle (vURLHandle) will be NULL.
 * If the BE is able to retrieve any offline URL then it will return the same in the parameter ppmURL
 * and also return the handle to get the next offline URL (pvNextURLHandle). The application can then 
 * call the same API again, this time the vURLHandle will be the pvNextURLHandle that was returned from
 * the previous call. The same process needs to be followed to get the list of offline URL.
 *
 * The BE can intimate the application that there are no more offline URL's in 2 ways.
 * First way is that the core will return the pvNextURLHandle as NULL and hence no more URL to retrieved.
 * The second way is that it will return the value of ppmURL as NULL and return as JC_OK. i.e. even though
 * the application passed the correct vURLHandle (i.e. pvNextURLHandle of a previous call) the BE could not
 * retrieve any more offline URL (because the offline URL list has finished). The application should not treat the
 * second way as a error. Instead it means that no more offline URL's to be retrieved.
 *
 * The application needs to use the API #jdi_BEFreeOfflineURL to free the URL that is returned from
 * this API.
 *
 * @note The application should not delete the cache while retrieving the offline URL list.
 *
 */
JC_RETCODE jdi_BEGetOfflineURL (JC_HANDLE		vHandle,
								JC_HANDLE		vURLHandle,
								JC_CHAR			**ppmURL,
								JC_HANDLE		*pvNextURLHandle) ;

/**
 * @brief		Frees the offline URL
 *
 * @param[in]		vHandle Specifies the handle returned by BE initialize.
 * @param[in,out]	pmURL Specifies the offline URL to be freed.
 * @retval			void
 * @note			Define BROWSER_CACHE_ENABLE to use jdi_BEFreeOfflineURL.
 *
 * The application needs to use this API to free the URL that it has received after calling
 * the API #jdi_BEGetOfflineURL.
 *
 */
void jdi_BEFreeOfflineURL (JC_HANDLE		vHandle,
						   JC_CHAR			*pmURL) ;
/**
 * @brief		Adds cache operator specific domains.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pcDomain Specifies the operator specific domain to be added.
 * @retval		JC_OK if the domain is added sucessfully
 * @note		Define BROWSER_CACHE_ENABLE to use jdi_BEAddCacheOperatorSpDomain.
 * @see			jdi_BEClearCacheOperatorSpDomains
 *
 * Generally the Browser engine decides about whether the cache content has to be added to operator
 * based on the header received. Browser engine also gives an option of adding the opeartor Specific
 * domain using this API. i.e. if the header contains no operator specific information then the
 * browser engine will also check for the domains that has been added using this API and based on that
 * it decides whether to add to general cache or operator cache.
 *
 */
JC_RETCODE jdi_BEAddCacheOperatorSpDomain (JC_HANDLE		vHandle,
										   const JC_INT8	*pcDomain) ;

/**
 * @brief		Clears the operator specific domains.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @note		Define BROWSER_CACHE_ENABLE to use jdi_BEClearCacheOperatorSpDomains.
 *
 * The application needs to use this API to clear all the operator specific domains that was previously
 * added using the API #jdi_BEAddCacheOperatorSpDomain. 
 *
 * The API will only delete the operator specific domains and also the file which stores the domians. 
 * The content related to the domain will still exist in the opeartor cache (which cannot be retrieved
 * with out the domain names). Its upto the application to decide whether it wants to clear the operator
 * specific cache information. The application can use the API #jdi_BESetEvent with (#E_BE_CACHE_CLEAR)
 * to clear the opeartor specific cache content.
 */
void jdi_BEClearCacheOperatorSpDomains (JC_HANDLE		vHandle) ;

#endif

#ifdef BROWSER_PUSH_ENABLE

/**
 * @brief		Sends a push initialize request.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstPushInit Specifies the Push initialize request.
 * @retval		JC_OK if the Push initialize request is processed successfully.
 * @note		Define BROWSER_PUSH_ENABLE to use jdi_BESendPushInitReq.
 * @see			jdi_BESendPushDeinitReq
 *
 * The application needs to used this API to send a push session initialization
 * request to the lower layer. The application needs to specify the push details
 * like the type of push session (confirmed or HTTP), port and IP address etc in
 * the pstPushInit. The browser will set up a push session by using the parameters
 * specified in pstPushInit.
 */
JC_RETCODE jdi_BESendPushInitReq (JC_HANDLE				vHandle,
								  ST_BE_PUSH_INIT		*pstPushInit) ;

/**
 * @brief		Sends a push deinitialize request.
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiPushSessionID Specifies the ID of the push session to be deinitialized.
 * @retval		JC_OK if the Push deinitialize request is processed successfully.
 * @note		Define BROWSER_PUSH_ENABLE to use jdi_BESendPushDeinitReq.
 *
 * The application needs to used this API when it needs the browser to send a push deinitialize
 * request to the transport. The application needs to specify the ID of the push session to be
 * deinitialized in the parameter uiPushSessionID.
 *
 * Once the push session is initialized the browser will specify the push session ID
 * using the application callback type E_BE_PUSH_LISTENER. The application has to use 
 * this ID while deinitializing the push session.
 */
JC_RETCODE jdi_BESendPushDeinitReq (JC_HANDLE			vHandle,
									JC_UINT32			uiPushSessionID) ;

/**
 * @brief		Sends a push request request.
 * 
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	pstPushRegisterInfo Specifies the information about the type of push to be registered.
 * @retval		JC_OK if the Push register request is processed successfully.
 * @note		Define BROWSER_PUSH_ENABLE to use jdi_BERegisterForPushContent.
 *
 * The application needs to use this API to receive only specific kind of push content.
 * That is the application can specify the ID or the content type of the push content that it wants to recieve.
 * Once registered the communicator will send only registered push content to the applicaton. 
 */
JC_RETCODE jdi_BERegisterForPushContent (JC_HANDLE					vHandle,
										 ST_BE_PUSH_REGISTER_INFO	*pstPushRegisterInfo) ;

#endif

/**
 * @brief		Sends the security indication response.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	bIsSecurityParamAccepted Specifies whether the security is accepted by the application
 * @retval		JC_OK if the Security indication response is sent successfully to the lower layer.
 *
 * Once the security is established the lower layer will send the security details
 * to the application (using E_BE_SECURITY_IND_LISTENER callback type).
 * The transport will suspend the request until the application sends the response of
 * the security indication.
 *
 * The application needs to use this API to send the security indication response. The
 * field bIsSecurityParamAccepted will specify whether the application wants to go ahead with the security.
 * that is negotiated.
 */
JC_RETCODE jdi_BESendSecurityIndResponse (JC_HANDLE			vHandle,
										  JC_BOOLEAN		bIsSecurityParamAccepted) ;

/**
 * @brief		Update the security indication response
 * @param[in] vHandle Specifies the handle returned by BE initialize.
 * @param[in] bIsProceed Specifies whether to proceed with the indication specified.
 * 
 * The application needs to use this API to send the security indication response for the
 * following security indication event:
 * #E_BE_SECURITY_SWITCH
 * #E_BE_SECUREINLINE_IN_NONSECURE
 * #E_BE_NONSECUREINLINE_IN_SECURE
 * #E_BE_NON_SECURE_POST
 */
JC_RETCODE jdi_BEHandleAppSecurityIndicationResp (JC_HANDLE		vHandle,
												  JC_BOOLEAN	bIsProceed) ;

/**
 * @brief		Sends the security failure response.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	bSetNextSecurityClass Specifies whether the application wants to go with next level of security
 * @retval		JC_OK if the Security failure response is sent successfully to the transport.
 *
 * Once a class of security (Class II or Class III) fails the transport will send a security
 * failure indication to the application (using E_BE_SECURITY_FAILURE_LISTENER callback type)
 * The transport will not try to set the next class of security until the application
 * sends the security failure response. 
 *
 * The application needs to use this API to send the security failure response. 
 * The field bSetNextSecurityClass Specifies whether the application wants to
 * go with the next level of security.
 *
 * @note The transport will send the failure indication only for class III and class II
 */
JC_RETCODE jdi_BESendSecurityFailureResponse (JC_HANDLE		vHandle,
											  JC_BOOLEAN	bSetNextSecurityClass) ;

/**
 * @brief		Sends a request to lower layer to retrieve a certificate.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiSecurityReferenceID Sepcifies the security reference ID.
 * @param[in]	uiCertIndex Specifies the index of the certificate to be retrieved.
 * @retval		JC_OK if the request for the certificate is sent successfully.
 *
 * Once the security is established the BE will send the security indication to the application
 * (#ST_BE_SECURITY_IND). In this the BE will pass only one certificate. But there may be more certificates
 * which are received by the browser (while establishing security). To download the other certifocates this
 * API has to be used.
 *
 * In the security indication a security reference ID is passed. The same has to be passed while retrieving
 * the certificates.
 *
 * The uiCertIndex has to be determined based on which index of total cert count (received in security indication)
 * the application wants to receive.
 *
 * The application has to retrieve the certificates before sending ther response of the security indication
 * (i.e. before calling #jdi_BESendSecurityIndResponse). After the security ind response is sent the lower layer
 * will delete the certificates and hence the BE will not be able to retrieve the content.
 */
JC_RETCODE jdi_BEGetCertInfo (JC_HANDLE					vHandle,
							  JC_UINT32					uiSecurityReferenceID,
							  JC_UINT32					uiCertIndex) ;

/**
 * @brief		Suspends a request
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiRequestID Specifies the ID of the request to be suspended.
 * @retval		JC_OK if the suspend request is sent successfully to the transport.
 * @retval		JC_ERR_INVALID_PARAMETER If the particular request cannot be suspended by the 
 *				application.
 *
 * The application needs to call this API to suspend a request. 
 * The application needs to specify the ID of the request to be suspended.
 */
JC_RETCODE jdi_BESuspendRequest (JC_HANDLE		vHandle,
								JC_UINT32		uiRequestID) ;

/**
 * @brief		Resumes a request
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiRequestID Specifies the ID of the request to be resumed
 * @retval		JC_OK if the resume request is sent successfully to the transport.
 * @retval		JC_ERR_INVALID_PARAMETER If the particular request cannot be resumed by the 
 *				application.
 *
 * The application needs to call this API to resume a request which was previously suspended. 
 * The application needs to specify the ID of the request to be resumed.
 */
JC_RETCODE jdi_BEResumeRequest (JC_HANDLE		vHandle,
								JC_UINT32		uiRequestID) ;

/**
 * @brief		Zooms the content currently displayed on the browser window.
 *
 * @param[in]	vHandle     Specifies the handle returned by BE initialize.
 * @param[in]	uhZoomValue Specifies the zoom value.
 * @retval		JC_OK if zooming is done successfully.
 *
 * The application needs to use this API to zoom the content that is currently displayed on the
 * screen. Once the content is zoomed the application can revert back to the original display by
 * specifying the zoom value as 100.
 */
JC_RETCODE jdi_BEZoomTo (JC_HANDLE		vHandle,
						 JC_UINT16		uhZoomValue) ;

/**
 * @brief		Toggles the display of the current page between thumbnail view and normal view
 *
 * @param[in]	vHandle		Specifies the handle returned by BE initialize.
 * @param[in]	uhZoomValue	Specifies Zoom factor to be applied for thumbnail mode. 
 * @retval		JC_OK if the page view is set properly.
 *
 * The application can set the zoom factor for thumbnail mode. If zoom factor is zero, this function
 * calculates the zoom factor based on page size and visible size so that the content gets fit 
 * within visible area for thumbnail view.
 */
JC_RETCODE jdi_BESetThumbnailView (JC_HANDLE	vHandle,
								   JC_UINT16	uhZoomValue) ;

/**
 * @brief		Resizes all the browser windows.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiWidth Specifies the width of the resized browser window
 * @param[in]	uiHeight Specifies the height of the resized browser window.
 * @retval		JC_OK if all the browser windows are resized successully.
 *
 * The application needs to use this API to resize all the browser windows.
 */
JC_RETCODE jdi_BESizeTo (JC_HANDLE		vHandle,
						 JC_UINT32		uiWidth,
						 JC_UINT32		uiHeight) ;

/**
 * @brief		Sets the focus to next visible control.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @retval		JC_OK always
 *
 * The application needs to call this API to set the focus to next visible &
 * selectable control (if any) in the current view.
 */
JC_RETCODE jdi_BESetNextVisibleFocus (JC_HANDLE			vHandle) ;

/**
 * @brief		Closes the browser.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	uiWindowID Specifies the ID of the browser window which needs to be closed.
 * @retval		JC_OK if the browser is closed successfully
 * @see			jdi_BEOpen
 *
 * The application needs to call this API to close the browser. This API does not
 * deinitialize the browser or any of it components. It just deinitializes the display
 * engine which was initialized for the browser window.
 */
JC_RETCODE jdi_BEClose	(JC_HANDLE		vHandle,
						 JC_UINT32		uiWindowID) ;

/**
 * @brief		Deinitializes a browser component.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	eBeComponent Specifies the browser component which is to be deinitialized.
 *
 * The application needs to call this API to deinitialize a browser componet.
 */
void jdi_BEDeinitComponent (JC_HANDLE		vHandle,
							E_BE_COMPONENT	eBeComponent) ;


/**
 * @brief	  The application needs to call this API to deinitialize the browser.
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 *
 * The application needs to call this API to deinitialize the browser.
 */
void jdi_BEDeinitialize (JC_HANDLE	vHandle) ;

/**
 * @brief		Executes a read PPP. 
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	eType Specifies the non-context read PPP type
 * @param[in]	vPPPArg Specifies the PPP argument.
 * @retval		JC_OK if non-context read PPP is executed successfully
 * @retval		JC_ERR_DE_PLUGIN_NOT_FOUND if the plug in to execute the read PPP is not found.
 *
 * The application needs to use this API to execute a read PPP. The API first 
 * searches for the plug in function for read PPP using the plug in type eType. If the plug
 * in function is found then it is called using the PPP argument vPPPArg
 */
JC_RETCODE jdi_BEExecuteReadPPP (JC_HANDLE					vHandle,
								 E_PPP_READ_PLUGIN			eType,
								 JC_HANDLE					vPPPArg) ;

/**
 * @brief		Executes a read write PPP.
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	eType Specifies the context read write PPP type
 * @param[in]	vPPPArg Specifies the PPP argument.
 * @retval		JC_OK if context read write PPP is executed successfully
 * @retval		JC_ERR_DE_PLUGIN_NOT_FOUND if the plug in to execute the read write PPP is not found.
 *
 * The application needs to use this API to execute a read write PPP. The API first 
 * searches for the plug in function for read write PPP using the plug in type eType. If the plug
 * in function is found then it is called using the PPP argument vPPPArg
 */
JC_RETCODE jdi_BEExecuteReadWritePPP (JC_HANDLE						vHandle,
									  E_PPP_READWRITE_PLUGIN		eType,
									  JC_HANDLE						vPPPArg) ;

/** 
 * @brief This function executes the read write upp based on its type
 * @param[in] vHandle  Browser engine handle
 * @param[in] eType    Context Read Write UPP Type
 * @param[in] vUPPArg  Reference to the PPP Argument
 * @retval JC_OK  Success on executing 
 * @retval JC_ERR_DE_PLUGIN_NOT_FOUND Plugin is not registered for this type
 * @retval Others Error on executing
 */
JC_RETCODE jdi_BEExecuteReadWriteUPP (JC_HANDLE						vHandle,
									  E_UPP_READWRITE_PLUGIN		eType,
									  JC_HANDLE						vUPPArg) ;


/**
 * @brief		Gets the list of Link elements
 *
 * @param[in]	vHandle	Specifies the handle returned by BE initialize.
 * @param[out]	ppstPageLinkList		Specifies the list of link elements of the page
 *
 * The application needs to use this API to get the list of Link elements,
 * if any, in the downloaded page. The link elements would then be displayed as menu
 * items for the Links menu. 
 */
void jdi_BEGetPageLinkList (JC_HANDLE				vHandle,
							ST_BE_PAGE_LINK_LIST    **ppstPageLinkList) ;

/**
 * @brief		Activates a link
 *
 * @param[in]	vHandle		Specifies the handle returned by BE initialize.
 * @param[out]	vTagHandle	Reference to the activated link element
 *
 * When a link, from the links displayed as menu options, is selected, the application
 * needs to use this API to pass information about the selected link to the
 * browser, so that appropriate action can be taken
 */
JC_RETCODE jdi_BEActivateLink (JC_HANDLE	vHandle,
							   JC_HANDLE	vTagHandle) ;

/**
 * @brief		Activates an anchor
 *
 * @param[in]	vHandle		Specifies the handle returned by BE initialize.
 * @param[out]	vTagHandle	Reference to the activated link element
 *
 * When a particular anchor is selected from the displayed list, the application
 * needs to use this API to pass information about the selected anchor to the
 * browser, so that appropriate action can be taken
 */
JC_RETCODE jdi_BEActivateAnchor (JC_HANDLE	vHandle,
								 JC_HANDLE	vTagHandle) ;

/**
 * @brief		Sets the accelerator URL
 * @param[in]	vHandle		Specifies the handle returned by BE initialize.
 * @param[in]	pmAcceleratorURL	Specifies the accelerator URL. 
 *
 * If the pmAcceleratorURL is NULL, then the accelerator will be disabled.
 */
JC_RETCODE jdi_BESetAcceleratorURL (JC_HANDLE		vHandle,
									JC_CHAR			*pmAcceleratorURL) ;

/**
 * @brief		Suspends the browser
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	bSuspendLowerLayer	Specifies that lower layer has to be suspended.
 * @retval		JC_OK if the suspend operation is successful.
 * @retval		Other On error.
 *
 * The application needs to call this API to suspend the browser. If application wants 
 * to suspend the lower layer also then bSuspendLowerLayer should be set to E_TRUE. 
 * After this browser will not process any requests and will not accept user actions 
 * until it is resumed again using #jdi_BEResumeBrowser is called.
 */
JC_RETCODE jdi_BESuspendBrowser (JC_HANDLE		vHandle,
								 JC_BOOLEAN		bSuspendLowerLayer) ;

/**
 * @brief		Resumes the browser
 *
 * @param[in]	vHandle Specifies the handle returned by BE initialize.
 * @param[in]	bResumeLowerLayer Specifies that lawer layer has to be resumed.
 * @retval		JC_OK if the resume operation is successful.
 * @retval		Others On error.
 *
 * The application needs to call this API to resume the browser which is already suspended. 
 * If lower layer aslo has been suspended then bResumeLowerLayer should be st to E_TRUE 
 * so that lower layer also will be resumed. After this browser will start its normal processing.
 */
JC_RETCODE jdi_BEResumeBrowser (JC_HANDLE		vHandle, 
								JC_BOOLEAN		bResumeLowerLayer) ;

/**
 * @brief This function takes the index as input parameter and moves back or front in navigation depending on 
 *								index value i.e., back if index negative and front if index is positive
 * @param[in]	vHandle			Specifies the handle returned by DE initialize.
 * @param[in]	hIndex			Index value for navigation
 * retVal		returns JC_OK if successs else appropriate error.
**/
JC_RETCODE	jdi_BENavigateByIndex (JC_HANDLE					vHandle,
								   JC_INT16						hIndex) ;

/**
 * @brief This function gets the navigation handle
 * @param[in]	vHandle				Specifies the handle returned by DE initialize.
 * @param[in, out]	pvNavgHandle	Specifies the handle to hold the navigation handle.
 * retVal		returns JC_OK if successs else appropriate error.
**/
JC_RETCODE	jdi_BEGetNavigationHandle (JC_HANDLE		vHandle,
									   JC_HANDLE		*pvNavgHandle) ;

/**
 * @brief This function forms and returns the absolute URI from the relative URI passed
 * @param[in]  vHandle        Browser Engine Handle
 * @param[in]  pmRelativeURI  Reference to relative URI
 * @param[out] ppmAbsoluteURI Reference to hold the absolute URI
 * @retval JC_OK  Success on getting the absolute URI
 * @retval Others Error on getting the absolute URI
 */
JC_RETCODE jdi_BEGetAbsoluteURI (JC_HANDLE		vHandle,
								 JC_CHAR		*pmRelativeURI,
								 JC_CHAR		**ppmAbsoluteURI) ;

/**
 * @brief This function frees the absolute URI returned by jdi_DEGetAbsoluteURI 
 * @param[in] vHandle       Browser Engine Handle
 * @param[in] pmAbsoluteURI Reference to absolute URI to be freed
 * @return No return value
 */
void jdi_BEFreeAbsoluteURI (JC_HANDLE		vHandle,
							JC_CHAR			*pmAbsoluteURI) ;

#ifdef BROWSER_IMODE_ATTRIBUTES

/**
 * @breif This function sends the response of the user for the ID number
 * @param[in]  vHandle  Browser engine handle
 * @param[in]  pstIDResp	Specifies the user response whether to send the ID along with User-Agent header or not
 * @retval JC_OK  On Success
 * @retval Others On Failure
 *
 * This function should be called by application to send the user response, whether
 * ID number has to be sent along with the User-Agent header or not. This function will be used 
 * when the callback #E_BE_ID_NUMBER_LISTENER is implemented as non blocking. If the user 
 * says yes then pstIDResp contains the UserAgent header value which should be sent to server.
 */
JC_RETCODE jdi_BESendIDNumberResponse (JC_HANDLE			vHandle,
									   ST_BE_UTN_ID_RESP	*pstIDResp) ;

#endif

#ifdef BROWSER_DEBUG_ENABLE

/**
 * @breif This function returns the current active DE handle 
 * @param[in]  vHandle    Browser engine handle
 * @param[out] pvDEHandle Reference to hold the DE handle
 * @retval JC_OK  Success on getting DE Handle
 * @retval Others Error on getting the DE Handle
 */
JC_RETCODE jdi_BEGetDEHandle (JC_HANDLE		vHandle,
							  JC_HANDLE		*pvDEHandle) ;

#endif


/**
 * @brief The error code is returned when the browser is denied access to fetch 
 *		  or show a particular content.
 */
#define JC_ERR_BE_ACCESS_DENIED					TERMINAL_ERR_CODE(BE_MODULE, 0x01)
/**
 * @brief The error code is returned when the browser comes across a content type which 
 *		  is not supported.
 */
#define JC_ERR_BE_UNSUPPORTED_CONTENT_TYPE		TERMINAL_ERR_CODE(BE_MODULE, 0x02)
/**
 * @brief The error code is returned when the size of the page to be displayed exceeds the
 *		  maximum size that can be displayed on the browser.
 */
#define JC_ERR_BE_PAGE_SIZE_EXCEEDS				TERMINAL_ERR_CODE(BE_MODULE, 0x03)
/**
 * @brief The error code is returned when the input provided to the browser is not valid.
 */
#define JC_ERR_BE_INVALID_INPUT					TERMINAL_ERR_CODE(BE_MODULE, 0x04)
/**
 * @brief The error code is returned when a particular URL is getting redirected more than 
 *		  the maximum of redirections allowed by the browser.
 */
#define JC_ERR_BE_EXCEEDS_MAX_REDIRECT			TERMINAL_ERR_CODE(BE_MODULE, 0x05)
/**
 * @brief The error code is returned if the application reqeusts for a particular history record
 *		  and the record is not found.
 */
#define JC_ERR_BE_HISTORY_NOT_FOUND				TERMINAL_ERR_CODE(BE_MODULE, 0x06)
/**
 * @brief The error code is returned if the history is full.
 */
#define JC_ERR_BE_HISTORY_FULL					TERMINAL_ERR_CODE(BE_MODULE, 0x07)
/**
 * @brief The error specifies the HTTP status.
 */
#define JC_ERR_BE_HTTP_STATUS					TERMINAL_ERR_CODE(BE_MODULE, 0x08)
/**
 * @brief The error specifes that the page recieved is an empty page
 */
#define JC_ERR_BE_EMPTY_PAGE					TERMINAL_ERR_CODE(BE_MODULE, 0x09)
/**
 * @brief The error code is returned by the application when it wants the browser to 
 *		  suspend a particular request.
 */
#define JC_ERR_BE_SUSPEND_REQUEST				TERMINAL_ERR_CODE(BE_MODULE, 0x0A)
/**
 * @brief The error code Specifies that the cache revalidate request has to be sent.
 */
#define JC_ERR_BE_CACHE_REVALIDATE				TERMINAL_ERR_CODE(BE_MODULE, 0x0B)
/**
 * @brief The error code Specifies that the downloaded content is not cachable
 */
#define JC_ERR_BE_CANNOT_CACHE					TERMINAL_ERR_CODE(BE_MODULE, 0x0C)

/**
 * @brief The error code specifies that the application will take care of handling the content
 * and indicating its result at a later point of time using the API #jdi_BEHandleContentIndication.
 */
#define JC_ERR_BE_CONTENT_HANDLED				TERMINAL_ERR_CODE(BE_MODULE, 0x0D)


#ifdef __cplusplus
}
#endif

#endif

/* End of File */



