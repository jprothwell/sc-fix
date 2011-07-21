#define MODULE_NAME "JDD_BROWSERCONFIG"
#define FILE_NAME "jdd_browserconfig.c"
/***************************************************************************
 *
 * File Name jdd_dataconnection.c
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
 * @file    jdd_browserconfig.c
 * @ingroup jdd_browserconfig
 * @brief   Contains functions for configuring browser
 **/
 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
  
 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include <ddl.h>
#include <jcal.h>
#include <jdd_browserconfig.h>
#include <jdi_cache.h>

 /***************************************************************************
 * Macros
 **************************************************************************/
 #ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					OTHER_MODULE
#else
#define __MODULE_ID__					0
#endif

 /**
 * @brief	This macro is used for calculating cache file size.
 **/
#define CACHE_DEFAULT_SIZE				(100 * 1024)

 /**
 * @brief	This macro is used for calculating cache file size.
 **/
#define CACHE_MAX_FILE					(1)

 /**
 * @brief	This macro defines the size of each cache file in bytes.
 * @see		#jdd_CacheConfigGetSizeInfo
 **/
#define CACHE_SINGLE_FILE_SIZE			(CACHE_DEFAULT_SIZE/CACHE_MAX_FILE)

 /**
 * @brief	This macro defines the size in bytes for each chunk in a 
 *			cache file.
 * @see		#jdd_CacheConfigGetSizeInfo
 **/
#define CACHE_CHUNK_SIZE				(512)

 /**
 * @brief	This macro defines age in seconds for cache entries when 
 *			there is no expiry date specified by content provider. 
 *			By default, expiry is set to be 1 day.
 * @see		#jdd_CacheConfigGetStaleValue
 **/
#define CACHE_STALE_AGE				(60 * 60 * 24)

 /**
 * @brief	This macro defines chunk size in bytes, for reading a file
 *			when file:// scheme is used.
 * @see		#jdd_BrowserConfigGetFileChunkSize
 **/
#define	FILE_SCHEME_BUFFER_SIZE			(5192)

 /**
 * @brief	This macro defines chunk size in bytes, for cache module
 *			to read from cache when content has to be fetched from cache
 * @see		#jdd_BrowserConfigGetCacheChunkSize
 **/
#define CACHE_BUFFER_SIZE				(512)

 /**
 * @brief	This macro defines maximum number of encoded URIs to be
 *			stored by punycode convertor
 * @see		#jdd_BrowserConfigGetMaxPCURI
 **/
#define PC_MAX_URI						(5)

 /**
 * @brief	This macro defines block count for internal memory pool
 *			managed and used by browser core.
 * @see		#jdd_BrowserConfigGetMemoryPoolInfo
 **/
#define MEM_MGR_BROWSER_BLOCK			(320)

 /**
 * @brief	This macro defines block size for internal memory pool
 *			managed and used by browser core.
 * @see		#jdd_BrowserConfigGetMemoryPoolInfo
 **/
#define MEM_MGR_BROWSER_SIZE			(32)

 /**
 * @brief	This macro defines maximum value of float type supported
 *			by application/platform
 * @see		#jdd_BrowserConfigGetMaxFloat
 **/
#define FLOAT_MAX_VALUE	(3.40282347E+38f)
 
/**
 * @brief	This macro defines minimum value of float type supported
 *			by application/platform
 * @see		#jdd_BrowserConfigGetMaxFloat
 **/
#define	FLOAT_MIN_VALUE (1.17549435E-38f)
 /***************************************************************************
 * Global Variable declarations and definitions
 **************************************************************************/
 /**
 * @brief	This string array defines URL types to be handled by
 *			browser core.
 * @see		#jdd_BrowserConfigUrlTypes
 */
 static const JC_INT8 *gpacUrlTypes [] =
{
	"http://",
	"https://",
	"file://",
	JC_NULL
} ;

/**
 * @brief	This string defines the user agent with which the browser
 *			is supposed to be identified with.
 * @see		#jdd_BrowserConfigGetUserAgent
 */
const JC_INT8 *gpacUserAgent = \
"Mozilla/5.0 (compatible; Jataayu/jB5/5.0; Windows XP; Generic; en;)" ;

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
 * @ingroup		jdd_browserconfig 
 * @brief 		This function is used for getting the details about each
 *				cache file of particular type.
 * @param[in] 	eStorageType Specifies the type of cache.
 * @param[in] 	uiFileIndex Specifies the number of cache file for which
 *				details needs be gathered.
 * @param[in] 	pstCacheConfig Specifies the size of cache file and chunk
 *				size of that particular file.
 *
 * This function is used for getting details about each cache file of
 * particular type, defined by eStorageType.
 */
 void jdd_CacheConfigGetSizeInfo (E_CACHE_STORAGE_TYPE eStorageType, \
			JC_UINT32 uiFileIndex, ST_CACHE_CONFIG *pstCacheConfig) 
{
	PARAM_INTENTIONALLY_NOT_USED (uiFileIndex) ;

	jdd_AssertOnFail ((JC_NULL != pstCacheConfig), \
							"pstCacheConfig is NULL") ;
					
	if (E_CACHE_OPERATOR_SPECIFIC == eStorageType)
	{
		pstCacheConfig->uiCacheSize = CACHE_SINGLE_FILE_SIZE ;
		pstCacheConfig->uiChunkSize = CACHE_CHUNK_SIZE ;
	}
	else
	{
		pstCacheConfig->uiCacheSize = CACHE_SINGLE_FILE_SIZE ;
		pstCacheConfig->uiChunkSize = CACHE_CHUNK_SIZE ;
	}
}

/**
 * @ingroup		jdd_browserconfig 
 * @brief 		Provides the stale value for all the cache entry.
 * @retval		Stale Value in seconds
 *
 * Sometimes cache entries does not have an expiry date associated with it. 
 * These entries may reside in cache until the entire cache is cleared. 
 * To avoid this, application can set a default expiry time for cache 
 * entries without an associated expiry time. The application can thus 
 * avoid cache information to be stored forever.
 */
JC_UINT32 jdd_CacheConfigGetStaleValue (void) 
{
	return (CACHE_STALE_AGE) ;
}

/**
 * @ingroup		jdd_browserconfig 
 * @brief 		Specifies whether the cache revalidation header is required
 *				explicitly to revalidate the content.
 * @retval		E_TRUE if revalidation is necessary
 * @retval		E_FALSE otherwise
 *
 * If this function returns true, then the core will not 
 * send revalidation request unless revalidation header is present
 * in the headers. If this function returns false, then the core will 
 * send revalidation header even if revalidation header is not present,
 * provided the ETAG or Last Modified header is present.
 */
JC_BOOLEAN jdd_CacheConfigIsRevalHeaderExplicitlyReq (void)
{
	return (E_FALSE) ;
}

/**
 * @ingroup		jdd_browserconfig 
 * @brief 		Specifies whether cache file creation during initialization
 *				has to be done in asynchronous way.
 * @retval		E_TRUE if asynchronous method is desired
 * @retval		E_FALSE otherwise
 *
 * This function specifies whether the cache module is to
 * create all files at one go or one file at a time. If this function 
 * returns E_TRUE, then the cache will return the control after creating
 * one file. At a later point of time core can call 
 * another cache API to continue with file creation.
 */
JC_BOOLEAN jdd_BrowserConfigCacheAsyncFileCreation (void)
{
	return (E_FALSE) ;
}

/**
 * @ingroup		jdd_browserconfig 
 * @brief 		Provides URL list to be supported by browser
 * @retval		String array of URL types
 * 
 * This function is to specify URL types that needs to be handled 
 * internally by browser core. The URL types that are not returned by the 
 * function will not be handled by browser core and will be sent to 
 * the scheme listener of the application. Application can handle
 * the URL type in the listener.
 */
const JC_INT8 ** jdd_BrowserConfigUrlTypes (void) 
{
	return (gpacUrlTypes) ;
}

/**
 * @ingroup		jdd_browserconfig 
 * @brief 		Provides maximum float value.
 * @retval		Maximum float value supported by application/platform
 * 
 * This function should return maximum value of float that the browser core
 * can use for its operations. Value should be decided based on the 
 * application's requirement or/and platform's constraints.
 */
JC_FLOAT jdd_BrowserConfigGetMaxFloat (void)
{
	return (FLOAT_MAX_VALUE) ;
}

/**
 * @ingroup		jdd_browserconfig 
 * @brief 		Provides minimum float value.
 * @retval		Minimum float value supported by application/platform
 * 
 * This function should return minimum value of float that the browser core
 * can use for its operations. Value should be decided based on the 
 * application's requirement or/and platform's constraints.
 */
JC_FLOAT jdd_BrowserConfigGetMinFloat (void)
{
	return (FLOAT_MIN_VALUE) ;
}

/**
 * @ingroup		jdd_browserconfig
 
  
 * 
 * When the content is fetched from the server the content is received in
 * chunks. The browser needs to follow the same mechanism for file 
 * content (having file scheme: "file://") as well. 
 * The application needs to use this function to specify the chunk size 
 * when the content is read from a file.
 */
JC_UINT32 jdd_BrowserConfigGetFileChunkSize (void)
{
	return (FILE_SCHEME_BUFFER_SIZE) ;
}

/**
 * @ingroup		jdd_browserconfig
 
  
 * 
 * When content is fetched from any server, the content is received in chunks.
 * The browser needs to follow the same mechanism for cache content (when 
 * request is retrieved from cache) as well. 
 * The application needs to use this function to specify the chunk size 
 * when the content is read from the cache.
 */
JC_UINT32 jdd_BrowserConfigGetCacheChunkSize (void)
{
	return (CACHE_BUFFER_SIZE) ;
}

/**
 * @ingroup		jdd_browserconfig
 * @brief 		Provides the internal memory pool information.
 * @param[out]	puiBlockNum	Specifies number of blocks that need to be 
				allocated for the internal memory pool.
 * @param[out]	puiBlockSize Specifies the size of the block, within which
 *				the memory needs to be allocated from the internal memory 
 *				pool. Any memory allocation greater than the block size will
 *				be allocated from the generic memory pool using memory 
 *				abstraction layer (jdd_memapi functions).
 * 
 * Internal memory pool allows core to manage (allocate and free) memory 
 * requirements for small chunks. 
 * This API needs to configure the maximum number of blocks that are
 * controlled by the core. 
 *
 * Say for example: if the block count is configured as 320 and memory size
 * is configured as 32 bytes. The core allocates 10KB of data (320 * 32) and
 * uses it for small memory allocation which is less than 32 bytes. 
 * 
 * If all 320 blocks are occupied, then even for small memory allocation, 
 * core shall use the #jdd_MemAlloc API. 
 *
 * Using this mechanism, memory fragmentation can be avoided at the system 
 * level. If platform has better memory management, then internal memory pool
 * need not be enabled (Internel memory pool is enabled if 
 * MEM_INTERNAL_POOL_ENABLE is defined in ddldefines.h). In that case, 
 * this API will not be used. 
 *
 * Generally it is recommended to provide atleast 10KB of data for core 
 
 * on memory allocation pattern.  
 */
void jdd_BrowserConfigGetMemoryPoolInfo (JC_UINT32 *puiBlockNum, \
				JC_UINT32 *puiBlockSize)
{
	jdd_AssertOnFail ((JC_NULL != puiBlockNum) && (JC_NULL != puiBlockSize), \
					"jdd_BrowserConfigGetMemoryPoolInfo: \
					puiBlockNum/puiBlockSize is NULL") ;
					
	*puiBlockNum = MEM_MGR_BROWSER_BLOCK ;
	*puiBlockSize = MEM_MGR_BROWSER_SIZE ;
}

/**
 * @ingroup		jdd_browserconfig
 * @brief 		Provides maximum number of converted URI that can be 
 *				stored by punycode converter (PC).
 
 * 
 * Punycode converter converts a hostname into an encoded string so that 
 * the domain name can be resolved. The punycode converter module can 
 * store the URI and the encoded string so that each time it need not
 * encode the string. The maximum number of such encoded string that can 
 * be stored can be specified using this function.
 */
JC_UINT32 jdd_BrowserConfigGetMaxPCURI (void)
{
	return (PC_MAX_URI) ;
}

/**
 * @ingroup		jdd_browserconfig
 * @brief 		Provides the browser user agent.
 * @retval		JC_INT8 string describing User-Agent
 * 
 * Content adaptation done by server/gateway might be based on the the 
 * user-agent string. Content displayed by IE might be different from the
 * one seen on Mozilla, as both use different user agent string.
 * So, application should choose a suitable user-agent and return it
 * from this function. Same will be used by core while making requests
 * to the server.
 */
const JC_INT8 * jdd_BrowserConfigGetUserAgent (void)
{
    return (gpacUserAgent) ;
}

/**
* @brief This function specifies whether the navigation page has to be taken from cache or not
*
* @retval	E_TRUE	Take the page from cache directly
* @retval	E_FALSE	Don't take the page from cache directly
*
* If this function returns E_TRUE, then whenever navigation is done (back or forward) and 
* if the cache page is not expired it will be taken from cache without revalidation process. 
* If it returns E_FALSE the page will be revalidated with the server if required.
*/
JC_BOOLEAN jdd_BrowserConfigNavigationPageFromCache (void)
{
	return E_TRUE ;
}

/* End of File */
