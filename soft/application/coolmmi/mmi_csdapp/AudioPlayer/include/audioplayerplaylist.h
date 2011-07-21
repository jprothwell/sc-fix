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
*	AudioPlayerPlayList.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   Audio player play list
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log: AudioPlayerPlayList.h,v $
 * Revision 1.1.1.1  2006/04/20 08:07:35  hzk
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/

#ifndef __AUDIO_PLAYER_PLAY_LIST_H__
#define __AUDIO_PLAYER_PLAY_LIST_H__

/*
 * Need to include:
 * (1) mmi_data_types.h
 */

#ifdef __MMI_AUDIO_PLAYER__

BOOL mmi_audply_check_restorable( UI_string_type ext );
BOOL mmi_audply_apply_picked_file( void );
U8 mmi_audply_regen_and_reload_playlist( BOOL need_regen_list, BOOL need_reload_list );
U8 mmi_audply_gen_playlist( UI_string_type folder );
void mmi_audply_split_filename_ext( UI_string_type filename, UI_string_type filename_without_ext, UI_string_type ext );
void mmi_audply_extract_filename( UI_string_type filename, UI_string_type fullfilename );
void mmi_audply_extract_ext( UI_string_type filename, UI_string_type ext );
BOOL mmi_audply_playlist_get_filename( UI_string_type filename, S32 index );
U8 mmi_audply_playlist_get_fullfilename( UI_string_type fullfilename, S32 index );
void mmi_audply_start_playlist_at( S32 index );
S32 mmi_audply_pick_playlist( void );
S32 mmi_audply_pick_playlist_next( void );
S32 mmi_audply_pick_playlist_prev( void );
void mmi_audply_pick_playlist_at( S32 index );
extern UI_string_type mmi_audply_get_present_list_file(void);
void mmi_audply_init_playlist( void );
U8 mmi_audply_playlist_append_one( S8* fullfilename );
BOOL mmi_audply_playlist_remove_one( S32 index );
BOOL mmi_audply_playlist_remove_all( void );
U8 mmi_audply_get_preferred_or_available_list( void );
BOOL mmi_audply_is_dir_ready( UI_string_type path );
BOOL mmi_audply_does_file_exist( UI_string_type path );
BOOL mmi_audply_is_any_file_selected( void );
#endif /* __MMI_AUDIO_PLAYER__ */

#endif // __AUDIO_PLAYER_PLAY_LIST_H__


