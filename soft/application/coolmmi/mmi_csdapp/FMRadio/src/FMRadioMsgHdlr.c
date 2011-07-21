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

#include "mmi_features.h"

#ifdef __MMI_FM_RADIO__
#define __NEWSIMULATOR
#include "globaldefs.h"
#include "globalscrenum.h"
#include "commonscreens.h"

#include "kal_release.h"
#include "aud_defs.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "mdi_include.h"
#include "settingprofile.h"

#include "fmradiodef.h"
#include "fmradiotype.h"
#include "fmradioprot.h"
#include "fmradiomainscreen.h"

#if defined(__MMI_FM_RADIO_RECORD__)
#include "fs_type.h"
#include "fs_errcode.h"
#if defined(__DRM_SUPPORT__)
#include "fs_errcode.h"
#include "drm_gprot.h"
#endif
#include "keybrd.h"
#endif
#undef __NEWSIMULATOR

#include "mmi_trace.h"

#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"

static void (*search_all_channels_callback_func)(U8* signal_band);

typedef struct
{
	U16 freq;
	U16 stop_freq;
	BOOL is_step_up;
} mmi_fmrdo_search_one_struct;

typedef struct
{
	U16 get_signal_count;
	U16 sub_band_mask;
	U8 signal_band[FM_BANDWIDTH+1];
	U8 select_count;
} mmi_fmrdo_search_all_struct;

static mmi_fmrdo_search_all_struct fmrdo_search_all;
static mmi_fmrdo_search_one_struct fmrdo_search_one;
static U16 g_fmrdo_search_cancel_count = 0;

extern mmi_fmrdo_struct g_fmrdo; // defined in FMRadioSrc.c
 
extern BOOL fmr_play_from_bg_to_fg;// defined in mdi_audio.c


BOOL mmi_fmrdo_continued( S32 result )
{
	if( g_fmrdo.is_preempted )
	{
		mmi_fmrdo_power_on( TRUE );
		if( g_fmrdo.in_main_screen )
		{
			mmi_fmrdo_redraw_main_toggle_button( FMRDO_BUTTON_POWER, FALSE, g_fmrdo.is_power_on ,TRUE);
			if( g_fmrdo.is_button_down && g_fmrdo.selected_button == FMRDO_BUTTON_POWER )
			{
				g_fmrdo.selected_button = FMRDO_BUTTON_NONE;
				g_fmrdo.is_button_down = FALSE;
			}
 			else if( mmi_fmrdo_is_stepping_up() || mmi_fmrdo_is_stepping_down() )
				mdi_fmr_mute( 1 ); // mute in the begining of step down/up
		}
		else if( g_idle_context.IsOnIdleScreen ) // update idle screen
			RedrawCategoryFunction();

		return TRUE;
	}

	return FALSE;
}

void mmi_fmrdo_preempted( S32 result )
{
	if( g_fmrdo.state == FMRDO_STATE_NOT_READY )
		return;

#if defined(__MMI_FM_RADIO_RECORD__)
	/*Stop FM radio recording, but do not stop playing*/
	if( (g_fmrdo.state == FMRDO_STATE_RECORD || g_fmrdo.state == FMRDO_STATE_RECORD_PAUSE ) && 
	     result == MDI_AUDIO_STOP_FM_RECORD)
	{
		/*Display enter file name screen*/
		mmi_fmrdo_record_stopped_hdlr();
		
		g_fmrdo.state = FMRDO_STATE_READY;

		return;
	}
	/*Stop FM radio recording, then stop FM radio*/
	else if(g_fmrdo.state == FMRDO_STATE_RECORD || g_fmrdo.state == FMRDO_STATE_RECORD_PAUSE)
	{
		g_fmrdo.state = FMRDO_STATE_READY;
		/*Save file using default name*/

		mmi_fmrdo_entry_restore_main_screen();
		
		/*record not finish correctly, show the error, and remain FM radio playing*/
		if(result != MDI_AUDIO_TERMINATED)
		{
			mmi_fmrdo_record_error_hdlr(result);
			return;
		}
	}
#endif

	/*Stop FM Playing*/
	mmi_fmrdo_search_frequency_stop();
	mmi_fmrdo_search_all_channels_stop();
	g_fmrdo.is_power_on = FALSE;
	g_fmrdo.is_preempted = TRUE;
	g_fmrdo.state = FMRDO_STATE_NOT_READY;
	if( g_fmrdo.in_main_screen )
	{
		// update freqency text color
		mmi_fmrdo_redraw_main_frequency( g_fmrdo.frequency, TRUE );
		mmi_fmrdo_redraw_main_toggle_button( FMRDO_BUTTON_POWER, FALSE, FALSE ,TRUE);
		if( g_fmrdo.is_button_down && g_fmrdo.selected_button == FMRDO_BUTTON_POWER )
		{
			g_fmrdo.selected_button = FMRDO_BUTTON_NONE;
			g_fmrdo.is_button_down = FALSE;
		}
	}
	else if( g_idle_context.IsOnIdleScreen ) // update idle screen
		RedrawCategoryFunction();
#ifdef __MMI_SUBLCD__
	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
		ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
}

void mmi_fmrdo_goto_channel( U16 channel_index )
{
	U16 freq = g_fmrdo.channel_list.freq[channel_index];
	
	FMR_TRACE();
	
	g_fmrdo.frequency = freq;
	g_fmrdo.channel_index = channel_index;

	if( g_fmrdo.in_main_screen )
	{
		mmi_fmrdo_redraw_main_frequency( freq, TRUE );
		mmi_fmrdo_redraw_main_channel_tuner( freq );
		mmi_fmrdo_redraw_main_channel_name( 0 );
	}
	
	if( g_fmrdo.is_power_on )
	{
		FMR_TRACE_ARG("mdi_fmr_set_freq( %d ).", freq);
		mdi_fmr_set_freq( freq );
	}

#ifdef __MMI_SUBLCD__
	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
		ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
}

void mmi_fmrdo_set_frequency( U16 freq )
{
	S16 channel_index;
	S16 error;
	
        FMR_TRACE();

        FMR_TRACE_ARG("g_fmrdo.in_main_screen= %d",g_fmrdo.in_main_screen);
        FMR_TRACE_ARG("g_fmrdo.is_power_on = %d",g_fmrdo.is_power_on);
        
	g_fmrdo.frequency = freq;
	//Added by liuxl for fixing bug-13554 on 20090730-Start
	WriteValue( NVRAM_FMRDO_FREQUENCY, &g_fmrdo.frequency, DS_SHORT, &error );
	//Added by liuxl for fixing bug-13554 on 20090730-End

        FMR_TRACE_ARG("g_fmrdo.frequency = %d", g_fmrdo.frequency);

	if( (channel_index = mmi_fmrdo_match_channel_list( freq )) >= 0 )
	{
		g_fmrdo.channel_index = channel_index;
                FMR_TRACE_ARG("channel_index = %d",channel_index);
	}
	else
		g_fmrdo.channel_index = -1;

	if( g_fmrdo.in_main_screen )
	{
		mmi_fmrdo_redraw_main_frequency( freq, TRUE );
		mmi_fmrdo_redraw_main_channel_tuner( freq );
		mmi_fmrdo_redraw_main_channel_name( 0 );
	}
	
	if( g_fmrdo.is_power_on )
	{
		FMR_TRACE_ARG("mdi_fmr_set_freq( %d ).", freq);
		mdi_fmr_set_freq( freq );
	}		

#ifdef __MMI_SUBLCD__
	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
		ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
}

void mmi_fmrdo_power_on_before_preset_auto_search( void )
{
        FMR_TRACE();

	g_fmrdo.is_preempted = FALSE;
#if 0//jiashuo change , for our platform can not support mdi_check_device_ownership function
	if( !mdi_check_device_ownership() )
		g_fmrdo.is_power_on = FALSE;
	else
	{
		// mute it before searching
		mdi_audio_set_mute( AUD_VOLUME_FMR, TRUE );
		
		mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER2, (mdi_callback) mmi_fmrdo_preempted );
		g_fmrdo.is_power_on = TRUE;
		g_fmrdo.state = FMRDO_STATE_READY;
	}
#else
	// mute it before searching
	mdi_audio_set_mute( AUD_VOLUME_FMR, TRUE );
// beginning--modified by kecx for fixing bug 11506 on 20090612
	if(g_fmrdo.is_audio_out)
	{
		mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER_BOTH, (mdi_callback) mmi_fmrdo_preempted );

	}
	else
	{
		mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER2, (mdi_callback) mmi_fmrdo_preempted );

	}
// end--modified by kecx for fixing bug 11506 on 20090612
	g_fmrdo.is_power_on = TRUE;
	g_fmrdo.state = FMRDO_STATE_READY;
#endif

}
void mmi_fmrdo_power_on( BOOL turn_on )
{
			S16 channel_index;
        FMR_TRACE();

	if( !g_fmrdo.is_ready )
		return;

	//FMR_TRACE_ARG("[########FMRadio] mmi_fmrdo_power_on()  g_fmrdo.state  == %d.",g_fmrdo.state );
	//FMR_TRACE_ARG("[########FMRadio] mmi_fmrdo_power_on()  g_fmrdo.is_power_on  == %d.",g_fmrdo.is_power_on );
	//FMR_TRACE_ARG("[########FMRadio] mmi_fmrdo_power_on()  turn_on  == %d.", turn_on );
	
	if( (g_fmrdo.state == FMRDO_STATE_NOT_READY && !turn_on) )//|| (g_fmrdo.state != FMRDO_STATE_NOT_READY && turn_on) )
	{
		g_fmrdo.is_preempted = FALSE;
		FMR_TRACE_ARG("[########FMRadio] mmi_fmrdo_power_on()  g_fmrdo.is_preempted  == FALSE.");
		return;
	}
	
	if( turn_on )
	{      
		//if( !mdi_check_device_ownership() )    //shizh for debug
		if( mdi_check_device_ownership() )
		{
			g_fmrdo.is_power_on = FALSE;
		}
		else
		{
			
			//mdi_audio_set_volume( AUD_VOLUME_FMR, g_fmrdo.volume );
			/*yanghong 20060407 add to select the if audio is played out*/
			if(g_fmrdo.is_audio_out)
			{ 
				//DRV_GpioSetLevel(DM_DEVID_GPIO_20, DRV_GPIO_HIGH);
				//DRV_GpioSetLevel(DM_DEVID_GPIO_6, DRV_GPIO_HIGH);
				//MCI_AudioSetOutputPath(4,0);//for jessamine
				FMR_TRACE_ARG("===============audio out on=================\r\n");
				mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER_BOTH, (mdi_callback) mmi_fmrdo_preempted );

			}
			else /*yanghong add end*/
			{
				//DRV_GpioSetLevel(DM_DEVID_GPIO_20, DRV_GPIO_LOW);
			    //DRV_GpioSetLevel(DM_DEVID_GPIO_6, DRV_GPIO_LOW);
				//MCI_AudioSetOutputPath(3,0);//for jessamine
			    FMR_TRACE_ARG("==========audio out off================\r\n");
				mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER2, (mdi_callback) mmi_fmrdo_preempted );

			}

	        if(!fmr_play_from_bg_to_fg) /*if is not condition that when fm radio is played from background to foreground*/
			{
				mdi_fmr_set_freq( g_fmrdo.frequency );
				mdi_audio_set_volume( AUD_VOLUME_FMR, g_fmrdo.volume );//change the set volume function position for jessamine,20080505
			}
			else
			{
				/*after fm radio is played from background to foreground,set the flag as FALSE*/
				fmr_play_from_bg_to_fg = FALSE;
			}
			
			g_fmrdo.state = FMRDO_STATE_READY;
			g_fmrdo.is_power_on = TRUE;
			
			if( (channel_index = mmi_fmrdo_match_channel_list( g_fmrdo.frequency )) >= 0 )
			{
				g_fmrdo.channel_index = channel_index;
			}
			else
			{
				g_fmrdo.channel_index = -1;
			}
		}
		
		if( g_fmrdo.in_main_screen )
		{	
			mmi_fmrdo_redraw_main_frequency( g_fmrdo.frequency, TRUE );
#ifdef __MMI_FM_RADIO_ANIMATION__			
			mmi_fmrdo_animation_start();
#endif			
		}
	}
	else
	{
		mdi_audio_stop_fmr();
#ifdef __MMI_FM_RADIO_ANIMATION__
		mmi_fmrdo_animation_stop();
#endif
#if defined(__MMI_MAINLCD_220X176__)
	
	g_fmrdo.is_preempted = FALSE;
#endif 

	}
#if !defined(__MMI_MAINLCD_220X176__)

	g_fmrdo.is_preempted = FALSE;
#endif 

#ifdef __MMI_SUBLCD__
	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
		ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
}


BOOL mmi_fmrdo_state_is_idle(void)
{

  return (g_fmrdo.state == FMRDO_STATE_NOT_READY);
  
}


void mmi_fmrdo_search_frequency_start( U16 start_freq, BOOL is_step_up )
{
	FMR_TRACE();
	// mute it before searching
	mdi_audio_set_mute( AUD_VOLUME_FMR, TRUE );

	fmrdo_search_one.freq = start_freq;
	if( is_step_up )
	{
		if( start_freq > MIN_FM_FREQUENCY )
			fmrdo_search_one.stop_freq = start_freq - 1;
		else
			start_freq = MAX_FM_FREQUENCY;
	}
	else
	{
		if( start_freq < MAX_FM_FREQUENCY )
			fmrdo_search_one.stop_freq = start_freq + 1;
		else
			start_freq = MIN_FM_FREQUENCY;
	}
	fmrdo_search_one.is_step_up = is_step_up;
	g_fmrdo.state = FMRDO_STATE_SEARCH_ONE;
	
	// start valid stop test
	//FMR_TRACE_ARG("start_freq = %d.", start_freq);
	mdi_fmr_check_is_valid_stop( start_freq, is_step_up, mmi_fmrdo_search_frequency );

#ifdef __MMI_SUBLCD__
	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
		ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
}

void mmi_fmrdo_search_frequency( BOOL is_valid )
{
	//FMR_TRACE();
	
	if(g_fmrdo_search_cancel_count > 0)
	{
	    g_fmrdo_search_cancel_count--;
	    return;
	}

	if( g_fmrdo.state != FMRDO_STATE_SEARCH_ONE )
		return;
	
	if( !is_valid && fmrdo_search_one.freq != fmrdo_search_one.stop_freq )
	{
		// update frequency
		if( fmrdo_search_one.is_step_up )
		{
			if( fmrdo_search_one.freq < MAX_FM_FREQUENCY )
				fmrdo_search_one.freq++;
			else
				fmrdo_search_one.freq = MIN_FM_FREQUENCY;
		}
		else
		{
			if( fmrdo_search_one.freq > MIN_FM_FREQUENCY )
				fmrdo_search_one.freq--;
			else
				fmrdo_search_one.freq = MAX_FM_FREQUENCY;
		}

		// update display
		if( g_fmrdo.in_main_screen )
		{
			mmi_fmrdo_redraw_main_frequency( fmrdo_search_one.freq, TRUE );
			mmi_fmrdo_redraw_main_channel_tuner( fmrdo_search_one.freq );
		}

		// test valid stop again
		mdi_fmr_check_is_valid_stop( fmrdo_search_one.freq, fmrdo_search_one.is_step_up, mmi_fmrdo_search_frequency );
	}
	else
	{
		// don't move this statement below mmi_fmrdo_set_frequency for properly updating subLCD
		g_fmrdo.state = FMRDO_STATE_READY;
		mmi_fmrdo_set_frequency( fmrdo_search_one.freq );
		mdi_audio_set_volume( AUD_VOLUME_FMR, g_fmrdo.volume );
	}
}

void mmi_fmrdo_search_frequency_stop( void )
{		
        FMR_TRACE();
        
	if( g_fmrdo.is_ready && g_fmrdo.state == FMRDO_STATE_SEARCH_ONE )
	{
	    g_fmrdo_search_cancel_count++;
		//mdi_fmr_cancel_checking_is_valid_stop();
		// don't move this statement below mmi_fmrdo_set_frequency for properly updating subLCD
		g_fmrdo.state = FMRDO_STATE_READY;
		mmi_fmrdo_set_frequency( fmrdo_search_one.freq );
		mdi_audio_set_volume( AUD_VOLUME_FMR, g_fmrdo.volume );
	}
}

S16 mmi_fmrdo_find_best_frequency_in_sub_band( U8* sub_band )
{
	U8 signal_level;
	U8 max_signal_level = 1;
	S16 i;
	S16 index = -1;

	for( i = 0; i < FM_SUB_BANDWIDTH; i++ )
	{
		if( !FM_SIG_MARK( sub_band[i] ) )
		{
			signal_level = FM_SIG_LVL( sub_band[i] );
			if( signal_level >= max_signal_level )
			{
				index = i;
				max_signal_level = signal_level;
			}
		}
	}

	return index;
}

void mmi_fmrdo_search_all_channels_start( void (*callback_func)(U8* signal_band) )
{
	// mute it before searching
	mdi_audio_set_mute( AUD_VOLUME_FMR, TRUE );
	
	/// Add by JoeTsou for auto search 0508
//    FMDrv_EvaluateRSSIThreshold();
	
	search_all_channels_callback_func = callback_func;
	g_fmrdo.state = FMRDO_STATE_SEARCH_ALL;

	// init search_p
	memset( &fmrdo_search_all, 0, sizeof(mmi_fmrdo_search_all_struct) );

	// populate signal band
	mdi_fmr_get_signal_level( MIN_FM_FREQUENCY, 1, mmi_fmrdo_search_all_channels );

#ifdef __MMI_SUBLCD__
	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
		ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
}

void mmi_fmrdo_search_all_channels( U8 signal_level )
{
	U16 i, j, offset;
	U16 avaliable_sub_band_count;
	mmi_fmrdo_search_all_struct* search_p = &fmrdo_search_all;

	if( g_fmrdo.state != FMRDO_STATE_SEARCH_ALL )
		return;

	search_p->signal_band[search_p->get_signal_count] = signal_level;
  search_p->get_signal_count++;
	if( search_p->get_signal_count < FM_BANDWIDTH )
		mdi_fmr_get_signal_level( (U16) (MIN_FM_FREQUENCY + search_p->get_signal_count), 1, mmi_fmrdo_search_all_channels ); // populate signal band
	else
	{
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif		
		// search until no available frequency or FM_SUB_BAND_NUM frequencies are found
		avaliable_sub_band_count = FM_SUB_BAND_NUM;
		do
		{
			S16 index;
			
			for( offset = i = 0; i < FM_SUB_BAND_NUM; i++ )
			{        
				if( !FM_SUB_MARK( search_p->sub_band_mask, i ) )
				{
					index = mmi_fmrdo_find_best_frequency_in_sub_band( &search_p->signal_band[offset] );
					if( index >= 0 )
					{
						j = offset + index;
						FM_MARK_SIG( search_p->signal_band[j] );
						search_p->select_count++;
						if( search_p->select_count == FM_SUB_BAND_NUM )
							goto finished;
					}
					else
					{
						FM_MARK_SUB( search_p->sub_band_mask, i );
						avaliable_sub_band_count--;
					}
				}
				offset += FM_SUB_BANDWIDTH;
			}
		}while( avaliable_sub_band_count > 0 );

	finished:
		if( search_all_channels_callback_func != NULL )
		{
#if defined(__MMI_FM_RADIO_FIRST_CHANNEL__)
			MMI_BOOL is_channel_found = MMI_FALSE;
			
			for( i = 0; i < FM_BANDWIDTH; i++ )
			{
				if( FM_SIG_MARK( search_p->signal_band[i] ) )
				{
					/* Set first channel to play after search */
					mdi_fmr_set_freq( (U16)(i + MIN_FM_FREQUENCY));
					g_fmrdo.frequency = i + MIN_FM_FREQUENCY;
					is_channel_found = MMI_TRUE;
					break;
				}
			}

			if(!is_channel_found)
#endif
			{
				mdi_fmr_set_freq( g_fmrdo.frequency );
			}
			
			mdi_audio_set_volume( AUD_VOLUME_FMR, g_fmrdo.volume );
			search_all_channels_callback_func( search_p->signal_band );
			search_all_channels_callback_func = NULL;
#ifdef __MMI_SUBLCD__
			if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
				ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
		}
	}
}

void mmi_fmrdo_search_all_channels_stop( void )
{
	if( g_fmrdo.is_ready && g_fmrdo.state == FMRDO_STATE_SEARCH_ALL )
	{
		mdi_fmr_set_freq( g_fmrdo.frequency );
		mdi_audio_set_volume( AUD_VOLUME_FMR, g_fmrdo.volume );
		search_all_channels_callback_func = NULL;
		mdi_fmr_cancel_getting_signal_level();
		g_fmrdo.state = FMRDO_STATE_READY;
		DeleteScreenIfPresent( SCR_ID_FMRDO_SEARCHING );

#ifdef __MMI_SUBLCD__
		if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
			ShowIdleAppSubLcd();
#endif // __MMI_SUBLCD__
	}
}

#if defined(__MMI_FM_RADIO_RECORD__)
/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_start_record
* DESCRIPTION
*   Start to record FM radio
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_start_record(UI_string_type filename, U8 format)
{
	U8 file_format;
	mdi_result result;
   
#ifdef MMI_ON_HARDWARE_P
#ifdef __DRM_SUPPORT__
	{
		FS_HANDLE handle;
		BOOL is_permitted = FALSE;

		if( (handle = DRM_open_file( (PU16)filename, FS_READ_ONLY, DRM_PERMISSION_NONE )) >= FS_NO_ERROR )
		{
			if( DRM_get_rights( handle ) == NULL )
				is_permitted = TRUE;
			DRM_close_file( handle );
		}
		else if( handle < -1 ) /* file open error. but not DRM related problem*/
		{
			is_permitted = TRUE;
		}

		if( !is_permitted )
		{
			mmi_fmrdo_record_error_hdlr(DRM_NOT_PERMIT_TO_APPEND);
			return;
		}
	}
#endif
	switch(format)
	{

		case FM_FORMAT_AMR:
			file_format = MEDIA_AMR;
			break;

		case FM_FORMAT_MP3:
			file_format = MEDIA_DAF;
			break;		
		case FM_FORMAT_WAV:
			file_format = MEDIA_WAV_DVI_ADPCM;
			break;
		default:
			file_format = MEDIA_DAF;
			break;
	}
#else
	file_format = 0;
#endif

      result=mdi_fmr_start_record(filename, file_format, NULL, mmi_fmrdo_preempted);
      if(result==MDI_AUDIO_SUCCESS)
      {
      	g_fmrdo.state = FMRDO_STATE_RECORD;
      	mmi_fmrdo_entry_main_record_screen();

		mmi_fmrdo_util_init_duration();
		mmi_fmrdo_util_start_duration();
      }
      else
      {
		mmi_fmrdo_record_error_hdlr(result);
      }

}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_stop_record
* DESCRIPTION
*   Stop record FM radio
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_stop_record(void)
{
	S32 result;

	/* State will update in mmi_fmrdo_preempted() when call back*/
	if((result = mdi_audio_stop_fmr_record_only()) != MDI_AUDIO_SUCCESS)
	{
		/*No reason not stop, error Handling Here.*/
		g_fmrdo.state = FMRDO_STATE_READY;
			
		mmi_fmrdo_entry_restore_main_screen();
		mmi_fmrdo_record_error_hdlr(result);		
	}
	else
	{
		g_fmrdo.is_power_on = TRUE; 
		mmi_fmrdo_power_on( TRUE );        
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_stop_record_on_exit
* DESCRIPTION
*   Always stop FM radio record when exit the main screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_stop_record_on_exit(void)
{
	S32 result;

	if(g_fmrdo.state == FMRDO_STATE_RECORD || g_fmrdo.state == FMRDO_STATE_RECORD_PAUSE)
	{
		if((result = mdi_audio_stop_fmr_record_only()) != MDI_AUDIO_SUCCESS)
		{
			/*No reason not stop, error Handling Here.*/
			g_fmrdo.state = FMRDO_STATE_READY;
			mmi_fmrdo_record_error_hdlr(result);				
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_pause_record
* DESCRIPTION
*   Pause record FM radio
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_pause_record(void)
{
	mdi_result result;
	
	if( g_fmrdo.state == FMRDO_STATE_RECORD )
	{
		result = mdi_fmr_pause_record(mmi_fmrdo_preempted);

		mmi_fmrdo_util_stop_duration();
		if(result==MDI_AUDIO_SUCCESS)
		{
			g_fmrdo.state = FMRDO_STATE_RECORD_PAUSE;
			mmi_fmrdo_entry_main_pause_screen();
		}   
		else
		{
			g_fmrdo.state = FMRDO_STATE_READY;

			mmi_fmrdo_entry_restore_main_screen();
			mmi_fmrdo_record_error_hdlr(result);
	      }

		// clear key events to prevent external queue overflow
		ClearKeyEvents();
	}

}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_resume_record
* DESCRIPTION
*   Resume record FM radio from pause state.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_resume_record(void)
{
	mdi_result result;

	if( g_fmrdo.state == FMRDO_STATE_RECORD_PAUSE )
	{

		result=mdi_frm_resume_record(mmi_fmrdo_preempted);
		if(result==MDI_AUDIO_SUCCESS)
		{
			g_fmrdo.state = FMRDO_STATE_RECORD;

			mmi_fmrdo_util_start_duration();
			mmi_fmrdo_entry_main_record_screen();
		}   
		else
		{
			g_fmrdo.state = FMRDO_STATE_READY;
			
			mmi_fmrdo_entry_restore_main_screen();
			mmi_fmrdo_record_error_hdlr(result);
		}

		// clear key events to prevent external queue overflow
		ClearKeyEvents();
	}
}
#endif

#endif // __MMI_FM_RADIO__


