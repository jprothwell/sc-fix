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
*	AudioPlayerMainScreen.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   Audio player main screen
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

#ifndef __AUDIO_PLAYER_MAIN_SCREEN_H__
#define __AUDIO_PLAYER_MAIN_SCREEN_H__

/*
 * Need to include:
 * (1) gui_data_types.h
 */
#ifdef __MMI_AUDIO_PLAYER__

extern signed char g_audply_main_selected_button;

void mmi_audply_redraw_subLCD( BOOL is_show );
void mmi_audply_exit_subLCD( void );

void mmi_audply_show_mainscreen( U16 title, U16 left_softkey, U16 right_softkey, S8 skin );

void mmi_audply_exit_mainscreen( void );

void mmi_audply_redraw_main_screen( void );
void mmi_audply_redraw_main_title_background( S32 x1, S32 y1, S32 x2, S32 y2 );
void mmi_audply_redraw_main_title( void );
void mmi_audply_redraw_LSK_down( void );
void mmi_audply_redraw_LSK_up( void );
void mmi_audply_redraw_RSK_down( void );
void mmi_audply_redraw_RSK_up( void );
void mmi_audply_redraw_main_playlisticon( void );
void mmi_audply_redraw_main_time( void );
void mmi_audply_redraw_main_file_index( void );
void mmi_audply_redraw_main_volumebar( void );
void mmi_audply_redraw_main_progressbar(void);
#if defined(__PROJECT_GALLITE_C01__)
void mmi_audply_redraw_main_volumectrl( U8 sel, BOOL down ); // sel = 0 -> dec, sel = 1 -> inc
#endif
#ifdef __MMI_TOUCH_SCREEN__
void mmi_audply_redraw_main_volumectrl( U8 sel, BOOL down ); // sel = 0 -> dec, sel = 1 -> inc
void mmi_audply_redraw_main_setting( BOOL on );
#endif
void mmi_audply_redraw_main_buttons( void );
#endif /* __MMI_AUDIO_PLAYER__ */

#if defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
extern MMI_BOOL mmi_audply_is_lyrics_valid(void);
#endif
#endif // __AUDIO_PLAYER_MAIN_SCREEN_H__

