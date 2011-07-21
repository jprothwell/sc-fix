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
*	AudioPlayerMsgHandler.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   Audio player message handling
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
#include "mdi_audio.h"
#include "mmi_trace.h"

#define __NEWSIMULATOR
#include "globaldefs.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "timerevents.h"
#include "settingprofile.h"
#include "hardwareevents.h"
#include "wgui_categories.h"

#include "idleappdef.h"
#include "commonscreens.h"

// dependent headers for AudioPlayerXXX.h"
#include "gui_data_types.h"
#include "frameworkstruct.h"
#include "globalconstants.h"
#include "mdi_datatype.h"
#include "kal_release.h"
#include "conversions.h"
#include "filemgr.h"
#include "fat_fs.h"
#include "filesystemdef.h"

#include "mdi_audio.h"
#include "audioplayerdef.h"
#include "audioplayertype.h"
#include "audioplayerprot.h"
#include "audioplayermainscreen.h"
//#include "audioplayerplaylist.h"

#include "callmanagementgprot.h"
#include "idleappprot.h"
#include "soundeffect.h"
#include "audioplayerplaylist.h"
#include "profilegprots.h"
#ifdef __DRM_SUPPORT__
#include "fs_errcode.h"
#include "drm_gprot.h"
#define DRM_NOT_PERMIT_TO_PLAY	9999
#endif
#include "mci.h"
#undef __NEWSIMULATOR
#ifdef __MMI_RUNNING_LED__
#include "led.h"
#include "gpioinc.h"
#endif
#ifdef MMI_ON_HARDWARE_P
#include "aud_main.h"
#else
#define AUD_FULL_PROGRESS_CNT (9999)
#endif
#if defined(__MMI_MAINLCD_220X176__)
extern S32 curr_vertical_hilite_index;
extern S32 curr_horizon_hilite_index;
#endif
mdi_handle mdi_h;

// For single play
mdi_handle mdi_h2;
extern U8 single_play_state;
extern U32 single_play_duration;
extern kal_uint32 single_play_time;
extern BOOL in_single_play_screen;
extern U8 single_volume;

#ifdef __MMI_AUDIO_PLAYER__

extern BOOL g_mmi_audply_play_completed_after_button_down; // defined in AudioPlayerSrc.c
extern BOOL g_mmi_audply_init_done; // defined in AudioPlayerSrc.c
extern UI_character_type root_dir_phone[4]; // defined in AudioPlayerSrc.c
extern UI_character_type root_dir_card[4]; // defined in AudioPlayerSrc.c
extern UI_character_type phone_list_name[FMGR_MAX_FILE_LEN+4]; // defined in AudioPlayerSrc.c
extern UI_character_type card_list_name[FMGR_MAX_FILE_LEN+4]; // defined in AudioPlayerSrc.c
extern BOOL g_mmi_audply_init_done;

extern void (*RedrawCategoryFunction)(void); // defined in wgui_categories.c

extern audply_struct g_audply; // defined in AudioPlayerSrc.c
extern signed char g_audply_main_selected_button; // defined in AudioPlayerMainScreen.c
extern BOOL g_audply_main_selected_button_down; // defined in AudioPlayerMainScreen.c

#if defined(__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__)
extern audply_lyric_struct audply_lyric;
extern audply_lyric_printing_struct audply_lyric_print;
extern MMI_BOOL lyric_reEntry;
extern MMI_BOOL lyric_parse_ok;
#ifdef __GDI_MEMORY_PROFILE_2__
extern GDI_HANDLE audply_osd_layer;      /* defined in AudioPlayerMainScren.c */
#endif /*__GDI_MEMORY_PROFILE_2__*/
#endif
#ifdef __MMI_A2DP_SUPPORT__
#include "av_bt.h"
static BOOL wait_open_bt = FALSE;
#endif

#ifdef __MMI_A2DP_SUPPORT__
extern mmi_bt_dev_struct *mmi_audply_get_bt_headset(void);
static kal_bool to_resume = KAL_FALSE;
#endif
void mmi_audply_do_restore_action( void );
void mmi_audply_resume(void);
extern BOOL mmi_idle_is_allow_to_display_song_name(void);
extern void idle_screen_show_network_details(void);
extern void time_update();
//extern void set_time_update();     //It's redundant codes, deleted by kecx for optimizing codes on 20090610
extern void record_time();
#if defined(__PROJECT_GALLITE_C01__)
void mmi_audply_error_play_next(void);
#endif

#ifdef __MMI_A2DP_SUPPORT__
/*****************************************************************************
 * FUNCTION
 *  mmi_audply_is_wait_open_bt
 * DESCRIPTION
 *  return if it's waiting for opening bt
 * PARAMETERS
 *  void
 * RETURNS
 *  BOOL
 *****************************************************************************/
BOOL mmi_audply_is_wait_open_bt(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    return wait_open_bt;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_bt_open_callback
 * DESCRIPTION
 *  a callback function invoked by av_bt_open apis
 * PARAMETERS
 *  result
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_bt_open_callback(S32 result)
{

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    BOOL prev_wait_open_bt ;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

  mmi_trace(g_sw_BT, "mmi_audply_bt_open_callback::g_audply.state=%d,result=%d,to_resume=%d,g_audply.need_restore=%d,g_audply.state=%d",\
  g_audply.state,result,to_resume,g_audply.need_restore,g_audply.state);

    prev_wait_open_bt = wait_open_bt;

    wait_open_bt = FALSE;

    switch( result )
    {
    case AV_BT_CALLBACK_EVENT_OPEN_OK:
        if (g_audply.state == STATE_PLAY)
        {
            av_bt_open_codec(KAL_TRUE);
        #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
            g_audply_spectrum_blocked = TRUE ;
            mmi_audply_turn_off_spectrum();
        #endif
        }
        else
        {
            av_bt_open_codec(KAL_FALSE);
        #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
            g_audply_spectrum_blocked = TRUE ;
        #endif
            if (to_resume)
            {
                ASSERT(g_audply.state == STATE_PAUSED);
                to_resume = KAL_FALSE;
                /*change state because it is not called by button pressing and 
                  it might already be in main screen so there is no chance to update 
                  state by calling entry main screen*/
                g_audply_main_selected_button = (signed char)AUDPLY_MAIN_PLAY;
		  mmi_audply_cancel_fast_timer_no_condition();
                g_audply_main_selected_button_down = FALSE;                                
                mmi_audply_do_resume_action();
            }
            else
            {
                if (g_audply.need_restore)
                {
                    mmi_audply_do_restore_action();
                }
                else
                {
                    mmi_audply_do_play_action();
                }
            }
            if( g_audply.state == STATE_IDLE )
            {
                av_bt_close_codec();
            #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
                g_audply_spectrum_blocked = FALSE ;
            #endif
            }
        }
        break;
    case AV_BT_CALLBACK_EVENT_OPEN_FAILED:
    case AV_BT_CALLBACK_EVENT_OPEN_STOPPED:
        if (g_audply.state == STATE_PLAY)
        {
            av_bt_close_codec();
        #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
            g_audply_spectrum_blocked = FALSE ;
            mmi_audply_turn_on_spectrum();
        #endif            
        }
        else if (to_resume)
        {
            ASSERT(g_audply.state == STATE_PAUSED);
            to_resume = KAL_FALSE;
            /*change state because it is not called by button pressing and 
              it might already be in main screen so there is no chance to update 
              state by calling entry main screen*/
            g_audply_main_selected_button = (signed char)AUDPLY_MAIN_PLAY;
	     mmi_audply_cancel_fast_timer_no_condition();
            g_audply_main_selected_button_down = FALSE;                            
            mmi_audply_do_resume_action();
        }
        else
        {
            if (g_audply.need_restore)
            {
                mmi_audply_do_restore_action();
            }
            else
            {
                mmi_audply_do_play_action();
            }
        }
        break;
    case AV_BT_CALLBACK_EVENT_OPEN_CANCELED:
        to_resume = KAL_FALSE;
        g_audply.need_replay = prev_wait_open_bt;

        /*when ab bt is canceled by mdi (interrupted callback), we shall update the play button
          since nobody else will release the button in this situation*/        
        g_audply_main_selected_button = (signed char)AUDPLY_MAIN_PAUSE;
	 mmi_audply_cancel_fast_timer_no_condition();
        g_audply_main_selected_button_down = FALSE;
        if(g_audply.in_main_screen)
        {
            mmi_audply_redraw_main_buttons();
        }        
        break;
    case AV_BT_CALLBACK_EVENT_STREAM_ABORT_IND:
    case AV_BT_CALLBACK_EVENT_STREAM_CLOSE_IND:
    case AV_BT_CALLBACK_EVENT_STREAM_SUSPEND_IND:
    case AV_BT_CALLBACK_EVENT_UNEXPECTED_DISCONNECT_IND:
        if( g_audply.state == STATE_PLAY )
        {
            av_bt_close_codec();
        #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
            g_audply_spectrum_blocked = FALSE ;
            mmi_audply_turn_on_spectrum();
        #endif            
        }
        break;
    case AV_BT_CALLBACK_EVENT_STREAM_START_IND:
        if (g_audply.state == STATE_PLAY)
        {
            wait_open_bt = TRUE;
            av_bt_open(mmi_audply_get_bt_headset(), g_audply.filefullname, //temp del g_audply.prev_filefullname for sucess compile 
                mmi_audply_bt_open_callback, FALSE, //!mmi_audply_is_play_speed_normal()
                KAL_TRUE);
        }
        else
        {
            av_bt_close(KAL_FALSE);
        }
        break;
    case AV_BT_CALLBACK_EVENT_INQUIRY_ORCONHFP_START_IND:
        if (mmi_audply_is_output_to_bt() && av_bt_is_streaming() &&
            g_audply.state == STATE_PLAY)
        {
            mmi_audply_do_pause_action();
            if (g_audply.state != STATE_PLAY)
            {
                av_bt_close_codec();
            #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
                g_audply_spectrum_blocked = FALSE ;  
            #endif                
            }
            if (g_audply.state == STATE_PAUSED)
            {
                to_resume = KAL_TRUE;
                av_bt_pause();
            }
            //av_bt_pause();
        }
        break;
    case AV_BT_CALLBACK_EVENT_INQUIRY_ORCONHFP_STOP_IND:
        if (to_resume)
        {
            to_resume = KAL_FALSE;
            if (g_audply.state == STATE_PAUSED )//|| g_audply.pause_idle
            {
      //          if(!g_audply.pause_idle)
                {
                    mmi_audply_resume();
                }
 //               else
                {
    //                mmi_audply_play();
                }
            }
            else if (g_audply.state == STATE_IDLE && !wait_open_bt)
            {
                av_bt_close(KAL_FALSE);
            }
        }
        break;
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_switch_bt_output
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_switch_bt_output(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	mmi_trace(g_sw_BT,"mmi_audply_switch_bt_output is called,mmi_audply_is_output_to_bt()=%d",mmi_audply_is_output_to_bt());
	mmi_trace(g_sw_BT,"g_audply.state %d",g_audply.state );
    if (mmi_audply_is_output_to_bt())
    {
        /* switch output: OFF -> ON */
        if (g_audply.state == STATE_PLAY)
        {
            wait_open_bt = TRUE;
            av_bt_open(mmi_audply_get_bt_headset(), g_audply.filefullname, //temp del g_audply.prev_filefullname for sucess compile 
                mmi_audply_bt_open_callback, FALSE, //!mmi_audply_is_play_speed_normal()
                KAL_TRUE);
        }
    }
    else
    {
        /* switch output: ON -> OFF */
        if (g_audply.state == STATE_PLAY)
        {
            av_bt_close_codec();
        #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
            g_audply_spectrum_blocked = FALSE ;
            mmi_audply_turn_on_spectrum();
        #endif    
        }
        av_bt_close(KAL_TRUE);
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_sync_bt_output
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_sync_bt_output(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    AV_BT_CALLBACK callback = NULL;
    kal_wchar *filename;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

//    MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_AUDPLY_TRC_SYNC_BT_OUTPUT);
	mmi_trace(g_sw_BT,"mmi_audply_sync_bt_output is called,g_audply.state=%d,wait_open_bt=%d",g_audply.state ,wait_open_bt);

    ASSERT( mmi_audply_is_output_to_bt() );

    to_resume = KAL_FALSE;
    if (g_audply.state == STATE_PLAY)
    {
        mmi_audply_do_pause_action();
        av_bt_close_codec();
        #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
            g_audply_spectrum_blocked = FALSE ;
            /*normally not need to turn on spectrum, in case pause not succeed*/
            mmi_audply_turn_on_spectrum();
        #endif        
        callback = mmi_audply_bt_open_callback;
    }
    else if (wait_open_bt)
    {
        callback = mmi_audply_bt_open_callback;
    }

    if (callback)
    {
        if (g_audply.state == STATE_PLAY) // can't pause
        {
            filename = g_audply.filefullname; //g_audply.prev_filefullname;
        }
        else if (g_audply.state == STATE_PAUSED)
        {
            to_resume = KAL_TRUE;
            filename = g_audply.filefullname;     // g_audply.prev_filefullname;
        }
        else
        {
            filename = g_audply.filefullname;
        }
        wait_open_bt = TRUE;
        av_bt_open(mmi_audply_get_bt_headset(), filename, callback, 
           FALSE , KAL_TRUE);//!mmi_audply_is_play_speed_normal()
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_bt_stop
 * DESCRIPTION
 *  
 * PARAMETERS
 *  stop_codec      [IN]
 *  notify_opener   [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_bt_stop(BOOL stop_codec, BOOL notify_opener)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
mmi_trace(g_sw_BT,"mmi_audply_bt_stop");
    /* always stop codec*/
    if( mmi_audply_is_output_to_bt() )
    {
//        if (stop_codec)
//        {
            av_bt_close_codec();
        #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
            g_audply_spectrum_blocked = FALSE ;
            mmi_audply_turn_on_spectrum();
        #endif            
//        }
        av_bt_close(notify_opener);
    }
}

#endif /* __MMI_A2DP_SUPPORT__ */

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_play
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_play(void)
{
    mmi_trace(g_sw_MP3, "mmi_audply_play:g_audply.state=%d\n",g_audply.state);
 
#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)
    to_resume = KAL_FALSE;
    if( mmi_audply_is_output_to_bt() )
    {
        if( g_audply.state == STATE_PLAY )
        {
            mmi_audply_do_stop_action();
            av_bt_close_codec();
        }    

        wait_open_bt = TRUE;
        av_bt_open(mmi_audply_get_bt_headset(), g_audply.filefullname, 
            mmi_audply_bt_open_callback,FALSE,
            KAL_TRUE);// !mmi_audply_is_play_speed_normal()
    }
    else
#endif /* __MMI_A2DP_SUPPORT__ */
    {
        mmi_audply_do_play_action();    /* do play action */
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_restore
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_restore(void)
{
 
  mmi_trace(g_sw_MP3," mmi_audply_restore is called");
#if (defined(__MMI_BT_COOLSAND_SUPPORT__)||defined(__MMI_BT_RDA_SUPPORT__)) && defined(__MMI_A2DP_SUPPORT__)
    to_resume = KAL_FALSE;
    if( mmi_audply_is_output_to_bt() )
    {
        wait_open_bt = TRUE;
	g_audply.need_restore =TRUE;
        av_bt_open(mmi_audply_get_bt_headset(), g_audply.filefullname, 
            mmi_audply_bt_open_callback, FALSE,
            KAL_TRUE);
    }
    else
#endif /* __MMI_A2DP_SUPPORT__ */
    {
        mmi_audply_do_restore_action();    /* do restore action */
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_pause
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_pause(void)
{
    mmi_audply_do_pause_action();   /* do pause action */
 
#if (defined(__MMI_BT_COOLSAND_SUPPORT__)|| defined (__MMI_BT_RDA_SUPPORT__) )&& defined(__MMI_A2DP_SUPPORT__)
    if( mmi_audply_is_output_to_bt() )
    {
        if( g_audply.state != STATE_PLAY )
        {
            av_bt_close_codec();
        #ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
            g_audply_spectrum_blocked = FALSE ;
        #endif            
            av_bt_close(KAL_FALSE);
        }
        //av_bt_close(KAL_FALSE);
    }
#endif /* __MMI_A2DP_SUPPORT__ */
}

/*****************************************************************************
 * FUNCTION
 *  mmi_audply_resume
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_audply_resume(void)
{
 
mmi_trace(g_sw_MP3,"mmi_audply_resume is called");
#if (defined(__MMI_BT_COOLSAND_SUPPORT__) ||defined(__MMI_BT_RDA_SUPPORT__))&& defined(__MMI_A2DP_SUPPORT__)
    if( mmi_audply_is_output_to_bt() )
    {
        to_resume = KAL_TRUE;
        wait_open_bt = TRUE;
        av_bt_open(mmi_audply_get_bt_headset(), g_audply.filefullname, 
            mmi_audply_bt_open_callback, FALSE,
            KAL_TRUE);//!mmi_audply_is_play_speed_normal()
    }
    else
#endif /* __MMI_A2DP_SUPPORT__ */
    {
        mmi_audply_do_resume_action();  /* do resume action */
    }
}
/*****************************************************************************
* FUNCTION
*     mmi_audply_do_play_action
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
void mmi_audply_do_play_action( void )
{
	S32 result;
	
	TBM_ENTRY(0x2A21);
		mmi_trace(g_sw_MP3, "mmi_audply_do_play_action");

	result = mmi_audply_do_play_action_without_handle_result(); 
	mmi_trace(g_sw_MP3, "mmi_audply_do_play_action,result is %d",result);

	if( result == MDI_AUDIO_SUCCESS
            || result ==  MDI_AUDIO_BAD_FORMAT
            || result ==  MDI_AUDIO_INVALID_FORMAT
            || result ==  MCI_ERR_INVALID_FORMAT
            || result ==  MCI_ERR_BAD_FORMAT
            || result ==  MCI_ERR_UNKNOWN_FORMAT
        )
	{
		mmi_audply_played();
		//chenhe,fix bug #6353
		//mdi_audio_set_volume( VOL_TYPE_MEDIA, g_audply.volume );
		//chenhe,fix bug #6353
	}
	else if( result == MCI_ERR_OUT_OF_MEMORY ) //chenhe add for bug 14000 ,20090927
	{
		mmi_audply_play_callback_hdlr(MDI_AUDIO_END_OF_FILE);
	}
	else
	{ 
		mmi_audply_stopped();
		if( result > 0 && result != MDI_AUDIO_BLOCKED )
			DisplayPopup( (PU8)GetString( mmi_audply_err_str( result ) ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
	}
	
	TBM_EXIT(0x2A21);
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_do_resume_action_without_handle_result
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
S32 mmi_audply_do_restore_action_without_handle_result(void)
{
	mdi_result result;

	StopTimer( AUDPLY_DELAY_PLAY_TIMER );
	g_audply.wait_next = FALSE;
	g_audply.need_restore = FALSE;
	//mmi_audply_clear_play_seconds();
	mmi_trace(g_sw_MP3,"without_handle_result is called");
	if( g_audply.in_main_screen )
		{
		mmi_audply_redraw_main_time();	// update time: show MM:SS
		mmi_audply_redraw_main_progressbar();
		}
	
	// This could happen when timer experied after suspend background play or another play or record req by others
	if( mdi_audio_is_background_play_suspended() || (g_audply.state == STATE_IDLE && !mdi_audio_is_idle()) )
	{
		g_audply.need_replay = TRUE;
		g_audply.need_restore = TRUE;
		g_audply.state = STATE_IDLE;
		#ifdef __MMI_RUNNING_LED__
			StopLEDPatternMusicRunning();
		#endif	
		result = -1;
	}
	else
	{
		g_audply.state = STATE_IDLE; // for preventing from terminated callback;
		#ifdef __MMI_RUNNING_LED__
			StopLEDPatternMusicRunning();
		#endif	
		g_audply.need_replay = FALSE;
#ifdef __DRM_SUPPORT__
#ifdef MMI_ON_HARDWARE_P
		{
			FS_HANDLE handle;
			BOOL is_permitted = FALSE;
	
			if( (handle = DRM_open_file( (PU16)g_audply.filefullname, FS_READ_ONLY, DRM_PERMISSION_NONE )) >= FS_NO_ERROR )
			{
				if( DRM_validate_permission( handle, DRM_PERMISSION_PLAY ) )
				{
					DRM_consume_rights( handle, DRM_PERMISSION_PLAY );
					is_permitted = TRUE;
				}
				DRM_close_file( handle );
			}
			else if( handle < -1 ) // file open error but not DRM related problem
				is_permitted = TRUE;

			if( !is_permitted )
			{
				mdi_audio_stop_all();
				return (S32) DRM_NOT_PERMIT_TO_PLAY;
			}
		}
#endif
#endif
		result = mdi_audio_restore_file_with_vol_path( g_audply.filefullname, DEVICE_AUDIO_PLAY_ONCE, &mdi_h,
			mmi_audply_play_callback_hdlr, g_audply.volume, MDI_DEVICE_SPEAKER2 );
		if(  result == MDI_AUDIO_SUCCESS )
        { 
			g_audply.state = STATE_PLAY;
			#ifdef __MMI_RUNNING_LED__
            StartLEDPatternMusicRunning();
			#endif
        }
		else if( result == MDI_AUDIO_BLOCKED )
		{
			g_audply.need_replay = TRUE;
			g_audply.need_restore = TRUE;
		}
		else if( result == MDI_AUDIO_BAD_FORMAT ) // in this case, it means not able to do real resume. Let's try replay it
		{
			mmi_audply_clear_play_seconds();
			if( g_audply.in_main_screen )
				{
				mmi_audply_redraw_main_time();	// update time: show 00:00
				mmi_audply_redraw_main_progressbar();
				}
			result = mdi_audio_play_file_with_vol_path( g_audply.filefullname, DEVICE_AUDIO_PLAY_ONCE, &mdi_h,
				mmi_audply_play_callback_hdlr, g_audply.volume, MDI_DEVICE_SPEAKER2,1 );
			if( result == MDI_AUDIO_SUCCESS )
            { 
				g_audply.state = STATE_PLAY;
			#ifdef __MMI_RUNNING_LED__
                StartLEDPatternMusicRunning();
            #endif
		}
	}
	}

	return (S32) result;
}


/*****************************************************************************
* FUNCTION
*     mmi_audply_do_resume_action
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
void mmi_audply_do_restore_action( void )
{
	S32 result = mmi_audply_do_restore_action_without_handle_result();
      mmi_trace(g_sw_MP3,"mmi_audply_do_restore_action,result is %d",result);

	if( result == MDI_AUDIO_SUCCESS )
		mmi_audply_played();
	else
	{
		mmi_audply_clear_play_seconds();
		mmi_audply_stopped();
		if( result > 0 && result != MDI_AUDIO_BLOCKED )
			DisplayPopup( (PU8)GetString( mmi_audply_err_str( result ) ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_do_play_action_and_back_to_main_screen
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
void mmi_audply_do_play_action_and_back_to_main_screen( void )
{
	S32 result = mmi_audply_do_play_action_without_handle_result();
	mmi_trace(g_sw_MP3, "mmi_audply_do_play_action_and_back_to_main_screen");
	if( result == MDI_AUDIO_SUCCESS )
	{
		mmi_audply_played();
		//mdi_audio_set_volume( VOL_TYPE_MEDIA, g_audply.volume );
	}
	else
		mmi_audply_stopped();
	GoBackToHistory( SCR_ID_AUDPLY_MAIN );
	if( result > 0 && result != MDI_AUDIO_BLOCKED )
		DisplayPopup( (PU8)GetString( mmi_audply_err_str( result ) ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_do_play_action_without_handle_result
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
S32 mmi_audply_do_play_action_without_handle_result(void)
{
	mdi_result result;

	StopTimer( AUDPLY_DELAY_PLAY_TIMER );
	g_audply.wait_next = FALSE;
	g_audply.need_restore = FALSE;
//	mmi_audply_clear_play_seconds();
	if( g_audply.in_main_screen )
		{
		mmi_audply_redraw_main_time();	// update time: show 00:00
		mmi_audply_redraw_main_progressbar();
		}

	mmi_trace(g_sw_MP3,"mmi_audply_do_play_action_without_handle_result,mdi_audio_is_background_play_suspended is %d,g_audply.state %d,mdi_audio_is_idle is %d", mdi_audio_is_background_play_suspended(), g_audply.state,mdi_audio_is_idle() ,mdi_audio_is_idle);
#if 1
	// This could happen when timer experied after suspend background play or another play or record req by others
	if( mdi_audio_is_background_play_suspended() || (g_audply.state == STATE_IDLE && !mdi_audio_is_idle()) )
	{
		g_audply.need_replay = TRUE;
		g_audply.state = STATE_IDLE;
		#ifdef __MMI_RUNNING_LED__		
			StopLEDPatternMusicRunning();
		#endif
		result = -1;
	}
	else
#endif //chenhe test
	{
		g_audply.state = STATE_IDLE; // for preventing from terminated callback;

		g_audply.need_replay = FALSE;
		#ifdef __MMI_RUNNING_LED__
			StopLEDPatternMusicRunning();
		#endif
#ifdef __DRM_SUPPORT__
#ifdef MMI_ON_HARDWARE_P
		{
			FS_HANDLE handle;
			BOOL is_permitted = FALSE;
	
			if( (handle = DRM_open_file( (PU16)g_audply.filefullname, FS_READ_ONLY, DRM_PERMISSION_NONE )) >= FS_NO_ERROR )
			{
				if( DRM_validate_permission( handle, DRM_PERMISSION_PLAY ) )
				{
					DRM_consume_rights( handle, DRM_PERMISSION_PLAY );
					is_permitted = TRUE;
				}
				DRM_close_file( handle );
			}
			else if( handle < -1 ) // file open error but not DRM related problem
				is_permitted = TRUE;

			if( !is_permitted )
			{
				mdi_audio_stop_all();
				return (S32) DRM_NOT_PERMIT_TO_PLAY;
			}
		}
#endif
#endif


		result = mdi_audio_play_file_with_vol_path( g_audply.filefullname, DEVICE_AUDIO_PLAY_ONCE, &mdi_h,
			mmi_audply_play_callback_hdlr, g_audply.volume, MDI_DEVICE_SPEAKER2,0 );



		if( result == MDI_AUDIO_SUCCESS )
		{
			InitEQ();
			g_audply.state = STATE_PLAY;
        	#ifdef __MMI_RUNNING_LED__
			StartLEDPatternMusicRunning();
			#endif
		}
		else if( result == MDI_AUDIO_BLOCKED )
		{
			g_audply.need_replay = TRUE;
		}
		else if(result==MCI_ERR_OUT_OF_MEMORY)//chenhe add for bug 14000 ,20090927
		{
			g_audply.state = STATE_PLAY;
			#ifdef __MMI_RUNNING_LED__			
            StartLEDPatternMusicRunning();
			#endif
		}
	}

	return (S32) result;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_do_pause_action
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
void mmi_audply_do_pause_action(void)
{
	mdi_result result;

	TBM_ENTRY(0x2A22);
	mmi_trace(g_sw_MP3,"mmi_audply_do_pause_action is called");
	g_audply.need_replay = FALSE;
	g_audply.need_restore = FALSE;
	result = mdi_audio_pause( mdi_h, mmi_audply_pause_callback_hdlr );
	mmi_trace(g_sw_MP3,"mmi_audply_do_pause_action is called,result=%d,g_audply.state=%d",result,g_audply.state);
	if( result == MDI_AUDIO_SUCCESS )
		mmi_audply_paused();
	else if( result == MDI_AUDIO_UNSUPPORTED_OPERATION )
	{
		// pause action for ADIF AAC files are not supportted so keep it playing
		g_audply_main_selected_button = AUDPLY_MAIN_PLAY;
		mmi_audply_cancel_fast_timer_no_condition();
		DisplayPopup( (PU8)GetString( STR_ID_AUDPLY_CANNOT_PAUSE ), IMG_GLOBAL_WARNING, 0, 1000, (U8)WARNING_TONE );
	}
	else
		mmi_audply_play_completed();

#if (defined(__MMI_BT_COOLSAND_SUPPORT__)  ||defined(__MMI_BT_RDA_SUPPORT__))&& defined(__MMI_A2DP_SUPPORT__)
	if( mmi_audply_is_output_to_bt() )
	{
	if( g_audply.state != STATE_PLAY )
	{
		av_bt_close_codec();
#ifdef __MMI_AUDIO_PLAYER_A2DP_BLOCK_SPECTRUM__
		g_audply_spectrum_blocked = FALSE ;
#endif            
		av_bt_close(KAL_FALSE);
		}
		//av_bt_close(KAL_FALSE);
	}
#endif
#if defined(__MMI_MAINLCD_220X176__)
		if(3 == curr_vertical_hilite_index)//guoyt added,when audply paused,reset hilite shortcut.
		{
			curr_vertical_hilite_index = 0;
			curr_horizon_hilite_index = 3;
		}
#endif
	TBM_EXIT(0x2A22);
}
/*****************************************************************************
* FUNCTION
*     mmi_audply_do_seek_action
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
 BOOL PreDoProgressAction;
void mmi_audply_do_before_seek_action(INT32 progress)
{
	U32 duration ;
	
	if( g_audply.state == STATE_PLAY )
		{
			mmi_audply_do_pause_action();	// do pause action
			PreDoProgressAction = TRUE;
		}
		mdi_audio_get_file_time_duration(g_audply.filefullname,(U32)progress,&duration);
	duration = (U64)duration*16384/1000;
	mmi_audply_reset_play_seconds((U32)duration);
	mmi_trace(1, "audply *************mmi_audply_do_before_seek_action    duration = %d   ",duration);
}
/*****************************************************************************
* FUNCTION
*     mmi_audply_do_seek_action
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
void mmi_audply_do_seek_action(INT32 progress)
{
		mdi_audio_set_file_data_progress(progress);
		mdi_audio_set_file_data_progress(AUD_FULL_PROGRESS_CNT+100);
	if( g_audply.state == STATE_PAUSED )
		{
		if(PreDoProgressAction)
			{
				mmi_audply_resume();
				PreDoProgressAction = FALSE;
			}
		}
}
/*****************************************************************************
* FUNCTION
*     mmi_audply_do_seek_action
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
void mmi_audply_do_seeking_action(INT32 progress)
{
	U32 duration ;
	duration = mdi_audio_set_file_data_progress(progress);
	mmi_trace(1, "audply *************mmi_audply_do_seeking_action111    duration = %d   ",duration);
	duration =(((U64) duration*16384)/1000);
	mmi_audply_reset_play_seconds((U32)duration);
	mmi_trace(1, "audply *************mmi_audply_do_seeking_action222    duration = %d   ",duration);
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_get_progress
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
INT32 mmi_audply_get_progress(void)
{
	U32 PlayProgress = 0;
	mdi_audio_get_file_data_progress(&PlayProgress);
	return PlayProgress;
}
/*****************************************************************************
* FUNCTION
*     mmi_audply_do_resume_action
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
void mmi_audply_do_resume_action(void)
{
	mdi_result result;
mmi_trace(g_sw_MP3,"mmi_audply_do_resume_action");

	TBM_ENTRY(0x2A23);

	g_audply.need_replay = FALSE;
	g_audply.need_restore = FALSE;
	result = mdi_audio_resume( mdi_h, mmi_audply_resume_callback_hdlr );
	if( result == MDI_AUDIO_SUCCESS )
	{
        #ifdef MMI_ON_HARDWARE_P
		#ifdef __MMI_SOUND_EFFECT__
            MCI_AudioSetEQ(g_settings_audio_equalizer.index);
		#endif
        #endif
            mmi_audply_resumed();
		//chenhe,fix bug #6353
		//mdi_audio_set_volume( VOL_TYPE_MEDIA, g_audply.volume );
		//chenhe,fix bug #6353
	}
	else
	{
		g_audply.state = STATE_IDLE;
		#ifdef __MMI_RUNNING_LED__
			StopLEDPatternMusicRunning();
		#endif
		DisplayPopup( (PU8)GetString( mmi_audply_err_str( (S32)result ) ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
	}

	TBM_EXIT(0x2A23);
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_do_stop_action
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
void mmi_audply_do_stop_action(void)
{
	g_audply.need_replay = FALSE;
	g_audply.need_restore = FALSE;

	g_audply.state = STATE_IDLE; // for preventing from set g_audply.need_replay = TRUE;
	#ifdef __MMI_RUNNING_LED__
		StopLEDPatternMusicRunning();
	#endif
	mdi_audio_stop_file();
	mmi_audply_stopped();
#if defined(__MMI_MAINLCD_220X176__)
	if(3 == curr_vertical_hilite_index)//guoyt added,when stopped audply,reset hilite shortcut.
	{
		curr_vertical_hilite_index = 0;
		curr_horizon_hilite_index = 3;
	}
#endif


}

/*****************************************************************************
* FUNCTION
*     mmi_audply_stop_playing
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
void mmi_audply_stop_playing(void)
{
    if( !g_mmi_audply_init_done )
	{
		return;
	}
	StopTimer( AUDPLY_DELAY_PLAY_TIMER );
	g_audply.wait_next = FALSE;
    g_audply.need_replay = FALSE;
    g_audply.need_restore = FALSE;
    mmi_audply_clear_play_seconds();
	if( g_audply.state != STATE_IDLE )
		mmi_audply_do_stop_action(); // stop song
//#if (defined(__MMI_BT_COOLSAND_SUPPORT__)  ||defined(__MMI_BT_RDA_SUPPORT__))&& defined(__MMI_A2DP_SUPPORT__)
    	//mmi_audply_bt_stop(KAL_FALSE, KAL_FALSE);
//#endif
#ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__     
    mmi_audply_stop_lyric();
#endif
	TBM_EXIT(0x2A24);
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_background_callback_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     mdi_result result
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_audply_background_callback_hdlr( mdi_result result )
{
	if( g_audply.need_replay )
	{
		StopTimer( AUDPLY_DELAY_PLAY_TIMER ); // stop previous play request
		if( g_audply.wait_next )//|| !g_audply.need_restore )
			mmi_audply_play();
		else
			mmi_audply_restore();
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_play_callback_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     mdi_result result
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_play_callback_hdlr( mdi_result result )
{
    mmi_trace(g_sw_MP3,"Audply: mmi_audply_play_callback_hdlr, state:%d, result:%d, restorable:%d",
                       g_audply.state,result,g_audply.restorable);
	// ignore it
	if( g_audply.state != STATE_PLAY )
		return;

	// handle result
	g_audply.need_restore = FALSE;
	switch( result )
	{
	case MDI_AUDIO_END_OF_FILE:
		g_audply.need_replay = FALSE;
		mmi_audply_play_completed();
		break;
       case MDI_AUDIO_BAD_FORMAT:
       case MDI_AUDIO_INVALID_FORMAT:
       case MCI_ERR_INVALID_FORMAT:
       case MCI_ERR_BAD_FORMAT:
       case MCI_ERR_UNKNOWN_FORMAT:
                g_audply.need_replay = FALSE;
                g_audply_main_selected_button_down =FALSE;
                mmi_audply_play_completed();
                DisplayPopup( (PU8)GetString( mmi_audply_err_str( (S32)result ) ), IMG_GLOBAL_WARNING/*IMG_GLOBAL_ERROR*/, 0, 1000, (U8)ERROR_TONE );//modify by lin @20070830
		break;
	case MDI_AUDIO_TERMINATED:
		g_audply.need_replay = TRUE;//chenhe modify the value to TRUE for paly resume
		mmi_trace(g_sw_MP3,"mmi_audply_stopped will be called ss");
		mmi_audply_stopped();
		if( g_audply.restorable )
		{
			//if( mdi_audio_store_file( mdi_h ) == MDI_AUDIO_SUCCESS )//chenhe comment it temparary for audio play reusme
				g_audply.need_restore = TRUE;
		}
		break;
	default:
		g_audply.need_replay = FALSE;
		mmi_audply_stopped();
#if defined(__PROJECT_GALLITE_C01__)
#else
		DisplayPopup( (PU8)GetString( mmi_audply_err_str( (S32)result ) ), IMG_GLOBAL_WARNING/*IMG_GLOBAL_ERROR*/, 0, 1000, (U8)ERROR_TONE );//modify by lin @20070830
#endif
		break;
	}
}


/*****************************************************************************
* FUNCTION
*     mmi_audply_pause_callback_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     mdi_result result
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_pause_callback_hdlr( mdi_result result )
{	
	mmi_trace(g_sw_MP3,"mmi_audply_pause_callback_hdlr is called");

	// ignore it
	if( g_audply.state != STATE_PAUSED )
		return;
	
	// handle result
	g_audply.need_replay = FALSE;
	g_audply.need_restore = FALSE;
	
	switch( result )
	{
	case MDI_AUDIO_END_OF_FILE:
		mmi_audply_play_completed();
		break;
	case MDI_AUDIO_TERMINATED:
		mmi_audply_stopped();
		break;
	default:
		mmi_audply_stopped();
		DisplayPopup( (PU8)GetString( mmi_audply_err_str( (S32)result ) ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		break;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_resume_callback_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     mdi_result result
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_resume_callback_hdlr( mdi_result result )
{
	// ignore it
	if( g_audply.state != STATE_PLAY )
		return;

	// handle result
	g_audply.need_restore = FALSE;
	
	switch( result )
	{
	case MDI_AUDIO_END_OF_FILE:
		g_audply.need_replay = FALSE;
		mmi_audply_play_completed();
		break;
	case MDI_AUDIO_TERMINATED:
		g_audply.need_replay = TRUE;

		mmi_audply_stopped();
		if( g_audply.restorable )
		{
			if( mdi_audio_store_file( mdi_h ) == MDI_AUDIO_SUCCESS )
				g_audply.need_restore = TRUE;
		}
		break;
	default:
		g_audply.need_replay = FALSE;
		mmi_audply_stopped();
		DisplayPopup( (PU8)GetString( mmi_audply_err_str( (S32)result ) ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		break;
	}
}
BOOL mmi_audio_need_replay(void)
{
	return g_audply.need_replay;
}
/*****************************************************************************
* FUNCTION
*     mmi_audply_play_completed
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*    void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_play_completed(void)
{
	    mmi_trace(g_sw_MP3,"mmi_audply_play_completed");
	// update state
	g_audply.state = STATE_IDLE;
	#ifdef __MMI_RUNNING_LED__
		StopLEDPatternMusicRunning();
	#endif
	#if (defined(__MMI_BT_COOLSAND_SUPPORT__) || defined (__MMI_BT_RDA_SUPPORT__))&& defined(__MMI_A2DP_SUPPORT__)
    mmi_audply_bt_stop(KAL_TRUE, KAL_FALSE);
#endif
	StopTimer( AUDPLY_COUNT_PLAY_TIMER );
	
	// check if buttin is pressed down is to avoid repeating on a short file too quick and cause a result to block other commands
	if( g_audply.in_main_screen && g_audply_main_selected_button_down && 
		(g_audply_main_selected_button == AUDPLY_MAIN_STOP ||
		g_audply_main_selected_button == AUDPLY_MAIN_PREV || 
		g_audply_main_selected_button == AUDPLY_MAIN_NEXT) ) 
		g_mmi_audply_play_completed_after_button_down = TRUE;
	else
	{
		BOOL picked_OK = FALSE;
		if( mmi_audply_pick_playlist() < 0 )
		{
			mmi_audply_pick_playlist(); // pick after play list  reset
			if( mmi_audply_apply_picked_file() )
			{
				mmi_audply_clear_play_seconds();
				g_audply_main_selected_button = (signed char) AUDPLY_MAIN_STOP;
				mmi_audply_cancel_fast_timer_no_condition();
				g_audply_main_selected_button_down = FALSE;
				picked_OK = TRUE;
			}
		}
		else
		{
			if( mmi_audply_apply_picked_file() )
			{
				StopTimer( AUDPLY_DELAY_PLAY_TIMER ); // stop previous play request
				    mmi_trace(g_sw_MP3, "wait 0.7s,mmi_play");
				StartTimer( AUDPLY_DELAY_PLAY_TIMER, 700, mmi_audply_play ); // wait for 0.7 second to request play
				g_audply.wait_next = TRUE;
				g_audply_main_selected_button = (signed char) AUDPLY_MAIN_STOP;
				mmi_audply_cancel_fast_timer_no_condition();
				g_audply_main_selected_button_down = FALSE;
				picked_OK = TRUE;
			}
		}

		if( g_audply.in_main_screen )
			mmi_audply_redraw_main_screen();
		else if( picked_OK && g_audply.in_list_screen )
			dynamic_list_goto_item( g_audply.pick_index );
		else if( g_idle_context.IsOnIdleScreen ) // update idle screen
			RedrawCategoryFunction();
	}

    #ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
        mmi_audply_stop_scroll_lyric_and_clean_screen();
        audply_lyric.is_ready = MMI_FALSE;
        lyric_parse_ok = MMI_FALSE;
        mmi_audply_lyric_check_backlight(0);
    #endif /* __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__ */ 
#ifdef __MMI_SUBLCD__
	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
		ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
}
#if defined(__PROJECT_GALLITE_C01__)
void mmi_audply_error_play_next(void)
{
    GoBackHistory();
    mmi_audply_play_completed();
}
#endif
/*****************************************************************************
* FUNCTION
*     mmi_audply_stopped
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*    void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_stopped(void)
{
	// update state
	mmi_trace(g_sw_MP3,"mmi_audply_stopped is called");


		if( g_audply.state != STATE_PAUSED )
		{
			mmi_audply_clear_play_seconds();
			mdi_audio_set_file_data_progress(0);
		}


	g_audply.state = STATE_IDLE;
	#ifdef __MMI_RUNNING_LED__
		StopLEDPatternMusicRunning();
	#endif
	if( g_audply.n_total_items > 0 )
		g_audply_main_selected_button = (signed char) AUDPLY_MAIN_STOP;
	else
		g_audply_main_selected_button = (signed char) AUDPLY_MAIN_DISABLED;

	mmi_audply_cancel_fast_timer_no_condition();

	g_audply_main_selected_button_down = FALSE;
	
	StopTimer( AUDPLY_COUNT_PLAY_TIMER );
    #ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
        mmi_audply_stop_lyric();
    #endif /* __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__ */ 

    if( g_audply.in_main_screen )
    {
    g_audply.const_seconds = 0;
    g_audply.play_seconds = 0;
    mmi_audply_redraw_main_buttons();
    mmi_audply_redraw_main_time();
    mmi_audply_redraw_main_progressbar();
    }
	else if( g_idle_context.IsOnIdleScreen ) // update idle screen
		RedrawCategoryFunction();


#if (defined(__MMI_BT_COOLSAND_SUPPORT__)||defined (__MMI_BT_RDA_SUPPORT__)) && defined(__MMI_A2DP_SUPPORT__)
    mmi_audply_bt_stop(KAL_TRUE, KAL_FALSE);
#endif//??????zhou siyou


#ifdef __MMI_SUBLCD__
	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
		ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_resumed
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*    void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_resumed(void)
{
	// update state
	g_audply.state = STATE_PLAY;
	#ifdef __MMI_RUNNING_LED__
		StartLEDPatternMusicRunning();
	#endif
	mmi_trace(g_sw_MP3,"mmi_audply_resumed is called");
	mmi_audply_start_record_play_seconds();
	StartTimer( AUDPLY_COUNT_PLAY_TIMER, 200, mmi_audply_timer );

    #ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
        mmi_audply_lyric_check_backlight(1);
    #endif 
	if( g_audply.in_main_screen )
	{
		mmi_audply_redraw_main_buttons();
	}
    else if (g_idle_context.IsOnIdleScreen) /* update idle screen */
    {
    #ifdef __GDI_MEMORY_PROFILE_2__
        if (mmi_idle_is_allow_to_display_song_name() == MMI_TRUE)
        {
            idle_screen_show_network_details();
        }
    #else /* __GDI_MEMORY_PROFILE_2__ */ 
        RedrawCategoryFunction();
    #endif /* __GDI_MEMORY_PROFILE_2__ */ 
    }
	
#ifdef __MMI_SUBLCD__
	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
		ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_paused
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*    void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_paused(void)
{
	// update state
	g_audply.state = STATE_PAUSED;
	#ifdef __MMI_RUNNING_LED__
		StopLEDPatternMusicRunning();
	#endif
	
	//mmi_audply_update_play_seconds(); //chenhe commit for jasper bug
	StopTimer( AUDPLY_COUNT_PLAY_TIMER );

    #ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
        mmi_audply_lyric_check_backlight(0);
    #endif 
	if( g_audply.in_main_screen )
	{
		mmi_audply_redraw_main_buttons();
	}
	else if(g_idle_context.IsOnIdleScreen)
	{
	mmi_trace(g_sw_MP3," On Idle Screen!");

    #ifdef __GDI_MEMORY_PROFILE_2__

        if (mmi_idle_is_allow_to_display_song_name() == MMI_TRUE)
        {
            idle_screen_show_network_details();
        }
    #else /* __GDI_MEMORY_PROFILE_2__ */ 
        RedrawCategoryFunction();
    #endif /* __GDI_MEMORY_PROFILE_2__ */ 
	
	}

#ifdef __MMI_SUBLCD__
	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
		ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_played
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*    void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_played(void)
{
#if 0
	// update state
	g_audply.state = STATE_PLAY;
	
	mmi_audply_start_record_play_seconds();
	StartTimer( AUDPLY_COUNT_PLAY_TIMER, 500, mmi_audply_timer );
	
	g_audply_main_selected_button = (signed char) AUDPLY_MAIN_PLAY; // for prev, next button
	g_audply_main_selected_button_down = FALSE;

	if( g_audply.in_main_screen )
		mmi_audply_redraw_main_buttons();
	else if( g_idle_context.IsOnIdleScreen ) // update idle screen
		RedrawCategoryFunction();

#ifdef __MMI_SUBLCD__
	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
		ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
#endif
mmi_trace(g_sw_MP3,"mmi_audply_played");
    #ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
        /* re-inialize the lyric. */
        lyric_parse_ok = mmi_audply_init_lyric();

	mmi_trace(g_sw_MP3,"lyric_parse_ok=%d", lyric_parse_ok);
	
        /* re-direct the target_layer, cause init_lyric will clean it to 0 ; */
    #ifdef __GDI_MEMORY_PROFILE_2__
        audply_lyric_print.target_layer = audply_osd_layer;
    #endif /*__GDI_MEMORY_PROFILE_2__*/
        lyric_reEntry = MMI_TRUE;
    #endif /* __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__ */ 
	g_audply.state = STATE_PLAY;
	#ifdef __MMI_RUNNING_LED__
		StartLEDPatternMusicRunning();
	#endif
	mmi_trace(g_sw_MP3,"mmi_audply_played");
	mmi_audply_start_record_play_seconds();
    { 
    #ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__    
        if (g_audply.lyrics_display &&
			g_audply.in_main_screen &&
			mmi_audply_is_lyrics_valid())
        {
        	mmi_trace(g_sw_MP3,"mmi_audply_lyric_on_time()");
              mmi_audply_lyric_on_time();
        }
    #endif /*__MMI_AUDIO_PLAYER_LYRIC_DISPLAY__*/
    }
	StartTimer( AUDPLY_COUNT_PLAY_TIMER, 200, mmi_audply_timer );
	
	g_audply_main_selected_button = (signed char) AUDPLY_MAIN_PLAY; // for prev, next button
	mmi_audply_cancel_fast_timer_no_condition();
	g_audply_main_selected_button_down = FALSE;
    #ifdef __MMI_AUDIO_PLAYER_LYRIC_DISPLAY__
        mmi_audply_lyric_check_backlight(1);
    #endif 

	if( g_audply.in_main_screen )
		mmi_audply_redraw_main_buttons();
	else if( g_idle_context.IsOnIdleScreen ) // update idle screen
		RedrawCategoryFunction();
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_msdc_plug_in_hdlr
* DESCRIPTION
*     handle CARD plug-in event
* IMPACT 
*     
* PARAMETERS
*     BOOL has_popup
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_msdc_plug_in_hdlr( BOOL has_popup )
{
	if( g_mmi_audply_init_done )
	{
		if( g_audply.preferred_list == LIST_FROM_CARD && g_audply.present_list == LIST_FROM_PHONE )
		{
			// switch present list to CARD
			g_audply.present_list = LIST_FROM_CARD;
			
			mmi_audply_regen_and_reload_playlist( (BOOL) g_audply.list_auto_gen, TRUE );
			
			// go to audio player main screen if it's not in call screen
			if( !IsScreenPresent( GetCmMarkerScrnID() ) )
			{
				if( DeleteUptoScrID( SCR_ID_AUDPLY_MAIN ) && !has_popup )
					GoBackHistory();
				else if( !has_popup && g_audply.in_main_screen )
					mmi_audply_update_main();
			}
			else  
			{
				U16 start_scr_id, end_scr_id;
				if( GetNextScrnIdOf( SCR_ID_AUDPLY_MAIN, &start_scr_id ) && GetPreviousScrnIdOf( GetCmMarkerScrnID(), &end_scr_id ) )
					DeleteBetweenScreen( start_scr_id, end_scr_id );
			}
		}
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_msdc_plug_out_hdlr
* DESCRIPTION
*     handle CARD plug-out event
* IMPACT 
*     
* PARAMETERS
*     BOOL has_popup
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_msdc_plug_out_hdlr( BOOL has_popup )
{
	if( g_mmi_audply_init_done )
	{
		if( g_audply.preferred_list == LIST_FROM_CARD && g_audply.present_list == LIST_FROM_CARD )
		{
			// switch present list to PHONE
			g_audply.present_list = LIST_FROM_PHONE;
			
			mmi_audply_regen_and_reload_playlist( (BOOL) g_audply.list_auto_gen, TRUE );
			
			// go to audio player main screen if it's not in call screen
			if( !IsScreenPresent( GetCmMarkerScrnID() ) )
			{
				if( DeleteUptoScrID( SCR_ID_AUDPLY_MAIN ) && !has_popup )
					GoBackHistory();
				else if( !has_popup && g_audply.in_main_screen )
					mmi_audply_update_main();
			}
			else  
			{
				U16 start_scr_id, end_scr_id;
				if( GetNextScrnIdOf( SCR_ID_AUDPLY_MAIN, &start_scr_id ) && GetPreviousScrnIdOf( GetCmMarkerScrnID(), &end_scr_id ) )
					DeleteBetweenScreen( start_scr_id, end_scr_id );
			}
		}
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_is_play_activated
* DESCRIPTION
*     indicate whether will audio player (or single song) play or attempt to play song soon
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_audply_is_play_activated(void)
{
	return single_play_state == STATE_PLAY || g_audply.state == STATE_PLAY || g_audply.wait_next;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_is_playing
* DESCRIPTION
*     indicate whether will audio player is playing a song
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_audply_is_playing(void)
{
	return g_mmi_audply_init_done &&  g_audply.state == STATE_PLAY;
}

BOOL mmi_audply_is_in_app(void)
{
	return g_mmi_audply_init_done &&  g_audply.state != STATE_IDLE;
}

#endif   /* __MMI_AUDIO_PLAYER__ */

/*****************************************************************************
* FUNCTION
*     mmi_audply_err_str
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S32 result
* RETURNS
*     U16
* GLOBALS AFFECTED
*     
*****************************************************************************/
U16 mmi_audply_err_str( S32 result )
{
	switch( result )
	{
#ifdef __DRM_SUPPORT__
	case DRM_NOT_PERMIT_TO_PLAY:
		return STR_GLOBAL_DRM_PROHIBITED;
#endif
	case MDI_AUDIO_FAIL:
		return STR_ID_AUDPLY_COMMAND_ERROR;
	case MDI_AUDIO_BUSY:
		return STR_ID_AUDPLY_DEVICE_BUSY;
	case MDI_AUDIO_DISC_FULL:
		return STR_ID_AUDPLY_LIST_FULL;
	case MDI_AUDIO_OPEN_FILE_FAIL:
		return STR_ID_AUDPLY_OPEN_FILE_ERROR;
	case MDI_AUDIO_BAD_FORMAT:
		return STR_ID_AUDPLY_BAD_FORMAT;
	case MDI_AUDIO_INVALID_FORMAT:
		return STR_ID_AUDPLY_INVALID_FORMAT;
	case MDI_AUDIO_UNSUPPORTED_CHANNEL:
		return STR_ID_AUDPLY_UNSUPPORTED_CHANNEL;
	case MDI_AUDIO_UNSUPPORTED_FREQ:
		return STR_ID_AUDPLY_UNSUPPORTED_FREQ;
	case MDI_AUDIO_UNSUPPORTED_TYPE:
		return STR_ID_AUDPLY_UNSUPPORTED_TYPE;
	case MDI_AUDIO_ERROR:
		return STR_ID_AUDPLY_ERROR;
	case MDI_AUDIO_NO_SPACE:
		return STR_ID_AUDPLY_NO_SPACE;
	default:
		return STR_ID_AUDPLY_COMMAND_ERROR;
   }
}
#if 0     //It's redundant codes, deleted by kecx for fixing bug 11790 on 20090604
void play_time()
{
	single_play_time++;
	coolsand_UI_start_timer(1000,play_time);
}
#endif
extern U8 single_volume;
U32 global_var_uptime(BOOL bSet);
/*****************************************************************************
* FUNCTION
*     mmi_audply_do_single_play_action
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     mdi_result result
* RETURNS
*     S32
* GLOBALS AFFECTED
*     
*****************************************************************************/
S32 mmi_audply_do_single_play_action( UI_string_type filefullname )
{
	/*the variable of single_volume save current volume of audio player.-for fixing bug-13008 by liuxl on 20090706*/
	mdi_result result = mdi_audio_play_file_with_vol_path( filefullname, DEVICE_AUDIO_PLAY_ONCE, &mdi_h2,
		mmi_audply_single_play_callback_hdlr, single_volume, MDI_DEVICE_SPEAKER2,0);
	//single_volume = GetRingVolumeLevel();     //It's redundant code. commented by liuxl on 20090706
	single_play_duration = 0;
	if( result == MDI_AUDIO_SUCCESS )
	{
		single_play_state = STATE_PLAY;
		kal_get_time( &single_play_time );
		StartTimer( AUDPLY_SINGLE_PLAY_TIMER, 500, mmi_audply_single_play_timer );
	}
	else
		single_play_state = STATE_IDLE;

	return (S32) result;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_do_single_stop_action
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
void mmi_audply_do_single_stop_action(void)
{
	single_play_state = STATE_IDLE;
	mdi_audio_stop_file();
	StopTimer( AUDPLY_SINGLE_PLAY_TIMER );
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_single_play_callback_hdlr
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     mdi_result result
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_single_play_callback_hdlr( mdi_result result )
{
	if( single_play_state == STATE_IDLE )
		return;
	
	switch( result )
	{
	case MDI_AUDIO_END_OF_FILE:
		StopTimer( AUDPLY_SINGLE_PLAY_TIMER );
		single_play_state = STATE_IDLE;
		if( in_single_play_screen )
			GoBackHistory();
		else
			DeleteNScrId( SCR_ID_AUDPLY_PLAY_ANIMATION ); //under popup screen
		break;
	case MDI_AUDIO_TERMINATED:
		single_play_state = STATE_IDLE;
		StopTimer( AUDPLY_SINGLE_PLAY_TIMER );
		if( in_single_play_screen )
			GoBackHistory();
		else
			DeleteNScrId( SCR_ID_AUDPLY_PLAY_ANIMATION ); //under popup screen
		break;
	default:
		single_play_state = STATE_IDLE;
		StopTimer( AUDPLY_SINGLE_PLAY_TIMER );
		DisplayPopup( (PU8)GetString( mmi_audply_err_str( (S32)result ) ), IMG_GLOBAL_WARNING, 0, 1000, (U8)WARNING_TONE );
		DeleteNScrId( SCR_ID_AUDPLY_PLAY_ANIMATION );
		break;
	}
}

