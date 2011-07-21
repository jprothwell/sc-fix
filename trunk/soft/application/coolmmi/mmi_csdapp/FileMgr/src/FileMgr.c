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
 *	 FileMgr.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  File Manager.
 *
 * Author:
 * -------
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef MMI_ON_HARDWARE_P
    #undef __DRM_SUPPORT__
    #undef __DRM_V01__
#endif



#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

#include "protocolevents.h"
#include "queuegprot.h"
#include "unicodexdcl.h"		/* unicode transform functions */
#include "nvramtype.h"			/* nvram data type */
#include "nvramprot.h"			/* nvram access fucntions */
#include "nvramenum.h"			/* nvram id define */
#include "timerevents.h"
#include "globaldefs.h"			/* global image, string define */
#include "globalscrenum.h"
#include "globalmenuitems.h"
#include "commonscreens.h"
#include "debuginitdef.h"
#include "historygprot.h"

#include "wgui_datetime.h"
#include "wgui_categories.h"
#include "wgui_categories_fmgr.h"
#include "wgui_categories_multimedia.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "gdi_include.h"		/* for gdi_assert */
#ifdef __MMI_TOUCH_SCREEN__
    #include "wgui_touch_screen.h"
#endif

#include "filemgr.h"
#include "datetimetype.h"
#include "mainmenudef.h"
#include "gpioinc.h"
#include "idleappprot.h"
#include "simdetectiongexdcl.h"
#include "simdetectiongprot.h"
#include "profilegprots.h"
#include "phonesetupgprots.h"
#include "phonebookgprot.h"
#include "todolistenum.h"
#include "alarmdef.h"
#include "settingprofile.h"		/* error tone */
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "fat_fs.h"
#include "fmt_struct.h"
#include "fmt_main.h"
#include "filesystemdef.h"		/* file function */
#include "callmanagementgprot.h"
#include "wpssprotos.h"
#include "conversions.h"
#include "mmithemes.h"
// Lisen 0623
#include "factorymodeprot.h"

#include "gbhmi.h" //added by zhoumn for the bug of editor full @2007/06/16

#ifdef __DRM_SUPPORT__
    #include "drm_gprot.h"
    #include "drm_main.h"
    #include "wap_ps_struct.h"
    #include "wapadp.h"
//#include "stk_if.h"
#endif /*__DRM_SUPPORT__*/
#ifdef __MMI_MESSAGES_EMS__
    #include "messagesexdcl.h"
#endif
#ifdef __MMI_MMS__
    #include "wapprot.h"
#endif
#ifdef __MMI_VCALENDAR__
    #include "vcalendar.h"
#endif
#ifdef __MMI_VCARD__
    #include "vcard.h"
#endif
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
    #include "profilemgrgprot.h"
#endif
#ifdef __MMI_RING_COMPOSER__
    #include "composeringtonedefs.h"
    #include "composeringtoneprots.h"	/* for ring tone composer */
#endif
#ifdef __MMI_SOUND_RECORDER__
    #include "soundrecorderdef.h"
    #include "soundrecorderprot.h"
#endif
#ifdef __MMI_AUDIO_PLAYER__
    #include "audioplayerdef.h"
    #include "audioplayerprot.h"
#endif
#ifdef __MMI_AB_REPEATER__
    #include "abrepeaterdef.h"
    #include "abrepeaterprot.h"
#endif
#ifdef __MMI_CAMERA__
    #include "gdi_lcd_config.h"
    #include "mmi_features_camera.h"
    #include "mdi_datatype.h"
    #include "mdi_camera.h"
    #include "cameraapp.h"
    #include "cameramenu.h"
    #include "imageviewer.h"
    #include "imageviewerresdefs.h"
#endif
#ifdef __MMI_VIDEO__
    #include "mmi_features_video.h"
    #include "vdoplyscrenum.h"
    #include "vdoplystrenum.h"
    #include "vdoplyimgenum.h"
    #include "vdoplygprot.h"
#endif
#ifdef __MMI_PHOTOEDITOR__
    #include "photoeditorgprot.h"
#endif 
#ifdef __MMI_IRDA_SUPPORT__
    #include "irdammigprots.h"
#endif
#ifdef __J2ME__
    #include "javaprot.h"
#endif
#ifdef __MMI_EMAIL__
    #include "emailappgprot.h"
#endif

#ifdef __MMI_FM_RADIO_RECORD__
    #include "fmradioprot.h"
#endif
#ifdef __MMI_SOUND_RECORDER__
    #include "soundrecorderdef.h"
#endif

#ifdef __MMI_EBOOK_READER__
#include "ebookdefs.h"
#include "ebookprot.h"
#include "ebookgprot.h"
#include "ebookmanager.h"
#endif /* __MMI_EBOOK_READER__ */ 

#ifdef __MMI_OPP_SUPPORT__
#include "oppmmigprots.h"
#endif
#include "filemgrstruct.h"
#include "filemanagerdef.h"
#include "filemanagergprot.h"
#include "thememanager.h"
#include  "funandgamesdefs.h"

#include "mmi_trace.h"
#include "vdoplystrenum.h"
#include "cameraapp.h"
#undef __NEWSIMULATOR 

#ifdef __MMI_ACCEL_OPP_SUPPORT__
	#include "oppmmigprots.h"
#endif
#include "mci.h"
#include "usb_interface.h"
#ifdef __MMI_BT_COOLSAND_SUPPORT__
#if defined(__MMI_FTS_SUPPORT__)
#include "xml_def.h"
#include "btmmiscrgprots.h"
#include "btmmicm.h"
#include "bluetooth_struct.h"
#include "btmmiftpgprots.h"
#include "btmmiftpscr.h"
#include "btmmiftp.h"
extern mmi_ftps_conn_cntx_struct *act_server_cntx_p;
#endif
#endif
#include "phonebookdef.h"
volatile BOOL g_doFsProcess = 0;
#if (CSW_EXTENDED_API_PMD_USB == 0)
extern BOOL g_NeedReDetectUsb;
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */

extern void SendMessageForUSB(UINT32 action);
#include "bootup.h"
extern void BatteryIndicationPopup(U16 stringId);

//
// extern S8 nPrintableStr[];
//
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ 
U16 fmgr_download_move_sucess=FALSE;
void highlight_fmgr_activate_theme(void);
void fmgr_Activate_theme();
void fmgr_move_theme_file();
U8 mmi_activatetheme_frmfilemanager(PS8 path,PS8 filename);
void fmgr_display_themepopUp();
void fmgr_theme_move_rsp(void *);
    #define MMI_THEME_LOCATION  "THEMES/"

void fmgr_theme_single_delete();
void fmgr_display_theme_delte_popUp();
U16 is_mmi_theme_file_active(PS8 file_name);
void fmgr_theme_delete_highlight();
S16 get_storage_type(PS8 path);
U16 check_file_location(PS8 path);

void theme_folder_delete_rsp_hdlr(void *info);
S16 g_istheme_folder_created=FALSE;

//S8 gtheme_folder_path[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];
S8 gtheme_folder_path[(FMGR_MAX_FILE_NAME_LEN+1)*ENCODING_LENGTH];
//S8 gtest_theme_file[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];
S8 gtest_theme_file[(FMGR_MAX_FILE_NAME_LEN+1)*ENCODING_LENGTH];
#endif 

BOOL g_FsTaskIsRun = FALSE;
extern BOOL g_tflashCardExist;
extern BOOL g_hasMountedTFlashFS;
#if defined(DUAL_TFLASH_SUPPORT)
extern BOOL g_tflashCard2Exist;
extern BOOL g_hasMountedTFlash2FS;
#endif
#ifdef __MMI_SOUND_RECORDER__
extern U16 mmi_sndrec_get_menuId( void );
#endif // __MMI_SOUND_RECORDER__ 

/*wangr add  20070723 beigin, for fixbug 5587*/
#ifdef MJPG_SUPPORT
extern void mmi_imgview_view_detail_press_up(void);
extern void mmi_imgview_view_detail_press_down(void);
#endif
/*wangr add  20070723 end*/

S32 fmgr_get_application_highlight_idx(void );
void fmgr_set_application_highlight_idx(void );
void fmgr_reset_application_highlight_idx(void);

BOOL g_VM_Interrupt = FALSE;
extern void mmi_show_qq_select_confirm(void (*default_entry) (void));
extern BOOL checkVMOn(void);
extern VOID mmi_display_gprs_active(void) ;

/******************************************************************/
/*	Internal Variable */
/******************************************************************/
fmgr_context_struct         g_fmgr_context;
fmgr_context_struct         *fmgr_p         = &g_fmgr_context;

const fmgr_filter_struct fmgr_filter[FMGR_MAX_FILTER_COUNT] =
{
	{ FMGR_TYPE_BMP, "BMP"},
	{ FMGR_TYPE_JPG, "JPG"},
	{ FMGR_TYPE_JPEG,"JPEG"},       
	{ FMGR_TYPE_GIF, "GIF"},
	{ FMGR_TYPE_PNG, "PNG"},
	{ FMGR_TYPE_EMS, "EMS"},
	{ FMGR_TYPE_ANM, "ANM"},
	{ FMGR_TYPE_WBMP,"WBMP"},
	{ FMGR_TYPE_WBM, "WBM"},
#if 1//gdm
	{ FMGR_TYPE_PBM, "PBM"},
#endif

	{ FMGR_TYPE_IMY, "IMY"},
	{ FMGR_TYPE_MID, "MID"},
	{ FMGR_TYPE_MIDI, "MIDI"}, 
	{ FMGR_TYPE_WAV, "WAV"},
	{ FMGR_TYPE_AMR, "AMR"},
	{ FMGR_TYPE_AAC, "AAC"},
	{ FMGR_TYPE_DAF, "MP3"},
	{ FMGR_TYPE_VM,  "VM "},
	{ FMGR_TYPE_AWB, "AWB"},
	{ FMGR_TYPE_AIF, "AIF"},
	{ FMGR_TYPE_AIFF,"AIFF"},
	{ FMGR_TYPE_AIFC,"AIFC"},
	{ FMGR_TYPE_AU,  "AU "},
	{ FMGR_TYPE_SND, "SND"},                   
	{ FMGR_TYPE_M4A, "M4A"},                   
	{ FMGR_TYPE_MMF, "MMF"},
	{ FMGR_TYPE_WMA, "WMA"},

	{ FMGR_TYPE_3GP, "3GP"},
	{ FMGR_TYPE_MP4, "MP4"},
	{ FMGR_TYPE_AVI, "AVI"},   
	{ FMGR_TYPE_3G2, "3G2"},
	{ FMGR_TYPE_AVI, "MPEG"},  
	{ FMGR_TYPE_RM, "RM "}, 
	{ FMGR_TYPE_RMVB, "RMVB"},

	{ FMGR_TYPE_JAD, "JAD"},
	{ FMGR_TYPE_JAR, "JAR"},
	{ FMGR_TYPE_VCF, "VCF"},
	{ FMGR_TYPE_VCS, "VCS"},
	{FMGR_TYPE_THEME, "MED"},
	{FMGR_TYPE_EBOOK_TXT, "TXT"},
	{FMGR_TYPE_EBOOK_PDB, "PDB"},
	{FMGR_TYPE_DOC,"DOC"},
	{FMGR_TYPE_XLS,"XLS"},
	{FMGR_TYPE_PPT,"PPT"},
	{FMGR_TYPE_SMIL,"SMIL"},
	{FMGR_TYPE_PDF,"PDF"},
	{FMGR_TYPE_HTM,"HTM"},
	{FMGR_TYPE_TIFF,"TIFF"},
	{FMGR_TYPE_XML,"XML"},
	{FMGR_TYPE_QCP,"QCP"},
	{ FMGR_TYPE_CSV, "CSV"},


	/*
	“.txt",+
	".vcs",+
	".vcf",   +  
	".gif",++
	“.jpg",++
	".jpeg",++
	".png",++
	".bmp", ++
	".wbmp",++
	".amr",+
	".mid",+
	".midi",+
	".wav",+
	".mp3",+
	".mmf",+
	".imy",++
	".3gp",+
	".mp4",+
	".doc",
	".xls",
	".ppt",
	".smil",
	".pdf",
	".htm",
	".tiff",
	".xml",     
	".qcp",
	".3g2",
	".mpeg",


	*/

};


/************************************************************************
* Extern Functions
************************************************************************/

#ifdef __MMI_FILE_MANAGER__
/*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	mmi_fmgr_init
* DESCRIPTION
*	init FMGR highlight handlers
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_fmgr_init (void)
{
    /* drive options */
    SetHiliteHandler(MENU_FMGR_DRIVE_OPEN, highlight_fmgr_drive_open);
    SetHiliteHandler(MENU_FMGR_DRIVE_FORMAT, highlight_fmgr_format);
    SetHiliteHandler(MENU_FMGR_DRIVE_CHECK, highlight_fmgr_check_drive);   
    SetHiliteHandler(MENU_FMGR_DRIVE_UNMOUNT, highlight_fmgr_unmount);
    SetHiliteHandler(MENU_FMGR_FOLDER_CREATE, highlight_fmgr_create_folder);

    /* select options */
    SetHiliteHandler(MENU_FMGR_SELECT_DONE, highlight_fmgr_app_select);
    SetHiliteHandler(MENU_FMGR_TREE_OPEN, highlight_fmgr_tree_open);
    SetHiliteHandler(MENU_FMGR_TREE_SELECT, highlight_fmgr_tree_copy);

    /* folder options */
    SetHiliteHandler(MENU_FMGR_GEN_OPTION_OPEN, highlight_fmgr_open);
    SetHiliteHandler(MENU_FMGR_GEN_OPTION_RENAME, highlight_fmgr_rename);
    SetHiliteHandler(MENU_FMGR_GEN_OPTION_DELETE, highlight_fmgr_delete);
    SetHiliteHandler(MENU_FMGR_GEN_OPTION_SORT, highlight_fmgr_sort);

    /* file options */
    SetHiliteHandler(MENU_FMGR_IMG_OPTION_VIEW, highlight_fmgr_view_image);
    SetHiliteHandler(MENU_FMGR_IMG_OPTION_EDIT, highlight_fmgr_edit_image);
    SetHiliteHandler(MENU_FMGR_AUD_OPTION_PLAY, highlight_fmgr_play_audio);   
    SetHiliteHandler(MENU_FMGR_VID_OPTION_PLAY, highlight_fmgr_play_video);
    SetHiliteHandler(MENU_FMGR_EXE_OPTION_EXECUTE, highlight_fmgr_execute);
    SetHiliteHandler(MENU_FMGR_GEN_OPTION_FORWARD, highlight_fmgr_forward);
    SetHiliteHandler(MENU_FMGR_GEN_OPTION_DETAIL, highlight_fmgr_detail);
    SetHiliteHandler(MENU_FMGR_GEN_OPTION_COPY, highlight_fmgr_copy);
    SetHiliteHandler(MENU_FMGR_GEN_OPTION_MOVE, highlight_fmgr_move);
    SetHiliteHandler(MENU_FMGR_GEN_OPTION_DELETE_ALL, highlight_fmgr_delete_all_file);

    /* sort options */
    SetHiliteHandler(MENU_FMGR_SORT_BY_NAME, highlight_fmgr_sort_by_name);
    SetHiliteHandler(MENU_FMGR_SORT_BY_TYPE, highlight_fmgr_sort_by_type);
    SetHiliteHandler(MENU_FMGR_SORT_BY_TIME, highlight_fmgr_sort_by_time);
    SetHiliteHandler(MENU_FMGR_SORT_BY_SIZE, highlight_fmgr_sort_by_size);
    SetHiliteHandler(MENU_FMGR_SORT_NONE, highlight_fmgr_sort_none);

  
    
    /* fwd options */
    SetHiliteHandler(MENU_FMGR_FWD_WALLPAPER, highlight_fmgr_to_wallpaper);
    SetHiliteHandler(MENU_FMGR_FWD_SUB_WALLPAPER, highlight_fmgr_to_sub_wallpaper);   
    SetHiliteHandler(MENU_FMGR_FWD_SCREENSAVER, highlight_fmgr_to_screensaver);
    SetHiliteHandler(MENU_FMGR_FWD_POWER_ON, highlight_fmgr_to_power_on_display);
    SetHiliteHandler(MENU_FMGR_FWD_POWER_OFF, highlight_fmgr_to_power_off_display);   
    SetHiliteHandler(MENU_FMGR_FWD_PHB, highlight_fmgr_to_phb);
    SetHiliteHandler(MENU_FMGR_FWD_TODOLIST, highlight_fmgr_to_todolist);
    SetHiliteHandler(MENU_FMGR_FWD_SMS, highlight_fmgr_to_sms);
    SetHiliteHandler(MENU_FMGR_FWD_MMS, highlight_fmgr_to_mms);
    SetHiliteHandler(MENU_FMGR_FWD_EMS, highlight_fmgr_to_ems);
    SetHiliteHandler(MENU_FMGR_FWD_EMAIL, highlight_fmgr_to_email);
    SetHiliteHandler(MENU_FMGR_FWD_IRDA, highlight_fmgr_to_irda);
    SetHiliteHandler(MENU_FMGR_FWD_PROFILE, highlight_fmgr_to_profile);
#if defined( __MMI_ACCEL_OPP_SUPPORT__) ||	defined( __MMI_OPP_SUPPORT__)
    SetHiliteHandler(MENU_FMGR_FWD_BT, highlight_fmgr_to_bt);
    SetHiliteHandler(MENU_FMGR_FWD_PROFILE, highlight_fmgr_to_profile);
#endif
    SetHiliteHandler(MENU_FMGR_INPUT_METHOD, highlight_fmgr_input_method);
 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
    SetHiliteHandler(MENU_FMGR_ACTIVATE_THEME, highlight_fmgr_activate_theme);
    SetHiliteHandler(MENU_FMGR_THEME_DELETE, fmgr_theme_delete_highlight);
#endif

#ifdef __MMI_EBOOK_READER__
    SetHiliteHandler(MENU_FMGR_EBK_OPTION_OPEN, highlight_fmgr_open_ebook);
#endif 

#if defined (MMI_ON_HARDWARE_P) && defined (__FS_CARD_SUPPORT__)
    SetProtocolEventHandler(fmgr_msdc_plug_in_hdlr, PRT_MMI_FMGR_CARD_PLUG_IN_IND);
    SetProtocolEventHandler(fmgr_msdc_plug_out_hdlr, PRT_MMI_FMGR_CARD_PLUG_OUT_IND);
#endif   

    fmgr_init_context();
}

/*****************************************************************************
* FUNCTION
*	fmgr_init_context
* DESCRIPTION
*	init FMGR context
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmgr_init_context (void)
{
#ifdef __DRM_SUPPORT__ 
    S32 attr;
#endif
    S16 error;
#ifdef MMI_ON_HARDWARE_P
    S8 drive;
#endif
    U8 temp;

    TBM_ENTRY(0x2847);
    TRACE_EF(g_sw_FMGR," into fmgr_init_context:fmgr_p->drv_count = %d",fmgr_p->drv_count);
    /* init global variable */
    memset (fmgr_p, 0, sizeof (fmgr_context_struct));
    fmgr_p->src_application         = APP_FILEMANAGER; /* idle screen and the menu one
                      level upper than FMGR need to
                      reset fmgr_p->src_application */
    ReadValue(NVRAM_EF_FMGR_SORT_OPTION_LID, &temp, DS_BYTE, &error);
    if ( temp == 0xff )
        fmgr_p->sort_option = FS_NO_SORT;
    else
        fmgr_p->sort_option = temp << 5;

#ifndef MMI_ON_HARDWARE_P
    /* on PC */
    fmgr_p->drv_count=2;
    strcpy ((PS8)fmgr_p->drv_list[0], (PS8)"C:/");
    strcpy ((PS8)fmgr_p->drv_list[1], (PS8)"E:/");
    TRACE_EF(g_sw_FMGR," into no MMI_ON_HARDWARE_P :fmgr_p->drv_count = %d",fmgr_p->drv_count);
#else
    /* on target */
#if 0 /* wufasong deleted 2007.03.29 since Wuzhicheng had deleted the storage type : FMGR_NAND_STORAGE. */
    if ( ((drive = MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_NO_ALT_DRIVE)) >= 'A') &&
    (drive <= 'Z') )
    {
        /* Normal 1 existed */
        fmgr_p->drv_count++;
        sprintf ((PS8)fmgr_p->drv_list[FMGR_NAND_STORAGE], "%c:/", drive);
    }
#endif
    if ( ((drive = MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 1, FS_NO_ALT_DRIVE)) >= 'A') &&
    (drive <= 'Z') )
    {
        /* Normal 2 existed */
        fmgr_p->drv_count++;
        sprintf ((PS8)fmgr_p->drv_list[FMGR_NOR_STORAGE], "%c:/", drive);
    }

    TRACE_EF(g_sw_FMGR," into fmgr_init_context:   fmgr_p->drv_count = %d,drive=%c:",fmgr_p->drv_count, drive);
    
#ifdef __DRM_SUPPORT__ 
    MMI_FS_CreateDir(DRM_NOR_PATH);

    MMI_FS_CreateDir(DRM_RO_PATH);
    attr = MMI_FS_GetAttributes(DRM_RO_PATH);
    if ( attr >= FS_NO_ERROR )
        MMI_FS_SetAttributes(DRM_RO_PATH, (U8)(attr | FS_ATTR_HIDDEN));

    MMI_FS_XDelete(DRM_TEMP_DECRYPTED_FOLDER, FS_FILE_TYPE | FS_DIR_TYPE | FS_RECURSIVE_TYPE);
    MMI_FS_CreateDir(DRM_TEMP_DECRYPTED_FOLDER);
    attr = MMI_FS_GetAttributes(DRM_TEMP_DECRYPTED_FOLDER);
    if ( attr >= FS_NO_ERROR )
        MMI_FS_SetAttributes(DRM_TEMP_DECRYPTED_FOLDER, (U8)(attr | FS_ATTR_HIDDEN));

#ifdef __DRM_V01__
    fmgr_p->drv_count ++;
    UnicodeToAnsii((PS8)fmgr_p->drv_list[FMGR_DRM_STORAGE], (PS8) DRM_NOR_PATH);
#endif /*__DRM_V01__*/	
    fmgr_p->drv_count ++;
    UnicodeToAnsii((PS8)fmgr_p->drv_list[FMGR_DRM_RO_STORAGE], (PS8) DRM_RO_PATH);   
#endif /*__DRM_SUPPORT__*/

#if defined(DUAL_TFLASH_SUPPORT)
    if (g_tflashCard2Exist == TRUE) 
    {
        drive = 'F';
        fmgr_p->drv_count++;
        sprintf ((PS8)fmgr_p->drv_list[FMGR_STORAGE_TFCARD_SECOND], "%c:/", drive);
    }
#endif

    if ( ((drive = MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE)) >= 'A') &&
    (drive <= 'Z') )
    {
        fmgr_p->drv_count++;
        sprintf ((PS8)fmgr_p->drv_list[FMGR_REMOVABLE_STORAGE], "%c:/", drive);

        
        TRACE_EF(g_sw_FMGR," into fmgr_init_context:   fmgr_p->drv_count = %d,drive=%c:",fmgr_p->drv_count, drive);
        return ;
    }
    
    //if ((g_tflashCardExist == TRUE) && (g_hasMountedTFlashFS == FALSE))
    if (g_tflashCardExist == TRUE) 
    {
        drive = 'D';
        fmgr_p->drv_count++;
        sprintf ((PS8)fmgr_p->drv_list[FMGR_REMOVABLE_STORAGE], "%c:/", drive);
    }
#endif
    mmi_trace(g_sw_FMGR," exit fmgr_init_context:fmgr_p->drv_count = %d",fmgr_p->drv_count);
    TBM_EXIT(0x2847);
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_drive_list
* DESCRIPTION
*	highlight handler for drive
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_drive_list(S32 hiliteID)
{
    S16 index;
    U8 sel_type;

    TRACE_FMGR_FUNCTION();
    fmgr_p->cur_index = hiliteID;
    index = fmgr_get_drive_index (hiliteID);
    TRACE_EF(g_sw_FMGR, "fmgr_p->file_path = %s, hiliteID = %d,index = %d.", fmgr_p->file_path, hiliteID,index);
    if ( index >= 0 )
    {
        AnsiiToUnicodeString((PS8)fmgr_p->file_path, (PS8)fmgr_p->drv_list[index]);
    }



    if ( fmgr_p->src_application == APP_FILEMANAGER )
    {
        TRACE_FMGR_FUNC("(fmgr_p->src_application == APP_FILEMANAGER)");
        TRACE_EF(g_sw_FMGR, "fmgr_p->tree_action = %d", fmgr_p->tree_action);
        if ( fmgr_p->tree_action == FMGR_ACTION_COPY ||
        fmgr_p->tree_action == FMGR_ACTION_MOVE )
        {
#ifdef __DRM_SUPPORT__   	
            if ( index == FMGR_DRM_RO_STORAGE )
            {
                ChangeLeftSoftkey(0, 0);
                ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
                SetRightSoftkeyFunction(fmgr_abort_copy_select, KEY_EVENT_UP);      
                 
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_reset_list_item_selected_callback();
#endif
                 
            }
            else
#endif
#ifdef __DRM_V01__
                if ( index == FMGR_DRM_STORAGE )
            {
                ChangeLeftSoftkey(STR_GLOBAL_DONE, 0);
                ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                SetLeftSoftkeyFunction(fmgr_copy, KEY_EVENT_UP);
                SetRightSoftkeyFunction(fmgr_abort_copy_select, KEY_EVENT_UP);      
                 
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_reset_list_item_selected_callback();
#endif
                 
            }
            else
#endif
            {
                ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);
                SetKeyHandler(entry_fmgr_tree_explorer, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                SetLeftSoftkeyFunction(entry_fmgr_drive_option, KEY_EVENT_UP);
                SetRightSoftkeyFunction(fmgr_abort_copy_select, KEY_EVENT_UP);
                 
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_register_list_item_selected_callback(entry_fmgr_tree_explorer);
#endif
                 
            }
 			ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
        }
        else
        {
#ifdef __DRM_SUPPORT__
            if ( index == FMGR_DRM_RO_STORAGE 
#ifdef __DRM_V01__
            || index == FMGR_DRM_STORAGE
#endif            
            )
            {
                ChangeLeftSoftkey(STR_FMGR_OPEN, 0);
                SetKeyHandler(entry_fmgr_explorer, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                SetLeftSoftkeyFunction(entry_fmgr_explorer, KEY_EVENT_UP);
                SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);       
                 
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_register_list_item_selected_callback(entry_fmgr_explorer);
#endif
                 
            }
            else
#endif
            {
                ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);
                SetKeyHandler(entry_fmgr_explorer, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                SetLeftSoftkeyFunction(entry_fmgr_drive_option, KEY_EVENT_UP);
                SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);//Bring the dpad-left key into effect 090911 gdm
                SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
                 
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_register_list_item_selected_callback(entry_fmgr_explorer);
#endif
                 
            }
        }
        //ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN); //Bring the dpad-left key into effect 090911 gdm
        return;
    }

#ifdef __DRM_SUPPORT__
    if ( index == FMGR_DRM_RO_STORAGE 
#ifdef __DRM_V01__
    || index == FMGR_DRM_STORAGE
#endif      
    )
    {
        /* not FMGR, can't enter DRM drive */
        ChangeLeftSoftkey(0, 0);
        ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
        ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
        ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
        SetRightSoftkeyFunction(fmgr_abort_app_select, KEY_EVENT_UP);
         
#ifdef __MMI_TOUCH_SCREEN__
        mmi_wgui_reset_list_item_selected_callback();
#endif
         
        return;
    }
#endif

    sel_type = fmgr_p->sel_type & 0x0f;
    switch ( sel_type )
    {
        case FMGR_SELECT_FILE:
            ChangeLeftSoftkey(STR_FMGR_OPEN, 0);
            SetKeyHandler(entry_fmgr_explorer, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
            SetLeftSoftkeyFunction(entry_fmgr_explorer, KEY_EVENT_UP);
             
#ifdef __MMI_TOUCH_SCREEN__
            mmi_wgui_register_list_item_selected_callback(entry_fmgr_explorer);
#endif
             
            break;
        case FMGR_SELECT_FILE_OR_FOLDER:
        case FMGR_SELECT_FOLDER:
            ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);
            SetKeyHandler(entry_fmgr_explorer, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
            SetLeftSoftkeyFunction(entry_fmgr_drive_option, KEY_EVENT_UP);
             
#ifdef __MMI_TOUCH_SCREEN__
            mmi_wgui_register_list_item_selected_callback(entry_fmgr_explorer);
#endif
             
            break;
        case FMGR_SELECT_BROWSE:
            ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);
            SetKeyHandler(entry_fmgr_explorer, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
            SetLeftSoftkeyFunction(entry_fmgr_app_drive_option, KEY_EVENT_UP);
             
#ifdef __MMI_TOUCH_SCREEN__
            mmi_wgui_register_list_item_selected_callback(entry_fmgr_explorer);
#endif
             
            break;
    }
    ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(fmgr_abort_app_select, KEY_EVENT_UP);
}
#if defined( __MMI_ACCEL_OPP_SUPPORT__) ||	defined( __MMI_OPP_SUPPORT__)
/*****************************************************************************
 * FUNCTION
 *  highlight_fmgr_to_bt
 * DESCRIPTION
 *  highlight handler for forward to irda
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void highlight_fmgr_to_bt(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    fmgr_p->tree_action = FMGR_ACTION_FWD_BT;
    SetLeftSoftkeyFunction(fmgr_forward_to_app, KEY_EVENT_UP);
}
#endif
/*****************************************************************************
* FUNCTION
*	fmgr_get_drive_index
* DESCRIPTION
*	map highlight id and drive index
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
S8 fmgr_get_drive_index(S32 hiliteID)
{
    U8 i=0, count=0;

    for ( i=0; i<FMGR_TOTAL_STORAGE; i++ )
    {
        if ( fmgr_p->drv_list[i][0] != 0 )
            count++;
        if ( count == hiliteID+1 )
            break;
    }

    if ( i==FMGR_TOTAL_STORAGE )
        return(S8)-1;

    return i;
}

/*****************************************************************************
* FUNCTION
*	fmgr_return_to_editor
* DESCRIPTION
*	
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmgr_return_to_editor(void)
{
    GoBacknHistory(1);
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_drive_open
* DESCRIPTION
*	highlight handler for open drive
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_drive_open(void)
{
    TRACE_FMGR_FUNCTION();
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( entry_fmgr_explorer, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_format
* DESCRIPTION
*	highlight handler for format drive
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_format (void)
{
    TRACE_AUD_FUNCTION();
    fmgr_p->drive_action = FMGR_ACTION_FORMAT;
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( entry_fmgr_drive_confirm, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_check_drive
* DESCRIPTION
*	highlight handler for scan drive
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_check_drive (void)
{
    fmgr_p->drive_action = FMGR_ACTION_CHECK_DRIVE;
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( entry_fmgr_drive_confirm, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_unmount
* DESCRIPTION
*	highlight handler for unmount card
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_unmount (void)
{
    fmgr_p->drive_action = FMGR_ACTION_UNMOUNT;
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( entry_fmgr_drive_confirm, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_node
* DESCRIPTION
*	highlight handler for file or folder
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_node(S32 index)
{
    FMGR_FILE_INFO_STRUCT file_info;
    U8 sel_type;

    TRACE_FMGR_FUNCTION();
    mmi_fmgr_kernel_get_file_info(index, &file_info);
    fmgr_p->cur_index = index;

    if ( fmgr_p->src_application == APP_FILEMANAGER )
    {
#ifdef __DRM_SUPPORT__
        if ( pfnUnicodeStrncmp ((PS8)fmgr_p->file_path, (PS8)DRM_RO_PATH, 
        pfnUnicodeStrlen((PS8)DRM_RO_PATH)) == 0 )
        {
            /* DRM rights, back and left arrow only go back to drive list */
            ChangeLeftSoftkey(STR_GLOBAL_DELETE, 0);
            ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
            SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
            SetLeftSoftkeyFunction(entry_fmgr_delete_confirm, KEY_EVENT_UP);
            SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
             
#ifdef __MMI_TOUCH_SCREEN__
            mmi_wgui_reset_list_item_selected_callback();
#endif
             
            return;
        }
        else
#endif	
#ifdef __DRM_V01__
            if ( pfnUnicodeStrncmp ((PS8)fmgr_p->file_path, (PS8)DRM_NOR_PATH, 
        pfnUnicodeStrlen((PS8)DRM_NOR_PATH)) == 0 )
        {
            /* DRM folder, back and left arrow only go back to drive list */
            ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
            SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
            SetLeftSoftkeyFunction(entry_fmgr_file_option, KEY_EVENT_UP);
            SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
             
#ifdef __MMI_TOUCH_SCREEN__
            mmi_wgui_reset_list_item_selected_callback();
#endif
             
            return;
        }
        else
#endif
        {
            if ( file_info.attribute & FS_ATTR_DIR )
            {
                /* folder node */
                SetKeyHandler(fmgr_set_curr_path_and_enter, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                SetLeftSoftkeyFunction(entry_fmgr_folder_option, KEY_EVENT_UP);
                 
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_register_list_item_selected_callback(fmgr_set_curr_path_and_enter);
#endif
                 
            }
            else
            {
                /* file node */
                ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                SetLeftSoftkeyFunction(entry_fmgr_file_option, KEY_EVENT_UP);
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_reset_list_item_selected_callback();
#endif
            }
            SetKeyHandler(fmgr_back_to_parent, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
            SetRightSoftkeyFunction(fmgr_back_to_parent, KEY_EVENT_UP);
        }
        return;
    }

    /* for other applications */
    sel_type = fmgr_p->sel_type & 0x0f;
    switch ( sel_type )
    {
        case FMGR_SELECT_FILE:
            /* select file */
            if ( file_info.attribute & FS_ATTR_DIR )
            {
                ChangeLeftSoftkey(STR_FMGR_OPEN, 0);
                SetKeyHandler(fmgr_set_curr_path_and_enter, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                SetLeftSoftkeyFunction(fmgr_set_curr_path_and_enter, KEY_EVENT_UP);
                 
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_register_list_item_selected_callback(fmgr_set_curr_path_and_enter);
#endif
                 
            }
            else
            {
                ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
                ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                SetLeftSoftkeyFunction(fmgr_return_to_app, KEY_EVENT_UP);
                 
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_reset_list_item_selected_callback();
#endif
                 
            }

            SetKeyHandler(fmgr_back_to_parent, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
            SetRightSoftkeyFunction(fmgr_back_to_parent, KEY_EVENT_UP);
            break;

        case FMGR_SELECT_FOLDER:
            /* select folder to save file */
            if ( file_info.attribute & FS_ATTR_DIR )
            {
                ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);
                SetKeyHandler(fmgr_set_curr_path_and_enter, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                SetLeftSoftkeyFunction(entry_fmgr_folder_option, KEY_EVENT_UP);
                 
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_register_list_item_selected_callback(fmgr_set_curr_path_and_enter);
#endif
                 
            }

            SetKeyHandler(fmgr_back_to_parent, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
            SetRightSoftkeyFunction(fmgr_abort_app_select, KEY_EVENT_UP);
            break;

        case FMGR_SELECT_FILE_OR_FOLDER:
            /* select file or folder */
            if ( file_info.attribute & FS_ATTR_DIR )
            {
                /* folder node */
				
                ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);
                SetKeyHandler(fmgr_set_curr_path_and_enter, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                SetLeftSoftkeyFunction(entry_fmgr_folder_option, KEY_EVENT_UP);
                 
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_register_list_item_selected_callback(fmgr_set_curr_path_and_enter);
#endif
                 
            }
            else
            {
				
                /* file node */
                ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
                ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
                SetLeftSoftkeyFunction(fmgr_return_to_app, KEY_EVENT_UP);
                 
#ifdef __MMI_TOUCH_SCREEN__
                mmi_wgui_reset_list_item_selected_callback();
#endif
                 
            }

            SetKeyHandler(fmgr_back_to_parent, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
            SetRightSoftkeyFunction(fmgr_abort_app_select, KEY_EVENT_UP);
            break;

        case FMGR_SELECT_BROWSE:
            /* browse file */
            if ( file_info.attribute & FS_ATTR_DIR )
            {
                /* folder node */
                ChangeLeftSoftkey(0, 0);
                //SetLeftSoftkeyFunction(entry_fmgr_app_folder_option, KEY_EVENT_UP);
            }
            else
            {
                /* file node */
                ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);
                SetLeftSoftkeyFunction(entry_fmgr_app_file_option, KEY_EVENT_UP);
	                if ( FMGR_FILTER_IS_SET_IMAGE(&file_info.file_type) )
	                {
           #ifdef __MMI_CAMERA__
               #if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
	                    SetKeyHandler(mmi_imageviewer_entry_app_screen, KEY_ENTER, KEY_EVENT_UP);
               #else
	                    SetKeyHandler(mmi_imgview_entry_view_detail_screen, KEY_ENTER, KEY_EVENT_UP);
              #endif
          #endif
	                }
			else 
			{
				ClearKeyHandler(KEY_ENTER, KEY_EVENT_UP);	
			}
            }

            ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
            ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
            SetRightSoftkeyFunction(fmgr_abort_app_select, KEY_EVENT_UP);
#ifdef __MMI_TOUCH_SCREEN__
            mmi_wgui_reset_list_item_selected_callback();
#endif
            break;

    }
}


/*****************************************************************************
* FUNCTION
*	mmi_fmgr_highlight_matrix_node
* DESCRIPTION
*	highlight matrix node
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_fmgr_highlight_matrix_node(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/ 
    FMGR_FILE_INFO_STRUCT file_info;
    U8 sel_type;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_fmgr_kernel_get_file_info(index, &file_info);
    fmgr_p->cur_index = index;

    /* for other applications */
    sel_type = fmgr_p->sel_type & 0x0f;
    switch ( sel_type )
    {
        
        case FMGR_SELECT_FILE:
            /* select file */
            if ( file_info.attribute & FS_ATTR_DIR )
            {
                ChangeLeftSoftkey(STR_FMGR_OPEN, 0);
                SetLeftSoftkeyFunction(fmgr_set_curr_path_and_enter, KEY_EVENT_UP);
            }
            else
            {
                ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
                SetLeftSoftkeyFunction(fmgr_return_to_app, KEY_EVENT_UP);
            }

            SetRightSoftkeyFunction(fmgr_abort_app_select, KEY_EVENT_UP);
            break;


        case FMGR_SELECT_FOLDER:
            /* select folder to save file */
            if ( file_info.attribute & FS_ATTR_DIR )
            {
                ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);
                SetLeftSoftkeyFunction(entry_fmgr_folder_option, KEY_EVENT_UP);
            }

            SetRightSoftkeyFunction(fmgr_abort_app_select, KEY_EVENT_UP);
            break;

        case FMGR_SELECT_FILE_OR_FOLDER:
            /* select file or folder */
            if ( file_info.attribute & FS_ATTR_DIR )
            {
                ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);
                SetLeftSoftkeyFunction(entry_fmgr_folder_option, KEY_EVENT_UP);
            }
            else
            {
                ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
                SetLeftSoftkeyFunction(fmgr_return_to_app, KEY_EVENT_UP);
            }
            SetRightSoftkeyFunction(fmgr_abort_app_select, KEY_EVENT_UP);
            break;

        case FMGR_SELECT_BROWSE:
            /* browse file */
            if ( file_info.attribute & FS_ATTR_DIR )
            {
                ChangeLeftSoftkey(0, 0);
            }
            else
            {
                ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);
                SetLeftSoftkeyFunction(entry_fmgr_app_file_option, KEY_EVENT_UP);

                if ( FMGR_FILTER_IS_SET_IMAGE(&file_info.file_type) )
                {
#ifdef __MMI_CAMERA__
#if defined(__CAMERA_OSD_HORIZONTAL__) || defined(__HORIZONTAL_CAMERA_WALLPAPER_ENABLE__)
                    SetKeyHandler(mmi_imageviewer_entry_app_screen, KEY_ENTER, KEY_EVENT_UP);
#else
                    SetKeyHandler(mmi_imgview_entry_view_detail_screen, KEY_ENTER, KEY_EVENT_UP);
#endif
#endif
                }
            }
            SetRightSoftkeyFunction(fmgr_abort_app_select, KEY_EVENT_UP);
            break;
    }
}



/*****************************************************************************
* FUNCTION
*	highlight_fmgr_tree_node
* DESCRIPTION
*	highlight handler for folder when select destination for copy or move
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_tree_node(S32 index)
{
    fmgr_p->cur_index = index;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_tree_open
* DESCRIPTION
*	highlight handler for open folder in tree explorer
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_tree_open(void)
{
    TRACE_FMGR_FUNCTION();
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(fmgr_set_curr_tree_and_enter, KEY_EVENT_UP);
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_open
* DESCRIPTION
*	highlight handler for open folder
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_open (void)
{
    TRACE_FMGR_FUNCTION();
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(fmgr_set_curr_path_and_enter, KEY_EVENT_UP);
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_create_folder
* DESCRIPTION
*	highlight handler for create folder
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_create_folder(void)
{
    TRACE_FMGR_FUNCTION();
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_create_folder_hdlr, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_app_select
* DESCRIPTION
*	highlight handler for select folder by app
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_app_select(void)
{
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_return_to_app, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_folder_done
* DESCRIPTION
*	highlight handler for confirm create folder
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_folder_done(void)
{
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_create_folder, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_view_image
* DESCRIPTION
*	highlight handler for view image
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_view_image(void)
{
    SetKeyHandler(fmgr_view_image_hdlr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_view_image_hdlr, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_register_list_item_selected_callback(fmgr_view_image_hdlr);
#endif

}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_edit_image
* DESCRIPTION
*	highlight handler for edit image
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_edit_image(void)
{
    SetKeyHandler(fmgr_edit_image_hdlr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_edit_image_hdlr, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_register_list_item_selected_callback(fmgr_edit_image_hdlr);
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_play_audio
* DESCRIPTION
*	highlight handler for play audio
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_play_audio(void)
{
    SetKeyHandler(fmgr_play_audio_hdlr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_play_audio_hdlr, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_register_list_item_selected_callback(fmgr_play_audio_hdlr);
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_play_video
* DESCRIPTION
*	highlight handler for play video
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_play_video(void)
{
    SetKeyHandler(fmgr_play_video_hdlr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_play_video_hdlr, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_register_list_item_selected_callback(fmgr_play_video_hdlr);
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_execute
* DESCRIPTION
*	highlight handler for install java
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_execute(void)
{
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_execute_hdlr, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_sort
* DESCRIPTION
*	highlight handler for sort
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_sort(void)
{
    TRACE_FMGR_FUNCTION();
    SetKeyHandler(entry_fmgr_sort_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( entry_fmgr_sort_option, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_register_list_item_selected_callback(entry_fmgr_sort_option);
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_forward
* DESCRIPTION
*	highlight handler for forward file
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_forward(void)
{
    SetKeyHandler(entry_fmgr_forward_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( entry_fmgr_forward_option, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_register_list_item_selected_callback(entry_fmgr_forward_option);
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_detail
* DESCRIPTION
*	highlight handler for display detail
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_detail (void)
{
    TRACE_FMGR_FUNCTION();
    SetKeyHandler(entry_fmgr_detail, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( entry_fmgr_detail, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_register_list_item_selected_callback(entry_fmgr_detail);
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_rename
* DESCRIPTION
*	highlight handler for rename file or folder
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_rename(void)
{
    TRACE_FMGR_FUNCTION();
    SetKeyHandler(fmgr_rename_hdlr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_rename_hdlr, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_register_list_item_selected_callback(fmgr_rename_hdlr);
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_rename_done
* DESCRIPTION
*	highlight handler for confrim rename
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_rename_done(void)
{
    TRACE_FMGR_FUNCTION();
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_rename, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_delete
* DESCRIPTION
*	highlight handler for delete file or folder
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_delete(void)
{
    TRACE_FMGR_FUNCTION();
    fmgr_p->delete_all = FMGR_DELETE_SINGLE;
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( entry_fmgr_delete_confirm, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_delete_all_file
* DESCRIPTION
*	highlight handler for delete all files
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_delete_all_file(void)
{
    fmgr_p->delete_all = FMGR_DELETE_ALL;
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( entry_fmgr_delete_confirm, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_copy
* DESCRIPTION
*	highlight handler for copy file
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_copy(void)
{
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_copy_hdlr, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_move
* DESCRIPTION
*	highlight handler for move file
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_move(void)
{
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_move_hdlr, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_tree_copy
* DESCRIPTION
*	highlight handler for confrim copy to a selected destination folder
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_tree_copy (void)
{
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_copy, KEY_EVENT_UP );
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_sort_by_name
* DESCRIPTION
*	highlight handler for sort by name
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_sort_by_name(void)
{
    TRACE_FMGR_FUNCTION();
    fmgr_p->sort_option = FS_SORT_NAME;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_sort_by_type
* DESCRIPTION
*	highlight handler for sort by type
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_sort_by_type(void)
{
    TRACE_FMGR_FUNCTION();
    fmgr_p->sort_option = FS_SORT_TYPE;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_sort_by_time
* DESCRIPTION
*	highlight handler for sort by time
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_sort_by_time(void)
{
    TRACE_FMGR_FUNCTION();
    fmgr_p->sort_option = FS_SORT_TIME;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_sort_by_size
* DESCRIPTION
*	highlight handler for sort by size
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_sort_by_size(void)
{
    TRACE_FMGR_FUNCTION();
    fmgr_p->sort_option = FS_SORT_SIZE;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_sort_none
* DESCRIPTION
*	highlight handler for no sort
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_sort_none(void)
{
    TRACE_FMGR_FUNCTION();
    fmgr_p->sort_option = FS_NO_SORT;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_wallpaper
* DESCRIPTION
*	highlight handler for forward to wallpaper
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_wallpaper(void)
{
    TRACE_FMGR_FUNCTION();
    fmgr_p->tree_action = FMGR_ACTION_FWD_WALLPAPER;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_sub_wallpaper
* DESCRIPTION
*	highlight handler for forward to wallpaper
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_sub_wallpaper(void)
{
    fmgr_p->tree_action = FMGR_ACTION_FWD_SUB_WALLPAPER;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_screensaver
* DESCRIPTION
*	highlight handler for forward to screen saver
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_screensaver(void)
{
    TRACE_FMGR_FUNCTION();

    fmgr_p->tree_action = FMGR_ACTION_FWD_SCREENSAVER;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_power_on_display
* DESCRIPTION
*	highlight handler for forward to screen saver
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_power_on_display(void)
{
    TRACE_FMGR_FUNCTION();
    fmgr_p->tree_action = FMGR_ACTION_FWD_POWER_ON;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_power_off_display
* DESCRIPTION
*	highlight handler for forward to screen saver
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_power_off_display(void)
{
    TRACE_FMGR_FUNCTION();
    fmgr_p->tree_action = FMGR_ACTION_FWD_POWER_OFF;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_phb
* DESCRIPTION
*	highlight handler for forward to phonebook
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_phb(void)
{
    TRACE_FMGR_FUNCTION();
    fmgr_p->tree_action = FMGR_ACTION_FWD_PHONEBOOK;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_todolist
* DESCRIPTION
*	highlight handler for forward to to do list
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_todolist(void)
{
    fmgr_p->tree_action = FMGR_ACTION_FWD_TODOLIST;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_sms
* DESCRIPTION
*	highlight handler for forward to sms
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_sms(void)
{
    fmgr_p->tree_action = FMGR_ACTION_FWD_SMS;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_ems
* DESCRIPTION
*	highlight handler for forward to ems
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_ems(void)
{
    fmgr_p->tree_action = FMGR_ACTION_FWD_EMS;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_mms
* DESCRIPTION
*	highlight handler for forward to mms
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_mms(void)
{
    fmgr_p->tree_action = FMGR_ACTION_FWD_MMS;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_email
* DESCRIPTION
*	highlight handler for forward to email
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_email(void)
{
    fmgr_p->tree_action = FMGR_ACTION_FWD_EMAIL;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_irda
* DESCRIPTION
*	highlight handler for forward to irda
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_irda(void)
{
    fmgr_p->tree_action = FMGR_ACTION_FWD_IRDA;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_to_profile
* DESCRIPTION
*	highlight handler for forward to profile
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_to_profile(void)
{
    fmgr_p->tree_action = FMGR_ACTION_FWD_PROFILE;
}

/*****************************************************************************
* FUNCTION
*	highlight_fmgr_input_method
* DESCRIPTION
*	highlight handler for change input method in editor option
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void highlight_fmgr_input_method (void)
{
    SetInputMethodAndDoneCaptionIcon(IMG_SCR_PBOOK_CAPTION);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP);
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
}

    #if defined (MMI_ON_HARDWARE_P) && defined (__FS_CARD_SUPPORT__)
/*****************************************************************************
* FUNCTION
*	fmgr_msdc_plug_in_hdlr
* DESCRIPTION
*	msdc plug in message handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmgr_msdc_plug_in_hdlr (void *info)
{
    fmt_mmi_card_plug_in_ind_struct *msg_ptr;
    //Lisen 0623
    mmi_fmt_get_msdc_status_rsp_strcut card_status; 

    msg_ptr = (fmt_mmi_card_plug_in_ind_struct *)info;

    //Lisen 0623
    card_status.present = MMI_TRUE;
    card_status.type = msg_ptr->type;
    FM_MemoryCardResponseHandler(&card_status);

    if ( !IdleIsPowerOnPeriod() )
    {
        TurnOnBacklight(1);
        DisplayPopup((PU8)GetString(STR_FMGR_REMOVABLE_PLUGIN), IMG_GLOBAL_INFO, 0, 1000, WARNING_TONE);
#ifdef __MMI_AUDIO_PLAYER__
        mmi_audply_msdc_plug_in_hdlr(TRUE);
#endif
        return;
    }
#ifdef __MMI_AUDIO_PLAYER__
    mmi_audply_msdc_plug_in_hdlr(FALSE);
#endif
}

/*****************************************************************************
* FUNCTION
*	fmgr_msdc_plug_out_hdlr
* DESCRIPTION
*	msdc plug out message handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmgr_msdc_plug_out_hdlr(void *info)
{
    //Lisen 0623
    mmi_fmt_get_msdc_status_rsp_strcut card_status; 
    U16 marker_screen=0, start_screen=0, end_screen=0;

    //Lisen 0623
    card_status.present = MMI_FALSE;
    FM_MemoryCardResponseHandler(&card_status);

    if ( !IdleIsPowerOnPeriod() )
    {
        U32 strId;
        fmt_mmi_msdc_ind_struct *msg_ptr;

        TurnOnBacklight(1);
        msg_ptr = (fmt_mmi_msdc_ind_struct*)info;
        if ( msg_ptr->cause == FS_NO_ERROR || msg_ptr->cause == FS_MSDC_NOT_PRESENT )
            strId = STR_FMGR_REMOVABLE_PLUGOUT;
        else
            strId = GetFileSystemErrorString(msg_ptr->cause);

        DisplayPopup((PU8)GetString(STR_FMGR_REMOVABLE_PLUGOUT), IMG_GLOBAL_INFO, 0, 1000, WARNING_TONE);
#ifdef __MMI_AUDIO_PLAYER__
        mmi_audply_msdc_plug_out_hdlr(TRUE);
#endif
    }
    else
    {
#ifdef __MMI_AUDIO_PLAYER__
        mmi_audply_msdc_plug_out_hdlr(FALSE);
#endif
    }

    if ( isInCall() == TRUE )
        marker_screen = GetCmMarkerScrnID();
    else if ( AlmIsTonePlaying() == TRUE )
    {
#if defined(__MMI_TODOLIST__)
        if ( IsScreenPresent(SCR_TDL_REMINDER) )
            marker_screen = SCR_TDL_REMINDER;
        else
            marker_screen = SCR_ID_ALM_INDICATION;
#else
        marker_screen = SCR_ID_ALM_INDICATION;
#endif /* __MMI_TODOLIST__ */
    }
    if ( marker_screen != 0 )
        GetPreviousScrnIdOf(marker_screen, &end_screen);

    switch ( fmgr_p->src_application )
    {
        case APP_FILEMANAGER:
            /* currently in FileMgr application,
               go back to root after display pop up */
            if ( end_screen == 0 )
                DeleteUptoScrID(SCR_FMGR_MAIN);
            else
            {
                GetNextScrnIdOf(SCR_FMGR_MAIN, &start_screen);
                DeleteBetweenScreen(start_screen, end_screen);
            }
            break;
        case APP_PHONEBOOK:
        case APP_PHONESETUP:
            break;

#ifdef __MMI_SOUND_RECORDER__
        case APP_SOUNDRECORDER:
            if ( end_screen == 0 )
                DeleteUptoScrID(SCR_FMGR_EXPLORER);
            else
            {
                GetNextScrnIdOf(SCR_FMGR_EXPLORER, &start_screen);
                DeleteBetweenScreen(start_screen, end_screen);
            }
            break;
#endif

#ifdef __MMI_AB_REPEATER__
        case APP_ABREPEATER:
            if ( end_screen == 0 )
                DeleteUptoScrID(SCR_FMGR_EXPLORER);
            else
            {
                GetNextScrnIdOf(SCR_FMGR_EXPLORER, &start_screen);
                DeleteBetweenScreen(start_screen, end_screen);
            }
            break;
#endif

#ifdef __MMI_CAMERA__
        case APP_IMAGEVIEWER:
            if ( end_screen == 0 )
                DeleteUptoScrID(SCR_ID_IMGVIEW_APP);
            else
            {
                GetNextScrnIdOf(SCR_ID_IMGVIEW_APP, &start_screen);
                DeleteBetweenScreen(start_screen, end_screen);
            }
            break;
#endif /* __MMI_CAMERA__ */

#ifdef __MMI_VIDEO__
        case APP_VDOPLY:
            if ( end_screen == 0 )
                DeleteUptoScrID(SCR_ID_VDOPLY_APP);
            else
            {
                GetNextScrnIdOf(SCR_ID_VDOPLY_APP, &start_screen);
                DeleteBetweenScreen(start_screen, end_screen);
            }
            break;
#endif /* __MMI_VIDEO__ */

#ifdef __MMI_EBOOK_READER__
        case APP_EBOOK:
            if (end_screen == 0)
            {
                DeleteUptoScrID(SCR_FMGR_EXPLORER);
            }
            else
            {
                //GetNextScrnIdOf(SCR_FMGR_EXPLORER, &start_screen);
                if (IsScreenPresent(SCR_FMGR_EXPLORER) && SCR_FMGR_EXPLORER != marker_screen)
                DeleteBetweenScreen(SCR_FMGR_EXPLORER, end_screen);
            }
            break;
#endif /* __MMI_EBOOK_READER__ */ 

        default:
            /* not on FileMgr screen, don't do anything */
            break;
    }
}
    #endif

extern BOOL is_phb_init_done(void);
void fmgr_launch (void)
{
#if 0
	if(!is_phb_init_done())
	{
		//开机初始化 stringid =25803  modified by renyh  2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_PROGRESS/*IMG_GLOBAL_ACTIVATED*/, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);//modified by lin for bug 6761@20070929
		return;
	}
#endif
	fmgr_p->src_application = APP_FILEMANAGER;
	FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
	fmgr_p->tree_action = FMGR_ACTION_NONE;
	TRACE_FMGR_FUNCTION();
        fmgr_reset_application_highlight_idx();
	entry_fmgr_main();
}

extern int checkUsbstate(void);
extern VOID mmi_display_usb_busy(void);
void entry_fmgr_main(void)
{
    S32     i, drv_idx;
    U8*      guiBuffer;
    U16     drive_count, lsk_str=0, lsk_img=0, imageId=0, titleId=STR_FMGR_TITLE;
    pBOOL   thick_style = FALSE;
    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_MAIN);

    /* init active explorer as default explorer */
    fmgr_p->active_explorer_screen = SCR_FMGR_EXPLORER;

    fmgr_p->cache_end_idx = 0;
    fmgr_p->cache_start_idx = 0;
    memset ((PU8)fmgr_p->file_cache, 0, sizeof (FMGR_FILE_INFO_STRUCT) * FMGR_CACHE_ENTRY_COUNT);

    if ( fmgr_is_msdc_present() == FALSE
        /*+ zhouqin modify for dual t-flash card 20110503*/
        #if defined(DUAL_TFLASH_SUPPORT)
         && (fmgr_is_sec_msdc_present() == FALSE)
         #endif
        /*- zhouqin modify for dual t-flash card 20110503*/
     )
    {
        PRINT_INFORMATION(("No Card\n"));
    }
	else
	{
		if(checkUsbstate())
		{
			mmi_display_usb_busy();
			return;
		}
	}

    if ( fmgr_p->drv_count == 0 )
    {
        PRINT_INFORMATION(("No Drive\n"));
//wug add used to cs_XXXX 20070821 start
//#ifdef __MMI_CAMERA__    wug del used to no camera
        /*WeiD Add Start For 6013 Ver: TBM780  on 2007-8-13 16:6 */
        DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, CAMERA_WARNING_POPUPDURATION, 1000, WARNING_TONE);
        /*WeiD Add End  For 6013 Ver: TBM780  on 2007-8-13 16:6 */
//#endif      wug del used to no camera
//wug add used to cs_XXXX 20070821 end
/*WeiD Del Start For 6013 Ver: TBM780  on 2007-8-13 16:6 */
        #if 0
        DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_EMPTY,
        0, 1000, EMPTY_LIST_TONE);
        #endif
/*WeiD Del End  For 6013 Ver: TBM780  on 2007-8-13 16:6 */

        if ( fmgr_p->src_application != APP_FILEMANAGER )
            fmgr_abort_app_select();
        TRACE_EF(g_sw_FMGR, "gdm:fmgr_p->drv_count= %d", fmgr_p->drv_count);
        return;
    }

    for ( i=0; i<FMGR_TOTAL_STORAGE && fmgr_p->drv_list[i][0] != 0; i++ )
    {
        if ( MMI_FS_GetDevStatus(fmgr_p->drv_list[i][0], FS_FEATURE_STATE_ENUM) == FS_DEVICE_BUSY )
        {
            DisplayPopup((PU8)GetString(FMGR_FS_DEVICE_BUSY_TEXT), IMG_GLOBAL_ERROR, 0,
            1000, ERROR_TONE);
            if ( fmgr_p->src_application != APP_FILEMANAGER )
                fmgr_abort_app_select();
            //return;
        }
    }


    EntryNewScreen(SCR_FMGR_MAIN, NULL, entry_fmgr_main, NULL);

    RegisterHighlightHandler(highlight_fmgr_drive_list);

    if ( fmgr_p->src_application == APP_FILEMANAGER )
    {
        if ( fmgr_p->tree_action == FMGR_ACTION_COPY )
        {
            thick_style = TRUE;
            titleId = STR_FMGR_COPY;
        }
        else if ( fmgr_p->tree_action == FMGR_ACTION_MOVE )
        {
            thick_style = TRUE;
            titleId = STR_FMGR_MOVE;
        }
    }
    else
    {
        if ( (fmgr_p->src_application == APP_IMAGEVIEWER) ||
        (fmgr_p->src_application == APP_VDOPLY) )
            thick_style = TRUE;
    }

    DeleteScreenIfPresent(SCR_FMGR_TREE_EXPLORER);
    DeleteScreenIfPresent(SCR_FMGR_FILE_OPTION);
    if ( fmgr_p->tree_action == FMGR_ACTION_NONE )
        DeleteScreenIfPresent(SCR_FMGR_EXPLORER);

    /* get actual drive count */
    drive_count = 0;

    for ( i = 0 ; i < fmgr_p->drv_count; i++ )
    {
        drv_idx = fmgr_get_drive_index(i);

        if ( drv_idx == FMGR_REMOVABLE_STORAGE )
        {
            /* test if card is actually present */
            if ( fmgr_is_msdc_present() == TRUE )
                drive_count++;
        }
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
        else if ( drv_idx == FMGR_STORAGE_TFCARD_SECOND )
        {
            /* test if card 2 is actually present */
            if ( fmgr_is_sec_msdc_present() == TRUE )
                drive_count++;
        }
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
        else
            drive_count++;
        TRACE_EF(g_sw_FMGR, "drive_count= %d", drive_count);
    }

    if ( drive_count > 0 )
    {
        lsk_str = STR_GLOBAL_OPTIONS;
        lsk_img = IMG_GLOBAL_OPTIONS;
    }

    imageId = fmgr_get_app_title_icon_id();
    TRACE_EF(g_sw_FMGR, "drvcnt %d(%d) . %s %s %s",fmgr_p->drv_count,drive_count, fmgr_p->drv_list[0],fmgr_p->drv_list[1],fmgr_p->drv_list[2]);
    TRACE_EF(g_sw_FMGR, "thick_style = %d", thick_style);
    if ( thick_style == TRUE )
        ShowCategory213Screen(   titleId, imageId,        /* title	*/
        lsk_str, lsk_img,        /* lsk */
        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,  /* rsk */
        drive_count,
        mmi_fmgr_draw_drv_menuitem_callback,
        mmi_fmgr_hide_drv_menuitem_callback,
        0,             /* hiligth item idx */
        CAT213_ARROW_ALL,
        (U8*)guiBuffer);

    else
        /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-7 15:8 */
        ShowCategory212Screen(
        titleId, 
        imageId,        /* title */
        lsk_str, 
        lsk_img,       /* lsk */
        STR_GLOBAL_BACK, 
        IMG_GLOBAL_BACK,  /* rsk */
        drive_count,
        mmi_fmgr_get_drv_item_callback,
        mmi_fmgr_get_drv_hint_callback,
        mmi_fmgr_draw_drv_info_callback,
        mmi_fmgr_hide_drv_info_callback,
        2,             /* info box row count */
        0,
        0,
        (U8*)guiBuffer);
        /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-7 15:8 */

    if ( drive_count == 0 )
        SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    else
	fmgr_p->enter_folder = TRUE; //gdm 090602 fix buger 11778

    return;
}

void entry_fmgr_drive_option(void)
{
    U8* guiBuffer;
    U16 nNumofItem, iconId;
    U16 nStrItemList[20];
    U16 scrId=SCR_FMGR_DRIVE_OPTION;

    TRACE_FMGR_FUNCTION();
    if ( MMI_FS_GetDevStatus(fmgr_p->file_path[0], FS_FEATURE_STATE_ENUM) == FS_DEVICE_BUSY )
    {
        DisplayPopup((PU8)GetString(FMGR_FS_DEVICE_BUSY_TEXT), IMG_GLOBAL_ERROR, 0,
        1000, ERROR_TONE);
        ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);
#ifdef __MMI_TOUCH_SCREEN__
        mmi_wgui_reset_list_item_selected_callback();
#endif
        return;
    }
    TRACE_FMGR_FUNC("(MMI_FS_GetDevStatus(fmgr_p->file_path[0], FS_FEATURE_STATE_ENUM) == FS_DEVICE_BUSY)");
    if ( fmgr_p->src_application == APP_FILEMANAGER )
    {
        if ( fmgr_p->tree_action == FMGR_ACTION_COPY ||
        fmgr_p->tree_action == FMGR_ACTION_MOVE )
        {
            scrId = SCR_FMGR_TREE_DRIVE_OPTION;
            nNumofItem = GetNumOfChild(MITEM_FMGR_TREE_OPTIONS);
            GetSequenceStringIds(MITEM_FMGR_TREE_OPTIONS,nStrItemList);
            SetParentHandler(MITEM_FMGR_TREE_OPTIONS);
        }
        else
        {
            nNumofItem = GetNumOfChild(MITEM_FMGR_DRIVE_OPTIONS);
            GetSequenceStringIds(MITEM_FMGR_DRIVE_OPTIONS,nStrItemList);
            SetParentHandler(MITEM_FMGR_DRIVE_OPTIONS);
            TRACE_EF(g_sw_FMGR, "fmgr_p->cur_index = %d", fmgr_p->cur_index);
            switch ( fmgr_get_drive_index(fmgr_p->cur_index) )
            {
/*WUZC Del Start For MMIFS Ver:    on 2007-1-22 12:0 */
#if 0
                case FMGR_NAND_STORAGE:
                    /* highlight NAND Drive, hide unmount option */
                    nNumofItem--;
                    break;
#endif
/*WUZC Del End  For MMIFS Ver:    on 2007-1-22 12:0 */
                case FMGR_NOR_STORAGE:
                    /* highli NOR Drive, hide unmount option */
                    nNumofItem--;
                    break;
                case FMGR_REMOVABLE_STORAGE:
#if defined(DUAL_TFLASH_SUPPORT)
                case FMGR_STORAGE_TFCARD_SECOND:
#endif
                    /* highlight removable drive */
                    nNumofItem--;  
                    break;
                default:
                    /* wrong index, hide unmount option */
                    nNumofItem--;
                    break;
            }
//	    fmgr_p->cur_index = 0;//add gdm 081225, greenstone,  it represent  hiligth item idx in entry_fmgr_explorer()
        }
    }
    else
    {
        /* other app for select */       
        nNumofItem = GetNumOfChild(MITEM_FMGR_APP_DRIVE_OPTIONS);     
        GetSequenceStringIds(MITEM_FMGR_APP_DRIVE_OPTIONS,nStrItemList);     
        SetParentHandler(MITEM_FMGR_APP_DRIVE_OPTIONS);   
    }

    EntryNewScreen(scrId, NULL,
    entry_fmgr_drive_option, NULL);

    guiBuffer = GetCurrGuiBuffer(scrId);

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    iconId = fmgr_get_app_title_icon_id ();

    ShowCategory15Screen(STR_GLOBAL_OPTIONS, iconId,
    STR_GLOBAL_OK, IMG_GLOBAL_OK,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    nNumofItem, nStrItemList,
    (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
    return;

}


void entry_fmgr_drive_confirm(void)
{
    U16 stringId;
    S8 index;

    TRACE_FMGR_FUNCTION();
    switch ( fmgr_p->drive_action )
    {
        case FMGR_ACTION_FORMAT:
			
            TRACE_FMGR_FUNCTION();
			
		if(checkVMOn())
		{
		    mmi_show_qq_select_confirm(entry_fmgr_drive_confirm);

			return;
		}

            index = fmgr_get_drive_index(fmgr_p->cur_index);
            if ( index == FMGR_REMOVABLE_STORAGE )
                stringId = STR_FMGR_FORMAT;
            else
            {
#if (!defined( APP_STORAGE_IN_SYS_DRV)) && defined(__MMI_EMAIL__)
                stringId = STR_FMGR_FORMAT_CONFIRM;
#else
                stringId = STR_FMGR_FORMAT;
#endif
            }
            break;
        case FMGR_ACTION_CHECK_DRIVE:
            stringId = STR_FMGR_CHECK_DRIVE;
            break;
        case FMGR_ACTION_UNMOUNT:
            stringId = STR_FMGR_UNMOUNT;
            break;
        default:
            return;
    }

    TRACE_FMGR_FUNCTION();
    DisplayConfirm(
    STR_GLOBAL_YES,IMG_GLOBAL_YES,
    STR_GLOBAL_NO,IMG_GLOBAL_NO,
    get_string(stringId), IMG_GLOBAL_QUESTION, WARNING_TONE);

    SetLeftSoftkeyFunction(fmgr_drive_action_hdlr,KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    TRACE_FMGR_FUNCTION();
    return;
}

void entry_fmgr_drive_animation(void)
{
#ifdef MMI_ON_HARDWARE_P
    U16 stringId;

    TurnOnBacklight(0);

    switch ( fmgr_p->drive_action )
    {
        case FMGR_ACTION_FORMAT:
            stringId = STR_FMGR_FORMATING;
            break;
        case FMGR_ACTION_CHECK_DRIVE:
            stringId = STR_FMGR_CHECK_DRIVE;
            break;
        case FMGR_ACTION_UNMOUNT:
            stringId = STR_FMGR_UNMOUNTING;
            break;
        default:
            return;
    }

    EntryNewScreen(SCR_FMGR_DRIVE_ANIMATION, exit_fmgr_drive_animation,
    NULL, NULL);

    ShowCategory66Screen(STR_FMGR_TITLE, GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID),
    0, 0, 0, 0,
    (PU8)GetString(stringId),
    IMG_GLOBAL_PROGRESS, NULL);

    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
    if ( !IsScreenPresent(SCR_FMGR_DRIVE_ANIMATION) )
        DeleteUptoScrID(SCR_FMGR_MAIN);
#else
    DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0,
    1000, SUCCESS_TONE);
#endif
}

void exit_fmgr_drive_animation(void)
{
    if ( isInCall() == FALSE )
    {
        /* pop up interrupt => add to history */
        history_t currHistory;
        S16     nHistory    = 0;

        currHistory.scrnID = SCR_FMGR_DRIVE_ANIMATION;
        currHistory.entryFuncPtr = entry_fmgr_drive_animation;
        pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
        GetCategoryHistory( currHistory.guiBuffer );
        AddHistory( currHistory );
    }
#ifdef __FS_CHECKDRIVE_SUPPORT__	
    else
    {
        /* interrupt by Call => abort check drive */
        if ( fmgr_p->tree_action == FMGR_ACTION_CHECK_DRIVE )
        {
#ifdef MMI_ON_HARDWARE_P
            FS_Abort(FS_ABORT_CHECK_DRIVE);
#endif
            FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
            fmgr_p->tree_action = FMGR_ACTION_NONE;
            fmgr_p->enter_folder = TRUE;
        }
    }
#endif	
}

void fmgr_create_folder_hdlr (void)
{
    memset (fmgr_p->new_file_name, 0, sizeof (fmgr_p->new_file_name));
    entry_fmgr_filename_editor();
    SetHiliteHandler(MENU_FMGR_EDITOR_DONE, highlight_fmgr_folder_done);
}

extern U8 ClearDelScrnIDCallbackHandlerEx(U16 ScrnID, HistoryDelCBPtr funcPtr);
void entry_fmgr_explorer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/ 
    U8* guiBuffer;
    S32 highlightId=0;
    U16 count, iconId;
    U8  arrow_display;

#ifdef __MMI_CAMERA__
    U16 imgview_style;
    S32 box_width;
    S32 box_height; 
#endif /* __MMI_CAMERA__ */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/     
	mmi_trace(g_sw_FMGR, "chenxia!!! here is entry_fmgr_explorer, fmgr_p->enter_folder=%d",fmgr_p->enter_folder);
    TBM_ENTRY(0x2846);
    TRACE_FMGR_FUNCTION();
    if ( MMI_FS_GetDevStatus(fmgr_p->file_path[0], FS_FEATURE_STATE_ENUM) == FS_DEVICE_BUSY )
    {
        DisplayPopup((PU8)GetString(FMGR_FS_DEVICE_BUSY_TEXT), IMG_GLOBAL_ERROR, 0,
        1000, ERROR_TONE);
        ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);
#ifdef __MMI_TOUCH_SCREEN__
        mmi_wgui_reset_list_item_selected_callback();
#endif
        TBM_EXIT(0x2846);
        return;
    }

    if ((fmgr_p->file_path[0]== 'D' ||fmgr_p->file_path[0]  == 'd') &&(g_tflashCardExist == TRUE) && (g_hasMountedTFlashFS == FALSE))
    {
        TRACE_EF(g_sw_FMGR,"DisplayPopup(No format, Tcard unsupported FS!)" );
        DisplayPopup((PU8)GetString(FMGR_FS_INVALID_FILE_SYSTEM_TEXT), IMG_GLOBAL_WARNING, 0, 1000, ERROR_TONE);   
        return ;
    }
    mmi_fmgr_kernel_init ((PS8)fmgr_p->file_path, fmgr_p->filter_type, &count);

    TRACE_EF(g_sw_FMGR, "mmi_fmgr_kernel_init: filecount = %d", count);
    /* browse type do not pop up empty */
    if ( count == 0 )
    {
        TRACE_FMGR_FUNC("(count == 0)");
        if ( fmgr_p->src_application == APP_FILEMANAGER || 
   #if defined(__MMI_IRDA_SUPPORT__) || defined(__MMI_OPP_SUPPORT__) 
   	fmgr_p->src_application == APP_EXTDEVICE ||
#endif
    #ifdef __MMI_FTS_SUPPORT__
            fmgr_p->src_application == APP_FTP ||
    #endif 
#ifdef __MMI_FM_RADIO_RECORD__
        fmgr_p->src_application == APP_FMRDO ||
#endif
        (fmgr_p->src_application != APP_FILEMANAGER && 
        fmgr_p->sel_type != FMGR_SELECT_BROWSE) )
        {
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);

            DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 0,
            1000, EMPTY_LIST_TONE);
            DeleteScreenIfPresent(SCR_FMGR_FOLDER_OPTION);
            DeleteScreenIfPresent(SCR_FMGR_DRIVE_OPTION);
            ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);
            fmgr_p->enter_folder = FALSE;
#ifdef __MMI_TOUCH_SCREEN__
            mmi_wgui_reset_list_item_selected_callback();
#endif
            TBM_EXIT(0x2846);
            return;
        }
    }

    EntryNewScreen(SCR_FMGR_EXPLORER, exit_fmgr_explorer,
    NULL, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_EXPLORER);
	ClearDelScrnIDCallbackHandlerEx(SCR_FMGR_EXPLORER,NULL);
    if ( fmgr_p->enter_folder == TRUE )
    {
        /* enter explorer screen by user selection, not go back history after interrupt */
        fmgr_p->enter_folder = FALSE;        
        fmgr_p->cur_index = fmgr_get_application_highlight_idx();
        DeleteUptoScrID(SCR_FMGR_EXPLORER);

        /* enter explorer screen by user selection shall always highlight first item */
        guiBuffer = NULL;
        fmgr_p->cur_index = 0;

    }
    if ( GetCurrScrnId() == SCR_FMGR_DRIVE_OPTION )
    {
        DeleteScreenIfPresent(SCR_FMGR_DRIVE_OPTION);
    }

    if ( fmgr_p->parent_name[0] != 0 || fmgr_p->parent_name[1] != 0 )
    {
        mmi_trace(g_sw_FMGR, "gdm: in entry_fmgr_explorer():call fmgr_get_parent_highlight_id");
        highlightId = fmgr_get_parent_highlight_id();
        fmgr_p->cur_index  = highlightId;
        memset (fmgr_p->parent_name, 0, sizeof(fmgr_p->parent_name));
        guiBuffer = NULL;
    }
    RegisterHighlightHandler(highlight_fmgr_node);

    /****************************************************************************
    * Display Menu                                                              
    *
    * 1. File Manager		: Use standard list menu with info box. 
    * 2. Image Viewer		: Use thick list menu.
    * 3. Video Player		: Use thick list menu.
    *****************************************************************************/
    /* detemine display arrow type */
    /* if is browse mode, only display up and down arrow */

    if ( (fmgr_p->sel_type & 0x0f) == FMGR_SELECT_BROWSE )
        arrow_display = CAT213_ARROW_UP | CAT213_ARROW_DOWN;
    else
        arrow_display = CAT213_ARROW_ALL;

    iconId = fmgr_get_app_title_icon_id();

    if ( fmgr_p->cur_index > fmgr_p->file_count )
        fmgr_p->cur_index = 0;

    switch ( fmgr_p->src_application )
    {
#ifdef __MMI_CAMERA__
        case APP_IMAGEVIEWER:
            {
                GetCategory214BoxSize(3, 3, &box_width, &box_height);

                /* create image cache */
                mmi_fmgr_create_thumb_image_cache((box_width>box_height)?box_width:box_height);

                imgview_style = mmi_imgview_get_view_style();
                if ( imgview_style == IMGVIEW_VIEW_STYLE_LIST )
                {
                    /* list image viewer */
                    ShowCategory213Screen(  STR_ID_IMGVIEW_NAME, iconId,     /* title */
                    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, /* lsk */
                    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,   /* rsk */
                    count,
                    mmi_fmgr_draw_file_menuitem_callback,
                    mmi_fmgr_hide_file_menuitem_callback,
                    fmgr_p->cur_index,        /* hiligth item idx */
                    arrow_display,
                    (U8*)NULL);
                }
                else if ( imgview_style == IMGVIEW_VIEW_STYLE_MATRIX )
                {
                    /* matrix image view */
                    RegisterHighlightHandler(mmi_fmgr_highlight_matrix_node);
                    ShowCategory214Screen(  STR_ID_IMGVIEW_NAME, iconId,
                    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,     
                    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
                    count,           /* num of item */
                    mmi_fmgr_draw_file_matrix_callback,  /* draw image callback */
                    mmi_fmgr_hide_file_matrix_callback,  /* hide image callback */
                    3,             /* number per row */    
                    3,             /* number per col */
                    fmgr_p->cur_index,       /* hilighted item */
                    (U8*)NULL);
                }
                else
                {
                    MMI_ASSERT(0);
                }

                if ( count == 0 )
                {
                    SetRightSoftkeyFunction(mmi_imgview_exit_from_fmgr_emptry_folder, KEY_EVENT_UP);
                    SetLeftSoftkeyFunction(entry_fmgr_app_empty_folder_option, KEY_EVENT_UP);
                }
            }
            break;
#endif /* __MMI_CAMERA__ */

#ifdef __MMI_VIDEO__
        case APP_VDOPLY:
            {

                /* create image cache */
                mmi_fmgr_create_thumb_image_cache(MMI_thick_menuitem_height);

#ifdef __VDOREC_FEATURE_STORAGE__

                ShowCategory213Screen(  STR_ID_VDOPLY_APP_NAME, iconId,    /* title */
                STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, /* lsk */
                STR_GLOBAL_BACK, IMG_GLOBAL_BACK,   /* rsk */
                count,
                mmi_fmgr_draw_file_menuitem_callback,
                mmi_fmgr_hide_file_menuitem_callback,
                fmgr_p->cur_index,        /* hiligth item idx */
                arrow_display,
                (U8*)NULL);
                if ( count == 0 )
                {
                    SetRightSoftkeyFunction(mmi_vdoply_exit_from_fmgr_emptry_folder, KEY_EVENT_UP);
                    SetLeftSoftkeyFunction(entry_fmgr_app_empty_folder_option, KEY_EVENT_UP);
                }

#else
                ShowCategory213Screen(  STR_ID_VDOPLY_APP_NAME, iconId,    /* title */
                0, 0,             /* lsk */
                STR_GLOBAL_BACK, IMG_GLOBAL_BACK,   /* rsk */
                count,
                mmi_fmgr_draw_file_menuitem_callback,
                mmi_fmgr_hide_file_menuitem_callback,
                fmgr_p->cur_index,        /* hiligth item idx */
                arrow_display,
                (U8*)NULL);
                if ( count == 0 )
                {
                    SetRightSoftkeyFunction(mmi_vdoply_exit_from_fmgr_emptry_folder, KEY_EVENT_UP);
                }

#endif /* __VDOREC_FEATURE_STORAGE__ */
            }
            break;
#endif /* __MMI_VIDEO__ */

#ifdef __MMI_RING_COMPOSER__
        case APP_RINGTONECOMPOSER:
            {

                /* create image cache */
                mmi_fmgr_create_thumb_image_cache(MMI_MENUITEM_HEIGHT*2);

                if ( count>0 )
                {
                    /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-7 15:13 */
                    ShowCategory212Screen(STR_COMPOSER_RINGTONE_LIST, iconId,
                    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,  /* lsk */
                    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,    /* rsk */
                    count,
                    mmi_fmgr_get_file_item_callback,
                    mmi_fmgr_get_file_hint_callback,
                    mmi_fmgr_draw_file_info_callback,
                    mmi_fmgr_hide_file_info_callback,
                    2,               /* info box row count */
                    0,               /* hiligth item idx */
                    0,
                    (U8*)guiBuffer);
                    /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-7 15:13 */
                }
                else
                {
                    ShowCategory213Screen(  STR_COMPOSER_RINGTONE_LIST, iconId,
                    STR_COMPOSER_RINGTONE_ADD, IMG_GLOBAL_OPTIONS, /* lsk */
                    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,     /* rsk */
                    0,
                    mmi_fmgr_draw_file_menuitem_callback,
                    mmi_fmgr_hide_file_menuitem_callback,
                    0,                /* hiligth item idx */
                    0,
                    (U8*)guiBuffer);

                    SetLeftSoftkeyFunction(RngcExecAdd, KEY_EVENT_UP);
                    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
                }
            }
            break;
#endif /* __MMI_RING_COMPOSER__ */

#ifdef __MMI_SOUND_RECORDER__
        case APP_SOUNDRECORDER:
            {
                if ( count>0 )
                {
                    /* create image cache */
                    mmi_fmgr_create_thumb_image_cache(MMI_MENUITEM_HEIGHT*2);

                    /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-7 15:13 */
                    ShowCategory212Screen(
#ifdef __MMI_SOUND_RECORDER_STYLE_X__
                    STR_ID_SNDREC_LIST,
#else
                    STR_ID_SNDREC_MAIN,
#endif
                    iconId, /* title	 */
                    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, /* lsk */
                    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,   /* rsk */
                    count,
                    mmi_fmgr_get_file_item_callback,
                    mmi_fmgr_get_file_hint_callback,
                    mmi_fmgr_draw_file_info_callback,
                    mmi_fmgr_hide_file_info_callback,
                    2, /* info box row count */
                    0, /* hiligth item idx */
                    0,
                    (U8*)guiBuffer);
                    /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-7 15:13 */
                }
                else
                {

                    /* create image cache */
                    mmi_fmgr_create_thumb_image_cache(MMI_thick_menuitem_height);

                    ShowCategory213Screen(
#ifdef __MMI_SOUND_RECORDER_STYLE_X__
                    STR_ID_SNDREC_LIST,
#else
                    STR_ID_SNDREC_MAIN,
#endif
                    iconId, /* title	 */
                    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, /* lsk */
                    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,   /* rsk */
                    0,
                    mmi_fmgr_draw_file_menuitem_callback,
                    mmi_fmgr_hide_file_menuitem_callback,
                    0,           /* hiligth item idx */
                    0,
                    (U8*)guiBuffer);
                    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
                    SetLeftSoftkeyFunction(entry_fmgr_app_empty_folder_option, KEY_EVENT_UP);
                }
            }
            break;
#endif
#ifdef __MMI_AB_REPEATER__
        case APP_ABREPEATER:
            {
                if ( count>0 )
                {
                    /* create image cache */
                    mmi_fmgr_create_thumb_image_cache(MMI_MENUITEM_HEIGHT*2);

                    /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-7 15:12 */
                    ShowCategory212Screen(
                    STR_ID_ABREPEAT_TITLE,
                    iconId, /* title	 */
                    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, /* lsk */
                    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,   /* rsk */
                    count,
                    mmi_fmgr_get_file_item_callback,
                    mmi_fmgr_get_file_hint_callback,
                    mmi_fmgr_draw_file_info_callback,
                    mmi_fmgr_hide_file_info_callback,
                    2, /* info box row count */
                    0, /* hiligth item idx */
                    0,
                    (U8*)guiBuffer);
                    /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-7 15:12 */
                }
                else
                {

                    /* create image cache */
                    mmi_fmgr_create_thumb_image_cache(MMI_thick_menuitem_height);

                    ShowCategory213Screen(
                    STR_ID_ABREPEAT_TITLE,
                    iconId, /* title	 */
                    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, /* lsk */
                    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,   /* rsk */
                    0,
                    mmi_fmgr_draw_file_menuitem_callback,
                    mmi_fmgr_hide_file_menuitem_callback,
                    0,           /* hiligth item idx */
                    0,
                    (U8*)guiBuffer);
                    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
                    SetLeftSoftkeyFunction(entry_fmgr_app_empty_folder_option, KEY_EVENT_UP);
                }
            }
            break;
#endif
    #ifdef __MMI_EBOOK_READER__
        case APP_EBOOK:
        {

            /* create image cache */
            mmi_fmgr_create_thumb_image_cache(MMI_MENUITEM_HEIGHT * 2);

            if (count > 0)
            {
                ShowCategory212Screen(
                    EBOOK_TITLE_BOOKSHELF,
                    GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
                    STR_GLOBAL_OPTIONS,
                    IMG_GLOBAL_OPTIONS, /* lsk */
                    STR_GLOBAL_BACK,
                    IMG_GLOBAL_BACK,    /* rsk */
                    count,
                    ebr_get_file_item_callback,
                    mmi_fmgr_get_file_hint_callback,
                    mmi_fmgr_draw_file_info_callback,
                    mmi_fmgr_hide_file_info_callback,
                    2,                  /* info box row count */
                    0,                  /* info bg img id */
                    0,                  /* hiligth item idx */
                    (U8*) guiBuffer);

            }
            else
            {
                ShowCategory213Screen(
                    EBOOK_TITLE_BOOKSHELF,
                    GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
                    STR_GLOBAL_OPTIONS,
                    IMG_GLOBAL_OPTIONS, /* lsk */
                    STR_GLOBAL_BACK,
                    IMG_GLOBAL_BACK,    /* rsk */
                    0,
                    mmi_fmgr_draw_file_menuitem_callback,
                    mmi_fmgr_hide_file_menuitem_callback,
                    0,                  /* hiligth item idx */
                    0,
                    (U8*) guiBuffer);
                SetLeftSoftkeyFunction(ebr_show_book_shelf_option, KEY_EVENT_UP);
                SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

            }

        }
            break;
    #endif /* __MMI_EBOOK_READER__ */ /* __MMI_RING_COMPOSER__ */

        case APP_FILEMANAGER:
        default: /* others */
            {

                /* create image cache */
                mmi_trace(g_sw_FMGR, "gdm: in entry_fmgr_explorer(),fmgr_p->cur_index=%d,highlightId=%d",fmgr_p->cur_index,highlightId);
                mmi_fmgr_create_thumb_image_cache(MMI_MENUITEM_HEIGHT*2);

                /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-7 15:13 */
                ShowCategory212Screen(  STR_FMGR_TITLE, iconId,       /* title */
                STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, /* lsk */
                STR_GLOBAL_BACK, IMG_GLOBAL_BACK,   /* rsk */
                count,
                mmi_fmgr_get_file_item_callback,
                mmi_fmgr_get_file_hint_callback,
                mmi_fmgr_draw_file_info_callback,
                mmi_fmgr_hide_file_info_callback,
                2, /* info box row count */
                0,
                fmgr_p->cur_index, //highlightId,              /* hiligth item idx */
                (U8*)guiBuffer);//(U8*)guiBuffer);
                /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-7 15:13 */
            }

            break;
    }
    /*****************************************************************************/
	//chenhe,to free filemgr chain memory
	SetDelScrnIDCallbackHandler(SCR_FMGR_EXPLORER,(HistoryDelCBPtr)MMI_FS_XFindClose);
    /* key handlers are set in highlight_fmgr_node */
    TBM_EXIT(0x2846);
    return;
}




void exit_fmgr_explorer(void)
{
    mmi_fmgr_free_thumb_image_cache();

    StopTimer(FMGR_AUDIO_TIMER);
    if ( fmgr_p->audio_state == TRUE )
    {
        fmgr_p->audio_state = FALSE;
#ifdef __MMI_AUDIO_PLAYER__ 
        mdi_audio_stop_file();
#endif
    }

    /* for app select file, after return callback, no need to add explorer into history */
    if ( (fmgr_p->file_path[0] != 0) && IsScreenPresent(SCR_FMGR_EXPLORER) == FALSE )
    {
        history_t  currHistory;
        S16       nHistory    = 0;

        currHistory.scrnID = SCR_FMGR_EXPLORER;
        currHistory.entryFuncPtr = entry_fmgr_explorer;
        pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
        GetCategoryHistory( currHistory.guiBuffer );
        AddHistory( currHistory );
    }
    else
    {
        U8* guiBuffer;

        guiBuffer = GetCurrGuiBuffer(SCR_FMGR_EXPLORER);
        GetCategoryHistory( guiBuffer );
    }
}

void entry_fmgr_folder_option(void)
{
    U16 nStrItemList[20];
    U16 nNumofItem;
    U16 menuId, iconId;
    U8* guiBuffer;

    EntryNewScreen(SCR_FMGR_FOLDER_OPTION, NULL,
    entry_fmgr_folder_option, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_FOLDER_OPTION);

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    if ( fmgr_p->src_application == APP_FILEMANAGER )
    {
        menuId = MITEM_FMGR_FOLDER_OPTIONS; //1605
    }
#ifdef __MMI_FTS_SUPPORT__
    else if (fmgr_p->src_application == APP_FTP)
    {
        menuId = MENU_CONN_FTP_SHARED_FOLDER;
    }
#endif /* __MMI_FTS_SUPPORT__ */ 
    else
    {
        /* other app for select */    
        menuId = MITEM_FMGR_APP_FOLDER_OPTIONS;   //1606
    }

    nNumofItem = GetNumOfChild(menuId);
    GetSequenceStringIds(menuId,nStrItemList);
    SetParentHandler(menuId);

    iconId = fmgr_get_app_title_icon_id ();

    ShowCategory15Screen(STR_GLOBAL_OPTIONS, iconId,
    STR_GLOBAL_OK, IMG_GLOBAL_OK,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    nNumofItem, nStrItemList,
    (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
    return;

}

void entry_fmgr_file_option (void)
{
    FMGR_FILE_INFO_STRUCT file_info;
    U16 nStrItemList[20];
    U16 parentId;
    U16 nNumofItem;
    U8* guiBuffer;

    /* only APP_FILEMANAGER will enter file option,
     other app for select will not have file option menu,
     other app for browse will enter app file option */
    fmgr_set_application_highlight_idx();
    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

    if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE
#ifndef __MMI_SHOW_FILE_EXT__
    || mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == FALSE
#endif
    )
        return;
    else
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
    EntryNewScreen(SCR_FMGR_FILE_OPTION, exit_fmgr_file_option, entry_fmgr_file_option, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_FILE_OPTION);
    parentId = fmgr_get_file_option_menu(&file_info);

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
    nNumofItem = GetNumOfChild_Ext(parentId);
    GetSequenceStringIds_Ext(parentId,nStrItemList);
    SetParentHandler(parentId);
    TRACE_EF(g_sw_FMGR, "[fmgr_get_file_option_menu:] nNumofItem = %d parentId=%d.", nNumofItem,parentId);

    ShowCategory15Screen(STR_GLOBAL_OPTIONS, GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID),
    STR_GLOBAL_OK, IMG_GLOBAL_OK,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    nNumofItem, nStrItemList,
    (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );


    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
    return;
}

void exit_fmgr_file_option(void)
{
    mmi_frm_unhide_menu_item(MENU_FMGR_GEN_OPTION_FORWARD);
    mmi_frm_unhide_menu_item(MENU_FMGR_IMG_OPTION_EDIT);
}

void entry_fmgr_sort_option(void)
{
    U16 nStrItemList[10];
    U8* guiBuffer;
    U16 nNumofItem, iconId;

    TRACE_FMGR_FUNCTION();


    if ( fmgr_is_parent2_present() == FALSE )
    {
        DisplayPopup((PU8)GetString(FMGR_FS_PATH_NOT_FOUND_TEXT), IMG_GLOBAL_ERROR,
        0, 1000, ERROR_TONE);

        /* go back to root, reload file manager */
        if ( fmgr_p->src_application == APP_FILEMANAGER )
            fmgr_adjust_history(SCR_FMGR_MAIN);
        else
            fmgr_adjust_history(0);
        //fmgr_return_to_app();
        return;
    }

    EntryNewScreen(SCR_FMGR_SORT_OPTION, exit_fmgr_sort_option,
    entry_fmgr_sort_option, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_SORT_OPTION);

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
    nNumofItem = GetNumOfChild(MENU_FMGR_GEN_OPTION_SORT);
    GetSequenceStringIds(MENU_FMGR_GEN_OPTION_SORT,nStrItemList);
    SetParentHandler(MENU_FMGR_GEN_OPTION_SORT);
    iconId = fmgr_get_app_title_icon_id();

    ShowCategory15Screen(STR_FMGR_SORT, iconId,
    STR_GLOBAL_OK, IMG_GLOBAL_OK,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    nNumofItem, nStrItemList,
    (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

    SetLeftSoftkeyFunction( fmgr_sort, KEY_EVENT_UP );
    SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    return;
}

void exit_fmgr_sort_option(void)
{
    S16      error;
    U8           temp;

    ReadValue(NVRAM_EF_FMGR_SORT_OPTION_LID, &temp, DS_BYTE, &error);
    if ( temp == 0xff )
        fmgr_p->sort_option = FS_NO_SORT;
    else
        fmgr_p->sort_option = temp << 5;
}

void entry_fmgr_forward_option(void)
{
    FMGR_FILE_INFO_STRUCT file_info;
    U16 nStrItemList[10];
    U16 parentId, imageId=0;
    U16 nNumofItem;
    U8* guiBuffer;

    TRACE_FMGR_FUNCTION();
    if ( fmgr_check_file_present() < FMGR_NO_ERROR )
        return;

    /* show menu according to file type */
    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

    parentId = fmgr_get_forward_option_menu(&file_info, NULL);
	mmi_trace(g_sw_BT, "entry_fmgr_forward_option::parentId=%d", parentId);
    if ( parentId == 0 )
    {
        DisplayPopup((U8*)GetString(STR_FMGR_INVALID_FORMAT),
        IMG_GLOBAL_WARNING, 0, 1000, WARNING_TONE);
        return;
    }

    /* only audio, image file can forward to MMS */
    if ( (parentId == MENU_FMGR_FWD_OPTION_GEN) && 
    (!(FMGR_FILTER_IS_SET_IMAGE(&file_info.file_type) || 
    FMGR_FILTER_IS_SET_AUDIO(&file_info.file_type))) )
        mmi_frm_hide_menu_item(MENU_FMGR_FWD_MMS);

  if (mmi_bootup_get_active_flight_mode() == 1)
  	{
	   mmi_frm_hide_menu_item(MENU_FMGR_FWD_BT);
  	}
    nNumofItem = GetNumOfChild_Ext(parentId);

    TRACE_EF(g_sw_FMGR, "entry_fmgr_forward_option(): parentId=%d,  nNumofItem=%d",parentId,nNumofItem);
    if ( nNumofItem == 0 )
    {
        DisplayPopup((U8*)GetString(STR_FMGR_INVALID_FORMAT),
        IMG_GLOBAL_WARNING, 0, 1000, WARNING_TONE);
        return;
    }

    EntryNewScreen(SCR_FMGR_FORWARD_OPTION, exit_fmgr_forward_option,
    NULL, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_FORWARD_OPTION);
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
    GetSequenceStringIds_Ext(parentId,nStrItemList);
    SetParentHandler(parentId);

    imageId = fmgr_get_app_title_icon_id();

    ShowCategory15Screen(STR_FMGR_FORWARD, imageId,
    STR_GLOBAL_OK, IMG_GLOBAL_OK,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    nNumofItem, nStrItemList,
    (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

    SetLeftSoftkeyFunction( fmgr_forward_to_app, KEY_EVENT_UP );
    SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    return;
}

void exit_fmgr_forward_option(void)
{
    history_t currHistory;
    S16      nHistory    = 0;

    fmgr_p->tree_action = FMGR_ACTION_NONE;
    mmi_frm_unhide_menu_item(MENU_FMGR_FWD_MMS);
    mmi_frm_unhide_menu_item(MENU_FMGR_FWD_BT);	
    currHistory.scrnID = SCR_FMGR_FORWARD_OPTION;
    currHistory.entryFuncPtr = entry_fmgr_forward_option;
    pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
    GetCategoryHistory( currHistory.guiBuffer );
    AddHistory( currHistory );
}

void entry_fmgr_detail (void)
{
#ifdef __DRM_SUPPORT__	
    FS_HANDLE handle;
    BOOL need_activate=FALSE;
#endif
    FMGR_FILE_INFO_STRUCT file_info;
    S8      buffer[32];
    U32 file_size;
    //U8*	description[10];
    U8  *guiBuffer, *info;

    TRACE_FMGR_FUNCTION();
    EntryNewScreen(SCR_FMGR_DETAIL, NULL,
    entry_fmgr_detail, NULL);

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);
    guiBuffer = GetCurrGuiBuffer( SCR_FMGR_DETAIL );

    info = (PU8)subMenuData;

    pfnUnicodeStrcpy((PS8)info, (PS8)GetString(STR_ID_FMGR_FILE_DATE));
    pfnUnicodeStrcat ((PS8)info, (PS8)L"\n");       
    date_string(&file_info.time, (PU16)&info[pfnUnicodeStrlen((PS8)info)<<1], DT_IDLE_SCREEN);
    pfnUnicodeStrcat ((PS8)info, (PS8)L"\n");

    pfnUnicodeStrcat((PS8)info, (PS8)GetString(STR_ID_FMGR_FILE_SIZE));
    pfnUnicodeStrcat ((PS8)info, (PS8)L"\n");       
    if ( file_info.file_size > 1024*1024 )
    {
        file_size = file_info.file_size/1024;
        sprintf(buffer, "%d.%dM", (U16)(file_size/1024), (U16)((file_size%1024)/103));
    }
    else if ( file_info.file_size > 1024 )
    {
        sprintf(buffer, "%d.%dK", (U16)(file_info.file_size/1024), (U16)((file_info.file_size%1024)/103));
    }
    else /* less than 1024 */
    {
        sprintf(buffer, "%dB", (U16)(file_info.file_size));
    }
    AnsiiToUnicodeString((PS8)&info[pfnUnicodeStrlen((PS8)info)<<1], (PS8)buffer);
    pfnUnicodeStrcat ((PS8)info, (PS8)L"\n");    

#ifdef __DRM_SUPPORT__	
    if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == TRUE
#ifndef __MMI_SHOW_FILE_EXT__
    && mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == TRUE
#endif
    )
    {
#if 0	
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif		   
        {
            U8 perm=0;

            if ( FMGR_FILTER_IS_SET_AUDIO(&file_info.file_type) )
                perm = DRM_PERMISSION_PLAY;
            else if ( FMGR_FILTER_IS_SET_IMAGE(&file_info.file_type) )
                perm = DRM_PERMISSION_DISPLAY;
#ifdef __J2ME__			
            else if ( FMGR_FILTER_IS_SET (&file_info.file_type, FMGR_TYPE_JAR) ||
            FMGR_FILTER_IS_SET (&file_info.file_type, FMGR_TYPE_JAD) )
                perm = DRM_PERMISSION_EXECUTE;
#endif			
            handle = DRM_open_file ((PU16)fmgr_p->file_path, FS_READ_ONLY, perm);
            if ( handle > FS_NO_ERROR )
            {
                need_activate = fmgr_populate_drm_info (handle, perm, info);
                DRM_close_file (handle);
            }
        }
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
    }
#endif /*__DRM_SUPPORT__*/


#ifdef __DRM_SUPPORT__
    if ( need_activate == TRUE )
    {
        ShowCategory74Screen(STR_FMGR_DETAIL, GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID), 
        STR_FMGR_LSK_ACTIVATE, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
        (PU8)subMenuData, MAX_SUB_MENUS*MAX_SUB_MENU_SIZE, guiBuffer);    
        if ( FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_UNKNOW) )
            SetLeftSoftkeyFunction(entry_fmgr_request_unknown_rights_confirm, KEY_EVENT_UP);
        else
            SetLeftSoftkeyFunction(entry_fmgr_request_rights_confirm, KEY_EVENT_UP);
    }
    else
#endif      

        ShowCategory74Screen(STR_FMGR_DETAIL, GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID), 
        0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
        (PU8)subMenuData, MAX_SUB_MENUS*MAX_SUB_MENU_SIZE, guiBuffer);

    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

    #ifdef __DRM_SUPPORT__
BOOL fmgr_populate_drm_info (U32 handle, U8 permission, U8* data)
{
    DRM_RIGHTS_STRUCT *rights;
    FS_HANDLE input;
    S32 start=0, end=0;
    BOOL ret = FALSE;

    input = handle;
    rights = DRM_get_rights(input);
    if ( rights == NULL )
        return ret;

    if ( (rights->method & DRM_METHOD_FORWARD_LOCK) ||
    (rights->method & DRM_METHOD_COMBINED_DELIVERY) || 
    (rights->method & DRM_METHOD_SEPARATE_DELIVERY) )
    {
        pfnUnicodeStrcat((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_RIGHT));
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");              
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DRM_PROTECTED));     
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");              
    }
    else
    {
        pfnUnicodeStrcat((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_RIGHT));
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");              
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DRM_NOT_PROTECTED));     
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");              
        return ret;
    }

    if ( rights->method & DRM_METHOD_SEPARATE_DELIVERY &&
    rights->status == DRM_STATUS_RIGHTS_PENDING )
    {
        memset (fmgr_p->rights_issuer, 0, sizeof(fmgr_p->rights_issuer));
        if ( strncmp ((PS8)rights->issuer, "http", 4) != 0 )
            strcpy ((PS8)fmgr_p->rights_issuer, "http://");
        strcat ((PS8)fmgr_p->rights_issuer, (PS8)rights->issuer);
        ret = TRUE;
    }

    if ( rights->method == DRM_METHOD_FORWARD_LOCK )
    {
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_COUNT));    
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DRM_UNLIMITED));       
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_START));    
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DRM_UNLIMITED));       
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_END));      
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DRM_UNLIMITED));       
        return ret;
    }
    if ( (rights->method & DRM_METHOD_COMBINED_DELIVERY) || 
    ((rights->method & DRM_METHOD_SEPARATE_DELIVERY) && 
    rights->status == DRM_STATUS_RIGHTS_PRESENT) )
    {
        MYTIME t;
        S8 buffer[20];
        DRM_CONSTRAINT_STRUCT *constraint;

        if ( (permission & DRM_PERMISSION_DISPLAY) && (rights->permission & permission) )
            constraint = &rights->display;
        else if ( (permission & DRM_PERMISSION_PLAY) && (rights->permission & permission) )
            constraint = &rights->play;
        else if ( (permission & DRM_PERMISSION_EXECUTE) && (rights->permission & permission) )
            constraint = &rights->execute;
        else
        {
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_COUNT));    
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_GLOBAL_INVALID));       
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_START));    
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_GLOBAL_INVALID));       
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_END));      
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_GLOBAL_INVALID));       
            return ret;
        }

        if ( constraint->type & DRM_CONSTRAINT_COUNT )
        {
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_COUNT));    
            sprintf((char *)buffer, "\n%d/%d", constraint->used_count, constraint->total_count);
            AnsiiToUnicodeString((PS8)&data[pfnUnicodeStrlen((PS8)data)<<1], (PS8)buffer);
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        }
        else
        {
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_COUNT));    
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            pfnUnicodeStrcat((PS8)data, (PS8)GetString(STR_FMGR_DRM_UNLIMITED));
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        }

        if ( constraint->type & DRM_CONSTRAINT_DATETIME )
        {
            if ( (constraint->type & DRM_CONSTRAINT_INTERVAL) &&
            constraint->start_intv != 0 )
            {
                start = (constraint->start_time > constraint->start_intv) ? constraint->start_time : constraint->start_intv;
                end = (constraint->end_time > constraint->end_intv) ? constraint->end_intv : constraint->end_time;
            }
            else
            {
                start = constraint->start_time;
                end = constraint->end_time;
            }
        }
        else if ( constraint->type & DRM_CONSTRAINT_INTERVAL )
        {
            if ( constraint->interval <= 0 )
            {
                start = -1;
                end = -1;
            }
            else
            {
                start = constraint->start_intv;
                end = constraint->end_intv;
            }
        }

        if ( start < 0 )
        {
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_START));       
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_GLOBAL_INVALID));       
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        }
        else if ( start == 0 )
        {
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_START));      
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DRM_UNLIMITED));       
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        }
        else
        {
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_START));      
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            mmi_dt_utc_sec_2_mytime(start, &t);
            date_string (&t, (PU16)&data[pfnUnicodeStrlen((PS8)data)<<1], DT_IDLE_SCREEN);
            pfnUnicodeStrcat((PS8)data, (PS8)L" ");
            time_string (&t, (PU16)&data[pfnUnicodeStrlen((PS8)data)<<1], DT_IDLE_SCREEN);
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        }

        if ( end < 0 )
        {
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_END));     
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_GLOBAL_INVALID));       
        }
        else if ( end == 0 )
        {
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_END));     
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DRM_UNLIMITED));       
        }
        else
        {
            pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_END));     
            pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
            mmi_dt_utc_sec_2_mytime(end, &t);
            date_string (&t, (PU16)&data[pfnUnicodeStrlen((PS8)data)<<1], DT_IDLE_SCREEN);
            pfnUnicodeStrcat((PS8)data, (PS8)L" ");
            time_string (&t, (PU16)&data[pfnUnicodeStrlen((PS8)data)<<1], DT_IDLE_SCREEN);
        }
    }
    else
    {
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_COUNT));    
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_GLOBAL_NOT_AVAILABLE));     
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_START));    
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_GLOBAL_NOT_AVAILABLE));     
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_FMGR_DETAIL_END));      
        pfnUnicodeStrcat ((PS8)data, (PS8)L"\n");       
        pfnUnicodeStrcat ((PS8)data, (PS8)GetString(STR_GLOBAL_NOT_AVAILABLE));     
    }

    return ret;
}
    #endif /*__DRM_SUPPORT__*/

void fmgr_rename_hdlr (void)
{
    FMGR_FILE_INFO_STRUCT file_info;
    PS8 ext_name;


    TRACE_FMGR_FUNCTION();

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);
    pfnUnicodeStrcpy((PS8)fmgr_p->new_file_name, (PS8)file_info.file_name);

    /* rename file: old name in fmgr_p->old_file_path */
    pfnUnicodeStrcpy((PS8)fmgr_p->old_file_path, (PS8)fmgr_p->file_path);
    /*WeiD Modify Start For 6242 Ver: TBM780  on 2007-8-15 18:27 */
    if((pfnUnicodeStrlen((PS8)fmgr_p->old_file_path) + pfnUnicodeStrlen((PS8)file_info.file_name)) > FMGR_MAX_PATH_LEN)
    /*WeiD Modify End  For 6242 Ver: TBM780  on 2007-8-15 18:27 */
    {
        // display popup file name too long
        DisplayPopup((PU8)GetString(STR_ID_FMGR_FILENAME_TOO_LONG),
                IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
        return ;
    }
    pfnUnicodeStrcat((PS8)fmgr_p->old_file_path, (PS8)file_info.file_name);


#ifdef __MMI_SHOW_FILE_EXT__   
    if ( !(file_info.attribute & FS_ATTR_DIR) )
    {
        ext_name = mmi_fmgr_extract_ext_file_name((PS8)fmgr_p->new_file_name);

        /* rename file should keep original extension filename in fmgr_p->ext_name
           and append after user input new name */
        if ( ext_name != NULL )
        {
            *(ext_name - 2) = 0;
            fmgr_p->ext_name = mmi_fmgr_extract_ext_file_name((PS8)fmgr_p->old_file_path);   
        }
    }
#endif

#ifdef MMI_ON_HARDWARE_P
    if ( file_info.is_short )
    {
        U8 temp[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];

        /* rename short name file should pass converted short name to editor,
         so that the display will be the same as in File Manager */
        pfnUnicodeStrncpy((S8*)temp, (S8*)fmgr_p->new_file_name,sizeof(temp)/2-1);
        mmi_chset_mixed_text_to_ucs2_str( (U8*)fmgr_p->new_file_name, (MAX_SUBMENU_CHARACTERS-1) * ENCODING_LENGTH, (U8*)temp, (mmi_chset_enum)PhnsetGetDefEncodingType() );
    }
#endif
    entry_fmgr_filename_editor();
    SetHiliteHandler(MENU_FMGR_EDITOR_DONE, highlight_fmgr_rename_done);
}

void entry_fmgr_filename_editor (void)
{

    U8* guiBuffer;
    S32 length =FMGR_MAX_FILE_LEN;
    U16 iconId;
	
    TRACE_FMGR_FUNCTION();
    EntryNewScreen(SCR_FMGR_FILENAME_EDITOR, NULL,
    entry_fmgr_filename_editor, NULL);

    if ( IsScreenPresent(SCR_FMGR_FOLDER_OPTION) || 
    IsScreenPresent(SCR_FMGR_APP_FOLDER_OPTION) ||
    IsScreenPresent(SCR_FMGR_TREE_OPTION) ||
    IsScreenPresent(SCR_FMGR_FILE_OPTION) ||
    IsScreenPresent(SCR_FMGR_APP_FILE_OPTION) )
    {
        /* create under folder or rename, check folder present or not */
        /* create under drive no need to check drive present */
        U8 flag = fmgr_check_file_present();

        if ( flag < FMGR_NO_ERROR )
            return;
#ifndef __MMI_SHOW_FILE_EXT__   /*Enetr rename editor, cut down 5 character of filename,fix bug14379, gdm 090916*/
       if ( flag == FMGR_FILE )
            length -= 5;
#endif
    }

    TRACE_FMGR_FUNCTION();
    if ( pfnUnicodeStrlen((PS8)fmgr_p->new_file_name) >= length )
        memset (&fmgr_p->new_file_name[(length-1)<<1], 0, 
                        sizeof (fmgr_p->new_file_name)-((length-1)<<1));

    guiBuffer = GetCurrGuiBuffer( SCR_FMGR_FILENAME_EDITOR );
    iconId = fmgr_get_app_title_icon_id();

    ShowCategory5Screen(STR_GLOBAL_EDIT, iconId,
    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    (INPUT_TYPE_ALPHANUMERIC_UPPERCASE),
    (PU8)fmgr_p->new_file_name, length, guiBuffer);
#if defined(__MMI_GB__)
    GBSetInputboxLSKFunction(entry_fmgr_editor_option);
#endif
    SetLeftSoftkeyFunction(entry_fmgr_editor_option,KEY_EVENT_UP);
    SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    return;

}

void entry_fmgr_editor_option (void)
{

    U16 nStrItemList[10] = {0};
    U8* guiBuffer;
    U16 nNumofItem = 0, imageId=0;
    //UINT i = 0;
    TRACE_FMGR_FUNCTION();
    TRACE_FMGR_FUNCTION();

    EntryNewScreen(SCR_FMGR_EDITOR_OPTION, NULL,
    entry_fmgr_editor_option, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_EDITOR_OPTION);

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
    nNumofItem = GetNumOfChild(MITEM_FMGR_EDITOR_OPTION);
    GetSequenceStringIds(MITEM_FMGR_EDITOR_OPTION,nStrItemList);
    SetParentHandler(MITEM_FMGR_EDITOR_OPTION);

    imageId = fmgr_get_app_title_icon_id();

    ShowCategory15Screen(STR_GLOBAL_OPTIONS, imageId,
    STR_GLOBAL_OK, IMG_GLOBAL_OK,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    nNumofItem, nStrItemList,
    (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

    RegisterInputMethodScreenCloseFunction(fmgr_return_to_editor);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    return;

}

void entry_fmgr_delete_confirm (void)
{
    TRACE_FMGR_FUNCTION();

#ifdef MMI_ON_HARDWARE_P
    U16 stringId;

    if ( fmgr_check_file_present() < FMGR_NO_ERROR )
    {
        TRACE_FMGR_FUNC("fmgr_check_file_present()<FMGR_NO_ERROR return");
        return;
    }

    TRACE_EF(g_sw_FMGR, "entry_fmgr_delete_confirm : fmgr_p");

    if ( fmgr_p->delete_all == FMGR_DELETE_ALL )
        stringId = STR_FMGR_DELETE_ALL;
    else
        stringId = STR_GLOBAL_DELETE;
    TRACE_FMGR_FUNC("IMG_GLOBAL_NO");
  
    DisplayNewconfirm(
    STR_GLOBAL_YES,IMG_GLOBAL_YES,
    STR_GLOBAL_NO,IMG_GLOBAL_NO,
    get_string(stringId), IMG_GLOBAL_QUESTION, WARNING_TONE,entry_fmgr_delete_confirm);

    TRACE_FMGR_FUNC("STR_GLOBAL_YES");
    if ( fmgr_p->delete_all == FMGR_DELETE_ALL )
    {
        SetLeftSoftkeyFunction(fmgr_delete_all_file,KEY_EVENT_UP);
    }
    else
    {
        SetLeftSoftkeyFunction(fmgr_delete,KEY_EVENT_UP);
    }
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    return;
#else
    PopupNoSupport();
#endif
}

void entry_fmgr_app_drive_option(void)
{
    U16 nStrItemList[10];
    U16 nNumofItem;
    U16 menuId, iconId;
    U8* guiBuffer;

    EntryNewScreen(SCR_FMGR_APP_DRIVE_OPTION, NULL,
    entry_fmgr_app_drive_option, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_APP_DRIVE_OPTION);

    switch ( fmgr_p->src_application )
    {
    #ifdef __MMI_FTS_SUPPORT__
        case APP_FTP:
            menuId = MENU_CONN_FTP_SHARED_FOLDER;
            break;
    #endif /* __MMI_FTS_SUPPORT__ */ 
        default:
            menuId = MITEM_FMGR_APP_TYPE_OPTIONS;
            break;
    }

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
    nNumofItem = GetNumOfChild(menuId);
    GetSequenceStringIds(menuId,nStrItemList);
    SetParentHandler(menuId);
    iconId = fmgr_get_app_title_icon_id ();

    ShowCategory15Screen(STR_GLOBAL_OPTIONS, iconId,
    STR_GLOBAL_OK, IMG_GLOBAL_OK,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    nNumofItem, nStrItemList,
    (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    return;
}

void entry_fmgr_app_folder_option(void)
{
    U16 nStrItemList[10];
    U16 nNumofItem;
    U16 menuId, iconId;
    U8* guiBuffer;
    EntryNewScreen(SCR_FMGR_APP_FOLDER_OPTION, NULL,
    entry_fmgr_app_folder_option, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_APP_FOLDER_OPTION);

    switch ( fmgr_p->src_application )
    {

        default:
            menuId = MITEM_FMGR_APP_TYPE_OPTIONS;
            break;
    }

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
    nNumofItem = GetNumOfChild(menuId);
    GetSequenceStringIds(menuId,nStrItemList);
    SetParentHandler(menuId);
    iconId = fmgr_get_app_title_icon_id();

    ShowCategory15Screen(STR_GLOBAL_OPTIONS, iconId,
    STR_GLOBAL_OK, IMG_GLOBAL_OK,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    nNumofItem, nStrItemList,
    (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);   
    return;
}


void entry_fmgr_app_empty_folder_option(void)
{
    U16 nStrItemList[10];
    U16 nNumofItem;
    U16 menuId=0, iconId;
    U8* guiBuffer;

    EntryNewScreen(SCR_FMGR_APP_EMPTY_FOLDER_OPTION, NULL, entry_fmgr_app_empty_folder_option, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_APP_EMPTY_FOLDER_OPTION);

    switch ( fmgr_p->src_application )
    {
#ifdef __MMI_CAMERA__
        case APP_IMAGEVIEWER:
            menuId = MENU_ID_IMGVIEW_EMPTY_FOLDER_OPTION;
            break;
#endif /* __MMI_CAEMRA__ */

#ifdef __MMI_VIDEO__
        case APP_VDOPLY:
            menuId = MENU_ID_VDOPLY_EMPTY_FOLDER_OPTION;
            break;
#endif /* __MMI_VIDEO__ */

#ifdef __MMI_SOUND_RECORDER__
        case APP_SOUNDRECORDER:
#ifdef MT6205B
            menuId = MENU_ID_SNDREC_OPTION_4;
#else
            menuId = MENU_ID_SNDREC_OPTION_1;
#endif
		mmi_trace(1,"MENU_ID_SNDREC_OPTION_1 is %d,MENU_ID_SNDREC_SETTINGS is %d",MENU_ID_SNDREC_OPTION_1,MENU_ID_SNDREC_SETTINGS);
            break;
#endif // __MMI_SOUND_RECORDER__

#ifdef __MMI_AB_REPEATER__
        case APP_ABREPEATER:
            menuId = MENU_ID_AB_REPEAT_EMPTY_FOLDER_OPTION;
            break;
#endif
        default:
            MMI_ASSERT(0);
            break;
    }

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
    nNumofItem = GetNumOfChild(menuId);
    GetSequenceStringIds(menuId,nStrItemList);
    SetParentHandler(menuId);
    iconId = fmgr_get_app_title_icon_id();

    ShowCategory15Screen(STR_GLOBAL_OPTIONS, iconId,
    STR_GLOBAL_OK, IMG_GLOBAL_OK,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    nNumofItem, nStrItemList,
    (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);   
    return;
}

void entry_fmgr_app_file_option(void)
{

    U16 nStrItemList[10];
    U16 nNumofItem;
    U16 menuId, iconId;
    U8* guiBuffer;
    FMGR_FILE_INFO_STRUCT file_info;

    EntryNewScreen(SCR_FMGR_APP_FILE_OPTION, NULL, entry_fmgr_app_file_option, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_APP_FILE_OPTION);

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);
    menuId = fmgr_get_app_file_option_menu(&file_info);
    switch ( fmgr_p->src_application )
    {
#ifdef __MMI_AB_REPEATER__
        case APP_ABREPEATER:
            menuId = MENU_ID_AB_REPEATER_OPTION;
            break;
#endif
        case APP_RINGTONECOMPOSER:
            menuId = MENU_COMP_RINGTONE_COMP_SELMENU_1;
            break;

#ifdef __MMI_CAMERA__
        case APP_IMAGEVIEWER:
            if ( menuId == MITEM_FMGR_APP_IMG_TYPE_OPTIONS )
                menuId = MENU_ID_IMGVIEW_FILE_OPTION;
            else
                menuId = MENU_ID_IMGVIEW_FILE_OPTION_NO_SIM;
            break;
#endif /* __MMI_CAEMRA__ */

#ifdef __MMI_VIDEO__
        case APP_VDOPLY:
            if ( menuId == MITEM_FMGR_APP_VID_TYPE_OPTIONS )
            {
                BOOL is_drm;
                U16 forward;

                /* check if can foward or not */
                forward = fmgr_get_forward_option_menu(&file_info, &is_drm);
                if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
                {
                    /* cant foward */
                    menuId = MENU_ID_VDOPLY_FILE_OPTION_NO_SIM;
                }
                else
                {
                    if ( GetNumOfChild(forward) == 0 )
                    {
                        /* have foward menu, but no child menu, hide also */
                        menuId = MENU_ID_VDOPLY_FILE_OPTION_NO_SIM;
                    }
                    else
                    {
                        menuId = MENU_ID_VDOPLY_FILE_OPTION;
                    }
                }

            }
            else
            {
                menuId = MENU_ID_VDOPLY_FILE_OPTION_NO_SIM;
            }
            break;
#endif /* __MMI_VIDEO__ */

#ifdef __MMI_SOUND_RECORDER__
        case APP_SOUNDRECORDER:
            menuId = mmi_sndrec_get_menuId();
            break;
#endif // __MMI_SOUND_RECORDER__

#ifdef __MMI_EBOOK_READER__
        case APP_EBOOK:
            menuId = mmi_ebook_get_menuId();
            break;
#endif /* __MMI_EBOOK_READER__ */ 


        default:
            break;
    }

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
    nNumofItem = GetNumOfChild(menuId);
    GetSequenceStringIds(menuId,nStrItemList);
    SetParentHandler(menuId);
    iconId = fmgr_get_app_title_icon_id();

    ShowCategory15Screen(STR_GLOBAL_OPTIONS, iconId,
    STR_GLOBAL_OK, IMG_GLOBAL_OK,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    nNumofItem, nStrItemList,
    (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    return;
}

void entry_fmgr_tree_explorer(void)
{
    U8* guiBuffer;
    U16 count, titleId;
    FMGR_FILTER filter;

    FMGR_FILTER_INIT(&filter);
    FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);

    mmi_fmgr_kernel_init ((PS8)fmgr_p->file_path, filter, &count);
    TRACE_EF(g_sw_FMGR, "entry_fmgr_tree_explorer : filter = FMGR_TYPE_FOLDER,  count = %d.",count);
    if ( count == 0 )
    {
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
        DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 0, 1000, EMPTY_LIST_TONE);
        ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);
        DeleteScreenIfPresent(SCR_FMGR_TREE_OPTION);
        DeleteScreenIfPresent(SCR_FMGR_DRIVE_OPTION);
        fmgr_p->enter_folder = FALSE;
#ifdef __MMI_TOUCH_SCREEN__
        mmi_wgui_reset_list_item_selected_callback();
#endif
        return;
    }

    EntryNewScreen(SCR_FMGR_TREE_EXPLORER, exit_fmgr_tree_explorer,
    NULL, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_TREE_EXPLORER);

    if ( fmgr_p->enter_folder == TRUE )
    {
        /* enter tree explorer screen by user selection, not go back history after interrupt */
        fmgr_p->enter_folder = FALSE;
        DeleteUptoScrID(SCR_FMGR_TREE_EXPLORER);
        /* enter tree explorer screen by user selection shall always highlight first item */
        guiBuffer = NULL;
    }

    if ( GetCurrScrnId() == SCR_FMGR_TREE_DRIVE_OPTION )
        DeleteScreenIfPresent(SCR_FMGR_TREE_DRIVE_OPTION);

    if ( fmgr_p->tree_action == FMGR_ACTION_COPY )
    {
        titleId = STR_FMGR_COPY;
    }
    else
    {
        titleId = STR_FMGR_MOVE;
    }

    RegisterHighlightHandler(highlight_fmgr_tree_node);

    ShowCategory213Screen(  titleId, GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID), /* title			*/
    STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, /* lsk			*/
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,   /* rsk			*/
    count,
    mmi_fmgr_draw_file_menuitem_callback,
    mmi_fmgr_hide_file_menuitem_callback,
    0,              /* hiligth item idx */
    CAT213_ARROW_ALL,
    guiBuffer);

    SetKeyHandler(fmgr_back_to_tree_parent, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(entry_fmgr_tree_option, KEY_EVENT_UP);
    SetKeyHandler(fmgr_set_curr_tree_and_enter, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(fmgr_back_to_tree_parent, KEY_EVENT_UP);
#ifdef __MMI_TOUCH_SCREEN__
    mmi_wgui_reset_list_item_selected_callback();
#endif
    return;

}

void exit_fmgr_tree_explorer(void)
{
    if ( IsScreenPresent(SCR_FMGR_TREE_EXPLORER) == FALSE )
    {
        history_t currHistory;
        S16     nHistory    = 0;

        currHistory.scrnID = SCR_FMGR_TREE_EXPLORER;
        currHistory.entryFuncPtr = entry_fmgr_tree_explorer;
        pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
        GetCategoryHistory( currHistory.guiBuffer );
        AddHistory( currHistory );
    }
    else
    {
        U8* guiBuffer;

        guiBuffer = GetCurrGuiBuffer(SCR_FMGR_TREE_EXPLORER);
        GetCategoryHistory( guiBuffer );
    }
}

void entry_fmgr_tree_option(void)
{
    U16 nStrItemList[10];
    U8* guiBuffer;
    U16 nNumofItem;

    EntryNewScreen(SCR_FMGR_TREE_OPTION, NULL,
    entry_fmgr_tree_option, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_TREE_OPTION);

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
    nNumofItem = GetNumOfChild(MITEM_FMGR_TREE_OPTIONS);
    GetSequenceStringIds(MITEM_FMGR_TREE_OPTIONS,nStrItemList);
    SetParentHandler(MITEM_FMGR_TREE_OPTIONS);

    ShowCategory15Screen(STR_GLOBAL_OPTIONS, GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID),
    STR_GLOBAL_OK, IMG_GLOBAL_OK,
    STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
    nNumofItem, nStrItemList,
    (U16 *)gIndexIconsImageList, 1, 0,guiBuffer );

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    return;
}

/*****************************************************************************
* FUNCTION
*	fmgr_get_forward_option_menu
* DESCRIPTION
*	get forward option menu id for highlighted file
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U16 fmgr_get_forward_option_menu (FMGR_FILE_INFO_STRUCT *file_info, BOOL *is_drm)
{
	BOOL isSimValid = MMI_FALSE;
	U16 i;
#ifdef __DRM_SUPPORT__
    U8 drm_forward=FMGR_DRM_FWD_APP_FWD_OUT;   /* 0: can't forward at all, 
                1: can forward to app, can't forward off device, 
                2: can't forward to app, can forward off device,
                3: can forward to app and off device */

    drm_forward = fmgr_get_drm_forward_rule (file_info, is_drm);
    PRINT_INFORMATION (("*** fmgr_get_forward_option_menu, rule=%d, is_drm=%d\n", forward, is_drm));
    if ( drm_forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
        return 0;
#else
    if ( is_drm != NULL )
        *is_drm = FALSE;
#endif
        	mmi_trace(g_sw_BT, "fmgr_get_forward_option_menu::MENU_FMGR_FWD_OPTION_GEN=%d", MENU_FMGR_FWD_OPTION_GEN);
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].IsSimValid == 1)
		{
			isSimValid = MMI_TRUE;
			break;
		}
	}
    if ( FMGR_FILTER_IS_SET_IMAGE(&file_info->file_type) )
    {
        /* can forward to wallpaper, screensaver, phonebook */
	if(isSimValid		
#ifdef __FLIGHT_MODE_SUPPORT__      	
        && mmi_bootup_get_active_flight_mode() == 0
#endif      	
        )
        {
#ifdef __DRM_SUPPORT__      
            if ( drm_forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
                return 0;
            if ( drm_forward == FMGR_DRM_FWD_APP_NO_FWD_OUT )
                return MENU_FMGR_FWD_OPTION_IMG_DRM;
            if ( drm_forward == FMGR_DRM_NO_FWD_APP_FWD_OUT )
                return MENU_FMGR_FWD_OPTION_GEN;
#endif            
            return MENU_FMGR_FWD_OPTION_IMG;
        }
        else  /* image can't forward when no sim */
            return MENU_FMGR_FWD_OPTION_IMG;    //chenqiang fix 12221
    }
    else if ( FMGR_FILTER_IS_SET_AUDIO(&file_info->file_type) )
    {
        /* can forward to profile */
	if(isSimValid	
#ifdef __FLIGHT_MODE_SUPPORT__      	
        && mmi_bootup_get_active_flight_mode() == 0
#endif      	
        )
        {
#ifdef __DRM_SUPPORT__      
            if ( drm_forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
                return 0;
            if ( drm_forward == FMGR_DRM_FWD_APP_NO_FWD_OUT )
                return MENU_FMGR_FWD_OPTION_AUD_NO_SIM;
            if ( drm_forward == FMGR_DRM_NO_FWD_APP_FWD_OUT )
                return MENU_FMGR_FWD_OPTION_GEN;
#endif            
            return MENU_FMGR_FWD_OPTION_AUD;
        }
        else
        {
#ifdef __DRM_SUPPORT__
            if ( drm_forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT || 
            drm_forward == FMGR_DRM_NO_FWD_APP_FWD_OUT )
                return 0;
#endif
            return MENU_FMGR_FWD_OPTION_AUD_NO_SIM;
        }
    }
#ifdef __MMI_VIDEO__
    else if ( FMGR_FILTER_IS_SET_VIDEO(&file_info->file_type) )
    {
        /* can forward to app */
	if((isSimValid)		
#ifdef __FLIGHT_MODE_SUPPORT__      	
        && mmi_bootup_get_active_flight_mode() == 0
#endif      	      	
        )
        {
#ifdef __DRM_SUPPORT__      
            if ( drm_forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT || 
            drm_forward == FMGR_DRM_FWD_APP_NO_FWD_OUT )
                return 0;
#endif 
            return MENU_FMGR_FWD_OPTION_VID;
        }
        else
            return MENU_FMGR_FWD_OPTION_VID_NO_SIM;
    }
#endif   
#ifdef __MMI_VCALENDAR__
    else if ( FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_VCS) )
    {
        /* can forward to todolist */
	if((isSimValid)		
#ifdef __FLIGHT_MODE_SUPPORT__      	
        && mmi_bootup_get_active_flight_mode() == 0
#endif      	      	
        )
            return MENU_FMGR_FWD_OPTION_VCAL;
        else
            return MENU_FMGR_FWD_OPTION_VCAL_NO_SIM;
    }
#endif   
#ifdef __MMI_VCARD__
    else if ( FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_VCF) )
    {
        /* can forward to phonebook */
	if((isSimValid)		
#ifdef __FLIGHT_MODE_SUPPORT__      	
        && mmi_bootup_get_active_flight_mode() == 0
#endif      	      	
        )
            //return MENU_FMGR_FWD_OPTION_VCARD;
            return MENU_FMGR_FWD_OPTION_GEN;
        else
            return 0;         
    }
#endif   
    else
    {
        /* unrecognized file type, can only forward off device */
	if((isSimValid)		
#ifdef __FLIGHT_MODE_SUPPORT__      	
        && mmi_bootup_get_active_flight_mode() == 0
#endif      	      	
        )
        {
#ifdef __DRM_SUPPORT__
            if ( drm_forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT || 
            drm_forward == FMGR_DRM_FWD_APP_NO_FWD_OUT )
                return 0;
#endif
            return MENU_FMGR_FWD_OPTION_GEN;
        }
        else
        {
              if( mmi_bootup_get_active_flight_mode() == 0) 
	              return MENU_FMGR_FWD_OPTION_GEN;
		else 
		{
	        	mmi_trace(g_sw_BT, "fmgr_get_forward_option_menu::return 0?");

	            return 0;
		}
        }
    }
}

/*****************************************************************************
* FUNCTION
*	fmgr_get_file_option_menu
* DESCRIPTION
*	get file option menu id for highlighted file
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U16 fmgr_get_file_option_menu (FMGR_FILE_INFO_STRUCT *file_info)
{
    U16 forward;
    BOOL is_drm = FALSE;

    forward = fmgr_get_forward_option_menu(file_info, &is_drm);
    TRACE_EF(g_sw_FMGR, "---------fmgr_get_forward_option_menu: forward= %d", forward);

    if ( (forward != FMGR_DRM_NO_FWD_APP_NO_FWD_OUT) &&
    (GetNumOfChild(forward) == 0) )
        forward = FMGR_DRM_NO_FWD_APP_NO_FWD_OUT;
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
    if ( FMGR_FILTER_IS_SET(&file_info->file_type,FMGR_TYPE_THEME) )
    {
        if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
            return MITEM_FMGR_THEME_TYPE_OPTIONS;    /* view only */
        else
            return MITEM_FMGR_THEME_TYPE_OPTIONS;     /* view + forward */
    }
#endif
    if ( FMGR_FILTER_IS_SET_IMAGE(&file_info->file_type) )
    {
#ifdef __DRM_V01__
        if ( pfnUnicodeStrncmp ((PS8)fmgr_p->file_path, (PS8)DRM_NOR_PATH, 
        pfnUnicodeStrlen((PS8)DRM_NOR_PATH)) == 0 )
            return MITEM_FMGR_DRM_IMG_TYPE_OPTIONS;
        else
#endif
            if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
            mmi_frm_hide_menu_item(MENU_FMGR_GEN_OPTION_FORWARD);    /* view only */
        //else
        {
#ifdef __MMI_PHOTOEDITOR__      
            if ( (is_drm == TRUE) ||
            (FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_JPG) == FALSE &&
            FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_JPEG) == FALSE) )
                mmi_frm_hide_menu_item(MENU_FMGR_IMG_OPTION_EDIT);
#endif            
        }
        return MITEM_FMGR_JPG_IMG_TYPE_OPTIONS;
    }
    else if ( FMGR_FILTER_IS_SET_AUDIO(&file_info->file_type) )
    {
#ifdef __DRM_V01__
        if ( pfnUnicodeStrncmp ((PS8)fmgr_p->file_path, (PS8)DRM_NOR_PATH, 
        pfnUnicodeStrlen((PS8)DRM_NOR_PATH)) == 0 )
            return MITEM_FMGR_DRM_AUD_TYPE_OPTIONS;
        else
#endif
            if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
            return MITEM_FMGR_AUD_TYPE_NO_FWD_OPTIONS;    /* play only */
        else
            return MITEM_FMGR_AUD_TYPE_OPTIONS;       /* play + forward */
    }
#ifdef __MMI_VIDEO__
    else if ( FMGR_FILTER_IS_SET_VIDEO(&file_info->file_type) )
    {
        if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
            return MITEM_FMGR_VID_TYPE_NO_SIM_OPTIONS;  /* play only */
        else
            return MITEM_FMGR_VID_TYPE_OPTIONS;       /* play + forward */
    }
#endif
#ifdef __MMI_EBOOK_READER__
    else if ((FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_EBOOK_TXT)) ||
        (FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_EBOOK_PDB)))
    {
		if (forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT)
        {
			return MITEM_FMGR_EBOOK_NO_FWD_TYPE_OPTIONS;  /* no forward */
		}
		else
		{
        return MITEM_FMGR_EBOOK_TYPE_OPTIONS;   /* view + forward */
    }
    }
#endif /* __MMI_EBOOK_READER__ */ 
#if (defined ( __J2ME__	) && !defined (__MMI_FMGR_DISABLE_JAVA_INSTALL__))
    else if ( FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_JAD) ||
    FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_JAR) )
    {
#ifdef __DRM_V01__
        if ( pfnUnicodeStrncmp ((PS8)fmgr_p->file_path, (PS8)DRM_NOR_PATH, 
        pfnUnicodeStrlen((PS8)DRM_NOR_PATH)) == 0 )
            return MITEM_FMGR_DRM_EXE_TYPE_OPTIONS;
        else
#endif
            if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
            return MITEM_FMGR_EXE_TYPE_NO_SIM_OPTIONS;  /* exe only */
        else
            return MITEM_FMGR_EXE_TYPE_OPTIONS;       /* exe + forward */
    }
#endif
    else
    {
        if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
            return MITEM_FMGR_GEN_TYPE_NO_SIM_OPTIONS;   /* no forward */
        else
            return MITEM_FMGR_GEN_TYPE_OPTIONS;     /* forward only */
    }
}

/*****************************************************************************
* FUNCTION
*	fmgr_get_app_file_option_menu
* DESCRIPTION
*	get file option menu id for highlighted file
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U16 fmgr_get_app_file_option_menu (FMGR_FILE_INFO_STRUCT *file_info)
{
    U16 forward;
    BOOL is_drm = FALSE;

    forward = fmgr_get_forward_option_menu(file_info, &is_drm);
 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
    if ( FMGR_FILTER_IS_SET(&file_info->file_type,FMGR_TYPE_THEME) )
    {
        if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
            return MITEM_FMGR_THEME_TYPE_OPTIONS;    /* view only */
        else
            return MITEM_FMGR_THEME_TYPE_OPTIONS;     /* view + forward */
    }
#endif

    if ( (forward != FMGR_DRM_NO_FWD_APP_NO_FWD_OUT) &&
    (GetNumOfChild(forward) == 0) )
        forward = FMGR_DRM_NO_FWD_APP_NO_FWD_OUT;

    if ( FMGR_FILTER_IS_SET_IMAGE(&file_info->file_type) )
    {
        if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
            return MITEM_FMGR_DRM_IMG_TYPE_OPTIONS;   /* view only */
        else
            return MITEM_FMGR_APP_IMG_TYPE_OPTIONS;       /* view + forward */
    }
    else if ( FMGR_FILTER_IS_SET_AUDIO(&file_info->file_type) )
    {
        if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
            return MITEM_FMGR_DRM_AUD_TYPE_OPTIONS;     /* play only */
        else
            return MITEM_FMGR_APP_AUD_TYPE_OPTIONS;       /* play + forward */
    }
#ifdef __MMI_VIDEO__
    else if ( FMGR_FILTER_IS_SET_VIDEO(&file_info->file_type) )
    {
        //if (forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT)
        return MITEM_FMGR_APP_VID_TYPE_OPTIONS;   /* play only */
        //else
        //return MITEM_FMGR_APP_VID_TYPE_OPTIONS;                   /* play + forward */
    }
#endif
#if (defined ( __J2ME__	) && !defined (__MMI_FMGR_DISABLE_JAVA_INSTALL__))
    else if ( FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_JAD) ||
    FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_JAR) )
    {
        if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
            return MITEM_FMGR_DRM_EXE_TYPE_OPTIONS;   /* exe only */
        else
            return MITEM_FMGR_APP_EXE_TYPE_OPTIONS;       /* exe + forward */
    }
#endif
#ifdef __MMI_EBOOK_READER__
    else if ((FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_EBOOK_TXT)) ||
        (FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_EBOOK_PDB)))
    {
		if (forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT)
        {
			return MITEM_FMGR_APP_EBOOK_NO_FWD_TYPE_OPTIONS;  /* no forward */
		}
		else
		{
            return MITEM_FMGR_APP_EBOOK_TYPE_OPTIONS;   /* view + forward */
        }
    }
#endif /* __MMI_EBOOK_READER__ */ 
    else
    {
        if ( forward == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT )
            return MITEM_FMGR_DRM_GEN_TYPE_OPTIONS;    /* no forward */
        else
            return MITEM_FMGR_APP_TYPE_OPTIONS;     /* forward only */
    }   
}


/*****************************************************************************
* FUNCTION
*	mmi_fmgr_view_detail_press_up
* DESCRIPTION
*  press up in view detail screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_fmgr_view_detail_press_up(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32							index;
	S32							cur_index;
	FMGR_FILE_INFO_STRUCT	file_info;
	S32							counter;
	S32							file_count;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* exit and clear previous screen for history - then enter again */
	cur_index	= mmi_fmgr_kernel_get_cur_idx();
	file_count	= mmi_fmgr_kernel_get_file_count();
	counter		= 0;


	if(file_count > 1)
	{
		do{
			
			mmi_fmgr_kernel_move_selection_up();
			index = mmi_fmgr_kernel_get_cur_idx();
			mmi_fmgr_kernel_get_file_info(index, &file_info);

			counter++;


			if(!(file_info.attribute & FS_ATTR_DIR))
			{
				EntryNewScreen(SCR_ID_FMGR_VIEW_IMAGE, NULL, NULL, NULL);

				DeleteScreenIfPresent(SCR_ID_FMGR_VIEW_IMAGE);

				fmgr_view_image_hdlr();
				return;
			}


			if(counter >= (file_count-1))
				return;
	

		} while(1);
	}

}



/*****************************************************************************
* FUNCTION
*	mmi_fmgr_view_detail_press_down
* DESCRIPTION
*  press down in view detail screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_fmgr_view_detail_press_down(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32							index;
	S32							cur_index;
	FMGR_FILE_INFO_STRUCT	file_info;
	S32							counter;
	S32							file_count;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* exit and clear previous screen for history - then enter again */
	cur_index	= mmi_fmgr_kernel_get_cur_idx();
	file_count	= mmi_fmgr_kernel_get_file_count();
	counter		= 0;


	if(file_count > 1)
	{
		do{
		
			mmi_fmgr_kernel_move_selection_down();
			index = mmi_fmgr_kernel_get_cur_idx();
			mmi_fmgr_kernel_get_file_info(index, &file_info);

			counter++;

			if(!(file_info.attribute & FS_ATTR_DIR))
			{
				EntryNewScreen(SCR_ID_FMGR_VIEW_IMAGE, NULL, NULL, NULL);

				DeleteScreenIfPresent(SCR_ID_FMGR_VIEW_IMAGE);
				
				fmgr_view_image_hdlr();

				return;
			}


			if(counter >= (file_count-1))
				return;
	

		} while(1);
	}
}

void fmgr_view_image_hdlr (void)
{
    FMGR_FILE_INFO_STRUCT file_info;
	U16							total_count;
	S8							buffer[32];				/* for xxxx/xxxx */
	S8							Count_buffer[64];				/* for xxxx/xxxx */
	S8 view_filepath[FMGR_PATH_BUFFER_SIZE];
	

    EntryNewScreen(SCR_ID_FMGR_VIEW_IMAGE, NULL, fmgr_view_image_hdlr, NULL);
#if 0
    if ( fmgr_check_file_present() < FMGR_NO_ERROR )
    {
		DeleteScreenIfPresent(SCR_ID_FMGR_VIEW_IMAGE);
	    GoBackHistory();
        return;
    }
#endif
    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

#if 0
    if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE
#ifndef __MMI_SHOW_FILE_EXT__
    || mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == FALSE
#endif
    )
        return;
#endif
	mmi_fmgr_kernel_get_path_name(view_filepath);
   if( (pfnUnicodeStrlen(view_filepath)+pfnUnicodeStrlen(file_info.file_name))*2 
   	   >= FMGR_PATH_BUFFER_SIZE )
   	{
   	    DisplayPopup((PU8)GetString(STR_ID_FMGR_FILENAME_TOO_LONG), IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);//STR_ID_FMGR_FILENAME_TOO_LONG
		DeleteNHistory(1);
		return;
     }
   else
	  pfnUnicodeStrcat(view_filepath, file_info.file_name);
	total_count = mmi_fmgr_kernel_get_file_count();

	sprintf(buffer, "%d/%d", fmgr_p->cur_index+1, total_count);
	AnsiiToUnicodeString(Count_buffer, (PS8)buffer);

#ifdef __DRM_SUPPORT__
    {
        U8 result;

        result = fmgr_check_drm_rights(NULL, DRM_PERMISSION_DISPLAY);
        if ( result == FMGR_DRM_NO_PERMISSION )
        {
            DisplayPopup((PU8)GetString(STR_GLOBAL_DRM_PROHIBITED), IMG_GLOBAL_ERROR,
            0, 1000, ERROR_TONE);
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);         
            return;
        }
        else if ( result == FMGR_DRM_REQUEST_RIGHTS )
        {
            entry_fmgr_request_rights_confirm();
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
            return;
        }
    }
#endif /*__DRM_SUPPORT__*/


#ifdef __MMI_MESSAGES_EMS__
    if ( FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_EMS) )
        ShowCategory144Screen (0, GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID),
        0, 0, 
        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
        EMS_OBJECT_MY_PICTURE_STATE|CATEGORY144_FMGR_PREVIEW, 1,
        NULL, (U16*)&view_filepath,//fmgr_p->file_path, file_info.is_short, NULL,
        0, NULL);
    else if ( FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_ANM) )
        ShowCategory144Screen (0, GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID),
        0, 0, 
        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
        EMS_OBJECT_MY_ANIMATION_STATE|CATEGORY144_FMGR_PREVIEW, 1,
        NULL, (U16*)&view_filepath,//fmgr_p->file_path, file_info.is_short, NULL,
        0, NULL);
    else
#endif	   
        /* entry cat222 to decode and display a image from file */
        ShowCategory222Screen(  0,            /* title */
        GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID),  /* title icon */
        0,  0,           /* left soft key */
        STR_GLOBAL_BACK, IMG_GLOBAL_BACK, /* right soft key */
        GDI_COLOR_WHITE,       /* content BG color */
        (PS8)Count_buffer,         /* button bar string */
        (PS8)view_filepath,//fmgr_p->file_path,     /* image file name */
        file_info.is_short,      /* is_short */
        NULL);          /* result callback */


    ClearAllKeyHandler();
//wug del (// #ifdef MJPG_SUPPORT) used to cs200 start 20070821
// #ifdef MJPG_SUPPORT
	if(total_count > 1)
	{
		SetKeyHandler(mmi_fmgr_view_detail_press_up,		KEY_UP_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(mmi_fmgr_view_detail_press_down,	KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	}
//#endif
//wug del (// #ifdef MJPG_SUPPORT) used to cs200 end 20070821
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

 //   mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
    /* pass file name to image viewer
       hook image view func */
}

void fmgr_edit_image_hdlr (void)
{
#ifdef __MMI_PHOTOEDITOR__
    FMGR_FILE_INFO_STRUCT file_info;

    if ( fmgr_check_file_present() < FMGR_NO_ERROR )
        return;

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

    if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE
#ifndef __MMI_SHOW_FILE_EXT__
    || mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == FALSE
#endif
    )
        return;

    /* hook to photo art application */
    mmi_phoart_entry_from_fmgr(fmgr_p->file_path, file_info.is_short);

    mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
#endif   
}

void fmgr_play_audio_hdlr(void)
{
	FMGR_FILE_INFO_STRUCT file_info;
    BOOL silent, preview;

#ifdef __MMI_SILENT_MEETING_PROFILE__
    silent = IsSilentModeActivated() || IsMeetingModeActivated();
#else
    silent = IsSilentModeActivated();
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
    preview = (GetExitScrnID() != SCR_FMGR_FILE_OPTION && GetExitScrnID() != SCR_FMGR_APP_FILE_OPTION);

	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(fmgr_play_audio_hdlr);
		if(!preview)g_VM_Interrupt = TRUE;
		return;
	}

    if(g_VM_Interrupt)
   {
       preview = FALSE;
        g_VM_Interrupt = FALSE;	
   }
	
    if ( fmgr_check_file_present() < FMGR_NO_ERROR )
    {
        return;
    }

    /* pass file name to audio player
       hook audio player func */
    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);
    if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE
#ifndef __MMI_SHOW_FILE_EXT__
    || mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == FALSE
#endif
    )
        return;

#ifdef __DRM_SUPPORT__
    if ( !(preview && silent) )
    {
        U8 result;

        result = fmgr_check_drm_rights(NULL, DRM_PERMISSION_PLAY);
        if ( result == FMGR_DRM_NO_PERMISSION )
        {
            DisplayPopup((PU8)GetString(STR_GLOBAL_DRM_PROHIBITED), IMG_GLOBAL_ERROR,
            0, 1000, ERROR_TONE);
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
            return;
        }
        else if ( result == FMGR_DRM_REQUEST_RIGHTS )
        {
            entry_fmgr_request_rights_confirm();
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
            return;
        }
    }
#endif /*__DRM_SUPPORT__*/

    if ( !preview )
    {
#ifdef __MMI_AUDIO_PLAYER__

        mmi_audply_single_play((PU16)fmgr_p->file_path, NULL);
        
#endif
    }
    else if ( !silent )
    {
        fmgr_p->audio_state = TRUE;
#ifdef __MMI_AUDIO_PLAYER__
#ifdef MMI_ON_HARDWARE_P
        mdi_audio_play_file((PU16)fmgr_p->file_path,
        DEVICE_AUDIO_PLAY_INFINITE, NULL, NULL);
#else
        mdi_audio_play_file_with_vol_path((PU16)fmgr_p->file_path,
        DEVICE_AUDIO_PLAY_INFINITE, NULL, NULL,
        GetRingVolumeLevel(), MDI_DEVICE_SPEAKER2,0);
#endif
#endif
    }
    mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
}


void fmgr_play_video_hdlr(void)
{
	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(fmgr_play_video_hdlr);
		return;
	}
#ifdef __MMI_VIDEO__
    mmi_vdoply_entry_player_screen();
#endif /* __MMI_VIDEO__ */
}


void fmgr_execute_hdlr (void)
{
#ifdef __J2ME__
    FMGR_FILE_INFO_STRUCT file_info;

    if ( fmgr_check_file_present() < FMGR_NO_ERROR )
        return;

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);
    if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE
#ifndef __MMI_SHOW_FILE_EXT__      
    || mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == FALSE
#endif      
    )
        return;

#ifdef __DRM_SUPPORT__
    {
        U8 result;

        result = fmgr_check_drm_rights(NULL, DRM_PERMISSION_EXECUTE);
        if ( result == FMGR_DRM_NO_PERMISSION )
        {
            DisplayPopup((PU8)GetString(STR_GLOBAL_DRM_PROHIBITED), IMG_GLOBAL_ERROR,
            0, 1000, ERROR_TONE);
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
            return;
        }
        else if ( result == FMGR_DRM_REQUEST_RIGHTS )
        {
            entry_fmgr_request_rights_confirm();
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
            return;
        }
    }
#endif /*__DRM_SUPPORT__*/

    mmi_java_local_install_content_routing((kal_uint8*) fmgr_p->file_path);
    mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
    /* pass file name to java
       hook java func */
#endif // __J2ME__
}

void fmgr_drive_action_hdlr (void)
{
    TRACE_FMGR_FUNCTION();
#ifdef MMI_ON_HARDWARE_P	
    U8 index;

    index = fmgr_get_drive_index(fmgr_p->cur_index);

    TRACE_EF(g_sw_FMGR, "fmgr_get_drive_index  index=%d", index);
    switch ( fmgr_p->drive_action )
    {
        case FMGR_ACTION_FORMAT:
            TRACE_FMGR_FUNCTION();
            fmgr_send_format_req((U8)fmgr_p->drv_list[index][0], FS_FORMAT_HIGH,
            (PsFuncPtr)fmgr_drive_rsp_hdlr);
            TRACE_EF(g_sw_FMGR, "fmgr_drive_action_hdlr fmgr_p->drv_list[index][0] =%c",fmgr_p->drv_list[index][0]);
            break;
        case FMGR_ACTION_CHECK_DRIVE:
            TRACE_FMGR_FUNCTION();
            fmgr_send_check_drive_req((U8)fmgr_p->drv_list[index][0],
            (PsFuncPtr)fmgr_drive_rsp_hdlr);
            break;
        case FMGR_ACTION_UNMOUNT:
            TRACE_FMGR_FUNCTION();
            fmgr_send_unmount_req(0, FS_NONBLOCK_MODE, (PsFuncPtr)fmgr_drive_rsp_hdlr);
            break;
        default:
            return;
    }
    entry_fmgr_drive_animation();
#else
    DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_AVAILABLE), IMG_GLOBAL_ERROR, 
    0, 1000, WARNING_TONE);
#endif
}

void fmgr_copy_hdlr (void)
{
    TRACE_FMGR_FUNCTION();
#ifdef MMI_ON_HARDWARE_P	
    fmgr_p->tree_action = FMGR_ACTION_COPY;
    fmgr_aux_copy_hdlr();
#else
    DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_AVAILABLE), IMG_GLOBAL_ERROR, 
    0, 1000, WARNING_TONE);
#endif   
}

void fmgr_move_hdlr (void)
{
    TRACE_FMGR_FUNCTION();
#ifdef MMI_ON_HARDWARE_P	
    fmgr_p->tree_action = FMGR_ACTION_MOVE;
    fmgr_aux_copy_hdlr();
#else
    DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_AVAILABLE), IMG_GLOBAL_ERROR, 
    0, 1000, WARNING_TONE);
#endif   
}

void fmgr_aux_copy_hdlr(void)
{
    FMGR_FILE_INFO_STRUCT file_info;
    S32 count;
    TRACE_FMGR_FUNCTION();

    if ( fmgr_check_file_present() < FMGR_NO_ERROR )
    {
        fmgr_p->enter_folder = FALSE;
        FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
        fmgr_p->tree_action = FMGR_ACTION_NONE;
        return;
    }

    /* copy file */
    /* set default copy path */
    TRACE_FMGR_FUNCTION();
    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

    pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)file_info.file_name);
#ifndef __MMI_SHOW_FILE_EXT__
    pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)file_info.file_ext);
#endif

    pfnUnicodeStrcpy((PS8)fmgr_p->old_file_path, (PS8)fmgr_p->file_path);

    /* short file name src file should be renamed to long file name */
#ifdef MMI_ON_HARDWARE_P
    if ( file_info.is_short == TRUE )
    {
        PS8 temp, temp1;
        S32 fs_ret;

        temp = mmi_fmgr_extract_file_name((PS8)fmgr_p->file_path);
        temp1 = mmi_fmgr_extract_file_name((PS8)fmgr_p->old_file_path);
        mmi_chset_mixed_text_to_ucs2_str((U8*)temp1, (MAX_SUBMENU_CHARACTERS-1) * ENCODING_LENGTH, (U8*)temp, PhnsetGetDefEncodingType() );
        fs_ret = MMI_FS_Rename(fmgr_p->file_path, (PU8)fmgr_p->old_file_path);
        if ( fs_ret < FS_NO_ERROR )
        {
            DisplayPopup((PU8)GetString(GetFileSystemErrorString(fs_ret)),
            IMG_GLOBAL_EMPTY, 0, 1000, ERROR_TONE);
            return;
        }
    }
#endif
    mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);

    while ( pfnUnicodeStrlen((PS8)fmgr_p->file_path) >= 3 )
    {
        count = MMI_FS_Count((PU8)fmgr_p->file_path, FS_DIR_TYPE | FS_FILTER_SYSTEM_ATTR);
        if ( count < 0 )
        {
//
//             UnicodeToAnsii((PS8)nPrintableStr, (PS8)fmgr_p->file_path);
//             TRACE_EF(g_sw_FMGR,"copy fail: %s conut = %d", nPrintableStr, count);
//

            DisplayPopup((PU8)GetString(GetFileSystemErrorString(count)),
            IMG_GLOBAL_EMPTY, 0, 1000, ERROR_TONE);
            return;
        }
        else if ( count == 0 )
        {
            if ( pfnUnicodeStrlen((PS8)fmgr_p->file_path) == 3 )
            {
                /* no folder under drive */
                entry_fmgr_main();
                return;
            }
            else
                mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
        }
        else
            break;
    }
    entry_fmgr_tree_explorer();
}

void fmgr_abort_copy_select(void)
{
    fmgr_p->enter_folder = FALSE;
    FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
    fmgr_p->tree_action = FMGR_ACTION_NONE;
    mmi_fmgr_remove_last_dir((PS8)fmgr_p->old_file_path);
    pfnUnicodeStrcpy((PS8)fmgr_p->file_path, (PS8)fmgr_p->old_file_path);
    GoBackToHistory(SCR_FMGR_EXPLORER);
}

void fmgr_set_curr_path_and_enter (void)
{
    FMGR_FILE_INFO_STRUCT file_info;

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

    if ( pfnUnicodeStrcmp((PS8)file_info.file_name, (PS8)L".") == 0 )
    {
        return;
    }

    if ( pfnUnicodeStrcmp((PS8)file_info.file_name, (PS8)L"..") == 0 )
    {
        fmgr_back_to_parent();
        return;
    }
    TRACE_FMGR_FUNCTION();
    if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE ||
    mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)L"/") == FALSE )
    {
        return;
    }

    if ( (fmgr_p->src_application != APP_FILEMANAGER) &&
    (fmgr_p->sel_type == FMGR_SELECT_FOLDER) &&
    (MMI_FS_Count((PU8)fmgr_p->file_path, FS_DIR_TYPE | FS_FILTER_SYSTEM_ATTR)) == 0 )
    {
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);

        DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 0,
        1000, EMPTY_LIST_TONE);
        DeleteUptoScrID(SCR_FMGR_EXPLORER);
        return;
    }

    if ( pfnUnicodeStrlen((PS8)fmgr_p->file_path) + 3 >= FMGR_MAX_PATH_LEN )
    {
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
        DisplayPopup((PU8)GetString(FMGR_FS_PATH_OVER_LEN_ERROR_TEXT), IMG_GLOBAL_ERROR, 0,
        1000, ERROR_TONE);
        DeleteUptoScrID(SCR_FMGR_EXPLORER);
        return;
    }
    fmgr_p->enter_folder = TRUE;
    entry_fmgr_explorer();
}

void fmgr_back_to_parent (void)
{
    PS8 ptr;

    if ( pfnUnicodeStrlen((PS8)fmgr_p->file_path) <= 3 )
    {
        if ( fmgr_p->src_application == APP_FILEMANAGER &&
        IsScreenPresent(SCR_FMGR_MAIN) )
        {
            GoBackToHistory(SCR_FMGR_MAIN);
        }
        else
        {
            entry_fmgr_main();
        }
        return;
    }

    /* remove last level of folder, find last back slash character in the path string */
    fmgr_p->enter_folder = TRUE;
    ptr = mmi_fmgr_extract_file_name((PS8)fmgr_p->file_path);
    if ( ptr != NULL )
    {
        S32 end;

        end = pfnUnicodeStrlen(ptr)-1;
        if ( ptr[end<<1] == '/' && ptr[(end<<1)+1] == 0 )
            ptr[end<<1] = 0;
        pfnUnicodeStrcpy((PS8)fmgr_p->parent_name, ptr);
        ptr[0] = 0;
        ptr[1] = 0;
    }
    else
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
    if ( IsScreenPresent(SCR_FMGR_EXPLORER) )
        GoBackToHistory(SCR_FMGR_EXPLORER);
    else
        entry_fmgr_explorer();
}

void fmgr_set_curr_tree_and_enter (void)
{
    if ( GetExitScrnID() != SCR_FMGR_TREE_DRIVE_OPTION )
    {
        FMGR_FILE_INFO_STRUCT file_info;

        mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

        if ( pfnUnicodeStrcmp((PS8)file_info.file_name, (PS8)L".") == 0 )
            return;

        if ( pfnUnicodeStrcmp((PS8)file_info.file_name, (PS8)L"..") == 0 )
        {
            fmgr_back_to_tree_parent();
            return;
        }

        if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE ||
        mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)L"/") == FALSE )
        {
            pfnUnicodeStrcpy((S8*)fmgr_p->file_path, (S8*)fmgr_p->old_file_path);
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
            FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
            fmgr_p->tree_action = FMGR_ACTION_NONE;
            fmgr_p->enter_folder = FALSE;
            DeleteUptoScrID(SCR_FMGR_EXPLORER);
            return;
        }

        if ( MMI_FS_Count((PU8)fmgr_p->file_path, FS_DIR_TYPE | FS_FILTER_SYSTEM_ATTR) == 0 )
        {
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
            DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 0,
            1000, EMPTY_LIST_TONE);
            DeleteUptoScrID(SCR_FMGR_TREE_EXPLORER);
            return;
        }

        if ( pfnUnicodeStrlen((PS8)fmgr_p->file_path) + 3 > FMGR_MAX_PATH_LEN )
        {
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
            DisplayPopup((PU8)GetString(FMGR_FS_PATH_OVER_LEN_ERROR_TEXT), IMG_GLOBAL_ERROR, 0,
            1000, ERROR_TONE);
            DeleteUptoScrID(SCR_FMGR_TREE_EXPLORER);
            return;
        }
    }

    fmgr_p->enter_folder = TRUE;
    entry_fmgr_tree_explorer();
}

void fmgr_back_to_tree_parent (void)
{
    if ( pfnUnicodeStrlen((PS8)fmgr_p->file_path) <= 3 )
    {
        DeleteUptoScrID(SCR_FMGR_EXPLORER);
        entry_fmgr_main();
        return;
    }

    fmgr_p->enter_folder = TRUE;
    mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
#if 0
    if ( IsScreenPresent(SCR_FMGR_TREE_EXPLORER) )
        GoBackToHistory(SCR_FMGR_TREE_EXPLORER);
    else
#endif        
        entry_fmgr_tree_explorer();
}

void fmgr_create_folder (void)
{
    FMGR_FILE_INFO_STRUCT file_info;
    S32 fs_ret, len;
    S8 *ptr;
    TBM_ENTRY(0x2849);
    TRACE_FMGR_FUNCTION();


    ptr = mmi_fmgr_skip_space((PS8)fmgr_p->new_file_name);

    len = pfnUnicodeStrlen(ptr);

    if ( len==0 ||(ptr[0] == '.' && ptr[1] == 0 ))
    {
        if ( ptr[0] == '.' )
            DisplayPopup((PU8)GetString(FMGR_FS_INVALID_FILENAME_TEXT),
            IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
        else
            DisplayPopup((PU8)GetString(STR_FMGR_EMPTY_FILENAME),
            IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
        DeleteScreenIfPresent(SCR_FMGR_FILENAME_EDITOR);
        DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
        TBM_EXIT(0x2849);
        return;
    }

    /* new file name can't contain '/' or '\' */
    while ( len>0 )
    {
        len--;
        if ( (ptr[len<<1] == '/' && ptr[(len<<1)+1] == 0) ||
        (ptr[len<<1] == '\\' && ptr[(len<<1)+1] == 0) )
        {
            DisplayPopup((PU8)GetString(FMGR_FS_INVALID_FILENAME_TEXT),
            IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
            DeleteScreenIfPresent(SCR_FMGR_FILENAME_EDITOR);
            DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
            TBM_EXIT(0x2849);
            return;
        }
    }


    if ( IsScreenPresent(SCR_FMGR_FOLDER_OPTION) || IsScreenPresent(SCR_FMGR_APP_FOLDER_OPTION)
    || (IsScreenPresent(SCR_FMGR_TREE_OPTION) && IsScreenPresent(SCR_FMGR_EXPLORER)) )
    {
        TRACE_FMGR_FUNC("SCR_FMGR_APP_FOLDER_OPTION");
        mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

        if ( fmgr_check_file_present() < FMGR_NO_ERROR )
        {
            TBM_EXIT(0x2849);
            return;
        }

        /* create folder under drive no need to append current folder name */
        if ( !(file_info.file_name[0] == 0 && file_info.file_name[1] == 0) &&
        (mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE ||
        mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)L"/") == FALSE) )
        {
            DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
            TBM_EXIT(0x2849);
            return;
        }
    }

    TRACE_EF(g_sw_FMGR, "path(len) = %d, pfnUnicodeStrlen(ptr) = %d", pfnUnicodeStrlen((PS8)fmgr_p->file_path), pfnUnicodeStrlen(ptr));
    if ( pfnUnicodeStrlen((PS8)fmgr_p->file_path) + pfnUnicodeStrlen(ptr) + 1 >= FMGR_MAX_PATH_LEN-15 )
    {
        DisplayPopup((PU8)GetString(FMGR_FS_PATH_OVER_LEN_ERROR_TEXT), IMG_GLOBAL_ERROR,
        0, 1000, ERROR_TONE);
        DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
        TBM_EXIT(0x2849);
        return;
    }

    fs_ret = MMI_FS_SetCurrentDir((PU8)fmgr_p->file_path);
    TRACE_EF(g_sw_FMGR, "fs_ret = %d" ,fs_ret);


    if ( fs_ret == FS_NO_ERROR )
    {
		mmi_trace(g_sw_FMGR, "chenxia!!! fun:fmgr_create_folder;before MCI_FsmMakeDir");
        fs_ret = MMI_FS_CreateDir ((PU8)ptr);

        if ( fs_ret == FS_NO_ERROR )
        {
            /* success */
            fmgr_p->enter_folder = TRUE;
            if ( fmgr_p->tree_action == FMGR_ACTION_NONE )
            {
                if ( IsScreenPresent(fmgr_p->active_explorer_screen) )
                    GoBackToHistory(fmgr_p->active_explorer_screen);
                else
                {
                    // create folder under drive
                    TRACE_FMGR_FUNCTION();
                    entry_fmgr_explorer();
                    DeleteScreenIfPresent(SCR_FMGR_DRIVE_OPTION);
                    DeleteScreenIfPresent(SCR_FMGR_FILENAME_EDITOR);
                    DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
                }
            }
            else
            {
                /* create folder when copy, move */
                if ( IsScreenPresent(SCR_FMGR_TREE_EXPLORER) )
                    GoBackToHistory(SCR_FMGR_TREE_EXPLORER);
                else
                {
                    // create folder under drive
                    entry_fmgr_tree_explorer();
                    DeleteScreenIfPresent(SCR_FMGR_DRIVE_OPTION);
                    DeleteScreenIfPresent(SCR_FMGR_TREE_DRIVE_OPTION);               
                    DeleteScreenIfPresent(SCR_FMGR_FILENAME_EDITOR);
                    DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
                }
            }
            return;
        }
    }
    /* fail */
//
//     UnicodeToAnsii((PS8)nPrintableStr, (PS8)fmgr_p->file_path);
//     TRACE_EF(g_sw_FMGR, "create folder fail under: %s", nPrintableStr);
//

    DisplayPopup((PU8)GetString(GetFileSystemErrorString(fs_ret)),
    IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
    DeleteScreenIfPresent(SCR_FMGR_FILENAME_EDITOR);
    DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
    if ( IsScreenPresent(SCR_FMGR_FOLDER_OPTION) || IsScreenPresent(SCR_FMGR_APP_FOLDER_OPTION) ||
    IsScreenPresent(SCR_FMGR_TREE_OPTION) )
        mmi_fmgr_remove_last_dir(fmgr_p->file_path);
    TBM_EXIT(0x2849);
}

extern BOOL mmi_fmgr_judge_enter(S8* line);
void fmgr_rename(void)
{

    FMGR_FILE_INFO_STRUCT file_info;
    S32 fs_ret, len;
    U16 max_len = FMGR_MAX_PATH_LEN;
    S8 *ptr;
    /*WeiD Add Start For 6013 Ver: TBM780  on 2007-8-13 15:36 */
    BOOL judge_enter = FALSE;
    /*WeiD Add End  For 6013 Ver: TBM780  on 2007-8-13 15:36 */

    TBM_ENTRY(0x284A);
    TRACE_FMGR_FUNCTION();

    if ( fmgr_check_file_present() < FMGR_NO_ERROR )
    {
        TBM_EXIT(0x284A);
        return;
    }

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

    /* rename file: old name in fmgr_p->old_file_path, new name in fmgr_p->file_path */

     /*WeiD Add Start For 6013 Ver: TBM780  on 2007-8-13 15:36 */
     judge_enter =  mmi_fmgr_judge_enter((PS8)fmgr_p->new_file_name);

    if(judge_enter)
    {
    	    DisplayPopup((PU8)GetString(FMGR_FS_INVALID_FILENAME_TEXT),
	        IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
	        DeleteScreenIfPresent(SCR_FMGR_FILENAME_EDITOR);
	        DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
	        TBM_EXIT(0x284A);
	        return;
    }
     /*WeiD Add End  For 6013 Ver: TBM780  on 2007-8-13 15:36 */
    ptr = mmi_fmgr_skip_space ((PS8)fmgr_p->new_file_name);
    len = pfnUnicodeStrlen(ptr);
    if ( len==0 )
    {
        DisplayPopup((PU8)GetString(STR_FMGR_EMPTY_FILENAME),
        IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
        DeleteScreenIfPresent(SCR_FMGR_FILENAME_EDITOR);
        DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
        TBM_EXIT(0x284A);
        return;
    }
    if ( (len==1) && (ptr[0] == '.') && (ptr[1] == 0) )
    {
        DisplayPopup((PU8)GetString(FMGR_FS_INVALID_FILENAME_TEXT),
        IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE); 
        DeleteScreenIfPresent(SCR_FMGR_FILENAME_EDITOR);
        DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
        TBM_EXIT(0x284A);
        return;    
    }

    /* new file name can't contain '/' or '\' */
    while ( len>0 )
    {
        len--;
        if ( (ptr[len<<1] == '/' && ptr[(len<<1)+1] == 0) ||
        (ptr[len<<1] == '\\' && ptr[(len<<1)+1] == 0) )
        {
            DisplayPopup((PU8)GetString(FMGR_FS_INVALID_FILENAME_TEXT),
            IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
            DeleteScreenIfPresent(SCR_FMGR_FILENAME_EDITOR);
            DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
            TBM_EXIT(0x284A);
            return;
        }
    }

    /* rename folder need more restrict length */
    if ( file_info.attribute & FS_ATTR_DIR )
        max_len -= 11;

    if ( pfnUnicodeStrlen((PS8)fmgr_p->file_path) + pfnUnicodeStrlen((PS8)ptr) + 1 
#ifdef __MMI_SHOW_FILE_EXT__
    + pfnUnicodeStrlen((PS8)fmgr_p->ext_name)
#else
    + pfnUnicodeStrlen ((PS8)file_info.file_ext)
#endif
    >= max_len )
    {
        DisplayPopup((PU8)GetString(FMGR_FS_PATH_OVER_LEN_ERROR_TEXT),
        IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
        DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
        TBM_EXIT(0x284A);
        return;
    }

    pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)ptr);

    // restore '.' in file name
    if ( !(file_info.attribute & FS_ATTR_DIR) )
    {
#ifdef __MMI_SHOW_FILE_EXT__		
        pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)L".");
        pfnUnicodeStrcat((PS8)fmgr_p->file_path, fmgr_p->ext_name);
#else
        pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)file_info.file_ext);
#endif
    }

    fs_ret = MMI_FS_Rename (fmgr_p->old_file_path, (PU8)fmgr_p->file_path);

    TRACE_EF(g_sw_FMGR, "fs_ret = %d", fs_ret );

    mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);       

    if ( fs_ret == FS_NO_ERROR )
    {
        /* success */
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
        if ( file_info.attribute & FS_ATTR_DIR )
            PmgFmgrDelExtMelodyFunc((PS8)fmgr_p->old_file_path, PMG_FMGR_DEL_ALL);
        else
            PmgFmgrDelExtMelodyFunc((PS8)fmgr_p->old_file_path, PMG_FMGR_DEL_CERTAIN);
#endif      	
        fmgr_p->enter_folder = TRUE;
        GoBackToHistory(fmgr_p->active_explorer_screen);
    }
    else
    {
        /* fail */
//
//         UnicodeToAnsii((PS8)nPrintableStr, (PS8)fmgr_p->old_file_path);
//         PRINT_INFORMATION(("rename fail: %s", nPrintableStr));
//

        DisplayPopup((PU8)GetString(GetFileSystemErrorString(fs_ret)),
        IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
        DeleteScreenIfPresent(SCR_FMGR_FILENAME_EDITOR);
        DeleteScreenIfPresent(SCR_FMGR_EDITOR_OPTION);
    }

    TBM_EXIT(0x284A);
}

void fmgr_delete (void)
{

    FMGR_FILE_INFO_STRUCT file_info;
    U8 flag;

    TRACE_FMGR_FUNCTION();
    flag = fmgr_check_file_present();

    TRACE_EF(g_sw_FMGR, "fmgr_delete:fmgr_check_file_present()=%d",flag );

    if ( flag <= FMGR_NO_ERROR )
        return;

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);
#if 0 //def __DRM_SUPPORT__
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

    /* delete file */

		
    entry_fmgr_delete_animation();
    DeleteScreenIfPresent(SCR_CONFIRM_SCREEN );

    if ( flag == FMGR_FOLDER )
    {

        TRACE_FMGR_FUNC("(flag == FMGR_FOLDER)");

        pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)file_info.file_name);
        pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)L"/");

//
//         UnicodeToAnsii(nPrintableStr, fmgr_p->file_path);
//         TRACE_EF(g_sw_FMGR, "fmgr_delete(): file_path = %s ",nPrintableStr);
//

        fmgr_p->delete_all = FMGR_DELETE_FOLDER;

        mmi_fmgr_send_delete_req((PU8)fmgr_p->file_path,
        FS_RECURSIVE_TYPE, fmgr_delete_rsp_hdlr);
    }
    else
    {

        TRACE_FMGR_FUNC("(flag != FMGR_FOLDER)");

        pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)file_info.file_name);
#ifndef __MMI_SHOW_FILE_EXT__
        pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)file_info.file_ext);
#endif

        mmi_fmgr_send_delete_req((PU8)fmgr_p->file_path,
        FS_FILE_TYPE, fmgr_delete_rsp_hdlr);
    }
    //entry_fmgr_delete_animation();

}

void fmgr_delete_all_file (void)
{
    U8 flag = 0;
    flag = fmgr_is_parent_present();

    TRACE_EF(g_sw_FMGR, "fmgr_delete_all_file (fmgr_is_parent_present()) flag = %d",flag);

    if ( flag == FALSE )
    {
        DisplayPopup((PU8)GetString(FMGR_FS_PATH_NOT_FOUND_TEXT), IMG_GLOBAL_ERROR,
        0, 1000, ERROR_TONE);

        /* go back to root, reload file manager */
        if ( fmgr_p->src_application == APP_FILEMANAGER )
            fmgr_adjust_history(SCR_FMGR_MAIN);
        else
            fmgr_adjust_history(0);
        return;
    }

    /* delete files including drm file + rights */
    entry_fmgr_delete_animation();
    DeleteScreenIfPresent(SCR_CONFIRM_SCREEN );
    mmi_fmgr_send_delete_req((PU8)fmgr_p->file_path, FS_DIR_TYPE, fmgr_delete_rsp_hdlr);
    //entry_fmgr_delete_animation();
}

void entry_fmgr_delete_animation(void)
{
    TurnOnBacklight(0);

    g_FsTaskIsRun = TRUE;

#ifdef __MMI_AUDIO_PLAYER__
    mdi_audio_suspend_background_play();
#endif

    EntryNewScreen(SCR_FMGR_DELETE_ANIMATION, exit_fmgr_delete_animation,
    NULL, NULL);

    ShowCategory66Screen(STR_GLOBAL_DELETE, 0,
    0, 0, 0, 0,
    (PU8)GetString(STR_FMGR_DELETING),
    IMG_GLOBAL_PROGRESS, NULL);

    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
    if ( !IsScreenPresent(SCR_FMGR_DELETE_ANIMATION) )
        DeleteUptoScrID(fmgr_p->active_explorer_screen);

    SetDelScrnIDCallbackHandler (SCR_FMGR_DELETE_ANIMATION, 
    (HistoryDelCBPtr)fmgr_delete_animation_callback);
}

void exit_fmgr_delete_animation(void)
{
     g_FsTaskIsRun = FALSE;

    if ( isInCall() == FALSE )
    {
        /* pop up interrupt => add to history */
        history_t currHistory;
        S16     nHistory    = 0;

        currHistory.scrnID = SCR_FMGR_DELETE_ANIMATION;
        currHistory.entryFuncPtr = entry_fmgr_delete_animation;
        pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
        GetCategoryHistory( currHistory.guiBuffer );
        AddHistory( currHistory );
    }
    else
    {
        /* interrupt by Call => abort XDelete */
        if ( fmgr_p->delete_all == FMGR_DELETE_FOLDER || 
        (fmgr_p->delete_all == FMGR_DELETE_ALL && 
        FMGR_FILTER_IS_SET(&fmgr_p->delete_filter_type, FMGR_TYPE_ALL)) )
        {
#ifdef MMI_ON_HARDWARE_P
            MMI_FS_Abort(FS_ABORT_XDELETE);
#endif
#ifdef MMI_ON_HARDWARE_P		
            fmt_abort_copy();
#endif
            fmgr_p->cur_index = 0;
            /*if (fmgr_p->delete_all != FMGR_DELETE_ALL)
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
            fmgr_p->delete_all = FMGR_DELETE_NONE;
            fmgr_p->enter_folder = TRUE;*/
        }
        else if ( fmgr_p->delete_all == FMGR_DELETE_ALL )
        {
#ifdef MMI_ON_HARDWARE_P		
            fmt_abort_copy();
#endif
            fmgr_p->cur_index = 0;
        }
    }
}

U8 fmgr_delete_animation_callback (void *param)
{
    if ( fmgr_p->delete_all == FMGR_DELETE_NONE )
        return FALSE;
    else
        return TRUE;
}

void do_finally_process(void)
{
	//add by licheng for usb detected
	g_doFsProcess = 0;
#if (CSW_EXTENDED_API_PMD_USB == 0)
	if(g_NeedReDetectUsb)
	{
		g_NeedReDetectUsb = 0;
		if (USB_DETECTED_OK == MCI_USBDetect())
			SendMessageForUSB(DEVUSB_DETECT_ACTION_PLUGIN); //chenhe,080607
		else
			BatteryIndicationPopup(STR_CHARGER_CONNECTED);
	}
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
}

void mmi_fmgr_send_delete_req(PU8 path, U32 type, PsFuncPtr callback)
{

    mmi_fmt_delete_req_strcut *msgReq = NULL;
    ilm_struct *ilm_ptr = NULL;

    TBM_ENTRY(0x284B);
    TRACE_EF(g_sw_FMGR, "############## mmi_fmgr_send_delete_req()");



    ilm_ptr = allocate_ilm( MOD_FMT );
    if ( NULL == ilm_ptr )
    {
        ; //
    }
    msgReq = (mmi_fmt_delete_req_strcut*) OslConstructDataPtr(sizeof (mmi_fmt_delete_req_strcut));
    if ( NULL == msgReq )
    {
        ;//
    }
    SetProtocolEventHandler(callback, PRT_MMI_FMGR_DELETE_RSP);

    msgReq->type = type;
    msgReq->path = path;
    //msgReq->filter = &fmgr_p->filter_type;

    if ( type == FS_DIR_TYPE && fmgr_p->src_application != APP_FILEMANAGER )
    {
        fmgr_p->delete_filter_type = fmgr_p->filter_type;
        FMGR_FILTER_CLEAR(&fmgr_p->delete_filter_type, FMGR_TYPE_FOLDER);
        msgReq->filter = &fmgr_p->delete_filter_type; 
    }
    else
    {
        FMGR_FILTER_INIT(&fmgr_p->delete_filter_type);
        FMGR_FILTER_SET_ALL(&fmgr_p->delete_filter_type);

        msgReq->filter = &fmgr_p->delete_filter_type;
    }


    ilm_ptr->local_para_ptr = (local_para_struct *)msgReq;
    ilm_ptr->msg_id = MSG_ID_MMI_FMT_DELETE_REQ;
    ilm_ptr->peer_buff_ptr = NULL;

    SEND_ILM( MOD_MMI, MOD_FMT, MMI_L4C_SAP, ilm_ptr);
    g_doFsProcess = 1; //add by licheng for usb detected
    TRACE_EF(g_sw_FMGR, "############## mmi_fmgr_send_delete_req()");
    TBM_EXIT(0x284B);

#if 0    
    mmi_fmt_delete_rsp_strcut finfo;
    UINT Ret = 0;
    memset(&finfo, 0x00, sizeof(finfo));

    UINT len = 0;
    U8    fl_path[64] = "\0";


    if ( type == FS_FILE_TYPE )   //'del item'   in someone folder
    {
        TRACE_FMGR_FUNC("(type == FS_FILE_TYPE)");
        Ret = MMI_FS_Delete(path);
    }
    else if ( type == FS_DIR_TYPE )   //'del all item' in someone folder
    {
        FS_HANDLE file_handle = 0;
        U8 buf_file_name[64] = "\0";
        U8 path_file_name[128] = "\0";

        UINT return_value = 0;
        FS_DOSDirEntry  file_info;

        pfnUnicodeStrcpy(fl_path, path);
        len = pfnUnicodeStrlen (fl_path) << 1;

        if ( (fl_path[len - 2] != '*') && (fl_path[len - 1] == 0) )
            pfnUnicodeStrcat(fl_path, L"*");

//
//         UnicodeToAnsii(nPrintableStr, fl_path);
//         TRACE_EF(g_sw_FMGR, "fl_path = %s",fl_path);
//

        memset(&file_info, 0x00, sizeof(file_info));

        if ( fmgr_p->src_application != APP_FILEMANAGER &&  (fmgr_p->sel_type & FMGR_SELECT_BROWSE) )
            file_handle = MMI_FS_FindFirstN((PU16)fl_path, fmgr_p->filter_pattern, 0,
            0, 0, FS_ATTR_SYSTEM|FS_ATTR_HIDDEN|FS_ATTR_VOLUME,
            &file_info, (PU16)buf_file_name,
            FMGR_MAX_FILE_LEN*ENCODING_LENGTH, 0, FS_FIND_DEFAULT);

        else
            file_handle = MMI_FS_FindFirstN((PU16)fl_path, fmgr_p->filter_pattern, 0,
            FS_ATTR_DIR, 0, FS_ATTR_SYSTEM|FS_ATTR_HIDDEN|FS_ATTR_VOLUME,
            &file_info, (PU16)buf_file_name,
            FMGR_MAX_FILE_LEN*ENCODING_LENGTH, 0, FS_FIND_DEFAULT);
        if ( file_handle > 0 )
        {
            do
            {
                if ( file_info.Attributes != FS_ATTR_DIR )
                {
                    memset(path_file_name, 0x00, sizeof(path_file_name));
                    pfnUnicodeStrcat(path_file_name,path);
                    pfnUnicodeStrcat(path_file_name,buf_file_name);
                    Ret = MMI_FS_Delete(path_file_name);

                    if ( Ret != FS_NO_ERROR )
                    {
                        break;
                    }
                    memset(&file_info, 0x00, sizeof(file_info));                    
                }

                if ( fmgr_p->src_application != APP_FILEMANAGER && (fmgr_p->sel_type & FMGR_SELECT_BROWSE) )
                    return_value = MMI_FS_FindNextN(file_handle, fmgr_p->filter_pattern, 0,
                    0, &file_info, (PU16)buf_file_name,
                    FMGR_MAX_FILE_LEN*ENCODING_LENGTH, FS_FIND_DEFAULT);
                else
                    return_value = MMI_FS_FindNextN(file_handle, fmgr_p->filter_pattern, 0,
                    FS_ATTR_DIR, &file_info, (PU16)buf_file_name,
                    FMGR_MAX_FILE_LEN*ENCODING_LENGTH, FS_FIND_DEFAULT);

            } while ( return_value == FS_NO_ERROR );

            MMI_FS_FindClose(file_handle);
        }

    }
    else if ( type == FS_RECURSIVE_TYPE ) //'del folder item'  
    {
        Ret = MMI_FS_RemoveDir(path);

    }

    TRACE_EF(g_sw_FMGR, "mmi_fmgr_send_delete_req(): Del file type = 0x%0x , result  = %d",type,Ret);

    if ( Ret == FS_NO_ERROR )
        finfo.result = FS_NO_ERROR;
    else
        finfo.result = FS_ERROR_RESERVED;

    fmgr_delete_rsp_hdlr(&finfo);
/*Wuzc Del Start For FMGR Ver:    on 2007-3-2 18:7 */
#if 0
#ifdef MMI_ON_HARDWARE_P	
    MYQUEUE Message;
    mmi_fmt_delete_req_strcut* msgReq;

    Message.oslSrcId=MOD_MMI;
    Message.oslDestId=MOD_FMT;
    Message.oslMsgId = PRT_MMI_FMGR_DELETE_REQ;

    ClearAllKeyHandler();
    SetProtocolEventHandler (callback, PRT_MMI_FMGR_DELETE_RSP);

    msgReq = (mmi_fmt_delete_req_strcut*) OslConstructDataPtr(sizeof (mmi_fmt_delete_req_strcut));

    msgReq->type = type;
    msgReq->path = path;


    if ( type == FS_DIR_TYPE && fmgr_p->src_application != APP_FILEMANAGER )
    {
        fmgr_p->delete_filter_type = fmgr_p->filter_type;
        FMGR_FILTER_CLEAR(&fmgr_p->delete_filter_type, FMGR_TYPE_FOLDER);
        msgReq->filter = &fmgr_p->delete_filter_type; 
        //	msgReq->filter = fmgr_p->filter_type & (~FMGR_TYPE_FOLDER);
    }
    else
    {
        FMGR_FILTER_INIT(&fmgr_p->delete_filter_type);
        FMGR_FILTER_SET_ALL(&fmgr_p->delete_filter_type);

        msgReq->filter = &fmgr_p->delete_filter_type;
    }

    Message.oslDataPtr = (oslParaType *)msgReq;
    Message.oslPeerBuffPtr= NULL;
    OslMsgSendExtQueue(&Message);
#else
    mmi_fmt_delete_req_strcut msgReq;

    msgReq.type = type;
    msgReq.path = path;

    if ( type == FS_DIR_TYPE && fmgr_p->src_application != APP_FILEMANAGER )
    {
        fmgr_p->delete_filter_type = fmgr_p->filter_type;
        FMGR_FILTER_CLEAR(&fmgr_p->delete_filter_type, FMGR_TYPE_FOLDER);
        msgReq.filter = &fmgr_p->delete_filter_type; 
        //	msgReq->filter = fmgr_p->filter_type & (~FMGR_TYPE_FOLDER);
    }
    else
    {
        FMGR_FILTER_INIT(&fmgr_p->delete_filter_type);
        FMGR_FILTER_SET_ALL(&fmgr_p->delete_filter_type);

        msgReq.filter = &fmgr_p->delete_filter_type;
    }

    fmt_delete_hdlr((oslParaType *)&msgReq, NULL);
#endif
#endif
#endif
/*Wuzc Del End  For FMGR Ver:    on 2007-3-2 18:7 */
}

void fmgr_delete_rsp_hdlr(void *info)
{
    mmi_fmt_delete_rsp_strcut *msgPtr;
    TBM_ENTRY(0x284E);
    TRACE_EF(g_sw_FMGR, "################ fmgr_delete_rsp_hdlr()");

    TurnOffBacklight();

#ifdef __MMI_AUDIO_PLAYER__
    mdi_audio_resume_background_play();
#endif

    msgPtr = (mmi_fmt_delete_rsp_strcut *)info;

    if ( msgPtr->result >= FS_NO_ERROR )
    {
        TRACE_EF(g_sw_FMGR, "################ fmgr_delete_rsp_hdlr()");
        /* move selcetion up one step */
        if ( fmgr_p->cur_index>0 )
            fmgr_p->cur_index--;
        
        /* success */
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
        if ( fmgr_p->delete_all == FMGR_DELETE_SINGLE )
            PmgFmgrDelExtMelodyFunc((PS8)fmgr_p->file_path, PMG_FMGR_DEL_CERTAIN);
        else
            PmgFmgrDelExtMelodyFunc((PS8)fmgr_p->file_path, PMG_FMGR_DEL_ALL);
#endif
        fmgr_p->enter_folder = TRUE;

        if ( isInCall() == TRUE && GetExitScrnID() != SCR_FMGR_DELETE_ANIMATION )
        {
            DeleteScreenIfPresent(SCR_FMGR_DELETE_ANIMATION);
        }
        else
        {
            DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0,
            1000, SUCCESS_TONE);
            DeleteScreenIfPresent(SCR_FMGR_DELETE_ANIMATION);
        }
    }
    else
    {
        /* fail */
//
//         UnicodeToAnsii((PS8)nPrintableStr, (PS8)fmgr_p->file_path);
//         PRINT_INFORMATION(("delete fail: %s", nPrintableStr));
//

        if ( isInCall() == TRUE && GetExitScrnID() != SCR_FMGR_DELETE_ANIMATION )
        {
            DeleteScreenIfPresent(SCR_FMGR_DELETE_ANIMATION);
        }
        else
        {
            DisplayPopup((PU8)GetString(GetFileSystemErrorString(msgPtr->result)),
            IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
            DeleteScreenIfPresent(SCR_FMGR_DELETE_ANIMATION);
        }
    }
    if ( fmgr_p->delete_all != FMGR_DELETE_ALL )
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
#ifdef __DRM_SUPPORT__   
    else if ( pfnUnicodeStrncmp ((PS8)fmgr_p->file_path, (PS8)DRM_RO_PATH, 
    pfnUnicodeStrlen((PS8)DRM_RO_PATH)) == 0
#ifdef __DRM_V01__						
    || pfnUnicodeStrncmp ((PS8)fmgr_p->file_path, (PS8)DRM_NOR_PATH, 
    pfnUnicodeStrlen((PS8)DRM_NOR_PATH)) == 0
#endif
    )
        DeleteScreenIfPresent(SCR_FMGR_EXPLORER);
#endif

    if ( fmgr_p->src_application == APP_FILEMANAGER &&
    MMI_FS_Count ((PU8)fmgr_p->file_path, FS_DIR_TYPE | FS_FILE_TYPE | FS_FILTER_SYSTEM_ATTR) == 0 )
    {
#ifdef __DRM_SUPPORT__   
        if ( pfnUnicodeStrncmp ((PS8)fmgr_p->file_path, (PS8)DRM_RO_PATH, 
        pfnUnicodeStrlen((PS8)DRM_RO_PATH)) == 0 )
            DeleteScreenIfPresent(SCR_FMGR_EXPLORER);
        else
#endif         
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
    }
    TRACE_EF(g_sw_FMGR, "################ fmgr_delete_rsp_hdlr()");

    fmgr_p->delete_all = FMGR_DELETE_NONE;
    do_finally_process();	
    TBM_EXIT(0x284E);
}

void fmgr_copy (void)
{
    FMGR_FILE_INFO_STRUCT file_info;
    S8 *temp;
    U16 scrnId;

    scrnId = GetExitScrnID();
    if ( scrnId != SCR_FMGR_DRIVE_OPTION &&
    scrnId != SCR_FMGR_TREE_DRIVE_OPTION &&
    scrnId != SCR_FMGR_MAIN )
    {
        /* copy to drive no need to append current folder name */
        mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

        if ( !(file_info.file_name[0] == 0 && file_info.file_name[1] == 0) &&
        (mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name)==FALSE ||
        mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)L"/")==FALSE) )
        {
            DeleteScreenIfPresent(SCR_FMGR_TREE_OPTION);
            return;
        }
    }

#ifdef MMI_ON_HARDWARE_P
#if 0
	mmi_trace(g_sw_FMGR, "######gdm:fmgr_copy, fmt_need_abort=%d", fmt_need_abort);
    if ( fmt_get_abort_copy () == TRUE )
    {
        /* stil copying, busy */
        DisplayPopup((PU8)GetString(FMGR_FS_DEVICE_BUSY_TEXT), IMG_GLOBAL_ERROR, 0,
        1000, ERROR_TONE);
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
        DeleteUptoScrID(fmgr_p->active_explorer_screen);
        return;
    }
#endif
#endif

    /* copy file */
    temp = mmi_fmgr_extract_file_name((PS8)fmgr_p->old_file_path);
    if ( pfnUnicodeStrlen((PS8)fmgr_p->file_path) + pfnUnicodeStrlen(temp) >= FMGR_MAX_PATH_LEN )
    {
        DisplayPopup((PU8)GetString(FMGR_FS_PATH_OVER_LEN_ERROR_TEXT), IMG_GLOBAL_ERROR,
        0, 1000, ERROR_TONE);
        DeleteScreenIfPresent(SCR_FMGR_TREE_OPTION);
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
        return;
    }

	mmi_trace(g_sw_FMGR, "chenxia!!! fun:fmgr_copy;");
    pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)temp);

    if ( pfnUnicodeStrcmp((PS8)fmgr_p->old_file_path, (PS8)fmgr_p->file_path) == 0 )
    {
        /* can't copy, move to same folder */
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
        FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
        fmgr_p->tree_action = FMGR_ACTION_NONE;
        DisplayPopup((PU8)GetString(FMGR_FS_FILE_EXISTS_TEXT),
        IMG_GLOBAL_EMPTY, 0, 1000, ERROR_TONE);
#if 0   	
/* under construction !*/
/* under construction !*/
#endif
        DeleteUptoScrID(fmgr_p->active_explorer_screen);
        return;
    }
    entry_fmgr_copy_animation();
    /* display animation screen */
    mmi_fmgr_send_copy_req(fmgr_p->tree_action, (PU8)fmgr_p->old_file_path,
    (PU8)fmgr_p->file_path, (PsFuncPtr)fmgr_copy_rsp_hdlr);
    //entry_fmgr_copy_animation();
    return;
}



void entry_fmgr_copy_animation(void)
{
    U16 stringId;

    TurnOnBacklight(0);
    g_FsTaskIsRun = TRUE;
#ifdef __MMI_AUDIO_PLAYER__
    mdi_audio_suspend_background_play();
#endif

    if ( fmgr_p->tree_action == FMGR_ACTION_COPY )
        stringId = STR_FMGR_COPYING;
    else if ( fmgr_p->tree_action == FMGR_ACTION_MOVE )
        stringId = STR_FMGR_MOVING;
    else
        return;

    EntryNewScreen(SCR_FMGR_COPY_ANIMATION, exit_fmgr_copy_animation,
    NULL, NULL);

    ShowCategory66Screen(STR_FMGR_TITLE, GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID),
    0, 0, 0, 0,
    (PU8)GetString(stringId),
    IMG_GLOBAL_PROGRESS, NULL);

    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
    if ( !IsScreenPresent(SCR_FMGR_COPY_ANIMATION) )
        DeleteUptoScrID(fmgr_p->active_explorer_screen);
}

void exit_fmgr_copy_animation(void)
{
    g_FsTaskIsRun = FALSE;
    if ( isInCall() == FALSE )
    {
        /* pop up interrupt => add to history */
        history_t  currHistory;
        S16       nHistory    = 0;

        currHistory.scrnID = SCR_FMGR_COPY_ANIMATION;
        currHistory.entryFuncPtr = entry_fmgr_copy_animation;
        pfnUnicodeStrcpy( (S8 *)currHistory.inputBuffer, (S8 *)&nHistory );
        GetCategoryHistory( currHistory.guiBuffer );
        AddHistory( currHistory );
    }
    else
    {
        /* interrupt by Call => abort copy */
        if ( fmgr_p->tree_action == FMGR_ACTION_COPY ||
        fmgr_p->tree_action == FMGR_ACTION_MOVE )
        {
#ifdef MMI_ON_HARDWARE_P
            fmt_abort_copy();
#endif
            FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
            fmgr_p->tree_action = FMGR_ACTION_NONE;
            fmgr_p->enter_folder = TRUE;
        }
    }
}

void mmi_fmgr_send_copy_req (U8 action, PU8 src_file, PU8 dest_file, PsFuncPtr callback)
{

    mmi_fmt_copy_req_strcut *msg_ptr = NULL;
    ilm_struct *ilm_ptr = NULL;
	TBM_ENTRY(0x284C);
	TRACE_FMGR_FUNCTION();

    TRACE_FMGR_FUNCTION();

    ilm_ptr = allocate_ilm( MOD_FMT );
    if ( NULL == ilm_ptr )
    {
        ; //
    }
    msg_ptr = (mmi_fmt_copy_req_strcut *)OslConstructDataPtr(sizeof (mmi_fmt_copy_req_strcut) );
    if ( NULL == msg_ptr )
    {
        ;//
    }
    SetProtocolEventHandler(callback, MSG_ID_MMI_FMT_COPY_RSP);
    msg_ptr->src_file = src_file;
    if(FMGR_ACTION_COPY == action)
    {
        msg_ptr->action = FMT_COPY;
    }
    else
    {
        msg_ptr->action = FMT_MOVE;
    }
    UCS2Strcpy( msg_ptr->dest_file, dest_file);
    ilm_ptr->local_para_ptr = (local_para_struct *)msg_ptr;
    ilm_ptr->msg_id = MSG_ID_MMI_FMT_COPY_REQ;
    ilm_ptr->peer_buff_ptr = NULL;
    SEND_ILM( MOD_MMI, MOD_FMT, MMI_L4C_SAP, ilm_ptr);
    g_doFsProcess = 1; //add by licheng for usb detected
    TBM_EXIT(0x284C);
    return ;


#if 0
    UINT res = 0;
    mmi_fmt_copy_rsp_strcut finfo;

    memset(&finfo, 0x00, sizeof(finfo));
    if ( action == FMGR_ACTION_COPY )
        res = MMI_FS_Move(src_file, dest_file, FS_MOVE_COPY, NULL,NULL);
    else
        res = MMI_FS_Move(src_file, dest_file, FS_MOVE_KILL, NULL,NULL);

    TRACE_EF(g_sw_FMGR, "mmi_fmgr_send_copy_req(): res = %d", res);
    if ( res < 0 )
    {
        finfo.result = FS_ERROR_RESERVED;
        fmgr_copy_rsp_hdlr(&finfo);     
    }
    else
    {
        finfo.result = FS_NO_ERROR;
        fmgr_copy_rsp_hdlr(&finfo);     
    }

    return;

/*Wuzc Del Start For FMGR Ver:    on 2007-3-2 11:25 */

#ifdef MMI_ON_HARDWARE_P	
    MYQUEUE Message;
    mmi_fmt_copy_req_strcut* msgReq;

    Message.oslSrcId=MOD_MMI;
    Message.oslDestId=MOD_FMT;
    Message.oslMsgId = PRT_MMI_FMGR_COPY_REQ;

    ClearAllKeyHandler();
    SetProtocolEventHandler (callback, PRT_MMI_FMGR_COPY_RSP);

    msgReq = (mmi_fmt_copy_req_strcut*) OslConstructDataPtr(sizeof (mmi_fmt_copy_req_strcut));

    if ( action == FMGR_ACTION_COPY )
        msgReq->action = FMT_COPY;
    else
        msgReq->action = FMT_MOVE;
    msgReq->src_file = src_file;
    pfnUnicodeStrcpy((PS8)msgReq->dest_file, (PS8)dest_file);

    Message.oslDataPtr = (oslParaType *)msgReq;
    Message.oslPeerBuffPtr= NULL;
    OslMsgSendExtQueue(&Message);
#else    

    mmi_fmt_copy_req_strcut msgReq;

    if ( action == FMT_COPY )
        msgReq.action = FMT_COPY;
    else
        msgReq.action = FMT_MOVE;      

    msgReq.src_file = src_file;
    pfnUnicodeStrcpy((PS8)msgReq.dest_file, (PS8)dest_file);

    fmt_copy_hdlr((oslParaType *)&msgReq, NULL);
#endif
#endif
/*Wuzc Del End  For FMGR Ver:    on 2007-3-2 11:25 */
}

void fmgr_copy_rsp_hdlr(void *info)
{
    mmi_fmt_copy_rsp_strcut *msgPtr;

    msgPtr = (mmi_fmt_copy_rsp_strcut *)info;
    TBM_ENTRY(0x284D);
    TRACE_EF(g_sw_FMGR, "################# fmgr_copy_rsp_hdlr()! ");

    TurnOffBacklight();

    if ( isInCall() == TRUE && GetExitScrnID() != SCR_FMGR_COPY_ANIMATION )
    {
        if ( msgPtr->result == FS_ABORTED_ERROR )
        {
            /* aborted => go back to source folder */
            pfnUnicodeStrcpy((S8*)fmgr_p->file_path, (S8*)fmgr_p->old_file_path);
        }
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
        FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
        fmgr_p->tree_action = FMGR_ACTION_NONE;
        fmgr_p->enter_folder = TRUE;
        DeleteScreenIfPresent(SCR_FMGR_COPY_ANIMATION);
	 do_finally_process();
        TBM_EXIT(0x284D);
        return;
    }

#ifdef __MMI_AUDIO_PLAYER__
    mdi_audio_resume_background_play();
#endif
    if ( msgPtr->result >= 0 )
    {
        TRACE_EF(g_sw_FMGR, "################# fmgr_copy_rsp_hdlr()!>0 ");
        /* success */
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__      	
        if ( fmgr_p->tree_action == FMGR_ACTION_MOVE )
            PmgFmgrDelExtMelodyFunc((PS8)fmgr_p->old_file_path, PMG_FMGR_DEL_CERTAIN);
#endif
        DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0,
        1000, SUCCESS_TONE);
        DeleteScreenIfPresent(SCR_FMGR_COPY_ANIMATION);
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
        FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
        fmgr_p->tree_action = FMGR_ACTION_NONE;
        fmgr_p->enter_folder = TRUE;   
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
        fmgr_download_move_sucess=TRUE;
#endif
    }
    else
    {
        TRACE_EF(g_sw_FMGR, "################# fmgr_copy_rsp_hdlr()!<0 ");
        /* fail */
//
//         UnicodeToAnsii((PS8)nPrintableStr, (PS8)fmgr_p->file_path);
//         PRINT_INFORMATION(("copy rsp fail: %s", nPrintableStr));
//

        pfnUnicodeStrcpy((S8*)fmgr_p->file_path, (S8*)fmgr_p->old_file_path);
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
        FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
        fmgr_p->tree_action = FMGR_ACTION_NONE;
        fmgr_p->enter_folder = FALSE;

        if ( msgPtr->result == FS_ABORTED_ERROR )
        {
            if ( IsScreenPresent(fmgr_p->active_explorer_screen) )
                GoBackToHistory(fmgr_p->active_explorer_screen);
        }
        else
        {
            DisplayPopup((PU8)GetString(GetFileSystemErrorString(msgPtr->result)),
            IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
            DeleteScreenIfPresent(SCR_FMGR_COPY_ANIMATION);      
        }

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__	//commented by ritesh
        if ( g_istheme_folder_created==TRUE )
        {
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
            fmgr_p->delete_all = FMGR_DELETE_FOLDER;

            mmi_fmgr_send_delete_req((PU8)gtheme_folder_path,
            FS_RECURSIVE_TYPE, theme_folder_delete_rsp_hdlr);
            g_istheme_folder_created=FALSE;

        }
#endif	//ritesh
    }
    TRACE_EF(g_sw_FMGR, "################# fmgr_copy_rsp_hdlr()!end ");
	//add by licheng for usb detected
	do_finally_process();
	TBM_EXIT(0x284D);
}

void fmgr_send_format_req(U8 drive, U32 level, PsFuncPtr callback)
{
	
    mmi_fmt_format_req_strcut *msgReq = NULL;
    ilm_struct *ilm_ptr = NULL;

    ilm_ptr = allocate_ilm( MOD_FMT );
    msgReq = (mmi_fmt_format_req_strcut *)OslConstructDataPtr(sizeof (mmi_fmt_format_req_strcut) );

    if ( NULL == ilm_ptr )
    {
        ;
    }
    if ( NULL == msgReq )
    {
        ;
    }

    SetProtocolEventHandler(callback, MSG_ID_MMI_FMT_FORMAT_RSP);
    msgReq->level = level;
    msgReq->drive = drive;
    ilm_ptr->local_para_ptr = (local_para_struct *)msgReq;
    ilm_ptr->msg_id = MSG_ID_MMI_FMT_FORMAT_REQ;
    ilm_ptr->peer_buff_ptr = NULL;

    SEND_ILM( MOD_MMI, MOD_FMT, MMI_L4C_SAP, ilm_ptr);

    TRACE_EF(g_sw_FMGR, "#################### fmgr_send_format_req");
    g_doFsProcess = 1; //add by licheng for usb detected
    return;

#if 0
    MYQUEUE Message;
    mmi_fmt_format_req_strcut* msgReq;

    Message.oslSrcId=MOD_MMI;
    Message.oslDestId=MOD_FMT;
    Message.oslMsgId = PRT_MMI_FMGR_FORMAT_REQ;

    ClearAllKeyHandler();
    SetProtocolEventHandler (callback, MSG_ID_MMI_FMT_FORMAT_RSP);

    msgReq = (mmi_fmt_format_req_strcut*) OslConstructDataPtr(sizeof (mmi_fmt_format_req_strcut));

    msgReq->level = level;
    msgReq->drive = drive;

    Message.oslDataPtr = (oslParaType *)msgReq;
    Message.oslPeerBuffPtr= NULL;
    OslMsgSendExtQueue(&Message);
#endif    
}

void fmgr_drive_rsp_hdlr(void *info)
{
    mmi_fmt_format_rsp_strcut *msgPtr;

    msgPtr = (mmi_fmt_format_rsp_strcut *)info;

    if ( isInCall() == TRUE && GetExitScrnID() != SCR_FMGR_DRIVE_ANIMATION 
    && fmgr_p->tree_action == FMGR_ACTION_CHECK_DRIVE )
    {
        FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
        fmgr_p->tree_action = FMGR_ACTION_NONE;
        fmgr_p->enter_folder = TRUE;
        DeleteScreenIfPresent(SCR_FMGR_DRIVE_ANIMATION);
	 do_finally_process();
        return;
    }

    TurnOffBacklight();

    fmgr_p->enter_folder = TRUE;
    FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
    fmgr_p->tree_action = FMGR_ACTION_NONE;
    fmgr_p->drive_action = FMGR_ACTION_NONE;

    if ( isInCall() == TRUE && GetExitScrnID() != SCR_FMGR_DRIVE_ANIMATION )
    {
        DeleteScreenIfPresent(SCR_FMGR_DRIVE_ANIMATION);
	 do_finally_process();
        return;
    }

    if ( msgPtr->result >= 0 )
    {
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ 
        if ( fmgr_p->drive_action==FMGR_ACTION_FORMAT )
        {
            U8 themetype;
            S8 drive_index;
            themetype=tm_get_curr_theme_type();
            drive_index = fmgr_get_drive_index(fmgr_p->cur_index);

            if ( (themetype==THEME_TYPE_DOWNLOADED_PHONE) && ((drive_index==FMGR_NAND_STORAGE)||(drive_index==FMGR_NOR_STORAGE)) )
            {
                ActivateDefaultTheme();
            }
            else if ( (themetype==THEME_TYPE_DOWNLOADED_CARD) && (drive_index==FMGR_REMOVABLE_STORAGE) )
            {
                ActivateDefaultTheme();
            }
        }
#endif
        /* success */
        DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0,
        1000, SUCCESS_TONE);
        DeleteScreenIfPresent(SCR_FMGR_DRIVE_ANIMATION);
    }
    else
    {
        /* fail */
        U16 stringId;

        if ( msgPtr->result == FS_TIMEOUT )
            /* if file open => can't format */
            stringId = FMGR_FS_LOCK_MUTEX_FAIL_TEXT;
        else
            stringId = GetFileSystemErrorString(msgPtr->result);

        DisplayPopup((PU8)GetString(stringId),
        IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
        DeleteScreenIfPresent(SCR_FMGR_DRIVE_ANIMATION);
    }
	//add by licheng for usb detected
	do_finally_process();
}

void fmgr_send_check_drive_req(U8 drive, PsFuncPtr callback)
{
    MYQUEUE Message;
    mmi_fmt_check_drive_req_strcut* msgReq;

    Message.oslSrcId=MOD_MMI;
    Message.oslDestId=MOD_FMT;
    Message.oslMsgId = PRT_MMI_FMGR_CHECK_DRIVE_REQ;

    ClearAllKeyHandler();
    SetProtocolEventHandler (callback, PRT_MMI_FMGR_CHECK_DRIVE_RSP);

    msgReq = (mmi_fmt_check_drive_req_strcut*) OslConstructDataPtr(sizeof (mmi_fmt_check_drive_req_strcut));

    msgReq->drive = drive;

    Message.oslDataPtr = (oslParaType *)msgReq;
    Message.oslPeerBuffPtr= NULL;
    OslMsgSendExtQueue(&Message);
}

void fmgr_send_unmount_req(U8 index, U32 mode, PsFuncPtr callback)
{
    MYQUEUE Message;
    mmi_fmt_unmount_req_strcut* msgReq;

    Message.oslSrcId=MOD_MMI;
    Message.oslDestId=MOD_FMT;
    Message.oslMsgId = PRT_MMI_FMGR_UNMOUNT_REQ;

    ClearAllKeyHandler();
    SetProtocolEventHandler (callback, PRT_MMI_FMGR_UNMOUNT_RSP);

    msgReq = (mmi_fmt_unmount_req_strcut*) OslConstructDataPtr(sizeof (mmi_fmt_unmount_req_strcut));

    msgReq->index = index;
    msgReq->mode = mode;

    Message.oslDataPtr = (oslParaType *)msgReq;
    Message.oslPeerBuffPtr= NULL;
    OslMsgSendExtQueue(&Message);
}

void fmgr_sort(void)
{
    S16 error;
    U8 temp;

    /* write to nvram */
    temp = fmgr_p->sort_option >> 5;
    if ( fmgr_p->sort_option == FS_SORT_NAME || fmgr_p->sort_option == FS_SORT_SIZE || 
    fmgr_p->sort_option == FS_SORT_TIME || fmgr_p->sort_option == FS_SORT_TYPE || 
    fmgr_p->sort_option == FS_NO_SORT )
        WriteValue(NVRAM_EF_FMGR_SORT_OPTION_LID, &temp, DS_BYTE, &error);
    fmgr_p->enter_folder = TRUE;
    GoBackToHistory(fmgr_p->active_explorer_screen);
}

#ifdef __MMI_ACCEL_OPP_SUPPORT__
extern void mmi_opp_fmgr_forward_to_opp_callback(void *buf_p, U32 result);
#endif
#ifdef __MMI_SEND_MMS_FROM_IMG_VIEW__
extern void jmms_content_insert_hdlr(char * content_source);
#endif
void fmgr_forward_to_app(void)
{
    FMGR_FILE_INFO_STRUCT file_info;
    TRACE_FMGR_FUNCTION();

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

    if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE
#ifndef __MMI_SHOW_FILE_EXT__
    || mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == FALSE
#endif
    )
        return;
    TRACE_EF(g_sw_FMGR, "fmgr_forward_to_app(): tree_action = %d",fmgr_p->tree_action);

    DeleteUptoScrID(SCR_FMGR_EXPLORER);

    switch ( fmgr_p->tree_action )
    {
        case FMGR_ACTION_FWD_WALLPAPER:
            /* pass file name to Wallpaper */
            /* hook Wallpaper func */
            if ( 
            FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_GIF) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_BMP)
#ifdef JPG_DECODE		         	
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG)             
#endif         	
            )
            {
                PhnsetWallpaperForwardFromFmgr((PS8)fmgr_p->file_path, (int)file_info.is_short);
            }
            else
            {
#ifdef JPG_DECODE
                DisplayPopup((PU8)GetString(STR_FMGR_INVALID_WALLPAPER_FORMAT), 
                IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
#else
                DisplayPopup((PU8)GetString(STR_FMGR_INVALID_WALLPAPER_FORMAT_NOJPG), 
                IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
#endif
            }  
            break;

#ifdef __MMI_SUB_WALLPAPER__
        case FMGR_ACTION_FWD_SUB_WALLPAPER:
            /* pass file name to sub Wallpaper */
            /* hook sub Wallpaper func */
            if ( 
            FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_GIF) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_BMP)
#ifdef JPG_DECODE		         	
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG)             
#endif         	
            )
            {
                PhnsetSubWallpaperForwardFromFmgr((PS8)fmgr_p->file_path, (int)file_info.is_short);
            }
            else
            {
#ifdef JPG_DECODE
                DisplayPopup((PU8)GetString(STR_FMGR_INVALID_WALLPAPER_FORMAT), 
                IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
#else
                DisplayPopup((PU8)GetString(STR_FMGR_INVALID_WALLPAPER_FORMAT_NOJPG), 
                IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
#endif
            }  
            break;
#endif

        case FMGR_ACTION_FWD_SCREENSAVER:
            /* pass file name to Screen Saver */
            /* hook Screen Saver func */
            if ( 
            FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_GIF) 
            ||  FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_BMP)
#ifdef JPG_DECODE		         	         	
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG)             
#endif         	
#if defined(__MMI_SCREENSAVER_DISPLAY_VIDEO__)
//#ifdef MP4_CODEC   //guojian deleted for jassmine 20080508
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_3GP) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_MP4) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_RM) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_RMVB) 
//#endif
#ifdef MJPG_SUPPORT
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_AVI)
#endif
#endif//(__MMI_SCREENSAVER_DISPLAY_VIDEO__) && !defined(WIN32)
            )
            {
                PhnsetScrSvrForwardFromFmgr((PS8)fmgr_p->file_path, (int)file_info.is_short);
            }
            else
            {
#ifdef JPG_DECODE
                DisplayPopup((PU8)GetString(STR_FMGR_INVALID_WALLPAPER_FORMAT), 
                IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
#else
                DisplayPopup((PU8)GetString(STR_FMGR_INVALID_WALLPAPER_FORMAT_NOJPG), 
                IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
#endif
            }  
            break;

#ifdef __MMI_POWER_ON_OFF_DISPLAY__
        case FMGR_ACTION_FWD_POWER_ON:
        case FMGR_ACTION_FWD_POWER_OFF:
            if ( 
            FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_GIF) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_BMP)         
#ifdef JPG_DECODE		         	         	
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG)             
#endif         	
#if defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ ) 
//#ifdef MP4_CODEC  //guojian deleted for jassmine 20080508
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_3GP) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_MP4) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_RM) 
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_RMVB) 
//#endif
#ifdef MJPG_SUPPORT
            || FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_AVI) 
#endif
#endif //defined( __MMI_POWER_ON_OFF_DISPLAY_VIDEO__ ) && !defined(WIN32)
            )
            {
                PhnsetPwrOnOffForwardFromFmgr((PS8)fmgr_p->file_path, fmgr_p->tree_action, (int)file_info.is_short);
//
//                 UnicodeToAnsii(nPrintableStr, fmgr_p->file_path);
//                 TRACE_EF(g_sw_FMGR, "to power on/off file path = %s",nPrintableStr);
//
            }
            else
            {
                DisplayPopup((PU8)GetString(STR_FMGR_INVALID_FORMAT), 
                IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);

            }
            break;
#endif //__MMI_POWER_ON_OFF_DISPLAY__

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
        case FMGR_ACTION_FWD_PROFILE:
            /* pass file name to Profile */
            /* hook Profile func */
            PmgSetShortFileName( file_info.is_short ); // let PMGR know whether it's short filename
            PmgFmgrAddExtMelodyFunc((PS8)fmgr_p->file_path);
            break;
#endif

#ifdef __MMI_EMAIL__
        case FMGR_ACTION_FWD_EMAIL:
            /* pass file name to Email */
            if ( (fmgr_p->src_application != APP_FILEMANAGER) && 
            (fmgr_p->src_application != APP_IMAGEVIEWER) &&
            (fmgr_p->src_application != APP_VDOPLY) &&            
            (fmgr_p->sel_type & FMGR_SELECT_BROWSE) )
            {
                /* fwd from browser app to transport app: 
                 needs to delete history for browser app and add marker for it */
                fmgr_adjust_history(SCR_FMGR_EXPLORER);
                DeleteScreenIfPresent(SCR_FMGR_EXPLORER);
                fmgr_add_marker_for_app(fmgr_p->src_application);
            }
            /* hook Email func */
            mmi_email_fmgr_send ((PS8)fmgr_p->file_path);
            break;
#endif

#ifdef MMI_ON_HARDWARE_P
        case FMGR_ACTION_FWD_PHONEBOOK:
            /* pass file name to PHB */
            if ( (fmgr_p->src_application != APP_FILEMANAGER) && 
            (fmgr_p->src_application != APP_IMAGEVIEWER) &&
            (fmgr_p->src_application != APP_VDOPLY) &&            
            (fmgr_p->sel_type & FMGR_SELECT_BROWSE) )
            {
                /* fwd from browser app to transport app: 
                 needs to delete history for browser app and add marker for it */
                fmgr_adjust_history(SCR_FMGR_EXPLORER);
                DeleteScreenIfPresent(SCR_FMGR_EXPLORER);
                fmgr_add_marker_for_app(fmgr_p->src_application);
            }
            /* hook PHB func */
#ifdef __MMI_VCARD__
            if ( pfnUnicodeStrcmp((PS8)file_info.file_ext, (S8*)L"VCF") == 0 )
                mmi_vcard_recv_from_file_mgr((PS8)fmgr_p->file_path);
            else
#endif
                mmi_phb_fmgr_from_file_mgr((PS8)fmgr_p->file_path);
//
//             UnicodeToAnsii(nPrintableStr, fmgr_p->file_path);
//             TRACE_EF(g_sw_FMGR, "to phonebook file path = %s",nPrintableStr);
//
            break;

#ifdef __MMI_VCALENDAR__
        case FMGR_ACTION_FWD_TODOLIST:
            /* pass file name to To do list */
            /* hook to do list func */
            mmi_vclndr_fmgr_recv((PS8)fmgr_p->file_path);
            break;
#endif
        case FMGR_ACTION_FWD_SMS:
            /* pass file name to SMS */
            /* hook SMS func */
#ifdef __MMI_VCARD__
            if ( pfnUnicodeStrcmp ((PS8)file_info.file_ext, (S8*)L"VCF") == 0 )
            {
                mmi_vcard_fmgr_sms_forward((PS8)fmgr_p->file_path);
                break;
            }
#endif
#ifdef __MMI_VCALENDAR__
            if ( pfnUnicodeStrcmp ((PS8)file_info.file_ext, (S8*)L"VCS") == 0 )
            {
                mmi_vclndr_fmgr_sms_forward((PS8)fmgr_p->file_path);
                break;
            }
#endif
            break;

#ifdef __MMI_MESSAGES_EMS__
        case FMGR_ACTION_FWD_EMS:
            /* pass file name to EMS */
            if ( (fmgr_p->src_application != APP_FILEMANAGER) && 
            (fmgr_p->src_application != APP_IMAGEVIEWER) &&
            (fmgr_p->src_application != APP_VDOPLY) &&            
            (fmgr_p->sel_type & FMGR_SELECT_BROWSE) )
            {
                /* fwd from browser app to transport app: 
                 needs to delete history for browser app and add marker for it */
                fmgr_adjust_history(SCR_FMGR_EXPLORER);
                DeleteScreenIfPresent(SCR_FMGR_EXPLORER);
                fmgr_add_marker_for_app(fmgr_p->src_application);
            }

            /* hook EMS func */
            if ( FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_IMY) )
                mmi_ems_fm_send_by_ems((PS8)fmgr_p->file_path, EMS_OBJECT_MY_MELODY_STATE);
            else if ( FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_ANM) )
                mmi_ems_fm_send_by_ems((PS8)fmgr_p->file_path, EMS_OBJECT_MY_ANIMATION_STATE);
            else if ( FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_EMS) )
                mmi_ems_fm_send_by_ems((PS8)fmgr_p->file_path, EMS_OBJECT_MY_PICTURE_STATE);
            else
                DisplayPopup((PU8)GetString(STR_FMGR_INVALID_EMS_FORMAT),
                IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
            break;
#endif

//2011-5-27 fzb add
#ifdef __MMI_SEND_MMS_FROM_IMG_VIEW__
        case FMGR_ACTION_FWD_MMS:
		if((fmgr_p->src_application == APP_IMAGEVIEWER) ||
			(fmgr_p->src_application == APP_FILEMANAGER))
			jmms_content_insert_hdlr((PS8)fmgr_p->file_path);
		break;
#endif
#ifdef __MMI_MMS__
        case FMGR_ACTION_FWD_MMS:
            /* pass file name to MMS */
            if ( (fmgr_p->src_application != APP_FILEMANAGER) && 
            (fmgr_p->src_application != APP_IMAGEVIEWER) &&
            (fmgr_p->src_application != APP_VDOPLY) &&            
            (fmgr_p->sel_type & FMGR_SELECT_BROWSE) )
            {
                /* fwd from browser app to transport app: 
                 needs to delete history for browser app and add marker for it */
                fmgr_adjust_history(SCR_FMGR_EXPLORER);
                DeleteScreenIfPresent(SCR_FMGR_EXPLORER);
                fmgr_add_marker_for_app(fmgr_p->src_application);
            }

            /* hook MMS func */
            mms_content_insert_hdlr((PS8)fmgr_p->file_path);
            break;
#endif
#endif

#if defined (MMI_ON_HARDWARE_P) && defined (__MMI_IRDA_SUPPORT__)
        case FMGR_ACTION_FWD_IRDA:
            if ( MMI_IRDA_ERR_SUCCESS == mmi_irda_send_file_obj( (PS8)fmgr_p->file_path, mmi_irda_fmgr_forward_to_irda_callback ) )
            {
                mmi_irda_entry_fmgr_fwd(); 
            }
            break;
#endif
#if defined( __MMI_ACCEL_OPP_SUPPORT__) ||	defined( __MMI_OPP_SUPPORT__)
        case FMGR_ACTION_FWD_BT:
            if (MMI_OPP_ERR_SUCCESS == mmi_opp_discovery_device((PS8) fmgr_p->file_path, mmi_opp_fmgr_forward_to_opp_callback))
            {
                mmi_opp_entry_opp_device_discovery();
            }
            break;
#endif /* __MMI_ACCEL_OPP_SUPPORT__ */ 
        default:
            DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_AVAILABLE), 
            IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
            break;
    }
    mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
    fmgr_p->tree_action = FMGR_ACTION_NONE;
    DeleteScreenIfPresent(SCR_FMGR_FORWARD_OPTION);
}

void fmgr_dummy_function (void)
{
    /*WUZC Modify Start For FMGR Ver:    on 2007-1-17 11:34 */
    StartTimer(FMGR_SCR_TIMER, 1000, GoBackHistory);
    /*WUZC Modify End  For FMGR Ver:    on 2007-1-17 11:34 */
    return;
}

void fmgr_add_marker_for_app(U8 app_id)
{
    history_t   scr101;
    U16 nHistory = 0;

    pfnUnicodeStrcpy((S8*)scr101.inputBuffer, (PS8)&nHistory);
    scr101.scrnID = FMGR_SCR_MARKER;
    switch ( app_id )
    {
#ifdef __MMI_RING_COMPOSER__
        case APP_RINGTONECOMPOSER:
            scr101.entryFuncPtr = EntryRngcApp;
            break;
#endif
#ifdef __MMI_VIDEO__
        case APP_VDOPLY:
            return;
            scr101.entryFuncPtr = mmi_vdoply_entry_app;
            break;
#endif
#ifdef __MMI_CAMERA__
        case APP_IMAGEVIEWER:
            return;
            scr101.entryFuncPtr = mmi_imgview_entry_app;
            break;
#endif
#ifdef __MMI_SOUND_RECORDER__
        case APP_SOUNDRECORDER:
            scr101.entryFuncPtr = mmi_sndrec_entry_main;
            break;
#endif
#ifdef __MMI_AB_REPEATER__
        case APP_ABREPEATER:
            scr101.entryFuncPtr = mmi_abrepeat_entry_list;
            break;
#endif
#ifdef __MMI_IRDA_SUPPORT__
        case APP_EXTDEVICE:
            scr101.entryFuncPtr = entry_fmgr_explorer;
            break;
#endif
#ifdef __MMI_FM_RADIO_RECORD__
        case APP_FMRDO:
            scr101.entryFuncPtr = entry_fmgr_explorer;
            break;
#endif

        default:
            scr101.entryFuncPtr = fmgr_dummy_function;
            break;
    }
    AddHistory(scr101);
}

void mmi_fmgr_select_path_and_enter (U8 app_id, U8 sel_type, FMGR_FILTER filter, PS8 start_path, PsExtFuncPtr callback)
{
mmi_trace(g_sw_FMGR, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
    TBM_ENTRY(0x2845);

    if ( MMI_FS_GetDevStatus(start_path[0], FS_FEATURE_STATE_ENUM) == FS_DEVICE_BUSY )
    {
        DisplayPopup((PU8)GetString(FMGR_FS_DEVICE_BUSY_TEXT), IMG_GLOBAL_ERROR, 0,
        1000, ERROR_TONE);
        //callback(NULL);
        TBM_EXIT(0x2845);
        return;
    }
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
    fmgr_reset_application_highlight_idx();
    /* for other application to enter file manager with a starting path */
    fmgr_p->src_application = app_id;
    fmgr_p->sel_type = sel_type;
    fmgr_p->app_callback = callback;
    fmgr_p->filter_type = filter;
    fmgr_p->tree_action = FMGR_ACTION_NONE;
    fmgr_p->cur_index = fmgr_get_application_highlight_idx();
    fmgr_p->cache_end_idx = 0;
    fmgr_p->cache_start_idx = 0;

    memset ((PU8)fmgr_p->file_cache, 0, sizeof(FMGR_FILE_INFO_STRUCT)*FMGR_CACHE_ENTRY_COUNT);

	/*gaosh Add Start For 6327 Ver: TBM780_T_20070823_2145  on 2007-8-24 13:54 */
	if ( fmgr_p->src_application == APP_FILEMANAGER )
		fmgr_adjust_history(SCR_FMGR_MAIN);
	else
		fmgr_remove_all_screen ();
	/*gaosh Add End  For 6327 Ver: TBM780_T_20070823_2145  on 2007-8-24 13:54 */
	
    switch ( app_id )
    {
        default:
            {
                BOOL add_marker = TRUE;
                fmgr_p->active_explorer_screen = SCR_FMGR_EXPLORER;

                /* init FMGR */
                if ( pfnUnicodeStrcmp((PS8)start_path, (PS8)L"root") == 0 )
                {
                    entry_fmgr_main();
                }
                else
                {
                    pfnUnicodeStrcpy ((PS8)fmgr_p->file_path, (PS8)start_path);
                    entry_fmgr_explorer();
                    if ( fmgr_p->file_count == 0 )
                        add_marker = FALSE;
                }
                if ( add_marker == TRUE &&
                (sel_type & (FMGR_SELECT_FILE | FMGR_SELECT_FILE_SET |FMGR_SELECT_FOLDER | FMGR_SELECT_FILE_OR_FOLDER)) )
                    fmgr_add_marker_for_app (app_id);
            }
    }

    TBM_EXIT(0x2845);
}

void fmgr_return_to_app (void)
{
    FMGR_FILE_INFO_STRUCT file_info;

    if ( fmgr_p->sel_type == FMGR_SELECT_FOLDER && fmgr_p->file_path[0] == MMI_CARD_DRV )
    {
        FS_DiskInfo disk_info;
        U8 path[10];

        /* check write protection */
        kal_wsprintf((PU16)path, "%c:/", fmgr_p->file_path[0]);
        MMI_FS_GetDiskInfo((PU8)path, &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
        if ( disk_info.WriteProtect )
        {
            DisplayPopup((PU8)GetString(FMGR_FS_WRITE_PROTECTION_TEXT), IMG_GLOBAL_ERROR, 0,
            1000, ERROR_TONE);
            //DeleteScreenIfPresent(SCR_FMGR_FOLDER_OPTION);
            fmgr_abort_app_select();
            return;
        }
    }

    // return from drive option menu => select drive, no need to get file info
    memset (&file_info, 0, sizeof (FMGR_FILE_INFO_STRUCT));
    if ( GetExitScrnID() != SCR_FMGR_DRIVE_OPTION )
        mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

#ifdef __DRM_SUPPORT__
    if ( (fmgr_p->sel_type & FMGR_SELECT_FILE) &&
    mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == TRUE
#ifndef __MMI_SHOW_FILE_EXT__
    && mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == TRUE
#endif
    )
    {
        if ( fmgr_select_drm_file() == FALSE )
        {
            DisplayPopup((U8*)GetString(STR_GLOBAL_DRM_PROHIBITED),
            IMG_GLOBAL_WARNING, 0, 1000, ERROR_TONE);
            if ( !(fmgr_p->sel_type & FMGR_SELECT_REPEAT) )
            {
                //DeleteScreenIfPresent(SCR_FMGR_FILE_OPTION);      
                fmgr_abort_app_select();
            }
            return;
        }
    }
#endif /* __DRM_SUPPORT__ */

    if ( GetExitScrnID() != SCR_FMGR_DRIVE_OPTION )
    {
        pBOOL exceed=FALSE;
        /* pass path back to other application */
        if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE )
            exceed = TRUE;
        if ( (file_info.attribute & FS_ATTR_DIR) && 
        mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)L"/") == FALSE )
            exceed = TRUE;
#ifndef __MMI_SHOW_FILE_EXT__
        else
        {
            if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == FALSE )
                exceed = TRUE;
        }
#endif
        if ( exceed == TRUE )
        {
            fmgr_abort_app_select();
            return;
        }
        else
        {
            fmgr_p->app_callback((PU16)fmgr_p->file_path, file_info.is_short);
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
        }
    }
    else
    {
        fmgr_p->app_callback((PU16)fmgr_p->file_path, file_info.is_short);
    }

    DeleteScreenIfPresent(SCR_FMGR_FOLDER_OPTION);
    if ( (fmgr_p->sel_type & FMGR_SELECT_REPEAT) == 0 )
        fmgr_reset_app_select();
}


void fmgr_abort_app_select(void)
{
    if ( fmgr_p->app_callback )
    {
        DeleteUptoScrID(FMGR_SCR_MARKER);
        DeleteScreenIfPresent(FMGR_SCR_MARKER);
        fmgr_p->app_callback(NULL, 0);
        fmgr_reset_app_select();
    }
}

void fmgr_reset_app_select (void)
{
    mmi_trace(g_sw_FMGR, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
    /* de-init FMGR */

    if ( fmgr_p->sel_type & FMGR_SELECT_REPEAT )
    {
        DeleteUptoScrID(FMGR_SCR_MARKER);
        DeleteScreenIfPresent(FMGR_SCR_MARKER);
    }

    fmgr_p->src_application = APP_FILEMANAGER;
    fmgr_p->sel_type = 0;
    fmgr_p->app_callback = NULL;
    FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
    fmgr_p->cache_end_idx = 0;
    fmgr_p->cache_start_idx = 0;
    fmgr_p->cur_index = 0;
    memset ((PU8)fmgr_p->file_path, 0, sizeof (fmgr_p->file_path));
    memset ((PU8)fmgr_p->file_cache, 0, sizeof(FMGR_FILE_INFO_STRUCT)*FMGR_CACHE_ENTRY_COUNT);
}

BOOL fmgr_exe_app_call_back(void)
{
    FMGR_FILE_INFO_STRUCT file_info;

    // return from drive option menu => select drive, no need to get file info
    if ( GetExitScrnID() != SCR_FMGR_DRIVE_OPTION )
        mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

#ifdef __DRM_SUPPORT__
    if ( (fmgr_p->sel_type & FMGR_SELECT_FILE) &&
    mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == TRUE
#ifndef __MMI_SHOW_FILE_EXT__
    && mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == TRUE
#endif
    )
    {
        if ( fmgr_select_drm_file() == FALSE )
        {
            DisplayPopup((U8*)GetString(STR_GLOBAL_DRM_PROHIBITED),
            IMG_GLOBAL_WARNING, 0, 1000, ERROR_TONE);
            if ( !(fmgr_p->sel_type & FMGR_SELECT_REPEAT) )
                fmgr_abort_app_select();
            return FALSE;
        }
    }
#endif /* __DRM_SUPPORT__ */

    DeleteUptoScrID(FMGR_SCR_MARKER);
    DeleteScreenIfPresent(FMGR_SCR_MARKER);

    if ( GetExitScrnID() != SCR_FMGR_DRIVE_OPTION )
    {
        /* pass path back to other application */
        pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)file_info.file_name);
        if ( file_info.attribute & FS_ATTR_DIR )
            pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)L"/");
#ifndef __MMI_SHOW_FILE_EXT__
        else
            pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)file_info.file_ext);
#endif

        fmgr_p->app_callback((PU16)fmgr_p->file_path, file_info.is_short);
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
    }
    else
    {
        fmgr_p->app_callback((PU16)fmgr_p->file_path, file_info.is_short);
    }

    return TRUE;
}

void fmgr_adjust_history(S32 marker)
{
    U16 scrnId;

    if ( marker == 0 )
    {
        /* delete all fmgr screens in history and return to other app */
        if ( DeleteUptoScrID(SCR_FMGR_MAIN) == ST_FAILURE )
        {
            DeleteUptoScrID(fmgr_p->active_explorer_screen);
        }
        //fmgr_return_to_app();
        return;
    }

    scrnId = GetCurrScrnId();
    if ( scrnId > FMGR_SCR_BASE && scrnId < FMGR_SCR_END )
        DeleteUptoScrID((U16)marker);
    else
    {
        for ( scrnId=FMGR_SCR_BASE; scrnId<FMGR_SCR_END; scrnId++ )
            if ( scrnId != SCR_FMGR_MAIN )
                DeleteScreenIfPresent(scrnId);
    }
}

void fmgr_remove_all_screen (void)
{
    U16 scrnId;

    for ( scrnId=FMGR_SCR_BASE; scrnId<FMGR_SCR_END; scrnId++ )
        DeleteScreenIfPresent(scrnId);
}

void fmgr_set_sort_option (U16 sort)
{
    fmgr_p->sort_option = sort;
}

void fmgr_get_sort_option(U16 *sort)
{
    *sort = fmgr_p->sort_option;
}

    #ifdef __DRM_SUPPORT__
BOOL fmgr_select_drm_file (void)
{
    FS_HANDLE handle;
    BOOL set_as = TRUE;
    BOOL fwd_to = TRUE;

    handle = DRM_open_file((PU16)fmgr_p->file_path, FS_READ_ONLY, DRM_PERMISSION_NONE);
    if ( handle > FS_NO_ERROR )
    {
        DRM_RIGHTS_STRUCT *rights;

        if ( (rights = DRM_get_rights(handle)) != NULL )
        {
            if ( (rights->method & DRM_METHOD_SEPARATE_DELIVERY) )
            {
                if ( rights->method & DRM_METHOD_FORWARD_LOCK )
                    fwd_to = FALSE; /* can't forward */
                if ( !(rights->status == DRM_STATUS_RIGHTS_PRESENT &&
                rights->display.type == DRM_CONSTRAINT_NONE && 
                rights->play.type == DRM_CONSTRAINT_NONE) )
                    set_as = FALSE; /* can't set as ringtine wallpaper */
            }
            else
            {
                fwd_to = FALSE; /* can't forward */
                if ( rights->status == DRM_STATUS_RIGHTS_PRESENT &&
                (rights->display.type != DRM_CONSTRAINT_NONE || 
                rights->play.type != DRM_CONSTRAINT_NONE) )
                    set_as = FALSE; /* can't set as ringtone wallpaper */
            }
        }
        DRM_close_file(handle);
    }
    else
    {
        /* can't open file */
        set_as = FALSE;
        fwd_to = FALSE;
    }
    mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
    if ( ((fmgr_p->src_application == APP_PROFILES || 
    fmgr_p->src_application == APP_PHONEBOOK ||
    fmgr_p->src_application == APP_SETTINGS ||
    (fmgr_p->src_application == APP_EMAIL && fmgr_p->sel_type & FMGR_SELECT_FILE_SET)) 
    && set_as == FALSE) ||
    ((fmgr_p->src_application == APP_WAP || 
    fmgr_p->src_application == APP_MESSAGES||
    (fmgr_p->src_application == APP_EMAIL && !(fmgr_p->sel_type & FMGR_SELECT_FILE_SET))) 
    && fwd_to == FALSE) )
        return FALSE;
    else
        return TRUE;
}

U8 fmgr_check_drm_rights(FMGR_FILE_INFO_STRUCT* file_info, U8 permission)
{
    FS_HANDLE handle;
    U8 ok_flag = FMGR_DRM_GRANT_PERMISSION;

    if ( file_info != NULL )
    {
        if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info->file_name) == FALSE
#ifndef __MMI_SHOW_FILE_EXT__
        || mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info->file_ext) == FALSE
#endif
        )
            return FMGR_DRM_NO_PERMISSION;
    }

    handle = DRM_open_file ((PU16)fmgr_p->file_path, FS_READ_ONLY, permission);
    if ( handle >= FS_NO_ERROR )
    {
        DRM_RIGHTS_STRUCT *rights;

        rights = DRM_get_rights (handle);
        if ( rights != NULL )
        {
            /* DRM file */
            if ( DRM_check_rights(rights, permission) == FALSE )
            {
                if ( (rights->method & DRM_METHOD_SEPARATE_DELIVERY) && 
                rights->status == DRM_STATUS_RIGHTS_PENDING )
                {
                    /* request rights from server */
                    ok_flag = FMGR_DRM_REQUEST_RIGHTS;
                    memset (fmgr_p->rights_issuer, 0, sizeof(fmgr_p->rights_issuer));
                    if ( strncmp ((PS8)rights->issuer, "http", 4) != 0 )
                        strcpy ((PS8)fmgr_p->rights_issuer, "http://");
                    strcat ((PS8)fmgr_p->rights_issuer, (PS8)rights->issuer);
                }
                else
                    ok_flag = FMGR_DRM_NO_PERMISSION;
            }
            else if ( file_info == NULL && !(permission & DRM_PERMISSION_EXECUTE) )
            {
                /* show thumbnail won't consume rights */
                /* java will be consumed when play, not install */
                DRM_consume_rights(handle, permission);
            }
        }
        DRM_close_file (handle);
    }
    else
        ok_flag = FMGR_DRM_NO_PERMISSION;

    if ( file_info != NULL )
        mmi_fmgr_remove_last_dir((PS8) fmgr_p->file_path);

    return ok_flag;
}

/*****************************************************************************
* FUNCTION
*	fmgr_get_drm_forward_rule
* DESCRIPTION
*	get forward rule of highlighted file
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
U8 fmgr_get_drm_forward_rule(FMGR_FILE_INFO_STRUCT *file_info, BOOL *is_drm)
{
    FS_HANDLE handle;
    DRM_RIGHTS_STRUCT *rights;
    U8 fwd_flag = FMGR_DRM_FWD_APP_FWD_OUT;

    if ( is_drm != NULL )
        *is_drm = FALSE;
    if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info->file_name) == TRUE
#ifndef __MMI_SHOW_FILE_EXT__
    && mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info->file_ext) == TRUE
#endif
    )
    {
        handle = DRM_open_file((PU16)fmgr_p->file_path, FS_READ_ONLY, DRM_PERMISSION_NONE);
        if ( handle > FS_NO_ERROR )
        {
            if ( (rights = DRM_get_rights(handle)) != NULL )
            {
                DRM_CONSTRAINT_STRUCT *cons;
                U8 permission = DRM_PERMISSION_NONE;

                if ( is_drm != NULL )
                    *is_drm = TRUE;
                if ( FMGR_FILTER_IS_SET_AUDIO(&file_info->file_type) )
                {
                    cons = &rights->play;
                    permission |= DRM_PERMISSION_PLAY;
                }
                else if ( FMGR_FILTER_IS_SET_IMAGE(&file_info->file_type) )
                {
                    cons = &rights->display;
                    permission |= DRM_PERMISSION_DISPLAY;
                }
                else
                    cons = NULL;

                if ( rights->method & DRM_METHOD_SEPARATE_DELIVERY )
                {
                    if ( !(rights->status == DRM_STATUS_RIGHTS_PRESENT &&
                    (rights->permission & permission) &&
                    cons != NULL &&
                    cons->type == DRM_CONSTRAINT_NONE ) )
                    {
                        if ( rights->method & DRM_METHOD_FORWARD_LOCK )
                            fwd_flag = FMGR_DRM_NO_FWD_APP_NO_FWD_OUT; /* can't forward, can't set as ringtine wallpaper */
                        else
                            fwd_flag = FMGR_DRM_NO_FWD_APP_FWD_OUT; /* can forward, can't set as ringtone wallpaper */
                    }
                }
                else if ( rights->method & DRM_METHOD_COMBINED_DELIVERY )
                {
                    if ( (rights->permission & permission) &&
                    cons!= NULL && cons->type == DRM_CONSTRAINT_NONE )
                        fwd_flag = FMGR_DRM_FWD_APP_NO_FWD_OUT; /* can't forward, can set as ringtone wallpaper */
                    else
                        fwd_flag = FMGR_DRM_NO_FWD_APP_NO_FWD_OUT; /* can't forward, can't set as ringtine wallpaper */
                }
                else
                    fwd_flag = FMGR_DRM_FWD_APP_NO_FWD_OUT; /* can't forward, can set as ringtone wallpaper */
            }
            DRM_close_file(handle);
        }
        else
        {
            /* open file fail */
            fwd_flag = FMGR_DRM_NO_FWD_APP_NO_FWD_OUT;
        }
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
    }
    return fwd_flag;
}

void entry_fmgr_request_rights_confirm(void)
{
    DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, 
    (PU16)GetString(STR_FMGR_REQ_RIGHTS), NULL, 0);

    DeleteScreenIfPresent(SCR_FMGR_FILE_OPTION);
    SetLeftSoftkeyFunction(fmgr_send_rights_request, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void entry_fmgr_request_unknown_rights_confirm(void)
{
    DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO, IMG_GLOBAL_NO, 
    (PU16)GetString(STR_FMGR_INVALID_REQ_RIGHTS), NULL, 0);

    DeleteScreenIfPresent(SCR_FMGR_FILE_OPTION);
    SetLeftSoftkeyFunction(fmgr_send_rights_request, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void fmgr_send_rights_request (void)
{
    ilm_struct *ilm_ptr = NULL;
    wap_browser_startup_req_struct *param_ptr;

    param_ptr = (wap_browser_startup_req_struct*)construct_local_para(sizeof(wap_browser_startup_req_struct), TD_CTRL);
    param_ptr->type = 2;
    strcpy ((char*)param_ptr->url, (char*)fmgr_p->rights_issuer);

    ilm_ptr = allocate_ilm(MOD_MMI);
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;
    ilm_ptr->msg_id         = MSG_ID_WAP_BROWSER_STARTUP_REQ;
    ilm_ptr->peer_buff_ptr  = NULL;

    SEND_ILM(MOD_MMI, MOD_WAP, WAP_MMI_SAP, ilm_ptr);
}
    #endif

U8 fmgr_check_file_present(void)
{
	//char filename[256];
    FMGR_FILE_INFO_STRUCT file_info;
    S8 *pFileName = NULL;
    FILE_HANDLE handle = -1;
    S32 fs_ret;
    U16 index;
    U8 flag = FMGR_NOT_EXIST;
    U32 iLen = 0;

    index = pfnUnicodeStrlen((PS8)fmgr_p->file_path)*2;
    memset ((PS8)fmgr_p->file_path+index, 0, FMGR_MAX_FILE_NAME_LEN*ENCODING_LENGTH-index);

    fs_ret = MMI_FS_SetCurrentDir((PU8)fmgr_p->file_path);

    if ( fs_ret < 0 )
    {
        return flag;
    }

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

//    iLen = pfnUnicodeStrlen(file_info.file_name ) + pfnUnicodeStrlen(file_info.file_ext) ; 
	iLen = pfnUnicodeStrlen(file_info.file_name ); 
    if ( iLen >  FMGR_MAX_FILE_NAME_LEN)
    {
        TRACE_EF(g_sw_FMGR, " iLen >  FMGR_MAX_FILE_NAME_LEN iLen = %d." , iLen);
        return FMGR_FILENAME_TOO_LONG;
    }

    pFileName = OslMalloc( iLen*2  + LEN_FOR_UNICODE_NULL_CHAR);
    if ( pFileName == NULL )
    {
        return FMGR_MALLOC_ERR;
    }
    
    if ( file_info.attribute & FS_ATTR_DIR )
    {
        /* is folder */
        handle = MMI_FS_Open((PU8)file_info.file_name, FS_OPEN_DIR | FS_READ_ONLY);
        flag = FMGR_FOLDER;
    }
    else
    {
        /* is file */
        pfnUnicodeStrcpy(pFileName, file_info.file_name);
#ifndef __MMI_SHOW_FILE_EXT__
        pfnUnicodeStrcat((PS8)pFileName, (PS8)(file_info.file_ext));
#endif
       
        handle = MMI_FS_Open((PU8)pFileName, FS_READ_ONLY);
        flag = FMGR_FILE;
    }

    if ( handle >= 0 )
    {
        MMI_FS_Close (handle);
        TRACE_EF(g_sw_FMGR, "colse open handle,fs_ret=%d,handle =%d,flag = %d",fs_ret,handle,flag);
    }
    else
    {  
        TRACE_EF(g_sw_FMGR, "fmgr_check_file_present() handle < 0");

        if ( FS_FILE_OPENED == handle )
        {
            flag = FMGR_FILE_OPENED;
            TRACE_EF(g_sw_FMGR, "fmgr_check_file_present() FMGR_FILE_OPENED");
        }
        else if (FS_FILENAME_TOO_LONG == handle)
        {
            flag = FMGR_FILENAME_TOO_LONG;
            DisplayPopup((PU8)GetString(STR_ID_FMGR_FILENAME_TOO_LONG),
                IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
        }
        else
        {
            flag = FMGR_NOT_EXIST;
            DisplayPopup((PU8)GetString(GetFileSystemErrorString(handle)),
                IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);

            /* go back to root, reload file manager */
            if ( fmgr_p->src_application == APP_FILEMANAGER )
            {
                fmgr_adjust_history(SCR_FMGR_MAIN);
            }
            else
            {
                fmgr_adjust_history(0);
            }

        }
        
    }

    if (pFileName != NULL)
    {
        OslMfree(pFileName);
        pFileName = NULL;
    }
    
    return flag;
}

pBOOL fmgr_is_parent2_present(void)
{
    FMGR_FILE_INFO_STRUCT file_info;
    FILE_HANDLE handle;
    S8 *pFileName = NULL;
   
    S32 fs_ret;
    U16 index;
    U8 flag = FMGR_NOT_EXIST;
    U32 iLen = 0;
	
    index = pfnUnicodeStrlen((PS8)fmgr_p->file_path)*2;
    memset ((PS8)fmgr_p->file_path+index, 0, FMGR_MAX_FILE_NAME_LEN*ENCODING_LENGTH-index);

    fs_ret = MMI_FS_SetCurrentDir((PU8)fmgr_p->file_path);

    if ( fs_ret < 0 )
    {
        return flag;
    }

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

    iLen = pfnUnicodeStrlen(file_info.file_name ); 
    if ( iLen >  FMGR_MAX_FILE_NAME_LEN)
    {
        TRACE_EF(g_sw_FMGR, " iLen >  FMGR_MAX_FILE_NAME_LEN iLen = %d." , iLen);
        return FMGR_FILENAME_TOO_LONG;
    }

#if 1//gdm 080424
    pFileName = OslMalloc( iLen*2  + LEN_FOR_UNICODE_NULL_CHAR);
    if ( pFileName == NULL )
    {
        return FMGR_MALLOC_ERR;
    }
    
    if ( file_info.attribute & FS_ATTR_DIR )
    {
        /* is folder */
        handle = MMI_FS_Open((PU8)file_info.file_name, FS_OPEN_DIR | FS_READ_ONLY);
        flag = FMGR_FOLDER;
    }
    else
    {
        /* is file */
        pfnUnicodeStrcpy(pFileName, file_info.file_name);
#ifndef __MMI_SHOW_FILE_EXT__
        pfnUnicodeStrcat((PS8)pFileName, (PS8)(file_info.file_ext));
#endif
        handle = MMI_FS_Open((PU8)pFileName, FS_READ_ONLY);
        flag = FMGR_FILE;
    }

#endif

    if ( handle >= 0 )
    {
        MMI_FS_Close (handle);
        TRACE_EF(g_sw_FMGR, "close handle,fs_ret=%d,handle =%d,flag = %d",fs_ret,handle,flag);
        if (pFileName != NULL)
        {
            OslMfree(pFileName);
            pFileName = NULL;
        }
        return TRUE;
    }
    else
    {
        if (pFileName != NULL)
        {
            OslMfree(pFileName);
            pFileName = NULL;
        }
        return FALSE;
    }
}


pBOOL fmgr_is_parent_present(void)
{
    FILE_HANDLE handle = FS_ERROR_RESERVED;

    handle = MMI_FS_Open((PU8)fmgr_p->file_path, FS_OPEN_DIR | FS_READ_ONLY);
    if ( handle >= 0 )
    {
        MMI_FS_Close (handle);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#endif /* __MMI_FILE_MANAGER__ */

U16 fmgr_get_app_title_icon_id (void)
{
    U16 iconId;

    switch ( fmgr_p->src_application )
    {
#ifdef __MMI_CAMERA__
        case APP_IMAGEVIEWER:
            iconId = GetRootTitleIcon(MENU_ID_IMGVIEW_APP);
            break;
        case APP_CAMERA:
            iconId = GetRootTitleIcon(MENU_ID_CAMERA_APP);         
            break;
#endif
#ifdef __MMI_RING_COMPOSER__
        case APP_RINGTONECOMPOSER:
            iconId = GetRootTitleIcon(MENU3103_RINGTONE_COMPOSER);
            break;
#endif         
#ifdef __MMI_SOUND_RECORDER__
        case APP_SOUNDRECORDER:
            iconId = GetRootTitleIcon(MENU_ID_SNDREC_MAIN);
            break;
#endif
#ifdef __MMI_AB_REPEATER__
        case APP_ABREPEATER:
            iconId = GetRootTitleIcon(MENU_ID_AB_REPEATER_MAIN);
            break;
#endif
#ifdef __MMI_AUDIO_PLAYER__
        case APP_AUDIOPLAYER:
            iconId = MAIN_MENU_TITLE_MULTIMEDIA_ICON;
            break;
#endif
#ifdef __MMI_VIDEO__
        case APP_VDOPLY:
            iconId = GetRootTitleIcon(MENU_ID_VDOPLY_APP);
            break;
        case APP_VDOREC:
            iconId = GetRootTitleIcon(MENU_ID_VDOREC_APP);
            break;
#endif
#ifdef __MMI_EMAIL__
        case APP_EMAIL:
            iconId = GetRootTitleIcon(MENU_ID_EMAIL_MAIN);
            break;
#endif
#if 0
/* under construction !*/
/* under construction !*/
#endif
#ifdef __MMI_TODOLIST__
        case APP_TODOLIST:
            iconId = GetRootTitleIcon(ORGANIZER_TODOLIST_MENU);
            break;
#endif         
#ifdef __MMI_IRDA_SUPPORT__
        case APP_EXTDEVICE:
            iconId = GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID);
            break;
#endif         
        case APP_PHONEBOOK:
            iconId = GetRootTitleIcon(MAIN_MENU_PHONEBOOK_MENUID);
            break;
        case APP_SETTINGS:
            iconId = GetRootTitleIcon(MAIN_MENU_SETTINGS_MENUID);
            break;
        case APP_MESSAGES:
            iconId = GetRootTitleIcon(MAIN_MENU_MESSAGES_MENUID);
            break;
        case APP_PROFILES:
            iconId = GetRootTitleIcon(MAIN_MENU_PROFILES_MENUID);
            break;
#ifdef __MMI_MAINLCD_220X176__
        case APP_ALARM:
            iconId = GetRootTitleIcon(MAIN_MENU_ALARM_MENUID);
            break;
        case APP_TOOLS:
            iconId = GetRootTitleIcon(MAIN_MENU_TOOLS_MENUID);
            break;			
#endif
        case APP_FILEMANAGER:
            iconId = GetRootTitleIcon(MAIN_MENU_FILE_MNGR_MENUID);
            break;
    #ifdef __MMI_FTS_SUPPORT__
        case APP_FTP:
            iconId = GetRootTitleIcon(MENU_CONN_FTP_SETTING);
            break;
    #endif /* __MMI_FTS_SUPPORT__ */ 
    #ifdef __MMI_EBOOK_READER__
        case APP_EBOOK:
            iconId = GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID);
            break;
    #endif /* __MMI_EBOOK_READER__ */ 
        default:
            iconId = 0;
            break;
    }
    return iconId;
}

pBOOL fmgr_is_msdc_present(void)
{
#ifndef MMI_ON_HARDWARE_P
    return g_tflashCardExist;
#else
    //if ( MMI_FS_GetDevStatus (fmgr_p->drv_list[FMGR_REMOVABLE_STORAGE][0], FS_MOUNT_STATE_ENUM) == FS_MSDC_MOUNT_ERROR )
       if(g_tflashCardExist)
       {
            return TRUE;
       }
        else
        {
            return FALSE;
        }
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
#endif
}

/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
pBOOL fmgr_is_sec_msdc_present(void)
{
#ifndef MMI_ON_HARDWARE_P
    return g_tflashCard2Exist;
#else
    //if ( MMI_FS_GetDevStatus (fmgr_p->drv_list[FMGR_REMOVABLE_STORAGE][0], FS_MOUNT_STATE_ENUM) == FS_MSDC_MOUNT_ERROR )
       if(g_tflashCard2Exist)
       {
            return TRUE;
       }
        else
        {
            return FALSE;
        }
#endif
}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ 

void highlight_fmgr_activate_theme(void)
{
    SetKeyHandler(fmgr_Activate_theme, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction( fmgr_Activate_theme, KEY_EVENT_UP );
}

void fmgr_Activate_theme(void)
{
    FMGR_FILE_INFO_STRUCT file_info;

    if ( fmgr_check_file_present() < FMGR_NO_ERROR )
        return;

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);
    switch ( mmi_activatetheme_frmfilemanager(fmgr_p->file_path,(PS8)file_info.file_name) )
    {
        case 0:
            fmgr_display_themepopUp();
            break;
        case 1:
            break;
        default:
            break;
    }


}
void fmgr_display_themepopUp()
{
    DisplayConfirm(STR_FMGR_MOVE, IMG_GLOBAL_YES, STR_GLOBAL_BACK,IMG_GLOBAL_BACK, 
    get_string(STR_ID_FMGR_THEME_INCORRECT_LOCATION), IMG_GLOBAL_QUESTION, WARNING_TONE);

    SetLeftSoftkeyFunction(fmgr_move_theme_file, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
void fmgr_move_theme_file()
{
    char name[20];
    FMGR_FILE_INFO_STRUCT file_info;
    S32 count;
    S32 create_result;
    FS_HANDLE   file_handle = FS_ERROR_RESERVED;

    pfnUnicodeStrcpy((PS8)name,(PS8)FMGR_DEFAULT_FOLDER_THEMES);

    fmgr_p->tree_action = FMGR_ACTION_MOVE;
    if ( fmgr_check_file_present() < FMGR_NO_ERROR )
    {
        fmgr_p->enter_folder = FALSE;
        FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
        fmgr_p->tree_action = FMGR_ACTION_NONE;
        return;
    }


    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

    pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)file_info.file_name);
#ifndef __MMI_SHOW_FILE_EXT__
    pfnUnicodeStrcat((PS8)fmgr_p->file_path, (PS8)file_info.file_ext);
#endif

    pfnUnicodeStrcpy((PS8)fmgr_p->old_file_path, (PS8)fmgr_p->file_path);

    /* short file name src file should be renamed to long file name */
#ifdef MMI_ON_HARDWARE_P
    if ( file_info.is_short == TRUE )
    {
        PS8 temp, temp1;

        temp = mmi_fmgr_extract_file_name((PS8)fmgr_p->file_path);
        temp1 = mmi_fmgr_extract_file_name((PS8)fmgr_p->old_file_path);
        mmi_chset_mixed_text_to_ucs2_str((U8*)temp1, (MAX_SUBMENU_CHARACTERS-1) * ENCODING_LENGTH, (U8*)temp, PhnsetGetDefEncodingType() );
        FS_Rename((PU16)fmgr_p->file_path, (PU16)fmgr_p->old_file_path);
    }
#endif
    mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);

    while ( pfnUnicodeStrlen((PS8)fmgr_p->file_path) >=3 )
    {

        if ( pfnUnicodeStrlen((PS8)fmgr_p->file_path) == 3 )
        {

            break;
        }
        mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);

    }

    mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, name);

    file_handle = MMI_FS_Open((kal_uint16*)fmgr_p->file_path, FS_OPEN_DIR | FS_READ_ONLY);

    /* path already exist */
    if ( file_handle>=0 )
    {
        FS_Close(file_handle);
        create_result=0;
    }
    else
    {
        create_result = MMI_FS_CreateDir((PU16)fmgr_p->file_path);  
        pfnUnicodeStrcpy((PS8)gtheme_folder_path, (PS8)fmgr_p->file_path);
        g_istheme_folder_created=TRUE;
    }

    if ( mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == TRUE )
    {
        //mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext);
        mmi_fmgr_send_copy_req(fmgr_p->tree_action, (PU8)fmgr_p->old_file_path,
        (PU8)fmgr_p->file_path, (PsFuncPtr)fmgr_theme_move_rsp);
        entry_fmgr_copy_animation();
    }

}
void fmgr_theme_move_rsp(void *info)
{
    U16 flag;
    FMGR_FILE_INFO_STRUCT file_info;
    U8 activeflag;



    fmgr_download_move_sucess=FALSE;
    fmgr_copy_rsp_hdlr(info);
    if ( fmgr_download_move_sucess )
    {
        mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

        flag=mmi_activatetheme_frmfilemanager(fmgr_p->file_path,file_info.file_name);
    }
    fmgr_download_move_sucess=FALSE;
    g_istheme_folder_created=FALSE;

    //GobackHistory();
}
void fmgr_theme_delete_highlight()
{
    SetLeftSoftkeyFunction(fmgr_display_theme_delte_popUp, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void fmgr_theme_single_delete()
{
    FMGR_FILE_INFO_STRUCT file_info;
    S8 name[100];
    S16 flag,status;
    int count;
    S32 error_type;

    flag = fmgr_check_file_present();

    if ( flag <= FMGR_NO_ERROR )
        return;

    mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);
    flag=check_file_location(fmgr_p->file_path);
    if ( flag )
    {

        status=tm_delete_theme_from_fmgr(flag,(PU8)file_info.file_name, &error_type);

        switch ( status )
        {
            case THEME_ERROR_SUCCESS:
                DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0,
                1000, SUCCESS_TONE);
                break;
            case THEME_ERROR_ALREADY_ACTIVATED:
                DisplayPopup((PU8)get_string(STR_ID_FMGR_ACTIVE_THEME_DELETE), IMG_GLOBAL_ERROR, 0,
                2000, ERROR_TONE);


                break;
            default:
                DisplayPopup((PU8)GetString(GetFileSystemErrorString(error_type)),IMG_GLOBAL_ERROR, TRUE, 2000,ERROR_TONE);
                break;

        }
        if ( MMI_FS_Count ((PU16)fmgr_p->file_path, FS_DIR_TYPE | FS_FILE_TYPE | FS_FILTER_SYSTEM_ATTR) == 0 )
            mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);

        fmgr_p->enter_folder = TRUE;
        DeleteScreenIfPresent(SCR_FMGR_FILE_OPTION);
    }
    else
    {


        //pfnUnicodeStrcpy((PS8)name,(PS8)file_info.file_name);
        // mmi_fmgr_append_file_name((PSstr_global_8)name, (PS8)file_info.file_ext);
        //activeflag=is_mmi_theme_file_active(name);
        //if(activeflag)                     
        //	fmgr_display_theme_delte_popUp();
        //else
        fmgr_delete();
    }


}

void fmgr_display_theme_delte_popUp()
{
    DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO, 
    get_string(STR_GLOBAL_DELETE), IMG_GLOBAL_QUESTION, WARNING_TONE);

    SetLeftSoftkeyFunction(fmgr_theme_single_delete, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}


U8 mmi_activatetheme_frmfilemanager(PS8 path,PS8 filename)
{
    //char name1[40],name[100];
    int flag,status;

    flag=check_file_location(path);
    EntryNewScreen(0, NULL, NULL, NULL);
    ShowCategory66Screen(STR_SCR3001_THEMES_CAPTION,0, 0, 0, 0, 0, (PU8)GetString(STR_ID_DLT_THM_PROGRESS), IMG_THEME_ACTIVATION, NULL);

    switch ( flag )
    {
        case THEME_TYPE_DOWNLOADED_PHONE:
            status=tm_activate_theme_from_fmgr(THEME_TYPE_DOWNLOADED_PHONE,(PU8)filename);
            break;
        case THEME_TYPE_DOWNLOADED_CARD:
            status=tm_activate_theme_from_fmgr(THEME_TYPE_DOWNLOADED_CARD,(PU8)filename);
            break;


    }
    if ( flag )
    {
        HandleThemeActivationErrorStatus((U16)status);
    }

    return flag;
}



U16 is_mmi_theme_file_active(PS8 file_name)
{

    if ( !pfnUnicodeStrcmp(gtest_theme_file,file_name) )
        return TRUE;
    else
        return FALSE;
}

S16 get_storage_type(PS8 path)
{
    if ( !pfnUnicodeStrncmp(path,"C:/",3) )
        return THEME_TYPE_DOWNLOADED_PHONE;
    else if ( !pfnUnicodeStrncmp(path,"D:/",3) )
        return THEME_TYPE_DOWNLOADED_CARD;
    else return -1;
}

U16 check_file_location(PS8 path)
{
    S8  buf_UCS2[FMGR_PATH_BUFFER_SIZE];
    S8  buf_UCS21[FMGR_PATH_BUFFER_SIZE];
    S8  ascii_path[FMGR_PATH_BUFFER_SIZE];
//	#ifdef MMI_ON_HARDWARE_P
    S8 drive;
    S16 len=0;
//	#endif


    UnicodeToAnsii(ascii_path,path);
    while ( ascii_path[len]!='\0' )
    {
        ascii_path[len]=(char )toupper(ascii_path[len]);
        len++;
    }


    if ( ((drive = MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM)) >= 'A') &&
    (drive <= 'Z') )
    {
        sprintf ((PS8)buf_UCS2, "%c:/", drive);

        //AnsiiToUnicodeString(buf_UCS2,root);
        //pfnUnicodeStrcat((PS8)buf_UCS2, (PS8)MMI_PHONE_THEME);
        strcat(buf_UCS2,MMI_THEME_LOCATION);
    }
    if ( ((drive = MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE)) >= 'A') &&
    (drive <= 'Z') )
    {
        /* Removable existed */


        sprintf ((PS8)buf_UCS21, "%c:/", drive);

        //AnsiiToUnicodeString(buf_UCS21,MMI_CARD_THEME);
        strcat(buf_UCS21,MMI_THEME_LOCATION);
    }

    /*if(!pfnUnicodeStrcmp(path,buf_UCS2))
     return THEME_TYPE_DOWNLOADED_PHONE;
    else if (!pfnUnicodeStrcmp(path,buf_UCS21))
     return THEME_TYPE_DOWNLOADED_CARD;
    else return 0;*/

    if ( !strcmp(ascii_path,buf_UCS2) )
        return THEME_TYPE_DOWNLOADED_PHONE;
    else if ( !strcmp(ascii_path,buf_UCS21) )
        return THEME_TYPE_DOWNLOADED_CARD;
    else return 0;


}

void theme_folder_delete_rsp_hdlr(void *info)
{
    g_istheme_folder_created=FALSE;
}

#endif
#ifdef __MMI_EBOOK_READER__


/*****************************************************************************
 * FUNCTION
 *  highlight_fmgr_open_ebook
 * DESCRIPTION
 *  Highlight function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void highlight_fmgr_open_ebook(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(fmgr_open_ebook_hdlr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(fmgr_open_ebook_hdlr, KEY_EVENT_UP);
#ifdef __MMI_TOUCH_SCREEN__
    wgui_register_list_item_selected_callback(fmgr_open_ebook_hdlr);
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  fmgr_open_ebook_hdlr
 * DESCRIPTION
 *  Open book
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void fmgr_open_ebook_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FMGR_FILE_INFO_STRUCT file_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_DOWNLOAD_AGENT__
    /* download agent file */
    if (fmgr_is_da_file())
    {
     	PS8 file_name;
        FS_HANDLE handle = FS_ERROR_RESERVED;

        /* Check if file exists or not */
        handle = MMI_FS_Open((PU16) fmgr_p->da_file_path, FS_READ_ONLY);
        if (handle >= FS_NO_ERROR)
        {
        	MMI_FS_Close(handle);
        }
        else
        {
            DisplayPopup((PU8) GetString(GetFileSystemErrorString(handle)), IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
            DeleteScreenIfPresent(SCR_FMGR_DA_FILE_OPTION);
            return;
        }
        pfnUnicodeStrcpy((PS8) fmgr_p->file_path, (PS8) fmgr_p->da_file_path);
        file_name = mmi_fmgr_extract_file_name((PS8) fmgr_p->file_path);
        pfnUnicodeStrcpy ((PS8)file_info.file_name, file_name);
        mmi_fmgr_remove_last_dir((PS8) fmgr_p->file_path);
        file_info.is_short = FALSE;
    }
    /* non download agent file */
    else
#endif /* __MMI_DOWNLOAD_AGENT__ */ 
    {
        mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);
        if (fmgr_check_file_present() < FMGR_NO_ERROR)
        {
            return;
        }
    }

	 ebr_frm_open_book_from_file_manager((S8*)file_info.file_name,(S8*)fmgr_p->file_path, 0, file_info.is_short, FALSE);
}

/*****************************************************************************
 * FUNCTION
 *  ebr_fmgr_copy_new_file_path_after_move
 * DESCRIPTION
 *  Copy the new path
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_fmgr_copy_new_file_path_after_move(S8* new_file_path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	pfnUnicodeStrcpy((PS8)fmgr_p->file_path,(PS8)new_file_path);
	fmgr_p->cur_index = 0;
	fmgr_p->tree_action = FMGR_ACTION_NONE;
    fmgr_p->enter_folder = TRUE;
}
#endif /* __MMI_EBOOK_READER__ */ 

#ifdef __TFLASH_MOUNT_DYNAMIC__

void fmgr_add_TFlash_content(void)
{
    S8 drive;
    
    g_hasMountedTFlashFS = TRUE;
    g_tflashCardExist = TRUE;

    if ( ((drive = MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE)) >= 'A') &&
    (drive <= 'Z') )
    {
        fmgr_p->drv_count++;
        sprintf ((PS8)fmgr_p->drv_list[FMGR_REMOVABLE_STORAGE], "%c:/", drive);
    }

}

void fmgr_remove_TFlash_content(void)
{
    S8 drive;

    if ( ((drive = MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE)) >= 'A') &&
    (drive <= 'Z') )
    {
        fmgr_p->drv_count--;
        sprintf ((PS8)fmgr_p->drv_list[FMGR_REMOVABLE_STORAGE], "%c:/", drive);
    }
    g_hasMountedTFlashFS = FALSE;
    g_tflashCardExist = FALSE;

}
#endif


//support APP only defined by APPLICATION_DEPLOYxxxxxxx
S32 gAppHighlightIdx = 0;
S32 fmgr_get_application_highlight_idx(void )
{
    S32 t;
    mmi_trace(g_sw_FMGR,"fmgr_get_application_highlight_idx " );
    
     t = gAppHighlightIdx ;
     gAppHighlightIdx = 0;
    return t;

}

void fmgr_set_application_highlight_idx(void )
{
    mmi_trace(g_sw_FMGR,"fmgr_set_application_highlight_idx " );
    gAppHighlightIdx = fmgr_p->cur_index;
    return;
        
}

void fmgr_reset_application_highlight_idx(void )
{
    mmi_trace(g_sw_FMGR,"fmgr_reset_application_highlight_idx " );
    gAppHighlightIdx = 0;
    return;
        
} 

