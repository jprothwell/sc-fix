/***************************************************************************
 * $Id: jdi_bom.h,v 1.73 2009/05/09 13:25:05 shajudevhtmlbrow Exp $
 * $Revision: 1.73 $
 * $Date: 2009/05/09 13:25:05 $
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
 * $Log: jdi_bom.h,v $
 * Revision 1.73  2009/05/09 13:25:05  shajudevhtmlbrow
 * encoding support in BOM.
 *
 * Revision 1.72  2009/04/21 12:15:41  vikramdevhtmlbrow
 * window.frameElement support added.
 *
 * Revision 1.71  2009/03/10 09:01:03  vikramdevhtmlbrow
 * Added support for window.pageXOffset/scrollX and
 * window.pageYOffset/scrollY.
 *
 * Revision 1.70  2009/01/21 14:32:13  kavithadevhtmlbrow
 * Popup Blocker for Window.Open
 *
 * Revision 1.69  2009/01/12 12:49:40  shajudevhtmlbrow
 * ScriptEngine_V_1_1_4_14_12_01_2009 tag and related changes
 *
 * Revision 1.68  2008/12/19 10:58:25  shajudevhtmlbrow
 * document.all support based on Strict XHTML flag
 *
 * Revision 1.67  2008/12/17 12:00:24  kavithadevhtmlbrow
 * window.open - updated with window count and window.closed and other issues
 *
 * Revision 1.66  2008/12/17 04:48:31  shajudevhtmlbrow
 * ScriptEngine_V_1_1_4_11_16_12_2008
 *
 * Revision 1.65  2008/12/16 07:23:53  kavithadevhtmlbrow
 * DE_WINDOW_OPEN_ENABLE to BROWSER_WINDOW_OPEN
 *
 * Revision 1.64  2008/12/10 15:37:59  kavithadevhtmlbrow
 * window.open - updated
 *
 * Revision 1.63  2008/12/10 14:46:16  kavithadevhtmlbrow
 * window.open Support - DE_WINDOW_OPEN_ENABLE
 *
 * Revision 1.62  2008/12/09 10:49:24  shajudevhtmlbrow
 * ScriptEngine_V_1_1_4_10_09_12_2008
 *
 * Revision 1.61  2008/09/17 15:10:26  shajudevhtmlbrow
 * Changes for mouse event, key event , onselect and onchange
 *
 *  
 ***************************************************************************/

#ifndef _JDI_BOM_H_
#define _JDI_BOM_H_

#include <jdi_scriptinfo.h>
#include <jdi_xmldatatype.h>

typedef JC_HANDLE       BOM_CONTEXT ;
typedef JC_HANDLE		BOM_SESSION ;

/**
 * @brief Specify history operation associated with fetching a URL.
 */

typedef enum e_bom_fetch_url_history_op
{
    E_BOM_HISTORY_NOOP, /**< Specifies no changes to history */
    E_BOM_HISTORY_ADDITION, /**< Specifies addition of given URL to history */
    E_BOM_HISTORY_REPLACE /**< Specifies replacement of current history entry with given URL */ 
} E_BOM_FETCH_URL_HISTORY_OP ;

/**
 * @brief Specify a URL to be fetched.
 */

typedef struct bomFetchURL
{
    JC_INT8                         *pcURL ;/**< Specifies the URL to be fetched. */
    E_BOM_FETCH_URL_HISTORY_OP      eHistoryOp ; /**< Specifies history operation associated with fetching the URL */
} ST_BOM_FETCH_URL ;

/**
 * @brief Specify a setTimeout call
 */
typedef struct st_bom_timer_info
{
    JC_HANDLE       vTimeoutHandle ;
    JC_UINT32       uiMilliSecs ;
    JC_UINT32       uiTimerId ;
    JC_UINT32       uiArgCnt ;
} ST_BOM_TIMER_INFO ;

/**
 * @brief Check whether given window is an existing window
 */
typedef struct st_bom_window_exists
{
    JC_INT8             *pcWindowName ;
    JC_BOOLEAN          bExists ; /* E_TRUE if given window exists */
} ST_BOM_WINDOW_EXISTS ;

/** @cond */

/**
 * @brief Specifies window informations (innerHeight, innerWidth, window handle)
 */
typedef struct st_bom_window_info
{
    JC_INT32			iInnerValue ; /* window innerHeight/innerWidth */
#ifdef BROWSER_WINDOW_OPEN
	JC_HANDLE			vBOMSession ; /**< Specifies the BOM session handle required for notifying the application **/ 
	JC_BOOLEAN			bWinExists ; /**< Specifies whether the passed BOM session exists or not **/
#endif
} ST_BOM_WINDOW_INFO ;

#ifdef BROWSER_WINDOW_OPEN
/**
 * @brief Specify the bomsession that is to be returned to script engine 
 */
typedef struct st_bom_window_open_rsp
{
	JC_HANDLE			vBOMSession ; /**< Specifies the bom session that is to be returned to script engine. */
} ST_BOM_WINDOW_OPEN_RESP ;

#endif
/**
 * @brief Specify a history index
 */
typedef struct st_bom_history_index
{
    JC_INT32        iIndex ;
} ST_BOM_HISTORY_INDEX ;

/**
 * @brief Specify selected option index for a SELECT element
 */
typedef struct st_bom_select_option
{
    ST_XML_TAG      *pstTag ;  /* Specifies SELECT element */
    JC_INT32        iIndex ;   /* Specifies selected option index. If no option is selected,return -1 on retrieval. On setting, give -1 when no option is to be selected. On setting, select only one option whether mutiple is set or not. */
} ST_BOM_SELECT_OPTION ;

/**
 * @brief Specify checked status for input control
 */
typedef struct st_bom_input_status
{
    ST_XML_TAG      *pstTag ;	/* Specifies INPUT element */
    JC_BOOLEAN      bChecked ;  /* E_TRUE if INPUT element is checked */
} ST_BOM_INPUT_STATUS ;

/**
 * @brief Specify selected status for OPTION element
 */
typedef struct st_bom_option_status
{
    ST_XML_TAG      *pstTag ;     /* Specifies OPTION element */
    JC_BOOLEAN      bSelected ;	  /* E_TRUE if OPTION element is selected */
} ST_BOM_OPTION_STATUS ;

/**
 * @brief Specify TextArea's value
 *
 * BOM will not free the memory allocated for pmValue when TextArea value is got.
 * BOM will free the memory allocated for pmValue when TextArea value is set.
 */
typedef struct st_bom_textarea_value
{
    ST_XML_TAG		*pstTag ;  /* Specifies TEXTAREA element */ 
    JC_CHAR         *pmValue ; /* Specifies TEXTAREA value get/to set. JC_NULL is valid in get/set */
} ST_BOM_TEXTAREA_VALUE ;

/**
 * @brief Specify TextArea's number of rows and columns
 */

typedef struct st_bom_textarea_rowcol
{
    ST_XML_TAG		*pstTag ;  /* Specifies TEXTAREA element */ 
    JC_UINT32       uiValue ; /* Specifies TEXTAREA rows/cols value*/
} ST_BOM_TEXTAREA_ROWCOL ;

/**
 * @brief Specify INPUT's value
 *
 * BOM will not free the memory allocated for pmValue when INPUT value is got.
 * BOM will free the memory allocated for pmValue when INPUT value is set.
 */
typedef struct st_bom_input_value
{
    ST_XML_TAG      *pstTag ;     /* Specifies INPUT element */ 
    JC_CHAR         *pmValue ;    /* Specifies INPUT value get/to set. JC_NULL is valid in get/set */
} ST_BOM_INPUT_VALUE ;


/**
 * @brief Specify image dimensions
 */

typedef struct st_bom_image_dim
{
    ST_XML_TAG		*pstTag ;  /* Specifies Image element */ 
    JC_UINT32       uiValue ; /* Specifies Image width/height value*/
} ST_BOM_IMAGE_DIM ;

/**
 * @brief Specify image has loaded or not
 */

typedef struct st_bom_image_complete
{
	ST_XML_TAG		*pstTag ; /* Specifies Image element */
	JC_BOOLEAN		bIsLoaded ; /* E_TRUE if given image has loaded completely or else E_FALSE*/
} ST_BOM_IMAGE_COMPLETE ;

/**
 * @brief Specify an element content
 *
 */
typedef struct st_bom_element_content
{
    ST_XML_TAG			*pstTag ;	  /* Specifies element whose content to set/get */ 
    JC_CHAR             *pmContent ;  /* Specifies content */ 
} ST_BOM_ELEMENT_CONTENT ;

/**
 * @brief Handles the data transfer for E_BOM_INSERT_ADJACENT_HTML callback
 *
 */
typedef struct st_bom_adjacent_html
{
    ST_XML_TAG			*pstTag ;		/* Specifies element which will be adjacent */ 
	ST_XML_TAG			*pstParent ;	/* Specifies parent of the element */ 
    JC_CHAR             *pmContent ;	/* Specifies content */ 
} ST_BOM_ADJACENT_HTML ;

/**
 * @brief This structure specifies event class.
 *
 */
typedef enum e_bom_event_class
{
	E_BOM_OTHER_EVENT, /**< Specifies for normal events*/
	E_BOM_KEY_EVENT, /**< Specifies for key events*/
	E_BOM_MOUSE_EVENT /**< Specifies for mouse events*/
} E_BOM_EVENT_CLASS ;


/**
 * @brief This structure specifies the key event info.
 *
 */
typedef struct st_bom_key_event
{
    JC_INT32            iKeyCode ;   /**< Specifies key code */ 
	JC_INT8				cCharCode ;   /**< Specifies character code */ 
} ST_BOM_KEY_EVENT ;

/**
 * @brief This structure specifies the mouse event info.
 *
 */
typedef struct st_bom_mouse_event
{
	JC_BOOLEAN			bAltPressed ; /**< This flag specifies whether ALT key is pressed or not during the mouse action.*/
	JC_BOOLEAN			bCtrlPressed ;/**< This flag specifies whether CONTROL key is pressed or not during the mouse action.*/
	JC_BOOLEAN			bShiftPressed ;/**< This flag specifies whether SHIFT key is pressed or not during the mouse action.*/
	JC_BOOLEAN			bMetaPressed ;/**< This flag specifies whether Meta key is pressed or not during the mouse action.*/
	JC_INT8				cButton ; /**< Specifies which mouse button is pressed. 0 for left, 1 for middle and 2 for right.*/
	ST_XML_TAG			*pstRelTarget ; /**< Reference to a node related to the event—for example, on a mouseover it references 
											the node the mouse is leaving; on mouseout it references the node to which the 
											mouse is moving */
	JC_INT32			iClientX ; /**< Horizontal coordinate of the event relative to the browser's content pane*/
	JC_INT32			iClientY ; /**< Vertical coordinate of the event relative to the browser's content pane*/
	JC_INT32			iScreenX ; /**< Horizontal coordinate of the event relative to the whole screen*/
	JC_INT32			iScreenY ; /**< Vertical coordinate of the event relative to the whole screen*/
	JC_INT32			iOffsetX ; /**< Horizontal coordinate of the event relative to the target*/
	JC_INT32			iOffsetY ; /**< Vertical coordinate of the event relative to the target*/
	JC_INT32			iX ; /**< Horizontal coordinate of the event relative to the target's parent*/
	JC_INT32			iY ; /**< Vertical coordinate of the event relative to the target's parent*/
} ST_BOM_MOUSE_EVENT ;

/**
 * @brief This structure specifies the event info.
 *
 */

typedef union u_bom_event_info
{
	ST_BOM_KEY_EVENT	stKeyEvent ;
	ST_BOM_MOUSE_EVENT	stMouseEvent ;
} U_BOM_EVENT_INFO ;


/**
 * @brief Specify an event
 *
 */
typedef struct st_bom_event_info
{
	E_BOM_EVENT_CLASS	eEventClass ; /**< Specifies which event class*/	
	U_BOM_EVENT_INFO	uEvent ; /**< Specifies specific event info*/	
    ST_XML_TAG          *pstTarget ; /**< Specifies target element */ 
    JC_INT8             *pcType ;    /**< Specifies event type.Use event type's defined in jdi_html.h */ 
} ST_BOM_EVENT_INFO ;

/**
 * @brief Specify URI information
 *
 */
typedef struct st_bom_absolute_uri
{
    JC_CHAR            *pmURI ;			/* Specifies relative uri */ 
    JC_CHAR			   *pmAbsoluteURI ; /* Specifies absolute uri.BOM will free allocated memory. */
} ST_BOM_ABSOLUTE_URI ;

/**
 * @brief Specifies document.write related information
 *
 */
typedef struct st_bom_document_write
{
    JC_CHAR            *pmWriteStr ;	/* Specifies the string to be written */ 
    JC_CHAR			   *pmScriptBuff ;  /* Specifies return script buffer (if any). */
} ST_BOM_DOCUMENT_WRITE ;

#ifdef BROWSER_AJAX_ENABLE

/**
 * @brief Specifies the Ajax request method types
 *
 */
typedef enum e_bom_ajax_method_type
{
    E_AJAX_METHOD_GET,		/* Specifies the GET method type for Ajax request. */
	E_AJAX_METHOD_PUT,		/* Specifies the PUT method type for Ajax request. */
	E_AJAX_METHOD_POST,		/* Specifies the POST method type for Ajax request. */
	E_AJAX_METHOD_HEAD,		/* Specifies the HEAD method type for Ajax request. */
	E_AJAX_METHOD_TRACE,	/* Specifies the TRACE method type for Ajax request. */
	E_AJAX_METHOD_DELETE,	/* Specifies the DELETE method type for Ajax request. */
	E_AJAX_METHOD_OPTIONS,	/* Specifies the OPTIONS method type for Ajax request. */
	E_AJAX_METHOD_CONNECT,	/* Specifies the CONNECT method type for Ajax request. */
	E_AJAX_METHOD_INVALID	/* Specifies the invalid method type for Ajax request. */
} E_BOM_AJAX_METHOD_TYPE ;

/**
 * @brief Specifies the Ajax request information
 *
 */
typedef struct st_bom_ajax_req_open
{
    JC_HANDLE				vAjaxReqHandle ;	/* Specifies Unique Ajax Request Handle. */ 
    JC_CHAR					*pmURL ;			/* Specifies the request URL. */
	JC_CHAR					*pmUserName ;		/* Specifies the Username for the authentication. */
	JC_CHAR					*pmPassword ;		/* Specifies the Password for the authentication. */
	JC_BOOLEAN				bAsync ;			/* Specifies whether the script execution is asynchronous or not */
	E_BOM_AJAX_METHOD_TYPE	eAjaxMethodType ;	/* Specifies the Method type of the Ajax request. */
} ST_BOM_AJAX_REQ_OPEN ;

/**
 * @brief Specifies the request or response header information. 
 *		  If the callback type is E_BOM_AJAX_SET_HEADER then both pcHeaderName and pcHeaderValue are input values.
 *		  If the callback type is E_BOM_AJAX_GET_HEADER then pcHeaderName is input and pcHeaderValue is output.
 *
 */
typedef struct st_bom_ajax_header
{
    JC_HANDLE			vAjaxReqHandle ;	/* Specifies Unique Ajax Request Handle. */ 
    JC_INT8				*pcHeaderName ;		/* Specifies the header name. */
	JC_INT8				*pcHeaderValue ;	/* Specifies the header value. */
} ST_BOM_AJAX_HEADER ;

/**
 * @brief Specifies the Ajax request information
 *
 */
typedef struct st_bom_ajax_req_send
{
    JC_HANDLE			vAjaxReqHandle ;	/* Specifies Unique Ajax Request Handle. */ 
	JC_UINT32			uiPostLen ;			/* Specifies the Post data length. */
    JC_INT8				*pcPostData ;		/* Specifies the post data for the Ajax request. */
} ST_BOM_AJAX_REQ_SEND ;

/**
 * @brief Specifies the HTTP STATUS of the Ajax response
 *
 */
typedef struct st_bom_ajax_http_status
{
    JC_HANDLE			vAjaxReqHandle ;	/* Specifies Unique Ajax Request Handle */ 
    JC_UINT32			uiHttpStatus ;		/* Specifies the HTTP STATUS of the Ajax Response. This will be filled if the callback type is #E_BOM_AJAX_GET_HTTP_STATUS. */
	JC_INT8				*pcStatusText ;		/* Specifies the HTTP STATUS TEXT of the Ajax Response. This will be filled if the callback type is #E_BOM_AJAX_GET_HTTP_STATUS_TEXT. */
} ST_BOM_AJAX_HTTP_STATUS ;

/**
 * @brief Specifies the Response of the Ajax request in the form of text
 *
 */
typedef struct st_bom_ajax_resp_text
{
    JC_HANDLE			vAjaxReqHandle ;	/* Specifies Unique Ajax Request Handle. */ 
    JC_UINT32			uiRespLen ;			/* Specifies the length of the Ajax response. */
	JC_CHAR				*pmResponse ;		/* Specifies the Ajax response as a Text. */
} ST_BOM_AJAX_RESP_TEXT ;

/**
 * @brief Specifies the Response of the Ajax request in the form of XML Tree
 *
 */
typedef struct st_bom_ajax_resp_xml
{
    JC_HANDLE			vAjaxReqHandle ;		/* Specifies Unique Ajax Request Handle. */ 
	JC_HANDLE			vResponseXMLObj ;		/* Specifies Document Obj constructed for responseXML property. */ 
	ST_XML_TREE			*pstXMLTree ;			/* Specifies the Root element of the tree formed from the Ajax response. */
} ST_BOM_AJAX_RESP_XML ;

/**
 * @brief Specifies the Response of the Ajax request in the form of XML Tree
 *
 */
typedef struct st_bom_ajax_all_header
{
    JC_HANDLE			vAjaxReqHandle ;	/* Specifies Unique Ajax Request Handle. */ 
	JC_UINT32			uiHeaderLen ;		/* Specifies the length of the response header. */ 
	JC_UINT8			*pucHeader ;		/* Specifies the whole response header as a string. */
} ST_BOM_AJAX_ALL_HEADER ;

#endif /* BROWSER_AJAX_ENABLE */

/**
 * @brief Setting style property
 */
typedef struct st_bom_style_info
{
    ST_XML_TAG          *pstElement ;   /* Specifies the Element for which style property is to be get/set */
    JC_INT8             *pcProperty ;	/* Specifies the Style property attribute */
	JC_CHAR             *pmValue ;		/* Specifies the value of the Style property */
} ST_BOM_STYLE_INFO ;

/**
 * @brief Specifies attribute information.
 *
 * This structure is used with E_BOM_SET_ATTRIBUTE callback. It can be used in
 * four different cases.
 *
 * 1) To set inline content for an element. In this case pstElement and pmValue should
 *	  be present. pstattribute and pcAttribute should not be there. The pmValue should 
 *	  be set as an inline content to the element.
 * 2) To set/modify the attribute and attribute value for an element. In this case
 *	  pstElement, pmValue and pcAttribute should be present. pstattribute should
 *	  not be there. The new attribute node will be returned in pstAttribute.
 * 3) To set a value for an attribute, when it doesn't know the element node. In this case
 *	  pstAttribute and pmValue should present. pcAttribute and pstElement should be NULL
 * 4) To add/replace an attribute node for an element. In this case pstAttribute and 
 *	  pstElement should be present. pcAttribute and pmValue should be NULL. If it is a
 *	  replace then replaced attribute node should be returned in pstAttribute.
 */

typedef struct st_bom_attr_info
{
	ST_XML_TAG          *pstElement ;	/* Specifies the XML Element */
	JC_INT8				*pcAttribute ;	/* Specifies attribute name */
	JC_CHAR             *pmValue ;		/* Specifies attribute value */
	JC_BOOLEAN			bCaseSensitive ; /* Specifies case sensitive search for 
											attribute name, if the value is E_TRUE*/
	JC_BOOLEAN			bRedraw ;		 /*< Specifies whether this attribute needs a redraw */
	ST_XML_ATTRIBUTE	*pstAttribute ; /* Specifies the attribute node */
} ST_BOM_ATTR_INFO ;

/**
 * @brief Specifies script buffer.
 */
typedef union u_bom_script_src
{
	JC_CHAR 		*pmFileName ; /**< The source of script is filename. Currently not used. */
	JC_CHAR			*pmBuffer ; /**< The source of script is unicode. */
	JC_UINT8		*pucBuffer ; /**< The source of script is buffer, in ascii format; where the encoding is specified in pucCharset. */
} U_BOM_SCRIPT_SRC ;

/**
 * @brief Specifies script source information.
 */
typedef enum e_bom_source_info
{
	E_BOM_SOURCE_FILE,
	E_BOM_SOURCE_UNICODE,
	E_BOM_SOURCE_ASCII
} E_BOM_SCRIPT_INFO ;

/**
 * @brief Specifies script evalutaion parameters.
 */
typedef struct st_bom_eval_params
{
	E_BOM_SCRIPT_INFO	eSourceInfo ; /**< Specifies the type of input script given, it will be E_SOURCE_ASCII
									  if the buffer is taken from .js file and E_SOURCE_UNICODE in case of inline scripts.
									  and E_SOURCE_FILE in case when the buffer is direct file input*/

	U_BOM_SCRIPT_SRC	uScriptSrc ;  /**< if the buffer is taken .js file, it should be in pucBuffer and if it is inline script
									  the buffer should given in pmBuffer without coping. pmFileName contains the file name from 
									  where the buffer was directly taken*/

	JC_UINT32			uiScriptSrcLen ; /**< The length of the Script Input Source Buffer */
	JC_UINT8			*pucCharset ; /* Charset of script content.Any BOM 
 
 								    in script is handled by Script Engine.
 								    Defaults to UTF-8 if JC_NULL is passed. */
	JC_INT8			*pcContentType ; /* Reference to the content type */
} ST_BOM_EVAL_PARAMS ;

/**
 * @brief Specifies the offset values for an element.
 */
typedef struct st_bom_offset_value
{
	ST_XML_TAG		*pstElement ; /* Element whose offset values has to be calculated*/
	ST_XML_TAG		*pstOffsetParent ; /* Element's Container element*/
	JC_INT32		iOffsetValue ; /* Offset values with respect to offsetParent*/
} ST_BOM_OFFSET_VALUE ;

/**
 * @brief Specifies the scroll values for an element.
 */
typedef struct st_bom_scroll_value
{
	ST_XML_TAG		*pstElement ; /**< Element whose offset values has to be calculated*/
	JC_INT32		iScrollValue ; /**< Scroll values */
} ST_BOM_SCROLL_VALUE ;

/**
 * @brief Specifies the types for document modification.
 */
typedef enum e_bom_modify_type
{
	E_BOM_DOCUMENT_ADD_NODE, /**< Indicates the element should be added to the document*/
	E_BOM_DOCUMENT_REMOVE_NODE /**< Indicates the element should be removed from the document*/
} E_BOM_DOCUMENT_MODIFY_TYPE ;

/**
 * @brief Specifies the types for inserting element/text/html.
 */
typedef enum e_bom_insert_type
{
	E_BOM_INSERT_INVALID,
	E_BOM_BEFORE_BEGIN, /**< Indicates that the content should be inserted immediately before the object*/
	E_BOM_AFTER_BEGIN, /**< Indicates that the content should be inserted after the start of the object but before all other content*/
	E_BOM_BEFORE_END, /**< Indicates that the content should be inserted immediately before the end of the object, after all other content.*/
	E_BOM_AFTER_END /**< Indicates that the content should be inserted immediately after the end of the object*/
} E_BOM_INSERT_TYPE ;

/**
 * @brief Define the structure for E_BOM_MODIFY_DOCUMENT callback.
 */
typedef struct st_bom_update_document
{
	E_BOM_DOCUMENT_MODIFY_TYPE	eModifyType ; /**< Types of modification operations.*/
	ST_XML_TAG					*pstParent ; /**< Specifies the parent of XML Element for which the document
												   modification is needed*/
	ST_XML_TAG					*pstElement ; /**< Specifies the XML Element for which the document
												   modification is needed*/
} ST_BOM_MODIFY_DOCUMENT ;

/**
 * @brief Define the structure for E_BOM_GET_COOKIE, E_BOM_GET_REFERRER, 
 *		  E_BOM_GET_URI and E_BOM_GET_DOMAIN callback. 
 *		  Value to be filled according to the context.
 */

typedef struct st_bom_document_info
{
	JC_INT8		*pcValue ;
} ST_BOM_DOCUMENT_INFO ;

/**
 * @brief Define the structure for E_BOM_GET_COOKIE and E_BOM_SET_COOKIE. 
 *		  Value to be filled according to the context.
 */

typedef struct st_bom_cookie_info
{
	JC_CHAR		*pmCookie ;
} ST_BOM_COOKIE_INFO ;

/**
 * @brief Define the structure for E_BOM_ELEMENT_FROMPOINT callback. 
 */

typedef struct st_bom_element_frompoint
{
	JC_INT32		iX ; /**< Specifies the x position at which element is to searched>*/
	JC_INT32		iY ; /**< Specifies the y position at which element is to searched>*/
	ST_XML_TAG		*pstElement ; /**< Specifies the XML Element found at the specified (x, y) position>*/
} ST_BOM_ELEMENT_FROMPOINT ;

/**
 * @brief Define the structure for E_BOM_GET_SESSION_HANDLE callback. 
 */

typedef struct st_bom_session_info
{
	ST_XML_TAG		*pstElement ; /**< Specifies the XML Element for which the session handle is required>*/
	JC_HANDLE		vBOMSession ; /**< Specifies the session handle of frame or iframe element if any else JC_NULL >*/
} ST_BOM_SESSION_INFO ;

/**
* @brief structure foe getting current element from application
*/

typedef struct st_bom_current_element
{
	ST_XML_TAG		*pstElement ;
} ST_BOM_CURRENT_ELEMENT ;

/**
* @brief structure for handling E_BOM_EMBED_GET callback
*/

typedef struct st_bom_embed_get
{
	JC_HANDLE		vEmbedObj ;
	JC_HANDLE		vPropertyID ;
	JC_HANDLE		vResult ;
} ST_BOM_EMBED_GET ;

/**
* @brief structure for handling E_BOM_IS_STRICT_XHTML_ENABLE callback
*/

typedef struct st_bom_strict_xhtml
{
	JC_BOOLEAN		bIsXHTMLEnable ;
} ST_BOM_STRICT_XHTML ;

/**
* @brief structure for handling E_BOM_GET_ENCODED_BUFFER callback
*/

typedef struct st_bom_encoded_buffer
{
	JC_CHAR			*pmBuffer ;
	JC_INT8			*pcEncBuffer ;
	JC_UINT32		uiBufLen ;
} ST_BOM_ENCODED_BUFFER ;

/** 
 * @brief Specifies different types of application callback registered with BOM.
 */
typedef enum e_bom_callback_type
{
    /* Settings for XHTML */
	E_BOM_IS_STRICT_XHTML_ENABLE, /**< Intimates application to get is strich XHTML is enbaled or not. (Argument passed ST_BOM_STRICT_XHTML) */
	/* Settings for XHTML */
	E_BOM_GET_ENCODED_BUFFER, /**< Intimates application to get encoded buffer if mrntioned in the page or else utf8. (Argument passed ST_BOM_ENCODED_BUFFER) */
    /* history object */
    E_BOM_GET_HISTORY_COUNT, /**< Intimates application to get history count. (Argument passed JC_UINT32) */
	E_BOM_NAVIGATE_HISTORY_BACK, /**< Intimates application to navigate back in history. */
	E_BOM_NAVIGATE_HISTORY_FORWARD, /**< Intimates application to navigate forward in history. */
    /* Note: Form variable data is preserved */
	E_BOM_NAVIGATE_HISTORY_ATINDEX, /**< Intimates application to navigate to a URL at a given index in history. (Argument passed #ST_BOM_HISTORY_INDEX*) */
    
    /* navigator object */
	E_BOM_GET_APPLICATION_INFO, /**< Intimates application to get the application information. (Argument passed #ST_BOM_APPLICATION_INFO*) */
    /* location object */
    /* Note: Assumes set location properties does not update history */
    E_BOM_FETCH_URL, /**< Intimates application to fetch given URL. (Argument passed #ST_BOM_FETCH_URL*) */
    E_BOM_FETCH_ANCHOR, /**< Intimates application to fetch given anchor. (Argument passed #JC_INT8*) */
    /* Note: Form variable data is initialized */
    E_BOM_RELOAD, /**< Intimates application to reload current URL from server or cache. (Argument passed #JC_BOOLEAN) */
    
    /* window object */
    E_BOM_SHOW_ALERT, /**< Intimates application to display alert message. (Argument passed #ST_BOM_ALERT*) */
                      /**< Returns JC_ERR_USER_WAIT_ACTION for non-blocking implementation. */
    E_BOM_SHOW_CONFIRM, /**< Intimates application to display confirm message. (Argument passed #ST_BOM_CONFIRM*) */
                        /**< Returns JC_ERR_USER_WAIT_ACTION for non-blocking implementation. */
    E_BOM_SHOW_PROMPT, /**< Intimates application to display prompt message. (Argument passed #ST_BOM_PROMPT*) */
                       /**< Returns JC_ERR_USER_WAIT_ACTION for non-blocking implementation. */
	E_BOM_FREE_PROMPT_STRING, /**< Intimates the application to free the prompt string. This callback will be used only for blocking call.(Argument passed #JC_CHAR*) */   
#ifdef BROWSER_WINDOW_OPEN
	E_BOM_DOES_WINDOW_EXISTS, /**< Checks whether given window exists or not.(Argument passed #ST_BOM_WINDOW_EXISTS*) */
    E_BOM_GET_WINDOW_OPENER, /**< Initimates the DE to obtain the opening window's page context */
	E_BOM_GET_WINDOW_CLOSED, /**< Intimates the DE to check whether the window is closed or not **/
#endif
	E_BOM_WINDOW_OPEN, /**< Intimates application to open a new web browser window. (Argument passed #ST_BOM_WINDOW_OPEN*) */
	E_BOM_WINDOW_CLOSE, /**< Intimates the DE that the window is going to be closed from script */
	E_BOM_SET_TIMEOUT, /**< Intimates application to execute code after a specified time interval. (Argument passed #ST_BOM_SET_TIMEOUT*) */
    E_BOM_CLEAR_TIMEOUT, /**< Intimates application to cancel a setTimeout. (Argument passed #JC_UINT32) */
    E_BOM_SET_INTERVAL, /**< Intimates application to execute code in specified time intervals. (Argument passed #ST_BOM_SET_TIMEOUT*) */
    E_BOM_CLEAR_INTERVAL, /**< Intimates application to cancel a setInterval. (Argument passed #JC_UINT32) */
    E_BOM_GET_INNER_WIDTH, /**< Intimates application to get the window innerWidth. (Argument passed #ST_BOM_WINDOW_INFO*) */
	E_BOM_GET_INNER_HEIGHT, /**< Intimates application to get the window innerHeight. (Argument passed #ST_BOM_WINDOW_INFO*) */
	E_BOM_GET_SCROLL_X, /**< Intimates application to get the window scroll X. (Argument passed #ST_BOM_SCROLL_VALUE*) */
	E_BOM_GET_SCROLL_Y, /**< Intimates application to get the window scroll Y. (Argument passed #ST_BOM_SCROLL_VALUE*) */

    /* document object */
    E_BOM_GET_COOKIE, /**< Intimates application to get cookie for current domain. (Argument passed #ST_BOM_DOCUMENT_INFO*).BOM will free memory allocated for cookie using memory handle given with jdi_BOMInitialize call. */
                      /**< Returns JC_ERR_COOKIE_DISABLED, JC_ERR_COOKIE_NOT_FOUND. */
    E_BOM_SET_COOKIE, /**< Intimates application to set cookie for current domain. (Argument passed #JC_INT8*) */
                      /**< Returns JC_ERR_COOKIE_DISABLED.*/
    E_BOM_IS_COOKIE_ENABLED, /**< Returns whether cookie is enabled or not. (Argument passed #JC_BOOLEAN*) */
    E_BOM_GET_REFERRER, /**< Intimates application to get referrer URI. (Argument passed #ST_BOM_DOCUMENT_INFO*).BOM will free memory allocated for referrer URI using memory handle given with jdi_BOMInitialize call. */
    E_BOM_GET_URI, /**< Intimates application to get URI of a document. (Argument passed #ST_BOM_DOCUMENT_INFO*).BOM will free memory allocated for URI using memory handle given with jdi_BOMInitialize call. */
    E_BOM_GET_DOMAIN, /**< Intimates application to get domain of a document. (Argument passed #ST_BOM_DOCUMENT_INFO*).BOM will free memory allocated for domain using memory handle given with jdi_BOMInitialize call. */
    E_BOM_SET_TITLE, /**< Intimates application to set Title of document.(Argument passed #JC_CHAR*). JC_NULL is valid for title */
    E_BOM_DOCUMENT_CLOSE, /**< Intimates application to close document stream & force rendering. */
    E_BOM_DOCUMENT_OPEN, /**< Intimates application to clear existing document tree. */
    E_BOM_DOCUMENT_WRITE, /**< Intimates application to write a string.If JC_NULL is passed, the window is cleared (Argument passed #ST_BOM_DOCUMENT_WRITE*).*/
	E_BOM_ELEMENT_FROMPOINT, /**< Intimates application to get element at particular (x, y) position. (Argument passed #ST_BOM_ELEMENT_FROMPOINT*).*/
    
    /* form element */
    E_BOM_SUBMIT_FORM, /**< Intimates application to submit a form.(Argument passed #ST_XML_TAG *) */
    E_BOM_RESET_FORM, /**< Intimates application to reset a form.(Argument passed #ST_XML_TAG *) */

    /* SELECT element */
    E_BOM_GET_SELECT_INDEX, /**< Intimates application to get selected index for a SELECT element.(Argument passed #ST_BOM_SELECT_OPTION*) */
    E_BOM_SET_SELECT_INDEX, /**< Intimates application to set selected index for a SELECT element.(Argument passed #ST_BOM_SELECT_OPTION*) */

    /* SCREEN object */
	E_BOM_GET_SCREEN_INFO, /**<Intimates application to get the available screen resources.(Argument passed #ST_BOM_SCREEN_INFO*)*/
	
	/* SELECT, INPUT, ANCHOR, TextArea elements */
    E_BOM_BLUR_INPUT, /**< Intimates application to remove focus from an input control.(Argument passed #ST_XML_TAG *) */
    E_BOM_FOCUS_INPUT, /**< Intimates application to give focus for an input control.(Argument passed #ST_XML_TAG *) */

    /* INPUT, TextArea elements */
    E_BOM_SELECT_INPUT, /**< Intimates application to select an input control.(Argument passed #ST_XML_TAG *) */

    /* INPUT element */
    E_BOM_GET_INPUT_CHECKED, /**< Intimates application to get input's checked status.(Argument passed #ST_BOM_INPUT_STATUS*) */
    E_BOM_SET_INPUT_CHECKED, /**< Intimates application to set input's checked status.(Argument passed #ST_BOM_INPUT_STATUS*) */
    E_BOM_GET_INPUT_VALUE, /**< Intimates application to get Input value.Applicable for text,file,password input controls.(Argument passed #ST_BOM_INPUT_VALUE*) */
    E_BOM_SET_INPUT_VALUE, /**< Intimates application to set Input value.Applicable for text,file,password input controls.(Argument passed #ST_BOM_INPUT_VALUE*) */
    E_BOM_CLICK_INPUT, /**< Intimates application to simulate a mouse click for an input control.(Argument passed #ST_XML_TAG *) */

    /* OPTION element */
    E_BOM_GET_OPTION_SELECTED, /**< Intimates application to get OPTION's selected status.(Argument passed #ST_BOM_OPTION_STATUS*) */
    E_BOM_SET_OPTION_SELECTED, /**< Intimates application to set OPTION's selected status.(Argument passed #ST_BOM_OPTION_STATUS*) */

    /* TEXTAREA element */
    E_BOM_GET_TEXTAREA_VALUE, /**< Intimates application to get TextArea value.(Argument passed #ST_BOM_TEXTAREA_VALUE*) */
    E_BOM_SET_TEXTAREA_VALUE, /**< Intimates application to set TextArea value.(Argument passed #ST_BOM_TEXTAREA_VALUE*) */
    E_BOM_GET_TEXTAREA_ROWS, /**< Intimates application to get TextArea rows.(Argument passed #ST_BOM_TEXTAREA_ROWCOL*) */
	E_BOM_GET_TEXTAREA_COLS, /**< Intimates application to get TextArea columns.(Argument passed #ST_BOM_TEXTAREA_ROWCOL*) */
	
	/* IMAGE element */
    E_BOM_GET_IMAGE_WIDTH, /**< Intimates application to get image width.(Argument passed #ST_BOM_IMAGE_DIM*) */
	E_BOM_GET_IMAGE_HEIGHT, /**< Intimates application to get image height.(Argument passed #ST_BOM_IMAGE_DIM*) */
	E_BOM_GET_IMAGE_COMPLETE, /**< Intimates application to get if the image has loaded or not.(Argument passed #ST_BOM_IMAGE_COMPLETE*) */

    E_BOM_REDRAW_PAGE, /**< Intimates application to redraw a page. */
	E_BOM_GET_INNER_HTML, /**< Intemate application to get the contents of an element.(Argument passed #ST_BOM_ELEMENT_CONTENT*)*/
    E_BOM_SET_INNER_HTML, /**< Intimates application to set contents of an element.(Argument passed #ST_BOM_ELEMENT_CONTENT*)*/

	/* HTML element */
	E_BOM_GET_INNER_TEXT, /**< Intemate application to get the contents of an element.(Argument passed #ST_BOM_ELEMENT_CONTENT*)*/
	E_BOM_SET_INNER_TEXT, /**< Intimates application to set text contents for an element.(Argument passed #ST_BOM_ELEMENT_CONTENT*)*/
	E_BOM_GET_ABSOLUTE_URI, /**< Intimates application to get absolute URI.(Argument passed #ST_BOM_ABSOLUTE_URI*) */
	E_BOM_GET_OFFSET_PARENT, /**< Intiates application to get the offset parent for an element.(Argument passed #ST_BOM_OFFSET_VALUE*)*/
	E_BOM_GET_OFFSET_TOP, /**< Intimates application to get the offset top for an element.(Argument passed #ST_BOM_OFFSET_VALUE*)*/
	E_BOM_GET_OFFSET_LEFT, /**< Intimates application to get the offset left for an element.(Argument passed #ST_BOM_OFFSET_VALUE*)*/
	E_BOM_GET_OFFSET_HEIGHT, /**< Intimates application to get the offset height for an element.(Argument passed #ST_BOM_OFFSET_VALUE*)*/
	E_BOM_GET_OFFSET_WIDTH, /**< Intimates application to get the offset width for an element.(Argument passed #ST_BOM_OFFSET_VALUE*)*/
	E_BOM_GET_SCROLL_HEIGHT, /**< Intimates application to get the scroll height value for the element.(Argument passed #ST_BOM_SCROLL_VALUE*)*/
	E_BOM_GET_SCROLL_LEFT, /**< Intimates application to get the scroll left value for the element.(Argument passed #ST_BOM_SCROLL_VALUE*)*/
	E_BOM_GET_SCROLL_TOP, /**< Intimates application to get the scroll top value for the element.(Argument passed #ST_BOM_SCROLL_VALUE*)*/
	E_BOM_GET_SCROLL_WIDTH, /**< Intimates application to get the scroll width value for the element.(Argument passed #ST_BOM_SCROLL_VALUE*)*/
	
	/* Ajax Events */
#ifdef BROWSER_AJAX_ENABLE

	E_BOM_AJAX_INIT_REQ, /**< Intimates application to initialize the AJAX request, by forming the Ajax node.(Argument passed #JC_HANDLE) */
	E_BOM_AJAX_OPEN, /**< Intimates application to add the AJAX request information, by forming the request node.(Argument passed #ST_BOM_AJAX_REQ_OPEN) */
	E_BOM_AJAX_SET_HEADER, /**< Intimates application to add a header to the request.(Argument passed #ST_BOM_AJAX_HEADER*) */
	E_BOM_AJAX_SEND_REQUEST, /**< Intimates application to send the given AJAX request to the server.(Argument passed #ST_BOM_AJAX_REQ_SEND*) */
	E_BOM_AJAX_GET_HTTP_STATUS, /**< Requests application for HTTP STATUS information for an ajax request.(Argument passed #ST_BOM_AJAX_HTTP_STATUS*). */
	E_BOM_AJAX_GET_HTTP_STATUS_TEXT, /**< Requests application for HTTP STATUS TEXT information for an ajax request.(Argument passed #ST_BOM_AJAX_HTTP_STATUS*). */
	E_BOM_AJAX_GET_RESP_TEXT, /**< Requests application for the response of an AJAX request in the form of a text.(Argument passed #ST_BOM_AJAX_RESP_TEXT*) */
	E_BOM_AJAX_GET_RESP_XML, /**< Requests application for the response of an AJAX request in the form of an XML Tree.(Argument passed #ST_BOM_AJAX_RESP_XML*) */
	E_BOM_AJAX_GET_HEADER, /**< Requests application for a specific response header.(Argument passed #ST_BOM_AJAX_HEADER*) */
	E_BOM_AJAX_GET_ALL_HEADER, /**< Requests application for the complete response header.(Argument passed #ST_BOM_AJAX_ALL_HEADER*) */
	E_BOM_AJAX_ABORT, /**< Requests application to stop the AJAX request. (Argument passed JC_HANDLE)*/
	E_BOM_AJAX_DELETE, /**< Requests application to delete the AJAX request information. (Argument passed JC_HANDLE)*/
	E_BOM_AJAX_RESET,  /**< Requests application to reset the AJAX response information. (Argument passed JC_HANDLE)*/
	E_BOM_AJAX_OVERRIDE_MIMETYPE,  /**< Requests application to override the header value of response information. (Argument passed #ST_BOM_AJAX_HEADER*)*/
	E_BOM_AJAX_SET_RESP_XML, /**< Requests application for the storing the responseXML object constructed.(Argument passed #ST_BOM_AJAX_RESP_XML*) */

#endif /* BROWSER_AJAX_ENABLE */

	E_BOM_SET_ATTRIBUTE_VALUE, /**< Intimates application to set attribute value for an element	(Argument passed #ST_BOM_ATTR_INFO*) */	
	E_BOM_GET_CURRENT_ELEMENT, /**< Intimates application to last element processed	(Argument passed #ST_BOM_CURRENT_ELEMENT*) */	
	/* STYLE */
	E_BOM_GET_STYLE_PROPERTY, /**< Intimates application to get style property for an element (Argument passed #ST_BOM_STYLE_INFO*) */ 
	E_BOM_SET_STYLE_PROPERTY, /**< Intimates application to set style property for an element (Argument passed #ST_BOM_STYLE_INFO*) */ 
	E_BOM_MODIFY_DOCUMENT, /**< Intimates application before the updation of a document tree (Argument passed #ST_BOM_MODIFY_DOCUMENT*) */ 
#ifdef WIDGET_OBJECT_EXTENSION
	E_BOM_EMBED_CTOR, /**< Intimates application to register the properties and methods need to be registered to the embed object(Argument passed #JC_HANDLE) */ 
	E_BOM_EMBED_GET, /**< Intimates application to get the value for a property of embed object (Argument passed #ST_BOM_EMBED_GET*)*/ 
#endif 
	E_BOM_GET_SESSION_HANDLE,  /**< Intimates application to get the session handle of a frame or iframe element if any (Argument passed #ST_BOM_SESSION_INFO*) */ 
	E_BOM_GET_OUTER_HTML, /**< Intimate application to get the outer html contents of an element.(Argument passed #ST_BOM_ELEMENT_CONTENT*)*/
    E_BOM_SET_OUTER_HTML, /**< Intimates application to set outer html contents of an element.(Argument passed #ST_BOM_ELEMENT_CONTENT*)*/
	E_BOM_GET_OUTER_TEXT, /**< Intimate application to get the outer text contents of an element.(Argument passed #ST_BOM_ELEMENT_CONTENT*)*/
    E_BOM_SET_OUTER_TEXT, /**< Intimates application to set outer text contents of an element.(Argument passed #ST_BOM_ELEMENT_CONTENT*)*/
	E_BOM_INSERT_ADJACENT_HTML, /**< Intimates application to insert the html content adjacent to given element.(Argument passed #ST_BOM_ADJACENT_HTML*)*/
	E_BOM_GET_FRAME_ELEMENT  /**< Intimates application to get the frame or iframe element for the given session handle if any (Argument passed #ST_BOM_SESSION_INFO*) */ 
} E_BOM_CALLBACK_TYPE ; 

#ifdef BROWSER_AJAX_ENABLE

/**
 * @brief Specifies type of Ajax Resp events that can be sent from application to BOM layer.
 */
typedef enum e_bom_ajax_resp_type
{
	E_BOM_AJAX_NONE, /**< Used to represent no event. */
	E_BOM_AJAX_SEND_FAIL, /**< Intimates BOM Layer that Ajax Request send operation is failed. */
	E_BOM_AJAX_SEND_SUCCESS, /**< Intimates BOM Layer that Ajax Request send operation is success. */
	E_BOM_AJAX_RECEIVING, /**< Intimates BOM Layer that Ajax Request recv operation is in progress. */
	E_BOM_AJAX_RECV_FAIL, /**< Intimates BOM Layer that Ajax Request recv operation is failed. */
    E_BOM_AJAX_RECV_SUCCESS, /**< Intimates BOM Layer that Ajax Request recv operation is success. */
	E_BOM_AJAX_ABORT_SUCCESS, /**< Intimates BOM Layer that Ajax Request has been successfully stopped. */
	E_BOM_AJAX_RESUME /**< Intimates BOM Layer to resume the Ajax script execution. */
} E_BOM_AJAX_RESP_TYPE ;

/**
 * @brief Specifies the Response of the Ajax request in the form of XML Tree
 *
 */
typedef struct st_bom_ajax_resp_event
{
    JC_HANDLE				vAjaxReqHandle ;	/* Specifies Unique Ajax Request Handle. */ 
	E_BOM_AJAX_RESP_TYPE	eAjaxStatus ;		/* Specifies the Ajax status. */
	JC_BOOLEAN				bAsync ;			/* Specifies whether the script execution is asynchronous or not */
} ST_BOM_AJAX_RESP_EVENT ;

#endif /* BROWSER_AJAX_ENABLE */

/**
 * @brief Specifies type of events to send from application to BOM layer.
 */
typedef enum e_bom_event_type
{
	E_BOM_ALERT_RESP, /**< Used by application to send alert response to BOM layer. (Argument to be passed #ST_BOM_ALERT_RESP*) */
	E_BOM_CONFIRM_RESP,	/**< Used by applicaiton to send confirm response to BOM layer. (Argument to be passed #ST_BOM_CONFIRM_RESP*) */
	E_BOM_PROMPT_RESP, /**< Used by application to send prompt response to BOM layer. (Argument to be passed #ST_BOM_PROMPT_RESP*) */
    E_BOM_CONTINUE_RESP, /**< Used by application to send script execution continue response to BOM layer. */
#ifdef BROWSER_AJAX_ENABLE
	E_BOM_AJAX_RESP, /**< Used by application to intimate the BOM Layer about the AJAX Request status. (Argument to be passed #ST_BOM_AJAX_RESP_EVENT) */
	E_BOM_AJAX_DELETE_RESP_XML, /**< Used by application to intimate the BOM Layer about the AJAX Request status. (Argument to be passed #JC_HANDLE) */
#endif /* BROWSER_AJAX_ENABLE */
#ifdef BROWSER_WINDOW_OPEN
	E_BOM_WINDOW_OPEN_RESP, /**< Used by application to send window open confirm response to BOM layer about  **/
#endif
    E_BOM_TIMEOUT_RESP, /**< Used by application to timeout response to BOM layer. (Argument to be passed #ST_BOM_SET_TIMEOUT*)*/
    E_BOM_TIMEOUT_CLEAR, /**< Used by application to intimate BOM layer to clear timeout handles. (Argument to be passed #ST_BOM_SET_TIMEOUT*)*/
	E_BOM_TERMINATE_EXEC /**< Used by application to intimate BOM layer to terminate the script execution.*/
} E_BOM_EVENT_TYPE ;

/**
 * @brief Specifies type of resume from application to BOM layer.
 */
typedef enum e_bom_application_event
{
    E_BOM_NORMAL, 
    E_BOM_EVENT, 
    E_BOM_TIMEOUT
} E_BOM_APPLICATION_EVENT ;

/**
 * @brief Specifies the frame information of a particular child frame
 */
typedef struct st_bom_frame_info
{
   JC_HANDLE			vParentSession ;
   JC_CHAR				*pmFrameName ;
   JC_UINT32			uiFrameIndex ;
} ST_BOM_FRAME_INFO ;

/**
 * @brief	    Prototype of Application callback registered in BOM layer.
 *
 * @param[in]	pvArg Application can pass a context argument in jdi_BOMInitialize.
 *				BOM layer pass the same argument to application when callback is invoked.
 * @param[in]	eCallbackType Specifies application callback type.
 * @param[in]	pvCallbackInfo Specifies callback argument.
 * @return		The callback function can return any application related error.
 *
 * Application shall implement a callback with this prototype and pass its address
 * in jdi_BOMInitialize. BOM layer invokes this callback as needed with required parameters.
 * 
 */
typedef JC_RETCODE (* CB_BOM_GENERAL_CALLBACK) (void					*pvArg,
											    E_BOM_CALLBACK_TYPE 	eCallbackType,
											    void 				    *pvCallbackInfo) ;

/**
 * @brief	    Initializes Browser Object Model
 * @param[in]	vMemHandle              Memory Handle
 * @param[in]	cbAppCallback           Application's general callback 
 * @param[in]   uiOWRThresholdVal       Script Engine's OWR threshold value.
 *										Application may set watchdog timer value
 *										for this parameter
 * @param[out]	pvBOMCtxt               Initialized BOM context
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 * Application shall implement a callback of CB_BOM_GENERAL_CALLBACK prototype.
 * The callback shall handle callback types given in E_BOM_CALLBACK_TYPE.
 */

JC_RETCODE jdi_BOMInitialize (JC_HANDLE                 vMemHandle, 
                              CB_BOM_GENERAL_CALLBACK   cbAppCallback, 
							  JC_CHAR					*pmPath, 
                              JC_UINT32                 uiOWRThresholdVal, 
                              BOM_CONTEXT               *pvBOMCtxt
                             ) ;

/**
 * @brief	    Deinitializes Browser Object Model
 * @param[in]	vBOMCtxt                BOM context
 * @retval 	Nil
 */

void jdi_BOMDeinitialize (BOM_CONTEXT       vBOMCtxt) ;

/**
 * @brief	    Initializes a Parent Frame's Browser Object Model session
 * @param[in]	vBOMCtxt                Initialized BOM context
 * @param[in]	pstXMLTree              XML tree
 * @param[in]	pvAppArg                Application argument to be passed 
 *                                      in cbAppCallback which is passed as parameter 
 *										to jdi_BOMInitialize
 * @param[out]	pvBOMSession            Initialized BOM session
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 * This interface should be called only once to intialize a BOM session for 
 * the parent frame's session. There will be only one parent frame and rest others 
 * will always be child sessions.
 */

JC_RETCODE jdi_BOMInitParentSession (BOM_CONTEXT				vBOMCtxt, 
									 ST_XML_TREE				*pstXMLTree, 
									 void						*pvAppArg, 
									 BOM_SESSION				*pvBOMSession
									 ) ;

/**
 * @brief	    Deinitializes a Parent Frame's Browser Object Model session
 * @param[in]	vBOMSession                Initialized BOM session
 * @retval 	Nil
 *
 * This interface should be called only once to intialize a BOM session for 
 * the parent frame's session.
 */

void jdi_BOMDeinitParentSession (BOM_SESSION vBOMSession) ;

/**
 * @brief	    Initializes a Child Frame's Browser Object Model session
 * @param[in]	vBOMCtxt                Initialized BOM context
 * @param[in]	pstBOMFrameInfo			Frame information of the particular frame
 * @param[in]	pvAppArg                Application argument to be passed 
 *                                      in cbAppCallback which is passed as parameter 
 *										to jdi_BOMInitialize
 * @param[in]	bIsAliasChild			The boolean value will be set in case if the session 
 *					is created during window.opener call. The only difference when compared to
 *					child session creation would be that Self, Parent, Top and Window objects
 *					will be set of the same session's window object just like parent session.
 * @param[out]	pvBOMSession            Initialized BOM session
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 * This interface should be called for every child frame. The frame information contains
 * parent frame handle and frame name and frame index which has to be set by DE.
 */

JC_RETCODE jdi_BOMInitChildSession (BOM_CONTEXT				vBOMCtxt, 
									ST_BOM_FRAME_INFO		*pstBOMFrameInfo, 
									void					*pvAppArg, 
									JC_BOOLEAN				bIsAliasChild,
									BOM_SESSION				*pvBOMSession
									) ;

/**
 * @brief	    Initializes a Child Frame's Document and document related objects
 * @param[in]	vBOMSession             Initialized BOM session
 * @param[in]	pstXMLTree				XML tree
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 * This interface should be called for every child frame.
 */

JC_RETCODE jdi_BOMInitDocument (BOM_SESSION				vBOMSession, 
								ST_XML_TREE				*pstXMLTree
								) ;

/**
 * @brief	    Deinitializes a Child Frame's Browser Object Model session
 * @param[in]	vBOMSession				Initialized BOM session
 * @param[in]	bIsAliasChild			The boolean value will be set in case if the session 
 *					is created during window.opener call. The only difference when compared to
 *					child session creation would be that Self, Parent, Top and Window objects
 *					will be set of the same session's window object just like parent session.
 * @retval 	Nil
 *
 * This interface should be called for freeing all the session information of every 
 * child frame.
 */

void jdi_BOMDeinitChildSession (BOM_SESSION		vBOMSession, 
								JC_BOOLEAN		bIsAliasChild) ;

/**
 * @brief	    Resets a Parent Frame's Browser Object Model session
 * @param[in]	vBOMSession                Initialized BOM session
 * @retval 	JC_OK on success
 *			Script Engine related errors 
 */

JC_RETCODE	jdi_BOMResetParentSession (BOM_SESSION vBOMSession) ;

/**
 * @brief	    Resets a Child Frame's Browser Object Model session
 * @param[in]	vBOMSession                Initialized BOM session
 * @param[in]	bIsAliasChild The boolean value will be set in case if the session 
 *				is created during window.opener call. The only difference when compared to
 *				child session creation would be that Self, Parent, Top and Window objects
 *				will be set of the same session's window object just like parent session.
 * @retval 	JC_OK on success
 *			Script Engine related errors 
 */

JC_RETCODE	jdi_BOMResetChildSession (BOM_SESSION	vBOMSession,
									  JC_BOOLEAN	bIsAliasChild) ;


/**
 * @brief	    Parses and evaluates a Script
 * @param[in]	vBOMSession             Initialized BOM session
 * @param[in]	pstEvalParams			Reference to script evaluation parameters.
 * @param[in]   bHasMoreScript		    E_TRUE if there is more script to 
 *                                      evaluate
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval 	JC_ERR_WAIT_USER_ACTION     Indicates waiting for user action
 * @retval 	JC_ERR_BOM_CONTINUE			Indicates script execution is suspended 
 *										& has to be continued
 * @retval 	JC_ERR_SE_ERROR             Indicates compilation/evaluation error
 */

JC_RETCODE jdi_BOMEvaluateScript (BOM_SESSION			vBOMSession,
                                  ST_BOM_EVAL_PARAMS	*pstEvalParams,
                                  JC_BOOLEAN			bHasMoreScript
                                 ) ;

/**
 * @brief	    Parses and evaluates more Script
 * @param[in]	vBOMSession             Initialized BOM session
 * @param[in]	pstEvalParams			Reference to script evaluation parameters.
 * @param[in]   bHasMoreScript		    E_TRUE if there is more script to 
 *                                      evaluate
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval 	JC_ERR_WAIT_USER_ACTION     Indicates waiting for user action
 * @retval 	JC_ERR_BOM_CONTINUE			Indicates script execution is suspended 
 *										& has to be continued
 * @retval 	JC_ERR_SE_ERROR             Indicates compilation/evaluation error
 * 
 * Application can evaluate a script content as chunks using this API.
 * Application shall call jdi_BOMEvaluateScript (bHasMoreScript is E_TRUE) 
 * for first chunk,and shall call jdi_BOMEvaluateMoreScript for further chunks.
 * bHasMoreScript is E_TRUE for intermediate chunks,and is E_FALSE for last 
 * chunk.
 */

JC_RETCODE jdi_BOMEvaluateMoreScript (BOM_SESSION			vBOMSession,
									  ST_BOM_EVAL_PARAMS	*pstEvalParams, 
                                      JC_BOOLEAN			bHasMoreScript
                                      ) ;

/**
 * @brief	    Set events to BOM layer
 * @param[in]	vBOMSession             Initialized BOM session
 * @param[in]	eEventType              Event type
 * @param[in]	eResumeType             Resume type to be set by application
 * @param[in]	pvEventInfo             Event related information
 * @param[out]	pbAllowDefault			E_TRUE indicates allow event's default
 *                                      action.E_FALSE indicates prevent 
 *                                      event's default action
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval 	JC_ERR_WAIT_USER_ACTION     Indicates waiting for user action
 * @retval 	JC_ERR_BOM_CONTINUE			Indicates script execution is suspended 
 *										& has to be continued
 * @retval 	JC_ERR_BOM_SE_ERROR         Indicates evaluation error
 * 
 * Some script functions (ie alert, prompt, confirm functions) requires 
 * user interaction. Application can choose to implement these functions 
 * as blocking/non-blocking.If application implements as non-blocking,
 * it can use this API to set user interaction details.
 */

JC_RETCODE jdi_BOMSetEvent (BOM_SESSION             vBOMSession, 
                            E_BOM_EVENT_TYPE        eEventType, 
                            E_BOM_APPLICATION_EVENT eResumeType, 
                            void                    *pvEventInfo, 
                            JC_BOOLEAN              *pbAllowDefault
                           ) ;
   
/**
 * @brief	    Fires an event
 * @param[in]	vBOMSession             Initialized BOM session
 * @param[in]	pstEventInfo            Event related information
 * @param[out]	pbAllowDefault			E_TRUE indicates allow event's default
 *                                      action.E_FALSE indicates prevent 
 *                                      event's default action
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval 	JC_ERR_WAIT_USER_ACTION     Indicates waiting for user action
 * @retval 	JC_ERR_BOM_SE_ERROR         Indicates compilation/evaluation error
 *
 * Application shall fire user interaction events and HTML events like
 * onload, onunload etc using this API.
 */
                         
JC_RETCODE jdi_BOMFireEvent (BOM_SESSION          vBOMSession, 
                             ST_BOM_EVENT_INFO    *pstEventInfo, 
							 JC_BOOLEAN			  *pbAllowDefault
                            ) ;

/**
 * @brief Updates BOM when content is added or removed
 * @param[in]  vBOMSession 	    : BOM session handle
 * @param[in]  pstStartTag		: Start ST_XML_TAG* of dynamic XML content.
 *								  (can be JC_NULL)
 * @param[in]  pstEndTag		: End ST_XML_TAG* of dynamic XML content.
 *								  (can be JC_NULL)
 * @param[in]  bAdd				: E_TRUE indicates contents are added.
 *								  E_FALSE indicates contents are removed
 * @retval JC_OK				: Success
 * @retval Other errors
 *
 * The function is used in two scenarios.First case is in BOM session 
 * initialization phase, where XML content can be received in chunks 
 * and DOM tree is built in multiple steps.In this case,pstStartTag and pstEndTag 
 * can be JC_NULL. Second case is when dynamic XML content is generated & DOM tree 
 * needs to be updated.In this case pstStartTag,pstEndTag shall not be JC_NULL.
 **/

JC_RETCODE  jdi_BOMUpdate (BOM_SESSION		vBOMSession,
						   ST_XML_TAG		*pstStartTag, 
						   ST_XML_TAG		*pstEndTag, 
						   JC_BOOLEAN		bAdd
						   ) ;

/**
 * @brief Intimates application if Document is initiallized or not
 * @param[in]  vBOMSession 	    : BOM session handle
 * @retval E_TRUE document has been initialized and E_FALSE document 
 *			has not been initialized
 *
 * The function is used to check if BOM Document for the particular session has been 
 * initialized or not.
 **/

JC_BOOLEAN  jdi_BOMIsDocumentInitialized (BOM_SESSION		vBOMSession) ;

#ifdef BROWSER_CUSTOM_SCRIPT

/**
 * @brief Initializes an Custom Object from application
 * @param[in]	vBOMSession             BOM Session
 * @param[in]	pstCustomObjInfo        Info about the object to be initialized  
 * @param[out]  pvObjHandle				Pointer to BOM handle for this object
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 * Application shall implement a callback of CB_BOM_CUSTOM_CALLBACK prototype.
 * The callback shall handle callback types given in E_BOM_CUSTOM_CALLBACK.
 */
JC_RETCODE jdi_BOMInitCustomObject (BOM_SESSION				vBOMSession,
								    ST_BOM_CUSTOM_OBJECT	*pstCustomObjInfo,
									JC_BOOLEAN				bHasInstance,
								    JC_HANDLE				*pvObjHandle) ;

/**
 * @brief Registers a method with the Custom Object in the session
 * @param[in]	vBOMSession             Initialized BOM Session
 * @param[in]	vObjHandle				Object handle returned from jdi_BOMInitCustomObject 
 * @param[out]  pstMethodInfo			Method info to be regestered.
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 */
JC_RETCODE jdi_BOMRegisterCustomObjMethod (BOM_SESSION			vBOMSession,
										   JC_HANDLE			vObjHandle,
										   ST_BOM_CUSTOM_METHOD	*pstMethodInfo) ;


/**
 * @brief Registers a property with the Custom Object 
 * @param[in]	vBOMSession             Initialized BOM Session
 * @param[in]	vObjHandle				New Object handle supplied from BOM in ST_BOM_CUSTOM_CTOR
 * @param[out]  pstPropInfo				Property info to be regestered.
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 */
JC_RETCODE jdi_BOMRegisterCustomObjProp (BOM_SESSION			vBOMSession,
									     JC_HANDLE				vObjHandle,
									     ST_BOM_CUSTOM_PROPERTY	*pstPropInfo) ;

/**
 * @brief	Gives the reference ID that will be used for the correspondence 
 *			with BOM for each particular property. After the session is created
 * @param[in]		vBOMSession             Initialized BOM Session
 * @param[in]		uiPropCnt				Number of properties in the list
 * @param[in/out]	pstPropList				Reference to the property list.
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 */
JC_RETCODE jdi_BOMGetCustomPropertyID (BOM_SESSION				vBOMSession,
									   JC_UINT32				uiPropCnt,
									   ST_BOM_CUSTOM_PROP_LIST	*pstPropList) ;

/**
 * @brief Gets the value contained in a custom object method's arguement 
 * @param[in]	vBOMSession             Initialized BOM Session
 * @param[in]	vArgsPtr	            Pointer to arguement value
 * @param[in]   bArray					Specify weather vArgsPtr is an array or not
 * @param[in]   iIndex					Index of the argument list.
 * @param[out]  pstResult				Data contained in vArgsPtr.
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 */
JC_RETCODE jdi_BOMGetCustomObjArgValue (BOM_SESSION			vBOMSession,
									    JC_HANDLE			vArgsPtr,
										JC_BOOLEAN			bArray,
										JC_INT32			iIndex,
									    ST_BOM_VALUE		*pstResult) ;

/**
 * @brief Free the data inside bom value that has been obtained from 
 * @param[in]	vBOMSession             Initialized BOM Session
 * @param[in]	pstValue				Reference to the BOM value.
 * @retval 	No return value
 *
 * The data inside bom value that has been obtained from jdi_BOMGetCustomObjArgValue()
 * is freed.
 */
void jdi_BOMFreeCustomValue (BOM_SESSION		vBOMSession,
							 ST_BOM_VALUE		*pstValue) ;

/**
 * @brief Copy the BOM value and returns a reference to the copied value
 * @param[in]	vBOMSession             Initialized BOM Session
 * @param[in]   pstValue				Contain value which need to be copied
 * @param[in]   pstResult				Reference to the copied value.
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 */
JC_RETCODE jdi_BOMCopyCustomValue (BOM_SESSION			vBOMSession,
								   ST_BOM_VALUE			*pstValue,
								   ST_BOM_VALUE			*pstResult) ;

/**
 * @brief Sets the result of the custom method/get call back in to Result
 * @param[in]	vBOMSession             Initialized BOM Session
 * @param[in]   pstValue				Contain Result value which will be returned
 * @param[in]   vResult					Pointer to Result value. e.g. vResultVal 
 *										of CB_BOM_CUSTOM_METHOD callback.
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 */
JC_RETCODE jdi_BOMSetCustomResult (BOM_SESSION			vBOMSession,
								   ST_BOM_VALUE			*pstValue,
								   JC_HANDLE			vResult) ;

/** 
 * @brief Executes the given function object 
 * @param[in]  vBOMSession				BOM session
 * @param[in]  vFunObjVal				Function Object to be executed
 * @param[in]  pstArgList				List of arguments if any
 * @param[in]  uhNoOfArgs				No of arguments to the function
 * @param[out] pstResult					Result value
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval 	JC_ERR_WAIT_USER_ACTION     Indicates waiting for user action
 * @retval 	JC_ERR_BOM_CONTINUE			Indicates script execution is suspended 
 *										& has to be continued
 * @retval 	JC_ERR_SE_ERROR             Indicates compilation/evaluation error
 */
JC_RETCODE jdi_BOMExecuteFunctionObject (BOM_SESSION		vBOMSession,
										 JC_HANDLE			vFunObjVal,
										 ST_BOM_VALUE		*pstArgList, 
										 JC_UINT16			uhNoOfArgs,
										 ST_BOM_VALUE		*pstResult
										 ) ;

/**
 * @brief Gets the value contained in a custom object method's arguement 
 * @param[in]	vObjHandle      Initialized BOM Session
 * @retval 	JC_HANDLE			Custom data pointer if any set for the object or JC_NULL
 *
 */
JC_HANDLE jdi_BOMGetCustomObjData (JC_HANDLE	vObjHandle) ;

/**
 * @brief Deinitializes an Custom Object 
 * @param[in]	vBOMSession             Initialized BOM Session
 * @param[in]	vObjHandle              Object handle returned from jdi_BOMInitAppObject 
 * @param[out]	pvCustomDataPtr         Pointer to the custom data
 * @retval 	No return value
 *
 */
void jdi_BOMDeinitCustomObject (BOM_CONTEXT		vBOMContext,
							    JC_HANDLE		vObjHandle) ;

#endif /* BROWSER_CUSTOM_SCRIPT */
/**
 * @brief Script engine evaluation/compilation error
 */
#define JC_ERR_BOM_SE_ERROR			TERMINAL_ERR_CODE(BOM_MODULE, 0x01)
#define JC_ERR_BOM_CONTINUE			TERMINAL_ERR_CODE(BOM_MODULE, 0x02)
#define JC_ERR_BOM_SET_TIMER		TERMINAL_ERR_CODE(BOM_MODULE, 0x03)
#define JC_ERR_BOM_WRONG_ARGS		TERMINAL_ERR_CODE(BOM_MODULE, 0x04)
#define JC_ERR_BOM_STACK_OVERFLOW   TERMINAL_ERR_CODE(BOM_MODULE, 0x05)

#endif /* _JDI_BOM_H_ */

