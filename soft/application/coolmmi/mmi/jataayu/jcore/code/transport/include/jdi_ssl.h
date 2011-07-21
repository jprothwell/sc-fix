/***************************************************************************
 * $Id: jdi_ssl.h,v 1.7 2008/01/15 11:57:45 browserdevjtbase Exp $
 * $Revision: 1.7 $
 * $Date: 2008/01/15 11:57:45 $
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
 * $Log: jdi_ssl.h,v $
 * Revision 1.7  2008/01/15 11:57:45  browserdevjtbase
 * Updated the jtbase.
 *
 * Revision 1.11  2008/01/10 14:38:03  sriramdevhtmlbrow
 * Updated documentation
 *
 * Revision 1.10  2007/12/28 12:29:14  kumardevhtmlbrow
 * Doxygen comment updated.
 *
 * Revision 1.9  2007/09/06 15:03:36  kumardevhtmlbrow
 * SSL document update.
 *
 * Revision 1.8  2007/06/28 06:39:46  sriramdevhtmlbrow
 * Implementation of New SSL APIs and changes in HTTP to use those APIs
 *
 * Revision 1.7  2007/06/22 06:07:23  kumardevhtmlbrow
 * SSL Abstraction layer updated for creating the socket in the SSL Layer.
 *
 * Revision 1.6  2007/02/20 04:07:06  kumardevhtmlbrow
 * Error code update
 *
 * Revision 1.5  2006/12/20 09:01:30  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.4  2006/11/24 07:28:54  kumardevhtmlbrow
 * Lint Warning fix
 *
 * Revision 1.3  2006/10/19 06:13:21  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.2  2006/09/21 06:43:21  kumardevhtmlbrow
 * Update for generic SSL Layer
 *
 * Revision 1.1  2006/09/13 11:19:15  kumardevhtmlbrow
 * Update ...
 *
 *  
 ***************************************************************************/

#ifndef _JDI_SSL_H
#define _JDI_SSL_H
/**
 * @mainpage
 *
 * @version 1.0
 * SSL Interface Description
 *
 * This documentation specifies the SSL abstraction layer that shall be used by the 
 * HTTP Stack or any other application, which needs the SSL support.
 * 
 */

/**
 * @file jdi_ssl.h
 * @brief Defines the data structures and API's for the SSL support. 
 * 
 */

/** 
 * @brief Specifies the different callback type that the SSL layer shall invoke to inform the
 * application layer.
 */
typedef enum _ssl_callback_type
{
	E_SSL_NEGOTIATED_CIPHER_SUITES, /**< Specifies the negotiated cipher suites. Application passed for this callback type is #ST_SSL_NEGOTIATED_CIPHER_SUITES.*/
	E_SSL_SERVER_CERTIFICATE, /**< Specifies the server certificate that has been sent during the handshake. Application passed for this callback type is #ST_SSL_SERVER_CERTIFICATE.*/
	E_SSL_WRITE_CALLBACK, /**< Specifies the write callback that will be called by ssl interface when lower ssl layer needs to write some data. The callback argument passed is of type ST_SSL_IO_DATA.*/
	E_SSL_READ_CALLBACK /**< Specifies the write callback that will be called by ssl interface when lower ssl layer needs to write some data. The callback argument passed is of type ST_SSL_IO_DATA.*/
} E_SSL_CALLBACK_TYPE ;

/**
 * @brief Specifies the different cipher suites.
 */
typedef enum _e_ssl_cipher_suite
{
	E_SSL_CS_RSA_WITH_RC4_128_SHA, /**< RSA with RC4 128 bit SHA algorithm. */
	E_SSL_CS_RSA_WITH_AES_256_CBC_SHA, /**< RSA with AES 256 bit CBC SHA algorithm. */
	E_SSL_CS_RSA_WITH_AES_128_CBC_SHA, /**< RSA with AES 128 bit CBC SHA algorithm. */
	E_SSL_CS_RSA_WITH_RC4_128_MD5, /**< RSA with RC4 128 bit MD5 algorithm. */
	E_SSL_CS_RSA_WITH_3DES_EDE_CBC_SHA, /**< RSA with 3DES EDE CBC SHA algorithm. */
	E_SSL_CS_RSA_WITH_DES_CBC_SHA, /**< RSA with AES DES CBC SHA algorithm. */
	E_SSL_CS_RSA_EXPORT_WITH_RC4_40_MD5, /**< RSA Export with RC4 40 MD5 algorithm. */
	E_SSL_CS_RSA_EXPORT_WITH_DES40_CBC_SHA, /**< RSA EXPORT with DES 40 CBC SHA algorithm. */
	E_SSL_CS_RSA_WITH_NULL_MD5, /**< RSA WITH MD5 algorithm. */
									
	E_SSL_CS_ECDH_ECDSA_SECT163K1_WITH_RC4_128_SHA, /**< ECDH ECDSA SECT163K1 with RC4 128 bit SHA algorithm. */
	E_SSL_CS_ECDH_ECDSA_SECT163K1_NULL_SHA, /**< ECDH ECDSA SECT163K1 with SHA algorithm. */
									
	E_SSL_CS_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA, /**< DHE DSS EXPORT with DES 40 CBC SHA algorithm. */
	E_SSL_CS_DHE_DSS_WITH_DES_CBC_SHA, /**< DHE DSS with DES CBC SHA algorithm. */
	E_SSL_CS_DHE_DSS_WITH_3DES_EDE_CBC_SHA, /**< DHE DSS with 3DES EDE CBC SHA algorithm. */
	E_SSL_CS_DHE_DSS_EXPORT1024_WITH_DES_CBC_SHA, /**< DHE DSS EXPORT1024 with DES CBC SHA algorithm. */
	E_SSL_CS_DHE_DSS_EXPORT1024_WITH_RC4_56_SHA, /**< DHE DSS EXPORT1024 with RC4 56 bit SHA algorithm. */
	E_SSL_CS_DHE_DSS_WITH_RC4_128_SHA, /**< DHE DSS with RC4 128 SHA algorithm. */
	E_SSL_CS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA, /**< DHE RSA EXPORT with DES 40 CBC SHA algorithm. */
	E_SSL_CS_DHE_RSA_WITH_DES_CBC_SHA, /**< DHE RSA with DES CBC SHA algorithm. */
	E_SSL_CS_DHE_RSA_WITH_3DES_EDE_CBC_SHA, /**< DHE RSA with 3DES EDE CBC SHA algorithm. */
										
	E_SSL_CS_DH_ANON_EXPORT_WITH_DES40_CBC_SHA, /**< DH Anonymous EXPORT with DES 40 CBC SHA algorithm. */
	E_SSL_CS_DH_ANON_EXPORT_WITH_RC4_40_MD5, /**< DH Anonymous EXPORT with RC4 40 MD5 algorithm. */
	E_SSL_CS_DH_ANON_WITH_RC4_128_MD5, /**< DH Anonymous with RC4 128 bit MD5 algorithm. */
	E_SSL_CS_DH_ANON_WITH_DES_CBC_SHA, /**< DH Anonymous with DES CBC SHA algorithm. */
	E_SSL_CS_DH_ANON_WITH_3DES_EDE_CBC_SHA, /**< DH Anonymous with 3DES EDE CBC SHA algorithm. */
	E_SSL_ALG_CIPHER_NONE /**< Specifies the end of cipher suite list. */
} E_SSL_CIPHER_SUITE ;

/**
 * @brief Specifies the negotiated cipher suites
 */
typedef struct ssl_negotiated_cipher_suites
{
	E_SSL_CIPHER_SUITE			eCipherSuite ; /**< Specifies the negotiated cipher suites. */
} ST_SSL_NEGOTIATED_CIPHER_SUITES ;

/**
 * @brief Specifies the server certificate details during handshake.
 */
typedef struct ssl_server_certificate
{
	JC_UINT8					*pucBuffer ; /**< Specifies the server certificate details that is exchanged during handshake. */
	JC_UINT32					uiLength ; /**< Specifies the length of the server certificate. */
} ST_SSL_SERVER_CERTIFICATE ;

/**
 * @brief Specifies the structure used by read, write callbacks.
 */
typedef struct ssl_io_data
{
	JC_UINT8					*pucBuffer ; /**< Specifies the buffer that should be used for socket read and write operations. */
	JC_UINT32					uiLength ; /**< Specifies the length of the read, write buffer. */
	JC_UINT32					uiProcessed ; /**< Specifies the length of the buffer that is processed during read, write operations. */
} ST_SSL_IO_DATA ;

/**
 * @brief Specifies the callback prototype which shall be used by the SSL layer.
 * @param[in] pvArg Specifies the application argument, which is registered using #jdi_SSLInitSessionCtxt.
 * @param[in] eCallbackType Specifies the different callback type.
 * @param[in] pvCallbackInfo Specifies the callback information. Please refer E_SSL_CALLBACK_TYPE for the information passed.
 */
typedef JC_RETCODE (* CB_SSL_GENERAL_CALLBACK) (void					*pvArg,
												E_SSL_CALLBACK_TYPE 	eCallbackType,
												void 					*pvCallbackInfo) ;

/**
 * @brief Specifies the user certificate
 */
typedef struct _st_ssl_user_cert
{
	JC_UINT8				*pucCert ; /**< Specifies the user certificate. */
	JC_UINT32				uiCertSize ; /**< Specifies the size of the user certificate. */
	JC_UINT8				*pucPassword ; /**< Specifies the password of the user certificate. */
	JC_UINT8				*pucKey ; /**< Specifies the public key used in the user certificate. */
	JC_UINT32				uiKeySize ; /**< Specifies the size of the public key. */
} ST_SSL_USER_CERT ;

/**
 * @brief Specifies that there is an error in certificates list creation.
 */
#define JC_ERR_SSL_CREATE_CERT_LIST				TERMINAL_ERR_CODE (SSL_MODULE, 0x01)
/**
 * @brief Specifies that there is an error while adding a certificate.
 */
#define JC_ERR_SSL_ADD_CERTIFICATE				TERMINAL_ERR_CODE (SSL_MODULE, 0x02)
/**
 * @brief Specifies that there is an error while adding cipher suites.
 */
#define JC_ERR_SSL_SET_CIPHER_SUITE				TERMINAL_ERR_CODE (SSL_MODULE, 0x03)
/**
 * @brief Specifies that there is an error while creating a session.
 */
#define JC_ERR_SSL_SESSION_CREATION				TERMINAL_ERR_CODE (SSL_MODULE, 0x04)
/**
 * @brief Specifies that an unexpected data has been received from the server.
 */
#define JC_ERR_SSL_UNEXPECTED_MSG				TERMINAL_ERR_CODE (SSL_MODULE, 0x05)
/**
 * @brief Specifies that the verification of SSL Record Message Authentication Code failed.
 */
#define JC_ERR_SSL_BAD_MAC						TERMINAL_ERR_CODE (SSL_MODULE, 0x06)
/**
 * @brief Specifies that the decryption of SSL record failed.
 */
#define JC_ERR_SSL_DECRYPT_FAILED				TERMINAL_ERR_CODE (SSL_MODULE, 0x07)
/**
 * @brief Specifies unknown SSL record type.
 */
#define JC_ERR_SSL_UNKNOWN_RECORD				TERMINAL_ERR_CODE (SSL_MODULE, 0x08)
/**
 * @brief Specifies that an error received from application Input/Outpu callbacks.
 */
#define JC_ERR_SSL_IO							TERMINAL_ERR_CODE (SSL_MODULE, 0x09)
/**
 * @brief Specifies that fatal alert was received or sent.
 */
#define JC_ERR_SSL_FATAL_ALERT					TERMINAL_ERR_CODE (SSL_MODULE, 0x0A)
/**
 * @brief Specifies the general protocol error. May be caused by incorrectly formatted message.
 */
#define JC_ERR_SSL_PROTOCOL						TERMINAL_ERR_CODE (SSL_MODULE, 0x0B)
/**
 * @brief Specifies that the server is trying to resume a session with different session parameters.
 */
#define JC_ERR_SSL_RESUMABLE_SESSION			TERMINAL_ERR_CODE (SSL_MODULE, 0x0C)
/**
 * @brief Specifies that the session was closed.
 */
#define JC_ERR_SSL_SESSION_CLOSED				TERMINAL_ERR_CODE (SSL_MODULE, 0x0D)
/**
 * @brief Specifies that the session was closed gracefully.
 */
#define JC_ERR_SSL_SESSION_CLOSED_GRACEFUL		TERMINAL_ERR_CODE (SSL_MODULE, 0x0E)
/**
 * @brief Specifies that the global context has not been configured with a protocol.
 */
#define JC_ERR_SSL_BAD_CONTEXT					TERMINAL_ERR_CODE (SSL_MODULE, 0x0F)
/**
 * @brief Specifies that the handshake is already completed.
 */
#define JC_ERR_SSL_HANDSHAKE_ALREADY_COMPLETED	TERMINAL_ERR_CODE (SSL_MODULE, 0x10)
/**
 * @brief Specifies that a renegotiated handshake has been refused by the peer.
 */
#define JC_ERR_SSL_RENEGOTIATION_REFUSED		TERMINAL_ERR_CODE (SSL_MODULE, 0x11)
/**
 * @brief Specifies that renegotiation with the peer has not started yet. There is application 
 *		  data which must be processed with read API before the handshake can proceed.
 */
#define JC_ERR_SSL_READ_REQUIRED				TERMINAL_ERR_CODE (SSL_MODULE, 0x12)
/**
 * @brief Specifies that a renegotiation has been requested by the peer. 
*		  To proceed with renegotiation call handshake API. If read, write 
*		  or close API is called then the renegotiation will be refused.
 */
#define JC_ERR_SSL_RENEGOTIATION_REQUESTED		TERMINAL_ERR_CODE (SSL_MODULE, 0x13)
/**
 * @brief Specifies that hasdshake is required before proceeding with the current action.
 */
#define JC_ERR_SSL_HANDSHAKE_REQUIRED			TERMINAL_ERR_CODE (SSL_MODULE, 0x14)
/**
 * @brief Specifies that there is an error read operation.
 */
#define JC_ERR_SSL_READ_FAILURE					TERMINAL_ERR_CODE (SSL_MODULE, 0x15)
/**
 * @brief Specifies that the NULL callback was passed while setting callbacks.
 */
#define JC_ERR_SSL_NULL_CALLBACK				TERMINAL_ERR_CODE (SSL_MODULE, 0x16)
/**
 * @brief Specifies that initialization of global context failed.
 */
#define JC_ERR_SSL_INIT							TERMINAL_ERR_CODE (SSL_MODULE, 0x17)
/**
 * @brief Specifies that there is an error handshake process.
 */
#define JC_ERR_SSL_HANDSHAKE					TERMINAL_ERR_CODE (SSL_MODULE, 0x18)
 /**
 * @brief Specifies that there is an error in write operation.
 */
#define JC_ERR_SSL_WRITE_FAILURE				TERMINAL_ERR_CODE (SSL_MODULE, 0x19)
/**
 
 */
#define JC_ERR_SSL_OVER_FLOW					TERMINAL_ERR_CODE (SSL_MODULE, 0x1A)
/**
 * @brief Specifies that the buffer length is too small to hold the master secret.
 */
#define JC_ERR_SSL_BAD_LENGTH					TERMINAL_ERR_CODE (SSL_MODULE, 0x1B)
/**
 * @brief Specifies that before installing a private key, a ciphersuite or client authentication 
 *		  suite using the same key exchange algorithm (RSA vs ECC) must be installed first.
 */
#define JC_ERR_SSL_NEEDS_CIPHER_OR_CLIENTAUTH	TERMINAL_ERR_CODE (SSL_MODULE, 0x1C)
/**
 * @brief Specifies that Pseudo-Random Number Generator(PRNG) not set yet. 
 *		  A PRNG must be installed first by calling appropriate API.
 */
#define JC_ERR_SSL_NEEDS_PRNG					TERMINAL_ERR_CODE (SSL_MODULE, 0x1D)
/**
 * @brief Specifies that a certificate has incorrect DER encoding.
 */
#define JC_ERR_SSL_DER_BAD_ENCODING				TERMINAL_ERR_CODE (SSL_MODULE, 0x1E)
/**
 * @brief Specifies that a certificate has incorrect base64 encoding.
 */
#define JC_ERR_SSL_BASE64_BAD_ENCODING			TERMINAL_ERR_CODE (SSL_MODULE, 0x1F)
/**
 * @brief Specifies that the protocol set is wrong. Protocol side must be server when set from a client.
 */
#define JC_ERR_SSL_BAD_SIDE						TERMINAL_ERR_CODE (SSL_MODULE, 0x20)
/**
 * @brief Specifies that there is a negotiation error in handshake process.
 */
#define JC_ERR_SSL_NEGOTIATION					TERMINAL_ERR_CODE (SSL_MODULE, 0x21)
/**
 * @brief Specifies that the finished message is incorrect.
 */
#define JC_ERR_SSL_BAD_FINISH_MSG				TERMINAL_ERR_CODE (SSL_MODULE, 0x22)
/**
 * @brief Specifies that one of the trusted certificates has expired, 
 *		  but the trusted list is still installed in the global context.
 */
#define JC_ERR_SSL_WARN_TRUSTED_EXPIRED			TERMINAL_ERR_CODE (SSL_MODULE, 0x23)
/**
 * @brief Specifies the general error that will be returned incase of unknown errors.
 */
#define JC_ERR_SSL_UNKNOWN						TERMINAL_ERR_CODE (SSL_MODULE, 0x24)

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Initialize the SSL global context.
 * @param[out] pvSSLGlobalHandle Returns the global context reference.
 *
 * This function shall initialize the global context and register the required function
 * required by the lower security protocol layer.
 */
JC_RETCODE jdi_SSLInitGlobalCtxt (JC_HANDLE		*pvSSLGlobalHandle) ;

/**
 * @brief Registers the required cipher suite.
 * @param[in] vSSLGlobalHandle Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 * @param[in] peSSLCipherSuite Specifies the list of cipher suite that needs to be registered as 
 *							   the supported cipher suite with the lower layer protocol layer.
 * @param[in] uhCipherSuiteCount Specifies the number of cipher suites that are getting registered in peSSLCipherSuite.
 *
 * This function needs to convert the specified cipher suite to the protocol layer supported cipher suite.
 * If any of the cipher suite's are not supported can be ignored.
 */
JC_RETCODE jdi_SSLSetCipherSuite (JC_HANDLE					vSSLGlobalHandle,
								  const E_SSL_CIPHER_SUITE	*peSSLCipherSuite,
								  JC_UINT16					uhCipherSuiteCount) ;

/**
 * @brief Adds the root (trusted) certificate to the session context.
 * @param[in] vSSLGlobalHandle Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 * @param[in] pucCert Specifies the root certificate data.
 * @param[in] uiSize Specifies the pucCert size.
 * 
 * This function needs to pass the root certificate information to the security protocol layer.
 * This function will be called multiple times by the application layer (in this case it will be
 * HTTP Stack). 
 */
JC_RETCODE jdi_SSLAddRootCertificate (JC_HANDLE			vSSLGlobalHandle,
									  const JC_UINT8	*pucCert,
									  JC_UINT32			uiSize) ;

/**
 * @brief Adds the user certificate to the session context.
 * @param[in] vSSLGlobalHandle Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 * @param[in] pstUserCert Specifies the user certificate information, which contains user data, key and password.
 *
 * This function needs to pass the user certificate information to the security protocol layer. 
 * This function will be called multiple times by the application layer (in this case it will be 
 * HTTP Stack).
 */
JC_RETCODE jdi_SSLAddUserCertificate (JC_HANDLE					vSSLGlobalHandle,
									  const ST_SSL_USER_CERT	*pstUserCert) ;

#ifdef JCF_SSL_WITH_SOCKET_CALLS

/**
 * @brief Creates a socket.
 * @param[in] vSSLGlobalHandle  Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 * @param[out] piSocket			Holds the socket id that is created
 * @retval	  JC_OK				On Success.
 * @retval	  Others			On Failure.
 *
 * This function shall create a socket.
 */
JC_RETCODE jdi_SSLCreateSocket (JC_HANDLE					vSSLGlobalHandle,
								JC_INT32					*piSocket) ;

/**
 * @brief Connects the given socket.
 * @param[in] vSSLGlobalHandle  Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 * @param[in] iSocket			Specifies the socket id.
 * @param[in] pucHostIP			Specifies the server IP to which socket has to be connected.
 * @param[in] uhPortNumber		Specifies the port number on the server to which socket has to be connected.
 * @retval	  JC_OK				On Success.
 * @retval    JC_ERR_NET_WOULDBLOCK		If the connect operation is nonblocking.
 * @retval	  Others			On Failure.
 *
 * This function connects the given socket to the given server and port number. 
 * If it is a nonblocking call then status of the connection will be polled using 
 * the function #jdi_SSLConnectPoll.
 * Here pucHost can be a proxy server also, in which case tunnelling will happen 
 * using the function #jdi_SSLTunnelRequest.
 */
JC_RETCODE jdi_SSLConnect (JC_HANDLE						vSSLGlobalHandle,
						   JC_INT32							iSocket,
						   JC_UINT8							*pucHostIP,
						   JC_UINT16						uhPortNumber) ;

/**
 * @brief Polls for the status of the connect operation.
 * @param[in] vSSLGlobalHandle  Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 * @param[in] iSocket			Specifies the socket id.
 * @retval	  JC_OK				On Success.
 * @retval    JC_ERR_NET_SOCKET_NOT_READY	If the socket is not yet connected.
 * @retval	  Others			On Failure.
 *
 * This function shall poll for the status of the connect operation done by 
 * the function #jdi_SSLConnect.
 */
JC_RETCODE jdi_SSLConnectPoll (JC_HANDLE					vSSLGlobalHandle,
							   JC_INT32						iSocket) ;

/**
 * @brief Sends a tunnel request to the proxy server.
 * @param[in] vSSLGlobalHandle  Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 * @param[in] iSocket			Specifies the socket id.
 * @param[in] pucBuffer			Specifies the tunnel request buffer.
 * @param[in] iLength			Specifies the length of the tunnel request buffer.
 * @param[in,out] puiSentBytes	Holds the no of bytes sent successfully.
 * @retval	  JC_OK				On Success.
 * @retval    JC_ERR_NET_WOULDBLOCK		If the send operation is nonblocking.
 * @retval	  Others			On Failure.
 *
 * This function will be called only if proxy server is used. This function will send 
 * a tunnel request to the proxy server. After this response for this tunnel request 
 * will be received using the function jdi_SSLTunnelResponse .
 */
JC_RETCODE jdi_SSLTunnelRequest (JC_HANDLE					vSSLGlobalHandle,
								 JC_INT32					iSocket,
								 JC_UINT8					*pucBuffer,
								 JC_INT32					iLength,
								 JC_UINT32					*puiSentBytes) ;

/**
 * @brief Gets tunnel response from the proxy server.
 * @param[in] vSSLGlobalHandle  Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 * @param[in] iSocket			Specifies the socket id.
 * @param[in] pucBuffer			Specifies the tunnel request buffer.
 * @param[in] iLength			Specifies the actual length of the buffer pucBuffer.
 * @param[in] puiRecvdBytes		Specifies the length of the response buffer that is stored in pucBuffer.
 * @retval	  JC_OK				On Success.
 * @retval    JC_ERR_NET_WOULDBLOCK		If the receive operation is nonblocking.
 * @retval	  Others			On Failure.
 *
 * This function will be called only if the function jdi_SSLTunnelRequest has 
 * already been called. This function will receive the tunnel response 
 * from the proxy server. If this tunnel process is success furthur communication 
 * will happen directly with the origin server.
 */
JC_RETCODE jdi_SSLTunnelResponse (JC_HANDLE					vSSLGlobalHandle,
								  JC_INT32					iSocket,
								  JC_UINT8					*pucBuffer,
								  JC_INT32					iLength,
								  JC_UINT32					*puiRecvdBytes) ;

/**
 * @brief Closes the given socket.
 * @param[in] vSSLGlobalHandle  Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 * @param[in] iSocket			Specifies the socket id.
 * @retval	  JC_OK				On Success.
 * @retval	  Others			On Failure.
 *
 * This function will close the given socket that is created using #jdi_SSLCreateSocket.
 */
JC_RETCODE jdi_SSLCloseSocket (JC_HANDLE					vSSLGlobalHandle,
							   JC_INT32						iSocket) ;

#endif

/**
 * @brief Creates the session context for the specified socket.
 * @param[in] vSSLGlobalHandle Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 * @param[in] pucHost Specifies the server with which secure communication has to be done. Even if the 
 *			  proxy server is used this will specify the server in the request.
 * @param[in] uhPortNumber Specifies the port number
 * @param[in] iSocket Specifies the socket identifier for which the session context needs to be created.
 * @param[in] pvArg Specifies the application argument, which needs to be used when invoking cbSSLCallback.
 * @param[in] cbSSLCallback Specifies callback function, which needs to be used by the SSL Layer for informing the handshake information.
 * @param[out] pvSSLSessionHandle Returns the session context reference. 
 *
 * This function shall create the session context for the specified socket and this session
 * context is ready for the handshake. 
 */
JC_RETCODE jdi_SSLInitSessionCtxt (JC_HANDLE				vSSLGlobalHandle,
								   JC_UINT8					*pucHost,
								   JC_UINT16				uhPortNumber,
								   JC_INT32					iSocket,
								   void						*pvArg,
								   CB_SSL_GENERAL_CALLBACK	cbSSLCallback,
								   JC_HANDLE				*pvSSLSessionHandle) ;

/** 
 * @brief Performs the handshake
 * @param[in] vSSLSessionHandle Specifies the session context reference returned by #jdi_SSLInitSessionCtxt.
 *
 * This function shall perform the security negotiation with the origin server for the specified
 * session context and performs the security handshake.
 */
JC_RETCODE jdi_SSLPerformHandshake (JC_HANDLE			vSSLSessionHandle) ;

/** 
 * @brief Reads data from the session socket. 
 * @param[in] vSSLSessionHandle Specifies the session context reference returned by #jdi_SSLInitSessionCtxt.
 * @param[in,out] pucBuffer Specifies the reference where the data needs to be read from the session 
 *							socket after decoding.
 * @param[in] uiLen Specifies the size of the pucBuffer (the maximum data that can be copied).
 * @param[out] puiRecvdBytes Returns the actual number of bytes copied to the pucBuffer.
 * 
 * This function shall internally invoke the protocol layer for reading the data from the 
 * session socket and returns the unencrypted data.
 */
JC_RETCODE jdi_SSLReadFromSecureSocket (JC_HANDLE		vSSLSessionHandle,
										JC_UINT8		*pucBuffer,
										JC_UINT32		uiLen,
										JC_UINT32		*puiRecvdBytes) ;					

/** 
 * @brief Writes the data to the session socket.
 * @param[in] vSSLSessionHandle Specifies the session context reference returned by #jdi_SSLInitSessionCtxt.
 * @param[in] pucBuffer Specifies the data that needs to be written to the socket after encryption.
 * @param[in] uiLen Specifies the size of the pucBuffer.
 * @param[out] puiSentBytes Returns the actual number of pucBuffer sent to the socket. 
 *
 * This function shall internally invoke the security protocol layer for writing the data 
 * to the secure socket. 
 */
JC_RETCODE jdi_SSLWriteToSecureSocket (JC_HANDLE		vSSLSessionHandle,
									   const JC_UINT8	*pucBuffer,
									   JC_UINT32		uiLen,
									   JC_UINT32		*puiSentBytes) ;

/**
 * @brief Checks whether any data is queued up at the SSL layer, before sending further data
 * to the SSL.
 * @param[in] vSSLSessionHandle Specifies the session context reference returned by #jdi_SSLInitSessionCtxt.
 * This function should send any data that is available in the SSL layer queue. If there are data in 
 * queue appropriate error needs to be returned (like wouldblock.)
 */
JC_RETCODE jdi_SSLFlushSecureSocket (JC_HANDLE			vSSLSessionHandle) ;

/**
 * @brief Deinitialize the session context.
 * @param[in] vSSLGlobalHandle Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 * @param[in] vSSLSessionHandle Specifies the session context reference returned by #jdi_SSLInitSessionCtxt.
 * 
 * This function shall deinitialize the specified session context and close the connection 
 * with the other end.
 */
JC_RETCODE jdi_SSLDeinitSessionCtxt (const JC_HANDLE	vSSLGlobalHandle,
									 JC_HANDLE			vSSLSessionHandle) ;

/**
 * @brief Deinitialize the global context.
 * @param[in] vSSLGlobalHandle Specifies the reference that is returned from #jdi_SSLInitGlobalCtxt.
 *
 * This function shall deinitialize the global session context.
 */
JC_RETCODE jdi_SSLDeinitGlobalCtxt (JC_HANDLE			vSSLGlobalHandle) ;

#ifdef __cplusplus
}
#endif

#endif

/* End of File */


