/*************************************************************************
* File Name£ºDevExtWguiinlineedit.c
* Description£º
* Date£º2009-10-29   	
* Author£ºbruce	    	    	
* Usage: included by wgi_inline_edit.c
* Copyright (c) 2009 SHZMT All Rights Reserved.
************************************************************************/
#ifdef _KK_SUPPORT_ 

#include "compswitch.h"

extern U8 *GetCategoryVideo_57History(U8 *history_buffer);

extern BOOL	gFlagEntryWep;
#if (KK_MTKXXXX_SWSUBVER==KK_MTKXXXX_V09A52)	//BN10123

void KK_handle_multi_line_rd_only_close(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_multiline_inputbox_vertical_scroll();
    reset_multiline_inputbox();
}

void KK_redraw_multiline_rd_only(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_set_clip(MMI_fixed_list_menu.x, MMI_fixed_list_menu.y, MMI_fixed_list_menu.x + MMI_fixed_list_menu.width - 1, MMI_fixed_list_menu.y + MMI_fixed_list_menu.height - 1);
    register_multiline_inputbox_viewer_keys();
    show_multiline_inputbox();
    gui_pop_clip();
}

void KK_wgui_show_image_text_item(S32 x1, S32 y1, S32 iwidth, S32 iheight, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    wgui_inline_image_text *i = (wgui_inline_image_text*) item->item;
    wgui_inline_item_text_edit *i_edit = (wgui_inline_item_text_edit*) i->inline_full_text;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    draw_image_text_control(i, x1, y1, iwidth, iheight, i->image1, i->image2, i->image3, i_edit->buffer, 0);

}

void KK_clear_inline_image_text_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_scrolling_text_stop(&wgui_inline_scroll_text);
    memset(&wgui_inline_scroll_text, 0, sizeof(wgui_inline_scroll_text));
    clear_inline_item_keys = UI_dummy_function;
    redraw_current_inline_item = UI_dummy_function;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = UI_dummy_inline_item_pen_function;
#endif 
}
void KK_wgui_show_image_text_item_highlight(S32 x1, S32 y1, S32 iwidth, S32 iheight, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_text *i = (wgui_inline_image_text*) item->item;
    wgui_inline_item_text_edit *i_edit = (wgui_inline_item_text_edit*) i->inline_full_text;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i->x = x1;
    i->y = y1;
    i->width = iwidth;
    i->height = iheight;

    clear_inline_item_keys = KK_clear_inline_image_text_item;
    redraw_current_inline_item = redraw_image_text_control;
#ifdef __MMI_TOUCH_SCREEN__
//    wgui_current_inline_item_pen_function = inline_image_text_pen_handler;
#endif 
    current_image_text_control = i;
    draw_image_text_control(i, x1, y1, iwidth, iheight, i->image1, i->image2, i->image3, i_edit->buffer, 1);

}
#endif
BOOL get_inline_opacity_changed(void)
{
	return inline_opacity_changed;
}	
void set_inline_opacity_changed(BOOL status)
{
	inline_opacity_changed = status;
}

#if (KK_MTKXXXX_SWVER < KK_MTKXXXX_V08A)

void ShowCategory57_wepvideochatScreen(
	U16 imgHeight,
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_icons,
        InlineItem *list_of_items,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    U16 list_height;
    U16 list_offset_y;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_content_x = MMI_COMMON_CONTENT_X;
    MMI_content_y = MMI_COMMON_CONTENT_Y +imgHeight-MMI_COMMON_TITLE_HEIGHT;
    MMI_content_width = MMI_COMMON_CONTENT_WIDTH;
    MMI_content_height = MMI_COMMON_CONTENT_HEIGHT-imgHeight+MMI_COMMON_TITLE_HEIGHT;

    MMI_title_x = MMI_COMMON_TITLE_X;
    MMI_title_y = MMI_COMMON_TITLE_Y;
    MMI_title_width = MMI_COMMON_TITLE_WIDTH;
    MMI_title_height = MMI_COMMON_TITLE_HEIGHT;
	move_multiline_inputbox(MMI_content_x,MMI_content_y);
//	resize_multiline_inputbox(MMI_content_width,MMI_content_height);

	move_fixed_list(MMI_content_x, MMI_content_y);
   	resize_fixed_list(MMI_content_width+12,MMI_content_height);

    /* init append related parameters */
    if (cat57_append_type == CAT57_APPEND_TOP_INFO_BOX)
    {
        list_height = MMI_content_height - MMI_menuitem_height * cat57_info_row_count;
        list_offset_y = MMI_title_y + MMI_title_height + MMI_menuitem_height * cat57_info_row_count;

        cat57_pos_x1 = 0;
        cat57_pos_y1 = MMI_title_y + MMI_title_height;
        cat57_pos_x2 = UI_device_width - 1;
        cat57_pos_y2 = MMI_title_y + MMI_title_height + MMI_menuitem_height * cat57_info_row_count - 1;
	#if (KK_MTKXXXX_SWVER < KK_MTKXXXX_V08A)
        cat57_is_need_draw = FALSE;
	#endif
    }

    gdi_layer_lock_frame_buffer();

#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
#endif 

    ShowCommonCategoryInlineEdit(
        (UI_string_type) get_string(title),
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        list_of_icons,
        list_of_items,
        highlighted_item,
        history_buffer,
        Cat57HighlightHandler,
        Cat57UnHighlightHandler,
//        MMI_CATEGORY57_ID);
MMI_CATEGORY57_WEP_VIDEO_CHAT_ID);

    if (flag_scroll)//pangpeng 090216 for dual scrollbar
    {
        MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_SCROLLBAR;
    }

#ifdef __MMI_UI_LIST_CACHE_BACKGROUND__
     
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_BKGRND_IN_LAYER;
#endif /* __MMI_UI_LIST_CACHE_BACKGROUND__ */ 

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory57Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = GetCategory57History;
    GetCategoryHistorySize = GetCategory57HistorySize;
    dm_data.s32ScrId = (S32) GetActiveScreenId();
    dm_data.s32CatId = MMI_CATEGORY57_WEP_VIDEO_CHAT_ID;
    dm_data.s32flags = 0;
    dm_data.s32flags |= DM_SHOW_VKPAD;
    dm_data.s32flags |= DM_NO_BLT ;	
    dm_data.s32flags |=DM_NO_TITLE;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(dm_category57_controlled_area);
    dm_register_vkpad_callback(cat57_virtual_keypad_callback);
    dm_redraw_category_screen();
}

#elif (KK_MTKXXXX_SWVER < KK_MTKXXXX_V09A) /* 08A, 08B*/

		void ShowCategory57_wepvideochatScreen(
	U16 imgHeight,
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_icons,
        InlineItem *list_of_items,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    U16 list_height;
    U16 list_offset_y;
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_content_x = MMI_COMMON_CONTENT_X;
    MMI_content_y = MMI_COMMON_CONTENT_Y +imgHeight-MMI_COMMON_TITLE_HEIGHT ;
    MMI_content_width = MMI_COMMON_CONTENT_WIDTH;
    MMI_content_height = MMI_COMMON_CONTENT_HEIGHT-imgHeight+MMI_COMMON_TITLE_HEIGHT;

    MMI_title_x = MMI_COMMON_TITLE_X;
    MMI_title_y = MMI_COMMON_TITLE_Y ;
    MMI_title_width = MMI_COMMON_TITLE_WIDTH;
    MMI_title_height = MMI_COMMON_TITLE_HEIGHT;
	move_multiline_inputbox(MMI_content_x,MMI_content_y);

	move_fixed_list(MMI_content_x, MMI_content_y );
   	resize_fixed_list(MMI_content_width+20,MMI_content_height);

    /* init append related parameters */
    if (cat57_append_type == CAT57_APPEND_TOP_INFO_BOX)
    {
        list_height = MMI_content_height - MMI_menuitem_height * cat57_info_row_count;
        list_offset_y = MMI_title_y + MMI_title_height + MMI_menuitem_height * cat57_info_row_count;

        cat57_pos_x1 = 0;
        cat57_pos_y1 = MMI_title_y + MMI_title_height ;
        cat57_pos_x2 = UI_device_width - 1;
        cat57_pos_y2 = MMI_title_y + MMI_title_height + MMI_menuitem_height * cat57_info_row_count - 1;

        //bruce091023 cat57_is_need_draw = FALSE;
    }
    
    gdi_layer_lock_frame_buffer();

#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
#endif 

    ShowCommonCategoryInlineEdit(
        (UI_string_type) get_string(title),
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        list_of_icons,
        list_of_items,
        highlighted_item,
        history_buffer,
        Cat57HighlightHandler,
        Cat57UnHighlightHandler,
//        MMI_CATEGORY57_ID);
MMI_CATEGORY57_WEP_VIDEO_CHAT_ID);

    if (flag_scroll)
    {
        MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_SCROLLBAR;
    }
	
    //MMI_fixed_list_menu.normal_filler = &wep_bg_fill;
	
//    MMI_fixed_list_menu.flags = UI_LIST_MENU_DISABLE_SCROLLBAR | UI_LIST_MENU_ENABLE_TRANSITION;
#ifdef __MMI_UI_LIST_CACHE_BACKGROUND__
     
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_BKGRND_IN_LAYER;
#endif /* __MMI_UI_LIST_CACHE_BACKGROUND__ */ 

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory57Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = GetCategoryVideo_57History;//GetCategory57History;
    GetCategoryHistorySize = GetCategory57HistorySize;
    dm_data.s32ScrId = (S32) GetActiveScreenId();
    dm_data.s32CatId = MMI_CATEGORY57_WEP_VIDEO_CHAT_ID;
    dm_data.s32flags = 0;
    dm_data.s32flags |= DM_SHOW_VKPAD;
    dm_data.s32flags |= DM_NO_BLT ;	
    dm_data.s32flags |=DM_NO_TITLE;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(dm_category57_controlled_area);
    dm_register_vkpad_callback(cat57_virtual_keypad_callback);
    dm_redraw_category_screen();
	

}

#else /*09A*/

void ShowCategory57_wepvideochatScreen(
	U16 imgHeight,
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_icons,
        InlineItem *list_of_items,
        S32 highlighted_item,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    dm_coordinates gui_list_get_coordinates;
    U16 list_height;
    U16 list_offset_y;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_content_x = MMI_COMMON_CONTENT_X;
    MMI_content_y = MMI_COMMON_CONTENT_Y +imgHeight-MMI_COMMON_TITLE_HEIGHT;
    MMI_content_width = MMI_COMMON_CONTENT_WIDTH;
    MMI_content_height = MMI_COMMON_CONTENT_HEIGHT-imgHeight+MMI_COMMON_TITLE_HEIGHT;

    MMI_title_x = MMI_COMMON_TITLE_X;
    MMI_title_y = MMI_COMMON_TITLE_Y;
    MMI_title_width = MMI_COMMON_TITLE_WIDTH;
    MMI_title_height = MMI_COMMON_TITLE_HEIGHT;

//	move_multiline_inputbox(MMI_content_x,MMI_content_y);
//	resize_multiline_inputbox(MMI_content_width,MMI_content_height );


    /* init append related parameters */
    if (cat57_append_type == CAT57_APPEND_TOP_INFO_BOX)
    {
        list_height = MMI_content_height - MMI_menuitem_height * cat57_info_row_count;
        list_offset_y = MMI_title_y + MMI_title_height + MMI_menuitem_height * cat57_info_row_count;

        cat57_pos_x1 = 0;
        cat57_pos_y1 = MMI_title_y + MMI_title_height;
        cat57_pos_x2 = UI_device_width - 1;
        cat57_pos_y2 = MMI_title_y + MMI_title_height + MMI_menuitem_height * cat57_info_row_count - 1;

        //bruce	cat57_is_need_draw = FALSE;
    }

    gdi_layer_lock_frame_buffer();

#if defined(__MMI_TOUCH_SCREEN__)
#ifdef __MMI_VIRTUAL_KEYBOARD__ //
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
#endif
#endif 

    ShowCommonCategoryInlineEdit(
        (UI_string_type) get_string(title),
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        list_of_icons,
        list_of_items,
        highlighted_item,
        history_buffer,
        Cat57HighlightHandler,
        Cat57UnHighlightHandler,
//        MMI_CATEGORY57_ID);
MMI_CATEGORY57_WEP_VIDEO_CHAT_ID);

    if (flag_scroll)
    {
        MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_SCROLLBAR;
    }
		if (history_buffer == NULL)
    {
        dm_get_control_coordinates_from_category(
            -1,
            MMI_CATEGORY57_WEP_VIDEO_CHAT_ID,
            DM_INLINE_FIXED_LIST1,
            -1,
            &gui_list_get_coordinates);
        
        resize_fixed_list(gui_list_get_coordinates.s16Width, gui_list_get_coordinates.s16Height);
#if defined(__MMI_TOUCH_SCREEN__)
        mmi_imc_set_inline_editor_full_height(MMI_fixed_list_menu.height);
#endif
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
#ifdef __MMI_UI_LIST_CACHE_BACKGROUND__
     
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_BKGRND_IN_LAYER;
#endif /* __MMI_UI_LIST_CACHE_BACKGROUND__ */ 

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory57Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = GetCategory57History;
    GetCategoryHistorySize = GetCategory57HistorySize;
    dm_data.s32ScrId = (S32) GetActiveScreenId();
    dm_data.s32CatId = MMI_CATEGORY57_WEP_VIDEO_CHAT_ID;
    dm_data.s32flags = 0;
    dm_data.s32flags |= DM_SHOW_VKPAD;
    dm_data.s32flags |= DM_NO_TITLE;
    dm_data.s32flags |= DM_NO_BLT;/*pangpeng add @ 090226 for no flicker*/
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(dm_category57_controlled_area);
    dm_register_vkpad_callback(cat57_virtual_keypad_callback);
    dm_redraw_category_screen();
}

#endif


//import varibables to wep module
//template
//S32 wepextget_wgui_inline_item_key_code( void ){return wgui_inline_item_key_code;}
//S32 wepextset_wgui_inline_item_key_code( void ){return wgui_inline_item_key_code;}
#if 1//def WEP_MTKBASE_EXTERN_ALLINONE

S32 *wepextptr_wgui_inline_item_key_code = &wgui_inline_item_key_code;
S32 *wepextptr_wgui_inline_item_key_event = &wgui_inline_item_key_event;
S32 *wepextptr_wgui_inline_item_x = &wgui_inline_item_x;
S32 *wepextptr_wgui_inline_item_y = &wgui_inline_item_y;
S32 *wepextptr_wgui_inline_item_width = &wgui_inline_item_width;
S32 *wepextptr_wgui_inline_item_height = &wgui_inline_item_height;
S32 *wepextptr_wgui_current_inline_item_type = &wgui_current_inline_item_type;
U8 **wepextptr_wgui_current_history_buffer = &(wgui_current_history_buffer);
#if (KK_MTKXXXX_SWSUBVER<KK_MTKXXXX_V09A52)	//BN10123
U8 *wepextptr_inline_image_text_edit_flag = &inline_image_text_edit_flag;
#endif
U8 *wepextptr_wgui_inline_item_handled = &wgui_inline_item_handled;


wgui_inline_item_text_edit **wepextptr_current_wgui_inline_text_edit_item = &(current_wgui_inline_text_edit_item);
//U8 ***wepextptr_current_wgui_inline_text_edit_text_p = &(current_wgui_inline_text_edit_text_p);
void wepextset_current_wgui_inline_text_edit_text_p( U8 *text_p ){ *current_wgui_inline_text_edit_text_p = text_p;}
void wepextset2_current_wgui_inline_text_edit_text_p( U8 **text_p ){ current_wgui_inline_text_edit_text_p = text_p;}
U8 *wepextget_current_wgui_inline_text_edit_text_p( void ){return (*current_wgui_inline_text_edit_text_p);}
U16 wepextget2_current_wgui_inline_text_edit_text_p( void ){return (*((U16*)(*current_wgui_inline_text_edit_text_p)));}


S32 *wepextptr_n_inline_text_edit_items = &n_inline_text_edit_items;

//void (*inline_text_edit_RSK_function) (void) = inline_text_edit_RSK_function;
//void *((*wepextget_inline_text_edit_RSK_function)(void))  = &inline_text_edit_RSK_function;
void wepextset_inline_text_edit_RSK_function( void (*function)( void ) ){ inline_text_edit_RSK_function = function;}

UI_string_type *wepextptr_inline_text_edit_RSK_label_string = &inline_text_edit_RSK_label_string;
PU8 *wepextptr_inline_text_edit_RSK_label_icon= &inline_text_edit_RSK_label_icon;

UI_string_type *wepextptr_inline_full_screen_edit_saved_title = &inline_full_screen_edit_saved_title;
PU8 *wepextptr_inline_full_screen_edit_saved_title_icon = &inline_full_screen_edit_saved_title_icon;

UI_string_type *wepextptr_inline_full_screen_edit_saved_LSK_label = &inline_full_screen_edit_saved_LSK_label;
UI_string_type *wepextptr_inline_full_screen_edit_saved_RSK_label = &inline_full_screen_edit_saved_RSK_label;
#if 1
PU8 *wepextptr_inline_full_screen_edit_saved_LSK_icon = &inline_full_screen_edit_saved_LSK_icon;
PU8 *wepextptr_inline_full_screen_edit_saved_RSK_icon = &inline_full_screen_edit_saved_RSK_icon;

S32 *wepextptr_n_inline_image_text = &n_inline_image_text;
#endif

#if 0
//get
S32 wepextget_wgui_inline_item_key_code( void ){return wgui_inline_item_key_code;}
S32 wepextget_wgui_inline_item_key_event( void ){return wgui_inline_item_key_event;}
S32 wepextget_wgui_inline_item_x( void ){return wgui_inline_item_x;}
S32 wepextget_wgui_inline_item_y( void ){return wgui_inline_item_y;}
S32 wepextget_wgui_inline_item_width( void ){return wgui_inline_item_width;}
S32 wepextget_wgui_inline_item_height( void ){return wgui_inline_item_height;}
S32 wepextget_wgui_current_inline_item_type( void ){return wgui_current_inline_item_type;}
PU8 wepextget_wgui_current_history_buffer( void ){return wgui_current_history_buffer;}
U8 wepextget_inline_image_text_edit_flag( void ){return inline_image_text_edit_flag;}
//set
void wepextset_wgui_inline_item_key_code( S32 key_code ){ wgui_inline_item_key_code = key_code;}
void wepextset_wgui_inline_item_key_event( S32 key_event ){ wgui_inline_item_key_event = key_event;}
void wepextset_wgui_inline_item_x( S32 x ){ wgui_inline_item_x = x;}
void wepextset_wgui_inline_item_y( S32 y ){ wgui_inline_item_y = y;}
void wepextset_wgui_inline_item_width( S32 width ){ wgui_inline_item_width = width;}
void wepextset_wgui_inline_item_height( S32 height ){ wgui_inline_item_height = height;}
void wepextset_wgui_current_inline_item_type( S32 type ){ wgui_current_inline_item_type = type;}
void wepextset_wgui_current_history_buffer( PU8 buffer ){ wgui_current_history_buffer = buffer;}
void wepextset_inline_image_text_edit_flag( U8 flag ){ inline_image_text_edit_flag = flag;}


//get
wgui_inline_item_text_edit *wepextget_current_wgui_inline_text_edit_item( void ){return current_wgui_inline_text_edit_item;}
PU8 *wepextget_current_wgui_inline_text_edit_text_p( void ){return current_wgui_inline_text_edit_text_p;}
S32 wepextget_n_inline_text_edit_items( void ){return n_inline_text_edit_items;}
void *wepextget_inline_text_edit_RSK_function( void ){return (void *)inline_text_edit_RSK_function;}
UI_string_type wepextget_inline_text_edit_RSK_label_string( void ){return inline_text_edit_RSK_label_string;}
PU8 wepextget_inline_text_edit_RSK_label_icon( void ){return inline_text_edit_RSK_label_icon;}
//set
void wepextset_current_wgui_inline_text_edit_item( wgui_inline_item_text_edit *item ){ current_wgui_inline_text_edit_item = item;}
void wepextset_current_wgui_inline_text_edit_text_p( PU8 *text_p ){ current_wgui_inline_text_edit_text_p = text_p;}
void wepextset_n_inline_text_edit_items( S32 items ){ n_inline_text_edit_items = items;}
void wepextset_inline_text_edit_RSK_function( void (*function)(void) ){ inline_text_edit_RSK_function = function;}
void wepextset_inline_text_edit_RSK_label_string( UI_string_type string ){ inline_text_edit_RSK_label_string = string;}
void wepextset_inline_text_edit_RSK_label_icon( PU8 icon ){ inline_text_edit_RSK_label_icon = icon;}


//get
UI_string_type wepextget_inline_full_screen_edit_saved_title( void ){return inline_full_screen_edit_saved_title;}
UI_string_type wepextget_inline_full_screen_edit_saved_LSK_label( void ){return inline_full_screen_edit_saved_LSK_label;}
UI_string_type wepextget_inline_full_screen_edit_saved_RSK_label( void ){return inline_full_screen_edit_saved_RSK_label;}
PU8 wepextget_inline_full_screen_edit_saved_LSK_icon( void ){return inline_full_screen_edit_saved_LSK_icon;}
PU8 wepextget_inline_full_screen_edit_saved_RSK_icon( void ){return inline_full_screen_edit_saved_RSK_icon;}
PU8 wepextget_inline_full_screen_edit_saved_title_icon( void ){return inline_full_screen_edit_saved_title_icon;}

//set
void wepextset_inline_full_screen_edit_saved_title( UI_string_type title ){ inline_full_screen_edit_saved_title = title;}
void wepextset_inline_full_screen_edit_saved_LSK_label( UI_string_type label ){ inline_full_screen_edit_saved_LSK_label = label;}
void wepextset_inline_full_screen_edit_saved_RSK_label( UI_string_type label ){ inline_full_screen_edit_saved_RSK_label = label;}
void wepextset_inline_full_screen_edit_saved_LSK_icon( PU8 icon ){ inline_full_screen_edit_saved_LSK_icon = icon;}
void wepextset_inline_full_screen_edit_saved_RSK_icon( PU8 icon ){ inline_full_screen_edit_saved_RSK_icon = icon;}
void wepextset_inline_full_screen_edit_saved_title_icon( PU8 icon ){ inline_full_screen_edit_saved_title_icon = icon;}


//get
S32 wepextget_n_inline_image_text( void ){return n_inline_image_text;}
//set
void wepextset_n_inline_image_text( S32 key_code ){ n_inline_image_text = key_code;}
#endif


//import procedures pointer to wep module
void wepextpfn_register_inline_fixed_list_keys( void )
{ 
	register_inline_fixed_list_keys();
}

void wepextpfn_inline_edit_prepare_mask_buffer(U8 *buffer1, U8 *buffer2)
{ 
	inline_edit_prepare_mask_buffer(buffer1, buffer2);
}

void wepextpfn_inline_edit_current_highlight_handler (S32 item_index)
{  
	inline_edit_current_highlight_handler(item_index);
}

void wepextset_inline_edit_current_highlight_handler (void (*function)(S32 item_index))
{  
	inline_edit_current_highlight_handler = function;
}

//set to update the func pointer, then exec
//void *wepextpfn_wgui_inline_edit_handle_multiline_inputbox_input( void ){ return (void *)(wgui_inline_edit_handle_multiline_inputbox_input);}
void (*wepextppfn_wgui_inline_edit_handle_multiline_inputbox_input)( void ) = wgui_inline_edit_handle_multiline_inputbox_input;
void wepextset_wgui_inline_edit_handle_multiline_inputbox_input( void )
{
	wepextppfn_wgui_inline_edit_handle_multiline_inputbox_input = wgui_inline_edit_handle_multiline_inputbox_input;
}


#else




#endif

#endif /* _KK_SUPPORT_ */
