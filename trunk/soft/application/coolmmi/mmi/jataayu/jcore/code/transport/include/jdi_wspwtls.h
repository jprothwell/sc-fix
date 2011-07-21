/***************************************************************************
 * $Id: jdi_wspwtls.h,v 1.2 2006/11/16 11:32:26 browserdevjtbase Exp $
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
 * $Log: jdi_wspwtls.h,v $
 * Revision 1.2  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.1  2006/05/02 11:22:19  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.6  2006/04/11 13:36:19  kumardevhtmlbrow
 * Lint Warning ...
 *
 * Revision 1.5  2006/03/16 07:35:41  kumardevhtmlbrow
 * Added the application to add the session header
 *
 * Revision 1.4  2006/03/04 11:52:30  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2006/03/01 14:57:58  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_wspwtls.h
 * @ingroup WAPStack
 * @brief Defines the error codes and enums that are used for using WTLS.
 *
 * This file defines the cipher suite and key exchange algorithms that are used for making WTLS connection. 
 */

#ifndef _JDI_WSPWTLS_H
#define _JDI_WSPWTLS_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WAP_WITH_WTLS
#ifdef CERTICOM_WTLS

/**
 * @brief The wap stack returns this error if the certificate data specified
 *		  by the application is invalid.
 */
#define JC_ERR_WTLS_INVALID_CERT_CONTENT        TERMINAL_ERR_CODE(WSP_MODULE, 0x41)
/**
 * @brief The wap stack returns this error if the type of certificate provided
 *		  by the application is invalid. 
 */
#define	JC_ERR_WTLS_INVALID_CERT_TYPE			TERMINAL_ERR_CODE(WSP_MODULE, 0x42)
/**
 * @brief The wap stack returns this error if an error occured while creating
 *		  the WTLS global context.
 */
#define	JC_ERR_WTLS_INIT_CREATE_GLOBAL_CONTEXT	TERMINAL_ERR_CODE(WSP_MODULE, 0x43)
/**
 * @brief The wap stack returns this error if an error occured while setting the WTLS time 
 *		  out value.
 */
#define	JC_ERR_WTLS_INIT_SET_TIMEOUT_VALUE		TERMINAL_ERR_CODE(WSP_MODULE, 0x44)
/**
 * @brief The wap stack returns this error if an error occured while setting the size of
 *		  WTLS input/output buffer
 */
#define	JC_ERR_WTLS_INIT_SET_IO_BUFF_SZ			TERMINAL_ERR_CODE(WSP_MODULE, 0x45)
/**
 * @brief The wap stack returns this error if an error occured while setting the read 
 *		  function for WTLS.
 */
#define	JC_ERR_WTLS_INIT_SET_READ_FUNC			TERMINAL_ERR_CODE(WSP_MODULE, 0x46)
/**
 * @brief The wap stack returns this error if an error occured while setting the write 
 *		  function for WTLS.
 */
#define	JC_ERR_WTLS_INIT_SET_WRITE_FUNC			TERMINAL_ERR_CODE(WSP_MODULE, 0x47)
/**
 * @brief The wap stack returns this error if an error occured while setting the WTLS 
 *		  key refresh rate
 */
#define	JC_ERR_WTLS_INIT_KEY_REFRESH_RATE		TERMINAL_ERR_CODE(WSP_MODULE, 0x48)
/**
 * @brief The wap stack returns this error if the cipher suites specified by the 
 *		  application is invalid.
 */
#define	JC_ERR_WTLS_INVALID_CIPHERSUITES		TERMINAL_ERR_CODE(WSP_MODULE, 0x49)
/**
 * @brief The wap stack returns this error if the format of the certificates, provided by the 
 *		  application, is invalid.
 */
#define	JC_ERR_WTLS_INVALID_CERT_FORMAT			TERMINAL_ERR_CODE(WSP_MODULE, 0x4A)
/**
 * @brief The wap stack returns this error if the signature algoritm specified by the 
 *		  application is invalid.
 */
#define	JC_ERR_WTLS_INVALID_SIGNATURE_ALG		TERMINAL_ERR_CODE(WSP_MODULE, 0x4B)
/**
 * @brief The wap stack returns this error to specify that an error occured while
 *		  creating a linked list of certificates
 */
#define	JC_ERR_WTLS_CREATE_CERT_LIST			TERMINAL_ERR_CODE(WSP_MODULE, 0x4C)
/**
 * @brief The wap stack returns this error to specify that an error occured while
 *		  adding a trusted certificate.
 */
#define	JC_ERR_WTLS_ADD_TRUSTED_CERT			TERMINAL_ERR_CODE(WSP_MODULE, 0x4D)
/**
 * @brief The wap stack returns this error if the key algoritm specified by the 
 *		  application is invalid.
 */
#define	JC_ERR_WTLS_INVALID_KEY_ALG				TERMINAL_ERR_CODE(WSP_MODULE, 0x4E)
/**
 * @brief The wap stack returns this error to specify that an error occured while
 *		  adding a user certificate.
 */
#define	JC_ERR_WTLS_ADD_USER_CERTIFICATE		TERMINAL_ERR_CODE(WSP_MODULE, 0x4F)
/**
 * @brief The wap stack returns this error if an error occured while intializing WTLS
 */
#define	JC_ERR_WTLS_INIT						TERMINAL_ERR_CODE(WSP_MODULE, 0x50)
/**
 * @brief The wap stack returns this error if the handshaking has failed. 
 */
#define	JC_ERR_WTLS_HANDSHAKE					TERMINAL_ERR_CODE(WSP_MODULE, 0x51)


/**
 * @brief This enumeration defines the list of ciphers suites that are provided by the 
 *		  WTLS module. 
 * @note  This enumeration can be used only if the flag WAP_WITH_WTLS is enabled.
 *
 * The cipher suite specifies the encryption algorithm (sometimes of a particular strength), 
 * the MAC algorithm, and the MAC algorithm bit strength. The MAC algorithm is used in full 
 * strength mode during the handshake, the strength refers to the number of bits retained
 * to guarantee the integrity of application data.
 * Encryption Algorithm are DES, RC5 (two strength), Triple DES.
 * MAC (Message integrity check) algorithms are SHA and MD5, in a variety of strengths.
 */
typedef enum e_wtls_cipher_suites 
{
	E_WTLS_ALG_CIPHER_EMPTY, /**< Used internally */
	E_WTLS_ALG_CIPHER_DES_SHA_0, /**< DES encryption algorithm with zero bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_3DES_SHA_0, /**< Triple-DES encryption algorithm with zero bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_DES_SHA_40, /**< DES encryption algorithm with 40 bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_3DES_SHA_40, /**< Triple-DES encryption algorithm with 40 bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_DES_SHA_80, /**< DES encryption algorithm with 80 bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_3DES_SHA_80, /**< Triple-DES encryption algorithm with 80 bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_DES_SHA_160, /**< DES encryption algorithm with 160 bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_3DES_SHA_160, /**< Triple-DES encryption algorithm with zero bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_DES_MD5_40, /**< DES encryption algorithm with 40 bit strength of MD5-MAC. */ 
	E_WTLS_ALG_CIPHER_3DES_MD5_40, /**< Triple-DES encryption algorithm with 40 bit strength of MD5-MAC. */ 
	E_WTLS_ALG_CIPHER_DES_MD5_80, /**< DES encryption algorithm with 80 bit strength of MD5-MAC. */ 
	E_WTLS_ALG_CIPHER_3DES_MD5_80, /**< Triple-DES encryption algorithm with 80 bit strength of MD5-MAC. */ 
	E_WTLS_ALG_CIPHER_DES_MD5_128, /**< DES encryption algorithm with 128 bit strength of MD5-MAC. */ 
	E_WTLS_ALG_CIPHER_3DES_MD5_128, /**< Triple-DES encryption algorithm with 128 bit strength of MD5-MAC. */ 
	E_WTLS_ALG_CIPHER_RC5_56_SHA_0, /**< RC5-56 encryption algorithm with zero bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_RC5_56_SHA_40, /**< RC5-56 encryption algorithm with 40 bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_RC5_56_SHA_80, /**< RC5-56 encryption algorithm with 80 bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_RC5_56_SHA_160, /**< RC5-56 encryption algorithm with 160 bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_RC5_128_SHA_0, /**< RC5-128 encryption algorithm with zero bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_RC5_128_SHA_40, /**< RC5-128 encryption algorithm with 40 bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_RC5_128_SHA_80, /**< RC5-128 encryption algorithm with 80 bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_RC5_128_SHA_160, /**< RC5-128 encryption algorithm with 160 bit strength of SHA-MAC. */ 
	E_WTLS_ALG_CIPHER_NONE /**< NULL encryption algorithm. */ 
} E_WTLSCipherSuites, E_WTLS_CIPHER_SUITE ;

/**
 * @brief This enumeration defines the list of key exchange algorithm that are provided by the 
 *		  WTLS module. 
 * @note  This enumeration can be used only if the flag WAP_WITH_WTLS is enabled.
 */
typedef enum e_wtls_key_exchange_algs 
{
	E_WTLS_ALG_KE_EMPTY, /**< Used internally */
	/* Class I */
	E_WTLS_ALG_KE_ECDH_3_ANON_CLN, /**< Elliptic Curve Diffie-Helman (163 bit) anonymous key exchange algorithm with no maximum bit-strength. Used for WTLS Class I transaction. */
	E_WTLS_ALG_KE_ECDH_5_ANON_CLN, /**< Elliptic Curve Diffie-Helman (163 bit) anonymous key exchange algorithm with no maximum bit-strength. Used for WTLS Class I transaction. */
	E_WTLS_ALG_KE_ECDH_7_ANON_CLN, /**< Elliptic Curve Diffie-Helman (160 bit) anonymous key exchange algorithm with no maximum bit-strength. Used for WTLS Class I transaction. */
	E_WTLS_ALG_KE_DH_1_ANON_CLN, /**< Diffie-Helman (512 bit) anonymous key exchange algorithm with no maximum bit-strength. Used for WTLS Class I transaction. */
	E_WTLS_ALG_KE_DH_1_ANON_512_CLN, /**< Diffie-Helman (512 bit) anonymous key exchange algorithm with 512 maximum bit-strength. Used for WTLS Class I transaction. */
	E_WTLS_ALG_KE_DH_1_ANON_768_CLN, /**< Diffie-Helman (512 bit) anonymous key exchange algorithm with 768 maximum bit-strength. Used for WTLS Class I transaction. */
	E_WTLS_ALG_KE_DH_2_ANON_CLN, /**< Diffie-Helman (768 bit) anonymous key exchange algorithm with no maximum bit-strength. Used for WTLS Class I transaction. */
	E_WTLS_ALG_KE_DH_2_ANON_768_CLN, /**< Diffie-Helman (768 bit) anonymous key exchange algorithm with 768 maximum bit-strength. Used for WTLS Class I transaction. */
	E_WTLS_ALG_KE_RSA_ANON_CLN, /**< RSA anonymous key exchange algorithm with no maximum bit-strength. Used for WTLS Class I transaction. */
	E_WTLS_ALG_KE_RSA_ANON_512_CLN, /**< RSA anonymous key exchange algorithm with 512 maximum bit-strength. Used for WTLS Class I transaction. */
	E_WTLS_ALG_KE_RSA_ANON_768_CLN, /**< RSA anonymous key exchange algorithm with 768 maximum bit-strength. Used for WTLS Class I transaction. */

	/* Class II */

	E_WTLS_ALG_KE_ECDH_3_AUTH_CLN_CLASS2, /**< Elliptic Curve Diffie-Helman (163 bit) authenticated key exchange algorithm with no maximum bit-strength. Used for WTLS Class II transaction. */
	E_WTLS_ALG_KE_ECDH_5_AUTH_CLN_CLASS2, /**< Elliptic Curve Diffie-Helman (163 bit) authenticated key exchange algorithm with no maximum bit-strength. Used for WTLS Class II transaction. */
	E_WTLS_ALG_KE_ECDH_7_AUTH_CLN_CLASS2, /**< Elliptic Curve Diffie-Helman (160 bit) authenticated key exchange algorithm with no maximum bit-strength. Used for WTLS Class II transaction. */
	E_WTLS_ALG_KE_RSA_AUTH_CLN_CLASS2, /**< RSA authenticated key exchange algorithm with no maximum bit-strength. Used for WTLS Class II transaction. */
	E_WTLS_ALG_KE_RSA_AUTH_512_CLN_CLASS2, /**< RSA authenticated key exchange algorithm with 512 maximum bit-strength. Used for WTLS Class II transaction. */
	E_WTLS_ALG_KE_RSA_AUTH_768_CLN_CLASS2, /**< RSA authenticated key exchange algorithm with 768 maximum bit-strength. Used for WTLS Class II transaction. */

	/* Class III */

	E_WTLS_ALG_KE_ECDH_3_AUTH_CLN_CLASS3, /**< Elliptic Curve Diffie-Helman (163 bit) authenticated key exchange algorithm with no maximum bit-strength. Used for WTLS Class III transaction. */
	E_WTLS_ALG_KE_ECDH_5_AUTH_CLN_CLASS3, /**< Elliptic Curve Diffie-Helman (163 bit) authenticated key exchange algorithm with no maximum bit-strength. Used for WTLS Class III transaction. */
	E_WTLS_ALG_KE_ECDH_7_AUTH_CLN_CLASS3, /**< Elliptic Curve Diffie-Helman (160 bit) authenticated key exchange algorithm with no maximum bit-strength. Used for WTLS Class III transaction. */
	E_WTLS_ALG_KE_RSA_AUTH_CLN_CLASS3, /**< RSA authenticated key exchange algorithm with no maximum bit-strength. Used for WTLS Class III transaction. */
	E_WTLS_ALG_KE_RSA_AUTH_512_CLN_CLASS3, /**< RSA authenticated key exchange algorithm with 512 maximum bit-strength. Used for WTLS Class III transaction. */
	E_WTLS_ALG_KE_RSA_AUTH_768_CLN_CLASS3, /**< RSA authenticated key exchange algorithm with 768 maximum bit-strength. Used for WTLS Class III transaction. */
	E_WTLS_ALG_KE_NONE /**< NULL key exchange algorithm */
} E_WTLSkeyExAlgs, E_WTLS_KEY_EXCHANGE_ALG ;

#endif
#endif

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */
