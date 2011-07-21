/* zhuoxz,2009-4-28 */
#ifdef JATAAYU_SUPPORT
#include "jmms_inc.h"
#include "messagingdatatypes.h"
#include "messagingapi.h"
#include "jdi_communicator.h"


#include "jmms_cb.h"
#include "commonscreens.h"
#include "jmms_composeutils.h"
#include "jmms_addrdlg.h"
#include "jmms_composedlg.h"
#include "phonebookgprot.h"
#include "jdi_cutils_tcs.h"
#include "jdi_cutils_time.h"
#include "jdi_bookmark.h"
#include "jdi_displayinfo.h"
#include "jdi_browserengine.h"
#include "jdi_history.h"
#include "ddlutils.h"

static FOLDER_INFO gFolderInfo;

JC_UINT32 jdi_CUtilsTcsOccurenceCount(const JC_CHAR	*pmSrc,
									   JC_CHAR mChar, JC_UINT32 uiStart)
{
  return 0;
}

JC_RETCODE jdi_CUtilsTcsStrCat(const JC_INT8 *pcSrc, JC_CHAR *pmDest)
{
  jc_strcat(pmDest,pcSrc);
  return 0;
}

JC_CHAR* jdi_CUtilsTcsDuplicate(JC_HANDLE vMemHandle, const JC_CHAR *pmStr)
{
  return NULL;
}

JC_RETCODE jdi_MsgGetMessageHeaders (MSG_HANDLE hMsg, MESSAGE_HEADERS **ppMsgHeaders)
{
  return 0;
}

JC_RETCODE jdi_CUtilsCharToTchar(JC_HANDLE vMemHandle, const JC_INT8 *pcSrc, JC_CHAR **ppmDest)
{
  *ppmDest = malloc(_tcslen(pcSrc)+1);
  _tcscpy(*ppmDest,pcSrc);
  return JC_OK;
}

JC_RETCODE jdi_CommunicationFreeReference (JC_HANDLE vHandle, JC_UINT32 uiRequestID)
{
  return 0;
}

JC_RETCODE jdi_MsgUpdateInlineText (MSG_HANDLE hMsg, JC_UINT8 *pTextBuffer, JC_UINT32 uiBufferLen,
										JC_UINT8 ucPageIndex, JC_UINT32 uiMediaID)
{
  return 0;
}

JC_RETCODE jdi_MsgSetPageTimer (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex, JC_UINT32 uiDuration)
{
  return 0;
}

JC_RETCODE jdi_MsgAddPage(MSG_HANDLE hMsg, JC_UINT8 ucPageIndex)
{
  return 0;
}

JC_RETCODE jdi_MsgViewHandleEvent(MSG_HANDLE hMsg, EViewEventType eEventType, void *pEventData)
{
  return 0;
}

JC_INT32 jdi_CUtilsTtoI(JC_HANDLE vMemHandle, const JC_CHAR *pmValue)
{
  return 0;
}

JC_CHAR	* jdi_CUtilsItoT(JC_HANDLE vMemHandle, JC_INT32 iNum)
{
  return NULL;
}

JC_INT8* jdi_CUtilsStrDuplicate(JC_HANDLE vMemHandle, const JC_INT8 *pcStr)
{
  return NULL;
}

JC_INT16 jdi_CUtilsTcsStrCaseCmp(const JC_CHAR *pmFirstStr, const JC_INT8 *pcSecondStr)
{
  return 0;
}

JC_INT32 jdi_CUtilsTcsLastIndexOf(const JC_CHAR *pmSrc, JC_CHAR mSearchVal, JC_INT32 iEndIndex)
{
  return 0;
}

JC_RETCODE	jdi_MsgAddMedia(MSG_HANDLE hMsg, MEDIA_INFO *pMediaInfo,
									JC_UINT8 ucPageIndex, JC_UINT32 *pMediaID)
{
  return 0;
}

JC_RETCODE jdi_MsgSetPresentationType(MSG_HANDLE hMsg, EPresentationType ePresentationType)
{
  return 0;
}

JC_RETCODE jdi_MsgSetMaxPages(MSG_HANDLE hMsg, JC_UINT8 ucMaxPages)
{
  return 0;
}

JC_RETCODE jdi_MsgSetMaxSize(MSG_HANDLE hMsg, JC_UINT32 uiMaxMsgSize)
{
  return 0;
}

JC_RETCODE jdi_MsgSetEncodingType(MSG_HANDLE hMsg, JC_BOOLEAN bIsTextEncoding)
{
  return 0;
}

JC_RETCODE jdi_MsgSetContentClass(MSG_HANDLE hMsg, EContentClass eContentClass)
{
  return 0;
}

JC_RETCODE jdi_MsgSetCreationMode(MSG_HANDLE hMsg, ECreationMode eCreationMode)
{
  return 0;
}

JC_RETCODE jdi_MsgSetMessagingType(MSG_HANDLE hMsg, EMessagingType eMessagingType)
{
  return 0;
}

JC_RETCODE jdi_MsgSetMessageHeaders (MSG_HANDLE hMsg, MESSAGE_HEADERS *pMsgHeaders)
{
  return 0;
}

JC_RETCODE jdi_MsgDeleteMedia(MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
											JC_UINT32 uiMediaID, JC_BOOLEAN bIsAttachment)
{
  return 0;
}

JC_RETCODE jdi_MsgDeInit(MSG_HANDLE hMsg)
{
  return 0;
}

JC_RETCODE jdi_MsgParseEncapsulatedBuffer(MSG_HANDLE *phMsg,
						CONTENT_DATA *pEncapsulatedContent,
						EMessagingType eMessagingType, JC_BOOLEAN bWithMedia)
{
  return 0;
}

JC_RETCODE 	jdi_StoreSaveMsg (STORE_HANDLE hStore, JC_CHAR *pFolderName, JC_UINT16 uhCategory,
						MESSAGE_HEADERS *pMsgHeaders, CONTENT_DATA *pMsgContent,
						JC_UINT32 *pMsgIndex, JC_UINT32 uiExtraBytes)
{
  return 0;
}

JC_RETCODE 	jdi_StoreSetActiveFolder (STORE_HANDLE hStore, JC_CHAR *pFolderName)
{
  return 0;
}

JC_RETCODE	jdi_MsgGetEncapsulatedBuffer (MSG_HANDLE hMsg,
								CONTENT_DATA *pEncapsulatedContent)
{
  return 0;
}

JC_RETCODE 	jdi_MsgInit (MSG_HANDLE *phMsg, void *pCallbackArg)
{
  return 0;
}

JC_RETCODE	jdi_MsgDeletePage (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex)
{
  return 0;
}

JC_RETCODE	jdi_MsgAddSMILRegionLayout (MSG_HANDLE hMsg,
														REGION_LAYOUT *pRegionLayout)
{
  return 0;
}

JC_RETCODE	jdi_MsgGetSMILRegionLayout (MSG_HANDLE hMsg,
														REGION_LAYOUT **ppRegionLayout)
{
  return 0;
}

JC_RETCODE	jdi_MsgSetSMILRootLayout (MSG_HANDLE hMsg, JC_UINT32 uiHeight,
												JC_UINT32 uiWidth, JC_INT8 *pBGColor)
{
  return 0;
}

JC_RETCODE jdd_MMIGetDeviceCharacteristics (DeviceCharacteristics *pstCharacteristics)
{
  return 0;
}

JC_RETCODE	jdi_MsgGetSMILRootLayout (MSG_HANDLE hMsg, JC_UINT32 *pHeight,
												JC_UINT32 *pWidth, JC_INT8 **ppBGColor)
{
  return 0;
}

JC_RETCODE jdi_CommunicationFetchURL (JC_HANDLE 				vHandle,
									  const ST_COMM_FETCH_URL	*pstFetchURL,
									  JC_UINT32					*puiRequestID)
{
  return 0;
}

JC_RETCODE	jdi_MsgView (MSG_HANDLE hMsg, CONTENT_DATA *pContent)
{
  return 0;
}

JC_RETCODE 	jdi_MsgRegisterCallbacks (MSG_HANDLE hMsg,
												EMsgCallbackTypes eCallbackType, void *pCallbackFunction)
{
  return 0;
}

JC_RETCODE 	jdi_StoreCreateFolder (STORE_HANDLE hStore, JC_CHAR *pFolderName,
										JC_UINT16 *uhCategory)
{
  return 0;
}

JC_RETCODE	jdi_StoreInit (STORE_HANDLE *phStore,JC_UINT32 uiMsgType)
{
  return 0;
}

JC_RETCODE	jdi_StoreGetFolders (STORE_HANDLE hStore, FOLDER_INFO **ppFolderList)
{
  memset(&gFolderInfo,0,sizeof(FOLDER_INFO));  
  *ppFolderList = &gFolderInfo;
  return JC_OK;
}

JC_RETCODE jdi_CommunicationProfile (JC_HANDLE 				vHandle,
									 const ST_COMM_PROFILE 	*pstProfile)
{
  return 0;
}

JC_RETCODE jdi_CommunicationDataConnection (JC_HANDLE					vHandle,
											const ST_GENERIC_DATA_CONN	*pstDataConn)
{
  return 0;
}

JC_RETCODE jdi_CommunicationInitialize (const JC_TASK_INFO 			*pstAppTaskInfo,
										const JC_TASK_INFO			*pstTransportTaskInfo,
										void 						*pvCallbackArg,
										CB_COMM_GENERAL_INDICATION	cbIndication,
										JC_HANDLE					vMemHandle,
										JC_HANDLE 					*pvHandle)
{
  return 0;
}

JC_RETCODE jdi_CUtilsCharToTcharMem (const JC_INT8		*pcSrc, 
									 JC_UINT32			uiDestSize,
									 JC_CHAR			*pmDest)
{
  return 0;
}

JC_RETCODE jdi_CUtilsTcharToCharMem (const JC_CHAR		*pmSrc, 
									 JC_UINT32			uiDestSize,
									 JC_INT8			*pcDest)
{
  return 0;
}

JC_RETCODE 	jdi_StoreGetMsgList (STORE_HANDLE hStore, MESSAGE_LIST**ppMessageList,
									RETRIEVE_FILTERS *pRetrieveFilters)
{
  return 0;
}

JC_RETCODE 	jdi_StoreUpdateMsgList (STORE_HANDLE hStore, JC_BOOLEAN bflag)
{
  return 0;
}

JC_RETCODE 	jdi_StoreGetMsgFromStore (STORE_HANDLE hStore, JC_UINT32 uiMessageID, CONTENT_DATA *pMsgContent)
{
  return 0;
}

JC_RETCODE	jdi_MsgReply (MSG_HANDLE hMsg, CONTENT_DATA *pContent,
									JC_BOOLEAN bReplyAll, MSG_HANDLE *phNewMsgHandle)
{
  return 0;
}

JC_RETCODE	jdi_MsgForward (MSG_HANDLE hMsg, CONTENT_DATA *pContent,
										MSG_HANDLE *phNewMsgHandle)
{
  return 0;
}

JC_RETCODE 	jdi_StoreDeleteMsg (STORE_HANDLE hStore, JC_UINT32 uiMessageID)
{
  return 0;
}

JC_RETCODE 	jdi_StoreDeleteAllMsgs (STORE_HANDLE hStore)
{
  return 0;
}

JC_RETCODE	jdi_MsgCompose (MSG_HANDLE *phMsg, EMessagingType eMessagingType,
									CONTENT_DATA *pEncapsulatedContent)
{
  return 0;
}

JC_RETCODE	jdi_MsgGetSize (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex, JC_UINT32 *pSize)
{
  return 0;
}

JC_RETCODE 	jdi_StoreMoveMsg (STORE_HANDLE hStore, JC_UINT32 uiMessageID,
								JC_CHAR *pDestFolder)
{
  return 0;
}

JC_INT32 jdi_CUtilsTcsStrCpy (const JC_INT8 *pcSrc, JC_CHAR *pmDest)
{
  return 0;
}

JC_RETCODE jdi_CUtilsStrTcsNCpy (const JC_CHAR *pmStr, JC_INT32 iCount, JC_INT8 *pcDest)
{
  return 0;
}

JC_RETCODE jdi_CUtilsTcharToChar (JC_HANDLE vMemHandle, const JC_CHAR *pmSrc, JC_INT8 **ppcDest)
{
  *ppcDest = malloc(_tcslen(pmSrc)+1);
  _tcscpy(*ppcDest,pmSrc);
  return 0;
}

JC_INT8 * jdi_CUtilsStrCaseStr (const JC_INT8	*pcSrc, const JC_INT8	*pcSubStr)
{
  return 0;
}

JC_BOOLEAN jdi_CUtilsTcsStrEndsWith (const JC_CHAR *pmSrc,
									 const JC_INT8 *pcSubStr)
{
  return E_FALSE;
}

JC_RETCODE jdi_BookmarkSaveFavourites (JC_HANDLE vBookmarkHandle)
{
  return 0;
}

JC_RETCODE jdi_BookmarkInsertItem (JC_HANDLE vBookmarkHandle,
								   ST_BOOKMARK_ATTRIBUTES *pstBookmark,
								   ST_CATEGORY_LIST *pstParentCategory)
{
  return 0;
}

JC_RETCODE jdi_BookmarkUpdateFavourites (const JC_HANDLE vBookmarkHandle,
										 ST_BOOKMARK_ATTRIBUTES *pstOriginalBookmark,
										 ST_BOOKMARK_ATTRIBUTES *pstUpdatedBookmark,
										 ST_CATEGORY_LIST *pstParentCategory)
{
  return 0;
}

JC_RETCODE jdi_BookmarkDeleteFavourites (JC_HANDLE vBookmarkHandle,
										 ST_BOOKMARK_ATTRIBUTES *pstBookmark,
										 ST_CATEGORY_LIST *pstCategory)
{
  return 0;
}

JC_RETCODE jdi_BookmarkDeleteAll (JC_HANDLE vBookmarkHandle,
								  ST_CATEGORY_LIST *pstCategory,
								  JC_BOOLEAN bReadOnlyAlso)
{
  return 0;
}

JC_RETCODE jdi_BookmarkOpen (JC_HANDLE vBookmarkHandle,
							 ST_CATEGORY_LIST *pstCategoryList,
							 ST_CATEGORY_LIST **ppstCategoryList)
{
  return 0;
}

JC_RETCODE jdi_BookmarkInitialize (const JC_CHAR *pmBookmarkFileName,
								   JC_UINT32 uiMaxCategory,
								   JC_UINT32 uiMaxBookmark,
								   JC_BOOLEAN bDuplicateAllowed,
								   JC_HANDLE *pvBookmarkHandle)
{
  return 0;
}

JC_RETCODE jdi_BEGetPageContent (JC_HANDLE vHandle,
								 ST_BE_PAGE_CONTENT *pstPageContent)
{
  return 0;
}

JC_CHAR * jdi_CUtilsTcsStrStr (const JC_CHAR *pmSrc, 
							   const JC_INT8 *pcSubStr)
{
  return NULL;
}

JC_BOOLEAN jdi_CUtilsTcsStrStartsWith (const JC_CHAR	*pmSrc, 
									   const JC_INT8	*pcSubStr)
{
  return E_FALSE;
}

JC_RETCODE jdi_BESetEvent (JC_HANDLE vHandle, E_BE_EVENT_TYPE eEventType,
						   void *pvEventInfo)
{
  return 0;
}

const ST_HISTORY_DOMAIN * jdi_HistoryGet (JC_HANDLE vHandle)
{
  return NULL;
}

const ST_HISTORY_DOMAIN * jdi_HistorySortDomain (JC_HANDLE vHandle, 
												 E_HISTORY_SORT_ORDER  eSortOrder,
												 E_HISTORY_SORT_ITEM	eSortItem)
{
  return NULL;
}

JC_RETCODE jdi_BEFetchRequest (JC_HANDLE vHandle, const ST_BE_FETCH_REQ	*pstFetchReq)
{
  return 0;
}

JC_RETCODE jdi_HistoryDeleteDomain (JC_HANDLE vHandle, const ST_HISTORY_DOMAIN *pstDomain)
{
  return 0;
}

void jdi_HistoryClearSpecific (JC_HANDLE vHandle, const JC_CHAR *pmURL)
{
  return;
}

void jdi_HistoryClearAll (JC_HANDLE vHandle, const ST_HISTORY_DOMAIN	*pstDomain)
{
  return;
}

JC_RETCODE jdi_BEGetHistoryContext (JC_HANDLE vHandle, JC_HANDLE *pvHistoryCtxt)
{
  return 0;
}

JC_RETCODE jdi_BEStopRequest (JC_HANDLE	vHandle)
{
  return 0;
}

JC_RETCODE jdi_BEResumeRequest (JC_HANDLE vHandle, JC_UINT32 uiRequestID)
{
  return 0;
}

JC_RETCODE jdi_BESetProperties (JC_HANDLE vHandle, E_BE_SET_OPTIONS eSetOptions,
								void *pvOptionInfo)
{
  return 0;
}

JC_RETCODE jdi_BESetOption (JC_HANDLE vHandle, ST_BE_SET_OPTION *pstSetOption)
{
  return 0;
}

JC_RETCODE jdi_BESetProfile (JC_HANDLE vHandle, const ST_BE_PROFILE	*pstProfile)
{
  return 0;
}

JC_RETCODE jdi_BESetDataConn (JC_HANDLE vHandle, const ST_GENERIC_DATA_CONN	*pstDataConn)
{
  return 0;
}

void jdi_BEGetNavigationInfo (JC_HANDLE vHandle, ST_BE_NAVIGATION_INFO	*pstNavInfo)
{
  return;
}

JC_RETCODE jdi_BEReloadPage (JC_HANDLE vHandle)
{
  return 0;
}

JC_RETCODE jdi_BEHandleNavigation (JC_HANDLE vHandle, E_BE_NAVIGATION_TYPE eNavType)
{
  return 0;
}

JC_RETCODE jdi_BESizeTo (JC_HANDLE vHandle, JC_UINT32 uiWidth, JC_UINT32 uiHeight)
{
  return 0;
}

JC_RETCODE jdi_BEInitComponent (JC_HANDLE vHandle, E_BE_COMPONENT eBeComponent, void *pvComponentInfo)
{
  return 0;
}

JC_RETCODE jdi_BERefreshDisplay (JC_HANDLE vHandle)
{
  return 0;
}

JC_RETCODE jdi_BEInitialize (void *pvArg, CB_BE_GENERAL_CALLBACK cbCallbackFnc,
               JC_TASK_INFO stBrowserAppTaskInfo, JC_TASK_INFO stTransportTaskInfo,
							 JC_HANDLE *pvHandle)
{
  return 0;
}

void jdi_BEDeinitialize (JC_HANDLE	vHandle)
{
  return;
}

JC_RETCODE jdi_BEDisconnect (JC_HANDLE vHandle)
{
  return 0;
}

void jdi_BEDeinitComponent (JC_HANDLE vHandle, E_BE_COMPONENT	eBeComponent)
{
  return;
}

JC_RETCODE jdi_BEOpen (JC_HANDLE vHandle, JC_UINT32 uiWindowID)
{
  return 0;
}

JC_RETCODE jdi_BEClose	(JC_HANDLE vHandle, JC_UINT32 uiWindowID)
{
  return 0;
}

JC_RETCODE jdi_CommunicationEventHandler (JC_HANDLE vHandle, JC_EVENT_DATA *pstEventData)
{
  return 0;
}

JC_RETCODE jdi_BEGetPageInfo (JC_HANDLE vHandle, ST_BE_PAGE_INFO *pstPageInfo)
{
  return 0;
}

JC_RETCODE jdi_URLFormNext (const JC_CHAR *pmBaseURL, const JC_CHAR *pmCurrentURL, 
							JC_HANDLE vMemHandle, JC_CHAR **ppmNextURL)
{
  return 0;
}

JC_CHAR * jdi_CUtilsTcsCopyIndexNum (JC_HANDLE vMemHandle, const JC_CHAR *pmStr, JC_INT32 iBegin, JC_INT32 iNum)
{
  return NULL;
}

JC_RETCODE	jdi_MsgGetMediaList (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
									MEDIA_INFO **ppMediaInfo, JC_UINT8 *pMediaCount,
									JC_BOOLEAN bIsAttachment)
{
  return 0;
}

JC_CHAR * jdi_CUtilsTcsTrim (JC_CHAR *pmSrc)
{
  return NULL;
}

JC_RETCODE jdi_BEStopAllDownload (JC_HANDLE vHandle)
{
  return 0;
}

JC_RETCODE jdi_BESendSessionHeaders (JC_HANDLE						vHandle,
									 const ST_BE_SESSION_HEADER		*pstSessionHeader)
{
  return 0;
}

const ST_HISTORY_HOST * jdi_HistorySortHost (JC_HANDLE					vHandle,
											 E_HISTORY_SORT_ORDER		eSortOrder,
											 E_HISTORY_SORT_ITEM		eSortItem,
											 const ST_HISTORY_DOMAIN	*pstDomain)
{
  return NULL;
}

JC_RETCODE	jdi_MsgAsyncErrorIndCBHandler (MSG_HANDLE		hMsg, 
												   ErrorCode		eErrorType, 
												   JC_RETCODE		eAsyncResponse, 
												   void				*pReturnInfo)
{
  return 0;
}

JC_RETCODE	jdi_MsgGetMediaCount (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
											JC_UINT8 *pMediaCount, JC_BOOLEAN bIsAttachment)
{
  return 0;
}

JC_RETCODE	jdi_MsgGetPageTimer (MSG_HANDLE hMsg, JC_UINT8 ucPageIndex,
											JC_UINT32 *pDuration)
{
  return 0;
}

JC_RETCODE jdi_CommunicationStopReq (JC_HANDLE 			vHandle,
									 JC_UINT32			uiRequestID)
{
  return 0;
}

JC_INT16 jdi_CUtilsTcsStrCmp (const JC_CHAR		*pmFirstStr, 
							  const JC_INT8		*pcSecondStr)
{
  return 0;
}

JC_RETCODE jdi_StoreRestoreFolderInfo (STORE_HANDLE hStore)
{
  return 0;
}

JC_RETCODE jdi_CUtilsCharToTcharNMem (const JC_INT8		*pcSrc, 
									  JC_INT32			iSrcLength,
									  JC_UINT32			uiDestSize, 
									  JC_CHAR			*pmDest)
{
  return 0;
}

JC_INT32 jdi_CUtilsStrIndexOf (const JC_INT8	*pcSrc, 
							   JC_INT8			cChar,
							   JC_UINT32		uiStart)
{
  return 0;
}

JC_RETCODE jdi_CommunicationDisconnect (JC_HANDLE 		vHandle)
{
  return 0;
}

JC_RETCODE jdi_CommunicationSetOption (JC_HANDLE				vHandle,
									   const ST_COMM_SET_OPTION	*pstCommSetOption)
{
  return 0;
}			

JC_RETCODE jdi_StoreUpdateMsg (STORE_HANDLE hStore, JC_UINT32 uiMessageID,
										MESSAGE_HEADERS *pMsgHeaders, CONTENT_DATA *pMsgContent)
{
  return 0;
}

JC_RETCODE jdi_CUtilsTcsStrNCpy(JC_HANDLE vMemHandle,
								 const JC_INT8	*pcStr, 
								 JC_INT32		iCount, 
								 JC_CHAR		**ppmDest)
{
  return 0;
}

JC_RETCODE jdi_BookmarkDeInitialize (JC_HANDLE 	vBookmarkHandle)
{
  return 0;
}

JC_RETCODE 	jdi_StoreDeinit (STORE_HANDLE hStore)
{
  return 0;
}

JC_RETCODE	jdi_MsgGetMaxPages (MSG_HANDLE hMsg, JC_UINT8 *pMaxPages)
{
  return 0;
}

JC_RETCODE jdi_CUtilsTcsURIUnescape (JC_CHAR *pmURL)
{
  return 0;
}

void jdi_CUtilsStrToLower (JC_INT8		*pcStr)
{

}

JC_RETCODE jdi_BEGetFocusedControlInfo (JC_HANDLE				vHandle,
										ST_BE_CONTROL_INFO		*pstCtrlInfo) 
{
  return 0;
}

JC_INT32 jdi_CUtilsTcsIndexOfSubStr (const JC_CHAR	*pmStr, 
									 const JC_INT8	*pcSubStr, 
									 JC_INT32		iStartIndex)
{
	return 0;
}

#endif
