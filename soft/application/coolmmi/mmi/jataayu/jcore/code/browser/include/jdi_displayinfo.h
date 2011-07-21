/***************************************************************************
 * $Id: jdi_displayinfo.h,v 1.104 2009/03/10 10:55:06 shajudevhtmlbrow Exp $
 * $Revision: 1.104 $
 * $Date: 2009/03/10 10:55:06 $
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
 * $Log: jdi_displayinfo.h,v $
 * Revision 1.104  2009/03/10 10:55:06  shajudevhtmlbrow
 * appvesrion changed to pass more information like platform and language
 *
 * Revision 1.103  2009/03/04 13:04:13  kavithadevhtmlbrow
 * Doxygen Comments Updated
 *
 * Revision 1.102  2009/02/26 04:15:42  sudhadevhtmlbrow
 * Pannaing support in thumbnail mode.
 *
 * Revision 1.101  2009/02/13 09:46:03  arpitadevhtmlbrow
 * E_BE_STYLUS_TYPE compilation error
 *
 * Revision 1.100  2009/02/13 06:07:32  sudhadevhtmlbrow
 * Handling of STYLUS_MOVE event type for pannaing support
 *
 * Revision 1.99  2009/01/21 14:32:13  kavithadevhtmlbrow
 * Popup Blocker for Window.Open
 *
 * Revision 1.98  2008/12/19 07:50:27  prabhatdevhtmlbrow
 
 *
 * Revision 1.97  2008/12/17 12:00:24  kavithadevhtmlbrow
 * window.open - updated with window count and window.closed and other issues
 *
 * Revision 1.96  2008/12/17 09:28:33  kavithadevhtmlbrow
 * UserAgent in Application Info
 *
 * Revision 1.95  2008/12/16 07:23:53  kavithadevhtmlbrow
 * DE_WINDOW_OPEN_ENABLE to BROWSER_WINDOW_OPEN
 *
 * Revision 1.94  2008/12/10 14:46:16  kavithadevhtmlbrow
 * window.open Support - DE_WINDOW_OPEN_ENABLE
 *
 * Revision 1.93  2008/11/18 12:28:40  kavithadevhtmlbrow
 * Setting Default Encoding from Property
 *
 * Revision 1.92  2008/11/13 08:59:58  kavithadevhtmlbrow
 * Chunk Size as Property
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_displayinfo.h
 * @ingroup BrEngine
 * @brief Defines the exposed enumerations, structures of the browser engine that
 *		  can be used by the communicator, application and display engine modules.
 * 
 * This file defines the various structures and enumnerations that are used for the interface 
 * between the browser, communicator and display engine modules.
 */

#ifndef _JDI_DISPLAY_INFO_H
#define _JDI_DISPLAY_INFO_H

/** @cond */
#include <jdi_httpstatus.h>
#include <jdi_mimeinfo.h>
/** @endcond */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief The enumeration to be used to specify the display mode
 */
typedef enum e_be_display_mode
{
	E_DISPLAY_NORMAL_MODE, /**< Specifies normal display mode */
	E_DISPLAY_FIT_MODE, /**< Specifies to Fit To screen mode */
	E_DISPLAY_BESTFIT_MODE, /**< Specifies Best Fit mode */
	E_DISPLAY_THUMBNAIL_MODE, /**< Specifies Thumbnail mode */
	E_DISPLAY_3D_MODE /**< Specifies 3D mode */
} E_BE_DISPLAY_MODE ;

/**
 * @brief The emueration to be used to specify the position of the access key
 */
typedef enum e_be_accesskey_display
{
	E_ACCESSKEY_AT_END, /**< Specifies that access key is to set at the end */
	E_ACCESSKEY_AT_BEGIN, /**< Specifies that access key is to set at the beginning */
	E_ACCESSKEY_NO_DISPLAY, /**< Specifies that the access key vlaue will not be displayed */
	E_ACCESSKEY_NONE /**< Specifies that access key is not set */
} E_BE_ACCESSKEY_DISPLAY ;

/**
 * @brief The emueration to be used to specify the referer header mode. 
 */
typedef enum e_be_referer_header_mode
{
	E_REFERER_PAGE_ONLY, /**< Specifies that the referer header should be sent for the page request only */
	E_REFERER_ALL_REQUESTS, /**< Specifies that referer header should be sent for page and inline requests */
	E_REFERER_DUMMY, /**< Specifies that a dummy referer header should be added for all the requests **/
	E_REFERER_OFF /** Specifies that referer header should not be added for any request */
} E_BE_REFERER_HEADER_MODE ;

/**
 * @brief The enumeration used to specify the scroll mode.
 */
typedef enum e_be_scroll_mode
{
	E_SCROLL_SLOW, /**< Specifies that the scroll speed will be slow */
	E_SCROLL_MEDIUM, /**< Specifies that the scroll speed will be medium */
	E_SCROLL_FAST, /**< Specifies that the scroll speed will be fast */
	E_SCROLL_LEVEL /**< Specifies that the scrolling speed will be specified by the application  */
} E_BE_SCROLL_MODE ;

/**
 * @brief The enumeration used to specify the scroll direction.
*/
typedef enum e_be_scroll_direction
{
	E_SCROLL_UP, /**< Specifies the scroll up direction */
	E_SCROLL_DOWN, /**< Specifies the scroll down direction */
	E_SCROLL_LEFT, /**< Specifies the scroll left direction */
	E_SCROLL_RIGHT /**< Specifies the scroll right direction */
} E_BE_SCROLL_DIRECTION ;

#ifdef BROWSER_MOUSE_ENABLE
/**
 * @brief The application needs to specify this event type every time mouse operation is to 
 *		  be performed by the browser
 */
typedef enum e_be_mouse_type
{
	E_MOUSE_WINDOW, /**< Specifies that the mouse operation is to be done on the browser created dynamic windows */
	E_MOUSE_VSCROLLBAR, /**< Specifies that the mouse operation is to be done on the vertical scroll bar of the browser */
	E_MOUSE_HSCROLLBAR, /**< Specifies that the mouse operation is to be dont on the horizontal scroll bar of the browser */
	E_MOUSE_MOVEEVENT /**< Specifies that the mouse operation to be done based upon where the mouse is moved w.r.t the main window */
} E_BE_MOUSE_TYPE ;
#else
/**
 * @brief The application needs to specify this event type every time stylus operation is to 
 *		  be performed by the browser
 */
typedef enum e_be_stylus_type
{
	E_STYLUS_WINDOW, /**< Specifies that the stylus operation is to be done on the browser created dynamic windows */
	E_STYLUS_VSCROLLBAR, /**< Specifies that the stylus operation is to be done on the vertical scroll bar of the browser */
	E_STYLUS_HSCROLLBAR, /**< Specifies that the stylus operation is to be dont on the horizontal scroll bar of the browser */
	E_STYLUS_MOVE /**< Specifies that the stylus operation to be done based upon where the stylus is moved w.r.t the main window */
} E_BE_STYLUS_TYPE ;
#endif

/**
 * @brief This enum specifies the various CSS types which the application may
 *		  set the default CSS for.
 */
typedef enum e_be_default_css_type
{
	E_WML1_CSS, /**< Specifies the default CSS for WML1 */
	E_WML2_CSS, /**< Specifies the default CSS for WML2 */
	E_COMMON_CSS /** Specifies the default Common CSS type */
} E_BE_DEFAULT_CSS_TYPE ;

/**
 * @brief Specifies the enumeration for asynchronous event type 
 */
typedef enum e_be_async_event_type
{
	E_ASYNC_CTRL	/**< Specifies that the async event is for control */
} E_BE_ASYNC_EVENT_TYPE ;

/**
 * @brief Specifies the enumeration for mime types supported 
 */
typedef enum e_be_mime_type
{
	E_MIME_PAGE_TYPE, /**< Specifies that the mime type corresponds to a page request */
	E_MIME_IMAGE_TYPE, /**< Specifies that the mime type corresponds to an image request */
	E_MIME_OBJECT_TYPE, /**< Specifies that the mime type corresponds to an object request */
	E_MIME_CSS_TYPE, /**< Specifies that the mime type corresponds to a CSS request */
	E_MIME_DRM_TYPE, /**< Specifies that the mime type corresponds to a DRM request */
	E_MIME_UNKNOWN_TYPE /**< Specifies that the mime type is unknown to the browser. */
} E_BE_MIME_TYPE ;


/**
 * @brief Specifies the enumeration for 
 */
typedef enum e_be_control_action
{
	E_ACTION_SAVE,	  /**< Specifies that the control is associated with an action 
					  	   and the mime content (for image/object) associated with 
						   it can be saved  */
	E_ACTION_NOSAVE,  /**< Specifies that the control is associated with an action 
						   and the mime content (for image/object) associated with 
						   it cannot be saved  */
	E_NOACTION_SAVE,  /**< Specifies that the control is not associated with any 
						   action and the mime content (for image/object) associated 
						   with it can be saved  */
	E_NOACTION_NOSAVE /**< Specifies that the control is not associated with any 
						   action and the mime content (for image/object) associated 
						   with it cannot be saved  */
} E_BE_CONTROL_ACTION ;

/**
 * @brief Specifies the type of the focused button control
 */
typedef enum e_be_button_type
{
	E_BUTTON_SUBMIT,	/**< Specifies that the type of the button currently focused is
							 a submit button */
	E_BUTTON_RESET,		/**< Specifies that the type of the button currently focused is
							 a reset button */
	E_BUTTON_NORMAL,	/**< Specifies that the type of the button currently focused is
							 a normal button */
	E_BUTTON_FILE,		/**< Specifies that the type of the button currently focused is
							 a file type button */
	E_BUTTON_DO			/**< Specifies that the type of the button currently focused is
							 a do control */
} E_BE_BUTTON_TYPE ;

/**
 * @brief Specifies the display mode property
 */
typedef struct st_be_display_mode_info
{
	E_BE_DISPLAY_MODE		eDisplayMode ; /**< Specifies the display mode */
	JC_UINT32				uiMaxWidth ; /**< Specifies the maximum width of the browser window */ /* Only for Normal & BestFit Mode */
	JC_BOOLEAN				bTextOnly ; /**< Specifies that only text is to be displayed */
	JC_BOOLEAN				bStrictXHTML ; /**< Specifies whether XHTML be handled strictly */
	JC_BOOLEAN				bDisplayDirectImage ; /**< Specifies whether for an Image URL the browser has to display or not */
	JC_BOOLEAN				bDisplayAHref ; /**< Specifies whether Href controls have to be displayed */
	JC_BOOLEAN				bMetaRefresh ; /**< Specifies whether Meta Refresh is to be ignored or applied */
	JC_BOOLEAN				bNoDisplayAdjust ; /**<Specifies whether the display can be adjusted to avoid horizontal scroll bar */
	JC_UINT32				uiMinFontSize ; /**< Specifies the minimum font size value to be applied to any fetched page.
												Is applied over and above the text size value specified by the user. */
	E_BE_ACCESSKEY_DISPLAY	eLinkAccessKey ; /**< Specifies the postition at which the link access key needs to be displayed */
	E_BE_ACCESSKEY_DISPLAY	eFormAccessKey ; /**< Specifies the position at which the form access key needs to be displayed */
} ST_BE_DISPLAY_MODE_INFO ;


/**
 * @brief Specifies the visibility property of the browser
 */
typedef struct st_be_option_visible
{
	JC_BOOLEAN 		bIsVisibility ; /**< Specifies the visibilty of the browser window */
} ST_BE_OPTION_VISIBLE ;


/**
 * @brief The application needs to specify the various graphics state options in this structure.
 */
typedef struct st_be_option_graphic_state
{
	JC_BOOLEAN 				bIsVisibility ; /**< Specifies whether graphics is visible or not */
	JC_BOOLEAN				bIsSmartImage ; /**< Specifies whether the smart image concept (of using pluggins to validate an Image URI) is enabled or not */
} ST_BE_OPTION_GRAPHICS ;


/**
 * @brief The application needs to specify the audio options using the following structure
 */
typedef struct st_be_option_audio
{
	JC_BOOLEAN				bEnable ; /**< Specifies whether the audio is enabled in browser or not */
	JC_BOOLEAN				bIsSmartAudio ; /**< Specifies whether the smart audio conecpt (of using pluggins to validate an audio URI) is enabled or not */
} ST_BE_OPTION_AUDIO ;


/**
 * @brief The enumeration needs to be used to specify whether the image can be selected or not
 */
typedef struct st_be_option_image_selection
{
	JC_BOOLEAN				bSelectable ; /**< Specifies whether image is selectable or not */
} ST_BE_OPTION_IMAGE_SELECTION ;

/**
 * @brief The enumeration needs to be used to specify the referer header mode.
 */
typedef struct st_be_option_refere_header_mode
{
	E_BE_REFERER_HEADER_MODE	eRefererHeaderMode ; /**< Specifies the referer header mode */
} ST_BE_OPTION_REFERER_HEADER_MODE ;

/**
 * @brief The enumeration needs to be used to specify the magnification value.
 */
typedef struct st_be_option_magnification
{
	JC_UINT16				uhMagnification ; /**< Specifies the magnification value */
	JC_UINT16				uhTextMagnification ; /**< Specifies the text magnification value */
} ST_BE_OPTION_MAGNIFICATION ;


/**
 * @brief Specifies the scroll mode property
 */
typedef struct st_be_option_scroll_mode
{
	E_BE_SCROLL_MODE		eMode ; /**< Specifies the scroll mode */
	JC_UINT32				uiLevel ; /**< Specifies the scroll level */
	JC_BOOLEAN				bFourKeyScrolling ;
	JC_BOOLEAN				bIntelligentPointer ; /**< Specified whether cursor scrolling is enabled/disabled */
} ST_BE_OPTION_SCROLL_MODE ;


/**
 * @brief Specifies the scroll direction and the offset property
 */
typedef struct st_be_scroll_no_focus
{
	E_BE_SCROLL_DIRECTION	eDirection ; /**< Specifies the scroll direction */
	JC_INT32				iOffset ; /**< Specifies the Offset value */
} ST_BE_SCROLL_NO_FOCUS ;


/**
 * @brief Specifies the do element display property
 */
typedef struct st_be_option_do_display
{
	JC_BOOLEAN		bNoDisplay ; /**< Specifies whether the DO element needs to be displayed */
} ST_BE_OPTION_DO_DISPLAY ;



/**
 * @brief Specifies the style property
 */
typedef struct st_be_option_style
{
	JC_INT8			*pcBuffer ; /**< Specifies CSS style property to be applied */
	JC_UINT32		uiBufferLen ;  /**< Specifies the buffer length */
	JC_UINT32		uiFontSize ;
} ST_BE_OPTION_STYLE ;


/**
 * @brief Specifies the encoding property
 */
typedef struct st_be_option_encoding
{
	JC_INT8				*pcEncoding ;  /**< Specified the charset encoding */
} ST_BE_OPTION_ENCODING ;

/**
 * @brief Specifies the CSS properties
 */
typedef struct st_be_option_css
{
	const JC_INT8	**ppcCSSMediaTypes ; /**< Specifies the Supported CSS Media Type */
	JC_BOOLEAN		bAuthorCSSEnable ; /**< Specifies whether Author CSS would be applied to the page or not */
	JC_BOOLEAN		bEmbedStyleAsBlock ; /**< Specifies whether Embedded style for any block would be maintained separately in the embedded list or not */
} ST_BE_OPTION_CSS ;

/**
 * @brief The following structure is used for passing the default CSS values from the application to DE.
 */
typedef struct st_be_default_css
{
	E_BE_DEFAULT_CSS_TYPE		eType ; /**< Specifies the default CSS types that need to be set by the application.*/
	JC_INT8						*pcBuffer ; /**< Specifies the default CSS buffer. */
	JC_UINT32					uiBufLen ; /**< Specifies the CSS buffer length. */
	JC_BOOLEAN					bUseBufMem ; /**< Specifies whether DE can directly use the memory associated with the passed Buffer instead of making a copy. */
} ST_BE_DEFAULT_CSS ;

/**
 * @brief Specifies the performace related information
 */
typedef struct st_be_performance
{
	JC_BOOLEAN				bNoRedrawTree ; /**< Specifies not to re-draw the content. */
	JC_UINT32				uiLayoutBlockSize ; /**< Specifies the layout block size - zero takes default value */
	JC_UINT32				uiStackBlockSize ; /**< Specifies the stack block size during layout - zero takes default value */
} ST_BE_PERFORMANCE ;

#ifdef BROWSER_MOUSE_ENABLE
/**
 * @brief Specifies mouse information and its type
 */
typedef struct st_be_stylus_info
{
	E_BE_MOUSE_TYPE		eType ; /**< Specifies the mouse type */
	union u_mouse_data
	{
		JC_UINT32			uiPosition ;   /**< Specifies the position of the mouse in scrollbar */
		JC_MOUSE_EVENT		stMouseEvent ;  /**< Specifies the mouse event data */
	} uMouseData ;  /**< Specifies union for position and control ID */
} ST_BE_MOUSE_INFO ;
#else
/**
 * @brief Specifies stylus information and its type
 */
typedef struct st_be_stylus_info
{
	E_BE_STYLUS_TYPE		eType ; /**< Specifies the stylus type */
	union u_stylus_data
	{
		JC_UINT32			uiPosition ;   /**< Specifies the position of the stylus in scrollbar */
		JC_STYLUS_EVENT		stStylusEvent ;  /**< Specifies the stylus event data */
	} uStylusData ;  /**< Specifies union for position and control ID */
} ST_BE_STYLUS_INFO ;
#endif

/**
 * @brief Specifies asynchronous event type and data
 */
typedef struct st_be_async_event_data
{
	E_BE_ASYNC_EVENT_TYPE		eType ;			/**< Specifies the asynchronous event type */
	void						*pvEventData ;  /**< Specifies the event data specific to the event type */
} ST_BE_ASYNC_EVENT_DATA ;


/**
 * @brief Specifies the information for multimedia content 
 */
typedef struct st_be_multimedia_content
{
	JC_OBJECT_INFO			stObjectInfo ;  /**< Specifies the object information */
	JC_UINT32				uiDRMRefID ;	/**< Specifies the DRM reference ID if the content is DRM parsed content */
} ST_BE_MULTIMEDIA_CONTENT ;

typedef ST_AUTH_HEADERS		ST_BE_AUTH_INFO ;

/**
 * @brief Specifies post data info
 */
typedef union u_be_post_data
{
	JC_INT8			*pcPostData ;
	JC_CHAR			*pmFileName ;
} U_BE_POST_DATA ;

/**
 * @brief Specifies post data info
 */
typedef struct st_be_post_data
{
	JC_UINT32		uiPostLen ;
	JC_BOOLEAN		bIsFileData ;
	U_BE_POST_DATA	uPostData ;
} ST_BE_POST_DATA ;

/**
 * @brief Specifies fetch request info
 */
typedef struct st_be_fetch_req
{
	JC_CHAR						*pmURI ; /**< Specifies the URL to be fetched */
	ST_BE_POST_DATA				*pstPostData ; /** Contains the post data information */ 
	ST_BE_AUTH_INFO				*pstAuthInfo ; /**< Specifies the authorization information */
	ST_BE_AUTH_INFO				*pstProxyAuthInfo ; /**< Specifies the proxy authorization information */
	ST_MIME_HEADERS				*pstMimeHeaders ; /**< Specifies the Mime Header information */
	JC_BOOLEAN					bIsReloadPage ; /**< If this flag is set then the Page will be reloaded and will not be taken from the cache  */
} ST_BE_FETCH_REQ ;

/**
 * @brief This structure is used by the application or BE to send the timer response back to the
 *		  display engine.
 */
typedef struct st_be_timer_resp
{
	JC_HANDLE		vTimerHandle ; /**< Specifies the timer handle. The application needs to return this handle while sending the timer response */
	JC_UINT32		uiTimerID ; /**< Specifies the timer ID */
} ST_BE_TIMER_RESP ;

/**
 * @brief Specifies the page progress information
 */
typedef struct st_be_page_progress
{
	JC_UINT32		uiBytesRecvd ; /**< Specifies the bytes received */
	JC_UINT32		uiTotalImages ; /**< Specifies the total number of images to be displayed */
	JC_UINT32		uiImagesRecvd ; /**< Specifies the total number of images received so far */
	JC_UINT32		uiTotalCss ; /**< Specifies the total number of CSS content in the page to be fetched */
	JC_UINT32		uiCssRecvd ; /**< Specifies the CSS contents received so far */
	JC_UINT32		uiTotalObjects ; /**< Specifies the total number of objects to be received */
	JC_UINT32		uiObjectRecvd ; /**< Specifies the objects received so far */
} ST_BE_PAGE_PROGRESS ;

 /**
  * @brief The browser uses this structure to send the page info details to the application.
  */
typedef struct st_be_page_info
{
	JC_CHAR				*pmURI ; /**< Specifies the URL of the page */
	JC_CHAR				*pmTitle ; /**< Specifies the Title of the page */
	JC_INT8				*pcPostData ; /**< Specifies the post data of the page request */
	JC_UINT32			uiPostLen ; /**< Specifies the post data length */
	ST_MIME_HEADERS		*pstHeaders ; /**< Specifies the headers */
	JC_BOOLEAN			bFreeMemory ; /**< Specifies whether the application has to free the memory of the structure after use */
} ST_BE_PAGE_INFO ;

/**
 * @brief While specifying the page content the buffer for the page can be specified as a Unicode
 *		  buffer or in a file name. The following union is to be used to specify whether the buffer
 *		  is specified as unicode buffer or in a file.
 */
typedef union u_be_page_buffer
{
	JC_UINT8		*pucBuffer ; /**< Specifies the buffer content */
	JC_CHAR			*pmFileName ; /**< Specifies the file where the buffer is stored */
} U_BE_PAGE_BUFFER ;

/**
 * @brief Specifies information about the Page and its content
 */
typedef struct st_be_page_content
{
	ST_BE_PAGE_INFO		stPageInfo ; /**< Specifies the page information */
	JC_INT8				*pcContentType ; /**< Specifies the content type of the page */
	JC_BOOLEAN			bIsFileContent ; /**< Specifies whether the content of the page is specified in the file. */
	U_BE_PAGE_BUFFER	uBuffer ; /**< Specifies whether content of the page is specified in buffer or in a file */
	JC_UINT32			uiBufferLen ; /**< Specifies the buffer length */
	E_HTTP_STATUS		eStatus ; /**< Specifies the lower layer status */
} ST_BE_PAGE_CONTENT ;

/**
 * @brief Specifies url content info
 */
typedef struct st_be_url_content
{
	JC_CHAR				*pmURI ; /**< Specifies the URL */
	JC_BOOLEAN			bIsFileContent ; /**< Specifies whether the content is specifies in the buffer or file. */
	U_BE_PAGE_BUFFER	uBuffer ; /**< Specifies whether content of the page is specified in buffer or in a file */
	JC_UINT32			uiBufLen ; /**< Specifies the length of the buffer */
	JC_INT8				*pcContentType ; /**< Specifies the content type */
	ST_MIME_HEADERS		*pstHeaders ; /**< Specifies the headers */
	E_HTTP_STATUS		eHttpStatus ; /**< Specifies the lower layer status */
	JC_BOOLEAN			bHasMoreData ; /**< Specifies whether more data is to be received for the URL */
} ST_BE_URL_CONTENT ;


/**
 * @brief The following structure specifies the details of a control displayed on the browser.
 */
typedef struct st_be_control_info			
{
	EControlType			eType ; /**< Specifies the control type */
	JC_BOOLEAN				bIsSelectable ; /**< Specifies whether the control is selectable or not */
	JC_BOOLEAN				bIsSubwindow ; /**< Specifies whether the control is a subwindow or not */
	E_BE_CONTROL_ACTION		eAction ;  /**< Specifies the control activity information */
	JC_UINT32				uiCtrlID ; /**< Specifies the control ID */
	JC_UINT32				uiDRMRefID ; /**< Specifies the DRM Reference ID if focus control content is DRM content */
	JC_CHAR					*pmTitle ; /**< Specifies the title of the control if specifed */
	JC_CHAR					*pmURI ; /**< Specifies the URI of the control if its an image */		/** Control is of type Image  **/
	ST_MIME_HEADERS			*pstHeaders ; /**< Specifies the headers of the control if its an image */	/** Control is of type Image  **/
} ST_BE_CONTROL_INFO ;


/**
 * @brief The browser uses the following structure to send the scrolling info to the application
 */
typedef struct st_be_scroll_info
{
	JC_UINT32		uiMaxAxis ; /**< Specifies the maximum axis value to which the scrolling can be done */
	JC_UINT32		uiCurrAxis ; /**< Specifies the current axis value */
	JC_BOOLEAN		bScrollDownRight ; /**< Specifies that if it is horizontal scrolling then scrolling has to be done to the right or 
											if it is vertical scrolling then scrolling has to be done in downward direction   */
	JC_BOOLEAN		bScrollUpLeft ; /**< Specifies that if it is horizontal scrolling then scrolling has to be done to the left or 
										 if it is vertical scrolling then scrolling has to be done in the upward direction   */
} ST_BE_SCROLL_INFO ;

/**
 * @brief The browser uses the following structure to send the list of link elements of a page to t
 *		  the application.
 */
typedef struct st_be_page_link_list
{
	JC_CHAR							*pmRel ; /**< Specifies the rel (relationship) value for the Link element */
	JC_CHAR							*pmTitle ; /**< Specifies the title value to be displayed for the Link element */
	JC_HANDLE						vElementHandle ; /**< Specifies the handle for the Link element */
	struct st_be_page_link_list		*pstNext ; /**< Specifies the pointer for the next Link element */
} ST_BE_PAGE_LINK_LIST ;


/**
 * @brief Specifies information about anchors in a page 
 */
typedef struct st_be_anchor_list
{
	JC_CHAR						*pmURI ; /**< Specifies the URI of the anchor */
	JC_CHAR						*pmInline ; /**< Specifies the inline content of the anchor URI */
	JC_HANDLE					vTagHandle ; /**< Specifies the handle to the anchor element */
	struct st_be_anchor_list	*pstNext ; /**< Specifies the next anchor details */
} ST_BE_ANCHOR_LIST ;

/**
 * @brief Specifies the details of all the anchor elements in a page 
 */
typedef struct st_be_anchor_data
{
	JC_UINT32			uiAnchorCount ; /**< Specifies the number of anchor elements whose details is specified in the anchor list */
	ST_BE_ANCHOR_LIST	*pstAnchorList ; /**< Specifies the list containing the details of the all the anchor elements */
} ST_BE_ANCHOR_DATA ;

/**
 * @brief The browser uses the following structure to send the do element details to the application
 */
typedef struct st_be_page_do_list				/** E_BE_DO_HANDLER_CALLBACK  **/
{
	JC_CHAR						*pmType ; /**< Specifies the do element type */
	JC_CHAR						*pmLabel ; /**< Specifies the label of the do element */
	JC_HANDLE					vHandle ; /**< Specifies the handle associated with the do element details */
	struct st_be_page_do_list	*pstNext ; /**< Pointer to the next do element structure */
} ST_BE_PAGE_DO_LIST ;


/**
 * @brief The DE uses this enumeration to specify whether the forward or backward navigation is
 *        possible for the particular page being displayed
 */
typedef struct st_be_navigation_info	
{
	JC_BOOLEAN		bBackNavigation ; /**< Specifies whether backward navigation is possible */
	JC_BOOLEAN		bForwardNavigation ; /**< Specifies whether forward navigation is possible */
} ST_BE_NAVIGATION_INFO ;


/**
 * @brief Specifies the information for begin page listener 
 */
typedef struct st_be_begin_page_info
{
	JC_CHAR			*pmURI ;	/**< Specifies the URL of the page */
	JC_BOOLEAN		bFileUpload ; /**< Sets this flag if the request given has file upload data */
} ST_BE_BEGIN_PAGE_INFO ;


/**
 * @brief Specifies the information for end page listener
 */
typedef struct st_be_end_page_info
{
	ST_BE_NAVIGATION_INFO	stNavg ;	/**< Specifies the Navigation info */
	JC_RETCODE				rCode ;			/**< Specifies the success/error code of the page */
	E_HTTP_STATUS			eStatus ;	/**< Specifies the http status code of the page */
	JC_RETCODE				rPageStatusCode ;
	JC_BOOLEAN				bRequestPending ;
#ifdef BROWSER_WEB_SPIDER_ON
	JC_UINT32				uiInternalScript ;
	JC_UINT32				uiExternalScript ;
	JC_UINT32				uiTotalCtrl ;
	JC_UINT32				uiTotalImgCtrl ;
	JC_UINT32				uiTotalSubwindow ;
	JC_UINT32				uiFloatSubwindow ;	
#endif
} ST_BE_END_PAGE_INFO ;

/**
 * @brief Specifies the information for end display listener
 */
typedef struct st_be_end_display_info
{
	JC_BOOLEAN			bRedraw ;	/** Specifies whether browser goes for redraw - this is before script processing */
} ST_BE_END_DISPLAY_INFO ;

/**
 * @brief Specifies the information for begin fetch listener
 */
typedef struct st_be_begin_fetch_info
{
	JC_CHAR					*pmURI ;	/**< Specifies the URL of the current fetch */
} ST_BE_BEGIN_FETCH_INFO ;

/**
 * @brief Specifies the type of the focused button control
 */
typedef struct st_be_button_type
{
	E_BE_BUTTON_TYPE	eButtonType ; /**< Specifies the type of the focused button control */
} ST_BE_BUTTON_TYPE ;

/**
 * @brief Specifies the information for end fetch listener
 */
typedef struct st_be_end_fetch_info
{
	JC_CHAR					*pmURI ;		/**< Specifies the URL of the fetched content */
	JC_INT8					*pcContentType ;	/**< Specifies the content type of the fetched content */
	JC_RETCODE				rCode ;			/**< Specifies the success/error code of the fetched content */
	E_HTTP_STATUS			eStatus ;		/**< Specifies the http status code  of the fetched content */
#ifdef BROWSER_WEB_SPIDER_ON
	JC_BOOLEAN				bIsCacheContent ; /**< Specifies whether the current fetch is from cache */
#endif
} ST_BE_END_FETCH_INFO ;


#ifdef DE_FILE_UPLOAD_ENABLE

/**
 * @brief Specifies the information about file browse display name
 *        The memory is allocated by application and freed by the browser
 *        using jdd_MemFree
 */
typedef struct st_be_file_browse_name
{
	JC_CHAR			*pmDisplayName ; /**< Specifies the name to be displayed on the browse button */
} ST_BE_FILE_BROWSE_NAME ;


/**
 * @brief Specifies the information about file browse button when pressed,
 *        Sends the accept type and received the file name based on it.
 */
typedef struct st_be_file_browse_info
{
	JC_CHAR			*pmAccept ;   /**< Specifies the accept type specifies for file. */
	JC_CHAR			*pmFileName ; /**< Specifies the name of the file, allocated by application and free by browser using jdd_MemFree. */
	void			*pvRef ;	  /**< Specifies the reference of the input element in which filename (pmFileName) has to be displayed. */
	JC_RETCODE		rCode ;       /**< Specifies the ret code while getting the file name. */
} ST_BE_FILE_BROWSE_INFO ;

#endif


/**
 * @brief The browser uses the following structure to send the DRM content to the application
 */
typedef struct st_be_drm_content		
{
	ST_BE_PAGE_CONTENT		stPageContent ; /**< Specifies the page content */
	/** Call back function an argument **/
	/** To be define more **/
} ST_BE_DRM_CONTENT ;


/**
 * @brief The following structure is used by the browser to specify the details of the
 *		  alert box that is to be displayed by the application.
 */
typedef struct st_be_wmls_alert
{
	JC_CHAR				*pmMessage ; /**< Specifies the alert message */
	void				*pvCbArg ; /**< Specifies the callback argument to be used in the response of the alert */
} ST_BE_WMLS_ALERT ;

/**
 * @brief The application needs to use this function to send the reponse of the alert function.
 */
typedef struct st_be_wmls_alert_rsp
{
	void				*pvCbArg ; /**< Specifies the alert callback response argument */
} ST_BE_WMLS_ALERT_RESP ;

/**
 * @brief The browser uses the following structure to specify the details of the confirm 
 *		  box that is to be displayed by the application.
 */
typedef struct st_be_wmls_confirm
{
	JC_CHAR				*pmMessage ; /**< Specifies the confirmation message */
	JC_CHAR				*pmOkConfirm ; /**< Specifies the message to be displayed as OK (positive response) */
	JC_CHAR				*pmCancelConfirm ; /**< Specifies the message to be displayed as Cancel (negative response) */
	JC_BOOLEAN			bIsOkStatus ; /**< To return the status of the confirm in case of blocking call */
	void				*pvCbArg ; /**< Specifies the callback argument to be used in the confirmation response (non-blocking call)  */
} ST_BE_WMLS_CONFIRM ;

/**
 * @brief In case the application is using non-blocking mechanism for a confirm box then it needs
 *		  to use the following structure to send the response of confirm to the browser.
 */
typedef struct st_be_wmls_confirm_rsp
{
	void				*pvCbArg ; /**< Specifies the confirm response argument */
	JC_BOOLEAN			bIsOkStatus ; /**< Specifies the status of the confirm box */
} ST_BE_WMLS_CONFIRM_RESP ;

/**
 * @brief The browser uses the following structure to specify the details of the prompt
 *		  box that is to be displayed by the application.
 */
typedef struct st_be_wmls_prompt
{
	JC_CHAR				*pmMessage ; /**< Specifies the prompt message */
	JC_CHAR				*pmDefaultValue ; /**< Specifies the default value that will be returned in case the user does not enter a value */
	JC_CHAR				*pmValue ; /**< Specifies the value entered by the user */ 
	JC_BOOLEAN			bIsOkStatus ; /**< Specifies the status of the prompt message (blocking call) */
	void				*pvCbArg ; /**< Specifies the callback argument to be used in the prompt response (non-blocking call) */
} ST_BE_WMLS_PROMPT ;

/**
 * @brief In case the application is using non-blocking mechanism for a prompt box then it needs
 *		  to use the following structure to send the response of prompt to the browser.
 */
typedef struct st_be_wmls_prompt_rsp
{
	void				*pvCbArg ; /**< Specifies the prompt response callback argument */
	JC_BOOLEAN			bIsOkStatus ; /**< Specifies the status of the prompt message */
	JC_CHAR				*pmValue ; /**< Specifies the value entered by the user */
} ST_BE_WMLS_PROMPT_RESP ;

/**
 * @brief The browser uses the following structure to specify the make call details to the 
 *		  application.
 */
typedef struct st_be_wtai_makecall
{
	JC_INT8				*pcPhoneNumber ; /**< Specifies the phone number to be used for make call */
	void				*pvCbArg ; /**< Specifies the callback argument to be used for make call response */
} ST_BE_WTAI_MAKECALL ;

/**
 * @brief In case the application is using non-blocking mechanism for a make call operation then 
 *		  it needs to use the following structure to send the response of make call to the browser.
 */
typedef struct st_be_wtai_makecall_resp
{
	JC_RETCODE			rCode ; /**< Specifies error if any while making a call */
	void				*pvCbArg ; /**< Specifies the make call response argument */
} ST_BE_WTAI_MAKECALL_RESP ;

/**
 * @brief The browser uses the following structure to specify the WTAI DTMF details to the 
 *		  application.
 */
typedef struct st_be_wtai_dtmf
{
	JC_INT8				*pcDTMF ; /**< Specifies the DTMF information */
	void				*pvCbArg ; /**< Specifies the send DTMF response argument */
} ST_BE_WTAI_SENDDTMF ;

/**
 * @brief The browser uses the following structure to specify the phone book operation to the
 *		  application.
 */
typedef struct st_be_wtai_pb
{
	JC_CHAR				*pmName ; /**< Specifies the name to be searched in the phone book */
	JC_INT8				*pcPhoneNumber ; /**< Specifies the phone number to be searched in the phone book */
	void				*pvCbArg ; /**< Specifies the argument to be used while sending the WTAI Phonebook response to the browser */
} ST_BE_WTAI_PHONEBOOK ; 

/**
 * @brief In case the application is using non-blocking mechanism for a phone book operation then 
 *		  it needs to use the following structure to send the response of phone book operation to the browser.
 */
typedef struct st_be_wtai_pb_resp
{
	JC_RETCODE			rCode ; /**< Specifies error if any while searching the phone book */
	void				*pvCbArg ; /**< Specifies the WTAI Phone book response argument */
} ST_BE_WTAI_PHONEBOOK_RESP ;


/**
 * @brief In case the application is using non-blocking mechanism for a send dtmf operation then 
 *		  it needs to use the following structure to send the response of send dtmf operation to the browser.
 */
typedef struct st_be_wtai_send_dtmf_resp
{
	JC_RETCODE			rCode ; /**< Specifies error if any while sending the DTMF */
	void				*pvCbArg ; /**< Specifies the WTAI Send DTMF response argument */
} ST_BE_WTAI_SEND_DTMF_RESP ;


typedef struct st_be_url_submit_wiz
{
	JC_CHAR				*pmURL ;
	JC_BOOLEAN			bIsInputData ;
	ST_BE_POST_DATA		stPostData ;
} ST_BE_URL_SUBMIT_WIZ ;

typedef struct st_be_script_exec_confirmation_resp
{
	JC_BOOLEAN			bIsAllowed ;
} ST_BE_SCRIPT_EXEC_CONFIRMATION_RESP ;

typedef struct st_be_script_exec_confirmation
{
	JC_CHAR				*pmPageURL ;
	JC_CHAR				*pmScriptURL ;
} ST_BE_SCRIPT_EXEC_CONFIRMATION ;

/******* DE_SCRIPT_ENABLE *******/
#ifdef DE_SCRIPT_ENABLE

/**
 * @brief The application needs to specify the script options using the following structure 
 */
typedef struct st_be_option_script
{
	JC_BOOLEAN		bIsEnable ; /**< The application specifies whether the script is enabled or not.*/
	JC_BOOLEAN		bDisableSetTimeOut ; /**< Specifies whether SetTimeOut JavaScript feature has to be disabled. */
} ST_BE_OPTION_SCRIPT ;

/**
 * @brief The application uses this structure to return the execution confirm
 *		  response for the ecmascript to the browser.
 */

typedef struct st_be_bom_exec_confirmation_resp
{
	JC_BOOLEAN			bIsAllowed ; /**< To return the status of the execution confirm message*/
} ST_BE_BOM_EXEC_CONFIRM_RESP ;

/**
 * @brief This browser uses this structure to pass the execution confirm request
 *		  informations for ecmascript to the application.  
 */

typedef struct st_be_bom_exec_confirmation
{
	JC_BOOLEAN			bIsAllowed ; /**< To return the status of the execution confirm in case of blocking call */ 
} ST_BE_BOM_EXEC_CONFIRM ;

/**
 * @brief The application uses this structure to return the abort confirm
 *		  response for the ecmascript to the browser.
 */

typedef struct st_be_bom_abort_confirmation_resp
{
	JC_BOOLEAN			bIsAbortOK ; /**< To return the status of the abort confirm message*/
} ST_BE_BOM_ABORT_CONFIRM_RESP ;

/**
 * @brief This browser uses this structure to pass the abort confirm request
 *		  informations for ecmascript to the application.  
 */

typedef struct st_be_bom_abort_confirmation
{
	JC_BOOLEAN			bIsAbortOK ; /**< To return the status of the abort confirm in case of blocking call */ 
} ST_BE_BOM_ABORT_CONFIRM ;

#endif

/**
 * @brief This structure is used to send the low memory information like low memory
 *        occurs in run-time memory or in file system
 */
typedef struct st_be_low_memory_info
{
	JC_BOOLEAN			bIsFileMem ;
	JC_BOOLEAN			bIsRuntimeMem ;
} ST_BE_LOW_MEMORY_INFO ;

/**
 * @brief This structure is used to get the apllication information.
 *
 */
typedef struct st_be_application_info
{
	JC_INT8		*pcAppName ; /**< Specifies the application name */
	JC_INT8		*pcAppVersion ; /**< Specifies the application version. It is recommended to
									 send platform and language information additionally
									 [For eg. "5.3.4.3 (Windows ; en)"]*/
	JC_INT8		*pcPlatform ; /**< Specifies the browser platform */
	JC_INT8		*pcLanguage ; /**< Specifies the browser language in two character format */
	JC_INT8		*pcUserAgent ; /**< Specifies the browser useragent */
} ST_BE_APPLICATION_INFO ;

#ifdef BROWSER_EXTENDED_SUPPORT

/**
 * @brief This structure is used as input and output parameter for anchor destination 
 *		  content type and charset parameter support callbacks and events
 */
typedef struct st_be_anchor_param_support
{
	void			*pvRef ; /**< Specifies the reference of the current anchor element, input parameter which should be returned */
	JC_BOOLEAN		bProceed ; /**< Specifies whether to fetch the given anchor from the server or not, output parameter */
} ST_BE_ANCHOR_PARAM_SUPPORT ;

#endif


#ifdef BROWSER_WINDOW_OPEN

/**
 * @brief This structure is used to get the new window's ID.
 *
 */
typedef struct st_be_window_info
{
	JC_UINT32		uiWindowID ; /**< Specifies the ID of the new window that is created **/
	JC_UINT32		uiWindowCnt ; /**< Specifies the current window count **/
	JC_BOOLEAN		bAdvertisement ; /**< Specifies whether the current request being made is for an advertisement or not **/
} ST_BE_WINDOW_INFO ;

/**
 * @brief This structure is used to return the window open call response.
 *
 */
typedef struct st_be_window_open_resp
{
	JC_BOOLEAN			bAllowWindow ; /**< Specifies that the window is allowed to be opened **/
} ST_BE_WINDOW_OPEN_RESP ;

#endif /** BROWSER_WINDOW_OPEN **/

/**
 * @brief The error code specifies that wtai add phone book (called for WMLS execution)
 *		  failed because the name parameter is unacceptable or too long.
 */
#define JC_ERR_BE_WTAI_INVALID_PB_NAME			TERMINAL_ERR_CODE(BE_MODULE, 0x21)
/**
 * @brief The error code specifies that wtai add phone book (called for WMLS execution)
 *		  failed because the number parameter is too long.
 */
#define JC_ERR_BE_WTAI_INVALID_PB_NUMBER		TERMINAL_ERR_CODE(BE_MODULE, 0x22)
/**
 * @brief The error code specifies that wtai add phone book (called for WMLS execution)
 *		  failed because the phone book name is invalid.
 */
#define JC_ERR_BE_WTAI_INVALID_PB_ENTRY			TERMINAL_ERR_CODE(BE_MODULE, 0x23)
/**
 * @brief The error code specifies that wtai add phone book (called for WMLS execution) 
 *		  failed because the phone book entry could not be written.
 */
#define JC_ERR_BE_WTAI_INVALID_PB_FULL			TERMINAL_ERR_CODE(BE_MODULE, 0x24)
/**
 * @brief The error code specifies that wtai add phone book (called for WMLS execution)
 *		  failed because of some unspecified error.
 */
#define JC_ERR_BE_WTAI_UNSPECIFIED				TERMINAL_ERR_CODE(BE_MODULE, 0x25)
/**
 * @brief The error code specifies that wmls make call failed because the network is not available.
 */
#define JC_ERR_BE_WMLS_NETWORK_NOT_AVAILABLE	TERMINAL_ERR_CODE(BE_MODULE, 0x26)
/**
 * @brief The error code specifies that wmls make call failed because the party (the person being called)
 * is not answering.
 */
#define JC_ERR_BE_WMLS_CALLED_PARTY_NOT_ANSWER	TERMINAL_ERR_CODE(BE_MODULE, 0x27)
/**
 * @brief The error code specifies that wmls make call failed because the party (the person being called)
 * is busy.
 */
#define JC_ERR_BE_WMLS_CALLED_PARTY_BUSY		TERMINAL_ERR_CODE(BE_MODULE, 0x28)

/****** End of DE_SCRIPT_ENABLE *****/

#ifdef BROWSER_WINDOW_OPEN
/**
 * @brief The error code specifies that the application has already reached the maximum window
 * count and that it cannot create any more new windows.
 */
#define JC_ERR_BE_MAX_WINDOW_COUNT			TERMINAL_ERR_CODE(BE_MODULE, 0x30)
/**
 * @brief The error code specifies that the passed window has been processed.
 */
#define JC_ERR_BE_WINDOW_PROCESSED			TERMINAL_ERR_CODE(BE_MODULE, 0x31)
#endif

#ifdef __cplusplus
}
#endif

#endif

/* End of File */


