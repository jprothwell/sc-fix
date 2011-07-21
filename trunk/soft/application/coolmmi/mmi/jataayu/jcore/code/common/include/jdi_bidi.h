/***************************************************************************
 * $Id: jdi_bidi.h,v 1.7 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.7 $
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
 * $Log: jdi_bidi.h,v $
 * Revision 1.7  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.7  2007/03/06 13:15:08  shajudevhtmlbrow
 * Changes made according to the code review.
 *
 * Revision 1.6  2006/06/13 09:53:02  shajudevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2006/06/06 12:23:50  shajudevhtmlbrow
 * Changed function for checking direction type to return for european and
 * arabic number.
 *
 * Revision 1.4  2006/06/06 11:52:32  kumardevhtmlbrow
 * Updated the compiler warning
 *
 * Revision 1.3  2006/06/02 13:14:18  shajudevhtmlbrow
 * New function for checking directional type added to bidi interface
 *
 * Revision 1.2  2006/04/06 06:38:15  shajudevhtmlbrow
 * BIDI_ENABLED switch added
 *
 * Revision 1.1  2006/03/22 12:43:53  shajudevhtmlbrow
 * Initial version of the bidi implementation
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_bidi.h
 * @ingroup browser
 * @brief Provides the set of interface for the browser application to perform the
 *		  bidirectional directional formatting of a unicode text.
 * 
 * This set of interface is used for initializing bidi engine, setting/getting the 
 * attribute values which can alter the behaviour of the bidi engine, setting bidi
 * class to set the level of bidirectional formatting. This also provides an api
 * which coverts the unicode bidirectional text in logical order to visual order.
 * A bidirectional text is a mixture of left to right and right to left scripts.
 * For example the mixture of Arabic text and english comes under bidirectional text.
 * For normal unicode text it will not perform anything. There is one api to 
 * deinitialize the bidi engine which will free all the resources allocated for 
 * bidi engine.
 */

#ifndef JDI_BIDI_H
#define JDI_BIDI_H

#ifdef BIDI_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    This enumeration contains the direction type.
 */

typedef	 enum   tagBidiType
{	
	E_BIDI_NEUTRAL,		/**<Nuetral and weak type.*/
	E_BIDI_LTR,			/**<Left to right direction type*/
	E_BIDI_RTL,			/**<Right to left direction type */
	E_BIDI_EN,			/**<European number*/
	E_BIDI_AN			/**<Arabic Number*/
} E_BIDI_TYPE  ;


/**
 * @brief    This enumeration contains the orientation type.
 */

typedef	 enum   tagBidiOrientation
{	
	E_ORIENTATION_CONTEXTUAL, /**<The global orientation is set according to the
								  direction of the first strong character. */
	E_ORIENTATION_RTL,		  /**<The global orientation is set to right-to-left */
	E_ORIENTATION_LTR		  /**<The global orientation is set to left-to-right. */
} E_BIDI_ORIENTATION  ;


/**
 * @brief  This enumeration contains the override type.
 */

typedef	 enum   tagBidiOverride
{	
	E_OVERRIDE_NONE, /**<No override. */ 
	E_OVERRIDE_RTL,  /**<Right to left override. */
	E_OVERRIDE_LTR  /**<Left to right override. */
} E_BIDI_OVERRIDE_TYPE  ;


/**
 * @brief This enumeration contains the bidi attributes.
 */

typedef enum tagBidiAttrib
{	
	E_BIDI_ATTRIB_ORIENATION,/**<High level protocols can set the global page orientation.
								 It can be any value of type E_BIDI_ORIENTAION. */
	E_BIDI_ATTRIB_EN_TO_AN,	 /**<High level protocols can reset all EN(European Number) to
								 AN (Arabic Number), if this attribute have a true value. */
	E_BIDI_ATTRIB_AN_TO_EN,  /**<High level protocols can reset all AN to EN, if this
								 attribute have a true value. */
	E_BIDI_ATTRIB_OVERRIDE,  /**<High level protocol can emulate directional overriding. */
	E_BIDI_ATTRIB_SWAPPING	 /**<If true then symmetric shape swapping must be done (for
								 characters such as (){}[]<>) in right to left context. */
} E_BIDI_ATTRIBUTE ;			 


/**
 * @brief  This union contains the bidi attribute types
 */

typedef  union tagBidiAttribType 
{
	E_BIDI_ORIENTATION 	eOrientaion ; /**<Holds the orientation type for the 
										  attribute E_BIDI_ATTRIB_ORIENATION. */
	JC_BOOLEAN	 		bSet ;		  /**<Holds true/false value for the attributes
										  E_BIDI_ATTRIB_EN_TO_AN, E_BIDI_ATTRIB_AN_TO_EN,
										  and E_BIDI_ATTRIB_SWAPPING. */
	E_BIDI_OVERRIDE_TYPE	eOverride ;   /**<Holds the override type for the attribute
										  E_BIDI_ATTRIB_SWAPPING. */
} U_BIDI_ATTRIB_TYPE ;


/**
 * @brief This enumeration defines the bidirectional classes.
 */

typedef	 enum   tagBidiClass
{	
	E_BIDI_NONE,		/**<No bidirectional formatting. */
	E_BIDI_IMPLICIT,	/**<The implicit bidirectional algorithm and the directional
 
	E_BIDI_FULL			/**<The implicit bidirectional algorithm, the implicit
 
							and overriding codes are supported. */
} E_BIDI_CLASS;			 


/**
 * @brief  This structure contains input values for bidi engine interface
 *		   (jdi_BIDITransformLogicalToVisual).
 */

typedef  struct tagBidiInput 
{
	JC_CHAR		*pmLogicalText ;  /**<The text in logical order which should be 
									  processed by bidi algorithm. */
	JC_INT32	iLogicalTextLen ; /**<The length of text in logical order. */
	JC_BOOLEAN	bPosMappingLtoV ; /**<Should be true if the position mapping 
									  from logical to visual is required*/
	JC_BOOLEAN	bPosMappingVtoL ; /**<Should be true if the position mapping 
									  from visual to logical is required */
} ST_BIDI_INPUT ;


/**
 * @brief   This structure contains output values from bidi engine.
 *		   (used in jdi_BIDITransformLogicalToVisual).
 */

typedef  struct tagBidiOutPut 
{
	JC_CHAR		*pmVisualText ;		/**<The output text from bidi engine, in visual order. */
	JC_INT32	iVisualTextLen ;	/**<The length of the text in visual order. */
	JC_UINT32   *puiPositionLtoV ;  /**<Position mapping from Logical to Visual.
										This is useful in cases like, making a block of text
										to bold in logical order, it can appear in
										different sections in visual order. In such cases
										we should need to know the character mapping in
										logical and visual order. */
	JC_UINT32   *puiPositionVtoL ;	/**<Position mapping from Visual to Logical. */
} ST_BIDI_OUTPUT ;


/**
 * @brief   This funtion initializes the bidi engine.
 * @param[in]	vMemHandle		Handle to the Memory
 * @param[out]	pvBidiHandle	Handle to the Bidi Engine
 * @retval		JC_OK On success.
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @see			jdi_BIDIDeInitialize
 * @note		The parameters which should not be NULL are vMemHandle
 *				and pvBidiHandle.
 * @note		The caller function should pass pvBidiHandle with all the
 *				other bidi API.
 *
 * This API is used to initialize the bidi engine. The function gives a handle to
 * bidi engine as output. 
 */

JC_RETCODE jdi_BIDIInitialize (JC_HANDLE	vMemHandle,
						       JC_HANDLE 	*pvBidiHandle) ;


/**
 * @brief     This API deinitializes the BIDI Engine.
 * @param[in]	vBidiHandle		Handle to the Bidi Engine
 * @retval		JC_OK On success.
 * @see			jdi_BIDIInitialize
 * @note		The parameter which should not be NULL is vBidiHandle.
 *
 * This API deinitializes the BIDI Engine specified by the handle.
 * It frees all the resources used by BIDI engine.
 */

JC_RETCODE jdi_BIDIDeInitialize (JC_HANDLE  vBidiHandle) ;


/**
 * @brief JDI API converts a unicode text in logical order to visual order.
 * @param[in]	vBidiHandle	Handle to the Bidi Engine.
 * @param[in]	pstInput	This structure contains input values which includes
 *							text in logical order, its length and request flags
 *							for position mapping.	.
 * @param[out]	pbRTLContext Flag has a true value if the context is RTL. 
 * @param[out]	pstOutput	This structure contains output values which 
 *							includes text in visual order, context and position
 *							mappings.
 * @retval		JC_OK On success.
 * @retval		JC_ERR_MEMORY_ALLOCATION on memory allocation error.
 * @retval		JC_ERR_INVALID_PARAMETER on invalid parameters.
 * @retval		Other errors.
 * @see			jdi_BIDIInitialize
 * @see			jdi_BIDISetValue
 * @see			jdi_BIDIGetValue
 * @see			jdi_BIDISetBidiClass
 * @see			ST_BIDI_INPUT
 * @see			ST_BIDI_OUTPUT
 * @note		The parameter which should not be NULL are vBidiHandle,
 *				pstInput and pstOutput.
 * @note		The text in pstInput also should have a not null value.
 * @note		The caller function should free the pmVisualText, puiPositionLtoV,
 *				and puiPositionVtoL fields in pstOutput after the usage. The 
 *				user can use jdi_MemFree for that purpose with the memory handle
 *				which is passed with jdi_BIDIInitialize API.
 * @note		If the bidi operation is not performed, then pstOutput will not 
 *				contain any data. So the caller function need to assure that whether
 *				pstOutput->pmVisualText have a not null value. If it has a null value
 *				then the caller function have to use the logical text for further
 *				operation as if no bidi operation is done.
 *
 * This API converts the unicode bidirectional text in logical order to visual order
 * by applying bi-directional algorithm. This returns the context of the text which
 * can be used used to align the text and also returns position mapping of characters 
 * in logical to visual and visual to logical if the corresponding flag is set.
 * This position mapping can be used in cases like, highlighting a part text in
 * logical order by making bold or italic. In that case the highlighted area can be
 * devided to more than one section in visual order. The position mapping will help
 * in locating the different highlighted sections. After the usage the caller function
 * needs to free the output values.
 */

JC_RETCODE  jdi_BIDITransformLogicalToVisual (JC_HANDLE  		vBidiHandle,
											  ST_BIDI_INPUT		*pstInput,
											  JC_BOOLEAN		*pbRTLContext,
											  ST_BIDI_OUTPUT	*pstOutput) ;


/**
 * @brief JDI API resolves the embedding levels.
 * @param[in]	vBidiHandle	Handle to the Bidi Engine.
 * @param[in]	pstInput	This structure contains input values which includes
 *							text in logical order and its length.
 * @param[out]	pbRTLContext Flag has a true value if the context is RTL. 
 * @param[out]	ppiEmbedLevels	This is a reference to resolved embedding levels.
 *
 * @retval		JC_OK On success.
 * @retval		JC_ERR_MEMORY_ALLOCATION on memory allocation error.
 * @retval		JC_ERR_INVALID_PARAMETER on invalid parameters.
 * @retval		Other errors.
 * @see			jdi_BIDIInitialize
 * @see			jdi_BIDISetValue
 * @see			jdi_BIDIGetValue
 * @see			jdi_BIDISetBidiClass
 * @see			ST_BIDI_INPUT
 * @note		The parameter which should not be NULL are vBidiHandle,
 *				pstInput and ppiEmbedLevels.
 * @note		The text in pstInput also should have a not null value.
 * @note		The caller function should free the ppiEmbedLevels after the usage. The 
 *				user can use jdi_MemFree for that purpose with the memory handle
 *				which is passed with jdi_BIDIInitialize API.
 * @note		If the ppiEmbedLevels have a null value, then further bidi operation 
 *				should not be performed. The caller function have to treat the text 
 *				as a normal text.
 *
 * This API resolves the explicit and implicit embedding levels of the given input
 * text. This will API will do all the steps to resolving embedding levels as 
 * specified in bi-directional algorithm.
 */

JC_RETCODE  jdi_BIDIResolveEmbedLevels (JC_HANDLE  		vBidiHandle,
										ST_BIDI_INPUT	*pstInput,
										JC_BOOLEAN		*pbRTLContext, 
										JC_INT32		**ppiEmbedLevels) ;


/**
 * @brief JDI API converts a unicode text in logical order to visual order.
 * @param[in]	vBidiHandle	Handle to the Bidi Engine.
 * @param[in]	pstInput	This structure contains input values which includes
 *							text in logical order, its length and request flags
 *							for position mapping.	.
 * @param[in]	piEmbedLevels	This is the reference to embedding levels.
 * @param[out]	pstOutput	This structure contains output values which 
 *							includes text in visual order, context and position
 *							mappings.
 * @retval		JC_OK On success.
 * @retval		JC_ERR_MEMORY_ALLOCATION on memory allocation error.
 * @retval		JC_ERR_INVALID_PARAMETER on invalid parameters.
 * @retval		Other errors.
 * @see			jdi_BIDIInitialize
 * @see			jdi_BIDIResolveEmbedLevels
 * @see			ST_BIDI_INPUT
 * @see			ST_BIDI_OUTPUT
 * @note		The parameter which should not be NULL are vBidiHandle,
 *				pstInput, piEmbedLevels and pstOutput.
 * @note		The text in pstInput also should have a not null value.
 * @note		The caller function should free the pmVisualText, puiPositionLtoV,
 *				and puiPositionVtoL fields in pstOutput after the usage. The 
 *				user can use jdi_MemFree for that purpose with the memory handle
 *				which is passed with jdi_BIDIInitialize API.
 * @note		If the bidi operation is not performed, then pstOutput will not 
 *				contain any data. So the caller function need to assure that whether
 *				pstOutput->pmVisualText have a not null value. If it has a null value
 *				then the caller function have to use the logical text for further
 *				operation as if the text is a normal text.
 *
 * This API converts the unicode bidirectional text in logical order to visual order
 * by applying bi-directional algorithm. This will make use of resolved embedding level,
 * the output of jdi_BIDIResolveEmbedLevels. This returns the context of the text which
 * can be used used to align the text and also returns position mapping of characters 
 * in logical to visual and visual to logical, if the corresponding flag is set.
 * This position mapping can be used in cases like, highlighting a part text in
 * logical order by making bold or italic. In that case the highlighted area can be
 * devided to more than one section in visual order. The position mapping will help
 * in locating the different highlighted sections. After the usage the caller function
 * needs to free the output values.
 */

JC_RETCODE  jdi_BIDIDisplayReorder (JC_HANDLE  		vBidiHandle,
									ST_BIDI_INPUT	*pstInput,
									JC_INT32		*piEmbedLevels, 
									ST_BIDI_OUTPUT	*pstOutput) ;


/**
 * @brief     This API set a value to the specified bidi attribute.
 * @param[in]	vBidiHandle		Handle to the Bidi Engine 
 * @param[in]	eAttribute		Bidi Attribute 
 * @param[in]	puAttribVal		Attribute value
 * @retval		JC_OK On success.
 * @retval		JC_ERR_INVALID_PARAMETER on invalid parameters.
 * @see			jdi_BIDIInitialize
 * @see			jdi_BIDIGetValue
 * @see			E_BIDI_ATTRIBUTE
 * @see			U_BIDI_ATTRIB_TYPE
 * @note		The parameters which should not be NULL are vBidiHandle,
 *				and pstAttribVal.
 * @note		The caller fuction should need to pass the attribute value in 
 *				the appropriate field in the union variable pstAttribVal.
 *
 * This API set the value to the specified bidi attribute. This way caller 
 * function can alter the characteristic of bidi engine.   
 */

JC_RETCODE 	jdi_BIDISetValue (JC_HANDLE				vBidiHAndle, 
							  E_BIDI_ATTRIBUTE 		eAttribute,
							  U_BIDI_ATTRIB_TYPE	*puAttribVal) ;


/**
 * @brief     This API get the value of the specified bidi attribute.
 * @param[in]	vBidiHandle		Handle to the Bidi Engine
 * @param[in]	eAttribute		Bidi Attribute 
 * @param[out]	puAttribVal		Attribute value
 * @retval		JC_OK On success.
 * @retval		JC_ERR_INVALID_PARAMETER on invalid parameters.
 * @see			jdi_BIDIInitialize
 * @see			jdi_BIDISetValue
 * @see			E_BIDI_ATTRIBUTE
 * @see			U_BIDI_ATTRIB_TYPE
 * @note		The parameters which should not be NULL are vBidiHandle,
 *				and pstAttribVal.
 *  
 * This API get the value of the specified bidi attribute.
 */

JC_RETCODE 	jdi_BIDIGetValue (JC_HANDLE				vBidiHAndle,
							  E_BIDI_ATTRIBUTE 		eAttribute,
							  U_BIDI_ATTRIB_TYPE	*puAttribVal) ;


/**
 * @brief     This API set the current bidi class.
 * @param[in]	vBidiHandle		Handle to the Bidi Engine 
 * @param[in]	eBidiClass		Bidi Class.
 * @retval		JC_OK On success.
 * @see			jdi_BIDIInitialize
 * @see			E_BIDI_CLASS
 * @note		The parameter which should not be NULL is vBidiHandle
 *
 *  This API set the current bidi class, according to which bidi engine
 * will process the text. This way user can set the engine to do not perform
 * any biderectional formatting or allow only implicit formatting or do full
 * bidirectional formatting (implicit and explicit). 
 */

JC_RETCODE 	jdi_BIDISetBidiClass (JC_HANDLE		vBidiHandle,
								  E_BIDI_CLASS	eBidiClass) ;

/**
 * @brief     This API Check whether a character is a RTL type or not.
 * @param[in]	mChar		Character to check 
 * @retval		E_TRUE if the character is a RTL type. Otherwise E_FALSE
 *
 * This API checks whether the character is a RTL type or not. This API use 
 * unicode character database to identify the type. It will give a true value,
 * if the character is an arabic letter/number or RTL type or RLE or RLO.
 */

JC_BOOLEAN	jdi_BIDICheckRTLType (JC_CHAR mChar) ;

/**
 * @brief     This API Check whether a character is a RTL or LTR or other.
 * @param[in]	mChar		Character to check 
 * @retval		E_BIDI_LTR if the character is a LTR type.
 * @retval		E_BIDI_RTL if the character is a RTL type.
 * @retval		E_BIDI_EN if the character is a european number.
 * @retval		E_BIDI_AN if the character is a arabic number.
 * @retval		E_BIDI_NEUTRAL if the character is a other type.
 *
 * This API checks whether the type of the character. This API use 
 * unicode character database to identify the type. It will give a E_BIDI_RTL value,
 * if the character is an arabic letter/number or RTL type or RLE or RLO and it
 * will give E_BIDI_LTR for LTR or LRE or LRO; E_BIDI_EN  for european number,
 * E_BIDI_AN for arabic number and E_BIDI_NEUTRAL for others.
 */

E_BIDI_TYPE jdi_BIDICheckDirType (JC_CHAR mChar) ;

/**
 * @brief     This API frees the memory allocated for Output Info.
 * @param[in]	vBidiHandle	Handle to the Bidi Engine.
 * @param[in]	pstOutput	This structure contains output values which 
 *							includes text in visual order, context and position
 *							mappings.
 *
 * This API frees the memory allocated for Output info structure in
 * jdi_BIDITransformLogicalToVisual or jdi_BIDIDisplayReorder
 */

void	jdi_BIDIFreeBidiOutInfo (JC_HANDLE			vBidiHandle,
								 ST_BIDI_OUTPUT		*pstOutput) ;

/**
 * @brief     This API frees the memory allocated for embedding levels.
 * @param[in]	vBidiHandle		Handle to the Bidi Engine.
 * @param[in]	piEmbedLevels	This is the reference to embedding levels which is the
 *								output of jdi_BIDIResolveEmbedLevels.
 *
 * This API frees the memory allocated for embedding levels in 
 * jdi_BIDIResolveEmbedLevels
 */

void	jdi_BIDIFreeEmbedLevels (JC_HANDLE		vBidiHandle,
								 JC_INT32		*piEmbedLevels) ;

#ifdef __cplusplus
}
#endif
#endif /*BIDI_ENABLED*/
#endif /*End Of File*/
