 /***************************************************************************
 *
 * File Name : jwap_mmi_cb.c
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
 * @file		jwap_mmi_cb.c
 * @ingroup	jWAP
 * @brief	
 *
 **/

#define MODULE_NAME "JWAP"
#define FILE_NAME "jwap_mmi_cb.c"

#ifdef JATAAYU_SUPPORT 
/***************************************************************************
* System Include Files
**************************************************************************/
#include "commonscreens.h"
#include "filemgr.h"
#include "filemanagerdef.h"
#include "fmt_def.h"
#include "satgprots.h"
/***************************************************************************
* User Include Files
**************************************************************************/

#include "jwap_inc.h"


 
#include "jddex_platform.h"
#include "jdi_cutils_tcsstr.h" 
#include "jdd_netdatatype.h"
#include "jdd_netapi.h"
#include "jwap_history.h"
#include "jwap_httpdownload.h"
#include "jwap_push.h"

#include "jmms_inc.h"
#include "jmms_cb.h"
#include "jmms_interface.h"
#include "jmms_communicator.h"
#include "jmms_utils.h"

#include "custom_nvram_editor_data_item.h"
#include "dataaccountgprot.h"
#include "jprofilehandlertypes.h"
#include "jprofilehandlerprots.h"

extern void mmi_show_qq_select_confirm(void (*default_entry) (void));

/***************************************************************************
* Macros
**************************************************************************/

typedef enum _jwap_option_menu_items
{	
	E_OPT_NAVIGATION,
	E_OPT_HOMEPAGE,
	E_OPT_FETCH_URL,
	E_OPT_RECENT_PAGES,
	E_OPT_BOOKMARKS,
	E_OPT_SAVE_PAGE,
	E_OPT_SAVE_IMAGE,
	E_OPT_SHOW_URL,
	E_OPT_SET_AS_HOMEPAGE,
	E_OPT_ADD_BOOKMARKS,
	E_OPT_SETTINGS,
	E_OPT_SERVICE_INBOX,
	E_OPT_QUIT,
	E_OPT_MENU_COUNT
}JWAP_OPTION_MENU_ITEMS ;

typedef enum _jwap_nav_menu_items
{
	E_NAV_BACK,
	E_NAV_NEXT,
	E_NAV_REFRESH,
	E_NAV_MENU_COUNT,
	
}JWAP_NAV_MENU_ITEMS ;

typedef enum _jwap_main_menu_items
{
	E_MAIN_WAP_HOMEPAGE, 
	E_MAIN_WAP_BOOKMARKS,
	E_MAIN_WAP_RECENT,
	E_MAIN_WAP_SAVED_PAGES,
	E_MAIN_WAP_FETCH_URL,
#ifdef JWAP_PUSH_ENABLE	
	E_MAIN_SERVICE_INBOX,
#endif	
	E_MAIN_WAP_SETTINGS,	
	E_MAIN_WAP_ABOUT_US,
	E_MAIN_MENU_COUNT
}JWAP_MAIN_MENU_ITEMS ;

typedef enum _jwap_offline_pages_menu_items
{
	E_OP_GO,
	E_OP_DELETE,
	E_OP_DELETE_ALL,
	E_OP_SAVE_PAGE,
	E_OP_MENU_COUNT
}JWAP_OFFLINE_PAGES_MENU_ITEMS ;

/***************************************************************************
* Global variables declarations
**************************************************************************/
static	void (*sWAPOptionsLSKHandlers [E_OPT_MENU_COUNT])(void) ;
extern void GBSetInputboxLSKFunction(FuncPtr f);
static U16	g_file_count;
static JC_UINT32 g_FolderItemIndex;
static U16 g_OfflinePageCount = 0;

/***************************************************************************
* Local Function Prototypes
***************************************************************************/
void jWap_homePage_entry_handler () ;
void jWap_fetchUrl_entry_handler () ;

/***************************************************************************
* All Local Function Definitions
***************************************************************************/

/***************************************************************************
* All Global Function Definitions
**************************************************************************/
void jWap_keyUpArrHandler ()
{
	jWap_Generic_Key_Handler (KEY_UP_ARROW) ;
}

void jWap_keyDownArrHandler ()
{
	jWap_Generic_Key_Handler (KEY_DOWN_ARROW) ;
}

void jWap_keyLeftArrHandler ()
{
	jWap_Generic_Key_Handler (KEY_LEFT_ARROW) ;
}

void jWap_keyRightArrHandler ()
{
	jWap_Generic_Key_Handler (KEY_RIGHT_ARROW) ;
}

void jWap_keyEnterHandler ()
{
	jWap_Generic_Key_Handler (KEY_ENTER) ;
}

void jWap_keyEndHandler (void)
{
	#ifdef __MMI_MULTI_SIM__
	mmi_trace (1, "JDD_LOG: keyEndHandler[%d] simid[%d]",
		g_pstJWapAppInfo->stWapState.eTrConnState, g_pstJWapAppInfo->original_simid) ;
	#else
	mmi_trace (1, "JDD_LOG: keyEndHandler[%d]",
		g_pstJWapAppInfo->stWapState.eTrConnState) ;
	#endif
	if (g_pstJWapAppInfo->pmPushURL)
	{
		if (E_WAP_LAUNCH_URL == g_pstJWapAppInfo->eWapLaunchMode)
		{
			jdd_MemFree (g_pstJWapAppInfo->pmPushURL) ;
		}
		else
		{
			/*	Whenever push url is fetched, it has to be fetched with the specified SIM,
				so pmPushURL is made not null to identifiy this case */
			g_pstJWapAppInfo->pmPushURL = JC_NULL ;
		}
	}
	#ifdef __MMI_MULTI_SIM__
	g_pstJWapAppInfo->simid = g_pstJWapAppInfo->original_simid ;
	#endif
	g_pstJWapAppInfo->stWapState.eWapExitKeyPressed = E_WAP_EXIT_KEY_END ;
	g_pstJWapAppInfo->eWapLaunchMode = E_WAP_LAUNCH_NORMAL ;
	jwap_ApplicationExitHandler();
}

void jWap_priMmi_cancel_handler ()
{
	JC_RETCODE				rCode ;
	ST_BE_PAGE_CONTENT	stPageCont = {{0,}, } ;
	rCode = jWap_stop () ;
	if (JC_OK == rCode)
	{
		rCode = jdi_BEGetPageContent (g_pstJWapAppInfo->hBECore, &stPageCont) ;
		if (JC_OK != rCode)
		{
			/* There is no content displayed on Primary MMI */
			jWap_options_quit_handler () ;
		}
		else if (!jWap_isBack ())
		{
			/* This is the first page. Remove LSK */	
			ClearKeyHandler (KEY_RSK, KEY_EVENT_DOWN) ;
			ChangeRightSoftkey (0, 0) ;

			ChangeLeftSoftkey (STR_GLOBAL_OPTIONS, 0) ;

			if(0 == jc_wstrcmp(g_pstJWapAppInfo->pszTitleString, \
						(JC_CHAR*)GetString(STR_ID_WAP_CONNECTING)) )
			{
				jWap_setConnectingTitle(NULL) ;
			}
			else
			{
				jWap_setConnectingTitle(g_pstJWapAppInfo->pszTitleString);
			}
			
			SetLeftSoftkeyFunction (jWap_options_handler, KEY_EVENT_UP) ;
		}
		else
		{
			/* There are more than 1 page. Set to "Back"  */
			ChangeRightSoftkey (STR_GLOBAL_BACK, 0) ;	
			SetRightSoftkeyFunction (jWap_back, KEY_EVENT_UP);

			ChangeLeftSoftkey (STR_GLOBAL_OPTIONS, 0) ;
			SetLeftSoftkeyFunction (jWap_options_handler, KEY_EVENT_UP) ;
		}
	}
	else
	{
		/* Disable Soft Keys when cancelling is in progress */
		mmi_trace (1, "JDD_LOG: jWap_priMmi_cancel_handler >> jWap_stop () returns rCode [%X]",
				-rCode) ;
		ClearKeyHandler (KEY_LSK, KEY_EVENT_DOWN) ;
		ChangeLeftSoftkey (0, 0) ;
		ClearKeyHandler (KEY_RSK, KEY_EVENT_DOWN) ;
		ChangeRightSoftkey (0, 0) ;
	}
}

void jWap_exitOptionsScreen (void)
{
	mmi_trace(TRUE,"JDD_LOG: Func: %s eWapExitKeyPressed = %d",
		__FUNCTION__, g_pstJWapAppInfo->stWapState.eWapExitKeyPressed);
	DeleteUptoScrID(SCR_ID_WAP_MAIN_SCREEN) ;
	GoBackHistory () ;
	g_pstJWapAppInfo->stWapState.eWapExitKeyPressed = E_WAP_EXIT_KEY_NONE ;
}
void jWap_options_quit_handler ()
{
	mmi_trace (1, "JDD_LOG: quit_handler[%d]", g_pstJWapAppInfo->eWapLaunchMode) ;
	if (E_WAP_LAUNCH_NORMAL == g_pstJWapAppInfo->eWapLaunchMode)
	{
		/* If authorization info, security info is present, need to be freed here */
		jWap_showDisconnectingScreen () ;
		if (g_pstJWapAppInfo->pmPushURL)
		{
			/*	Whenever push url is fetched, it has to be fetched with the specified SIM,
				so pmPushURL is made not null to identifiy this case in E_WAP_LAUNCH_NORMAL */
			g_pstJWapAppInfo->pmPushURL = JC_NULL ;

			/*	Current implementation is, the sim id used to fetch the push url
				will be used for subsequent fetched, untill the user selects the
				different sim or come out of WAP */
			/*
			g_pstJWapAppInfo->bIsDCAvailable = E_FALSE ;
			g_pstJWapAppInfo->simid = g_pstJWapAppInfo->original_simid ;
			*/
		}
		jWap_freeFetchInfo (g_pstJWapAppInfo->pstFetchReq) ;
		g_pstJWapAppInfo->pstFetchReq = JC_NULL ;
		g_pstJWapAppInfo->stWapState.eWapExitKeyPressed = E_WAP_EXIT_KEY_QUIT ;
		if (E_WAP_BEGIN_DISPLAY_RECVD == jWAP_GET_STATE())
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s Wait for the end page listner",
				__FUNCTION__) ;
			/* Wait for the end page listner */
		}
		else
		{
			jWap_deInit_priMmi () ;
			jWap_exitOptionsScreen () ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_options_quit_handler >> The launch was from Idle") ;
		jWap_keyEndHandler () ;
	}
}

void jWap_authUsrNameCancel ()
{
	jWap_freeFetchInfo (g_pstJWapAppInfo->pstFetchReq) ;
	g_pstJWapAppInfo->pstFetchReq = JC_NULL ;
	GoBackHistory () ;	
}

void jWap_authPwdDone ()
{
	DeleteNHistory(1);
	GoBackHistory () ;
	jWap_setConnectingTitle((JC_CHAR*)GetString(STR_ID_WAP_CONNECTING)) ;
	jWap_send_fetch_request (JC_NULL) ;
	jWap_freeFetchInfo (g_pstJWapAppInfo->pstFetchReq) ;
	g_pstJWapAppInfo->pstFetchReq = JC_NULL ;
}

void jWap_showAuthPwdScreen ()
{
	S16 required_input_modes[] = {
								INPUT_MODE_MULTITAP_UPPERCASE_ABC ,
								INPUT_MODE_MULTITAP_LOWERCASE_ABC,
								INPUT_MODE_123,
								-1
								} ;
	U8* guiBuffer ; 
	mmi_trace (1, "@@@ jWap_showAuthUserNameScreen @@@") ;	
	EntryNewScreen (SCR_ID_WAP_AUTH_PASSWORD, NULL, jWap_showAuthPwdScreen, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_AUTH_PASSWORD) ;
	ShowCategory5Screen_ext(
		STR_ID_WAP_PASSWORD, 0,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_CANCEL, IMG_GLOBAL_BACK,
		INPUT_TYPE_ALPHANUMERIC_PASSWORD,
		(U8*)g_pstJWapAppInfo->pstFetchReq->pstAuthInfo->amPassword, MAX_AUTH_PASSWORD_SIZE,
		guiBuffer,0, required_input_modes, NULL);
	
#ifdef __MMI_GB__
	GBSetInputboxLSKFunction (jWap_authPwdDone);
#endif
	SetLeftSoftkeyFunction (jWap_authPwdDone, KEY_EVENT_UP) ;
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;
}

extern BOOL isInWapApp(void);
void jWap_showAuthUserNameScreen ()
{
	S16 required_input_modes[] = {
								INPUT_MODE_MULTITAP_UPPERCASE_ABC ,
								INPUT_MODE_MULTITAP_LOWERCASE_ABC,
								INPUT_MODE_123,
								-1
								} ;
	U8* guiBuffer ; 
	mmi_trace (1, "@@@ jWap_showAuthUserNameScreen @@@") ;
	//isInWapApp should be used by other APP,temply usded here
	if(!isInWapApp())
	{
		GoBackHistory();
		return;
	}
	EntryNewScreen (SCR_ID_WAP_AUTH_USERNAME, NULL, jWap_showAuthUserNameScreen, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_AUTH_USERNAME) ;

	ShowCategory5Screen_ext(STR_ID_WAP_USER_NAME,
		0,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_CANCEL, IMG_GLOBAL_BACK,
		INPUT_TYPE_ALPHANUMERIC_UPPERCASE|INPUT_TYPE_ALPHANUMERIC_LOWERCASE|INPUT_TYPE_NUMERIC,
		(U8*)g_pstJWapAppInfo->pstFetchReq->pstAuthInfo->amName, MAX_AUTH_NAME_SIZE,
		guiBuffer,0, required_input_modes, NULL);

#ifdef __MMI_GB__
	GBSetInputboxLSKFunction (jWap_showAuthPwdScreen);
#endif
	SetLeftSoftkeyFunction (jWap_showAuthPwdScreen, KEY_EVENT_UP) ;
	SetCategory5RightSoftkeyFunction (jWap_authUsrNameCancel, KEY_EVENT_UP) ;	
}

void jWap_optoins_refresh_handler ()
{
	INT	iErr ;
	mmi_trace (1, "@@@ jWap_optoins_refresh_handler @@@") ;
	
	DeleteScreenIfPresent(SCR_ID_WAP_OPTIONS) ;
	GoBackHistory () ;
	ChangeRightSoftkey (0, 0) ;
	ChangeLeftSoftkey (0, 0) ;
	
	SetRightSoftkeyFunction (0, KEY_EVENT_UP) ;
	SetLeftSoftkeyFunction (0, KEY_EVENT_UP) ;
	
	jWap_setConnectingTitle ((JC_CHAR*)GetString(STR_ID_WAP_CONNECTING) ) ;
	iErr =	jWap_refresh () ;
}

void jWap_optNavigation_next_handler ()
{
	if (jWap_isNext ())
	{
		DeleteScreenIfPresent(SCR_ID_WAP_OPTIONS) ;
		GoBackHistory () ;
		ChangeRightSoftkey (0, 0) ;
		ChangeLeftSoftkey (0, 0) ;
		
		SetRightSoftkeyFunction (0, KEY_EVENT_UP) ;
		SetLeftSoftkeyFunction (0, KEY_EVENT_UP) ;
		jWap_next () ;
	}
}

void jWap_setConnectingTitle (JC_CHAR *pszTitile)
{
	jWap_titleHandler (pszTitile) ;
}

void jWap_options_highlight_handler (int iSelectIndex) 
{
	mmi_trace (1, "JDD_LOG: jWap_options_highlight_handler >> iSelectIndex is [%d]", iSelectIndex) ;
	SetLeftSoftkeyFunction (*sWAPOptionsLSKHandlers [iSelectIndex], KEY_EVENT_UP) ;
}

void jWap_optoins_navigation_handler()
{
	U16 item_texts[E_NAV_MENU_COUNT] = {0, } ;
	INT iIndex = 0 ;
	
	item_texts [iIndex++] = STR_ID_WAP_OPTIONS_REFRESH ;
	if(jWap_isBack ())
		{
			item_texts [iIndex++] = STR_GLOBAL_BACK ;
		}
	if (jWap_isNext ())
		{
			item_texts [iIndex++] = STR_ID_WAP_OPTIONS_NEXT ;
		}	

	mmi_trace (1, "jwap_log: optoins_navigation_handler") ;

	EntryNewScreen (SCR_ID_WAP_OPTIONS, NULL, jWap_optoins_navigation_handler, NULL) ;
	RegisterHighlightHandler (jWap_options_navigation_highlight_handler) ;
	ShowCategory15Screen (STR_ID_WAP_TEXT, IMG_ID_JATAAYU_WAP_ICON, 
		STR_GLOBAL_OK, 0, 
		STR_GLOBAL_BACK, 0,
		iIndex, item_texts,(U16 *)gIndexIconsImageList, 
		1, 0, NULL) ;

	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;
}

void jWap_options_navigation_highlight_handler(JC_INT32 iIndex)
{				
	switch(iIndex)
	{
		case 0:
			{
			SetLeftSoftkeyFunction (jWap_optoins_refresh_handler ,KEY_EVENT_UP) ;
			}
			break ;
		case 1:
			{
				if(jWap_isBack ())
				{
					SetLeftSoftkeyFunction (jWap_optNavigation_back_handler,KEY_EVENT_UP);
				}
				else
				{
					SetLeftSoftkeyFunction (jWap_optNavigation_next_handler,KEY_EVENT_UP);
				}
			}
			break ;			
		case 2:
			{
				SetLeftSoftkeyFunction (jWap_optNavigation_next_handler, KEY_EVENT_UP) ;
			}
			break ;
		default:
			break ;
	}
}

void jWap_PageinputBox_highlightHanlder (S32 index)
{
	switch(index)
	{
		case 0:
			if (E_WAP_EDIT_IMAGE_FILE_NAME == g_pstJWapAppInfo->stWapState.eEditorState)
			{
				SetLeftSoftkeyFunction (jWap_SaveImage, KEY_EVENT_UP ) ;
			}
			else
			{
				SetLeftSoftkeyFunction (jWap_SavePage, KEY_EVENT_UP ) ;
			}
			break;
		case 1:
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP ) ;
			break ;
	}
	
}


void jWap_PageinputBoxLskHandler ()
{
	U16 item_texts[] = {STR_GLOBAL_DONE,
						//STR_GLOBAL_CANCEL,
						STR_GLOBAL_INPUT_METHOD} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, \
						//IMG_GLOBAL_L3
						} ;
	EntryNewScreen (SCR_ID_WIDGET_INPUTBOX_OPTION, 0, 0, 0) ;
	SetParentHandler (SCR_ID_WAP_INPUT_BOX_BK) ;
	RegisterHighlightHandler (jWap_PageinputBox_highlightHanlder);

	ShowCategory52Screen (STR_GLOBAL_OPTIONS, 0, 
		STR_GLOBAL_OK, 0,
		STR_GLOBAL_BACK, 0, 
		2, item_texts, item_icons,
		0, 0, 0, 0);
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP ) ;
}


void jWap_getPageFilenameFromUser (void)
{
	U8* guiBuffer = JC_NULL;
	
	EntryNewScreen (SCR_ID_WAP_INPUT_BOX_BK, JC_NULL, \
		jWap_getPageFilenameFromUser, JC_NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_INPUT_BOX_BK);		
	SetParentHandler (SCR_ID_WAP_OPTIONS) ;

	ShowCategory5Screen (STR_ID_WAP_HTTPDWN_FILENAME, 0,
		STR_GLOBAL_OPTIONS, 0,
		STR_GLOBAL_BACK, 0,
		INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
		(U8*)jWap_getInputBuff(), JDOWNLOAD_INPUTBOX_FILENAME_MAX, 
		guiBuffer);
	
#ifdef __MMI_GB__
		GBSetInputboxLSKFunction (jWap_PageinputBoxLskHandler);
#endif

	SetLeftSoftkeyFunction (jWap_PageinputBoxLskHandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistory );
}	

void jWap_SaveImage (void)
{
	JC_CHAR				*pmFileName = (JC_CHAR*)jWap_getInputBuff () ;
	JC_RETCODE 			rCode = JC_OK;
	ST_BE_URL_CONTENT	stImageInfo = {0, } ;
	JC_CHAR				*pmFolder = JC_NULL ;
	JC_CHAR				*pmAbsoluteFilePath = JC_NULL ;
	JC_UINT32			uiFilePathLen ;
		
	if (JC_NULL != g_pstJWapAppInfo->hBECore && JC_NULL != pmFileName)
	{
		if(0 == jc_wstrlen (pmFileName) || '.' == *pmFileName)
		{
			rCode = JC_ERR_FILE_NOT_FOUND ;
		}
		else
		{
			rCode = jdi_CUtilsCharToTchar (JC_NULL, CS_WAP_DOWLOAD_PATH, &pmFolder) ;
			if (JC_OK == rCode)
			{
				if (E_FALSE == jdd_FSIsDirExist (JC_NULL, pmFolder))
				{
					rCode = jdd_FSMakeDir (JC_NULL, pmFolder) ;
				}
			}
		}		
		if (JC_OK == rCode)
		{
			uiFilePathLen = jc_wstrlen (pmFolder) + jc_strlen (CFG_PATH_SEPARATOR) +
					jc_wstrlen (pmFileName) + jc_wstrlen (g_pstJWapAppInfo->amFileExt) ;
			pmAbsoluteFilePath = (JC_CHAR *)jdd_MemAlloc (sizeof (JC_CHAR), uiFilePathLen + 1) ;
			if (pmAbsoluteFilePath)
			{
				jc_wstrcpy (pmAbsoluteFilePath, pmFolder) ;
				jdi_CUtilsTcsStrCat (CFG_PATH_SEPARATOR, pmAbsoluteFilePath) ;
				jc_wstrcat (pmAbsoluteFilePath, pmFileName) ;
				if ('\0' != *g_pstJWapAppInfo->amFileExt)
				{
					jc_wstrcat (pmAbsoluteFilePath, g_pstJWapAppInfo->amFileExt) ;
				}
				else
				{
					/* Extention will be empty */
				}
			}
			else
			{
				rCode = JC_ERR_MEMORY_ALLOCATION ;
			}
			jdd_MemFree (pmFolder) ;
		}
		if (JC_OK == rCode)
		{
			JC_INT8 *pcURI ;
			if (jdd_FSIsFileExist (JC_NULL, pmAbsoluteFilePath))
			{
				rCode = JC_ERR_FILE_DIR_ALREADY_EXIST ;
			}
			else
			{
				rCode = jdi_BEGetFocusedControlMimeInfo (g_pstJWapAppInfo->hBECore, &stImageInfo) ;
				if (JC_OK == rCode && 0 < stImageInfo.uiBufLen)
				{
					if (E_FALSE == stImageInfo.bIsFileContent)
					{
						rCode = jWap_SaveToFile (pmAbsoluteFilePath, stImageInfo.uiBufLen, stImageInfo.uBuffer.pucBuffer) ;
					}
					else
					{
						rCode = jWap_CopyToFile (stImageInfo.uBuffer.pmFileName, pmAbsoluteFilePath) ;
					}
				}
				jdi_CUtilsTcharToChar (JC_NULL, pmAbsoluteFilePath, &pcURI) ;
				mmi_trace(1, "JDD_LOG: jWap_SaveImage (%s %X %d)", pcURI, -rCode, stImageInfo.bIsFileContent) ;				
				jdd_MemFree (pcURI) ;
			}			
		}		
    }
	else
	{
		rCode = JC_ERR_INVALID_PARAMETER ;
	}
	jdd_MemFree (pmAbsoluteFilePath) ;
	if (JC_OK != rCode)
	{
		if (JC_ERR_FILE_DIR_ALREADY_EXIST == rCode)
		{
			GoBackToHistory (SCR_ID_WAP_INPUT_BOX_BK) ;
			jWap_showErrorPopup (rCode) ;
		}
		else
		{
			jc_memset (pmFileName, 0, (jWAP_MAX_INPUT_SIZE + 1) * sizeof (JC_CHAR)) ;
			GoBackToHistory (SCR_ID_WAP_HOMEPAGE_SCREEN) ;
			DisplayPopup ((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
	}
	else
	{
		jc_memset (pmFileName, 0, (jWAP_MAX_INPUT_SIZE + 1) * sizeof (JC_CHAR)) ;
		GoBackToHistory (SCR_ID_WAP_HOMEPAGE_SCREEN) ;
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
	}
}

void jWap_SavePage (void)
{
	JC_CHAR			*pmFileName = (JC_CHAR*)jWap_getInputBuff () ;
	JC_RETCODE 		rCode = JC_OK;
	ST_PPP_SAVEPAGE stSavePage = {E_SAVEPAGE_MULTIPART, } ;
	JC_CHAR			*pmFolder ;
	JC_CHAR			*pmAbsoluteFilePath = JC_NULL ;
	JC_UINT32		uiFilePathLen ;
	JC_INT8 		*pcURI = JC_NULL;
	JC_UINT16 		uhSpecialCharIndex = 0;
		
	if (JC_NULL != g_pstJWapAppInfo->hBECore && JC_NULL != pmFileName)
	{
		if((0==jc_wstrlen(pmFileName)) || (E_FALSE == jWap_IsValidFileName(pmFileName, &uhSpecialCharIndex)))
		{
			rCode = JC_ERR_INVALID_PARAMETER ;
		}
		else
		{
			rCode = jdi_CUtilsCharToTchar (JC_NULL, CS_WAP_SAVED_PAGE_PATH, &pmFolder) ;
			if (JC_OK == rCode)
			{
				if (E_FALSE == jdd_FSIsDirExist (JC_NULL, pmFolder))
				{
					rCode = jdd_FSMakeDir (JC_NULL, pmFolder) ;
				}
			}
		}
		if (JC_OK == rCode)
		{
			uiFilePathLen = jc_wstrlen (pmFolder) + jc_strlen (CFG_PATH_SEPARATOR) +
								jc_wstrlen (pmFileName) + jc_strlen (JWAP_SAVE_PAGE_FILE_EXT) ;
			pmAbsoluteFilePath = (JC_CHAR *)jdd_MemAlloc (sizeof (JC_CHAR), uiFilePathLen + 1) ;
			if (pmAbsoluteFilePath)
			{
				jc_wstrcpy (pmAbsoluteFilePath, pmFolder) ;
				jdi_CUtilsTcsStrCat (CFG_PATH_SEPARATOR, pmAbsoluteFilePath) ;
				jc_wstrcat (pmAbsoluteFilePath, pmFileName) ;
				if ('\0' != *g_pstJWapAppInfo->amFileExt)
				{
					jc_wstrcat (pmAbsoluteFilePath, g_pstJWapAppInfo->amFileExt) ;
				}
				else
				{
					/* Extention will be empty */
				}				
				if (jdd_FSIsFileExist (JC_NULL, pmAbsoluteFilePath))
				{
					rCode = JC_ERR_FILE_DIR_ALREADY_EXIST ;
				}
				else
				{
					stSavePage.eSaveType = E_SAVEPAGE_MULTIPART ;
				    stSavePage.pmFileName = pmAbsoluteFilePath ;
					rCode = jdi_BEExecuteReadPPP (g_pstJWapAppInfo->hBECore,
														E_PPP_SAVEPAGE,
														 (void *)&stSavePage) ;
					jdi_CUtilsTcharToChar (JC_NULL, pmAbsoluteFilePath, &pcURI) ;
					mmi_trace(1, "JDD_LOG: jWap_SavePage (%s %X)", pcURI, -rCode) ;
					jdd_MemFree (pcURI) ;				
				}
			}
			else
			{
				rCode = JC_ERR_MEMORY_ALLOCATION ;
			}
			jdd_MemFree (pmFolder) ;
		}		
    }
	else
	{
		rCode = JC_ERR_INVALID_PARAMETER ;
	}
	mmi_trace(1, "JDD_LOG: jWap_SavePage rCode[%x]", -rCode) ;
	jdd_MemFree (pmAbsoluteFilePath) ;
	if (JC_OK != rCode)
	{
		if (JC_ERR_FILE_DIR_ALREADY_EXIST == rCode)
		{
			GoBackToHistory (SCR_ID_WAP_INPUT_BOX_BK) ;
			jWap_showErrorPopup (rCode) ;
		}
		else if(JC_ERR_INVALID_PARAMETER == rCode) 
		{
			GoBackToHistory (SCR_ID_WAP_INPUT_BOX_BK) ;
			DisplayPopup ((PU8)GetString(STR_ID_WAP_INVALID_INPUT), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
		else
		{
			jc_memset (pmFileName, 0, (jWAP_MAX_INPUT_SIZE + 1) * sizeof (JC_CHAR)) ;
			GoBackToHistory (SCR_ID_WAP_HOMEPAGE_SCREEN) ;
			DisplayPopup ((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		}
	}
	else
	{
		jc_memset (pmFileName, 0, (jWAP_MAX_INPUT_SIZE + 1) * sizeof (JC_CHAR)) ;
		DeleteNHistory(2);
		GoBackHistory();
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, 2000, 1) ;
	}
}

void jWap_options_SaveImage_Handler (void)
{
	ST_BE_URL_CONTENT	stImageInfo = {0, } ;
	JC_RETCODE			rCode ;
	const JC_INT8			*pcExtention = JC_NULL ;
	extern BOOL 		g_hasMountedTFlashFS ;
		
	mmi_trace(1,"jdd_log: SaveImage entry cardinfo[%d]", g_hasMountedTFlashFS);
	
	if (FALSE == g_hasMountedTFlashFS)
	{
		DisplayPopup(GetString (STR_ID_WAP_NO_MEMORY_CARD),
							IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		return;
	}

	rCode = jdi_BEGetFocusedControlMimeInfo (g_pstJWapAppInfo->hBECore, &stImageInfo) ;
	if (JC_OK == rCode && stImageInfo.pcContentType)
	{
		jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, (jWAP_MAX_INPUT_SIZE + 1) * sizeof (JC_CHAR)) ;
		jc_memset (g_pstJWapAppInfo->amFileExt, 0, JWAP_MAX_FILE_EXT_LEN * sizeof (JC_CHAR)) ;
		jWap_GetFileNameFromURL (stImageInfo.pmURI, (JC_CHAR *)g_pstJWapAppInfo->pcInputBuff, \
								(JDOWNLOAD_INPUTBOX_FILENAME_MAX- 1)) ;
		jdd_MiscGetExtForContentType (stImageInfo.pcContentType, &pcExtention) ;
		if (pcExtention)
		{
			jdi_CUtilsCharToTcharNMem (pcExtention, jc_strlen (pcExtention),
				JWAP_MAX_FILE_EXT_LEN - 1, g_pstJWapAppInfo->amFileExt) ;
		}
		else
		{
			/* File will be saved without any extention */
		}
		g_pstJWapAppInfo->stWapState.eEditorState = E_WAP_EDIT_IMAGE_FILE_NAME ;
		jWap_getPageFilenameFromUser () ;
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_options_SaveImage_Handler >> jdi_BEGetFocusedControlMimeInfo [%X]", -rCode) ;
	}
}

void jWap_options_OfflinePages_ExitHandler()
{
	mmi_trace(1,"jdd_log: OfflinePages_ExitHandler called");
}

void jWap_options_OfflinePages_HltHandler(int iSelectIndex)
{
	mmi_trace(1,"jwap_log: get_highlited_id [%d]", iSelectIndex);
	g_FolderItemIndex = iSelectIndex;
}

void jwap_OptionSavedPagesHltHandler (int iSelectIndex)
{
	mmi_trace (1, "jwap_log: SavedPagesHltHandler Index [%d]", iSelectIndex) ;
	switch (iSelectIndex)
	{
		case 0:
		{
			if(0 < g_OfflinePageCount)	
				SetLeftSoftkeyFunction (jwap_ViewSavedPages,KEY_EVENT_UP) ;
			else
				SetLeftSoftkeyFunction (jWap_options_SavePage_Handler,KEY_EVENT_UP) ;	
		}
			break ;
		case 1:
			SetLeftSoftkeyFunction (jwap_DeleteSavedPagesEntryHandler ,KEY_EVENT_UP) ;
			break ;
		case 2:
			SetLeftSoftkeyFunction (jwap_DeleteAllSavedPagesEntryHandler ,KEY_EVENT_UP) ;
			break ;
		case 3:
			SetLeftSoftkeyFunction (jWap_options_SavePage_Handler,KEY_EVENT_UP) ;
			break ;	
		default:
			break ;
	}
}

void jWap_OptionOfflinePages()
{
	U8* guiBuffer = NULL;
	U16 item_texts[E_OP_MENU_COUNT] = {0, } ;
	INT iIndex = 0 ;

	mmi_trace(1,"jdd_log: OptionOfflinePages no[%d]", g_OfflinePageCount);
	if(0 < g_OfflinePageCount)
	{
		item_texts [iIndex++] = STR_ID_WAP_VIEW ;
		item_texts [iIndex++] = STR_GLOBAL_DELETE ;
		item_texts [iIndex++] = STR_GLOBAL_DELETE_ALL ;
	}
	item_texts [iIndex++] = STR_ID_WAP_OPTIONS_SAVEPAGE ;
	
	EntryNewScreen (SCR_ID_SAVED_PAGES_OPTION_SRCEEN, NULL, \
						jWap_OptionOfflinePages, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_SAVED_PAGES_OPTION_SRCEEN) ;

	RegisterHighlightHandler (jwap_OptionSavedPagesHltHandler) ;
	ShowCategory15Screen (STR_ID_WAP_SAVED_PAGES, IMG_ID_JATAAYU_WAP_ICON,
					 STR_GLOBAL_OK, 0,
					 STR_GLOBAL_BACK, 0,
					 iIndex, item_texts,
					 (PU16)gIndexIconsImageList,
					 1, 0, guiBuffer);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}

void jWap_options_OfflinePages_EntryHandler()
{
	extern BOOL 	g_hasMountedTFlashFS ;
	U16				file_count = 0;
	U8				*guiBuffer = NULL;
		
	mmi_trace(1,"jdd_log: SavePage entry cardinfo[%d]", g_hasMountedTFlashFS);
	
	if (FALSE == g_hasMountedTFlashFS)
	{
		DisplayPopup(GetString (STR_ID_WAP_NO_MEMORY_CARD),
							IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		return;
	}	

	file_count = jwap_GetOfflinePageCountFromFolder();
	g_OfflinePageCount = file_count;
	
	EntryNewScreen (SCR_ID_WAP_OPT_OFFLINE_PAGES, \
					jWap_options_OfflinePages_ExitHandler, \
					jWap_options_OfflinePages_EntryHandler, NULL) ;
	
	RegisterHighlightHandler (jWap_options_OfflinePages_HltHandler) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_OPT_OFFLINE_PAGES) ;
		
	ShowCategory184Screen(STR_ID_WAP_SAVED_PAGES, IMG_ID_JATAAYU_WAP_ICON, \
						STR_GLOBAL_OPTIONS, 0, 
						STR_GLOBAL_BACK, 0, 
						file_count, 
						jwap_GetSavedpagesListItems, 
						0, 0, guiBuffer);
	SetLeftSoftkeyFunction(jWap_OptionOfflinePages, KEY_EVENT_UP);	
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);	
}

void jWap_options_SavePage_Handler (void)
{
	ST_BE_PAGE_INFO	stPageInfo ;
	JC_RETCODE		rCode ;
	JC_CHAR			*pmFileName = (JC_CHAR*)jWap_getInputBuff () ;
	extern BOOL 		g_hasMountedTFlashFS ;
		
	mmi_trace(1,"jdd_log: SavePage entry cardinfo[%d]", g_hasMountedTFlashFS);
	
	if (FALSE == g_hasMountedTFlashFS)
	{
		DisplayPopup(GetString (STR_ID_WAP_NO_MEMORY_CARD),
							IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		return;
	}
		
	jc_memset (&stPageInfo, 0, sizeof (ST_BE_PAGE_INFO)) ;
	jc_memset (g_pstJWapAppInfo->amFileExt, 0, JWAP_MAX_FILE_EXT_LEN * sizeof (JC_CHAR)) ;
	rCode = jdi_BEGetPageInfo (g_pstJWapAppInfo->hBECore, &stPageInfo) ;
	if (JC_OK == rCode)
	{
		jc_memset (pmFileName, 0, (jWAP_MAX_INPUT_SIZE + 1) * sizeof (JC_CHAR)) ;
		if (stPageInfo.pmTitle)
		{
			jc_tcsncpy (pmFileName, stPageInfo.pmTitle, JDOWNLOAD_INPUTBOX_FILENAME_MAX) ;
		}
		else
		{
			/* sina.cn - bug 14588   - Found that giving directly the url does not work. File open is failing. so need
				to change. May be . / : characters are not supported as filename by the platform */
			jc_tcsncpy (pmFileName, stPageInfo.pmURI, JDOWNLOAD_INPUTBOX_FILENAME_MAX) ;
		}
		jdi_CUtilsCharToTcharNMem (JWAP_SAVE_PAGE_FILE_EXT, jc_strlen (JWAP_SAVE_PAGE_FILE_EXT),
				JWAP_MAX_FILE_EXT_LEN - 1, g_pstJWapAppInfo->amFileExt) ;
		g_pstJWapAppInfo->stWapState.eEditorState = E_WAP_EDIT_PAGE_FILE_NAME ;
		jWap_getPageFilenameFromUser () ;
	}
	else
	{
		mmi_trace (1, "JDD_LOG: OfflinePages_Handler [%x]", -rCode) ;
	}
}

void jWap_options_add_bookmark_handler()
{
	ST_BE_PAGE_INFO		stPageInfo ;
	JC_RETCODE			rCode ;
	jc_memset (&stPageInfo, 0, sizeof (ST_BE_PAGE_INFO)) ;
	
    rCode = jdi_BEGetPageInfo (g_pstJWapAppInfo->hBECore, &stPageInfo) ; 
	
	if(JC_OK == rCode)
	{
		jWap_addEditBm_addInterface (stPageInfo.pmTitle, stPageInfo.pmURI) ;
	}
	else
	{
		DisplayPopup ((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, \
			 1000, 1); //Last argument is tone id. 1 = ERROR tone
	}

	
}

void jWap_optNavigation_back_handler ()
{
	if (jWap_isBack ())
	{
		DeleteScreenIfPresent(SCR_ID_WAP_OPTIONS) ;
		GoBackHistory () ;
		ChangeRightSoftkey (0, 0) ;
		ChangeLeftSoftkey (0, 0) ;
		
		SetRightSoftkeyFunction (0, KEY_EVENT_UP) ;
		SetLeftSoftkeyFunction (0, KEY_EVENT_UP) ;
		jWap_back () ;
	}
}

void jWap_options_handler ()
{	
	U8					*guiBuffer ; 
	JC_INT32			iItemIndex = 0 ;
	U16					stItemTexts [E_OPT_MENU_COUNT] = {0, } ;
	U16					stItemIcons [E_OPT_MENU_COUNT] ;
	ST_BE_PAGE_INFO	stPageInfo = {0, } ;
	ST_BE_URL_CONTENT	stImageInfo = {0, } ;
	ST_BE_NAVIGATION_INFO	stNavigationInfo = {0, } ;
	JC_RETCODE			rCode ;

	jc_memset (sWAPOptionsLSKHandlers, 0, sizeof (sWAPOptionsLSKHandlers)) ;
	if (g_pstJWapAppInfo->hBECore)
	{
		jdi_BEGetNavigationInfo (g_pstJWapAppInfo->hBECore, &stNavigationInfo) ;
		jdi_BEGetFocusedControlMimeInfo (g_pstJWapAppInfo->hBECore, &stImageInfo) ;
		rCode = jdi_BEGetPageInfo (g_pstJWapAppInfo->hBECore, &stPageInfo) ;
		/*	Check for enabling navigation option	*/
		if (JC_OK == rCode &&
			(E_TRUE == stNavigationInfo.bBackNavigation ||
			E_TRUE == stNavigationInfo.bForwardNavigation ||
			JC_NULL != stPageInfo.pmURI))
		{
			g_pstJWapAppInfo->uiNumOptItems = (JC_UINT32)(E_OPT_MENU_COUNT) ;

			stItemTexts [iItemIndex] = STR_ID_WAP_NAVIGATION ;
			stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
			sWAPOptionsLSKHandlers [iItemIndex++] = jWap_optoins_navigation_handler ;
		}
		else
		{
			g_pstJWapAppInfo->uiNumOptItems = (JC_UINT32)(E_OPT_MENU_COUNT - 1) ;
		}
		/*	Check for enabling save image option	*/
		if (JC_NULL == stImageInfo.pcContentType)
		{
			g_pstJWapAppInfo->uiNumOptItems-- ;
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jWap_options_handler >> ASSERT Case") ;
	}
	stItemTexts [iItemIndex] = STR_ID_WAP_HOMEPAGE ;
	stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
	sWAPOptionsLSKHandlers [iItemIndex++] = jWap_homePage_entry_handler ;

	stItemTexts [iItemIndex] = STR_ID_WAP_FETCH_URL ;
	stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
	sWAPOptionsLSKHandlers [iItemIndex++] = jWap_fetchUrl_entry_handler ;

	stItemTexts [iItemIndex] = STR_ID_WAP_BOOKMARKS ;
	stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
	sWAPOptionsLSKHandlers [iItemIndex++] = jWap_BookMark_entry_handler ;

	stItemTexts [iItemIndex] = STR_ID_WAP_RECENT ;
	stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
	sWAPOptionsLSKHandlers [iItemIndex++] = jWap_recentPages_entry_handler ;	

	if (stImageInfo.pcContentType)
	{
		stItemTexts [iItemIndex] = STR_ID_WAP_OPTIONS_SAVEIMAGE ;
		stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
		sWAPOptionsLSKHandlers [iItemIndex++] = jWap_options_SaveImage_Handler ;
	}
	if (stPageInfo.pmURI)
	{
		stItemTexts [iItemIndex] = STR_ID_WAP_SAVED_PAGES ;
		stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;

		sWAPOptionsLSKHandlers [iItemIndex++] = jWap_options_OfflinePages_EntryHandler ;
	
		//Show URL
		stItemTexts [iItemIndex] = STR_ID_VIEW_URL;
		stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
		sWAPOptionsLSKHandlers [iItemIndex++] = jWap_options_Show_URL ;

		//set as Homepage
		stItemTexts [iItemIndex] = STR_ID_SET_AS_HOMEPAGE_URL;
		stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
		sWAPOptionsLSKHandlers [iItemIndex++] = jWap_options_Set_URL_As_Homepage_EntryHandler ;
	
		stItemTexts [iItemIndex] = STR_ID_WAP_ADD_TO_BM ;
		stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
		sWAPOptionsLSKHandlers [iItemIndex++] = jWap_options_add_bookmark_handler ;
	}

#ifdef JWAP_PUSH_ENABLE	

	//Service inbox in option
	stItemTexts [iItemIndex] = STR_ID_WAP_SERVICE_INBOX;
	stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
	sWAPOptionsLSKHandlers [iItemIndex++] = jWap_push_entry_handler ;
#endif	

	//settings option
	stItemTexts [iItemIndex] = STR_ID_WAP_SETTINGS ;
	stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
	sWAPOptionsLSKHandlers [iItemIndex++] = jWap_settings_entry_handler ;

	stItemTexts [iItemIndex] = STR_GLOBAL_QUIT ;
	stItemIcons [iItemIndex] = gIndexIconsImageList [iItemIndex] ;
	sWAPOptionsLSKHandlers [iItemIndex++] = jWap_options_quit_handler ;

	/*	This screen is added to history. It will be deleted when before entering
		primary mmi. This is also deleted in primary mmi entry handler */
	EntryNewScreen (SCR_ID_WAP_OPTIONS, JC_NULL, jWap_options_handler, JC_NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_OPTIONS) ;
	RegisterHighlightHandler (jWap_options_highlight_handler) ;
	ShowCategory15Screen (STR_ID_WAP_TEXT, IMG_ID_JATAAYU_WAP_ICON, 
		STR_GLOBAL_OK, 0, 
		STR_GLOBAL_BACK, 0,
		iItemIndex,
		stItemTexts,
		(PU16)gIndexIconsImageList,
		1, 0, guiBuffer) ;

	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;
}

void jWap_priMmi_exit_handler ()
{        
	mmi_trace (1, "jwap_log: exit pri mmi called") ;
	g_pstJWapAppInfo->bIsPriMmiActive = E_FALSE ;
}

void jWap_set_priMmiScreen ()
{
	U8* guiBuffer; 
	INT iErr ;
	JC_INT8	*pctst ;

	mmi_trace (1, "JDD_LOG: jWap_set_priMmiScreen () Entry") ;
	if (IsScreenPresent (SCR_ID_WAP_OPTIONS))
		DeleteScreenIfPresent(SCR_ID_WAP_OPTIONS) ;
	jWap_init_priMmi () ;
	g_pstJWapAppInfo->bIsPriMmiActive = E_TRUE ;
	EntryNewScreen (SCR_ID_WAP_HOMEPAGE_SCREEN, jWap_priMmi_exit_handler, \
		jWap_set_priMmiScreen, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_HOMEPAGE_SCREEN) ;
	SetParentHandler (SCR_ID_WAP_MAIN_SCREEN) ;

	ShowCategory7Screen (0, IMG_ID_JATAAYU_WAP_END_ANIMATION_GIF,
		0, 0, 
		0, 0, 0, guiBuffer) ;

	if (g_pstJWapAppInfo->pszTitleString)
	{
		jdi_CUtilsTcharToChar(NULL,g_pstJWapAppInfo->pszTitleString,&pctst) ;
		if (pctst)
		{
			mmi_trace (1, "JDD_LOG: jWap_set_priMmiScreen >> jWap_set title [%s]",pctst) ;
			jdd_MemFree (pctst) ;
		}
	}
	ChangeTitleString ((U8*)g_pstJWapAppInfo->pszTitleString) ;
	if(jWAP_GET_STATE() != E_WAP_END_DISPLAY_RECVD)
	{
		ChangeTitleIcon (IMG_ID_JATAAYU_WAP_ANIMATION_GIF) ;
	}
	draw_title () ;
	
	jWap_setSoftKeys () ;
	jWap_Register_KeyHandler_Funcs () ;

	iErr = jWap_refreshPriMmi () ;

}


/*	CB Registered with: Popup
	Description: This function is invoked when the display of popup ends.
		If the error is when fetching the very first page, it returns to main menu.
		Otherwise, it remains in the same screen.
*/
void jWap_errFromPrimary ()
{
	DeleteUptoScrID(SCR_ID_WAP_MAIN_SCREEN) ;
}


void jWap_homePage_entry_handler ()
{
	INT	iErr ;
	//U16 *pmDest ;
	JC_BOOLEAN bIsValidURL ;
	JC_CHAR 		*pszCorrectedURL ;

	pszCorrectedURL = JC_NULL ;

	bIsValidURL = aapIsValidTcharUrl(g_pstJWapAppInfo->amHomePage, &pszCorrectedURL, NULL) ;
	{
		JC_INT8	*pcHome ;
		jdi_CUtilsTcharToChar (NULL, pszCorrectedURL, &pcHome) ;
		//mmi_trace (1, "JDD_LOG: jWap_homePage_entry_handler >> Home Page: [%s]", pcHome) ;
		jdd_MemFree (pcHome) ;
	}
	
	if(!pszCorrectedURL || bIsValidURL == E_FALSE)
	{
		iErr = JC_ERR_INVALID_PARAMETER ;
	}
	else
	{
		jc_wstrcpy (g_pstJWapAppInfo->amHomePage, pszCorrectedURL) ;
		jdd_MemFree (pszCorrectedURL) ;
		iErr = JC_OK ;
	}

	if (!iErr)
	{
		jWap_set_priMmiScreen () ;	
		/* ARR and other Display settings */
		iErr = jWap_config_dispSettings () ;
		if (JC_OK == iErr)
		{
			iErr = jWap_config_connProfile () ;
		}

		jWap_setConnectingTitle((JC_CHAR*)GetString(STR_ID_WAP_CONNECTING)) ;
		iErr = jWap_send_fetch_request (g_pstJWapAppInfo->amHomePage) ;
	}

	if (iErr)
	{
		jWAP_SET_STATE (E_WAP_END_DISPLAY_RECVD) ;
		DisplayPopup((PU8)GetString(STR_GLOBAL_INVALID), IMG_GLOBAL_ERROR, 1, 5000, 1); //Last argument is tone id. 1 = ERROR tone
		jWap_errFromPrimary();
	}
	mmi_trace (1, "JDD_LOG: jWap_homePage_entry_handler () returned [%X]", JRET(iErr)) ;
}



void jWap_fetchUrl_entry_handler ()
{
	U8* guiBuffer ;
	EntryNewScreen (SCR_ID_WAP_INPUT_BOX, NULL, \
		jWap_fetchUrl_entry_handler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_INPUT_BOX) ;
	
	ShowCategory5Screen(
		STR_ID_WAP_FETCH_URL, 
		IMG_ID_JATAAYU_WAP_ICON,
		STR_GLOBAL_OPTIONS,
		IMG_GLOBAL_OPTIONS,
		STR_GLOBAL_BACK,
		IMG_GLOBAL_BACK,
		INPUT_TYPE_ALPHANUMERIC_LOWERCASE | INPUT_TYPE_OVERRIDE_DEFAULT | INPUT_TYPE_USE_ONLY_ENGLISH_MODES,
		jWap_getInputBuff (),
		jWAP_MAX_INPUT_SIZE,
		guiBuffer);
	
#ifdef __MMI_GB__
	GBSetInputboxLSKFunction(jWap_Options_fetchUrl);
#endif
	SetLeftSoftkeyFunction (jWap_Options_fetchUrl, KEY_EVENT_UP) ;
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;	
	RegisterInputMethodScreenCloseFunction (GoBackHistory );	
}


void jWap_Options_fetchUrl()
{
	U16 item_texts[] = {
		STR_ID_WAP_GO, \
		STR_ID_WAP_ADD_TO_BM,
		STR_GLOBAL_CANCEL,
		STR_GLOBAL_INPUT_METHOD
		} ;

	mmi_trace (1, "jwap_log: Options_fetchUrl called") ;

	EntryNewScreen (SCR_ID_FETCH_OPTIONS, NULL, jWap_Options_fetchUrl, NULL) ;
	RegisterHighlightHandler (jWap_fetch_options_highlight_handler) ;
	
	ShowCategory15Screen (STR_ID_WAP_FETCH_URL, IMG_ID_JATAAYU_WAP_ICON, 
							STR_GLOBAL_OK,
							IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,
							IMG_GLOBAL_BACK,
							(sizeof (item_texts)/sizeof (U16)), 
							item_texts, (PU16)gIndexIconsImageList, 
							1, 0, 0) ;

	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;
}

void jWap_cancelFetchUrl ()
{
	jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, jWAP_MAX_INPUT_SIZE) ;
	GoBacknHistory (1) ;
}

void jwap_fetch_go_back_2_history(void)
{
	GoBacknHistory(1);
}

void jWap_fetch_options_highlight_handler(JC_INT32 iIndex) 
{
	switch(iIndex)
	{
		case 0:
		{
			SetLeftSoftkeyFunction (jWap_fetchUrl,KEY_EVENT_UP) ; 
		}
		break ;

		case 1:
		{
			SetLeftSoftkeyFunction (jWap_fetch_options_add_bookmark_handler,KEY_EVENT_UP) ;
		}
		break ;

		case 2:
		{
			SetLeftSoftkeyFunction (jWap_cancelFetchUrl, KEY_EVENT_UP ) ;
		}
		break;

		case 3:
		{
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP ) ;
            	       RegisterInputMethodScreenCloseFunction (jwap_fetch_go_back_2_history);
		}
		break ;
			
		default:
		break ;
	}
}

void jWap_fetchUrl ()
{
	INT 			iErr ;
	JC_BOOLEAN 		bIsValidURL ;
	JC_CHAR 		*pszCorrectedURL ;
	
	if( 0 !=  jc_wstrlen(jWap_getInputBuff()))

	{
		bIsValidURL = aapIsValidTcharUrl((U16*)jWap_getInputBuff(), \
						&pszCorrectedURL, NULL) ;
		mmi_trace (1, "JDD_LOG: jWap_fetchUrl () >> ValidURL: %d", bIsValidURL) ;
		if (bIsValidURL == E_TRUE)
		{
			
			jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, jWAP_MAX_INPUT_SIZE) ;

			if (IsScreenPresent (SCR_ID_WAP_INPUT_BOX))
			{
				mmi_trace (1, "JDD_LOG: jWap_fetchUrl () >> Input Box Deleted") ;
				DeleteScreenIfPresent (SCR_ID_WAP_INPUT_BOX) ;
			}
			jWap_set_priMmiScreen () ;
			/* Pop out Input Box used to enter URL */	
			iErr = jWap_config_dispSettings () ;

			if (!iErr)
			{
				iErr = jWap_config_connProfile () ;
			}

			if (!iErr)
			{
				jWap_setConnectingTitle ((JC_CHAR*)GetString( \
					STR_ID_WAP_CONNECTING)) ;
				iErr =	jWap_send_fetch_request (pszCorrectedURL) ;
				jdd_MemFree(pszCorrectedURL) ;
				pszCorrectedURL = NULL ;
				if (iErr)
				{
					DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, 3000, 1); //Last argument is tone id. 1 = ERROR tone
					jWap_errFromPrimary();
				}
				else
				{
					//DeleteNHistory (1);
				}
				mmi_trace (1, "JDD_LOG: jWap_fetchUrl () >> HomePage Fetch Request: [%X]", JRET(iErr)) ;
			}
		}
		else
		{
			jdd_MemFree(pszCorrectedURL) ;
			pszCorrectedURL = NULL ;
			DisplayPopup((PU8)GetString(STR_ID_WAP_BM_WRONG_URL), IMG_GLOBAL_ERROR, 1,1000, 1);
		}
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_ID_WAP_BM_NO_URL), IMG_GLOBAL_ERROR, 1,1000, 1);
	}
}


JC_RETCODE jWap_FetchPushUrl (void)
{
	JC_RETCODE retCode = JC_OK ;
	if (g_pstJWapAppInfo->pmPushURL)
	{
		/*	TODO. Check whether the sim g_pstJWapAppInfo->simid, is available */
		if (1)
		{			
			jWap_set_priMmiScreen () ;
			retCode = jWap_config_dispSettings () ;
			if (JC_OK == retCode)
			{
				retCode = jWap_config_connProfile () ;
			}
			if (JC_OK == retCode)
			{
				jWap_setConnectingTitle ((JC_CHAR*)GetString(STR_ID_WAP_CONNECTING)) ;
				retCode = jWap_send_fetch_request (g_pstJWapAppInfo->pmPushURL) ;
				mmi_trace (1, "JDD_LOG: jWap_FetchPushUrl () >> jWap_send_fetch_request () returned [%X]",
					JRET(retCode)) ;
			}
		}
		else
		{
			// Display that SIM Unavailable
			//DisplayPopup((PU8)GetString(STR_ID_WAP_BM_WRONG_URL), IMG_GLOBAL_ERROR, 1,1000, 1);
		}
	}
	return retCode ;
}

	
void jWap_fetch_options_add_bookmark_handler()
{
	jWap_addEditBm_addInterface(NULL,(JC_CHAR*)jWap_getInputBuff ()) ;
}

/* Temporary Implementation for About Us Screen */
/* Need to find a suitable category screen for this */
static U8* aboutScreen [3] ;

void jWap_aboutUs_entry_handler ()
{	
	aboutScreen [0] = (U8 *)GetString(STR_ID_WAP_ABOUT_US_JWAP_BROWSER);	
	aboutScreen [1] = (U8 *)GetString(STR_ID_WAP_ABOUT_US_VERSION);	
	aboutScreen [2] = (U8 *)GetString(STR_ID_WAP_ABOUT_US_BUILD);

	EntryNewScreen(SCR_ID_WAP_ABOUT_US, NULL, jWap_aboutUs_entry_handler, NULL);
	ShowCategory6Screen(STR_ID_WAP_ABOUT_US, IMG_ID_JATAAYU_WAP_END_ANIMATION_GIF,
				          0, 0,
				          STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				          3,aboutScreen,NULL,0,NULL);

	SetLeftSoftkeyFunction (0,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;	
}

void jwap_ViewSavedPages ()
{
	JC_CHAR		*pmURL = JC_NULL, *pmFolderPath = JC_NULL, *pTempFolderPath = JC_NULL;
	JC_RETCODE	rCode = JC_OK ;
	JC_UINT32	uiFilePathLen ;

	FMGR_FILE_INFO_STRUCT	file_info;
	mmi_trace(1, "jwap_log: selected File [%d]", g_FolderItemIndex);
	
	mmi_fmgr_kernel_get_file_info(g_FolderItemIndex, &file_info);
	if (JC_NULL == file_info.file_name)
	{
		DisplayPopup((PU8)GetString(STR_ID_WAP_FAIL_TO_OPEN), IMG_GLOBAL_ERROR, 1,1000, 1);
		GoBackHistory () ;
	}
	else
	{
		JC_INT8 *pcURL ;
		JC_INT32	iIndex = 0;
		iIndex = jdi_CUtilsTcsIndexOfSubStr ((const JC_CHAR *)file_info.file_name, ".mrt", 0) ;
		if (0 < iIndex)
		{
			rCode = jdi_CUtilsCharToTchar (JC_NULL, CS_WAP_SAVED_PAGE_PATH_WITH_END_SLASH, \
										&pTempFolderPath);
			pmFolderPath = jdd_MemAlloc(jc_tcslen(pTempFolderPath) + \
										jc_tcslen(file_info.file_name) + 1, sizeof(JC_CHAR));

			jc_wstrcpy(pmFolderPath, pTempFolderPath);
			jc_wstrcat(pmFolderPath, (const UINT16*)file_info.file_name);

			
			jdi_CUtilsTcharToChar (JC_NULL, pmFolderPath, &pcURL) ;
			mmi_trace (1, "jWap_page_selection_callback file name [%s]", pcURL) ;
			jdd_MemFree (pcURL) ;
			jdd_MemFree (pTempFolderPath) ;
			uiFilePathLen = jc_wstrlen (pmFolderPath) + jc_strlen ("file://") ;
			pmURL = (JC_CHAR *)jdd_MemAlloc (sizeof (JC_CHAR), uiFilePathLen + 1) ;
			if (pmURL)
			{
				jdi_CUtilsTcsStrCpy ("file://", pmURL) ;
				jc_wstrcat (pmURL, pmFolderPath) ;

				jWap_set_priMmiScreen () ;
				rCode = jWap_config_dispSettings () ;
				if (JC_OK == rCode)
				{
					rCode = jWap_config_connProfile () ;
				}
				if (JC_OK == rCode)
				{
					jWap_setConnectingTitle ((JC_CHAR*)GetString (STR_ID_WAP_CONNECTING)) ;
					rCode = jWap_send_fetch_request (pmURL) ;
				}
				jdi_CUtilsTcharToChar (JC_NULL, pmURL, &pcURL) ;
				mmi_trace (1, "jWap_page_selection_callback URL [%s]", pcURL) ;
				jdd_MemFree (pcURL) ;
				jdd_MemFree (pmURL) ;
				jdd_MemFree (pmFolderPath) ;			
			}
		}
		else
		{
			rCode = JC_ERR_GENERAL_FAILURE;
		}
	}
	if (JC_OK != rCode)
	{
		DisplayPopup((PU8)GetString(STR_ID_WAP_FAIL_TO_OPEN), IMG_GLOBAL_ERROR, 1,1000, 1);
		mmi_trace (1, "JDD_LOG: jWap_page_selection_callback >> rCode [%X]", -rCode) ;
	}
}

pBOOL jwap_GetSavedpagesListItems( S32 item_index, UI_string_type str_buff, \
					UI_image_type* img_buff_p, U8 str_img_mask )
{
	FMGR_FILE_INFO_STRUCT	file_info;
	JC_INT32 				iIndex = 0;
	JC_CHAR					*pmListBuffer = JC_NULL;
	pBOOL 					rCode = JC_OK;
	
	if((item_index < 0) || ((JC_UINT32)item_index >= g_file_count))
		rCode = FALSE;	
	
	mmi_fmgr_kernel_get_file_info(item_index, &file_info);
	iIndex = jdi_CUtilsTcsIndexOfSubStr ((const JC_CHAR *)file_info.file_name, ".mrt", 0) ;
	if (0 < iIndex)
	{
		pmListBuffer = jdi_CUtilsTcsCopyIndexNum (JC_NULL, (const JC_CHAR *)file_info.file_name, 0,
										 iIndex) ;		
		rCode =  TRUE;		
	}			
	else
	{
		pmListBuffer = jdi_CUtilsTcsDuplicate(JC_NULL, (const JC_CHAR *)file_info.file_name);		
		rCode =  TRUE;
	}
	jc_wstrcpy((UINT16*)str_buff,(UINT16*)(pmListBuffer));
       *img_buff_p =  (UI_image_type)GetImage((U16)(IMG_WAP_SAVED_PAGE_ICON));
	//test code
	if (JC_NULL != pmListBuffer)
	{
		JC_INT8 *pcTemp = JC_NULL;
		jdi_CUtilsTcharToChar (JC_NULL, (const JC_CHAR *)pmListBuffer, &pcTemp) ;
		mmi_trace(12, "jdc_log: rCode pcTemp[%s]",pcTemp);
		jdd_MemFree(pcTemp);			
	}
	jdd_MemFree(pmListBuffer);	
	return rCode;
}

void jwap_DeleteSavedPagesEntryHandler()
{
	DisplayNewconfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, \
		get_string(STR_ID_MMS_CNF_DELETE), IMG_GLOBAL_QUESTION, 1,jwap_DeleteSavedPagesEntryHandler) ;
	SetLeftSoftkeyFunction(jwap_DeleteSavedPages,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP) ;
}

void jwap_DeleteSavedPages()
{
	FMGR_FILE_INFO_STRUCT	file_info;
	JC_CHAR*   				pmFolderPath = JC_NULL, *pTempFolderPath = JC_NULL;
	JC_RETCODE 				rCode = JC_OK;
	U16 file_count= 0;
	
	mmi_trace(1, "jwap_log: selected File [%d]", g_FolderItemIndex);	
	mmi_fmgr_kernel_get_file_info(g_FolderItemIndex, &file_info);
	if(file_info.file_name)
	{
		rCode = jdi_CUtilsCharToTchar (JC_NULL, CS_WAP_SAVED_PAGE_PATH_WITH_END_SLASH, \
									&pTempFolderPath);
		pmFolderPath = jdd_MemAlloc(jc_tcslen(pTempFolderPath) + \
									jc_tcslen(file_info.file_name) + 1, sizeof(JC_CHAR));

		jc_wstrcpy(pmFolderPath, pTempFolderPath);
		jc_wstrcat(pmFolderPath, (const UINT16*)file_info.file_name);
		
		rCode = jdd_FSDelete (JC_NULL, pmFolderPath);

		if(JC_OK == rCode)
		{						
			file_count = jwap_GetOfflinePageCountFromFolder(); 
			DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1,1000, 1);			
			if(0 < file_count)
			{				
				DeleteNHistory(2);
				GoBackHistory();
			}
			else
			{
				//incase no saved pages after deleted last page 
				DeleteNHistory(3);
				GoBackHistory();
			}
		}
		mmi_trace(1, "jwap_log: DeleteSavedPages rCode[%d], file[%d]", rCode, file_count);
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_ID_WAP_INVALID_INPUT), IMG_GLOBAL_ERROR, 1,1000, 1);
		DeleteNHistory(1);
		GoBackHistory();
	}
}

void jwap_DeleteAllSavedPagesEntryHandler()
{
	DisplayNewconfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, \
		get_string(STR_ID_MMS_CNF_DELETE_ALL), IMG_GLOBAL_QUESTION, 1,jwap_DeleteAllSavedPagesEntryHandler) ;
	SetLeftSoftkeyFunction(jwap_DeleteAllSavedPages,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP) ;
}

void jwap_DeleteAllSavedPages()
{
	JC_RETCODE 		rCode = JC_OK;
	JC_CHAR*   		pmFolderPath = JC_NULL;
	
	rCode = jdi_CUtilsCharToTchar (JC_NULL, CS_WAP_SAVED_PAGE_PATH, \
									&pmFolderPath);
	if(pmFolderPath && JC_OK == rCode)
	{
		rCode = jdd_FSDelDir (JC_NULL, pmFolderPath) ;
		mmi_trace(1, "jwap_log: jdd_FSDelDir[%d]", rCode);
		if(JC_OK == rCode)
		{
			DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1,1000, 1);			
		}
		else
		{
			DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1,1000, 1);			
		}
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1,1000, 1);				
	}
	DeleteNHistory(3);
	GoBackHistory();	
}

void jwap_SavedPagesOptionHltHandler (int iSelectIndex)
{
	mmi_trace (1, "jdd_log: SavedPagesOption Index [%d]", iSelectIndex) ;
	switch (iSelectIndex)
	{
		case 0:
			SetLeftSoftkeyFunction (jwap_ViewSavedPages,KEY_EVENT_UP) ;
			break ;
		case 1:
			SetLeftSoftkeyFunction (jwap_DeleteSavedPagesEntryHandler ,KEY_EVENT_UP) ;
			break ;
		case 2:
			SetLeftSoftkeyFunction (jwap_DeleteAllSavedPagesEntryHandler ,KEY_EVENT_UP) ;
			break ;
		default:
			break ;
	}
}

void jwap_SavedPagesOptionLSKHandler()
{
	U8* guiBuffer;
	U16 item_texts[] = {STR_ID_WAP_VIEW, \
						 STR_GLOBAL_DELETE,
						 STR_GLOBAL_DELETE_ALL
					 	};

	EntryNewScreen (SCR_ID_SAVED_PAGES_OPTION_SRCEEN, NULL, \
						jwap_SavedPagesOptionLSKHandler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_SAVED_PAGES_OPTION_SRCEEN) ;
	
	RegisterHighlightHandler (jwap_SavedPagesOptionHltHandler) ;
	ShowCategory15Screen (STR_ID_WAP_SAVED_PAGES, IMG_ID_JATAAYU_WAP_ICON,
					 STR_GLOBAL_OK, 0,
					 STR_GLOBAL_BACK, 0,
					 (sizeof (item_texts)/sizeof (U16)), item_texts,
					 (PU16)gIndexIconsImageList,
					 1, 0, guiBuffer);

	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}

void jwap_SelectedFileIndex(int iSelectIndex)
{
	mmi_trace(1,"jwap_log: get_highlited_id [%d]", iSelectIndex);
	g_FolderItemIndex = iSelectIndex;
}

void jwap_PreapareSavedPagesMMI()
{
	U8 *guiBuffer = NULL;
	JC_UINT32 uiCount;
	
	uiCount = mmi_fmgr_kernel_get_file_count();	
	mmi_trace(1, "jwap_log: MMI called count is [%d]", uiCount);
	EntryNewScreen(SCR_ID_WAP_SAVED_PAGES, JC_NULL, \
						jwap_PreapareSavedPagesMMI, NULL);
	RegisterHighlightHandler(jwap_SelectedFileIndex);
	SetParentHandler(SCR_ID_WAP_MAIN_SCREEN);
	ShowCategory184Screen(STR_ID_WAP_SAVED_PAGES, IMG_ID_JATAAYU_WAP_ICON, \
						STR_GLOBAL_OPTIONS, 0, 
						STR_GLOBAL_BACK, 0, 
						uiCount, 
						jwap_GetSavedpagesListItems, 
						0, 0, guiBuffer);
	SetLeftSoftkeyFunction(jwap_SavedPagesOptionLSKHandler, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void jWap_offline_pages_entry_handler (void)
{
	U16		file_count = 0;

	file_count = jwap_GetOfflinePageCountFromFolder();
	mmi_trace(1, "jwap_log: fmgr_kernel_init count is [%d]", file_count);
	g_file_count = file_count;
	if(0 < file_count)
	{		
		jwap_PreapareSavedPagesMMI();
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_ID_WAP_NO_OFFLINE_PAGES), \
						IMG_GLOBAL_INFO, 1, 2000, 1);		
	}
}

JC_CHAR* jwap_GetCurrentPageURL(void)
{
    JC_RETCODE        rCode     = JC_OK ;
    ST_BE_PAGE_INFO   stPageInfo ;

    jc_memset (&stPageInfo, 0x0, sizeof (ST_BE_PAGE_INFO)) ;
    rCode = jdi_BEGetPageInfo (g_pstJWapAppInfo->hBECore, &stPageInfo) ;

    if (JC_OK == rCode)    
        return stPageInfo.pmURI;    
    else
        return JC_NULL;    
}

void jWap_options_Show_URL_Exit_Handler()
{
	mmi_trace(1, "jdd_log: Show_URL_Exit_Handler called");
	jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, (jWAP_MAX_INPUT_SIZE + 1)* sizeof(JC_CHAR)) ;
}
void jWap_options_Show_URL()
{
	JC_CHAR * pmShowURI = jwap_GetCurrentPageURL() ;
	jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, (jWAP_MAX_INPUT_SIZE + 1)* sizeof(JC_CHAR)) ;
	
	if(JC_NULL != pmShowURI)
	{		
		jc_tcsncpy((JC_CHAR*)g_pstJWapAppInfo->pcInputBuff,\
				pmShowURI, jWAP_MAX_INPUT_SIZE) ;
	}
	
	EntryNewScreen(SCR_ID_WAP_SHOW_URL, jWap_options_Show_URL_Exit_Handler, \
					jWap_options_Show_URL, NULL);
	ShowCategory78Screen(STR_ID_VIEW_URL, IMG_ID_JATAAYU_WAP_ICON,
						0, 0,
						STR_GLOBAL_BACK, 0,
						NULL,
						(U8*) g_pstJWapAppInfo->pcInputBuff,
						jc_tcslen(g_pstJWapAppInfo->pcInputBuff),
						NULL);
	SetLeftSoftkeyFunction (0,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
			
}

void jwap_SetAsHmPageMenuDoneHandler()
{
	JC_INT8 	cUrlName[jWAP_MAX_INPUT_SIZE + 1] = {0,};

	if(jc_wstrlen((JC_CHAR*)g_pstJWapAppInfo->pcInputBuff))
	{	
		jdi_CUtilsTcharToCharMem((JC_CHAR*)g_pstJWapAppInfo->pcInputBuff, jWAP_MAX_INPUT_SIZE + 1, cUrlName);
		
		//Coolsand interface to set Homepage in setting use "g_pstJWapAppInfo->pcInputBuff" value
		mmi_ph_brw_set_activated_profile_homepage(cUrlName);

		jWap_readUpdateDataConnInfo () ;	
		mmi_trace(1, "jdd_log:DoneHandler name[%s]", g_pstJWapAppInfo->pcInputBuff);
		jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, (jWAP_MAX_INPUT_SIZE + 1)* sizeof(JC_CHAR)) ;
				
		DeleteNHistory(1) ;	
		GoBackHistory();
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, \
						1, 2000, 1);
	}
	else
	{	
		GoBackToHistory(SCR_ID_WAP_SETAS_HMPAGE_OPT_SCREEN);
		DisplayPopup((PU8)GetString(STR_ID_WAP_INVALID_INPUT), IMG_GLOBAL_ERROR, \
						1, 2000, 1);
	}
}

void jwap_SetAsHmPageMenuCancelHandler()
{
	DeleteNHistory(1) ;
	GoBackHistory();
}

void jwap_SetAsHmPageMenuInputMethod(void)
{
	GoBacknHistory(1);
}

void jwap_SetURLAsHomepageEditScreenMenuHandler(S32 index)
{
	switch(index)
	{
		case 0:
			SetLeftSoftkeyFunction (jwap_SetAsHmPageMenuDoneHandler, KEY_EVENT_UP ) ;
			break;
			
		case 1:
			SetLeftSoftkeyFunction (jwap_SetAsHmPageMenuCancelHandler, KEY_EVENT_UP ) ;
			break;

		case 2:
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP ) ;
			RegisterInputMethodScreenCloseFunction (jwap_SetAsHmPageMenuInputMethod);
			break ;
	}	
}


void jwap_SetURLAsHomepageOptLSKHandler ()
{
	U16 item_texts[] = {STR_GLOBAL_DONE, \
						STR_GLOBAL_CANCEL, \
						STR_GLOBAL_INPUT_METHOD} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, \
						IMG_GLOBAL_L3} ;
	
	EntryNewScreen (SCR_ID_WAP_SET_AS_HMPAGE_OPTSCREEN, 0, \
					jwap_SetURLAsHomepageOptLSKHandler, 0) ;
	SetParentHandler (0);
	
	RegisterHighlightHandler (jwap_SetURLAsHomepageEditScreenMenuHandler);

	ShowCategory52Screen (STR_GLOBAL_OPTIONS, IMG_ID_JATAAYU_WAP_ICON, 
							STR_GLOBAL_OK, 0,
							STR_GLOBAL_BACK, 0, 
							3, item_texts, item_icons,
							0, 0, 0, 0);	
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP ) ;	
	
}

void jWap_options_Set_URL_As_Homepage_EntryHandler()
{
	JC_CHAR * pmURI = jwap_GetCurrentPageURL() ;
	jc_memset (g_pstJWapAppInfo->pcInputBuff, 0, (jWAP_MAX_INPUT_SIZE + 1)* sizeof(JC_CHAR)) ;
	
	if(JC_NULL != pmURI)
	{		
		jc_tcsncpy((JC_CHAR*)g_pstJWapAppInfo->pcInputBuff,\
				pmURI, jWAP_MAX_INPUT_SIZE) ;
	}
	jWap_options_Set_URL_As_Homepage();
}

void jWap_options_Set_URL_As_Homepage()
{
	S16 required_input_modes[] = {INPUT_MODE_MULTITAP_LOWERCASE_ABC,
								INPUT_MODE_MULTITAP_UPPERCASE_ABC ,
								INPUT_MODE_123,
								-1
								} ;
	U8* guiBuffer = JC_NULL;	
	EntryNewScreen (SCR_ID_WAP_SETAS_HMPAGE_OPT_SCREEN, JC_NULL, \
					jWap_options_Set_URL_As_Homepage, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_SETAS_HMPAGE_OPT_SCREEN);			

	ShowCategory5Screen_ext (STR_ID_SET_AS_HOMEPAGE_URL, IMG_ID_JATAAYU_WAP_ICON,
						STR_GLOBAL_OPTIONS, 0,
						STR_GLOBAL_BACK, 0,
						INPUT_TYPE_ALPHANUMERIC_LOWERCASE|INPUT_TYPE_ALPHANUMERIC_UPPERCASE|INPUT_TYPE_NUMERIC,
						(U8*)g_pstJWapAppInfo->pcInputBuff, JWAP_MAX_URL_LEN, 
						guiBuffer, 0, required_input_modes, NULL
						);	
#ifdef __MMI_GB__
	GBSetInputboxLSKFunction (jwap_SetURLAsHomepageOptLSKHandler);
#endif	
	SetLeftSoftkeyFunction (jwap_SetURLAsHomepageOptLSKHandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);		
}

void jWap_mainMenu_highlight_handler (int iSelectIndex) 
{

	mmi_trace (1, "jwap_log: mainMenu_highlight_handler Index: %d", iSelectIndex) ;
	
	switch (iSelectIndex)
	{
		case E_MAIN_WAP_HOMEPAGE:
			SetLeftSoftkeyFunction (jWap_homePage_entry_handler ,KEY_EVENT_UP) ;
			break ;
			
		case E_MAIN_WAP_BOOKMARKS:
#ifdef BOOKMARK_VBOOKMARK_STORAGE 			
			SetLeftSoftkeyFunction (jWap_BookMark_entry_handler ,KEY_EVENT_UP) ;
#endif
			break ;

		case E_MAIN_WAP_RECENT:
			SetLeftSoftkeyFunction (jWap_recentPages_entry_handler ,KEY_EVENT_UP) ;
			break ;
			
		case E_MAIN_WAP_SAVED_PAGES:
			SetLeftSoftkeyFunction (jWap_offline_pages_entry_handler ,KEY_EVENT_UP) ;
			break ;
			
		case E_MAIN_WAP_FETCH_URL:
			SetLeftSoftkeyFunction (jWap_fetchUrl_entry_handler ,KEY_EVENT_UP) ;
			break ;
#ifdef JWAP_PUSH_ENABLE	
		case E_MAIN_SERVICE_INBOX:
			SetLeftSoftkeyFunction (jWap_push_entry_handler , KEY_EVENT_UP) ;
			break ;		
#endif
		case E_MAIN_WAP_SETTINGS:
			SetLeftSoftkeyFunction (jWap_settings_entry_handler ,KEY_EVENT_UP) ;
			break ;		
			
		case E_MAIN_WAP_ABOUT_US:
			SetLeftSoftkeyFunction (jWap_aboutUs_entry_handler, KEY_EVENT_UP) ;
			break ;
			
		default:
			break ;		
	}

}

#include "satstruct.h"
extern SATCONTEXT gSATContext;

JC_UINT8 jwap_ApplicationExitHandler(void)
{
	mmi_trace(1, "JDD_LOG: jwap_ApplicationExitHandler winID[%d]",\
				g_pstJWapAppInfo->uiWinId);

        jWap_showDisconnectingScreen () ;

        DeleteScreenIfPresent(SCR_ID_WAP_MAIN_SCREEN);
        jddex_FreePlatformWidgets();
	jwap_freeSettingsInfo () ;
	jWap_freeDownloadInfo () ;	
	jWap_freeBookmarkInfo () ;
	jWap_historyFreeMemory () ;
	#ifdef JWAP_PUSH_ENABLE	
	jWap_PushFreeMemory () ;
	#endif
	if (g_pstJWapAppInfo->hBECore && 0 != g_pstJWapAppInfo->uiWinId)
	{
		mmi_trace(12,"JDD_LOG: BEStopRequest called");
		jdi_BEStopRequest (g_pstJWapAppInfo->hBECore) ;
		jdi_BEStopAllDownload (g_pstJWapAppInfo->hBECore) ;
	}
	if (g_pstJWapAppInfo->pstFetchReq)
	{
		jWap_freeFetchInfo (g_pstJWapAppInfo->pstFetchReq) ;
		g_pstJWapAppInfo->pstFetchReq = JC_NULL ;
	}
	if (E_WAP_BEGIN_DISPLAY_RECVD == jWAP_GET_STATE())
	{
		mmi_trace(12,"JDD_LOG: Func: %s Wait for the end page listner", __FUNCTION__);
		/* Wait for the end page listner */
	}
	else
	{
		if (0 != g_pstJWapAppInfo->uiWinId)
		{
			jWap_deInit_priMmi () ;
		}
		jWap_deInit_engine () ;
		jWap_SetWapAppStatus(E_FALSE);
		if(!gSATContext.isInSATMenu)
		{
			jWap_exitDisconnectingScreen () ;
		}
		else
		{
			g_pstJWapAppInfo->stWapState.eWapExitKeyPressed = E_WAP_EXIT_KEY_NONE ;
		}
        
        	WAPLaunchBrowserRsp(NULL);
	}
    
	return 0;
}
void jWap_GobackHandler(void)
{
	mmi_trace(1,"JDD_LOG: jWap_GobackHandler ()") ;
	g_pstJWapAppInfo->stWapState.eWapExitKeyPressed = E_WAP_EXIT_KEY_NONE ;
	jwap_ApplicationExitHandler () ;
//	GoBackHistory () ;
}
void jWap_new_mainMenu_entry_handler ()
{
	U8* guiBuffer; 
	U16 item_texts[] = {
		STR_ID_WAP_HOMEPAGE, \
		STR_ID_WAP_BOOKMARKS,
		STR_ID_WAP_RECENT,
		STR_ID_WAP_SAVED_PAGES,
		STR_ID_WAP_FETCH_URL,
#ifdef JWAP_PUSH_ENABLE
		STR_ID_WAP_SERVICE_INBOX,
#endif		
		STR_ID_WAP_SETTINGS,
		STR_ID_WAP_ABOUT_US
		} ;

	mmi_trace (1, "@@@ jWap_new_mainMenu_entry_handler @@@") ;

	EntryNewScreen (SCR_ID_WAP_MAIN_SCREEN, NULL, jWap_new_mainMenu_entry_handler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_MAIN_SCREEN) ;	
	RegisterHighlightHandler (jWap_mainMenu_highlight_handler) ;
	ShowCategory15Screen (STR_ID_WAP_TEXT, IMG_ID_JATAAYU_WAP_ICON, 
							STR_GLOBAL_OK, 0, 
							STR_GLOBAL_BACK, 0,
							E_MAIN_MENU_COUNT, 
							item_texts, (PU16)gIndexIconsImageList, 
							1, 0, guiBuffer);

	SetRightSoftkeyFunction (jWap_GobackHandler, KEY_EVENT_UP);	
	//SetDelScrnIDCallbackHandler(SCR_ID_WAP_MAIN_SCREEN,jwap_ApplicationExitHandler); 
}

/**
 * @ingroup		jwap
 * @brief			Entry Handler for Menu Screen
 * This is the entry handler for main menu screen. This function initializes jB5 
 * core, necessary jdd modules and also the WAP application context.
 * This function re-registers jWap_new_mainMenu_entry_handler as the new
 * entry handler which will not do any initialization tasks. This is done to avoid
 * multiple initialization when user switches b/w menu items like History, 
 
 *
 */
#ifdef __MMI_COMPRESS_CODE_SECTION__
extern BOOL checkVMOn();
extern void mmi_display_gprs_active();
extern void cdfu_wap();
#endif
void jWap_mainMenu_entry_handler (void)
{
	INT iErr ;

	mmi_trace (1, "JDD_LOG: jWap_mainMenu_entry_handler with Launch Mode [%d]",
		g_pstJWapAppInfo->eWapLaunchMode) ;
	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
	#ifdef __MMI_MULTI_SIM__
       if(MTPNP_AD_GET_UsableSide_Index()==-1)
        {
            mmi_display_noSimCard();
            return;
       }
	#endif
#ifdef __MMI_COMPRESS_CODE_SECTION__
	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(jWap_mainMenu_entry_handler);
		return;
	}
	cdfu_wap();
#endif
#ifdef __NATIVE_JAVA__
	if(checkVMOn())
	{
		mmi_show_qq_select_confirm(jWap_mainMenu_entry_handler);
		return;
	}
#endif
	jWap_SetWapAppStatus(E_TRUE); 
	jc_memset (jWap_getInputBuff(), 0, jWAP_MAX_INPUT_SIZE) ;
	jWap_initAppSettingsInfo () ;
	iErr = jWap_init_engine() ; 
	if (!iErr)
	{
		switch (g_pstJWapAppInfo->eWapLaunchMode)
		{
			case E_WAP_LAUNCH_NORMAL:
				jWap_new_mainMenu_entry_handler () ;
				break ;

			case E_WAP_LAUNCH_URL:
				if (g_pstJWapAppInfo->pmPushURL)
				{
					iErr = jWap_FetchPushUrl () ;
					if (JC_OK != iErr)
					{
						mmi_trace (1, "JDD_LOG: jWap_mainMenu_entry_handler >> jWap_FetchPushUrl failed (%X)", -iErr) ;
						jdd_MemFree (g_pstJWapAppInfo->pmPushURL) ;
						g_pstJWapAppInfo->pmPushURL = JC_NULL ;
						#ifdef __MMI_MULTI_SIM__
						g_pstJWapAppInfo->simid = g_pstJWapAppInfo->original_simid ;
						#endif
					}
				}
				break ;
#ifdef	JWAP_PUSH_ENABLE	

			case E_WAP_LAUNCH_PUSH_ITEM:
				{
					//code is commented so that user can see all the push messages
					//jWap_ShowLatestItemDetails () ;
					jWap_push_entry_handler () ;	
				}
				break ;

			case E_WAP_LAUNCH_PUSH_INBOX:
				break;
#endif				
			default:
				break ;
		}
	}
	mmi_trace (1, "JDD_LOG: Browser Init Status: %X", JRET(iErr)) ;
}


void jWap_highlight_handler (void)
{
#if 0
	JC_RETCODE rCode = JC_OK;
	MMS_MEDIA_INFO stMMSMediaInfo;

	jc_memset(&stMMSMediaInfo, 0, sizeof(MMS_MEDIA_INFO));

	jdi_CUtilsCharToTchar(0, "/t/mms_test_files/25kjpg.jpg", &stMMSMediaInfo.pFileName);
	jdi_CUtilsCharToTchar(0, "9611909208", &stMMSMediaInfo.pRecpt_To);
	jdi_CUtilsCharToTchar(0, "9611909209", &stMMSMediaInfo.pRecpt_CC);
	jdi_CUtilsCharToTchar(0, "9611909210", &stMMSMediaInfo.pRecpt_BCC);
	stMMSMediaInfo.eMediaType = E_MMS_MEDIA_OTHERS;

	rCode = jmms_LaunchMMSFromApp(&stMMSMediaInfo);
	mmi_trace (1, "JDD_LOG: jmms_LaunchMMSFromApp: %d @@@", rCode) ;
#endif	
  g_pstJWapAppInfo->eWapLaunchMode = E_WAP_LAUNCH_NORMAL ;
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	SetLeftSoftkeyFunction (MTPNP_jWap_mainMenu_entry_handler ,KEY_EVENT_UP) ;
#else
	SetLeftSoftkeyFunction (jWap_mainMenu_entry_handler ,KEY_EVENT_UP) ;
#endif

}

#endif //_JATAAYU_APP__

