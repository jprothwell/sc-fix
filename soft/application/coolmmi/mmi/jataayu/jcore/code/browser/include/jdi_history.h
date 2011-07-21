/***************************************************************************
 * $Id: jdi_history.h,v 1.27 2007/12/05 13:53:23 kumardevhtmlbrow Exp $
 * $Revision: 1.27 $
 * $Date: 2007/12/05 13:53:23 $
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
 * $Log: jdi_history.h,v $
 * Revision 1.27  2007/12/05 13:53:23  kumardevhtmlbrow
 * Update for cache chunk and interface file updated.
 *
 * Revision 1.26  2007/05/31 12:05:22  shreyasdevhtmlbrow
 * Doxygen comment updated
 *
 * Revision 1.25  2007/02/02 11:46:54  kumardevhtmlbrow
 * Compilation warning fix.
 *
 * Revision 1.24  2006/12/20 09:01:30  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.23  2006/11/17 12:37:39  kumardevhtmlbrow
 * Update the history module for handling the corrupted history file.
 *
 * Revision 1.22  2006/09/28 09:21:40  kumardevhtmlbrow
 * Update
 *
 * Revision 1.21  2006/06/01 10:03:23  kumardevhtmlbrow
 * Updated ....
 *
 * Revision 1.20  2006/05/11 07:20:10  kumardevhtmlbrow
 * Updated the interface documentation ...
 *
 * Revision 1.19  2006/04/11 14:03:50  kavithadevhtmlbrow
 * History Add Title
 *
 * Revision 1.18  2006/03/08 12:29:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.17  2005/12/12 15:29:01  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.16  2005/09/12 12:20:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.15  2005/08/12 10:57:42  kumardevhtmlbrow
 * no message
 *
 * Revision 1.14  2005/07/19 08:58:53  kumardevhtmlbrow
 * no message
 *
 * Revision 1.13  2005/07/18 14:29:25  kumardevhtmlbrow
 * no message
 *
 * Revision 1.12  2005/07/18 07:03:26  kumardevhtmlbrow
 * no message
 *
 * Revision 1.11  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.10  2005/06/24 13:36:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.9  2005/06/23 09:49:45  kumardevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/06/16 13:48:17  kumardevhtmlbrow
 * no message
 *
 * Revision 1.7  2005/05/27 13:26:15  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2005/05/21 11:20:48  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.5  2005/05/19 15:00:26  kumardevhtmlbrow
 * Updated the history & cookie
 *
 * Revision 1.4  2005/05/19 06:35:43  kumardevhtmlbrow
 * Updated the parameters ...
 *
 * Revision 1.3  2005/05/17 09:43:42  kumardevhtmlbrow
 * History Updated ...
 *
 * Revision 1.2  2005/05/07 07:43:07  kumardevhtmlbrow
 * Modified domain for using cmem
 *
 * Revision 1.1  2005/05/05 12:03:15  kumardevhtmlbrow
 * Initialize History
 *
 *  
 ***************************************************************************/

/**
 * @page history History
 * History holds and maintains information about the pages that have been fetched by the
 * user. Important information regarding the page such as URL, post data and time of
 * fetching is maintained in the history for future references. Whenever the user wants to 
 * view the information for the pages browsed in the recent past, history provides them with
 * the relevant information. Moreover it also provides different operations like sorting, searching
 * etc on the information available for better user convenience. 
 *
 * Whenever the browser application is terminated the history information is written
 * to a persistent history file, which is refered to in the subsequent intialization of the browser.
 *
 * Following are the API's that are exposed by the history:
 * 
 * @li #jdi_HistoryClearAll
 * @li #jdi_HistoryClearSpecific
 * @li #jdi_HistoryDeleteDomain
 * @li #jdi_HistoryGet
 * @li #jdi_HistorySortDomain
 * @li #jdi_HistoryStorePersistent
 * @li #jdi_HistorySortHost
 * 
 * To initialize and deinitialize the history the application should not use history API's instead 
 * it should use #jdi_BEInitComponent to initiaze the history and #jdi_BEDeinitComponent to 
 * deinitialize the history.
 *
 * All the exposed API's of the history needs a history handle. The application needs to use the API
 * #jdi_BEGetHistoryContext to get the history handle. The handle will become invalid once the history
 * component is deinitialzed.
 */

/**
 * @file jdi_history.h
 * @ingroup History
 * @brief Defines the exposed enumerations, structures and APIs of the history module that
 *		  can be used by the application.
 */

#ifndef _JDI_HISTORY_H
#define _JDI_HISTORY_H

#include <jdi_browsermodule.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Specifies the default domain name that is created internally by the 
 * history module, when the number of domain exceeds.
 */
#define DEFAULT_DOMAIN						"Others"

/**
 * @brief The application needs to use this enum to specify the order in which the sorting
 *		  of history has to be done.
 */
typedef enum history_sort_order
{
	E_HISTORY_ASCENDING_ORDER, /**< Specifies that history has to be sorted in ascending order */
	E_HISTORY_DESCENDING_ORDER /**< Specifies that history has to be sorted in descending order */
} E_HISTORY_SORT_ORDER ;

/**
 * @brief The application needs to use this enum to specify the condition based on which the
 *		  history has to be sorted.
 */
typedef enum history_sort_item
{
	E_HISTORY_SORT_URL, /**< Specifies that history has to be sorted based on the URL */
	E_HISTORY_SORT_ACCESSED_TIME /**< SPecifies that history has to be sorted based on the access time */
} E_HISTORY_SORT_ITEM ;

/**
 * @brief The following structure is used to store a history item. The application needs to use this
 *		  structure when it wants to add a history item to the linked list of history items maintained
 *		  in the history . The history module also uses the structure while returning the 
 *		  host information to the application 
 *		  @see ST_HISTORY_HOST
 */
typedef struct _history_page_info
{
	JC_CHAR		*pmURL ; /**< Specifies the URL */
	JC_INT8		*pcPostData ; /**< Specifies the post data */
	JC_UINT32	uiPostLen ; /**< Specifies the post data length */
	JC_CHAR		*pmTitle ; /**< Specifies the title name of the history item */
} ST_HISTORY_PAGE_INFO ;

/**
 * @brief The history module uses this structure to specify the access times of a history item.
 *		  The structure will be used while sending the host information to the application 
 *		  @see ST_HISTORY_HOST
 */
typedef struct _history_access_time
{
	JC_UINT64					ulAccessTime ; /**< Specifies the time when the history item was accessed */
	struct _history_access_time *pstNextTime ; /**< Pointer to the next access time node */
} ST_HISTORY_ACCESS_TIME ;

/**
 * @brief The history module uses this structure to specify the complete information of a history
 *		  item. This structure will specify the page information, the accessed time list etc
 */
typedef struct _history_host
{
	ST_HISTORY_PAGE_INFO		stPageInfo ; /**< Specifies the page information of the history item */
	JC_UINT64					ulLastAccessedTime ; /**< Specifies the last time when the history item was accessed. */
	JC_UINT64					ulHashKey ; /**< Specifies the hash value for the history URL */
	ST_HISTORY_ACCESS_TIME		*pstTime ; /**< Specifies the linked list of access times of the history item */
	struct _history_host		*pstNextHost ; /**< Pointer to the next history item */ 
	struct _history_host		*pstOriginalNextHost ;/**< Pointer to the next node of the original domain list which is used internally by the history (not to be used by the application) */
} ST_HISTORY_HOST ;

/**
 * @brief The history module uses this structure to specify the domain inforamtion.
 */
typedef struct _history_domain
{
	JC_CHAR					*pmDomain ; /**< Specifies the domain name */
	ST_HISTORY_HOST			*pstHost ; /**< Specifies the various history items that comes under the domain name */
	JC_INT32				iCurrHistoryCount ; /**< Specifies the number of history items that comes under the domain name */ 
	JC_UINT64				ulLastAccessTime ; /**< Specifies when URL from the particular domain was last accessed in History */
	struct _history_domain	*pstNextDomain ; /**< Pointer to the next domain information */
	struct _history_domain	*pstOriginalNextDomain ; /**< Pointer to the next node of the origianl domain list which is used internally by the history (not to be used by the application) */
} ST_HISTORY_DOMAIN ;

/** @cond */

/**
 * @brief		Initializes the history
 *
 * @param[in]	iMaxDomain Specifies the maximum number of domain information that can be 
 *				stored by the history.
 * @param[in]	iMaxHost Specifies the maximum number of history items that can be stored in a domain.
 * @param[in]	pmFileName Specifies the file in which the previous history information is stored
 * @param[in]	vMemHandle Specifies the memory handle that the hisroty module needs to use.
 * @param[out]	pvHandle This API returns this handle. From then on the application should use
 *				this handle for using the various history APIs.
 * @retval		JC_ERR_MEMORY_ALLOCATION If a memory allocation error occurs
 * @retval		JC_ERR_FILE_SYS_INIT if the file system could not be initialized.
 * @retval		JC_ERR_FILE_OPEN if the file, in which history information is stored, could not be opened.
 * @retval		JC_OK if history is initialized sucessfully. 
 * @see			jdi_HistoryDeinitialize
 *
 * This API Initializes the history section by allocating memory for a handle , and then reads
 * from persistent memory any history information stored previously.
 */

JC_RETCODE jdi_HistoryInitialize (JC_INT32			iMaxDomain, 
								  JC_INT32			iMaxHost, 
								  const JC_CHAR		*pmFileName, 
								  JC_HANDLE			vMemHandle,
								  JC_HANDLE			*pvHandle) ;

/**
 * @brief		Deinitializes the history.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the history initialize API 
 * @param[in]	pmFileName Specifies the file in which the history information is to be stored.
 * @retval		JC_ERR_FILE_SYS_INIT if the file system could not be initialized.
 * @retval		JC_ERR_FILE_OPEN if the file, in which history information is to be stored, could not be opened.
 * @retval		JC_OK if the history is deinitialized successfully.
 *
 * This API deinitializes the history by storing the history information to the history file
 * and deallocates the memory used by the history.
 * 
 * The application needs to specify the name of the history file to which the current
 * history information is to be stored.
 */

JC_RETCODE jdi_HistoryDeinitialize	(JC_HANDLE			vHandle, 
									 const JC_CHAR		*pmFileName) ;

/**	
 * @brief		Adds a history information.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the history initialize API.
 * @param[in]	pstPageInfo Specifies the history information to be added.
 * @retval		JC_OK If the history information is sucessfully added to the history.
 * @retval		JC_ERR_MEMORY_ALLOCATION If memory error occurs.
 * @see			jdi_HistoryClearSpecific
 *
 * This API adds the specified page to the history list.
 *
 * The history nodes are classified based on the domain to which the history URL belongs.
 * The API checks if the domain, to which the history URL belongs, is in the history list
 * then the page is added under that domain. Else it is added under a common domain. 
 *
 * If the history list (for any domain) is full then the least recently used history 
 * information will be removed.
 */
JC_RETCODE jdi_HistoryAdd			(JC_HANDLE					vHandle,
									 const ST_HISTORY_PAGE_INFO *pstPageInfo) ;

/**
 * @brief		Checks if a history info is present in the history list.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the history initialize API.
 * @param[in]	pstPageInfo Specifies the history information which needs to be searched.
 * @param[in]	pstDomain The application can also specify the domain in which the history information is to be searched
 * @retval		E_TRUE if the history information is found else returns E_FALSE
 *
 * This API searches the history list for particular history mode (pstPageInfo).
 *
 * If the domain parameter (pstDomain) is specified then only that particular domain
 * will be searched and if it is NULL then the whole history list will be searched.
 */
JC_BOOLEAN jdi_HistoryIsAvailable (JC_HANDLE					vHandle,
								   const ST_HISTORY_PAGE_INFO	*pstPageInfo,
								   const ST_HISTORY_DOMAIN		*pstDomain) ;
/**
 * @brief		Clears all the history entries
 *
 * @param[in]	vHandle Specifies the handle that was returned by the history initialize API.
 * @param[in]	pstDomain Specifies the domain whose history information is to be cleared
 * @retval		void
 * @see			jdi_HistoryClearSpecific
 *
 * The application needs to use this API to clear all the history information 
 * stored in the history list.
 *
 * If the application specifies the domain information then the API will clear all the entires
 * of the domain. Else all the entries of all the domains are cleared
 */

/** @endcond */

void jdi_HistoryClearAll		(JC_HANDLE					vHandle,
								 const ST_HISTORY_DOMAIN	*pstDomain) ;

/**
 * @brief		Clears all the history entries which has particular URL.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the history initialize API.
 * @param[in]	pmURL Specifies the URL
 * @retval		void
 * @see			jdi_HistoryClearAll
 *
 * This API removes the history entries, which has the URL as the URL specified in the 
 * input parameter.
 */

void jdi_HistoryClearSpecific (JC_HANDLE			vHandle,
							   const JC_CHAR		*pmURL) ;

/**
 * @brief		Deletes a domain
 *
 * @param[in]	vHandle Specifies the handle that was returned by the history initialize API.
 * @param[in]	pstDomain Specifies the domain to be deleted.
 * @retval		JC_OK if the domain is found in the history and it is sucessfully deleted.
 * @retval		JC_ERR_HISTORY_DOMAIN_NOT_FOUND if the domain is not found in the history
 * @retval		JC_ERR_HISTORY_DOMAIN_EMPTY if the domain list maintained in the history is empty.
 * @note		The history maintains a common domain which stores the history entries which 
 *			    does not have a domain. This common domain cannot be deleted.
 *
 * The application needs to use this API to delete a specific domain and the memory associated 
 * with it.
 */

JC_RETCODE jdi_HistoryDeleteDomain (JC_HANDLE				vHandle,
									const ST_HISTORY_DOMAIN *pstDomain) ;

/**
 * @brief		Gets all the entries of the history
 *
 * @param[in]	vHandle Specifies the handle that was returned by the history initialize API.
 * @retval		Pointer to the first domain node in the linked list of domains.
 *
 * The application needs to use this API to get all the entries of the history. The
 * API returns the pointer to the first node of the linked list of domains.
 *
 * @note The application should not free the history domain list that is returned
 * by this API.
 */

const ST_HISTORY_DOMAIN * jdi_HistoryGet (JC_HANDLE vHandle) ;

/**
 * @brief		Sorts the history list based on the domain name
 *
 * @param[in]	vHandle Specifies the handle that was returned by the history initialize API.
 * @param[in]	eSortOrder Specifies the order in which the sorting has to be done (ascending or descending) 
 * @param[in]	eSortItem	Specifies the item (URL or Accessed Time) in which the sorting has to be done.
 * @retval		Pointer to the first domain node in the sorted linked list of domains.
 *
 * This API sorts the entire history list on the basis of domain names and returns the 
 * pointer to the first domain of the sorted list.
 */

const ST_HISTORY_DOMAIN * jdi_HistorySortDomain (JC_HANDLE  vHandle, 
												 E_HISTORY_SORT_ORDER  eSortOrder,
												 E_HISTORY_SORT_ITEM	eSortItem) ;

/**
 * @brief		Stores the history information to the history file.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the history initialize API.
 * @param[in]	pmFileName Specifies the name of the file to which the history data is to be added.
 * @retval		JC_ERR_FILE_SYS_INIT if the file system could not be initialized.
 * @retval		JC_ERR_FILE_OPEN if the file, in which history information is to be stored, could not be opened.
 * @retval		JC_OK if the history information is successfully stored in the history file.
 *
 * During deinitialization the information stored in history is stored in the history file. 
 * If the application wants the entire information in history to be written to the history file
 * without deinitializing the history then this API has to be used.
 */

JC_RETCODE jdi_HistoryStorePersistent (JC_HANDLE		vHandle, 
									   const JC_CHAR	*pmFileName) ;

/**
 * @brief		Sorts all the entries of a particular domain
 *
 * @param[in]	vHandle Specifies the handle that was returned by the history initialize APi.
 * @param[in]	eSortOrder Specifies the order (ascending or descending) in which the sorting has to be done
 * @param[in]	eSortItem Specifies the condition based on which the sorting has to be done.
 * @param[in]	pstDomain Specifies the domain whose entries have to be sorted.
 * @retval		Pointer to the first history entry in the sorted linked list of history entries
 *
 * The application needs to use this API to sort all the entries of a particular domain.
 * 
 * The application needs to specify the sort order and the condition based on which the sorting has to
 * be done. The API will sort the entries and then return the pointer of the first entry
 * of the sorted linked list of history entries.
 */

const ST_HISTORY_HOST * jdi_HistorySortHost (JC_HANDLE					vHandle,
											 E_HISTORY_SORT_ORDER		eSortOrder,
											 E_HISTORY_SORT_ITEM		eSortItem,
											 const ST_HISTORY_DOMAIN	*pstDomain) ;

/** @cond */

/**
 * @brief		Adds title to a history node.
 *
 * @param[in]	vHandle Specifies the handle that was returned by the history initialize API.
 * @param[in]	pmURL Specifies the URL of the history node whose title needs to be updated.
 * @param[in]	pmTitle Specifies the title to be added.
 * @retval		JC_OK if the title is added successfully.
 *
 * The application needs to use this API to add a title to a history node.
 * The API will search for all the history nodes which has the same URL as that mentioned
 * in the input parameter (pmURL) and update the title of all those nodes.
 */

JC_RETCODE	jdi_HistoryAddTitle (JC_HANDLE		vHandle,
								 const JC_CHAR	*pmURL,
								 const JC_CHAR	*pmTitle) ;
JC_RETCODE jdi_HistoryResolveSearchURL (const JC_CHAR	*pmBaseURL, 
										const JC_CHAR	*pmCurrentURL, 
										JC_HANDLE		vMemHandle, 
										JC_HANDLE		vHistoryHandle) ;

/** @endcond */

/**
 * @brief This error is returned if a domain to be deleted is not found in the domain list of
 *		  the history.
 */
#define JC_ERR_HISTORY_DOMAIN_NOT_FOUND  TERMINAL_ERR_CODE(HISTORY_MODULE, 0x01)

/**
 * @brief This error is returned if the domain list is empty and the application has requested
 *		  for the deletion of a domain.
 */
#define JC_ERR_HISTORY_DOMAIN_EMPTY		 TERMINAL_ERR_CODE(HISTORY_MODULE, 0x02)

/**
 * @brief This error is returned when the history's persistent data is corrupted. But this error 
 * initializes the history module, only the history persistent data is deleted. If application
 * requires it can show this error code to the end-user.
 */
#define JC_ERR_HISTORY_FILE_CORRUPT		 TERMINAL_ERR_CODE(HISTORY_MODULE, 0x03)

/**
 * @brief This error is returned when the specific URL searched is not available in the
 * history.
 */
#define JC_ERR_HISTORY_NOT_FOUND		 TERMINAL_ERR_CODE(HISTORY_MODULE, 0x04)

#ifdef __cplusplus
}
#endif
									 
#endif

/* END OF FILE */


