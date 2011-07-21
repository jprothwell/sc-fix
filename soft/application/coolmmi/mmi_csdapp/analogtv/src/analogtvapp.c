/*
 * FileName:       
 * Author:     yuanqiulin    Version: G30  Date: 2007-3-29
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
/****************************************************************************
* Include Files                                                                
*****************************************************************************/
//#ifdef __ANALOG_TV_REC__
#if 1
#include "mmi_features.h"
#if defined(__MMI_ANALOGTV__)

#include "mmi_features_video.h" /* features */
#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"
#include "globaldefs.h"

#include "lcd_if.h"                     /* LCD layer enable flag */
#include "lcd_sw_rnd.h"                 /* for LCD size */

#include "nvramtype.h"                  /* NVRAM data type */
#include "nvramprot.h"                  /* NVRAM access fucntions */
#include "nvramenum.h"                  /* NVRAM ID define */

#include "nvram_data_items.h"           /* ID to store file path */
#include "mmi_trace.h"

/* framework related headers */
#include "custmenures.h"
#include "globalconstants.h"            /* KEY ID define */
#include "globaldefs.h"                 /* Golbal Image and String ID. */
#include "gui.h"                        /* Gui functions */
#include "wgui_categories.h"
#include "historygprot.h"               /* Screen history */
#include "unicodexdcl.h"                /* Unicode transform functions */
#include "eventsgprot.h"
#include "eventsdef.h"
#include "settingdefs.h"                /* ST_NOTIFYDURATION define */
#include "settingprofile.h"             /* SUCCESS_TONE define */
#include "wgui.h"                       /* for GetDateTime() */
#include "commonscreens.h"              /* DisplayPopup() */
#include "gpioinc.h"                    /* LED */
#include "debuginitdef.h"               /* MMI_ASSERT */
#include "scrmemmgrgprot.h"             /* screen memeory manager */
#include "settingprofile.h"
#include "wgui_categories_multimedia.h"
#include "wgui_categories_popup.h"
#include "wgui_status_icons.h"
#include "wgui_softkeys.h"
#include "fat_fs.h"
#include "filemanagerdef.h"
#include "filemanagergprot.h"           /* file path */
#include "filemgr.h"
#include "mmiapi_enum.h"                /* Stop LED */
#include "mainmenudef.h"                /* Multimedia icon */
#include "profilegprots.h"              /* playRequestedTone */
#include "keybrd.h"                     /* keypad tone related interface */
#include "simdetectiongprot.h"

/* gdi, mid, camera app headers */
#include "gdi_include.h"                /* graphic library */
#include "gdi_image_jpeg.h"             /* jpeg decoder */
#include "mdi_datatype.h"
#include "mdi_camera.h"
#include "mdi_audio.h"
#include "aud_defs.h"

#include "analogtvapp.h"                  /* analogtv app data */
#include "analogtvresdefs.h"              /* resource ID def */
#include "resource_analogtv_skin.h"	/*TV ui*/
#include "atvd_m.h"

#include "gui_setting.h"
#include "wgui.h"

#include "usbdevicegprot.h"

#include "mdi_audio.h"
#include "l1audio.h"

#include "wgui_categories_inputsenum.h"

#include "vdorecstrenum.h"
#include "vdorecapp.h"
#include "fmradiodef.h"
#include "audioplayerdef.h"
#include "mdi_video.h" 
#include "kal_non_specific_general_types.h"
#include "calendardef.h"
extern void UI_push_text_clip(void);
extern void UI_reset_text_clip(void);
extern void UI_move_text_cursor(S32 x, S32 y);
extern void UI_print_text(UI_string_type text);
extern void UI_pop_text_clip(void);


extern void mmi_fmrdo_power_on( BOOL turn_on );
#if defined(__QQ_SUPPORT__)&&(!defined(__MMI_ZW_QQ__))
extern void mmi_exit_qq_application(void (*f1)(void));
extern BOOL exit_app;
#endif
void mmi_analogtv_preview_show_pannel_background(void);
void mmi_analogtv_blt_buttonarea_screen(void);
void mmi_analogtv_entry_auto_search_state(void);
void preview_playwnd_up_handle(void);
void mmi_analogtv_fullscr_play_key(void);

extern kal_bool isp_digital_zoom_flag;

static void mmi_analogtv_exit_app_screen(void);
static void mmi_analogtv_enter_state(analogtv_state_enum state);
static void mmi_analogtv_exit_current_state(void);
static void mmi_analogtv_entry_preview_state(void);
static void mmi_analogtv_exit_preview_state(void);
static void mmi_analogtv_preview_start(void);
static void mmi_analogtv_preview_stop(void);
static void mmi_analogtv_preview_set_key_hdlr(void);
static void mmi_analogtv_inc_volume(void);
static void mmi_analogtv_dec_volume(void);
static void mmi_analogtv_set_channel_pre(void);
static void mmi_analogtv_set_channel_next(void);
static void mmi_analogtv_play_channel(U16 channel_id);
static void mmi_analogtv_fullscreen_preview(void);
static void mmi_analogtv_fullscreen_preview_stop(void);
#if defined(__MMI_ANALOGTV_SNAPSHOT__)
static void snap_button_down_handle(void);
#endif

extern BOOL atvif_IsFullScreen(void);
extern void atvif_SetFullScreen(BOOL on);
extern void atvif_SetCurModule(unsigned char is_atv_app);

BOOL mmi_analogtv_channel_list_empty(void);
void mmi_analogtv_show_open_str(void);
void mmi_analogtv_cancel_preview(void);
void mmi_analogtv_show_search_str(void);

void brightness_button_down_handle(void);
void brightness_button_up_handle(void);
BOOL mmi_analogtv_refresh_signal_levels(U8 level) ;

extern BOOL HandleLowBatteryProcess(void);

static S32 g_analogtv_brightness_setting;

//const U8 LStr_wait[]="Opening";

typedef enum
{
	CHANNEL_PRE,
	CHANNEL_NEXT,
	VOL_DEC,
	VOL_INC,
	MY_CHANNEL,
	TV_BRIGHTNESS,
	TV_COLOR,
	RECORD,
	LEFT_SOFTKEY,
	RIGHT_SOFTKEY,
	PREVIEW_WND,
	TOUCH_OBJ_NONE
	
}analogtv_TouchObj;

typedef struct
{
	gdi_handle base_layer_handle;
	gdi_handle preview_layer_handle;
	gdi_handle osd_layer_handle;

	PU8 preview_layer_buf_ptr;
	PU8 osd_layer_buf_ptr;

	U16 base_layer_x;
	U16 base_layer_y;	
	U16 base_layer_width;
	U16 base_layer_height;

	U16 preview_x;
	U16 preview_y;
	U16 preview_width;
	U16 preview_height;

	U16 osd_layer_x;
	U16 osd_layer_y;
	U16 osd_layer_width;
	U16 osd_layer_height;

	U16 button_layer_x;
	U16 button_layer_y;
	U16 button_layer_width;
	U16 button_layer_height;
		
	U16 capture_image_width;
	U16 capture_image_height;

	U16 lcm_rotate;
	analogtv_rect* play_wnd;
	analogtv_static_pic* bg_pannel;
	analogtv_button* channel;
	analogtv_button* channel_pre;
	analogtv_button* channel_next;
	analogtv_button_volume* vol;
	analogtv_button* mychannel;
	analogtv_button_bright* brightness;
	analogtv_button* color_depth;
	analogtv_button* record;
	analogtv_static_pic* searchpannel;
	analogtv_search_progress* searchprogress;
	analogtv_button* lsk;
	analogtv_button* rsk;
	analogtv_TouchObj touchobj;
}analogtv_ui_struct;
#ifdef WIN32
#define NVRAM_EF_ATV_PLAYER_CHANNEL_TOTAL     30
#define NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE      26 

#define NVRAM_EF_ATV_PLAYER_SETTINGS_TOTAL	1
#define NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE	30
#endif
kal_uint8 g_ATVPlayerNVRAMBuff[NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE]={
  0x0,//0 first use ATV flag
  0x6,// 1 ATV volume 0~7
  0x0, 
  0x0,// 3 last channel
  0x0,// 4 
  0x0,// 5 
  0x0,// 6 
  0x0,// 7 
  0x0,// 8 
  0x0,// 9 
  0x0,// 10 
  0x0,// 11 
  0x0,// 12 storage
  0x04,// 13 
  0x0, 
  0x0,//15 mute
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,

};

analogtv_context_struct g_analogtv_cntx;
analogtv_ui_struct g_analogtv_UI;
kal_uint8 g_analogtv_gpt_timer;

#if defined(__MMI_MOTIONSENSOR_FOR_TV_CHANNEL_CHANGE__)  
pBOOL gMotionChangeTVChannelFlag = FALSE;
extern void mmi_motion_tv_trigger(void);
extern void EntryScrMotionChangeTVChannelMode(void);
extern void HighlightPhnMotionSetTVChannel(void);
extern void HintTVChannelMode(U16 index);
#endif

#if defined(MMI_2D_SENSOR_SUPPORT)
 extern void   csd_mmi_turnon_2d_sensor_by_backlight(void);
 extern void   csd_mmi_turnoff_2d_sensor_by_backlight(void);
#endif

extern U8 isEarphonePlugged;

void __analogtv_debug_trace(const char * fmt, ...)
{
#if defined(__ANALOGTV_DEBUG__)

	char buf[512];
	va_list arg_ptr;
	int len;

	va_start(arg_ptr, fmt);
	len = vsprintf(buf, fmt, arg_ptr);
	va_end(arg_ptr);

	buf[len++] = '\n';
	buf[len++] = '\0';
#ifdef MMI_ON_WIN32
	OutputDebugString(buf);
#else
	//kal_prompt_trace(MOD_MMI, "%s", buf);
#endif

#endif
} // end WM_DebugPrint()

/*****************************************************************************
 * FUNCTION
 *  mmi_analogtv_hilight_app
 * DESCRIPTION
 *  app menu item hilight hdlr
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_analogtv_init_app(void)
{
	
	
	/* init context */
	g_analogtv_cntx.app_state = ANALOGTV_STATE_EXIT;
	g_analogtv_cntx.app_prev_state = ANALOGTV_STATE_EXIT;
	g_analogtv_cntx.app_next_state = ANALOGTV_STATE_EXIT;
	g_analogtv_cntx.osd_rotate = MDI_CAMERA_PREVIEW_ROTATE_0;
	g_analogtv_cntx.is_rsk_pressed=FALSE;
	//can read from nvram
	g_analogtv_cntx.volume = 0;  


	//init g_analogtv_UI	
	g_analogtv_UI.play_wnd=&g_AnalogTV_Preview_Layout_skin.play_wnd;
	g_analogtv_UI.bg_pannel=&g_AnalogTV_Preview_Layout_skin.panel.bg_pannel;
	g_analogtv_UI.channel=&g_AnalogTV_Preview_Layout_skin.panel.channel;
	g_analogtv_UI.channel_pre=&g_AnalogTV_Preview_Layout_skin.panel.channel_pre;
	g_analogtv_UI.channel_next=&g_AnalogTV_Preview_Layout_skin.panel.channel_next;
	g_analogtv_UI.vol=&g_AnalogTV_Preview_Layout_skin.panel.vol;
	g_analogtv_UI.vol->level=g_analogtv_cntx.volume;	
	g_analogtv_UI.mychannel=&g_AnalogTV_Preview_Layout_skin.panel.mychannel;
	g_analogtv_UI.brightness=&g_AnalogTV_Preview_Layout_skin.panel.brightness;
	g_analogtv_UI.color_depth=&g_AnalogTV_Preview_Layout_skin.panel.color_depth;
	g_analogtv_UI.record=&g_AnalogTV_Preview_Layout_skin.panel.record;
	g_analogtv_UI.searchpannel=&g_AnalogTV_Preview_Layout_skin.searchpannel;
	g_analogtv_UI.searchprogress=&g_AnalogTV_Preview_Layout_skin.searchprogress;
	g_analogtv_UI.lsk=&g_AnalogTV_Preview_Layout_skin.lsk;
	g_analogtv_UI.rsk=&g_AnalogTV_Preview_Layout_skin.rsk;

	g_analogtv_UI.base_layer_handle = GDI_LAYER_EMPTY_HANDLE;
	g_analogtv_UI.preview_layer_handle=GDI_LAYER_EMPTY_HANDLE;
	g_analogtv_UI.osd_layer_handle= GDI_LAYER_EMPTY_HANDLE;
	g_analogtv_UI.preview_layer_buf_ptr=NULL;
	g_analogtv_UI.osd_layer_buf_ptr=NULL;
		
	g_analogtv_UI.preview_x=g_analogtv_UI.play_wnd->x;
	g_analogtv_UI.preview_y=g_analogtv_UI.play_wnd->y;
	g_analogtv_UI.preview_width=g_analogtv_UI.play_wnd->width;
	g_analogtv_UI.preview_height=g_analogtv_UI.play_wnd->height;
	
	g_analogtv_UI.button_layer_x=0;
	g_analogtv_UI.button_layer_y=g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height;
	g_analogtv_UI.button_layer_width=g_analogtv_UI.bg_pannel->width;
	g_analogtv_UI.button_layer_height=g_analogtv_UI.bg_pannel->height-g_analogtv_UI.play_wnd->height;

	g_analogtv_UI.osd_layer_x=0;
	g_analogtv_UI.osd_layer_y=0;
	g_analogtv_UI.osd_layer_width=UI_DEVICE_WIDTH;
	g_analogtv_UI.osd_layer_height=UI_DEVICE_HEIGHT;

	
	g_analogtv_UI.capture_image_width=g_analogtv_UI.preview_width;
	g_analogtv_UI.capture_image_height=g_analogtv_UI.preview_height;

	g_analogtv_UI.lcm_rotate=MDI_CAMERA_LCM_ROTATE_0;
}

void mmi_analogtv_load_setting(void)
{
	S16 ret, error_code;
	U16 i;

	g_analogtv_cntx.analogtv_setting.cur_channel.cur_index=0;
	g_analogtv_cntx.analogtv_setting.cur_channel.channel.frequency=ANALOGTV_CHANNEL_DEFAULT;	
	g_analogtv_cntx.set_state=ANALOGTV_STATE_PREVIEW;
	g_analogtv_cntx.analogtv_setting.is_mychannel=FALSE;	
	g_analogtv_cntx.analogtv_setting.yuv_brightness=NORMAL_LEVEL;
	g_analogtv_cntx.analogtv_setting.yuv_color=NORMAL_LEVEL;
	g_analogtv_cntx.analogtv_setting.filename_seq_no= 0;
	g_analogtv_cntx.searched_area_id = g_analogtv_cntx.searching_area_id = TV_AREA_CHINA;
	
	memset((void *)g_ATVPlayerNVRAMBuff, 0x0, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE);
	ret = ReadRecord( NVRAM_EF_ATV_PLAYER_SETTINGS_LID, 1, (void *)g_ATVPlayerNVRAMBuff, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE, &error_code);

	g_analogtv_UI.vol->level = g_analogtv_cntx.volume = g_analogtv_cntx.analogtv_setting.volume = g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_VOLUME_ID];
	g_analogtv_cntx.analogtv_setting.cur_channel.cur_index = g_analogtv_cntx.index = g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_LAST_CHANNEL_ID];
	g_analogtv_cntx.analogtv_setting.filename_seq_no = g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_FILENAME_SEQ_ID];
	g_analogtv_cntx.searched_area_id = g_analogtv_cntx.searching_area_id = g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_SEARCH_AREA_ID];
	/*init storage */
	g_analogtv_cntx.analogtv_setting.storage = g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_STORAGE_ID];
	if(!((g_analogtv_cntx.analogtv_setting.storage == ANALOGTV_STORAGE_PHONE)||(g_analogtv_cntx.analogtv_setting.storage == ANALOGTV_STORAGE_CARD)))
		g_analogtv_cntx.analogtv_setting.storage = ANALOGTV_STORAGE_PHONE;
	g_analogtv_cntx.analogtv_setting.total_channel = g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_TOTAL_CHANNEL_ID];
	if( g_analogtv_cntx.analogtv_setting.total_channel >0)
	{
		for(i = 0; i < NVRAM_EF_ATV_PLAYER_CHANNEL_TOTAL; i++)
		{
			ret = ReadRecord( NVRAM_EF_ATV_PLAYER_CHANNEL_LID, i + 1, (void *)&g_analogtv_cntx.channel_list[i], NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE, &error_code);
			//kal_prompt_trace(MOD_MMI, "lxy read( type=%x, EId=%x, SId=%x, CId=%x)", g_ATV_player.channel_info[i].type, g_ATV_player.channel_info[i].EId, g_ATV_player.channel_info[i].SId, g_ATV_player.channel_info[i].CId);
			if (!((error_code == NVRAM_READ_SUCCESS) && (ret == NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE)))
			{
				ret = FALSE;
				break;
			}
		}
		g_analogtv_cntx.analogtv_setting.cur_channel.channel.frequency = g_analogtv_cntx.channel_list[g_analogtv_cntx.analogtv_setting.cur_channel.cur_index].frequency;
	}
	else
		memset((void *)g_analogtv_cntx.channel_list,0,sizeof(g_analogtv_cntx.channel_list));


        
      ReadValue(NVRAM_ANALOGTV_BRIGHT_SETTING, &g_analogtv_brightness_setting, DS_BYTE, &error_code);
      if(g_analogtv_brightness_setting == 0xff )
        {
            g_analogtv_brightness_setting = 4;
      }
        g_analogtv_cntx.analogtv_setting.yuv_brightness = g_analogtv_brightness_setting;
}

BOOL analogtv_is_full_screen(void)
{
    if((g_analogtv_UI.preview_width==UI_DEVICE_WIDTH)
        &&(g_analogtv_UI.preview_height==UI_DEVICE_HEIGHT))
        return TRUE;
    else
        return FALSE;

}
static U8 preview_state=1; 
void analogtv_preview_stop(void)
{
	if (preview_state)
	{
		preview_state=0;
		mmi_trace(g_sw_CAMERA,"[rdamtv]mmi_analogtv_preview_stop");
		mmi_analogtv_preview_stop();
	}
}
void analogtv_preview_start(void)
{
    kal_prompt_trace(g_sw_CAMERA,"[rdamtv]mmi_analogtv_preview_start %d",preview_state);
	if (!preview_state)
	{
		mmi_analogtv_preview_start();
		mmi_trace(g_sw_CAMERA,"[rdamtv]mmi_analogtv_preview_start end");
		preview_state=1;
	}
}

extern void mmi_audply_stop_playing(void);
extern BOOL mmi_fmrdo_is_power_on( void );
void mmi_analogtv_entry_app(void)
{
    if(HandleLowBatteryProcess())
    {
        mmi_trace(1,"low battery in analog tv");
        return ;
    }
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

//clear audio player and fm player background
        mdi_audio_clear_all_background_handler();
#ifdef __MMI_AUDIO_PLAYER__
	mmi_audply_stop_playing();
#endif 

#if 0//def __MMI_FM_RADIO__
    if (mmi_fmrdo_is_power_on())
    {
        DisplayConfirm(
               STR_GLOBAL_YES,
               IMG_GLOBAL_YES,
               STR_GLOBAL_NO,
               IMG_GLOBAL_NO,
               get_string(STR_ID_AUDPLY_STOP_FMR),
               IMG_GLOBAL_QUESTION,
               WARNING_TONE);

        SetLeftSoftkeyFunction(mmi_analogtv_entry_preview_screen, KEY_EVENT_UP);
        SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    }
    else
#endif
	mmi_analogtv_entry_preview_screen();
}
#ifdef MMI_ON_HARDWARE_P
extern void sxr_Sleep (u32 Period);
#else
void sxr_Sleep (u32 Period)
{
}
#endif

void mmi_analogtv_entry_preview_screen(void)
{

	EntryNewScreen(SCR_ID_ANALOGTV_APP, mmi_analogtv_exit_app_screen, mmi_analogtv_entry_preview_screen, NULL);
	DeleteScreenIfPresent(SCR_ID_ANALOGTV_APP);
	DeleteScreenIfPresent(SCR_ID_ANALOGTV_OPTION);

	ClearAllKeyHandler();

#if defined(MMI_2D_SENSOR_SUPPORT)
        //    csd_mmi_turnoff_2d_sensor_by_backlight();
#endif

#if defined(MMI_ON_HARDWARE_P) 
    mmi_trace(1,"  mmi_analogtv_entry_preview_screen " );

	atvif_SetCurModule(1);
#endif

	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

	/* entry full screen app */
	entry_full_screen();
    
        
#ifdef __MMI_FM_RADIO__
    if (mmi_fmrdo_is_power_on())
           mmi_fmrdo_power_on(FALSE);
#endif 

	/* stop MMI sleep */

	TurnOnBacklight(0);
#if defined(MMI_ON_HARDWARE_P)

	/* force all playing keypad tone off */
//k	AudioStopReq(GetCurKeypadTone());

	/* disable key pad tone */
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);

      // mdi_audio_set_mute(MDI_VOLUME_MEDIA, TRUE);

	//mdi_audio_stop_all();
#endif
	
	/* disalbe align timer  */
	UI_disable_alignment_timers();

	/* enable multi-layer */
	gdi_layer_multi_layer_enable();      
    
	gdi_layer_clear(GDI_COLOR_BLACK);

	gdi_layer_get_base_handle(&g_analogtv_UI.base_layer_handle);
		
	//preview layer
	if(g_analogtv_cntx.set_state==ANALOGTV_STATE_FULL_SCR_PLAY)
	{
		g_analogtv_UI.preview_x=0;
		g_analogtv_UI.preview_y=0;
		g_analogtv_UI.preview_width=UI_DEVICE_WIDTH;
		g_analogtv_UI.preview_height=UI_DEVICE_HEIGHT;
#ifndef LCDSIZE_220_176
		atvif_SetFullScreen(TRUE);	
#endif
	}
	gdi_layer_create(
	g_analogtv_UI.preview_x,
	g_analogtv_UI.preview_y,
	g_analogtv_UI.preview_width,
	g_analogtv_UI.preview_height,
	&g_analogtv_UI.preview_layer_handle);
	gdi_layer_push_and_set_active(g_analogtv_UI.preview_layer_handle);

	gdi_layer_clear(GDI_COLOR_BLACK);
	gdi_layer_pop_and_restore_active();
	
	//osd layer
	g_analogtv_UI.osd_layer_buf_ptr=mmi_frm_scrmem_alloc(ANALOGTV_OSD_BUFFER_SIZE);
	MMI_ASSERT(g_analogtv_UI.osd_layer_buf_ptr);
	gdi_layer_create_double_using_outside_memory(
	g_analogtv_UI.osd_layer_x, 
	g_analogtv_UI.osd_layer_y, 
	g_analogtv_UI.osd_layer_width, 
	g_analogtv_UI.osd_layer_height, 
	&g_analogtv_UI.osd_layer_handle, 
	(PU8)g_analogtv_UI.osd_layer_buf_ptr, 
	(S32)ANALOGTV_OSD_BUFFER_SIZE);        /* this buffer is double buffer size */

	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);

	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	gdi_layer_toggle_double();
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();

	gdi_layer_set_blt_layer(
	g_analogtv_UI.base_layer_handle,
	g_analogtv_UI.preview_layer_handle,    	
	g_analogtv_UI.osd_layer_handle,    	
	0);

	mmi_analogtv_set_search_area();
 
#if defined(MMI_ON_HARDWARE_P)	
	mdi_camera_power_on();
#endif
        sxr_Sleep(100);
        mdi_audio_set_mute(MDI_VOLUME_MEDIA, FALSE);
        ANALOGTV_ENTER_STATE(g_analogtv_cntx.set_state);
        #if defined(ANALOG_TV_SUPPORT) && defined(__FF_AUDIO_SET__)
        GpioSpeakerModeStart();
        #endif

}

 
#if defined(__MMI_ANALOGTV_SHOW_CHANNEL_NUMBER__)
void mmi_analogtv_draw_image(S32 channel_index)
{
	S32 x1, y1, x2, y2; 
	S32 width,height;
	S32 num1, num2;
	
	x1 = 1;
	y1 = 1;
	
	num1 = channel_index /10;
	num2 = channel_index % 10;

	gui_measure_image((PU8)GetImage(IMG_ID_ANALOGTV_CHANNEL_NUM0),&width,&height);
	
	x2 = x1 + 2*width +1;
	y2 = y1 + height;

	
	if(num1 > 0)
	{
		gdi_image_draw_id(x1, y1,(IMG_ID_ANALOGTV_CHANNEL_NUM0 + num1));
		x1 += width + 1;
		gdi_image_draw_id(x1, y1,(IMG_ID_ANALOGTV_CHANNEL_NUM0 + num2));
	}
	else
	{
		gdi_draw_solid_rect(x1, y1, x2, y2, GDI_COLOR_BLACK);
		gdi_image_draw_id(x1, y1,(IMG_ID_ANALOGTV_CHANNEL_NUM0 + num2));			
	}
}
void mmi_draw_next_channel_number(void)
{
	U16 channel_index,index, total_channel;
	
	index = g_analogtv_cntx.analogtv_setting.cur_channel.cur_index + 1;
	total_channel = g_analogtv_cntx.analogtv_setting.total_channel;
	if(!total_channel)
	{
		channel_index = total_channel;
		return;
	}

	if(index == total_channel)
	{
		channel_index = 1;
	}
	else
	{
		channel_index = index +1;
	}
	mmi_analogtv_draw_image(channel_index);
}

void mmi_draw_pre_channel_number(void)
{
	U16 channel_index,index, total_channel;
	
	index = g_analogtv_cntx.analogtv_setting.cur_channel.cur_index + 1;
	total_channel = g_analogtv_cntx.analogtv_setting.total_channel;
	if(!total_channel)
	{
		channel_index = total_channel;
		return;
	}
	
	if(index == 1)
	{
		channel_index = total_channel;
	}
	else
	{
		channel_index = index - 1;
	}
	mmi_analogtv_draw_image(channel_index);
}
void mmi_analogtv_show_channel_number(void)
{
	U16 channel_index, total_channel;
	S32 width,height;
	S32 num1,num2;
	
	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);

	channel_index = g_analogtv_cntx.analogtv_setting.cur_channel.cur_index + 1;
	total_channel = g_analogtv_cntx.analogtv_setting.total_channel;
	
	if(!total_channel)
	{
		channel_index = total_channel;
	}
	
	mmi_analogtv_draw_image(channel_index);

	gdi_layer_pop_and_restore_active();
}

void mmi_analogtv_show_channel_number_TV(void)
{
	gdi_layer_lock_frame_buffer();
	mmi_analogtv_show_channel_number();
	gdi_layer_unlock_frame_buffer();
	mmi_analogtv_blt_screen();
}
#endif
void autosearch_req_goback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GoBackToHistory(SCR_ID_ANALOGTV_OPTION);
}

void mmi_analogtv_entry_Search(void)
{

	DisplayConfirm(STR_GLOBAL_YES,
	    IMG_GLOBAL_YES,
	    STR_GLOBAL_NO,
	    IMG_GLOBAL_NO,
	    get_string(STR_MENU_ID_ANALOGTV_CHANNEL_AUTO_SEARCH),
	    IMG_GLOBAL_QUESTION,
	    WARNING_TONE);
/*
	ShowCategory164Screen(
	    STR_GLOBAL_YES,
	    IMG_GLOBAL_YES,
	    STR_GLOBAL_NO,
	    IMG_GLOBAL_NO,
	    STR_MENU_ID_ANALOGTV_CHANNEL_AUTO_SEARCH,
	    IMG_GLOBAL_QUESTION,
	    NULL);

	playRequestedTone(WARNING_TONE);
*/
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Search_Auto, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

	
#if 0
	U16 nStrItemList[MAX_SUB_MENUS];
	U16 nNumofItem;
	U8 *guiBuffer;

	U16 SettingsSubmenuImageList[MAX_SUB_MENUS];


	EntryNewScreen(SCR_ID_ANALOGTV_OPTION_SEARCH, NULL, mmi_analogtv_entry_Search, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_OPTION_SEARCH);
	nNumofItem = GetNumOfChild(MENU_ID_ANALOGTV_OPTION_CHANNEL_SEARCH);
	GetSequenceStringIds(MENU_ID_ANALOGTV_OPTION_CHANNEL_SEARCH, nStrItemList);
	GetSequenceImageIds(MENU_ID_ANALOGTV_OPTION_CHANNEL_SEARCH, SettingsSubmenuImageList);
	SetParentHandler(MENU_ID_ANALOGTV_OPTION_CHANNEL_SEARCH);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	wgui_enable_mini_tab_bar(MENU_ID_ANALOGTV_OPTION_CHANNEL_SEARCH);
#endif 
	ShowCategory15Screen(
	STR_MENU_ID_ANALOGTV_OPTION_CHANNEL_SEARCH,
	0,
	STR_GLOBAL_OK,
	IMG_GLOBAL_OK,
	STR_GLOBAL_BACK,
	IMG_GLOBAL_BACK,
	nNumofItem,
	nStrItemList,
#ifndef __MMI_SUBMENU_ALL_NUMBER_ICON__
					  SettingsSubmenuImageList,
#else						
					  (U16*)gIndexIconsImageList,
#endif 
	LIST_MENU,
	0,
	guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
#endif	
}

void mmi_analogtv_entry_Search_Auto(void)
{
        g_analogtv_cntx.set_state=ANALOGTV_STATE_PREVIEW_CHANNEL_AUTO_SEARCH;
        if(IsScreenPresent(SCR_ID_ANALOGTV_APP ))
        {
            GoBackToHistory(SCR_ID_ANALOGTV_APP);	
        }
        else
        {
            mmi_analogtv_entry_preview_screen();
        }
}

void mmi_analogtv_entry_Search_Manual(void)
{
	
}

void mmi_analogtv_entry_Select(void)
{
	U16 nStrItemList[MAX_SUB_MENUS];
	U16 nNumofItem;
	U8 *guiBuffer;

	U16 SettingsSubmenuImageList[MAX_SUB_MENUS];


	EntryNewScreen(SCR_ID_ANALOGTV_OPTION_CHANNEL_SELECT, NULL, mmi_analogtv_entry_Select, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_OPTION_CHANNEL_SELECT);
	nNumofItem = GetNumOfChild(MENU_ID_ANALOGTV_OPTION_CHANNEL_SELECT);
	GetSequenceStringIds(MENU_ID_ANALOGTV_OPTION_CHANNEL_SELECT, nStrItemList);
	GetSequenceImageIds(MENU_ID_ANALOGTV_OPTION_CHANNEL_SELECT, SettingsSubmenuImageList);
	SetParentHandler(MENU_ID_ANALOGTV_OPTION_CHANNEL_SELECT);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	wgui_enable_mini_tab_bar(MENU_ID_ANALOGTV_OPTION_CHANNEL_SELECT);
#endif 
	ShowCategory15Screen(
	STR_MENU_ID_ANALOGTV_OPTION_CHANNEL_SELECT,
	0,
	STR_GLOBAL_OK,
	IMG_GLOBAL_OK,
	STR_GLOBAL_BACK,
	IMG_GLOBAL_BACK,
	nNumofItem,
	nStrItemList,
#ifndef __MMI_SUBMENU_ALL_NUMBER_ICON__
					  SettingsSubmenuImageList,
#else						
					  (U16*)gIndexIconsImageList,
#endif 
	LIST_MENU,
	0,
	guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);	
}

void mmi_analogtv_highlight_channel_list(S32 index)
{
	g_analogtv_cntx.index=index;
	g_analogtv_cntx.channel=g_analogtv_cntx.channel_list[index];
	if(g_analogtv_cntx.channel_list[index].Present!= 0)
	{
		ChangeLeftSoftkey( STR_GLOBAL_OPTIONS, IMG_GLOBAL_OK );
		SetLeftSoftkeyFunction( mmi_analogtv_entry_channel_list_option,KEY_EVENT_UP);
		SetKeyHandler( mmi_analogtv_entry_channel_list_option, KEY_ENTER, KEY_EVENT_UP);
	}
	else
	{
		ChangeLeftSoftkey( 0, 0 );
		ClearKeyHandler( KEY_ENTER,  KEY_EVENT_UP);
	}
}

void mmi_analogtv_entry_Select_List(void)
{
	U8* guiBuffer;
	S32 i ,j;
	U8 highlited;
	U8* channel_list_name[NVRAM_EF_ATV_PLAYER_CHANNEL_TOTAL];
	
	EntryNewScreen(SCR_ID_ANALOGTV_CHANNEL_LIST, NULL, mmi_analogtv_entry_Select_List, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_CHANNEL_LIST);


	highlited = g_analogtv_cntx.analogtv_setting.cur_channel.cur_index;
	/* construct displayed string and icon */
	for(i=0, j=0; i<NVRAM_EF_ATV_PLAYER_CHANNEL_TOTAL; i++) 
	{
		if(g_analogtv_cntx.channel_list[i].Present ==FALSE)
		{
			memset((void *)g_analogtv_cntx.channel_list[i].name,0,sizeof(g_analogtv_cntx.channel_list[i].name));
			pfnUnicodeStrcpy((S8*)g_analogtv_cntx.channel_list[i].name,(S8*)GetString(STR_GLOBAL_EMPTY_LIST));
		}
		else
		{
			j++;
		}
		channel_list_name[i]=(U8*)g_analogtv_cntx.channel_list[i].name;
	}
	g_analogtv_cntx.index=0;
	RegisterHighlightHandler(mmi_analogtv_highlight_channel_list);
	
	ShowCategory53Screen(STR_MENU_ID_ANALOGTV_CHANNEL_LIST,0,
			STR_GLOBAL_EDIT, IMG_GLOBAL_OK,
			STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			NVRAM_EF_ATV_PLAYER_CHANNEL_TOTAL, channel_list_name,
		//#ifndef __MMI_SUBMENU_ALL_NUMBER_ICON__ 
			//NULL, 
		//#else						
			(U16*)gIndexIconsImageList,
		//#endif 
			NULL, 0, highlited, guiBuffer);

	//SetLeftSoftkeyFunction(mmi_analogtv_entry_channel_list_option,KEY_EVENT_UP);	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
	
}

void mmi_analogtv_highlight_user_channel_list(S32 index)
{
	g_analogtv_cntx.index=index;
	g_analogtv_cntx.channel=g_analogtv_cntx.analogtv_setting.mychannel[index];
	
}

void mmi_analogtv_entry_user_channel_list_option(void)
{
	U16 nStrItemList[MAX_SUB_MENUS];
	U16 nNumofItem;
	U8 *guiBuffer;

	U16 SettingsSubmenuImageList[MAX_SUB_MENUS];


	EntryNewScreen(SCR_ID_ANALOGTV_USER_CHANNEL_LIST_OPTION, NULL, mmi_analogtv_entry_user_channel_list_option, NULL);

	guiBuffer = GetCurrGuiBuffer(MENU_ID_ANALOGTV_CHANNEL_USER_DEFINED_OPTION);
	nNumofItem = GetNumOfChild(MENU_ID_ANALOGTV_CHANNEL_USER_DEFINED_OPTION);
	GetSequenceStringIds(MENU_ID_ANALOGTV_CHANNEL_USER_DEFINED_OPTION, nStrItemList);
	GetSequenceImageIds(MENU_ID_ANALOGTV_CHANNEL_USER_DEFINED_OPTION, SettingsSubmenuImageList);
	SetParentHandler(MENU_ID_ANALOGTV_CHANNEL_USER_DEFINED_OPTION);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	wgui_enable_mini_tab_bar(MENU_ID_ANALOGTV_CHANNEL_USER_DEFINED_OPTION);
#endif 
	ShowCategory15Screen(
	STR_GLOBAL_OPTIONS,//STR_MENU_ID_ANALOGTV_CHANNEL_LIST_OPTION,
	0,
	STR_GLOBAL_OK,
	IMG_GLOBAL_OK,
	STR_GLOBAL_BACK,
	IMG_GLOBAL_BACK,
	nNumofItem,
	nStrItemList,
#ifndef __MMI_SUBMENU_ALL_NUMBER_ICON__
					  SettingsSubmenuImageList,
#else						
					  (U16*)gIndexIconsImageList,
#endif 
	LIST_MENU,
	0,
	guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void mmi_analogtv_entry_Select_User_Channel(void)
{
	U8* guiBuffer;
	S32 i;
	U8 highlited=0;
	U8* channel_list_name[MAX_USER_CHANNEL_NUMBER];
	
	EntryNewScreen(SCR_ID_ANALOGTV_USER_CHANNEL_LIST, NULL, mmi_analogtv_entry_Select_User_Channel, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_USER_CHANNEL_LIST);

	if(guiBuffer == NULL)
		highlited= 0;

	/* construct displayed string and icon */
	for(i=0; i<MAX_USER_CHANNEL_NUMBER; i++) 
	{
		if(g_analogtv_cntx.analogtv_setting.mychannel[i].Present!=TRUE)
		{
			memset((void *)g_analogtv_cntx.analogtv_setting.mychannel[i].name,0,sizeof(g_analogtv_cntx.analogtv_setting.mychannel[i].name));
			pfnUnicodeStrcpy((S8*)g_analogtv_cntx.analogtv_setting.mychannel[i].name,(S8*)GetString(STR_GLOBAL_EMPTY_LIST));
		}
		channel_list_name[i]=(U8*)g_analogtv_cntx.analogtv_setting.mychannel[i].name;
	}
	g_analogtv_cntx.index=0;
	RegisterHighlightHandler(mmi_analogtv_highlight_user_channel_list);
	
	ShowCategory53Screen(STR_MENU_ID_ANALOGTV_CHANNEL_USER_DEFINED,0,
			STR_GLOBAL_EDIT, IMG_GLOBAL_OK,
			STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			MAX_USER_CHANNEL_NUMBER, channel_list_name, NULL, 
			NULL, 0, highlited, guiBuffer);

	SetLeftSoftkeyFunction(mmi_analogtv_entry_user_channel_list_option,KEY_EVENT_UP);	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
	
}

extern int current_chn;
void mmi_analogtv_entry_Select_List_Play(void)
{
	int index=0;
	g_analogtv_cntx.analogtv_setting.is_mychannel=FALSE;
	index=g_analogtv_cntx.analogtv_setting.cur_channel.cur_index=g_analogtv_cntx.index;
	
	//current_chn=g_analogtv_cntx.channel_list[index].frequency; 
	
	g_analogtv_cntx.analogtv_setting.cur_channel.channel=g_analogtv_cntx.channel_list[index];
	
	//kal_prompt_trace(MOD_MED,"mmi_analogtv_entry_Select_List_Play@@@ ch_temp=%d,test=%d",current_chn,g_analogtv_cntx.analogtv_setting.cur_channel.channel.frequency);

	GoBackToHistory(SCR_ID_ANALOGTV_APP);
}

void channel_list_rename_confirm(void)
{
	int index=0;
	S16 ret, error_code;
	
	index=g_analogtv_cntx.index;
	memset((void *)g_analogtv_cntx.channel_list[index].name,0,sizeof(g_analogtv_cntx.channel_list[index].name));
	pfnUnicodeStrcpy((S8 *)g_analogtv_cntx.channel_list[index].name,(S8 *)g_analogtv_cntx.channel.name);
	if(index < g_analogtv_cntx.analogtv_setting.total_channel  && index >= 0)
	{
		ret = WriteRecord( NVRAM_EF_ATV_PLAYER_CHANNEL_LID, index + 1, (void *)&g_analogtv_cntx.channel_list[index], NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE, &error_code);
	}
	GoBackHistory();
}

extern void GBSetInputboxLSKFunction(FuncPtr f);

void mmi_analogtv_entry_Select_List_Rename(void)
{
	U8* guiBuffer;

	EntryNewScreen(SCR_ID_ANALOGTV_CHANNEL_LIST_OPTION_RENAME, NULL, mmi_analogtv_entry_Select_List_Rename, NULL);
	
	SetParentHandler(0);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_CHANNEL_LIST_OPTION_RENAME);

	ShowCategory5Screen(STR_MENU_ID_ANALOGTV_CHANNEL_LIST_RENAME, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, //  |INPUT_TYPE_USE_ENCODING_BASED_LENGTH
						  (U8 *)g_analogtv_cntx.channel.name,
						  MAX_CH_NAME_LEN, guiBuffer);
#ifdef __MMI_GB__
	GBSetInputboxLSKFunction(channel_list_rename_confirm);
#endif

	SetLeftSoftkeyFunction(channel_list_rename_confirm,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	DeleteScreenIfPresent(SCR_ID_ANALOGTV_CHANNEL_LIST_OPTION);
}

void mmi_analogtv_entry_Select_List_Add(void)
{
	int i=0;
	int j=0;
	int index=0;
	for(i=0;i<MAX_USER_CHANNEL_NUMBER;i++)
	{
		if(g_analogtv_cntx.analogtv_setting.mychannel[i].Present)
			j++;
	}
	if(j == MAX_USER_CHANNEL_NUMBER)
	{
		j = 0;
	}
	index=g_analogtv_cntx.index;
	g_analogtv_cntx.analogtv_setting.mychannel[j]=g_analogtv_cntx.channel_list[index];
	g_analogtv_cntx.analogtv_setting.total_mychannel=j+1;
	GoBackHistory();
}

void mmi_analogtv_entry_Select_User_Play(void)
{
	int index=0;
	g_analogtv_cntx.analogtv_setting.is_mychannel=FALSE;
	index=g_analogtv_cntx.index;
	g_analogtv_cntx.analogtv_setting.cur_mychannel.cur_index=index;
	g_analogtv_cntx.analogtv_setting.cur_mychannel.channel=g_analogtv_cntx.analogtv_setting.mychannel[index];
	GoBackToHistory(SCR_ID_ANALOGTV_APP);
}

void user_channel_list_rename_confirm(void)
{
	int index=0;
	index=g_analogtv_cntx.index;
	memset((void *)g_analogtv_cntx.analogtv_setting.mychannel[index].name,0,sizeof(g_analogtv_cntx.analogtv_setting.mychannel[index].name));
	pfnUnicodeStrcpy((S8 *)g_analogtv_cntx.analogtv_setting.mychannel[index].name,(S8 *)g_analogtv_cntx.channel.name);
	GoBackHistory();
}

void mmi_analogtv_entry_Select_User_Rename(void)
{
	U8* guiBuffer;

	EntryNewScreen(SCR_ID_ANALOGTV_USER_CHANNEL_LIST_OPTION_RENAME, NULL, mmi_analogtv_entry_Select_User_Rename, NULL);
	
	SetParentHandler(0);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_USER_CHANNEL_LIST_OPTION_RENAME);

	ShowCategory5Screen(STR_MENU_ID_ANALOGTV_CHANNEL_USER_DEFINED_RENAME, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, //  |INPUT_TYPE_USE_ENCODING_BASED_LENGTH
						  (U8 *)g_analogtv_cntx.channel.name,
						  MAX_CH_NAME_LEN, guiBuffer);

	SetLeftSoftkeyFunction(user_channel_list_rename_confirm,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

void mmi_analogtv_entry_Add(void)
{
	int i=0;
	int j=0;
	for(i=0;i<MAX_USER_CHANNEL_NUMBER;i++)
	{
		if(g_analogtv_cntx.analogtv_setting.mychannel[i].Present)
			j++;
	}
	if(j==MAX_USER_CHANNEL_NUMBER)
	{
		j=0;
	}
	g_analogtv_cntx.analogtv_setting.mychannel[j]=g_analogtv_cntx.analogtv_setting.cur_channel.channel;
	g_analogtv_cntx.analogtv_setting.total_mychannel=j+1;
	GoBackHistory();
}
void mmi_analogtv_delete_all_channel(void)
{
	S16  error_code;

	mmi_analogtv_reset_all_nvram_list();
	g_analogtv_cntx.analogtv_setting.total_channel = 0;
	g_analogtv_cntx.analogtv_setting.cur_channel.cur_index =g_analogtv_cntx.index=0;
	g_analogtv_cntx.analogtv_setting.cur_channel.channel.frequency=ANALOGTV_CHANNEL_DEFAULT;	
	
	g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_TOTAL_CHANNEL_ID] = 0x0;
	WriteRecord( NVRAM_EF_ATV_PLAYER_SETTINGS_LID, 1, (void *)g_ATVPlayerNVRAMBuff, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE, &error_code);

	DisplayPopup( (PU8)GetString( STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0, 1000, (U8)SUCCESS_TONE);


        if(IsScreenPresent(SCR_ID_ANALOGTV_APP )) 
        {
            DeleteUptoScrID( SCR_ID_ANALOGTV_APP);
    	    g_analogtv_cntx.set_state=ANALOGTV_STATE_PREVIEW_CHANNEL_AUTO_SEARCH;
        }
        else
        {
            DisplayIdleScreen();
        }
//mmi_analogtv_entry_preview_screen();       
    //GoBackHistory();
}

void mmi_analogtv_delete_all_channel_confirm( void )
{
	DisplayConfirm(
	    STR_GLOBAL_YES,
	    IMG_GLOBAL_YES,
	    STR_GLOBAL_NO,
	    IMG_GLOBAL_NO,
	    get_string(STR_GLOBAL_DELETE_ALL),
	    IMG_GLOBAL_QUESTION,
	    WARNING_TONE);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_analogtv_delete_all_channel, KEY_EVENT_UP); 
}

void mmi_analogtv_save_setting(void)
{

	S16 error_code;

	g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_STORAGE_ID] = g_analogtv_cntx.analogtv_setting.storage;
	g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_FILENAME_SEQ_ID] = g_analogtv_cntx.analogtv_setting.filename_seq_no;

	g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_VOLUME_ID] = g_analogtv_cntx.volume;
	g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_LAST_CHANNEL_ID] = g_analogtv_cntx.analogtv_setting.cur_channel.cur_index;

	WriteRecord( NVRAM_EF_ATV_PLAYER_SETTINGS_LID, 1, (void *)g_ATVPlayerNVRAMBuff, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE, &error_code);
}


void mmi_analogtv_highlight_search_area_list(S32 index)
{
	g_analogtv_cntx.searching_area_id =index;

	//kal_prompt_trace(MOD_MED,"g_analogtv_cntx.searching_area_id=%d",g_analogtv_cntx.searching_area_id);

}

/*****************************************************************************
 * DAVID ADD FOR BUG5555
 * FUNCTION
 *  mmi_analogtv_entry_delete_channel_confirm
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_analogtv_entry_delete_channel_confirm( void )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(
        SCR_ID_ANALOGTV_DELETE_CHANNEL_CONFIRM,
        NULL,
        mmi_analogtv_entry_delete_channel_confirm,
        NULL);

    ShowCategory164Screen(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        STR_GLOBAL_YES,
        IMG_GLOBAL_QUESTION,
        NULL);

    playRequestedTone(WARNING_TONE);

    SetLeftSoftkeyFunction(mmi_analogtv_entry_Select_List_Delete, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void mmi_analogtv_entry_Select_List_Delete(void) 
{
	int index=0 ,i;
	S16 error_code; 
	BOOL	Present;	
	U8	name[(MAX_CH_NAME_LEN+1)*ENCODING_LENGTH];							// User of a Data Account
	U16	frequency;
	U16 total_channel;
	
	index=g_analogtv_cntx.index;
	total_channel = g_analogtv_cntx.analogtv_setting.total_channel;
	
	g_analogtv_cntx.channel_list[index].Present = FALSE;
	g_analogtv_cntx.channel_list[index].frequency= 0;
	memset((void *)g_analogtv_cntx.channel_list[index].name,0,sizeof(g_analogtv_cntx.channel_list[index].name));
	

	for(i = index; i<g_analogtv_cntx.analogtv_setting.total_channel; i++)
	{
		memset((void *)name,0,sizeof(name));
		memset((void *)g_analogtv_cntx.channel_list[i].name,0,sizeof(g_analogtv_cntx.channel_list[i].name));
		
		Present = g_analogtv_cntx.channel_list[i+1].Present;
		frequency = g_analogtv_cntx.channel_list[i+1].frequency;
		pfnUnicodeStrcpy((S8*)name, (S8*)g_analogtv_cntx.channel_list[i+1].name);

		g_analogtv_cntx.channel_list[i].Present = Present;
		g_analogtv_cntx.channel_list[i].frequency = frequency;
		pfnUnicodeStrcpy((S8*)g_analogtv_cntx.channel_list[i].name, (S8*)name);
		WriteRecord(NVRAM_EF_ATV_PLAYER_CHANNEL_LID, i + 1, (void *)&g_analogtv_cntx.channel_list[i], NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE, &error_code);
	}
	g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_TOTAL_CHANNEL_ID] = --g_analogtv_cntx.analogtv_setting.total_channel;
	g_analogtv_cntx.analogtv_setting.cur_channel.cur_index =g_analogtv_cntx.index=0;
	
	if(g_analogtv_cntx.analogtv_setting.total_channel == 0)
	{	
		g_analogtv_cntx.analogtv_setting.cur_channel.channel.frequency=ANALOGTV_CHANNEL_DEFAULT;
		g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_TOTAL_CHANNEL_ID] = 0x0;
	}
	else
	{
		g_analogtv_cntx.analogtv_setting.cur_channel.channel.frequency = g_analogtv_cntx.channel_list[g_analogtv_cntx.analogtv_setting.cur_channel.cur_index].frequency;
	}

	/*
	if(index < NVRAM_EF_ATV_PLAYER_CHANNEL_TOTAL  && index >= 0)
	{
		WriteRecord(NVRAM_EF_ATV_PLAYER_CHANNEL_LID, index + 1, (void *)&g_analogtv_cntx.channel_list[index], NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE, &error_code);
	}*/

	WriteRecord( NVRAM_EF_ATV_PLAYER_SETTINGS_LID, 1, (void *)g_ATVPlayerNVRAMBuff, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE, &error_code);

	//GoBackHistory();
	DisplayPopup( (PU8)GetString( STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 0, 1000, (U8)SUCCESS_TONE);

	DeleteUptoScrID(SCR_ID_ANALOGTV_CHANNEL_LIST);
	
}

void Hilight_Channel_Select_List_Delete(void) 
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
#if 0
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Select_List_Delete, KEY_EVENT_UP);
#else
	SetLeftSoftkeyFunction(mmi_analogtv_entry_delete_channel_confirm, KEY_EVENT_UP);
#endif
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_delete_channel_confirm, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
}
void mmi_analogtv_set_search_area(void)
{
    atvd_SetTVArea(g_analogtv_cntx.searched_area_id);
	g_analogtv_cntx.current_area_chn_count = atvd_GetTotalChannels();
}

void mmi_analogtv_search_aera_done(void)
{
	S16 error_code;

	if(g_analogtv_cntx.searched_area_id != g_analogtv_cntx.searching_area_id)
	{
		g_analogtv_cntx.searched_area_id = g_analogtv_cntx.searching_area_id;
		g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_SEARCH_AREA_ID] = g_analogtv_cntx.searched_area_id;
		WriteRecord( NVRAM_EF_ATV_PLAYER_SETTINGS_LID, 1, (void *)g_ATVPlayerNVRAMBuff, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE, &error_code);
        //mmi_analogtv_set_search_area();
		g_analogtv_cntx.set_state=ANALOGTV_STATE_PREVIEW_CHANNEL_AUTO_SEARCH;
		
	}
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	GoBacknHistory(1);
}
//For TV169
void mmi_analogtv_entry_Search_Area(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *pGuiBuffer;
    U8 index;
    U8   hilight=0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    hilight = g_analogtv_cntx.searching_area_id = g_analogtv_cntx.searched_area_id;
	
    EntryNewScreen(SCR_ID_ANALOGTV_SEARCH_AREA, NULL, mmi_analogtv_entry_Search_Area, NULL);

    pGuiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_SEARCH_AREA);

	for(index=0; index<(STR_MENU_ID_ANALOGTV_OPTION_MAXIMUM_AREA - STR_MENU_ID_ANALOGTV_OPTION_CHINA_AREA); index++)
	{
		subMenuDataPtrs[index] = (PU8) GetString(STR_MENU_ID_ANALOGTV_OPTION_CHINA_AREA + index);
	}

    RegisterHighlightHandler(mmi_analogtv_highlight_search_area_list);

    ShowCategory36Screen(
        STR_MENU_ID_ANALOGTV_OPTION_SEARCH_AREA,
        0,
        STR_GLOBAL_OK,
        0,
        STR_GLOBAL_BACK,
        0,
        index,
        (U8 **) subMenuDataPtrs,
        hilight,
        pGuiBuffer);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(mmi_analogtv_search_aera_done, KEY_EVENT_UP);
    SetKeyHandler(0, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
}

void mmi_analogtv_entry_Effect(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer,hilight=0;       /* Buffer holding history data */
		

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_ID_ANALOGTV_SETTING_EFFECT, NULL, mmi_analogtv_entry_Effect, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_SETTING_EFFECT);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_ID_ANALOGTV_OPTION_EFFECT);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_ID_ANALOGTV_OPTION_EFFECT);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_ID_ANALOGTV_OPTION_EFFECT,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_ID_ANALOGTV_OPTION_EFFECT);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* 8 Display Category Screen */
	ShowCategory11Screen(STR_MENU_ID_ANALOGTV_SETTING_STORAGE, 0,
						 STR_GLOBAL_OK, 0,
						 STR_GLOBAL_BACK, 0,
						 nNumofItem, nStrItemList,hilight,guiBuffer);
    

}



void mmi_analogtv_effect_bright_setting_set(void)
{
        
	S16 error;
        g_analogtv_cntx.analogtv_setting.yuv_brightness = g_analogtv_brightness_setting;
        
      WriteValue(NVRAM_ANALOGTV_BRIGHT_SETTING, &g_analogtv_brightness_setting, DS_BYTE, &error);
        
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);

	DeleteNHistory(1);	
}

void mmi_analogtv_effect_bright_highlight_hdlr(S32 index)
{
    
    ClearKeyHandler(KEY_UP_ARROW,  KEY_EVENT_DOWN);
    ClearKeyHandler( KEY_UP_ARROW,  KEY_EVENT_UP);
    ClearKeyHandler( KEY_DOWN_ARROW,  KEY_EVENT_DOWN);
    ClearKeyHandler( KEY_DOWN_ARROW,  KEY_EVENT_UP);
    ChangeLeftSoftkey(STR_GLOBAL_DONE,IMG_GLOBAL_OK);
    SetCategory57LeftSoftkeyFunction( mmi_analogtv_effect_bright_setting_set );
}
void mmi_analogtv_exit_Effect_Brightness(void)
{
	history_t	h;
	U16 inputBufferSize;

	CloseCategory57Screen();
	h.scrnID = SCR_ID_ANALOGTV_SETTING_EFFECT_BRIGHT;
	h.entryFuncPtr = mmi_analogtv_entry_Effect_Brightness;
	GetCategoryHistory( h.guiBuffer );
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data( (U8*) h.inputBuffer );
	AddNHistory( h, inputBufferSize );

}
void mmi_analogtv_entry_Effect_Brightness(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8		*guiBuffer;
	U8		*inputBuffer;
	U16	inputBufferSize;
	S32	item_offset = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	UI_image_ID_type item_icons[2] =
	{
	    IMG_GLOBAL_L1,
        IMG_GLOBAL_L2
	};
	UI_string_type brightness_str[7] ;//=

	EntryNewScreen(SCR_ID_ANALOGTV_SETTING_EFFECT_BRIGHT, mmi_analogtv_exit_Effect_Brightness, NULL, NULL);


	InitializeCategory57Screen();


        for(item_offset = 0;item_offset<7;item_offset++)
        {
                brightness_str[item_offset] = (UI_string_type)GetString(STR_CAL_CHINESE_NO1+item_offset);
        }

        item_offset = 0;
	g_analogtv_brightness_setting = g_analogtv_cntx.analogtv_setting.yuv_brightness;

	/****************************************************************************
	*  Create inline selection                                                               
	*****************************************************************************/

		SetInlineItemCaption(	&wgui_inline_items[item_offset++],
							(PU8)GetString((U16)(STR_MENU_ID_ANALOGTV_OPTION_EFFECT_BRIGHTNESS)));   
        DisableInlineItemHighlight(&wgui_inline_items[item_offset-1]);
		SetInlineItemActivation(	&wgui_inline_items[item_offset], 
							INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 
							0 );
		SetInlineItemSelect(		&wgui_inline_items[item_offset++], 
							7, 
							(U8**)brightness_str, 
							&g_analogtv_brightness_setting);
		//RegisterInlineSelectHighlightHandler(&wgui_inline_items[item_offset - 1], 
		//	mmi_camera_setting_shutter_sound_hdlr);
		
	
	/* enter category */
	guiBuffer	= GetCurrGuiBuffer( SCR_ID_ANALOGTV_SETTING_EFFECT_BRIGHT );
	inputBuffer = GetCurrNInputBuffer( SCR_ID_ANALOGTV_SETTING_EFFECT_BRIGHT, &inputBufferSize );
	
	if(inputBuffer != NULL)		
		SetCategory57Data(wgui_inline_items, 1, inputBuffer);	


	DisableCategory57ScreenDone();
	RegisterHighlightHandler( mmi_analogtv_effect_bright_highlight_hdlr );

	/* show category */
	ShowCategory57Screen(STR_MENU_ID_ANALOGTV_OPTION_EFFECT_BRIGHTNESS,
                                    0,		
								STR_GLOBAL_OK, IMG_GLOBAL_OK,											
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,									
								item_offset,
								item_icons, 
								wgui_inline_items,
								0, guiBuffer);

	/* set key hdlr */
    
    ClearKeyHandler(KEY_UP_ARROW,  KEY_EVENT_DOWN);
    ClearKeyHandler( KEY_UP_ARROW,  KEY_EVENT_UP);
    ClearKeyHandler( KEY_DOWN_ARROW,  KEY_EVENT_DOWN);
    ClearKeyHandler( KEY_DOWN_ARROW,  KEY_EVENT_UP);
    
	SetCategory57LeftSoftkeyFunction( mmi_analogtv_effect_bright_setting_set );
	SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );

}

void mmi_analogtv_entry_Effect_Color_Depth(void)
{

}

void mmi_analogtv_entry_Recorder(void)
{

}
/**************************************************************

	FUNCTION NAME		: mmi_analogtv_setting(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: 

	RETURNS				: void

 

**************************************************************/
void mmi_analogtv_setting(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U16 SettingsSubmenuImageList[MAX_SUB_MENUS];

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_ID_ANALOGTV_SETTING, NULL, mmi_analogtv_setting, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_SETTING);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_ID_ANALOGTV_OPTION_SETTING);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_ID_ANALOGTV_OPTION_SETTING);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_ID_ANALOGTV_OPTION_SETTING,nStrItemList);

	GetSequenceImageIds(MENU_ID_ANALOGTV_OPTION_SETTING, SettingsSubmenuImageList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_ID_ANALOGTV_OPTION_SETTING);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);


	/* 8 Display Category Screen */
/*	ShowCategory11Screen(STR_ID_ATV_PLAYER_SETTING, NULL,
						 STR_GLOBAL_OK, 0,
						 STR_GLOBAL_BACK, 0,
						 nNumofItem, nStrItemList,0,guiBuffer);
*/	
	ShowCategory15Screen(STR_MENU_ID_ANALOGTV_OPTION_SETTING, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList,
//#ifndef __MMI_SUBMENU_ALL_NUMBER_ICON__
//						  SettingsSubmenuImageList,
//#else						
						  (U16*)gIndexIconsImageList,  //lrg add 
//#endif 
						  LIST_MENU, 0, guiBuffer);
}

/**************************************************************

	FUNCTION NAME		: mmi_analogtv_setting_storage(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: 

	RETURNS				: void

 

**************************************************************/
void mmi_analogtv_setting_storage(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer,hilight;       /* Buffer holding history data */
	S16 error_code;
	S16 cardDrvLetter;
		
	//if storage is t-flash but it does not exist,
	//reset storage before enter setting screen.

	if(g_analogtv_cntx.analogtv_setting.storage == ANALOGTV_STORAGE_CARD && 
		((cardDrvLetter = MMI_CARD_DRV) < 0))
	{		
		g_analogtv_cntx.analogtv_setting.storage = ANALOGTV_STORAGE_PHONE;
		g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_STORAGE_ID] = g_analogtv_cntx.analogtv_setting.storage;
		WriteRecord( NVRAM_EF_ATV_PLAYER_SETTINGS_LID, 1, (void *)g_ATVPlayerNVRAMBuff, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE, &error_code);
	}
	hilight = g_analogtv_cntx.analogtv_setting.storage;

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_ID_ANALOGTV_SETTING_STORAGE, NULL, mmi_analogtv_setting_storage, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_SETTING_STORAGE);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_ID_ANALOGTV_SETTING_STORAGE);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_ID_ANALOGTV_SETTING_STORAGE);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_ID_ANALOGTV_SETTING_STORAGE,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_ID_ANALOGTV_SETTING_STORAGE);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* 8 Display Category Screen */
	ShowCategory11Screen(STR_MENU_ID_ANALOGTV_SETTING_STORAGE, 0,
						 STR_GLOBAL_OK, 0,
						 STR_GLOBAL_BACK, 0,
						 nNumofItem, nStrItemList,hilight,guiBuffer);
}

/**************************************************************

	FUNCTION NAME		: mmi_analogtv_set_storage_phone(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: 

	RETURNS				: void

 

**************************************************************/
void mmi_analogtv_set_storage_phone(void)
{
	S16 error_code;

	if(g_analogtv_cntx.analogtv_setting.storage != ANALOGTV_STORAGE_PHONE)
	{
		g_analogtv_cntx.analogtv_setting.storage = ANALOGTV_STORAGE_PHONE;
		g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_STORAGE_ID] = g_analogtv_cntx.analogtv_setting.storage;
		WriteRecord( NVRAM_EF_ATV_PLAYER_SETTINGS_LID, 1, (void *)g_ATVPlayerNVRAMBuff, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE, &error_code);
	}
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);

	GoBacknHistory(1);
}

/**************************************************************

	FUNCTION NAME		: mmi_analogtv_set_storage_card(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: 

	RETURNS				: void

 

**************************************************************/
void mmi_analogtv_set_storage_card(void)
{
	S16 error_code;

	if( MMI_CARD_DRV > 0 )
	{
		if(g_analogtv_cntx.analogtv_setting.storage != ANALOGTV_STORAGE_CARD)
		{
			g_analogtv_cntx.analogtv_setting.storage = ANALOGTV_STORAGE_CARD;
			g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_STORAGE_ID] = g_analogtv_cntx.analogtv_setting.storage;
			WriteRecord( NVRAM_EF_ATV_PLAYER_SETTINGS_LID, 1, (void *)g_ATVPlayerNVRAMBuff, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE, &error_code);
		}

		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	}

	else
	{
		if(g_analogtv_cntx.analogtv_setting.storage != ANALOGTV_STORAGE_PHONE)
		{
			g_analogtv_cntx.analogtv_setting.storage = ANALOGTV_STORAGE_PHONE;
			g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_STORAGE_ID] = g_analogtv_cntx.analogtv_setting.storage;
			WriteRecord( NVRAM_EF_ATV_PLAYER_SETTINGS_LID, 1, (void *)g_ATVPlayerNVRAMBuff, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE, &error_code);
		}

		DisplayPopup((PU8)GetString(STR_ID_AUDPLY_NO_CARD) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
	}	
	//GoBacknHistory(1);
}


void mmi_analogtv_hilight_app(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_app, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}

void highlight_mainmenu_analogtv(void)
{
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    SetLeftSoftkeyFunction(mmi_mainmenu_analogtv, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}
void Hilight_Option_Channel_Search(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Search, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Search, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

void Hilight_Channel_Search_Auto(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Search_Auto, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Search_Auto, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	
}

void Hilight_Channel_Search_Manual(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Search_Manual, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Search_Manual, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}

void Hilight_Option_Channel_Select(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Select, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Select, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}

void Hilight_Channel_Select_List(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Select_List, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Select_List, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	
}

void Hilight_Channel_Select_User_Defined(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Select_User_Channel, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Select_User_Channel, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	
}

void Hilight_Channel_Select_List_Play(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Select_List_Play, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Select_List_Play, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}

void Hilight_Channel_Select_List_Rename(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Select_List_Rename, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Select_List_Rename, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	
}


void Hilight_User_Defined_Play(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Select_User_Play, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Select_User_Play, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	
}

void Hilight_User_Defined_Rename(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Select_User_Rename, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Select_User_Rename, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
}

/**************************************************************

	FUNCTION NAME		: HighlightATVPlayerSetting

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: 

	RETURNS				: void

 

**************************************************************/
void Hilight_Analogtv_Setting(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(mmi_analogtv_setting,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(mmi_analogtv_setting,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

/**************************************************************

	FUNCTION NAME		: HighlightATVPlayerSettingStorage

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: 

	RETURNS				: void

 

**************************************************************/
void Hilight_Analogtv_Setting_Storage(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(mmi_analogtv_setting_storage,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(mmi_analogtv_setting_storage,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: HighlightATVPlayerSettingStorage

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: 

	RETURNS				: void

 

**************************************************************/
void Hilight_Analogtv_Setting_Delete_All_Channel(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(mmi_analogtv_delete_all_channel_confirm, KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightATVPlayerPhone(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: 

	RETURNS				: void

 

**************************************************************/
void Hilight_Analogtv_Phone(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(mmi_analogtv_set_storage_phone,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightATVPlayerCard(void)

  	PURPOSE				: 

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: 

	RETURNS				: void

 

**************************************************************/
void Hilight_Analogtv_Card(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(mmi_analogtv_set_storage_card,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

void Hilight_Option_Add_User_Channel(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Add, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Add, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
	
}

void Hilight_Option_Search_Area(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Search_Area, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Search_Area, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
	
}

void Hilight_Option_Effect(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Effect, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Effect, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	

}

void Hilight_Effect_Brightness(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Effect_Brightness, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Effect_Brightness, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);		
}

void Hilight_Effect_Color_Depth(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Effect_Color_Depth, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Effect_Color_Depth, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);		
	
}

void Hilight_Option_Recorder(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_analogtv_entry_Recorder, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_entry_Recorder, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);		
	
}

void InitAnalogTV(void)
{
	SetHiliteHandler (MENU_ID_ANALOGTV_MAIN, mmi_analogtv_hilight_app);

//search
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_CHANNEL_SEARCH, Hilight_Option_Channel_Search);
	SetHiliteHandler (MENU_ID_ANALOGTV_CHANNEL_AUTO_SEARCH, Hilight_Channel_Search_Auto);
	SetHiliteHandler (MENU_ID_ANALOGTV_CHANNEL_MANUAL_SEARCH, Hilight_Channel_Search_Manual);
//select	
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_CHANNEL_SELECT, Hilight_Option_Channel_Select);
	SetHiliteHandler (MENU_ID_ANALOGTV_CHANNEL_LIST, Hilight_Channel_Select_List);
	SetHiliteHandler (MENU_ID_ANALOGTV_CHANNEL_USER_DEFINED, Hilight_Channel_Select_User_Defined);
	SetHiliteHandler (MENU_ID_ANALOGTV_CHANNEL_LIST_PLAY, Hilight_Channel_Select_List_Play);
	SetHiliteHandler (MENU_ID_ANALOGTV_CHANNEL_LIST_RENAME, Hilight_Channel_Select_List_Rename);
	SetHiliteHandler (MENU_ID_ANALOGTV_CHANNEL_USER_DEFINED_PLAY, Hilight_User_Defined_Play);
	SetHiliteHandler (MENU_ID_ANALOGTV_CHANNEL_USER_DEFINED_RENAME, Hilight_User_Defined_Rename);
	SetHiliteHandler (MENU_ID_ANALOGTV_CHANNEL_LIST_DELETE, Hilight_Channel_Select_List_Delete); 
//add	
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_ADD_USER_CHANNEL, Hilight_Option_Add_User_Channel);
//area
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_SEARCH_AREA, Hilight_Option_Search_Area);
#if 0
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_CHINA_AREA, Hilight_Search_Area_China);
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_SHENZHEN_AREA, Hilight_Search_Area_Shenzhen);
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_HONGKONG_AREA, Hilight_Search_Area_Hongkong);
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_AMERICA_AREA, Hilight_Search_Area_America);
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_ARGENTINA_AREA, Hilight_Search_Area_Argentina);
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_BRAZIL_AREA, Hilight_Search_Area_Brazil);
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_EUROPE_AREA, Hilight_Search_Area_Europe);
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_JAPAN_AREA, Hilight_Search_Area_Japan);
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_VIETNAM_AREA, Hilight_Search_Area_Vietnam);
#endif
//setting
	SetHiliteHandler( MENU_ID_ANALOGTV_OPTION_SETTING, Hilight_Analogtv_Setting );
	SetHiliteHandler( MENU_ID_ANALOGTV_SETTING_STORAGE, Hilight_Analogtv_Setting_Storage );
	SetHiliteHandler( MENU_ID_ANALOGTV_SETTING_STORAGE_PHONE, Hilight_Analogtv_Phone );
	SetHiliteHandler( MENU_ID_ANALOGTV_SETTING_STORAGE_CARD, Hilight_Analogtv_Card );
	SetHiliteHandler( MENU_ID_ANALOGTV_SETTING_DELETE_ALL_CHANNEL, Hilight_Analogtv_Setting_Delete_All_Channel );

//effect	
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_EFFECT, Hilight_Option_Effect);
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_EFFECT_BRIGHTNESS, Hilight_Effect_Brightness);
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_EFFECT_COLOR_DEPTH, Hilight_Effect_Color_Depth);
//record	
	SetHiliteHandler (MENU_ID_ANALOGTV_OPTION_RECORDER, Hilight_Option_Recorder);

	mmi_analogtv_init_app();
	mmi_analogtv_load_setting();
	
}

void mmi_analogtv_exit_preview_screen(void)
{
#if ANALOG_TV_SUPPORT_MCP_LOCK//xiaoyifeng for atv mcp lock	
	atvd_stop_check_status();
#else
    StopTimer(RDAMTV_STATUS_CHECK_TIMER);
#endif
	GoBackHistory();
}

/*****************************************************************************
 * FUNCTION
 *  mmi_analogtv_preview_set_key_hdlr
 * DESCRIPTION
 *  set key handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 void mmi_analogtv_goto_channel(void)
{
#if defined(__MMI_ANALOGTV_SHOW_CHANNEL_NUMBER__)
	mmi_analogtv_show_channel_number_TV();
#endif
	mmi_analogtv_play_channel(g_analogtv_cntx.analogtv_setting.cur_channel.channel.frequency);
}

 void mmi_analogtv_preview_hot_channel_key_up(void)
{
	U16 Keycode,Keytype,focus_channel,i;
	U16 channel_index=0;
      
	//U16 hot_channel_keys_map[10][2] = {{KEY_0,0},{KEY_1,1},{KEY_2,2},{KEY_3,3},{KEY_4,4},{KEY_5,5},{KEY_6,6},{KEY_7,7},{KEY_8,8},{KEY_9,9}};
	U16 hot_channel_keys_map[9][2] = {{KEY_1,0},{KEY_2,1},{KEY_3,2},{KEY_4,3},{KEY_5,4},{KEY_6,5},{KEY_7,6},{KEY_8,7},{KEY_9,8}};
     //end
	channel_index=g_analogtv_cntx.analogtv_setting.cur_channel.cur_index;
	GetkeyInfo(&Keycode,&Keytype);
	for( i = 0; i < 10; i++ )
	{
		if(hot_channel_keys_map[i][0] == Keycode)
		{
			focus_channel = hot_channel_keys_map[i][1];
			if((channel_index!= focus_channel)&&(g_analogtv_cntx.channel_list[focus_channel].Present==TRUE))
			{
				g_analogtv_cntx.analogtv_setting.cur_channel.cur_index=focus_channel;
				g_analogtv_cntx.analogtv_setting.cur_channel.channel=g_analogtv_cntx.channel_list[focus_channel];
				mmi_analogtv_goto_channel();
				g_analogtv_cntx.analogtv_setting.is_mychannel=FALSE;	

			}
			break;
		}
	}
}

static void mmi_analogtv_preview_set_key_hdlr(void)
{
	U16 hot_channel_keys[10] = {KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9};
	
	ClearInputEventHandler(MMI_DEVICE_KEY);
	SetKeyHandler(mmi_analogtv_entry_option_screen, KEY_LSK, KEY_EVENT_UP);
	SetKeyHandler(mmi_analogtv_exit_preview_screen, KEY_RSK, KEY_EVENT_UP);

 
	SetKeyHandler(channel_pre_button_up_handle, KEY_LEFT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(channel_pre_button_down_handle, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(channel_next_button_up_handle, KEY_RIGHT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(channel_next_button_down_handle, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
//end

//as customer spec!    
	SetKeyHandler(volume_inc_button_up_handle, KEY_UP_ARROW, KEY_EVENT_UP);
	SetKeyHandler(volume_inc_button_down_handle, KEY_UP_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(volume_dec_button_up_handle, KEY_DOWN_ARROW, KEY_EVENT_UP);
	SetKeyHandler(volume_dec_button_down_handle, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    
//as customer spec!
    //   SetKeyHandler(brightness_button_up_handle, KEY_UP_ARROW, KEY_EVENT_UP);
//	SetKeyHandler(NULL, KEY_UP_ARROW, KEY_EVENT_DOWN);
	//SetKeyHandler(brightness_button_down_handle, KEY_DOWN_ARROW, KEY_EVENT_UP);
//	SetKeyHandler(NULL, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
//end	
 
#if defined(__MMI_ANALOGTV_SNAPSHOT__)
	SetKeyHandler(snap_button_down_handle, KEY_POUND, KEY_EVENT_DOWN); 
#endif
#if !defined(__SCREEN_AUTO_ROTATE_SUPPORT__) 
	 
	ClearKeyHandler(KEY_STAR, KEY_EVENT_UP);
       SetKeyHandler(preview_playwnd_up_handle, KEY_STAR, KEY_EVENT_UP); 
       //end
#endif
//end
	SetGroupKeyHandler(mmi_analogtv_preview_hot_channel_key_up,hot_channel_keys,
								10,KEY_EVENT_UP);
}


static void mmi_analogtv_exit_app_screen(void)
{
	S16 error_code;
	//kal_prompt_trace(MOD_MED,"mmi_analogtv_exit_app_screen");
	ANALOGTV_ENTER_STATE(ANALOGTV_STATE_EXIT);

#if defined(MMI_2D_SENSOR_SUPPORT)
#if defined(__MMI_SCREEN_ROTATE__)
           // csd_mmi_turnon_2d_sensor_by_backlight();
#endif
#endif
        StopTimer(ANALOGTV_AUTO_SEARCH_TIMER);
#if ANALOG_TV_SUPPORT_MCP_LOCK	
		atvd_stop_check_status();
#else
        StopTimer(RDAMTV_STATUS_CHECK_TIMER);
#endif
       mmi_trace(1,"  mmi_analogtv_exit_app_screen " );
	/* shut down camera */
//    	mdi_audio_set_volume(AUD_VOLUME_TV,g_analogtv_cntx.volume);
//	COS_Sleep(200);
	mdi_camera_power_off();
    #if defined(ANALOG_TV_SUPPORT) && defined(__FF_AUDIO_SET__)
    	GpioSpeakerModeEnd();
    #endif
	g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_VOLUME_ID] = g_analogtv_cntx.volume;
	
	if(g_analogtv_cntx.channel_list[g_analogtv_cntx.analogtv_setting.cur_channel.cur_index].Present == FALSE)
	{
		g_analogtv_cntx.analogtv_setting.cur_channel.cur_index = 0;
	}
	g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_LAST_CHANNEL_ID] = g_analogtv_cntx.analogtv_setting.cur_channel.cur_index;

	WriteRecord( NVRAM_EF_ATV_PLAYER_SETTINGS_LID, 1, (void *)g_ATVPlayerNVRAMBuff, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE, &error_code);

	/* resume alignment timer */
	UI_enable_alignment_timers();

	TurnOffBacklight();

	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);

	g_analogtv_UI.base_layer_handle=GDI_LAYER_EMPTY_HANDLE;

      if(g_analogtv_UI.preview_layer_handle!=GDI_LAYER_EMPTY_HANDLE)
      {
            gdi_layer_free(g_analogtv_UI.preview_layer_handle);
            g_analogtv_UI.preview_layer_handle=GDI_LAYER_EMPTY_HANDLE;
      }
      if(g_analogtv_UI.osd_layer_handle!=GDI_LAYER_EMPTY_HANDLE)
      {
            gdi_layer_free(g_analogtv_UI.osd_layer_handle);
            g_analogtv_UI.osd_layer_handle=GDI_LAYER_EMPTY_HANDLE;

      }

      if(g_analogtv_UI.osd_layer_buf_ptr)
      {
            mmi_frm_scrmem_free((void*)g_analogtv_UI.osd_layer_buf_ptr);
            g_analogtv_UI.osd_layer_buf_ptr= NULL;	
      }

	gdi_layer_multi_layer_disable();


       gdi_layer_restore_base_active();

     //   mdi_audio_set_mute(MDI_VOLUME_MEDIA, FALSE);
	COS_Sleep(200);
#if defined(MMI_ON_HARDWARE_P) 
       //MMI_Sleep(3 );
	atvif_SetCurModule(0);
#endif
 
}


/*****************************************************************************
 * FUNCTION
 *  mmi_analogtv_enter_state
 * DESCRIPTION
 *  enter next state
 * PARAMETERS
 *  state       [IN]        State to enter
 * RETURNS
 *  void
 *****************************************************************************/
 void mmi_analogtv_show_softkey(void)
{
	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
	if (g_analogtv_cntx.set_state== ANALOGTV_STATE_PREVIEW_CHANNEL_AUTO_SEARCH)
		change_left_softkey( 0 ,0);
	else
		change_left_softkey( STR_GLOBAL_OPTIONS ,0);
	change_right_softkey(STR_GLOBAL_BACK ,0);
	show_left_softkey();
	show_right_softkey();
	gdi_layer_pop_and_restore_active();

}

void mmi_analogtv_fullscr_play_set_key_hdlr(void)
{
        U16 hot_channel_keys[10] = {KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9};

        ClearInputEventHandler(MMI_DEVICE_KEY);
        SetKeyHandler(mmi_analogtv_entry_option_screen, KEY_LSK, KEY_EVENT_UP);
        SetKeyHandler(mmi_analogtv_exit_preview_screen, KEY_RSK, KEY_EVENT_UP);


//as customer spec!    
	SetKeyHandler(volume_inc_button_up_handle, KEY_UP_ARROW, KEY_EVENT_UP);
	SetKeyHandler(volume_inc_button_down_handle, KEY_UP_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(volume_dec_button_up_handle, KEY_DOWN_ARROW, KEY_EVENT_UP);
	SetKeyHandler(volume_dec_button_down_handle, KEY_DOWN_ARROW, KEY_EVENT_DOWN);


//as customer spec!    
	//SetKeyHandler(volume_inc_button_up_handle, KEY_2, KEY_EVENT_UP);
	//SetKeyHandler(volume_inc_button_down_handle, KEY_2, KEY_EVENT_DOWN);
	//SetKeyHandler(volume_dec_button_up_handle, KEY_8, KEY_EVENT_UP);
	//SetKeyHandler(volume_dec_button_down_handle, KEY_8, KEY_EVENT_DOWN);
    
//as customer spec!
    //   SetKeyHandler(brightness_button_up_handle, KEY_UP_ARROW, KEY_EVENT_UP);
	//SetKeyHandler(NULL, KEY_UP_ARROW, KEY_EVENT_DOWN);
	//SetKeyHandler(brightness_button_down_handle, KEY_DOWN_ARROW, KEY_EVENT_UP);
	//SetKeyHandler(NULL, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
//end	

        ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
        ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
        SetKeyHandler(mmi_analogtv_set_channel_next, KEY_RIGHT_ARROW, KEY_EVENT_UP);
        SetKeyHandler(mmi_analogtv_set_channel_pre, KEY_LEFT_ARROW, KEY_EVENT_UP);





#if !defined(__SCREEN_AUTO_ROTATE_SUPPORT__) 
        ClearKeyHandler(KEY_STAR, KEY_EVENT_UP);
        SetKeyHandler(mmi_analogtv_fullscr_play_key, KEY_STAR, KEY_EVENT_UP);
#endif
        SetGroupKeyHandler(mmi_analogtv_preview_hot_channel_key_up,hot_channel_keys,
								10,KEY_EVENT_UP);
}

void mmi_analogtv_fullscr_play_pen_up_hdlr(mmi_pen_point_struct pos)
{
	g_analogtv_cntx.set_state=ANALOGTV_STATE_PREVIEW;
	mmi_analogtv_entry_preview_screen();
}

void mmi_analogtv_fullscr_play_key(void)
{
	g_analogtv_cntx.set_state=ANALOGTV_STATE_PREVIEW;
	mmi_analogtv_entry_preview_screen();
    //ANALOGTV_ENTER_STATE(g_analogtv_cntx.set_state);
}

void mmi_analogtv_entry_fullscr_play_state(void)
{
#if defined(MMI_ON_HARDWARE_P)
	if(g_analogtv_cntx.analogtv_setting.is_mychannel)
	{
		atvd_SetChannel(g_analogtv_cntx.analogtv_setting.cur_mychannel.channel.frequency);
	}
	else
	{
		atvd_SetChannel(g_analogtv_cntx.analogtv_setting.cur_channel.channel.frequency);
	}
#endif
	/* register keys */
	mmi_analogtv_fullscr_play_set_key_hdlr();
#if !defined(__SCREEN_AUTO_ROTATE_SUPPORT__) 
	#if defined(__MMI_TOUCH_SCREEN__)
	wgui_register_pen_up_handler(mmi_analogtv_fullscr_play_pen_up_hdlr);
	#endif
#endif
	/* if no error occurs, start preview */
	#if defined(MMI_ON_HARDWARE_P)
	mmi_analogtv_fullscreen_preview();
       mmi_trace(1,"  mmi_analogtv_entry_fullscr_play_state volume %d  ",g_analogtv_cntx.volume);
	COS_Sleep(50);
	mdi_audio_set_volume(AUD_VOLUME_TV,0);
	mdi_audio_set_volume(AUD_VOLUME_TV,g_analogtv_cntx.volume);
	#endif
}

void mmi_analogtv_pre_entry_fullscr_play_state(void)
{
	
	gdi_layer_push_and_set_active(g_analogtv_UI.preview_layer_handle);
       gdi_layer_clear(GDI_COLOR_BLACK);
	gdi_layer_pop_and_restore_active();
	
	gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
       gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();

	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
       gdi_layer_clear(GDI_COLOR_TRANSPARENT);
      // mmi_analogtv_show_open_str();
	gdi_layer_pop_and_restore_active();
    

	gdi_layer_set_blt_layer(
	g_analogtv_UI.base_layer_handle,
	g_analogtv_UI.preview_layer_handle,    	
	g_analogtv_UI.osd_layer_handle,    	
	0);
    
	gdi_layer_blt_previous(0, 0, UI_device_width  - 1,  UI_device_height- 1);

}

void mmi_analogtv_exit_fullscr_play_state(void)
{
	atvif_SetFullScreen(FALSE);	
	//gdi_lcd_set_rotate(LCD_LAYER_ROTATE_NORMAL);
	//gdi_lcd_set_rotate_by_layer(FALSE);
	g_analogtv_UI.lcm_rotate=LCD_LAYER_ROTATE_NORMAL;

	
	g_analogtv_UI.preview_x=g_analogtv_UI.play_wnd->x;
	g_analogtv_UI.preview_y=g_analogtv_UI.play_wnd->y;
	g_analogtv_UI.preview_width=g_analogtv_UI.play_wnd->width;
	g_analogtv_UI.preview_height=g_analogtv_UI.play_wnd->height;

    
    #if 0
	gdi_layer_push_and_set_active(g_analogtv_UI.preview_layer_handle);
	gdi_layer_resize(g_analogtv_UI.preview_width, g_analogtv_UI.preview_height);
	gdi_layer_pop_and_restore_active();

	gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
	gdi_layer_set_position(0, 0);
	gdi_layer_resize(UI_device_width,UI_device_height);
	gdi_layer_pop_and_restore_active();

	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
	gdi_layer_set_position(0, 0);
	gdi_layer_resize(UI_device_width,UI_device_height);
	gdi_layer_pop_and_restore_active();
    #endif

}

static void mmi_analogtv_enter_state(analogtv_state_enum state)
{
        g_analogtv_cntx.app_prev_state = g_analogtv_cntx.app_state;
        g_analogtv_cntx.app_state = state;

        mmi_trace(1,"mmi_analogtv_enter_state state = %d  ",state);
        mmi_trace(1,"mmi_analogtv_enter_state state = %d  ",g_analogtv_cntx.app_prev_state);

        switch (state)
        {
            case ANALOGTV_STATE_EXIT:
                if(g_analogtv_cntx.app_prev_state ==ANALOGTV_STATE_PREVIEW_CHANNEL_AUTO_SEARCH)
                {
                    StopTimer(ANALOGTV_AUTO_SEARCH_TIMER);
                    g_analogtv_cntx.set_state=ANALOGTV_STATE_PREVIEW;
                }
#if ANALOG_TV_SUPPORT_MCP_LOCK	
                atvd_stop_check_status();
#else		
                StopTimer(RDAMTV_STATUS_CHECK_TIMER);
#endif
                g_analogtv_cntx.app_prev_state=ANALOGTV_STATE_EXIT;
                g_analogtv_cntx.app_next_state=ANALOGTV_STATE_EXIT;
            break;

            /* main lcd */
            case ANALOGTV_STATE_PREVIEW:
            {	
                mmi_analogtv_preview_show_controlpannel();	
                mmi_analogtv_entry_preview_state();
            }
            break;
            case ANALOGTV_STATE_PREVIEW_CHANNEL_AUTO_SEARCH:
            {
                mmi_analogtv_preview_show_autosearch_pannel();				
                mmi_analogtv_entry_auto_search_state();
            }
            break;
            case ANALOGTV_STATE_FULL_SCR_PLAY:
                mmi_analogtv_pre_entry_fullscr_play_state();
                mmi_analogtv_entry_fullscr_play_state();
            break;

            case ANALOGTV_STATE_SEARCH_CHANNEL:
                /* add function here */
            break;
            default:
            break;
        }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_analogtv_exit_current_state
 * DESCRIPTION
 *  exit analogtv current state
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_analogtv_exit_current_state(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
    mmi_trace(1," mmi_analogtv_exit_current_state %d   ", g_analogtv_cntx.app_state);
	switch (g_analogtv_cntx.app_state)
	{
	case ANALOGTV_STATE_EXIT:
	/* do nothing */
	break;

	/* main lcd */
	case ANALOGTV_STATE_PREVIEW:		
		mmi_analogtv_exit_preview_state();
	break;
		
	case ANALOGTV_STATE_PREVIEW_CHANNEL_AUTO_SEARCH:		
		mmi_analogtv_exit_preview_state();
	break;

	case ANALOGTV_STATE_FULL_SCR_PLAY:
		mmi_analogtv_exit_preview_state();
		mmi_analogtv_exit_fullscr_play_state();
	break;
	
	case ANALOGTV_STATE_SEARCH_CHANNEL:
	/* add function here */
	break;
       default:
        break;
	}
}

BOOL mmi_analogtv_preview_hit_tst(S32 pos_x, S32 pos_y, analogtv_button *touch_obj)
{
	if((pos_x >= touch_obj->x) &&
		(pos_x <  touch_obj->x + touch_obj->width) &&
		(pos_y >= touch_obj->y) &&
		(pos_y <  touch_obj->y + touch_obj->height))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL mmi_analogtv_preview_hit_playwnd_tst(S32 pos_x, S32 pos_y, analogtv_rect *touch_obj)
{
	if((pos_x >= touch_obj->x) &&
		(pos_x <  touch_obj->x + touch_obj->width) &&
		(pos_y >= touch_obj->y) &&
		(pos_y <  touch_obj->y + touch_obj->height))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}	
void channel_pre_button_down_handle(void)
{
	g_analogtv_UI.channel_pre->status=PRESSED;
	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
  	//channel pre
	gdi_image_draw_id(g_analogtv_UI.channel_pre->x,g_analogtv_UI.channel_pre->y,g_analogtv_UI.channel_pre->image_id[g_analogtv_UI.channel_pre->status]);

	//kal_prompt_trace(MOD_NVRAM,"------%d--%d",g_analogtv_UI.channel_pre->x,g_analogtv_UI.channel_pre->y);
	gdi_layer_pop_and_restore_active();
	mmi_analogtv_blt_buttonarea_screen();

}
static void mmi_analogtv_play_channel(U16 channel_id)
{
#if defined(MMI_ON_HARDWARE_P)
    uint8 ret = 0;

  //  mdi_audio_set_mute(MDI_VOLUME_MEDIA, TRUE);
	
    ret = atvd_SetChannel(channel_id);
   #if 0 
    if (ret)
    {
        mdi_audio_set_volume(AUD_VOLUME_TV,g_analogtv_cntx.volume);
        mdi_audio_set_mute(MDI_VOLUME_MEDIA, FALSE);
    }
    else
    {
        mmi_analogtv_show_search_str();      
        mmi_analogtv_blt_screen();
    }
#endif
#endif
}

void mmi_analogtv_set_channel_pre(void)
{
	int channel_index;

       mmi_trace(1,"mmi_analogtv_set_channel_pre %d  ", g_analogtv_cntx.analogtv_setting.total_channel);
	if(g_analogtv_cntx.analogtv_setting.total_channel == 0)
	{
		return;
	}
	g_analogtv_cntx.analogtv_setting.cur_channel.cur_index--;
	if(g_analogtv_cntx.analogtv_setting.cur_channel.cur_index<0)
		g_analogtv_cntx.analogtv_setting.cur_channel.cur_index=g_analogtv_cntx.analogtv_setting.total_channel-1;
	channel_index=g_analogtv_cntx.analogtv_setting.cur_channel.cur_index;
	g_analogtv_cntx.analogtv_setting.cur_channel.channel=g_analogtv_cntx.channel_list[channel_index];
	mmi_analogtv_play_channel(g_analogtv_cntx.analogtv_setting.cur_channel.channel.frequency);
	g_analogtv_cntx.analogtv_setting.is_mychannel=FALSE;
}
void channel_pre_button_up_handle(void)
{
	if(g_analogtv_UI.channel_pre->status == PRESSED)
	{
        	g_analogtv_UI.channel_pre->status=NORMAL;
        	//gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);

        	//gdi_layer_clear(GDI_COLOR_BLACK);
        	//gdi_layer_pop_and_restore_active();

        	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
          	//channel pre
          	#if defined(__MMI_ANALOGTV_SHOW_CHANNEL_NUMBER__)
          	mmi_draw_pre_channel_number();
        	#endif
        	gdi_image_draw_id(g_analogtv_UI.channel_pre->x,g_analogtv_UI.channel_pre->y,g_analogtv_UI.channel_pre->image_id[g_analogtv_UI.channel_pre->status]);

        	gdi_layer_pop_and_restore_active();
        	mmi_analogtv_blt_buttonarea_screen();
        	mmi_analogtv_set_channel_pre();
	}
}

void channel_next_button_down_handle(void)
{
	g_analogtv_UI.channel_next->status=PRESSED;
	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
	//channel next
	gdi_image_draw_id(g_analogtv_UI.channel_next->x,g_analogtv_UI.channel_next->y,g_analogtv_UI.channel_next->image_id[g_analogtv_UI.channel_next->status]);
	gdi_layer_pop_and_restore_active();
	mmi_analogtv_blt_buttonarea_screen();
}

void mmi_analogtv_set_channel_next(void)
{
//       kal_uint32 save_irq_mask;

	int channel_index;
	if(g_analogtv_cntx.analogtv_setting.total_channel == 0)
	{
		return;
	}
	g_analogtv_cntx.analogtv_setting.cur_channel.cur_index++;
	if(g_analogtv_cntx.analogtv_setting.cur_channel.cur_index>=g_analogtv_cntx.analogtv_setting.total_channel)
		g_analogtv_cntx.analogtv_setting.cur_channel.cur_index=0;
	channel_index=g_analogtv_cntx.analogtv_setting.cur_channel.cur_index;
	g_analogtv_cntx.analogtv_setting.cur_channel.channel=g_analogtv_cntx.channel_list[channel_index];

	mmi_analogtv_play_channel(g_analogtv_cntx.analogtv_setting.cur_channel.channel.frequency);

#if 0
	 save_irq_mask=SaveAndSetIRQMask();

	isp_digital_zoom_flag=KAL_TRUE;

	ENABLE_CAMERA_FRAME_READY_INT;

	RestoreIRQMask(save_irq_mask);
#endif	
	g_analogtv_cntx.analogtv_setting.is_mychannel=FALSE;
}
void channel_next_button_up_handle(void)
{
	if(g_analogtv_UI.channel_next->status == PRESSED)
	{
        	g_analogtv_UI.channel_next->status=NORMAL;
        	//gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);

        	//gdi_layer_clear(GDI_COLOR_BLACK);
        	//gdi_layer_pop_and_restore_active();
        	gdi_layer_reset_clip();
        	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
        	//channel next
        	#if defined(__MMI_ANALOGTV_SHOW_CHANNEL_NUMBER__)
        	mmi_draw_next_channel_number();
        	#endif
        	gdi_image_draw_id(g_analogtv_UI.channel_next->x,g_analogtv_UI.channel_next->y,g_analogtv_UI.channel_next->image_id[g_analogtv_UI.channel_next->status]);
        	gdi_layer_pop_and_restore_active();
        	mmi_analogtv_blt_buttonarea_screen();
        	mmi_analogtv_set_channel_next();
	}
}
void volume_dec_button_down_handle(void)
{
    g_analogtv_UI.vol->vol_dec.status=PRESSED;
    if (!analogtv_is_full_screen())
    {
        gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
        gdi_image_draw_id(g_analogtv_UI.vol->vol_dec.x,g_analogtv_UI.vol->vol_dec.y,g_analogtv_UI.vol->vol_dec.image_id[g_analogtv_UI.vol->vol_dec.status]);
        gdi_layer_pop_and_restore_active();
        mmi_analogtv_blt_buttonarea_screen();
    }
}

void volume_dec_button_up_handle(void)
{
	//int channel_index;
	if(g_analogtv_UI.vol->vol_dec.status == PRESSED)
	{
        g_analogtv_UI.vol->vol_dec.status=NORMAL;
        mmi_analogtv_dec_volume();
	}
}

void volume_inc_button_down_handle(void)
{
	g_analogtv_UI.vol->vol_inc.status=PRESSED;
    if (!analogtv_is_full_screen())
    {
        gdi_layer_reset_clip();
        gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
        //channel next
        gdi_image_draw_id(g_analogtv_UI.vol->vol_inc.x,g_analogtv_UI.vol->vol_inc.y,g_analogtv_UI.vol->vol_inc.image_id[g_analogtv_UI.vol->vol_inc.status]);
        gdi_layer_pop_and_restore_active();
        mmi_analogtv_blt_buttonarea_screen();
    }
}

void volume_inc_button_up_handle(void)
{
	if(g_analogtv_UI.vol->vol_inc.status == PRESSED)
	{
        g_analogtv_UI.vol->vol_inc.status=NORMAL;
        mmi_analogtv_inc_volume();
	}
}
void mychannel_button_down_handle(void)
{
	g_analogtv_UI.mychannel->status=PRESSED;
	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
	//vol
	gdi_image_draw_id(g_analogtv_UI.mychannel->x,g_analogtv_UI.mychannel->y,g_analogtv_UI.mychannel->image_id[g_analogtv_UI.mychannel->status]);
	gdi_layer_pop_and_restore_active();
	mmi_analogtv_blt_buttonarea_screen();

}

void mychannel_button_up_handle(void)
{
	g_analogtv_UI.mychannel->status=NORMAL;
	gdi_layer_reset_clip();
	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
	//vol
	gdi_image_draw_id(g_analogtv_UI.mychannel->x,g_analogtv_UI.mychannel->y,g_analogtv_UI.mychannel->image_id[g_analogtv_UI.mychannel->status]);
	gdi_layer_pop_and_restore_active();
	mmi_analogtv_blt_buttonarea_screen();

	mmi_analogtv_entry_Select_User_Channel();

}

void brightness_button_down_handle(void)
{
	g_analogtv_cntx.analogtv_setting.yuv_brightness-- ;

	if (g_analogtv_cntx.analogtv_setting.yuv_brightness <= 0)
	{
		g_analogtv_cntx.analogtv_setting.yuv_brightness = 0;
	}

	if (!analogtv_is_full_screen())
	{
		gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
#ifndef LCDSIZE_220_176
		gdi_image_draw_id(79,153,g_analogtv_UI.brightness->level_imgid[(U8)(g_analogtv_cntx.analogtv_setting.yuv_brightness)]);
#else
		gdi_image_draw_id(99,120,g_analogtv_UI.brightness->level_imgid[(U8)(g_analogtv_cntx.analogtv_setting.yuv_brightness)]);
#endif
		gdi_layer_pop_and_restore_active();
		mmi_analogtv_blt_buttonarea_screen();
	}
#ifndef MMI_ON_WIN32       
    //rdamtv_app_set_brightness(g_analogtv_cntx.analogtv_setting.yuv_brightness );
#endif
}

void brightness_button_up_handle(void)
{
	g_analogtv_cntx.analogtv_setting.yuv_brightness++;

	if( g_analogtv_cntx.analogtv_setting.yuv_brightness >= 6 )
	{
		g_analogtv_cntx.analogtv_setting.yuv_brightness = 6;
		//    return ;
	}

	if (!analogtv_is_full_screen())
	{
		gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
#ifndef LCDSIZE_220_176
		gdi_image_draw_id(79,153,g_analogtv_UI.brightness->level_imgid[(U8)(g_analogtv_cntx.analogtv_setting.yuv_brightness)]);
#else
		gdi_image_draw_id(99,120,g_analogtv_UI.brightness->level_imgid[(U8)(g_analogtv_cntx.analogtv_setting.yuv_brightness)]);
#endif
		gdi_layer_pop_and_restore_active();
		mmi_analogtv_blt_buttonarea_screen();
	}
#ifndef MMI_ON_WIN32       
    //rdamtv_app_set_brightness(g_analogtv_cntx.analogtv_setting.yuv_brightness );
#endif
}

void color_button_up_handle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
 
//	uint32 base_addr;
	//uint16 y,u,v;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
	g_analogtv_cntx.analogtv_setting.yuv_color=(g_analogtv_cntx.analogtv_setting.yuv_color+1)%3;
	__analogtv_debug_trace("[ANALOGTV]Line: [%d] ->>color_button_up_handle", __LINE__);	

#ifndef __RDAMTV__
	switch(g_analogtv_cntx.analogtv_setting.yuv_color)
	{
		case  NORMAL_LEVEL:
			g_analogtv_UI.color_depth->status=NORMAL;
		break;
		case  WEAK_LEVEL:
			g_analogtv_UI.color_depth->status=PRESSED;
		break;
		case  HIGH_LEVEL:
			g_analogtv_UI.color_depth->status=RELEASED;
		break;
		default:
			break;
	}
#endif

	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
	gdi_image_draw_id(g_analogtv_UI.color_depth->x,g_analogtv_UI.color_depth->y,g_analogtv_UI.color_depth->image_id[g_analogtv_UI.color_depth->status]);
	gdi_layer_pop_and_restore_active();
	mmi_analogtv_blt_buttonarea_screen();
	
}
BOOL mmi_analogtv_check_and_restore_valid_drv(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 ret_drv;
    S8 buf[64];
    S8 UCS2_buf[128];
    FS_HANDLE file_handle;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	__analogtv_debug_trace("[ANALOGTV]Line: [%d] ->>mmi_analogtv_check_and_restore_valid_drv", __LINE__);	
    /* Get storage location */
    if (g_analogtv_cntx.analogtv_setting.storage== ANALOGTV_STORAGE_PHONE)
    {
        /* path is phone */
        ret_drv = MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);
    }
    else if (g_analogtv_cntx.analogtv_setting.storage == ANALOGTV_STORAGE_CARD)
    {
        /* path is memory card */
        ret_drv = MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
    }
    else
    {
        ASSERT(0);
        return FALSE;
    }
    if (ret_drv >= 0)
    {
        /* drive available */
	 sprintf(buf, "%c:\\", (U8) ret_drv);
        AnsiiToUnicodeString(UCS2_buf, buf);

        file_handle = MMI_FS_Open((UINT8*) UCS2_buf, FS_OPEN_DIR | FS_READ_ONLY);
        /* Memory card not available */
        if (file_handle >= 0)
        {
            FS_Close(file_handle);
        }
        else
        {
            /* card not available error */
            g_analogtv_cntx.analogtv_setting.storage = ANALOGTV_STORAGE_PHONE;

            /* write back storage */
    		mmi_analogtv_save_setting();

            return FALSE;
        }
    }
    else
    {
        /* drive not available error */
        g_analogtv_cntx.analogtv_setting.storage = ANALOGTV_STORAGE_PHONE;

        /* write back storage */
    	mmi_analogtv_save_setting();

        return FALSE;
    }

    return TRUE;
}

#if defined(__MMI_ANALOGTV_SNAPSHOT__)

static void mmi_analogtv_get_next_filename(U8 file_type, PS8 file_buf, PS8 file_path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 buffer[25];
    S8 UCS2_buffer[50];
    S8 UCS2_testname[50];
    S8 UCS2_alpha[10];
    U16 alpha_count;
    U16 alpha_index;
    FS_HANDLE file_handle;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* get/store sequentail number */

    if (g_analogtv_cntx.analogtv_setting.filename_seq_no == 0xff)
    {
        g_analogtv_cntx.analogtv_setting.filename_seq_no = 1;    /* init value */
    }
    else
    {
        /* TODO: if seq no turn around, add some protection */
        g_analogtv_cntx.analogtv_setting.filename_seq_no++;
        if (g_analogtv_cntx.analogtv_setting.filename_seq_no == 0xff) /* more than 4 digits */
        {
            g_analogtv_cntx.analogtv_setting.filename_seq_no = 1;
        }
    }
    mmi_analogtv_save_setting();
	
    alpha_count = 'Z' - 'A' + 1;
    alpha_index = 0;

    if(file_type == ANALOGTV_FILE_TYPE_JPG)
    	sprintf((PS8) buffer, "TVP%04d", g_analogtv_cntx.analogtv_setting.filename_seq_no); 
    else if(file_type == ANALOGTV_FILE_TYPE_3GP)
    	sprintf((PS8) buffer, "TVR%04d", g_analogtv_cntx.analogtv_setting.filename_seq_no); 
		
    AnsiiToUnicodeString((PS8) UCS2_buffer, (PS8) buffer);

    while (1)
    {
        sprintf((PS8) buffer, "%c", 'A' + alpha_index);
        AnsiiToUnicodeString((PS8) UCS2_alpha, (PS8) buffer);

        pfnUnicodeStrcpy(UCS2_testname, UCS2_buffer);
        pfnUnicodeStrcat(UCS2_testname, UCS2_alpha);
		
    	if(file_type == ANALOGTV_FILE_TYPE_JPG)
        	pfnUnicodeStrcat(UCS2_testname, (PS8) L".jpg");
    	else if(file_type == ANALOGTV_FILE_TYPE_3GP)
        	pfnUnicodeStrcat(UCS2_testname, (PS8) L".3gp");

        pfnUnicodeStrncpy((PS8) file_buf, (PS8) file_path, FMGR_MAX_PATH_LEN);
        pfnUnicodeStrcat((PS8) file_buf, (PS8) UCS2_testname);

        file_handle = MMI_FS_Open((UINT8*) file_buf, FS_READ_ONLY);

        if (file_handle >= 0)
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

        /* exit condition - will get a XXXXZ.jpg as file name */
        if (alpha_index >= alpha_count - 1)
        {
            return;
        }
    }
}


/*****************************************************************************
* FUNCTION
*     mmi_analogtv_snapshot
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
static BOOL mmi_analogtv_snapshot(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 buf_pathname[FMGR_PATH_BUFFER_SIZE];
    S8 buf_filename[FMGR_PATH_BUFFER_SIZE];
    MDI_RESULT ret = -1;
    MDI_RESULT snap_ret = -1;
    S32 create_result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	__analogtv_debug_trace("[ANALOGTV]Line: [%d] ->>mmi_analogtv_snapshot", __LINE__);
   
    memset(buf_pathname, 0, FMGR_PATH_BUFFER_SIZE);

    /* init value */
    ret = 0;
   
    /* check if storage path is valid or not */
    if (mmi_analogtv_check_and_restore_valid_drv())
    {
        /* drv is valid, create directory */
//        mmi_imgview_get_storage_file_path(buf_pathname);
        create_result =  -1;//mmi_imgview_create_file_dir(buf_pathname);

        /* create directory failed */
        if (create_result == FS_WRITE_PROTECTION)
        {
            g_analogtv_cntx.last_error = ANALOGTV_ERR_WRITE_PROTECT;

            /* has error - restore to phone */
            g_analogtv_cntx.analogtv_setting.storage = ANALOGTV_STORAGE_PHONE;
        }
        else if (create_result == FS_DISK_FULL)
        {
            if (g_analogtv_cntx.analogtv_setting.storage == ANALOGTV_STORAGE_PHONE)
            {
                g_analogtv_cntx.last_error = ANALOGTV_ERROR_DISK_FULL;
            }
            else
            {
                /* storage is card */
                g_analogtv_cntx.last_error = ANALOGTV_ERROR_DISK_FULL;

                /* has error - restore to phone */
                g_analogtv_cntx.analogtv_setting.storage = ANALOGTV_STORAGE_PHONE;
            }
        }
        else if (create_result == FS_ROOT_DIR_FULL)
        {
            if (g_analogtv_cntx.analogtv_setting.storage == ANALOGTV_STORAGE_PHONE)
            {
                g_analogtv_cntx.last_error = ANALOGTV_ERR_ROOT_DIR_FULL;
            }
            else
            {
                /* storage is card */
                g_analogtv_cntx.last_error = ANALOGTV_ERROR_DISK_FULL;

                /* has error - restore to phone */
                g_analogtv_cntx.analogtv_setting.storage= ANALOGTV_STORAGE_PHONE;
            }
        }

    }
    else
    {
        /* drv no valid, means card is plugged out (or phone FAT corrupted) */
        if (g_analogtv_cntx.analogtv_setting.storage == ANALOGTV_STORAGE_PHONE)
        {
            g_analogtv_cntx.last_error = ANALOGTV_ERR_CORRUPT_FAT;
        }
        else
        {
            g_analogtv_cntx.last_error = ANALOGTV_ERR_NO_DISK;
        }
	  return FALSE;

    }

	 /* 
         * if record finish has popup, it will not entry saving directly, 
         * after popup gone, we have to enter saving procedure 
         */
  

    /* start snapshot */
    mmi_analogtv_get_next_filename(ANALOGTV_FILE_TYPE_JPG, buf_filename, buf_pathname);
    if (g_analogtv_cntx.app_state == ANALOGTV_STATE_PREVIEW)
    {
        /* stop */
        mmi_analogtv_preview_stop();
        
        snap_ret = mdi_camera_encode_layer_to_jpeg(g_analogtv_UI.preview_layer_handle, buf_filename);
        //kal_prompt_trace(MOD_MMI,"mdi_camera_encode_layer_to_jpeg:snap_ret=%d",snap_ret);
	    mmi_analogtv_preview_start();
    }
	
	if((snap_ret == 0)&&(ret == 0))
		return TRUE;
	else
	{
		g_analogtv_cntx.error_code = snap_ret;
		return FALSE;
	}
}
/*****************************************************************************
* FUNCTION
*     mmi_ATV_player_entry_snap
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
static void snap_button_down_handle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
 
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	//S8 buf_pathname[FMGR_PATH_BUFFER_SIZE];
	__analogtv_debug_trace("[ANALOGTV]Line: [%d] ->>snap_button_down_handle", __LINE__);
	if(g_analogtv_cntx.channel_list[g_analogtv_cntx.analogtv_setting.cur_channel.cur_index].Present== FALSE)
		return;
	#ifdef __USB_IN_NORMAL_MODE__
	if (mmi_usb_is_in_mass_storage_mode())
	{
		//if (mmi_usb_check_path_exported((PS8)buf_pathname))
		{
		    mmi_usb_app_unavailable_popup(0); 
		    return;
		}
	}
	#endif /* __USB_IN_NORMAL_MODE__ */ 
        
	ClearKeyHandler(KEY_POUND, KEY_EVENT_DOWN);
	if(mmi_analogtv_snapshot()== TRUE)
	{
	        
		SetKeyHandler( snap_button_down_handle, KEY_POUND, KEY_EVENT_DOWN);
             DisplayPopup((U8*) GetString(STR_GLOBAL_SNAPSHOTTING),IMG_GLOBAL_OK,1,500,(U8)SAVE_TONE);   
	}
	else
	{
		mmi_analogtv_display_pop_error(g_analogtv_cntx.error_code);
	}
}
#endif

void mmi_analogtv_display_pop_error(S16 error_code)
{

	__analogtv_debug_trace("[ANALOGTV]Line: [%d] ->>mmi_analogtv_display_pop_error", __LINE__);
	switch(error_code)
	{
		case MDI_RES_CAMERA_ERR_DISK_FULL:
		//case ANALOGTV_ERROR_DISK_FULL:
			DisplayPopup( (PU8)GetString(  FMGR_FS_DISK_FULL_TEXT ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
			break;
		/*	
		case ANALOGTV_ERROR_PLAY:
			DisplayPopup( (PU8)GetString(  STR_GLOBAL_ERROR ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
			break;
		case ANALOGTV_ERROR_DISK:
			DisplayPopup( (PU8)GetString(  STR_GLOBAL_ERROR ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
			break;
		*/
		default :
			DisplayPopup( (PU8)GetString(  STR_GLOBAL_ERROR ), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
			break;
	}
}
void record_button_down_handle(void)
{
	__analogtv_debug_trace("[ANALOGTV]Line: [%d] ->>record_button_down_handle", __LINE__);
	g_analogtv_UI.record->status=PRESSED;
	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
	//vol
	gdi_image_draw_id(g_analogtv_UI.record->x,g_analogtv_UI.record->y,g_analogtv_UI.record->image_id[g_analogtv_UI.record->status]);
	gdi_layer_pop_and_restore_active();
	mmi_analogtv_blt_buttonarea_screen();
	
}

void record_button_up_handle(void)
{
	//start record
}

void leftsfk_button_down_handle(void)
{
}

void rightsfk_button_down_handle(void)
{
}

void leftsfk_button_up_handle(void)
{
	mmi_analogtv_entry_option_screen();
}

void rightsfk_button_up_handle(void)
{
	GoBackHistory();

}

void preview_playwnd_up_handle(void)
{
	g_analogtv_cntx.set_state = ANALOGTV_STATE_FULL_SCR_PLAY;
   // ANALOGTV_ENTER_STATE(g_analogtv_cntx.set_state);
	mmi_analogtv_entry_preview_screen();
}
#if 0
void mmi_analogtv_preview_touch_scr_pen_down_hdlr(mmi_pen_point_struct pos)
{

	if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.channel_pre))
	{
		g_analogtv_UI.touchobj=CHANNEL_PRE;
		channel_pre_button_down_handle();
		return;
	}
	if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.channel_next))
	{
		g_analogtv_UI.touchobj=CHANNEL_NEXT;
		channel_next_button_down_handle();
		return;
	}
	if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, &g_analogtv_UI.vol->vol_dec))
	{
		g_analogtv_UI.touchobj=VOL_DEC;
		volume_dec_button_down_handle();
		return;
	}

	if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, &g_analogtv_UI.vol->vol_inc))
	{

		g_analogtv_UI.touchobj=VOL_INC;
		volume_inc_button_down_handle();

		return;
	
	}

	#if 0
	if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.mychannel))
	{
		g_analogtv_UI.touchobj=MY_CHANNEL;
		mychannel_button_down_handle();
		return;
	}

	if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.brightness))
	{
	
		g_analogtv_UI.touchobj=TV_BRIGHTNESS;
		return;
	
	}
	if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.color_depth))
	{
		g_analogtv_UI.touchobj=TV_COLOR;		
		return;
	
	}
	if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.record))
	{
		g_analogtv_UI.touchobj=RECORD;
		record_button_down_handle();
		return;
	
	}
	#endif
	if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.lsk))
	{
		g_analogtv_UI.touchobj=LEFT_SOFTKEY;
		leftsfk_button_down_handle();
		
		return;
	
	}
	if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.rsk))
	{
		g_analogtv_UI.touchobj=RIGHT_SOFTKEY;
		rightsfk_button_down_handle();
		return;
	
	}

	if(mmi_analogtv_preview_hit_playwnd_tst(pos.x, pos.y, g_analogtv_UI.play_wnd))
	{
		g_analogtv_UI.touchobj=PREVIEW_WND;
		return;	
	}
	
	g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;

}

void mmi_analogtv_preview_touch_scr_pen_up_hdlr(mmi_pen_point_struct pos)
{
	if(g_analogtv_UI.touchobj==TOUCH_OBJ_NONE)
		return;

	switch(g_analogtv_UI.touchobj)
	{
		case CHANNEL_PRE:
		{
			//kal_prompt_trace(MOD_NVRAM,"-----CHANNEL_PRE up");
			g_analogtv_UI.channel_pre->is_pressed=FALSE;
			channel_pre_button_up_handle();
		}
		break;
		case CHANNEL_NEXT:
		{
			g_analogtv_UI.channel_next->is_pressed=FALSE;
			channel_next_button_up_handle();
		}
		break;
		case VOL_DEC:
		{
			g_analogtv_UI.vol->vol_dec.is_pressed=FALSE;
			volume_dec_button_up_handle();

		}
		break;
		case VOL_INC:
		{
			g_analogtv_UI.vol->vol_inc.is_pressed=FALSE;
			volume_inc_button_up_handle();

		}
		break;
		case MY_CHANNEL:
		{
			g_analogtv_UI.mychannel->is_pressed=FALSE;
			mychannel_button_up_handle();
		}
		break;
		case TV_BRIGHTNESS:
		{
			g_analogtv_UI.brightness->is_pressed=FALSE;
			brightness_button_up_handle();
		}
		break;
		case TV_COLOR:
		{
			g_analogtv_UI.color_depth->is_pressed=FALSE;
			color_button_up_handle();
		}
		break;

		case RECORD:
		{
			g_analogtv_UI.record->is_pressed=FALSE;
			record_button_up_handle();
		}
		break;
		case LEFT_SOFTKEY:
		{	
			g_analogtv_UI.lsk->is_pressed=FALSE;
			leftsfk_button_up_handle();
		}	
		break;
		case RIGHT_SOFTKEY:
		{
			g_analogtv_UI.rsk->is_pressed=FALSE;
			rightsfk_button_up_handle();
		}	
		break;
		case PREVIEW_WND:
		{
		#if !defined(__SCREEN_AUTO_ROTATE_SUPPORT__) 
			preview_playwnd_up_handle();
		#endif
		}	
		break;
		default:
		break;
	}
}

void mmi_analogtv_preview_touch_scr_pen_move_hdlr(mmi_pen_point_struct pos)
{
	if(g_analogtv_UI.touchobj==TOUCH_OBJ_NONE)
		return;
	
	switch(g_analogtv_UI.touchobj)
	{
		case CHANNEL_PRE:
		{
			if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.channel_pre))
			{
				g_analogtv_UI.channel_pre->is_pressed=FALSE;
				g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
				g_analogtv_UI.channel_pre->status=NORMAL;
				gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
			  	//channel pre
				gdi_image_draw_id(g_analogtv_UI.channel_pre->x,g_analogtv_UI.channel_pre->y,g_analogtv_UI.channel_pre->image_id[g_analogtv_UI.channel_pre->status]);
			 	gdi_layer_pop_and_restore_active();
				mmi_analogtv_blt_buttonarea_screen();

			}
		}
		case CHANNEL_NEXT:
		{
			if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.channel_next))
			{
				g_analogtv_UI.channel_next->is_pressed=FALSE;
				g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
				g_analogtv_UI.channel_next->status=NORMAL;
				gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
			  	//channel pre
				gdi_image_draw_id(g_analogtv_UI.channel_next->x,g_analogtv_UI.channel_next->y,g_analogtv_UI.channel_next->image_id[g_analogtv_UI.channel_next->status]);
			 	gdi_layer_pop_and_restore_active();
				mmi_analogtv_blt_buttonarea_screen();
			}
		}
		
		break;
		case VOL_DEC:
		{
			if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, &g_analogtv_UI.vol->vol_dec))
			{
				g_analogtv_UI.vol->vol_dec.is_pressed=FALSE;
				g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
				g_analogtv_UI.vol->vol_dec.status=NORMAL;
				gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
				gdi_image_draw_id(g_analogtv_UI.vol->vol_dec.x,g_analogtv_UI.vol->vol_dec.y,g_analogtv_UI.vol->vol_dec.image_id[g_analogtv_UI.vol->vol_dec.status]);
			 	gdi_layer_pop_and_restore_active();
				mmi_analogtv_blt_buttonarea_screen();
			}
		}
		break;
		case VOL_INC:
		{
			if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, &g_analogtv_UI.vol->vol_inc))
			{
				g_analogtv_UI.vol->vol_inc.is_pressed=FALSE;
				g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
				g_analogtv_UI.vol->vol_inc.status=NORMAL;
				gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
				gdi_image_draw_id(g_analogtv_UI.vol->vol_inc.x,g_analogtv_UI.vol->vol_inc.y,g_analogtv_UI.vol->vol_inc.image_id[g_analogtv_UI.vol->vol_inc.status]);
			 	gdi_layer_pop_and_restore_active();
				mmi_analogtv_blt_buttonarea_screen();
			}
		}
		break;
		case MY_CHANNEL:
		{
			if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.mychannel))
			{
				g_analogtv_UI.mychannel->is_pressed=FALSE;
				g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
				g_analogtv_UI.mychannel->status=NORMAL;
				gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
				//vol
				gdi_image_draw_id(g_analogtv_UI.mychannel->x,g_analogtv_UI.mychannel->y,g_analogtv_UI.mychannel->image_id[g_analogtv_UI.mychannel->status]);
				gdi_layer_pop_and_restore_active();
				mmi_analogtv_blt_buttonarea_screen();
			}
		}
		break;

		case TV_BRIGHTNESS:
		{
			if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.brightness))
			{
				g_analogtv_UI.brightness->is_pressed=FALSE;
				g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
			}
		}
		break;
		case TV_COLOR:
		{
			if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.color_depth))
			{
				g_analogtv_UI.color_depth->is_pressed=FALSE;
				g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
			}
		}
		break;
		case RECORD:
		{
			if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.record))
			{
				g_analogtv_UI.record->is_pressed=FALSE;
				g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
				g_analogtv_UI.record->status=NORMAL;
				gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);				
				gdi_image_draw_id(g_analogtv_UI.record->x,g_analogtv_UI.record->y,g_analogtv_UI.record->image_id[g_analogtv_UI.record->status]);
				gdi_layer_pop_and_restore_active();
				mmi_analogtv_blt_buttonarea_screen();

			}
		}
		break;
		case LEFT_SOFTKEY:
		{
			if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.lsk))
			{
				g_analogtv_UI.lsk->is_pressed=FALSE;
				g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
				g_analogtv_UI.lsk->status=NORMAL;
			}
		}	
		break;
		case RIGHT_SOFTKEY:
		{
			if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.rsk))
			{
				g_analogtv_UI.rsk->is_pressed=FALSE;
				g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
				g_analogtv_UI.rsk->status=NORMAL;
			}
		}	
		break;
		case PREVIEW_WND:
		{
			if(!mmi_analogtv_preview_hit_playwnd_tst(pos.x, pos.y, g_analogtv_UI.play_wnd))
			{
				g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
			}
		}
		break;
		default:
		break;
	}
}
#endif
void mmi_analogtv_preview_touch_scr_pen_down_hdlr(mmi_pen_point_struct pos)
{

    if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.channel_pre))
    {
        g_analogtv_UI.touchobj=CHANNEL_PRE;
        channel_pre_button_down_handle();
        return;
    }
    if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.channel_next))
    {
        g_analogtv_UI.touchobj=CHANNEL_NEXT;
        channel_next_button_down_handle();
        return;
    }
    if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, &g_analogtv_UI.vol->vol_dec))
    {
        g_analogtv_UI.touchobj=VOL_DEC;
        volume_dec_button_down_handle();
        return;
    }

    if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, &g_analogtv_UI.vol->vol_inc))
    {

        g_analogtv_UI.touchobj=VOL_INC;
        volume_inc_button_down_handle();

        return;

    }

    if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.lsk))
    {
        g_analogtv_UI.touchobj=LEFT_SOFTKEY;
        leftsfk_button_down_handle();

        return;

    }
    if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.rsk))
    {
        g_analogtv_UI.touchobj=RIGHT_SOFTKEY;
        rightsfk_button_down_handle();
        return;

    }

    if(mmi_analogtv_preview_hit_playwnd_tst(pos.x, pos.y, g_analogtv_UI.play_wnd))
    {
        g_analogtv_UI.touchobj=PREVIEW_WND;
        return;	
    }

    g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;

}

void mmi_analogtv_preview_touch_scr_pen_up_hdlr(mmi_pen_point_struct pos)
{
    if(g_analogtv_UI.touchobj==TOUCH_OBJ_NONE)
        return;

    switch(g_analogtv_UI.touchobj)
    {
        case CHANNEL_PRE:
        {
            g_analogtv_UI.channel_pre->is_pressed=FALSE;
            channel_pre_button_up_handle();
        }
        break;
        case CHANNEL_NEXT:
        {
            g_analogtv_UI.channel_next->is_pressed=FALSE;
            channel_next_button_up_handle();
        }
        break;
        case VOL_DEC:
        {
            g_analogtv_UI.vol->vol_dec.is_pressed=FALSE;
            volume_dec_button_up_handle();

        }
        break;
        case VOL_INC:
        {
            g_analogtv_UI.vol->vol_inc.is_pressed=FALSE;
            volume_inc_button_up_handle();

        }
        break;
        case MY_CHANNEL:
        {
            g_analogtv_UI.mychannel->is_pressed=FALSE;
            mychannel_button_up_handle();
        }
        break;
        #if 0
        case TV_BRIGHTNESS:
        {
            g_analogtv_UI.brightness->is_pressed=FALSE;
            brightness_button_up_handle();
        }
        break;
        #endif
        case TV_COLOR:
        {
            g_analogtv_UI.color_depth->is_pressed=FALSE;
            color_button_up_handle();
        }
        break;

        case RECORD:
        {
            g_analogtv_UI.record->is_pressed=FALSE;
            record_button_up_handle();
        }
        break;
        case LEFT_SOFTKEY:
        {
            g_analogtv_UI.lsk->is_pressed=FALSE;
            leftsfk_button_up_handle();
        }
        break;
        case RIGHT_SOFTKEY:
        {
            g_analogtv_UI.rsk->is_pressed=FALSE;
            rightsfk_button_up_handle();
        }
        break;
        case PREVIEW_WND:
        {
#if !defined(__SCREEN_AUTO_ROTATE_SUPPORT__) 
            preview_playwnd_up_handle();
#endif
        }
        break;
        default:
        break;
    }
}

void mmi_analogtv_preview_touch_scr_pen_move_hdlr(mmi_pen_point_struct pos)
{
    if(g_analogtv_UI.touchobj==TOUCH_OBJ_NONE)
        return;

    switch(g_analogtv_UI.touchobj)
    {
    case CHANNEL_PRE:
    {
        if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.channel_pre))
        {
            g_analogtv_UI.channel_pre->is_pressed=FALSE;
            g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
            g_analogtv_UI.channel_pre->status=NORMAL;
            gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
            //channel pre
            gdi_image_draw_id(g_analogtv_UI.channel_pre->x,g_analogtv_UI.channel_pre->y,g_analogtv_UI.channel_pre->image_id[g_analogtv_UI.channel_pre->status]);
            gdi_layer_pop_and_restore_active();
            mmi_analogtv_blt_buttonarea_screen();

        }
    }
    case CHANNEL_NEXT:
    {
        if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.channel_next))
        {
            g_analogtv_UI.channel_next->is_pressed=FALSE;
            g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
            g_analogtv_UI.channel_next->status=NORMAL;
            gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
            //channel pre
            gdi_image_draw_id(g_analogtv_UI.channel_next->x,g_analogtv_UI.channel_next->y,g_analogtv_UI.channel_next->image_id[g_analogtv_UI.channel_next->status]);
            gdi_layer_pop_and_restore_active();
            mmi_analogtv_blt_buttonarea_screen();
        }
    }

    break;
    case VOL_DEC:
    {
        if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, &g_analogtv_UI.vol->vol_dec))
        {
            g_analogtv_UI.vol->vol_dec.is_pressed=FALSE;
            g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
            g_analogtv_UI.vol->vol_dec.status=NORMAL;
            gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
            gdi_image_draw_id(g_analogtv_UI.vol->vol_dec.x,g_analogtv_UI.vol->vol_dec.y,g_analogtv_UI.vol->vol_dec.image_id[g_analogtv_UI.vol->vol_dec.status]);
            gdi_layer_pop_and_restore_active();
            mmi_analogtv_blt_buttonarea_screen();
        }
    }
    break;
    case VOL_INC:
    {
        if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, &g_analogtv_UI.vol->vol_inc))
        {
            g_analogtv_UI.vol->vol_inc.is_pressed=FALSE;
            g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
            g_analogtv_UI.vol->vol_inc.status=NORMAL;
            gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
            gdi_image_draw_id(g_analogtv_UI.vol->vol_inc.x,g_analogtv_UI.vol->vol_inc.y,g_analogtv_UI.vol->vol_inc.image_id[g_analogtv_UI.vol->vol_inc.status]);
            gdi_layer_pop_and_restore_active();
            mmi_analogtv_blt_buttonarea_screen();
        }
    }
    break;
    case MY_CHANNEL:
    {
        if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.mychannel))
        {
            g_analogtv_UI.mychannel->is_pressed=FALSE;
            g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
            g_analogtv_UI.mychannel->status=NORMAL;
            gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);
            //vol
            gdi_image_draw_id(g_analogtv_UI.mychannel->x,g_analogtv_UI.mychannel->y,g_analogtv_UI.mychannel->image_id[g_analogtv_UI.mychannel->status]);
            gdi_layer_pop_and_restore_active();
            mmi_analogtv_blt_buttonarea_screen();
        }
    }
    break;
#if 0
    case TV_BRIGHTNESS:
    {
        if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.brightness))
        {
            g_analogtv_UI.brightness->is_pressed=FALSE;
            g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
        }
    }
    break;
#endif
    case TV_COLOR:
    {
        if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.color_depth))
        {
            g_analogtv_UI.color_depth->is_pressed=FALSE;
            g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
        }
    }
    break;
    case RECORD:
    {
        if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.record))
        {
            g_analogtv_UI.record->is_pressed=FALSE;
            g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
            g_analogtv_UI.record->status=NORMAL;
            gdi_layer_push_and_set_active(g_analogtv_UI.base_layer_handle);				
            gdi_image_draw_id(g_analogtv_UI.record->x,g_analogtv_UI.record->y,g_analogtv_UI.record->image_id[g_analogtv_UI.record->status]);
            gdi_layer_pop_and_restore_active();
            mmi_analogtv_blt_buttonarea_screen();

        }
    }
    break;
    case LEFT_SOFTKEY:
    {
        if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.lsk))
        {
            g_analogtv_UI.lsk->is_pressed=FALSE;
            g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
            g_analogtv_UI.lsk->status=NORMAL;
        }
    }
    break;
    case RIGHT_SOFTKEY:
    {
        if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.rsk))
        {
            g_analogtv_UI.rsk->is_pressed=FALSE;
            g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
            g_analogtv_UI.rsk->status=NORMAL;
        }
    }
    break;
    case PREVIEW_WND:
    {
        if(!mmi_analogtv_preview_hit_playwnd_tst(pos.x, pos.y, g_analogtv_UI.play_wnd))
        {
            g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
        }
    }
    break;
    default:
    break;
    }
}
static void mmi_analogtv_entry_preview_state(void)
{
    mmi_analogtv_preview_set_key_hdlr();
    //touch
#if defined(__MMI_TOUCH_SCREEN__)
    wgui_register_pen_down_handler(mmi_analogtv_preview_touch_scr_pen_down_hdlr);
    wgui_register_pen_up_handler(mmi_analogtv_preview_touch_scr_pen_up_hdlr);
    wgui_register_pen_move_handler(mmi_analogtv_preview_touch_scr_pen_move_hdlr);
#endif


#if defined(MMI_ON_HARDWARE_P)	
    if(g_analogtv_cntx.analogtv_setting.is_mychannel)
    {
        atvd_SetChannel(g_analogtv_cntx.analogtv_setting.cur_mychannel.channel.frequency);
    }
    else
    {
        atvd_SetChannel(g_analogtv_cntx.analogtv_setting.cur_channel.channel.frequency);
    }
#endif
#if defined(MMI_ON_HARDWARE_P)
    mmi_analogtv_preview_start();
    COS_Sleep(50);
    mdi_audio_set_volume(AUD_VOLUME_TV,0);
    mdi_audio_set_volume(AUD_VOLUME_TV,g_analogtv_cntx.volume);
#endif
}


/*****************************************************************************
 * FUNCTION
 *  mmi_analogtv_exit_preview_state
 * DESCRIPTION
 *  exit preview state
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_analogtv_exit_preview_state(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* exit app */
    if (g_analogtv_cntx.app_state !=ANALOGTV_STATE_FULL_SCR_PLAY )
    {
        /* stop previewing */
        mmi_analogtv_preview_stop();
    }
    else
	mmi_analogtv_fullscreen_preview_stop();
}
/*****************************************************************************
 * FUNCTION
 *  mmi_camera_preview_stop
 * DESCRIPTION
 *  stop previewing of main lcd
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_analogtv_preview_start(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    mdi_camera_preview_struct camera_preview_data;

    mdi_camera_setting_struct camera_setting_data;
    GDI_HANDLE preview_layer_handle = 0;
    U32 preview_layer_flag = 0;
    U32 blt_layer_flag = 0;
    S32 preview_wnd_offset_x = 0;
    S32 preview_wnd_offset_y = 0;
    U16 preview_wnd_width = 0;
    U16 preview_wnd_height = 0;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/


    /* preview parametes */
    camera_setting_data.zoom = 0;
    camera_setting_data.wb = 0;
    //camera_setting_data.manual_wb = FALSE;
    camera_setting_data.ev = 0;
    camera_setting_data.banding = 0;
    camera_setting_data.flash = 0;
    camera_setting_data.osd_rotate = g_analogtv_cntx.osd_rotate;
    camera_setting_data.night = 0;
    camera_setting_data.iso = 0;
    camera_setting_data.ae_meter = 0;


    /* default effect */
    camera_setting_data.effect = 0;
    camera_setting_data.brightness = 128;
    camera_setting_data.saturation = 128;
    camera_setting_data.contrast = 128;
    camera_setting_data.hue = 0;

    camera_setting_data.preview_width = g_analogtv_UI.preview_width;
    camera_setting_data.preview_height = g_analogtv_UI.preview_height;

    /* capture parameters */
    camera_setting_data.image_width = g_analogtv_UI.preview_width;
    camera_setting_data.image_height = g_analogtv_UI.preview_height;

    camera_setting_data.lcm = MDI_CAMERA_PREVIEW_LCM_MAINLCD;
    camera_setting_data.preview_rotate = MDI_CAMERA_PREVIEW_ROTATE_0;

    camera_setting_data.lcm_rotate =g_analogtv_UI.lcm_rotate;//MDI_CAMERA_LCM_ROTATE_0;

    preview_layer_handle = g_analogtv_UI.preview_layer_handle;
    /* use preview wnd's position */
    preview_wnd_offset_x = 	0;//g_analogtv_cntx.active_camera_osd_ptr->preview_wnd.offset_x;
    preview_wnd_offset_y = 	0;//g_analogtv_cntx.active_camera_osd_ptr->preview_wnd.offset_y;
    preview_wnd_width = 	g_analogtv_UI.preview_width;
    preview_wnd_height = g_analogtv_UI.preview_height;

    /* preview (hw), OSD */
    blt_layer_flag = GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1|GDI_LAYER_ENABLE_LAYER_2;
    preview_layer_flag = GDI_LAYER_ENABLE_LAYER_1;

    /* preview data for preview start */
    camera_preview_data.preview_layer_handle = g_analogtv_UI.preview_layer_handle;
    camera_preview_data.preview_wnd_offset_x = 0;
    camera_preview_data.preview_wnd_offset_y = 0;
    camera_preview_data.preview_wnd_width = g_analogtv_UI.preview_width;
    camera_preview_data.preview_wnd_height = g_analogtv_UI.preview_height;
    camera_preview_data.blt_layer_flag = GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1|GDI_LAYER_ENABLE_LAYER_2;
    camera_preview_data.preview_layer_flag = GDI_LAYER_ENABLE_LAYER_1;
    //camera_preview_data.is_lcd_update = TRUE;
    camera_preview_data.src_key_color = GDI_COLOR_TRANSPARENT;
    //camera_preview_data.is_tvout = TRUE;
    mdi_camera_preview_start(preview_layer_handle,
        camera_preview_data.preview_wnd_offset_x,
        camera_preview_data.preview_wnd_offset_y,
        camera_preview_data.preview_wnd_width,
        camera_preview_data.preview_wnd_height,
        blt_layer_flag,
        preview_layer_flag,
        GDI_COLOR_BLACK,
        TRUE,
        &camera_setting_data);

}


/*****************************************************************************
 * FUNCTION
 *  mmi_analogtv_preview_stop
 * DESCRIPTION
 *  stop previewing of main lcd
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_analogtv_preview_stop(void)
{
    mdi_camera_preview_stop();
}

static void mmi_analogtv_fullscreen_preview(void)
{
    mdi_camera_preview_struct camera_preview_data;

    mdi_camera_setting_struct camera_setting_data;
    GDI_HANDLE preview_layer_handle = 0;
    U32 preview_layer_flag = 0;
    U32 blt_layer_flag = 0;
    S32 preview_wnd_offset_x = 0;
    S32 preview_wnd_offset_y = 0;
    U16 preview_wnd_width = 0;
    U16 preview_wnd_height = 0;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    camera_setting_data.zoom = 0;
    camera_setting_data.wb = 0;
    //camera_setting_data.manual_wb = FALSE;
    camera_setting_data.ev = 0;
    camera_setting_data.banding = 0;
    camera_setting_data.flash = 0;
    camera_setting_data.osd_rotate = g_analogtv_cntx.osd_rotate;
    camera_setting_data.night = 0;
    camera_setting_data.iso = 0;
    camera_setting_data.ae_meter = 0;


    /* default effect */
    camera_setting_data.effect = 0;
    camera_setting_data.brightness = 128;
    camera_setting_data.saturation = 128;
    camera_setting_data.contrast = 128;
    camera_setting_data.hue = 0;


    camera_setting_data.preview_width =  UI_DEVICE_WIDTH ; //g_analogtv_UI.preview_width;
    camera_setting_data.preview_height = UI_DEVICE_HEIGHT ;//g_analogtv_UI.preview_height;

    /* capture parameters */
    camera_setting_data.image_width = g_analogtv_UI.preview_width;
    camera_setting_data.image_height = g_analogtv_UI.preview_height;

    camera_setting_data.lcm = MDI_CAMERA_PREVIEW_LCM_MAINLCD;
    camera_setting_data.preview_rotate = MDI_CAMERA_PREVIEW_ROTATE_0;

    camera_setting_data.lcm_rotate =g_analogtv_UI.lcm_rotate;//MDI_CAMERA_LCM_ROTATE_0;

    //camera_setting_data.continue_capture = 0;

    preview_layer_handle = g_analogtv_UI.preview_layer_handle;
    /* use preview wnd's position */
    preview_wnd_offset_x = 	0;//g_analogtv_cntx.active_camera_osd_ptr->preview_wnd.offset_x;
    preview_wnd_offset_y = 	0;//g_analogtv_cntx.active_camera_osd_ptr->preview_wnd.offset_y;
    preview_wnd_width = 	g_analogtv_UI.preview_width;
    preview_wnd_height = g_analogtv_UI.preview_height;

    /* preview (hw), OSD */
    blt_layer_flag = GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1|GDI_LAYER_ENABLE_LAYER_2;
    preview_layer_flag = GDI_LAYER_ENABLE_LAYER_1;

    /* preview data for preview start */
    camera_preview_data.preview_layer_handle = g_analogtv_UI.preview_layer_handle;
    camera_preview_data.preview_wnd_offset_x = 0;
    camera_preview_data.preview_wnd_offset_y = 0;
    camera_preview_data.preview_wnd_width = g_analogtv_UI.preview_width;
    camera_preview_data.preview_wnd_height = g_analogtv_UI.preview_height;
    camera_preview_data.blt_layer_flag = GDI_LAYER_ENABLE_LAYER_0|GDI_LAYER_ENABLE_LAYER_1|GDI_LAYER_ENABLE_LAYER_2;
    camera_preview_data.preview_layer_flag = GDI_LAYER_ENABLE_LAYER_1;
    //camera_preview_data.is_lcd_update = TRUE;
    camera_preview_data.src_key_color = GDI_COLOR_TRANSPARENT;
    //camera_preview_data.is_tvout = TRUE;
    mdi_camera_preview_start(preview_layer_handle,
        camera_preview_data.preview_wnd_offset_x,
        camera_preview_data.preview_wnd_offset_y,
        camera_preview_data.preview_wnd_width,
        camera_preview_data.preview_wnd_height,
        blt_layer_flag,
        preview_layer_flag,
        GDI_COLOR_BLACK,
        TRUE,
        &camera_setting_data);
}

static void mmi_analogtv_fullscreen_preview_stop(void)
{
    mdi_camera_preview_stop();

}

/*****************************************************************************
 * FUNCTION
 *  mmi_analogtv_inc_volume
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_analogtv_inc_volume(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_analogtv_cntx.volume < MAX_VOL_LEVEL-1)
    {
        g_analogtv_cntx.volume++;
        mdi_audio_set_volume(AUD_VOLUME_TV,g_analogtv_cntx.volume+1);
        g_analogtv_UI.vol->level=g_analogtv_cntx.volume;
        if (!analogtv_is_full_screen())
        {
            gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
            gdi_image_draw_id(g_analogtv_UI.vol->vol_inc.x,g_analogtv_UI.vol->vol_inc.y,g_analogtv_UI.vol->vol_inc.image_id[g_analogtv_UI.vol->vol_inc.status]);
            gdi_image_draw_id(g_analogtv_UI.vol->vol_indication.x,g_analogtv_UI.vol->vol_indication.y,g_analogtv_UI.vol->level_imgid[g_analogtv_UI.vol->level]);
            gdi_layer_pop_and_restore_active();
            mmi_analogtv_blt_buttonarea_screen();
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_analogtv_dec_volume
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_analogtv_dec_volume(void)
{
/*----------------------------------------------------------------*/
/* Local Variables                                                */
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/* Code Body                                                      */
/*----------------------------------------------------------------*/

    if (g_analogtv_cntx.volume > 0)
    {
        g_analogtv_cntx.volume--;
        mdi_audio_set_volume(AUD_VOLUME_TV,g_analogtv_cntx.volume);
        g_analogtv_UI.vol->level= g_analogtv_cntx.volume;
        if (!analogtv_is_full_screen())
        {
            gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
            gdi_image_draw_id(g_analogtv_UI.vol->vol_dec.x,g_analogtv_UI.vol->vol_dec.y,g_analogtv_UI.vol->vol_dec.image_id[g_analogtv_UI.vol->vol_dec.status]);
            gdi_image_draw_id(g_analogtv_UI.vol->vol_indication.x,g_analogtv_UI.vol->vol_indication.y,g_analogtv_UI.vol->level_imgid[g_analogtv_UI.vol->level]);
            gdi_layer_pop_and_restore_active();
            mmi_analogtv_blt_buttonarea_screen();
        }
    }

}
//fanhuaran

void mmi_analogtv_preview_show_controlpannel(void)
{
    gdi_layer_lock_frame_buffer();
    mmi_analogtv_preview_show_pannel_background();
    mmi_analogtv_show_open_str();
    mmi_analogtv_preview_show_button();
    mmi_analogtv_show_softkey();
#if defined(__MMI_ANALOGTV_SHOW_CHANNEL_NUMBER__)
    mmi_analogtv_show_channel_number();
#endif
    gdi_layer_unlock_frame_buffer();
    mmi_analogtv_blt_screen();
}

void mmi_analogtv_entry_option_screen(void)
{
    U16 nStrItemList[MAX_SUB_MENUS];
    U16 nNumofItem;
    U8 *guiBuffer;

    U16 SettingsSubmenuImageList[MAX_SUB_MENUS];

    EntryNewScreen(SCR_ID_ANALOGTV_OPTION, NULL, mmi_analogtv_entry_option_screen, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_OPTION);
    nNumofItem = GetNumOfChild(MENU_ID_ANALOGTV_OPTION);
    GetSequenceStringIds(MENU_ID_ANALOGTV_OPTION, nStrItemList);
    GetSequenceImageIds(MENU_ID_ANALOGTV_OPTION, SettingsSubmenuImageList);
    SetParentHandler(MENU_ID_ANALOGTV_OPTION);
    ClearAllKeyHandler();
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
#ifdef __MMI_WGUI_MINI_TAB_BAR__
    wgui_enable_mini_tab_bar(MENU_ID_ANALOGTV_OPTION);
#endif 
    ShowCategory15Screen(
        STR_GLOBAL_OPTIONS,
        0,
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nNumofItem,
        nStrItemList,
        (U16*)gIndexIconsImageList,
        LIST_MENU,
        0,
        guiBuffer);

    SetRightSoftkeyFunction(mmi_analogtv_entry_preview_screen, KEY_EVENT_UP);
}

void mmi_analogtv_preview_show_pannel_background(void)
{
        
    gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
    gdi_layer_push_clip();
    gdi_layer_reset_clip();
    gdi_layer_set_clip(0,g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height,
                                    UI_DEVICE_WIDTH,UI_DEVICE_HEIGHT);

    if (g_analogtv_UI.bg_pannel->image_id !=0)
        gdi_image_draw_id(g_analogtv_UI.bg_pannel->x,g_analogtv_UI.bg_pannel->y,g_analogtv_UI.bg_pannel->image_id);
    gdi_layer_pop_clip();
    gdi_layer_pop_and_restore_active();
}


void mmi_analogtv_preview_show_button(void)
{

    gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);

    gdi_image_draw_id(g_analogtv_UI.channel_pre->x,g_analogtv_UI.channel_pre->y,g_analogtv_UI.channel_pre->image_id[g_analogtv_UI.channel_pre->status]);

    //channel next
    gdi_image_draw_id(g_analogtv_UI.channel_next->x,g_analogtv_UI.channel_next->y,g_analogtv_UI.channel_next->image_id[g_analogtv_UI.channel_next->status]);
    //vol
    gdi_image_draw_id(g_analogtv_UI.vol->vol_dec.x,g_analogtv_UI.vol->vol_dec.y,g_analogtv_UI.vol->vol_dec.image_id[g_analogtv_UI.vol->vol_dec.status]);
    gdi_image_draw_id(g_analogtv_UI.vol->vol_inc.x,g_analogtv_UI.vol->vol_inc.y,g_analogtv_UI.vol->vol_inc.image_id[g_analogtv_UI.vol->vol_inc.status]);
    gdi_image_draw_id(g_analogtv_UI.vol->vol_indication.x,g_analogtv_UI.vol->vol_indication.y,g_analogtv_UI.vol->level_imgid[g_analogtv_UI.vol->level]);
#ifndef LCDSIZE_220_176
    gdi_image_draw_id(79,153,g_analogtv_UI.brightness->level_imgid[(U8)(g_analogtv_cntx.analogtv_setting.yuv_brightness)]);
#else
    gdi_image_draw_id(99,120,g_analogtv_UI.brightness->level_imgid[(U8)(g_analogtv_cntx.analogtv_setting.yuv_brightness)]);
#endif
#if 0
    //my channel
    gdi_image_draw_id(g_analogtv_UI.mychannel->x,g_analogtv_UI.mychannel->y,g_analogtv_UI.mychannel->image_id[g_analogtv_UI.mychannel->status]);
    //brightness

    gdi_image_draw_id(g_analogtv_UI.brightness->x,g_analogtv_UI.brightness->y,g_analogtv_UI.brightness->image_id[g_analogtv_UI.brightness->status]);

    //color depth
    gdi_image_draw_id(g_analogtv_UI.color_depth->x,g_analogtv_UI.color_depth->y,g_analogtv_UI.color_depth->image_id[g_analogtv_UI.color_depth->status]);
    //record 
    gdi_image_draw_id(g_analogtv_UI.record->x,g_analogtv_UI.record->y,g_analogtv_UI.record->image_id[g_analogtv_UI.record->status]);
#endif
    gdi_layer_pop_and_restore_active();
}

void mmi_analogtv_blt_screen(void)
{
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}

void mmi_analogtv_blt_buttonarea_screen(void)
{
#if 0        
        gdi_layer_blt(g_analogtv_UI.osd_layer_handle,0,0,0,
                                g_analogtv_UI.button_layer_x,
                                g_analogtv_UI.button_layer_y, 
                                g_analogtv_UI.button_layer_x + g_analogtv_UI.button_layer_width,
                                g_analogtv_UI.button_layer_y+ g_analogtv_UI.button_layer_width);
#else
        gdi_layer_blt_previous(
                                g_analogtv_UI.button_layer_x,
                                g_analogtv_UI.button_layer_y, 
                                g_analogtv_UI.button_layer_x + g_analogtv_UI.button_layer_width,
                                g_analogtv_UI.button_layer_y+ g_analogtv_UI.button_layer_width);
#endif
}

void mmi_analogtv_blt_searchprogress_screen(void)
{
        U16 x,y;

        x = g_analogtv_cntx.index;
        y =g_analogtv_cntx.current_area_chn_count;

        //offset
        x = x*g_analogtv_UI.bg_pannel->width/y ;



        gdi_layer_lock_frame_buffer();

        gdi_draw_solid_rect(
                        g_analogtv_UI.bg_pannel->x,
                        g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height+4,
                        g_analogtv_UI.bg_pannel->x +x,
                        g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height + 9,
                        GDI_COLOR_GREEN);

        gdi_layer_unlock_frame_buffer();

#if 0
        gdi_layer_blt(g_analogtv_UI.osd_layer_handle,0,0,0,
                        g_analogtv_UI.bg_pannel->x,
                        g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height+1,
                        g_analogtv_UI.bg_pannel->x +g_analogtv_UI.bg_pannel->width,
                        g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height + 12
                        );  //add for tv search progressbar 20071225	
#else
        gdi_layer_blt_previous(
        g_analogtv_UI.bg_pannel->x,
        g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height+1,
        g_analogtv_UI.bg_pannel->x +g_analogtv_UI.bg_pannel->width,
        g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height + 12
        );  //add for tv search progressbar 20071225	
#endif
}

void mmi_analogtv_show_auto_search_progress(void)
{
        U16 x=0;
        U16 y=0;
        U8 tmp_str[20] = {0x00};
        U16 ucs2_str[10] = {0x00};

        x = g_analogtv_cntx.index;
        y = g_analogtv_cntx.current_area_chn_count;

        x = x*g_analogtv_UI.bg_pannel->width/y ;
        
        lcd_SemTake();

        gdi_layer_lock_frame_buffer();
        gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
        mmi_analogtv_preview_show_autosearch_pannel();
        mmi_analogtv_refresh_signal_levels(atvd_GetSignalLevel());
        gdi_draw_solid_rect(
                        g_analogtv_UI.bg_pannel->x,
                        g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height+4,
                        g_analogtv_UI.bg_pannel->x +x,
                        g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height + 9,
                        GDI_COLOR_GREEN);

        //draw x%100 str
        x = g_analogtv_cntx.index*100;
        x = x/g_analogtv_cntx.current_area_chn_count;

        //refresh x%100
        gui_reset_text_clip();
        gui_move_text_cursor((UI_device_width>>1) -24, 
        g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height + 9+3);

        sprintf((char *)tmp_str, "%02d/100",x);
        AnsiiToUnicodeString((S8 *) ucs2_str, (S8 *) tmp_str);
        gui_print_bordered_text((UI_string_type)  ucs2_str );

        gdi_layer_pop_and_restore_active();
        gdi_layer_unlock_frame_buffer();
        
        lcd_SemFree();
        
}
/*****************************************************************************
* FUNCTION
*	mmi_analogtv_write_nvram_channel_list
* DESCRIPTION
*  write list from NVRAM
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
BOOL mmi_analogtv_refresh_signal_levels(U8 level) 
{
    //draw bg
        U16 bg_id = g_analogtv_UI.searchpannel->image_id;

        if (!analogtv_is_full_screen())
        {
            U8 offset = 13;
            if( g_analogtv_cntx.app_state!= ANALOGTV_STATE_PREVIEW_CHANNEL_AUTO_SEARCH)
            {
                offset = 0;
                bg_id = g_analogtv_UI.bg_pannel->image_id;
            }

            gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);
            gdi_layer_push_clip();
            gdi_layer_reset_clip();
            gdi_layer_set_clip(g_AnalogTV_Preview_Layout_skin.signal.x,
                            g_AnalogTV_Preview_Layout_skin.signal.y+offset,
                            g_AnalogTV_Preview_Layout_skin.signal.x+23-1,
                            g_AnalogTV_Preview_Layout_skin.signal.y+offset+16-1);



            gdi_image_draw_id(g_analogtv_UI.bg_pannel->x,
                            g_analogtv_UI.bg_pannel->y,
                            bg_id);

            gdi_image_draw_id(g_AnalogTV_Preview_Layout_skin.signal.x, 
                            g_AnalogTV_Preview_Layout_skin.signal.y+offset,
                            ( g_AnalogTV_Preview_Layout_skin.signal.image_id +level));

            gdi_layer_pop_clip();
            gdi_layer_pop_and_restore_active();
#if 0            
            gdi_layer_blt(g_analogtv_UI.osd_layer_handle,0,0,0,
                            g_AnalogTV_Preview_Layout_skin.signal.x,
                            g_AnalogTV_Preview_Layout_skin.signal.y+offset,
                            g_AnalogTV_Preview_Layout_skin.signal.x+23-1,
                            g_AnalogTV_Preview_Layout_skin.signal.y+offset+16-1);
#else
            gdi_layer_blt_previous(
                            g_AnalogTV_Preview_Layout_skin.signal.x,
                            g_AnalogTV_Preview_Layout_skin.signal.y+offset,
                            g_AnalogTV_Preview_Layout_skin.signal.x+23-1,
                            g_AnalogTV_Preview_Layout_skin.signal.y+offset+16-1);
#endif
        }

    return TRUE;
}

/*****************************************************************************
* FUNCTION
*	mmi_analogtv_write_nvram_channel_list
* DESCRIPTION
*  write list from NVRAM
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
static BOOL mmi_analogtv_write_nvram_channel_list(analogtv_channel_struct *channel_info,U16 index) 
{
        S16 ret, error_code;
        BOOL result;

        ret = WriteRecord( NVRAM_EF_ATV_PLAYER_CHANNEL_LID, index + 1, (void *)channel_info, NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE, &error_code);

        if (((error_code == NVRAM_WRITE_SUCCESS) && (ret == NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE)))
            result = TRUE;
        else
            result = FALSE;

        return result;
}
void channel_auto_search_function(void)
{
        U16 j=0;
        S8 LocalBuffer[16+1];
        S8 StrVal[16+1];
        S16 error_code;
        S32 length;

        if ((g_analogtv_cntx.is_rsk_pressed!=TRUE) && (g_analogtv_cntx.index<g_analogtv_cntx.current_area_chn_count))
        {
            if (atvd_ScanChannel(g_analogtv_cntx.index))
            {
                j = g_analogtv_cntx.analogtv_setting.cur_channel.cur_index;
                sprintf(LocalBuffer,"%d", j+1);
                AnsiiToUnicodeString((PS8)StrVal,(PS8)LocalBuffer);

                g_analogtv_cntx.channel_list[j].Present = TRUE;
                g_analogtv_cntx.channel_list[j].frequency = g_analogtv_cntx.index;
                //pfnUnicodeStrcpy((PS8)g_analogtv_cntx.channel_list[j].name, (PS8) GetString(STR_ID_CHANNEL_NAME));
                length = pfnUnicodeStrlen((const S8 *)GetString(STR_ID_CHANNEL_NAME));
                if(length > MAX_CH_NAME_LEN - 2)
                {
                    pfnUnicodeStrncpy((PS8)g_analogtv_cntx.channel_list[j].name, (PS8) GetString(STR_ID_CHANNEL_NAME), MAX_CH_NAME_LEN - 5);
                    pfnUnicodeStrcat((PS8) g_analogtv_cntx.channel_list[j].name,(PS8)L"...");
                }
                else
                {
                    pfnUnicodeStrcpy((PS8)g_analogtv_cntx.channel_list[j].name, (PS8) GetString(STR_ID_CHANNEL_NAME));
                    pfnUnicodeStrcat((PS8) g_analogtv_cntx.channel_list[j].name,(PS8)StrVal);
                }

                if (g_analogtv_cntx.analogtv_setting.cur_channel.cur_index < NVRAM_EF_ATV_PLAYER_CHANNEL_TOTAL)
                {
                    mmi_analogtv_write_nvram_channel_list(&g_analogtv_cntx.channel_list[j],j);

                    g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_LAST_CHANNEL_ID] = j;
                    j = ++g_analogtv_cntx.analogtv_setting.cur_channel.cur_index;

                    g_ATVPlayerNVRAMBuff[ATV_PLAYER_SETTING_TOTAL_CHANNEL_ID] = g_analogtv_cntx.analogtv_setting.total_channel = j;

                    WriteRecord( NVRAM_EF_ATV_PLAYER_SETTINGS_LID, 1, (void *)g_ATVPlayerNVRAMBuff, NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE, &error_code);
                }
            }

            if(g_analogtv_cntx.index > 0)
                g_analogtv_cntx.index2 = g_analogtv_UI.searchprogress->progress_tic.x + g_analogtv_cntx.index*(UI_device_width - 2*g_analogtv_UI.searchprogress->progress_tic.x)/g_analogtv_cntx.current_area_chn_count;

            g_analogtv_cntx.index++;

            mmi_analogtv_show_auto_search_progress();
            StartTimer(ANALOGTV_AUTO_SEARCH_TIMER, ANALOGTV_AUTO_SEARCH_TIME_DELAY, channel_auto_search_function);
            //StartTimer(ANALOGTV_AUTO_SEARCH_TIMER, ANALOGTV_AUTO_SEARCH_TIME_DELAY, channel_auto_search_function);
        }
        else
        {
            StopTimer(ANALOGTV_AUTO_SEARCH_TIMER);
            g_analogtv_cntx.is_rsk_pressed=FALSE;
            g_analogtv_cntx.analogtv_setting.total_channel=g_analogtv_cntx.analogtv_setting.cur_channel.cur_index;
            g_analogtv_cntx.analogtv_setting.cur_channel.cur_index=0;
            g_analogtv_cntx.analogtv_setting.cur_channel.channel=g_analogtv_cntx.channel_list[0];
            //kal_prompt_trace(MOD_NVRAM,"--------channel-search-- finish");
            g_analogtv_cntx.set_state = ANALOGTV_STATE_PREVIEW;

            mmi_analogtv_entry_Select_List();
        }
}

/*****************************************************************************
* FUNCTION
*	mmi_ATV_player_reset_all_nvram_list
* DESCRIPTION
*  reset list from NVRAM
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
 BOOL mmi_analogtv_reset_all_nvram_list(void) 
{
        S16 ret, error_code;
        U16 i;
        BOOL result = TRUE;

        memset((void *)g_analogtv_cntx.channel_list,0,sizeof(g_analogtv_cntx.channel_list));

        for(i = 0; i < NVRAM_EF_ATV_PLAYER_CHANNEL_TOTAL; i++)
        {
            ret = WriteRecord( NVRAM_EF_ATV_PLAYER_CHANNEL_LID, i + 1, (void *)&g_analogtv_cntx.channel_list[i], NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE, &error_code);
            if (!((error_code == NVRAM_WRITE_SUCCESS) && (ret == NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE)))
            {
                result = FALSE;
                break;
            }
        }
        return result;
}

void mmi_analogtv_search_touch_scr_pen_down_hdlr(mmi_pen_point_struct pos)
{
        g_analogtv_UI.touchobj= TOUCH_OBJ_NONE;

        if(mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.rsk))
        {
            g_analogtv_UI.touchobj=RIGHT_SOFTKEY;
            rightsfk_button_down_handle();
        }
}
void mmi_analogtv_search_touch_scr_pen_up_hdlr(mmi_pen_point_struct pos)
{
        if(g_analogtv_UI.touchobj==TOUCH_OBJ_NONE)
            return;

        if(g_analogtv_UI.touchobj==RIGHT_SOFTKEY)
        {
            g_analogtv_UI.rsk->is_pressed=FALSE;
            mmi_analogtv_exit_preview_auto_search_screen();
        }
}
void mmi_analogtv_search_touch_scr_pen_move_hdlr(mmi_pen_point_struct pos)
{
        if(g_analogtv_UI.touchobj==TOUCH_OBJ_NONE)
            return;
        if(g_analogtv_UI.touchobj==RIGHT_SOFTKEY)
        {
            if(!mmi_analogtv_preview_hit_tst(pos.x, pos.y, g_analogtv_UI.rsk))
            {
                g_analogtv_UI.rsk->is_pressed=FALSE;
                g_analogtv_UI.touchobj=TOUCH_OBJ_NONE;
                g_analogtv_UI.rsk->status=NORMAL;
            }
        }

}
void mmi_analogtv_entry_auto_search_state(void)
{

        g_analogtv_UI.preview_x=g_analogtv_UI.play_wnd->x;
        g_analogtv_UI.preview_y=g_analogtv_UI.play_wnd->y;
        g_analogtv_UI.preview_width=g_analogtv_UI.play_wnd->width;
        g_analogtv_UI.preview_height=g_analogtv_UI.play_wnd->height;
        atvif_SetFullScreen(FALSE);	

#if defined(MMI_ON_HARDWARE_P)	
        mdi_camera_power_on();
#endif
        sxr_Sleep(100);

#if defined(__MMI_TOUCH_SCREEN__)
        wgui_register_pen_down_handler(mmi_analogtv_search_touch_scr_pen_down_hdlr);
        wgui_register_pen_up_handler(mmi_analogtv_search_touch_scr_pen_up_hdlr);
        wgui_register_pen_move_handler(mmi_analogtv_search_touch_scr_pen_move_hdlr);
#endif	
        mmi_analogtv_preview_auto_search_set_key_hdlr();
        mmi_analogtv_reset_all_nvram_list();
        memset((void *)g_analogtv_cntx.channel_list,0,sizeof(g_analogtv_cntx.channel_list));
        g_analogtv_cntx.analogtv_setting.cur_channel.cur_index = 0;
        g_analogtv_cntx.analogtv_setting.total_channel =0;
        g_analogtv_cntx.index=1;
        g_analogtv_cntx.index2=g_analogtv_UI.searchprogress->progress_tic.x;	

        mmi_analogtv_blt_searchprogress_screen();

#if defined(MMI_ON_HARDWARE_P)
        atvd_SetChannel(0);
#endif
        /* if no error occurs, start preview */
#if defined(MMI_ON_HARDWARE_P)
        mmi_analogtv_preview_start();
#endif
        mdi_audio_set_volume(AUD_VOLUME_TV,g_analogtv_cntx.volume);

        channel_auto_search_function();
}

void mmi_analogtv_preview_show_autosearch_pannel(void)
{
        gdi_layer_lock_frame_buffer();
        gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);

        //kal_prompt_trace(MOD_NVRAM,"g_analogtv_UI.searchpannel->x=%d,g_analogtv_UI.searchpannel->y=%d",g_analogtv_UI.searchpannel->x,g_analogtv_UI.searchpannel->y);

        if (g_analogtv_UI.searchpannel->image_id!=0)
            gdi_image_draw_id(g_analogtv_UI.searchpannel->x,g_analogtv_UI.searchpannel->y,g_analogtv_UI.searchpannel->image_id);
#if 0
        gdi_draw_round_rect(g_analogtv_UI.bg_pannel->x,
            g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height+1,
            g_analogtv_UI.bg_pannel->x +g_analogtv_UI.bg_pannel->width,
            g_analogtv_UI.play_wnd->y+g_analogtv_UI.play_wnd->height + 12, 
            GDI_COLOR_GRAY, 
            GDI_COLOR_WHITE, 
            2);
#endif
        //mmi_analogtv_show_search_str();
        mmi_analogtv_show_softkey();
        gdi_layer_pop_and_restore_active();
        gdi_layer_unlock_frame_buffer();
        mmi_analogtv_blt_screen();	
}

void mmi_analogtv_preview_auto_search_set_key_hdlr(void)
{
        ClearInputEventHandler(MMI_DEVICE_KEY);
        change_left_softkey( 0 ,0);
        show_right_softkey();
        SetKeyHandler(mmi_analogtv_exit_preview_auto_search_screen, KEY_RSK, KEY_EVENT_UP);

        /* clear End key */
        //ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
        //ClearKeyHandler(KEY_END, KEY_LONG_PRESS);

        //SetKeyHandler(mmi_analogtv_inc_volume, KEY_VOL_UP, KEY_EVENT_UP);
        //SetKeyHandler(mmi_analogtv_dec_volume, KEY_VOL_DOWN, KEY_EVENT_UP);
}

void mmi_analogtv_exit_preview_auto_search_screen(void)
{
        g_analogtv_cntx.is_rsk_pressed=TRUE;
}

void mmi_analogtv_entry_channel_list_option(void)
{
        U16 nStrItemList[MAX_SUB_MENUS];
        U16 nNumofItem;
        U8 *guiBuffer;

        U16 SettingsSubmenuImageList[MAX_SUB_MENUS];


        EntryNewScreen(SCR_ID_ANALOGTV_CHANNEL_LIST_OPTION, NULL, mmi_analogtv_entry_channel_list_option, NULL);

        guiBuffer = GetCurrGuiBuffer(MENU_ID_ANALOGTV_CHANNEL_LIST_OPTION);
        nNumofItem = GetNumOfChild(MENU_ID_ANALOGTV_CHANNEL_LIST_OPTION);
        GetSequenceStringIds(MENU_ID_ANALOGTV_CHANNEL_LIST_OPTION, nStrItemList);
        GetSequenceImageIds(MENU_ID_ANALOGTV_CHANNEL_LIST_OPTION, SettingsSubmenuImageList);
        SetParentHandler(MENU_ID_ANALOGTV_CHANNEL_LIST_OPTION);
        RegisterHighlightHandler(ExecuteCurrHiliteHandler);
#ifdef __MMI_WGUI_MINI_TAB_BAR__
        wgui_enable_mini_tab_bar(MENU_ID_ANALOGTV_OPTION);
#endif 
        ShowCategory15Screen(
                        STR_GLOBAL_OPTIONS,//STR_MENU_ID_ANALOGTV_CHANNEL_LIST_OPTION,
                        0,
                        STR_GLOBAL_OK,
                        IMG_GLOBAL_OK,
                        STR_GLOBAL_BACK,
                        IMG_GLOBAL_BACK,
                        nNumofItem,
                        nStrItemList,
                        (U16*)gIndexIconsImageList,
                        LIST_MENU,
                        0,
                        guiBuffer);

        SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
void mmi_mainmenu_analogtv(void)
{
        U16 nStrItemList[MAX_SUB_MENUS];
        U16 nNumofItem;
        U8 *guiBuffer;

        U16 SettingsSubmenuImageList[MAX_SUB_MENUS];


        EntryNewScreen(SCR_ID_ANALOGTV_MENU, NULL, mmi_mainmenu_analogtv, NULL);

        guiBuffer = GetCurrGuiBuffer(SCR_ID_ANALOGTV_MENU);
        nNumofItem = GetNumOfChild(MAIN_MENU_ANALOGTV_MENUID);
        GetSequenceStringIds(MAIN_MENU_ANALOGTV_MENUID, nStrItemList);
        GetSequenceImageIds(MAIN_MENU_ANALOGTV_MENUID, SettingsSubmenuImageList);
        SetParentHandler(MAIN_MENU_ANALOGTV_MENUID);
        RegisterHighlightHandler(ExecuteCurrHiliteHandler);
#ifdef __MMI_WGUI_MINI_TAB_BAR__
        wgui_enable_mini_tab_bar(MAIN_MENU_ANALOGTV_MENUID);
#endif 
        ShowCategory15Screen(
                        STR_MENU_ID_ANALOGTV_MAIN,
                        0,
                        STR_GLOBAL_OK,
                        IMG_GLOBAL_OK,
                        STR_GLOBAL_BACK,
                        IMG_GLOBAL_BACK,
                        nNumofItem,
                        nStrItemList,
#ifndef __MMI_SUBMENU_ALL_NUMBER_ICON__
                        SettingsSubmenuImageList,
#else						
                        (U16*)gIndexIconsImageList,
#endif 
                        LIST_MENU,
                        0,
                        guiBuffer);

        SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

//add for engineering mode & factory mode 20071203 begin
extern U8 gCurrentMode;
static short ch_index =0;
extern int rdamtv_factory_ch[3];
static void ExitATVEngineerModePlayScn(void)
{

	//kal_prompt_trace(MOD_MED,"ExitATVEngineerModePlayScn begin");

	ch_index =1;
	mmi_analogtv_exit_fullscr_play_state();
	
	//StopTimer( ANALOGTV_SPLITSCR_TIMER_ID);
//	GPTI_StopItem(g_analogtv_gpt_timer);

	/* turn on/of 3D sound */
#ifdef __MMI_AUDIO_3D_EFFECT__
	if (g_analogtv_cntx.is_3d_sound_on == TRUE)
	{
	mdi_audio_3d_turn_on();
	}
#endif /* __MMI_AUDIO_3D_EFFECT__ */ 

#if defined(MMI_ON_HARDWARE_P)
	mdi_video_rec_preview_stop();

	/* shut down camera */
	mdi_camera_power_off();

	mdi_fmr_power_off();
        mmi_trace(1,"  ExitATVEngineerModePlayScn " );

	atvif_SetCurModule(0);
#endif

	/* resume alignment timer */
	UI_enable_alignment_timers();

	/* let MMI can sleep */
       //SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, 3);
	TurnOffBacklight();

	/* resume background audio */
	mdi_audio_resume_background_play();

	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);

	gdi_layer_flatten_to_base(
g_analogtv_UI.base_layer_handle,
	g_analogtv_UI.preview_layer_handle,
		
	g_analogtv_UI.osd_layer_handle,	
	0
	);
	g_analogtv_UI.base_layer_handle=GDI_LAYER_EMPTY_HANDLE;
	
	gdi_layer_free(g_analogtv_UI.preview_layer_handle);
	//kal_prompt_trace(MOD_NVRAM,"--exit--preview_layer_handle");
	gdi_layer_free(g_analogtv_UI.osd_layer_handle);
	//kal_prompt_trace(MOD_NVRAM,"--exit--osd_layer_handle");

	g_analogtv_UI.preview_layer_handle=GDI_LAYER_EMPTY_HANDLE;
	g_analogtv_UI.osd_layer_handle=GDI_LAYER_EMPTY_HANDLE;
	
//	mmi_frm_scrmem_free((void*)g_analogtv_UI.preview_layer_buf_ptr);
//	kal_prompt_trace(MOD_NVRAM,"--exit--preview_layer_buf_ptr");
//	g_analogtv_UI.preview_layer_buf_ptr= NULL;	
	mmi_frm_scrmem_free((void*)g_analogtv_UI.osd_layer_buf_ptr);
	//kal_prompt_trace(MOD_NVRAM,"--exit--osd_layer_buf_ptr");
	g_analogtv_UI.osd_layer_buf_ptr= NULL;	

	gdi_layer_multi_layer_disable();
	//kal_prompt_trace(MOD_MED,"ExitATVEngineerModePlayScn end");

}


static void HandleATVEngineerModeKeyEntry(void)
{
	static U16 totle_key_val =0;
	U16 curkeyval=0,Keytype=0;

	GetkeyInfo(&curkeyval,&Keytype);		
	if(curkeyval <= KEY_9)
	{
		totle_key_val=totle_key_val*10+curkeyval;

		if(totle_key_val> atvd_GetTotalChannels())
		{
			totle_key_val=0;
		}
	}
	else if(curkeyval==KEY_STAR)
	{
		if(totle_key_val>0)
		{
			atvd_SetChannel(totle_key_val);

			totle_key_val=0;
		}	
	}
}

void analogtv_factorymode_up_press()
{
	if (++ch_index < atvd_GetTotalChannels())
	{
		atvd_SetChannel(rdamtv_factory_ch[ch_index]);
	}
	else
		ch_index = atvd_GetTotalChannels()-1;
}

void analogtv_factorymode_down_press()
{
	if (--ch_index >= 0)
		atvd_SetChannel(rdamtv_factory_ch[ch_index]);
	else
		ch_index = 0;
}

void EntryATVEngineerModePlayScn(void)
{	
	U16 EMATVPlayScreenKeys[] = {KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_STAR};

	EntryNewScreen(SCR_ID_ANALOGTV_APP, ExitATVEngineerModePlayScn, EntryATVEngineerModePlayScn, NULL);
	DeleteScreenIfPresent(SCR_ID_ANALOGTV_APP);

    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	if(gCurrentMode == ENGINEERING_MODE)
	{
		SetGroupKeyHandler(HandleATVEngineerModeKeyEntry,EMATVPlayScreenKeys,11,KEY_EVENT_DOWN);
	}
	else if(gCurrentMode == FACTORY_MODE)
	{
		SetKeyHandler(analogtv_factorymode_up_press, KEY_UP_ARROW , KEY_EVENT_UP);
		SetKeyHandler(analogtv_factorymode_down_press, KEY_DOWN_ARROW , KEY_EVENT_UP);
	}
	
#ifdef __MMI_AUDIO_3D_EFFECT__
	g_analogtv_cntx.is_3d_sound_on = mmi_aud_3d_get_status();
	if (g_analogtv_cntx.is_3d_sound_on == TRUE)
	{
		mdi_audio_3d_turn_off();
	}
#endif /* __MMI_AUDIO_3D_EFFECT__ */ 

#if defined(MMI_ON_HARDWARE_P)
        mmi_trace(1,"  EntryATVEngineerModePlayScn " );

	atvif_SetCurModule(1);
#endif

	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();

	/* entry full screen app */
	entry_full_screen();


	gdi_layer_clear(GDI_COLOR_BLACK);

	/* stop MMI sleep */

	TurnOnBacklight(0);
#if defined(MMI_ON_HARDWARE_P)

	/* force all playing keypad tone off */
	AudioStopReq(GetCurKeypadTone());

	/* disable key pad tone */
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);

	//mdi_audio_set_mute(AUD_VOLUME_FMR,TRUE);
	
	mdi_audio_stop_all();
//	mdi_audio_stop_background_timer();
	
	//if(!isEarphonePlugged)
	//	mdi_fmr_power_on_with_path(MDI_DEVICE_SPEAKER_BOTH, NULL);
	//else
	//	mdi_fmr_power_on_with_path(MDI_DEVICE_SPEAKER2, NULL);


	/* disalbe align timer  */
	UI_disable_alignment_timers();

#endif
	/* enable multi-layer */
	gdi_layer_multi_layer_enable();

	gdi_layer_get_base_handle(&g_analogtv_UI.base_layer_handle);

	//preview layer
	g_analogtv_UI.preview_x=0;
	g_analogtv_UI.preview_y=0;
	g_analogtv_UI.preview_width=UI_device_width;
	g_analogtv_UI.preview_height=UI_device_height;

	gdi_layer_create(
	g_analogtv_UI.preview_x,
	g_analogtv_UI.preview_y,
	g_analogtv_UI.preview_width,
	g_analogtv_UI.preview_height,
	&g_analogtv_UI.preview_layer_handle);
	gdi_layer_push_and_set_active(g_analogtv_UI.preview_layer_handle);

	gdi_layer_clear(GDI_COLOR_BLACK);
	gdi_layer_pop_and_restore_active();
	
	//osd layer
	g_analogtv_UI.osd_layer_buf_ptr=mmi_frm_scrmem_alloc(ANALOGTV_OSD_BUFFER_SIZE);
	MMI_ASSERT(g_analogtv_UI.osd_layer_buf_ptr);
	gdi_layer_create_double_using_outside_memory(
	g_analogtv_UI.osd_layer_x, 
	g_analogtv_UI.osd_layer_y, 
	g_analogtv_UI.osd_layer_width, 
	g_analogtv_UI.osd_layer_height, 
	&g_analogtv_UI.osd_layer_handle, 
	(PU8)g_analogtv_UI.osd_layer_buf_ptr, 
	(S32)ANALOGTV_OSD_BUFFER_SIZE);        /* this buffer is double buffer size */

	gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);

	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	gdi_layer_toggle_double();
	gdi_layer_clear(GDI_COLOR_TRANSPARENT);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_and_restore_active();

	gdi_layer_set_blt_layer(
	g_analogtv_UI.base_layer_handle,
	g_analogtv_UI.preview_layer_handle,    	
	g_analogtv_UI.osd_layer_handle,    	
	0);
	
#if defined(MMI_ON_HARDWARE_P)	
	mdi_camera_power_on();
        sxr_Sleep(100);

	if (gCurrentMode == ENGINEERING_MODE)
	{
		atvd_SetTVArea(TV_AREA_CHINA_SHENZHEN);
		atvd_SetChannel(1);
	}
	else if(gCurrentMode == FACTORY_MODE)
	{
		atvd_SetTVArea(TV_AREA_FACTORYMODE);
		ch_index = 0;
		atvd_SetChannel(rdamtv_factory_ch[0]);
	}

	mmi_analogtv_pre_entry_fullscr_play_state();
	mmi_analogtv_fullscreen_preview();
	//mdi_audio_set_volume(AUD_VOLUME_FMR,3);

       //SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL5);

	//mdi_audio_set_mute(AUD_VOLUME_FMR,FALSE);
    	//mdi_fmr_mute(FALSE);
#endif
	//kal_prompt_trace(MOD_MED,"EntryATVEngineerModePlayScn end");

}

analogtv_state_enum mmi_analogtv_get_state( void )
{
	return g_analogtv_cntx.set_state;
}

#if defined(__MMI_MOTIONSENSOR_FOR_TV_CHANNEL_CHANGE__)  
pBOOL GetMotionChangeTVChannelMode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gMotionChangeTVChannelFlag;
}

void HighlightPhnMotionSetTVChannel(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GetMotionChangeTVChannelMode() == TRUE)
    {
        ChangeLeftSoftkey(STR_GLOBAL_OFF, 0);
    }
    else
    {
        ChangeLeftSoftkey(STR_GLOBAL_ON, 0);
    }

    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(EntryScrMotionChangeTVChannelMode, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void HintTVChannelMode(U16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GetMotionChangeTVChannelMode() == TRUE)
    {
        mmi_ucs2cpy((S8*) hintData[index], (S8*) GetString(STR_GLOBAL_ON));
    }
    else
    {
        mmi_ucs2cpy((S8*) hintData[index], (S8*) GetString(STR_GLOBAL_OFF));
    }
    return;
}

void SetTVChannelMode(pBOOL flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gMotionChangeTVChannelFlag = flag;
    WriteValue(NVRAM_MOTION_CHANGE_TV_CHANNEL, &gMotionChangeTVChannelFlag, DS_BYTE, &error);
}

void LoadTVChannelMode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ReadValue(NVRAM_MOTION_CHANGE_TV_CHANNEL, &gMotionChangeTVChannelFlag, DS_BYTE, &error);
}

void EntryScrMotionChangeTVChannelMode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GetMotionChangeTVChannelMode() == TRUE)
    {
        SetTVChannelMode(FALSE);
        ChangeLeftSoftkey(STR_GLOBAL_OFF, 0);
    }
    else
    {
        SetTVChannelMode(TRUE);
        ChangeLeftSoftkey(STR_GLOBAL_ON, 0);
    }
    HintTVChannelMode(GetCurrHiliteID());
    RedrawCategoryFunction();
    HighlightPhnMotionSetTVChannel();
}

void mmi_motion_tv_trigger(void)
{

	switch(g_analogtv_cntx.set_state)
	{
		case ANALOGTV_STATE_PREVIEW:
		case ANALOGTV_STATE_FULL_SCR_PLAY:
			channel_next_button_down_handle();
			channel_next_button_up_handle();
			break;
		default:
			break;
	}
}
#endif

#ifdef __ANALOGTV_MOTION_TITLE_FULLSCREEN__
static void mmi_analogtv_tile_event_callback(S32 x,S32 y,S32 z)
{
	static BOOL mState = 0;

    /****************************************************************/
    // Code Body
    /****************************************************************/
	switch(g_analogtv_cntx.set_state)
	{
		/* normal */
		case ANALOGTV_STATE_PREVIEW:
			mState = 0;
			break;
		/* fullscreen */
		case ANALOGTV_STATE_FULL_SCR_PLAY:
			mState = 1;
			break;
		default:
			return;
	}
	//kal_prompt_trace(MOD_NIL,"vdoply_tile_event_callback:x=%d,y=%d,z=%d",x,y,z);
	if(mState == 0 && x>-200 && x<200 && y > 512){
		g_analogtv_cntx.set_state = ANALOGTV_STATE_FULL_SCR_PLAY;
		mmi_analogtv_entry_preview_screen();
	}
	if(mState == 1 && y>-200 && y<200 && x > 250){
		g_analogtv_cntx.set_state = ANALOGTV_STATE_PREVIEW;
		mmi_analogtv_entry_preview_screen();
	}	
}
#endif

kal_bool mmi_analogtv_is_in_idle_state( void )
{
	return (g_analogtv_cntx.state == ANALOGTV_STATE_EXIT);
}

#if 1// defined(__SCREEN_AUTO_ROTATE_SUPPORT__) 
void mmi_atv_rotate_switch(U8 direct)
{
#if defined(CAM_MD_SUPPORT)
	if (GetActiveScreenId() == SCR_ID_ANALOGTV_APP)
	{
		//mmi_scr_set_rotate_disable(MMI_SCR_SPECIAL_ROT_TV);

            switch (g_analogtv_cntx.app_state)
            {
                case ANALOGTV_STATE_PREVIEW:		
                        preview_playwnd_up_handle();
                        break;

                case ANALOGTV_STATE_FULL_SCR_PLAY:
                        mmi_analogtv_fullscr_play_key();
                    break;
                default:
                    break;
            }
	}

#else
	if (GetActiveScreenId() == SCR_ID_ANALOGTV_APP)
	{
		//mmi_scr_set_rotate_disable(MMI_SCR_SPECIAL_ROT_TV);

            switch (g_analogtv_cntx.app_state)
            {
                case ANALOGTV_STATE_PREVIEW:		
                switch (direct)
                {
                    case MMI_FRM_SCREEN_ROTATE_90:
                    case MMI_FRM_SCREEN_ROTATE_270:
                        preview_playwnd_up_handle();
                        break;

                    default:
                        break;
                }
                break;

                case ANALOGTV_STATE_FULL_SCR_PLAY:
                switch (direct)
                {		
                    case MMI_FRM_SCREEN_ROTATE_180:
                    case MMI_FRM_SCREEN_ROTATE_0:
                        mmi_analogtv_fullscr_play_key();
                    break;
                default:
                    break;
                }
                    break;
                default:
                    break;
            }
	}
#endif
}
#endif

BOOL mmi_atv_is_analogtv_scr()
{
	return (GetActiveScreenId() == SCR_ID_ANALOGTV_APP);
}

BOOL mmi_analogtv_channel_list_empty(void)
{
        U8 i;

	for(i=0; i<NVRAM_EF_ATV_PLAYER_CHANNEL_TOTAL; i++) 
	{
		if(g_analogtv_cntx.channel_list[i].Present ==TRUE)
		{
		    return FALSE;
		}
	}
    return TRUE;

}

void mmi_analogtv_show_open_str(void)
{
#if 0
        S8 *str_wait;
        S32 l=0,h=0;
        str_wait = GetString( STR_GLOBAL_OPEN);
        Get_StringWidthHeight(str_wait,&l,&h);
        gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);

        UI_push_text_clip();
        UI_reset_text_clip();
        UI_move_text_cursor(g_analogtv_UI.bg_pannel->x+ (( g_analogtv_UI.bg_pannel->width -l )>>1),
                                            g_analogtv_UI.bg_pannel->y+ ((g_analogtv_UI.play_wnd->height)>>1));
        UI_print_text((UI_string_type) str_wait);
        UI_pop_text_clip();
        gdi_layer_pop_and_restore_active();
#endif        
}

void mmi_analogtv_show_search_str(void)
{
        S8 *str_wait;
        S32 l=0,h=0;
        str_wait = GetString( STR_GLOBAL_SEARCHING);
        Get_StringWidthHeight(str_wait,&l,&h);
       // gdi_layer_push_and_set_active(g_analogtv_UI.osd_layer_handle);

        UI_push_text_clip();
        UI_reset_text_clip();
        UI_move_text_cursor(g_analogtv_UI.bg_pannel->x+ (( g_analogtv_UI.bg_pannel->width -l )>>1),
                                            g_analogtv_UI.bg_pannel->y+ ((g_analogtv_UI.play_wnd->height)>>1));
        UI_print_text((UI_string_type) str_wait);
        UI_pop_text_clip();
      //  gdi_layer_pop_and_restore_active();
}


void mmi_analogtv_cancel_preview(void)
{
    DeleteScreenIfPresent(SCR_ID_ANALOGTV_APP);
    GoBackHistory();
}

void mmi_analogtv_clean_app(void)
{
     if(IsScreenPresent( SCR_ID_ANALOGTV_APP) )
       DeleteUptoScrID(IDLE_SCREEN_ID);
}

#endif /* __MMI_ANALOGTV__ */ 



#endif
