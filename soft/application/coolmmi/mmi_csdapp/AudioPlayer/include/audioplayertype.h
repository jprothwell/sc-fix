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
*	AudioPlayerType.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   Audio player type definition
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log$
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/
#include "filemgr.h"
#include "audioplayerdef.h"
#ifndef _AUDIO_PLAYER_TYPE_H
#define _AUDIO_PLAYER_TYPE_H

/*
 * Need to include:
 * (1) gui_data_types.h
 * (3) kal_release.h
 * (4) Conversions.h
 * (5) FrameworkStruct.h
 * (6) GlobalConstants.h
 * (7) FileMgr.h
 * (8) AudioPlayerDef.h
 */
#ifdef __MMI_AUDIO_PLAYER__
typedef UINT32 (*get_total_num)();
typedef BOOL (*get_file_name)(UI_string_type filename, UINT32 index);

#define MAX_ID3_TITLE_LEN 80
#define MAX_ID3_ARTIST_LEN 80
#define MAX_ID3_ALBUM_LEN 80
#define MAX_ID3_GENRE_LEN 30
#define MAX_ID3_YEAR_LEN 5



typedef struct
{
	S32 n_total_items;
	S32 pick_index;
	S32 pick_count;
	U8 bitset[MAX_PLAY_LIST_BYTES];
	
	// settings
	U8 list_auto_gen;
	U8 preferred_list;
	U8 skin;
	U8 repeat_mode; // 0: off, 1: repeat one, 2: repeat all
	U8 random_mode; // 0: off, 1: on
	U8 background_play; // o: off, 1: on
#if defined(__MMI_NOKIA_STYLE_N800__)||defined(CAM_MD_SUPPORT)
	U8 rotated_enable; // o: off, 1: on
#endif	
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
    U8 lyrics_display;  /* o: off, 1: on */
#endif
	// cmd & states
	UI_character_type title[FMGR_MAX_FILE_LEN+1];
	UI_character_type filefullname[FMGR_MAX_PATH_LEN+1];
	U8 state;
	BOOL restorable;
	BOOL need_restore;
	BOOL wait_next;
	BOOL need_replay;
	BOOL in_main_screen;
	BOOL in_list_screen;
	U8 volume;
	
	// buffer for dynamic load
	S32	buff_head;
	S32	n_buff_items;
	//UI_character_type buff_filename[PLAY_LIST_BUFF_SIZE][FMGR_MAX_FILE_LEN+1];
	UI_character_type buff_filename[PLAY_LIST_BUFF_SIZE][FMGR_MAX_PATH_LEN+1];  //by liwh
	
	// duration
	U32 const_seconds;
	U32	play_seconds;
	kal_uint32 play_time;

	// present play list
	U8 present_list;
	BOOL is_subLCD_shown;
} audply_struct;

typedef struct
{
	S16 x, y;
	S16 w, h;
} audply_main_title_struct;

typedef struct
{
	S16 x, y;
	S16 w, h;
} audply_main_time_struct;

typedef struct
{
	S16 x, y;
	S16 w, h;
} audply_main_file_index_struct;

typedef struct
{
	S16 x, y;
	S16 w, h;
	UI_image_type list_icon[NO_OF_LIST];
} audply_main_playlisticon_struct;

typedef struct
{
	S16 x, y;
	S16 w, h;
	S16 ind_left;
	UI_image_type ind_img;
} audply_main_volumebar_struct;

typedef struct
{
    S16 x, y;
    S16 w, h;
    PU8 img;
    PU8 img_fail ;
    INT32 PlayProgress;
}audply_main_progressbar_struct;

typedef struct
{
	S16 x, y;
	S16 w, h;
	UI_image_type up_img, down_img;
	BOOL down;
} audply_main_volumectrl_struct;

#ifdef __MMI_TOUCH_SCREEN__
typedef struct
{
	S16 x, y;
	S16 w, h;
	UI_image_type up_img, down_img;
	BOOL down;
} audply_main_setting_struct;
#endif

typedef struct
{
	S16 x, y;
	S16 w, h;
	UI_image_type on_img, off_img, disabled_img, down_img;
} audply_main_button_struct;

#ifdef __MMI_AUDIO_PLAYER_ANIMATION__
typedef struct
{
	S16 x, y;
	S16 w, h;
	S32 img_handle;
	UI_image_type animation_img;
} audply_main_animation_struct;
#endif

#ifdef __MMI_AUDIO_PLAYER_SKIN_SHOW_REPEAT_SHUFFLE__
typedef struct
{
    S16 x, y;
    S16 w, h;
    PU8 off_up_img, all_up_img, one_up_img;
#ifdef __MMI_TOUCH_SCREEN__      
    PU8 off_down_img, all_down_img, one_down_img;
    BOOL down;
#endif /* __MMI_TOUCH_SCREEN__ */
} audply_main_repeat_struct;

typedef struct
{
    S16 x, y;
    S16 w, h;
    PU8 off_up_img, on_up_img;
#ifdef __MMI_TOUCH_SCREEN__  
    PU8 off_down_img, on_down_img;
    BOOL down;
#endif /*__MMI_TOUCH_SCREEN__*/
} audply_main_shuffle_struct;
#endif

#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
#define FIND_TAG_END                                              \
do{                                                                 \
   if ((cur_char = (U16*)mmi_ucs2chr((S8*)cur_char,AnsiiToUnicode(']'))) == NULL)\
         return TRUE;                                             \
   else                                                           \
      status = TAG_END;                                           \
}while(0)
#define TRIM_LEFT                                                  \
do{                                                                  \
   cur_char = (U16*)mmi_ucs2trim_left((S8*)cur_char,(S8*)L" \t\r\n");\
   if (!cur_char)                                                  \
      return TRUE;                                                 \
}while(0)
typedef struct audply_lyric_line
{
    S32 time;
    S8 *line_lyric1;
    struct audply_lyric_line *next;
} audply_lyric_line_struct;
typedef struct
{
    BOOL is_ready;
    S32 lyric_count;
    UI_character_type lyric_file_path[(FMGR_MAX_PATH_LEN + 5) *ENCODING_LENGTH];
    audply_lyric_line_struct lyric[AUDPLY_LYRIC_MAX_LINE_COUNT];
    audply_lyric_line_struct *first_line;
    audply_lyric_line_struct *cur_line;
    S32 offset_time;
    INT8 lrc_encoding_chset;
} audply_lyric_struct;
typedef struct
{
    S16 start_x;
    S16 start_y;
    S32 width;
    S32 height;
    color_t  lyric_fg_color;
    color_t  lyric_border_color;
    S32  target_layer;
} audply_lyric_printing_struct;
#endif /* __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__ */ 
#endif /* __MMI_AUDIO_PLAYER__ */

#endif /* _AUDIO_PLAYER_TYPE_H */


