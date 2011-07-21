/****************************************************************
 * $Id: jdi_inputvalidation.h,v 1.10 2007/04/19 11:07:04 kavithadevhtmlbrow Exp $
 * $Revision: 1.10 $
 * $Date: 2007/04/19 11:07:04 $
 * 
 * IMPORTANT NOTICE
 * Please note that any and all title and/or intellectual property 
 * rights in and to this Software or any part of this	(including 
 * without limitation any images, photographs, animations,	video, 
 * audio, music, text and/or "applets," incorporated into	   the 
 * Software), herein mentioned to as "Software", the  accompanying 
 * printed materials, and any copies of the Software, are owned by 
 * Jataayu Software (P) Ltd., Bangalore ("Jataayu") or	 Jataayu's 
 * suppliers as the case may be. The Software is protected		by 
 * copyright, including without limitation by applicable copyright 
 * laws, international treaty provisions,	other	  intellectual 
 * property laws and applicable laws in the country in	which  the 
 * Software is being used. You shall not modify,adapt or translate 
 * the Software,without prior express written consent from Jataayu.
 * You shall not reverse engineer, decompile,	disassemble		or 
 * otherwise alter the Software,except and only to the extent that 
 * such activity is  expressly  permitted   by   applicable	   law 
 * notwithstanding this limitation. Unauthorized  reproduction  or 
 * redistribution of this program or any  portion of it may result 
 * in severe civil and criminal penalties and will  be  prosecuted 
 * to the maximum extent possible under the law.  Jataayu reserves 
 * all rights not expressly granted.
 *
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL  REPRESENTATION  AND   WARRANTIES,   EITHER 
 * EXPRESS OR IMPLIED, INCLUDING  BUT  NOT  LIMITED  TO THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, 
 * AND/OR FITNESS FOR A  PARTICULAR  PURPOSE  OR  USE,  TITLE    OR 
 * INFRINGEMENT ARE  EXPRESSLY  DISCLAIMED  TO  THE  FULLEST EXTENT 
 * PERMITTED BY LAW. YOU ASSUME THE ENTIRE RISK AS TO THE  ACCURACY 
 * AND THE USE OF THIS SOFTWARE.JATAAYU SHALL NOT BE LIABLE FOR ANY 
 * CONSEQUENTIAL,  INCIDENTAL,  INDIRECT,  EXEMPLARY,   SPECIAL  OR 
 * PUNITIVE DAMAGES INCLUDING  WITHOUT LIMITATION ANY LOSS OF DATA, 
 * OR; LOSS OF PROFIT,SAVINGS BUSINESS OR GOODWILL OR OTHER SIMILAR 
 * LOSS RESULTING FROM  OR OUT OF  THE USE OR INABILITY TO USE THIS 
 * SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 ******************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdi_inputvalidation.h,v $
 * Revision 1.10  2007/04/19 11:07:04  kavithadevhtmlbrow
 * Format Validation updated
 *
 * Revision 1.9  2006/07/05 08:57:46  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2006/05/26 10:04:11  kumardevhtmlbrow
 * Updated....
 *
 * Revision 1.7  2006/05/12 13:25:35  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2006/04/22 06:53:15  kumardevhtmlbrow
 * LINT warning ....
 *
 * Revision 1.5  2006/02/21 06:12:44  pathakdevhtmlbrow
 * Added jdi function to validate whole input against given format
 *
 * Revision 1.4  2005/10/05 12:08:10  kavithadevhtmlbrow
 * Input validation set value updated
 *
 * Revision 1.3  2005/09/12 12:20:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/08/17 06:23:56  kumardevhtmlbrow
 * Updated the jdd_MemAlloc to jdi_MemAlloc function.
 *
 * Revision 1.1  2005/07/07 09:27:49  kumardevhtmlbrow
 * no message
 *
 * 
 *  
 *****************************************************************/

#ifndef _JDI_INPUTVALIDATION_H
#define _JDI_INPUTVALIDATION_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	    Validates the Input format.
 *
 * @param[in]	pmFormat    Specifies the Input Format String.
 * @param[out]  pbIsEmptyOK Reference to hold the empty ok flag
 * @retval		E_TRUE if the specified format is valid else E_FALSE.
 *
 * This API needs to be called for validation of any Input Format Specifier
 * as per specification.
 *
 */
JC_BOOLEAN jdi_InputValidationIsFormatValid (const JC_CHAR	*pmFormat,
											 JC_BOOLEAN		*pbIsEmptyOK) ;


/**
 * @brief	    This function checks whether the input value is 
 *				according to the specified format and adds any 
 *				special character to the begining of input string.
 *
 * @param[in]	vMemHandle Memory Handle to be used for any memory operation.
 * @param[in]   pmFormat Specifies the Input Format Specifier.
 * @param[in]	pmValue The input string entered for the particular input.
 * @param[out]	ppmRetValue Specifies the return value for the input string.
 * @retval		Memory related error Code or Success.
 *
 * This function checks whether if the input value is according to the input
 * format specifier, if the input value string "pmValue" is not NULL.If the 
 * input value is not as per the format specified then NULL is returned as the
 * output value from the function.If the input value  matches the format
 * then a copy of the input value is returned as output from the fucntion, if 
 * memory allocation is successful else memory allocation error is returned.
 * If the input value "pmValue" is NULL and the format specifies certain 
 * special charaters at the begining of the string then those charaters are
 * added to the begining of the return value provided no memory allocation 
 * error occurs in which case the error code is returned, else if no special 
 * characters is specified at the begining of the format string, NULL is returned
 * as the return value.
 *
 *
 * @note	The memory allocated for the output value is to be freed by the 
 *			caller of this API. 
 */

JC_RETCODE jdi_InputValidationSetValue (JC_HANDLE vMemHandle, 
									  const JC_CHAR	*pmFormat, 
									  const JC_CHAR	*pmValue,
									  JC_CHAR	**ppmRetValue) ;


/**
 * @brief	    This funciton checks whether the user input matches the format
 *				specified
 * @param[in]	vMemHandle Memory Handle to be used for any memory operation.
 * @param[in]   pmFormat Specifies the Input Format Specifier.
 * @param[out]	piCurrPos the current cursor position within the input string.	
 * @param[out]	ppmValue The input value for the particular input.
 * @retval		Memory related error Code or Success.
 * @note		The cursor position in the input string should not be negative
 *
 * This funciton checks whether each charater of the input string entered by the
 * user matches the format specified for the particular input.This function is 
 * to be called for each charater of user input.If the input charater matches the
 * format specified it is accepted else if it does not match the format it is removed
 * from the input value string and the value upto the matching character is returned
 * as output provided no memory error occurs.Also if the input format specfies certain
 * special charaters after the current charater input of the input string. then those
 * characters are added to the output value of the input string (provided no memory
 * error occurs).If any memory error occurs the specifies error code is returned.
 *
 * @note	The memory allocated for the output value is to be freed by the 
 *			caller of this API. 
 */

JC_RETCODE jdi_InputValidationValidateInput (JC_HANDLE vMemHandle, 
											 const JC_CHAR	*pmFormat, 
											 JC_INT32	*piCurrPos, 
											 JC_CHAR	**ppmValue) ;


/**
 * @brief	    Validates the total input entered by the user.
 *
 * @param[in]	vMemHandle, Memory manager handle
 * @param[in]	pmFormat Specifies the Input Format String.
 * @param[in]	pmValue The input string entered by the user
 * @retval		E_TRUE if the input entered is valid according to format else E_FALSE.
 *
 * This API validates the entire input value entered by the user against the
 * fornmat specified by the input format specifier for the particular input.
 *
 */

JC_RETCODE	jdi_InputValidationCheckValidity (JC_HANDLE vMemHandle, 
											  const JC_CHAR	*pmFormat, 
											  const JC_CHAR	*pmValue, 
											  JC_BOOLEAN	*pbIsValid) ;


#ifdef __cplusplus
}
#endif

#endif

/* End of File */


