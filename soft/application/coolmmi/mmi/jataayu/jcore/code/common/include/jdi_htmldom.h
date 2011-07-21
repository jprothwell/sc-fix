/***************************************************************************
 * $Id: jdi_htmldom.h,v 1.13 2009/02/27 14:06:24 browserdevjtbase Exp $
 * $Revision: 1.13 $
 * $Date: 2009/02/27 14:06:24 $
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
 * $Log: jdi_htmldom.h,v $
 * Revision 1.13  2009/02/27 14:06:24  browserdevjtbase
 * Core Updates
 *
 * Revision 1.34  2009/02/25 14:27:15  vikramdevhtmlbrow
 * setting length and value for select element.
 *
 * Revision 1.33  2008/11/11 11:09:24  shajudevhtmlbrow
 * BOM & DOM peak memory optimization changes
 *
 * Revision 1.32  2008/10/30 14:12:49  shajudevhtmlbrow
 * changes for proper nodlist updation.
 *
 * Revision 1.31  2008/10/22 07:21:34  shajudevhtmlbrow
 * changes for updating the nodelist based on current element.
 *
 * Revision 1.30  2008/06/25 07:18:31  shajudevhtmlbrow
 * changes for bomDefaultPut and support for embeds and WRT custom
 * object changes.
 *
 * Revision 1.29  2008/06/24 11:04:41  shajudevhtmlbrow
 * chnages for x-dev issues and form array index access.
 *
 * Revision 1.28  2008/06/24 05:23:24  shajudevhtmlbrow
 * changes for x-dev issues.
 *
 * Revision 1.27  2008/06/02 07:11:48  shajudevhtmlbrow
 * code review changes for new option updation through script.
 *
 * Revision 1.26  2008/05/27 06:24:58  shajudevhtmlbrow
 * WRT custom object implementation
 *
 * Revision 1.25  2008/05/26 07:13:47  shajudevhtmlbrow
 * changes for adding/removing option through script
 *
 * Revision 1.24  2008/05/12 04:56:56  shajudevhtmlbrow
 * Code review changes.
 *
 * Revision 1.23  2008/05/08 10:58:40  shajudevhtmlbrow
 * Changes for restricting the document search up to current element.
 *
 * Revision 1.22  2008/03/18 15:15:38  shajudevhtmlbrow
 * innerText and form element get changes.
 *
 * Revision 1.21  2007/09/05 05:12:10  kavithadevhtmlbrow
 * DOM Attribute, Content type in EvaluateScript, Script Cont Status in DE
 *
 * Revision 1.20  2007/08/13 06:23:35  babithapkdevhtmlbrow
 * HTMLTableElement.create & delete caption/tHead/tFoot interfaces
 * updates DOM tree.
 * Modified HTMLTableElement.caption/tHead/tFoot set interfaces.
 *
 * Revision 1.19  2007/08/03 12:50:37  babithapkdevhtmlbrow
 * Added jdi_HTMLDomGetFirstDescendant.
 * Removed jdi_HTMLTableGettHead, jdi_HTMLTableGetCaption, jdi_HTMLTableGettHead.
 *
 * Revision 1.18  2007/07/18 11:21:45  babithapkdevhtmlbrow
 * Used NodeList interfaces for DOM 2 HTML Collections.
 * Added jdi_HTMLGetNamedItem for DOM 2 HTML Collections.
 * Removed interfaces which are wrappers for DOM 2 core interfaces.
 * Used jdi_XMLGetDescendantByNameCount instead of domHTMLGetTableRowCount.
 *
 * Revision 1.17  2007/07/17 11:30:49  babithapkdevhtmlbrow
 * Default values for Element attributes are given from BOM layer.
 * Modified jdi_HTMLDomGetNumberAttr.
 *
 * Revision 1.16  2007/07/17 09:10:49  babithapkdevhtmlbrow
 * Added jdi_HTMLGetElementIndexInParent.
 * Modified jdi_HTMLTableCellGetCellIndex.
 * Modified HTMLInputElement.size.
 *
 * Revision 1.15  2007/07/12 09:56:30  babithapkdevhtmlbrow
 * Default value of rowspan & colspan for HTMLTableCellElement is 1
 *
 * Revision 1.14  2007/07/03 08:10:47  babithapkdevhtmlbrow
 * Removed ddl.h & jcal.h from interface files
 *
 * Revision 1.13  2007/06/18 11:50:00  babithapkdevhtmlbrow
 * Modified HTMLTableRowElement.rowIndex
 *
 * Revision 1.12  2007/06/07 12:37:59  babithapkdevhtmlbrow
 * Modified Live objects (ie NodeList,HTMLCollection, HTMLOptionsCollection)
 * when content is added or removed
 *
 * Revision 1.11  2007/05/30 10:20:46  babithapkdevhtmlbrow
 * Modified get & set logic of HTMLInputElement.value
 *
 * Revision 1.10  2007/05/21 09:29:00  babithapkdevhtmlbrow
 * Live objects and event listeners structures are updated when dynamic content is added/removed.
 *
 * Revision 1.9  2007/05/14 10:41:54  babithapkdevhtmlbrow
 * Modified function names as per coding standards
 *
 * Revision 1.8  2007/04/02 07:46:27  babithapkdevhtmlbrow
 * Modified HTMLInputElement interfaces
 *
 * Revision 1.7  2007/03/30 09:27:34  babithapkdevhtmlbrow
 * Modified jdi_HTMLDocGetBody
 *
 * Revision 1.6  2007/02/19 06:18:53  babithapkdevhtmlbrow
 * Modified document.title set implementation
 *
 * Revision 1.5  2007/02/08 06:06:40  babithapkdevhtmlbrow
 * Added jdi_HTMLDOMGetForm.Removed get contentDocument interfaces.
 *
 * Revision 1.4  2007/01/17 03:43:52  babithapkdevhtmlbrow
 * jdi_HTMLDOMGetElement returns a NodeList if more than one element is present with a name attribute value, else a Node is returned.
 *
 * Revision 1.3  2007/01/16 05:24:44  babithapkdevhtmlbrow
 * HTMLInputElement::type modified
 *
 * Revision 1.2  2007/01/16 04:06:28  babithapkdevhtmlbrow
 * HTMLInputElement::defaultValue modified
 *
 * Revision 1.1  2007/01/10 11:23:55  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

#ifndef _JDI_HTMLDOM_H_
#define _JDI_HTMLDOM_H_

#include <jdi_dom.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef JC_HANDLE         HTML_COLLECTION ;
typedef JC_HANDLE         HTML_OPTIONS_COLLECTION ;
typedef JC_HANDLE         HTML_DOCUMENT ;

/* Initialize/Cleanup */
JC_RETCODE jdi_HTMLDomInitialize (
                      void		                *pvAppArg, 
                      CB_DOM_GENERAL_CALLBACK   cbDOMGeneralCallback, 
                      E_DOM_INITPARAM_TYPE      eParamType,
                      U_DOM_INITPARAM           uParams,
                      JC_HANDLE                 vMemHandle, 
                      HTML_DOCUMENT             *pDocument
                      ) ;

void jdi_HTMLDomCleanup (HTML_DOCUMENT       HtmlDocument) ;

JC_RETCODE	jdi_HTMLDomGetElement (HTML_DOCUMENT	   HtmlDocument, 
                                   DOM_ELEMENT         vElement, 
								   JC_CHAR			   *pmIdOrName, 
								   JC_HANDLE		   *pvElements, 
                                   JC_BOOLEAN          *pbMultiple
								  ) ;


JC_RETCODE jdi_HTMLDomSetBooleanAttr (HTML_DOCUMENT     HtmlDocument, 
                                      DOM_ELEMENT       Element, 
                                      JC_INT8           *pcAttr, 
                                      JC_BOOLEAN        bValue, 
                                      E_DOM_EXCEPTION   *peException
                                    ) ;

JC_INT32 jdi_HTMLDomGetNumberAttr (HTML_DOCUMENT       HtmlDocument, 
                                   DOM_ELEMENT         Element, 
                                   JC_INT8             *pcAttr, 
								   JC_INT32			   iDefAttrVal
                                  ) ;

JC_INT32 jdi_HTMLDomUniGetNumberAttr (HTML_DOCUMENT			HtmlDocument, 
									  DOM_ELEMENT			Element, 
									  JC_CHAR				*pmAttr, 
									  JC_INT32				iDefAttrVal
									  ) ;

JC_RETCODE jdi_HTMLDomSetNumberAttr (HTML_DOCUMENT       HtmlDocument, 
                                     DOM_ELEMENT         Element, 
                                     JC_INT8             *pcAttr, 
                                     JC_INT32            iAttrValue, 
                                     E_DOM_EXCEPTION     *peException
                                    ) ;

const JC_CHAR * jdi_HTMLDomGetElementContent (HTML_DOCUMENT    HtmlDocument, 
                                              DOM_ELEMENT      Element, 
                                              E_DOM_EXCEPTION  *peException
                                            ) ;

JC_RETCODE jdi_HTMLDomSetElementContent (HTML_DOCUMENT     HtmlDocument, 
                                         DOM_ELEMENT       Element, 
                                         JC_CHAR           *pmContent, 
                                         E_DOM_EXCEPTION   *peException
                                        ) ;

JC_RETCODE jdi_HTMLDomGetForm (HTML_DOCUMENT		HtmlDocument, 
                               DOM_ELEMENT		    HtmlElement, 
                               DOM_ELEMENT          *pFormElement
                              ) ;

JC_RETCODE jdi_HTMLDomGetFirstDescendant (HTML_DOCUMENT           HtmlDocument, 
 										  DOM_ELEMENT             vParentElement, 
										  JC_INT8				  *pcDescendantName, 
										  DOM_ELEMENT             *pvDescendant
										 ) ;
/**
 * @brief Returns the index of an element in its parent element.
 * @param[in]  vElement				Given Element
 * @retval Index					-1,0 or positive integer
 *
 * The index is in document tree order and is zero based.
 */

JC_INT32 jdi_HTMLGetElementIndexInParent (DOM_NODE vElement) ;


/* HTMLCollection & HTMLOptionsCollection */
DOM_NODE jdi_HTMLGetNamedItem (DOM_NODE_LIST		   vNodeList, 
                               JC_CHAR                 *pmName
                              ) ;

/* HTMLDocument */

/**
 * @brief This function gets document title
 * @param[in]  vHtmlDocument		: Reference to DOM Document
 * @retval Title string on Success
 * @retval NULL in case of Errors
 */

const JC_CHAR * jdi_HTMLDocGetTitle (HTML_DOCUMENT   vHtmlDocument) ;

/**
 * @brief This function sets document title
 * @param[in]  vHtmlDocument		: Reference to DOM Document
 * @param[in]  pmTitle				: New Title string
 * @retval JC_OK : Success
 * @retval Other Errors
 */

JC_RETCODE jdi_HTMLDocSetTitle (HTML_DOCUMENT		vHtmlDocument, 
                                JC_CHAR				*pmTitle
								) ;

/**
 * @brief This function gets body element from tree
 * @param[in]  vHtmlDocument		: Reference to DOM Document
 * @param[in]  pvBody				: Reference to Body or Frameset element
 * @retval JC_OK : Success
 * @retval Other Errors
 */

JC_RETCODE jdi_HTMLDocGetBody (JC_HANDLE		vHtmlDocument, 
                               DOM_ELEMENT		*pvBody
                               ) ;

JC_RETCODE jdi_HTMLDocSetBody (HTML_DOCUMENT    HtmlDocument, 
                               DOM_ELEMENT		vBody, 
							   E_DOM_EXCEPTION	*peException
                              ) ;

JC_RETCODE jdi_HTMLDocGetApplets (HTML_DOCUMENT      vHtmlDocument, 
                                  HTML_COLLECTION    *pvApplets
                                 ) ;

/**
 * @brief This function gets list of anchor and area tags with href attribute
 * @param[in]  vHtmlDocument		: Reference to DOM Document
 * @param[out]  pvLinks				: Reference to list of links
 * @retval JC_OK : Success
 * @retval Other Errors
 */

JC_RETCODE jdi_HTMLDocGetLinks (HTML_DOCUMENT      vHtmlDocument, 
                                HTML_COLLECTION    *pvLinks
                                ) ;

/**
 * @brief This function gets list of anchor tags with name attribute
 * @param[in]  vHtmlDocument		: Reference to DOM Document
 * @param[out]  pvAnchors			: Reference to list of anchors
 * @retval JC_OK : Success
 * @retval Other Errors
 */

JC_RETCODE jdi_HTMLDocGetAnchors (HTML_DOCUMENT      vHtmlDocument, 
                                  HTML_COLLECTION    *pvAnchors
                                 ) ;

/**
 * @brief Gives the node list of embed elements in the page
 * @param[in]  HtmlDocument		HTML Document handle
 * @param[out]  pEmbeds			Reference to list of embed elements
 * @retval JC_OK				Indicate Success
 * @retval JC_ERR_MEMORY_ALLOCATION Indicate memory allocation error.
 *
 * The function shall be called after the new content is added to XML tree.
 */
JC_RETCODE jdi_HTMLDocGetEmbeds (HTML_DOCUMENT      vHtmlDocument, 
                                 HTML_COLLECTION    *pvEmbeds) ;

JC_RETCODE jdi_HTMLDocOpen (HTML_DOCUMENT HtmlDocument) ;

JC_RETCODE jdi_HTMLDocClose (HTML_DOCUMENT HtmlDocument) ;

JC_RETCODE jdi_HTMlDocWrite (HTML_DOCUMENT HtmlDocument, JC_CHAR   *pmText) ;

JC_RETCODE jdi_HTMLDocWriteln (HTML_DOCUMENT HtmlDocument, JC_CHAR *pmText) ;

/**
 * @brief This function gets list of nodes with given name attribute value
 * @param[in]  vHtmlDocument		: Reference to DOM Document
 * @param[in]  pmNameVal			: Reference to name attribute value
 * @param[in]  pvNodeList			: Reference to nodelist of nodes with given name
 * @retval JC_OK : Success
 * @retval Other Errors
 */

JC_RETCODE jdi_HTMLDocGetElementsByName (HTML_DOCUMENT      vHtmlDocument, 
                                         JC_CHAR            *pmNameVal, 
                                         DOM_NODE_LIST      *pvNodeList
                                         ) ;

/* HTMLFormElement */

/**
 * @brief This function gets list of form controls of the given form element
 * @param[in]  vHtmlDocument		: Reference to DOM Document
 * @param[in]  vFormElement			: Reference to Form Element
 * @param[in]  pvElements			: Reference to collection of form controls
 * @retval JC_OK : Success
 * @retval Other Errors
 */

JC_RETCODE jdi_HTMLFormGetElements (HTML_DOCUMENT     vHtmlDocument, 
                                    DOM_ELEMENT       vFormElement, 
                                    HTML_COLLECTION   *pvElements
                                   ) ;

/**
 * @brief This function gets count of form controls of the given form element
 * @param[in]  vHtmlDocument		: Reference to DOM Document
 * @param[in]  vFormElement			: Reference to Form Element
 * @retval Count of form controls
 * @retval Other Errors
 */

JC_INT32 jdi_HTMLFormGetLength (HTML_DOCUMENT	vHtmlDocument, 
								DOM_ELEMENT		vFormElement) ;

/* HTMLSelectElement */
const JC_INT8 * jdi_HTMLSelectGetType (DOM_ELEMENT SelectElement) ;

const JC_CHAR * jdi_HTMLSelectGetValue (HTML_DOCUMENT           HtmlDocument,
                                        DOM_ELEMENT             SelectElement, 
                                        JC_INT32                iSelectedIndex, 
                                        E_DOM_EXCEPTION         *peException) ;

JC_RETCODE jdi_HTMLSelectSetValue (HTML_DOCUMENT		vHtmlDocument, 
                                   DOM_ELEMENT			vSelectElement, 
								   JC_CHAR				*pmValue, 
								   JC_INT32				*piIndex
								   ) ;

/**
 * @brief This function gets count of options in the given select element
 * @param[in]  vHtmlDocument		: Reference to DOM Document
 * @param[in]  vFormElement			: Reference to Select Element
 * @retval Count of options
 */

JC_UINT32 jdi_HTMLSelectGetLength (HTML_DOCUMENT	vHtmlDocument, 
								   DOM_ELEMENT		vSelectElement) ;

JC_RETCODE jdi_HTMLSelectAdd (HTML_DOCUMENT       HtmlDocument, 
                              DOM_ELEMENT         SelectElement, 
                              DOM_ELEMENT         Element, 
                              DOM_ELEMENT         BeforeElement,
                              E_DOM_EXCEPTION     *peException
                             ) ;
/**
 * @brief This function removes the option element at the given index
 * @param[in]  HtmlDocument		    Html Document pointer
 * @param[in]  SelectElement        Select Element whose option will be removed
 * @param[in]  uiIndex				Index of the option element in the list
 * @param[out] pvOption				Pointer to the removed option element
 * @param[out] peException		    DOM Exception (if any) occurred
 * @retval JC_OK                    Indicate success
 * @retval JC_ERR_MEMORY_ALLOCATION Indicate memory allocation error.
 */
JC_RETCODE jdi_HTMLSelectRemove (HTML_DOCUMENT       HtmlDocument, 
                                 DOM_ELEMENT         vSelectElement, 
                                 JC_UINT32           uiIndex,
								 DOM_ELEMENT         *pvOption,
								 E_DOM_EXCEPTION     *peException
                                ) ;

/* HTMLOptionElement */
const JC_CHAR * jdi_HTMLOptionGetValue (HTML_DOCUMENT       HtmlDocument, 
                                        DOM_ELEMENT         OptionElement, 
                                        E_DOM_EXCEPTION     *peException
                                       ) ;

/* HTMLInputElement */
const JC_CHAR * jdi_HTMLInputGetDefaultValue (HTML_DOCUMENT      HtmlDocument, 
                                              DOM_ELEMENT        InputElement
                                             ) ;

JC_BOOLEAN jdi_HTMLInputGetDefaultChecked (HTML_DOCUMENT      HtmlDocument,  
                                           DOM_ELEMENT        InputElement
                                          ) ;

JC_UINT32 jdi_HTMLInputGetSize (HTML_DOCUMENT      HtmlDocument, 
                                DOM_ELEMENT        InputElement
                               ) ;

JC_RETCODE jdi_HTMLInputSetSize (HTML_DOCUMENT      HtmlDocument, 
                                 DOM_ELEMENT        InputElement, 
                                 JC_INT32           iSize, 
                                 E_DOM_EXCEPTION    *peException
                                ) ;

JC_BOOLEAN  jdi_HTMLCanCheckInput (HTML_DOCUMENT      HtmlDocument, 
                                   DOM_ELEMENT        InputElement
                                  ) ;

JC_BOOLEAN  jdi_HTMLIsNonTextInputCtrl (HTML_DOCUMENT      HtmlDocument, 
										DOM_ELEMENT        InputElement
									   ) ;

const JC_CHAR * jdi_HTMLInputGetType (HTML_DOCUMENT     HtmlDocument, 
                                      DOM_ELEMENT       InputElement
                                     ) ;

/* HTMLButtonElement */
const JC_CHAR * jdi_HTMLButtonGetType (DOM_ELEMENT   Button) ;

const JC_CHAR * jdi_HTMLButtonGetValue (DOM_ELEMENT   Button) ;
JC_RETCODE jdi_HTMLButtonSetValue (
                          HTML_DOCUMENT             HtmlDocument, 
                          DOM_ELEMENT               Button,
                          JC_CHAR                   *pmValue, 
                          E_DOM_EXCEPTION           *peException
                         ) ;

/* HTMLTableElement */
JC_RETCODE jdi_HTMLTableSetCaption (HTML_DOCUMENT               HtmlDocument, 
                                    DOM_ELEMENT                 Table, 
                                    DOM_ELEMENT                 Caption, 
                                    E_DOM_EXCEPTION             *peException
                                   ) ;

JC_RETCODE jdi_HTMLTableSettHead (HTML_DOCUMENT             HtmlDocument, 
                                  DOM_ELEMENT               Table, 
                                  DOM_ELEMENT               tHead, 
                                  E_DOM_EXCEPTION           *peException
                                 ) ;

JC_RETCODE jdi_HTMLTableSettFoot (HTML_DOCUMENT             HtmlDocument, 
                                  DOM_ELEMENT               Table, 
                                  DOM_ELEMENT               tFoot, 
                                  E_DOM_EXCEPTION           *peException
                                 ) ;

JC_RETCODE jdi_HTMLTableCreateTHead (HTML_DOCUMENT      HtmlDocument, 
                                     DOM_ELEMENT        Table, 
                                     DOM_ELEMENT        *pTHead, 
									 JC_BOOLEAN			*pbNewlyCreated
                                    ) ;
JC_RETCODE jdi_HTMLTableDeleteTHead (HTML_DOCUMENT      HtmlDocument, 
                                     DOM_ELEMENT        Table, 
									 JC_BOOLEAN			*pbExisting
                                    ) ;

JC_RETCODE jdi_HTMLTableCreateTFoot (HTML_DOCUMENT      HtmlDocument, 
                                     DOM_ELEMENT        Table, 
                                     DOM_ELEMENT        *pTFoot, 
									 JC_BOOLEAN			*pbNewlyCreated
                                    ) ;
JC_RETCODE jdi_HTMLTableDeleteTFoot (HTML_DOCUMENT      HtmlDocument, 
                                     DOM_ELEMENT        Table, 
									 JC_BOOLEAN			*pbExisting
                                    ) ;

JC_RETCODE jdi_HTMLTableCreateCaption (HTML_DOCUMENT      HtmlDocument, 
                                       DOM_ELEMENT        Table, 
                                       DOM_ELEMENT        *pCaption, 
									   JC_BOOLEAN		  *pbNewlyCreated
                                      ) ;
JC_RETCODE jdi_HTMLTableDeleteCaption (HTML_DOCUMENT      HtmlDocument, 
                                       DOM_ELEMENT        Table, 
									   JC_BOOLEAN		  *pbExisting
                                      ) ;

/**
 * @brief This function inserts a row into the given table
 * @param[in]  vHtmlDocument	: Reference to DOM Document
 * @param[in]  vTable			: Reference to Table Element
 * @param[in]  iIndex			: Index at which row is to be introduced
 * @param[out]  pvNewRow		: Reference to inserted row
 * @param[out]  peException		: Reference to exceptions(if any)
 * @retval JC_OK : Success
 * @retval Other Errors
 */

JC_RETCODE jdi_HTMLTableInsertRow (HTML_DOCUMENT      vHtmlDocument, 
                                   DOM_ELEMENT        vTable, 
                                   JC_INT32           iIndex, 
                                   DOM_ELEMENT        *pvNewRow, 
                                   E_DOM_EXCEPTION    *peException
                                   ) ;
/**
 * @brief This function deletes a row from the given table
 * @param[in]  vHtmlDocument	: Reference to DOM Document
 * @param[in]  vTable			: Reference to Table Element
 * @param[in]  iIndex			: Index at which row is to be introduced
 * @param[out]  pvNewRow		: Reference to exceptions(if any)
 * @retval JC_OK : Success
 * @retval Other Errors
 */

JC_RETCODE jdi_HTMLTableDeleteRow (HTML_DOCUMENT      vHtmlDocument, 
                                   DOM_ELEMENT        vTable, 
                                   JC_INT32           iIndex, 
                                   E_DOM_EXCEPTION    *peException
                                   ) ;

/* HTMLTableSectionElement */
JC_RETCODE jdi_HTMLTableSectionInsertRow (
                              HTML_DOCUMENT             HtmlDocument, 
                              DOM_ELEMENT               TableSection, 
                              JC_INT32                  iIndex, 
                              DOM_ELEMENT               *pNewRow,  
                              E_DOM_EXCEPTION           *peException
                             ) ;
JC_RETCODE jdi_HTMLTableSectionDeleteRow (
                              HTML_DOCUMENT             HtmlDocument, 
                              DOM_ELEMENT               TableSection, 
                              JC_INT32                  iIndex,
                              E_DOM_EXCEPTION           *peException
                             ) ;

/* HTMLTableRowElement */

/**
 * @brief This function gets the row index for the tr element
 * @param[in]  vHtmlDocument	: Reference to DOM Document
 * @param[in]  vTableRow		: Reference to TableRow Element
 * @param[out]  piRowIndex		: Reference to index of the row in table
 * @retval JC_OK : Success
 * @retval Other Errors
 */

JC_RETCODE jdi_HTMLTableRowGetRowIndex (HTML_DOCUMENT	vHtmlDocument, 
									    DOM_ELEMENT		vTableRow, 
                                        JC_INT32        *piRowIndex
                                       ) ;

JC_RETCODE jdi_HTMLTableRowGetCells (HTML_DOCUMENT         HtmlDocument, 
                                     DOM_ELEMENT           TableRow, 
                                     HTML_COLLECTION       *pCells
                                    ) ;

JC_RETCODE jdi_HTMLTableRowInsertCell (HTML_DOCUMENT         HtmlDocument, 
                                       DOM_ELEMENT           TableRow, 
                                       JC_INT32              iIndex, 
                                       DOM_ELEMENT           *pNewCell, 
                                       E_DOM_EXCEPTION       *peException
                                      ) ;
JC_RETCODE jdi_HTMLTableRowDeleteCell (HTML_DOCUMENT         HtmlDocument, 
                                       DOM_ELEMENT           TableRow, 
                                       JC_INT32              iIndex, 
                                       E_DOM_EXCEPTION       *peException
                                      ) ;

/* HTMLTableCellElement */

/**
 * @brief Returns cell index in a Table row.
 * @param[in]  TableCell				Given cell element
 * @retval Index						-1,0 or positive integer
 *
 * The index is in document tree order and is zero based.
 */

JC_INT32 jdi_HTMLTableCellGetCellIndex (DOM_ELEMENT TableCell) ;

/**
 * @brief Gets element/NodeList contained in form element having given id or 
 *        name attribute.
 * @param[in]  HtmlDocument    : HTML Document pointer
 * @param[in]  vFormElement    : Enclosing form element 
 * @param[in]  pmIdOrName	   : Id or name attribute value
 * @param[out] pvElements	   : Element/NodeList containing elements 
 *                               having given id or name attribute value
 * @param[out] pbMultiple	   : E_TRUE if more than one element else E_FALSE.
 *                               if E_TRUE, *pvElements is a NodeList else a 
 *                               Node.
 * @retval	   JC_OK		   : Indicate Success
 * @retval	   JC_ERR_MEMORY_ALLOCATION : Indicate memory allocation error
 *
 * id attribute has more priority than name attribute when searching for an 
 * element. For form element a node or a NodeList containing elements
 * is returned based on the number of elements having given name or id attribute.
 */

JC_RETCODE	jdi_HTMLDomGetFormElement (HTML_DOCUMENT	   vHtmlDocument, 
									   DOM_ELEMENT         vFormElement, 
									   JC_CHAR			   *pmNameOrId, 
									   JC_HANDLE		   *pvElements, 
									   JC_BOOLEAN          *pbMultiple
									  ) ;

/**
 * @brief Gets the form control by index.
 * @param[in]  vHtmlDocument   : HTML Document pointer
 * @param[in]  vFormElement    : Enclosing form element 
 * @param[in]  iIndex		   : Index at which form array has to be searched
 * @param[out] pvElements	   : Form control at the particular index
 * @retval	   JC_OK		   : Indicate Success
 * @retval	   JC_ERR_MEMORY_ALLOCATION : Indicate memory allocation error
 *
 */

JC_RETCODE	jdi_HTMLGetIndexedFormControl (HTML_DOCUMENT	vHtmlDocument, 
										   DOM_ELEMENT		vFormElement, 
										   JC_INT32			iIndex, 
										   JC_HANDLE		*pvFormControl
										  ) ;

/**
 * @brief Gets the updated nodelist from the current nodelist.
 * @param[in]  vDocument	: Document pointer
 * @param[in]  vNodeList	: Given Nodelist
 * @param[out]  pvNodeList	: Updated Nodelist
 * @retval	   JC_OK		   : Indicate Success
 * @retval	   Other Errors
 */
JC_RETCODE	jdi_DOMGetUpdatedNodeList (JC_HANDLE		vDocument, 
									   JC_HANDLE		vNodeList, 
									   JC_HANDLE		*pvNodeList) ;

#ifdef __cplusplus
}
#endif

#endif  /* _JDI_HTMLDOM_H_ */

/* End of File */

