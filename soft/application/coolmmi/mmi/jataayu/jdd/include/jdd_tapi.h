/***************************************************************************
 * $Id: jdd_tapi.h,v 1.2 2006/05/02 11:19:21 kumardevjtbase Exp $
 * $Revision: 1.2 $
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
 * $Log: jdd_tapi.h,v $
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.5  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.2  2005/03/24 10:57:50  kumardevhtmlbrow
 * Changed as per the coding standard
 *
 *  
 ***************************************************************************/


#ifndef _JDD_TAPI_H_
#define _JDD_TAPI_H_

/** 
 * @file jdd_tapi.h
 * @ingroup TAPI
 * @brief Describes the basic telephone interface calls.
 *
 * All the telephony API calls can be implemented either as the blocking call or 
 * non-blocking call depending upon the platform characteristics. If the platform 
 * does not support the blocking call, then the telephony application layer needs to 
 * invoke the callback function once the operation is succeeded or failed with the 
 * appropriate status value for each API calls as specified in this document. 
 * In this case (platform does not support blocking call), the return value should be 
 * JC_ERR_TAPI_CONTINUE if the TAPI layer is going to invoke the callback function. 
 * If the platform supports the blocking call, then the API calls need to be return 
 * with the specific return value. 
 */

/**
 * @brief This is the handle associated with the TAPI initialize.
 */

typedef void * JDD_TAPI ;

/**
 * @brief This structure defines address book details.
 */
typedef struct st_jc_address_details
{
	JC_CHAR *psName ; /**< Specifies the name of the address book. */
	JC_CHAR *psPhoneNumber ; /**< Specifies the phone number of the address book. */
	JC_CHAR *psMobileNumber ; /**< Specifies the mobile number of the address book. */
	JC_CHAR *psEmailID ; /**< Specifies the email-id of the address book. */
} JC_ADDRESS_DETAILS ;

/**
 * @brief This enumeration specifies different TAPI operation available.
 */
typedef enum e_tapi_operations
{
	E_MAKE_CALL, /**< Specifies the make call operation. */
	E_SEND_DTMF, /**< Specifies the send DTMF operation. */
	E_ADD_PHONEBOOK, /**< Specifies the addition of new entry to the phone book. */
	E_GET_PHONEBOOK_COUNT, /**< Specifies the action to retrieve the number of entries in the phone book. */
	E_GET_PHONEBOOK_ENTRY /**< Specifies the action to retrieve the specific entry from the phone book. */
} ETAPIOperation ;

/** 
 * @brief This callback function is used when the TAPI API's are implemented as non-blocking call.
 * @param[in] pvAppArg Specifies the callback argument which needs to be passed when the TAPI API's are invoked.
 * @param[in] eOperation Specifies the TAPI operation type.
 * @param[in] rCode Specifies the status code.
 * 
 * This callback function is used to update the status of the TAPI operation.
 */
typedef void (* JC_TAPI_CALLBACK) (void *pvAppArg, ETAPIOperation eOperation, 
								   JC_RETCODE rCode) ;

#ifdef __cplusplus
extern "C"
{
#endif

/** 
 * @brief The telephony layer shall be initialized using this API call. 
 * @param[out] pvTAPIHandle Returns the reference, which shall be used with the other API calls.
 * This function needs to be invoked by the application before calling any of the TAPI 
 * calls specified in this document. If the platform does not require any 
 * initialization this call can return the success value. 
 */
JC_RETCODE jdd_TAPIInitialize (JDD_TAPI *pvTAPIHandle) ;

/**
 * @brief The telephony layer shall be deinitialized using this API call. 
 * @param[in] vTAPIHandle Specifies the reference to the TAPI, which is returned using the #jdd_TAPIInitialize.
 */
JC_RETCODE jdd_TAPIDeInitialize (JDD_TAPI vTAPIHandle) ;

/**
 * @brief This function makes voice call to the specified phone number. 
 * @param[in] vTAPIHandle Specifies the reference to the TAPI, which is returned using the #jdd_TAPIInitialize.
 * @param[in] pcNumber Specifies the phone number where voice calls need to be established.
 * @param[in] cbFunc Specifies the callback function (if available), for the non-blocking environment to know the status.
 * @param[in] pvArg Specifies the callback argument which needs to be used when invoking cbFunc.
 * @retval JC_ERR_TAPI_LINE_BUSY Specifies the line is busy.
 * @retval JC_ERR_TAPI_NETWORK_UNREACHABLE Specifies the network is unreachable. 
 * @retval JC_ERR_TAPI_NO_ANSWER Specifies there is no response from the other end.
 * Some environment does not allow making voice call to be blocked, in order to support 
 * that environment the callback function is provided. In this case this function 
 * should return JC_ERR_TAPI_CONTINUE and the callback function need to be invoked once 
 * the voice call is activated or failed with the proper error value.  
 * 
 * If the environment supports blocking call this function can directly return the 
 * status value as specified above when the voice is activated or failed. 
 */
JC_RETCODE jdd_TAPIMakeCall (JDD_TAPI vTAPIHandle, JC_INT8 *pcNumber, 
							 JC_TAPI_CALLBACK cbFunc, void *pvArg) ;

/**
 * @brief This function shall send the DTMF tone on the existing voice call channel. 
 * @param[in] vTAPIHandle Specifies the reference to the TAPI, which is returned using the #jdd_TAPIInitialize.
 * @param[in] pcDTMFTone Specifies the DTMF Tone, which need to be sent using the existing voice channel.
 * @param[in] cbFunc Specifies the callback function (if available), for the non-blocking environment to know the status.
 * @param[in] pvArg Specifies the callback argument which needs to be used when invoking cbFunc.
 */
JC_RETCODE jdd_TAPISendDTMFTone (JDD_TAPI vTAPIHandle, JC_INT8 *pcDTMFTone,
								 JC_TAPI_CALLBACK cbFunc, void *pvArg) ;

/**
 * @brief This function adds the specified name and phone number to the phone book.
 * @param[in] vTAPIHandle Specifies the reference to the TAPI, which is returned using the #jdd_TAPIInitialize.
 * @param[in] pstAddressDetails This structure specifies the name, mobile number, phone number and email-id that needs to be added to the phone book entry.
 * @param[in] cbFunc Specifies the callback function (if available), for the non-blocking environment to know the status.
 * @param[in] pvArg Specifies the callback argument which needs to be used when invoking cbFunc.
 */
JC_RETCODE jdd_TAPIAddPhoneBook (JDD_TAPI vTAPIHandle, JC_ADDRESS_DETAILS *pstAddressDetails,
								 JC_TAPI_CALLBACK cbFunc, void *pvArg) ;

/**
 * @brief This function retrieves the total number of entries in the phone's address book.
 * @param[in] vTAPIHandle Specifies the reference to the TAPI, which is returned using the #jdd_TAPIInitialize.
 * @param[out] puiAddressCount This parameter shall contain the total number of address entries in the phone book.
 * @param[in] cbFunc Specifies the callback function (if available), for the non-blocking environment to know the status.
 * @param[in] pvArg Specifies the callback argument which needs to be used when invoking cbFunc.
 */
JC_RETCODE jdd_TAPIGetAddressCount (JDD_TAPI vTAPIHandle, JC_UINT32 *puiAddressCount,
									JC_TAPI_CALLBACK cbFunc, void *pvArg) ;

/**
 * @brief This function retrieves the address entry as specified by the address index from the phone book.
 * @param[in] vTAPIHandle Specifies the reference to the TAPI, which is returned using the #jdd_TAPIInitialize.
 * @param[out] pstAddressDetails Retrieves the address book information for the given index.
 * @param[in] uiAddressIndex Specifies the index of the address entry to be retrieved. The index shall range from 0 to the maximum address book entry limit.
 * @param[in] cbFunc Specifies the callback function (if available), for the non-blocking environment to know the status.
 * @param[in] pvArg Specifies the callback argument which needs to be used when invoking cbFunc.
 */
JC_RETCODE jdd_TAPIGetAddressByIndex (JDD_TAPI vTAPIHandle, JC_ADDRESS_DETAILS *pstAddressDetails,
									  JC_UINT32 uiAddressIndex, JC_TAPI_CALLBACK cbFunc,
									  void *pvArg) ;

#ifdef __cplusplus
}
#endif

# endif

/* END OF FILE */


