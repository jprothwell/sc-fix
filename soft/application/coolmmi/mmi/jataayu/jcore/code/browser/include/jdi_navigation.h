/***************************************************************************
 * $Id: jdi_navigation.h,v 1.24 2008/11/12 15:00:09 kavithadevhtmlbrow Exp $
 * $Revision: 1.24 $
 * $Date: 2008/11/12 15:00:09 $
 * 
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
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
 * $Log: jdi_navigation.h,v $
 * Revision 1.24  2008/11/12 15:00:09  kavithadevhtmlbrow
 * GetNavigationHandle and NavigateByIndex
 *
 * Revision 1.23  2007/12/05 13:53:23  kumardevhtmlbrow
 * Update for cache chunk and interface file updated.
 *
 * Revision 1.22  2007/04/17 09:21:24  kavithadevhtmlbrow
 * Navigation Index with frame handle
 *
 * Revision 1.21  2007/03/02 11:47:21  shreyasdevhtmlbrow
 * RVCT Compilation fix
 *
 * Revision 1.20  2007/02/26 05:52:13  pathakdevhtmlbrow
 * Added function for Clearing all navigation entries
 *
 * Revision 1.19  2007/02/14 12:22:50  kavithadevhtmlbrow
 * Navigate at Index : Navigate To updated
 *
 * Revision 1.18  2007/01/09 14:27:30  kumardevhtmlbrow
 * Added the utility function
 *
 * Revision 1.17  2006/11/29 05:18:53  kavithadevhtmlbrow
 * Set Navigation Handle
 *
 * Revision 1.16  2006/09/28 09:21:40  kumardevhtmlbrow
 * Update
 *
 * Revision 1.15  2006/07/05 03:46:48  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.14  2006/04/11 14:04:08  kavithadevhtmlbrow
 * Navigation replace curr page ctx
 *
 * Revision 1.13  2005/12/14 12:52:57  kavithadevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.12  2005/12/13 09:11:05  kavithadevhtmlbrow
 * NavigateTo Added
 *
 * Revision 1.11  2005/12/12 15:29:01  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.10  2005/09/21 11:24:40  kavithadevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.9  2005/09/21 07:31:01  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2005/09/20 07:01:25  kumardevhtmlbrow
 * Added the page context to navigation
 *
 * Revision 1.7  2005/09/12 12:20:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/08/12 10:57:42  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/06/23 09:49:45  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/06/17 08:17:36  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.2  2005/05/19 06:35:43  kumardevhtmlbrow
 * Updated the parameters ...
 *
 * Revision 1.1  2005/05/10 13:38:52  kumardevhtmlbrow
 * Added the navigation module
 *
 *  
 ***************************************************************************/

#ifndef _JDI_NAVIGATION_H
#define _JDI_NAVIGATION_H

/**
 * @file jdi_navigation.h
 * @brief Defines the error code and exposed enumeration for the navigation.
 * 
 */

/** @cond */
#include <jdi_browsermodule.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Specifies the minimum navigation count.
 */
#define  DEFAULT_MAX_NAVIGATION_COUNT				25

/**
 * @brief Specifies the navigation page information.
 */
typedef struct _navigation_page_info
{
	JC_CHAR		*pmURL ; /**< Specifies the URI of the page. */
	JC_INT8		*pcPostData ; /**< Specifies the post data of the page (if any). */
	JC_UINT32	uiPostLen ; /**< Specifies the length of the post data. */
} ST_NAVIGATION_PAGE_INFO ;

/** 
 * @brief Specifies the navigation type. 
 */
typedef enum _navigation_type
{
	E_NAVIGATION_NONE, /**< No navigation update. */
	E_NAVIGATION_BACK, /**< Specifies the one step back navigation. */
	E_NAVIGATION_FORWARD, /**< Specifies the one step forward navigation. */
	E_NAVIGATION_INDEX /**< Specifies the number of step to move towards forward or backward. */
} E_NAVIGATION_TYPE ;

/**
 * @brief Specifies the navigation page context.
 */
typedef struct _navigation_page_context
{
	JC_UINT32	uiXOffset ; /**< Specifies the X offset. */
	JC_UINT32	uiYOffset ; /**< Specifies the Y offset. */
	JC_UINT32	uiControlIndex ; /**< Specifies the control index. */
} ST_NAVIGATION_PAGE_CONTEXT ;

/**
 * @brief Specifies the navigation history.
 */

typedef struct _navigation_history
{
	ST_NAVIGATION_PAGE_INFO			stPageInfo ;			/**<Specifies the navigation page information. */ 
	ST_NAVIGATION_PAGE_CONTEXT		stPageContext ;			/**<Specifies the navigation page context. */ 
	JC_HANDLE						vPageFrameHandle ;		/**<Specifies the handle to the page Frame. */ 
	JC_UINT64						ulLastAccessedTime ;	/**<Specifies the time when the page last accessed. */ 
	struct _navigation_history		*pstNextPage ;			/**<Specifies the next page. */ 
	struct _navigation_history		*pstPrevPage ;			/**<Specifies the previous page. */ 
} ST_NAVIGATION_HISTORY ;

/**
 * @brief Specifies the navigation list. 
 **/
typedef struct _navigation_list 
{
	ST_NAVIGATION_HISTORY			*pstFirstPage ;		/**< Specifies the First page */
	ST_NAVIGATION_HISTORY			*pstCurrentPage ;	/**< Specifies the Current page */
} ST_NAVIGATION_LIST ;
 
/**
 * @brief Specifies the callback function for the frame update.
 */
typedef void (* CB_DE_REMOVE_PAGE_FRAME_LIST) (JC_HANDLE vDEContextHandle, JC_HANDLE vPageFrameHandle) ;

/**
 * @brief			Initializes the navigation history .
 * @param[in]		uiMaxNavigationCount Maximum navigation history
 * @param[in]       cbPageFrameHandle Callback Function to remove frame
 *					list from the DE Context.
 * @param[in]		vPageFrameCallBackArg DE Context handle as argument to the Call Back.
 * @param[in]		vMemHandle Handle to Memory Manager
 * @param[in, out]	pvHandle Handle to the navigation history.
 * return			appropriate error code or success
 * Initializes the navigation history by allocating required memory
 *
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_RETCODE jdi_NavigationInitialize	(JC_UINT32						uiMaxNavigationCount, 
									 CB_DE_REMOVE_PAGE_FRAME_LIST	cbPageFrameHandle,
									 JC_HANDLE						vPageFrameCallBackArg,
									 JC_HANDLE						vMemHandle,
									 JC_HANDLE						*pvHandle)  ;

/**
 * @brief			Deinitializes the navigation history .
 * @param[in]		vHandle Handle to the navigation history.
 * return			appropriate error code or success
 * Deinitializes the navigation history by deallocating all the used memory
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
void jdi_NavigationDeinitialize			(JC_HANDLE vHandle) ;

/**
 * @brief			Adds a page to the navigation history
 * @param[in]		vHandle Handle to the navigation history.
 * @param[in]		pstPageInfo pointer to the page information structure
 * return			appropriate error code or success
 * This function adds a page to the navigation history,next to current
 * page .If the navigation history is already full then the least recently
 * used page history is deleted to make space for the new page.
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_RETCODE jdi_NavigationAdd	(JC_HANDLE						vHandle, 
								 const ST_NAVIGATION_PAGE_INFO	*pstPageInfo) ;

/**
 * @brief			Gets back the previous page from navigation history .
 * @param[in]		vHandle Handle to the navigation history.
 * @param[in, out]	pstPageInfo Used as a value result argument to get back
 *					the page info of the previous page .
 * @param[out]		pvFrameHandle Handle to hold the frame list
 * return			appropriate error code or success
 * This function is used to retrieve the previous page from the navigation 
 * history.After retrieving the information current page is set to the previous 
 * page.
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_RETCODE jdi_NavigationPrevious (JC_HANDLE vHandle, 
									 ST_NAVIGATION_PAGE_INFO *pstPageInfo, 
									 JC_HANDLE	*pvFrameHandle) ;

/**
 * @brief			Gets back the Next page from navigation history .
 * @param[in]		vHandle Handle to the navigation history.
 * @param[in,out]	pstPageInfo Used as a value result argument to get back
 *					the page info of the Next page .
 * @param[out]		pvFrameHandle Handle to hold the frame list
 * return			appropriate error code or success
 * This function is used to retrieve the Next page from the navigation 
 * history.After retrieving the information current page is set to the Next 
 * page.
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_RETCODE jdi_NavigationNext	(JC_HANDLE vHandle, 
								 ST_NAVIGATION_PAGE_INFO *pstPageInfo, 
								 JC_HANDLE	*pvFrameHandle) ;

/**
 * @brief			Checks whether previous page exist in navigation history .
 * @param[in]		vHandle Handle to the navigation history.
 * return			either TRUE or FALSE
 * This function checks whether previous page exists in the navigation
 * history. 
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_BOOLEAN jdi_NavigationIsPrevious		(JC_HANDLE vHandle) ;

/**
 * @brief			Checks whether next page exist in navigation history .
 * @param[in]		vHandle Handle to the navigation history.
 * return			either TRUE or FALSE
 * This function checks whether next page exists in the navigation
 * history
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_BOOLEAN jdi_NavigationIsNext			(JC_HANDLE vHandle) ;

/**
 * @brief			Adds page context to the current page.
 * @param[in]		vHandle Handle to the navigation history.
 * @param[in]		pstPageContext pointer to the page context
 * @param[in]		ePreviousType Specifies the previous navigation type.
 * return			JC_OK or JC_ERR_NAVIGATION_NO_PAGE_FOUND
 * This funtion adds page context to the current page and if the current
 * page is NULL it returns error page not found.
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_RETCODE jdi_NavigationSetCurrPageContext (JC_HANDLE vHandle,
											 const ST_NAVIGATION_PAGE_CONTEXT	*pstPageContext,
											 E_NAVIGATION_TYPE					ePreviousType) ;

/**
 * @brief			Gets page context of the current page
 * @param[in]		vHandle Handle to the navigation history.
 * @param[out]		pstPageContext pointer to the page context
 * return			JC_OK or JC_ERR_NAVIGATION_NO_PAGE_FOUND
 * This funtion gets the page context of the current page.If the current 
 * page is NULL it returns error page not found.
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_RETCODE jdi_NavigationGetCurrPageContext (JC_HANDLE vHandle,
											 ST_NAVIGATION_PAGE_CONTEXT *pstPageContext) ;

/**
 * @brief			Navigates to the specific page .
 * @param[in]		vHandle Handle to the navigation history.
 * @param[in]		eNavigationType The type of navigation
 * @param[in]		hIndex Index into the navigation list.
 * return			appropriate error code or success
 * this funtion is used to navigate to the specific page
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_RETCODE jdi_NavigationNavigateTo	(JC_HANDLE					vHandle, 
									 E_NAVIGATION_TYPE			eNavigationType,
									 JC_INT16					hIndex) ;

/**
 * @brief			Replaces the current page in the navigation history
 *					with the new page contents
 * @param[in]		vHandle Handle to the navigation history.
 * @param[in]		pstPageInfo pointer to the page information structure
 * return			appropriate error code or success
 * This function replaces the curent page in the navigation history with 
 * the contents of the new page.In order to set the page contetns for the 
 * newly replaced page we need to call jdi_NavigationSetCurrPageContext
 * after calling this function.
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_RETCODE jdi_NavigationReplaceCurrPage	(JC_HANDLE						vHandle, 
											 const ST_NAVIGATION_PAGE_INFO	*pstPageInfo) ;

/**
 * @brief			Navigates to the page specified by the index.
 * @param[in]		vHandle Handle to the navigation history.
 * @param[in]		hIndex Index of the page to be navigated to.
 * @param[out]      pstPageInfo    Reference to hold the page info
 * @param[out]      pvFrameHandle Reference to hold the page frame handle
 * return			appropriate error code or success
 * this funtion is used to navigate to page specified by Index
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_RETCODE jdi_NavigationNavigateToIndex	(JC_HANDLE					vHandle,
											 JC_INT16					hIndex, 
											 ST_NAVIGATION_PAGE_INFO	*pstPageInfo,
											 JC_HANDLE					*pvFrameHandle) ;

/**
 * @brief			Gets the count of the current navigation history
 * @param[in]		vHandle Handle to the navigation history.
 * return			Count of the navigation history
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_UINT32	jdi_NavigationGetCount (JC_HANDLE	vHandle) ;

/**
 * @brief			Adds a page with frame handle to the navigation history
 * @param[in]		vHandle Handle to the navigation history.
 * @param[in]		vPageFrameHandle Handle to the page Frame.
 * return			appropriate error code or success
 * This function adds the Frame Handle to the current page in the navigation
 * history.
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
JC_RETCODE jdi_NavigationAddFrameHandle (JC_HANDLE						vHandle, 
										 JC_HANDLE						vPageFrameHandle) ;

/**
 * @brief			Clears the navigation list.
 * @param[in]		vHandle Handle to the navigation history.
 * return			void
 * Clears the navigation list and frees all memory associated with it.
 * @note This API's are used by the Display Engine and not required for the browser application.
 **/
void jdi_NavigationClearAll (JC_HANDLE  vHandle) ;

/**
 * @brief Gets the navigation list. 
 * @param[in]	vHandle				Handle to the navigation history.
 * @param[out]	pstNavigationList	Handle to navigation list. 
 * retVal returns JC_OK on success else appropriate error code.
**/
JC_RETCODE jdi_NavigationGetList (JC_HANDLE				vHandle,
								  ST_NAVIGATION_LIST	*pstNavigationList) ;

/** @endcond */

/**
 * @brief Error specifies the navigation list is empty (ie) there is no current page available 
 */
#define JC_ERR_NAVIGATION_NO_PAGE_FOUND				TERMINAL_ERR_CODE(NAVIGATION_MODULE, 0x01)
/**
 * @brief Error specifies the previous page is empty in the navigation list.
 */
#define JC_ERR_NAVIGATION_PREV_PAGE_NOT_FOUND		TERMINAL_ERR_CODE(NAVIGATION_MODULE, 0x02)
/**
 * @brief Error specifies there is no next page in the navigation list.
 */
#define JC_ERR_NAVIGATION_NEXT_PAGE_NOT_FOUND		TERMINAL_ERR_CODE(NAVIGATION_MODULE, 0x03)

#ifdef __cplusplus
}
#endif

#endif

/* End of File */


