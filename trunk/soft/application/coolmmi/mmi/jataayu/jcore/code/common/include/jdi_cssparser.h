/***************************************************************************
 * $Id: jdi_cssparser.h,v 1.13 2009/03/16 10:50:39 browserdevjtbase Exp $
 * $Revision: 1.13 $
 * $Date: 2009/03/16 10:50:39 $
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
 * $Log: jdi_cssparser.h,v $
 * Revision 1.13  2009/03/16 10:50:39  browserdevjtbase
 * Core Updates
 *
 * Revision 1.27  2009/03/10 06:31:14  kavithadevhtmlbrow
 * jdi_CSSDeletePropertyNode - Added
 *
 * Revision 1.26  2008/12/09 09:49:48  sudhadevhtmlbrow
 * CSS optimization changes - property name
 *
 * Revision 1.25  2008/02/26 10:52:15  kavithadevhtmlbrow
 * CSS Validation updated
 *
 * Revision 1.24  2008/02/25 14:10:18  kavithadevhtmlbrow
 * CSS Validation Added
 *
 * Revision 1.23  2008/01/21 09:46:22  kavithadevhtmlbrow
 * CSS buffer from file/buffer
 *
 * Revision 1.22  2007/12/28 12:36:07  kumardevhtmlbrow
 * doxygen changes.
 *
 * Revision 1.21  2007/02/19 11:53:24  kavithadevhtmlbrow
 * Charset in CSS parser
 *
 *  
 ***************************************************************************/

#ifndef _JDI_CSSPARSER_H
#define _JDI_CSSPARSER_H

#define CSS_IMPORTANT			0xFFFFFFFF

/**
  * @brief Specifies the interface which is reference to the function which 
  * check whether passed media is supported or not
  */
typedef JC_BOOLEAN (*CB_MEDIA_SUPPORTED) (JC_INT8		*pcMedia,
										  void			*pvArg) ;

/**
  * @brief Specifies the interface which is reference to the function which 
  * form the URL based on the passed parameter
  */
typedef JC_RETCODE (*CB_FORM_URL) (void			*pvArg, 
								   JC_CHAR		*pmUrl, 
								   JC_CHAR		*pmBaseUrl, 
								   JC_CHAR		**ppmFormedUrl) ;

/** @brief Specifies the interface which is reference to the function which 
  *	gets the pointer to the property name passed */

typedef JC_INT8 *(*CB_GET_PROPERTY_NAME) (JC_INT8		 *pcName,
										  void			 *pvArg) ;


#ifdef CSS_VALIDATION_ENABLE
/**
  * @brief Specifies the interface which is reference to the function which 
  * check whether the property value is valid or not 
  */
typedef JC_RETCODE (*CB_VALIDATE_PROPERTY) (JC_INT8		*pcPropertyName,
											JC_INT8		*pcPropertyValue,
											void		*pvArg) ;
#endif

/**
  * @brief Specifies the type of CSS Selectors
  */

typedef enum cssSelectorType
{
	E_UNIVERSAL_SELECTOR,			/**< Specifies the UNIVERSAL selector */
	E_TYPE_SELECTOR,				/**< Specifies the TYPE selector */
	E_DESCENDANT_SELECTOR,			/**< Specifies the DESCENDANT selector */
	E_CHILD_SELECTOR,				/**< Specifies the CHILD selector */
	E_PSEUDO_SELECTOR,				/**< Specifies the PSEUDO selector */
	E_ADJACENT_SELECTOR,			/**< Specifies the ADJACENT selector */
	E_ATTRIBUTE_SELECTOR,			/**< Specifies the ATTRIBUTE selector */
	E_ATTRIBUTE_VALUE,				/**< Specifies the ATTRIBUTE value */
	E_SPACE_SEP_ATTRIBUTE_VALUE,	/**< Specifies the space separated ATTRIBUTE value */
	E_HYPHEN_SEP_ATTRIBUTE_VALUE,	/**< Specifies the hyphen separated ATTRIBUTE value */
	E_CLASS_SELECTOR,				/**< Specifies the CLASS selector */
	E_ID_SELECTOR,					/**< Specifies the ID selector */
	E_BEGINS_WITH_ATTRIBUTE_VALUE,	/**< Specifies the begins with ATTRIBUTE value*/
	E_ENDS_WITH_ATTRIBUTE_VALUE,	/**< Specifies the ends with ATTRIBUTE value*/
	E_END_OF_SELECTOR_LIST			/**< Specifies the end of selector list*/
} E_CSS_SELECTOR_TYPE ;


/**
 * @brief This structure contains the details of selectors property.
 * 
 */

typedef struct cssProperty
{
	JC_INT8					*pcName ;	/**< Specifies the property name*/
	JC_INT8					*pcValue ;	/**< Specifies the property value*/
	struct cssProperty		*pstNext ;	/**< Specifies the reference to the next node*/
	JC_UINT32				uiPriority ;	/**< Specifies the property's priority*/
}  ST_CSS_PROPERTY ;

/**
 * @brief This structure contains the details of selector name.
 * 
 */

typedef struct cssSelectorDetails
{
	JC_INT8						*pcName ;		/**< Specifies the selector name*/
	struct cssSelectorDetails	*pstPrev ;		/**< Specifies the reference to the previous selector node*/
	struct cssSelectorDetails	*pstNext ;		/**< Specifies the reference to the next selector details node*/
	E_CSS_SELECTOR_TYPE			eSelectorType ;	/**< Specifies the kind of selector*/
} ST_CSS_SEL_DETAILS ;

/**
 * @brief This structure contains the details of selector.
 * 
 */

typedef struct cssSelector
{
	ST_CSS_SEL_DETAILS		*pstSelDetails ;		/**< Specifies the reference to the selector details*/
	ST_CSS_PROPERTY			*pstProperty ;			/**< Specifies the reference to the property*/
	struct cssSelector		*pstNext ;				/**< Specifies the reference to the next selector node*/
	JC_INT32				iSelectorSpecificity ;	/**< Specifies the weightage of the selectors*/
	JC_BOOLEAN				bIsUTF8 ;				/**< Specifies the flag to indicate the selector is in utf8 format or not*/
} ST_CSS_SELECTOR ;

/**
 * @brief This structure contains the details of "@import"
 * URL list.
 */

typedef struct cssUrlList
{
	JC_CHAR 			*pmUrl ;	/**< Specifies the reference to the URL*/
	struct cssUrlList	*pstNext ;	/**< Specifies the reference to the next URLlist node*/
	JC_UINT32			uiPriority ;	/**< Specifies the priority of the node*/
} ST_CSS_URL_LIST ;

/**
 * @brief This structure defines the data used for grouping the css.
 */
typedef struct css_different_selectors
{
	ST_CSS_SELECTOR	*pstFirstSelector ;		/**< Specifies the list of selectors with the weightage 0 to 50*/
	ST_CSS_SELECTOR	*pstSecondSelector ;	/**< Specifies the list of selectors with the weightage 51 to 100*/
	ST_CSS_SELECTOR	*pstThridSelector ;		/**< Specifies the list of selectors with the weightage 101 to 250*/
	ST_CSS_SELECTOR	*pstFourthSelector ;	/**< Specifies the list of selectors with the weightage 251 to 500*/
	ST_CSS_SELECTOR	*pstFifthSelector ;		/**< Specifies the list of selectors with the weightage 501 to 1000*/
	ST_CSS_SELECTOR	*pstSixthSelector ;		/**< Specifies the list of selectors with the weightage 1001 to 1500*/	
} ST_CSS_DIFF_SELECTORS ;


/**
 * @brief This structure contains all parsed data
 * of CSS buffer.
 */

typedef struct cssData
{
	ST_CSS_SELECTOR			*pstSelector ;		/**< Specifies the reference to the selector list*/
	ST_CSS_URL_LIST			*pstUrlList ;		/**< Specifies the reference to the URL list*/
	ST_CSS_DIFF_SELECTORS	*pstStartSelector ;	/**< Specifies the reference to the start selector list*/
	JC_HANDLE				vHeapHandle ;		/**< Specifies the heap handle*/
	JC_HANDLE				vMemHandle ;		/**< Specifies the memory manager handle*/
} ST_CSS_DATA ;


/**
 * @brief Specifies the union of css buffer/file
 */
typedef union cssBuffer
{
	JC_INT8		*pcBuffer ;
	JC_CHAR		*pmFilename ;
} U_CSS_BUFFER ; 


/**
 * @brief This structure contains required CSS parameter
 * to parse the CSS buffer.
 */

typedef struct cssParam
{
	JC_BOOLEAN				bIsFileContent ;	/**< Specifies whether the content of the css is specified in the file. */
	U_CSS_BUFFER			uBuffer ; 			/**< Specifies the reference to the css buffer or filename */
	JC_UINT32				uiBufLen ;			/**< Specifies the css buffer length */
	JC_UINT32				uiBlockSize ;		/**< Specifies the block size*/
	JC_UINT32				uiPriority ;		/**< Specifies the priority*/
	JC_BOOLEAN				bUseBufMem ;		/**< Specifies the flag to use buffer memory*/
	JC_BOOLEAN				bStdMode ;			/**< Specifies the page is in STD mode */
	JC_INT8					*pcCharset ;		/**< Specifies whether charset conversion is already applied to the buffer is */
	JC_CHAR					*pmUrl ;			/**< Specifies the reference to the URL*/
	CB_FORM_URL				cbFormUrl ;			/**< Specifies the reference to function to form URL*/
	CB_MEDIA_SUPPORTED		cbIsMediaSupported ;/**< Specifies the reference to function to know supported media*/
	CB_GET_PROPERTY_NAME	cbGetPropertyName ; /** <Specifies the reference to function to get the property name*/
#ifdef CSS_VALIDATION_ENABLE
	CB_VALIDATE_PROPERTY	cbIsPropertyValid ;/**< Specifies the reference to function to check whether the property value is valid*/
#endif
	void	 				*pvArg ;			/**< Specifies the reference to the function argument*/
	JC_HANDLE				vMemHandle ;		/**< Specifies the memory manager handler*/
} ST_CSS_PARAM ;


/**
 * @brief Specifies the error code when parser fails to parse the content. 
 */
#define JC_ERR_CSS_PARSING_ERR 			TERMINAL_ERR_CODE(CSS_MODULE, 0x01)
/**
 * @brief Specifies the error code to indicate invalid css property. 
 */
#define JC_ERR_CSS_INVALID_PROPERTY 	TERMINAL_ERR_CODE(CSS_MODULE, 0x02)
/**
 * @brief Specifies the error code to indicate invalid css selector. 
 */
#define	JC_ERR_CSS_INVALID_SELECTOR		TERMINAL_ERR_CODE(CSS_MODULE, 0x03)
/**
 * @brief Specifies the error code when block of content is yet to parsed. 
 */
#define	JC_ERR_CSS_CONTINUE				TERMINAL_ERR_CODE(CSS_MODULE, 0x04)

/*** Function Prototypes			***/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	This function parse the CSS buffer and provide the list.
 * @param[in]		pstCssParam		Specifies the CSS data of parsing.
 * @param[in,out]	pvHandle		Holds the reference when parsing to be continued
 * @param[in,out]	pstCssData		Specifies the reference to ST_CSS_DATA with
 * previous parsed content (if any), which gets updated in this function.
 * @retval		JC_ERR_CSS_CONTINUE				Parsing of block is success
 * @retval		JC_ERR_MEMORY_ALLOCATION		Fails to provide memory block
 * @retval		JC_ERR_CSS_INVALID_PROPERTY		Invalid css property
 * @retval		JC_ERR_CSS_INVALID_SELECTOR		Invalid css selector
 * @retval		JC_OK							Parsing is complete and success
 * @retval		others							Error while parsing.
 * @note		The parameter which should not be NULL are pstCssParam, pstCssData and pstCssParam->pcBuffer.
 * 
 * This function is called when application wants to parsed the CSS Content.
 * It gets the CSS Buffer for parsing, and it stores the parsed content 
 * into the linked list, which is going to be used, to provide cascading 
 * style to the widgets. 
 */
JC_RETCODE jdi_CSSParseContent (ST_CSS_PARAM	*pstCssParam, 
								JC_HANDLE		*pvHandle,
								ST_CSS_DATA		*pstCssData) ;

/**
 * @brief	This function free all the allocated memory which is used to store the CSS content.
 * @param[in]	pstCssData		Specifies the reference to ST_CSS_DATA 
 * @note		The parameter which should not be NULL is pstCssData.
 * 
 * This function is called when application wants to free all the 
 * allocated memory which is used to store the CSS content.
 */
void jdi_CSSFreeData (ST_CSS_DATA *pstCssData) ;

/**
 * @brief To free the property and selector nodes from the CSS tree depending upon the priority passed.
 * @param[in]		uiPriority	priority of the property.
 * @param[in,out]	pstCssData	Specifies the reference to ST_CSS_DATA. 
 *
 * This function takes the uiPriority as the input and deletes the property nodes which have their priority equal
 * to uiPriority. If there are no properties left in selector node, then this function also deletes the selector 
 * node from the CSS tree.
 */
void jdi_CSSDeletePropertyNode (JC_UINT32		uiPriority,
							    ST_CSS_DATA		*pstCssData) ;



#ifdef __cplusplus
}
#endif

#endif

/* End of File */

