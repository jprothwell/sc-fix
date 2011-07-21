/***************************************************************************
*
* File Name : jdi_httpssl.h
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
*
* File Description
* ---------------
*
* Purpose			:
*
*
*
* 
* Created By		: Mallikarjun
* Created Date		: 
*
* Current Revision	:
*
*
***************************************************************************
*
*
* Revision Details
* ----------------
* 
* 1. Modified By, Modified Date, Purpose
* 2. Modified By, Modified Date, Purpose
* 3. Modified By, Modified Date, Purpose
*
*
*
***************************************************************************/

#ifndef _JDI_HTTPSSL_H
#define _JDI_HTTPSSL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HTTP_WITH_SSL

#define JC_ERR_SSL_SHARED_LIB_NOT_OPEN				TERMINAL_ERR_CODE(HTTP_MODULE, 0x31)
#define JC_ERR_SSL_NULL_CONTEXT						TERMINAL_ERR_CODE(HTTP_MODULE, 0x32)
#define JC_ERR_SSL_OVER_FLOW						TERMINAL_ERR_CODE(HTTP_MODULE, 0x33)
#define JC_ERR_SSL_INIT								TERMINAL_ERR_CODE(HTTP_MODULE, 0x34)
#define JC_ERR_SSL_BAD_CERTIFICATE					TERMINAL_ERR_CODE(HTTP_MODULE, 0x35)
#define JC_ERR_SSL_UNKNOWN_STATE					TERMINAL_ERR_CODE(HTTP_MODULE, 0x36)
#define JC_ERR_SSL_INVALID_VERSION					TERMINAL_ERR_CODE(HTTP_MODULE, 0x37)
#define JC_ERR_SSL_NEGOTIATION						TERMINAL_ERR_CODE(HTTP_MODULE, 0x38)
#define JC_ERR_SSL_CRYPTO							TERMINAL_ERR_CODE(HTTP_MODULE, 0x39)
#define JC_ERR_SSL_BAD_FINISH_MSG					TERMINAL_ERR_CODE(HTTP_MODULE, 0x3A)
#define JC_ERR_SSL_HANDSHAKE						TERMINAL_ERR_CODE(HTTP_MODULE, 0x3B)
#define JC_ERR_SSL_WRITE_FAILURE					TERMINAL_ERR_CODE(HTTP_MODULE, 0x3C)
#define JC_ERR_SSL_INVALID_CERT_DATES				TERMINAL_ERR_CODE(HTTP_MODULE, 0x3D)
#define JC_ERR_SSL_BASIC_CONSTRAINTS				TERMINAL_ERR_CODE(HTTP_MODULE, 0x3E)
#define JC_ERR_SSL_CERT_KEY_USAGE					TERMINAL_ERR_CODE(HTTP_MODULE, 0x3F)
#define JC_ERR_SSL_UNKNOWN_CRITICAL_CERT_EXTN		TERMINAL_ERR_CODE(HTTP_MODULE, 0x40)
#define JC_ERR_SSL_CERT_SIGNATURE_INVALID			TERMINAL_ERR_CODE(HTTP_MODULE, 0x41)
#define JC_ERR_SSL_NON_MATCHING_CERT_CHAIN			TERMINAL_ERR_CODE(HTTP_MODULE, 0x42)
#define JC_ERR_SSL_UNKNOWN_CERT_CHAIN				TERMINAL_ERR_CODE(HTTP_MODULE, 0x43)
#define JC_ERR_SSL_NEGOTIATED_CIPHER_SUITE_CB		TERMINAL_ERR_CODE(HTTP_MODULE, 0x44)
#define JC_ERR_SSL_INIT_SESSION_DB					TERMINAL_ERR_CODE(HTTP_MODULE, 0x45)
#define JC_ERR_SSL_NEEDS_CIPHER_OR_CLIENTAUTH		TERMINAL_ERR_CODE(HTTP_MODULE, 0x46)
#define JC_ERR_SSL_NEEDS_PRNG						TERMINAL_ERR_CODE(HTTP_MODULE, 0x47)
#define JC_ERR_SSL_ADD_TRUSTED_CERT					TERMINAL_ERR_CODE(HTTP_MODULE, 0x48)
#define JC_ERR_SSL_ADD_USER_CERTIFICATE				TERMINAL_ERR_CODE(HTTP_MODULE, 0x49)
#define JC_ERR_SSL_BAD_LENGTH						TERMINAL_ERR_CODE(HTTP_MODULE, 0x4A)
#define JC_ERR_SSL_DER_BAD_ENCODING					TERMINAL_ERR_CODE(HTTP_MODULE, 0x4B)
#define JC_ERR_SSL_BASE64_BAD_ENCODING				TERMINAL_ERR_CODE(HTTP_MODULE, 0x4C)
#define JC_ERR_SSL_BAD_INDEX						TERMINAL_ERR_CODE(HTTP_MODULE, 0x4D)
#define JC_ERR_SSL_NULL_CB							TERMINAL_ERR_CODE(HTTP_MODULE, 0x4E)
#define JC_ERR_SSL_WARN_TRUSTED_EXPIRED				TERMINAL_ERR_CODE(HTTP_MODULE, 0x4F)
#define JC_ERR_SSL_INTERNAL							TERMINAL_ERR_CODE(HTTP_MODULE, 0x50)
#define	JC_ERR_SSL_UNDEFINED						TERMINAL_ERR_CODE(HTTP_MODULE, 0x51)
#define	JC_ERR_SSL_NO_PTR							TERMINAL_ERR_CODE(HTTP_MODULE, 0x52)
#define	JC_ERR_SSL_ILLEGAL_PARAM					TERMINAL_ERR_CODE(HTTP_MODULE, 0x53)
#define	JC_ERR_SSL_BAD_SIDE							TERMINAL_ERR_CODE(HTTP_MODULE, 0x54)

typedef enum ssl_cipher_suites 
{
	E_SSL_CS_RSA_WITH_RC4_128_SHA,
	E_SSL_CS_RSA_WITH_AES_256_CBC_SHA,
	E_SSL_CS_RSA_WITH_AES_128_CBC_SHA,
	E_SSL_CS_RSA_WITH_RC4_128_MD5,
	E_SSL_CS_RSA_WITH_3DES_EDE_CBC_SHA,
	E_SSL_CS_RSA_WITH_DES_CBC_SHA,
	E_SSL_CS_RSA_EXPORT_WITH_RC4_40_MD5,
	E_SSL_CS_RSA_EXPORT_WITH_DES40_CBC_SHA,
	E_SSL_CS_RSA_WITH_NULL_MD5,
									
	E_SSL_CS_ECDH_ECDSA_SECT163K1_WITH_RC4_128_SHA,
	E_SSL_CS_ECDH_ECDSA_SECT163K1_NULL_SHA,
									
	E_SSL_CS_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA,
	E_SSL_CS_DHE_DSS_WITH_DES_CBC_SHA,
	E_SSL_CS_DHE_DSS_WITH_3DES_EDE_CBC_SHA,
	E_SSL_CS_DHE_DSS_EXPORT1024_WITH_DES_CBC_SHA,
	E_SSL_CS_DHE_DSS_EXPORT1024_WITH_RC4_56_SHA,
	E_SSL_CS_DHE_DSS_WITH_RC4_128_SHA,
	E_SSL_CS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA,
	E_SSL_CS_DHE_RSA_WITH_DES_CBC_SHA,
	E_SSL_CS_DHE_RSA_WITH_3DES_EDE_CBC_SHA,
										
	E_SSL_CS_DH_ANON_EXPORT_WITH_DES40_CBC_SHA,
	E_SSL_CS_DH_ANON_EXPORT_WITH_RC4_40_MD5,
	E_SSL_CS_DH_ANON_WITH_RC4_128_MD5,
	E_SSL_CS_DH_ANON_WITH_DES_CBC_SHA,
	E_SSL_CS_DH_ANON_WITH_3DES_EDE_CBC_SHA,
	E_SSL_ALG_CIPHER_NONE
} E_SSL_CIPHER_SUITE, E_TLS_CIPHER_SUITES ;

typedef enum tls_crypto_types
{
	E_TPC_RSA_PUBLIC_KEY,
	E_TPC_RSA_PRIVATE_KEY,
	E_TPC_RSA_KEY_PAIR,

	E_TPC_DSA_PUBLIC_KEY,
	E_TPC_DSA_PRIVATE_KEY,
	E_TPC_DSA_KEY_PAIR,
		
	E_TPC_DSA,
	E_TPC_DSA_WITH_SHA1,

	E_TPC_RSA,
	E_TPC_RSA_PKCS1,
	E_TPC_RSA_WITH_MD5,
	E_TPC_RSA_WITH_SHA1,
	E_TPC_RSA_WITH_SHA1_RAW,	/* This type is used by WTLS for raw RSA signature */
	E_TPC_RSA_WITH_SHA1_PKCS1	/* This algorithm is used to sign WTLS content */
} E_TLS_CRYPTO_TYPES ;

/**
 * @brief This structure contains information about trusted certificates 
 *		  which will be used in the handshake process with a server.
 */
typedef struct ssl_trusted_cert_info
{
	JC_UINT8						*pucCert ;
	JC_UINT64						ulLen ;
	struct ssl_trusted_cert_info	*pstNext ;
} ST_SSL_TRUSTED_CERTIFICATE_INFO ;

#endif

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */

