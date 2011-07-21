/***************************************************************************
 * $Id: jdd_browserconfig.h,v 1.35 2008/12/10 14:46:16 kavithadevhtmlbrow Exp $
 * $Revision: 1.35 $
 * $Date: 2008/12/10 14:46:16 $
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
 * $Log: jdd_browserconfig.h,v $
 * Revision 1.35  2008/12/10 14:46:16  kavithadevhtmlbrow
 * window.open Support - DE_WINDOW_OPEN_ENABLE
 *
 * Revision 1.34  2008/10/07 07:40:12  sriramdevhtmlbrow
 * Made changes to take the page from cache if it is not expired, during back or forward navigation without revalidation. jtrack issue 2446.
 *
 * Revision 1.33  2008/02/29 06:33:01  kumardevhtmlbrow
 * doxygen update.
 *
 *  
 ***************************************************************************/

/**
 * @page browserconfig Browser Configuration
 * 
 * @section cache Cache Configurations
 * @li Set cache chunk size using #jdd_BrowserConfigGetCacheChunkSize.
 * @li Set cache stale value using #jdd_CacheConfigGetStaleValue.
 * @li Set cache revalidation option using #jdd_CacheConfigIsRevalHeaderExplicitlyReq.
 *
 * @section flt Float Configurations
 * @li Set maximum value of float using #jdd_BrowserConfigGetMaxFloat.
 * @li Set minimum value of float using #jdd_BrowserConfigGetMinFloat.
 * 
 * @section other Other Configurations
 * @li Set the type of schemes that is supported by the browser application (browser core and the application)
 * using #jdd_BrowserConfigUrlTypes.
 * @li Set file chunk size using #jdd_BrowserConfigGetFileChunkSize.
 * @li Set the internal memory pool information using #jdd_BrowserConfigGetMemoryPoolInfo
 
 * @li Set the browser user agent using #jdd_BrowserConfigGetUserAgent.
 */

/**
 * @file jdd_browserconfig.h
 * @ingroup BrConfig
 * @brief This file contains the prototypes for configuration functions used by Browser Engine
 */

#ifndef _JDD_BROWSER_CONFIG_H
#define _JDD_BROWSER_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
/**
 
  
 *
 * When the content is fetched from the server the content is received in chunks. The browser
 * needs to follow the same mechanism for cache content (when request is retrieved from cache) as well. 
 * The application needs to use this function to specify the chunk size when the content is read
 * from the cache.
 */
JC_UINT32 jdd_BrowserConfigGetCacheChunkSize (void) ;

/**
 * @brief			Provides the stale value for all the cache entry.
 * @retval			Stale value for any cache entry.
 *
 * This function will be called to get the stale value for all the cache entries.
 * Sometimes the cache entries does not have a expiry date associated with it. These entries may reside
 * in the cache until the entire cache is cleared. To avoid that the application can specify the expity time
 * for the cache. This value will be used in case the cache does not have the expiry information. The
 * application can thus avoid cache information to be stored forever.
 */
JC_UINT32 jdd_CacheConfigGetStaleValue (void) ;

/**
 * @brief			Specifies whether the cache revalidation header is required explicitly to
 *					revalidate the content.
 * @retval			Boolean value.
 *
 * This function is specified so that the application can specify whether the revalidation header 
 * is explicitly required for cache revalidation. If this function returns true then the core will
 * not send request for revalidation unless revalidation header is present in the headers. If this function returns
 * false then the core will send the revalidation header even if revalidation header is not present
 * provided the ETAG or Last Modified headers are present.
 */
JC_BOOLEAN jdd_CacheConfigIsRevalHeaderExplicitlyReq (void) ;

/**
 * @brief			Specifies whether the cache file creation during initialization has to be done in 
 *					a Async way.
 * @retval			Boolean value.
 *
 * This function is specified so that the application can specify whether it wants the cache module to
 * create the files at one go or one file at a time. If this function returns E_TRUE then the cache will
 * return the control after creating one file. At a later point of time the application can call 
 * another cache API to continue with file creation.
 */
JC_BOOLEAN jdd_BrowserConfigCacheAsyncFileCreation (void) ;

/**
 * @brief			Provides the supported URL types
 * @retval			List of URL types.
 *
 * This function will be called to get the URL types that the application expects the browser to handle.
 *
 * This function is specified so that the application can specify URL types that needs to be handled 
 * internally by the browser. The URL types that are not returned by the function will not be handled 
 * by the browser and will be sent to the scheme listerner of the application. The applicaion can handle
 * the URL types in the listener.
 */
const JC_INT8 ** jdd_BrowserConfigUrlTypes (void) ;

/**
 * @brief			Provides that the maximum float value.
 
 *
 * This function will be called to get the maximum value of float that the browser can use for its
 * operations. The application needs to decide the value based on the application's requirement 
 * or/and platform's constraints.
 */
JC_FLOAT jdd_BrowserConfigGetMaxFloat (void) ;

/**
 * @brief			Provides that the minimum float value.
 * @retval			JC_FLOAT specifying the min float value.
 *
 * This function will be called to get the minimum value of float that the browser can use for its
 * operations. The application needs to decide the value based on the application's requirement 
 * or/and platform's constraints.
 */
JC_FLOAT jdd_BrowserConfigGetMinFloat (void) ;


/**
 
  
 *
 * When the content is fetched from the server the content is received in chunks. The browser
 * needs to follow the same mechanism for file content (when file scheme file://) as well. 
 * The application needs to use this function to specify the chunk size when the content is read
 * from a file.
 */
JC_UINT32 jdd_BrowserConfigGetFileChunkSize (void) ;

/**
 * @brief			Provides the internal memory poll information.
 * @param[out]		puiBlockNum	Specifies the number of blocks that needs to be allocated for the internal memory poll.
 * @param[out]		puiBlockSize Specifies the size of the block, within which the memory needs to be allocated from the internal memory poll. 
 *								 Any memory allocation greater than the block size will be allocated from the generic memory poll using memory abstraction layer.
 * 
 * Internal memory poll allows the core to manage the memory allocation and free for the 
 * small memory chunk. This API allows to configure the maximum number of blocks that are
 * controlled by the core and the small memory size. 
 *
 * Say for example: if the block count is configured as 320 and memory size is configured 
 * as 32 bytes. The core allocates 10KB of data (320 * 32) and manages for the small memory 
 * allocation which is less than 32 bytes. 
 * 
 * If the 320 blocks are occupied then for the small memory allocation, the core shall use the 
 * jdd_MemAlloc API. 
 *
 * Using this mechanism, the memory fragmentation can be avoided at the system level and 
 * core handles the small memory chunk. If the platform has the better memory management,
 * then the internal memory poll need not be enabled and in that case, this API will not be 
 * used. 
 *
 
 * it can go upto 50~100KB. This can be fine tuned based on the memory allocation pattern.  
 */
void jdd_BrowserConfigGetMemoryPoolInfo (JC_UINT32 *puiBlockNum, JC_UINT32 *puiBlockSize) ;

/**
 * @brief			Provides the maximum number of converted URI that can be stored by punycode converter (PC).
 
 *
 * Punycode converter converts a hostname into a encoded string so that the domain name can be resolved. 
 * The punycode converter module can store the URI and the encoded string so that each time it need not
 * encode the string. The maximum number of such encoded string that can be stored can be specifies using
 * this function.
 */
JC_UINT32 jdd_BrowserConfigGetMaxPCURI (void) ;

/**
 * @brief			Provides the browser user agent.
 * 
 */
const JC_INT8 * jdd_BrowserConfigGetUserAgent (void) ;

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
JC_BOOLEAN jdd_BrowserConfigNavigationPageFromCache (void) ;

/**
* @brief This function specifies the maximum number of windows that can be opened.
*
 
*
* This function returns the maximum number of windows that can be created/opened
* by the application.
*/
JC_UINT32 jdd_BrowserConfigMaxWindowCount (void) ;


#ifdef __cplusplus
}
#endif

#endif

/* End of File */



