/***************************************************************************
 * $Id: jdi_dom.h,v 1.21 2009/04/13 06:38:21 browserdevjtbase Exp $
 * $Revision: 1.21 $
 * $Date: 2009/04/13 06:38:21 $
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
 * $Log: jdi_dom.h,v $
 * Revision 1.21  2009/04/13 06:38:21  browserdevjtbase
 * Core Update
 *
 * Revision 1.32  2009/03/18 12:47:15  shajudevhtmlbrow
 * New error code jdi_ERR_DOM_CONTINUE for resuming eventflow and
 * new ESMP event object.
 *
 * Revision 1.31  2008/11/11 11:09:24  shajudevhtmlbrow
 * BOM & DOM peak memory optimization changes
 *
 * Revision 1.30  2008/10/30 14:12:49  shajudevhtmlbrow
 * changes for proper nodlist updation.
 *
 * Revision 1.29  2008/09/15 11:08:26  shajudevhtmlbrow
 * chnges for mukltiple document in single session and same host object.
 *
 * Revision 1.28  2008/08/08 11:02:09  shajudevhtmlbrow
 * changes for createComment and processing instruction.
 *
 * Revision 1.27  2008/07/23 04:38:00  kumardevhtmlbrow
 * Compilation warning updated.
 *
 * Revision 1.26  2008/07/14 07:34:23  shajudevhtmlbrow
 * support for node::isSupported feature added
 *
 * Revision 1.25  2008/06/25 14:28:36  shajudevhtmlbrow
 * Script features (insertAdjescentElement/Text/HTML) added
 *
 * Revision 1.24  2008/06/24 05:23:24  shajudevhtmlbrow
 * changes for x-dev issues.
 *
 * Revision 1.23  2008/06/02 07:11:48  shajudevhtmlbrow
 * code review changes for new option updation through script.
 *
 * Revision 1.22  2008/05/26 07:13:47  shajudevhtmlbrow
 * changes for adding/removing option through script
 *
 * Revision 1.21  2008/05/12 04:56:56  shajudevhtmlbrow
 * Code review changes.
 *
 * Revision 1.20  2008/05/08 10:58:40  shajudevhtmlbrow
 * Changes for restricting the document search up to current element.
 *
 * Revision 1.19  2008/04/01 04:53:34  shajudevhtmlbrow
 * Event handling changes for setting event object in this object
 *
 * Revision 1.18  2008/03/04 06:02:37  shajudevhtmlbrow
 * changes for add/remove event listeners and restructuring event flow
 *
 * Revision 1.17  2008/02/19 12:08:25  shajudevhtmlbrow
 * DOM optimization changes
 *
 * Revision 1.16  2008/01/18 08:02:08  kumardevhtmlbrow
 * uiRefCnt in the node and nodegroup has been updated to uhRefCnt for the memory reduction. Re-organization of the members inside the node & nodegroup for less structure size for the 4 byte alignment platform.
 *
 * Revision 1.15  2007/10/08 14:09:51  kavithadevhtmlbrow
 * Removed ESMP_V_1_0 & BOM_OMA_ESMP_CONFORMANCE macro and done based on runtime
 *
 * Revision 1.14  2007/09/05 05:12:10  kavithadevhtmlbrow
 * DOM Attribute, Content type in EvaluateScript, Script Cont Status in DE
 *
 * Revision 1.13  2007/08/20 08:24:48  kavithadevhtmlbrow
 * vTarget added for eventlistener callback
 *
 * Revision 1.12  2007/08/17 07:12:16  babithapkdevhtmlbrow
 * For Text.splitText(), iOffset parameter is made long.
 *
 * Revision 1.11  2007/08/08 10:53:10  babithapkdevhtmlbrow
 * Keep function Object also for Event Listener.
 *
 * Revision 1.10  2007/07/12 09:14:46  babithapkdevhtmlbrow
 * Now Application interacts with DOM through BOM.
 *
 * Revision 1.9  2007/07/12 08:34:59  babithapkdevhtmlbrow
 * A single DOM structure (ie ST_DOM_NODE) is associated with
 * an ST_XML_TAG & ST_XML_ATTRIBUTE.
 *
 * Revision 1.8  2007/07/03 08:10:47  babithapkdevhtmlbrow
 * Removed ddl.h & jcal.h from interface files
 *
 * Revision 1.7  2007/06/07 12:37:59  babithapkdevhtmlbrow
 * Modified Live objects (ie NodeList,HTMLCollection, HTMLOptionsCollection)
 * when content is added or removed
 *
 * Revision 1.6  2007/05/21 09:29:00  babithapkdevhtmlbrow
 * Live objects and event listeners structures are updated when dynamic content is added/removed.
 *
 * Revision 1.5  2007/05/03 11:35:29  babithapkdevhtmlbrow
 * Modified jdi_DOMUpdate to handle dynamic xml content
 *
 * Revision 1.4  2007/04/25 11:31:32  babithapkdevhtmlbrow
 * Renamed pbNoPreventDefault parameter as pbAllowDefault of CB_DOM_EVAL_SCRIPT
 *
 * Revision 1.3  2007/03/02 11:47:21  shreyasdevhtmlbrow
 * RVCT Compilation fix
 *
 * Revision 1.2  2007/02/23 06:21:58  babithapkdevhtmlbrow
 * Supported prevent default action of events
 *
 * Revision 1.1  2007/01/10 11:23:55  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

#ifndef _JDI_DOM_H_
#define _JDI_DOM_H_

#include <jdi_xmldatatype.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum domException
{
    E_DOM_NO_EXCEPTION = 0,
    E_DOM_INDEX_SIZE_ERR,
    E_DOM_STRING_SIZE_ERR,
    E_DOM_HIERARCHY_REQUEST_ERR,
    E_DOM_WRONG_DOCUMENT_ERR,         
    E_DOM_INVALID_CHARACTER_ERR,      
    E_DOM_NO_DATA_ALLOWED_ERR,        
    E_DOM_NO_MODIFICATION_ALLOWED_ERR,
    E_DOM_NOT_FOUND_ERR,
    E_DOM_NOT_SUPPORTED_ERR,          
    E_DOM_INUSE_ATTRIBUTE_ERR,        
    E_DOM_INVALID_STATE_ERR,          
    E_DOM_SYNTAX_ERR, 
    E_DOM_INVALID_MODIFICATION_ERR,   
    E_DOM_NAMESPACE_ERR,          
    E_DOM_INVALID_ACCESS_ERR 
} E_DOM_EXCEPTION ;

typedef JC_HANDLE       DOM_NODE ;
typedef JC_HANDLE       DOM_CHARACTER_DATA ;
typedef JC_HANDLE       DOM_ELEMENT ;
typedef JC_HANDLE       DOM_ATTRIBUTE ;
typedef JC_HANDLE       DOM_TEXT ;
typedef JC_HANDLE       DOM_CDATA_SECTION ;
typedef JC_HANDLE       DOM_COMMENT ;
typedef JC_HANDLE       DOM_DOCUMENT_FRAGMENT ;
typedef JC_HANDLE       DOM_PROCESSING_INSTR ;
typedef JC_HANDLE       DOM_ENTITY ;
typedef JC_HANDLE       DOM_ENTITY_REFERENCE ;
typedef JC_HANDLE       DOM_NOTATION ;
typedef JC_HANDLE       DOM_DOCUMENT_TYPE ;
typedef JC_HANDLE       DOM_IMPLEMENTATION ;
typedef JC_HANDLE       DOM_DOCUMENT ;
typedef JC_HANDLE       DOM_NODE_LIST ;
typedef JC_HANDLE       DOM_NAMED_NODE_MAP ;

typedef JC_UINT32       DOM_TIMESTAMP ;

typedef enum domInitParamType
{
    E_DOM_INITPARAM_BUFFER,
    E_DOM_INITPARAM_TREE
} E_DOM_INITPARAM_TYPE ;

typedef enum domEventListener
{
    E_DOM_EVENT_LISTENER_ATTR = 0, 
	E_DOM_EVENT_LISTENER_SCRIPT,
	E_DOM_EVENT_LISTENER_ASSIGN
} E_DOM_EVENT_LISTENER ;

typedef enum domObjectType
{
	E_TYPE_NODE,
	E_TYPE_OTHER_OBJECTS
} E_DOM_OBJECT_TYPE ;

typedef struct domInitBuffer
{
    JC_UINT8        *pucBuffer ;
    JC_UINT32       uiLength ;
} ST_DOM_INIT_BUFFER ;

typedef union domInitParam 
{
    ST_DOM_INIT_BUFFER  *pstBuffer ;
    ST_XML_TREE         *pstTree ; /* TODO: dependency with XML */
} U_DOM_INITPARAM ;

typedef struct stDomEvalScript
{
	JC_HANDLE					vTarget ; /*Species the Event Target */
	JC_HANDLE					vListener ; /*Specifies event listener.*/
	E_DOM_EVENT_LISTENER		eListener ; /*Specifies the type of add/remove listener*/
	JC_BOOLEAN					bAllowDefault ; /*Specifies whether event handler returns false
									  If an event handler returns false, pbAllowDefault is E_FALSE.
									  Don't modify this value inside callback function.*/
} ST_DOM_EVAL_SCRIPT ;

typedef struct stDomEventInfo
{
	JC_INT8			*pcEventType ;
	DOM_NODE		vEventTarget ;
	JC_HANDLE		vListener ;
	JC_BOOLEAN		bAllowDefault ;
} ST_DOM_EVENT_INFO ;

typedef struct stDomSetAttribute
{
	ST_XML_TAG			*pstElement ; /*Specifies the Element whose attribute is to be modified */
	JC_INT8				*pcAttribute ; /*Specifies the attribute is to be modified */
	JC_CHAR				*pmValue ; /* Specifies the new attribute value */
	JC_BOOLEAN			bCaseSensitive ; /* E_TRUE if ESMP_V_1_0 is enabled or else E_FALSE */
	ST_XML_ATTRIBUTE	*pstAttribute ; /* Return param used to update nodegroup list */
} ST_DOM_SET_ATTRIBUTE ;

typedef struct stDomCurrentElt
{
	ST_XML_TAG		*pstElement ;
} ST_DOM_CURRENT_ELEMENT ;

typedef struct stDomDeleteObj
{
	JC_HANDLE			vDomObj ;
	E_DOM_OBJECT_TYPE	eType ;
} ST_DOM_DELETE_OBJECT ;

typedef enum e_dom_callback_type
{
	E_DOM_EVAL_EVENT_SCRIPT, /* Used to evaluate Event Script */
	E_DOM_SET_ATTRIBUTE,	/* Used when a element attribute is set */
	E_DOM_EXECUTE_EVENT_FUNCTION, /* Used for executing event function */
	E_DOM_GET_CURRENT_ELEMENT, /* Used to get last element processed by applicaton */
	E_DOM_DELETE_OBJECT /* Used to delete the object from the object list */
} E_DOM_CALLBACK_TYPE ;

/**
 * @brief	    Prototype of BOM callback.
 *
 * @param[in]	pvArg	BOM can pass the session context in jdi_DOMInitialize.
 *				DOM pass the same argument to BOM when CB_DOM_GENERAL_CALLBACK
                callback is invoked.
 * @param[in]		eCallbackType  Callback type.
 * @param[in,out]	pvCallbackInfo Callback related info.
 * @return		The callback function can return any BOM related error.
 *
 * BOM shall implement a function with this prototype and pass its address in
 * jdi_DOMInitialize.
 */
typedef JC_RETCODE	(* CB_DOM_GENERAL_CALLBACK) (void					*pvArg,
												 E_DOM_CALLBACK_TYPE	eCallbackType,
												 void					*pvCallbackInfo) ;


JC_RETCODE jdi_DOMInitialize (
              void		                    *pvAppArg, 
              CB_DOM_GENERAL_CALLBACK       cbDOMGeneralCallback, 
              E_DOM_INITPARAM_TYPE          eParamType, 
              U_DOM_INITPARAM               uParams, 
              JC_HANDLE                     vMemHandle, 
              DOM_DOCUMENT                  *pDocument
              ) ;

/**
 * @brief Adds a range of element & text content to Live objects 
 *		  (i.e NodeList, HTMLCollection, HTMLOptionsCollection)
 *
 * @param[in]  vDocument		Document handle
 * @param[in]  pstStartTag		Start ST_XML_TAG
 * @param[in]  pstEndTag		End ST_XML_TAG
 * @retval JC_OK				Indicate Success
 * @retval JC_ERR_MEMORY_ALLOCATION Indicate memory allocation error.
 *
 * The function shall be called after the new content is added to XML tree.
 */

JC_RETCODE	jdi_DOMAddContentToLiveObjects (DOM_DOCUMENT			vDocument, 
										    ST_XML_TAG			    *pstStartTag, 
										    ST_XML_TAG			    *pstEndTag
										 ) ;

/**
 * @brief Removes a range of element & text content from Live objects 
 *		  (i.e NodeList, HTMLCollection, HTMLOptionsCollection)
 *
 * @param[in]  vDocument			Document handle
 * @param[in]  pstParentStartTag	Parent of start ST_XML_TAG. This is 
 *									required since given content is already 
 *									removed from XML tree.
 * @param[in]  pstStartTag			Start ST_XML_TAG
 * @param[in]  pstEndTag			End ST_XML_TAG
 * @retval Nil
 *
 * The function shall be called after the given content is removed from XML tree.
 */

void	jdi_DOMRemoveContentFromLiveObjects (DOM_DOCUMENT	vDocument, 
										     ST_XML_TAG		*pstParentStartTag, 
										     ST_XML_TAG		*pstStartTag,  
										     ST_XML_TAG		*pstEndTag
											) ;

JC_HANDLE   jdi_DOMGetXMLNode (DOM_NODE      vNode) ;

/**
 * @brief This function gets body element from tree
 * @param[in]  vDocument				   : Reference to DOM Document
 * @retval Reference to Body or Frameset element
 */

JC_HANDLE   jdi_DOMGetDocumentBody (JC_HANDLE		vDocument) ;

void jdi_DOMCleanup (DOM_DOCUMENT Document) ;

void jdi_DOMFreeElement (
             DOM_DOCUMENT	Document,
		     DOM_ELEMENT	Element
             ) ;
void jdi_DOMFreeAttribute (
               DOM_DOCUMENT		Document,
		       DOM_ATTRIBUTE	Attribute
               ) ;
void jdi_DOMFreeTextNode (
              DOM_DOCUMENT	Document,
			  DOM_TEXT  	Text
              ) ;
void jdi_DOMFreeCDATASection (
              DOM_DOCUMENT		Document,
			  DOM_CDATA_SECTION	CDATASection
              ) ;
void jdi_DOMFreeComment (
             DOM_DOCUMENT	    Document,
			 DOM_COMMENT	    Comment
             ) ;
void jdi_DOMFreeDocFragment (
             DOM_DOCUMENT		    Document,
			 DOM_DOCUMENT_FRAGMENT	DocFragment
             ) ;
void jdi_DOMFreeProcessingInstr (
             DOM_DOCUMENT		    Document,
			 DOM_PROCESSING_INSTR	ProcInstr
             ) ;
void jdi_DOMFreeEntity (
            DOM_DOCUMENT	Document,
			DOM_ENTITY	    Entity
            ) ;
void jdi_DOMFreeEntityRef (
           DOM_DOCUMENT		    Document,
		   DOM_ENTITY_REFERENCE	EntityRef
           ) ;
void jdi_DOMFreeNodeList (
           DOM_DOCUMENT		    Document,
           DOM_NODE_LIST        NodeList
           ) ;
void jdi_DOMFreeNodeMap (
           DOM_DOCUMENT		    Document,
           DOM_NAMED_NODE_MAP   NodeMap
           ) ;
JC_RETCODE jdi_DOMGetRootNode (
           DOM_DOCUMENT		    Document, 
           DOM_NODE             *pvRootNode
           ) ;

/* DOMImplementation interfaces */
JC_BOOLEAN jdi_DOMImplHasFeature (
             DOM_IMPLEMENTATION    DomImpl,
             JC_INT8               *pcFeature, 
             JC_INT8               *pcVersion
            ) ;

JC_RETCODE jdi_DOMImplCreateDocType (
              DOM_DOCUMENT          Document,
              DOM_IMPLEMENTATION    DomImpl,
              JC_INT8               *pcQualifiedName,
              JC_INT8               *pcPublicId, 
              JC_INT8               *pcSystemId, 
              DOM_DOCUMENT_TYPE     *pDocType,
              E_DOM_EXCEPTION       *peException
              ) ;
JC_RETCODE jdi_DOMImplCreateDocument (
              DOM_DOCUMENT          Document,
              DOM_IMPLEMENTATION    DomImpl,
              JC_CHAR               *pmNameSpaceURI,
              JC_INT8               *pcQualifiedName,
              DOM_DOCUMENT_TYPE     DocType, 
              DOM_DOCUMENT          *pNewDocument,
              E_DOM_EXCEPTION       *peException
              ) ;

/* Node interfaces */
JC_RETCODE  jdi_DOMNodeInsertBefore (
                   DOM_DOCUMENT     Document,
                   DOM_NODE         Node,
                   DOM_NODE         NewChild,
                   DOM_NODE         RefChild,
                   DOM_NODE         *pInsertedChild,
                   E_DOM_EXCEPTION  *peException
                   ) ;
JC_RETCODE  jdi_DOMNodeReplaceChild (
                   DOM_DOCUMENT     Document,
                   DOM_NODE         Node,
                   DOM_NODE         NewChild,
                   DOM_NODE         OldChild,
                   DOM_NODE         *pReplacedChild,
                   E_DOM_EXCEPTION  *peException
                   ) ;
JC_RETCODE  jdi_DOMNodeRemoveChild (
                   DOM_DOCUMENT     Document,
                   DOM_NODE         Node,
                   DOM_NODE         OldChild,
                   DOM_NODE         *pRemovedChild,
                   E_DOM_EXCEPTION  *peException
                   ) ;
JC_RETCODE  jdi_DOMNodeAppendChild (
                   DOM_DOCUMENT     Document,
                   DOM_NODE         Node,
                   DOM_NODE         NewChild,
                   DOM_NODE         *pAppendedChild,
                   E_DOM_EXCEPTION  *peException
                   ) ;
JC_BOOLEAN  jdi_DOMNodeHasChildNodes (DOM_NODE Node) ;
JC_RETCODE  jdi_DOMCloneNode (
                   DOM_DOCUMENT Document,
                   DOM_NODE     Node,
                   JC_BOOLEAN   bDeep,
                   DOM_NODE     *pClonedNode
                   ) ;
JC_RETCODE  jdi_DOMNodeNormalize (DOM_DOCUMENT Document, DOM_NODE Node) ;
JC_BOOLEAN  jdi_DOMNodeIsSupported (
                   DOM_NODE       Node,
                   JC_INT8        *pcFeature, 
                   JC_INT8        *pcVersion
                   ) ;
JC_BOOLEAN  jdi_DOMNodeHasAttributes (DOM_NODE Node) ;
const JC_INT8 * jdi_DOMGetNodeName (DOM_NODE Node) ;
const JC_CHAR * jdi_DOMGetNodeValue (
                   DOM_NODE             Node,
                   E_DOM_EXCEPTION      *peException
                   ) ;
JC_RETCODE  jdi_DOMSetNodeValue (
                   DOM_DOCUMENT         Document,
                   DOM_NODE             Node,
                   JC_CHAR              *pmValue,
                   E_DOM_EXCEPTION      *peException
                   ) ;
E_DOM_NODE_TYPE jdi_DOMGetNodeType (DOM_NODE Node) ;

JC_RETCODE jdi_DOMGetNodeParent (DOM_DOCUMENT     Document,
                                 DOM_NODE         Node, 
                                 DOM_NODE         *pParent) ;
/**
 * @brief This function gets child nodes of a node
 * @param[in]  vDocument				   : Given Document Handle
 * @param[in]  vNode					   : Given Node Handle
 * @param[out] pvChildNodes			   : Node List Handle
 * @retval JC_OK					   : Success
 * @retval Other errors
 */

JC_RETCODE jdi_DOMGetNodeChildNodes (
                     DOM_DOCUMENT       vDocument,
                     DOM_NODE			vNode, 
					 DOM_NODE_LIST		*pvChildNodes
					 ) ;

JC_RETCODE jdi_DOMGetNodeFirstChild (DOM_DOCUMENT   Document,
                                     DOM_NODE       Node, 
                                     DOM_NODE       *pFirstChild
                                     ) ;
/**
 * @brief This function gets Last Child of the given node
 * @param[in]  vDocument			: Given Document Handle
 * @param[in]  vNode				: Given Node Handle
 * @param[out] pvLastChild			: Last Node Handle
 * @retval JC_OK					: Success
 * @retval Other errors
 */

JC_RETCODE jdi_DOMGetNodeLastChild (DOM_DOCUMENT	vDocument,
									DOM_NODE		vNode, 
									DOM_NODE		*pvLastChild
									) ;
JC_RETCODE jdi_DOMGetNodePrevSibling (DOM_DOCUMENT  Document,
                                      DOM_NODE      Node, 
                                      DOM_NODE      *pPrevSibling
                                      ) ;

/**
 * @brief This function gets the Next Sibling of the given node
 * @param[in]  vDocument				   : Given Document Handle
 * @param[in]  vNode					   : Given Node Handle
 * @param[out] pvNextSibling			   : Next Sibling Handle
 * @retval JC_OK						   : Success
 * @retval Other errors
 */

JC_RETCODE jdi_DOMGetNodeNextSibling (DOM_DOCUMENT  Document,
                                      DOM_NODE      Node, 
                                      DOM_NODE      *pNextSibling
                                      ) ;
JC_RETCODE jdi_DOMGetNodeAttributes (DOM_DOCUMENT       vDocument,
                                     DOM_NODE           vNode, 
                                     DOM_NAMED_NODE_MAP *pvAttributes
                                     ) ;
const JC_CHAR * jdi_DOMGetNodeNameSpaceURI (DOM_NODE Node) ;
const JC_INT8 * jdi_DOMGetNodePrefix (DOM_NODE Node) ;
JC_RETCODE jdi_DOMSetNodePrefix (
           DOM_DOCUMENT         Document,
           DOM_NODE             Node,
           JC_INT8              *pcPrefix,
           E_DOM_EXCEPTION      *peException
           ) ;
const JC_INT8 * jdi_DOMGetNodeLocalName (DOM_NODE Node) ;
/**
 * @brief This function gets if the specified feature and version are supported or not
 * @param[in]  vDocument				: Given Document Handle
 * @param[in]  pcFeature				: Given Feature
 * @param[out] pcVersion				: Given Version
 * @retval E_TRUE if the feature is supported or else E_FALSE
 */

JC_BOOLEAN	jdi_DOMIsSupported (DOM_DOCUMENT	vDocument, 
								JC_INT8			*pcFeature, 
								JC_INT8			*pcVersion
								) ;
/* TODO add ownerdocument */

/* Document interfaces */
DOM_DOCUMENT_TYPE jdi_DOMGetDocumentType(DOM_DOCUMENT Document) ;

DOM_IMPLEMENTATION jdi_DOMGetDocImplementation (DOM_DOCUMENT Document) ;

JC_RETCODE jdi_DOMGetDocElement (DOM_DOCUMENT Document, 
                                 DOM_ELEMENT  *pDocElement) ;

JC_RETCODE jdi_DOMCreateElement (
                    DOM_DOCUMENT    Document,
                    JC_INT8         *pcTagName,
                    DOM_ELEMENT     *pElement,
                    E_DOM_EXCEPTION *peException
                    ) ;

JC_RETCODE jdi_DOMCreateDocFragment (DOM_DOCUMENT           Document,
                                     DOM_DOCUMENT_FRAGMENT  *pDocFragment) ;
JC_RETCODE jdi_DOMCreateTextNode (
                    DOM_DOCUMENT    Document,
                    JC_CHAR         *pmData,
                    DOM_TEXT        *pText
                    ) ;

/**
 * @brief This function creates and inserts new Text Node 
 * @param[in]  Document		DOM Document
 * @param[in]  vPosNode		Reference to the previous node
 * @param[in]  pmData		Text data
 * @param[out] pText		New Text Node
 * @retval returns JC_OK on success
 */
JC_RETCODE jdi_DOMInsertTextNode (DOM_DOCUMENT		vDocument,
								  DOM_ELEMENT		vParentNode,
								  DOM_ELEMENT		vPosNode,
								  JC_INT8			*pcData) ;

/**
 * @brief This function creates a comment node 
 * @param[in]  vDocument		DOM Document
 * @param[in]  pmData			Given Data
 * @param[out] pvComment		New Comment Node
 * @retval returns JC_OK on success
 */
JC_RETCODE jdi_DOMCreateComment (DOM_DOCUMENT		vDocument,
								 JC_CHAR			*pmData, 
								 DOM_COMMENT		*pvComment
								 ) ;

JC_RETCODE jdi_DOMCreateCDATASection (
                    DOM_DOCUMENT        Document,
                    JC_CHAR             *pmData,
                    DOM_CDATA_SECTION   *pCDATASection,
                    E_DOM_EXCEPTION     *peException
                    ) ;

/**
 * @brief This function creates a Processing instruction 
 * @param[in]  vDocument		DOM Document
 * @param[in]  pcTarget			Given target
 * @param[in]  pmData			Given Data
 * @param[out] pvProcInstr		New Processing instruction
 * @retval returns JC_OK on success
 */
JC_RETCODE jdi_DOMCreateProcessingInstr (DOM_DOCUMENT			vDocument,
										 JC_INT8				*pcTarget, 
										 JC_CHAR				*pmData, 
										 DOM_PROCESSING_INSTR	*pvProcInstr, 
										 E_DOM_EXCEPTION		*peException
										 ) ;
JC_RETCODE jdi_DOMCreateAttribute (
                    DOM_DOCUMENT    Document,
                    JC_INT8         *pcName,
                    DOM_ATTRIBUTE   *pAttribute,
                    E_DOM_EXCEPTION *peException
                    ) ;

JC_RETCODE jdi_DOMCreateEntityReference (
                    DOM_DOCUMENT            Document,
                    JC_INT8                 *pcName,
                    DOM_ENTITY_REFERENCE    *pEntityRef,
                    E_DOM_EXCEPTION         *peException
                    ) ;

/**
 * @brief This function gets the list of the elements with a particular Tag name
 * @param[in]  vDocument		DOM Document
 * @param[in]  pcTagName		Name of the Tag
 * @param[out] pvElements		List of Elements
 * @retval returns JC_OK on success
 */

JC_RETCODE jdi_DOMGetElementsByTagName (DOM_DOCUMENT    vDocument, 
										JC_INT8         *pcTagName, 
										DOM_NODE_LIST   *pvElements
										) ;

JC_RETCODE jdi_DOMImportNode (
                    DOM_DOCUMENT    Document,
                    DOM_NODE        NodeToImport,
                    JC_BOOLEAN      bDeep,
                    DOM_NODE        *pImportedNode,
                    E_DOM_EXCEPTION *peException
                    ) ;

JC_RETCODE jdi_DOMCreateElementNS (
                    DOM_DOCUMENT    Document,
                    JC_CHAR         *pmNameSpaceURI,
                    JC_INT8         *pcQualifiedName,
                    DOM_NODE        *pElement,
                    E_DOM_EXCEPTION *peException
                    ) ;

JC_RETCODE jdi_DOMCreateAttributeNS (
                    DOM_DOCUMENT    Document,
                    JC_CHAR         *pmNameSpaceURI,
                    JC_INT8         *pcQualifiedName,
                    DOM_ATTRIBUTE   *pAttribute,
                    E_DOM_EXCEPTION *peException
                    ) ;

JC_RETCODE jdi_DOMGetElementsByTagNameNS (
                    DOM_DOCUMENT    Document,
                    JC_CHAR         *pmNameSpaceURI,
                    JC_INT8         *pcLocalName,
                    DOM_NODE_LIST   *pElements
                    ) ;

JC_RETCODE jdi_DOMGetElementById (
                    DOM_DOCUMENT    Document,
                    JC_CHAR         *pmElementId,
                    DOM_ELEMENT     *pElement
                    ) ;

/* NodeList interfaces */
DOM_NODE jdi_DOMGetNodeListItem (
                   DOM_NODE_LIST     NodeList,
                   JC_UINT32         uiIndex
                   ) ;

JC_UINT32 jdi_DOMGetNodeListLength (DOM_NODE_LIST NodeList) ;

/**
 * @brief This function gives the owner node of a list
 * @param[in]  NodeList					NodeList Pointer
 * @param[out] pOwnerNode				Reference to the owner node
 * @retval JC_OK on success
 */
JC_RETCODE jdi_DOMNodeListGetOwnerNode (DOM_NODE_LIST		vNodeList,
										DOM_NODE			*pOwnerNode) ;

/* NamedNodeMap interfaces */
DOM_NODE  jdi_DOMGetNamedItem (
                   DOM_NAMED_NODE_MAP    NodeMap,
                   JC_INT8               *pcName
                   ) ;

JC_RETCODE jdi_DOMSetNamedItem (
                   DOM_DOCUMENT          Document,
                   DOM_NAMED_NODE_MAP    NodeMap,
                   DOM_NODE              Node,
                   DOM_NODE              *pReplacedNode,
                   E_DOM_EXCEPTION       *peException
                   ) ;
void jdi_DOMRemoveNamedItem (
                   DOM_DOCUMENT          Document, 
                   DOM_NAMED_NODE_MAP    NodeMap,
                   JC_INT8               *pcName,
                   DOM_NODE              *pRemovedNode,
                   E_DOM_EXCEPTION       *peException
                   ) ;

DOM_NODE jdi_DOMGetNamedNodeMapItem (
                   DOM_NAMED_NODE_MAP    NodeMap,
                   JC_UINT32             uiIndex
                   ) ;

JC_UINT32 jdi_DOMGetNamedNodeMapLength (DOM_NAMED_NODE_MAP NodeMap) ;

DOM_NODE  jdi_DOMGetNamedItemNS (
                   DOM_NAMED_NODE_MAP    NodeMap,
                   JC_CHAR               *pmNameSpaceURI,
                   JC_INT8               *pcLocalName
                   ) ;

JC_RETCODE jdi_DOMSetNamedItemNS (
                   DOM_DOCUMENT          Document,
                   DOM_NAMED_NODE_MAP    NodeMap,
                   DOM_NODE              Node,
                   DOM_NODE              *pSetNode,
                   E_DOM_EXCEPTION       *peException
                   ) ;
void jdi_DOMRemoveNamedItemNS (
                   DOM_NAMED_NODE_MAP    NodeMap,
                   JC_CHAR               *pmNameSpaceURI,
                   JC_INT8               *pcLocalName,
                   DOM_NODE              *pRemovedNode,
                   E_DOM_EXCEPTION       *peException
                   ) ;

/* Attribute interfaces */
const JC_INT8 * jdi_DOMGetAttrName (DOM_ATTRIBUTE Attribute) ;

JC_BOOLEAN jdi_DOMGetAttrSpecified (DOM_ATTRIBUTE Attribute) ;

const JC_CHAR * jdi_DOMGetAttrValue (DOM_ATTRIBUTE Attribute) ;

JC_RETCODE jdi_DOMSetAttrValue (
            DOM_DOCUMENT     Document,
            DOM_ATTRIBUTE    Attribute,
            JC_CHAR          *pmValue,
            E_DOM_EXCEPTION  *peException
            ) ;

JC_RETCODE jdi_DOMGetAttrOwnerElement (DOM_DOCUMENT  Document,
                                       DOM_ATTRIBUTE Attribute, 
                                       DOM_ELEMENT   *pOwnerElement) ;

/* Element interfaces */
const JC_INT8 * jdi_DOMGetEltTagName (DOM_ELEMENT Element) ;

/* ESMP V1.0 Interfaces BEGIN */
JC_RETCODE jdi_ESMPDOMSetEltAttribute (
            DOM_DOCUMENT    Document,
            DOM_ELEMENT     Element,
            JC_INT8         *pcName,
            JC_CHAR         *pmValue, 
            JC_BOOLEAN      bCaseSensitive, 
            E_DOM_EXCEPTION *peException
            ) ;

JC_RETCODE jdi_ESMPDOMRemoveEltAttribute (
            DOM_DOCUMENT    Document,
            DOM_ELEMENT     Element,
            JC_INT8         *pcName, 
            JC_BOOLEAN      bCaseSensitive, 
            E_DOM_EXCEPTION *peException
            ) ;
/* ESMP V1.0 Interfaces END */

const JC_CHAR * jdi_DOMGetEltAttribute (DOM_ELEMENT    Element, 
										JC_INT8        *pcName, 
										JC_BOOLEAN     bCaseSensitive
										) ;

/**
 * @brief This function gets the DOM Attribute Value
 * @param[in]  Element			DOM Element
 * @param[in]  pmName			Name of the attribute
 * @param[in]  bCaseSensitive	If E_TRUE case sensitive search
 * @retval const JC_CHAR * :	Value of the attribute
 */

const JC_CHAR * jdi_DOMGetUniEltAttribute (DOM_ELEMENT    Element, 
										   JC_CHAR        *pmName, 
										   JC_BOOLEAN     bCaseSensitive
										   ) ;

JC_RETCODE jdi_DOMSetEltAttribute (
            DOM_DOCUMENT    Document,
            DOM_ELEMENT     Element,
            JC_INT8         *pcName,
            JC_CHAR         *pmValue,
            E_DOM_EXCEPTION *peException
            ) ;

JC_RETCODE jdi_DOMRemoveEltAttribute (
            DOM_DOCUMENT    Document,
            DOM_ELEMENT     Element,
            JC_INT8         *pcName,
            E_DOM_EXCEPTION *peException
            ) ;

JC_RETCODE jdi_DOMEltGetAttributeNode (
            DOM_DOCUMENT     Document,
            DOM_ELEMENT      Element,
            JC_INT8          *pcName,
            DOM_ATTRIBUTE    *pAttribute
            ) ;

JC_RETCODE jdi_DOMEltSetAttributeNode (
            DOM_DOCUMENT     Document,
            DOM_ELEMENT      Element,
            DOM_ATTRIBUTE    NewAttr,
            DOM_ATTRIBUTE    *pReplacedAttr,
            E_DOM_EXCEPTION  *peException
            ) ;
JC_RETCODE jdi_DOMEltRemoveAttributeNode (
            DOM_DOCUMENT     Document,
            DOM_ELEMENT      Element,
            DOM_ATTRIBUTE    OldAttr,
            DOM_ATTRIBUTE    *pRemovedAttr,
            E_DOM_EXCEPTION  *peException
            ) ;

JC_RETCODE jdi_DOMEltGetElementsByTagName (
            DOM_DOCUMENT	Document,
            DOM_ELEMENT     Element,
            JC_INT8         *pcName,
            DOM_NODE_LIST   *pElements
            ) ;

const JC_CHAR * jdi_DOMEltGetAttributeNS (
            DOM_ELEMENT    Element,
            JC_CHAR        *pmNameSpaceURI,
            JC_INT8        *pcLocalName
            ) ;

JC_RETCODE jdi_DOMEltSetAttributeNS (
            DOM_DOCUMENT    Document,
            DOM_ELEMENT     Element,
            JC_CHAR         *pmNameSpaceURI,
            JC_INT8         *pcQualifiedName,
            JC_CHAR         *pmValue,
            E_DOM_EXCEPTION *peException
            ) ;

void jdi_DOMEltRemoveAttributeNS (
            DOM_DOCUMENT    Document,
            DOM_ELEMENT     Element,
            JC_CHAR         *pmNameSpaceURI,
            JC_INT8         *pcLocalName,
            E_DOM_EXCEPTION *peException
            ) ;

JC_RETCODE jdi_DOMEltGetAttributeNodeNS (
            DOM_DOCUMENT   Document,
            DOM_ELEMENT    Element,
            JC_CHAR        *pmNameSpaceURI,
            JC_INT8        *pcLocalName,
            DOM_ATTRIBUTE  *pAttribute
            ) ;

JC_RETCODE jdi_DOMEltSetAttributeNodeNS (
            DOM_DOCUMENT     Document,
            DOM_ELEMENT      Element,
            DOM_ATTRIBUTE    NewAttr,
            DOM_ATTRIBUTE    *pReplacedAttr,
            E_DOM_EXCEPTION  *peException
            ) ;

JC_RETCODE jdi_DOMEltGetElementsByTagNameNS (
            DOM_DOCUMENT   Document,
            DOM_ELEMENT    Element,
            JC_CHAR        *pmNameSpaceURI,
            JC_INT8        *pcLocalName,
            DOM_NODE_LIST  *pElements
            ) ;

JC_BOOLEAN jdi_DOMEltHasAttribute (
            DOM_ELEMENT    Element,
            JC_INT8        *pcName
            ) ;

/**
 * @brief This function checks if there are is attribute for en element
 * @param[in]  Element			DOM Element
 * @param[in]  pmName			Name of the attribute
 * @retval JC_BOOLEAN :  If it has that attribute then returns E_TRUE
 *							returns E_FALSE 
 */

JC_BOOLEAN jdi_DOMUniEltHasAttribute	(DOM_ELEMENT    Element, 
										 JC_CHAR        *pmName) ;

JC_BOOLEAN jdi_DOMEltHasAttributeNS (
            DOM_ELEMENT    Element,
            JC_CHAR        *pmNameSpaceURI,
            JC_INT8        *pcLocalName
            ) ;

/* Character Data interfaces */
const JC_CHAR * jdi_DOMGetCharData (
                   DOM_CHARACTER_DATA    CharData,
                   E_DOM_EXCEPTION       *peException
                   ) ;

JC_RETCODE jdi_DOMSetCharData (
                   DOM_DOCUMENT          Document,
                   DOM_CHARACTER_DATA    CharData,
                   JC_CHAR               *pmData,
                   E_DOM_EXCEPTION       *peException
                   ) ;

JC_UINT32 jdi_DOMGetCharDataLength (DOM_CHARACTER_DATA    CharData) ;

/**
 * @brief Get a substring from character data
 * @param[in]  Document 	   : Document pointer
 * @param[in]  CharData	       : Character Data node
 * @param[in]  iOffset	       : Start Offset in character data
 * @param[in]  iCount		   : Count of characters from iOffset
 * @param[out] ppmSubString	   : Newly allocated substring
 * @retval	   JC_OK		   : Indicate Success
 * @retval	   JC_ERR_MEMORY_ALLOCATION : Indicate memory allocation error
 *
 * Application shall free memory allocated for substring using jdi_MemFree
 */

JC_RETCODE jdi_DOMSubStringCharData (
                   DOM_DOCUMENT             Document,
                   DOM_CHARACTER_DATA       CharData,
                   JC_INT32                 iOffset,
                   JC_INT32                 iCount, 
                   JC_CHAR                  **ppmSubString,
                   E_DOM_EXCEPTION          *peException
                   ) ;

JC_RETCODE jdi_DOMAppendCharData (
                   DOM_DOCUMENT             Document,
                   DOM_CHARACTER_DATA       CharData,
                   JC_CHAR                  *pmData,
                   E_DOM_EXCEPTION          *peException
                   ) ;

JC_RETCODE jdi_DOMInsertCharData (
                   DOM_DOCUMENT             Document,
                   DOM_CHARACTER_DATA       CharData,
                   JC_INT32                 iOffset,
                   JC_CHAR                  *pmData, 
                   E_DOM_EXCEPTION          *peException
                   ) ;

JC_RETCODE jdi_DOMDeleteCharData (
                   DOM_DOCUMENT				Document, 
                   DOM_CHARACTER_DATA       CharData,
                   JC_INT32                 iOffset,
                   JC_INT32                 iCount, 
                   E_DOM_EXCEPTION          *peException
                   ) ;

JC_RETCODE jdi_DOMReplaceCharData (
                   DOM_DOCUMENT				Document, 
                   DOM_CHARACTER_DATA       CharData,
                   JC_INT32                 iOffset,
                   JC_INT32                 iCount, 
                   JC_CHAR                  *pmData,
                   E_DOM_EXCEPTION          *peException
                   ) ;

/* Text interfaces */
JC_RETCODE jdi_DOMTxtSplitText (
                    DOM_DOCUMENT       Document,
                    DOM_TEXT           Text,
                    JC_INT32           iOffset,
                    DOM_TEXT           *pNextText,
                    E_DOM_EXCEPTION    *peException
                    ) ;

/* Document Type interfaces */
const JC_INT8 * jdi_DOMDocTypeGetDTName (DOM_DOCUMENT_TYPE DocType) ;
DOM_NAMED_NODE_MAP jdi_DOMDocTypeGetEntities 
                (DOM_DOCUMENT_TYPE DocType) ;
DOM_NAMED_NODE_MAP jdi_DOMDocTypeGetNotations 
                (DOM_DOCUMENT_TYPE DocType) ;
const JC_INT8 * jdi_DOMDocTypeGetPublicId (DOM_DOCUMENT_TYPE DocType) ;
const JC_INT8 * jdi_DOMDocTypeGetSystemId (DOM_DOCUMENT_TYPE DocType) ;
const JC_INT8 * jdi_DOMDocTypeGetInternalSubset (DOM_DOCUMENT_TYPE DocType) ;

/* Notation interfaces */

const JC_INT8 * jdi_DOMNotationGetPublicId (DOM_NOTATION Notation) ;

const JC_INT8 * jdi_DOMNotationGetSystemId (DOM_NOTATION Notation) ;

/* Entity interfaces */
const JC_INT8 * jdi_DOMEntityGetPublicId (DOM_ENTITY Entity) ; 

const JC_INT8 * jdi_DOMEntityGetSystemId (DOM_ENTITY Entity) ;

const JC_INT8 * jdi_DOMEntityGetNotationName (DOM_ENTITY Entity) ; 

/* Processing Instruction interfaces */

const JC_INT8 * jdi_DOMProcInstrGetTarget (DOM_PROCESSING_INSTR ProcInstr) ;

const JC_CHAR * jdi_DOMProcInstrGetData (DOM_PROCESSING_INSTR ProcInstr) ;

/**
 * @brief DOM module error codes
 */
#define JC_ERR_DOM_CONTINUE			TERMINAL_ERR_CODE(DOM_MODULE, 0x01)

#ifdef __cplusplus
}
#endif

#endif
/* End of File */
