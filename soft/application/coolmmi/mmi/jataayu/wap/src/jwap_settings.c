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
 * @file		jwap_settings.c
 * @ingroup	jWAP
 * @brief	
 *
 **/

#define MODULE_NAME "JWAP"
#define FILE_NAME "jwap_settings.c"

#ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

#include "jwap_inc.h"



#include "commonscreens.h"
#include "cfw.h"

#include "jdi_cutils_tcsstr.h"
#include "jdi_communicator.h"
#include "jdd_dataconnection.h"

#include "custom_nvram_editor_data_item.h"
#include "dataaccountgprot.h"
#include "jprofilehandlertypes.h"
#include "jprofilehandlerprots.h"
#include "settingprofile.h"
#ifdef  __MMI_MULTI_SIM__
#include "mtpnp_ad_resdef.h"
#endif
/***************************************************************************
 * Macros
 **************************************************************************/

#define MAX_WAP_INLINE_ITEMS   4

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/
typedef enum _jwap_settings_options_enum
{
	E_WAP_SETT_TIMEOUT_CAPTION,
	E_WAP_SETT_TIMEOUT_ITEM,
	E_WAP_SETT_GRAPHICS_CAPTION,
	E_WAP_SETT_GRAPHICS_ITEM,
	E_WAP_SETT_CACHE_CAPTION,
	E_WAP_SETT_CACHE_ITEM,
	E_WAP_SETT_COOKIE_CAPTION,
	E_WAP_SETT_COOKIE_ITEM,
	E_WAP_SETT_TOTAL_ITEMS
}E_WAP_SETTINGS_MENU_ITEMS ;


typedef struct _settings_options_info
{
	S8* sTimeoutOptLst [MAX_WAP_INLINE_ITEMS] ;
	S8* sGraphicsOptLst [2] ;
	S8* sCacheOptLst [2] ;
	S8* sCookieOptLst [2] ;
	S32 currTOSelItem ;
	S32 currCacheSelItem ;
	S32 currCookieSelItem ;
	S32 currGraphicsSelItem ;
	S8	bUpdateConnSett ;
	InlineItem wgui_inline_list_items [E_WAP_SETT_TOTAL_ITEMS] ;
}SETTINGS_OPTIONS_INFO ;


static SETTINGS_OPTIONS_INFO *g_pstSettOptInfo ;


 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/
//extern void mmi_brw_restore_default_profile_settings(void);  
//extern void mmi_brw_mms_restore_default_profile_settings(void); 

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

void entry_activate_profile(void)
{
//	jWap_initProfiles () ;
//	mmi_brw_restore_default_profile_settings();
//	mmi_brw_mms_restore_default_profile_settings();
//	mmi_ph_brw_set_activated_profile_homepage("my_wap.com");
	//mmi_trace(1,"mmi_ph_init()  ok");
	mmi_ph_pre_entry_show_profile_list_menu(PH_BROWSER_PROFILE);
	//mmi_trace(1,"mmi_ph_pre_entry_show_profile_list_menu()  ok");

	return;
}


void jWap_settingsOption_inItemSel_handler ()
{
	ST_WAP_SETTINGS	stWapSettInfo ;
	INT				iErr ;

	jc_memset (&stWapSettInfo, 0, sizeof (ST_WAP_SETTINGS)) ;
	stWapSettInfo.stWAPOptions.bEnableCache = g_pstSettOptInfo->currCacheSelItem ;
	stWapSettInfo.stWAPOptions.bEnableCookie = g_pstSettOptInfo->currCookieSelItem ;
	stWapSettInfo.stWAPOptions.bEnableGraphics = g_pstSettOptInfo->currGraphicsSelItem ;

	switch (g_pstSettOptInfo->currTOSelItem) 
	{
		case 0:
			stWapSettInfo.stWAPOptions.uiTimeout = 30 ;
			break ;
		case 2:
			stWapSettInfo.stWAPOptions.uiTimeout = 100 ;
			break ;
		case 3:
			stWapSettInfo.stWAPOptions.uiTimeout = 0 ;
			break ;
		case 1:
		default:
			stWapSettInfo.stWAPOptions.uiTimeout = 60 ;
			break ;
	}


	jWap_setUpdateCore (&stWapSettInfo) ;
	iErr = jWap_updateSettingsFile (&stWapSettInfo) ;

	
	if (!iErr)
	{
		DisplayPopup(GetString (STR_GLOBAL_DONE), IMG_GLOBAL_YES, 1, 2000, 1) ;
	}
	else
	{
		DisplayPopup(GetString (STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
	}

	/* Free without null-check. If it crashes, there is some problem with the UI flow! */
	jdd_MemFree (g_pstSettOptInfo) ;
	g_pstSettOptInfo = JC_NULL ;
}


void jWap_settingsOption_back_handler ()
{
	jwap_freeSettingsInfo () ;
	GoBackHistory () ;
}

/***************************************************************************
 * All Global Function Definitions
 **************************************************************************/
 INT jWap_readFromSettFile (ST_WAP_SETTINGS  *pstWapSettInfo)
 {
	 /* Info should be read from file and the struct should be updated */
	 JC_CHAR *pmFileName ;
	 JDD_FILE hSettFile ;
	 INT  iErr ;
 
	 iErr = JC_OK ;
	 pmFileName = JC_NULL ;
	 jdi_CUtilsCharToTchar (JC_NULL, CS_WAP_SETTINGS_FILE_NAME, &pmFileName) ;
 
	 if (pmFileName)
	 {
		 hSettFile = jdd_FSOpen (JC_NULL, pmFileName, E_OPEN_READ_MODE) ;
		 if (hSettFile)
		 {
			 if (1 != jdd_FSRead ((void *)pstWapSettInfo, sizeof (ST_WAP_SETTINGS), \
				 1, hSettFile))
			 {
				 iErr = JC_ERR_FILE_READ ;
			 }
			 
			 jdd_FSClose (hSettFile) ;
		 }
		 else
		 {
			 iErr = JC_ERR_FILE_OPEN ;
		 }
		 jdd_MemFree (pmFileName) ;
	 }
	 else
	 {
		 iErr = JC_ERR_MEMORY_ALLOCATION ;
	 }
 
	 /* Set Default Values */
	 if (iErr)
	 {
		 pstWapSettInfo->stWAPOptions.bEnableCache = 1 ;
		 pstWapSettInfo->stWAPOptions.bEnableCookie = 1 ;
		 pstWapSettInfo->stWAPOptions.bEnableGraphics = 1 ;
		 pstWapSettInfo->stWAPOptions.uiTimeout = 60 ;
	 }
	 
	 mmi_trace (1, "@@@ jWap_updateSettingsFile returned: %x", JRET(iErr)) ;
	 return (iErr) ;
 }
 
 
 
 INT  jWap_updateSettingsFile (ST_WAP_SETTINGS	 *pstWapSettInfo)
 {
	 JC_CHAR *pmFileName ;
	 JDD_FILE hSettFile ;
	 INT  iErr ;
 
	 iErr = JC_OK ;
	 pmFileName = JC_NULL ;

	 jdi_CUtilsCharToTchar (JC_NULL, CS_WAP_SETTINGS_FILE_NAME, &pmFileName) ;
 
	 if (pmFileName)
	 {
		 hSettFile = jdd_FSOpen (JC_NULL, pmFileName, E_CREATE_WRITE_MODE) ;
		 if (hSettFile)
		 {
			 jdd_FSWrite ((void *)pstWapSettInfo, sizeof (ST_WAP_SETTINGS), \
				 1, hSettFile) ;
			 jdd_FSClose (hSettFile) ;
		 }
		 else
		 {
			 iErr = JC_ERR_FILE_OPEN ;
		 }
		 jdd_MemFree (pmFileName) ;
	 }
	 else
	 {
		 iErr = JC_ERR_MEMORY_ALLOCATION ;
	 }
 
	 mmi_trace (1, "@@@ jWap_updateSettingsFile returned: %x", JRET(iErr)) ;
	 return (iErr) ;
 }
 

 void jWap_readUpdateDataConnInfo ()
 {
	 nvram_wap_profile_content_struct *profCont ;
	 ST_BE_PROFILE			 stProxySett ;
	 ST_GENERIC_DATA_CONN	 stGenDataConn ;
	 JC_CHAR				 amApn [MAX_DATA_CONN_APN_SIZE] ;
 	 JC_CHAR 				*pmHome ;
	 
	 profCont =  mmi_ph_get_activated_wap_profile () ;
	 
  
	 if (profCont)
	 {
		 jc_memset (&stProxySett, 0, sizeof (ST_BE_PROFILE)) ;
		 jc_memset (&stGenDataConn, 0, sizeof (ST_GENERIC_DATA_CONN)) ;
 
		 /*  Fill Proxy Info */
		 jc_snprintf (stProxySett.acHostName, BE_MAX_HOST_SIZE, "%d.%d.%d.%d", \
			 profCont->proxy_ip[0],
			 profCont->proxy_ip[1],
			 profCont->proxy_ip[2],
			 profCont->proxy_ip[3]) ;
	 
		 /* Secured Connection is not yet added in UI */
		 if (PH_CONN_TYPE_CONNECTION_OREINTED == profCont->conn_type)
		 {
		 	
			 stProxySett.uiPortNum = 9201 ; /* No UI is provided for WAP Port. It is always 9201 */
			 stProxySett.eMode = E_BE_WSP_CONNECTION_ORIENTED ;
					
		 }
		 #ifdef CS_SECURITY_ENABLE
		 else if (PH_CONN_TYPE_CONNECTION_OREINTED_SECURE== profCont->conn_type)
		 {
			 stProxySett.uiPortNum = 9203 ; /* No UI is provided for WAP Port. It is always 9201 */
			 stProxySett.eMode = E_BE_WSP_CONNECTION_ORIENTED_SECURE;

		 }
		 #endif
		 else /* Secured Connection is not yet supported. Considering HTTP as default */
		 {
			 if (!jc_strcmp ("0.0.0.0", stProxySett.acHostName))
			 {
				 stProxySett.eMode = E_BE_WPHTTP_DIRECT ;
			 }
			 else
			 {
				 stProxySett.eMode = E_BE_WPHTTP_PROXY ;
				 stProxySett.uiPortNum = profCont->proxy_port ;
			 }
		 }
 
		 /* Fill Data Conn Info */
		 stGenDataConn.eBearerType = E_DATA_CONN_GPRS_BEARER_TYPE ;
		 mmi_dtcnt_get_apn (profCont->data_account, \
			 (S8*)amApn, MAX_DATA_CONN_APN_SIZE) ;
		 jdi_CUtilsTcharToCharMem (amApn, MAX_DATA_CONN_APN_SIZE, \
			 stGenDataConn.uDataConnType.stGPRSDataConn.acAPN) ;
		 #if 0
		 jc_strncpy (stGenDataConn.acUserName, profCont->username, \
		 	MAX_DATA_CONN_USERNAME_SIZE) ;
		 jc_strncpy (stGenDataConn.acPassword, profCont->password, \
		 	MAX_DATA_CONN_PASSWORD_SIZE) ;
		 #else
		      mmi_dtcnt_get_user_pass(profCont->data_account, \
			 (S8*)stGenDataConn.acUserName, (S8*)stGenDataConn.acPassword, \
		 	MAX_DATA_CONN_PASSWORD_SIZE) ;
		 #endif
#ifdef __MMI_MULTI_SIM__
		mmi_trace(1,"jdd_log: jWap_readUpdateDataConnInfo sim ID is	%d",g_pstJWapAppInfo->simid);

		stGenDataConn.uiConnID = WAP_MMS_UICONNID_SIM1_FLAG << g_pstJWapAppInfo->simid;
		
		stGenDataConn.uiConnID |= WAP_MMS_UICONNID_WAP_FLAG;		
#endif		

		mmi_trace (1, "@@@ APN Set: %s", stGenDataConn.uDataConnType.stGPRSDataConn.acAPN) ;
#if 0  //it has been set by csw 2009-10-26 fengzb
		/* Temporary Fix for Setting DNS server */
		if (!jc_strcasecmp (stGenDataConn.uDataConnType.stGPRSDataConn.acAPN, \
				"airtelgprs.com"))
		{		
			CFW_SetDnsServer ("202.56.250.5", "202.56.250.6") ;
			mmi_trace (1, "@@@ DNS Set for airtelgprs.com") ;
		}
		else if (!jc_strcasecmp (stGenDataConn.uDataConnType.stGPRSDataConn.acAPN, \
				"cmnet"))
		{
			CFW_SetDnsServer ("211.136.17.107", "211.136.17.107") ;
			mmi_trace (1, "@@@ DNS Set for cmnet") ;
		}
#endif		
		if (profCont->homepage_url)
		{
			pmHome = JC_NULL ;
			jdi_CUtilsCharToTchar (NULL, profCont->homepage_url, &pmHome) ;
		}
		
		jWap_updateDataConnSettings (&stProxySett, &stGenDataConn, (JC_CHAR*)pmHome) ;

		if (pmHome)
		{
			jdd_MemFree (pmHome) ;
		}
	 }
	 else
	 {
		 DisplayPopup ((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, \
				 5000, 1); //Last argument is tone id. 1 = ERROR tone
	 }
	 
 }

#if 0
void jWap_initProfiles () 
{
#ifdef __MMI_MULTI_SIM__
	MTPNP_mmi_ph_init();
#else
	mmi_ph_init () ;
#endif
}
#endif /* Guoj delete. It's not used now at 2009-12-3 */

void 	jWap_deInitProfiles ()
{
	mmi_ph_deinit_profiles () ;
}

 
/*****************************************************************************
 * FUNCTION
 *  mmi_jWap_settingsOptions_save_changes
 * DESCRIPTION
 *  This is the entry function for the save edit profile changes confirmation screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_jWap_settingsOptions_save_changes(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        (UI_string_type) GetString(STR_GLOBAL_SAVE),
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);

    SetLeftSoftkeyFunction(jWap_settingsOption_inItemSel_handler, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}

void jWap_settingsOptions_entry_handler ()
{
	U8* guiBuffer ;             
	U8* inputBuffer ;     
	U16 inputBufferSize ; 
	INT	iPos ;
	U16 icon_list[E_WAP_SETT_TOTAL_ITEMS] = {0};
	ST_WAP_SETTINGS	stWapSettInfo ;

	if (g_pstSettOptInfo)
	{
		jc_memset (g_pstSettOptInfo, 0, sizeof (SETTINGS_OPTIONS_INFO)) ;
		jc_memset (&stWapSettInfo, 0, sizeof (ST_WAP_SETTINGS)) ;
		jWap_readFromSettFile (&stWapSettInfo) ;
		
		EntryNewScreen (SCR_ID_WAP_SETTINGS_OPTIONS, NULL, NULL, NULL);
		
		guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_SETTINGS_OPTIONS);
		InitializeCategory57Screen ();
		//jc_memset (wgui_inline_list_items, 0, sizeof (wgui_inline_list_items));
		
		/* Set Timeout Option Items */
		iPos = 0 ;

		switch (stWapSettInfo.stWAPOptions.uiTimeout)
		{
			case 0: /* No Timeout */
				g_pstSettOptInfo->currTOSelItem = 3 ;
				break ;
				
			case 30:
				g_pstSettOptInfo->currTOSelItem = 0 ;
				break ;

			case 100:
				g_pstSettOptInfo->currTOSelItem = 2 ;
				break ;

			case 60:
			default: /* 60s is default */
				g_pstSettOptInfo->currTOSelItem = 1 ;
				break ;
		}
				
		g_pstSettOptInfo->sTimeoutOptLst [iPos++] = GetString (STR_ID_WAP_SETTINGS_30S) ;
		g_pstSettOptInfo->sTimeoutOptLst [iPos++] = GetString (STR_ID_WAP_SETTINGS_60S) ;
		g_pstSettOptInfo->sTimeoutOptLst [iPos++] = GetString (STR_ID_WAP_SETTINGS_100S) ;
		g_pstSettOptInfo->sTimeoutOptLst [iPos++] = GetString (STR_ID_WAP_SETTINGS_NO_TIMEOUT) ;
		SetInlineItemCaption(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_TIMEOUT_CAPTION], \
			(U8 *) GetString(STR_ID_WAP_SETTINGS_TIMEOUT));
		SetInlineItemActivation(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_TIMEOUT_ITEM], \
			INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0) ;
		SetInlineItemSelect(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_TIMEOUT_ITEM],(S32)iPos,
			(U8**)g_pstSettOptInfo->sTimeoutOptLst, 
			(S32 *)&g_pstSettOptInfo->currTOSelItem);
		/************************/

		
		/* Set Graphics Option Items */
		iPos = 0 ;
		
		g_pstSettOptInfo->currGraphicsSelItem = stWapSettInfo.stWAPOptions.bEnableGraphics ;
		g_pstSettOptInfo->sGraphicsOptLst [iPos++] = GetString (STR_GLOBAL_OFF) ;
		g_pstSettOptInfo->sGraphicsOptLst [iPos++] = GetString (STR_GLOBAL_ON) ;
		
		SetInlineItemCaption(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_GRAPHICS_CAPTION], \
			(U8 *) GetString(STR_ID_WAP_SETTINGS_GRAPHICS));
		SetInlineItemActivation(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_GRAPHICS_ITEM], \
			INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
		SetInlineItemSelect(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_GRAPHICS_ITEM],(S32)iPos,
			(U8**)g_pstSettOptInfo->sGraphicsOptLst, 
			(S32 *)&g_pstSettOptInfo->currGraphicsSelItem);
		/************************/


		/* Set Cache Option Items */
		iPos = 0 ;
		g_pstSettOptInfo->currCacheSelItem = stWapSettInfo.stWAPOptions.bEnableCache ;
		g_pstSettOptInfo->sCacheOptLst [iPos++] = GetString (STR_GLOBAL_OFF) ;
		g_pstSettOptInfo->sCacheOptLst [iPos++] = GetString (STR_GLOBAL_ON) ;
		
		SetInlineItemCaption(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_CACHE_CAPTION], \
			(U8 *) GetString(STR_ID_WAP_SETTINGS_CACHE));
		SetInlineItemActivation(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_CACHE_ITEM], \
			INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
		SetInlineItemSelect(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_CACHE_ITEM],(S32)iPos,
			(U8**)g_pstSettOptInfo->sCacheOptLst, 
			(S32 *)&g_pstSettOptInfo->currCacheSelItem);
		/************************/
		

		/* Set Cookie Option Items */
		iPos = 0 ;
		g_pstSettOptInfo->currCookieSelItem = stWapSettInfo.stWAPOptions.bEnableCookie ;
		
		g_pstSettOptInfo->sCookieOptLst [iPos++] = GetString (STR_GLOBAL_OFF) ;
		g_pstSettOptInfo->sCookieOptLst [iPos++] = GetString (STR_GLOBAL_ON) ;
		
		SetInlineItemCaption(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_COOKIE_CAPTION], \
			(U8 *) GetString(STR_ID_WAP_SETTINGS_COOKIE));
		SetInlineItemActivation(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_COOKIE_ITEM], \
			INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
		SetInlineItemSelect(&g_pstSettOptInfo->wgui_inline_list_items[E_WAP_SETT_COOKIE_ITEM],(S32)iPos,
			(U8**)g_pstSettOptInfo->sCookieOptLst, 
			(S32 *)&g_pstSettOptInfo->currCookieSelItem);
		/************************/

		/* Put numbering icons on alternate items */
		icon_list[E_WAP_SETT_TIMEOUT_CAPTION] = gIndexIconsImageList[0];
		icon_list[E_WAP_SETT_GRAPHICS_CAPTION] = gIndexIconsImageList[1];
		icon_list[E_WAP_SETT_CACHE_CAPTION] = gIndexIconsImageList[2];
		icon_list[E_WAP_SETT_COOKIE_CAPTION] = gIndexIconsImageList[3];

		
		inputBuffer = GetCurrNInputBuffer (SCR_ID_WAP_SETTINGS_OPTIONS, &inputBufferSize );	  //added for inline edit history
		if(inputBuffer != NULL)
			SetCategory57Data(g_pstSettOptInfo->wgui_inline_list_items, E_WAP_SETT_TOTAL_ITEMS, inputBuffer);

		//DisableCategory57ScreenDone();
		ShowCategory57Screen(STR_ID_WAP_SETTINGS_OPTIONS,IMG_ID_JATAAYU_WAP_ICON,
	                        STR_GLOBAL_OK, IMG_GLOBAL_OK,
	                        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                        E_WAP_SETT_TOTAL_ITEMS,
	                        (PU16)icon_list,g_pstSettOptInfo->wgui_inline_list_items,0,guiBuffer);

		//ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_GLOBAL_OK) ;
		//SetCategory57LeftSoftkeyFunction (jWap_settingsOption_inItemSel_handler);
		SetCategory57RightSoftkeyFunctions (mmi_jWap_settingsOptions_save_changes, GoBackHistory);
		//SetKeyHandler(jWap_keyEndHandler, KEY_END, KEY_EVENT_DOWN) ;
	}
}


void jWap_dataConn_entry_handler ()
{
	//mmi_ph_init () ;
	//mmi_ph_entry_show_profile_list_menu () ;
	g_pstSettOptInfo->bUpdateConnSett = E_TRUE ;
	entry_activate_profile () ;
}


void jWap_settingsClearCache ()
{
	INT iErr ;
	
	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
	iErr = jWap_clearCache () ;
	if (!iErr)
	{
		DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, \
			1,	2000, 1);
	}
	else
	{
		DisplayPopup ((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, \
			1,	2000, 1);
	}
	DeleteScreenIfPresent(SCR_CONFIRM_SCREEN);
}

void jWap_SettingsClearCache_handler ()
{
	if(g_pstJWapAppInfo->stWapSettings.stWAPOptions.bEnableCache)
	{
		DisplayNewconfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, \
			get_string(STR_GLOBAL_CLEAR), IMG_GLOBAL_QUESTION, 1,jWap_SettingsClearCache_handler) ;
		SetLeftSoftkeyFunction(jWap_settingsClearCache,KEY_EVENT_UP) ;
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP) ;
	}
	else
	{
		DisplayPopup ((PU8)GetString(STR_GLOBAL_NOT_AVAILABLE), IMG_GLOBAL_ERROR, \
				1,	2000, 1);
	}
}

void jWap_settingsClearCookie ()
{
	INT iErr ;
	
	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
	iErr = jWap_clearCookie () ;
	if (!iErr)
	{
		DisplayPopup ((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, \
			1,	2000, 1);
	}
	else
	{
		DisplayPopup ((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, \
			1,	2000, 1);
	}
	DeleteScreenIfPresent(SCR_CONFIRM_SCREEN);
}


void jWap_SettingsClearCookie_handler ()
{	
	if(g_pstJWapAppInfo->stWapSettings.stWAPOptions.bEnableCache)
	{
		DisplayNewconfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, \
			get_string(STR_GLOBAL_CLEAR), IMG_GLOBAL_QUESTION, 1,jWap_SettingsClearCookie_handler) ;
		SetLeftSoftkeyFunction(jWap_settingsClearCookie, KEY_EVENT_UP) ;
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP) ;
	}
	else
	{
		DisplayPopup ((PU8)GetString(STR_GLOBAL_NOT_AVAILABLE), IMG_GLOBAL_ERROR, \
				1,	2000, 1);
	}

}

#ifdef __MMI_MULTI_SIM__
void jwap_SettingsSelectSimCardEntryHandler()
{
	g_pstSettOptInfo->bUpdateConnSett = E_TRUE ;
//	jWap_initProfiles ();
	MTPNP_jWap_select_sim_entry_handler() ;
}
#endif	
void jWap_settings_highlight_handler (int iSelectIndex)
{
	mmi_trace (1, "@@@ jWap_settings_highlight_handler %d @@@", iSelectIndex) ;
	
	switch (iSelectIndex)
	{
		case 0:
	#ifdef __MMI_MULTI_SIM__
			SetLeftSoftkeyFunction (MTPNP_jwap_edit_ph_select_sim_entry_handler, KEY_EVENT_UP) ;	
	#else
			SetLeftSoftkeyFunction (jWap_dataConn_entry_handler, KEY_EVENT_UP) ;
	#endif
			break ;
			
		case 1:
			SetLeftSoftkeyFunction (jWap_settingsOptions_entry_handler, KEY_EVENT_UP) ;
			break ;
			
		case 2:
			SetLeftSoftkeyFunction (jWap_SettingsClearCache_handler, KEY_EVENT_UP) ;
			break ;

		case 3:
			SetLeftSoftkeyFunction (jWap_SettingsClearCookie_handler, KEY_EVENT_UP) ;
			break ;
	#ifdef __MMI_MULTI_SIM__
		case 4:
			SetLeftSoftkeyFunction (jwap_SettingsSelectSimCardEntryHandler, KEY_EVENT_UP) ;
			break ;	
	#endif			
		default:
			break ; 	
	}
}

void jWap_Opt_settings_highlight_handler(int iSelectIndex)
{
	mmi_trace (1, "@@@ jWap_Opt_settings_highlight_handler %d @@@", iSelectIndex) ;
	
	switch (iSelectIndex)
	{		
		case 0:
			SetLeftSoftkeyFunction (jWap_settingsOptions_entry_handler, KEY_EVENT_UP) ;
			break ;
			
		case 1:
			SetLeftSoftkeyFunction (jWap_SettingsClearCache_handler, KEY_EVENT_UP) ;
			break ;

		case 2:
			SetLeftSoftkeyFunction (jWap_SettingsClearCookie_handler, KEY_EVENT_UP) ;
			break ;
		default:
			break ; 	
	}

}

void jWap_settings_exit_handler ()
{
	mmi_trace(1, "$$$update is called %d",g_pstSettOptInfo->bUpdateConnSett);
	if (g_pstSettOptInfo->bUpdateConnSett)
	{
		jWap_readUpdateDataConnInfo () ;
		g_pstSettOptInfo->bUpdateConnSett = E_FALSE ;
	}

	jWap_deInitProfiles () ;

	if (g_pstSettOptInfo)
	{
		jdd_MemFree (g_pstSettOptInfo) ;
		g_pstSettOptInfo = JC_NULL ;
	}
	GoBackHistory () ;
}


void jWap_settings_entry_handler ()
{
	U8* guiBuffer; 
	U16 setting_main_screen_texts[] = {
		STR_ID_WAP_SETTING_PROFILES, \
		STR_ID_WAP_SETTINGS_OPTIONS,
		STR_ID_WAP_SETTINGS_CLEAR_CACHE,
		STR_ID_WAP_SETTINGS_CLEAR_COOKIE,
#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
		STR_ID_MULTI_SIM_CARD_MENU,
#endif		
		} ;
	U16 setting_opt_screen_texts[] = {
		STR_ID_WAP_SETTINGS_OPTIONS,\
		STR_ID_WAP_SETTINGS_CLEAR_CACHE,
		STR_ID_WAP_SETTINGS_CLEAR_COOKIE,
		} ;
	

	mmi_trace (1, "@@@ jWap_settings_entry_handler @@@") ;

	if (!g_pstSettOptInfo)
	{
		g_pstSettOptInfo = jdd_MemAlloc (1, sizeof (SETTINGS_OPTIONS_INFO)) ;
	}
	
	EntryNewScreen (SCR_ID_WAP_SETTINGS, NULL, jWap_settings_entry_handler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WAP_SETTINGS) ;	
	
	if(IsScreenPresent(SCR_ID_WAP_HOMEPAGE_SCREEN))
	{
		RegisterHighlightHandler (jWap_Opt_settings_highlight_handler) ;
		ShowCategory15Screen(STR_ID_WAP_SETTINGS, IMG_ID_JATAAYU_WAP_ICON, 
							STR_GLOBAL_OK, 0, 
							STR_GLOBAL_BACK, 0,
							(sizeof (setting_opt_screen_texts)/sizeof (U16)), 
							setting_opt_screen_texts, (PU16)gIndexIconsImageList, 
							1, 0, guiBuffer
							 );
	}	
	else
	{		
		RegisterHighlightHandler (jWap_settings_highlight_handler) ;
		ShowCategory15Screen(STR_ID_WAP_SETTINGS, IMG_ID_JATAAYU_WAP_ICON, 
							STR_GLOBAL_OK, 0, 
							STR_GLOBAL_BACK, 0,
							(sizeof (setting_main_screen_texts)/sizeof (U16)), 
							setting_main_screen_texts, (PU16)gIndexIconsImageList, 
							1, 0, guiBuffer
							 );
	}
	SetRightSoftkeyFunction (jWap_settings_exit_handler, KEY_EVENT_UP);
	//SetKeyHandler(jWap_keyEndHandler, KEY_END, KEY_EVENT_DOWN) ;
}


void jwap_freeSettingsInfo ()
{
	if (g_pstSettOptInfo)
	{
		jdd_MemFree (g_pstSettOptInfo) ;
		g_pstSettOptInfo = JC_NULL ;
		jWap_deInitProfiles () ;
	}
}
#endif //__JATAAYU_APP__


