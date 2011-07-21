/***************************************************************************
 * $Id: jdi_cutils_tcsstr.h,v 1.6 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.6 $
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
 * $Log: jdi_cutils_tcsstr.h,v $
 * Revision 1.6  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.15  2006/05/06 10:02:03  kavithadevhtmlbrow
 * TcsStrCopyIndex
 *
 * Revision 1.14  2006/04/19 07:24:01  kavithadevhtmlbrow
 * Comments Updated
 *
 * Revision 1.13  2006/04/18 11:33:38  kavithadevhtmlbrow
 * jdi_CUtilsTcsUTF8CaseCmp Added
 *
 * Revision 1.12  2006/03/20 05:04:26  kavithadevhtmlbrow
 * TcsStrSplToLower
 *
 * Revision 1.11  2006/01/17 15:33:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.10  2005/09/27 06:48:44  kavithadevhtmlbrow
 * UTF8 Handling
 *
 * Revision 1.9  2005/09/26 07:34:58  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2005/09/16 12:03:30  kavithadevhtmlbrow
 * TcsStrSplCmp Function added
 *
 * Revision 1.7  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/08/29 07:16:08  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/08/17 06:22:00  kumardevhtmlbrow
 * Updated the jdd_MemAlloc to jdi_MemAlloc function.
 *
 * Revision 1.4  2005/08/05 04:55:32  kavithadevhtmlbrow
 * jdi_CUtilsStrTcsNCpy Added
 *
 * Revision 1.3  2005/07/18 12:51:10  kavithadevhtmlbrow
 * Added ends with function
 *
 * Revision 1.2  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.1  2005/06/16 13:47:10  kumardevhtmlbrow
 * Updated the extra function and separated out the tcs and tcsstr function.
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_cutils_tcsstr.h
 * @ingroup commonutils
 * @brief Provides the set of functions for converting unicode strings to other type
 * of strings and vice versa.
 * 
 * Functions for different operations such as converting a character string to a
 * unicode string and the reverse operations, comparison of a unicode string with 
 * a character string, checking if a unicode string starts or ends with a character 
 * string, and finding the index of a character string in a unicode string and some 
 * operations between UTF8 and unicode strings.
 */

#ifndef _JDI_CUTILS_TCSSTR_H
#define _JDI_CUTILS_TCSSTR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function converts character string to unicode string.
 * @param[in]	pcSrc		Specifies the source string
 * @param[in]	uiDestSize	Size of the destination string
 * @param[out]	pmDest		Holds the unicode string that is formed
 * @retval		JC_OK On success
 * @retval		JC_ERR_INVALID_PARAMETER One or more of the input parameters are not valid
 * @note		The parameters pcSrc and pmDest should not be NULL.
 * @note		The caller function should allocate memory for the destination string.
 *
 * This function is used convert a character string to a unicode string.
 * The caller function has to allocate memory for the destination string.
 */
JC_RETCODE jdi_CUtilsCharToTcharMem (const JC_INT8		*pcSrc, 
									 JC_UINT32			uiDestSize,
									 JC_CHAR			*pmDest) ;

/**
 * @brief This function converts specified no of characters in a 
 *		  character string to unicode string.
 * @param[in]	pcSrc		Specifies the source string
 * @param[in]	iSrcLength	Size of the source string
 * @param[in]	uiDestSize	Size of the destination string
 * @param[out]	pmDest		Holds the unicode string that is formed
 * @retval		JC_OK On success
 * @retval		JC_ERR_INVALID_PARAMETER One or more of the input parameters are not valid
 * @note		The parameters pcSrc and pmDest should not be NULL.
 * @note		The caller function should allocate memory for the destination string.
 *
 * This function is used convert a specified no of characters in a 
 * character string to a unicode string. The caller function has to allocate 
 * memory for the destination string.
 */
JC_RETCODE jdi_CUtilsCharToTcharNMem (const JC_INT8		*pcSrc, 
									  JC_INT32			iSrcLength,
									  JC_UINT32			uiDestSize, 
									  JC_CHAR			*pmDest) ;

/**
 * @brief This function converts a unicode string to character string.
 * @param[in]	pmSrc		Specifies the source string
 * @param[in]	uiDestSize	Size of the destination string
 * @param[out]	pcDest		Holds the character string that is formed
 * @retval		JC_OK On success
 * @retval		JC_ERR_INVALID_PARAMETER One or more of the input parameters are not valid
 * @note		The parameters pmSrc and pcDest should not be NULL.
 * @note		The caller function should allocate memory for the destination string.
 *
 * This function is used convert a unicode string to a character string. 
 * The caller function has to allocate memory for the destination string.
 */
JC_RETCODE jdi_CUtilsTcharToCharMem (const JC_CHAR		*pmSrc, 
									 JC_UINT32			uiDestSize,
									 JC_INT8			*pcDest) ;

/**
 * @brief This function converts unicode string to character string.
 * @param[in]	vMemHandle	Handle used in memory operations
 * @param[in]	pmSrc		Specifies the source string
 * @param[out]	ppcDest		Holds the character string that is formed
 * @retval		JC_OK On success
 * @retval		JC_ERR_INVALID_PARAMETER One or more of the input parameters are not valid
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @note		The parameters pmSrc and ppcDest should not be NULL.
 * @note		The caller function has to free the memory allocated for the destination.
 *
 * This function is used convert a unicode string to a character string.
 * The function will allocate memory for the destination string.
 */
JC_RETCODE jdi_CUtilsTcharToChar (JC_HANDLE			vMemHandle,
								  const JC_CHAR		*pmSrc, 
								  JC_INT8			**ppcDest) ;

/**
 * @brief This function converts a character string to unicode string.
 * @param[in]	vMemHandle	Handle used in memory operations
 * @param[in]	pcSrc		Specifies the source string
 * @param[out]	ppmDest		Holds the unicode string that is formed
 * @retval		JC_OK On success
 * @retval		JC_ERR_INVALID_PARAMETER One or more of the input parameters are not valid
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @note		The parameters pcSrc and ppmDest should not be NULL.
 * @note		The caller function has to free the memory allocated for the destination.
 *
 * This function is used to convert a character string to a unicode string.
 * The function will allocate memory for the destination string.
 */
JC_RETCODE jdi_CUtilsCharToTchar (JC_HANDLE			vMemHandle,
								  const JC_INT8		*pcSrc, 
								  JC_CHAR			**ppmDest) ;

/**
 * @brief This function converts a character string to unicode string.
 * @param[in]	pcSrc		Specifies the source string
 * @param[out]	pmDest		Holds the unicode string that is formed
 * @retval		Returns the no of characters that is copied
 * @note		The parameters pcSrc and pmDest should not be NULL.
 * @note		The caller function has to allocate memory for the destination string.
 *
 * This function is used to convert a character string to a unicode string and 
 * returns the no of characters that is copied.
 * The caller function has to allocate memory for the destination string.
 */
JC_INT32 jdi_CUtilsTcsStrCpy (const JC_INT8		*pcSrc, 
							  JC_CHAR			*pmDest) ;

/**
 * @brief This function compares the given unicode string and character string 
 *		  irrespective of case.
 * @param[in]	pmFirstStr	Unicode string
 * @param[in]	pcSecondStr	Character string
 * @retval		-1  First string is less than the second string
 * @retval		0	Both the strings are equal
 * @retval		1   First string is greater than the second string
 * @note		The parameters pmFirstStr and pcSecondStr should not be NULL.
 *
 * This function is used to compare the given unicode string with the given 
 * character string. It performs the comparison irrespective of case.
 */
JC_INT16 jdi_CUtilsTcsStrCaseCmp (const JC_CHAR		*pmFirstStr,
								  const JC_INT8		*pcSecondStr) ;

/**
 * @brief This function finds the location of a character string in a unicode string.
 * @param[in]	pmSrc		Unicode string
 * @param[in]	pcSubStr	Character substring
 * @retval		NULL If the given substring is not found in the unicode string
 * @retval		Pointer to the location of the first occurrence of the substring 
 *				in the given unicode string if found
 * @note		The parameters pmSrc and pcSubStr should not be NULL.
 *
 * This function is used to find the occurrence of a character string in a 
 * unicode string. It returs the pointer to first occurrence of the character 
 * substring in the given unicode string.
 */
JC_CHAR * jdi_CUtilsTcsStrStr (const JC_CHAR	*pmSrc, 
							   const JC_INT8	*pcSubStr) ;

/**
 * @brief This function finds the location of a character string in a 
 *		  unicode string irrespective of case.
 * @param[in]	pmSrc		Unicode string
 * @param[in]	pcSubStr	Character substring
 * @retval		NULL If the given substring is not found in the unicode string
 * @retval		Pointer to the location of the first occurrence of the substring 
 *				in the given unicode string if found
 * @note		The parameters pmSrc and pcSubStr should not be NULL.
 *
 * This function is used to find the occurrence of a character string in a 
 * unicode string. It performs the search irrespective of case. It returs 
 * the pointer to first occurrence of the character substring in the given 
 * unicode string.
 */
JC_CHAR * jdi_CUtilsTcsStrCaseStr (const JC_CHAR	*pmSrc,
								   const JC_INT8	*pcSubStr) ;

/**
 * @brief This function converts specified no of characters in 
 *		   character string to unicode string.
 * @param[in]	vMemHandle	Handle used in memory operations
 * @param[in]	pcSrc		Specifies the source string
 * @param[in]	iCount		No of characters to be copied
 * @param[out]	ppmDest		Holds the unicode string that is formed
 * @retval		JC_OK On success
 * @retval		JC_ERR_INVALID_PARAMETER One or more of the input parameters are not valid
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @note		The parameters pcSrc and ppmDest should not be NULL.
 * @note		The caller function has to free the memory allocated for the destination.
 *
 * This function is used convert a specified no of characters in a 
 * character string to a unicode string. This function allocates memory 
 * for the destination string.
 */
JC_RETCODE jdi_CUtilsTcsStrNCpy (JC_HANDLE		vMemHandle,
								 const JC_INT8	*pcStr, 
								 JC_INT32		iCount, 
								 JC_CHAR		**ppmDest) ;

/**
 * @brief This function concatenates the given character string to the unicode string.
 * @param[in]	pcSrc		Specifies the source string
 * @param[out]	pmDest		destination string to which the source string has to be concatenated.
 * @retval		JC_OK On success
 * @retval		JC_ERR_NULL_POINTER One or more of the input parameters are NULL
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @note		The parameters pcSrc and pmDest should not be NULL.
 * @note		The destination string should have enough memory to hold the new string
 *
 * This function is used concatenate the given character string to the unicode 
 * string. The calling function should make sure that the destination string has 
 * enough memory to hold the new string.
 */
JC_RETCODE jdi_CUtilsTcsStrCat (const JC_INT8	*pcSrc, 
								JC_CHAR			*pmDest) ;

/**
 * @brief This function compares the given unicode string and character string.
 * @param[in]	pmFirstStr	Unicode string
 * @param[in]	pcSecondStr	Character string
 * @retval		-1  First string is less than the second string
 * @retval		0	Both the strings are equal
 * @retval		1   First string is greater than the second string
 * @note		The parameters pmFirstStr and pcSecondStr should not be NULL.
 *
 * This function is used to compare the given unicode string with the given 
 * character string.
 */
JC_INT16 jdi_CUtilsTcsStrCmp (const JC_CHAR		*pmFirstStr, 
							  const JC_INT8		*pcSecondStr) ;

/**
 * @brief This function performs a special comparison of unicode string and character string.
 * @param[in]	pmFirstStr	Unicode string
 * @param[in]	pcSecondStr	Character string
 * @retval		-1  First string is less than the second string
 * @retval		0	Both the strings are equal
 * @retval		1   First string is greater than the second string
 * @note		The parameters pmFirstStr and pcSecondStr should not be NULL.
 *
 * This function is used to compare the given unicode string with the given 
 * character string. This function ignores the backslash characters present 
 * in the charcacter string. This function treats the characters single quote 
 * and double quote alike. With these conditions it performs the comparison.
 */
JC_INT16 jdi_CUtilsTcsStrSplCharCmp (const JC_CHAR		*pmFirstStr,
									 const JC_INT8		*pcSecondStr) ;

/**
 * @brief This function finds the position of a character string in a unicode string.
 * @param[in]	pmSrc		Unicode string
 * @param[in]	pcSubStr	Character substring
 * @param[in]	iStartIndex	Index in the source stirng from which search has to start
 * @retval		-1 If the given substring is not found in the unicode string
 * @retval		Position of the first occurrence of the substring in the given 
 *				unicode string if found
 * @note		The parameters pmStr and pcSubStr should not be NULL.
 *
 * This function is used to find the first occurrence of a character string in a 
 * unicode string. It performs the search starting from the given index. 
 * It returs the position of first occurrence of the character substring 
 * in the given unicode string.
 */
JC_INT32 jdi_CUtilsTcsIndexOfSubStr (const JC_CHAR	*pmStr, 
									 const JC_INT8	*pcSubStr, 
									 JC_INT32		iStartIndex) ;

/**
 * @brief This function checks if a unicode string starts with a 
 *		  character substring.
 * @param[in]	pmSrc		Specifies the source string
 * @param[in]	pcSubStr	Specifies the character Substring
 * @retval		E_TRUE if the unicode string starts with the given substring
 * @retval		E_FALSE if the unicode string does not start with the given substring
 * @note		The parameters pmSrc and pcSubStr should not be NULL.
 *
 * This function is used to check if the given unicode string starts with 
 * the given character substring.
 */
JC_BOOLEAN jdi_CUtilsTcsStrStartsWith (const JC_CHAR	*pmSrc, 
									   const JC_INT8	*pcSubStr) ;

/**
 * @brief This function checks if a unicode string ends with a 
 *		  character substring.
 * @param[in]	pmSrc		Specifies the source string
 * @param[in]	pcSubStr	Specifies the character Substring
 * @retval		E_TRUE if the unicode string ends with the given substring
 * @retval		E_FALSE if the unicode string does not end with the given substring
 * @note		The parameters pmSrc and pcSubStr should not be NULL.
 *
 * This function is used to check if the given unicode string ends with 
 * the given character substring.
 */
JC_BOOLEAN jdi_CUtilsTcsStrEndsWith (const JC_CHAR	*pmSrc,
									 const JC_INT8	*pcSubStr) ;

/**
 * @brief This function converts specified no of characters in a 
 *		   unicode string to a character string.
 * @param[in]	pmStr		Specifies the source string
 * @param[in]	iCount		No of characters to be copied
 * @param[out]	pcDest		Holds the character string that is formed
 * @retval		JC_OK On success
 * @retval		JC_ERR_INVALID_PARAMETER One or more of the input parameters are not valid
 * @note		The parameters pmStr and pcDest should not be NULL.
 * @note		The calling function has to allocate memory for the destination.
 *
 * This function is used convert a specified no of characters in a 
 * unicode string to character string.
 */
JC_RETCODE jdi_CUtilsStrTcsNCpy (const JC_CHAR		*pmStr, 
								 JC_INT32			iCount, 
								 JC_INT8			*pcDest) ;

/**
 * @brief This function compares the given unicode string and UTF8 format string.
 * @param[in]	pmFirstStr	Unicode string
 * @param[in]	pcUTF8Str	String in UTF8 format
 * @retval		-1  First string is less than the second string
 * @retval		0	Both the strings are equal
 * @retval		1   First string is greater than the second string
 * @note		The parameters pmFirstStr and pcUTF8Str should not be NULL.
 *
 * This function is used to compare the given unicode string with the given 
 * UTF8 format string.
 */
JC_INT16 jdi_CUtilsTcsUTF8Cmp (const JC_CHAR	*pmFirstStr,
							   const JC_INT8	*pcUTF8Str) ;

/**
 * @brief This function compares the given unicode string and UTF8 format string.
 * @param[in]	pmFirstStr	Unicode string
 * @param[in]	pcUTF8Str	String in UTF8 format
 * @retval		-1  First string is less than the second string
 * @retval		0	Both the strings are equal
 * @retval		1   First string is greater than the second string
 * @note		The parameters pmFirstStr and pcUTF8Str should not be NULL.
 *
 * This function is used to compare the given unicode string with the given 
 * UTF8 format string (case insensitive compare).
 */
JC_INT16 jdi_CUtilsTcsUTF8CaseCmp (const JC_CHAR	*pmFirstStr,
								   const JC_INT8	*pcUTF8Str) ;

/**
 * @brief This function converts a UTF8 format string to unicode string.
 * @param[in]	vMemHandle	Handle used in memory operations
 * @param[in]	pucSrc		Specifies the source string
 * @param[out]	ppmDest		Holds the unicode string that is formed
 * @retval		JC_OK On success
 * @retval		JC_ERR_INVALID_PARAMETER One or more of the input parameters are not valid
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @note		The parameters pucSrc and ppmDest should not be NULL.
 * @note		The caller function has to free the memory allocated for the destination.
 *
 * This function is used convert a string in a UTF8 format to a unicode string.
 * The function will allocate memory for the destination string.
 */
JC_RETCODE jdi_CUtilsUTF8ToTchar (JC_HANDLE		vMemHandle,
								  JC_UINT8		*pucSrc,
								  JC_CHAR		**ppmDest) ;

/**
 * @brief This function performs a special comparison of unicode string and UTF8 format string.
 * @param[in]	pmFirstStr	Unicode string
 * @param[in]	pcSecondStr	String in UTF8 format
 * @retval		-1  First string is less than the second string
 * @retval		0	Both the strings are equal
 * @retval		1   First string is greater than the second string
 * @note		The parameters pmFirstStr and pcSecondStr should not be NULL.
 *
 * This function is used to compare the given unicode string with the given 
 * UTF8 format string. This function ignores the backslash characters present 
 * in the UTF8 format string. This function trats the characters single quote 
 * and double quote alike. With these conditions it performs the comparison.
 */
JC_INT16 jdi_CUtilsTcsUTF8SplCharCmp (const JC_CHAR		*pmFirstStr,
									  const JC_INT8		*pcSecondStr) ;

/**
 * @brief This function converts unicode string to character string in lowercase  
 * @param[in]   vMemHandle  Memory Handle
 * @param[in]	pmSrc		Specifies the source string
 * @param[out]	pcDest		Holds the character string that is formed
 * @retval		JC_OK On success
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @note		The parameters pmSrc and ppcDest should not be NULL.
 * @note		The caller function has to free the memory allocated for the destination.
 *
 * This function is used convert a unicode string to a character string in lowercase,
 * with a care the content within quotes will not be converted into lowercase.
 * The function will allocate memory for the destination string.
 */
JC_RETCODE jdi_CUtilsTcsStrSplConvertToLower (JC_HANDLE			vMemHandle, 
											  const JC_CHAR		*pmSrc, 
										      JC_INT8			**ppcDest) ;


/**
 * @brief	  The function copies a given source string with the length to be copied 
 *            specified in the form of begin and end index and returns the copied string. 
 * @param[in]   vMemHandle Specifies the memory handle used by the common utils module
 * @param[in]	pcSrc Specifies the source character string that needs to be copied
 * @param[in]	iBegin Specifies the index value of source string from which the copying has to be started
 * @param[in]	iEnd Specifies the index value of source string at which copying has to end
 * @retval		JC_INT8 unicode string which is formed after copying
 * @retval		JC_NULL if a memory allocation error occurs
 *
 * The function is used to copy specific part of the source string. The part to be copied is 
 * specified by the begin and end index.
 */
JC_CHAR * jdi_CUtilsTcsStrCopyIndex (JC_HANDLE			vMemHandle, 
									 const JC_INT8		*pcStr, 
									 JC_INT32			iBegin, 
									 JC_INT32			iEnd) ;

#ifdef __cplusplus
}
#endif

#endif

/* End of File */


