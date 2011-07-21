/***************************************************************************
 
 * $Revision: 1.29 $
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
 
 * Revision 1.29  2009/01/15 12:56:18  arpitadevhtmlbrow
 
 *
 * Revision 1.28  2008/09/11 07:11:59  kavithadevhtmlbrow
 
 *
 * Revision 1.27  2007/05/30 14:39:03  shreyasdevhtmlbrow
 * Updated the doxygen comments.
 *
 * Revision 1.26  2007/05/21 14:16:15  kumardevhtmlbrow
 
 *
 * Revision 1.25  2007/01/16 07:09:34  kumardevhtmlbrow
 
 *
 * Revision 1.24  2006/10/10 07:59:46  kumardevhtmlbrow
 * Update
 *
 * Revision 1.23  2006/09/04 05:18:21  kumardevhtmlbrow
 * Update ..
 *
 * Revision 1.22  2006/06/16 13:18:34  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.21  2006/06/15 05:58:02  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.20  2006/03/16 15:47:25  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.19  2006/03/16 07:35:41  kumardevhtmlbrow
 * Added the application to add the session header
 *
 * Revision 1.18  2006/02/09 04:28:55  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.17  2006/01/06 14:43:38  rajivdevhtmlbrow
 
 *
 * Revision 1.16  2005/12/23 11:37:59  rajivdevhtmlbrow
 
 *
 * Revision 1.15  2005/12/12 15:29:01  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.14  2005/09/12 12:20:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.13  2005/09/01 12:44:39  rajivdevhtmlbrow
 
 *
 * Revision 1.12  2005/08/26 12:28:46  kumardevhtmlbrow
 * no message
 *
 * Revision 1.11  2005/08/01 06:21:24  rajivdevhtmlbrow
 * remove unnecessary error code - rajiv
 *
 * Revision 1.10  2005/07/30 14:39:47  kumardevhtmlbrow
 * no message
 *
 * Revision 1.9  2005/07/30 13:55:10  rajivdevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/07/27 13:04:55  rajivdevhtmlbrow
 * add error code
 *
 * Revision 1.7  2005/07/18 14:29:25  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/06/24 13:36:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/06/24 10:06:48  rajivdevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/06/01 07:22:08  rajivdevhtmlbrow
 * Change the variable type
 *
 * Revision 1.2  2005/05/30 15:36:35  kumardevhtmlbrow
 
 *
 * Revision 1.1  2005/05/04 06:40:00  kumardevhtmlbrow
 
 *
 *  
 ***************************************************************************/

#ifndef _JDI_BOOKMARK_H
#define _JDI_BOOKMARK_H

/** 
 
 * 
 * Browser core provides generic component which can be used by the application to manage the user created favourites.
 
  
 * 
 
  
 * 
 
  
 * 
 
  
 * 
 
  
 * 
 
  
 * 
 
  
 * 
 
  
 * 
 */

/**
 
 
 * @brief Defines the exposed data structures and API's for the browser application to 
 
 * 
 
  
 * 
 */

/** @cond */
#include <jdi_browsermodule.h>
/** @endcond */

/**
 * @brief Specifies the operation is not succeeded as the category has a read-only attribute.
 */
#define JC_ERR_BOOKMARK_READ_ONLY_CATEGORY 			TERMINAL_ERR_CODE(BOOKMARK_MODULE, 0x01)

/**
 
 */
#define JC_ERR_BOOKMARK_READ_ONLY_BOOKMARK 			TERMINAL_ERR_CODE(BOOKMARK_MODULE, 0x02)

/**
 * @brief Specifies that total number of category exceeds the specified category limit.
 */
#define JC_ERR_BOOKMARK_CATEGORY_LIMIT	 			TERMINAL_ERR_CODE(BOOKMARK_MODULE, 0x03)

/**
 * @brief Specifies that total number of favourite within category exceeds the specified favourite limit.
 */
#define JC_ERR_BOOKMARK_FAVOURITE_LIMIT 			TERMINAL_ERR_CODE(BOOKMARK_MODULE, 0x04)

/**
 
 */
#define JC_ERR_BOOKMARK_CATEGORY_EXIST				TERMINAL_ERR_CODE(BOOKMARK_MODULE, 0x05)

/**
 
 */
#define JC_ERR_BOOKMARK_FAVOURITE_EXIST				TERMINAL_ERR_CODE(BOOKMARK_MODULE, 0x06)

/**
 
 */
#define JC_ERR_BOOKMARK_FILE_CORRUPT				TERMINAL_ERR_CODE(BOOKMARK_MODULE, 0x07)

#ifdef __cplusplus
extern "C" {
#endif

/**
 
 *
 
 */

typedef struct bookmarkAttributes
{
	JC_CHAR 			*pmURL ; /**< Specifies the URL of the favorite. */
	JC_CHAR	 			*pmTitle ; /**< Specifies the title associated with this page (if any). */
	JC_INT8 			*pcCharSet ; /**< Specifies the charset associated with this page (if any). */
	JC_INT8				*pcUserData ; /**< Specifies the user data associated with this URL (if any).*/
	JC_INT8 			*pcImgMimeType ; /**< Specifies the type of the image that needs to be associated with this URL, which is available in the link tag of the page. */
	JC_UINT8 			*pucImgBuffer ; /**< Specifies the image for this URL which shall be available in the link tag of the page. */
	JC_INT32			iPageXPos ; /**< Specifies the x co-ordinates where the current focus is available. This value can be used in the future to navigate directly to the specified location of the page.*/
	JC_INT32			iPageYPos ;	/**< Specifies the y co-ordinates where the current focus is available. This value can be used in the future to navigate directly to the specified location of the page.*/
	JC_UINT32 			uiImgSize ;	/**< Specifies the size of the image for the pucImgBuffer. */
	JC_UINT32 			uiHttpStatus ; /**< Specifies the HTTP Status of the page. */
	JC_UINT32			uiElementType ; /**< Specifies the element type */
	JC_UINT32 			uiUserDataLen ; /**< Specifies the user data length */
	JC_BOOLEAN			bIsReadOnly ;  
	ST_CUTILS_TIME		stLastVisitedTime ;  
	ST_CUTILS_TIME		stCreatedTime ;  
	struct  bookmarkAttributes	*pstNext ;  
} ST_BOOKMARK_ATTRIBUTES ;

/**
 * @brief Defines the category information.
 * 
 
 */

typedef struct categoryList
{
	ST_BOOKMARK_ATTRIBUTES	*pstBookmarkAttributes ;  
	JC_CHAR 				*pmCategory ; /**< Specifies the name of the category. */
	JC_BOOLEAN				bIsReadOnly ; /**< Specifies whether this category is read only. */
	JC_UINT32				uiCategoryId ;  
	JC_UINT32				uiParentCategoryId ;  
	struct categoryList 	*pstChildCategory ; /**< Specifies the reference to the sub-category (child category). */
	struct categoryList		*pstParentCategory ; /**< Specifies the reference to its parent-category (parent category). */
	struct categoryList 	*pstNext ; /**< Specifies the reference to the next category list. */
} ST_CATEGORY_LIST ;

/**
 * @brief Defines the category information.
 */

typedef struct category_info
{
	JC_UINT32			uiCategoryCount ; /**< Specifies the number of categories available in the specified category (does not count the sub-category information). */
	JC_UINT32			uiBookmarkCount ;  
	JC_UINT32			uiCategoryTotalCount ; /**< Specifies the number of categories available in the specified category (which also includes all the categories available in the child category). */
	JC_UINT32			uiBookmarkTotalCount ;  
} ST_CATEGORY_INFO ;

/**
 * @brief This enumeration specifies on which basis the sorting operation can be applied.
 */

typedef enum bmSortItemList
{
	E_BM_SORT_TITLE, /**< Specifies the sorting by title attribute in the favourites */
	E_BM_SORT_URL, /**< Specifies the sorting by URL attribute in the favourites */
	E_BM_SORT_HTTP_STATUS, /**< Specifies the sorting by HTTP Status attribute in the favourites */
	E_BM_SORT_LAST_VISITED_TIME, /**< Specifies the sorting by last visited time attribute in the favourites */
	E_BM_SORT_CREATED_TIME  /**< Specifies the sorting by created time attribute in the favourites */
} E_BM_SORT_ITEM_LIST ;

/**
 * @brief This enumeration specifies the sorting order.
 *		   
 */

typedef enum bmSortOrder
{
	E_BM_ASCENDING_ORDER, /**< Specifies sorting in the ascending order */
	E_BM_DESCENDING_ORDER /**< Specifies sorting in the descending order */
} E_BM_SORT_ORDER ;

/**
 
  
 * @param[in]   uiMaxCategory       Specifies the maximum number of categories allowed. Number of categories should be greater than zero.
 
  
 * 
 
  
 * 
 * @retval JC_ERR_FILE_OPEN			If there is any error in opening the specified file (if it exists).
 
  
 *			
 
  
 * 
 */

JC_RETCODE jdi_BookmarkInitialize (const JC_CHAR   *pmBookmarkFileName,
								   JC_UINT32		uiMaxCategory,
								   JC_UINT32		uiMaxBookmark,
								   JC_BOOLEAN		bDuplicateAllowed,
								   JC_HANDLE		*pvBookmarkHandle) ;

/**
 
  
 * 
 
 * @retval JC_ERR_MEMORY_ALLOCATION If there is any memory allocation error during the process of deinitialization.
 
 *
 
  
 * 
 * updates into the persistent storage. 
 */

JC_RETCODE jdi_BookmarkDeInitialize (JC_HANDLE 	vBookmarkHandle) ;

/**
 * @brief To provide the list of favourites available into the specified category
 
 * @param[in]	pstCategoryList		Specifies the category list which contents needs to be retrive
 * @param[out]  ppstCategoryList	Returns the category list and its favourites
 * @retval  JC_OK	If the category matches in the category list
 * @retval  Others  If category not found in the category list
 *			
 
 * @note		The parameter ppstCategoryList should not be NULL.
 *
 
 * It provides the list of favourites and sub category available into the given category.
 */

JC_RETCODE jdi_BookmarkOpen (JC_HANDLE 	vBookmarkHandle,
							 ST_CATEGORY_LIST *pstCategoryList,
							 ST_CATEGORY_LIST **ppstCategoryList) ;

/**
 * @brief To insert category into the specified category. 
 
 * @param[in]	pstParentCategory	Specifies the parent category list where this 
 * 									category needs to be inserted.If needs to be 
 * 									inserted at the root this value should be NULL.
 * @param[in]   pmCategory			Specifies the category name, which needs to be 
 *									inserted to the pstParentCategory.
 * @param[in]   bIsReadOnly			Specifies whether the category is read only.
 * @return  
 * @retval JC_OK		If it insert the specified category in the list
 * @retval Others       If it fails to insert the category. 
 *			
 
 * @note		The parameter pmCategory should not be NULL.
 *
 * This function should be used for the insertion of the category to the specified
 * parent category location. If parent category is not specified than it will store 
 * the passed category to the root category. It returns error if the number of 
 * category is exceeds the specified value.
 */

JC_RETCODE jdi_BookmarkInsertCategory (JC_HANDLE 				vBookmarkHandle,
									   ST_CATEGORY_LIST			*pstParentCategory,
									   JC_CHAR					*pmCategory,
									   JC_BOOLEAN				bIsReadOnly) ;

/**
 
  
 * 
 
 * @return  
 
  
 *			
 
  
 *
 
  
 * 
 * the specified value and will not allow to insert item in that location.
 */

JC_RETCODE jdi_BookmarkInsertItem (JC_HANDLE				vBookmarkHandle,
								   ST_BOOKMARK_ATTRIBUTES 	*pstBookmark,
								   ST_CATEGORY_LIST			*pstParentCategory) ;

/**
 * @brief	To update the category name for the specified category. 
 
 * @param[in]	pstOriginalCategory Specifies the category where category name needs to be updated
 * @param[in]   pmCategoryName		Specifies the name of the category.
 * @return  
 * @retval  JC_OK		If the category is inserted successfully.
 * @retval  Others		If the parameter passing are not valid or unable to update category.
 *			
 
 * @note		The parameter pstOriginalCategory should not be NULL.
 * @note		The parameter pmCategoryName should not be NULL.
 *
 * This function should be used to change category name of the specified category.
 * It will check for the duplication, if it is there than it will indicate it by the
 * specific error and will not allow to update the category name.
 **/

JC_RETCODE jdi_BookmarkUpdateCategory (const JC_HANDLE		vBookmarkHandle,
									   ST_CATEGORY_LIST		*pstOriginalCategory,
									   JC_CHAR				*pmCategoryName) ;

/**
 * @brief	To move the category to the specified category. 
 
 * @param[in]	pstOriginalCategory Specifies the category, which needs to be moved.
 * @param[in]   pstParentCategory   Specifies the category, where passed Category needs to be inserted..
 * @return  
 * @retval  JC_OK		If the category is inserted successfully.
 * @retval  Others		If the parameter passing are not valid or unable to update category.
 *			
 
 * @note		The parameter pstOriginalCategory should not be NULL.
 *
 * This function shall move the category to the specified category. While moving it will
 * check the maximum category limit, if its exceeding to the specified one than it will
 * not allow to move category.
 **/

JC_RETCODE jdi_BookmarkMoveCategory (JC_HANDLE			vBookmarkHandle,
									 ST_CATEGORY_LIST	*pstOriginalCategory,
									 ST_CATEGORY_LIST	*pstParentCategory) ;

/**
 
  
 * 
 
  
 * @return  
 
  
 *			
 
  
 * 
 *
 
  
 * it will not allow to enter the updated favorites and return error for that.
 **/

JC_RETCODE jdi_BookmarkUpdateFavourites (const JC_HANDLE 		vBookmarkHandle,
										 ST_BOOKMARK_ATTRIBUTES	*pstOriginalBookmark,
										 ST_BOOKMARK_ATTRIBUTES	*pstUpdatedBookmark,
										 ST_CATEGORY_LIST		*pstParentCategory) ;

/**
 
  
 * 
 
  
 * @return  
 
  
 *			
 
  
 *
 
  
 * 
 * 
 **/

JC_RETCODE jdi_BookmarkMoveFavourites (JC_HANDLE				vBookmarkHandle,
									   ST_BOOKMARK_ATTRIBUTES	*pstBookmark,
									   ST_CATEGORY_LIST			*pstOriginalCategory,
									   ST_CATEGORY_LIST			*pstNewCategory) ;

/**
 * @brief	To delete the favorite from the passed category list
 
  
 * 
 * @return  
 
  
 *			
 
  
 *
 * This function shall delete the favorite from the passed category list in which 
 * it is available. 
 **/

JC_RETCODE jdi_BookmarkDeleteFavourites (JC_HANDLE 				vBookmarkHandle,
										 ST_BOOKMARK_ATTRIBUTES	*pstBookmark,
										 ST_CATEGORY_LIST		*pstCategory) ;

/**
 * @brief To delete the passed category with its content.
 
 * @param[in] pstCategory       Specifies the category, which needs to be removed from the list.
 * @return  
 * @retval  JC_OK		If the category is deleted successfully.
 * @retval  Others		If the parameter passing are not valid or unable to delete the category.
 *			
 
 * @note		The parameter pstCategory should not be NULL.
 *
 * This function should be used to delete the specefied category and all its content. 
 **/

JC_RETCODE jdi_BookmarkDeleteCategory (JC_HANDLE 		vBookmarkHandle,
									   ST_CATEGORY_LIST	*pstCategory) ;

/**
 * @brief To delete all the category.
 
 * @param[in]   pstCategory			Specifies the category list 
 * @param[in] 	bReadOnlyAlso		Flag to remove the read only category
 * @return  
 * @retval  JC_OK		If its able to delete successfully.
 *			
 
 *
 * This function shall delete all the category. If flag bReadOnlyAlso
 * is specify false than all those category and its content whoever is having 
 * read-only attribute true will not be deleted and if that flag is true than
 * it will delete all.
 **/

JC_RETCODE jdi_BookmarkDeleteAll (JC_HANDLE 				vBookmarkHandle,
								  ST_CATEGORY_LIST			*pstCategory,
								  JC_BOOLEAN				bReadOnlyAlso) ;
/**
 
  
 * @return  
 * @retval  JC_OK		If the category sorted successfully.
 * @retval  Others		If the parameter passing are not valid or unable to save the details
 *			
 
 *
 
 * any modification made by the user than only it will store whole list again.
 
  
 **/

JC_RETCODE jdi_BookmarkSaveFavourites (JC_HANDLE 		vBookmarkHandle) ;

/**
 * @brief To search the favourites. 
 
 * @param[in]   pmURL				Specifies the required favourite to search
 * @param[out]  ppstCategoryList	Reference of the categorylist where searched url is avalible
 
 * @return  
 * @retval  JC_OK		If its able to search the required favorites.
 *			
 
 * @note		The parameter pmURL should not be NULL.
 * @note		The parameter ppstCategoryList should not be NULL.
 
 *
 * This function shall search the favourites and provide the category node where it is 
 
 **/

JC_RETCODE jdi_BookmarkSearchFavourites (JC_HANDLE 		         vBookmarkHandle,
										 JC_CHAR 			     *pmURL,
										 ST_CATEGORY_LIST 	     **ppstCategoryList,
										 ST_BOOKMARK_ATTRIBUTES	 **ppstBookmark) ;

/**
 * @brief To sort the content of the category on the basis of the passed sorting order parameter. 
 
 * @param[in]   pmCategoryName	Specifies the category name in which sorting operation is applied.
 * @param[in] 	eSortByItem		Specifies the item name on which sorting operation is applied.
 * @param[in] 	eSortOrder		Specifies the sorting order on passed item.	
 * @param[out]  ppstCategoryList	Contains the list of sorted data.
 * @return  
 * @retval  JC_OK		If the category sorted successfully.
 * @retval  Others		If the parameter passing are not valid or unable to sort the category.
 *			
 
 * @note		The parameter pmCategoryName should not be NULL.
 * @note		The parameter ppstCategoryList should not be NULL.
 *
 
 * will be done based on the provided ascending/descending order, and will apply on the 
 * specified item like title,URL,created-time etc.
 **/

JC_RETCODE jdi_BookmarkSortFavourites (JC_HANDLE 			vBookmarkHandle,
									   JC_CHAR 				*pmCategoryName,
									   E_BM_SORT_ITEM_LIST	eSortByItem,
									   E_BM_SORT_ORDER 		eSortOrder,
									   ST_CATEGORY_LIST 	**ppstCategoryList) ;

/**
 
  
 * @param[in]   pstCategoryList		Specifies the category list for which the information 
									needs to be retrieved. If it is NULL then the counts 
 
  
 *									specified category.
 * 
 
 * @note		The parameter pstCategoryInfo should not be NULL.
 *
 * This function shall retrieve the information like number of favourites and category available
 * into the specified category.
 * 
 * If NULL is passed it will retrieve all the information and provides number of categories and favourites as a whole and also provide 
 * number of categories and favourites available in the root directory.
 * 
 * If the category is passed, it provides the number of categories & favourites available in 
 
  
 **/

void jdi_BookmarkGetInfo (JC_HANDLE				vBookmarkHandle,
						  ST_CATEGORY_LIST		*pstCategoryList,
						  ST_CATEGORY_INFO		*pstCategoryInfo) ;

/**
 * @brief To sort the category from the root. 
 
 * @param[in]   eSortOrder			Specifies the sorting order
 * @return  
 * @retval  JC_OK		If its able to sort the category in specified order.
 *			
 
 *
 * This function shall sort the category from the root. Sorting
 * will be done based on the provided ascending/descending order,
 **/

JC_RETCODE jdi_BookmarkSortCategory  (JC_HANDLE 			vBookmarkHandle,
									  E_BM_SORT_ORDER 		eSortOrder) ;

/**
 
  
 * 
 * @param[out]  pmDirPath			Reference of the directory path where the file is to be created
 
 * @return  
 
 * @retval  Others		If fails while exporting
 *			
 
 *
 
  
 * 
 **/

JC_RETCODE jdi_BookmarkExport (JC_HANDLE				vBookmarkHandle,
							   ST_BOOKMARK_ATTRIBUTES	*pstBookmark,
							   JC_CHAR					*pmDirPath,
							   JC_CHAR					*pmFileName) ;

/**
 
  
 * @param[in]   pstCategoryList		Specifies the category list from where we have to export all 
 *                                  favourite to a file,whose name depend upon the category name,
 *								    if it is NULL then a default directory will be created and all 
 
 * @param[out]  pmDirPath			Reference of the directory path where the file is to be created
 * @return  
 
 * @retval  Others		If fails while exporting
 *			
 
 * @note		The parameter pmDirPath should not be NULL. 
 *
 
 * at specified directory path
 **/

JC_RETCODE jdi_BookmarkExportAll (JC_HANDLE				vBookmarkHandle,
								  ST_CATEGORY_LIST		*pstCategoryList,
								  JC_CHAR				*pmDirPath) ;

/**
 
  
 * 
 *									needs to be import
 
 * @return  
 
 * @retval  Others		If fails while importing
 *			
 
 * @note		The parameter pmFileName should not be NULL. 
 
 *
 
 **/

JC_RETCODE jdi_BookmarkImport (JC_HANDLE				vBookmarkHandle,
							   JC_CHAR					*pmFileName,
							   ST_BOOKMARK_ATTRIBUTES	**ppstBookmark) ;

/**
 
  
 * 
 *			
 
 *
 
 **/

void jdi_BookmarkFreeFavourite (JC_HANDLE				vBookmarkHandle,
							    ST_BOOKMARK_ATTRIBUTES	*pstBookmark) ;

/**
 
 * @param[in]	bMultiFileStorage		Specifies the storage type 
 
 * @return  
 * @retval JC_OK	If the storage type is set correctly
 * @retval error    Error while setting the type and retrieving information
 *
 
 * details should be maintained in a single vbm file or in a file system. Depending on
 
 */
JC_RETCODE	jdi_BookmarkSetStorageType (JC_BOOLEAN		bMultiFileStorage,
										JC_HANDLE		vBookmarkHandle) ;

/**
 
 *		  All indexes start from zero.
 
  
 * @param[in]	pstCategory			Specifies the category containing the favorite.
 
 *
 * @return JC_OK for success
 *
 **/
JC_RETCODE jdi_BookmarkChangeIndex (JC_HANDLE				vBookmarkHandle,
									ST_BOOKMARK_ATTRIBUTES	*pstBookmark,
									ST_CATEGORY_LIST		*pstCategory,
									JC_UINT8				ucNewBookMarkIndex) ;

#ifdef __cplusplus
}
#endif

#endif


