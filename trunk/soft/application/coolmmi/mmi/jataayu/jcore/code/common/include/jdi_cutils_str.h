/***************************************************************************
 * $Id: jdi_cutils_str.h,v 1.8 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.8 $
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
 * $Log: jdi_cutils_str.h,v $
 * Revision 1.8  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.19  2007/12/28 12:36:07  kumardevhtmlbrow
 * doxygen changes.
 *
 * Revision 1.18  2006/12/22 13:12:58  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.17  2006/12/05 13:09:10  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.16  2006/09/06 14:11:09  kumardevhtmlbrow
 * Update for handling meta cache.
 *
 * Revision 1.15  2006/05/06 10:02:03  kavithadevhtmlbrow
 * TcsStrCopyIndex
 *
 * Revision 1.14  2006/01/17 14:39:12  kumardevhtmlbrow
 * Updated the comments ...
 *
 * Revision 1.13  2005/12/22 08:36:22  kavithadevhtmlbrow
 * Index of Substring updated
 *
 * Revision 1.12  2005/11/21 12:43:50  kavithadevhtmlbrow
 * jdi_CUtilsStrCaseIndexOf
 *
 * Revision 1.11  2005/09/17 10:20:30  kavithadevhtmlbrow
 * Handle attribute style updated
 *
 * Revision 1.10  2005/09/16 06:05:25  rajivdevhtmlbrow
 * jdi_CUtilsStrRemoveSpaceTabs
 *
 * Revision 1.9  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/08/29 07:16:08  kumardevhtmlbrow
 * no message
 *
 * Revision 1.7  2005/08/17 06:22:00  kumardevhtmlbrow
 * Updated the jdd_MemAlloc to jdi_MemAlloc function.
 *
 * Revision 1.6  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/06/16 13:47:10  kumardevhtmlbrow
 * Updated the extra function and separated out the tcs and tcsstr function.
 *
 * Revision 1.4  2005/06/08 15:57:35  kumardevhtmlbrow
 * WBXML Initialize Module ...
 *
 * Revision 1.3  2005/05/18 15:36:39  kumardevhtmlbrow
 * Update
 *
 * Revision 1.2  2005/05/10 07:55:48  kumardevhtmlbrow
 * Added additional utility function
 *
 * Revision 1.1  2005/04/08 08:25:44  kumardevhtmlbrow
 * Initial Version
 *
 *  
 ***************************************************************************/

/*
 * @file jdi_cutils_str.h
 * @ingroup CommonUtils
 * @brief It contains declarations of character string related functions.
 *
 * The file contains the declarations of the character string related functions. 
 * 
 */

#ifndef _JDI_CUTILS_STR_H
#define _JDI_CUTILS_STR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	  This function identifies the first occurance of a character sub string in a character string
 * @param[in]	pcSrc Specifies the source character string in which the sub string needs to be searched
 * @param[in]   pcSubStr Specifies the sub string
 * @retval		pointer of the first occurance of the sub string in the source string.
 * @retval		JC_NULL if the sub string is not found
 *
 * The function is used to search for the first occurance of a sub string in a character string.
 * The function returns an address of a string which points to part of the source string where
 * the sub string was found.
 */
JC_INT8 * jdi_CUtilsStrCaseStr (const JC_INT8	*pcSrc, 
							    const JC_INT8	*pcSubStr) ;

/**
 * @brief	  The function returns the index at which a character is present in a character string
 * @param[in]	pcSrc Specifies the source character string in which the character needs to be searched
 * @param[in]   cChar Specifies the character to be searched.
 * @param[in]   uiStart Specifies the index from which the search should start
 * @retval		Index at which the character is present in the string
 * @note		If the value returned by the function is -1 it signifies that the character is
 *				not found in the string.
 *
 * The function searches a character in a string and returns the index value at which the character
 * was found in the string. 
 */
JC_INT32 jdi_CUtilsStrIndexOf (const JC_INT8	*pcSrc, 
							   JC_INT8			cChar,
							   JC_UINT32		uiStart) ;

/**
 * @brief	  The function returns the last occurance of a character in a character string
 * @param[in]	pcSrc Specifies the source character string in which the character needs to be searched
 * @param[in]   cChar Specifies the character to be searched.
 * @param[in]   uiBefore Specifies the index of source string upto which the search for character can be done
 * @retval		Index of the last occurance of the character in the string.
 * @note		If the value returned by the function is -1 it signifies that the character is
 *				not found in the string.
 *
 * The function searches for the last occurance of a character in a string. 
 */
JC_INT32 jdi_CUtilsStrLastIndexOf (const JC_INT8	*pcSrc,
								   JC_INT8			cChar,
								   JC_UINT32		uiBefore) ;

/**
 * @brief This function checks if a string ends with a substring.
 * @param[in]	pcSrc		Specifies the source string
 * @param[in]	pcSubStr	Specifies the Substring
 * @param[in]	iEndIndex	Specifies the end index of source string. The function should check for the substring
 *							in the source string from this index (in reverse order).
 * @retval		E_TRUE if the string ends with the given substring
 * @retval		E_FALSE if the string does not end with the given substring
 * @note		The parameters pcSrc and pcSubStr should not be NULL.
 *
 * This function is used to check if the given string ends with the given 
 * substring.
 */
JC_BOOLEAN jdi_CUtilsStrEndsWith (const JC_INT8	*pcSrc,
								  const JC_INT8	*pcSubStr,
								  JC_INT32		iEndIndex) ;

/**
 * @brief	  The function returns the last occurance of a character in a character string
 * @param[in]	pcSrc Specifies the source character string in which the character needs to be searched
 * @param[in]   cChar Specifies the character to be searched.
 * @param[in]   uiBefore Specifies the index of source string upto which the search for character can be done
 * @retval		Index of the last occurance of the character in the string.
 * @note		If the value returned by the function is -1 it signifies that the character is
 *				not found in the string.
 *
 * The function searches for the last occurance of a character in a string. 
 */
JC_BOOLEAN jdi_CUtilsStrStartsWith (const JC_INT8	*pcSrc, 
									const JC_INT8	*pcSubStr) ;

/**
 * @brief	  The function compares and checks if a particular string starts with a particular sub string.
 *			  Case insensitive comparison is done between the characters of the two strings
 * @param[in]	pcSrc Specifies the source character string in which the sub string needs to be searched
 * @param[in]   pcSubStr Specifies the sub string
 * @retval		JC_BOOLEAN value specifying whether the source string starts with the given sub string or not
 *
 * The function checks if the given sub string is present at the starting of the sources string.
 * The function also makes sure that when the two strings are compared a case insensitive comparison
 * is done.
 */
JC_BOOLEAN jdi_CUtilsStrStartsWithCase (const JC_INT8	*pcSrc, 
										const JC_INT8	*pcSubStr) ;

/**
 * @brief	  The function returns a duplicate string of the given source string
 * @param[in]   vMemHandle Specifies the memory handle used by the common utils module
 * @param[in]	pcSrc Specifies the source character string which needs to be duplicated
 * @retval		JC_INT8 character string which is a duplicate of the source string
 * @retval		JC_NULL if a memory allocation error occurs
 *
 * The function creates a duplicate string of the source string that is passed as the input
 * parameter of the function.
 */
JC_INT8* jdi_CUtilsStrDuplicate (JC_HANDLE vMemHandle, const JC_INT8 *pcStr) ;

/**
 * @brief	  The function copies a given source string with the length to be copied 
 *            specified in the form of begin and end index and returns the copied string. 
 * @param[in]   vMemHandle Specifies the memory handle used by the common utils module
 * @param[in]	pcSrc Specifies the source character string that needs to be copied
 * @param[in]	iBegin Specifies the index value of source string from which the copying has to be started
 * @param[in]	iEnd Specifies the index value of source string at which copying has to end
 * @retval		JC_INT8 character string which is formed after copying
 * @retval		JC_NULL if a memory allocation error occurs
 *
 * The function is used to copy specific part of the source string. The part to be copied is specified
 * by the begin and end index.
 */
JC_INT8* jdi_CUtilsStrCopyIndex (JC_HANDLE			vMemHandle,
								 const JC_INT8		*pcStr,
								 JC_INT32			iBegin,
								 JC_INT32			iEnd) ;

/**
 * @brief	  The function copies a given source string to a destination string. The number of
 *			  characters to be copied is specified as a input parameter.
 * @param[in]   vMemHandle Specifies the memory handle used by the common utils module
 * @param[in]	pcSrc Specifies the source character string that needs to be copied
 * @param[in]	iBegin Specifies the index value of source string from which the copying has to be started
 * @param[in]	iNumber Specifies the number of characters to be copied from source string to destination string.
 * @retval		JC_INT8 character string which is formed after copying
 * @retval		JC_NULL if a memory allocation error occurs
 *
 * The function is used to copy specific part of the source string. The copying starts from the begin
 * index of the source string and the number of characters to be copied is specified by iNumber parameter.
 */
JC_INT8* jdi_CUtilsStrCopyIndexNum (JC_HANDLE		vMemHandle,
									const JC_INT8	*pcStr,
									JC_INT32		iBegin,
									JC_INT32		iNumber) ;

/**
 * @brief	  The function is returns the index of a particular sub string in the source string.
 * @param[in]   pcBuffer Specifies the source string in which the sub string needs to be searched
 * @param[in]	pcSubStr Specifies the sub string
 * @param[in]	iStartIndex Specifies the index of the source string from which the search has to start
 * @param[in]	iLength Specifies the length of the source buffer
 * @retval		Index value at which the sub string was found in the source string
 * @note		If the value returned by the function is -1 it signifies that the character is
 *				not found in the string.
 *
 * The function returns the index of the first occurance of a sub string in a given source string.
 * The search for the sub string will be done from the iBegin Index to the length of the buffer
 */
JC_INT32 jdi_CUtilsStrIndexOfSubString (const JC_INT8	*pcBuffer, 
										const JC_INT8	*pcSubStr, 
										JC_INT32		iStartIndex, 
										JC_INT32		iLength) ;

/**
 * @brief	  The function is returns the index of a particular sub string in the source string.
 *			  Case insensitive comparison is done between the characters of the two strings
 * @param[in]   pcBuffer Specifies the source string in which the sub string needs to be searched
 * @param[in]	pcSubStr Specifies the sub string
 * @param[in]	iStartIndex Specifies the index of the source string from which the search has to start
 * @param[in]	iLength Specifies the length of the source buffer
 * @retval		Index value at which the sub string was found in the source string
 * @note		If the value returned by the function is -1 it signifies that the character is
 *				not found in the string.
 *
 * The function returns the index of the first occurance of a sub string in a given source string.
 * The search for the sub string will be done from the iBegin Index to the length of the buffer
 * Case insensitive comparison is done between the characters of the two strings
 */
JC_INT32 jdi_CUtilsStrCaseIndexOfSubString (const JC_INT8	*pcBuffer, 
											const JC_INT8	*pcSubStr, 
											JC_INT32		iStartIndex, 
											JC_INT32		iLength) ;

/**
 * @brief	  The function converts a given source string to lower case.
 * @param[in]   vMemHandle Specifies the memory handle used by the common utils module
 * @param[in]	pcStr Specifies the string to be converted to lower case
 * @param[in]	iLength Specifies the length of the source string
 * @retval		JC_INT character string which is the lower case string of the sources string
 * @note		If the value returned is JC_NULL then either the string or the length is
 *			    invalid or memory allocation error has occured.
 *
 * The function gets the sources string as the input parameter and converts it into lower case
 * and returns the value to the calling function.
 */
JC_INT8 * jdi_CUtilsStrNConvertToLower (JC_HANDLE		vMemHandle,
										const JC_INT8	*pcStr,
										JC_INT32		iLength) ;

/**
 * @brief	  The function returns the index of the first occurance of a character in a string. The
 *			  search is done in between the start and the end index.
 * @param[in]   pcSrc Source string which the character is to be searched
 * @param[in]	cChar Specifies the character to be searched
 * @param[in]	uiStart Specifies the index of the source string from which the search will start
 * @param[in]	uiEnd Specifies the index of the source string at which the search should end.
 * @retval		JC_INT32 Specifying the index at which the first occurance of the character was found
 * @note		If the value returned is -1 then it signifies that the character was not found in the string.
 *
 * The function returns the index of the first occurance of a character in a string. The character
 * will be searched between the start and end index of the source string.
 */
JC_INT32 jdi_CUtilsStrIndexOfBetween (const JC_INT8	*pcSrc,
									  JC_INT8		cChar,
									  JC_UINT32		uiStart,
									  JC_UINT32		uiEnd) ;

/**
 * @brief	  The function returns the number of occurances of a character in a string. The search
 *			  is started from the start index specified as the input parameter.
 * @param[in]   pcSrc Source string which the character is to be searched
 * @param[in]	cChar Specifies the character to be searched
 * @param[in]	iStartIndex Specifies the index of source index from which the search has to be started.
 * @retval		JC_INT32 Specifying number of occurances of the character in the string.
 *
 * The function returns the number of occurances of a character in a string. The search
 * is started from the start index specified as the input parameter.
 */
JC_INT32 jdi_CUtilsStrOccurenceCount (const JC_INT8 *pcStr,
									  JC_INT8		cSearch, 
									  JC_INT32		iStartIndex) ;

/**
 * @brief	  The function copies the source string to destination string. While copying all the
 *			  spaces, tabs and new lines are ignored. So the destination string will have the source
 *			  string without spaces, newlines and tabs.
 * @param[in]   pcSrc Source string which needs to be copied
 * @param[in]	pcDest Specifies the destination string
 * @retval		Length of the destination string
 * @note		The parameters which should not be NULL are pcSrc and pcDest
 *
 * The function removes all the spaces, tabs and new lines from the sources string and copies 
 * it to the destination string.
 */
JC_UINT32 jdi_CUtilsStrRemoveSpaceTabs (const JC_INT8	*pcSrc,
										JC_INT8			*pcDest) ;

/**
 * @brief	  The function converts a string to lower case
 * @param[in,out]   pcSrc Source string which needs to be converted to lower case
 * @retval		void
 * @note		The parameters which should not be NULL is pcSrc
 *
 * The function parses the given string character by character and converts it into lower case.
 */
void jdi_CUtilsStrToLower (JC_INT8		*pcStr) ;

/**
 * @brief	  The function trims the whitespaces that are present on the right and left sides of the source string
 * @param[in]   pcSrc Specifies the source string.
 * @retval		JC_INT character string which is the trimmed source string
 * @note		The parameters which should not be NULL is pcSrc
 *
 * The function checks for any white spaces on the right and left sides of the source string. If 
 * present the function removes them and returns the trimmed source string to the calling function.
 */
JC_INT8 * jdi_CUtilsStrTrim (JC_INT8	*pcSrc) ;

/**
 * @brief	  The function checks if the given string is made up of numerics
 * @param[in]   pcSrc Specifies the source string.
 * @retval		E_TRUE if the string contains only numberics else E_FALSE
 * @note		The parameters which should not be NULL is pcSrc
 *
 * The function checks if the given string is made up of only numerics or not.
 */
JC_BOOLEAN jdi_CUtilsStrIsNumeric (const JC_INT8	*pcStr) ;

#ifdef __cplusplus
}
#endif

#endif


