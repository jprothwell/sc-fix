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

/*******************************************************************************
* Filename:
* ---------
*	AudioPlayerMainSrc.c
*
* Project:
* --------
 
*
* Description:
* ------------
*   Audio player main program
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 * 
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/

#include "mmi_features.h"
#include "mmi_trace.h"
#include "mdiplayer.h"
#include "phonebooktypes.h"
#include "simdetectiondef.h"
#include "settingdefs.h"
#include "settingsgdcl.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#define __NEWSIMULATOR 
#include "globaldefs.h"
#include "globalscrenum.h"
#include "commonscreens.h"
#include "custdatares.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "custmenures.h"
#include "wgui_categories.h"
#include "wgui_categories_multimedia.h"
#include "wgui_categories_popup.h"
#include "hardwareevents.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"
#include "callmanagementiddef.h"

#include "timerevents.h"
#include "mainmenudef.h"
#include "funandgamesdefs.h"
#include "customcfg.h"
#include "unicodexdcl.h"
#include "filesystemdef.h"
#include "filemanagergprot.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "custom_nvram_editor_data_item.h"
#include "fat_fs.h"

#include "profilegprots.h"
#include "settingprofile.h"

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
#include "filemgr.h"
#include "profilemgrgprot.h"
#endif
#include "oslmemory.h"

#ifdef __MMI_FM_RADIO__
void mmi_fmrdo_power_on( BOOL turn_on );
#endif // __MMI_FM_RADIO__

// dependent headers for AudioPlayerXXX.h"
#include "gui_data_types.h"
#include "frameworkstruct.h"
#include "globalconstants.h"
#include "mdi_datatype.h"
#include "kal_release.h"
#include "conversions.h"
#include "filemgr.h"

#include "aud_daf_parser.h"

#include "mdi_audio.h"
#include "audioplayerdef.h"
#include "audioplayertype.h"
#include "audioplayerprot.h"
#include "audioplayermainscreen.h"
//#include "audioplayerplaylist.h"

#include "vdoplystrenum.h"
#include "audioplayerplaylist.h"
#ifdef __DRM_SUPPORT__
#ifdef MMI_ON_HARDWARE_P
#include "fs_errcode.h"
#include "drm_gprot.h"
#endif
#endif

#ifdef __MMI_SOUND_EFFECT__
#include "settingdefs.h"
#include "soundeffect.h"
#endif

 
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif
 
#ifdef __MMI_AVRCP_SUPPORT__
#include "btmmicm.h"
#include "btmmiavrcpgprot.h"
static U8 mmi_audply_bt_avrcp_cmd_hdlr(U8 command, mmi_avrcp_key_events key_events);
#endif /* __MMI_AVRCP_SUPPORT__ */ 

#ifdef __MMI_A2DP_SUPPORT__
#include "simdetectiongprot.h"
#include "btmmiscrgprots.h"
#include "av_bt.h"
#include "extdevicedefs.h"
#endif /* __MMI_A2DP_SUPPORT__ */
#if defined(MOTION_SENSOR_SUPPORT)
#include "gsensorapp.h"
#include "phonesetup.h"
#endif
#undef __NEWSIMULATOR 
extern pBOOL mmi_bootup_get_active_flight_mode();
extern void mmi_audply_cancel_fast_timer_handel(void);
#define GET_WCHAR(x) ( (UI_character_type)( (*(x)) | ((*(x+1))<<8) ) )

// For single play
U8 single_play_state = STATE_IDLE;
U32 single_play_duration;
kal_uint32 single_play_time;
BOOL in_single_play_screen = FALSE;
U8 single_volume = 5;
static void (*single_play_exit_callback_func)(void) = NULL;
extern BOOL HandleLowBatteryProcess(void);
#ifdef MMI_ON_HARDWARE_P
extern UINT8 audioplayer_restore_flag;
#else
UINT8 audioplayer_restore_flag;
#endif

#ifdef __MMI_AUDIO_PLAYER__

/* detail */
U16 g_mmi_audply_detail_fields[MAX_DAF_VIEW_FIELDS]; 

/* settings */
UI_string_type g_mmi_audply_settings_list_auto_gen[2];
UI_string_type g_mmi_audply_settings_skin[NO_OF_SKIN];
UI_string_type g_mmi_audply_settings_repeat[3];
UI_string_type g_mmi_audply_settings_shuffle[2];
UI_string_type g_mmi_audply_settings_background_play[2];
//UI_string_type g_mmi_audply_settings_preferred_list[NO_OF_LIST];
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
UI_string_type g_mmi_audply_settings_preferred_list[3];
#else
UI_string_type g_mmi_audply_settings_preferred_list[2];
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
#if defined(__MMI_NOKIA_STYLE_N800__)||defined(CAM_MD_SUPPORT)
UI_string_type g_mmi_audply_settings_rotated[2];
#endif
#if defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
//static S32 selected_lyrics_style;
//static UI_string_type g_mmi_audply_settings_lyrics[2];
 S8 g_mmi_lyric_str_buff[AUDPLY_LYRIC_STR_BUFF_MAX_SIZE]; 
audply_lyric_struct audply_lyric;
audply_lyric_printing_struct audply_lyric_print;
scrolling_text audply_scrolling_lyric1;
scrolling_text *audply_scrolling_lyric_p1 = NULL;
MMI_BOOL lyric_reEntry = MMI_FALSE; 
MMI_BOOL lyric_parse_ok = MMI_FALSE;
static MMI_BOOL lyric_backlight_has_turn_on = MMI_FALSE;
static MMI_BOOL lyric_full = MMI_FALSE;
extern U8 PhnsetGetDefEncodingType(void);
extern void TurnOnBacklight(U8 TimerFlag);
extern void mmi_show_qq_select_confirm(void (*default_entry) (void));

#endif /* __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__ */ 
#ifdef __MMI_SOUND_EFFECT__
UI_string_type g_mmi_audply_settings_equalizer[MAX_EQUALIZER_NUM];
#endif
U16 g_mmi_audply_settings_highlighted_item;

static S32 selected_list_auto_gen;
static S32 selected_preferred_list;
static S32 selected_skin;
static S32 selected_repeat_mode; // 0: off, 1: repeat one, 2: repeat all
static S32 selected_random_mode; // 0: off, 1: on
static S32 selected_background_play; // 0: off, 1: on
#ifdef __MMI_SOUND_EFFECT__
static S32 selected_equalizer_index;
#endif

U16 g_mmi_audply_option_highlighted_item;
U16 g_mmi_audply_list_highlighted_item;

// detail
aud_info_struct* daf_info_p = NULL;

// drives and paths
S16 phoneDrvLetter;
S16 cardDrvLetter;
UI_character_type root_dir_phone[20];
UI_character_type root_dir_card[20];
UI_character_type phone_list_name[FMGR_MAX_FILE_LEN+40];
UI_character_type card_list_name[FMGR_MAX_FILE_LEN+40];
UI_character_type fullfilename_temp[FMGR_MAX_PATH_LEN+1];

/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
S16 seccardDrvLetter;
UI_character_type root_dir_sec_card[20];
UI_character_type sec_card_list_name[FMGR_MAX_FILE_LEN+40];
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
// This variable is used to avoid repeating on a short file too quick and cause a result to block other commands
BOOL g_mmi_audply_play_completed_after_button_down;

extern signed char g_audply_main_selected_button; // defined in AudioPlayerMainScreen.c
extern BOOL g_audply_main_selected_button_down; // defined in AudioPlayerMainScreen.c
extern wgui_inline_item wgui_inline_items[]; // wgui_inline_edit.c
extern mmi_phb_context_struct g_phb_cntx;

// main control object
audply_struct g_audply;

// add files in a folder
static FS_HANDLE add_file_cursor = -1;
S16 add_file_result = -1;
S16 add_file_path_len;
UI_character_type add_file_path[FMGR_MAX_PATH_LEN+1];

BOOL g_mmi_audply_init_done = FALSE;
#if defined(__MMI_NOKIA_STYLE_N800__)||defined(CAM_MD_SUPPORT)
S32 g_mmi_audply_rotated_enable = 0;
#endif
extern audply_skin_struct g_audply_skins[NO_OF_SKIN]; // defined in resource_audply_skins.c 
#ifdef __MMI_A2DP_SUPPORT__
PS8 g_mmi_audply_settings_bt_a2dp_output[2];
#endif
#ifdef __MMI_A2DP_SUPPORT__
static S32 selected_bt_a2dp_output;     /* -1: block, 0: off, 1: on */

#endif

//chenhe for jasper+
#define AUDIO_PLAYER_PLAY_TIMER 200
//static UINT32 audio_playTime=0;//,audio_HasplayedTime=0;
//chenhe for jasper-
extern U8 GetHandsetInPhone(); 
#if defined(CAM_MD_SUPPORT)
extern void csd_start_sw_motionsensor(void);
extern void csd_stop_sw_motionsensor(void);
#endif

#if 0
const UI_character_type *audio_file_lower_exts[] = {

	L"wav", 
	L"amr",
#ifdef DAF_DECODE
	L"mp3",
#endif
#ifdef AAC_DECODE
	L"aac",
#endif
	L"mid",
	L"imy",
	L"vm ",
#ifdef AMRWB_DECODE
	L"awb",
#endif
	L"aif",
	L"aiff",
	L"aifc",
	L"au ",
	L"snd"
#if defined( MP4_CODEC ) && defined( AAC_DECODE )
	, L"m4a"
#endif
#ifdef MMF_DECODE
	, L"mmf"
#endif
#ifdef WMA_DECODE
	, L"wma"
#endif
};

const UI_character_type *audio_file_upper_exts[] = {
	L"WAV",
	L"AMR",
#ifdef DAF_DECODE
	L"MP3",
#endif
#ifdef AAC_DECODE
	L"AAC",
#endif
	L"MID",
	L"IMY",
	L"VM ",
#ifdef AMRWB_DECODE
	L"AWB",
#endif
	L"AIF",
	L"AIFF",
	L"AIFC",
	L"AU ",
	L"SND"
#if defined( MP4_CODEC ) && defined( AAC_DECODE )
	, L"M4A"
#endif
#ifdef MMF_DECODE
	, L"MMF"
#endif
#ifdef WMA_DECODE
	, L"WMA"
#endif

};
#endif

const UI_character_type * const audio_file_lower_exts[] = {

	(UI_character_type *)	"wav", 
	(UI_character_type *)	"amr",
#ifdef DAF_DECODE
	(UI_character_type *)	"mp3",
#endif
#ifdef AAC_DECODE
	(UI_character_type *)	"aac",
#endif
	(UI_character_type *)	"mid",
	(UI_character_type *)	"imy",
	(UI_character_type *)	"vm ",
#ifdef AMRWB_DECODE
	(UI_character_type *)	"awb",
#endif
	(UI_character_type *)	"aif",
	(UI_character_type *)	"aiff",
	(UI_character_type *)	"aifc",
	(UI_character_type *)	"au ",
	(UI_character_type *)	"snd"
#if defined( MP4_CODEC ) && defined( AAC_DECODE )
	,	(UI_character_type *) "m4a"
#endif
#ifdef MMF_DECODE
	, 	(UI_character_type *)"mmf"
#endif
#ifdef WMA_DECODE
	, 	(UI_character_type *)"wma"
#endif
};

const UI_character_type * const audio_file_upper_exts[] = {
	(UI_character_type *)	"WAV",
	(UI_character_type *)	"AMR",
#ifdef DAF_DECODE
	(UI_character_type *)	"MP3",
#endif
#ifdef AAC_DECODE
	(UI_character_type *)	"AAC",
#endif
	(UI_character_type *)	"MID",
	(UI_character_type *)	"IMY",
	(UI_character_type *)	"VM ",
#ifdef AMRWB_DECODE
	(UI_character_type *)	"AWB",
#endif
	(UI_character_type *)	"AIF",
	(UI_character_type *)	"AIFF",
	(UI_character_type *)	"AIFC",
	(UI_character_type *)	"AU ",
	(UI_character_type *)	"SND"
#if defined( MP4_CODEC ) && defined( AAC_DECODE )
	, 	(UI_character_type *)"M4A"
#endif
#ifdef MMF_DECODE
	, 	(UI_character_type *)"MMF"
#endif
#ifdef WMA_DECODE
	, 	(UI_character_type *)"WMA"
#endif

};

const U16 audio_file_icon_ids[] = {
	IMG_ID_AUDPLY_FILE_WAV_ICON,
	IMG_ID_AUDPLY_FILE_AMR_ICON,
#ifdef DAF_DECODE
	IMG_ID_AUDPLY_FILE_AUD_ICON,
#endif
#ifdef AAC_DECODE
	IMG_ID_AUDPLY_FILE_AAC_ICON,
#endif
	IMG_ID_AUDPLY_FILE_MID_ICON,
	IMG_ID_AUDPLY_FILE_IMY_ICON,
	IMG_ID_AUDPLY_FILE_AUD_ICON,
#ifdef AMRWB_DECODE
	IMG_ID_AUDPLY_FILE_AWB_ICON,
#endif
	IMG_ID_AUDPLY_FILE_AUD_ICON,
	IMG_ID_AUDPLY_FILE_AUD_ICON,
	IMG_ID_AUDPLY_FILE_AUD_ICON,
	IMG_ID_AUDPLY_FILE_AUD_ICON,
	IMG_ID_AUDPLY_FILE_AUD_ICON
#ifdef MP4_CODEC
//	, IMG_ID_AUDPLY_FILE_AUD_ICON
#endif
#ifdef MMF_DECODE
	, IMG_ID_AUDPLY_FILE_AUD_ICON
#endif
#ifdef WMA_DECODE
	, IMG_ID_AUDPLY_FILE_AUD_ICON
#endif
};
void mmi_audply_fast_forward(void);
void mmi_audply_fast_backward(void);
//string id of option item
static U16 option_texts[7]; // keep option text id array here because we need to check highlight text id in mmi_audply_option_right_arrow_key
#ifdef __MMI_A2DP_SUPPORT__

/* av bt */
extern kal_int32 med_util_utf8_to_ucs2(kal_uint8 *dest, kal_uint32 dest_size, kal_uint8 *src, kal_uint32 src_size);

typedef struct
{
    BOOL bt_a2dp_output;
    BOOL bt_a2dp_headset_valid;
    mmi_bt_dev_struct bt_a2dp_headset;
} mmi_audply_bt_settings_struct;

static BOOL wait_fot_bt_pwron;
static mmi_audply_bt_settings_struct bt_settings;
static mmi_audply_bt_settings_struct bt_settings_csd;//zhou siyou add 091030

static mmi_bt_dev_struct *selected_bt_a2dp_headset_temp;
static UI_character_type setting_bt_a2dp_headset_name[(MMI_BT_BD_NAME_LEN+1)/2];
static void mmi_audply_sap_block_a2dp(void);
static U8 *mmi_audply_bts_inline_change_select_callback(void);
static U8 *mmi_audply_bts_inline_default_callback(void);
 BOOL mmi_audply_bluetooth_settings_apply_change(BOOL bt_a2dp_output, BOOL back_to_main);


/*****************************************************************************
 * FUNCTION
 *  mmi_audply_is_output_to_bt
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  BOOL
 *****************************************************************************/
BOOL mmi_audply_is_output_to_bt(void)
{
    return (mmi_bt_is_power_on(FALSE) && !mmi_bootup_get_active_flight_mode() && 
        bt_settings_csd.bt_a2dp_output);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_get_bt_headset
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  mmi_bt_dev_struct*
 *****************************************************************************/
mmi_bt_dev_struct *mmi_audply_get_bt_headset(void)
{
    if (bt_settings_csd.bt_a2dp_headset_valid)
        return &bt_settings_csd.bt_a2dp_headset;
    return NULL;
}
extern U8 new_connet_a2dp;
void mmi_audply_set_bt_headset(  U32 lap, U8 uap, U16 nap,U8 *name_p)
{
	mmi_trace(1,"mmi_audply_set_bt_headset is called ");
	bt_settings_csd.bt_a2dp_headset_valid=TRUE;
	 bt_settings_csd.bt_a2dp_output=TRUE;
	bt_settings_csd.bt_a2dp_headset.bd_addr.lap=lap;
	bt_settings_csd.bt_a2dp_headset.bd_addr.uap=uap;
	bt_settings_csd.bt_a2dp_headset.bd_addr.nap=nap;

	if(g_audply.state==STATE_PLAY)
		new_connet_a2dp=TRUE;
	else
		new_connet_a2dp=FALSE;
	
	//bt_settings.bt_a2dp_headset.name
}
extern void av_bt_close_codec(void);

void mmi_audply_clean_set_bt_headset(  void)
{
	mmi_trace(1,"mmi_audply_clean_set_bt_headset is called ");
	bt_settings_csd.bt_a2dp_headset_valid=FALSE;
	 bt_settings_csd.bt_a2dp_output=FALSE;
	bt_settings_csd.bt_a2dp_headset.bd_addr.lap=0;
	bt_settings_csd.bt_a2dp_headset.bd_addr.uap=0;
	bt_settings_csd.bt_a2dp_headset.bd_addr.nap=0;
	av_bt_close_codec();
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_read_bt_settings
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_read_bt_settings(void)
{
#ifdef CSD_TARGET
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    ReadRecord(NVRAM_EF_AUDIO_PLAYER_BT_SETTINGS_LID, 1, (void*)&bt_settings,
        sizeof(mmi_audply_bt_settings_struct), &error);
#else
    bt_settings.bt_a2dp_output = FALSE;
    bt_settings.bt_a2dp_headset_valid = FALSE;
#endif /* CSD_TARGET */ 
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_write_bt_settings
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_write_bt_settings(void)
{
#ifdef CSD_TARGET
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    WriteRecord(NVRAM_EF_AUDIO_PLAYER_BT_SETTINGS_LID, 1, (void*)&bt_settings,
        sizeof(mmi_audply_bt_settings_struct), &error);
#endif /* CSD_TARGET */
}
#else
void mmi_audply_clean_set_bt_headset(  void){}
void mmi_audply_set_bt_headset(  U32 lap, U8 uap, U16 nap,U8 *name_p){}
#endif /* __MMI_A2DP_SUPPORT__ */

/*****************************************************************************
* FUNCTION
*     mmi_audply_read_settings
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_read_settings( void )
{
#ifdef MMI_ON_HARDWARE_P
	S16 error;
	ReadValue( NVRAM_AUDPLY_LIST_AUTO_GEN, &g_audply.list_auto_gen, DS_BYTE, &error );
	ReadValue( NVRAM_AUDPLY_VOLUME, &g_audply.volume, DS_BYTE, &error );
	ReadValue( NVRAM_AUDPLY_PREFER_LIST, &g_audply.preferred_list, DS_BYTE, &error );
	if(MMI_CARD_DRV>0)
	{
	g_audply.preferred_list = LIST_FROM_CARD;
        WriteValue(NVRAM_AUDPLY_PREFER_LIST,  &g_audply.preferred_list, DS_BYTE, &error );	
	}
	ReadValue( NVRAM_AUDPLY_SKIN, &g_audply.skin, DS_BYTE, &error );
	ReadValue( NVRAM_AUDPLY_REPEAT, &g_audply.repeat_mode, DS_BYTE, &error );
	ReadValue( NVRAM_AUDPLY_SHUFFLE, &g_audply.random_mode, DS_BYTE, &error );
	ReadValue( NVRAM_AUDPLY_BACKGROUND_PLAY, &g_audply.background_play, DS_BYTE, &error );
#if defined(__MMI_NOKIA_STYLE_N800__)||defined(CAM_MD_SUPPORT)
	ReadValue( NVRAM_AUDPLY_ROTATED_ENABLE, &g_audply.rotated_enable, DS_BYTE, &error );
#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
    ReadValue(NVRAM_AUDPLY_LYRICS_DISPLAY, &g_audply.lyrics_display, DS_BYTE, &error);
    {
        g_audply.lyrics_display = 1;//0;
        WriteValue(NVRAM_AUDPLY_LYRICS_DISPLAY, &g_audply.lyrics_display, DS_BYTE, &error);
    }    
#endif
#else
	g_audply.list_auto_gen = 1;
	g_audply.volume = 4;
	g_audply.preferred_list = 0;
	g_audply.skin = 0;
	g_audply.repeat_mode = 0;
	g_audply.random_mode = 0;
	g_audply.background_play = 1;
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_write_settings
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_write_settings( void )
{
	S16 error;
	WriteValue( NVRAM_AUDPLY_LIST_AUTO_GEN, &g_audply.list_auto_gen, DS_BYTE, &error );
	WriteValue( NVRAM_AUDPLY_VOLUME, &g_audply.volume, DS_BYTE, &error );
	WriteValue( NVRAM_AUDPLY_PREFER_LIST, &g_audply.preferred_list, DS_BYTE, &error );
	WriteValue( NVRAM_AUDPLY_SKIN, &g_audply.skin, DS_BYTE, &error );
	WriteValue( NVRAM_AUDPLY_REPEAT, &g_audply.repeat_mode, DS_BYTE, &error );
	WriteValue( NVRAM_AUDPLY_SHUFFLE, &g_audply.random_mode, DS_BYTE, &error );
	WriteValue( NVRAM_AUDPLY_BACKGROUND_PLAY, &g_audply.background_play, DS_BYTE, &error );
#if defined(__MMI_NOKIA_STYLE_N800__)||defined(CAM_MD_SUPPORT)
	WriteValue( NVRAM_AUDPLY_ROTATED_ENABLE, &g_audply.rotated_enable, DS_BYTE, &error );
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_get_volume
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     U8
* GLOBALS AFFECTED
*     
*****************************************************************************/
U8 mmi_audply_get_volume()
{
	if( !g_mmi_audply_init_done )
	{
#ifdef MMI_ON_HARDWARE_P
		S16 error;
		ReadValue( NVRAM_AUDPLY_VOLUME, &g_audply.volume, DS_BYTE, &error );
#else
		g_audply.volume = 4;
#endif
	}

	return g_audply.volume;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_save_volume
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     U8 volume
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_audply_save_volume( U8 volume )
{
	S16 error;
	g_audply.volume = volume;
	WriteValue( NVRAM_AUDPLY_VOLUME, &g_audply.volume, DS_BYTE, &error );

	return (error == NVRAM_WRITE_SUCCESS);
}
/*Jinzh Add Start For 6232 Ver: TBM780  on 2007-8-14 18:40 */
/*****************************************************************************
* FUNCTION
*     mmi_audply_get_state
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     U8
* GLOBALS AFFECTED
*     
*****************************************************************************/
audply_state_enum mmi_audply_get_state()
{
	return g_audply.state;
}
/*Jinzh Add End  For 6232 Ver: TBM780  on 2007-8-14 18:40 */
/*****************************************************************************
* FUNCTION
*     mmi_audply_get_fs_info
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_get_fs_info()
{
#if 0
	S8 temp[4];
	
	// get phone drive
	if( (phoneDrvLetter = MMI_PUBLIC_DRV) > 0 )
	{
		sprintf( temp, "%c:/", (S8) phoneDrvLetter );
		AnsiiToUnicodeString( (S8*)root_dir_phone, temp );
		pfnUnicodeStrcpy( (S8*)phone_list_name, (const S8*)root_dir_phone );
		pfnUnicodeStrcat( (S8*)phone_list_name, (const S8*)(L"audio_play_list.txt") );
	}
	
	// get card drive
	if( (cardDrvLetter = MMI_CARD_DRV) > 0 )
	{
		sprintf( temp, "%c:/", (S8) cardDrvLetter);
		AnsiiToUnicodeString( (S8*)root_dir_card, temp );
		pfnUnicodeStrcpy( (S8*)card_list_name, (const S8*)root_dir_card );
		pfnUnicodeStrcat( (S8*)card_list_name, (const S8*)(L"audio_play_list.txt") );
	}
	#endif
	S8 temp[10] ;

	//S8 Ansnamelist[30] = "/home/audio_play_list.txt"; //chenhe add a '/'
	S8 Ansnamelist[30] = "audio_play_list.txt"; //chenhe for jasper,for flash only have 1 sector include nvram and flash FS
	S8 uninamelist[60];

	S8 testforlist[100];
	
	AnsiiToUnicodeString(uninamelist,Ansnamelist);
	
	// get phone drive
	if( (phoneDrvLetter = MMI_PUBLIC_DRV) > 0 )
	{
		sprintf( temp, "%c:/", (S8) phoneDrvLetter );
		AnsiiToUnicodeString( (S8*)root_dir_phone, temp );
		pfnUnicodeStrcpy( (S8*)phone_list_name, (const S8*)root_dir_phone );
		pfnUnicodeStrcat( (S8*)phone_list_name, uninamelist );

		UnicodeToAnsii(testforlist, (S8*)phone_list_name);
		
		mmi_trace(1,"mmi_audply_get_fs_info,phone_list_name is %s",testforlist);
	}
	
	// get card drive
	if( (cardDrvLetter = MMI_CARD_DRV) > 0 )
	{
		sprintf( temp, "%c:/", (S8) cardDrvLetter);
		AnsiiToUnicodeString( (S8*)root_dir_card, temp );
		pfnUnicodeStrcpy( (S8*)card_list_name, (const S8*)root_dir_card );//chenhe
		pfnUnicodeStrcat( (S8*)card_list_name, uninamelist );//chenhe
		//pfnUnicodeStrcpy( (S8*)card_list_name, (const S8*)uninamelist );//chenhe

		UnicodeToAnsii(testforlist,(S8*)card_list_name);
		mmi_trace(1,"mmi_audply_get_fs_info,card_list_name is %s",testforlist);

		
	}
    
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    // get card 2 drive
	if( (seccardDrvLetter = MMI_SEC_CARD_DRV) > 0 )
	{
		sprintf( temp, "%c:/", (S8) seccardDrvLetter);
		AnsiiToUnicodeString( (S8*)root_dir_sec_card, temp );
		pfnUnicodeStrcpy( (S8*)sec_card_list_name, (const S8*)root_dir_sec_card );
		pfnUnicodeStrcat( (S8*)sec_card_list_name, uninamelist );
		//pfnUnicodeStrcpy( (S8*)card_list_name, (const S8*)uninamelist );
		UnicodeToAnsii(testforlist,(S8*)sec_card_list_name);
		mmi_trace(1,"mmi_audply_get_fs_info,sec_card_list_name is %s",testforlist);
	}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
	
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_set_subLCD_shown
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     BOOL is_shown
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_set_subLCD_shown( BOOL is_shown )
{
	g_audply.is_subLCD_shown = is_shown;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_delete_history_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     U8
* GLOBALS AFFECTED
*     
*****************************************************************************/
U8 mmi_audply_delete_history_hdlr( void* in_param )
{
	if( !g_audply.background_play )
	{
		// There is a case that state = STATE_IDLE and need_replay = TRUE
		// and we need to prevent from replay after this function call by setting need_replay = FALSE
		g_audply.need_replay = FALSE;
		mmi_audply_stop_playing();
	}
#if defined(__MMI_MAINLCD_220X176__)

	mdi_audio_resume_background_play();
#endif

	return FALSE;
}

/*****************************************************************************
* FUNCTION
*     InitAudioPlayerApp
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void InitAudioPlayerApp()
{
	//SetHiliteHandler( AUDIO_PLAYER_MAIN_MENUID, HighlightAudioPlayer );
     // mmi_trace(1, "InitAudioPlayerApp");

	SetHiliteHandler(AUDIO_PLAYER_MAIN_MENUID, HighlightAudioPlayer );
#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
	SetHiliteHandler( MAIN_MENU_OPTION_AUDIO_PLAYER_APP, HighlightAudioPlayer );
#endif
	g_audply.state = STATE_IDLE;
	g_audply.is_subLCD_shown = FALSE;  
	mmi_audply_read_settings(); 
#ifdef __MMI_A2DP_SUPPORT__
    mmi_audply_read_bt_settings();
    av_bt_set_default_callback(mmi_audply_bt_open_callback);
#endif

#ifdef __MMI_AVRCP_SUPPORT__
    mmi_bt_avrcp_set_cmd_hdlr(mmi_audply_bt_avrcp_cmd_hdlr);
#endif 
#ifdef __MMI_AUDIO_PLAYER_ON_SUBLCD__
	SetHiliteHandler(MENU_ID_SUB_AUDPLY,  HighlightSubAudioPlayer);
	g_audply.in_sub_screen = FALSE;
#endif 
} 

/*****************************************************************************
* FUNCTION
*     HighlightAudioPlayer
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void HighlightAudioPlayer()
{
	ChangeLeftSoftkey( STR_GLOBAL_OK, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, 0 );

	SetLeftSoftkeyFunction( mmi_audply_entry_main, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#if !defined(__MMI_MAINLCD_220X176__) && !defined(__MMI_BLACK_BERRY_MAIN_MENU__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */	
	SetKeyHandler( mmi_audply_entry_main, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN ); 
#endif
}

#ifdef __MMI_AVRCP_SUPPORT__

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_bt_avrcp_cmd_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  command         [IN]        
 *  key_press       [IN]        
 * RETURNS
 *  U8
 *****************************************************************************/
 
BOOL mmi_audply_background_callback_hdlr( mdi_result result );
U8 mmi_audply_bt_avrcp_cmd_hdlr(U8 command, mmi_avrcp_key_events key_events)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 result = MMI_AVRCP_CR_ACCEPT;
    //UI_string_type list_name ;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   mmi_trace(g_sw_BT, "mmi_audply_bt_avrcp_cmd_hdlr::command =%d, key_events=%d", command, key_events);

    /* Use popup to indicate received messages */
#ifdef __MMI_AVRCP_DEBUG__
    U8 display_buffer[80];

    mmi_ucs2cpy((PS8)display_buffer, (const PS8)GetString(mmi_bt_avrcp_util_get_cmd_string(command)));
    switch (key_events)
    {
        case MMI_AVRCP_KEY_UP :
            mmi_ucs2cat((PS8)display_buffer, (const PS8)L"\n KEY UP\0");    
            break;
        case MMI_AVRCP_KEY_DOWN:
            mmi_ucs2cat((PS8)display_buffer, (const PS8)L"\n KEY DOWN\0");
            break;
        case MMI_AVRCP_KEY_CANCELED:
            mmi_ucs2cat((PS8)display_buffer, (const PS8)L"\n KEY CANCELED\0");    
            break;
        default:
            break;
    }
   
    if((command == MMI_AVRCP_POP_PLAY ||
       command == MMI_AVRCP_POP_STOP ||
       command == MMI_AVRCP_POP_PAUSE ||
       command == MMI_AVRCP_POP_FORWARD ||
       command == MMI_AVRCP_POP_BACKWARD) &&
       g_audply.in_main_screen)
    {
    	/* Do nothing */
    }
    else
    {
		DisplayPopup(display_buffer, IMG_GLOBAL_OK, 0, 1000, (U8) SUCCESS_TONE);    	
        goto avrcp_finish;
    }
#endif /* __MMI_AVRCP_DEBUG__ */
    


 mmi_trace(g_sw_BT, "mmi_audply_bt_avrcp_cmd_hdlr::command =%d, key_events=%d,g_idle_context.IsOnIdleScreen=%d,g_mmi_audply_init_done%d,g_audply.background_play=%d,g_audply.in_main_screen=%d,g_audply.state =%d", \
 command, key_events,g_idle_context.IsOnIdleScreen,g_mmi_audply_init_done,g_audply.background_play,\
 g_audply.in_main_screen,g_audply.state );

     mmi_trace(1,"avrcp_cmd_hdlr11,button_down=%d, g_audply.state=%d,selected_button=%d",\
	 	g_audply_main_selected_button_down,g_audply.state,g_audply_main_selected_button);
    if (g_idle_context.IsOnIdleScreen)
    {
    #ifdef __USB_IN_NORMAL_MODE__
        /* check is in mass storage mode */
        if (mmi_usb_is_in_mass_storage_mode())
        {
            /* phone drive is exported, cant use this app, popup message in key up */
            if(key_events == MMI_AVRCP_KEY_UP)
            {
                mmi_usb_app_unavailable_popup(0);   /* pass 0 will show default string */
            }
            result = MMI_AVRCP_CR_REJECT;
            goto avrcp_finish;
        }
    #endif /* __USB_IN_NORMAL_MODE__ */ 


	    /* initialization after re-power on */
	    if (!g_mmi_audply_init_done)
	    {
	        mmi_audply_get_fs_info();

	        /* register mdi background handler */
            g_audply.need_restore = FALSE;
            g_audply.wait_next = FALSE;
            g_audply.need_replay = FALSE;
            g_audply.in_list_screen = FALSE;
            g_audply.state = STATE_IDLE;
            g_mmi_audply_play_completed_after_button_down = FALSE;

	        g_audply.present_list = mmi_audply_get_preferred_or_available_list();
	    }
       	//list_name = mmi_audply_get_present_list_file();
            if (!g_mmi_audply_init_done || !mmi_audply_does_file_exist(PRESENT_LIST_FILE)) 
		{
			mmi_trace(1,"mmi_audply_regen_and_reload_playlist will be called,g_audply.list_auto_gen=%d",g_audply.list_auto_gen);
			mmi_audply_regen_and_reload_playlist((BOOL) g_audply.list_auto_gen, TRUE);
	    	}

	 
mmi_trace(1,"g_mmi_audply_init_done=%d",g_mmi_audply_init_done);

	    if (!g_mmi_audply_init_done)
        {
            g_audply_main_selected_button_down = FALSE;
            if (g_audply.n_total_items == 0)
            {
                g_audply_main_selected_button = (signed char)AUDPLY_MAIN_DISABLED;
            }
            else
            {
                g_audply_main_selected_button = (signed char)AUDPLY_MAIN_STOP;
            }
		mmi_audply_cancel_fast_timer_no_condition();
        }
        g_mmi_audply_init_done = TRUE;
		
		mdi_audio_set_background_handler(MDI_BACKGROUND_APP_AUDPLY, mmi_audply_background_callback_hdlr ); //chenhe enable for jessmine replay after interrupt
		
		if(!g_audply.background_play)
		{
		    result = MMI_AVRCP_CR_REJECT;
		    goto avrcp_finish;
		}
    }
    else if (!g_audply.in_main_screen && !(g_mmi_audply_init_done &&
                                           (g_audply.state == STATE_PLAY || g_audply.state == STATE_PAUSED/*|| g_audply.state == STATE_IDLE*/)))
    {

//	mmi_trace(1,"g_mmi_audply_init_done =%d, g_audply.state = %d ",g_mmi_audply_init_done, g_audply.state );
	
        result = MMI_AVRCP_CR_REJECT;
        goto avrcp_finish;
    }

    if (key_events == MMI_AVRCP_KEY_CANCELED)
    {
        switch (command)
        {
            case MMI_AVRCP_POP_PLAY:
            case MMI_AVRCP_POP_STOP:
            case MMI_AVRCP_POP_PAUSE:
            case MMI_AVRCP_POP_FORWARD:
            case MMI_AVRCP_POP_BACKWARD:
//                mmi_audply_press_button_canceled();
                break;
            default :
                break;
        }
        /*the return value do not mean anything in key canceled event*/
        result = MMI_AVRCP_CR_REJECT;
        goto avrcp_finish;
    }
     mmi_trace(1,"avrcp_cmd_hdlr22,button_down=%d, g_audply.state=%d,selected_button=%d",\
	 	g_audply_main_selected_button_down,g_audply.state,g_audply_main_selected_button);
mmi_trace(1,"command=%d,g_audply.state=%d,key_events=%d",command,g_audply.state,key_events);
    switch (command)
    {
        case MMI_AVRCP_POP_PLAY:
            if(g_audply.state != STATE_PLAY)
            {
                if (key_events == MMI_AVRCP_KEY_UP)
                {
                    mmi_audply_press_play_button_up();
                }
                else if(key_events == MMI_AVRCP_KEY_DOWN)
                {
                    mmi_audply_press_play_button_down();
                }
            }
            break;

        case MMI_AVRCP_POP_STOP:
            if (key_events == MMI_AVRCP_KEY_UP)
            {
                mmi_audply_press_stop_button_up();
            }
            else if(key_events == MMI_AVRCP_KEY_DOWN)
            {
                mmi_audply_press_stop_button_down();
            }
            break;

        case MMI_AVRCP_POP_PAUSE:
            if(g_audply.state != STATE_PAUSED)
            {
                if (key_events == MMI_AVRCP_KEY_UP)
                {
                    mmi_audply_press_play_button_up();
                }
                else if(key_events == MMI_AVRCP_KEY_DOWN)
                {
                    mmi_audply_press_play_button_down();
                }
            }
            break;

        case MMI_AVRCP_POP_FORWARD:
            if (key_events == MMI_AVRCP_KEY_UP)
            {
                mmi_audply_press_next_button_up();
            }
            else if (key_events == MMI_AVRCP_KEY_DOWN)
            {
                mmi_audply_press_next_button_down();
            }            
            else if(key_events == MMI_AVRCP_KEY_LONGPRESS)
            {
                if(GetActiveScreenId() == SCR_ID_AUDPLY_MAIN )
                {
  //                  mmi_audply_press_next_button_longpress();
                }
                
            }
            else if(key_events == MMI_AVRCP_KEY_REPEAT)
            {

            }
            break;

        case MMI_AVRCP_POP_BACKWARD:
            if (key_events == MMI_AVRCP_KEY_UP)
            {
                mmi_audply_press_prev_button_up();
            }
            else if(key_events == MMI_AVRCP_KEY_DOWN)
            {
                mmi_audply_press_prev_button_down();
            }
            else if(key_events == MMI_AVRCP_KEY_LONGPRESS)
            {
                if(GetActiveScreenId() == SCR_ID_AUDPLY_MAIN )
                {
//                    mmi_audply_press_prev_button_longpress();
                }
                
            }
            else if(key_events == MMI_AVRCP_KEY_REPEAT)
            {
            
            }
            break;

        default:
            result = MMI_AVRCP_CR_NOT_IMPLEMENT;
            break;
    }

avrcp_finish:

#ifdef __MMI_AVRCP_DEBUG__
	return MMI_AVRCP_CR_ACCEPT;
#else
	return result;
#endif
    
}

#endif /* __MMI_AVRCP_SUPPORT__ */ 


/*****************************************************************************
* FUNCTION
*     mmi_audply_update_main
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_update_main()
{
	audply_skin_struct* skin_p;
	FuncPtr button_press_hdlr[] = {
		mmi_audply_press_play_button_down, // AUDPLY_MAIN_PLAY
		mmi_audply_press_prev_button_down, // AUDPLY_MAIN_PREV
		mmi_audply_press_next_button_down, // AUDPLY_MAIN_NEXT
		mmi_audply_press_stop_button_down // AUDPLY_MAIN_STOP
	};
	FuncPtr button_release_hdlr[] = {
		mmi_audply_press_play_button_up, // AUDPLY_MAIN_PLAY
		mmi_audply_press_prev_button_up, // AUDPLY_MAIN_PREV
		mmi_audply_press_next_button_up, // AUDPLY_MAIN_NEXT
		mmi_audply_press_stop_button_up // AUDPLY_MAIN_STOP
	};
	
	 g_audply_main_selected_button_down = FALSE;
    
	// need to check current condition
	if( g_audply.n_total_items == 0 )
		g_audply_main_selected_button = (signed char) AUDPLY_MAIN_DISABLED;
	else
	{
		switch( g_audply.state )
		{
		case STATE_IDLE:
			g_audply_main_selected_button = (signed char) AUDPLY_MAIN_STOP;
			break;
		case STATE_PLAY:
			g_audply_main_selected_button = (signed char) AUDPLY_MAIN_PLAY;
			break;
		case STATE_PAUSED:
			g_audply_main_selected_button = (signed char) AUDPLY_MAIN_PAUSE;
			break;
		}
	}

	// show main screen
	mmi_audply_show_mainscreen( STR_ID_AUDPLY_TITLE, STR_ID_AUDPLY_LIST, STR_GLOBAL_BACK, g_audply.skin );
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__

    /* re print lyric, or it will  only be written while next lyric time tag is reached. */
    if (g_audply.lyrics_display &&
        g_audply.state != STATE_IDLE &&
        mmi_audply_is_lyrics_valid())
    {
#ifndef __MMI_AUDIO_PLAYER_LYRIC_SCREEN_BASED_ASM__
        U8 new_encoding_type = PhnsetGetDefEncodingType();
        if (audply_lyric.lrc_encoding_chset != new_encoding_type)   
        {
            lyric_parse_ok = mmi_audply_init_lyric();
        }
#else
        lyric_parse_ok = mmi_audply_init_lyric();
#endif /* __MMI_AUDIO_PLAYER_LYRIC_SCREEN_BASED_ASM__ */
	mmi_trace(1,"mmi_audply_lyric_on_time()----2 - ,lyric_parse_ok is %d",lyric_parse_ok);
        mmi_audply_lyric_on_time();
    }
#endif /* __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__ */ 
	
	// register LSK, RSK hanlders
	SetKeyHandler( mmi_audply_entry_list, KEY_LSK, KEY_EVENT_UP );
	SetKeyHandler(GoBackHistory,KEY_RSK,KEY_EVENT_UP);
	
	// register play / pause, stop, prev, next button handlers
	skin_p = &g_audply_skins[g_audply.skin];
	SetKeyHandler( mmi_audply_fast_backward, KEY_LEFT_ARROW, KEY_LONG_PRESS );
    	SetKeyHandler( mmi_audply_fast_forward, KEY_RIGHT_ARROW, KEY_LONG_PRESS );
	#ifdef __MUSIC_KEY_SUPPORT__
    	SetKeyHandler( mmi_audply_fast_backward, KEY_LEFT_ARROW, KEY_LONG_PRESS );
    	SetKeyHandler( mmi_audply_fast_forward, KEY_RIGHT_ARROW, KEY_LONG_PRESS );
        SetKeyHandler( UI_dummy_function, KEY_LEFT_ARROW, KEY_EVENT_UP );
    	SetKeyHandler( UI_dummy_function, KEY_RIGHT_ARROW, KEY_EVENT_UP );	
	#else
    	SetKeyHandler( button_release_hdlr[ skin_p->left_key_mapping ], KEY_LEFT_ARROW, KEY_EVENT_UP );
    	SetKeyHandler( button_release_hdlr[ skin_p->right_key_mapping ], KEY_RIGHT_ARROW, KEY_EVENT_UP );	
    	SetKeyHandler( button_press_hdlr[ skin_p->left_key_mapping ], KEY_LEFT_ARROW, KEY_EVENT_DOWN );
    	SetKeyHandler( button_press_hdlr[ skin_p->right_key_mapping ], KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
    #endif
    SetKeyHandler( button_release_hdlr[ skin_p->up_key_mapping ], KEY_UP_ARROW, KEY_EVENT_UP );
	SetKeyHandler( button_release_hdlr[ skin_p->down_key_mapping ], KEY_DOWN_ARROW, KEY_EVENT_UP );
    SetKeyHandler( button_press_hdlr[ skin_p->up_key_mapping ], KEY_UP_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( button_press_hdlr[ skin_p->down_key_mapping ], KEY_DOWN_ARROW, KEY_EVENT_DOWN );

#if defined(__PROJECT_GALLITE_C01__)
	SetKeyHandler( mmi_audply_press_inc_volume, KEY_UP_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_dec_volume, KEY_DOWN_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_inc_volume_up, KEY_UP_ARROW, KEY_EVENT_UP );
	SetKeyHandler( mmi_audply_press_dec_volume_up, KEY_DOWN_ARROW, KEY_EVENT_UP );
#endif

#ifdef __MUSIC_KEY_SUPPORT__
	SetKeyHandler( mmi_audply_press_play_button_down, KEY_MUSIC_PLAY, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_stop_button_down, KEY_MUSIC_STOP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_prev_button_down, KEY_MUSIC_PREV, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_next_button_down, KEY_MUSIC_NEXT, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_play_button_up, KEY_MUSIC_PLAY, KEY_EVENT_UP );
	SetKeyHandler( mmi_audply_press_stop_button_up, KEY_MUSIC_STOP, KEY_EVENT_UP );
	SetKeyHandler( mmi_audply_press_prev_button_up, KEY_MUSIC_PREV, KEY_EVENT_UP );
	SetKeyHandler( mmi_audply_press_next_button_up, KEY_MUSIC_NEXT, KEY_EVENT_UP );
#endif

//wug modify used to cs200 start 20070821

	// register volume inc, dec handlers
#if defined(__NO_SIDE_VOLUME_KEY_56__)//niej for CS621 adjust volume during play music
	// register volume inc, dec handlers
	SetKeyHandler( mmi_audply_press_inc_volume, KEY_6, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_dec_volume, KEY_5, KEY_EVENT_DOWN );
#elif defined(__NO_SIDE_VOLUME_KEY_46__)
	// register volume inc, dec handlers
	SetKeyHandler( mmi_audply_press_inc_volume, KEY_6, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_dec_volume, KEY_4, KEY_EVENT_DOWN );
#elif defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__)
	// register volume inc, dec handlers
	SetKeyHandler( mmi_audply_press_inc_volume, KEY_POUND, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_dec_volume, KEY_STAR, KEY_EVENT_DOWN );
#else
	// register volume inc, dec handlers
	SetKeyHandler( mmi_audply_press_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
#if defined(__PROJECT_GALLITE_C01__)
	SetKeyHandler( mmi_audply_press_inc_volume_up, KEY_VOL_UP, KEY_EVENT_UP );
	SetKeyHandler( mmi_audply_press_dec_volume_up, KEY_VOL_DOWN, KEY_EVENT_UP );
#endif	
#endif

//wug modify used to cs200 end 20070821
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_entry_main
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
#ifdef __MMI_TOUCH_SCREEN__
void mmi_audply_pen_down_hdlr(mmi_pen_point_struct pos);
void mmi_audply_pen_up_hdlr(mmi_pen_point_struct pos);
void mmi_audply_pen_move_hdlr(mmi_pen_point_struct pos);
#endif

extern int checkUsbstate(void);
extern VOID mmi_display_usb_busy(void);
extern VOID mmi_display_gprs_active(void) ;
extern BOOL checkVMOn(void);
extern void mdi_audio_suspend_background_playFM(void);
extern void mmi_show_qq_select_confirm(void (*default_entry)(void));

void mmi_audply_entry_main()
{

    if(HandleLowBatteryProcess())
    {
        mmi_trace(1,"low battery in audply");
        return ;
    }

	/*liwh Add Start For 6198 Ver: TBM780  on 2007-8-12 13:40 */
#if 0
	if (!is_phb_init_done())
	{
	    //开机初始化 stringid = 25803  modified by renyh   2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_PROGRESS, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		return;
	}
#endif /* Guoj delete. It's not used now at 2009-6-9 */
	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(mmi_audply_entry_main);
		return;
	}
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
	/*liwh Add End  For 6198 Ver: TBM780  on 2007-8-12 13:40 */
	
	/*liwh Add Start For 6053 Ver: TBM780  on 2007-8-2 17:14 */
        // dingjian20071030 open Tcars
#ifndef COOLSAND_MULTIMEDIA_USE_FLASHFS
        if( (cardDrvLetter = MMI_CARD_DRV) <= 0 
            /*+ zhouqin modify for dual t-flash card 20110503*/
            #if defined(DUAL_TFLASH_SUPPORT)
             && ((seccardDrvLetter = MMI_SEC_CARD_DRV) <= 0 )
            #endif
            /*- zhouqin modify for dual t-flash card 20110503*/
          )
	{
	DisplayPopup( (PU8)GetString(/*STR_ID_AUDPLY_NO_CARD*/ STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD ), IMG_GLOBAL_WARNING, 0, 1000, (U8)WARNING_TONE );	
	return;
	}
#endif        
	/*liwh Add End  For 6053 Ver: TBM780  on 2007-8-2 17:14 */
#ifdef __MMI_FM_RADIO__
#if defined(__MMI_MAINLCD_220X176__)
		mdi_audio_suspend_background_playFM();
#endif
#endif // __MMI_FM_RADIO__

  
  //     mmi_trace(1, "0000000000000000000000000000000000000000");
	EntryNewScreen( SCR_ID_AUDPLY_MAIN, mmi_audply_exit_main, NULL, NULL );
	entry_full_screen();
#if defined(CAM_MD_SUPPORT)
if(g_audply.rotated_enable )
      csd_start_sw_motionsensor();
#endif

    
#if defined(MOTION_SENSOR_SUPPORT)
		if(MotionSensor_GetSetting() == 1)
		{
			motion_sensor_set_app_id(GSENSOR_APP_SHAKE_AUDIOPLAYER);
			motion_sensor_shake(TRUE);
		}
#endif

#ifdef __MMI_FM_RADIO__
	mmi_fmrdo_power_on( FALSE );
	mdi_audio_set_background_handler(MDI_BACKGROUND_APP_FMR, NULL ); //chenhe enable for jessmine replay after interrupt

#endif // __MMI_FM_RADIO__
	
	// initialization after re-power on
	if( !g_mmi_audply_init_done )
	{
		mmi_audply_get_fs_info();
		
		// register mdi background handler
		mdi_audio_set_background_handler(MDI_BACKGROUND_APP_AUDPLY, mmi_audply_background_callback_hdlr ); //chenhe enable for jessmine replay after interrupt
		g_audply.need_restore = FALSE;
		g_audply.wait_next = FALSE;
		g_audply.need_replay = FALSE;
		g_audply.in_list_screen = FALSE;
		g_audply.state = STATE_IDLE;
		g_mmi_audply_play_completed_after_button_down = FALSE;
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
    		lyric_reEntry = MMI_TRUE; 
#endif
		
		g_audply.present_list = mmi_audply_get_preferred_or_available_list();
		
	}

#ifndef 	COOLSAND_MULTIMEDIA_USE_FLASHFS	
	/*liwh Add Start For 000000 Ver: TBM780  on 2007-8-2 17:16 */
	g_audply.present_list = LIST_FROM_CARD;
	g_audply.preferred_list = LIST_FROM_CARD;
	/*liwh Add End  For 000000 Ver: TBM780  on 2007-8-2 17:16 */
#endif	
	if( !g_mmi_audply_init_done || !mmi_audply_does_file_exist( PRESENT_LIST_FILE ) )
		mmi_audply_regen_and_reload_playlist( (BOOL) g_audply.list_auto_gen, TRUE );

	g_mmi_audply_init_done = TRUE;
    
	g_audply.in_main_screen = TRUE;

	mmi_audply_update_main();
 
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_register_down_handler(mmi_audply_pen_down_hdlr);
	mmi_pen_register_up_handler(mmi_audply_pen_up_hdlr);
	mmi_pen_register_move_handler(mmi_audply_pen_move_hdlr);
#endif
 

#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
    mmi_audply_lyric_check_backlight(1);
#endif 
	SetDelScrnIDCallbackHandler( SCR_ID_AUDPLY_MAIN, (HistoryDelCBPtr)mmi_audply_delete_history_hdlr );
}
/*****************************************************************************
* FUNCTION
*     mmi_audply_exit_main
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_exit_main()
{
	history_t currHistory;
	mmi_audply_cancel_fast_timer_no_condition();
	mmi_audply_exit_mainscreen();
	g_audply.in_main_screen = FALSE;
	#if 0
	//chenhe for m039 low power,090313
	if(mmi_audply_get_state()!=STATE_PLAY)
	{
		mmi_audply_do_stop_action();
	}
	#endif
	currHistory.scrnID = SCR_ID_AUDPLY_MAIN;
	currHistory.entryFuncPtr = mmi_audply_entry_main;
	pfnUnicodeStrcpy ( (S8*)currHistory.inputBuffer, (S8*)L"" );
	AddHistory( currHistory );
#if defined(MOTION_SENSOR_SUPPORT)
	if(MotionSensor_GetSetting() == 1)
	{
		motion_sensor_clear_app_id(GSENSOR_APP_SHAKE_AUDIOPLAYER);
		motion_sensor_shake(FALSE);
	}
#endif
#if defined(CAM_MD_SUPPORT)
  csd_stop_sw_motionsensor();
#endif

}

/*****************************************************************************
* FUNCTION
*     mmi_audply_clear_play_seconds
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_clear_play_seconds()
{
	g_audply.const_seconds = 0;
	g_audply.play_seconds = 0;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_start_record_play_seconds
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_start_record_play_seconds()
{
#if 1
	g_audply.const_seconds += g_audply.play_seconds;
	g_audply.play_seconds = 0;
	kal_get_time( &g_audply.play_time );
	mmi_trace(1,"chenhe,mmi_audply_start_record_play_seconds,g_audply.play_time is %d",g_audply.play_time);
#else
	g_audply.const_seconds += g_audply.play_seconds;
	g_audply.play_seconds = 0;
	if(audio_playTime%5 == 0||audio_playTime>5)
	{
		audio_playTime = 0;
	}
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_get_play_seconds
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     S32
* GLOBALS AFFECTED
*     
*****************************************************************************/
S32 mmi_audply_get_play_seconds()
{
	return (S32)((g_audply.const_seconds + g_audply.play_seconds) >>14);
	//return (S32)(g_audply.const_seconds + g_audply.play_seconds) ;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_reset_play_seconds
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     S32
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_reset_play_seconds(U32 play_seconds)
{

	g_audply.const_seconds = play_seconds;
	g_audply.play_seconds = 0;
	kal_get_time( &g_audply.play_time );


}

/*****************************************************************************
* FUNCTION
*     mmi_audply_update_play_seconds
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_update_play_seconds()
{
#if 1
	kal_uint32 current_time;
	S32 old_play_seconds = g_audply.play_seconds;
		
	kal_get_time( &current_time );

	g_audply.play_seconds = current_time - g_audply.play_time;

	#if 0
	if( (g_audply.play_seconds-old_play_seconds)*0.004615 && g_audply.in_main_screen )
	#else
	if( ((g_audply.play_seconds-old_play_seconds)) && g_audply.in_main_screen )
	#endif
	{
	mmi_audply_redraw_main_time();
	mmi_audply_redraw_main_progressbar();
	}
#endif

#if 0	
	//g_audply.play_seconds++;
	audio_playTime++;
	g_audply.play_seconds = audio_playTime/5;
	//mmi_trace(1,"mmi_audply_update_play_seconds,audio_playTime is %d,g_audply.play_seconds is %d,audio_HasplayedTime is %d",audio_playTime,g_audply.play_seconds,audio_HasplayedTime);

	if(g_audply.play_seconds !=audio_HasplayedTime )
	{
		if( g_audply.in_main_screen)
		{
			mmi_audply_redraw_main_time();
		}
		
		audio_HasplayedTime = g_audply.play_seconds;
	}
#endif

#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
        if (g_audply.lyrics_display && mmi_audply_is_lyrics_valid()&& g_audply.in_main_screen )
        {
        	mmi_trace(1,"mmi_audply_lyric_on_time()");
		mmi_audply_lyric_on_time();
        }
#endif /* __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__ */ 
}



/*****************************************************************************
* FUNCTION
*     mmi_audply_timer
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_timer()
{
	if( g_audply.state == STATE_PLAY )
	{
		mmi_audply_update_play_seconds();
		StartTimer( AUDPLY_COUNT_PLAY_TIMER, AUDIO_PLAYER_PLAY_TIMER, mmi_audply_timer );
	}
}
/*****************************************************************************
* FUNCTION
*     mmi_audply_press_stop_button_down
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_press_stop_button_down()
{
	if( g_audply_main_selected_button == AUDPLY_MAIN_DISABLED )
		return;
	
	// change selected button & update state
	g_audply_main_selected_button = (signed char) AUDPLY_MAIN_STOP;
	g_audply_main_selected_button_down = TRUE;
	mmi_audply_cancel_fast_timer_no_condition();

	// update button display
	if (g_audply.in_main_screen)
    {
		mmi_audply_redraw_main_buttons();
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_press_stop_button_up
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
extern audply_main_animation_struct	audply_main_animation;
#endif

void mmi_audply_press_stop_button_up()
{
	if( g_audply_main_selected_button_down && (g_audply_main_selected_button == AUDPLY_MAIN_STOP) )
	{
		BOOL need_redraw_buttons = (g_audply.state == STATE_IDLE );
		#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
		if ( audply_main_animation.img_handle!=GDI_ERROR_HANDLE)
			gdi_image_stop_animation( audply_main_animation.img_handle );
		#endif

		// update selected button state
		g_audply_main_selected_button_down = FALSE;
		
		mmi_audply_stop_playing();
	
		g_mmi_audply_play_completed_after_button_down = FALSE;
		
		// update button display
		if( need_redraw_buttons && g_audply.in_main_screen)
			mmi_audply_redraw_main_buttons();
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_press_play_button_down
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_press_play_button_down()
{
	if( g_audply_main_selected_button == AUDPLY_MAIN_DISABLED )
		return;
    mmi_trace(1,"mmi_audply_press_play_button_down,g_audply.state%d",g_audply.state);
	// update selected button state
	g_audply_main_selected_button_down = TRUE;
	
	if( g_audply.state == STATE_PLAY )
		g_audply_main_selected_button = (signed char) AUDPLY_MAIN_PAUSE;	// change selected button
	else if( g_audply.state == STATE_PAUSED )
		g_audply_main_selected_button = (signed char) AUDPLY_MAIN_PLAY;	// change selected button
	else //  g_audply.state == STATE_IDLE
		g_audply_main_selected_button = (signed char) AUDPLY_MAIN_PLAY;	// change selected button

	mmi_audply_cancel_fast_timer_no_condition();
	// update button display
	if (g_audply.in_main_screen)
    {
		mmi_audply_redraw_main_buttons();
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_press_play_button_up
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
extern void mmi_audply_resume(void);

void mmi_audply_press_play_button_up()
{
     mmi_trace(1,"press_play_button_up,button_down=%d, g_audply.state=%d,selected_button=%d",\
	 	g_audply_main_selected_button_down,g_audply.state,g_audply_main_selected_button);
	if( g_audply_main_selected_button_down && ((g_audply_main_selected_button == AUDPLY_MAIN_PLAY) ||
		(g_audply_main_selected_button == AUDPLY_MAIN_PAUSE)) )
	{	
		// update selected button state
		g_audply_main_selected_button_down = FALSE;
	
		if( g_audply.state == STATE_PLAY )
			{
		#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
              if (g_audply.in_main_screen)
                {
			if ( audply_main_animation.img_handle!=GDI_ERROR_HANDLE)
			gdi_image_stop_animation( audply_main_animation.img_handle );
                 }
		#endif
			mmi_audply_do_pause_action();	// do pause action
			}
		else if( g_audply.state == STATE_PAUSED )
			{
			#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
                    if (g_audply.in_main_screen)
                    {
				// show animation
				gdi_image_draw_animation( audply_main_animation.x, audply_main_animation.y, audply_main_animation.animation_img, &audply_main_animation.img_handle );
                     }
			#endif
			 mmi_audply_resume();//mmi_audply_do_resume_action();	// do resume action
			}
		else // g_audply.state == STATE_IDLE
			{
			 #ifdef __MMI_AUDIO_PLAYER_ANIMATION__
                         if (g_audply.in_main_screen)
                        {
				if ( audply_main_animation.img_handle!=GDI_ERROR_HANDLE)
					gdi_image_stop_animation( audply_main_animation.img_handle );
				gdi_image_draw_animation( audply_main_animation.x, audply_main_animation.y, audply_main_animation.animation_img, &audply_main_animation.img_handle );
                       }
			#endif
			mmi_audply_play();	// do play action
			}
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_press_prev_button_down
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_press_prev_button_down()
{
	if( g_audply_main_selected_button == AUDPLY_MAIN_DISABLED )
		return;
	
	// change selected button & update state
	g_audply_main_selected_button = (signed char) AUDPLY_MAIN_PREV;
	g_audply_main_selected_button_down = TRUE;
	mmi_audply_cancel_fast_timer_no_condition();

	// update button display
	if (g_audply.in_main_screen)
    {
		mmi_audply_redraw_main_buttons();
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_press_prev_button_up
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_press_prev_button_up()
{
    mmi_trace(g_sw_MP3, "mmi_audply_press_prev_button_up,selected_button_down=%d,g_audply_main_selected_button=%d,g_audply.state=%d,g_audply.need_replay=%d,GetHandsetInPhone=%d"\
    ,g_audply_main_selected_button_down,g_audply_main_selected_button,g_audply.state,g_audply.need_replay,GetHandsetInPhone());
	if( g_audply_main_selected_button_down && (g_audply_main_selected_button == AUDPLY_MAIN_PREV) )
	{	
		// update selected button state
		g_audply_main_selected_button_down = FALSE;

	 #ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
            gui_cancel_timer(mmi_audply_scroll_lyric_handler);
        #endif 
		
		// pick the prev song
#if defined(__PROJECT_GALLITE_C01__)
		if( g_audply.random_mode )
		{
		    mmi_audply_pick_playlist_next();
		}
		else
		{
		    mmi_audply_pick_playlist_prev();
		}
#else
		mmi_audply_pick_playlist_prev();
#endif		
		if( mmi_audply_apply_picked_file() )
		{

			
			if( g_audply.state == STATE_PLAY || //g_audply.state == STATE_PAUSED || //jiashuo del for greenstone bug ,20080903
				g_mmi_audply_play_completed_after_button_down || g_audply.need_replay )
			{
				mdi_audio_stop_file();//chenhe,fix bug 11110 ,090306
				StopTimer( AUDPLY_DELAY_PLAY_TIMER ); // stop previous play request
				StartTimer( AUDPLY_DELAY_PLAY_TIMER, 700, mmi_audply_play ); // wait for 0.7 second to request play
				g_audply.wait_next = TRUE;
			}
			//jiashuo add for greenstone bug ,20080903
			else if	( g_audply.state == STATE_PAUSED )
			{
			    mmi_audply_stop_playing();
			}

			// update main title, file_index
			if (g_audply.in_main_screen)
            {
            	 #ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
                    mmi_audply_stop_scroll_lyric_and_clean_screen();
                #endif 
				mmi_audply_redraw_main_title();
				mmi_audply_redraw_main_file_index();
				mmi_audply_stopped();
			}
		}
		mdi_audio_set_file_data_progress(0);
        
        audioplayer_restore_flag = 0;

		g_mmi_audply_play_completed_after_button_down = FALSE;
		
		// update main buttons
		if (g_audply.in_main_screen)
        {
			mmi_audply_redraw_main_buttons();
		}
	}
	else if( g_audply_main_selected_button_down && (g_audply_main_selected_button == AUDPLY_MAIN_BACKWARD_LONG_PRESS) )
		{
			// update selected button state
			g_audply_main_selected_button_down = FALSE;
			mmi_audply_cancel_fast_timer_handel();
			if (g_audply.in_main_screen)
        		{
				mmi_audply_fast_timer_redraw_icon();
			}
		}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_press_next_button_down
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_press_next_button_down()
{
	if( g_audply_main_selected_button == AUDPLY_MAIN_DISABLED )
		return;
	
	// change selected button & update state
	g_audply_main_selected_button = (signed char) AUDPLY_MAIN_NEXT;
	g_audply_main_selected_button_down = TRUE;
	mmi_audply_cancel_fast_timer_no_condition();

	// update button display
	if (g_audply.in_main_screen)
    {
		mmi_audply_redraw_main_buttons();
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_press_next_button_up
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_press_next_button_up()
{
		mmi_trace(g_sw_MP3,"next_button_up,g_audply.state =%d,after_button_down=%d,g_audply.need_replay =%d",g_audply.state,g_mmi_audply_play_completed_after_button_down ,g_audply.need_replay );

	if( g_audply_main_selected_button_down && (g_audply_main_selected_button == AUDPLY_MAIN_NEXT) )
	{
		// update selected button state
		g_audply_main_selected_button_down = FALSE;
		
	#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
            gui_cancel_timer(mmi_audply_scroll_lyric_handler);
        #endif 
		// pick the next song
		mmi_audply_pick_playlist_next();
		
		if( mmi_audply_apply_picked_file() )
		{

			
			if( g_audply.state == STATE_PLAY || //g_audply.state == STATE_PAUSED || //jiashuo del for greenstone bug ,20080903
				g_mmi_audply_play_completed_after_button_down || g_audply.need_replay )
			{
                mdi_audio_stop_file();     // modified by kecx for fixing bug 11521 on 20090611
				StopTimer( AUDPLY_DELAY_PLAY_TIMER );
				StartTimer( AUDPLY_DELAY_PLAY_TIMER, 700, mmi_audply_play ); // wait for 0.7 second to request play
				g_audply.wait_next = TRUE;
			}
			//jiashuo add for greenstone bug ,20080903
			else if	( g_audply.state == STATE_PAUSED )
			{
			    mmi_audply_stop_playing();
			}

			// update main title, file_index
			if (g_audply.in_main_screen)
            {
            		#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
                    		mmi_audply_stop_scroll_lyric_and_clean_screen();
                	#endif 
				mmi_audply_redraw_main_title();
				mmi_audply_redraw_main_file_index();
				mmi_audply_stopped();
			}
		}
	    audioplayer_restore_flag = 0;


		g_mmi_audply_play_completed_after_button_down = FALSE;
		
		// update main buttons
		if (g_audply.in_main_screen)
        {
			mmi_audply_redraw_main_buttons();
		}
	}
	else if( g_audply_main_selected_button_down && (g_audply_main_selected_button == AUDPLY_MAIN_FORWARD_LONG_PRESS) )
	{
			// update selected button state
			g_audply_main_selected_button_down = FALSE;
			mmi_audply_cancel_fast_timer_handel();
			if (g_audply.in_main_screen)
        		{
				mmi_audply_fast_timer_redraw_icon();
			}
	}
}

#if defined(__PROJECT_GALLITE_C01__)
void mmi_audply_press_inc_volume_up()
{
	mmi_audply_redraw_main_volumectrl(1,FALSE);
}
void mmi_audply_press_dec_volume_up()
{
	mmi_audply_redraw_main_volumectrl(0,FALSE);
}
#endif
/*****************************************************************************
* FUNCTION
*     mmi_audply_press_inc_volume
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_press_inc_volume()
{
	if( g_audply.volume < MAX_VOL_LEVEL)
	{
                S16 error;
         //beginning--modified by kecx for fixing bug 12951 on 20090707       
                g_audply.volume++;
                if (g_audply.state != STATE_IDLE)
                {
                    mdi_audio_set_volume( VOL_TYPE_MEDIA, g_audply.volume );
                }
                if( g_audply.in_main_screen )
                {
#if defined(__PROJECT_GALLITE_C01__)                
                    mmi_audply_redraw_main_volumectrl(1,TRUE);
#endif					
                    mmi_audply_redraw_main_volumebar();
                }
         //end--modified by kecx for fixing bug 12951 on 20090707       
                WriteValue( NVRAM_AUDPLY_VOLUME, &g_audply.volume, DS_BYTE, &error );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_press_dec_volume
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_press_dec_volume()
{
        if( g_audply.volume > 0 )
        {
                S16 error;
         //beginning--modified by kecx for fixing bug 12951 on 20090707       
                g_audply.volume--;
                if (g_audply.state != STATE_IDLE)
                    {
                        mdi_audio_set_volume( VOL_TYPE_MEDIA, g_audply.volume );
                    }
                if( g_audply.in_main_screen )
                    { 
#if defined(__PROJECT_GALLITE_C01__)                
                    	   mmi_audply_redraw_main_volumectrl(0,TRUE);
#endif					
                        mmi_audply_redraw_main_volumebar();
                    }
         //end--modified by kecx for fixing bug 12951 on 20090707       
                WriteValue( NVRAM_AUDPLY_VOLUME, &g_audply.volume, DS_BYTE, &error );
        }
}
#if defined(MEM_ULC_3216) // wangmg: disable icon show
extern		BOOL dynamic_item_text_align_left;
#endif
/*****************************************************************************
* FUNCTION
*     mmi_audply_entry_list
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_entry_list()
{
	U8* guiBuffer;
	U16 title_id;
	S32 highlighted_item = 0;
	
	EntryNewScreen( SCR_ID_AUDPLY_PLAYLIST, mmi_audply_exit_list, NULL, NULL );
	
	g_audply.in_list_screen = TRUE;
	
//chenhe ++
#if 0
   media_vid_open_file_req_struct* msg_p = NULL;
   ilm_struct  *ilm_ptr = NULL;

   msg_p = (media_vid_open_file_req_struct*)
            construct_local_para(sizeof(media_vid_open_file_req_struct), TD_CTRL);  
                            
   msg_p->file_name_p = (kal_wchar*)"aaa";
   msg_p->seq_num=1;
   msg_p->cache_in_mem=1;
   
   ilm_ptr = allocate_ilm(MOD_MMI);
   ilm_ptr->src_mod_id = MOD_MMI;
   ilm_ptr->dest_mod_id = MOD_MED;
   ilm_ptr->sap_id = MED_SAP;
   
   ilm_ptr->msg_id = (msg_type)MSG_ID_MEDIA_VID_OPEN_FILE_REQ;
   ilm_ptr->local_para_ptr = (local_para_struct*)msg_p;
   ilm_ptr->peer_buff_ptr = NULL;

   msg_send_ext_queue(ilm_ptr);
   mmi_trace(1,"mmi_audply_entry_list,send vdo message successfully,MSG_ID_MEDIA_VID_OPEN_FILE_REQ is %d",MSG_ID_MEDIA_VID_OPEN_FILE_REQ);
#endif
	 
//chenhe --


	// title
	if( phoneDrvLetter > 0 || cardDrvLetter > 0 
        /*+ zhouqin modify for dual t-flash card 20110503*/
        #if defined(DUAL_TFLASH_SUPPORT)
            ||seccardDrvLetter > 0
        #endif
        /*- zhouqin modify for dual t-flash card 20110503*/
        )
	{
	//if(  cardDrvLetter > 0 )
		/*fix bug 13581 by liuxl on 20090803*/
		if((cardDrvLetter < 0) && (LIST_FROM_CARD == g_audply.preferred_list))
		{
			title_id = STR_ID_AUDPLY_LIST_FROM_PHONE;
			g_audply.preferred_list = LIST_FROM_PHONE; 
		}
        /*+ zhouqin modify for dual t-flash card 20110503*/
        #if defined(DUAL_TFLASH_SUPPORT)
        else if((seccardDrvLetter < 0) && (LIST_FROM_SEC_CARD == g_audply.preferred_list))
		{
			title_id = STR_ID_AUDPLY_LIST_FROM_PHONE;
			g_audply.preferred_list = LIST_FROM_PHONE; 
		}
        #endif
        /*- zhouqin modify for dual t-flash card 20110503*/
		else
        /*+ zhouqin modify for dual t-flash card 20110503*/
        {      
            #if defined(DUAL_TFLASH_SUPPORT)
                title_id = (g_audply.present_list == LIST_FROM_PHONE) ? STR_ID_AUDPLY_LIST_FROM_PHONE : ((g_audply.present_list == LIST_FROM_CARD) ? STR_ID_AUDPLY_LIST_FROM_CARD : STR_ID_AUDPLY_LIST_FROM_SEC_CARD);
            #else
                title_id = (g_audply.present_list == LIST_FROM_PHONE) ? STR_ID_AUDPLY_LIST_FROM_PHONE : STR_ID_AUDPLY_LIST_FROM_CARD;
            #endif
        }
        /*- zhouqin modify for dual t-flash card 20110503*/
	}
	else
		title_id = STR_ID_AUDPLY_PLAYLIST;
    
        /*+ zhouqin modify for dual t-flash card 20110503*/
        #if defined(DUAL_TFLASH_SUPPORT)
        if((cardDrvLetter < 0) && (LIST_FROM_CARD == g_audply.preferred_list))
		{
			g_audply.preferred_list = LIST_FROM_PHONE; 
		}
        else if((seccardDrvLetter < 0) && (LIST_FROM_SEC_CARD == g_audply.preferred_list))
		{
			g_audply.preferred_list = LIST_FROM_PHONE; 
		}
        #else
	        if(cardDrvLetter<0)
	        {
		    g_audply.present_list = LIST_FROM_PHONE;
	        }
        #endif
        /*- zhouqin modify for dual t-flash card 20110503*/  
	
	// register highligh handler
	RegisterHighlightHandler( mmi_audply_list_highlight_hdlr );
	
	// show category screen
	if( g_audply.n_total_items == 0 )
	{
		ShowCategory52Screen( title_id, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
			STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
			STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			0, NULL, NULL, NULL, 1, 0, NULL );
	}
	else
	{
		// if we delete some items and the previous highlighted item is out of boundary, set it to the last one
		if( (guiBuffer = GetCurrGuiBuffer( SCR_ID_AUDPLY_PLAYLIST )) != NULL )
		{
			list_menu_category_history *hist = (list_menu_category_history*)guiBuffer;
			if( hist->highlighted_item >= g_audply.n_total_items )
				hist->highlighted_item = g_audply.n_total_items - 1;
		}
		else if( g_audply.pick_index >= 0 ) // highlighted item
			highlighted_item = g_audply.pick_index;
		else
			highlighted_item = 0; // this should never happen
#if defined(MEM_ULC_3216) // wangmg: disable icon show
		dynamic_item_text_align_left = TRUE;
#endif

		ShowCategory184Screen( title_id, MAIN_MENU_TITLE_MULTIMEDIA_ICON,	// title text and icon
			STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,				// LSK text and icon
			STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 						// RSK text and icon
			g_audply.n_total_items, mmi_audply_list_getitem, NULL,
			highlighted_item, guiBuffer );
	}
	
	// register LSK, RSK hanlders
	SetLeftSoftkeyFunction( mmi_audply_entry_option, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	
	// register left right arrow key handler
	SetKeyHandler( mmi_audply_entry_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
	
	// register volume inc, dec handlers
	SetKeyHandler( mmi_audply_press_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_exit_list
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_exit_list()
{
	history_t currHistory;
	
	g_audply.in_list_screen = FALSE;
	
	currHistory.scrnID = SCR_ID_AUDPLY_PLAYLIST;
	currHistory.entryFuncPtr = mmi_audply_entry_list;
	GetCategoryHistory( currHistory.guiBuffer );
	pfnUnicodeStrcpy ( (S8*)currHistory.inputBuffer, (S8*)L"" );
	AddHistory( currHistory );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_list_highlight_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S32 index
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_list_highlight_hdlr( S32 index )
{
	g_mmi_audply_list_highlighted_item = (U16)index;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_lookup_audio_file_format
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     UI_string_type file_ext
* RETURNS
*     S32
* GLOBALS AFFECTED
*     
*****************************************************************************/
S32 mmi_audply_lookup_audio_file_format( UI_string_type file_ext )
{
#if 0
	S32 i, j;
	for( i = 0; i < NO_AUDIO_FILE_TYPES; i++ )
	{
		BOOL match = TRUE;
		for( j = 0; j < 4; j++ ) // it's is not necessary to check j < 4, but for safety
		{
			UI_character_type file_ext_char = file_ext[j];
			if( file_ext_char != audio_file_lower_exts[i][j] && file_ext_char != audio_file_upper_exts[i][j] )
			{
				match = FALSE;
				break;
			}
			else if( file_ext_char == 0 ) // both are end and length less than 4
				return i;
		}
		if( match )
			return i;
	}
	
	return -1;
#endif
#if 0
       S8 mp3format1[5] = 	"MP3";
	S8 mp3format2[5] = "mp3";
	S8 mp3format3[5] = "Mp3";
	S8 mp3format4[5] = "mP3";

	S8 afileext[10];

	UnicodeToAnsii(afileext, file_ext);

	if(strcmp(afileext, mp3format1) == 0 || strcmp(afileext, mp3format2) == 0 ||strcmp(afileext, mp3format3) == 0 ||strcmp(afileext, mp3format4) == 0 )
		return TRUE;

	return FALSE;
	#endif 

	//chenhe add for jasperII+
	S8 tempAnsii[20]={0};
	UINT16 i=0;
	UnicodeToAnsii(tempAnsii, (S8*)file_ext);
	while(tempAnsii[i] != 0)
	{
		if(tempAnsii[i]>='a' &&tempAnsii[i]<='z' )
		tempAnsii[i]=tempAnsii[i]-('a'-'A');
		i++;
	}
//	mmi_trace(0,"mmi_audply_lookup_audio_file_format,222,tempAnsii is %s",tempAnsii);

	if(strcmp(tempAnsii,"WAV")==0
#ifdef DAF_DECODE
		||strcmp(tempAnsii,"MP3")==0 
#endif /* DAF_DECODE */
		||strcmp(tempAnsii,"AMR")==0 
#ifdef AAC_DECODE
		||strcmp(tempAnsii,"AAC")==0
#endif /* AAC_DECODE */
#ifdef WMA_DECODE
		||strcmp(tempAnsii,"WMA")==0
#endif /* WMA_DECODE */
#ifndef TARGET_MIDI_SUPPORT_OFF
		||strcmp(tempAnsii,"MID")==0
#endif
		)
	{
	 return TRUE;
	}
	else
	{
		return FALSE;
	}
	//chenhe add for jasperII-	
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_list_getitem
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S32 item_index
*     UI_string_type str_buff
*     UI_image_type* img_buff_p
*     U8 str_img_mask
* RETURNS
*     pBOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
pBOOL mmi_audply_list_getitem( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	S32 file_ext_index;
	UI_character_type filename[FMGR_MAX_PATH_LEN+1];
	UI_character_type ext[FMGR_MAX_EXT_LEN+1];
		
	// get file name without ext
	mmi_audply_playlist_get_filename( filename, item_index );

	mmi_audply_split_filename_ext( filename, str_buff, ext );
	file_ext_index = mmi_audply_lookup_audio_file_format( ext );
	#ifdef __MMI_SHOW_FILE_EXT__
	#if !defined(__MMI_SHOW_DAF_FILE_EXT__) && defined(DAF_DECODE)
	// hide daf but show others ext
	if( file_ext_index != 2 )
	#endif
	{
		if( ext[0] != 0 ) // if its ext is not blank
		{
			pfnUnicodeStrcat( (S8*) str_buff, (S8*)L"." );
			pfnUnicodeStrcat( (S8*) str_buff, (S8*)ext );
		}
	}
	#endif // __MMI_SHOW_FILE_EXT__


	*img_buff_p = (UI_image_type) GetImage( IMG_ID_AUDPLY_FILE_AUD_ICON );
#if defined(MEM_ULC_3216)
	*img_buff_p = UI_NULL_IMAGE;
#else
	*img_buff_p = (UI_image_type) GetImage( IMG_ID_AUDPLY_FILE_AUD_ICON );
#endif
    
#if 0
	// get ext icon
	if( file_ext_index >= 0 )
		*img_buff_p = (UI_image_type) GetImage( audio_file_icon_ids[file_ext_index] );
	else
		*img_buff_p = NULL;
#endif

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_entry_option
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_entry_option()
{
 	S32 n_items;
 	U16 item_icons[7];
	U8* guiBuffer;

	EntryNewScreen( SCR_ID_AUDPLY_OPTION, mmi_audply_exit_option, NULL, NULL );

	// gui buffer
	guiBuffer = GetCurrGuiBuffer( SCR_ID_AUDPLY_OPTION );
	
 	// items
	if( g_audply.list_auto_gen )
	{
		if( g_audply.n_total_items == 0 )
		{
			// regenerate list, settings
			option_texts[0] =  STR_ID_AUDPLY_OPTION_REGEN_LIST;
			item_icons[0] = IMG_GLOBAL_L1;
			option_texts[1] =  STR_ID_AUDPLY_OPTION_SETTINGS;
			item_icons[1] = IMG_GLOBAL_L2;
			n_items = 2;
		}
		else
		{
			// play, detail, add to rings, regenerate list, settings
			option_texts[0] = STR_ID_AUDPLY_OPTION_PLAY;
			item_icons[0] = IMG_GLOBAL_L1;
			option_texts[1] = STR_ID_AUDPLY_OPTION_DETAIL;
			item_icons[1] = IMG_GLOBAL_L2;
			option_texts[2] = STR_ID_AUDPLY_OPTION_ADD_TO_RINGS;
			item_icons[2] = IMG_GLOBAL_L3;
			option_texts[3] =  STR_ID_AUDPLY_OPTION_REGEN_LIST;
			item_icons[3] = IMG_GLOBAL_L4;
			option_texts[4] = STR_ID_AUDPLY_OPTION_SETTINGS;
			item_icons[4] = IMG_GLOBAL_L5;
			n_items = 5;
		}
	}
	else
	{
		if( g_audply.n_total_items == 0 )
		{
			// add, settings
			option_texts[0] = STR_ID_AUDPLY_OPTION_ADD;
			item_icons[0] = IMG_GLOBAL_L1;
			option_texts[1] = STR_ID_AUDPLY_OPTION_SETTINGS;
			item_icons[1] = IMG_GLOBAL_L2;
			n_items = 2;
		}
		else
		{
			// play, detail, add to rings, add, remove, remove all, settings
			option_texts[0] = STR_ID_AUDPLY_OPTION_PLAY;
			item_icons[0] = IMG_GLOBAL_L1;
			option_texts[1] = STR_ID_AUDPLY_OPTION_DETAIL;
			item_icons[1] = IMG_GLOBAL_L2;
			option_texts[2] = STR_ID_AUDPLY_OPTION_ADD_TO_RINGS;
			item_icons[2] = IMG_GLOBAL_L3;
			option_texts[3] = STR_ID_AUDPLY_OPTION_ADD;
			item_icons[3] = IMG_GLOBAL_L4;
			option_texts[4] = STR_ID_AUDPLY_OPTION_REMOVE;
			item_icons[4] = IMG_GLOBAL_L5;
			option_texts[5] = STR_ID_AUDPLY_OPTION_REMOVE_ALL;
			item_icons[5] = IMG_GLOBAL_L6;
			option_texts[6] = STR_ID_AUDPLY_OPTION_SETTINGS;
			item_icons[6] = IMG_GLOBAL_L7;
			n_items = 7;
		}
	}
	
	// register highligh handler
	RegisterHighlightHandler( mmi_audply_option_highlight_hdlr );
	
	// show category screen
	ShowCategory15Screen( STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_MULTIMEDIA_ICON,	// title text and icon
		STR_GLOBAL_OK, IMG_GLOBAL_OK,								// LSK text and icon
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 							// RSK text and icon
		n_items, option_texts, item_icons, LIST_MENU, 0, guiBuffer );
	
	// register LSK, RSK hanlders
	SetLeftSoftkeyFunction( mmi_audply_option_LSK, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	
	// register left, right key handlers
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_option_right_arrow_key, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	
	// register volume inc, dec handlers
	SetKeyHandler( mmi_audply_press_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
} 

/*****************************************************************************
* FUNCTION
*     mmi_audply_exit_option
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_exit_option()
{
/*
	history currHistory;
	currHistory.scrnID = SCR_ID_AUDPLY_OPTION;
	currHistory.entryFuncPtr = mmi_audply_entry_option;
	GetCategoryHistory( currHistory.guiBuffer );
	pfnUnicodeStrcpy( (S8*)currHistory.inputBuffer, (S8*)L"" );
	AddHistory( currHistory );
*/
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_option_highlight_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S32 index
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_option_highlight_hdlr( S32 index )
{
	g_mmi_audply_option_highlighted_item = (U16)index;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_option_right_arrow_key
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_option_right_arrow_key()
{
	U16 highlighted_item_text_id = option_texts[g_mmi_audply_option_highlighted_item];
	if( highlighted_item_text_id != STR_ID_AUDPLY_OPTION_ADD_TO_RINGS &&
		highlighted_item_text_id != STR_ID_AUDPLY_OPTION_REGEN_LIST )
		mmi_audply_option_LSK();
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_show_update_list_result
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     U8 result
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_show_update_list_result( U8 result )
{
	switch( result )
	{
	case 0: // success
		DisplayPopup( (PU8)GetString( STR_GLOBAL_DONE ), IMG_GLOBAL_ACTIVATED, 1, 1000, (U8)SUCCESS_TONE );
		break;
	case 1: // disk full
		DisplayPopup( (PU8)GetString( STR_ID_AUDPLY_CANNOT_UPDATE_LIST_DISK_FULL ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		break;
	case 2: // access file error
		DisplayPopup( (PU8)GetString( STR_ID_AUDPLY_CANNOT_UPDATE_LIST_FS_FAIL ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		break;
	case 3: // list full
		DisplayPopup( (PU8)GetString( STR_ID_AUDPLY_LIST_FULL ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		break;	
	case 4:
		DisplayPopup( (PU8)GetString( STR_ID_AUDPLY_UNSUPPORTED_TYPE ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		break;	
	default:
		mmi_trace(1,"mmi_audply_show_update_list_result,error");
		break;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_option_LSK
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_option_LSK()
{
	FMGR_FILTER filter;
	U8 result;
     mmi_trace(1,"mmi_audply_option_LSK,g_audply.list_auto_gen =%d,g_audply.n_total_items=%d,g_mmi_audply_option_highlighted_item=%d",
	 	g_audply.list_auto_gen ,g_audply.n_total_items,g_mmi_audply_option_highlighted_item);
	if( g_audply.list_auto_gen )
	{
		if( g_audply.n_total_items == 0 )
		{
			switch( g_mmi_audply_option_highlighted_item )
			{
			case 0: // regenerate list
				result = mmi_audply_regen_and_reload_playlist( TRUE, TRUE );
				// clear play list gui buffer
				HistoryReplace( SCR_ID_AUDPLY_PLAYLIST, SCR_ID_AUDPLY_PLAYLIST, mmi_audply_entry_list );
				mmi_audply_show_update_list_result( result );
				break;
			case 1: // settings
				mmi_audply_entry_settings();
				break;
			}
		}
		else
		{
			switch( g_mmi_audply_option_highlighted_item )
			{
			case 0: // play
				mmi_audply_start_playlist_at( g_mmi_audply_list_highlighted_item );
				mmi_audply_pick_playlist_at( g_mmi_audply_list_highlighted_item );
				if( mmi_audply_apply_picked_file() )

				{
					/*//ADD BY LIWH
					g_audply.state = STATE_PLAY;
					mmi_audply_stop_playing();////add by li for temp
					g_audply.state = STATE_IDLE;
					//mmi_audply_do_play_action_and_back_to_main_screen();
					StopTimer( AUDPLY_DELAY_PLAY_TIMER );
					StartTimer( AUDPLY_DELAY_PLAY_TIMER, 700, mmi_audply_do_play_action_and_back_to_main_screen );
					//	mmi_audply_do_play_action_and_back_to_main_screen();*/
					mmi_audply_stop_playing(); 
					GoBackToHistory(SCR_ID_AUDPLY_MAIN); 
					mmi_audply_play(); 


				}
				break;
			case 1: // detail
				mmi_audply_entry_detail();
				break;
			case 2: // add to rings
			#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
				PmgSetShortFileName( mmi_audply_playlist_get_fullfilename( fullfilename_temp, g_mmi_audply_list_highlighted_item ) == 2 );
				if (!mmi_audply_does_file_exist(fullfilename_temp))
				{
					DisplayPopup(
						(PU8) GetString(STR_ID_AUDPLY_OPEN_FILE_ERROR),
						IMG_GLOBAL_ERROR,
						0,
						1000,
						(U8) ERROR_TONE);
				}
				else
			#if defined( __DRM_SUPPORT__ ) && defined( MMI_ON_HARDWARE_P )
				if( !mmi_profiles_check_DRM_content_forwardable( fullfilename_temp ) )
					DisplayPopup( (PU8)GetString( STR_GLOBAL_DRM_PROHIBITED ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
				else
			#endif
					PmgFmgrAddExtMelodyFunc( (S8 *)fullfilename_temp );
			#endif
				break;
			case 3: // regenerate list
				result = mmi_audply_regen_and_reload_playlist( TRUE, TRUE );
				// clear play list gui buffer
				HistoryReplace( SCR_ID_AUDPLY_PLAYLIST, SCR_ID_AUDPLY_PLAYLIST, mmi_audply_entry_list );
				mmi_audply_show_update_list_result( result );
				break;
			case 4: // settings
				mmi_audply_entry_settings();
				break;
			}
		}
	}
	else
	{
		if( g_audply.n_total_items == 0 )
		{
			switch( g_mmi_audply_option_highlighted_item )
			{
			case 0: // add
#ifdef __MMI_FILE_MANAGER__
				
				FMGR_FILTER_INIT(&filter);
				FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
				FMGR_FILTER_SET_AUDIO(&filter);
				FMGR_FILTER_CLEAR(&filter,FMGR_TYPE_M4A);
				mmi_fmgr_select_path_and_enter( APP_AUDIOPLAYER, FMGR_SELECT_FILE_OR_FOLDER | FMGR_SELECT_REPEAT,
				filter, (PS8) PRESENT_LIST_ROOT, (PsExtFuncPtr) mmi_audply_add_file_hdlr );
#endif // __MMI_FILE_MANAGER__
				break;
			case 1: // settings
				mmi_audply_entry_settings();
				break;
			}
		}
		else
		{
			switch( g_mmi_audply_option_highlighted_item )
			{
			case 0: // play
				mmi_audply_start_playlist_at( g_mmi_audply_list_highlighted_item );
				mmi_audply_pick_playlist_at( g_mmi_audply_list_highlighted_item );
				if( mmi_audply_apply_picked_file() )
					{
						g_audply.state = STATE_PLAY;
		            mmi_audply_stop_playing();////add by li for temp

		            g_audply.state = STATE_IDLE;
					//mmi_audply_do_play_action_and_back_to_main_screen();

					StopTimer( AUDPLY_DELAY_PLAY_TIMER );
		//	       StartTimer( AUDPLY_DELAY_PLAY_TIMER, 700, mmi_audply_do_play_action_and_back_to_main_screen );
					//mmi_audply_do_play_action_and_back_to_main_screen();}
                     GoBackToHistory(SCR_ID_AUDPLY_MAIN);
                        mmi_audply_play();
					
				break;
					}
				
			case 1: // detail
				mmi_audply_entry_detail();
				break;
			case 2: // add to rings
			#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
				PmgSetShortFileName( mmi_audply_playlist_get_fullfilename( fullfilename_temp, g_mmi_audply_list_highlighted_item ) == 2 );
				if (!mmi_audply_does_file_exist(fullfilename_temp))
				{
					DisplayPopup(
						(PU8) GetString(STR_ID_AUDPLY_OPEN_FILE_ERROR),
						IMG_GLOBAL_ERROR,
						0,
						1000,
						(U8) ERROR_TONE);
				}
				else
			#if defined( __DRM_SUPPORT__ ) && defined( MMI_ON_HARDWARE_P )
				if( !mmi_profiles_check_DRM_content_forwardable( fullfilename_temp ) )
					DisplayPopup( (PU8)GetString( STR_GLOBAL_DRM_PROHIBITED ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
				else
			#endif
					PmgFmgrAddExtMelodyFunc( (S8 *)fullfilename_temp );
			#endif // __MMI_PROFILE_EXTMELODY_SUPPORT__
				break;
			case 3: // add
#ifdef __MMI_FILE_MANAGER__
				FMGR_FILTER_INIT(&filter);
				FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
				FMGR_FILTER_SET_AUDIO(&filter);
				FMGR_FILTER_CLEAR(&filter,FMGR_TYPE_M4A);
				mmi_fmgr_select_path_and_enter( APP_AUDIOPLAYER, FMGR_SELECT_FILE_OR_FOLDER | FMGR_SELECT_REPEAT,
					filter, (PS8) PRESENT_LIST_ROOT, (PsExtFuncPtr) mmi_audply_add_file_hdlr );
#endif // __MMI_FILE_MANAGER__
				break;
			case 4: // remove
				mmi_audply_entry_remove_confirm();
				break;
			case 5: // remove all
				mmi_audply_entry_remove_all_confirm();
				break;
			case 6: // settings
				mmi_audply_entry_settings();
				break;
			}
		}
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_process_add_files_in_folder
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_process_add_files_in_folder( void )
{
	
	
	// folder
	S32 i;
	FS_DOSDirEntry file_info;
	UI_character_type filename[FMGR_MAX_PATH_LEN+1] = {0};
	UI_character_type ext[FMGR_MAX_EXT_LEN+1] = {0};
	UI_character_type filexpr_temp[FMGR_MAX_PATH_LEN+1] = {0};
	S8 filefliter[10] = "/*.*";
	S8 ufliefliter[20];
	S8 gang[4] = "/";
	S8 ugang[8] = {0};

	//UnicodeToAnsii((S8*)ufliefliter, (S8*)filefliter);
	AnsiiToUnicodeString((S8*)ufliefliter, (S8*)filefliter);
	AnsiiToUnicodeString((S8*)ugang, (S8*)gang);


	TBM_ENTRY(0x2A25);
        
	
#if 0
	if( add_file_cursor < 0 )
	{
		// first time
		pfnUnicodeStrcpy( (S8*) filexpr_temp, (S8*) add_file_path );
		pfnUnicodeStrcat( (S8*) filexpr_temp, (S8*) ufliefliter );
		add_file_cursor = MMI_FS_FindFirst( filexpr_temp, 0, 0, &file_info, filename, (FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH );
		if( add_file_cursor < 0 )
		{
			mmi_audply_show_update_list_result( 0 );
			return;
		}
	}
	else if( MMI_FS_FindNext( add_file_cursor, &file_info, filename, (FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH ) != FS_NO_ERROR )
	{
		// next time
		mmi_audply_process_add_files_end();
		return;
	}

	i = 0;
	do
	{
		if( (file_info.Attributes & (FS_ATTR_DIR|FS_ATTR_VOLUME|FS_ATTR_HIDDEN|FS_ATTR_SYSTEM)) == 0 )
		{
			is_shortname = ((file_info.NTReserved & FS_SFN_MATCH) != 0);
			if( is_shortname )
			{
				for( j = 0; j < 3; j++ )
					ext[j] = (UI_character_type) file_info.Extension[j];
				ext[j] = 0;
			}
			else
				mmi_audply_extract_ext( filename, ext );
			if( mmi_audply_lookup_audio_file_format( ext ) >= 0 )
			{
				if( add_file_path_len + pfnUnicodeStrlen( (S8*) filename ) <= FMGR_MAX_PATH_LEN )
				{
					pfnUnicodeStrcpy( (S8*) fullfilename_temp, (S8*) add_file_path );
					pfnUnicodeStrcat( (S8*) fullfilename_temp, (S8*) filename );
					if( is_shortname )
						MARK_SNAME( fullfilename_temp );
					else
						MARK_LNAME( fullfilename_temp );
					if( g_audply.n_total_items >= MAX_PLAY_LIST_NUM )
					{
						add_file_result = 3;
						break;
					}
					else if( (add_file_result = mmi_audply_playlist_append_one( (S8*)fullfilename_temp )) != 0 )
						break;
					else
						add_file_result = 0;
				}
			}
		}
		if( (++i) == 10 )
		{
			StartTimer( AUDPLY_PROCESS_ADD_FILE_TIMER, 100, mmi_audply_process_add_files_in_folder );
			return;
		}
	} while( MMI_FS_FindNext( add_file_cursor, &file_info, filename, (FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH ) == FS_NO_ERROR );
	mmi_audply_process_add_files_end();
	#endif
	
	if( add_file_cursor < 0 )
	{
		// first time
		pfnUnicodeStrcpy( (S8*) filexpr_temp, (S8*) add_file_path );
		pfnUnicodeStrcat( (S8*) filexpr_temp, (S8*) ufliefliter );

		
		add_file_cursor = MMI_FS_FindFirst((const UINT8 *) filexpr_temp, 0, 0, &file_info, (UINT8*)filename, (INT32)(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH );

		
		if( add_file_cursor < 0 )
			
		{
			mmi_audply_show_update_list_result( 0 );

			TBM_EXIT(0x2A25);
			return;
		}
		
	}
	else if( MMI_FS_FindNext( add_file_cursor, &file_info, (UINT8*)filename, (FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH ) != FS_NO_ERROR )
	{
		// next time
		mmi_audply_process_add_files_end();

		TBM_EXIT(0x2A25);
		return;
	}
	
	
	i = 0;
	do
	{
	    if(!(file_info.Attributes == 16) && add_file_cursor >=0 )
	    	{
		//if( (file_info.Attributes & (FS_ATTR_DIR|FS_ATTR_VOLUME|FS_ATTR_HIDDEN|FS_ATTR_SYSTEM)) == 0 )
		{
		#if 0
			is_shortname = ((file_info.NTReserved & FS_SFN_MATCH) != 0);
			if( is_shortname )
			{
				for( j = 0; j < 3; j++ )
					ext[j] = (UI_character_type) file_info.Extension[j];
				ext[j] = 0;
			}
			else
		#endif		
				mmi_audply_extract_ext( filename, ext );
			if( mmi_audply_lookup_audio_file_format( ext ) >0 )
			{
				
				if( add_file_path_len + pfnUnicodeStrlen( (S8*) filename ) <= FMGR_MAX_PATH_LEN )
				{
					pfnUnicodeStrcpy( (S8*) fullfilename_temp, (S8*) add_file_path );
					pfnUnicodeStrcat( (S8*) fullfilename_temp, (S8*) ugang );
					pfnUnicodeStrcat( (S8*) fullfilename_temp, (S8*) filename );
					#if 0
					if( is_shortname )
						MARK_SNAME( fullfilename_temp );
					else
						MARK_LNAME( fullfilename_temp );
					#endif
					if( g_audply.n_total_items >= MAX_PLAY_LIST_NUM )
					{
						add_file_result = 3;
						break;
					}
					else if( (add_file_result = mmi_audply_playlist_append_one( (S8*)fullfilename_temp )) != 0 )
						break;
					else
						add_file_result = 0;
				}
			}
		}
		}	
		if( (++i) == 10 )
		{
		
			StopTimer(AUDPLY_PROCESS_ADD_FILE_TIMER);
			StartTimer( AUDPLY_PROCESS_ADD_FILE_TIMER, 100, mmi_audply_process_add_files_in_folder );
			
			TBM_EXIT(0x2A25);
			return;
		}
	} while( MMI_FS_FindNext( add_file_cursor, &file_info, (UINT8*)filename, (FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH ) == FS_NO_ERROR );

	MMI_FS_FindClose(add_file_cursor);
       add_file_cursor = -1;
	mmi_audply_process_add_files_end();
    
      

	TBM_EXIT(0x2A25);
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_process_add_files_end
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_process_add_files_end( void )
{
	//if( add_file_cursor >= 0 )
	{
		if( add_file_result >= 0 )
		{
			// re-initialize pick index if necessary
			if( g_audply.pick_index < 0 )
			{
				if( mmi_audply_pick_playlist() >= 0 )
					mmi_audply_apply_picked_file();
			}
			mmi_audply_show_update_list_result( (U8) add_file_result );
		}
		else
			mmi_audply_show_update_list_result( 0 );

		if(add_file_cursor>=0)
		{
			MMI_FS_FindClose( add_file_cursor);
			add_file_result = -1;
			add_file_cursor = -1;
		}
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_entry_processing_add
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_entry_processing_add( void )
{
	EntryNewScreen( SCR_ID_AUDPLY_Add_FILES, mmi_audply_exit_processing_add, NULL, NULL );

	// clear End key
	ClearKeyHandler( KEY_END, KEY_EVENT_DOWN );
	ClearKeyHandler( KEY_END, KEY_LONG_PRESS );

	// Show searching screen
	ShowCategory66Screen( STR_ID_AUDPLY_OPTION_ADD, 0,
		0, 0, STR_GLOBAL_CANCEL, 0, 
		(U8*)GetString( STR_ID_AUDPLY_PROCESSING ), IMG_GLOBAL_PROGRESS, NULL );

	// Set RSK to abort searching
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

	add_file_result = -1;
	add_file_cursor = -1;
	StartTimer( AUDPLY_PROCESS_ADD_FILE_TIMER, 100, mmi_audply_process_add_files_in_folder );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_exit_processing_add
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_exit_processing_add( void )
{
	StopTimer( AUDPLY_PROCESS_ADD_FILE_TIMER );
	if( add_file_cursor >= 0 )
	{
		if( add_file_result >= 0 )
		{
			// re-initialize pick index if necessary
			if( g_audply.pick_index < 0 )
			{
				if( mmi_audply_pick_playlist() >= 0 )
					mmi_audply_apply_picked_file();
			}
		}
		MMI_FS_FindClose( add_file_cursor);
		add_file_result = -1;
		add_file_cursor = -1;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_add_file_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S8* fullPath
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_add_file_hdlr( S8* fullPath, int is_short )
{
	U8 result = 0; // 0: success, 1: disk full, 2: other access file error, 3: list full
	S16 path_len = 0;
	S8* end = fullPath;
	S8* last_backlash;
	UI_character_type drive, code;

	if( fullPath == NULL )
	{
		GoBackToHistory( SCR_ID_AUDPLY_PLAYLIST );
		return;
	}

	//check if it's in different storage than present list
	drive = (UI_character_type) fullPath[0];
	if( drive != PRESENT_LIST_ROOT[0] )
	{
		DisplayPopup( (PU8)GetString( STR_ID_AUDPLY_FILE_IN_DIFFERENT_STORAGE ), IMG_GLOBAL_WARNING, 0, 1000, (U8)WARNING_TONE );
		return;
	}
	
	for( last_backlash = NULL; (code = (U16)GET_WCHAR(end)) != 0; end+=2, path_len++ )
	{
		if( code == (UI_character_type)_T_MMI'/' )
			last_backlash = end;
	}
	if( last_backlash != NULL && end - last_backlash == 2 )
	{
		// folder
		path_len = pfnUnicodeStrlen(fullPath);
		add_file_path_len = path_len;
		pfnUnicodeStrcpy( (S8*) add_file_path, (S8*) fullPath );
		
		
		mmi_audply_entry_processing_add();
		return;
	}
	else
	{
		// file
		if( g_audply.n_total_items >= MAX_PLAY_LIST_NUM )
			result = 3; // list full
		else
		{
			 
			#if 0
			if( is_short )
				MARK_SNAME2( fullPath );  
			else
				MARK_LNAME2( fullPath );  
			#endif
			

			// write the path to play list file
			result = mmi_audply_playlist_append_one( fullPath );

			// we should not modify the path string bcoz file manager will use it so recover drive char here
			fullPath[0] = (S8) drive;
		}
	}

	// re-initialize pick index if necessary
	if( g_audply.pick_index < 0 )
	{
		if( mmi_audply_pick_playlist() >= 0 )
			mmi_audply_apply_picked_file();
	}
	mmi_audply_show_update_list_result( result );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_entry_settings
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
//chenhe 090803 for bug 13435
void mmi_audply_restoreSetting(void)
{
#if defined(__PROJECT_GALLITE_C01__)
	g_audply.list_auto_gen=1;
#else
	g_audply.list_auto_gen=0;
#endif
	g_audply.preferred_list=0;
	g_audply.skin=0;
	g_audply.repeat_mode=0;
	g_audply.random_mode=0;
	g_audply.background_play=1;
#ifdef __MMI_SOUND_EFFECT__
	g_settings_audio_equalizer.index = 0;
#endif
// Beginning--modified by kecx for fixing bug 14995 on 20091030
   mdi_audio_reset_background_handler(0);
   mdi_audio_reset_background_handler(1);
   mdi_audio_reset_background_handler(2);
// end--modified by kecx for fixing bug 14995 on 20091030
#ifdef __MMI_SOUND_EFFECT__
	mmi_settings_write_audio_equalizer_to_nvram(&g_settings_audio_equalizer);
#endif
	mmi_audply_write_settings();
}

void mmi_audply_entry_settings()
{
	U8* guiBuffer;
	U8* inputBuffer;
#ifdef __MMI_SOUND_EFFECT__
	U16 i;
#endif
	U16 inputBufferSize;
	U16 n_items = 0;
	S32 item_offset = 0;
#if 0	
	UI_image_ID_type item_icons[] =
	{
		IMG_ID_AUDPLY_SETTINGS_PREFER_LIST,
		IMG_ID_AUDPLY_SETTINGS_LIST_AUTO_GEN,
		IMG_ID_AUDPLY_SETTINGS_SKIN,
		IMG_ID_AUDPLY_SETTINGS_REPEAT,
		IMG_ID_AUDPLY_SETTINGS_SHUFFLE,
		IMG_ID_AUDPLY_SETTINGS_BACKGROUND_PLAY,
#ifdef __MMI_SOUND_EFFECT__
		IMG_ID_SETTING_AUDIO_EQUALIZER,
#endif
	};
#endif
UI_image_ID_type item_icons[] =
	{
/*liwh Del Start For 000000 Ver: TBM780  on 2007-8-2 17:17 */
#ifdef 	COOLSAND_MULTIMEDIA_USE_FLASHFS
		IMG_ID_AUDPLY_SETTINGS_PREFER_LIST,
		#endif
/*liwh Del End  For 000000 Ver: TBM780  on 2007-8-2 17:17 */
		IMG_ID_AUDPLY_SETTINGS_LIST_AUTO_GEN,
		IMG_ID_AUDPLY_SETTINGS_REPEAT,
		IMG_ID_AUDPLY_SETTINGS_SHUFFLE,
		#if 0
		IMG_ID_AUDPLY_SETTINGS_SKIN,
		#endif

//		IMG_ID_AUDPLY_SETTINGS_BACKGROUND_PLAY,
#ifdef __MMI_SOUND_EFFECT__
		IMG_ID_SETTING_AUDIO_EQUALIZER,
#endif

#if defined(__MMI_NOKIA_STYLE_N800__)||defined(CAM_MD_SUPPORT)
               IMG_ID_AUDPLY_DETAIL_GENRE,
#endif
	#ifdef __MMI_A2DP_SUPPORT__
	    IMG_PROFILES_BT,
	    IMG_PROFILES_HEADSET,
	#endif /* __MMI_A2DP_SUPPORT__ */


	};
#ifdef __MMI_SOUND_EFFECT__
	UI_string_ID_type audio_equalizer_list_strings[] = 
	{
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_1,  /*无变化（db）	EQ=0*/
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_2, /*重低音（db）	EQ=1*/
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_3,  /*舞曲（db）		EQ=2*/
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_4,  /*古典乐（db）	EQ=3*/
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_5,  /*高音（db）  	EQ=4*/
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_6,   /*宴会（db）  	EQ=5*/
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_7,  /*流行音乐（db）	EQ=6*/
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_8,    /*摇滚乐（db）	EQ=7*/
	};
#endif
#if 0//zhou// defined(__MMI_BT_COOLSAND_SUPPORT__)  && defined(__MMI_A2DP_SUPPORT__)
    mmi_bt_dev_struct *selected_bth_dev;
#endif

	EntryNewScreen( SCR_ID_AUDPLY_SETTINGS, mmi_audply_exit_settings, NULL, NULL );
	
	InitializeCategory57Screen();
   
	guiBuffer = GetCurrGuiBuffer( SCR_ID_AUDPLY_SETTINGS );
	inputBuffer = GetCurrNInputBuffer( SCR_ID_AUDPLY_SETTINGS, &inputBufferSize );

	RegisterHighlightHandler( mmi_audply_settings_highlight_hdlr );
	
	if( guiBuffer == NULL )
	{
		selected_list_auto_gen = g_audply.list_auto_gen;
		selected_preferred_list = g_audply.preferred_list;
		selected_skin = g_audply.skin;
		selected_repeat_mode = g_audply.repeat_mode;
		selected_random_mode = g_audply.random_mode;
		selected_background_play = g_audply.background_play;
#ifdef __MMI_SOUND_EFFECT__
		selected_equalizer_index = (S32) g_settings_audio_equalizer.index;
#endif
    #if 0//zhou//#ifdef __MMI_A2DP_SUPPORT__
        /* Check SAP state */
        if (mmi_bt_is_simap_server_activate())
        {
            selected_bt_a2dp_output = -1;     
        }
        else if (bt_settings.bt_a2dp_output)
        {
            selected_bt_a2dp_output = 1;
        }
        else
        {
            selected_bt_a2dp_output = 0;
        }
        
        selected_bt_a2dp_headset_temp = NULL;
        selected_bth_dev = mmi_audply_get_bt_headset();
        if (selected_bth_dev == NULL)
        {
            UCS2Strcpy((PS8)setting_bt_a2dp_headset_name, (const PS8)GetString(STR_GLOBAL_NONE));
        }
        else
        {
            med_util_utf8_to_ucs2((kal_uint8*) setting_bt_a2dp_headset_name, 40,
                (kal_uint8*) selected_bth_dev->name, 32);
        }
    #endif /*  __MMI_A2DP_SUPPORT__ */

	}
	#if 0//zhou//#ifdef __MMI_A2DP_SUPPORT__
    else if (selected_bt_a2dp_headset_temp)
    {
        /* back from history */
        med_util_utf8_to_ucs2((kal_uint8*) setting_bt_a2dp_headset_name, 40,
            (kal_uint8*) selected_bt_a2dp_headset_temp->name, 32);
    }
#endif /* __MMI_BT_COOLSAND_SUPPORT__ && __MMI_A2DP_SUPPORT__ */

	
	// preferred list
/*liwh Del Start For 000000 Ver: TBM780  on 2007-8-2 17:19 */
#ifdef 	COOLSAND_MULTIMEDIA_USE_FLASHFS
	if( phoneDrvLetter > 0 || cardDrvLetter > 0 
                /*+ zhouqin modify for dual t-flash card 20110503*/
                #if defined(DUAL_TFLASH_SUPPORT)
                    ||seccardDrvLetter > 0 
                #endif
                /*- zhouqin modify for dual t-flash card 20110503*/
        )
	{
		g_mmi_audply_settings_preferred_list[0] = (UI_string_type) GetString( STR_ID_AUDPLY_LIST_FROM_PHONE );
		g_mmi_audply_settings_preferred_list[1] = (UI_string_type) GetString( STR_ID_AUDPLY_LIST_FROM_CARD );
        /*+ zhouqin modify for dual t-flash card 20110503*/
        #if defined(DUAL_TFLASH_SUPPORT)
        g_mmi_audply_settings_preferred_list[2] = (UI_string_type) GetString( STR_ID_AUDPLY_LIST_FROM_SEC_CARD );
        #endif
        /*- zhouqin modify for dual t-flash card 20110503*/
		SetInlineItemActivation( &wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
		SetInlineItemSelect( &wgui_inline_items[n_items], NO_OF_LIST, (U8**)g_mmi_audply_settings_preferred_list, &selected_preferred_list );
		item_offset = 1;
		n_items++;
	}
#endif
/*liwh Del End  For 000000 Ver: TBM780  on 2007-8-2 17:19 */
	
	// list auto gen
	g_mmi_audply_settings_list_auto_gen[0] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_OFF );
	g_mmi_audply_settings_list_auto_gen[1] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_ON );
	SetInlineItemActivation( &wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemSelect( &wgui_inline_items[n_items], 2, (U8**)g_mmi_audply_settings_list_auto_gen, &selected_list_auto_gen );
	//item_offset = 1;
	n_items++;
	

	// repeat
	g_mmi_audply_settings_repeat[0] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_OFF );
	g_mmi_audply_settings_repeat[1] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_ONE );
	g_mmi_audply_settings_repeat[2] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_ALL );
	SetInlineItemActivation( &wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemSelect( &wgui_inline_items[n_items], 3, (U8**)g_mmi_audply_settings_repeat, &selected_repeat_mode );
	n_items++;
	
	// shuffle
	g_mmi_audply_settings_shuffle[0] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_OFF );
	g_mmi_audply_settings_shuffle[1] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_ON );
	SetInlineItemActivation( &wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemSelect( &wgui_inline_items[n_items], 2, (U8**)g_mmi_audply_settings_shuffle, &selected_random_mode );
	n_items++;
#if 0	
	// skin
	g_mmi_audply_settings_skin[0] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_SKIN1 );
#ifndef __MMI_SLIM_AUDIO_PLAYER__
	g_mmi_audply_settings_skin[1] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_SKIN2 );
	g_mmi_audply_settings_skin[2] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_SKIN3 );
#endif
	SetInlineItemActivation( &wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemSelect( &wgui_inline_items[n_items], NO_OF_SKIN, (U8**)g_mmi_audply_settings_skin, &selected_skin );
	n_items++;
#endif

#if 0
	// background play
	g_mmi_audply_settings_background_play[0] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_OFF );
	g_mmi_audply_settings_background_play[1] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_ON );
	SetInlineItemActivation( &wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemSelect( &wgui_inline_items[n_items], 2, (U8**)g_mmi_audply_settings_background_play, &selected_background_play );
	n_items++;
#endif	
#ifdef __MMI_SOUND_EFFECT__
	// equalizer settings
	for( i = 0; i < g_settings_audio_equalizer.count; i++ )
		g_mmi_audply_settings_equalizer[i] = (UI_string_type) GetString( audio_equalizer_list_strings[i] );
	SetInlineItemActivation( &wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemSelect( &wgui_inline_items[n_items], g_settings_audio_equalizer.count, (U8**)g_mmi_audply_settings_equalizer, &selected_equalizer_index );
	n_items++;
#endif
#if defined(__MMI_NOKIA_STYLE_N800__)||defined(CAM_MD_SUPPORT)
	// rotate
	g_mmi_audply_settings_rotated[0] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_OFF );
	g_mmi_audply_settings_rotated[1] = (UI_string_type) GetString( STR_ID_AUDPLY_SETTINGS_ON );
	SetInlineItemActivation( &wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemSelect( &wgui_inline_items[n_items], 2, (U8**)g_mmi_audply_settings_rotated, &g_mmi_audply_rotated_enable );
	n_items++;
#endif
#if 0//zhou//#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)
    /* BT A2DP Output */
    g_mmi_audply_settings_bt_a2dp_output[0] = GetString(STR_ID_AUDPLY_SETTINGS_OFF);
    g_mmi_audply_settings_bt_a2dp_output[1] = GetString(STR_ID_AUDPLY_SETTINGS_ON);
    SetInlineItemActivation(&wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemUserDefinedSelect(
        &wgui_inline_items[n_items],
        mmi_audply_bts_inline_default_callback,
        mmi_audply_bts_inline_change_select_callback,
        mmi_audply_bts_inline_change_select_callback
        );
    /*SetInlineItemSelect(&wgui_inline_items[n_items], 2, (U8 **) g_mmi_audply_settings_bt_a2dp_output,
        &selected_bt_a2dp_output);
    RegisterInlineSelectHighlightHandler(&wgui_inline_items[n_items], mmi_audply_settings_inlineitem_highlight_hdlr);*/
    item_icons[n_items] = IMG_PROFILES_BT;
    n_items++;

    /* BT A2DP Headset */
    SetInlineItemActivation(&wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemDisplayOnly(&wgui_inline_items[n_items], (U8*) setting_bt_a2dp_headset_name);
    item_icons[n_items] = IMG_PROFILES_HEADSET;
    n_items++;
#endif /* __MMI_BT_COOLSAND_SUPPORT__ && __MMI_A2DP_SUPPORT__ */

	// show category screen
	ShowCategory57Screen( STR_ID_AUDPLY_OPTION_SETTINGS, MAIN_MENU_TITLE_MULTIMEDIA_ICON,	// title text and icon
		STR_GLOBAL_OK, IMG_GLOBAL_OK,											// LSK text and icon
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,										// RSK text and icon
		n_items,
		item_icons + 1 - item_offset, wgui_inline_items,
		0, guiBuffer);

	//SetCategory57LeftSoftkeyFunction( mmi_audply_settings_LSK );
	
	DisableCategory57ScreenDone();
	
//	SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );
	
	// register volume inc, dec handlers
	SetKeyHandler( mmi_audply_press_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_press_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_exit_settings
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_exit_settings()
{
	history_t	h;
	U16 inputBufferSize;

	CloseCategory57Screen();
	h.scrnID = SCR_ID_AUDPLY_SETTINGS;
	h.entryFuncPtr = mmi_audply_entry_settings;
	GetCategoryHistory( h.guiBuffer );
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data( (U8*) h.inputBuffer );
	AddNHistory( h, inputBufferSize );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_settings_highlight_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S32 index
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_settings_highlight_hdlr( S32 index )
{
    U16 i = 0 ;
#if 0//zhou//#ifdef __MMI_A2DP_SUPPORT__
    U16 bt_headset_index;
#endif
    UI_string_ID_type item_texts[AUDPLY_SETTING_ITEM_COUNT] ;
//    U8  total_drv_num;
 //   UI_string_type drv_inline_list_p = NULL;
/*liwh Del Start For 000000 Ver: TBM780  on 2007-8-2 17:18 */
		#ifdef COOLSAND_MULTIMEDIA_USE_FLASHFS
        item_texts[i++] = STR_ID_AUDPLY_SETTINGS_PREFER_LIST;
		#endif
/*liwh Del End  For 000000 Ver: TBM780  on 2007-8-2 17:18 */
    item_texts[i++] = STR_ID_AUDPLY_SETTINGS_LIST_AUTO_GEN;
    item_texts[i++] = STR_ID_AUDPLY_SETTINGS_REPEAT;
    item_texts[i++] = STR_ID_AUDPLY_SETTINGS_SHUFFLE;
//		STR_ID_AUDPLY_SETTINGS_SKIN,
//		STR_ID_AUDPLY_SETTINGS_BACKGROUND_PLAY,
#ifdef __MMI_SOUND_EFFECT__
    item_texts[i++] = STR_ID_SETTING_AUDIO_EQUALIZER;
#endif

#if defined(__MMI_NOKIA_STYLE_N800__)||defined(CAM_MD_SUPPORT)
    item_texts[i++] = STR_ID_AUDPLY_SETTINGS_ROTATED_CONTROL;
#endif

#if 0//zhou//#if (defined(__MMI_BT_COOLSAND_SUPPORT__)  ||defined(__MMI_BT_RDA_SUPPORT__))&& defined(__MMI_A2DP_SUPPORT__)
    item_texts[i++] = STR_ID_AUDPLY_SETTINGS_BT_A2DP_OUTPUT;
    bt_headset_index = i;
    item_texts[i++] = STR_ID_AUDPLY_SETTINGS_BT_A2DP_HEADSET;
#endif /* __MMI_A2DP_SUPPORT__ */
    ChangeTitleString((PU8) GetString(item_texts[index]));
	draw_title();
    ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_UP);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);
    
 
#if 0//zhou//#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)
    if (index == bt_headset_index) /* select headset */
    {
        ChangeLeftSoftkey(STR_GLOBAL_SELECT, 0);
        SetCategory57LeftSoftkeyFunction(mmi_audply_select_bt_a2dp_headset);
        if (selected_bt_a2dp_headset_temp)
        {
            ChangeRightSoftkey(STR_GLOBAL_DONE, 0);
            SetCategory57RightSoftkeyFunctions(mmi_audply_settings_LSK,
                mmi_audply_settings_LSK);
        }
        else
        {
            ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
            SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
        }
    }
    else if (index == (bt_headset_index - 1))
    {
        SetKeyHandler(mmi_audply_sap_block_a2dp, KEY_LEFT_ARROW, KEY_EVENT_UP);
        SetKeyHandler(mmi_audply_sap_block_a2dp, KEY_RIGHT_ARROW, KEY_EVENT_UP);
        ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
        ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
        SetCategory57LeftSoftkeyFunction(mmi_audply_settings_LSK);
        SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
    }
    else
#endif /* __MMI_A2DP_SUPPORT__ */
    {
        ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
        ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
        SetCategory57LeftSoftkeyFunction(mmi_audply_settings_LSK);
        SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
    }
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_settings_LSK
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_settings_LSK()
{

	// if we turn list auto gen ON, we need to regen the list
	BOOL need_regen_list = (selected_list_auto_gen && (g_audply.list_auto_gen != selected_list_auto_gen));
	BOOL need_reload_list;
	BOOL prefered_list_changed = (selected_preferred_list != g_audply.preferred_list);
	BOOL repeat_mode_changed = (selected_repeat_mode != g_audply.repeat_mode);
	BOOL random_mode_changed = (selected_random_mode != g_audply.random_mode);
	BOOL skin_changed = (g_audply.skin != selected_skin);
	U8 old_present_list = g_audply.present_list;
	TBM_ENTRY(0x2A29);

#if 0//zhou//#if (defined(__MMI_BT_COOLSAND_SUPPORT__) ||defined(__MMI_BT_RDA_SUPPORT__)) && defined(__MMI_A2DP_SUPPORT__)
    BOOL bt_a2dp_output = (selected_bt_a2dp_output == 1);
    BOOL need_power_on_bt = FALSE;
    BOOL bt_a2dp_change;
#endif
#if 0//zhou//#if defined(__MMI_A2DP_SUPPORT__) && defined(__MMI_AUDIO_TIME_STRETCH__)
    //U16 prev_speed = g_audply.speed;
#endif
#if 0//zhou//#if defined(__MMI_BT_COOLSAND_SUPPORT__)  && defined(__MMI_A2DP_SUPPORT__)
    /* check prohibted conditions for bt settings */
    mmi_trace(g_sw_BT, "mmi_audply_settings_LSK bt_a2dp_output=%d",bt_a2dp_output);

    if (bt_a2dp_output && selected_bt_a2dp_headset_temp == NULL
        && !bt_settings.bt_a2dp_headset_valid)
    {
        DisplayPopup(
            (PU8) GetString(STR_ID_AUDPLY_SETTINGS_ERR_MUST_SELECT_HEADSET),
            IMG_GLOBAL_WARNING,
            0,
            1000,
            (U8) WARNING_TONE);
        return;
    }
    else if (bt_a2dp_output)
    {
        /* when BT A2DP Output is ON and BT is not power on, 
         * check if it's not in flight mode then we power on BT 
         */
        if (mmi_bootup_get_active_flight_mode())
        {
            DisplayPopup(
                (PU8) GetString(STR_ID_AUDPLY_SETTINGS_ERR_CANNOT_USE_BLUETOOTH_IN_FLIGHT_MODE),
                IMG_GLOBAL_WARNING,
                0,
                1000,
                (U8) WARNING_TONE);
            return;
        }
        else if (!mmi_bt_is_power_on(FALSE))
        {
            need_power_on_bt = TRUE;
        }
    }
#endif /* __MMI_A2DP_SUPPORT__ */
	if(prefered_list_changed && (selected_preferred_list == LIST_FROM_CARD) && (MMI_CARD_DRV <= 0))
	{
		DisplayPopup( (PU8)GetString(/*STR_ID_AUDPLY_NO_CARD*/ STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD ), IMG_GLOBAL_WARNING, 0, 1000, (U8)WARNING_TONE );	
		return;
	}
	/*+ zhouqin modify for dual t-flash card 20110503*/
    #if defined(DUAL_TFLASH_SUPPORT)
    else if(prefered_list_changed && (selected_preferred_list == LIST_FROM_SEC_CARD) && (MMI_SEC_CARD_DRV <= 0))
	{
		DisplayPopup( (PU8)GetString(/*STR_ID_AUDPLY_NO_CARD*/ STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD ), IMG_GLOBAL_WARNING, 0, 1000, (U8)WARNING_TONE );	
		return;
	} 
    #endif
    /*- zhouqin modify for dual t-flash card 20110503*/
	// apply it
	g_audply.list_auto_gen = (U8) selected_list_auto_gen;
	g_audply.preferred_list = (U8) selected_preferred_list;
	g_audply.skin = (U8) selected_skin;
	g_audply.repeat_mode = (U8) selected_repeat_mode;
	g_audply.random_mode = (U8) selected_random_mode;
	g_audply.background_play = (U8) selected_background_play;
#ifdef __MMI_SOUND_EFFECT__
	mmi_settings_select_audio_equalizer( (S16) selected_equalizer_index );
#endif

#if defined(__MMI_NOKIA_STYLE_N800__)||defined(CAM_MD_SUPPORT)
        g_audply.rotated_enable = (U8) g_mmi_audply_rotated_enable;
#endif
#if defined(__MMI_NOKIA_STYLE_N800__)||defined(CAM_MD_SUPPORT)
        g_audply.rotated_enable = (U8) g_mmi_audply_rotated_enable;
#endif
	// save to nvram
	mmi_audply_write_settings();
	
    /* update present list */
    g_audply.present_list = mmi_audply_get_preferred_or_available_list();
    need_reload_list = (g_audply.present_list != old_present_list);

    /* if the present list is changed and list auto gen is ON, we need to regen the list */
    if (g_audply.list_auto_gen && need_reload_list)
    {
        need_regen_list = TRUE;
    }
    if (need_regen_list)
    {
        need_reload_list = TRUE;
    }
		
	// regen or reload playlist
	mmi_audply_regen_and_reload_playlist( need_regen_list, need_reload_list );
	if( need_reload_list )
	// clear play list gui buffer
		HistoryReplace( SCR_ID_AUDPLY_PLAYLIST, SCR_ID_AUDPLY_PLAYLIST, mmi_audply_entry_list );
		
	if( !need_reload_list && (repeat_mode_changed || random_mode_changed) )
	{
		mmi_audply_start_playlist_at( g_audply.pick_index );
		mmi_audply_pick_playlist_at( g_audply.pick_index );
	}
	
	// go back
 
#if 0//zhou//#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)
    if (need_power_on_bt)
    {
        mmi_bt_entry_power_switch();
        wait_fot_bt_pwron = TRUE;
    }
    else
    {
        bt_a2dp_change = mmi_audply_bluetooth_settings_apply_change(bt_a2dp_output, skin_changed);
	#ifdef __MMI_AUDIO_TIME_STRETCH__
        /* check if need to reopen av bt */
        if (!bt_a2dp_change && prev_speed == MDI_AUDIO_SPEED_NORMAL && 
            prev_speed != g_audply.speed && mmi_audply_is_output_to_bt() && 
            av_bt_is_mp3_cfg())
        {
            mmi_audply_sync_bt_output();
        }
    #endif /* __MMI_AUDIO_TIME_STRETCH__ */
    }
#else
	if( skin_changed )
		GoBackToHistory( SCR_ID_AUDPLY_MAIN );
	else
		GoBackHistory();
#endif     

	TBM_EXIT(0x2A29);
}

/*****************************************************************************
* FUNCTION
*     get_audfile_info
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
kal_bool get_audfile_info( kal_wchar* filename, aud_info_struct* aud_info_p )
{
  BOOL flag = FALSE;
  INT32 size;
  U32 file_size;
  S8 buffer[32];
  #if 0
  S8 filesize[10] = {0};
  S8 ufilesize[10] = {0};
  INT size1;
  INT32 size2 = 0;
  S8 unit[4] = "M";
  S8 uniunit[10] = {0};
  
  UnicodeToAnsii((S8*)uniunit, (S8*)unit);
  #endif
  if ((size = MMI_FS_GetFileSizeByName((const UINT8 *)filename)) >=0)
  	flag = TRUE;
  #if 0
  size1 = size/(1024*1024);
		size2 = size%(1024*1024);
		size2 = size2*10/(1024*1024);
              //size2 >>= 16;
		   
		filesize[0] = size1 + 48;
		filesize[1] = '.';
		filesize[2] = size2 + 48;
		
		AnsiiToUnicodeString((S8*)ufilesize, (S8*) filesize);

		//mmi_trace(1,"in function get_audfile_info:the size is %s",filesize);
		pfnUnicodeStrcpy((S8*)aud_info_p->size,ufilesize);
		pfnUnicodeStrcat((S8*)aud_info_p->size,uniunit);
  #endif	

  if ( size > 1024*1024 )
  {
	  file_size = size/1024;
	  sprintf(buffer, "%d.%dM", (U16)(file_size/1024), (U16)((file_size%1024)/103));
  }
  else if ( size > 1024 )
  {
	  sprintf(buffer, "%d.%dK", (U16)(size/1024), (U16)((size%1024)/103));
  }
  else /* less than 1024 */
  {
	  sprintf(buffer, "%dB", (U16)(size));
  }
  AnsiiToUnicodeString((S8*)aud_info_p->size, (PS8)buffer);
  pfnUnicodeStrcat ((S8*)aud_info_p->size, (PS8)L"\n");  


  #if 0
  fd = MMI_FS_Open(filename, FS_READ_ONLY);

 
	
  if(fd >= 0)
  	{
	    if(MMI_FS_GetFileSize(fd, &size) == FS_NO_ERROR);
	    {
		
		size1 = size/(1024*1024);
		size2 = size%(1024*1024);
		size2 = size2*10/(1024*1024);
              //size2 >>= 16;
		   
		filesize[0] = size1 + 48;
		filesize[1] = '.';
		filesize[2] = size2 + 48;
		
		AnsiiToUnicodeString((S8*)ufilesize, (S8*) filesize);

		mmi_trace(1,"in function get_audfile_info:the size is %s",filesize);
		pfnUnicodeStrcpy(aud_info_p->size,ufilesize);
		pfnUnicodeStrcat(aud_info_p->size,uniunit);
		
		//strcpy(aud_info_p->size, filesize);
		flag = TRUE;
		
	    }	  
		MMI_FS_Close(fd);
       }
  #endif

  return flag;
  
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_entry_detail
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_entry_detail()
{
	S8 *fields[MAX_DAF_VIEW_FIELDS];   
	U16 item_icons[MAX_DAF_VIEW_FIELDS];
	U8* guiBuffer;
	U8 item_count = 0;
	UI_string_type buffer = fullfilename_temp;
	FS_HANDLE handle;
	BOOL ok;
#if defined( __DRM_SUPPORT__ ) && defined( MMI_ON_HARDWARE_P )
	BOOL is_permitted = FALSE;
#endif
	mmi_trace(1,"chenhe,mmi_audply_entry_detail");
	EntryNewScreen(SCR_ID_AUDPLY_DETAIL, mmi_audply_exit_detail, NULL, NULL );

	guiBuffer = GetCurrGuiBuffer( SCR_ID_AUDPLY_DETAIL );
	
	mmi_audply_playlist_get_fullfilename( buffer, g_mmi_audply_list_highlighted_item );

#if defined( __DRM_SUPPORT__ ) && defined( MMI_ON_HARDWARE_P )
	if( (handle = DRM_open_file( (PU16) buffer, FS_READ_ONLY, 0 )) >= FS_NO_ERROR )
	{
		if( DRM_validate_permission( handle, DRM_PERMISSION_PLAY ) )
			is_permitted = TRUE;
		DRM_close_file( handle );
	}
#else
	if( (handle = MMI_FS_Open( (UINT8*) buffer, FS_READ_ONLY )) >= FS_NO_ERROR )
		MMI_FS_Close( handle );
#endif
	else if( handle < -1 ) // file open error but not DRM related problem
	{
		DisplayPopup( (PU8)GetString( STR_ID_AUDPLY_OPEN_FILE_ERROR ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		DeleteUptoScrID( SCR_ID_AUDPLY_PLAYLIST );
		return;
	}
#if defined( __DRM_SUPPORT__ ) && defined( MMI_ON_HARDWARE_P )
	if( !is_permitted )
	{
		DisplayPopup( (PU8)GetString( STR_GLOBAL_DRM_PROHIBITED ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		DeleteUptoScrID( SCR_ID_AUDPLY_PLAYLIST );
		return;
	}
#endif
	if( (daf_info_p = (aud_info_struct*) OslMalloc( sizeof(aud_info_struct) )) == NULL )
		ok = FALSE;
#ifdef __RICH_AUDIO_PROFILE__	
#ifndef MMI_ON_WIN32
	else if( get_aud_info( buffer, daf_info_p ) )
		ok = TRUE;
#endif
	else		
		ok = FALSE;
#else
	else
		ok = TRUE;
#endif


	mmi_trace(1,"chenhe,mmi_audply_entry_detail,ok is %d",ok);

	if( !ok )
	{
		DisplayPopup( (PU8)GetString( STR_ID_AUDPLY_ERROR ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		DeleteUptoScrID( SCR_ID_AUDPLY_PLAYLIST );
	}
	else
	{
#ifdef __MMI_AUDIO_PLAYER_DETAIL_SHOW_MORE_INFO__
		// filename
		mmi_audply_playlist_get_filename( (UI_string_type) daf_info_p->filename, g_mmi_audply_list_highlighted_item ); // get file name without ext
		fields[item_count] = (S8*)daf_info_p->filename;
		g_mmi_audply_detail_fields[item_count] = STR_GLOBAL_FILENAME;
		item_icons[item_count++] = IMG_ID_AUDPLY_DETAIL_FILENAME;

		// channel
		if( daf_info_p->channel_num != 0 )
		{
			if( daf_info_p->channel_num == 1 )
				fields[item_count] = GetString( STR_ID_AUDPLY_DETAIL_MONO );
			else
				fields[item_count] = GetString( STR_ID_AUDPLY_DETAIL_STEREO );
			g_mmi_audply_detail_fields[item_count] = STR_ID_AUDPLY_DETAIL_CHANNEL;
			item_icons[item_count++] = IMG_ID_AUDPLY_DETAIL_CHANNEL;
		}
#endif

#ifdef __RICH_AUDIO_PROFILE__
		// quality
		if( daf_info_p->quality[0] != 0 )
		{
			fields[item_count] = (S8*)daf_info_p->quality;
			g_mmi_audply_detail_fields[item_count] = STR_ID_AUDPLY_DETAIL_QUALITY;
			item_icons[item_count++] = IMG_ID_AUDPLY_DETAIL_QUALITY;
		}

		// size
		fields[item_count] = (S8*)daf_info_p->size;
		g_mmi_audply_detail_fields[item_count] = STR_ID_AUDPLY_DETAIL_SIZE;
		item_icons[item_count++] = IMG_ID_AUDPLY_DETAIL_FILE;
		
		if( daf_info_p->duration[0] != 0 )
		{
			fields[item_count] = (S8*)daf_info_p->duration;
			g_mmi_audply_detail_fields[item_count] = STR_ID_AUDPLY_DETAIL_TIME;
			item_icons[item_count++] = IMG_ID_AUDPLY_DETAIL_TIME;
		}

		if( daf_info_p->title[0] != 0 )
		{
			fields[item_count] = (S8*)daf_info_p->title;
			g_mmi_audply_detail_fields[item_count] = STR_ID_AUDPLY_DETAIL_TITLE;
			item_icons[item_count++] = IMG_ID_AUDPLY_DETAIL_TITLE;
		}

		if( daf_info_p->artist[0] != 0 )
		{
			fields[item_count] = (S8*)daf_info_p->artist;
			g_mmi_audply_detail_fields[item_count] = STR_ID_AUDPLY_DETAIL_ARTIST;
			item_icons[item_count++] = IMG_ID_AUDPLY_DETAIL_ARTIST;
		}
		
		if( daf_info_p->album[0] !=0 )
		{
			fields[item_count] = (S8*)daf_info_p->album;
			g_mmi_audply_detail_fields[item_count] = STR_ID_AUDPLY_DETAIL_ALBUM;
			item_icons[item_count++] = IMG_ID_AUDPLY_DETAIL_ALBUM;
		}
		
		if( daf_info_p->genre[0] != 0 )
		{
			fields[item_count] = (S8*)daf_info_p->genre;
			g_mmi_audply_detail_fields[item_count] = STR_ID_AUDPLY_DETAIL_GENRE;
			item_icons[item_count++] = IMG_ID_AUDPLY_DETAIL_GENRE;
		}
		
		if( daf_info_p->year[0] != 0 )
		{
			fields[item_count] = (S8*)daf_info_p->year;
			g_mmi_audply_detail_fields[item_count] = STR_ID_AUDPLY_DETAIL_YEAR;
			item_icons[item_count++] = IMG_ID_AUDPLY_DETAIL_YEAR;
		}
#endif

		RegisterHighlightHandler( mmi_audply_detail_highlight_hdlr );

		ShowCategory84Screen( STR_ID_AUDPLY_OPTION_DETAIL, MAIN_MENU_TITLE_MULTIMEDIA_ICON,	// title text and icon
			0, 0,										// LSK text and icon
			STR_GLOBAL_BACK, IMG_GLOBAL_BACK,									// RSK text and icon
			item_count, (U8**)fields, item_icons,
			0, 0, guiBuffer );

//		SetLeftSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
		SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

		SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
		
		// register volume inc, dec handlers
		SetKeyHandler( mmi_audply_press_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
		SetKeyHandler( mmi_audply_press_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_exit_detail
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_exit_detail()
{

	history_t currHistory;
	if( daf_info_p != NULL )
	{
		OslMfree( daf_info_p );
		daf_info_p = NULL;
	}
	currHistory.scrnID = SCR_ID_AUDPLY_DETAIL;
	currHistory.entryFuncPtr = mmi_audply_entry_detail;
	pfnUnicodeStrcpy( (S8*)currHistory.inputBuffer, (S8*)L"" );
	GetCategoryHistory( currHistory.guiBuffer );
	AddHistory( currHistory );

}

/*****************************************************************************
* FUNCTION
*     mmi_audply_detail_highlight_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S32 index
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_detail_highlight_hdlr( S32 index )
{
	ChangeTitleString( (PU8)GetString( g_mmi_audply_detail_fields[index] ) );
	draw_title();
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_entry_remove_confirm
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_entry_remove_confirm()
{
	DisplayConfirm( STR_GLOBAL_YES, IMG_GLOBAL_YES,
		STR_GLOBAL_NO,IMG_GLOBAL_NO, 
		get_string(STR_ID_AUDPLY_OPTION_REMOVE), IMG_GLOBAL_QUESTION, WARNING_TONE );

	SetLeftSoftkeyFunction( mmi_audply_remove_confirm_LSK, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_exit_remove_confirm
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_exit_remove_confirm()
{
	history_t currHistory;
	currHistory.scrnID = SCR_ID_AUDPLY_REMOVE_CONFIRM;
	currHistory.entryFuncPtr = mmi_audply_entry_remove_confirm;
	pfnUnicodeStrcpy( (S8*)currHistory.inputBuffer, (S8*)L"" );
	AddHistory( currHistory );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_remove_confirm_LSK
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_remove_confirm_LSK()
{
	TBM_ENTRY(0x2A27);
	mmi_trace(1,"mmi_audply_remove_confirm_LSK");
	if( g_audply.pick_index == g_mmi_audply_list_highlighted_item )
		mmi_audply_stop_playing();

	if( mmi_audply_playlist_remove_one( g_mmi_audply_list_highlighted_item ) )
		GoBackHistory();
	else
	{
		GoBackHistory();
		DisplayPopup( (PU8)GetString( STR_ID_AUDPLY_CANNOT_UPDATE_LIST_FS_FAIL ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
	}

	TBM_EXIT(0x2A27);
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_entry_remove_all_confirm
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_entry_remove_all_confirm()
{
	DisplayConfirm( STR_GLOBAL_YES, IMG_GLOBAL_YES,
		STR_GLOBAL_NO,IMG_GLOBAL_NO, 
		get_string(STR_ID_AUDPLY_OPTION_REMOVE_ALL), IMG_GLOBAL_QUESTION, WARNING_TONE );

	SetLeftSoftkeyFunction( mmi_audply_remove_all_confirm_LSK, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_exit_remove_all_confirm
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_exit_remove_all_confirm()
{
	history_t currHistory;
	currHistory.scrnID = SCR_ID_AUDPLY_REMOVE_ALL_CONFIRM;
	currHistory.entryFuncPtr = mmi_audply_entry_remove_all_confirm;
	pfnUnicodeStrcpy( (S8*)currHistory.inputBuffer, (S8*)L"" );
	AddHistory( currHistory );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_remove_all_confirm_LSK
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_remove_all_confirm_LSK()
{
	TBM_ENTRY(0x2A28);
	mmi_trace(1,"mmi_audply_remove_all_confirm_LSK is called");
	mmi_audply_stop_playing();
	if( mmi_audply_playlist_remove_all() )
		GoBackHistory();
	else
	{
		GoBackHistory();
		DisplayPopup( (PU8)GetString( STR_ID_AUDPLY_CANNOT_UPDATE_LIST_FS_FAIL ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
	}

	TBM_EXIT(0x2A28);
}

#endif /* __MMI_AUDIO_PLAYER__ */

/*****************************************************************************
* FUNCTION
*     mmi_audply_single_inc_volume
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_single_inc_volume( void )
{
	if( single_volume < MAX_VOL_LEVEL )
		mdi_audio_set_volume( VOL_TYPE_MEDIA, ++single_volume );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_press_dec_volume
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_single_dec_volume( void )
{
	if( single_volume > 0 )
		mdi_audio_set_volume( VOL_TYPE_MEDIA, --single_volume );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_single_play_timer
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
extern U32 global_var_uptime(BOOL bSet);

void mmi_audply_single_play_timer()
{

	kal_uint32 current_time;

	kal_get_time( &current_time );


	single_play_duration = current_time - single_play_time;     
	StartTimer( AUDPLY_SINGLE_PLAY_TIMER, 500, mmi_audply_single_play_timer );

}

/*****************************************************************************
* FUNCTION
*     mmi_audply_single_play
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     UI_string_type filefullname
*     void (*exit_callback_func)(void)
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_single_play( UI_string_type filefullname, void (*exit_callback_func)(void) )
{
	S32 result;
	single_play_exit_callback_func = exit_callback_func;
	result = mmi_audply_do_single_play_action( filefullname);

	if( result == MDI_AUDIO_SUCCESS )
		mmi_audply_entry_play_animation();
	else
	{
		if( single_play_exit_callback_func != NULL )
		{
			single_play_exit_callback_func();
			single_play_exit_callback_func = NULL;
		}
		DisplayPopup( (PU8)GetString( mmi_audply_err_str( result ) ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_entry_play_animation
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_entry_play_animation()
{
	U8* guiBuffer;
	
	EntryNewScreen( SCR_ID_AUDPLY_PLAY_ANIMATION, mmi_audply_exit_play_animation, NULL, NULL );
	
	in_single_play_screen = TRUE;
	guiBuffer = GetCurrGuiBuffer( SCR_ID_AUDPLY_PLAY_ANIMATION );

	if( single_play_state == STATE_IDLE )
		ShowCategory223Screen( STR_ID_AUDPLY_TITLE, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
			0, 0,
			STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			IMG_ID_AUDPLY_STOP_ANIMATION, 0,
			single_play_duration, guiBuffer );
	else
	{
		ShowCategory223Screen( STR_ID_AUDPLY_TITLE, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
			0, 0,
			STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			IMG_ID_AUDPLY_PLAY_ANIMATION, 1,
			single_play_duration, guiBuffer );
	}
	
	// register RSK handler
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
//niej modify used to cs621 and tc9751 start 20070929
	// register volume inc, dec handlers
#if defined(__NO_SIDE_VOLUME_KEY_56__)	// register volume inc, dec handlers
	SetKeyHandler( mmi_audply_single_inc_volume, KEY_6, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_single_dec_volume, KEY_5, KEY_EVENT_DOWN );
#elif defined(__NO_SIDE_VOLUME_KEY_46__)
	SetKeyHandler( mmi_audply_single_inc_volume, KEY_6, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_single_dec_volume, KEY_4, KEY_EVENT_DOWN );
#elif defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__)
	SetKeyHandler( mmi_audply_single_inc_volume, KEY_POUND, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_single_dec_volume, KEY_STAR, KEY_EVENT_DOWN );

#else
	// register volume inc, dec handlers
	SetKeyHandler( mmi_audply_single_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_audply_single_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
#endif

//niej modify end
	SetDelScrnIDCallbackHandler( SCR_ID_AUDPLY_PLAY_ANIMATION, (HistoryDelCBPtr)mmi_audply_delete_play_animation_history_hdlr );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_exit_play_animation
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_exit_play_animation()
{
	history_t currHistory;
	currHistory.scrnID = SCR_ID_AUDPLY_PLAY_ANIMATION;
	currHistory.entryFuncPtr = mmi_audply_entry_play_animation;
	pfnUnicodeStrcpy( (S8*)currHistory.inputBuffer, (S8*)L"" );
	AddHistory( currHistory );
	in_single_play_screen = FALSE;
	if( single_play_exit_callback_func != NULL )
	{
		single_play_exit_callback_func();
		single_play_exit_callback_func = NULL;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_delete_play_animation_history_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void* param_p
* RETURNS
*     U8
* GLOBALS AFFECTED
*     
*****************************************************************************/
U8 mmi_audply_delete_play_animation_history_hdlr( void* param_p )
{
	if( single_play_state != STATE_IDLE )
		mmi_audply_do_single_stop_action();

	return FALSE;
}

#if defined(__MMI_NOKIA_STYLE_N800__)||defined(CAM_MD_SUPPORT)
BOOL mmi_audply_is_rotate_enable(void)
{
	return g_mmi_audply_rotated_enable ;
}
#endif

#ifdef __MMI_A2DP_SUPPORT__
/*****************************************************************************
 * FUNCTION
 *  mmi_audply_bts_inline_default_callback
 * DESCRIPTION
 *  callback function for setting "BT A2DP output"
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static U8 *mmi_audply_bts_inline_default_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (selected_bt_a2dp_output == -1 || selected_bt_a2dp_output == 0)
    {
        return (PU8)GetString(STR_ID_AUDPLY_SETTINGS_OFF);
    }
    else if (selected_bt_a2dp_output == 1)
    {
        return (PU8)GetString(STR_ID_AUDPLY_SETTINGS_ON);
    }
    else
    {
        selected_bt_a2dp_output = 0;
        return (PU8)GetString(STR_ID_AUDPLY_SETTINGS_OFF);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_audply_bts_inline_change_select_callback
 * DESCRIPTION
 *  callback function for setting "BT A2DP output"
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static U8 *mmi_audply_bts_inline_change_select_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (selected_bt_a2dp_output == 0)
    {
        selected_bt_a2dp_output = 1;
        return (PU8)GetString(STR_ID_AUDPLY_SETTINGS_ON);
    }
    else if (selected_bt_a2dp_output == 1)
    {
        selected_bt_a2dp_output = 0;
        return (PU8)GetString(STR_ID_AUDPLY_SETTINGS_OFF);
    }
    else if (selected_bt_a2dp_output == -1)
    {
        return (PU8)GetString(STR_ID_AUDPLY_SETTINGS_OFF);
    }
    else
    {
        selected_bt_a2dp_output = 0;
        return (PU8)GetString(STR_ID_AUDPLY_SETTINGS_OFF);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_audply_sap_block_a2dp
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_audply_sap_block_a2dp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (mmi_bt_is_simap_server_activate())
    {
        DisplayPopup(
            (PU8)GetString(STR_BT_PROHIBIT_BY_SIMAP),
            IMG_GLOBAL_WARNING,
            1,
            1000,
            (U8)WARNING_TONE);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_audply_entry_bluetooth_settings
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_entry_bluetooth_settings(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U8 *inputBuffer;
    U16 n_items = 0;
    U16 inputBufferSize;
    mmi_bt_dev_struct *selected_bth_dev;
    U16 item_icons[] = 
    {
        IMG_PROFILES_BT,
        0,
        IMG_PROFILES_HEADSET,
        0
    };

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_ID_AUDPLY_BLUETOOTH_SETTINGS, mmi_audply_exit_bluetooth_settings, NULL, NULL);

    InitializeCategory57Screen();

    RegisterHighlightHandler(mmi_audply_bluetooth_settings_highlight_hdlr);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_AUDPLY_BLUETOOTH_SETTINGS);
    inputBuffer = GetCurrNInputBuffer(SCR_ID_AUDPLY_BLUETOOTH_SETTINGS, &inputBufferSize);

    /* init setting values */
    if (inputBuffer == NULL)
    {
        /* Check SAP state */
        if (mmi_bt_is_simap_server_activate())
        {
            selected_bt_a2dp_output = -1;     
        }
        else if (bt_settings.bt_a2dp_output)
        {
            selected_bt_a2dp_output = 1;
        }
        else
        {
            selected_bt_a2dp_output = 0;
        }
        selected_bt_a2dp_headset_temp = NULL;
        selected_bth_dev = mmi_audply_get_bt_headset();
        if (selected_bth_dev == NULL)
        {
            UCS2Strcpy((PS8)setting_bt_a2dp_headset_name, (const PS8)GetString(STR_GLOBAL_NONE));
        }
        else
        {
            med_util_utf8_to_ucs2((kal_uint8*) setting_bt_a2dp_headset_name, MMI_BT_BD_NAME_LEN,
                (kal_uint8*) selected_bth_dev->name, MMI_BT_BD_NAME_LEN);
        }
    }
    else if (selected_bt_a2dp_headset_temp)
    {
        /* back from history */
        med_util_utf8_to_ucs2((kal_uint8*) setting_bt_a2dp_headset_name, MMI_BT_BD_NAME_LEN,
            (kal_uint8*) selected_bt_a2dp_headset_temp->name, MMI_BT_BD_NAME_LEN);
    }

    /* BT A2DP Output */
    g_mmi_audply_settings_bt_a2dp_output[0] = GetString(STR_ID_AUDPLY_SETTINGS_OFF);
    g_mmi_audply_settings_bt_a2dp_output[1] = GetString(STR_ID_AUDPLY_SETTINGS_ON);
    
    SetInlineItemActivation(&wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&wgui_inline_items[n_items], (PU8)GetString((U16)(STR_ID_AUDPLY_SETTINGS_BT_A2DP_OUTPUT)));
    n_items++;
    SetInlineItemActivation(&wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemUserDefinedSelect(
        &wgui_inline_items[n_items],
        mmi_audply_bts_inline_default_callback,
        mmi_audply_bts_inline_change_select_callback,
        mmi_audply_bts_inline_change_select_callback
        );
    /*SetInlineItemSelect(&wgui_inline_items[n_items], 2, (U8 **) g_mmi_audply_settings_bt_a2dp_output,
        &selected_bt_a2dp_output);
    RegisterInlineSelectHighlightHandler(&wgui_inline_items[n_items], mmi_audply_settings_inlineitem_highlight_hdlr);*/
    

    /* BT A2DP Headset */
    n_items++;
    SetInlineItemActivation(&wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&wgui_inline_items[n_items], (PU8)GetString((U16)(STR_ID_AUDPLY_SETTINGS_BT_A2DP_HEADSET)));
    n_items++;
    SetInlineItemActivation(&wgui_inline_items[n_items], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemDisplayOnly(&wgui_inline_items[n_items], (U8*) setting_bt_a2dp_headset_name);    
    n_items++;

    if (inputBuffer != NULL)
    {
        SetCategory57Data(wgui_inline_items, n_items * 2, inputBuffer);
    }

    /* show category screen */
    ShowCategory57Screen(
        STR_ID_AUDPLY_BLUETOOTH_SETTINGS,
        GetRootTitleIcon(AUDIO_PLAYER_MAIN_MENUID),    /* title text and icon */
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,                      /* LSK text and icon */
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,                    /* RSK text and icon */
        n_items,
        item_icons,
        wgui_inline_items,
        0,
        guiBuffer);

    DisableCategory57ScreenDone();

    /* register volume inc, dec handlers */
//    SetKeyHandler(mmi_audply_press_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN);
//    SetKeyHandler(mmi_audply_press_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_exit_bluetooth_settings
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_exit_bluetooth_settings(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    history_t h;
    U16 inputBufferSize;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CloseCategory57Screen();
    h.scrnID = SCR_ID_AUDPLY_BLUETOOTH_SETTINGS;
    h.entryFuncPtr = mmi_audply_entry_bluetooth_settings;
    GetCategoryHistory(h.guiBuffer);
    inputBufferSize = (U16) GetCategory57DataSize();
    GetCategory57Data((U8*) h.inputBuffer);
    AddNHistory(h, inputBufferSize);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_bluetooth_settings_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_bluetooth_settings_highlight_hdlr(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_UP);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);
    if (index == 3) /* select headset */
    {
        ChangeLeftSoftkey(STR_GLOBAL_SELECT, 0);
        SetCategory57LeftSoftkeyFunction(mmi_audply_select_bt_a2dp_headset);
        if (selected_bt_a2dp_headset_temp)
        {
            ChangeRightSoftkey(STR_GLOBAL_DONE, 0);
            SetCategory57RightSoftkeyFunctions(mmi_audply_bluetooth_settings_done,
                mmi_audply_bluetooth_settings_done);
        }
        else
        {
            ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
            SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
        }
    }
    else if (index == 1)
    {
        SetKeyHandler(mmi_audply_sap_block_a2dp, KEY_LEFT_ARROW, KEY_EVENT_UP);
        SetKeyHandler(mmi_audply_sap_block_a2dp, KEY_RIGHT_ARROW, KEY_EVENT_UP);
        ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
        ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
        SetCategory57LeftSoftkeyFunction(mmi_audply_bluetooth_settings_done);
        SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
    }
    else
    {
        ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
        ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
        SetCategory57LeftSoftkeyFunction(mmi_audply_bluetooth_settings_done);
        SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_select_bt_a2dp_headset_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dev     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_select_bt_a2dp_headset_callback(mmi_bt_dev_struct* dev)
{
	mmi_trace(g_sw_BT, "mmi_audply_select_bt_a2dp_headset_callback");
    selected_bt_a2dp_headset_temp = dev;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_select_bt_a2dp_headset
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_select_bt_a2dp_headset(void)
{
	mmi_trace(g_sw_BT, "mmi_audply_select_bt_a2dp_headset");
    if (mmi_bootup_get_active_flight_mode())
    {
        DisplayPopup(
            (PU8) GetString(STR_ID_AUDPLY_SETTINGS_ERR_CANNOT_USE_BLUETOOTH_IN_FLIGHT_MODE),
            IMG_GLOBAL_WARNING,
            0,
            1000,
            (U8) WARNING_TONE);
    }
    else
    {
        av_bt_select_headset(mmi_audply_select_bt_a2dp_headset_callback);
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_bluetooth_settings_apply_change
 * DESCRIPTION
 *  
 * PARAMETERS
 *  bt_a2dp_output      [IN]
 *  back_to_main        [IN]
 * RETURNS
 *  BOOL
 *****************************************************************************/
BOOL mmi_audply_bluetooth_settings_apply_change(BOOL bt_a2dp_output, BOOL back_to_main)
{
    BOOL headset_changed = FALSE;
    BOOL output_changed = FALSE;
    BOOL two_layers_settings = (AUDPLY_SETTING_ITEM_COUNT > 10);

    if (selected_bt_a2dp_headset_temp != NULL && (!bt_settings.bt_a2dp_headset_valid
        || !av_bt_cmp_bt_addr(&selected_bt_a2dp_headset_temp->bd_addr, &bt_settings.bt_a2dp_headset.bd_addr)))
    {
        /* a headset is selected from list and previous headset is invalid or bt addr are different */
        headset_changed = TRUE;
    }

    if (bt_settings.bt_a2dp_output)
    {
        if (!bt_a2dp_output)
            output_changed = TRUE;
    }
    else
    {
        if (bt_a2dp_output)
            output_changed = TRUE;
    }

    /* update current setting values & write to NVRAM */
    bt_settings.bt_a2dp_output = bt_a2dp_output;
    if (selected_bt_a2dp_headset_temp)
    {
        memcpy( &bt_settings.bt_a2dp_headset, selected_bt_a2dp_headset_temp, 
            sizeof(mmi_bt_dev_struct) );
        bt_settings.bt_a2dp_headset_valid = TRUE;
    }
    mmi_audply_write_bt_settings();

    /* apply output or headset change */
    if (output_changed)
    {
        /* switch output: OFF -> ON or ON -> OFF */
        mmi_audply_switch_bt_output();
    }
    else if(bt_a2dp_output && (headset_changed || !av_bt_is_codec_open()))
    {
        /* output is ON and change headset */
        mmi_audply_sync_bt_output();
    }


    /* for sync highlight in list screen with pick index */
    g_mmi_audply_list_highlighted_item = -1;


    /* go back history */
    if (back_to_main)
    {
        /* go back to audio player main screen */
    #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
        if( bt_a2dp_output && IsScreenPresent(SCR_ID_AUDPLY_MAIN))
        {
            DisplayPopup(
                (PU8) GetString(STR_ID_AUDPLY_A2DP_BLOCK_SPECTRUM),
                IMG_GLOBAL_WARNING,
                0,
                1000,
                (U8) WARNING_TONE);
            DeleteNHistory(1);
        }
        else
    #endif
        {
            GoBackToHistory(SCR_ID_AUDPLY_MAIN);
        }
    }
    else if (two_layers_settings)
    {
        /* two layer settings has bluetooth settings screen */
        if (!DeleteScreenIfPresent(SCR_ID_AUDPLY_BLUETOOTH_SETTINGS))
        {
            /* there no popup or some screen on top of bluetooth settings screen */
        #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
            if(bt_a2dp_output && IsScreenPresent(SCR_ID_AUDPLY_MAIN))
            {
                DisplayPopup(
                    (PU8) GetString(STR_ID_AUDPLY_A2DP_BLOCK_SPECTRUM),
                    IMG_GLOBAL_WARNING,
                    0,
                    1000,
                    (U8) WARNING_TONE);
                DeleteNHistory(1);
            }
            else
        #endif
            {
                GoBackHistory();
            }
        }
    }
    else
    {
        /* only settings screen */
        if (!DeleteScreenIfPresent(SCR_ID_AUDPLY_SETTINGS))
        {
            /* there no popup or some screen on top of settings screen */
        #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
            if(bt_a2dp_output && IsScreenPresent(SCR_ID_AUDPLY_MAIN))
            {
                DisplayPopup(
                    (PU8) GetString(STR_ID_AUDPLY_A2DP_BLOCK_SPECTRUM),
                    IMG_GLOBAL_WARNING,
                    0,
                    1000,
                    (U8) WARNING_TONE);
                DeleteNHistory(1);
            }
            else
        #endif
            {
                GoBackHistory();
            }
        }
    }

    return headset_changed || output_changed;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_bt_delete_device_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  bd_addr      [IN]    when it's NULL, means all devices
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_bt_delete_device_callback(mmi_bt_dev_addr* bd_addr)
{
    if (bt_settings.bt_a2dp_output)
    {
        /* if it's delete all, or the target device bt addr is the same with ours, then turn it off */
        if (bd_addr == NULL || av_bt_cmp_bt_addr(bd_addr, &bt_settings.bt_a2dp_headset.bd_addr))
        {
            bt_settings.bt_a2dp_output = KAL_FALSE;
            bt_settings.bt_a2dp_headset_valid = KAL_FALSE;
            mmi_audply_write_bt_settings();

            /* switch output: ON -> OFF */
            mmi_audply_switch_bt_output();
        }
    }
    else if (bt_settings.bt_a2dp_headset_valid)
    {
        /* if it's delete all, or the target device bt addr is the same with ours, then set it invalid */
        if (bd_addr == NULL || av_bt_cmp_bt_addr(bd_addr, &bt_settings.bt_a2dp_headset.bd_addr))
        {
            bt_settings.bt_a2dp_headset_valid = KAL_FALSE;
            mmi_audply_write_bt_settings();
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_bt_power_off_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  success      [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_bt_power_off_callback(BOOL success)
{
    if (success)
    {
        if (bt_settings.bt_a2dp_output)
        {
            bt_settings.bt_a2dp_output = KAL_FALSE;
            mmi_audply_write_bt_settings();

            /* switch output: ON -> OFF */
            mmi_audply_switch_bt_output();
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_bt_power_on_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  success      [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_bt_power_on_callback(BOOL success)
{
    if (wait_fot_bt_pwron)
    {
        mmi_audply_bluetooth_settings_apply_change(success, FALSE);//settings_done_back_to_main
        wait_fot_bt_pwron = FALSE;
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_bluetooth_settings_done
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_bluetooth_settings_done(void)
{
    BOOL bt_a2dp_output = (selected_bt_a2dp_output == 1);

    /* only two layers settings will invoke this function, so no need back to main */
//    settings_done_back_to_main = FALSE;

    if (bt_a2dp_output && selected_bt_a2dp_headset_temp == NULL
        && !bt_settings.bt_a2dp_headset_valid)
    {
        DisplayPopup(
            (PU8) GetString(STR_ID_AUDPLY_SETTINGS_ERR_MUST_SELECT_HEADSET),
            IMG_GLOBAL_WARNING,
            0,
            1000,
            (U8) WARNING_TONE);
    }
    else if (bt_a2dp_output)
    {
        /* when BT A2DP Output is ON and BT is not power on, 
         * check if it's not in flight mode then we power on BT 
         */
        if (mmi_bootup_get_active_flight_mode())
        {
            DisplayPopup(
                (PU8) GetString(STR_ID_AUDPLY_SETTINGS_ERR_CANNOT_USE_BLUETOOTH_IN_FLIGHT_MODE),
                IMG_GLOBAL_WARNING,
                0,
                1000,
                (U8) WARNING_TONE);
        }
        else if (!mmi_bt_is_power_on(FALSE))
        {
            mmi_bt_entry_power_switch();
            wait_fot_bt_pwron = TRUE;
        }
        else
        {
            mmi_audply_bluetooth_settings_apply_change(bt_a2dp_output, FALSE);//settings_done_back_to_main
        }
    }
    else
    {
        mmi_audply_bluetooth_settings_apply_change(bt_a2dp_output, FALSE);//settings_done_back_to_main
    }
}
#endif /* __MMI_A2DP_SUPPORT__ */



#if defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
#ifndef __MMI_SUBLCD__
extern bitmap *current_LCD_device_bitmap; // defined in wgui.c
extern bitmap main_LCD_device_bitmap; // defined in wingui.c
extern bitmap sub_LCD_device_bitmap; // defined in wingui.c
#endif /* __MMI_SUBLCD__ */ 
extern S32 mmi_ucs2strlen(const S8 *arrOut);
static MMI_BOOL mmi_audply_parse_lyric(U16 *cur_char);
static MMI_BOOL mmi_audply_lrc_FS_Read_one_line(
                            FS_HANDLE handle,
                            S8 *temp_buf_pool,
                            const U32 temp_buf_pool_size,
                            INT32* read_size,
                            mmi_chset_enum lrc_encoding_chset,
                            audply_UCS2_encode_enum BL);
static void mmi_audply_redraw_lyric_background(S32 x1, S32 y1, S32 x2, S32 y2);
static S32 UCS2StrMatch(const S8 *strSrc, const S8 *break_chars, S32 matchs_count, S8 **strMatchs);
static mmi_chset_enum mmi_audply_lrc_encoding_type(const S8 *str, audply_UCS2_encode_enum *BL);
static void mmi_audply_clear_lyric_context(void);
static void mmi_audply_lyric_post_process(S8 *src, U32 total_size);
static void mmi_audply_lyric_UCS2_memcpy(S8 *dest, S8 *src, U32 total_size, audply_UCS2_encode_enum BL);
extern S8 *mmi_ucs2trim_left(const S8 *strSrc, const S8 *TrimChars);
extern S8 *mmi_ucs2cat(S8 *strDestination, const S8 *strSource);
extern S8 *mmi_ucs2chr(const S8 *strSrc, U16 c);
extern S8 *mmi_ucs2cpy(S8 *strDestination, const S8 *strSource);
extern S32 mmi_ucs2find(const S8 *strSrc, const S8 *break_chars, S32 *read_length, S16 direction);
extern U16 AnsiiToUnicode(U8 CharValue);
extern S32 mmi_ucs2toi(const S8 *strSrc, S32 *out_num, S32 *read_length);
void mmi_audply_stop_scroll_lyric_and_clean_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_audply.lyrics_display &&
        g_audply.in_main_screen &&
        mmi_audply_is_lyrics_valid()
       )
    {
        /* clean the screen */
        if (audply_scrolling_lyric_p1)
        {
            gui_scrolling_text_stop(audply_scrolling_lyric_p1);
            audply_scrolling_lyric_p1 = NULL;
        }
    #ifdef __GDI_MEMORY_PROFILE_2__
        if (audply_lyric_print.target_layer)
        {
            gdi_layer_push_and_set_active(audply_lyric_print.target_layer);
        }
    #endif /* __GDI_MEMORY_PROFILE_2__ */ 
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gdi_layer_lock_frame_buffer();
    #endif 
        gdi_layer_push_clip();
        gdi_layer_set_clip(
            audply_lyric_print.start_x,
            audply_lyric_print.start_y,
            audply_lyric_print.start_x + audply_lyric_print.width,
            audply_lyric_print.start_y + audply_lyric_print.height);
        mmi_audply_redraw_lyric_background(
            audply_lyric_print.start_x,
            audply_lyric_print.start_y,
            audply_lyric_print.start_x + audply_lyric_print.width,
            audply_lyric_print.start_y + audply_lyric_print.height);

        gdi_layer_pop_clip();
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gdi_layer_unlock_frame_buffer();
        gdi_layer_blt_previous(
            audply_lyric_print.start_x,
            audply_lyric_print.start_y,
            audply_lyric_print.start_x + audply_lyric_print.width,
            audply_lyric_print.start_y + audply_lyric_print.height);
    #endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
    #ifdef __GDI_MEMORY_PROFILE_2__
        if (audply_lyric_print.target_layer)
        {
            gdi_layer_pop_and_restore_active();
        }
    #endif /* __GDI_MEMORY_PROFILE_2__ */ 
    }
}
/*****************************************************************************
 * FUNCTION
 *  mmi_audply_redraw_lyric_background
 * DESCRIPTION
 *  redraw audio player lyric  background
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_audply_redraw_lyric_background(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT);
}
/*****************************************************************************
 * FUNCTION
 *  mmi_audply_scroll_lyric_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_scroll_lyric_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (audply_scrolling_lyric_p1)
    {
        gui_handle_scrolling_text(audply_scrolling_lyric_p1);
    }
}
/*****************************************************************************
 * FUNCTION
 *  mmi_audply_redraw_lyric
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_redraw_lyric(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    // #ifdef __MMI_SUBLCD__
    if (audply_lyric.is_ready && audply_lyric.cur_line)
    {
        S32 x, y;           /* the cursor position */
        S32 str_w, str_h;   /* the lyric width and height */
        audply_lyric_line_struct *cur_line = audply_lyric.cur_line;
        bitmap *saved_graphics_context = current_LCD_device_bitmap;

    #ifdef __GDI_MEMORY_PROFILE_2__
        if (audply_lyric_print.target_layer)
        {
            gdi_layer_push_and_set_active(audply_lyric_print.target_layer);
        }
    #endif /* __GDI_MEMORY_PROFILE_2__ */ 
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gdi_layer_lock_frame_buffer();
    #endif 
        gdi_layer_push_clip();

        gdi_layer_set_clip(
            audply_lyric_print.start_x,
            audply_lyric_print.start_y,
            audply_lyric_print.start_x + audply_lyric_print.width,
            audply_lyric_print.start_y + audply_lyric_print.height);

        if (audply_scrolling_lyric_p1)  /* if there is some scroll lyric, make it stop no matter how. */
        {
            gui_scrolling_text_stop(audply_scrolling_lyric_p1);
            audply_scrolling_lyric_p1 = NULL;
        }

        if (cur_line->line_lyric1)
        {
            gui_set_font(&MMI_medium_font);
            gui_measure_string((UI_string_type) cur_line->line_lyric1, &str_w, &str_h);
            y = audply_lyric_print.start_y + audply_lyric_print.height / 2 - str_h / 2;
            y = y-2 ; /*for bordered*/
            x = audply_lyric_print.start_x + 2  ;  /* +2 to prevent bordered char from exceeding the edge*/

            mmi_audply_redraw_lyric_background(
                audply_lyric_print.start_x,
                audply_lyric_print.start_y,
                audply_lyric_print.start_x + audply_lyric_print.width,
                audply_lyric_print.start_y + audply_lyric_print.height);

            if (str_w <= audply_lyric_print.width - 2)
            {
                S32 temp_x;

                gdi_layer_push_clip();
                /* y+4 for bordered lyrics */
                gdi_layer_set_clip(x, y, x + audply_lyric_print.width, y + str_h + 4);
                gui_set_text_color(audply_lyric_print.lyric_fg_color);
                gui_set_text_border_color(audply_lyric_print.lyric_border_color);
                if (r2lMMIFlag)
                {
                    temp_x = x + (audply_lyric_print.width + str_w) / 2;
                }
                else
                {
                    temp_x = x + (audply_lyric_print.width - str_w) / 2;
                }
                gui_move_text_cursor(temp_x, y+2);
                gui_print_bordered_text((UI_string_type) cur_line->line_lyric1);
                gdi_layer_pop_clip();
            }
            else    /* scroll */
            {
                audply_scrolling_lyric_p1 = &audply_scrolling_lyric1;
                gui_create_scrolling_text(
                    audply_scrolling_lyric_p1,
                    x,
                    y+2,
                    audply_lyric_print.width-4,   /* -4 to prevent bordered wrods from exceeding the edge*/
                    str_h,
                    (UI_string_type) cur_line->line_lyric1,
                    mmi_audply_scroll_lyric_handler,
                    mmi_audply_redraw_lyric_background,
                    audply_lyric_print.lyric_fg_color,
                    audply_lyric_print.lyric_border_color);

                audply_scrolling_lyric_p1->text_font = (UI_font_type)&MMI_medium_font;
                audply_scrolling_lyric_p1->scroll_size = 3; /* the pixel number of each scroll action. */
                audply_scrolling_lyric_p1->scroll_time = 200;   /* in ms. the time between each scroll action. */
                audply_scrolling_lyric_p1->scroll_pause = 5;    /* the pause time after showing the lyric and strat to scroll.
                                                                   the time is scroll_time*scroll_pause ; */
                audply_scrolling_lyric_p1->flags |= UI_SCROLLING_TEXT_BORDERED_TEXT;
                /* audply_scrolling_lyric_p1->scroll_gap = 32 ;  */
                /* while the whole lyric is scroll over, it should re-appear by scrolling out.
                   this is the gap in pixels between the re-appear scrolling out */
                gui_show_scrolling_text(audply_scrolling_lyric_p1);

            }
        }
        else
        {
            mmi_audply_redraw_lyric_background(
                audply_lyric_print.start_x,
                audply_lyric_print.start_y,
                audply_lyric_print.start_x + audply_lyric_print.width,
                audply_lyric_print.start_y + audply_lyric_print.height);
        }

        gdi_layer_pop_clip();

    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gdi_layer_unlock_frame_buffer();
        gdi_layer_blt_previous(
            audply_lyric_print.start_x,
            audply_lyric_print.start_y,
            audply_lyric_print.start_x + audply_lyric_print.width,
            audply_lyric_print.start_y + audply_lyric_print.height);
    #endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 

    #ifdef __GDI_MEMORY_PROFILE_2__
        if (audply_lyric_print.target_layer)
        {
            gdi_layer_pop_and_restore_active();
        }
    #endif /* __GDI_MEMORY_PROFILE_2__ */ 

        if (saved_graphics_context == &main_LCD_device_bitmap)
        {
            UI_set_main_LCD_graphics_context();
        }
        else if (saved_graphics_context == &sub_LCD_device_bitmap)
        {
            UI_set_sub_LCD_graphics_context();
        }
    }
}
void mmi_audply_lyric_on_time(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (audply_lyric.is_ready && audply_lyric.cur_line)
    {
        S32 next_line_time = 0;
        S32 play_time = (mmi_audply_get_play_seconds() - audply_lyric.offset_time)*1000;
        audply_lyric_line_struct *cur_line = audply_lyric.first_line;

	
        if (audply_lyric.cur_line->next)
        {
            next_line_time = audply_lyric.cur_line->next->time;
        }
        else
        {
            next_line_time = 0xffffff;
        }

	
        if (play_time >= audply_lyric.cur_line->time && play_time < next_line_time)
        {
        	mmi_trace(1,"lyric_reEntry=%d", lyric_reEntry);
			
            if (!lyric_reEntry)
            {
                return;
            }
            else
            {
                lyric_reEntry = MMI_FALSE;
            }
        }
        
        if (play_time < audply_lyric.first_line->time)
        {
            return;
        }
        
        while (cur_line)
        {
            if (cur_line->next)
            {
                if (play_time >= cur_line->time && play_time < cur_line->next->time)
                {
                    audply_lyric.cur_line = cur_line;
                    next_line_time = cur_line->next->time;
                    break;
                }
                else
                {
                    cur_line = cur_line->next;
                }
            }
            else
            {
                audply_lyric.cur_line = cur_line;
                next_line_time = 0xffffff;
                break;
            }
        }

        mmi_audply_redraw_lyric();
    }
}

/*****************************************************************************
 * FUNCTION
 *  UCS2StrMatch
 * DESCRIPTION
 *  For lyric parse string
 * PARAMETERS
 *  strSrc              [IN]        
 *  break_chars         [IN]        
 *  matchs_count        [IN]        
 *  strMatchs           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static S32 UCS2StrMatch(const S8 *strSrc, const S8 *break_chars, S32 matchs_count, S8 **strMatchs)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 offset, i;
    U16 *src = (U16*) strSrc;
    S32 src_length = mmi_ucs2strlen(strSrc);
    S32 break_chars_length = mmi_ucs2strlen(break_chars);
    U16 **matchs = (U16 **) strMatchs;
    S8 *is_matching = (S8*)OslMalloc(matchs_count);
    S8 *matchs_length = (S8*)OslMalloc(matchs_count);


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < matchs_count; i++)
    {
        is_matching[i] = 1;
        matchs_length[i] = mmi_ucs2strlen(strMatchs[i]);
    }

    for (offset = 0; offset < src_length; offset++)
    {
        MMI_BOOL no_match_all = MMI_TRUE;

        for (i = 0; i < matchs_count; i++)
        {
            if (!is_matching[i])    /* not match */
            {
                continue;
            }
            no_match_all = MMI_FALSE;
            if (offset == matchs_length[i])
            {
                S32 x = 0;

                for (; x < break_chars_length; x++)
                {
                    if (src[offset] == ((U16*) break_chars)[x])
                    {
                        OslMfree(is_matching);
                        OslMfree(matchs_length);
                        return i;   /* match */
                    }
                }
                is_matching[i] = 0;
            }
            else if (matchs[i][offset] != src[offset])
            {
                is_matching[i] = 0;
            }
        }
        if (no_match_all)
        {
            OslMfree(is_matching);
            OslMfree(matchs_length);
            return -1;
        }
    }
    OslMfree(is_matching);
    OslMfree(matchs_length);
    return -1;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_lyric_check_backlight
 * DESCRIPTION
 *  check if the backlight should be turned on for displaying lyric.
 *  conditions to fit : 1. under play state
 *  2. lyrics_display == 1
 *  3. lyric parse ok
 *  4. in audply main screen
 * PARAMETERS
 *  on      [IN]        Boolean to check turn on or turn off.  on:1, off:0
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_lyric_check_backlight(MMI_BOOL on)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!on && lyric_backlight_has_turn_on)    /* to turn off backlight */
    {
        TurnOffBacklight();
        lyric_backlight_has_turn_on = MMI_FALSE;
    }
    else if (on && !lyric_backlight_has_turn_on)    /* to turn on backlight while displaying lyric */
    {

        if (g_audply.state == STATE_PLAY &&
            g_audply.lyrics_display &&
            lyric_parse_ok && 
            g_audply.in_main_screen &&
            mmi_audply_is_lyrics_valid())
        {
            TurnOnBacklight(0);
            lyric_backlight_has_turn_on = MMI_TRUE;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_audply_lrc_encoding_type
 * DESCRIPTION
 *  check if the lrc is an Unicode encoded file or the user specified encoded file.
 * PARAMETERS
 *  str     [IN]        Pointer to the text source.
 *  BL      [OUT]       A number to verify big or little indian
 * RETURNS
 *  a mmi_chset_enum that sprcifies UTF8/UCS2/User Specified  encoded type.
 *****************************************************************************/
static mmi_chset_enum mmi_audply_lrc_encoding_type(const S8 *str, audply_UCS2_encode_enum *BL)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (str[0] == (S8) 0xEF && str[1] == (S8) 0xBB && str[2] == (S8) 0xBF)
    {
        *BL = NON_UCS2;
        return MMI_CHSET_UTF8;
    }
    else if (str[0] == (S8) 0xFE && str[1] == (S8) 0xFF)
    {
        *BL = UCS2_BIG_ENDIAN;
        return MMI_CHSET_UCS2;
    }
    else if (str[0] == (S8) 0xFF && str[1] == (S8) 0xFE)
    {
        *BL = UCS2_LITTLE_ENDIAN;
        return MMI_CHSET_UCS2;
    }
    else
    {
        *BL = NON_UCS2;
        return PhnsetGetDefEncodingType();
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_stop_lyric
 * DESCRIPTION
 *  called when audio playing is stopped
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_stop_lyric(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* clean the scrolling screen. */
    mmi_audply_stop_scroll_lyric_and_clean_screen();
    audply_lyric.is_ready = MMI_FALSE;
    lyric_parse_ok = MMI_FALSE;
    mmi_audply_lyric_check_backlight(0);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_clear_lyric_context
 * DESCRIPTION
 *  Clear all lyric context and set audply_lyric.is_ready to FALSE.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_audply_clear_lyric_context(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (audply_scrolling_lyric_p1)
    {
        gui_scrolling_text_stop(audply_scrolling_lyric_p1);
        audply_scrolling_lyric_p1 = NULL;
    }
    memset(g_mmi_lyric_str_buff, 0, AUDPLY_LYRIC_STR_BUFF_MAX_SIZE);
    memset(&audply_lyric, 0, sizeof(audply_lyric_struct));
    lyric_full = MMI_FALSE;
    audply_lyric.is_ready = MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_audply_lyric_post_process
 * DESCRIPTION
 *  If  the string buffer pool is not large enough, the string might ended in the middle of a
 *  lyrics line.which indicates that it is not ended in '\n' nor '\t'. the parser only takes the
 *  lyrics ended with '\n' or '\t'. this function try to take care of it.
 * PARAMETERS
 *  src             [?]         
 *  total_size      [IN]        in bytes
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_audply_lyric_post_process(S8 *src, U32 total_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 num_length = 0;
    S8 *temp_src = src + total_size - 2;    /* subtract by 2 because it is 2 byte aligned. */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	mmi_trace(1,"mmi_audply_lyric_post_process()");
	
    /* U16* temp_src = (U16*)src + (total_size/2) -1  ; */
    if (mmi_ucs2find(temp_src, (S8*) L"\r\n", &num_length, -1) == 1)
    {
        temp_src -= num_length * 2;
        memset(temp_src + 2, 0, num_length * 2);
    }
    else
    {
        src[total_size - 1] = 0x00;
        src[total_size - 2] = '\n';
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_audply_lyric_UCS2_memcpy
 * DESCRIPTION
 *  If  the string buffer pool is not large enough, the string might ended in the middle of a
 *  lyrics line.which indicates that it is not ended in '\n' nor '\t'. the parser only takes the
 *  lyrics ended with '\n' or '\t'. this function try to take care of it.
 * PARAMETERS
 *  dest            [?]         
 *  src             [?]         
 *  total_size      [IN]        in bytes
 *  BL              [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_audply_lyric_UCS2_memcpy(S8 *dest, S8 *src, U32 total_size, audply_UCS2_encode_enum BL)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (BL == UCS2_LITTLE_ENDIAN)
    {
        memcpy(dest, src, total_size);
    }
    else if (BL == UCS2_BIG_ENDIAN)
    {
        U32 i;

        for (i = 0; i < total_size; i += 2)
        {
            *(dest + i) = *(src + i + 1);
            *(dest + i + 1) = *(src + i);
        }
    }

}


/*****************************************************************************
 * FUNCTION
 *  mmi_audply_lrc_FS_Read_one_line
 * DESCRIPTION
 *  read a line from FS into a temp buffer
 *  1. if the buffer is not large enough : append '\n' in the end of it and seek for real '\n' in the FS
 *  2. if the buffer is larger than one line : seek for first '\n' in FS
 * IMPACT
 *  when it is successfully parsed, it'll set audply_lyric.is_ready to TRUE.
 *  otherwise, it is reset to FALSE by calling mmi_audply_clear_lyric().
 * PARAMETERS
 *  handle                  [IN]        File handle for FS
 *  temp_buf_pool           [IN]        Temp buffer bool to store the read chars from FS
 
 *  read_size               [OUT]       The real read size for one line
 *  lrc_encoding_chset      [IN]        
 *  BL                      [IN]        To indicate big or little endian
 * RETURNS
 *  return TRUE if it's successfully read; otherwise, return FALSE.
 *****************************************************************************/
static MMI_BOOL mmi_audply_lrc_FS_Read_one_line(
                                        FS_HANDLE handle,
                                        S8 *temp_buf_pool,
                                        const U32 temp_buf_pool_size,
                                        INT32* read_size,
                                        mmi_chset_enum lrc_encoding_chset,
                                        audply_UCS2_encode_enum BL)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_FS_Read(handle, (void *)temp_buf_pool, temp_buf_pool_size, (INT32*)read_size) < 0)
    {
        return MMI_FALSE;
    }
    else if (*read_size != 0)
    {
        U32 i;
        U32 line_len;
        U32 read_over_total_line_len = 0;
        MMI_BOOL found = MMI_FALSE;
	
        for (i = 0; i < *read_size; i++)
        {
            if (lrc_encoding_chset == MMI_CHSET_UCS2)
            {
                if (BL == UCS2_LITTLE_ENDIAN)
                {
                    if (temp_buf_pool[i] == '\n' && temp_buf_pool[i + 1] == 0)
                    {
                        found = MMI_TRUE;
                        break;
                    }
                }
                else if (BL == UCS2_BIG_ENDIAN)
                {
                    if (temp_buf_pool[i] == 0 && temp_buf_pool[i + 1] == '\n')
                    {
                        found = MMI_TRUE;
                        break;
                    }
                }
                i++;
            }
            else
            {
                if (temp_buf_pool[i] == '\n')
                {
                    found = MMI_TRUE;
                    break;
                }
            }
        }

        if (found)
        {
            /* if found, i will not be added by for loop. */
            line_len = i + 1;
            /* make it 2 byte aligned. */
            if (lrc_encoding_chset == MMI_CHSET_UCS2)
            {
                line_len++;
            }
        }
        else
        {
            line_len = i;
        }
        /* reach the end of file */
        if ((line_len < *read_size) || (line_len == *read_size && found))
        {
            /* seek back to the end of first line */
            MMI_FS_Seek(handle, line_len - *read_size, FS_FILE_CURRENT);
            /* replace with zero value till the end of temp_buf */
            memset(temp_buf_pool + line_len, 0, *read_size - line_len);
        }
        else    /* line_len == *read_size && !found */
        {
            S8 *read_over_buf = NULL;
            const U32 read_over_buf_size = 256;
            INT32  read_over_read_size;
            U32 j, read_over_line_len;
            U32 read_over_total_size = 0;

            if ((read_over_buf = OslMalloc(read_over_buf_size)) == NULL)
            {
                return MMI_FALSE;
            }
            else
            {
                memset(read_over_buf, 0, read_over_buf_size);
            }

            if(temp_buf_pool_size-line_len>=2)
            {
                line_len += 2 ;
                *read_size = line_len;
            }

            if (BL == UCS2_LITTLE_ENDIAN)
            {
                temp_buf_pool[line_len - 1] = 0;
                temp_buf_pool[line_len - 2] = '\n';

            }
            else if (BL == UCS2_BIG_ENDIAN)
            {
                temp_buf_pool[line_len - 1] = '\n';
                temp_buf_pool[line_len - 2] = 0;
            }
            else
            {
                temp_buf_pool[line_len - 2] = '\n';
                temp_buf_pool[line_len - 1] = 0;
            }

            found = MMI_FALSE;
            do
            {
                if (MMI_FS_Read(handle, (void *)read_over_buf, read_over_buf_size, (INT32*) &read_over_read_size) < 0)
                {
                    OslMfree(read_over_buf);
                    return MMI_FALSE;
                }

                if (!read_over_read_size)   /* end of file */
                {
                    OslMfree(read_over_buf);
                    return MMI_TRUE;
                }
                else
                {
                    for (j = 0; j < read_over_read_size; j++)
                    {
                        if (lrc_encoding_chset == MMI_CHSET_UCS2)
                        {
                            if (BL == UCS2_LITTLE_ENDIAN)
                            {
                                if (read_over_buf[j] == '\n' && read_over_buf[j + 1] == 0)
                                {
                                    found = MMI_TRUE;
                                    break;
                                }
                            }
                            else if (BL == UCS2_BIG_ENDIAN)
                            {
                                if (read_over_buf[j] == 0 && read_over_buf[j + 1] == '\n')
                                {
                                    found = MMI_TRUE;
                                    break;
                                }
                            }
                            j++;
                        }
                        else
                        {
                            if (read_over_buf[j] == '\n')
                            {
                                found = MMI_TRUE;
                                break;
                            }
                        }

                    }
                    read_over_line_len = j + 1;

                    /* make it 2 byte aligned */
                    if (lrc_encoding_chset == MMI_CHSET_UCS2)
                    {
                        read_over_line_len++;
                    }

                    read_over_total_line_len += read_over_line_len;
                    read_over_total_size += read_over_read_size;
                }
                /* haven't reach the end */
            } while (read_over_line_len >= read_over_read_size && !found);
            OslMfree(read_over_buf);
            MMI_FS_Seek(handle, read_over_total_line_len - read_over_total_size, FS_FILE_CURRENT);
        }
        *read_size = line_len;  /* + read_over_total_line_len ; */
        return MMI_TRUE;
    }
    else
    {
        return MMI_TRUE;
    }

}


/*****************************************************************************
 * FUNCTION
 *  mmi_audply_init_lyric
 * DESCRIPTION
 *  Init and parse lyric
 * IMPACT
 *  when it is successfully parsed, it'll set audply_lyric.is_ready to TRUE.
 *  otherwise, it is reset to FALSE by calling mmi_audply_clear_lyric().
 * PARAMETERS
 *  void
 * RETURNS
 *  return TRUE if it's successfully parsed; otherwise, return FALSE.
 *****************************************************************************/
MMI_BOOL mmi_audply_init_lyric(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FS_HANDLE handle;
    UI_character_type *file_name_end = NULL, *temp_file_name_end = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

#ifdef __MMI_AUDIO_PLAYER_LYRIC_SCREEN_BASED_ASM__
    if (g_mmi_lyric_str_buff == NULL)
    {
        return MMI_FALSE;
    }
#endif /*__MMI_AUDIO_PLAYER_LYRIC_SCREEN_BASED_ASM__*/

    mmi_audply_clear_lyric_context();

    mmi_ucs2cpy((PS8)audply_lyric.lyric_file_path, (const PS8)g_audply.filefullname);

    temp_file_name_end = audply_lyric.lyric_file_path;
    while( (temp_file_name_end = (UI_character_type*) mmi_ucs2chr((S8*) temp_file_name_end, AnsiiToUnicode('.'))) != NULL )
    {
        file_name_end = temp_file_name_end;
        temp_file_name_end++;
    }
	
    if (file_name_end)
    {
        *file_name_end = 0;
    }
    mmi_ucs2cat((PS8)audply_lyric.lyric_file_path, (const PS8)L".lrc");

	handle = MMI_FS_Open((UINT8*) audply_lyric.lyric_file_path, FS_READ_ONLY);
	
    if (handle >= FS_NO_ERROR)
    {
        INT32  file_size = 0;
        U32 str_size = 0;

        S8 *temp_buf_pool = NULL;
        const U32 temp_buf_pool_size = AUDPLY_LYRIC_TMP_BUFF_MAX_SIZE;   /* 128 ;//256 ; */
        INT32 read_size;

        S8 *str_buf_pool = g_mmi_lyric_str_buff;    /* converted buff pool */
        S8 *temp_str_buf_pool = str_buf_pool;
        U32 buf_size = AUDPLY_LYRIC_STR_BUFF_MAX_SIZE;           /* size of converted buff pool */
        U32 temp_str_buf_size = buf_size;

        U32 total_read_size = 0;
        U32 total_str_size = 0;

        audply_UCS2_encode_enum BL = NON_UCS2;  /* when it is encoded in UCS2, use this variable to indicate Big or Little endian */
	
        if (MMI_FS_GetFileSize(handle, &file_size) < 0 || !file_size)
        {
            MMI_FS_Close(handle);
            return MMI_FALSE;
        }

        if ((temp_buf_pool = OslMalloc(temp_buf_pool_size)) == NULL)
        {
            MMI_FS_Close(handle);
            return MMI_FALSE;
        }
        memset(temp_buf_pool, 0, temp_buf_pool_size);

        /* to know what coding type it is */
        MMI_FS_Read(handle, (void *)temp_buf_pool, 3, (INT32 *)&read_size);
	
        audply_lyric.lrc_encoding_chset = mmi_audply_lrc_encoding_type(temp_buf_pool, &BL);
        MMI_FS_Seek(handle, -3, FS_FILE_CURRENT);
	
        while (total_read_size < file_size && !lyric_full)
        {
			
            /* read one line until '\n' */
            if (mmi_audply_lrc_FS_Read_one_line
                (handle, temp_buf_pool, temp_buf_pool_size, &read_size, audply_lyric.lrc_encoding_chset, BL) == MMI_FALSE)
            {
                OslMfree((void*)temp_buf_pool);
                MMI_FS_Close(handle);
                return MMI_FALSE;
            }
            if (!read_size)
            {
                break;
            }
            // append zero if not large enough.
            //                      if(read_size<temp_buf_pool_size)
            //                              memset(temp_buf_pool+read_size,0,temp_buf_pool_size-read_size) ;

            if (audply_lyric.lrc_encoding_chset != MMI_CHSET_UCS2)
            {

                if (temp_str_buf_size >= 4) /* if size is smaller than 4 , the converter might over write the un-authorized memory space. */
                {
                    str_size = mmi_chset_convert(
                                audply_lyric.lrc_encoding_chset,
                                MMI_CHSET_UCS2,
                                temp_buf_pool,
                                temp_str_buf_pool,
                                temp_str_buf_size);
                    /* str_size -= 2 ; */
                    temp_str_buf_pool[str_size - 1] = 0;
                    temp_str_buf_pool[str_size - 2] = '\n';
                }
                if (temp_str_buf_size == str_size + 2)
                {
                    mmi_audply_lyric_post_process(str_buf_pool, buf_size);
                }

                // if need to concern about the str_buf not adequate situation, need to append '\n' ??
                //MMI_ASSERT(str_size!=0) ;
            }
            else
            {
                if (temp_str_buf_size < read_size)
                {
                    mmi_audply_lyric_UCS2_memcpy(temp_str_buf_pool, temp_buf_pool, temp_str_buf_size, BL);
                    str_size = read_size;
                    mmi_audply_lyric_post_process(str_buf_pool, buf_size);
                }
                else
                {
                    mmi_audply_lyric_UCS2_memcpy(temp_str_buf_pool, temp_buf_pool, read_size, BL);
                    str_size = read_size;
                }
            }

            if (!(mmi_audply_parse_lyric((U16*) temp_str_buf_pool)))
            {
                OslMfree((void*)temp_buf_pool);
                audply_lyric.is_ready = MMI_FALSE;
                MMI_FS_Close(handle);
                return MMI_FALSE;
            }

            /* str_buff is not adequate for the lrc file. */
            if (total_str_size + str_size > buf_size)   /* temp_str_buf_size) */
            {
                total_str_size = buf_size;
                break;
            }
            total_str_size += str_size;
            total_read_size += read_size;
            temp_str_buf_size -= str_size;
            temp_str_buf_pool += str_size;
	   
        }

        OslMfree((void*)temp_buf_pool);

/*        if (total_str_size > buf_size)
        {
            audply_lyric.lyric_buf_pool.str_size = buf_size;
        }
        else
        {
            audply_lyric.lyric_buf_pool.str_size = total_str_size - 2;
        }

        audply_lyric.lyric_buf_pool.buf = str_buf_pool;
        audply_lyric.lyric_buf_pool.buf_size = buf_size;*/

        audply_lyric.cur_line = audply_lyric.first_line;
        audply_lyric.is_ready = MMI_TRUE;
        MMI_FS_Close(handle);

        return MMI_TRUE;

    }
    return MMI_FALSE;   /* no such .lrc exists. */
}


/*****************************************************************************
 * FUNCTION
 *  mmi_audply_parse_lyric
 * DESCRIPTION
 *  parse current lyric string
 * PARAMETERS
 *  cur_char        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
static MMI_BOOL mmi_audply_parse_lyric(U16 *cur_char)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    audply_lyric_parse_status_enum status = LINE_START;
    S32 match_index = 0;
    S32 num_length = 0;
    S32 time_phase1, time_phase2, time_phase3, time_phaseMS;
    U32 temp_time;
    S32 temp_num, i, j;
    U16 *temp_ptrU16 = NULL;
    U16 *tag_ids[5] = 
	{
		(UI_character_type*)L"ar", 
		(UI_character_type*)L"ti", 
		(UI_character_type*)L"al", 
		(UI_character_type*)L"by", 
		(UI_character_type*)L"offset" 
	};

    S32 time_count_line = 0;    /* counting how many time tages for one lyric */
    S8 *line_lyric1 = NULL ;

    //      U16             line_lyric1_size;
    //      S8*             line_lyric2;
    //      U16             line_lyric2_size;
    audply_lyric_line_struct *temp_lyric = NULL;
    audply_lyric_line_struct *cur_lyric = NULL;
    audply_lyric_line_struct *pre_lyric = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (cur_char)
    {
    	mmi_trace(1,"mmi_audply_parse_lyric,status=%d",status);
        switch (status)
        {
            case LINE_START:
                time_count_line = 0;
                line_lyric1 = NULL;
                //                      line_lyric1_size = 0;
                //                      line_lyric2 = NULL;
                //                      line_lyric2_size = 0;

                if ((cur_char = (U16*) mmi_ucs2chr((S8*) cur_char, AnsiiToUnicode('['))) == NULL)
                {
                    return MMI_TRUE;
                }
                else
                {
                    status = TAG_START;
                }
                break;

            case LINE_END:
		 mmi_trace(1,"line_lyric1=%x, time_count_line=%d", line_lyric1,time_count_line);
                if (line_lyric1 || time_count_line)
                {
                    if (!audply_lyric.first_line)
                    {
                        audply_lyric.first_line = &audply_lyric.lyric[0];
                        for (j = 0; j < time_count_line; j++)
                        {
                            if (audply_lyric.first_line->time > audply_lyric.lyric[j].time)
                            {
                                audply_lyric.first_line = &audply_lyric.lyric[j];
                            }
                        }
                    }
                    for (i = 0; i < time_count_line; i++)
                    {
                        temp_lyric = &audply_lyric.lyric[audply_lyric.lyric_count - i - 1];
                        temp_lyric->line_lyric1 = line_lyric1;
                        /* temp_lyric->line_lyric2 = line_lyric2; */
                        cur_lyric = audply_lyric.first_line;
                        pre_lyric = NULL;
                        while (cur_lyric)
                        {
							if(temp_lyric == cur_lyric)
							{
								break;
							}
                            if (temp_lyric->time < cur_lyric->time)
                            {
                                temp_lyric->next = cur_lyric;
                                if (pre_lyric)
                                {
                                    pre_lyric->next = temp_lyric;
                                }
                                else
                                {
                                    audply_lyric.first_line = temp_lyric;
                                }
                                break;
                            }
                            pre_lyric = cur_lyric;
                            cur_lyric = cur_lyric->next;
                            ASSERT(pre_lyric != cur_lyric);                            
                        }
                        if (!cur_lyric && temp_lyric != audply_lyric.first_line)
                        {
                            if (pre_lyric)
                            {
                                pre_lyric->next = temp_lyric;
                            }
                            else
                            {
                                audply_lyric.first_line = temp_lyric;
                            }
                        }
                    }
                }
                return MMI_TRUE;
            case TAG_START:
                time_phase1 = 0xffffff;
                time_phase2 = 0xffffff;
                time_phase3 = 0xffffff;
                time_phaseMS = 0xffffff;

                TRIM_LEFT;
                match_index = UCS2StrMatch((S8*) cur_char, (S8*) L"[]:. \t\r\n", 5, (S8 **) tag_ids);

                if (match_index == -1)
                {
                    cur_char--;
                    status = TAG_TIME1;
                }
                else
                {
                    cur_char += mmi_ucs2strlen((S8*) tag_ids[match_index]);
                    TRIM_LEFT;
                    if (!cur_char)
                    {
                        return MMI_TRUE;
                    }
                    if (*cur_char != AnsiiToUnicode(':'))
                    {
                        FIND_TAG_END;
                    }
                    else
                    {
                        switch (match_index)    /* { L"ar", L"ti", L"al", L"by", L"offset" } */
                        {
                            case 0:
                                status = TAG_ID_AR;
                                break;
                            case 1:
                                status = TAG_ID_TI;
                                break;
                            case 2:
                                status = TAG_ID_AL;
                                break;
                            case 3:
                                status = TAG_ID_BY;
                                break;
                            case 4:
                                status = TAG_ID_OFFSET;
                                break;
                        }
                    }
                }
                break;

            case TAG_END:
                temp_time = 0xffffff;
                if (time_phase1 != 0xffffff && time_phase2 != 0xffffff && time_phase3 != 0xffffff)
                {
                    temp_time = time_phase1 * 3600 + time_phase2 * 60 + time_phase3;
                }
                else if (time_phase1 != 0xffffff && time_phase2 != 0xffffff)
                {
                    temp_time = time_phase1 * 60 + time_phase2;
                }
                else if (time_phase1 != 0xffffff)
                {
                    temp_time = time_phase1;
                }

                if(temp_time != 0xffffff)
                {
                    temp_time *= 1000 ;
                    if (time_phaseMS != 0xffffff)
                    {
                        temp_time += time_phaseMS ;                                            
                    }
                }
                else
                {
                    if (time_phaseMS != 0xffffff)
                    {
                        temp_time = time_phaseMS ;
                     }
                }
                
                if (temp_time != 0xffffff)
                {
                    /* handle more than one time tags for one lyric */
                    temp_lyric = &audply_lyric.lyric[audply_lyric.lyric_count++];
                    time_count_line++;
                    temp_lyric->next = NULL;
                    temp_lyric->time = temp_time;
		     mmi_trace(1,"temp_lyric->time = %d", temp_lyric->time);
                    if (audply_lyric.lyric_count == AUDPLY_LYRIC_MAX_LINE_COUNT)
                    {
                        lyric_full = MMI_TRUE;
                        return MMI_TRUE;
                    }
                }

                cur_char = (U16*) mmi_ucs2trim_left((S8*) cur_char, (S8*) L" \t");
                if (!cur_char)
                {
                    return MMI_TRUE;
                }
                if (*cur_char ==AnsiiToUnicode('['))
                {
                    status = TAG_START;
                }
                else if (*cur_char == AnsiiToUnicode('\r') || *cur_char == AnsiiToUnicode('\n'))
                {
                    status = LINE_END;
                }
                else
                {
                    status = TAG_LYRIC1;
                    continue;
                }
                break;

            case TAG_LYRIC1:
                if (mmi_ucs2find((S8*) cur_char, (S8*) L"\r\n", &num_length, 1) == -1)
                {
                    return MMI_FALSE;
                }
                line_lyric1 = (S8*) cur_char;
                cur_char += num_length;
                /*       if (*cur_char == L'`')
                   {
                   status = TAG_LYRIC2;
                   continue;
                   } */
                if (*cur_char == AnsiiToUnicode('\r') || *cur_char == AnsiiToUnicode('\n'))
                {
                    status = LINE_END;
                }
                *cur_char = 0;
                break;

                /*
                 * case TAG_LYRIC2:
                 * if (mmi_ucs2find((S8*)cur_char,(S8*)L"\r\n", &num_length, 1) == -1)
                 * {
                 * return MMI_FALSE;
                 * }
                 * line_lyric2 = (S8*)cur_char;
                 * cur_char += num_length;
                 * status = LINE_END;
                 * *cur_char = 0;
                 * break;
                 */
            case TAG_TIME1:
                TRIM_LEFT;
                if (mmi_ucs2toi((S8*) cur_char, &time_phase1, &num_length) >= 0 &&
                    time_phase1 >= 0 && time_phase1 <= 60)
                {
                    cur_char += num_length;
                    TRIM_LEFT;
                    if (*cur_char == AnsiiToUnicode(':'))
                    {
                        status = TAG_TIME2;
                    }
                    else if (*cur_char == AnsiiToUnicode('.'))
                    {
                        status = TAG_MS;
                    }
                    else
                    {
                        FIND_TAG_END;
                    }
                }
                else
                {
                    time_phase1 = 0xffffff;
                    FIND_TAG_END;
                }
                break;

            case TAG_TIME2:
                TRIM_LEFT;
                if (mmi_ucs2toi((S8*) cur_char, &time_phase2, &num_length) >= 0 &&
                    time_phase2 >= 0 && time_phase2 <= 60)
                {
                    cur_char += num_length;
                    TRIM_LEFT;
                    if (*cur_char == AnsiiToUnicode(':'))
                    {
                        status = TAG_TIME3;
                    }
                    else if (*cur_char == AnsiiToUnicode('.'))
                    {
                        status = TAG_MS;
                    }
                    else
                    {
                        FIND_TAG_END;
                    }
                }
                else
                {
                    time_phase2 = 0xffffff;
                    FIND_TAG_END;
                }
                break;

            case TAG_TIME3:
                TRIM_LEFT;
                if (mmi_ucs2toi((S8*) cur_char, &time_phase3, &num_length) >= 0 &&
                    time_phase3 >= 0 && time_phase3 <= 60)
                {
                    cur_char += num_length;
                    TRIM_LEFT;
                    if (*cur_char == AnsiiToUnicode('.'))
                    {
                        status = TAG_MS;
                    }
                    else
                    {
                        FIND_TAG_END;
                    }
                }
                else
                {
                    time_phase3 = 0xffffff;
                    FIND_TAG_END;
                }
                break;

            case TAG_MS:
                TRIM_LEFT;
                if (mmi_ucs2toi((S8*) cur_char, &time_phaseMS, &num_length) >= 0 &&
                      time_phaseMS >= 0 && time_phaseMS < 1000)
                {
                    if(time_phaseMS>0 && time_phaseMS < 10)
                    {
                        time_phaseMS *= 100 ;
                    }
                    else if(time_phaseMS>=10 && time_phaseMS < 100)
                    {
                        time_phaseMS *= 10 ;
                    }
                }
                else
                {
                    time_phaseMS = 0xffffff;
                }
                FIND_TAG_END;
                break;
            case TAG_ID_AR:
                temp_ptrU16 = (U16*) mmi_ucs2chr((S8*) cur_char, AnsiiToUnicode(']'));
                if (temp_ptrU16 == NULL)
                {
                    return MMI_TRUE;
                }
                /* if (temp_ptrU16 - cur_char <= MAX_ID3_ARTIST_LEN + 1)
                   {
                   mmi_ucs2ncpy((S8*)audply_lyric.lyric_tag_id.artist,(S8*)cur_char,
                   temp_ptrU16 - cur_char );
                   } */
                FIND_TAG_END;
                break;

            case TAG_ID_TI:
                temp_ptrU16 = (U16*) mmi_ucs2chr((S8*) cur_char, AnsiiToUnicode(']'));
                if (temp_ptrU16 == NULL)
                {
                    return MMI_TRUE;
                }
                /* if (temp_ptrU16 - cur_char <= MAX_ID3_TITLE_LEN + 1)
                   {
                   mmi_ucs2ncpy((S8*)audply_lyric.lyric_tag_id.title,(S8*)cur_char,
                   temp_ptrU16 - cur_char );
                   } */
                FIND_TAG_END;
                break;

            case TAG_ID_AL:
                temp_ptrU16 = (U16*) mmi_ucs2chr((S8*) cur_char, AnsiiToUnicode(']'));
                if (temp_ptrU16 == NULL)
                {
                    return MMI_TRUE;
                }
                /* if (temp_ptrU16 - cur_char <= MAX_ID3_ALBUM_LEN + 1)
                   {
                   mmi_ucs2ncpy((S8*)audply_lyric.lyric_tag_id.album,(S8*)cur_char,
                   temp_ptrU16 - cur_char );
                   } */
                FIND_TAG_END;
                break;

            case TAG_ID_BY:
                temp_ptrU16 = (U16*) mmi_ucs2chr((S8*) cur_char, AnsiiToUnicode(']'));
                if (temp_ptrU16 == NULL)
                {
                    return MMI_TRUE;
                }
                /* if (temp_ptrU16 - cur_char <= MAX_ID3_GENRE_LEN + 1)
                   {
                   mmi_ucs2ncpy((S8*)audply_lyric.lyric_tag_id.genre,(S8*)cur_char,
                   temp_ptrU16 - cur_char );
                   } */
                FIND_TAG_END;
                break;

            case TAG_ID_OFFSET:
                TRIM_LEFT;
                if (mmi_ucs2toi((S8*) cur_char, &temp_num, &num_length) >= 0)
                {
                    audply_lyric.offset_time = temp_num;
                }
                FIND_TAG_END;
                break;

            default:
                break;
        }   /* End switch */

        cur_char++;
    }   /* End whlie */
    return MMI_TRUE;
}
#endif /* __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__ */ 

void mmi_audply_clean_app(void)
{
    //   DeleteScreenIfPresent(SCR_ID_ANALOGTV_APP);
    if(mmi_audply_is_playing()||IsScreenPresent( SCR_ID_AUDPLY_MAIN))
    {
        mmi_audply_stop_playing();
        DeleteUptoScrID(IDLE_SCREEN_ID);
    }
}

//use IMG_DM_SWITCH_PROGRESS to reduce the flash size
#include "mtpnp_ad_resdef.h"
BOOL mmi_audply_gen_playlist_progress(U8 index)
{
//relate to the resource IMG_DM_SWITCH_PROGRESS
//image's frame count
#ifdef __MMI_MAINLCD_176X220__  //other LCD SIZE need adjust the w,h 
#define GIF_FRAMES 18
//image's w
#define w  160
//image's h
#define h  20

    if(!IsScreenPresent(SCR_ID_AUDPLY_MAIN))
        return MMI_TRUE;
    gdi_layer_lock_frame_buffer();
 //   gdi_draw_solid_rect(x1-2, y1-10, x1+w+2, y1+h+10, GDI_COLOR_TRANSPARENT);
    if(index==0)
    gdi_draw_round_rect(4,
                                   ((UI_device_height-h)>>1) - (h>>1),
                                    UI_device_width -4-1 ,
                                    ((UI_device_height-h)>>1)+h+(h>>1),
                                    GDI_COLOR_GRAY,GDI_COLOR_GRAY,2);


    gdi_image_draw_animation_single_frame_id(
        (UI_device_width-w)>>1, 
        (UI_device_height-h)>>1, 
        IMG_GLOBAL_PROGRESS, 
        index%GIF_FRAMES);
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(4,
                                   ((UI_device_height-h)>>1) - (h>>1),
                                    UI_device_width -4-1 ,
                                    ((UI_device_height-h)>>1)+h+(h>>1));
#endif
    return MMI_TRUE;
}

