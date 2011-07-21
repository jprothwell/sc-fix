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
 *   VdoPlyApp.C
 *
 * Project:
 * -------- 
 
 *
 * Description:
 * ------------
 *   Video Player Applcation.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------

 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"
#ifdef __MMI_VIDEO__

#define __NEWSIMULATOR 

#include "stdc.h"
#include "l4dr1.h"
#include "globaldefs.h"

#include "mmi_features_video.h"	/* features */
#include "lcd_sw_rnd.h"
#include "custmenures.h"
#include "custdatares.h"			/* res handle functions */
#include "globalconstants.h"		/* key id define */
#include "fat_fs.h"					/* must after "globalconstants.h" */
#include "globaldefs.h"				/* golbal image and string id. */
#include "gui.h"						/* gui functions */
#include "wgui_categories.h"
#include "historygprot.h"			/* screen history */
#include "unicodexdcl.h"			/* unicode transform functions */
#include "eventsgprot.h"		
#include "filesystemdef.h"			/* file function */
#include "gpioinc.h"					/* led */
#include "settingdefs.h"			/* popup sound */
#include "debuginitdef.h"			/* mmi_assert */	
#include "commonscreens.h"			/* popup */
#include "keybrd.h"					/* clear key function */
#include "sublcdhistorygprot.h"	/* sublcd */
#include "mainmenudef.h"			/* multi-media icon */
#include "frameworkstruct.h"		/* for psextfuncptr */
#include "conversions.h"			/* char set conversion */
#include "phonesetupgprots.h"		/* phnsetgetdefencodingtype() */

#include "nvramtype.h"				/* nvram data type */
#include "nvramprot.h"				/* nvram access fucntions */	
#include "nvramenum.h"				/* nvram id define */

#include "med_api.h"					/* media task */
#include "med_main.h"				/* media task */

#include "lcd_sw_rnd.h"
#include "lcd_if.h"
#include "gdi_include.h"			/* graphic library */


#include "mdi_datatype.h"
#include "mdi_audio.h"				/* audio lib */
#include "mdi_video.h"				/* video lib */
#include "mdi_camera.h"				/* camera lib */
#include "cameraapp.h"				

#include "settingprofile.h"		/* warning tone */
#include "filemanagergprot.h"		/* file path / file error  */
#include "filemanagerdef.h"		/* error string id */
#include "filemgr.h"

#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
	#include "wgui_touch_screen.h"
#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

#include "resource_video_skins.h"		/* ui custom config */
#include "vdorecstrenum.h"
#include "vdorecimgenum.h"
#include "vdoplyimgenum.h"
#include "vdoplystrenum.h"
#include "vdoplyscrenum.h"

#include "vdorecapp.h"
#include "vdorecgprot.h"
#include "vdoplyapp.h"
#include "vdoplygprot.h"

#include "wgui_categories_multimedia.h"
#include "wgui_status_icons.h"			/* status ui */
#include "rtc_sw.h"
#include "kal_release.h"

#ifdef __MMI_TVOUT__
	#include "mdi_tv.h"						/* mdi tvout */
	#include "wallpaperdefs.h"				/* some header for phonsetup.h */
	#include "downloaddefs.h"				/* some header for phonsetup.h */
	#include "phonesetup.h"					/* tvout display style */
#endif /* __MMI_TVOUT__ */
#include "mmi_trace.h"
//#include "coolprofile.h"
#include "phonebooktypes.h"
#include "simdetectiondef.h"
#include "settingsgdcl.h"
#include "audioplayerdef.h"
#include "audioplayerprot.h"
#include "mdi_video.h"
#include "multimemmng.h"

#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_AVRCP_SUPPORT__)
#include "btmmicm.h"
#include "btmmiavrcpgprot.h"
#endif /* __MMI_BT_COOLSAND_SUPPORT__ && __MMI_AVRCP_SUPPORT__ */ 

#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_FTS_SUPPORT__)
#include "btmmicm.h"
#include "btmmiftpgprots.h"
#endif /* __MMI_BT_COOLSAND_SUPPORT__ && __MMI_FTS_SUPPORT__ */ 

#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_OPP_SUPPORT__)
#include "btmmicm.h"
#include "btmmioppgprots.h"
#endif /* __MMI_BT_COOLSAND_SUPPORT__ && __MMI_OPP_SUPPORT__ */ 
#if defined(__MMI_BT_COOLSAND_SUPPORT__)
#include "btmmiscr.h"
#endif
#if defined(__VDOPLY_FEATURE_BT__)
#include "simdetectiongprot.h"
#include "btmmiscrgprots.h"
#include "av_bt.h"
//#include "app_str.h"
#include "audioplayerprot.h"
#endif /* __VDOPLY_FEATURE_BT__ */
/***************************************************************************** 
* Define
*****************************************************************************/
#define VDOPLY_OSD_BUFFER_SIZE 			(((GDI_LCD_WIDTH*GDI_LCD_HEIGHT*GDI_MAINLCD_BIT_PER_PIXEL)>>3)*2) /* double buffer */
#define VDOPLY_OSD_FADE_OUT_TIME			(3*1000)		/* 4 sec */
#define VDOPLY_OSD_FADE_OUT_VALUE		(32)			/* fade 32/256 */
#define VDOPLY_DISPLAY_SET_WIDTH			LCD_HEIGHT
#define VDOPLY_DISPLAY_SET_HEIGHT			LCD_WIDTH


/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef enum
{
	VDOPLY_PLAY_FROM_FILE = 0,
	VDOPLY_PLAY_FROM_ID
} vdoply_play_src_enum;

/***************************************************************************** 
* Global Variable
*****************************************************************************/
extern BOOL		r2lMMIFlag;
extern UINT8 display_flag;
extern U16 gZoomHeight, gZoomWidth;

#ifndef APPMEM_SUPPORT
	extern U32 	camera_osd_layer_buffer[];
#endif /* APPMEM_SUPPORT */

/***************************************************************************** 
* Global Function
*****************************************************************************/
extern kal_uint8	GpioCtrlReq(kal_uint8 action);
extern void 		AudioStopReq(U16 soundId);
extern U16 			GetCurKeypadTone(void);
extern void 		refresh_status_icon_display(void); /* in Wgui_status_icons.c */
#ifdef __MMI_STOP_SIGNAL_REPORT__
extern void NetWorkSignalStopReport(U8 nStart);
#endif
extern void mmi_show_qq_select_confirm(void (*default_entry) (void));

/***************************************************************************** 
* Local Variable
*****************************************************************************/
vdoply_context_struct	g_vdoply_cntx;
BOOL b_video_playing = FALSE;

static U8 is_lsk_pressed_In_Interrupted_state = 0;

/***************************************************************************** 
* Local Function
*****************************************************************************/
#ifdef __MMI_UI_STYLE_4__
	static void mmi_vdoply_redraw_status_bg(void);
#endif /* __MMI_UI_STYLE_4__ */

/* video player screen */
static void mmi_vdoply_blt_screen(void);
static void mmi_vdoply_disply_popup(PU8 str_ptr, U16 img_id, U16 image_on_bottom, U16 tone_duration, U16 tone_id);
static BOOL mmi_vdoply_check_and_display_error_popup(MDI_RESULT result);
static void mmi_vdoply_highlight_play(void);
static void mmi_vdoply_entry_player_screen_internal(void);
static void mmi_vdoply_draw_title(void);
static void mmi_vdoply_draw_panel_state(void);
static void mmi_vdoply_draw_panel_progress(void);
static void mmi_vdoply_draw_panel_timer(void);
static void mmi_vdoply_draw_panel_speed(void);
static void mmi_vdoply_draw_panel_volume(void);
static void mmi_vdoply_draw_style_text(PS8 str, S32 offset_x, S32 offset_y, video_style_text_struct *style_txt);
static void mmi_vdoply_draw_softkey(void);
static void mmi_vdoply_draw_Leftsoftkeypress(void);
static void mmi_vdoply_draw_Rightsoftkeypress(void);
static void mmi_vdoply_draw_panel(void);
static void mmi_vdoply_init_panel(void);

#ifdef __VDOPLY_FEATURE_FULLSCREEN__
	static void mmi_vdoply_draw_fullscr_panel_state(void);
	static void mmi_vdoply_draw_fullscr_panel_progress(void);
	static void mmi_vdoply_draw_fullscr_panel_timer(void);
	static void mmi_vdoply_draw_fullscr_panel_speed(void);
	static void mmi_vdoply_draw_fullscr_panel_volume(void);
	static void mmi_vdoply_draw_fullscr_softkey(void);		
	       void mmi_vdoply_draw_fullscr_osd(void);
	static void mmi_vdoply_init_fullscr_panel(void);	
#if 0
	static void mmi_vdoply_fullscr_reset_osd_fade(BOOL sleep);
#endif
	static void mmi_vdoply_fullscr_osd_fade_cyclic(void);
	static void mmi_vdoply_fullscr_osd_show_up(void);	
	static void mmi_vdoply_display_set_coordinate();
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */


static MDI_RESULT mmi_vdoply_play_video(void);


#ifdef __VDOPLY_FEATURE_SNAPSHOT__
	static void mmi_vdoply_player_snapshot(void);
	static void mmi_vdoply_player_snapshot_key_press(void);
	static void mmi_vdoply_player_snapshot_key_release(void);	
#endif /* __VDOPLY_FEATURE_SNAPSHOT__ */

static void mmi_vdoply_player_timer_cyclic(void);
static void mmi_vdoply_set_softkey(PS8 lsk_str_ptr, PS8 rsk_str_ptr, U16 lsk_img_id, U16 rsk_img_id);
static void mmi_vdoply_lsk_release(void);
static void mmi_vdoply_rsk_release(void);

#ifdef __VDOPLY_FEATURE_VOLUME__
	static void mmi_vdoply_vol_up_key_press(void);
	static void mmi_vdoply_vol_up_key_release(void);
	static void mmi_vdoply_vol_down_key_press(void);
	static void mmi_vdoply_vol_down_key_release(void);	
#endif /* __VDOPLY_FEATURE_VOLUME__ */

#ifdef __VDOPLY_FEATURE_SPEED__
	static void mmi_vdoply_speed_inc_key_press(void);
	static void mmi_vdoply_speed_inc_key_release(void);
	static void mmi_vdoply_speed_dec_key_release(void);	
#endif /* __VDOPLY_FEATURE_SPEED__*/

static void mmi_vdoply_seek_result_hdlr(MDI_RESULT result);
static void mmi_vdoply_seek_key_release(void);
static BOOL mmi_vdoply_seek_foward(void);
#if 0
static void mmi_vdoply_seek_foward_key_press_cyclic(void);
#endif
static void mmi_vdoply_seek_foward_key_press(void);
static void mmi_vdoply_seek_foward_key_release(void);
static BOOL mmi_vdoply_seek_backward(void);
#if 0
static void mmi_vdoply_seek_backward_key_press_cyclic(void);
#endif
static void mmi_vdoply_seek_backward_key_press(void);
static void mmi_vdoply_seek_backward_key_release(void);
static void mmi_vdoply_lsk_press(void);
static void mmi_vdoply_rsk_press(void);

#ifdef __VDOPLY_FEATURE_FULLSCREEN__
	static void mmi_vdoply_toggle_fullscreen_display(void);
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

static void mmi_vdoply_register_key_event(U16 state);
static void mmi_vdoply_player_enter_state(U16 state);
static void mmi_vdoply_exit_player_screen_internal(void);
static S16 	mmi_vdoply_get_speed_factor(void);
#ifdef __MMI_SUBLCD__
	static void mmi_vdoply_draw_sublcd_icon(void);
	static void mmi_vdoply_entry_sublcd_screen(void);
	static void mmi_vdoply_exit_sublcd_screen(void);
#endif /* __MMI_SUBLCD__ */

#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__)
	static U8 mmi_vdoply_clam_close_event_hdlr(void* ptr);
#endif /* __MMI_CLAMSHELL__ && __MMI_SLIDE__ */

void mmi_vodply_player_play_finish_hdlr(MDI_RESULT result);
void mmi_vdoply_player_open_file_done_hdlr(MDI_RESULT result, mdi_video_info_struct *vdo_clip);

/* player list */
static void mmi_vdoply_highlight_app(void);
	 	 void mmi_vdoply_exit_from_fmgr(void* filename, int is_short);
void mmi_vdoply_exit_from_fmgr_emptry_folder(void);

/* re-enter */
static void mmi_vdoply_re_entry_player_screen_from_file(void);
static void mmi_vdoply_re_entry_player_screen_from_id(void);

/* storage */
static void mmi_vdoply_hint_set_storage(U16 index);
static void mmi_vdoply_highlight_set_storage(void);
static void mmi_vdoply_set_storage_value(void);
static void mmi_vdoply_entry_set_storage_screen(void);
/* bluetooth */
#if defined(__MMI_AUDIO_PLAYER__) && defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)    
static void mmi_vdoply_highlight_bluetooth_setting(void);
#endif


#if defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
	static void mmi_vdoply_touch_scr_pen_down_hdlr(mmi_pen_point_struct pos);
	static void mmi_vdoply_touch_scr_pen_up_hdlr(mmi_pen_point_struct pos);
//	static void mmi_vdoply_touch_scr_pen_move_hdlr(mmi_pen_point_struct pos);   //delete by chenqiang 20090715
	static BOOL mmi_vdoply_touch_scr_hit_test(S32 pos_x, S32 pos_y, vdoply_ui_touch_struct *touch_obj);
#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ & __MMI_TOUCH_SCREEN__ */

extern void StopPowerOnOffLED(void);
static void mmi_vdoply_toggle_fullscreen_display_timer(void);  //timer ʱСӦƵ guojian added 20080619

extern BOOL HandleLowBatteryProcess(void);

/*****************************************************************************
*
* VDOPLY Play                                                        
*
*****************************************************************************/

BOOL mmi_vdoply_aud_with_bt(void)
{
   return (g_vdoply_cntx.state == VDOPLY_STATE_BT_CONNECTING )||
   	(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_BT_CONNECTING);
}


#if defined(__VDOPLY_FEATURE_BT__)
/*****************************************************************************
 * FUNCTION
 *  mmi_vdoply_bt_open_callback
 * DESCRIPTION
 *  a callback function invoked by av_bt_open apis
 * PARAMETERS
 *  result
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vdoply_bt_open_callback(S32 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/      
          
   TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_bt_open_callback,result = %d ",result);
	
//    gui_cancel_timer(mmi_vdoply_draw_bt_connecting_anim);
//    mmi_vdoply_clear_bt_connecting_anim();
    
    switch (result)
    {
        case AV_BT_CALLBACK_EVENT_OPEN_OK:
            /* 
             *  NOTICE: even after receive EVENT_OPEN_FAILED or EVENT_OPEN_STOPPED,
             *          bt driver may still send EVENT_OPEN_OK, this is due to driver 
             *          has retry mechanism.
             */
            
            if (g_vdoply_cntx.state == VDOPLY_STATE_BT_CONNECTING)
            {
		  TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_bt_open_callback,state:  VDOPLY_STATE_BT_CONNECTING ");		
                av_bt_open_codec(KAL_TRUE);                
                mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
            }    
            else if (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_BT_CONNECTING)
            {
                TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_bt_open_callback,state:  VDOPLY_STATE_FULLSCR_BT_CONNECTING ");                  
                av_bt_open_codec(KAL_TRUE);                
               mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
			
            }            
            else if (g_vdoply_cntx.state == VDOPLY_STATE_STREAM_BT_CONNECTING)
            {
                TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_bt_open_callback,state:  VDOPLY_STATE_STREAM_BT_CONNECTING ");   
                av_bt_open_codec(KAL_TRUE);                
                mmi_vdoply_player_enter_state(VDOPLY_STATE_STREAM_BUFFERING);
            }
            else if (g_vdoply_cntx.state == VDOPLY_STATE_STREAM_FULLSCR_BT_CONNECTING)
            {
                TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_bt_open_callback,state:  VDOPLY_STATE_STREAM_FULLSCR_BT_CONNECTING ");                 
                av_bt_open_codec(KAL_TRUE);                
                mmi_vdoply_player_enter_state(VDOPLY_STATE_STREAM_FULLSCR_BUFFERING);
            }
            else if (g_vdoply_cntx.state == VDOPLY_STATE_STREAM_BUFFERING ||
                     g_vdoply_cntx.state == VDOPLY_STATE_STREAM_FULLSCR_BUFFERING)
            {
                TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_bt_open_callback,state:  VDOPLY_STATE_STREAM_BUFFERING, VDOPLY_STATE_STREAM_FULLSCR_BUFFERING ");                  
                av_bt_open_codec(KAL_TRUE);
            }
            else if (g_vdoply_cntx.state == VDOPLY_STATE_PLAY ||
                     g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY ||
                     g_vdoply_cntx.state == VDOPLY_STATE_STREAM_PLAY ||
                     g_vdoply_cntx.state == VDOPLY_STATE_STREAM_FULLSCR_PLAY)
            {
                /* 
                 * since it is already play, we may set TRUE, which will directly
                 * change output to BT 
                 */
                TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_bt_open_callback,state:  VDOPLY_STATE_PLAY  VDOPLY_STATE_FULLSCR_PLAY   ");                     
                av_bt_open_codec(KAL_TRUE);
            }            
            else
            {
				/* maybe should do some handling later */
            }
            break;

        case AV_BT_CALLBACK_EVENT_OPEN_FAILED:
        case AV_BT_CALLBACK_EVENT_OPEN_STOPPED:
#ifdef __MMI_BT_AUDIO_VIA_SCO__
        case AV_BT_CALLBACK_EVENT_OPEN_SCO:
#endif
            if (g_vdoply_cntx.state == VDOPLY_STATE_BT_CONNECTING)
            {
                 mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
            }
            else if (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_BT_CONNECTING)
            {
                 mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
            }            
            else if (g_vdoply_cntx.state == VDOPLY_STATE_STREAM_BT_CONNECTING)
            {
                mmi_vdoply_player_enter_state(VDOPLY_STATE_STREAM_BUFFERING);
            }
            else if (g_vdoply_cntx.state == VDOPLY_STATE_STREAM_FULLSCR_BT_CONNECTING)
            {
                mmi_vdoply_player_enter_state(VDOPLY_STATE_STREAM_FULLSCR_BUFFERING);
            }
            else
            {
               TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_bt_open_callback,state:  MMI_TRC_VDOPLY_CALLING_AV_BT_CLOSE_CODEC  "); 
                             
                av_bt_close_codec();
            }
            break;
        case AV_BT_CALLBACK_EVENT_OPEN_CANCELED:
            break;
        case AV_BT_CALLBACK_EVENT_STREAM_ABORT_IND:
        case AV_BT_CALLBACK_EVENT_STREAM_CLOSE_IND:
        case AV_BT_CALLBACK_EVENT_STREAM_SUSPEND_IND:
        case AV_BT_CALLBACK_EVENT_UNEXPECTED_DISCONNECT_IND:

	   TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_bt_open_callback,state:  MMI_TRC_VDOPLY_CALLING_AV_BT_CLOSE_CODEC 2222  "); 		
            av_bt_close_codec();
            if (g_vdoply_cntx.state == VDOPLY_STATE_BT_CONNECTING)
            {
                 /* connect failed - play the video */
                 mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
            }
            else if (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_BT_CONNECTING)
            {
                 /* connect failed - play the video */
                 mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
            }            
            else if (g_vdoply_cntx.state == VDOPLY_STATE_STREAM_BT_CONNECTING)
            {
                 /* connect failed - start buffering */
                mmi_vdoply_player_enter_state(VDOPLY_STATE_STREAM_BUFFERING);
            }
            else if (g_vdoply_cntx.state == VDOPLY_STATE_STREAM_FULLSCR_BT_CONNECTING)
            {
                 /* connect failed - start buffering */
                mmi_vdoply_player_enter_state(VDOPLY_STATE_STREAM_FULLSCR_BUFFERING);
            }
            else 
			{
				/* maybe should do some handling later */
            }         
            break;
        
        case AV_BT_CALLBACK_EVENT_STREAM_START_IND:
        case AV_BT_CALLBACK_EVENT_INQUIRY_ORCONHFP_START_IND:        
        case AV_BT_CALLBACK_EVENT_INQUIRY_ORCONHFP_STOP_IND:        
            /* ignore */
            break;

        default:
            MMI_ASSERT(0);
            break;
    }
}



#endif

/*****************************************************************************
 * FUNCTION
 *  mmi_vdoply_is_output_to_bt
 * DESCRIPTION
 *  a callback function invoked by av_bt_open apis
 * PARAMETERS
 *  result
 * RETURNS
 *  void
 *****************************************************************************/
static MMI_BOOL mmi_vdoply_is_output_to_bt(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/   
 //   MMI_TRACE(MMI_MEDIA_TRC_G2_APP, MMI_TRC_VDOPLY_IS_OUTPUT_TO_BT);      
     TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_is_output_to_bt ");	
   
#if defined(__VDOPLY_FEATURE_BT__)

//    if (mmi_a2dp_is_output_to_bt() &&
    if (mmi_audply_is_output_to_bt() &&		
        (g_vdoply_cntx.video_info.track == MDI_VIDEO_TRACK_AV ||
         g_vdoply_cntx.video_info.track == MDI_VIDEO_TRACK_A_ONLY))
    {
       
	 TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_is_output_to_bt :MMI_TRUE ");		
        return MMI_TRUE;
    }
    else
    {
     
	 TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_is_output_to_bt :MMI_FALSE ");		
        return MMI_FALSE;
    }

#else
    return MMI_FALSE;
#endif 
}



/*****************************************************************************
 * FUNCTION
 *  mmi_vdoply_connect_bt
 * DESCRIPTION
 *  start play video
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_A2DP_SUPPORT__
 extern mmi_bt_dev_struct *mmi_audply_get_bt_headset(void);
#endif
static void mmi_vdoply_connect_bt(void)
{
#if defined(__VDOPLY_FEATURE_BT__)        
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL is_stereo;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
  
     TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_connect_bt   g_vdoply_cntx.state =%d  ",g_vdoply_cntx.state);	
    if (g_vdoply_cntx.state == VDOPLY_STATE_BT_CONNECTING || 
        g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_BT_CONNECTING || 
        g_vdoply_cntx.state == VDOPLY_STATE_STREAM_BT_CONNECTING ||        
        g_vdoply_cntx.state == VDOPLY_STATE_STREAM_FULLSCR_BT_CONNECTING)
    {
        if (mmi_vdoply_is_output_to_bt())
        {
            /* if connecting is not start in 600ms, we will show bt connecting anim */
 //           gui_start_timer(600, mmi_vdoply_draw_bt_connecting_anim);
            
        #ifdef __MMI_VIDEO_STREAM__
            if (g_vdoply_cntx.state == VDOPLY_STATE_STREAM_BT_CONNECTING ||
                g_vdoply_cntx.state == VDOPLY_STATE_STREAM_FULLSCR_BT_CONNECTING)
            {
                /* get audio channel and sampling rate when first time buffering */
                if (g_vdoply_cntx.video_info.aud_channel_no == 0 &&
                    g_vdoply_cntx.video_info.aud_sample_rate == 0)
                {
                    mdi_video_stream_get_audio_info(&(g_vdoply_cntx.video_info.aud_channel_no),
                                                    &(g_vdoply_cntx.video_info.aud_sample_rate));
                }
            }
        #endif /* __MMI_VIDEO_STREAM__ */
        
            if (g_vdoply_cntx.video_info.aud_channel_no == 2)
            {
                is_stereo = MMI_TRUE;
            }
            else
            {
                is_stereo = MMI_FALSE;
            }

     //       MMI_TRACE(MMI_MEDIA_TRC_G2_APP, MMI_TRC_VDOPLY_CALLING_AV_BT_OPEN_EX, __LINE__);  
           TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_connect_bt  CALLING_AV_BT_OPEN_EX ");	
            av_bt_open_ex(
                mmi_audply_get_bt_headset(),
                g_vdoply_cntx.video_info.aud_sample_rate,
                (kal_bool)is_stereo,
                mmi_vdoply_bt_open_callback,
                TRUE);   

            return;
        }
        else
        {
            /* shall not enter here */
            MMI_ASSERT(0);
        }
    }
    else
    {
        MMI_ASSERT(0);
    }
#else
    MMI_ASSERT(0);
#endif /* __VDOPLY_FEATURE_BT__ */


}


/*****************************************************************************
 * FUNCTION
 *  mmi_vdoply_disconnect_bt
 * DESCRIPTION
 *  start play video
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vdoply_disconnect_bt(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
      
        TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_disconnect_bt ");	
    
#if defined(__VDOPLY_FEATURE_BT__)    
//    gui_cancel_timer(mmi_vdoply_draw_bt_connecting_anim);
//    mmi_vdoply_clear_bt_connecting_anim();

    if (mmi_vdoply_is_output_to_bt())
    {
    	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_disconnect_bt  CALLING_AV_BT_CLOSE");		
        av_bt_close(FALSE);
    }

    /* TODO: stop anim */
#endif /* __VDOPLY_FEATURE_BT__ */
 
}




/*****************************************************************************
 * FUNCTION
 *  mmi_vdoply_bt_close_codec
 * DESCRIPTION
 *  start play video
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_vdoply_bt_close_codec(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
      
     TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_bt_close_codec ");	
#if defined(__VDOPLY_FEATURE_BT__)    
    if (mmi_vdoply_is_output_to_bt())
    {
  	 TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_bt_close_codec CALLING_AV_BT_CLOSE_CODEC ");	
        av_bt_close_codec();
    }
#endif /* __VDOPLY_FEATURE_BT__ */
 
}

/*****************************************************************************
* FUNCTION
*	mmi_vdoply_is_in_app
* DESCRIPTION
*  is in video player app or not
* PARAMETERS
*  void
* RETURNS
*  BOOL
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
BOOL mmi_vdoply_is_in_app(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdoply_cntx.state != VDOPLY_STATE_EXIT)
		return TRUE;
	else
		return FALSE;
}

BOOL mmi_vdoply_is_in_playing()
{
	return ( g_vdoply_cntx.state == VDOPLY_STATE_PLAY ||g_vdoply_cntx.state == VDOPLY_STATE_PLAY_SEEKING||g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY||g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY_SEEKING);
}

/*****************************************************************************
* FUNCTION
*	mmi_vdoply_blt_part
* DESCRIPTION
*  blt part of the screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/

static void mmi_vdoply_blt_part(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/


#ifdef NEVER
	#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		gdi_layer_blt_previous(0, 0, UI_device_height-1, UI_device_width-1);
	#else
		if((g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_IDLE) || 
			(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PAUSE) || 
			(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY) || 
			(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_SNAPSHOT)||
			(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_SEEKING)||
			(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY_SEEKING))
		{
			gdi_layer_blt_previous(0, 0, UI_device_height-1, UI_device_width-1);
		}
		else
		{	
			gdi_layer_blt_previous(0, g_vdoply_osd_cntx.play_wnd.rect.offset_y+g_vdoply_osd_cntx.play_wnd.rect.height, UI_device_width-1, UI_device_height-1);	
		}
	#endif 
#endif /* NEVER */
#ifdef __MMI_MAINLCD_220X176__
			gdi_layer_merge_for_video(g_vdoply_osd_cntx.play_wnd.rect.offset_x+g_vdoply_osd_cntx.play_wnd.rect.width, 0, UI_device_width-1, g_vdoply_osd_cntx.play_wnd.rect.offset_y+g_vdoply_osd_cntx.play_wnd.rect.height);	
#endif
			gdi_layer_merge_for_video(0, g_vdoply_osd_cntx.play_wnd.rect.offset_y+g_vdoply_osd_cntx.play_wnd.rect.height, UI_device_width-1, UI_device_height-1);	
			mdi_video_display_interface();

}





/*****************************************************************************
* FUNCTION
*	mmi_vdoply_blt_screen
* DESCRIPTION
*  blt full screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_blt_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	TBM_ENTRY(0x2A65);
#ifdef NEVER
	#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		gdi_layer_blt_previous(0, 0, UI_device_height-1, UI_device_width-1);
	#else
		if((g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_IDLE) || 
			(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PAUSE) || 
			(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY) || 
			(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_SNAPSHOT)||
			(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_SEEKING)||
			(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY_SEEKING))
		{
//			gdi_layer_blt_previous(0, 0, UI_device_height-1, UI_device_width-1);
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
		}
		else
		{
			gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

		}
	#endif 
#endif /* NEVER */
		gdi_layer_merge_for_video(0, 0, UI_device_width-1, UI_device_height-1);
		mdi_video_display_interface();

	TBM_EXIT(0x2A65);
}


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_redraw_status_bg
* DESCRIPTION
*  redraw status backgroud
* PARAMETERS
*  void
* RETURNS
*  BOOL
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __MMI_UI_STYLE_4__
static void mmi_vdoply_redraw_status_bg(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 x1, y1, x2, y2;
   	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	get_status_icon_bar_clip(0, &x1, &y1, &x2, &y2);

	gdi_layer_push_clip();
	gdi_layer_set_clip(x1, y1, x2, y2);
	gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);

	gdi_layer_pop_clip();
}
#endif /* __MMI_UI_STYLE_4__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_disply_popup
* DESCRIPTION
*  display popup. will roate if is horizontal screen 
* PARAMETERS
*  str_ptr				IN		string pointer
*	img_id				IN		image id
*	image_on_bottom	IN		is image on bottom
*	tone_duration		IN		tone duration
*	tone_id				IN 	tine id
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_disply_popup(PU8 str_ptr,
												U16 img_id, 
												U16 image_on_bottom, 
												U16 tone_duration, 
												U16 tone_id)
{
	#if defined(__VDOPLY_FEATURE_HORIZONTAL_VIEW__)
		DisplayPopupRotated((PU8)str_ptr, (U16)img_id, (U8)image_on_bottom, (U32)tone_duration, (U8)tone_id, MMI_FRM_SCREEN_ROTATE_270);
	#else
		DisplayPopup((PU8)str_ptr, (U16)img_id, (U8)image_on_bottom, (U32)tone_duration, (U8)tone_id);
	#endif /* __VDOPLY_FEATURE_HORIZONTAL_VIEW__ */
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_check_and_display_error_popup
* DESCRIPTION
*  display error popup.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static BOOL mmi_vdoply_check_and_display_error_popup(MDI_RESULT result)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	TRACE_EF(g_sw_MED_Vid,"Func: %s result=%d", __FUNCTION__,result);
	if( result < 0)
	{
		switch(result)
		{
		case MDI_RES_VDOPLY_ERR_SNAPSHOT_DISK_FULL:
			mmi_vdoply_disply_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			break;

		case MDI_RES_VDOPLY_ERR_SNAPSHOT_ROOT_DIR_FULL:	
			mmi_vdoply_disply_popup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			break;

		case MDI_RES_VDOPLY_ERR_SNAPSHOT_WRITE_PROTECTION:
			mmi_vdoply_disply_popup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_WRITE_PROTECTION), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
			break;

		case MDI_RES_VDOPLY_ERR_SNAPSHOT_NO_DISK:
		case MDI_RES_VDOPLY_ERR_SNAPSHOT_FAILED:
			mmi_vdoply_disply_popup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_SNAPSHOT_FAILED), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
			break;
			
		/* followings are serious error, has to exit app */
		case MDI_RES_VDOPLY_ERR_OPEN_FILE_FAILED:
		case MDI_RES_VDOPLY_ERR_PLAY_FAILED:
		case MDI_RES_VDOPLY_ERR_SEEK_FAILED:
			mmi_vdoply_disply_popup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_INVALID_VIDEO_FILE), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			DeleteNHistory(1);
			break;

		default:	
			MMI_ASSERT(0); /* use assert here, make sure all error case is handled properly */
		}
	
		return TRUE;
	}
	else 
		return FALSE;
}




/*****************************************************************************
* FUNCTION
*	mmi_vdoply_highlight_play
* DESCRIPTION
*  highlight handler
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_highlight_play(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_vdoply_entry_player_screen, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
}




/*****************************************************************************
* FUNCTION
*	mmi_vdoply_entry_player_screen
* DESCRIPTION
*  enter player's screen from filemanger, will use file manager's selected file
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdoply_entry_player_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	FMGR_FILE_INFO_STRUCT	file_info;
	S32 							index;
	S8								buffer[FMGR_PATH_BUFFER_SIZE];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
	/*----------------------------------------------------------------*/
	
	/* get file name from file manager's structure  */
	index	= mmi_fmgr_kernel_get_cur_idx();
	
	mmi_fmgr_kernel_get_file_info(index, &file_info);
	mmi_fmgr_kernel_get_path_name(buffer);

	g_vdoply_cntx.play_source = VDOPLY_PLAY_FROM_FILE;

	pfnUnicodeStrcat(buffer, file_info.file_name);

	mmi_vdoply_entry_player_screen_from_file(buffer, file_info.is_short);
	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_entry_player_screen_from_file
* DESCRIPTION
*  enter player's screen and play from file
* PARAMETERS
*  filename		IN			filename with full path
*	is_short		IN			short name, need to covert from BIG5 to Unicode before display
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdoply_entry_player_screen_from_file(PS8 filename, BOOL is_short)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	

   	TBM_ENTRY(0x2A6D);
	EntryNewScreen(SCR_ID_VDOPLY_PLAYER, 
						mmi_vdoply_exit_player_screen_internal, 
						mmi_vdoply_re_entry_player_screen_from_file, 
						NULL);
    
#ifdef __MMI_STOP_SIGNAL_REPORT__
	NetWorkSignalStopReport(0); //Stop the signal report for "No more Envolope" issue
#endif
   /* allcoate buffer for filename */
	g_vdoply_cntx.filename = coolsand_UI_malloc(FMGR_PATH_BUFFER_SIZE);
	
	g_vdoply_cntx.play_source = VDOPLY_PLAY_FROM_FILE;

	g_vdoply_cntx.is_short_filename = is_short;

	pfnUnicodeStrcpy(g_vdoply_cntx.filename, filename);
	
	/* store path to NVRAM, use for recover filename when back from interrupt */
	mmi_fmgr_util_store_filepath(g_vdoply_cntx.filename, FMGR_PATH_ID_VIDEO);
	
	gZoomHeight = 0;
	gZoomWidth = 0;

	mmi_vdoply_entry_player_screen_internal();
	TBM_EXIT(0x2A6D);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_re_entry_player_screen_from_file
* DESCRIPTION
*  re-entry player screen for playing file
* PARAMETERS
*  filename		IN			filename with full path
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_re_entry_player_screen_from_file(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   	TBM_ENTRY(0x2A6C);
	EntryNewScreen(SCR_ID_VDOPLY_PLAYER, 
						mmi_vdoply_exit_player_screen_internal, 
						mmi_vdoply_re_entry_player_screen_from_file, 
						NULL);

   /* allcoate buffer for filename */
	MMI_ASSERT(g_vdoply_cntx.filename == NULL);

#ifdef __MMI_STOP_SIGNAL_REPORT__
	NetWorkSignalStopReport(0); //Stop the signal report for "No more Envolope" issue
#endif

	g_vdoply_cntx.filename = coolsand_UI_malloc(FMGR_PATH_BUFFER_SIZE);

	/* get path to NVRAM, use for recover filename when back from interrupt */
	mmi_fmgr_util_load_filepath(g_vdoply_cntx.filename, FMGR_PATH_ID_VIDEO);
	
	g_vdoply_cntx.play_source = VDOPLY_PLAY_FROM_FILE;

	#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;
	#endif

	mmi_vdoply_entry_player_screen_internal();
	TBM_EXIT(0x2A6C);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_entry_player_screen_from_id
* DESCRIPTION
*  enter player's screen and play from ID 
* PARAMETERS
*  vidoe_id				IN		video clip resource id
*	video_name_id		IN		video's name string id
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdoply_entry_player_screen_from_id(U16 video_id, U16 video_name_id)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	EntryNewScreen(SCR_ID_VDOPLY_PLAYER, 
						mmi_vdoply_exit_player_screen_internal, 
						mmi_vdoply_re_entry_player_screen_from_id, 
						NULL);
   
	g_vdoply_cntx.play_source = VDOPLY_PLAY_FROM_ID;

	g_vdoply_cntx.video_id 			= video_id;
	g_vdoply_cntx.video_name_id	= video_name_id;
	
	if(video_name_id != 0)
	{
		g_vdoply_cntx.filename 				= GetString(g_vdoply_cntx.video_name_id);
		g_vdoply_cntx.is_short_filename 	= FALSE;
	}
	else
	{
		g_vdoply_cntx.filename 				= NULL;
		g_vdoply_cntx.is_short_filename 	= FALSE;		
	}	

	#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;
	#endif

	mmi_vdoply_entry_player_screen_internal();
}


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_re_entry_player_screen_from_id
* DESCRIPTION
*  re-enter player's screen and play from ID 
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_re_entry_player_screen_from_id(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	EntryNewScreen(SCR_ID_VDOPLY_PLAYER, 
						mmi_vdoply_exit_player_screen_internal, 
						mmi_vdoply_re_entry_player_screen_from_id, 
						NULL);

	g_vdoply_cntx.play_source 	= VDOPLY_PLAY_FROM_ID;
	

	if(g_vdoply_cntx.video_id != 0)
	{
		g_vdoply_cntx.filename 				= GetString(g_vdoply_cntx.video_name_id);
		g_vdoply_cntx.is_short_filename 	= FALSE;
	}
	else
	{
		g_vdoply_cntx.filename 				= NULL;
		g_vdoply_cntx.is_short_filename 	= FALSE;
	}	

   mmi_vdoply_entry_player_screen_internal();
}


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_entry_player_screen_internal
* DESCRIPTION
*  internal play screen function, allows to play from id and filename 
* PARAMETERS
*  void 
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_entry_player_screen_internal(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16	error;
	U8		*guiBuffer;
	U8		*inputBuffer;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	TBM_ENTRY(0x2A61);
	/* set TV out mode */
	/* if is full screen multimedia mode, change to tvout mode2 */
	#ifdef __MMI_TVOUT__
		if(mdi_tvout_is_enable())
		{
			if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
			{
				/* if is fullscreen, we shall set to owner to video here, to avoid GDI's update */
				mdi_tvout_set_owner(MDI_TV_OWNER_VIDEO);		
				
				/* start mode 2 - full screen preview update */
				mdi_tvout_set_mode(	MDI_TV_MODE_VIDEO_PLAYBACK,
											0,						/* layer_width */
											0);					/* layer_height */
				
			}
		}
	#endif /* __MMI_TVOUT__ */


	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

	g_vdoply_cntx.play_wnd_layer_handle = GDI_LAYER_EMPTY_HANDLE;

#ifdef __MMI_CAMERA__

	/* load camera setting for snap shot directory */
	mmi_camera_load_setting();
#endif	
	/* entry full screen app */
	//#ifndef __MMI_UI_STYLE_4__
		/* other style no status */
		entry_full_screen();
	//#endif /* __MMI_UI_STYLE_4__ */	
	

	/* entry sublcd */
	#ifdef __MMI_SUBLCD__
		if(!g_vdoply_cntx.is_sub_display)
		{
			ForceSubLCDScreen(mmi_vdoply_entry_sublcd_screen);
		}
	#endif 

	#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__) && defined(MMI_ON_HARDWARE_P)
		/* register event hdlr */
		SetInterruptEventHandler(mmi_vdoply_clam_close_event_hdlr, NULL, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
	#endif /* __MMI_CLAMSHELL__ && __MMI_SLIDE__ && MMI_ON_HARDWARE_P */




	guiBuffer	= GetCurrGuiBuffer(SCR_ID_VDOPLY_PLAYER);
	inputBuffer	= GetCurrInputBuffer(SCR_ID_VDOPLY_PLAYER);	

	if(guiBuffer == NULL)
	{
		
		g_vdoply_cntx.cur_status.play_time 				= 0;	
		g_vdoply_cntx.cur_status.start_play_time		= 0;
		g_vdoply_cntx.cur_status.speed					= VDOPLY_DEFAULT_SPEED;				
		
		ReadValue((U8)NVRAM_VDOPLY_VOLUMN, (void*)&g_vdoply_cntx.cur_status.volume, DS_BYTE, &error);

		if(g_vdoply_cntx.cur_status.volume == 0xff)
			g_vdoply_cntx.cur_status.volume = LEVEL5;	
	}
	

	g_vdoply_cntx.is_seeking					= FALSE;
	g_vdoply_cntx.is_seek_bwd_key_pressed 	= FALSE;
	g_vdoply_cntx.is_seek_fwd_key_pressed 	= FALSE;
	g_vdoply_cntx.is_lsk_pressed 				= FALSE;	
	g_vdoply_cntx.is_rsk_pressed 				= FALSE;	
	g_vdoply_cntx.is_speed_inc_key_pressed = FALSE;	
	g_vdoply_cntx.is_speed_dec_key_pressed	= FALSE;	
	g_vdoply_cntx.is_vol_inc_key_pressed	= FALSE;	
	g_vdoply_cntx.is_vol_dec_key_pressed	= FALSE;	

	g_vdoply_cntx.fullscr_osd_opacity		= 255;
	
	/****************************************************************************
	* Init proper state for video applcation 	                                                                   
	*****************************************************************************/	
	/* force all playing keypad tone off */
	AudioStopReq(GetCurKeypadTone());
	
  	/* disable key pad tone */
  	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
#if  defined(__MMI_AUDIO_PLAYER__) && defined(__PROJECT_GALLITE_C01__)
	mmi_audply_stop_playing();
#endif // __MMI_AUDIO_PLAYER__

	/* suspend background play */
	mdi_audio_suspend_background_play();

	/* disalbe align timer and enter game */
	UI_disable_alignment_timers();

	/* stop LCD patten */
	StopLEDPatternBackGround();

	/* stop MMI sleep */
	TurnOnBacklight(0);

	/* init aud volume */		
	mdi_audio_set_volume(MDI_VOLUME_VIDEO, g_vdoply_cntx.cur_status.volume);	

	/* use medium font */
	coolsand_UI_set_font(&MMI_medium_font);

	#if defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
		mmi_wgui_register_pen_down_handler(mmi_vdoply_touch_scr_pen_down_hdlr);
		mmi_wgui_register_pen_up_handler(mmi_vdoply_touch_scr_pen_up_hdlr);
//		mmi_wgui_register_pen_move_handler(mmi_vdoply_touch_scr_pen_move_hdlr);  //delete by chenqiang 20090715
	#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ & __MMI_TOUCH_SCREEN__*/


	/* get normal screen layer rotate angle */
	#ifdef __VDOPLY_FEATURE_HORIZONTAL_VIEW__
		mmi_frm_set_self_rotation_flag(MMI_FRM_SCREEN_ROTATE_270);
	
		g_vdoply_cntx.normal_style_rotate 	 	= GDI_LAYER_ROTATE_270;
		g_vdoply_cntx.normal_style_lcd_width 	= UI_device_height;
		g_vdoply_cntx.normal_style_lcd_height 	= UI_device_width;		
		
		/* rotate base layer */
		gdi_layer_set_rotate((U8)g_vdoply_cntx.normal_style_rotate);
		gdi_layer_resize(g_vdoply_cntx.normal_style_lcd_width, g_vdoply_cntx.normal_style_lcd_height);		
	
	#else
		g_vdoply_cntx.normal_style_rotate 		= GDI_LAYER_ROTATE_0;
		g_vdoply_cntx.normal_style_lcd_width 	= UI_device_width;
		g_vdoply_cntx.normal_style_lcd_height 	= UI_device_height;			
	#endif /* __VDOPLY_FEATURE_HORIZONTAL_VIEW__ */

	

	/****************************************************************************
	* Init double osd buffer for full screen display                                                              
	*****************************************************************************/
	#if 0//def __VDOPLY_FEATURE_FULLSCREEN__
			/* create full screen osd layer */	
		#ifdef APPMEM_SUPPORT
			/* with ASM */
			g_vdoply_cntx.osd_layer_buf_ptr = AllocMediaMemory(VDOPLY_OSD_BUFFER_SIZE); 
		#else
			/* no ASM */
			g_vdoply_cntx.osd_layer_buf_ptr = (PU8)camera_osd_layer_buffer;
		#endif /* APPMEM_SUPPORT */

		gdi_layer_create_double_using_outside_memory(0,
																	0,
																	UI_device_height,		/* rotate - swap width with height*/
																	UI_device_width,		/* rotate - swap width with height*/
																	&g_vdoply_cntx.osd_layer_handle,
																	(PU8)g_vdoply_cntx.osd_layer_buf_ptr, 
																	(S32)(VDOPLY_OSD_BUFFER_SIZE));

		gdi_layer_push_and_set_active(g_vdoply_cntx.osd_layer_handle);
		gdi_layer_set_rotate(GDI_LAYER_ROTATE_270);
				
		gdi_layer_set_opacity(TRUE, (U8)g_vdoply_cntx.fullscr_osd_opacity);
		gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);	
		gdi_layer_pop_and_restore_active();	
		
	#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
	

	
	/****************************************************************************
	* draw skin                                                                 
	*****************************************************************************/	

	/* init panel */
	mmi_vdoply_init_panel();

	#ifdef __VDOPLY_FEATURE_FULLSCREEN__
		mmi_vdoply_init_fullscr_panel();
	#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
	
	gdi_layer_lock_frame_buffer();
	
	gdi_layer_reset_clip();				/* in this app. use full screen clip */
	coolsand_UI_reset_text_clip();		/* in this app. use full screen clip */
	if(guiBuffer == NULL) 
	{
		g_vdoply_cntx.cur_status.start_play_time	= 0;
		g_vdoply_cntx.cur_status.play_time 			= 0;	
		g_vdoply_cntx.cur_status.play_ratio 		= 0;	
	}
	
	
	if((gZoomHeight == 0) &&  (gZoomWidth == 0)) //not full screen
	{
	gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);

	#ifdef __MMI_UI_STYLE_4__
		/* restore hide status function */
		register_hide_status_icon_bar(0, mmi_vdoply_redraw_status_bg);	
		refresh_status_icon_display();	
	#endif /* __MMI_UI_STYLE_4__ */

	/* draw title bar */
	mmi_vdoply_draw_title();

	/* draw softkey */
	mmi_vdoply_set_softkey(NULL, (PS8)GetString(STR_GLOBAL_BACK), 0, IMG_ID_VDOPLY_RSK_BACK);
	mmi_vdoply_draw_softkey();
	mmi_vdoply_draw_panel();
		}
	gdi_layer_unlock_frame_buffer();

//	mmi_vdoply_blt_screen();

	/* check if it is first entry */
	if( (guiBuffer == NULL) || (g_vdoply_cntx.cur_status.play_time == 0))
	{

		/* (1) First time enter */
		/* (2) Back from interrupt, but was interrupt at idle state */
		mmi_vdoply_player_enter_state(VDOPLY_STATE_OPENING);
	}
	else
		mmi_vdoply_player_enter_state(VDOPLY_STATE_INTERRUPTED_OPENING);

	TBM_EXIT(0x2A61);
}


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_play_video
* DESCRIPTION
*  start play video
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static MDI_RESULT mmi_vdoply_play_video(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

   MDI_RESULT ret;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   	TBM_ENTRY(0x2A6B);
	if(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY)
	{
		TBM_EXIT(0x2A6B);
		/* start play video - fullscreen */
		ret = mdi_video_ply_play(	g_vdoply_cntx.base_layer_handle,							
											GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1,	/* blt layer */	
											GDI_LAYER_ENABLE_LAYER_0,									/* play_layer */
											1,																	/* repeat */
											TRUE,																/* visual update */
											MDI_DEVICE_SPEAKER2,											/* audio path */
											VDOPLY_PLAYBACK_ROTATE_270,								/* rotate */
											mmi_vdoply_get_speed_factor(),							/* speed factor */
											mmi_vodply_player_play_finish_hdlr);					/* play layer */
			
	}
	else if(g_vdoply_cntx.state == VDOPLY_STATE_PLAY)
	{
		#ifdef __VDOPLY_FEATURE_HORIZONTAL_VIEW__

			TBM_EXIT(0x2A6B);
			ret = mdi_video_ply_play(	g_vdoply_cntx.play_wnd_layer_handle,	
		 										GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1,		/* blt layer */
												GDI_LAYER_ENABLE_LAYER_1,										/* play_layer_flag */
												1,																		/* repeat */
												TRUE,																	/* visual update */
												MDI_DEVICE_SPEAKER2,
												VDOPLY_PLAYBACK_ROTATE_270,									/* rotate */
												mmi_vdoply_get_speed_factor(),								/* speed factor */
												mmi_vodply_player_play_finish_hdlr);						/* play layer */	
		
		#else

			TBM_EXIT(0x2A6B);
			ret = mdi_video_ply_play(	g_vdoply_cntx.play_wnd_layer_handle,	
		 										GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1,		/* blt layer */
												GDI_LAYER_ENABLE_LAYER_1,										/* play_layer_flag */
												1,																		/* repeat */
												TRUE,																	/* visual update */
												MDI_DEVICE_SPEAKER2,
												VDOPLY_PLAYBACK_ROTATE_0,										/* rotate */
												mmi_vdoply_get_speed_factor(),								/* speed factor */
												mmi_vodply_player_play_finish_hdlr);						/* play layer */	
		
		#endif /* __VDOPLY_FEATURE_HORIZONTAL_VIEW__ */
	}
	else
	{
		MMI_ASSERT(0);
		TBM_EXIT(0x2A6B);
		return -1; /* error */
	}

      if (ret != MDI_RES_VDOPLY_SUCCEED)
      	{
      	
        mmi_vdoply_bt_close_codec();

	}

      return  ret;

	TBM_EXIT(0x2A6B);
   
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_title
* DESCRIPTION
*  draw title
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#if defined(__PROJECT_GALLITE_C01__) 
scrolling_text VP_scrolling_text;
BOOL	vp_title_scrolling = FALSE;


void VP_text_stop_scrolling(void)
{
	gui_scrolling_text_stop(&VP_scrolling_text);
	vp_title_scrolling = FALSE;
}

void VP_scrolling_text_timer_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_handle_scrolling_text(&VP_scrolling_text);
}  

void VP_draw_scrolling_text_background(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	if(vp_title_scrolling)
	{
	    gui_push_clip();
	    gui_set_clip_preset(x1, y1, x2, y2);
	    //gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);
	    gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_TRANSPARENT );
	    gui_pop_clip();
	}
	else
	{
		gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);
	}
}   

S8	 buf_filename[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];
#endif
static void mmi_vdoply_draw_title(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 str_len;
	S32 index;
	S8	 buf_filename[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];
	S8	 buf_filename_dest[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];
	S32 str_width;
	S32 str_height;
	S32 offset_x;
	S32 offset_y;
	S32 dot_width;
	S32 dot_height;
	S32 str_total_width;
	S32 char_width;
	S32 char_height;

	UI_character_type *char_ptr; 
	video_rect_struct *bbox_rect_p;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* display filename */
	
	if( (g_vdoply_osd_cntx.title.is_draw_filename) || (g_vdoply_osd_cntx.title.is_draw_app_name))
	{
		if(g_vdoply_osd_cntx.title.is_draw_filename)
		{
			str_len = pfnUnicodeStrlen(g_vdoply_cntx.filename);

			/* find file name - by finding "\\"*/
			for(index = str_len*2-2; index >= 2; index -= 2)
			{
				if( pfnUnicodeStrncmp((PS8)&g_vdoply_cntx.filename[index], (PS8)L"/", 1) == 0)
				{
					index+=2;
					break;
				}
			}

			pfnUnicodeStrncpy((PS8)buf_filename, (PS8)(&(g_vdoply_cntx.filename[index])),FMGR_MAX_FILE_LEN);
			mmi_fmgr_hide_ext_name(buf_filename);
//			mmi_fmgr_extract_file_name(buf_filename);


		}
		else  /* g_vdoply_osd_cntx.title.is_draw_app_name */
		{
			pfnUnicodeStrncpy((PS8)buf_filename, (PS8)GetString(STR_ID_VDOPLY_APP_NAME),FMGR_MAX_FILE_LEN);
		}

		
		coolsand_UI_measure_string((UI_string_type)buf_filename, &str_width, &str_height);
		bbox_rect_p = (video_rect_struct *)&g_vdoply_osd_cntx.title.rect;

		/* if string too long, change to xxxxxx.. */
#if defined(__PROJECT_GALLITE_C01__) 
		if(str_width > bbox_rect_p->width)
		{
			str_len = pfnUnicodeStrlen(buf_filename);
			coolsand_UI_measure_string((UI_string_type)L"..", &dot_width, &dot_height);

			str_total_width = dot_width;
			char_ptr = (UI_character_type*)buf_filename;
			
			for( index = 0 ; index < str_len; index++ )
			{
				coolsand_UI_measure_character(char_ptr[index], &char_width, &char_height);
					
				if( (str_total_width+char_width) > bbox_rect_p->width)
				{	
					pfnUnicodeStrcpy(&buf_filename[(index-1)*ENCODING_LENGTH], (PS8)L"..");
					break;
				}
				else
					str_total_width+=char_width;
			}

			str_width = str_total_width;
		}
#endif

		if(g_vdoply_osd_cntx.title.v_align == VIDEO_ALIGN_TOP)
			offset_y = 0;
		else if (g_vdoply_osd_cntx.title.v_align == VIDEO_ALIGN_BOTTOM)
			offset_y = bbox_rect_p->height-str_height;
		else /* center */
			offset_y = (bbox_rect_p->height-str_height) >> 1;

		offset_y += bbox_rect_p->offset_y;


		if(g_vdoply_osd_cntx.title.h_align == VIDEO_ALIGN_LEFT)
			offset_x = 0;
		else if(g_vdoply_osd_cntx.title.h_align == VIDEO_ALIGN_RIGHT)
			offset_x = bbox_rect_p->width-str_width;
		else
			offset_x = (bbox_rect_p->width-str_width) >> 1;

		offset_x += bbox_rect_p->offset_x;

		/* short file name need covert from BIG5 to unicode */
		if(g_vdoply_cntx.is_short_filename)
		{
			mmi_chset_mixed_text_to_ucs2_str( (U8*)buf_filename_dest, sizeof(buf_filename_dest), (U8*)buf_filename, PhnsetGetDefEncodingType() );
			
			mmi_vdoply_draw_style_text(buf_filename_dest, offset_x, offset_y,(video_style_text_struct *) &g_vdoply_osd_cntx.title.style_text);
		}
		else
		{
			mmi_vdoply_draw_style_text(buf_filename, offset_x, offset_y,(video_style_text_struct *) &g_vdoply_osd_cntx.title.style_text);
		}

#if defined(__PROJECT_GALLITE_C01__) 
		VP_text_stop_scrolling();
		vp_title_scrolling = TRUE;
		gui_create_scrolling_text(
		    &VP_scrolling_text,
		    offset_x,
		    offset_y,
		    UI_device_width - 1,
		    str_height,
		    (UI_string_type) buf_filename,
		    VP_scrolling_text_timer_callback,
		    VP_draw_scrolling_text_background,
		    gui_color(0, 0, 0),
		    gui_color(0, 0, 0));
		gui_show_scrolling_text(&VP_scrolling_text);
#endif

		coolsand_UI_reset_text_clip(); /* restore clip to full screen */
	
	}
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_panel_state
* DESCRIPTION
*  draw panel's state icon
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_draw_panel_state(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#if defined(__MMI_MAINLCD_240X320__)&& (!defined(__VDOPLY_FEATURE_TOUCH_SCREEN__))
    S32 img_width;          
    S32 img_height;
#endif                    //added by kecx for 240*320 screen without touch screen on 20090618

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    // beginning--modified by kecx on 20090310
	#ifndef __VDOPLY_FEATURE_TOUCH_SCREEN__
	#ifdef __MMI_MAINLCD_240X320__        //for 240*320 screen
		//guojian added begin 20071214 for jasper
		gdi_image_get_dimension_id(IMG_ID_VDOPLY_OSD_STATE_PLAY, &img_width, &img_height);
		gdi_layer_set_clip(	g_vdorec_osd_cntx.panel.state.offset_x, 
							g_vdorec_osd_cntx.panel.state.offset_y,
							g_vdorec_osd_cntx.panel.state.offset_x + img_width,
							g_vdorec_osd_cntx.panel.state.offset_y+ img_height);
		gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);
		gdi_layer_reset_clip();
		//guojian added end
	#endif
    // end--modified by kecx on 20090310
		switch(g_vdoply_cntx.state)
		{
			case VDOPLY_STATE_INTERRUPTED_PAUSE:
			case VDOPLY_STATE_PAUSE:
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.panel.state.offset_x, 
										g_vdoply_osd_cntx.panel.state.offset_y,
										(U16)(IMG_ID_VDOPLY_OSD_STATE_PLAY));
			}
			break;

			case VDOPLY_STATE_PLAY:
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.panel.state.offset_x, 
										g_vdoply_osd_cntx.panel.state.offset_y,
										(U16)(IMG_ID_VDOPLY_OSD_STATE_PAUSE));
			}
			break;	

			case VDOPLY_STATE_IDLE:
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.panel.state.offset_x, 
										g_vdoply_osd_cntx.panel.state.offset_y,
										(U16)(IMG_ID_VDOPLY_OSD_STATE_PLAY));
			}
			break;	

			case VDOPLY_STATE_SNAPSHOT:
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.panel.state.offset_x, 
										g_vdoply_osd_cntx.panel.state.offset_y,
										(U16)(IMG_ID_VDOPLY_OSD_STATE_SNAP));
			}
			break;	
		}
		
	#else
    // beginning--modified by kecx on 20090310
		/* fullscreen icon */
		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			if(g_vdoply_cntx.touch_fullscreen.is_press)
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.touch_fullscreen.offset_x,
										g_vdoply_osd_cntx.touch_fullscreen.offset_y,
										(U16)(IMG_ID_VDOPLY_TOUCH_OSD_FULLSCREEN_SEL));
			}
			else
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.touch_fullscreen.offset_x,
										g_vdoply_osd_cntx.touch_fullscreen.offset_y,
										(U16)(IMG_ID_VDOPLY_TOUCH_OSD_FULLSCREEN));
			}
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
		/* snapshot icon */
		#ifdef __VDOPLY_FEATURE_SNAPSHOT__
	
			if(g_vdoply_cntx.touch_snapshot.is_press)
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.touch_snapshot.offset_x,
										g_vdoply_osd_cntx.touch_snapshot.offset_y,
										(U16)(IMG_ID_VDOPLY_TOUCH_OSD_SNAPSHOT_SEL));
			}
			else
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.touch_snapshot.offset_x,
										g_vdoply_osd_cntx.touch_snapshot.offset_y,
										(U16)(IMG_ID_VDOPLY_TOUCH_OSD_SNAPSHOT));
			}				
			
		#endif /* __VDOPLY_FEATURE_SNAPSHOT__ */

		switch(g_vdoply_cntx.state)
		{
			case VDOPLY_STATE_IDLE:			
			case VDOPLY_STATE_INTERRUPTED_PAUSE:
			case VDOPLY_STATE_PAUSE:
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.panel.state.offset_x, 
										g_vdoply_osd_cntx.panel.state.offset_y,
										(U16)(IMG_ID_VDOPLY_OSD_STATE_PLAY));
			#if 0
				/* pause icon */
				if(g_vdoply_cntx.touch_play.is_press)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_play.offset_x,
											g_vdoply_osd_cntx.touch_play.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_OSD_PLAY_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_play.offset_x,
											g_vdoply_osd_cntx.touch_play.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_OSD_PLAY));
				}

				/* stop icon */
				if(g_vdoply_cntx.touch_stop.is_press)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_stop.offset_x,
											g_vdoply_osd_cntx.touch_stop.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_OSD_STOP_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_stop.offset_x,
											g_vdoply_osd_cntx.touch_stop.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_OSD_STOP));
				}
			#endif
			}
			break;

			case VDOPLY_STATE_PLAY:
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.panel.state.offset_x, 
										g_vdoply_osd_cntx.panel.state.offset_y,
										(U16)(IMG_ID_VDOPLY_OSD_STATE_PAUSE));
                       #if 0
				/* play icon */				
				if(g_vdoply_cntx.touch_play.is_press)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_play.offset_x,
											g_vdoply_osd_cntx.touch_play.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_OSD_PAUSE_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_play.offset_x,
											g_vdoply_osd_cntx.touch_play.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_OSD_PAUSE));
				}
				/* stop icon */
				if(g_vdoply_cntx.touch_stop.is_press)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_stop.offset_x,
											g_vdoply_osd_cntx.touch_stop.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_OSD_STOP_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_stop.offset_x,
											g_vdoply_osd_cntx.touch_stop.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_OSD_STOP));
				}
			#endif	
			}
			break;
    // end--modified by kecx on 20090310
		}
	#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_panel_progress
* DESCRIPTION
*  draw panel's progress bar
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_draw_panel_progress(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 offset_x;
	S32 offset_y;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* draw empty first */
	gdi_image_draw_id(g_vdoply_osd_cntx.panel.progress.bg.offset_x, 
							g_vdoply_osd_cntx.panel.progress.bg.offset_y,
							(U16)(IMG_ID_VDOPLY_OSD_PROGRESS_EMPTY));					

	if(g_vdoply_cntx.cur_status.play_ratio != 0)
	{
		/* draw inside filling */
		offset_x = g_vdoply_osd_cntx.panel.progress.fill.offset_x;
		offset_y = g_vdoply_osd_cntx.panel.progress.fill.offset_y;	
		
		gdi_layer_set_clip(	offset_x, 
							offset_y,
							offset_x+g_vdoply_cntx.cur_status.play_ratio,
							offset_y+g_vdoply_cntx.progress.fill_height);

		gdi_image_draw_id(g_vdoply_osd_cntx.panel.progress.fill.offset_x+ 2, 
								g_vdoply_osd_cntx.panel.progress.fill.offset_y,
								(U16)(IMG_ID_VDOPLY_OSD_PROGRESS_FILL));
	}
	gdi_layer_reset_clip();	
}


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_playing_progress
* DESCRIPTION
*  draw panel's progress bar
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_draw_playing_progress(U32 *is_ui_update)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 offset_x;
	S32 offset_y;
	U32	prev_play_ratio = g_vdoply_cntx.cur_status.play_ratio;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/				
 /* avoid driver return play time > video length */
		if(g_vdoply_cntx.cur_status.play_time > g_vdoply_cntx.video_info.total_time_duration)
		{
			g_vdoply_cntx.cur_status.play_time = g_vdoply_cntx.video_info.total_time_duration;
		}

		/* play ratio scale is 1/width of progress */
		g_vdoply_cntx.cur_status.play_ratio = 	(U32)(g_vdoply_cntx.cur_status.play_time*	\
																	g_vdoply_cntx.progress.fill_width/	\
																	g_vdoply_cntx.video_info.total_time_duration);
	if(g_vdoply_cntx.cur_status.play_ratio != prev_play_ratio)
		{
		/* draw inside filling */
		offset_x = g_vdoply_osd_cntx.panel.progress.fill.offset_x;
		offset_y = g_vdoply_osd_cntx.panel.progress.fill.offset_y;	
		
		gdi_layer_set_clip(	offset_x + prev_play_ratio, 
							offset_y,
							offset_x+g_vdoply_cntx.cur_status.play_ratio,
							offset_y+g_vdoply_cntx.progress.fill_height);

		gdi_image_draw_id(g_vdoply_osd_cntx.panel.progress.fill.offset_x+ 2, 
								g_vdoply_osd_cntx.panel.progress.fill.offset_y,
								(U16)(IMG_ID_VDOPLY_OSD_PROGRESS_FILL));
		gdi_layer_reset_clip();

		*is_ui_update = 1;
		}
		
}
/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_panel_timer
* DESCRIPTION
*  draw panel's timer
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdoply_draw_panel_timer(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32 time;
	S32 hour;
	S32 hour_remain;
	S32 min;
	S32 sec;


   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	time = (U32)(g_vdoply_cntx.cur_status.play_time/1000);
	hour				= time/3600;
	hour_remain		= time%3600;
	min				= hour_remain/60;
	sec				= hour_remain%60;

			/* hr */
			gdi_image_draw_id(g_vdoply_cntx.timer.hr_0_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)hour/10));

			gdi_image_draw_id(g_vdoply_cntx.timer.hr_1_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)hour%10));
			
			gdi_image_draw_id(g_vdoply_cntx.timer.col_0_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_COL));
			

			/* min */
			gdi_image_draw_id(g_vdoply_cntx.timer.min_0_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)min/10));

			gdi_image_draw_id(g_vdoply_cntx.timer.min_1_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)min%10));
			
			gdi_image_draw_id(g_vdoply_cntx.timer.col_1_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_COL));


			/* sec */
			gdi_image_draw_id(g_vdoply_cntx.timer.sec_0_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)sec/10));

			gdi_image_draw_id(g_vdoply_cntx.timer.sec_1_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)sec%10));

		

}





/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_Playing_timer
* DESCRIPTION
*  draw panel's timer
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdoply_draw_Playing_timer(U32 play_time,U32 *is_ui_update)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32 time;
	S32 hour;
	S32 hour_remain;
	S32 min;
	S32 sec;
       S32 old_sec = 0;
	S32 old_min = 0;
	S32 old_hour = 0;


   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	time = (U32)(g_vdoply_cntx.cur_status.play_time/1000);
	old_hour				= time/3600;
	hour_remain		= time%3600;
	old_min				= hour_remain/60;
	old_sec				= hour_remain%60;


	g_vdoply_cntx.cur_status.play_time = play_time;


	time = (U32)(g_vdoply_cntx.cur_status.play_time/1000);
	hour				= time/3600;
	hour_remain		= time%3600;
	min				= hour_remain/60;
	sec				= hour_remain%60;

	if(old_sec != sec)
		{
		if(old_hour != hour)
			{
			/* hr */
			gdi_image_draw_id(g_vdoply_cntx.timer.hr_0_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)hour/10));

			gdi_image_draw_id(g_vdoply_cntx.timer.hr_1_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)hour%10));
			
			gdi_image_draw_id(g_vdoply_cntx.timer.col_0_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_COL));
			
			}
			if(old_min != min)
				{
			/* min */
			gdi_image_draw_id(g_vdoply_cntx.timer.min_0_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)min/10));

			gdi_image_draw_id(g_vdoply_cntx.timer.min_1_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)min%10));
			
			gdi_image_draw_id(g_vdoply_cntx.timer.col_1_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_COL));
				}
			/* sec */
			if(sec/10 != old_sec/10)
				{
				gdi_image_draw_id(g_vdoply_cntx.timer.sec_0_offset_x,
										g_vdoply_cntx.timer.offset_y, 
										(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)sec/10));
				}

			gdi_image_draw_id(g_vdoply_cntx.timer.sec_1_offset_x,
									g_vdoply_cntx.timer.offset_y, 
									(U16)(IMG_ID_VDOPLY_OSD_TIMER_0+(U16)sec%10));

			*is_ui_update = 1;
		}
		

}

/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_panel_speed
* DESCRIPTION
*  draw panel's speed icon
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_draw_panel_speed(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOPLY_FEATURE_SPEED__
		gdi_image_draw_id(g_vdoply_osd_cntx.panel.speed.offset_x, 
								g_vdoply_osd_cntx.panel.speed.offset_y,
								(U16)(IMG_ID_VDOPLY_OSD_SPEED_START+g_vdoply_cntx.cur_status.speed+1));


		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__

			/* speed inc */
			if( (g_vdoply_cntx.touch_speed_inc.is_press) || 
				 (g_vdoply_cntx.is_speed_inc_key_pressed) )
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.touch_speed_inc.offset_x,
										g_vdoply_osd_cntx.touch_speed_inc.offset_y,
										IMG_ID_VDOPLY_TOUCH_OSD_SPEED_INC_SEL);			
			}
			else
			{

				if(g_vdoply_cntx.cur_status.speed < VDOPLY_FEATURE_SPEED_COUNT-1)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_speed_inc.offset_x,
											g_vdoply_osd_cntx.touch_speed_inc.offset_y,
											IMG_ID_VDOPLY_TOUCH_OSD_SPEED_INC);		

				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_speed_inc.offset_x,
											g_vdoply_osd_cntx.touch_speed_inc.offset_y,
											IMG_ID_VDOPLY_TOUCH_OSD_SPEED_INC_DIS);	
						
				}
			}

			/* speed dec */
			if( (g_vdoply_cntx.touch_speed_dec.is_press) || 
				 (g_vdoply_cntx.is_speed_dec_key_pressed) )
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.touch_speed_dec.offset_x,
										g_vdoply_osd_cntx.touch_speed_dec.offset_y,
										IMG_ID_VDOPLY_TOUCH_OSD_SPEED_DEC_SEL);			
			}
			else
			{
				if(g_vdoply_cntx.cur_status.speed > 0)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_speed_dec.offset_x,
											g_vdoply_osd_cntx.touch_speed_dec.offset_y,
											IMG_ID_VDOPLY_TOUCH_OSD_SPEED_DEC);		
				}
				else
				{
						gdi_image_draw_id(g_vdoply_osd_cntx.touch_speed_dec.offset_x,
												g_vdoply_osd_cntx.touch_speed_dec.offset_y,
												IMG_ID_VDOPLY_TOUCH_OSD_SPEED_DEC_DIS);	
				}	
			}
		
		#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
	
	#endif /* __VDOPLY_FEATURE_SPEED__ */

}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_panel_volume
* DESCRIPTION
*  draw panel's volumn icon
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_draw_panel_volume(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	#ifdef __VDOPLY_FEATURE_VOLUME__
             //if (g_vdoply_cntx.cur_status.volume >= 0)
             {
             
		gdi_image_draw_id(g_vdoply_osd_cntx.panel.volume.offset_x, 
								g_vdoply_osd_cntx.panel.volume.offset_y,
								(U16)(IMG_ID_VDOPLY_OSD_VOLUME_START+g_vdoply_cntx.cur_status.volume));
             }
    
		TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_draw_panel_volume g_vdoply_cntx.cur_status.volume = %d",g_vdoply_cntx.cur_status.volume);
		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		TRACE_EF(g_sw_MED_Vid,"touch_volume_inc.offset_x=%d, touch_volume_inc.offset_y=%d",g_vdoply_osd_cntx.touch_volume_inc.offset_x,g_vdoply_osd_cntx.touch_volume_inc.offset_y);
			/* vol inc */
			if( (g_vdoply_cntx.touch_volume_inc.is_press) || 
				 (g_vdoply_cntx.is_vol_inc_key_pressed) )
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.touch_volume_inc.offset_x,
										g_vdoply_osd_cntx.touch_volume_inc.offset_y,
//										IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_DOWN);			
										IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_INC_SEL);			
			}
			else
			{
				if(g_vdoply_cntx.cur_status.volume <= LEVEL7)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_volume_inc.offset_x,
											g_vdoply_osd_cntx.touch_volume_inc.offset_y,
//											IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_UP);		
											IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_INC);		
				}
				else
				{
						gdi_image_draw_id(g_vdoply_osd_cntx.touch_volume_inc.offset_x,
												g_vdoply_osd_cntx.touch_volume_inc.offset_y,
//												IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_INC_UP);	
												IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_INC_DIS);	
				}
			}
			/* vol dec */
			if( (g_vdoply_cntx.touch_volume_dec.is_press) ||
				 (g_vdoply_cntx.is_vol_dec_key_pressed) )
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.touch_volume_dec.offset_x,
										g_vdoply_osd_cntx.touch_volume_dec.offset_y,
//										IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_DOWN);			
										IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_DEC_SEL);			
			}			
			else
			{
				if(g_vdoply_cntx.cur_status.volume > LEVEL1)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_volume_dec.offset_x,
											g_vdoply_osd_cntx.touch_volume_dec.offset_y,
//											IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_UP);		
											IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_DEC);		
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_volume_dec.offset_x,
											g_vdoply_osd_cntx.touch_volume_dec.offset_y,
//											IMG_ID_AUDPLY_MAIN_SKIN1_VOLUME_DEC_UP);	
											IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_DEC_DIS);	
				}
			}
		
		#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
	
	#endif /* __VDOPLY_FEATURE_VOLUME__ */
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_style_text
* DESCRIPTION
*  init panel's UI item
* PARAMETERS
*  str	 		In		string pointer
* 	offset_x	 	In  	string pos x
* 	offset_y 	In  	string pos y
*  style_txt	In 	style
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_draw_style_text(PS8 str, S32 offset_x, S32 offset_y, video_style_text_struct *style_txt)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(r2lMMIFlag)
	{
		offset_x += coolsand_UI_get_string_width((UI_string_type)str);
	}
	
	if(style_txt->style == VIDEO_TEXT_STYLE_NORMAL)
	{
		coolsand_UI_move_text_cursor(offset_x, offset_y);
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->r, style_txt->g ,style_txt->b));
	  	coolsand_UI_print_text((UI_string_type)str);	
	}
	else if(style_txt->style == VIDEO_TEXT_STYLE_BORDER)
	{
		coolsand_UI_move_text_cursor(offset_x, offset_y);
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->r, style_txt->g, style_txt->b));
		coolsand_UI_set_text_border_color(coolsand_UI_color(style_txt->style_r, style_txt->style_g, style_txt->style_b));
		coolsand_UI_print_bordered_text((UI_string_type)str);
	}
	else if(style_txt->style == VIDEO_TEXT_STYLE_SHADOW)
	{
		coolsand_UI_move_text_cursor(offset_x+1, offset_y+1);
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->style_r, style_txt->style_g ,style_txt->style_b));
		coolsand_UI_print_text((UI_string_type)str);	

		coolsand_UI_move_text_cursor(offset_x, offset_y);	
		coolsand_UI_set_text_color(coolsand_UI_color(style_txt->r, style_txt->g ,style_txt->b));
		coolsand_UI_print_text((UI_string_type)str);	
	}
	else
	{
		MMI_ASSERT(0);
	} 
	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_softkey
* DESCRIPTION
*  draw panel's buttom bar
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_draw_softkey(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 lsk_offset_x;
	S32 lsk_offset_y;	
	S32 rsk_offset_x;
	S32 rsk_offset_y;	
	S32 str_width;
	S32 str_height;
	S32 img_width;
	S32 img_height;
	U16 image_id;


	#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		vdoply_ui_touch_struct	*touch_p;
	#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	TRACE_EF(g_sw_MED_Vid, "(g_vdorec_osd_cntx.softkey.is_icon_softkey == FALSE)=%d",(g_vdorec_osd_cntx.softkey.is_icon_softkey == FALSE));
	if(g_vdoply_osd_cntx.softkey.is_icon_softkey == FALSE)
	{

#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__     // beginning--modified by kecx on 20090310
		gdi_image_get_dimension_id(IMG_ID_VDOPLY_OSD_STATE_PLAY, &img_width, &img_height);
		gdi_layer_set_clip(	g_vdoply_osd_cntx.softkey.rect.offset_x, 
							g_vdoply_osd_cntx.softkey.rect.offset_y,
							g_vdoply_osd_cntx.softkey.rect.offset_x + (LCD_WIDTH - img_width)/2,
							g_vdoply_osd_cntx.softkey.rect.offset_y + img_height);
		gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);
		gdi_layer_set_clip(	LCD_WIDTH - (LCD_WIDTH - img_width)/2, 
							LCD_HEIGHT - img_height,
							LCD_WIDTH,
							LCD_HEIGHT);
		gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);	
		gdi_layer_reset_clip();
#else   // end--modified by kecx on 20090310
              
#if defined(__MMI_MAINLCD_220X176__) || defined(MEM_ULC_3216)
		gdi_layer_set_clip(	g_vdoply_osd_cntx.softkey.rect.offset_x,
								g_vdoply_osd_cntx.softkey.rect.offset_y,
							LCD_WIDTH - 1,
							LCD_HEIGHT - 1);
		gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);
		gdi_layer_reset_clip();
		mmi_vdoply_draw_panel_timer();
#else
		gdi_image_draw_id(g_vdoply_osd_cntx.softkey.rect.offset_x,
								g_vdoply_osd_cntx.softkey.rect.offset_y,
								IMG_ID_VDOPLY_OSD_BUTTOM_BAR_BG);	
#endif
#endif  // modified by kecx for fixing bug 12526 on 20090611
		/* lsk */
		if(g_vdoply_cntx.lsk_str_ptr != NULL)
		{

			coolsand_UI_measure_string((UI_string_type)g_vdoply_cntx.lsk_str_ptr, &str_width, &str_height);
			lsk_offset_x = 2;
			lsk_offset_y = (UI_device_height-MMI_button_bar_height)+((MMI_button_bar_height-str_height)>>1)+1;

			if(g_vdoply_cntx.is_lsk_pressed)
			{
				lsk_offset_x++;
				lsk_offset_y++;	
			}

			mmi_vdoply_draw_style_text(g_vdoply_cntx.lsk_str_ptr, 
												lsk_offset_x,
												lsk_offset_y,
												(video_style_text_struct *)&g_vdoply_osd_cntx.softkey.lsk_text);

			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				touch_p = &g_vdoply_cntx.touch_lsk;
				touch_p->offset_x = lsk_offset_x;
				touch_p->offset_y = lsk_offset_y;
				touch_p->width		= str_width;
				touch_p->height	= str_height;			
			#endif		

		}

		/* rsk */
		if(g_vdoply_cntx.rsk_str_ptr != NULL)
		{
			
			coolsand_UI_measure_string((UI_string_type)g_vdoply_cntx.rsk_str_ptr, &str_width, &str_height);
			rsk_offset_x = UI_device_width-str_width-2;
			rsk_offset_y = (UI_device_height-MMI_button_bar_height)+((MMI_button_bar_height-str_height)>>1)+1;

			if(g_vdoply_cntx.is_rsk_pressed)
			{
				rsk_offset_x++;
				rsk_offset_y++;	
			}
			
			mmi_vdoply_draw_style_text(g_vdoply_cntx.rsk_str_ptr, 
												rsk_offset_x,
												rsk_offset_y,
												(video_style_text_struct *)&g_vdoply_osd_cntx.softkey.lsk_text);

			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				touch_p = &g_vdoply_cntx.touch_rsk;
				touch_p->offset_x = rsk_offset_x;
				touch_p->offset_y = rsk_offset_y;
				touch_p->width		= str_width;
				touch_p->height	= str_height;			
			#endif		

		}
	}
	else
	{
		/* draw icon softkey */
		gdi_layer_reset_clip();

		/* lsk */
		if(g_vdoply_cntx.lsk_img_id != 0)
		{
			if(g_vdoply_cntx.is_lsk_pressed)
				image_id = g_vdoply_cntx.lsk_img_id+1;
			else
				image_id = g_vdoply_cntx.lsk_img_id;

			gdi_image_get_dimension_id(image_id, &img_width, &img_height);

			lsk_offset_x = g_vdoply_osd_cntx.softkey.icon_lsk_pos.offset_x;
			lsk_offset_y = g_vdoply_osd_cntx.softkey.icon_lsk_pos.offset_y;

			gdi_image_draw_id(lsk_offset_x, lsk_offset_y, image_id);

			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				/* remain these code if design change */
				touch_p = &g_vdoply_cntx.touch_lsk;
				touch_p->offset_x = lsk_offset_x;
				touch_p->offset_y = lsk_offset_y;
				touch_p->width		= img_width;
				touch_p->height	= img_height;			
			#endif				
		}
		
		/* rsk */
		if(g_vdoply_cntx.rsk_img_id != 0)
		{
			if(g_vdoply_cntx.is_rsk_pressed)
			{
				image_id = g_vdoply_cntx.rsk_img_id+1;
			}
			else
			{
				image_id = g_vdoply_cntx.rsk_img_id;
			}
			
			gdi_image_get_dimension_id(image_id, &img_width, &img_height);

			rsk_offset_x = g_vdoply_osd_cntx.softkey.icon_rsk_pos.offset_x;
			rsk_offset_y = g_vdoply_osd_cntx.softkey.icon_rsk_pos.offset_y;

			gdi_image_draw_id(rsk_offset_x, rsk_offset_y, image_id);

			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				touch_p = &g_vdoply_cntx.touch_rsk;
				touch_p->offset_x = rsk_offset_x;
				touch_p->offset_y = rsk_offset_y;
				touch_p->width		= img_width;
				touch_p->height	= img_height;			
			#endif				
		}

	}

	
}




/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_Leftsoftkey
* DESCRIPTION
*  draw panel's buttom bar
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_draw_Leftsoftkeypress(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 lsk_offset_x;
	S32 lsk_offset_y;	
	S32 str_width;
	S32 str_height;
	S32 img_width;
	S32 img_height;
	U16 image_id;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	TRACE_EF(g_sw_MED_Vid, "(g_vdorec_osd_cntx.softkey.is_icon_softkey == FALSE)=%d",(g_vdorec_osd_cntx.softkey.is_icon_softkey == FALSE));
	if(g_vdoply_osd_cntx.softkey.is_icon_softkey == FALSE)
	{
		/* lsk */
		if(g_vdoply_cntx.lsk_str_ptr != NULL)
		{

			coolsand_UI_measure_string((UI_string_type)g_vdoply_cntx.lsk_str_ptr, &str_width, &str_height);
			lsk_offset_x = 3;
			lsk_offset_y = (UI_device_height-MMI_button_bar_height)+((MMI_button_bar_height-str_height)>>1)+2;

			mmi_vdoply_draw_style_text(g_vdoply_cntx.lsk_str_ptr, 
												lsk_offset_x,
												lsk_offset_y,
												(video_style_text_struct *)&g_vdoply_osd_cntx.softkey.lsk_text);

		}
		
	}
	else
	{
		/* draw icon softkey */
		gdi_layer_reset_clip();

		/* lsk */
		if(g_vdoply_cntx.lsk_img_id != 0)
		{
			if(g_vdoply_cntx.is_lsk_pressed)
				image_id = g_vdoply_cntx.lsk_img_id+1;
			else
				image_id = g_vdoply_cntx.lsk_img_id;

			gdi_image_get_dimension_id(image_id, &img_width, &img_height);

			lsk_offset_x = g_vdoply_osd_cntx.softkey.icon_lsk_pos.offset_x;
			lsk_offset_y = g_vdoply_osd_cntx.softkey.icon_lsk_pos.offset_y;

			gdi_image_draw_id(lsk_offset_x, lsk_offset_y, image_id);
		}
		
	}

	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_softkey
* DESCRIPTION
*  draw panel's buttom bar
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_draw_Rightsoftkeypress(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 rsk_offset_x;
	S32 rsk_offset_y;	
	S32 str_width;
	S32 str_height;
	S32 img_width;
	S32 img_height;
	U16 image_id;



   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	TRACE_EF(g_sw_MED_Vid, "(g_vdorec_osd_cntx.softkey.is_icon_softkey == FALSE)=%d",(g_vdorec_osd_cntx.softkey.is_icon_softkey == FALSE));
	if(g_vdoply_osd_cntx.softkey.is_icon_softkey == FALSE)
	{
		/* rsk */
		if(g_vdoply_cntx.rsk_str_ptr != NULL)
		{
			
			coolsand_UI_measure_string((UI_string_type)g_vdoply_cntx.rsk_str_ptr, &str_width, &str_height);
			rsk_offset_x = UI_device_width-str_width-1;
			rsk_offset_y = (UI_device_height-MMI_button_bar_height)+((MMI_button_bar_height-str_height)>>1)+2;
			
			mmi_vdoply_draw_style_text(g_vdoply_cntx.rsk_str_ptr, 
												rsk_offset_x,
												rsk_offset_y,
												(video_style_text_struct *)&g_vdoply_osd_cntx.softkey.lsk_text);

		}
	}
	else
	{
		/* draw icon softkey */
		gdi_layer_reset_clip();

		/* rsk */
		if(g_vdoply_cntx.rsk_img_id != 0)
		{
			if(g_vdoply_cntx.is_rsk_pressed)
			{
				image_id = g_vdoply_cntx.rsk_img_id+1;
			}
			else
			{
				image_id = g_vdoply_cntx.rsk_img_id;
			}
			
			gdi_image_get_dimension_id(image_id, &img_width, &img_height);

			rsk_offset_x = g_vdoply_osd_cntx.softkey.icon_rsk_pos.offset_x;
			rsk_offset_y = g_vdoply_osd_cntx.softkey.icon_rsk_pos.offset_y;

			gdi_image_draw_id(rsk_offset_x, rsk_offset_y, image_id);

		}

	}

	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_panel
* DESCRIPTION
*  draw panel
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_draw_panel(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_vdoply_draw_panel_volume();
	mmi_vdoply_draw_panel_speed();
	mmi_vdoply_draw_panel_timer();
	mmi_vdoply_draw_panel_progress();
	mmi_vdoply_draw_panel_state();
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_init_panel
* DESCRIPTION
*  init panel's UI item
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_init_panel(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 digit_width;
	S32 col_width;	
	S32 height;
	
	#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		vdoply_ui_touch_struct	*touch_p;
	#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	/* timer */
	gdi_image_get_dimension_id(IMG_ID_VDOPLY_OSD_TIMER_COL, 	&col_width, &height);	
	gdi_image_get_dimension_id(IMG_ID_VDOPLY_OSD_TIMER_0, 	&digit_width, &height);
	
	g_vdoply_cntx.timer.offset_y 			= g_vdoply_osd_cntx.panel.timer.offset_y;
	
	g_vdoply_cntx.timer.hr_0_offset_x 	= g_vdoply_osd_cntx.panel.timer.offset_x;	
	g_vdoply_cntx.timer.hr_1_offset_x	= g_vdoply_cntx.timer.hr_0_offset_x+digit_width;
	g_vdoply_cntx.timer.col_0_offset_x	= g_vdoply_cntx.timer.hr_1_offset_x+digit_width;
	g_vdoply_cntx.timer.min_0_offset_x	= g_vdoply_cntx.timer.col_0_offset_x+col_width;
	g_vdoply_cntx.timer.min_1_offset_x	= g_vdoply_cntx.timer.min_0_offset_x+digit_width;
	g_vdoply_cntx.timer.col_1_offset_x	= g_vdoply_cntx.timer.min_1_offset_x+digit_width;
	g_vdoply_cntx.timer.sec_0_offset_x	= g_vdoply_cntx.timer.col_1_offset_x+col_width;	
	g_vdoply_cntx.timer.sec_1_offset_x	= g_vdoply_cntx.timer.sec_0_offset_x+digit_width;	

	/* progress */
	gdi_image_get_dimension_id(IMG_ID_VDOPLY_OSD_PROGRESS_FILL,	
										&g_vdoply_cntx.progress.fill_width, 
										&g_vdoply_cntx.progress.fill_height);



	#define INIT_TOUCH_OBJ(__x__, __X__)\
	{\
		touch_p = &g_vdoply_cntx.touch_##__x__;\
		touch_p->offset_x = g_vdoply_osd_cntx.touch_##__x__.offset_x;\
		touch_p->offset_y = g_vdoply_osd_cntx.touch_##__x__.offset_y;\
		touch_p->is_press = FALSE;\
		gdi_image_get_dimension_id(__X__, &touch_p->width, &touch_p->height);\
	}\

	#define INIT_TOUCH_FULL_OBJ(__x__, __X__)\
	{\
		touch_p = &g_vdoply_cntx.full_touch_##__x__;\
		touch_p->offset_x = g_vdoply_osd_cntx.full_touch_##__x__##.offset_x;\
		touch_p->offset_y = g_vdoply_osd_cntx.full_touch_##__x__##.offset_y;\
		touch_p->is_press = FALSE;\
		gdi_image_get_dimension_id(IMG_ID_VDOPLY_TOUCH_FULL_OSD_##__X__##, &touch_p->width, &touch_p->height);\
	}\

	#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;
	
		/* init touch obj region */
		
//		INIT_TOUCH_OBJ(speed_inc,SPEED_INC);
//		INIT_TOUCH_OBJ(speed_dec,SPEED_DEC);
		INIT_TOUCH_OBJ(volume_inc,IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_INC);	
		INIT_TOUCH_OBJ(volume_dec,IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_DEC);	
		INIT_TOUCH_OBJ(progress_inc,IMG_ID_VDOPLY_TOUCH_OSD_PROGRESS_INC);	
		INIT_TOUCH_OBJ(progress_dec,IMG_ID_VDOPLY_TOUCH_OSD_PROGRESS_DEC);
		INIT_TOUCH_OBJ(play,IMG_ID_VDOPLY_OSD_STATE_PLAY);
		INIT_TOUCH_OBJ(stop,IMG_ID_VDOPLY_OSD_STATE_STOP);
//		INIT_TOUCH_OBJ(snapshot,SNAPSHOT);
		INIT_TOUCH_OBJ(fullscreen,IMG_ID_VDOPLY_TOUCH_OSD_FULLSCREEN);

		/* progress bar */
		touch_p = &g_vdoply_cntx.touch_progress_bar;
		touch_p->offset_x = g_vdoply_osd_cntx.panel.progress.empty.offset_x;
		touch_p->offset_y = g_vdoply_osd_cntx.panel.progress.empty.offset_y;
		touch_p->is_press = FALSE;
		gdi_image_get_dimension_id(IMG_ID_VDOPLY_OSD_PROGRESS_BG, &touch_p->width, &touch_p->height);

		#if 0//def __VDOPLY_FEATURE_FULLSCREEN__
//			INIT_TOUCH_FULL_OBJ(speed_inc,SPEED_INC);
//			INIT_TOUCH_FULL_OBJ(speed_dec,SPEED_DEC);
			INIT_TOUCH_FULL_OBJ(volume_inc,VOLUME_INC);	
			INIT_TOUCH_FULL_OBJ(volume_dec,VOLUME_DEC);	
			INIT_TOUCH_FULL_OBJ(progress_inc,PROGRESS_INC);	
			INIT_TOUCH_FULL_OBJ(progress_dec,PROGRESS_DEC);
			INIT_TOUCH_FULL_OBJ(play,PLAY);
			INIT_TOUCH_FULL_OBJ(stop,STOP);

			/* progress bar */
			touch_p = &g_vdoply_cntx.full_touch_progress_bar;
			touch_p->offset_x = g_vdoply_osd_cntx.full_panel.progress.empty.offset_x;
			touch_p->offset_y = g_vdoply_osd_cntx.full_panel.progress.empty.offset_y;
			touch_p->is_press = FALSE;
			gdi_image_get_dimension_id(IMG_ID_VDOPLY_OSD_PROGRESS_EMPTY, &touch_p->width, &touch_p->height);
				
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */


	#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
	

}





/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_fullscr_panel_state
* DESCRIPTION
*  draw fullscreen panel's state icon
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
static void mmi_vdoply_draw_fullscr_panel_state(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifndef __VDOPLY_FEATURE_TOUCH_SCREEN__

	  	/* currently - fullscreen do not draw state */
		
	#else
		gdi_layer_reset_clip();
	
		/* draw play/pause/stop icons */
		switch(g_vdoply_cntx.state)
		{
			case VDOPLY_STATE_FULLSCR_IDLE:			
			case VDOPLY_STATE_FULLSCR_PAUSE:
			{
				/* pause icon */
				if(g_vdoply_cntx.full_touch_play.is_press)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_play.offset_x,
											g_vdoply_osd_cntx.full_touch_play.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_FULL_OSD_PLAY_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_play.offset_x,
											g_vdoply_osd_cntx.full_touch_play.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_FULL_OSD_PLAY));
				}

				/* stop icon */
				if(g_vdoply_cntx.full_touch_stop.is_press)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_stop.offset_x,
											g_vdoply_osd_cntx.full_touch_stop.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_FULL_OSD_STOP_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_stop.offset_x,
											g_vdoply_osd_cntx.full_touch_stop.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_FULL_OSD_STOP));
				}

			}
			break;
		

			case VDOPLY_STATE_FULLSCR_PLAY:
			{
				/* play icon */				
				if(g_vdoply_cntx.full_touch_play.is_press)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_play.offset_x,
											g_vdoply_osd_cntx.full_touch_play.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_FULL_OSD_PAUSE_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_play.offset_x,
											g_vdoply_osd_cntx.full_touch_play.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_FULL_OSD_PAUSE));
				}
			
				/* stop icon */
				if(g_vdoply_cntx.full_touch_stop.is_press)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_stop.offset_x,
											g_vdoply_osd_cntx.full_touch_stop.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_FULL_OSD_STOP_SEL));
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_stop.offset_x,
											g_vdoply_osd_cntx.full_touch_stop.offset_y,
											(U16)(IMG_ID_VDOPLY_TOUCH_FULL_OSD_STOP));
				}

			}
			break;
		
		}

	#endif

}
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_fullscr_panel_progress
* DESCRIPTION
*  draw fullscreen panel's progress bar
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
static void mmi_vdoply_draw_fullscr_panel_progress(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 offset_x;
	S32 offset_y;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	gdi_image_draw_id(g_vdoply_osd_cntx.full_panel.progress.bg.offset_x, 
							g_vdoply_osd_cntx.full_panel.progress.bg.offset_y,
							(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_PROGRESS_BG));



	/* draw empty first */
	gdi_image_draw_id(g_vdoply_osd_cntx.full_panel.progress.empty.offset_x, 
							g_vdoply_osd_cntx.full_panel.progress.empty.offset_y,
							(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_PROGRESS_EMPTY));
							

	if(g_vdoply_cntx.cur_status.play_ratio != 0)
	{
		/* draw inside filling */
		offset_x = g_vdoply_osd_cntx.full_panel.progress.fill.offset_x;
		offset_y = g_vdoply_osd_cntx.full_panel.progress.fill.offset_y;	
		
		gdi_layer_set_clip(	offset_x, 
									offset_y,
									offset_x+g_vdoply_cntx.cur_status.play_ratio,
									offset_y+g_vdoply_cntx.fullscr_progress.fill_height);

		gdi_image_draw_id(g_vdoply_osd_cntx.full_panel.progress.fill.offset_x, 
								g_vdoply_osd_cntx.full_panel.progress.fill.offset_y,
								(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_PROGRESS_FILL));
	}
	
	gdi_layer_reset_clip();



	#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__

		/* progress inc */
		if( (g_vdoply_cntx.full_touch_progress_inc.is_press) ||
			 (g_vdoply_cntx.is_seek_fwd_key_pressed))
		{
			gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_progress_inc.offset_x,
									g_vdoply_osd_cntx.full_touch_progress_inc.offset_y,
									IMG_ID_VDOPLY_TOUCH_FULL_OSD_PROGRESS_INC_SEL);			
		}
		else
		{

			gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_progress_inc.offset_x,
									g_vdoply_osd_cntx.full_touch_progress_inc.offset_y,
									IMG_ID_VDOPLY_TOUCH_FULL_OSD_PROGRESS_INC);		
		}
		
		/* progress dec */
		if( (g_vdoply_cntx.full_touch_progress_dec.is_press) ||
			 (g_vdoply_cntx.is_seek_bwd_key_pressed))
		{
			gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_progress_dec.offset_x,
									g_vdoply_osd_cntx.full_touch_progress_dec.offset_y,
									IMG_ID_VDOPLY_TOUCH_FULL_OSD_PROGRESS_DEC_SEL);			
		}
		else
		{

			gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_progress_dec.offset_x,
									g_vdoply_osd_cntx.full_touch_progress_dec.offset_y,
									IMG_ID_VDOPLY_TOUCH_FULL_OSD_PROGRESS_DEC);		
		}
		
	#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

	
}
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_fullscr_panel_timer
* DESCRIPTION
*  draw fullscreen panel's timer
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
void mmi_vdoply_draw_fullscr_panel_timer(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32 time;
	S32 hour;
	S32 hour_remain;
	S32 min;
	S32 sec;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	time = (U32)(g_vdoply_cntx.cur_status.play_time/1000);

	hour				= time/3600;
	hour_remain		= time%3600;
	min				= hour_remain/60;
	sec				= hour_remain%60;

	/* hr */
	gdi_image_draw_id(g_vdoply_cntx.fullscr_timer.hr_0_offset_x,
							g_vdoply_cntx.fullscr_timer.offset_y, 
							(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_TIMER_0+(U16)hour/10));

	gdi_image_draw_id(g_vdoply_cntx.fullscr_timer.hr_1_offset_x,
							g_vdoply_cntx.fullscr_timer.offset_y, 
							(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_TIMER_0+(U16)hour%10));
	
	gdi_image_draw_id(g_vdoply_cntx.fullscr_timer.col_0_offset_x,
							g_vdoply_cntx.fullscr_timer.offset_y, 
							(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_TIMER_COL));
	
	/* min */
	gdi_image_draw_id(g_vdoply_cntx.fullscr_timer.min_0_offset_x,
							g_vdoply_cntx.fullscr_timer.offset_y, 
							(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_TIMER_0+(U16)min/10));

	gdi_image_draw_id(g_vdoply_cntx.fullscr_timer.min_1_offset_x,
							g_vdoply_cntx.fullscr_timer.offset_y, 
							(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_TIMER_0+(U16)min%10));
	
	gdi_image_draw_id(g_vdoply_cntx.fullscr_timer.col_1_offset_x,
							g_vdoply_cntx.fullscr_timer.offset_y, 
							(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_TIMER_COL));

	/* sec */
	gdi_image_draw_id(g_vdoply_cntx.fullscr_timer.sec_0_offset_x,
							g_vdoply_cntx.fullscr_timer.offset_y, 
							(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_TIMER_0+(U16)sec/10));

	gdi_image_draw_id(g_vdoply_cntx.fullscr_timer.sec_1_offset_x,
							g_vdoply_cntx.fullscr_timer.offset_y, 
							(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_TIMER_0+(U16)sec%10));

}
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_fullscr_panel_speed
* DESCRIPTION
*  draw fullscreen panel's speed icon
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
static void mmi_vdoply_draw_fullscr_panel_speed(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOPLY_FEATURE_SPEED__
		gdi_image_draw_id(g_vdoply_osd_cntx.full_panel.speed.offset_x, 
								g_vdoply_osd_cntx.full_panel.speed.offset_y,
								(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_SPEED_START+g_vdoply_cntx.cur_status.speed+1));

		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__

			/* speed inc */
			if( (g_vdoply_cntx.full_touch_speed_inc.is_press) || 
			  	 (g_vdoply_cntx.is_speed_inc_key_pressed) )
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_speed_inc.offset_x,
										g_vdoply_osd_cntx.full_touch_speed_inc.offset_y,
										IMG_ID_VDOPLY_TOUCH_FULL_OSD_SPEED_INC_SEL);			
			}
			else
			{

				if(g_vdoply_cntx.cur_status.speed < VDOPLY_FEATURE_SPEED_COUNT-1)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_speed_inc.offset_x,
											g_vdoply_osd_cntx.full_touch_speed_inc.offset_y,
											IMG_ID_VDOPLY_TOUCH_FULL_OSD_SPEED_INC);		

				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_speed_inc.offset_x,
											g_vdoply_osd_cntx.full_touch_speed_inc.offset_y,
											IMG_ID_VDOPLY_TOUCH_FULL_OSD_SPEED_INC_DIS);	
						
				}
			}

			/* speed dec */
			if( (g_vdoply_cntx.full_touch_speed_dec.is_press) || 
			  	 (g_vdoply_cntx.is_speed_dec_key_pressed) )
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_speed_dec.offset_x,
										g_vdoply_osd_cntx.full_touch_speed_dec.offset_y,
										IMG_ID_VDOPLY_TOUCH_FULL_OSD_SPEED_DEC_SEL);			
			}
			else
			{
				if(g_vdoply_cntx.cur_status.speed > 0)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_speed_dec.offset_x,
											g_vdoply_osd_cntx.full_touch_speed_dec.offset_y,
											IMG_ID_VDOPLY_TOUCH_FULL_OSD_SPEED_DEC);		
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_speed_dec.offset_x,
											g_vdoply_osd_cntx.full_touch_speed_dec.offset_y,
											IMG_ID_VDOPLY_TOUCH_FULL_OSD_SPEED_DEC_DIS);	
				}	
			}
		
		#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

	
	#endif /* __VDOPLY_FEATURE_SPEED__ */

}
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_fullscr_panel_volume
* DESCRIPTION
*  draw fullscreen panel's volumn icon
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
static void mmi_vdoply_draw_fullscr_panel_volume(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	#ifdef __VDOPLY_FEATURE_VOLUME__
		gdi_image_draw_id(g_vdoply_osd_cntx.full_panel.volume.offset_x, 
								g_vdoply_osd_cntx.full_panel.volume.offset_y,
								(U16)(IMG_ID_VDOPLY_FULLSCR_OSD_VOLUME_START+g_vdoply_cntx.cur_status.volume+1));

		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		
			/* vol inc */
			if(g_vdoply_cntx.full_touch_volume_inc.is_press)
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_volume_inc.offset_x,
										g_vdoply_osd_cntx.full_touch_volume_inc.offset_y,
										IMG_ID_VDOPLY_TOUCH_FULL_OSD_VOLUME_INC_SEL);			
			}
			else
			{
				if(g_vdoply_cntx.cur_status.volume < LEVEL7)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_volume_inc.offset_x,
											g_vdoply_osd_cntx.full_touch_volume_inc.offset_y,
											IMG_ID_VDOPLY_TOUCH_FULL_OSD_VOLUME_INC);		
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_volume_inc.offset_x,
											g_vdoply_osd_cntx.full_touch_volume_inc.offset_y,
											IMG_ID_VDOPLY_TOUCH_FULL_OSD_VOLUME_INC_DIS);	
				}
			}

				
			/* vol dec */
			if(g_vdoply_cntx.full_touch_volume_dec.is_press)
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_volume_dec.offset_x,
										g_vdoply_osd_cntx.full_touch_volume_dec.offset_y,
										IMG_ID_VDOPLY_TOUCH_FULL_OSD_VOLUME_DEC_SEL);			
			}			
			else
			{
				if(g_vdoply_cntx.cur_status.volume > LEVEL1)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_volume_dec.offset_x,
											g_vdoply_osd_cntx.full_touch_volume_dec.offset_y,
											IMG_ID_VDOPLY_TOUCH_FULL_OSD_VOLUME_DEC);		
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.full_touch_volume_dec.offset_x,
											g_vdoply_osd_cntx.full_touch_volume_dec.offset_y,
											IMG_ID_VDOPLY_TOUCH_FULL_OSD_VOLUME_DEC_DIS);	
				}
			}
		
		#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
	
	#endif /* __VDOPLY_FEATURE_VOLUME__ */
}
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_fullscr_softkey
* DESCRIPTION
*  draw fullscreen's softkey
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
static void mmi_vdoply_draw_fullscr_softkey(void)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 lsk_offset_x;
	S32 lsk_offset_y;	
	S32 rsk_offset_x;
	S32 rsk_offset_y;	
	S32 str_width;
	S32 str_height;
	S32 spacing;

	#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		vdoply_ui_touch_struct *touch_p;
	#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* lsk */
	if(g_vdoply_cntx.lsk_str_ptr != NULL)
	{
		coolsand_UI_measure_string((UI_string_type)g_vdoply_cntx.lsk_str_ptr, &str_width, &str_height);
		spacing = (MMI_SOFTKEY_HEIGHT-str_height)>>1;
		
		lsk_offset_x = UI_device_height-str_width-spacing;
		lsk_offset_y = UI_device_width-str_height-spacing;

		if(g_vdoply_cntx.is_lsk_pressed)
		{
			lsk_offset_x++;
			lsk_offset_y++;	
		}

		mmi_vdoply_draw_style_text(g_vdoply_cntx.lsk_str_ptr, 
											lsk_offset_x,
											lsk_offset_y,
											(video_style_text_struct *)&g_vdoply_osd_cntx.full_softkey.lsk_text);

		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		
			touch_p = &g_vdoply_cntx.full_touch_lsk;
			touch_p->offset_x = lsk_offset_x;
			touch_p->offset_y = lsk_offset_y;
			touch_p->width		= str_width;
			touch_p->height	= str_height;			
			
		#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */				
	}


	/* rsk */
	if(g_vdoply_cntx.rsk_str_ptr != NULL)
	{
		
		coolsand_UI_measure_string((UI_string_type)g_vdoply_cntx.rsk_str_ptr, &str_width, &str_height);
		spacing = (MMI_SOFTKEY_HEIGHT-str_height)>>1;		
		
		rsk_offset_x = UI_device_height-str_width-spacing;
		rsk_offset_y = spacing;

		if(g_vdoply_cntx.is_rsk_pressed)
		{
			rsk_offset_x++;
			rsk_offset_y++;	
		}
		
		mmi_vdoply_draw_style_text(g_vdoply_cntx.rsk_str_ptr, 
											rsk_offset_x,
											rsk_offset_y,
											(video_style_text_struct *)&g_vdoply_osd_cntx.full_softkey.rsk_text);

		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		
			touch_p = &g_vdoply_cntx.full_touch_rsk;
			touch_p->offset_x = rsk_offset_x;
			touch_p->offset_y = rsk_offset_y;
			touch_p->width		= str_width;
			touch_p->height	= str_height;			
			
		#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */	
		
	}

}
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_draw_fullscr_panel
* DESCRIPTION
*  draw fullscreen panel
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/

#ifdef __VDOPLY_FEATURE_FULLSCREEN__

void mmi_vdoply_draw_fullscr_osd(void)
{
	S32 layer_width, layer_height, offset_x, offset_y;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	gdi_layer_push_and_set_active(g_vdoply_cntx.osd_layer_handle);
	gdi_layer_toggle_double();
	gdi_layer_resize(UI_device_height, UI_device_width);	

//	gdi_layer_clear(GDI_COLOR_TRANSPARENT);

	gdi_image_draw_id(g_vdoply_osd_cntx.full_panel.rect.offset_x, 
							g_vdoply_osd_cntx.full_panel.rect.offset_y,
							IMG_ID_AUDPLY_MAIN_SKIN1_BG);
//							IMG_ID_VDOPLY_FULLSCR_OSD_BG);
	gdi_layer_get_dimension(&layer_width, &layer_height);
	gdi_layer_get_position(&offset_x, &offset_y);
	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_draw_fullscr_osd  osd_layer_handle(%d,%d,%d,%d)",offset_x,offset_y,layer_width,layer_height);											

	mmi_vdoply_draw_fullscr_softkey();
	mmi_vdoply_draw_fullscr_panel_volume();
	mmi_vdoply_draw_fullscr_panel_speed();
	mmi_vdoply_draw_fullscr_panel_timer();
	mmi_vdoply_draw_fullscr_panel_progress(); 
	mmi_vdoply_draw_fullscr_panel_state();

//	gdi_layer_set_rotate_osd(GDI_LAYER_ROTATE_270);
//	gdi_layer_resize(UI_device_width, UI_device_height);	

	gdi_layer_pop_and_restore_active();
}

#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_init_fullscr_panel
* DESCRIPTION
*  init fullscreen panel's UI item
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
static void mmi_vdoply_init_fullscr_panel(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 digit_width;
	S32 col_width;	
	S32 height;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* timer */
	gdi_image_get_dimension_id(IMG_ID_VDOPLY_FULLSCR_OSD_TIMER_COL, 	&col_width, &height);	
	gdi_image_get_dimension_id(IMG_ID_VDOPLY_FULLSCR_OSD_TIMER_0, 		&digit_width, &height);
	
	g_vdoply_cntx.fullscr_timer.offset_y 			= g_vdoply_osd_cntx.full_panel.timer.offset_y;
	
	g_vdoply_cntx.fullscr_timer.hr_0_offset_x 	= g_vdoply_osd_cntx.full_panel.timer.offset_x;	
	g_vdoply_cntx.fullscr_timer.hr_1_offset_x		= g_vdoply_cntx.fullscr_timer.hr_0_offset_x+digit_width;
	g_vdoply_cntx.fullscr_timer.col_0_offset_x	= g_vdoply_cntx.fullscr_timer.hr_1_offset_x+digit_width;
	g_vdoply_cntx.fullscr_timer.min_0_offset_x	= g_vdoply_cntx.fullscr_timer.col_0_offset_x+col_width;
	g_vdoply_cntx.fullscr_timer.min_1_offset_x	= g_vdoply_cntx.fullscr_timer.min_0_offset_x+digit_width;
	g_vdoply_cntx.fullscr_timer.col_1_offset_x	= g_vdoply_cntx.fullscr_timer.min_1_offset_x+digit_width;
	g_vdoply_cntx.fullscr_timer.sec_0_offset_x	= g_vdoply_cntx.fullscr_timer.col_1_offset_x+col_width;	
	g_vdoply_cntx.fullscr_timer.sec_1_offset_x	= g_vdoply_cntx.fullscr_timer.sec_0_offset_x+digit_width;	

	/* progress */
	gdi_image_get_dimension_id(IMG_ID_VDOPLY_FULLSCR_OSD_PROGRESS_FILL,	
										&g_vdoply_cntx.fullscr_progress.fill_width, 
										&g_vdoply_cntx.fullscr_progress.fill_height);
	

}
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_fullscr_reset_osd_fade
* DESCRIPTION
*  fade fullscr osd
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
#if 0
static void mmi_vdoply_fullscr_reset_osd_fade(BOOL sleep)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdoply_cntx.fullscr_osd_opacity != 255)
	{
		g_vdoply_cntx.fullscr_osd_opacity = 255;

 		gdi_layer_push_and_set_active(g_vdoply_cntx.osd_layer_handle);
		gdi_layer_set_opacity(TRUE, (U8)g_vdoply_cntx.fullscr_osd_opacity);
		gdi_layer_pop_and_restore_active();  
	}

	if(sleep)
	{
		/* start fade out timer */
		coolsand_UI_start_timer(VDOPLY_OSD_FADE_OUT_TIME, mmi_vdoply_fullscr_osd_fade_cyclic);   
	}
}
#endif
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_fullscr_osd_fade_cyclic
* DESCRIPTION
*  fade fullscr osd
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
static void mmi_vdoply_fullscr_osd_fade_cyclic(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* reset fullscreen osd fade out timer */
	if(g_vdoply_cntx.fullscr_osd_opacity > VDOPLY_OSD_FADE_OUT_VALUE)
	{
		g_vdoply_cntx.fullscr_osd_opacity -= VDOPLY_OSD_FADE_OUT_VALUE;
		
		coolsand_UI_start_timer(100, mmi_vdoply_fullscr_osd_fade_cyclic);
	}
	else
	{
		g_vdoply_cntx.fullscr_osd_opacity = 0;

		/* if totally fade out, use any key to wake up */
		ClearAllKeyHandler();
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_1, 				KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_2, 				KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_3, 				KEY_EVENT_UP);		
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_4, 				KEY_EVENT_UP);		
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_5, 				KEY_EVENT_UP);			
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_6,				KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_7, 				KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_8,				KEY_EVENT_UP);		
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_9, 				KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_0, 				KEY_EVENT_UP);		
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_POUND, 		KEY_EVENT_UP);
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_LSK, 			KEY_EVENT_UP);
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_RSK, 			KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_ENTER, 		KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_LEFT_ARROW, 	KEY_EVENT_UP);
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_RIGHT_ARROW, KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_UP_ARROW, 	KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_DOWN_ARROW, 	KEY_EVENT_UP);	
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_CLEAR, 		KEY_EVENT_UP);		

		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_VOL_UP, 		KEY_EVENT_UP);
		SetKeyHandler(mmi_vdoply_fullscr_osd_show_up, KEY_VOL_DOWN, 	KEY_EVENT_UP);

		/* start still use for toggle screen */
		SetKeyHandler(mmi_vdoply_toggle_fullscreen_display_timer, KEY_STAR, 	KEY_EVENT_UP);
	
	}
	
 	gdi_layer_push_and_set_active(g_vdoply_cntx.osd_layer_handle);
	gdi_layer_set_opacity(TRUE, (U8)g_vdoply_cntx.fullscr_osd_opacity);
	gdi_layer_pop_and_restore_active();  

//	mmi_vdoply_blt_screen();

}
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_fullscr_osd_show_up
* DESCRIPTION
*  fade fullscr osd
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
static void mmi_vdoply_fullscr_osd_show_up(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_vdoply_register_key_event(g_vdoply_cntx.state);
//	mmi_vdoply_fullscr_reset_osd_fade(TRUE);	

//	mmi_vdoply_blt_screen();
}
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_get_next_filename
* DESCRIPTION
*  get next file name to be snapshot
* PARAMETERS
*  file_buf  	OUT		filename buffer ptr
*	file_path	IN			filepath
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_SNAPSHOT__

static void mmi_vdoply_get_next_snapshot_filename(PS8 file_buf, PS8 file_path)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8					buffer[25];
	S8					UCS2_buffer[50];
	S8					UCS2_testname[50];
	S8					UCS2_alpha[10];
	S16				error = 0;
	U16				filename_seq_no;
	U16				alpha_count;
	U16				alpha_index;
	FS_HANDLE		file_handle;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	/* get/store sequentail number */
	ReadValue(NVRAM_VDOPLY_SNAPSHOT_SEQ_NO, &filename_seq_no, DS_SHORT , &error);
	if(filename_seq_no == 0xffff)
	{
		filename_seq_no = 1;	/* init value */
	}
	else 
	{
		/* TODO: if seq no turn around, add some protection */
		filename_seq_no++;
		if(filename_seq_no > 9999)	/* more than 4 digits */
			filename_seq_no = 1;
	}
	WriteValue(NVRAM_VDOPLY_SNAPSHOT_SEQ_NO, &filename_seq_no, DS_SHORT , &error);

	alpha_count		= 'Z'-'A'+1 ;
	alpha_index		= 0;

	sprintf((PS8)buffer, "SNP%04d", filename_seq_no);
	AnsiiToUnicodeString((PS8)UCS2_buffer, (PS8)buffer);	

	while(1)
	{
		sprintf((PS8)buffer, "%c", 'A'+alpha_index);
		AnsiiToUnicodeString((PS8)UCS2_alpha, (PS8)buffer);

		pfnUnicodeStrcpy(UCS2_testname, UCS2_buffer);
		pfnUnicodeStrcat(UCS2_testname, UCS2_alpha);
		pfnUnicodeStrcat(UCS2_testname, (PS8)L".jpg");

		pfnUnicodeStrncpy((PS8)file_buf, (PS8)file_path, FMGR_MAX_PATH_LEN);
		pfnUnicodeStrcat((PS8)file_buf, (PS8)UCS2_testname);

		file_handle = MMI_FS_Open((const UINT8 *)file_buf, FS_READ_ONLY);
	
		if(file_handle >= 0)
		{
			/* file exist */
			FS_Close(file_handle);
			alpha_index++;	
		}
		else
		{
			/* file not exit. return */
			return;
		}

		/* exit condition - will get a IMGXXXXZ.jpg as file name */
		if(alpha_index >= alpha_count-1)
			return;
	}
}

#endif

/*****************************************************************************
* FUNCTION
*	mmi_vdoply_player_snapshot
* DESCRIPTION
*  snapshot current play screen.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_SNAPSHOT__
static void mmi_vdoply_player_snapshot(void) 
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8						buf_pathname[FMGR_PATH_BUFFER_SIZE];
	S8						buf_filename[FMGR_PATH_BUFFER_SIZE];
	U16					prev_state;
	MDI_RESULT			ret;
	MDI_RESULT			snap_ret;
	S32					create_result;


   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdoply_cntx.is_seeking)
	{
		/* if is seeking, do not take snapshot, this may avoid delay timer and NVRAM access problem */
		return;
	}

	/* already exit */
	if(GetExitScrnID() != SCR_ID_VDOPLY_PLAYER)
		return;

	memset(buf_pathname, 0, FMGR_PATH_BUFFER_SIZE);
	
	/* init value */
	ret = 0;

	/* check if storage path is valid or not */
	if(mmi_vdorec_check_and_restore_valid_drv())
	{
		/* if drv is valid, check if the folder is crated or not */
		mmi_camera_get_storage_file_path(buf_pathname);
		create_result = mmi_camera_create_file_dir(buf_pathname);

		/* create directory failed */
		if(create_result == FS_WRITE_PROTECTION)
		{
			ret = MDI_RES_VDOPLY_ERR_SNAPSHOT_WRITE_PROTECTION;
		}
		else if(create_result == FS_DISK_FULL)
		{
			ret  = MDI_RES_VDOPLY_ERR_SNAPSHOT_DISK_FULL;
		}
		else if(create_result == FS_ROOT_DIR_FULL)
		{
			ret  = MDI_RES_VDOPLY_ERR_SNAPSHOT_ROOT_DIR_FULL;
		}
		else
		{
			/* no error */
			ret = MDI_RES_VDOPLY_SUCCEED;
		}
		
		/* error check */
		if(mmi_vdoply_check_and_display_error_popup(ret) == TRUE) /* ture means has error popup */
		{
			return;	
		}
	}
	else	
 	{
		return;
	}

	prev_state = g_vdoply_cntx.state;
	g_vdoply_cntx.state	= VDOPLY_STATE_SNAPSHOT;

	/* start snapshot */	
	mmi_vdoply_get_next_snapshot_filename(buf_filename, buf_pathname);

	if(prev_state == VDOPLY_STATE_PLAY)
	{
		/* stop */
		mdi_video_ply_stop();
	}
    else if (g_vdoply_cntx.state == VDOPLY_STATE_BT_CONNECTING ||
             g_vdoply_cntx.state == VDOPLY_STATE_STREAM_BT_CONNECTING)
    {    
        mmi_vdoply_disconnect_bt();
    }


	/* draw snapshot state icon */
	mmi_vdoply_draw_panel_state();
	mmi_vdoply_blt_screen();

	snap_ret = mdi_video_ply_snapshot(g_vdoply_cntx.play_wnd_layer_handle, buf_filename);

	g_vdoply_cntx.state = prev_state;

	if(prev_state == VDOPLY_STATE_PLAY)
	{
		/* if is seeking, means snapshot event is insert into seeking event */
		/* since after seeking, video will restart again, we do not need to start play here */

	//	ret = mmi_vdoply_play_video();

	        if (mmi_vdoply_is_output_to_bt())
	        {
	            mmi_vdoply_player_enter_state(VDOPLY_STATE_BT_CONNECTING);
	        }
	        else
	        {
	            ret = mmi_vdoply_play_video();
			/* error check */
			if(mmi_vdoply_check_and_display_error_popup(ret) == TRUE) /* ture means has error popup */
				return;	
               
	        }

		/* reset start play time */
		g_vdoply_cntx.cur_status.start_play_time = g_vdoply_cntx.cur_status.play_time;		
	}
	
	/* draw previw state icon */
	mmi_vdoply_draw_panel_state();
	mmi_vdoply_blt_screen();


	/* error check */
	if(mmi_vdoply_check_and_display_error_popup(snap_ret) == TRUE) /* ture means has error popup */
		return;	
	
}
#endif /* __VDOPLY_FEATURE_SNAPSHOT__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_player_snapshot_key_press
* DESCRIPTION
*  press snapshot key
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_SNAPSHOT__
static void mmi_vdoply_player_snapshot_key_press(void) 
{
	#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		/* draw press snapshot key */	
		g_vdoply_cntx.touch_snapshot.is_press = TRUE;
	#endif

	mmi_vdoply_draw_panel_state();
	mmi_vdoply_blt_screen();	
}
#endif /* __VDOPLY_FEATURE_SNAPSHOT__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_player_snapshot_key_release
* DESCRIPTION
*  release snapshot key
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_SNAPSHOT__
static void mmi_vdoply_player_snapshot_key_release(void) 
{
	#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		/* draw release snapshot key */	
		g_vdoply_cntx.touch_snapshot.is_press = FALSE;
	#endif
 
	mmi_vdoply_player_snapshot();	
}
#endif /* __VDOPLY_FEATURE_SNAPSHOT__ */





static void mmi_vdoply_play_time_result_hdlr(UINT32 playtime)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdoply_cntx.is_seeking == FALSE)
		g_vdoply_cntx.cur_status.play_time = playtime;
}


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_player_timer_cyclic
* DESCRIPTION
*  timer. will ask actual play time from driver and display on screen.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_player_timer_cyclic(void)  
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32	prev_play_ratio;
	U32	prev_play_time;
	BOOL	is_ui_update;	
	S32 	speed_factor;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	is_ui_update		= FALSE;
	prev_play_time		= (U32)(g_vdoply_cntx.cur_status.play_time);	/* change to sec */
	prev_play_ratio	= g_vdoply_cntx.cur_status.play_ratio;
	speed_factor		= mmi_vdoply_get_speed_factor();

	if(g_vdoply_cntx.state == VDOPLY_STATE_PLAY)
	{
		/* get current play time */
		mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);

		/* avoid driver return play time > video length */
		if(g_vdoply_cntx.cur_status.play_time > g_vdoply_cntx.video_info.total_time_duration)
		{
			g_vdoply_cntx.cur_status.play_time = g_vdoply_cntx.video_info.total_time_duration;
		}

		/* play ratio scale is 1/width of progress */
		g_vdoply_cntx.cur_status.play_ratio = 	(U32)(g_vdoply_cntx.cur_status.play_time*	\
																	g_vdoply_cntx.progress.fill_width/	\
																	g_vdoply_cntx.video_info.total_time_duration);

//		if(g_vdoply_cntx.cur_status.play_time != prev_play_time) 
		{
			if( ((speed_factor > 0) && (g_vdoply_cntx.cur_status.play_time > g_vdoply_cntx.cur_status.start_play_time)) ||  /* play foward */
				 ((speed_factor < 0) && (g_vdoply_cntx.cur_status.play_time < g_vdoply_cntx.cur_status.start_play_time)) )	/* play backward */
			{
				is_ui_update = TRUE;
	
				mmi_vdoply_draw_panel_timer();

				if(g_vdoply_cntx.cur_status.play_ratio != prev_play_ratio)
				{
					mmi_vdoply_draw_panel_progress();
				}
			}	
		}

		if(is_ui_update)
		{
		//	mmi_vdoply_blt_screen();

			mmi_vdoply_blt_part();
//		       display_flag=1;   //added by guojian for refresh mp4 UI  at "vid_msg_handler.c"
		}
			
		/* start cyclic timer */	
//		coolsand_UI_start_timer(300, mmi_vdoply_player_timer_cyclic); //update playing time by MDI,so not update here



	}
	#ifdef __VDOPLY_FEATURE_FULLSCREEN__
	else if(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY)
	{
		/* get current play time */
//		mdi_video_ply_get_cur_play_time(&g_vdoply_cntx.cur_status.play_time);
		mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);


		/* avoid driver return play time > video length */
		if(g_vdoply_cntx.cur_status.play_time > g_vdoply_cntx.video_info.total_time_duration)
		{
			g_vdoply_cntx.cur_status.play_time = g_vdoply_cntx.video_info.total_time_duration;
		}

#if 0
		/* play ratio scale is 1/width of progress */
		g_vdoply_cntx.cur_status.play_ratio = 	(U32)(g_vdoply_cntx.cur_status.play_time*	\
																	g_vdoply_cntx.fullscr_progress.fill_width/	\
																	g_vdoply_cntx.video_info.total_time_duration);
#else
		/* play ratio scale is 1/width of progress */
		g_vdoply_cntx.cur_status.play_ratio = 	(U32)(g_vdoply_cntx.cur_status.play_time*	\
																	g_vdoply_cntx.progress.fill_width/	\
																	g_vdoply_cntx.video_info.total_time_duration);

#endif /* Guoj delete. It's not used now */

//		if(g_vdoply_cntx.cur_status.play_time != prev_play_time) 
		{
			if( ((speed_factor > 0) && (g_vdoply_cntx.cur_status.play_time > g_vdoply_cntx.cur_status.start_play_time)) ||  /* play foward */
				 ((speed_factor < 0) && (g_vdoply_cntx.cur_status.play_time < g_vdoply_cntx.cur_status.start_play_time)) )	/* play backward */
			{
				is_ui_update = TRUE;

//				mmi_vdoply_draw_fullscr_osd();
			}	
		}

		if(is_ui_update)
		{
//			mmi_vdoply_blt_screen();
		}

		/* start cyclic timer */	
		coolsand_UI_start_timer(300, mmi_vdoply_player_timer_cyclic);

	}
	#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */


	
}
/*****************************************************************************
* FUNCTION
*	mmi_vdoply_playing_draw_pressevent_icon
* DESCRIPTION
*  draw playing press event icon
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_playing_draw_pressevent_icon(U32 *is_ui_update)  
{
		/*lest softkey*/
		if(
			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
			g_vdoply_cntx.touch_lsk.is_press||
			#endif
			g_vdoply_cntx.is_lsk_pressed)
			{
			mmi_vdoply_draw_Leftsoftkeypress();
			*is_ui_update = 1;
			}
		/*right softkey*/
		if(
			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
			g_vdoply_cntx.touch_rsk.is_press||
			#endif
			g_vdoply_cntx.is_rsk_pressed)
			{
			mmi_vdoply_draw_Rightsoftkeypress();
			*is_ui_update = 1;
			}
		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
		/*play / pause softkey*/
		if(g_vdoply_cntx.touch_play.is_press)
			{
			gdi_image_draw_id(g_vdoply_osd_cntx.panel.state.offset_x, 
								g_vdoply_osd_cntx.panel.state.offset_y,
								(U16)(IMG_ID_VDOPLY_OSD_STATE_PAUSE));
			*is_ui_update = 1;
			}
		/*full screen*/
		if(g_vdoply_cntx.touch_fullscreen.is_press)
			{
				gdi_image_draw_id(g_vdoply_osd_cntx.touch_fullscreen.offset_x,
										g_vdoply_osd_cntx.touch_fullscreen.offset_y,
										(U16)(IMG_ID_VDOPLY_TOUCH_OSD_FULLSCREEN_SEL));
			*is_ui_update = 1;
			}
		#endif
#ifdef __VDOPLY_FEATURE_VOLUME__
		if(
			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
			g_vdoply_cntx.touch_volume_inc.is_press||
			g_vdoply_cntx.touch_volume_dec.is_press||
			#endif
			g_vdoply_cntx.is_vol_inc_key_pressed||
			g_vdoply_cntx.is_vol_dec_key_pressed)
			{
			mmi_vdoply_draw_panel_volume();
			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
			g_vdoply_cntx.touch_volume_inc.is_press = FALSE;
			g_vdoply_cntx.touch_volume_dec.is_press = FALSE;
			#endif
			g_vdoply_cntx.is_vol_inc_key_pressed = FALSE;
			g_vdoply_cntx.is_vol_dec_key_pressed = FALSE;
			*is_ui_update = 1;
			}
			else
				{
		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
			/* vol inc */
				if(g_vdoply_cntx.cur_status.volume <= LEVEL7)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_volume_inc.offset_x,
											g_vdoply_osd_cntx.touch_volume_inc.offset_y,
											IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_INC);		
				}
				else
				{
						gdi_image_draw_id(g_vdoply_osd_cntx.touch_volume_inc.offset_x,
												g_vdoply_osd_cntx.touch_volume_inc.offset_y,
												IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_INC_DIS);	
				}

			/* vol dec */
				if(g_vdoply_cntx.cur_status.volume > LEVEL1)
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_volume_dec.offset_x,
											g_vdoply_osd_cntx.touch_volume_dec.offset_y,
											IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_DEC);		
				}
				else
				{
					gdi_image_draw_id(g_vdoply_osd_cntx.touch_volume_dec.offset_x,
											g_vdoply_osd_cntx.touch_volume_dec.offset_y,
											IMG_ID_VDOPLY_TOUCH_OSD_VOLUME_DEC_DIS);	
				}

		*is_ui_update = 1;
		#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

				}
#endif

}
extern void gdi_layer_Merge_frame_buffer_forPartVideo(U16 lcd_y);
extern BOOL gdi_jmpbuf_is_used;
static void mmi_vdoply_player_timer(U32 play_time)  
{
	U32	is_ui_update = 0; 
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
   if(gdi_jmpbuf_is_used)
   	{
   		return;
   	}
	if(g_vdoply_cntx.state == VDOPLY_STATE_PLAY)
	{

		mmi_vdoply_draw_Playing_timer(play_time,&is_ui_update);

		mmi_vdoply_draw_playing_progress(&is_ui_update);

		mmi_vdoply_playing_draw_pressevent_icon(&is_ui_update);

		if(1 == is_ui_update)
		{
#ifdef __MMI_MAINLCD_220X176__
		gdi_layer_merge_for_video(g_vdoply_osd_cntx.play_wnd.rect.offset_x+g_vdoply_osd_cntx.play_wnd.rect.width, 0, UI_device_width-1, g_vdoply_osd_cntx.play_wnd.rect.offset_y+g_vdoply_osd_cntx.play_wnd.rect.height);	
#endif
		gdi_layer_Merge_frame_buffer_forPartVideo(g_vdoply_osd_cntx.play_wnd.rect.offset_y+g_vdoply_osd_cntx.play_wnd.rect.height);
		}
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_set_softkey
* DESCRIPTION
*  set softkey display string
* PARAMETERS
*  lsk_str_ptr	 IN		lsk string
*  rsk_str_ptr	 IN		rsk string
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_set_softkey(PS8 lsk_str_ptr, PS8 rsk_str_ptr, U16 lsk_img_id, U16 rsk_img_id)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_vdoply_cntx.lsk_str_ptr	= lsk_str_ptr;
	g_vdoply_cntx.rsk_str_ptr 	= rsk_str_ptr;
	
	g_vdoply_cntx.lsk_img_id 	= lsk_img_id;
	g_vdoply_cntx.rsk_img_id 	= rsk_img_id;	
}

#if 0
static void mmi_vdoply_lsk_release_timer(void)  //timer ʱСӦƵ guojian added 20080619
{
	coolsand_UI_start_timer(200, mmi_vdoply_lsk_release);

}
#endif

#ifdef __VDOPLY_FEATURE_FULLSCREEN__
static void mmi_vdoply_toggle_fullscreen_display_timer(void)  //timer ʱСӦƵ guojian added 20080619
{
	coolsand_UI_cancel_timer(mmi_vdoply_toggle_fullscreen_display);
	coolsand_UI_start_timer(200, mmi_vdoply_toggle_fullscreen_display);
}
#endif
/*****************************************************************************
* FUNCTION
*	mmi_vdoply_lsk_release
* DESCRIPTION
*  lsk release hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_lsk_release(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32 cur_time;
   U32 total_time;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	TBM_ENTRY(0x2A64);
	g_vdoply_cntx.is_lsk_pressed = FALSE;

	/* stop seek key timer */
	if(g_vdoply_cntx.is_seeking)
		coolsand_UI_cancel_timer(mmi_vdoply_seek_key_release);

	/* stop update time timer */
	coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
		
	switch(g_vdoply_cntx.state)
	{
	       case VDOPLY_STATE_BT_CONNECTING:
	            mmi_vdoply_draw_softkey();
	            mmi_vdoply_blt_screen();
	            
	        #ifdef __VDOPLY_FEATURE_LSK_OPTION__            
	        
	            /* link to option screen */
	        #ifdef __VDOPLY_FEATURE_ENHNACE_OPTION__	
	            mmi_vdoply_entry_option_scr();
	        #else
	            mmi_vdoply_entry_adjust_option_scr();
	        #endif
	       
	        #endif /* __VDOPLY_FEATURE_LSK_OPTION__ */ 
	            break;
		case VDOPLY_STATE_IDLE:		/* play */
			TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_lsk_release VDOPLY_STATE_IDLE");
			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				g_vdoply_cntx.touch_play.is_press = FALSE;
			#endif
			if((gZoomHeight == VDOPLY_DISPLAY_SET_HEIGHT) &&  (gZoomWidth == VDOPLY_DISPLAY_SET_WIDTH)) //full screen
			{
					mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
				}
			else
				{
	//				mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
			            if (mmi_vdoply_is_output_to_bt())
			            {
			                mmi_vdoply_player_enter_state(VDOPLY_STATE_BT_CONNECTING);
			            }
			            else
			            {
			                mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
			            }
					
				}
			break;

		case VDOPLY_STATE_PLAY:		/* pause */

			/* if is seeking in play state, the video is already stopped */
			if(!g_vdoply_cntx.is_seeking)
			{
				mdi_video_ply_pause();
			}
			/* get latest time */
			mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);

			/* re-calc paly ratio for progress display */
			g_vdoply_cntx.cur_status.play_ratio = (U32)(g_vdoply_cntx.cur_status.play_time*g_vdoply_cntx.progress.fill_width/g_vdoply_cntx.video_info.total_time_duration);
			
			mmi_vdoply_draw_panel_timer();
			mmi_vdoply_draw_panel_progress();

			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				g_vdoply_cntx.touch_play.is_press = FALSE;
			#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
			
			
			mmi_vdoply_player_enter_state(VDOPLY_STATE_PAUSE);


			break;

		case VDOPLY_STATE_INTERRUPTED_PAUSE:
		case VDOPLY_STATE_PAUSE:	/* resume */
		
			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				g_vdoply_cntx.touch_play.is_press = FALSE;
				mmi_vdoply_draw_panel_state();
			#else
				mmi_vdoply_draw_softkey();
			#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */			

//			mmi_vdoply_blt_screen();
			mmi_vdoply_blt_part();
			
//			mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);

	            if (mmi_vdoply_is_output_to_bt())
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_BT_CONNECTING);
	            }
	            else
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
	            }
			
			break;
			
		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			/* full screen */
			case VDOPLY_STATE_FULLSCR_IDLE:
//				mmi_vdoply_draw_fullscr_osd();
//				mmi_vdoply_blt_screen();

             TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_lsk_release VDOPLY_STATE_FULLSCR_IDLE");
//				mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
		            if (mmi_vdoply_is_output_to_bt())
		            {
		                mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_BT_CONNECTING);
		            }
		            else
		            {
		                mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
		            }

				break;
				
			case VDOPLY_STATE_FULLSCR_PAUSE:
				/* get latest time */
//				mdi_video_ply_get_cur_play_time(&g_vdoply_cntx.cur_status.play_time);
				mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);
				/* re-calc paly ratio for progress display */
				cur_time 	= g_vdoply_cntx.cur_status.play_time;
				total_time 	= g_vdoply_cntx.video_info.total_time_duration;
				g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.fullscr_progress.fill_width/total_time);
				
//				mmi_vdoply_draw_fullscr_osd();
//				mmi_vdoply_blt_screen();
		   TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_lsk_release VDOPLY_STATE_FULLSCR_PAUSE");		
//				mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
			            if (mmi_vdoply_is_output_to_bt())
			            {
			                mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_BT_CONNECTING);
			            }
			            else
			            {
			                mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
			            }
				break;
				
			case VDOPLY_STATE_FULLSCR_PLAY:
//				mmi_vdoply_draw_fullscr_osd();
//				mmi_vdoply_blt_screen();
				
				/* if is seeking in play state, the video is already stopped */
				if(!g_vdoply_cntx.is_seeking)
				{
					//mdi_video_ply_stop();
					mdi_video_ply_pause();
				}
				
				mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PAUSE);
				break;
				
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
		
	}

	if(g_vdoply_cntx.is_seeking)
		g_vdoply_cntx.is_seeking = FALSE;

	TBM_EXIT(0x2A64);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_rsk_release
* DESCRIPTION
*  rsk release hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_rsk_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_vdoply_cntx.is_rsk_pressed = FALSE;	

	TBM_ENTRY(0x2A6A);
	if(g_vdoply_cntx.is_seeking)
		coolsand_UI_cancel_timer(mmi_vdoply_seek_key_release);

	#if 1///ndef __VDOPLY_FEATURE_TOUCH_SCREEN__

		switch(g_vdoply_cntx.state)
		{
			case VDOPLY_STATE_EXIT:		/* exit */
//				mmi_vdoply_draw_softkey();
//				mmi_vdoply_blt_screen();
//				mmi_vdoply_blt_part();
			
				mmi_vdoply_player_enter_state(VDOPLY_STATE_EXIT);
				break;
			case VDOPLY_STATE_IDLE:		/* exit */
//				mmi_vdoply_draw_softkey();
//				mmi_vdoply_blt_screen();
//				mmi_vdoply_blt_part();
				
				if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_FILE)
				{
					mdi_video_ply_close_file();
				}
				else if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_ID)
				{
					mdi_video_ply_close_clip_id();		
				}
				
				mmi_vdoply_player_enter_state(VDOPLY_STATE_EXIT);
				break;

			case VDOPLY_STATE_PLAY:		/* stop */
//				mmi_vdoply_draw_softkey();
//				mmi_vdoply_blt_screen();
//				mmi_vdoply_blt_part();
				
			/* if is seeking in play state, the video is already stopped */
				if(!g_vdoply_cntx.is_seeking)
					mdi_video_ply_stop();

				
				mmi_vdoply_player_enter_state(VDOPLY_STATE_IDLE);
				break;


		        case VDOPLY_STATE_BT_CONNECTING: 
		            mmi_vdoply_draw_softkey();
		            mmi_vdoply_blt_screen();
		            mmi_vdoply_disconnect_bt();
		            mmi_vdoply_player_enter_state(VDOPLY_STATE_IDLE);
		            break;

			case VDOPLY_STATE_INTERRUPTED_PAUSE:
			case VDOPLY_STATE_PAUSE:	 /* stop */		
//				mmi_vdoply_draw_softkey();
//				mmi_vdoply_blt_screen();
//				mmi_vdoply_blt_part();
				
				mmi_vdoply_player_enter_state(VDOPLY_STATE_IDLE);
				break;

			case VDOPLY_STATE_INTERRUPTED_SEEKING:
//				mmi_vdoply_draw_softkey();
//				mmi_vdoply_blt_screen();
//				mmi_vdoply_blt_part();

				mdi_video_ply_stop_non_block_seek();

				if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_FILE)
				{
					mdi_video_ply_close_file();
				}
				else if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_ID)
				{
					mdi_video_ply_close_clip_id();		
				}
				
				mmi_vdoply_player_enter_state(VDOPLY_STATE_EXIT);
				
				break;

			case VDOPLY_STATE_INTERRUPTED_OPENING:
			case VDOPLY_STATE_OPENING:	/* exit */
//				mmi_vdoply_draw_softkey();
//				mmi_vdoply_blt_screen();
//				mmi_vdoply_blt_part();
				
				/* stop loading animation */
				gdi_image_stop_animation_all();

				if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_FILE)
				{
					mdi_video_ply_close_file();
				}
				else if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_ID)
				{
					mdi_video_ply_close_clip_id();		
				}
				
				mmi_vdoply_player_enter_state(VDOPLY_STATE_EXIT);
				break;

				
			#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			
				/* full screen */
				case VDOPLY_STATE_FULLSCR_IDLE:
//					mmi_vdoply_draw_fullscr_osd();
//					mmi_vdoply_blt_screen();
					
					/* rotate base layer back to normal */
//					gdi_layer_set_rotate((U8)g_vdoply_cntx.normal_style_rotate);	
//					gdi_layer_resize(g_vdoply_cntx.normal_style_lcd_width, g_vdoply_cntx.normal_style_lcd_height);
					mmi_vdoply_display_set_coordinate();
					/*
					if(g_vdoply_cntx.video_info.width==320 && g_vdoply_cntx.video_info.height==240)
					{
						mdi_video_ply_fullscreen_display_set(240,180,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty);
					}
					else*/
					{
						mdi_video_ply_fullscreen_display_set(g_vdoply_cntx.video_info.width,g_vdoply_cntx.video_info.height,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty,0);
					}

					mmi_vdoply_player_enter_state(VDOPLY_STATE_IDLE);
					break;
					
				case VDOPLY_STATE_FULLSCR_PAUSE:
//					mmi_vdoply_draw_fullscr_osd();
//					mmi_vdoply_blt_screen();
					
					mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_IDLE);
					break;
					
				case VDOPLY_STATE_FULLSCR_PLAY:
//					mmi_vdoply_draw_fullscr_osd();
//					mmi_vdoply_blt_screen();
					
					/* if is seeking in play state, the video is already stopped */
					if(!g_vdoply_cntx.is_seeking)
						mdi_video_ply_stop();
					
					mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_IDLE);
					break;

			        case VDOPLY_STATE_FULLSCR_BT_CONNECTING:
         TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_rsk_release VDOPLY_STATE_FULLSCR_BT_CONNECTING");	
						
			     //       mmi_vdoply_fullscr_reset_osd_fade(TRUE);
			     //       mmi_vdoply_draw_fullscr_osd();
			     //       mmi_vdoply_blt_screen();
			            mmi_vdoply_disconnect_bt();
			            mmi_vdoply_player_enter_state(VDOPLY_STATE_PAUSE);
			            break;


					
			#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

		}
	#else

		/* touch screen */
		switch(g_vdoply_cntx.state)
		{
			case VDOPLY_STATE_EXIT:		
			case VDOPLY_STATE_OPENING:
			case VDOPLY_STATE_PAUSE:					
			case VDOPLY_STATE_IDLE:		
			case VDOPLY_STATE_PLAY:		
			case VDOPLY_STATE_INTERRUPTED_OPENING:	
			case VDOPLY_STATE_INTERRUPTED_SEEKING:
			case VDOPLY_STATE_INTERRUPTED_PAUSE:
				mmi_vdoply_draw_softkey();
				mmi_vdoply_blt_part();
				
				GoBackHistory();
				break;
					
			#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			
				/* full screen */
				case VDOPLY_STATE_FULLSCR_IDLE:
				case VDOPLY_STATE_FULLSCR_PAUSE:
				case VDOPLY_STATE_FULLSCR_PLAY:
					mmi_vdoply_toggle_fullscreen_display();
					break;
					
			#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

		}
		
	#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */


	if(g_vdoply_cntx.is_seeking)
		g_vdoply_cntx.is_seeking = FALSE;

	TBM_EXIT(0x2A6A);
		
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_vol_up_key_press
* DESCRIPTION
*  volume up key press hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_VOLUME__
static void mmi_vdoply_vol_up_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdoply_cntx.cur_status.volume <= LEVEL7)
	{
		g_vdoply_cntx.cur_status.volume++;
		mdi_audio_set_volume(MDI_VOLUME_VIDEO, g_vdoply_cntx.cur_status.volume);	

		g_vdoply_cntx.is_vol_inc_key_pressed = TRUE;		
	}
	else
	{
		return;
	}
	
	switch(g_vdoply_cntx.state)
	{
		/* normal */
		case VDOPLY_STATE_IDLE:
		case VDOPLY_STATE_PAUSE:
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
			mmi_vdoply_draw_panel_volume();
			mmi_vdoply_blt_part();
			break;
		case VDOPLY_STATE_PLAY:
		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			/* full screen */
			case VDOPLY_STATE_FULLSCR_IDLE:
			case VDOPLY_STATE_FULLSCR_PAUSE:
			case VDOPLY_STATE_FULLSCR_PLAY:
				//mmi_vdoply_fullscr_reset_osd_fade(TRUE);				
//				mmi_vdoply_draw_fullscr_osd();
//				mmi_vdoply_blt_screen();
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
				break;
		default:
			MMI_ASSERT(0);
	}
	
}
#endif /* __VDOPLY_FEATURE_VOLUME__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_vol_up_key_release
* DESCRIPTION
*  volume up key release hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_VOLUME__
static void mmi_vdoply_vol_up_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if(g_vdoply_cntx.is_vol_inc_key_pressed)
   {
   if(g_vdoply_cntx.state != VDOPLY_STATE_PLAY)
   	{
		g_vdoply_cntx.is_vol_inc_key_pressed = FALSE;
   	}
		
		
			switch(g_vdoply_cntx.state)
			{
				/* normal */
				case VDOPLY_STATE_IDLE:
				case VDOPLY_STATE_PAUSE:
				
				case VDOPLY_STATE_INTERRUPTED_PAUSE:
					mmi_vdoply_draw_panel_volume();
					mmi_vdoply_blt_part();
					break;
				case VDOPLY_STATE_PLAY:
				#ifdef __VDOPLY_FEATURE_FULLSCREEN__
					case VDOPLY_STATE_FULLSCR_IDLE:
					case VDOPLY_STATE_FULLSCR_PAUSE:
					case VDOPLY_STATE_FULLSCR_PLAY:
						//mmi_vdoply_fullscr_reset_osd_fade(TRUE);					
//						mmi_vdoply_draw_fullscr_osd();
//						mmi_vdoply_blt_screen();						
				#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
						break;
			}

			
   }
	
}
#endif /* __VDOPLY_FEATURE_VOLUME__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_vol_down_key_press
* DESCRIPTION
*  volume down key press hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_VOLUME__
static void mmi_vdoply_vol_down_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdoply_cntx.cur_status.volume > LEVEL1)
	{
		g_vdoply_cntx.cur_status.volume--;
		mdi_audio_set_volume(MDI_VOLUME_VIDEO, g_vdoply_cntx.cur_status.volume);	

		g_vdoply_cntx.is_vol_dec_key_pressed = TRUE;		
	}
	else
	{
		return;
	}	



	switch(g_vdoply_cntx.state)
	{
		/* normal */
		case VDOPLY_STATE_IDLE:
		case VDOPLY_STATE_PAUSE:
		
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
			mmi_vdoply_draw_panel_volume();
			mmi_vdoply_blt_part();
			break;
		case VDOPLY_STATE_PLAY:
		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			/* full screen */
			case VDOPLY_STATE_FULLSCR_IDLE:
			case VDOPLY_STATE_FULLSCR_PAUSE:
			case VDOPLY_STATE_FULLSCR_PLAY:
				//mmi_vdoply_fullscr_reset_osd_fade(TRUE);
//				mmi_vdoply_draw_fullscr_osd();
//				mmi_vdoply_blt_screen();
				
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
            break;
		default:
			MMI_ASSERT(0);
	}

}
#endif /* __VDOPLY_FEATURE_VOLUME__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_vol_down_key_release
* DESCRIPTION
*  volume down key release hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_VOLUME__
static void mmi_vdoply_vol_down_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if(g_vdoply_cntx.is_vol_dec_key_pressed)
   {
      if(g_vdoply_cntx.state != VDOPLY_STATE_PLAY)
      	{
		g_vdoply_cntx.is_vol_dec_key_pressed = FALSE;
      	}
		

		
			switch(g_vdoply_cntx.state)
			{
				/* normal */
				case VDOPLY_STATE_IDLE:
				case VDOPLY_STATE_PAUSE:
				
				case VDOPLY_STATE_INTERRUPTED_PAUSE:
					mmi_vdoply_draw_panel_volume();
					mmi_vdoply_blt_part();
					break;
				case VDOPLY_STATE_PLAY:
				#ifdef __VDOPLY_FEATURE_FULLSCREEN__
					case VDOPLY_STATE_FULLSCR_IDLE:
					case VDOPLY_STATE_FULLSCR_PAUSE:
					case VDOPLY_STATE_FULLSCR_PLAY:
						//mmi_vdoply_fullscr_reset_osd_fade(TRUE);					
//						mmi_vdoply_draw_fullscr_osd();
//						mmi_vdoply_blt_screen();						
				#endif /*  __VDOPLY_FEATURE_FULLSCREEN__ */
						break;
			}
			
			
			

   }
		
}
#endif /* __VDOPLY_FEATURE_VOLUME__ */




/*****************************************************************************
* FUNCTION
*	mmi_vdoply_speed_inc_key_press
* DESCRIPTION
*  up arrow key press hdlr, for speed up
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_SPEED__
static void mmi_vdoply_speed_inc_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	MDI_RESULT 	ret;
   BOOL			is_speed_change = FALSE;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdoply_cntx.cur_status.speed < VDOPLY_FEATURE_SPEED_COUNT-1)
	{
		g_vdoply_cntx.cur_status.speed++;
		is_speed_change = TRUE;
		
		g_vdoply_cntx.is_speed_inc_key_pressed = TRUE;		
	}
	else
	{
		return;
	}	
	
	switch(g_vdoply_cntx.state)
	{
		/* normal */
		case VDOPLY_STATE_IDLE:
		case VDOPLY_STATE_PAUSE:
		case VDOPLY_STATE_PLAY:
		case VDOPLY_STATE_SEEKING:
		case VDOPLY_STATE_PLAY_SEEKING:		
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
			mmi_vdoply_draw_panel_speed();
			break;

		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			/* full screen */
			case VDOPLY_STATE_FULLSCR_IDLE:
			case VDOPLY_STATE_FULLSCR_PAUSE:
			case VDOPLY_STATE_FULLSCR_PLAY:
			case VDOPLY_STATE_FULLSCR_SEEKING:
			case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:
				//mmi_vdoply_fullscr_reset_osd_fade(TRUE);					
				mmi_vdoply_draw_fullscr_osd();
				break;
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
		
		default:
			MMI_ASSERT(0);
	}

	mmi_vdoply_blt_screen();

	/* if is seeking, it is already stopped */
	if(is_speed_change)
	{
		if( (g_vdoply_cntx.state == VDOPLY_STATE_PLAY || 
			  g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY)&& 
			  (!g_vdoply_cntx.is_seeking))
		{
			/* speed change, need pause and restart */
			mdi_video_ply_stop();

			ret = mmi_vdoply_play_video();
				
			/* error check */
			if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
				return;
			
			/* reset start play time */
			g_vdoply_cntx.cur_status.start_play_time = g_vdoply_cntx.cur_status.play_time;
		}
	}
}
#endif /* __VDOPLY_FEATURE_SPEED__ */


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_speed_inc_key_release
* DESCRIPTION
*  speed up key press
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_SPEED__
static void mmi_vdoply_speed_inc_key_release(void)
{
	if(g_vdoply_cntx.is_speed_inc_key_pressed)
	{
		g_vdoply_cntx.is_speed_inc_key_pressed = FALSE;

		#if defined(__VDOPLY_FEATURE_TOUCH_SCREEN__)
		
			switch(g_vdoply_cntx.state)
			{
				/* normal */
				case VDOPLY_STATE_IDLE:
				case VDOPLY_STATE_PAUSE:
				case VDOPLY_STATE_PLAY:
				case VDOPLY_STATE_INTERRUPTED_PAUSE:
					mmi_vdoply_draw_panel_speed();
					break;

				#ifdef __VDOPLY_FEATURE_FULLSCREEN__
					case VDOPLY_STATE_FULLSCR_IDLE:
					case VDOPLY_STATE_FULLSCR_PAUSE:
					case VDOPLY_STATE_FULLSCR_PLAY:
						mmi_vdoply_fullscr_reset_osd_fade(TRUE);					
						mmi_vdoply_draw_fullscr_osd();
						break;
				#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
			}

		mmi_vdoply_blt_screen();
		#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
	}
	
}
#endif /* __VDOPLY_FEATURE_SPEED__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_speed_dec_key_press
* DESCRIPTION
*  down arrow key press hdlr, for speed down
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/	
#ifdef __VDOPLY_FEATURE_SPEED__
static void mmi_vdoply_speed_dec_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
	MDI_RESULT 	ret;
   BOOL			is_speed_change = FALSE;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdoply_cntx.cur_status.speed > 0)
	{
		g_vdoply_cntx.cur_status.speed--;
		is_speed_change = TRUE;
		
		g_vdoply_cntx.is_speed_dec_key_pressed = TRUE;		
	}
	else
	{
		return;
	}

	
	switch(g_vdoply_cntx.state)
	{
		/* normal */
		case VDOPLY_STATE_IDLE:
		case VDOPLY_STATE_PAUSE:
		case VDOPLY_STATE_PLAY:
		case VDOPLY_STATE_SEEKING:
		case VDOPLY_STATE_PLAY_SEEKING:
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
			mmi_vdoply_draw_panel_speed();
			break;

		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			/* full screen */
			case VDOPLY_STATE_FULLSCR_IDLE:
			case VDOPLY_STATE_FULLSCR_PAUSE:
			case VDOPLY_STATE_FULLSCR_PLAY:
			case VDOPLY_STATE_FULLSCR_SEEKING:
			case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:
				mmi_vdoply_fullscr_reset_osd_fade(TRUE);					
				mmi_vdoply_draw_fullscr_osd();
				break;
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
		
		default:
			MMI_ASSERT(0);
	}
	
	mmi_vdoply_blt_screen();

	if(is_speed_change)
	{
		/* speed change, need pause and restart */
		if( (g_vdoply_cntx.state == VDOPLY_STATE_PLAY || 
			  g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY)&& 
			  (!g_vdoply_cntx.is_seeking))
		{
			/* speed change, need pause and restart */
			mdi_video_ply_stop();

			ret = mmi_vdoply_play_video();


			/* error check */
			if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
				return;	
			
			/* reset start play time */
			g_vdoply_cntx.cur_status.start_play_time = g_vdoply_cntx.cur_status.play_time;
		}
	}
	
}
#endif


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_speed_inc_key_release
* DESCRIPTION
*  speed up key press
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_SPEED__
static void mmi_vdoply_speed_dec_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	if(g_vdoply_cntx.is_speed_dec_key_pressed)
	{
		g_vdoply_cntx.is_speed_dec_key_pressed = FALSE;

		#if defined(__VDOPLY_FEATURE_TOUCH_SCREEN__)	
			switch(g_vdoply_cntx.state)
			{
				/* normal */
				case VDOPLY_STATE_IDLE:
				case VDOPLY_STATE_PAUSE:
				case VDOPLY_STATE_PLAY:
				case VDOPLY_STATE_INTERRUPTED_PAUSE:
					mmi_vdoply_draw_panel_speed();
					break;

				#ifdef __VDOPLY_FEATURE_FULLSCREEN__					
					case VDOPLY_STATE_FULLSCR_IDLE:
					case VDOPLY_STATE_FULLSCR_PAUSE:
					case VDOPLY_STATE_FULLSCR_PLAY:
						mmi_vdoply_fullscr_reset_osd_fade(TRUE);					
						mmi_vdoply_draw_fullscr_osd();
						break;
				#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
			}

		mmi_vdoply_blt_screen();
		#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
	}

}
#endif /* __VDOPLY_FEATURE_SPEED__ */



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_result_hdlr
* DESCRIPTION
*  seek result
* PARAMETERS
*	result		IN		seek result
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/	
static void mmi_vdoply_seek_result_hdlr(MDI_RESULT result)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* error check */
	if(mmi_vdoply_check_and_display_error_popup(result)== TRUE) /* ture means has error popup */
			return;	

	/* seek partial done */
	if(result == MDI_RES_VDOPLY_SEEK_PARTIAL_DONE || result ==MDI_RES_VDOPLY_SUCCEED)
	{
//		/* get current play time */
		mdi_video_get_cur_play_time_block_req(mmi_vdoply_play_time_result_hdlr);

		/* play ratio scale is 1/width of progress */
		g_vdoply_cntx.cur_status.play_ratio = 	(U32)(g_vdoply_cntx.cur_status.play_time*	\
																	g_vdoply_cntx.progress.fill_width/	\
						 											g_vdoply_cntx.video_info.total_time_duration);
	}
   	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_result_hdlr result=%d",result);
	/* enter next state */
	TRACE_EF(g_sw_MED_Vid,"Func: %s g_vdoply_cntx.state=%d", __FUNCTION__,g_vdoply_cntx.state);
	switch(g_vdoply_cntx.state)
	{
		case VDOPLY_STATE_IDLE:
			if((gZoomHeight == VDOPLY_DISPLAY_SET_HEIGHT) &&  (gZoomWidth == VDOPLY_DISPLAY_SET_WIDTH)) //full screen
			{
				if(2 == is_lsk_pressed_In_Interrupted_state)
					{
						mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
						
					}
			}
			else
			{
				if(2 == is_lsk_pressed_In_Interrupted_state)
					{
						mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
						
					}
			}
			is_lsk_pressed_In_Interrupted_state = 0;
			break;
			
		case VDOPLY_STATE_SEEKING:
//			mmi_vdoply_blt_screen();
//			mmi_vdoply_blt_part();
			
			/* seek finish, back to pause state */
			mmi_vdoply_player_enter_state(VDOPLY_STATE_PAUSE);
			break;
			
		case VDOPLY_STATE_PLAY_SEEKING:
//			mmi_vdoply_blt_screen();
//			mmi_vdoply_blt_part();
  			TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_result_hdlr VDOPLY_STATE_PLAY_SEEKING");

			/* seek finish, back to play state */
//			mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);

	            if (mmi_vdoply_is_output_to_bt())
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_BT_CONNECTING);
	            }
	            else
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
	            }
	   			
			break;

		/* Full Screen */
		case VDOPLY_STATE_FULLSCR_SEEKING:
//			mmi_vdoply_blt_screen();
			
			/* seek finish, back to pause state */
			mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PAUSE);		
			break;

		case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:
//			mmi_vdoply_blt_screen();

			/* seek finish, back to play state */
//			mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
          TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_result_hdlr VDOPLY_STATE_FULLSCR_PLAY_SEEKING");
	            if (mmi_vdoply_is_output_to_bt())
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_BT_CONNECTING);
	            }
	            else
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
	            }

			break;

		case VDOPLY_STATE_INTERRUPTED_SEEKING:
//			mmi_vdoply_blt_screen();
//			mmi_vdoply_blt_part();

			/* seek finish, go to pause state */
//			mmi_vdoply_player_enter_state(VDOPLY_STATE_INTERRUPTED_PAUSE);

			if((gZoomHeight == VDOPLY_DISPLAY_SET_HEIGHT) &&  (gZoomWidth == VDOPLY_DISPLAY_SET_WIDTH)) //full screen
			{
				TRACE_EF(g_sw_MED_Vid,"Func: %s VDOPLY_STATE_INTERRUPTED_SEEKING fullscreen", __FUNCTION__);
				if(2 == is_lsk_pressed_In_Interrupted_state)
					{
						mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
						
					}
				else
					{
						mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PAUSE);
					}
			}
			else
			{
				TRACE_EF(g_sw_MED_Vid,"Func: %s VDOPLY_STATE_INTERRUPTED_SEEKING not full screen", __FUNCTION__);
				if(2 == is_lsk_pressed_In_Interrupted_state)
					{
						mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
						
					}
				else
					{
						mmi_vdoply_player_enter_state(VDOPLY_STATE_INTERRUPTED_PAUSE);
					}
			}
			is_lsk_pressed_In_Interrupted_state = 0;
			break;
			
	}
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_key_release
* DESCRIPTION
*  seek key release hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/	
static void mmi_vdoply_seek_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_key_release g_vdoply_cntx.is_seeking = %d,playtime=%d",g_vdoply_cntx.is_seeking, g_vdoply_cntx.cur_status.play_time);
	g_vdoply_cntx.is_seeking = FALSE;
	switch(g_vdoply_cntx.state)
	{
		/* normal */
		case VDOPLY_STATE_IDLE:
		case VDOPLY_STATE_PAUSE:
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
			mmi_vdoply_player_enter_state(VDOPLY_STATE_SEEKING);
			break;
		case VDOPLY_STATE_BT_CONNECTING: 	
		case VDOPLY_STATE_PLAY:

			mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY_SEEKING);
			break;
		
		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			/* full screen */
			case VDOPLY_STATE_FULLSCR_IDLE:
			case VDOPLY_STATE_FULLSCR_PAUSE:

				mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_SEEKING);
				break;
				
			case VDOPLY_STATE_FULLSCR_PLAY:
				mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY_SEEKING);
				break;
		#endif 

		default:
			MMI_ASSERT(0);
	}
	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_key_release 4545454545 g_vdoply_cntx.is_seeking = %d",g_vdoply_cntx.is_seeking);

}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_foward
* DESCRIPTION
*  seek foward
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/	
static BOOL mmi_vdoply_seek_foward(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32 	jump_time = 0;
	U32 	total_time = 0;
	U32 	cur_time = 0;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/ 	
	cur_time 	= g_vdoply_cntx.cur_status.play_time;
	total_time 	= g_vdoply_cntx.video_info.total_time_duration;
	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_foward cur_time=%d,total_time=%d,fill_width=%d",cur_time,total_time,g_vdoply_cntx.progress.fill_width);	
	if(cur_time < total_time)
	{
		jump_time = total_time/g_vdoply_cntx.progress.fill_width;

		if(jump_time == 0)
			jump_time = 1;

		jump_time <<= 3; 
		if( (jump_time + cur_time) >= total_time)		/* time stamp is from 0 to total-1 */
		{
			cur_time = total_time -1;
		}
		else
			cur_time	+=jump_time;

		switch(g_vdoply_cntx.state)
		{
			/* normal */
			case VDOPLY_STATE_IDLE:
				/* change to pause state */
				g_vdoply_cntx.state = VDOPLY_STATE_PAUSE;
				
			case VDOPLY_STATE_PAUSE:
			case VDOPLY_STATE_PLAY:
			case VDOPLY_STATE_INTERRUPTED_PAUSE:
			case VDOPLY_STATE_SEEKING:
			case VDOPLY_STATE_PLAY_SEEKING:				
				/* play ratio scale is 1/width of progress */				
				g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.progress.fill_width/total_time);
				g_vdoply_cntx.cur_status.play_time  = cur_time;
				mmi_vdoply_draw_panel_timer();
				mmi_vdoply_draw_panel_progress();
				#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				gdi_image_draw_id(g_vdoply_osd_cntx.touch_progress_inc.offset_x,
							g_vdoply_osd_cntx.touch_progress_inc.offset_y,
							IMG_ID_VDOPLY_TOUCH_OSD_PROGRESS_INC_SEL);	
				#endif

				mmi_vdoply_blt_part();
				break;

		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			/* full screen */
			case VDOPLY_STATE_FULLSCR_IDLE:
				/* change to pause state */
				g_vdoply_cntx.state = VDOPLY_STATE_FULLSCR_PAUSE;	
				
			case VDOPLY_STATE_FULLSCR_PAUSE:
			case VDOPLY_STATE_FULLSCR_PLAY:
			case VDOPLY_STATE_FULLSCR_SEEKING:
			case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:
				/* play ratio scale is 1/width of progress */
				g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.fullscr_progress.fill_width/total_time);
				g_vdoply_cntx.cur_status.play_time  = cur_time;

//				mmi_vdoply_fullscr_reset_osd_fade(TRUE);
//				mmi_vdoply_draw_fullscr_osd();
//				mmi_vdoply_blt_screen();
				break;
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
		
			default:
				MMI_ASSERT(0);
		}

		
		

		return TRUE;
	}

	return FALSE; /* reach limit, cant seek anymore */
	
}


#if 0
/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_foward_key_press_cyclic
* DESCRIPTION
*  right arrow long press timer hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_seek_foward_key_press_cyclic(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(mmi_vdoply_seek_foward())
		coolsand_UI_start_timer(100, mmi_vdoply_seek_foward_key_press_cyclic);
}
#endif



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_foward_key_press
* DESCRIPTION
*  right arrow press hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/


static void mmi_vdoply_seek_foward_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_foward_key_press g_vdoply_cntx.is_seeking=%d,g_vdoply_cntx.is_seek_fwd_key_pressed=%d g_vdoply_cntx.cur_status.play_time=%d",g_vdoply_cntx.is_seeking,g_vdoply_cntx.is_seek_fwd_key_pressed,g_vdoply_cntx.cur_status.play_time);
	TRACE_EF(g_sw_MED_Vid,"Func: %s g_vdoply_cntx.state=%d", __FUNCTION__,g_vdoply_cntx.state);
	if(g_vdoply_cntx.is_seeking)
	{
		coolsand_UI_cancel_timer(mmi_vdoply_seek_key_release);
		g_vdoply_cntx.is_seeking = FALSE;
	}
   
	if(!g_vdoply_cntx.is_seek_fwd_key_pressed)
	{
		g_vdoply_cntx.is_seek_fwd_key_pressed 	= TRUE;

		switch(g_vdoply_cntx.state)
		{
			case VDOPLY_STATE_PLAY:
//				if(!g_vdoply_cntx.is_seeking)
				if(b_video_playing)
				{
					mdi_video_ply_stop();
					coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);

				}
				mmi_vdoply_draw_panel_progress();
//				mmi_vdoply_blt_screen();
				mmi_vdoply_blt_part();

				break;
		        case VDOPLY_STATE_BT_CONNECTING:
		            /* hide process layer */
		            
		            mmi_vdoply_disconnect_bt();
		            mmi_vdoply_draw_panel_progress();
		            mmi_vdoply_blt_part();
		            break;

				
			case VDOPLY_STATE_FULLSCR_PLAY:
				#ifdef __VDOPLY_FEATURE_FULLSCREEN__
					if(!g_vdoply_cntx.is_seeking)
					{
						mdi_video_ply_stop();				
						coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
					}

//					mmi_vdoply_draw_fullscr_panel_progress();
//					mmi_vdoply_blt_screen();
				#endif
				break;

			case VDOPLY_STATE_IDLE:
				mmi_vdoply_draw_panel_progress();
//				mmi_vdoply_blt_screen();
				mmi_vdoply_blt_part();
				g_vdoply_cntx.state = VDOPLY_STATE_PAUSE;
				break;

			case VDOPLY_STATE_SEEKING:
			case VDOPLY_STATE_PLAY_SEEKING:	
				/* stop previous seeking action */
				mdi_video_ply_stop_non_block_seek();
				
				mmi_vdoply_draw_panel_progress();
//				mmi_vdoply_blt_screen();
				mmi_vdoply_blt_part();
				break;
			case VDOPLY_STATE_FULLSCR_SEEKING:
			case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:	
				#ifdef __VDOPLY_FEATURE_FULLSCREEN__
					/* stop previous seeking action */
					mdi_video_ply_stop_non_block_seek();
				
//					mmi_vdoply_draw_fullscr_panel_progress();
//					mmi_vdoply_blt_screen();
				#endif /*__VDOPLY_FEATURE_FULLSCREEN__*/
				break;

		}

		g_vdoply_cntx.is_seeking = TRUE;
		TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_foward_key_press enter seek");
		if(mmi_vdoply_seek_foward())
		{
	//		coolsand_UI_start_timer(200, mmi_vdoply_seek_foward_key_press_cyclic);
		}
	}
}




/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_foward_key_press
* DESCRIPTION
*  right arrow press hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/


static void mmi_vdoply_seek_foward_key_long_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_vdoply_cntx.is_seeking)
	{
		coolsand_UI_cancel_timer(mmi_vdoply_seek_foward_key_long_press);
	}
   
	if(g_vdoply_cntx.is_seek_fwd_key_pressed)
	{
		mmi_vdoply_seek_foward();
	}

	coolsand_UI_start_timer(100, mmi_vdoply_seek_foward_key_long_press);
}


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_foward_key_release
* DESCRIPTION
*  right arrow release hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_seek_foward_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_foward_key_release g_vdoply_cntx.is_seeking=%d,g_vdoply_cntx.is_seek_fwd_key_pressed=%d",g_vdoply_cntx.is_seeking,g_vdoply_cntx.is_seek_fwd_key_pressed);
   
	if(!g_vdoply_cntx.is_seek_fwd_key_pressed)
		return;
	
	g_vdoply_cntx.is_seek_fwd_key_pressed = FALSE;
if(g_vdoply_cntx.is_seeking)
	{
		coolsand_UI_cancel_timer(mmi_vdoply_seek_foward_key_long_press);
	}
	switch(g_vdoply_cntx.state)
	{
		case VDOPLY_STATE_IDLE:
		case VDOPLY_STATE_PAUSE:
		case VDOPLY_STATE_PLAY:
		case VDOPLY_STATE_SEEKING:
		case VDOPLY_STATE_PLAY_SEEKING:
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
		case VDOPLY_STATE_INTERRUPTED_SEEKING:
			mmi_vdoply_draw_panel_progress();
			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
			gdi_image_draw_id(g_vdoply_osd_cntx.touch_progress_inc.offset_x,
									g_vdoply_osd_cntx.touch_progress_inc.offset_y,
									IMG_ID_VDOPLY_TOUCH_OSD_PROGRESS_INC);
			#endif
//			mmi_vdoply_blt_screen();
			mmi_vdoply_blt_part();

			break;
		case VDOPLY_STATE_FULLSCR_IDLE:
		case VDOPLY_STATE_FULLSCR_PAUSE:			
		case VDOPLY_STATE_FULLSCR_PLAY:		
		case VDOPLY_STATE_FULLSCR_SEEKING:
		case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:
 	#ifdef __VDOPLY_FEATURE_FULLSCREEN__
 //			mmi_vdoply_draw_fullscr_panel_progress();
 	#endif /*__VDOPLY_FEATURE_FULLSCREEN__*/
//			mmi_vdoply_blt_screen();
			break;
	}

//	coolsand_UI_cancel_timer(mmi_vdoply_seek_foward_key_press_cyclic);
	coolsand_UI_start_timer(200, mmi_vdoply_seek_key_release);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_backward
* DESCRIPTION
*  seek backward
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static BOOL mmi_vdoply_seek_backward(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32 			jump_time;
	U32			total_time;
	U32 			cur_time;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	cur_time 	= g_vdoply_cntx.cur_status.play_time;
	total_time 	= g_vdoply_cntx.video_info.total_time_duration;
	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_backward g_vdoply_cntx.video_info.total_time_duration=%d, g_vdoply_cntx.cur_status.play_time= %d",g_vdoply_cntx.video_info.total_time_duration,g_vdoply_cntx.cur_status.play_time);

		
	if(cur_time > 0)
	{
		jump_time = total_time/g_vdoply_cntx.progress.fill_width;

		if(jump_time == 0)
			jump_time = 1;

		jump_time <<= 3; 

		if( jump_time > cur_time )
			cur_time = 0;
		else
			cur_time	-= jump_time;

		/* play ratio scale is 1/width of progress */


		switch(g_vdoply_cntx.state)
		{
			/* normal */
			case VDOPLY_STATE_IDLE:
				/* change to pause state */
				g_vdoply_cntx.state = VDOPLY_STATE_PAUSE;
					
			case VDOPLY_STATE_PAUSE:
			case VDOPLY_STATE_PLAY:
			case VDOPLY_STATE_INTERRUPTED_PAUSE:
			case VDOPLY_STATE_SEEKING:
			case VDOPLY_STATE_PLAY_SEEKING:	
				/* play ratio scale is 1/width of progress */				
				g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.progress.fill_width/total_time);
				g_vdoply_cntx.cur_status.play_time  = cur_time;

				mmi_vdoply_draw_panel_timer();
				mmi_vdoply_draw_panel_progress();
				#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				gdi_image_draw_id(g_vdoply_osd_cntx.touch_progress_dec.offset_x,
									g_vdoply_osd_cntx.touch_progress_dec.offset_y,
									IMG_ID_VDOPLY_TOUCH_OSD_PROGRESS_DEC_SEL);
				#endif

				mmi_vdoply_blt_part();
				break;
				
			#ifdef __VDOPLY_FEATURE_FULLSCREEN__
				/* full screen */
				case VDOPLY_STATE_FULLSCR_IDLE:
					/* change to pause state */
					g_vdoply_cntx.state = VDOPLY_STATE_FULLSCR_PAUSE;
					
				case VDOPLY_STATE_FULLSCR_PAUSE:
				case VDOPLY_STATE_FULLSCR_PLAY:
				case VDOPLY_STATE_FULLSCR_SEEKING:
				case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:										
					/* play ratio scale is 1/width of progress */				
					g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.fullscr_progress.fill_width/total_time);
					g_vdoply_cntx.cur_status.play_time  = cur_time;

//					mmi_vdoply_fullscr_reset_osd_fade(TRUE);
//					mmi_vdoply_draw_fullscr_osd();
//					mmi_vdoply_blt_screen();
					break;
			#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */	

			default:
				MMI_ASSERT(0);
		}

		

		return TRUE;
	}

	return FALSE; /* reach limit, cant seek anymore */
	
}
	

#if 0
/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_backward_key_press_cyclic
* DESCRIPTION
*  left arrow long press timer hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_seek_backward_key_press_cyclic(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(mmi_vdoply_seek_backward()) 
		coolsand_UI_start_timer(100, mmi_vdoply_seek_backward_key_press_cyclic);
	
}
#endif


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_backward_key_press
* DESCRIPTION
*  left arrow press hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/	
static void mmi_vdoply_seek_backward_key_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_backward_key_press g_vdoply_cntx.is_seeking=%d,g_vdoply_cntx.is_seek_bwd_key_pressed=%d",g_vdoply_cntx.is_seeking,g_vdoply_cntx.is_seek_bwd_key_pressed);

	if(g_vdoply_cntx.is_seeking)
   	{
		coolsand_UI_cancel_timer(mmi_vdoply_seek_key_release);
		g_vdoply_cntx.is_seeking = FALSE;
   	}
   
	if(!g_vdoply_cntx.is_seek_bwd_key_pressed)
	{
		g_vdoply_cntx.is_seek_bwd_key_pressed 	= TRUE;

		switch(g_vdoply_cntx.state)
		{
			case VDOPLY_STATE_PLAY:
//				if(!g_vdoply_cntx.is_seeking)
				if(b_video_playing)
				{
					mdi_video_ply_stop();
					coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
				}
				mmi_vdoply_draw_panel_progress();
//				mmi_vdoply_blt_screen();	
				mmi_vdoply_blt_part();

				break;

		        case VDOPLY_STATE_BT_CONNECTING:
		            /* hide process layer */
		   //         mmi_vdoply_set_state_layer_config(VDOPLY_STATE_SEEKING);
		            
		            mmi_vdoply_disconnect_bt();
		            mmi_vdoply_draw_panel_progress();
		            mmi_vdoply_blt_part();
		            break;

				
			case VDOPLY_STATE_FULLSCR_PLAY:
				#ifdef __VDOPLY_FEATURE_FULLSCREEN__
					if(!g_vdoply_cntx.is_seeking)
					{	
						mdi_video_ply_stop();
						coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
					}
					
//					mmi_vdoply_draw_fullscr_panel_progress();
//					mmi_vdoply_blt_screen();	
				#endif /*__VDOPLY_FEATURE_FULLSCREEN__*/

				break;

			case VDOPLY_STATE_SEEKING:
			case VDOPLY_STATE_PLAY_SEEKING:		
				/* stop previous seeking action */

				mdi_video_ply_stop_non_block_seek();

				mmi_vdoply_draw_panel_progress();
//				mmi_vdoply_blt_screen();
				mmi_vdoply_blt_part();
				break;

				
			case VDOPLY_STATE_FULLSCR_SEEKING:
			case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:	
				#ifdef __VDOPLY_FEATURE_FULLSCREEN__
					/* stop previous seeking action */
					mdi_video_ply_stop_non_block_seek();

//					mmi_vdoply_draw_fullscr_panel_progress();
//					mmi_vdoply_blt_screen();
				#endif /*__VDOPLY_FEATURE_FULLSCREEN__*/
				
				break;
		}

		g_vdoply_cntx.is_seeking = TRUE;
	
		if(mmi_vdoply_seek_backward()) 
		{
	//		coolsand_UI_start_timer(200, mmi_vdoply_seek_backward_key_press_cyclic);
		}
	}
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_backward_key_long_press
* DESCRIPTION
*  left arrow press hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/	
static void mmi_vdoply_seek_backward_key_long_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if(g_vdoply_cntx.is_seeking)
   	{
		coolsand_UI_cancel_timer(mmi_vdoply_seek_backward_key_long_press);
   	}
   
	if(g_vdoply_cntx.is_seek_bwd_key_pressed)
	{
		mmi_vdoply_seek_backward();
	}

	coolsand_UI_start_timer(100, mmi_vdoply_seek_backward_key_long_press);
}


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_seek_backward_key_release
* DESCRIPTION
*  left arrow release hdlr
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/	
static void mmi_vdoply_seek_backward_key_release(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_seek_backward_key_release g_vdoply_cntx.is_seeking=%d,g_vdoply_cntx.is_seek_bwd_key_pressed=%d",g_vdoply_cntx.is_seeking,g_vdoply_cntx.is_seek_bwd_key_pressed);

	if(!g_vdoply_cntx.is_seek_bwd_key_pressed)
		return;
	
	g_vdoply_cntx.is_seek_bwd_key_pressed = FALSE;
if(g_vdoply_cntx.is_seeking)
   	{
		coolsand_UI_cancel_timer(mmi_vdoply_seek_backward_key_long_press);
   	}
	switch(g_vdoply_cntx.state)
	{
		case VDOPLY_STATE_IDLE:
		case VDOPLY_STATE_PAUSE:
		case VDOPLY_STATE_PLAY:
		case VDOPLY_STATE_SEEKING:
		case VDOPLY_STATE_PLAY_SEEKING:
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
		case VDOPLY_STATE_INTERRUPTED_SEEKING:
			mmi_vdoply_draw_panel_progress();
			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
			gdi_image_draw_id(g_vdoply_osd_cntx.touch_progress_dec.offset_x,
									g_vdoply_osd_cntx.touch_progress_dec.offset_y,
									IMG_ID_VDOPLY_TOUCH_OSD_PROGRESS_DEC);		
			#endif
//			mmi_vdoply_blt_screen();
			mmi_vdoply_blt_part();
			break;
		case VDOPLY_STATE_FULLSCR_IDLE:
		case VDOPLY_STATE_FULLSCR_PAUSE:			
		case VDOPLY_STATE_FULLSCR_PLAY:		
		case VDOPLY_STATE_FULLSCR_SEEKING:
		case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:
	#ifdef __VDOPLY_FEATURE_FULLSCREEN__
//			mmi_vdoply_draw_fullscr_osd();
	#endif/* __VDOPLY_FEATURE_FULLSCREEN__ */


//			mmi_vdoply_blt_screen();
			break;
	}

//	coolsand_UI_cancel_timer(mmi_vdoply_seek_backward_key_press_cyclic);
	coolsand_UI_start_timer(200, mmi_vdoply_seek_key_release);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_lsk_press
* DESCRIPTION
*  lsk hdlr when pressed
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_lsk_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_vdoply_cntx.is_lsk_pressed = TRUE;	

//	coolsand_UI_cancel_timer(mmi_vdoply_lsk_release);

	switch(g_vdoply_cntx.state)
	{
		/* normal */
		case VDOPLY_STATE_IDLE:
			g_vdoply_cntx.cur_status.start_play_time	= 0;
			g_vdoply_cntx.cur_status.play_time 			= 0;	
			g_vdoply_cntx.cur_status.play_ratio 		= 0;	
		case VDOPLY_STATE_PAUSE:
		case VDOPLY_STATE_PLAY:
		case VDOPLY_STATE_OPENING:				
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
		case VDOPLY_STATE_INTERRUPTED_OPENING:	
		case VDOPLY_STATE_INTERRUPTED_SEEKING:
		case VDOPLY_STATE_SEEKING:
		case VDOPLY_STATE_PLAY_SEEKING:	
              case VDOPLY_STATE_BT_CONNECTING:
		if(!((gZoomHeight == VDOPLY_DISPLAY_SET_HEIGHT) &&  (gZoomWidth == VDOPLY_DISPLAY_SET_WIDTH)))
			{
			#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				g_vdoply_cntx.touch_play.is_press = TRUE;
			#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
			if(g_vdoply_cntx.state != VDOPLY_STATE_PLAY)
			{
				mmi_vdoply_draw_softkey();
				mmi_vdoply_draw_panel_state();
				mmi_vdoply_blt_part();
				}
			
			}
			
			break;
		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			/* full screen */
			case VDOPLY_STATE_FULLSCR_IDLE:
			case VDOPLY_STATE_FULLSCR_PAUSE:
			case VDOPLY_STATE_FULLSCR_PLAY:
			case VDOPLY_STATE_FULLSCR_SEEKING:
			case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:
			case VDOPLY_STATE_FULLSCR_BT_CONNECTING:	
				/* reset osd's opacity */
//				mmi_vdoply_fullscr_reset_osd_fade(TRUE);
								
//				mmi_vdoply_draw_fullscr_osd();
//				mmi_vdoply_blt_screen();
				break;
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

		default:
			MMI_ASSERT(0);
	}
	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_rsk_press
* DESCRIPTION
*  rsk hdlr when pressed
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_rsk_press(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	g_vdoply_cntx.is_rsk_pressed = TRUE;	
	switch(g_vdoply_cntx.state)
	{
		/* normal */
		case VDOPLY_STATE_IDLE:
		case VDOPLY_STATE_PAUSE:
		case VDOPLY_STATE_PLAY:
		case VDOPLY_STATE_OPENING:				
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
		case VDOPLY_STATE_INTERRUPTED_OPENING:
		case VDOPLY_STATE_INTERRUPTED_SEEKING:
		case VDOPLY_STATE_SEEKING:
		case VDOPLY_STATE_PLAY_SEEKING:		
			if(g_vdoply_cntx.state != VDOPLY_STATE_PLAY)
			{
			mmi_vdoply_draw_softkey();
			mmi_vdoply_draw_panel_state();
			mmi_vdoply_blt_part();
				}
			break;
		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			/* full screen */
			case VDOPLY_STATE_FULLSCR_IDLE:
			case VDOPLY_STATE_FULLSCR_PAUSE:
			case VDOPLY_STATE_FULLSCR_PLAY:
			case VDOPLY_STATE_FULLSCR_SEEKING:
			case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:	
				/* reset osd's opacity */
//				mmi_vdoply_fullscr_reset_osd_fade(TRUE);
				
//				mmi_vdoply_draw_fullscr_osd();
//				mmi_vdoply_blt_screen();
				break;
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

		default:
			MMI_ASSERT(0);
	}
	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_toggle_fullscreen_display
* DESCRIPTION
*  toggle to fullscreen display
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
static void mmi_vdoply_display_set_coordinate()
{
	switch(g_vdoply_cntx.state)
	{

		case VDOPLY_STATE_IDLE:
		case VDOPLY_STATE_PLAY:	
		case VDOPLY_STATE_PAUSE:	
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
		case VDOPLY_STATE_SEEKING:
		case VDOPLY_STATE_PLAY_SEEKING:
			g_vdoply_cntx.video_info.startx = 0;
			g_vdoply_cntx.video_info.starty = 0;
			break;
		case VDOPLY_STATE_FULLSCR_IDLE:
		case VDOPLY_STATE_FULLSCR_PLAY:	
		case VDOPLY_STATE_FULLSCR_PAUSE:	
		case VDOPLY_STATE_FULLSCR_SEEKING:	
		case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:
			/*
			if(g_vdoply_cntx.video_info.width==320 && g_vdoply_cntx.video_info.height==240)
			{
				g_vdoply_cntx.video_info.startx = g_vdoply_osd_cntx.play_wnd.rect.offset_x +(g_vdoply_osd_cntx.play_wnd.rect.width - 240) / 2;
				g_vdoply_cntx.video_info.starty = g_vdoply_osd_cntx.play_wnd.rect.offset_y +(g_vdoply_osd_cntx.play_wnd.rect.height - 180) / 2;
			}
			else*/
			{
				g_vdoply_cntx.video_info.startx = g_vdoply_osd_cntx.play_wnd.rect.offset_x +(g_vdoply_osd_cntx.play_wnd.rect.width - g_vdoply_cntx.video_info.width) / 2;
				g_vdoply_cntx.video_info.starty = g_vdoply_osd_cntx.play_wnd.rect.offset_y +(g_vdoply_osd_cntx.play_wnd.rect.height - g_vdoply_cntx.video_info.height) / 2;

			}
			break;

	}

}
static void mmi_vdoply_toggle_fullscreen_display(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* flush key event */
	ClearKeyEvents();
   	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_toggle_fullscreen_display");
   /* if is seeking, do not allowed toggle */
	if(g_vdoply_cntx.is_seeking)
		return;
	mmi_vdoply_display_set_coordinate();
	switch(g_vdoply_cntx.state)
	{
		/* normal */
		case VDOPLY_STATE_PLAY:
			mdi_video_ply_pause();
			TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_toggle_fullscreen_display VDOPLY_STATE_PLAY");
			COS_Sleep(400);
#ifdef __MMI_MAINLCD_220X176__
			mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_HEIGHT,VDOPLY_DISPLAY_SET_WIDTH,0,0,0);
#else
			mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_WIDTH,VDOPLY_DISPLAY_SET_HEIGHT,0,0,1);
#endif
	#if 0		
			mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																					NULL);	
	#endif
//			mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);

	                if (mmi_vdoply_is_output_to_bt())
	                {
	                    mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_BT_CONNECTING);
	                }
	                else
	                {
	                    mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
	                }
	
			break;

	        case VDOPLY_STATE_BT_CONNECTING:
          TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_toggle_fullscreen_display VDOPLY_STATE_BT_CONNECTING");
				
	            mmi_vdoply_disconnect_bt();

	            if (mmi_vdoply_is_output_to_bt())
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_BT_CONNECTING);
	            }
	            else
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
	            }
	            break;


		case VDOPLY_STATE_IDLE:
#ifdef __MMI_MAINLCD_220X176__
			mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_HEIGHT,VDOPLY_DISPLAY_SET_WIDTH,0,0,0);
#else
			mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_WIDTH,VDOPLY_DISPLAY_SET_HEIGHT,0,0,1);
#endif
			mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_IDLE);
			break;
			
		case VDOPLY_STATE_PAUSE:
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
#ifdef __MMI_MAINLCD_220X176__
			mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_HEIGHT,VDOPLY_DISPLAY_SET_WIDTH,0,0,0);
#else
			mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_WIDTH,VDOPLY_DISPLAY_SET_HEIGHT,0,0,1);
#endif
			mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PAUSE);
			mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																					NULL);	
			break;

		case VDOPLY_STATE_SEEKING:	
			/* stop non-blocking seeking */
			mdi_video_ply_stop_non_block_seek();
#ifdef __MMI_MAINLCD_220X176__
			mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_HEIGHT,VDOPLY_DISPLAY_SET_WIDTH,0,0,0);
#else
			mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_WIDTH,VDOPLY_DISPLAY_SET_HEIGHT,0,0,1);
#endif			

			 mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																					NULL);	

			mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PAUSE);
			break;
		
		case VDOPLY_STATE_PLAY_SEEKING:
			/* stop non-blocking seeking */
			mdi_video_ply_stop_non_block_seek();
#ifdef __MMI_MAINLCD_220X176__
			mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_HEIGHT,VDOPLY_DISPLAY_SET_WIDTH,0,0,0);
#else
			mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_WIDTH,VDOPLY_DISPLAY_SET_HEIGHT,0,0,1);
#endif			
			mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																					NULL);	

               TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_toggle_fullscreen_display VDOPLY_STATE_PLAY_SEEKING");
//			mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
		          if (mmi_vdoply_is_output_to_bt())
		            {
		                mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_BT_CONNECTING);
		            }
		            else
		            {
		                mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
		            }

			
			break;

		/* fullscreen */
		case VDOPLY_STATE_FULLSCR_PAUSE:
			coolsand_UI_cancel_timer(mmi_vdoply_fullscr_osd_fade_cyclic);
			/*
			if(g_vdoply_cntx.video_info.width==320 && g_vdoply_cntx.video_info.height==240)
			{
				mdi_video_ply_fullscreen_display_set(240,180,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty);
			}
			else*/
			{
				mdi_video_ply_fullscreen_display_set(g_vdoply_cntx.video_info.width,g_vdoply_cntx.video_info.height,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty,0);
			}
		//	mmi_vdoply_blt_screen();
			mmi_vdoply_player_enter_state(VDOPLY_STATE_PAUSE);
			mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																					NULL);	
			break;
	        case VDOPLY_STATE_FULLSCR_BT_CONNECTING:
				
                 TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_toggle_fullscreen_display VDOPLY_STATE_FULLSCR_BT_CONNECTING!!!");
		
	            mmi_vdoply_disconnect_bt();
	            gui_cancel_timer(mmi_vdoply_fullscr_osd_fade_cyclic);
	            
	        #ifdef __VDOPLY_FEATURE_HIDE_FULLSCR_OSD__
	            /* no osd - back to veritcal screen and pause */
	            mmi_vdoply_enter_state(VDOPLY_STATE_PAUSE);
	        #else            
	            /* have osd */          
	            if (mmi_vdoply_is_output_to_bt())
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_BT_CONNECTING);
	            }
	            else
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
	            }
	        #endif
	            break;

			
		case VDOPLY_STATE_FULLSCR_PLAY:
			mdi_video_ply_pause();
			COS_Sleep(400);
			coolsand_UI_cancel_timer(mmi_vdoply_fullscr_osd_fade_cyclic);	
			/*
			if(g_vdoply_cntx.video_info.width==320 && g_vdoply_cntx.video_info.height==240)
			{
				mdi_video_ply_fullscreen_display_set(240,180,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty);
			}
			else*/
			{
				mdi_video_ply_fullscreen_display_set(g_vdoply_cntx.video_info.width,g_vdoply_cntx.video_info.height,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty,0);
			}
		//	mmi_vdoply_blt_screen();
		#if 0
			mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																					NULL);	
		#endif
//			mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);

	                if (mmi_vdoply_is_output_to_bt())
	                {
	                    mmi_vdoply_player_enter_state(VDOPLY_STATE_BT_CONNECTING);
	                }
	                else
	                {
	                    mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
	                }

		
			break;

		case VDOPLY_STATE_FULLSCR_IDLE:
			
			coolsand_UI_cancel_timer(mmi_vdoply_fullscr_osd_fade_cyclic);	
			/*
			if(g_vdoply_cntx.video_info.width==320 && g_vdoply_cntx.video_info.height==240)
			{
				mdi_video_ply_fullscreen_display_set(240,180,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty);
			}
			else*/
			{
				mdi_video_ply_fullscreen_display_set(g_vdoply_cntx.video_info.width,g_vdoply_cntx.video_info.height,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty,0);
			}

			mmi_vdoply_player_enter_state(VDOPLY_STATE_IDLE);
			break;

		case VDOPLY_STATE_FULLSCR_SEEKING:	
			/* stop non-blocking seeking */
			mdi_video_ply_stop_non_block_seek();
			
			coolsand_UI_cancel_timer(mmi_vdoply_fullscr_osd_fade_cyclic);
			/*
			if(g_vdoply_cntx.video_info.width==320 && g_vdoply_cntx.video_info.height==240)
			{
				mdi_video_ply_fullscreen_display_set(240,180,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty);
			}
			else*/
			{
				mdi_video_ply_fullscreen_display_set(g_vdoply_cntx.video_info.width,g_vdoply_cntx.video_info.height,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty,0);
			}

		//	mmi_vdoply_blt_screen();
			mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																					NULL);	

			mmi_vdoply_player_enter_state(VDOPLY_STATE_PAUSE);
			break;

		case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:
			/* stop non-blocking seeking */
			mdi_video_ply_stop_non_block_seek();
			coolsand_UI_cancel_timer(mmi_vdoply_fullscr_osd_fade_cyclic);
			/*
			if(g_vdoply_cntx.video_info.width==320 && g_vdoply_cntx.video_info.height==240)
			{
				mdi_video_ply_fullscreen_display_set(240,180,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty);
			}
			else*/
			{
				mdi_video_ply_fullscreen_display_set(g_vdoply_cntx.video_info.width,g_vdoply_cntx.video_info.height,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty,0);
			}
		//	mmi_vdoply_blt_screen();
			mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																					NULL);	

	//		mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);

	            if (mmi_vdoply_is_output_to_bt())
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_BT_CONNECTING);
	            }
	            else
	            {
	                mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
	            }


			
			break;

		default:
			MMI_ASSERT(0);
			
	}

}
#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */


static void mmi_vdoply_lsk_press_In_Interrupted_state(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	is_lsk_pressed_In_Interrupted_state = 1;	

}
static void mmi_vdoply_lsk_release_In_Interrupted_state(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if(1 == is_lsk_pressed_In_Interrupted_state)
	is_lsk_pressed_In_Interrupted_state = 2;	

}
/*****************************************************************************
* FUNCTION
*	mmi_vdoply_register_key_event
* DESCRIPTION
*  register player's key event hdlr for different state 
* PARAMETERS
*	state	 IN		video player's state
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/	

static void mmi_vdoply_register_key_event(U16 state)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	TBM_ENTRY(0x2A63);
	switch(state)
	{
		case VDOPLY_STATE_IDLE:
		case VDOPLY_STATE_PLAY:	
		case VDOPLY_STATE_PAUSE:	
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
			ClearAllKeyHandler();
			
			SetKeyHandler(mmi_vdoply_lsk_press, 	KEY_LSK, KEY_EVENT_DOWN);	
//			SetKeyHandler(mmi_vdoply_lsk_release, 	KEY_LSK, KEY_EVENT_DOWN);	
			SetKeyHandler(mmi_vdoply_lsk_release, 	KEY_LSK, KEY_EVENT_UP);		
//			SetKeyHandler(mmi_vdoply_lsk_release_timer, 	KEY_LSK, KEY_EVENT_UP);		

			SetKeyHandler(mmi_vdoply_rsk_press, 	KEY_RSK, KEY_EVENT_DOWN);	
//			SetKeyHandler(mmi_vdoply_rsk_release, 	KEY_RSK, KEY_EVENT_DOWN);	
			SetKeyHandler(mmi_vdoply_rsk_release, 	KEY_RSK, KEY_EVENT_UP);		

//			SetKeyHandler(mmi_vdoply_lsk_release, 	KEY_ENTER, KEY_EVENT_DOWN);		
			SetKeyHandler(mmi_vdoply_lsk_press, 	KEY_ENTER, 	KEY_EVENT_DOWN);
			SetKeyHandler(mmi_vdoply_lsk_release, 	KEY_ENTER, KEY_EVENT_UP);		
//			SetKeyHandler(mmi_vdoply_lsk_release_timer, 	KEY_ENTER, KEY_EVENT_UP);		

			#ifdef __VDOPLY_FEATURE_VOLUME__
			//modify by niej no side key in CS621, so use key5 and key6 to adjust key volume 2007/09/10 begin 
			#if defined(__NO_SIDE_VOLUME_KEY_56__)
				SetKeyHandler(mmi_vdoply_vol_up_key_press, 		KEY_6, 	KEY_EVENT_DOWN);
				SetKeyHandler(mmi_vdoply_vol_up_key_release, 	KEY_6, 	KEY_EVENT_UP);
				SetKeyHandler(mmi_vdoply_vol_down_key_press, 	KEY_5,	KEY_EVENT_DOWN);
				SetKeyHandler(mmi_vdoply_vol_down_key_release, 	KEY_5, 	KEY_EVENT_UP);
		      #elif defined(__NO_SIDE_VOLUME_KEY_46__)
		        SetKeyHandler(mmi_vdoply_vol_up_key_press, 		KEY_6, 	KEY_EVENT_DOWN);
		        SetKeyHandler(mmi_vdoply_vol_up_key_release, 	KEY_6, 	KEY_EVENT_UP);
		        SetKeyHandler(mmi_vdoply_vol_down_key_press, 	KEY_4,	KEY_EVENT_DOWN);
		        SetKeyHandler(mmi_vdoply_vol_down_key_release, 	KEY_4, 	KEY_EVENT_UP);  
		      #elif defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__)
		        SetKeyHandler(mmi_vdoply_vol_up_key_press, 		KEY_POUND, 	KEY_EVENT_DOWN);
		        SetKeyHandler(mmi_vdoply_vol_up_key_release, 	KEY_POUND, 	KEY_EVENT_UP);
		        SetKeyHandler(mmi_vdoply_vol_down_key_press, 	KEY_STAR,	  KEY_EVENT_DOWN);
		        SetKeyHandler(mmi_vdoply_vol_down_key_release,KEY_STAR,   KEY_EVENT_UP); 
			#else
				SetKeyHandler(mmi_vdoply_vol_up_key_press, 		KEY_VOL_UP, 	KEY_EVENT_DOWN);
				SetKeyHandler(mmi_vdoply_vol_up_key_release, 	KEY_VOL_UP, 	KEY_EVENT_UP);
				SetKeyHandler(mmi_vdoply_vol_down_key_press, 	KEY_VOL_DOWN,	KEY_EVENT_DOWN);
				SetKeyHandler(mmi_vdoply_vol_down_key_release, 	KEY_VOL_DOWN, 	KEY_EVENT_UP);	
			#endif	
			//modify by niej no side key in CS621, so use key5 and key6 to adjust key volume 2007/09/10 end
			#endif /* __VDOPLY_FEATURE_VOLUME__ */

			#ifdef __VDOPLY_FEATURE_HORIZONTAL_VIEW__
				#ifdef __VDOPLY_FEATURE_SPEED__
					SetKeyHandler(mmi_vdoply_speed_inc_key_press, 	KEY_RIGHT_ARROW, 	KEY_EVENT_DOWN);
					SetKeyHandler(mmi_vdoply_speed_inc_key_release, KEY_RIGHT_ARROW, 	KEY_EVENT_UP);			
					SetKeyHandler(mmi_vdoply_speed_dec_key_press, 	KEY_LEFT_ARROW, 	KEY_EVENT_DOWN);				
					SetKeyHandler(mmi_vdoply_speed_dec_key_release, KEY_LEFT_ARROW, 	KEY_EVENT_UP);
				#endif
				SetKeyHandler(mmi_vdoply_seek_backward_key_press, 		KEY_UP_ARROW, 		KEY_EVENT_DOWN);
				SetKeyHandler(mmi_vdoply_seek_backward_key_release, 	KEY_UP_ARROW, 		KEY_EVENT_UP);
				SetKeyHandler(mmi_vdoply_seek_foward_key_press, 		KEY_DOWN_ARROW, 	KEY_EVENT_DOWN);
				SetKeyHandler(mmi_vdoply_seek_foward_key_release, 		KEY_DOWN_ARROW, 	KEY_EVENT_UP);	
			#else /* not horizontal view */
				#ifdef __VDOPLY_FEATURE_SPEED__
					TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_register_key_event __VDOPLY_FEATURE_SPEED__");
					SetKeyHandler(mmi_vdoply_speed_inc_key_press, 	KEY_UP_ARROW, 		KEY_EVENT_DOWN);
					SetKeyHandler(mmi_vdoply_speed_inc_key_release, KEY_UP_ARROW, 		KEY_EVENT_UP);			
					SetKeyHandler(mmi_vdoply_speed_dec_key_press, 	KEY_DOWN_ARROW, 	KEY_EVENT_DOWN);				
					SetKeyHandler(mmi_vdoply_speed_dec_key_release, KEY_DOWN_ARROW, 	KEY_EVENT_UP);
				#endif 
				SetKeyHandler(mmi_vdoply_seek_backward_key_press, 		KEY_LEFT_ARROW, 	KEY_EVENT_DOWN);
				SetKeyHandler(mmi_vdoply_seek_backward_key_long_press, 		KEY_LEFT_ARROW, 	KEY_LONG_PRESS);
				SetKeyHandler(mmi_vdoply_seek_backward_key_release, 	KEY_LEFT_ARROW, 	KEY_EVENT_UP);
				SetKeyHandler(mmi_vdoply_seek_foward_key_press, 		KEY_RIGHT_ARROW, 	KEY_EVENT_DOWN);
				SetKeyHandler(mmi_vdoply_seek_foward_key_long_press, 		KEY_RIGHT_ARROW, 	KEY_LONG_PRESS);
				SetKeyHandler(mmi_vdoply_seek_foward_key_release, 		KEY_RIGHT_ARROW, 	KEY_EVENT_UP);	
			#endif /* __VDOPLY_FEATURE_HORIZONTAL_VIEW__ */

			#ifdef __VDOPLY_FEATURE_SNAPSHOT__
				SetKeyHandler(mmi_vdoply_player_snapshot_key_press, KEY_CAMERA, KEY_EVENT_DOWN);
				SetKeyHandler(mmi_vdoply_player_snapshot_key_release, KEY_CAMERA, KEY_EVENT_UP);			
			#endif

			#ifdef __VDOPLY_FEATURE_FULLSCREEN__
				//if (g_vdoply_cntx.video_info.media_type != MED_TYPE_MJPG)  //avi file can't be full screen
				{
			//		if((g_vdoply_cntx.video_info.width==320 && g_vdoply_cntx.video_info.height==240) || (g_vdoply_cntx.video_info.width==176 && g_vdoply_cntx.video_info.height==144))
					{
	//					SetKeyHandler(mmi_vdoply_toggle_fullscreen_display, KEY_STAR, KEY_EVENT_UP);
						SetKeyHandler(mmi_vdoply_toggle_fullscreen_display_timer, /*KEY_STAR*/ KEY_0, KEY_EVENT_UP);
					}
				}
			#endif
			break;
     
		case VDOPLY_STATE_OPENING:		
		case VDOPLY_STATE_INTERRUPTED_OPENING:
		case VDOPLY_STATE_INTERRUPTED_SEEKING:
			ClearAllKeyHandler();
			SetKeyHandler(mmi_vdoply_lsk_press_In_Interrupted_state, 	KEY_LSK, KEY_EVENT_DOWN);	
			SetKeyHandler(mmi_vdoply_lsk_release_In_Interrupted_state, 	KEY_LSK, KEY_EVENT_UP);		
//			SetKeyHandler(mmi_vdoply_rsk_press, KEY_RSK, KEY_EVENT_DOWN);	
//			SetKeyHandler(mmi_vdoply_rsk_release, KEY_RSK, KEY_EVENT_UP);		
			break;

		case VDOPLY_STATE_SEEKING:
		case VDOPLY_STATE_PLAY_SEEKING:	
			/* seek state, shall not allow any key event */
			ClearAllKeyHandler();
			break;
			
		/* Full Screen */
		case VDOPLY_STATE_FULLSCR_IDLE:
		case VDOPLY_STATE_FULLSCR_PLAY:	
		case VDOPLY_STATE_FULLSCR_PAUSE:			
			ClearAllKeyHandler();
			
			SetKeyHandler(mmi_vdoply_lsk_press, KEY_LSK, KEY_EVENT_DOWN);	
			SetKeyHandler(mmi_vdoply_lsk_release, KEY_LSK, KEY_EVENT_UP);		

			SetKeyHandler(mmi_vdoply_rsk_press, KEY_RSK, KEY_EVENT_DOWN);	
			SetKeyHandler(mmi_vdoply_rsk_release, KEY_RSK, KEY_EVENT_UP);		

			#ifdef __VDOPLY_FEATURE_VOLUME__			
			#if defined(__NO_SIDE_VOLUME_KEY_56__)
			SetKeyHandler(mmi_vdoply_vol_up_key_press, 		KEY_6, 	KEY_EVENT_DOWN);
			SetKeyHandler(mmi_vdoply_vol_up_key_release, 	KEY_6, 	KEY_EVENT_UP);
			SetKeyHandler(mmi_vdoply_vol_down_key_press, 	KEY_5,	KEY_EVENT_DOWN);
			SetKeyHandler(mmi_vdoply_vol_down_key_release, 	KEY_5, 	KEY_EVENT_UP);
			#elif defined(__NO_SIDE_VOLUME_KEY_46__)
			SetKeyHandler(mmi_vdoply_vol_up_key_press, 		KEY_6, 	KEY_EVENT_DOWN);
			SetKeyHandler(mmi_vdoply_vol_up_key_release, 	KEY_6, 	KEY_EVENT_UP);
			SetKeyHandler(mmi_vdoply_vol_down_key_press, 	KEY_4,	KEY_EVENT_DOWN);
			SetKeyHandler(mmi_vdoply_vol_down_key_release, 	KEY_4, 	KEY_EVENT_UP);  
			#elif defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__)
			SetKeyHandler(mmi_vdoply_vol_up_key_press, 		KEY_POUND, 	KEY_EVENT_DOWN);
			SetKeyHandler(mmi_vdoply_vol_up_key_release, 	KEY_POUND, 	KEY_EVENT_UP);
			SetKeyHandler(mmi_vdoply_vol_down_key_press, 	KEY_STAR,	  KEY_EVENT_DOWN);
			SetKeyHandler(mmi_vdoply_vol_down_key_release,KEY_STAR,   KEY_EVENT_UP); 
			#else
			SetKeyHandler(mmi_vdoply_vol_up_key_press, 		KEY_VOL_UP, 	KEY_EVENT_DOWN);
			SetKeyHandler(mmi_vdoply_vol_up_key_release, 	KEY_VOL_UP, 	KEY_EVENT_UP);
			SetKeyHandler(mmi_vdoply_vol_down_key_press, 	KEY_VOL_DOWN,	KEY_EVENT_DOWN);
			SetKeyHandler(mmi_vdoply_vol_down_key_release, 	KEY_VOL_DOWN, 	KEY_EVENT_UP);	
			#endif		
			#endif /* __VDOPLY_FEATURE_VOLUME__ */

			#ifdef __VDOPLY_FEATURE_SPEED__
				SetKeyHandler(mmi_vdoply_speed_inc_key_press, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
				SetKeyHandler(mmi_vdoply_speed_inc_key_release, KEY_RIGHT_ARROW, KEY_EVENT_UP);			
				SetKeyHandler(mmi_vdoply_speed_dec_key_press, KEY_LEFT_ARROW, KEY_EVENT_DOWN);	
				SetKeyHandler(mmi_vdoply_speed_dec_key_release, KEY_LEFT_ARROW, KEY_EVENT_UP);					
			#endif /* __VDOPLY_FEATURE_SPEED__ */

			SetKeyHandler(mmi_vdoply_seek_backward_key_press, KEY_UP_ARROW, KEY_EVENT_DOWN);
			SetKeyHandler(mmi_vdoply_seek_foward_key_press, KEY_DOWN_ARROW, KEY_EVENT_DOWN);

			SetKeyHandler(mmi_vdoply_seek_backward_key_release, KEY_UP_ARROW, KEY_EVENT_UP);
			SetKeyHandler(mmi_vdoply_seek_foward_key_release, KEY_DOWN_ARROW, KEY_EVENT_UP);		

			#ifdef __VDOPLY_FEATURE_FULLSCREEN__
//				SetKeyHandler(mmi_vdoply_toggle_fullscreen_display, KEY_STAR, KEY_EVENT_UP);
				SetKeyHandler(mmi_vdoply_toggle_fullscreen_display_timer,/* KEY_STAR*/ KEY_0, KEY_EVENT_UP);
			#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
			break;

		case VDOPLY_STATE_FULLSCR_SEEKING:
		case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:	
			/* seek state, shall not allow any key event */
			ClearAllKeyHandler();				
			break;

	}	

	TBM_EXIT(0x2A63);
}

void mmi_vdoply_turnOnBacklight()
{
	mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
														g_vdoply_cntx.play_wnd_layer_handle,
															NULL);	

}

/*****************************************************************************
* FUNCTION
*	mmi_vdoply_player_enter_state
* DESCRIPTION
*  state transition function
* PARAMETERS
*  state 	IN 	video player's state
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
#ifdef __FF_AUDIO_SET__
extern void MusicEndAudioMode();
#endif

static void mmi_vdoply_player_enter_state(U16 state)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32			image_width;
	S32			image_height;
	MDI_RESULT 	ret = 0;
	U16			prev_state;
	U32			cur_time;
	U32			total_time;

	
	video_rect_struct 		*wnd_rect_p = (video_rect_struct *)&g_vdoply_osd_cntx.play_wnd.rect;
	mdi_video_info_struct	video_info;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   	TBM_ENTRY(0x2A62);
   
	prev_state = g_vdoply_cntx.state;

   /* flush key event */
	ClearKeyEvents();

   
	/* enter next state */
	TRACE_EF(g_sw_MED_Vid,"Func: %s state=%d", __FUNCTION__,state);
	switch(state)
	{
		case VDOPLY_STATE_IDLE:
			g_vdoply_cntx.state = state;
			if((gZoomHeight == VDOPLY_DISPLAY_SET_HEIGHT) &&  (gZoomWidth == VDOPLY_DISPLAY_SET_WIDTH)) //full screen
			{
				mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_WIDTH,VDOPLY_DISPLAY_SET_HEIGHT,0,0,1);
			}
			else
				{
			g_vdoply_cntx.cur_status.start_play_time	= 0;
			g_vdoply_cntx.cur_status.play_time 			= 0;	
			g_vdoply_cntx.cur_status.play_ratio 		= 0;	

			#if 0
			/* restore base layer to normal */
			gdi_layer_set_rotate((U8)g_vdoply_cntx.normal_style_rotate);
			gdi_layer_resize(g_vdoply_cntx.normal_style_lcd_width, g_vdoply_cntx.normal_style_lcd_height);
			gdi_layer_set_blt_layer(g_vdoply_cntx.base_layer_handle, g_vdoply_cntx.play_wnd_layer_handle,0 ,0);
			#endif
			#if 0//defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) 
				mmi_vdoply_set_softkey(NULL , (PS8)GetString(STR_GLOBAL_BACK), 0, IMG_ID_VDOPLY_RSK_BACK);
			#else
				mmi_vdoply_set_softkey(	(PS8)GetString(STR_ID_VDOPLY_PLAY), 
												(PS8)GetString(STR_GLOBAL_BACK),
												IMG_ID_VDOPLY_LSK_PLAY,
												IMG_ID_VDOPLY_RSK_BACK);													
			#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
#if 0
			if(prev_state == VDOPLY_STATE_FULLSCR_IDLE)
			{
				#ifdef __MMI_UI_STYLE_4__
					/* update status icon */
					refresh_status_icon_display();	
				#endif /* __MMI_UI_STYLE_4__ */

				
				/* back from fulls screen, need draw full skin */
				gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);

				#ifdef __MMI_UI_STYLE_4__
					/* update status icon */
					gdi_layer_lock_frame_buffer();
					refresh_status_icon_display();	
					gdi_layer_unlock_frame_buffer();
				#endif /* __MMI_UI_STYLE_4__ */
				
				mmi_vdoply_draw_title();
				mmi_vdoply_draw_softkey();				
				mmi_vdoply_draw_panel();
				mmi_vdoply_blt_screen();
			}
			else
			{
				mmi_vdoply_draw_softkey();
				mmi_vdoply_draw_panel_state();
				mmi_vdoply_draw_panel_progress();
				mmi_vdoply_draw_panel_timer();
				mmi_vdoply_blt_part();
			}
#endif
				#ifdef __MMI_UI_STYLE_4__
					/* update status icon */
					refresh_status_icon_display();	
				#endif /* __MMI_UI_STYLE_4__ */

				
				/* back from fulls screen, need draw full skin */
				gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);

				#ifdef __MMI_UI_STYLE_4__
					/* update status icon */
					gdi_layer_lock_frame_buffer();
					refresh_status_icon_display();	
					gdi_layer_unlock_frame_buffer();
				#endif /* __MMI_UI_STYLE_4__ */
				
				mmi_vdoply_draw_title();
				mmi_vdoply_draw_softkey();				
				mmi_vdoply_draw_panel();
				//mmi_vdoply_blt_screen();
			
			
			coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
#if 0						
			ret = mdi_video_ply_seek_and_get_frame(g_vdoply_cntx.cur_status.play_time,
																g_vdoply_cntx.play_wnd_layer_handle);
#endif
			mdi_video_ply_fullscreen_display_set(g_vdoply_cntx.video_info.width,g_vdoply_cntx.video_info.height,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty,0);			

				mmi_vdoply_blt_screen();//must behind mdi_video_ply_fullscreen_display_set. sheen
				}
			ret = mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																					mmi_vdoply_seek_result_hdlr);	

			TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_player_enter_state mdi_video_ply_seek_and_get_frame ret = %d",ret);
			mmi_vdoply_register_key_event(state);


          		  /* disconnect BT if needed */
           		 mmi_vdoply_disconnect_bt();

			/* error check */
			if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
			{
				TBM_EXIT(0x2A62);
				return;
			}
			#ifdef MMI_ON_WIN32
			
			gdi_layer_blt_base_layer(0,0, UI_device_width-1, UI_device_height-1);
			
			#endif
			TurnOffBacklight();//shenh
			break;
     
		case VDOPLY_STATE_PLAY:
			g_vdoply_cntx.state = state;
			
			g_vdoply_cntx.is_seeking					= FALSE;
			g_vdoply_cntx.is_seek_bwd_key_pressed 	= FALSE;
			g_vdoply_cntx.is_seek_fwd_key_pressed 	= FALSE;
			g_vdoply_cntx.is_lsk_pressed 				= FALSE;	
			g_vdoply_cntx.is_rsk_pressed 				= FALSE;	
			g_vdoply_cntx.is_speed_inc_key_pressed = FALSE;	
			g_vdoply_cntx.is_speed_dec_key_pressed	= FALSE;	
			g_vdoply_cntx.is_vol_inc_key_pressed	= FALSE;	
			g_vdoply_cntx.is_vol_dec_key_pressed	= FALSE;	
			
			TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_player_enter_state VDOPLY_STATE_PLAY");
#if 0
			/* restore base layer to normal */
			gdi_layer_set_rotate((U8)g_vdoply_cntx.normal_style_rotate);
			gdi_layer_resize(g_vdoply_cntx.normal_style_lcd_width, g_vdoply_cntx.normal_style_lcd_height);
			gdi_layer_set_blt_layer(g_vdoply_cntx.base_layer_handle, g_vdoply_cntx.play_wnd_layer_handle, 0, 0);
#endif		
			#if 0//defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) 
				mmi_vdoply_set_softkey(NULL , (PS8)GetString(STR_GLOBAL_BACK), 0, IMG_ID_VDOPLY_RSK_BACK);
			#else
				mmi_vdoply_set_softkey(	(PS8)GetString(STR_ID_VDOPLY_PAUSE) , 
												(PS8)GetString(STR_ID_VDOPLY_STOP),
												IMG_ID_VDOPLY_LSK_PAUSE,
												IMG_ID_VDOPLY_RSK_STOP);
			#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

			if(prev_state == VDOPLY_STATE_FULLSCR_PLAY)
			{
				/* re-calc paly ratio for progress display */
				cur_time 	= g_vdoply_cntx.cur_status.play_time;
				total_time 	= g_vdoply_cntx.video_info.total_time_duration;
				g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.progress.fill_width/total_time);
				
				/* back from fulls screen, need draw full skin */
				gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);

				#ifdef __MMI_UI_STYLE_4__
					/* update status icon */
					gdi_layer_lock_frame_buffer();
					refresh_status_icon_display();	
					gdi_layer_unlock_frame_buffer();
				#endif /* __MMI_UI_STYLE_4__ */
				
				mmi_vdoply_draw_title();
				mmi_vdoply_draw_softkey();				
				mmi_vdoply_draw_panel();
				mmi_vdoply_blt_screen();
			}
			else
			{
				mmi_vdoply_draw_softkey();
				mmi_vdoply_draw_panel_state();
				mmi_vdoply_draw_panel_progress();
				mmi_vdoply_draw_panel_timer();
                mmi_vdoply_draw_panel_volume();
				mmi_vdoply_blt_part();
			}
			
			
//			mmi_vdoply_blt_screen();
//			mmi_vdoply_blt_part();
			/* start play video */
			ret = mmi_vdoply_play_video();
			mmi_vdoply_register_key_event(state);
			
			/* error check */
			if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
			{
				TBM_EXIT(0x2A62);
				return;
			}
			
			/* start draw timer */
			g_vdoply_cntx.cur_status.start_play_time = g_vdoply_cntx.cur_status.play_time;
			mmi_vdoply_player_timer_cyclic();
			TurnOnBacklight(0);//wangzl: when play,don't close backlight
			break;
			
             case VDOPLY_STATE_BT_CONNECTING:
			 	
                   TRACE_EF(g_sw_MED_Vid,"enter VDOPLY_STATE_BT_CONNECTING");
                      g_vdoply_cntx.state = state;
			if(prev_state == VDOPLY_STATE_FULLSCR_PLAY)
			{

				/* back from fulls screen, need draw full skin */
				gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);

				#ifdef __MMI_UI_STYLE_4__
					/* update status icon */
					gdi_layer_lock_frame_buffer();
					refresh_status_icon_display();	
					gdi_layer_unlock_frame_buffer();
				#endif /* __MMI_UI_STYLE_4__ */
				
				mmi_vdoply_draw_title();
				mmi_vdoply_draw_softkey();				
				mmi_vdoply_draw_panel();
				mmi_vdoply_blt_screen();
			}
			else
			{
				mmi_vdoply_draw_softkey();
				mmi_vdoply_draw_panel_state();
				mmi_vdoply_draw_panel_progress();
				mmi_vdoply_draw_panel_timer();
				mmi_vdoply_blt_part();
			}

                     mmi_vdoply_connect_bt();
                      break;



		case VDOPLY_STATE_PAUSE:
			TRACE_EF(g_sw_MED_Vid,"enter VDOPLY_STATE_PAUSE");
			g_vdoply_cntx.state = state;
#if 0
			/* restore base layer to normal */
			gdi_layer_set_rotate((U8)g_vdoply_cntx.normal_style_rotate);
			gdi_layer_resize(g_vdoply_cntx.normal_style_lcd_width, g_vdoply_cntx.normal_style_lcd_height);
			gdi_layer_set_blt_layer(g_vdoply_cntx.base_layer_handle, g_vdoply_cntx.play_wnd_layer_handle, 0, 0);
#endif
			#if 0//defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) 
				mmi_vdoply_set_softkey(NULL , (PS8)GetString(STR_GLOBAL_BACK), 0, IMG_ID_VDOPLY_RSK_BACK);
			#else
				mmi_vdoply_set_softkey(	(PS8)GetString(STR_ID_VDOPLY_RESUME), 
												(PS8)GetString(STR_ID_VDOPLY_STOP),
												IMG_ID_VDOPLY_LSK_RESUME,
												IMG_ID_VDOPLY_RSK_STOP);
			#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

			if(prev_state == VDOPLY_STATE_FULLSCR_PAUSE)
			{
				/* re-calc paly ratio for progress display */
				cur_time 	= g_vdoply_cntx.cur_status.play_time;
				total_time 	= g_vdoply_cntx.video_info.total_time_duration;
				g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.progress.fill_width/total_time);
				
				/* back from fulls screen, need draw full skin */
				gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);

				#ifdef __MMI_UI_STYLE_4__
					/* update status icon */
					gdi_layer_lock_frame_buffer();
					refresh_status_icon_display();	
					gdi_layer_unlock_frame_buffer();
				#endif /* __MMI_UI_STYLE_4__ */
				
				mmi_vdoply_draw_title();
				mmi_vdoply_draw_softkey();				
				mmi_vdoply_draw_panel();
				mmi_vdoply_blt_screen();
			}
			else
			{
				mmi_vdoply_draw_softkey();
				mmi_vdoply_draw_panel_state();
				mmi_vdoply_draw_panel_progress();
				mmi_vdoply_draw_panel_timer();
				mmi_vdoply_blt_part();
			}

			mmi_vdoply_register_key_event(state);
			
			/* get current play time */
			mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);
//			mmi_vdoply_blt_screen();
//			mmi_vdoply_blt_part();


                  /* disconnect BT if needed */
                    mmi_vdoply_disconnect_bt();

			coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
			TurnOffBacklight(); //wangzl:add close backlight after a timer
			break;

		case VDOPLY_STATE_INTERRUPTED_PAUSE:
			g_vdoply_cntx.state = state;
			
			gdi_layer_lock_frame_buffer();

			#if 0//defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) 
				mmi_vdoply_set_softkey(NULL , (PS8)GetString(STR_GLOBAL_BACK), 0, IMG_ID_VDOPLY_RSK_BACK);
			#else
				mmi_vdoply_set_softkey(	(PS8)GetString(STR_ID_VDOPLY_RESUME), 
												(PS8)GetString(STR_ID_VDOPLY_STOP),
												IMG_ID_VDOPLY_LSK_RESUME,
												IMG_ID_VDOPLY_RSK_STOP);
			#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

			mmi_vdoply_draw_softkey();
			mmi_vdoply_draw_panel();
			
			mmi_vdoply_register_key_event(state);

			gdi_layer_unlock_frame_buffer();

//			mmi_vdoply_blt_screen();
			mmi_vdoply_blt_part();

			coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
			TurnOffBacklight();//shenh

			break;

		case VDOPLY_STATE_OPENING:
		case VDOPLY_STATE_INTERRUPTED_OPENING:
			g_vdoply_cntx.state = state;
			mmi_vdoply_set_softkey(NULL , (PS8)GetString(STR_GLOBAL_BACK), 0, IMG_ID_VDOPLY_RSK_BACK);

			mmi_vdoply_draw_softkey();
		

			gdi_image_get_dimension_id(IMG_ID_VDOPLY_LOADING, &image_width, &image_height);
			gdi_image_draw_animation_id(	((wnd_rect_p->width-image_width)>>1)+wnd_rect_p->offset_x, 
													((wnd_rect_p->height-image_height)>>1)+wnd_rect_p->offset_y, 
													IMG_ID_VDOPLY_LOADING, NULL);
//			mmi_vdoply_blt_screen();
//			mmi_vdoply_blt_part();
			/* open file */
			if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_FILE)
			{
				ret = mdi_video_ply_open_file(g_vdoply_cntx.filename, mmi_vdoply_player_open_file_done_hdlr,mmi_vdoply_player_timer);
			}
			else if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_ID)
			{
				/* play from ID is a blocking function, we will call opened done hdlr by ourself */
				ret = mdi_video_ply_open_clip_id(g_vdoply_cntx.video_id, &video_info, TRUE);

				if(ret == MDI_RES_VDOPLY_SUCCEED)
				{
					/* open success, call open done hdlr */
					mmi_vdoply_player_open_file_done_hdlr(MDI_RES_VDOPLY_SUCCEED, &video_info);
				}
			}
			else
			{
				ASSERT(0);
			}
			
			mmi_vdoply_register_key_event(state);
			/* error check */
			if(mmi_vdoply_check_and_display_error_popup(ret) == TRUE) /* true means has error popup */
			{
				TBM_EXIT(0x2A62);
				return;
			}

			break;

		case VDOPLY_STATE_INTERRUPTED_SEEKING:
			g_vdoply_cntx.state = state;
			if((gZoomHeight == VDOPLY_DISPLAY_SET_HEIGHT) &&  (gZoomWidth == VDOPLY_DISPLAY_SET_WIDTH)) //full screen
			{
				mdi_video_ply_fullscreen_display_set(VDOPLY_DISPLAY_SET_WIDTH,VDOPLY_DISPLAY_SET_HEIGHT,0,0,1);
			}
			else
			{
				mdi_video_ply_fullscreen_display_set(g_vdoply_cntx.video_info.width,g_vdoply_cntx.video_info.height,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty,0);	
				/* re-calc paly ratio for progress display */
				cur_time 	= g_vdoply_cntx.cur_status.play_time;
				total_time 	= g_vdoply_cntx.video_info.total_time_duration;
				g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.progress.fill_width/total_time);
				
				/* back from fulls screen, need draw full skin */
				gdi_image_draw_id(0, 0, IMG_ID_VDOPLY_OSD_BG);

				#ifdef __MMI_UI_STYLE_4__
					/* update status icon */
					gdi_layer_lock_frame_buffer();
					refresh_status_icon_display();	
					gdi_layer_unlock_frame_buffer();
				#endif /* __MMI_UI_STYLE_4__ */
				
				mmi_vdoply_draw_title();
				mmi_vdoply_draw_softkey();				
				mmi_vdoply_draw_panel();
				mmi_vdoply_blt_screen();
			}			
			
				
			ret = mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																				mmi_vdoply_seek_result_hdlr);	

			/* error check */
			if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
			{
				TBM_EXIT(0x2A62);
				return;
			}

			mmi_vdoply_register_key_event(state);
			break;

		case VDOPLY_STATE_SEEKING:
		case VDOPLY_STATE_PLAY_SEEKING:
			g_vdoply_cntx.state = state;

			ret = mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																					mmi_vdoply_seek_result_hdlr);	
				
			TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_player_enter_state VDOPLY_STATE_PLAY_SEEKING ret=%d",ret);											

            		/* disconnect BT if needed */
          	  	mmi_vdoply_disconnect_bt();

			/* error check */
			if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
			{
				TBM_EXIT(0x2A62);
				return;
			}

			mmi_vdoply_register_key_event(state);
			break;

		case VDOPLY_STATE_EXIT:
			#ifdef __FF_AUDIO_SET__
			MusicEndAudioMode();
			#endif
			g_vdoply_cntx.state = state;
			GoBackHistory();
			break;


	#ifdef __VDOPLY_FEATURE_FULLSCREEN__

		case VDOPLY_STATE_FULLSCR_IDLE:
			
			g_vdoply_cntx.state = state;
			
			g_vdoply_cntx.cur_status.start_play_time	= 0;
			g_vdoply_cntx.cur_status.play_time 			= 0;	
			g_vdoply_cntx.cur_status.play_ratio 		= 0;	

			/* reset osd's opacity */
//			mmi_vdoply_fullscr_reset_osd_fade(TRUE);

			#if 0//defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) 
				mmi_vdoply_set_softkey(NULL , (PS8)GetString(STR_GLOBAL_BACK), 0, IMG_ID_VDOPLY_RSK_BACK);
			#else
				mmi_vdoply_set_softkey(	(PS8)GetString(STR_ID_VDOPLY_PLAY), 
												(PS8)GetString(STR_GLOBAL_BACK),
												IMG_ID_VDOPLY_LSK_PLAY,
												IMG_ID_VDOPLY_RSK_BACK);
			#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

			
			mmi_vdoply_register_key_event(state);

			/* rotate and resize base layer */
//			gdi_layer_set_rotate(GDI_LAYER_ROTATE_270);
//			gdi_layer_resize(UI_device_height, UI_device_width);
			
//			gdi_layer_set_blt_layer(g_vdoply_cntx.base_layer_handle, g_vdoply_cntx.osd_layer_handle, 0, 0);
//			gdi_layer_clear(GDI_COLOR_BLACK);

			if(prev_state == VDOPLY_STATE_IDLE)
			{
				/* re-calc paly ratio for progress display */
				cur_time 	= g_vdoply_cntx.cur_status.play_time;
				total_time 	= g_vdoply_cntx.video_info.total_time_duration;
				g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.fullscr_progress.fill_width/total_time);
			}
		
//			mmi_vdoply_draw_fullscr_osd();
			
			coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
#if 0		
			ret = mdi_video_ply_seek_and_get_frame(g_vdoply_cntx.cur_status.play_time,
																g_vdoply_cntx.base_layer_handle);
#endif

			ret = mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																				g_vdoply_cntx.play_wnd_layer_handle,
																					mmi_vdoply_seek_result_hdlr);	

           		 /* disconnect BT if needed */
            		mmi_vdoply_disconnect_bt();

			
//			mmi_vdoply_blt_screen();

			/* error check */
			if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
			{
				TBM_EXIT(0x2A62);
				return;
			}
			TurnOffBacklight(); //wangzl:add close backlight after a timer

			break;
     
	
		case VDOPLY_STATE_FULLSCR_PLAY:

				g_vdoply_cntx.state = state;

				/* reset osd's opacity */
//				mmi_vdoply_fullscr_reset_osd_fade(TRUE);

				#if 0//defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) 
					mmi_vdoply_set_softkey(NULL , (PS8)GetString(STR_GLOBAL_BACK), 0, IMG_ID_VDOPLY_RSK_BACK);
				#else
					mmi_vdoply_set_softkey(	(PS8)GetString(STR_ID_VDOPLY_PAUSE), 
													(PS8)GetString(STR_ID_VDOPLY_STOP),
													IMG_ID_VDOPLY_LSK_PAUSE,
													IMG_ID_VDOPLY_RSK_BACK);
				#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */


				mmi_vdoply_register_key_event(state);

				/* rotate and resize base layer */
//				gdi_layer_set_rotate(GDI_LAYER_ROTATE_270);
//				gdi_layer_resize(UI_device_height, UI_device_width);
				

//				gdi_layer_set_blt_layer(g_vdoply_cntx.base_layer_handle, g_vdoply_cntx.osd_layer_handle,0 ,0);
//				gdi_layer_clear(GDI_COLOR_BLACK);

				if(prev_state == VDOPLY_STATE_PLAY)
				{
					/* re-calc paly ratio for progress display */
					cur_time 	= g_vdoply_cntx.cur_status.play_time;
					total_time 	= g_vdoply_cntx.video_info.total_time_duration;
					g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.fullscr_progress.fill_width/total_time);
				}
#if 0
				ret = mdi_video_ply_seek_and_get_frame(g_vdoply_cntx.cur_status.play_time,
																	g_vdoply_cntx.base_layer_handle);

				/* error check */
				if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
				{
					TBM_EXIT(0x2A62);
					return;
				}

#endif
//				mmi_vdoply_draw_fullscr_osd();
//				mmi_vdoply_blt_screen();
				
				/* start play video */
				ret = mmi_vdoply_play_video();
						
				/* error check */
				if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
				{
					TBM_EXIT(0x2A62);
					return;
				}
				
				/* start draw timer */
				g_vdoply_cntx.cur_status.start_play_time = g_vdoply_cntx.cur_status.play_time;
				mmi_vdoply_player_timer_cyclic();
				TurnOnBacklight(0);//wangzl: when play,don't close backlight

				break;
				
       		 case VDOPLY_STATE_FULLSCR_BT_CONNECTING:
                           g_vdoply_cntx.state = state;
                          TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_player_enter_state VDOPLY_STATE_FULLSCR_BT_CONNECTING ");	

     

                     mmi_vdoply_connect_bt();

               
                                break;
                            

			case VDOPLY_STATE_FULLSCR_PAUSE:
				g_vdoply_cntx.state = state;

				/* reset osd's opacity */
//				mmi_vdoply_fullscr_reset_osd_fade(TRUE);

				#if 0//defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) 
					mmi_vdoply_set_softkey(NULL , (PS8)GetString(STR_GLOBAL_BACK), 0, IMG_ID_VDOPLY_RSK_BACK);
				#else
					mmi_vdoply_set_softkey(	(PS8)GetString(STR_ID_VDOPLY_RESUME), 
													(PS8)GetString(STR_ID_VDOPLY_STOP),
													IMG_ID_VDOPLY_LSK_RESUME,
													IMG_ID_VDOPLY_RSK_BACK);
				#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
				
				mmi_vdoply_register_key_event(state);

				/* rotate and resize base layer */
//				gdi_layer_set_rotate(GDI_LAYER_ROTATE_270);
//				gdi_layer_resize(UI_device_height, UI_device_width);	
				
//				gdi_layer_set_blt_layer(g_vdoply_cntx.base_layer_handle, g_vdoply_cntx.osd_layer_handle,0 ,0);
//				gdi_layer_clear(GDI_COLOR_BLACK);

				/* get current play time */
//				mdi_video_ply_get_cur_play_time(&g_vdoply_cntx.cur_status.play_time);
				mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);

#if 0
				ret = mdi_video_ply_seek_and_get_frame(g_vdoply_cntx.cur_status.play_time,
																	g_vdoply_cntx.base_layer_handle);
#endif


				if(prev_state == VDOPLY_STATE_PAUSE)
				{
					/* re-calc paly ratio for progress display */
					cur_time 	= g_vdoply_cntx.cur_status.play_time;
					total_time 	= g_vdoply_cntx.video_info.total_time_duration;
					g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.fullscr_progress.fill_width/total_time);
				}
				ret = MDI_RES_VDOPLY_SUCCEED;
				

  			  /* disconnect BT if needed */
            			mmi_vdoply_disconnect_bt();

				/* error check */
				if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
				{
					TBM_EXIT(0x2A62);
					return;
				}
				
//				mmi_vdoply_draw_fullscr_osd();
//				mmi_vdoply_blt_screen();

				coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);

				TurnOffBacklight(); //wangzl:add close backlight after a timer
				
				break;

			case VDOPLY_STATE_FULLSCR_SEEKING:
			case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:	
				g_vdoply_cntx.state = state;				

				ret = mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																					g_vdoply_cntx.base_layer_handle,
																					mmi_vdoply_seek_result_hdlr);	
          			  /* disconnect BT if needed */
           			 mmi_vdoply_disconnect_bt();

				/* error check */
				if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
				{
					TBM_EXIT(0x2A62);
					return;
				}

				mmi_vdoply_register_key_event(state);
				break;
				
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */


	}
	TBM_EXIT(0x2A62);
}                    



/*****************************************************************************
* FUNCTION
*	 mmi_vdoply_clam_close_event_hdlr()
* DESCRIPTION
*   handle clam close event 
* PARAMETERS
*	 ptr   IN	    message data ptr
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__) && defined(MMI_ON_HARDWARE_P)
static U8 mmi_vdoply_clam_close_event_hdlr(void* ptr)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_eq_gpio_detect_ind_struct *gpioDetectInd;
	
	gpioDetectInd = (mmi_eq_gpio_detect_ind_struct *)ptr;
	
	if((gpioDetectInd->gpio_device == EXT_DEV_CLAM_CLOSE) || 
		(gpioDetectInd->gpio_device == EXT_DEV_CLAM_OPEN) )
	{
		
		if(gpioDetectInd->gpio_device == EXT_DEV_CLAM_CLOSE)
		{
			GoBackHistory();
			return FALSE;	
		}
	}

	/* return false means will pass this interrut to default handler */
	return FALSE;	
}
#endif /* __MMI_CLAMSHELL__ && __MMI_SLIDE__ && MMI_ON_HARDWARE_P */



/*****************************************************************************
* FUNCTION
*	mmi_vodply_player_play_finish_hdlr
* DESCRIPTION
*  function to be called when play finished.
* PARAMETERS
*  result 	IN 		play finish result callback	 
*  (if result >=0, means successfully finished, if result < 0 measn some error happened)
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vodply_player_play_finish_hdlr(MDI_RESULT result)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   BOOL	is_back_from_fullscreen;	
	S32	speed_factor;	

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* if not in player screen */
	/* 
		this may happend when video has error and driver send out this message, 
		but before MMI receive this message, MMI stop and play the video again, 
		that will cause driver to send another play finish message. When MMI 
		receive first message, it will exit and display popup, when the second 
		message comes, MMI already exit video play app. so we shall igore this 
		message.
	*/
	
	TRACE_EF(g_sw_MED_Vid,"mmi_vodply_player_play_finish_hdlr g_vdoply_cntx.state=%d",g_vdoply_cntx.state);
	g_vdoply_cntx.is_lsk_pressed 				= FALSE;
	g_vdoply_cntx.is_rsk_pressed 				= FALSE;	
	g_vdoply_cntx.is_speed_inc_key_pressed = FALSE;
	g_vdoply_cntx.is_speed_dec_key_pressed = FALSE;
	g_vdoply_cntx.is_vol_inc_key_pressed 	= FALSE;	
	g_vdoply_cntx.is_vol_dec_key_pressed 	= FALSE;

	is_back_from_fullscreen = FALSE;
	speed_factor = mmi_vdoply_get_speed_factor();
#ifdef __FF_AUDIO_SET__
	  	MusicEndAudioMode();    //changed by jinzh:20070730
#endif	
	if(GetExitScrnID() != SCR_ID_VDOPLY_PLAYER)
		return;

	/* stop UI seeking */
//	coolsand_UI_cancel_timer(mmi_vdoply_seek_foward_key_press_cyclic);
//	coolsand_UI_cancel_timer(mmi_vdoply_seek_backward_key_press_cyclic);
	coolsand_UI_cancel_timer(mmi_vdoply_seek_key_release);	
	g_vdoply_cntx.is_seeking = FALSE;

	gZoomHeight = 0;
	gZoomWidth = 0;
	/* check if has error or not */
	if(mmi_vdoply_check_and_display_error_popup(result) == TRUE) /* true means have error */
		return;

	if( (g_vdoply_cntx.state == VDOPLY_STATE_PLAY) ||
		 (g_vdoply_cntx.state == VDOPLY_STATE_PLAY_SEEKING))
	{
//		if(g_vdoply_cntx.state == VDOPLY_STATE_PLAY_SEEKING)   //Ƶĩβʱֹٰͣʱʾ.Seek֮ǰstopһ
		{
			mdi_video_ply_stop_non_block_seek();
		}
		
		/* get current play time */
//		mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);

		/* draw timer bar to end */
		if(speed_factor > 0)
			g_vdoply_cntx.cur_status.play_ratio = g_vdoply_cntx.progress.fill_width;
		else
			g_vdoply_cntx.cur_status.play_ratio = 0;

		mmi_vdoply_draw_panel_progress();
//		mmi_vdoply_draw_panel_timer();
		
		mmi_vdoply_blt_part();

		/* retore normal speed */
		g_vdoply_cntx.cur_status.speed = VDOPLY_DEFAULT_SPEED;
		mmi_vdoply_draw_panel();
		
		mmi_vdoply_player_enter_state(VDOPLY_STATE_IDLE);	
	}
	else if( (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY) ||
				(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY_SEEKING) )
	{
#ifdef __VDOPLY_FEATURE_FULLSCREEN__	
		//if(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY_SEEKING)
		{
			mdi_video_ply_stop_non_block_seek();
		}
		
		/* get current play time */
		mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);

		/* draw timer bar to end */
		if(speed_factor > 0)
			g_vdoply_cntx.cur_status.play_ratio = g_vdoply_cntx.fullscr_progress.fill_width;
		else
			g_vdoply_cntx.cur_status.play_ratio = 0;

		/* retore normal speed */
		g_vdoply_cntx.cur_status.speed = VDOPLY_DEFAULT_SPEED;
//		mmi_vdoply_draw_fullscr_osd();

//		mmi_vdoply_blt_screen();
		
//		mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_IDLE);	
		g_vdoply_cntx.state = VDOPLY_STATE_FULLSCR_IDLE;
		mmi_vdoply_display_set_coordinate();
		mdi_video_ply_fullscreen_display_set(g_vdoply_cntx.video_info.width,g_vdoply_cntx.video_info.height,g_vdoply_cntx.video_info.startx,g_vdoply_cntx.video_info.starty,0);

		mmi_vdoply_player_enter_state(VDOPLY_STATE_IDLE);

#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */		
	}

}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_player_open_file_done_hdlr
* DESCRIPTION
*  function to be called when file is opened.
* PARAMETERS
*  result	  	IN		open file result
*	(if result >=0, means successfully opened, if result < 0 measn some error happened)  
*  vdo_clip 	IN 	video info
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdoply_player_open_file_done_hdlr(MDI_RESULT result, mdi_video_info_struct *vdo_clip)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 resized_offset_x;
	S32 resized_offset_y;
	S32 resized_width;
	S32 resized_height;

	video_osd_play_wnd_struct *play_wnd_ptr =(video_osd_play_wnd_struct *) &g_vdoply_osd_cntx.play_wnd;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* check if already exit player screen */
	if(GetExitScrnID() != SCR_ID_VDOPLY_PLAYER)
	{
		return;
	}
	if(!( (g_vdoply_cntx.state == VDOPLY_STATE_OPENING) 
		|| (g_vdoply_cntx.state == VDOPLY_STATE_INTERRUPTED_OPENING)))
		return;
	/* stop loading animation */
	gdi_image_stop_animation_all();
	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_player_open_file_done_hdlr result = %d,vdo_clip->width:%d, vdo_clip->height:%d",result,vdo_clip->width, vdo_clip->height);
	TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_player_open_file_done_hdlr track = %d,vdo_clip->aud_channel:%d, vdo_clip->aud_sample_rate:%d",vdo_clip->track,vdo_clip->aud_channel, vdo_clip->aud_sample_rate);
	/* if open sucessfully */
	if(result >= 0)
	{
		/* clear bg's window region */
		gdi_draw_solid_rect(	play_wnd_ptr->rect.offset_x,
									play_wnd_ptr->rect.offset_y,
									play_wnd_ptr->rect.offset_x+play_wnd_ptr->rect.width-1,
									play_wnd_ptr->rect.offset_y+play_wnd_ptr->rect.height-1,									
									GDI_COLOR_BLACK);
		
		/* calc draw size */
		gdi_image_util_fit_bbox(play_wnd_ptr->rect.width,
										play_wnd_ptr->rect.height,
										vdo_clip->width,
										vdo_clip->height,
										&resized_offset_x,
										&resized_offset_y,
										&resized_width,
										&resized_height);


		/* fit it into player window */
		g_vdoply_cntx.video_info.width					= (U16)g_vdoply_osd_cntx.play_wnd.rect.width;//vdo_clip->width;
		g_vdoply_cntx.video_info.height					=  (U16)(g_vdoply_osd_cntx.play_wnd.rect.width*3)>>2;//vdo_clip->height;
		if(g_vdoply_cntx.video_info.height > g_vdoply_osd_cntx.play_wnd.rect.height)
			g_vdoply_cntx.video_info.height = (U16)g_vdoply_osd_cntx.play_wnd.rect.height;
		g_vdoply_cntx.video_info.total_time_duration	= vdo_clip->total_time_duration;
		g_vdoply_cntx.video_info.media_type = vdo_clip->media_type;

            g_vdoply_cntx.video_info.aud_channel_no = vdo_clip->aud_channel;
            g_vdoply_cntx.video_info.aud_sample_rate= vdo_clip->aud_sample_rate;
            g_vdoply_cntx.video_info.track= vdo_clip->track;
/*
		if(g_vdoply_cntx.state == VDOPLY_STATE_INTERRUPTED_OPENING)
		{

		}
		else
		*/
		{
				/*
				if(g_vdoply_cntx.video_info.width==320 && g_vdoply_cntx.video_info.height==240)
				{
					g_vdoply_cntx.video_info.startx = g_vdoply_osd_cntx.play_wnd.rect.offset_x +(g_vdoply_osd_cntx.play_wnd.rect.width - 240) / 2;
					g_vdoply_cntx.video_info.starty = g_vdoply_osd_cntx.play_wnd.rect.offset_y +(g_vdoply_osd_cntx.play_wnd.rect.height - 180) / 2;
				}
				else*/
				{
					g_vdoply_cntx.video_info.startx = g_vdoply_osd_cntx.play_wnd.rect.offset_x +(g_vdoply_osd_cntx.play_wnd.rect.width - g_vdoply_cntx.video_info.width) / 2;
					g_vdoply_cntx.video_info.starty = g_vdoply_osd_cntx.play_wnd.rect.offset_y +(g_vdoply_osd_cntx.play_wnd.rect.height - g_vdoply_cntx.video_info.height) / 2;

				}
		}

		/* this avoid driver return total time druation == 0, which will cause divid by zero */
		if(g_vdoply_cntx.video_info.total_time_duration == 0)
		{
			g_vdoply_cntx.video_info.total_time_duration = 1;
		}
		
		/* this is used solve when interrupt happened while video is finish,
		   driver already send out finish idication but MMI didnt reveice this message
		   and call get time. Will get play time which exceed total time */
		if(g_vdoply_cntx.cur_status.play_time >= g_vdoply_cntx.video_info.total_time_duration)
		{
			g_vdoply_cntx.cur_status.play_time = 0;
		}

		/* create play window layer */
		gdi_layer_get_active(&g_vdoply_cntx.base_layer_handle);
		#ifdef __VDOPLY_FEATURE_HORIZONTAL_VIEW__

			gdi_layer_create(	0,
									0, 
									resized_width, 
									resized_height,
									&g_vdoply_cntx.play_wnd_layer_handle);

			gdi_layer_push_and_set_active(g_vdoply_cntx.play_wnd_layer_handle);

			/* set playback layer rotate */
			gdi_layer_set_rotate((U8)g_vdoply_cntx.normal_style_rotate);
			gdi_layer_set_position(	play_wnd_ptr->rect.offset_x+resized_offset_x,
											play_wnd_ptr->rect.offset_y+resized_offset_y);

		#else
			/*
			gdi_layer_create(	play_wnd_ptr->rect.offset_x+resized_offset_x,
									play_wnd_ptr->rect.offset_y+resized_offset_y, 
									resized_width, 
									resized_height,
									&g_vdoply_cntx.play_wnd_layer_handle);
			*/
			gdi_layer_create(	g_vdoply_cntx.video_info.startx,
									g_vdoply_cntx.video_info.starty, 
									g_vdoply_cntx.video_info.width, 
									g_vdoply_cntx.video_info.height,
									&g_vdoply_cntx.play_wnd_layer_handle);


			gdi_layer_push_and_set_active(g_vdoply_cntx.play_wnd_layer_handle);
		
		#endif /* __VDOPLY_FEATURE_HORIZONTAL_VIEW__ */
			
		gdi_layer_clear_background(GDI_COLOR_BLACK);
		gdi_layer_pop_and_restore_active();

		gdi_layer_set_blt_layer(g_vdoply_cntx.base_layer_handle, g_vdoply_cntx.play_wnd_layer_handle, 0, 0);
		if(g_vdoply_cntx.state == VDOPLY_STATE_OPENING)
			mmi_vdoply_player_enter_state(VDOPLY_STATE_IDLE);
		else if(g_vdoply_cntx.state == VDOPLY_STATE_INTERRUPTED_OPENING)
			mmi_vdoply_player_enter_state(VDOPLY_STATE_INTERRUPTED_SEEKING);
	}
	else
	{
		/* if open file error - popup and go to previous window */
		DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_INVALID_VIDEO_FILE), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
		DeleteNHistory(1);
	}

}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_exit_player_screen_internal
* DESCRIPTION
*  exit view detail screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_exit_player_screen_internal(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16		error;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* exit handler in different state */
	switch(g_vdoply_cntx.state)
	{
		case VDOPLY_STATE_EXIT:
			break;
			
		case VDOPLY_STATE_IDLE:
		case VDOPLY_STATE_PAUSE:
		case VDOPLY_STATE_OPENING:
		case VDOPLY_STATE_INTERRUPTED_OPENING:
		case VDOPLY_STATE_INTERRUPTED_PAUSE:
		case VDOPLY_STATE_SNAPSHOT:
			mdi_video_ply_stop();
			if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_FILE)
			{
				mdi_video_ply_close_file();
			}
			else if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_ID)
			{
				mdi_video_ply_close_clip_id();		
			}

			gdi_layer_flatten_previous_to_base();
			break;

		case VDOPLY_STATE_PLAY:

			/* interrupt while playing, store play time var */
			mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);

			/* if is seeking, it is already stopped */

			if(!g_vdoply_cntx.is_seeking)
				mdi_video_ply_stop();
			if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_FILE)
			{
				TRACE_EF(g_sw_MED_Vid,"mmi_vdoply_exit_player_screen_internal VDOPLY_PLAY_FROM_FILE g_vdoply_cntx.is_seeking=%d",g_vdoply_cntx.is_seeking);
				mdi_video_ply_close_clip_file();
			}
			else if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_ID)
			{
				mdi_video_ply_close_clip_id();		
			}

			gdi_layer_flatten_previous_to_base();
			break;

		case VDOPLY_STATE_SEEKING:
		case VDOPLY_STATE_PLAY_SEEKING:	
		case VDOPLY_STATE_INTERRUPTED_SEEKING:
		
			/* stop non-blocking seeking */
			mdi_video_ply_stop_non_block_seek();

			if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_FILE)
			{
				mdi_video_ply_close_clip_file();
			}
			else if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_ID)
			{
				mdi_video_ply_close_clip_id();		
			}

			gdi_layer_flatten_previous_to_base();
			break;

		case VDOPLY_STATE_FULLSCR_IDLE:
		case VDOPLY_STATE_FULLSCR_PAUSE:
			mdi_video_ply_stop();
			if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_FILE)
			{
				mdi_video_ply_close_clip_file();
			}
			else if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_ID)
			{
				mdi_video_ply_close_clip_id();		
			}
			
			/* clear base as black */
			/* we cant use flatten since layer size is different */
			gdi_layer_clear(GDI_COLOR_BLACK);

			break;
			
		case VDOPLY_STATE_FULLSCR_PLAY:	
			/* interrupt while playing, store play time var */
//			mdi_video_ply_get_cur_play_time(&g_vdoply_cntx.cur_status.play_time);
			mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);


			/* if is seeking, it is already stopped */

			if(!g_vdoply_cntx.is_seeking)
				mdi_video_ply_stop();

			if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_FILE)
			{
				mdi_video_ply_close_clip_file();
			}
			else if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_ID)
			{
				mdi_video_ply_close_clip_id();		
			}
			
			/* clear base as black */
			/* we cant use flatten since layer size is different */
			gdi_layer_clear(GDI_COLOR_BLACK);
			
			break;

		case VDOPLY_STATE_FULLSCR_SEEKING:
		case VDOPLY_STATE_FULLSCR_PLAY_SEEKING:
			/* stop non-blocking seeking */
			mdi_video_ply_stop_non_block_seek();
			
			if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_FILE)
			{
				mdi_video_ply_close_clip_file();
			}
			else if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_ID)
			{
				mdi_video_ply_close_clip_id();		
			}

			/* clear base as black */
			/* we cant use flatten since layer size is different */
			gdi_layer_clear(GDI_COLOR_BLACK);
			
			break;
			
	}

	#ifdef __FF_AUDIO_SET__
	MusicEndAudioMode();//chenhe add
	#endif
	g_vdoply_cntx.state = VDOPLY_STATE_EXIT;
	/* rotate base layer back to normal */
	gdi_layer_set_rotate(GDI_LAYER_ROTATE_0);	
	gdi_layer_resize(UI_device_width, UI_device_height);

	/* restore to base layer only */
	gdi_layer_set_blt_layer(g_vdoply_cntx.base_layer_handle, 0, 0, 0);

	/* stop all timer */
//	coolsand_UI_cancel_timer(mmi_vdoply_seek_backward_key_press_cyclic);
//	coolsand_UI_cancel_timer(mmi_vdoply_seek_foward_key_press_cyclic);
	coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
	coolsand_UI_cancel_timer(mmi_vdoply_seek_key_release);
//	coolsand_UI_cancel_timer(mmi_vdoply_lsk_release);

	#ifdef __VDOPLY_FEATURE_FULLSCREEN__
		coolsand_UI_cancel_timer(mmi_vdoply_toggle_fullscreen_display);
		coolsand_UI_cancel_timer(mmi_vdoply_fullscr_osd_fade_cyclic);
	#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

	#ifdef __MMI_UI_STYLE_4__
		/* restore hide status to default */
		register_hide_status_icon_bar(0,hide_status_icons_bar0);
	#endif /* __MMI_UI_STYLE_4__ */
	
	/* stop all playing animation */
	gdi_image_stop_animation_all();

	/* disable multi layer */
	gdi_layer_multi_layer_disable();

	/* resume alignment timer */
	UI_disable_alignment_timers();

	/* resume LCD patten */
	StopLEDPatternPowerOn();

	/* let MMI can sleep */
	TurnOffBacklight();

	/* resume background audio */
	mdi_audio_resume_background_play();

	WriteValue((U8)NVRAM_VDOPLY_VOLUMN, (void*)&g_vdoply_cntx.cur_status.volume, DS_BYTE , &error);

	ClearProtocolEventHandler(MSG_ID_MEDIA_VID_SEEK_DONE_IND);
	ClearProtocolEventHandler(MSG_ID_MEDIA_VID_FILE_READY_IND);
	ClearProtocolEventHandler(MSG_ID_MEDIA_VID_PLAY_FINISH_IND);
	ClearProtocolEventHandler(MSG_ID_MEDIA_VID_PLAY_TIME_IND);

	/* re-enable keypad tone */
  	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);

	/* free buffer if play from file */
	if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_FILE)
	{
		coolsand_UI_free(g_vdoply_cntx.filename);
		g_vdoply_cntx.filename = NULL;
	}
	else if(g_vdoply_cntx.play_source == VDOPLY_PLAY_FROM_ID)
	{
		g_vdoply_cntx.filename	= NULL;
	}

	/* un-hook clam close msg */
	#if defined(__MMI_CLAMSHELL__) && !defined(__MMI_SLIDE__) && defined(MMI_ON_HARDWARE_P)
		SetInterruptEventHandler(NULL, NULL, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
	#endif /* __MMI_CLAMSHELL__ && __MMI_SLIDE__ && MMI_ON_HARDWARE_P */

	/* free play wnd layer */
	if(g_vdoply_cntx.play_wnd_layer_handle != GDI_LAYER_EMPTY_HANDLE)
	{
		gdi_layer_free(g_vdoply_cntx.play_wnd_layer_handle);
		g_vdoply_cntx.play_wnd_layer_handle = GDI_LAYER_EMPTY_HANDLE;
	}

	/* free osd layer resource */
	#ifdef __VDOPLY_FEATURE_FULLSCREEN__
	
		/****** free osd layer ******/
        if(g_vdoply_cntx.osd_layer_handle!=GDI_LAYER_EMPTY_HANDLE)
        { 
            gdi_layer_free(g_vdoply_cntx.osd_layer_handle);
            g_vdoply_cntx.osd_layer_handle = GDI_LAYER_EMPTY_HANDLE;
        }

		#ifdef APPMEM_SUPPORT
			/* with ASM */
			FreeMediaMemory((unsigned long)g_vdoply_cntx.osd_layer_buf_ptr); 
			g_vdoply_cntx.osd_layer_buf_ptr = NULL;
		#endif /* APPMEM_SUPPORT */
			
	#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
	
	/* exit sublcd */
#ifdef __MMI_SUBLCD__
	if(g_vdoply_cntx.is_sub_display)
	{
		GoBackSubLCDHistory();
		g_vdoply_cntx.is_sub_display = FALSE;
	}
#endif


	/* restore TV out mode */
	#ifdef __MMI_TVOUT__

		if(mdi_tvout_is_enable())
		{
			if(mmi_phnset_tvout_get_style() == SETTING_TVOUT_STYLE_FULLSCREEN)
			{
				mdi_tvout_set_owner(MDI_TV_OWNER_GDI);			
				
				/* restore back to start mode1 */
				mdi_tvout_set_mode(	MDI_TV_MODE_LCD_SCREEN,
											0,						/* layer_width */
											0);					/* layer_height */
			}
		}
	#endif /* __MMI_TVOUT__ */

#ifdef __MMI_STOP_SIGNAL_REPORT__
	NetWorkSignalStopReport(1); //Start the signal report because it was stoped at entry function
#endif
#if defined(__PROJECT_GALLITE_C01__) 
	VP_text_stop_scrolling();
#endif
}



/*****************************************************************************
* FUNCTION
*	 mmi_vdoply_get_speed_factor()
* DESCRIPTION
*   get speed factor 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static S16 mmi_vdoply_get_speed_factor(void)
{
	#ifdef __VDOPLY_FEATURE_SPEED__
		switch(g_vdoply_cntx.cur_status.speed)
		{
			case 0: 
				return VDOPLY_FEATURE_SPEED_0;
			case 1: 
				return VDOPLY_FEATURE_SPEED_1;
			case 2: 
				return VDOPLY_FEATURE_SPEED_2;
			case 3: 
				return VDOPLY_FEATURE_SPEED_3;
			case 4: 
				return VDOPLY_FEATURE_SPEED_4;
			case 5: 
				return VDOPLY_FEATURE_SPEED_5;		
			case 6: 
				return VDOPLY_FEATURE_SPEED_6;	
			case 7: 
				return VDOPLY_FEATURE_SPEED_7;	
			case 8: 
				return VDOPLY_FEATURE_SPEED_8;	

			default:
				ASSERT(0);
				return 0;
		}
	#else
		return 100; /* 1x play speed */
	#endif /* __VDOPLY_FEATURE_SPEED__ */
	
}


/*****************************************************************************
* FUNCTION
*	 mmi_vdoply_draw_sublcd_icon()
* DESCRIPTION
*   draw a sublcd icon.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__
static void mmi_vdoply_draw_sublcd_icon(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 image_width;
	S32 image_height;
	S32 lcd_width;
	S32 lcd_height;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	gdi_lcd_set_active(GDI_LCD_SUB_LCD_HANDLE);
	gdi_lcd_get_dimension(&lcd_width, &lcd_height);

	gdi_layer_reset_clip();

	gdi_layer_clear_background(GDI_COLOR_WHITE);
	gdi_image_get_dimension_id(IMG_ID_VDOPLY_SUBLCD_ICON, &image_width, &image_height);
	gdi_image_draw_id((lcd_width-image_width)>>1,(lcd_height-image_height)>>1,IMG_ID_VDOPLY_SUBLCD_ICON);
	gdi_layer_blt_base_layer(0,0, lcd_width-1, lcd_height-1);

	gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
}
#endif /* __MMI_SUBLCD__ */



/*****************************************************************************
* FUNCTION
*	 mmi_vdoply_entry_sublcd_screen()
* DESCRIPTION
*   entry sublcd screen, if still remain on standard MMI sublcd screen, time update will
*   redraw sublcd while mainlcd is previewing, will cause driver assert.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__
static void mmi_vdoply_entry_sublcd_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ExecSubLCDCurrExitHandler();
	ShowCategory331Screen();
	SetSubLCDExitHandler(mmi_vdoply_exit_sublcd_screen);

	/* draw a icon on sub */
	mmi_vdoply_draw_sublcd_icon();
	
	g_vdoply_cntx.is_sub_display = TRUE;
}
#endif /* __MMI_SUBLCD__ */



/*****************************************************************************
* FUNCTION
*	 mmi_vdoply_exit_sublcd_screen()
* DESCRIPTION
*   exit sublcd screen
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_SUBLCD__
static void mmi_vdoply_exit_sublcd_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   SubLCDHistoryNode SubLCDHistory;
	
	/* someone may exit/draw sublcd first then exit mainlcd, this may cause driver assert */
   /* also stop preview here */
	
	if( g_vdoply_cntx.state == VDOPLY_STATE_PLAY)
	{
		/* interrupt while playing, store play time var */
//		mdi_video_ply_get_cur_play_time(&g_vdoply_cntx.cur_status.play_time);
		mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);

		/* if is seeking, it is already stopped */
		if(!g_vdoply_cntx.is_seeking)
			mdi_video_ply_stop();

		g_vdoply_cntx.state = VDOPLY_STATE_IDLE;
	}

	/* stop seek skip frame timer */
	coolsand_UI_cancel_timer(mmi_vdoply_seek_key_release);
	
	/* exit sub */
	g_vdoply_cntx.is_sub_display = FALSE;
	
	SubLCDHistory.entryFuncPtr = mmi_vdoply_entry_sublcd_screen;
	AddSubLCDHistory(&SubLCDHistory);
}
#endif /* __MMI_SUBLCD__ */




/*****************************************************************************
*
* Initialization and Entry of Video Player Application                                                                  
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	mmi_vdoply_highlight_app
* DESCRIPTION
*  app hilight handler
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_highlight_app(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	TRACE_EF(g_sw_MED_Vid,"IMG_GLOBAL_OK=%d,IMG_STATUS=%d,IMG_VICON=%d",IMG_GLOBAL_OK,IMG_STATUS,IMG_VICON);

	SetLeftSoftkeyFunction(mmi_vdoply_entry_app,KEY_EVENT_UP);
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(mmi_vdoply_entry_app, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */	
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_init_app
* DESCRIPTION
*  init video player applcation, register highlight handler 
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdoply_init_app(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* init video player application highlight handler */
//	SetHiliteHandler(	MENU_ID_SNDREC_MAIN, mmi_vdoply_highlight_app);
	SetHiliteHandler(	MENU_ID_VDOPLY_APP, mmi_vdoply_highlight_app);

	SetHiliteHandler(	MENU_ID_VDOPLY_PLAY, mmi_vdoply_highlight_play);
	SetHiliteHandler(	MENU_ID_VDOPLY_STORAGE,	mmi_vdoply_highlight_set_storage);	

	SetHintHandler(MENU_ID_VDOPLY_STORAGE,	mmi_vdoply_hint_set_storage);

	#if defined(__MMI_AUDIO_PLAYER__) && defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)    
    SetHiliteHandler(MENU_ID_VDOPLY_BLUETOOTH_SETTING, mmi_vdoply_highlight_bluetooth_setting);    
     #endif

	#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
		SetHiliteHandler(	MAIN_MENU_OPTION_VDOPLY_APP, mmi_vdoply_highlight_app);
	#endif
	/* init variable */
	g_vdoply_cntx.is_sub_display 			= FALSE;
	g_vdoply_cntx.video_id					= 0;
	g_vdoply_cntx.filename					= NULL;	

	g_vdoply_cntx.base_layer_handle		= GDI_LAYER_EMPTY_HANDLE;
	g_vdoply_cntx.play_wnd_layer_handle	= GDI_LAYER_EMPTY_HANDLE;
	g_vdoply_cntx.osd_layer_handle 		= GDI_LAYER_EMPTY_HANDLE;

}


void mmi_vdo_get_display_rect(U16 *width, U16 *height)
{
	mmi_trace(TRUE,"Func: %s width:%d, height:%d", __FUNCTION__, g_vdoply_osd_cntx.play_wnd.rect.width, g_vdoply_osd_cntx.play_wnd.rect.height);

	*width = g_vdoply_osd_cntx.play_wnd.rect.width;
	*height = g_vdoply_osd_cntx.play_wnd.rect.height;
}
/*****************************************************************************
* FUNCTION
*	mmi_vdoply_exit_from_fmgr
* DESCRIPTION
*  callback function when exit from fmgr's browser
* PARAMETERS
*  filename		IN		not used
*  is_short		IN		is short name
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdoply_exit_from_fmgr(void* filename, int is_short)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	DeleteUptoScrID(SCR_ID_VDOPLY_APP);
	DeleteNHistory(1);	
	GoBackHistory();
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_exit_from_fmgr_emptry_folder
* DESCRIPTION
*  exit from fmgr when enter empty foloder
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
void mmi_vdoply_exit_from_fmgr_emptry_folder(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	DeleteUptoScrID(SCR_ID_VDOPLY_APP);
	DeleteNHistory(1);
	GoBackHistory();
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_entry_app
* DESCRIPTION
*  video player entry point 
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
extern mmi_phb_context_struct g_phb_cntx;
extern pwr_context_struct g_pwr_context;
extern SecuritySetupContext g_SecuritySetupContext;
extern int checkUsbstate(void);
extern VOID mmi_display_usb_busy(void);
extern BOOL is_phb_init_done(void);
extern BOOL checkVMOn(void);
extern VOID mmi_display_gprs_active(void) ;
void mmi_vdoply_entry_app(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8				buf_filepath[FMGR_PATH_BUFFER_SIZE];
	FS_HANDLE	file_handle;
	FMGR_FILTER	filter;
	S32			create_result;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	TBM_ENTRY(0x2A60);
   #if 0
	if(!is_phb_init_done())  //have no SIM or phb is ready
	{
	    //������ʼ�� stringid = 25803  modified by renyh  2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_PROGRESS, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		return;
	}
#endif
    if(HandleLowBatteryProcess())
    {
        mmi_trace(1,"low battery in vdo ply");
        return ;
    }

	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(mmi_vdoply_entry_app);
		return;
	}
   	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
	EntryNewScreen(SCR_ID_VDOPLY_APP, NULL, mmi_vdoply_entry_app, NULL);
#ifdef __MMI_FM_RADIO__
	mmi_fmrdo_power_on( FALSE );
	mdi_audio_set_background_handler(MDI_BACKGROUND_APP_FMR, NULL ); //chenhe enable for jessmine replay after interrupt

#endif // __MMI_FM_RADIO__

	/* load video's storage setting */
	mmi_vdorec_load_setting();
	mmi_vdorec_get_storage_file_path(buf_filepath);
	create_result = mmi_vdorec_create_file_dir(buf_filepath);

//	AnsiiToUnicodeString(buf_filepath,"D:/VIDEO");
	/* check if it is valid path */
	file_handle	= MMI_FS_Open((const UINT8 *)buf_filepath, FS_OPEN_DIR | FS_READ_ONLY);

#ifdef MMI_ON_WIN32
  /* zhuoxz,2009-5-27 */
  if (file_handle>=0)
  {
    file_handle = FS_FOLDER_HANDLE;
  }
#endif /*MMI_ON_WIN32*/

        // dingjian20071030 open Tcars
//       file_handle = FS_FOLDER_HANDLE;
	if(file_handle == FS_FOLDER_HANDLE)  //the folder exists
	{
		
		FMGR_FILTER_INIT(&filter);
		FMGR_FILTER_SET_VIDEO(&filter);

		mmi_fmgr_select_path_and_enter(	APP_VDOPLY, 
													FMGR_SELECT_BROWSE, 
													filter, 
													buf_filepath, 
													(PsExtFuncPtr)mmi_vdoply_exit_from_fmgr);
		
		/* check if device is busy or not */
		if (MMI_FS_GetDevStatus(buf_filepath[0], FS_FEATURE_STATE_ENUM) == FS_DEVICE_BUSY)
		{
			/*  mmi_fmgr_select_path_and_enter will also add screen to histry */
			DeleteUptoScrID(SCR_ID_VDOPLY_APP);
		
			/* Delete up to previous screen */
			DeleteNHistory(1);
		}
	}
	else
	{
		if(file_handle >= 0)   //if the file_handle is a file handle,close the file
		{
			MMI_FS_Close(file_handle);
		}
		if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_PHONE)
		{
			/* storage is phone */
			
			/* if storage is already on phone, but it is still failed, means phone's FAT is corrupted */
			if(create_result == FS_DISK_FULL)
			{
				DisplayPopup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			}
			else if(create_result == FS_ROOT_DIR_FULL)
			{
				DisplayPopup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			}			
			else
			{
				DisplayPopup((PU8)GetString(FMGR_FS_FAT_ALLOC_ERROR_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			}
	
			DeleteNHistory(1);
			return;
		}
		else
		{
			/* storage is card */
			
			/* photo's path is not exist, and card is write protected, cant create photo folder */
			if(create_result == FS_WRITE_PROTECTION)
			{
				DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_WRITE_PROTECTION), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			}
			else if(create_result == FS_DISK_FULL)
			{
				DisplayPopup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			}
			else if(create_result == FS_ROOT_DIR_FULL)
			{
				DisplayPopup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			}	
			else
			{
				DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			}

			#if 0     //modified for MP4 ֻ�ܴӼ��俨��ȡ���ݣ����ܴ��ֻ���ȡ
				g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_PHONE;
				mmi_vdorec_store_setting();
		
				DeleteUptoScrID(SCR_ID_VDOPLY_APP);
			#else
				DeleteNHistory(1);
			#endif 
			TBM_EXIT(0x2A60);
			return;

		}
	}
	TBM_EXIT(0x2A60);
}




/*****************************************************************************
*
* Select Prefered Storage                                                                
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	mmi_vdoply_hint_set_storage
* DESCRIPTION
*  storage's hint
* PARAMETERS
*  index		 IN		selection index
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_hint_set_storage(U16 index)
{

	S8 temp_str[100];
	AnsiiToUnicodeString(temp_str,(S8*)GetString( (U16)(STR_ID_VDOPLY_STORAGE_PHONE+g_vdorec_cntx.setting.storage) ));
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	memset((PS8)hintData[(index)], 0, sizeof(hintData[(index)]));
	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)GetString( (U16)(STR_ID_VDOPLY_STORAGE_PHONE+g_vdorec_cntx.setting.storage) ));
}

/*****************************************************************************
 * FUNCTION
 *  mmi_vdoply_highlight_bluetooth_setting
 * DESCRIPTION
 *  highlight set storage
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#if defined(__MMI_AUDIO_PLAYER__) && defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)    
static void mmi_vdoply_highlight_bluetooth_setting(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_audply_entry_bluetooth_settings, KEY_EVENT_UP);
    SetKeyHandler(mmi_audply_entry_bluetooth_settings, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}
#endif /* defined(__MMI_AUDIO_PLAYER__) && defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)  */



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_highlight_set_storage
* DESCRIPTION
*  highlight set storage
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_highlight_set_storage(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_vdoply_entry_set_storage_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_vdoply_entry_set_storage_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
}


/*****************************************************************************
* FUNCTION
*	mmi_vdoply_set_storage_value
* DESCRIPTION
*  set storage value
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_set_storage_value(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8				buf_UCS2[FMGR_PATH_BUFFER_SIZE];
  	S8				buf_filepath[FMGR_PATH_BUFFER_SIZE];
	S32			ret_drv=-1;
	S8				buf[64];
	FS_HANDLE	file_handle;
	S32			create_result;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	TBM_ENTRY(0x2A6F);
        /*+ zhouqin modify for dual t-flash card 20110503*/
        #if defined(DUAL_TFLASH_SUPPORT)
        g_vdorec_cntx.setting.storage = GetCurrHiliteID()+1;
        #else
        g_vdorec_cntx.setting.storage = GetCurrHiliteID();
        #endif
        /*- zhouqin modify for dual t-flash card 20110503*/
	if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_PHONE)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);
	}
	else if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_MEMORY_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE , 1, FS_NO_ALT_DRIVE);
	}
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    else if(g_vdorec_cntx.setting.storage == VDOREC_SETTING_STORAGE_MEMORY_SEC_CARD)
	{
		ret_drv	= MMI_FS_GetDrive(FS_DRIVE_TFCARD_SECOND , 1, FS_NO_ALT_DRIVE);
	}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
	else
	{
		ASSERT(0);
	}

	if(ret_drv >= 0)
	{
		sprintf(buf, "%c:/", (U8)ret_drv);
		AnsiiToUnicodeString(buf_UCS2, buf);

		file_handle = MMI_FS_Open((const UINT8 *)buf_UCS2, FS_OPEN_DIR | FS_READ_ONLY);

		if(file_handle ==FS_FOLDER_HANDLE)
		{
			/* drive is avaiable */

			/* check if photo path exist or not */
			mmi_vdorec_get_storage_file_path(buf_filepath);
			create_result = mmi_vdorec_create_file_dir(buf_filepath);

			if(create_result == FS_WRITE_PROTECTION)
			{
				DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_WRITE_PROTECTION), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_PHONE;
			}
			else if(create_result == FS_ROOT_DIR_FULL)
			{
				DisplayPopup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_PHONE;
			}
			else if(create_result == FS_DISK_FULL)
			{
				DisplayPopup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 0, ST_NOTIFYDURATION, WARNING_TONE);
				g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_PHONE;
			}			
			else
			{
				g_vdorec_cntx.is_storage_path_changed = TRUE;
				DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
				DeleteUptoScrID(SCR_ID_VDOPLY_APP);
			}
		}
		else
		{
			if(file_handle>=0)
			{
				MMI_FS_Close(file_handle);
			}
			DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
			/* set phone internal space as default */
			g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_PHONE;
		}
	}
	else	
	{
		/* get drive error  */
		DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);

		/*+ zhouqin modify for dual t-flash card 20110503*/
		#ifndef  DUAL_TFLASH_SUPPORT
		/* set phone internal space as default */
		g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_PHONE;
		#else
		g_vdorec_cntx.setting.storage = VDOREC_SETTING_STORAGE_MEMORY_CARD;
		#endif
		/*- zhouqin modify for dual t-flash card 20110503*/
	}
	
	mmi_vdorec_store_setting();

	TBM_EXIT(0x2A6F);
}



/*****************************************************************************
* FUNCTION
*	mmi_vdoply_entry_set_storage_screen
* DESCRIPTION
*  enter set storage screen
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static void mmi_vdoply_entry_set_storage_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	
	EntryNewScreen(SCR_ID_VDOPLY_STORAGE, NULL, mmi_vdoply_entry_set_storage_screen, NULL);
   
	/*+ zhouqin modify for dual t-flash card 20110503*/
	#if defined(DUAL_TFLASH_SUPPORT)
	mmi_frm_hide_menu_item(MENU_ID_VDOPLY_STORAGE_PHONE);
	#endif
	/*- zhouqin modify for dual t-flash card 20110503*/
	/* load all settings */
	mmi_vdorec_load_setting();

	/*+ zhouqin modify for dual t-flash card 20110503*/
	#if defined(DUAL_TFLASH_SUPPORT)
	nItems = GetNumOfChild_Ext(MENU_ID_VDOPLY_STORAGE);
	GetSequenceStringIds_Ext(MENU_ID_VDOPLY_STORAGE,ItemList);
	GetSequenceImageIds_Ext(MENU_ID_VDOPLY_STORAGE,ItemIcons);
	#else
	nItems = GetNumOfChild(MENU_ID_VDOPLY_STORAGE);
	GetSequenceStringIds(MENU_ID_VDOPLY_STORAGE,ItemList);
	GetSequenceImageIds(MENU_ID_VDOPLY_STORAGE,ItemIcons);
	#endif
	/*- zhouqin modify for dual t-flash card 20110503*/
	SetParentHandler(MENU_ID_VDOPLY_STORAGE);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory11Screen(	STR_ID_VDOPLY_STORAGE, 
									MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									nItems, 
									ItemList,
									/*+ zhouqin modify for dual t-flash card 20110503*/
									#if defined(DUAL_TFLASH_SUPPORT)
									g_vdorec_cntx.setting.storage-1,				/* current index */
									#else
									g_vdorec_cntx.setting.storage,				/* current index */
									#endif
									/*- zhouqin modify for dual t-flash card 20110503*/
									NULL);

	SetLeftSoftkeyFunction(mmi_vdoply_set_storage_value, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
*
*	Touch Screen Support
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_vdoply_touch_scr_pen_down_hdlr()
* DESCRIPTION
*   touch screen hdlr when pen down
* PARAMETERS
*	 pos		IN			pen down position
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
static void mmi_vdoply_touch_scr_pen_down_hdlr(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	S32 shift;
	S32 ratio;

    
	#if 0//defined(__VDOPLY_FEATURE_FULLSCREEN__) || defined(__VDOPLY_FEATURE_HORIZONTAL_VIEW__)
		S32	 	tmp_x;
		S32		tmp_y;
	#endif /* __VDOPLY_FEATURE_FULLSCREEN__ || __VDOPLY_FEATURE_HORIZONTAL_VIEW__ */

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	TRACE_EF(g_sw_MED_Vid,"coordinates of touch pen (%d,%d)",pos.x,pos.y);
	if( (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_IDLE) || 
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PAUSE) || 
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY) || 
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_SNAPSHOT) ||
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_SEEKING)  ||
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY_SEEKING) )
	{
        //TODO:This block seems doesn't work, check it!
		#if 0//def __VDOPLY_FEATURE_FULLSCREEN__

			/* cord tranfrom, since already rotate */		
			tmp_x = pos.x;
			tmp_y = pos.y;			
			pos.x = tmp_y;
			pos.y = (UI_device_width-1)-tmp_x;

			
			if(g_vdoply_cntx.fullscr_osd_opacity == 0)
			{
				/* wake up osd */
				mmi_vdoply_fullscr_reset_osd_fade(TRUE);
				mmi_vdoply_blt_screen();
				return;
			}

			/* show full screen osd, do not allow it sleep */
			mmi_vdoply_fullscr_reset_osd_fade(FALSE);
			coolsand_UI_cancel_timer(mmi_vdoply_fullscr_osd_fade_cyclic); 
		
			/* volume inc */
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_volume_inc))
			{
				if(g_vdoply_cntx.cur_status.volume >= LEVEL7)
					return;
				
				g_vdoply_cntx.full_touch_volume_inc.is_press = TRUE;
				mmi_vdoply_vol_up_key_press();

				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_VOLUME_INC;			
				return;				
			}	

			/* volume dec */
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_volume_dec))
			{
				if(g_vdoply_cntx.cur_status.volume == LEVEL1)
					return;
				
				g_vdoply_cntx.full_touch_volume_dec.is_press = TRUE;
				mmi_vdoply_vol_down_key_press();

				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_VOLUME_DEC;					
				return;		
			}	

#ifdef __VDOPLY_FEATURE_SPEED__	
			/* speed inc */
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_speed_inc))
			{
				if(g_vdoply_cntx.cur_status.speed >= VDOPLY_FEATURE_SPEED_COUNT-1)
					return;
				
				g_vdoply_cntx.full_touch_speed_inc.is_press = TRUE;
				mmi_vdoply_speed_inc_key_press();
				
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_SPEED_INC;	
				return;
			}	

			/* speed dec */
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_speed_dec))
			{
				if(g_vdoply_cntx.cur_status.speed == 0)
					return;
				
				g_vdoply_cntx.full_touch_speed_dec.is_press = TRUE;
				mmi_vdoply_speed_dec_key_press();

				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_SPEED_DEC;	
				return;		
			}	
#endif
			/* progress inc */
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_progress_inc))
			{
				if((g_vdoply_cntx.state != VDOPLY_STATE_FULLSCR_SEEKING) && 
					(g_vdoply_cntx.state != VDOPLY_STATE_FULLSCR_PLAY_SEEKING))
				{					
					/* seek foward, state change from idle to pause if nessary */
					if(g_vdoply_cntx.state == VDOPLY_STATE_IDLE)
						g_vdoply_cntx.state = VDOPLY_STATE_PAUSE;
					
					g_vdoply_cntx.full_touch_progress_inc.is_press = TRUE;
					mmi_vdoply_seek_foward_key_press();

					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_PROGRESS_INC;	
				}
				return;		
			}	

			/* progress dec */
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_progress_dec))
			{
				if((g_vdoply_cntx.state != VDOPLY_STATE_FULLSCR_SEEKING) && 
					(g_vdoply_cntx.state != VDOPLY_STATE_FULLSCR_PLAY_SEEKING))
				{						
					g_vdoply_cntx.full_touch_progress_dec.is_press = TRUE;
					mmi_vdoply_seek_backward_key_press();

					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_PROGRESS_DEC;	
				}
				return;		
			}	

			/* progress bar */
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_progress_bar))
			{
				if((!g_vdoply_cntx.is_seeking) &&
					(g_vdoply_cntx.state != VDOPLY_STATE_FULLSCR_SEEKING) && 
					(g_vdoply_cntx.state != VDOPLY_STATE_FULLSCR_PLAY_SEEKING))
				{		
					g_vdoply_cntx.full_touch_progress_bar.is_press = TRUE;

					g_vdoply_cntx.is_seeking = TRUE;

					/* seek , state change from idle to pause if nessary */
					if(g_vdoply_cntx.state == VDOPLY_STATE_IDLE)
						g_vdoply_cntx.state = VDOPLY_STATE_PAUSE;
					
					if(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY)
					{
						coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
						mdi_video_ply_stop();
					}

					shift = pos.x - g_vdoply_osd_cntx.full_panel.progress.empty.offset_x;
					ratio	= (shift<<10)/g_vdoply_cntx.fullscr_progress.fill_width;

					g_vdoply_cntx.cur_status.play_time = (g_vdoply_cntx.video_info.total_time_duration*ratio)>>10;
				
					/* play ratio scale is 1/width of progress */
					g_vdoply_cntx.cur_status.play_ratio = 	(U32)(g_vdoply_cntx.cur_status.play_time*	\
																				g_vdoply_cntx.fullscr_progress.fill_width/	\
																				g_vdoply_cntx.video_info.total_time_duration);
				
					mmi_vdoply_draw_fullscr_osd();
					mmi_vdoply_blt_screen();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_PROGRESS_BAR;	
				}
				return;		
			}	

			/* play/pause */
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_play))
			{
				g_vdoply_cntx.full_touch_play.is_press = TRUE;
				mmi_vdoply_draw_fullscr_osd();
				mmi_vdoply_blt_screen();

				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_PLAY;	
				return;		
			}	

			/* stop */
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_stop))
			{
				g_vdoply_cntx.full_touch_stop.is_press = TRUE;
				mmi_vdoply_draw_fullscr_osd();
				mmi_vdoply_blt_screen();

				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_STOP;	
				return;		
			}	

			/* rsk */
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_rsk))
			{
				mmi_vdoply_rsk_press();
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_RSK;	
				return;		
			}	
			
			/* lsk */
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_lsk))
			{
				mmi_vdoply_lsk_press();
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_LSK;	
				return;		
			}	
			
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
	}
	else
	{
		#if defined(__VDOPLY_FEATURE_HORIZONTAL_VIEW__)
			/* cord tranfrom, since already rotate */		
			tmp_x = pos.x;
			tmp_y = pos.y;			
			pos.x = tmp_y;
			pos.y = (UI_device_width-1)-tmp_x;
		#endif /* __VDOPLY_FEATURE_HORIZONTAL_VIEW__ */
		if( (g_vdoply_cntx.state == VDOPLY_STATE_SEEKING) || 
		 	 (g_vdoply_cntx.state == VDOPLY_STATE_PLAY_SEEKING))
		{
			/* if seeking, touch shall not work */
			return;
		}
		/* lsk */
		if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_lsk))
		{
			if(g_vdoply_cntx.state != VDOPLY_STATE_PLAY)
			{
				mmi_vdoply_lsk_press();
			}
			g_vdoply_cntx.touch_lsk.is_press = TRUE;
			g_vdoply_cntx.touch_object = VDOPLY_TOUCH_LSK;	
			return;		
		}	
		/* rsk */
		if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_rsk))
		{
		if(g_vdoply_cntx.state != VDOPLY_STATE_PLAY)
			{
			mmi_vdoply_rsk_press();
			}
			g_vdoply_cntx.touch_rsk.is_press = TRUE;
			g_vdoply_cntx.touch_object = VDOPLY_TOUCH_RSK;	
			return;		
		}	
		if( (g_vdoply_cntx.state == VDOPLY_STATE_OPENING) || 
		 	 (g_vdoply_cntx.state == VDOPLY_STATE_INTERRUPTED_OPENING) || 
			 (g_vdoply_cntx.state == VDOPLY_STATE_INTERRUPTED_SEEKING))
		 {
			/* if not open and seek done, cant not pass to other touch event */		 
			return;
		 }
		/* volume inc */
		if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_volume_inc))
		{
			if(g_vdoply_cntx.cur_status.volume > LEVEL7)
				return;
			
			g_vdoply_cntx.touch_volume_inc.is_press = TRUE;
			mmi_vdoply_vol_up_key_press();

			g_vdoply_cntx.touch_object = VDOPLY_TOUCH_VOLUME_INC;			
			return;				
		}	

		/* volume dec */
		if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_volume_dec))
		{
			if(g_vdoply_cntx.cur_status.volume == LEVEL1)
				return;
			
			g_vdoply_cntx.touch_volume_dec.is_press = TRUE;
			mmi_vdoply_vol_down_key_press();

			g_vdoply_cntx.touch_object = VDOPLY_TOUCH_VOLUME_DEC;					
			return;		
		}	
#ifdef __VDOPLY_FEATURE_SPEED__
		/* speed inc */
		if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_speed_inc))
		{
			if(g_vdoply_cntx.cur_status.speed >= VDOPLY_FEATURE_SPEED_COUNT-1)
				return;
			
			g_vdoply_cntx.touch_speed_inc.is_press = TRUE;
			mmi_vdoply_speed_inc_key_press();
			
			g_vdoply_cntx.touch_object = VDOPLY_TOUCH_SPEED_INC;	
			return;
		}	

		/* speed dec */
		if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_speed_dec))
		{
			if(g_vdoply_cntx.cur_status.speed == 0)
				return;
			
			g_vdoply_cntx.touch_speed_dec.is_press = TRUE;
			mmi_vdoply_speed_dec_key_press();

			g_vdoply_cntx.touch_object = VDOPLY_TOUCH_SPEED_DEC;	
			return;		
		}	
#endif
		/* progress inc */
		if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_progress_inc))
		{
			TRACE_EF(g_sw_MED_Vid,"progress bar touch_progress_inc(offset_x,offset_y,width,height): (%d,%d,%d,%d)",g_vdoply_cntx.touch_progress_inc.offset_x,g_vdoply_cntx.touch_progress_inc.offset_y,g_vdoply_cntx.touch_progress_inc.width,g_vdoply_cntx.touch_progress_inc.height);
			if((g_vdoply_cntx.state != VDOPLY_STATE_SEEKING) && 
				(g_vdoply_cntx.state != VDOPLY_STATE_PLAY_SEEKING) &&
				(g_vdoply_cntx.state != VDOPLY_STATE_INTERRUPTED_SEEKING))
			{
				g_vdoply_cntx.touch_progress_inc.is_press = TRUE;
				mmi_vdoply_seek_foward_key_press();

				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_PROGRESS_INC;	
			}
			return;		
		}	

		/* progress dec */
		if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_progress_dec))
		{
			TRACE_EF(g_sw_MED_Vid,"progress bar touch_progress_dec(offset_x,offset_y,width,height): (%d,%d,%d,%d)",g_vdoply_cntx.touch_progress_dec.offset_x,g_vdoply_cntx.touch_progress_dec.offset_y,g_vdoply_cntx.touch_progress_dec.width,g_vdoply_cntx.touch_progress_dec.height);
			if((g_vdoply_cntx.state != VDOPLY_STATE_SEEKING) && 
				(g_vdoply_cntx.state != VDOPLY_STATE_PLAY_SEEKING) &&
				(g_vdoply_cntx.state != VDOPLY_STATE_INTERRUPTED_SEEKING))
			{	
				g_vdoply_cntx.touch_progress_dec.is_press = TRUE;
				mmi_vdoply_seek_backward_key_press();

				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_PROGRESS_DEC;	
			}
			return;		
		}	

		/* progress bar */
		if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_progress_bar))
		{
			TRACE_EF(g_sw_MED_Vid,"progress bar touch_progress_bar (%d,%d,%d,%d)",g_vdoply_cntx.touch_progress_bar.offset_x,g_vdoply_cntx.touch_progress_bar.offset_y,g_vdoply_cntx.touch_progress_bar.width,g_vdoply_cntx.touch_progress_bar.height);
			if((!g_vdoply_cntx.is_seeking) &&
				(g_vdoply_cntx.state != VDOPLY_STATE_SEEKING) && 
				(g_vdoply_cntx.state != VDOPLY_STATE_PLAY_SEEKING) &&
				(g_vdoply_cntx.state != VDOPLY_STATE_INTERRUPTED_SEEKING))
			{		
				g_vdoply_cntx.touch_progress_bar.is_press = TRUE;

				g_vdoply_cntx.is_seeking = TRUE;
				
				if(g_vdoply_cntx.state == VDOPLY_STATE_PLAY)
				{
					coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
					mdi_video_ply_stop();
				}

				shift = pos.x - g_vdoply_osd_cntx.panel.progress.empty.offset_x;
				ratio	= (shift<<10)/g_vdoply_cntx.progress.fill_width;

				g_vdoply_cntx.cur_status.play_time = (g_vdoply_cntx.video_info.total_time_duration*ratio)>>10;
			
				/* play ratio scale is 1/width of progress */
				g_vdoply_cntx.cur_status.play_ratio = 	(U32)(g_vdoply_cntx.cur_status.play_time*	\
																			g_vdoply_cntx.progress.fill_width/	\
																			g_vdoply_cntx.video_info.total_time_duration);
			
				mmi_vdoply_draw_panel_progress();
				mmi_vdoply_draw_panel_timer();
				mmi_vdoply_blt_part();
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_PROGRESS_BAR;	
			}
			return;		
		}	

		/* play/pause */
		if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_play))
		{
			g_vdoply_cntx.touch_play.is_press = TRUE;
			//mmi_vdoply_draw_panel_state();
			//mmi_vdoply_blt_part();

			g_vdoply_cntx.touch_object = VDOPLY_TOUCH_PLAY;	
			return;		
		}	

		/* stop */
		if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_stop))
		{
			g_vdoply_cntx.touch_stop.is_press = TRUE;
			//mmi_vdoply_draw_panel_state();
			//mmi_vdoply_blt_part();

			g_vdoply_cntx.touch_object = VDOPLY_TOUCH_STOP;	
			return;		
		}	

		/* snapshot */
		#ifdef __VDOPLY_FEATURE_SNAPSHOT__
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_snapshot))
			{
				g_vdoply_cntx.touch_snapshot.is_press = TRUE;
				mmi_vdoply_draw_panel_state();
				mmi_vdoply_blt_screen();

				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_SNAPSHOT;	
				return;		
			}	
		#endif /* __VDOPLY_FEATURE_SNAPSHOT__ */

		/* fullscreen */
		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			if(mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_fullscreen))
			{
				g_vdoply_cntx.touch_fullscreen.is_press = TRUE;
				if(g_vdoply_cntx.state != VDOPLY_STATE_PLAY)
			{
				mmi_vdoply_draw_panel_state();
				mmi_vdoply_blt_screen();
					}

				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_FULLSCREEN;	
				return;		
			}	
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
		

	}
	

	
}
#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ & __MMI_TOUCH_SCREEN__ */



/*****************************************************************************
* FUNCTION
*	 mmi_vdoply_touch_scr_pen_up_hdlr()
* DESCRIPTION
*   touch screen hdlr when pen up
* PARAMETERS
*	 pos		IN			pen up position
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
static void mmi_vdoply_touch_scr_pen_up_hdlr(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	U32 			cur_time;
	U32			total_time;
	MDI_RESULT	ret;

	#ifdef __MMI_MAINLCD_220X176__
	#elif defined __VDOPLY_FEATURE_FULLSCREEN__
		S32	 	tmp_x = 0;
		S32		tmp_y = 0;
	#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	if( (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_IDLE) || 
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PAUSE) || 
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY) || 
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_SNAPSHOT) ||
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_SEEKING)  ||
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY_SEEKING) )
	{
		#ifdef __MMI_MAINLCD_220X176__
			mmi_vdoply_toggle_fullscreen_display_timer();
		#elif defined __VDOPLY_FEATURE_FULLSCREEN__
			/* cord tranfrom, since already rotate */		
			tmp_x = pos.x;
			tmp_y = pos.y;			
			pos.x = tmp_y;
			pos.y = (UI_device_width-1)-tmp_x;

			/*  let osd can sleep */
			//mmi_vdoply_fullscr_reset_osd_fade(TRUE);
		
			switch(g_vdoply_cntx.touch_object)
			{
				case VDOPLY_TOUCH_VOLUME_INC:
					g_vdoply_cntx.full_touch_volume_inc.is_press = FALSE;
					g_vdoply_cntx.is_vol_inc_key_pressed = FALSE;
						
					mmi_vdoply_draw_fullscr_osd();
					mmi_vdoply_blt_screen();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					break;
			
				case VDOPLY_TOUCH_VOLUME_DEC:
					g_vdoply_cntx.full_touch_volume_dec.is_press = FALSE;
					g_vdoply_cntx.is_vol_dec_key_pressed = FALSE;

					mmi_vdoply_draw_fullscr_osd();
					mmi_vdoply_blt_screen();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					break;
			
				case VDOPLY_TOUCH_SPEED_INC:
					g_vdoply_cntx.full_touch_speed_inc.is_press = FALSE;
					g_vdoply_cntx.is_speed_inc_key_pressed = FALSE;
					
					mmi_vdoply_draw_fullscr_osd();
					mmi_vdoply_blt_screen();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					break;

				case VDOPLY_TOUCH_SPEED_DEC:
					g_vdoply_cntx.full_touch_speed_dec.is_press = FALSE;
					g_vdoply_cntx.is_speed_dec_key_pressed = FALSE;

					mmi_vdoply_draw_fullscr_osd();
					mmi_vdoply_blt_screen();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					break;

				case VDOPLY_TOUCH_PROGRESS_INC:
					g_vdoply_cntx.full_touch_progress_inc.is_press = FALSE;
						
					mmi_vdoply_seek_foward_key_release();
							
					mmi_vdoply_draw_fullscr_osd();
					mmi_vdoply_blt_screen();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					break;

				case VDOPLY_TOUCH_PROGRESS_DEC:
					g_vdoply_cntx.full_touch_progress_dec.is_press = FALSE;

					mmi_vdoply_seek_backward_key_release();
				
					mmi_vdoply_draw_fullscr_osd();
					mmi_vdoply_blt_screen();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					break;

				case VDOPLY_TOUCH_PROGRESS_BAR:
					g_vdoply_cntx.full_touch_progress_bar.is_press = FALSE;

					g_vdoply_cntx.is_seeking = FALSE;

					/* seek and draw frame */
					ret = mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																						g_vdoply_cntx.base_layer_handle,
																						mmi_vdoply_seek_result_hdlr);	

					/* error check */
					if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
						return;	

					mmi_vdoply_blt_screen();

					if(g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY)
					{
						ret = mmi_vdoply_play_video();
				
						/* error check */
						if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
							return;	

						/* start digital timer */
						g_vdoply_cntx.cur_status.start_play_time = g_vdoply_cntx.cur_status.play_time;
						mmi_vdoply_player_timer_cyclic();				
					}
								
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					break;
			
				case VDOPLY_TOUCH_PLAY:
					g_vdoply_cntx.full_touch_play.is_press = FALSE;

					/* stop seek key timer */
					if(g_vdoply_cntx.is_seeking)
						coolsand_UI_cancel_timer(mmi_vdoply_seek_key_release);

					/* stop update time timer */
					coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
					
					switch(g_vdoply_cntx.state)
					{
						case VDOPLY_STATE_FULLSCR_PAUSE:		/* resume */			
						case VDOPLY_STATE_FULLSCR_IDLE:		/* play */
							mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PLAY);
							break;

						case VDOPLY_STATE_FULLSCR_PLAY:		/* pause */
							/* if is seeking in play state, the video is already stopped */
							if(!g_vdoply_cntx.is_seeking)
								mdi_video_ply_pause();
							/* get latest time */
//							mdi_video_ply_get_cur_play_time(&g_vdoply_cntx.cur_status.play_time);
							mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);

							/* re-calc paly ratio for progress display */
							cur_time 	= g_vdoply_cntx.cur_status.play_time;
							total_time 	= g_vdoply_cntx.video_info.total_time_duration;
							g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.fullscr_progress.fill_width/total_time);
							
							mmi_vdoply_draw_fullscr_osd();
							mmi_vdoply_blt_screen();
						
							
							
							mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_PAUSE);
							break;
						
					}
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;
					break;
			
				case VDOPLY_TOUCH_STOP:
					g_vdoply_cntx.full_touch_stop.is_press = FALSE;

					mmi_vdoply_draw_fullscr_osd();
					mmi_vdoply_blt_screen();

					/* if is seeking in play state, the video is already stopped */
					if(!g_vdoply_cntx.is_seeking)
						mdi_video_ply_stop();

					mmi_vdoply_player_enter_state(VDOPLY_STATE_FULLSCR_IDLE);
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;
					break;

			
				case VDOPLY_TOUCH_RSK:
					/* if move out */
					mmi_vdoply_rsk_release();
						
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					break;
					
				case VDOPLY_TOUCH_LSK:
					/* if move out */
					mmi_vdoply_lsk_release();
	
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					break;
					
			}
			
		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

	}
	else
	{
		/* not fullscreen */
		switch(g_vdoply_cntx.touch_object)
		{
			case VDOPLY_TOUCH_VOLUME_INC:
			if(g_vdoply_cntx.state != VDOPLY_STATE_PLAY)
				{
				g_vdoply_cntx.touch_volume_inc.is_press = FALSE;
				g_vdoply_cntx.is_vol_inc_key_pressed = FALSE;
				mmi_vdoply_draw_panel_volume();
				mmi_vdoply_blt_part();
				}
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				break;
		
			case VDOPLY_TOUCH_VOLUME_DEC:
			if(g_vdoply_cntx.state != VDOPLY_STATE_PLAY)
				{
				g_vdoply_cntx.touch_volume_dec.is_press = FALSE;
				g_vdoply_cntx.is_vol_dec_key_pressed = FALSE;
				mmi_vdoply_draw_panel_volume();
				mmi_vdoply_blt_part();
				}
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				break;
		
			case VDOPLY_TOUCH_SPEED_INC:
				g_vdoply_cntx.touch_speed_inc.is_press = FALSE;
				g_vdoply_cntx.is_speed_inc_key_pressed = FALSE;
				
				mmi_vdoply_draw_panel_speed();
				mmi_vdoply_blt_part();
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				break;

			case VDOPLY_TOUCH_SPEED_DEC:
				g_vdoply_cntx.touch_speed_dec.is_press = FALSE;
				g_vdoply_cntx.is_speed_dec_key_pressed = FALSE;

				mmi_vdoply_draw_panel_speed();
				mmi_vdoply_blt_part();
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				break;

			case VDOPLY_TOUCH_PROGRESS_INC:
				g_vdoply_cntx.touch_progress_inc.is_press = FALSE;
				mmi_vdoply_seek_foward_key_release();
						
				mmi_vdoply_draw_panel_progress();
				mmi_vdoply_blt_part();
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				break;

			case VDOPLY_TOUCH_PROGRESS_DEC:
				g_vdoply_cntx.touch_progress_dec.is_press = FALSE;
					
				mmi_vdoply_seek_backward_key_release();
				
				mmi_vdoply_draw_panel_progress();
				mmi_vdoply_blt_part();
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				break;

			case VDOPLY_TOUCH_PROGRESS_BAR:
				g_vdoply_cntx.touch_progress_bar.is_press = FALSE;

				g_vdoply_cntx.is_seeking = FALSE;

				/* seek and draw frame */
//				ret = mdi_video_ply_seek_and_get_frame(g_vdoply_cntx.cur_status.play_time,
//																	g_vdoply_cntx.play_wnd_layer_handle);

				ret = mdi_video_ply_non_block_seek_and_get_frame(	g_vdoply_cntx.cur_status.play_time,
																					g_vdoply_cntx.base_layer_handle,
																					mmi_vdoply_seek_result_hdlr);	
				/* error check */
				if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
					return;	

//				mmi_vdoply_blt_part();

				if(g_vdoply_cntx.state == VDOPLY_STATE_PLAY)
				{
					ret = mmi_vdoply_play_video();
			
					/* error check */
					if(mmi_vdoply_check_and_display_error_popup(ret)== TRUE) /* ture means has error popup */
						return;	

					/* start digital timer */
					g_vdoply_cntx.cur_status.start_play_time = g_vdoply_cntx.cur_status.play_time;
					mmi_vdoply_player_timer_cyclic();				
				}
				if(g_vdoply_cntx.state == VDOPLY_STATE_IDLE)
				{
					mmi_vdoply_player_enter_state(VDOPLY_STATE_PAUSE);
				}
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				break;
		
			case VDOPLY_TOUCH_PLAY:
				g_vdoply_cntx.touch_play.is_press = FALSE;

				/* stop seek key timer */
				if(g_vdoply_cntx.is_seeking)
					coolsand_UI_cancel_timer(mmi_vdoply_seek_key_release);

				/* stop update time timer */
				coolsand_UI_cancel_timer(mmi_vdoply_player_timer_cyclic);
				
				switch(g_vdoply_cntx.state)
				{
					case VDOPLY_STATE_INTERRUPTED_PAUSE:
					case VDOPLY_STATE_PAUSE:	/* resume */			
					case VDOPLY_STATE_IDLE:		/* play */
						mmi_vdoply_player_enter_state(VDOPLY_STATE_PLAY);
						break;

					case VDOPLY_STATE_PLAY:		/* pause */
						/* if is seeking in play state, the video is already stopped */
						if(!g_vdoply_cntx.is_seeking)
							mdi_video_ply_pause();
						/* get latest time */
//						mdi_video_ply_get_cur_play_time(&g_vdoply_cntx.cur_status.play_time);
						mdi_video_get_cur_play_time_req(mmi_vdoply_play_time_result_hdlr);

						/* re-calc paly ratio for progress display */
						cur_time 	= g_vdoply_cntx.cur_status.play_time;
						total_time 	= g_vdoply_cntx.video_info.total_time_duration;
						g_vdoply_cntx.cur_status.play_ratio = (U32)(cur_time*g_vdoply_cntx.progress.fill_width/total_time);
						
						mmi_vdoply_draw_panel_progress();
						mmi_vdoply_blt_part();
					
						
						
						mmi_vdoply_player_enter_state(VDOPLY_STATE_PAUSE);
						break;
					
				}
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;
				break;
		
			case VDOPLY_TOUCH_STOP:
				g_vdoply_cntx.touch_stop.is_press = FALSE;

				mmi_vdoply_draw_panel_state();
				mmi_vdoply_blt_part();

				/* if is seeking in play state, the video is already stopped */
				if(!g_vdoply_cntx.is_seeking)
					mdi_video_ply_stop();

				mmi_vdoply_player_enter_state(VDOPLY_STATE_IDLE);
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;
				break;

			/* snapshot */
			#ifdef __VDOPLY_FEATURE_SNAPSHOT__
			
				case VDOPLY_TOUCH_SNAPSHOT:
					g_vdoply_cntx.touch_snapshot.is_press = FALSE;

					mmi_vdoply_draw_panel_state();
					mmi_vdoply_blt_screen();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;
					mmi_vdoply_player_snapshot();
					break;
					
			#endif /* __VDOPLY_FEATURE_SNAPSHOT__ */

			/* fullscreen */
			#ifdef __VDOPLY_FEATURE_FULLSCREEN__
			
				case VDOPLY_TOUCH_FULLSCREEN:
						g_vdoply_cntx.touch_fullscreen.is_press = FALSE;

						mmi_vdoply_draw_panel_state();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;
						mmi_vdoply_toggle_fullscreen_display();
						break;
						
			#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

			case VDOPLY_TOUCH_RSK:
				g_vdoply_cntx.touch_rsk.is_press = FALSE;
				mmi_vdoply_rsk_release();
					
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				break;
			case VDOPLY_TOUCH_LSK:
				/* if move out */
				g_vdoply_cntx.touch_lsk.is_press = FALSE;
				mmi_vdoply_lsk_release();
					
				g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				break;

		}

	}


	
}
#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ & __MMI_TOUCH_SCREEN__ */


/*****************************************************************************
* FUNCTION
*	 mmi_vdoply_touch_scr_pen_move_hdlr()
* DESCRIPTION
*   touch screen hdlr when pen move
* PARAMETERS
*	 pos		IN			pen move position
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__VDOPLY_FEATURE_TOUCH_SCREEN__n) && defined(__MMI_TOUCH_SCREEN__)
static void mmi_vdoply_touch_scr_pen_move_hdlr(mmi_pen_point_struct pos)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	S32 			shift;
	S32 			ratio;
		
	#if defined(__VDOPLY_FEATURE_FULLSCREEN__) || defined(__VDOPLY_FEATURE_HORIZONTAL_VIEW__)
		S32	 	tmp_x;
		S32		tmp_y;
	#endif /* __VDOPLY_FEATURE_FULLSCREEN__ || __VDOPLY_FEATURE_HORIZONTAL_VIEW__ */
	
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	if(g_vdoply_cntx.touch_object == VDOPLY_TOUCH_NONE)
		return;


	if( (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_IDLE) || 
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PAUSE) || 
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY) || 
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_SNAPSHOT) ||
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_SEEKING)  ||
		 (g_vdoply_cntx.state == VDOPLY_STATE_FULLSCR_PLAY_SEEKING) )
	{
		#ifdef __VDOPLY_FEATURE_FULLSCREEN__
		
			/* cord tranfrom, since already rotate */		
			tmp_x = pos.x;
			tmp_y = pos.y;			
			pos.x = tmp_y;
			pos.y = (UI_device_width-1)-tmp_x;

			
			switch(g_vdoply_cntx.touch_object)
			{
				case VDOPLY_TOUCH_VOLUME_INC:
					if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_volume_inc))
					{		
						/* if move out */
						g_vdoply_cntx.full_touch_volume_inc.is_press = FALSE;
						g_vdoply_cntx.is_vol_inc_key_pressed = FALSE;
						
						mmi_vdoply_draw_fullscr_osd();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					}
					break;
			
				case VDOPLY_TOUCH_VOLUME_DEC:
					if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_volume_dec))
					{		
						/* if move out */
						g_vdoply_cntx.full_touch_volume_dec.is_press = FALSE;
						g_vdoply_cntx.is_vol_dec_key_pressed = FALSE;
						
						mmi_vdoply_draw_fullscr_osd();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					}
					break;
			
				case VDOPLY_TOUCH_SPEED_INC:
					if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_speed_inc))
					{		
						/* if move out */
						g_vdoply_cntx.full_touch_speed_inc.is_press = FALSE;
						g_vdoply_cntx.is_speed_inc_key_pressed = FALSE;
						
						mmi_vdoply_draw_fullscr_osd();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					}
					break;

				case VDOPLY_TOUCH_SPEED_DEC:
					if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_speed_dec))
					{		
						/* if move out */
						g_vdoply_cntx.full_touch_speed_dec.is_press = FALSE;
						g_vdoply_cntx.is_speed_dec_key_pressed = FALSE;
						
						mmi_vdoply_draw_fullscr_osd();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					}
					break;

				case VDOPLY_TOUCH_PROGRESS_INC:
					if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_progress_inc))
					{		
						/* if move out */
						g_vdoply_cntx.full_touch_progress_inc.is_press = FALSE;
						
						mmi_vdoply_seek_foward_key_release();
						
						mmi_vdoply_draw_fullscr_osd();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					}
					break;

				case VDOPLY_TOUCH_PROGRESS_DEC:
					if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_progress_dec))
					{		
						/* if move out */
						g_vdoply_cntx.full_touch_progress_dec.is_press = FALSE;
						
						mmi_vdoply_seek_backward_key_release();

						mmi_vdoply_draw_fullscr_osd();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					}
					break;
					
				case VDOPLY_TOUCH_PROGRESS_BAR:

						g_vdoply_cntx.full_touch_progress_bar.is_press = TRUE;

						/* out of bar region */
						if(pos.x > (g_vdoply_osd_cntx.full_panel.progress.empty.offset_x+g_vdoply_cntx.fullscr_progress.fill_width-1))
						{
							g_vdoply_cntx.cur_status.play_time = g_vdoply_cntx.video_info.total_time_duration;
						}
						else if(pos.x < g_vdoply_osd_cntx.full_panel.progress.empty.offset_x)
						{
							g_vdoply_cntx.cur_status.play_time = 0;
						}
						else
						{
							shift = pos.x - g_vdoply_osd_cntx.full_panel.progress.empty.offset_x+1;
							ratio	= (shift<<10)/g_vdoply_cntx.fullscr_progress.fill_width;

							g_vdoply_cntx.cur_status.play_time = (g_vdoply_cntx.video_info.total_time_duration*ratio)>>10;
						}

						/* play ratio scale is 1/width of progress */
					g_vdoply_cntx.cur_status.play_ratio = 	(U32)(g_vdoply_cntx.cur_status.play_time*	\
																				g_vdoply_cntx.fullscr_progress.fill_width/	\
																				g_vdoply_cntx.video_info.total_time_duration);

						mmi_vdoply_draw_fullscr_osd();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_PROGRESS_BAR;	
					
					break;

				case VDOPLY_TOUCH_PLAY:
					if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_play))
					{		
						/* if move out */
						g_vdoply_cntx.full_touch_play.is_press = FALSE;
						
						mmi_vdoply_draw_fullscr_osd();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					}
					break;
			
				case VDOPLY_TOUCH_STOP:
					if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_stop))
					{		
						/* if move out */
						g_vdoply_cntx.full_touch_stop.is_press = FALSE;
						
						mmi_vdoply_draw_fullscr_osd();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					}
					break;

		
				case VDOPLY_TOUCH_RSK:
					if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_rsk))
					{		
						/* if move out */
						g_vdoply_cntx.is_rsk_pressed = FALSE;	

						mmi_vdoply_draw_fullscr_osd();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					}
					break;
					
				case VDOPLY_TOUCH_LSK:
					if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.full_touch_lsk))
					{		
						/* if move out */
						g_vdoply_cntx.is_lsk_pressed = FALSE;	

						mmi_vdoply_draw_fullscr_osd();
						mmi_vdoply_blt_screen();
						g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
					}
					break;
					
			}

		#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

	}
	else
	{

		#if defined(__VDOPLY_FEATURE_HORIZONTAL_VIEW__)
		
			/* cord tranfrom, since already rotate */		
			tmp_x = pos.x;
			tmp_y = pos.y;			
			pos.x = tmp_y;
			pos.y = (UI_device_width-1)-tmp_x;
			
		#endif /* __VDOPLY_FEATURE_HORIZONTAL_VIEW__ */
		
		/* not fullscreen */
		switch(g_vdoply_cntx.touch_object)
		{
			case VDOPLY_TOUCH_VOLUME_INC:
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_volume_inc))
				{		
					/* if move out */
					g_vdoply_cntx.touch_volume_inc.is_press = FALSE;
					g_vdoply_cntx.is_vol_inc_key_pressed= FALSE;
					
					mmi_vdoply_draw_panel_volume();
					mmi_vdoply_blt_part();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;
		
			case VDOPLY_TOUCH_VOLUME_DEC:
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_volume_dec))
				{		
					/* if move out */
					g_vdoply_cntx.touch_volume_dec.is_press = FALSE;
					g_vdoply_cntx.is_vol_dec_key_pressed= FALSE;
					
					mmi_vdoply_draw_panel_volume();
					mmi_vdoply_blt_part();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;
		
			case VDOPLY_TOUCH_SPEED_INC:
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_speed_inc))
				{		
					/* if move out */
					g_vdoply_cntx.touch_speed_inc.is_press = FALSE;
					g_vdoply_cntx.is_speed_inc_key_pressed= FALSE;
					
					mmi_vdoply_draw_panel_speed();
					mmi_vdoply_blt_part();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;

			case VDOPLY_TOUCH_SPEED_DEC:
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_speed_dec))
				{		
					/* if move out */
					g_vdoply_cntx.touch_speed_dec.is_press = FALSE;
					g_vdoply_cntx.is_speed_dec_key_pressed= FALSE;
					
					mmi_vdoply_draw_panel_speed();
					mmi_vdoply_blt_part();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;

			case VDOPLY_TOUCH_PROGRESS_INC:
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_progress_inc))
				{		
					/* if move out */
					g_vdoply_cntx.touch_progress_inc.is_press = FALSE;
					
					mmi_vdoply_seek_foward_key_release();
					
					mmi_vdoply_draw_panel_progress();
					mmi_vdoply_blt_part();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;

			case VDOPLY_TOUCH_PROGRESS_DEC:
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_progress_dec))
				{		
					/* if move out */
					g_vdoply_cntx.touch_progress_dec.is_press = FALSE;
						
					mmi_vdoply_seek_backward_key_release();

					mmi_vdoply_draw_panel_progress();
					mmi_vdoply_blt_part();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;
				
			case VDOPLY_TOUCH_PROGRESS_BAR:

//					g_vdoply_cntx.full_touch_progress_bar.is_press = TRUE;
					g_vdoply_cntx.touch_progress_bar.is_press = TRUE; 

					/* out of bar region */
					if(pos.x > (g_vdoply_osd_cntx.panel.progress.empty.offset_x+g_vdoply_cntx.progress.fill_width-1))
					{
						g_vdoply_cntx.cur_status.play_time = g_vdoply_cntx.video_info.total_time_duration;
					}
					else if(pos.x < g_vdoply_osd_cntx.panel.progress.empty.offset_x)
					{
						g_vdoply_cntx.cur_status.play_time = 0;
					}
					else
					{
						shift = pos.x - g_vdoply_osd_cntx.panel.progress.empty.offset_x;
						ratio	= (shift<<10)/g_vdoply_cntx.progress.fill_width;

						g_vdoply_cntx.cur_status.play_time = (g_vdoply_cntx.video_info.total_time_duration*ratio)>>10;
					}

					/* play ratio scale is 1/width of progress */
					g_vdoply_cntx.cur_status.play_ratio = 	(U32)(g_vdoply_cntx.cur_status.play_time*	\
																				g_vdoply_cntx.progress.fill_width/	\
																				g_vdoply_cntx.video_info.total_time_duration);
						
					mmi_vdoply_draw_panel_progress();
					mmi_vdoply_draw_panel_timer();
					mmi_vdoply_blt_part();
				
				break;

			case VDOPLY_TOUCH_PLAY:
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_play))
				{		
					/* if move out */
					g_vdoply_cntx.touch_play.is_press = FALSE;
					
					mmi_vdoply_draw_panel_state();
					mmi_vdoply_blt_part();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;
		
			case VDOPLY_TOUCH_STOP:
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_stop))
				{		
					/* if move out */
					g_vdoply_cntx.touch_stop.is_press = FALSE;
					
					mmi_vdoply_draw_panel_state();
					mmi_vdoply_blt_screen();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;

			/* snapshot */
			#ifdef __VDOPLY_FEATURE_SNAPSHOT__
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_snapshot))
				{		
					/* if move out */
					g_vdoply_cntx.touch_snapshot.is_press = FALSE;
					
					mmi_vdoply_draw_panel_state();
					mmi_vdoply_blt_screen();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;
			#endif /* __VDOPLY_FEATURE_SNAPSHOT__ */

			/* fullscreen */
			#ifdef __VDOPLY_FEATURE_FULLSCREEN__
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_fullscreen))
				{		
					/* if move out */
					g_vdoply_cntx.touch_fullscreen.is_press = FALSE;
					
					mmi_vdoply_draw_panel_state();
					mmi_vdoply_blt_screen();		
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;

			#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */

			case VDOPLY_TOUCH_RSK:
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_rsk))
				{		
					/* if move out */
					g_vdoply_cntx.is_rsk_pressed = FALSE;	
					g_vdoply_cntx.touch_rsk.is_press = FALSE;
					mmi_vdoply_draw_softkey();
					mmi_vdoply_blt_part();
					
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;
			case VDOPLY_TOUCH_LSK:
				if(!mmi_vdoply_touch_scr_hit_test(pos.x, pos.y, &g_vdoply_cntx.touch_lsk))
				{		
					/* if move out */
					g_vdoply_cntx.is_lsk_pressed = FALSE;	
					g_vdoply_cntx.touch_lsk.is_press = FALSE;
					mmi_vdoply_draw_softkey();
					mmi_vdoply_blt_part();
					g_vdoply_cntx.touch_object = VDOPLY_TOUCH_NONE;	
				}
				break;
		}		
	}

	
}
#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ & __MMI_TOUCH_SCREEN__ */


/*****************************************************************************
* FUNCTION
*	 mmi_vdoply_touch_scr_hit_test()
* DESCRIPTION
*   test if is within touch region
* PARAMETERS
*	 pos_x		IN			position x
*	 pos_y		IN			position y
*	 touch		IN			touch object structure
* RETURNS
*   BOOL	
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__VDOPLY_FEATURE_TOUCH_SCREEN__) && defined(__MMI_TOUCH_SCREEN__)
static BOOL mmi_vdoply_touch_scr_hit_test(S32 pos_x, S32 pos_y, vdoply_ui_touch_struct *touch_obj)
{
 	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	TRACE_EF(g_sw_MED_Vid ,"touch_obj-->offset_x=%d,offset_y=%d,\n\
                                                       offset_width=%d,offset_height=%d",
                                                       touch_obj->offset_x, touch_obj->offset_y, 
                                                       touch_obj->width, touch_obj->height);
	if((pos_x >= touch_obj->offset_x) &&
		(pos_x <  touch_obj->offset_x + touch_obj->width) &&
		(pos_y >= touch_obj->offset_y) &&
		(pos_y <  touch_obj->offset_y + touch_obj->height))
	{
		TRACE_EF(g_sw_MED_Vid,"------after pen hit return TRUE------");
		return TRUE;
	}
	else
	{
		TRACE_EF(g_sw_MED_Vid,"------after pen hit return FALSE------");
		return FALSE;
	}
}
#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ & __MMI_TOUCH_SCREEN__ */

void mmi_vdoply_clean_app(void)
{
    if(IsScreenPresent( SCR_ID_VDOPLY_APP))
       DeleteUptoScrID(IDLE_SCREEN_ID);
}


#endif /* __MMI_VIDEO__ */




