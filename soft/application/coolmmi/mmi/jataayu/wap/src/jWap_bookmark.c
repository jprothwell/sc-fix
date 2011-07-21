#define MODULE_NAME "JWAP"
#define FILE_NAME "jwap_bookmark.c"
/***************************************************************************
*
 
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
* THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND
* AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS
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
***************************************************************************/

/**
 
* @ingroup	jWAP
* @brief		Describes the Task related API's.
*
**/

#ifdef JATAAYU_SUPPORT
/***************************************************************************
* System Include Files
**************************************************************************/


/***************************************************************************
* User Include Files
**************************************************************************/
#include "jwap_inc.h"


#include "jdi_cutils_tcsstr.h"
#include "jwap_interface.h"
#include "jwap_mmi_cb.h"
#include "commonscreens.h"
#include "jwap_mmi_cb.h"
#ifdef MMI_ON_WIN32
#include "jdi_bookmark.h"
#endif
#include "jdi_cutils_tcs.h"
#include "settingprofile.h"
#include "jwap_str_id.h"
#include "jwap_push.h"

#define BM_TITLE_LEN 20

U8* 		g_pcBMTitleValue ;
JC_INT32	g_bm_selected_index ;
JC_UINT32 	g_bkListCount = 0;
JC_UINT8* 	g_puBookMarkList[BOOKMARK_COUNT] ; 

void 		jWap_addEditBm_entry_handler () ;
static JC_RETCODE jWap_addEditBm_saveToFile () ;

extern void GBSetInputboxLSKFunction(FuncPtr f);

typedef enum _jwap_bookmark_add
{
	E_WAP_BM_TITLE_CAPTION,
	E_WAP_BM_TITLE_ITEM,
	E_WAP_BM_URL_CAPTION,
	E_WAP_BM_URL_ITEM,
	E_WAP_BM_TOTAL_ITEMS
}E_WAP_BOOKMARK_ADD ;

typedef struct _add_edit_bm_info
{
	S8		    			*s8Title ; //The GUI buffer size is 1.5KB. So, title length is reduced to 20B
	S8						*s8Url ;
	ST_BOOKMARK_ATTRIBUTES 	*pstBookmarkAttributes ;
	InlineItem 				wgui_inline_list_items [E_WAP_BM_TOTAL_ITEMS] ;
	BOOL					bFromFetchdPage ;
}ST_ADD_BM_INFO ;

static ST_ADD_BM_INFO *pstBmAddOptInfo ;


void jWap_freeBookmarkInfo ()
{
	if (pstBmAddOptInfo)
	{
		if (pstBmAddOptInfo->s8Title)
		{
			jdd_MemFree (pstBmAddOptInfo->s8Title) ;
		}

		jdd_MemFree (pstBmAddOptInfo->s8Url) ;	
		jdd_MemFree (pstBmAddOptInfo) ;
		pstBmAddOptInfo = JC_NULL ;
	}
}

void jWap_addEditBm_addInterface (JC_CHAR *pmTitle, JC_CHAR *pmUrl)
{
	if (pstBmAddOptInfo)
	{
		jdd_MemFree (pstBmAddOptInfo) ;
	}
	pstBmAddOptInfo = jdd_MemAlloc (1, sizeof (ST_ADD_BM_INFO)) ;

	if (NULL != pmTitle)
	{
		pstBmAddOptInfo->s8Title = jdd_MemAlloc  (sizeof (JC_CHAR), BM_TITLE_LEN + 1) ;
		jc_wstrncpy (pstBmAddOptInfo->s8Title, pmTitle, BM_TITLE_LEN) ;
	}
	pstBmAddOptInfo->s8Url = (S8*)jdi_CUtilsTcsDuplicate (0, pmUrl) ;
	if (jc_wstrlen (pmUrl) > jWAP_MAX_INPUT_SIZE)
	{
	    pstBmAddOptInfo->s8Url [jWAP_MAX_INPUT_SIZE] = (JC_CHAR)'\0' ;
	}

	pstBmAddOptInfo->bFromFetchdPage = E_TRUE ;
	jWap_addEditBm_saveToFile () ;
	pstBmAddOptInfo->bFromFetchdPage = E_FALSE ;	
	jWap_freeBookmarkInfo () ;
}

JC_RETCODE jWap_addEditBm_saveToFile ()
{
	ST_BOOKMARK_ATTRIBUTES 	stBookMarkAttributes ;
	JC_RETCODE				rCode = JC_ERR_INVALID_PARAMETER;
	JC_CHAR					*pmTempUrl = JC_NULL, *pmTempTitle = JC_NULL ;	
	
	if (jc_wstrlen (pstBmAddOptInfo->s8Url))
	{
		jc_memset (&stBookMarkAttributes, 0, sizeof (ST_BOOKMARK_ATTRIBUTES)) ;

		if(jc_wstrlen(pstBmAddOptInfo->s8Title))
		{
			pmTempTitle = jdi_CUtilsTcsDuplicate (NULL, (JC_CHAR *) \
								pstBmAddOptInfo->s8Title ) ;
			jdi_CUtilsTcsTrim (pmTempTitle) ;

			if (pmTempTitle)
				stBookMarkAttributes.pmTitle = pmTempTitle ;
		}
		
		stBookMarkAttributes.pmURL = (JC_CHAR*)pstBmAddOptInfo->s8Url ;

		if (!aapIsValidTcharUrl (stBookMarkAttributes.pmURL, \
						&pmTempUrl, NULL))
		{
			rCode = JC_ERR_INVALID_PARAMETER ;
		}
		else
		{
			stBookMarkAttributes.pmURL = pmTempUrl ;
			rCode = JC_OK;
		}
		
		if (JC_OK == rCode)
		{
			if (pstBmAddOptInfo->pstBookmarkAttributes)
			{				
				rCode = jdi_BookmarkUpdateFavourites ( \
					g_pstJWapAppInfo->vBookMarkHandle, \
					pstBmAddOptInfo->pstBookmarkAttributes, \
					&stBookMarkAttributes, NULL);
			}
			else
			{
				
				rCode = jdi_BookmarkInsertItem(g_pstJWapAppInfo->vBookMarkHandle, \
							&stBookMarkAttributes, NULL);
			}
		}		
		if(JC_OK == rCode)
		{
			rCode = jdi_BookmarkSaveFavourites (g_pstJWapAppInfo->vBookMarkHandle) ; // persistant saving
			if(rCode == JC_OK)
			{
				DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), \
					IMG_GLOBAL_ACTIVATED, 1, 2000, 1);
			}
			else
			{
				DisplayPopup((PU8)GetString(STR_GLOBAL_INVALID), \
					IMG_GLOBAL_ERROR, 1, 2000, 1);
			}
		}
		else if(JC_ERR_BOOKMARK_FAVOURITE_LIMIT == rCode)
		{
			DisplayPopup((PU8)GetString(STR_ID_WAP_BM_MAX_LIMIT), \
				IMG_GLOBAL_ERROR, 1, 2000, 1);
		}
		else if(JC_ERR_BOOKMARK_FAVOURITE_EXIST == rCode)
		{
			DisplayPopup((PU8)GetString(STR_ID_WAP_BM_ALREADY_EXITS), \
				IMG_GLOBAL_ERROR, 1, 2000, 1);
		}
		else if (JC_ERR_INVALID_PARAMETER == rCode)
		{
			DisplayPopup((PU8)GetString(STR_GLOBAL_INVALID), \
				IMG_GLOBAL_ERROR, 1, 2000, 1);
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_GLOBAL_INVALID), \
				IMG_GLOBAL_ERROR, 1, 2000, 1);
			mmi_trace (1, "@@@ jWapAddBookSaveTitle @@@ rcode else %d",rCode) ;
		}        
		DeleteUptoScrID(SCR_ID_WAP_BOOKMARK);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_ID_WAP_BM_NO_URL), IMG_GLOBAL_ERROR, \
			1, 2000, 1);	
	}

	if (pmTempUrl)
	{
		jdd_MemFree (pmTempUrl) ;
	}

	if (pmTempTitle)
	{
		jdd_MemFree (pmTempTitle) ;
	}
	return rCode;
}

void jWap_addEdit_exit_handler (void)
{
	history_t   Scr; 
	S16 nHistory = 0;
	U16 inputBufferSize;                //added for inline edit history

	Scr.scrnID = SCR_ID_WAP_ADD_EDIT_BM;
	CloseCategory57Screen();
	Scr.entryFuncPtr = jWap_addEditBm_entry_handler;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, (U16)inputBufferSize);                       //added for inline edit history
}

void jWap_addEdit_highlight_handler ()
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(GoBackHistory);
}

#ifdef	JWAP_PUSH_ENABLE
extern ST_WAP_PUSH_INFO	*g_pstPush_info;
#endif

void jWap_bmSaveRSKHandler ()
{
	JC_RETCODE rCode;
	rCode = jWap_addEditBm_saveToFile () ;
	if(JC_OK == rCode)
	{
		jWap_freeBookmarkInfo () ;
	}
	#ifdef	JWAP_PUSH_ENABLE
	if(g_pstPush_info && (E_TRUE == g_pstPush_info->bIsBookMarkAdded))
	{		
		DeleteUptoScrID(SCR_ID_WAP_SERVICE_MSG_OPTIONS_SCREEN);
		jWap_freeBookmarkInfo () ;
		if (g_pstJWapAppInfo->vBookMarkHandle)
		{
			rCode = jdi_BookmarkDeInitialize (g_pstJWapAppInfo->vBookMarkHandle) ;
			g_pstJWapAppInfo->vBookMarkHandle = JC_NULL ;
		}
	
	}
	#endif
}

void jWap_addEdit_lsk_handler()
{
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
	SetLeftSoftkeyFunction (EntryScrForInputMethodAndDone, KEY_EVENT_UP);
}

extern void jWap_addEditBm_entry_handler();

void jWap_addEditBm_exit_handler(void)
{
	history_t   Scr; 
	S16 nHistory = 0;
	U16 inputBufferSize;                //added for inline edit history

	Scr.scrnID = SCR_ID_WAP_ADD_EDIT_BM;
	CloseCategory57Screen();
	Scr.entryFuncPtr = jWap_addEditBm_entry_handler;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, (U16)inputBufferSize);                       //added for inline edit history
}

void Fill_Inline_Struct(void *pComposeInlineStruct) 
{
	InlineItem *InlineStruct = (InlineItem *)pComposeInlineStruct;
	if(pstBmAddOptInfo)
	{
		SetInlineItemCaption(&InlineStruct[E_WAP_BM_TITLE_CAPTION], (U8 *) GetString(STR_ID_WAP_BM_TITLE));	
		SetInlineItemActivation(&InlineStruct[E_WAP_BM_TITLE_ITEM], KEY_LSK, KEY_EVENT_UP);
		SetInlineItemFullScreenEdit(&InlineStruct[E_WAP_BM_TITLE_ITEM],STR_ID_WAP_BM_TITLE,0,
									(PU8)pstBmAddOptInfo->s8Title, 
									BM_TITLE_LEN, 
									(INPUT_TYPE_ALPHANUMERIC_SENTENCECASE | INPUT_TYPE_USE_ENCODING_BASED_LENGTH | INPUT_TYPE_ONE_LESS_CHARACTER));
		
		SetInlineFullScreenEditCustomFunction(&InlineStruct[E_WAP_BM_TITLE_ITEM], jWap_addEdit_lsk_handler);

		SetInlineItemCaption(&InlineStruct[E_WAP_BM_URL_CAPTION], (U8 *) GetString(STR_ID_WAP_BM_URL));	
		SetInlineItemActivation(&InlineStruct[E_WAP_BM_URL_ITEM], KEY_LSK, KEY_EVENT_UP);
		SetInlineItemFullScreenEdit(&InlineStruct[E_WAP_BM_URL_ITEM],STR_ID_WAP_BM_URL,0,
									(PU8)pstBmAddOptInfo->s8Url, 
									jWAP_MAX_INPUT_SIZE, 
									(INPUT_TYPE_ALPHANUMERIC_LOWERCASE | INPUT_TYPE_OVERRIDE_DEFAULT | INPUT_TYPE_USE_ONLY_ENGLISH_MODES));
		
		SetInlineFullScreenEditCustomFunction(&InlineStruct[E_WAP_BM_URL_ITEM], jWap_addEdit_lsk_handler);
	}
}

void jWap_addEditBm_add_highlight_hdlr(S32 nIndex)
{
	mmi_trace (1, "JDC_LOG: jWap_bmInitBookMark view change[%d]", jc_strlen(pstBmAddOptInfo->s8Url)) ;	
	if(0 < jc_strlen(pstBmAddOptInfo->s8Url))
	{
		ChangeRightSoftkey(STR_GLOBAL_DONE, 0);
		SetCategory57RightSoftkeyFunctions(jWap_bmSaveRSKHandler, jWap_bmSaveRSKHandler);
	}
	else
	{
		SetCategory57RightSoftkeyFunctions(jWap_bmSaveRSKHandler, jwap_SaveBMRSKCallBack);
	}		
}

void jWap_addEditBm_entry_handler()
{
	U8* guiBuffer;
	U8* inputBuffer;     
	U16 inputBufferSize;                
    InlineItem wgui_inline_list_items[E_WAP_BM_TOTAL_ITEMS];
	U16 icon_list[E_WAP_BM_TOTAL_ITEMS] = {0};
	
	EntryNewScreen(SCR_ID_WAP_ADD_EDIT_BM, jWap_addEditBm_exit_handler, \
		NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_WAP_ADD_EDIT_BM);
	InitializeCategory57Screen();
	jc_memset( wgui_inline_list_items, 0, sizeof(wgui_inline_list_items) );
	
	Fill_Inline_Struct((void *)wgui_inline_list_items);
	
	inputBuffer = GetCurrNInputBuffer (SCR_ID_WAP_ADD_EDIT_BM, &inputBufferSize );//added for inline edit history

	if(inputBuffer != NULL)
		SetCategory57Data(wgui_inline_list_items, E_WAP_BM_TOTAL_ITEMS, \
		inputBuffer);

	icon_list[E_WAP_BM_TITLE_CAPTION] = gIndexIconsImageList[0];
	icon_list[E_WAP_BM_URL_CAPTION] = gIndexIconsImageList[1];
	RegisterHighlightHandler(jWap_addEditBm_add_highlight_hdlr );
	ShowCategory57Screen(STR_ID_WAP_BM_ADD,IMG_ID_JATAAYU_WAP_ICON,
                        STR_GLOBAL_OK,IMG_GLOBAL_OK,
                        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
                        E_WAP_BM_TOTAL_ITEMS,
                        icon_list,wgui_inline_list_items,0,guiBuffer);

#if defined(__MMI_GB__)  //add by yaosq 20061110 for leftsoft in profile
	mmi_frm_set_highlight_handler ();
	GBSetInputboxLSKFunction (EntryScrForInputMethodAndDone); 
#endif	
	//SetCategory57RightSoftkeyFunctions(jWap_bmSaveRSKHandler, jwap_SaveBMRSKCallBack);	
}

void jWap_BookMark_exit_handler()
{
	FreeBookmarkList(g_puBookMarkList, g_bkListCount);
}

void jWap_BookMark_entry_handler ()
{	 
	JC_RETCODE 				rCode ;	
	U8						*guiBuffer = NULL;
	JC_INT32				iListCount ;
	rCode = JC_OK ;

	mmi_trace (1, "@@@ jWap_bmInitBookMark begining@@@") ;	
	jc_memset (g_puBookMarkList, 0, (sizeof (JC_UINT8*) * BOOKMARK_COUNT)) ;	
	if(NULL == g_pstJWapAppInfo->vBookMarkHandle)
	{
		mmi_trace (1, "@@@ jWap_bmInitBookMark @@@") ;
	 	jWap_bmInitBookMark() ;
	}
	 
	if(NULL != g_pstJWapAppInfo->vBookMarkHandle)
	{
		mmi_trace (1, "@@@ jWap_bmFillBookMarkToList @@@") ;
		iListCount = jWap_bmFillBookMarkToList(g_puBookMarkList) ;
		g_bkListCount = iListCount;
		
		mmi_trace (1, "@@@ if condition @@@") ;
		EntryNewScreen (SCR_ID_WAP_BOOKMARK, jWap_BookMark_exit_handler, \
			jWap_BookMark_entry_handler, NULL) ;
		RegisterHighlightHandler (jWap_BM_options_highlight_handler) ;
		guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_BOOKMARK) ;		

		if(JC_NULL != g_puBookMarkList[0])
		{
				ShowCategory84Screen(STR_ID_WAP_BOOKMARKS, 
                 IMG_ID_JATAAYU_WAP_ICON,
                 STR_GLOBAL_OPTIONS,
                 IMG_GLOBAL_OPTIONS,
                 STR_GLOBAL_BACK,
                 IMG_GLOBAL_BACK,
                 iListCount,
                 (U8**)g_puBookMarkList,
   				 (PU16)gIndexIconsImageList,
   				 LIST_MENU,
   				 0,
   				 guiBuffer);
				
			SetLeftSoftkeyFunction (jWap_bmOptionBookMark, \
				KEY_EVENT_UP);
		}
		else
		{
				ShowCategory84Screen(STR_ID_WAP_BOOKMARKS, 
                 IMG_ID_JATAAYU_WAP_ICON,
                 STR_ID_WAP_BM_ADD,
                 IMG_GLOBAL_OPTIONS,
                 STR_GLOBAL_BACK,
                 IMG_GLOBAL_BACK,
                 iListCount,
                 (U8**)g_puBookMarkList,
   				 NULL,
   				 LIST_MENU,
   				 0,
   				 guiBuffer);
			SetLeftSoftkeyFunction(jWapAddBookSaveTitle, \
				KEY_EVENT_UP);
		}
		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);		
	}
}

void FreeBookmarkList(JC_UINT8** puBkkList,JC_INT32 iListCount)
{
	JC_INT32 iIndex ;

	mmi_trace (1, "@@@ FreeBookmarkList begin count %d @@@", iListCount) ;
	
	for(iIndex = 0;iIndex<iListCount;iIndex++)
		{
			jdd_MemFree(puBkkList[iIndex]) ;
			puBkkList[iIndex] = NULL ;
		}
	mmi_trace (1, "@@@ FreeBookmarkList end @@@") ;
}

void jWap_BM_options_highlight_handler(int iSelectIndex)
{
	g_bm_selected_index = iSelectIndex ;
}

void jWapAddBookSaveTitle()
{
	ST_BOOKMARK_ATTRIBUTES	*pstBookmarkAttributes ;
	ST_CATEGORY_LIST		*pstBookMarkList ;
	JC_INT32				iIndexCount ;
		
	pstBookmarkAttributes = NULL ;
	pstBookMarkList 	  = NULL ;
	iIndexCount = 0 ;

	if (pstBmAddOptInfo)
	{
		jdd_MemFree (pstBmAddOptInfo) ;
		pstBmAddOptInfo = JC_NULL ;
	}

	pstBmAddOptInfo = jdd_MemAlloc(1, sizeof(ST_ADD_BM_INFO)) ;
	pstBmAddOptInfo->s8Title = jdd_MemAlloc (sizeof (JC_CHAR), BM_TITLE_LEN + 1) ;
	pstBmAddOptInfo->s8Url = jdd_MemAlloc (sizeof (JC_CHAR), jWAP_MAX_INPUT_SIZE + 1) ;
	jWap_addEditBm_entry_handler () ;
}

void jWap_bmOptionBookMark() 
{	
	U16 item_texts[] = {STR_ID_WAP_GO, \
						STR_ID_WAP_BM_EDIT,
						STR_ID_WAP_BM_CLEAR,
						STR_ID_WAP_BM_CLEARALL,
						STR_ID_WAP_ADD_TO_BM	,
						} ;

	mmi_trace (1, "@@@ jWap_options_handler. Items @@@") ;

	EntryNewScreen (SCR_ID_BM_WAP_OPTIONS, NULL, NULL, NULL) ;
	RegisterHighlightHandler (jWap_bmoptions_highlight_handler) ;
	SetParentHandler(SCR_ID_WAP_BOOKMARK);
	ShowCategory15Screen (STR_ID_WAP_BOOKMARKS, IMG_ID_JATAAYU_WAP_ICON, 
							STR_GLOBAL_OK,
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,
							IMG_GLOBAL_BACK,
							(sizeof (item_texts)/sizeof (U16)), 
							item_texts,
							(PU16)gIndexIconsImageList, 
							1, 0, NULL) ;
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;	
}

void jWap_bmoptions_highlight_handler(int iSelectIndex) 
{
	mmi_trace (1, "@@@ jWap_bmoptions_highlight_handler index %d @@@", iSelectIndex) ;
	switch(iSelectIndex)
	{
		case 0:
			SetLeftSoftkeyFunction (jWap_bmfetchBookMark, KEY_EVENT_UP) ;
			break ;
		case 1:
			SetLeftSoftkeyFunction (jWap_bmEditBookMark, KEY_EVENT_UP) ;
			break ;
		case 2:
			SetLeftSoftkeyFunction (jWap_bmClearBookMark, KEY_EVENT_UP) ;
			break ;
			
		case 3:
			SetLeftSoftkeyFunction (jWap_bmClearAllBookMark, KEY_EVENT_UP) ;
			break ;
		case 4:
			SetLeftSoftkeyFunction (jWap_bmAddBookMark, KEY_EVENT_UP) ;
			break ;
		default:
			break ;
	}
}

void jWap_bmEditBookMark()
{
	ST_BOOKMARK_ATTRIBUTES	*pstBookmarkAttributes ;
	ST_CATEGORY_LIST		*pstBookMarkList ;
	JC_INT32				iIndexCount ;
		
	pstBookmarkAttributes = NULL ;
	pstBookMarkList 	  = NULL ;
	iIndexCount = 0 ;

	if (pstBmAddOptInfo)
	{
		jdd_MemFree (pstBmAddOptInfo) ;
		pstBmAddOptInfo = JC_NULL ;
	}	
	pstBmAddOptInfo = jdd_MemAlloc(1, sizeof(ST_ADD_BM_INFO));
	pstBookMarkList = jWap_bmGetCatagoryListPtr() ;
	if(NULL != pstBookMarkList)
	{
		pstBookmarkAttributes = pstBookMarkList->pstBookmarkAttributes ;
		while(NULL != pstBookmarkAttributes)
		{
			if(iIndexCount < g_bm_selected_index)
			{
				pstBookmarkAttributes = pstBookmarkAttributes->pstNext ;
				iIndexCount ++ ;
			}
			else
			{
				break ;
			}
		}
		
        if(NULL != pstBookmarkAttributes)
        {
    		pstBmAddOptInfo->pstBookmarkAttributes = pstBookmarkAttributes ;
			pstBmAddOptInfo->s8Title = jdd_MemAlloc (sizeof(JC_CHAR), \
											BM_TITLE_LEN + 1) ;
			if(NULL != pstBookmarkAttributes->pmTitle)
			{
				jc_wstrncpy(pstBmAddOptInfo->s8Title, pstBookmarkAttributes->pmTitle, \
					BM_TITLE_LEN) ; 
			}

			pstBmAddOptInfo->s8Url = jdd_MemAlloc (sizeof(JC_CHAR), \
											jWAP_MAX_INPUT_SIZE + 1) ;
			if(NULL != pstBookmarkAttributes->pmURL)
			{
				jc_wstrncpy(pstBmAddOptInfo->s8Url, pstBookmarkAttributes->pmURL, \
					jWAP_MAX_INPUT_SIZE) ;
			}
		}
	}	
	jWap_addEditBm_entry_handler () ;	
}

void jWap_bmAddBookMark()
{
	mmi_trace (1, "@@@ jWap_bmAddBookMark @@@") ;

	jWapAddBookSaveTitle() ;
}

void jWap_bmClearBookMark()
{
	GetBmClearUserConfirmation() ;
}

void jWap_bmClearAllBookMark()
{
	GetBmClearAllUserConfirmation() ;
}

void GetBmClearUserConfirmation()
{
	mmi_trace (1, "@@@ GetUserConfirmation. begin");
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, get_string(STR_ID_WAP_CLEAR_CONFIRMATION), IMG_GLOBAL_QUESTION, WARNING_TONE) ;
	SetLeftSoftkeyFunction(HandleBmClearYes,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction(HandleBmNo,KEY_EVENT_UP) ;
	mmi_trace (1, "@@@ GetUserConfirmation. end");
}

void GetBmClearAllUserConfirmation()
{
	mmi_trace (1, "@@@ GetUserConfirmation. begin");
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, get_string(STR_ID_WAP_CLEAR_ALL_CONFIRMATION), IMG_GLOBAL_QUESTION, WARNING_TONE) ;
	SetLeftSoftkeyFunction(HandleBmClearAllYes,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction(HandleBmNo,KEY_EVENT_UP) ;
	mmi_trace (1, "@@@ GetUserConfirmation. end");
}

void jWap_GoBackToOption()
{
	if (IsScreenPresent(SCR_ID_WAP_OPTIONS))
		DeleteUptoScrID(SCR_ID_WAP_OPTIONS);
	else
		DeleteUptoScrID(SCR_ID_WAP_MAIN_SCREEN);
}

void HandleBmClearYes()
{
	ST_BOOKMARK_ATTRIBUTES	*pstBookmarkAttributes ;
	ST_CATEGORY_LIST* pstBookMarkList ;
	JC_INT32		iIndexCount ;
	JC_RETCODE 		rCode ;
	JC_UINT8		*puCBookMarkList[BOOKMARK_COUNT] ; 

	pstBookmarkAttributes = NULL ;
	iIndexCount = 0 ; 
	rCode = JC_OK ;

	pstBookMarkList = jWap_bmGetCatagoryListPtr() ;
	if(NULL != pstBookMarkList)
	{
		pstBookmarkAttributes = pstBookMarkList->pstBookmarkAttributes ;
		while(NULL != pstBookmarkAttributes)
		{
			if(iIndexCount < g_bm_selected_index)
			{
				pstBookmarkAttributes = pstBookmarkAttributes->pstNext ;
				iIndexCount ++ ;
			}
			else
			{
				break ;
			}
		}		
		mmi_trace (1, "@@@ fav count is%d @@@", iIndexCount) ;		
		if(NULL != pstBookmarkAttributes)
		{
			rCode = jdi_BookmarkDeleteFavourites(g_pstJWapAppInfo->vBookMarkHandle, \
												pstBookmarkAttributes,NULL) ;
			if(JC_OK == rCode)
			{
				jdi_BookmarkSaveFavourites (g_pstJWapAppInfo->vBookMarkHandle) ;
				iIndexCount = jWap_bmFillBookMarkToList(puCBookMarkList) ;
				if(JC_NULL != puCBookMarkList[0])
				{
					GoBackHistory() ;
				}
				else
				{
					GoBackHistory() ;					
					SetLeftSoftkeyFunction (jWap_bmAddBookMark, KEY_EVENT_UP) ;
					SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;
				}
				DisplayPopup((PU8)GetString(STR_ID_WAP_DELETE_SUCCESS), IMG_GLOBAL_ACTIVATED, 1, 2000, 1);
			}
			else
			{
				DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1,1000, 1);
			}
		}
	}
}

void HandleBmClearAllYes()
{
	ST_CATEGORY_LIST* pstBookMarkList  ;
	JC_RETCODE			rCode ;
	rCode = JC_OK ;
	pstBookMarkList = NULL ;
	pstBookMarkList = jWap_bmGetCatagoryListPtr() ;
	if(NULL != pstBookMarkList)
	{
		rCode = jdi_BookmarkDeleteAll(g_pstJWapAppInfo->vBookMarkHandle, \
										pstBookMarkList,E_TRUE) ;
		if(JC_OK == rCode)
		{
			jdi_BookmarkSaveFavourites (g_pstJWapAppInfo->vBookMarkHandle) ;				
			DisplayPopup(GetString (STR_ID_WAP_DELETE_SUCCESS), IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
			jWap_GoBackToOption();				
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1,1000, 1);
		}
	}
}

void HandleBmNo()
{
	GoBackHistory() ;
}

void jWap_bmfetchBookMark()
{
	ST_CATEGORY_LIST			*pstBookMarkList ;
	ST_BOOKMARK_ATTRIBUTES	*pstBookmarkAttributes ;
	INT							iErr ;
	int 							index ;

	index 					= 0 ;
	pstBookMarkList 			= NULL ;
	pstBookmarkAttributes 		= NULL ;
	pstBookMarkList 			= NULL ;	

	pstBookMarkList = jWap_bmGetCatagoryListPtr() ;
	mmi_trace (1, "@@@ jWap_bmfetchBookMark begin @@@") ;
	if(NULL != pstBookMarkList)
	{
		pstBookmarkAttributes = pstBookMarkList->pstBookmarkAttributes ;

		while(NULL != pstBookmarkAttributes)
		{
			if(index < g_bm_selected_index)
			{
				pstBookmarkAttributes = pstBookmarkAttributes->pstNext ;
				index++ ;				
			}
			else
			{
				break ;
			}
		}
	}

	if(pstBookmarkAttributes && pstBookmarkAttributes->pmURL)
	{
		iErr = 0 ;
			
		if (!iErr)
		{
			iErr = jWap_config_connProfile () ;
		}
		if (!iErr)
		{
			if(IsScreenPresent (SCR_ID_WAP_HOMEPAGE_SCREEN))
			{
				DeleteUptoScrID (SCR_ID_WAP_HOMEPAGE_SCREEN) ;
				GoBackHistory () ;
			}
			else
			{
				DeleteScreenIfPresent (SCR_ID_WAP_BOOKMARK) ;
				DeleteScreenIfPresent (SCR_ID_BM_WAP_OPTIONS) ;
				jWap_set_priMmiScreen() ;
				iErr = jWap_config_dispSettings () ;
			}
			
			jWap_setConnectingTitle((JC_CHAR*)GetString( \
				STR_ID_WAP_CONNECTING)) ;
			jWap_send_fetch_request(pstBookmarkAttributes->pmURL) ;
		}
	}	

}

ST_CATEGORY_LIST* jWap_bmGetCatagoryListPtr()
{
	JC_RETCODE rCode ; 
	ST_CATEGORY_LIST* pstBookMarkList = NULL ;

	rCode = JC_OK ;	
	rCode = jdi_BookmarkOpen (g_pstJWapAppInfo->vBookMarkHandle, NULL, \
								&pstBookMarkList) ;				
	return pstBookMarkList ;
}

JC_INT32 jWap_bmFillBookMarkToList(JC_UINT8 **puBookMarkList)
{
	JC_INT32 iIndex ; 
	ST_BOOKMARK_ATTRIBUTES* pstBookMarkAttributes ;
	ST_CATEGORY_LIST* pstBookMarkList ;

	iIndex = 0 ;
	pstBookMarkList  = NULL ;
	
	pstBookMarkList = jWap_bmGetCatagoryListPtr() ;
	if( NULL != pstBookMarkList)
	{
		pstBookMarkAttributes = pstBookMarkList->pstBookmarkAttributes ;
		while(NULL != pstBookMarkAttributes)
		{
			if(0 != jc_wstrlen(pstBookMarkAttributes->pmTitle))
				{
					puBookMarkList[iIndex] = jdd_MemAlloc((jc_wstrlen(pstBookMarkAttributes->pmTitle)+1), sizeof(JC_CHAR)) ;
					jc_wstrcpy((JC_CHAR*)puBookMarkList[iIndex],(JC_CHAR*)(pstBookMarkAttributes->pmTitle)) ;
					pstBookMarkAttributes = pstBookMarkAttributes->pstNext ;
					iIndex++ ;
				}
			else
				{
					puBookMarkList[iIndex] = jdd_MemAlloc((jc_wstrlen(pstBookMarkAttributes->pmURL)+1), sizeof(JC_CHAR)) ;
					jc_wstrcpy((JC_CHAR*)puBookMarkList[iIndex],(JC_CHAR*)(pstBookMarkAttributes->pmURL)) ;
					pstBookMarkAttributes = pstBookMarkAttributes->pstNext ;
					iIndex++ ;
				}
		}	
	}			
	return iIndex ;
}

void jWap_bmInitBookMark()
{
	JC_RETCODE 				rCode ;
	U16						*pszBookMark ; 
	ST_BOOKMARK_SETTINGS 	*pstBookmarkSettings ;

	pszBookMark = JC_NULL ;
	mmi_trace (1, "@@@ jWap_bmInitBookMark inside @@@") ;
	rCode = jdi_CUtilsCharToTchar(NULL, CS_WAP_BOOKMARK_FILE_NAME, &(pszBookMark)) ;

	if(JC_OK == rCode)
	{
		pstBookmarkSettings = JC_NULL ;
		pstBookmarkSettings = (ST_BOOKMARK_SETTINGS*)jdd_MemAlloc(1,sizeof(ST_BOOKMARK_SETTINGS)) ;
		if(NULL != pstBookmarkSettings)
		{
			mmi_trace (1, "@@@ jWapAddBookSaveTitle before core api after first alloc@@@") ;
			pstBookmarkSettings->pmBookmarkFileName = (JC_CHAR*)jdd_MemAlloc(jc_wstrlen(pszBookMark)+1,sizeof(JC_CHAR)) ;
			if(NULL != pstBookmarkSettings->pmBookmarkFileName)
			{
				mmi_trace (1, "@@@ jWapAddBookSaveTitle before core api after second alloc@@@") ;
				jc_wstrncpy(pstBookmarkSettings->pmBookmarkFileName,pszBookMark,jc_wstrlen(pszBookMark)) ;

				pstBookmarkSettings->uiMaxCategory		= 1 ;
				pstBookmarkSettings->uiMaxBookmark		= BOOKMARK_COUNT ;
				pstBookmarkSettings->bFileMultiStorage  = E_FALSE ;
				mmi_trace (1, "@@@ jWapAddBookSaveTitle before core api@@@") ;

				rCode = jdi_BookmarkInitialize (pstBookmarkSettings->pmBookmarkFileName,
						pstBookmarkSettings->uiMaxCategory,
						pstBookmarkSettings->uiMaxBookmark,
						pstBookmarkSettings->bFileMultiStorage, 
						&(g_pstJWapAppInfo->vBookMarkHandle)) ;

                 

				if(jdd_FSIsFileExist(NULL, pszBookMark) == E_FALSE)
				{
					ST_BOOKMARK_ATTRIBUTES 	stBookMarkAttributes ={0,};

					mmi_trace(12,"JDD_LOG: bookmark file doesnt exist");

					stBookMarkAttributes.pmTitle =(JC_CHAR*) L"wap.sina.com";
					stBookMarkAttributes.pmURL =(JC_CHAR*)L"http://wap.sina.com";
					rCode = jdi_BookmarkInsertItem(g_pstJWapAppInfo->vBookMarkHandle, \
								&stBookMarkAttributes, NULL);
					mmi_trace(12,"JDD_LOG: 1 jdi_BookmarkInsertItem ret %d", rCode);

					stBookMarkAttributes.pmTitle =(JC_CHAR*) L"wap.baidu.com";
					stBookMarkAttributes.pmURL =(JC_CHAR*)L"http://wap.baidu.com";
					rCode = jdi_BookmarkInsertItem(g_pstJWapAppInfo->vBookMarkHandle, \
								&stBookMarkAttributes, NULL);
					mmi_trace(12,"JDD_LOG: 2 jdi_BookmarkInsertItem ret %d", rCode);

					rCode = jdi_BookmarkSaveFavourites (g_pstJWapAppInfo->vBookMarkHandle) ; // persistant saving
					mmi_trace(12,"JDD_LOG: jdi_BookmarkSaveFavourites ret %d", rCode);

				}
					
				mmi_trace (1, "@@@ jWapAddBookSaveTitle after core api %d @@@",rCode) ;
				jdd_MemFree(pstBookmarkSettings->pmBookmarkFileName) ;
			}
			jdd_MemFree (pstBookmarkSettings) ;
		}

		jdd_MemFree (pszBookMark) ;
	}
	else
	{
		mmi_trace (1, "@@@ jWapAddBookSaveTitle before core api rcode is %d @@@",rCode) ;
	}
}

void jwap_SaveBMRSKCallBack()
{
	mmi_trace (1, "JDC_LOG: BACK fun called");
	jWap_freeBookmarkInfo () ;
	GoBackHistory () ;
}

void jwap_PrepareBMLaunch(JC_CHAR	 *pmPushUrl)
{
	if (pstBmAddOptInfo)
	{
		jdd_MemFree (pstBmAddOptInfo) ;
		pstBmAddOptInfo = JC_NULL ;
	}

	pstBmAddOptInfo = jdd_MemAlloc(1, sizeof(ST_ADD_BM_INFO)) ;
	pstBmAddOptInfo->s8Title = jdd_MemAlloc (sizeof (JC_CHAR), BM_TITLE_LEN + 1) ;
	pstBmAddOptInfo->s8Url = jdd_MemAlloc (sizeof (JC_CHAR), jWAP_MAX_INPUT_SIZE + 1) ;

	if(pmPushUrl)
		jc_wstrncpy(pstBmAddOptInfo->s8Url, pmPushUrl, jWAP_MAX_INPUT_SIZE);
	
	jWap_addEditBm_entry_handler () ;
}

#endif

