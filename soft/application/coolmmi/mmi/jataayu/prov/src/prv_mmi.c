#define MODULE_NAME "JPRV"
#define FILE_NAME "prv_mmi.c"

/***************************************************************************
 *
 * File Name : prv_mmi.c
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
 * @file		prv_mmi.c
 * @ingroup		provisioning
 * @brief	
 *
 **/

 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

#ifdef MMI_ON_HARDWARE_P
#include "cs_types.h"
#endif

#include "mmi_trace.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "globaldefs.h"
#include "eventsgprot.h"
#include "wgui_status_icons.h"
#include "commonscreens.h"

#include "ddl.h"
#include "jcal.h"
#include "jdi_cutils_tcsstr.h"
#include "jdi_provisionapi.h"
#include "jdi_provisionstructures.h"
#include "jdi_hashalg.h"
#include "prv_defines.h"
#include "prv_platform.h"

#include "prv_main.h"


#include "prv_str_id.h"
#include "settingprofile.h"
#include "jmms_str_id.h"

#include "custom_nvram_editor_data_item.h"
#include "custom_mmi_default_value.h"
#include "nvram_user_defs.h"
#include "jprofilehandlertypes.h"
#include "nvramtype.h"
#include "dataaccountdef.h"
#include "nvramprot.h"
#include "prv_mmi.h"


typedef void (*FPOINTER) (void) ;
extern void GBSetInputboxLSKFunction(FuncPtr f);

ST_CS_DATACONN_PROFILE_NAME gpszDataConnList[MAX_DATA_ACCOUNT_GPRS_LIMIT];
ST_CS_PROXY_PROFILE_NAME gpszProxyProfileList[NVRAM_WAP_PROFILE_MAX];
int giSelectProfileIndex;

//screen1

void prv_MMIDisplayNewProvAlertScreen()
{
	U8* guiBuffer;
	S8  temp[4];
       S8  temp_unicode[8];
	S8  * new_setting_alert__p = NULL;

	mmi_trace(1, "JPRV_LOG:prv_MMIDisplayNewProvAlertScreen invoked");
	EntryNewScreen(SCR_ID_PROV_NEW_ALERT_SCREEN, NULL, prv_MMIDisplayNewProvAlertScreen, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_NEW_ALERT_SCREEN) ;
	
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));

	 new_setting_alert__p = (S8*) GetString(STR_ID_PROV_NEWPROV_ALERT);
	 sprintf(temp, "%d", (gstProvContext.pstProvInfo->nSimID + 1));
        AnsiiToUnicodeString((S8*) temp_unicode, (S8*) temp);
        UCS2Strcpy((S8*) gstProvContext.szScreeBuff, new_setting_alert__p);
        UCS2Strcat((S8*) gstProvContext.szScreeBuff, temp_unicode);
	
	ShowCategory78Screen((U16)STR_ID_PROV_NEW_SETTINGS,JC_NULL,
						STR_GLOBAL_VIEW, JC_NULL,
						STR_GLOBAL_CANCEL, JC_NULL,
						NULL,
						(U8*) gstProvContext.szScreeBuff,
						jc_tcslen(gstProvContext.szScreeBuff),
						guiBuffer);

	SetLeftSoftkeyFunction (prv_ProcessProvInfo,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (prv_MMIDisplayProvDiscardConfirmationScreen, KEY_EVENT_UP);
	SetDelScrnIDCallbackHandler(SCR_ID_PROV_NEW_ALERT_SCREEN,prv_ApplicationExitHandler); 
}

void prv_MMIDisplayProvDiscardConfirmationScreen()
{
	U8* guiBuffer;

	mmi_trace(1, "JPRV_LOG:prv_MMIDisplayDiscardConfirmationScreen invoked");
	EntryNewScreen(SCR_ID_PROV_DISCARD_CONFIRM_SCREEN, NULL, prv_MMIDisplayProvDiscardConfirmationScreen, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_DISCARD_CONFIRM_SCREEN) ;
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));
	jc_tcsncpy(gstProvContext.szScreeBuff,(JC_CHAR*)GetString(STR_ID_PROV_DISCARD_CONFIRMATION),PROV_MAX_SCREEN_BUFF-1 );
	ShowCategory78Screen(STR_ID_PROV_TITLE,JC_NULL,
						STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO,
						NULL,
						(U8*) gstProvContext.szScreeBuff,
						jc_tcslen(gstProvContext.szScreeBuff),
						NULL);
	SetLeftSoftkeyFunction (prv_MMIDisplayIDLEScreen,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);	
}

void prv_MMIDisplayNewSettingsSummaryScreen()
{
	U8* guiBuffer; 

	mmi_trace(1, "JPRV_LOG:prv_MMIDisplayNewSettingsSummaryScreen invoked");
	EntryNewScreen(SCR_ID_PROV_NEWSETTINGS_SUMMARY_SCREEN, NULL, prv_MMIDisplayNewSettingsSummaryScreen, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_NEWSETTINGS_SUMMARY_SCREEN) ;
	prv_FormNewSettingsSummaryList();
	ShowCategory78Screen(STR_ID_PROV_NEW_SETTINGS,JC_NULL,
						STR_GLOBAL_SAVE, IMG_GLOBAL_YES,
						STR_GLOBAL_CANCEL, IMG_GLOBAL_NO,
						NULL,
						(U8*) gstProvContext.szScreeBuff,
						jc_tcslen(gstProvContext.szScreeBuff),
						guiBuffer);
	SetLeftSoftkeyFunction (prv_MMIDisplayProvProfileDetails,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (prv_MMIDisplayProvDiscardConfirmationScreen, KEY_EVENT_UP);	
}


void prv_MMIDisplayContProvProfDetails()
{
	U8* guiBuffer; 

	mmi_trace(1, "JPRV_LOG:prv_MMIDisplayContProvProfDetails invoked");
	EntryNewScreen(SCR_ID_PROV_CONT_DETAILS_SCREEN,NULL, prv_MMIDisplayContProvProfDetails, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_CONT_DETAILS_SCREEN) ;	

	prv_FormContinousProfDeatilsBuff();
	ShowCategory78Screen((U16)STR_ID_PROV_INSTALL_SETTINGS,JC_NULL,
						STR_ID_PROV_DOWNLOAD, JC_NULL, // add a resoucre for download string
						STR_ID_PROV_DISCARD, JC_NULL,
						NULL,
						(U8*) gstProvContext.szScreeBuff,
						jc_tcslen(gstProvContext.szScreeBuff),
						guiBuffer);
	SetLeftSoftkeyFunction (prv_HandleDownloadContinousProvData ,KEY_EVENT_UP) ;
		
	SetRightSoftkeyFunction (prv_MMIDisplayContDiscardConfirmationScreen, KEY_EVENT_UP);
}

void prv_MMIDisplayContDiscardConfirmationScreen()
{
	U8* guiBuffer; 

	mmi_trace(1, "JPRV_LOG:prv_MMIDisplayDiscardConfirmationScreen invoked");
	EntryNewScreen(SCR_ID_CONTPROV_DISCARD_CONFIRM_SCREEN, NULL, prv_MMIDisplayContDiscardConfirmationScreen, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_CONTPROV_DISCARD_CONFIRM_SCREEN) ;
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));
	jc_tcsncpy(gstProvContext.szScreeBuff,(JC_CHAR*)GetString(STR_ID_PROV_DISCARD_CONFIRMATION),PROV_MAX_SCREEN_BUFF-1 );
	ShowCategory78Screen((U16)STR_ID_PROV_TITLE,JC_NULL,
						STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO,
						NULL,
						(U8*) gstProvContext.szScreeBuff,
						jc_tcslen(gstProvContext.szScreeBuff),
						guiBuffer);
	SetLeftSoftkeyFunction (prv_MMIDisplayIDLEScreen,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);	
}




void prv_DisplayDownloadScreen()
{
	DisplayNewconfirm(JC_NULL, JC_NULL, STR_GLOBAL_ABORT, JC_NULL,
						(U16*)GetString(STR_ID_PROV_DOWNLOAD), IMG_GLOBAL_PROGRESS, WARNING_TONE,prv_DisplayDownloadScreen) ;
		
	SetLeftSoftkeyFunction(JC_NULL,KEY_EVENT_UP) ;
		
		
	SetRightSoftkeyFunction(prv_HandleStopRequest,KEY_EVENT_UP) ;
	SetKeyHandler(prv_ContDownloadEndKeyHandler, KEY_END, KEY_EVENT_DOWN) ;
}


//screen3
void prv_MMIDisplayProvProfileDetails()
{
	U8* guiBuffer; 

	mmi_trace(1, "JPRV_LOG:prv_MMIDisplayProvProfileDetails invoked");
	EntryNewScreen(SCR_ID_PROV_DETAILS_SCREEN,NULL, prv_MMIDisplayProvProfileDetails, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_DETAILS_SCREEN) ;
	prv_FormCSProfDetailsBuff();
	ShowCategory78Screen((U16)STR_ID_PROV_INSTALL_SETTINGS,JC_NULL,
					STR_GLOBAL_SAVE, JC_NULL,
					STR_ID_PROV_DISCARD, JC_NULL,
					NULL,
					(U8*) gstProvContext.szScreeBuff,
					jc_tcslen(gstProvContext.szScreeBuff),
					guiBuffer);
	SetLeftSoftkeyFunction (prv_CheckmaxProfiles,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (prv_MMIDisplayDiscardConfirmationScreen, KEY_EVENT_UP);	
}



//screen 4
void prv_MMIDisplayDiscardConfirmationScreen()
{
	U8* guiBuffer; 

	mmi_trace(1, "JPRV_LOG:prv_MMIDisplayDiscardConfirmationScreen invoked");
	EntryNewScreen(SCR_ID_PROF_DISCARD_CONFIRM_SCREEN, NULL, prv_MMIDisplayDiscardConfirmationScreen, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROF_DISCARD_CONFIRM_SCREEN) ;
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));
	jc_tcsncpy(gstProvContext.szScreeBuff,(JC_CHAR*)GetString(STR_ID_PROV_DISCARD_CONFIRMATION),PROV_MAX_SCREEN_BUFF-1 );
	ShowCategory78Screen((U16)STR_ID_PROV_TITLE,JC_NULL,
						STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO,
						NULL,
						(U8*) gstProvContext.szScreeBuff,
						jc_tcslen(gstProvContext.szScreeBuff),
						guiBuffer);
	SetLeftSoftkeyFunction (prv_DiscardSettings,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (prv_MMIDisplayProvProfileDetails, KEY_EVENT_UP);	
}


//screen 5, 7
void prv_MMIDisplayDiscardSuccessScreen()
{
	U8* guiBuffer; 

	EntryNewScreen(SCR_ID_PROV_DISCARD_SUCCESS_SCREEN, NULL, prv_MMIDisplayDiscardSuccessScreen, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_DISCARD_SUCCESS_SCREEN) ;
	mmi_trace(1, "JPRV_LOG:prv_MMIDisplayDiscardSuccessScreen invoked");
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));
	jc_tcsncpy(gstProvContext.szScreeBuff,(JC_CHAR*)GetString(STR_ID_PROV_DISCARD_SUCC_MSG),PROV_MAX_SCREEN_BUFF-1 );
	ShowCategory78Screen((U16)STR_ID_PROV_TITLE,JC_NULL,
						STR_GLOBAL_OK, JC_NULL,
						JC_NULL, JC_NULL,
						NULL,
						(U8*) gstProvContext.szScreeBuff,
						jc_tcslen(gstProvContext.szScreeBuff),
						guiBuffer);
	SetLeftSoftkeyFunction (prv_ShowNextCSProfile,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (NULL, KEY_EVENT_UP);	
}

//screen 6
void prv_MMIDisplaySaveConfirmationScreen()
{
	U8* guiBuffer;
	EntryNewScreen(SCR_ID_PROV_SAVE_CONFIRM_SCREEN, NULL, prv_MMIDisplaySaveConfirmationScreen, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_SAVE_CONFIRM_SCREEN) ;
	
	mmi_trace(1, "JPRV_LOG:prv_MMIDisplaySaveConfirmationScreen invoked");
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));
	jc_tcsncpy(gstProvContext.szScreeBuff,(JC_CHAR*)GetString(STR_ID_PROV_SAVE_CONFIRMATION),PROV_MAX_SCREEN_BUFF-1 );
	ShowCategory78Screen((U16)STR_ID_PROV_TITLE,JC_NULL,
						STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO,
						NULL,
						(U8*) gstProvContext.szScreeBuff,
						jc_tcslen(gstProvContext.szScreeBuff),
						guiBuffer);
	SetLeftSoftkeyFunction (prv_SaveSettings,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (prv_MMIDisplayDiscardConfirmationScreen, KEY_EVENT_UP);	
}

void prv_MMIDisplayReplaceConfirmationScreen()
{
	U8* guiBuffer;
	EntryNewScreen(SCR_ID_PROV_REPLACE_CONFIRM_SCREEN, NULL, prv_MMIDisplayReplaceConfirmationScreen, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_REPLACE_CONFIRM_SCREEN) ;
	
	mmi_trace(1, "JPRV_LOG:prv_MMIDisplayReplaceConfirmationScreen invoked");
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));
	jc_tcsncpy(gstProvContext.szScreeBuff,(JC_CHAR*)GetString(STR_ID_PROV_REPLACE_CONFIRMATION),PROV_MAX_SCREEN_BUFF-1 );
	ShowCategory78Screen((U16)STR_ID_PROV_TITLE,JC_NULL,
						STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO,
						NULL,
						(U8*) gstProvContext.szScreeBuff,
						jc_tcslen(gstProvContext.szScreeBuff),
						guiBuffer);
	SetLeftSoftkeyFunction (prv_ReplaceSettings,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (prv_MMIDisplayDiscardConfirmationScreen, KEY_EVENT_UP);	
}

//screen 8
void prv_MMIDisplaySaveSuccessScreen()
{
	U8* guiBuffer;

	mmi_trace(1, "JPRV_LOG:prv_MMIDisplaySaveSuccessScreen invoked");
	EntryNewScreen(SCR_ID_PROV_SAVE_SUCCESS_SCREEN, NULL, prv_MMIDisplaySaveSuccessScreen, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_SAVE_SUCCESS_SCREEN) ;
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));
	jc_tcsncpy(gstProvContext.szScreeBuff,(JC_CHAR*)GetString(STR_ID_PROV_SAVE_SUCC_MSG),PROV_MAX_SCREEN_BUFF-1 );
	ShowCategory78Screen((U16)STR_ID_PROV_TITLE,JC_NULL,
						STR_GLOBAL_OK, JC_NULL,
						JC_NULL, JC_NULL,
						NULL,
						(U8*) gstProvContext.szScreeBuff,
						jc_tcslen(gstProvContext.szScreeBuff),
						guiBuffer);
	SetLeftSoftkeyFunction (prv_ShowNextCSProfile,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (NULL, KEY_EVENT_UP);	
}



void prv_MMIDisplayProvFailScreen()
{
	U8* guiBuffer;
	
	mmi_trace(1, "JPRV_LOG:prv_MMIDisplayProvFailScreen invoked");
	EntryNewScreen(SCR_ID_PROV_FAIL_SCREEN, NULL, prv_MMIDisplayProvFailScreen, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_FAIL_SCREEN) ;
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));
	jc_tcsncpy(gstProvContext.szScreeBuff,(JC_CHAR*)GetString(STR_ID_PROV_FAIL_MSG),PROV_MAX_SCREEN_BUFF-1 );
	ShowCategory78Screen((U16)STR_ID_PROV_TITLE,JC_NULL,
						STR_GLOBAL_OK, JC_NULL,
						JC_NULL, JC_NULL,
						NULL,
						(U8*) gstProvContext.szScreeBuff,
						jc_tcslen(gstProvContext.szScreeBuff),
						guiBuffer);
	SetLeftSoftkeyFunction (prv_MMIDisplayIDLEScreen,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (NULL, KEY_EVENT_UP);	
}


//screen 2
void prv_MMIDisplayEnterPINScreen()
{
	u8* guiBuffer; 
	S16 pRequired_input_modes [] = {INPUT_MODE_123,
									INPUT_MODE_MULTITAP_UPPERCASE_ABC,
									INPUT_MODE_MULTITAP_LOWERCASE_ABC,
									-1} ;
	mmi_trace(1, "JPRV_LOG:prv_MMIDisplayEnterPINScreen invoked");

	EntryNewScreen (SCR_ID_PROV_USERPIN_SCREEN, NULL, prv_MMIDisplayEnterPINScreen, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_USERPIN_SCREEN) ;

	ShowCategory5Screen_ext(STR_ID_PROV_ENTER_PIN,JC_NULL,
							STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
							STR_GLOBAL_CANCEL, JC_NULL,
							(INPUT_TYPE_NUMERIC|INPUT_TYPE_ALPHANUMERIC_UPPERCASE|INPUT_TYPE_ALPHANUMERIC_LOWERCASE) , 
							(U8 *)gstProvContext.pmPinValue, WAP_MAX_PROFILE_PASSWD_LEN, 
							guiBuffer, 0,
							pRequired_input_modes, NULL);
	
	SetLeftSoftkeyFunction (prv_MMIUserPINLskHandler, KEY_EVENT_UP);	
    #if defined(__MMI_GB__)  
       GBSetInputboxLSKFunction(prv_MMIUserPINLskHandler);
    #endif
	SetCategory5RightSoftkeyFunction(prv_MMIDisplayProvDiscardConfirmationScreen, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistory);
}


void prv_MMIUserPINLskHandler ()
{
	U16 item_texts[] = {STR_GLOBAL_DONE, \
						STR_GLOBAL_INPUT_METHOD} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, } ;
	
	EntryNewScreen (SCR_ID_PROV_USERPIN_OPTIONS_SCREEN, 0, 0, 0) ;
	SetParentHandler (SCR_ID_PROV_USERPIN_SCREEN);
	
	RegisterHighlightHandler (prv_MMIUserPINLskhighlightHanlder);

	ShowCategory52Screen (STR_GLOBAL_OPTIONS, 0, 
							STR_GLOBAL_OK, 0,
							STR_GLOBAL_BACK, 0, 
							2, item_texts, item_icons,
							0, 0, 0, 0);

	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP ) ;
}

void prv_MMIUserPINLskhighlightHanlder (S32 index)
{
	switch(index)
	{
		case 0:
			SetLeftSoftkeyFunction (prv_CheckAuthentication, KEY_EVENT_UP ) ;
			break;
			
		case 1:
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP ) ;
			break ;
	}
	
}
void prv_MMIDisplayNetErrorPopUp()
{
	mmi_trace(1,"JPRV_LOG:prv_MMIDisplayNetErrorPopUp()");
 	DisplayPopup((U8 *)GetString(STR_ID_PROV_NET_ERROR), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
}

void prv_MMIDisplayInvalidSettingsPopUp()
{
	mmi_trace(1,"JPRV_LOG:prv_MMIDisplayNetErrorPopUp()");
 	DisplayPopup((U8 *)GetString(STR_ID_PROV_INVALID_SETTING), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
}

void prv_MMIDisplayIDLEScreen()
{
	prv_DeInitContext();
	mmi_trace(1,"JPRV_LOG:prv_MMIDisplayIDLEScreen()");
	DisplayIdleScreen();
	
}


pBOOL prv_MMIGetProxyProfileItemData(S32 item_index, UI_string_type str_buff_p, PU8 *img_buff_p, U8 str_img_mask)
{    
    if ((item_index < 0) || (item_index > NVRAM_WAP_PROFILE_MAX))
    {
        return FALSE;
    }

    jc_wstrcpy(str_buff_p, (UINT16*)(gpszProxyProfileList[item_index].proxy_id));
    
    *img_buff_p = (PU8) GetImage( (U16)(IMG_GLOBAL_L1 + item_index) );

    return E_TRUE;
}

void prv_MMISelectedProfileIndex(int iSelectIndex)
{
	mmi_trace(1,"JPRV_LOG: get_highlited_id [%d]", iSelectIndex);
	giSelectProfileIndex = iSelectIndex;
}

void prv_MMIShowProxyProfileList()
{
	U8* guiBuffer; 
	
	mmi_trace(1,"JPRV_LOG:prv_MMIShowProfileList()");
	
	EntryNewScreen (SCR_ID_PROV_SHOW_PROXYPROFLIST_SCREEN, NULL, prv_MMIShowProxyProfileList, NULL) ;
	RegisterHighlightHandler (prv_MMISelectedProfileIndex) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_SHOW_PROXYPROFLIST_SCREEN) ;	
	
	ShowCategory184Screen(STR_ID_PROV_REPLACE_PROFILE, JC_NULL,
				        STR_GLOBAL_OK, IMG_GLOBAL_OK,
				        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				        NVRAM_WAP_PROFILE_MAX,
				        prv_MMIGetProxyProfileItemData,
				        0, 0,
				        guiBuffer);
	
	SetLeftSoftkeyFunction (prv_ReplaceProxyProfile, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}

pBOOL prv_MMIGetDataConnItemData(S32 item_index, UI_string_type str_buff_p, PU8 *img_buff_p, U8 str_img_mask)
{    
    if ((item_index < 0) || (item_index > MAX_DATA_ACCOUNT_GPRS_LIMIT))
    {
        return FALSE;
    }

    jc_wstrcpy(str_buff_p, (UINT16*)(gpszDataConnList[item_index].nap_id));
    
    *img_buff_p = (PU8) GetImage( (U16)(IMG_GLOBAL_L1 + item_index) );

    return E_TRUE;
}


void prv_MMIShowDataConnProfileList()
{
	U8* guiBuffer; 
	
	mmi_trace(1,"JPRV_LOG:prv_MMIShowProfileList()");
	
	EntryNewScreen (SCR_ID_PROV_SHOW_DATACONNLIST_SCREEN, NULL, prv_MMIShowDataConnProfileList, NULL) ;
	RegisterHighlightHandler (prv_MMISelectedProfileIndex) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_PROV_SHOW_DATACONNLIST_SCREEN) ;	
	
	ShowCategory184Screen(STR_ID_PROV_REPLACE_PROFILE, JC_NULL,
				        STR_GLOBAL_OK, IMG_GLOBAL_OK,
				        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				        NVRAM_WAP_PROFILE_MAX,
				        prv_MMIGetDataConnItemData,
				        0, 0,
				        guiBuffer);
	
	SetLeftSoftkeyFunction (prv_ReplaceDataConnProfile, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}

