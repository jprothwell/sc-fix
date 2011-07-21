/***************************************************************************
 * $Id: jdi_cache.h,v 1.47 2008/10/13 11:55:47 kumardevhtmlbrow Exp $
 * $Revision: 1.47 $
 * $Date: 2008/10/13 11:55:47 $
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
 * $Log: jdi_cache.h,v $
 * Revision 1.47  2008/10/13 11:55:47  kumardevhtmlbrow
 * Added the feature to cache the re-directed and post data content.
 *
 * Revision 1.46  2007/12/28 12:35:26  kumardevhtmlbrow
 * Doxygen changes
 *
 * Revision 1.45  2007/12/05 13:53:22  kumardevhtmlbrow
 * Update for cache chunk and interface file updated.
 *
 * Revision 1.44  2007/07/18 12:52:54  shreyasdevhtmlbrow
 * Handling of cache resizing
 *
 * Revision 1.43  2007/05/29 04:41:07  shreyasdevhtmlbrow
 * Error code added for big cache content
 *
 * Revision 1.42  2007/04/09 05:48:25  kumardevhtmlbrow
 * Update
 *
 * Revision 1.41  2007/03/22 07:20:08  shreyasdevhtmlbrow
 * Boolean changed to bit flags
 *
 * Revision 1.40  2007/02/02 06:55:01  kumardevhtmlbrow
 * Moved the mem mgr header file to the cache file's and removed it from the exposed header file.
 *
 * Revision 1.39  2006/12/28 11:40:22  kumardevhtmlbrow
 * Update
 *
 * Revision 1.38  2006/12/19 05:08:01  kumardevhtmlbrow
 * Updated
 *
 * Revision 1.37  2006/12/05 13:09:09  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.36  2006/09/06 14:11:09  kumardevhtmlbrow
 * Update for handling meta cache.
 *
 * Revision 1.35  2006/08/30 04:54:53  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.34  2006/04/11 07:22:54  kumardevhtmlbrow
 * Lint Warning Fix
 *
 * Revision 1.33  2006/04/05 10:34:44  kumardevhtmlbrow
 * Lint Warning Update ...
 *
 * Revision 1.32  2006/03/04 05:48:35  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.31  2006/01/03 10:32:49  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.30  2005/12/28 05:53:39  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.29  2005/12/21 15:44:19  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.28  2005/12/20 15:52:00  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.27  2005/12/20 07:27:22  kumardevhtmlbrow
 * Updated the cache flush
 *
 * Revision 1.26  2005/12/12 15:29:01  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.25  2005/12/05 14:06:39  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.24  2005/12/05 13:14:24  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.23  2005/09/16 14:51:48  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.22  2005/09/12 12:20:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.21  2005/09/01 08:19:14  kumardevhtmlbrow
 * no message
 *
 * Revision 1.20  2005/08/25 14:20:37  rajivdevhtmlbrow
 * error code added
 *
 * Revision 1.19  2005/08/24 14:46:13  kumardevhtmlbrow
 * Updated the cache after integration with BE.
 *
 * Revision 1.18  2005/08/17 06:23:56  kumardevhtmlbrow
 * Updated the jdd_MemAlloc to jdi_MemAlloc function.
 *
 * Revision 1.17  2005/08/12 10:57:42  kumardevhtmlbrow
 * no message
 *
 * Revision 1.16  2005/08/10 14:05:27  rajivdevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.15  2005/07/22 09:29:54  kumardevhtmlbrow
 * Added invalidate object and service
 *
 * Revision 1.14  2005/07/21 10:46:13  kumardevhtmlbrow
 * Added the additional function to free the retrieve handle
 *
 * Revision 1.13  2005/07/15 15:40:23  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.12  2005/06/28 05:51:59  kumardevhtmlbrow
 * no message
 *
 * Revision 1.11  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.10  2005/06/23 14:31:05  kumardevhtmlbrow
 * no message
 *
 * Revision 1.9  2005/06/23 09:49:45  kumardevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/06/21 06:57:58  deepakrdevhtmlbrow
 * Commited by Deepak On 20th june
 *
 * Revision 1.7  2005/06/17 05:28:05  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/06/17 04:46:08  kumardevhtmlbrow
 * Updated the cache module ..
 *
 * Revision 1.5  2005/05/18 11:39:26  kumardevhtmlbrow
 * Cache Updated by Deepak Relon
 *
 * Revision 1.4  2005/05/13 10:05:40  deepakrdevhtmlbrow
 * Updated On 13th May
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_cache.h
 * @ingroup browser
 * @brief Provides the set of interface for the browser application for updating 
 * the content that are received into the cache.
 * 
 * This set of Interface is used for Initializing the cache module, adding/retrieving header
 * and adding/retrieving content to cache files. It also provides feature to clear the cache
 * file content, to set/get file sizes, to enable/disable the cache. It also provides feature
 * for Invalidating particular URI or Domain (a set of URI that share the same URI prifix).
 * With the help of these api's application can add various domain for Operator specific cache
 * and can clear these domain list also. Application can forcefully flush the content to cache
 * files also. Validation of cache files content is also been taken care internally. If any 
 * problem occurs while retrieving/adding, for that also a set of api's are provided. And last
 * but not the least, most important, an api is also provided for Deinitializing the cache module.
 */

#ifndef _JDI_CACHE_H
#define _JDI_CACHE_H

#include <jdi_browsermodule.h>

/**
 * @brief This enumeration contains the cache storage type.
 */
typedef enum _cache_storage_type
{
	E_CACHE_OPERATOR_SPECIFIC, /**< Specifies the Cache operator specific cache. */
	E_CACHE_OTHERS	/**< Specifies the general/user specific cache */
} E_CACHE_STORAGE_TYPE ;

/**
 * @brief This structure describes the cache header information.
 */
typedef struct _cache_header
{
	JC_INT8				*pcURL ; /**< Specifies the url that need to be stored or retrieved. */
	JC_INT8				*pcPostData ; /**< Specifies the post data corresponding to url. */
	JC_UINT32			uiPostLength ; /**< Specifies the length of postdata. */
	JC_UINT8			*pucHeader ; /**< Specifies the header content corresponding to url. */
	JC_UINT32			uiHeaderLen ; /**< Specifies the length of header content. */
	JC_UINT16			uhStatusCode ; /**< Specifies the HTTP status code. */
	JC_BOOLEAN			bHeaderType ;  /**< Specifies the header type of corresponding url. */
	JC_INT32			iContentLength ; /**< Specifies the content length of the url and -1 if size is not known. */
	JC_INT8				*pcMimeType ; /**< Specifies the Mime type corresponding to url. */
	JC_UINT32			uiMaxAge ;  
	JC_BOOLEAN			bIsMustRevalidate ; /**< Specifies the must revalidate value corresponding to url as TRUE or FALSE */
	JC_BOOLEAN			bIsPageRequest ; /**< Specifies whether the cache entry is for a main page request */
} ST_CACHE_HEADER ;

/**
 * @brief This structure describes the cache extension header information, which is used to 
 
 */
typedef struct _cache_header_ext
{
	JC_INT8		*pcURL ; /**< Specifies the URL that need to be stored or retrieved */
	JC_INT8		*pcPostData ; /**< Specifies the post data correspoding to url. */
	JC_UINT32	uiPostLength ; /**< Specifies the length of postdata. */
	JC_BOOLEAN	bIsMaxAgeSet ;  
	JC_UINT32	uiMaxAge ;  
	JC_BOOLEAN  bIsMustRevalidate ; /**< Specifies the must revalidate value corresponding to url as TRUE or FALSE */
} ST_CACHE_HEADER_EXT ;

/**
 * @brief This structure describes the cache configuration information.
 */
typedef struct _cache_config
{
	JC_UINT32		uiCacheSize ; /**< Specifies the File size for each cache file. */
	JC_UINT32		uiChunkSize ; /**< Specifies the Chunk size for each cache file. */
} ST_CACHE_CONFIG ;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function initializes the cache. 
 * @param[in]	pmGeneralStorage Specifies the General Cache file Name
 * @param[in]	uiGeneralSize Specifies the General Cache file Size
 * @param[in]	pmOperatorStorage Specifies the operator specific Cache file Name
 * @param[in]	uiOperatorSize Specifies the operator specific Cache file size
 * @param[in]	pmDomainInfoStorage Specifies the Domain Info storage file name.
 * @param[in]	ucNumOfCacheFile Specifies the number of cache files to be created.
 * @param[in]	vMemHandle Specifies the Memory Handle.
 * @param[out]	pvHandle Specifies a pointer to JC_HANDLE, used as out parameter.
 * @retval		JC_OK On successful initialize of the cache
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_ERR_INVALID_PARAMETER On passing Invalid Handle.
 * @retval		JC_ERR_ASYNC_CALL Specifies that the cache initialization has to be continued
 *				at a later point of time by using the API #jdi_CacheInitContinue.
 * @retval		Others 
 * @see			jdi_CacheDeinitialize
 * @note		The parameter which should not be NULL are vHandle and pmGeneralStorage.
 * @note		The caller function should pass pvHandle with all the other cache API.
 *
 * This function is used to Initialize the cache file and is used when application want to
 * store the content in cache files. The function Initializes the cache with the File name
 * and the size of the General and Operator specific cache that are mentioned as the input 
 * parameter. It also initializes other values such as the chunk size, No of chunks and 
 * initializing the chunks. Application can Initialize the general cache only Instead of
 * Initializing both at one time by specifying the pmOperatorStorage as NULL and uiOeratorSize
 * as Zero. But Operator cache can't be Initialized alone.
 */

JC_RETCODE jdi_CacheInitialize (const JC_CHAR					*pmGeneralStorage,
								JC_UINT32						uiGeneralSize,
								const JC_CHAR					*pmOperatorStorage,
								JC_UINT32						uiOperatorSize, 
								const JC_CHAR					*pmDomainInfoStorage,
								JC_HANDLE						vMemHandle,
								JC_HANDLE						*pvHandle) ;


/**
 * @brief		This API is used to continue with cache initialization
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @retval		JC_OK On successful initialize of the cache
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_ERR_INVALID_PARAMETER On passing Invalid Handle.
 * @retval		JC_ERR_ASYNC_CALL Specifies that the cache initialization has to be continued
 *				at a later point of time by using the API #jdi_CacheInitContinue.
 * @note		The parameter which should not be NULL are vHandle and pmGeneralStorage.
 * @note		The caller function should pass pvHandle with all the other cache API.
 *
 * During initialization the cache files are created. If the application wants to create one file at
 * a time and then later continue with next file creation then the cache will return the error JC_ERR_ASYNC_CALL
 * during init. The application then needs to call this API at a later point of time. This API may also
 * return the same error and the application will have to call this API until it returns JC_OK.
 *
 * If this API returns any other error then the application should explicitly call the
 * cache deinit function.
 */
JC_RETCODE jdi_CacheInitContinue (JC_HANDLE		vHandle) ;


/**
 * @brief	This function sets the size of the cache file based on the cache storage type.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	eStorageType Specifies the type of cache whose size need to be set.
 * @param[in]	uiSize Specifies the size of cache that need to be set.
 * @see			jdi_CacheGetSize
 * @note		The parameter which should not be NULL is vHandle.
 * 
 * This function is used when application want to set the size of cache file. It can
 * be done by specifying the size and type of cache for which size need to be set.
 */

JC_RETCODE jdi_CacheSetSize (JC_HANDLE				vHandle, 
							 E_CACHE_STORAGE_TYPE	eStorageType,
							 JC_UINT32				uiSize) ;
/**
 * @brief	This function gets the size of the cache as per cache storage type and store it in puiSize.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	eStorageType Specifies the type of cache whose size need to be get.
 * @param[out]	puiSize Specifies the pointer variable in which it returns the size of cache.
 * @retval		JC_OK On success
 * @see			jdi_CacheSetSize
 * @note		The parameter which should not be NULL is vHandle.
 * 
 * This function is used when application want to gets the size of cache file. It can
 * be done by specifying the size and type of cache for which size need to be obtained.
 */

JC_RETCODE jdi_CacheGetSize (JC_HANDLE vHandle, E_CACHE_STORAGE_TYPE eStorageType,
							 JC_UINT32 *puiSize) ;

/**
 * @brief	This function allows to enable or disable the cache.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	eStorageType Specifies the type of cache which need to be enable or disable.
 * @param[in]	bIsEnable Specifies the value for enabling or disabling the cache.
 * @note		The parameter which should not be NULL is vHandle.
 * 
 * This function is used for enabling or disabling the cache. Application can disable
 * or enable the cache of specific type. The function has a boolean value as input
 * parameter and based on the value the cache is enabled or disabled.
 */

void jdi_CacheEnable (JC_HANDLE vHandle, E_CACHE_STORAGE_TYPE eStorageType,
							JC_BOOLEAN bIsEnable) ;

/**
 * @brief	This function removes all the cache contents based on the type of cache.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	eStorageType Specifies the type of cache which need to be clear.
 * @retval		JC_OK On success
 * @retval		JC_ERR_CACHE_DISABLED If cache is disabled.
 * @retval		Others 
 * @note		The parameter which should not be NULL is vHandle.
 * 
 * This function is used when application want to clears the content stored in cache files.
 * This can be done by specifying the cache handle along with the type of cache for which 
 * content needs to be cleared. 
 */

JC_RETCODE jdi_CacheClear (JC_HANDLE vHandle, E_CACHE_STORAGE_TYPE eStorageType) ;

/**
 * @brief	This function add the Header into the cache of particular type.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	pstCacheHeader Specifies the structure which contains the input values of header.
 * @param[in]	bIsOpSpecific Specifies the whether content need to be added to Operator Specific file or not.
 * @param[out]	pvAddHandle Specifies a pointer to JC_HANDLE, used as out parameter and
 *				used while adding content or calling add error function.
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_ERR_CACHE_DISABLED If cache is disabled.
 * @retval		JC_OK On success.
 * @retval		others.
 * @see			jdi_CacheAddContent
 * @see			jdi_CacheAddError
 * @note		The parameter which should not be NULL are vHandle, pstCacheHeader and pvAddHandle.
 * 
 * This function is called when application want to add the page header to cache file. 
 * The function writes or add the page header information to the cache file of particular
 * type based on bIsOpSpecific parameter if it is TRUE it means content will be added to 
 * Operator specific cache otherwise to general/user cache. This function fills the add
 * page header info structure and returns the vAddHandle. 
 */

JC_RETCODE jdi_CacheAddHeader (JC_HANDLE vHandle, ST_CACHE_HEADER *pstCacheHeader, 
							   JC_BOOLEAN bIsOpSpecific, JC_HANDLE *pvAddHandle) ;

/**
 
 * content available in the cache database.
 * @param[in] vHandle Specifies the cache handle returned from the cache initialize function.
 * @param[in] pstCacheHeaderExt Specifies the cache header extension information.
 */
JC_RETCODE jdi_CacheUpdateHeader (JC_HANDLE					vHandle,
								  const ST_CACHE_HEADER_EXT *pstCacheHeaderExt) ;

/**
 * @brief	This function add the cache content into cache file based on vAddHandle.
 * @param[in]   vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	vAddHandle Specifies the handle to based on which content are going to be add in cache file.
 * @param[in]	pucBuffer Specifies the buffer which contains the data to be written into the cache file
 * @param[in]	uiLength Specifies the buffer length.
 * @param[in]	bIsMoreData Specifies the whether still more data is pending or not.
 * @retval		JC_ERR_FILE_WRITE If write function of cache file fails.
 * @retval		JC_ERR_CACHE_WRITE If writing to cache file fails.
 * @retval		JC_OK On success.
 * @retval		Others.
 * @see			jdi_CacheAddHeader
 * @see			jdi_CacheAddError
 * @note		The parameter which should not be NULL are vHandle, pucBuffer and vAddHandle.
 *
 * The function is used for writing the page content into the cache file, based on the 
 * vAddHandle information. This function writes the buffer content to the cache file if
 * uiLength is > 0 and also bIsMoreData is TRUE. 
 */

JC_RETCODE jdi_CacheAddContent (JC_HANDLE vHandle, JC_HANDLE vAddHandle, 
								const JC_UINT8 *pucBuffer, JC_UINT32 uiLength, 
								JC_BOOLEAN bIsMoreData) ; 

/**
 * @brief	This function delete the cache content if any error occurs during writting the cache content to file.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	vAddHandle Specifies the handle to based on which content are going to be add in cache file.
 * @retval		JC_ERR_FILE_READ If fails during reading from cache file.
 * @retval		JC_ERR_FILE_WRITE If fails during writing to cache file.
 * @retval		JC_OK On success.
 * @retval		Others.
 * @see			jdi_CacheAddHeader
 * @see			jdi_CacheAddContent
 * @note		The parameter which should not be NULL are vHandle, pucBuffer and vAddHandle.
 * 
 * This function delete the cache content if any error occurs during writing the cache content to file.
 */

JC_RETCODE jdi_CacheAddError (JC_HANDLE vHandle, JC_HANDLE vAddHandle) ;

/**
 * @brief	This function retrieving Header from the cache of particular type.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	pcURL Specifies the URL corresponding to which Header needs to be retrieved from the cache file.
 * @param[in]	pcPostData Specifies the post data with respect to the URL.
 * @param[in]	uiPostLength Specifies the post data length.
 * @param[out]	pstCacheHeader Specifies the out parameter which will contain the page header values.
 * @param[out]	pvRetrieveHandle Specifies a pointer to JC_HANDLE, used as out parameter and
 *				used while retrieving content or calling retrieve error function.
 * @retval		JC_OK On success.
 * @retval		JC_ERR_CACHE_DISABLED If cache is disabled.
 * @retval		JC_ERR_CACHE_HEADER_NOT_EXIST If header not found in cache file.
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		Others.
 * @see			jdi_CacheRetrieveContent
 * @see			jdi_CacheRetrieveError
 * @note		The parameter which should not be NULL are vHandle, pvRetrieveHandle, pstCacheHeader and pcURL. 
 * 
 * This function is used when application want to retrieve any header from the cache file.
 * For this application has to specify the cache handle and URL. The function retrieves
 * the page header from the cache of particular type. This function fills the cache header
 * structure and return the header information. It also return the error code
 * JC_ERR_CACHE_PAGE_EXPIRED if page is expired in Cache file along with cache headers. 
 */

JC_RETCODE jdi_CacheRetrieveHeader (JC_HANDLE vHandle, const JC_INT8 *pcURL,
									const JC_INT8 *pcPostData, JC_UINT32 uiPostLength,
									ST_CACHE_HEADER *pstCacheHeader, 
									JC_HANDLE *pvRetrieveHandle) ;

/**
 * @brief	This function retrieves Content from the cache file of particular type.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	vRetrieveHandle Specifies the handle to corresponding to which content need to retrieved.
 * @param[in]	pucBuffer Specifies the buffer which need to be filled with cache page content.
 * @param[in]	iBufSize Specifies the buffer length.
 * @param[out]	piCopiedSize Specifies the out parameter which will contain the length of actual data copied into buffer.
 * @param[out]	pbIsMoreData Specifies whether still more data is left in cache file for reading or not.
 * @retval		JC_OK On success.
 * @retval		JC_ERR_FILE_READ If failed during reading from cache file.
 * @retval		Others.
 * @see			jdi_CacheRetrieveHeader
 * @see			jdi_CacheRetrieveError
 * @note		The parameter which should not be NULL are vHandle, pvRetrieveHandle, pucBuffer, pbIsMoreData and piCopiedSize. 
 * 
 * This function is used for getting the content from the cache file based on vRetrieveHandle, 
 * specified as input parameter. This function fills the buffer and return it. It also returns
 * the length of data that is copied into the buffer. When there is no content left it makes
 * pbIsMoreData as FALSE.
 */

JC_RETCODE jdi_CacheRetrieveContent (JC_HANDLE vHandle, JC_HANDLE vRetrieveHandle, 
									 JC_UINT8 *pucBuffer, JC_INT32 iBufSize, 
									 JC_INT32 *piCopiedSize, JC_BOOLEAN	*pbIsMoreData) ;

/**
 * @brief	This function free the vRetrieveHandle if any error occurs during reading from the cache file.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	vRetrieveHandle Specifies the handle to corresponding to which content need to retrieved.
 * @retval		JC_OK On success.
 * @see			jdi_CacheRetrieveHeader
 * @see			jdi_CacheRetrieveContent
 * @note		The parameter which should not be NULL are vHandle and pvRetrieveHandle.
 * 
 * This function is called if any error occur during retrieving the content from the cache file. 
 * The function free the vRetrieveHandle and free the memory if allocated inside the function.
 */

JC_RETCODE jdi_CacheRetrieveError (JC_HANDLE vHandle, JC_HANDLE vRetrieveHandle) ;

/**
 * @brief	This function invalidate the cache object uniquely identified by the given URI in cache file.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	pcURL Specifies the URL which need to be invalidated.
 * @param[in]   pcPostData Specifies the post data corresponding to url which need to invalidate.
 * @param[in]   uiPostLength Specifies the length of post data.
 * @retval		JC_ERR_FILE_READ If fails during reading from cache file.
 * @retval		JC_ERR_FILE_WRITE If fails during writing to cache file.
 * @retval		JC_ERR_CACHE_HEADER_NOT_EXIST If header not found in cache file.
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @retval		Others. 
 * @see			jdi_CacheInvalidateService
 * @note		The parameter which should not be NULL are vHandle and pcURL.
 * 
 * This function is called when application want to invalidate the cache object uniquely
 * identified by the given URI in cache file. The function invalidate the content in cache
 * file as per the url and post data specified. 
 */

JC_RETCODE jdi_CacheInvalidateObject (JC_HANDLE vHandle, const JC_INT8 *pcURL,
									  const JC_INT8 *pcPostData, JC_UINT32 uiPostLength) ;

/**
 * @brief	This function invalidate the cache content of a particular domain in cache file.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	pcURL Specifies the URL Domain which need to be invalidated.
 * @retval		JC_ERR_FILE_READ If fails during reading from cache file.
 * @retval		JC_ERR_FILE_WRITE If fails during writing to cache file.
 * @retval		JC_ERR_CACHE_HEADER_NOT_EXIST If header not found in cache file.
 * @retval		JC_ERR_CACHE_DOMAIN_MISMATCH If domain mismatch occurs.
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @retval		Others. 
 * @see			jdi_CacheInvalidateObject
 * @note		The parameter which should not be NULL are vHandle and pcURL.
 * 
 * This function is called when application want to invalidate the cache content of a
 * particular domain, in cache file as specified in URI. This function invalidates all
 * the objects that share the same URI prefix. 
 */

JC_RETCODE jdi_CacheInvalidateService (JC_HANDLE vHandle, const JC_INT8 *pcURL) ;

/**
 * @brief	This function Add the operator Specific Domain names to Operator Domain Info file.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	pcDomainName Specifies the Name of the Domain as input parameter.
 * @retval		JC_ERR_MEMORY_ALLOCATION On memory allocation error.
 * @retval		JC_OK On success.
 * @see			jdi_CacheClearDomain
 * @note		The parameter which should not be NULL are vHandle and pcDomainName.
 * 
 * This function is used for Adding the operator Specific Domain names to Operator Domain
 * Info file.
 */

JC_RETCODE jdi_CacheAddDomain (JC_HANDLE vHandle, const JC_INT8 *pcDomainName) ;

/**
 * @brief	This function Clears or delete the domain specific file.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @see			jdi_CacheAddDomain
 * @note		The parameter which should not be NULL is vHandle.
 * 
 * The function is used for clearing the domain specific file, which contains the domain
 * info for different operators.
 */

void jdi_CacheClearDomain (JC_HANDLE vHandle) ;

/**
 * @brief	This function deinitializes the cache. 
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @retval		JC_OK On success.
 * @see			jdi_CacheInitialize
 * @note		The parameter which should not be NULL is vHandle.
 * 
 * This function is used for deinitializes both the cache handle by deinitailising 
 * all the handles used.
 */

JC_RETCODE jdi_CacheDeinitialize (JC_HANDLE vHandle) ;

/**
 * @brief	This function Flushes all the data which so ever in the buffer and reopen the files again.
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize function.
 * @param[in]	eStorageType Specifies the type of cache which need to be flushed.
 * @retval		JC_ERR_FILE_OPEN If failed in opening the file.
 * @retval		JC_ERR_CACHE_DISABLED If cache file is disabled.
 * @retval		JC_OK On success.
 * @note		The parameter which should not be NULL is vHandle.
 * 
 * This function is used for flushing all the data which so ever in the buffer and 
 * reopen the files again.
 */

JC_RETCODE jdi_CacheFlush (JC_HANDLE vHandle, E_CACHE_STORAGE_TYPE eStorageType) ;

/**
 * @brief		This API Retrieves list of URL's (page request) which are currently cached.
 *
 * @param[in]	vHandle Specifies the cache handle returned from cache initialize API.
 * @param[in]	vURLHandle Specifies the URL handle which was previously returned with the same
 *				API
 * @param[out]	ppcURL The API will return the cached URL in this parameter. 
 * @param[out]	pvNextURLHandle The API will return the handle to the next cached URL in this 
 *				parameter.
 * @retval		JC_OK if the cached URL is retrieved successfully.
 * @retval		JC_ERR_CACHE_EMPTY If the cache is empty.
 * @see			jdi_CacheFreeCachedURL
 *
 * The application needs to use this API to get the list of cached URL's.
 * The API is capable of returning only one URL when it is called (not a list of URL). The application
 * has to call this API again to get the next URL in the cache (to avoid high memory usage).
 *
 * The first time this function is called the value of the URL handle (vURLHandle) will be NULL.
 * If the Cache is able to retrieve any cached URL then it will return the same in the parameter ppmURL
 * and also return the handle to get the next cached URL (pvNextURLHandle). The application can then 
 * call the same API again, this time the vURLHandle will be the pvNextURLHandle that was returned from
 * the previous call. The same process needs to be followed to get the list of cached URL.
 *
 * The Cache can intimate the application that there are no more cached URL's in 2 ways.
 * First way is that the cache will return the pvNextURLHandle as NULL and hence no more URL to retrieved.
 * The second way is that it will return the value of ppcURL as NULL and return as JC_OK. i.e. even though
 * the application passed the correct vURLHandle (i.e. pvNextURLHandle of a previous call) the cache could not
 * retrieve anymore cached URL (because the cached URL list has finished). The application should not treat the
 * second way as a error. Instead it means that no more cached URL's to be retrieved.
 *
 * The application needs to use the API #jdi_CacheFreeCachedURL to free the URL that is returned from
 * this API.
 *
 * @note The application should not delete the cache while retrieving the cached URL list.
 *
 */
JC_RETCODE jdi_CacheGetCachedURL (JC_HANDLE			vHandle,
								  JC_HANDLE			vURLHandle,
								  JC_INT8			**ppcURL,
								  JC_HANDLE			*pvNextURLHandle) ;

/**
 * @brief			Frees the cached URL
 *
 * @param[in]		vHandle Specifies the handle returned by cache initialize.
 * @param[in,out]	pcURL Specifies the cached URL to be freed.
 * @retval			void
 *
 * The application needs to use this API to free the URL that it has received after calling
 * the API #jdi_CacheGetCachedURL.
 *
 */
void jdi_CacheFreeCachedURL (JC_HANDLE		vHandle,
							 JC_INT8		*pcURL) ;

/**
 * @brief	This function is used for getting the details about each cache file of particular type.
 * @param[in]	eStorageType Specifies the type of cache.
 * @param[in]	uiFileIndex Specifies the number of cache file for which details needs be gathered.
 * @param[out]	pstCacheConfig Specifies the size of cache file and chunk size of that particular file.
 * 
 * This function is used for getting the details about each cache file of particular type.
 * Here the details about the each cache file is going to be store in pstCacheConfig structure.
 */

void jdd_CacheConfigGetSizeInfo (E_CACHE_STORAGE_TYPE eStorageType, JC_UINT32 uiFileIndex, 
								 ST_CACHE_CONFIG *pstCacheConfig) ;

#ifdef __cplusplus
}	
#endif

/**
 * @brief Specifies the error code while getting the LRU page. 
 */ 
#define JC_ERR_CACHE_LRU				TERMINAL_ERR_CODE(CACHE_MODULE, 0x01) 
/**
 * @brief Specifies the error code when cache is disabled. 
 */
#define JC_ERR_CACHE_DISABLED			TERMINAL_ERR_CODE(CACHE_MODULE, 0x02) 
/**
 * @brief Specifies the error code while reading the header from cache file.
 */
#define JC_ERR_CACHE_HEADER_READ		TERMINAL_ERR_CODE(CACHE_MODULE, 0x03)
/**
 * @brief Specifies the error code while post data length didn't match with cache post data length.
 */
#define JC_ERR_CACHE_POSTLEN			TERMINAL_ERR_CODE(CACHE_MODULE, 0x04)
/**
 * @brief Specifies the error code while writing to cache file.
 */
#define JC_ERR_CACHE_WRITE				TERMINAL_ERR_CODE(CACHE_MODULE, 0x05)
/**
 * @brief Specifies the error code while writing page header to cache file.
 */
#define JC_ERR_CACHE_HEADER_WRITE		TERMINAL_ERR_CODE(CACHE_MODULE, 0x06) 
/**
 * @brief Specifies the error code while writing particular header item to cache file.
 */
#define JC_ERR_CACHE_HEADER_ITEM		TERMINAL_ERR_CODE(CACHE_MODULE, 0x07)
/**
 * @brief Specifies the error code when the page to be cached is bigger than the cache file size.
 */
#define JC_ERR_CACHE_FULL				TERMINAL_ERR_CODE(CACHE_MODULE, 0x08)
/**
 * @brief Specifies the error code while Deleting the Expired Pages.
 */
#define JC_ERR_CACHE_DELETE_EXPIRE		TERMINAL_ERR_CODE(CACHE_MODULE, 0x09)
/**
 * @brief Specifies the error code while retrieving header and if it wont exist in cache file.
 */
#define JC_ERR_CACHE_HEADER_NOT_EXIST	TERMINAL_ERR_CODE(CACHE_MODULE, 0x0A)
/**
 * @brief Specifies the error code if no more Expired page found.
 */
#define JC_ERR_CACHE_NO_EXPIRE_PAGE		TERMINAL_ERR_CODE(CACHE_MODULE, 0x0B)
/**
 * @brief Specifies the error code to indicate the page is expired in Cache.
 */
#define JC_ERR_CACHE_PAGE_EXPIRED		TERMINAL_ERR_CODE(CACHE_MODULE, 0x0C)
/**
 * @brief Specifies the error code while matching the domain.
 */
#define JC_ERR_CACHE_DOMAIN_MISMATCH	TERMINAL_ERR_CODE(CACHE_MODULE, 0x0D)
/**
 * @brief Specifies the error code while opening the Operator Info file.
 */
#define JC_ERR_CACHE_OPERATOR_FILE		TERMINAL_ERR_CODE(CACHE_MODULE, 0x0E)
/**
 
 */
#define JC_ERR_CACHE_OPERATOR_FILE_EOF	TERMINAL_ERR_CODE(CACHE_MODULE, 0x0F)
/**
 * @brief Specifies the error code to indicate the mismatch between cache file size during validation.
 */
#define JC_ERR_CACHE_FILESIZE_MISMATCH	TERMINAL_ERR_CODE(CACHE_MODULE, 0x10)
/**
 * @brief Specifies the error code to indicate the master cache file is corrupted.
 */
#define JC_ERR_CACHE_CORRUPT			TERMINAL_ERR_CODE(CACHE_MODULE, 0x11)
/**
 * @brief Specifies that the data to be added to the cache is too big.
 */
#define JC_ERR_CACHE_ENTRY_TOO_BIG		TERMINAL_ERR_CODE(CACHE_MODULE, 0x12)

#endif

/* End of File */


