
#include "mmi_data_types.h"
#include "gdi_datatype.h"
#include "funandgamesdefs.h"
#include "timerevents.h"
#include "gdi_include.h"
#include "globaldefs.h"
#include "globalmenuitems.h"
#include "frameworkstruct.h"
#include "wgui_draw_manager.h"
#include "wgui_categories_defs.h"
#include "wgui_softkeys.h"
#include "mmi_features_camera.h"
#include "cameraresdefs.h"
#include "audioplayerdef.h"
#include "vdoplystrenum.h"
#include "mainmenudef.h"
#include "soundrecorderdef.h"
#include "fmschedulerecdef.h"
#include "fmradiodef.h"
#include "custmenures.h"
#include "wgui_categories_mediamenu_screen.h"


#if defined(__SG_MMI_MEDIA_SPECIAL_MENU__)
extern U8 disable_menu_shortcut_box_display;
extern void (*ExitCategoryFunction) (void);
extern S32(*GetCategoryHistorySize) (void) ;
extern BOOL r2lMMIFlag;
extern hiliteInfo maxHiliteInfo[MAX_HILITE_HANDLER];
extern U8 MMI_disable_title_shortcut_display;
extern void dm_redraw_category_screen(void);
static BOOL g_is_in_media_screen = FALSE;
extern void (*RedrawCategoryFunction) (void);
extern stFontAttribute	MMI_medium_font;
extern void GoBackHistory(void);
extern void mmi_phb_entry_main_menu(void);
void show_a_ball(BOOL is_small_ball,int index);
void reset_ball(BOOL is_small_ball,int index);
void new_ball(void);
media_fixed_menu_struct mmi_media_menu = {0};
ball_struct small_ball[MAX_SMALL_BALL] = {0};
ball_struct big_ball [MAX_BIG_BALL]= {0};
gdi_handle multimedia_menu_layer;

static const media_resource_struct mediamenu[MMI_MEDIA_MENU_MAX_NUM] = 
{
	{MENU_ID_IMGVIEW_APP, IMG_ID_UN_MEDIA_FMRDO_TITLE,IMG_ID_HI_MEDIA_FMRDO_TITLE,STR_ID_CAMERA_APP_NAME},
	{MENU_ID_CAMERA_APP, IMG_ID_UN_MEDIA_IMGVIEW_NAME,IMG_ID_HI_MEDIA_IMGVIEW_NAME,STR_ID_AUDPLY_TITLE},
	{AUDIO_PLAYER_MAIN_MENUID, IMG_ID_UN_MEDIA_CAMERA_APP_NAME,IMG_ID_HI_MEDIA_CAMERA_APP_NAME,STR_ID_VDOPLY_APP_NAME},
	{MENU_ID_VDOPLY_APP, IMG_ID_UN_MEDIA_VDOREC_APP_NAME,IMG_ID_HI_MEDIA_VDOREC_APP_NAME,MAIN_MENU_VDOREC_TEXT},
	{MENU_ID_VDOREC_APP, IMG_ID_UN_MEDIA_SNDREC_MAIN,IMG_ID_HI_MEDIA_SNDREC_MAIN ,STR_ID_SNDREC_MAIN},
	{MENU_ID_SNDREC_MAIN, IMG_ID_UN_MEDIA_VDOREC_APP_NAME,IMG_ID_HI_MEDIA_VDOREC_APP_NAME,STR_ID_SNDREC_MAIN},
	{MENU_ID_FMRDO_MAIN, IMG_ID_UN_MEDIA_FMRDO_TITLE,IMG_ID_HI_MEDIA_FMRDO_TITLE,STR_ID_FMRDO_TITLE}
		
};


void mmi_media_hightlighted_fixed_menu_function(void* item,S32 index)
{
	U16 menu_id;
	FuncPtr currFuncPtr = NULL;
	media_fixed_menu_struct *m = (media_fixed_menu_struct*)item;
	
	menu_id = m->media_menu_id[index];
	currFuncPtr = maxHiliteInfo[menu_id].entryFuncPtr;

	if (currFuncPtr != NULL)		
	{
		(*currFuncPtr)();
	}
}

void mmi_entry_media_fixed_item(void)
{
	mmi_media_hightlighted_fixed_menu_function(&mmi_media_menu,mmi_media_menu.highlighted_item);
}

BOOL is_in_special_media_screen(void)
{
	return g_is_in_media_screen;
}

void media_show_menu_str(U16 str_y,BOOL is_higliht,UI_string_type str_string)
{
	S32 str_width,str_height;
	gui_set_font(&MMI_medium_font);
	gui_measure_string(str_string, &str_width, &str_height);
	str_y += ((MMI_MEDIA_MENU_STR_H -str_height )>>1);

	if(is_higliht)
	{
		gui_set_text_color(gui_color(255,255,255));
	}
	else
	{
       	gui_set_text_color(gui_color(160,160,160));
	}
	if (r2lMMIFlag)
	{
		gui_move_text_cursor(MAIN_LCD_DEVICE_WIDTH- 5, str_y);
	}
	else
	{
		gui_move_text_cursor(MMI_MEDIA_MENU_STR_X, str_y);
	}
	if(str_width <=MMI_MEDIA_MENU_STR_W)
		gui_print_text(str_string);
	else
		gui_print_truncated_text(MMI_MEDIA_MENU_STR_X,str_y,MMI_MEDIA_MENU_STR_W,str_string);
		
}


void mmi_media_display_fixed_menu_background(void)
{
	gdi_handle base_handle;
	S32 i;
	gdi_layer_lock_frame_buffer();
	for(i = 0;i<MAX_SMALL_BALL;i++)
	{
		reset_ball(TRUE,i);
		reset_ball(FALSE,i);
	}
	
	gdi_layer_restore_base_active();
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);	
#ifdef __SG_MMI_MEDIA_SPECIAL_MENU_WITH_SOFTKEY__
	gdi_draw_solid_rect(0,0,MAIN_LCD_DEVICE_WIDTH -1,MAIN_LCD_DEVICE_HEIGHT - MMI_BUTTON_BAR_HEIGHT -1, GDI_COLOR_BLACK);
#else
	gdi_draw_solid_rect(0,0,MAIN_LCD_DEVICE_WIDTH -1,MAIN_LCD_DEVICE_HEIGHT-1, GDI_COLOR_BLACK);
#endif
	gui_show_image(MMI_MEDIA_MENU_HALF_BALL_X,MMI_MEDIA_MENU_HALF_BALL_Y,get_image(IMG_ID_MEDIA_HALF_BALL_BG));
	//small ball
	for(i=0;i<MAX_SMALL_BALL;i++)
	{
		show_a_ball(TRUE,i);
		if(small_ball[i].ball_x >= BALL_RECT_X2 +SMALL_BALL_R )
		{
			reset_ball(TRUE,i);
		}
	}

	//big ball	
	for(i=0;i<MAX_SMALL_BALL;i++)
	{
		show_a_ball(FALSE,i);
		if(big_ball[i].ball_x >= BALL_RECT_X2 +BIG_BALL_R )
		{
			reset_ball(FALSE,i);
		}
	}	
	gdi_layer_unlock_frame_buffer();
#ifdef __SG_MMI_MEDIA_SPECIAL_MENU_WITH_SOFTKEY__
	gui_BLT_double_buffer(0,0,MAIN_LCD_DEVICE_WIDTH -1,MAIN_LCD_DEVICE_HEIGHT- MMI_BUTTON_BAR_HEIGHT -1);
#else
	gui_BLT_double_buffer(0,0,MAIN_LCD_DEVICE_WIDTH -1,MAIN_LCD_DEVICE_HEIGHT-1);
#endif
	

}

void dm_setup_and_draw_meida_menu(S16 **UICtrlAccessPtr_p)
{
	mmi_media_display_fixed_menu_background();
} 


void reset_ball(BOOL is_small_ball,int index)
{
	if(is_small_ball )
	{
		small_ball[index].ball_b = rand() % (BALL_RECT_X2);

		small_ball[index].ball_c = MMI_MEDIA_MENU_LINE_Y + rand() % 75;
		small_ball[index].ball_y = MMI_MEDIA_MENU_LINE_Y - rand() % 10;
		small_ball[index].ball_x = SMALL_BALL_A*(small_ball[index].ball_y -small_ball[index].ball_c)*(small_ball[index].ball_y -small_ball[index].ball_c) + small_ball[index].ball_b  ;
		small_ball[index].offset  = small_ball[index].ball_b + BALL_RECT_X2  - small_ball[index].ball_x;
		if(small_ball[index].offset  < MMI_MEDIA_MENU_STR_X + SMALL_BALL_R)
			small_ball[index].offset  = MMI_MEDIA_MENU_STR_X + SMALL_BALL_R ;
	}
	else
	{
		big_ball[index].ball_b = rand() % (BALL_RECT_X2);
		big_ball[index].ball_c = MMI_MEDIA_MENU_LINE_Y + rand() % 75;
		big_ball[index].ball_y = MMI_MEDIA_MENU_LINE_Y - rand() % 10;
		big_ball[index].ball_x = SMALL_BALL_A*(big_ball[index].ball_y -big_ball[index].ball_c)*(big_ball[index].ball_y -big_ball[index].ball_c) + big_ball[index].ball_b  ;
		big_ball[index].offset  = big_ball[index].ball_b + BALL_RECT_X2 - big_ball[index].ball_x;
		if(big_ball[index].offset  < MMI_MEDIA_MENU_STR_X + BIG_BALL_R)
			big_ball[index].offset  = MMI_MEDIA_MENU_STR_X + BIG_BALL_R ;
	}
	
}

void Show_every_ball(void)
{	
	int i;
	//gdi_handle base_layer;
	StopTimer(MEDIA_MENU_EFFECT_BALL_TIMER);
	StartTimer(MEDIA_MENU_EFFECT_BALL_TIMER,5,Show_every_ball);

	gdi_layer_lock_frame_buffer();

	gdi_layer_restore_base_active();
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	
	
#ifdef __SG_MMI_MEDIA_SPECIAL_MENU_WITH_SOFTKEY__
	gdi_draw_solid_rect(0,0,UI_DEVICE_WIDTH -1,UI_DEVICE_HEIGHT - MMI_BUTTON_BAR_HEIGHT -1, GDI_COLOR_BLACK);
#else
	gdi_draw_solid_rect(0,0,UI_DEVICE_WIDTH -1,UI_DEVICE_HEIGHT -1, GDI_COLOR_BLACK);
#endif
	gui_show_image(MMI_MEDIA_MENU_HALF_BALL_X,MMI_MEDIA_MENU_HALF_BALL_Y,get_image(IMG_ID_MEDIA_HALF_BALL_BG));

	//small ball
	for(i=0;i<MAX_SMALL_BALL;i++)
	{
		show_a_ball(TRUE,i);
		if(small_ball[i].ball_x >= BALL_RECT_X2 +SMALL_BALL_R )
			reset_ball(TRUE,i);
	}

	//big ball	
	for(i=0;i<MAX_SMALL_BALL;i++)
	{
		show_a_ball(FALSE,i);
		if(big_ball[i].ball_x >= BALL_RECT_X2 +BIG_BALL_R )
			reset_ball(FALSE,i);	
	}

	gdi_layer_unlock_frame_buffer();
	//gdi_layer_blt_previous(MMI_MEDIA_MENU_STR_X, MMI_MEDIA_MENU_HALF_BALL_Y , BALL_RECT_X2 , BALL_RECT_Y2);
	gdi_layer_blt_previous(0, 0 , BALL_RECT_X2 , BALL_RECT_Y2);

	
}


void show_a_ball(BOOL is_small_ball,int index)
{
	if(is_small_ball )
	{
		small_ball[index].ball_y++;
		small_ball[index].ball_x = SMALL_BALL_A*(small_ball[index].ball_y -small_ball[index].ball_c)*(small_ball[index].ball_y -small_ball[index].ball_c)   +  small_ball[index].offset;
		gdi_image_draw_id(small_ball[index].ball_x-SMALL_BALL_R, small_ball[index].ball_y-SMALL_BALL_R,IMG_ID_MEDIA_MENU_SMALL_BALL );
	}
	else
	{
		big_ball[index].ball_y++;
		big_ball[index].ball_x = SMALL_BALL_A*(big_ball[index].ball_y -big_ball[index].ball_c)*(big_ball[index].ball_y -big_ball[index].ball_c)   +  big_ball[index].offset;
		gdi_image_draw_id(big_ball[index].ball_x-BIG_BALL_R, big_ball[index].ball_y-SMALL_BALL_R,IMG_ID_MEDIA_MENU_BIG_BALL );
	}
}


void mmi_display_multimedia_menu_str_and_icon(S32 hilite_index)
{
	gdi_handle base_layer;
	S16 menu_y = MMI_MEDIA_MENU_LINE_Y - (MMI_MEDIA_ICON_H*(hilite_index+1));
	S32 first_displayed_menu_index;
	UI_string_type menu_string;
 
	StopTimer(MEDIA_MENU_EFFECT_BALL_TIMER);//I am afraid complex caculating cause system collapse.
	gdi_layer_lock_frame_buffer();
	gdi_layer_set_active(multimedia_menu_layer);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
	gdi_layer_clear_background(GDI_COLOR_TRANSPARENT);
	
	gdi_draw_solid_rect(MMI_MEDIA_MENU_STR_X, MMI_MEDIA_MENU_LINE_Y, MAIN_LCD_DEVICE_WIDTH - 1, MMI_MEDIA_MENU_LINE_Y +2, GDI_COLOR_WHITE);
	gdi_draw_solid_rect(MMI_MEDIA_MENU_STR_X, MMI_MEDIA_MENU_LINE_Y + 2, MAIN_LCD_DEVICE_WIDTH - 1, MMI_MEDIA_MENU_LINE_Y +4, GDI_COLOR_BLACK);

	if(menu_y >= 0)
		first_displayed_menu_index = 0;
	else														
	{
		S32 i;
		for(i=1;i<MMI_MEDIA_MENU_MAX_NUM;i++)
		{
			if((menu_y + i*MMI_MEDIA_ICON_H) >= 0)
			{
				first_displayed_menu_index = i;
				menu_y += i*MMI_MEDIA_ICON_H;
				break;
			}
		}
	}
	for(first_displayed_menu_index;first_displayed_menu_index<MMI_MEDIA_MENU_MAX_NUM;first_displayed_menu_index++)
	{
		menu_string = (UI_string_type)get_string(mmi_media_menu.media_select_title_id[first_displayed_menu_index]);
		if(mmi_media_menu.highlighted_item != first_displayed_menu_index)
		{
			gdi_image_draw_id(MMI_MEDIA_ICON_X,menu_y,mmi_media_menu.media_unselect_image_id[first_displayed_menu_index]);
			media_show_menu_str(menu_y,FALSE,menu_string);	
		}	
		else
		{
			gdi_image_draw_id(MMI_MEDIA_ICON_X,menu_y,mmi_media_menu.media_select_image_id[first_displayed_menu_index]);
			media_show_menu_str(menu_y,TRUE,menu_string);	
		}
		menu_y += MMI_MEDIA_ICON_H;	
	}

	gdi_layer_unlock_frame_buffer();
	gdi_layer_get_base_handle(&base_layer);
	gdi_layer_set_blt_layer(base_layer,multimedia_menu_layer, 0,0);
	gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height- 1);
	gdi_layer_restore_base_active();

	//mmi_media_hightlighted_fixed_menu_function(&mmi_media_menu,mmi_media_menu.highlighted_item);
	StartTimer(MEDIA_MENU_EFFECT_BALL_TIMER,5,Show_every_ball);
}

void exit_multimedia_menu_screen(void)
{
	if(multimedia_menu_layer != NULL)
	{
		gdi_layer_free(multimedia_menu_layer);
		multimedia_menu_layer = GDI_LAYER_EMPTY_HANDLE;
	}
}

void mmi_display_multimedia_menu(void)
{
	gdi_layer_multi_layer_enable();
	gdi_layer_create(0, 0, UI_device_width, UI_device_height,&multimedia_menu_layer);
	mmi_display_multimedia_menu_str_and_icon(mmi_media_menu.highlighted_item);
}

void new_ball(void)
{
	int i;
	S16 halfball_x = 0;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	//»­°ëÔ²
       StopTimer(MEDIA_MENU_EFFECT_TIMER);
	while(halfball_x > MMI_MEDIA_MENU_HALF_BALL_X)
	{
		halfball_x -=20;
		if(halfball_x < MMI_MEDIA_MENU_HALF_BALL_X)
			halfball_x = MMI_MEDIA_MENU_HALF_BALL_X;
		
		gdi_layer_restore_base_active();
		gdi_layer_lock_frame_buffer();
		gdi_layer_clear(GDI_COLOR_TRANSPARENT);
		gdi_image_draw_id(halfball_x,MMI_MEDIA_MENU_HALF_BALL_Y,IMG_ID_MEDIA_HALF_BALL_BG);
		gdi_layer_unlock_frame_buffer();
		gdi_layer_blt_previous(halfball_x, MMI_MEDIA_MENU_HALF_BALL_Y, UI_DEVICE_WIDTH - 1, MMI_MEDIA_MENU_HALF_BALL_Y+ MMI_MEDIA_MENU_HALF_BALL_H -1);
	}	

	for(i=0;i<MAX_SMALL_BALL;i++)
	{
		reset_ball(TRUE,i);
	}

	for(i=0;i<MAX_BIG_BALL;i++)
	{
		reset_ball(FALSE,i);
	}
	Show_every_ball();
}

void media_set_fixed_menu_item_function(media_fixed_menu_struct* m,void(*item_display_function)(S32 index),void(*item_highlighted_function)(void* item,S32 index))
{
	m->item_display_function = item_display_function;
	m->item_highlighted_function = item_highlighted_function;
}

void setup_media_screen_menu(void)
{
	U8 i;
	for(i=0;i<MMI_MEDIA_MENU_MAX_NUM;i++)
	{
		mmi_media_menu.media_menu_id[i] = mediamenu[i].MenuId;
		mmi_media_menu.media_unselect_image_id[i] = mediamenu[i].ImageId;
		mmi_media_menu.media_select_image_id[i] = mediamenu[i].SelImageId;
		mmi_media_menu.media_select_title_id[i] = mediamenu[i].strId;
	}
	media_set_fixed_menu_item_function(&mmi_media_menu,mmi_display_multimedia_menu_str_and_icon,mmi_media_hightlighted_fixed_menu_function);
}

U8 set_media_menu_category_history(U16 history_ID, U8 *history_buffer)
{
    if (history_buffer != NULL)
    {
        media_menu_category_history *h = (media_menu_category_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            mmi_media_menu.highlighted_item = h->highlighted_item;
            return (1);
        }
    }
    return (0);
}

void ExitmediamenuCategoryScreen(void)
{
    StopTimer(MEDIA_MENU_EFFECT_TIMER);
    StopTimer(MEDIA_MENU_EFFECT_BALL_TIMER);

    g_is_in_media_screen = FALSE;
    MMI_disable_title_shortcut_display = 0; /* 042905 Calvin added */
    disable_menu_shortcut_box_display = 0;
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    reset_pop_up_descriptions();

#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    reset_all_force_flags_for_hints();
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_unblock_list_effect();
#endif 
#endif /* __MMI_UI_HINTS_IN_MENUITEM__ */ 

}

void go_to_media_menu_item_no_redraw(media_fixed_menu_struct* m,S32 index)
{
	m->highlighted_item = index;
	m->item_display_function(m->highlighted_item);
}

void fixed_media_menu_goto_item_no_redraw(S32 item_index)
{
	mmi_media_menu.highlighted_item = item_index;
	mmi_media_display_fixed_menu_background();
	//go_to_media_menu_item_no_redraw(&mmi_media_menu, item_index);
}


void media_goto_previous_item(media_fixed_menu_struct* m)
{

	if(m->highlighted_item == 0)
		m->highlighted_item = MMI_MEDIA_MENU_MAX_NUM -1;
	else
		m->highlighted_item--;
	m->item_display_function(m->highlighted_item);		
}

void media_goto_next_item(media_fixed_menu_struct* m)
{

	if(m->highlighted_item == MMI_MEDIA_MENU_MAX_NUM -1)
		 m->highlighted_item = 0;
	else
		m->highlighted_item++;
	m->item_display_function(m->highlighted_item);
}

void fixed_media_goto_previous_item(void)
{
	media_goto_previous_item(&mmi_media_menu);
	mmi_entry_media_fixed_item();
}

void fixed_media_goto_next_item(void)
{
	media_goto_next_item(&mmi_media_menu);
	mmi_entry_media_fixed_item();
}	
void register_media_fixed_menu_keys(void)
{
	SetKeyHandler(fixed_media_goto_previous_item,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(fixed_media_goto_next_item,KEY_DOWN_ARROW,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void ShowCategorymediamenuScreen(
        U16 left_softkey,
        U16 right_softkey,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	dm_data_struct dm_data;
	//U8 h_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	gdi_layer_lock_frame_buffer();
#ifdef __SG_MMI_MEDIA_SPECIAL_MENU_WITH_SOFTKEY__
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetupCategoryKeyHandlers();
#else
	register_media_fixed_menu_keys();
#endif
	
	g_is_in_media_screen = TRUE;
#if 0
	h_flag = set_media_menu_category_history(MMI_CATEGORY_MEDIA_ID, history_buffer);
	if (h_flag)
	{
		fixed_media_menu_goto_item_no_redraw(mmi_media_menu.highlighted_item);
	}
	else
	{
		fixed_media_menu_goto_item_no_redraw(MMI_MEDIA_MENU_DEFALT_HL_ITEM);

	}
#endif	
	gdi_layer_unlock_frame_buffer();
	
	mmi_media_hightlighted_fixed_menu_function(&mmi_media_menu,mmi_media_menu.highlighted_item);
	ExitCategoryFunction = ExitmediamenuCategoryScreen;
	RedrawCategoryFunction = dm_redraw_category_screen;
	GetCategoryHistorySize = dm_get_category_history_size;
	dm_data.S32ScrId = (S32) GetActiveScreenId();
	dm_data.S32CatId = MMI_CATEGORY_MEDIA_ID;
	dm_data.S32flags = 0;
	dm_setup_data(&dm_data);
	dm_redraw_category_screen();
}

#endif


