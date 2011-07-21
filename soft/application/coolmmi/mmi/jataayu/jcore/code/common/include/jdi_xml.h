/***************************************************************************
 * $Id: jdi_xml.h,v 1.20 2008/08/14 08:25:27 browserdevjtbase Exp $
 * $Revision: 1.20 $
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
 * $Log: jdi_xml.h,v $
 * Revision 1.20  2008/08/14 08:25:27  browserdevjtbase
 * Core Updates
 *
 * Revision 1.30  2008/08/11 07:43:04  pankajdevhtmlbrow
 * Compilation warnings removed.
 *
 * Revision 1.29  2008/06/25 14:28:36  shajudevhtmlbrow
 * Script features (insertAdjescentElement/Text/HTML) added
 *
 * Revision 1.28  2008/03/18 15:15:38  shajudevhtmlbrow
 * innerText and form element get changes.
 *
 * Revision 1.27  2008/03/07 08:07:53  kavithadevhtmlbrow
 * jdi_XMLUpdateRoot
 *
 * Revision 1.26  2007/09/07 11:51:00  kavithadevhtmlbrow
 * ParseNInsert - As and when received
 *
 * Revision 1.25  2007/09/05 05:12:10  kavithadevhtmlbrow
 * DOM Attribute, Content type in EvaluateScript, Script Cont Status in DE
 *
 * Revision 1.24  2007/08/03 12:32:46  babithapkdevhtmlbrow
 * Added jdi_XMLGetDescendantAtIndex
 *
 * Revision 1.23  2007/07/03 11:47:12  babithapkdevhtmlbrow
 * Removed JC_ERR_XML_NODE_NOT_FOUND.
 * NOT_FOUND_ERR DOMException is handled in DOM layer
 *
 * Revision 1.22  2007/06/29 07:30:00  babithapkdevhtmlbrow
 * Supported Node.normalize()
 *
 * Revision 1.21  2007/06/14 10:06:42  babithapkdevhtmlbrow
 * jdi_XMLRemoveAttr returns JC_OK even if given attribute is not present
 *
 * Revision 1.20  2007/06/05 10:24:05  babithapkdevhtmlbrow
 * Added jdi_XMLGetLastDescendant
 *
 * Revision 1.19  2007/06/05 09:03:21  babithapkdevhtmlbrow
 * Added jdi_XMLTagHasAttrByName
 *
 * Revision 1.18  2007/05/08 09:04:30  kavithadevhtmlbrow
 * jdi_XMLParseNInsert added and jdi_XMLParseNAdd updated
 *
 * Revision 1.17  2007/04/10 10:37:55  kavithadevhtmlbrow
 * pcEncoding handling in ParseNAdd updated
 *
 * Revision 1.16  2007/02/21 09:54:00  babithapkdevhtmlbrow
 * Modified jdi_XMLSetAttr,jdi_XMLRemoveAttr, jdi_XMLCreateTag, jdi_XMLCreateAttribute for strict xHTML pages
 *
 * Revision 1.15  2007/02/14 12:19:00  babithapkdevhtmlbrow
 * Modified jdi_XMLCloneTag
 *
 * Revision 1.14  2007/01/10 11:24:29  kavithadevhtmlbrow
 * XML DOM Upgrade Functions
 *
 * Revision 1.13  2006/12/05 13:09:10  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.12  2006/11/27 09:56:35  kumardevhtmlbrow
 * Additional function provided to set the new attribute for the specified attribute.
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_xml.h
 * @ingroup XML
 * @brief Defines the exposed funciton prototypes for XML Parser.
 */


#ifndef JDI_XML_H
#define JDI_XML_H

#include <jdi_xmldatatype.h>

#ifdef __cplusplus
extern "C" {
#endif

/*** Function Prototypes			***/

JC_RETCODE jdi_XMLMakeTree (const ST_XML_PARAM	*pstXMLParam, 
							JC_HANDLE			*pvHandle, 
							ST_XML_TREE			*pstXMLTree) ;

JC_RETCODE jdi_XMLUpdateTree (JC_HANDLE			vHandle, 
							  ST_XML_TREE		*pstXMLTree) ;

JC_RETCODE jdi_XMLFreeTree (JC_HANDLE vMemHandle, ST_XML_TREE *pstXMLTree) ;

/**
 * @brief This function updates root node from the tree to parse handle
 * @param[out] pstXMLTree XML Tree formed after parsing
 * @param[in]  vHandle    Reference to the parse handle
 * @return No return value
 */
void jdi_XMLUpdateRoot (ST_XML_TREE		*pstXMLTree,
						JC_HANDLE		vHandle) ;

JC_RETCODE jdi_XMLAddNode (JC_HANDLE			vMemHandle,
						   const ST_XML_TREE	*pstXMLTree,
						   ST_XML_TAG			*pstPosNode,
						   const ST_XML_TAG		*pstAddNode,
						   ST_XML_TAG			**ppstNewNode) ;

JC_RETCODE jdi_XMLReplaceNode (JC_HANDLE		vMemHandle,
							   ST_XML_TREE		*pstXMLTree,
							   ST_XML_TAG		*pstExistingNode,
							   const ST_XML_TAG	*pstReplaceNode,
							   ST_XML_TAG		**ppstNewNode) ;

JC_RETCODE jdi_XMLParseNAdd (JC_HANDLE				vMemHandle,
							 JC_UINT8				*pucBuffer,
							 JC_UINT32				uiBufferLen,
							 ST_XML_TAG				*pstPosNode, 
							 ST_XML_TREE			*pstXMLTree) ;

JC_BOOLEAN jdi_XMLIsChild (const ST_XML_TAG	*pstElement, 
						   const ST_XML_TAG	*pstParent) ;


/**
 * @brief This function sets the value of an attribute
 * @param[in]  pstTree			 XML Tree
 * @param[in]  pstAttribute		 Attribute
 * @param[in]  pmValue	         Attribute value
 * @retval JC_OK : Success
 * @retval Other errors
 */
JC_RETCODE   jdi_XMLSetAttrValue (const ST_XML_TREE		*pstTree,
								  ST_XML_ATTRIBUTE		*pstAttribute,
								  const JC_CHAR			*pmValue) ;

/**
 * @brief This function sets attribute for an element
 * @param[in]  pstTree			 XML Tree
 * @param[in]  pstElement		 Element
 * @param[in]  pcNameSpaceURI	 Namespace URI (JC_NULL if no namespace)
 * @param[in]  pcName	         Attribute name
 * @param[in]  pmValue	         Attribute value
 * @param[in]  bCaseSensitive	 E_TRUE indicates case sensitive search
 *                               for attribute name.
 * @param[out] ppstNewAttr		 Reference to new attribute
 * @retval JC_OK : Success
 * @retval Other errors
 */
JC_RETCODE   jdi_XMLSetAttr (ST_XML_TREE        *pstTree,
						     ST_XML_TAG         *pstElement,
                             JC_INT8            *pcNameSpaceURI,
	                         JC_INT8			*pcName ,
	                         JC_CHAR			*pmValue , 
                             JC_BOOLEAN         bCaseSensitive, 
						     ST_XML_ATTRIBUTE   **ppstNewAttr
                             ) ;
/**
 * @brief This function creates a new Attribute
 * @param[in]  pstXMLTree	    Pointer to XML Tree
 * @param[in]  pcName	        Name of the Attribute
 * @param[in]  pmValue	        Value of the Attribute
 * @param[out] ppstAttribute    Newly created Attribute 
 * @retval JC_OK : Success
 * @retval Other errors
 */

JC_RETCODE jdi_XMLCreateAttribute (ST_XML_TREE		*pstXMLTree,
								   JC_INT8			*pcName,
								   JC_CHAR			*pmValue,
								   ST_XML_ATTRIBUTE	**ppstAttribute) ;

#ifdef XML_DOM_UPGRADE

/**
 * @brief This function parses the content and inserts to the existing tree
 * @param[in]     pstXMLParam  Specifies the data to be inserted
 * @param[in,out] pvHandle	   Holds the reference when parsing to be continued
 * @param[in,out] pstXMLTree  Reference to Existing XML Tree
 * @retval JC_OK  Success on parsing and adding the content
 * @retval Others Error on parsing and adding the content
 */
JC_RETCODE jdi_XMLParseNInsert (ST_XML_INSERT_PARAM	*pstXMLInsert, 
								JC_HANDLE			*pvHandle,
								ST_XML_TREE			*pstXMLTree) ;

/**
 * @brief This function creates a new Tag
 * @param[in]  pstXMLTree	    Pointer to XML Tree
 * @param[in]  pcName	        Name of the Tag
 * @param[in]  eTagType	        ST_XML_TAG type
 * @param[out] ppstTag	        Newly created Tag
 * @retval JC_OK : Success
 * @retval Other errors
 */

JC_RETCODE jdi_XMLCreateTag (ST_XML_TREE	    *pstXMLTree,
							 JC_INT8		    *pcName, 
                             E_XML_DATA_TYPE    eTagType, 
							 ST_XML_TAG		    **ppstTag) ;

/**
 * @brief This function clones an ST_XML_TAG 
 * @param[in]  vMemHandle	    Memory handle
 * @param[in]  pstXMLTree	    Pointer to XML Tree
 * @param[in]  pstTag	        ST_XML_TAG to be cloned
 * @param[in]  bDeep	        If E_TRUE, the whole subtree is cloned 
 *                              else given ST_XML_TAG alone is cloned
 * @param[out] ppstClonedTag	Cloned ST_XML_TAG
 * @retval JC_OK : Success
 * @retval Other errors
 */

JC_RETCODE jdi_XMLCloneTag (JC_HANDLE		vMemHandle,
                            ST_XML_TREE		*pstXMLTree,
							ST_XML_TAG		*pstTag, 
							JC_BOOLEAN		bDeep, 
							ST_XML_TAG		**ppstClonedTag) ;

/**
 * @brief This function gets first child of a node
 * @param[in]  pstParentNode	    Given node
 * @retval Pointer to First child : Success
 * @retval JC_NULL				  : No Child
 */

ST_XML_TAG*	jdi_XMLGetFirstChild (ST_XML_TAG *pstParentNode) ;

/**
 * @brief This function gets last child of a node
 * @param[in]  pstParentNode		Given node
 * @retval Pointer to Last child  : Success
 * @retval JC_NULL				  : No Child
 */

ST_XML_TAG* jdi_XMLGetLastChild (ST_XML_TAG *pstParentNode) ;

/**
 * @brief This function gets previous sibling of a node
 * @param[in]  pstNode					 Given node
 * @retval Pointer to previous sibling : Success
 * @retval JC_NULL					   : No previous sibling
 */

ST_XML_TAG* jdi_XMLGetPrevSibling (ST_XML_TAG *pstNode) ;

/**
 * @brief This function gets next sibling of a node
 * @param[in]  pstNode					 Given node
 * @retval Pointer to next sibling	   : Success
 * @retval JC_NULL					   : No next sibling
 */

ST_XML_TAG* jdi_XMLGetNextSibling (ST_XML_TAG *pstNode) ;

/**
 * @brief This function retrieves owner element of an attribute
 * @param[in]  pstTree					 XML Tree
 * @param[in]  pstAttr					 Attribute
 * @retval Pointer to owner element	   : Success
 * @retval JC_NULL					   : Error
 */
ST_XML_TAG* jdi_XMLGetOwnerElement (ST_XML_TREE         *pstTree,
                                    ST_XML_ATTRIBUTE    *pstAttr) ;

/**
 * @brief This function inserts a node as child before the Reference child
 * @param[in]  pstParent		Parent Node
 * @param[in]  pstNewNode		New node to be inserted
 * @param[in]  pstRefChild		Child before which given node to be inserted
 * @param[out] ppstNewChild		Reference to new child inserted
 * @retval JC_OK : Success
 *
 * The function assumes pstRefChild is a child of pstParent
 */
JC_RETCODE jdi_XMLInsertBefore	(ST_XML_TAG   *pstParent,
								 ST_XML_TAG   *pstNewNode,
								 ST_XML_TAG   *pstRefChild,
                                 ST_XML_TAG   **ppstNewChild) ;
/**
 * @brief This function removes given child
 * @param[in]  pstParent		Parent Node
 * @param[in]  pstOldChild		Child to be removed
 * @param[in]  bDeep		    If E_TRUE,memory is freed
 * @retval JC_OK : Success
 *
 * The function assumes pstOldChild is a child of pstParent
 */
JC_RETCODE jdi_XMLRemoveChild	(ST_XML_TAG		*pstParent,
								 ST_XML_TAG		*pstOldChild,
                                 ST_XML_TAG		**ppstRemovedChild) ;

/**
 * @brief This function replaces a child with another node
 * @param[in]  pstParent		 Parent Node
 * @param[in]  pstNewNode		 New node
 * @param[in]  pstOldChild		 Child to be replaced
 * @param[out] ppstNewChild      Reference to new replaced child
 * @retval JC_OK : Success
 *
 * The function assumes pstOldChild is a child of pstParent
 */
JC_RETCODE jdi_XMLReplaceChild	(ST_XML_TAG	   *pstParent,
                                 ST_XML_TAG    *pstNewChild,
                                 ST_XML_TAG    *pstOldChild,
                                 ST_XML_TAG    **ppstReplacedChild) ;

JC_RETCODE   jdi_XMLSetAttrNode (ST_XML_TAG			*pstElement,
								 ST_XML_ATTRIBUTE	*pstNewAttr, 
								 ST_XML_ATTRIBUTE	**ppstReplacedAttr) ;


/**
 * @brief This function removes attribute of an element if present
 * @param[in]  pstTree			 XML Tree
 * @param[in]  pstElement		 Element
 * @param[in]  pcNameSpaceURI	 Namespace URI (JC_NULL if no namespace)
 * @param[in]  pcName			 Attribute Name
 * @param[in]  bCaseSensitive	 E_TRUE indicates case sensitive search
 *                               for attribute name.
 * @param[out] ppstRemovedAttr	 Removed Attribute
 * @retval JC_OK : Success		 Indicates either attribute is removed successfully
 *								 or given attribute is not present
 * @retval Other errors
 */
JC_RETCODE  jdi_XMLRemoveAttr		(ST_XML_TREE         *pstTree,
									 ST_XML_TAG          *pstElement,
									 JC_INT8		     *pcNameSpaceURI,
							         JC_INT8             *pcName, 
                                     JC_BOOLEAN          bCaseSensitive, 
                                     ST_XML_ATTRIBUTE    **ppstRemovedAttr) ;

/**
 * @brief This function sets character data of a node.
 * @param[in]  pstTree			 XML Tree
 * @param[in]  pstNode		     Given node
 * @param[in]  pcData	         Data to set (JC_NULL is valid)
 * @retval JC_OK : Success
 * @retval Other errors
 */
JC_RETCODE   jdi_XMLSetCharData (ST_XML_TREE     *pstTree,
                                 ST_XML_TAG      *pstNode,
                                 JC_CHAR         *pmData /*NULL terminated*/
                                ) ;
/**
 * @brief This function adds an ST_XML_TAG after given ST_XML_TAG
 * @param[in]  pstTag				   : Given ST_XML_TAG
 * @param[in]  pstNewTag			   : New ST_XML_TAG
 * @retval void
 */

void jdi_XMLLinkTag (ST_XML_TAG   *pstTag,  ST_XML_TAG   *pstNewTag) ; 

/**
 * @brief This function removes an ST_XML_TAG and its subchilds
 * @param[in]  pstOldTag			   : ST_XML_TAG to remove
 * @retval void
 */

void jdi_XMLUnLinkTag (ST_XML_TAG   *pstOldTag) ;

/**
 * @brief This function splits a text node into two nodes at given offset
 * @param[in]  pstTree			 XML Tree
 * @param[in]  pstNode		     Given node
 * @param[in]  uiOffset	         Offset at which node to be split
 * @param[out] ppstNewNode	     Reference to new node
 * @retval JC_OK : Success
 * @retval Other errors
 */
JC_RETCODE   jdi_XMLSplitText (ST_XML_TREE   *pstTree,
                               ST_XML_TAG    *pstNode,
                               JC_UINT32     uiOffset,
                               ST_XML_TAG    **ppstNewNode) ;

/**
 * @brief This function returns count of childs of an ST_XML_TAG
 * @param[in]  pstTag	Given ST_XML_TAG
 * @retval Count of childs
 */
JC_UINT32    jdi_XMLGetChildCount (ST_XML_TAG   *pstTag) ;

/**
 * @brief This function returns count of descendants of an ST_XML_TAG
 * @param[in]  pstTag	Given ST_XML_TAG
 * @retval Count of descendants
 */
JC_UINT32     jdi_XMLGetDescendantCount (ST_XML_TAG  *pstTag) ;

/**
 * @brief Returns count of descendants with given name of an ST_XML_TAG
 * @param[in]  pstTag	Given ST_XML_TAG
 * @param[in]  pcName	Name to be matched for
 * @retval Count of descendants
 */
JC_UINT32     jdi_XMLGetDescendantByNameCount (ST_XML_TAG  *pstTag, 
                                               JC_INT8     *pcName
                                              ) ;

/**
 * @brief Returns first matching ST_XML_TAG by given name 
 * @param[in]  pstTree	Given ST_XML_TREE
 * @param[in]  pcName	Given Tag name
 * @retval First matching tag (if any) else JC_NULL
 */
ST_XML_TAG *    jdi_XMLGetTagByName (ST_XML_TREE    *pstTree, 
                                     JC_INT8        *pcName
                                    ) ;

/**
 * @brief This function returns count of previous siblings of an ST_XML_TAG
 * @param[in]  pstTag	Given ST_XML_TAG
 * @retval Count of previous siblings
 */
JC_UINT32     jdi_XMLGetPrevSiblingCount (ST_XML_TAG *pstTag) ;

/**
 * @brief This function frees memory allocated for an ST_XML_TAG
 * @param[in]  pstXMLTree	Tree with which ST_XML_TAG is associated
 * @param[in]  pstTag	    Given ST_XML_TAG
 * @retval Nil
 */

void jdi_XMLFreeTag (ST_XML_TREE	*pstXMLTree, 
				     ST_XML_TAG		*pstTag
                    ) ;

/**
 * @brief This function frees memory allocated for an ST_XML_ATTRIBUTE
 * @param[in]  pstXMLTree	Tree with which ST_XML_ATTRIBUTE is associated
 * @param[in]  pstAttr	    Given ST_XML_ATTRIBUTE
 * @retval Nil
 * 
 * The function assumes the attribute is not associated with an element.
 */

void jdi_XMLFreeAttribute (ST_XML_TREE	        *pstXMLTree, 
				           ST_XML_ATTRIBUTE		*pstAttr
                          ) ;

/**
 * @brief This function checks whether a tag has given attribute
 * @param[in]  pstTag	    Given ST_XML_TAG
 * @param[in]  pstAttr	    Given ST_XML_ATTRIBUTE
 * @retval E_TRUE           Indicate tag has given attribute
 * @retval E_FALSE          Indicate tag does not have given attribute
 * 
 */

JC_BOOLEAN  jdi_XMLTagHasAttribute (ST_XML_TAG          *pstTag, 
                                    ST_XML_ATTRIBUTE    *pstAttr
                                   ) ;

/**
 * @brief This function checks whether a tag has attribute of given name
 * @param[in]  pstTag	    Given ST_XML_TAG
 * @param[in]  pcAttrName	Given Attribute name
 * @retval E_TRUE           Indicate tag has given attribute
 * @retval E_FALSE          Indicate tag does not have given attribute
 * 
 */

JC_BOOLEAN  jdi_XMLTagHasAttrByName (ST_XML_TAG          *pstTag, 
									 JC_INT8			 *pcAttrName
									) ;

/**
 * @brief This function checks whether a tag has given attribute name & value
 * @param[in]  pstTag	    Given ST_XML_TAG
 * @param[in]  pcName	    Given attribute name
 * @param[in]  pmValue	    Given attribute value
 * @retval E_TRUE           Indicate tag has given attribute
 * @retval E_FALSE          Indicate tag does not have given attribute
 * 
 */

JC_BOOLEAN  jdi_XMLTagHasAttrNameValue (ST_XML_TAG          *pstTag, 
										JC_INT8				*pcName, 
										JC_CHAR				*pmValue
									   ) ;

/**
 * @brief This function returns last descendant of a parent ST_XML_TAG
 * @param[in]  pstParent	Given parent
 * @retval Last descendant if present, else returns parent itself
 */

ST_XML_TAG *   jdi_XMLGetLastDescendant (ST_XML_TAG		*pstParent) ;

/**
 * @brief This function normalizes an ST_XML_TAG
 * @param[in]  pstXMLTree           Given ST_XML_TREE
 * @param[in]  pstTag	            Given ST_XML_TAG
 * @retval JC_OK                    Indicates success
 * @retval JC_ERR_MEMORY_ALLOCATION Indicates memory allocation error
 * 
 */

JC_RETCODE  jdi_XMLNormalize (ST_XML_TREE           *pstTree, 
                              ST_XML_TAG            *pstTag
                              ) ;

/**
 * @brief This function gets descendant of given name at given index
 * @param[in]  pstParent            Given ST_XML_TAG
 * @param[in]  pcDescName	        Given descendant name
 * @param[in]  uiIndex	            Given index (zero based)
 * @retval Matching descendant at given index else JC_NULL
 */

ST_XML_TAG * jdi_XMLGetDescendantAtIndex (ST_XML_TAG    *pstParent, 
                                          JC_INT8       *pcDescName, 
                                          JC_UINT32     uiIndex
                                         ) ;

/**
 * @brief This function removes all childs of a given node
 * @param[in]  pstXMLTree           Given ST_XML_TREE
 * @param[in]  pstParent            Given ST_XML_TAG
 * @retval void
 */

void jdi_XMLRemoveAllChilds (ST_XML_TREE      *pstXMLTree, 
							 ST_XML_TAG       *pstParent) ;

/**
 * @brief This function creates an in-line tag with the given value 
 *		  and inserts it as a child of the Element given
 * @param[in]  pstXMLTree           Given ST_XML_TREE
 * @param[in]  pstParent            Given ST_XML_TAG
 * @param[in]  pstParent            Given ST_XML_TAG
 * @retval JC_OK                    Indicates success
 * @retval JC_ERR_MEMORY_ALLOCATION Indicates memory allocation error
 *
 */

JC_RETCODE jdi_XMLInsertInlineTag (ST_XML_TREE      *pstXMLTree, 
								   ST_XML_TAG       *pstParent,
								   ST_XML_TAG       *pstRefTag,
								   JC_CHAR			*pmValue) ;

#endif

#ifdef __cplusplus
}
#endif

#endif

/* End of File */


