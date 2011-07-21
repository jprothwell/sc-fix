/***************************************************************************
 * $Id: jdi_events.h,v 1.8 2008/09/30 09:44:14 browserdevjtbase Exp $
 * $Revision: 1.8 $
 * $Date: 2008/09/30 09:44:14 $
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
 * $Log: jdi_events.h,v $
 * Revision 1.8  2008/09/30 09:44:14  browserdevjtbase
 * Core Updates
 *
 * Revision 1.16  2008/09/17 15:10:27  shajudevhtmlbrow
 * Changes for mouse event, key event , onselect and onchange
 *
 * Revision 1.15  2008/07/23 04:33:59  kumardevhtmlbrow
 * Compilation Warning updated
 *
 * Revision 1.14  2008/05/26 12:08:16  shajudevhtmlbrow
 * Dispatch event is changed to pass the event type.
 *
 * Revision 1.13  2008/03/04 06:02:37  shajudevhtmlbrow
 * changes for add/remove event listeners and restructuring event flow
 *
 * Revision 1.12  2007/08/08 10:53:10  babithapkdevhtmlbrow
 * Keep function Object also for Event Listener.
 *
 * Revision 1.11  2007/07/12 08:34:59  babithapkdevhtmlbrow
 * A single DOM structure (ie ST_DOM_NODE) is associated with
 * an ST_XML_TAG & ST_XML_ATTRIBUTE.
 *
 * Revision 1.10  2007/07/03 08:10:47  babithapkdevhtmlbrow
 * Removed ddl.h & jcal.h from interface files
 *
 * Revision 1.9  2007/06/29 12:10:57  babithapkdevhtmlbrow
 * For an event attribute, the attribute value in XML tree is used in Event Listener table.
 *
 * Revision 1.8  2007/05/21 09:29:00  babithapkdevhtmlbrow
 * Live objects and event listeners structures are updated when dynamic content is added/removed.
 *
 * Revision 1.7  2007/05/08 06:56:29  babithapkdevhtmlbrow
 * Added jdi_DOMGetCurrentEventTarget
 *
 * Revision 1.6  2007/05/03 11:34:13  babithapkdevhtmlbrow
 * Updated EventListeners structure when xml content generated dynamically
 *
 * Revision 1.5  2007/04/25 11:37:18  babithapkdevhtmlbrow
 * Renamed pbNoPreventDefault parameter as pbAllowDefault in jdi_DOMFireHTMLEvent & jdiDOMDispatchEvent.
 * Supported user interaction script methods in event flow.
 *
 * Revision 1.4  2007/02/26 09:18:41  babithapkdevhtmlbrow
 * Changed E_DOM_EVENT_NO_EXCEPTION enum value
 *
 * Revision 1.3  2007/02/23 06:21:58  babithapkdevhtmlbrow
 * Supported prevent default action of events
 *
 * Revision 1.2  2007/01/17 08:51:56  babithapkdevhtmlbrow
 * Handled return code of event handler script evaluation.
 *
 * Revision 1.1  2007/01/10 11:23:55  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

#ifndef	 _DOM_EVENT_H_
#define	 _DOM_EVENT_H_

#include <jdi_dom.h>

#define DOM_UI_EVENTS		   "UIEvents"
#define DOM_MOUSE_EVENTS	   "MouseEvents"
#define DOM_MUTATION_EVENTS	   "MutationEvents"
#define DOM_HTML_EVENTS        "HTMLEvents"

/* UIEvent types */
#define DOM_EVENT_FOCUS_IN    "DOMFocusIn"
#define DOM_EVENT_FOCUS_OUT   "DOMFocusOut"
#define DOM_EVENT_ACTIVATE    "DOMActivate"

#define DOM_EVENT_LOAD		  "load"
#define DOM_EVENT_UNLOAD	  "unload"
/* MouseEvent types */
#define DOM_EVENT_CLICK        "click"
#define DOM_EVENT_MOUSE_DOWN   "mousedown"
#define DOM_EVENT_MOUSE_UP     "mouseup"
#define DOM_EVENT_MOUSE_OVER   "mouseover"
#define DOM_EVENT_MOUSE_MOVE   "mousemove"
#define DOM_EVENT_MOUSE_OUT    "mouseout"
/* KeyEvent types */
#define DOM_EVENT_KEYUP			"keyup"
#define DOM_EVENT_KEYDOWN		"keydown"
#define DOM_EVENT_KEYPRESS		"keypress"
/* MutationEvent types */
#define DOM_EVENT_SUBTREE_MODIFIED             "DOMSubtreeModified"
#define DOM_EVENT_NODE_INSERTED                "DOMNodeInserted"
#define DOM_EVENT_NODE_REMOVED                 "DOMNodeRemoved"
#define DOM_EVENT_NODE_REMOVED_FROM_DOCUMENT   "DOMNodeRemovedFromDocument"
#define DOM_EVENT_NODE_INSERTED_INTO_DOCUMENT  "DOMNodeInsertedIntoDocument"
#define DOM_EVENT_ATTR_MODIFIED                "DOMAttrModified"
#define DOM_EVENT_CHARACTER_DATA_MODIFIED      "DOMCharacterDataModified"

/* HTML Intrinsic Events */
#define	DOM_ONLOAD			"onload"
#define	DOM_ONUNLOAD		"onunload"
#define	DOM_ONCLICK			"onclick"
#define	DOM_ONDBLCLICK		"ondblclick"
#define	DOM_ONMOUSEUP		"onmouseup"
#define	DOM_ONMOUSEOVER		"onmouseover"
#define	DOM_ONMOUSEMOVE		"onmousemove"
#define	DOM_ONMOUSEOUT		"onmouseout"
#define	DOM_ONMOUSEDOWN		"onmousedown"
#define	DOM_ONFOCUS			"onfocus"
#define	DOM_ONBLUR			"onblur"
#define	DOM_ONKEYPRESS		"onkeypress"
#define	DOM_ONKEYDOWN		"onkeydown"
#define	DOM_ONKEYUP			"onkeyup"
#define	DOM_ONSUBMIT		"onsubmit"
#define	DOM_ONRESET			"onreset"
#define	DOM_ONSELECT		"onselect"
#define	DOM_ONCHANGE		"onchange"

/* DOM Level 0 events */
#define	DOM_ONABORT		    "onabort"
#define	DOM_ONERROR		    "onerror"
#define	DOM_ONRESIZE		"onresize"
#define	DOM_ONSCROLL		"onscroll"

typedef DOM_NODE        DOM_EVENT_TARGET ;
typedef JC_HANDLE       DOM_EVENT_LISTENER ;
typedef	JC_HANDLE		DOM_EVENT ;
typedef JC_HANDLE       DOM_UI_EVENT_VIEW ; /* TODO */

typedef enum domEventPhase
{
    E_DOM_NO_PHASE = 0, 
	E_DOM_CAPTURING_PHASE   = 1,
	E_DOM_AT_TARGET,
	E_DOM_BUBBLING_PHASE
} E_DOM_EVENT_PHASE ;

typedef enum domAttrChangeType
{
    E_DOM_ATTR_CHANGE_NONE = 0, /* not in specification */
    E_DOM_ATTR_CHANGE_MODIFICATION = 1, 
    E_DOM_ATTR_CHANGE_ADDITION, 
    E_DOM_ATTR_CHANGE_REMOVAL
} E_DOM_ATTR_CHANGE_TYPE ;

typedef enum domEventException
{
    E_DOM_UNSPECIFIED_EVENT_TYPE_ERR = 0, 
    E_DOM_EVENT_NO_EXCEPTION
} E_DOM_EVENT_EXCEPTION ;

/**
 * @brief Initialize EventListeners with HTML Event Attributes
 * @param[in]  vDocument		: Document handle 
 * @param[in]  pstStartTag		: Start ST_XML_TAG* of dynamic xml content.
 *								  (can be JC_NULL)
 * @param[in]  pstEndTag		: End ST_XML_TAG* of dynamic xml content.
 *								  (can be JC_NULL)
 * @param[in]  bAdd				: E_TRUE indicates contents are added
 *								  E_FALSE indicates contents are removed
 * @retval 	JC_OK               : Indicates success
 * @retval Other errors
 *
 * EventListeners structure is updated in two scenarios.First case is DOM 
 * initialization phase, where an XML content can be received in chunks.
 * In this case,pstStartTag and pstEndTag can be JC_NULL. Second case is 
 * when dynamic xml content is generated.In this case pstStartTag,pstEndTag 
 * shall be non-JC_NULL.
 */

JC_RETCODE	jdi_DOMInitListeners (DOM_DOCUMENT	vDocument,
								  ST_XML_TAG	*pstStartTag, 
								  ST_XML_TAG	*pstEndTag, 
								  JC_BOOLEAN	bAdd
								 ) ;

/* Cleanup EventListeners */
void jdi_DOMCleanupListeners (DOM_DOCUMENT     Document) ;

JC_RETCODE jdi_DOMFireEvent (DOM_DOCUMENT		    Document, 
							 ST_DOM_EVENT_INFO		*pstDomEventInfo, 
                             E_DOM_EVENT_EXCEPTION  *peEventException
                            ) ;

JC_RETCODE	jdi_DOMFireMutationEvent (DOM_DOCUMENT				Document, 
									  DOM_NODE					EventTarget, 
									  JC_INT8					*pcEventType, 
                                      DOM_NODE                  RelatedNode, 
									  JC_CHAR					*pmPrevValue, 
									  JC_CHAR					*pmNewValue, 
									  JC_INT8					*pcAttrName, 
									  E_DOM_ATTR_CHANGE_TYPE	eAttrChange
                                      ) ;

/* EventTarget interfaces */
JC_RETCODE jdi_DOMAddEventListener	(DOM_DOCUMENT				Document, 
                                     DOM_EVENT_TARGET			EventTarget, 
									 JC_INT8					*pcEventType, 
									 JC_HANDLE					vListener, 
									 E_DOM_EVENT_LISTENER		eListener, 
									 JC_BOOLEAN					bUseCapture 
									) ; 

void	jdi_DOMRemoveEventListener	(DOM_DOCUMENT				Document, 
                                     DOM_EVENT_TARGET			EventTarget, 
									 JC_INT8					*pcEventType, 
									 JC_HANDLE					vListener, 
									 E_DOM_EVENT_LISTENER		eListener, 
									 JC_BOOLEAN					bUseCapture
									 ) ;

/**
 * @brief Dispatches the specified event into the event system..
 * @param[in]  vDocument			: Document handle 
 * @param[in]  pstDomEventInfo		: Event related information
 * @param[in]  vEvent				: Event to be dispatched.
 * @param[out]  peException			: Exceptions generated (if any)
 * @retval 	JC_OK               : Indicates success
 * @retval Other errors
 *
 * Note:- If the the target is specified as document node, then make target to Body tag. 
 *		  This is done because listeners on Document are added on Body itself.
 */

JC_RETCODE	jdi_DOMDispatchEvent (DOM_DOCUMENT		    vDocument,
								  ST_DOM_EVENT_INFO		*pstDomEventInfo, 
								  DOM_EVENT		        vEvent, 
                                  E_DOM_EVENT_EXCEPTION	*peException) ;

/* EventListener interfaces TODO */
JC_RETCODE jdi_DOMHandleEvent (DOM_DOCUMENT         Document, 
                               DOM_EVENT_LISTENER   EventListener, 
                               DOM_EVENT            Event) ;

/* Event interfaces */
const JC_INT8 *	jdi_DOMGetEventType	(DOM_EVENT		Event) ;

JC_RETCODE jdi_DOMGetEventTarget (DOM_DOCUMENT		Document, 
								  DOM_EVENT			Event, 
								  DOM_EVENT_TARGET	*pvTarget) ;

JC_RETCODE jdi_DOMGetEventCurrentTarget (DOM_DOCUMENT		Document, 
										 DOM_EVENT			Event, 
										 DOM_EVENT_TARGET	*pvCurrTarget) ;

E_DOM_EVENT_PHASE jdi_DOMGetEventPhase (DOM_EVENT 	Event) ;

JC_BOOLEAN jdi_DOMEventCanBubble (DOM_EVENT		Event) ;

JC_BOOLEAN jdi_DOMEventCancelable (DOM_EVENT		Event) ;

DOM_TIMESTAMP jdi_DOMGetEventTimeStamp (DOM_EVENT   Event) ;

void jdi_DOMEventStopPropagation (DOM_EVENT   Event) ;

void jdi_DOMEventPreventDefault (DOM_EVENT   Event) ;

JC_RETCODE jdi_DOMInitEvent	(DOM_DOCUMENT   Document, 
                             DOM_EVENT		Event,
							 JC_INT8		*pcEventType,
							 JC_BOOLEAN		bCanBubble, 
							 JC_BOOLEAN		bCancelable) ;

/* DocumentEvent interfaces */
JC_RETCODE	jdi_DOMCreateEvent (DOM_DOCUMENT        Document, 
                                JC_INT8			    *pcEventType, 
							    DOM_EVENT		    *pEvent, 
							    E_DOM_EXCEPTION	    *peException) ;

void jdi_DOMFreeEvent (DOM_DOCUMENT		Document,
				       DOM_EVENT	    Event) ;

/* UIEvent interfaces */
DOM_UI_EVENT_VIEW jdi_DOMGetUIEventView (DOM_EVENT  UIEvent) ;

JC_INT32 jdi_DOMGetUIEventDetail (DOM_EVENT  UIEvent) ;

JC_RETCODE	jdi_DOMInitUIEvent	(DOM_DOCUMENT        Document, 
                                 DOM_EVENT           Event, 
                                 JC_INT8		     *pcEventType, 
							     JC_BOOLEAN		     bCanBubble, 
							     JC_BOOLEAN		     bCancelable, 
                                 DOM_UI_EVENT_VIEW   vEventView, 
							     JC_INT32            iDetailArg) ;


/* MouseEvent interfaces */
JC_INT32 jdi_DOMGetMouseEventScreenX (DOM_EVENT Event) ;

JC_INT32 jdi_DOMGetMouseEventScreenY (DOM_EVENT Event) ;

JC_INT32 jdi_DOMGetMouseEventClientX (DOM_EVENT Event) ;

JC_INT32 jdi_DOMGetMouseEventClientY (DOM_EVENT Event) ;

JC_BOOLEAN jdi_DOMGetMouseEventCtrlKey (DOM_EVENT   Event) ;

JC_BOOLEAN jdi_DOMGetMouseEventShiftKey (DOM_EVENT   Event) ;

JC_BOOLEAN jdi_DOMGetMouseEventAltKey (DOM_EVENT   Event) ;

JC_BOOLEAN jdi_DOMGetMouseEventMetaKey (DOM_EVENT   Event) ;

JC_UINT16 jdi_DOMGetMouseEventButton (DOM_EVENT     Event) ;

DOM_EVENT_TARGET jdi_DOMGetMouseEventRelatedTarget (DOM_EVENT Event) ;

JC_RETCODE  jdi_DOMInitMouseEvent  (
                                    DOM_DOCUMENT        Document, 
                                    DOM_EVENT           Event, 
                                    JC_INT8		        *pcEventType, 
							        JC_BOOLEAN		    bCanBubble, 
							        JC_BOOLEAN		    bCancelable, 
                                    DOM_UI_EVENT_VIEW   vEventView, 
							        JC_INT32            iDetailArg, 
                                    JC_INT32            iScreenX, 
                                    JC_INT32            iScreenY, 
                                    JC_INT32            iClientX, 
                                    JC_INT32            iClientY, 
                                    JC_BOOLEAN          bCtrlKey, 
                                    JC_BOOLEAN          bAltKey, 
                                    JC_BOOLEAN          bShiftKey, 
                                    JC_BOOLEAN          bMetaKey, 
                                    JC_UINT16           uiButton, 
                                    DOM_EVENT_TARGET    RelatedTarget
                                    ) ;

/* MutationEvent interfaces */
DOM_NODE jdi_DOMGetMutationEventRelatedNode (DOM_EVENT  Event) ;

const JC_CHAR * jdi_DOMGetMutationEventPrevValue (DOM_EVENT Event) ;

const JC_CHAR * jdi_DOMGetMutationEventNewValue (DOM_EVENT  Event) ;

const JC_INT8 * jdi_DOMGetMutationEventAttrName (DOM_EVENT  Event) ;

E_DOM_ATTR_CHANGE_TYPE jdi_DOMGetMutationEventAttrChange (DOM_EVENT   Event) ;

JC_RETCODE jdi_DOMInitMutationEvent (DOM_DOCUMENT				Document,
									 DOM_EVENT                  Event, 
                                     JC_INT8		            *pcEventType, 
							         JC_BOOLEAN		            bCanBubble, 
							         JC_BOOLEAN		            bCancelable, 
                                     DOM_NODE                   RelatedNode, 
                                     JC_CHAR                    *pmPrevValue, 
                                     JC_CHAR                    *pmNewValue, 
                                     JC_INT8                    *pcAttrName, 
                                     E_DOM_ATTR_CHANGE_TYPE     eAttrChange
                                    ) ;

JC_RETCODE  jdi_DOMResumeEventFlow (DOM_DOCUMENT    Document, 
                                    JC_BOOLEAN      *pbAllowDefault) ;

/**
 * @brief Returns current target of current event being processed.
 * @param[in]  vDocument		: Document handle
 * @param[out] pvCurrentTarget	: Current target node (JC_NULL if event
 *								  is not being processed)
 * @retval 	JC_OK	Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION	Indicates memory allocation error
 *
 * Caller has to free current target using jdi_DOMFreeNode ().
 */

JC_RETCODE	jdi_DOMGetCurrentEventTarget (DOM_DOCUMENT			vDocument, 
										  DOM_EVENT_TARGET		*pvCurrentTarget) ;

#endif

/* End of File */

