/***************************************************************************
 * $Id: jdi_cutils.h,v 1.8 2008/06/16 05:52:17 browserdevjtbase Exp $
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
 * $Log: jdi_cutils.h,v $
 * Revision 1.8  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.28  2007/12/28 12:36:07  kumardevhtmlbrow
 * doxygen changes.
 *
 * Revision 1.27  2007/01/09 14:27:30  kumardevhtmlbrow
 * Added the utility function
 *
 * Revision 1.26  2006/12/05 13:09:10  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.25  2006/07/20 04:58:19  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.24  2006/03/01 14:57:58  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.23  2006/01/18 06:04:23  shreyasdevhtmlbrow
 * Removed Multiple declaration of Is Hex digits function
 *
 * Revision 1.22  2006/01/17 14:39:12  kumardevhtmlbrow
 * Updated the comments ...
 *
 * Revision 1.21  2005/12/17 06:15:28  kumardevhtmlbrow
 * Updated for handling the Operator caching to check the domain information
 *
 * Revision 1.20  2005/12/12 15:29:01  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.19  2005/09/26 15:57:29  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.18  2005/09/26 14:20:19  kavithadevhtmlbrow
 * jdi_CUtilsIsHexDigits added
 *
 * Revision 1.17  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.16  2005/08/17 06:22:00  kumardevhtmlbrow
 * Updated the jdd_MemAlloc to jdi_MemAlloc function.
 *
 * Revision 1.15  2005/08/09 12:18:43  kavithadevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.14  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.13  2005/06/24 13:36:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.12  2005/06/24 12:04:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.11  2005/06/22 09:32:32  kumardevhtmlbrow
 * no message
 *
 * Revision 1.10  2005/06/16 13:47:10  kumardevhtmlbrow
 * Updated the extra function and separated out the tcs and tcsstr function.
 *
 * Revision 1.9  2005/06/04 07:59:16  kumardevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/05/18 15:36:39  kumardevhtmlbrow
 * Update
 *
 * Revision 1.7  2005/05/13 06:08:35  kumardevhtmlbrow
 * Added the regularly used constants and common module header file
 *
 * Revision 1.6  2005/05/10 07:55:48  kumardevhtmlbrow
 * Added additional utility function
 *
 * Revision 1.5  2005/05/10 05:42:52  kumardevhtmlbrow
 * Added the hash utility function
 *
 * Revision 1.4  2005/05/09 15:13:02  kumardevhtmlbrow
 * Code Review Update and added the Stack utility function
 *
 * Revision 1.3  2005/05/09 06:30:54  kumardevhtmlbrow
 * Fixed the lint warning/info
 *
 * Revision 1.2  2005/04/08 08:25:56  kumardevhtmlbrow
 * Removed Char & TChar function and moved to separate file
 *
 * Revision 1.1  2005/04/05 13:05:14  kavithadevhtmlbrow
 * Common Utility Files
 *
 *  
 ***************************************************************************/

/*
 * @file jdi_commonutils.h
 * @ingroup CommonUtils
 * @brief It contains declaration of common utility functions.
 *
 * The file contains the declarations of the common utility functions. Common utility signifies
 * that none of the functions in this file is specific to string or time or any other characteristic.
 */

#ifndef _JDI_CUTILS_H
#define _JDI_CUTILS_H

#include <jdi_commonmodule.h>
#include <jdi_cutils_str.h>
#include <jdi_cutils_tcs.h>
#include <jdi_cutils_tcsstr.h>
#include <jdi_cutils_stack.h>

/*** Macros ***/

#define SPACE				32
#define SMALL_BUF			127

#ifdef __cplusplus
extern "C" {
#endif

/*** Error Codes	***/

/**
 * @brief The error code is returned if an invalid data is specified to any of the common utillty function
 */
#define	JC_ERR_UTILS_INVALID_DATA		TERMINAL_ERR_CODE (COMMON_UTILS_MODULE, 0x01)

/*** Numeric Utility  ***/

/**
 * @brief	  This function converts a hexa decimal value to decimal value.
 * @param[in]	pcHexValue Specefies the hexa decimal value to be converted.
 * @param[in,out] piDecValue The function will return the decimal value in this parameter.
 * @retval		JC_OK if the hexadecimal value is converted into decimal value.
 * @retval		JC_ERR_INVALID_PARAMETER if the hexadecimal buffer is NULL or the buffer in which
 *				the decimal value is to be returned is not allocated by the calling function.
 * @retval		JC_ERR_UTILS_INVALID_DATA if the hexadecimal content is invalid.
 * @note		The parameter which should not be NULL are pcHexValue and piDecValue
 *
 * The function is to be used when a hexa decimal value is to be converted to a decimal value
 * The function parses through the hexa decimal buffer and converts each hexa decimal value to
 * the corresponding decimal value.
 */
JC_RETCODE jdi_CUtilsStrHexaToDecimal (const JC_INT8		*pcHexValue,
									   JC_INT32				*piDecValue) ;

/**
 * @brief	  The function is used to convert an integer value to ascii value.
 * @param[in] vMemHandle Specifies the memory handle to be used by the common utility module.
 * @param[in] iNumber Specifies the Integer value to be converted.
 * @retval		character buffer containing the ascii value
 * @retval		JC_NULL If a memory allocation error occured.
 *
 * The function is used to convert an integer to ascii value. The function parses through each 
 * digit of the integer value to be converted and then converts it into ascii value.
 */
JC_INT8 * jdi_CUtilsItoA (JC_HANDLE vMemHandle, JC_INT32 iNumber) ;

/**
 * @brief	  The function is used to convert an integer value to unicode Value.
 * @param[in] vMemHandle Specifies the memory handle to be used by the common utility module.
 * @param[in] iNum Specifies the Integer value to be converted.
 * @retval		unicode buffer containing the Converted unicode Value
 * @retval		JC_NULL If a memory allocation error occured.
 *
 * The function is used to convert an integer to unicode value. The function parses through each 
 * digit of the integer value to be converted and then converts it into unicode value.
 */
JC_CHAR	* jdi_CUtilsItoT (JC_HANDLE vMemHandle, JC_INT32 iNum) ;

/**
 * @brief	  This function is used to get the hash value of a character String.
 * @param[in] pcString Specifies the character string whose hash value is to be retunred
 * @retval	  JC_UINT32 Value specifying the hash value of the character string
 *
 * The function is used to obtain the hash value of a character string.
 */
JC_UINT32 jdi_CUtilsGetHashValue (const JC_INT8 *pcString) ;

/**
 * @brief	  This function is used to get the hash value of a sub string of a specified character string.
 * @param[in] pcString Specifies the character string which is passed as the input to this function. Substring 
 *			  of this string needs to be parsed and the hash value of the substring needs to be returned.
 * @param[in] iEndLen Specifies the length of the substring from the starting of the string.
 * @retval	  JC_UINT32 Value specifying the hash value of the sub string
 *
 * The function parses the given charater string to the length specified as the parameter and 
 * return the hash value of the substring.
 */
JC_UINT32 jdi_CUtilsGetSubStrHashValue (const JC_INT8 *pcString, JC_INT32 iEndLen) ;

/**
 * @brief	  This function is used to get the hash value of a unicode string.
 * @param[in] pmString Specifies the unicode string whose hash value is to be retunred
 * @retval	  JC_UINT32 Value specifying the hash value of the unicode string
 *
 * The function is used to obtain the hash value of a unicode string.
 */
JC_UINT32 jdi_CUtilsTcsGetHashValue (const JC_CHAR *pmString) ;

void jdi_CUtilsTcsGetEvenOddHashValue (const JC_CHAR *pmString, 
									   JC_UINT32 *puiOddHashValue,
									   JC_UINT32 *puiEvenHashValue) ;
/**
 * @brief	  This function checks whether a character string consists of hexadecimal characters
 * @param[in] pcValue Specifies the character string
 * @retval	  JC_BOOLEAN value specifying whether the character string consists of hexadecimal digits or not
 *
 * The function is used to check whether a character string consists of hexadecimal digits. 
 * The function checks each character of the string and returns E_TRUE if all the characters turn
 * out to be hexadecimal digits.
 */
JC_BOOLEAN jdi_CUtilsIsHexDigits (const JC_INT8 *pcValue) ;

/**
 * @brief	  This function validates an IP address
 * @param[in] pcIPAddress Specifies the IP address which needs to be validated
 * @retval	  JC_BOOLEAN value specifying whether the IP address is valid or not
 *
 * The function is used to validate an IP address. 
 */
JC_BOOLEAN jdi_CUtilsIsValidIPAddress (const JC_INT8 *pcIPAddress) ;

/**
 * @brief	  This function checks whether the given address is a IP address or not
 * @param[in] pcIPAddress Specifies the IP address which needs to be verified
 * @retval	  JC_BOOLEAN value specifying whether the address is IP address or not
 *
 * The function is used to check whether a given address is an IP address or not.
 */

JC_BOOLEAN	jdi_CUtilsIsIPAddress (const JC_INT8	*pcIPAddress) ;

/**
 * @brief	  This function converts a unicode string to integer value
 * @param[in] vMemHandle Specifies the memory handle to be used by the common utility module
 * @param[in] pmValue Specifies the unicode string to be converted.
 * @retval	  JC_INT32 Integer value that is obtained from the unicode string
 *
 * This function is to be used to convert an unicode string to an interger value. 
 */
JC_INT32 jdi_CUtilsTtoI (JC_HANDLE vMemHandle, const JC_CHAR *pmValue) ;

/**
 * @brief This function converts the JC_CHAR buffer of specified
 *		  base to unsigned long Decimal value
 * @param[in]  vMemHandle  memhandle to be used for memory allocation
 * @param[in]  pmValue  Specifies the CHAR buffer to converted.
 * @param[in]  iBase Specifies the Number base to use for conversion.
 * @retval	JC_UINT64 converted long value.
 */

JC_UINT64 jdi_CUtilsBaseTtoUL (JC_HANDLE vMemHandle, const JC_CHAR *pmValue, const JC_INT32 iBase) ;

/**
 * @brief This function converts the JC_CHAR buffer double precision value
 * @param[in]  vMemHandle  memhandle to be used for memory allocation
 * @param[in]  pmValue  Specifies the CHAR buffer to converted.
 * @retval	JC_DOUBLE converted value.
 */

JC_DOUBLE jdi_CUtilsTtoD (JC_HANDLE vMemHandle, const JC_CHAR *pmValue) ;

/**
 * @brief	  This function is used to read the content from a file
 * @param[in] vMemHandle Specifies the memory handle to be used by the common utility module
 * @param[in] pmFileName Specifies the name of the file that needs to be read
 * @param[out] ppucData The data that is read from the file is returned in this parameter.
 * @param[out] puiLength Specifies the length of the data that was read from the file.
 * @retval	  JC_OK if the data is read from the file successfully.
 * @retval	  JC_ERR_INVALID_PARAMETER if file name or the ppucData are NULL
 * @retval	  JC_ERR_FILE_SYS_INIT if file system initialization fails
 * @retval	  JC_ERR_FILE_NOT_FOUND if the file name specified is not found
 * @retval	  JC_ERR_FILE_OPEN if the file specified cannot be opened
 * @retval	  JC_ERR_FILE_READ if file reading operation fails.
 * @retval	  JC_ERR_MEMORY_ALLOCATION if memory allocation error occurs
 * @note	  The parameters which should not be null are ppucData and pmfileName
 *
 * The function is used to read the contents from a file. The function will open and read the file
 * using the JDD file operations and return the data to the calling function.
 */
JC_RETCODE jdi_CUtilsReadFromFile (JC_HANDLE	vMemHandle,
								   JC_CHAR		*pmFileName, 
								   JC_UINT8		**ppucData,
								   JC_UINT32	*puiLength) ;

/**
 * @brief	  This function is used to write the content into a file
 * @param[in] pmFileName Specifies the name of the file into which the data is to be written
 * @param[in] pucData Specifies the data that is to be written into the file
 * @param[out] uiLength Specifies the length of the data that is to be written into file.
 * @retval	  JC_OK if the data is written into the file successfully.
 * @retval	  JC_ERR_INVALID_PARAMETER if file name, ppucData are NULL or uiLength is 0
 * @retval	  JC_ERR_FILE_SYS_INIT if file system initialization fails
 * @retval	  JC_ERR_FILE_OPEN if the file specified cannot be opened
 * @retval	  JC_ERR_FILE_WRITE if file writing operation fails.
 * @note	  The parameters which should not be null are ppucData and pmfileName
 * @note	  The parameters which should not have the value 0 is uiLength
 *
 * The function is used to write contents into a file. The function will open the file and write the
 * data into the file using the JDD file operations.
 */
JC_RETCODE jdi_CUtilsWriteIntoFile (const JC_CHAR	*pmFileName, 
									const JC_UINT8	*pucData,
									JC_UINT32		uiLength) ;

#ifdef __cplusplus
}
#endif

#endif

/* End of File */



