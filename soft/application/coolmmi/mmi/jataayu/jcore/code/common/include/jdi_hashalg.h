/***************************************************************************
 * $Id: jdi_hashalg.h,v 1.12 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.12 $
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
 * $Log: jdi_hashalg.h,v $
 * Revision 1.12  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.12  2008/04/11 13:07:45  kumardevhtmlbrow
 * Changes to support the image tag when the reference is given as part of the src itself.
 *
 * Revision 1.11  2008/01/08 07:45:05  kumardevhtmlbrow
 * Added the Quoted Printable to the hash algorithm module.
 *
 * Revision 1.10  2007/08/10 12:11:35  kumardevhtmlbrow
 * Hash algorithm interface updated.
 *
 * Revision 1.9  2007/07/13 14:47:21  kumardevhtmlbrow
 * Hashalgorithm updated with the new SHA1 and MD5 implementation.
 *
 * Revision 1.8  2007/07/02 10:04:07  kumardevhtmlbrow
 * MD5 Algorithm has been updated.
 *
 * Revision 1.7  2007/01/04 14:10:40  kumardevhtmlbrow
 * Added the base64 to hashing algorithm
 *
 * Revision 1.6  2006/01/03 14:29:26  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2006/01/03 10:32:49  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.4  2006/01/02 06:14:01  kumardevhtmlbrow
 * Hashalgorithm updated ...
 *
 * Revision 1.3  2005/12/29 08:04:05  kumardevhtmlbrow
 * Updated Hash algorithm and added push in browser & communicator
 *
 *  
 ***************************************************************************/

#ifndef _HASHALGO_H
#define _HASHALGO_H


#include <jdi_md5.h>
#include <jdi_sha1.h>

#define HASHLEN				16
#define HASHHEXLEN			32

typedef JC_UINT8 HASH_STRING_VALUE [HASHLEN] ;
typedef JC_UINT8 HASH_STRING_HEX_VALUE [HASHHEXLEN + 1] ;


/**
 * @brief Calculate the H(A) as per the HTTP Digest Specification.
 *
 */
JC_RETCODE jdi_DigestCalcHA1 (const JC_UINT8			*pucAlg,
							  const JC_UINT8			*pucUserName,
							  const JC_UINT8			*pucRealm,
							  const JC_UINT8			*pucPassword,
							  const JC_UINT8			*pucNonce,
							  const JC_UINT8			*pucCNonce,
							  HASH_STRING_HEX_VALUE		aucSessionKey) ;

/**
 * @brief Calculate request-digest/response-digest as per HTTP Digest spec 
 *
 * @param[in] aucSessionKey Specifies the HA1 digest value. 
 * @param[in] pucNonce Specifies Nonce value from the server.
 * @param[in] pucNonceCount Specifies the 8 digit hex value.
 * @param[in] pucCNonce Client Nonce value.
 * @param[in] pucQop qop-value: "", "auth", "auth-int".
 * @param[in] pucMethod Method from the request.
 * @param[in] pucDigestURI Request methoed 
 * @param[in] aucEntityKey Entity Body; if qop="auth-int".
 * @param[in] aucRespDigest Request digest or Response digest.
 */
JC_RETCODE jdi_DigestCalcResponse (const HASH_STRING_HEX_VALUE		aucSessionKey, 
								   const JC_UINT8					*pucNonce,  
								   const JC_UINT8					*pucNonceCount,  
								   const JC_UINT8					*pucCNonce,  
								   const JC_UINT8					*pucQop, 
								   const JC_UINT8					*pucMethod, 
								   const JC_UINT8					*pucDigestURI, 
								   const HASH_STRING_HEX_VALUE		aucEntityKey, 
								   HASH_STRING_HEX_VALUE			aucRespDigest) ;


JC_RETCODE jdi_HMACSHA1 (JC_UINT8			*pucKey, 
						 JC_UINT32			uiKeyLen, 
						 JC_UINT8			*pucText, 
						 JC_UINT32			uiTextLen, 
						 JC_UINT8			aucDigest[SHA1_DIGEST_SIZE]) ;

JC_RETCODE jdi_HMACSHA1DigestHex (JC_UINT8		*pucKey,
								  JC_UINT32		uiKeyLen,
								  JC_UINT8		*pucText,
								  JC_UINT32		uiTextLen,
								  JC_UINT8		**ppucDigest,
								  JC_UINT32		*puiDigestLen) ;

/**
 * @brief This function encodes the given data in "base64 encoding"
 * @param[in]	vMemHandle		Handle that is used in memory operations
 * @param[in]	pucUnEncode		The data which has to be encoded.
 * @param[in]	iUnEncodeLen	Length of the data
 * @param[out]	ppcEncoded		Holds the encoded data
 * @param[out]	piEncodedLen	Holds the length of the encoded data
 * @retval	JC_OK						Successfully encoded the data.
 * @retval	JC_ERR_MEMORY_ALLOCATION	Fails to allocate memory
 * @note	The parameters pcUnEncode and ppcEncoded should not be NULL.
 *
 * This function is called by application when the given data has to be 
 * encoded using base64 encoding method.
 */
JC_RETCODE jdi_HashBase64Encode (JC_HANDLE		vMemHandle, 
								 const JC_UINT8	*pucUnEncode, 
								 JC_INT32		iUnEncodeLen,
								 JC_INT8		**ppcEncoded,
								 JC_INT32		*piEncodedLen) ;

/**
 * @brief This function decodes the given data which is in "base64 encoding"
 * @param[in]	pcEncodedBuf		The data which has to be decoded.
 * @param[in]	iEncodedBufLen		Length of the data
 * @param[out]	pucDecodedBuf		Holds the decoded data
 * @param[out]	piDecodedBufLen		Holds the length of the decoded data
 * @retval	JC_OK					Successfully decoded the data.
 * @note	The parameters pcEncodedBuf and pucDecodedBuf should not be NULL.
 *
 * This function is called by application when the given data that is in 
 * bease64encoded form has to be decoded.
 */
JC_RETCODE jdi_HashBase64Decode	(JC_INT8	*pcEncodedBuf,
								 JC_INT32	iEncodedBufLen,
								 JC_UINT8	*pucDecodedBuf,
								 JC_INT32	*piDecodedBufLen) ;

/**
 * @brief This function decodes the given unicode data which is in "base64 encoding"
 * @param[in]	pmEncodedBuf		The data which has to be decoded.
 * @param[in]	iEncodedBufLen		Length of the data
 * @param[out]	pucDecodedBuf		Holds the decoded data
 * @param[out]	piDecodedBufLen		Holds the length of the decoded data
 * @retval	JC_OK					Successfully decoded the data.
 */
JC_RETCODE jdi_HashBase64DecodeUnicode (JC_CHAR		*pmEncodedBuf,
										JC_INT32	iEncodedBufLen,
										JC_UINT8	*pucDecodedBuf,
										JC_INT32	*piDecodedBufLen) ;

/**
 * @brief This function decodes the given data which is in "Quoted Printable" encoded
 * @param[in]	pcEncodedBuf		The data which has to be decoded.
 * @param[in]	iEncodedBufLen		Length of the data
 * @param[in]	bIsWordEncoded		Specifies whether the Quoted printable is word encoded. 
 * @param[out]	pucDecodedBuf		Holds the decoded data
 * @param[out]	piDecodedBufLen		Holds the length of the decoded data
 * @retval	JC_OK					Successfully decoded the data.
 */
JC_RETCODE jdi_HashQuotedPrintableDecode	(JC_INT8		*pcEncodedBuf,
											 JC_INT32		iEncodedBufLen,
											 JC_BOOLEAN		bIsWordEncoded,
											 JC_UINT8		*pucDecodedBuf,
											 JC_INT32		*piDecodedBufLen) ;
#endif
/* End of File */

