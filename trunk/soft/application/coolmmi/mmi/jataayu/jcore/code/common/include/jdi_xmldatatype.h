/***************************************************************************
 * $Id: jdi_xmldatatype.h,v 1.16 2009/08/31 06:24:35 browserdevjtbase Exp $
 * $Revision: 1.16 $
 * $Date: 2009/08/31 06:24:35 $
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
 * $Log: jdi_xmldatatype.h,v $
 * Revision 1.16  2009/08/31 06:24:35  browserdevjtbase
 * Core Update base on jB5 5.3.5.5
 *
 * Revision 1.35  2009/07/20 14:27:36  prabhatdevhtmlbrow
 * response with content type : application\xml from an ajax request
 * is considered as E_PAGE_OTHER.
 *
 * Revision 1.34  2008/07/25 13:07:47  kavithadevhtmlbrow
 * i-XHTML in Page Type
 *
 * Revision 1.33  2008/07/18 10:52:34  kavithadevhtmlbrow
 * XML Comments and PI Types in XML Tag
 *
 * Revision 1.32  2008/07/14 07:08:49  kavithadevhtmlbrow
 
 *
 * Revision 1.31  2008/06/17 13:00:13  shajudevhtmlbrow
 * New position type is handled in jdi_XMLParseNInsert
 *
 * Revision 1.30  2008/05/27 13:54:10  shajudevhtmlbrow
 * embed node type added under JCF_OPTIMIZE_ENUM
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_xmldatatype.h
 * @ingroup XML
 * @brief Defines the exposed data types for XML Parser.
 *
 * Defines various structure, enums and error codes of XML parser
 * that can be used by the application that uses this module.
 */


#ifndef JDI_XML_DATATYPE_H
#define JDI_XML_DATATYPE_H

#include <jdi_commonmodule.h>


/*** Type definitions			***/

/**
 * @brief Specifies the enumeration for the page type.
 */
typedef enum xmlPageType
{
	E_PAGE_WML1,
	E_PAGE_WML2,
	E_PAGE_XHTML,
	E_PAGE_IXHTML,
	E_PAGE_CHTML,
	E_PAGE_HTML,
	E_PAGE_OTHER
} E_XML_PAGE_TYPE ;

#ifdef JCF_OPTIMIZE_ENUM
typedef JC_UINT8	E_XML_DATA_TYPE ;

#define E_XML_DATA_TYPE_ATTRIBUTE			0
#define E_XML_DATA_TYPE_INLINE				1
#define E_XML_DATA_TYPE_OBJECT				2
#define E_XML_DATA_TYPE_PI					3
#define E_XML_DATA_TYPE_COMMENT				4

#else
/**
 * @brief Specifies the attribute data type
 */
typedef enum xmlDataType
{
	E_XML_DATA_TYPE_ATTRIBUTE,
	E_XML_DATA_TYPE_INLINE,
	E_XML_DATA_TYPE_OBJECT,
	E_XML_DATA_TYPE_PI,
	E_XML_DATA_TYPE_COMMENT
} E_XML_DATA_TYPE ;
#endif

/**
 * @brief Specifies the enumeration for the callback type for getting tag or attribute name.
 */
typedef enum xmlCBType
{
	E_XML_CB_TAG,
	E_XML_CB_ATTRIBUTE
} E_XML_CB_TYPE ;

/**
 * @brief Specifies the type of the position node used in ParseNInsert/ParseNAdd
 */
typedef enum xmlPosType
{
	E_XML_PREV_SIBLING,
	E_XML_NEXT_SIBLING,
	E_XML_PARENT_NODE
} E_XML_POS_TYPE ;

#ifdef XML_DOM_UPGRADE
#ifdef JCF_OPTIMIZE_ENUM
typedef JC_UINT8	E_DOM_ACCESS_TYPE ;
#define	E_DOM_READONLY     0
#define E_DOM_READWRITE    1

typedef JC_UINT8	E_DOM_NODE_TYPE ;
#define E_DOM_ELEMENT						1
#define E_DOM_ATTRIBUTE						2
#define E_DOM_TEXT							3
#define E_DOM_CDATA_SECTION					4
#define E_DOM_ENTITY_REFERENCE				5
#define E_DOM_ENTITY						6
#define E_DOM_PROCESSING_INSTRUCTION		7
#define E_DOM_COMMENT						8
#define E_DOM_DOCUMENT						9
#define E_DOM_DOCUMENT_TYPE					10
#define E_DOM_DOCUMENT_FRAGMENT				11
#define E_DOM_NOTATION						12
#define E_DOM_EMBED_NODE					13
#else
typedef enum domAccessType 
{
	E_DOM_READONLY     = 0,
	E_DOM_READWRITE    = 1
} E_DOM_ACCESS_TYPE ;

/* Note:- Same enum value for nodetype in DOM & XML (Don't change) */
typedef enum domNodeType 
{
    E_DOM_ELEMENT = 1,
    E_DOM_ATTRIBUTE,
    E_DOM_TEXT,
    E_DOM_CDATA_SECTION,
    E_DOM_ENTITY_REFERENCE,
    E_DOM_ENTITY,
    E_DOM_PROCESSING_INSTRUCTION,
    E_DOM_COMMENT,
    E_DOM_DOCUMENT,
    E_DOM_DOCUMENT_TYPE,
    E_DOM_DOCUMENT_FRAGMENT,
    E_DOM_NOTATION,
	E_DOM_EMBED_NODE
} E_DOM_NODE_TYPE ;
#endif

#define XML_DOM_INFORMATION \
	E_DOM_NODE_TYPE         eType ; \
	E_DOM_ACCESS_TYPE       eAccess ;

#endif /* XML_DOM_UPGRADE */

/**
 * @brief This structure defines the attributes of xml tags.
 */
typedef struct xmlAttribute
{
#ifdef XML_DOM_UPGRADE
    XML_DOM_INFORMATION
#endif /* XML_DOM_UPGRADE */	
	JC_INT8					*pcName ;
	JC_CHAR					*pmValue ;
	struct xmlAttribute		*pstNext ;
} ST_XML_ATTRIBUTE ;

/**
 * @brief This structure defines the object attribute data
 */
typedef struct xmlObject
{
	JC_UINT32				uiLength ;
	JC_UINT8				ucData[1] ;
} ST_XML_OBJECT ;

/**
 * @brief Specifies the union of type of attributes
 */
typedef union _u_attribute
{
	ST_XML_ATTRIBUTE	*pstAttrList ;
	JC_CHAR				*pmValue ;
	ST_XML_OBJECT		*pstObject ;
} U_XML_ATTRIBUTE ;

/**
 * @brief This structure stores the information about the tag.
 */
typedef struct xmlTag
{
#ifdef XML_DOM_UPGRADE
    XML_DOM_INFORMATION
#endif /* XML_DOM_UPGRADE */
	E_XML_DATA_TYPE			eDataType ;
	JC_UINT32				uiID ;
	JC_INT8					*pcName ;
	U_XML_ATTRIBUTE			uAttribute ;
	JC_HANDLE				vRefHandle ;
	struct xmlTag			*pstParent ;
	struct xmlTag			*pstPrev ;
	struct xmlTag			*pstNext ;
} ST_XML_TAG ;


/**
 * @brief This structure contains information about DTD in the page
 */
typedef struct xmlDTD
{
	JC_FLOAT		fVersion ;
	JC_INT8			*pcDefn ;
	JC_INT8			*pcURI ;
} ST_XML_DTD ;


/**
 * @brief This structure contains information about the parsed page.
 */
typedef struct xmlTree
{
	ST_XML_TAG 			*pstRoot ;
	JC_INT8				*pcEncoding ;
	JC_BOOLEAN			bIsWellFormed ;
	JC_BOOLEAN			bIsNestedTable ;
	JC_BOOLEAN			bIsBidi ;
	E_XML_PAGE_TYPE		ePageType ;
	ST_XML_DTD			stDtd ;
	JC_INT32			iErrPos ;
	JC_HANDLE			vHeapHandle ;
	JC_HANDLE			vTreeInfo ;
#ifdef XML_DEBUG_INFO
	JC_UINT32			uiChildLevel ;
#endif
} ST_XML_TREE ;

/**
 * @brief This function gets the tag or attribute name
 * @param[in]  pvArg		Callback argument
 * @param[in]  bCaseFlag	Specifies whether case compare is needed or not
 * @param[in]  eType		Callback type
 * @param[in]  pcName		Tag or Attribute name
 * @param[out]  ppcNewName	Holds the new Tag or Attribute name
 * @retval JC_OK : Success
 *
 * This callback function has to be implemented by the application. When this 
 * function is called, the application has to give either tag or attribute 
 * name based on the callback type.
 */
typedef JC_RETCODE (* CB_GET_TAG_ATTR_NAME) (void			*pvArg,
											 JC_BOOLEAN		bCaseFlag,
											 E_XML_CB_TYPE	eType,
											 const JC_INT8	*pcName,
											 JC_INT8		**ppcNewName) ;

/**
 * @brief This structure contains information about the page that is to be parsed.
 */
typedef struct xmlParam
{
	JC_UINT8				*pucBuffer ;
	JC_UINT32				uiBufferLen ;
	JC_INT8					*pcContentType ;
	JC_INT8					*pcDefEncoding ;
	JC_INT8					*pcEncoding ;
	JC_UINT32				uiBlockSize ;
	JC_UINT32				uiMaxNestedChild ;
	JC_BOOLEAN				bHasMoreData ;
	JC_BOOLEAN				bRemoveSpaceTabs ;
	JC_BOOLEAN				bIsStrictXHTML ;
	JC_BOOLEAN				bAppXMLAsXML ;	/** Set this when application/xml to be handled as XML, else default it will be handled as XHTML page */
#ifdef XML_EXTENDED_SUPPORT
	JC_BOOLEAN				bParseComment ;
#endif
	JC_HANDLE				vMemHandle ;
	CB_GET_TAG_ATTR_NAME	cbGetTagAttributeName ;
	void					*pvCBArg ;
	JC_UINT32				uiCDataTagCnt ;
	JC_INT8					**ppcCDataTags ;
} ST_XML_PARAM ;


#ifdef XML_DOM_UPGRADE

/**
 * @brief Specifies the data to be inserted in XML tree
 */
typedef struct xmlInsertParam
{
	JC_INT8				*pcEncoding ;
	JC_UINT8			*pucBuffer ;
	JC_UINT32			uiBufferLen ;
	JC_BOOLEAN			bHasMoreData ;
	JC_HANDLE			vMemHandle ;
	E_XML_POS_TYPE		ePosType ;
	ST_XML_TAG			*pstPosNode ;
} ST_XML_INSERT_PARAM ;

#endif

/**
 * @brief This structure defines data used for tag stack used internally
 */
typedef struct tagStack
{
	ST_XML_TAG			*pstParent ;
	struct tagStack		*pstNext ;
} ST_TAG_STACK ;

/**
 * @brief This structure defines the data used for parsing the xml.
 */
typedef struct xmlHeap
{
	JC_HANDLE	vTagHandle ;
	JC_HANDLE	vAttrHandle ;
	JC_HANDLE	vDMemHandle ;
} ST_XML_HEAP ;


/*** Macros				***/

#define JC_ERR_XML_OK					TERMINAL_ERR_CODE(XML_MODULE, 0x01)
#define JC_ERR_XML_CONTINUE 			TERMINAL_ERR_CODE(XML_MODULE, 0x02)
#define JC_ERR_XML_INVALID_DATA			TERMINAL_ERR_CODE(XML_MODULE, 0x03)
#ifdef XML_DOM_UPGRADE
#define JC_ERR_XML_INVALID_CHARACTER	TERMINAL_ERR_CODE(XML_MODULE, 0x04)
#endif

#endif

/* End of File */


