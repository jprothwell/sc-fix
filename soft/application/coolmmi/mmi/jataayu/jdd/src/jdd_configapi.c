#define MODULE_NAME "JDD_CONFIG"
#define FILE_NAME "jdd_configapi.c"
/***************************************************************************
 *
 * File Name jdd_configapi.c
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
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
 ***************************************************************************/
 /** 
 * @file    jdd_configapi.c
 * @ingroup jdd_configapi
 * @brief   Describes config related API's.
 *
 **/
 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
  
 /***************************************************************************
 * User Include Files
 **************************************************************************/
# include <ddl.h>
# include <jcal.h>
# include <jdd_configapi.h>

 /***************************************************************************  
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					OTHER_MODULE
#else
#define __MODULE_ID__					0
#endif


#define KTOUCH_HEADERS
#define KTOUCH_MMS_HEADERS

#define		BROWSER_RDF_LOCATION_STRING     "http://nds1.nds.nokia.com/uaprof/N6230r200.xml"
#define		MMS_RDF_LOCATION_STRING         "http://nds1.nds.nokia.com/uaprof/N6230r200.xml"


#ifdef KTOUCH_HEADERS
//#define		BROWSER_USER_AGENT_STRING      "Nokia7610/2.0 (5.0509.0) SymbianOS/7.0s Series60/2.1 Profile/MIDP-2.0 Configuration/CLDC-1.0"
#define		BROWSER_USER_AGENT_STRING       "TIANYU-KTOUCH/A930/Screen-240X320"


#else
#define		BROWSER_USER_AGENT_STRING       "Samsung SGH-BC01"
#endif


#ifdef KTOUCH_MMS_HEADERS
#define		MMS_USER_AGENT_STRING           "Nokia7610/2.0 (5.0509.0) SymbianOS/7.0s Series60/2.1 Profile/MIDP-2.0 Configuration/CLDC-1.0"
#else
#define		MMS_USER_AGENT_STRING           "Samsung SGH-BC01"
#endif

#define		MAX_CLIENT_SDU_SIZE				(130 * 1024)
#define		MAX_SERVER_SDU_SIZE				(130 * 1024)
#define		MAX_CLIENT_MESSAGE_SIZE			(-1)
#define		MAX_SERVER_MESSAGE_SIZE			(-1)
#define		MAX_OS_REQUEST					(4)
#define		MAX_PUSH_OS_REQUEST				(-1)
#define		MAX_SOCKET_ALIVE_TIME			(300)
#define		MAX_SOCKET_SELECT_TIMEOUT		(1000)
//#define	MAX_HTTP_CONNECTIONS			(10)
#define		MAX_HTTP_CONNECTIONS			(1)


#define		MAX_TRANSPORT_POLLING_TIME		(0)

//#define		WSP_GET_SEND_PDU_SIZE			(2800)
//#define		WSP_GET_SEND_PDU_SIZE			(1024) //modify 2010-09-17
//#define		WSP_GET_RECV_PDU_SIZE			(2800)

#define		WSP_GET_SEND_PDU_SIZE			(1410) //modify 2010-09-21
#define		WSP_GET_RECV_PDU_SIZE			(2800)//modify 2010-09-21

#define		WTLS_CONTEXT_FILE_NAME			"/_wap_mms/wap_tempfiles/wtlscontext.dat"	

#define		DEFAULT_SOCKET_TCP_RECV_BUFFER	(1024 * 5)
#define		DEFAULT_USER_DATA_SIZE			(1024 * 2)

#define		MIN_TRANSPORT_TIMER_VALUE		(5000)
#define		MIN_PUSH_TIMER_VALUE			(500)

#define MAX_HTTP_RECV_QUEUE_SIZE                        (0)


 /***************************************************************************
 * Global Variable declarations and definitions
 **************************************************************************/


static const JC_INT8 * g_pcRootCertName [] =
{
	JC_NULL
} ;

/* K-TOUCH */


#ifdef KTOUCH_HEADERS
static const JC_INT8 * const g_pBrowserCharset [] = 
{
	"utf-8", 
	"utf-16", 
	"iso-8859-1", 
	"iso-10646-ucs-2", 
#ifdef __MMI_LANG_SM_CHINESE__	
	"GB2312", 
#endif	
	"windows-1252", 
	"us-ascii",
	JC_NULL
} ;
#else
static const JC_INT8 * const g_pBrowserCharset [] = 
{
	"windows-1252",
	"utf-8; q=0.99",
	"iso-8859-1; q=0.50",
	JC_NULL
} ;
#endif



static JC_INT8 *g_pBrowserEncoding [] = 
{
#ifdef DEFLATE_ENABLE
	"deflate",
	"gzip",
#endif
	JC_NULL
} ;

#ifdef KTOUCH_HEADERS

static const JC_INT8 * const g_pBrowserContentType [] = 
{
	"application/xhtml+xml; profile=http://www.wapforum.org/xhtml",
	"application/vnd.wap.xhtml+xml", 
	"application/vnd.wap.wmlc", 
	"application/vnd.wap.wmlscriptc", 
	"text/vnd.wap.wml", 
	"image/vnd.wap.wbmp", 
	"image/gif", 
	"image/bmp", 
	"image/png", 
	"image/jpeg",
	"image/jpg",
	"audio/sp-midi",
	"audio/midi", 
	"audio/wav",
	"audio/x-wav",
	//"audio/mpeg",
	"audio/aac",
	"audio/mp3",
	"audio/amr",
	//"video/mpeg",
	"video/mp4",
	"video/3gpp",
	"video/avi",
	#ifdef JATAAYU_SUPPORT_SECURITY
	"application/vnd.wap.wtls-ca-certificate", 
	"application/x-x509-ca-cert", 
	"application/vnd.wap.hashed-certificate", 
	"application/vnd.wap.signed-certificate", 
	#endif
	"*/*", 
	"text/html",
	"text/css", 
	"text/plain",
	JC_NULL
} ;

#else

static const JC_INT8 * const g_pBrowserContentType [] = 
{
	"text/vnd.wap.wml; q=0.5",
	"application/xhtml+xml; q=0.99",
	"application/vnd.oma.dd+xml",
	"application/vnd.oma.dd2+xml",
	"application/vnd.wap.multipart.related", 
	"application/vnd.wap.multipart.mixed", 
	"application/vnd.wap.mms-message",
	"application/vnd.oma.drm.message",
	"multipart/*",
	"image/bmp",
	"image/gif",
	"image/jpeg",
	"image/vnd.wap.wbmp",
	"image/png",
	"text/css", 
	"text/plain",
	//"application/x-javascript",
	//"application/x-ecmascript",
	//"application/javascript",
	//"application/ecmascript",
	"application/wml+xml",
	"application/vnd.wap.wml+xml",
	"application/vnd.wap.xhtml+xml; q=0.99",
	"application/vnd.wap.wmlc",
	"application/vnd.wap.wmlscriptc",
	"application/vnd.wap.wtls-ca-certificate", 
	"application/x-x509-ca-cert",
	"application/vnd.wap.sic",
	"application/vnd.wap.slc",
	"application/vnd.wap.coc	",
	//"text/javascript",
	//"text/ecmascript",
	"text/vnd.wap.si",
	"text/vnd.wap.sl",
	"text/vnd.wap.co",
	"text/x-vCalendar", 
	"text/x-vCard", 
	//"application/java-archive", 
	//"text/vnd.sun.j2me.app-descriptor", 
	"audio/sp-midi",
	"audio/midi", 
	"audio/wav",
	"audio/aac",
	"audio/x-wav",
	"audio/mpeg",
	"video/mpeg",
	JC_NULL
} ;

#endif

static JC_INT8 *g_pBrowserCSSType [] = 
{
	"test/css",
	JC_NULL
} ;


#ifdef KTOUCH_HEADERS

static JC_INT8 *g_pBrowserImageType [] = 
{
	"image/bmp",
	"image/gif",
	"image/jpeg",
	"image/vnd.wap.wbmp",
	"image/png",
	JC_NULL
} ;

#else

static JC_INT8 *g_pBrowserImageType [] = 
{
	"image/gif",
	"*/*",
	JC_NULL
} ;

#endif	

static JC_INT8 *g_pBrowserAudioType [] = 
{
	JC_NULL
} ;

static JC_INT8 *g_pBrowserVideoType [] = 
{
	"video/avi",
	"video/mp4",
	"video/3gpp",	
	JC_NULL
} ;


#ifdef KTOUCH_HEADERS
static JC_INT8 *g_pBrowserLanguageType [] = 
{
#ifdef __MMI_LANG_TR_CHINESE__
	"zh-tw", 
#endif
#ifdef __MMI_LANG_SM_CHINESE__
	"zh-cn", 
#endif
	"en",
	JC_NULL
} ;

#else

static JC_INT8 *g_pBrowserLanguageType [] = 
{
	"en-us",
	"en;q=0.5",
	JC_NULL
} ;

#endif

#ifdef WAP_WITH_WTLS

static E_WTLS_CIPHER_SUITE g_aiWTLSCipherSuites [] = 
{
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
	0
} ;

/* Supported Key Exchange Algorithms */
static E_WTLS_KEY_EXCHANGE_ALG g_aiWTLSClass1KeyExAlgs [] = 
{
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
	0
} ;

static E_WTLS_KEY_EXCHANGE_ALG g_aiWTLSClass2KeyExAlgs [] = 
{
	/* Class II */
	E_WTLS_ALG_KE_RSA_AUTH_CLN_CLASS2,
	E_WTLS_ALG_KE_RSA_AUTH_512_CLN_CLASS2,
	E_WTLS_ALG_KE_RSA_AUTH_768_CLN_CLASS2,
	E_WTLS_ALG_KE_ECDH_3_AUTH_CLN_CLASS2,
	E_WTLS_ALG_KE_ECDH_5_AUTH_CLN_CLASS2,
	E_WTLS_ALG_KE_ECDH_7_AUTH_CLN_CLASS2,	
	0
} ;

static E_WTLS_KEY_EXCHANGE_ALG g_aiWTLSClass3KeyExAlgs [] = 
{
	/* Class III */
	E_WTLS_ALG_KE_RSA_AUTH_CLN_CLASS3,
	E_WTLS_ALG_KE_RSA_AUTH_512_CLN_CLASS3,
	E_WTLS_ALG_KE_RSA_AUTH_768_CLN_CLASS3,
	E_WTLS_ALG_KE_ECDH_3_AUTH_CLN_CLASS3,
	E_WTLS_ALG_KE_ECDH_5_AUTH_CLN_CLASS3,
	E_WTLS_ALG_KE_ECDH_7_AUTH_CLN_CLASS3,	
	0
} ;

#endif /* WAP_WITH_WTLS */

#ifdef HTTP_WITH_SSL
static E_SSL_CIPHER_SUITE g_aiSSLCipherSuites [] = 
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
	0
} ;
#endif /* HTTP_WITH_SSL */



/**
 * @brief	If application/platform/network operator supports any additional 
 *			headers,such information need to be provided for lower layers like 
 *			"HTTP" and "WSP".
 * 			This structure is intended to provide such information for all the 
 *			requests from transport.
 * @see		#jdd_CFGGetGenericHeader
 **/
typedef struct st_app_header
{
	JC_INT8 *pHeaderName ;
	JC_INT8 *pHeaderValue ;
} ST_APP_HEADER ;

static ST_APP_HEADER g_genericAppHeaders [] =
{
#if defined (UPGRADE_OMA_DRM_2)
	{"DRM-Version","2.0"},
#endif
	{JC_NULL, JC_NULL}
} ;

/***************************************************************************
 * Local Function Prototypes
 **************************************************************************/
 
 
 /***************************************************************************
 * All Local Function Definitions
 **************************************************************************/
 

/***************************************************************************
 * All Global Function Definitions
 **************************************************************************/

/**
 * @ingroup		jdd_configapi
 * @brief		This function initializes the CFG module.
 * @retval		JC_OK	Initialization process is successful
 * @see			#jdd_CFGDeInitialize
 *
 * The function is not used currently by core. Application can invoke this
 * to initialize any config resources if necessary.
 */
JC_RETCODE jdd_CFGInitialize (void) 
{
	JC_RETCODE	rCode = JC_ERR_NOT_IMPLEMENTED ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_CFGInitialize : Entry") ;
	
	/*--- Platform Specific Implementation Start ---*/
		
	/* Platform Specific Function for CFG Initialize */				
		
	rCode = JC_OK ;	

	/*--- Platform Specific Implementation End ---*/

	JDD_LOG_INFO0 ("jdd_CFGInitialize : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_configapi
 * @brief		This function deinitializes the CFG module.
 * @retval		JC_OK	deInitialization process is successful
 * @see			#jdd_CFGInitialize
 *
 * This function cleans up resources allocated by #jdd_CFGInitialize
 * The function is not used currently by core. 
 */
JC_RETCODE jdd_CFGDeInitialize (void)
{
	JC_RETCODE	rCode = JC_ERR_NOT_IMPLEMENTED ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_CFGDeInitialize : Entry") ;
	
	/*--- Platform Specific Implementation Start ---*/
		
	/* Platform Specific Function for CFG DeInitialize */				
		
	rCode = JC_OK ;	

	/*--- Platform Specific Implementation End ---*/

	JDD_LOG_INFO0 ("jdd_CFGDeInitialize : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the Location where RDF information
 *					is available.
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure that  
 *               		contains the task ID of the application
 * @param[out]		pcRDFLocation Holds the Location of RDF
 * @param[in,out]		puiLength Length of pcRDFLocation in bytes on input,
 *                	and the required length as output if the function
 *               		returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 * @retval			JC_OK if the Location of RDF is located successfully 
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY	 if the given memory for 
 *					pcRDFLocation is not sufficient.
 * @retval			JC_ERR_CFG_NOT_FOUND if the RDF location is not specified
 * @note				The parameters pcRDFLocation and puiLength should not
 *					be NULL
 *
 * This function will be called by transport to get the location of RDF.
 * This information will be sent to the server in the request buffer. If the 
 * given memory for pcRDFLocation is not sufficient then it stores the required
 * memory length in puiLength and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetRDFLocation (	JC_TASK_INFO *pstTaskInfo, \
									JC_INT8 *pcRDFLocation, \
									JC_UINT32 *puiLength)
{
	JC_RETCODE	rCode = JC_ERR_CFG_NOT_FOUND ;
	JC_UINT32 	uiRdfUrlLength ;

	JDD_LOG_INFO0 ("jdd_CFGGetRDFLocation : Entry") ;

	jdd_AssertOnFail ( (JC_NULL != pstTaskInfo) && \
						(JC_NULL != pcRDFLocation) && \
						(JC_NULL != puiLength), \
						"jdd_CFGGetRDFLocation : Input Parameter is NULL") ;
	
	if (E_TASK_MMS_UI == pstTaskInfo->iAppID )
	{
		uiRdfUrlLength = jc_strlen (MMS_RDF_LOCATION_STRING) ;
	}
	else
	{
		uiRdfUrlLength = jc_strlen (BROWSER_RDF_LOCATION_STRING) ;
	}

	if (uiRdfUrlLength < *puiLength)
	{
		if(E_TASK_MMS_UI == pstTaskInfo->iAppID )
		{
			jc_strcpy (pcRDFLocation, MMS_RDF_LOCATION_STRING) ;
		}
		else
		{
			jc_strcpy (pcRDFLocation, BROWSER_RDF_LOCATION_STRING) ;
		}
		rCode = JC_OK ;
	}
	else
	{
		*puiLength = uiRdfUrlLength ;
		rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
		JDD_LOG_INFO1 ("jdd_CFGGetRDFLocation: Returns [%d]", rCode) ;
	}	

	JDD_LOG_INFO0 ("jdd_CFGGetRDFLocation : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the User agent information
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure that 
 *               		contains the task ID of the application
 * @param[out]		pcUserAgent Holds the user agent information
 * @param[in,out]		puiLength Length of pcUserAgent in bytes on input,
 *                	and the required length as output if the function
 *               		returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 * @retval			JC_OK On Success
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY	 if the given memory for 
 *					pcUserAgent is not sufficient
 * @note				The parameters pcUserAgent and puiLength should not
 *					be NULL
 *
 * This function will be called by transport to get the user agent information.
 * This information will be sent to the server in the fetch request buffer. If 
 * the given memory for pcUserAgent is not sufficient then this function stores 
 * the required memory length in puiLength and returns 
 * JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetUserAgent (JC_TASK_INFO *pstTaskInfo, \
								JC_INT8 *pcUserAgent, \
								JC_UINT32 *puiLength)
{
	JC_RETCODE	rCode = JC_ERR_NOT_IMPLEMENTED ;
	JC_UINT32	uiUserAgentLength ;

	JDD_LOG_INFO0 ("jdd_CFGGetUserAgent : Entry") ;

	jdd_AssertOnFail ((JC_NULL != pstTaskInfo) && \
						(JC_NULL != pcUserAgent) && \
						(JC_NULL != puiLength), \
						"jdd_CFGGetUserAgent : Input Parameter is NULL") ;	
	
	if (E_TASK_MMS_UI == pstTaskInfo->iAppID)
	{
		uiUserAgentLength = jc_strlen (MMS_USER_AGENT_STRING) ;
	}
	else
	{
		uiUserAgentLength = jc_strlen (BROWSER_USER_AGENT_STRING) ;
	}

	if (uiUserAgentLength < *puiLength)
	{
		if (E_TASK_MMS_UI == pstTaskInfo->iAppID)
		{
			jc_strcpy (pcUserAgent, MMS_USER_AGENT_STRING) ;
		}
		else
		{
			jc_strcpy (pcUserAgent, BROWSER_USER_AGENT_STRING) ;
		}
		rCode = JC_OK ;
	}
	else
	{
		*puiLength = uiUserAgentLength ;
		rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
		JDD_LOG_INFO1 ("jdd_CFGGetUserAgent: Returns [%d]", rCode) ;
	}
	
	JDD_LOG_INFO0 ("jdd_CFGGetUserAgent : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the Accept charsets supported 
 *					by the application
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure that 
 *               		contains the task ID of the application
 * @param[out]		pcCharset Holds the accept charset
 * @param[in,out]		puiLength Length of pcCharset in bytes on input,
 *                	and the required length as output if the function
 *               		returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 * @param[in]		uiCharSetIndex	Specifies the index of the accept 
 *					charset in the list
 * @retval			JC_OK	On success
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY	 if the given memory for 
 *					pcCharset is not sufficient
 * @retval			JC_ERR_CFG_NOT_FOUND	if Accept charset is not found  
 *					in the list for the given index	
 * @note				The parameters pcCharset and puiLength should not 
 *					be NULL.
 *
 * This function will be called by transport to get the accept charsets
 * supported by the application. This information will be sent to the server 
 * in the fetch request buffer. If the given memory for pcCharset is not  
 * sufficient, then it stores the required memory length in puiLength and  
 * returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptCharset (JC_TASK_INFO *pstTaskInfo, \
									JC_INT8 *pcCharset, \
									JC_UINT32 *puiLength, \
									JC_UINT32 uiCharSetIndex)
{
	JC_RETCODE	rCode = JC_ERR_NOT_IMPLEMENTED ;
	JC_UINT32 	uiAcceptCharsetLength ;

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptCharset : Entry") ;

	jdd_AssertOnFail ((JC_NULL != pcCharset) && \
						(JC_NULL != puiLength), \
						"jdd_CFGGetAcceptCharset : Input Parameter is NULL") ;
		
	PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;	
		
	if (uiCharSetIndex < \
			( (sizeof(g_pBrowserCharset) / sizeof(JC_INT8 *)) - 1) )
	{
		uiAcceptCharsetLength = jc_strlen (g_pBrowserCharset [uiCharSetIndex]) ;
		if (uiAcceptCharsetLength < *puiLength)
		{
			jc_strcpy (pcCharset, g_pBrowserCharset[uiCharSetIndex]) ;
			rCode = JC_OK ;
		}
		else
		{
			*puiLength = uiAcceptCharsetLength ;
			rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
			JDD_LOG_INFO1 ("jdd_CFGGetAcceptCharset: Returns [%d]", rCode) ;
		}
	}
	else
	{
		rCode = JC_ERR_CFG_NOT_FOUND ;
		JDD_LOG_INFO1 ("jdd_CFGGetAcceptCharset: Returns [%d]", rCode) ;
	}

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptCharset : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the Accept content-types supported
 *					by the application
 * @param[in]		pstTaskInfo	Pointer to a JC_TASK_INFO structure that 
 *               		contains the task ID of the application
 * @param[out]		pcContentType Holds the accept contenttype
 * @param[in,out]		puiLength Length of pcContentType in bytes on input,
 *                	and the required length as output if the function
 *               		returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 * @param[in]		uiContentTypeIndex Specifies the index of the accept 
 *					contenttype in the list
 * @retval			JC_OK	On success
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY	The given memory for 
 *					pcContentType is not sufficient
 * @retval			JC_ERR_CFG_NOT_FOUND	Accept contenttype is not found 
 *					in the list for the given index	
 * @note				The parameters pcContentType and puiLength should not
 *					be NULL
 *
 * This function will be called by transport to get the accept contenttypes
 * supported by the application. This information will be sent to the server 
 * in the request buffer. If the given memory for pcContentType is not 
 * sufficient, then it stores the required memory length in puiLength and 
 * returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptContentType (JC_TASK_INFO *pstTaskInfo, \
										JC_INT8 *pcContentType, \
										JC_UINT32 *puiLength, \
										JC_UINT32 uiContentTypeIndex)
{
	JC_RETCODE	rCode = JC_ERR_NOT_IMPLEMENTED ;
	JC_UINT32 	uiAcceptContentTypeLength ;

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptContentType : Entry") ;

	jdd_AssertOnFail ((JC_NULL != pcContentType) && \
						(JC_NULL != puiLength), \
						"jdd_CFGGetAcceptContentType : \
						Input Parameter is NULL") ;

	PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;

	
	if (uiContentTypeIndex < ( \
			(sizeof(g_pBrowserContentType) / sizeof(JC_INT8 *)) - 1) )
	{
		uiAcceptContentTypeLength = jc_strlen ( \
						g_pBrowserContentType [uiContentTypeIndex]) ;
		if (uiAcceptContentTypeLength < *puiLength)
		{
			jc_strcpy (pcContentType, \
					g_pBrowserContentType [uiContentTypeIndex]) ;
			rCode = JC_OK ;
		}
		else
		{
			*puiLength = uiAcceptContentTypeLength ;
			rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
			JDD_LOG_INFO1 ("jdd_CFGGetAcceptContentType: Returns [%d]", rCode) ;

		}
	}	
	else
	{
	  rCode = JC_ERR_CFG_NOT_FOUND ;
	  JDD_LOG_INFO1 ("jdd_CFGGetAcceptContentType: Returns [%d]", rCode) ;
	}

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptContentType : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the Accept Content types supported  
 *					by the application for CSS content.
 * @param[in]		pstTaskInfo	Pointer to a JC_TASK_INFO structure that 
 *               		contains the task ID of the application
 * @param[out]		pcContentType Holds the accept content type
 * @param[in,out]		puiLength Length of pcContentType in bytes on input,	
 *                	and the required length as output if the function
 *               		returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 * @param[in]		uiContentTypeIndex	Specifies the index of the Accept
 *					Content type in the list
 * @retval			JC_OK	On Success
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY If the given memory for 
 *					pcContentType is not sufficient
 * @retval			JC_ERR_CFG_NOT_FOUND	If accept content type is not found 
 *					in the list for the given index	
 * @note				The parameters pcContentType and puiLength should 
 *					not be NULL.
 *
 * This function will be called by transport to get the accept contenttypes
 * supported by the application for CSS content. This information will be 
 * sent to the server in the request buffer. If the given memory for 
 * pcContentType is not sufficient, then it stores the required memory length 
 * in puiLength and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptContentTypeForCSSRequest ( \
												JC_TASK_INFO *pstTaskInfo, \
												JC_INT8 *pcContentType, \
												JC_UINT32 *puiLength, \
												JC_UINT32 uiContentTypeIndex)
{
	JC_RETCODE	rCode = JC_ERR_NOT_IMPLEMENTED ;
    JC_UINT32 	uiAcceptContentTypeLength ;

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptContentTypeForCSSRequest : Entry") ;

	jdd_AssertOnFail ((JC_NULL != pcContentType) && \
						(JC_NULL != puiLength), \
						"jdd_CFGGetAcceptContentTypeForCSSRequest : \
						Input Parameter is NULL") ;
    
    PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;

    
    if (uiContentTypeIndex < ( (sizeof(g_pBrowserCSSType) / sizeof(JC_INT8 *)) - 1) )
    {
        uiAcceptContentTypeLength = jc_strlen (g_pBrowserCSSType [uiContentTypeIndex]) ;
        if (uiAcceptContentTypeLength < *puiLength)
        {
            jc_strcpy (pcContentType, g_pBrowserCSSType [uiContentTypeIndex]) ;
            rCode = JC_OK ;
        }
        else
        {
            *puiLength = uiAcceptContentTypeLength ;
            rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
			JDD_LOG_INFO1 ("jdd_CFGGetAcceptContentTypeForCSSRequest: \
						Returns [%d]", rCode) ;
        }
    }
	else
	{
	  rCode = JC_ERR_CFG_NOT_FOUND ;
	  JDD_LOG_INFO1 ("jdd_CFGGetAcceptContentTypeForCSSRequest: \
					Returns [%d]", rCode) ;
	}

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptContentTypeForCSSRequest : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the Accept contenttypes supported
 *					by the application for image content.
 * @param[in]		pstTaskInfo  Pointer to a JC_TASK_INFO structure that 
 *               		contains the task ID of the application
 * @param[out]		pcContentType	Holds the accept contenttype
 * @param[in,out]		puiLength  Length of pcContentType in bytes on input,
 *                	and the required length as output if the function
 *               		returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 * @param[in]		uiContentTypeIndex  Specifies the index of the accept 
 *					contenttype in the list
 * @retval			JC_OK	On Success
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY	 If the given memory for 
 *					pcContentType is not sufficient
 * @retval			JC_ERR_CFG_NOT_FOUND	If accept content type is not found 
 *					in the list for the given index	
 * @note				The parameters pcContentType and puiLength should not 
 *					be NULL.
 *
 * This function will be called by transport to get the accept content types
 * supported by the application for image content. This information will be 
 * sent to the server in the request buffer. If the given memory for 
 * pcContentType is not sufficient then it stores the required memory length 
 * in puiLength and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptContentTypeForImageRequest ( \
												JC_TASK_INFO *pstTaskInfo, \
												JC_INT8 *pcContentType, \
												JC_UINT32 *puiLength, \
												JC_UINT32 uiContentTypeIndex)
{
	JC_RETCODE	rCode = JC_ERR_NOT_IMPLEMENTED ;
    JC_UINT32 	uiAcceptContentTypeLength ;

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptContentTypeForImageRequest : Entry") ;

	jdd_AssertOnFail ((JC_NULL != pcContentType) && \
						(JC_NULL != puiLength), \
						"jdd_CFGGetAcceptContentTypeForImageRequest : \
						Input Parameter is NULL") ; 
    
    PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;
    
    if (uiContentTypeIndex < ( (sizeof(g_pBrowserImageType) / sizeof(JC_INT8 *)) - 1) )
    {
        uiAcceptContentTypeLength = jc_strlen (g_pBrowserImageType [uiContentTypeIndex]) ;
        if (uiAcceptContentTypeLength < *puiLength)
        {
            jc_strcpy (pcContentType, g_pBrowserImageType [uiContentTypeIndex]) ;
            rCode = JC_OK ;
        }
        else
        {
            *puiLength = uiAcceptContentTypeLength ;
            rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
			JDD_LOG_INFO1 ("jdd_CFGGetAcceptContentTypeForImageRequest: \
							Returns [%d]", rCode) ;

        }
    }
	else
	{
		rCode = JC_ERR_CFG_NOT_FOUND ;
		JDD_LOG_INFO1 ("jdd_CFGGetAcceptContentTypeForImageRequest: \
						Returns [%d]", rCode) ;
	}

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptContentTypeForImageRequest : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the Accept content types supported
 *					by the application for audio content.
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure that 
 *               		contains the task ID of the application
 * @param[out]		pcContentType Holds the accept contenttype
 * @param[in,out]		puiLength Length of pcContentType in bytes on input,
 *                	and the required length as output if the function
 *               		returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 * @param[in]		uiContentTypeIndex Specifies the index of the accept
 *					content type in the list
 * @retval			JC_OK On Success
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY	 If the given memory for 
 *					pcContentType is not sufficient
 * @retval			JC_ERR_CFG_NOT_FOUND If Accept content type is not found 
 *					in the list for the given index	
 * @note				The parameters pcContentType and puiLength should not
 *					be NULL.
 *
 * This function will be called by transport to get the accept content types
 * supported by the application for audio content. This information will be 
 * sent to the server in the fetch request buffer. If the given memory for 
 * pcContentType is not sufficient, then it stores the required memory length 
 * in puiLength and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptContentTypeForAudioRequest ( \
												JC_TASK_INFO *pstTaskInfo, \
												JC_INT8 *pcContentType, \
												JC_UINT32 *puiLength, \
												JC_UINT32 uiContentTypeIndex)
{
	JC_RETCODE	rCode = JC_ERR_NOT_IMPLEMENTED ;
    JC_UINT32 	uiAcceptContentTypeLength ;

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptContentTypeForAudioRequest : Entry") ;

	jdd_AssertOnFail ((JC_NULL != pcContentType) && \
						(JC_NULL != puiLength), \
						"jdd_CFGGetAcceptContentTypeForAudioRequest : \
						Input Parameter is NULL") ;    
    
    PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;
      
    if (uiContentTypeIndex < \
			( (sizeof(g_pBrowserAudioType) / sizeof(JC_INT8 *)) - 1) )
    {
        uiAcceptContentTypeLength = jc_strlen ( \
						g_pBrowserAudioType [uiContentTypeIndex]) ;
        if (uiAcceptContentTypeLength < *puiLength)
        {
            jc_strcpy (pcContentType, \
							g_pBrowserAudioType [uiContentTypeIndex]) ;
            rCode = JC_OK ;
        }
        else
        {
            *puiLength = uiAcceptContentTypeLength ;
            rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
			JDD_LOG_INFO1 ("jdd_CFGGetAcceptContentTypeForAudioRequest: \
						Returns [%d]", rCode) ;
        }
    }
	else
	{
	  rCode = JC_ERR_CFG_NOT_FOUND ;
	  JDD_LOG_INFO1 ("jdd_CFGGetAcceptContentTypeForAudioRequest: \
						Returns [%d]", rCode) ;
	}

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptContentTypeForAudioRequest : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the Accept content types supported  
 *					by the application for video content.
 * @param[in]		pstTaskInfo  Pointer to a JC_TASK_INFO structure that 
 *               	contains the task ID of the application
 * @param[out]		pcContentType Holds the accept contenttype
 * @param[in, out]	*puiLength Length of pcContentType in bytes on input,
 *                	and the required length as output if the function
 *               	returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 * @param[in]		uiContentTypeIndex Specifies the index of the accept 
 *					content type in the list
 * @retval			JC_OK On Success
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY If the given memory for 
 *					pcContentType is not sufficient
 * @retval			JC_ERR_CFG_NOT_FOUND	If accept content type is not found 
 *					in the list for the given index	
 * @note			The parameters pcContentType and puiLength should not 
 *					be NULL.
 *
 * This function will be called by transport to get the accept content types
 * supported by the application for video content. This information will be 
 * sent to the server in the fetch request buffer. If the given memory for 
 * pcContentType is not sufficient, then it stores the required memory length 
 * in puiLength and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptContentTypeForVideoRequest ( \
												JC_TASK_INFO *pstTaskInfo, \
												JC_INT8 *pcContentType, \
												JC_UINT32 *puiLength, \
												JC_UINT32 uiContentTypeIndex)
{
	JC_RETCODE 	rCode = JC_ERR_NOT_IMPLEMENTED ;
    JC_UINT32 	uiAcceptContentTypeLength ;

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptContentTypeForVideoRequest : Entry") ;

	jdd_AssertOnFail ((JC_NULL != pcContentType) && \
						(JC_NULL != puiLength), \
						"jdd_CFGGetAcceptContentTypeForVideoRequest : \
						Input Parameter is NULL") ;    
    
    PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;
    
    if (uiContentTypeIndex < ( \
			(sizeof(g_pBrowserVideoType) / sizeof(JC_INT8 *)) - 1) )
    {
        uiAcceptContentTypeLength = jc_strlen ( \
					g_pBrowserVideoType [uiContentTypeIndex]) ;
        if (uiAcceptContentTypeLength < *puiLength)
        {
            jc_strcpy (pcContentType, \
						g_pBrowserVideoType [uiContentTypeIndex]) ;
            rCode = JC_OK ;
        }
        else
        {
            *puiLength = uiAcceptContentTypeLength ;
            rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
			JDD_LOG_INFO1 ("jdd_CFGGetAcceptContentTypeForVideoRequest: \
						Returns [%d]", rCode) ;
        }
    }
    else
    {
		rCode = JC_ERR_CFG_NOT_FOUND ;
		JDD_LOG_INFO1 ("jdd_CFGGetAcceptContentTypeForVideoRequest: \
						Returns [%d]", rCode) ;
    }

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptContentTypeForVideoRequest : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief	  		This function provides the Accept languages supported  
 *					by the application.
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure that 
 *               		contains the task ID of the application
 * @param[out]		pcLanguage Holds the accept language
 * @param[in, out]	puiLangLength Length of pcContentType in bytes on input,
 *                	and the required length as output if the function
 *               		returns JC_ERR_CFG_INSUFFICIENT_MEMORY	
 * @param[in]		uiLangTypeIndex Specifies the index of the accept 
 *					language in the list
 * @retval			JC_OK On Success
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY If the given memory for 
 *					pcLanguage is not sufficient
 * @retval			JC_ERR_CFG_NOT_FOUND If accept language is not found 
 *					in the list for the given index	
 * @note				The parameters pcLanguage and puiLangLength should 
 *					not be NULL.
 *
 * This function will be called by transport to get the accept languages
 * supported by the application. This information will be sent to the server 
 * in the fetch request buffer. If the given memory for pcLanguage is not  
 * sufficient, then it stores the required memory length in puiLangLength 
 * and returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptLanguage (JC_TASK_INFO *pstTaskInfo, \
									 JC_INT8 *pcLanguage, \
									 JC_UINT32 *puiLangLength, \
									 JC_UINT32 uiLangTypeIndex)
{
	JC_RETCODE 	rCode = JC_ERR_NOT_IMPLEMENTED ;
    JC_UINT32 	uiAcceptLanguageLength ;

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptLanguage : Entry") ;

	jdd_AssertOnFail ((JC_NULL != pcLanguage) && \
						(JC_NULL != puiLangLength), \
						"jdd_CFGGetAcceptLanguage : \
						Input Parameter is NULL") ;    
    
    PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;
    
    if (uiLangTypeIndex < \
		( (sizeof(g_pBrowserLanguageType) / sizeof(JC_INT8 *)) - 1) )
	{
		uiAcceptLanguageLength = jc_strlen ( \
					g_pBrowserLanguageType [uiLangTypeIndex]) ;
		if (uiAcceptLanguageLength < *puiLangLength)
		{
			jc_strcpy (pcLanguage, g_pBrowserLanguageType [uiLangTypeIndex]) ;
			rCode = JC_OK ;
		}
		else
		{
			*puiLangLength = uiAcceptLanguageLength ;
			rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
			JDD_LOG_INFO1 ("jdd_CFGGetAcceptLanguage: Returns [%d]", rCode) ;
		}
	}
	else
	{
		rCode = JC_ERR_CFG_NOT_FOUND ;
		JDD_LOG_INFO1 ("jdd_CFGGetAcceptLanguage: Returns [%d]", rCode) ;
	}

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptLanguage : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the Accept encodings supported 
 *					by the application
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure that 
 *               		contains the task ID of the application
 * @param[out]		pcEncoding Holds the accept encoding
 * @param[in,out]		puiLength Length of pcContentType in bytes on input,
 *                	and the required length as output if the function
 *               		returns JC_ERR_CFG_INSUFFICIENT_MEMORY		
 * @param[in]		uiIndex Specifies the index of the accept encoding 
 *					in the list
 * @retval			JC_OK On Success
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY If the given memory for 
 *					pcEncoding is not sufficient
 * @retval			JC_ERR_CFG_NOT_FOUND	If accept encoding is not found 
 *					in the list for the given index	
 * @note				The parameters pcEncoding and puiLength should 
 *					not be NULL.
 *
 * This function will be called by transport to get the accept encodings 
 * supported by the application. This information will be sent to the server 
 * in the fetch request buffer. If the given memory for pcEncoding is not  
 * sufficient then it stores the required memory length in puiLength and 
 * returns JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetAcceptEncoding (JC_TASK_INFO *pstTaskInfo, \
									 JC_INT8 *pcEncoding, \
									 JC_UINT32 *puiLength, \
									 JC_UINT32 uiIndex)
{
	JC_RETCODE 	rCode = JC_ERR_NOT_IMPLEMENTED ;
    JC_UINT32 	uiAcceptLength ;

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptEncoding : Entry") ;

	jdd_AssertOnFail ((JC_NULL != pcEncoding) && \
						(JC_NULL != puiLength), \
						"jdd_CFGGetAcceptEncoding : \
						Input Parameter is NULL") ;    
    
    PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;
    
    if (uiIndex < ( (sizeof (g_pBrowserEncoding) / sizeof (JC_INT8 *)) - 1) )
	{
		uiAcceptLength = jc_strlen (g_pBrowserEncoding [uiIndex]) ;
		if (uiAcceptLength < *puiLength)
		{
			jc_strcpy (pcEncoding, g_pBrowserEncoding [uiIndex]) ;
			rCode = JC_OK ;
		}
		else
		{
			*puiLength = uiAcceptLength ;
			rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
			JDD_LOG_INFO1 ("jdd_CFGGetAcceptEncoding: Returns [%d]", rCode) ;
		}
	}
	else
	{
		rCode = JC_ERR_CFG_NOT_FOUND ;
		JDD_LOG_INFO1 ("jdd_CFGGetAcceptEncoding: Returns [%d]", rCode) ;
	}

	JDD_LOG_INFO0 ("jdd_CFGGetAcceptEncoding : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief	  		This function provides the generic data that 
 *					the application wants to send to the lower layers.
 * @param[in]		pstTaskInfo  Pointer to a JC_TASK_INFO structure that 
 *               		contains the task ID of the application
 * @param[out]		pcHeaderName The name of the generic header should be 
 *					returned in this parameter
 * @param[in,out]		uiHeaderNameLen  Specifies the length of the 
 *					header name
 * @param[in,out]		pcHeaderValue The header value should be returned in 
 * 					this parameter.
 * @param[in,out]		puiHeaderValueLen Specifies the length of the 
 *					header value
 * @param[in]		uiGenericHeaderIndex Specifies the index of the 
 *					generic header
 * @retval			JC_OK On Success 
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY	The memory given for 
 *					the header value is insufficient
 * @retval			JC_ERR_CFG_NOT_FOUND	If the header related to the 
 *					generic header index is not found.
 *
 * This function will be called by transport to get the generic header that 
 * the application wants to send to the lower layers (HTTP, WSP).
 * If the given memory for pcHeaderValue is not sufficient 
 * then it stores the required memory length in puiLength and returns 
 * JC_ERR_CFG_INSUFFICIENT_MEMORY
 */
JC_RETCODE jdd_CFGGetGenericHeader (JC_TASK_INFO *pstTaskInfo, \
									JC_INT8 *pcHeaderName, \
									JC_INT32 uiHeaderNameLen, \
									JC_INT8	*pcHeaderValue, \
									JC_UINT32 *puiHeaderValueLen, \
									JC_UINT32 uiGenericHeaderIndex)

{
	JC_RETCODE 	rCode = JC_ERR_NOT_IMPLEMENTED ;
	JC_UINT32 	uiIndexValueLen ;
    JC_UINT32 	uiHeaderSize ;

	JDD_LOG_INFO0 ("jdd_CFGGetGenericHeader : Entry") ;

	jdd_AssertOnFail ((JC_NULL != pcHeaderName) && \
						(JC_NULL != pcHeaderValue) && \
						(JC_NULL != puiHeaderValueLen), \
						"jdd_CFGGetGenericHeader : \
						Input Parameter is NULL") ;    
    
    PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;
	PARAM_INTENTIONALLY_NOT_USED (uiHeaderNameLen) ;

	uiHeaderSize = sizeof (g_genericAppHeaders) / sizeof (ST_APP_HEADER) ;
	uiHeaderSize -- ;
	
	if (uiGenericHeaderIndex <= uiHeaderSize && 0 != uiHeaderSize )
	{
		if (JC_NULL != g_genericAppHeaders[uiGenericHeaderIndex].pHeaderName \
			&& JC_NULL != \
			g_genericAppHeaders[uiGenericHeaderIndex].pHeaderValue)
		{
			uiIndexValueLen = jc_strlen ( \
				g_genericAppHeaders[uiGenericHeaderIndex].pHeaderValue) ;
			if (*puiHeaderValueLen <= uiIndexValueLen )
			{
				rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
				*puiHeaderValueLen = uiIndexValueLen + 1 ;
				JDD_LOG_INFO1 ("jdd_CFGGetGenericHeader: Returns [%d]", rCode) ;
			}
			else
			{
				jc_strcpy (pcHeaderName, \
					g_genericAppHeaders[uiGenericHeaderIndex].pHeaderName) ;
				jc_strcpy (pcHeaderValue, \
					g_genericAppHeaders[uiGenericHeaderIndex].pHeaderValue) ;
				rCode = JC_OK ;
			}
		}
		else
		{
			rCode = JC_ERR_CFG_NOT_FOUND ;
			JDD_LOG_INFO1 ("jdd_CFGGetGenericHeader: Returns [%d]", rCode) ;
		}
	}
	else
	{
		rCode = JC_ERR_CFG_NOT_FOUND ;
		JDD_LOG_INFO1 ("jdd_CFGGetGenericHeader: Returns [%d]", rCode) ;
	}
	
	JDD_LOG_INFO0 ("jdd_CFGGetGenericHeader : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the task information that handles 
 *					push messages.
 * @param[in]		pcContentType Content-type string of the WAP 
 *					push message
 * @param[in,out]		pstTaskInfo  Stores the task information, that will 
 *					handle the push messages
 * @retval			JC_OK  On Success
 * @note				The parameter pstTaskInfo should not be NULL.
 *
 * This function will be called by transport when it receives a push message 
 * to get the task information that handles the push messages
 */
JC_RETCODE jdd_CFGGetMimeHandlerTaskInfo (const JC_INT8 *pcContentType, \
										  JC_TASK_INFO *pstTaskInfo)
{
	JC_RETCODE 	rCode = JC_ERR_NOT_IMPLEMENTED ;
	
	JDD_LOG_INFO0 ("jdd_CFGGetMimeHandlerTaskInfo : Entry") ;
	
	jdd_AssertOnFail ((JC_NULL != pstTaskInfo), \
						"jdd_CFGGetMimeHandlerTaskInfo : \
						pstTaskInfo is NULL") ;        
	mmi_trace(12, "jdd_log: jdd_CFGGetMimeHandlerTaskInfo content type[%s]", pcContentType);
	if (JC_NULL != pcContentType && \
		(jc_strstr (pcContentType, "application/vnd.wap.mms-message") ||
		jc_strstr (pcContentType, "text/vnd.wap.si") ||
		jc_strstr (pcContentType, "application/vnd.wap.sic") ||
		jc_strstr (pcContentType, "text/vnd.wap.sl") ||
		jc_strstr (pcContentType, "application/vnd.wap.slc")
#ifdef JOTA_PROVISION_APP_ENABLE		
		|| jc_strstr (pcContentType, "application/vnd.wap.connectivity-wbxml")
		|| jc_strstr (pcContentType, "application/vnd.wap.connectivity-xml")
#endif		
	))
	{
	   mmi_trace(12,"JDD_LOG: push to be sent to MMS Core");
		pstTaskInfo->iTaskID = E_TASK_MMS_CORE ;
		pstTaskInfo->iAppID = E_TASK_MMS_UI ;
	}
	/*
	else if (
		(pcContentType && (JC_NULL != jc_strstr (pcContentType, \
			"application/vnd.syncml.ds.notification")) ) || \
		(pcContentType && (JC_NULL != jc_strstr (pcContentType, \
			"application/vnd.syncml+wbxml")) ) || \
		(pcContentType && (JC_NULL != jc_strstr (pcContentType, \
		"application/vnd.syncml+xml")) ) )	
	{
		pstTaskInfo->iTaskID = E_TASK_SYNCML_CORE ;
		pstTaskInfo->iAppID  = E_TASK_SYNCML_UI ;
	}
	*/
	else
	{
		 mmi_trace(12,"JDD_LOG: push to be sent to browser core");
		pstTaskInfo->iTaskID = E_TASK_BROWSER_CORE ;
		pstTaskInfo->iAppID = E_TASK_BROWSER_UI ;
	}
	rCode = JC_OK ;
	
	
	JDD_LOG_INFO0 ("jdd_CFGGetMimeHandlerTaskInfo : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the client SDU size.
 * @retval			JC_INT32 Returns the size of the client SDU size
 *
 * This function will be called by transport while setting 
 * the client capabilities.
 */
JC_INT32 jdd_CFGGetClientSDUSize (void)
{
	return (MAX_CLIENT_SDU_SIZE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the server SDU size.
 * @retval			JC_INT32 Returns the size of the server SDU size
 *
 * This function will be called by transport while setting 
 * the client capabilities.
 */
JC_INT32 jdd_CFGGetServerSDUSize (void)
{
	return (MAX_SERVER_SDU_SIZE) ;
}
	
/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the client capability for the size 
 *					of a message that it can send to the server.
 * @retval			JC_INT32	Returns the size of the client messages
 *
 * This function will be called by transport while setting 
 * the client capabilities.
 */
JC_INT32 jdd_CFGGetClientMsgSize (void)
{
	return (MAX_CLIENT_MESSAGE_SIZE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the client capability for the size 
 *					of a message that it can accept from the server.
 * @retval			JC_INT32	Returns the size of the server messages
 *
 * This function will be called by transport while setting 
 * the client capabilities.
 */
JC_INT32 jdd_CFGGetServerMsgSize (void)
{
	return (MAX_SERVER_MESSAGE_SIZE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the client capability for  
 
  
 *					by client
 *
 * This function will be called by transport while setting 
 * the client capabilities.
 */
JC_UINT8 jdd_CFGGetMaximumOSRequest (void)
{
	return (MAX_OS_REQUEST) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the client capability for 
 
  
 *					by client
 *
 * This function will be called by transport while setting 
 * the client capabilities.
 */
JC_INT32 jdd_CFGGetPushMaxOSRequest (void)
{
	return (MAX_PUSH_OS_REQUEST) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the client capability for 
 *					confirmed push operations.
 * @retval			E_TRUE	Client supports confirmed push operations
 * @retval			E_FALSE	Client doest not support confirmed push operations
 *
 * This function will be called by transport while setting 
 * the client capabilities.
 */
JC_BOOLEAN jdd_CFGIsConfirmedPushSupported (void)
{
	return (E_FALSE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the client capability for 
 *					push operations.
 * @retval			E_TRUE	Client supports push operations
 * @retval			E_FALSE	Client doest not support push operations
 *
 * This function will be called by transport while setting 
 * the client capabilities.
 */
JC_BOOLEAN jdd_CFGIsPushSupported (void)
{
	return (E_FALSE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the client capability for 
 *					resuming a session.
 * @retval			E_TRUE	Client supports resuming a session
 * @retval			E_FALSE	Client doest not support resuming a session
 *
 * This function will be called by transport while setting 
 * the client capabilities.
 */
JC_BOOLEAN jdd_CFGIsSessionResumeSupported (void)
{
	return (E_FALSE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the client capability for 
 *					acknowledgement headers.
 * @retval			E_TRUE	Client supports acknowledgement headers
 * @retval			E_FALSE	Client does not support acknowledgement headers
 *
 * This function will be called by transport while setting 
 * the client capabilities.
 */
JC_BOOLEAN jdd_CFGIsAckHeadersSupported (void)
{
	return (E_FALSE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the client capability for 
 *					large data transfer.
 * @retval			E_TRUE	Client supports large data transfer
 * @retval			E_FALSE	Client does not support large data transfer
 *
 * This function will be called by transport while setting 
 * the client capabilities.
 */
JC_BOOLEAN jdd_CFGIsLargeDataTransferSupported (void)
{
	return (E_FALSE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the PDU size used in 
 *					send operations in WSP.
 * @retval			JC_UINT16 Returns the size of send PDU size
 *
 * This function is called by transport during initialization of WSP module.
 */
JC_UINT16 jdd_CFGWSPGetSendPDUSize (void)
{
	return (WSP_GET_SEND_PDU_SIZE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the PDU size used in 
 *					receive operations in WSP.
 * @retval			JC_UINT16 Returns the size of receive PDU size
 *
 * This function is called by transport during initialization of WSP module.
 */
JC_UINT16 jdd_CFGWSPGetRecvPDUSize (void)
{
	return (WSP_GET_RECV_PDU_SIZE) ;
}


#ifdef WAP_WITH_WTLS

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the Cipher suite count
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure 
 *					that contains the task ID of the application
 * @param[out]		puiCipherSuiteCount The number of cipher suites 
 *					supported by browser is returned in this field.
 * @retval			JC_OK On Success
 * @note				The parameter puiCipherSuiteCount should not be NULL.
 *
 * This function will be called by transport to get the Cipher suite count.
 */
JC_RETCODE jdd_CFGGetCipherSuiteCount (JC_TASK_INFO *pstTaskInfo, \
										JC_UINT32 *puiCipherSuiteCount)
{
	JC_RETCODE	rCode = JC_OK ;
	JC_UINT32	uiIndex = 0 ;

	JDD_LOG_INFO0 ("jdd_CFGGetCipherSuiteCount : Entry") ;

	jdd_AssertOnFail ((JC_NULL != puiCipherSuiteCount), \
						"jdd_CFGGetCipherSuiteCount : \
						puiCipherSuiteCount is NULL") ;        

	PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;

	do
	{
		uiIndex ++ ;
	}
	while ('\0' != g_aiWTLSCipherSuites [uiIndex]) ;

	*puiCipherSuiteCount = uiIndex ;

	JDD_LOG_INFO0 ("jdd_CFGGetCipherSuiteCount : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the key exchange algorithm count
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure 
 *					that contains the task ID of the application
 * @param[in]		uiClass Specifies the class of algorithm
 * @param[out]		puiKeyExAlgCount Holds the algorithm count
 * @retval			JC_OK On Success
 * @retval			JC_ERR_CFG_NOT_FOUND If invalid class is mentioned.
 * @note				The parameter puiKeyExAlgCount should not be NULL.
 *
 * This function will be called by transport to get the key exchange 
 * algorithm count.
 */
JC_RETCODE jdd_CFGGetKeyExAlgCount (JC_TASK_INFO *pstTaskInfo, \
									JC_UINT32 uiClass, \
									JC_UINT32 *puiKeyExAlgCount)
{
	JC_RETCODE	rCode = JC_OK ;
	JC_UINT32	uiIndex = 0 ;

	JDD_LOG_INFO0 ("jdd_CFGGetKeyExAlgCount : Entry") ;

	jdd_AssertOnFail ((JC_NULL != puiKeyExAlgCount), \
						"jdd_CFGGetKeyExAlgCount : \
						puiKeyExAlgCount is NULL") ;    

	PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;

	switch (uiClass)
	{
		case 1 :
			do
			{
				uiIndex ++ ;
			}
			while ('\0' != g_aiWTLSClass1KeyExAlgs [uiIndex]) ;

			break ;

		case 2 :
			do
			{
				uiIndex ++ ;
			}
			while ('\0' != g_aiWTLSClass2KeyExAlgs [uiIndex]) ;

			break ;

		case 3 :
			do
			{
				uiIndex ++ ;
			}
			while ('\0' != g_aiWTLSClass3KeyExAlgs [uiIndex]) ;

			break ;

		default :
			rCode = JC_ERR_CFG_NOT_FOUND ;
			JDD_LOG_INFO1 ("jdd_CFGGetKeyExAlgCount: Returns [%d]", rCode) ;
	}

	if (JC_OK == rCode)
	{
		*puiKeyExAlgCount = uiIndex ;
	}

	JDD_LOG_INFO0 ("jdd_CFGGetKeyExAlgCount : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the Cipher suites to be used 
 *					during secure session
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure 
 *					that contains the task ID of the application
 * @param[in]		uiIndex Specifies the index of the algorithm 
 *					in the list
 * @param[out]		phCipherSuite Holds the Cipher suite
 * @retval			JC_OK On Success
 * @retval			JC_ERR_CFG_NOT_FOUND No Cipher suite found in the list 
 *					with the given index
 * @note				The parameter phCipherSuite should not be NULL.
 *
 * This function will be called by transport to get the Cipher suites.
 * One of them will be selected in handshake process that will be used 
 * during the secure session.
 */
JC_RETCODE jdd_CFGGetCipherSuite (JC_TASK_INFO *pstTaskInfo, \
									JC_UINT32 uiIndex, \
									E_WTLS_CIPHER_SUITE *phCipherSuite)
{
	JC_RETCODE	rCode = JC_OK ;

	JDD_LOG_INFO0 ("jdd_CFGGetCipherSuite : Entry") ;

	jdd_AssertOnFail ((JC_NULL != phCipherSuite), \
						"jdd_CFGGetCipherSuite : \
						phCipherSuite is NULL") ; 

	PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;

	*phCipherSuite = g_aiWTLSCipherSuites [uiIndex] ;

	JDD_LOG_INFO0 ("jdd_CFGGetCipherSuite : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the key exchange algorithms 
 *					to be used during secure session
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure 
 *					that contains the task ID of the application
 * @param[in]		uiIndex Specifies the index of the algorithm 
 *					in the list
 * @param[in]		uiClass Specifies the class of algorithm
 * @param[out]		phKeyExAlgorithm Holds the algorithm
 * @retval			JC_OK On Success
 * @retval			JC_ERR_CFG_NOT_FOUND No algorithm found in the list 
 *					with the given index
 * @note				The parameter phKeyExAlgorithm should not be NULL.
 *
 * This function will be called by transport to get the key exchange 
 * algorithms. One of them will be selected in handshake process that 
 * will be used during the secure session.
 */
JC_RETCODE jdd_CFGGetKeyExAlgorithm ( JC_TASK_INFO *pstTaskInfo, \
									JC_UINT32 uiIndex, \
									JC_UINT32 uiClass, \
									E_WTLS_KEY_EXCHANGE_ALG *phKeyExAlgorithm)
{
	JC_RETCODE	rCode = JC_OK ;

	JDD_LOG_INFO0 ("jdd_CFGGetKeyExAlgorithm : Entry") ;

	jdd_AssertOnFail ((JC_NULL != phKeyExAlgorithm), \
						"jdd_CFGGetKeyExAlgorithm : \
						phKeyExAlgorithm is NULL") ;    

	PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;

	switch (uiClass)
	{
		case 1 :

			*phKeyExAlgorithm = g_aiWTLSClass1KeyExAlgs [uiIndex] ;

			break ;

		case 2 :

			*phKeyExAlgorithm = g_aiWTLSClass2KeyExAlgs [uiIndex] ;

			break ;

		case 3 :

			*phKeyExAlgorithm = g_aiWTLSClass3KeyExAlgs [uiIndex] ;

			break ;

		default :
		
			rCode = JC_ERR_CFG_NOT_FOUND ;
			JDD_LOG_INFO1 ("jdd_CFGGetKeyExAlgorithm: Returns [%d]", rCode) ;
	}	

	JDD_LOG_INFO0 ("jdd_CFGGetKeyExAlgorithm : Exit") ;
	return (rCode) ;
}

#endif /*WAP_WITH_WTLS*/

#ifdef HTTP_WITH_SSL

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the SSL Cipher suite count
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure 
 *					that contains the task ID of the application
 * @param[out]		puiCipherSuiteCount Holds the SSL Cipher suite count
 * @retval			JC_OK Successfully got the SSL Cipher suite count
 * @note				The parameter puiCipherSuiteCount should not be NULL.
 *
 * This function will be called by transport to get the SSL Cipher suite count.
 */
JC_RETCODE jdd_CFGGetSSLCipherSuiteCount (JC_TASK_INFO *pstTaskInfo, \
											JC_UINT32 *puiCipherSuiteCount)
{
	JC_RETCODE 	rCode = JC_OK ;
	JC_UINT32	uiIndex = 0 ;

	JDD_LOG_INFO0 ("jdd_CFGGetSSLCipherSuiteCount : Entry") ;

	jdd_AssertOnFail ((JC_NULL != puiCipherSuiteCount), \
						"jdd_CFGGetSSLCipherSuiteCount : \
						puiCipherSuiteCount is NULL") ;        

	PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;

	do
	{
		uiIndex ++ ;
	}
	while ('\0' != g_aiSSLCipherSuites [uiIndex]) ;

	*puiCipherSuiteCount = uiIndex ;

	JDD_LOG_INFO0 ("jdd_CFGGetSSLCipherSuiteCount : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the SSL Cipher suites 
 *					to be used during secure session
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure 
 *					that contains the task ID of the application
 * @param[in]		uiIndex Specifies the index of the cipher suite 
 *					in the list
 * @param[out]		phCipherSuite Holds the Cipher suite
 * @retval			JC_OK On Success
 * @retval			JC_ERR_CFG_NOT_FOUND No Cipher suite found in the list 
 *					with the given index
 * @note				The parameter phCipherSuite should not be NULL.
 *
 * This function will be called by transport to get the SSL Cipher suites.
 * One of them will be selected in handshake process that will be used  
 * during the secure session.
 */
JC_RETCODE jdd_CFGGetSSLCipherSuite (JC_TASK_INFO *pstTaskInfo, \
										JC_UINT32 uiIndex, \
										E_SSL_CIPHER_SUITE *phCipherSuite)
{
	JC_RETCODE	rCode = JC_OK ;

	JDD_LOG_INFO0 ("jdd_CFGGetSSLCipherSuite : Entry") ;

	jdd_AssertOnFail ((JC_NULL != phCipherSuite), \
						"jdd_CFGGetSSLCipherSuite : \
						phCipherSuite is NULL") ; 

	PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;

	*phCipherSuite = g_aiSSLCipherSuites [uiIndex] ;

	JDD_LOG_INFO0 ("jdd_CFGGetSSLCipherSuite : Exit") ;
	return (rCode) ;
}

#endif /* HTTP_WITH_SSL */

/**
 * @ingroup			jdd_configapi
 * @brief			This function provides the root certificates to be used 
 *					during secure session
 * @param[in]		pstTaskInfo Pointer to a JC_TASK_INFO structure 
 *					that contains the task ID of the application
 * @param[in]		uiRootIndex Specifies the index of the certificate 
 *					in the list
 * @param[in,out]		puiRootLength specifies the length of the variable 
 *					pmFileName
 * @param[in,out]		pmFileName Holds the file name which has the required 
 *					certificate information
 * @retval			JC_OK On Success
 * @retval			JC_ERR_CFG_INSUFFICIENT_MEMORY The given memory for the 
 *					variable pmFileName is not sufficient
 * @note				The parameters puiRootLength and pmFileName 
 *					should not be NULL.
 *
 * This function will be called by transport to get the root certificates, 
 * which will be used during the secure session establishment.
 */
JC_RETCODE jdd_CFGGetRootCertificate (JC_TASK_INFO *pstTaskInfo, \
										JC_UINT32 uiRootIndex, \
										JC_UINT32 *puiRootLength, \
										JC_CHAR *pmFileName)
{
	JC_RETCODE		rCode = JC_ERR_CFG_NOT_FOUND ;	
	JC_UINT32		uiReqRootLength ;
	const JC_INT8	*pcSrc ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_CFGGetRootCertificate : Entry") ;

	jdd_AssertOnFail ((JC_NULL != puiRootLength) && \
						(JC_NULL != pmFileName), \
						"jdd_CFGGetRootCertificate : \
						Input Parameter is NULL") ; 

	PARAM_INTENTIONALLY_NOT_USED (pstTaskInfo) ;
	
	if (JC_NULL != g_pcRootCertName [uiRootIndex])
	{
		uiReqRootLength = jc_strlen (g_pcRootCertName [uiRootIndex]) ;
		
		if (uiReqRootLength < *puiRootLength)
		{
			pcSrc = g_pcRootCertName [uiRootIndex] ;
			if ('\0' != *pcSrc)
			{
				while ('\0' != *pcSrc) 
				{
					*pmFileName = *pcSrc ;
					pmFileName++ ;
					pcSrc++ ;						
				}
				*pmFileName = '\0' ;
				rCode = JC_OK ;
			}
		}
		else
		{
			*puiRootLength = uiReqRootLength ;
			rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
			JDD_LOG_INFO1 ("jdd_CFGGetRootCertificate: Returns [%d]", rCode) ;
		}
	}
	
	JDD_LOG_INFO0 ("jdd_CFGGetRootCertificate : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function specifies the file name to be used to 
 *					store and retrieve the WTLS Context information.
 * @param[in]		uiSize Not used currently
 * @param[in,out]		pcName Holds the WTLS Context name
 * @param[in,out]		puiLength Not used currently
 * @retval			JC_OK On Success
 * @note				The parameter pcName should not be NULL
 *
 * This function will be called to store and retrieve the WTLS Context 
 * information that will be used during secure session.
 */
JC_RETCODE jdd_CFGGetWTLSContextName (JC_UINT32 uiSize, \
										JC_INT8 *pcName, \
										JC_UINT32 *puiLength)
{
	JC_RETCODE	rCode = JC_OK ;	

	JDD_LOG_INFO0 ("jdd_CFGGetWTLSContextName : Entry") ;

	jdd_AssertOnFail ((JC_NULL != pcName), \
						"jdd_CFGGetWTLSContextName : pcName is NULL") ;

	PARAM_INTENTIONALLY_NOT_USED (puiLength) ;
	PARAM_INTENTIONALLY_NOT_USED (uiSize) ;

	jc_strcpy (pcName, WTLS_CONTEXT_FILE_NAME) ;

	JDD_LOG_INFO0 ("jdd_CFGGetWTLSContextName : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the maximum no. of connections 
 *					to be used in HTTP.
 
 * @note				The return value should not be zero
 *
 * The function will be called during initialization of HTTP module to 
 * set the maximum no. of socket connections that can be opened at any time.
 */
JC_UINT32 jdd_CFGGetHTTPMaxConnections (void)
{
	return (MAX_HTTP_CONNECTIONS) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the TCP Receive buffer length 
 *					to be used in HTTP.
 * @retval			JC_UINT32 TCP Receive buffer length in bytes
 *
 * The function will be called during initialization of HTTP module to set 
 * the length of the TCP receive buffer to be used in socket
 * receive operations. 
 */
JC_UINT32 jdd_CFGGetTCPRecvBuffer (void)
{
	return (DEFAULT_SOCKET_TCP_RECV_BUFFER) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the user data size that 
 *					the transport can request to the application.
 * @retval			JC_UINT32 Returns User data Size in bytes
 *
 * The function will be called by transport to get the user data size that 
 * the transport can request to the application in one chunk.
 */
JC_UINT32 jdd_CFGGetUserDataSize (void)
{
	return (DEFAULT_USER_DATA_SIZE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the minimum timer value of 
 *					the transport timer
 * @retval			JC_UINT32 Transport Timer value
 *
 * The function will be called to find out the transport timer value.
 */
JC_UINT32 jdd_CFGGetTransportTimerValue (void)
{
	return (MIN_TRANSPORT_TIMER_VALUE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function defines the minimum timer value of 
 *					the push timer
 * @retval			JC_UINT32 Push Timer value
 *
 * The function will be called to find out the push timer value.
 */

JC_UINT32 jdd_CFGGetPushTimerValue (void)
{
	return (MIN_PUSH_TIMER_VALUE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function specifies whether the transport can 
 *					perform continuous polling in case of WSP connection 
 *					mode or should it wait for the application response 
 *					before polling.
 * @retval			E_TRUE If continous polling is enabled.
 * @retval			E_FALSE If continous polling is disabled.
 **/
JC_BOOLEAN jdd_CFGIsWSPContinuousPollingEnabled (void)
{
	return (E_FALSE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function specifies the maximum WTP Group size that 
 *					can be processed by the WAPStack during receiving.
 * @retval			JC_UINT16  Any value greater than zero, specifies the 
 *					WTP Group Size. 
 * @retval			If it is zero, then the default value 
 *					of the gateway is used.
 * @note				This API should be configured for those platforms where 
 *					the dynamic memory is very limited.
 */
JC_UINT16 jdd_CFGGetWSPMaxWTPGroupSize (void)
{
	return (0) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function specifies the maximum time for a socket 
 *					to be kept alive in HTTP 
 * @retval			JC_UINT32 returns the maximum alive time for 
 *					a socket in seconds.							
 */
JC_UINT32 jdd_CFGGetMaxSocketAliveTime (void)
{
	return (MAX_SOCKET_ALIVE_TIME) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function specifies whether the idle socket 
 *					(no request is pending) in the HTTP layer needs to be
 *					closed, in case if the connection is persistent.
 * @retval			E_TRUE If socket is to be closed.
 * @retval			E_FALSE If socket is not to be closed.
 **/
JC_BOOLEAN jdd_CFGCloseIdleSocket (void)
{
	return (E_FALSE) ;
}

/**
 * @ingroup			jdd_configapi
 * @brief			This function specifies the maximum time the transport 
 *					task can poll when the data is available in the socket. 
 * @retval			Returns the maximum time that transport can poll 
 *					continuously. If it returns zero, the transport poll
 *					until the lower layer returns would-block.
 **/
JC_UINT32 jdd_CFGGetMaxTransportPollingTime (void)
{
	return (MAX_TRANSPORT_POLLING_TIME) ;
}

/**
 * @brief Specifies the maximum number of requests that can be pipelined on a 
 * pipeline supported socket connection at a time.
 * @return	Number of requests that can be pipelined.
 * @return	0	There is no limit on the number of requests that can be pipelined.
 */
JC_UINT16 jdd_CFGHTTPGetMaxPipelineReqCount (void)
{
	return (2) ;
}

/**
 * @brief Specifies the maximum amount of data that can be queued at HTTP
 * @return	0	Specifies that there is no limit on the amount of data that can be queued.
 * @return	Others	Maximum amount of data that can be queued at HTTP layer.
 *
 * This function is used for event based sockets. When receive events comes from platform 
 * HTTP layer will read the data and put it in queue. So if application wants to put a limit
 * on the amount of data that can be queued at HTTP layer, it can be configured here. Data 
 * will be queued in HTTP layer until it reaches this limit, once it reaches limit, the 
 * receive events will be ignored and count will be maintained  for each receive event.
 * Once the data in the queue goes below this limit HTTP once again starts reading data.
 * If 0 is returned here, it is assumed that there is no limit on the queue.
 */
JC_UINT32 jdd_CFGGetHTTPMaxRecvQueueSize (void)
{
	return MAX_HTTP_RECV_QUEUE_SIZE ;
}


#ifdef	EMAIL_PROTOCOL_ENABLE

/**
 * @ingroup			jdd_configapi
 * @brief			This function specifies the maximum time for a socket to 
 *					be kept alive in Email Protocols.
 * @retval			JC_UINT32 returns the maximum alive time for a socket 
 *					in seconds.
 */
JC_UINT32 jdd_CFGGetMaxEmailSocketAliveTime (void)
{
	return (MAX_SOCKET_ALIVE_TIME) ;
}


/**
 * @ingroup			jdd_configapi
 * @brief			This function specifies the maximum time value for 
 *					socket select call in Email Protocols.
 * @retval			JC_UINT32  returns the maximum alive time for a socket 
 *					in milliseconds.
 **/
JC_UINT32 jdd_CFGGetMaxEmailSocketSelectTimeout (void)
{
	return (MAX_SOCKET_SELECT_TIMEOUT) ;
}

#endif	/* EMAIL_PROTOCOL_ENABLE */
/* END OF FILE */
