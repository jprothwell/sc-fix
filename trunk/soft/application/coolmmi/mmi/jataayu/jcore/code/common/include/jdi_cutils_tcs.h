/***************************************************************************
 * $Id: jdi_cutils_tcs.h,v 1.9 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.9 $
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
 * $Log: jdi_cutils_tcs.h,v $
 * Revision 1.9  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.26  2008/01/23 13:51:56  kavithadevhtmlbrow
 * jdi_CUtilsTcsIsNumeric Added
 *
 * Revision 1.25  2007/12/28 12:36:07  kumardevhtmlbrow
 * doxygen changes.
 *
 * Revision 1.24  2007/06/01 10:00:27  kavithadevhtmlbrow
 * jdi_CUtilsTcsCaseIndexOfSubTcs Added
 *
 * Revision 1.23  2006/12/05 13:09:10  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.22  2006/11/10 05:44:14  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.21  2006/05/25 14:02:16  kumardevhtmlbrow
 * unicode trim function added.
 *
 * Revision 1.20  2006/04/18 11:33:14  kavithadevhtmlbrow
 * jdi_CUtilsTcsCaseStr Added
 *
 * Revision 1.19  2006/01/17 15:33:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.18  2005/10/03 10:04:45  rajivdevhtmlbrow
 * jdi_CUtilsTcsStartsWithCase
 *
 * Revision 1.17  2005/09/17 11:58:39  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.16  2005/09/15 13:41:16  kavithadevhtmlbrow
 * Remove space tabs
 *
 * Revision 1.15  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.14  2005/08/17 06:22:00  kumardevhtmlbrow
 * Updated the jdd_MemAlloc to jdi_MemAlloc function.
 *
 * Revision 1.13  2005/08/09 12:18:43  kavithadevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.12  2005/08/01 13:09:38  kumardevhtmlbrow
 * no message
 *
 * Revision 1.11  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.10  2005/06/16 13:47:10  kumardevhtmlbrow
 * Updated the extra function and separated out the tcs and tcsstr function.
 *
 * Revision 1.9  2005/06/08 14:53:55  kumardevhtmlbrow
 * Added additional function for jdi_CUtilsTcsStrCat
 *
 * Revision 1.8  2005/06/06 15:59:25  kumardevhtmlbrow
 * Added the utility function ..
 *
 * Revision 1.7  2005/06/04 07:59:16  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/06/01 06:56:21  kumardevhtmlbrow
 * Updated the memory utils and test program
 *
 * Revision 1.5  2005/05/18 15:36:39  kumardevhtmlbrow
 * Update
 *
 * Revision 1.4  2005/05/16 08:19:09  kumardevhtmlbrow
 * Added URI Parse function.
 *
 * Revision 1.3  2005/05/10 07:55:48  kumardevhtmlbrow
 * Added additional utility function
 *
 * Revision 1.2  2005/04/15 15:46:31  kumardevhtmlbrow
 * Added jdi_CUtilsTcsStrNCpy
 *
 * Revision 1.1  2005/04/08 08:25:44  kumardevhtmlbrow
 * Initial Version
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_cutils_tcs.h
 * @ingroup commonutils
 * @brief Provides the set of functions for manipulating unicode strings.
 * 
 * Functions for different operations such as copying an unicode string upto specified 
 * indices, getting a duplicate copy, finding the index of a given unicode character,
 * finding the no of occurrences of a unicode character, checking the existence of a 
 * substring, converting form lower to upper case and removing spaces and tabs etc.
 */

#ifndef _JDI_CUTILS_TCS_H
#define _JDI_CUTILS_TCS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function copies the portion of a string between the specified indices.
 * @param[in]	vMemHandle	Handle used in memory operations
 * @param[in]	pmStr		Specifies the source string
 * @param[in]	iBegin		Start index
 * @param[in]	iEnd		End index
 * @retval		Returns a string on success
 * @retval		NULL On memory allocation error
 * @note		The parameter pmStr should not be NULL.
 * @note		The caller function should free the returned string.
 *
 * This function is used copy a string from the given start index to 
 * the end index. It allocates memory to the new string, so the calling funcion 
 * has to free that string.
 */
JC_CHAR * jdi_CUtilsTcsCopyIndex (JC_HANDLE vMemHandle, const JC_CHAR* pmStr, JC_INT32 iBegin, JC_INT32 iEnd) ;

/**
 * @brief This function copies a given no of characters from a string.
 * @param[in]	vMemHandle	Handle used in memory operations
 * @param[in]	pmStr		Specifies the source string
 * @param[in]	iBegin		Start index
 * @param[in]	iNum		No of characters to be copied
 * @retval		Returns a string on success
 * @retval		NULL On memory allocation error
 * @note		The parameter pmStr should not be NULL.
 * @note		The caller function should free the returned string.
 *
 * This function is used to copy given no of characters from a string 
 * starting from an index. It allocates memory to the new string, so the calling 
 * funcion has to free that string.
 */
JC_CHAR * jdi_CUtilsTcsCopyIndexNum (JC_HANDLE vMemHandle, const JC_CHAR *pmStr, JC_INT32 iBegin, JC_INT32 iNum) ;

/**
 * @brief This function makes a copy of the given string.
 * @param[in]	vMemHandle	Handle used in memory operations
 * @param[in]	pmStr		Specifies the source string
 * @retval		Returns a string on success
 * @retval		NULL On memory allocation error
 * @note		The parameter pmStr should not be NULL.
 * @note		The caller function should free the returned string.
 *
 * This function is used to make a duplicate copy of a given string. 
 * This function allocates memory to the new string, so the calling function
 * has to free that string.
 */
JC_CHAR* jdi_CUtilsTcsDuplicate (JC_HANDLE vMemHandle, const JC_CHAR *pmStr) ;

/**
 * @brief This function finds the last occurrence of a character in a string.
 * @param[in]	pmSrc		Source string
 * @param[in]	mSearchVal	Character that has to be searched
 * @param[in]	iEndIndex	Specifies an index in the source string till which 
 *				the search has to be done
 * @retval		Index of the character if found
 * @retval		-1 if not found
 * @note		The parameter pmStr should not be NULL.
 *
 * This function is used to search for the last occurrence of a character in a 
 * string. This function searches the given string till the specified index only.
 */
JC_INT32 jdi_CUtilsTcsLastIndexOf (const JC_CHAR *pmSrc, JC_CHAR mSearchVal, 
								   JC_INT32 iEndIndex) ;

/**
 * @brief This function finds the first occurrence of a character in a string.
 * @param[in]	pmSrc		Source string
 * @param[in]	mSearchVal	Character that has to be searched
 * @param[in]	iStartIndex	Specifies an index in the source string from which 
 *				the search has to begin
 * @retval		Index of the character if found
 * @retval		-1 if not found
 * @note		The parameter pmStr should not be NULL.
 *
 * This function is used to search for the first occurrence of a character in a 
 * string. This function searches the given string staring from the given index only.
 */
JC_INT32 jdi_CUtilsTcsIndexOf (const JC_CHAR *pmSrc, JC_CHAR mSearchVal, 
							   JC_INT32 iStartIndex) ;

/**
 * @brief This function counts the no of occurrences of a character in a string.
 * @param[in]	pmSrc	Source string
 * @param[in]	mChar	Character that has to be searched
 * @param[in]	uiStart	Specifies an index in the source string from which 
 *				the search has to begin
 * @retval		No of occurrences of the character
 * @note		The parameter pmStr should not be NULL.
 *
 * This function is used to count the no of occurrences of a character in a 
 * string. This function searches the given string staring from the given index only.
 */
JC_UINT32 jdi_CUtilsTcsOccurenceCount (const JC_CHAR	*pmSrc,
									   JC_CHAR			mChar,
									   JC_UINT32		uiStart) ;

/**
 * @brief This function checks if a string starts with a substring.
 * @param[in]	pmStr		Specifies the source string
 * @param[in]	pmSubStr	Specifies the Substring
 * @retval		E_TRUE if the string starts with the given substring
 * @retval		E_FALSE if the string does not start with the given substring
 * @note		The parameters pmStr and pmSubStr should not be NULL.
 *
 * This function is used to check if the given string starts with the given 
 * substring.
 */
JC_BOOLEAN jdi_CUtilsTcsStartsWith (const JC_CHAR	*pmSrc, 
									const JC_CHAR	*pmSubStr) ;

/**
 * @brief This function checks if a string ends with a substring.
 * @param[in]	pmStr		Specifies the source string
 * @param[in]	pmSubStr	Specifies the Substring
 * @retval		E_TRUE if the string ends with the given substring
 * @retval		E_FALSE if the string does not end with the given substring
 * @note		The parameters pmStr and pmSubStr should not be NULL.
 *
 * This function is used to check if the given string ends with the given 
 * substring.
 */
JC_BOOLEAN jdi_CUtilsTcsEndsWith (const JC_CHAR	*pmSrc,
								  const JC_CHAR	*pmSubStr) ;

/**
 * @brief This function finds the index of a substring in a string.
 * @param[in]	pmStr		Specifies the source string
 * @param[in]	pmSubStr	Specifies the Substring
 * @param[in]	iStartIndex	Specifies an index in the source string from which 
 *				the search has to begin
 * @retval		Index of the sustring if found
 * @retval		-1 if not found
 * @note		The parameters pmStr and pmSubStr should not be NULL.
 *
 * This function is used to find the index of a substring in a given string.
 * This function searches the source string starting from the given index only.
 */
JC_INT32 jdi_CUtilsTcsIndexOfSubTcs (const JC_CHAR	*pmStr, 
									 const JC_CHAR	*pmSubStr, 
									 JC_INT32		iStartIndex) ;

/**
 * @brief This function finds the index of a substring in a string irrespective of the case.
 * @param[in]	pmStr		Specifies the source string
 * @param[in]	pmSubStr	Specifies the Substring
 * @param[in]	iStartIndex	Specifies an index in the source string from which 
 *				the search has to begin
 * @retval		Index of the sustring if found
 * @retval		-1 if not found
 * @note		The parameters pmStr and pmSubStr should not be NULL.
 *
 * This function is used to find the index of a substring in a given string 
 * irrespective of the case. This function searches the source string starting 
 * from the given index only.
 */
JC_INT32 jdi_CUtilsTcsCaseIndexOfSubTcs (const JC_CHAR	*pmStr, 
										 const JC_CHAR	*pmSubStr, 
										 JC_INT32		iStartIndex) ;

/**
 * @brief This function converts the given string to upper case.
 * @param[in,out]	pmStr	Specifies the source string
 * @retval			No return value
 * @note			The parameter pmStr should not be NULL.
 * @note			This function uses the same memory.
 *
 * This function is used to convert the given string to uppercase. This function 
 * will not create any memory, it uses the same string.
 */
void jdi_CUtilsTcsLowerToUpper (JC_CHAR *pmStr) ;

/**
 * @brief This function removes spaces, tabs, newline and carriage return 
 *		  characters from the given string.
 * @param[in]	pmStr	Specifies the source string
 * @param[out]	pmDest	Holds the new string that is formed
 * @retval		Returns the length of the new string
 * @note		The parameters pmStr, pmDest, pbIsShyCharPresent should not be NULL.
 * @note		This calling function should allocate memory for pmDest.
 *
 * This function is used to remove spaces, tabs, newline and carriage return 
 * characters from the given string. This function copies the characters from 
 * the source string to the destination string and returns the length of the 
 * new string. This function will not allocate memory.
 */
JC_UINT32 jdi_CUtilsTcsRemoveSpaceTabs (const JC_CHAR		*pmSrc,
										JC_CHAR				*pmDest) ;

/**
 * @brief This function checks irrespective of case if a string starts with a substring.
 * @param[in]	pmStr		Specifies the source string
 * @param[in]	pmSubStr	Specifies the Substring
 * @retval		E_TRUE if the string starts with the given substring
 * @retval		E_FALSE if the string does not start with the given substring
 * @note		The parameters pmStr and pmSubStr should not be NULL.
 *
 * This function is used to check if the given string starts with the given 
 * substring irrespective of case.
 */
JC_BOOLEAN jdi_CUtilsTcsStartsWithCase (const JC_CHAR	*pmSrc, 
										const JC_CHAR	*pmSubStr) ;


/**
 * @brief	  This function identifies the first occurance of a sub string in 
 *            a string irrespective of case.
 * @param[in]	pmSrc    Specifies the source character string in which the sub string needs to be searched
 * @param[in]   pmSubStr Specifies the sub string
 * @retval		pointer of the first occurance of the sub string in the source string.
 * @retval		JC_NULL if the sub string is not found
 *
 * The function is used to search for the first occurance of a sub string in a string.
 * The function returns an address of a string which points to part of the source string where
 * the sub string was found.
 */
JC_CHAR * jdi_CUtilsTcsCaseStr (const JC_CHAR	*pmSrc, 
							    const JC_CHAR	*pmSubStr) ;

/**
 * @brief		Trims the unicode string.
 * @param[in,out] pcSrc Reference to string to be trimmed
 */
JC_CHAR * jdi_CUtilsTcsTrim (JC_CHAR *pmSrc) ;

/**
 * @brief This function verifies whether the passed string is numeric or not
 * @param[in] pmStr Reference to the string
 * @return E_TRUE	If the passed string is numeric
 * @return E_FALSE	If the passed string is not numeric
 */
JC_BOOLEAN jdi_CUtilsTcsIsNumeric (const JC_CHAR	*pmStr) ;

#ifdef __cplusplus
}
#endif

#endif


