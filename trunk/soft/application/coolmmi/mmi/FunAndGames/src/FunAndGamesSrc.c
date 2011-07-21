/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   FunAndGamesSrc.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Fun And Game source file
 *
 * Author:
 * -------
 * -------
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"
#include "mmi_trace.h"
#define __NEWSIMULATOR 

#include "globaldefs.h"				/* global define */
#include "mainmenudef.h"
#include "custmenures.h"

#include "eventsgprot.h"			/* display screen, history related */
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_inline_edit.h"

#include "funandgamesdefs.h"		/* app header files */
#include "funandgamesprots.h"
#include "gameprots.h"
#include "mmithemes.h"
#include "downloadprotos.h"
#include "composeringtoneprots.h"
#include "voicememoprot.h"
#include "settingprofile.h"		/* success_tone */
#include "settingdefs.h"			/* st_notifcation */
#include "commonscreens.h"			/* popup */
#include "simdetectiondef.h"
#include "simdetectiongexdcl.h"
#include "dual_sim.h"
#if !defined(MMI_ON_WIN32)
#include "javadef.h"
#endif
//#include "wapadp.h" //wufasong delete 20061106

#if defined(__MMI_GAME__)
#include "gameframework.h"
#endif /* __MMI_GAME__ */

#if defined(__MMI_PICTURE_EDITOR_SUPPORT__)
#include "pictureeditorprots.h"
#endif /* __MMI_PICTURE_EDITOR_SUPPORT__ */

#if defined(__J2ME__)
#if 0//JiaShuo Delete for J2ME
#include "wapadp.h"
#include "wap_ps_struct.h"
#include "jam_msg_handler.h"
extern void InitJAVA(void);
#endif
#endif /* !__J2ME__ */	

#if defined(__MMI_RING_COMPOSER__) && defined(__MMI_IMELODY_SUPPORT__)
#include "composeringtoneprots.h"
#endif	/* __MMI_RING_COMPOSER__ && __MMI_IMELODY_SUPPORT__ */ 

#if defined(__MMI_VOICE_MEMO__)
#include "voicememoprot.h"
#endif /* __MMI_VOICE_MEMO__ */

#if defined(__MMI_AUDIO_PLAYER__)
#include "mmi_data_types.h"	/* dependent headers for audioplayerprot.h" */
#include "mdi_datatype.h"		/* dependent headers for audioplayerprot.h" */
#include "audioplayerprot.h"
#endif	/* __MMI_AUDIO_PLAYER__ */

#if defined(__MMI_SOUND_RECORDER__)
#include "soundrecorderprot.h"
#endif	/* __MMI_SOUND_RECORDER__ */

#if defined(__MMI_CAMERA__)
    #include "mmi_features_camera.h"
	#include "gdi_include.h"

	#include "mdi_datatype.h"
	#include "mdi_camera.h"
	#include "cameraapp.h"
	#include "filemgr.h"				/* file management */
	#include "imageviewer.h"
    #include "cameraresdefs.h"
#endif	/* __MMI_CAMERA__ */

#if defined(__MMI_VIDEO__)
#include "vdoplygprot.h"
#include "vdorecgprot.h"
#endif	/* __MMI_VIDEO__ */

#if defined(__MMI_PHOTOEDITOR__)
#include "photoeditorgprot.h"
#endif /* __MMI_PHOTOEDITOR__ */


#ifdef __MMI_FM_RADIO__
#include "fmradioprot.h"
#ifdef __MMI_FM_RADIO_CHECK_CHIP_OR_HIDE__
extern void mmi_fmrdo_skip_highlight_hdlr( S32 index ); // defined in FMRadioSrc.c
extern U16 mmi_fmrdo_check_chip_or_hide( U16 n_items, U16* string_ids, U16* icon_ids ); // defined in FMRadioSrc.c
#endif
#endif

#if defined(__MMI_PHB_CALLER_RES_SETTING__)
//#include "messagesexdcl.h"
//#include "simdetectiongexdcl.h"
#endif

#ifdef __MMI_AB_REPEATER__
#include "abrepeaterprot.h"
#endif

#if defined (__J2ME__)
#include "mtpnp_ad_resdef.h"
#endif

#include "imageviewerresdefs.h"
#ifdef __GPRS_MODE__
#include "dataaccountgprot.h"
#include "dataaccount.h"
#include "dataaccountenum.h"
#include "dataaccountdef.h"
#include "dataaccountstruct.h"
#endif

/***************************************************************************** 
* Constance
*****************************************************************************/
#define MAX_MEMORY_SIZE 16

/***************************************************************************** 
* Struct
*****************************************************************************/

#if defined(__MMI_GAME__) || defined (__J2ME__)

typedef enum {
#if defined(__MMI_GAME__)
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
	FNG_SETTING_GAME_BACKGROUND,
	//FNG_SETTING_GAME_SOUND_EFFECTS,
#else	///__MMI_GAME_MULTICHANNEL_SOUND__
	FNG_SETTING_GAME_AUD,
#endif //__MMI_GAME_MULTICHANNEL_SOUND__
	//FNG_SETTING_GAME_VIB,
#endif /* __MMI_GAME__ */
	FNG_SETTING_GAME_ITEM_COUNT
} fng_setting_game_inline_enum;

typedef enum {
#if defined(__J2ME__)
	FNG_SETTING_JAVA_AUD,
	FNG_SETTING_JAVA_VIB,
#ifdef __MMI_MULTI_SIM__
    FNG_SETTING_JAVA_SIM,
#endif
#if 0//JiaShuo Delete for J2ME
    FNG_SETTING_JAVA_PROF,
	FNG_SETTING_JAVA_STORAGE,
	FNG_SETTING_JAVA_HEAP,
#endif
#ifdef __NATIVE_JAVA__
	FNG_SETTING_JAVA_STORAGEPATH,
#endif
#endif /* __J2ME__ */
	FNG_SETTING_JAVA_ITEM_COUNT
} fng_setting_java_inline_enum;

typedef struct{

	/* strings for inline selection display */
#if defined(__MMI_GAME__)
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
	UI_string_type   game_background_music_str[2];	
	UI_string_type   game_sound_effect_str[2];
#else// __MMI_GAME_MULTICHANNEL_SOUND__
	UI_string_type game_aud_str[2];
#endif//__MMI_GAME_MULTICHANNEL_SOUND__
	UI_string_type game_vib_str[2];

#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
	S32				game_background_music_selected;
	S32				game_sound_effect_selected;
#else// __MMI_GAME_MULTICHANNEL_SOUND__
	S32				game_aud_selected;
#endif//__MMI_GAME_MULTICHANNEL_SOUND__
	S32				game_vib_selected;

	/* inline selection item */
	InlineItem	inline_game_items[FNG_SETTING_GAME_ITEM_COUNT*2];  /* one for caption, one for selection */

#endif /* __MMI_GAME__ */
	U8 data_account;

#if defined(__J2ME__)
	#define MAX_WAP_PROF_SUM 16
	#define WAP_PROF_MAX_NAME_SIZE 40
	
	UI_string_type java_aud_str[7];
	S32 java_aud_selected;

	UI_string_type java_vib_str[2];
	S32 java_vib_selected;
    
#ifdef __MMI_MULTI_SIM__
        UI_string_type java_sim_str[MMI_SIM_NUMBER];
        S32 java_sim_selected;
#endif

#ifdef __NATIVE_JAVA__
        UI_string_type java_storagePath_str[2];
        S32 java_storagePath_selected;
#endif
	U8 java_prof_id[MAX_WAP_PROF_SUM+1]; // disable + profiles
	UI_string_type java_prof_str[MAX_WAP_PROF_SUM+1]; // disable + profiles
	S32			 java_prof_selected;
	S32			java_prof_sum;
	UI_character_type java_prof_strs[MAX_WAP_PROF_SUM][WAP_PROF_MAX_NAME_SIZE];

	UI_string_type java_storage_str[1];
	S32			 java_storage_selected;
	UI_character_type java_storage_strs[1][MAX_MEMORY_SIZE];

	UI_string_type java_heap_str[1];
	S32			 java_heap_selected;
	UI_character_type java_heap_strs[1][MAX_MEMORY_SIZE];

	/* inline selection item */
	InlineItem	inline_java_items[FNG_SETTING_JAVA_ITEM_COUNT*2];  /* one for caption, one for selection */

#endif /* __J2ME__ */
}  fng_setting_inline_data_struct;


typedef struct{
	fng_setting_inline_data_struct setting_inline;
} fng_cntx_struct;

typedef enum
{
    JAVA_SETTINGS_AUD_STR = 0,
    JAVA_SETTINGS_AUD,
    JAVA_SETTINGS_VIB_STR,
    JAVA_SETTINGS_VIB,
    JAVA_SETTINGS_SIM_STR,
    JAVA_SETTINGS_SIM,
    #ifdef __NATIVE_JAVA__
    JAVA_SETTINGS_STORAGE_STR,
    JAVA_SETTINGS_STORAGE,
    #endif
    JAVA_SETTINGS_PROF_STR,
    JAVA_SETTINGS_PROF,
} java_settings_inline_item_enum;


#endif /* __MMI_GAME__ || __J2ME__ */

/***************************************************************************** 
* Global Variable
*****************************************************************************/
extern wgui_inline_item wgui_inline_items[];
extern pwr_context_struct g_pwr_context[];
#ifdef __NATIVE_JAVA__
extern U8 g_mmi_java_storagepath ;
#endif

/***************************************************************************** 
* Local Variable
*****************************************************************************/
#if defined(__MMI_GAME__) || defined (__J2ME__)
fng_cntx_struct g_fng_cntx;
#endif /* __MMI_GAME__ || __J2ME__ */


/***************************************************************************** 
* Local Function
*****************************************************************************/
void mmi_fng_highlight_setting_game(void);
void mmi_fng_highlight_setting_java(void);
void mmi_fng_exit_setting_game_screen(void);
void mmi_fng_exit_setting_java_screen(void);
void mmi_fng_entry_setting_game_screen(void);
void mmi_fng_entry_setting_java_screen(void);
void mmi_fng_real_entry_setting_java_screen(void);
void mmi_fng_setting_game_done_hdlr(void);
void mmi_fng_setting_java_done_hdlr(void);
#ifdef __NATIVE_JAVA__
unsigned char gcj_IsJavaOn();
#endif
#ifdef __MMI_GPRS_FEATURES__
void mmi_fng_java_data_acct(void);
void mmi_fng_java_data_acct_callback(U16 index);
#endif
/***************************************************************************** 
* External Function
*****************************************************************************/
extern kal_uint32 get_mids_avail_storage(void);
extern kal_uint32 get_j2me_heap_size(void);
extern void display_j2me_is_busy(void);
extern kal_bool check_and_display_j2me_is_busy(void);
extern kal_bool j2me_is_no_sim(void);
#if 1
extern S32 GetChildMenuIDIndexByParentMenuID (U16 parentMenuId, U16 childMenuId);
/**************************************************************

	FUNCTION NAME		: mmi_fng_entry_screen

  	PURPOSE				: entry FunAndGame menu

	INPUT PARAMETERS	: nil
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
extern pBOOL mmi_bootup_get_active_flight_mode(void);
void mmi_fng_entry_screen(void)
{
#ifdef WIN32 
	U16	ItemList[MAX_SUB_MENUS]; /* add 2 due to Java and Java Setting menu are added into resource but not in PC environment */
	U16	ItemIcons[MAX_SUB_MENUS];
#else 
   	U16	ItemList[MAX_SUB_MENUS];
   	U16	ItemIcons[MAX_SUB_MENUS];
#endif /* WIN32 */
	U16	nItems;
	U8		*guiBuffer;
	
	EntryNewScreen(SCR3000_FUNANDGAMES, NULL, mmi_fng_entry_screen, NULL);

#if defined(__MMI_MAINLCD_128X160__) || defined(__MMI_MAINLCD_160X128__)||defined (__MMI_MAINLCD_128X128__)
	if(mmi_bootup_get_active_flight_mode() == 1)
	{
		mmi_frm_hide_menu_item(MAIN_MENU_SERVICES_MENUID);
	}
	else
	{
		if(MTPNP_AD_Get_UsableSide_Number() == 0)
		{
			mmi_frm_hide_menu_item(MAIN_MENU_SERVICES_MENUID);
		}
		else
		{
			mmi_frm_unhide_menu_item(MAIN_MENU_SERVICES_MENUID);
		}
	}
#endif

	guiBuffer = GetCurrGuiBuffer(SCR3000_FUNANDGAMES);
	nItems = GetNumOfChild_Ext(MAIN_MENU_FUNANDGAMES_MENUID);
	GetSequenceStringIds_Ext(MAIN_MENU_FUNANDGAMES_MENUID,ItemList);
	GetSequenceImageIds_Ext(MAIN_MENU_FUNANDGAMES_MENUID,ItemIcons);
	SetParentHandler(MAIN_MENU_FUNANDGAMES_MENUID);
#if defined( __MMI_FM_RADIO__ ) && defined( __MMI_FM_RADIO_CHECK_CHIP_OR_HIDE__ )
	RegisterHighlightHandler( mmi_fmrdo_skip_highlight_hdlr );
	nItems = mmi_fmrdo_check_chip_or_hide( nItems, ItemList, ItemIcons );
#else
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
#endif

#if defined(__MMI_PHB_CALLER_RES_SETTING__)
   /* No SIM, remove caller picture menu items */
{
	BOOL isSimValid = MMI_FALSE;
	U16 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].IsSimValid == 1)
		{
			isSimValid = MMI_TRUE;
			break;
		}
	}
   if( !(isSimValid) )
   {
   	S32 MenuItemId;
   	
   	MenuItemId = GetChildMenuIDIndexByParentMenuID(MAIN_MENU_FUNANDGAMES_MENUID,MENU_ID_PHB_CALLER_RING_1);
   	if(MenuItemId != -1)	/*Menu Exists.*/
   	{
	   	nItems-=1;
   	}
   }
}
#endif
 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(MAIN_MENU_FUNANDGAMES_MENUID);
#endif
 

#ifdef __MMI_NUMBER_MENU__
	ShowCategory52Screen(
 
	

								STR_SCR3000_FUNANDGAMES_CAPTION,

								MAIN_MENU_TITLE_FUNANDGAMES_ICON,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								nItems, ItemList, (PU16)gIndexIconsImageList, 0, 0, 0, guiBuffer	);
#else
	ShowCategory52Screen(
  


								STR_SCR3000_FUNANDGAMES_CAPTION,

								MAIN_MENU_TITLE_FUNANDGAMES_ICON,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								nItems, ItemList, ItemIcons, 0, 0, 0, guiBuffer	);
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
#endif
#if 0


/**************************************************************

	FUNCTION NAME		: mmi_fng_init_app

  	PURPOSE				: FunAndGame intilize funtion for each app and hilite hdlr

	INPUT PARAMETERS	: nil
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_fng_init_app(void)
{
	#if defined(__MMI_THEMES_APPLICATION__)
		InitThemes();
	#endif

	#if defined(__DOWNLOAD__)
		InitDownload();
	#endif

	#if defined(__MMI_VOICE_MEMO__)
		InitVoiceMemoApp();
	#endif /* __MMI_VOICE_MEMO__ */

	#if defined(__MMI_GAME__)
		SetHiliteHandler(MENU3105_GAMES, mmi_game_highlight_app);
	#endif /* __MMI_GAME__ */

	#if defined(__MMI_AUDIO_PLAYER__)
		InitAudioPlayerApp();
	#endif /* __MMI_AUDIO_PLAYER__ */

	#ifdef __MMI_AB_REPEATER__
   		InitABRepeaterApp();
	#endif

	#if defined( __MMI_CAMERA__)
		mmi_camera_init_app();
		mmi_imgview_init_app();
	#endif /* __MMI_CAMERA__ */

	#if defined(__MMI_VIDEO__)
		mmi_vdoply_init_app(); 
		mmi_vdorec_init_app();
	#endif	/* __MMI_VIDEO__ */

	#if defined(__MMI_PHOTOEDITOR__)
		mmi_phoedt_init_app();
	#endif /* __MMI_PHOTOEDITOR__ */

	 
	#ifdef __MMI_FM_RADIO__
		mmi_fmrdo_init_app();
	#endif
	 

	#if defined(__J2ME__)
		SetHiliteHandler(MENU3108_JAVA, Highlight_JAVA_MENU);
		InitJAVA();
	#endif /* __J2ME__ */
		
	#if defined(__MMI_PICTURE_EDITOR_SUPPORT__)
		SetHiliteHandler(MENU_PICTURE_EDITOR, Highlight_MENU_PICTURE_EDITOR);
		InitPictureEditor();
	#endif /* __MMI_PICTURE_EDITOR_SUPPORT__ */

	#if defined(__MMI_GAME__)
		SetHiliteHandler(MENU_ID_FNG_SETTING_GAME, mmi_fng_highlight_setting_game);
	#endif /* __MMI_GAME__ */
	#if defined (__J2ME__)
		SetHiliteHandler(MENU_ID_FNG_SETTING_JAVA, mmi_fng_highlight_setting_java);
	#endif /* __J2ME__ */
}
#endif
/**************************************************************

	FUNCTION NAME		: mmi_fng_setting_game_done_hdlr

  	PURPOSE				: done hdlr for game setting

	INPUT PARAMETERS	: nil
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_fng_setting_game_done_hdlr(void)
{
#if defined(__MMI_GAME__)
	/* game effect */
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
	GFX.is_background_music_on	= g_fng_cntx.setting_inline.game_background_music_selected;
	GFX.is_sound_effect_on	= g_fng_cntx.setting_inline.game_sound_effect_selected;
#else//__MMI_GAME_MULTICHANNEL_SOUND__
	GFX.is_aud_on	= g_fng_cntx.setting_inline.game_aud_selected;
#endif//__MMI_GAME_MULTICHANNEL_SOUND__
	GFX.is_vib_on	= g_fng_cntx.setting_inline.game_vib_selected;
	mmi_gx_store_setting();

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	DeleteNHistory(1);
#endif /* __MMI_GAME__ */
}
/**************************************************************

	FUNCTION NAME		: mmi_fng_setting_java_done_hdlr

  	PURPOSE				: done hdlr for java setting

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_fng_setting_java_done_hdlr(void)
{
#if defined(__J2ME__)

#ifdef __MMI_MULTI_SIM__
    if (!MTPNP_PFAL_Is_Card_Usable(g_fng_cntx.setting_inline.java_sim_selected))
    {
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);		
        return;
    }
#endif

	/* java effect */
	g_mmi_java_aud_volume_level		= g_fng_cntx.setting_inline.java_aud_selected;
	g_mmi_java_is_vib_on		= g_fng_cntx.setting_inline.java_vib_selected;
#ifdef __MMI_MULTI_SIM__
	g_mmi_java_sim_id = g_fng_cntx.setting_inline.java_sim_selected;
#endif
	g_mmi_java_wap_prof_id = g_fng_cntx.setting_inline.data_account;

#ifdef __NATIVE_JAVA__
      
    S16 CardDrv = -1;
     g_mmi_java_storagepath = g_fng_cntx.setting_inline.java_storagePath_selected;
    if(g_mmi_java_storagepath == 1 )
    {
       CardDrv = MMI_FS_GetDrive( FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE );
        if (CardDrv <=0)    /* Card Storage is present */
    	{
	    g_mmi_java_storagepath = g_fng_cntx.setting_inline.java_storagePath_selected = 0;	
		DisplayPopup((PU8)GetString(STR_ID_IMGVIEW_NOTIFY_NO_MEMORY_CARD) ,
			IMG_GLOBAL_PROGRESS, 5,
			UI_POPUP_NOTIFYDURATION_TIME,
			WARNING_TONE);
		DeleteNHistory(1);
		return;
    	}
    }
		
	extern void gcj_init_java_dir();
	gcj_init_java_dir();
#endif
	mmi_java_store_setting();

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	DeleteNHistory(1);
#endif /* __J2ME__ */
}

/**************************************************************

	FUNCTION NAME		: mmi_fng_setting_game_highlight_hdlr

  	PURPOSE				: highlight hdlr

	INPUT PARAMETERS	: S32 index
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_fng_setting_game_highlight_hdlr(S32 index)
{
#if defined(__MMI_GAME__)
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(mmi_fng_setting_game_done_hdlr);
#endif /* __MMI_GAME__ */
}

/**************************************************************

	FUNCTION NAME		: mmi_fng_setting_java_highlight_hdlr

  	PURPOSE				: highlight hdlr

	INPUT PARAMETERS	: S32 index
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_fng_setting_java_highlight_hdlr(S32 index)
{
#if defined (__J2ME__)
    if (index == JAVA_SETTINGS_PROF)
    {
        ChangeLeftSoftkey(STR_GLOBAL_SELECT, 0);
        SetLeftSoftkeyFunction(mmi_fng_java_data_acct, KEY_EVENT_UP);

    }
    else
    {
        ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
        SetCategory57LeftSoftkeyFunction(mmi_fng_setting_java_done_hdlr);
    }
#endif /* __J2ME__ */
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ph_prof_list_data_acct
 * DESCRIPTION
 *  LSK handler of Data Account item in Profile Setting inline editor screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 #ifdef __MMI_GPRS_FEATURES__
void mmi_fng_java_data_acct(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    mmi_dtcnt_select_account_ext(mmi_fng_java_data_acct_callback, NULL, DATA_ACCOUNT_BEARER_GPRS,g_fng_cntx.setting_inline.data_account);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_fng_java_data_acct_callback
 * DESCRIPTION
 *  This is the callback function for Data Account
 * PARAMETERS
 *  index       [IN]        This is the index for Data Account
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_fng_java_data_acct_callback(U16 index)
{
    g_fng_cntx.setting_inline.data_account = index - 10;
    GoBackHistory();
}
#endif
/**************************************************************

	FUNCTION NAME		: mmi_fng_exit_setting_game_screen

  	PURPOSE				: exit point for game setting screen

	INPUT PARAMETERS	: viud
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_fng_exit_setting_game_screen(void)
{
	U16 		inputBufferSize;                
	history_t 	h;
	S16		nHistory = 0;

	h.scrnID = SCR_ID_FNG_SETTING_GAME;
	CloseCategory57Screen();
	h.entryFuncPtr = mmi_fng_entry_setting_game_screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer,(S8*)&nHistory);
	GetCategory57History (h.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data ((U8*)h.inputBuffer);
	AddNHistory(h, inputBufferSize);
	
}
/**************************************************************

	FUNCTION NAME		: mmi_fng_exit_setting_java_screen

  	PURPOSE				: exit point for java setting screen

	INPUT PARAMETERS	: viud
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_fng_exit_setting_java_screen(void)
{
	U16 		inputBufferSize;                
	history_t 	h;
	S16		nHistory = 0;

	h.scrnID = SCR_ID_FNG_SETTING_JAVA;
	CloseCategory57Screen();
	h.entryFuncPtr = mmi_fng_real_entry_setting_java_screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer,(S8*)&nHistory);
	GetCategory57History (h.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data ((U8*)h.inputBuffer);
	AddNHistory(h, inputBufferSize);

}

/**************************************************************

	FUNCTION NAME		: mmi_fng_entry_setting_game_screen

  	PURPOSE				: entry point for game setting screen

	INPUT PARAMETERS	: nil
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
	
void mmi_fng_entry_setting_game_screen(void)
{
#if defined(__MMI_GAME__)

	U8		*guiBuffer;
	U8		*inputBuffer;
	U16	inputBufferSize;
	S32	item_offset = 0;
	
	UI_image_ID_type item_icons[] =
	{
		#if defined( __MMI_GAME__)
		#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
		//	IMG_ID_FNG_SETTING_GAME_BACK_AUD,
		//	0,
			IMG_ID_FNG_SETTING_GAME_AUD,
			0,
		#else//__MMI_GAME_MULTICHANNEL_SOUND__
			IMG_ID_FNG_SETTING_GAME_AUD,
			0,
		#endif//__MMI_GAME_MULTICHANNEL_SOUND__
		//	IMG_ID_FNG_SETTING_GAME_VIB,
		//	0,
		#endif /* __MMI_GAME__ */
	};

	
	/* load setting */
	#if defined( __MMI_GAME__)
		mmi_gx_load_setting();
	#endif /* __MMI_GAME__ */	

	EntryNewScreen(SCR_ID_FNG_SETTING_GAME, mmi_fng_exit_setting_game_screen, NULL, NULL);

	InitializeCategory57Screen();

	/***************************************************************************** 
	* Game  
	*****************************************************************************/
#if defined( __MMI_GAME__)
	/* TRUE (1) is on */
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
	g_fng_cntx.setting_inline.game_background_music_selected = GFX.is_background_music_on;
	g_fng_cntx.setting_inline.game_sound_effect_selected = GFX.is_sound_effect_on;
#else//__MMI_GAME_MULTICHANNEL_SOUND__
	g_fng_cntx.setting_inline.game_aud_selected = GFX.is_aud_on;
#endif//__MMI_GAME_MULTICHANNEL_SOUND__

	g_fng_cntx.setting_inline.game_vib_selected = GFX.is_vib_on;	

#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
	g_fng_cntx.setting_inline.game_background_music_str[0] = (UI_string_type)GetString((U16)(STR_GLOBAL_OFF));
	g_fng_cntx.setting_inline.game_background_music_str[1] = (UI_string_type)GetString((U16)(STR_GLOBAL_ON));
	g_fng_cntx.setting_inline.game_sound_effect_str[0] = (UI_string_type)GetString((U16)(STR_GLOBAL_OFF));
	g_fng_cntx.setting_inline.game_sound_effect_str[1] = (UI_string_type)GetString((U16)(STR_GLOBAL_ON));
#else//__MMI_GAME_MULTICHANNEL_SOUND__
	g_fng_cntx.setting_inline.game_aud_str[0] = (UI_string_type)GetString((U16)(STR_GLOBAL_OFF));
	g_fng_cntx.setting_inline.game_aud_str[1] = (UI_string_type)GetString((U16)(STR_GLOBAL_ON));
#endif//__MMI_GAME_MULTICHANNEL_SOUND__	
	g_fng_cntx.setting_inline.game_vib_str[0] = (UI_string_type)GetString((U16)(STR_GLOBAL_OFF));
	g_fng_cntx.setting_inline.game_vib_str[1] = (UI_string_type)GetString((U16)(STR_GLOBAL_ON));

	memset((void*)g_fng_cntx.setting_inline.inline_game_items, 0, sizeof(InlineItem)*FNG_SETTING_GAME_ITEM_COUNT*2);

#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
	/* background music */
	SetInlineItemCaption(&g_fng_cntx.setting_inline.inline_game_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_FNG_SETTING_GAME_AUD)));

	SetInlineItemActivation(&g_fng_cntx.setting_inline.inline_game_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&g_fng_cntx.setting_inline.inline_game_items[item_offset++],
								2,
								(U8**)g_fng_cntx.setting_inline.game_background_music_str,
								&g_fng_cntx.setting_inline.game_background_music_selected);

	/*sound effect */
	#if 0
	SetInlineItemCaption(&g_fng_cntx.setting_inline.inline_game_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_FNG_SETTING_MULTICHANNEL_SOUND_EFFECT)));

	SetInlineItemActivation(&g_fng_cntx.setting_inline.inline_game_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&g_fng_cntx.setting_inline.inline_game_items[item_offset++],
								2,
								(U8**)g_fng_cntx.setting_inline.game_sound_effect_str,
								&g_fng_cntx.setting_inline.game_sound_effect_selected);
	#endif

#else//__MMI_GAME_MULTICHANNEL_SOUND__
	/* game audio */
	SetInlineItemCaption(&g_fng_cntx.setting_inline.inline_game_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_FNG_SETTING_GAME_AUD)));

	SetInlineItemActivation(&g_fng_cntx.setting_inline.inline_game_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&g_fng_cntx.setting_inline.inline_game_items[item_offset++],
								2,
								(U8**)g_fng_cntx.setting_inline.game_aud_str,
								&g_fng_cntx.setting_inline.game_aud_selected);

#endif//__MMI_GAME_MULTICHANNEL_SOUND__
	/* game vib */
#if 0
	SetInlineItemCaption(&g_fng_cntx.setting_inline.inline_game_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_FNG_SETTING_GAME_VIB)));

	SetInlineItemActivation(&g_fng_cntx.setting_inline.inline_game_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&g_fng_cntx.setting_inline.inline_game_items[item_offset++],
								2,
								(U8**)g_fng_cntx.setting_inline.game_vib_str,
								&g_fng_cntx.setting_inline.game_vib_selected);
#endif	

#endif /* __MMI_GAME__ */
	
	guiBuffer	= GetCurrGuiBuffer(SCR_ID_FNG_SETTING_GAME);
	inputBuffer = GetCurrNInputBuffer(SCR_ID_FNG_SETTING_GAME, &inputBufferSize );

	if(inputBuffer != NULL)		
		SetCategory57Data(g_fng_cntx.setting_inline.inline_game_items, FNG_SETTING_GAME_ITEM_COUNT*2, inputBuffer);	

	RegisterHighlightHandler(mmi_fng_setting_game_highlight_hdlr);

	/* show category */
	ShowCategory57Screen(STR_ID_FNG_SETTING_GAME, MAIN_MENU_TITLE_FUNANDGAMES_ICON,		
								STR_GLOBAL_OK, IMG_GLOBAL_OK,											
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,									
								item_offset,
								item_icons, 
								g_fng_cntx.setting_inline.inline_game_items,
								0, guiBuffer);

	/* set key hdlr */
	SetCategory57LeftSoftkeyFunction(mmi_fng_setting_game_done_hdlr);
	SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
	DisableCategory57ScreenDone();	
#endif /* __MMI_GAME__ */
}
#if 0
/**************************************************************

	FUNCTION NAME		: mmi_fng_recv_wap_prof_list_rsp

  	PURPOSE				: response handler of recv_wap_prof_list_req

	INPUT PARAMETERS	: void* MSG
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_fng_recv_wap_prof_list_rsp(void * MSG)
{
#ifdef __J2ME__	
	S32 i;
	wap_get_prof_list_rsp_struct *msg = (wap_get_prof_list_rsp_struct*) MSG;
	if(!j2me_is_no_sim() && msg->result != WAP_PROF_SUCCESS)
	{
		display_j2me_is_busy();
		return;
	}
	g_fng_cntx.setting_inline.java_prof_sum = msg->number;
	for(i=0;i<g_fng_cntx.setting_inline.java_prof_sum && i<MAX_WAP_PROF_SUM;i++)
	{
		memcpy(	g_fng_cntx.setting_inline.java_prof_strs[i],
					msg->prof_list[i].name.value,
					msg->prof_list[i].name.len);

		g_fng_cntx.setting_inline.java_prof_strs[i][msg->prof_list[i].name.len/2] = L'\0';
		g_fng_cntx.setting_inline.java_prof_str[i] = g_fng_cntx.setting_inline.java_prof_strs[i];
		g_fng_cntx.setting_inline.java_prof_id[i]  = msg->prof_list[i].id;
	}
	if(msg->prof_list)
		OslMfree(msg->prof_list);
	
	mmi_fng_real_entry_setting_java_screen();
#endif
}
#endif
/**************************************************************

	FUNCTION NAME		: mmi_fng_entry_setting_java_screen

  	PURPOSE				: entry point for java setting screen

	INPUT PARAMETERS	: nil
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_fng_entry_setting_java_screen(void)
{
#if 0//JiaShuo Modify for J2ME
#ifdef __J2ME__
	MYQUEUE   Message;

	SetProtocolEventHandler(mmi_fng_recv_wap_prof_list_rsp,MSG_ID_WAP_GET_PROF_LIST_RSP);

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_WAP;
	Message.oslMsgId = MSG_ID_WAP_GET_PROF_LIST_REQ;
	Message.oslDataPtr = (local_para_struct *) NULL;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
#endif
#else
#ifdef __J2ME__
    g_fng_cntx.setting_inline.data_account = g_mmi_java_wap_prof_id;
    mmi_fng_real_entry_setting_java_screen();
#endif
#endif
	
}

/**************************************************************

	FUNCTION NAME		: mmi_fng_real_entry_setting_java_screen

  	PURPOSE				: real entry point for java setting screen

	INPUT PARAMETERS	: nil
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
#ifdef __MMI_GPRS_FEATURES__

extern GPRSDataAccountNode g_GPRS_DataAccount[];

#endif /* __MMI_GPRS_FEATURES__ */ 

void mmi_fng_real_entry_setting_java_screen(void)
{
#if defined (__J2ME__)

	U8		*guiBuffer;
	U8		*inputBuffer;
	U16	inputBufferSize;
	S32	item_offset = 0;
#if 0//JiaShuo Delete for J2ME
	S8 javaTempString[2][MAX_MEMORY_SIZE];
#endif
#ifdef __MMI_MULTI_SIM__
	U8 i;
#endif

	UI_image_ID_type item_icons[] =
	{
			IMG_ID_FNG_SETTING_JAVA_AUD,
			0,
			IMG_ID_FNG_SETTING_JAVA_VIB,
			0,
			IMG_ID_FNG_SETTING_JAVA_PROF_ID,//select sim
			0,
#if 0//JiaShuo Delete for J2ME
			IMG_ID_FNG_SETTING_JAVA_STORAGE,
			0,
			IMG_ID_FNG_SETTING_JAVA_HEAP,
			0,
#endif
#ifdef __NATIVE_JAVA__
			IMG_ID_CAMERA_STORAGE,
			0,
#endif	
			IMG_ID_FNG_SETTING_JAVA_PROF_ID,
			0,
	};

#if 0//JiaShuo Delete for J2ME
	if(check_and_display_j2me_is_busy())
		return;
#endif
	
	/* load setting */
	mmi_java_load_setting();

	EntryNewScreen(SCR_ID_FNG_SETTING_JAVA, mmi_fng_exit_setting_java_screen, NULL, NULL);
	
	/***************************************************************************** 
	* JAVA  
	*****************************************************************************/
	/* JAVA TODO: Set value here */

	g_fng_cntx.setting_inline.java_aud_selected = g_mmi_java_aud_volume_level;
	g_fng_cntx.setting_inline.java_vib_selected = g_mmi_java_is_vib_on;
#ifdef __MMI_MULTI_SIM__
    g_fng_cntx.setting_inline.java_sim_selected = g_mmi_java_sim_id;
#endif
#ifdef __NATIVE_JAVA__

	g_fng_cntx.setting_inline.java_storagePath_selected = g_mmi_java_storagepath;
#endif
#if 0//JiaShuo Delete for J2ME
	g_fng_cntx.setting_inline.java_storage_selected = 0;
	g_fng_cntx.setting_inline.java_heap_selected = 0;
#endif

	g_fng_cntx.setting_inline.java_aud_str[0] = (UI_string_type)GetString((U16)(STR_GLOBAL_OFF));
	g_fng_cntx.setting_inline.java_aud_str[1] = (UI_string_type)L"1";
	g_fng_cntx.setting_inline.java_aud_str[2] = (UI_string_type)L"2";
	g_fng_cntx.setting_inline.java_aud_str[3] = (UI_string_type)L"3";
	g_fng_cntx.setting_inline.java_aud_str[4] = (UI_string_type)L"4";
	g_fng_cntx.setting_inline.java_aud_str[5] = (UI_string_type)L"5";
	g_fng_cntx.setting_inline.java_aud_str[6] = (UI_string_type)L"6";

	g_fng_cntx.setting_inline.java_vib_str[0] = (UI_string_type)GetString((U16)(STR_GLOBAL_OFF));
	g_fng_cntx.setting_inline.java_vib_str[1] = (UI_string_type)GetString((U16)(STR_GLOBAL_ON));

#ifdef __MMI_MULTI_SIM__
    for(i = 0; i < MMI_SIM_NUMBER; i++)
    {
        g_fng_cntx.setting_inline.java_sim_str[i] = (UI_string_type) GetString((U16) STRING_MTPNP_SIM1 + i);
    }
#endif

#ifdef __NATIVE_JAVA__
        g_fng_cntx.setting_inline.java_storagePath_str[0] = (UI_string_type) GetString(STR_ID_CAMERA_STORAGE_PHONE );
        g_fng_cntx.setting_inline.java_storagePath_str[1] = (UI_string_type) GetString(STR_ID_CAMERA_STORAGE_MEMORY_CARD );

#endif

#if 0//JiaShuo Delete for J2ME
	memset(javaTempString[0], 0, sizeof(javaTempString[0]));
	sprintf(javaTempString[0], "%8u ", get_mids_avail_storage());
	AnsiiToUnicodeString((S8*)g_fng_cntx.setting_inline.java_storage_strs[0],(S8*)javaTempString[0]);
	UCS2Strcpy((S8*)g_fng_cntx.setting_inline.java_storage_strs[0]+18, GetString((U16)STR_JAVA_INFO_BYTE));
	g_fng_cntx.setting_inline.java_storage_str[0] = g_fng_cntx.setting_inline.java_storage_strs[0];

	memset(javaTempString[1], 0, sizeof(javaTempString[1]));
	sprintf(javaTempString[1], "%8u ", get_j2me_heap_size());
	AnsiiToUnicodeString((S8*)g_fng_cntx.setting_inline.java_heap_strs[0],(S8*)javaTempString[1]);
	UCS2Strcpy((S8*)g_fng_cntx.setting_inline.java_heap_strs[0]+18, GetString((U16)STR_JAVA_INFO_BYTE));
	g_fng_cntx.setting_inline.java_heap_str[0] = g_fng_cntx.setting_inline.java_heap_strs[0];
#endif

	memset((void*)g_fng_cntx.setting_inline.inline_java_items, 0, sizeof(InlineItem)*FNG_SETTING_JAVA_ITEM_COUNT*2);

	/* java audio */
	SetInlineItemCaption(&g_fng_cntx.setting_inline.inline_java_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_FNG_SETTING_JAVA_AUD)));

	SetInlineItemActivation(&g_fng_cntx.setting_inline.inline_java_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&g_fng_cntx.setting_inline.inline_java_items[item_offset++],
								7,
								(U8**)g_fng_cntx.setting_inline.java_aud_str,
								&g_fng_cntx.setting_inline.java_aud_selected);

	/* java vibration */
	SetInlineItemCaption(&g_fng_cntx.setting_inline.inline_java_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_FNG_SETTING_JAVA_VIB)));

	SetInlineItemActivation(&g_fng_cntx.setting_inline.inline_java_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&g_fng_cntx.setting_inline.inline_java_items[item_offset++],
								2,
								(U8**)g_fng_cntx.setting_inline.java_vib_str,
								&g_fng_cntx.setting_inline.java_vib_selected);
#ifdef __MMI_MULTI_SIM__
    if (!mmi_java_is_no_network_service())
    {
        SetInlineItemCaption(
            &g_fng_cntx.setting_inline.inline_java_items[item_offset++],
            (PU8) GetString((U16) (STR_ID_MULTI_SIM_CARD_MENU)));

        SetInlineItemActivation(
            &g_fng_cntx.setting_inline.inline_java_items[item_offset],
            INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
            0);

        SetInlineItemSelect(
            &g_fng_cntx.setting_inline.inline_java_items[item_offset++],
            MMI_SIM_NUMBER,
            (U8 **) g_fng_cntx.setting_inline.java_sim_str,
            &g_fng_cntx.setting_inline.java_sim_selected);
    }    
#endif

#ifdef __NATIVE_JAVA__
   if (!gcj_IsJavaOn())
   	{
        SetInlineItemCaption(
            &g_fng_cntx.setting_inline.inline_java_items[item_offset++],
            (PU8) GetString((U16) (STR_ID_CAMERA_STORAGE)));

        SetInlineItemActivation(
            &g_fng_cntx.setting_inline.inline_java_items[item_offset],
            INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
            0);

        SetInlineItemSelect(
            &g_fng_cntx.setting_inline.inline_java_items[item_offset++],
            2,
            (U8 **) g_fng_cntx.setting_inline.java_storagePath_str,
            &g_fng_cntx.setting_inline.java_storagePath_selected);
   	}
#endif

   if (!IsJavaOn())
   {
        SetInlineItemCaption(
            &g_fng_cntx.setting_inline.inline_java_items[item_offset++],
            (PU8) GetString((U16) (STR_ID_DTCNT_CAPTION)));

        SetInlineItemActivation(
            &g_fng_cntx.setting_inline.inline_java_items[item_offset],
            INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
            0);
        
        SetInlineItemDisplayOnly(
            &g_fng_cntx.setting_inline.inline_java_items[item_offset++],
            (U8*) g_GPRS_DataAccount[g_fng_cntx.setting_inline.data_account].DataAccountName);
   }
   
#if 0//JiaShuo Delete for J2ME

	/* java prof */
	if(!j2me_is_no_sim())
	{
	SetInlineItemCaption(&g_fng_cntx.setting_inline.inline_java_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_FNG_SETTING_JAVA_PROF_ID)));

	SetInlineItemActivation(&g_fng_cntx.setting_inline.inline_java_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&g_fng_cntx.setting_inline.inline_java_items[item_offset++],
								g_fng_cntx.setting_inline.java_prof_sum,
								(U8**)g_fng_cntx.setting_inline.java_prof_str,
								&g_fng_cntx.setting_inline.java_prof_selected);	
	}
	/* java storage */
	SetInlineItemCaption(&g_fng_cntx.setting_inline.inline_java_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_FNG_SETTING_JAVA_STORAGE)));

	SetInlineItemActivation(&g_fng_cntx.setting_inline.inline_java_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemDisplayOnly(&g_fng_cntx.setting_inline.inline_java_items[item_offset++],
								(U8*)g_fng_cntx.setting_inline.java_storage_str[0]);

	/* java heap */
	SetInlineItemCaption(&g_fng_cntx.setting_inline.inline_java_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_FNG_SETTING_JAVA_HEAP)));

	SetInlineItemActivation(&g_fng_cntx.setting_inline.inline_java_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemDisplayOnly(&g_fng_cntx.setting_inline.inline_java_items[item_offset++],
								(U8*)g_fng_cntx.setting_inline.java_heap_str[0]);
#endif


	guiBuffer	= GetCurrGuiBuffer(SCR_ID_FNG_SETTING_JAVA);
	inputBuffer = GetCurrNInputBuffer(SCR_ID_FNG_SETTING_JAVA, &inputBufferSize );

	InitializeCategory57Screen();

	if(inputBuffer != NULL)		
		SetCategory57Data(g_fng_cntx.setting_inline.inline_java_items, FNG_SETTING_JAVA_ITEM_COUNT*2, inputBuffer);	

	RegisterHighlightHandler(mmi_fng_setting_java_highlight_hdlr);

	/* show category */
	ShowCategory57Screen(STR_ID_FNG_SETTING_JAVA, MAIN_MENU_TITLE_FUNANDGAMES_ICON,		
								STR_GLOBAL_OK, IMG_GLOBAL_OK,											
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,									
								item_offset,
								item_icons, 
								g_fng_cntx.setting_inline.inline_java_items,
								0, guiBuffer);

	/* set key hdlr */
	if(GetHighlightedItem() == JAVA_SETTINGS_PROF)
	{
	    ChangeLeftSoftkey(STR_GLOBAL_SELECT, 0);
	    SetLeftSoftkeyFunction(mmi_fng_java_data_acct, KEY_EVENT_UP);
	}
	else
	{
	    SetCategory57LeftSoftkeyFunction(mmi_fng_setting_java_done_hdlr);
	}
    
	SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
	DisableCategory57ScreenDone();	
#endif /* __J2ME__ */
}
/**************************************************************

	FUNCTION NAME		: mmi_fng_real_entry_setting_game_screen

  	PURPOSE				: highlight setting game

	INPUT PARAMETERS	: nil
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_fng_highlight_setting_game(void)
{
	TRACE_AUD_FUNCTION();

#if defined(__MMI_GAME__)
	SetLeftSoftkeyFunction(mmi_fng_entry_setting_game_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_fng_entry_setting_game_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif /* __MMI_GAME__ */

}

#if defined (__MMI_SUPPORT_JME__) 
extern void jmetoc_showlist();
extern void jmetoc_qq_entry();
extern void VM_select_sim_entry();
extern void DataAccountReadyCheck_QQApn();
extern void VM_setProxyIp();
#if defined(__QQ_MENU_SEPERATE__)
void mmi_jme_highlight_app(void)
{
	#ifndef	MMI_ON_WIN32
	SetLeftSoftkeyFunction(jmetoc_qq_entry,KEY_EVENT_UP);
	SetKeyHandler(jmetoc_qq_entry, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif
}
void mmi_jme_highlight_sim(void)
{
	#ifndef	MMI_ON_WIN32
	SetLeftSoftkeyFunction(VM_select_sim_entry,KEY_EVENT_UP);
	SetKeyHandler(VM_select_sim_entry, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif
}
void mmi_jme_highlight_apn(void)
{
	SetLeftSoftkeyFunction(DataAccountReadyCheck_QQApn,KEY_EVENT_UP);
	SetKeyHandler(DataAccountReadyCheck_QQApn, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
void mmi_jme_highlight_proxy(void)
{
	#ifndef	MMI_ON_WIN32
	SetLeftSoftkeyFunction(VM_setProxyIp,KEY_EVENT_UP);
	SetKeyHandler(VM_setProxyIp, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif
}

#else
void mmi_jme_highlight_app(void)
{
	SetLeftSoftkeyFunction(jmetoc_showlist,KEY_EVENT_UP);
	SetKeyHandler(jmetoc_showlist, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#endif
#endif
#if defined (__ZYX_PLATFORM_SUPPORT__) 
extern void ZYX_List_entry();
void mmi_zyx_highlight_app(void)
{
	SetLeftSoftkeyFunction(ZYX_List_entry,KEY_EVENT_UP);
	SetKeyHandler(ZYX_List_entry, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

#endif

/**************************************************************

	FUNCTION NAME		: mmi_fng_highlight_setting_java

  	PURPOSE				: highlight java setting

	INPUT PARAMETERS	: nil
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_fng_highlight_setting_java(void)
{
#if defined (__J2ME__)
	SetLeftSoftkeyFunction(mmi_fng_entry_setting_java_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_fng_entry_setting_java_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif /* __J2ME__ */
}

#if 0


/* NOTICE */
/* THIS FUNCTION should move to picture_editor's source, shall not put here */
/**************************************************************

	FUNCTION NAME		: Highlight_MENU_PICTURE_EDITOR

  	PURPOSE				: highlight picture editor

	INPUT PARAMETERS	: nil
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/

 
#ifdef __MMI_PICTURE_EDITOR_SUPPORT__
void Highlight_MENU_PICTURE_EDITOR(void)
{
	SetLeftSoftkeyFunction(EntryScr4PicEditSelectMenu,KEY_EVENT_UP);
	SetKeyHandler(EntryScr4PicEditSelectMenu, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#endif
#endif
 



