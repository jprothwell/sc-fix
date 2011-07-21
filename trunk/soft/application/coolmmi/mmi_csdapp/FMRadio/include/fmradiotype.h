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
*	FMRadioType.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   FM Radio type definition
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log: FMRadioType.h,v $
 * Revision 1.1.1.1  2006/10/24 08:16:41  huangjiwei
 * BASELINE VERSION
 *
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/
#ifndef _FM_RADIO_TYPE_H
#define _FM_RADIO_TYPE_H

#include "mmi_features.h"

#ifdef __MMI_FM_RADIO__
/*
 * Need to include:
 * (1) xxx.h
 * (2) yyy.h
 */

#if defined(__MMI_FM_RADIO_RECORD__)
typedef struct
{
	UI_character_type fullName[MAX_FM_FULL_NAME_LEN];
	U32 duration;
	U32 last_duration_ticks;
	U8 format;
	U8 storage;
	BOOL is_append;
	BOOL user_abort;
} mmi_fmrdo_record_struct;
#endif

typedef struct
{
	UI_character_type name[FM_RADIO_CHANNEL_NUM][21];
	U16 freq[FM_RADIO_CHANNEL_NUM];
} mmi_fmrdo_channel_list_struct;

typedef struct
{
	U16 step_freq;
	U16 frequency;
	U8 selected_button;
	BOOL is_button_down;
	BOOL is_search_on;
	BOOL is_power_on;
	BOOL is_subLCD_shown;
	U8 normal_repeat_step_count;
	BOOL is_long_press_number_key;
	U8 twinkle_frequency_count;
	
	// selected channel index
	S16 channel_index;
	
	// channel list
	mmi_fmrdo_channel_list_struct channel_list;
	
	// settings
	BOOL is_background_play;
	U8 skin_index;
	U8 volume;
	BOOL is_audio_out;
	
	U8 state;
	BOOL is_ready;
	BOOL in_main_screen;
	BOOL is_preempted;

	// record
#if defined(__MMI_FM_RADIO_RECORD__)
	mmi_fmrdo_record_struct rec;
#endif	
} mmi_fmrdo_struct;

#ifdef __MMI_TOUCH_SCREEN__
typedef struct
{
	S16 x;
	S16 y;
} fmrdo_point_struct;

typedef struct
{
	const fmrdo_point_struct* points;
	S16 num;
} fmrdo_polygon_area_struct;
#endif

typedef struct
{
	U8 LSK_text_border_color_r, LSK_text_border_color_g, LSK_text_border_color_b;
	U8 LSK_text_color_r, LSK_text_color_g, LSK_text_color_b;
	U8 RSK_text_border_color_r, RSK_text_border_color_g, RSK_text_border_color_b;
	U8 RSK_text_color_r, RSK_text_color_g, RSK_text_color_b;
	U16 bg_image_id;
	U8 channel_name_text_border_color_r, channel_name_text_border_color_g, channel_name_text_border_color_b;
	U8 channel_name_text_color_r, channel_name_text_color_g, channel_name_text_color_b;
	S16 channel_name_x, channel_name_y;
	S16 channel_name_w, channel_name_h;
	U8 frequency_text_on_color_r, frequency_text_on_color_g, frequency_text_on_color_b;
	U8 frequency_text_off_color_r, frequency_text_off_color_g, frequency_text_off_color_b;
	S16 frequency_x, frequency_y;
	S16 frequency_w, frequency_h;
	U16 volume_image_id;
	U8 volume_bar_type; // 0: horizontal, 1: vertical
	S16 volume_bar_x, volume_bar_y;
	U8 volume_ind_start_gap, volume_ind_end_gap;
	U16 tuner_scale_bg_image_id;
	U16 tuner_scale_ind_image_id;
	U8 tuner_scale_type; // 0: horizontal, 1: vertical
	S16 tuner_scale_x, tuner_scale_y;
	signed char tunner_scale_ind_offset;
	U8 tuner_scale_start_gap, tuner_scale_end_gap;
	U16 step_down_button_up_image_id;
	U16 step_down_button_down_image_id;
	S16 step_down_button_x, step_down_button_y;
	U16 step_up_button_up_image_id;
	U16 step_up_button_down_image_id;
	S16 step_up_button_x, step_up_button_y;
	U16 search_button_on_up_image_id;
	U16 search_button_on_down_image_id;
	U16 search_button_off_up_image_id;
	U16 search_button_off_down_image_id;
	S16 search_button_x, search_button_y;
	U16 power_button_on_up_image_id;
	U16 power_button_on_down_image_id;
	U16 power_button_off_up_image_id;
	U16 power_button_off_down_image_id;
	S16 power_button_x, power_button_y;
	U8 up_key_mapping, down_key_mapping, left_key_mapping, right_key_mapping, ok_key_mapping;

#ifdef __MMI_FM_RADIO_ANIMATION__
	S16 animation_x, animation_y;
	U16 animation_image_id;
#endif	
#ifdef __MMI_TOUCH_SCREEN__
	S16 volume_inc_x, volume_inc_y;
	U16 volume_inc_up_image_id, volume_inc_down_image_id;
	S16 volume_dec_x, volume_dec_y;
	U16 volume_dec_up_image_id, volume_dec_down_image_id;
	S16 setting_x, setting_y;
	U16 setting_up_image_id, setting_down_image_id;
	fmrdo_polygon_area_struct volume_inc_area;
	fmrdo_polygon_area_struct volume_dec_area;
	fmrdo_polygon_area_struct step_down_button_area;
	fmrdo_polygon_area_struct step_up_button_area;
	fmrdo_polygon_area_struct power_button_area;
#if defined(__MMI_FM_RADIO_RECORD__)	
	fmrdo_polygon_area_struct record_button_area;//ADD 080515 for record TouchScreen
#endif	
	fmrdo_polygon_area_struct search_button_area;
	fmrdo_polygon_area_struct setting_area;
	fmrdo_polygon_area_struct frequency_area;
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
	U16 rec_button_image_id;
	U16 rec_pause_button_image_id;	
	S16 rec_button_x, rec_button_y;
	U16 rec_duration_number_id_base; // base is 0, 1 is base plus 1, 2 is base plus 2, ...
	U16 rec_duration_number_col_image_id;
	S16 rec_duration_x,rec_duration_y;
#endif
} mmi_fmrdo_skin_struct;
#endif // __MMI_FM_RADIO__
#endif // _FM_RADIO_TYPE_H


