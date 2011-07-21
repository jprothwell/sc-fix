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
*	FMRadioSrc.c
*
* Project:
* --------
 
*
* Description:
* ------------
*   FM Radio main program
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 * 
 * 
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/

#include "mmi_features.h"

#ifdef __MMI_FM_RADIO__
#include "profilesdefs.h"
#include "globaldefs.h"
#include "globalscrenum.h"
#include "mainmenudef.h"
#include "commonscreens.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "profilegprots.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"

#ifdef __MMI_AUDIO_PLAYER__
extern void mmi_audply_stop_playing( void );
#endif // __MMI_AUDIO_PLAYER__

#include "aud_defs.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "device.h"
#include "settingprofile.h"
#include "timerevents.h"

#include "fmradiodef.h"
#include "fmradiotype.h"
#include "fmradioprot.h"
#include "fmradiomainscreen.h"
#if defined(__MMI_FM_RADIO_RECORD__)
#include "fs_type.h"
#include "filemanagerdef.h"
#include "filemanagergprot.h"
#include "filemgr.h"
#include "fs_func.h"
#ifdef MMI_ON_HARDWARE_P
#include "rtc_sw.h"
#endif

#include "usbdevicegprot.h"
#endif

#include "settingdefs.h"

#include "mmi_trace.h"
#if 0//defined(__PROJECT_GALLITE_C01__)
#include "dual_sim.h"
#include "mtpnp_ad_resdef.h"
#endif
 
mmi_fmrdo_struct g_fmrdo; // all memeber variables are zero initial
static BOOL channel_name_edit_done;
static U16 main_option_highlighted_item_text;
static U16 channel_list_highlighted_item_index;
static U16 channel_option_highlighted_item_text;
static U16 channel_name_edit_option_highlighted_item_text;
static UI_character_type channel_name_str[MAX_NAME_INPUT_LEN + 10]; // used for showing channel_name or freq in idle screen or subLCD

static S32 selected_background_play;
static S32 selected_audio_out;		//yanghong 20060406 add
static S32 selected_skin_index;
static UI_character_type manual_input_buffer[MAX_MANUAL_INPUT_LEN];
static UI_character_type name_inline_editor_buffer[MAX_NAME_INPUT_LEN];
static UI_character_type freq_inline_editor_buffer[MAX_MANUAL_INPUT_LEN];
static UI_character_type name_text_editor_buffer[MAX_NAME_INPUT_LEN];
static UI_string_type background_play_texts[2];
static UI_string_type audio_out_texts[2]; //yanghong 20060406 add
//static UI_string_type skin_texts[3];//delete fmradio skin select menu in setting

#if defined(__MMI_FM_RADIO_RECORD__)
static S32 selected_record_format;
static S32 selected_record_storage;
static UI_string_type record_format_texts[FM_REC_FORMAT_NUM];
//static UI_string_type record_storage_texts[FM_REC_STORAGE_NUM];
static UI_character_type fmrdo_editor_buff[MAX_FM_INPUT_FILE_LEN+2];
#endif

static BOOL fmrdo_key_press_down_first_flag = FALSE;
static U16 fmrdo_key_code_first=0;

extern wgui_inline_item wgui_inline_items[]; // defined in wgui_inline_edit.c
extern const mmi_fmrdo_skin_struct g_fmrdo_skins[]; // defined in resource_fmradio_skins.c
extern U8 isEarphonePlugged; // definedin gpio.c
extern U8 isEarphonePlugged_1; 
extern void kal_wsprintf(unsigned short *outstr, char *fmt,...);
extern void kal_get_time(kal_uint32 *ticks_ptr);
extern void mmi_fmrdo_entry_main(void);
extern void mmi_fmrdo_printf_resid(void);
#ifdef __MMI_GB__
extern void GBSetInputboxLSKFunction(FuncPtr f); 
#endif
extern void mmi_show_qq_select_confirm(void (*default_entry) (void));

extern BOOL HandleLowBatteryProcess(void);

#ifdef __MMI_FM_RADIO_CHECK_CHIP_OR_HIDE__
#ifdef MMI_ON_HARDWARE_P
extern BOOL FMR_IsChipValid( void ); // this function is definedin in l1audio
#else
#define FMR_IsChipValid(x) (TRUE)
#endif
S32 g_fmrdo_menu_item_index;


/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_skip_highlight_hdlr
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
void mmi_fmrdo_skip_highlight_hdlr( S32 index )
{
	if( g_fmrdo_menu_item_index >= 0 && index >= g_fmrdo_menu_item_index )
		index++;
	ExecuteCurrHiliteHandler( index );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_check_chip_or_hide
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     U16 n_items, U16* string_ids, U16* icon_ids
* RETURNS
*     U16
* GLOBALS AFFECTED
*     
*****************************************************************************/
U16 mmi_fmrdo_check_chip_or_hide( U16 n_items, U16* string_ids, U16* icon_ids )
{
	if( !FMR_IsChipValid() )
	{
		/* hide FM Radio application menu item */
		S16 i, j;
		for( i = n_items-1; i >= 0; i-- )
		{
			if( string_ids[i] == STR_ID_FMRDO_TITLE ) // look for FM Radio application string id
				break;
		}
		g_fmrdo_menu_item_index = i;
		if( i >= 0 ) // found FM Radio application string id
		{
			n_items--;
			for( j = i; j < n_items; j++ ) // move rest application string & image ids up
			{
				string_ids[j] = string_ids[j+1];
				icon_ids[j] = icon_ids[j+1];
			}
		}
	}
	else
		g_fmrdo_menu_item_index = -1;

	return n_items;
}
#endif /* __MMI_FM_RADIO_CHECK_CHIP_OR_HIDE__ */

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_set_subLCD_shown
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
void mmi_fmrdo_set_subLCD_shown( BOOL is_shown )
{
	g_fmrdo.is_subLCD_shown = is_shown;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_power_on_main_screen
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*    BOOL turn_on
* RETURNS
*    void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_power_on_main_screen( BOOL turn_on )
{
	if( g_fmrdo.in_main_screen )
		mmi_fmrdo_power_on( turn_on );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_is_power_on
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*    void
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_fmrdo_is_power_on( void )
{
	return (g_fmrdo.is_ready && g_fmrdo.is_power_on);
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_get_channel_name_or_freq
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*    void
* RETURNS
*     UI_string_type
* GLOBALS AFFECTED
*     
*****************************************************************************/
UI_string_type mmi_fmrdo_get_channel_name_or_freq( void )
{
	UI_string_type str = channel_name_str;
	U8 str1[MAX_NAME_INPUT_LEN];
	U8 str2[MAX_NAME_INPUT_LEN];	

	if( g_fmrdo.state == FMRDO_STATE_SEARCH_ALL || g_fmrdo.state == FMRDO_STATE_SEARCH_ONE )
		pfnUnicodeStrcpy( (S8*) str, (const S8*) GetString( STR_ID_FMRDO_SEARCHING ) );
	else
	{
		if( g_fmrdo.channel_index >= 0 )
		{
			if( pfnUnicodeStrlen((S8*)g_fmrdo.channel_list.name[g_fmrdo.channel_index]) > 0 )
			{
				//kal_wsprintf( str, "%d. %w", g_fmrdo.channel_index + 1, g_fmrdo.channel_list.name[g_fmrdo.channel_index] );
				sprintf(str1, "%d.", g_fmrdo.channel_index + 1);
				AnsiiToUnicodeString(str2, str1);
				pfnUnicodeStrcpy((S8 *)str,str2);
				pfnUnicodeStrcat((S8 *)str,(const S8 *)g_fmrdo.channel_list.name[g_fmrdo.channel_index]);

			}
			else
			{
				//kal_wsprintf( str, "%d. %w", g_fmrdo.channel_index + 1, g_fmrdo.channel_list.name[g_fmrdo.channel_index] );
				sprintf(str1, "%d.", g_fmrdo.channel_index + 1);
				AnsiiToUnicodeString(str2, str1);
				pfnUnicodeStrcpy((S8 *)str,str2);
				pfnUnicodeStrcat((S8 *)str,(const S8 *)g_fmrdo.channel_list.name[g_fmrdo.channel_index]);
			}
		}
		else
		{
			//kal_wsprintf( str, "FM %d.%d", g_fmrdo.frequency / 10, g_fmrdo.frequency % 10 );
			sprintf(str1,"FM %d.%d", g_fmrdo.frequency  / 10, g_fmrdo.frequency  % 10);
			AnsiiToUnicodeString((S8 *)str, str1);

		}
	}
	return str;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_preset_list_done
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     U8* signal_band
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_preset_list_done( U8* signal_band )
{
	U16 i, j;
	S16	error;
	
	g_fmrdo.state = FMRDO_STATE_READY;

	// clear channel list
	memset( g_fmrdo.channel_list.freq, 0, FM_RADIO_CHANNEL_NUM*2 );
	for( i = 0; i < FM_RADIO_CHANNEL_NUM; i++ )
		pfnUnicodeStrcpy( (S8*)g_fmrdo.channel_list.name[i], (S8*)L"" );

	// set channel list
	for( i = j = 0; i < FM_BANDWIDTH; i++ )
	{
		if( FM_SIG_MARK( signal_band[i] ) )
		{
			mmi_fmrdo_set_channel( j, (U16) (i + MIN_FM_FREQUENCY), (UI_string_type)GetString( STR_ID_FMRDO_NEW_CHANNEL ) );
			j++;
			if( j >= FM_RADIO_CHANNEL_NUM )  break; // this line should never be reached if the searching algorithm is correct!!
		}
	}

	// write channel list from nvram
	WriteRecord( NVRAM_EF_FM_RADIO_LID, 1, (void*)&g_fmrdo.channel_list, NVRAM_EF_FM_RADIO_SIZE, &error );
	
	// rematch channel index
	g_fmrdo.channel_index = mmi_fmrdo_match_channel_list( g_fmrdo.frequency );
	DisplayPopup( (PU8)GetString( STR_GLOBAL_DONE ), IMG_GLOBAL_ACTIVATED, 1, 1000, (U8)SUCCESS_TONE );
	DeleteScreenIfPresent( SCR_ID_FMRDO_SEARCHING );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_start_twinkle_frequency
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
void mmi_fmrdo_start_twinkle_frequency( void )
{
	g_fmrdo.twinkle_frequency_count = 0;
	StartTimer( FMRDO_TWINKLE_FREQUENCY_TIMER, TWINKLE_FREQUENCY_ON_TIME, mmi_fmrdo_twinkle_frequency_timer );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_stop_twinkle_frequency
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
void mmi_fmrdo_stop_twinkle_frequency( BOOL is_shown )
{
	StopTimer( FMRDO_TWINKLE_FREQUENCY_TIMER);
	if( is_shown && g_fmrdo.twinkle_frequency_count % 2 == 0 )
		mmi_fmrdo_redraw_main_frequency( g_fmrdo.frequency, TRUE );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_long_press_number_key
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
void mmi_fmrdo_long_press_number_key( void )
{
	U16 key_code, key_type;
	U16 channel_index;
	S16 error;

	FMR_TRACE();
	
	if( g_fmrdo.state != FMRDO_STATE_SEARCH_ONE )
	{
		// get key number
		GetkeyInfo( (U16*)&key_code, (U16*)&key_type );
		channel_index = key_code - KEY_1;
		
		// set channel
		mmi_fmrdo_set_channel( channel_index, g_fmrdo.frequency, (UI_string_type) GetString( STR_ID_FMRDO_NEW_CHANNEL ) );

		// write channel list from nvram
		WriteRecord( NVRAM_EF_FM_RADIO_LID, 1, (void*)&g_fmrdo.channel_list, NVRAM_EF_FM_RADIO_SIZE, &error );
		
		g_fmrdo.channel_index = channel_index;
		
		// update channel name
		mmi_fmrdo_redraw_main_channel_name( 0 );
		
		// start twinkle frequency
		mmi_fmrdo_start_twinkle_frequency();
	}
	g_fmrdo.is_long_press_number_key = TRUE;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_release_number_key
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
void mmi_fmrdo_release_number_key( void )
{
        U16 key_code, key_type;
        U16 freq;

        FMR_TRACE();

        if( g_fmrdo.state == FMRDO_STATE_SEARCH_ONE )
                mmi_fmrdo_search_frequency_stop();

        if(fmrdo_key_press_down_first_flag)
        {
                if( !g_fmrdo.is_long_press_number_key )
                {
                // stop previous twinkle frequency if necessary
                mmi_fmrdo_stop_twinkle_frequency( FALSE );
                	
                GetkeyInfo( (U16*)&key_code, (U16*)&key_type );
                key_code=fmrdo_key_code_first*10+key_code;
                TRACE_EF(g_sw_FMR,"MMI FMR: key_code=%d,fmrdo_key_code_first=%d",key_code,fmrdo_key_code_first);
                freq = g_fmrdo.channel_list.freq[key_code - KEY_1];
                if( freq != 0 )
                	mmi_fmrdo_goto_channel( (U16)(key_code - KEY_1) );
                }
                g_fmrdo.is_long_press_number_key = FALSE;
                fmrdo_key_press_down_first_flag=FALSE;
                fmrdo_key_code_first=0;
        }
        else
        {
                if( !g_fmrdo.is_long_press_number_key )
                {
                	// stop previous twinkle frequency if necessary
                	mmi_fmrdo_stop_twinkle_frequency( FALSE );
                		
                	GetkeyInfo( (U16*)&key_code, (U16*)&key_type );
                	freq = g_fmrdo.channel_list.freq[key_code - KEY_1];
                	if( freq != 0 )
                		mmi_fmrdo_goto_channel( (U16)(key_code - KEY_1) );
                }
                g_fmrdo.is_long_press_number_key = FALSE;
        }
}
/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_wait_number_key_hdlr
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
void mmi_fmrdo_wait_number_key_hdlr( void )
{
    if(fmrdo_key_press_down_first_flag)
    {
        fmrdo_key_press_down_first_flag=FALSE;
        mmi_fmrdo_release_number_key();
    }
}
/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_release_0_key
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
void mmi_fmrdo_release_0_key( void )
{
    if(fmrdo_key_press_down_first_flag)
    {
        mmi_fmrdo_release_number_key();
    }
}
/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_release_number_key_extend
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
void mmi_fmrdo_release_number_key_extend( void )
{
        U16 key_code, key_type;

        GetkeyInfo( (U16*)&key_code, (U16*)&key_type );
        if((!fmrdo_key_press_down_first_flag)&&((KEY_1==key_code)||(key_code==KEY_2)))
        {
            fmrdo_key_code_first=key_code;
            fmrdo_key_press_down_first_flag=TRUE;
            coolsand_UI_start_timer(1000, mmi_fmrdo_wait_number_key_hdlr);  
        }
        else
        {
            mmi_fmrdo_release_number_key();
        }
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_twinkle_frequency_timer
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
void mmi_fmrdo_twinkle_frequency_timer( void )
{
	if( g_fmrdo.twinkle_frequency_count % 2 == 0 ) // twinkle off
	{
		mmi_fmrdo_redraw_main_frequency( g_fmrdo.frequency, FALSE );
		if( g_fmrdo.twinkle_frequency_count < TWINKLE_FREQUENCY_COUNT )
		{
			g_fmrdo.twinkle_frequency_count++;
			StartTimer( FMRDO_TWINKLE_FREQUENCY_TIMER, TWINKLE_FREQUENCY_OFF_TIME, mmi_fmrdo_twinkle_frequency_timer );
		}
	}
	else // twinkle on
	{
		mmi_fmrdo_redraw_main_frequency( g_fmrdo.frequency, TRUE );
		if( g_fmrdo.twinkle_frequency_count < TWINKLE_FREQUENCY_COUNT )
		{
			g_fmrdo.twinkle_frequency_count++;
			StartTimer( FMRDO_TWINKLE_FREQUENCY_TIMER, TWINKLE_FREQUENCY_ON_TIME, mmi_fmrdo_twinkle_frequency_timer );
		}
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_step_down_timer
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
void mmi_fmrdo_step_down_timer( void )
{
	if( g_fmrdo.step_freq > MIN_FM_FREQUENCY )
		g_fmrdo.step_freq--;
	else
		g_fmrdo.step_freq = MAX_FM_FREQUENCY;

	// update display
	if( g_fmrdo.in_main_screen )
	{
		mmi_fmrdo_redraw_main_frequency( g_fmrdo.step_freq, TRUE );
		mmi_fmrdo_redraw_main_channel_tuner( g_fmrdo.step_freq );
	}

	if( g_fmrdo.normal_repeat_step_count == 0 && g_fmrdo.is_power_on )
		mdi_fmr_mute( 1 ); // mute in the begining of step down
	
	if( g_fmrdo.normal_repeat_step_count < NORMAL_REAPEAT_STEP_COUNT )
	{
		g_fmrdo.normal_repeat_step_count++;
		StartTimer( FMRDO_REPEAT_STEP_TIMER, NORMAL_REAPEAT_STEP_PERIOD, mmi_fmrdo_step_down_timer );
	}
	else
		StartTimer( FMRDO_REPEAT_STEP_TIMER, FAST_REAPEAT_STEP_PERIOD, mmi_fmrdo_step_down_timer );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_step_up_timer
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
void mmi_fmrdo_step_up_timer( void )
{
	if( g_fmrdo.step_freq < MAX_FM_FREQUENCY )
		g_fmrdo.step_freq++;
	else
		g_fmrdo.step_freq = MIN_FM_FREQUENCY;

	// update display
	if( g_fmrdo.in_main_screen )
	{
		mmi_fmrdo_redraw_main_frequency( g_fmrdo.step_freq, TRUE );
		mmi_fmrdo_redraw_main_channel_tuner( g_fmrdo.step_freq );
	}

	if( g_fmrdo.normal_repeat_step_count == 0 && g_fmrdo.is_power_on )
		mdi_fmr_mute( 1 ); // mute in the begining of step up
	
	if( g_fmrdo.normal_repeat_step_count < NORMAL_REAPEAT_STEP_COUNT )
	{
		g_fmrdo.normal_repeat_step_count++;
		StartTimer( FMRDO_REPEAT_STEP_TIMER, NORMAL_REAPEAT_STEP_PERIOD, mmi_fmrdo_step_up_timer );
	}
	else
		StartTimer( FMRDO_REPEAT_STEP_TIMER, FAST_REAPEAT_STEP_PERIOD, mmi_fmrdo_step_up_timer );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_is_stepping_up
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_fmrdo_is_stepping_up( void )
{
	return (g_fmrdo.is_ready && g_fmrdo.selected_button == FMRDO_BUTTON_STEP_UP && g_fmrdo.is_button_down);
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_is_stepping_down
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_fmrdo_is_stepping_down( void )
{
	return (g_fmrdo.is_ready && g_fmrdo.selected_button == FMRDO_BUTTON_STEP_DOWN && g_fmrdo.is_button_down);
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_get_freq_from_str
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     UI_string_type input_str
* RETURNS
*     U16
* GLOBALS AFFECTED
*     
*****************************************************************************/
U16 mmi_fmrdo_get_freq_from_str( UI_string_type input_str )
{
	U16 freq = 0;
	BOOL fraction = FALSE;
	BOOL skip = FALSE;
	BOOL one_digit_dicimal = TRUE;
	S16 i;
	
	for( i = 0; input_str[i] != 0; i++ )
	{
		if( input_str[i] == (UI_character_type)_T_MMI'.' )
		{
			if(	(input_str[i+1] != 0) && (input_str[i+2] != 0))
			{
				one_digit_dicimal = FALSE;	
			}
			fraction = TRUE;
		}
		else if( !skip )
		{
			freq = freq * 10 + input_str[i] - (UI_character_type)_T_MMI'0';
			if( fraction )
				skip = TRUE;
		}
	}
	
	if( freq != 0 && !skip )
	{
		freq *= 10;
	}

	if(!one_digit_dicimal)
	{
		freq = 	0xffff; /* Only allow one digit after the '.' */
	}

	return freq;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_set_channel
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S16 channel_index
*     U16 freq
*     UI_string_type name
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_set_channel( S16 channel_index, U16 freq, UI_string_type name )
{
	if( (g_fmrdo.channel_index == channel_index) && (g_fmrdo.channel_list.freq[g_fmrdo.channel_index] != freq) )
	{
		g_fmrdo.channel_index = -1;
	}

	if( (g_fmrdo.channel_index == -1) && (freq == g_fmrdo.frequency) )
	{
		g_fmrdo.channel_index = channel_index;
	}
	pfnUnicodeStrcpy( (S8*)g_fmrdo.channel_list.name[channel_index], (S8*)name );

	g_fmrdo.channel_list.freq[channel_index] = freq;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_get_volume
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
U8 mmi_fmrdo_get_volume( void )
{
	S16	error;
	
	if( !g_fmrdo.is_ready )
		ReadValue( NVRAM_FMRDO_VOLUME, &g_fmrdo.volume, DS_BYTE, &error );
	
	return g_fmrdo.volume;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_set_volume
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     U8 volume
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_fmrdo_set_volume( U8 volume )
{
	S16 error;
	
	g_fmrdo.volume = volume;
	WriteValue( NVRAM_FMRDO_VOLUME, &g_fmrdo.volume, DS_BYTE, &error );
	
	if( g_fmrdo.is_ready && g_fmrdo.state != FMRDO_STATE_SEARCH_ONE && g_fmrdo.state != FMRDO_STATE_SEARCH_ALL )
		mdi_audio_set_volume( AUD_VOLUME_FMR, volume );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_write_settings_to_nvram
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
void mmi_fmrdo_write_settings_to_nvram( void )
{
	S16 error;
	
	WriteValue( NVRAM_FMRDO_BACKGROUND_PLAY, &g_fmrdo.is_background_play, DS_BYTE, &error );
	WriteValue( NVRAM_FMRDO_AUDIO_OUT, &g_fmrdo.is_audio_out, DS_BYTE, &error ); //yanghong 20060421 add
	WriteValue( NVRAM_FMRDO_SKIN, &g_fmrdo.skin_index, DS_BYTE, &error );

#if defined(__MMI_FM_RADIO_RECORD__)
	WriteValue( NVRAM_FMRDO_RECORD_FORMAT, &g_fmrdo.rec.format, DS_BYTE, &error );
	WriteValue( NVRAM_FMRDO_RECORD_STORAGE, &g_fmrdo.rec.storage, DS_BYTE, &error );	
#endif	
	
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_inc_volume
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
void mmi_fmrdo_inc_volume( void )
{
	FMR_TRACE();
	
	if( g_fmrdo.volume < MAX_VOL_LEVEL )
	{
		mmi_fmrdo_set_volume( ++g_fmrdo.volume );
		if( g_fmrdo.in_main_screen )
			mmi_fmrdo_redraw_main_volume();
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_dec_volume
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
void mmi_fmrdo_dec_volume( void )
{
	FMR_TRACE();	
	if( g_fmrdo.volume > 0 )
	{
		mmi_fmrdo_set_volume( --g_fmrdo.volume );
		if( g_fmrdo.in_main_screen )
			mmi_fmrdo_redraw_main_volume();
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_init_app
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
void mmi_fmrdo_init_app( void )
{
#ifdef __MMI_FM_RADIO_CHECK_CHIP_OR_HIDE__
	if( FMR_IsChipValid() )
#endif
		SetHiliteHandler( MENU_ID_FMRDO_MAIN, mmi_fmrdo_highlight_menu );
	g_fmrdo.is_subLCD_shown = FALSE;

#if defined(__MMI_FM_RADIO_RECORD__)
	SetHiliteHandler( MENU_ID_FMRDO_EDIT_FILENAME_SAVE, mmi_fmrdo_highlight_record_save_file );
	SetHiliteHandler( MENU_ID_FMRDO_EDIT_FILENAME_INPUT_METHOD, mmi_fmrdo_highlight_record_input_method );	
#endif
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_run_app
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
#ifdef __MMI_SUPPORT_JME__
extern BOOL checkVMOn(void);
#endif
extern VOID mmi_display_gprs_active(void) ;
extern pBOOL mmi_bootup_get_active_flight_mode(void);
#if defined(CAM_MD_SUPPORT)

extern void csd_stop_sw_motionsensor(void);
#endif
void mmi_fmrdo_run_app( void )
{
        FMR_TRACE();
//   isEarphonePlugged=TRUE; 
 //     isEarphonePlugged=isEarphonePlugged_1; 
#ifdef __FLIGHT_MODE_SUPPORT__
    if (mmi_bootup_get_active_flight_mode() != 0)
    {
        /* prevent from entry BT main menu from shortcut */
        DisplayPopup(
            (U8*) GetString(STR_MENU_FLIGHT_MODE),
            IMG_GLOBAL_ERROR,
            0,
            UI_POPUP_NOTIFYDURATION_TIME,
            (U8) WARNING_TONE);
        return;
    }
#endif /* __FLIGHT_MODE_SUPPORT__ */
	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(mmi_fmrdo_run_app);
		return;
	}
#if 0//defined(__PROJECT_GALLITE_C01__)
    if (MTPNP_AD_Get_Status() == MTPNP_AD_FLIGHT)
    {
        DisplayPopup( (PU8)GetString( STRING_MTPNP_DM_FLIGHT ), IMG_GLOBAL_WARNING, 1, 2000, (U8)WARNING_TONE );
        return;
    }
#endif
		
#if defined(CAM_MD_SUPPORT)
        csd_stop_sw_motionsensor();
#endif
#ifdef __MMI_MAINLCD_220X176__

mdi_audio_suspend_background_play();
#else

#ifdef __MMI_AUDIO_PLAYER__
	mmi_audply_stop_playing();
#endif // __MMI_AUDIO_PLAYER__
#endif

	if( !g_fmrdo.is_ready )
	{
#ifdef MMI_ON_HARDWARE_P
		S16	error;

		// read channel list from nvram
		ReadRecord( NVRAM_EF_FM_RADIO_LID, 1, (void*)&g_fmrdo.channel_list, NVRAM_EF_FM_RADIO_SIZE, &error );
		
		// read settings from nvram
		ReadValue( NVRAM_FMRDO_BACKGROUND_PLAY, &g_fmrdo.is_background_play, DS_BYTE, &error );
/*yanghong 20060421 modify begin */
		{
			U8 data;
			ReadValue( NVRAM_FMRDO_AUDIO_OUT, &data, DS_BYTE, &error ); 
			if(data == 0xFF)
			{
#ifdef __MMI_FM_RADIO_BIND_EARPHONE__
				{
					g_fmrdo.is_audio_out = 0;
				}
#else
				if( isEarphonePlugged_1 )
				{
					g_fmrdo.is_audio_out = 0;
				}		
				else
				{
					g_fmrdo.is_audio_out = 1;
				}
#endif
				WriteValue( NVRAM_FMRDO_AUDIO_OUT, &g_fmrdo.is_audio_out, DS_BYTE, &error ); //yanghong 20060421 add
			}
			else
				g_fmrdo.is_audio_out = data;
		}
/*yanghong modify end*/
		ReadValue( NVRAM_FMRDO_SKIN, &g_fmrdo.skin_index, DS_BYTE, &error );
		ReadValue( NVRAM_FMRDO_VOLUME, &g_fmrdo.volume, DS_BYTE, &error );
		//g_fmrdo.volume = 6;
		//Added by liuxl for fixing bug-13554 on 20090730-Start
		// read frequency from nvram
		{
			U16 frequency;
			ReadValue( NVRAM_FMRDO_FREQUENCY, &frequency, DS_SHORT,&error ); 

			if((frequency < MIN_FM_FREQUENCY) ||((frequency > MAX_FM_FREQUENCY)))
			{
				g_fmrdo.frequency = DEFAULT_FREQUENCY;
				WriteValue( NVRAM_FMRDO_FREQUENCY, &g_fmrdo.frequency, DS_SHORT, &error ); //yanghong 20060421 add
			}
			else
				g_fmrdo.frequency = frequency;
		}
		//Added by liuxl for fixing bug-13554 on 20090730-End


#if defined(__MMI_FM_RADIO_RECORD__)
		ReadValue( NVRAM_FMRDO_RECORD_FORMAT, &g_fmrdo.rec.format, DS_BYTE, &error );
		ReadValue( NVRAM_FMRDO_RECORD_STORAGE, &g_fmrdo.rec.storage, DS_BYTE, &error );
		if(g_fmrdo.rec.format >= FM_REC_FORMAT_NUM) g_fmrdo.rec.format = 0;
		if(g_fmrdo.rec.storage >= FM_REC_STORAGE_NUM) g_fmrdo.rec.storage = 0;
#endif	

#if 0 //shizh,ÁÙÊ±·½·¨
                //g_fmrdo.channel_list.name={"FM88.5","FM90.0","FM91.0",
                //                                               "FM91.5","FM96.6","FM97.4",
                //                                               "FM102.0","FM104.0","FM106.6"};

                g_fmrdo.channel_list.freq[0]=885; 
                g_fmrdo.channel_list.freq[1]=900;                
                g_fmrdo.channel_list.freq[2]=910;
                g_fmrdo.channel_list.freq[3]=915;
                g_fmrdo.channel_list.freq[4]=966;
                g_fmrdo.channel_list.freq[5]=974;
                g_fmrdo.channel_list.freq[6]=1020;
                g_fmrdo.channel_list.freq[7]=1040;                
                g_fmrdo.channel_list.freq[8]=1066;                
#endif                
#else
		memset( &g_fmrdo.channel_list, 0, NVRAM_EF_FM_RADIO_SIZE );

                
	//	g_fmrdo.is_background_play = 1;
		g_fmrdo.skin_index = 0;
		g_fmrdo.volume = 4;
#if defined(__MMI_FM_RADIO_RECORD__)
		g_fmrdo.rec.format = 0;
		g_fmrdo.rec.storage = 0;
#endif			
#endif // MMI_ON_HARDWARE_P

		// reset channel index
		g_fmrdo.channel_index = -1;
		
		// default frequency value
		//g_fmrdo.frequency = DEFAULT_FREQUENCY;//commented by liuxl for fixing bug-13554 on 20090730
		
		// search & power states
		g_fmrdo.is_search_on = FALSE;	//yanghong 20060408 modify, FALSE;//yuzl 20080226 modify TURE
		g_fmrdo.is_power_on = FALSE;

		mdi_audio_set_background_handler( MDI_BACKGROUND_APP_FMR, (mdi_bg_callback) mmi_fmrdo_continued );

		g_fmrdo.is_ready = TRUE;
	}
	// power on
#ifdef __MMI_FM_RADIO_BIND_EARPHONE__
	if( isEarphonePlugged_1 )
#endif
	{/*If it plug in earphone, let it go main. Or popup "plug in earphone" to notice user.*/
		mmi_fmrdo_power_on( TRUE );
		mdi_audio_set_background_handler( MDI_BACKGROUND_APP_FMR, (mdi_bg_callback) mmi_fmrdo_continued );		
	}
		
		//mmi_fmrdo_set_volume(g_fmrdo.volume);
		mmi_fmrdo_entry_main();

		
#ifdef __MMI_FM_RADIO_BIND_EARPHONE__
	if(! isEarphonePlugged_1 )
		DisplayPopup( (PU8)GetString( STR_ID_FMRDO_PLEASE_PLUG_IN_EARPHONE ), IMG_GLOBAL_WARNING, 1, 2000, (U8)WARNING_TONE );
#endif

}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_highlight_menu
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
void mmi_fmrdo_highlight_menu( void )
{
        FMR_TRACE();
	ChangeLeftSoftkey( STR_GLOBAL_OK, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, 0 );

	SetLeftSoftkeyFunction( mmi_fmrdo_run_app, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
#if !defined __MMI_MAINLCD_220X176__ && !defined(__MMI_BLACK_BERRY_MAIN_MENU__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	SetKeyHandler( mmi_fmrdo_run_app, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
#endif	
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_match_channel_list
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     U16 frequency
* RETURNS
*     S16
* GLOBALS AFFECTED
*     
*****************************************************************************/
S16 mmi_fmrdo_match_channel_list( U16 frequency )
{
	S16 i;

	// try matching current frequency first	
	if( g_fmrdo.channel_index >= 0 && g_fmrdo.channel_list.freq[g_fmrdo.channel_index] == frequency )
		return g_fmrdo.channel_index;

	for( i = 0; i < FM_RADIO_CHANNEL_NUM; i++ )
		if( g_fmrdo.channel_list.freq[i] == frequency )
			return i;
	
	return -1;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_press_step_down_button
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
void mmi_fmrdo_press_step_down_button( void )
{
        FMR_TRACE();

	if( g_fmrdo.is_button_down )
		return;

	g_fmrdo.selected_button = FMRDO_BUTTON_STEP_DOWN;
	g_fmrdo.is_button_down = TRUE;
	g_fmrdo.normal_repeat_step_count = 0;
	
	mmi_fmrdo_redraw_main_push_button( g_fmrdo.selected_button, g_fmrdo.is_button_down );

	mmi_fmrdo_search_frequency_stop();
	
	g_fmrdo.step_freq = g_fmrdo.frequency;
	StartTimer( FMRDO_REPEAT_STEP_TIMER, NORMAL_REAPEAT_STEP_PERIOD, mmi_fmrdo_step_down_timer );	
	
	// stop previous twinkle frequency if necessary
	mmi_fmrdo_stop_twinkle_frequency( TRUE );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_release_step_down_button
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
void mmi_fmrdo_release_step_down_button( void )
{

	U16 freq;
	BOOL is_freq_changed = (g_fmrdo.normal_repeat_step_count != 0);
        FMR_TRACE();

	if( g_fmrdo.is_button_down && g_fmrdo.selected_button == FMRDO_BUTTON_STEP_DOWN )
	{
		StopTimer( FMRDO_REPEAT_STEP_TIMER );
		
		g_fmrdo.is_button_down = FALSE;
		
		mmi_fmrdo_redraw_main_push_button( g_fmrdo.selected_button, g_fmrdo.is_button_down );

		if( is_freq_changed )
			freq = g_fmrdo.step_freq; // frequency is changed so we don't need to decrease it again
		else
		{
			// frequency is not changed during button press down so we need to decrease it
			if( g_fmrdo.frequency > MIN_FM_FREQUENCY )
				freq = g_fmrdo.frequency - 1;
			else
				freq = MAX_FM_FREQUENCY;
		}
		
		if( g_fmrdo.is_power_on && g_fmrdo.is_search_on )
		{
			mmi_fmrdo_redraw_main_channel_name( (UI_string_type) GetString( STR_ID_FMRDO_SEARCHING ) ); // show searching text
			mmi_fmrdo_search_frequency_start( freq, FALSE ); // do search in non-blocking mode
		}
		else
			mmi_fmrdo_set_frequency( freq );
		
		g_fmrdo.normal_repeat_step_count = 0;
		g_fmrdo.selected_button = FMRDO_BUTTON_NONE;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_press_step_up_button
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
void mmi_fmrdo_press_step_up_button( void )
{
        //FMR_TRACE();

	if( g_fmrdo.is_button_down )
		return;

	g_fmrdo.selected_button = FMRDO_BUTTON_STEP_UP;
	g_fmrdo.is_button_down = TRUE;
	g_fmrdo.normal_repeat_step_count = 0;
	
	mmi_fmrdo_redraw_main_push_button( g_fmrdo.selected_button, g_fmrdo.is_button_down );

	mmi_fmrdo_search_frequency_stop();
	
	g_fmrdo.step_freq = g_fmrdo.frequency;
	StartTimer( FMRDO_REPEAT_STEP_TIMER, NORMAL_REAPEAT_STEP_PERIOD, mmi_fmrdo_step_up_timer );
	
	// stop previous twinkle frequency if necessary
	mmi_fmrdo_stop_twinkle_frequency( TRUE );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_release_step_up_button
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
void mmi_fmrdo_release_step_up_button( void )
{

	U16 freq;
	BOOL is_freq_changed = (g_fmrdo.normal_repeat_step_count != 0);
        FMR_TRACE();
	
	if( g_fmrdo.is_button_down && g_fmrdo.selected_button == FMRDO_BUTTON_STEP_UP )
	{
		StopTimer( FMRDO_REPEAT_STEP_TIMER );

		g_fmrdo.is_button_down = FALSE;
		
		mmi_fmrdo_redraw_main_push_button( g_fmrdo.selected_button, g_fmrdo.is_button_down );

		if( is_freq_changed )
			freq = g_fmrdo.step_freq; // frequency is changed so we don't need to increase it again
		else
		{
			// frequency is not changed during button press down so we need to increase it
			if( g_fmrdo.frequency < MAX_FM_FREQUENCY )
				freq = g_fmrdo.frequency + 1;
			else
				freq = MIN_FM_FREQUENCY;
		}

		if( g_fmrdo.is_power_on && g_fmrdo.is_search_on )
		{
			mmi_fmrdo_redraw_main_channel_name( (UI_string_type) GetString( STR_ID_FMRDO_SEARCHING ) ); // show searching text
			mmi_fmrdo_search_frequency_start( freq, TRUE ); // do search in non-blocking mode
		}
		else
			mmi_fmrdo_set_frequency( freq );

		g_fmrdo.normal_repeat_step_count = 0;
		g_fmrdo.selected_button = FMRDO_BUTTON_NONE;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_press_search_button
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
void mmi_fmrdo_press_search_button( void )
{
        FMR_TRACE();

	if( g_fmrdo.is_button_down )
		return;

	g_fmrdo.selected_button = FMRDO_BUTTON_SEARCH;
	g_fmrdo.is_button_down = TRUE;
	
	mmi_fmrdo_redraw_main_toggle_button( g_fmrdo.selected_button, g_fmrdo.is_button_down, g_fmrdo.is_search_on ,TRUE);
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_press_power_button
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
void mmi_fmrdo_release_search_button( void )
{
        FMR_TRACE();

	if( g_fmrdo.is_button_down && g_fmrdo.selected_button == FMRDO_BUTTON_SEARCH )
	{
		g_fmrdo.is_button_down = FALSE;
		g_fmrdo.is_search_on = !g_fmrdo.is_search_on;
		
		mmi_fmrdo_redraw_main_toggle_button( g_fmrdo.selected_button, g_fmrdo.is_button_down, g_fmrdo.is_search_on ,TRUE);

		// stop if it's searching
		mmi_fmrdo_search_frequency_stop();

		g_fmrdo.selected_button = FMRDO_BUTTON_NONE;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_press_power_button
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
void mmi_fmrdo_press_power_button( void )
{
        FMR_TRACE();

	if( g_fmrdo.is_button_down )
		return;

#ifdef __MMI_FM_RADIO_BIND_EARPHONE__
	if( !isEarphonePlugged_1 )
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMRDO_PLEASE_PLUG_IN_EARPHONE ), IMG_GLOBAL_WARNING, 1, 2000, (U8)WARNING_TONE );
		return;
	}
#endif
	g_fmrdo.selected_button = FMRDO_BUTTON_POWER;
	g_fmrdo.is_button_down = TRUE;
	
	mmi_fmrdo_redraw_main_toggle_button( g_fmrdo.selected_button, g_fmrdo.is_button_down, g_fmrdo.is_power_on ,TRUE);
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_release_power_button
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
void mmi_fmrdo_release_power_button( void )
{
        FMR_TRACE();
                        
	if( g_fmrdo.is_button_down && (g_fmrdo.selected_button == FMRDO_BUTTON_POWER) )
	{
		g_fmrdo.is_button_down = FALSE;
	        g_fmrdo.is_power_on = !g_fmrdo.is_power_on;        //shizh 
			
		// stop if it's searching
		mmi_fmrdo_search_frequency_stop();

		// power off if it's on
		//mmi_fmrdo_power_on( (BOOL)!g_fmrdo.is_power_on );
		mmi_fmrdo_power_on( g_fmrdo.is_power_on );

		mmi_fmrdo_redraw_main_toggle_button( g_fmrdo.selected_button, g_fmrdo.is_button_down, g_fmrdo.is_power_on ,TRUE);

		g_fmrdo.selected_button = FMRDO_BUTTON_NONE;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_press_setting_button
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
void mmi_fmrdo_press_setting_button( void )
{
	if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
	{
		mmi_fmrdo_redraw_main_push_down_button( FMRDO_BUTTON_SETTINGS, TRUE );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_release_setting_button
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
void mmi_fmrdo_release_setting_button( void )
{
	if(g_fmrdo.state != FMRDO_STATE_RECORD && g_fmrdo.state != FMRDO_STATE_RECORD_PAUSE)
	{
		mmi_fmrdo_redraw_main_push_down_button( FMRDO_BUTTON_SETTINGS, FALSE );
		mmi_fmrdo_entry_settings();
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_root
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void *param_p
* RETURNS
*     U8
* GLOBALS AFFECTED
*     
*****************************************************************************/
U8 mmi_fmrdo_exit_root( void *param_p )
{
	if( !g_fmrdo.is_background_play )
		mmi_fmrdo_power_on( FALSE );
#ifdef __MMI_MAINLCD_220X176__

	mdi_audio_resume_background_play();
#endif

	return FALSE;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_main
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
extern void mmi_fmrdo_pen_down_hdlr(mmi_pen_point_struct pos);
extern void mmi_fmrdo_pen_up_hdlr(mmi_pen_point_struct pos);
extern void mmi_fmrdo_pen_move_hdlr(mmi_pen_point_struct pos);
#endif
void mmi_fmrdo_main_register_keys(void)
{
	U16 number_keys[] = {KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9 };
	const mmi_fmrdo_skin_struct* skin_p;
	
	void (*button_press_hdlr[])(void) = {
		MMI_dummy_function, // FMRDO_BUTTON_NONE
		mmi_fmrdo_press_power_button, // FMRDO_BUTTON_POWER
		mmi_fmrdo_press_search_button, // FMRDO_BUTTON_SEARCH
		mmi_fmrdo_press_step_up_button, // FMRDO_BUTTON_STEP_UP
		mmi_fmrdo_press_step_down_button, // FMRDO_BUTTON_STEP_DOWN
		MMI_dummy_function, // FMRDO_BUTTON_NONE
		MMI_dummy_function, // FMRDO_BUTTON_NONE
		mmi_fmrdo_press_setting_button // FMRDO_BUTTON_SETTING
	};
	void (*button_release_hdlr[])(void) = {
		MMI_dummy_function, // FMRDO_BUTTON_NONE
		mmi_fmrdo_release_power_button, // FMRDO_BUTTON_POWER
		mmi_fmrdo_release_search_button, // FMRDO_BUTTON_SEARCH
		mmi_fmrdo_release_step_up_button, // FMRDO_BUTTON_STEP_UP
		mmi_fmrdo_release_step_down_button, // FMRDO_BUTTON_STEP_DOWN
		MMI_dummy_function, // FMRDO_BUTTON_NONE
		MMI_dummy_function, // FMRDO_BUTTON_NONE
		mmi_fmrdo_release_setting_button // FMRDO_BUTTON_SETTING
	};

        FMR_TRACE();
	FMR_TRACE_ARG("g_fmrdo_skins[ %d ] ", g_fmrdo.skin_index);
	
	// associate button press, release handlers with skin settings and register arrow key handlers
	
	skin_p = &g_fmrdo_skins[ g_fmrdo.skin_index ];


         FMR_TRACE_ARG("g_fmrdo_skins[0].up_key_mapping = %d", g_fmrdo_skins[0].up_key_mapping);
         FMR_TRACE_ARG("g_fmrdo_skins[0].down_key_mapping = %d", g_fmrdo_skins[0].down_key_mapping);
         FMR_TRACE_ARG("g_fmrdo_skins[0].left_key_mapping = %d", g_fmrdo_skins[0].left_key_mapping);
         FMR_TRACE_ARG("g_fmrdo_skins[0].right_key_mapping = %d", g_fmrdo_skins[0].right_key_mapping);

        
         /*  g_fmrdo_skins is a const.
             g_fmrdo_skins[0].up_key_mapping = 1;
             g_fmrdo_skins[0].down_key_mapping = 2;
             g_fmrdo_skins[0].left_key_mapping = 3;
             g_fmrdo_skins[0].right_key_mapping = 4;
         */

#if FMR_DEBUG
	SetKeyHandler( mmi_fmrdo_release_search_button, KEY_UP_ARROW, KEY_EVENT_UP );
	SetKeyHandler( mmi_fmrdo_release_power_button, KEY_DOWN_ARROW, KEY_EVENT_UP );
	SetKeyHandler( mmi_fmrdo_release_step_down_button, KEY_LEFT_ARROW, KEY_EVENT_UP );
	SetKeyHandler( mmi_fmrdo_release_step_up_button, KEY_RIGHT_ARROW, KEY_EVENT_UP );
	
	SetKeyHandler( mmi_fmrdo_press_search_button, KEY_UP_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_press_power_button, KEY_DOWN_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_press_step_down_button, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_press_step_up_button, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
#else
	SetKeyHandler( button_release_hdlr[ skin_p->up_key_mapping ], KEY_UP_ARROW, KEY_EVENT_UP );// 1
	SetKeyHandler( button_release_hdlr[ skin_p->down_key_mapping ], KEY_DOWN_ARROW, KEY_EVENT_UP ); // 2
	SetKeyHandler( button_release_hdlr[ skin_p->left_key_mapping ], KEY_LEFT_ARROW, KEY_EVENT_UP ); // 3
	SetKeyHandler( button_release_hdlr[ skin_p->right_key_mapping ], KEY_RIGHT_ARROW, KEY_EVENT_UP ); // 4
	SetKeyHandler( button_release_hdlr[ skin_p->ok_key_mapping ], KEY_ENTER, KEY_EVENT_UP ); // ok key
	SetKeyHandler( button_press_hdlr[ skin_p->up_key_mapping ], KEY_UP_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( button_press_hdlr[ skin_p->down_key_mapping ], KEY_DOWN_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( button_press_hdlr[ skin_p->left_key_mapping ], KEY_LEFT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( button_press_hdlr[ skin_p->right_key_mapping ], KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( button_press_hdlr[ skin_p->ok_key_mapping ], KEY_ENTER, KEY_EVENT_DOWN );
#endif

	SetKeyHandler( MMI_dummy_function, KEY_UP_ARROW, KEY_EVENT_REPEAT );
	SetKeyHandler( MMI_dummy_function, KEY_RIGHT_ARROW, KEY_EVENT_REPEAT );
	SetKeyHandler( MMI_dummy_function, KEY_LEFT_ARROW, KEY_EVENT_REPEAT );
	SetKeyHandler( MMI_dummy_function, KEY_DOWN_ARROW, KEY_EVENT_REPEAT );
	
	// register number key handlers
	SetGroupKeyHandler( mmi_fmrdo_release_number_key_extend, number_keys, 9, KEY_EVENT_UP );
	SetGroupKeyHandler( mmi_fmrdo_long_press_number_key, number_keys, 9, KEY_LONG_PRESS );
	SetKeyHandler( mmi_fmrdo_release_0_key, KEY_0, KEY_EVENT_UP );

	// register LSK, RSK hanlders
	SetKeyHandler( mmi_fmrdo_entry_main_option, KEY_LSK, KEY_EVENT_UP );
	SetKeyHandler( GoBackHistory, KEY_RSK, KEY_EVENT_UP );

	#if defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__) /*Disable key *,# to change volume for fixing bug 13011 by liulxl on 20090703 */
	{
		// register volume inc, dec handlers  //yuzl change volume key to  *,#
		SetKeyHandler( mmi_fmrdo_inc_volume, KEY_POUND, KEY_EVENT_DOWN );
		SetKeyHandler( mmi_fmrdo_dec_volume, KEY_STAR, KEY_EVENT_DOWN );
	}
	#else
	{
		SetKeyHandler( mmi_fmrdo_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
		SetKeyHandler( mmi_fmrdo_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
	}
	#endif

 
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_register_down_handler(mmi_fmrdo_pen_down_hdlr);
	mmi_pen_register_up_handler(mmi_fmrdo_pen_up_hdlr);
	mmi_pen_register_move_handler(mmi_fmrdo_pen_move_hdlr);
#endif
 

	//register FM record key handler
#if defined(__MMI_FM_RADIO_RECORD__)
	SetKeyHandler( mmi_fmrdo_release_record_button, KEY_ENTER, KEY_EVENT_UP );
	SetKeyHandler( mmi_fmrdo_release_power_button, KEY_DOWN_ARROW, KEY_EVENT_UP ); 
	SetKeyHandler( mmi_fmrdo_press_power_button, KEY_DOWN_ARROW, KEY_EVENT_DOWN); 
#endif
	
}

void mmi_fmrdo_entry_main( void )
{
        FMR_TRACE();

        if(HandleLowBatteryProcess())
        {
            mmi_trace(1,"low battery in fm");
            return ;
        }

        
        mmi_fmrdo_printf_resid();  //shizh:trace func for print resId
        
	EntryNewScreen( SCR_ID_FMRDO_MAIN, mmi_fmrdo_exit_main, mmi_fmrdo_entry_main, NULL );

	entry_full_screen();

	g_fmrdo.in_main_screen = TRUE;

	// button state
	g_fmrdo.selected_button = FMRDO_BUTTON_NONE;
	g_fmrdo.is_button_down = FALSE;
	g_fmrdo.normal_repeat_step_count = 0;
	g_fmrdo.is_long_press_number_key = FALSE;
	g_fmrdo.twinkle_frequency_count = 0;
	
	// draw main screen
	mmi_fmrdo_show_main( STR_GLOBAL_OPTIONS, STR_GLOBAL_BACK, g_fmrdo.skin_index );

	//Register all key handler
	mmi_fmrdo_main_register_keys();

	SetDelScrnIDCallbackHandler( SCR_ID_FMRDO_MAIN, (HistoryDelCBPtr)mmi_fmrdo_exit_root );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_main
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
void mmi_fmrdo_exit_main( void )
{
	FMR_TRACE();
	g_fmrdo.in_main_screen = FALSE;

	// stop timer and update frequency if we don't release step up/down button yet
	StopTimer( FMRDO_REPEAT_STEP_TIMER );
	StopTimer( FMRDO_TWINKLE_FREQUENCY_TIMER );

	FMR_TRACE_ARG(" g_fmrdo.normal_repeat_step_count = %d.",  g_fmrdo.normal_repeat_step_count );
	FMR_TRACE_ARG(" g_fmrdo.step_freq = %d.",  g_fmrdo.step_freq );

	
	if( g_fmrdo.normal_repeat_step_count != 0 )
		mmi_fmrdo_set_frequency( g_fmrdo.step_freq );

	// stop previous twinkle frequency if necessary
	mmi_fmrdo_stop_twinkle_frequency( FALSE );

	// stop if it's searching
	mmi_fmrdo_search_frequency_stop();

#if defined(__MMI_FM_RADIO_RECORD__)
	/*Abort due to exit screen*/
	g_fmrdo.rec.user_abort = FALSE;
	mmi_fmrdo_stop_record_on_exit();
	mmi_fmrdo_util_stop_duration();
#endif	
	
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_main_option
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
void mmi_fmrdo_entry_main_option( void )
{

	U16 item_texts[10];
	U16 item_icons[10];
	U16 item_count, i;
	U8* gui_buffer;
	U8* item_popups[10];
        FMR_TRACE();
	
	EntryNewScreen( SCR_ID_FMRDO_MAIN_OPTION, mmi_fmrdo_exit_main_option, mmi_fmrdo_entry_main_option, NULL );
	
	gui_buffer = GetCurrGuiBuffer( SCR_ID_FMRDO_MAIN_OPTION );
	item_count = GetNumOfChild( MENU_ID_FMRDO_MAIN_OPTION );
	GetSequenceStringIds( MENU_ID_FMRDO_MAIN_OPTION, item_texts );
	GetSequenceImageIds( MENU_ID_FMRDO_MAIN_OPTION, item_icons );
	SetParentHandler( MENU_ID_FMRDO_MAIN_OPTION );
	RegisterHighlightHandler( mmi_fmrdo_main_option_highlight_hdlr );

	FMR_TRACE_ARG("[#FMRadio:]mmi_fmrdo_entry_main_option() item_count=%d",item_count);
	
	for( i = 0; i < item_count; i++ )
		item_popups[i] = NULL;
	
	ShowCategory52Screen( STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
		STR_GLOBAL_OK, 0,
		STR_GLOBAL_BACK, 0,
		item_count, item_texts, item_icons, (U8**) item_popups, 0, 0, gui_buffer );

	// register left, right arrow key handlers
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_main_option_LSK, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	
	// register LSK, RSK hanlders
	SetLeftSoftkeyFunction( mmi_fmrdo_main_option_LSK, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	
	// register volume inc, dec handlers
	SetKeyHandler( mmi_fmrdo_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_main_option
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
void mmi_fmrdo_exit_main_option( void )
{
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_main_option_highlight_hdlr
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
void mmi_fmrdo_main_option_highlight_hdlr( S32 index )
{
        FMR_TRACE();
	main_option_highlighted_item_text = (U16) GetSeqItemStringId( (U16)MENU_ID_FMRDO_MAIN_OPTION, (U16)index );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_main_option_LSK
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
void mmi_fmrdo_main_option_LSK( void )
{
        FMR_TRACE();
	switch( main_option_highlighted_item_text )
	{
		case STR_ID_FMRDO_CHANNEL_LIST:
			mmi_fmrdo_entry_channel_list();
			break;
		case STR_ID_FMRDO_MANUAL_INPUT:
			mmi_fmrdo_entry_manual_input();
			break;
		case STR_ID_FMRDO_PRESET_AUTO_SEARCH:
			mmi_fmrdo_entry_preset_list_confirm();
			break;
		case STR_ID_FMRDO_SETTINGS:
			mmi_fmrdo_entry_settings();
			break;
#if defined(__MMI_FM_RADIO_RECORD__)
		case STR_ID_FMRDO_RECORD:
			mmi_fmrdo_release_record_button( );
			break;
		case STR_ID_FMRDO_APPEND:
			mmi_fmrdo_release_append_button( );
			break;
		case STR_ID_FMRDO_FILE_LIST:
			mmi_fmrdo_entry_record_list( );
			break;
#endif			
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_channel_list_item
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
pBOOL mmi_fmrdo_channel_list_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	// show frequency text
	U16 freq = g_fmrdo.channel_list.freq[item_index];
	U8 string[20];

	FMR_TRACE_ARG("[mmi_fmrdo_channel_list_item] g_fmrdo.channel_list.freq[%d]= %d",
	        item_index, g_fmrdo.channel_list.freq[item_index]);
	        
	if( freq == 0 )
	{
		pfnUnicodeStrcpy( (S8*)str_buff, GetString( STR_ID_FMRDO_EMPTY ) );
	}
	else
		//kal_wsprintf( str_buff, "FM %d.%d", freq / 10, freq % 10 );
	{		
		sprintf(string,"FM %d.%d", freq / 10, freq % 10);
		AnsiiToUnicodeString((S8 *)str_buff, string);
	}	
		
	
	// show order icon
	*img_buff_p = (UI_image_type) GetImage( (U16)(IMG_GLOBAL_L1 + item_index) );

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_channel_list_item_hint
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S32 item_index
*     UI_string_type *hint_array
* RETURNS
*     S32
* GLOBALS AFFECTED
*     
*****************************************************************************/
S32 mmi_fmrdo_channel_list_item_hint( S32 item_index, UI_string_type *hint_array )
{
	U16 freq = g_fmrdo.channel_list.freq[item_index];
	UI_string_type name = g_fmrdo.channel_list.name[item_index];
	U16 name_len;
	if( freq == 0 || (name_len = (U16)pfnUnicodeStrlen((const S8*)name)) == 0 )
		return 0;
	pfnUnicodeStrcpy( (S8*)hint_array[0], (S8*)name );

	return 1;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_channel_list
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
void mmi_fmrdo_entry_channel_list( void )
{
	U8* gui_buffer;
	S32 highlighted_item;

        FMR_TRACE();
	
	EntryNewScreen( SCR_ID_FMRDO_CHANNEL_LIST, mmi_fmrdo_exit_channel_list, mmi_fmrdo_entry_channel_list, NULL );
	
	// register highligh handler
	RegisterHighlightHandler( mmi_fmrdo_channel_list_highlight_hdlr );
	
	gui_buffer = GetCurrGuiBuffer( SCR_ID_FMRDO_CHANNEL_LIST );
	if( g_fmrdo.channel_index >= 0 )
		highlighted_item = g_fmrdo.channel_index;
	else
		highlighted_item = 0;

	ShowCategory184Screen( STR_ID_FMRDO_CHANNEL_LIST, MAIN_MENU_TITLE_MULTIMEDIA_ICON,	// title text and icon
		STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,							// LSK text and icon
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 								// RSK text and icon
		FM_RADIO_CHANNEL_NUM, mmi_fmrdo_channel_list_item, mmi_fmrdo_channel_list_item_hint,
		highlighted_item, gui_buffer );
	
	// register LSK, RSK hanlders
	SetLeftSoftkeyFunction( mmi_fmrdo_entry_channel_option, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	
	// register left right arrow key handler
	SetKeyHandler( mmi_fmrdo_entry_channel_option, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
	
	// register volume inc, dec handlers
	SetKeyHandler( mmi_fmrdo_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_channel_list
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
void mmi_fmrdo_exit_channel_list( void )
{
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_channel_list_highlight_hdlr
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
void mmi_fmrdo_channel_list_highlight_hdlr( S32 index )
{
	channel_list_highlighted_item_index = (U16)index;
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_manual_input
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
void mmi_fmrdo_entry_manual_input( void )
{
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
	U8 string [20];
	
        FMR_TRACE();

	EntryNewScreen( SCR_ID_FMRDO_MANUAL_INPUT, mmi_fmrdo_exit_manual_input, NULL, NULL );
	
	InitializeCategory57Screen();
   
	guiBuffer = GetCurrGuiBuffer( SCR_ID_FMRDO_MANUAL_INPUT );
	inputBuffer = GetCurrNInputBuffer( SCR_ID_FMRDO_MANUAL_INPUT, &inputBufferSize );

	RegisterHighlightHandler( mmi_fmrdo_manual_input_highlight_hdlr );

	if( guiBuffer == NULL )
	{
		//kal_wsprintf( manual_input_buffer, "%d.%d", g_fmrdo.frequency / 10, g_fmrdo.frequency % 10 );
		sprintf(string,  "%d.%d", g_fmrdo.frequency / 10, g_fmrdo.frequency % 10); //shizh
		AnsiiToUnicodeString((S8 *)manual_input_buffer,string);
	}

	SetInlineItemActivation( &wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemTextEdit( &wgui_inline_items[0], (U8*)manual_input_buffer, MAX_MANUAL_INPUT_LEN, INPUT_TYPE_DECIMAL_NUMERIC );
	LeftJustifyInlineItem( &wgui_inline_items[0] );
	SetInlineItemFullWidth( &wgui_inline_items[0] );
	EnableInlineItemBoundary( &wgui_inline_items[0] );

	DisableCategory57ScreenDone();
	
	// show category screen
	ShowCategory57Screen( STR_ID_FMRDO_MANUAL_INPUT, MAIN_MENU_TITLE_MULTIMEDIA_ICON,	// title text and icon
		STR_GLOBAL_OK, IMG_GLOBAL_OK,									// LSK text and icon
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,								// RSK text and icon
		1,
		0, wgui_inline_items,
		0, guiBuffer );

	SetCategory57LeftSoftkeyFunction( mmi_fmrdo_manual_input_LSK );
	SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );
	
	// register volume inc, dec handlers
	SetKeyHandler( mmi_fmrdo_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_manual_input
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
void mmi_fmrdo_exit_manual_input( void )
{
	history_t	h;
	U16 inputBufferSize;

	CloseCategory57Screen();
	h.scrnID = SCR_ID_FMRDO_MANUAL_INPUT;
	h.entryFuncPtr = mmi_fmrdo_entry_manual_input;
	GetCategoryHistory( h.guiBuffer );
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data( (U8*) h.inputBuffer );
	AddNHistory( h, inputBufferSize );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_manual_input_highlight_hdlr
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
void mmi_fmrdo_manual_input_highlight_hdlr( S32 index )
{
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_manual_input_LSK
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
void mmi_fmrdo_manual_input_LSK( void )
{
	// calculate frequency number
	U16 freq = mmi_fmrdo_get_freq_from_str( manual_input_buffer );
	
	// check frequency input
	if(freq == 0xffff) /* invalid decimal digit */
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMRDO_VALID_FM_DECIMAL ), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)WARNING_TONE );
	}
	else if( freq < MIN_FM_FREQUENCY || freq > MAX_FM_FREQUENCY ) /* warning invalid frequency */
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMRDO_VALID_FM_BANDWIDTH ), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)WARNING_TONE );
	}
	else
	{
		mmi_fmrdo_set_frequency( freq );
		GoBackToHistory( SCR_ID_FMRDO_MAIN );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_preset_list_confirm
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
void mmi_fmrdo_entry_preset_list_confirm( void )
{
        FMR_TRACE();
	EntryNewScreen( SCR_ID_FMRDO_PRESET_LIST_CONFIRM, mmi_fmrdo_exit_preset_list_confirm, NULL, NULL );
	
	ShowCategory164Screen( STR_GLOBAL_YES, IMG_GLOBAL_YES,
		STR_GLOBAL_NO,IMG_GLOBAL_NO, 
		STR_ID_FMRDO_PRESET_LIST, IMG_GLOBAL_QUESTION, NULL );

	playRequestedTone( WARNING_TONE );

	SetLeftSoftkeyFunction( mmi_fmrdo_preset_list_confirm_LSK, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_preset_list_confirm
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
void mmi_fmrdo_exit_preset_list_confirm( void )
{
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_preset_list_LSK
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
void mmi_fmrdo_preset_list_confirm_LSK( void )
{
#ifdef __MMI_FM_RADIO_BIND_EARPHONE__
	if( isEarphonePlugged_1 && !g_fmrdo.is_power_on )
#else
	if( !g_fmrdo.is_power_on )
#endif
		mmi_fmrdo_power_on_before_preset_auto_search();
	
	if( g_fmrdo.is_power_on ) // if audio is block, power won't be turned on
	{
		mmi_fmrdo_entry_searching();
	
		// start search all channels
		mmi_fmrdo_search_all_channels_start( mmi_fmrdo_preset_list_done );
	}
	else
	{
		DisplayPopup( (U8*)GetString( STR_ID_FMRDO_POWER_ON_FIRST ), IMG_GLOBAL_WARNING, 1, 1000, (U8)WARNING_TONE );
		DeleteUptoScrID( SCR_ID_FMRDO_MAIN_OPTION );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_settings
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
void mmi_fmrdo_entry_settings( void )
{
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
	U16 inline_count = 0;
	UI_image_ID_type item_icons[] = 
	{
		IMG_ID_FMRDO_BACKGROUND_PLAY,
		0,
		/*yanghong 20060407 add*/
		IMG_ID_FMRDO_AUDIO_OUT,
		0
		/*yanghong add end*/
//		IMG_ID_FMRDO_SKIN,//yuzl delete skin
//		0
#if defined(__MMI_FM_RADIO_RECORD__)
		,
		IMG_ID_FMRDO_RECORD_FORMAT,
		0
//jiashuo debug for jessamine ,20080513
//		,
//		IMG_ID_FMRDO_RECORD_STORAGE,
//		0
#endif		
	};
        FMR_TRACE();
	EntryNewScreen( SCR_ID_FMRDO_SETTINGS, mmi_fmrdo_exit_settings, NULL, NULL );
	
	InitializeCategory57Screen();
   
	guiBuffer = GetCurrGuiBuffer( SCR_ID_FMRDO_SETTINGS );
	inputBuffer = GetCurrNInputBuffer( SCR_ID_FMRDO_SETTINGS, &inputBufferSize );

	RegisterHighlightHandler( mmi_fmrdo_settings_highlight_hdlr );

	if( guiBuffer == NULL )
	{
		selected_background_play = g_fmrdo.is_background_play;
		selected_audio_out	= g_fmrdo.is_audio_out;//yanghong 20060417 add
		selected_skin_index = g_fmrdo.skin_index;
#if defined(__MMI_FM_RADIO_RECORD__)
		selected_record_format = g_fmrdo.rec.format;
		selected_record_storage = g_fmrdo.rec.storage;
#endif		
	}

	// background play label
	SetInlineItemActivation( &wgui_inline_items[inline_count],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemCaption( &wgui_inline_items[inline_count++], (U8*)GetString( STR_ID_FMRDO_BACKGROUND_PLAY ) );

	// background play select
	background_play_texts[0] = (UI_string_type) GetString( STR_GLOBAL_OFF );
	background_play_texts[1] = (UI_string_type) GetString( STR_GLOBAL_ON );
	SetInlineItemActivation( &wgui_inline_items[inline_count], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemSelect( &wgui_inline_items[inline_count++], 2, (U8**)background_play_texts, &selected_background_play );

	/*yanghong 20060407 add*/
	//audio out
	SetInlineItemActivation( &wgui_inline_items[inline_count], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption( &wgui_inline_items[inline_count++], (U8*)GetString( STR_ID_FMRDO_AUDIO_OUT) );
	audio_out_texts[0] = (UI_string_type) GetString( STR_GLOBAL_OFF );
	audio_out_texts[1] = (UI_string_type) GetString( STR_GLOBAL_ON );
	SetInlineItemActivation( &wgui_inline_items[inline_count], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemSelect( &wgui_inline_items[inline_count++], 2, (U8**)audio_out_texts, &selected_audio_out);
	/*yanghong add end*/
#if 0 //yuzl delete shin menu in radio setting
	// skin label
	SetInlineItemActivation( &wgui_inline_items[inline_count],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemCaption( &wgui_inline_items[inline_count++], (U8*)GetString( STR_ID_FMRDO_SKIN ) );
	
	// skin select
	skin_texts[0] = (UI_string_type) GetString( STR_ID_FMRDO_SKIN_1 );
	skin_texts[1] = (UI_string_type) GetString( STR_ID_FMRDO_SKIN_2 );
	skin_texts[2] = (UI_string_type) GetString( STR_ID_FMRDO_SKIN_3 );
	SetInlineItemActivation( &wgui_inline_items[inline_count], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemSelect( &wgui_inline_items[inline_count++], 3, (U8**)skin_texts, &selected_skin_index );	
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
	// record format label
	SetInlineItemActivation( &wgui_inline_items[inline_count],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemCaption( &wgui_inline_items[inline_count++], (U8*)GetString( STR_ID_FMRDO_RECORD_FORMAT ) );

	// record format select
	record_format_texts[FM_FORMAT_AMR] = (UI_string_type) GetString( STR_ID_FMRDO_FORMAT_AMR );
	record_format_texts[FM_FORMAT_WAV] = (UI_string_type) GetString( STR_ID_FMRDO_FORMAT_WAV );
	SetInlineItemActivation( &wgui_inline_items[inline_count], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemSelect( &wgui_inline_items[inline_count++], 2, (U8**)record_format_texts, &selected_record_format );	
//jiashuo del for jessamine , 20080513
#if 0
	// If card exists.
	if(MMI_CARD_DRV > 0)
	{
		// record storage label
		SetInlineItemActivation( &wgui_inline_items[inline_count],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
		SetInlineItemCaption( &wgui_inline_items[inline_count++], (U8*)GetString( STR_ID_FMRDO_RECORD_STORAGE ) );

		// record storage select
		record_storage_texts[FM_STORAGE_PHONE] = (UI_string_type) GetString( STR_ID_FMRDO_STORAGE_PHONE );
		record_storage_texts[FM_STORAGE_CARD] = (UI_string_type) GetString( STR_ID_FMRDO_STORAGE_CARD );
		SetInlineItemActivation( &wgui_inline_items[inline_count], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
		SetInlineItemSelect( &wgui_inline_items[inline_count++], 2, (U8**)record_storage_texts, &selected_record_storage );		
	}
	else
	{
		selected_record_storage = FM_STORAGE_PHONE;
	}
#endif
//jiashuo del for jessamine
#endif
		
	DisableCategory57ScreenDone();

	if(inputBuffer != NULL)
	{
		SetCategory57Data(wgui_inline_items, inline_count, inputBuffer);
	}
	
	// show category screen
	ShowCategory57Screen( STR_ID_FMRDO_SETTINGS, MAIN_MENU_TITLE_MULTIMEDIA_ICON,	// title text and icon
		STR_GLOBAL_OK, IMG_GLOBAL_OK,								// LSK text and icon
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,							// RSK text and icon
		inline_count,
		item_icons, wgui_inline_items,
		0, guiBuffer );

	SetCategory57LeftSoftkeyFunction( mmi_fmrdo_settings_LSK );
	SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );
	
	// register volume inc, dec handlers
	SetKeyHandler( mmi_fmrdo_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_settings
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
void mmi_fmrdo_exit_settings( void )
{
	history_t	h;
	U16 inputBufferSize;

	CloseCategory57Screen();
	h.scrnID = SCR_ID_FMRDO_SETTINGS;
	h.entryFuncPtr = mmi_fmrdo_entry_settings;
	GetCategoryHistory( h.guiBuffer );
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data( (U8*) h.inputBuffer );
	AddNHistory( h, inputBufferSize );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_settings_highlight_hdlr
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
void mmi_fmrdo_settings_highlight_hdlr( S32 index )
{
	ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK );
	SetCategory57LeftSoftkeyFunction( mmi_fmrdo_settings_LSK );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_settings_LSK
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
void mmi_fmrdo_settings_LSK( void )
{
	BOOL skin_changed = (g_fmrdo.skin_index != selected_skin_index);
	BOOL audio_changed = (g_fmrdo.is_audio_out != selected_audio_out); //yanghong 20060407 add
#if defined(__MMI_FM_RADIO_RECORD__)
	if((selected_record_storage == FM_STORAGE_CARD) && (!fmgr_is_msdc_present()))
	{
		DisplayPopup((PU8)GetString(STR_ID_FMRDO_NO_CARD) ,IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		return;
	}
#endif
	
	g_fmrdo.is_background_play = selected_background_play;
	g_fmrdo.is_audio_out = selected_audio_out; //yanghong 20060407 add
	g_fmrdo.skin_index = (unsigned char)selected_skin_index;

#if defined(__MMI_FM_RADIO_RECORD__)
	g_fmrdo.rec.format = selected_record_format;
	g_fmrdo.rec.storage = selected_record_storage;
#endif	

	mmi_fmrdo_write_settings_to_nvram();

/*yanghong 20060407 add begin: during playing FM, path can't shift to another device.
 * so the fm must power off first, and then power on again in the other device.
 */
	if(audio_changed &&(g_fmrdo.is_power_on))
	{
		mdi_audio_stop_fmr();
		//kal_sleep_task(2);
		
		if(g_fmrdo.is_audio_out)
		{
			mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER_BOTH, (mdi_callback) mmi_fmrdo_preempted );
		}
		else 
		{
			mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER2, (mdi_callback) mmi_fmrdo_preempted );
		}
		mdi_fmr_set_freq( g_fmrdo.frequency );
		mdi_audio_set_volume( AUD_VOLUME_FMR, g_fmrdo.volume );
		g_fmrdo.state = FMRDO_STATE_READY;
		g_fmrdo.is_power_on = TRUE;
	}
/*yanghong add end*/
	if( skin_changed )
		GoBackToHistory( SCR_ID_FMRDO_MAIN );
	else
		GoBackHistory();
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_channel_option
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
void mmi_fmrdo_entry_channel_option( void )
{
	BOOL is_empty_channel = (g_fmrdo.channel_list.freq[channel_list_highlighted_item_index] == 0);
	U16 item_texts[] = { STR_ID_FMRDO_PLAY, STR_GLOBAL_DELETE,STR_ID_FMRDO_EDIT };
	U16 item_icons[] = { IMG_GLOBAL_L1, IMG_GLOBAL_L2,IMG_GLOBAL_L3 };
	U8* gui_buffer;
	U8* item_popups[] = { NULL, NULL,NULL };
	
	EntryNewScreen( SCR_ID_FMRDO_CHANNEL_OPTION, mmi_fmrdo_exit_channel_option, NULL, NULL );
	
	gui_buffer = GetCurrGuiBuffer( SCR_ID_FMRDO_CHANNEL_OPTION );
	RegisterHighlightHandler( mmi_fmrdo_channel_option_highlight_hdlr );
	
	if( is_empty_channel )
	{
		ShowCategory52Screen( STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
			STR_GLOBAL_OK, 0,
			STR_GLOBAL_BACK, 0,
			1, 
			item_texts+2,
			item_icons, 
			(U8**) item_popups, 0, 0, gui_buffer );
	}
	else
	{
		ShowCategory52Screen( STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
			STR_GLOBAL_OK, 0,
			STR_GLOBAL_BACK, 0,
			3, item_texts, item_icons, (U8**) item_popups, 0, 0, gui_buffer );
	}

	// register left, right arrow key handlers
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_channel_option_LSK, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	
	// register LSK, RSK hanlders
	SetLeftSoftkeyFunction( mmi_fmrdo_channel_option_LSK, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	
	// register volume inc, dec handlers
	SetKeyHandler( mmi_fmrdo_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_channel_option
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
void mmi_fmrdo_exit_channel_option( void )
{
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_channel_option_highlight_hdlr
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
void mmi_fmrdo_channel_option_highlight_hdlr( S32 index )
{
    BOOL is_empty_channel = (g_fmrdo.channel_list.freq[channel_list_highlighted_item_index] == 0);
    U16 item_texts[] = {STR_ID_FMRDO_PLAY, STR_GLOBAL_DELETE, STR_ID_FMRDO_EDIT};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    channel_option_highlighted_item_text = (is_empty_channel ? item_texts[index + 2] : item_texts[index]);

}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_channel_option_LSK
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
void mmi_fmrdo_channel_option_LSK( void )
{
	FMR_TRACE();
	switch( channel_option_highlighted_item_text )
	{
		case STR_ID_FMRDO_PLAY:
			if( !g_fmrdo.is_power_on )
			{
#ifdef __MMI_FM_RADIO_BIND_EARPHONE__
				if( !isEarphonePlugged_1 )
				{
					DisplayPopup( (PU8)GetString( STR_ID_FMRDO_PLEASE_PLUG_IN_EARPHONE ), IMG_GLOBAL_WARNING, 1, 2000, (U8)WARNING_TONE );
					return;
				}
#endif			
				mmi_fmrdo_power_on( TRUE );
			}

			//Added by liuxl for fixing bug-13554 on 20090730-Start
                      {
                        S16 error;
                        U16 freq = g_fmrdo.channel_list.freq[channel_list_highlighted_item_index];
                        WriteValue( NVRAM_FMRDO_FREQUENCY, &freq, DS_SHORT, &error );
                        //Added by liuxl for fixing bug-13554 on 20090730-End
                        
                        mmi_fmrdo_goto_channel( channel_list_highlighted_item_index );
                        GoBackToHistory( SCR_ID_FMRDO_MAIN );
                      }
			break;
              case STR_GLOBAL_DELETE:
                    mmi_fmrdo_entry_clear_channel_confirm();
                    break;

		case STR_ID_FMRDO_EDIT:
			mmi_fmrdo_entry_channel_edit();
			break;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_searching
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
void mmi_fmrdo_entry_searching( void )
{
	EntryNewScreen( SCR_ID_FMRDO_SEARCHING, mmi_fmrdo_exit_searching, NULL, NULL );

	// clear End key
	ClearKeyHandler( KEY_END, KEY_EVENT_DOWN );
	ClearKeyHandler( KEY_END, KEY_LONG_PRESS );

	// Show searching screen
	ShowCategory66Screen( STR_ID_FMRDO_PRESET_AUTO_SEARCH, 0,
		0, 0, STR_GLOBAL_CANCEL, 0, 
		(U8*)GetString( STR_ID_FMRDO_SEARCHING ), IMG_GLOBAL_PROGRESS, NULL );

	// Set RSK to abort searching
	SetRightSoftkeyFunction( mmi_fmrdo_searching_abort, KEY_EVENT_UP );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_searching
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
void mmi_fmrdo_exit_searching( void )
{
	if( g_fmrdo.state == FMRDO_STATE_SEARCH_ALL )
	{
		history_t currHistory;
	
		currHistory.scrnID = SCR_ID_FMRDO_SEARCHING;
		currHistory.entryFuncPtr = mmi_fmrdo_entry_searching;
		GetCategoryHistory( currHistory.guiBuffer );
		pfnUnicodeStrcpy ( (S8*)currHistory.inputBuffer, (S8*)L"" );
		AddHistory( currHistory );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_searching_abort
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
void mmi_fmrdo_searching_abort( void )
{
	mmi_fmrdo_search_all_channels_stop();
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_channel_edit
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
void mmi_fmrdo_entry_channel_edit( void )
{
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
	U8 string[20];
	
	UI_image_ID_type item_icons[] = 
	{
		IMG_ID_FMRDO_CHANNEL_NAME,
		0,
		IMG_ID_FMRDO_FREQUENCY,
		0
	};

	EntryNewScreen( SCR_ID_FMRDO_CHANNEL_EDIT, mmi_fmrdo_exit_channel_edit, NULL, NULL );
	
	InitializeCategory57Screen();
   
	inputBuffer = GetCurrNInputBuffer( SCR_ID_FMRDO_CHANNEL_EDIT, &inputBufferSize );

	RegisterHighlightHandler( mmi_fmrdo_channel_edit_highlight_hdlr );

	if( (guiBuffer = GetCurrGuiBuffer( SCR_ID_FMRDO_CHANNEL_EDIT )) == NULL )
	{
		U16 freq = g_fmrdo.channel_list.freq[channel_list_highlighted_item_index];
		pfnUnicodeStrcpy( (S8*)name_inline_editor_buffer, (S8*)g_fmrdo.channel_list.name[channel_list_highlighted_item_index] );
		if( freq == 0 )
		{
			freq_inline_editor_buffer[0] = 0;
		}
		else
		{
			//kal_wsprintf( freq_inline_editor_buffer, "%d.%d", freq / 10, freq % 10 );
			sprintf(string,"%d.%d", freq / 10, freq % 10);
			AnsiiToUnicodeString((S8 *)freq_inline_editor_buffer, string);
		}

		channel_name_edit_done = FALSE;
	}

	// channel name label
	SetInlineItemActivation( &wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemCaption( &wgui_inline_items[0], (U8*)GetString( STR_ID_FMRDO_CHANNEL_NAME ) );

	// channel name
	SetInlineItemActivation( &wgui_inline_items[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemDisplayOnly( &wgui_inline_items[1], (U8*)name_inline_editor_buffer );
	
	// frequency label
	SetInlineItemActivation( &wgui_inline_items[2], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemCaption( &wgui_inline_items[2], (U8*)GetString( STR_ID_FMRDO_FREQUENCY ) );
	
	// frequency input
	SetInlineItemActivation( &wgui_inline_items[3], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemTextEdit( &wgui_inline_items[3], (U8*)freq_inline_editor_buffer, MAX_MANUAL_INPUT_LEN, INPUT_TYPE_DECIMAL_NUMERIC );
	LeftJustifyInlineItem( &wgui_inline_items[3] );
	SetInlineItemFullWidth( &wgui_inline_items[3] );
	EnableInlineItemBoundary( &wgui_inline_items[3] );

	DisableCategory57ScreenDone();

	// show category screen
	ShowCategory57Screen( STR_ID_FMRDO_EDIT, MAIN_MENU_TITLE_MULTIMEDIA_ICON,	// title text and icon
		STR_GLOBAL_OK, IMG_GLOBAL_OK,								// LSK text and icon
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,							// RSK text and icon
		4,
		item_icons, wgui_inline_items,
		0, guiBuffer );

	// register volume inc, dec handlers - inline screen will register vol up and vol down key
	//SetKeyHandler( mmi_fmrdo_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	//SetKeyHandler( mmi_fmrdo_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_channel_edit
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
void mmi_fmrdo_exit_channel_edit( void )
{
	history_t	h;
	U16 inputBufferSize;

	CloseCategory57Screen();
	h.scrnID = SCR_ID_FMRDO_CHANNEL_EDIT;
	h.entryFuncPtr = mmi_fmrdo_entry_channel_edit;
	GetCategoryHistory( h.guiBuffer );
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data( (U8*) h.inputBuffer );
	AddNHistory( h, inputBufferSize );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_channel_edit_highlight_hdlr
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
void mmi_fmrdo_channel_edit_highlight_hdlr( S32 index )
{
	if( index == 1 ) // edit channel name
	{
		ChangeLeftSoftkey( STR_GLOBAL_EDIT, 0 );
		SetCategory57LeftSoftkeyFunction( mmi_fmrdo_entry_channel_name_edit );
		if( channel_name_edit_done )
		{
			ChangeRightSoftkey( STR_GLOBAL_DONE, 0 );
			SetCategory57RightSoftkeyFunctions( mmi_fmrdo_channel_edit_LSK, mmi_fmrdo_channel_edit_LSK );
		}
		else
			SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );
	}
	else
	{
		SetCategory57LeftSoftkeyFunction( mmi_fmrdo_channel_edit_LSK );
		SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_channel_edit_LSK
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
void mmi_fmrdo_channel_edit_LSK( void )
{
	// calculate frequency number
	U16 freq = mmi_fmrdo_get_freq_from_str( freq_inline_editor_buffer );
	
	// check frequency input
	if( freq_inline_editor_buffer[0] == 0 )
	{
		if( g_fmrdo.channel_list.freq[channel_list_highlighted_item_index] == 0 ) // it's empty, no need to clear it
			DisplayPopup( (PU8)GetString( STR_ID_FMRDO_VALID_FM_BANDWIDTH ), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)WARNING_TONE );
		else
			mmi_fmrdo_entry_clear_channel_confirm(); // clear channel
	}
	else if(freq == 0xffff) /* invalid decimal digit */
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMRDO_VALID_FM_DECIMAL ), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)WARNING_TONE );
	}	
	else if( freq < MIN_FM_FREQUENCY || freq > MAX_FM_FREQUENCY ) /* warning invalid frequency */
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMRDO_VALID_FM_BANDWIDTH ), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)WARNING_TONE );
	}
	else
	{
		mmi_fmrdo_entry_save_channel_confirm();
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_channel_name_edit
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
void mmi_fmrdo_entry_channel_name_edit( void )
{
	U8* gui_buffer;

	EntryNewScreen( SCR_ID_FMRDO_CHANNEL_NAME_EDIT, NULL, mmi_fmrdo_entry_channel_name_edit, NULL );

	if( (gui_buffer = GetCurrGuiBuffer( SCR_ID_FMRDO_CHANNEL_NAME_EDIT)) == NULL )
		pfnUnicodeStrcpy( (S8*)name_text_editor_buffer, (S8*)name_inline_editor_buffer );

	ShowCategory5Screen( STR_ID_FMRDO_CHANNEL_NAME, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
		STR_GLOBAL_OPTIONS, 0,
		STR_GLOBAL_BACK, 0,
		INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
		(U8*) name_text_editor_buffer, MAX_NAME_INPUT_LEN, 
		gui_buffer );

#ifdef __MMI_GB__
	GBSetInputboxLSKFunction(mmi_fmrdo_entry_channel_name_edit_option);
#endif
	
	SetLeftSoftkeyFunction( mmi_fmrdo_entry_channel_name_edit_option, KEY_EVENT_UP );
	SetCategory5RightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	RegisterInputMethodScreenCloseFunction( GoBackHistory );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_channel_name_edit_option
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
void mmi_fmrdo_entry_channel_name_edit_option( void )
{
	U16 item_texts[] = { STR_GLOBAL_DONE, STR_GLOBAL_INPUT_METHOD};
	U16 item_icons[] = { IMG_GLOBAL_L1, IMG_GLOBAL_L2 };
	
	EntryNewScreen( SCR_ID_FMRDO_CHANNEL_NAME_EDIT_OPTION, NULL, NULL, NULL );

	RegisterHighlightHandler( mmi_fmrdo_channel_name_edit_option_highlight_hdlr );

	ShowCategory52Screen( STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
		STR_GLOBAL_OK, 0,
		STR_GLOBAL_BACK, 0, 
		2, item_texts, item_icons,
		NULL, 0, 0, NULL );
	
	// register left, right arrow key handlers
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_channel_name_edit_option_LSK, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	
	// register LSK, RSK handlers
	SetLeftSoftkeyFunction( mmi_fmrdo_channel_name_edit_option_LSK, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_channel_name_edit_option_highlight_hdlr
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
void mmi_fmrdo_channel_name_edit_option_highlight_hdlr( S32 index )
{
	U16 item_texts[] = { STR_GLOBAL_DONE, STR_GLOBAL_INPUT_METHOD };
	channel_name_edit_option_highlighted_item_text = item_texts[index];
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_channel_name_edit_option_LSK
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
void mmi_fmrdo_channel_name_edit_option_LSK( void )
{
	switch( channel_name_edit_option_highlighted_item_text )
	{
		case STR_GLOBAL_DONE:
			pfnUnicodeStrcpy( (S8*)name_inline_editor_buffer, (S8*)name_text_editor_buffer );
			channel_name_edit_done = TRUE;
			GoBackToHistory( SCR_ID_FMRDO_CHANNEL_EDIT );
			break;
		case STR_GLOBAL_INPUT_METHOD:
			EntryInputMethodScreen();
			break;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_clear_channel_confirm
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
void mmi_fmrdo_entry_clear_channel_confirm( void )
{
	EntryNewScreen( SCR_ID_FMRDO_CLEAR_CHANNEL_CONFIRM, mmi_fmrdo_exit_clear_channel_confirm, mmi_fmrdo_entry_clear_channel_confirm, NULL );
	
	ShowCategory164Screen( STR_GLOBAL_YES, IMG_GLOBAL_YES,
		STR_GLOBAL_NO,IMG_GLOBAL_NO, 
		STR_GLOBAL_CLEAR, IMG_GLOBAL_QUESTION, NULL );

	playRequestedTone( WARNING_TONE );

	SetLeftSoftkeyFunction( mmi_fmrdo_clear_channel_LSK, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_clear_channel_confirm
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
void mmi_fmrdo_exit_clear_channel_confirm( void )
{
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_clear_channel_LSK
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
void mmi_fmrdo_clear_channel_LSK( void )
{
	S16 error;

	mmi_fmrdo_set_channel( channel_list_highlighted_item_index, 0, (UI_string_type)L"" );

	// write channel list from nvram
	WriteRecord( NVRAM_EF_FM_RADIO_LID, 1, (void*)&g_fmrdo.channel_list, NVRAM_EF_FM_RADIO_SIZE, &error );
	
	GoBackToHistory( SCR_ID_FMRDO_CHANNEL_LIST );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_entry_save_channel_confirm
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
void mmi_fmrdo_entry_save_channel_confirm( void )
{
	EntryNewScreen( SCR_ID_FMRDO_SAVE_CHANNEL_CONFIRM, mmi_fmrdo_exit_save_channel_confirm, mmi_fmrdo_entry_save_channel_confirm, NULL );
	
	ShowCategory164Screen( STR_GLOBAL_YES, IMG_GLOBAL_YES,
		STR_GLOBAL_NO,IMG_GLOBAL_NO, 
		STR_GLOBAL_SAVE, IMG_GLOBAL_QUESTION, NULL );

	playRequestedTone( WARNING_TONE );

	SetLeftSoftkeyFunction( mmi_fmrdo_save_channel_LSK, KEY_EVENT_UP );
	SetRightSoftkeyFunction( mmi_fmrdo_save_channel_RSK, KEY_EVENT_UP );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_exit_save_channel_confirm
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
void mmi_fmrdo_exit_save_channel_confirm( void )
{
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_save_channel_LSK
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
void mmi_fmrdo_save_channel_LSK( void )
{
	S16 error;
	U16 freq = mmi_fmrdo_get_freq_from_str( freq_inline_editor_buffer );
	
	// set channel
	mmi_fmrdo_set_channel( channel_list_highlighted_item_index, freq, name_inline_editor_buffer );

	// write channel list from nvram
	WriteRecord( NVRAM_EF_FM_RADIO_LID, 1, (void*)&g_fmrdo.channel_list, NVRAM_EF_FM_RADIO_SIZE, &error );

	GoBackToHistory( SCR_ID_FMRDO_CHANNEL_LIST );
}

/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_save_channel_RSK
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
void mmi_fmrdo_save_channel_RSK( void )
{
	GoBackToHistory( SCR_ID_FMRDO_CHANNEL_LIST );
}

#if defined(__MMI_FM_RADIO_RECORD__)
#define MMI_FMRADIO_REC
/*****************************************************************************
* FUNCTION
*     mmi_fmrdo_press_search_button
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
void mmi_fmrdo_press_record_button( void )
{
        FMR_TRACE();

	if( g_fmrdo.is_button_down )
		return;

	g_fmrdo.selected_button = FMRDO_BUTTON_REC;
	g_fmrdo.is_button_down = TRUE;
	
	mmi_fmrdo_redraw_main_toggle_button( g_fmrdo.selected_button, g_fmrdo.is_button_down, g_fmrdo.is_search_on ,TRUE);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_release_record_button
* DESCRIPTION
*   Send out the FM radio record request when release record button
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_release_record_button( void )
{
        FMR_TRACE();

	/*Check if in USB mode before enter setting*/
#if defined(__USB_IN_NORMAL_MODE__)
	if(mmi_usb_is_in_mass_storage_mode())
	{
		mmi_usb_app_unavailable_popup(0);
		return;
	}
#endif

	/*Enter from option menu, go back to main screen and record*/
	if( !g_fmrdo.in_main_screen )
	{
		ClearInputEventHandler(MMI_DEVICE_ALL);
		GoBackHistory();
	}

	mmi_fmrdo_send_record_req(FALSE);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_release_append_button
* DESCRIPTION
*   Select a file to append after release append button
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_release_append_button( void )
{
	UI_character_type path[MAX_FM_PATH_LEN];
	FMGR_FILTER	filter;

        FMR_TRACE();

	/*Check if in USB mode before enter setting*/
#if defined(__USB_IN_NORMAL_MODE__)
	if(mmi_usb_is_in_mass_storage_mode())
	{
		mmi_usb_app_unavailable_popup(0);
		return;
	}
#endif

	mmi_fmrdo_util_get_storage_path(path, g_fmrdo.rec.storage);

	FMGR_FILTER_INIT(&filter);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_AMR);

	mmi_fmgr_select_path_and_enter(APP_FMRDO, FMGR_SELECT_FILE, filter, 
												(PS8)path, mmi_fmrdo_select_file_callback);
   
	ClearKeyHandler( KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_release_stop_button
* DESCRIPTION
*   Stop FM radio record when release stop button
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_release_stop_button( void )
{
	/*Abort by user*/
	g_fmrdo.rec.user_abort = TRUE;
	mmi_fmrdo_stop_record();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_release_pause_button
* DESCRIPTION
*   Pause FM radio record when release pause button
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_release_pause_button(void)
{
	mmi_fmrdo_pause_record();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_release_resume_button
* DESCRIPTION
*   Resume FM radio record when release resuem button
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_release_resume_button(void)
{
	mmi_fmrdo_resume_record();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_select_file_callback
* DESCRIPTION
*   The callback function for file manager after select a file to append
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_select_file_callback(void* fullname, int is_short)
{
	if( fullname == NULL )
	{
		GoBackHistory();
	}
	else
	{
		pfnUnicodeStrcpy((S8*)g_fmrdo.rec.fullName, (S8*)fullname);  

		GoBackToHistory(SCR_ID_FMRDO_MAIN);
		mmi_fmrdo_send_record_req(TRUE);

	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_send_record_req
* DESCRIPTION
*   Turn on the FM radio in advace and send out the record request.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_send_record_req(BOOL is_append)
{
	if( !g_fmrdo.in_main_screen )
	{
		return;
	}

	if(g_fmrdo.is_power_on == FALSE)
	{
#if defined(__MMI_FM_RADIO_BIND_EARPHONE__)
		if( !isEarphonePlugged_1 )
		{
			DisplayPopup( (PU8)GetString( STR_ID_FMRDO_PLEASE_PLUG_IN_EARPHONE ), IMG_GLOBAL_WARNING, 1, 2000, (U8)WARNING_TONE );
			return;
		}
#endif
		// stop if it's searching
		mmi_fmrdo_search_frequency_stop();
		mmi_fmrdo_power_on(TRUE);
		mmi_fmrdo_redraw_main_toggle_button( FMRDO_BUTTON_POWER, g_fmrdo.is_button_down, g_fmrdo.is_power_on ,TRUE);
	}
	//jiashuo add for jessamine 20080514,only card disc can use
	if( MMI_CARD_DRV <= 0 )
	{
		mmi_fmrdo_record_error_hdlr(MDI_AUDIO_NO_DISC);
		return;
	}

	if(!is_append)
	{
		mmi_fmrdo_util_get_new_file(g_fmrdo.rec.fullName);
	}
	
	g_fmrdo.rec.is_append = is_append;
	g_fmrdo.rec.user_abort	 = FALSE;
	mmi_fmrdo_start_record(g_fmrdo.rec.fullName, g_fmrdo.rec.format);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_entry_main_record_screen
* DESCRIPTION
*   Entry record screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_entry_main_record_screen(void)
{
	mmi_fmrdo_redraw_main_toggle_button( FMRDO_BUTTON_REC, FALSE, TRUE ,TRUE);
	mmi_fmrdo_change_main_SK_record();

	//ClearKeyHandler(KEY_ENTER, KEY_EVENT_UP);
	ClearInputEventHandler(MMI_DEVICE_ALL);

	SetKeyHandler( mmi_fmrdo_release_pause_button, KEY_LSK, KEY_EVENT_UP );
	SetKeyHandler( mmi_fmrdo_release_stop_button, KEY_RSK, KEY_EVENT_UP );	
#if defined(__MMI_MAINLCD_220X176__)
	SetKeyHandler( NULL, KEY_END, KEY_EVENT_DOWN);	
	SetKeyHandler( mmi_fmrdo_release_stop_button, KEY_END, KEY_EVENT_UP );	
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_register_down_handler(mmi_fmrdo_pen_down_hdlr);
	mmi_pen_register_up_handler(mmi_fmrdo_pen_up_hdlr);
	mmi_pen_register_move_handler(mmi_fmrdo_pen_move_hdlr);
#endif
#endif
	// register volume inc, dec handlers
	SetKeyHandler( mmi_fmrdo_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );	
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_entry_main_pause_screen
* DESCRIPTION
*   Entry pause screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_entry_main_pause_screen(void)
{
#if defined(__MMI_MAINLCD_220X176__)
	mmi_fmrdo_redraw_main_toggle_button( FMRDO_BUTTON_REC_PAUSE, FALSE, FALSE ,TRUE);
#else
	mmi_fmrdo_redraw_main_toggle_button( FMRDO_BUTTON_REC_PAUSE, FALSE, TRUE ,TRUE);
#endif
	mmi_fmrdo_change_main_SK_pause();

	//ClearKeyHandler(KEY_ENTER, KEY_EVENT_UP);
	ClearInputEventHandler(MMI_DEVICE_ALL);

	SetKeyHandler( mmi_fmrdo_release_resume_button, KEY_LSK, KEY_EVENT_UP );
	SetKeyHandler( mmi_fmrdo_release_stop_button, KEY_RSK, KEY_EVENT_UP );
#if defined(__MMI_MAINLCD_220X176__)
	SetKeyHandler( NULL, KEY_END, KEY_EVENT_DOWN);	
	SetKeyHandler( mmi_fmrdo_release_stop_button, KEY_END, KEY_EVENT_UP );	
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_register_down_handler(mmi_fmrdo_pen_down_hdlr);
	mmi_pen_register_up_handler(mmi_fmrdo_pen_up_hdlr);
	mmi_pen_register_move_handler(mmi_fmrdo_pen_move_hdlr);
#endif
#endif

	// register volume inc, dec handlers
	SetKeyHandler( mmi_fmrdo_inc_volume, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( mmi_fmrdo_dec_volume, KEY_VOL_DOWN, KEY_EVENT_DOWN );	
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_entry_restore_main_screen
* DESCRIPTION
*   Restore to FM playing screen from record or pause screen.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_entry_restore_main_screen(void)
{
	mmi_fmrdo_util_stop_duration();

	mmi_fmrdo_redraw_main_toggle_button( FMRDO_BUTTON_REC, FALSE, TRUE ,FALSE);
	mmi_fmrdo_redraw_main_time_duration( FALSE );
	mmi_fmrdo_restore_main_SK();

	//SetKeyHandler( mmi_fmrdo_release_record_button, KEY_ENTER, KEY_EVENT_UP );
	
	//SetKeyHandler( mmi_fmrdo_entry_main_option, KEY_LSK, KEY_EVENT_UP );
	//SetKeyHandler( GoBackHistory, KEY_RSK, KEY_EVENT_UP );
	mmi_fmrdo_main_register_keys();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_record_stopped_hdlr
* DESCRIPTION
*   Handler when stop FM record. Need to enter file name if it is not an append file.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_record_stopped_hdlr(void)
{
	UI_string_type filename;

	mmi_fmrdo_util_stop_duration();

	/*not abort by user, exit directly*/
	if(!g_fmrdo.rec.user_abort)
	{
		return;
	}

	/*No need to enter file name*/
	if(g_fmrdo.rec.is_append)	
	{
		mmi_fmrdo_entry_restore_main_screen();
	}
	else
	{
		filename=mmi_fmrdo_util_get_file_name(g_fmrdo.rec.fullName);
		memcpy(fmrdo_editor_buff, filename, (MAX_FM_INPUT_FILE_LEN+2)*ENCODING_LENGTH);
		mmi_fmrdo_util_remove_file_ext(fmrdo_editor_buff);

		mmi_fmrdo_entry_edit_filename();
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_entry_edit_filename
* DESCRIPTION
*   Entry edit file name screen.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_entry_edit_filename(void)
{
	U8* guiBuffer;

	EntryNewScreen( SCR_ID_FMRDO_EDIT_FILENAME, NULL, mmi_fmrdo_entry_edit_filename, NULL );
	ClearInputEventHandler(MMI_DEVICE_ALL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_FMRDO_EDIT_FILENAME); 
	ShowCategory5Screen(STR_ID_FMRDO_TITLE, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
               STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
               STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
               INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
               (U8*) fmrdo_editor_buff, (MAX_FM_INPUT_FILE_LEN+1), 
               guiBuffer);
	
#if defined(__MMI_GB__)                                                               
	GBSetInputboxLSKFunction(mmi_fmrdo_entry_edit_filename_option); 
#endif             

	SetLeftSoftkeyFunction(mmi_fmrdo_entry_edit_filename_option, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(mmi_fmrdo_entry_edit_abort_query, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_entry_edit_abort_query
* DESCRIPTION
*   Abort reocrd confirm
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_entry_edit_abort_query(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO, 
				get_string(STR_GLOBAL_ABORT), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(mmi_fmrdo_abort_edit_file, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_abort_edit_file
* DESCRIPTION
*   Abort the record file if not enter file name.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_abort_edit_file(void)
{
	S32 result = 0;

#ifndef MMI_ON_WIN32
	result = MMI_FS_Delete((const UINT8 *)g_fmrdo.rec.fullName);
#endif	
	DeleteScreenIfPresent(SCR_ID_FMRDO_EDIT_FILENAME);
	if(result<0)
	{
		DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)WARNING_TONE);
	}
	else
	{
		GoBackHistory();
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_entry_edit_filename_option
* DESCRIPTION
*   Entry edit file name option.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_entry_edit_filename_option(void)
{
	U8* guiBuffer;
	U16 nStrItemList[2]; 
	U16 nNumofItem;
	
	EntryNewScreen(SCR_ID_FMRDO_EDIT_FILENAME_OPTIONS, NULL, mmi_fmrdo_entry_edit_filename_option, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_FMRDO_EDIT_FILENAME_OPTIONS);
	nNumofItem = GetNumOfChild(MENU_ID_FMRDO_EDIT_FILENAME_OPTIONS);
	GetSequenceStringIds(MENU_ID_FMRDO_EDIT_FILENAME_OPTIONS, nStrItemList);
	SetParentHandler(MENU_ID_FMRDO_EDIT_FILENAME_OPTIONS);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory15Screen(STR_GLOBAL_OPTIONS, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (U16*)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);

	RegisterInputMethodScreenCloseFunction(mmi_fmrdo_go_back_2_history);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_highlight_record_save_file
* DESCRIPTION
*   Highlight function for save file
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_highlight_record_save_file(void)
{
	SetLeftSoftkeyFunction( mmi_fmrdo_record_save_file, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

	SetKeyHandler( mmi_fmrdo_record_save_file, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_highlight_record_input_method
* DESCRIPTION
*   Highlight function for change input method
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_highlight_record_input_method(void)
{
	SetLeftSoftkeyFunction( EntryInputMethodScreen, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );

	SetKeyHandler( EntryInputMethodScreen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_record_save_file
* DESCRIPTION
*   Save the recorded FM radio file according to the newly entered name
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_record_save_file(void)
{
	S32 result = 0;
	UI_character_type fullname[MAX_FM_FULL_NAME_LEN];
	UI_character_type path[MAX_FM_PATH_LEN];
	if( fmrdo_editor_buff[0] == 0 )
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)WARNING_TONE);
		DeleteScreenIfPresent(SCR_ID_FMRDO_EDIT_FILENAME_OPTIONS);
		return;
	}

	mmi_fmrdo_util_get_storage_path(path, g_fmrdo.rec.storage);
	//kal_wsprintf(fullname, "%w%w", path, fmrdo_editor_buff);
	pfnUnicodeStrcpy((S8 *)fullname,(const S8 *)path);
	pfnUnicodeStrcat((S8 *)fullname,(const S8 *)fmrdo_editor_buff);
	mmi_fmrdo_util_append_file_ext(fullname, g_fmrdo.rec.format);	
#ifndef MMI_ON_WIN32
	result = MMI_FS_Rename((const UINT8 *)g_fmrdo.rec.fullName,(const UINT8 *)fullname);
#endif
	#if defined(__MMI_MAINLCD_220X176__)		
	mmi_fmrdo_power_on(FALSE);
	mmi_fmrdo_power_on(TRUE);
	#endif
	if((result < 0)&&( result != ERR_FS_FILE_EXIST))
	{
		DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)WARNING_TONE);
		DeleteScreenIfPresent(SCR_ID_FMRDO_EDIT_FILENAME_OPTIONS);
	}
	else
	{
		DeleteScreenIfPresent(SCR_ID_FMRDO_EDIT_FILENAME_OPTIONS);
		DeleteScreenIfPresent(SCR_ID_FMRDO_EDIT_FILENAME );
		GoBackHistory();
	}   
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_entry_record_list
* DESCRIPTION
*   Entry file list in the folder.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_entry_record_list(void)
{
	UI_character_type path[MAX_FM_PATH_LEN];
	FMGR_FILTER	filter;

        FMR_TRACE();

	/*Check if in USB mode before enter setting*/
#if defined(__USB_IN_NORMAL_MODE__)
	if(mmi_usb_is_in_mass_storage_mode())
	{
		mmi_usb_app_unavailable_popup(0);
		return;
	}
#endif

	mmi_fmrdo_util_get_storage_path(path, g_fmrdo.rec.storage);

	FMGR_FILTER_INIT(&filter);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_WAV);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_AMR);
	
	mmi_fmgr_select_path_and_enter(APP_FMRDO, FMGR_SELECT_BROWSE, filter, 
												(PS8)path, mmi_fmrdo_browse_file_callback);
   
	ClearKeyHandler( KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_browse_file_callback
* DESCRIPTION
*   Callback function for file manager after select a file from file list.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_browse_file_callback(void* fullname, int is_short)
{
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_record_error_hdlr
* DESCRIPTION
*   Handle the error case for FM radio record
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_record_error_hdlr(S32 cause)
{
	U16 str_id = 0;

#ifdef MMI_ON_HARDWARE_P
	switch(cause)
	{
		case MDI_AUDIO_SUCCESS:
			GoBackHistory();
			break;
#if defined(__DRM_SUPPORT__)
		case DRM_NOT_PERMIT_TO_APPEND:
			str_id = STR_GLOBAL_DRM_PROHIBITED;
			break;
#endif
		case MDI_AUDIO_FAIL:
		case MDI_AUDIO_ERROR:
			str_id = STR_GLOBAL_ERROR;
			break;
		case MDI_AUDIO_BAD_FORMAT:
			str_id = STR_ID_FMRDO_BAD_FORMAT;
			break;
		case MDI_AUDIO_INVALID_FORMAT:
			str_id = STR_ID_FMRDO_INVALID_FORMAT;
			break;
		case MDI_AUDIO_BUSY:
			str_id = STR_ID_FMRDO_DEVICE_BUSY;
			break;
		case MDI_AUDIO_DISC_FULL:
		case MDI_AUDIO_NO_SPACE:
			str_id = FMGR_FS_DISK_FULL_TEXT;
			break;
		case MDI_AUDIO_OPEN_FILE_FAIL:
			str_id = STR_ID_FMRDO_OPEN_FILE_ERROR;
			break;
		case MDI_AUDIO_NO_DISC:
			str_id = STR_ID_FMRDO_NO_CARD;
			break;
		case MDI_AUDIO_WRITE_PROTECTION:
			str_id = STR_ID_FMRDO_WRITE_PROTECTION_ERROR;
			break;
		case MDI_AUDIO_END_OF_FILE:
		case MDI_AUDIO_TERMINATED:
			GoBackHistory();
			break;	
		default:
			GoBackHistory();
			break;
	}
#endif

	if(str_id !=0)
	{
		DisplayPopup((PU8)GetString(str_id) ,IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
	}
}

#define MMI_FMRADIO_REC_UTIL
/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_get_storage_path
* DESCRIPTION
*   Get the storage drive and path according to the record setting
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_util_get_storage_path(UI_string_type path, U8 storage)
{
	S16 drv;
	UINT8 tmp[128];
//	UINT8 tmp1[128];
//jiashuo for jessamine ,20080514
#if 0
	if(storage == FM_STORAGE_CARD)
	{
		drv = MMI_CARD_DRV;
	}
	else
	{
		drv = MMI_PUBLIC_DRV;
	}
#else
	drv = MMI_CARD_DRV;
#endif 

	//kal_wsprintf((kal_wchar*)path, "%c:\\%w", drv, FMGR_DEFAULT_FOLDER_AUDIO);
	sprintf(tmp,"%c:%c%s",drv,'/',FMGR_DEFAULT_FOLDER_AUDIO);
	AnsiiToUnicodeString((S8*)path,(S8*)tmp);

}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_check_storage_folder
* DESCRIPTION
*   Check if the storage folder exists. if not, create it.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_util_check_storage_folder(UI_string_type path)
{
	FS_HANDLE file_handle;
	S32 result;

	file_handle = MMI_FS_Open((const UINT8 *)path, FS_OPEN_DIR | FS_READ_ONLY );
	if (file_handle < 0) 
	{
		result = MMI_FS_CreateDir((const UINT8 *)path);
	}
	else
	{
		MMI_FS_Close(file_handle);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_append_file_ext
* DESCRIPTION
*   Append the file extension according to the setting.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_util_append_file_ext(UI_string_type filename, U8 format)
{
	UINT8 tmpExt[12];
      switch(format)
      {
         case FM_FORMAT_AMR:
	 	AnsiiToUnicodeString(tmpExt,FM_FILE_EXT_AMR);
		pfnUnicodeStrcat((S8*)filename, (S8*)tmpExt);
            //pfnUnicodeStrncat((S8*)filename, (S8*)FM_FILE_EXT_AMR, MAX_FM_FILE_NAME_LEN);
            break;
#if 0            
		 case FM_FORMAT_MP3:
		 AnsiiToUnicodeString(tmpExt,FM_FILE_EXT_MP3);
		 pfnUnicodeStrcat((S8*)filename, (S8*)tmpExt);
			 //pfnUnicodeStrncat((S8*)filename, (S8*)FM_FILE_EXT_AMR, MAX_FM_FILE_NAME_LEN);
			 break;
#endif

         case FM_FORMAT_WAV:
           // pfnUnicodeStrncat((S8*)filename, (S8*)FM_FILE_EXT_WAV, MAX_FM_FILE_NAME_LEN);
	 	AnsiiToUnicodeString(tmpExt,FM_FILE_EXT_WAV);
		pfnUnicodeStrcat((S8*)filename, (S8*)tmpExt);
            break;
//#if defined( AMRWB_ENCODE ) && defined( AMRWB_DECODE )
//         case FM_FORMAT_AWB:
//            pfnUnicodeStrncat((S8*)filename, (S8*)FM_FILE_EXT_AWB, MAX_FM_FILE_NAME_LEN);
//            break;
//#endif
      }
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_remove_file_ext
* DESCRIPTION
*   Remove file extenstion for editing file name
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_util_remove_file_ext(UI_string_type filename)
{
	UI_string_type p;

	p=filename+pfnUnicodeStrlen((const S8*)filename);

	while(p>filename)
	{
		if (*p==(UI_character_type)_T_MMI'.')
		{
			*p=0;
			break;
		}
		p--;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_check_file_exist
* DESCRIPTION
*  Check if file already exists.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_fmrdo_util_check_file_exist(UI_string_type filename)
{
	FS_HANDLE fd;

	fd=MMI_FS_Open((const UINT8 *)filename,FS_READ_ONLY);
	if(fd>=0)
	{
		MMI_FS_Close(fd);
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_increase_file_name
* DESCRIPTION
*   Change the file name by increase the number.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_util_increase_file_name(UI_string_type filename)
{
	UI_string_type p;
	U16 file_count;

	p=filename+pfnUnicodeStrlen((const S8*)filename);
	while(p > filename)
	{
		if (*p==(UI_character_type)_T_MMI'.')
		{
			file_count=(*(p-2)-'0')*10+(*(p-1)-'0');
			file_count++;
			*(p-1)=file_count%10+'0';
			*(p-2)=file_count/10+'0';
			break;
		}
		p--;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_get_new_file
* DESCRIPTION
*   Get a new file name to record.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_util_get_new_file(UI_string_type fullname)
{
	UI_character_type filename[MAX_FM_FILE_NAME_LEN];
	UI_character_type path[MAX_FM_PATH_LEN];
	UINT8 tmp[256];

#ifndef MMI_ON_WIN32
	t_rtc rtc_time;
#endif

	mmi_fmrdo_util_get_storage_path(path, g_fmrdo.rec.storage);
	mmi_fmrdo_util_check_storage_folder(path);

#ifdef MMI_ON_WIN32
	kal_wsprintf((kal_wchar*)filename, "0101000000");
#else
	RTC_GetTime(&rtc_time);
	kal_wsprintf((kal_wchar*)filename, "%02d%02d%02d%02d00", rtc_time.rtc_mon, rtc_time.rtc_day, rtc_time.rtc_hour, rtc_time.rtc_min);
#endif	
	//kal_wsprintf((kal_wchar*)fullname,"%w%w", path,(const kal_wchar*)filename);
	AnsiiToUnicodeString(tmp,(S8*)filename);
	pfnUnicodeStrcpy((S8*)fullname,(const S8 *)path);
	pfnUnicodeStrcat((S8*)fullname,(const S8 *)tmp);

	mmi_fmrdo_util_append_file_ext(fullname, g_fmrdo.rec.format);
   
	while(mmi_fmrdo_util_check_file_exist(fullname))
	{

		mmi_fmrdo_util_increase_file_name(fullname);
	}

}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_get_file_name
* DESCRIPTION
*   Get file name for editing.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
UI_string_type mmi_fmrdo_util_get_file_name(UI_string_type fullname)
{
	UI_string_type p;
	p=fullname+pfnUnicodeStrlen((const S8*)fullname);

	while(p>fullname)
	{
		if (*p==(UI_character_type)_T_MMI'\\'||*p==(UI_character_type)_T_MMI'/') //chenhe,we juse '/' in jasper instead of '\'
		{
			return (p+1);
		}
		p--;
	}
	return p;
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_init_duration
* DESCRIPTION
*   Init duration time for display.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_util_init_duration(void)
{
	g_fmrdo.rec.duration = 0;
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_start_duration
* DESCRIPTION
*   Start to count the fduration time.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void Aud_get_time( kal_uint32*tmpTime );
void mmi_fmrdo_util_start_duration(void)
{
	Aud_get_time(&g_fmrdo.rec.last_duration_ticks);

    mmi_fmrdo_redraw_main_time_duration(TRUE);
	coolsand_UI_start_timer(300,mmi_fmrdo_util_update_duration);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_update_duration
* DESCRIPTION
*   Update duration time in the record screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_util_update_duration(void)
{
	U32 current_time;

	Aud_get_time( &current_time );

	g_fmrdo.rec.duration += current_time - g_fmrdo.rec.last_duration_ticks;
	g_fmrdo.rec.last_duration_ticks = current_time;

	mmi_fmrdo_redraw_main_time_duration(TRUE);
	coolsand_UI_start_timer(300,mmi_fmrdo_util_update_duration);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_util_stop_duration
* DESCRIPTION
*   Stop duration time.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_util_stop_duration(void)
{
    U32 current_time;

    Aud_get_time(&current_time);
    
    g_fmrdo.rec.duration += current_time - g_fmrdo.rec.last_duration_ticks;

	coolsand_UI_cancel_timer(mmi_fmrdo_util_update_duration);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmrdo_go_back_2_history
* DESCRIPTION
*   Go back two level screen after save.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmrdo_go_back_2_history(void) 
{
	GoBacknHistory(1);
}
#endif

void mmi_fmrdo_printf_resid(void)
{
#if 0 //for global image ID
    TRACE_EF(FMR_DEBUG, "IMG_GLOBAL_OK= %d", IMG_GLOBAL_OK);
    TRACE_EF(FMR_DEBUG, "IMG_GLOBAL_BACK= %d", IMG_GLOBAL_BACK);
    TRACE_EF(FMR_DEBUG, "IMG_STATUS= %d", IMG_STATUS);
    TRACE_EF(FMR_DEBUG, "IMG_GLOBAL_MAIN_MENU_BG= %d", IMG_GLOBAL_MAIN_MENU_BG);

#endif
#if 0
    TRACE_EF(FMR_DEBUG, "STR_ID_FMRDO_TITLE= %d", STR_ID_FMRDO_TITLE);        
    TRACE_EF(FMR_DEBUG, "STR_ID_FMRDO_CHANNEL_LIST= %d", STR_ID_FMRDO_CHANNEL_LIST);        
    TRACE_EF(FMR_DEBUG, "STR_ID_FMRDO_POWER_ON_FIRST= %d", STR_ID_FMRDO_POWER_ON_FIRST);        
    TRACE_EF(FMR_DEBUG, "STR_ID_FMRDO_PLEASE_PLUG_IN_EARPHONE= %d", STR_ID_FMRDO_PLEASE_PLUG_IN_EARPHONE);        

    TRACE_EF(FMR_DEBUG, "STR_ID_FMRDO_TITLE= %d", STR_ID_FMRDO_TITLE);  

    //menu Id
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_MAIN= %d", MENU_ID_FMRDO_MAIN);  
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_MAIN_OPTION= %d", MENU_ID_FMRDO_MAIN_OPTION);  
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_CHANNEL_LIST= %d", MENU_ID_FMRDO_CHANNEL_LIST);  
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_MANUAL_INPUT= %d", MENU_ID_FMRDO_MANUAL_INPUT);  
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_PRESET_AUTO_SEARCH= %d", MENU_ID_FMRDO_PRESET_AUTO_SEARCH);  
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_SETTINGS= %d", MENU_ID_FMRDO_SETTINGS);  
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_RECORD= %d", MENU_ID_FMRDO_RECORD);  
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_APPEND= %d", MENU_ID_FMRDO_APPEND);  
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_FILE_LIST= %d", MENU_ID_FMRDO_FILE_LIST);  
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_EDIT_FILENAME_OPTIONS= %d", MENU_ID_FMRDO_EDIT_FILENAME_OPTIONS);  
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_EDIT_FILENAME_SAVE= %d", MENU_ID_FMRDO_EDIT_FILENAME_SAVE);  
    TRACE_EF(FMR_DEBUG, "MENU_ID_FMRDO_EDIT_FILENAME_INPUT_METHOD= %d", MENU_ID_FMRDO_EDIT_FILENAME_INPUT_METHOD);  
    //TRACE_EF(FMR_DEBUG, "= %d", );  

    //image Id
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_BASE = %d", IMG_ID_FMRDO_BASE );  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_APP= %d", IMG_ID_FMRDO_APP);
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_BACKGROUND_PLAY= %d", IMG_ID_FMRDO_BACKGROUND_PLAY);  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_SKIN= %d", IMG_ID_FMRDO_SKIN);  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_CHANNEL_NAME= %d", IMG_ID_FMRDO_CHANNEL_NAME);  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_FREQUENCY= %d", IMG_ID_FMRDO_FREQUENCY);  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_SKIN_1_BACKGROUND= %d", IMG_ID_FMRDO_SKIN_1_BACKGROUND);  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_SKIN_1_VOLUME= %d", IMG_ID_FMRDO_SKIN_1_VOLUME);  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_SKIN_1_TUNER_SCALE= %d", IMG_ID_FMRDO_SKIN_1_TUNER_SCALE);  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_SKIN_1_TUNER_SCALE_INDICATOR= %d", IMG_ID_FMRDO_SKIN_1_TUNER_SCALE_INDICATOR);  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_UP= %d", IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_UP);  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_DOWN= %d", IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_DOWN);
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_DOWN= %d",IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_DOWN );  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_UP= %d", IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_UP);  
    TRACE_EF(FMR_DEBUG, "IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_DOWN= %d", IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_DOWN);  

    //TRACE_EF(FMR_DEBUG, "STR_ID_FMRDO_RECORD,= %d", STR_ID_FMRDO_RECORD,);        
    //TRACE_EF(FMR_DEBUG, "STR_ID_FMRDO_WRITE_PROTECTION_ERROR= %d", STR_ID_FMRDO_WRITE_PROTECTION_ERROR);        
    //TRACE_EF(FMR_DEBUG, "STR_ID_FMRDO_APPEND= %d", STR_ID_FMRDO_APPEND); 

#endif
}
void mmi_fmradio_clean_app(void)
{
    if(mmi_fmrdo_is_power_on()||IsScreenPresent( SCR_ID_FMRDO_MAIN)) 
    {
       mmi_fmrdo_power_on(FALSE);
       DeleteUptoScrID(IDLE_SCREEN_ID);
    }
}

#endif // __MMI_FM_RADIO__


