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
 *	 FileMgr.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  File Manager Header
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _FILE_MGR_H_
#define _FILE_MGR_H_

#include "gui_data_types.h"
#include "datetimetype.h"
#include "fmt_def.h"
#include "globalconstants.h" /* wufasong added 2007.03.22 */

/* Notice - Include "FileSystemDef.h" before this file */
/************************************************************************/
/* Define                                                               */
/************************************************************************/
 
#define FMGR_MAX_FILE_LEN				(MAX_SUBMENU_CHARACTERS) /* Same size as menuitem's length */
#define FMGR_MAX_FILE_NAME_LEN             (255)
#define FMGR_MAX_PATH_LEN				(260)			
#define FMGR_MAX_EXT_LEN				(5)
#define FMGR_MAX_INPUT_FILE_LEN		(FMGR_MAX_FILE_LEN - FMGR_MAX_EXT_LEN)

/* path buffer size */
#define FMGR_PATH_BUFFER_SIZE			(520)
#define FMGR_PATH_CHAR_COUNT			(259)

/* image cache */
 

 
#define FMGR_THUMB_IMAGE_CACHE_MAX_COUNT			(30)	/* may cache up to 30 images */
#define FMGR_THUMB_IMAGE_HANDLE_MAX_COUNT			(10)	/* may decode up to 10 images on a screen at a time */
#define FMGR_THUMB_IMAGE_BUFFER_SIZE				(60)	/* 60 k */ /* this buffer imporve viewing performace alot !! */
#define FMGR_DELAY_DECODER_COUNT						(10)

/* file catch entry */
#ifdef __MMI_SLIM_FILE_MANAGER__
#define FMGR_CACHE_ENTRY_COUNT						(10)
#else
#define FMGR_CACHE_ENTRY_COUNT						(32)
#endif
#define FMGR_CACHE_SHORT_FILE_LEN					FMGR_MAX_FILE_LEN*ENCODING_LENGTH



typedef enum
{
/*WUZC Del Start For MMIFS Ver:    on 2007-1-19 11:3 */
   #if 0
   FMGR_NAND_STORAGE,
   #endif
/*WUZC Del End  For MMIFS Ver:    on 2007-1-19 11:3 */
   FMGR_NOR_STORAGE,
#ifdef __DRM_V01__
   FMGR_DRM_STORAGE,
#endif
#ifdef __DRM_SUPPORT__
   FMGR_DRM_RO_STORAGE,
#endif
   FMGR_REMOVABLE_STORAGE,
#if defined(DUAL_TFLASH_SUPPORT)
   FMGR_STORAGE_TFCARD_SECOND,
#endif
   FMGR_TOTAL_STORAGE
} fmgr_drive_enum;

typedef enum
{
   FMGR_NOT_EXIST,
   FMGR_FILENAME_TOO_LONG,
   FMGR_MALLOC_ERR,
   FMGR_NO_ERROR,
   FMGR_FILE,
   FMGR_FOLDER,
   FMGR_FILE_OPENED
   } fmgr_attr_enum;

typedef enum
{
	FMGR_DELETE_NONE,
	FMGR_DELETE_SINGLE,
	FMGR_DELETE_FOLDER,
	FMGR_DELETE_ALL
} fmgr_delete_enum;

#define FMGR_SELECT_FILE   				0x01
#define FMGR_SELECT_FOLDER                   0x02
#define FMGR_SELECT_FILE_OR_FOLDER     0x04
#define FMGR_SELECT_BROWSE					0x08
#define FMGR_SELECT_REPEAT                   0x10
#define FMGR_SELECT_FILE_SET                0x20

typedef enum
{
   FMGR_ACTION_NONE,
   FMGR_ACTION_FORMAT,
   FMGR_ACTION_CHECK_DRIVE,
   FMGR_ACTION_UNMOUNT,
   FMGR_ACTION_COPY,
   FMGR_ACTION_MOVE,
   FMGR_ACTION_FWD_WALLPAPER,
   FMGR_ACTION_FWD_SUB_WALLPAPER,
   FMGR_ACTION_FWD_SCREENSAVER,
   FMGR_ACTION_FWD_POWER_ON,
   FMGR_ACTION_FWD_POWER_OFF,
   FMGR_ACTION_FWD_PHONEBOOK,   
   FMGR_ACTION_FWD_TODOLIST,
   FMGR_ACTION_FWD_PROFILE,
    FMGR_ACTION_FWD_BGSOUND,
   FMGR_ACTION_FWD_SMS,
   FMGR_ACTION_FWD_MMS,
   FMGR_ACTION_FWD_EMS,
   FMGR_ACTION_FWD_EMAIL,
   FMGR_ACTION_FWD_IRDA,
    FMGR_ACTION_FWD_BT,
    FMGR_ACTION_PRINT_TO_PICTBRIGE,
    FMGR_ACTION_PRINT_TO_BLUETOOTH    
} fmgr_tree_action_enum;

typedef enum
{
   FMGR_DRM_GRANT_PERMISSION,
   FMGR_DRM_NO_PERMISSION,
   FMGR_DRM_REQUEST_RIGHTS
} fmgr_drm_status_enum;

typedef enum
{
   FMGR_DRM_NO_FWD_APP_NO_FWD_OUT,
   FMGR_DRM_FWD_APP_NO_FWD_OUT,
   FMGR_DRM_NO_FWD_APP_FWD_OUT,
   FMGR_DRM_FWD_APP_FWD_OUT
} fmgr_drm_forward_enum;

/************************************************************************/
/* Data Structure                                                       */
/************************************************************************/
/* Catch struct */
typedef S16	FMGR_INDEX;

typedef struct
{
	S8				file_name[(FMGR_MAX_FILE_NAME_LEN+1)*ENCODING_LENGTH];
	S8				file_ext[(FMGR_MAX_EXT_LEN+1)*ENCODING_LENGTH];
	FMGR_FILTER	file_type;
	U32			file_size;
	U16		   index;
	/* creation date */
	MYTIME		time;
	/* file info */
	U8				attribute;
	BOOL			is_used;
	BOOL			is_short;
	BOOL			is_able_display;
} FMGR_FILE_INFO_STRUCT;




/*
*  Some applcation that wish to access fmgr caching system 
*	instead of using explorer may directly access these function  
*	ex: Audio player, Sound Recorder, Image Viewer 
*/

/* callback function for UI Display. For Categories 212, 213, 214 */
extern pBOOL 	mmi_fmgr_get_drv_item_callback(S32 item_idx, UI_string_type str_buff, UI_image_type *img_buff_p, U8	str_img_mask);
extern S32		mmi_fmgr_get_drv_hint_callback(S32 item_idx, UI_string_type *hint_array);
extern void		mmi_fmgr_draw_drv_info_callback(S32 item_idx, S32 x1, S32 y1, S32 x2, S32 y2);
extern void		mmi_fmgr_hide_drv_info_callback(void);
extern void		mmi_fmgr_draw_drv_menuitem_callback(S32 item_idx, BOOL is_hilighted, S32 x1, S32 y1, S32 x2, S32 y2);
extern void		mmi_fmgr_hide_drv_menuitem_callback(void);

extern pBOOL 	mmi_fmgr_get_file_item_callback(S32 item_idx, UI_string_type str_buff, UI_image_type *img_buff_p, U8	str_img_mask);
extern S32		mmi_fmgr_get_file_hint_callback(S32 item_idx, UI_string_type *hint_array);
extern void		mmi_fmgr_draw_file_info_callback(S32 item_idx, S32 x1, S32 y1, S32 x2, S32 y2);
extern void		mmi_fmgr_hide_file_info_callback(void);
extern void		mmi_fmgr_draw_file_menuitem_callback(S32 item_idx, BOOL is_hilighted, S32 x1, S32 y1, S32 x2, S32 y2);
extern void		mmi_fmgr_hide_file_menuitem_callback(void);

extern void		mmi_fmgr_draw_file_matrix_callback(S32 idx, S32 page_start_idx, S32 page_end_idx, BOOL is_hilighted, BOOL is_need_update, BOOL is_new_page, S32 x1, S32 y1, S32 x2, S32 y2);
extern void 	mmi_fmgr_hide_file_matrix_callback(S32 idx);

extern void 	mmi_fmgr_create_thumb_image_cache(S32 image_size);
extern void 	mmi_fmgr_free_thumb_image_cache(void);

/* File Manager caching kernal function [Bear] */
extern void 	mmi_fmgr_kernel_init(S8 *file_path, FMGR_FILTER filter,  U16 *file_count_ptr);
extern void 	mmi_fmgr_kernel_get_file_info(S32 index, FMGR_FILE_INFO_STRUCT *file_info_ptr);
extern void		mmi_fmgr_kernel_get_file_info_ex(S32 index, FMGR_FILE_INFO_STRUCT *file_info_ptr, U8 align_flag);

extern void 	mmi_fmgr_kernel_get_path_name(S8 *full_path_name);
extern void 	mmi_fmgr_kernel_hide_ext_name(S8 *str_buf_ptr, FMGR_FILE_INFO_STRUCT *info_ptr);
extern BOOL		mmi_fmgr_kernel_delete_all(void);
extern void		mmi_fmgr_kernel_load_cache(U16 start_index);
extern U16		mmi_fmgr_kernel_init_continue(PS8 path, UINT *seq, S32 total, PU16 cached_num, U16 start_index, BOOL read_all);
extern U16 		mmi_fmgr_kernel_list (S8 *file_path, U16 *cached_num, U16 start_index, BOOL read_all);


extern void		mmi_fmgr_kernel_set_cur_idx(S32 index);
extern S32		mmi_fmgr_kernel_get_cur_idx(void);
extern U16	 	mmi_fmgr_kernel_get_file_count(void);
extern void		mmi_fmgr_kernel_move_selection_up(void);
extern void		mmi_fmgr_kernel_move_selection_down(void);

/* File Manager Explorer */
extern void		mmi_fmgr_init (void);
extern void		mmi_fmgr_select_path_and_enter (U8 app_id, U8 sel_type, FMGR_FILTER filter, 
									                 		 PS8 start_path, PsExtFuncPtr callback);
extern void		mmi_fmgr_send_copy_req (U8 action, PU8 src_file, PU8 dest_file, PsFuncPtr callback);

extern void		mmi_fmgr_send_delete_req (PU8 path, U32 type, PsFuncPtr callback);

extern pBOOL ebr_get_file_item_callback(S32 item_idx, UI_string_type str_buff, PU8 *img_buff_p, U8 str_img_mask);
extern void fmgr_open_ebook_hdlr(void);


/* internal use functions */

void fmgr_init_context (void);
void fmgr_launch (void);

void highlight_fmgr_drive_list(S32 hiliteID);

void highlight_fmgr_drive_open(void);
void highlight_fmgr_format (void);
void highlight_fmgr_check_drive (void);
void highlight_fmgr_unmount (void);
void highlight_fmgr_create_folder(void);

void highlight_fmgr_app_select(void);
void highlight_fmgr_folder_done(void);

void highlight_fmgr_node(S32 index);
void highlight_fmgr_tree_node(S32 index);

void highlight_fmgr_tree_open(void);
void highlight_fmgr_view_image(void);
void highlight_fmgr_edit_image(void);
void highlight_fmgr_play_audio(void);
void highlight_fmgr_play_video(void);
void highlight_fmgr_execute(void);

void highlight_fmgr_open (void);
void highlight_fmgr_sort(void);
void highlight_fmgr_forward(void);
void highlight_fmgr_detail (void);
void highlight_fmgr_rename(void);
void highlight_fmgr_rename_done(void);
void highlight_fmgr_delete(void);
void highlight_fmgr_delete_all_file(void);
void highlight_fmgr_copy(void);
void highlight_fmgr_tree_copy (void);
void highlight_fmgr_move(void);

void highlight_fmgr_sort_by_name(void);
void highlight_fmgr_sort_by_type(void);
void highlight_fmgr_sort_by_time(void);
void highlight_fmgr_sort_by_size(void);
void highlight_fmgr_sort_none(void);

void highlight_fmgr_to_wallpaper(void);
void highlight_fmgr_to_sub_wallpaper(void);
void highlight_fmgr_to_screensaver(void);
void highlight_fmgr_to_power_on_display(void);
void highlight_fmgr_to_power_off_display(void);
void highlight_fmgr_to_phb(void);
void highlight_fmgr_to_todolist(void);
void highlight_fmgr_to_sms(void);
void highlight_fmgr_to_ems(void);
void highlight_fmgr_to_mms(void);
void highlight_fmgr_to_email(void);
void highlight_fmgr_to_irda(void);
void highlight_fmgr_to_profile(void);
extern void highlight_fmgr_to_bgsound(void);
extern void highlight_fmgr_to_bt(void);

extern void highlight_fmgr_print(void);
extern void entry_fmgr_print_option(void);

extern void highlight_fmgr_print_to_pictbrige(void);
extern void fmgr_print_to_pictbrige(void);
extern void highlight_fmgr_print_to_bluetooth(void);
extern void fmgr_print_to_bluetooth(void);
void highlight_fmgr_input_method (void);

void entry_fmgr_main(void);
void entry_fmgr_explorer(void);
void exit_fmgr_explorer(void);

void entry_fmgr_drive_option(void);
void entry_fmgr_drive_confirm(void);
void entry_fmgr_drive_animation(void);
void exit_fmgr_drive_animation(void);

void entry_fmgr_folder_option(void);
void entry_fmgr_file_option (void);
void exit_fmgr_file_option(void);

void entry_fmgr_tree_option(void);

void entry_fmgr_sort_option(void);
void exit_fmgr_sort_option(void);
void entry_fmgr_forward_option(void);
void exit_fmgr_forward_option(void);

void entry_fmgr_filename_editor (void);
void entry_fmgr_editor_option (void);

void entry_fmgr_delete_confirm (void);

void entry_fmgr_app_drive_option(void);
void entry_fmgr_app_folder_option(void);
void entry_fmgr_app_file_option(void);
void entry_fmgr_app_empty_folder_option(void);

void entry_fmgr_tree_explorer(void);
void exit_fmgr_tree_explorer(void);

void entry_fmgr_detail (void);

void entry_fmgr_copy_animation(void);
void exit_fmgr_copy_animation(void);
void entry_fmgr_delete_animation(void);
void exit_fmgr_delete_animation(void);
U8 fmgr_delete_animation_callback (void *param);

void fmgr_set_curr_path_and_enter (void);
void fmgr_set_curr_tree_and_enter(void);
void fmgr_back_to_parent (void);
void fmgr_back_to_tree_parent (void);

#ifdef __FS_CARD_SUPPORT__
void fmgr_msdc_plug_in_hdlr (void *info);
void fmgr_msdc_plug_out_hdlr (void *info);
#endif

void fmgr_copy_rsp_hdlr(void *info);
void fmgr_delete_rsp_hdlr(void *info);
void fmgr_send_format_req(U8 drive, U32 level, PsFuncPtr callback);
void fmgr_send_check_drive_req(U8 drive, PsFuncPtr callback);
void fmgr_send_unmount_req(U8 index, U32 mode, PsFuncPtr callback);
void fmgr_drive_rsp_hdlr(void *info);

void fmgr_view_image_hdlr (void);
void fmgr_edit_image_hdlr (void);
void fmgr_play_audio_hdlr(void);
void fmgr_play_video_hdlr(void);
void fmgr_execute_hdlr (void);
void fmgr_create_folder_hdlr (void);
void fmgr_rename_hdlr (void);
void fmgr_drive_action_hdlr(void);
void fmgr_copy_hdlr (void);
void fmgr_move_hdlr (void);
void fmgr_aux_copy_hdlr(void);

void fmgr_sort(void);
void fmgr_create_folder(void);
void fmgr_rename(void);
void fmgr_delete (void);
void fmgr_delete_all_file (void);
void fmgr_copy (void);
void fmgr_move (void);
void fmgr_abort_copy_select(void);

void fmgr_return_to_app (void);
void fmgr_forward_to_app(void);
void fmgr_abort_app_select(void);
void fmgr_reset_app_select (void);
BOOL fmgr_exe_app_call_back(void); // used for app that want to exe callback without reset history
void fmgr_add_marker_for_app(U8 app_id);

void fmgr_set_sort_option (U16 sort);
void fmgr_get_sort_option(U16 *sort);

U16 fmgr_get_app_file_option_menu (FMGR_FILE_INFO_STRUCT *file_info);
U16 fmgr_get_file_option_menu (FMGR_FILE_INFO_STRUCT *file_info);
U16 fmgr_get_forward_option_menu (FMGR_FILE_INFO_STRUCT *file_info, BOOL *is_drm);


#ifdef __DRM_SUPPORT__
BOOL fmgr_select_drm_file (void);
BOOL fmgr_populate_drm_info (U32 handle, U8 permission, U8* data);
U8 fmgr_check_drm_rights(FMGR_FILE_INFO_STRUCT *file_info, U8 permission);
U8 fmgr_get_drm_forward_rule(FMGR_FILE_INFO_STRUCT *file_info, BOOL *is_drm);
void entry_fmgr_request_rights_confirm(void);
void entry_fmgr_request_unknown_rights_confirm(void);
void fmgr_send_rights_request (void);
#endif

#ifdef __MMI_IRDA_SUPPORT__
void entry_fmgr_send_irda(void);
void fmgr_forward_to_irda_callback( void *buf_p,  U32 result );
#endif

U16 fmgr_get_app_title_icon_id (void);
pBOOL fmgr_is_msdc_present(void);
pBOOL fmgr_is_parent_present(void);
pBOOL fmgr_is_parent2_present(void);
U8 fmgr_check_file_present(void);
S8 fmgr_get_drive_index(S32 hiliteID);
void fmgr_return_to_editor(void);
void fmgr_adjust_history(S32 scrnId);
void fmgr_remove_all_screen (void);
U8 fmgr_construct_filter_pattern(void);
S32 fmgr_get_parent_highlight_id(void);
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
pBOOL fmgr_is_sec_msdc_present(void);
#endif
/*- zhouqin modify for dual t-flash card 20110503*/


#endif /* _FILE_MGR_H_ */

