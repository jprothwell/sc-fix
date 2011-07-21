/***************************************************************************
 * $Id: jdi_wapgsm.h,v 1.8 2008/06/16 05:52:17 browserdevjtbase Exp $
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
 * $Log: jdi_wapgsm.h,v $
 * Revision 1.8  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.10  2006/11/24 07:29:20  kumardevhtmlbrow
 * Update
 *
 * Revision 1.9  2006/06/07 15:24:36  kumardevhtmlbrow
 * LINT Warning
 *
 * Revision 1.8  2006/04/11 13:36:19  kumardevhtmlbrow
 * Lint Warning ...
 *
 * Revision 1.7  2006/03/01 14:57:58  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2005/12/29 07:41:11  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/08/12 10:57:42  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/06/23 09:49:46  kumardevhtmlbrow
 * no message
 *
 * Revision 1.1  2005/03/29 15:00:11  kumardevhtmlbrow
 * Added the WAPGSM Module.
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_wapgsm.h
 * @ingroup wapgsm
 * @brief Defines the exposed functions of the WAP GSM that is used by the application.
 * 
 * The WAP GSM module is introdued to parse the WAP GSM messages. This module will parse the
 * messages and also performs the segmentation and reassembly of the message packets. The 
 * final reassembled message packet is then sent to the transport using callback mechanism.
 */

#ifndef _JDI_WAPGSM_H
#define _JDI_WAPGSM_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	  This is the prototype of the callback which will be called by WAP GSM to send
 *			  the reassembled message to the application
 * @param[in]	pucBuffer Pointer to the reassempled message.
 * @param[in]	iLength Specifies the length of the message.
 * @param[in]	pcOrigAddress Specifies the address of the message originator
 * @param[in]	pcSMSAddress Specifies the SME address.
 * @param[in]	pvArg Specifies the application arguement.
 * @retval		void
 * @note		The parameters which should not be NULL are pvAppArg and pucBuffer.
 *
 * The application needs to implement a callback with the following prototype in order to get 
 * the reassembled message from the WAP GSM. 
 */

typedef void (* CB_WAPGSM_PARSE_MESSAGE) (const JC_UINT8	*pucBuffer,
										  JC_INT32			iLength, 
										  const JC_INT8		*pcOrigAddress,
										  JC_INT8			*pcSMSAddress,
										  void				*pvArg) ;

/**
 * @brief	  The application needs to use this function to initialize the WAP GSM module
 * @param[in]	cbWAPGSMMessage Specifies the application callback that is to be called by the
 *			    WAP GSM once the message is parsed.
 * @param[in]	pvArg Specifies the application argument that will be passed in the callback.
 * @param[in]	vMemHandle Specifies the memory handle that is to be used by WAP GSM.
 * @param[out]	pvHandle The initialize function returns this handle. The application needs to use
 *				this handle when it uses any WAP GSM functions.
 * @retval		JC_OK if the WAP GSM is initialized sucessfully.
 * @retval		JC_ERR_MEMORY_ALLOCATION if memory error occurs.
 * @note		The parameters which should not be NULL are pvHandle and cbWAPGSMMessage.
 * @see jdi_WAPGSMDeInitialize
 * @see CB_WAPGSM_PARSE_MESSAGE
 *
 * The application needs to call this function to initialize the WAP GSM module.
 */

JC_RETCODE jdi_WAPGSMInitialize (CB_WAPGSM_PARSE_MESSAGE		cbWAPGSMMessage,
								 void							*pvArg,
								 JC_HANDLE						vMemHandle,
								 JC_HANDLE						*pvHandle) ;

/**
 * @brief	  The application needs to use this function to deinitialize the WAP GSM module
 * @param[in]	vHandle Specifies the handle that is returned in the wap gsm initialize function.
 * @retval		void
 *
 * The application needs to call this function to deinitialize the WAP GSM module.
 */

void jdi_WAPGSMDeInitialize (JC_HANDLE vHandle) ;

/**
 * @brief	  The application needs to use this function to parse a WAP GSM message
 * @param[in]	vHandle Specifies the handle that is returned in the wap gsm initialize function.
 * @param[in]	pcOrigAddress Specifies the address of the message originator
 * @param[in]	pcSMSAddress Specifies the SME address.
 * @param[in]	pucBuffer Specifies the raw message data.
 * @param[in]	iLength Specifies the length of the message data.
 * @retval		JC_OK if the message is sucessfully parsed.
 * @retval		JC_ERR_MEMORY_ALLOCATION if a memory error occurs.
 *
 * The application needs to use this function to parse a raw message data. The WAP GSM will parse
 * this data and will send the parsed message to the application once all the segments of the 
 * message is received.
 */

JC_RETCODE jdi_WAPGSMParseMessage (JC_HANDLE			vHandle, 
								   const JC_INT8		*pcOrigAddress, 
								   JC_INT8				*pcSMSAddress,
								   const JC_UINT8		*pucBuffer,
								   JC_INT32				iLength) ;

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


