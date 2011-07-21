/***************************************************************************
 * $Id: jdi_wtls.h,v 1.5 2009/02/04 09:29:44 browserdevjtbase Exp $
 * $Revision: 1.5 $
 * $Date: 2009/02/04 09:29:44 $
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
 * $Log: jdi_wtls.h,v $
 * Revision 1.5  2009/02/04 09:29:44  browserdevjtbase
 * Core Updates
 *
 * Revision 1.4  2007/11/14 08:52:37  browserdevjtbase
 * core update.
 *
 * Revision 1.3  2007/09/27 14:52:23  browserdevjtbase
 * Core Update
 *
 * Revision 1.2  2006/11/29 11:51:16  browserdevjtbase
 * General Update
 *
 * Revision 1.1  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.1  2006/09/26 11:39:46  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.1  2006/09/25 11:24:43  kumardevjemulator
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

#ifndef _JDI_WTLS_H
#define _JDI_WTLS_H

#include <jdi_transportmodule.h>

/**
 * @file jdi_wtls.h
 * @brief Defines the data structures and API's for the WTLS support. 
 * 
 */
#define JC_ERR_WTLS_GLOBAL_CONTEXT				TERMINAL_ERR_CODE(WTLS_MODULE, 0x01)
#define JC_ERR_WTLS_GLOBAL_CONTEXT_PROTOCOL		TERMINAL_ERR_CODE(WTLS_MODULE, 0x02)
#define JC_ERR_WTLS_GLOBAL_CONTEXT_IOREAD		TERMINAL_ERR_CODE(WTLS_MODULE, 0x03)
#define JC_ERR_WTLS_GLOBAL_CONTEXT_IOWRITE		TERMINAL_ERR_CODE(WTLS_MODULE, 0x04)
#define JC_ERR_WTLS_GLOBAL_CONTEXT_TIMEOUT		TERMINAL_ERR_CODE(WTLS_MODULE, 0x05)
#define JC_ERR_WTLS_GLOBAL_CONTEXT_IOSIZE		TERMINAL_ERR_CODE(WTLS_MODULE, 0x06)
#define JC_ERR_WTLS_GLOBAL_CONTEXT_REFRESH		TERMINAL_ERR_CODE(WTLS_MODULE, 0x07)
#define JC_ERR_WTLS_GLOBAL_CONTEXT_CERT_FORMAT	TERMINAL_ERR_CODE(WTLS_MODULE, 0x08)
#define JC_ERR_WTLS_GLOBAL_CONTEXT_SIGN_ALG		TERMINAL_ERR_CODE(WTLS_MODULE, 0x09)
#define JC_ERR_WTLS_SET_CIPHERSUITES			TERMINAL_ERR_CODE(WTLS_MODULE, 0x0A)
#define JC_ERR_WTLS_TRUSTED_CERT				TERMINAL_ERR_CODE(WTLS_MODULE, 0x0B)
#define JC_ERR_WTLS_CERT_INVALID				TERMINAL_ERR_CODE(WTLS_MODULE, 0x0C)
#define JC_ERR_WTLS_SESSION_CONTEXT				TERMINAL_ERR_CODE(WTLS_MODULE, 0x0D)
#define JC_ERR_WTLS_HANDSHAKE					TERMINAL_ERR_CODE(WTLS_MODULE, 0x0E)
#define JC_ERR_WTLS_USER_CERT_EXCEEDED			TERMINAL_ERR_CODE(WTLS_MODULE, 0x0F)
#define JC_ERR_WTLS_USER_CERT					TERMINAL_ERR_CODE(WTLS_MODULE, 0x10)
#define JC_ERR_WTLS_INVALID_IP_ADDRESS			TERMINAL_ERR_CODE(WTLS_MODULE, 0x11)
#define JC_ERR_WTLS_NO_CONNECTION				TERMINAL_ERR_CODE(WTLS_MODULE, 0x12)
#define JC_ERR_WTLS_ABORT_CONNECTION			TERMINAL_ERR_CODE(WTLS_MODULE, 0x13)

typedef enum _wtls_trn_type
{
	E_WTLS_TRN_CLASS_1,
	E_WTLS_TRN_CLASS_2,
	E_WTLS_TRN_CLASS_3
} E_WTLS_TRN_TYPE ;

/** 
 * @brief Specifies the different callback type that the WTLS layer shall invoke to inform the
 * application layer.
 */
typedef enum _wtls_callback_type
{
	E_WTLS_NEGOTIATED_CIPHER_SUITES, /**< Specifies the negotiated cipher suites. Argument passed for this callback type is #ST_WTLS_NEGOTIATED_CIPHER_SUITES.*/
	E_WTLS_NEGOTIATED_KEY_EXCHANGE_ALG, /**< Specifies the negotiated key exchange algorithm. Argument passed for this callback type is #ST_WTLS_NEGOTIATED_KEY_EXCHANGE_ALG. */
	E_WTLS_SERVER_CERTIFICATE, /**< Specifies the server certificate that has been sent during the handshake. Application passed for this callback type is #ST_WTLS_SERVER_CERTIFICATE.*/
	E_WTLS_WRITE_CALLBACK, /**< Specifies the write callback that will be called by WTLS interface when lower wtls layer needs to write some data. The callback argument passed is of type #ST_WTLS_IO_DATA.*/
	E_WTLS_READ_CALLBACK /**< Specifies the write callback that will be called by WTLS interface when lower wtls layer needs to write some data. The callback argument passed is of type #ST_WTLS_IO_DATA.*/
} E_WTLS_CALLBACK_TYPE ;

/**
 * @brief Specifies the WTLS Cipher suite.
 */
typedef enum _e_wtls_cipher_suites
{
	E_WTLS_ALG_CIPHER_EMPTY = 0, /**< Used internally */
	E_WTLS_ALG_CIPHER_DES_SHA_0,
	E_WTLS_ALG_CIPHER_3DES_SHA_0,
	E_WTLS_ALG_CIPHER_DES_SHA_40,
	E_WTLS_ALG_CIPHER_3DES_SHA_40,
	E_WTLS_ALG_CIPHER_DES_SHA_80,
	E_WTLS_ALG_CIPHER_3DES_SHA_80,
	E_WTLS_ALG_CIPHER_DES_SHA_160,
	E_WTLS_ALG_CIPHER_3DES_SHA_160,
	E_WTLS_ALG_CIPHER_DES_MD5_40,
	E_WTLS_ALG_CIPHER_3DES_MD5_40,
	E_WTLS_ALG_CIPHER_DES_MD5_80,
	E_WTLS_ALG_CIPHER_3DES_MD5_80,
	E_WTLS_ALG_CIPHER_DES_MD5_128,
	E_WTLS_ALG_CIPHER_3DES_MD5_128,
	E_WTLS_ALG_CIPHER_RC5_56_SHA_0,
	E_WTLS_ALG_CIPHER_RC5_56_SHA_40,
	E_WTLS_ALG_CIPHER_RC5_56_SHA_80,
	E_WTLS_ALG_CIPHER_RC5_56_SHA_160,
	E_WTLS_ALG_CIPHER_RC5_128_SHA_0,
	E_WTLS_ALG_CIPHER_RC5_128_SHA_40,
	E_WTLS_ALG_CIPHER_RC5_128_SHA_80,
	E_WTLS_ALG_CIPHER_RC5_128_SHA_160,
	E_WTLS_ALG_MAX_CIPHER_SUITE
} E_WTLS_CIPHER_SUITE ;

/**
 * @brief Specifies the WTLS Key Exchange algorithm.
 */
typedef enum e_wtls_key_exchange_algs
{
	E_WTLS_ALG_KE_EMPTY = 0, /**< Used internally */
	/* Class I */

	E_WTLS_ALG_KE_RSA_ANON_CLN,
	E_WTLS_ALG_KE_RSA_ANON_512_CLN,
	E_WTLS_ALG_KE_RSA_ANON_768_CLN,
	E_WTLS_ALG_KE_ECDH_3_ANON_CLN,
	E_WTLS_ALG_KE_ECDH_5_ANON_CLN,
	E_WTLS_ALG_KE_ECDH_7_ANON_CLN,
	E_WTLS_ALG_KE_DH_1_ANON_CLN,
	E_WTLS_ALG_KE_DH_1_ANON_512_CLN,
	E_WTLS_ALG_KE_DH_1_ANON_768_CLN,
	E_WTLS_ALG_KE_DH_2_ANON_CLN,
	E_WTLS_ALG_KE_DH_2_ANON_768_CLN,
	
	/* Class II */

	E_WTLS_ALG_KE_ECDH_3_AUTH_CLN_CLASS2,
	E_WTLS_ALG_KE_ECDH_5_AUTH_CLN_CLASS2,
	E_WTLS_ALG_KE_ECDH_7_AUTH_CLN_CLASS2,
	E_WTLS_ALG_KE_RSA_AUTH_CLN_CLASS2,
	E_WTLS_ALG_KE_RSA_AUTH_512_CLN_CLASS2,
	E_WTLS_ALG_KE_RSA_AUTH_768_CLN_CLASS2,

	/* Class III */

	E_WTLS_ALG_KE_ECDH_3_AUTH_CLN_CLASS3,
	E_WTLS_ALG_KE_ECDH_5_AUTH_CLN_CLASS3,
	E_WTLS_ALG_KE_ECDH_7_AUTH_CLN_CLASS3,
	E_WTLS_ALG_KE_RSA_AUTH_CLN_CLASS3,
	E_WTLS_ALG_KE_RSA_AUTH_512_CLN_CLASS3,
	E_WTLS_ALG_KE_RSA_AUTH_768_CLN_CLASS3,
	E_WTLS_ALG_KE_MAX

} E_WTLS_KEY_EXCHANGE_ALG ;

/**
 * @brief Specifies the negotiated cipher suites
 */
typedef struct wtls_negotiated_cipher_suites
{
	E_WTLS_CIPHER_SUITE			eCipherSuite ; /**< Specifies the negotiated cipher suites. */
} ST_WTLS_NEGOTIATED_CIPHER_SUITES ;

/**
 * @brief Specifies the key exchange algorithm
 */
typedef struct wtls_negotiated_key_exchange_alg
{
	E_WTLS_KEY_EXCHANGE_ALG		eKeyAlgorithm ; /**< Specifies the negotiated key exchange algorithm. */
} ST_WTLS_NEGOTIATED_KEY_EXCHANGE_ALG ;

/**
 * @brief Specifies the server certificate details during handshake.
 */
typedef struct wtls_server_certificate
{
	JC_UINT8					*pucBuffer ; /**< Specifies the server certificate details that is exchanged during handshake. */
	JC_UINT32					uiLength ; /**< Specifies the length of the server certificate. */
} ST_WTLS_SERVER_CERTIFICATE ;

/**
 * @brief Specifies the structure used by read, write callbacks.
 */
typedef struct wtls_io_data
{
	JC_UINT8					*pucBuffer ; /**< Specifies the buffer that should be used for socket read and write operations. */
	JC_UINT32					uiLength ; /**< Specifies the length of the read, write buffer. */
	JC_UINT32					uiProcessed ; /**< Specifies the length of the buffer that is processed during read, write operations. */
} ST_WTLS_IO_DATA ;


/**
 * @brief Specifies the user certificate
 */
typedef struct _st_wtls_user_cert
{
	JC_UINT8				*pucCert ; /**< Specifies the user certificate. */
	JC_UINT32				uiCertSize ; /**< Specifies the size of the user certificate. */
	JC_UINT8				*pucPassword ; /**< Specifies the password of the user certificate. */
	JC_UINT8				*pucKey ; /**< Specifies the public key used in the user certificate. */
	JC_UINT32				uiKeySize ; /**< Specifies the size of the public key. */
} ST_WTLS_USER_CERT ;

/**
 * @brief Specifies the callback prototype which shall be used by the WTLS layer.
 * @param[in] pvArg Specifies the application argument, which is registered using #jdi_WTLSInitSessionCtxt.
 * @param[in] eCallbackType Specifies the different callback type.
 * @param[in] pvCallbackInfo Specifies the callback information. Please refer #E_WTLS_CALLBACK_TYPE for the information passed.
 */
typedef JC_RETCODE (* CB_WTLS_GENERAL_CALLBACK) (void					*pvArg,
												 E_WTLS_CALLBACK_TYPE 	eCallbackType,
												 void 					*pvCallbackInfo) ;

/**
 * @brief Initialize the WTLS global context.
 * @param[in] eWTLSTrnType Specifies the WTLS Transaction type.
 * @param[out] pvWTLSGlobalHandle Returns the global context reference.
 *
 * This function shall initialize the global context and register the required function
 * required by the lower security protocol layer.
 */
JC_RETCODE jdi_WTLSInitGlobalCtxt (E_WTLS_TRN_TYPE	eWTLSTrnType,
								   JC_HANDLE		*pvWTLSGlobalHandle) ;

/**
 * @brief Registers the required cipher suite.
 * @param[in] vWTLSGlobalHandle Specifies the reference that is returned from jdi_WTLSInitGlobalCtxt.
 * @param[in] peWTLSCipherSuite Specifies the list of cipher suite that needs to be registered as 
 *							   the supported cipher suite with the lower layer protocol layer.
 * @param[in] uhCipherSuiteCount Specifies the number of cipher suites that are getting registered in peWTLSCipherSuite.
 *
 * This function needs to convert the specified cipher suite to the protocol layer supported cipher suite.
 * If any of the cipher suite's are not supported can be ignored.
 */
JC_RETCODE jdi_WTLSSetCipherSuite (JC_HANDLE					vWTLSGlobalHandle,
								   const E_WTLS_CIPHER_SUITE	*peWTLSCipherSuite,
								   JC_UINT16					uhCipherSuiteCount) ;

/**
 * @brief Registers the required key exchange algorithm
 * @param[in] vWTLSGlobalHandle Specifies the reference that is returned from jdi_WTLSInitGlobalCtxt.
 * @param[in] peWTLSKeyExcAlg Specifies the list of key exchange algorithm that needs to be registered as 
 *							   the supported algorithm with the lower layer protocol layer.
 * @param[in] uhKeyAlgCount Specifies the number of key exchange algorithm that are getting registered in peWTLSKeyExcAlg.
 *
 * This function needs to convert the specified cipher suite to the protocol layer supported cipher suite.
 * If any of the cipher suite's are not supported can be ignored.
 */
JC_RETCODE jdi_WTLSSetKeyExchangeAlg (JC_HANDLE						vWTLSGlobalHandle,
									  const E_WTLS_KEY_EXCHANGE_ALG	*peWTLSKeyExcAlg,
									  JC_UINT16						uhKeyAlgCount) ;
/**
 * @brief Adds the root (trusted) certificate to the session context.
 * @param[in] vWTLSGlobalHandle Specifies the reference that is returned from #jdi_WTLSInitGlobalCtxt.
 * @param[in] pucCert Specifies the root certificate data.
 * @param[in] uiSize Specifies the pucCert size.
 * 
 * This function needs to pass the root certificate information to the security protocol layer.
 * This function will be called multiple times by the application layer (in this case it will be
 * WAPStack). 
 */
JC_RETCODE jdi_WTLSAddRootCertificate (JC_HANDLE		vWTLSGlobalHandle,
									   const JC_UINT8	*pucCert,
									   JC_UINT32		uiSize) ;

/**
 * @brief Adds the user certificate to the session context.
 * @param[in] vWTLSGlobalHandle Specifies the reference that is returned from #jdi_WTLSInitGlobalCtxt.
 * @param[in] pstUserCert Specifies the user certificate information, which contains user data, key and password.
 *
 * This function needs to pass the user certificate information to the security protocol layer. 
 * This function will be called multiple times by the application layer (in this case it will be 
 * WAPStack).
 */
JC_RETCODE jdi_WTLSAddUserCertificate (JC_HANDLE				vWTLSGlobalHandle,
									   const ST_WTLS_USER_CERT	*pstUserCert) ;

#ifdef JCF_WTLS_WITH_SOCKET_CALLS

/**
 * @brief Creates a socket.
 * @param[in] vWTLSGlobalHandle  Specifies the reference that is returned from #jdi_WTLSInitGlobalCtxt.
 * @param[in] uiConnID			Data Connection identifier
 * @param[in] pucGatewayAddress Specifies the WAP Gateway IP Address
 * @param[in] uhPortNumber		Specifies the port number where the WAP gateway is running.
 * @param[out] piSocket			Holds the socket id that is created
 * @retval	  JC_OK				On Success.
 * @retval	  Others			On Failure.
 *
 * This function shall create a socket.
 */
JC_RETCODE jdi_WTLSCreateSocket (JC_HANDLE					vWTLSGlobalHandle,
								 JC_UINT32					uiConnID, 
								 JC_UINT8					*pucGatewayAddress,
								 JC_UINT16					uhPortNumber,
								 JC_INT32					*piSocket) ;

/**
 * @brief Closes the given socket.
 * @param[in] vWTLSGlobalHandle  Specifies the reference that is returned from #jdi_WTLSInitGlobalCtxt.
 * @param[in] iSocket			Specifies the socket id.
 * @retval	  JC_OK				On Success.
 * @retval	  Others			On Failure.
 *
 * This function will close the given socket that is created using #jdi_WTLSCreateSocket.
 */
JC_RETCODE jdi_WTLSCloseSocket (JC_HANDLE					vWTLSGlobalHandle,
								JC_INT32					iSocket) ;

#endif

/**
 * @brief Creates the session context for the specified socket.
 * @param[in] vWTLSGlobalHandle Specifies the reference that is returned from #jdi_WTLSInitGlobalCtxt.
 * @param[in] iSocket Specifies the socket identifier for which the session context needs to be created.
 * @param[in] pvArg Specifies the application argument, which needs to be used when invoking cbWTLSCallback.
 * @param[in] cbWTLSCallback Specifies callback function, which needs to be used by the WTLS Layer for informing the handshake information.
 * @param[out] pvWTLSSessionHandle Returns the session context reference. 
 *
 * This function shall create the session context for the specified socket and this session
 * context is ready for the handshake. 
 */
JC_RETCODE jdi_WTLSInitSessionCtxt (JC_HANDLE					vWTLSGlobalHandle,
									JC_INT32					iSocket,
									void						*pvArg,
									CB_WTLS_GENERAL_CALLBACK	cbWTLSCallback,
									JC_HANDLE					*pvWTLSSessionHandle) ;

/** 
 * @brief Performs the handshake
 * @param[in] vWTLSSessionHandle Specifies the session context reference returned by #jdi_WTLSInitGlobalCtxt.
 *
 * This function shall perform the security negotiation with the origin server for the specified
 * session context and performs the security handshake.
 */
JC_RETCODE jdi_WTLSPerformHandshake (JC_HANDLE			vWTLSSessionHandle) ;

/** 
 * @brief Reads data from the session socket. 
 * @param[in] vWTLSSessionHandle Specifies the session context reference returned by #jdi_WTLSInitGlobalCtxt.
 * @param[in,out] pucBuffer Specifies the reference where the data needs to be read from the session 
 *							socket after decoding.
 * @param[in] uiLen Specifies the size of the pucBuffer (the maximum data that can be copied).
 * @param[out] puiRecvdBytes Returns the actual number of bytes copied to the pucBuffer.
 * 
 * This function shall internally invoke the protocol layer for reading the data from the 
 * session socket and returns the unencrypted data.
 */
JC_RETCODE jdi_WTLSReadFromSecureSocket (JC_HANDLE		vWTLSSessionHandle,
										 JC_UINT8		*pucBuffer,
										 JC_UINT32		uiLen,
										 JC_UINT32		*puiRecvdBytes) ;					

/** 
 * @brief Writes the data to the session socket.
 * @param[in] vWTLSSessionHandle Specifies the session context reference returned by #jdi_WTLSInitGlobalCtxt.
 * @param[in] pucBuffer Specifies the data that needs to be written to the socket after encryption.
 * @param[in] uiLen Specifies the size of the pucBuffer.
 * @param[out] puiSentBytes Returns the actual number of pucBuffer sent to the socket. 
 *
 * This function shall internally invoke the security protocol layer for writing the data 
 * to the secure socket. 
 */
JC_RETCODE jdi_WTLSWriteToSecureSocket (JC_HANDLE		vWTLSSessionHandle,
										const JC_UINT8	*pucBuffer,
										JC_UINT32		uiLen,
										JC_UINT32		*puiSentBytes) ;

/**
 * @brief Gets the extended WTLS error.
 * @param[in] vWTLSSessionHandle Specifies the session context reference returned by #jdi_WTLSInitGlobalCtxt.
 * @return	  Returns the handshake extended error.
 * 
 * This function will return the WTLS extended error if any. The extended error is received when 
 * handshake is successful but there is some extended error associated to the handshake.
 */
JC_INT32 jdi_WTLSGetHandshakeExtendedError (JC_HANDLE	vWTLSSessionHandle) ;

/**
 * @brief Deinitialize the session context.
 * @param[in] vWTLSGlobalHandle Specifies the reference that is returned from #jdi_WTLSInitGlobalCtxt.
 * @param[in] vWTLSSessionHandle Specifies the session context reference returned by #jdi_WTLSInitGlobalCtxt.
 * 
 * This function shall deinitialize the specified session context and close the connection 
 * with the other end.
 */
JC_RETCODE jdi_WTLSDeinitSessionCtxt (JC_HANDLE			vWTLSGlobalHandle,
									  JC_HANDLE			vWTLSSessionHandle) ;

/**
 * @brief Deinitialize the global context.
 * @param[in] vWTLSGlobalHandle Specifies the reference that is returned from #jdi_WTLSInitGlobalCtxt.
 *
 * This function shall deinitialize the global session context.
 */
JC_RETCODE jdi_WTLSDeinitGlobalCtxt (JC_HANDLE			vWTLSGlobalHandle) ;

#endif

/* End of File */

